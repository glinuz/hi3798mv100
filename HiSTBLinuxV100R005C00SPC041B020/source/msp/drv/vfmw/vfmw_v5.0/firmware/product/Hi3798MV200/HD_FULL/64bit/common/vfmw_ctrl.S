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
	str	w20, [x0, 1320]
	mov	w0, 0
.L122:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L125:
	str	wzr, [x0, 1320]
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
	add	x2, x0, 323584
	mov	w0, 0
	str	w20, [x2, 2660]
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
	add	x0, x0, 323584
	ldr	w0, [x0, 2660]
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
	mov	x2, 34504
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
	add	x19, x0, 1712
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
	mov	x3, 34504
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
	add	x0, x0, 1584
	ldr	x3, [x3, 104]
	blr	x3
	ldr	x1, [x21, w19, sxtw 3]
	ldr	w0, [x1, 1584]
	sub	w0, w0, #1
	cmp	w0, 1
	bls	.L203
	mov	w0, 0
	str	wzr, [x1, 1584]
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
	add	x0, x0, 327680
	add	x0, x0, 2752
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
	mov	w0, 44584
	movk	w0, 0xc, lsl 16
	str	w0, [x20]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L284:
	mov	w0, 34744
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
	ldr	w3, [x0, 1388]
	cmp	w3, w2
	bgt	.L313
	ldr	x1, .LC165
	ldr	w3, [x0, 1392]
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
	add	x0, x3, 327680
	ldr	w1, [x0, 2704]
	cbz	w1, .L359
	ldr	w0, [x0, 2196]
	mov	w2, 0
	cmp	w22, w0
	bne	.L341
	b	.L364
	.p2align 3
.L342:
	add	x0, x3, x0, lsl 4
	ldr	w0, [x0, 2196]
	cmp	w22, w0
	beq	.L339
.L341:
	add	w2, w2, 1
	cmp	w1, w2
	mov	w21, w2
	sxtw	x0, w2
	add	x0, x0, 20480
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
	add	x0, x3, 294912
	ldrsb	w4, [x0, 2234]
	cmp	w4, wzr
	ble	.L359
	add	x0, x3, 315392
	mov	w19, 0
	add	x0, x0, 3776
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
	add	x1, x19, 315392
	add	x19, x19, 319488
	strb	wzr, [x1, 3662]
	strb	wzr, [x19, 2766]
	blr	x2
	cmp	w22, w21
	bls	.L353
	sxtw	x25, w20
	uxtw	x19, w21
	.p2align 2
.L347:
	add	w21, w21, 1
	add	x19, x19, 20480
	add	x0, x19, 137
	ldr	x3, [x23, x25, lsl 3]
	uxtw	x19, w21
	ldr	x4, [x24, 104]
	add	x1, x19, 20480
	mov	x2, 16
	add	x1, x1, 137
	add	x0, x3, x0, lsl 4
	add	x1, x3, x1, lsl 4
	blr	x4
	cmp	w21, w22
	bcc	.L347
.L346:
	uxtw	x0, w22
	ldr	x4, [x23, w20, sxtw 3]
	add	x0, x0, 20480
	ldr	x3, [x24, 96]
	add	x0, x0, 137
	mov	x2, 16
	mov	w1, 0
	add	x0, x4, x0, lsl 4
	blr	x3
	ldr	x1, [x23, w20, sxtw 3]
	mov	w0, 0
	add	x1, x1, 327680
	ldr	w2, [x1, 2704]
	sub	w2, w2, #1
	str	w2, [x1, 2704]
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
	add	x0, x0, 327680
	add	x0, x0, 2752
	bl	SYNTAX_ReleaseImage
	mov	w23, w0
	ldr	w2, [x20, 196]
	cmn	w2, #1
	beq	.L375
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x29, 64
	str	w2, [x29, 220]
	add	x0, x0, 327680
	add	x0, x0, 2752
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
	add	x0, x19, 327680
	ldr	x20, [x0, 2768]
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
	add	x0, x19, 327680
	ldr	x20, [x0, 3376]
	b	.L421
	.p2align 3
.L410:
	add	x0, x19, 327680
	ldr	x20, [x0, 3800]
	b	.L421
	.p2align 3
.L419:
	add	x0, x19, 331776
	ldr	x20, [x0, 1048]
	b	.L421
	.p2align 3
.L418:
	add	x0, x19, 327680
	ldr	x20, [x0, 2992]
	b	.L421
	.p2align 3
.L413:
	add	x0, x19, 327680
	ldr	x20, [x0, 2784]
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
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L437
	cbz	w20, .L432
	cmp	w20, 16
	beq	.L433
	mov	w0, 0
.L431:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L433:
	ldr	w0, [x19, 5284]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	uxtb	w0, w0
	ret
	.p2align 3
.L432:
	ldrb	w0, [x19, 1284]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L437:
	ldr	x1, .LC218
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L431
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
	bhi	.L439
	ldr	x21, .LC219
	ldr	x0, [x21, w19, sxtw 3]
	cbz	x0, .L439
	ldr	w1, [x0, 12]
	cmp	w1, 1
	bne	.L442
	ldr	w22, [x0, 16]
	cmp	w22, 1
	beq	.L443
.L442:
	ldr	x1, .LC222
	mov	w2, w19
	mov	w0, 22
	mov	w20, 0
	bl	dprint_vfmw
.L441:
	mov	w0, w20
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L439:
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
.L443:
	mov	w0, w19
	bl	VCTRL_GetDecParam
	cbz	x0, .L448
	ldr	x0, [x21, w19, sxtw 3]
	mov	w20, 0
	str	wzr, [x0, 452]
	str	wzr, [x0, 456]
	b	.L441
	.p2align 3
.L448:
	ldr	x23, [x21, w19, sxtw 3]
	ldr	x0, .LC223
	ldr	w20, [x23, 452]
	cbz	w20, .L449
	ldr	x0, [x0]
	blr	x0
	ldr	x1, [x21, w19, sxtw 3]
	ldr	w2, [x1, 456]
	cmp	w0, w2
	bls	.L446
	sub	w0, w0, w2
	mov	w20, 0
	cmp	w0, 40
	bls	.L441
.L446:
	mov	w20, w22
	str	wzr, [x1, 452]
	str	wzr, [x1, 456]
	b	.L441
.L449:
	ldr	x0, [x0]
	str	w22, [x23, 452]
	blr	x0
	str	w0, [x23, 456]
	b	.L441
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
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	w21, w1
	bl	_mcount
	cmp	w19, 15
	bhi	.L460
	ldr	x20, .LC227
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L461
	ldr	w0, [x0, 16]
	cmp	w0, 1
	beq	.L454
	ldr	x3, .LC230
	mov	w2, 5392
	ldr	x1, .LC226
	mov	w0, 0
	bl	dprint_vfmw
.L450:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L454:
	mov	w0, w19
	bl	VCTRL_GetDecParam
	cbz	x0, .L450
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w1, [x0, 672]
	cbz	w1, .L455
	cmp	w1, 16
	bne	.L450
	add	x0, x0, 327680
	mov	w1, w21
	add	x0, x0, 2760
	bl	HEVCDEC_VDMProcSedOnly
	b	.L450
	.p2align 3
.L460:
	ldr	x3, .LC225
	mov	w2, 5390
	ldr	x1, .LC226
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L455:
	add	x0, x0, 327680
	mov	w1, w21
	add	x0, x0, 2760
	bl	H264DEC_VDMProcSedOnly
	b	.L450
	.p2align 3
.L461:
	ldr	x3, .LC228
	mov	w2, 5391
	ldr	x1, .LC226
	bl	dprint_vfmw
	b	.L450
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
	cbz	x0, .L489
	add	x0, x0, 327680
	ldr	w1, [x0, 2752]
	cmp	w1, 17
	bls	.L490
.L482:
	mov	w0, 0
.L464:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L490:
	ldr	x2, .LC234
	ldrb	w1, [x2,w1,uxtw]
	adr	x2, .Lrtx466
	add	x1, x2, w1, sxtb #2
	br	x1
.Lrtx466:
	.section	.rodata
	.align	0
	.align	2
.L466:
	.byte	(.L465 - .Lrtx466) / 4
	.byte	(.L467 - .Lrtx466) / 4
	.byte	(.L468 - .Lrtx466) / 4
	.byte	(.L469 - .Lrtx466) / 4
	.byte	(.L482 - .Lrtx466) / 4
	.byte	(.L470 - .Lrtx466) / 4
	.byte	(.L471 - .Lrtx466) / 4
	.byte	(.L482 - .Lrtx466) / 4
	.byte	(.L472 - .Lrtx466) / 4
	.byte	(.L473 - .Lrtx466) / 4
	.byte	(.L474 - .Lrtx466) / 4
	.byte	(.L474 - .Lrtx466) / 4
	.byte	(.L474 - .Lrtx466) / 4
	.byte	(.L475 - .Lrtx466) / 4
	.byte	(.L482 - .Lrtx466) / 4
	.byte	(.L476 - .Lrtx466) / 4
	.byte	(.L477 - .Lrtx466) / 4
	.byte	(.L478 - .Lrtx466) / 4
	.text
	.p2align 3
.L478:
	add	x0, x0, 2760
	bl	VP9DEC_GetRemainImg
	mov	w0, 0
	b	.L464
	.p2align 3
.L477:
	add	x0, x0, 2760
	bl	HEVC_FlushOutputFrmFromDPB
	cbnz	w0, .L491
.L480:
	ldr	x1, .LC238
	mov	w0, 32
	bl	dprint_vfmw
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	mov	w0, 0
	b	.L464
	.p2align 3
.L476:
	add	x0, x0, 2760
	bl	MVC_FlushDecoder
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	ldr	x0, [x19, w20, sxtw 3]
	mov	w1, -1
	add	x0, x0, 327680
	add	x0, x0, 2760
	bl	MVC_FlushDPB
	cbz	w0, .L482
	ldr	x2, .LC239
	mov	w3, 5481
	ldr	x1, .LC241
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 0
	b	.L464
	.p2align 3
.L475:
	add	x0, x0, 2760
	bl	VP8DEC_GetRemainImg
	mov	w0, 0
	b	.L464
	.p2align 3
.L474:
	add	x0, x0, 2760
	bl	VP6DEC_GetRemainImg
	mov	w0, 0
	b	.L464
	.p2align 3
.L473:
	add	x0, x0, 2760
	bl	REAL9DEC_GetRemainImg
	mov	w0, 0
	b	.L464
	.p2align 3
.L472:
	add	x0, x0, 2760
	bl	REAL8DEC_GetRemainImg
	mov	w0, 0
	b	.L464
	.p2align 3
.L471:
	add	x0, x0, 2760
	bl	AVSDEC_GetRemainImg
	mov	w0, 0
	b	.L464
	.p2align 3
.L470:
	add	x0, x0, 2760
	bl	DIVX3DEC_GetRemainImg
	mov	w0, 0
	b	.L464
	.p2align 3
.L469:
	add	x0, x0, 2760
	bl	MPEG2DEC_GetRemainImg
	mov	w0, 0
	b	.L464
	.p2align 3
.L468:
	add	x0, x0, 2760
	bl	MPEG4DEC_GetRemainImg
	mov	w0, 0
	b	.L464
	.p2align 3
.L467:
	add	x0, x0, 2760
	bl	VC1DEC_GetRemainImg
	mov	w0, 0
	b	.L464
	.p2align 3
.L465:
	add	x0, x0, 2760
	bl	H264_FlushDecoder
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	ldr	x0, [x19, w20, sxtw 3]
	add	x0, x0, 327680
	add	x0, x0, 2760
	bl	FlushDPB
	cbz	w0, .L482
	ldr	x1, .LC236
	mov	w2, 5448
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 0
	b	.L464
	.p2align 3
.L491:
	ldr	x1, .LC236
	mov	w2, 5458
	mov	w0, 0
	bl	dprint_vfmw
	b	.L480
.L489:
	ldr	x3, .LC232
	mov	w2, 5422
	ldr	x1, .LC233
	bl	dprint_vfmw
	mov	w0, -1
	b	.L464
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
	.xword	.L466
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
	bhi	.L515
	ldr	x3, .LC244
	ldr	x1, [x3, w19, sxtw 3]
	cbz	x1, .L516
	ldr	w0, [x1, 16]
	cmp	w0, 1
	bne	.L517
	ldr	w2, [x1, 672]
	cmp	w2, 17
	bls	.L518
.L511:
	mov	w0, 0
	b	.L494
	.p2align 3
.L518:
	ldr	x4, .LC247
	ldrb	w2, [x4,w2,uxtw]
	adr	x4, .Lrtx498
	add	x2, x4, w2, sxtb #2
	br	x2
.Lrtx498:
	.section	.rodata
	.align	0
	.align	2
.L498:
	.byte	(.L497 - .Lrtx498) / 4
	.byte	(.L499 - .Lrtx498) / 4
	.byte	(.L500 - .Lrtx498) / 4
	.byte	(.L501 - .Lrtx498) / 4
	.byte	(.L511 - .Lrtx498) / 4
	.byte	(.L502 - .Lrtx498) / 4
	.byte	(.L503 - .Lrtx498) / 4
	.byte	(.L511 - .Lrtx498) / 4
	.byte	(.L504 - .Lrtx498) / 4
	.byte	(.L505 - .Lrtx498) / 4
	.byte	(.L506 - .Lrtx498) / 4
	.byte	(.L506 - .Lrtx498) / 4
	.byte	(.L506 - .Lrtx498) / 4
	.byte	(.L507 - .Lrtx498) / 4
	.byte	(.L511 - .Lrtx498) / 4
	.byte	(.L508 - .Lrtx498) / 4
	.byte	(.L509 - .Lrtx498) / 4
	.byte	(.L510 - .Lrtx498) / 4
	.text
	.p2align 3
.L510:
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	VP9DEC_GetImageBuffer
.L494:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L509:
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	HEVCDEC_GetImageBuffer
	b	.L494
	.p2align 3
.L508:
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	MVC_DEC_GetImageBuffer
	b	.L494
	.p2align 3
.L507:
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	VP8DEC_GetImageBuffer
	b	.L494
	.p2align 3
.L506:
	ldr	w2, [x3, 4076]
	tbz	x2, 0, .L494
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	VP6DEC_GetImageBuffer
	b	.L494
	.p2align 3
.L505:
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	REAL9DEC_GetImageBuffer
	b	.L494
	.p2align 3
.L504:
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	REAL8DEC_GetImageBuffer
	b	.L494
	.p2align 3
.L503:
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	AVSDEC_GetImageBuffer
	b	.L494
	.p2align 3
.L502:
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	DIVX3DEC_GetImageBuffer
	b	.L494
	.p2align 3
.L501:
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	MPEG2DEC_GetImageBuffer
	b	.L494
	.p2align 3
.L500:
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	MPEG4DEC_GetImageBuffer
	b	.L494
	.p2align 3
.L499:
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	VC1DEC_GetImageBuffer
	b	.L494
	.p2align 3
.L497:
	add	x0, x1, 327680
	add	x0, x0, 2760
	bl	H264DEC_GetImageBuffer
	b	.L494
	.p2align 3
.L517:
	ldr	x3, .LC246
	mov	w2, 5546
	ldr	x1, .LC243
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L494
	.p2align 3
.L515:
	ldr	x3, .LC242
	mov	w2, 5544
	ldr	x1, .LC243
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L494
.L516:
	ldr	x3, .LC245
	mov	w2, 5545
	ldr	x1, .LC243
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L494
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
	.xword	.L498
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
	bhi	.L525
	ldr	x0, .LC250
	ldr	x2, [x0, w19, sxtw 3]
	cbz	x2, .L526
	ldr	w0, [x2, 12]
	cmp	w0, 1
	bne	.L527
	cbz	x20, .L528
	ldr	w1, [x20]
	mov	w0, 0
	str	w1, [x2, 1324]
.L521:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L527:
	ldr	x3, .LC254
	mov	w2, 5664
	ldr	x1, .LC249
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L521
	.p2align 3
.L525:
	ldr	x3, .LC248
	mov	w2, 5662
	ldr	x1, .LC249
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L521
.L526:
	ldr	x3, .LC252
	mov	w2, 5663
	ldr	x1, .LC249
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L521
.L528:
	ldr	x3, .LC256
	mov	w2, 5665
	ldr	x1, .LC249
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L521
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
	bhi	.L535
	ldr	x0, .LC259
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L536
	ldr	w1, [x0, 12]
	cmp	w1, 1
	bne	.L537
	cbz	x20, .L538
	ldr	x3, .LC263
	mov	x2, 20
	mov	x1, x20
	add	x0, x0, 1356
	ldr	x3, [x3, 104]
	blr	x3
	mov	w0, 0
.L531:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L537:
	ldr	x3, .LC261
	mov	w2, 5677
	ldr	x1, .LC258
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L531
	.p2align 3
.L535:
	ldr	x3, .LC257
	mov	w2, 5675
	ldr	x1, .LC258
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L531
.L538:
	ldr	x3, .LC262
	mov	w2, 5678
	ldr	x1, .LC258
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L531
.L536:
	ldr	x3, .LC260
	mov	w2, 5676
	ldr	x1, .LC258
	bl	dprint_vfmw
	mov	w0, -1
	b	.L531
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
	cbz	x0, .L542
	add	x1, x29, 40
	mov	w0, w19
	bl	SM_GetInfo
	cbnz	w0, .L542
	ldr	w0, [x29, 48]
	cmp	w0, 16
	bgt	.L543
	ldr	w0, [x29, 56]
	cmp	w0, 524288
	ble	.L542
.L543:
	mov	w0, w19
	bl	VCTRL_GetImageBuffer
	cmp	w0, 1
	csetm	w0, ne
	cbnz	w0, .L541
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
.L542:
	mov	w0, -1
.L541:
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
	bhi	.L552
	ldr	x0, .LC267
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L552
	ldr	w1, [x0, 12]
	cbz	w1, .L552
	ldr	w1, [x0, 16]
	cbz	w1, .L552
	ldr	w1, [x0, 28]
	cbz	w1, .L552
	ldr	w0, [x0, 672]
	ldr	x19, [sp, 16]
	cmp	w0, 21
	csetm	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L552:
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
.L556:
	ldr	x2, [x3, x0, lsl 3]
	add	x1, x2, 327680
	add	x1, x1, 2760
	cbz	x2, .L554
	cmp	x19, x1
	beq	.L555
.L554:
	add	x0, x0, 1
	cmp	x0, 16
	bne	.L556
	mov	w0, -1
	str	w0, [x19, 616]
.L559:
	ldr	x1, .LC271
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L555:
	cmn	w0, #1
	str	w0, [x19, 616]
	mov	w0, 1
	beq	.L559
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
	cbz	x19, .L571
	ldr	w0, [x19, 616]
	mov	w1, w20
	mov	w2, 0
	bl	FSP_SetDisplay
	ldr	w0, [x19, 616]
	mov	w1, w20
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L572
	ldr	w0, [x19, 616]
	ldr	x1, [x20, 560]
	bl	FreeUsdByDec
	str	xzr, [x20, 560]
	ldr	x1, [x20, 568]
	ldr	w0, [x19, 616]
	bl	FreeUsdByDec
	str	xzr, [x20, 568]
	mov	w0, 1
.L570:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L572:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L571:
	mov	w0, -2
	b	.L570
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
	cbz	x0, .L590
	ldr	w1, [x0, 672]
	cmp	w1, 20
	bne	.L591
	ldr	w0, [x0, 592]
	cmp	w0, 2
	ble	.L592
	mov	w1, 1
	mov	w0, w20
	bl	FSP_NewLogicFs
	mov	w22, w0
	tbnz	w0, #31, .L593
	mov	w1, w0
	mov	w0, w20
	bl	FSP_GetLogicFs
	mov	x19, x0
	ldr	x1, .LC282
	mov	w2, w22
	mov	w0, 18
	bl	dprint_vfmw
	cbz	x19, .L594
	ldr	x0, [x19, 640]
	cbz	x0, .L580
	ldr	x1, [x19, 648]
	cbz	x1, .L580
	ldr	x2, [x19, 656]
	cbz	x2, .L580
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
.L575:
	mov	w0, w1
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L591:
	ldr	x1, .LC276
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L575
	.p2align 3
.L592:
	ldr	x1, .LC278
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L575
	.p2align 3
.L580:
	ldr	x1, .LC288
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L575
	.p2align 3
.L593:
	ldr	x1, .LC280
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L575
.L590:
	ldr	x1, .LC274
	mov	w2, 6018
	ldr	x3, .LC273
	bl	dprint_vfmw
	mov	w1, -1
	b	.L575
.L594:
	ldr	x1, .LC284
	mov	w2, 6048
	mov	w0, 22
	bl	dprint_vfmw
	mov	w1, -1
	b	.L575
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
	cbz	x1, .L613
	ldr	w0, [x1, 672]
	cmp	w0, 20
	bls	.L614
.L595:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L614:
	ldr	x2, .LC292
	ldrb	w0, [x2,w0,uxtw]
	adr	x2, .Lrtx599
	add	x0, x2, w0, sxtb #2
	br	x0
.Lrtx599:
	.section	.rodata
	.align	0
	.align	2
.L599:
	.byte	(.L598 - .Lrtx599) / 4
	.byte	(.L600 - .Lrtx599) / 4
	.byte	(.L601 - .Lrtx599) / 4
	.byte	(.L602 - .Lrtx599) / 4
	.byte	(.L595 - .Lrtx599) / 4
	.byte	(.L603 - .Lrtx599) / 4
	.byte	(.L604 - .Lrtx599) / 4
	.byte	(.L595 - .Lrtx599) / 4
	.byte	(.L605 - .Lrtx599) / 4
	.byte	(.L606 - .Lrtx599) / 4
	.byte	(.L607 - .Lrtx599) / 4
	.byte	(.L607 - .Lrtx599) / 4
	.byte	(.L607 - .Lrtx599) / 4
	.byte	(.L608 - .Lrtx599) / 4
	.byte	(.L595 - .Lrtx599) / 4
	.byte	(.L609 - .Lrtx599) / 4
	.byte	(.L610 - .Lrtx599) / 4
	.byte	(.L611 - .Lrtx599) / 4
	.byte	(.L595 - .Lrtx599) / 4
	.byte	(.L595 - .Lrtx599) / 4
	.byte	(.L612 - .Lrtx599) / 4
	.text
	.p2align 3
.L612:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	USERDEC_Init
	b	.L595
	.p2align 3
.L611:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	VP9DEC_Init
	b	.L595
	.p2align 3
.L610:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	HEVCDEC_Init
	b	.L595
	.p2align 3
.L609:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	MVC_DEC_Init
	b	.L595
	.p2align 3
.L608:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	VP8DEC_Init
	b	.L595
	.p2align 3
.L607:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	VP6DEC_Init
	b	.L595
	.p2align 3
.L606:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	REAL9DEC_Init
	b	.L595
	.p2align 3
.L605:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	REAL8DEC_Init
	b	.L595
	.p2align 3
.L604:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	AVSDEC_Init
	b	.L595
	.p2align 3
.L603:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	DIVX3DEC_Init
	b	.L595
	.p2align 3
.L602:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	MPEG2DEC_Init
	b	.L595
	.p2align 3
.L601:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	MPEG4DEC_Init
	b	.L595
	.p2align 3
.L600:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	VC1DEC_Init
	b	.L595
	.p2align 3
.L598:
	add	x0, x1, 327680
	add	x1, x1, 680
	add	x0, x0, 2760
	bl	H264DEC_Init
	b	.L595
	.p2align 3
.L613:
	ldr	x3, .LC290
	mov	w2, 6103
	ldr	x1, .LC291
	mov	w0, 0
	bl	dprint_vfmw
	b	.L595
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
	.xword	.L599
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
	cbz	x0, .L617
	blr	x0
	mul	w0, w0, w20
	str	w0, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L617:
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
	mov	w1, 44584
	mov	w0, 34744
	cmp	w20, 26
	movk	w1, 0xc, lsl 16
	movk	w0, 0xb2, lsl 16
	csel	w22, w1, w0, ne
	cmp	w23, 2
	beq	.L649
	ldr	x0, .LC294
	ldr	w2, [x19, 36]
	ldr	w4, [x19, 60]
	ldp	w3, w0, [x0, -208]
	add	w5, w2, w3
	add	w23, w4, w0
	cmp	w5, wzr
	ccmp	w23, wzr, 1, ge
	blt	.L650
	cmp	w5, 16
	bgt	.L651
	str	w5, [x19, 36]
	str	w23, [x19, 60]
.L621:
	tbnz	w5, #31, .L626
.L625:
	ldr	w2, [x19, 12]
	cmp	w2, 31
	ble	.L626
	ldr	w3, [x19, 16]
	cmp	w3, 31
	ble	.L626
	ldr	w1, [x19, 56]
	mov	w0, 262143
	cmp	w1, w0
	ble	.L626
	ldr	w0, [x19, 44]
	cmp	w0, 1
	beq	.L652
	cmp	w0, 2
	beq	.L653
.L628:
	ldr	w0, [x19, 40]
	mov	w4, 1
	cbnz	w0, .L654
	ldr	x0, .LC301
	sub	w23, w20, #27
	mov	w7, -2
	cmp	w23, 5
	csinv	w7, w7, wzr, ls
	ldr	x8, [x0, 128]
	cbz	x8, .L632
.L656:
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
	cbnz	w0, .L632
	ldr	w0, [x24, -196]
	add	w1, w0, 16
	str	w1, [x21]
	ldr	w1, [x19, 176]
	cmp	w1, 1
	beq	.L633
	add	w0, w0, 307200
	add	w0, w0, 2576
	str	w0, [x21]
	ldr	w0, [x19, 56]
	add	w0, w0, 32768
	str	w0, [x21, 4]
.L634:
	str	w22, [x21, 8]
	mov	w0, 0
	ldr	w1, [x19, 44]
	cbnz	w1, .L655
.L635:
	cmp	w23, 5
	bhi	.L638
	mov	w1, 44808
	movk	w1, 0x13, lsl 16
	cmp	w0, w1
	csel	w0, w0, w1, cs
.L638:
	add	w1, w22, w0
	mov	w0, 0
	str	w1, [x21, 8]
.L623:
	add	sp, x29, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L654:
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
	cbnz	x8, .L656
.L632:
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
.L655:
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
	bne	.L635
	mov	w5, 3992
	mov	w1, 2240
	and	x0, x0, 4294967295
	smaddl	x2, w4, w5, x2
	smaddl	x3, w3, w1, x2
	cmp	x3, x0
	csel	x0, x3, x0, cs
	b	.L635
	.p2align 3
.L653:
	ldr	w0, [x19, 20]
	cmp	w0, wzr
	ble	.L626
	ldr	w0, [x19, 24]
	cmp	w0, wzr
	ble	.L626
.L648:
	ldr	w0, [x19, 28]
	cmp	w0, wzr
	ble	.L626
	ldr	w0, [x19, 32]
	cmp	w0, wzr
	bgt	.L628
.L626:
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
.L649:
	ldr	w5, [x19, 36]
	b	.L621
	.p2align 3
.L652:
	ldr	w0, [x19, 20]
	cmp	w0, wzr
	bgt	.L648
	b	.L626
	.p2align 3
.L633:
	ldr	w0, [x19, 56]
	add	w0, w0, 339968
	add	w0, w0, 2576
	str	w0, [x21, 4]
	b	.L634
	.p2align 3
.L651:
	ldr	x1, .LC298
	mov	w0, 32
	bl	dprint_vfmw
	str	w23, [x19, 60]
	mov	w5, 16
	str	w5, [x19, 36]
	b	.L625
	.p2align 3
.L650:
	ldr	x1, .LC296
	mov	w5, w0
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, -1
	b	.L623
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
	bhi	.L659
	ldr	x0, .LC306
	ldrb	w0, [x0,w1,uxtw]
	adr	x1, .Lrtx660
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx660:
	.section	.rodata
	.align	0
	.align	2
.L660:
	.byte	(.L673 - .Lrtx660) / 4
	.byte	(.L673 - .Lrtx660) / 4
	.byte	(.L673 - .Lrtx660) / 4
	.byte	(.L673 - .Lrtx660) / 4
	.byte	(.L673 - .Lrtx660) / 4
	.byte	(.L661 - .Lrtx660) / 4
	.byte	(.L661 - .Lrtx660) / 4
	.byte	(.L661 - .Lrtx660) / 4
	.text
	.p2align 3
.L673:
	mov	w0, 3
.L659:
	ldr	x19, .LC307
	sub	w0, w8, w0
	sub	w0, w0, #2
	ldr	w1, [x19, -208]
	tbnz	w1, #31, .L680
.L662:
	cmp	w0, 15
	mov	w6, 0
	bgt	.L663
	ldr	w6, [x19, -208]
	add	w0, w0, w6
	cmp	w0, 16
	bgt	.L681
.L663:
	add	w0, w6, w23
	ldr	w28, [x19, -204]
	cmp	w0, wzr
	ble	.L682
.L664:
	cmp	w0, 17
	mov	w23, 17
	csel	w23, w0, w23, le
.L665:
	cmp	w26, 23
	bhi	.L666
	add	w0, w6, w28
	add	w1, w8, w0
	add	w2, w27, w0
	cmp	w1, wzr
	add	w0, w22, w0
	ccmp	w2, wzr, 4, gt
	ccmp	w0, wzr, 4, gt
	ble	.L683
	mov	w22, w0
	mov	w27, w2
	mov	w8, w1
.L666:
	cmp	w26, 32
	bhi	.L676
	ldr	x28, .LC312
	ldr	x9, [x28, 128]
	cbz	x9, .L668
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
	cbnz	w0, .L668
	ldr	w0, [x19, 4]
	add	w0, w0, 307200
	add	w0, w0, 2576
	str	w0, [x21]
	ldr	w0, [x26, 24]
	add	w0, w0, 32768
	str	w0, [x20]
	ldr	x8, [x28, 128]
	cbz	x8, .L670
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
	cbnz	w0, .L670
	ldr	w0, [x19, 4]
	add	w0, w0, 307200
	add	w0, w0, 2576
	str	w0, [x21, 4]
	ldr	w0, [x26, 24]
	add	w0, w0, 32768
	str	w0, [x20, 4]
	ldr	x8, [x28, 128]
	cbz	x8, .L672
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
	cbnz	w0, .L672
	ldr	w1, [x19, 4]
	add	w1, w1, 307200
	add	w1, w1, 2576
	str	w1, [x21, 8]
	ldr	w1, [x26, 24]
	add	w1, w1, 32768
	str	w1, [x20, 8]
.L667:
	add	sp, x29, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L661:
	ldr	x19, .LC307
	mov	w0, 1
	sub	w0, w8, w0
	sub	w0, w0, #2
	ldr	w1, [x19, -208]
	tbz	w1, #31, .L662
.L680:
	str	wzr, [x19, -208]
	b	.L662
	.p2align 3
.L681:
	sub	w0, w6, w0
	ldr	w28, [x19, -204]
	add	w6, w0, 16
	add	w0, w6, w23
	cmp	w0, wzr
	bgt	.L664
.L682:
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
	b	.L665
	.p2align 3
.L683:
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
	b	.L666
	.p2align 3
.L668:
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
.L670:
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
.L672:
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
.L676:
	mov	w0, -1
	b	.L667
	.size	VCTRL_GetChanMemSize, .-VCTRL_GetChanMemSize
	.align	3
.LC305:
	.xword	CapItem
	.align	3
.LC306:
	.xword	.L660
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
	cbz	w0, .L709
	ldr	w0, [x19, 112]
	cmp	w0, wzr
	cset	w21, ne
.L685:
	ldr	w20, [x19, 24]
	ldr	w22, [x19, 16]
	ldr	x23, [x19, 8]
	cbz	w20, .L686
	ldr	x0, .LC316
	ldr	x1, [x0, 120]
	cbz	x1, .L738
	mov	w0, 0
	blr	x1
	cbz	w21, .L689
.L690:
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
.L686:
	mov	w2, w20
	mov	x1, x23
	mov	w0, w22
	bl	MEM_DelMemRecord
	ldr	w20, [x19, 88]
	ldr	w22, [x19, 80]
	ldr	x23, [x19, 72]
	cbz	w20, .L692
	ldr	x0, .LC316
	ldr	x1, [x0, 120]
	cbz	x1, .L739
	mov	w0, 0
	blr	x1
	cbz	w21, .L695
.L696:
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	add	x0, x19, 64
	ldr	x3, [x3, 96]
	blr	x3
.L692:
	mov	w2, w20
	mov	x1, x23
	mov	w0, w22
	bl	MEM_DelMemRecord
	ldr	w20, [x19, 56]
	ldr	w22, [x19, 48]
	ldr	x23, [x19, 40]
	cbnz	w20, .L740
.L698:
	mov	x1, x23
	mov	w2, w20
	mov	w0, w22
	bl	MEM_DelMemRecord
	bl	SM_InstInit
	ldr	w21, [x19, 144]
	ldr	w22, [x19, 136]
	ldr	x23, [x19, 128]
	cbnz	w21, .L741
.L701:
	mov	w2, w21
	mov	x1, x23
	mov	w0, w22
	add	x20, x19, 216
	bl	MEM_DelMemRecord
	ldr	w21, [x19, 240]
	ldr	w22, [x19, 232]
	ldr	x23, [x19, 224]
	cbnz	w21, .L742
.L702:
	mov	w2, w21
	mov	x1, x23
	mov	w0, w22
	bl	MEM_DelMemRecord
	ldr	w21, [x19, 208]
	ldr	w22, [x19, 200]
	ldr	x23, [x19, 192]
	cbnz	w21, .L743
.L703:
	mov	w2, w21
	mov	x1, x23
	mov	w0, w22
	add	x20, x19, 152
	bl	MEM_DelMemRecord
	ldr	w21, [x19, 176]
	ldr	w22, [x19, 168]
	ldr	x23, [x19, 160]
	cbnz	w21, .L744
.L704:
	mov	w2, w21
	mov	w0, w22
	mov	x1, x23
	add	x19, x19, 248
	bl	MEM_DelMemRecord
	ldr	w20, [x19, 24]
	ldr	w21, [x19, 16]
	ldr	x22, [x19, 8]
	cbnz	w20, .L745
	mov	w2, w20
	mov	x1, x22
	mov	w0, w21
	bl	MEM_DelMemRecord
	ldr	x19, .LC319
	ldr	w0, [x19, 456]
	cmp	w0, 1
	beq	.L746
.L706:
	ldr	w0, [x19, 460]
	cmp	w0, 1
	beq	.L747
.L707:
	ldr	w0, [x19, 464]
	add	x19, x19, 128
	cmp	w0, 1
	beq	.L748
.L708:
	mov	w0, 0
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L740:
	mov	w0, 0
	bl	SM_CloseSCDDrv
	cbz	w21, .L699
.L700:
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	add	x0, x19, x2
	ldr	x3, [x3, 96]
	blr	x3
	b	.L698
	.p2align 3
.L741:
	mov	x1, x23
	mov	w0, w22
	bl	MEM_UnmapRegisterAddr
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	add	x0, x19, 120
	ldr	x3, [x3, 96]
	blr	x3
	b	.L701
	.p2align 3
.L745:
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
	bne	.L706
.L746:
	ldr	x2, .LC317
	mov	w0, 127
	mov	x1, 0
	ldr	x2, [x2, 376]
	blr	x2
	str	wzr, [x19, 456]
	ldr	w0, [x19, 460]
	cmp	w0, 1
	bne	.L707
	.p2align 2
.L747:
	ldr	x2, .LC317
	mov	w0, 136
	mov	x1, 0
	add	x19, x19, 128
	ldr	x2, [x2, 376]
	blr	x2
	str	wzr, [x19, 332]
	ldr	w0, [x19, 336]
	cmp	w0, 1
	bne	.L708
.L748:
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
.L744:
	mov	x1, x23
	mov	w0, w22
	bl	MEM_UnmapRegisterAddr
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	mov	x0, x20
	ldr	x3, [x3, 96]
	blr	x3
	b	.L704
	.p2align 3
.L743:
	mov	x1, x23
	mov	w0, w22
	bl	MEM_UnmapRegisterAddr
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	add	x0, x19, 184
	ldr	x3, [x3, 96]
	blr	x3
	b	.L703
	.p2align 3
.L742:
	mov	x1, x23
	mov	w0, w22
	bl	MEM_UnmapRegisterAddr
	ldr	x3, .LC317
	mov	x2, 32
	mov	w1, 0
	mov	x0, x20
	ldr	x3, [x3, 96]
	blr	x3
	b	.L702
	.p2align 3
.L738:
	ldr	x1, .LC318
	mov	w2, 7098
	mov	w0, 0
	bl	dprint_vfmw
	cbnz	w21, .L690
	.p2align 2
.L689:
	ldr	w0, [x19, 16]
	ldr	x1, [x19, 8]
	bl	MEM_ReleaseMemBlock
	b	.L690
	.p2align 3
.L739:
	ldr	x1, .LC318
	mov	w2, 7119
	mov	w0, 0
	bl	dprint_vfmw
	cbnz	w21, .L696
	.p2align 2
.L695:
	ldr	w0, [x19, 80]
	ldr	x1, [x19, 72]
	bl	MEM_ReleaseMemBlock
	b	.L696
	.p2align 3
.L709:
	mov	w21, 0
	b	.L685
	.p2align 3
.L699:
	ldr	w0, [x19, 48]
	ldr	x1, [x19, 40]
	bl	MEM_ReleaseMemBlock
	b	.L700
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
	cbnz	w0, .L750
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
	cbz	w0, .L794
	mov	w2, 53248
	ldr	x1, .LC322
	movk	w2, 0xf8c3, lsl 16
.L789:
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x3, .LC323
	mov	x2, 32
	mov	w1, 0
	mov	x0, x20
.L790:
	ldr	x3, [x3, 96]
	blr	x3
	mov	x0, x19
	bl	CloseDrivers
	mov	w0, -1
.L753:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L794:
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
	cbnz	w0, .L754
	ldr	w0, [x19, 200]
	ldr	w2, [x19, 208]
	ldr	x1, [x19, 192]
	bl	MEM_AddMemRecord
	ldr	w0, [x19, 96]
	cbnz	w0, .L755
.L756:
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
	cbz	x0, .L757
	blr	x0
	cmp	w0, wzr
	mov	w2, w0
	ble	.L757
	mov	w21, 3
	mov	w4, 0
	mov	x3, x19
	mov	w1, 0
	add	x0, x29, 64
	str	w21, [x19, 28]
	bl	MEM_AllocMemBlock
	cbnz	w0, .L758
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
	cbz	x1, .L759
	add	x0, x29, 88
	blr	x1
	cbnz	w0, .L759
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
	cbnz	w0, .L760
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
	cbnz	w0, .L795
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
	cbnz	w0, .L788
.L766:
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
	cbnz	w0, .L796
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
	cbnz	w0, .L797
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
	cbnz	w0, .L798
	str	w22, [x21, 336]
	b	.L753
	.p2align 3
.L755:
	ldr	w1, [x19, 112]
	cbz	w1, .L756
	ldr	x22, .LC324
	ldr	x1, [x22, 96]
	cbz	x1, .L770
	blr	x1
	mov	w20, w0
	mov	w21, w0
	ldr	w0, [x19, 96]
.L763:
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
	cbz	x1, .L764
	add	x0, x29, 88
	blr	x1
	cbnz	w0, .L764
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
	cbnz	w0, .L765
	ldr	x20, .LC323
	b	.L766
	.p2align 3
.L750:
	ldr	x1, .LC322
	mov	w2, -121438208
	b	.L789
.L757:
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
	b	.L753
.L798:
	ldr	x1, .LC342
	mov	w2, 181
.L788:
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
	b	.L753
.L770:
	mov	w21, -1
	mov	w20, w21
	b	.L763
.L797:
	ldr	x1, .LC342
	mov	w2, 136
	b	.L788
.L796:
	ldr	x1, .LC342
	mov	w2, 127
	b	.L788
.L759:
	ldr	x1, .LC327
.L791:
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
	b	.L753
.L795:
	ldr	x1, .LC339
	mov	w2, 0
	b	.L788
.L758:
	ldr	x1, .LC333
	b	.L791
.L760:
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
	b	.L753
.L764:
	ldr	x1, .LC327
	mov	w2, 0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x3, .LC323
	mov	x2, 32
	mov	w1, 0
	mov	x0, x19
	b	.L790
.L754:
	ldr	x1, .LC322
	mov	w2, 49152
	movk	w2, 0xf8c3, lsl 16
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x3, .LC323
	mov	x2, 32
	mov	w1, 0
	mov	x0, x21
	b	.L790
.L765:
	ldr	x1, .LC329
	mov	w2, 0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x3, .LC323
	mov	x2, 32
	mov	w1, 0
	mov	x0, x23
	b	.L790
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
	cbnz	w19, .L830
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
	cbz	x0, .L802
	ldr	x1, .LC353
	add	x0, x29, 72
	ldr	x2, [x20, 184]
	blr	x2
	cbz	x0, .L822
	ldr	x1, .LC355
	add	x0, x29, 72
	ldr	x2, [x20, 184]
	blr	x2
	cbz	x0, .L831
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
	bhi	.L807
.L837:
	mov	w0, 5000
	mov	w1, -5000
	cmp	w2, w0
	csel	w0, w2, w0, le
	cmp	w0, w1
	csel	w0, w0, w1, ge
.L803:
	str	w0, [x21, -208]
.L805:
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
	cbz	x0, .L808
	ldr	x1, .LC353
	add	x0, x29, 72
	ldr	x2, [x20, 184]
	blr	x2
	cbz	x0, .L809
	ldr	x1, .LC355
	add	x0, x29, 72
	ldr	x2, [x20, 184]
	blr	x2
	cbz	x0, .L832
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
	bhi	.L813
.L836:
	mov	w19, 5000
	mov	w0, -5000
	cmp	w2, w19
	csel	w19, w2, w19, le
	cmp	w19, w0
	csel	w19, w19, w0, ge
.L809:
	str	w19, [x21, -204]
.L811:
	ldr	w0, [x21, 3368]
	str	xzr, [x29, 72]
	cbz	w0, .L814
	ldr	x2, .LC365
	and	w4, w0, 1
	ubfx	x3, x0, 1, 1
	ubfx	x1, x0, 2, 1
	strb	w4, [x21, 3372]
	ubfx	x0, x0, 3, 1
	strb	w3, [x2]
	strb	w1, [x21, 3373]
	strb	w0, [x21, 3374]
.L815:
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
.L819:
	str	w2, [x0], 4
	cmp	x1, x0
	bne	.L819
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
	cbnz	w0, .L830
	ldr	x2, [x20, 240]
	mov	w1, 1
	blr	x2
	ldr	x2, .LC372
	add	x0, x22, 144
	ldr	x1, .LC374
	ldr	x3, [x20, 360]
	blr	x3
	cbnz	w0, .L833
	mov	w1, 1
	str	w1, [x22, 128]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L814:
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
	cbz	x0, .L815
	ldr	x1, .LC353
	add	x0, x29, 72
	ldr	x2, [x20, 184]
	blr	x2
	cbz	x0, .L834
	ldr	x1, .LC355
	add	x0, x29, 72
	ldr	x2, [x20, 184]
	blr	x2
	cbz	x0, .L835
	ldr	x3, [x20, 168]
	mov	x1, 0
	mov	w2, 0
	blr	x3
	mov	w1, w0
	str	w0, [x21, 3368]
.L817:
	ldr	x2, .LC365
	and	w4, w1, 1
	ubfx	x3, x1, 1, 1
	ubfx	x0, x1, 2, 1
	strb	w4, [x21, 3372]
	ubfx	x1, x1, 3, 1
	strb	w3, [x2]
	strb	w0, [x21, 3373]
	strb	w1, [x21, 3374]
	b	.L815
	.p2align 3
.L808:
	ldr	w2, [x21, -204]
	mov	w1, 10000
	add	w0, w2, 4096
	add	w0, w0, 904
	cmp	w0, w1
	bls	.L836
.L813:
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
	b	.L809
	.p2align 3
.L802:
	ldr	w2, [x21, -208]
	mov	w1, 10000
	add	w0, w2, 4096
	add	w0, w0, 904
	cmp	w0, w1
	bls	.L837
.L807:
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
	b	.L803
	.p2align 3
.L822:
	mov	w0, 0
	b	.L803
.L834:
	ldr	w1, [x21, 3368]
	b	.L817
.L833:
	mov	x0, x19
	bl	CloseDrivers
.L830:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	w0, -1
	ldp	x29, x30, [sp], 80
	ret
.L832:
	ldr	x2, .LC356
	mov	w3, 8451
	ldr	x1, .LC358
	mov	w0, 22
	add	x2, x2, 48
	bl	dprint_vfmw
	b	.L811
.L831:
	ldr	x2, .LC356
	mov	w3, 8404
	ldr	x1, .LC358
	mov	w0, 22
	add	x2, x2, 24
	bl	dprint_vfmw
	b	.L805
.L835:
	ldr	x2, .LC356
	mov	w3, 8508
	ldr	x1, .LC358
	mov	w0, 22
	add	x2, x2, 72
	bl	dprint_vfmw
	b	.L815
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
	beq	.L843
	cmp	w19, w22
	mov	w19, w21
	blt	.L844
.L840:
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
.L844:
	ldr	x1, .LC382
	mov	w0, 30
	mov	w19, 3
	bl	dprint_vfmw
	b	.L840
	.p2align 3
.L843:
	cmp	w20, w22
	mov	w19, w21
	ble	.L840
	ldr	x1, .LC380
	mov	w0, 30
	mov	w19, 0
	bl	dprint_vfmw
	b	.L840
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
.L848:
	ldr	x2, [x3, x0, lsl 3]
	add	x1, x2, 327680
	add	x1, x1, 2760
	cbz	x2, .L846
	cmp	x19, x1
	beq	.L847
.L846:
	add	x0, x0, 1
	cmp	x0, 16
	bne	.L848
	mov	w0, -1
.L847:
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
.L858:
	ldr	x1, [x4, x0, lsl 3]
	mov	w5, w0
	cbz	x1, .L855
	ldr	w2, [x1, 280]
	cmp	w2, w19
	bhi	.L856
	ldr	w3, [x1, 288]
	add	w2, w2, w3
	cmp	w19, w2
	bcc	.L857
.L856:
	ldr	w2, [x1, 312]
	cmp	w19, w2
	bcc	.L855
	ldr	w1, [x1, 320]
	add	w2, w2, w1
	cmp	w19, w2
	bcc	.L860
.L855:
	add	x0, x0, 1
	cmp	x0, 16
	bne	.L858
	mov	w0, -1
.L857:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L860:
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
	bhi	.L938
	ldr	x20, .LC389
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L939
	ldr	w22, [x0, 16]
	cmp	w22, 1
	bne	.L940
	ldr	w21, [x0, 32]
	cbnz	w21, .L941
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
	bls	.L871
	str	w0, [x2, w19, sxtw 2]
	mov	w23, w22
.L871:
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w0, [x0, 672]
	sub	w1, w0, #10
	cmp	w1, 2
	bhi	.L872
	ldr	w1, [x20, 4076]
	tbnz	x1, 0, .L872
.L873:
	mov	w0, w19
	bl	VCTRL_GetImageBuffer
	cmp	w0, 1
	beq	.L874
	cmp	w23, 1
	mov	w21, -3
	beq	.L942
.L913:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L872:
	and	w1, w0, -17
	cmp	w1, 4
	ccmp	w0, 14, 4, ne
	beq	.L873
	mov	w0, w19
	bl	VCTRL_GetDecParam
	cbz	x0, .L873
	cmp	w23, 1
	mov	w21, -2
	bne	.L913
	ldr	x1, .LC398
	mov	w2, w19
	mov	w0, 23
	bl	dprint_vfmw
	b	.L913
	.p2align 3
.L874:
	ldr	x26, [x20, w19, sxtw 3]
	ldr	w2, [x26, 1552]
	cbz	w2, .L875
	cmp	w2, 1
	beq	.L943
	add	x26, x26, 323584
	ldr	w3, [x26, 2616]
.L897:
	ldr	x1, .LC418
	mov	w0, 31
	mov	w21, -1
	bl	dprint_vfmw
	b	.L913
	.p2align 3
.L875:
	add	x22, x26, 816
	ldrb	w0, [x22, 1]
	cbnz	w0, .L878
	ldr	w0, [x26, 1592]
	cbz	w0, .L944
.L878:
	ldr	x1, .LC404
	mov	w0, 29
	ldr	x2, [x22, 40]
	ldr	x3, [x22, 56]
	bl	dprint_vfmw
	ldr	x4, [x20, w19, sxtw 3]
	ldr	w1, [x4, 52]
	cmp	w1, 1
	beq	.L945
	sub	w0, w1, #2
	cmp	w0, 1
	ccmp	w1, 6, 4, hi
	bne	.L881
.L898:
	add	x2, x26, 824
	add	x0, x4, 1272
	bl	IsNewpicSeg
	cbnz	w0, .L882
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w1, [x0, 52]
	cmp	w1, 3
	ldr	x1, [x0, 744]
	beq	.L936
	cmn	x1, #1
	beq	.L884
.L936:
	str	x1, [x0, 736]
.L884:
	ldr	x3, [x0, 760]
	mov	x2, -1
	ldr	x1, [x0, 776]
	str	x2, [x0, 744]
	str	x3, [x0, 752]
	str	x1, [x0, 768]
	cbnz	x1, .L885
	ldr	x1, [x22, 64]
	str	x1, [x0, 768]
.L885:
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
	bne	.L886
	ldr	x1, [x22, 40]
.L937:
	str	x1, [x0, 744]
.L888:
	ldr	x1, [x22, 56]
	str	x1, [x0, 760]
	ldr	x1, [x22, 64]
	str	x1, [x0, 776]
	ldr	w1, [x22, 72]
	str	w1, [x0, 796]
	ldr	w1, [x22, 76]
	str	w1, [x0, 800]
	ldr	w1, [x22, 80]
	str	w1, [x0, 804]
	ldr	w1, [x22, 84]
	str	w1, [x0, 812]
.L887:
	ldr	x2, [x0, 752]
	mov	w0, 29
	ldr	x1, .LC408
	bl	dprint_vfmw
	b	.L889
	.p2align 3
.L942:
	ldr	x1, .LC400
	mov	w2, w19
	mov	w0, 23
	bl	dprint_vfmw
	b	.L913
	.p2align 3
.L945:
	add	x0, x4, 327680
	ldr	x0, [x0, 2760]
	ldrsb	w1, [x0, 596]
	cmp	w1, 1
	beq	.L898
	.p2align 2
.L881:
	ldr	x2, [x22, 40]
	cmn	x2, #1
	beq	.L946
	str	x2, [x4, 736]
.L891:
	ldr	x3, [x22, 56]
	str	x3, [x4, 752]
	ldr	x1, .LC410
	ldr	x0, [x22, 64]
	str	x0, [x4, 768]
	mov	w0, 29
	ldr	w5, [x22, 72]
	str	w5, [x4, 784]
	ldr	w5, [x22, 76]
	str	w5, [x4, 788]
	ldr	w5, [x22, 80]
	str	w5, [x4, 792]
	ldr	w5, [x22, 84]
	str	w5, [x4, 808]
	bl	dprint_vfmw
.L889:
	mov	w0, 3
	bl	CHECK_REC_POS_ENABLE
	cmp	w23, 1
	ccmp	w0, wzr, 4, eq
	bne	.L947
.L892:
	ldr	x1, [x20, w19, sxtw 3]
	mov	x0, x22
	add	x2, x1, 327680
	ldr	w1, [x1, 672]
	add	x2, x2, 2760
	bl	DecodeStream
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w1, [x0, 672]
	cmp	w1, 16
	beq	.L948
.L893:
	mov	w0, 4
	bl	CHECK_REC_POS_ENABLE
	cbz	w0, .L913
	ldr	x0, [x24]
	add	x25, x25, 3512
	blr	x0
	mov	w1, w0
	ldr	x2, .LC414
	ldr	w0, [x25, w19, sxtw 2]
	ldr	w2, [x2]
	sub	w0, w1, w0
	cmp	w0, w2
	bls	.L913
	mov	w0, w19
	str	w1, [x25, w19, sxtw 2]
	bl	VCTRL_GetDecParam
	cbz	x0, .L913
	ldr	x0, .LC411
	lsl	w2, w19, 24
	add	w2, w2, 262144
	mov	w1, 7897
	add	x0, x0, 88
	bl	DBG_AddTrace
	b	.L913
	.p2align 3
.L882:
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w1, [x0, 52]
	cmp	w1, 3
	beq	.L887
.L886:
	ldr	x1, [x22, 40]
	cmn	x1, #1
	bne	.L937
	b	.L888
	.p2align 3
.L944:
	add	x1, x26, 824
	mov	w0, w19
	bl	SM_ReadSegStream
	cbz	w0, .L878
	cmp	w23, 1
	beq	.L949
.L879:
	ldr	x0, [x20, w19, sxtw 3]
	mov	w1, 1
	mov	w21, -4
	str	w1, [x0, 440]
	b	.L913
	.p2align 3
.L940:
	ldr	x3, .LC391
	mov	w2, 7649
	ldr	x1, .LC388
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L913
	.p2align 3
.L941:
	ldr	x3, .LC393
	mov	w2, 7650
	ldr	x1, .LC388
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L913
	.p2align 3
.L948:
	ldr	w1, [x0, 1616]
	cmp	w1, 1
	beq	.L893
	add	x0, x0, 327680
	add	x0, x0, 2760
	bl	IsMain10Profile
	cbnz	w0, .L950
	ldr	x0, [x20, w19, sxtw 3]
	str	wzr, [x0, 1616]
	b	.L893
	.p2align 3
.L943:
	add	x0, x26, 323584
	ldr	w3, [x0, 2616]
	tst	w3, -3
	beq	.L875
	cmp	w3, 1
	bne	.L897
	ldr	x1, .LC416
	mov	w0, 31
	mov	w21, -3
	bl	dprint_vfmw
	b	.L913
	.p2align 3
.L946:
	ldr	x2, [x4, 736]
	b	.L891
	.p2align 3
.L938:
	ldr	x3, .LC387
	mov	w2, 7647
	ldr	x1, .LC388
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L913
	.p2align 3
.L947:
	ldr	x0, .LC411
	lsl	w2, w19, 24
	add	w2, w2, 196608
	mov	w1, 7859
	add	x0, x0, 88
	bl	DBG_AddTrace
	b	.L892
.L950:
	ldr	x2, .LC411
	mov	w3, w19
	ldr	x1, .LC413
	mov	w0, 0
	add	x2, x2, 88
	bl	dprint_vfmw
	ldr	x0, [x20, w19, sxtw 3]
	mov	w1, 1
	str	w1, [x0, 1616]
	bl	VDMDRV_AddMatchMode
	b	.L893
.L939:
	ldr	x3, .LC390
	mov	w2, 7648
	ldr	x1, .LC388
	mov	w21, -1
	bl	dprint_vfmw
	b	.L913
.L949:
	ldr	x1, .LC402
	mov	w2, w19
	mov	w0, 23
	bl	dprint_vfmw
	b	.L879
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
	b	.L954
	.p2align 3
.L978:
	ldr	w1, [x1, 28]
	str	w1, [x2, x0, lsl 2]
	add	x0, x0, 1
	cmp	x0, 16
	beq	.L977
.L954:
	ldr	x1, [x23, x0, lsl 3]
	add	x2, x22, 3576
	cbnz	x1, .L978
	add	x1, x22, 3576
	str	wzr, [x1, x0, lsl 2]
	add	x0, x0, 1
	cmp	x0, 16
	bne	.L954
.L977:
	ldr	x24, .LC423
	mov	w20, -1
	.p2align 2
.L955:
	ldr	w3, [x22, 3576]
	add	x1, x22, 3580
	mov	w0, 0
	mov	w19, 0
	.p2align 2
.L956:
	add	w0, w0, 1
	cmp	w0, 16
	beq	.L979
.L958:
	ldr	w2, [x1]
	add	x1, x1, 4
	cmp	w3, w2
	bge	.L956
	mov	w19, w0
	add	w0, w0, 1
	cmp	w0, 16
	mov	w3, w2
	bne	.L958
.L979:
	add	x2, x22, 3576
	add	x0, x23, 128
	add	w1, w19, 1
	cmp	w3, wzr
	sxtw	x21, w19
	str	wzr, [x2, w19, sxtw 2]
	str	w1, [x0, 28]
	ble	.L959
	ldr	x0, [x23, w19, sxtw 3]
	cbz	x0, .L955
	ldr	w1, [x0, 16]
	cmp	w1, 1
	bne	.L955
	ldr	w1, [x0, 76]
	cmp	w1, wzr
	ble	.L961
	ldr	w0, [x0, 80]
	cbz	w0, .L980
.L961:
	mov	w21, 200
	b	.L966
	.p2align 3
.L964:
	subs	w21, w21, #1
	cbnz	w0, .L955
	beq	.L955
.L966:
	mov	w0, w19
	bl	DecChanSyntax
	cmn	w0, #1
	mov	w20, w0
	bne	.L964
	ldr	x4, [x22, 3376]
	cbz	x4, .L965
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	mov	w0, w19
	blr	x4
.L965:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 96
	ret
.L980:
	add	x1, x29, 72
	mov	w0, w19
	bl	SM_GetInfo
	cbnz	w0, .L981
.L962:
	ldr	x20, [x23, x21, lsl 3]
	ldr	w1, [x29, 84]
	ldr	w2, [x20, 76]
	ldr	w0, [x20, 688]
	bl	GetNextDecodeMode
	str	w0, [x20, 688]
	ldr	x1, [x23, x21, lsl 3]
	ldr	w0, [x1, 688]
	cbz	w0, .L963
	str	wzr, [x1, 680]
	str	wzr, [x1, 684]
	b	.L961
.L959:
	str	wzr, [x0, 28]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 96
	ret
.L963:
	ldr	w0, [x1, 72]
	str	w0, [x1, 680]
	str	w0, [x1, 684]
	b	.L961
.L981:
	ldr	x3, [x24, 96]
	mov	x2, 24
	mov	w1, 0
	add	x0, x29, 72
	blr	x3
	b	.L962
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
	ble	.L983
	ldr	x1, [x21, 232]
	blr	x1
.L983:
	ldr	x1, [x21, 24]
	mov	w0, 0
	mov	w22, 0
	blr	x1
	bl	SM_Wakeup
	ldr	w0, [x19, 3644]
	mov	w1, 101
	str	w1, [x20, 152]
	cbz	w0, .L990
.L984:
	mov	w0, 102
	mov	w19, 0
	str	w0, [x20, 152]
	bl	VDMDRV_WakeUpVdm
	.p2align 2
.L985:
	mov	w0, w19
	mov	w1, 0
	add	w19, w19, 1
	bl	DeleteRawPacketInBuffer
	cmp	w19, 16
	bne	.L985
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
.L990:
	bl	DecSyntax
	mov	w22, w0
	b	.L984
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
.L992:
.L999:
	mov	x1, x25
	mov	w0, 2
	str	wzr, [x20, 152]
	bl	dprint_vfmw
	str	w21, [x20, 132]
	ldr	w0, [x20, 136]
	cmp	w0, 2
	bne	.L1014
	b	.L997
	.p2align 3
.L1016:
	str	w21, [x20, 152]
	bl	VCTRL_RunProcess
	mov	x1, x22
	mov	w0, 22
	bl	dprint_vfmw
	ldr	w0, [x20, 136]
	cmp	w0, 2
	beq	.L997
.L1014:
	cmp	w0, 3
	bne	.L1016
.L995:
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
.L997:
	mov	w2, 4
	mov	x1, x26
	mov	w0, 2
	str	w2, [x20, 152]
	mov	w27, 5
	bl	dprint_vfmw
	mov	w0, 1
	str	w0, [x20, 132]
	b	.L994
	.p2align 3
.L1017:
	ldr	x1, [x24, 232]
	blr	x1
.L994:
	mov	x1, x23
	mov	w0, 22
	str	w27, [x20, 152]
	bl	dprint_vfmw
	ldr	w1, [x20, 136]
	mov	w0, 10
	cmp	w1, 1
	beq	.L999
	cmp	w1, 3
	bne	.L1017
	b	.L995
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
	cbz	x0, .L1036
	ldr	w1, [x0, 672]
	cmp	w1, 20
	bls	.L1037
.L1036:
	mov	x0, 0
.L1019:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1037:
	ldr	x2, .LC443
	ldrb	w1, [x2,w1,uxtw]
	adr	x2, .Lrtx1021
	add	x1, x2, w1, sxtb #2
	br	x1
.Lrtx1021:
	.section	.rodata
	.align	0
	.align	2
.L1021:
	.byte	(.L1034 - .Lrtx1021) / 4
	.byte	(.L1022 - .Lrtx1021) / 4
	.byte	(.L1023 - .Lrtx1021) / 4
	.byte	(.L1024 - .Lrtx1021) / 4
	.byte	(.L1036 - .Lrtx1021) / 4
	.byte	(.L1025 - .Lrtx1021) / 4
	.byte	(.L1026 - .Lrtx1021) / 4
	.byte	(.L1036 - .Lrtx1021) / 4
	.byte	(.L1027 - .Lrtx1021) / 4
	.byte	(.L1028 - .Lrtx1021) / 4
	.byte	(.L1029 - .Lrtx1021) / 4
	.byte	(.L1029 - .Lrtx1021) / 4
	.byte	(.L1029 - .Lrtx1021) / 4
	.byte	(.L1030 - .Lrtx1021) / 4
	.byte	(.L1036 - .Lrtx1021) / 4
	.byte	(.L1031 - .Lrtx1021) / 4
	.byte	(.L1032 - .Lrtx1021) / 4
	.byte	(.L1033 - .Lrtx1021) / 4
	.byte	(.L1036 - .Lrtx1021) / 4
	.byte	(.L1036 - .Lrtx1021) / 4
	.byte	(.L1034 - .Lrtx1021) / 4
	.text
	.p2align 3
.L1034:
	add	x0, x0, 327680
	ldr	x19, [sp, 16]
	add	x0, x0, 2768
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1032:
	add	x0, x0, 389120
	add	x0, x0, 3824
	b	.L1019
	.p2align 3
.L1031:
	add	x0, x0, 327680
	add	x0, x0, 3672
	b	.L1019
	.p2align 3
.L1030:
	add	x0, x0, 335872
	add	x0, x0, 2776
	b	.L1019
	.p2align 3
.L1029:
	add	x0, x0, 339968
	add	x0, x0, 3224
	b	.L1019
	.p2align 3
.L1033:
	add	x0, x0, 598016
	add	x0, x0, 3640
	b	.L1019
	.p2align 3
.L1022:
	add	x0, x0, 380928
	add	x0, x0, 2952
	b	.L1019
	.p2align 3
.L1024:
	add	x0, x0, 417792
	add	x0, x0, 448
	b	.L1019
	.p2align 3
.L1023:
	add	x0, x0, 352256
	add	x0, x0, 912
	b	.L1019
	.p2align 3
.L1028:
	add	x0, x0, 380928
	add	x0, x0, 3424
	b	.L1019
	.p2align 3
.L1027:
	add	x0, x0, 380928
	add	x0, x0, 3456
	b	.L1019
	.p2align 3
.L1026:
	add	x0, x0, 344064
	add	x0, x0, 2576
	b	.L1019
	.p2align 3
.L1025:
	add	x0, x0, 368640
	add	x0, x0, 3400
	b	.L1019
	.size	VCTRL_GetChanVoQue, .-VCTRL_GetChanVoQue
	.align	3
.LC442:
	.xword	.LANCHOR0
	.align	3
.LC443:
	.xword	.L1021
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
	bhi	.L1050
	mov	w0, w23
	bl	VCTRL_GetChanVoQue
	mov	x20, x0
	cbz	x0, .L1046
	add	x2, x29, 96
	add	x1, x29, 88
	str	wzr, [x29, 88]
	str	wzr, [x2, -4]!
	bl	GetQueueImgNum
	ldr	w2, [x29, 88]
	mov	w0, -1
	cbz	w2, .L1040
	cmp	w2, 32
	bhi	.L1042
.L1044:
	ldr	x21, .LC449
	mov	w19, 0
	ldr	x24, .LC448
	sxtw	x25, w23
	ldr	x22, .LC451
	add	x21, x21, 104
	.p2align 2
.L1043:
	ldrh	w5, [x20, 4]
	mov	w4, w19
	ldr	x8, [x24, x25, lsl 3]
	mov	x1, x22
	add	w5, w5, w19
	mov	w7, 0
	and	x5, x5, 63
	add	x8, x8, x19, sxtw 4
	add	x5, x5, 2
	add	x8, x8, 327680
	mov	x6, 0
	mov	w3, 4225
	mov	x2, x21
	mov	w0, 1
	ldr	x5, [x20, x5, lsl 3]
	add	w19, w19, 1
	sxth	w19, w19
	ldr	w5, [x5, 28]
	str	w5, [x8, 2196]
	str	wzr, [x8, 2192]
	str	xzr, [x8, 2200]
	bl	dprint_vfmw
	ldr	w1, [x29, 88]
	cmp	w19, w1
	bcc	.L1043
.L1045:
	ldr	x2, [x24, w23, sxtw 3]
	mov	w0, 0
	add	x2, x2, 327680
	str	w1, [x2, 2704]
.L1040:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1042:
	ldr	x1, .LC447
	mov	w3, 32
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x29, 88]
	cbnz	w1, .L1044
	ldr	x24, .LC448
	b	.L1045
.L1046:
	mov	w0, -1
	b	.L1040
.L1050:
	ldr	x3, .LC444
	mov	w2, 4190
	ldr	x1, .LC445
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1040
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
	bhi	.L1068
	cbz	x19, .L1069
	mov	w0, w22
	bl	VCTRL_GetChanVoQue
	mov	x20, x0
	cbz	x0, .L1061
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
	cbnz	w2, .L1070
	ldr	x6, .LC459
	ldr	x4, [x6, w22, sxtw 3]
	add	x0, x4, 327680
	ldr	w0, [x0, 2704]
	cbz	w0, .L1059
	mov	w1, 0
	.p2align 2
.L1060:
	sxtw	x2, w1
	add	w1, w1, 1
	add	x3, x2, 20480
	lsl	x2, x2, 4
	add	x5, x19, x2
	sxth	w1, w1
	add	x3, x4, x3, lsl 4
	ldr	w0, [x3, 2196]
	str	w0, [x5, 4]
	str	xzr, [x5, 8]
	str	wzr, [x19, x2]
	ldr	x4, [x6, x22, lsl 3]
	add	x0, x4, 327680
	ldr	w0, [x0, 2704]
	cmp	w1, w0
	bcc	.L1060
	str	w0, [x19, 512]
	b	.L1059
	.p2align 3
.L1070:
	cmp	w2, 32
	bhi	.L1071
.L1056:
	mov	w0, 0
	.p2align 2
.L1058:
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
	bhi	.L1058
	uxth	w1, w2
.L1057:
	ldrh	w0, [x20, 4]
	add	w0, w1, w0
	and	w0, w0, 63
	strh	w0, [x20, 4]
	str	w2, [x19, 512]
.L1059:
	ldr	x1, [x21, 32]
	mov	w0, 4
	blr	x1
	mov	w0, 0
.L1053:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1071:
	ldr	x1, .LC458
	mov	w3, 32
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w2, [x29, 72]
	cbnz	w2, .L1056
	mov	w2, 0
	mov	w1, 0
	b	.L1057
.L1061:
	mov	w0, -1
	b	.L1053
.L1068:
	ldr	x3, .LC453
	mov	w2, 4305
	ldr	x1, .LC454
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1053
.L1069:
	ldr	x3, .LC456
	mov	w2, 4306
	ldr	x1, .LC454
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1053
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
	cbz	x0, .L1080
	mov	w0, w19
	bl	VCTRL_GetChanVoQue
	cbz	x0, .L1075
	mov	x1, x21
	mov	x2, x23
	bl	GetQueueImgNum
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w1, [x0, 672]
	cbz	w1, .L1081
	cmp	w1, 16
	beq	.L1082
	cmp	w1, 17
	beq	.L1083
	cmp	w1, 15
	beq	.L1084
	mov	w0, 2
	str	w0, [x22]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1081:
	add	x0, x0, 356352
	ldr	w0, [x0, 2376]
	str	w0, [x22]
.L1072:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1075:
	str	wzr, [x23]
	str	wzr, [x21]
	str	wzr, [x22]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1084:
	add	x0, x0, 11452416
	ldr	w0, [x0, 3520]
	str	w0, [x22]
	b	.L1072
	.p2align 3
.L1082:
	add	x0, x0, 331776
	ldr	w0, [x0, 1376]
	str	w0, [x22]
	b	.L1072
	.p2align 3
.L1083:
	mov	w0, 9
	str	w0, [x22]
	b	.L1072
	.p2align 3
.L1080:
	ldr	x3, .LC461
	mov	w2, 8042
	ldr	x1, .LC462
	bl	dprint_vfmw
	b	.L1072
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
	bhi	.L1098
	stp	wzr, wzr, [x29, 60]
	str	wzr, [x29, 68]
	cbz	x19, .L1085
	ldr	x3, .LC468
	mov	x2, 100
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	ldr	x21, .LC469
	ldr	x0, [x21, w20, sxtw 3]
	cbz	x0, .L1085
	ldr	w0, [x0, 1288]
	str	w0, [x19, 32]
	ldr	x0, [x21, w20, sxtw 3]
	ldr	w0, [x0, 1292]
	str	w0, [x19, 36]
	ldr	x0, [x21, w20, sxtw 3]
	ldr	w0, [x0, 1296]
	str	w0, [x19, 44]
	ldr	x5, [x21, w20, sxtw 3]
	add	x0, x5, 294912
	ldrsb	w0, [x0, 2234]
	cmp	w0, wzr
	ble	.L1093
	mov	x2, 57010
	sub	w4, w0, #1
	mov	w3, 100
	movk	x2, 0x4, lsl 16
	add	x0, x5, 315392
	mov	w1, 0
	umaddl	x2, w4, w3, x2
	add	x0, x0, 3662
	add	x3, x5, x2
	.p2align 2
.L1092:
	ldrsb	w2, [x0], 100
	cmp	w2, 3
	csinc	w1, w1, w1, ne
	cmp	x0, x3
	bne	.L1092
.L1090:
	str	w1, [x19, 84]
	add	x3, x29, 68
	add	x2, x29, 64
	mov	w0, w20
	ldr	x4, [x21, w20, sxtw 3]
	add	x1, x29, 60
	sxtw	x22, w20
	add	x4, x4, 294912
	ldrsb	w4, [x4, 2234]
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
	mov	x0, 15048
	ldr	x1, [x21, w20, sxtw 3]
	movk	x0, 0x5, lsl 16
	add	x0, x1, x0
	ldrb	w0, [x0, 3632]
	and	w0, w0, 1
	str	w0, [x19, 88]
.L1085:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1098:
	ldr	x3, .LC466
	mov	w2, 1501
	ldr	x1, .LC467
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
.L1093:
	mov	w1, 0
	b	.L1090
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
	bhi	.L1103
	ldr	x0, .LC474
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1104
	ldr	w0, [x0, 1388]
.L1101:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1103:
	ldr	x3, .LC472
	mov	w2, 8099
	ldr	x1, .LC473
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1101
.L1104:
	ldr	x3, .LC475
	mov	w2, 8100
	ldr	x1, .LC473
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1101
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
	bhi	.L1109
	ldr	x0, .LC478
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1110
	ldr	w0, [x0, 1392]
.L1107:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1109:
	ldr	x3, .LC476
	mov	w2, 8107
	ldr	x1, .LC477
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1107
.L1110:
	ldr	x3, .LC479
	mov	w2, 8108
	ldr	x1, .LC477
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1107
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
	bhi	.L1115
	ldr	x0, .LC482
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1114
	ldr	w0, [x0, 44]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1115:
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
.L1114:
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
	bhi	.L1121
.L1117:
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
	bhi	.L1122
.L1118:
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
.L1121:
	ldr	x1, .LC485
	mov	w0, 32
	bl	dprint_vfmw
	ldr	w2, [x20, -208]
	b	.L1117
	.p2align 3
.L1122:
	ldr	x1, .LC487
	mov	w2, w19
	mov	w0, 32
	bl	dprint_vfmw
	ldr	w19, [x20, -204]
	b	.L1118
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
	beq	.L1168
	cmp	w19, 15
	bhi	.L1136
	ldr	x20, .LC492
	sxtw	x23, w19
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1136
	ldr	w1, [x0, 12]
	cbz	w1, .L1136
	ldr	w1, [x0, 16]
	cbnz	w1, .L1169
	.p2align 2
.L1136:
	ldr	x2, .LC493
	mov	w3, w19
	ldr	x1, .LC503
	mov	w0, 1
	add	x2, x2, 128
	mov	w19, -1
	bl	dprint_vfmw
	str	xzr, [x22, 8]
	mov	w0, 5
.L1166:
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
.L1168:
	ldr	x23, .LC489
	ldr	w28, [x23, 64]
	tbnz	w28, #31, .L1125
	ldr	x25, .LC493
	add	x20, x23, 68
	ldr	x27, .LC495
	add	x23, x23, 128
	ldr	x24, .LC492
	add	x25, x25, 128
	.p2align 2
.L1126:
	cmp	w28, 15
	bhi	.L1127
	ldr	x0, [x24, w28, sxtw 3]
	sxtw	x26, w28
	cbz	x0, .L1127
	ldr	w1, [x0, 12]
	cbz	w1, .L1127
	ldr	w1, [x0, 16]
	cbz	w1, .L1127
	ldr	w1, [x0, 28]
	cbz	w1, .L1127
	ldr	w0, [x0, 672]
	cmp	w0, 21
	beq	.L1127
	mov	w0, w28
	bl	VDMDRV_IsChanDec
	cmp	w0, 1
	beq	.L1170
	mov	w0, w28
	bl	VCTRL_GetDecParam
	cbz	x0, .L1171
	ldr	x1, [x24, x26, lsl 3]
	cbz	x1, .L1172
	str	x0, [x29, 104]
	ldr	w0, [x1, 672]
	str	w0, [x29, 100]
	ldr	x0, .LC501
	str	w28, [x29, 96]
	ldr	x1, [x0, -256]
	add	x0, x29, 96
	blr	x1
	cmp	w0, 1
	beq	.L1173
	str	xzr, [x22, 8]
	cmp	x23, x20
	bne	.L1135
.L1125:
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
.L1127:
	mov	w3, w28
	mov	x2, x25
	mov	x1, x27
	mov	w0, 1
	bl	dprint_vfmw
.L1129:
	cmp	x23, x20
	beq	.L1125
.L1135:
	ldr	w28, [x20], 4
	tbz	w28, #31, .L1126
	b	.L1125
	.p2align 3
.L1172:
	ldr	x1, .LC500
	mov	w3, w28
	mov	w2, 8237
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1129
	.p2align 3
.L1170:
	ldr	x1, .LC497
	mov	w2, w28
	mov	w0, 22
	bl	dprint_vfmw
	b	.L1129
	.p2align 3
.L1169:
	ldr	w1, [x0, 28]
	cbz	w1, .L1136
	ldr	w0, [x0, 672]
	cmp	w0, 21
	beq	.L1136
	mov	w0, w19
	bl	VDMDRV_IsChanDec
	cmp	w0, 1
	beq	.L1174
	mov	w0, w19
	bl	VCTRL_GetDecParam
	cbz	x0, .L1175
	ldr	x1, [x20, x23, lsl 3]
	cbz	x1, .L1176
	ldr	x2, .LC501
	ldr	w1, [x1, 672]
	stp	w19, w1, [x29, 96]
	ldr	x1, [x2, -256]
	str	x0, [x29, 104]
	add	x0, x29, 96
	blr	x1
	cmp	w0, 1
	bne	.L1141
	ldp	w4, w3, [x29, 96]
	ldr	x2, [x29, 104]
	mov	w0, w19
	mov	w1, 4
	str	x2, [x22, 8]
	stp	w4, w3, [x22]
	mov	w19, 0
	bl	VDMDRV_SetChanIsDec
	b	.L1125
	.p2align 3
.L1171:
	ldr	x1, .LC499
	mov	w2, w28
	mov	w0, 22
	bl	dprint_vfmw
	b	.L1129
	.p2align 3
.L1141:
	mov	w0, 5
	mov	w19, -1
	str	xzr, [x22, 8]
	b	.L1166
	.p2align 3
.L1173:
	ldp	w5, w4, [x29, 96]
	ldr	x2, [x29, 104]
	mov	w1, 4
	mov	w0, w28
	str	x2, [x22, 8]
	stp	w5, w4, [x22]
	mov	w19, 0
	bl	VDMDRV_SetChanIsDec
	b	.L1125
.L1176:
	ldr	x1, .LC500
	mov	w3, w19
	mov	w2, 8322
	mov	w0, 1
	mov	w19, -1
	bl	dprint_vfmw
	str	xzr, [x22, 8]
	mov	w0, 5
	b	.L1166
.L1174:
	ldr	x1, .LC497
	mov	w2, w19
.L1167:
	mov	w0, 22
	mov	w19, -1
	bl	dprint_vfmw
	str	xzr, [x22, 8]
	mov	w0, 5
	b	.L1166
.L1175:
	ldr	x1, [x21, 248]
	blr	x1
	mov	w2, w19
	ldr	x1, .LC499
	b	.L1167
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
	bhi	.L1181
	ldr	x0, .LC507
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1180
	ldr	w0, [x0, 444]
.L1179:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1181:
	ldr	x3, .LC505
	mov	w2, 9281
	ldr	x1, .LC506
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1179
.L1180:
	mov	w0, -1
	b	.L1179
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
	bhi	.L1188
	ldr	x0, .LC510
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1182
	str	w20, [x0, 444]
.L1182:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1188:
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
	bhi	.L1195
	ldr	x0, .LC513
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1189
	str	w20, [x0, 448]
.L1189:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1195:
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
	cbz	w20, .L1203
	cmp	w20, 1
	mov	w0, 3
	mov	w1, 4
	csel	w0, w0, w1, eq
	cmp	w19, 15
	bhi	.L1210
.L1200:
	ldr	x21, .LC516
	ldr	x1, [x21, w19, sxtw 3]
	cbz	x1, .L1201
	str	w0, [x1, 448]
.L1201:
	ldr	x0, .LC517
	ldr	x4, [x0, 3376]
	cbz	x4, .L1202
	add	x2, x29, 64
	mov	w3, 4
	mov	w1, 115
	mov	w0, w19
	str	w20, [x2, -8]!
	blr	x4
.L1202:
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
.L1203:
	cmp	w19, 15
	mov	w0, 2
	bls	.L1200
.L1210:
	ldr	x3, .LC514
	mov	w2, 9305
	ldr	x1, .LC515
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, .LC516
	b	.L1201
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
	add	x0, x0, 327680
	ldr	w1, [x0, 2752]
	cmp	w1, 17
	bls	.L1236
.L1231:
	mov	w19, -1
.L1228:
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
.L1236:
	ldr	x2, .LC521
	ldrb	w1, [x2,w1,uxtw]
	adr	x2, .Lrtx1214
	add	x1, x2, w1, sxtb #2
	br	x1
.Lrtx1214:
	.section	.rodata
	.align	0
	.align	2
.L1214:
	.byte	(.L1213 - .Lrtx1214) / 4
	.byte	(.L1215 - .Lrtx1214) / 4
	.byte	(.L1216 - .Lrtx1214) / 4
	.byte	(.L1217 - .Lrtx1214) / 4
	.byte	(.L1231 - .Lrtx1214) / 4
	.byte	(.L1218 - .Lrtx1214) / 4
	.byte	(.L1219 - .Lrtx1214) / 4
	.byte	(.L1231 - .Lrtx1214) / 4
	.byte	(.L1220 - .Lrtx1214) / 4
	.byte	(.L1221 - .Lrtx1214) / 4
	.byte	(.L1222 - .Lrtx1214) / 4
	.byte	(.L1222 - .Lrtx1214) / 4
	.byte	(.L1222 - .Lrtx1214) / 4
	.byte	(.L1223 - .Lrtx1214) / 4
	.byte	(.L1231 - .Lrtx1214) / 4
	.byte	(.L1224 - .Lrtx1214) / 4
	.byte	(.L1225 - .Lrtx1214) / 4
	.byte	(.L1226 - .Lrtx1214) / 4
	.text
	.p2align 3
.L1213:
	add	x0, x0, 2760
	bl	H264DEC_GetRemainImg
	mov	w19, w0
	.p2align 2
.L1212:
	cmn	w19, #1
	beq	.L1228
	cbnz	w19, .L1237
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
.L1215:
	add	x0, x0, 2760
	bl	VC1DEC_GetRemainImg
	mov	w19, w0
	b	.L1212
	.p2align 3
.L1216:
	add	x0, x0, 2760
	bl	MPEG4DEC_GetRemainImg
	mov	w19, w0
	b	.L1212
	.p2align 3
.L1217:
	add	x0, x0, 2760
	bl	MPEG2DEC_GetRemainImg
	mov	w19, w0
	b	.L1212
	.p2align 3
.L1218:
	add	x0, x0, 2760
	bl	DIVX3DEC_GetRemainImg
	mov	w19, w0
	b	.L1212
	.p2align 3
.L1219:
	add	x0, x0, 2760
	bl	AVSDEC_GetRemainImg
	mov	w19, w0
	b	.L1212
	.p2align 3
.L1220:
	add	x0, x0, 2760
	bl	REAL8DEC_GetRemainImg
	mov	w19, w0
	b	.L1212
	.p2align 3
.L1221:
	add	x0, x0, 2760
	bl	REAL9DEC_GetRemainImg
	mov	w19, w0
	b	.L1212
	.p2align 3
.L1222:
	add	x0, x0, 2760
	bl	VP6DEC_GetRemainImg
	mov	w19, w0
	b	.L1212
	.p2align 3
.L1223:
	add	x0, x0, 2760
	bl	VP8DEC_GetRemainImg
	mov	w19, w0
	b	.L1212
	.p2align 3
.L1224:
	add	x0, x0, 2760
	bl	MVC_DEC_GetRemainImg
	mov	w19, w0
	b	.L1212
	.p2align 3
.L1225:
	add	x0, x0, 2760
	bl	HEVCDEC_GetRemainImg
	mov	w19, w0
	b	.L1212
	.p2align 3
.L1226:
	add	x0, x0, 2760
	bl	VP9DEC_GetRemainImg
	mov	w19, w0
	b	.L1212
	.p2align 3
.L1237:
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
	.xword	.L1214
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
	cbz	x0, .L1272
	ldr	w1, [x0, 672]
	ldr	w25, [x0, 1336]
	cmp	w1, 17
	bls	.L1273
.L1238:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1273:
	ldr	x2, .LC531
	ldrh	w1, [x2,w1,uxtw #1]
	adr	x2, .Lrtx1242
	add	x1, x2, w1, sxth #2
	br	x1
.Lrtx1242:
	.section	.rodata
	.align	0
	.align	2
.L1242:
	.2byte	(.L1241 - .Lrtx1242) / 4
	.2byte	(.L1243 - .Lrtx1242) / 4
	.2byte	(.L1244 - .Lrtx1242) / 4
	.2byte	(.L1245 - .Lrtx1242) / 4
	.2byte	(.L1238 - .Lrtx1242) / 4
	.2byte	(.L1246 - .Lrtx1242) / 4
	.2byte	(.L1247 - .Lrtx1242) / 4
	.2byte	(.L1238 - .Lrtx1242) / 4
	.2byte	(.L1248 - .Lrtx1242) / 4
	.2byte	(.L1249 - .Lrtx1242) / 4
	.2byte	(.L1250 - .Lrtx1242) / 4
	.2byte	(.L1250 - .Lrtx1242) / 4
	.2byte	(.L1250 - .Lrtx1242) / 4
	.2byte	(.L1251 - .Lrtx1242) / 4
	.2byte	(.L1238 - .Lrtx1242) / 4
	.2byte	(.L1252 - .Lrtx1242) / 4
	.2byte	(.L1253 - .Lrtx1242) / 4
	.2byte	(.L1254 - .Lrtx1242) / 4
	.text
	.p2align 3
.L1254:
	add	x0, x0, 327680
	mov	w3, w24
	mov	w2, w25
	mov	w1, w20
	add	x0, x0, 2760
	bl	VP9DEC_VDMPostProc
	cmp	w25, 1
	beq	.L1238
.L1270:
	ldr	x0, [x21, w19, sxtw 3]
.L1271:
	add	x0, x0, 327680
	str	xzr, [x0, 2768]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1253:
	add	x0, x0, 327680
	mov	w5, w24
	mov	w4, w25
	mov	x3, x23
	mov	w2, 1
	mov	w1, w20
	add	x0, x0, 2760
	bl	HEVCDEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 327680
	ldrb	w0, [x0, 2767]
	cmp	w0, 1
	beq	.L1274
.L1257:
	cmp	w25, 1
	beq	.L1238
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 331776
	str	xzr, [x0, 1048]
	b	.L1238
	.p2align 3
.L1252:
	add	x0, x0, 327680
	mov	w1, w20
	mov	w2, 1
	add	x0, x0, 2760
	bl	MVC_DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 327680
	ldrb	w1, [x0, 2761]
	cmp	w1, 1
	beq	.L1275
	str	xzr, [x0, 2992]
	b	.L1238
	.p2align 3
.L1251:
	add	x0, x0, 327680
	mov	w1, w20
	add	x0, x0, 2760
	bl	VP8DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 327680
	str	xzr, [x0, 2768]
	b	.L1238
	.p2align 3
.L1250:
	add	x0, x0, 327680
	mov	w1, w20
	add	x0, x0, 2760
	bl	VP6DEC_VDMPostProc
	b	.L1270
	.p2align 3
.L1249:
	add	x0, x0, 327680
	mov	w2, w22
	mov	w1, w20
	add	x0, x0, 2760
	bl	REAL9DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 327680
	str	xzr, [x0, 2768]
	b	.L1238
	.p2align 3
.L1248:
	add	x0, x0, 327680
	mov	w2, w22
	mov	w1, w20
	add	x0, x0, 2760
	bl	REAL8DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 327680
	str	xzr, [x0, 2768]
	b	.L1238
	.p2align 3
.L1247:
	add	x0, x0, 327680
	mov	w1, w20
	add	x0, x0, 2760
	bl	AVSDEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 327680
	ldrb	w1, [x0, 2760]
	cmp	w1, 1
	beq	.L1276
	str	xzr, [x0, 2784]
	b	.L1238
	.p2align 3
.L1246:
	add	x0, x0, 327680
	mov	w2, w22
	mov	w1, w20
	add	x0, x0, 2760
	bl	DIVX3DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 327680
	str	xzr, [x0, 2768]
	b	.L1238
	.p2align 3
.L1245:
	add	x0, x0, 327680
	mov	w1, w20
	add	x0, x0, 2760
	bl	MPEG2DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x0, 417792
	ldrb	w1, [x1, 1074]
	cmp	w1, 1
	bne	.L1271
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x0, 417792
	strb	wzr, [x1, 1074]
	b	.L1271
	.p2align 3
.L1244:
	add	x0, x0, 327680
	mov	w1, w20
	mov	x2, x23
	add	x0, x0, 2760
	bl	MPEG4DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x0, 352256
	ldrb	w1, [x1, 1556]
	cmp	w1, 1
	beq	.L1277
.L1260:
	add	x0, x0, 327680
	str	xzr, [x0, 3800]
	b	.L1238
	.p2align 3
.L1243:
	add	x0, x0, 327680
	mov	w1, w20
	add	x0, x0, 2760
	bl	VC1DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x0, 344064
	ldrb	w1, [x1, 1191]
	cmp	w1, 1
	bne	.L1271
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x0, 344064
	strb	wzr, [x1, 1191]
	b	.L1271
	.p2align 3
.L1241:
	add	x0, x0, 327680
	mov	w5, w24
	mov	w4, w25
	mov	x3, x23
	mov	w2, 1
	mov	w1, w20
	add	x0, x0, 2760
	bl	H264DEC_VDMPostProc
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 344064
	ldrb	w0, [x0, 1304]
	cmp	w0, 1
	beq	.L1278
.L1255:
	cmp	w25, 1
	beq	.L1238
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 327680
	str	xzr, [x0, 3376]
	b	.L1238
	.p2align 3
.L1276:
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 327680
	strb	wzr, [x0, 2760]
	str	xzr, [x0, 2784]
	b	.L1238
.L1275:
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 327680
	strb	wzr, [x0, 2761]
	str	xzr, [x0, 2992]
	b	.L1238
	.p2align 3
.L1272:
	ldr	x3, .LC529
	mov	w2, 5184
	ldr	x1, .LC530
	bl	dprint_vfmw
	b	.L1238
	.p2align 3
.L1277:
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x1, x0, 352256
	strb	wzr, [x1, 1556]
	b	.L1260
.L1274:
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 327680
	strb	wzr, [x0, 2767]
	b	.L1257
.L1278:
	mov	w0, w19
	bl	VCTRL_OutputLastFrame
	ldr	x0, [x21, w19, sxtw 3]
	add	x0, x0, 344064
	strb	wzr, [x0, 1304]
	b	.L1255
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
	.xword	.L1242
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
	bhi	.L1287
	ldr	x0, .LC534
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L1288
	ldr	w0, [x0, 16]
	cmp	w0, 1
	beq	.L1283
	ldr	x3, .LC536
	mov	w2, 5376
	ldr	x1, .LC533
	mov	w0, 0
	bl	dprint_vfmw
.L1279:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1283:
	mov	w0, w19
	bl	VCTRL_GetDecParam
	cbz	x0, .L1279
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
.L1287:
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
.L1288:
	ldr	x3, .LC535
	mov	w2, 5375
	ldr	x1, .LC533
	bl	dprint_vfmw
	b	.L1279
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
	bls	.L1309
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
.L1309:
	ldr	x0, .LC538
	ldrb	w0, [x0,w19,uxtw]
	adr	x1, .Lrtx1294
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1294:
	.section	.rodata
	.align	0
	.align	2
.L1294:
	.byte	(.L1293 - .Lrtx1294) / 4
	.byte	(.L1295 - .Lrtx1294) / 4
	.byte	(.L1296 - .Lrtx1294) / 4
	.byte	(.L1297 - .Lrtx1294) / 4
	.byte	(.L1298 - .Lrtx1294) / 4
	.byte	(.L1299 - .Lrtx1294) / 4
	.byte	(.L1300 - .Lrtx1294) / 4
	.text
	.p2align 3
.L1299:
	cmp	w20, 1
	mov	w1, 12
	mov	w0, 13
	csel	w0, w0, w1, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1300:
	cmp	w20, 1
	mov	w0, 7
	csinc	w0, w0, wzr, ne
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1293:
	cmp	w20, 1
	mov	w0, 6
	csel	w0, wzr, w0, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1295:
	cmp	w20, 1
	mov	w1, 8
	mov	w0, 2
	csel	w0, w0, w1, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1296:
	cmp	w20, 1
	mov	w1, 10
	mov	w0, 4
	csel	w0, w0, w1, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1297:
	cmp	w20, 1
	mov	w1, 9
	mov	w0, 3
	csel	w0, w0, w1, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1298:
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
	.xword	.L1294
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
	beq	.L1319
	ldr	w0, [x20, 4]
	cmp	w0, 1
	beq	.L1313
	mov	w0, w21
	mov	w2, 0
	mov	w1, w22
	bl	FSP_ClearLogicFs
	mov	w0, 0
.L1312:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1313:
	ldr	w0, [x20, 48]
	bl	MEM_Phy2Vir
	cbz	x0, .L1320
.L1314:
	mov	w1, w22
	mov	w0, w21
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L1321
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
	beq	.L1322
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
	add	x2, x2, 327680
	add	x3, x2, 2768
	add	x2, x2, 2760
	bl	InsertImgToVoQueue
	cmp	w0, 1
	mov	w0, 0
	beq	.L1312
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
	b	.L1312
	.p2align 3
.L1320:
	ldr	w0, [x20, 4]
	cmp	w0, 1
	bne	.L1314
	ldr	x1, .LC544
	mov	w0, 0
	ldr	w2, [x20, 48]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1312
.L1319:
	ldr	x1, .LC542
	mov	w0, 0
	ldr	w2, [x20, 40]
	bl	dprint_vfmw
	mov	w0, w22
	b	.L1312
.L1322:
	ldr	x1, .LC548
	mov	w2, w0
	mov	w0, w23
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1312
.L1321:
	ldr	x1, .LC546
	mov	w2, 5864
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1312
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
	cbz	x0, .L1331
	cbz	x19, .L1332
	ldr	w2, [x0, 672]
	cmp	w2, 20
	bne	.L1333
	ldr	x3, .LC557
	mov	x1, x19
	mov	x2, 72
	add	x0, x0, 600
	ldr	x3, [x3, 104]
	blr	x3
	mov	x1, x19
	mov	w0, w20
	bl	VDH_PutUsrdecFrame
	cbnz	w0, .L1334
.L1325:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1333:
	ldr	x1, .LC556
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1325
	.p2align 3
.L1334:
	ldr	x1, .LC559
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1325
.L1332:
	ldr	x3, .LC554
	mov	w2, 6077
	ldr	x1, .LC552
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1325
.L1331:
	ldr	x3, .LC551
	mov	w2, 6076
	ldr	x1, .LC552
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1325
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
	bhi	.L1389
	ldr	x19, .LC562
	ldr	x0, [x19, w20, sxtw 3]
	cbz	x0, .L1390
	ldr	w1, [x0, 12]
	cmp	w1, 1
	bne	.L1340
	ldr	w0, [x0, 672]
	cmp	w0, 21
	beq	.L1340
	and	w1, w0, -17
	cmp	w1, 4
	cset	w2, eq
	cmp	w0, 7
	cset	w1, eq
	orr	w1, w2, w1
	cbnz	w1, .L1336
	cmp	w0, 14
	beq	.L1336
	mov	x1, 0
	mov	w0, 0
	.p2align 2
.L1345:
	cmp	w1, 15
	bhi	.L1344
	ldr	x2, [x19, x1, lsl 3]
	cbz	x2, .L1344
	ldr	w3, [x2, 12]
	cbz	w3, .L1344
	ldr	w3, [x2, 16]
	cbz	w3, .L1344
	ldr	w3, [x2, 28]
	cbz	w3, .L1344
	ldr	w2, [x2, 672]
	cmp	w2, 21
	and	w3, w2, -17
	beq	.L1344
	cmp	w3, 4
	ccmp	w2, 7, 4, ne
	ccmp	w2, 14, 4, ne
	csinc	w0, w0, w0, eq
.L1344:
	add	x1, x1, 1
	cmp	x1, 16
	bne	.L1345
	cmp	w0, 1
	beq	.L1391
.L1346:
	mov	x0, 0
	mov	w5, 0
	.p2align 2
.L1354:
	ldr	x2, [x19, x0]
	mov	w4, 0
	add	x0, x0, 8
	cbz	x2, .L1352
	ldr	w3, [x2, 672]
	cbnz	w3, .L1353
	ldr	w1, [x2, 1340]
	cmp	w1, 1
	cset	w4, eq
.L1353:
	cmp	w3, 6
	cset	w1, eq
	orr	w1, w4, w1
	cmp	w1, wzr
	csinc	w5, w5, wzr, eq
.L1352:
	cmp	x0, 128
	bne	.L1354
	cmp	w5, 1
	beq	.L1392
.L1355:
	ldr	x0, [x19, w20, sxtw 3]
	ldr	w0, [x0, 672]
	cmp	w0, 1
	bne	.L1336
	ldr	x0, .LC567
	ldrb	w0, [x0]
	cmp	w0, 1
	bne	.L1393
	ldr	x0, .LC568
	ldr	x1, [x0, 56]
	cbz	x1, .L1336
	mov	w0, 0
	blr	x1
	b	.L1336
	.p2align 3
.L1340:
	ldr	x1, .LC566
	mov	w2, 9875
	mov	w0, 0
	bl	dprint_vfmw
.L1336:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1389:
	ldr	x1, .LC561
	mov	w2, 9863
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L1393:
	mov	w1, 0
	mov	w0, 2
	bl	VDH_Need_Module
	b	.L1336
	.p2align 3
.L1392:
	ldr	x0, .LC567
	ldrb	w0, [x0]
	cmp	w0, 1
	bne	.L1356
	ldr	x0, .LC568
	ldr	x1, [x0, 24]
	cbz	x1, .L1394
	mov	w0, 0
	blr	x1
	b	.L1355
	.p2align 3
.L1391:
	ldr	x21, .LC567
	ldrb	w1, [x21]
	cmp	w1, 1
	bne	.L1347
	ldr	x0, .LC568
	ldr	x1, [x0, 8]
	cbz	x1, .L1395
	mov	w0, 0
	blr	x1
.L1349:
	ldrb	w0, [x21]
	cmp	w0, 1
	bne	.L1350
	ldr	x0, .LC568
	ldr	x1, [x0, 40]
	cbz	x1, .L1396
	mov	w0, 0
	blr	x1
	b	.L1346
	.p2align 3
.L1390:
	ldr	x1, .LC564
	mov	w2, 9869
	bl	dprint_vfmw
	b	.L1336
.L1350:
	mov	w1, 0
	mov	w0, 0
	bl	VDH_Need_Module
	b	.L1346
.L1347:
	mov	w1, 0
	bl	VDH_Need_Module
	b	.L1349
.L1356:
	mov	w1, 0
	mov	w0, 3
	bl	VDH_Need_Module
	b	.L1355
.L1394:
	ldr	x1, .LC569
	mov	w2, 9966
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1355
.L1396:
	ldr	x1, .LC569
	mov	w2, 9931
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1346
.L1395:
	ldr	x1, .LC569
	mov	w2, 9917
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1349
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
	bhi	.L1408
	ldr	x19, .LC572
	ldr	x0, [x19, w20, sxtw 3]
	cbz	x0, .L1409
	ldr	w21, [x0, 12]
	cmp	w21, 1
	bne	.L1410
	ldr	w0, [x0, 672]
	cmp	w0, 21
	beq	.L1411
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
	cbnz	w0, .L1403
	str	w21, [x1, 28]
	mov	x1, 0
	ldr	x0, .LC577
	ldr	x5, .LC578
	b	.L1406
	.p2align 3
.L1413:
	ldr	w4, [x2, 28]
	str	w4, [x3, x1, lsl 2]
	ldr	w2, [x2, 28]
	str	w2, [x5, x1, lsl 2]
	add	x1, x1, 1
	cmp	x1, 16
	beq	.L1412
.L1406:
	ldr	x2, [x19, x1, lsl 3]
	sub	x3, x0, #248
	cbnz	x2, .L1413
	sub	x2, x0, #248
	str	wzr, [x5, x1, lsl 2]
	str	wzr, [x2, x1, lsl 2]
	add	x1, x1, 1
	cmp	x1, 16
	bne	.L1406
	.p2align 2
.L1412:
	sub	x0, x0, #248
	bl	VDMDRV_SetPriority
	mov	w0, w20
	bl	VDMDRV_ClearChanIsDec
.L1403:
	mov	w0, w20
	bl	VDMDRV_ClearChanIsDec
	mov	w0, w20
	bl	VCTRL_OpenHardware
	mov	w0, 0
.L1399:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L1410:
	ldr	x3, .LC574
	mov	w2, 2893
	ldr	x1, .LC571
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1399
.L1408:
	ldr	x3, .LC570
	mov	w2, 2891
	ldr	x1, .LC571
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1399
.L1411:
	ldr	x3, .LC576
	mov	w2, 2894
	ldr	x1, .LC571
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1399
.L1409:
	ldr	x3, .LC573
	mov	w2, 2892
	ldr	x1, .LC571
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1399
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
.L1420:
	ldr	x0, [x4, x1]
	cbz	x0, .L1415
	ldr	w2, [x0, 672]
	cbnz	w5, .L1416
	ldr	w3, [x0, 12]
	cmp	w3, 1
	beq	.L1460
.L1417:
	mov	w3, 0
	cbnz	w2, .L1419
	ldr	w3, [x0, 1340]
.L1419:
	cbnz	w19, .L1415
	cmp	w2, 6
	ccmp	w3, 1, 4, ne
	bne	.L1415
	ldr	w2, [x0, 12]
	cmp	w2, 1
	beq	.L1461
	.p2align 2
.L1415:
	add	x1, x1, 8
	cmp	x1, 128
	bne	.L1420
	cbnz	w5, .L1421
	ldr	x22, .LC581
	ldrb	w0, [x22]
	cmp	w0, 1
	beq	.L1426
	mov	w0, w21
	bl	SCDDRV_ResetSCD
	mov	w0, w21
	bl	VDMDRV_ResetVDH
	ldrb	w0, [x22]
	cmp	w0, 1
	beq	.L1426
	mov	w1, 0
	mov	w0, 1
	bl	VDH_UnNeed_Module
	ldrb	w0, [x22]
	cmp	w0, 1
	bne	.L1429
.L1468:
	ldr	x0, .LC582
	ldr	x1, [x0, 48]
	cbz	x1, .L1462
	mov	w0, 0
	blr	x1
.L1432:
	mov	w0, 0
	bl	SCDDRV_SetStateIdle
.L1421:
	cbnz	w19, .L1433
	ldr	x0, .LC581
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L1463
	mov	w1, 0
	mov	w0, 3
	bl	VDH_UnNeed_Module
.L1433:
	cbnz	w20, .L1414
	ldr	x0, .LC581
	ldrb	w0, [x0]
	cmp	w0, 1
	bne	.L1464
	ldr	x0, .LC582
	ldr	x1, [x0, 64]
	cbz	x1, .L1414
	mov	w0, 0
	blr	x1
.L1414:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1416:
	cmp	w20, wzr
	ccmp	w2, 1, 0, eq
	bne	.L1417
	ldr	w3, [x0, 12]
	cmp	w3, 1
	bne	.L1417
	ldr	w3, [x0, 16]
.L1438:
	cmp	w3, 1
	csinc	w20, w20, wzr, ne
	b	.L1417
	.p2align 3
.L1460:
	ldr	w3, [x0, 16]
	cmp	w3, 1
	beq	.L1465
.L1418:
	cmp	w2, 1
	ccmp	w20, wzr, 0, eq
	beq	.L1438
	b	.L1417
	.p2align 3
.L1461:
	ldr	w0, [x0, 16]
	cmp	w0, 1
	cset	w19, eq
	b	.L1415
	.p2align 3
.L1465:
	sub	w6, w2, #5
	mov	w5, w3
	cmp	w6, 1
	ccmp	w2, 3, 0, hi
	cset	w6, ls
	cbnz	w6, .L1418
	sub	w6, w2, #8
	cmp	w6, 5
	bls	.L1418
	sub	w5, w2, #15
	cmp	w5, 2
	cset	w5, ls
	b	.L1418
	.p2align 3
.L1464:
	mov	w1, 0
	mov	w0, 2
	bl	VDH_UnNeed_Module
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1463:
	ldr	x0, .LC582
	ldr	x1, [x0, 32]
	cbz	x1, .L1466
	mov	w0, 0
	blr	x1
	b	.L1433
	.p2align 3
.L1426:
	ldr	x0, .LC582
	ldr	x1, [x0, 16]
	cbz	x1, .L1467
	mov	w0, 0
	blr	x1
.L1428:
	mov	w0, 0
	bl	VDMDRV_SetStateIdle
	ldrb	w0, [x22]
	cmp	w0, 1
	beq	.L1468
.L1429:
	mov	w1, 0
	mov	w0, 0
	bl	VDH_UnNeed_Module
	b	.L1421
.L1467:
	ldr	x1, .LC583
	mov	w2, 10075
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1428
.L1462:
	ldr	x1, .LC583
	mov	w2, 10090
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1432
.L1466:
	ldr	x1, .LC583
	mov	w2, 10108
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1433
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
	bhi	.L1475
	ldr	x20, .LC586
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1476
	ldr	w1, [x0, 12]
	cmp	w1, 1
	bne	.L1474
	str	w1, [x0, 32]
	mov	w0, w19
	ldr	x1, [x20, w19, sxtw 3]
	str	wzr, [x1, 20]
	ldr	x1, [x20, w19, sxtw 3]
	str	wzr, [x1, 16]
	bl	SM_Stop
	cbnz	w0, .L1474
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
.L1471:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1474:
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1475:
	ldr	x3, .LC584
	mov	w2, 3000
	ldr	x1, .LC585
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1471
.L1476:
	ldr	x3, .LC588
	mov	w2, 3001
	ldr	x1, .LC585
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1471
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
	b	.L1479
	.p2align 3
.L1483:
	ldr	x1, [x19, 232]
	mov	w0, 10
	add	w20, w20, 1
	blr	x1
	cmp	w20, 50
	beq	.L1481
.L1479:
	ldr	x1, [x19, 24]
	mov	w0, 0
	blr	x1
	mov	w0, w22
	bl	VCTRL_StopChan
	mov	w21, w0
	ldr	x1, [x19, 32]
	mov	w0, 0
	blr	x1
	cbnz	w21, .L1483
	cmp	w20, 49
	bgt	.L1481
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1481:
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
	cbz	x1, .L1507
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
	ldr	w1, [x0, 1552]
	cmp	w1, 1
	beq	.L1508
.L1487:
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
	cbz	x0, .L1509
	add	x0, x0, 323584
	str	wzr, [x0, 2660]
.L1489:
	ldr	x1, [x20, w19, sxtw 3]
	mov	w0, w19
	str	wzr, [x1, 1328]
	bl	InitSyntaxCtx
	ldr	x0, [x20, w19, sxtw 3]
	mov	x2, 456
	ldr	x3, [x21, 96]
	mov	w1, 0
	add	x0, x0, 816
	blr	x3
	ldr	x0, [x20, w19, sxtw 3]
	mov	x2, -1
	add	x1, x0, 327680
	str	x2, [x0, 736]
	str	x2, [x0, 744]
	str	wzr, [x0, 1272]
	str	wzr, [x1, 2728]
	str	wzr, [x1, 2732]
	str	wzr, [x1, 2736]
	ldr	w2, [x0, 1552]
	cmp	w2, 1
	beq	.L1510
.L1490:
	mov	w0, w19
	bl	FSP_SwitchNotify
	ldr	x0, [x20, w19, sxtw 3]
	ldr	w0, [x0, 672]
	cmp	w0, 20
	mov	w0, w19
	beq	.L1491
	bl	FSP_ResetInstance
.L1492:
	ldr	x1, [x21, 32]
	mov	w0, 4
	blr	x1
	mov	w0, w19
	bl	InitUsdPool
	mov	w0, w19
	bl	SM_Reset
	ldr	x0, .LC597
	ldr	w1, [x0, -184]
	cbz	w1, .L1493
	sub	x20, x0, #176
	str	w19, [x29, 60]
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1495
	mov	w0, 512
	add	x1, x29, 60
	bl	VCTRL_SetDbgOption
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1495
	add	x1, x29, 60
	mov	w0, 512
	bl	VCTRL_SetDbgOption
.L1495:
	ldr	x1, .LC599
	mov	w0, 32
	bl	dprint_vfmw
.L1493:
	cbnz	w22, .L1511
.L1497:
	mov	w0, w19
	bl	VDMDRV_ClearChanIsDec
	mov	w0, 0
.L1486:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1511:
	mov	w0, w19
	bl	VCTRL_StartChan
	b	.L1497
	.p2align 3
.L1491:
	bl	FSP_EmptyInstance
	b	.L1492
	.p2align 3
.L1510:
	str	wzr, [x0, 1592]
	str	wzr, [x1, 2708]
	b	.L1490
	.p2align 3
.L1508:
	add	x1, x0, 327680
	ldr	w2, [x0, 672]
	ldr	w1, [x1, 2752]
	cmp	w1, w2
	bne	.L1487
	str	wzr, [x0, 20]
	mov	w0, w19
	bl	VCTRL_FindOccupiedFrame
	b	.L1487
	.p2align 3
.L1509:
	bl	VCTRL_SetSendStreamAgainFlag.part.2
	b	.L1489
.L1507:
	ldr	x3, .LC593
	mov	w2, 4447
	ldr	x1, .LC594
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1486
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
	tbnz	w0, #31, .L1513
	ldr	w0, [x25, 12]
	cmp	w0, 31
	ble	.L1513
	ldr	w0, [x25, 16]
	cmp	w0, 31
	ble	.L1513
	ldr	w1, [x25, 56]
	mov	w0, 262143
	cmp	w1, w0
	ble	.L1513
	ldr	w0, [x25, 44]
	cmp	w0, 1
	beq	.L1574
	cmp	w0, 2
	beq	.L1575
.L1515:
	ldr	w0, [x25, 176]
	cmp	w0, 1
	bls	.L1518
	str	wzr, [x25, 176]
.L1518:
	mov	w4, 34744
	mov	w19, 1
	cmp	w20, 26
	mov	w26, 44584
	movk	w4, 0xb2, lsl 16
	mov	w3, w21
	add	x2, x29, 120
	mov	x1, x25
	mov	w0, w20
	movk	w26, 0xc, lsl 16
	stp	w19, w19, [x25, 200]
	csel	w26, w26, w4, ne
	bl	VCTRL_GetChanMemSizeWithOption
	cbnz	w0, .L1576
	add	x21, x25, 80
	ldr	x0, [x21, 56]
	cbz	x0, .L1521
	ldr	w0, [x21, 64]
	cbnz	w0, .L1522
.L1521:
	mov	w0, 3
	str	w0, [x29, 260]
	mov	w0, 5
	bl	VFMW_GetString
	ldr	w2, [x29, 128]
	mov	w4, 1
	add	x3, x29, 232
	mov	w1, 0
	bl	MEM_AllocMemBlock
	cbnz	w0, .L1577
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
.L1524:
	ldr	x0, [x21, 8]
	cbnz	x0, .L1578
.L1526:
	ldr	w0, [x25, 176]
	cmp	w0, 1
	beq	.L1528
	mov	w0, 3
	str	w0, [x29, 260]
	mov	w0, 6
	bl	VFMW_GetString
	ldr	w2, [x29, 120]
	mov	w4, 0
	add	x3, x29, 232
	mov	w1, 0
	bl	MEM_AllocMemBlock
	cbnz	w0, .L1579
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
.L1531:
	ldr	x0, [x21, 32]
	cbz	x0, .L1535
	ldr	w0, [x21, 40]
	cbnz	w0, .L1536
.L1535:
	mov	w1, 3
	mov	w0, 7
	str	w1, [x29, 260]
	bl	VFMW_GetString
	ldr	w2, [x29, 124]
	mov	w4, 0
	add	x3, x29, 232
	mov	w1, 0
	bl	MEM_AllocMemBlock
	cbnz	w0, .L1580
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
.L1540:
	ldr	x1, [x22, 24]
	mov	w0, 0
	blr	x1
	ldr	x28, .LC619
	ldr	x0, [x28]
	cbz	x0, .L1554
	add	x0, x28, 8
	mov	w21, 1
	.p2align 2
.L1547:
	ldr	x1, [x0]
	add	x0, x0, 8
	cbz	x1, .L1581
	add	w21, w21, 1
	cmp	w21, 16
	bne	.L1547
	ldr	x1, .LC623
	mov	w2, w21
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x22, 32]
	mov	w0, 0
	blr	x1
	mov	w0, -1
.L1569:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 320
	ret
	.p2align 3
.L1578:
	ldr	w0, [x21, 16]
	cbz	w0, .L1526
	ldr	x3, [x22, 104]
	mov	x2, 24
	add	x0, x29, 136
	mov	x1, x21
	blr	x3
	ldr	w0, [x25, 176]
	ldr	w2, [x29, 152]
	cmp	w0, 1
	beq	.L1533
	ldr	w4, [x29, 120]
	cmp	w2, w4
	blt	.L1582
.L1533:
	ldr	w0, [x29, 136]
	ldr	x1, [x29, 144]
	bl	MEM_AddMemRecord
	str	wzr, [x29, 108]
	b	.L1531
	.p2align 3
.L1528:
	str	wzr, [x29, 108]
	str	wzr, [x29, 136]
	str	xzr, [x29, 144]
	str	wzr, [x29, 152]
	b	.L1531
	.p2align 3
.L1575:
	ldr	w0, [x25, 20]
	cmp	w0, wzr
	ble	.L1513
	ldr	w0, [x25, 24]
	cmp	w0, wzr
	ble	.L1513
.L1571:
	ldr	w0, [x25, 28]
	cmp	w0, wzr
	ble	.L1513
	ldr	w0, [x25, 32]
	cmp	w0, wzr
	bgt	.L1515
.L1513:
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
.L1581:
	sxtb	w0, w21
	str	w0, [x29, 100]
.L1545:
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
	add	x0, x0, 1376
	blr	x4
	ldr	x0, [x28, w21, sxtw 3]
	mov	w2, 21
	ldr	x9, .LC624
	lsl	x6, x27, 4
	ldr	x8, .LC625
	mov	w7, 30
	str	w20, [x0, 44]
	str	w26, [x0, 1300]
	mov	w26, 255
	ldr	x0, [x28, w21, sxtw 3]
	str	w21, [x0, 8]
	str	w20, [x0, 708]
	mov	w20, 0
	str	wzr, [x0, 1320]
	ldr	x0, [x28, w21, sxtw 3]
	ldr	w1, [x25]
	str	w1, [x0, 40]
	mov	w0, w21
	ldr	x1, [x28, w21, sxtw 3]
	add	x10, x1, 327680
	str	w2, [x1, 672]
	str	w2, [x10, 2752]
	str	wzr, [x1, 36]
	ldr	x1, [x28, w21, sxtw 3]
	add	x2, x1, 1632
	str	x2, [x9, x6]
	add	x6, x1, 294912
	add	x6, x6, 2224
	str	x6, [x8, w21, sxtw 3]
	ldr	w6, [x25, 200]
	str	w6, [x1, 1704]
	ldr	w6, [x25, 204]
	str	w6, [x1, 1708]
	str	w7, [x1, 1624]
	bl	VCTRL_ResetChan
	ldr	x0, [x28, w21, sxtw 3]
	ldr	w1, [x29, 108]
	str	w1, [x0, 360]
	ldr	w1, [x29, 104]
	str	w1, [x0, 364]
	add	x1, x0, 323584
	add	x2, x1, 2560
	str	w19, [x0, 368]
	add	x0, x0, 327680
	stp	wzr, wzr, [x2, 76]
	stp	wzr, wzr, [x2, 84]
	str	wzr, [x1, 2652]
	str	wzr, [x1, 2656]
	ldr	w1, [x25, 176]
	str	w1, [x0, 2716]
	str	wzr, [x0, 2708]
	.p2align 2
.L1549:
	uxtw	x1, w20
	ldr	x6, [x28, x27, lsl 3]
	lsl	x19, x1, 6
	ldr	x5, [x22, 96]
	add	x19, x19, x1, lsl 3
	mov	x2, 72
	add	x0, x19, 323584
	mov	w1, 0
	add	x0, x6, x0
	add	w20, w20, 1
	add	x0, x0, 2664
	blr	x5
	ldr	x0, [x28, x27, lsl 3]
	cmp	w20, 32
	add	x1, x0, x19
	add	x1, x1, 323584
	str	w26, [x1, 2664]
	str	w26, [x1, 2668]
	str	w26, [x1, 2672]
	bne	.L1549
	ldr	x5, [x22, 96]
	add	x0, x0, 327680
	mov	x2, 1288
	mov	w1, 0
	add	x0, x0, 872
	add	x24, x24, 88
	blr	x5
	ldr	x0, [x28, x27, lsl 3]
	mov	x2, 24
	ldr	x5, [x22, 96]
	mov	w1, 0
	add	x0, x0, 327680
	add	x0, x0, 2160
	blr	x5
	str	wzr, [x24, x27, lsl 2]
	ldr	x0, [x28, x27, lsl 3]
	ldr	w7, [x29, 184]
	ldr	x4, [x29, 144]
	add	x6, x0, 323584
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
	str	wzr, [x6, 2616]
	ldr	w6, [x0, 1552]
	cmp	w6, 1
	beq	.L1550
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
.L1572:
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
	add	x4, x1, 294912
	add	x2, x1, 278528
	stp	w21, w21, [x1, 248]
	ldrb	w1, [x29, 100]
	strb	w1, [x4, 2224]
	str	w21, [x2, 1864]
	bl	FSP_EnableInstance
	cbnz	w0, .L1583
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
	add	x19, x19, 327680
	blr	x0
	ldr	x1, [x22, 32]
	str	w0, [x19, 2724]
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
.L1574:
	ldr	w0, [x25, 20]
	cmp	w0, wzr
	bgt	.L1571
	b	.L1513
	.p2align 3
.L1522:
	ldr	x3, [x22, 104]
	mov	x2, 24
	add	x0, x29, 184
	add	x1, x25, 128
	blr	x3
	ldr	w0, [x29, 128]
	ldr	w2, [x29, 200]
	cmp	w2, w0
	blt	.L1584
	ldr	w0, [x29, 184]
	mov	w19, 0
	ldr	x1, [x29, 192]
	bl	MEM_AddMemRecord
	b	.L1524
.L1550:
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
	b	.L1572
.L1536:
	ldr	x3, [x22, 104]
	add	x1, x25, 104
	add	x0, x29, 160
	mov	x2, 24
	blr	x3
	ldr	x1, [x29, 168]
	cbz	x1, .L1541
	ldr	w0, [x29, 124]
	ldr	w2, [x29, 176]
	cmp	w2, w0
	blt	.L1541
	ldr	w0, [x29, 160]
	bl	MEM_AddMemRecord
	str	wzr, [x29, 104]
	b	.L1540
.L1580:
	cbnz	w19, .L1585
.L1538:
	ldr	w0, [x29, 108]
	cbnz	w0, .L1586
.L1539:
	ldr	x1, .LC616
	mov	w2, 2363
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1569
.L1586:
	ldr	w0, [x29, 136]
	ldr	x1, [x29, 144]
	bl	MEM_ReleaseMemBlock
	ldr	w0, [x29, 136]
	ldr	w2, [x29, 152]
	ldr	x1, [x29, 144]
	bl	MEM_DelMemRecord
	b	.L1539
.L1579:
	cbnz	w19, .L1587
.L1530:
	ldr	x1, .LC612
	mov	w2, 2305
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1569
.L1554:
	mov	w21, 0
	str	wzr, [x29, 100]
	b	.L1545
.L1577:
	ldr	x1, .LC608
	mov	w2, 2267
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1569
.L1583:
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
	add	x5, x1, 327680
	add	x2, x1, 323584
	ldr	w6, [x1, 364]
	add	x2, x2, 2664
	ldr	w3, [x1, 360]
	ldr	w4, [x1, 368]
	ldr	w5, [x5, 2716]
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
	b	.L1569
.L1541:
	cbnz	w19, .L1588
.L1543:
	ldr	w0, [x29, 108]
	cbnz	w0, .L1589
.L1544:
	ldr	x1, .LC618
	mov	w2, 2392
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1569
.L1576:
	ldr	x1, .LC606
	mov	w2, 2257
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1569
.L1585:
	ldr	w0, [x29, 184]
	ldr	x1, [x29, 192]
	bl	MEM_ReleaseMemBlock
	ldr	w0, [x29, 184]
	ldr	w2, [x29, 200]
	ldr	x1, [x29, 192]
	bl	MEM_DelMemRecord
	b	.L1538
.L1587:
	ldr	w0, [x29, 184]
	ldr	x1, [x29, 192]
	bl	MEM_ReleaseMemBlock
	ldr	w0, [x29, 184]
	ldr	w2, [x29, 200]
	ldr	x1, [x29, 192]
	bl	MEM_DelMemRecord
	b	.L1530
.L1584:
	ldr	x1, .LC610
	mov	w2, 2283
	mov	w0, w19
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1569
.L1582:
	cbnz	w19, .L1590
.L1534:
	ldr	x1, .LC614
	mov	w3, w2
	mov	w0, 1
	mov	w2, 2337
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1569
.L1589:
	ldr	w0, [x29, 136]
	ldr	x1, [x29, 144]
	bl	MEM_ReleaseMemBlock
	ldr	w0, [x29, 136]
	ldr	w2, [x29, 152]
	ldr	x1, [x29, 144]
	bl	MEM_DelMemRecord
	b	.L1544
.L1588:
	ldr	w0, [x29, 184]
	ldr	x1, [x29, 192]
	bl	MEM_ReleaseMemBlock
	ldr	w0, [x29, 184]
	ldr	w2, [x29, 200]
	ldr	x1, [x29, 192]
	bl	MEM_DelMemRecord
	b	.L1543
.L1590:
	ldr	w0, [x29, 184]
	ldr	x1, [x29, 192]
	bl	MEM_ReleaseMemBlock
	ldr	w2, [x29, 200]
	ldr	w0, [x29, 184]
	ldr	x1, [x29, 192]
	bl	MEM_DelMemRecord
	ldr	w4, [x29, 120]
	ldr	w2, [x29, 152]
	b	.L1534
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
	bhi	.L1615
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
	bls	.L1616
	mov	w4, 8
	mov	w3, 6
.L1597:
	lsl	x0, x1, 3
	sub	x0, x0, x1
	add	x0, x2, x0, lsl 2
	ldr	w0, [x0, 12]
	cmp	w0, w4
	bgt	.L1598
.L1620:
	mov	w0, 1
	str	w0, [x29, 164]
.L1599:
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
	beq	.L1617
	cmp	w19, 26
	beq	.L1618
	cmp	w19, 18
	bne	.L1601
	mov	w1, 4
	mov	w0, 2
	str	w1, [x29, 164]
	str	w0, [x29, 188]
.L1601:
	add	x2, x29, 64
	add	x1, x29, 48
	mov	w0, w19
	bl	VCTRL_GetChanMemSize
	cbnz	w0, .L1619
	cbz	x21, .L1608
	ldr	w0, [x21, 16]
	cbz	w0, .L1608
	ldr	w2, [x21]
	cbz	w2, .L1605
	ldr	x5, [x21, 8]
	cbz	x5, .L1605
	ldr	w3, [x29, 48]
	ldr	w1, [x29, 64]
	add	w1, w3, w1
	cmp	w0, w1
	blt	.L1605
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
.L1604:
	mov	w2, 1
	add	x1, x29, 128
	mov	w0, w19
	bl	VCTRL_CreateChanWithOption
	cmp	w0, wzr
	csinv	w0, w0, wzr, ge
.L1593:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 336
	ret
	.p2align 3
.L1616:
	ldr	x3, .LC631
	ldrb	w0, [x3,w0,uxtw]
	adr	x3, .Lrtx1596
	add	x0, x3, w0, sxtb #2
	br	x0
.Lrtx1596:
	.section	.rodata
	.align	0
	.align	2
.L1596:
	.byte	(.L1595 - .Lrtx1596) / 4
	.byte	(.L1595 - .Lrtx1596) / 4
	.byte	(.L1595 - .Lrtx1596) / 4
	.byte	(.L1595 - .Lrtx1596) / 4
	.byte	(.L1595 - .Lrtx1596) / 4
	.byte	(.L1606 - .Lrtx1596) / 4
	.byte	(.L1606 - .Lrtx1596) / 4
	.byte	(.L1606 - .Lrtx1596) / 4
	.text
	.p2align 3
.L1606:
	lsl	x0, x1, 3
	mov	w4, 3
	sub	x0, x0, x1
	mov	w3, 1
	add	x0, x2, x0, lsl 2
	ldr	w0, [x0, 12]
	cmp	w0, w4
	ble	.L1620
.L1598:
	sub	w0, w0, w3
	sub	w0, w0, #2
	str	w0, [x29, 164]
	b	.L1599
	.p2align 3
.L1595:
	mov	w4, 5
	mov	w3, 3
	b	.L1597
	.p2align 3
.L1608:
	mov	w3, 0
	b	.L1604
	.p2align 3
.L1618:
	mov	w1, 16
	mov	w0, 6
	str	w1, [x29, 164]
	str	w0, [x29, 188]
	b	.L1601
	.p2align 3
.L1617:
	str	wzr, [x29, 164]
	str	w3, [x29, 188]
	b	.L1601
	.p2align 3
.L1605:
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
	b	.L1593
	.p2align 3
.L1615:
	ldr	x1, .LC628
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1593
	.p2align 3
.L1619:
	ldr	x1, .LC633
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1593
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
	.xword	.L1596
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
	mov	x22, x1
	bl	_mcount
	str	wzr, [x29, 84]
	cmp	w20, 15
	bhi	.L1664
	ldr	x19, .LC642
	ldr	x0, [x19, w20, sxtw 3]
	cbz	x0, .L1665
	ldr	w0, [x0, 12]
	cmp	w0, 1
	bne	.L1666
	mov	x1, x22
	mov	w0, w20
	bl	VCTRL_CheckChanWH
	mov	w21, w0
	cbnz	w0, .L1667
	ldr	x23, .LC647
	add	x24, x23, 152
	mov	x0, x24
	bl	PDT_GetVdecCapability
	ldr	w0, [x23, 172]
	ldr	w2, [x22]
	cmp	w0, 20
	bhi	.L1628
	cmp	w0, w2
	beq	.L1629
	add	x0, x23, 176
	add	x3, x23, 300
	.p2align 2
.L1631:
	ldr	w1, [x0]
	add	x0, x0, 4
	cmp	w1, 20
	bhi	.L1628
	cmp	w1, w2
	beq	.L1629
	cmp	x3, x0
	bne	.L1631
.L1629:
	cmp	w2, 8
	beq	.L1668
.L1632:
	cmp	w2, 9
	beq	.L1634
.L1638:
	mov	x1, x22
	mov	w0, w20
	bl	VCTRL_CmpConfigParameter
	cbz	w0, .L1669
	ldr	x24, .LC652
	mov	x2, 120
	ldr	x0, [x19, w20, sxtw 3]
	mov	x1, x22
	ldr	x3, [x24, 104]
	add	x0, x0, 52
	blr	x3
	ldr	x0, [x19, w20, sxtw 3]
	mov	x2, 72
	ldr	w1, [x22, 16]
	str	w1, [x0, 28]
	ldr	x0, [x19, w20, sxtw 3]
	ldr	w1, [x22]
	ldr	x3, [x24, 96]
	str	w1, [x0, 672]
	ldr	w1, [x22, 104]
	str	w1, [x0, 1560]
	mov	w1, 0
	add	x0, x29, 88
	blr	x3
	ldr	x0, [x19, w20, sxtw 3]
	ldr	w0, [x0, 392]
	str	w0, [x29, 104]
	bl	MEM_Phy2Vir
	str	x0, [x29, 88]
	ldr	x1, [x19, w20, sxtw 3]
	ldr	w2, [x22, 16]
	strb	w2, [x29, 96]
	ldr	w2, [x1, 404]
	mov	w1, 49152
	movk	w1, 0xf8c3, lsl 16
	ldr	w0, [x22]
	stp	w2, w1, [x29, 108]
	ldr	w1, [x22, 92]
	str	w0, [x29, 100]
	cmp	w1, 1
	beq	.L1670
	cbz	w0, .L1671
.L1640:
	ldr	w1, [x22, 88]
	cmp	w0, 16
	str	wzr, [x29, 128]
	str	w1, [x29, 136]
	bne	.L1672
	ldr	w0, [x22, 56]
.L1641:
	str	w0, [x29, 132]
	mov	w1, 0
	mov	x0, 0
	.p2align 2
.L1643:
	ldr	x2, [x19, x0]
	add	x0, x0, 8
	cmp	x2, xzr
	csinc	w1, w1, w1, eq
	cmp	x0, 128
	bne	.L1643
	cmp	w1, 1
	ble	.L1644
	str	wzr, [x29, 132]
.L1644:
	ldr	x3, [x24, 104]
	add	x26, x22, 4
	mov	x2, 12
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
	ldr	w2, [x22, 92]
	cmp	w2, 1
	beq	.L1673
.L1645:
	ldr	x0, [x19, w20, sxtw 3]
	mov	w25, 1
	mov	x1, x26
	ldr	w2, [x0, 344]
	str	w2, [x0, 1304]
	ldr	w2, [x0, 352]
	str	w2, [x0, 1308]
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
	ldr	w2, [x22, 20]
	str	w2, [x0, 684]
	ldr	w2, [x22, 20]
	str	w2, [x0, 680]
	ldr	w2, [x22, 28]
	str	w2, [x0, 688]
	ldr	w2, [x22, 32]
	str	w2, [x0, 692]
	ldr	w2, [x22, 36]
	str	w2, [x0, 696]
	ldr	w2, [x22, 60]
	str	w2, [x0, 1312]
	ldr	w2, [x22, 68]
	str	w2, [x0, 1316]
	str	w25, [x0, 1348]
	str	w25, [x0, 1352]
	ldr	x3, [x24, 104]
	ldr	w2, [x22, 80]
	cmp	w2, w25
	cset	w2, eq
	str	w2, [x0, 1332]
	ldr	w2, [x22, 84]
	str	w2, [x0, 1336]
	ldr	w2, [x22, 52]
	str	w2, [x0, 1340]
	ldr	w2, [x29, 132]
	str	w2, [x0, 1344]
	add	x2, x0, 327680
	str	wzr, [x0, 1592]
	add	x0, x0, 1276
	str	wzr, [x0, 336]
	ldrsb	w4, [x22, 117]
	strb	w4, [x2, 2712]
	mov	x2, 12
	blr	x3
	ldr	x0, [x19, w20, sxtw 3]
	add	x1, x0, 327680
	ldr	w2, [x0, 672]
	ldr	w0, [x1, 2752]
	cmp	w0, w2
	beq	.L1646
	mov	w0, w20
	bl	VCTRL_ResetChan
	ldr	w1, [x22]
	ldr	x0, [x19, w20, sxtw 3]
	cmp	w1, 20
	beq	.L1647
	ldr	w2, [x0, 672]
	add	x1, x0, 327680
.L1646:
	ldr	x4, .LC655
	mov	x0, 0
	str	w2, [x1, 2752]
	b	.L1651
	.p2align 3
.L1675:
	ldr	w3, [x1, 28]
	str	w3, [x2, x0, lsl 2]
	ldr	w1, [x1, 28]
	str	w1, [x4, x0, lsl 2]
	add	x0, x0, 1
	cmp	x0, 16
	beq	.L1674
.L1651:
	ldr	x1, [x19, x0, lsl 3]
	add	x2, x23, 304
	cbnz	x1, .L1675
	add	x1, x23, 304
	str	wzr, [x4, x0, lsl 2]
	str	wzr, [x1, x0, lsl 2]
	add	x0, x0, 1
	cmp	x0, 16
	bne	.L1651
.L1674:
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
.L1669:
	ldr	x1, .LC651
	mov	w21, 0
	bl	dprint_vfmw
.L1661:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 160
	ret
	.p2align 3
.L1628:
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
.L1671:
	ldr	w0, [x22, 88]
	ldr	w1, [x22, 52]
	str	w0, [x29, 136]
	str	w1, [x29, 128]
	ldr	w0, [x22, 56]
	b	.L1641
	.p2align 3
.L1670:
	ldr	w2, [x22, 96]
	stp	w1, w2, [x29, 140]
	ldr	w1, [x22, 100]
	str	w1, [x29, 148]
	cbnz	w0, .L1640
	b	.L1671
.L1668:
	add	x0, x29, 84
	bl	REAL8DEC_Support
	ldr	w0, [x29, 84]
	cbz	w0, .L1637
	ldr	w2, [x22]
	b	.L1632
.L1634:
	add	x0, x29, 84
	bl	REAL9DEC_Support
	ldr	w0, [x29, 84]
	cbnz	w0, .L1638
.L1637:
	mov	w21, -1
	b	.L1661
	.p2align 3
.L1666:
	ldr	x3, .LC644
	mov	w2, 3250
	ldr	x1, .LC641
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L1661
.L1672:
	mov	w0, 0
	b	.L1641
.L1673:
	add	x1, x29, 80
	mov	w0, w20
	strb	w2, [x29, 80]
	bl	FSP_ConfigAttribute
	b	.L1645
.L1667:
	ldr	x1, .LC646
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L1661
.L1647:
	ldr	w3, [x0, 396]
	ldr	w4, [x0, 400]
	ldr	w1, [x0, 1388]
	ldr	w2, [x0, 1392]
	mov	w0, w20
	bl	VCTRL_PartitionMemForStdUser
	cbnz	w0, .L1648
	ldr	x0, [x19, w20, sxtw 3]
	add	x1, x0, 327680
	ldr	w2, [x0, 672]
	b	.L1646
.L1664:
	ldr	x3, .LC640
	mov	w2, 3248
	ldr	x1, .LC641
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L1661
.L1665:
	ldr	x3, .LC643
	mov	w2, 3249
	ldr	x1, .LC641
	mov	w21, -1
	bl	dprint_vfmw
	b	.L1661
.L1648:
	ldr	x1, .LC654
	mov	w0, w25
	mov	w21, -1
	bl	dprint_vfmw
	b	.L1661
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
	cbz	x21, .L1769
	cmp	w19, 544
	ldr	w20, [x21]
	beq	.L1678
	bls	.L1794
	cmp	w19, 1280
	beq	.L1701
	bhi	.L1702
	cmp	w19, 897
	beq	.L1703
	bls	.L1795
	cmp	w19, 1024
	beq	.L1710
	bhi	.L1711
	cmp	w19, 898
	beq	.L1792
	cmp	w19, 899
	bne	.L1769
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
.L1792:
	mov	w0, w20
	bl	VCTRL_StartChan
	mov	w3, w0
	ldr	x1, .LC741
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	.p2align 2
.L1793:
	mov	w0, 1
.L1677:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L1702:
	cmp	w19, 1537
	beq	.L1716
	bls	.L1796
	cmp	w19, 1540
	beq	.L1722
	bhi	.L1723
	cmp	w19, 1538
	beq	.L1724
	cmp	w19, 1539
	bne	.L1769
	ldr	x2, .LC767
	ubfiz	x20, x20, 4, 32
	mov	w1, 1
	mov	w0, w1
	ldr	x2, [x2, x20]
	str	w1, [x2, 48]
	b	.L1677
	.p2align 3
.L1794:
	cmp	w19, 11
	beq	.L1680
	bls	.L1797
	cmp	w19, 512
	beq	.L1689
	bls	.L1798
	cmp	w19, 515
	beq	.L1695
	bhi	.L1696
	cmp	w19, 513
	beq	.L1697
	cmp	w19, 514
	bne	.L1769
	cmp	w20, 15
	bhi	.L1793
	ldr	x19, .LC676
	add	x21, x19, 648
	ldr	x0, [x21, w20, uxtw 3]
	cbz	x0, .L1799
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
	cbz	w0, .L1793
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
	b	.L1677
	.p2align 3
.L1798:
	cmp	w19, 13
	beq	.L1691
	bcc	.L1692
	cmp	w19, 256
	beq	.L1693
	cmp	w19, 257
	bne	.L1769
	ldr	x19, .LC659
	mov	w0, w20
	mov	w2, 67108864
	mov	w1, 1
	ldr	w20, [x19]
	str	w2, [x19]
	bl	PrintScdRawState
	mov	w0, 1
	str	w20, [x19]
	b	.L1677
	.p2align 3
.L1795:
	cmp	w19, 768
	beq	.L1705
	bls	.L1800
	cmp	w19, 769
	beq	.L1708
	cmp	w19, 896
	bne	.L1769
	mov	w0, w20
	bl	VCTRL_StopChan
	mov	w3, w0
	ldr	x1, .LC737
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1796:
	cmp	w19, 1282
	beq	.L1718
	bcc	.L1719
	cmp	w19, 1283
	beq	.L1720
	cmp	w19, 1536
	bne	.L1769
	cmp	w20, 15
	bhi	.L1766
	ldr	x3, .LC763
	mov	w19, 1
	ldr	x1, .LC765
	mov	w2, w20
	mov	w0, 32
	ldr	x3, [x3, w20, uxtw 3]
	str	w19, [x3, 1336]
	bl	dprint_vfmw
	mov	w0, w19
	b	.L1677
	.p2align 3
.L1797:
	cmp	w19, 4
	beq	.L1682
	bhi	.L1683
	cmp	w19, 1
	beq	.L1793
	cbz	w19, .L1685
	cmp	w19, 2
	bne	.L1801
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
	cbnz	w0, .L1793
	add	x1, x29, 88
	mov	w0, w19
	str	w20, [x29, 108]
	bl	VCTRL_ConfigChan
	b	.L1793
	.p2align 3
.L1683:
	cmp	w19, 6
	beq	.L1793
	bcc	.L1687
	cmp	w19, 7
	bne	.L1802
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
	cbnz	w0, .L1793
	add	x1, x29, 88
	mov	w0, w19
	str	w20, [x29, 112]
	bl	VCTRL_ConfigChan
	b	.L1793
	.p2align 3
.L1711:
	cmp	w19, 1025
	beq	.L1714
	cmp	w19, 1026
	bne	.L1769
	ldr	x3, .LC742
	mov	w2, w20
	ldr	x1, .LC748
	mov	w0, 32
	str	w20, [x3, 3644]
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1696:
	cmp	w19, 516
	beq	.L1699
	cmp	w19, 528
	bne	.L1769
	cmp	w20, 15
	bhi	.L1793
	ldr	x21, .LC676
	add	x19, x21, 976
	ldr	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1803
.L1759:
	ldr	x1, .LC723
	b	.L1787
	.p2align 3
.L1723:
	cmp	w19, 1541
	beq	.L1726
	cmp	w19, 1792
	bne	.L1769
	cbnz	w20, .L1793
	ldr	x20, .LC687
	ldr	w19, [x20, 76]
	cmp	w19, 1
	beq	.L1804
	ldr	x1, .LC773
	mov	w0, 32
	bl	dprint_vfmw
	mov	w1, 1
	str	w1, [x20, 76]
	mov	w0, w1
	b	.L1677
	.p2align 3
.L1714:
	ldr	x3, .LC676
	mov	w2, w20
	ldr	x1, .LC746
	mov	w0, 32
	str	w20, [x3, 1768]
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1724:
	ldr	x3, .LC766
	ldr	x1, .LC765
.L1786:
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
.L1697:
	cmp	w20, 15
	bhi	.L1793
	ldr	x21, .LC676
	add	x19, x21, 384
	ldr	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1805
	ldr	x1, .LC699
.L1787:
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x1, .LC662
	ldr	x0, [x19, w20, uxtw 3]
	ldr	x1, [x1, 72]
	blr	x1
	str	xzr, [x19, w20, uxtw 3]
	b	.L1793
	.p2align 3
.L1720:
	ldr	x3, .LC758
	ldr	x1, .LC760
	b	.L1786
	.p2align 3
.L1693:
	ldr	x1, .LC686
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, w20
	bl	DBG_PrintTracer
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1708:
	bl	VCTRL_Resume
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1685:
	ldr	x1, .LC659
	mov	w0, 1
	str	w20, [x1]
	b	.L1677
	.p2align 3
.L1726:
	ldr	x3, .LC676
	mov	w2, w20
	ldr	x1, .LC769
	mov	w0, 32
	str	w20, [x3, 1772]
	bl	dprint_vfmw
	b	.L1793
	.p2align 3
.L1687:
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
	cbnz	w0, .L1730
	mov	x1, x21
	mov	w0, w19
	str	w20, [x29, 116]
	bl	VCTRL_ConfigChan
	b	.L1793
	.p2align 3
.L1699:
	ldr	x19, .LC662
	mov	w0, w20
	ldp	w23, w24, [x21, 4]
	ldr	x1, [x19, 384]
	blr	x1
	mov	x22, x0
	cbz	x0, .L1806
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
	cbz	x0, .L1753
	ldr	x1, .LC714
	ldr	x5, [x1, 304]
	cbz	x5, .L1754
	mov	w4, 1
	mov	w3, w24
	mov	w2, w23
	mov	x1, x22
	blr	x5
.L1755:
	ldr	x1, [x19, 72]
	mov	x0, x20
	blr	x1
	ldr	x1, .LC717
	mov	x2, x21
	mov	w0, 32
	bl	dprint_vfmw
.L1752:
	ldr	x1, [x19, 304]
	mov	x0, x22
	blr	x1
	b	.L1793
.L1803:
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
	cbz	x0, .L1757
	ldr	x1, .LC719
	mov	w3, w20
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
.L1801:
	cmp	w19, 3
	beq	.L1793
	.p2align 2
.L1769:
	mov	w0, -2
	b	.L1677
	.p2align 3
.L1800:
	cmp	w19, 560
	bne	.L1769
	cmp	w20, 15
	bhi	.L1793
	ldr	x21, .LC676
	add	x19, x21, 1504
	ldr	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1807
	ldr	x1, .LC735
	b	.L1787
	.p2align 3
.L1705:
	bl	VCTRL_Suspend
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1682:
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
	cbnz	w0, .L1793
	add	x1, x29, 88
	mov	w0, w19
	str	w20, [x29, 120]
	bl	VCTRL_ConfigChan
	b	.L1793
	.p2align 3
.L1695:
	ldr	x19, .LC662
	mov	w0, w20
	ldr	x1, [x19, 384]
	blr	x1
	mov	x22, x0
	cbz	x0, .L1808
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
	cbz	x0, .L1750
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
.L1749:
	ldr	x1, [x19, 304]
	mov	x0, x22
	blr	x1
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1766:
	ldr	x1, .LC762
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1722:
	ldr	x1, .LC767
	ubfiz	x20, x20, 4, 32
	mov	w0, 1
	ldr	x1, [x1, x20]
	str	wzr, [x1, 48]
	b	.L1677
	.p2align 3
.L1710:
	ldr	x3, .LC742
	mov	w2, w20
	ldr	x1, .LC744
	mov	w0, 32
	str	w20, [x3, 3640]
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1716:
	cmp	w20, 15
	bhi	.L1766
	ldr	x3, .LC763
	mov	w2, w20
	ldr	x1, .LC765
	mov	w0, 32
	ldr	x3, [x3, w20, uxtw 3]
	str	wzr, [x3, 1336]
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1703:
	mov	w0, w20
	bl	VCTRL_ResetChan
	mov	w3, w0
	ldr	x1, .LC739
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1680:
	cmp	w20, 1
	bls	.L1809
	ldr	x1, .LC675
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1689:
	cmp	w20, 15
	bhi	.L1793
	ldr	x21, .LC676
	sub	x19, x21, #176
	ldr	x0, [x19, w20, uxtw 3]
	cbz	x0, .L1810
	ldr	x1, .LC693
	b	.L1787
	.p2align 3
.L1701:
	ldr	x3, .LC749
	ldr	x1, .LC751
	b	.L1786
	.p2align 3
.L1719:
	ldr	x3, .LC752
	ldr	x1, .LC754
	b	.L1786
	.p2align 3
.L1718:
	ldr	x3, .LC755
	ldr	x1, .LC757
	b	.L1786
	.p2align 3
.L1692:
	cmp	w20, 1
	bls	.L1811
	ldr	x1, .LC680
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1677
	.p2align 3
.L1691:
	cmp	w20, 16
	bls	.L1735
	ldr	x1, .LC682
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
	.p2align 3
.L1678:
	cmp	w20, 15
	bhi	.L1793
	ldr	x21, .LC676
	add	x19, x21, 1240
	ldr	x0, [x19, w20, uxtw 3]
	cbnz	x0, .L1759
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
	cbz	x0, .L1760
	ldr	x1, .LC725
	mov	w3, w20
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
	.p2align 3
.L1730:
	ldr	x1, .LC668
	mov	w2, w19
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
	.p2align 3
.L1735:
	ldr	x3, .LC676
	mov	w2, w20
	ldr	x1, .LC684
	mov	w0, 32
	str	w20, [x3, 368]
	bl	dprint_vfmw
	mov	w0, 1
	b	.L1677
.L1802:
	cmp	w19, 8
	beq	.L1793
	mov	w0, -2
	b	.L1677
.L1809:
	ldr	x3, .LC671
	ldr	x1, .LC673
	b	.L1786
.L1811:
	ldr	x3, .LC676
	mov	w2, w20
	ldr	x1, .LC678
	mov	w0, 32
	str	w20, [x3, -184]
	bl	dprint_vfmw
	b	.L1793
.L1804:
	ldr	x1, .LC771
	mov	w0, 32
	bl	dprint_vfmw
	str	wzr, [x20, 76]
	mov	w0, w19
	b	.L1677
.L1806:
	ldr	x1, .LC709
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1752
.L1808:
	ldr	x1, .LC709
	mov	w2, w20
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1749
.L1810:
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
	cbz	x0, .L1738
	ldr	x1, .LC689
	mov	w3, w20
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
.L1807:
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
	cbz	x0, .L1763
	ldr	x1, .LC731
	mov	w3, w20
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
.L1805:
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
	cbz	x0, .L1742
	ldr	x1, .LC695
	mov	w3, w20
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
.L1750:
	ldr	x1, .LC713
	mov	x2, x21
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1749
.L1754:
	ldr	x1, .LC715
	mov	w2, 9005
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1755
.L1799:
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
	cbz	x0, .L1745
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
	cbz	w0, .L1746
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
	b	.L1793
.L1753:
	ldr	x1, .LC713
	mov	x2, x21
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1752
.L1746:
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
	b	.L1793
.L1757:
	ldr	x1, .LC721
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
.L1742:
	ldr	x1, .LC697
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
.L1760:
	ldr	x1, .LC727
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
.L1738:
	ldr	x1, .LC691
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
.L1763:
	ldr	x1, .LC733
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
.L1745:
	ldr	x1, .LC705
	add	x2, x29, 88
	mov	w0, 32
	bl	dprint_vfmw
	b	.L1793
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
	cbz	x1, .L1839
	ldr	w0, [x22]
	cmp	w0, 1
	bhi	.L1840
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
	cbz	x0, .L1841
	add	x0, x0, 323584
	str	wzr, [x0, 2660]
.L1817:
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
	str	w2, [x1, 1328]
	bl	InitSyntaxCtx
	ldr	x0, [x20, w19, sxtw 3]
	add	x1, x0, 327680
	ldr	w0, [x0, 672]
	add	x1, x1, 2760
	bl	ReleaseCurPic
	ldr	x0, [x20, w19, sxtw 3]
	mov	x2, 456
	ldr	x3, [x24, 96]
	mov	w1, 0
	add	x0, x0, 816
	blr	x3
	ldr	x0, [x20, w19, sxtw 3]
	mov	x1, -1
	ldr	w2, [x0, 1552]
	str	x1, [x0, 736]
	cmp	w2, 1
	str	x1, [x0, 744]
	str	wzr, [x0, 1272]
	beq	.L1842
.L1818:
	ldr	w0, [x0, 672]
	cmp	w0, 20
	beq	.L1819
	ldr	w0, [x22, 8]
	cbz	w0, .L1843
.L1819:
	mov	w0, w19
	bl	FSP_EmptyInstance
.L1820:
	mov	w0, w19
	bl	InitUsdPool
	ldr	w0, [x22]
	cbz	w0, .L1844
.L1821:
	ldr	x0, .LC788
	ldr	w1, [x0, -184]
	cbz	w1, .L1822
	sub	x20, x0, #176
	str	w19, [x29, 76]
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1824
	mov	w0, 512
	add	x1, x29, 76
	bl	VCTRL_SetDbgOption
	ldr	x0, [x20, w19, sxtw 3]
	cbz	x0, .L1824
	add	x1, x29, 76
	mov	w0, 512
	bl	VCTRL_SetDbgOption
.L1824:
	ldr	x1, .LC789
	mov	w0, 32
	bl	dprint_vfmw
.L1822:
	cbnz	w23, .L1845
.L1826:
	mov	w0, w19
	bl	VDMDRV_ClearChanIsDec
	mov	w0, 0
.L1814:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1844:
	mov	w0, w19
	bl	SM_Reset
	b	.L1821
	.p2align 3
.L1845:
	mov	w0, w19
	bl	VCTRL_StartChan
	b	.L1826
	.p2align 3
.L1842:
	str	wzr, [x0, 1592]
	b	.L1818
	.p2align 3
.L1843:
	mov	w0, w19
	bl	FSP_ResetInstance
	b	.L1820
	.p2align 3
.L1840:
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
.L1841:
	bl	VCTRL_SetSendStreamAgainFlag.part.2
	b	.L1817
.L1839:
	ldr	x3, .LC782
	mov	w2, 4047
	ldr	x1, .LC783
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1814
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
	bhi	.L1857
	ldr	x21, .LC796
	ldr	x0, [x21, w19, sxtw 3]
	cbz	x0, .L1858
	ldr	w22, [x0, 12]
	cmp	w22, 1
	bne	.L1859
	str	wzr, [x0, 16]
	mov	w0, 0
	ldr	x1, [x20, 32]
	blr	x1
	mov	w0, w19
	bl	SCDDRV_IsChanDec
	cbnz	w0, .L1860
.L1851:
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
	cbnz	w0, .L1861
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
	add	x2, x1, 327680
	ldr	x3, .LC804
	ldr	w5, [x1, 368]
	str	w5, [x29, 116]
	ldr	w2, [x2, 2716]
	ldr	w5, [x1, 360]
	str	w2, [x29, 104]
	add	x2, x1, 264
	str	w5, [x29, 108]
	str	x2, [x29, 72]
	add	x2, x1, 323584
	ldr	w5, [x1, 364]
	add	x2, x2, 2664
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
.L1848:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L1860:
	mov	w0, w19
	ldr	x23, [x20, 256]
	bl	SCDDRV_IsChanDec
	cmp	w0, wzr
	mov	w2, 500
	cset	w1, eq
	mov	w0, w22
	blr	x23
	b	.L1851
	.p2align 3
.L1859:
	ldr	x3, .LC800
	mov	w2, 2764
	ldr	x1, .LC795
	mov	w0, 0
.L1856:
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
.L1861:
	mov	w0, 4
	blr	x1
	ldr	x1, [x20, 32]
	mov	w0, 0
	blr	x1
	mov	w0, -1
	b	.L1848
	.p2align 3
.L1857:
	ldr	x3, .LC793
	mov	w2, 2740
	ldr	x1, .LC795
	mov	w0, 0
	b	.L1856
.L1858:
	ldr	x3, .LC798
	mov	w2, 2750
	ldr	x1, .LC795
	b	.L1856
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
	bne	.L1870
	mov	x19, 0
	.p2align 2
.L1866:
	ldr	x1, [x20, x19, lsl 3]
	mov	w0, w19
	add	x19, x19, 1
	cbz	x1, .L1865
	bl	VCTRL_DestroyChanWithOption
.L1865:
	cmp	x19, 16
	bne	.L1866
	bl	FSP_Exit
	ldr	x21, .LC807
	ldr	w1, [x20, 132]
	mov	w2, 3
	str	w2, [x20, 136]
	cbz	w1, .L1868
	ldr	x1, [x21, 232]
	mov	w0, 10
	mov	w19, 49
	blr	x1
	b	.L1869
	.p2align 3
.L1880:
	ldr	x1, [x21, 232]
	blr	x1
	subs	w19, w19, #1
	beq	.L1868
.L1869:
	ldr	w1, [x20, 132]
	mov	w0, 10
	cbnz	w1, .L1880
.L1868:
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
.L1879:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L1870:
	mov	w0, -1
	b	.L1879
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
	beq	.L1891
	mov	w19, 2
	cmp	w20, wzr
	csel	w19, w19, wzr, eq
.L1882:
	ldr	x0, .LC810
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L1893
	mov	w1, w19
	mov	w0, 0
	bl	VDH_Require_DspCode
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1893:
	ldr	x20, .LC811
	ldr	x1, [x20, 8]
	cbz	x1, .L1894
	mov	w0, 0
	blr	x1
.L1885:
	ldr	x1, [x20, 40]
	cbz	x1, .L1895
	mov	w0, 0
	blr	x1
.L1887:
	ldr	x1, [x20, 24]
	cbz	x1, .L1896
	mov	w0, 0
	blr	x1
.L1889:
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
.L1891:
	mov	w19, 1
	b	.L1882
	.p2align 3
.L1894:
	ldr	x1, .LC812
	mov	w2, 3799
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1885
	.p2align 3
.L1895:
	ldr	x1, .LC812
	mov	w2, 3800
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1887
	.p2align 3
.L1896:
	ldr	x1, .LC812
	mov	w2, 3801
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1889
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
	cbnz	w1, .L1909
	ldr	w20, [x0, 1340]
	mov	w0, 0
	cmp	w20, wzr
	cset	w3, eq
	cmp	w1, 6
	cset	w2, ne
	tst	w3, w2
	beq	.L1928
.L1915:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1909:
	cmp	w1, 6
	mov	w3, 1
	cset	w2, ne
	mov	w20, 0
	tst	w3, w2
	mov	w0, 0
	bne	.L1915
.L1928:
	ldr	x21, .LC814
	add	x0, x21, 3388
	add	x3, x21, 3448
	ldr	w2, [x21, 3384]
	cbnz	w2, .L1899
	.p2align 2
.L1900:
	ldr	w2, [x0]
	add	x0, x0, 4
	cmp	x3, x0
	cbnz	w2, .L1899
	bne	.L1900
	mov	w0, w19
	bl	VCTRL_InitDspCode
	cmp	w20, 1
	beq	.L1929
	add	x21, x21, 3384
	mov	w1, 2
	mov	w0, 0
	str	w1, [x21, w19, sxtw 2]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1899:
	cmp	w2, 1
	beq	.L1930
	cmp	w1, 6
	mov	w0, 0
	beq	.L1915
	ldr	x1, .LC818
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1915
	.p2align 3
.L1930:
	cmp	w20, 1
	mov	w0, 0
	beq	.L1915
	ldr	x1, .LC816
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1915
	.p2align 3
.L1929:
	add	x21, x21, 3384
	mov	w0, 0
	str	w20, [x21, w19, sxtw 2]
	b	.L1915
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
	add	x0, x0, 323584
	str	w19, [x0, 2616]
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
	add	x0, x0, 323584
	ldp	x29, x30, [sp], 32
	ldr	w0, [x0, 2636]
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
	beq	.L1935
	cbz	w19, .L1936
	cmp	w19, 2
	bne	.L1944
	ldr	x0, .LC823
	add	x0, x0, 88
	ldr	w1, [x0, w20, sxtw 2]
	cmp	w1, 1
	beq	.L1940
.L1942:
	str	w19, [x0, w20, sxtw 2]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1944:
	mov	w0, 0
.L1934:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1936:
	ldr	x1, .LC823
	mov	w0, 0
	add	x1, x1, 88
	str	wzr, [x1, w20, sxtw 2]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1935:
	ldr	x0, .LC823
	add	x0, x0, 88
	ldr	w1, [x0, w20, sxtw 2]
	cbz	w1, .L1942
.L1940:
	mov	w0, -1
	b	.L1934
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
	add	x1, x0, 323584
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
	str	w0, [x1, 2644]
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
	ldr	x0, .LC826
	mov	w2, 32
	ldr	x20, [x22, w19, sxtw 3]
	ldr	x26, [x0, w19, sxtw 3]
	add	x1, x20, 323584
	add	x25, x20, 327680
	ldr	w0, [x1, 2644]
	ldr	w5, [x25, 2152]
	cmp	w0, w2
	ldr	w21, [x25, 2144]
	csel	w0, w0, w2, le
	str	w0, [x25, 1952]
	ldr	w4, [x24, 12]
	cmp	w4, w5
	beq	.L1947
	ldr	w0, [x1, 2632]
	cbz	w0, .L2023
.L1947:
	ldr	w0, [x25, 2120]
	cbz	w0, .L1949
	ldr	w0, [x25, 2184]
	cbz	w0, .L2024
.L1949:
	ldr	x23, .LC833
	mov	w0, 8
	ldr	x1, [x23, 24]
	blr	x1
	ldr	x0, [x22, w19, sxtw 3]
	add	x2, x0, 323584
	ldr	w1, [x2, 2664]
	cmp	w1, 255
	beq	.L1979
	add	x2, x2, 2736
	mov	x1, 1
	b	.L1951
	.p2align 3
.L2025:
	add	w1, w1, 1
	cmp	w1, 32
	beq	.L2018
.L1951:
	ldr	w3, [x2]
	add	x2, x2, 72
	cmp	w3, 255
	bne	.L2025
.L1950:
	lsl	x2, x1, 6
	ldr	x3, [x23, 104]
	add	x1, x2, x1, lsl 3
	mov	x2, 72
	add	x1, x1, 323584
	add	x0, x0, x1
	mov	x1, x24
	add	x0, x0, 2664
	blr	x3
	ldr	x0, [x22, w19, sxtw 3]
	add	x0, x0, 327680
	ldr	w1, [x0, 2184]
	add	w1, w1, 1
	str	w1, [x0, 2184]
	ldr	w0, [x24]
	cmp	w0, 255
	beq	.L1972
	ldr	w2, [x24, 12]
	ldr	w0, [x24, 32]
	ldr	x1, [x24, 24]
	bl	MEM_AddMemRecord
.L1972:
	ldr	w0, [x24, 4]
	cmp	w0, 255
	beq	.L1953
	ldr	w2, [x24, 16]
	ldr	w0, [x24, 48]
	ldr	x1, [x24, 40]
	bl	MEM_AddMemRecord
.L1953:
	ldr	w0, [x24, 8]
	cmp	w0, 255
	beq	.L1954
	ldr	w2, [x24, 20]
	ldr	w0, [x24, 64]
	ldr	x1, [x24, 56]
	bl	MEM_AddMemRecord
.L1954:
	ldr	x0, [x22, w19, sxtw 3]
	add	x0, x0, 327680
	ldr	w1, [x0, 2188]
	cbz	w1, .L1970
	ldrsb	w1, [x26, 1]
	cbnz	w1, .L2026
.L1956:
	ldr	w0, [x25, 1952]
	strb	w0, [x26, 10]
	add	x0, x25, 872
	strb	wzr, [x26, 11]
	ldr	w1, [x0, 1248]
	strb	w1, [x26, 13]
	ldr	w1, [x24]
	cmp	w1, 255
	beq	.L2027
.L1974:
	ldrb	w0, [x0, 16]
	cbz	w0, .L2028
	add	x0, x25, 900
	mov	x1, 1
	b	.L1959
	.p2align 3
.L2029:
	add	w1, w1, 1
	cmp	w1, 32
	beq	.L2018
.L1959:
	ldrb	w2, [x0]
	add	x0, x0, 12
	cbnz	w2, .L2029
.L1958:
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
	add	x4, x4, 327680
	add	x2, x2, 327680
	sub	w21, w21, w5
	add	w21, w21, w3
	str	w3, [x4, 880]
	add	x3, x25, 872
	add	x0, x3, x0
	ldr	w7, [x24, 12]
	str	w7, [x4, 884]
	add	w7, w5, w21
	strb	w6, [x4, 888]
	ldr	w4, [x25, 2124]
	add	w4, w4, 1
	str	w4, [x25, 2124]
	str	w7, [x2, 1776]
	ldr	w4, [x25, 1908]
	add	w5, w5, w4
	add	w21, w5, w21
	str	w21, [x2, 1264]
	ldr	w2, [x24, 48]
	str	w2, [x0, 520]
	strb	w6, [x0, 528]
	ldr	w0, [x25, 2132]
	add	w0, w0, 1
	str	w0, [x25, 2132]
	ldr	w0, [x24, 8]
	cmp	w0, 255
	beq	.L1961
	ldrb	w0, [x25, 1960]
	cbz	w0, .L1981
.L2030:
	add	x2, x25, 1968
	mov	w0, 1
	.p2align 2
.L1963:
	ldrb	w3, [x2]
	add	x2, x2, 8
	cbz	w3, .L1962
	add	w0, w0, 1
	cmp	w0, 20
	bne	.L1963
.L1961:
	add	x4, x26, 20480
	ldr	w0, [x4, 1532]
	cbz	w0, .L1982
	add	x2, x4, 1632
	mov	x0, 1
	b	.L1966
	.p2align 3
.L1965:
	add	w0, w0, 1
	cmp	w0, 32
	beq	.L2018
.L1966:
	ldr	w3, [x2]
	add	x2, x2, 100
	cbnz	w3, .L1965
	mov	w5, w0
.L1964:
	lsl	x3, x1, 2
	lsl	x2, x0, 2
	sub	x1, x3, x1
	sub	x2, x2, x0
	add	x3, x20, x3
	add	x6, x26, 28672
	add	x20, x20, x1, lsl 2
	lsl	x1, x2, 5
	add	x2, x2, x1
	add	x20, x20, 327680
	add	x0, x2, x0
	ldr	w1, [x6, 396]
	add	x0, x26, x0
	add	x3, x3, 327680
	add	x0, x0, 20480
	add	w1, w1, 1
	str	w1, [x6, 396]
	add	x1, x25, 872
	add	x2, x26, 24576
	str	w5, [x0, 1444]
	add	x5, x26, 20
	ldr	w7, [x20, 880]
	str	w7, [x0, 1452]
	ldr	w7, [x20, 884]
	str	w7, [x0, 1456]
	mov	w7, 1
	ldr	w8, [x3, 1264]
	str	w8, [x0, 1464]
	ldr	w8, [x25, 872]
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
	ldr	w3, [x3, 1776]
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
	cbnz	w0, .L1978
	strb	w7, [x26, 12]
	str	wzr, [x2, 3748]
	str	wzr, [x2, 3756]
	ldr	w0, [x25, 872]
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
.L1978:
	ldrsb	w1, [x26, 13]
	ldr	w0, [x6, 400]
	cmp	w0, w1
	bcs	.L1968
	ldr	w1, [x24, 8]
	cmp	w1, 255
	beq	.L1968
	uxtw	x1, w0
	mov	w2, 1
	add	w0, w0, 1
	add	x7, x25, x1, lsl 3
	add	x1, x26, x1, lsl 3
	add	x1, x1, 28672
	ldr	w7, [x7, 1956]
	str	w7, [x1, 232]
	strb	w2, [x1, 236]
	strb	wzr, [x1, 237]
	str	w0, [x6, 400]
.L1968:
	ldr	w0, [x6, 404]
	add	x7, x25, 872
	ldr	w1, [x25, 1952]
	cmp	w0, w1
	bcs	.L1969
	ldr	w1, [x24, 4]
	cmp	w1, 255
	beq	.L1969
	ldr	w8, [x25, 2132]
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
.L1969:
	ldr	w1, [x25, 2116]
	str	w1, [x6, 228]
	ldr	w1, [x4, 1432]
	cbnz	w1, .L1970
	ldr	w1, [x4, 1452]
	str	w1, [x4, 1352]
	mov	w1, 1
	str	wzr, [x4, 1344]
	ldr	w2, [x25, 872]
	str	w2, [x4, 1368]
	str	w5, [x4, 1404]
	str	w3, [x4, 1408]
	ldr	w2, [x25, 1940]
	str	w2, [x4, 1412]
	ldr	w2, [x25, 1944]
	str	w2, [x4, 1416]
	ldr	w0, [x25, 1948]
	str	w0, [x4, 1420]
	strb	wzr, [x4, 1336]
	strb	wzr, [x4, 1337]
	strb	wzr, [x4, 1338]
	str	w1, [x4, 1432]
.L1970:
	ldr	x1, [x23, 32]
	mov	w0, 8
	blr	x1
	mov	w0, 0
.L2015:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L2018:
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
.L2026:
	ldr	w0, [x0, 2716]
	cbnz	w0, .L1956
	ldrsb	w1, [x26, 23]
	ldr	w2, [x25, 1952]
	add	w0, w1, 4
	strb	wzr, [x26, 11]
	cmp	w0, w2
	csel	w0, w0, w2, le
	strb	w0, [x26, 10]
	add	x0, x25, 872
	ldr	w1, [x0, 1248]
	strb	w1, [x26, 13]
	ldr	w1, [x24]
	cmp	w1, 255
	bne	.L1974
.L2027:
	ldr	w0, [x24, 8]
	mov	x1, 0
	cmp	w0, 255
	beq	.L1961
	ldrb	w0, [x25, 1960]
	cbnz	w0, .L2030
	b	.L1981
	.p2align 3
.L2024:
	ldr	w5, [x24, 8]
	cmp	w5, 255
	bne	.L1949
	ldr	x2, .LC828
	mov	w4, 0
	ldr	x1, .LC832
	mov	w3, 10235
	add	x2, x2, 152
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2015
.L1981:
	mov	w0, 0
	.p2align 2
.L1962:
	add	x0, x20, x0, uxtw 3
	ldr	w4, [x24, 64]
	add	x0, x0, 327680
	mov	w3, 1
	str	w4, [x0, 1956]
	strb	w3, [x0, 1960]
	ldr	w0, [x25, 2128]
	add	w0, w0, 1
	str	w0, [x25, 2128]
	b	.L1961
.L1979:
	mov	x1, 0
	b	.L1950
.L1982:
	mov	w5, 0
	mov	x0, 0
	b	.L1964
.L2028:
	mov	x1, 0
	b	.L1958
.L2023:
	ldr	x2, .LC828
	mov	w3, 10225
	ldr	x1, .LC830
	mov	w0, 1
	add	x2, x2, 152
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2015
	.size	VCTRL_SetSingleFrameBufToChan, .-VCTRL_SetSingleFrameBufToChan
	.align	3
.LC826:
	.xword	s_pFspInst
	.align	3
.LC827:
	.xword	.LANCHOR0
	.align	3
.LC828:
	.xword	.LANCHOR2
	.align	3
.LC830:
	.xword	.LC829
	.align	3
.LC832:
	.xword	.LC831
	.align	3
.LC833:
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
	ldr	x21, .LC834
	add	x2, x21, 88
	ldr	w0, [x2, w19, sxtw 2]
	cbz	w0, .L2040
	ldr	x2, .LC840
	mov	w3, w19
	ldr	x1, .LC842
	mov	w0, 0
	add	x2, x2, 184
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2040:
	mov	x1, x20
	mov	w3, 1
	mov	w0, w19
	str	w3, [x2, w19, sxtw 2]
	bl	VCTRL_SetSingleFrameBufToChan
	mov	w20, w0
	cbnz	w0, .L2033
	ldr	x0, .LC835
	ldr	x2, [x0, w19, sxtw 3]
	add	x2, x2, 323584
.L2034:
	ldr	x1, .LC839
	add	x21, x21, 88
	mov	w3, 2
	mov	w0, 31
	str	w3, [x2, 2616]
	bl	dprint_vfmw
	str	wzr, [x21, w19, sxtw 2]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2033:
	ldr	x22, .LC835
	ldr	x2, [x22, w19, sxtw 3]
	add	x2, x2, 323584
	ldr	w0, [x2, 2636]
	cbz	w0, .L2034
	ldr	x1, .LC837
	mov	w2, w19
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x2, [x22, w19, sxtw 3]
	mov	w0, 3
	add	x2, x2, 323584
	str	w0, [x2, 2616]
	b	.L2034
	.size	VCTRL_AllocMemToDecoder, .-VCTRL_AllocMemToDecoder
	.align	3
.LC834:
	.xword	.LANCHOR5
	.align	3
.LC835:
	.xword	.LANCHOR0
	.align	3
.LC837:
	.xword	.LC836
	.align	3
.LC839:
	.xword	.LC838
	.align	3
.LC840:
	.xword	.LANCHOR2
	.align	3
.LC842:
	.xword	.LC841
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
	cbz	x27, .L2041
	ldr	x1, .LC845
	mov	w0, 32
	bl	dprint_vfmw
	cmp	w22, wzr
	ccmp	w22, 3, 4, ne
	bne	.L2044
	ldr	x27, .LC847
	cbz	w23, .L2046
	ldr	x28, .LC846
	mov	w24, 0
	str	wzr, [x29, 156]
	.p2align 2
.L2048:
	cbz	w19, .L2049
	ldr	w1, [x29, 140]
	lsr	w0, w24, 4
	ubfiz	x22, x24, 8, 4
	ldr	w23, [x29, 156]
	mov	w26, 0
	mul	w0, w0, w1
	add	x22, x0, x22
	.p2align 2
.L2047:
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
	bhi	.L2047
.L2049:
	ldr	w0, [x29, 156]
	add	w24, w24, 1
	add	w0, w0, w19
	str	w0, [x29, 156]
	ldr	w0, [x29, 152]
	cmp	w0, w24
	bne	.L2048
.L2046:
	ldp	w0, w1, [x29, 128]
	cmp	w19, w0
	ldr	w0, [x29, 152]
	cset	w28, eq
	cmp	w0, w1
	cset	w0, eq
	ands	w0, w28, w0
	str	w0, [x29, 120]
	bne	.L2146
	ldr	w0, [x29, 176]
	ldr	w1, [x29, 96]
	ldr	x22, [x27, 960]
	mul	w0, w19, w0
	add	x0, x0, x1, uxtw
	add	x22, x22, x0
	ldr	w0, [x29, 132]
	cbz	w0, .L2054
	ldr	x28, .LC846
	uxtw	x21, w19
	mov	w23, 0
	.p2align 2
.L2055:
	ldr	w1, [x29, 128]
	mov	x0, x22
	ldr	x4, [x28, 88]
	add	w23, w23, 1
	ldr	x2, [x29, 144]
	add	x22, x22, x21
	blr	x4
	ldr	w0, [x29, 132]
	cmp	w0, w23
	bne	.L2055
.L2054:
	cbz	w25, .L2147
	ldr	w1, [x29, 140]
	mov	w24, 0
	ldr	x28, .LC846
	mov	w23, 0
	lsr	w0, w1, 1
	str	w0, [x29, 156]
	.p2align 2
.L2058:
	cbz	w19, .L2060
	ldr	w1, [x29, 156]
	lsr	w0, w23, 3
	ubfiz	x21, x23, 8, 3
	uxtw	x22, w24
	mov	w26, 0
	mul	w0, w0, w1
	add	x21, x0, x21
	.p2align 2
.L2057:
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
	bhi	.L2057
.L2060:
	add	w23, w23, 1
	add	w24, w24, w19
	cmp	w25, w23
	bne	.L2058
	ldr	w0, [x29, 124]
	cmp	w0, 1
	beq	.L2148
.L2088:
	ldr	w0, [x29, 124]
	cbz	w0, .L2149
.L2063:
	ldr	w0, [x29, 120]
	cbnz	w0, .L2150
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
	cbz	w22, .L2041
	ldr	w0, [x29, 128]
	uxtw	x24, w2
	ldr	x28, .LC846
	mov	w21, 0
	lsr	w23, w0, 1
	.p2align 2
.L2068:
	ldr	x3, [x28, 88]
	mov	x0, x20
	ldr	x2, [x29, 144]
	mov	w1, w23
	add	w21, w21, 1
	add	x20, x20, x24
	blr	x3
	cmp	w22, w21
	bne	.L2068
	mov	w20, 0
	.p2align 2
.L2069:
	ldr	x3, [x28, 88]
	mov	x0, x19
	ldr	x2, [x29, 144]
	mov	w1, w23
	add	w20, w20, 1
	add	x19, x19, x24
	blr	x3
	cmp	w22, w20
	bne	.L2069
.L2041:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 160
	ret
.L2044:
	cmp	w22, 1
	ldr	w1, [x29, 152]
	cset	w0, ne
	str	w0, [x29, 132]
	cmp	w1, w0
	ldr	x28, .LC846
	ldr	x27, .LC847
	bls	.L2071
	ldr	w24, [x29, 132]
	.p2align 2
.L2073:
	cbz	w19, .L2074
	ldr	w1, [x29, 140]
	lsr	w0, w24, 4
	lsr	w23, w24, 1
	ubfiz	x22, x24, 8, 4
	mov	w26, 0
	mul	w0, w0, w1
	mul	w23, w23, w19
	add	x22, x0, x22
	.p2align 2
.L2072:
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
	bhi	.L2072
.L2074:
	ldr	w0, [x29, 152]
	add	w24, w24, 2
	cmp	w0, w24
	bhi	.L2073
.L2071:
	ldr	w0, [x29, 152]
	ldr	x4, [x28, 88]
	ldr	x2, [x29, 144]
	mul	w1, w19, w0
	ldr	x0, [x27, 960]
	lsr	w1, w1, 1
	blr	x4
	cbz	w25, .L2151
	ldr	w1, [x29, 140]
	mov	w24, 0
	mov	w23, 0
	lsr	w0, w1, 1
	str	w0, [x29, 156]
	.p2align 2
.L2078:
	cbz	w19, .L2080
	ldr	w1, [x29, 156]
	lsr	w0, w23, 3
	ubfiz	x21, x23, 8, 3
	uxtw	x22, w24
	mov	w26, 0
	mul	w0, w0, w1
	add	x21, x21, x0
	.p2align 2
.L2077:
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
	bhi	.L2077
.L2080:
	add	w23, w23, 1
	add	w24, w24, w19
	cmp	w25, w23
	bne	.L2078
	ldr	w0, [x29, 124]
	cmp	w0, 1
	beq	.L2152
	ldr	w0, [x29, 124]
	cbnz	w0, .L2083
	ldr	w0, [x29, 132]
	cmp	w25, w0
	bls	.L2086
	.p2align 2
.L2087:
	ldr	w0, [x29, 132]
	add	w0, w0, 2
	str	w0, [x29, 132]
	cmp	w25, w0
	bhi	.L2087
.L2086:
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
.L2083:
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
.L2146:
	ldr	w0, [x29, 152]
	ldr	x28, .LC846
	ldr	x2, [x29, 144]
	mul	w1, w19, w0
	ldr	x4, [x28, 88]
	ldr	x0, [x27, 960]
	blr	x4
	b	.L2054
.L2152:
	ldr	w0, [x29, 132]
	mov	w9, 0
	ldr	x4, [x27, 960]
	mul	w0, w19, w0
	ubfiz	x19, x19, 1, 31
	add	x4, x4, x0
	.p2align 2
.L2082:
	ldr	w2, [x29, 136]
	cbz	w2, .L2084
	lsr	w0, w9, 1
	mov	w1, 1
	mul	w0, w0, w2
	add	w8, w2, w0
	.p2align 2
.L2081:
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
	bne	.L2081
.L2084:
	add	w9, w9, 2
	add	x4, x4, x19
	cmp	w25, w9
	bhi	.L2082
	b	.L2083
.L2148:
	ldr	x4, [x27, 960]
	mov	w0, 0
	mov	w8, 0
	.p2align 2
.L2062:
	ldr	w1, [x29, 136]
	mov	w7, w0
	cbz	w1, .L2064
	ldr	w1, [x29, 136]
	add	w7, w1, w0
	mov	w1, 1
	.p2align 2
.L2061:
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
	bne	.L2061
.L2064:
	add	w8, w8, 1
	add	x4, x4, x19
	cmp	w25, w8
	mov	w0, w7
	bne	.L2062
	b	.L2063
.L2149:
	ldr	x28, .LC846
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
	b	.L2063
.L2150:
	ldr	w0, [x29, 136]
	ldr	x19, .LC846
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
	b	.L2041
.L2151:
	ldr	w0, [x29, 124]
	cmp	w0, 1
	beq	.L2083
	ldr	w0, [x29, 124]
	cbnz	w0, .L2083
	b	.L2086
.L2147:
	ldr	w0, [x29, 124]
	cmp	w0, 1
	bne	.L2088
	b	.L2063
	.size	DBG_WriteBigTileYuv, .-DBG_WriteBigTileYuv
	.align	3
.LC845:
	.xword	.LC844
	.align	3
.LC846:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC847:
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
	ldr	x1, .LC849
	mov	w2, w20
	mov	w0, 2
	bl	dprint_vfmw
	cmp	w20, 15
	bhi	.L2202
	ldr	x21, .LC852
	ldr	x0, [x21, w20, sxtw 3]
	cbz	x0, .L2203
	ldr	w0, [x0, 12]
	cmp	w0, 1
	bne	.L2204
	ldr	x23, .LC855
	mov	w0, 4
	ldr	x1, [x23, 24]
	blr	x1
	ldr	x0, [x21, w20, sxtw 3]
	ldr	w4, [x0, 20]
	cmp	w4, 1
	bne	.L2205
	add	x0, x0, 327680
	mov	x1, x19
	add	x0, x0, 2752
	bl	SYNTAX_ReceiveImage
	cmp	w0, 1
	ccmp	x19, xzr, 4, eq
	mov	w24, w0
	beq	.L2160
	ldr	x22, .LC859
	add	x0, x22, 648
	ldr	x0, [x0, w20, sxtw 3]
	cbz	x0, .L2161
	ldr	w1, [x19, 108]
	ldr	w2, [x22, 968]
	ubfx	x1, x1, 2, 3
	ldr	w4, [x19, 116]
	cmp	w1, wzr
	ldr	w6, [x19, 140]
	cset	w7, eq
	cbz	w2, .L2162
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
.L2161:
	add	x22, x22, 1240
	ldr	x2, [x22, w20, sxtw 3]
	cbz	x2, .L2160
	ldr	x0, [x19, 232]
	cbz	x0, .L2160
	ldr	w1, [x19, 224]
	cbnz	w1, .L2206
	.p2align 2
.L2160:
	cmp	w24, 1
	mov	w22, -1
	beq	.L2207
.L2166:
	mov	w0, 12
	bl	CHECK_REC_POS_ENABLE
	cbnz	w0, .L2177
.L2175:
	cbz	w22, .L2178
.L2176:
	ldr	x1, [x21, w20, sxtw 3]
	mov	w0, 4
	ldr	x2, [x23, 32]
	add	x1, x1, 344064
	ldrb	w1, [x1, 2200]
	strb	w1, [x19, 472]
	blr	x2
.L2198:
	add	sp, x29, 0
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 624
	ret
	.p2align 3
.L2207:
	ldr	w0, [x19, 200]
	str	w22, [x19, 196]
	cmp	w0, 1
	beq	.L2208
	mov	w22, 0
.L2213:
	mov	w0, 12
	bl	CHECK_REC_POS_ENABLE
	cbz	w0, .L2175
	b	.L2177
.L2172:
	ldr	x0, [x21, w20, sxtw 3]
	mov	x1, x19
	add	x0, x0, 327680
	add	x0, x0, 2752
	bl	SYNTAX_ReleaseImage
	ldr	w0, [x29, 252]
	cbz	w0, .L2209
	ldr	x2, .LC856
	mov	w0, w24
	ldr	x1, .LC865
	ldr	w3, [x19, 192]
	add	x2, x2, 208
	bl	dprint_vfmw
	ldr	x3, [x23, 104]
	mov	x2, 624
	add	x1, x29, 64
	mov	x0, x19
	blr	x3
	ldp	w1, w2, [x29, 92]
.L2200:
	mov	w0, 3
	str	w24, [x19, 188]
	str	w0, [x19, 356]
	mov	w0, 12
	str	w22, [x19, 196]
	stp	w1, w2, [x19, 204]
	bl	CHECK_REC_POS_ENABLE
	cbnz	w0, .L2210
	.p2align 2
.L2178:
	ldr	x0, [x21, w20, sxtw 3]
	mov	w22, 0
	ldr	w1, [x19, 108]
	str	w1, [x0, 432]
	ldr	w1, [x19, 356]
	str	w1, [x0, 436]
	b	.L2176
	.p2align 3
.L2210:
	mov	w22, 0
	.p2align 2
.L2177:
	ldr	x0, [x23]
	blr	x0
	mov	w3, w0
	ldr	x1, .LC859
	ldr	x0, .LC866
	add	x1, x1, 1776
	ldr	w2, [x0]
	ldr	w0, [x1, w20, sxtw 2]
	sub	w0, w3, w0
	cmp	w0, w2
	bls	.L2175
	lsl	w2, w20, 24
	ldr	x0, .LC856
	add	w2, w2, 786432
	cmp	w24, 1
	str	w3, [x1, w20, sxtw 2]
	csinc	w2, w2, w2, ne
	add	x0, x0, 232
	mov	w1, 4803
	bl	DBG_AddTrace
	cbnz	w22, .L2176
	b	.L2178
	.p2align 3
.L2162:
	str	wzr, [sp]
	mov	w5, 0
	str	wzr, [sp, 8]
	str	wzr, [sp, 16]
	str	wzr, [sp, 24]
	ldr	w3, [x19, 112]
	ldp	x1, x2, [x19, 504]
	bl	DBG_WriteBigTileYuv
	b	.L2161
	.p2align 3
.L2208:
	ldr	w0, [x19, 188]
	cbz	w0, .L2211
	ldp	w1, w2, [x19, 28]
	b	.L2200
	.p2align 3
.L2204:
	ldr	x3, .LC854
	mov	w2, 4573
	ldr	x1, .LC851
	mov	w0, 0
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2198
	.p2align 3
.L2205:
	ldr	x2, .LC856
	mov	w3, 4607
	ldr	x1, .LC858
	mov	w0, 0
	add	x2, x2, 208
	mov	w22, -1
	bl	dprint_vfmw
	ldr	x1, [x23, 32]
	mov	w0, 4
	blr	x1
	b	.L2198
	.p2align 3
.L2206:
	ldr	x3, [x23, 88]
	blr	x3
	b	.L2160
	.p2align 3
.L2211:
	ldr	x0, [x21, w20, sxtw 3]
	add	x1, x29, 64
	add	x0, x0, 327680
	add	x0, x0, 2752
	bl	SYNTAX_ReceiveImage
	cmp	w0, 1
	bne	.L2212
	ldr	w0, [x19, 192]
	ldr	w1, [x29, 256]
	cmp	w0, w1
	bge	.L2169
	ldp	w0, w4, [x29, 92]
	ldp	w3, w2, [x29, 100]
	ldr	w1, [x29, 220]
	ldr	w22, [x29, 252]
	stp	w0, w4, [x19, 204]
	str	w1, [x19, 196]
	mov	w0, 3
	stp	w3, w2, [x19, 212]
	str	w0, [x19, 356]
	cbz	w22, .L2166
	str	w24, [x19, 188]
	mov	w22, 0
	b	.L2213
	.p2align 3
.L2202:
	ldr	x3, .LC850
	mov	w2, 4569
	ldr	x1, .LC851
	mov	w0, 0
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2198
	.p2align 3
.L2212:
	ldr	x1, [x21, w20, sxtw 3]
	mov	w22, w0
	add	x0, x1, 327680
	add	x0, x0, 2752
	bl	SYNTAX_ReturnImage
	ldr	x1, [x23, 32]
	mov	w0, 4
	blr	x1
	b	.L2198
.L2169:
	bgt	.L2214
	cbnz	w0, .L2172
	ldr	x0, [x21, w20, sxtw 3]
	add	x0, x0, 327680
	add	x0, x0, 2752
	bl	SYNTAX_ReturnImage
	ldr	x0, [x21, w20, sxtw 3]
	mov	x1, x19
	add	x0, x0, 327680
	add	x0, x0, 2752
	bl	SYNTAX_ReleaseImage
	ldr	x2, .LC856
	mov	w0, w24
	ldr	x1, .LC863
	mov	w24, -2
	ldr	w3, [x19, 192]
	add	x2, x2, 208
	bl	dprint_vfmw
	b	.L2166
.L2203:
	ldr	x3, .LC853
	mov	w2, 4570
	ldr	x1, .LC851
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2198
.L2214:
	ldr	x0, [x21, w20, sxtw 3]
	add	x0, x0, 327680
	add	x0, x0, 2752
	bl	SYNTAX_ReturnImage
	ldr	x0, [x21, w20, sxtw 3]
	mov	x1, x19
	add	x0, x0, 327680
	add	x0, x0, 2752
	bl	SYNTAX_ReleaseImage
	ldr	x2, .LC856
	mov	w0, w24
	ldr	x1, .LC861
	mov	w24, -2
	ldr	w3, [x19, 192]
	add	x2, x2, 208
	ldr	w4, [x29, 256]
	bl	dprint_vfmw
	b	.L2166
.L2209:
	ldr	x0, [x21, w20, sxtw 3]
	add	x1, x29, 64
	add	x0, x0, 327680
	add	x0, x0, 2752
	bl	SYNTAX_ReleaseImage
	ldr	x2, .LC856
	mov	w0, w24
	ldr	x1, .LC865
	mov	w24, -2
	ldr	w3, [x19, 192]
	add	x2, x2, 208
	bl	dprint_vfmw
	b	.L2166
	.size	VCTRL_GetChanImage, .-VCTRL_GetChanImage
	.align	3
.LC849:
	.xword	.LC848
	.align	3
.LC850:
	.xword	.LC51
	.align	3
.LC851:
	.xword	.LC2
	.align	3
.LC852:
	.xword	.LANCHOR0
	.align	3
.LC853:
	.xword	.LC5
	.align	3
.LC854:
	.xword	.LC186
	.align	3
.LC855:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC856:
	.xword	.LANCHOR2
	.align	3
.LC858:
	.xword	.LC857
	.align	3
.LC859:
	.xword	.LANCHOR5
	.align	3
.LC861:
	.xword	.LC860
	.align	3
.LC863:
	.xword	.LC862
	.align	3
.LC865:
	.xword	.LC864
	.align	3
.LC866:
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
	ldr	x0, .LC870
	ldr	x0, [x0, w19, sxtw 3]
	add	x0, x0, 327680
	ldr	w1, [x0, 2752]
	cmp	w1, 2
	beq	.L2217
	cmp	w1, 16
	beq	.L2218
	cbz	w1, .L2223
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2223:
	add	x0, x0, 2760
	bl	H264DEC_GetImageWidth
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2218:
	add	x0, x0, 2760
	bl	HEVCDEC_GetImageWidth
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2217:
	add	x0, x0, 2760
	bl	MPEG4DEC_GetImageWidth
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_GetImageWidth, .-VCTRL_GetImageWidth
	.align	3
.LC870:
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
	ldr	x0, .LC871
	ldr	x0, [x0, w19, sxtw 3]
	add	x0, x0, 327680
	ldr	w1, [x0, 2752]
	cmp	w1, 2
	beq	.L2226
	cmp	w1, 16
	beq	.L2227
	cbz	w1, .L2232
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2232:
	add	x0, x0, 2760
	bl	H264DEC_GetImageHeight
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2227:
	add	x0, x0, 2760
	bl	HEVCDEC_GetImageHeight
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2226:
	add	x0, x0, 2760
	bl	MPEG4DEC_GetImageHeight
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_GetImageHeight, .-VCTRL_GetImageHeight
	.align	3
.LC871:
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
	ldr	x3, .LC872
	mov	x2, 24
	mov	w1, 0
	add	x0, x29, 56
	ldr	x3, [x3, 96]
	blr	x3
	ldr	x0, .LC873
	ldr	x6, [x0, 104]
	cbz	x6, .L2234
	ldr	x4, .LC874
	add	x5, x29, 56
	mov	w3, 0
	mov	w2, w21
	mov	w1, w20
	mov	w0, w19
	ldr	x4, [x4, w19, sxtw 3]
	add	x4, x4, 327680
	ldr	w4, [x4, 2752]
	blr	x6
.L2234:
	ldr	w0, [x29, 60]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.size	VCTRL_GetFsSize, .-VCTRL_GetFsSize
	.align	3
.LC872:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC873:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC874:
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
	ldr	x0, .LC875
	mov	w1, 1
	add	x0, x0, 1840
	str	w1, [x0, w19, sxtw 2]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_EnableEsBuf, .-VCTRL_EnableEsBuf
	.align	3
.LC875:
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
	ldr	x0, .LC877
	add	x0, x0, 1840
	str	wzr, [x0, w19, sxtw 2]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_DisableEsBuf, .-VCTRL_DisableEsBuf
	.align	3
.LC877:
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
	ldr	x0, .LC879
	add	x0, x0, 1840
	ldr	w0, [x0, w19, sxtw 2]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VCTRL_IfEsBufEnable, .-VCTRL_IfEsBufEnable
	.align	3
.LC879:
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
	.type	__func__.49993, %object
	.size	__func__.49993, 18
__func__.49993:
	.string	"VCTRL_EventReport"
	.zero	6
	.type	__func__.50005, %object
	.size	__func__.50005, 18
__func__.50005:
	.string	"VCTRL_SetCallBack"
	.zero	6
	.type	__FUNCTION__.50111, %object
	.size	__FUNCTION__.50111, 29
__FUNCTION__.50111:
	.string	"VCTRL_PartitionMemForStdUser"
	.zero	3
	.type	__func__.50135, %object
	.size	__func__.50135, 23
__func__.50135:
	.string	"VCTRL_FreeChanResource"
	.zero	1
	.type	__FUNCTION__.50394, %object
	.size	__FUNCTION__.50394, 23
__FUNCTION__.50394:
	.string	"VCTRL_ReleaseChanImage"
	.zero	1
	.type	__func__.50395, %object
	.size	__func__.50395, 23
__func__.50395:
	.string	"VCTRL_ReleaseChanImage"
	.zero	1
	.type	__func__.50421, %object
	.size	__func__.50421, 18
__func__.50421:
	.string	"VCTRL_GetDecParam"
	.align	3
.LANCHOR2 = . + 0
	.type	__FUNCTION__.50487, %object
	.size	__FUNCTION__.50487, 19
__FUNCTION__.50487:
	.string	"VCTRL_FlushDecoder"
	.zero	5
	.type	__FUNCTION__.50818, %object
	.size	__FUNCTION__.50818, 18
__FUNCTION__.50818:
	.string	"GetExtraRefFrmNum"
	.zero	6
	.type	__FUNCTION__.50824, %object
	.size	__FUNCTION__.50824, 19
__FUNCTION__.50824:
	.string	"GetExtraDispFrmNum"
	.zero	5
	.type	__FUNCTION__.50830, %object
	.size	__FUNCTION__.50830, 16
__FUNCTION__.50830:
	.string	"GetExtraCtrlCmd"
	.type	__FUNCTION__.50750, %object
	.size	__FUNCTION__.50750, 14
__FUNCTION__.50750:
	.string	"DecChanSyntax"
	.zero	2
	.type	__func__.50312, %object
	.size	__func__.50312, 24
__func__.50312:
	.string	"VCTRL_FindOccupiedFrame"
	.type	__func__.50808, %object
	.size	__func__.50808, 24
__func__.50808:
	.string	"VCTRL_ExtAccGetDecParam"
	.type	__func__.51065, %object
	.size	__func__.51065, 30
__func__.51065:
	.string	"VCTRL_SetSingleFrameBufToChan"
	.zero	2
	.type	__func__.51084, %object
	.size	__func__.51084, 24
__func__.51084:
	.string	"VCTRL_AllocMemToDecoder"
	.type	__func__.50376, %object
	.size	__func__.50376, 19
__func__.50376:
	.string	"VCTRL_GetChanImage"
	.zero	5
	.type	__FUNCTION__.50384, %object
	.size	__FUNCTION__.50384, 19
__FUNCTION__.50384:
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
.LC829:
	ASCII(.string	"%s,%d,actul size = 0x%x,except size= 0x%x\n" )
	.zero	5
.LC831:
	ASCII(.string	"%s,%d,configuredFrameNum = %x,PmvNodeStatus= 0x%x\n" )
	.zero	5
.LC836:
	ASCII(.string	"DFS, request frame store fail!, chanid: %d\n" )
	.zero	4
.LC838:
	ASCII(.string	"DFS, set decoding thread state to success!\n " )
	.zero	3
.LC841:
	ASCII(.string	"%s set chan %d ctrl state CHAN_FRAME_STORE_ALLOC failed!\n" )
	.zero	6
.LC844:
	ASCII(.string	"VFMW: WriteBigTileYuv %dx%d...\n" )
.LC848:
	ASCII(.string	"call VCTRL_GetChanImage, ChanID = %d,\n" )
	.zero	1
.LC857:
	ASCII(.string	"%s,%d,ImgOutputEn = %d, should not be output!\n" )
	.zero	1
.LC860:
	ASCII(.string	"%s: view_0 id(%d) > view_1 id(%d), error!\n" )
	.zero	5
.LC862:
	ASCII(.string	"%s: continuous view0 id(%d)!\n" )
	.zero	2
.LC864:
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
	.type	FspInstInfo.50107, %object
	.size	FspInstInfo.50107, 3212
FspInstInfo.50107:
	.zero	3212
	.type	g_VdmCharacter, %object
	.size	g_VdmCharacter, 4
g_VdmCharacter:
	.zero	4
	.type	last_rec_pos_time.50391, %object
	.size	last_rec_pos_time.50391, 64
last_rec_pos_time.50391:
	.zero	64
	.type	extra_ref, %object
	.size	extra_ref, 4
extra_ref:
	.zero	4
	.type	extra_disp, %object
	.size	extra_disp, 4
extra_disp:
	.zero	4
	.type	MemArrange.50605, %object
	.size	MemArrange.50605, 1272
MemArrange.50605:
	.zero	1272
	.type	MemArrange.50639, %object
	.size	MemArrange.50639, 1272
MemArrange.50639:
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
	.type	last_rec_pos_time.50744, %object
	.size	last_rec_pos_time.50744, 64
last_rec_pos_time.50744:
	.zero	64
	.type	last_rec_pos_decparam_time.50745, %object
	.size	last_rec_pos_decparam_time.50745, 64
last_rec_pos_decparam_time.50745:
	.zero	64
	.type	ChanPriTab.50705, %object
	.size	ChanPriTab.50705, 64
ChanPriTab.50705:
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
	.type	PrioArray.50207, %object
	.size	PrioArray.50207, 64
PrioArray.50207:
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
	.type	Capability.50251, %object
	.size	Capability.50251, 148
Capability.50251:
	.zero	148
	.zero	4
	.type	PrioArray.50250, %object
	.size	PrioArray.50250, 64
PrioArray.50250:
	.zero	64
	.type	g_CurProcChan, %object
	.size	g_CurProcChan, 4
g_CurProcChan:
	.zero	4
	.type	save_file_cnt.50869, %object
	.size	save_file_cnt.50869, 4
save_file_cnt.50869:
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
	.type	save_file_cnt.50872, %object
	.size	save_file_cnt.50872, 4
save_file_cnt.50872:
	.zero	4
	.type	save_seg_chan_num, %object
	.size	save_seg_chan_num, 4
save_seg_chan_num:
	.zero	4
	.type	vfmw_save_yuv_file, %object
	.size	vfmw_save_yuv_file, 256
vfmw_save_yuv_file:
	.zero	256
	.type	save_file_cnt.50877, %object
	.size	save_file_cnt.50877, 4
save_file_cnt.50877:
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
	.type	save_file_cnt.50890, %object
	.size	save_file_cnt.50890, 4
save_file_cnt.50890:
	.zero	4
	.type	save_metadata_chan_num, %object
	.size	save_metadata_chan_num, 4
save_metadata_chan_num:
	.zero	4
	.type	vfmw_save_metadata_reorder_file, %object
	.size	vfmw_save_metadata_reorder_file, 256
vfmw_save_metadata_reorder_file:
	.zero	256
	.type	save_file_cnt.50893, %object
	.size	save_file_cnt.50893, 4
save_file_cnt.50893:
	.zero	4
	.type	save_metadata_reorder_chan_num, %object
	.size	save_metadata_reorder_chan_num, 4
save_metadata_reorder_chan_num:
	.zero	4
	.type	vfmw_save_el_file, %object
	.size	vfmw_save_el_file, 256
vfmw_save_el_file:
	.zero	256
	.type	save_file_cnt.50896, %object
	.size	save_file_cnt.50896, 4
save_file_cnt.50896:
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
	.type	last_rec_pos_time.50381, %object
	.size	last_rec_pos_time.50381, 64
last_rec_pos_time.50381:
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
