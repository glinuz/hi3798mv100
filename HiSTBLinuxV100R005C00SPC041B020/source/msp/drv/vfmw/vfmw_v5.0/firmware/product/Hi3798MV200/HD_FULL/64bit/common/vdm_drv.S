#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vdm_drv.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	VDMDRV_Init
	.type	VDMDRV_Init, %function
VDMDRV_Init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	sxtw	x19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC1
	ldr	x0, .LC0
	mov	x2, 64
	mov	w1, 0
	ldr	x3, [x0, 96]
	add	x0, x20, x19, lsl 6
	blr	x3
	lsl	x0, x19, 2
	ldr	x1, .LC2
	sub	x19, x0, x19
	add	x0, x20, x0
	add	x19, x1, x19, lsl 5
	str	wzr, [x0, 64]
	str	wzr, [x0, 72]
	str	wzr, [x19, 16]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_Init, .-VDMDRV_Init
	.align	3
.LC0:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC1:
	.xword	.LANCHOR0
	.align	3
.LC2:
	.xword	g_VfmwGlobalStat
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_Reset
	.type	VDMDRV_Reset, %function
VDMDRV_Reset:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	sbfiz	x19, x19, 6, 32
	bl	_mcount
	ldr	x3, .LC3
	mov	x2, 64
	ldr	x0, .LC4
	mov	w1, 0
	ldr	x3, [x3, 96]
	add	x0, x0, x19
	blr	x3
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_Reset, .-VDMDRV_Reset
	.align	3
.LC3:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC4:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_SetPriority
	.type	VDMDRV_SetPriority, %function
VDMDRV_SetPriority:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x6, .LC7
	mov	w4, 0
	mov	w3, 0
	add	x5, x6, 144
	.p2align 2
.L10:
	mov	x0, 0
	mov	w2, 0
	.p2align 2
.L5:
	ldr	w1, [x19, x0, lsl 2]
	cmp	w1, w2
	ble	.L4
	mov	w3, w0
	mov	w2, w1
.L4:
	add	x0, x0, 1
	cmp	x0, 16
	bne	.L5
	cmp	w2, wzr
	ble	.L16
	str	w3, [x5], 4
	add	w4, w4, 1
	str	wzr, [x19, w3, sxtw 2]
	cmp	w4, 16
	bne	.L10
.L3:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L16:
	cmp	w4, 15
	add	x1, x6, 80
	mov	w2, -1
	bgt	.L3
	.p2align 2
.L11:
	add	x0, x1, x4, sxtw 2
	add	w4, w4, 1
	cmp	w4, 16
	str	w2, [x0, 64]
	bne	.L11
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_SetPriority, .-VDMDRV_SetPriority
	.align	3
.LC7:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_AdjustPriorityAfterDec
	.type	VDMDRV_AdjustPriorityAfterDec, %function
VDMDRV_AdjustPriorityAfterDec:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x8, .LC8
	ldr	w0, [x8, 144]
	cmp	w19, w0
	beq	.L27
	add	x2, x8, 148
	mov	x0, 1
	.p2align 2
.L23:
	ldr	w3, [x2]
	mov	w1, w0
	add	x2, x2, 4
	cmp	w3, w19
	beq	.L30
	add	x0, x0, 1
	cmp	x0, 16
	bne	.L23
.L27:
	mov	x3, 4
	mov	x0, 0
	mov	w1, 0
.L18:
	add	x2, x8, 144
	add	x7, x2, x3
	ldr	w3, [x2, x3]
	tbz	w3, #31, .L24
	b	.L17
	.p2align 3
.L26:
	add	x7, x2, x3, lsl 2
	ldr	w3, [x2, x3, lsl 2]
	tbnz	w3, #31, .L17
.L24:
	add	x4, x8, 80
	ldr	w5, [x2, x0, lsl 2]
	add	w1, w1, 1
	ldr	w6, [x4, w3, sxtw 2]
	ldr	w4, [x4, w5, sxtw 2]
	cmp	w6, w4
	blt	.L25
	str	w3, [x2, x0, lsl 2]
	str	w5, [x7]
.L25:
	cmp	w1, 14
	sxtw	x0, w1
	add	x3, x0, 1
	ble	.L26
.L17:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L30:
	cmp	w0, 14
	bgt	.L17
	add	x3, x0, 1
	lsl	x3, x3, 2
	b	.L18
	.size	VDMDRV_AdjustPriorityAfterDec, .-VDMDRV_AdjustPriorityAfterDec
	.align	3
.LC8:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_PrepareSleep
	.type	VDMDRV_PrepareSleep, %function
VDMDRV_PrepareSleep:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC11
	sxtw	x0, w19
	add	x2, x1, x0, lsl 2
	ldr	w3, [x2, 64]
	cbnz	w3, .L34
	lsl	x0, x0, 6
	ldr	w0, [x1, x0]
	cbz	w0, .L35
	mov	w1, 1
	mov	w0, 0
	str	w1, [x2, 64]
.L32:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L35:
	mov	w1, 2
	str	w1, [x2, 64]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L34:
	mov	w0, -1
	b	.L32
	.size	VDMDRV_PrepareSleep, .-VDMDRV_PrepareSleep
	.align	3
.LC11:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_GetSleepStage
	.type	VDMDRV_GetSleepStage, %function
VDMDRV_GetSleepStage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC12
	add	x19, x0, x19, sxtw 2
	ldr	w0, [x19, 64]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_GetSleepStage, .-VDMDRV_GetSleepStage
	.align	3
.LC12:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_ForceSleep
	.type	VDMDRV_ForceSleep, %function
VDMDRV_ForceSleep:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x3, .LC13
	sxtw	x0, w19
	add	x19, x3, x0, lsl 2
	ldr	w1, [x19, 64]
	cmp	w1, 2
	beq	.L38
	ldr	x4, .LC14
	add	x0, x3, x0, lsl 6
	mov	x2, 64
	mov	w1, 0
	ldr	x3, [x4, 96]
	blr	x3
	mov	w0, 2
	str	w0, [x19, 64]
.L38:
	ldr	x1, .LC16
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_ForceSleep, .-VDMDRV_ForceSleep
	.align	3
.LC13:
	.xword	.LANCHOR0
	.align	3
.LC14:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC16:
	.xword	.LC15
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_ExitSleep
	.type	VDMDRV_ExitSleep, %function
VDMDRV_ExitSleep:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC17
	add	x19, x0, x19, sxtw 2
	str	wzr, [x19, 64]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_ExitSleep, .-VDMDRV_ExitSleep
	.align	3
.LC17:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_StartRepair
	.type	VDMDRV_StartRepair, %function
VDMDRV_StartRepair:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	mov	x21, x2
	bl	_mcount
	ldr	x0, .LC18
	ldr	x3, [x0, 240]
	cbz	x3, .L44
	mov	x1, x21
	mov	w0, w19
	sbfiz	x19, x19, 6, 32
	blr	x3
	ldr	x0, .LC21
	str	w20, [x0, x19]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L44:
	ldr	x1, .LC20
	mov	w2, 295
	mov	w0, 0
	sbfiz	x19, x19, 6, 32
	bl	dprint_vfmw
	ldr	x0, .LC21
	str	w20, [x0, x19]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	VDMDRV_StartRepair, .-VDMDRV_StartRepair
	.align	3
.LC18:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC20:
	.xword	.LC19
	.align	3
.LC21:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_StartDec
	.type	VDMDRV_StartDec, %function
VDMDRV_StartDec:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x0, .LC22
	ldr	x2, [x0, 248]
	cbz	x2, .L47
	mov	x1, x20
	mov	w0, w19
	sbfiz	x19, x19, 6, 32
	blr	x2
	ldr	x0, .LC24
	mov	w1, 1
	str	w1, [x0, x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L47:
	ldr	x1, .LC23
	mov	w2, 303
	mov	w0, 0
	sbfiz	x19, x19, 6, 32
	bl	dprint_vfmw
	ldr	x0, .LC24
	mov	w1, 1
	str	w1, [x0, x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_StartDec, .-VDMDRV_StartDec
	.align	3
.LC22:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC23:
	.xword	.LC19
	.align	3
.LC24:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_IsChanDec
	.type	VDMDRV_IsChanDec, %function
VDMDRV_IsChanDec:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC25
	add	x19, x0, x19, sxtw 2
	ldr	w0, [x19, 208]
	ldr	x19, [sp, 16]
	cmp	w0, wzr
	cset	w0, ne
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_IsChanDec, .-VDMDRV_IsChanDec
	.align	3
.LC25:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_SetChanIsDec
	.type	VDMDRV_SetChanIsDec, %function
VDMDRV_SetChanIsDec:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	add	w20, w20, 1
	ldr	x1, .LC26
	mov	w0, w19
	add	x19, x1, x19, sxtw 2
	str	w20, [x19, 208]
	bl	VDMDRV_AdjustPriorityAfterDec
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_SetChanIsDec, .-VDMDRV_SetChanIsDec
	.align	3
.LC26:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_ClearChanIsDec
	.type	VDMDRV_ClearChanIsDec, %function
VDMDRV_ClearChanIsDec:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC27
	add	x19, x0, x19, sxtw 2
	str	wzr, [x19, 208]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_ClearChanIsDec, .-VDMDRV_ClearChanIsDec
	.align	3
.LC27:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_ParamInit
	.type	VDMDRV_ParamInit, %function
VDMDRV_ParamInit:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x2, .LC30
	mov	x0, 0
	mov	w3, 1
	.p2align 2
.L54:
	ldr	x1, [x2, x0]
	add	x0, x0, 8
	cmp	x0, 128
	cbz	x1, .L53
	str	w3, [x1, 24]
.L53:
	bne	.L54
	ldp	x29, x30, [sp], 16
	ret
	.size	VDMDRV_ParamInit, .-VDMDRV_ParamInit
	.align	3
.LC30:
	.xword	s_pstVfmwChan
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_AddMatchMode
	.type	VDMDRV_AddMatchMode, %function
VDMDRV_AddMatchMode:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	VDMDRV_AddMatchMode, .-VDMDRV_AddMatchMode
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_CheckAttachMode
	.type	VDMDRV_CheckAttachMode, %function
VDMDRV_CheckAttachMode:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bgt	.L60
	ldr	x22, .LC31
	ldr	x0, [x22, w19, sxtw 3]
	cbz	x0, .L60
	add	x1, x0, 294912
	ldr	x20, .LC32
	ldr	w2, [x1, 2268]
	ldr	w1, [x1, 2272]
	cmp	w2, 3839
	cset	w21, hi
	cmp	w1, 2159
	cset	w1, hi
	ands	w21, w21, w1
	bne	.L85
	add	x1, x20, 336
	ldr	w1, [x1, w19, sxtw 2]
.L68:
	cmp	w1, 10
	beq	.L70
.L69:
	ldr	w1, [x0, 24]
	cbz	w1, .L71
.L70:
	ldr	w1, [x0, 1624]
	cmp	w1, 35
	ble	.L72
	cbz	w21, .L71
	ldr	x2, .LC33
	mov	w5, 1
	ldr	x1, .LC39
	mov	w4, w19
	str	w5, [x0, 1612]
	mov	w3, 558
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x0, [x22, w19, sxtw 3]
	.p2align 2
.L71:
	str	wzr, [x0, 24]
	add	x20, x20, 272
	ldr	x0, [x22, w19, sxtw 3]
	ldr	w0, [x0, 1624]
	str	w0, [x20, w19, sxtw 2]
.L60:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L85:
	ldr	w4, [x0, 1624]
	add	x0, x20, 272
	ldr	x24, .LC33
	mov	w3, 530
	ldr	x1, .LC35
	ldr	w5, [x0, w19, sxtw 2]
	mov	x2, x24
	cmp	w4, w5
	sub	w0, w5, w4
	sub	w23, w4, w5
	csel	w23, w0, w23, ls
	mov	w0, 22
	bl	dprint_vfmw
	cmp	w23, 2
	bgt	.L65
	add	x0, x20, 336
	ldr	w1, [x0, w19, sxtw 2]
	add	w1, w1, 1
	str	w1, [x0, w19, sxtw 2]
.L66:
	cmn	w1, #1
	beq	.L67
	ldr	x0, [x22, w19, sxtw 3]
	b	.L68
	.p2align 3
.L72:
	str	wzr, [x0, 1612]
	add	x20, x20, 272
	ldr	x0, [x22, w19, sxtw 3]
	str	wzr, [x0, 24]
	ldr	x0, [x22, w19, sxtw 3]
	ldr	w0, [x0, 1624]
	str	w0, [x20, w19, sxtw 2]
	b	.L60
	.p2align 3
.L65:
	add	x23, x20, 336
	ldr	x1, .LC37
	mov	w3, 539
	mov	x2, x24
	mov	w0, 22
	str	wzr, [x23, w19, sxtw 2]
	bl	dprint_vfmw
	ldr	w1, [x23, w19, sxtw 2]
	b	.L66
	.p2align 3
.L67:
	add	x1, x20, 336
	mov	w2, 11
	ldr	x0, [x22, w19, sxtw 3]
	str	w2, [x1, w19, sxtw 2]
	b	.L69
	.size	VDMDRV_CheckAttachMode, .-VDMDRV_CheckAttachMode
	.align	3
.LC31:
	.xword	s_pstVfmwChan
	.align	3
.LC32:
	.xword	.LANCHOR0
	.align	3
.LC33:
	.xword	.LANCHOR1
	.align	3
.LC35:
	.xword	.LC34
	.align	3
.LC37:
	.xword	.LC36
	.align	3
.LC39:
	.xword	.LC38
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_ChnDecReady
	.type	VDMDRV_ChnDecReady, %function
VDMDRV_ChnDecReady:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	mov	w0, w19
	bl	VCTRL_IsChanActive
	cbnz	w0, .L90
	ldr	x0, .LC45
	add	x0, x0, x19, sxtw 2
	ldr	w20, [x0, 208]
	cbnz	w20, .L91
	mov	w0, w19
	str	xzr, [x21]
	bl	VCTRL_GetDecParam
	str	x0, [x21]
	cbz	x0, .L92
.L88:
	mov	w0, w20
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L90:
	ldr	x2, .LC42
	mov	w3, w19
	ldr	x1, .LC44
	mov	w0, 22
	add	x2, x2, 24
	mov	w20, 1
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L91:
	ldr	x1, .LC47
	mov	w2, w19
	mov	w0, 22
	mov	w20, 1
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L92:
	ldr	x1, .LC49
	mov	w2, w19
	mov	w0, 22
	mov	w20, 1
	bl	dprint_vfmw
	b	.L88
	.size	VDMDRV_ChnDecReady, .-VDMDRV_ChnDecReady
	.align	3
.LC42:
	.xword	.LANCHOR1
	.align	3
.LC44:
	.xword	.LC43
	.align	3
.LC45:
	.xword	.LANCHOR0
	.align	3
.LC47:
	.xword	.LC46
	.align	3
.LC49:
	.xword	.LC48
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_MMUIntServeProc
	.type	VDMDRV_MMUIntServeProc, %function
VDMDRV_MMUIntServeProc:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x21, .LC50
	mov	w0, 0
	ldr	x1, [x21, 24]
	blr	x1
	ldr	x19, .LC51
	ldr	x2, [x19, 216]
	cbz	x2, .L97
	mov	w1, w20
	mov	w0, 5
	blr	x2
	mov	w22, w0
	ldr	x2, [x19, 216]
	cbz	x2, .L112
	mov	w1, w20
	mov	w0, 6
	and	w23, w22, 15
	mov	w24, 0
	blr	x2
	mov	w25, w0
	ldr	x2, [x19, 216]
	cbz	x2, .L94
	mov	w1, w20
	mov	w0, 7
	blr	x2
	mov	w24, w0
.L94:
	cbz	w23, .L97
	tbnz	x22, 2, .L113
	tbnz	x22, 3, .L114
	.p2align 2
.L97:
	ldr	x1, [x19, 192]
	cbz	x1, .L100
	mov	w0, w20
	blr	x1
.L101:
	ldr	x1, [x21, 32]
	mov	w0, 0
	blr	x1
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L100:
	ldr	x1, .LC56
	mov	w2, 1725
	mov	w0, 0
	bl	dprint_vfmw
	b	.L101
	.p2align 3
.L114:
	ldr	x1, .LC55
	mov	w3, w24
	mov	w2, w22
	mov	w0, 32
	bl	dprint_vfmw
	b	.L97
	.p2align 3
.L113:
	ldr	x1, .LC53
	mov	w3, w25
	mov	w2, w22
	mov	w0, 32
	bl	dprint_vfmw
	tbz	x22, 3, .L97
	b	.L114
.L112:
	and	w23, w0, 15
	mov	w25, 0
	mov	w24, 0
	b	.L94
	.size	VDMDRV_MMUIntServeProc, .-VDMDRV_MMUIntServeProc
	.align	3
.LC50:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC51:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC53:
	.xword	.LC52
	.align	3
.LC55:
	.xword	.LC54
	.align	3
.LC56:
	.xword	.LC19
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_IsVdmInISR
	.type	VDMDRV_IsVdmInISR, %function
VDMDRV_IsVdmInISR:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC57
	add	x19, x0, x19, sxtw 2
	ldr	w0, [x19, 400]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_IsVdmInISR, .-VDMDRV_IsVdmInISR
	.align	3
.LC57:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_SetStateIdle
	.type	VDMDRV_SetStateIdle, %function
VDMDRV_SetStateIdle:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	sbfiz	x19, x19, 6, 32
	bl	_mcount
	ldr	x0, .LC58
	str	wzr, [x0, x19]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_SetStateIdle, .-VDMDRV_SetStateIdle
	.align	3
.LC58:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_IsVdmIdle
	.type	VDMDRV_IsVdmIdle, %function
VDMDRV_IsVdmIdle:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	sbfiz	x19, x19, 6, 32
	bl	_mcount
	ldr	x0, .LC59
	ldr	w0, [x0, x19]
	ldr	x19, [sp, 16]
	cmp	w0, wzr
	cset	w0, ne
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_IsVdmIdle, .-VDMDRV_IsVdmIdle
	.align	3
.LC59:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_GetCurrChanID
	.type	VDMDRV_GetCurrChanID, %function
VDMDRV_GetCurrChanID:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	sbfiz	x19, x19, 6, 32
	bl	_mcount
	ldr	x0, .LC60
	add	x19, x0, x19
	ldr	w0, [x19, 8]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_GetCurrChanID, .-VDMDRV_GetCurrChanID
	.align	3
.LC60:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_WaitVdmReadyIfNoIsr
	.type	VDMDRV_WaitVdmReadyIfNoIsr, %function
VDMDRV_WaitVdmReadyIfNoIsr:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x0, x30
	mov	w19, 0
	bl	_mcount
	ldr	x20, .LC61
	b	.L121
	.p2align 3
.L123:
	add	w19, w19, 1
	cmp	w19, 400
	beq	.L122
.L121:
	ldr	x1, [x20, 264]
	mov	w0, w21
	cbz	x1, .L123
	blr	x1
	cbz	w0, .L123
.L122:
	cmp	w19, 399
	ldr	x21, [sp, 32]
	cset	w0, hi
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	VDMDRV_WaitVdmReadyIfNoIsr, .-VDMDRV_WaitVdmReadyIfNoIsr
	.align	3
.LC61:
	.xword	g_vdm_hal_fun_ptr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_AvsFirstFldNeedRepair
	.type	VDMDRV_AvsFirstFldNeedRepair, %function
VDMDRV_AvsFirstFldNeedRepair:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	x21, x1
	sxtw	x20, w2
	bl	_mcount
	cmp	w19, 6
	mov	w0, 0
	beq	.L132
.L129:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L132:
	ldrb	w1, [x21, 17]
	mov	w0, 0
	cmp	w1, 1
	bne	.L129
	lsl	x0, x20, 3
	ldr	x1, .LC62
	sub	x0, x0, x20
	lsl	x0, x0, 3
	sub	x0, x0, x20
	lsl	x0, x0, 2
	sub	x20, x0, x20
	add	x20, x1, x20, lsl 3
	ldr	w0, [x20, 1248]
	cmp	w0, wzr
	cset	w0, gt
	b	.L129
	.size	VDMDRV_AvsFirstFldNeedRepair, .-VDMDRV_AvsFirstFldNeedRepair
	.align	3
.LC62:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_AvsSecondFldNeedRepair
	.type	VDMDRV_AvsSecondFldNeedRepair, %function
VDMDRV_AvsSecondFldNeedRepair:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	x21, x1
	sxtw	x20, w2
	bl	_mcount
	cmp	w19, 6
	mov	w0, 0
	beq	.L137
.L134:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L137:
	ldrb	w1, [x21, 17]
	mov	w0, 0
	cmp	w1, 1
	bne	.L134
	lsl	x0, x20, 3
	ldr	x1, .LC63
	sub	x0, x0, x20
	lsl	x0, x0, 3
	sub	x0, x0, x20
	lsl	x0, x0, 2
	sub	x20, x0, x20
	add	x20, x1, x20, lsl 3
	ldr	w0, [x20, 2124]
	cmp	w0, wzr
	cset	w0, gt
	b	.L134
	.size	VDMDRV_AvsSecondFldNeedRepair, .-VDMDRV_AvsSecondFldNeedRepair
	.align	3
.LC63:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_IsMpeg4NvopCopy
	.type	VDMDRV_IsMpeg4NvopCopy, %function
VDMDRV_IsMpeg4NvopCopy:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L142
	cmp	w20, 2
	mov	w0, 0
	beq	.L143
.L140:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L143:
	ldrb	w0, [x19, 166]
	ldp	x19, x20, [sp, 16]
	cmp	w0, 1
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L142:
	ldr	x1, .LC65
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L140
	.size	VDMDRV_IsMpeg4NvopCopy, .-VDMDRV_IsMpeg4NvopCopy
	.align	3
.LC65:
	.xword	.LC64
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_IsVc1SkpicCopy
	.type	VDMDRV_IsVc1SkpicCopy, %function
VDMDRV_IsVc1SkpicCopy:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L148
	cmp	w20, 1
	mov	w0, 0
	beq	.L149
.L146:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L149:
	ldrb	w0, [x19, 1]
	ldp	x19, x20, [sp, 16]
	cmp	w0, 1
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L148:
	ldr	x1, .LC67
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L146
	.size	VDMDRV_IsVc1SkpicCopy, .-VDMDRV_IsVc1SkpicCopy
	.align	3
.LC67:
	.xword	.LC66
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_IsVp6NvopCopy
	.type	VDMDRV_IsVp6NvopCopy, %function
VDMDRV_IsVp6NvopCopy:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	cbz	x20, .L154
	sub	w19, w19, #10
	mov	w0, 0
	cmp	w19, 2
	bhi	.L152
	ldrb	w0, [x20, 1]
	cmp	w0, 1
	cset	w0, eq
.L152:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L154:
	ldr	x1, .LC69
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L152
	.size	VDMDRV_IsVp6NvopCopy, .-VDMDRV_IsVp6NvopCopy
	.align	3
.LC69:
	.xword	.LC68
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_DoBeforeDec
	.type	VDMDRV_DoBeforeDec, %function
VDMDRV_DoBeforeDec:
	stp	x29, x30, [sp, -304]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	w25, w0
	mov	x0, x30
	mov	w22, w1
	mov	x21, x2
	bl	_mcount
	mov	x26, 0
	ldr	x0, .LC70
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L156
	ldr	x3, .LC71
	add	x0, x29, 112
	mov	x2, 188
	mov	w1, 0
	mov	x26, x0
	ldr	x3, [x3, 96]
	blr	x3
.L156:
	ldr	x0, [x21]
	cbz	x0, .L198
	mov	w0, w22
	sxtw	x23, w25
	bl	VCTRL_GetVidStd
	ldr	x24, .LC74
	mov	w19, w0
	ldr	x0, .LC75
	add	x27, x24, x23, lsl 6
	ldr	x1, [x0]
	str	w19, [x27, 12]
	cbz	x1, .L159
	ldr	x0, [x21]
	str	x0, [x29, 104]
	add	x0, x29, 96
	stp	w22, w19, [x29, 96]
	blr	x1
	cmp	w0, 1
	mov	w20, w0
	beq	.L191
	ldr	w19, [x27, 12]
.L159:
	ldr	x0, [x21]
	cbz	x0, .L199
	cmp	w19, 2
	beq	.L200
.L163:
	cmp	w19, 1
	beq	.L201
.L165:
	mov	x1, x0
	mov	w0, w19
	bl	VDMDRV_IsVp6NvopCopy
	cmp	w0, 1
	beq	.L202
	add	x19, x24, x23, lsl 6
	ldr	x1, [x21]
	ldr	w0, [x19, 12]
	bl	VCTRL_IsSedOnlyMode
	uxtb	w0, w0
	cbz	w0, .L203
	mov	w0, 1
	str	w0, [x19, 56]
.L170:
	ldr	x0, .LC83
	ldr	x4, [x0, 256]
	cbz	x4, .L172
	add	x19, x24, x23, lsl 6
	ldr	x1, [x21]
	mov	x3, x26
	mov	w2, w25
	ldr	w0, [x19, 12]
	blr	x4
	mov	w20, w0
	cbz	w0, .L204
.L172:
	ldr	x1, .LC85
	lsl	x23, x23, 6
	mov	w0, 0
	add	x19, x24, x23
	mov	w20, 0
	bl	dprint_vfmw
	str	wzr, [x24, x23]
	mov	w5, 100
	mov	w0, w22
	mov	w1, w5
	mov	w4, w25
	mov	x3, 0
	mov	w2, 15
	str	w5, [x19, 4]
	add	x22, x24, x22, sxtw 2
	bl	VCTRL_VdmPostProc
	ldr	x3, .LC71
	mov	x2, 64
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	str	wzr, [x22, 208]
.L191:
	mov	w0, w20
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 304
	ret
	.p2align 3
.L200:
	ldrb	w1, [x0, 166]
	cmp	w1, 1
	bne	.L165
	mov	x2, x26
	mov	w1, w25
	bl	PostPro_CopyYuv
	mov	w20, w0
	cbz	w0, .L205
.L167:
	mov	w20, 0
	ldr	x27, [sp, 80]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 304
	ret
	.p2align 3
.L201:
	ldrb	w1, [x0, 1]
	cmp	w1, 1
	bne	.L165
	ldr	x1, .LC82
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x0, [x21]
	mov	x2, x26
	mov	w1, w25
	bl	PostPro_CopyYuv_Vc1
	mov	w20, w0
	cbnz	w0, .L167
	add	x19, x24, x23, lsl 6
	mov	x2, x26
	mov	w1, w25
	mov	w0, 2
	str	wzr, [x19, 4]
	bl	VDMDRV_StartRepair
	ldr	x21, .LC71
	ldr	x0, [x21]
	blr	x0
	mov	w1, w0
	lsl	x0, x23, 2
	ldr	x3, .LC77
	sub	x23, x0, x23
	add	x2, x24, x22, sxtw 2
	mov	w0, w22
	str	w1, [x19, 20]
	add	x23, x3, x23, lsl 5
	add	w3, w25, 1
	str	w3, [x2, 208]
	str	w1, [x23, 4]
	bl	VDMDRV_AdjustPriorityAfterDec
	mov	w0, 7
	bl	CHECK_REC_POS_ENABLE
	cbz	w0, .L167
	ldr	x0, [x21]
	add	x24, x24, 2160
	blr	x0
	mov	w3, w0
	ldr	x1, .LC78
	ldr	w0, [x24, w22, sxtw 2]
	ldr	w1, [x1]
	sub	w0, w3, w0
	cmp	w0, w1
	bls	.L167
	ldr	x0, .LC79
	add	w2, w25, w22, lsl 24
	add	w2, w2, 458752
	mov	w1, 704
	add	x0, x0, 48
	str	w3, [x24, w22, sxtw 2]
	bl	DBG_AddTrace
	b	.L191
	.p2align 3
.L203:
	str	wzr, [x19, 56]
	b	.L170
	.p2align 3
.L202:
	ldr	x1, .LC82
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x0, [x21]
	mov	x2, x26
	mov	w1, w25
	bl	PostPro_CopyYuv_Vp6
	mov	w20, w0
	cbnz	w0, .L167
	add	x19, x24, x23, lsl 6
	mov	x2, x26
	mov	w1, w25
	mov	w0, 2
	str	wzr, [x19, 4]
	bl	VDMDRV_StartRepair
	ldr	x21, .LC71
	ldr	x0, [x21]
	blr	x0
	mov	w1, w0
	lsl	x0, x23, 2
	ldr	x3, .LC77
	sub	x23, x0, x23
	add	x2, x24, x22, sxtw 2
	mov	w0, w22
	str	w1, [x19, 20]
	add	x23, x3, x23, lsl 5
	add	w3, w25, 1
	str	w3, [x2, 208]
	str	w1, [x23, 4]
	bl	VDMDRV_AdjustPriorityAfterDec
	mov	w0, 7
	bl	CHECK_REC_POS_ENABLE
	cbz	w0, .L167
	ldr	x0, [x21]
	add	x24, x24, 2160
	blr	x0
	mov	w3, w0
	ldr	x1, .LC78
	ldr	w0, [x24, w22, sxtw 2]
	ldr	w1, [x1]
	sub	w0, w3, w0
	cmp	w0, w1
	bls	.L167
	ldr	x0, .LC79
	add	w2, w25, w22, lsl 24
	add	w2, w2, 458752
	mov	w1, 737
	add	x0, x0, 48
	str	w3, [x24, w22, sxtw 2]
	bl	DBG_AddTrace
	b	.L191
	.p2align 3
.L204:
	mov	x1, x26
	mov	w0, w25
	bl	VDMDRV_StartDec
	ldr	x21, .LC71
	ldr	x0, [x21]
	blr	x0
	mov	w1, w0
	lsl	x0, x23, 2
	ldr	x3, .LC77
	sub	x23, x0, x23
	add	x2, x24, x22, sxtw 2
	mov	w0, w22
	str	w1, [x19, 20]
	add	x23, x3, x23, lsl 5
	add	w3, w25, 1
	str	w3, [x2, 208]
	str	w1, [x23, 4]
	bl	VDMDRV_AdjustPriorityAfterDec
	mov	w0, 5
	bl	CHECK_REC_POS_ENABLE
	cbz	w0, .L167
	ldr	x0, [x21]
	add	x24, x24, 2160
	blr	x0
	mov	w3, w0
	ldr	x1, .LC78
	ldr	w0, [x24, w22, sxtw 2]
	ldr	w1, [x1]
	sub	w0, w3, w0
	cmp	w0, w1
	bls	.L167
	ldr	x0, .LC79
	add	w2, w25, w22, lsl 24
	add	w2, w2, 327680
	mov	w1, 784
	add	x0, x0, 48
	str	w3, [x24, w22, sxtw 2]
	bl	DBG_AddTrace
	b	.L191
	.p2align 3
.L198:
	ldr	x1, .LC73
	mov	w2, w22
	mov	w0, 22
	mov	w20, 1
	bl	dprint_vfmw
	ldr	x27, [sp, 80]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 304
	ret
	.p2align 3
.L199:
	ldr	x1, .LC76
	add	x20, x24, x23, lsl 6
	bl	dprint_vfmw
	ldr	w19, [x20, 12]
	ldr	x0, [x21]
	cbnz	x0, .L163
	ldr	x1, .LC80
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w19, [x20, 12]
	ldr	x0, [x21]
	b	.L165
	.p2align 3
.L205:
	add	x21, x24, x23, lsl 6
	mov	x2, x26
	mov	w1, w25
	mov	w0, w19
	str	wzr, [x21, 4]
	bl	VDMDRV_StartRepair
	ldr	x19, .LC71
	ldr	x0, [x19]
	blr	x0
	mov	w1, w0
	lsl	x0, x23, 2
	ldr	x3, .LC77
	sub	x23, x0, x23
	add	x2, x24, x22, sxtw 2
	mov	w0, w22
	str	w1, [x21, 20]
	add	x23, x3, x23, lsl 5
	add	w3, w25, 1
	str	w3, [x2, 208]
	str	w1, [x23, 4]
	bl	VDMDRV_AdjustPriorityAfterDec
	mov	w0, 7
	bl	CHECK_REC_POS_ENABLE
	cbz	w0, .L167
	ldr	x0, [x19]
	add	x24, x24, 2160
	blr	x0
	mov	w3, w0
	ldr	x1, .LC78
	ldr	w0, [x24, w22, sxtw 2]
	ldr	w1, [x1]
	sub	w0, w3, w0
	cmp	w0, w1
	bls	.L167
	ldr	x0, .LC79
	add	w2, w25, w22, lsl 24
	add	w2, w2, 458752
	mov	w1, 671
	add	x0, x0, 48
	str	w3, [x24, w22, sxtw 2]
	bl	DBG_AddTrace
	b	.L191
	.size	VDMDRV_DoBeforeDec, .-VDMDRV_DoBeforeDec
	.align	3
.LC70:
	.xword	g_HalDisable
	.align	3
.LC71:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC73:
	.xword	.LC72
	.align	3
.LC74:
	.xword	.LANCHOR0
	.align	3
.LC75:
	.xword	AcceleratorCharacter
	.align	3
.LC76:
	.xword	.LC64
	.align	3
.LC77:
	.xword	g_VfmwGlobalStat
	.align	3
.LC78:
	.xword	g_TraceFramePeriod
	.align	3
.LC79:
	.xword	.LANCHOR1
	.align	3
.LC80:
	.xword	.LC66
	.align	3
.LC82:
	.xword	.LC81
	.align	3
.LC83:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC85:
	.xword	.LC84
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_ActivateVDH
	.type	VDMDRV_ActivateVDH, %function
VDMDRV_ActivateVDH:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	w24, w0
	mov	x0, x30
	sbfiz	x20, x24, 6, 32
	bl	_mcount
	ldr	x25, .LC88
	ldr	x22, .LC87
	mov	w0, 5
	add	x21, x20, 48
	ldr	x1, [x25, 24]
	add	x21, x22, x21
	blr	x1
	ldr	w0, [x22, 144]
	tbnz	w0, #31, .L207
	add	x19, x22, 148
	add	x23, x22, 208
	add	x20, x22, x20
	b	.L208
	.p2align 3
.L211:
	cmp	x19, x23
	beq	.L207
	ldr	w0, [x19], 4
	tbnz	w0, #31, .L207
.L208:
	mov	x1, x21
	str	w0, [x20, 8]
	bl	VDMDRV_ChnDecReady
	cbnz	w0, .L211
	ldr	w1, [x20, 8]
	mov	x2, x21
	mov	w0, w24
	bl	VDMDRV_DoBeforeDec
	cbnz	w0, .L211
.L207:
	ldr	x1, [x25, 32]
	mov	w0, 5
	blr	x1
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.size	VDMDRV_ActivateVDH, .-VDMDRV_ActivateVDH
	.align	3
.LC87:
	.xword	.LANCHOR0
	.align	3
.LC88:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_AfterDec
	.type	VDMDRV_AfterDec, %function
VDMDRV_AfterDec:
	stp	x29, x30, [sp, -464]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	w20, w0
	mov	x0, x30
	uxtb	w24, w1
	bl	_mcount
	ldr	x21, .LC91
	ldr	x0, .LC90
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L270
	ldr	x3, [x21, 96]
	add	x0, x29, 272
	mov	x2, 188
	mov	w1, 0
	str	x0, [x29, 104]
	sxtw	x26, w20
	blr	x3
	ldr	x19, .LC93
	ldr	x22, .LC92
	add	x0, x19, x26, lsl 6
	ldrsw	x0, [x0, 8]
	ldr	x0, [x22, x0, lsl 3]
	cbz	x0, .L271
.L309:
	ldr	w23, [x0, 1336]
.L215:
	mov	x2, 136
	ldr	x3, [x21, 96]
	mov	w1, 0
	add	x0, x29, x2
	blr	x3
	lsl	x2, x26, 2
	add	x3, x19, 2224
	sub	x2, x2, x26
	lsl	x1, x2, 4
	sub	x0, x1, x2
	lsl	x0, x0, 2
	ldr	w1, [x3, x0]
	add	x0, x3, x0
	and	w1, w1, 15
	cmp	w1, 5
	beq	.L304
	ldr	w28, [x0, 8]
	and	w28, w28, 31
.L217:
	lsl	x25, x26, 6
	ldr	x1, .LC95
	add	x27, x19, x25
	mov	w0, 2
	ldr	w2, [x19, x25]
	ldr	w3, [x27, 28]
	ldr	w4, [x27, 4]
	bl	dprint_vfmw
	ldr	w3, [x19, x25]
	cmp	w3, 1
	beq	.L305
	cmp	w3, 2
	beq	.L306
	cmp	w3, 3
	beq	.L307
	ldr	x1, .LC109
	mov	w4, w24
	ldr	w2, [x27, 8]
	mov	w0, 0
	bl	dprint_vfmw
	ldrsw	x0, [x27, 8]
	add	x19, x19, x0, lsl 2
	str	wzr, [x19, 208]
.L213:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 464
	ret
	.p2align 3
.L307:
	ldr	w0, [x27, 28]
	cmp	w0, 1
	beq	.L308
.L257:
	lsl	x26, x26, 6
	mov	w1, 100
	add	x22, x19, x26
	mov	w4, w20
	add	x3, x29, 136
	mov	w2, w28
	str	wzr, [x19, x26]
	ldr	w0, [x22, 4]
	cmp	w0, w1
	csel	w1, w0, w1, le
	ldr	w0, [x22, 8]
	cmp	w1, wzr
	csel	w1, w1, wzr, ge
	str	w1, [x22, 4]
.L294:
	bl	VCTRL_VdmPostProc
	ldrsw	x3, [x22, 8]
	mov	x2, 64
	ldr	x4, [x21, 96]
	mov	w1, 0
	mov	x0, x22
.L295:
	add	x19, x19, x3, lsl 2
	str	wzr, [x19, 208]
	blr	x4
	mov	w0, w20
	bl	VDMDRV_ActivateVDH
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 464
	ret
	.p2align 3
.L270:
	ldr	x19, .LC93
	sxtw	x26, w20
	ldr	x22, .LC92
	add	x0, x19, x26, lsl 6
	str	xzr, [x29, 104]
	ldrsw	x0, [x0, 8]
	ldr	x0, [x22, x0, lsl 3]
	cbnz	x0, .L309
.L271:
	mov	w23, 0
	b	.L215
	.p2align 3
.L305:
	ldr	w0, [x27, 28]
	cmp	w0, 1
	beq	.L310
	ldr	x4, [x21, 96]
	mov	x2, 16
	mov	w1, 0
	add	x0, x29, 120
	str	w3, [x29, 100]
	blr	x4
	ldr	w1, [x27, 12]
	add	x0, x29, 120
	stp	w20, w1, [x29, 120]
	ldr	x1, [x27, 48]
	str	x1, [x29, 128]
	bl	PostPro_GetErrRatio
	mov	w1, w0
	ldr	w3, [x29, 100]
	str	w1, [x27, 4]
	mov	w0, w3
	ldr	w2, [x27, 8]
	mov	w3, w1
	ldr	x1, .LC99
	bl	dprint_vfmw
	ldr	w0, [x27, 12]
	sub	w1, w0, #8
	cmp	w1, 1
	ccmp	w0, 5, 4, hi
	beq	.L311
	ldr	w0, [x27, 4]
	cbz	w0, .L220
	ldr	x1, .LC100
	ldr	x4, [x1]
	cbz	x4, .L220
	str	w0, [x29, 112]
	mov	w3, 4
	ldr	w0, [x27, 8]
	add	x2, x29, 112
	mov	w1, 114
	blr	x4
.L220:
	cmp	w23, 1
	beq	.L312
.L222:
	add	x24, x19, x26, lsl 6
	ldrsw	x0, [x24, 8]
	ldr	x0, [x22, x0, lsl 3]
	add	x0, x0, 327680
	ldrsb	w0, [x0, 2712]
	cbnz	w0, .L313
	ldr	w1, [x24, 4]
.L261:
	cmp	w23, 1
	beq	.L314
	cmp	w1, 100
	lsl	x26, x26, 6
	mov	w0, 100
	add	x22, x19, x26
	csel	w0, w1, w0, le
	mov	w4, w20
	cmp	w0, wzr
	add	x3, x29, 136
	csel	w0, w0, wzr, ge
	mov	w2, w28
	mov	w1, w0
	str	w0, [x22, 4]
	str	wzr, [x19, x26]
	ldr	w0, [x22, 8]
	b	.L294
	.p2align 3
.L304:
	ldr	w28, [x0, 12]
	and	w28, w28, 1
	b	.L217
	.p2align 3
.L306:
	ldr	w0, [x27, 28]
	cmp	w0, 1
	beq	.L315
.L245:
	add	x22, x19, x26, lsl 6
	ldr	w0, [x22, 12]
	ldr	x1, [x22, 48]
	cmp	w0, 6
	beq	.L316
.L246:
	cbz	x1, .L317
	cmp	w0, 2
	beq	.L318
	add	x1, x19, x26, lsl 6
	ldr	w0, [x1, 12]
	ldr	x1, [x1, 48]
.L250:
	cmp	w0, 1
	beq	.L319
.L254:
	bl	VDMDRV_IsVp6NvopCopy
	cmp	w0, 1
	beq	.L255
	lsl	x5, x26, 6
	add	x22, x19, x5
	ldr	x0, [x22, 48]
	ldr	w1, [x22, 12]
	cbz	x0, .L320
	cmp	w1, 17
	bne	.L257
	add	x0, x0, 262144
	ldrb	w0, [x0, 1028]
	cmp	w0, 1
	bne	.L257
	mov	w4, w20
	add	x3, x29, 136
	mov	w2, w28
	mov	w1, 0
	str	wzr, [x19, x5]
	ldr	w0, [x22, 8]
	str	wzr, [x22, 4]
	b	.L294
	.p2align 3
.L311:
	str	wzr, [x27, 4]
	b	.L220
	.p2align 3
.L308:
	str	wzr, [x27, 28]
	b	.L257
	.p2align 3
.L316:
	ldrb	w2, [x1, 17]
	cmp	w2, 1
	bne	.L246
	lsl	x3, x26, 3
	sub	x3, x3, x26
	lsl	x3, x3, 3
	sub	x3, x3, x26
	lsl	x3, x3, 2
	sub	x3, x3, x26
	add	x3, x19, x3, lsl 3
	ldr	w3, [x3, 2124]
	cmp	w3, wzr
	ble	.L246
	ldr	x3, .LC110
	ldr	x5, [x3, 280]
	cbz	x5, .L248
	ldr	x4, [x29, 104]
	mov	w3, w20
	blr	x5
	cbnz	w0, .L248
	ldr	x2, [x29, 104]
	mov	w1, w20
	mov	w0, 3
	bl	VDMDRV_StartRepair
	ldr	x0, [x21]
	blr	x0
	mov	w2, w0
	lsl	x0, x26, 2
	ldr	x1, .LC101
	sub	x26, x0, x26
	str	w2, [x22, 20]
	mov	w0, 7
	add	x1, x1, x26, lsl 5
	str	w2, [x1, 4]
	bl	CHECK_REC_POS_ENABLE
	cbz	w0, .L213
	ldr	x0, [x21]
	add	x19, x19, 2408
	blr	x0
	mov	w4, w0
	ldr	w3, [x22, 8]
	ldr	x1, .LC102
	ldr	w0, [x19, w3, sxtw 2]
	ldr	w1, [x1]
	sub	w0, w4, w0
	cmp	w0, w1
	bls	.L213
	ldr	x0, .LC103
	add	w2, w20, w3, lsl 24
	add	w2, w2, 458752
	mov	w1, 1192
	add	x0, x0, 72
	str	w4, [x19, w3, sxtw 2]
	bl	DBG_AddTrace
	b	.L213
	.p2align 3
.L315:
	str	wzr, [x27, 28]
	b	.L245
	.p2align 3
.L313:
	ldr	x1, [x24, 48]
	ldr	w0, [x24, 12]
	bl	PostPro_GetRepairStratage
	ldr	w1, [x24, 4]
	cmp	w1, wzr
	ccmp	w0, 1, 4, ne
	beq	.L261
	ldr	w0, [x24, 12]
	ldr	x1, [x24, 48]
	cmp	w0, 6
	beq	.L321
.L236:
	ldr	x2, .LC110
	ldr	x5, [x2, 280]
	cbz	x5, .L248
	ldr	x4, [x29, 104]
	mov	w3, w20
	mov	w2, 0
	blr	x5
	cbz	w0, .L322
.L248:
	lsl	x26, x26, 6
	mov	w4, w20
	add	x20, x19, x26
	add	x3, x29, 136
	mov	w2, w28
	mov	w1, 100
	ldr	w0, [x20, 8]
	bl	VCTRL_VdmPostProc
	str	wzr, [x19, x26]
	ldrsw	x0, [x20, 8]
	add	x19, x19, x0, lsl 2
	str	wzr, [x19, 208]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 464
	ret
	.p2align 3
.L312:
	add	x0, x19, x26, lsl 6
	str	wzr, [x0, 4]
	b	.L222
	.p2align 3
.L310:
	ldr	x1, [x27, 48]
	mov	w2, w20
	ldr	w0, [x27, 12]
	bl	PostPro_SetFullRepair
	str	wzr, [x27, 28]
	mov	w0, 100
	ldr	x1, .LC97
	str	w0, [x27, 4]
	mov	w2, w24
	mov	w0, 0
	bl	dprint_vfmw
	b	.L220
	.p2align 3
.L314:
	lsl	x24, x26, 6
	cmp	w1, 100
	add	x23, x19, x24
	mov	w0, 100
	csel	w1, w1, w0, le
	cmp	w1, wzr
	ldr	w0, [x23, 44]
	csel	w1, w1, wzr, ge
	str	w1, [x23, 4]
	cmp	w0, 1
	beq	.L323
	cmp	w0, 2
	beq	.L324
	cmp	w0, 3
	bne	.L213
	ldr	w0, [x23, 8]
	add	x3, x29, 136
	mov	w2, w28
	mov	w4, w20
	bl	VCTRL_VdmPostProc
	str	wzr, [x19, x24]
	ldr	w0, [x23, 12]
	cbnz	w0, .L233
.L297:
	ldrsw	x0, [x23, 8]
	ldr	x0, [x22, x0, lsl 3]
	add	x0, x0, 327680
	str	xzr, [x0, 3376]
.L234:
	add	x26, x19, x26, lsl 6
	mov	x2, 64
	mov	w1, 0
	mov	x0, x26
	ldr	x4, [x21, 96]
	ldrsw	x3, [x26, 8]
	b	.L295
	.p2align 3
.L322:
	ldr	x2, [x29, 104]
	mov	w1, w20
	mov	w0, 2
	add	x22, x19, x26, lsl 6
	bl	VDMDRV_StartRepair
	ldr	x0, [x21]
	blr	x0
	mov	w2, w0
	lsl	x0, x26, 2
	ldr	x1, .LC101
	sub	x26, x0, x26
	str	w2, [x22, 20]
	mov	w0, 7
	add	x1, x1, x26, lsl 5
	str	w2, [x1, 4]
	bl	CHECK_REC_POS_ENABLE
	cbz	w0, .L213
	ldr	x0, [x21]
	add	x19, x19, 2408
	blr	x0
	mov	w4, w0
	ldr	w3, [x22, 8]
	ldr	x1, .LC102
	ldr	w0, [x19, w3, sxtw 2]
	ldr	w1, [x1]
	sub	w0, w4, w0
	cmp	w0, w1
	bls	.L213
	ldr	x0, .LC103
	add	w2, w20, w3, lsl 24
	add	w2, w2, 458752
	mov	w1, 1151
	add	x0, x0, 72
	str	w4, [x19, w3, sxtw 2]
	bl	DBG_AddTrace
	b	.L213
.L323:
	ldr	w0, [x23, 8]
	mov	w4, w20
	add	x3, x29, 136
	mov	w2, w28
	bl	VCTRL_VdmPostProc
	ldr	w0, [x23, 12]
	cbz	w0, .L297
.L233:
	cmp	w0, 16
	bne	.L235
	ldrsw	x0, [x23, 8]
	ldr	x0, [x22, x0, lsl 3]
	add	x0, x0, 331776
	str	xzr, [x0, 1048]
	b	.L234
	.p2align 3
.L319:
	ldrb	w2, [x1, 1]
	cmp	w2, 1
	bne	.L254
.L255:
	lsl	x26, x26, 6
	mov	w4, w20
	add	x22, x19, x26
	add	x3, x29, 136
	mov	w2, w28
	mov	w1, 0
	str	wzr, [x19, x26]
	ldr	w0, [x22, 8]
	str	wzr, [x22, 4]
	b	.L294
	.p2align 3
.L318:
	ldrb	w0, [x1, 166]
	cmp	w0, 1
	beq	.L255
	add	x1, x19, x26, lsl 6
	ldr	w0, [x1, 12]
	ldr	x1, [x1, 48]
	b	.L254
	.p2align 3
.L321:
	ldrb	w2, [x1, 17]
	cmp	w2, 1
	bne	.L236
	lsl	x3, x26, 3
	add	x4, x19, 408
	sub	x3, x3, x26
	lsl	x3, x3, 3
	sub	x3, x3, x26
	lsl	x3, x3, 2
	sub	x3, x3, x26
	add	x3, x4, x3, lsl 3
	ldr	w4, [x3, 840]
	cmp	w4, wzr
	ble	.L325
	ldr	x2, .LC110
	ldr	x5, [x2, 280]
	cbz	x5, .L248
	ldr	x4, [x29, 104]
	mov	w3, w20
	mov	w2, 0
	blr	x5
	cbnz	w0, .L248
	ldr	x2, [x29, 104]
	mov	w1, w20
	mov	w0, 2
	bl	VDMDRV_StartRepair
	ldr	x0, [x21]
	blr	x0
	mov	w2, w0
	lsl	x0, x26, 2
	ldr	x1, .LC101
	sub	x26, x0, x26
	str	w2, [x24, 20]
	mov	w0, 7
	add	x1, x1, x26, lsl 5
	str	w2, [x1, 4]
	bl	CHECK_REC_POS_ENABLE
	cbz	w0, .L213
	ldr	x0, [x21]
	add	x19, x19, 2408
	blr	x0
	mov	w4, w0
	ldr	w3, [x24, 8]
	ldr	x1, .LC102
	ldr	w0, [x19, w3, sxtw 2]
	ldr	w1, [x1]
	sub	w0, w4, w0
	cmp	w0, w1
	bls	.L213
	ldr	x0, .LC103
	add	w2, w20, w3, lsl 24
	add	w2, w2, 458752
	mov	w1, 1088
	add	x0, x0, 72
	str	w4, [x19, w3, sxtw 2]
	bl	DBG_AddTrace
	b	.L213
	.p2align 3
.L317:
	ldr	x1, .LC104
	add	x22, x19, x26, lsl 6
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, [x22, 48]
	ldr	w0, [x22, 12]
	cbnz	x1, .L250
	ldr	x1, .LC105
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x22, 12]
	ldr	x1, [x22, 48]
	b	.L254
.L320:
	ldr	x1, .LC107
	bl	dprint_vfmw
	b	.L257
.L325:
	ldr	w3, [x3, 1716]
	cmp	w3, wzr
	ble	.L236
	ldr	x3, .LC110
	ldr	x5, [x3, 280]
	cbz	x5, .L248
	ldr	x4, [x29, 104]
	mov	w3, w20
	blr	x5
	cbnz	w0, .L248
	ldr	x2, [x29, 104]
	mov	w1, w20
	mov	w0, 3
	bl	VDMDRV_StartRepair
	ldr	x0, [x21]
	blr	x0
	mov	w2, w0
	lsl	x0, x26, 2
	ldr	x1, .LC101
	sub	x26, x0, x26
	str	w2, [x24, 20]
	mov	w0, 7
	add	x1, x1, x26, lsl 5
	str	w2, [x1, 4]
	bl	CHECK_REC_POS_ENABLE
	cbz	w0, .L213
	ldr	x0, [x21]
	add	x19, x19, 2408
	blr	x0
	mov	w4, w0
	ldr	w3, [x24, 8]
	ldr	x1, .LC102
	ldr	w0, [x19, w3, sxtw 2]
	ldr	w1, [x1]
	sub	w0, w4, w0
	cmp	w0, w1
	bls	.L213
	ldr	x0, .LC103
	add	w2, w20, w3, lsl 24
	add	w2, w2, 458752
	mov	w1, 1119
	add	x0, x0, 72
	str	w4, [x19, w3, sxtw 2]
	bl	DBG_AddTrace
	b	.L213
.L324:
	ldr	w0, [x23, 8]
	mov	w4, w20
	add	x3, x29, 136
	mov	w2, w28
	bl	VCTRL_VdmPostProc
	b	.L213
.L235:
	cmp	w0, 17
	bne	.L234
	ldrsw	x0, [x23, 8]
	ldr	x0, [x22, x0, lsl 3]
	add	x0, x0, 327680
	str	xzr, [x0, 2768]
	b	.L234
	.size	VDMDRV_AfterDec, .-VDMDRV_AfterDec
	.align	3
.LC90:
	.xword	g_HalDisable
	.align	3
.LC91:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC92:
	.xword	s_pstVfmwChan
	.align	3
.LC93:
	.xword	.LANCHOR0
	.align	3
.LC95:
	.xword	.LC94
	.align	3
.LC97:
	.xword	.LC96
	.align	3
.LC99:
	.xword	.LC98
	.align	3
.LC100:
	.xword	g_event_report
	.align	3
.LC101:
	.xword	g_VfmwGlobalStat
	.align	3
.LC102:
	.xword	g_TraceFramePeriod
	.align	3
.LC103:
	.xword	.LANCHOR1
	.align	3
.LC104:
	.xword	.LC64
	.align	3
.LC105:
	.xword	.LC66
	.align	3
.LC107:
	.xword	.LC106
	.align	3
.LC109:
	.xword	.LC108
	.align	3
.LC110:
	.xword	g_vdm_hal_fun_ptr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_CheckDevice
	.type	VDMDRV_CheckDevice, %function
VDMDRV_CheckDevice:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	sxtw	x19, w0
	mov	x0, x30
	lsl	x21, x19, 6
	mov	x22, x19
	bl	_mcount
	ldr	x20, .LC114
	add	x23, x20, x21
	ldr	w0, [x23, 8]
	bl	VCTRL_GetDecParam
	cbz	x0, .L355
.L327:
	lsl	x0, x19, 6
	add	x21, x20, x0
	ldr	w0, [x20, x0]
	cbnz	w0, .L356
.L328:
	add	x19, x20, x19, lsl 2
	mov	w0, 0
	ldr	w1, [x19, 64]
	cbz	w1, .L349
	cmp	w1, 1
	beq	.L357
.L333:
	mov	w0, 1
.L349:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L356:
	ldr	x0, .LC121
	ldr	x0, [x0]
	blr	x0
	str	w0, [x21, 24]
	ldr	w1, [x21, 20]
	cmp	w0, w1
	bcc	.L331
	sub	w3, w0, w1
	cmp	w1, wzr
	ccmp	w3, wzr, 1, ne
	blt	.L331
	cmp	w3, 1000
	bls	.L333
	ldr	x1, .LC123
	mov	w0, 1
	ldr	w2, [x21, 8]
	bl	dprint_vfmw
	ldr	x0, .LC124
	ldr	x4, [x0]
	cbz	x4, .L334
	ldr	w0, [x21, 8]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 101
	blr	x4
.L334:
	ldr	x0, .LC125
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L335
	mov	w1, w22
	mov	w0, 1
	bl	VDH_Reset_Module
.L336:
	lsl	x19, x19, 6
	mov	w21, 1
	add	x0, x20, x19
	ldr	w1, [x0, 36]
	str	w21, [x0, 28]
	cmp	w1, w21
	bne	.L338
	ldrsw	x1, [x0, 8]
	str	wzr, [x20, x19]
	str	wzr, [x0, 28]
	add	x19, x20, x1, lsl 2
	str	wzr, [x0, 36]
	mov	w0, 0
	str	wzr, [x19, 208]
	b	.L349
	.p2align 3
.L331:
	str	w0, [x21, 20]
	mov	w0, 1
	b	.L349
	.p2align 3
.L357:
	mov	w2, 2
	str	w2, [x19, 64]
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L355:
	ldr	w3, [x20, x21]
	cbz	w3, .L328
	ldr	x0, .LC115
	ldr	x1, [x0, 272]
	cbz	x1, .L329
	mov	w0, w19
	blr	x1
	ldr	w3, [x20, x21]
	cbnz	w0, .L358
.L329:
	lsl	x23, x19, 6
	ldr	x1, .LC119
	add	x21, x20, x23
	mov	w0, 1
	ldr	w2, [x21, 8]
	bl	dprint_vfmw
	str	wzr, [x21, 36]
	ldr	x0, .LC120
	mov	w1, 1309
	ldr	w2, [x20, x23]
	add	x0, x0, 88
	bl	DBG_AddTrace
	ldr	x3, .LC121
	mov	x2, 64
	mov	w1, 0
	mov	x0, x21
	ldr	x3, [x3, 96]
	blr	x3
	ldrsw	x0, [x21, 8]
	add	x0, x20, x0, lsl 2
	str	wzr, [x0, 208]
	b	.L327
	.p2align 3
.L338:
	mov	w0, w22
	mov	w1, 0
	bl	VDMDRV_AfterDec
	mov	w0, w21
	b	.L349
	.p2align 3
.L335:
	ldr	x0, .LC115
	ldr	x1, [x0, 160]
	cbz	x1, .L337
	mov	w0, w22
	blr	x1
	b	.L336
	.p2align 3
.L358:
	ldr	x1, .LC117
	mov	w0, 1
	ldr	w2, [x23, 8]
	bl	dprint_vfmw
	mov	w0, 1
	str	w0, [x23, 36]
	b	.L327
.L337:
	ldr	x1, .LC126
	mov	w2, 1360
	mov	w0, 0
	bl	dprint_vfmw
	b	.L336
	.size	VDMDRV_CheckDevice, .-VDMDRV_CheckDevice
	.align	3
.LC114:
	.xword	.LANCHOR0
	.align	3
.LC115:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC117:
	.xword	.LC116
	.align	3
.LC119:
	.xword	.LC118
	.align	3
.LC120:
	.xword	.LANCHOR1
	.align	3
.LC121:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC123:
	.xword	.LC122
	.align	3
.LC124:
	.xword	g_event_report
	.align	3
.LC125:
	.xword	g_HalDisable
	.align	3
.LC126:
	.xword	.LC19
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_WakeUpVdm
	.type	VDMDRV_WakeUpVdm, %function
VDMDRV_WakeUpVdm:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	bl	VDMDRV_CheckDevice
	cbz	w0, .L361
	ldp	x29, x30, [sp], 16
	ret
	.p2align 3
.L361:
	bl	VDMDRV_ActivateVDH
	ldp	x29, x30, [sp], 16
	ret
	.size	VDMDRV_WakeUpVdm, .-VDMDRV_WakeUpVdm
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_VdmIntServProc
	.type	VDMDRV_VdmIntServProc, %function
VDMDRV_VdmIntServProc:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	mov	w0, 15
	cmp	w20, wzr
	str	w0, [x29, 92]
	bgt	.L421
	ldr	x21, .LC131
	sxtw	x19, w20
	ldr	x0, .LC130
	add	x25, x21, x19, lsl 6
	ldr	x22, .LC132
	ldrsw	x1, [x25, 8]
	ldr	x1, [x0, x1, lsl 3]
	mov	w0, 0
	cbz	x1, .L365
	ldr	w24, [x1, 1336]
	ldr	x1, [x22, 24]
	blr	x1
	ldr	x23, .LC133
	ldr	x2, [x23, 216]
	cbz	x2, .L370
	mov	w1, w20
	mov	w0, 2
	blr	x2
	mov	w1, w0
	cmp	w24, 1
	beq	.L422
	tbz	x1, 0, .L370
.L372:
	mov	w24, 1
.L371:
	add	x0, x21, x19, lsl 2
	mov	w1, 1
	ldr	x3, [x22, 96]
	mov	x2, 16
	str	w1, [x0, 400]
	mov	w1, 0
	add	x0, x29, 96
	blr	x3
	str	w20, [x29, 104]
	ldr	x1, [x23, 296]
	add	x0, x29, 92
	str	x0, [x29, 96]
	cbz	x1, .L376
	add	x0, x29, 96
	blr	x1
.L377:
	ldr	x1, [x23, 184]
	cbz	x1, .L378
	mov	w0, w20
	blr	x1
.L379:
	mov	w0, 6
	bl	CHECK_REC_POS_ENABLE
	cbnz	w0, .L423
.L381:
	lsl	x1, x19, 6
	add	x0, x21, x1
	ldr	w2, [x0, 36]
	cmp	w2, 1
	beq	.L424
	lsl	x1, x19, 2
	ldr	x25, .LC136
	sub	x0, x1, x19
	add	x1, x21, x1
	lsl	x2, x0, 4
	lsl	x23, x0, 5
	sub	x2, x2, x0
	add	x26, x25, x23
	add	x0, x21, x2, lsl 2
	ldr	w2, [x1, 2536]
	ldr	w0, [x0, 2248]
	add	w0, w2, w0
	str	w0, [x1, 2536]
	ldr	w0, [x26, 4]
	cbnz	w0, .L425
.L386:
	lsl	x23, x19, 6
	add	x25, x21, x23
	ldr	w0, [x25, 56]
	cmp	w0, 1
	beq	.L426
	ldr	w2, [x21, x23]
	cbz	w2, .L393
	ldr	x0, [x25, 48]
	cbz	x0, .L427
.L393:
	lsl	x25, x19, 6
	add	x23, x21, x25
	ldr	w0, [x23, 8]
	bl	VCTRL_IsChanActive
	cbz	w0, .L395
	ldr	x1, .LC140
	mov	w2, 1612
	ldr	w3, [x23, 8]
	mov	w0, 0
.L420:
	bl	dprint_vfmw
	str	wzr, [x21, x25]
	ldrsw	x0, [x23, 8]
	add	x0, x21, x0, lsl 2
	str	wzr, [x0, 208]
.L396:
.L384:
	add	x0, x21, x19, lsl 6
	add	x19, x21, x19, lsl 2
	ldr	w0, [x0, 8]
	str	wzr, [x19, 400]
	bl	VCTRL_InformVdmFree
	cbnz	w24, .L369
.L398:
	ldr	x1, [x22, 32]
	mov	w0, 0
	blr	x1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L370:
	lsl	x19, x19, 6
	str	wzr, [x21, x19]
.L369:
	ldr	x0, .LC143
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L398
	mov	w1, w20
	mov	w0, 1
	bl	VDH_Notify_ModuleIdle
	b	.L398
	.p2align 3
.L421:
	ldr	x2, .LC127
	mov	w3, w20
	ldr	x1, .LC129
	mov	w4, 0
	add	x2, x2, 112
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L422:
	mov	w1, 5
	tst	w0, w1
	beq	.L370
	ldr	x2, [x23, 216]
	cbz	x2, .L372
	mov	w1, w20
	mov	w0, w24
	blr	x2
	mov	w1, 655360
	and	w0, w0, w1
	cmp	w0, w1
	beq	.L375
	cmp	w0, 524288
	bne	.L374
	mov	w0, 2
	mov	w24, 0
	str	w0, [x25, 44]
	b	.L371
	.p2align 3
.L365:
	ldr	x1, [x22, 24]
	blr	x1
	ldr	x23, .LC133
	ldr	x2, [x23, 216]
	cbz	x2, .L370
	mov	w1, w20
	mov	w0, 2
	blr	x2
	mov	w1, w0
	tbnz	x1, 0, .L372
	b	.L370
	.p2align 3
.L425:
	ldr	x0, [x22]
	blr	x0
	ldr	w4, [x25, x23]
	ldr	w1, [x26, 4]
	add	w2, w4, 49152
	add	w2, w2, 848
	cmp	w2, w1
	bcc	.L387
	ldr	w3, [x26, 8]
.L388:
	sub	w1, w0, w1
	lsl	x2, x19, 2
	add	w6, w1, w3
	sub	x3, x2, x19
	lsl	x2, x3, 4
	sub	w0, w0, w4
	sub	x2, x2, x3
	add	x1, x25, x3, lsl 5
	cmp	w0, 999
	add	x2, x21, x2, lsl 2
	stp	wzr, w6, [x1, 4]
	ldr	w5, [x2, 2248]
	ldr	w2, [x1, 64]
	add	w5, w2, w5, lsr 10
	ldr	w2, [x1, 60]
	add	w2, w2, 1
	stp	w2, w5, [x1, 60]
	bls	.L386
	add	x3, x21, x19, lsl 6
	ldr	w3, [x3, 16]
	cbnz	w3, .L390
	ldr	w3, [x1, 12]
.L391:
	lsl	w1, w2, 6
	lsl	w4, w3, 6
	sub	w1, w1, w2
	sub	w4, w4, w3
	lsl	w1, w1, 1
	lsl	w4, w4, 1
	sub	w2, w1, w2
	lsl	x1, x19, 2
	sub	w4, w4, w3
	sub	x1, x1, x19
	lsl	w3, w6, 6
	lsl	x1, x1, 5
	sub	w3, w3, w6
	lsl	w4, w4, 3
	lsl	w3, w3, 1
	lsl	w2, w2, 3
	sub	w3, w3, w6
	str	wzr, [x25, x1]
	lsl	w3, w3, 3
	add	x25, x25, x1
	udiv	w4, w4, w0
	stp	wzr, wzr, [x25, 60]
	stp	w5, w0, [x25, 68]
	udiv	w3, w3, w0
	stp	w3, w4, [x25, 16]
	udiv	w2, w2, w0
	str	w2, [x25, 24]
	b	.L386
	.p2align 3
.L395:
	ldr	w0, [x23, 8]
	bl	VCTRL_GetDecParam
	cbz	x0, .L428
	mov	w1, 1
	mov	w0, w20
	bl	VDMDRV_AfterDec
	b	.L396
	.p2align 3
.L423:
	ldr	x0, [x22]
	blr	x0
	add	x1, x21, x19, lsl 6
	add	x4, x21, 2472
	ldr	x2, .LC135
	ldr	w3, [x1, 8]
	ldr	w2, [x2]
	ldr	w1, [x4, w3, sxtw 2]
	sub	w1, w0, w1
	cmp	w1, w2
	bls	.L381
	ldr	x5, .LC127
	add	w2, w20, w3, lsl 24
	str	w0, [x4, w3, sxtw 2]
	add	w2, w2, 393216
	mov	w1, 1520
	add	x0, x5, 136
	bl	DBG_AddTrace
	b	.L381
	.p2align 3
.L424:
	ldrsw	x2, [x0, 8]
	str	wzr, [x21, x1]
	str	wzr, [x0, 36]
	add	x21, x21, x2, lsl 2
	str	wzr, [x21, 208]
	cbz	w24, .L398
	b	.L369
	.p2align 3
.L387:
	str	w1, [x25, x23]
	mov	w4, w1
	mov	w3, 0
	b	.L388
	.p2align 3
.L426:
	ldr	w0, [x25, 8]
	mov	w1, w20
	bl	VCTRL_VdmProcSedOnly
	str	wzr, [x21, x23]
	ldrsw	x0, [x25, 8]
	add	x21, x21, x0, lsl 2
	str	wzr, [x21, 208]
	cbz	w24, .L398
	b	.L369
	.p2align 3
.L375:
	add	x0, x21, x19, lsl 6
	mov	w1, 1
	mov	w24, w1
	str	w1, [x0, 44]
	b	.L371
	.p2align 3
.L376:
	ldr	x1, .LC134
	mov	w2, 1508
	mov	w0, 0
	bl	dprint_vfmw
	b	.L377
	.p2align 3
.L378:
	ldr	x1, .LC134
	mov	w2, 1510
	mov	w0, 0
	bl	dprint_vfmw
	b	.L379
	.p2align 3
.L374:
	cmp	w0, 131072
	bne	.L372
	ldr	w0, [x25, 44]
	cmp	w0, 2
	bne	.L375
	add	x0, x21, x19, lsl 6
	mov	w1, 3
	mov	w24, 1
	str	w1, [x0, 44]
	b	.L371
	.p2align 3
.L390:
	udiv	w3, w5, w3
	str	w3, [x1, 12]
	b	.L391
	.p2align 3
.L428:
	ldr	x1, .LC142
	mov	w2, 1622
	ldr	w3, [x23, 8]
	b	.L420
.L427:
	ldr	x1, .LC138
	bl	dprint_vfmw
	ldr	w0, [x25, 8]
	bl	VCTRL_GetDecParam
	cbz	x0, .L394
	ldr	w0, [x25, 8]
	mov	w4, w20
	ldr	w2, [x29, 92]
	mov	x3, 0
	mov	w1, 100
	bl	VCTRL_VdmPostProc
.L394:
	lsl	x19, x19, 6
	add	x0, x21, x19
	str	wzr, [x21, x19]
	ldrsw	x0, [x0, 8]
	add	x21, x21, x0, lsl 2
	str	wzr, [x21, 208]
	cbz	w24, .L398
	b	.L369
	.size	VDMDRV_VdmIntServProc, .-VDMDRV_VdmIntServProc
	.align	3
.LC127:
	.xword	.LANCHOR1
	.align	3
.LC129:
	.xword	.LC128
	.align	3
.LC130:
	.xword	s_pstVfmwChan
	.align	3
.LC131:
	.xword	.LANCHOR0
	.align	3
.LC132:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC133:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC134:
	.xword	.LC19
	.align	3
.LC135:
	.xword	g_TraceFramePeriod
	.align	3
.LC136:
	.xword	g_VfmwGlobalStat
	.align	3
.LC138:
	.xword	.LC137
	.align	3
.LC140:
	.xword	.LC139
	.align	3
.LC142:
	.xword	.LC141
	.align	3
.LC143:
	.xword	g_HalDisable
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_IsVp9RptFrmCopy
	.type	VDMDRV_IsVp9RptFrmCopy, %function
VDMDRV_IsVp9RptFrmCopy:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L433
	cmp	w20, 17
	mov	w0, 0
	beq	.L434
.L431:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L434:
	add	x19, x19, 262144
	ldrb	w0, [x19, 1028]
	ldp	x19, x20, [sp, 16]
	cmp	w0, 1
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L433:
	ldr	x1, .LC145
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L431
	.size	VDMDRV_IsVp9RptFrmCopy, .-VDMDRV_IsVp9RptFrmCopy
	.align	3
.LC145:
	.xword	.LC106
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_ResetVDH
	.type	VDMDRV_ResetVDH, %function
VDMDRV_ResetVDH:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC146
	ldr	w0, [x19, 8]
	cmp	w20, w0
	beq	.L440
.L435:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L440:
	ldr	x0, .LC147
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L437
	mov	w1, 0
	mov	w0, 1
	bl	VDH_Reset_Module
	str	wzr, [x19]
	b	.L435
	.p2align 3
.L437:
	ldr	x0, .LC148
	ldr	x1, [x0, 160]
	cbz	x1, .L439
	mov	w0, 0
	blr	x1
	str	wzr, [x19]
	b	.L435
.L439:
	ldr	x1, .LC149
	mov	w2, 1965
	mov	w0, 0
	bl	dprint_vfmw
	str	wzr, [x19]
	b	.L435
	.size	VDMDRV_ResetVDH, .-VDMDRV_ResetVDH
	.align	3
.LC146:
	.xword	.LANCHOR0
	.align	3
.LC147:
	.xword	g_HalDisable
	.align	3
.LC148:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC149:
	.xword	.LC19
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_MarkReloadDsp
	.type	VDMDRV_MarkReloadDsp, %function
VDMDRV_MarkReloadDsp:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cbnz	w20, .L441
	add	w0, w19, 1
	cmp	w0, 16
	bhi	.L441
	ldr	x0, .LC150
	mov	w1, 1
	cmn	w19, #1
	str	w1, [x0, 40]
	beq	.L441
	ldr	x5, .LC151
	mov	w7, -1
	ldr	x2, .LC152
	mov	w4, 0
	ldr	x1, .LC154
	mov	w3, 1995
	ldr	x6, [x5, w19, sxtw 3]
	add	x2, x2, 160
	mov	w0, 22
	str	w7, [x6, 36]
	ldr	x5, [x5]
	ldr	w5, [x5, 36]
	bl	dprint_vfmw
.L441:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_MarkReloadDsp, .-VDMDRV_MarkReloadDsp
	.align	3
.LC150:
	.xword	.LANCHOR0
	.align	3
.LC151:
	.xword	s_pstVfmwChan
	.align	3
.LC152:
	.xword	.LANCHOR1
	.align	3
.LC154:
	.xword	.LC153
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMDRV_UpdateMatchMode
	.type	VDMDRV_UpdateMatchMode, %function
VDMDRV_UpdateMatchMode:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cbnz	w20, .L448
	cmp	w19, 15
	bls	.L450
.L448:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L450:
	ldr	x4, .LC155
	mov	w7, 1
	ldr	x2, .LC156
	mov	w5, w19
	ldr	x1, .LC158
	mov	w3, 2008
	ldr	x6, [x4, w19, sxtw 3]
	add	x2, x2, 184
	mov	w0, 22
	str	w7, [x6, 36]
	ldr	x4, [x4, w19, sxtw 3]
	ldr	w4, [x4, 36]
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMDRV_UpdateMatchMode, .-VDMDRV_UpdateMatchMode
	.align	3
.LC155:
	.xword	s_pstVfmwChan
	.align	3
.LC156:
	.xword	.LANCHOR1
	.align	3
.LC158:
	.xword	.LC157
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
	.align	3
.LANCHOR1 = . + 0
	.type	__func__.11669, %object
	.size	__func__.11669, 23
__func__.11669:
	.string	"VDMDRV_CheckAttachMode"
	.zero	1
	.type	__func__.11674, %object
	.size	__func__.11674, 19
__func__.11674:
	.string	"VDMDRV_ChnDecReady"
	.zero	5
	.type	__FUNCTION__.11692, %object
	.size	__FUNCTION__.11692, 19
__FUNCTION__.11692:
	.string	"VDMDRV_DoBeforeDec"
	.zero	5
	.type	__FUNCTION__.11727, %object
	.size	__FUNCTION__.11727, 16
__FUNCTION__.11727:
	.string	"VDMDRV_AfterDec"
	.type	__FUNCTION__.11737, %object
	.size	__FUNCTION__.11737, 19
__FUNCTION__.11737:
	.string	"VDMDRV_CheckDevice"
	.zero	5
	.type	__func__.11753, %object
	.size	__func__.11753, 22
__func__.11753:
	.string	"VDMDRV_VdmIntServProc"
	.zero	2
	.type	__FUNCTION__.11755, %object
	.size	__FUNCTION__.11755, 22
__FUNCTION__.11755:
	.string	"VDMDRV_VdmIntServProc"
	.zero	2
	.type	__func__.11836, %object
	.size	__func__.11836, 21
__func__.11836:
	.string	"VDMDRV_MarkReloadDsp"
	.zero	3
	.type	__func__.11841, %object
	.size	__func__.11841, 23
__func__.11841:
	.string	"VDMDRV_UpdateMatchMode"
	.data
	.align	3
	.type	USE_FF_APT_EN, %object
	.size	USE_FF_APT_EN, 4
USE_FF_APT_EN:
	.word	1
	.zero	4
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
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC15:
	ASCII(.string	"====== forece VDM sleep! ======\n" )
	.zero	7
.LC19:
	ASCII(.string	"VDMHAL_NULL_FUN_PRINT,L%d\n" )
	.zero	5
.LC34:
	ASCII(.string	"%s,%d,  frameRate = %d,last = %d\n" )
	.zero	6
.LC36:
	ASCII(.string	"%s,%d, the frameRate change!\n" )
	.zero	2
.LC38:
	ASCII(.string	"%s,%d,chanid = %d,s32Is4k60fpsb = %d\n" )
	.zero	2
.LC43:
	ASCII(.string	"%s The channel %d is not active\n" )
	.zero	7
.LC46:
	ASCII(.string	"The channel %d is decoding\n" )
	.zero	4
.LC48:
	ASCII(.string	"The channel %d DEC PARAM is not ready\n" )
	.zero	1
.LC52:
	ASCII(.string	"SMMU_READ_ERR  MODULE : VFMW \t SMMU INT STATE: 0x%x \t read_err_addr  = 0x%x\n" )
	.zero	3
.LC54:
	ASCII(.string	"SMMU_WRITE_ERR MODULE : VFMW \t SMMU INT STATE: 0x%x \t write_err_addr = 0x%x\n" )
	.zero	3
.LC64:
	ASCII(.string	"VDMHAL_V200_IsMpeg4NvopCopy point of picture para null, wait forever!!!\n" )
	.zero	7
.LC66:
	ASCII(.string	"VDMDRV_IsVc1SkpicCopy point of picture para null, wait forever!!!\n" )
	.zero	5
.LC68:
	ASCII(.string	"VDMDRV_IsVp6NvopCopy point of picture para null, wait forever!!!\n" )
	.zero	6
.LC72:
	ASCII(.string	"The channel %d DEC PARAM is not ready(pDecParam == NULL)\n" )
	.zero	6
.LC81:
	ASCII(.string	"---SKIP REPAIR!---\n" )
	.zero	4
.LC84:
	ASCII(.string	"VDM Start Fail 2 !\n" )
	.zero	4
.LC94:
	ASCII(.string	"isr begin: *pVdmStateMachine = %d, timeout = %d, ErrRatio = %d\n" )
.LC96:
	ASCII(.string	"ISR process vdm timeout! FromInt = %d\n" )
	.zero	1
.LC98:
	ASCII(.string	"Chan %d: ErrRatio = %d\n" )
.LC106:
	ASCII(.string	"VDMDRV_IsVp9RptFrmCopy point of picture para null, wait forever!!!\n" )
	.zero	4
.LC108:
	ASCII(.string	"Chan %d VdmStateMachine=%d Error! FromInt = %d.\n" )
	.zero	7
.LC116:
	ASCII(.string	"Chan %d Mfde Running VdmStateMachine = %d, but DecParam = NULL!\n" )
	.zero	7
.LC118:
	ASCII(.string	"Chan %d VdmStateMachine = %d, but DecParam = NULL, Reset VDH MFDE!\n" )
	.zero	4
.LC122:
	ASCII(.string	"Chan %d VDM Time Out(%d ms), Reset VDM!\n" )
	.zero	7
.LC128:
	ASCII(.string	"%s: VdhId(%d) > %d\n" )
	.zero	4
.LC137:
	ASCII(.string	"vdm state machine = %d, but DecParam is null!\n" )
	.zero	1
.LC139:
	ASCII(.string	"line :%d ISR called but chan %d is not active!\n" )
.LC141:
	ASCII(.string	"lline: %d ISR called but chan %d param = NULL!\n" )
.LC153:
	ASCII(.string	"%s,%d, i = %d, MatchVdhIdPlus1 = %d\n" )
	.zero	3
.LC157:
	ASCII(.string	"%s,%d, MatchVdhIdPlus1 = %d,chanId = %d\n" )
	.bss
	.align	3
.LANCHOR0 = . + 0
	.type	g_VdmDrvParam, %object
	.size	g_VdmDrvParam, 64
g_VdmDrvParam:
	.zero	64
	.type	s_eVdmDrvSleepState, %object
	.size	s_eVdmDrvSleepState, 4
s_eVdmDrvSleepState:
	.zero	4
	.zero	4
	.type	g_DspDrvState, %object
	.size	g_DspDrvState, 8
g_DspDrvState:
	.zero	8
	.type	g_ChanCtx, %object
	.size	g_ChanCtx, 192
g_ChanCtx:
	.zero	192
	.type	g_LastFPS.11666, %object
	.size	g_LastFPS.11666, 64
g_LastFPS.11666:
	.zero	64
	.type	DiffFrameCnt.11665, %object
	.size	DiffFrameCnt.11665, 64
DiffFrameCnt.11665:
	.zero	64
	.type	s_VdmIsInISR, %object
	.size	s_VdmIsInISR, 4
s_VdmIsInISR:
	.zero	4
	.zero	4
	.type	g_RepairParam, %object
	.size	g_RepairParam, 1752
g_RepairParam:
	.zero	1752
	.type	last_rec_pos_time.11688, %object
	.size	last_rec_pos_time.11688, 64
last_rec_pos_time.11688:
	.zero	64
	.type	g_BackUp, %object
	.size	g_BackUp, 180
g_BackUp:
	.zero	180
	.zero	4
	.type	last_rec_pos_time.11719, %object
	.size	last_rec_pos_time.11719, 64
last_rec_pos_time.11719:
	.zero	64
	.type	last_rec_pos_time.11748, %object
	.size	last_rec_pos_time.11748, 64
last_rec_pos_time.11748:
	.zero	64
	.type	cycle.11747, %object
	.size	cycle.11747, 8
cycle.11747:
	.zero	8
	.type	g_VdmExtParam, %object
	.size	g_VdmExtParam, 140
g_VdmExtParam:
	.zero	140
	.zero	4
	.type	g_DecReport, %object
	.size	g_DecReport, 816
g_DecReport:
	.zero	816
	.type	g_HwMem, %object
	.size	g_HwMem, 1256
g_HwMem:
	.zero	1256
	.type	g_UpMsg, %object
	.size	g_UpMsg, 3200
g_UpMsg:
	.zero	3200
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
