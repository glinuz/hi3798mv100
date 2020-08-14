#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vfmw_ctrl.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	VCTRL_EventReport
	.type	VCTRL_EventReport, %function
VCTRL_EventReport:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	mov	x21, x2
	mov	w22, w3
	bl	_mcount
	cmp	w19, 15
	bhi	.L13
	ldr	x4, .LC4
	ldr	x0, [x4, w19, sxtw 3]
	cbz	x0, .L14
	ldr	w3, [x0, 40]
	cbz	w3, .L6
	cmp	w3, 1
	bne	.L15
	ldr	x3, [x4, 448]
	cbz	x3, .L16
.L9:
	mov	x2, x21
	mov	w1, w20
	mov	w0, w19
	blr	x3
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L15:
	ldr	x2, .LC11
	mov	w4, w22
	ldr	x1, .LC13
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
.L3:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L6:
	ldr	x3, [x4, 440]
	cbnz	x3, .L9
	ldr	x1, .LC8
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3
	.p2align 3
.L13:
	ldr	x3, .LC1
	mov	w2, 978
	ldr	x1, .LC3
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3
.L14:
	ldr	x3, .LC6
	mov	w2, 979
	ldr	x1, .LC3
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3
.L16:
	ldr	x1, .LC10
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3
	.size	VCTRL_EventReport, .-VCTRL_EventReport
	.align	3
.LC1:
	.xword	.LC0
	.align	3
.LC3:
	.xword	.LC2
	.align	3
.LC4:
	.xword	.LANCHOR0
	.align	3
.LC6:
	.xword	.LC5
	.align	3
.LC8:
	.xword	.LC7
	.align	3
.LC10:
	.xword	.LC9
	.align	3
.LC11:
	.xword	.LANCHOR1
	.align	3
.LC13:
	.xword	.LC12
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_Isr_0
	.type	VCTRL_Isr_0, %function
VCTRL_Isr_0:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	bl	VDMDRV_VdmIntServProc
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	VCTRL_Isr_0, .-VCTRL_Isr_0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_Scd_Isr
	.type	VCTRL_Scd_Isr, %function
VCTRL_Scd_Isr:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	bl	SM_SCDIntServeProc
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	VCTRL_Scd_Isr, .-VCTRL_Scd_Isr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_MMU_ISR
	.type	VCTRL_MMU_ISR, %function
VCTRL_MMU_ISR:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	bl	VDMDRV_MMUIntServeProc
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	VCTRL_MMU_ISR, .-VCTRL_MMU_ISR
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	VCTRL_StopTask.part.0, %function
VCTRL_StopTask.part.0:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC14
	ldr	w0, [x20, 132]
	cmp	w0, 1
	beq	.L23
	ldr	x21, .LC15
	mov	w0, 10
	mov	w19, 49
	ldr	x1, [x21, 232]
	blr	x1
	b	.L24
	.p2align 3
.L31:
	ldr	x1, [x21, 232]
	blr	x1
	subs	w19, w19, #1
	beq	.L30
.L24:
	ldr	w1, [x20, 132]
	mov	w0, 10
	cmp	w1, 1
	bne	.L31
.L23:
	str	wzr, [x20, 136]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L30:
	str	wzr, [x20, 136]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	VCTRL_StopTask.part.0, .-VCTRL_StopTask.part.0
	.align	3
.LC14:
	.xword	.LANCHOR0
	.align	3
.LC15:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	VCTRL_StartTask.part.1, %function
VCTRL_StartTask.part.1:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC16
	ldr	w0, [x20, 132]
	cmp	w0, 2
	beq	.L35
	ldr	x21, .LC17
	mov	w0, 10
	mov	w19, 49
	ldr	x1, [x21, 232]
	blr	x1
	b	.L36
	.p2align 3
.L43:
	ldr	x1, [x21, 232]
	blr	x1
	subs	w19, w19, #1
	beq	.L42
.L36:
	ldr	w1, [x20, 132]
	mov	w0, 10
	cmp	w1, 2
	bne	.L43
.L35:
	str	wzr, [x20, 136]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L42:
	str	wzr, [x20, 136]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	VCTRL_StartTask.part.1, .-VCTRL_StartTask.part.1
	.align	3
.LC16:
	.xword	.LANCHOR0
	.align	3
.LC17:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	VCTRL_SetSendStreamAgainFlag.part.2, %function
VCTRL_SetSendStreamAgainFlag.part.2:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x3, .LC18
	mov	w2, 1310
	ldr	x1, .LC19
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x29, x30, [sp], 16
	ret
	.size	VCTRL_SetSendStreamAgainFlag.part.2, .-VCTRL_SetSendStreamAgainFlag.part.2
	.align	3
.LC18:
	.xword	.LC5
	.align	3
.LC19:
	.xword	.LC2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_Suspend
	.type	VCTRL_Suspend, %function
VCTRL_Suspend:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x0, x30
	bl	_mcount
	ldr	x21, .LC20
	ldr	x0, [x21]
	blr	x0
	ldr	x24, .LC21
	mov	w25, w0
	ldr	x1, [x24]
	cbz	x1, .L46
	mov	w0, 0
	blr	x1
.L47:
	mov	w0, 0
	mov	w20, 32
	mov	w22, 0
	mov	w19, 0
	bl	SCDDRV_PrepareSleep
	mov	w0, 0
	bl	VDMDRV_PrepareSleep
	ldr	x0, [x21]
	blr	x0
	mov	w26, w0
	b	.L58
	.p2align 3
.L48:
	bl	VDMDRV_GetSleepStage
	cmp	w0, 2
	mov	w19, 1
	mov	w23, w19
	beq	.L51
.L82:
	tst	w23, w22
	mov	w0, 10
	bne	.L51
	subs	w20, w20, #1
	beq	.L83
	ldr	x1, [x21, 232]
	blr	x1
	cmp	w19, 1
	ccmp	w22, 1, 0, eq
	beq	.L51
.L58:
	mov	w0, 0
	bl	SCDDRV_GetSleepStage
	cmp	w0, 2
	mov	w0, 0
	beq	.L48
	bl	VDMDRV_GetSleepStage
	cmp	w0, 2
	mov	w23, w19
	bne	.L82
	mov	w22, 1
	b	.L82
	.p2align 3
.L83:
	ldr	x0, [x21]
	blr	x0
	cmp	w26, w0
	sub	w2, w0, w26
	bls	.L54
	mvn	w26, w26
	add	w2, w26, w0
.L54:
	ldr	x1, .LC25
	mov	w4, w22
	mov	w3, w19
	mov	w0, 0
	bl	dprint_vfmw
	cbnz	w19, .L70
	ldr	x1, .LC31
	mov	w2, 0
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	bl	SCDDRV_ForceSleep
.L70:
	cbnz	w22, .L51
	ldr	x1, .LC33
	mov	w2, 0
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	bl	VDMDRV_ForceSleep
.L51:
	ldr	x0, .LC26
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L84
.L60:
	ldr	x0, .LC27
	add	x1, x0, 128
	ldr	w0, [x0, 128]
	cmp	w0, 1
	beq	.L85
.L67:
	ldr	x0, [x21]
	blr	x0
	cmp	w25, w0
	sub	w2, w0, w25
	bls	.L69
	mvn	w25, w25
	add	w2, w25, w0
.L69:
	ldr	x1, .LC29
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L84:
	ldr	x1, [x24, 64]
	cbz	x1, .L61
	mov	w0, 0
	blr	x1
.L61:
	ldr	x1, [x24, 48]
	cbz	x1, .L62
	mov	w0, 0
	blr	x1
	ldr	x1, [x24, 16]
	cbz	x1, .L64
.L86:
	mov	w0, 0
	blr	x1
.L65:
	ldr	x1, [x24, 80]
	cbz	x1, .L60
	mov	w0, 0
	blr	x1
	ldr	x0, .LC27
	add	x1, x0, 128
	ldr	w0, [x0, 128]
	cmp	w0, 1
	bne	.L67
	.p2align 2
.L85:
	mov	w0, 2
	str	w0, [x1, 8]
	bl	VCTRL_StopTask.part.0
	b	.L67
.L62:
	ldr	x1, .LC23
	mov	w2, 382
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, [x24, 16]
	cbnz	x1, .L86
	.p2align 2
.L64:
	ldr	x1, .LC23
	mov	w2, 387
	mov	w0, 0
	bl	dprint_vfmw
	b	.L65
	.p2align 3
.L46:
	ldr	x1, .LC23
	mov	w2, 280
	mov	w0, 0
	bl	dprint_vfmw
	b	.L47
	.size	VCTRL_Suspend, .-VCTRL_Suspend
	.align	3
.LC20:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC21:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC23:
	.xword	.LC22
	.align	3
.LC25:
	.xword	.LC24
	.align	3
.LC26:
	.xword	g_HalDisable
	.align	3
.LC27:
	.xword	.LANCHOR0
	.align	3
.LC29:
	.xword	.LC28
	.align	3
.LC31:
	.xword	.LC30
	.align	3
.LC33:
	.xword	.LC32
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_Resume
	.type	VCTRL_Resume, %function
VCTRL_Resume:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC35
	ldr	x0, [x20]
	blr	x0
	mov	w19, w0
	ldr	x0, .LC36
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L107
.L88:
	mov	w0, 0
	bl	SCDDRV_ExitSleep
	mov	w0, 0
	bl	VDMDRV_ExitSleep
	ldr	x0, .LC39
	add	x1, x0, 128
	ldr	w0, [x0, 128]
	cmp	w0, 1
	beq	.L108
.L98:
	ldr	x0, [x20]
	blr	x0
	cmp	w19, w0
	sub	w2, w0, w19
	bls	.L100
	mvn	w2, w19
	add	w2, w2, w0
.L100:
	ldr	x1, .LC41
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L107:
	ldr	x21, .LC37
	ldr	x1, [x21, 8]
	cbz	x1, .L109
	mov	w0, 0
	blr	x1
.L90:
	ldr	x1, [x21, 40]
	cbz	x1, .L110
	mov	w0, 0
	blr	x1
.L92:
	ldr	x1, [x21, 56]
	cbz	x1, .L93
	mov	w0, 0
	blr	x1
.L93:
	ldr	x1, [x21, 72]
	cbz	x1, .L94
	mov	w0, 0
	blr	x1
.L94:
	ldr	x1, [x21, 144]
	cbz	x1, .L95
	mov	w0, 0
	blr	x1
.L96:
	ldr	x0, [x21, 152]
	cbz	x0, .L97
	blr	x0
	b	.L88
	.p2align 3
.L108:
	str	w0, [x1, 8]
	bl	VCTRL_StartTask.part.1
	b	.L98
	.p2align 3
.L110:
	ldr	x1, .LC38
	mov	w2, 432
	mov	w0, 0
	bl	dprint_vfmw
	b	.L92
	.p2align 3
.L95:
	ldr	x1, .LC38
	mov	w2, 439
	mov	w0, 0
	bl	dprint_vfmw
	b	.L96
	.p2align 3
.L97:
	ldr	x1, .LC38
	mov	w2, 440
	mov	w0, 0
	bl	dprint_vfmw
	b	.L88
	.p2align 3
.L109:
	ldr	x1, .LC38
	mov	w2, 427
	mov	w0, 0
	bl	dprint_vfmw
	b	.L90
	.size	VCTRL_Resume, .-VCTRL_Resume
	.align	3
.LC35:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC36:
	.xword	g_HalDisable
	.align	3
.LC37:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC38:
	.xword	.LC22
	.align	3
.LC39:
	.xword	.LANCHOR0
	.align	3
.LC41:
	.xword	.LC40
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ChanCtx_Init
	.type	ChanCtx_Init, %function
ChanCtx_Init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC46
	ldr	x0, .LC45
	mov	x2, 192
	mov	w1, 0
	ldr	x3, [x0, 96]
	mov	x0, x19
	blr	x3
	add	x0, x19, 64
	add	x1, x19, 128
	mov	w2, -1
	.p2align 2
.L112:
	str	w2, [x0], 4
	cmp	x0, x1
	bne	.L112
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	ChanCtx_Init, .-ChanCtx_Init
	.align	3
.LC45:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC46:
	.xword	g_ChanCtx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_Isr_1
	.type	VCTRL_Isr_1, %function
VCTRL_Isr_1:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 1
	bl	VDMDRV_VdmIntServProc
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	VCTRL_Isr_1, .-VCTRL_Isr_1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_Isr_2
	.type	VCTRL_Isr_2, %function
VCTRL_Isr_2:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 2
	bl	VDMDRV_VdmIntServProc
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	VCTRL_Isr_2, .-VCTRL_Isr_2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_Scd_Isr_1
	.type	VCTRL_Scd_Isr_1, %function
VCTRL_Scd_Isr_1:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 1
	bl	SM_SCDIntServeProc
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	VCTRL_Scd_Isr_1, .-VCTRL_Scd_Isr_1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_Scd_Isr_2
	.type	VCTRL_Scd_Isr_2, %function
VCTRL_Scd_Isr_2:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 2
	bl	SM_SCDIntServeProc
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	VCTRL_Scd_Isr_2, .-VCTRL_Scd_Isr_2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_InformVdmFree
	.type	VCTRL_InformVdmFree, %function
VCTRL_InformVdmFree:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC49
	mov	w0, 0
	ldr	x1, [x1, 248]
	blr	x1
	ldp	x29, x30, [sp], 16
	ret
	.size	VCTRL_InformVdmFree, .-VCTRL_InformVdmFree
	.align	3
.LC49:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ExtraWakeUpThread
	.type	VCTRL_ExtraWakeUpThread, %function
VCTRL_ExtraWakeUpThread:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC50
	mov	w0, 0
	ldr	x1, [x1, 248]
	blr	x1
	ldp	x29, x30, [sp], 16
	ret
	.size	VCTRL_ExtraWakeUpThread, .-VCTRL_ExtraWakeUpThread
	.align	3
.LC50:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetMoreGapEnable
	.type	VCTRL_SetMoreGapEnable, %function
VCTRL_SetMoreGapEnable:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cmp	w19, 15
	bhi	.L126
	ldr	x0, .LC54
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L127
	ldr	w1, [x0, 12]
	cmp	w1, 1
	bne	.L128
	cmp	w20, 1
	bhi	.L125
	str	w20, [x0, 1280]
	mov	w0, 0
.L122:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L125:
	str	wzr, [x0, 1280]
	mov	w0, -1
	b	.L122
	.p2align 3
.L128:
	ldr	x3, .LC57
	mov	w2, 911
	ldr	x1, .LC53
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L122
	.p2align 3
.L126:
	ldr	x3, .LC52
	mov	w2, 909
	ldr	x1, .LC53
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L122
.L127:
	ldr	x3, .LC55
	mov	w2, 910
	ldr	x1, .LC53
	bl	dprint_vfmw
	mov	w0, -1
	b	.L122
	.size	VCTRL_SetMoreGapEnable, .-VCTRL_SetMoreGapEnable
	.align	3
.LC52:
	.xword	.LC51
	.align	3
.LC53:
	.xword	.LC2
	.align	3
.LC54:
	.xword	.LANCHOR0
	.align	3
.LC55:
	.xword	.LC5
	.align	3
.LC57:
	.xword	.LC56
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetCallBack
	.type	VCTRL_SetCallBack, %function
VCTRL_SetCallBack:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	cbz	x20, .L137
	cbz	w19, .L133
	cmp	w19, 1
	beq	.L134
	ldr	x2, .LC62
	mov	w3, w19
	ldr	x1, .LC64
	mov	w0, 0
	add	x2, x2, 24
	bl	dprint_vfmw
	mov	w0, -1
.L131:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L134:
	ldr	x1, .LC61
	mov	w0, 0
	str	x20, [x1, 448]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L133:
	ldr	x1, .LC61
	mov	w0, 0
	str	x20, [x1, 440]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L137:
	ldr	x3, .LC59
	mov	w2, 1020
	ldr	x1, .LC60
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L131
	.size	VCTRL_SetCallBack, .-VCTRL_SetCallBack
	.align	3
.LC59:
	.xword	.LC58
	.align	3
.LC60:
	.xword	.LC2
	.align	3
.LC61:
	.xword	.LANCHOR0
	.align	3
.LC62:
	.xword	.LANCHOR1
	.align	3
.LC64:
	.xword	.LC63
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_StopTask
	.type	VCTRL_StopTask, %function
VCTRL_StopTask:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC65
	add	x1, x0, 128
	ldr	w0, [x0, 128]
	cmp	w0, 1
	bne	.L140
	mov	w0, 2
	str	w0, [x1, 8]
	bl	VCTRL_StopTask.part.0
.L139:
	ldp	x29, x30, [sp], 16
	ret
	.p2align 3
.L140:
	mov	w0, -1
	b	.L139
	.size	VCTRL_StopTask, .-VCTRL_StopTask
	.align	3
.LC65:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_StartTask
	.type	VCTRL_StartTask, %function
VCTRL_StartTask:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC67
	add	x1, x0, 128
	ldr	w0, [x0, 128]
	cmp	w0, 1
	bne	.L143
	str	w0, [x1, 8]
	bl	VCTRL_StartTask.part.1
.L142:
	ldp	x29, x30, [sp], 16
	ret
	.p2align 3
.L143:
	mov	w0, -1
	b	.L142
	.size	VCTRL_StartTask, .-VCTRL_StartTask
	.align	3
.LC67:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetStreamInterface
	.type	VCTRL_SetStreamInterface, %function
VCTRL_SetStreamInterface:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w21, w0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	cbz	x20, .L148
	ldr	x19, .LC72
	sxtw	x22, w21
	ldr	x0, [x19, w21, sxtw 3]
	cbz	x0, .L149
	ldr	x21, .LC74
	mov	x2, 24
	mov	x1, x20
	add	x0, x0, 176
	add	x19, x19, 480
	ldr	x3, [x21, 104]
	blr	x3
	lsl	x0, x22, 2
	ldr	x3, [x21, 104]
	sub	x0, x0, x22
	mov	x2, 24
	mov	x1, x20
	add	x0, x19, x0, lsl 3
	blr	x3
	mov	w0, 0
.L146:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L149:
	ldr	x3, .LC73
	mov	w2, 1297
	ldr	x1, .LC71
	bl	dprint_vfmw
	mov	w0, -1
	b	.L146
.L148:
	ldr	x3, .LC70
	mov	w2, 1296
	ldr	x1, .LC71
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L146
	.size	VCTRL_SetStreamInterface, .-VCTRL_SetStreamInterface
	.align	3
.LC70:
	.xword	.LC69
	.align	3
.LC71:
	.xword	.LC2
	.align	3
.LC72:
	.xword	.LANCHOR0
	.align	3
.LC73:
	.xword	.LC5
	.align	3
.LC74:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetSendStreamAgainFlag
	.type	VCTRL_SetSendStreamAgainFlag, %function
VCTRL_SetSendStreamAgainFlag:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	x0, .LC76
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L153
	add	x2, x0, 290816
	mov	w0, 0
	str	w20, [x2, 2484]
.L152:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L153:
	bl	VCTRL_SetSendStreamAgainFlag.part.2
	b	.L152
	.size	VCTRL_SetSendStreamAgainFlag, .-VCTRL_SetSendStreamAgainFlag
	.align	3
.LC76:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetSendStreamAgainFlag
	.type	VCTRL_GetSendStreamAgainFlag, %function
VCTRL_GetSendStreamAgainFlag:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC77
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L157
	add	x0, x0, 290816
	ldr	w0, [x0, 2484]
.L156:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L157:
	ldr	x3, .LC78
	mov	w2, 1317
	ldr	x1, .LC79
	bl	dprint_vfmw
	mov	w0, -1
	b	.L156
	.size	VCTRL_GetSendStreamAgainFlag, .-VCTRL_GetSendStreamAgainFlag
	.align	3
.LC77:
	.xword	.LANCHOR0
	.align	3
.LC78:
	.xword	.LC5
	.align	3
.LC79:
	.xword	.LC2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetStreamInterface
	.type	VCTRL_GetStreamInterface, %function
VCTRL_GetStreamInterface:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC80
	sxtw	x2, w19
	ldr	x1, [x20, w19, sxtw 3]
	cbz	x1, .L161
	ldr	x3, .LC81
	lsl	x0, x2, 2
	sub	x0, x0, x2
	add	x4, x20, 480
	mov	x2, 24
	add	x1, x1, 176
	ldr	x3, [x3, 120]
	add	x0, x4, x0, lsl 3
	blr	x3
	cbnz	w0, .L162
	ldr	x0, [x20, w19, sxtw 3]
	add	x0, x0, 176
.L159:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L161:
	mov	x0, 0
	b	.L159
	.p2align 3
.L162:
	ldr	x1, .LC83
	mov	w3, w19
	mov	w2, 1335
	mov	w0, 0
	bl	dprint_vfmw
	mov	x0, 0
	b	.L159
	.size	VCTRL_GetStreamInterface, .-VCTRL_GetStreamInterface
	.align	3
.LC80:
	.xword	.LANCHOR0
	.align	3
.LC81:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC83:
	.xword	.LC82
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetFrameInterface
	.type	VCTRL_SetFrameInterface, %function
VCTRL_SetFrameInterface:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L167
	ldr	x0, .LC88
	ldr	x0, [x0, w20, sxtw 3]
	cbz	x0, .L168
	ldr	x3, .LC90
	mov	x2, 24
	mov	x1, x19
	add	x0, x0, 224
	ldr	x3, [x3, 104]
	blr	x3
	mov	w0, 0
.L165:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L168:
	ldr	x3, .LC89
	mov	w2, 1349
	ldr	x1, .LC87
	bl	dprint_vfmw
	mov	w0, -1
	b	.L165
.L167:
	ldr	x3, .LC86
	mov	w2, 1348
	ldr	x1, .LC87
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L165
	.size	VCTRL_SetFrameInterface, .-VCTRL_SetFrameInterface
	.align	3
.LC86:
	.xword	.LC85
	.align	3
.LC87:
	.xword	.LC2
	.align	3
.LC88:
	.xword	.LANCHOR0
	.align	3
.LC89:
	.xword	.LC5
	.align	3
.LC90:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetFrameInterface
	.type	VCTRL_GetFrameInterface, %function
VCTRL_GetFrameInterface:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC91
	ldr	x1, [x0, w19, sxtw 3]
	ldr	x19, [sp, 16]
	add	x0, x1, 224
	cmp	x1, xzr
	csel	x0, x0, xzr, ne
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_GetFrameInterface, .-VCTRL_GetFrameInterface
	.align	3
.LC91:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetStreamSize
	.type	VCTRL_GetStreamSize, %function
VCTRL_GetStreamSize:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	w19, 15
	bhi	.L177
	ldr	x0, .LC94
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L178
	ldr	w2, [x0, 12]
	cmp	w2, 1
	bne	.L179
	mov	x2, 34464
	mov	x1, x20
	movk	x2, 0x1, lsl 16
	add	x0, x0, x2
	bl	GetSegStreamSize
.L174:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L179:
	ldr	x3, .LC98
	mov	w2, 1390
	ldr	x1, .LC93
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L174
	.p2align 3
.L177:
	ldr	x3, .LC92
	mov	w2, 1388
	ldr	x1, .LC93
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L174
.L178:
	ldr	x3, .LC96
	mov	w2, 1389
	ldr	x1, .LC93
	bl	dprint_vfmw
	mov	w0, -1
	b	.L174
	.size	VCTRL_GetStreamSize, .-VCTRL_GetStreamSize
	.align	3
.LC92:
	.xword	.LC51
	.align	3
.LC93:
	.xword	.LC2
	.align	3
.LC94:
	.xword	.LANCHOR0
	.align	3
.LC96:
	.xword	.LC95
	.align	3
.LC98:
	.xword	.LC97
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_CheckFreshRaw
	.type	VCTRL_CheckFreshRaw, %function
VCTRL_CheckFreshRaw:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	stp	wzr, wzr, [x29, 40]
	bhi	.L187
	ldr	x0, .LC101
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L188
	ldr	w1, [x0, 12]
	cmp	w1, 1
	bne	.L189
	add	x19, x0, 1672
	add	x1, x29, 40
	mov	x0, x19
	bl	GetRawStreamNum
	cbnz	w0, .L186
	add	x1, x29, 44
	mov	x0, x19
	bl	GetRawStreamSize
	cbnz	w0, .L186
	ldr	x1, .LC105
	mov	w0, 22
	ldp	w2, w3, [x29, 40]
	bl	dprint_vfmw
	ldr	w0, [x29, 40]
	cmp	w0, wzr
	ble	.L186
	ldr	w0, [x29, 44]
	cmp	w0, 3
	csetm	w0, le
.L182:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L186:
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L189:
	ldr	x3, .LC103
	mov	w2, 1407
	ldr	x1, .LC100
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L182
	.p2align 3
.L187:
	ldr	x3, .LC99
	mov	w2, 1405
	ldr	x1, .LC100
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L182
.L188:
	ldr	x3, .LC102
	mov	w2, 1406
	ldr	x1, .LC100
	bl	dprint_vfmw
	mov	w0, -1
	b	.L182
	.size	VCTRL_CheckFreshRaw, .-VCTRL_CheckFreshRaw
	.align	3
.LC99:
	.xword	.LC51
	.align	3
.LC100:
	.xword	.LC2
	.align	3
.LC101:
	.xword	.LANCHOR0
	.align	3
.LC102:
	.xword	.LC95
	.align	3
.LC103:
	.xword	.LC97
	.align	3
.LC105:
	.xword	.LC104
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_CheckFreshSeg
	.type	VCTRL_CheckFreshSeg, %function
VCTRL_CheckFreshSeg:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	stp	wzr, wzr, [x29, 40]
	bhi	.L195
	ldr	x0, .LC108
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L196
	ldr	w1, [x0, 12]
	cmp	w1, 1
	bne	.L197
	mov	x3, 34464
	add	x2, x29, 44
	movk	x3, 0x1, lsl 16
	add	x1, x29, 40
	add	x0, x0, x3
	bl	GetFreshSegStream
	ldr	x1, .LC112
	mov	w0, 22
	ldp	w2, w3, [x29, 40]
	bl	dprint_vfmw
	ldr	w0, [x29, 40]
	cmp	w0, wzr
	csetm	w0, le
.L192:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L197:
	ldr	x3, .LC110
	mov	w2, 1433
	ldr	x1, .LC107
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L192
	.p2align 3
.L195:
	ldr	x3, .LC106
	mov	w2, 1431
	ldr	x1, .LC107
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L192
.L196:
	ldr	x3, .LC109
	mov	w2, 1432
	ldr	x1, .LC107
	bl	dprint_vfmw
	mov	w0, -1
	b	.L192
	.size	VCTRL_CheckFreshSeg, .-VCTRL_CheckFreshSeg
	.align	3
.LC106:
	.xword	.LC51
	.align	3
.LC107:
	.xword	.LC2
	.align	3
.LC108:
	.xword	.LANCHOR0
	.align	3
.LC109:
	.xword	.LC95
	.align	3
.LC110:
	.xword	.LC97
	.align	3
.LC112:
	.xword	.LC111
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetDiscardPicParam
	.type	VCTRL_SetDiscardPicParam, %function
VCTRL_SetDiscardPicParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	w19, 15
	bhi	.L204
	cbz	x20, .L205
	ldr	x21, .LC117
	ldr	x0, [x21, w19, sxtw 3]
	cbz	x0, .L206
	ldr	x3, .LC119
	mov	x1, x20
	mov	x2, 8
	add	x0, x0, 1544
	ldr	x3, [x3, 104]
	blr	x3
	ldr	x1, [x21, w19, sxtw 3]
	ldr	w0, [x1, 1544]
	sub	w0, w0, #1
	cmp	w0, 1
	bls	.L203
	mov	w0, 0
	str	wzr, [x1, 1544]
.L200:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L203:
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L204:
	ldr	x3, .LC113
	mov	w2, 1454
	ldr	x1, .LC114
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L200
.L206:
	ldr	x3, .LC118
	mov	w2, 1456
	ldr	x1, .LC114
	bl	dprint_vfmw
	mov	w0, -1
	b	.L200
.L205:
	ldr	x3, .LC116
	mov	w2, 1455
	ldr	x1, .LC114
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L200
	.size	VCTRL_SetDiscardPicParam, .-VCTRL_SetDiscardPicParam
	.align	3
.LC113:
	.xword	.LC51
	.align	3
.LC114:
	.xword	.LC2
	.align	3
.LC116:
	.xword	.LC115
	.align	3
.LC117:
	.xword	.LANCHOR0
	.align	3
.LC118:
	.xword	.LC5
	.align	3
.LC119:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetSyntaxCtx
	.type	VCTRL_GetSyntaxCtx, %function
VCTRL_GetSyntaxCtx:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC120
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L209
	add	x0, x0, 294912
	add	x0, x0, 2576
.L208:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L209:
	mov	x0, 0
	b	.L208
	.size	VCTRL_GetSyntaxCtx, .-VCTRL_GetSyntaxCtx
	.align	3
.LC120:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetImageInterface
	.type	VCTRL_GetImageInterface, %function
VCTRL_GetImageInterface:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC121
	ldr	x1, [x0, w19, sxtw 3]
	ldr	x19, [sp, 16]
	add	x0, x1, 200
	cmp	x1, xzr
	csel	x0, x0, xzr, ne
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_GetImageInterface, .-VCTRL_GetImageInterface
	.align	3
.LC121:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_PartitionMemForStdUser
	.type	VCTRL_PartitionMemForStdUser, %function
VCTRL_PartitionMemForStdUser:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	w20, w0
	mov	x0, x30
	mov	w23, w1
	mov	w21, w2
	mov	w25, w3
	mov	w26, w4
	bl	_mcount
	ldr	x19, .LC122
	cmp	w23, 31
	ccmp	w21, 31, 4, gt
	ldr	x22, [x19, w20, sxtw 3]
	ble	.L225
	cmp	w25, wzr
	ccmp	w26, wzr, 4, ne
	beq	.L226
	ldr	x24, .LC127
	mov	x2, 32
	mov	w1, 0
	add	x0, x29, 96
	ldr	x3, [x24, 96]
	blr	x3
	strb	wzr, [x29, 97]
	mov	w2, 1
	add	x1, x29, 96
	mov	w0, w20
	strb	wzr, [x29, 99]
	stp	w23, w21, [x29, 104]
	stp	w23, w21, [x29, 120]
	strb	wzr, [x29, 101]
	strb	wzr, [x29, 102]
	strb	w2, [x29, 98]
	bl	FSP_ConfigInstance
	mov	w21, w0
	cbnz	w0, .L217
	add	x3, x29, 92
	mov	w2, w26
	mov	w1, w25
	mov	w0, w20
	bl	FSP_ConfigFrameBuf
	cbnz	w0, .L227
	add	x23, x19, 864
	mov	w0, w20
	mov	x1, x23
	bl	FSP_GetInstInfo
	ldrsb	w0, [x23, 1]
	mov	w1, 16
	cmp	w0, w1
	csel	w0, w0, w1, le
	str	w0, [x22, 592]
	cmp	w0, 2
	ble	.L228
	ldr	w0, [x23, 3208]
	add	x20, x22, 524
	ldr	x3, [x24, 96]
	mov	x2, 64
	mov	w1, 0
	str	w0, [x22, 588]
	mov	x0, x20
	blr	x3
	ldr	x3, [x24, 96]
	mov	x2, 64
	mov	w1, 0
	add	x0, x22, 460
	blr	x3
	ldr	w0, [x22, 592]
	cmp	w0, wzr
	ble	.L215
	sub	w4, w0, #1
	mov	w3, 100
	mov	x2, 124
	add	x1, x19, 888
	mov	x0, x20
	umaddl	x2, w4, w3, x2
	add	x3, x23, x2
	.p2align 2
.L222:
	ldr	w2, [x1], 100
	str	w2, [x0], 4
	cmp	x1, x3
	bne	.L222
.L215:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 128
	ret
.L226:
	ldr	x1, .LC126
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L215
.L217:
	ldr	x1, .LC134
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L215
.L227:
	ldr	x1, .LC129
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L215
.L228:
	ldr	x2, .LC130
	mov	w3, 1697
	ldr	x1, .LC132
	mov	w0, 0
	add	x2, x2, 48
	mov	w21, -1
	bl	dprint_vfmw
	b	.L215
.L225:
	ldr	x1, .LC124
	mov	w3, w21
	mov	w2, w23
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L215
	.size	VCTRL_PartitionMemForStdUser, .-VCTRL_PartitionMemForStdUser
	.align	3
.LC122:
	.xword	.LANCHOR0
	.align	3
.LC124:
	.xword	.LC123
	.align	3
.LC126:
	.xword	.LC125
	.align	3
.LC127:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC129:
	.xword	.LC128
	.align	3
.LC130:
	.xword	.LANCHOR1
	.align	3
.LC132:
	.xword	.LC131
	.align	3
.LC134:
	.xword	.LC133
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_UnInstallChan
	.type	VCTRL_UnInstallChan, %function
VCTRL_UnInstallChan:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	mov	w0, w19
	bl	FSP_DisableInstance
	mov	w0, w19
	bl	CloseUsdPool_WithMem
	ldr	x19, [sp, 16]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_UnInstallChan, .-VCTRL_UnInstallChan
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DelAllFrameMemRecord
	.type	DelAllFrameMemRecord, %function
DelAllFrameMemRecord:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x0, x30
	add	x20, x1, 2304
	bl	_mcount
	cbz	x19, .L248
	.p2align 2
.L243:
	ldr	w0, [x19]
	cmp	w0, 255
	beq	.L233
	ldr	w2, [x19, 12]
	ldr	w0, [x19, 32]
	ldr	x1, [x19, 24]
	bl	MEM_DelMemRecord
.L233:
	ldr	w0, [x19, 4]
	cmp	w0, 255
	beq	.L234
	ldr	w2, [x19, 16]
	ldr	w0, [x19, 48]
	ldr	x1, [x19, 40]
	bl	MEM_DelMemRecord
.L234:
	ldr	w0, [x19, 8]
	cmp	w0, 255
	beq	.L235
	ldr	w2, [x19, 20]
	ldr	w0, [x19, 64]
	ldr	x1, [x19, 56]
	bl	MEM_DelMemRecord
.L235:
	add	x19, x19, 72
	cmp	x19, x20
	bne	.L243
	mov	w0, 0
.L232:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L248:
	ldr	x3, .LC139
	mov	w2, 1754
	ldr	x1, .LC140
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L232
	.size	DelAllFrameMemRecord, .-DelAllFrameMemRecord
	.align	3
.LC139:
	.xword	.LC138
	.align	3
.LC140:
	.xword	.LC2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_FreeChanResource
	.type	VCTRL_FreeChanResource, %function
VCTRL_FreeChanResource:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L273
	ldr	x20, [x19]
	cbz	x20, .L274
	ldr	w0, [x19, 32]
	cmp	w0, 1
	beq	.L254
	ldr	w0, [x20, 16]
	ldr	w2, [x20, 24]
	ldr	x1, [x20, 8]
	bl	MEM_DelMemRecord
.L255:
	ldr	w0, [x19, 36]
	cmp	w0, 1
	beq	.L275
.L253:
	ldr	x20, [x19, 8]
	cbz	x20, .L276
	ldr	w0, [x20, 16]
	ldr	w2, [x20, 24]
	ldr	x1, [x20, 8]
	bl	MEM_DelMemRecord
	ldr	w0, [x19, 40]
	cmp	w0, 1
	beq	.L277
.L258:
	ldr	x20, [x19, 16]
	cbz	x20, .L278
.L259:
	ldr	w0, [x20, 16]
	ldr	w2, [x20, 24]
	ldr	x1, [x20, 8]
	bl	MEM_DelMemRecord
	ldr	w0, [x19, 44]
	cmp	w0, 1
	bne	.L260
	ldr	w0, [x20, 16]
	cbnz	w0, .L279
.L260:
	ldr	x1, .LC154
	ldr	x0, [x20, 8]
	ldr	x1, [x1, 320]
	blr	x1
	mov	w0, 0
.L251:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L254:
	ldr	x1, [x19, 24]
	cbz	x1, .L280
	mov	w0, w21
	bl	DelAllFrameMemRecord
	cbz	w0, .L255
	ldr	x1, .LC149
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x19, 36]
	cmp	w0, 1
	bne	.L253
	.p2align 2
.L275:
	ldr	w0, [x20, 16]
	cbz	w0, .L253
	ldr	x1, [x20, 8]
	bl	MEM_ReleaseMemBlock
	b	.L253
	.p2align 3
.L277:
	ldr	w0, [x20, 16]
	cbz	w0, .L258
	ldr	x1, [x20, 8]
	bl	MEM_ReleaseMemBlock
	b	.L258
	.p2align 3
.L276:
	ldr	x2, .LC141
	mov	w0, 0
	ldr	x1, .LC151
	add	x2, x2, 80
	bl	dprint_vfmw
	ldr	x20, [x19, 16]
	cbnz	x20, .L259
.L278:
	ldr	x2, .LC141
	mov	w0, 0
	ldr	x1, .LC153
	add	x2, x2, 80
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L279:
	ldr	x1, [x20, 8]
	bl	MEM_ReleaseMemBlock
	mov	w0, 0
	b	.L251
	.p2align 3
.L280:
	ldr	x2, .LC141
	mov	w0, 0
	ldr	x1, .LC147
	add	x2, x2, 80
	bl	dprint_vfmw
	b	.L255
	.p2align 3
.L274:
	ldr	x2, .LC141
	mov	w0, 0
	ldr	x1, .LC145
	add	x2, x2, 80
	bl	dprint_vfmw
	b	.L253
.L273:
	ldr	x2, .LC141
	mov	w0, 0
	ldr	x1, .LC143
	add	x2, x2, 80
	bl	dprint_vfmw
	mov	w0, -1
	b	.L251
	.size	VCTRL_FreeChanResource, .-VCTRL_FreeChanResource
	.align	3
.LC141:
	.xword	.LANCHOR1
	.align	3
.LC143:
	.xword	.LC142
	.align	3
.LC145:
	.xword	.LC144
	.align	3
.LC147:
	.xword	.LC146
	.align	3
.LC149:
	.xword	.LC148
	.align	3
.LC151:
	.xword	.LC150
	.align	3
.LC153:
	.xword	.LC152
	.align	3
.LC154:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanCtxSize
	.type	VCTRL_GetChanCtxSize, %function
VCTRL_GetChanCtxSize:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	w19, 26
	beq	.L284
	mov	w0, 11640
	movk	w0, 0xc, lsl 16
	str	w0, [x20]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L284:
	mov	w0, 1800
	movk	w0, 0xb2, lsl 16
	str	w0, [x20]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_GetChanCtxSize, .-VCTRL_GetChanCtxSize
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanCfg
	.type	VCTRL_GetChanCfg, %function
VCTRL_GetChanCfg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	w19, 15
	bhi	.L290
	cbz	x20, .L291
	ldr	x0, .LC159
	ldr	x1, [x0, w19, sxtw 3]
	cbz	x1, .L292
	ldr	x3, .LC161
	mov	x2, 120
	add	x1, x1, 52
	mov	x0, x20
	ldr	x3, [x3, 104]
	blr	x3
	mov	w0, 0
.L287:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L290:
	ldr	x3, .LC155
	mov	w2, 3052
	ldr	x1, .LC156
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L287
.L292:
	ldr	x3, .LC160
	mov	w2, 3054
	ldr	x1, .LC156
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L287
.L291:
	ldr	x3, .LC158
	mov	w2, 3053
	ldr	x1, .LC156
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L287
	.size	VCTRL_GetChanCfg, .-VCTRL_GetChanCfg
	.align	3
.LC155:
	.xword	.LC51
	.align	3
.LC156:
	.xword	.LC2
	.align	3
.LC158:
	.xword	.LC157
	.align	3
.LC159:
	.xword	.LANCHOR0
	.align	3
.LC160:
	.xword	.LC5
	.align	3
.LC161:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_CmpConfigParameter
	.type	VCTRL_CmpConfigParameter, %function
VCTRL_CmpConfigParameter:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x0, .LC162
	ldr	x2, [x0, w20, sxtw 3]
	ldr	w0, [x2, 672]
	cmp	w0, 21
	beq	.L309
	ldr	w0, [x2, 52]
	ldr	w1, [x19]
	cmp	w0, w1
	bne	.L309
	ldr	w3, [x2, 72]
	ldr	w1, [x19, 20]
	cmp	w3, w1
	bne	.L309
	ldr	w3, [x2, 68]
	ldr	w1, [x19, 16]
	cmp	w3, w1
	bne	.L309
	ldr	w3, [x2, 76]
	ldr	w1, [x19, 24]
	cmp	w3, w1
	bne	.L309
	ldr	w3, [x2, 80]
	ldr	w1, [x19, 28]
	cmp	w3, w1
	bne	.L309
	ldr	w3, [x2, 84]
	ldr	w1, [x19, 32]
	cmp	w3, w1
	bne	.L309
	ldr	w3, [x2, 88]
	ldr	w1, [x19, 36]
	cmp	w3, w1
	bne	.L309
	ldr	w3, [x2, 112]
	ldr	w1, [x19, 60]
	cmp	w3, w1
	bne	.L309
	ldr	w3, [x2, 128]
	ldr	w1, [x19, 76]
	cmp	w3, w1
	bne	.L309
	ldr	w3, [x2, 124]
	ldr	w1, [x19, 72]
	cmp	w3, w1
	bne	.L309
	ldr	w3, [x2, 116]
	ldr	w1, [x19, 64]
	cmp	w3, w1
	bne	.L309
	ldr	w3, [x2, 132]
	ldr	w1, [x19, 80]
	cmp	w3, w1
	bne	.L309
	cmp	w0, 1
	beq	.L311
	sub	w1, w0, #10
	mov	w0, 0
	cmp	w1, 2
	bhi	.L294
	ldr	w0, [x19, 4]
	ldr	w1, [x2, 56]
	ldp	x19, x20, [sp, 16]
	cmp	w1, w0
	csetm	w0, ne
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L309:
	mov	w0, -1
.L294:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L311:
	ldr	w1, [x2, 56]
	ldr	w0, [x19, 4]
	cmp	w1, w0
	bne	.L309
	ldr	w0, [x19, 8]
	ldr	w1, [x2, 60]
	cmp	w1, w0
	csetm	w0, ne
	b	.L294
	.size	VCTRL_CmpConfigParameter, .-VCTRL_CmpConfigParameter
	.align	3
.LC162:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_CheckChanWH
	.type	VCTRL_CheckChanWH, %function
VCTRL_CheckChanWH:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x0, .LC163
	ldr	x2, .LC164
	ldr	x0, [x0, w19, sxtw 3]
	ldr	w2, [x2]
	ldr	w3, [x0, 1348]
	cmp	w3, w2
	bgt	.L313
	ldr	x1, .LC165
	ldr	w3, [x0, 1352]
	mov	w0, 0
	ldr	w1, [x1]
	cmp	w3, w1
	ble	.L314
.L313:
	ldr	w1, [x20]
	mov	w0, 0
	cmp	w1, wzr
	and	w3, w1, -3
	ccmp	w1, 20, 4, ne
	ccmp	w3, 13, 4, ne
	cset	w1, ne
	cbnz	w1, .L318
.L314:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L318:
	ldr	x3, .LC165
	ldr	x1, .LC167
	ldr	w3, [x3]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L314
	.size	VCTRL_CheckChanWH, .-VCTRL_CheckChanWH
	.align	3
.LC163:
	.xword	.LANCHOR0
	.align	3
.LC164:
	.xword	g_ChipMaxWidth
	.align	3
.LC165:
	.xword	g_ChipMaxHeight
	.align	3
.LC167:
	.xword	.LC166
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SeekPts
	.type	VCTRL_SeekPts, %function
VCTRL_SeekPts:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x2, .LC168
	ldr	x0, [x2, w19, sxtw 3]
	cbz	x0, .L325
	ldr	w0, [x0, 672]
	sub	w3, w0, #10
	cmp	w3, 2
	bls	.L326
.L322:
	cmp	w0, 14
	ccmp	w0, 4, 4, ne
	beq	.L324
	mov	x1, x20
	mov	w0, w19
	bl	SM_SeekPts
	cmp	w0, wzr
	csetm	w0, ne
.L321:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L326:
	ldr	w1, [x2, 4076]
	tbnz	x1, 0, .L322
.L324:
	mov	w0, -1
	b	.L321
.L325:
	ldr	x3, .LC169
	mov	w2, 3740
	ldr	x1, .LC170
	bl	dprint_vfmw
	mov	w0, -1
	b	.L321
	.size	VCTRL_SeekPts, .-VCTRL_SeekPts
	.align	3
.LC168:
	.xword	.LANCHOR0
	.align	3
.LC169:
	.xword	.LC5
	.align	3
.LC170:
	.xword	.LC2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ReleaseStream
	.type	VCTRL_ReleaseStream, %function
VCTRL_ReleaseStream:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC171
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L330
	mov	w0, w19
	bl	SM_Reset
	cmp	w0, wzr
	csetm	w0, ne
.L329:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L330:
	ldr	x3, .LC172
	mov	w2, 4170
	ldr	x1, .LC173
	bl	dprint_vfmw
	mov	w0, -1
	b	.L329
	.size	VCTRL_ReleaseStream, .-VCTRL_ReleaseStream
	.align	3
.LC171:
	.xword	.LANCHOR0
	.align	3
.LC172:
	.xword	.LC5
	.align	3
.LC173:
	.xword	.LC2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ImageOutputEnable
	.type	VCTRL_ImageOutputEnable, %function
VCTRL_ImageOutputEnable:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	cmp	w19, 15
	bhi	.L334
	ldr	x20, .LC176
	mov	w0, 4
	ldr	x1, [x20, 24]
	blr	x1
	ldr	x1, .LC177
	mov	w0, 4
	ldr	w2, [x21]
	ldr	x1, [x1, w19, sxtw 3]
	str	w2, [x1, 20]
	ldr	x1, [x20, 32]
	blr	x1
	mov	w0, 0
.L333:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L334:
	ldr	x3, .LC174
	mov	w2, 4283
	ldr	x1, .LC175
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L333
	.size	VCTRL_ImageOutputEnable, .-VCTRL_ImageOutputEnable
	.align	3
.LC174:
	.xword	.LC51
	.align	3
.LC175:
	.xword	.LC2
	.align	3
.LC176:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC177:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_TryToRelSpecialFrm
	.type	VCTRL_TryToRelSpecialFrm, %function
VCTRL_TryToRelSpecialFrm:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	w20, w0
	mov	x0, x30
	mov	w22, w1
	bl	_mcount
	cmp	w20, 15
	bhi	.L362
	ldr	x23, .LC180
	ldr	x3, [x23, w20, sxtw 3]
	cbz	x3, .L363
	add	x0, x3, 294912
	ldr	w1, [x0, 2528]
	cbz	w1, .L359
	ldr	w0, [x0, 2020]
	mov	w2, 0
	cmp	w22, w0
	bne	.L341
	b	.L364
	.p2align 3
.L342:
	add	x0, x0, 2174
	add	x0, x3, x0, lsl 4
	ldr	w0, [x0, 4]
	cmp	w22, w0
	beq	.L339
.L341:
	add	w2, w2, 1
	cmp	w1, w2
	mov	w21, w2
	sxtw	x0, w2
	add	x0, x0, 16384
	bne	.L342
.L359:
	mov	w0, -1
.L356:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L364:
	mov	w21, 0
	.p2align 2
.L339:
	add	x0, x3, 262144
	ldrsb	w4, [x0, 2058]
	cmp	w4, wzr
	ble	.L359
	add	x0, x3, 282624
	mov	w19, 0
	add	x0, x0, 3600
	ldr	w2, [x0, -100]
	cmp	w22, w2
	bne	.L345
	b	.L365
	.p2align 3
.L348:
	ldr	w2, [x0], 100
	cmp	w22, w2
	beq	.L343
.L345:
	add	w19, w19, 1
	cmp	w19, w4
	bne	.L348
	b	.L359
.L365:
	mov	w19, 0
	.p2align 2
.L343:
	ldr	x24, .LC181
	sub	w22, w1, #1
	mov	w0, 3
	sxtw	x19, w19
	ldr	x1, [x24, 24]
	blr	x1
	lsl	x0, x19, 2
	ldr	x3, [x23, w20, sxtw 3]
	sub	x0, x0, x19
	ldr	x2, [x24, 32]
	lsl	x1, x0, 5
	add	x1, x0, x1
	mov	w0, 3
	add	x19, x1, x19
	add	x19, x3, x19
	add	x1, x19, 282624
	add	x19, x19, 286720
	strb	wzr, [x1, 3486]
	strb	wzr, [x19, 2590]
	blr	x2
	cmp	w22, w21
	bls	.L353
	sxtw	x25, w20
	uxtw	x19, w21
	.p2align 2
.L347:
	add	w21, w21, 1
	add	x19, x19, 16384
	add	x0, x19, 2174
	ldr	x3, [x23, x25, lsl 3]
	uxtw	x19, w21
	ldr	x4, [x24, 104]
	add	x1, x19, 16384
	mov	x2, 16
	add	x1, x1, 2174
	add	x0, x3, x0, lsl 4
	add	x1, x3, x1, lsl 4
	blr	x4
	cmp	w21, w22
	bcc	.L347
.L346:
	uxtw	x0, w22
	ldr	x4, [x23, w20, sxtw 3]
	add	x0, x0, 16384
	ldr	x3, [x24, 96]
	add	x0, x0, 2174
	mov	x2, 16
	mov	w1, 0
	add	x0, x4, x0, lsl 4
	blr	x3
	ldr	x1, [x23, w20, sxtw 3]
	mov	w0, 0
	add	x1, x1, 294912
	ldr	w2, [x1, 2528]
	sub	w2, w2, #1
	str	w2, [x1, 2528]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L353:
	mov	w22, w21
	b	.L346
.L362:
	ldr	x3, .LC178
	mov	w2, 4401
	ldr	x1, .LC179
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L356
.L363:
	ldr	x3, .LC178
	mov	w2, 4402
	ldr	x1, .LC179
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L356
	.size	VCTRL_TryToRelSpecialFrm, .-VCTRL_TryToRelSpecialFrm
	.align	3
.LC178:
	.xword	.LC51
	.align	3
.LC179:
	.xword	.LC2
	.align	3
.LC180:
	.xword	.LANCHOR0
	.align	3
.LC181:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ReleaseChanImage
	.type	VCTRL_ReleaseChanImage, %function
VCTRL_ReleaseChanImage:
	sub	sp, sp, #624
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	w19, 15
	bhi	.L387
	ldr	x21, .LC184
	ldr	x0, [x21, w19, sxtw 3]
	cbz	x0, .L388
	ldr	w0, [x0, 12]
	cmp	w0, 1
	bne	.L389
	cbz	x20, .L390
	ldr	x1, .LC191
	mov	w2, w19
	ldr	w3, [x20, 156]
	mov	w0, 2
	bl	dprint_vfmw
	ldr	x22, .LC192
	mov	w0, 4
	ldr	x1, [x22, 24]
	blr	x1
	ldr	x0, [x21, w19, sxtw 3]
	cbz	x0, .L391
	ldr	w0, [x0, 12]
	cmp	w0, 1
	bne	.L392
	ldr	w1, [x20, 28]
	mov	w0, w19
	bl	VCTRL_TryToRelSpecialFrm
	cbz	w0, .L393
	ldr	x0, [x21, w19, sxtw 3]
	mov	x1, x20
	add	x0, x0, 294912
	add	x0, x0, 2576
	bl	SYNTAX_ReleaseImage
	mov	w23, w0
	ldr	w2, [x20, 196]
	cmn	w2, #1
	beq	.L375
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x29, 64
	str	w2, [x29, 220]
	add	x0, x0, 294912
	add	x0, x0, 2576
	bl	SYNTAX_ReleaseImage
	mov	w23, w0
.L375:
	mov	w0, 13
	bl	CHECK_REC_POS_ENABLE
	cbnz	w0, .L394
.L377:
	cmp	w23, 1
	csetm	w19, ne
	cbnz	w19, .L395
.L379:
	ldr	x1, [x22, 32]
	mov	w0, 4
	blr	x1
.L368:
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 624
	ret
	.p2align 3
.L393:
	ldr	x1, [x22, 32]
	mov	w0, 4
	mov	w19, 0
	blr	x1
	ldr	x1, .LC198
	mov	w2, 4871
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	ldp	x21, x22, [sp, 32]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 624
	ret
	.p2align 3
.L395:
	ldr	x2, .LC200
	mov	w0, 0
	ldr	x1, .LC202
	add	x2, x2, 128
	bl	dprint_vfmw
	b	.L379
	.p2align 3
.L394:
	ldr	x0, [x22]
	add	x21, x21, 4080
	blr	x0
	mov	w3, w0
	ldr	x1, .LC199
	ldr	w0, [x21, w19, sxtw 2]
	ldr	w1, [x1]
	sub	w0, w3, w0
	cmp	w0, w1
	bls	.L377
	lsl	w2, w19, 24
	ldr	x0, .LC200
	cmp	w23, 1
	add	w2, w2, 851968
	csinc	w2, w2, w2, ne
	mov	w1, 4906
	add	x0, x0, 104
	str	w3, [x21, w19, sxtw 2]
	bl	DBG_AddTrace
	b	.L377
	.p2align 3
.L392:
	ldr	x1, .LC196
	mov	w2, 4859
	mov	w0, 0
.L386:
	bl	dprint_vfmw
	mov	w19, -1
	ldr	x1, [x22, 32]
	mov	w0, 4
	blr	x1
	b	.L368
	.p2align 3
.L389:
	ldr	x3, .LC187
	mov	w2, 4842
	ldr	x1, .LC183
	mov	w0, 0
	mov	w19, -1
	bl	dprint_vfmw
	b	.L368
	.p2align 3
.L387:
	ldr	x3, .LC182
	mov	w2, 4840
	ldr	x1, .LC183
	mov	w0, 0
	mov	w19, -1
	bl	dprint_vfmw
	b	.L368
.L390:
	ldr	x3, .LC189
	mov	w2, 4843
	ldr	x1, .LC183
	mov	w0, 0
	mov	w19, -1
	bl	dprint_vfmw
	b	.L368
.L388:
	ldr	x3, .LC185
	mov	w2, 4841
	ldr	x1, .LC183
	mov	w19, -1
	bl	dprint_vfmw
	b	.L368
.L391:
	ldr	x1, .LC194
	mov	w2, 4850
	b	.L386
	.size	VCTRL_ReleaseChanImage, .-VCTRL_ReleaseChanImage
	.align	3
.LC182:
	.xword	.LC51
	.align	3
.LC183:
	.xword	.LC2
	.align	3
.LC184:
	.xword	.LANCHOR0
	.align	3
.LC185:
	.xword	.LC5
	.align	3
.LC187:
	.xword	.LC186
	.align	3
.LC189:
	.xword	.LC188
	.align	3
.LC191:
	.xword	.LC190
	.align	3
.LC192:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC194:
	.xword	.LC193
	.align	3
.LC196:
	.xword	.LC195
	.align	3
.LC198:
	.xword	.LC197
	.align	3
.LC199:
	.xword	g_TraceImgPeriod
	.align	3
.LC200:
	.xword	.LANCHOR1
	.align	3
.LC202:
	.xword	.LC201
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetVidStd
	.type	VCTRL_GetVidStd, %function
VCTRL_GetVidStd:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L397
	ldr	x0, .LC204
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L397
	ldr	w1, [x0, 12]
	cmp	w1, 1
	beq	.L400
	ldr	x1, .LC208
	mov	w3, w19
	mov	w2, 4939
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, 21
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L397:
	ldr	x1, .LC206
	mov	w2, 4932
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, 21
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L400:
	ldr	w0, [x0, 672]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_GetVidStd, .-VCTRL_GetVidStd
	.align	3
.LC204:
	.xword	.LANCHOR0
	.align	3
.LC206:
	.xword	.LC205
	.align	3
.LC208:
	.xword	.LC207
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetDecParam
	.type	VCTRL_GetDecParam, %function
VCTRL_GetDecParam:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	cmp	w20, 15
	bhi	.L402
	ldr	x0, .LC209
	ldr	x19, [x0, w20, sxtw 3]
	cbz	x19, .L402
	ldr	w1, [x19, 12]
	cmp	w1, 1
	bne	.L405
	ldr	w1, [x19, 16]
	cmp	w1, 1
	bne	.L405
	ldr	w3, [x19, 672]
	cmp	w3, 17
	bls	.L428
.L406:
	ldr	x2, .LC213
	mov	w0, 0
	ldr	x1, .LC215
	add	x2, x2, 152
	bl	dprint_vfmw
.L405:
	mov	x1, 0
	mov	x0, x1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L428:
	ldr	x1, .LC212
	ldrb	w1, [x1,w3,uxtw]
	adr	x2, .Lrtx408
	add	x1, x2, w1, sxtb #2
	br	x1
.Lrtx408:
	.section	.rodata
	.align	0
	.align	2
.L408:
	.byte	(.L407 - .Lrtx408) / 4
	.byte	(.L427 - .Lrtx408) / 4
	.byte	(.L410 - .Lrtx408) / 4
	.byte	(.L427 - .Lrtx408) / 4
	.byte	(.L406 - .Lrtx408) / 4
	.byte	(.L427 - .Lrtx408) / 4
	.byte	(.L413 - .Lrtx408) / 4
	.byte	(.L406 - .Lrtx408) / 4
	.byte	(.L427 - .Lrtx408) / 4
	.byte	(.L427 - .Lrtx408) / 4
	.byte	(.L416 - .Lrtx408) / 4
	.byte	(.L416 - .Lrtx408) / 4
	.byte	(.L416 - .Lrtx408) / 4
	.byte	(.L427 - .Lrtx408) / 4
	.byte	(.L406 - .Lrtx408) / 4
	.byte	(.L418 - .Lrtx408) / 4
	.byte	(.L419 - .Lrtx408) / 4
	.byte	(.L427 - .Lrtx408) / 4
	.text
	.p2align 3
.L416:
	ldr	w0, [x0, 4076]
	tbz	x0, 0, .L405
	.p2align 2
.L427:
	add	x0, x19, 294912
	ldr	x20, [x0, 2592]
.L421:
	cbz	x20, .L405
	ldr	x0, .LC216
	ldr	x0, [x0]
	blr	x0
	str	w0, [x19, 48]
	mov	x1, x20
.L426:
	mov	x0, x1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L402:
	ldr	x1, .LC211
	mov	w3, w20
	mov	w2, 4956
	mov	w0, 22
	bl	dprint_vfmw
	mov	x1, 0
	b	.L426
	.p2align 3
.L407:
	add	x0, x19, 294912
	ldr	x20, [x0, 3200]
	b	.L421
	.p2align 3
.L410:
	add	x0, x19, 294912
	ldr	x20, [x0, 3624]
	b	.L421
	.p2align 3
.L419:
	add	x0, x19, 299008
	ldr	x20, [x0, 840]
	b	.L421
	.p2align 3
.L418:
	add	x0, x19, 294912
	ldr	x20, [x0, 2816]
	b	.L421
	.p2align 3
.L413:
	add	x0, x19, 294912
	ldr	x20, [x0, 2608]
	b	.L421
	.size	VCTRL_GetDecParam, .-VCTRL_GetDecParam
	.align	3
.LC209:
	.xword	.LANCHOR0
	.align	3
.LC211:
	.xword	.LC210
	.align	3
.LC212:
	.xword	.L408
	.align	3
.LC213:
	.xword	.LANCHOR1
	.align	3
.LC215:
	.xword	.LC214
	.align	3
.LC216:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_IsSedOnlyMode
	.type	VCTRL_IsSedOnlyMode, %function
VCTRL_IsSedOnlyMode:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L431
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L431:
	ldr	x1, .LC218
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_IsSedOnlyMode, .-VCTRL_IsSedOnlyMode
	.align	3
.LC218:
	.xword	.LC217
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ChanDecparamInValidFlag
	.type	VCTRL_ChanDecparamInValidFlag, %function
VCTRL_ChanDecparamInValidFlag:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L433
	ldr	x21, .LC219
	ldr	x0, [x21, w19, sxtw 3]
	cbz	x0, .L433
	ldr	w1, [x0, 12]
	cmp	w1, 1
	bne	.L436
	ldr	w22, [x0, 16]
	cmp	w22, 1
	beq	.L437
.L436:
	ldr	x1, .LC222
	mov	w2, w19
	mov	w0, 22
	mov	w20, 0
	bl	dprint_vfmw
.L435:
	mov	w0, w20
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L433:
	ldr	x1, .LC220
	mov	w3, w19
	mov	w2, 5120
	mov	w0, 22
	mov	w20, 0
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L437:
	mov	w0, w19
	bl	VCTRL_GetDecParam
	cbz	x0, .L442
	ldr	x0, [x21, w19, sxtw 3]
	mov	w20, 0
	str	wzr, [x0, 452]
	str	wzr, [x0, 456]
	b	.L435
	.p2align 3
.L442:
	ldr	x23, [x21, w19, sxtw 3]
	ldr	x0, .LC223
	ldr	w20, [x23, 452]
	cbz	w20, .L443
	ldr	x0, [x0]
	blr	x0
	ldr	x1, [x21, w19, sxtw 3]
	ldr	w2, [x1, 456]
	cmp	w0, w2
	bls	.L440
	sub	w0, w0, w2
	mov	w20, 0
	cmp	w0, 40
	bls	.L435
.L440:
	mov	w20, w22
	str	wzr, [x1, 452]
	str	wzr, [x1, 456]
	b	.L435
.L443:
	ldr	x0, [x0]
	str	w22, [x23, 452]
	blr	x0
	str	w0, [x23, 456]
	b	.L435
	.size	VCTRL_ChanDecparamInValidFlag, .-VCTRL_ChanDecparamInValidFlag
	.align	3
.LC219:
	.xword	.LANCHOR0
	.align	3
.LC220:
	.xword	.LC210
	.align	3
.LC222:
	.xword	.LC221
	.align	3
.LC223:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_VdmProcSedOnly
	.type	VCTRL_VdmProcSedOnly, %function
VCTRL_VdmProcSedOnly:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L449
	ldr	x0, .LC227
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L450
	ldr	w0, [x0, 16]
	cmp	w0, 1
	beq	.L448
	ldr	x3, .LC230
	mov	w2, 5392
	ldr	x1, .LC226
	mov	w0, 0
	bl	dprint_vfmw
.L444:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L448:
	mov	w0, w19
	bl	VCTRL_GetDecParam
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L449:
	ldr	x3, .LC225
	mov	w2, 5390
	ldr	x1, .LC226
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L450:
	ldr	x3, .LC228
	mov	w2, 5391
	ldr	x1, .LC226
	bl	dprint_vfmw
	b	.L444
	.size	VCTRL_VdmProcSedOnly, .-VCTRL_VdmProcSedOnly
	.align	3
.LC225:
	.xword	.LC224
	.align	3
.LC226:
	.xword	.LC2
	.align	3
.LC227:
	.xword	.LANCHOR0
	.align	3
.LC228:
	.xword	.LC186
	.align	3
.LC230:
	.xword	.LC229
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_FlushDecoder
	.type	VCTRL_FlushDecoder, %function
VCTRL_FlushDecoder:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC231
	ldr	x0, [x19, w20, sxtw 3]
	cbz	x0, .L478
	add	x0, x0, 294912
	ldr	w1, [x0, 2576]
	cmp	w1, 17
	bls	.L479
.L471:
	mov	w0, 0
.L453:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L479:
	ldr	x2, .LC234
	ldrb	w1, [x2,w1,uxtw]
	adr	x2, .Lrtx455
	add	x1, x2, w1, sxtb #2
	br	x1
.Lrtx455:
	.section	.rodata
	.align	0
	.align	2
.L455:
	.byte	(.L454 - .Lrtx455) / 4
	.byte	(.L456 - .Lrtx455) / 4
	.byte	(.L457 - .Lrtx455) / 4
	.byte	(.L458 - .Lrtx455) / 4
	.byte	(.L471 - .Lrtx455) / 4
	.byte	(.L459 - .Lrtx455) / 4
	.byte	(.L460 - .Lrtx455) / 4
	.byte	(.L471 - .Lrtx455) / 4
	.byte	(.L461 - .Lrtx455) / 4
	.byte	(.L462 - .Lrtx455) / 4
	.byte	(.L463 - .Lrtx455) / 4
	.byte	(.L463 - .Lrtx455) / 4
	.byte	(.L463 - .Lrtx455) / 4
	.byte	(.L464 - .Lrtx455) / 4
	.byte	(.L471 - .Lrtx455) / 4
	.byte	(.L465 - .Lrtx455) / 4
	.byte	(.L466 - .Lrtx455) / 4
	.byte	(.L467 - .Lrtx455) / 4
	.text
	.p2align 3
.L467:
	add	x0, x0, 2584
	bl	VP9DEC_GetRemainImg
	mov	w0, 0
	b	.L453
	.p2align 3
.L466:
	add	x0, x0, 2584
	bl	HEVC_FlushOutputFrmFromDPB
	cbnz	w0, .L480
.L469:
	ldr	x1, .LC238
	mov	w0, 32
	bl	dprint_vfmw
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	mov	w0, 0
	b	.L453
	.p2align 3
.L465:
	add	x0, x0, 2584
	bl	MVC_FlushDecoder
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	ldr	x0, [x19, w20, sxtw 3]
	mov	w1, -1
	add	x0, x0, 294912
	add	x0, x0, 2584
	bl	MVC_FlushDPB
	cbz	w0, .L471
	ldr	x2, .LC239
	mov	w3, 5481
	ldr	x1, .LC241
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 0
	b	.L453
	.p2align 3
.L464:
	add	x0, x0, 2584
	bl	VP8DEC_GetRemainImg
	mov	w0, 0
	b	.L453
	.p2align 3
.L463:
	add	x0, x0, 2584
	bl	VP6DEC_GetRemainImg
	mov	w0, 0
	b	.L453
	.p2align 3
.L462:
	add	x0, x0, 2584
	bl	REAL9DEC_GetRemainImg
	mov	w0, 0
	b	.L453
	.p2align 3
.L461:
	add	x0, x0, 2584
	bl	REAL8DEC_GetRemainImg
	mov	w0, 0
	b	.L453
	.p2align 3
.L460:
	add	x0, x0, 2584
	bl	AVSDEC_GetRemainImg
	mov	w0, 0
	b	.L453
	.p2align 3
.L459:
	add	x0, x0, 2584
	bl	DIVX3DEC_GetRemainImg
	mov	w0, 0
	b	.L453
	.p2align 3
.L458:
	add	x0, x0, 2584
	bl	MPEG2DEC_GetRemainImg
	mov	w0, 0
	b	.L453
	.p2align 3
.L457:
	add	x0, x0, 2584
	bl	MPEG4DEC_GetRemainImg
	mov	w0, 0
	b	.L453
	.p2align 3
.L456:
	add	x0, x0, 2584
	bl	VC1DEC_GetRemainImg
	mov	w0, 0
	b	.L453
	.p2align 3
.L454:
	add	x0, x0, 2584
	bl	H264_FlushDecoder
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	ldr	x0, [x19, w20, sxtw 3]
	add	x0, x0, 294912
	add	x0, x0, 2584
	bl	FlushDPB
	cbz	w0, .L471
	ldr	x1, .LC236
	mov	w2, 5448
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 0
	b	.L453
	.p2align 3
.L480:
	ldr	x1, .LC236
	mov	w2, 5458
	mov	w0, 0
	bl	dprint_vfmw
	b	.L469
.L478:
	ldr	x3, .LC232
	mov	w2, 5422
	ldr	x1, .LC233
	bl	dprint_vfmw
	mov	w0, -1
	b	.L453
	.size	VCTRL_FlushDecoder, .-VCTRL_FlushDecoder
	.align	3
.LC231:
	.xword	.LANCHOR0
	.align	3
.LC232:
	.xword	.LC5
	.align	3
.LC233:
	.xword	.LC2
	.align	3
.LC234:
	.xword	.L455
	.align	3
.LC236:
	.xword	.LC235
	.align	3
.LC238:
	.xword	.LC237
	.align	3
.LC239:
	.xword	.LANCHOR2
	.align	3
.LC241:
	.xword	.LC240
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetImageBuffer
	.type	VCTRL_GetImageBuffer, %function
VCTRL_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L504
	ldr	x3, .LC244
	ldr	x1, [x3, w19, sxtw 3]
	cbz	x1, .L505
	ldr	w0, [x1, 16]
	cmp	w0, 1
	bne	.L506
	ldr	w2, [x1, 672]
	cmp	w2, 17
	bls	.L507
.L500:
	mov	w0, 0
	b	.L483
	.p2align 3
.L507:
	ldr	x4, .LC247
	ldrb	w2, [x4,w2,uxtw]
	adr	x4, .Lrtx487
	add	x2, x4, w2, sxtb #2
	br	x2
.Lrtx487:
	.section	.rodata
	.align	0
	.align	2
.L487:
	.byte	(.L486 - .Lrtx487) / 4
	.byte	(.L488 - .Lrtx487) / 4
	.byte	(.L489 - .Lrtx487) / 4
	.byte	(.L490 - .Lrtx487) / 4
	.byte	(.L500 - .Lrtx487) / 4
	.byte	(.L491 - .Lrtx487) / 4
	.byte	(.L492 - .Lrtx487) / 4
	.byte	(.L500 - .Lrtx487) / 4
	.byte	(.L493 - .Lrtx487) / 4
	.byte	(.L494 - .Lrtx487) / 4
	.byte	(.L495 - .Lrtx487) / 4
	.byte	(.L495 - .Lrtx487) / 4
	.byte	(.L495 - .Lrtx487) / 4
	.byte	(.L496 - .Lrtx487) / 4
	.byte	(.L500 - .Lrtx487) / 4
	.byte	(.L497 - .Lrtx487) / 4
	.byte	(.L498 - .Lrtx487) / 4
	.byte	(.L499 - .Lrtx487) / 4
	.text
	.p2align 3
.L499:
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	VP9DEC_GetImageBuffer
.L483:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L498:
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	HEVCDEC_GetImageBuffer
	b	.L483
	.p2align 3
.L497:
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	MVC_DEC_GetImageBuffer
	b	.L483
	.p2align 3
.L496:
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	VP8DEC_GetImageBuffer
	b	.L483
	.p2align 3
.L495:
	ldr	w2, [x3, 4076]
	tbz	x2, 0, .L483
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	VP6DEC_GetImageBuffer
	b	.L483
	.p2align 3
.L494:
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	REAL9DEC_GetImageBuffer
	b	.L483
	.p2align 3
.L493:
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	REAL8DEC_GetImageBuffer
	b	.L483
	.p2align 3
.L492:
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	AVSDEC_GetImageBuffer
	b	.L483
	.p2align 3
.L491:
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	DIVX3DEC_GetImageBuffer
	b	.L483
	.p2align 3
.L490:
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	MPEG2DEC_GetImageBuffer
	b	.L483
	.p2align 3
.L489:
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	MPEG4DEC_GetImageBuffer
	b	.L483
	.p2align 3
.L488:
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	VC1DEC_GetImageBuffer
	b	.L483
	.p2align 3
.L486:
	add	x0, x1, 294912
	add	x0, x0, 2584
	bl	H264DEC_GetImageBuffer
	b	.L483
	.p2align 3
.L506:
	ldr	x3, .LC246
	mov	w2, 5546
	ldr	x1, .LC243
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L483
	.p2align 3
.L504:
	ldr	x3, .LC242
	mov	w2, 5544
	ldr	x1, .LC243
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L483
.L505:
	ldr	x3, .LC245
	mov	w2, 5545
	ldr	x1, .LC243
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L483
	.size	VCTRL_GetImageBuffer, .-VCTRL_GetImageBuffer
	.align	3
.LC242:
	.xword	.LC224
	.align	3
.LC243:
	.xword	.LC2
	.align	3
.LC244:
	.xword	.LANCHOR0
	.align	3
.LC245:
	.xword	.LC186
	.align	3
.LC246:
	.xword	.LC229
	.align	3
.LC247:
	.xword	.L487
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ConfigFFFBSpeed
	.type	VCTRL_ConfigFFFBSpeed, %function
VCTRL_ConfigFFFBSpeed:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	w19, 15
	bhi	.L514
	ldr	x0, .LC250
	ldr	x2, [x0, w19, sxtw 3]
	cbz	x2, .L515
	ldr	w0, [x2, 12]
	cmp	w0, 1
	bne	.L516
	cbz	x20, .L517
	ldr	w1, [x20]
	mov	w0, 0
	str	w1, [x2, 1284]
.L510:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L516:
	ldr	x3, .LC254
	mov	w2, 5664
	ldr	x1, .LC249
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L510
	.p2align 3
.L514:
	ldr	x3, .LC248
	mov	w2, 5662
	ldr	x1, .LC249
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L510
.L515:
	ldr	x3, .LC252
	mov	w2, 5663
	ldr	x1, .LC249
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L510
.L517:
	ldr	x3, .LC256
	mov	w2, 5665
	ldr	x1, .LC249
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L510
	.size	VCTRL_ConfigFFFBSpeed, .-VCTRL_ConfigFFFBSpeed
	.align	3
.LC248:
	.xword	.LC51
	.align	3
.LC249:
	.xword	.LC2
	.align	3
.LC250:
	.xword	.LANCHOR0
	.align	3
.LC252:
	.xword	.LC251
	.align	3
.LC254:
	.xword	.LC253
	.align	3
.LC256:
	.xword	.LC255
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ConfigPVRInfo
	.type	VCTRL_ConfigPVRInfo, %function
VCTRL_ConfigPVRInfo:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	w19, 15
	bhi	.L524
	ldr	x0, .LC259
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L525
	ldr	w1, [x0, 12]
	cmp	w1, 1
	bne	.L526
	cbz	x20, .L527
	ldr	x3, .LC263
	mov	x2, 20
	mov	x1, x20
	add	x0, x0, 1316
	ldr	x3, [x3, 104]
	blr	x3
	mov	w0, 0
.L520:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L526:
	ldr	x3, .LC261
	mov	w2, 5677
	ldr	x1, .LC258
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L520
	.p2align 3
.L524:
	ldr	x3, .LC257
	mov	w2, 5675
	ldr	x1, .LC258
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L520
.L527:
	ldr	x3, .LC262
	mov	w2, 5678
	ldr	x1, .LC258
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L520
.L525:
	ldr	x3, .LC260
	mov	w2, 5676
	ldr	x1, .LC258
	bl	dprint_vfmw
	mov	w0, -1
	b	.L520
	.size	VCTRL_ConfigPVRInfo, .-VCTRL_ConfigPVRInfo
	.align	3
.LC257:
	.xword	.LC51
	.align	3
.LC258:
	.xword	.LC2
	.align	3
.LC259:
	.xword	.LANCHOR0
	.align	3
.LC260:
	.xword	.LC251
	.align	3
.LC261:
	.xword	.LC253
	.align	3
.LC262:
	.xword	.LC255
	.align	3
.LC263:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_IsChanDecable
	.type	VCTRL_IsChanDecable, %function
VCTRL_IsChanDecable:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC264
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L531
	add	x1, x29, 40
	mov	w0, w19
	bl	SM_GetInfo
	cbnz	w0, .L531
	ldr	w0, [x29, 48]
	cmp	w0, 16
	bgt	.L532
	ldr	w0, [x29, 56]
	cmp	w0, 524288
	ble	.L531
.L532:
	mov	w0, w19
	bl	VCTRL_GetImageBuffer
	cmp	w0, 1
	csetm	w0, ne
	cbnz	w0, .L530
	ldr	x0, .LC265
	ldr	x0, [x0]
	blr	x0
	ldr	x1, [x20, w19, sxtw 3]
	ldp	x19, x20, [sp, 16]
	ldr	w1, [x1, 48]
	ldp	x29, x30, [sp], 64
	sub	w0, w0, w1
	cmp	w0, 50
	csetm	w0, hi
	ret
	.p2align 3
.L531:
	mov	w0, -1
.L530:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	ret
	.size	VCTRL_IsChanDecable, .-VCTRL_IsChanDecable
	.align	3
.LC264:
	.xword	.LANCHOR0
	.align	3
.LC265:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_IsChanSegEnough
	.type	VCTRL_IsChanSegEnough, %function
VCTRL_IsChanSegEnough:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC266
	ldr	x0, [x0, w19, sxtw 3]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ldr	w0, [x0, 440]
	cmp	w0, 1
	cset	w0, ne
	ret
	.size	VCTRL_IsChanSegEnough, .-VCTRL_IsChanSegEnough
	.align	3
.LC266:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_IsChanActive
	.type	VCTRL_IsChanActive, %function
VCTRL_IsChanActive:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L541
	ldr	x0, .LC267
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L541
	ldr	w1, [x0, 12]
	cbz	w1, .L541
	ldr	w1, [x0, 16]
	cbz	w1, .L541
	ldr	w1, [x0, 28]
	cbz	w1, .L541
	ldr	w0, [x0, 672]
	ldr	x19, [sp, 16]
	cmp	w0, 21
	csetm	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L541:
	mov	w0, -1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_IsChanActive, .-VCTRL_IsChanActive
	.align	3
.LC267:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	USERDEC_Init
	.type	USERDEC_Init, %function
USERDEC_Init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x3, .LC268
	mov	x2, 624
	mov	w1, 0
	mov	x0, x19
	ldr	w21, [x19, 608]
	ldr	x3, [x3, 96]
	blr	x3
	str	w21, [x19, 608]
	add	x0, x19, 8
	bl	ResetVoQueue
	str	x20, [x19]
	ldr	x3, .LC269
	mov	x0, 0
	.p2align 2
.L545:
	ldr	x2, [x3, x0, lsl 3]
	add	x1, x2, 294912
	add	x1, x1, 2584
	cbz	x2, .L543
	cmp	x19, x1
	beq	.L544
.L543:
	add	x0, x0, 1
	cmp	x0, 16
	bne	.L545
	mov	w0, -1
	str	w0, [x19, 616]
.L548:
	ldr	x1, .LC271
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L544:
	cmn	w0, #1
	str	w0, [x19, 616]
	mov	w0, 1
	beq	.L548
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	USERDEC_Init, .-USERDEC_Init
	.align	3
.LC268:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC269:
	.xword	.LANCHOR0
	.align	3
.LC271:
	.xword	.LC270
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	USERDEC_RecycleImage
	.type	USERDEC_RecycleImage, %function
USERDEC_RecycleImage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cbz	x19, .L560
	ldr	w0, [x19, 616]
	mov	w1, w20
	mov	w2, 0
	bl	FSP_SetDisplay
	ldr	w0, [x19, 616]
	mov	w1, w20
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L561
	ldr	w0, [x19, 616]
	ldr	x1, [x20, 560]
	bl	FreeUsdByDec
	str	xzr, [x20, 560]
	ldr	x1, [x20, 568]
	ldr	w0, [x19, 616]
	bl	FreeUsdByDec
	str	xzr, [x20, 568]
	mov	w0, 1
.L559:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L561:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L560:
	mov	w0, -2
	b	.L559
	.size	USERDEC_RecycleImage, .-USERDEC_RecycleImage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetUsrdecFrame
	.type	VCTRL_GetUsrdecFrame, %function
VCTRL_GetUsrdecFrame:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w20, w0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	ldr	x23, .LC272
	ldr	x0, [x23, w20, sxtw 3]
	cbz	x0, .L579
	ldr	w1, [x0, 672]
	cmp	w1, 20
	bne	.L580
	ldr	w0, [x0, 592]
	cmp	w0, 2
	ble	.L581
	mov	w1, 1
	mov	w0, w20
	bl	FSP_NewLogicFs
	mov	w22, w0
	tbnz	w0, #31, .L582
	mov	w1, w0
	mov	w0, w20
	bl	FSP_GetLogicFs
	mov	x19, x0
	ldr	x1, .LC282
	mov	w2, w22
	mov	w0, 18
	bl	dprint_vfmw
	cbz	x19, .L583
	ldr	x0, [x19, 640]
	cbz	x0, .L569
	ldr	x1, [x19, 648]
	cbz	x1, .L569
	ldr	x2, [x19, 656]
	cbz	x2, .L569
	ldr	w3, [x1, 16]
	ldr	w4, [x2, 16]
	ldr	x1, .LC286
	ldr	w2, [x0, 16]
	mov	w0, 18
	bl	dprint_vfmw
	ldr	x0, [x19, 640]
	ldr	w0, [x0, 16]
	str	w0, [x21, 40]
	ldr	x0, [x23, w20, sxtw 3]
	ldr	w0, [x0, 588]
	str	w0, [x21, 20]
	ldr	x0, [x19, 640]
	ldr	w0, [x0, 28]
	str	w0, [x21, 24]
	ldr	x0, [x19, 640]
	ldr	w0, [x0, 28]
	bl	MEM_Phy2Vir
	str	x0, [x21, 32]
	mov	w1, 0
.L564:
	mov	w0, w1
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L580:
	ldr	x1, .LC276
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L564
	.p2align 3
.L581:
	ldr	x1, .LC278
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L564
	.p2align 3
.L569:
	ldr	x1, .LC288
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L564
	.p2align 3
.L582:
	ldr	x1, .LC280
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L564
.L579:
	ldr	x1, .LC274
	mov	w2, 6018
	ldr	x3, .LC273
	bl	dprint_vfmw
	mov	w1, -1
	b	.L564
.L583:
	ldr	x1, .LC284
	mov	w2, 6048
	mov	w0, 22
	bl	dprint_vfmw
	mov	w1, -1
	b	.L564
	.size	VCTRL_GetUsrdecFrame, .-VCTRL_GetUsrdecFrame
	.align	3
.LC272:
	.xword	.LANCHOR0
	.align	3
.LC273:
	.xword	.LC5
	.align	3
.LC274:
	.xword	.LC2
	.align	3
.LC276:
	.xword	.LC275
	.align	3
.LC278:
	.xword	.LC277
	.align	3
.LC280:
	.xword	.LC279
	.align	3
.LC282:
	.xword	.LC281
	.align	3
.LC284:
	.xword	.LC283
	.align	3
.LC286:
	.xword	.LC285
	.align	3
.LC288:
	.xword	.LC287
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	InitSyntaxCtx
	.type	InitSyntaxCtx, %function
InitSyntaxCtx:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC289
	ldr	x1, [x0, w19, sxtw 3]
	cbz	x1, .L602
	ldr	w0, [x1, 672]
	cmp	w0, 20
	bls	.L603
.L584:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L603:
	ldr	x2, .LC292
	ldrb	w0, [x2,w0,uxtw]
	adr	x2, .Lrtx588
	add	x0, x2, w0, sxtb #2
	br	x0
.Lrtx588:
	.section	.rodata
	.align	0
	.align	2
.L588:
	.byte	(.L587 - .Lrtx588) / 4
	.byte	(.L589 - .Lrtx588) / 4
	.byte	(.L590 - .Lrtx588) / 4
	.byte	(.L591 - .Lrtx588) / 4
	.byte	(.L584 - .Lrtx588) / 4
	.byte	(.L592 - .Lrtx588) / 4
	.byte	(.L593 - .Lrtx588) / 4
	.byte	(.L584 - .Lrtx588) / 4
	.byte	(.L594 - .Lrtx588) / 4
	.byte	(.L595 - .Lrtx588) / 4
	.byte	(.L596 - .Lrtx588) / 4
	.byte	(.L596 - .Lrtx588) / 4
	.byte	(.L596 - .Lrtx588) / 4
	.byte	(.L597 - .Lrtx588) / 4
	.byte	(.L584 - .Lrtx588) / 4
	.byte	(.L598 - .Lrtx588) / 4
	.byte	(.L599 - .Lrtx588) / 4
	.byte	(.L600 - .Lrtx588) / 4
	.byte	(.L584 - .Lrtx588) / 4
	.byte	(.L584 - .Lrtx588) / 4
	.byte	(.L601 - .Lrtx588) / 4
	.text
	.p2align 3
.L601:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	USERDEC_Init
	b	.L584
	.p2align 3
.L600:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	VP9DEC_Init
	b	.L584
	.p2align 3
.L599:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	HEVCDEC_Init
	b	.L584
	.p2align 3
.L598:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	MVC_DEC_Init
	b	.L584
	.p2align 3
.L597:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	VP8DEC_Init
	b	.L584
	.p2align 3
.L596:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	VP6DEC_Init
	b	.L584
	.p2align 3
.L595:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	REAL9DEC_Init
	b	.L584
	.p2align 3
.L594:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	REAL8DEC_Init
	b	.L584
	.p2align 3
.L593:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	AVSDEC_Init
	b	.L584
	.p2align 3
.L592:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	DIVX3DEC_Init
	b	.L584
	.p2align 3
.L591:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	MPEG2DEC_Init
	b	.L584
	.p2align 3
.L590:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	MPEG4DEC_Init
	b	.L584
	.p2align 3
.L589:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	VC1DEC_Init
	b	.L584
	.p2align 3
.L587:
	add	x0, x1, 294912
	add	x1, x1, 680
	add	x0, x0, 2584
	bl	H264DEC_Init
	b	.L584
	.p2align 3
.L602:
	ldr	x3, .LC290
	mov	w2, 6103
	ldr	x1, .LC291
	mov	w0, 0
	bl	dprint_vfmw
	b	.L584
	.size	InitSyntaxCtx, .-InitSyntaxCtx
	.align	3
.LC289:
	.xword	.LANCHOR0
	.align	3
.LC290:
	.xword	.LC5
	.align	3
.LC291:
	.xword	.LC2
	.align	3
.LC292:
	.xword	.L588
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanRpuSize
	.type	VCTRL_GetChanRpuSize, %function
VCTRL_GetChanRpuSize:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x0, .LC293
	ldr	x0, [x0, 320]
	cbz	x0, .L606
	blr	x0
	mul	w0, w0, w20
	str	w0, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L606:
	mov	w0, -1
	mul	w0, w0, w20
	str	w0, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_GetChanRpuSize, .-VCTRL_GetChanRpuSize
	.align	3
.LC293:
	.xword	g_vdm_hal_fun_ptr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanMemSizeWithOption
	.type	VCTRL_GetChanMemSizeWithOption, %function
VCTRL_GetChanMemSizeWithOption:
	sub	sp, sp, #80
	stp	x29, x30, [sp, 16]
	add	x29, sp, 16
	stp	x19, x20, [sp, 32]
	stp	x21, x22, [sp, 48]
	stp	x23, x24, [sp, 64]
	mov	w20, w0
	mov	x0, x30
	mov	w23, w3
	mov	x19, x1
	mov	x21, x2
	bl	_mcount
	mov	w1, 11640
	mov	w0, 1800
	cmp	w20, 26
	movk	w1, 0xc, lsl 16
	movk	w0, 0xb2, lsl 16
	csel	w22, w1, w0, ne
	cmp	w23, 2
	beq	.L638
	ldr	x0, .LC294
	ldr	w2, [x19, 36]
	ldr	w4, [x19, 60]
	ldp	w3, w0, [x0, -208]
	add	w5, w2, w3
	add	w23, w4, w0
	cmp	w5, wzr
	ccmp	w23, wzr, 1, ge
	blt	.L639
	cmp	w5, 16
	bgt	.L640
	str	w5, [x19, 36]
	str	w23, [x19, 60]
.L610:
	tbnz	w5, #31, .L615
.L614:
	ldr	w2, [x19, 12]
	cmp	w2, 31
	ble	.L615
	ldr	w3, [x19, 16]
	cmp	w3, 31
	ble	.L615
	ldr	w1, [x19, 56]
	mov	w0, 262143
	cmp	w1, w0
	ble	.L615
	ldr	w0, [x19, 44]
	cmp	w0, 1
	beq	.L641
	cmp	w0, 2
	beq	.L642
.L617:
	ldr	w0, [x19, 40]
	mov	w4, 1
	cbnz	w0, .L643
	ldr	x0, .LC301
	sub	w23, w20, #27
	mov	w7, -2
	cmp	w23, 5
	csinv	w7, w7, wzr, ls
	ldr	x8, [x0, 128]
	cbz	x8, .L621
.L645:
	ldr	w0, [x19, 60]
	mov	w6, 0
	ldr	x24, .LC294
	mov	w1, 2147483647
	add	w5, w5, w0
	sub	x0, x24, #200
	add	w5, w5, 2
	str	x0, [sp]
	mov	w0, 1
	blr	x8
	cbnz	w0, .L621
	ldr	w0, [x24, -196]
	add	w1, w0, 16
	str	w1, [x21]
	ldr	w1, [x19, 176]
	cmp	w1, 1
	beq	.L622
	add	w0, w0, 307200
	add	w0, w0, 2576
	str	w0, [x21]
	ldr	w0, [x19, 56]
	add	w0, w0, 32768
	str	w0, [x21, 4]
.L623:
	str	w22, [x21, 8]
	mov	w0, 0
	ldr	w1, [x19, 44]
	cbnz	w1, .L644
.L624:
	cmp	w23, 5
	bhi	.L627
	mov	w1, 44808
	movk	w1, 0x13, lsl 16
	cmp	w0, w1
	csel	w0, w0, w1, cs
.L627:
	add	w1, w22, w0
	mov	w0, 0
	str	w1, [x21, 8]
.L612:
	add	sp, x29, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L643:
	mov	w0, 17
	add	w4, w5, w4
	cmp	w4, w0
	sub	w23, w20, #27
	csel	w4, w4, w0, le
	ldr	x0, .LC301
	mov	w7, -2
	cmp	w23, 5
	csinv	w7, w7, wzr, ls
	ldr	x8, [x0, 128]
	cbnz	x8, .L645
.L621:
	ldr	x1, .LC303
	mov	w2, 6390
	mov	w0, 0
	bl	dprint_vfmw
	add	sp, x29, 0
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L644:
	ldrsw	x2, [x19, 20]
	mov	w0, 4000
	ldp	w4, w3, [x19, 28]
	lsl	x1, x2, 7
	cmp	w20, 26
	sub	x2, x1, x2
	mov	w1, 2208
	lsl	x2, x2, 5
	smaddl	x0, w4, w0, x2
	smaddl	x0, w3, w1, x0
	bne	.L624
	mov	w5, 3992
	mov	w1, 2240
	and	x0, x0, 4294967295
	smaddl	x2, w4, w5, x2
	smaddl	x3, w3, w1, x2
	cmp	x3, x0
	csel	x0, x3, x0, cs
	b	.L624
	.p2align 3
.L642:
	ldr	w0, [x19, 20]
	cmp	w0, wzr
	ble	.L615
	ldr	w0, [x19, 24]
	cmp	w0, wzr
	ble	.L615
.L637:
	ldr	w0, [x19, 28]
	cmp	w0, wzr
	ble	.L615
	ldr	w0, [x19, 32]
	cmp	w0, wzr
	bgt	.L617
.L615:
	ldr	x1, .LC300
	mov	w0, 32
	bl	dprint_vfmw
	add	sp, x29, 0
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L638:
	ldr	w5, [x19, 36]
	b	.L610
	.p2align 3
.L641:
	ldr	w0, [x19, 20]
	cmp	w0, wzr
	bgt	.L637
	b	.L615
	.p2align 3
.L622:
	ldr	w0, [x19, 56]
	add	w0, w0, 339968
	add	w0, w0, 2576
	str	w0, [x21, 4]
	b	.L623
	.p2align 3
.L640:
	ldr	x1, .LC298
	mov	w0, 32
	bl	dprint_vfmw
	str	w23, [x19, 60]
	mov	w5, 16
	str	w5, [x19, 36]
	b	.L614
	.p2align 3
.L639:
	ldr	x1, .LC296
	mov	w5, w0
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, -1
	b	.L612
	.size	VCTRL_GetChanMemSizeWithOption, .-VCTRL_GetChanMemSizeWithOption
	.align	3
.LC294:
	.xword	.LANCHOR3
	.align	3
.LC296:
	.xword	.LC295
	.align	3
.LC298:
	.xword	.LC297
	.align	3
.LC300:
	.xword	.LC299
	.align	3
.LC301:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC303:
	.xword	.LC302
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanMemSize
	.type	VCTRL_GetChanMemSize, %function
VCTRL_GetChanMemSize:
	sub	sp, sp, #144
	stp	x29, x30, [sp, 32]
	add	x29, sp, 32
	stp	x19, x20, [sp, 48]
	stp	x21, x22, [sp, 64]
	stp	x23, x24, [sp, 80]
	stp	x25, x26, [sp, 96]
	stp	x27, x28, [sp, 112]
	uxtw	x24, w0
	mov	x0, x30
	mov	x21, x1
	mov	x20, x2
	bl	_mcount
	ldr	x25, .LC305
	lsl	x0, x24, 3
	sub	w1, w24, #18
	sub	x0, x0, x24
	cmp	w1, 7
	mov	x26, x24
	add	x0, x25, x0, lsl 2
	ldp	w23, w8, [x0, 8]
	ldp	w27, w22, [x0, 16]
	mov	w0, 6
	bhi	.L648
	ldr	x0, .LC306
	ldrb	w0, [x0,w1,uxtw]
	adr	x1, .Lrtx649
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx649:
	.section	.rodata
	.align	0
	.align	2
.L649:
	.byte	(.L662 - .Lrtx649) / 4
	.byte	(.L662 - .Lrtx649) / 4
	.byte	(.L662 - .Lrtx649) / 4
	.byte	(.L662 - .Lrtx649) / 4
	.byte	(.L662 - .Lrtx649) / 4
	.byte	(.L650 - .Lrtx649) / 4
	.byte	(.L650 - .Lrtx649) / 4
	.byte	(.L650 - .Lrtx649) / 4
	.text
	.p2align 3
.L662:
	mov	w0, 3
.L648:
	ldr	x19, .LC307
	sub	w0, w8, w0
	sub	w0, w0, #2
	ldr	w1, [x19, -208]
	tbnz	w1, #31, .L669
.L651:
	cmp	w0, 15
	mov	w6, 0
	bgt	.L652
	ldr	w6, [x19, -208]
	add	w0, w0, w6
	cmp	w0, 16
	bgt	.L670
.L652:
	add	w0, w6, w23
	ldr	w28, [x19, -204]
	cmp	w0, wzr
	ble	.L671
.L653:
	cmp	w0, 17
	mov	w23, 17
	csel	w23, w0, w23, le
.L654:
	cmp	w26, 23
	bhi	.L655
	add	w0, w6, w28
	add	w1, w8, w0
	add	w2, w27, w0
	cmp	w1, wzr
	add	w0, w22, w0
	ccmp	w2, wzr, 4, gt
	ccmp	w0, wzr, 4, gt
	ble	.L672
	mov	w22, w0
	mov	w27, w2
	mov	w8, w1
.L655:
	cmp	w26, 32
	bhi	.L665
	ldr	x28, .LC312
	ldr	x9, [x28, 128]
	cbz	x9, .L657
	lsl	x0, x24, 3
	add	x19, x19, 1072
	sub	x24, x0, x24
	mov	w5, w8
	lsl	x24, x24, 2
	mov	w7, -1
	add	x26, x25, x24
	mov	w6, 0
	mov	w4, w23
	mov	w1, 2147483647
	ldr	w2, [x25, x24]
	mov	w0, 1
	ldr	w3, [x26, 4]
	str	x19, [sp]
	blr	x9
	cbnz	w0, .L657
	ldr	w0, [x19, 4]
	add	w0, w0, 307200
	add	w0, w0, 2576
	str	w0, [x21]
	ldr	w0, [x26, 24]
	add	w0, w0, 32768
	str	w0, [x20]
	ldr	x8, [x28, 128]
	cbz	x8, .L659
	ldr	w3, [x26, 4]
	mov	w5, w27
	str	x19, [sp]
	mov	w7, -1
	mov	w6, 0
	mov	w4, w23
	ldr	w2, [x25, x24]
	mov	w1, 2147483647
	mov	w0, 1
	blr	x8
	cbnz	w0, .L659
	ldr	w0, [x19, 4]
	add	w0, w0, 307200
	add	w0, w0, 2576
	str	w0, [x21, 4]
	ldr	w0, [x26, 24]
	add	w0, w0, 32768
	str	w0, [x20, 4]
	ldr	x8, [x28, 128]
	cbz	x8, .L661
	ldr	w2, [x25, x24]
	mov	w7, -1
	ldr	w3, [x26, 4]
	mov	w5, w22
	mov	w4, w23
	mov	w6, 0
	mov	w1, 2147483647
	mov	w0, 1
	str	x19, [sp]
	blr	x8
	cbnz	w0, .L661
	ldr	w1, [x19, 4]
	add	w1, w1, 307200
	add	w1, w1, 2576
	str	w1, [x21, 8]
	ldr	w1, [x26, 24]
	add	w1, w1, 32768
	str	w1, [x20, 8]
.L656:
	add	sp, x29, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L650:
	ldr	x19, .LC307
	mov	w0, 1
	sub	w0, w8, w0
	sub	w0, w0, #2
	ldr	w1, [x19, -208]
	tbz	w1, #31, .L651
.L669:
	str	wzr, [x19, -208]
	b	.L651
	.p2align 3
.L670:
	sub	w0, w6, w0
	ldr	w28, [x19, -204]
	add	w6, w0, 16
	add	w0, w6, w23
	cmp	w0, wzr
	bgt	.L653
.L671:
	ldr	x1, .LC309
	mov	w3, w6
	mov	w2, w23
	mov	w0, 32
	stp	w8, w6, [x29, 104]
	bl	dprint_vfmw
	cmp	w23, 17
	mov	w0, 17
	csel	w23, w23, w0, le
	ldp	w8, w6, [x29, 104]
	b	.L654
	.p2align 3
.L672:
	str	w22, [sp]
	mov	w2, w8
	str	w6, [sp, 8]
	mov	w3, w6
	ldr	x1, .LC311
	mov	w5, w27
	ldr	w7, [x19, -204]
	mov	w0, 32
	str	w7, [sp, 16]
	mov	w4, w7
	str	w8, [x29, 108]
	bl	dprint_vfmw
	ldr	w8, [x29, 108]
	b	.L655
	.p2align 3
.L657:
	ldr	x1, .LC314
	mov	w2, 6562
	mov	w0, 0
	bl	dprint_vfmw
	add	sp, x29, 0
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L659:
	ldr	x1, .LC314
	mov	w2, 6586
	mov	w0, 0
	bl	dprint_vfmw
	add	sp, x29, 0
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L661:
	ldr	x1, .LC314
	mov	w2, 6610
	mov	w0, 0
	bl	dprint_vfmw
	add	sp, x29, 0
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L665:
	mov	w0, -1
	b	.L656
	.size	VCTRL_GetChanMemSize, .-VCTRL_GetChanMemSize
	.align	3
.LC305:
	.xword	CapItem
	.align	3
.LC306:
	.xword	.L649
	.align	3
.LC307:
	.xword	.LANCHOR3
	.align	3
.LC309:
	.xword	.LC308
	.align	3
.LC311:
	.xword	.LC310
	.align	3
.LC312:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC314:
	.xword	.LC313
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CloseDrivers
	.type	CloseDrivers, %function
CloseDrivers:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 96]
	cbz	w0, .L698
	ldr	w0, [x19, 112]
	cmp	w0, wzr
	cset	w21, ne
.L674:
	ldr	w20, [x19, 24]
	ldr	w22, [x19, 16]
	ldr	x23, [x19, 8]
	cbz	w20, .L675
	ldr	x0, .LC316
	ldr	x1, [x0, 120]
	cbz	x1, .L727
	mov	w0, 0
	blr	x1
	cbz	w21, .L678
.L679:
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
.L675:
	mov	w2, w20
	mov	x1, x23
	mov	w0, w22
	bl	MEM_DelMemRecord
	ldr	w20, [x19, 88]
	ldr	w22, [x19, 80]
	ldr	x23, [x19, 72]
	cbz	w20, .L681
	ldr	x0, .LC316
	ldr	x1, [x0, 120]
	cbz	x1, .L728
	mov	w0, 0
	blr	x1
	cbz	w21, .L684
.L685:
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	add	x0, x19, 64
	ldr	x3, [x3, 96]
	blr	x3
.L681:
	mov	w2, w20
	mov	x1, x23
	mov	w0, w22
	bl	MEM_DelMemRecord
	ldr	w20, [x19, 56]
	ldr	w22, [x19, 48]
	ldr	x23, [x19, 40]
	cbnz	w20, .L729
.L687:
	mov	x1, x23
	mov	w2, w20
	mov	w0, w22
	bl	MEM_DelMemRecord
	bl	SM_InstInit
	ldr	w21, [x19, 144]
	ldr	w22, [x19, 136]
	ldr	x23, [x19, 128]
	cbnz	w21, .L730
.L690:
	mov	w2, w21
	mov	x1, x23
	mov	w0, w22
	add	x20, x19, 216
	bl	MEM_DelMemRecord
	ldr	w21, [x19, 240]
	ldr	w22, [x19, 232]
	ldr	x23, [x19, 224]
	cbnz	w21, .L731
.L691:
	mov	w2, w21
	mov	x1, x23
	mov	w0, w22
	bl	MEM_DelMemRecord
	ldr	w21, [x19, 208]
	ldr	w22, [x19, 200]
	ldr	x23, [x19, 192]
	cbnz	w21, .L732
.L692:
	mov	w2, w21
	mov	x1, x23
	mov	w0, w22
	add	x20, x19, 152
	bl	MEM_DelMemRecord
	ldr	w21, [x19, 176]
	ldr	w22, [x19, 168]
	ldr	x23, [x19, 160]
	cbnz	w21, .L733
.L693:
	mov	w2, w21
	mov	w0, w22
	mov	x1, x23
	add	x19, x19, 248
	bl	MEM_DelMemRecord
	ldr	w20, [x19, 24]
	ldr	w21, [x19, 16]
	ldr	x22, [x19, 8]
	cbnz	w20, .L734
	mov	w2, w20
	mov	x1, x22
	mov	w0, w21
	bl	MEM_DelMemRecord
	ldr	x19, .LC319
	ldr	w0, [x19, 456]
	cmp	w0, 1
	beq	.L735
.L695:
	ldr	w0, [x19, 460]
	cmp	w0, 1
	beq	.L736
.L696:
	ldr	w0, [x19, 464]
	add	x19, x19, 128
	cmp	w0, 1
	beq	.L737
.L697:
	mov	w0, 0
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L729:
	mov	w0, 0
	bl	SM_CloseSCDDrv
	cbz	w21, .L688
.L689:
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	add	x0, x19, x2
	ldr	x3, [x3, 96]
	blr	x3
	b	.L687
	.p2align 3
.L730:
	mov	x1, x23
	mov	w0, w22
	bl	MEM_UnmapRegisterAddr
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	add	x0, x19, 120
	ldr	x3, [x3, 96]
	blr	x3
	b	.L690
	.p2align 3
.L734:
	mov	x1, x22
	mov	w0, w21
	bl	MEM_UnmapRegisterAddr
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	mov	w2, w20
	mov	x1, x22
	mov	w0, w21
	bl	MEM_DelMemRecord
	ldr	x19, .LC319
	ldr	w0, [x19, 456]
	cmp	w0, 1
	bne	.L695
.L735:
	ldr	x2, .LC317
	mov	w0, 127
	mov	x1, 0
	ldr	x2, [x2, 376]
	blr	x2
	str	wzr, [x19, 456]
	ldr	w0, [x19, 460]
	cmp	w0, 1
	bne	.L696
	.p2align 2
.L736:
	ldr	x2, .LC317
	mov	w0, 136
	mov	x1, 0
	add	x19, x19, 128
	ldr	x2, [x2, 376]
	blr	x2
	str	wzr, [x19, 332]
	ldr	w0, [x19, 336]
	cmp	w0, 1
	bne	.L697
.L737:
	ldr	x2, .LC317
	mov	x1, 0
	mov	w0, 181
	ldr	x2, [x2, 376]
	blr	x2
	str	wzr, [x19, 336]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L733:
	mov	x1, x23
	mov	w0, w22
	bl	MEM_UnmapRegisterAddr
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	mov	x0, x20
	ldr	x3, [x3, 96]
	blr	x3
	b	.L693
	.p2align 3
.L732:
	mov	x1, x23
	mov	w0, w22
	bl	MEM_UnmapRegisterAddr
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	add	x0, x19, 184
	ldr	x3, [x3, 96]
	blr	x3
	b	.L692
	.p2align 3
.L731:
	mov	x1, x23
	mov	w0, w22
	bl	MEM_UnmapRegisterAddr
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	mov	x0, x20
	ldr	x3, [x3, 96]
	blr	x3
	b	.L691
	.p2align 3
.L727:
	ldr	x1, .LC318
	mov	w2, 7098
	mov	w0, 0
	bl	dprint_vfmw
	cbnz	w21, .L679
	.p2align 2
.L678:
	ldr	w0, [x19, 16]
	ldr	x1, [x19, 8]
	bl	MEM_ReleaseMemBlock
	b	.L679
	.p2align 3
.L728:
	ldr	x1, .LC318
	mov	w2, 7119
	mov	w0, 0
	bl	dprint_vfmw
	cbnz	w21, .L685
	.p2align 2
.L684:
	ldr	w0, [x19, 80]
	ldr	x1, [x19, 72]
	bl	MEM_ReleaseMemBlock
	b	.L685
	.p2align 3
.L698:
	mov	w21, 0
	b	.L674
	.p2align 3
.L688:
	ldr	w0, [x19, 48]
	ldr	x1, [x19, 40]
	bl	MEM_ReleaseMemBlock
	b	.L689
	.size	CloseDrivers, .-CloseDrivers
	.align	3
.LC316:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC317:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC318:
	.xword	.LC22
	.align	3
.LC319:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	OpenDrivers
	.type	OpenDrivers, %function
OpenDrivers:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 120
	bl	_mcount
	mov	x2, x20
	mov	w1, 65536
	mov	w0, -121438208
	bl	MEM_MapRegisterAddr
	cbnz	w0, .L739
	ldr	w2, [x19, 144]
	add	x20, x19, 216
	ldr	x1, [x19, 128]
	ldr	w0, [x19, 136]
	bl	MEM_AddMemRecord
	mov	w0, 53248
	mov	x2, x20
	mov	w1, 4096
	movk	w0, 0xf8c3, lsl 16
	bl	MEM_MapRegisterAddr
	cbz	w0, .L783
	mov	w2, 53248
	ldr	x1, .LC322
	movk	w2, 0xf8c3, lsl 16
.L778:
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x3, .LC323
	mov	x2, 32
	mov	w1, 0
	mov	x0, x20
.L779:
	ldr	x3, [x3, 96]
	blr	x3
	mov	x0, x19
	bl	CloseDrivers
	mov	w0, -1
.L742:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L783:
	ldr	w2, [x20, 24]
	add	x21, x19, 184
	ldr	x1, [x20, 8]
	ldr	w0, [x20, 16]
	bl	MEM_AddMemRecord
	mov	w0, 49152
	mov	x2, x21
	mov	w1, 4096
	movk	w0, 0xf8c3, lsl 16
	bl	MEM_MapRegisterAddr
	cbnz	w0, .L743
	ldr	w0, [x19, 200]
	ldr	w2, [x19, 208]
	ldr	x1, [x19, 192]
	bl	MEM_AddMemRecord
	ldr	w0, [x19, 96]
	cbnz	w0, .L744
.L745:
	ldr	x20, .LC323
	mov	w0, 12
	ldr	x22, [x20, 136]
	bl	VFMW_GetString
	mov	w3, 0
	mov	x2, x0
	mov	x1, 20
	add	x0, x29, 64
	blr	x22
	ldr	x22, .LC324
	ldr	x0, [x22, 96]
	cbz	x0, .L746
	blr	x0
	cmp	w0, wzr
	mov	w2, w0
	ble	.L746
	mov	w21, 3
	mov	w4, 0
	mov	x3, x19
	mov	w1, 0
	add	x0, x29, 64
	str	w21, [x19, 28]
	bl	MEM_AllocMemBlock
	cbnz	w0, .L747
	ldr	w2, [x19, 24]
	ldr	x1, [x19, 8]
	ldr	w0, [x19, 16]
	bl	MEM_AddMemRecord
	ldr	x3, [x20, 96]
	mov	x2, 20
	mov	w1, 0
	add	x0, x29, 88
	blr	x3
	str	wzr, [x29, 96]
	ldr	w2, [x19, 16]
	ldr	w0, [x19, 24]
	ldr	x1, [x22, 112]
	stp	w2, w0, [x29, 88]
	cbz	x1, .L748
	add	x0, x29, 88
	blr	x1
	cbnz	w0, .L748
	str	w21, [x19, 92]
	mov	w0, 13
	add	x22, x19, 64
	ldr	x23, [x20, 136]
	bl	VFMW_GetString
	mov	w3, 0
	mov	x2, x0
	mov	x1, 20
	add	x0, x29, 64
	blr	x23
	mov	w4, 1
	mov	x3, x22
	mov	w2, 16384
	mov	w1, 0
	add	x0, x29, 64
	bl	MEM_AllocMemBlock
	cbnz	w0, .L749
	ldr	w2, [x19, 88]
	add	x23, x19, 32
	ldr	x1, [x19, 72]
	ldr	w0, [x19, 80]
	bl	MEM_AddMemRecord
	ldr	x2, .LC325
	mov	w0, 0
	ldr	w1, [x19, 80]
	str	w1, [x2, 1212]
	ldr	w1, [x19, 80]
	add	w1, w1, 4096
	add	w1, w1, 1024
	str	w1, [x2, 1216]
	bl	VDMDRV_Init
	ldr	x22, [x20, 136]
	mov	w0, 14
	bl	VFMW_GetString
	mov	x2, x0
	mov	w3, 0
	mov	x1, 20
	add	x0, x29, 64
	blr	x22
	str	w21, [x19, 60]
	mov	w2, 61440
	mov	w4, 0
	mov	x3, x23
	movk	w2, 0xa, lsl 16
	mov	w1, 0
	add	x0, x29, 64
	bl	MEM_AllocMemBlock
	cbnz	w0, .L784
	ldr	w2, [x19, 56]
	ldr	x1, [x19, 40]
	ldr	w0, [x19, 48]
	bl	MEM_AddMemRecord
	ldr	w1, [x19, 56]
	mov	w2, 0
	ldr	w0, [x19, 48]
	bl	SM_OpenSCDDrv
	ldr	x1, .LC337
	mov	w2, 0
	cbnz	w0, .L777
.L755:
	bl	SM_InstInit
	ldr	x21, [x20, 368]
	mov	w0, 0
	bl	VFMW_GetString
	mov	x3, x0
	ldr	x1, .LC340
	mov	x4, 0
	mov	x2, 32
	mov	w0, 127
	blr	x21
	cbnz	w0, .L785
	ldr	x21, .LC343
	mov	w0, 3
	mov	w22, 1
	ldr	x24, [x20, 368]
	add	x21, x21, 128
	str	w22, [x21, 328]
	bl	VFMW_GetString
	ldr	x1, .LC344
	mov	x3, x0
	mov	x4, 0
	mov	x2, 32
	mov	w0, 136
	blr	x24
	cbnz	w0, .L786
	mov	w0, 11
	ldr	x24, [x20, 368]
	str	w22, [x21, 332]
	bl	VFMW_GetString
	mov	x3, x0
	ldr	x1, .LC345
	mov	x4, 0
	mov	x2, 32
	mov	w0, 181
	blr	x24
	cbnz	w0, .L787
	str	w22, [x21, 336]
	b	.L742
	.p2align 3
.L744:
	ldr	w1, [x19, 112]
	cbz	w1, .L745
	ldr	x22, .LC324
	ldr	x1, [x22, 96]
	cbz	x1, .L759
	blr	x1
	mov	w20, w0
	mov	w21, w0
	ldr	w0, [x19, 96]
.L752:
	ldr	x1, [x19, 104]
	mov	w2, w21
	str	x1, [x19, 8]
	str	w0, [x19, 16]
	str	w21, [x19, 24]
	bl	MEM_AddMemRecord
	str	wzr, [x29, 96]
	ldr	w2, [x19, 16]
	ldr	w0, [x19, 24]
	ldr	x1, [x22, 112]
	stp	w2, w0, [x29, 88]
	cbz	x1, .L753
	add	x0, x29, 88
	blr	x1
	cbnz	w0, .L753
	bl	VDMDRV_Init
	add	x23, x19, 32
	ldr	x3, [x19, 104]
	mov	w4, 61440
	ldr	w0, [x19, 96]
	movk	w4, 0xa, lsl 16
	add	x3, x3, x20, sxtw
	mov	w2, w4
	add	w0, w21, w0
	mov	x1, x3
	str	x3, [x19, 40]
	str	w0, [x19, 48]
	str	w4, [x19, 56]
	bl	MEM_AddMemRecord
	ldr	w0, [x19, 48]
	mov	w2, 0
	ldr	w1, [x19, 56]
	bl	SM_OpenSCDDrv
	cbnz	w0, .L754
	ldr	x20, .LC323
	b	.L755
	.p2align 3
.L739:
	ldr	x1, .LC322
	mov	w2, -121438208
	b	.L778
.L746:
	ldr	x1, .LC331
	mov	w2, 0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x3, [x20, 96]
	mov	x2, 32
	mov	w1, 0
	mov	x0, x21
	blr	x3
	mov	x0, x19
	bl	CloseDrivers
	mov	w0, -1
	b	.L742
.L787:
	ldr	x1, .LC342
	mov	w2, 181
.L777:
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x3, [x20, 96]
	mov	x2, 32
	mov	w1, 0
	mov	x0, x23
	blr	x3
	mov	x0, x19
	bl	CloseDrivers
	mov	w0, -1
	b	.L742
.L759:
	mov	w21, -1
	mov	w20, w21
	b	.L752
.L786:
	ldr	x1, .LC342
	mov	w2, 136
	b	.L777
.L785:
	ldr	x1, .LC342
	mov	w2, 127
	b	.L777
.L748:
	ldr	x1, .LC327
.L780:
	mov	w2, 0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x3, [x20, 96]
	mov	x2, 32
	mov	w1, 0
	mov	x0, x19
	blr	x3
	mov	x0, x19
	bl	CloseDrivers
	mov	w0, -1
	b	.L742
.L784:
	ldr	x1, .LC339
	mov	w2, 0
	b	.L777
.L747:
	ldr	x1, .LC333
	b	.L780
.L749:
	ldr	x1, .LC335
	mov	w2, 0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x3, [x20, 96]
	mov	x2, 32
	mov	w1, 0
	mov	x0, x22
	blr	x3
	mov	x0, x19
	bl	CloseDrivers
	mov	w0, -1
	b	.L742
.L753:
	ldr	x1, .LC327
	mov	w2, 0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x3, .LC323
	mov	x2, 32
	mov	w1, 0
	mov	x0, x19
	b	.L779
.L743:
	ldr	x1, .LC322
	mov	w2, 49152
	movk	w2, 0xf8c3, lsl 16
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x3, .LC323
	mov	x2, 32
	mov	w1, 0
	mov	x0, x21
	b	.L779
.L754:
	ldr	x1, .LC329
	mov	w2, 0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x3, .LC323
	mov	x2, 32
	mov	w1, 0
	mov	x0, x23
	b	.L779
	.size	OpenDrivers, .-OpenDrivers
	.align	3
.LC322:
	.xword	.LC321
	.align	3
.LC323:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC324:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC325:
	.xword	g_HwMem
	.align	3
.LC327:
	.xword	.LC326
	.align	3
.LC329:
	.xword	.LC328
	.align	3
.LC331:
	.xword	.LC330
	.align	3
.LC333:
	.xword	.LC332
	.align	3
.LC335:
	.xword	.LC334
	.align	3
.LC337:
	.xword	.LC336
	.align	3
.LC339:
	.xword	.LC338
	.align	3
.LC340:
	.xword	VCTRL_Isr_0
	.align	3
.LC342:
	.xword	.LC341
	.align	3
.LC343:
	.xword	.LANCHOR0
	.align	3
.LC344:
	.xword	VCTRL_Scd_Isr
	.align	3
.LC345:
	.xword	VCTRL_MMU_ISR
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_OpenVfmw
	.type	VCTRL_OpenVfmw, %function
VCTRL_OpenVfmw:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x23, x0
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC347
	mov	x2, 8
	mov	w1, 0
	add	x0, x29, 64
	ldr	x3, [x20, 96]
	blr	x3
	ldr	x22, .LC348
	ldr	w19, [x22, 128]
	cbnz	w19, .L819
	ldr	x21, .LC349
	mov	w1, 1024
	ldr	x2, [x20, 160]
	add	x24, x21, 2344
	str	xzr, [x29, 72]
	mov	x0, x24
	blr	x2
	ldr	x1, .LC351
	mov	x0, x24
	ldr	x2, [x20, 192]
	blr	x2
	str	x0, [x29, 72]
	cbz	x0, .L791
	ldr	x1, .LC353
	add	x0, x29, 72
	ldr	x2, [x20, 184]
	blr	x2
	cbz	x0, .L811
	ldr	x1, .LC355
	add	x0, x29, 72
	ldr	x2, [x20, 184]
	blr	x2
	cbz	x0, .L820
	ldr	x3, [x20, 168]
	mov	w2, 0
	mov	x1, 0
	blr	x3
	mov	w2, w0
	str	w0, [x21, -208]
	add	w0, w2, 4096
	mov	w1, 10000
	add	w0, w0, 904
	cmp	w0, w1
	bhi	.L796
.L826:
	mov	w0, 5000
	mov	w1, -5000
	cmp	w2, w0
	csel	w0, w2, w0, le
	cmp	w0, w1
	csel	w0, w0, w1, ge
.L792:
	str	w0, [x21, -208]
.L794:
	add	x24, x21, 2344
	ldr	x2, [x20, 160]
	mov	w1, 1024
	mov	x0, x24
	str	xzr, [x29, 72]
	blr	x2
	ldr	x1, .LC362
	mov	x0, x24
	ldr	x2, [x20, 192]
	blr	x2
	str	x0, [x29, 72]
	cbz	x0, .L797
	ldr	x1, .LC353
	add	x0, x29, 72
	ldr	x2, [x20, 184]
	blr	x2
	cbz	x0, .L798
	ldr	x1, .LC355
	add	x0, x29, 72
	ldr	x2, [x20, 184]
	blr	x2
	cbz	x0, .L821
	ldr	x3, [x20, 168]
	mov	w2, 0
	mov	x1, 0
	blr	x3
	mov	w2, w0
	str	w0, [x21, -204]
	add	w0, w2, 4096
	mov	w1, 10000
	add	w0, w0, 904
	cmp	w0, w1
	bhi	.L802
.L825:
	mov	w19, 5000
	mov	w0, -5000
	cmp	w2, w19
	csel	w19, w2, w19, le
	cmp	w19, w0
	csel	w19, w19, w0, ge
.L798:
	str	w19, [x21, -204]
.L800:
	ldr	w0, [x21, 3368]
	str	xzr, [x29, 72]
	cbz	w0, .L803
	ldr	x2, .LC365
	and	w4, w0, 1
	ubfx	x3, x0, 1, 1
	ubfx	x1, x0, 2, 1
	strb	w4, [x21, 3372]
	ubfx	x0, x0, 3, 1
	strb	w3, [x2]
	strb	w1, [x21, 3373]
	strb	w0, [x21, 3374]
.L804:
	ldr	x3, [x20, 96]
	mov	x2, 352
	mov	w1, 0
	add	x0, x22, 128
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 128
	mov	w1, 0
	mov	x0, x22
	blr	x3
	ldr	x1, .LC368
	mov	w2, 134217728
	add	x0, x29, 64
	str	x1, [x21, 3376]
	mov	w1, 75497472
	stp	w2, w1, [x29, 64]
	bl	MEM_InitMemManager
	bl	FSP_Init
	ldr	x19, .LC369
	ldr	x3, [x20, 96]
	mov	x2, 192
	mov	w1, 0
	mov	x0, x19
	blr	x3
	add	x0, x19, 64
	add	x1, x19, 128
	mov	w2, -1
	.p2align 2
.L808:
	str	w2, [x0], 4
	cmp	x1, x0
	bne	.L808
	ldr	x3, [x20, 96]
	mov	x2, 64
	mov	w1, 0
	add	x0, x21, 3384
	add	x19, x22, 160
	blr	x3
	ldr	x3, [x20, 104]
	mov	x2, 24
	add	x1, x23, 8
	add	x0, x22, 256
	blr	x3
	mov	x0, x19
	bl	OpenDrivers
	cbnz	w0, .L819
	ldr	x2, [x20, 240]
	mov	w1, 1
	blr	x2
	ldr	x2, .LC372
	add	x0, x22, 144
	ldr	x1, .LC374
	ldr	x3, [x20, 360]
	blr	x3
	cbnz	w0, .L822
	mov	w1, 1
	str	w1, [x22, 128]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L803:
	add	x19, x21, 2344
	ldr	x2, [x20, 160]
	mov	w1, 1024
	mov	x0, x19
	blr	x2
	ldr	x1, .LC367
	mov	x0, x19
	ldr	x2, [x20, 192]
	blr	x2
	str	x0, [x29, 72]
	cbz	x0, .L804
	ldr	x1, .LC353
	add	x0, x29, 72
	ldr	x2, [x20, 184]
	blr	x2
	cbz	x0, .L823
	ldr	x1, .LC355
	add	x0, x29, 72
	ldr	x2, [x20, 184]
	blr	x2
	cbz	x0, .L824
	ldr	x3, [x20, 168]
	mov	x1, 0
	mov	w2, 0
	blr	x3
	mov	w1, w0
	str	w0, [x21, 3368]
.L806:
	ldr	x2, .LC365
	and	w4, w1, 1
	ubfx	x3, x1, 1, 1
	ubfx	x0, x1, 2, 1
	strb	w4, [x21, 3372]
	ubfx	x1, x1, 3, 1
	strb	w3, [x2]
	strb	w0, [x21, 3373]
	strb	w1, [x21, 3374]
	b	.L804
	.p2align 3
.L797:
	ldr	w2, [x21, -204]
	mov	w1, 10000
	add	w0, w2, 4096
	add	w0, w0, 904
	cmp	w0, w1
	bls	.L825
.L802:
	ldr	x1, .LC364
	mov	w0, 32
	bl	dprint_vfmw
	ldr	w19, [x21, -204]
	mov	w1, 5000
	mov	w0, -5000
	cmp	w19, w1
	csel	w19, w19, w1, le
	cmp	w19, w0
	csel	w19, w19, w0, ge
	b	.L798
	.p2align 3
.L791:
	ldr	w2, [x21, -208]
	mov	w1, 10000
	add	w0, w2, 4096
	add	w0, w0, 904
	cmp	w0, w1
	bls	.L826
.L796:
	ldr	x1, .LC360
	mov	w0, 32
	bl	dprint_vfmw
	ldr	w0, [x21, -208]
	mov	w2, 5000
	mov	w1, -5000
	cmp	w0, w2
	csel	w0, w0, w2, le
	cmp	w0, w1
	csel	w0, w0, w1, ge
	b	.L792
	.p2align 3
.L811:
	mov	w0, 0
	b	.L792
.L823:
	ldr	w1, [x21, 3368]
	b	.L806
.L822:
	mov	x0, x19
	bl	CloseDrivers
.L819:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	w0, -1
	ldp	x29, x30, [sp], 80
	ret
.L821:
	ldr	x2, .LC356
	mov	w3, 8451
	ldr	x1, .LC358
	mov	w0, 22
	add	x2, x2, 48
	bl	dprint_vfmw
	b	.L800
.L820:
	ldr	x2, .LC356
	mov	w3, 8404
	ldr	x1, .LC358
	mov	w0, 22
	add	x2, x2, 24
	bl	dprint_vfmw
	b	.L794
.L824:
	ldr	x2, .LC356
	mov	w3, 8508
	ldr	x1, .LC358
	mov	w0, 22
	add	x2, x2, 72
	bl	dprint_vfmw
	b	.L804
	.size	VCTRL_OpenVfmw, .-VCTRL_OpenVfmw
	.align	3
.LC347:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC348:
	.xword	.LANCHOR0
	.align	3
.LC349:
	.xword	.LANCHOR3
	.align	3
.LC351:
	.xword	.LC350
	.align	3
.LC353:
	.xword	.LC352
	.align	3
.LC355:
	.xword	.LC354
	.align	3
.LC356:
	.xword	.LANCHOR2
	.align	3
.LC358:
	.xword	.LC357
	.align	3
.LC360:
	.xword	.LC359
	.align	3
.LC362:
	.xword	.LC361
	.align	3
.LC364:
	.xword	.LC363
	.align	3
.LC365:
	.xword	.LANCHOR4
	.align	3
.LC367:
	.xword	.LC366
	.align	3
.LC368:
	.xword	VCTRL_EventReport
	.align	3
.LC369:
	.xword	g_ChanCtx
	.align	3
.LC372:
	.xword	VFMW_CTRL_thread_proc
	.align	3
.LC374:
	.xword	.LC373
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetNextDecodeMode
	.type	GetNextDecodeMode, %function
GetNextDecodeMode:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w21, w0
	mov	x0, x30
	mov	w19, w2
	mov	w22, w1
	bl	_mcount
	add	w20, w19, 7
	mov	w0, 21846
	lsl	w3, w19, 1
	movk	w0, 0x5555, lsl 16
	cmp	w19, wzr
	csel	w20, w20, w19, lt
	ldr	x1, .LC378
	smull	x0, w3, w0
	asr	w20, w20, 3
	mov	w5, w22
	lsr	x0, x0, 32
	mov	w4, w19
	sub	w3, w0, w3, asr 31
	mov	w2, w20
	mov	w0, 30
	bl	dprint_vfmw
	cmp	w21, 3
	beq	.L832
	cmp	w19, w22
	mov	w19, w21
	blt	.L833
.L829:
	ldr	x1, .LC384
	mov	w3, w19
	mov	w2, w21
	mov	w0, 30
	bl	dprint_vfmw
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L833:
	ldr	x1, .LC382
	mov	w0, 30
	mov	w19, 3
	bl	dprint_vfmw
	b	.L829
	.p2align 3
.L832:
	cmp	w20, w22
	mov	w19, w21
	ble	.L829
	ldr	x1, .LC380
	mov	w0, 30
	mov	w19, 0
	bl	dprint_vfmw
	b	.L829
	.size	GetNextDecodeMode, .-GetNextDecodeMode
	.align	3
.LC378:
	.xword	.LC377
	.align	3
.LC380:
	.xword	.LC379
	.align	3
.LC382:
	.xword	.LC381
	.align	3
.LC384:
	.xword	.LC383
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanIDByCtx
	.type	VCTRL_GetChanIDByCtx, %function
VCTRL_GetChanIDByCtx:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x3, .LC385
	mov	x0, 0
	.p2align 2
.L837:
	ldr	x2, [x3, x0, lsl 3]
	add	x1, x2, 294912
	add	x1, x1, 2584
	cbz	x2, .L835
	cmp	x19, x1
	beq	.L836
.L835:
	add	x0, x0, 1
	cmp	x0, 16
	bne	.L837
	mov	w0, -1
.L836:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_GetChanIDByCtx, .-VCTRL_GetChanIDByCtx
	.align	3
.LC385:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanIDByMemAddr
	.type	VCTRL_GetChanIDByMemAddr, %function
VCTRL_GetChanIDByMemAddr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x4, .LC386
	mov	x0, 0
	.p2align 2
.L847:
	ldr	x1, [x4, x0, lsl 3]
	mov	w5, w0
	cbz	x1, .L844
	ldr	w2, [x1, 280]
	cmp	w2, w19
	bhi	.L845
	ldr	w3, [x1, 288]
	add	w2, w2, w3
	cmp	w19, w2
	bcc	.L846
.L845:
	ldr	w2, [x1, 312]
	cmp	w19, w2
	bcc	.L844
	ldr	w1, [x1, 320]
	add	w2, w2, w1
	cmp	w19, w2
	bcc	.L849
.L844:
	add	x0, x0, 1
	cmp	x0, 16
	bne	.L847
	mov	w0, -1
.L846:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L849:
	mov	w0, w5
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_GetChanIDByMemAddr, .-VCTRL_GetChanIDByMemAddr
	.align	3
.LC386:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DecChanSyntax
	.type	DecChanSyntax, %function
DecChanSyntax:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L927
	ldr	x20, .LC389
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L928
	ldr	w22, [x0, 16]
	cmp	w22, 1
	bne	.L929
	ldr	w21, [x0, 32]
	cbnz	w21, .L930
	ldr	x24, .LC394
	mov	w23, 0
	str	wzr, [x0, 440]
	ldr	x0, [x24]
	blr	x0
	ldr	x25, .LC395
	ldr	x1, .LC396
	add	x2, x25, 3448
	ldr	w3, [x1]
	ldr	w1, [x2, w19, sxtw 2]
	sub	w1, w0, w1
	cmp	w1, w3
	bls	.L860
	str	w0, [x2, w19, sxtw 2]
	mov	w23, w22
.L860:
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w0, [x0, 672]
	sub	w1, w0, #10
	cmp	w1, 2
	bhi	.L861
	ldr	w1, [x20, 4076]
	tbnz	x1, 0, .L861
.L862:
	mov	w0, w19
	bl	VCTRL_GetImageBuffer
	cmp	w0, 1
	beq	.L863
	cmp	w23, 1
	mov	w21, -3
	beq	.L931
.L902:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L861:
	and	w1, w0, -17
	cmp	w1, 4
	ccmp	w0, 14, 4, ne
	beq	.L862
	mov	w0, w19
	bl	VCTRL_GetDecParam
	cbz	x0, .L862
	cmp	w23, 1
	mov	w21, -2
	bne	.L902
	ldr	x1, .LC398
	mov	w2, w19
	mov	w0, 23
	bl	dprint_vfmw
	b	.L902
	.p2align 3
.L863:
	ldr	x26, [x20, w19, sxtw 3]
	ldr	w2, [x26, 1512]
	cbz	w2, .L864
	cmp	w2, 1
	beq	.L932
	add	x26, x26, 290816
	ldr	w3, [x26, 2440]
.L886:
	ldr	x1, .LC418
	mov	w0, 31
	mov	w21, -1
	bl	dprint_vfmw
	b	.L902
	.p2align 3
.L864:
	add	x22, x26, 816
	ldrb	w0, [x22, 1]
	cbnz	w0, .L867
	ldr	w0, [x26, 1552]
	cbz	w0, .L933
.L867:
	ldr	x1, .LC404
	mov	w0, 29
	ldr	x2, [x22, 32]
	ldr	x3, [x22, 48]
	bl	dprint_vfmw
	ldr	x4, [x20, w19, sxtw 3]
	ldr	w1, [x4, 52]
	cmp	w1, 1
	beq	.L934
	sub	w0, w1, #2
	cmp	w0, 1
	ccmp	w1, 6, 4, hi
	bne	.L870
.L887:
	add	x2, x26, 824
	add	x0, x4, 1232
	bl	IsNewpicSeg
	cbnz	w0, .L871
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w1, [x0, 52]
	cmp	w1, 3
	ldr	x1, [x0, 744]
	beq	.L925
	cmn	x1, #1
	beq	.L873
.L925:
	str	x1, [x0, 736]
.L873:
	ldr	x3, [x0, 760]
	mov	x2, -1
	ldr	x1, [x0, 776]
	str	x2, [x0, 744]
	str	x3, [x0, 752]
	str	x1, [x0, 768]
	cbnz	x1, .L874
	ldr	x1, [x22, 56]
	str	x1, [x0, 768]
.L874:
	ldr	w2, [x0, 796]
	ldr	w1, [x0, 800]
	str	w2, [x0, 784]
	str	w1, [x0, 788]
	ldr	w2, [x0, 804]
	ldr	w1, [x0, 812]
	str	w2, [x0, 792]
	str	w1, [x0, 808]
	ldr	x2, [x0, 736]
	mov	w0, 29
	ldr	x1, .LC406
	bl	dprint_vfmw
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w1, [x0, 52]
	cmp	w1, 3
	bne	.L875
	ldr	x1, [x22, 32]
.L926:
	str	x1, [x0, 744]
.L877:
	ldr	x1, [x22, 48]
	str	x1, [x0, 760]
	ldr	x1, [x22, 56]
	str	x1, [x0, 776]
	ldr	w1, [x22, 64]
	str	w1, [x0, 796]
	ldr	w1, [x22, 68]
	str	w1, [x0, 800]
	ldr	w1, [x22, 72]
	str	w1, [x0, 804]
	ldr	w1, [x22, 76]
	str	w1, [x0, 812]
.L876:
	ldr	x2, [x0, 752]
	mov	w0, 29
	ldr	x1, .LC408
	bl	dprint_vfmw
	b	.L878
	.p2align 3
.L931:
	ldr	x1, .LC400
	mov	w2, w19
	mov	w0, 23
	bl	dprint_vfmw
	b	.L902
	.p2align 3
.L934:
	add	x0, x4, 294912
	ldr	x0, [x0, 2584]
	ldrsb	w1, [x0, 556]
	cmp	w1, 1
	beq	.L887
	.p2align 2
.L870:
	ldr	x2, [x22, 32]
	cmn	x2, #1
	beq	.L935
	str	x2, [x4, 736]
.L880:
	ldr	x3, [x22, 48]
	str	x3, [x4, 752]
	ldr	x1, .LC410
	ldr	x0, [x22, 56]
	str	x0, [x4, 768]
	mov	w0, 29
	ldr	w5, [x22, 64]
	str	w5, [x4, 784]
	ldr	w5, [x22, 68]
	str	w5, [x4, 788]
	ldr	w5, [x22, 72]
	str	w5, [x4, 792]
	ldr	w5, [x22, 76]
	str	w5, [x4, 808]
	bl	dprint_vfmw
.L878:
	mov	w0, 3
	bl	CHECK_REC_POS_ENABLE
	cmp	w23, 1
	ccmp	w0, wzr, 4, eq
	bne	.L936
.L881:
	ldr	x1, [x20, w19, sxtw 3]
	mov	x0, x22
	add	x2, x1, 294912
	ldr	w1, [x1, 672]
	add	x2, x2, 2584
	bl	DecodeStream
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w1, [x0, 672]
	cmp	w1, 16
	beq	.L937
.L882:
	mov	w0, 4
	bl	CHECK_REC_POS_ENABLE
	cbz	w0, .L902
	ldr	x0, [x24]
	add	x25, x25, 3512
	blr	x0
	mov	w1, w0
	ldr	x2, .LC414
	ldr	w0, [x25, w19, sxtw 2]
	ldr	w2, [x2]
	sub	w0, w1, w0
	cmp	w0, w2
	bls	.L902
	mov	w0, w19
	str	w1, [x25, w19, sxtw 2]
	bl	VCTRL_GetDecParam
	cbz	x0, .L902
	ldr	x0, .LC411
	lsl	w2, w19, 24
	add	w2, w2, 262144
	mov	w1, 7897
	add	x0, x0, 88
	bl	DBG_AddTrace
	b	.L902
	.p2align 3
.L871:
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w1, [x0, 52]
	cmp	w1, 3
	beq	.L876
.L875:
	ldr	x1, [x22, 32]
	cmn	x1, #1
	bne	.L926
	b	.L877
	.p2align 3
.L933:
	add	x1, x26, 824
	mov	w0, w19
	bl	SM_ReadSegStream
	cbz	w0, .L867
	cmp	w23, 1
	beq	.L938
.L868:
	ldr	x0, [x20, w19, sxtw 3]
	mov	w1, 1
	mov	w21, -4
	str	w1, [x0, 440]
	b	.L902
	.p2align 3
.L929:
	ldr	x3, .LC391
	mov	w2, 7649
	ldr	x1, .LC388
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L902
	.p2align 3
.L930:
	ldr	x3, .LC393
	mov	w2, 7650
	ldr	x1, .LC388
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L902
	.p2align 3
.L937:
	ldr	w1, [x0, 1576]
	cmp	w1, 1
	beq	.L882
	add	x0, x0, 294912
	add	x0, x0, 2584
	bl	IsMain10Profile
	cbnz	w0, .L939
	ldr	x0, [x20, w19, sxtw 3]
	str	wzr, [x0, 1576]
	b	.L882
	.p2align 3
.L932:
	add	x0, x26, 290816
	ldr	w3, [x0, 2440]
	tst	w3, -3
	beq	.L864
	cmp	w3, 1
	bne	.L886
	ldr	x1, .LC416
	mov	w0, 31
	mov	w21, -3
	bl	dprint_vfmw
	b	.L902
	.p2align 3
.L935:
	ldr	x2, [x4, 736]
	b	.L880
	.p2align 3
.L927:
	ldr	x3, .LC387
	mov	w2, 7647
	ldr	x1, .LC388
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L902
	.p2align 3
.L936:
	ldr	x0, .LC411
	lsl	w2, w19, 24
	add	w2, w2, 196608
	mov	w1, 7859
	add	x0, x0, 88
	bl	DBG_AddTrace
	b	.L881
.L939:
	ldr	x2, .LC411
	mov	w3, w19
	ldr	x1, .LC413
	mov	w0, 0
	add	x2, x2, 88
	bl	dprint_vfmw
	ldr	x0, [x20, w19, sxtw 3]
	mov	w1, 1
	str	w1, [x0, 1576]
	bl	VDMDRV_AddMatchMode
	b	.L882
.L928:
	ldr	x3, .LC390
	mov	w2, 7648
	ldr	x1, .LC388
	mov	w21, -1
	bl	dprint_vfmw
	b	.L902
.L938:
	ldr	x1, .LC402
	mov	w2, w19
	mov	w0, 23
	bl	dprint_vfmw
	b	.L868
	.size	DecChanSyntax, .-DecChanSyntax
	.align	3
.LC387:
	.xword	.LC51
	.align	3
.LC388:
	.xword	.LC2
	.align	3
.LC389:
	.xword	.LANCHOR0
	.align	3
.LC390:
	.xword	.LC186
	.align	3
.LC391:
	.xword	.LC229
	.align	3
.LC393:
	.xword	.LC392
	.align	3
.LC394:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC395:
	.xword	.LANCHOR3
	.align	3
.LC396:
	.xword	g_TraceBsPeriod
	.align	3
.LC398:
	.xword	.LC397
	.align	3
.LC400:
	.xword	.LC399
	.align	3
.LC402:
	.xword	.LC401
	.align	3
.LC404:
	.xword	.LC403
	.align	3
.LC406:
	.xword	.LC405
	.align	3
.LC408:
	.xword	.LC407
	.align	3
.LC410:
	.xword	.LC409
	.align	3
.LC411:
	.xword	.LANCHOR2
	.align	3
.LC413:
	.xword	.LC412
	.align	3
.LC414:
	.xword	g_TraceFramePeriod
	.align	3
.LC416:
	.xword	.LC415
	.align	3
.LC418:
	.xword	.LC417
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DecSyntax
	.type	DecSyntax, %function
DecSyntax:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x0, x30
	bl	_mcount
	ldr	x23, .LC421
	ldr	x22, .LC422
	mov	x0, 0
	b	.L943
	.p2align 3
.L967:
	ldr	w1, [x1, 28]
	str	w1, [x2, x0, lsl 2]
	add	x0, x0, 1
	cmp	x0, 16
	beq	.L966
.L943:
	ldr	x1, [x23, x0, lsl 3]
	add	x2, x22, 3576
	cbnz	x1, .L967
	add	x1, x22, 3576
	str	wzr, [x1, x0, lsl 2]
	add	x0, x0, 1
	cmp	x0, 16
	bne	.L943
.L966:
	ldr	x24, .LC423
	mov	w20, -1
	.p2align 2
.L944:
	ldr	w3, [x22, 3576]
	add	x1, x22, 3580
	mov	w0, 0
	mov	w19, 0
	.p2align 2
.L945:
	add	w0, w0, 1
	cmp	w0, 16
	beq	.L968
.L947:
	ldr	w2, [x1]
	add	x1, x1, 4
	cmp	w3, w2
	bge	.L945
	mov	w19, w0
	add	w0, w0, 1
	cmp	w0, 16
	mov	w3, w2
	bne	.L947
.L968:
	add	x2, x22, 3576
	add	x0, x23, 128
	add	w1, w19, 1
	cmp	w3, wzr
	sxtw	x21, w19
	str	wzr, [x2, w19, sxtw 2]
	str	w1, [x0, 28]
	ble	.L948
	ldr	x0, [x23, w19, sxtw 3]
	cbz	x0, .L944
	ldr	w1, [x0, 16]
	cmp	w1, 1
	bne	.L944
	ldr	w1, [x0, 76]
	cmp	w1, wzr
	ble	.L950
	ldr	w0, [x0, 80]
	cbz	w0, .L969
.L950:
	mov	w21, 200
	b	.L955
	.p2align 3
.L953:
	subs	w21, w21, #1
	cbnz	w0, .L944
	beq	.L944
.L955:
	mov	w0, w19
	bl	DecChanSyntax
	cmn	w0, #1
	mov	w20, w0
	bne	.L953
	ldr	x4, [x22, 3376]
	cbz	x4, .L954
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	mov	w0, w19
	blr	x4
.L954:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 96
	ret
.L969:
	add	x1, x29, 72
	mov	w0, w19
	bl	SM_GetInfo
	cbnz	w0, .L970
.L951:
	ldr	x20, [x23, x21, lsl 3]
	ldr	w1, [x29, 84]
	ldr	w2, [x20, 76]
	ldr	w0, [x20, 688]
	bl	GetNextDecodeMode
	str	w0, [x20, 688]
	ldr	x1, [x23, x21, lsl 3]
	ldr	w0, [x1, 688]
	cbz	w0, .L952
	str	wzr, [x1, 680]
	str	wzr, [x1, 684]
	b	.L950
.L948:
	str	wzr, [x0, 28]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 96
	ret
.L952:
	ldr	w0, [x1, 72]
	str	w0, [x1, 680]
	str	w0, [x1, 684]
	b	.L950
.L970:
	ldr	x3, [x24, 96]
	mov	x2, 24
	mov	w1, 0
	add	x0, x29, 72
	blr	x3
	b	.L951
	.size	DecSyntax, .-DecSyntax
	.align	3
.LC421:
	.xword	.LANCHOR0
	.align	3
.LC422:
	.xword	.LANCHOR3
	.align	3
.LC423:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_RunProcess
	.type	VCTRL_RunProcess, %function
VCTRL_RunProcess:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC427
	ldr	x20, .LC426
	mov	w1, 100
	ldr	x21, .LC428
	ldr	w0, [x19, 3640]
	str	w1, [x20, 152]
	cmp	w0, wzr
	ble	.L972
	ldr	x1, [x21, 232]
	blr	x1
.L972:
	ldr	x1, [x21, 24]
	mov	w0, 0
	mov	w22, 0
	blr	x1
	bl	SM_Wakeup
	ldr	w0, [x19, 3644]
	mov	w1, 101
	str	w1, [x20, 152]
	cbz	w0, .L979
.L973:
	mov	w0, 102
	mov	w19, 0
	str	w0, [x20, 152]
	bl	VDMDRV_WakeUpVdm
	.p2align 2
.L974:
	mov	w0, w19
	mov	w1, 0
	add	w19, w19, 1
	bl	DeleteRawPacketInBuffer
	cmp	w19, 16
	bne	.L974
	ldr	x1, [x21, 32]
	mov	w0, 0
	blr	x1
	ldr	x3, [x21, 256]
	mov	w0, 105
	mov	w2, 10
	str	w0, [x20, 152]
	mov	w1, 0
	mov	w0, 0
	blr	x3
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L979:
	bl	DecSyntax
	mov	w22, w0
	b	.L973
	.size	VCTRL_RunProcess, .-VCTRL_RunProcess
	.align	3
.LC426:
	.xword	.LANCHOR0
	.align	3
.LC427:
	.xword	.LANCHOR3
	.align	3
.LC428:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VFMW_CTRL_thread_proc
	.type	VFMW_CTRL_thread_proc, %function
VFMW_CTRL_thread_proc:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x0, x30
	mov	w21, 2
	bl	_mcount
	ldr	x20, .LC429
	ldr	x25, .LC431
	ldr	x22, .LC435
	ldr	x26, .LC433
	ldr	x23, .LC437
	ldr	x24, .LC438
.L981:
.L988:
	mov	x1, x25
	mov	w0, 2
	str	wzr, [x20, 152]
	bl	dprint_vfmw
	str	w21, [x20, 132]
	ldr	w0, [x20, 136]
	cmp	w0, 2
	bne	.L1003
	b	.L986
	.p2align 3
.L1005:
	str	w21, [x20, 152]
	bl	VCTRL_RunProcess
	mov	x1, x22
	mov	w0, 22
	bl	dprint_vfmw
	ldr	w0, [x20, 136]
	cmp	w0, 2
	beq	.L986
.L1003:
	cmp	w0, 3
	bne	.L1005
.L984:
	ldr	x1, .LC440
	mov	w2, 6
	mov	w0, 2
	str	w2, [x20, 152]
	bl	dprint_vfmw
	str	wzr, [x20, 132]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L986:
	mov	w2, 4
	mov	x1, x26
	mov	w0, 2
	str	w2, [x20, 152]
	mov	w27, 5
	bl	dprint_vfmw
	mov	w0, 1
	str	w0, [x20, 132]
	b	.L983
	.p2align 3
.L1006:
	ldr	x1, [x24, 232]
	blr	x1
.L983:
	mov	x1, x23
	mov	w0, 22
	str	w27, [x20, 152]
	bl	dprint_vfmw
	ldr	w1, [x20, 136]
	mov	w0, 10
	cmp	w1, 1
	beq	.L988
	cmp	w1, 3
	bne	.L1006
	b	.L984
	.size	VFMW_CTRL_thread_proc, .-VFMW_CTRL_thread_proc
	.align	3
.LC429:
	.xword	.LANCHOR0
	.align	3
.LC431:
	.xword	.LC430
	.align	3
.LC433:
	.xword	.LC432
	.align	3
.LC435:
	.xword	.LC434
	.align	3
.LC437:
	.xword	.LC436
	.align	3
.LC438:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC440:
	.xword	.LC439
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanVoQue
	.type	VCTRL_GetChanVoQue, %function
VCTRL_GetChanVoQue:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC442
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1025
	ldr	w1, [x0, 672]
	cmp	w1, 20
	bls	.L1026
.L1025:
	mov	x0, 0
.L1008:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1026:
	ldr	x2, .LC443
	ldrb	w1, [x2,w1,uxtw]
	adr	x2, .Lrtx1010
	add	x1, x2, w1, sxtb #2
	br	x1
.Lrtx1010:
	.section	.rodata
	.align	0
	.align	2
.L1010:
	.byte	(.L1023 - .Lrtx1010) / 4
	.byte	(.L1011 - .Lrtx1010) / 4
	.byte	(.L1012 - .Lrtx1010) / 4
	.byte	(.L1013 - .Lrtx1010) / 4
	.byte	(.L1025 - .Lrtx1010) / 4
	.byte	(.L1014 - .Lrtx1010) / 4
	.byte	(.L1015 - .Lrtx1010) / 4
	.byte	(.L1025 - .Lrtx1010) / 4
	.byte	(.L1016 - .Lrtx1010) / 4
	.byte	(.L1017 - .Lrtx1010) / 4
	.byte	(.L1018 - .Lrtx1010) / 4
	.byte	(.L1018 - .Lrtx1010) / 4
	.byte	(.L1018 - .Lrtx1010) / 4
	.byte	(.L1019 - .Lrtx1010) / 4
	.byte	(.L1025 - .Lrtx1010) / 4
	.byte	(.L1020 - .Lrtx1010) / 4
	.byte	(.L1021 - .Lrtx1010) / 4
	.byte	(.L1022 - .Lrtx1010) / 4
	.byte	(.L1025 - .Lrtx1010) / 4
	.byte	(.L1025 - .Lrtx1010) / 4
	.byte	(.L1023 - .Lrtx1010) / 4
	.text
	.p2align 3
.L1023:
	add	x0, x0, 294912
	ldr	x19, [sp, 16]
	add	x0, x0, 2592
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1021:
	add	x0, x0, 356352
	add	x0, x0, 3616
	b	.L1008
	.p2align 3
.L1020:
	add	x0, x0, 294912
	add	x0, x0, 3496
	b	.L1008
	.p2align 3
.L1019:
	add	x0, x0, 303104
	add	x0, x0, 2600
	b	.L1008
	.p2align 3
.L1018:
	add	x0, x0, 307200
	add	x0, x0, 3048
	b	.L1008
	.p2align 3
.L1022:
	add	x0, x0, 565248
	add	x0, x0, 3464
	b	.L1008
	.p2align 3
.L1011:
	add	x0, x0, 348160
	add	x0, x0, 2760
	b	.L1008
	.p2align 3
.L1013:
	add	x0, x0, 385024
	add	x0, x0, 256
	b	.L1008
	.p2align 3
.L1012:
	add	x0, x0, 319488
	add	x0, x0, 704
	b	.L1008
	.p2align 3
.L1017:
	add	x0, x0, 348160
	add	x0, x0, 3248
	b	.L1008
	.p2align 3
.L1016:
	add	x0, x0, 348160
	add	x0, x0, 3280
	b	.L1008
	.p2align 3
.L1015:
	add	x0, x0, 311296
	add	x0, x0, 2400
	b	.L1008
	.p2align 3
.L1014:
	add	x0, x0, 335872
	add	x0, x0, 3224
	b	.L1008
	.size	VCTRL_GetChanVoQue, .-VCTRL_GetChanVoQue
	.align	3
.LC442:
	.xword	.LANCHOR0
	.align	3
.LC443:
	.xword	.L1010
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_FindOccupiedFrame
	.type	VCTRL_FindOccupiedFrame, %function
VCTRL_FindOccupiedFrame:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	w23, w0
	mov	x0, x30
	bl	_mcount
	cmp	w23, 15
	bhi	.L1039
	mov	w0, w23
	bl	VCTRL_GetChanVoQue
	mov	x20, x0
	cbz	x0, .L1035
	add	x2, x29, 96
	add	x1, x29, 88
	str	wzr, [x29, 88]
	str	wzr, [x2, -4]!
	bl	GetQueueImgNum
	ldr	w2, [x29, 88]
	mov	w0, -1
	cbz	w2, .L1029
	cmp	w2, 32
	bhi	.L1031
.L1033:
	ldr	x21, .LC449
	mov	w19, 0
	ldr	x24, .LC448
	sxtw	x25, w23
	ldr	x22, .LC451
	add	x21, x21, 104
	.p2align 2
.L1032:
	ldrh	w5, [x20, 4]
	mov	w4, w19
	ldr	x8, [x24, x25, lsl 3]
	mov	x1, x22
	add	w5, w5, w19
	mov	w7, 0
	and	x5, x5, 63
	add	x8, x8, x19, sxtw 4
	add	x5, x5, 2
	add	x8, x8, 294912
	mov	x6, 0
	mov	w3, 4225
	mov	x2, x21
	mov	w0, 1
	ldr	x5, [x20, x5, lsl 3]
	add	w19, w19, 1
	sxth	w19, w19
	ldr	w5, [x5, 28]
	str	w5, [x8, 2020]
	str	wzr, [x8, 2016]
	str	xzr, [x8, 2024]
	bl	dprint_vfmw
	ldr	w1, [x29, 88]
	cmp	w19, w1
	bcc	.L1032
.L1034:
	ldr	x2, [x24, w23, sxtw 3]
	mov	w0, 0
	add	x2, x2, 294912
	str	w1, [x2, 2528]
.L1029:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1031:
	ldr	x1, .LC447
	mov	w3, 32
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x29, 88]
	cbnz	w1, .L1033
	ldr	x24, .LC448
	b	.L1034
.L1035:
	mov	w0, -1
	b	.L1029
.L1039:
	ldr	x3, .LC444
	mov	w2, 4190
	ldr	x1, .LC445
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1029
	.size	VCTRL_FindOccupiedFrame, .-VCTRL_FindOccupiedFrame
	.align	3
.LC444:
	.xword	.LC51
	.align	3
.LC445:
	.xword	.LC2
	.align	3
.LC447:
	.xword	.LC446
	.align	3
.LC448:
	.xword	.LANCHOR0
	.align	3
.LC449:
	.xword	.LANCHOR2
	.align	3
.LC451:
	.xword	.LC450
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ReportOccupiedFrame
	.type	VCTRL_ReportOccupiedFrame, %function
VCTRL_ReportOccupiedFrame:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	sxtw	x22, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	cmp	w22, 15
	bhi	.L1057
	cbz	x19, .L1058
	mov	w0, w22
	bl	VCTRL_GetChanVoQue
	mov	x20, x0
	cbz	x0, .L1050
	ldr	x21, .LC457
	add	x23, x29, 80
	mov	w0, 4
	str	wzr, [x29, 72]
	ldr	x1, [x21, 24]
	str	wzr, [x23, -4]!
	blr	x1
	mov	x2, x23
	add	x1, x29, 72
	mov	x0, x20
	bl	GetQueueImgNum
	ldr	w2, [x29, 72]
	cbnz	w2, .L1059
	ldr	x6, .LC459
	ldr	x4, [x6, w22, sxtw 3]
	add	x0, x4, 294912
	ldr	w0, [x0, 2528]
	cbz	w0, .L1048
	mov	w1, 0
	.p2align 2
.L1049:
	sxtw	x2, w1
	add	w1, w1, 1
	add	x3, x2, 16384
	lsl	x2, x2, 4
	add	x3, x3, 2174
	add	x5, x19, x2
	sxth	w1, w1
	add	x3, x4, x3, lsl 4
	ldr	w0, [x3, 4]
	str	w0, [x5, 4]
	str	xzr, [x5, 8]
	str	wzr, [x19, x2]
	ldr	x4, [x6, x22, lsl 3]
	add	x0, x4, 294912
	ldr	w0, [x0, 2528]
	cmp	w1, w0
	bcc	.L1049
	str	w0, [x19, 512]
	b	.L1048
	.p2align 3
.L1059:
	cmp	w2, 32
	bhi	.L1060
.L1045:
	mov	w0, 0
	.p2align 2
.L1047:
	ldrh	w1, [x20, 4]
	sbfiz	x3, x0, 4, 32
	add	w5, w0, 1
	add	x4, x19, x3
	add	w0, w1, w0
	and	x1, x0, 63
	sxth	w0, w5
	add	x1, x1, 2
	cmp	w2, w0
	ldr	x1, [x20, x1, lsl 3]
	ldr	w1, [x1, 28]
	str	w1, [x4, 4]
	str	xzr, [x4, 8]
	str	wzr, [x19, x3]
	bhi	.L1047
	uxth	w1, w2
.L1046:
	ldrh	w0, [x20, 4]
	add	w0, w1, w0
	and	w0, w0, 63
	strh	w0, [x20, 4]
	str	w2, [x19, 512]
.L1048:
	ldr	x1, [x21, 32]
	mov	w0, 4
	blr	x1
	mov	w0, 0
.L1042:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1060:
	ldr	x1, .LC458
	mov	w3, 32
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w2, [x29, 72]
	cbnz	w2, .L1045
	mov	w2, 0
	mov	w1, 0
	b	.L1046
.L1050:
	mov	w0, -1
	b	.L1042
.L1057:
	ldr	x3, .LC453
	mov	w2, 4305
	ldr	x1, .LC454
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1042
.L1058:
	ldr	x3, .LC456
	mov	w2, 4306
	ldr	x1, .LC454
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1042
	.size	VCTRL_ReportOccupiedFrame, .-VCTRL_ReportOccupiedFrame
	.align	3
.LC453:
	.xword	.LC51
	.align	3
.LC454:
	.xword	.LC2
	.align	3
.LC456:
	.xword	.LC455
	.align	3
.LC457:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC458:
	.xword	.LC446
	.align	3
.LC459:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanImgNum
	.type	VCTRL_GetChanImgNum, %function
VCTRL_GetChanImgNum:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	mov	x22, x1
	mov	x21, x2
	mov	x23, x3
	bl	_mcount
	ldr	x20, .LC460
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1069
	mov	w0, w19
	bl	VCTRL_GetChanVoQue
	cbz	x0, .L1064
	mov	x1, x21
	mov	x2, x23
	bl	GetQueueImgNum
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w1, [x0, 672]
	cbz	w1, .L1070
	cmp	w1, 16
	beq	.L1071
	cmp	w1, 17
	beq	.L1072
	cmp	w1, 15
	beq	.L1073
	mov	w0, 2
	str	w0, [x22]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1070:
	add	x0, x0, 323584
	ldr	w0, [x0, 2200]
	str	w0, [x22]
.L1061:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1064:
	str	wzr, [x23]
	str	wzr, [x21]
	str	wzr, [x22]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1073:
	add	x0, x0, 11419648
	ldr	w0, [x0, 3344]
	str	w0, [x22]
	b	.L1061
	.p2align 3
.L1071:
	add	x0, x0, 299008
	ldr	w0, [x0, 1168]
	str	w0, [x22]
	b	.L1061
	.p2align 3
.L1072:
	mov	w0, 9
	str	w0, [x22]
	b	.L1061
	.p2align 3
.L1069:
	ldr	x3, .LC461
	mov	w2, 8042
	ldr	x1, .LC462
	bl	dprint_vfmw
	b	.L1061
	.size	VCTRL_GetChanImgNum, .-VCTRL_GetChanImgNum
	.align	3
.LC460:
	.xword	.LANCHOR0
	.align	3
.LC461:
	.xword	.LC5
	.align	3
.LC462:
	.xword	.LC2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanState
	.type	VCTRL_GetChanState, %function
VCTRL_GetChanState:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	cmp	w20, 15
	bhi	.L1087
	stp	wzr, wzr, [x29, 60]
	str	wzr, [x29, 68]
	cbz	x19, .L1074
	ldr	x3, .LC468
	mov	x2, 100
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	ldr	x21, .LC469
	ldr	x0, [x21, w20, sxtw 3]
	cbz	x0, .L1074
	ldr	w0, [x0, 1248]
	str	w0, [x19, 32]
	ldr	x0, [x21, w20, sxtw 3]
	ldr	w0, [x0, 1252]
	str	w0, [x19, 36]
	ldr	x0, [x21, w20, sxtw 3]
	ldr	w0, [x0, 1256]
	str	w0, [x19, 44]
	ldr	x5, [x21, w20, sxtw 3]
	add	x0, x5, 262144
	ldrsb	w0, [x0, 2058]
	cmp	w0, wzr
	ble	.L1082
	mov	x2, 24066
	sub	w4, w0, #1
	mov	w3, 100
	movk	x2, 0x4, lsl 16
	add	x0, x5, 282624
	mov	w1, 0
	umaddl	x2, w4, w3, x2
	add	x0, x0, 3486
	add	x3, x5, x2
	.p2align 2
.L1081:
	ldrsb	w2, [x0], 100
	cmp	w2, 3
	csinc	w1, w1, w1, ne
	cmp	x0, x3
	bne	.L1081
.L1079:
	str	w1, [x19, 84]
	add	x3, x29, 68
	add	x2, x29, 64
	mov	w0, w20
	ldr	x4, [x21, w20, sxtw 3]
	add	x1, x29, 60
	sxtw	x22, w20
	add	x4, x4, 262144
	ldrsb	w4, [x4, 2058]
	str	w4, [x19, 92]
	bl	VCTRL_GetChanImgNum
	ldr	x2, .LC470
	lsl	x0, x22, 4
	add	x22, x0, x22, lsl 2
	add	x1, x29, 72
	add	x2, x2, 3648
	mov	w0, w20
	ldp	w4, w3, [x29, 64]
	add	x2, x2, x22, lsl 4
	sub	x2, x2, x22
	add	w3, w4, w3
	str	w3, [x19, 48]
	ldr	w3, [x2, 20]
	str	w3, [x19, 96]
	ldr	w2, [x2, 8]
	str	w2, [x19, 20]
	bl	SM_GetInfo
	ldp	w1, w2, [x29, 88]
	ldr	w0, [x29, 80]
	cmp	w2, 10
	stp	w1, w0, [x19, 52]
	cset	w0, gt
	str	w0, [x19, 80]
	mov	x0, 47640
	ldr	x1, [x21, w20, sxtw 3]
	movk	x0, 0x4, lsl 16
	add	x0, x1, x0
	ldrb	w0, [x0, 3616]
	and	w0, w0, 1
	str	w0, [x19, 88]
.L1074:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1087:
	ldr	x3, .LC466
	mov	w2, 1501
	ldr	x1, .LC467
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
.L1082:
	mov	w1, 0
	b	.L1079
	.size	VCTRL_GetChanState, .-VCTRL_GetChanState
	.align	3
.LC466:
	.xword	.LC465
	.align	3
.LC467:
	.xword	.LC2
	.align	3
.LC468:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC469:
	.xword	.LANCHOR0
	.align	3
.LC470:
	.xword	.LANCHOR3
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanWidth
	.type	VCTRL_GetChanWidth, %function
VCTRL_GetChanWidth:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L1092
	ldr	x0, .LC474
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1093
	ldr	w0, [x0, 1348]
.L1090:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1092:
	ldr	x3, .LC472
	mov	w2, 8099
	ldr	x1, .LC473
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1090
.L1093:
	ldr	x3, .LC475
	mov	w2, 8100
	ldr	x1, .LC473
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1090
	.size	VCTRL_GetChanWidth, .-VCTRL_GetChanWidth
	.align	3
.LC472:
	.xword	.LC51
	.align	3
.LC473:
	.xword	.LC2
	.align	3
.LC474:
	.xword	.LANCHOR0
	.align	3
.LC475:
	.xword	.LC5
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanHeight
	.type	VCTRL_GetChanHeight, %function
VCTRL_GetChanHeight:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L1098
	ldr	x0, .LC478
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1099
	ldr	w0, [x0, 1352]
.L1096:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1098:
	ldr	x3, .LC476
	mov	w2, 8107
	ldr	x1, .LC477
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1096
.L1099:
	ldr	x3, .LC479
	mov	w2, 8108
	ldr	x1, .LC477
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1096
	.size	VCTRL_GetChanHeight, .-VCTRL_GetChanHeight
	.align	3
.LC476:
	.xword	.LC51
	.align	3
.LC477:
	.xword	.LC2
	.align	3
.LC478:
	.xword	.LANCHOR0
	.align	3
.LC479:
	.xword	.LC5
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanCapLevel
	.type	VCTRL_GetChanCapLevel, %function
VCTRL_GetChanCapLevel:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L1104
	ldr	x0, .LC482
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1103
	ldr	w0, [x0, 44]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1104:
	ldr	x3, .LC480
	mov	w2, 8115
	ldr	x1, .LC481
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1103:
	mov	w0, 33
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_GetChanCapLevel, .-VCTRL_GetChanCapLevel
	.align	3
.LC480:
	.xword	.LC51
	.align	3
.LC481:
	.xword	.LC2
	.align	3
.LC482:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetVdecExtra
	.type	VCTRL_SetVdecExtra, %function
VCTRL_SetVdecExtra:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	ldr	x20, .LC483
	add	w0, w21, 4096
	mov	w1, 10000
	add	w0, w0, 904
	mov	w2, w21
	cmp	w0, w1
	str	w21, [x20, -208]
	bhi	.L1110
.L1106:
	mov	w3, 5000
	mov	w1, -5000
	cmp	w2, w3
	add	w0, w19, 4096
	csel	w2, w2, w3, le
	add	w0, w0, 904
	cmp	w2, w1
	mov	w3, 10000
	csel	w2, w2, w1, ge
	cmp	w0, w3
	stp	w2, w19, [x20, -208]
	bhi	.L1111
.L1107:
	mov	w2, 5000
	mov	w0, -5000
	cmp	w19, w2
	csel	w19, w19, w2, le
	cmp	w19, w0
	csel	w19, w19, w0, ge
	str	w19, [x20, -204]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1110:
	ldr	x1, .LC485
	mov	w0, 32
	bl	dprint_vfmw
	ldr	w2, [x20, -208]
	b	.L1106
	.p2align 3
.L1111:
	ldr	x1, .LC487
	mov	w2, w19
	mov	w0, 32
	bl	dprint_vfmw
	ldr	w19, [x20, -204]
	b	.L1107
	.size	VCTRL_SetVdecExtra, .-VCTRL_SetVdecExtra
	.align	3
.LC483:
	.xword	.LANCHOR3
	.align	3
.LC485:
	.xword	.LC484
	.align	3
.LC487:
	.xword	.LC486
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ExtAccGetDecParam
	.type	VCTRL_ExtAccGetDecParam, %function
VCTRL_ExtAccGetDecParam:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x22, x0
	mov	x0, x30
	bl	_mcount
	ldr	x21, .LC488
	mov	w0, 0
	ldr	x1, [x21, 24]
	blr	x1
	ldr	x1, [x21, 24]
	mov	w0, 5
	blr	x1
	ldr	w19, [x22]
	cmn	w19, #1
	beq	.L1157
	cmp	w19, 15
	bhi	.L1125
	ldr	x20, .LC492
	sxtw	x23, w19
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1125
	ldr	w1, [x0, 12]
	cbz	w1, .L1125
	ldr	w1, [x0, 16]
	cbnz	w1, .L1158
	.p2align 2
.L1125:
	ldr	x2, .LC493
	mov	w3, w19
	ldr	x1, .LC503
	mov	w0, 1
	add	x2, x2, 128
	mov	w19, -1
	bl	dprint_vfmw
	str	xzr, [x22, 8]
	mov	w0, 5
.L1155:
	ldr	x1, [x21, 32]
	blr	x1
	ldr	x1, [x21, 32]
	mov	w0, 0
	blr	x1
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L1157:
	ldr	x23, .LC489
	ldr	w28, [x23, 64]
	tbnz	w28, #31, .L1114
	ldr	x25, .LC493
	add	x20, x23, 68
	ldr	x27, .LC495
	add	x23, x23, 128
	ldr	x24, .LC492
	add	x25, x25, 128
	.p2align 2
.L1115:
	cmp	w28, 15
	bhi	.L1116
	ldr	x0, [x24, w28, sxtw 3]
	sxtw	x26, w28
	cbz	x0, .L1116
	ldr	w1, [x0, 12]
	cbz	w1, .L1116
	ldr	w1, [x0, 16]
	cbz	w1, .L1116
	ldr	w1, [x0, 28]
	cbz	w1, .L1116
	ldr	w0, [x0, 672]
	cmp	w0, 21
	beq	.L1116
	mov	w0, w28
	bl	VDMDRV_IsChanDec
	cmp	w0, 1
	beq	.L1159
	mov	w0, w28
	bl	VCTRL_GetDecParam
	cbz	x0, .L1160
	ldr	x1, [x24, x26, lsl 3]
	cbz	x1, .L1161
	str	x0, [x29, 104]
	ldr	w0, [x1, 672]
	str	w0, [x29, 100]
	ldr	x0, .LC501
	str	w28, [x29, 96]
	ldr	x1, [x0, -256]
	add	x0, x29, 96
	blr	x1
	cmp	w0, 1
	beq	.L1162
	str	xzr, [x22, 8]
	cmp	x23, x20
	bne	.L1124
.L1114:
	ldr	x1, [x21, 32]
	mov	w0, 5
	blr	x1
	ldr	x1, [x21, 32]
	mov	w0, 0
	blr	x1
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L1116:
	mov	w3, w28
	mov	x2, x25
	mov	x1, x27
	mov	w0, 1
	bl	dprint_vfmw
.L1118:
	cmp	x23, x20
	beq	.L1114
.L1124:
	ldr	w28, [x20], 4
	tbz	w28, #31, .L1115
	b	.L1114
	.p2align 3
.L1161:
	ldr	x1, .LC500
	mov	w3, w28
	mov	w2, 8237
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1118
	.p2align 3
.L1159:
	ldr	x1, .LC497
	mov	w2, w28
	mov	w0, 22
	bl	dprint_vfmw
	b	.L1118
	.p2align 3
.L1158:
	ldr	w1, [x0, 28]
	cbz	w1, .L1125
	ldr	w0, [x0, 672]
	cmp	w0, 21
	beq	.L1125
	mov	w0, w19
	bl	VDMDRV_IsChanDec
	cmp	w0, 1
	beq	.L1163
	mov	w0, w19
	bl	VCTRL_GetDecParam
	cbz	x0, .L1164
	ldr	x1, [x20, x23, lsl 3]
	cbz	x1, .L1165
	ldr	x2, .LC501
	ldr	w1, [x1, 672]
	stp	w19, w1, [x29, 96]
	ldr	x1, [x2, -256]
	str	x0, [x29, 104]
	add	x0, x29, 96
	blr	x1
	cmp	w0, 1
	bne	.L1130
	ldp	w4, w3, [x29, 96]
	ldr	x2, [x29, 104]
	mov	w0, w19
	mov	w1, 4
	str	x2, [x22, 8]
	stp	w4, w3, [x22]
	mov	w19, 0
	bl	VDMDRV_SetChanIsDec
	b	.L1114
	.p2align 3
.L1160:
	ldr	x1, .LC499
	mov	w2, w28
	mov	w0, 22
	bl	dprint_vfmw
	b	.L1118
	.p2align 3
.L1130:
	mov	w0, 5
	mov	w19, -1
	str	xzr, [x22, 8]
	b	.L1155
	.p2align 3
.L1162:
	ldp	w5, w4, [x29, 96]
	ldr	x2, [x29, 104]
	mov	w1, 4
	mov	w0, w28
	str	x2, [x22, 8]
	stp	w5, w4, [x22]
	mov	w19, 0
	bl	VDMDRV_SetChanIsDec
	b	.L1114
.L1165:
	ldr	x1, .LC500
	mov	w3, w19
	mov	w2, 8322
	mov	w0, 1
	mov	w19, -1
	bl	dprint_vfmw
	str	xzr, [x22, 8]
	mov	w0, 5
	b	.L1155
.L1163:
	ldr	x1, .LC497
	mov	w2, w19
.L1156:
	mov	w0, 22
	mov	w19, -1
	bl	dprint_vfmw
	str	xzr, [x22, 8]
	mov	w0, 5
	b	.L1155
.L1164:
	ldr	x1, [x21, 248]
	blr	x1
	mov	w2, w19
	ldr	x1, .LC499
	b	.L1156
	.size	VCTRL_ExtAccGetDecParam, .-VCTRL_ExtAccGetDecParam
	.align	3
.LC488:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC489:
	.xword	g_ChanCtx
	.align	3
.LC492:
	.xword	.LANCHOR0
	.align	3
.LC493:
	.xword	.LANCHOR2
	.align	3
.LC495:
	.xword	.LC494
	.align	3
.LC497:
	.xword	.LC496
	.align	3
.LC499:
	.xword	.LC498
	.align	3
.LC500:
	.xword	.LC210
	.align	3
.LC501:
	.xword	.LANCHOR5
	.align	3
.LC503:
	.xword	.LC502
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetLastFrameIdPlus2
	.type	VCTRL_GetLastFrameIdPlus2, %function
VCTRL_GetLastFrameIdPlus2:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L1170
	ldr	x0, .LC507
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1169
	ldr	w0, [x0, 444]
.L1168:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1170:
	ldr	x3, .LC505
	mov	w2, 9281
	ldr	x1, .LC506
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1168
.L1169:
	mov	w0, -1
	b	.L1168
	.size	VCTRL_GetLastFrameIdPlus2, .-VCTRL_GetLastFrameIdPlus2
	.align	3
.LC505:
	.xword	.LC51
	.align	3
.LC506:
	.xword	.LC2
	.align	3
.LC507:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetLastFrameIdPlus2
	.type	VCTRL_SetLastFrameIdPlus2, %function
VCTRL_SetLastFrameIdPlus2:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cmp	w19, 15
	bhi	.L1177
	ldr	x0, .LC510
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1171
	str	w20, [x0, 444]
.L1171:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1177:
	ldr	x3, .LC508
	mov	w2, 9293
	ldr	x1, .LC509
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_SetLastFrameIdPlus2, .-VCTRL_SetLastFrameIdPlus2
	.align	3
.LC508:
	.xword	.LC51
	.align	3
.LC509:
	.xword	.LC2
	.align	3
.LC510:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetLastFrameState
	.type	VCTRL_SetLastFrameState, %function
VCTRL_SetLastFrameState:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cmp	w19, 15
	bhi	.L1184
	ldr	x0, .LC513
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1178
	str	w20, [x0, 448]
.L1178:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1184:
	ldr	x3, .LC511
	mov	w2, 9305
	ldr	x1, .LC512
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_SetLastFrameState, .-VCTRL_SetLastFrameState
	.align	3
.LC511:
	.xword	.LC51
	.align	3
.LC512:
	.xword	.LC2
	.align	3
.LC513:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ReportLastFrame
	.type	VCTRL_ReportLastFrame, %function
VCTRL_ReportLastFrame:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	w20, w1
	mov	x0, x30
	bl	_mcount
	cbz	w20, .L1192
	cmp	w20, 1
	mov	w0, 3
	mov	w1, 4
	csel	w0, w0, w1, eq
	cmp	w19, 15
	bhi	.L1199
.L1189:
	ldr	x21, .LC516
	ldr	x1, [x21, w19, sxtw 3]
	cbz	x1, .L1190
	str	w0, [x1, 448]
.L1190:
	ldr	x0, .LC517
	ldr	x4, [x0, 3376]
	cbz	x4, .L1191
	add	x2, x29, 64
	mov	w3, 4
	mov	w1, 115
	mov	w0, w19
	str	w20, [x2, -8]!
	blr	x4
.L1191:
	ldr	x4, [x21, w19, sxtw 3]
	mov	w3, w20
	ldr	x1, .LC519
	mov	w2, w19
	mov	w0, 1
	ldr	w4, [x4, 448]
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1192:
	cmp	w19, 15
	mov	w0, 2
	bls	.L1189
.L1199:
	ldr	x3, .LC514
	mov	w2, 9305
	ldr	x1, .LC515
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, .LC516
	b	.L1190
	.size	VCTRL_ReportLastFrame, .-VCTRL_ReportLastFrame
	.align	3
.LC514:
	.xword	.LC51
	.align	3
.LC515:
	.xword	.LC2
	.align	3
.LC516:
	.xword	.LANCHOR0
	.align	3
.LC517:
	.xword	.LANCHOR3
	.align	3
.LC519:
	.xword	.LC518
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_OutputLastFrame
	.type	VCTRL_OutputLastFrame, %function
VCTRL_OutputLastFrame:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC520
	ldr	x0, [x0, w20, sxtw 3]
	add	x0, x0, 294912
	ldr	w1, [x0, 2576]
	cmp	w1, 17
	bls	.L1225
.L1220:
	mov	w19, -1
.L1217:
	mov	w0, w20
	mov	w1, 1
	bl	VCTRL_ReportLastFrame
	ldr	x1, .LC523
	mov	w0, 2
	bl	dprint_vfmw
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1225:
	ldr	x2, .LC521
	ldrb	w1, [x2,w1,uxtw]
	adr	x2, .Lrtx1203
	add	x1, x2, w1, sxtb #2
	br	x1
.Lrtx1203:
	.section	.rodata
	.align	0
	.align	2
.L1203:
	.byte	(.L1202 - .Lrtx1203) / 4
	.byte	(.L1204 - .Lrtx1203) / 4
	.byte	(.L1205 - .Lrtx1203) / 4
	.byte	(.L1206 - .Lrtx1203) / 4
	.byte	(.L1220 - .Lrtx1203) / 4
	.byte	(.L1207 - .Lrtx1203) / 4
	.byte	(.L1208 - .Lrtx1203) / 4
	.byte	(.L1220 - .Lrtx1203) / 4
	.byte	(.L1209 - .Lrtx1203) / 4
	.byte	(.L1210 - .Lrtx1203) / 4
	.byte	(.L1211 - .Lrtx1203) / 4
	.byte	(.L1211 - .Lrtx1203) / 4
	.byte	(.L1211 - .Lrtx1203) / 4
	.byte	(.L1212 - .Lrtx1203) / 4
	.byte	(.L1220 - .Lrtx1203) / 4
	.byte	(.L1213 - .Lrtx1203) / 4
	.byte	(.L1214 - .Lrtx1203) / 4
	.byte	(.L1215 - .Lrtx1203) / 4
	.text
	.p2align 3
.L1202:
	add	x0, x0, 2584
	bl	H264DEC_GetRemainImg
	mov	w19, w0
	.p2align 2
.L1201:
	cmn	w19, #1
	beq	.L1217
	cbnz	w19, .L1226
	mov	w0, w20
	mov	w1, 0
	bl	VCTRL_ReportLastFrame
	ldr	x1, .LC525
	mov	w0, 2
	bl	dprint_vfmw
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1204:
	add	x0, x0, 2584
	bl	VC1DEC_GetRemainImg
	mov	w19, w0
	b	.L1201
	.p2align 3
.L1205:
	add	x0, x0, 2584
	bl	MPEG4DEC_GetRemainImg
	mov	w19, w0
	b	.L1201
	.p2align 3
.L1206:
	add	x0, x0, 2584
	bl	MPEG2DEC_GetRemainImg
	mov	w19, w0
	b	.L1201
	.p2align 3
.L1207:
	add	x0, x0, 2584
	bl	DIVX3DEC_GetRemainImg
	mov	w19, w0
	b	.L1201
	.p2align 3
.L1208:
	add	x0, x0, 2584
	bl	AVSDEC_GetRemainImg
	mov	w19, w0
	b	.L1201
	.p2align 3
.L1209:
	add	x0, x0, 2584
	bl	REAL8DEC_GetRemainImg
	mov	w19, w0
	b	.L1201
	.p2align 3
.L1210:
	add	x0, x0, 2584
	bl	REAL9DEC_GetRemainImg
	mov	w19, w0
	b	.L1201
	.p2align 3
.L1211:
	add	x0, x0, 2584
	bl	VP6DEC_GetRemainImg
	mov	w19, w0
	b	.L1201
	.p2align 3
.L1212:
	add	x0, x0, 2584
	bl	VP8DEC_GetRemainImg
	mov	w19, w0
	b	.L1201
	.p2align 3
.L1213:
	add	x0, x0, 2584
	bl	MVC_DEC_GetRemainImg
	mov	w19, w0
	b	.L1201
	.p2align 3
.L1214:
	add	x0, x0, 2584
	bl	HEVCDEC_GetRemainImg
	mov	w19, w0
	b	.L1201
	.p2align 3
.L1215:
	add	x0, x0, 2584
	bl	VP9DEC_GetRemainImg
	mov	w19, w0
	b	.L1201
	.p2align 3
.L1226:
	mov	w1, w19
	mov	w0, w20
	bl	VCTRL_ReportLastFrame
	ldr	x1, .LC527
	sub	w2, w19, #2
	mov	w0, 2
	bl	dprint_vfmw
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_OutputLastFrame, .-VCTRL_OutputLastFrame
	.align	3
.LC520:
	.xword	.LANCHOR0
	.align	3
.LC521:
	.xword	.L1203
	.align	3
.LC523:
	.xword	.LC522
	.align	3
.LC525:
	.xword	.LC524
	.align	3
.LC527:
	.xword	.LC526
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_PostProc
	.type	VCTRL_PostProc, %function
VCTRL_PostProc:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	mov	w22, w2
	mov	x23, x3
	mov	w24, w4
	bl	_mcount
	ldr	x21, .LC528
	ldr	x0, [x21, w19, sxtw 3]
	cbz	x0, .L1261
	ldr	w1, [x0, 672]
	ldr	w25, [x0, 1296]
	cmp	w1, 17
	bls	.L1262
.L1227:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1262:
	ldr	x2, .LC531
	ldrh	w1, [x2,w1,uxtw #1]
	adr	x2, .Lrtx1231
	add	x1, x2, w1, sxth #2
	br	x1
.Lrtx1231:
	.section	.rodata
	.align	0
	.align	2
.L1231:
	.2byte	(.L1230 - .Lrtx1231) / 4
	.2byte	(.L1232 - .Lrtx1231) / 4
	.2byte	(.L1233 - .Lrtx1231) / 4
	.2byte	(.L1234 - .Lrtx1231) / 4
	.2byte	(.L1227 - .Lrtx1231) / 4
	.2byte	(.L1235 - .Lrtx1231) / 4
	.2byte	(.L1236 - .Lrtx1231) / 4
	.2byte	(.L1227 - .Lrtx1231) / 4
	.2byte	(.L1237 - .Lrtx1231) / 4
	.2byte	(.L1238 - .Lrtx1231) / 4
	.2byte	(.L1239 - .Lrtx1231) / 4
	.2byte	(.L1239 - .Lrtx1231) / 4
	.2byte	(.L1239 - .Lrtx1231) / 4
	.2byte	(.L1240 - .Lrtx1231) / 4
	.2byte	(.L1227 - .Lrtx1231) / 4
	.2byte	(.L1241 - .Lrtx1231) / 4
	.2byte	(.L1242 - .Lrtx1231) / 4
	.2byte	(.L1243 - .Lrtx1231) / 4
	.text
	.p2align 3
.L1243:
	add	x0, x0, 294912
	mov	w3, w24
	mov	w2, w25
	mov	w1, w20
	add	x0, x0, 2584
	bl	VP9DEC_VDMPostProc
	cmp	w25, 1
	beq	.L1227
.L1259:
	ldr	x0, [x21, w19, sxtw 3]
.L1260:
	add	x0, x0, 294912
	str	xzr, [x0, 2592]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1242:
	add	x0, x0, 294912
	mov	w5, w24
	mov	w4, w25
	mov	x3, x23
	mov	w2, 1
	mov	w1, w20
	add	x0, x0, 2584
	bl	HEVCDEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 294912
	ldrb	w0, [x0, 2591]
	cmp	w0, 1
	beq	.L1263
.L1246:
	cmp	w25, 1
	beq	.L1227
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 299008
	str	xzr, [x0, 840]
	b	.L1227
	.p2align 3
.L1241:
	add	x0, x0, 294912
	mov	w1, w20
	mov	w2, 1
	add	x0, x0, 2584
	bl	MVC_DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 294912
	ldrb	w1, [x0, 2585]
	cmp	w1, 1
	beq	.L1264
	str	xzr, [x0, 2816]
	b	.L1227
	.p2align 3
.L1240:
	add	x0, x0, 294912
	mov	w1, w20
	add	x0, x0, 2584
	bl	VP8DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 294912
	str	xzr, [x0, 2592]
	b	.L1227
	.p2align 3
.L1239:
	add	x0, x0, 294912
	mov	w1, w20
	add	x0, x0, 2584
	bl	VP6DEC_VDMPostProc
	b	.L1259
	.p2align 3
.L1238:
	add	x0, x0, 294912
	mov	w2, w22
	mov	w1, w20
	add	x0, x0, 2584
	bl	REAL9DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 294912
	str	xzr, [x0, 2592]
	b	.L1227
	.p2align 3
.L1237:
	add	x0, x0, 294912
	mov	w2, w22
	mov	w1, w20
	add	x0, x0, 2584
	bl	REAL8DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 294912
	str	xzr, [x0, 2592]
	b	.L1227
	.p2align 3
.L1236:
	add	x0, x0, 294912
	mov	w1, w20
	add	x0, x0, 2584
	bl	AVSDEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 294912
	ldrb	w1, [x0, 2584]
	cmp	w1, 1
	beq	.L1265
	str	xzr, [x0, 2608]
	b	.L1227
	.p2align 3
.L1235:
	add	x0, x0, 294912
	mov	w2, w22
	mov	w1, w20
	add	x0, x0, 2584
	bl	DIVX3DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 294912
	str	xzr, [x0, 2592]
	b	.L1227
	.p2align 3
.L1234:
	add	x0, x0, 294912
	mov	w1, w20
	add	x0, x0, 2584
	bl	MPEG2DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x0, 385024
	ldrb	w1, [x1, 882]
	cmp	w1, 1
	bne	.L1260
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x0, 385024
	strb	wzr, [x1, 882]
	b	.L1260
	.p2align 3
.L1233:
	add	x0, x0, 294912
	mov	w1, w20
	mov	x2, x23
	add	x0, x0, 2584
	bl	MPEG4DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x0, 319488
	ldrb	w1, [x1, 1348]
	cmp	w1, 1
	beq	.L1266
.L1249:
	add	x0, x0, 294912
	str	xzr, [x0, 3624]
	b	.L1227
	.p2align 3
.L1232:
	add	x0, x0, 294912
	mov	w1, w20
	add	x0, x0, 2584
	bl	VC1DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x0, 311296
	ldrb	w1, [x1, 999]
	cmp	w1, 1
	bne	.L1260
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x0, 311296
	strb	wzr, [x1, 999]
	b	.L1260
	.p2align 3
.L1230:
	add	x0, x0, 294912
	mov	w5, w24
	mov	w4, w25
	mov	x3, x23
	mov	w2, 1
	mov	w1, w20
	add	x0, x0, 2584
	bl	H264DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 311296
	ldrb	w0, [x0, 1128]
	cmp	w0, 1
	beq	.L1267
.L1244:
	cmp	w25, 1
	beq	.L1227
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 294912
	str	xzr, [x0, 3200]
	b	.L1227
	.p2align 3
.L1265:
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 294912
	strb	wzr, [x0, 2584]
	str	xzr, [x0, 2608]
	b	.L1227
.L1264:
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 294912
	strb	wzr, [x0, 2585]
	str	xzr, [x0, 2816]
	b	.L1227
	.p2align 3
.L1261:
	ldr	x3, .LC529
	mov	w2, 5184
	ldr	x1, .LC530
	bl	dprint_vfmw
	b	.L1227
	.p2align 3
.L1266:
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x0, 319488
	strb	wzr, [x1, 1348]
	b	.L1249
.L1263:
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 294912
	strb	wzr, [x0, 2591]
	b	.L1246
.L1267:
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 311296
	strb	wzr, [x0, 1128]
	b	.L1244
	.size	VCTRL_PostProc, .-VCTRL_PostProc
	.align	3
.LC528:
	.xword	.LANCHOR0
	.align	3
.LC529:
	.xword	.LC5
	.align	3
.LC530:
	.xword	.LC2
	.align	3
.LC531:
	.xword	.L1231
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_VdmPostProc
	.type	VCTRL_VdmPostProc, %function
VCTRL_VdmPostProc:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	mov	w21, w2
	mov	x22, x3
	mov	w23, w4
	bl	_mcount
	cmp	w19, 15
	bhi	.L1276
	ldr	x0, .LC534
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1277
	ldr	w0, [x0, 16]
	cmp	w0, 1
	beq	.L1272
	ldr	x3, .LC536
	mov	w2, 5376
	ldr	x1, .LC533
	mov	w0, 0
	bl	dprint_vfmw
.L1268:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1272:
	mov	w0, w19
	bl	VCTRL_GetDecParam
	cbz	x0, .L1268
	mov	w4, w23
	mov	x3, x22
	mov	w2, w21
	mov	w1, w20
	mov	w0, w19
	bl	VCTRL_PostProc
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1276:
	ldr	x3, .LC532
	mov	w2, 5374
	ldr	x1, .LC533
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1277:
	ldr	x3, .LC535
	mov	w2, 5375
	ldr	x1, .LC533
	bl	dprint_vfmw
	b	.L1268
	.size	VCTRL_VdmPostProc, .-VCTRL_VdmPostProc
	.align	3
.LC532:
	.xword	.LC224
	.align	3
.LC533:
	.xword	.LC2
	.align	3
.LC534:
	.xword	.LANCHOR0
	.align	3
.LC535:
	.xword	.LC186
	.align	3
.LC536:
	.xword	.LC229
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ExtAccPostProcess
	.type	VCTRL_ExtAccPostProcess, %function
VCTRL_ExtAccPostProcess:
	stp	x29, x30, [sp, -192]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	w21, w1
	bl	_mcount
	ldr	x19, .LC537
	mov	x2, 136
	mov	w1, 0
	add	x0, x29, 56
	ldr	x3, [x19, 96]
	blr	x3
	ldr	x1, [x19, 24]
	mov	w0, 0
	blr	x1
	ldr	x1, [x19, 24]
	mov	w0, 5
	blr	x1
	add	x3, x29, 56
	mov	w4, 0
	mov	w2, 0
	mov	w1, w21
	mov	w0, w20
	bl	VCTRL_VdmPostProc
	mov	w0, w20
	bl	VDMDRV_ClearChanIsDec
	ldr	x1, [x19, 32]
	mov	w0, 5
	blr	x1
	ldr	x1, [x19, 32]
	mov	w0, 0
	blr	x1
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 192
	ret
	.size	VCTRL_ExtAccPostProcess, .-VCTRL_ExtAccPostProcess
	.align	3
.LC537:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SvdecLastFrame
	.type	VCTRL_SvdecLastFrame, %function
VCTRL_SvdecLastFrame:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	add	w1, w20, 2
	mov	w0, w19
	bl	VCTRL_ReportLastFrame
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_SvdecLastFrame, .-VCTRL_SvdecLastFrame
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Get_YuvFormat
	.type	Get_YuvFormat, %function
Get_YuvFormat:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cmp	w19, 6
	bls	.L1298
	ldr	x1, .LC540
	mov	w3, w19
	mov	w2, 9829
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 13
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1298:
	ldr	x0, .LC538
	ldrb	w0, [x0,w19,uxtw]
	adr	x1, .Lrtx1283
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1283:
	.section	.rodata
	.align	0
	.align	2
.L1283:
	.byte	(.L1282 - .Lrtx1283) / 4
	.byte	(.L1284 - .Lrtx1283) / 4
	.byte	(.L1285 - .Lrtx1283) / 4
	.byte	(.L1286 - .Lrtx1283) / 4
	.byte	(.L1287 - .Lrtx1283) / 4
	.byte	(.L1288 - .Lrtx1283) / 4
	.byte	(.L1289 - .Lrtx1283) / 4
	.text
	.p2align 3
.L1288:
	cmp	w20, 1
	mov	w1, 12
	mov	w0, 13
	csel	w0, w0, w1, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1289:
	cmp	w20, 1
	mov	w0, 7
	csinc	w0, w0, wzr, ne
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1282:
	cmp	w20, 1
	mov	w0, 6
	csel	w0, wzr, w0, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1284:
	cmp	w20, 1
	mov	w1, 8
	mov	w0, 2
	csel	w0, w0, w1, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1285:
	cmp	w20, 1
	mov	w1, 10
	mov	w0, 4
	csel	w0, w0, w1, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1286:
	cmp	w20, 1
	mov	w1, 9
	mov	w0, 3
	csel	w0, w0, w1, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1287:
	cmp	w20, 1
	mov	w1, 11
	mov	w0, 5
	csel	w0, w0, w1, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Get_YuvFormat, .-Get_YuvFormat
	.align	3
.LC538:
	.xword	.L1283
	.align	3
.LC540:
	.xword	.LC539
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_PutUsrdecFrame
	.type	VDH_PutUsrdecFrame, %function
VDH_PutUsrdecFrame:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	w21, w0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x20, 40]
	mov	w0, w21
	bl	FSP_GetLogicIdByPhyAddr
	cmn	w0, #1
	mov	w22, w0
	beq	.L1308
	ldr	w0, [x20, 4]
	cmp	w0, 1
	beq	.L1302
	mov	w0, w21
	mov	w2, 0
	mov	w1, w22
	bl	FSP_ClearLogicFs
	mov	w0, 0
.L1301:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1302:
	ldr	w0, [x20, 48]
	bl	MEM_Phy2Vir
	cbz	x0, .L1309
.L1303:
	mov	w1, w22
	mov	w0, w21
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L1310
	str	w22, [x0, 156]
	mov	w23, 1
	ldr	x0, [x20, 64]
	str	x0, [x19, 592]
	ldr	x0, [x20, 64]
	str	x0, [x19, 600]
	ldr	x0, [x20, 64]
	str	x0, [x19, 608]
	ldr	x0, [x20, 64]
	str	x0, [x19, 616]
	stp	w23, wzr, [x19, 8]
	stp	wzr, wzr, [x19, 16]
	ldr	w0, [x20, 40]
	str	w0, [x19, 28]
	str	w0, [x19, 44]
	str	w0, [x19, 60]
	ldr	w1, [x20, 48]
	str	w1, [x19, 32]
	str	w1, [x19, 48]
	str	w1, [x19, 64]
	bl	MEM_Phy2Vir
	mov	x1, x0
	ldr	w0, [x19, 32]
	str	x1, [x19, 504]
	str	x1, [x19, 520]
	bl	MEM_Phy2Vir
	str	x0, [x19, 512]
	str	x0, [x19, 528]
	mov	w2, 11296
	ldr	w0, [x19, 28]
	ldr	w1, [x20, 44]
	ldr	w3, [x19, 32]
	add	w1, w0, w1
	str	w1, [x19, 36]
	ldr	w0, [x20, 44]
	str	wzr, [x19, 96]
	add	w0, w3, w0
	str	w0, [x19, 40]
	ldr	w0, [x20, 12]
	str	w0, [x19, 112]
	ldr	w0, [x20, 16]
	str	w0, [x19, 116]
	ldr	w0, [x20, 12]
	str	w0, [x19, 120]
	lsr	w1, w0, 1
	ldr	w0, [x20, 16]
	stp	w0, w1, [x19, 124]
	lsr	w0, w0, 1
	str	w0, [x19, 132]
	ldr	w0, [x20, 44]
	stp	wzr, w0, [x19, 136]
	str	w2, [x19, 108]
	str	wzr, [x19, 160]
	str	w23, [x19, 172]
	ldr	w0, [x20, 24]
	str	w0, [x19, 220]
	bl	MEM_Phy2Vir
	str	x0, [x19, 536]
	ldr	w1, [x20]
	ldr	w0, [x20, 8]
	bl	Get_YuvFormat
	cmp	w0, 13
	beq	.L1311
	str	w0, [x19, 468]
	mov	w24, 20
	mov	w2, w23
	mov	w1, w22
	ldr	w3, [x20, 56]
	mov	w0, w21
	str	w3, [x19, 444]
	ldr	w3, [x20, 60]
	str	w3, [x19, 440]
	ldr	w3, [x20, 52]
	str	w3, [x19, 448]
	str	w24, [x19, 376]
	bl	FSP_SetDisplay
	ldr	x2, .LC549
	mov	x4, x19
	mov	w1, w24
	mov	w0, w21
	ldr	x2, [x2, w21, sxtw 3]
	add	x2, x2, 294912
	add	x3, x2, 2592
	add	x2, x2, 2584
	bl	InsertImgToVoQueue
	cmp	w0, 1
	mov	w0, 0
	beq	.L1301
	ldr	x1, [x19, 560]
	mov	w0, w21
	bl	FreeUsdByDec
	str	xzr, [x19, 560]
	ldr	x1, [x19, 568]
	mov	w0, w21
	bl	FreeUsdByDec
	str	xzr, [x19, 568]
	mov	w0, w21
	mov	w2, 0
	mov	w1, w22
	bl	FSP_SetDisplay
	mov	w0, -1
	b	.L1301
	.p2align 3
.L1309:
	ldr	w0, [x20, 4]
	cmp	w0, 1
	bne	.L1303
	ldr	x1, .LC544
	mov	w0, 0
	ldr	w2, [x20, 48]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1301
.L1308:
	ldr	x1, .LC542
	mov	w0, 0
	ldr	w2, [x20, 40]
	bl	dprint_vfmw
	mov	w0, w22
	b	.L1301
.L1311:
	ldr	x1, .LC548
	mov	w2, w0
	mov	w0, w23
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1301
.L1310:
	ldr	x1, .LC546
	mov	w2, 5864
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1301
	.size	VDH_PutUsrdecFrame, .-VDH_PutUsrdecFrame
	.align	3
.LC542:
	.xword	.LC541
	.align	3
.LC544:
	.xword	.LC543
	.align	3
.LC546:
	.xword	.LC545
	.align	3
.LC548:
	.xword	.LC547
	.align	3
.LC549:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_PutUsrdecFrame
	.type	VCTRL_PutUsrdecFrame, %function
VCTRL_PutUsrdecFrame:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x0, .LC550
	ldr	x0, [x0, w20, sxtw 3]
	cbz	x0, .L1320
	cbz	x19, .L1321
	ldr	w2, [x0, 672]
	cmp	w2, 20
	bne	.L1322
	ldr	x3, .LC557
	mov	x1, x19
	mov	x2, 72
	add	x0, x0, 600
	ldr	x3, [x3, 104]
	blr	x3
	mov	x1, x19
	mov	w0, w20
	bl	VDH_PutUsrdecFrame
	cbnz	w0, .L1323
.L1314:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1322:
	ldr	x1, .LC556
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1314
	.p2align 3
.L1323:
	ldr	x1, .LC559
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1314
.L1321:
	ldr	x3, .LC554
	mov	w2, 6077
	ldr	x1, .LC552
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1314
.L1320:
	ldr	x3, .LC551
	mov	w2, 6076
	ldr	x1, .LC552
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1314
	.size	VCTRL_PutUsrdecFrame, .-VCTRL_PutUsrdecFrame
	.align	3
.LC550:
	.xword	.LANCHOR0
	.align	3
.LC551:
	.xword	.LC5
	.align	3
.LC552:
	.xword	.LC2
	.align	3
.LC554:
	.xword	.LC553
	.align	3
.LC556:
	.xword	.LC555
	.align	3
.LC557:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC559:
	.xword	.LC558
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ResetHardware
	.type	VCTRL_ResetHardware, %function
VCTRL_ResetHardware:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	mov	w0, w19
	bl	SCDDRV_ResetSCD
	mov	w0, w19
	bl	VDMDRV_ResetVDH
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_ResetHardware, .-VCTRL_ResetHardware
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_OpenHardware
	.type	VCTRL_OpenHardware, %function
VCTRL_OpenHardware:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	cmp	w20, 15
	bhi	.L1378
	ldr	x19, .LC562
	ldr	x0, [x19, w20, sxtw 3]
	cbz	x0, .L1379
	ldr	w1, [x0, 12]
	cmp	w1, 1
	bne	.L1329
	ldr	w0, [x0, 672]
	cmp	w0, 21
	beq	.L1329
	and	w1, w0, -17
	cmp	w1, 4
	cset	w2, eq
	cmp	w0, 7
	cset	w1, eq
	orr	w1, w2, w1
	cbnz	w1, .L1325
	cmp	w0, 14
	beq	.L1325
	mov	x1, 0
	mov	w0, 0
	.p2align 2
.L1334:
	cmp	w1, 15
	bhi	.L1333
	ldr	x2, [x19, x1, lsl 3]
	cbz	x2, .L1333
	ldr	w3, [x2, 12]
	cbz	w3, .L1333
	ldr	w3, [x2, 16]
	cbz	w3, .L1333
	ldr	w3, [x2, 28]
	cbz	w3, .L1333
	ldr	w2, [x2, 672]
	cmp	w2, 21
	and	w3, w2, -17
	beq	.L1333
	cmp	w3, 4
	ccmp	w2, 7, 4, ne
	ccmp	w2, 14, 4, ne
	csinc	w0, w0, w0, eq
.L1333:
	add	x1, x1, 1
	cmp	x1, 16
	bne	.L1334
	cmp	w0, 1
	beq	.L1380
.L1335:
	mov	x0, 0
	mov	w5, 0
	.p2align 2
.L1343:
	ldr	x2, [x19, x0]
	mov	w4, 0
	add	x0, x0, 8
	cbz	x2, .L1341
	ldr	w3, [x2, 672]
	cbnz	w3, .L1342
	ldr	w1, [x2, 1300]
	cmp	w1, 1
	cset	w4, eq
.L1342:
	cmp	w3, 6
	cset	w1, eq
	orr	w1, w4, w1
	cmp	w1, wzr
	csinc	w5, w5, wzr, eq
.L1341:
	cmp	x0, 128
	bne	.L1343
	cmp	w5, 1
	beq	.L1381
.L1344:
	ldr	x0, [x19, w20, sxtw 3]
	ldr	w0, [x0, 672]
	cmp	w0, 1
	bne	.L1325
	ldr	x0, .LC567
	ldrb	w0, [x0]
	cmp	w0, 1
	bne	.L1382
	ldr	x0, .LC568
	ldr	x1, [x0, 56]
	cbz	x1, .L1325
	mov	w0, 0
	blr	x1
	b	.L1325
	.p2align 3
.L1329:
	ldr	x1, .LC566
	mov	w2, 9875
	mov	w0, 0
	bl	dprint_vfmw
.L1325:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1378:
	ldr	x1, .LC561
	mov	w2, 9863
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L1382:
	mov	w1, 0
	mov	w0, 2
	bl	VDH_Need_Module
	b	.L1325
	.p2align 3
.L1381:
	ldr	x0, .LC567
	ldrb	w0, [x0]
	cmp	w0, 1
	bne	.L1345
	ldr	x0, .LC568
	ldr	x1, [x0, 24]
	cbz	x1, .L1383
	mov	w0, 0
	blr	x1
	b	.L1344
	.p2align 3
.L1380:
	ldr	x21, .LC567
	ldrb	w1, [x21]
	cmp	w1, 1
	bne	.L1336
	ldr	x0, .LC568
	ldr	x1, [x0, 8]
	cbz	x1, .L1384
	mov	w0, 0
	blr	x1
.L1338:
	ldrb	w0, [x21]
	cmp	w0, 1
	bne	.L1339
	ldr	x0, .LC568
	ldr	x1, [x0, 40]
	cbz	x1, .L1385
	mov	w0, 0
	blr	x1
	b	.L1335
	.p2align 3
.L1379:
	ldr	x1, .LC564
	mov	w2, 9869
	bl	dprint_vfmw
	b	.L1325
.L1339:
	mov	w1, 0
	mov	w0, 0
	bl	VDH_Need_Module
	b	.L1335
.L1336:
	mov	w1, 0
	bl	VDH_Need_Module
	b	.L1338
.L1345:
	mov	w1, 0
	mov	w0, 3
	bl	VDH_Need_Module
	b	.L1344
.L1383:
	ldr	x1, .LC569
	mov	w2, 9966
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1344
.L1385:
	ldr	x1, .LC569
	mov	w2, 9931
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1335
.L1384:
	ldr	x1, .LC569
	mov	w2, 9917
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1338
	.size	VCTRL_OpenHardware, .-VCTRL_OpenHardware
	.align	3
.LC561:
	.xword	.LC560
	.align	3
.LC562:
	.xword	.LANCHOR0
	.align	3
.LC564:
	.xword	.LC563
	.align	3
.LC566:
	.xword	.LC565
	.align	3
.LC567:
	.xword	g_HalDisable
	.align	3
.LC568:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC569:
	.xword	.LC22
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_StartChan
	.type	VCTRL_StartChan, %function
VCTRL_StartChan:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	cmp	w20, 15
	bhi	.L1397
	ldr	x19, .LC572
	ldr	x0, [x19, w20, sxtw 3]
	cbz	x0, .L1398
	ldr	w21, [x0, 12]
	cmp	w21, 1
	bne	.L1399
	ldr	w0, [x0, 672]
	cmp	w0, 21
	beq	.L1400
	mov	w0, w20
	bl	SM_Start
	ldr	x0, [x19, w20, sxtw 3]
	str	w21, [x0, 16]
	ldr	x0, [x19, w20, sxtw 3]
	str	w21, [x0, 20]
	ldr	x0, [x19, w20, sxtw 3]
	str	wzr, [x0, 32]
	ldr	x0, [x19, w20, sxtw 3]
	str	w21, [x0, 24]
	ldr	x1, [x19, w20, sxtw 3]
	ldr	w0, [x1, 28]
	cbnz	w0, .L1392
	str	w21, [x1, 28]
	mov	x1, 0
	ldr	x0, .LC577
	ldr	x5, .LC578
	b	.L1395
	.p2align 3
.L1402:
	ldr	w4, [x2, 28]
	str	w4, [x3, x1, lsl 2]
	ldr	w2, [x2, 28]
	str	w2, [x5, x1, lsl 2]
	add	x1, x1, 1
	cmp	x1, 16
	beq	.L1401
.L1395:
	ldr	x2, [x19, x1, lsl 3]
	sub	x3, x0, #248
	cbnz	x2, .L1402
	sub	x2, x0, #248
	str	wzr, [x5, x1, lsl 2]
	str	wzr, [x2, x1, lsl 2]
	add	x1, x1, 1
	cmp	x1, 16
	bne	.L1395
	.p2align 2
.L1401:
	sub	x0, x0, #248
	bl	VDMDRV_SetPriority
	mov	w0, w20
	bl	VDMDRV_ClearChanIsDec
.L1392:
	mov	w0, w20
	bl	VDMDRV_ClearChanIsDec
	mov	w0, w20
	bl	VCTRL_OpenHardware
	mov	w0, 0
.L1388:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L1399:
	ldr	x3, .LC574
	mov	w2, 2893
	ldr	x1, .LC571
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1388
.L1397:
	ldr	x3, .LC570
	mov	w2, 2891
	ldr	x1, .LC571
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1388
.L1400:
	ldr	x3, .LC576
	mov	w2, 2894
	ldr	x1, .LC571
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1388
.L1398:
	ldr	x3, .LC573
	mov	w2, 2892
	ldr	x1, .LC571
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1388
	.size	VCTRL_StartChan, .-VCTRL_StartChan
	.align	3
.LC570:
	.xword	.LC51
	.align	3
.LC571:
	.xword	.LC2
	.align	3
.LC572:
	.xword	.LANCHOR0
	.align	3
.LC573:
	.xword	.LC95
	.align	3
.LC574:
	.xword	.LC253
	.align	3
.LC576:
	.xword	.LC575
	.align	3
.LC577:
	.xword	.LANCHOR5
	.align	3
.LC578:
	.xword	g_ChanCtx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_CloseHardware
	.type	VCTRL_CloseHardware, %function
VCTRL_CloseHardware:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w21, w0
	mov	x0, x30
	mov	w19, 0
	mov	w20, 0
	bl	_mcount
	ldr	x4, .LC580
	mov	x1, 0
	mov	w5, 0
	.p2align 2
.L1409:
	ldr	x0, [x4, x1]
	cbz	x0, .L1404
	ldr	w2, [x0, 672]
	cbnz	w5, .L1405
	ldr	w3, [x0, 12]
	cmp	w3, 1
	beq	.L1449
.L1406:
	mov	w3, 0
	cbnz	w2, .L1408
	ldr	w3, [x0, 1300]
.L1408:
	cbnz	w19, .L1404
	cmp	w2, 6
	ccmp	w3, 1, 4, ne
	bne	.L1404
	ldr	w2, [x0, 12]
	cmp	w2, 1
	beq	.L1450
	.p2align 2
.L1404:
	add	x1, x1, 8
	cmp	x1, 128
	bne	.L1409
	cbnz	w5, .L1410
	ldr	x22, .LC581
	ldrb	w0, [x22]
	cmp	w0, 1
	beq	.L1415
	mov	w0, w21
	bl	SCDDRV_ResetSCD
	mov	w0, w21
	bl	VDMDRV_ResetVDH
	ldrb	w0, [x22]
	cmp	w0, 1
	beq	.L1415
	mov	w1, 0
	mov	w0, 1
	bl	VDH_UnNeed_Module
	ldrb	w0, [x22]
	cmp	w0, 1
	bne	.L1418
.L1457:
	ldr	x0, .LC582
	ldr	x1, [x0, 48]
	cbz	x1, .L1451
	mov	w0, 0
	blr	x1
.L1421:
	mov	w0, 0
	bl	SCDDRV_SetStateIdle
.L1410:
	cbnz	w19, .L1422
	ldr	x0, .LC581
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L1452
	mov	w1, 0
	mov	w0, 3
	bl	VDH_UnNeed_Module
.L1422:
	cbnz	w20, .L1403
	ldr	x0, .LC581
	ldrb	w0, [x0]
	cmp	w0, 1
	bne	.L1453
	ldr	x0, .LC582
	ldr	x1, [x0, 64]
	cbz	x1, .L1403
	mov	w0, 0
	blr	x1
.L1403:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1405:
	cmp	w20, wzr
	ccmp	w2, 1, 0, eq
	bne	.L1406
	ldr	w3, [x0, 12]
	cmp	w3, 1
	bne	.L1406
	ldr	w3, [x0, 16]
.L1427:
	cmp	w3, 1
	csinc	w20, w20, wzr, ne
	b	.L1406
	.p2align 3
.L1449:
	ldr	w3, [x0, 16]
	cmp	w3, 1
	beq	.L1454
.L1407:
	cmp	w2, 1
	ccmp	w20, wzr, 0, eq
	beq	.L1427
	b	.L1406
	.p2align 3
.L1450:
	ldr	w0, [x0, 16]
	cmp	w0, 1
	cset	w19, eq
	b	.L1404
	.p2align 3
.L1454:
	sub	w6, w2, #5
	mov	w5, w3
	cmp	w6, 1
	ccmp	w2, 3, 0, hi
	cset	w6, ls
	cbnz	w6, .L1407
	sub	w6, w2, #8
	cmp	w6, 5
	bls	.L1407
	sub	w5, w2, #15
	cmp	w5, 2
	cset	w5, ls
	b	.L1407
	.p2align 3
.L1453:
	mov	w1, 0
	mov	w0, 2
	bl	VDH_UnNeed_Module
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1452:
	ldr	x0, .LC582
	ldr	x1, [x0, 32]
	cbz	x1, .L1455
	mov	w0, 0
	blr	x1
	b	.L1422
	.p2align 3
.L1415:
	ldr	x0, .LC582
	ldr	x1, [x0, 16]
	cbz	x1, .L1456
	mov	w0, 0
	blr	x1
.L1417:
	mov	w0, 0
	bl	VDMDRV_SetStateIdle
	ldrb	w0, [x22]
	cmp	w0, 1
	beq	.L1457
.L1418:
	mov	w1, 0
	mov	w0, 0
	bl	VDH_UnNeed_Module
	b	.L1410
.L1456:
	ldr	x1, .LC583
	mov	w2, 10075
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1417
.L1451:
	ldr	x1, .LC583
	mov	w2, 10090
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1421
.L1455:
	ldr	x1, .LC583
	mov	w2, 10108
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1422
	.size	VCTRL_CloseHardware, .-VCTRL_CloseHardware
	.align	3
.LC580:
	.xword	.LANCHOR0
	.align	3
.LC581:
	.xword	g_HalDisable
	.align	3
.LC582:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC583:
	.xword	.LC22
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_StopChan
	.type	VCTRL_StopChan, %function
VCTRL_StopChan:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L1464
	ldr	x20, .LC586
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1465
	ldr	w1, [x0, 12]
	cmp	w1, 1
	bne	.L1463
	str	w1, [x0, 32]
	mov	w0, w19
	ldr	x1, [x20, w19, sxtw 3]
	str	wzr, [x1, 20]
	ldr	x1, [x20, w19, sxtw 3]
	str	wzr, [x1, 16]
	bl	SM_Stop
	cbnz	w0, .L1463
	mov	w1, 0
	mov	w0, w19
	bl	DeleteRawPacketInBuffer
	mov	w0, w19
	bl	VDMDRV_ClearChanIsDec
	ldr	x1, [x20, w19, sxtw 3]
	mov	w0, w19
	str	wzr, [x1, 32]
	bl	VCTRL_CloseHardware
	mov	w0, 0
.L1460:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1463:
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1464:
	ldr	x3, .LC584
	mov	w2, 3000
	ldr	x1, .LC585
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1460
.L1465:
	ldr	x3, .LC588
	mov	w2, 3001
	ldr	x1, .LC585
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1460
	.size	VCTRL_StopChan, .-VCTRL_StopChan
	.align	3
.LC584:
	.xword	.LC51
	.align	3
.LC585:
	.xword	.LC2
	.align	3
.LC586:
	.xword	.LANCHOR0
	.align	3
.LC588:
	.xword	.LC587
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_StopChanWithCheck
	.type	VCTRL_StopChanWithCheck, %function
VCTRL_StopChanWithCheck:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w22, w0
	mov	x0, x30
	mov	w20, 0
	bl	_mcount
	ldr	x19, .LC589
	b	.L1468
	.p2align 3
.L1472:
	ldr	x1, [x19, 232]
	mov	w0, 10
	add	w20, w20, 1
	blr	x1
	cmp	w20, 50
	beq	.L1470
.L1468:
	ldr	x1, [x19, 24]
	mov	w0, 0
	blr	x1
	mov	w0, w22
	bl	VCTRL_StopChan
	mov	w21, w0
	ldr	x1, [x19, 32]
	mov	w0, 0
	blr	x1
	cbnz	w21, .L1472
	cmp	w20, 49
	bgt	.L1470
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1470:
	ldr	x1, .LC591
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	VCTRL_StopChanWithCheck, .-VCTRL_StopChanWithCheck
	.align	3
.LC589:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC591:
	.xword	.LC590
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ResetChan
	.type	VCTRL_ResetChan, %function
VCTRL_ResetChan:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC592
	ldr	x1, [x20, w19, sxtw 3]
	cbz	x1, .L1496
	mov	w0, w19
	ldr	w22, [x1, 16]
	bl	VCTRL_StopChan
	ldr	x21, .LC595
	ldr	x1, [x20, w19, sxtw 3]
	mov	w0, 4
	ldr	x2, [x21, 24]
	str	wzr, [x1, 452]
	str	wzr, [x1, 456]
	blr	x2
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w1, [x0, 1512]
	cmp	w1, 1
	beq	.L1497
.L1476:
	sxtw	x3, w19
	ldr	x0, .LC596
	lsl	x1, x3, 4
	ldr	x4, [x21, 96]
	add	x3, x1, x3, lsl 2
	add	x0, x0, 3648
	mov	x2, 300
	mov	w1, 0
	add	x0, x0, x3, lsl 4
	sub	x0, x0, x3
	blr	x4
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1498
	add	x0, x0, 290816
	str	wzr, [x0, 2484]
.L1478:
	ldr	x1, [x20, w19, sxtw 3]
	mov	w0, w19
	str	wzr, [x1, 1288]
	bl	InitSyntaxCtx
	ldr	x0, [x20, w19, sxtw 3]
	mov	x2, 416
	ldr	x3, [x21, 96]
	mov	w1, 0
	add	x0, x0, 816
	blr	x3
	ldr	x0, [x20, w19, sxtw 3]
	mov	x2, -1
	add	x1, x0, 294912
	str	x2, [x0, 736]
	str	x2, [x0, 744]
	str	wzr, [x0, 1232]
	str	wzr, [x1, 2552]
	str	wzr, [x1, 2556]
	str	wzr, [x1, 2560]
	ldr	w2, [x0, 1512]
	cmp	w2, 1
	beq	.L1499
.L1479:
	mov	w0, w19
	bl	FSP_SwitchNotify
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w0, [x0, 672]
	cmp	w0, 20
	mov	w0, w19
	beq	.L1480
	bl	FSP_ResetInstance
.L1481:
	ldr	x1, [x21, 32]
	mov	w0, 4
	blr	x1
	mov	w0, w19
	bl	InitUsdPool
	mov	w0, w19
	bl	SM_Reset
	ldr	x0, .LC597
	ldr	w1, [x0, -184]
	cbz	w1, .L1482
	sub	x20, x0, #176
	str	w19, [x29, 60]
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1484
	mov	w0, 512
	add	x1, x29, 60
	bl	VCTRL_SetDbgOption
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1484
	add	x1, x29, 60
	mov	w0, 512
	bl	VCTRL_SetDbgOption
.L1484:
	ldr	x1, .LC599
	mov	w0, 32
	bl	dprint_vfmw
.L1482:
	cbnz	w22, .L1500
.L1486:
	mov	w0, w19
	bl	VDMDRV_ClearChanIsDec
	mov	w0, 0
.L1475:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1500:
	mov	w0, w19
	bl	VCTRL_StartChan
	b	.L1486
	.p2align 3
.L1480:
	bl	FSP_EmptyInstance
	b	.L1481
	.p2align 3
.L1499:
	str	wzr, [x0, 1552]
	str	wzr, [x1, 2532]
	b	.L1479
	.p2align 3
.L1497:
	add	x1, x0, 294912
	ldr	w2, [x0, 672]
	ldr	w1, [x1, 2576]
	cmp	w1, w2
	bne	.L1476
	str	wzr, [x0, 20]
	mov	w0, w19
	bl	VCTRL_FindOccupiedFrame
	b	.L1476
	.p2align 3
.L1498:
	bl	VCTRL_SetSendStreamAgainFlag.part.2
	b	.L1478
.L1496:
	ldr	x3, .LC593
	mov	w2, 4447
	ldr	x1, .LC594
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1475
	.size	VCTRL_ResetChan, .-VCTRL_ResetChan
	.align	3
.LC592:
	.xword	.LANCHOR0
	.align	3
.LC593:
	.xword	.LC5
	.align	3
.LC594:
	.xword	.LC2
	.align	3
.LC595:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC596:
	.xword	.LANCHOR3
	.align	3
.LC597:
	.xword	.LANCHOR5
	.align	3
.LC599:
	.xword	.LC598
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_CreateChanWithOption
	.type	VCTRL_CreateChanWithOption, %function
VCTRL_CreateChanWithOption:
	stp	x29, x30, [sp, -320]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	w20, w0
	mov	x0, x30
	mov	x25, x1
	mov	w21, w2
	bl	_mcount
	ldr	x22, .LC602
	mov	x2, 24
	mov	w1, 0
	add	x0, x29, 136
	ldr	x3, [x22, 96]
	blr	x3
	ldr	x3, [x22, 96]
	mov	x2, 24
	mov	w1, 0
	add	x0, x29, 160
	blr	x3
	ldr	x3, [x22, 96]
	mov	x2, 24
	mov	w1, 0
	add	x0, x29, 184
	blr	x3
	ldr	x4, [x22, 96]
	mov	x2, 24
	mov	w1, 0
	add	x0, x29, 208
	blr	x4
	ldr	x4, [x22, 96]
	mov	x2, 56
	mov	w1, 0
	add	x0, x29, 264
	blr	x4
	ldr	x24, .LC603
	ldr	w0, [x25, 36]
	str	x25, [x24, 80]
	tbnz	w0, #31, .L1502
	ldr	w0, [x25, 12]
	cmp	w0, 31
	ble	.L1502
	ldr	w0, [x25, 16]
	cmp	w0, 31
	ble	.L1502
	ldr	w1, [x25, 56]
	mov	w0, 262143
	cmp	w1, w0
	ble	.L1502
	ldr	w0, [x25, 44]
	cmp	w0, 1
	beq	.L1563
	cmp	w0, 2
	beq	.L1564
.L1504:
	ldr	w0, [x25, 176]
	cmp	w0, 1
	bls	.L1507
	str	wzr, [x25, 176]
.L1507:
	mov	w4, 1800
	mov	w19, 1
	cmp	w20, 26
	mov	w26, 11640
	movk	w4, 0xb2, lsl 16
	mov	w3, w21
	add	x2, x29, 120
	mov	x1, x25
	mov	w0, w20
	movk	w26, 0xc, lsl 16
	stp	w19, w19, [x25, 200]
	csel	w26, w26, w4, ne
	bl	VCTRL_GetChanMemSizeWithOption
	cbnz	w0, .L1565
	add	x21, x25, 80
	ldr	x0, [x21, 56]
	cbz	x0, .L1510
	ldr	w0, [x21, 64]
	cbnz	w0, .L1511
.L1510:
	mov	w0, 3
	str	w0, [x29, 260]
	mov	w0, 5
	bl	VFMW_GetString
	ldr	w2, [x29, 128]
	mov	w4, 1
	add	x3, x29, 232
	mov	w1, 0
	bl	MEM_AllocMemBlock
	cbnz	w0, .L1566
	ldr	w0, [x29, 248]
	mov	w19, 1
	ldr	w2, [x29, 256]
	ldr	x1, [x29, 240]
	bl	MEM_AddMemRecord
	ldr	w0, [x29, 248]
	str	w0, [x29, 184]
	ldr	x0, [x29, 240]
	str	x0, [x29, 192]
	ldr	w0, [x29, 256]
	str	w0, [x29, 200]
.L1513:
	ldr	x0, [x21, 8]
	cbnz	x0, .L1567
.L1515:
	ldr	w0, [x25, 176]
	cmp	w0, 1
	beq	.L1517
	mov	w0, 3
	str	w0, [x29, 260]
	mov	w0, 6
	bl	VFMW_GetString
	ldr	w2, [x29, 120]
	mov	w4, 0
	add	x3, x29, 232
	mov	w1, 0
	bl	MEM_AllocMemBlock
	cbnz	w0, .L1568
	ldr	w0, [x29, 248]
	ldr	w2, [x29, 256]
	ldr	x1, [x29, 240]
	bl	MEM_AddMemRecord
	ldr	w0, [x29, 248]
	str	w0, [x29, 136]
	ldr	x0, [x29, 240]
	str	x0, [x29, 144]
	ldr	w0, [x29, 256]
	str	w0, [x29, 152]
	mov	w0, 1
	str	w0, [x29, 108]
.L1520:
	ldr	x0, [x21, 32]
	cbz	x0, .L1524
	ldr	w0, [x21, 40]
	cbnz	w0, .L1525
.L1524:
	mov	w1, 3
	mov	w0, 7
	str	w1, [x29, 260]
	bl	VFMW_GetString
	ldr	w2, [x29, 124]
	mov	w4, 0
	add	x3, x29, 232
	mov	w1, 0
	bl	MEM_AllocMemBlock
	cbnz	w0, .L1569
	ldr	x1, [x29, 240]
	ldr	w0, [x29, 248]
	ldr	w2, [x29, 256]
	bl	MEM_AddMemRecord
	ldr	x0, [x29, 240]
	ldr	w1, [x29, 248]
	str	x0, [x29, 168]
	ldr	w0, [x29, 256]
	str	w0, [x29, 176]
	mov	w0, 1
	str	w1, [x29, 160]
	str	w0, [x29, 104]
.L1529:
	ldr	x1, [x22, 24]
	mov	w0, 0
	blr	x1
	ldr	x28, .LC619
	ldr	x0, [x28]
	cbz	x0, .L1543
	add	x0, x28, 8
	mov	w21, 1
	.p2align 2
.L1536:
	ldr	x1, [x0]
	add	x0, x0, 8
	cbz	x1, .L1570
	add	w21, w21, 1
	cmp	w21, 16
	bne	.L1536
	ldr	x1, .LC623
	mov	w2, w21
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x22, 32]
	mov	w0, 0
	blr	x1
	mov	w0, -1
.L1558:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 320
	ret
	.p2align 3
.L1567:
	ldr	w0, [x21, 16]
	cbz	w0, .L1515
	ldr	x3, [x22, 104]
	mov	x2, 24
	add	x0, x29, 136
	mov	x1, x21
	blr	x3
	ldr	w0, [x25, 176]
	ldr	w2, [x29, 152]
	cmp	w0, 1
	beq	.L1522
	ldr	w4, [x29, 120]
	cmp	w2, w4
	blt	.L1571
.L1522:
	ldr	w0, [x29, 136]
	ldr	x1, [x29, 144]
	bl	MEM_AddMemRecord
	str	wzr, [x29, 108]
	b	.L1520
	.p2align 3
.L1517:
	str	wzr, [x29, 108]
	str	wzr, [x29, 136]
	str	xzr, [x29, 144]
	str	wzr, [x29, 152]
	b	.L1520
	.p2align 3
.L1564:
	ldr	w0, [x25, 20]
	cmp	w0, wzr
	ble	.L1502
	ldr	w0, [x25, 24]
	cmp	w0, wzr
	ble	.L1502
.L1560:
	ldr	w0, [x25, 28]
	cmp	w0, wzr
	ble	.L1502
	ldr	w0, [x25, 32]
	cmp	w0, wzr
	bgt	.L1504
.L1502:
	ldr	x1, .LC604
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 320
	ret
	.p2align 3
.L1570:
	sxtb	w0, w21
	str	w0, [x29, 100]
.L1534:
	sxtw	x23, w26
	ldr	x4, [x22, 96]
	ldr	x0, [x29, 192]
	sub	x2, x23, #204800
	sub	x2, x2, #1544
	mov	w1, 0
	str	x0, [x28, w21, sxtw 3]
	sxtw	x27, w21
	blr	x4
	ldr	x0, [x28, w21, sxtw 3]
	mov	x2, 208
	ldr	x4, [x22, 104]
	mov	x1, x25
	add	x0, x0, 1336
	blr	x4
	ldr	x0, [x28, w21, sxtw 3]
	mov	w2, 21
	ldr	x9, .LC624
	lsl	x6, x27, 4
	ldr	x8, .LC625
	mov	w7, 30
	str	w20, [x0, 44]
	str	w26, [x0, 1260]
	mov	w26, 255
	ldr	x0, [x28, w21, sxtw 3]
	str	w21, [x0, 8]
	str	w20, [x0, 708]
	mov	w20, 0
	str	wzr, [x0, 1280]
	ldr	x0, [x28, w21, sxtw 3]
	ldr	w1, [x25]
	str	w1, [x0, 40]
	mov	w0, w21
	ldr	x1, [x28, w21, sxtw 3]
	add	x10, x1, 294912
	str	w2, [x1, 672]
	str	w2, [x10, 2576]
	str	wzr, [x1, 36]
	ldr	x1, [x28, w21, sxtw 3]
	add	x2, x1, 1592
	str	x2, [x9, x6]
	add	x6, x1, 262144
	add	x6, x6, 2048
	str	x6, [x8, w21, sxtw 3]
	ldr	w6, [x25, 200]
	str	w6, [x1, 1664]
	ldr	w6, [x25, 204]
	str	w6, [x1, 1668]
	str	w7, [x1, 1584]
	bl	VCTRL_ResetChan
	ldr	x0, [x28, w21, sxtw 3]
	ldr	w1, [x29, 108]
	str	w1, [x0, 360]
	ldr	w1, [x29, 104]
	str	w1, [x0, 364]
	add	x1, x0, 290816
	add	x2, x1, 2304
	str	w19, [x0, 368]
	add	x0, x0, 294912
	stp	wzr, wzr, [x2, 156]
	stp	wzr, wzr, [x2, 164]
	str	wzr, [x1, 2476]
	str	wzr, [x1, 2480]
	ldr	w1, [x25, 176]
	str	w1, [x0, 2540]
	str	wzr, [x0, 2532]
	.p2align 2
.L1538:
	uxtw	x1, w20
	ldr	x6, [x28, x27, lsl 3]
	lsl	x19, x1, 6
	ldr	x5, [x22, 96]
	add	x19, x19, x1, lsl 3
	mov	x2, 72
	add	x0, x19, 290816
	mov	w1, 0
	add	x0, x6, x0
	add	w20, w20, 1
	add	x0, x0, 2488
	blr	x5
	ldr	x0, [x28, x27, lsl 3]
	cmp	w20, 32
	add	x1, x0, x19
	add	x1, x1, 290816
	str	w26, [x1, 2488]
	str	w26, [x1, 2492]
	str	w26, [x1, 2496]
	bne	.L1538
	ldr	x5, [x22, 96]
	add	x0, x0, 294912
	mov	x2, 1288
	mov	w1, 0
	add	x0, x0, 696
	add	x24, x24, 88
	blr	x5
	ldr	x0, [x28, x27, lsl 3]
	mov	x2, 24
	ldr	x5, [x22, 96]
	mov	w1, 0
	add	x0, x0, 294912
	add	x0, x0, 1984
	blr	x5
	str	wzr, [x24, x27, lsl 2]
	ldr	x0, [x28, x27, lsl 3]
	ldr	w7, [x29, 184]
	ldr	x4, [x29, 144]
	add	x6, x0, 290816
	str	x4, [x0, 272]
	str	w7, [x0, 344]
	ldr	x4, [x29, 168]
	ldr	x7, [x29, 192]
	ldr	w1, [x29, 136]
	ldr	w2, [x29, 152]
	str	x4, [x0, 304]
	str	x7, [x0, 336]
	ldr	w5, [x29, 160]
	ldr	w4, [x29, 176]
	ldr	w7, [x29, 200]
	str	w1, [x0, 280]
	str	w2, [x0, 288]
	str	w5, [x0, 312]
	str	w1, [x0, 396]
	str	w2, [x0, 400]
	str	w4, [x0, 320]
	str	w7, [x0, 352]
	str	wzr, [x6, 2440]
	ldr	w6, [x0, 1512]
	cmp	w6, 1
	beq	.L1539
	str	w5, [x0, 392]
	ldr	w5, [x0, 388]
	str	w4, [x0, 404]
	ldr	w4, [x0, 4]
	sub	w2, w2, w5
	sub	w2, w2, w4
	add	w4, w5, w4
	sub	w2, w2, #307200
	add	w1, w4, w1
	sub	w2, w2, #2576
	str	w1, [x0, 376]
	add	w1, w2, w1
	str	w2, [x0, 380]
	add	w4, w1, 12288
	add	w2, w1, 45056
	add	w4, w4, 2575
	add	w1, w1, 15
	mov	w5, 14848
	add	w2, w2, 2575
	str	w5, [x0, 412]
	and	w1, w1, -16
	and	w4, w4, -16
.L1561:
	str	w1, [x0, 408]
	mov	w1, 262144
	str	w1, [x0, 428]
	and	w2, w2, -16
	ldr	x1, [x28, w21, sxtw 3]
	str	w4, [x0, 416]
	mov	w4, 32768
	str	w2, [x0, 424]
	str	w4, [x0, 420]
	mov	w0, w21
	str	w21, [x1, 8]
	ldr	x1, [x28, w21, sxtw 3]
	add	x2, x1, 262144
	stp	w21, w21, [x1, 248]
	ldrb	w1, [x29, 100]
	strb	w1, [x2, 2048]
	str	w21, [x2, 1824]
	bl	FSP_EnableInstance
	cbnz	w0, .L1572
	mov	w0, w21
	bl	SM_ClearInst
	ldr	x2, [x28, w21, sxtw 3]
	sub	x1, x23, #200704
	sub	x1, x1, #3592
	mov	w0, w21
	ldr	x2, [x2, 336]
	add	x1, x2, x1
	bl	OpenUsdPool_WithMem
	ldr	x1, [x28, w21, sxtw 3]
	mov	w2, 1
	mov	w0, w21
	str	w2, [x1, 12]
	bl	VDMDRV_ClearChanIsDec
	bl	VDMDRV_ParamInit
	ldr	x19, [x28, w21, sxtw 3]
	ldr	x0, [x22]
	add	x19, x19, 294912
	blr	x0
	ldr	x1, [x22, 32]
	str	w0, [x19, 2548]
	mov	w0, 0
	blr	x1
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 320
	ret
	.p2align 3
.L1563:
	ldr	w0, [x25, 20]
	cmp	w0, wzr
	bgt	.L1560
	b	.L1502
	.p2align 3
.L1511:
	ldr	x3, [x22, 104]
	mov	x2, 24
	add	x0, x29, 184
	add	x1, x25, 128
	blr	x3
	ldr	w0, [x29, 128]
	ldr	w2, [x29, 200]
	cmp	w2, w0
	blt	.L1573
	ldr	w0, [x29, 184]
	mov	w19, 0
	ldr	x1, [x29, 192]
	bl	MEM_AddMemRecord
	b	.L1513
.L1539:
	sub	w1, w4, #307200
	str	w5, [x0, 392]
	sub	w1, w1, #2576
	str	w1, [x0, 404]
	add	w2, w1, w5
	str	wzr, [x0, 376]
	add	w4, w2, 45056
	add	w5, w2, 12288
	add	w1, w2, 15
	add	w5, w5, 2575
	add	w2, w4, 2575
	mov	w4, 14848
	str	wzr, [x0, 380]
	and	w1, w1, -16
	str	w4, [x0, 412]
	and	w4, w5, -16
	b	.L1561
.L1525:
	ldr	x3, [x22, 104]
	add	x1, x25, 104
	add	x0, x29, 160
	mov	x2, 24
	blr	x3
	ldr	x1, [x29, 168]
	cbz	x1, .L1530
	ldr	w0, [x29, 124]
	ldr	w2, [x29, 176]
	cmp	w2, w0
	blt	.L1530
	ldr	w0, [x29, 160]
	bl	MEM_AddMemRecord
	str	wzr, [x29, 104]
	b	.L1529
.L1569:
	cbnz	w19, .L1574
.L1527:
	ldr	w0, [x29, 108]
	cbnz	w0, .L1575
.L1528:
	ldr	x1, .LC616
	mov	w2, 2363
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1558
.L1575:
	ldr	w0, [x29, 136]
	ldr	x1, [x29, 144]
	bl	MEM_ReleaseMemBlock
	ldr	w0, [x29, 136]
	ldr	w2, [x29, 152]
	ldr	x1, [x29, 144]
	bl	MEM_DelMemRecord
	b	.L1528
.L1568:
	cbnz	w19, .L1576
.L1519:
	ldr	x1, .LC612
	mov	w2, 2305
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1558
.L1543:
	mov	w21, 0
	str	wzr, [x29, 100]
	b	.L1534
.L1566:
	ldr	x1, .LC608
	mov	w2, 2267
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1558
.L1572:
	ldr	x1, .LC621
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w21
	bl	FSP_DisableInstance
	mov	w0, w21
	bl	CloseUsdPool_WithMem
	ldr	x1, [x28, w21, sxtw 3]
	add	x7, x29, 256
	str	xzr, [x28, w21, sxtw 3]
	mov	w0, w21
	add	x5, x1, 294912
	add	x2, x1, 290816
	ldr	w6, [x1, 364]
	add	x2, x2, 2488
	ldr	w3, [x1, 360]
	ldr	w4, [x1, 368]
	ldr	w5, [x5, 2540]
	str	x2, [x29, 288]
	stp	w5, w3, [x7, 40]
	stp	w6, w4, [x7, 48]
	add	x3, x1, 296
	add	x4, x1, 264
	add	x1, x1, 328
	str	x1, [x29, 280]
	stp	x4, x3, [x29, 264]
	bl	VDMDRV_ClearChanIsDec
	ldr	x1, [x22, 32]
	mov	w0, 0
	blr	x1
	add	x1, x29, 264
	mov	w0, w21
	bl	VCTRL_FreeChanResource
	mov	w0, -1
	b	.L1558
.L1530:
	cbnz	w19, .L1577
.L1532:
	ldr	w0, [x29, 108]
	cbnz	w0, .L1578
.L1533:
	ldr	x1, .LC618
	mov	w2, 2392
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1558
.L1565:
	ldr	x1, .LC606
	mov	w2, 2257
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1558
.L1574:
	ldr	w0, [x29, 184]
	ldr	x1, [x29, 192]
	bl	MEM_ReleaseMemBlock
	ldr	w0, [x29, 184]
	ldr	w2, [x29, 200]
	ldr	x1, [x29, 192]
	bl	MEM_DelMemRecord
	b	.L1527
.L1576:
	ldr	w0, [x29, 184]
	ldr	x1, [x29, 192]
	bl	MEM_ReleaseMemBlock
	ldr	w0, [x29, 184]
	ldr	w2, [x29, 200]
	ldr	x1, [x29, 192]
	bl	MEM_DelMemRecord
	b	.L1519
.L1573:
	ldr	x1, .LC610
	mov	w2, 2283
	mov	w0, w19
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1558
.L1571:
	cbnz	w19, .L1579
.L1523:
	ldr	x1, .LC614
	mov	w3, w2
	mov	w0, 1
	mov	w2, 2337
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1558
.L1578:
	ldr	w0, [x29, 136]
	ldr	x1, [x29, 144]
	bl	MEM_ReleaseMemBlock
	ldr	w0, [x29, 136]
	ldr	w2, [x29, 152]
	ldr	x1, [x29, 144]
	bl	MEM_DelMemRecord
	b	.L1533
.L1577:
	ldr	w0, [x29, 184]
	ldr	x1, [x29, 192]
	bl	MEM_ReleaseMemBlock
	ldr	w0, [x29, 184]
	ldr	w2, [x29, 200]
	ldr	x1, [x29, 192]
	bl	MEM_DelMemRecord
	b	.L1532
.L1579:
	ldr	w0, [x29, 184]
	ldr	x1, [x29, 192]
	bl	MEM_ReleaseMemBlock
	ldr	w2, [x29, 200]
	ldr	w0, [x29, 184]
	ldr	x1, [x29, 192]
	bl	MEM_DelMemRecord
	ldr	w4, [x29, 120]
	ldr	w2, [x29, 152]
	b	.L1523
	.size	VCTRL_CreateChanWithOption, .-VCTRL_CreateChanWithOption
	.align	3
.LC602:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC603:
	.xword	.LANCHOR5
	.align	3
.LC604:
	.xword	.LC299
	.align	3
.LC606:
	.xword	.LC605
	.align	3
.LC608:
	.xword	.LC607
	.align	3
.LC610:
	.xword	.LC609
	.align	3
.LC612:
	.xword	.LC611
	.align	3
.LC614:
	.xword	.LC613
	.align	3
.LC616:
	.xword	.LC615
	.align	3
.LC618:
	.xword	.LC617
	.align	3
.LC619:
	.xword	.LANCHOR0
	.align	3
.LC621:
	.xword	.LC620
	.align	3
.LC623:
	.xword	.LC622
	.align	3
.LC624:
	.xword	s_SmIIS
	.align	3
.LC625:
	.xword	s_pFspInst
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_CreateChan
	.type	VCTRL_CreateChan, %function
VCTRL_CreateChan:
	stp	x29, x30, [sp, -336]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	cmp	w19, 32
	bhi	.L1604
	ldr	x20, .LC629
	mov	x2, 208
	mov	w1, 0
	add	x0, x29, 128
	ldr	x3, [x20, 96]
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 24
	mov	w1, 0
	add	x0, x29, 80
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 24
	mov	w1, 0
	add	x0, x29, 104
	blr	x3
	uxtw	x1, w19
	ldr	x2, .LC630
	lsl	x0, x1, 3
	mov	w3, 2
	sub	x0, x0, x1
	str	w3, [x29, 136]
	lsl	x0, x0, 2
	add	x4, x2, x0
	ldr	w3, [x2, x0]
	ldr	w0, [x4, 4]
	stp	w3, w0, [x29, 140]
	mov	w3, 136
	sub	w0, w19, #18
	str	w3, [x29, 148]
	mov	w3, 32
	cmp	w0, 7
	str	w3, [x29, 156]
	mov	w3, 256
	str	w3, [x29, 160]
	bls	.L1605
	mov	w4, 8
	mov	w3, 6
.L1586:
	lsl	x0, x1, 3
	sub	x0, x0, x1
	add	x0, x2, x0, lsl 2
	ldr	w0, [x0, 12]
	cmp	w0, w4
	bgt	.L1587
.L1609:
	mov	w0, 1
	str	w0, [x29, 164]
.L1588:
	lsl	x0, x1, 3
	cmp	w19, 24
	sub	x1, x0, x1
	stp	wzr, wzr, [x29, 192]
	add	x1, x2, x1, lsl 2
	ldr	w0, [x1, 24]
	stp	w0, w3, [x29, 184]
	mov	w3, 1
	str	w3, [x29, 180]
	stp	w3, w3, [x29, 168]
	beq	.L1606
	cmp	w19, 26
	beq	.L1607
	cmp	w19, 18
	bne	.L1590
	mov	w1, 4
	mov	w0, 2
	str	w1, [x29, 164]
	str	w0, [x29, 188]
.L1590:
	add	x2, x29, 64
	add	x1, x29, 48
	mov	w0, w19
	bl	VCTRL_GetChanMemSize
	cbnz	w0, .L1608
	cbz	x21, .L1597
	ldr	w0, [x21, 16]
	cbz	w0, .L1597
	ldr	w2, [x21]
	cbz	w2, .L1594
	ldr	x5, [x21, 8]
	cbz	x5, .L1594
	ldr	w3, [x29, 48]
	ldr	w1, [x29, 64]
	add	w1, w3, w1
	cmp	w0, w1
	blt	.L1594
	ldr	w4, [x29, 68]
	mov	w6, 3
	str	w2, [x29, 104]
	add	x1, x29, 104
	add	w3, w4, w2
	sub	w0, w0, w4
	add	x2, x5, x4, sxtw
	str	w6, [x29, 136]
	ldr	x6, [x20, 104]
	str	w3, [x29, 80]
	str	w0, [x29, 96]
	add	x0, x29, 232
	str	x5, [x29, 112]
	str	w4, [x29, 120]
	str	x2, [x29, 88]
	mov	x2, 24
	blr	x6
	ldr	x3, [x20, 104]
	mov	x2, 24
	add	x1, x29, 80
	add	x0, x29, 208
	blr	x3
	mov	w3, 1
.L1593:
	mov	w2, 1
	add	x1, x29, 128
	mov	w0, w19
	bl	VCTRL_CreateChanWithOption
	cmp	w0, wzr
	csinv	w0, w0, wzr, ge
.L1582:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 336
	ret
	.p2align 3
.L1605:
	ldr	x3, .LC631
	ldrb	w0, [x3,w0,uxtw]
	adr	x3, .Lrtx1585
	add	x0, x3, w0, sxtb #2
	br	x0
.Lrtx1585:
	.section	.rodata
	.align	0
	.align	2
.L1585:
	.byte	(.L1584 - .Lrtx1585) / 4
	.byte	(.L1584 - .Lrtx1585) / 4
	.byte	(.L1584 - .Lrtx1585) / 4
	.byte	(.L1584 - .Lrtx1585) / 4
	.byte	(.L1584 - .Lrtx1585) / 4
	.byte	(.L1595 - .Lrtx1585) / 4
	.byte	(.L1595 - .Lrtx1585) / 4
	.byte	(.L1595 - .Lrtx1585) / 4
	.text
	.p2align 3
.L1595:
	lsl	x0, x1, 3
	mov	w4, 3
	sub	x0, x0, x1
	mov	w3, 1
	add	x0, x2, x0, lsl 2
	ldr	w0, [x0, 12]
	cmp	w0, w4
	ble	.L1609
.L1587:
	sub	w0, w0, w3
	sub	w0, w0, #2
	str	w0, [x29, 164]
	b	.L1588
	.p2align 3
.L1584:
	mov	w4, 5
	mov	w3, 3
	b	.L1586
	.p2align 3
.L1597:
	mov	w3, 0
	b	.L1593
	.p2align 3
.L1607:
	mov	w1, 16
	mov	w0, 6
	str	w1, [x29, 164]
	str	w0, [x29, 188]
	b	.L1590
	.p2align 3
.L1606:
	str	wzr, [x29, 164]
	str	w3, [x29, 188]
	b	.L1590
	.p2align 3
.L1594:
	ldr	x1, .LC635
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, .LC637
	mov	w0, 1
	ldr	w2, [x21]
	ldr	w4, [x21, 16]
	ldr	x3, [x21, 8]
	bl	dprint_vfmw
	ldr	w3, [x29, 48]
	mov	w0, 1
	ldr	w2, [x29, 64]
	ldr	x1, .LC639
	add	w2, w3, w2
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1582
	.p2align 3
.L1604:
	ldr	x1, .LC628
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1582
	.p2align 3
.L1608:
	ldr	x1, .LC633
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1582
	.size	VCTRL_CreateChan, .-VCTRL_CreateChan
	.align	3
.LC628:
	.xword	.LC627
	.align	3
.LC629:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC630:
	.xword	CapItem
	.align	3
.LC631:
	.xword	.L1585
	.align	3
.LC633:
	.xword	.LC632
	.align	3
.LC635:
	.xword	.LC634
	.align	3
.LC637:
	.xword	.LC636
	.align	3
.LC639:
	.xword	.LC638
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ConfigChan
	.type	VCTRL_ConfigChan, %function
VCTRL_ConfigChan:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	str	wzr, [x29, 84]
	cmp	w20, 15
	bhi	.L1645
	ldr	x22, .LC642
	ldr	x0, [x22, w20, sxtw 3]
	cbz	x0, .L1646
	ldr	w0, [x0, 12]
	cmp	w0, 1
	bne	.L1647
	mov	x1, x19
	mov	w0, w20
	bl	VCTRL_CheckChanWH
	mov	w21, w0
	cbnz	w0, .L1648
	ldr	x23, .LC647
	add	x24, x23, 152
	mov	x0, x24
	bl	PDT_GetVdecCapability
	ldr	w0, [x23, 172]
	ldr	w2, [x19]
	cmp	w0, 20
	bhi	.L1617
	cmp	w0, w2
	beq	.L1618
	add	x0, x23, 176
	add	x3, x23, 300
	.p2align 2
.L1620:
	ldr	w1, [x0]
	add	x0, x0, 4
	cmp	w1, 20
	bhi	.L1617
	cmp	w1, w2
	beq	.L1618
	cmp	x0, x3
	bne	.L1620
.L1618:
	cmp	w2, 8
	beq	.L1649
.L1621:
	cmp	w2, 9
	beq	.L1623
.L1627:
	mov	x1, x19
	mov	w0, w20
	bl	VCTRL_CmpConfigParameter
	cbz	w0, .L1650
	ldr	x24, .LC652
	mov	x2, 120
	ldr	x0, [x22, w20, sxtw 3]
	mov	x1, x19
	ldr	x3, [x24, 104]
	add	x0, x0, 52
	blr	x3
	ldr	x0, [x22, w20, sxtw 3]
	mov	x2, 72
	ldr	w1, [x19, 16]
	str	w1, [x0, 28]
	ldr	x0, [x22, w20, sxtw 3]
	ldr	w1, [x19]
	ldr	x3, [x24, 96]
	str	w1, [x0, 672]
	ldr	w1, [x19, 104]
	str	w1, [x0, 1520]
	mov	w1, 0
	add	x0, x29, 88
	blr	x3
	ldr	x0, [x22, w20, sxtw 3]
	ldr	w0, [x0, 392]
	str	w0, [x29, 104]
	bl	MEM_Phy2Vir
	str	x0, [x29, 88]
	ldr	x1, [x22, w20, sxtw 3]
	ldr	w2, [x19, 16]
	strb	w2, [x29, 96]
	ldr	w2, [x1, 404]
	mov	w1, 49152
	movk	w1, 0xf8c3, lsl 16
	ldr	w0, [x19]
	stp	w2, w1, [x29, 108]
	ldr	w1, [x19, 92]
	str	w0, [x29, 100]
	cmp	w1, 1
	beq	.L1651
.L1628:
	mov	w1, 0
	cbz	w0, .L1652
.L1629:
	ldr	w0, [x19, 88]
	add	x26, x19, 4
	ldr	x3, [x24, 104]
	mov	x2, 12
	stp	w1, wzr, [x29, 128]
	str	w0, [x29, 136]
	mov	x1, x26
	add	x0, x29, 116
	blr	x3
	add	x1, x29, 88
	mov	w0, w20
	bl	SM_Config
	ldr	x3, [x24, 96]
	mov	x2, 1
	mov	w1, 0
	add	x0, x29, 80
	blr	x3
	ldr	w2, [x19, 92]
	cmp	w2, 1
	beq	.L1653
.L1630:
	ldr	x0, [x22, w20, sxtw 3]
	mov	w25, 1
	mov	x1, x26
	ldr	w2, [x0, 344]
	str	w2, [x0, 1264]
	ldr	w2, [x0, 352]
	str	w2, [x0, 1268]
	ldr	w2, [x0, 376]
	str	w2, [x0, 700]
	ldr	w2, [x0, 380]
	str	w2, [x0, 704]
	ldr	w2, [x0, 408]
	str	w2, [x0, 712]
	ldr	w2, [x0, 412]
	str	w2, [x0, 716]
	ldr	w2, [x0, 416]
	str	w2, [x0, 720]
	ldr	w2, [x0, 420]
	str	w2, [x0, 724]
	ldr	w2, [x0, 424]
	str	w2, [x0, 728]
	ldr	w2, [x0, 428]
	str	w2, [x0, 732]
	ldr	w2, [x19, 20]
	str	w2, [x0, 684]
	ldr	w2, [x19, 20]
	str	w2, [x0, 680]
	ldr	w2, [x19, 28]
	str	w2, [x0, 688]
	ldr	w2, [x19, 32]
	str	w2, [x0, 692]
	ldr	w2, [x19, 36]
	str	w2, [x0, 696]
	ldr	w2, [x19, 60]
	str	w2, [x0, 1272]
	ldr	w2, [x19, 68]
	str	w2, [x0, 1276]
	str	w25, [x0, 1308]
	str	w25, [x0, 1312]
	ldr	x3, [x24, 104]
	ldr	w2, [x19, 80]
	cmp	w2, w25
	cset	w2, eq
	str	w2, [x0, 1292]
	ldr	w2, [x19, 84]
	str	w2, [x0, 1296]
	ldr	w2, [x19, 52]
	str	w2, [x0, 1300]
	ldr	w2, [x29, 132]
	str	w2, [x0, 1304]
	add	x2, x0, 294912
	str	wzr, [x0, 1552]
	add	x0, x0, 1236
	str	wzr, [x0, 336]
	ldrsb	w4, [x19, 117]
	strb	w4, [x2, 2536]
	mov	x2, 12
	blr	x3
	ldr	x0, [x22, w20, sxtw 3]
	add	x1, x0, 294912
	ldr	w2, [x0, 672]
	ldr	w0, [x1, 2576]
	cmp	w0, w2
	beq	.L1631
	mov	w0, w20
	bl	VCTRL_ResetChan
	ldr	w1, [x19]
	ldr	x0, [x22, w20, sxtw 3]
	cmp	w1, 20
	beq	.L1632
	ldr	w2, [x0, 672]
	add	x1, x0, 294912
.L1631:
	ldr	x4, .LC655
	mov	x0, 0
	str	w2, [x1, 2576]
	b	.L1636
	.p2align 3
.L1655:
	ldr	w3, [x1, 28]
	str	w3, [x2, x0, lsl 2]
	ldr	w1, [x1, 28]
	str	w1, [x4, x0, lsl 2]
	add	x0, x0, 1
	cmp	x0, 16
	beq	.L1654
.L1636:
	ldr	x1, [x22, x0, lsl 3]
	add	x2, x23, 304
	cbnz	x1, .L1655
	add	x1, x23, 304
	str	wzr, [x4, x0, lsl 2]
	str	wzr, [x1, x0, lsl 2]
	add	x0, x0, 1
	cmp	x0, 16
	bne	.L1636
.L1654:
	add	x0, x23, 304
	bl	VDMDRV_SetPriority
	mov	w0, w20
	bl	VDMDRV_ClearChanIsDec
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 160
	ret
	.p2align 3
.L1650:
	ldr	x1, .LC651
	mov	w21, 0
	bl	dprint_vfmw
.L1612:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 160
	ret
	.p2align 3
.L1617:
	ldr	x1, .LC649
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 160
	ret
	.p2align 3
.L1652:
	ldr	w1, [x19, 52]
	b	.L1629
.L1623:
	add	x0, x29, 84
	bl	REAL9DEC_Support
	ldr	w0, [x29, 84]
	cbnz	w0, .L1627
.L1626:
	mov	w21, -1
	b	.L1612
	.p2align 3
.L1649:
	add	x0, x29, 84
	bl	REAL8DEC_Support
	ldr	w0, [x29, 84]
	cbz	w0, .L1626
	ldr	w2, [x19]
	b	.L1621
	.p2align 3
.L1651:
	ldr	w2, [x19, 96]
	stp	w1, w2, [x29, 140]
	ldr	w1, [x19, 100]
	str	w1, [x29, 148]
	b	.L1628
	.p2align 3
.L1647:
	ldr	x3, .LC644
	mov	w2, 3250
	ldr	x1, .LC641
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L1612
.L1653:
	add	x1, x29, 80
	mov	w0, w20
	strb	w2, [x29, 80]
	bl	FSP_ConfigAttribute
	b	.L1630
.L1648:
	ldr	x1, .LC646
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L1612
.L1632:
	ldr	w3, [x0, 396]
	ldr	w4, [x0, 400]
	ldr	w1, [x0, 1348]
	ldr	w2, [x0, 1352]
	mov	w0, w20
	bl	VCTRL_PartitionMemForStdUser
	cbnz	w0, .L1633
	ldr	x0, [x22, w20, sxtw 3]
	add	x1, x0, 294912
	ldr	w2, [x0, 672]
	b	.L1631
.L1645:
	ldr	x3, .LC640
	mov	w2, 3248
	ldr	x1, .LC641
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L1612
.L1646:
	ldr	x3, .LC643
	mov	w2, 3249
	ldr	x1, .LC641
	mov	w21, -1
	bl	dprint_vfmw
	b	.L1612
.L1633:
	ldr	x1, .LC654
	mov	w0, w25
	mov	w21, -1
	bl	dprint_vfmw
	b	.L1612
	.size	VCTRL_ConfigChan, .-VCTRL_ConfigChan
	.align	3
.LC640:
	.xword	.LC51
	.align	3
.LC641:
	.xword	.LC2
	.align	3
.LC642:
	.xword	.LANCHOR0
	.align	3
.LC643:
	.xword	.LC5
	.align	3
.LC644:
	.xword	.LC56
	.align	3
.LC646:
	.xword	.LC645
	.align	3
.LC647:
	.xword	.LANCHOR5
	.align	3
.LC649:
	.xword	.LC648
	.align	3
.LC651:
	.xword	.LC650
	.align	3
.LC652:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC654:
	.xword	.LC653
	.align	3
.LC655:
	.xword	g_ChanCtx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetDbgOption
	.type	VCTRL_SetDbgOption, %function
VCTRL_SetDbgOption:
	stp	x29, x30, [sp, -208]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	w19, w0
	mov	x21, x1
	mov	x0, x30
	bl	_mcount
	cbz	x21, .L1749
	cmp	w19, 544
	ldr	w20, [x21]
	beq	.L1658
	bls	.L1774
	cmp	w19, 1280
	beq	.L1681
	bhi	.L1682
	cmp	w19, 897
	beq	.L1683
	bls	.L1775
	cmp	w19, 1024
	beq	.L1690
	bhi	.L1691
	cmp	w19, 898
	beq	.L1772
	cmp	w19, 899
	bne	.L1749
	mov	w0, w20
	bl	VCTRL_StopChan
	mov	w3, w0
	ldr	x1, .LC737
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, w20
	bl	VCTRL_ResetChan
	mov	w3, w0
	ldr	x1, .LC739
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
.L1772:
	mov	w0, w20
	bl	VCTRL_StartChan
	mov	w3, w0
	ldr	x1, .LC741
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	.p2align 2
.L1773:
	mov	w0, 1
.L1657:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L1682:
	cmp	w19, 1537
	beq	.L1696
	bls	.L1776
	cmp	w19, 1540
	beq	.L1702
	bhi	.L1703
	cmp	w19, 1538
	beq	.L1704
	cmp	w19, 1539
	bne	.L1749
	ldr	x2, .LC767
	ubfiz	x20, x20, 4, 32
	mov	w1, 1
	mov	w0, w1
	ldr	x2, [x2, x20]
	str	w1, [x2, 48]
	b	.L1657
	.p2align 3
.L1774:
	cmp	w19, 11
	beq	.L1660
	bls	.L1777
	cmp	w19, 512
	beq	.L1669
	bls	.L1778
	cmp	w19, 515
	beq	.L1675
	bhi	.L1676
	cmp	w19, 513
	beq	.L1677
	cmp	w19, 514
	bne	.L1749
	cmp	w20, 15
	bhi	.L1773
	ldr	x19, .LC676
	add	x21, x19, 648
	ldr	x0, [x21, w20, uxtw 3]
	cbz	x0, .L1779
	ldr	x1, .LC707
	mov	w0, 32
	add	x22, x19, 912
	bl	dprint_vfmw
	ldr	x23, .LC662
	ldr	x0, [x21, w20, uxtw 3]
	ldr	x1, [x23, 72]
	blr	x1
	str	xzr, [x21, w20, uxtw 3]
	ldr	x1, .LC687
	mov	w2, -1
	ldr	w0, [x19, 928]
	str	xzr, [x19, 944]
	str	w2, [x1, 72]
	str	xzr, [x19, 952]
	str	xzr, [x19, 960]
	str	wzr, [x19, 968]
	cbz	w0, .L1773
	ldr	x1, [x19, 920]
	bl	MEM_ReleaseMemBlock
	ldr	w2, [x19, 936]
	ldr	x1, [x19, 920]
	ldr	w0, [x19, 928]
	bl	MEM_DelMemRecord
	ldr	x3, [x23, 96]
	mov	x2, 32
	mov	w1, 0
	mov	x0, x22
	blr	x3
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1778:
	cmp	w19, 13
	beq	.L1671
	bcc	.L1672
	cmp	w19, 256
	beq	.L1673
	cmp	w19, 257
	bne	.L1749
	ldr	x19, .LC659
	mov	w0, w20
	mov	w2, 67108864
	mov	w1, 1
	ldr	w20, [x19]
	str	w2, [x19]
	bl	PrintScdRawState
	mov	w0, 1
	str	w20, [x19]
	b	.L1657
	.p2align 3
.L1775:
	cmp	w19, 768
	beq	.L1685
	bls	.L1780
	cmp	w19, 769
	beq	.L1688
	cmp	w19, 896
	bne	.L1749
	mov	w0, w20
	bl	VCTRL_StopChan
	mov	w3, w0
	ldr	x1, .LC737
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1776:
	cmp	w19, 1282
	beq	.L1698
	bcc	.L1699
	cmp	w19, 1283
	beq	.L1700
	cmp	w19, 1536
	bne	.L1749
	cmp	w20, 15
	bhi	.L1746
	ldr	x3, .LC763
	mov	w19, 1
	ldr	x1, .LC765
	mov	w2, w20
	mov	w0, 32
	ldr	x3, [x3, w20, uxtw 3]
	str	w19, [x3, 1296]
	bl	dprint_vfmw
	mov	w0, w19
	b	.L1657
	.p2align 3
.L1777:
	cmp	w19, 4
	beq	.L1662
	bhi	.L1663
	cmp	w19, 1
	beq	.L1773
	cbz	w19, .L1665
	cmp	w19, 2
	bne	.L1781
	lsr	w19, w20, 24
	ldr	x1, .LC661
	and	w20, w20, 16777215
	mov	w2, w19
	mov	w3, w20
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x3, .LC662
	mov	w1, 0
	mov	x2, 120
	add	x0, x29, 88
	ldr	x3, [x3, 96]
	blr	x3
	add	x1, x29, 88
	mov	w0, w19
	bl	VCTRL_GetChanCfg
	cbnz	w0, .L1773
	add	x1, x29, 88
	mov	w0, w19
	str	w20, [x29, 108]
	bl	VCTRL_ConfigChan
	b	.L1773
	.p2align 3
.L1663:
	cmp	w19, 6
	beq	.L1773
	bcc	.L1667
	cmp	w19, 7
	bne	.L1782
	lsr	w19, w20, 24
	ldr	x1, .LC670
	and	w20, w20, 16777215
	mov	w2, w19
	mov	w3, w20
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x3, .LC662
	mov	w1, 0
	mov	x2, 120
	add	x0, x29, 88
	ldr	x3, [x3, 96]
	blr	x3
	add	x1, x29, 88
	mov	w0, w19
	bl	VCTRL_GetChanCfg
	cbnz	w0, .L1773
	add	x1, x29, 88
	mov	w0, w19
	str	w20, [x29, 112]
	bl	VCTRL_ConfigChan
	b	.L1773
	.p2align 3
.L1691:
	cmp	w19, 1025
	beq	.L1694
	cmp	w19, 1026
	bne	.L1749
	ldr	x3, .LC742
	mov	w2, w20
	ldr	x1, .LC748
	mov	w0, 32
	str	w20, [x3, 3644]
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1676:
	cmp	w19, 516
	beq	.L1679
	cmp	w19, 528
	bne	.L1749
	cmp	w20, 15
	bhi	.L1773
	ldr	x21, .LC676
	add	x19, x21, 976
	ldr	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1783
.L1739:
	ldr	x1, .LC723
	b	.L1767
	.p2align 3
.L1703:
	cmp	w19, 1541
	beq	.L1706
	cmp	w19, 1792
	bne	.L1749
	cbnz	w20, .L1773
	ldr	x20, .LC687
	ldr	w19, [x20, 76]
	cmp	w19, 1
	beq	.L1784
	ldr	x1, .LC773
	mov	w0, 32
	bl	dprint_vfmw
	mov	w1, 1
	str	w1, [x20, 76]
	mov	w0, w1
	b	.L1657
	.p2align 3
.L1694:
	ldr	x3, .LC676
	mov	w2, w20
	ldr	x1, .LC746
	mov	w0, 32
	str	w20, [x3, 1768]
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1704:
	ldr	x3, .LC766
	ldr	x1, .LC765
.L1766:
	mov	w2, w20
	str	w20, [x3]
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	w0, 1
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L1677:
	cmp	w20, 15
	bhi	.L1773
	ldr	x21, .LC676
	add	x19, x21, 384
	ldr	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1785
	ldr	x1, .LC699
.L1767:
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x1, .LC662
	ldr	x0, [x19, w20, uxtw 3]
	ldr	x1, [x1, 72]
	blr	x1
	str	xzr, [x19, w20, uxtw 3]
	b	.L1773
	.p2align 3
.L1700:
	ldr	x3, .LC758
	ldr	x1, .LC760
	b	.L1766
	.p2align 3
.L1673:
	ldr	x1, .LC686
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, w20
	bl	DBG_PrintTracer
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1688:
	bl	VCTRL_Resume
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1665:
	ldr	x1, .LC659
	mov	w0, 1
	str	w20, [x1]
	b	.L1657
	.p2align 3
.L1706:
	ldr	x3, .LC676
	mov	w2, w20
	ldr	x1, .LC769
	mov	w0, 32
	str	w20, [x3, 1772]
	bl	dprint_vfmw
	b	.L1773
	.p2align 3
.L1667:
	lsr	w19, w20, 24
	ldr	x1, .LC666
	and	w20, w20, 7
	mov	w2, w19
	mov	w3, w20
	mov	w0, 32
	add	x21, x29, 88
	bl	dprint_vfmw
	ldr	x3, .LC662
	mov	w1, 0
	mov	x2, 120
	mov	x0, x21
	ldr	x3, [x3, 96]
	blr	x3
	mov	x1, x21
	mov	w0, w19
	bl	VCTRL_GetChanCfg
	cbnz	w0, .L1710
	mov	x1, x21
	mov	w0, w19
	str	w20, [x29, 116]
	bl	VCTRL_ConfigChan
	b	.L1773
	.p2align 3
.L1679:
	ldr	x19, .LC662
	mov	w0, w20
	ldp	w23, w24, [x21, 4]
	ldr	x1, [x19, 384]
	blr	x1
	mov	x22, x0
	cbz	x0, .L1786
	mov	w0, 19
	ldr	x25, [x19, 136]
	add	x21, x29, 88
	bl	VFMW_GetString
	mov	x2, x0
	ldr	x3, .LC687
	mov	w4, w20
	mov	x1, 50
	mov	x0, x21
	add	x3, x3, 8
	blr	x25
	ldr	x3, [x19, 64]
	mov	w2, 0
	mov	w1, 66
	mov	x0, x21
	blr	x3
	mov	x20, x0
	cbz	x0, .L1733
	ldr	x1, .LC714
	ldr	x5, [x1, 304]
	cbz	x5, .L1734
	mov	w4, 1
	mov	w3, w24
	mov	w2, w23
	mov	x1, x22
	blr	x5
.L1735:
	ldr	x1, [x19, 72]
	mov	x0, x20
	blr	x1
	ldr	x1, .LC717
	mov	x2, x21
	mov	w0, 32
	bl	dprint_vfmw
.L1732:
	ldr	x1, [x19, 304]
	mov	x0, x22
	blr	x1
	b	.L1773
.L1783:
	ldr	x22, .LC662
	mov	w0, 20
	ldr	x23, [x22, 136]
	bl	VFMW_GetString
	ldr	w1, [x21, 1232]
	mov	x2, x0
	ldr	x3, .LC687
	add	x0, x29, 88
	mov	w4, w1
	add	w1, w1, 1
	add	x3, x3, 8
	str	w1, [x21, 1232]
	mov	x1, 50
	blr	x23
	ldr	x3, [x22, 64]
	mov	w2, 0
	mov	w1, 66
	add	x0, x29, 88
	blr	x3
	str	w20, [x21, 1236]
	str	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1737
	ldr	x1, .LC719
	mov	w3, w20
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
.L1781:
	cmp	w19, 3
	beq	.L1773
	.p2align 2
.L1749:
	mov	w0, -2
	b	.L1657
	.p2align 3
.L1780:
	cmp	w19, 560
	bne	.L1749
	cmp	w20, 15
	bhi	.L1773
	ldr	x21, .LC676
	add	x19, x21, 1504
	ldr	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1787
	ldr	x1, .LC735
	b	.L1767
	.p2align 3
.L1685:
	bl	VCTRL_Suspend
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1662:
	lsr	w19, w20, 24
	ldr	x1, .LC664
	and	w20, w20, 3
	mov	w2, w19
	mov	w3, w20
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x3, .LC662
	mov	w1, 0
	mov	x2, 120
	add	x0, x29, 88
	ldr	x3, [x3, 96]
	blr	x3
	add	x1, x29, 88
	mov	w0, w19
	bl	VCTRL_GetChanCfg
	cbnz	w0, .L1773
	add	x1, x29, 88
	mov	w0, w19
	str	w20, [x29, 120]
	bl	VCTRL_ConfigChan
	b	.L1773
	.p2align 3
.L1675:
	ldr	x19, .LC662
	mov	w0, w20
	ldr	x1, [x19, 384]
	blr	x1
	mov	x22, x0
	cbz	x0, .L1788
	mov	w0, 18
	ldr	x23, [x19, 136]
	add	x21, x29, 88
	bl	VFMW_GetString
	mov	x2, x0
	ldr	x3, .LC687
	mov	w4, w20
	mov	x1, 50
	mov	x0, x21
	add	x3, x3, 8
	blr	x23
	ldr	x3, [x19, 64]
	mov	w2, 0
	mov	w1, 66
	mov	x0, x21
	blr	x3
	mov	x20, x0
	cbz	x0, .L1730
	ldr	x3, [x19, 88]
	mov	x2, x0
	mov	w1, 3145728
	mov	x0, x22
	blr	x3
	ldr	x1, [x19, 72]
	mov	x0, x20
	blr	x1
	ldr	x1, .LC711
	mov	x2, x21
	mov	w0, 32
	bl	dprint_vfmw
.L1729:
	ldr	x1, [x19, 304]
	mov	x0, x22
	blr	x1
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1746:
	ldr	x1, .LC762
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1702:
	ldr	x1, .LC767
	ubfiz	x20, x20, 4, 32
	mov	w0, 1
	ldr	x1, [x1, x20]
	str	wzr, [x1, 48]
	b	.L1657
	.p2align 3
.L1690:
	ldr	x3, .LC742
	mov	w2, w20
	ldr	x1, .LC744
	mov	w0, 32
	str	w20, [x3, 3640]
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1696:
	cmp	w20, 15
	bhi	.L1746
	ldr	x3, .LC763
	mov	w2, w20
	ldr	x1, .LC765
	mov	w0, 32
	ldr	x3, [x3, w20, uxtw 3]
	str	wzr, [x3, 1296]
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1683:
	mov	w0, w20
	bl	VCTRL_ResetChan
	mov	w3, w0
	ldr	x1, .LC739
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1660:
	cmp	w20, 1
	bls	.L1789
	ldr	x1, .LC675
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1669:
	cmp	w20, 15
	bhi	.L1773
	ldr	x21, .LC676
	sub	x19, x21, #176
	ldr	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1790
	ldr	x1, .LC693
	b	.L1767
	.p2align 3
.L1681:
	ldr	x3, .LC749
	ldr	x1, .LC751
	b	.L1766
	.p2align 3
.L1699:
	ldr	x3, .LC752
	ldr	x1, .LC754
	b	.L1766
	.p2align 3
.L1698:
	ldr	x3, .LC755
	ldr	x1, .LC757
	b	.L1766
	.p2align 3
.L1672:
	cmp	w20, 1
	bls	.L1791
	ldr	x1, .LC680
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1657
	.p2align 3
.L1671:
	cmp	w20, 16
	bls	.L1715
	ldr	x1, .LC682
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
	.p2align 3
.L1658:
	cmp	w20, 15
	bhi	.L1773
	ldr	x21, .LC676
	add	x19, x21, 1240
	ldr	x0, [x19, w20, uxtw 3]
	cbnz	x0, .L1739
	ldr	x22, .LC662
	mov	w0, 21
	ldr	x23, [x22, 136]
	bl	VFMW_GetString
	ldr	w1, [x21, 1496]
	mov	x2, x0
	ldr	x3, .LC687
	add	x0, x29, 88
	mov	w4, w1
	add	w1, w1, 1
	add	x3, x3, 8
	str	w1, [x21, 1496]
	mov	x1, 50
	blr	x23
	ldr	x3, [x22, 64]
	mov	w2, 0
	mov	w1, 66
	add	x0, x29, 88
	blr	x3
	str	w20, [x21, 1500]
	str	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1740
	ldr	x1, .LC725
	mov	w3, w20
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
	.p2align 3
.L1710:
	ldr	x1, .LC668
	mov	w2, w19
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
	.p2align 3
.L1715:
	ldr	x3, .LC676
	mov	w2, w20
	ldr	x1, .LC684
	mov	w0, 32
	str	w20, [x3, 368]
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1657
.L1782:
	cmp	w19, 8
	beq	.L1773
	mov	w0, -2
	b	.L1657
.L1789:
	ldr	x3, .LC671
	ldr	x1, .LC673
	b	.L1766
.L1791:
	ldr	x3, .LC676
	mov	w2, w20
	ldr	x1, .LC678
	mov	w0, 32
	str	w20, [x3, -184]
	bl	dprint_vfmw
	b	.L1773
.L1784:
	ldr	x1, .LC771
	mov	w0, 32
	bl	dprint_vfmw
	str	wzr, [x20, 76]
	mov	w0, w19
	b	.L1657
.L1786:
	ldr	x1, .LC709
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1732
.L1788:
	ldr	x1, .LC709
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1729
.L1790:
	ldr	x22, .LC662
	mov	w0, 15
	ldr	x23, [x22, 136]
	bl	VFMW_GetString
	ldr	w1, [x21, 372]
	mov	x2, x0
	ldr	x3, .LC687
	add	x0, x29, 88
	mov	w4, w1
	add	w1, w1, 1
	add	x3, x3, 8
	str	w1, [x21, 372]
	mov	x1, 50
	blr	x23
	ldr	x3, [x22, 64]
	mov	w2, 0
	mov	w1, 66
	add	x0, x29, 88
	blr	x3
	str	w20, [x21, 376]
	str	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1718
	ldr	x1, .LC689
	mov	w3, w20
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
.L1787:
	ldr	x22, .LC662
	mov	x1, 50
	ldr	w0, [x21, 1760]
	ldr	x3, .LC687
	mov	w4, w0
	ldr	x2, .LC729
	add	w0, w0, 1
	ldr	x5, [x22, 136]
	add	x3, x3, 8
	str	w0, [x21, 1760]
	add	x0, x29, 88
	blr	x5
	ldr	x3, [x22, 64]
	mov	w2, 0
	mov	w1, 66
	add	x0, x29, 88
	blr	x3
	str	w20, [x21, 1764]
	str	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1743
	ldr	x1, .LC731
	mov	w3, w20
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
.L1785:
	ldr	x22, .LC662
	mov	w0, 16
	ldr	x23, [x22, 136]
	bl	VFMW_GetString
	ldr	w1, [x21, 640]
	mov	x2, x0
	ldr	x3, .LC687
	add	x0, x29, 88
	mov	w4, w1
	add	w1, w1, 1
	add	x3, x3, 8
	str	w1, [x21, 640]
	mov	x1, 50
	blr	x23
	ldr	x3, [x22, 64]
	mov	w2, 0
	mov	w1, 66
	add	x0, x29, 88
	blr	x3
	str	w20, [x21, 644]
	str	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1722
	ldr	x1, .LC695
	mov	w3, w20
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
.L1730:
	ldr	x1, .LC713
	mov	x2, x21
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1729
.L1734:
	ldr	x1, .LC715
	mov	w2, 9005
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1735
.L1779:
	ldr	x22, .LC662
	mov	w0, 17
	ldr	x23, [x22, 136]
	bl	VFMW_GetString
	ldr	w1, [x19, 904]
	mov	x2, x0
	ldr	x24, .LC687
	add	x0, x29, 88
	mov	w4, w1
	add	w1, w1, 1
	add	x3, x24, 8
	str	w1, [x19, 904]
	mov	x1, 100
	blr	x23
	ldr	x3, [x22, 64]
	mov	w2, 0
	mov	w1, 578
	add	x0, x29, 88
	blr	x3
	str	x0, [x21, w20, uxtw 3]
	cbz	x0, .L1725
	ldr	x3, [x22, 96]
	add	x23, x19, 912
	mov	x2, 32
	mov	w1, 0
	mov	x0, x23
	blr	x3
	mov	w0, 24
	bl	VFMW_GetString
	mov	w4, 0
	mov	x3, x23
	mov	w2, 14155776
	mov	w1, 0
	bl	MEM_AllocMemBlock
	cbz	w0, .L1726
	ldr	x1, .LC701
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, [x21, w20, uxtw 3]
	ldr	x1, [x22, 72]
	blr	x1
	str	xzr, [x21, w20, uxtw 3]
	mov	w0, -1
	str	xzr, [x19, 944]
	str	xzr, [x19, 952]
	str	xzr, [x19, 960]
	str	wzr, [x19, 968]
	str	w0, [x24, 72]
	b	.L1773
.L1733:
	ldr	x1, .LC713
	mov	x2, x21
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1732
.L1726:
	ldr	w2, [x23, 24]
	ldr	x1, [x23, 8]
	ldr	w0, [x23, 16]
	bl	MEM_AddMemRecord
	ldr	x5, [x23, 8]
	lsr	w6, w20, 24
	ldr	x1, .LC703
	and	w20, w20, 3
	mov	w3, w6
	add	x7, x5, 9437184
	str	w6, [x24, 72]
	mov	w4, w20
	add	x6, x5, 11796480
	add	x2, x29, 88
	mov	w0, 32
	str	x7, [x19, 952]
	str	x5, [x19, 960]
	str	w20, [x19, 968]
	str	x6, [x19, 944]
	bl	dprint_vfmw
	b	.L1773
.L1737:
	ldr	x1, .LC721
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
.L1722:
	ldr	x1, .LC697
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
.L1740:
	ldr	x1, .LC727
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
.L1718:
	ldr	x1, .LC691
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
.L1743:
	ldr	x1, .LC733
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
.L1725:
	ldr	x1, .LC705
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1773
	.size	VCTRL_SetDbgOption, .-VCTRL_SetDbgOption
	.align	3
.LC659:
	.xword	g_PrintEnable
	.align	3
.LC661:
	.xword	.LC660
	.align	3
.LC662:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC664:
	.xword	.LC663
	.align	3
.LC666:
	.xword	.LC665
	.align	3
.LC668:
	.xword	.LC667
	.align	3
.LC670:
	.xword	.LC669
	.align	3
.LC671:
	.xword	USE_FF_APT_EN
	.align	3
.LC673:
	.xword	.LC672
	.align	3
.LC675:
	.xword	.LC674
	.align	3
.LC676:
	.xword	.LANCHOR5
	.align	3
.LC678:
	.xword	.LC677
	.align	3
.LC680:
	.xword	.LC679
	.align	3
.LC682:
	.xword	.LC681
	.align	3
.LC684:
	.xword	.LC683
	.align	3
.LC686:
	.xword	.LC685
	.align	3
.LC687:
	.xword	.LANCHOR4
	.align	3
.LC689:
	.xword	.LC688
	.align	3
.LC691:
	.xword	.LC690
	.align	3
.LC693:
	.xword	.LC692
	.align	3
.LC695:
	.xword	.LC694
	.align	3
.LC697:
	.xword	.LC696
	.align	3
.LC699:
	.xword	.LC698
	.align	3
.LC701:
	.xword	.LC700
	.align	3
.LC703:
	.xword	.LC702
	.align	3
.LC705:
	.xword	.LC704
	.align	3
.LC707:
	.xword	.LC706
	.align	3
.LC709:
	.xword	.LC708
	.align	3
.LC711:
	.xword	.LC710
	.align	3
.LC713:
	.xword	.LC712
	.align	3
.LC714:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC715:
	.xword	.LC22
	.align	3
.LC717:
	.xword	.LC716
	.align	3
.LC719:
	.xword	.LC718
	.align	3
.LC721:
	.xword	.LC720
	.align	3
.LC723:
	.xword	.LC722
	.align	3
.LC725:
	.xword	.LC724
	.align	3
.LC727:
	.xword	.LC726
	.align	3
.LC729:
	.xword	.LC728
	.align	3
.LC731:
	.xword	.LC730
	.align	3
.LC733:
	.xword	.LC732
	.align	3
.LC735:
	.xword	.LC734
	.align	3
.LC737:
	.xword	.LC736
	.align	3
.LC739:
	.xword	.LC738
	.align	3
.LC741:
	.xword	.LC740
	.align	3
.LC742:
	.xword	.LANCHOR3
	.align	3
.LC744:
	.xword	.LC743
	.align	3
.LC746:
	.xword	.LC745
	.align	3
.LC748:
	.xword	.LC747
	.align	3
.LC749:
	.xword	g_TraceCtrl
	.align	3
.LC751:
	.xword	.LC750
	.align	3
.LC752:
	.xword	g_TraceBsPeriod
	.align	3
.LC754:
	.xword	.LC753
	.align	3
.LC755:
	.xword	g_TraceFramePeriod
	.align	3
.LC757:
	.xword	.LC756
	.align	3
.LC758:
	.xword	g_TraceImgPeriod
	.align	3
.LC760:
	.xword	.LC759
	.align	3
.LC762:
	.xword	.LC761
	.align	3
.LC763:
	.xword	.LANCHOR0
	.align	3
.LC765:
	.xword	.LC764
	.align	3
.LC766:
	.xword	g_TunnelLineNumber
	.align	3
.LC767:
	.xword	s_SmIIS
	.align	3
.LC769:
	.xword	.LC768
	.align	3
.LC771:
	.xword	.LC770
	.align	3
.LC773:
	.xword	.LC772
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_ResetChanWithOption
	.type	VCTRL_ResetChanWithOption, %function
VCTRL_ResetChanWithOption:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	sxtw	x21, w0
	mov	x0, x30
	mov	x22, x1
	mov	x19, x21
	bl	_mcount
	ldr	x20, .LC781
	ldr	x1, [x20, w21, sxtw 3]
	cbz	x1, .L1819
	ldr	w0, [x22]
	cmp	w0, 1
	bhi	.L1820
	mov	w0, w21
	ldr	w23, [x1, 16]
	bl	VCTRL_StopChan
	ldr	x24, .LC786
	ldr	x0, .LC787
	lsl	x2, x21, 4
	ldr	x1, [x20, w21, sxtw 3]
	add	x21, x2, x21, lsl 2
	add	x0, x0, 3648
	ldr	x3, [x24, 96]
	add	x0, x0, x21, lsl 4
	mov	x2, 300
	str	wzr, [x1, 452]
	sub	x0, x0, x21
	str	wzr, [x1, 456]
	mov	w1, 0
	blr	x3
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1821
	add	x0, x0, 290816
	str	wzr, [x0, 2484]
.L1797:
	ldr	x1, [x24, 24]
	mov	w0, 4
	blr	x1
	mov	w0, w19
	bl	VCTRL_FindOccupiedFrame
	ldr	x1, [x24, 32]
	mov	w0, 4
	blr	x1
	ldr	x1, [x20, w19, sxtw 3]
	mov	w0, w19
	ldr	w2, [x22, 4]
	str	w2, [x1, 1288]
	bl	InitSyntaxCtx
	ldr	x0, [x20, w19, sxtw 3]
	add	x1, x0, 294912
	ldr	w0, [x0, 672]
	add	x1, x1, 2584
	bl	ReleaseCurPic
	ldr	x0, [x20, w19, sxtw 3]
	mov	x2, 416
	ldr	x3, [x24, 96]
	mov	w1, 0
	add	x0, x0, 816
	blr	x3
	ldr	x0, [x20, w19, sxtw 3]
	mov	x1, -1
	ldr	w2, [x0, 1512]
	str	x1, [x0, 736]
	cmp	w2, 1
	str	x1, [x0, 744]
	str	wzr, [x0, 1232]
	beq	.L1822
.L1798:
	ldr	w0, [x0, 672]
	cmp	w0, 20
	beq	.L1799
	ldr	w0, [x22, 8]
	cbz	w0, .L1823
.L1799:
	mov	w0, w19
	bl	FSP_EmptyInstance
.L1800:
	mov	w0, w19
	bl	InitUsdPool
	ldr	w0, [x22]
	cbz	w0, .L1824
.L1801:
	ldr	x0, .LC788
	ldr	w1, [x0, -184]
	cbz	w1, .L1802
	sub	x20, x0, #176
	str	w19, [x29, 76]
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1804
	mov	w0, 512
	add	x1, x29, 76
	bl	VCTRL_SetDbgOption
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1804
	add	x1, x29, 76
	mov	w0, 512
	bl	VCTRL_SetDbgOption
.L1804:
	ldr	x1, .LC789
	mov	w0, 32
	bl	dprint_vfmw
.L1802:
	cbnz	w23, .L1825
.L1806:
	mov	w0, w19
	bl	VDMDRV_ClearChanIsDec
	mov	w0, 0
.L1794:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1824:
	mov	w0, w19
	bl	SM_Reset
	b	.L1801
	.p2align 3
.L1825:
	mov	w0, w19
	bl	VCTRL_StartChan
	b	.L1806
	.p2align 3
.L1822:
	str	wzr, [x0, 1552]
	b	.L1798
	.p2align 3
.L1823:
	mov	w0, w19
	bl	FSP_ResetInstance
	b	.L1800
	.p2align 3
.L1820:
	ldr	x1, .LC785
	mov	w2, 4051
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1821:
	bl	VCTRL_SetSendStreamAgainFlag.part.2
	b	.L1797
.L1819:
	ldr	x3, .LC782
	mov	w2, 4047
	ldr	x1, .LC783
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1794
	.size	VCTRL_ResetChanWithOption, .-VCTRL_ResetChanWithOption
	.align	3
.LC781:
	.xword	.LANCHOR0
	.align	3
.LC782:
	.xword	.LC5
	.align	3
.LC783:
	.xword	.LC2
	.align	3
.LC785:
	.xword	.LC784
	.align	3
.LC786:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC787:
	.xword	.LANCHOR3
	.align	3
.LC788:
	.xword	.LANCHOR5
	.align	3
.LC789:
	.xword	.LC598
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_DestroyChanWithOption
	.type	VCTRL_DestroyChanWithOption, %function
VCTRL_DestroyChanWithOption:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC792
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	cmp	w19, 15
	bhi	.L1837
	ldr	x21, .LC796
	ldr	x0, [x21, w19, sxtw 3]
	cbz	x0, .L1838
	ldr	w22, [x0, 12]
	cmp	w22, 1
	bne	.L1839
	str	wzr, [x0, 16]
	mov	w0, 0
	ldr	x1, [x20, 32]
	blr	x1
	mov	w0, w19
	bl	SCDDRV_IsChanDec
	cbnz	w0, .L1840
.L1831:
	ldr	x1, [x20, 24]
	mov	w0, 0
	sxtw	x22, w19
	blr	x1
	ldr	x2, .LC801
	mov	w0, 4
	ldr	x1, [x20, 24]
	str	wzr, [x2, w19, sxtw 2]
	blr	x1
	mov	w0, w19
	bl	VCTRL_StopChan
	ldr	x1, [x20, 32]
	cbnz	w0, .L1841
	mov	w0, 4
	lsl	x22, x22, 4
	blr	x1
	mov	w0, w19
	bl	VCTRL_ResetChan
	bl	VDMDRV_ParamInit
	ldr	x1, .LC802
	mov	w0, 4
	ldr	x2, [x20, 24]
	add	x1, x1, 3384
	str	wzr, [x1, w19, sxtw 2]
	blr	x2
	mov	w0, w19
	bl	FSP_DisableInstance
	mov	w0, w19
	bl	CloseUsdPool_WithMem
	ldr	x1, [x21, w19, sxtw 3]
	mov	w0, w19
	ldr	x4, .LC803
	add	x2, x1, 294912
	ldr	x3, .LC804
	ldr	w5, [x1, 368]
	str	w5, [x29, 116]
	ldr	w2, [x2, 2540]
	ldr	w5, [x1, 360]
	str	w2, [x29, 104]
	add	x2, x1, 264
	str	w5, [x29, 108]
	str	x2, [x29, 72]
	add	x2, x1, 290816
	ldr	w5, [x1, 364]
	add	x2, x2, 2488
	str	xzr, [x4, x22]
	str	w5, [x29, 112]
	add	x5, x1, 296
	add	x1, x1, 328
	str	xzr, [x3, w19, sxtw 3]
	stp	x5, x1, [x29, 80]
	str	x2, [x29, 96]
	str	xzr, [x21, w19, sxtw 3]
	bl	VDMDRV_ClearChanIsDec
	mov	w0, w19
	bl	SCDDRV_ResetSCD
	mov	w0, w19
	bl	VDMDRV_ResetVDH
	ldr	x1, [x20, 32]
	mov	w0, 4
	blr	x1
	ldr	x1, [x20, 32]
	mov	w0, 0
	blr	x1
	ldr	x2, [x20, 48]
	mov	w1, w19
	mov	w0, 1
	blr	x2
	add	x1, x29, 72
	mov	w0, w19
	bl	VCTRL_FreeChanResource
	ldr	x2, [x20, 56]
	mov	w0, 1
	mov	w1, w19
	blr	x2
	mov	w0, 0
.L1828:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L1840:
	mov	w0, w19
	ldr	x23, [x20, 256]
	bl	SCDDRV_IsChanDec
	cmp	w0, wzr
	mov	w2, 500
	cset	w1, eq
	mov	w0, w22
	blr	x23
	b	.L1831
	.p2align 3
.L1839:
	ldr	x3, .LC800
	mov	w2, 2764
	ldr	x1, .LC795
	mov	w0, 0
.L1836:
	bl	dprint_vfmw
	ldr	x1, [x20, 32]
	mov	w0, 0
	blr	x1
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, -1
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L1841:
	mov	w0, 4
	blr	x1
	ldr	x1, [x20, 32]
	mov	w0, 0
	blr	x1
	mov	w0, -1
	b	.L1828
	.p2align 3
.L1837:
	ldr	x3, .LC793
	mov	w2, 2740
	ldr	x1, .LC795
	mov	w0, 0
	b	.L1836
.L1838:
	ldr	x3, .LC798
	mov	w2, 2750
	ldr	x1, .LC795
	b	.L1836
	.size	VCTRL_DestroyChanWithOption, .-VCTRL_DestroyChanWithOption
	.align	3
.LC792:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC793:
	.xword	.LC51
	.align	3
.LC795:
	.xword	.LC794
	.align	3
.LC796:
	.xword	.LANCHOR0
	.align	3
.LC798:
	.xword	.LC797
	.align	3
.LC800:
	.xword	.LC799
	.align	3
.LC801:
	.xword	g_LowDelaySeqIndex
	.align	3
.LC802:
	.xword	.LANCHOR3
	.align	3
.LC803:
	.xword	s_SmIIS
	.align	3
.LC804:
	.xword	s_pFspInst
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_DestroyChan
	.type	VCTRL_DestroyChan, %function
VCTRL_DestroyChan:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	mov	w0, w19
	bl	VCTRL_DestroyChanWithOption
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_DestroyChan, .-VCTRL_DestroyChan
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_CloseVfmw
	.type	VCTRL_CloseVfmw, %function
VCTRL_CloseVfmw:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC806
	ldr	w0, [x20, 128]
	cmp	w0, 1
	bne	.L1850
	mov	x19, 0
	.p2align 2
.L1846:
	ldr	x1, [x20, x19, lsl 3]
	mov	w0, w19
	add	x19, x19, 1
	cbz	x1, .L1845
	bl	VCTRL_DestroyChanWithOption
.L1845:
	cmp	x19, 16
	bne	.L1846
	bl	FSP_Exit
	ldr	x21, .LC807
	ldr	w1, [x20, 132]
	mov	w2, 3
	str	w2, [x20, 136]
	cbz	w1, .L1848
	ldr	x1, [x21, 232]
	mov	w0, 10
	mov	w19, 49
	blr	x1
	b	.L1849
	.p2align 3
.L1860:
	ldr	x1, [x21, 232]
	blr	x1
	subs	w19, w19, #1
	beq	.L1848
.L1849:
	ldr	w1, [x20, 132]
	mov	w0, 10
	cbnz	w1, .L1860
.L1848:
	ldr	x0, .LC808
	mov	x2, 64
	ldr	x3, [x21, 96]
	mov	w1, 0
	add	x0, x0, 3384
	blr	x3
	add	x0, x20, 160
	bl	CloseDrivers
	str	wzr, [x20, 128]
	mov	w0, 0
.L1859:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L1850:
	mov	w0, -1
	b	.L1859
	.size	VCTRL_CloseVfmw, .-VCTRL_CloseVfmw
	.align	3
.LC806:
	.xword	.LANCHOR0
	.align	3
.LC807:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC808:
	.xword	.LANCHOR3
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_InitDspCode
	.type	VCTRL_InitDspCode, %function
VCTRL_InitDspCode:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cmp	w20, 6
	beq	.L1871
	mov	w19, 2
	cmp	w20, wzr
	csel	w19, w19, wzr, eq
.L1862:
	ldr	x0, .LC810
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L1873
	mov	w1, w19
	mov	w0, 0
	bl	VDH_Require_DspCode
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1873:
	ldr	x20, .LC811
	ldr	x1, [x20, 8]
	cbz	x1, .L1874
	mov	w0, 0
	blr	x1
.L1865:
	ldr	x1, [x20, 40]
	cbz	x1, .L1875
	mov	w0, 0
	blr	x1
.L1867:
	ldr	x1, [x20, 24]
	cbz	x1, .L1876
	mov	w0, 0
	blr	x1
.L1869:
	mov	w1, w19
	mov	w0, 0
	bl	SCDDRV_LoadDspCode
	mov	w0, w21
	bl	VCTRL_CloseHardware
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1871:
	mov	w19, 1
	b	.L1862
	.p2align 3
.L1874:
	ldr	x1, .LC812
	mov	w2, 3799
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1865
	.p2align 3
.L1875:
	ldr	x1, .LC812
	mov	w2, 3800
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1867
	.p2align 3
.L1876:
	ldr	x1, .LC812
	mov	w2, 3801
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1869
	.size	VCTRL_InitDspCode, .-VCTRL_InitDspCode
	.align	3
.LC810:
	.xword	g_HalDisable
	.align	3
.LC811:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC812:
	.xword	.LC22
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_LoadDspCode
	.type	VCTRL_LoadDspCode, %function
VCTRL_LoadDspCode:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC813
	ldr	x0, [x0, w19, sxtw 3]
	ldr	w1, [x0, 672]
	cbnz	w1, .L1889
	ldr	w20, [x0, 1300]
	mov	w0, 0
	cmp	w20, wzr
	cset	w3, eq
	cmp	w1, 6
	cset	w2, ne
	tst	w3, w2
	beq	.L1908
.L1895:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1889:
	cmp	w1, 6
	mov	w3, 1
	cset	w2, ne
	mov	w20, 0
	tst	w3, w2
	mov	w0, 0
	bne	.L1895
.L1908:
	ldr	x21, .LC814
	add	x0, x21, 3388
	add	x3, x21, 3448
	ldr	w2, [x21, 3384]
	cbnz	w2, .L1879
	.p2align 2
.L1880:
	ldr	w2, [x0]
	add	x0, x0, 4
	cmp	x3, x0
	cbnz	w2, .L1879
	bne	.L1880
	mov	w0, w19
	bl	VCTRL_InitDspCode
	cmp	w20, 1
	beq	.L1909
	add	x21, x21, 3384
	mov	w1, 2
	mov	w0, 0
	str	w1, [x21, w19, sxtw 2]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1879:
	cmp	w2, 1
	beq	.L1910
	cmp	w1, 6
	mov	w0, 0
	beq	.L1895
	ldr	x1, .LC818
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1895
	.p2align 3
.L1910:
	cmp	w20, 1
	mov	w0, 0
	beq	.L1895
	ldr	x1, .LC816
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1895
	.p2align 3
.L1909:
	add	x21, x21, 3384
	mov	w0, 0
	str	w20, [x21, w19, sxtw 2]
	b	.L1895
	.size	VCTRL_LoadDspCode, .-VCTRL_LoadDspCode
	.align	3
.LC813:
	.xword	.LANCHOR0
	.align	3
.LC814:
	.xword	.LANCHOR3
	.align	3
.LC816:
	.xword	.LC815
	.align	3
.LC818:
	.xword	.LC817
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetChanFsPartitionState
	.type	VCTRL_SetChanFsPartitionState, %function
VCTRL_SetChanFsPartitionState:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	ldr	x0, .LC821
	ldr	x0, [x0, w20, sxtw 3]
	add	x0, x0, 290816
	str	w19, [x0, 2440]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_SetChanFsPartitionState, .-VCTRL_SetChanFsPartitionState
	.align	3
.LC821:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetCurChanFsSize
	.type	VCTRL_GetCurChanFsSize, %function
VCTRL_GetCurChanFsSize:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC822
	ldr	x0, [x0, w19, sxtw 3]
	ldr	x19, [sp, 16]
	add	x0, x0, 290816
	ldp	x29, x30, [sp], 32
	ldr	w0, [x0, 2460]
	ret
	.size	VCTRL_GetCurChanFsSize, .-VCTRL_GetCurChanFsSize
	.align	3
.LC822:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRT_SetChanCtrlState
	.type	VCTRT_SetChanCtrlState, %function
VCTRT_SetChanCtrlState:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cmp	w19, 1
	beq	.L1915
	cbz	w19, .L1916
	cmp	w19, 2
	bne	.L1924
	ldr	x0, .LC823
	add	x0, x0, 88
	ldr	w1, [x0, w20, sxtw 2]
	cmp	w1, 1
	beq	.L1920
.L1922:
	str	w19, [x0, w20, sxtw 2]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1924:
	mov	w0, 0
.L1914:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1916:
	ldr	x1, .LC823
	mov	w0, 0
	add	x1, x1, 88
	str	wzr, [x1, w20, sxtw 2]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1915:
	ldr	x0, .LC823
	add	x0, x0, 88
	ldr	w1, [x0, w20, sxtw 2]
	cbz	w1, .L1922
.L1920:
	mov	w0, -1
	b	.L1914
	.size	VCTRT_SetChanCtrlState, .-VCTRT_SetChanCtrlState
	.align	3
.LC823:
	.xword	.LANCHOR5
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetFsParamToChan
	.type	VCTRL_SetFsParamToChan, %function
VCTRL_SetFsParamToChan:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x0, .LC825
	ldr	w2, [x19, 4]
	ldr	x0, [x0, w20, sxtw 3]
	add	x1, x0, 290816
	str	w2, [x0, 380]
	ldr	w2, [x19]
	str	w2, [x0, 376]
	ldr	w2, [x19]
	str	w2, [x0, 280]
	ldr	x2, [x19, 16]
	str	x2, [x0, 272]
	ldr	w2, [x19, 4]
	str	w2, [x0, 288]
	ldr	w0, [x19, 8]
	str	w0, [x1, 2468]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_SetFsParamToChan, .-VCTRL_SetFsParamToChan
	.align	3
.LC825:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_SetSingleFrameBufToChan
	.type	VCTRL_SetSingleFrameBufToChan, %function
VCTRL_SetSingleFrameBufToChan:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	w19, w0
	mov	x0, x30
	mov	x24, x1
	bl	_mcount
	ldr	x22, .LC827
	ldr	x1, .LC826
	mov	w2, 32
	ldr	x23, .LC828
	mov	w0, 8
	ldr	x20, [x22, w19, sxtw 3]
	ldr	x26, [x1, w19, sxtw 3]
	add	x1, x20, 290816
	add	x25, x20, 294912
	ldr	w1, [x1, 2468]
	ldr	w21, [x25, 1968]
	cmp	w1, w2
	csel	w1, w1, w2, le
	str	w1, [x25, 1776]
	ldr	x1, [x23, 24]
	blr	x1
	ldr	x0, [x22, w19, sxtw 3]
	add	x2, x0, 290816
	ldr	w1, [x2, 2488]
	cmp	w1, 255
	beq	.L1957
	add	x2, x2, 2560
	mov	x1, 1
	b	.L1928
	.p2align 3
.L1998:
	add	w1, w1, 1
	cmp	w1, 32
	beq	.L1993
.L1928:
	ldr	w3, [x2]
	add	x2, x2, 72
	cmp	w3, 255
	bne	.L1998
.L1927:
	lsl	x2, x1, 6
	ldr	x3, [x23, 104]
	add	x1, x2, x1, lsl 3
	mov	x2, 72
	add	x1, x1, 290816
	add	x0, x0, x1
	mov	x1, x24
	add	x0, x0, 2488
	blr	x3
	ldr	x0, [x22, w19, sxtw 3]
	add	x0, x0, 294912
	ldr	w1, [x0, 2008]
	add	w1, w1, 1
	str	w1, [x0, 2008]
	ldr	w0, [x24]
	cmp	w0, 255
	beq	.L1950
	ldr	w2, [x24, 12]
	ldr	w0, [x24, 32]
	ldr	x1, [x24, 24]
	bl	MEM_AddMemRecord
.L1950:
	ldr	w0, [x24, 4]
	cmp	w0, 255
	beq	.L1930
	ldr	w2, [x24, 16]
	ldr	w0, [x24, 48]
	ldr	x1, [x24, 40]
	bl	MEM_AddMemRecord
.L1930:
	ldr	w0, [x24, 8]
	cmp	w0, 255
	beq	.L1931
	ldr	w2, [x24, 20]
	ldr	w0, [x24, 64]
	ldr	x1, [x24, 56]
	bl	MEM_AddMemRecord
.L1931:
	ldr	x0, [x22, w19, sxtw 3]
	add	x0, x0, 294912
	ldr	w1, [x0, 2012]
	cbz	w1, .L1948
	ldrsb	w1, [x26, 1]
	cbnz	w1, .L1999
.L1934:
	ldr	w0, [x25, 1776]
	strb	w0, [x26, 10]
	add	x0, x25, 696
	strb	wzr, [x26, 11]
	ldr	w1, [x0, 1248]
	strb	w1, [x26, 13]
	ldr	w1, [x24]
	cmp	w1, 255
	beq	.L2000
.L1952:
	ldrb	w0, [x0, 16]
	cbz	w0, .L2001
	add	x0, x25, 724
	mov	x1, 1
	b	.L1937
	.p2align 3
.L2002:
	add	w1, w1, 1
	cmp	w1, 32
	beq	.L1993
.L1937:
	ldrb	w2, [x0]
	add	x0, x0, 12
	cbnz	w2, .L2002
.L1936:
	uxtw	x0, w1
	ldr	w5, [x24, 32]
	lsl	x2, x0, 2
	mov	w6, 1
	sub	x0, x2, x0
	add	w3, w5, 1023
	lsl	x0, x0, 2
	and	w3, w3, -1024
	add	x4, x20, x0
	add	x2, x20, x2
	add	x4, x4, 294912
	add	x2, x2, 294912
	sub	w21, w21, w5
	add	w21, w21, w3
	str	w3, [x4, 704]
	add	x3, x25, 696
	add	x0, x3, x0
	ldr	w7, [x24, 12]
	str	w7, [x4, 708]
	add	w7, w5, w21
	strb	w6, [x4, 712]
	ldr	w4, [x25, 1948]
	add	w4, w4, 1
	str	w4, [x25, 1948]
	str	w7, [x2, 1600]
	ldr	w4, [x25, 1732]
	add	w5, w5, w4
	add	w21, w5, w21
	str	w21, [x2, 1088]
	ldr	w2, [x24, 48]
	str	w2, [x0, 520]
	strb	w6, [x0, 528]
	ldr	w0, [x25, 1956]
	add	w0, w0, 1
	str	w0, [x25, 1956]
	ldr	w0, [x24, 8]
	cmp	w0, 255
	beq	.L1939
	ldrb	w0, [x25, 1784]
	cbz	w0, .L1959
.L2003:
	add	x2, x25, 1792
	mov	w0, 1
	.p2align 2
.L1941:
	ldrb	w3, [x2]
	add	x2, x2, 8
	cbz	w3, .L1940
	add	w0, w0, 1
	cmp	w0, 20
	bne	.L1941
.L1939:
	add	x4, x26, 20480
	ldr	w0, [x4, 1532]
	cbz	w0, .L1960
	add	x2, x4, 1632
	mov	x0, 1
	b	.L1944
	.p2align 3
.L1943:
	add	w0, w0, 1
	cmp	w0, 32
	beq	.L1993
.L1944:
	ldr	w3, [x2]
	add	x2, x2, 100
	cbnz	w3, .L1943
	mov	w5, w0
.L1942:
	lsl	x3, x1, 2
	lsl	x2, x0, 2
	sub	x1, x3, x1
	sub	x2, x2, x0
	add	x3, x20, x3
	add	x6, x26, 28672
	add	x20, x20, x1, lsl 2
	lsl	x1, x2, 5
	add	x2, x2, x1
	add	x20, x20, 294912
	add	x0, x2, x0
	ldr	w1, [x6, 396]
	add	x0, x26, x0
	add	x3, x3, 294912
	add	x0, x0, 20480
	add	w1, w1, 1
	str	w1, [x6, 396]
	add	x1, x25, 696
	add	x2, x26, 24576
	str	w5, [x0, 1444]
	add	x5, x26, 20
	ldr	w7, [x20, 704]
	str	w7, [x0, 1452]
	ldr	w7, [x20, 708]
	str	w7, [x0, 1456]
	mov	w7, 1
	ldr	w8, [x3, 1088]
	str	w8, [x0, 1464]
	ldr	w8, [x25, 696]
	str	w8, [x0, 1468]
	ldr	w8, [x1, 1052]
	str	w8, [x0, 1484]
	ldr	w8, [x1, 1060]
	str	w8, [x0, 1488]
	ldr	w8, [x1, 1064]
	str	w8, [x0, 1492]
	ldr	w8, [x1, 1044]
	str	w8, [x0, 1496]
	ldr	w8, [x1, 1048]
	str	w8, [x0, 1500]
	ldr	w3, [x3, 1600]
	str	w3, [x0, 1472]
	ldr	w3, [x1, 1032]
	str	w3, [x0, 1476]
	ldr	w3, [x1, 1036]
	str	w3, [x0, 1480]
	mov	x3, x5
	ldr	w5, [x5, 8]
	str	w5, [x0, 1504]
	ldr	w3, [x3, 12]
	str	w3, [x0, 1508]
	ldr	w8, [x1, 1068]
	str	w8, [x0, 1512]
	ldr	w8, [x1, 1072]
	str	w8, [x0, 1516]
	ldr	w8, [x1, 1076]
	str	w8, [x0, 1520]
	strb	wzr, [x0, 1436]
	strb	wzr, [x0, 1437]
	strb	wzr, [x0, 1438]
	str	w7, [x0, 1532]
	ldr	w0, [x2, 3836]
	cbnz	w0, .L1956
	strb	w7, [x26, 12]
	str	wzr, [x2, 3748]
	str	wzr, [x2, 3756]
	ldr	w0, [x25, 696]
	str	w0, [x2, 3772]
	str	w5, [x2, 3808]
	str	w3, [x2, 3812]
	ldr	w0, [x1, 1068]
	str	w0, [x2, 3816]
	ldr	w0, [x1, 1072]
	str	w0, [x2, 3820]
	ldr	w0, [x1, 1076]
	str	w0, [x2, 3824]
	strb	wzr, [x2, 3740]
	strb	wzr, [x2, 3741]
	strb	wzr, [x2, 3742]
	str	w7, [x2, 3836]
.L1956:
	ldrsb	w1, [x26, 13]
	ldr	w0, [x6, 400]
	cmp	w0, w1
	bcs	.L1946
	ldr	w1, [x24, 8]
	cmp	w1, 255
	beq	.L1946
	uxtw	x1, w0
	mov	w2, 1
	add	w0, w0, 1
	add	x7, x25, x1, lsl 3
	add	x1, x26, x1, lsl 3
	add	x1, x1, 28672
	ldr	w7, [x7, 1780]
	str	w7, [x1, 232]
	strb	w2, [x1, 236]
	strb	wzr, [x1, 237]
	str	w0, [x6, 400]
.L1946:
	ldr	w0, [x6, 404]
	add	x7, x25, 696
	ldr	w1, [x25, 1776]
	cmp	w0, w1
	bcs	.L1947
	ldr	w1, [x24, 4]
	cmp	w1, 255
	beq	.L1947
	ldr	w8, [x25, 1956]
	uxtw	x1, w0
	add	w2, w0, 1
	lsl	x0, x1, 2
	sub	w9, w8, #1
	sub	x0, x0, x1
	lsl	x8, x9, 2
	sub	x8, x8, x9
	add	x7, x7, x8, lsl 2
	ldr	w8, [x7, 520]
	lsl	x7, x0, 5
	add	x7, x0, x7
	add	x0, x26, x0, lsl 2
	add	x1, x7, x1
	add	x0, x0, 24576
	add	x1, x26, x1
	add	x1, x1, 20480
	str	w8, [x1, 1460]
	mov	w1, 1
	str	wzr, [x0, 3948]
	str	w1, [x0, 3944]
	str	w2, [x6, 404]
.L1947:
	ldr	w1, [x25, 1940]
	str	w1, [x6, 228]
	ldr	w1, [x4, 1432]
	cbnz	w1, .L1948
	ldr	w1, [x4, 1452]
	str	w1, [x4, 1352]
	mov	w1, 1
	str	wzr, [x4, 1344]
	ldr	w2, [x25, 696]
	str	w2, [x4, 1368]
	str	w5, [x4, 1404]
	str	w3, [x4, 1408]
	ldr	w2, [x25, 1764]
	str	w2, [x4, 1412]
	ldr	w2, [x25, 1768]
	str	w2, [x4, 1416]
	ldr	w0, [x25, 1772]
	str	w0, [x4, 1420]
	strb	wzr, [x4, 1336]
	strb	wzr, [x4, 1337]
	strb	wzr, [x4, 1338]
	str	w1, [x4, 1432]
.L1948:
	ldr	x1, [x23, 32]
	mov	w0, 8
	blr	x1
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1993:
	ldr	x1, [x23, 32]
	mov	w0, 8
	blr	x1
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1999:
	ldr	w0, [x0, 2540]
	cbnz	w0, .L1934
	ldrsb	w1, [x26, 23]
	ldr	w2, [x25, 1776]
	add	w0, w1, 4
	strb	wzr, [x26, 11]
	cmp	w0, w2
	csel	w0, w0, w2, le
	strb	w0, [x26, 10]
	add	x0, x25, 696
	ldr	w1, [x0, 1248]
	strb	w1, [x26, 13]
	ldr	w1, [x24]
	cmp	w1, 255
	bne	.L1952
.L2000:
	ldr	w0, [x24, 8]
	mov	x1, 0
	cmp	w0, 255
	beq	.L1939
	ldrb	w0, [x25, 1784]
	cbnz	w0, .L2003
.L1959:
	mov	w0, 0
	.p2align 2
.L1940:
	add	x0, x20, x0, uxtw 3
	ldr	w4, [x24, 64]
	add	x0, x0, 294912
	mov	w3, 1
	str	w4, [x0, 1780]
	strb	w3, [x0, 1784]
	ldr	w0, [x25, 1952]
	add	w0, w0, 1
	str	w0, [x25, 1952]
	b	.L1939
.L1957:
	mov	x1, 0
	b	.L1927
.L1960:
	mov	w5, 0
	mov	x0, 0
	b	.L1942
.L2001:
	mov	x1, 0
	b	.L1936
	.size	VCTRL_SetSingleFrameBufToChan, .-VCTRL_SetSingleFrameBufToChan
	.align	3
.LC826:
	.xword	s_pFspInst
	.align	3
.LC827:
	.xword	.LANCHOR0
	.align	3
.LC828:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_AllocMemToDecoder
	.type	VCTRL_AllocMemToDecoder, %function
VCTRL_AllocMemToDecoder:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x21, .LC829
	add	x2, x21, 88
	ldr	w0, [x2, w19, sxtw 2]
	cbz	w0, .L2013
	ldr	x2, .LC835
	mov	w3, w19
	ldr	x1, .LC837
	mov	w0, 0
	add	x2, x2, 152
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2013:
	mov	x1, x20
	mov	w3, 1
	mov	w0, w19
	str	w3, [x2, w19, sxtw 2]
	bl	VCTRL_SetSingleFrameBufToChan
	mov	w20, w0
	cbnz	w0, .L2006
	ldr	x0, .LC830
	ldr	x2, [x0, w19, sxtw 3]
	add	x2, x2, 290816
.L2007:
	ldr	x1, .LC834
	add	x21, x21, 88
	mov	w3, 2
	mov	w0, 31
	str	w3, [x2, 2440]
	bl	dprint_vfmw
	str	wzr, [x21, w19, sxtw 2]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2006:
	ldr	x22, .LC830
	ldr	x2, [x22, w19, sxtw 3]
	add	x2, x2, 290816
	ldr	w0, [x2, 2460]
	cbz	w0, .L2007
	ldr	x1, .LC832
	mov	w2, w19
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x2, [x22, w19, sxtw 3]
	mov	w0, 3
	add	x2, x2, 290816
	str	w0, [x2, 2440]
	b	.L2007
	.size	VCTRL_AllocMemToDecoder, .-VCTRL_AllocMemToDecoder
	.align	3
.LC829:
	.xword	.LANCHOR5
	.align	3
.LC830:
	.xword	.LANCHOR0
	.align	3
.LC832:
	.xword	.LC831
	.align	3
.LC834:
	.xword	.LC833
	.align	3
.LC835:
	.xword	.LANCHOR2
	.align	3
.LC837:
	.xword	.LC836
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DBG_WriteBigTileYuv
	.type	DBG_WriteBigTileYuv, %function
DBG_WriteBigTileYuv:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	ldr	w8, [x29, 160]
	mov	x27, x0
	mov	x0, x30
	uxtw	x19, w3
	mov	w25, w4
	ldr	w23, [x29, 168]
	mov	x20, x2
	ldr	w24, [x29, 184]
	mov	w26, w8
	mov	x21, x1
	mov	w22, w5
	str	w8, [x29, 96]
	str	w7, [x29, 124]
	str	w6, [x29, 140]
	str	x27, [x29, 144]
	str	w4, [x29, 152]
	bl	_mcount
	sub	w0, w19, w26
	sub	w0, w0, w23
	mov	w23, w25
	mov	w2, w0
	ldr	w0, [x29, 176]
	lsr	w25, w25, 1
	sub	w0, w23, w0
	sub	w0, w0, w24
	mov	w3, w0
	lsr	w0, w19, 1
	stp	w2, w3, [x29, 128]
	str	w0, [x29, 136]
	cbz	x27, .L2014
	ldr	x1, .LC840
	mov	w0, 32
	bl	dprint_vfmw
	cmp	w22, wzr
	ccmp	w22, 3, 4, ne
	bne	.L2017
	ldr	x27, .LC842
	cbz	w23, .L2019
	ldr	x28, .LC841
	mov	w24, 0
	str	wzr, [x29, 156]
	.p2align 2
.L2021:
	cbz	w19, .L2022
	ldr	w1, [x29, 140]
	lsr	w0, w24, 4
	ubfiz	x22, x24, 8, 4
	ldr	w23, [x29, 156]
	mov	w26, 0
	mul	w0, w0, w1
	add	x22, x0, x22
	.p2align 2
.L2020:
	lsr	w1, w26, 8
	ldr	x6, [x27, 960]
	ubfiz	x1, x1, 12, 20
	add	x0, x23, x26, uxtw
	add	x1, x1, x22
	ldr	x5, [x28, 104]
	mov	x2, 256
	add	x1, x21, x1
	add	x0, x6, x0
	add	w26, w26, 256
	blr	x5
	cmp	w19, w26
	bhi	.L2020
.L2022:
	ldr	w0, [x29, 156]
	add	w24, w24, 1
	add	w0, w0, w19
	str	w0, [x29, 156]
	ldr	w0, [x29, 152]
	cmp	w0, w24
	bne	.L2021
.L2019:
	ldp	w0, w1, [x29, 128]
	cmp	w19, w0
	ldr	w0, [x29, 152]
	cset	w28, eq
	cmp	w0, w1
	cset	w0, eq
	ands	w0, w28, w0
	str	w0, [x29, 120]
	bne	.L2119
	ldr	w0, [x29, 176]
	ldr	w1, [x29, 96]
	ldr	x22, [x27, 960]
	mul	w0, w19, w0
	add	x0, x0, x1, uxtw
	add	x22, x22, x0
	ldr	w0, [x29, 132]
	cbz	w0, .L2027
	ldr	x28, .LC841
	uxtw	x21, w19
	mov	w23, 0
	.p2align 2
.L2028:
	ldr	w1, [x29, 128]
	mov	x0, x22
	ldr	x4, [x28, 88]
	add	w23, w23, 1
	ldr	x2, [x29, 144]
	add	x22, x22, x21
	blr	x4
	ldr	w0, [x29, 132]
	cmp	w0, w23
	bne	.L2028
.L2027:
	cbz	w25, .L2120
	ldr	w1, [x29, 140]
	mov	w24, 0
	ldr	x28, .LC841
	mov	w23, 0
	lsr	w0, w1, 1
	str	w0, [x29, 156]
	.p2align 2
.L2031:
	cbz	w19, .L2033
	ldr	w1, [x29, 156]
	lsr	w0, w23, 3
	ubfiz	x21, x23, 8, 3
	uxtw	x22, w24
	mov	w26, 0
	mul	w0, w0, w1
	add	x21, x0, x21
	.p2align 2
.L2030:
	lsr	w1, w26, 8
	ldr	x6, [x27, 960]
	ubfiz	x1, x1, 11, 21
	add	x0, x22, x26, uxtw
	add	x1, x1, x21
	ldr	x5, [x28, 104]
	mov	x2, 256
	add	x1, x20, x1
	add	x0, x6, x0
	add	w26, w26, 256
	blr	x5
	cmp	w19, w26
	bhi	.L2030
.L2033:
	add	w23, w23, 1
	add	w24, w24, w19
	cmp	w25, w23
	bne	.L2031
	ldr	w0, [x29, 124]
	cmp	w0, 1
	beq	.L2121
.L2061:
	ldr	w0, [x29, 124]
	cbz	w0, .L2122
.L2036:
	ldr	w0, [x29, 120]
	cbnz	w0, .L2123
	ldr	w0, [x29, 176]
	ldp	w3, w2, [x29, 132]
	lsr	w1, w0, 1
	ldr	x0, [x29, 96]
	ldr	x19, [x27, 944]
	lsr	w22, w3, 1
	mul	w1, w1, w2
	ldr	x20, [x27, 952]
	ubfx	x0, x0, 1, 31
	add	x0, x1, x0
	add	x20, x20, x0
	add	x19, x19, x0
	cbz	w22, .L2014
	ldr	w0, [x29, 128]
	uxtw	x24, w2
	ldr	x28, .LC841
	mov	w21, 0
	lsr	w23, w0, 1
	.p2align 2
.L2041:
	ldr	x3, [x28, 88]
	mov	x0, x20
	ldr	x2, [x29, 144]
	mov	w1, w23
	add	w21, w21, 1
	add	x20, x20, x24
	blr	x3
	cmp	w22, w21
	bne	.L2041
	mov	w20, 0
	.p2align 2
.L2042:
	ldr	x3, [x28, 88]
	mov	x0, x19
	ldr	x2, [x29, 144]
	mov	w1, w23
	add	w20, w20, 1
	add	x19, x19, x24
	blr	x3
	cmp	w22, w20
	bne	.L2042
.L2014:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 160
	ret
.L2017:
	cmp	w22, 1
	ldr	w1, [x29, 152]
	cset	w0, ne
	str	w0, [x29, 132]
	cmp	w1, w0
	ldr	x28, .LC841
	ldr	x27, .LC842
	bls	.L2044
	ldr	w24, [x29, 132]
	.p2align 2
.L2046:
	cbz	w19, .L2047
	ldr	w1, [x29, 140]
	lsr	w0, w24, 4
	lsr	w23, w24, 1
	ubfiz	x22, x24, 8, 4
	mov	w26, 0
	mul	w0, w0, w1
	mul	w23, w23, w19
	add	x22, x0, x22
	.p2align 2
.L2045:
	lsr	w1, w26, 8
	ldr	x7, [x27, 960]
	ubfiz	x1, x1, 12, 20
	add	x0, x23, x26, uxtw
	add	x1, x1, x22
	ldr	x6, [x28, 104]
	mov	x2, 256
	add	x1, x21, x1
	add	x0, x7, x0
	add	w26, w26, 256
	blr	x6
	cmp	w19, w26
	bhi	.L2045
.L2047:
	ldr	w0, [x29, 152]
	add	w24, w24, 2
	cmp	w0, w24
	bhi	.L2046
.L2044:
	ldr	w0, [x29, 152]
	ldr	x4, [x28, 88]
	ldr	x2, [x29, 144]
	mul	w1, w19, w0
	ldr	x0, [x27, 960]
	lsr	w1, w1, 1
	blr	x4
	cbz	w25, .L2124
	ldr	w1, [x29, 140]
	mov	w24, 0
	mov	w23, 0
	lsr	w0, w1, 1
	str	w0, [x29, 156]
	.p2align 2
.L2051:
	cbz	w19, .L2053
	ldr	w1, [x29, 156]
	lsr	w0, w23, 3
	ubfiz	x21, x23, 8, 3
	uxtw	x22, w24
	mov	w26, 0
	mul	w0, w0, w1
	add	x21, x21, x0
	.p2align 2
.L2050:
	lsr	w1, w26, 8
	ldr	x7, [x27, 960]
	ubfiz	x1, x1, 11, 21
	add	x0, x22, x26, uxtw
	add	x1, x1, x21
	ldr	x6, [x28, 104]
	mov	x2, 256
	add	x1, x20, x1
	add	x0, x7, x0
	add	w26, w26, 256
	blr	x6
	cmp	w19, w26
	bhi	.L2050
.L2053:
	add	w23, w23, 1
	add	w24, w24, w19
	cmp	w25, w23
	bne	.L2051
	ldr	w0, [x29, 124]
	cmp	w0, 1
	beq	.L2125
	ldr	w0, [x29, 124]
	cbnz	w0, .L2056
	ldr	w0, [x29, 132]
	cmp	w25, w0
	bls	.L2059
	.p2align 2
.L2060:
	ldr	w0, [x29, 132]
	add	w0, w0, 2
	str	w0, [x29, 132]
	cmp	w25, w0
	bhi	.L2060
.L2059:
	ldr	x4, [x28, 96]
	mov	x2, 1048576
	ldr	x0, [x27, 944]
	mov	w1, 128
	blr	x4
	ldr	x0, [x27, 952]
	mov	x2, 1048576
	ldr	x4, [x28, 96]
	mov	w1, 128
	blr	x4
.L2056:
	ldr	w0, [x29, 136]
	ldr	x20, [x29, 144]
	ldr	x4, [x28, 88]
	mul	w19, w0, w25
	ldr	x0, [x27, 952]
	mov	x2, x20
	lsr	w19, w19, 1
	mov	w1, w19
	blr	x4
	ldr	x4, [x28, 88]
	mov	x2, x20
	ldr	x0, [x27, 944]
	mov	w1, w19
	blr	x4
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 160
	ret
.L2119:
	ldr	w0, [x29, 152]
	ldr	x28, .LC841
	ldr	x2, [x29, 144]
	mul	w1, w19, w0
	ldr	x4, [x28, 88]
	ldr	x0, [x27, 960]
	blr	x4
	b	.L2027
.L2125:
	ldr	w0, [x29, 132]
	mov	w9, 0
	ldr	x4, [x27, 960]
	mul	w0, w19, w0
	ubfiz	x19, x19, 1, 31
	add	x4, x4, x0
	.p2align 2
.L2055:
	ldr	w2, [x29, 136]
	cbz	w2, .L2057
	lsr	w0, w9, 1
	mov	w1, 1
	mul	w0, w0, w2
	add	w8, w2, w0
	.p2align 2
.L2054:
	sub	w7, w1, #1
	uxtw	x2, w0
	ldr	x6, [x27, 944]
	add	w0, w0, 1
	cmp	w8, w0
	ldrb	w7, [x4, x7]
	strb	w7, [x6, x2]
	ldr	x6, [x27, 952]
	ldrb	w7, [x4, w1, uxtw]
	add	w1, w1, 2
	strb	w7, [x6, x2]
	bne	.L2054
.L2057:
	add	w9, w9, 2
	add	x4, x4, x19
	cmp	w25, w9
	bhi	.L2055
	b	.L2056
.L2121:
	ldr	x4, [x27, 960]
	mov	w0, 0
	mov	w8, 0
	.p2align 2
.L2035:
	ldr	w1, [x29, 136]
	mov	w7, w0
	cbz	w1, .L2037
	ldr	w1, [x29, 136]
	add	w7, w1, w0
	mov	w1, 1
	.p2align 2
.L2034:
	sub	w6, w1, #1
	uxtw	x2, w0
	ldr	x5, [x27, 944]
	add	w0, w0, 1
	cmp	w7, w0
	ldrb	w6, [x4, x6]
	strb	w6, [x5, x2]
	ldr	x5, [x27, 952]
	ldrb	w6, [x4, w1, uxtw]
	add	w1, w1, 2
	strb	w6, [x5, x2]
	bne	.L2034
.L2037:
	add	w8, w8, 1
	add	x4, x4, x19
	cmp	w25, w8
	mov	w0, w7
	bne	.L2035
	b	.L2036
.L2122:
	ldr	x28, .LC841
	mov	x2, 1048576
	ldr	x0, [x27, 944]
	mov	w1, 128
	ldr	x4, [x28, 96]
	blr	x4
	ldr	x0, [x27, 952]
	mov	x2, 1048576
	ldr	x4, [x28, 96]
	mov	w1, 128
	blr	x4
	b	.L2036
.L2123:
	ldr	w0, [x29, 136]
	ldr	x19, .LC841
	ldr	x20, [x29, 144]
	mul	w25, w0, w25
	ldr	x4, [x19, 88]
	mov	x2, x20
	ldr	x0, [x27, 952]
	mov	w1, w25
	blr	x4
	ldr	x4, [x19, 88]
	mov	x2, x20
	ldr	x0, [x27, 944]
	mov	w1, w25
	blr	x4
	b	.L2014
.L2124:
	ldr	w0, [x29, 124]
	cmp	w0, 1
	beq	.L2056
	ldr	w0, [x29, 124]
	cbnz	w0, .L2056
	b	.L2059
.L2120:
	ldr	w0, [x29, 124]
	cmp	w0, 1
	bne	.L2061
	b	.L2036
	.size	DBG_WriteBigTileYuv, .-DBG_WriteBigTileYuv
	.align	3
.LC840:
	.xword	.LC839
	.align	3
.LC841:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC842:
	.xword	.LANCHOR5
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetChanImage
	.type	VCTRL_GetChanImage, %function
VCTRL_GetChanImage:
	sub	sp, sp, #624
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	sub	sp, sp, #32
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x1, .LC844
	mov	w2, w20
	mov	w0, 2
	bl	dprint_vfmw
	cmp	w20, 15
	bhi	.L2175
	ldr	x21, .LC847
	ldr	x0, [x21, w20, sxtw 3]
	cbz	x0, .L2176
	ldr	w0, [x0, 12]
	cmp	w0, 1
	bne	.L2177
	ldr	x23, .LC850
	mov	w0, 4
	ldr	x1, [x23, 24]
	blr	x1
	ldr	x0, [x21, w20, sxtw 3]
	ldr	w4, [x0, 20]
	cmp	w4, 1
	bne	.L2178
	add	x0, x0, 294912
	mov	x1, x19
	add	x0, x0, 2576
	bl	SYNTAX_ReceiveImage
	cmp	w0, 1
	ccmp	x19, xzr, 4, eq
	mov	w24, w0
	beq	.L2133
	ldr	x22, .LC854
	add	x0, x22, 648
	ldr	x0, [x0, w20, sxtw 3]
	cbz	x0, .L2134
	ldr	w1, [x19, 108]
	ldr	w2, [x22, 968]
	ubfx	x1, x1, 2, 3
	ldr	w4, [x19, 116]
	cmp	w1, wzr
	ldr	w6, [x19, 140]
	cset	w7, eq
	cbz	w2, .L2135
	ldr	w5, [x19, 344]
	ldr	w3, [x19, 112]
	ldp	x1, x2, [x19, 504]
	str	w5, [sp, 24]
	ldr	w5, [x19, 340]
	str	w5, [sp, 16]
	ldr	w5, [x19, 336]
	str	w5, [sp, 8]
	ldr	w5, [x19, 332]
	str	w5, [sp]
	mov	w5, 0
	bl	DBG_WriteBigTileYuv
.L2134:
	add	x22, x22, 1240
	ldr	x2, [x22, w20, sxtw 3]
	cbz	x2, .L2133
	ldr	x0, [x19, 232]
	cbz	x0, .L2133
	ldr	w1, [x19, 224]
	cbnz	w1, .L2179
	.p2align 2
.L2133:
	cmp	w24, 1
	mov	w22, -1
	beq	.L2180
.L2139:
	mov	w0, 12
	bl	CHECK_REC_POS_ENABLE
	cbnz	w0, .L2150
.L2148:
	cbz	w22, .L2151
.L2149:
	ldr	x1, [x21, w20, sxtw 3]
	mov	w0, 4
	ldr	x2, [x23, 32]
	add	x1, x1, 311296
	ldrb	w1, [x1, 2024]
	strb	w1, [x19, 472]
	blr	x2
.L2171:
	add	sp, x29, 0
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 624
	ret
	.p2align 3
.L2180:
	ldr	w0, [x19, 200]
	str	w22, [x19, 196]
	cmp	w0, 1
	beq	.L2181
	mov	w22, 0
.L2186:
	mov	w0, 12
	bl	CHECK_REC_POS_ENABLE
	cbz	w0, .L2148
	b	.L2150
.L2145:
	ldr	x0, [x21, w20, sxtw 3]
	mov	x1, x19
	add	x0, x0, 294912
	add	x0, x0, 2576
	bl	SYNTAX_ReleaseImage
	ldr	w0, [x29, 252]
	cbz	w0, .L2182
	ldr	x2, .LC851
	mov	w0, w24
	ldr	x1, .LC860
	ldr	w3, [x19, 192]
	add	x2, x2, 176
	bl	dprint_vfmw
	ldr	x3, [x23, 104]
	mov	x2, 624
	add	x1, x29, 64
	mov	x0, x19
	blr	x3
	ldp	w1, w2, [x29, 92]
.L2173:
	mov	w0, 3
	str	w24, [x19, 188]
	str	w0, [x19, 356]
	mov	w0, 12
	str	w22, [x19, 196]
	stp	w1, w2, [x19, 204]
	bl	CHECK_REC_POS_ENABLE
	cbnz	w0, .L2183
	.p2align 2
.L2151:
	ldr	x0, [x21, w20, sxtw 3]
	mov	w22, 0
	ldr	w1, [x19, 108]
	str	w1, [x0, 432]
	ldr	w1, [x19, 356]
	str	w1, [x0, 436]
	b	.L2149
	.p2align 3
.L2183:
	mov	w22, 0
	.p2align 2
.L2150:
	ldr	x0, [x23]
	blr	x0
	mov	w3, w0
	ldr	x1, .LC854
	ldr	x0, .LC861
	add	x1, x1, 1776
	ldr	w2, [x0]
	ldr	w0, [x1, w20, sxtw 2]
	sub	w0, w3, w0
	cmp	w0, w2
	bls	.L2148
	lsl	w2, w20, 24
	ldr	x0, .LC851
	add	w2, w2, 786432
	cmp	w24, 1
	str	w3, [x1, w20, sxtw 2]
	csinc	w2, w2, w2, ne
	add	x0, x0, 200
	mov	w1, 4803
	bl	DBG_AddTrace
	cbnz	w22, .L2149
	b	.L2151
	.p2align 3
.L2135:
	str	wzr, [sp]
	mov	w5, 0
	str	wzr, [sp, 8]
	str	wzr, [sp, 16]
	str	wzr, [sp, 24]
	ldr	w3, [x19, 112]
	ldp	x1, x2, [x19, 504]
	bl	DBG_WriteBigTileYuv
	b	.L2134
	.p2align 3
.L2181:
	ldr	w0, [x19, 188]
	cbz	w0, .L2184
	ldp	w1, w2, [x19, 28]
	b	.L2173
	.p2align 3
.L2177:
	ldr	x3, .LC849
	mov	w2, 4573
	ldr	x1, .LC846
	mov	w0, 0
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2171
	.p2align 3
.L2178:
	ldr	x2, .LC851
	mov	w3, 4607
	ldr	x1, .LC853
	mov	w0, 0
	add	x2, x2, 176
	mov	w22, -1
	bl	dprint_vfmw
	ldr	x1, [x23, 32]
	mov	w0, 4
	blr	x1
	b	.L2171
	.p2align 3
.L2179:
	ldr	x3, [x23, 88]
	blr	x3
	b	.L2133
	.p2align 3
.L2184:
	ldr	x0, [x21, w20, sxtw 3]
	add	x1, x29, 64
	add	x0, x0, 294912
	add	x0, x0, 2576
	bl	SYNTAX_ReceiveImage
	cmp	w0, 1
	bne	.L2185
	ldr	w0, [x19, 192]
	ldr	w1, [x29, 256]
	cmp	w0, w1
	bge	.L2142
	ldp	w0, w4, [x29, 92]
	ldp	w3, w2, [x29, 100]
	ldr	w1, [x29, 220]
	ldr	w22, [x29, 252]
	stp	w0, w4, [x19, 204]
	str	w1, [x19, 196]
	mov	w0, 3
	stp	w3, w2, [x19, 212]
	str	w0, [x19, 356]
	cbz	w22, .L2139
	str	w24, [x19, 188]
	mov	w22, 0
	b	.L2186
	.p2align 3
.L2175:
	ldr	x3, .LC845
	mov	w2, 4569
	ldr	x1, .LC846
	mov	w0, 0
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2171
	.p2align 3
.L2185:
	ldr	x1, [x21, w20, sxtw 3]
	mov	w22, w0
	add	x0, x1, 294912
	add	x0, x0, 2576
	bl	SYNTAX_ReturnImage
	ldr	x1, [x23, 32]
	mov	w0, 4
	blr	x1
	b	.L2171
.L2142:
	bgt	.L2187
	cbnz	w0, .L2145
	ldr	x0, [x21, w20, sxtw 3]
	add	x0, x0, 294912
	add	x0, x0, 2576
	bl	SYNTAX_ReturnImage
	ldr	x0, [x21, w20, sxtw 3]
	mov	x1, x19
	add	x0, x0, 294912
	add	x0, x0, 2576
	bl	SYNTAX_ReleaseImage
	ldr	x2, .LC851
	mov	w0, w24
	ldr	x1, .LC858
	mov	w24, -2
	ldr	w3, [x19, 192]
	add	x2, x2, 176
	bl	dprint_vfmw
	b	.L2139
.L2176:
	ldr	x3, .LC848
	mov	w2, 4570
	ldr	x1, .LC846
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2171
.L2187:
	ldr	x0, [x21, w20, sxtw 3]
	add	x0, x0, 294912
	add	x0, x0, 2576
	bl	SYNTAX_ReturnImage
	ldr	x0, [x21, w20, sxtw 3]
	mov	x1, x19
	add	x0, x0, 294912
	add	x0, x0, 2576
	bl	SYNTAX_ReleaseImage
	ldr	x2, .LC851
	mov	w0, w24
	ldr	x1, .LC856
	mov	w24, -2
	ldr	w3, [x19, 192]
	add	x2, x2, 176
	ldr	w4, [x29, 256]
	bl	dprint_vfmw
	b	.L2139
.L2182:
	ldr	x0, [x21, w20, sxtw 3]
	add	x1, x29, 64
	add	x0, x0, 294912
	add	x0, x0, 2576
	bl	SYNTAX_ReleaseImage
	ldr	x2, .LC851
	mov	w0, w24
	ldr	x1, .LC860
	mov	w24, -2
	ldr	w3, [x19, 192]
	add	x2, x2, 176
	bl	dprint_vfmw
	b	.L2139
	.size	VCTRL_GetChanImage, .-VCTRL_GetChanImage
	.align	3
.LC844:
	.xword	.LC843
	.align	3
.LC845:
	.xword	.LC51
	.align	3
.LC846:
	.xword	.LC2
	.align	3
.LC847:
	.xword	.LANCHOR0
	.align	3
.LC848:
	.xword	.LC5
	.align	3
.LC849:
	.xword	.LC186
	.align	3
.LC850:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC851:
	.xword	.LANCHOR2
	.align	3
.LC853:
	.xword	.LC852
	.align	3
.LC854:
	.xword	.LANCHOR5
	.align	3
.LC856:
	.xword	.LC855
	.align	3
.LC858:
	.xword	.LC857
	.align	3
.LC860:
	.xword	.LC859
	.align	3
.LC861:
	.xword	g_TraceImgPeriod
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetImageWidth
	.type	VCTRL_GetImageWidth, %function
VCTRL_GetImageWidth:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC865
	ldr	x0, [x0, w19, sxtw 3]
	add	x0, x0, 294912
	ldr	w1, [x0, 2576]
	cmp	w1, 2
	beq	.L2190
	cmp	w1, 16
	beq	.L2191
	cbz	w1, .L2196
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2196:
	add	x0, x0, 2584
	bl	H264DEC_GetImageWidth
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2191:
	add	x0, x0, 2584
	bl	HEVCDEC_GetImageWidth
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2190:
	add	x0, x0, 2584
	bl	MPEG4DEC_GetImageWidth
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_GetImageWidth, .-VCTRL_GetImageWidth
	.align	3
.LC865:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetImageHeight
	.type	VCTRL_GetImageHeight, %function
VCTRL_GetImageHeight:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC866
	ldr	x0, [x0, w19, sxtw 3]
	add	x0, x0, 294912
	ldr	w1, [x0, 2576]
	cmp	w1, 2
	beq	.L2199
	cmp	w1, 16
	beq	.L2200
	cbz	w1, .L2205
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2205:
	add	x0, x0, 2584
	bl	H264DEC_GetImageHeight
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2200:
	add	x0, x0, 2584
	bl	HEVCDEC_GetImageHeight
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2199:
	add	x0, x0, 2584
	bl	MPEG4DEC_GetImageHeight
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_GetImageHeight, .-VCTRL_GetImageHeight
	.align	3
.LC866:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_GetFsSize
	.type	VCTRL_GetFsSize, %function
VCTRL_GetFsSize:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	mov	w21, w2
	bl	_mcount
	ldr	x3, .LC867
	mov	x2, 24
	mov	w1, 0
	add	x0, x29, 56
	ldr	x3, [x3, 96]
	blr	x3
	ldr	x0, .LC868
	ldr	x6, [x0, 104]
	cbz	x6, .L2207
	ldr	x4, .LC869
	add	x5, x29, 56
	mov	w3, 0
	mov	w2, w21
	mov	w1, w20
	mov	w0, w19
	ldr	x4, [x4, w19, sxtw 3]
	add	x4, x4, 294912
	ldr	w4, [x4, 2576]
	blr	x6
.L2207:
	ldr	w0, [x29, 60]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.size	VCTRL_GetFsSize, .-VCTRL_GetFsSize
	.align	3
.LC867:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC868:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC869:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_EnableEsBuf
	.type	VCTRL_EnableEsBuf, %function
VCTRL_EnableEsBuf:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC870
	mov	w1, 1
	add	x0, x0, 1840
	str	w1, [x0, w19, sxtw 2]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_EnableEsBuf, .-VCTRL_EnableEsBuf
	.align	3
.LC870:
	.xword	.LANCHOR5
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_DisableEsBuf
	.type	VCTRL_DisableEsBuf, %function
VCTRL_DisableEsBuf:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC872
	add	x0, x0, 1840
	str	wzr, [x0, w19, sxtw 2]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_DisableEsBuf, .-VCTRL_DisableEsBuf
	.align	3
.LC872:
	.xword	.LANCHOR5
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VCTRL_IfEsBufEnable
	.type	VCTRL_IfEsBufEnable, %function
VCTRL_IfEsBufEnable:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC874
	add	x0, x0, 1840
	ldr	w0, [x0, w19, sxtw 2]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_IfEsBufEnable, .-VCTRL_IfEsBufEnable
	.align	3
.LC874:
	.xword	.LANCHOR5
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
	.align	3
.LANCHOR1 = . + 0
	.type	__func__.52069, %object
	.size	__func__.52069, 18
__func__.52069:
	.string	"VCTRL_EventReport"
	.zero	6
	.type	__func__.52081, %object
	.size	__func__.52081, 18
__func__.52081:
	.string	"VCTRL_SetCallBack"
	.zero	6
	.type	__FUNCTION__.52187, %object
	.size	__FUNCTION__.52187, 29
__FUNCTION__.52187:
	.string	"VCTRL_PartitionMemForStdUser"
	.zero	3
	.type	__func__.52211, %object
	.size	__func__.52211, 23
__func__.52211:
	.string	"VCTRL_FreeChanResource"
	.zero	1
	.type	__FUNCTION__.52470, %object
	.size	__FUNCTION__.52470, 23
__FUNCTION__.52470:
	.string	"VCTRL_ReleaseChanImage"
	.zero	1
	.type	__func__.52471, %object
	.size	__func__.52471, 23
__func__.52471:
	.string	"VCTRL_ReleaseChanImage"
	.zero	1
	.type	__func__.52497, %object
	.size	__func__.52497, 18
__func__.52497:
	.string	"VCTRL_GetDecParam"
	.align	3
.LANCHOR2 = . + 0
	.type	__FUNCTION__.52559, %object
	.size	__FUNCTION__.52559, 19
__FUNCTION__.52559:
	.string	"VCTRL_FlushDecoder"
	.zero	5
	.type	__FUNCTION__.52890, %object
	.size	__FUNCTION__.52890, 18
__FUNCTION__.52890:
	.string	"GetExtraRefFrmNum"
	.zero	6
	.type	__FUNCTION__.52896, %object
	.size	__FUNCTION__.52896, 19
__FUNCTION__.52896:
	.string	"GetExtraDispFrmNum"
	.zero	5
	.type	__FUNCTION__.52902, %object
	.size	__FUNCTION__.52902, 16
__FUNCTION__.52902:
	.string	"GetExtraCtrlCmd"
	.type	__FUNCTION__.52822, %object
	.size	__FUNCTION__.52822, 14
__FUNCTION__.52822:
	.string	"DecChanSyntax"
	.zero	2
	.type	__func__.52388, %object
	.size	__func__.52388, 24
__func__.52388:
	.string	"VCTRL_FindOccupiedFrame"
	.type	__func__.52880, %object
	.size	__func__.52880, 24
__func__.52880:
	.string	"VCTRL_ExtAccGetDecParam"
	.type	__func__.53155, %object
	.size	__func__.53155, 24
__func__.53155:
	.string	"VCTRL_AllocMemToDecoder"
	.type	__func__.52452, %object
	.size	__func__.52452, 19
__func__.52452:
	.string	"VCTRL_GetChanImage"
	.zero	5
	.type	__FUNCTION__.52460, %object
	.size	__FUNCTION__.52460, 19
__FUNCTION__.52460:
	.string	"VCTRL_GetChanImage"
	.data
	.align	3
.LANCHOR4 = . + 0
	.type	g_not_direct_8x8_inference_flag, %object
	.size	g_not_direct_8x8_inference_flag, 1
g_not_direct_8x8_inference_flag:
	.byte	1
	.zero	7
	.type	s_VfmwSavePath, %object
	.size	s_VfmwSavePath, 64
s_VfmwSavePath:
	.byte	47
	.byte	109
	.byte	110
	.byte	116
	.byte	0
	.zero	59
	.type	save_yuv_chan_num, %object
	.size	save_yuv_chan_num, 4
save_yuv_chan_num:
	.word	-1
	.type	mask_mmu_err_int, %object
	.size	mask_mmu_err_int, 4
mask_mmu_err_int:
	.word	1
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC0:
	ASCII(.string	"InstID out of range" )
	.zero	4
.LC2:
	ASCII(.string	"vfmw_ctrl.c,L%d: %s\n" )
	.zero	3
.LC5:
	ASCII(.string	"Chan inactive" )
	.zero	2
.LC7:
	ASCII(.string	"s_VfmwCTrl.event_report_vdec = NULL\n" )
	.zero	3
.LC9:
	ASCII(.string	"s_VfmwCTrl.event_report_omxvdec = NULL\n" )
.LC12:
	ASCII(.string	"%s Unkown Adapter Type: %d, Len: %d\n" )
	.zero	3
.LC22:
	ASCII(.string	"VDMHAL_NULL_FUN_PRINT,L%d\n" )
	.zero	5
.LC24:
	ASCII(.string	"Wait sleep time out %d ms (isScdSleep=%d, isVdmSleep=%d)!\n" )
	.zero	5
.LC28:
	ASCII(.string	"Vfmw suspend totally take %d ms\n" )
	.zero	7
.LC30:
	ASCII(.string	"Force scd %d sleep.\n" )
	.zero	3
.LC32:
	ASCII(.string	"Force vdm %d sleep.\n" )
	.zero	3
.LC40:
	ASCII(.string	"Vfmw resume totally take %d ms\n" )
.LC51:
	ASCII(.string	"ChanID out of range" )
	.zero	4
.LC56:
	ASCII(.string	"can NOT config a not-opened Chan" )
	.zero	7
.LC58:
	ASCII(.string	"event_report null!" )
	.zero	5
.LC63:
	ASCII(.string	"%s Unkown Adapter Type: %d\n" )
	.zero	4
.LC69:
	ASCII(.string	"pstStreamParam is NULL" )
	.zero	1
.LC82:
	ASCII(.string	"L: %d, Stream Interface Error!!, ChanID: %d\n" )
	.zero	3
.LC85:
	ASCII(.string	"pstStreamIntf is NULL" )
	.zero	2
.LC95:
	ASCII(.string	"This Chan has not been created!!!" )
	.zero	6
.LC97:
	ASCII(.string	"can NOT get a not-opened Chan's information of bitstream" )
	.zero	7
.LC104:
	ASCII(.string	"Raw FreshNum=%d, FreshSize=%d\n" )
	.zero	1
.LC111:
	ASCII(.string	"Seg FreshNum=%d, FreshSize=%d\n" )
	.zero	1
.LC115:
	ASCII(.string	"pArgs is null" )
	.zero	2
.LC123:
	ASCII(.string	"image size abnormal(%dx%d)\n" )
	.zero	4
.LC125:
	ASCII(.string	"no mem to arrange for soft dec\n" )
.LC128:
	ASCII(.string	"partition fs memory fail!\n" )
	.zero	5
.LC131:
	ASCII(.string	"%s: L%d : vdm chan memory is too small to support user decoder\n" )
.LC133:
	ASCII(.string	"FSP_ConfigInstance fail!\n" )
	.zero	6
.LC138:
	ASCII(.string	"parameter err, null pointer!!!" )
	.zero	1
.LC142:
	ASCII(.string	"%s FATAL: pstChan = NULL!\n" )
	.zero	5
.LC144:
	ASCII(.string	"%s FATAL: pChanMem_vdh = NULL!\n" )
.LC146:
	ASCII(.string	"%s FATAL: pChanMem_frm = NULL!\n" )
.LC148:
	ASCII(.string	"DelAllFrameMemRecord err in VCTRL_FreeChanResource !\n" )
	.zero	2
.LC150:
	ASCII(.string	"%s FATAL: pChanMem_scd = NULL!\n" )
.LC152:
	ASCII(.string	"%s FATAL: pChanMem_ctx = NULL!\n" )
.LC157:
	ASCII(.string	"pstCfg is null" )
	.zero	1
.LC166:
	ASCII(.string	"Chip not support beyond %dx%d when Std != H264/VP8/MVC/USER.\n" )
	.zero	2
.LC186:
	ASCII(.string	"This channel is not opened" )
	.zero	5
.LC188:
	ASCII(.string	"pImage is NULL" )
	.zero	1
.LC190:
	ASCII(.string	"call VCTRL_ReleaseChanImage, ChanID, ImgID = %d,%d\n" )
	.zero	4
.LC193:
	ASCII(.string	"vdm_ctrl.c,L%d: Chan inactive\n" )
	.zero	1
.LC195:
	ASCII(.string	"vdm_ctrl.c,L%d: This channel is not opened\n" )
	.zero	4
.LC197:
	ASCII(.string	"vdm_ctrl.c,L%d: VCTRL_TryToRelSpecialFrm success!\n" )
	.zero	5
.LC201:
	ASCII(.string	"%s call SYNTAX_ReleaseImage failed\n" )
	.zero	4
.LC205:
	ASCII(.string	"vfmw_ctrl.c, line %d: ChanID out of range\n" )
	.zero	5
.LC207:
	ASCII(.string	"vfmw_ctrl.c, line %d: Chan %d is not open or not running\n" )
	.zero	6
.LC210:
	ASCII(.string	"vfmw_ctrl.c, line %d: ChanID out of range is %d\n" )
	.zero	7
.LC214:
	ASCII(.string	"%s VidStd=%d Error!\n" )
	.zero	3
.LC217:
	ASCII(.string	"Get DecParam Type Failure!\n" )
	.zero	4
.LC221:
	ASCII(.string	"Chan %d is not open or not running\n" )
	.zero	4
.LC224:
	ASCII(.string	"ChanId out of range" )
	.zero	4
.LC229:
	ASCII(.string	"This channel is not running" )
	.zero	4
.LC235:
	ASCII(.string	"line: %d Flush DPB failed!\n" )
	.zero	4
.LC237:
	ASCII(.string	"VCTRL_FlushDecoder\n" )
	.zero	4
.LC240:
	ASCII(.string	"fuction return value is null,%s %d MVC_OK != MVC_FlushDPB!!\n" )
	.zero	3
.LC251:
	ASCII(.string	"chan not create" )
.LC253:
	ASCII(.string	"can NOT start a not-opened Chan" )
.LC255:
	ASCII(.string	"NULL == pArgs" )
	.zero	2
.LC270:
	ASCII(.string	"-1 == VCTRL_GetChanIDByCtx() Err! \n" )
	.zero	4
.LC275:
	ASCII(.string	"VCTRL_GetUsrdecFrame but VFMW_USER != s_pstVfmwChan[ChanID]->eVidStd!\n" )
	.zero	1
.LC277:
	ASCII(.string	"VCTRL_GetUsrdecFrame but s_pstVfmwChan[ChanID]->s32UsrdecFrameNum < MIN_USERDEC_FRAME_NUM!\n" )
	.zero	4
.LC279:
	ASCII(.string	"get frame store fail!\n" )
	.zero	1
.LC281:
	ASCII(.string	"get image buffer ok: LogicFsID = %d\n" )
	.zero	3
.LC283:
	ASCII(.string	"line: %d, pLf is NULL!\n" )
.LC285:
	ASCII(.string	"decode %p, disp %p, tf %p\n" )
	.zero	5
.LC287:
	ASCII(.string	"VCTRL_GetUsrdecFrame get LogicFs fail!\n" )
.LC295:
	ASCII(.string	"ERROR: s32MaxRefFrameNum(%d), extra_ref(%d), s32DisplayFrameNum(%d), extra_disp(%d)\n" )
	.zero	3
.LC297:
	ASCII(.string	"ERROR: (MaxRefFrameNum(%d) + extra_ref(%d)) > 16\n" )
	.zero	6
.LC299:
	ASCII(.string	"CAP_LEVEL_USER_DEFINE_WITH_OPTION option is illegal.\n" )
	.zero	2
.LC302:
	ASCII(.string	"VCTRL_GetChanMemSizeByOption L%d: arrange return failure\n" )
	.zero	6
.LC308:
	ASCII(.string	"ERROR: (nPmv(%d) + extra_ref(%d)) <= 0\n" )
.LC310:
	ASCII(.string	"ERROR: TempMinFrame(%d + %d + %d), TempMiddleFrame(%d + %d + %d), TempFluentFrame(%d + %d + %d)\n" )
	.zero	7
.LC313:
	ASCII(.string	"VCTRL_GetChanMemSize L%d: arrange return failure\n" )
	.zero	6
.LC321:
	ASCII(.string	"MEM_MapRegisterAddr reg 0x%x failed!\n" )
	.zero	2
.LC326:
	ASCII(.string	"VDMHAL_OpenHAL Vdh %d failed!\n" )
	.zero	1
.LC328:
	ASCII(.string	"VDMHAL_OpenHAL Scd %d failed!\n" )
	.zero	1
.LC330:
	ASCII(.string	"VDMHAL_GetHalMemSize Vdh %d failed!\n" )
	.zero	3
.LC332:
	ASCII(.string	"MEM_AllocMemBlock Vdh %d failed!\n" )
	.zero	6
.LC334:
	ASCII(.string	"MEM_AllocMemBlock VFMW_VP9Prob %d failed!\n" )
	.zero	5
.LC336:
	ASCII(.string	"SM_OpenSCDDrv Scd %d failed!\n" )
	.zero	2
.LC338:
	ASCII(.string	"MEM_AllocMemBlock Scd %d failed!\n" )
	.zero	6
.LC341:
	ASCII(.string	"Register irq %d failed!\n" )
	.zero	7
.LC350:
	ASCII(.string	"extra_ref=" )
	.zero	5
.LC352:
	ASCII(.string	"=" )
	.zero	6
.LC354:
	ASCII(.string	" " )
	.zero	6
.LC357:
	ASCII(.string	"fuction return value is null,%s %d unknow error!!\n" )
	.zero	5
.LC359:
	ASCII(.string	"ERROR: extra_ref = %d is out of [ -5000, 5000 ]!\n" )
	.zero	6
.LC361:
	ASCII(.string	"extra_disp=" )
	.zero	4
.LC363:
	ASCII(.string	"ERROR: extra_disp = %d is out of [ -5000, 5000 ]!\n" )
	.zero	5
.LC366:
	ASCII(.string	"extra_cmd=" )
	.zero	5
.LC373:
	ASCII(.string	"HI_VFMW_VideoDecode" )
	.zero	4
.LC377:
	ASCII(.string	"(safe,warn,danger) :amount = %d,%d,%d,%d\n" )
	.zero	6
.LC379:
	ASCII(.string	"strm_ctrl: I->IPB\n" )
	.zero	5
.LC381:
	ASCII(.string	"strm_ctrl: IPB->I\n" )
	.zero	5
.LC383:
	ASCII(.string	"(CurrMode,NextMode) = %d,%d\n" )
	.zero	3
.LC392:
	ASCII(.string	"Syntax is stopped from This channel" )
	.zero	4
.LC397:
	ASCII(.string	"[%d] vdm busy\n" )
	.zero	1
.LC399:
	ASCII(.string	"[%d] no frame\n" )
	.zero	1
.LC401:
	ASCII(.string	"[%d] no stream\n" )
.LC403:
	ASCII(.string	"seg_pts: %lld, seg_usertag = %lld\n" )
	.zero	5
.LC405:
	ASCII(.string	"pic_pts: %lld\n" )
	.zero	1
.LC407:
	ASCII(.string	"pic_usertag: %lld\n" )
	.zero	5
.LC409:
	ASCII(.string	"pic_pts: %lld pic_usertag = %lld\n" )
	.zero	6
.LC412:
	ASCII(.string	"%s:ChanId %d this hevc file is 10bit!!!\n" )
	.zero	7
.LC415:
	ASCII(.string	"DFS, Wait Partition\n" )
	.zero	3
.LC417:
	ASCII(.string	"DFS, Partition fail, u32DynamicFrameStoreAllocEn: %d, eFspPartitionState: %d\n" )
	.zero	2
.LC430:
	ASCII(.string	"Thread running...\n" )
	.zero	5
.LC432:
	ASCII(.string	"Thread stopped!\n" )
	.zero	7
.LC434:
	ASCII(.string	"thread running...\n" )
	.zero	5
.LC436:
	ASCII(.string	"thread stopped...\n" )
	.zero	5
.LC439:
	ASCII(.string	"Thread killed!\n" )
.LC446:
	ASCII(.string	"ERROR:ReadNum = %d larger than VFMW_MAX_RESERVE_NUM(%d)\n" )
	.zero	7
.LC450:
	ASCII(.string	"%s,%d,specialFrmRec[%d]->PhyAddr = 0x%x,VirAddr = 0x%x,Length = %d\n" )
	.zero	4
.LC455:
	ASCII(.string	"pSpecialFrmInfo = null!!!" )
	.zero	6
.LC465:
	ASCII(.string	"ChanID out of range!" )
	.zero	3
.LC484:
	ASCII(.string	"SetVdecExtra ERROR: extra_ref = %d is out of [ -5000, 5000 ]!\n" )
	.zero	1
.LC486:
	ASCII(.string	"SetVdecExtra ERROR: extra_disp = %d is out of [ -5000, 5000 ]!\n" )
.LC494:
	ASCII(.string	"%s 1 The channel %d is not active\n" )
	.zero	5
.LC496:
	ASCII(.string	"The channel %d is deced\n" )
	.zero	7
.LC498:
	ASCII(.string	"The pDecParam of channel %d is NULL\n" )
	.zero	3
.LC502:
	ASCII(.string	"%s 2 The channel %d is not active\n" )
	.zero	5
.LC518:
	ASCII(.string	"Chan %d Report Last Frame, Value=%d, LastFrameState=%d\n" )
.LC522:
	ASCII(.string	"Last frame output failed!\n" )
	.zero	5
.LC524:
	ASCII(.string	"Last frame output successed!\n" )
	.zero	2
.LC526:
	ASCII(.string	"Last frame already output, image id: %d!\n" )
	.zero	6
.LC539:
	ASCII(.string	"line %d can not know YuvFormat = %d\n" )
	.zero	3
.LC541:
	ASCII(.string	"userdec frame(phy=%#x) address error!\n" )
	.zero	1
.LC543:
	ASCII(.string	"can not map vir addr for usrdat frame(cbphy=%#x)\n" )
	.zero	6
.LC545:
	ASCII(.string	"line: %d, pLogic is Null!\n" )
	.zero	5
.LC547:
	ASCII(.string	"eYuvFormat %d is not support!!\n" )
.LC553:
	ASCII(.string	"pUsrdecFrame == NULL" )
	.zero	3
.LC555:
	ASCII(.string	"VCTRL_PutUsrdecFrame: VidStd is %d\n" )
	.zero	4
.LC558:
	ASCII(.string	"VCTRL_PutUsrdecFrame return err\n" )
	.zero	7
.LC560:
	ASCII(.string	"L:%d ChanID out of range\n" )
	.zero	6
.LC563:
	ASCII(.string	"L:%d This Chan has not been created!!!\n" )
.LC565:
	ASCII(.string	"L:%d WARNING: vidio std not setted!\n" )
	.zero	3
.LC575:
	ASCII(.string	"WARNING: vidio std not setted!" )
	.zero	1
.LC587:
	ASCII(.string	"This Chan has not been created or has been destroyed !" )
	.zero	1
.LC590:
	ASCII(.string	"VCTRL_StopChanWithCheck fail!\n" )
	.zero	1
.LC598:
	ASCII(.string	"-------------------------------------------------------------------------\n" )
	.zero	5
.LC605:
	ASCII(.string	"line %d VCTRL_GetChanMemSizeWithOption failed!\n" )
.LC607:
	ASCII(.string	"L: %d : part we can NOT alloc buffer for channel context\n" )
	.zero	6
.LC609:
	ASCII(.string	"L: %d : can NOT alloc buffer for channel context\n" )
	.zero	6
.LC611:
	ASCII(.string	"L: %d : can NOT alloc buffer for channel vdh\n" )
	.zero	2
.LC613:
	ASCII(.string	"L: %d : can NOT alloc buffer for channel vdh, %#x < %#x\n" )
	.zero	7
.LC615:
	ASCII(.string	"L: %d : can NOT alloc buffer for channel scd\n" )
	.zero	2
.LC617:
	ASCII(.string	"L: %d : can NOT alloc buffer for SCD\n" )
	.zero	2
.LC620:
	ASCII(.string	"can NOT enable FSP instance\n" )
	.zero	3
.LC622:
	ASCII(.string	"vfmw have max(%d) channels opened, can NOT create more channels\n" )
	.zero	7
.LC627:
	ASCII(.string	"VCTRL_CreateChan eCapLevel = %d error! Try to use VCTRL_CreateChanWithOption(CAP_LEVEL_USER_DEFINE_WITH_OPTION, ...)\n" )
	.zero	2
.LC632:
	ASCII(.string	"VCTRL_GetChanMemSize ERROR\n" )
	.zero	4
.LC634:
	ASCII(.string	"channel memory have been allocated outside, but the mem-info is not correct!\n" )
	.zero	2
.LC636:
	ASCII(.string	"mem-info(phy, vir, size) = (%#x, %x, %d)\n" )
	.zero	6
.LC638:
	ASCII(.string	"this channel totally need %d bytes memory\n" )
	.zero	5
.LC645:
	ASCII(.string	"VCTRL_CheckChanWH return err.\n" )
	.zero	1
.LC648:
	ASCII(.string	"VFMW didn't support std %d.\n" )
	.zero	3
.LC650:
	ASCII(.string	"VCTRL_ConfigChan with same config paramater.\n" )
	.zero	2
.LC653:
	ASCII(.string	"can NOT VCTRL_PartitionMemForStdUser\n" )
	.zero	2
.LC660:
	ASCII(.string	"chan %d: new ChanErrThr = %d\n" )
	.zero	2
.LC663:
	ASCII(.string	"chan %d: new DecOrderOutput = %d\n" )
	.zero	6
.LC665:
	ASCII(.string	"chan %d: new DecMode = %d\n" )
	.zero	5
.LC667:
	ASCII(.string	"chan %d: GetChanCfg failed!\n" )
	.zero	3
.LC669:
	ASCII(.string	"chan %d: new StrmOFThr = %d\n" )
	.zero	3
.LC672:
	ASCII(.string	"USE_FF_APT_EN = %d\n" )
	.zero	4
.LC674:
	ASCII(.string	"USE_FF_APT_EN can NOT be set to %d, must be 0 or 1\n" )
	.zero	4
.LC677:
	ASCII(.string	"ResetCreateNewSave = %d\n" )
	.zero	7
.LC679:
	ASCII(.string	"ResetCreateNewSave can NOT be set to %d, must be 0 or 1\n" )
	.zero	7
.LC681:
	ASCII(.string	"NO SUCH CHANNEL : %d\n" )
	.zero	2
.LC683:
	ASCII(.string	"Pay attention to  CHANNEL : %d\n" )
.LC685:
	ASCII(.string	"print memory tracer...\n" )
.LC688:
	ASCII(.string	"OK create file '%s' for channel %d raw stream save\n" )
	.zero	4
.LC690:
	ASCII(.string	"failed create file '%s' for raw stream save!\n" )
	.zero	2
.LC692:
	ASCII(.string	">>>> close raw stream saving >>>>\n" )
	.zero	5
.LC694:
	ASCII(.string	"OK create file '%s' for channel %d seg stream save\n" )
	.zero	4
.LC696:
	ASCII(.string	"failed create file '%s' for seg stream save!\n" )
	.zero	2
.LC698:
	ASCII(.string	">>>> close seg stream saving >>>>\n" )
	.zero	5
.LC700:
	ASCII(.string	"Failed Alloc VFMW_BigTile1d_YUV for yuv save!\n" )
	.zero	1
.LC702:
	ASCII(.string	"OK create file '%s' for chan %d yuv save.(crop %d)\n" )
	.zero	4
.LC704:
	ASCII(.string	"failed create file '%s' for yuv save!\n" )
	.zero	1
.LC706:
	ASCII(.string	"OK close yuv save.\n" )
	.zero	4
.LC708:
	ASCII(.string	"address '0x%x' is not valid!\n" )
	.zero	2
.LC710:
	ASCII(.string	"2d image has been saved to '%s'\n" )
	.zero	7
.LC712:
	ASCII(.string	"open file '%s' fail!\n" )
	.zero	2
.LC716:
	ASCII(.string	"1d image has been saved to '%s'\n" )
	.zero	7
.LC718:
	ASCII(.string	"OK create file '%s' for channel %d metadata stream save\n" )
	.zero	7
.LC720:
	ASCII(.string	"failed create file '%s' for metadata stream save!\n" )
	.zero	5
.LC722:
	ASCII(.string	">>>> close metadata stream saving >>>>\n" )
.LC724:
	ASCII(.string	"OK create file '%s' for channel %d metadata_reorder stream save\n" )
	.zero	7
.LC726:
	ASCII(.string	"failed create file '%s' for metadata_reorder stream save!\n" )
	.zero	5
.LC728:
	ASCII(.string	"%s/vfmw_el_save_%d.dat" )
	.zero	1
.LC730:
	ASCII(.string	"OK create file '%s' for channel %d el stream save\n" )
	.zero	5
.LC732:
	ASCII(.string	"failed create file '%s' for el stream save!\n" )
	.zero	3
.LC734:
	ASCII(.string	">>>> close el stream saving >>>>\n" )
	.zero	6
.LC736:
	ASCII(.string	"stop chan %d: VCTRL_StopChan return %d\n" )
.LC738:
	ASCII(.string	"reset chan %d: VCTRL_ResetChan return %d\n" )
	.zero	6
.LC740:
	ASCII(.string	"start chan %d: VCTRL_StartChan return %d\n" )
	.zero	6
.LC743:
	ASCII(.string	"------ dec task delay %d ms ------\n" )
	.zero	4
.LC745:
	ASCII(.string	"------ dnr active per %d ms ------\n" )
	.zero	4
.LC747:
	ASCII(.string	"------ dec stop = %d ------\n" )
	.zero	3
.LC750:
	ASCII(.string	"------ g_TraceCtrl = %x ------\n" )
.LC753:
	ASCII(.string	"------ g_TraceBsPeriod = %x ------\n" )
	.zero	4
.LC756:
	ASCII(.string	"------ g_TraceFramePeriod = %x ------\n" )
	.zero	1
.LC759:
	ASCII(.string	"------ g_TraceImgPeriod = %x ------\n" )
	.zero	3
.LC761:
	ASCII(.string	"The channel number is to Large.\n" )
	.zero	7
.LC764:
	ASCII(.string	"------ s_pstVfmwChan[%d]->stSynExtraData.s32ModuleLowlyEnable = 1 ------\n" )
	.zero	6
.LC768:
	ASCII(.string	"------ Set SCD[%d] to work  ------\n" )
	.zero	4
.LC770:
	ASCII(.string	"unmask vfmw mmu err int!\n" )
	.zero	6
.LC772:
	ASCII(.string	"mask vfmw mmu err int!\n" )
.LC784:
	ASCII(.string	"vdm_ctrl.c,L%d: pOption->s32KeepBS error\n" )
	.zero	6
.LC794:
	ASCII(.string	"vdm_ctrl.c,L%d: %s\n" )
	.zero	4
.LC797:
	ASCII(.string	"This Chan has not been created or has been destroyed !!!" )
	.zero	7
.LC799:
	ASCII(.string	"can NOT stop a not-opened Chan" )
	.zero	1
.LC815:
	ASCII(.string	"WARNNING: dsp in lowdelay state, we can not supported  avs in the same time!!!\n" )
.LC817:
	ASCII(.string	"WARNNING: dsp in avs state, we can not supported  lowdelay in the same time!!!\n" )
.LC831:
	ASCII(.string	"DFS, request frame store fail!, chanid: %d\n" )
	.zero	4
.LC833:
	ASCII(.string	"DFS, set decoding thread state to success!\n " )
	.zero	3
.LC836:
	ASCII(.string	"%s set chan %d ctrl state CHAN_FRAME_STORE_ALLOC failed!\n" )
	.zero	6
.LC839:
	ASCII(.string	"VFMW: WriteBigTileYuv %dx%d...\n" )
.LC843:
	ASCII(.string	"call VCTRL_GetChanImage, ChanID = %d,\n" )
	.zero	1
.LC852:
	ASCII(.string	"%s,%d,ImgOutputEn = %d, should not be output!\n" )
	.zero	1
.LC855:
	ASCII(.string	"%s: view_0 id(%d) > view_1 id(%d), error!\n" )
	.zero	5
.LC857:
	ASCII(.string	"%s: continuous view0 id(%d)!\n" )
	.zero	2
.LC859:
	ASCII(.string	"%s: continuous view1 id(%d)!\n" )
	.bss
	.align	3
.LANCHOR0 = . + 0
.LANCHOR3 = . + 4352
.LANCHOR5 = . + 13056
	.type	s_pstVfmwChan, %object
	.size	s_pstVfmwChan, 128
s_pstVfmwChan:
	.zero	128
	.type	s_VfmwCTrl, %object
	.size	s_VfmwCTrl, 352
s_VfmwCTrl:
	.zero	352
	.type	s_StreamIntf, %object
	.size	s_StreamIntf, 384
s_StreamIntf:
	.zero	384
	.type	FspInstInfo.52183, %object
	.size	FspInstInfo.52183, 3212
FspInstInfo.52183:
	.zero	3212
	.type	g_VdmCharacter, %object
	.size	g_VdmCharacter, 4
g_VdmCharacter:
	.zero	4
	.type	last_rec_pos_time.52467, %object
	.size	last_rec_pos_time.52467, 64
last_rec_pos_time.52467:
	.zero	64
	.type	extra_ref, %object
	.size	extra_ref, 4
extra_ref:
	.zero	4
	.type	extra_disp, %object
	.size	extra_disp, 4
extra_disp:
	.zero	4
	.type	MemArrange.52677, %object
	.size	MemArrange.52677, 1272
MemArrange.52677:
	.zero	1272
	.type	MemArrange.52711, %object
	.size	MemArrange.52711, 1272
MemArrange.52711:
	.zero	1272
	.type	cmdline, %object
	.size	cmdline, 1024
cmdline:
	.zero	1024
	.type	extra_cmd, %object
	.size	extra_cmd, 4
extra_cmd:
	.zero	4
	.type	g_allow_B_before_P, %object
	.size	g_allow_B_before_P, 1
g_allow_B_before_P:
	.zero	1
	.type	g_not_allow_H264FullPictureRepair_flag, %object
	.size	g_not_allow_H264FullPictureRepair_flag, 1
g_not_allow_H264FullPictureRepair_flag:
	.zero	1
	.type	g_apple_airplay_mirrorinig_flag, %object
	.size	g_apple_airplay_mirrorinig_flag, 1
g_apple_airplay_mirrorinig_flag:
	.zero	1
	.zero	1
	.type	g_event_report, %object
	.size	g_event_report, 8
g_event_report:
	.zero	8
	.type	g_DSPState, %object
	.size	g_DSPState, 64
g_DSPState:
	.zero	64
	.type	last_rec_pos_time.52816, %object
	.size	last_rec_pos_time.52816, 64
last_rec_pos_time.52816:
	.zero	64
	.type	last_rec_pos_decparam_time.52817, %object
	.size	last_rec_pos_decparam_time.52817, 64
last_rec_pos_decparam_time.52817:
	.zero	64
	.type	ChanPriTab.52777, %object
	.size	ChanPriTab.52777, 64
ChanPriTab.52777:
	.zero	64
	.type	g_SleepTime, %object
	.size	g_SleepTime, 4
g_SleepTime:
	.zero	4
	.type	g_StopDec, %object
	.size	g_StopDec, 4
g_StopDec:
	.zero	4
	.type	g_VfmwChanStat, %object
	.size	g_VfmwChanStat, 4800
g_VfmwChanStat:
	.zero	4800
	.type	AcceleratorCharacter, %object
	.size	AcceleratorCharacter, 8
AcceleratorCharacter:
	.zero	8
	.type	PrioArray.52283, %object
	.size	PrioArray.52283, 64
PrioArray.52283:
	.zero	64
	.type	g_ResetCreateNewSave, %object
	.size	g_ResetCreateNewSave, 4
g_ResetCreateNewSave:
	.zero	4
	.zero	4
	.type	vfmw_save_str_file, %object
	.size	vfmw_save_str_file, 256
vfmw_save_str_file:
	.zero	256
	.type	pOption, %object
	.size	pOption, 8
pOption:
	.zero	8
	.type	s_ChanControlState, %object
	.size	s_ChanControlState, 64
s_ChanControlState:
	.zero	64
	.type	Capability.52327, %object
	.size	Capability.52327, 148
Capability.52327:
	.zero	148
	.zero	4
	.type	PrioArray.52326, %object
	.size	PrioArray.52326, 64
PrioArray.52326:
	.zero	64
	.type	g_CurProcChan, %object
	.size	g_CurProcChan, 4
g_CurProcChan:
	.zero	4
	.type	save_file_cnt.52941, %object
	.size	save_file_cnt.52941, 4
save_file_cnt.52941:
	.zero	4
	.type	save_str_chan_num, %object
	.size	save_str_chan_num, 4
save_str_chan_num:
	.zero	4
	.zero	4
	.type	vfmw_save_seg_file, %object
	.size	vfmw_save_seg_file, 256
vfmw_save_seg_file:
	.zero	256
	.type	save_file_cnt.52944, %object
	.size	save_file_cnt.52944, 4
save_file_cnt.52944:
	.zero	4
	.type	save_seg_chan_num, %object
	.size	save_seg_chan_num, 4
save_seg_chan_num:
	.zero	4
	.type	vfmw_save_yuv_file, %object
	.size	vfmw_save_yuv_file, 256
vfmw_save_yuv_file:
	.zero	256
	.type	save_file_cnt.52949, %object
	.size	save_file_cnt.52949, 4
save_file_cnt.52949:
	.zero	4
	.zero	4
	.type	MemRecord_YUV, %object
	.size	MemRecord_YUV, 32
MemRecord_YUV:
	.zero	32
	.type	pVl, %object
	.size	pVl, 8
pVl:
	.zero	8
	.type	pUl, %object
	.size	pUl, 8
pUl:
	.zero	8
	.type	pY, %object
	.size	pY, 8
pY:
	.zero	8
	.type	save_yuv_with_crop, %object
	.size	save_yuv_with_crop, 4
save_yuv_with_crop:
	.zero	4
	.zero	4
	.type	vfmw_save_metadata_file, %object
	.size	vfmw_save_metadata_file, 256
vfmw_save_metadata_file:
	.zero	256
	.type	save_file_cnt.52962, %object
	.size	save_file_cnt.52962, 4
save_file_cnt.52962:
	.zero	4
	.type	save_metadata_chan_num, %object
	.size	save_metadata_chan_num, 4
save_metadata_chan_num:
	.zero	4
	.type	vfmw_save_metadata_reorder_file, %object
	.size	vfmw_save_metadata_reorder_file, 256
vfmw_save_metadata_reorder_file:
	.zero	256
	.type	save_file_cnt.52965, %object
	.size	save_file_cnt.52965, 4
save_file_cnt.52965:
	.zero	4
	.type	save_metadata_reorder_chan_num, %object
	.size	save_metadata_reorder_chan_num, 4
save_metadata_reorder_chan_num:
	.zero	4
	.type	vfmw_save_el_file, %object
	.size	vfmw_save_el_file, 256
vfmw_save_el_file:
	.zero	256
	.type	save_file_cnt.52968, %object
	.size	save_file_cnt.52968, 4
save_file_cnt.52968:
	.zero	4
	.type	save_el_chan_num, %object
	.size	save_el_chan_num, 4
save_el_chan_num:
	.zero	4
	.type	g_DnrSleepTime, %object
	.size	g_DnrSleepTime, 4
g_DnrSleepTime:
	.zero	4
	.type	g_SCDID, %object
	.size	g_SCDID, 4
g_SCDID:
	.zero	4
	.type	last_rec_pos_time.52457, %object
	.size	last_rec_pos_time.52457, 64
last_rec_pos_time.52457:
	.zero	64
	.type	g_EsBufValidFlag, %object
	.size	g_EsBufValidFlag, 64
g_EsBufValidFlag:
	.zero	64
	.type	g_DnrIrqHandle, %object
	.size	g_DnrIrqHandle, 4
g_DnrIrqHandle:
	.zero	4
	.type	g_VdecIrqHandle, %object
	.size	g_VdecIrqHandle, 4
g_VdecIrqHandle:
	.zero	4
	.type	g_u32DFSMaxAllocTime, %object
	.size	g_u32DFSMaxAllocTime, 4
g_u32DFSMaxAllocTime:
	.zero	4
	.type	g_eVdmVersion, %object
	.size	g_eVdmVersion, 4
g_eVdmVersion:
	.zero	4
	.type	g_VfmwGlobalStat, %object
	.size	g_VfmwGlobalStat, 96
g_VfmwGlobalStat:
	.zero	96
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
