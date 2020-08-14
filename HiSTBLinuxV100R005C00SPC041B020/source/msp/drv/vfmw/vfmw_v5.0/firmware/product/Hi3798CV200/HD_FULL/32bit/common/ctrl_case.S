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
	.file	"ctrl_case.c"
	.text
	.align	2
	.global	CASE_GetCapability
	.type	CASE_GetCapability, %function
CASE_GetCapability:
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
	subs	r5, r0, #0
	beq	.L5
	ldr	r4, .L6
	mov	r0, #0
	ldr	r3, [r4, #12]
	blx	r3
	mov	r0, r5
	bl	PDT_GetVdecCapability
	ldr	r3, [r4, #16]
	mov	r0, #0
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L5:
	mov	r3, #61
	ldr	r2, .L6+4
	ldr	r1, .L6+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L7:
	.align	2
.L6:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_GetCapability, .-CASE_GetCapability
	.align	2
	.global	CASE_GetChanMemSize
	.type	CASE_GetChanMemSize, %function
CASE_GetChanMemSize:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #28)
	sub	sp, sp, #28
	subs	r5, r0, #0
	beq	.L13
	ldr	r6, .L15
	mov	r0, #0
	ldr	r3, [r6, #12]
	blx	r3
	sub	r2, fp, #40
	sub	r1, fp, #52
	ldr	r0, [r5]
	bl	VCTRL_GetChanMemSize
	cmp	r0, #0
	mvnne	r4, #0
	beq	.L14
.L11:
	ldr	r3, [r6, #16]
	mov	r0, #0
	blx	r3
	mov	r0, r4
.L10:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L14:
	ldr	r3, [fp, #-52]
	mov	r4, r0
	ldr	r1, [fp, #-40]
	ldr	r2, [fp, #-48]
	add	r1, r3, r1
	ldr	ip, [fp, #-36]
	ldr	r3, [fp, #-44]
	ldr	r0, [fp, #-32]
	add	r2, r2, ip
	stmia	r5, {r1, r2}
	add	r3, r3, r0
	str	r3, [r5, #8]
	b	.L11
.L13:
	mov	r3, #79
	ldr	r2, .L15+4
	ldr	r1, .L15+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L10
.L16:
	.align	2
.L15:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+20
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_GetChanMemSize, .-CASE_GetChanMemSize
	.align	2
	.global	CASE_GetChanMemSizeWithOption
	.type	CASE_GetChanMemSizeWithOption, %function
CASE_GetChanMemSizeWithOption:
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
	subs	r5, r0, #0
	beq	.L22
	ldr	r6, .L23
	mov	r0, #0
	ldr	r3, [r6, #12]
	blx	r3
	mov	r3, #1
	sub	r2, fp, #44
	ldr	r1, [r5, #8]
	ldr	r0, [r5]
	bl	VCTRL_GetChanMemSizeWithOption
	cmp	r0, #0
	ldreq	r3, [fp, #-36]
	moveq	r4, r0
	ldreq	r1, [fp, #-44]
	mvnne	r4, #0
	ldreq	r2, [fp, #-40]
	mov	r0, #0
	stmeqia	r5, {r1, r2, r3}
	ldr	r3, [r6, #16]
	blx	r3
	mov	r0, r4
.L19:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L22:
	mov	r3, #110
	ldr	r2, .L23+4
	ldr	r1, .L23+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L19
.L24:
	.align	2
.L23:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+40
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_GetChanMemSizeWithOption, .-CASE_GetChanMemSizeWithOption
	.align	2
	.global	CASE_CreateChan
	.type	CASE_CreateChan, %function
CASE_CreateChan:
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
	subs	r4, r0, #0
	beq	.L28
	add	r1, r4, #4
	ldr	r0, [r4]
	bl	VCTRL_CreateChan
	mov	r3, r0
	mov	r0, r0, asr #31
	str	r3, [r4]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L28:
	mov	r3, #145
	ldr	r2, .L29
	ldr	r1, .L29+4
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L30:
	.align	2
.L29:
	.word	.LANCHOR0+72
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_CreateChan, .-CASE_CreateChan
	.align	2
	.global	CASE_CreateChanWithOption
	.type	CASE_CreateChanWithOption, %function
CASE_CreateChanWithOption:
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
	subs	r4, r0, #0
	beq	.L34
	mov	r3, #0
	mov	r2, #2
	ldr	r1, [r4, #8]
	ldr	r0, [r4]
	bl	VCTRL_CreateChanWithOption
	mov	r3, r0
	mov	r0, r0, asr #31
	str	r3, [r4]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L34:
	mov	r3, #166
	ldr	r2, .L35
	ldr	r1, .L35+4
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L36:
	.align	2
.L35:
	.word	.LANCHOR0+88
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_CreateChanWithOption, .-CASE_CreateChanWithOption
	.align	2
	.global	CASE_DestroyChan
	.type	CASE_DestroyChan, %function
CASE_DestroyChan:
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
	mov	r4, #500
	mov	r5, r0
	ldr	r6, .L43
	b	.L38
.L39:
	ldr	r3, [r6, #116]
	sub	r4, r4, #1
	blx	r3
.L38:
	mov	r1, #2
	mov	r0, r5
	bl	VCTRT_SetChanCtrlState
	adds	r3, r4, #0
	movne	r3, #1
	cmn	r0, #1
	mov	r0, #1
	movne	r3, #0
	cmp	r3, #0
	bne	.L39
	cmp	r4, #0
	ble	.L42
	mov	r0, r5
	bl	VCTRL_DestroyChan
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L42:
	mov	r0, r3
	mvn	r2, #0
	ldr	r1, .L43+4
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L44:
	.align	2
.L43:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC1
	UNWIND(.fnend)
	.size	CASE_DestroyChan, .-CASE_DestroyChan
	.align	2
	.global	CASE_DestroyChanWithOption
	.type	CASE_DestroyChanWithOption, %function
CASE_DestroyChanWithOption:
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
	mov	r4, #500
	mov	r5, r0
	ldr	r6, .L51
	b	.L46
.L47:
	ldr	r3, [r6, #116]
	sub	r4, r4, #1
	blx	r3
.L46:
	mov	r1, #2
	mov	r0, r5
	bl	VCTRT_SetChanCtrlState
	adds	r3, r4, #0
	movne	r3, #1
	cmn	r0, #1
	mov	r0, #1
	movne	r3, #0
	cmp	r3, #0
	bne	.L47
	cmp	r4, #0
	ble	.L50
	mov	r0, r5
	bl	VCTRL_DestroyChanWithOption
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L50:
	mov	r0, r3
	mvn	r2, #0
	ldr	r1, .L51+4
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L52:
	.align	2
.L51:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC1
	UNWIND(.fnend)
	.size	CASE_DestroyChanWithOption, .-CASE_DestroyChanWithOption
	.align	2
	.global	CASE_ResetChan
	.type	CASE_ResetChan, %function
CASE_ResetChan:
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
	ldr	r5, .L54
	mov	r4, r0
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	mov	r0, r4
	bl	VCTRL_ResetChan
	ldr	r3, [r5, #16]
	mov	r4, r0
	mov	r0, #0
	blx	r3
	adds	r0, r4, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L55:
	.align	2
.L54:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	CASE_ResetChan, .-CASE_ResetChan
	.align	2
	.global	CASE_ResetChanWithOption
	.type	CASE_ResetChanWithOption, %function
CASE_ResetChanWithOption:
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
	subs	r5, r1, #0
	mov	r6, r0
	beq	.L59
	ldr	r4, .L60
	mov	r0, #0
	ldr	r3, [r4, #12]
	blx	r3
	mov	r1, r5
	mov	r0, r6
	bl	VCTRL_ResetChanWithOption
	ldr	r3, [r4, #16]
	adds	r4, r0, #0
	mov	r0, #0
	movne	r4, #1
	rsb	r4, r4, #0
	blx	r3
.L58:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L59:
	mov	r0, r5
	mov	r3, #251
	ldr	r2, .L60+4
	mvn	r4, #0
	ldr	r1, .L60+8
	bl	dprint_vfmw
	b	.L58
.L61:
	.align	2
.L60:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+116
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_ResetChanWithOption, .-CASE_ResetChanWithOption
	.align	2
	.global	CASE_StartChan
	.type	CASE_StartChan, %function
CASE_StartChan:
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
	ldr	r5, .L63
	mov	r4, r0
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	mov	r0, r4
	bl	VCTRL_EnableEsBuf
	mov	r0, r4
	bl	VCTRL_StartChan
	ldr	r3, [r5, #16]
	mov	r4, r0
	mov	r0, #0
	blx	r3
	adds	r0, r4, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L64:
	.align	2
.L63:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	CASE_StartChan, .-CASE_StartChan
	.align	2
	.global	CASE_StopChan
	.type	CASE_StopChan, %function
CASE_StopChan:
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
	bl	VCTRL_StopChanWithCheck
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	CASE_StopChan, .-CASE_StopChan
	.align	2
	.global	CASE_GetChanConfig
	.type	CASE_GetChanConfig, %function
CASE_GetChanConfig:
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
	subs	r5, r1, #0
	mov	r6, r0
	beq	.L69
	ldr	r4, .L70
	mov	r0, #0
	ldr	r3, [r4, #12]
	blx	r3
	mov	r1, r5
	mov	r0, r6
	bl	VCTRL_GetChanCfg
	ldr	r3, [r4, #16]
	adds	r4, r0, #0
	mov	r0, #0
	movne	r4, #1
	rsb	r4, r4, #0
	blx	r3
.L68:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L69:
	mov	r0, r5
	mov	r3, #304
	ldr	r2, .L70+4
	mvn	r4, #0
	ldr	r1, .L70+8
	bl	dprint_vfmw
	b	.L68
.L71:
	.align	2
.L70:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+144
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_GetChanConfig, .-CASE_GetChanConfig
	.align	2
	.global	CASE_ConfigChan
	.type	CASE_ConfigChan, %function
CASE_ConfigChan:
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
	subs	r6, r1, #0
	mov	r5, r0
	beq	.L76
	ldr	r4, .L78
	mov	r0, #0
	ldr	r3, [r4, #12]
	blx	r3
	mov	r1, r6
	mov	r0, r5
	bl	VCTRL_ConfigChan
	cmp	r0, #0
	beq	.L77
.L75:
	adds	r3, r0, #0
	ldr	r2, [r4, #16]
	mov	r0, #0
	movne	r3, #1
	rsb	r4, r3, #0
	blx	r2
.L74:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L77:
	mov	r0, r5
	bl	VCTRL_LoadDspCode
	b	.L75
.L76:
	mov	r0, r6
	mov	r3, #320
	ldr	r2, .L78+4
	mvn	r4, #0
	ldr	r1, .L78+8
	bl	dprint_vfmw
	b	.L74
.L79:
	.align	2
.L78:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+164
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_ConfigChan, .-CASE_ConfigChan
	.align	2
	.global	CASE_GetChanState
	.type	CASE_GetChanState, %function
CASE_GetChanState:
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
	ldr	r4, .L81
	mov	r5, r0
	mov	r6, r1
	mov	r0, #0
	ldr	r3, [r4, #12]
	blx	r3
	mov	r1, r6
	mov	r0, r5
	bl	VCTRL_GetChanState
	ldr	r3, [r4, #16]
	mov	r0, #0
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L82:
	.align	2
.L81:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	CASE_GetChanState, .-CASE_GetChanState
	.align	2
	.global	CASE_ReleaseStream
	.type	CASE_ReleaseStream, %function
CASE_ReleaseStream:
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
	ldr	r5, .L84
	mov	r4, r0
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	mov	r0, r4
	bl	VCTRL_ReleaseStream
	ldr	r3, [r5, #16]
	mov	r4, r0
	mov	r0, #0
	blx	r3
	adds	r0, r4, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L85:
	.align	2
.L84:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	CASE_ReleaseStream, .-CASE_ReleaseStream
	.align	2
	.global	CASE_SetStreamIntf
	.type	CASE_SetStreamIntf, %function
CASE_SetStreamIntf:
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
	subs	r5, r1, #0
	mov	r6, r0
	beq	.L89
	ldr	r4, .L90
	mov	r0, #0
	ldr	r3, [r4, #12]
	blx	r3
	mov	r1, r5
	mov	r0, r6
	bl	VCTRL_SetStreamInterface
	ldr	r3, [r4, #16]
	adds	r4, r0, #0
	mov	r0, #0
	movne	r4, #1
	rsb	r4, r4, #0
	blx	r3
.L88:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L89:
	mov	r0, r5
	movw	r3, #369
	ldr	r2, .L90+4
	mvn	r4, #0
	ldr	r1, .L90+8
	bl	dprint_vfmw
	b	.L88
.L91:
	.align	2
.L90:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+180
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_SetStreamIntf, .-CASE_SetStreamIntf
	.align	2
	.global	CASE_GetStreamSize
	.type	CASE_GetStreamSize, %function
CASE_GetStreamSize:
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
	subs	r4, r1, #0
	mov	r6, r0
	beq	.L95
	ldr	r5, .L96
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	mov	r1, r4
	mov	r0, r6
	bl	VCTRL_GetStreamSize
	ldr	r3, [r5, #16]
	mov	r4, r0
	mov	r0, #0
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L95:
	mov	r0, r4
	movw	r3, #385
	ldr	r2, .L96+4
	ldr	r1, .L96+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L97:
	.align	2
.L96:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+200
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_GetStreamSize, .-CASE_GetStreamSize
	.align	2
	.global	CASE_ExtraWakupThread
	.type	CASE_ExtraWakupThread, %function
CASE_ExtraWakupThread:
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
	ldr	r4, .L99
	mov	r0, #0
	ldr	r3, [r4, #12]
	blx	r3
	bl	VCTRL_ExtraWakeUpThread
	ldr	r3, [r4, #16]
	mov	r0, #0
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L100:
	.align	2
.L99:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	CASE_ExtraWakupThread, .-CASE_ExtraWakupThread
	.align	2
	.global	CASE_GetChanIDByMem
	.type	CASE_GetChanIDByMem, %function
CASE_GetChanIDByMem:
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
	beq	.L104
	ldr	r5, .L105
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	ldr	r0, [r4]
	bl	VCTRL_GetChanIDByMemAddr
	mov	r3, r0
	cmn	r3, #1
	str	r3, [r4]
	mov	r0, #0
	ldr	r2, [r5, #16]
	movne	r3, #0
	moveq	r3, #1
	rsb	r4, r3, #0
	blx	r2
.L103:
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L104:
	mov	r0, r4
	mov	r3, #412
	ldr	r2, .L105+4
	mvn	r4, #0
	ldr	r1, .L105+8
	bl	dprint_vfmw
	b	.L103
.L106:
	.align	2
.L105:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+220
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_GetChanIDByMem, .-CASE_GetChanIDByMem
	.align	2
	.global	CASE_SetDiscardPicParam
	.type	CASE_SetDiscardPicParam, %function
CASE_SetDiscardPicParam:
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
	subs	r4, r1, #0
	mov	r6, r0
	beq	.L110
	ldr	r5, .L111
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	mov	r1, r4
	mov	r0, r6
	bl	VCTRL_SetDiscardPicParam
	ldr	r3, [r5, #16]
	mov	r4, r0
	mov	r0, #0
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L110:
	mov	r0, r4
	movw	r3, #429
	ldr	r2, .L111+4
	ldr	r1, .L111+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L112:
	.align	2
.L111:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+240
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_SetDiscardPicParam, .-CASE_SetDiscardPicParam
	.align	2
	.global	CASE_SetPtsToSeek
	.type	CASE_SetPtsToSeek, %function
CASE_SetPtsToSeek:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #16)
	sub	sp, sp, #16
	subs	r7, r1, #0
	mov	r6, r0
	beq	.L117
	ldr	r5, .L119
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	sub	r1, fp, #40
	mov	r0, r6
	mov	r3, #0
	mov	r2, #1
	str	r3, [fp, #-36]
	str	r2, [fp, #-40]
	str	r3, [fp, #-32]
	bl	VCTRL_ResetChanWithOption
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r4, r0, #0
	cmp	r4, #0
	beq	.L118
.L116:
	ldr	r3, [r5, #16]
	mov	r0, #0
	blx	r3
	mov	r0, r4
.L115:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L118:
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_SeekPts
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r4, r0, #0
	b	.L116
.L117:
	mov	r0, r7
	movw	r3, #446
	ldr	r2, .L119+4
	ldr	r1, .L119+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L115
.L120:
	.align	2
.L119:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+264
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_SetPtsToSeek, .-CASE_SetPtsToSeek
	.align	2
	.global	CASE_SetTrickMode
	.type	CASE_SetTrickMode, %function
CASE_SetTrickMode:
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
	subs	r5, r1, #0
	mov	r6, r0
	beq	.L124
	ldr	r4, .L125
	mov	r0, #0
	ldr	r3, [r4, #12]
	blx	r3
	mov	r1, r5
	mov	r0, r6
	bl	VCTRL_ConfigFFFBSpeed
	ldr	r3, [r4, #16]
	adds	r4, r0, #0
	mov	r0, #0
	movne	r4, #1
	rsb	r4, r4, #0
	blx	r3
.L123:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L124:
	mov	r0, r5
	mov	r3, #472
	ldr	r2, .L125+4
	mvn	r4, #0
	ldr	r1, .L125+8
	bl	dprint_vfmw
	b	.L123
.L126:
	.align	2
.L125:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+284
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_SetTrickMode, .-CASE_SetTrickMode
	.align	2
	.global	CASE_SetCtrlInfo
	.type	CASE_SetCtrlInfo, %function
CASE_SetCtrlInfo:
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
	subs	r5, r1, #0
	mov	r6, r0
	beq	.L130
	ldr	r4, .L131
	mov	r0, #0
	ldr	r3, [r4, #12]
	blx	r3
	mov	r1, r5
	mov	r0, r6
	bl	VCTRL_ConfigPVRInfo
	ldr	r3, [r4, #16]
	adds	r4, r0, #0
	mov	r0, #0
	movne	r4, #1
	rsb	r4, r4, #0
	blx	r3
.L129:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L130:
	mov	r0, r5
	mov	r3, #488
	ldr	r2, .L131+4
	mvn	r4, #0
	ldr	r1, .L131+8
	bl	dprint_vfmw
	b	.L129
.L132:
	.align	2
.L131:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+304
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_SetCtrlInfo, .-CASE_SetCtrlInfo
	.align	2
	.global	CASE_SetFrameRate
	.type	CASE_SetFrameRate, %function
CASE_SetFrameRate:
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
	beq	.L136
	ldr	r2, .L137
	mov	r3, #0
	ldr	r1, [r1]
	ldr	r2, [r2, r0, asl #2]
	str	r1, [r2, #1512]
.L135:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L136:
	mov	r0, r1
	movw	r3, #502
	ldr	r2, .L137+4
	ldr	r1, .L137+8
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L135
.L138:
	.align	2
.L137:
	.word	s_pstVfmwChan
	.word	.LANCHOR0+324
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_SetFrameRate, .-CASE_SetFrameRate
	.align	2
	.global	CASE_AllocMemToDecoder
	.type	CASE_AllocMemToDecoder, %function
CASE_AllocMemToDecoder:
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
	subs	r4, r1, #0
	mov	r6, r0
	beq	.L146
	ldr	r5, .L148
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	mov	r1, r4
	mov	r0, r6
	bl	VCTRL_AllocMemToDecoder
	subs	r4, r0, #0
	bne	.L147
.L142:
	ldr	r3, [r5, #16]
	mov	r0, #0
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L147:
	ldr	r1, .L148+4
	mov	r0, #1
	bl	dprint_vfmw
	b	.L142
.L146:
	mov	r0, r4
	movw	r3, #513
	ldr	r2, .L148+8
	ldr	r1, .L148+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L149:
	.align	2
.L148:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LANCHOR0+344
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_AllocMemToDecoder, .-CASE_AllocMemToDecoder
	.align	2
	.global	CASE_SetFrameBufferNum
	.type	CASE_SetFrameBufferNum, %function
CASE_SetFrameBufferNum:
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
	beq	.L154
	ldr	r3, .L155
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	addne	r2, r3, #290816
	ldrne	ip, [r1]
	movne	r0, #0
	mvneq	r0, #0
	strne	ip, [r2, #1852]
	ldrne	r2, [r1]
	strne	r2, [r3, #1448]
	ldmfd	sp, {fp, sp, pc}
.L154:
	mov	r0, r1
	mov	r3, #532
	ldr	r2, .L155+4
	ldr	r1, .L155+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L156:
	.align	2
.L155:
	.word	s_pstVfmwChan
	.word	.LANCHOR0+368
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_SetFrameBufferNum, .-CASE_SetFrameBufferNum
	.align	2
	.global	CASE_GetUserDecFrame
	.type	CASE_GetUserDecFrame, %function
CASE_GetUserDecFrame:
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
	subs	r5, r1, #0
	mov	r6, r0
	beq	.L160
	ldr	r4, .L161
	mov	r0, #0
	ldr	r3, [r4, #12]
	blx	r3
	mov	r1, r5
	mov	r0, r6
	bl	VCTRL_GetUsrdecFrame
	ldr	r3, [r4, #16]
	adds	r4, r0, #0
	mov	r0, #0
	movne	r4, #1
	rsb	r4, r4, #0
	blx	r3
.L159:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L160:
	mov	r0, r5
	movw	r3, #553
	ldr	r2, .L161+4
	mvn	r4, #0
	ldr	r1, .L161+8
	bl	dprint_vfmw
	b	.L159
.L162:
	.align	2
.L161:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+392
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_GetUserDecFrame, .-CASE_GetUserDecFrame
	.align	2
	.global	CASE_PutUserDecFrame
	.type	CASE_PutUserDecFrame, %function
CASE_PutUserDecFrame:
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
	subs	r5, r1, #0
	mov	r6, r0
	beq	.L166
	ldr	r4, .L167
	mov	r0, #0
	ldr	r3, [r4, #12]
	blx	r3
	mov	r1, r5
	mov	r0, r6
	bl	VCTRL_PutUsrdecFrame
	ldr	r3, [r4, #16]
	adds	r4, r0, #0
	mov	r0, #0
	movne	r4, #1
	rsb	r4, r4, #0
	blx	r3
.L165:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L166:
	mov	r0, r5
	movw	r3, #569
	ldr	r2, .L167+4
	mvn	r4, #0
	ldr	r1, .L167+8
	bl	dprint_vfmw
	b	.L165
.L168:
	.align	2
.L167:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+416
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_PutUserDecFrame, .-CASE_PutUserDecFrame
	.align	2
	.global	CASE_StartLowDelayCalc
	.type	CASE_StartLowDelayCalc, %function
CASE_StartLowDelayCalc:
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
	mov	r3, r0
	beq	.L172
	ldr	ip, .L173
	mov	lr, #1
	ldr	r2, .L173+4
	mov	r0, #0
	strb	lr, [ip, r3]
	ldr	r1, [r1]
	str	r1, [r2, r3, asl #2]
	ldmfd	sp, {fp, sp, pc}
.L172:
	mov	r0, r1
	mov	r3, #588
	ldr	r2, .L173+8
	ldr	r1, .L173+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L174:
	.align	2
.L173:
	.word	g_LowDelayCalcStat
	.word	g_LowDelayCalcHandle
	.word	.LANCHOR0+440
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_StartLowDelayCalc, .-CASE_StartLowDelayCalc
	.align	2
	.global	CASE_StopLowDelayCalc
	.type	CASE_StopLowDelayCalc, %function
CASE_StopLowDelayCalc:
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
	ldr	ip, .L176
	mov	r2, r0
	ldr	r1, .L176+4
	mov	r3, #0
	mov	r0, r3
	strb	r3, [ip, r2]
	str	r3, [r1, r2, asl #2]
	ldmfd	sp, {fp, sp, pc}
.L177:
	.align	2
.L176:
	.word	g_LowDelayCalcStat
	.word	g_LowDelayCalcHandle
	UNWIND(.fnend)
	.size	CASE_StopLowDelayCalc, .-CASE_StopLowDelayCalc
	.align	2
	.global	CASE_ReportOccupiedFrame
	.type	CASE_ReportOccupiedFrame, %function
CASE_ReportOccupiedFrame:
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
	subs	r3, r1, #0
	mov	r5, r0
	beq	.L185
	bl	VCTRL_ReportOccupiedFrame
	subs	r4, r0, #0
	bne	.L186
.L181:
	adds	r0, r4, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L186:
	mov	r2, r5
	ldr	r1, .L187
	mov	r0, #1
	bl	dprint_vfmw
	b	.L181
.L185:
	mov	r0, r3
	ldr	r2, .L187+4
	movw	r3, #611
	ldr	r1, .L187+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L188:
	.align	2
.L187:
	.word	.LC3
	.word	.LANCHOR0+464
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_ReportOccupiedFrame, .-CASE_ReportOccupiedFrame
	.align	2
	.global	CASE_SetImageOutputInfo
	.type	CASE_SetImageOutputInfo, %function
CASE_SetImageOutputInfo:
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
	subs	r3, r1, #0
	mov	r5, r0
	beq	.L196
	bl	VCTRL_ImageOutputEnable
	subs	r4, r0, #0
	bne	.L197
.L192:
	adds	r0, r4, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L197:
	mov	r2, r5
	ldr	r1, .L198
	mov	r0, #1
	bl	dprint_vfmw
	b	.L192
.L196:
	mov	r0, r3
	ldr	r2, .L198+4
	movw	r3, #630
	ldr	r1, .L198+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L199:
	.align	2
.L198:
	.word	.LC4
	.word	.LANCHOR0+492
	.word	.LC0
	UNWIND(.fnend)
	.size	CASE_SetImageOutputInfo, .-CASE_SetImageOutputInfo
	.align	2
	.global	CASE_ReportDemuxRelease
	.type	CASE_ReportDemuxRelease, %function
CASE_ReportDemuxRelease:
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
	ldr	r4, .L205
	mov	r5, r0
	mov	r0, #0
	ldr	r3, [r4, #12]
	blx	r3
	mov	r0, r5
	bl	VCTRL_IfEsBufEnable
	cmp	r0, #0
	beq	.L201
	mov	r0, r5
	bl	VCTRL_DisableEsBuf
.L201:
	ldr	r3, [r4, #16]
	mov	r0, #0
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L206:
	.align	2
.L205:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	CASE_ReportDemuxRelease, .-CASE_ReportDemuxRelease
	.align	2
	.global	CASE_HandleEntry
	.type	CASE_HandleEntry, %function
CASE_HandleEntry:
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
	sub	r1, r1, #1
	cmp	r1, #44
	ldrls	pc, [pc, r1, asl #2]
	b	.L245
.L210:
	.word	.L209
	.word	.L245
	.word	.L245
	.word	.L211
	.word	.L212
	.word	.L213
	.word	.L214
	.word	.L215
	.word	.L216
	.word	.L217
	.word	.L218
	.word	.L219
	.word	.L220
	.word	.L221
	.word	.L222
	.word	.L245
	.word	.L223
	.word	.L224
	.word	.L245
	.word	.L225
	.word	.L226
	.word	.L227
	.word	.L245
	.word	.L245
	.word	.L228
	.word	.L229
	.word	.L245
	.word	.L230
	.word	.L245
	.word	.L231
	.word	.L245
	.word	.L245
	.word	.L232
	.word	.L233
	.word	.L234
	.word	.L235
	.word	.L236
	.word	.L237
	.word	.L245
	.word	.L238
	.word	.L239
	.word	.L245
	.word	.L240
	.word	.L241
	.word	.L242
.L242:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_ReportOccupiedFrame
.L241:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_SetImageOutputInfo
.L240:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_ReportDemuxRelease
.L239:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_SetFrameBufferNum
.L238:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_AllocMemToDecoder
.L237:
	ldr	ip, .L248
	mov	r2, #0
	ldr	r1, .L248+4
	mov	r3, r2
	strb	r2, [ip, r0]
	str	r2, [r1, r0, asl #2]
.L208:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L236:
	cmp	r2, #0
	beq	.L246
	ldr	ip, .L248
	mov	lr, #1
	ldr	r1, .L248+4
	mov	r3, #0
	strb	lr, [ip, r0]
	ldr	r2, [r2]
	str	r2, [r1, r0, asl #2]
	b	.L208
.L235:
	cmp	r2, #0
	beq	.L247
	ldr	ip, .L248+8
	mov	r3, #0
	ldr	r1, [r2]
	ldr	r2, [ip, r0, asl #2]
	str	r1, [r2, #1512]
	b	.L208
.L234:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_SetCtrlInfo
.L233:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_SetTrickMode
.L232:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_SetPtsToSeek
.L231:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_ExtraWakupThread
.L230:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_SetDiscardPicParam
.L229:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_PutUserDecFrame
.L228:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_GetUserDecFrame
.L227:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_ResetChanWithOption
.L226:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_ReleaseStream
.L225:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_GetChanIDByMem
.L224:
	mov	r0, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_GetChanMemSizeWithOption
.L223:
	mov	r0, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_GetChanMemSize
.L222:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_GetStreamSize
.L221:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_SetStreamIntf
.L220:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_ResetChan
.L219:
	bl	VCTRL_StopChanWithCheck
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r3, r0, #0
	b	.L208
.L218:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_StartChan
.L217:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_GetChanState
.L216:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_ConfigChan
.L215:
	mov	r1, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_GetChanConfig
.L214:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_DestroyChanWithOption
.L213:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_DestroyChan
.L212:
	mov	r0, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_CreateChanWithOption
.L211:
	mov	r0, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_CreateChan
.L209:
	mov	r0, r2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CASE_GetCapability
.L245:
	mvn	r3, #0
	b	.L208
.L246:
	mov	r0, r2
	mov	r3, #588
	ldr	r2, .L248+12
	ldr	r1, .L248+16
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L208
.L247:
	mov	r0, r2
	movw	r3, #502
	ldr	r2, .L248+20
	ldr	r1, .L248+16
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L208
.L249:
	.align	2
.L248:
	.word	g_LowDelayCalcStat
	.word	g_LowDelayCalcHandle
	.word	s_pstVfmwChan
	.word	.LANCHOR0+440
	.word	.LC0
	.word	.LANCHOR0+324
	UNWIND(.fnend)
	.size	CASE_HandleEntry, .-CASE_HandleEntry
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.14157, %object
	.size	__func__.14157, 19
__func__.14157:
	.ascii	"CASE_GetCapability\000"
	.space	1
	.type	__func__.14166, %object
	.size	__func__.14166, 20
__func__.14166:
	.ascii	"CASE_GetChanMemSize\000"
	.type	__func__.14174, %object
	.size	__func__.14174, 30
__func__.14174:
	.ascii	"CASE_GetChanMemSizeWithOption\000"
	.space	2
	.type	__func__.14180, %object
	.size	__func__.14180, 16
__func__.14180:
	.ascii	"CASE_CreateChan\000"
	.type	__func__.14187, %object
	.size	__func__.14187, 26
__func__.14187:
	.ascii	"CASE_CreateChanWithOption\000"
	.space	2
	.type	__func__.14213, %object
	.size	__func__.14213, 25
__func__.14213:
	.ascii	"CASE_ResetChanWithOption\000"
	.space	3
	.type	__func__.14227, %object
	.size	__func__.14227, 19
__func__.14227:
	.ascii	"CASE_GetChanConfig\000"
	.space	1
	.type	__func__.14233, %object
	.size	__func__.14233, 16
__func__.14233:
	.ascii	"CASE_ConfigChan\000"
	.type	__func__.14247, %object
	.size	__func__.14247, 19
__func__.14247:
	.ascii	"CASE_SetStreamIntf\000"
	.space	1
	.type	__func__.14253, %object
	.size	__func__.14253, 19
__func__.14253:
	.ascii	"CASE_GetStreamSize\000"
	.space	1
	.type	__func__.14263, %object
	.size	__func__.14263, 20
__func__.14263:
	.ascii	"CASE_GetChanIDByMem\000"
	.type	__func__.14269, %object
	.size	__func__.14269, 24
__func__.14269:
	.ascii	"CASE_SetDiscardPicParam\000"
	.type	__func__.14277, %object
	.size	__func__.14277, 18
__func__.14277:
	.ascii	"CASE_SetPtsToSeek\000"
	.space	2
	.type	__func__.14283, %object
	.size	__func__.14283, 18
__func__.14283:
	.ascii	"CASE_SetTrickMode\000"
	.space	2
	.type	__func__.14289, %object
	.size	__func__.14289, 17
__func__.14289:
	.ascii	"CASE_SetCtrlInfo\000"
	.space	3
	.type	__func__.14294, %object
	.size	__func__.14294, 18
__func__.14294:
	.ascii	"CASE_SetFrameRate\000"
	.space	2
	.type	__func__.14300, %object
	.size	__func__.14300, 23
__func__.14300:
	.ascii	"CASE_AllocMemToDecoder\000"
	.space	1
	.type	__func__.14306, %object
	.size	__func__.14306, 23
__func__.14306:
	.ascii	"CASE_SetFrameBufferNum\000"
	.space	1
	.type	__func__.14312, %object
	.size	__func__.14312, 21
__func__.14312:
	.ascii	"CASE_GetUserDecFrame\000"
	.space	3
	.type	__func__.14318, %object
	.size	__func__.14318, 21
__func__.14318:
	.ascii	"CASE_PutUserDecFrame\000"
	.space	3
	.type	__func__.14325, %object
	.size	__func__.14325, 23
__func__.14325:
	.ascii	"CASE_StartLowDelayCalc\000"
	.space	1
	.type	__func__.14335, %object
	.size	__func__.14335, 25
__func__.14335:
	.ascii	"CASE_ReportOccupiedFrame\000"
	.space	3
	.type	__func__.14342, %object
	.size	__func__.14342, 24
__func__.14342:
	.ascii	"CASE_SetImageOutputInfo\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"invalid condition: %s %d\012\000" )
	.space	2
.LC1:
	ASCII(.ascii	"VCTRT_SetChanCtrlState ERR: %d\012\000" )
.LC2:
	ASCII(.ascii	"VCTRL_AllocMemToDecoder failed!\012 \000" )
	.space	2
.LC3:
	ASCII(.ascii	"VCTRL_ReportOccupiedFrame[Chan %d] failed!\012\000" )
.LC4:
	ASCII(.ascii	"VCTRL_ImageOutputEnable[Chan %d] failed!\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
