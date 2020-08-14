#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"ctrl_case.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	CASE_GetCapability
	.type	CASE_GetCapability, %function
CASE_GetCapability:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L5
	ldr	x20, .LC3
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	mov	x0, x19
	bl	PDT_GetVdecCapability
	ldr	x1, [x20, 32]
	mov	w0, 0
	blr	x1
	mov	w0, 0
.L3:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L5:
	ldr	x2, .LC0
	mov	w3, 61
	ldr	x1, .LC2
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3
	.size	CASE_GetCapability, .-CASE_GetCapability
	.align	3
.LC0:
	.xword	.LANCHOR0
	.align	3
.LC2:
	.xword	.LC1
	.align	3
.LC3:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_GetChanMemSize
	.type	CASE_GetChanMemSize, %function
CASE_GetChanMemSize:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L11
	ldr	x21, .LC6
	mov	w0, 0
	mov	w20, -1
	ldr	x1, [x21, 24]
	blr	x1
	ldr	w0, [x19]
	add	x2, x29, 64
	add	x1, x29, 48
	bl	VCTRL_GetChanMemSize
	cbz	w0, .L12
.L9:
	ldr	x1, [x21, 32]
	mov	w0, 0
	blr	x1
	mov	w0, w20
.L8:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L12:
	ldp	w5, w4, [x29, 48]
	ldp	w2, w1, [x29, 64]
	ldr	w3, [x29, 56]
	mov	w20, 0
	ldr	w0, [x29, 72]
	add	w2, w5, w2
	add	w1, w4, w1
	add	w0, w3, w0
	str	w0, [x19, 8]
	stp	w2, w1, [x19]
	b	.L9
.L11:
	ldr	x2, .LC4
	mov	w3, 79
	ldr	x1, .LC5
	mov	w0, 0
	add	x2, x2, 24
	bl	dprint_vfmw
	mov	w0, -1
	b	.L8
	.size	CASE_GetChanMemSize, .-CASE_GetChanMemSize
	.align	3
.LC4:
	.xword	.LANCHOR0
	.align	3
.LC5:
	.xword	.LC1
	.align	3
.LC6:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_GetChanMemSizeWithOption
	.type	CASE_GetChanMemSizeWithOption, %function
CASE_GetChanMemSizeWithOption:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L18
	ldr	x21, .LC9
	mov	w0, 0
	mov	w20, -1
	ldr	x1, [x21, 24]
	blr	x1
	ldr	w0, [x19]
	mov	w3, 1
	ldr	x1, [x19, 8]
	add	x2, x29, 48
	bl	VCTRL_GetChanMemSizeWithOption
	cbz	w0, .L19
.L16:
	ldr	x1, [x21, 32]
	mov	w0, 0
	blr	x1
	mov	w0, w20
.L15:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L19:
	ldp	w2, w1, [x29, 48]
	ldr	w0, [x29, 56]
	mov	w20, 0
	stp	w2, w1, [x19]
	str	w0, [x19, 8]
	b	.L16
.L18:
	ldr	x2, .LC7
	mov	w3, 110
	ldr	x1, .LC8
	mov	w0, 0
	add	x2, x2, 48
	bl	dprint_vfmw
	mov	w0, -1
	b	.L15
	.size	CASE_GetChanMemSizeWithOption, .-CASE_GetChanMemSizeWithOption
	.align	3
.LC7:
	.xword	.LANCHOR0
	.align	3
.LC8:
	.xword	.LC1
	.align	3
.LC9:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_CreateChan
	.type	CASE_CreateChan, %function
CASE_CreateChan:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L23
	ldr	w0, [x19]
	add	x1, x19, 4
	bl	VCTRL_CreateChan
	mov	w1, w0
	asr	w0, w0, 31
	str	w1, [x19]
.L22:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L23:
	ldr	x2, .LC10
	mov	w3, 145
	ldr	x1, .LC11
	mov	w0, 0
	add	x2, x2, 80
	bl	dprint_vfmw
	mov	w0, -1
	b	.L22
	.size	CASE_CreateChan, .-CASE_CreateChan
	.align	3
.LC10:
	.xword	.LANCHOR0
	.align	3
.LC11:
	.xword	.LC1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_CreateChanWithOption
	.type	CASE_CreateChanWithOption, %function
CASE_CreateChanWithOption:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L27
	ldr	x1, [x19, 8]
	mov	w3, 0
	ldr	w0, [x19]
	mov	w2, 2
	bl	VCTRL_CreateChanWithOption
	mov	w1, w0
	asr	w0, w0, 31
	str	w1, [x19]
.L26:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L27:
	ldr	x2, .LC12
	mov	w3, 166
	ldr	x1, .LC13
	mov	w0, 0
	add	x2, x2, 96
	bl	dprint_vfmw
	mov	w0, -1
	b	.L26
	.size	CASE_CreateChanWithOption, .-CASE_CreateChanWithOption
	.align	3
.LC12:
	.xword	.LANCHOR0
	.align	3
.LC13:
	.xword	.LC1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_DestroyChan
	.type	CASE_DestroyChan, %function
CASE_DestroyChan:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	w19, 500
	bl	_mcount
	ldr	x21, .LC14
	b	.L29
	.p2align 3
.L30:
	ldr	x1, [x21, 232]
	sub	w19, w19, #1
	blr	x1
.L29:
	mov	w1, 2
	mov	w0, w20
	bl	VCTRT_SetChanCtrlState
	mov	w1, w0
	cmp	w19, wzr
	mov	w0, 1
	ccmn	w1, #1, 0, ne
	beq	.L30
	cmp	w19, wzr
	ble	.L33
	mov	w0, w20
	bl	VCTRL_DestroyChan
	cmp	w0, wzr
	csetm	w0, ne
.L32:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L33:
	ldr	x1, .LC16
	mov	w2, -1
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L32
	.size	CASE_DestroyChan, .-CASE_DestroyChan
	.align	3
.LC14:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC16:
	.xword	.LC15
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_DestroyChanWithOption
	.type	CASE_DestroyChanWithOption, %function
CASE_DestroyChanWithOption:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	w19, 500
	bl	_mcount
	ldr	x21, .LC17
	b	.L35
	.p2align 3
.L36:
	ldr	x1, [x21, 232]
	sub	w19, w19, #1
	blr	x1
.L35:
	mov	w1, 2
	mov	w0, w20
	bl	VCTRT_SetChanCtrlState
	mov	w1, w0
	cmp	w19, wzr
	mov	w0, 1
	ccmn	w1, #1, 0, ne
	beq	.L36
	cmp	w19, wzr
	ble	.L39
	mov	w0, w20
	bl	VCTRL_DestroyChanWithOption
	cmp	w0, wzr
	csetm	w0, ne
.L38:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L39:
	ldr	x1, .LC18
	mov	w2, -1
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L38
	.size	CASE_DestroyChanWithOption, .-CASE_DestroyChanWithOption
	.align	3
.LC17:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC18:
	.xword	.LC15
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_ResetChan
	.type	CASE_ResetChan, %function
CASE_ResetChan:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC19
	mov	w0, 0
	ldr	x1, [x19, 24]
	blr	x1
	mov	w0, w20
	bl	VCTRL_ResetChan
	mov	w20, w0
	ldr	x1, [x19, 32]
	mov	w0, 0
	blr	x1
	cmp	w20, wzr
	csetm	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	CASE_ResetChan, .-CASE_ResetChan
	.align	3
.LC19:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_ResetChanWithOption
	.type	CASE_ResetChanWithOption, %function
CASE_ResetChanWithOption:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L44
	ldr	x20, .LC22
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	mov	x1, x19
	mov	w0, w21
	bl	VCTRL_ResetChanWithOption
	cmp	w0, wzr
	ldr	x1, [x20, 32]
	mov	w0, 0
	csetm	w19, ne
	blr	x1
.L43:
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L44:
	ldr	x2, .LC20
	mov	w3, 251
	ldr	x1, .LC21
	mov	w0, 0
	add	x2, x2, 128
	mov	w19, -1
	bl	dprint_vfmw
	b	.L43
	.size	CASE_ResetChanWithOption, .-CASE_ResetChanWithOption
	.align	3
.LC20:
	.xword	.LANCHOR0
	.align	3
.LC21:
	.xword	.LC1
	.align	3
.LC22:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_StartChan
	.type	CASE_StartChan, %function
CASE_StartChan:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC23
	mov	w0, 0
	ldr	x1, [x19, 24]
	blr	x1
	mov	w0, w20
	bl	VCTRL_EnableEsBuf
	mov	w0, w20
	bl	VCTRL_StartChan
	mov	w20, w0
	ldr	x1, [x19, 32]
	mov	w0, 0
	blr	x1
	cmp	w20, wzr
	csetm	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	CASE_StartChan, .-CASE_StartChan
	.align	3
.LC23:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_StopChan
	.type	CASE_StopChan, %function
CASE_StopChan:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	mov	w0, w19
	bl	VCTRL_StopChanWithCheck
	cmp	w0, wzr
	csetm	w0, ne
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	CASE_StopChan, .-CASE_StopChan
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_GetChanConfig
	.type	CASE_GetChanConfig, %function
CASE_GetChanConfig:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L50
	ldr	x20, .LC26
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	mov	x1, x19
	mov	w0, w21
	bl	VCTRL_GetChanCfg
	cmp	w0, wzr
	ldr	x1, [x20, 32]
	mov	w0, 0
	csetm	w19, ne
	blr	x1
.L49:
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L50:
	ldr	x2, .LC24
	mov	w3, 304
	ldr	x1, .LC25
	mov	w0, 0
	add	x2, x2, 160
	mov	w19, -1
	bl	dprint_vfmw
	b	.L49
	.size	CASE_GetChanConfig, .-CASE_GetChanConfig
	.align	3
.LC24:
	.xword	.LANCHOR0
	.align	3
.LC25:
	.xword	.LC1
	.align	3
.LC26:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_ConfigChan
	.type	CASE_ConfigChan, %function
CASE_ConfigChan:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L55
	ldr	x20, .LC29
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	mov	x1, x19
	mov	w0, w21
	bl	VCTRL_ConfigChan
	cbz	w0, .L56
.L54:
	ldr	x1, [x20, 32]
	cmp	w0, wzr
	mov	w0, 0
	csetm	w19, ne
	blr	x1
.L53:
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L56:
	mov	w0, w21
	bl	VCTRL_LoadDspCode
	b	.L54
.L55:
	ldr	x2, .LC27
	mov	w3, 320
	ldr	x1, .LC28
	mov	w0, 0
	add	x2, x2, 184
	mov	w19, -1
	bl	dprint_vfmw
	b	.L53
	.size	CASE_ConfigChan, .-CASE_ConfigChan
	.align	3
.LC27:
	.xword	.LANCHOR0
	.align	3
.LC28:
	.xword	.LC1
	.align	3
.LC29:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_GetChanState
	.type	CASE_GetChanState, %function
CASE_GetChanState:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	ldr	x19, .LC30
	mov	w0, 0
	ldr	x1, [x19, 24]
	blr	x1
	mov	x1, x21
	mov	w0, w20
	bl	VCTRL_GetChanState
	ldr	x1, [x19, 32]
	mov	w0, 0
	blr	x1
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	CASE_GetChanState, .-CASE_GetChanState
	.align	3
.LC30:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_ReleaseStream
	.type	CASE_ReleaseStream, %function
CASE_ReleaseStream:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC31
	mov	w0, 0
	ldr	x1, [x19, 24]
	blr	x1
	mov	w0, w20
	bl	VCTRL_ReleaseStream
	mov	w20, w0
	ldr	x1, [x19, 32]
	mov	w0, 0
	blr	x1
	cmp	w20, wzr
	csetm	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	CASE_ReleaseStream, .-CASE_ReleaseStream
	.align	3
.LC31:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_SetStreamIntf
	.type	CASE_SetStreamIntf, %function
CASE_SetStreamIntf:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L62
	ldr	x20, .LC34
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	mov	x1, x19
	mov	w0, w21
	bl	VCTRL_SetStreamInterface
	cmp	w0, wzr
	ldr	x1, [x20, 32]
	mov	w0, 0
	csetm	w19, ne
	blr	x1
.L61:
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L62:
	ldr	x2, .LC32
	mov	w3, 369
	ldr	x1, .LC33
	mov	w0, 0
	add	x2, x2, 200
	mov	w19, -1
	bl	dprint_vfmw
	b	.L61
	.size	CASE_SetStreamIntf, .-CASE_SetStreamIntf
	.align	3
.LC32:
	.xword	.LANCHOR0
	.align	3
.LC33:
	.xword	.LC1
	.align	3
.LC34:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_GetStreamSize
	.type	CASE_GetStreamSize, %function
CASE_GetStreamSize:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L66
	ldr	x20, .LC37
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	mov	x1, x19
	mov	w0, w21
	bl	VCTRL_GetStreamSize
	mov	w19, w0
	ldr	x1, [x20, 32]
	mov	w0, 0
	blr	x1
	mov	w0, w19
.L65:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L66:
	ldr	x2, .LC35
	mov	w3, 385
	ldr	x1, .LC36
	mov	w0, 0
	add	x2, x2, 224
	bl	dprint_vfmw
	mov	w0, -1
	b	.L65
	.size	CASE_GetStreamSize, .-CASE_GetStreamSize
	.align	3
.LC35:
	.xword	.LANCHOR0
	.align	3
.LC36:
	.xword	.LC1
	.align	3
.LC37:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_ExtraWakupThread
	.type	CASE_ExtraWakupThread, %function
CASE_ExtraWakupThread:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC38
	mov	w0, 0
	ldr	x1, [x19, 24]
	blr	x1
	bl	VCTRL_ExtraWakeUpThread
	ldr	x1, [x19, 32]
	mov	w0, 0
	blr	x1
	ldr	x19, [sp, 16]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	CASE_ExtraWakupThread, .-CASE_ExtraWakupThread
	.align	3
.LC38:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_GetChanIDByMem
	.type	CASE_GetChanIDByMem, %function
CASE_GetChanIDByMem:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L71
	ldr	x20, .LC41
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	ldr	w0, [x19]
	bl	VCTRL_GetChanIDByMemAddr
	str	w0, [x19]
	cmn	w0, #1
	mov	w0, 0
	ldr	x1, [x20, 32]
	csetm	w19, eq
	blr	x1
.L70:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L71:
	ldr	x2, .LC39
	mov	w3, 412
	ldr	x1, .LC40
	mov	w0, 0
	add	x2, x2, 248
	mov	w19, -1
	bl	dprint_vfmw
	b	.L70
	.size	CASE_GetChanIDByMem, .-CASE_GetChanIDByMem
	.align	3
.LC39:
	.xword	.LANCHOR0
	.align	3
.LC40:
	.xword	.LC1
	.align	3
.LC41:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_SetDiscardPicParam
	.type	CASE_SetDiscardPicParam, %function
CASE_SetDiscardPicParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L75
	ldr	x20, .LC44
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	mov	x1, x19
	mov	w0, w21
	bl	VCTRL_SetDiscardPicParam
	mov	w19, w0
	ldr	x1, [x20, 32]
	mov	w0, 0
	blr	x1
	mov	w0, w19
.L74:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L75:
	ldr	x2, .LC42
	mov	w3, 429
	ldr	x1, .LC43
	mov	w0, 0
	add	x2, x2, 272
	bl	dprint_vfmw
	mov	w0, -1
	b	.L74
	.size	CASE_SetDiscardPicParam, .-CASE_SetDiscardPicParam
	.align	3
.LC42:
	.xword	.LANCHOR0
	.align	3
.LC43:
	.xword	.LC1
	.align	3
.LC44:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_SetPtsToSeek
	.type	CASE_SetPtsToSeek, %function
CASE_SetPtsToSeek:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w22, w0
	mov	x21, x1
	mov	x0, x30
	bl	_mcount
	cbz	x21, .L80
	ldr	x20, .LC47
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	str	wzr, [x29, 56]
	mov	w2, 1
	add	x1, x29, 48
	mov	w0, w22
	stp	w2, wzr, [x29, 48]
	bl	VCTRL_ResetChanWithOption
	cmp	w0, wzr
	csetm	w19, ne
	cbz	w19, .L81
.L79:
	ldr	x1, [x20, 32]
	mov	w0, 0
	blr	x1
	mov	w0, w19
.L78:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L81:
	mov	x1, x21
	mov	w0, w22
	bl	VCTRL_SeekPts
	cmp	w0, wzr
	csetm	w19, ne
	b	.L79
.L80:
	ldr	x2, .LC45
	mov	w3, 446
	ldr	x1, .LC46
	mov	w0, 0
	add	x2, x2, 296
	bl	dprint_vfmw
	mov	w0, -1
	b	.L78
	.size	CASE_SetPtsToSeek, .-CASE_SetPtsToSeek
	.align	3
.LC45:
	.xword	.LANCHOR0
	.align	3
.LC46:
	.xword	.LC1
	.align	3
.LC47:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_SetTrickMode
	.type	CASE_SetTrickMode, %function
CASE_SetTrickMode:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L85
	ldr	x20, .LC50
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	mov	x1, x19
	mov	w0, w21
	bl	VCTRL_ConfigFFFBSpeed
	cmp	w0, wzr
	ldr	x1, [x20, 32]
	mov	w0, 0
	csetm	w19, ne
	blr	x1
.L84:
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L85:
	ldr	x2, .LC48
	mov	w3, 472
	ldr	x1, .LC49
	mov	w0, 0
	add	x2, x2, 320
	mov	w19, -1
	bl	dprint_vfmw
	b	.L84
	.size	CASE_SetTrickMode, .-CASE_SetTrickMode
	.align	3
.LC48:
	.xword	.LANCHOR0
	.align	3
.LC49:
	.xword	.LC1
	.align	3
.LC50:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_SetCtrlInfo
	.type	CASE_SetCtrlInfo, %function
CASE_SetCtrlInfo:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L89
	ldr	x20, .LC53
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	mov	x1, x19
	mov	w0, w21
	bl	VCTRL_ConfigPVRInfo
	cmp	w0, wzr
	ldr	x1, [x20, 32]
	mov	w0, 0
	csetm	w19, ne
	blr	x1
.L88:
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L89:
	ldr	x2, .LC51
	mov	w3, 488
	ldr	x1, .LC52
	mov	w0, 0
	add	x2, x2, 344
	mov	w19, -1
	bl	dprint_vfmw
	b	.L88
	.size	CASE_SetCtrlInfo, .-CASE_SetCtrlInfo
	.align	3
.LC51:
	.xword	.LANCHOR0
	.align	3
.LC52:
	.xword	.LC1
	.align	3
.LC53:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_SetFrameRate
	.type	CASE_SetFrameRate, %function
CASE_SetFrameRate:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L93
	ldr	x1, .LC56
	mov	w0, 0
	ldr	w2, [x19]
	ldr	x1, [x1, w20, sxtw 3]
	str	w2, [x1, 1624]
.L92:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L93:
	ldr	x2, .LC54
	mov	w3, 502
	ldr	x1, .LC55
	mov	w0, 0
	add	x2, x2, 368
	bl	dprint_vfmw
	mov	w0, -1
	b	.L92
	.size	CASE_SetFrameRate, .-CASE_SetFrameRate
	.align	3
.LC54:
	.xword	.LANCHOR0
	.align	3
.LC55:
	.xword	.LC1
	.align	3
.LC56:
	.xword	s_pstVfmwChan
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_AllocMemToDecoder
	.type	CASE_AllocMemToDecoder, %function
CASE_AllocMemToDecoder:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L101
	ldr	x20, .LC59
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	mov	x1, x19
	mov	w0, w21
	bl	VCTRL_AllocMemToDecoder
	mov	w19, w0
	cbnz	w0, .L102
.L97:
	ldr	x1, [x20, 32]
	mov	w0, 0
	blr	x1
	mov	w0, w19
.L96:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L102:
	ldr	x1, .LC61
	mov	w0, 1
	bl	dprint_vfmw
	b	.L97
.L101:
	ldr	x2, .LC57
	mov	w3, 513
	ldr	x1, .LC58
	mov	w0, 0
	add	x2, x2, 392
	bl	dprint_vfmw
	mov	w0, -1
	b	.L96
	.size	CASE_AllocMemToDecoder, .-CASE_AllocMemToDecoder
	.align	3
.LC57:
	.xword	.LANCHOR0
	.align	3
.LC58:
	.xword	.LC1
	.align	3
.LC59:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC61:
	.xword	.LC60
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_SetFrameBufferNum
	.type	CASE_SetFrameBufferNum, %function
CASE_SetFrameBufferNum:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L107
	ldr	x0, .LC64
	ldr	x1, [x0, w20, sxtw 3]
	cbz	x1, .L106
	add	x2, x1, 323584
	ldr	w3, [x19]
	mov	w0, 0
	str	w3, [x2, 2644]
	ldr	w2, [x19]
	str	w2, [x1, 1560]
.L105:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L106:
	mov	w0, -1
	b	.L105
.L107:
	ldr	x2, .LC62
	mov	w3, 532
	ldr	x1, .LC63
	mov	w0, 0
	add	x2, x2, 416
	bl	dprint_vfmw
	mov	w0, -1
	b	.L105
	.size	CASE_SetFrameBufferNum, .-CASE_SetFrameBufferNum
	.align	3
.LC62:
	.xword	.LANCHOR0
	.align	3
.LC63:
	.xword	.LC1
	.align	3
.LC64:
	.xword	s_pstVfmwChan
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_GetUserDecFrame
	.type	CASE_GetUserDecFrame, %function
CASE_GetUserDecFrame:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L111
	ldr	x20, .LC67
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	mov	x1, x19
	mov	w0, w21
	bl	VCTRL_GetUsrdecFrame
	cmp	w0, wzr
	ldr	x1, [x20, 32]
	mov	w0, 0
	csetm	w19, ne
	blr	x1
.L110:
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L111:
	ldr	x2, .LC65
	mov	w3, 553
	ldr	x1, .LC66
	mov	w0, 0
	add	x2, x2, 440
	mov	w19, -1
	bl	dprint_vfmw
	b	.L110
	.size	CASE_GetUserDecFrame, .-CASE_GetUserDecFrame
	.align	3
.LC65:
	.xword	.LANCHOR0
	.align	3
.LC66:
	.xword	.LC1
	.align	3
.LC67:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_PutUserDecFrame
	.type	CASE_PutUserDecFrame, %function
CASE_PutUserDecFrame:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L115
	ldr	x20, .LC70
	mov	w0, 0
	ldr	x1, [x20, 24]
	blr	x1
	mov	x1, x19
	mov	w0, w21
	bl	VCTRL_PutUsrdecFrame
	cmp	w0, wzr
	ldr	x1, [x20, 32]
	mov	w0, 0
	csetm	w19, ne
	blr	x1
.L114:
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L115:
	ldr	x2, .LC68
	mov	w3, 569
	ldr	x1, .LC69
	mov	w0, 0
	add	x2, x2, 464
	mov	w19, -1
	bl	dprint_vfmw
	b	.L114
	.size	CASE_PutUserDecFrame, .-CASE_PutUserDecFrame
	.align	3
.LC68:
	.xword	.LANCHOR0
	.align	3
.LC69:
	.xword	.LC1
	.align	3
.LC70:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_StartLowDelayCalc
	.type	CASE_StartLowDelayCalc, %function
CASE_StartLowDelayCalc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L119
	ldr	x2, .LC73
	mov	w3, 1
	ldr	x1, .LC74
	mov	w0, 0
	strb	w3, [x2, w20, sxtw]
	ldr	w2, [x19]
	str	w2, [x1, w20, sxtw 2]
.L118:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L119:
	ldr	x2, .LC71
	mov	w3, 588
	ldr	x1, .LC72
	mov	w0, 0
	add	x2, x2, 488
	bl	dprint_vfmw
	mov	w0, -1
	b	.L118
	.size	CASE_StartLowDelayCalc, .-CASE_StartLowDelayCalc
	.align	3
.LC71:
	.xword	.LANCHOR0
	.align	3
.LC72:
	.xword	.LC1
	.align	3
.LC73:
	.xword	g_LowDelayCalcStat
	.align	3
.LC74:
	.xword	g_LowDelayCalcHandle
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_StopLowDelayCalc
	.type	CASE_StopLowDelayCalc, %function
CASE_StopLowDelayCalc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x2, .LC75
	mov	w0, 0
	ldr	x1, .LC76
	strb	wzr, [x2, w19, sxtw]
	str	wzr, [x1, w19, sxtw 2]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	CASE_StopLowDelayCalc, .-CASE_StopLowDelayCalc
	.align	3
.LC75:
	.xword	g_LowDelayCalcStat
	.align	3
.LC76:
	.xword	g_LowDelayCalcHandle
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_ReportOccupiedFrame
	.type	CASE_ReportOccupiedFrame, %function
CASE_ReportOccupiedFrame:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L128
	mov	x1, x19
	mov	w0, w20
	bl	VCTRL_ReportOccupiedFrame
	mov	w19, w0
	cbnz	w0, .L129
.L124:
	cmp	w19, wzr
	csetm	w0, ne
.L123:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L129:
	ldr	x1, .LC80
	mov	w2, w20
	mov	w0, 1
	bl	dprint_vfmw
	b	.L124
.L128:
	ldr	x2, .LC77
	mov	w3, 611
	ldr	x1, .LC78
	mov	w0, 0
	add	x2, x2, 512
	bl	dprint_vfmw
	mov	w0, -1
	b	.L123
	.size	CASE_ReportOccupiedFrame, .-CASE_ReportOccupiedFrame
	.align	3
.LC77:
	.xword	.LANCHOR0
	.align	3
.LC78:
	.xword	.LC1
	.align	3
.LC80:
	.xword	.LC79
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_SetImageOutputInfo
	.type	CASE_SetImageOutputInfo, %function
CASE_SetImageOutputInfo:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L137
	mov	x1, x19
	mov	w0, w20
	bl	VCTRL_ImageOutputEnable
	mov	w19, w0
	cbnz	w0, .L138
.L133:
	cmp	w19, wzr
	csetm	w0, ne
.L132:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L138:
	ldr	x1, .LC84
	mov	w2, w20
	mov	w0, 1
	bl	dprint_vfmw
	b	.L133
.L137:
	ldr	x2, .LC81
	mov	w3, 630
	ldr	x1, .LC82
	mov	w0, 0
	add	x2, x2, 544
	bl	dprint_vfmw
	mov	w0, -1
	b	.L132
	.size	CASE_SetImageOutputInfo, .-CASE_SetImageOutputInfo
	.align	3
.LC81:
	.xword	.LANCHOR0
	.align	3
.LC82:
	.xword	.LC1
	.align	3
.LC84:
	.xword	.LC83
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_ReportDemuxRelease
	.type	CASE_ReportDemuxRelease, %function
CASE_ReportDemuxRelease:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC85
	mov	w0, 0
	ldr	x1, [x19, 24]
	blr	x1
	mov	w0, w20
	bl	VCTRL_IfEsBufEnable
	cbz	w0, .L140
	mov	w0, w20
	bl	VCTRL_DisableEsBuf
.L140:
	ldr	x1, [x19, 32]
	mov	w0, 0
	blr	x1
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	CASE_ReportDemuxRelease, .-CASE_ReportDemuxRelease
	.align	3
.LC85:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CASE_HandleEntry
	.type	CASE_HandleEntry, %function
CASE_HandleEntry:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	mov	x21, x2
	bl	_mcount
	sub	w0, w19, #1
	cmp	w0, 44
	bls	.L185
.L184:
	mov	w0, -1
.L145:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L185:
	ldr	x1, .LC86
	ldrh	w0, [x1,w0,uxtw #1]
	adr	x1, .Lrtx147
	add	x0, x1, w0, sxth #2
	br	x0
.Lrtx147:
	.section	.rodata
	.align	0
	.align	2
.L147:
	.2byte	(.L146 - .Lrtx147) / 4
	.2byte	(.L184 - .Lrtx147) / 4
	.2byte	(.L184 - .Lrtx147) / 4
	.2byte	(.L148 - .Lrtx147) / 4
	.2byte	(.L149 - .Lrtx147) / 4
	.2byte	(.L150 - .Lrtx147) / 4
	.2byte	(.L151 - .Lrtx147) / 4
	.2byte	(.L152 - .Lrtx147) / 4
	.2byte	(.L153 - .Lrtx147) / 4
	.2byte	(.L154 - .Lrtx147) / 4
	.2byte	(.L155 - .Lrtx147) / 4
	.2byte	(.L156 - .Lrtx147) / 4
	.2byte	(.L157 - .Lrtx147) / 4
	.2byte	(.L158 - .Lrtx147) / 4
	.2byte	(.L159 - .Lrtx147) / 4
	.2byte	(.L184 - .Lrtx147) / 4
	.2byte	(.L160 - .Lrtx147) / 4
	.2byte	(.L161 - .Lrtx147) / 4
	.2byte	(.L184 - .Lrtx147) / 4
	.2byte	(.L162 - .Lrtx147) / 4
	.2byte	(.L163 - .Lrtx147) / 4
	.2byte	(.L164 - .Lrtx147) / 4
	.2byte	(.L184 - .Lrtx147) / 4
	.2byte	(.L184 - .Lrtx147) / 4
	.2byte	(.L165 - .Lrtx147) / 4
	.2byte	(.L166 - .Lrtx147) / 4
	.2byte	(.L184 - .Lrtx147) / 4
	.2byte	(.L167 - .Lrtx147) / 4
	.2byte	(.L184 - .Lrtx147) / 4
	.2byte	(.L168 - .Lrtx147) / 4
	.2byte	(.L184 - .Lrtx147) / 4
	.2byte	(.L184 - .Lrtx147) / 4
	.2byte	(.L169 - .Lrtx147) / 4
	.2byte	(.L170 - .Lrtx147) / 4
	.2byte	(.L171 - .Lrtx147) / 4
	.2byte	(.L172 - .Lrtx147) / 4
	.2byte	(.L173 - .Lrtx147) / 4
	.2byte	(.L174 - .Lrtx147) / 4
	.2byte	(.L184 - .Lrtx147) / 4
	.2byte	(.L175 - .Lrtx147) / 4
	.2byte	(.L176 - .Lrtx147) / 4
	.2byte	(.L184 - .Lrtx147) / 4
	.2byte	(.L177 - .Lrtx147) / 4
	.2byte	(.L178 - .Lrtx147) / 4
	.2byte	(.L179 - .Lrtx147) / 4
	.text
	.p2align 3
.L179:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_ReportOccupiedFrame
	b	.L145
	.p2align 3
.L178:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_SetImageOutputInfo
	b	.L145
	.p2align 3
.L177:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_ReportDemuxRelease
	b	.L145
	.p2align 3
.L176:
	cbz	x21, .L186
	ldr	x0, .LC89
	ldr	x1, [x0, w20, sxtw 3]
	cbz	x1, .L184
	add	x2, x1, 323584
	ldr	w3, [x21]
	mov	w0, 0
	str	w3, [x2, 2644]
	ldr	w2, [x21]
	str	w2, [x1, 1560]
	b	.L145
	.p2align 3
.L175:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_AllocMemToDecoder
	b	.L145
	.p2align 3
.L174:
	ldr	x2, .LC90
	mov	w0, 0
	ldr	x1, .LC91
	strb	wzr, [x2, w20, sxtw]
	str	wzr, [x1, w20, sxtw 2]
	b	.L145
	.p2align 3
.L173:
	cbz	x21, .L187
	ldr	x2, .LC90
	mov	w3, 1
	ldr	x1, .LC91
	mov	w0, 0
	strb	w3, [x2, w20, sxtw]
	ldr	w2, [x21]
	str	w2, [x1, w20, sxtw 2]
	b	.L145
	.p2align 3
.L172:
	cbz	x21, .L188
	ldr	x1, .LC89
	mov	w0, 0
	ldr	w2, [x21]
	ldr	x1, [x1, w20, sxtw 3]
	str	w2, [x1, 1624]
	b	.L145
	.p2align 3
.L171:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_SetCtrlInfo
	b	.L145
	.p2align 3
.L170:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_SetTrickMode
	b	.L145
	.p2align 3
.L169:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_SetPtsToSeek
	b	.L145
	.p2align 3
.L168:
	bl	CASE_ExtraWakupThread
	b	.L145
	.p2align 3
.L167:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_SetDiscardPicParam
	b	.L145
	.p2align 3
.L166:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_PutUserDecFrame
	b	.L145
	.p2align 3
.L165:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_GetUserDecFrame
	b	.L145
	.p2align 3
.L164:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_ResetChanWithOption
	b	.L145
	.p2align 3
.L163:
	mov	w0, w20
	bl	CASE_ReleaseStream
	b	.L145
	.p2align 3
.L162:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_GetChanIDByMem
	b	.L145
	.p2align 3
.L161:
	mov	x0, x21
	bl	CASE_GetChanMemSizeWithOption
	b	.L145
	.p2align 3
.L160:
	mov	x0, x21
	bl	CASE_GetChanMemSize
	b	.L145
	.p2align 3
.L159:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_GetStreamSize
	b	.L145
	.p2align 3
.L158:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_SetStreamIntf
	b	.L145
	.p2align 3
.L157:
	mov	w0, w20
	bl	CASE_ResetChan
	b	.L145
	.p2align 3
.L156:
	mov	w0, w20
	bl	VCTRL_StopChanWithCheck
	cmp	w0, wzr
	csetm	w0, ne
	b	.L145
	.p2align 3
.L155:
	mov	w0, w20
	bl	CASE_StartChan
	b	.L145
	.p2align 3
.L154:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_GetChanState
	b	.L145
	.p2align 3
.L153:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_ConfigChan
	b	.L145
	.p2align 3
.L152:
	mov	x1, x21
	mov	w0, w20
	bl	CASE_GetChanConfig
	b	.L145
	.p2align 3
.L151:
	mov	w0, w20
	bl	CASE_DestroyChanWithOption
	b	.L145
	.p2align 3
.L150:
	mov	w0, w20
	bl	CASE_DestroyChan
	b	.L145
	.p2align 3
.L149:
	mov	x0, x21
	bl	CASE_CreateChanWithOption
	b	.L145
	.p2align 3
.L148:
	mov	x0, x21
	bl	CASE_CreateChan
	b	.L145
	.p2align 3
.L146:
	mov	x0, x21
	bl	CASE_GetCapability
	b	.L145
.L186:
	ldr	x2, .LC87
	mov	w3, 532
	ldr	x1, .LC88
	mov	w0, 0
	add	x2, x2, 416
	bl	dprint_vfmw
	mov	w0, -1
	b	.L145
.L187:
	ldr	x2, .LC87
	mov	w3, 588
	ldr	x1, .LC88
	mov	w0, 0
	add	x2, x2, 488
	bl	dprint_vfmw
	mov	w0, -1
	b	.L145
.L188:
	ldr	x2, .LC87
	mov	w3, 502
	ldr	x1, .LC88
	mov	w0, 0
	add	x2, x2, 368
	bl	dprint_vfmw
	mov	w0, -1
	b	.L145
	.size	CASE_HandleEntry, .-CASE_HandleEntry
	.align	3
.LC86:
	.xword	.L147
	.align	3
.LC87:
	.xword	.LANCHOR0
	.align	3
.LC88:
	.xword	.LC1
	.align	3
.LC89:
	.xword	s_pstVfmwChan
	.align	3
.LC90:
	.xword	g_LowDelayCalcStat
	.align	3
.LC91:
	.xword	g_LowDelayCalcHandle
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.11536, %object
	.size	__func__.11536, 19
__func__.11536:
	.string	"CASE_GetCapability"
	.zero	5
	.type	__func__.11545, %object
	.size	__func__.11545, 20
__func__.11545:
	.string	"CASE_GetChanMemSize"
	.zero	4
	.type	__func__.11553, %object
	.size	__func__.11553, 30
__func__.11553:
	.string	"CASE_GetChanMemSizeWithOption"
	.zero	2
	.type	__func__.11559, %object
	.size	__func__.11559, 16
__func__.11559:
	.string	"CASE_CreateChan"
	.type	__func__.11566, %object
	.size	__func__.11566, 26
__func__.11566:
	.string	"CASE_CreateChanWithOption"
	.zero	6
	.type	__func__.11592, %object
	.size	__func__.11592, 25
__func__.11592:
	.string	"CASE_ResetChanWithOption"
	.zero	7
	.type	__func__.11606, %object
	.size	__func__.11606, 19
__func__.11606:
	.string	"CASE_GetChanConfig"
	.zero	5
	.type	__func__.11612, %object
	.size	__func__.11612, 16
__func__.11612:
	.string	"CASE_ConfigChan"
	.type	__func__.11626, %object
	.size	__func__.11626, 19
__func__.11626:
	.string	"CASE_SetStreamIntf"
	.zero	5
	.type	__func__.11632, %object
	.size	__func__.11632, 19
__func__.11632:
	.string	"CASE_GetStreamSize"
	.zero	5
	.type	__func__.11642, %object
	.size	__func__.11642, 20
__func__.11642:
	.string	"CASE_GetChanIDByMem"
	.zero	4
	.type	__func__.11648, %object
	.size	__func__.11648, 24
__func__.11648:
	.string	"CASE_SetDiscardPicParam"
	.type	__func__.11656, %object
	.size	__func__.11656, 18
__func__.11656:
	.string	"CASE_SetPtsToSeek"
	.zero	6
	.type	__func__.11662, %object
	.size	__func__.11662, 18
__func__.11662:
	.string	"CASE_SetTrickMode"
	.zero	6
	.type	__func__.11668, %object
	.size	__func__.11668, 17
__func__.11668:
	.string	"CASE_SetCtrlInfo"
	.zero	7
	.type	__func__.11673, %object
	.size	__func__.11673, 18
__func__.11673:
	.string	"CASE_SetFrameRate"
	.zero	6
	.type	__func__.11679, %object
	.size	__func__.11679, 23
__func__.11679:
	.string	"CASE_AllocMemToDecoder"
	.zero	1
	.type	__func__.11685, %object
	.size	__func__.11685, 23
__func__.11685:
	.string	"CASE_SetFrameBufferNum"
	.zero	1
	.type	__func__.11691, %object
	.size	__func__.11691, 21
__func__.11691:
	.string	"CASE_GetUserDecFrame"
	.zero	3
	.type	__func__.11697, %object
	.size	__func__.11697, 21
__func__.11697:
	.string	"CASE_PutUserDecFrame"
	.zero	3
	.type	__func__.11704, %object
	.size	__func__.11704, 23
__func__.11704:
	.string	"CASE_StartLowDelayCalc"
	.zero	1
	.type	__func__.11714, %object
	.size	__func__.11714, 25
__func__.11714:
	.string	"CASE_ReportOccupiedFrame"
	.zero	7
	.type	__func__.11721, %object
	.size	__func__.11721, 24
__func__.11721:
	.string	"CASE_SetImageOutputInfo"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC1:
	ASCII(.string	"invalid condition: %s %d\n" )
	.zero	6
.LC15:
	ASCII(.string	"VCTRT_SetChanCtrlState ERR: %d\n" )
.LC60:
	ASCII(.string	"VCTRL_AllocMemToDecoder failed!\n " )
	.zero	6
.LC79:
	ASCII(.string	"VCTRL_ReportOccupiedFrame[Chan %d] failed!\n" )
	.zero	4
.LC83:
	ASCII(.string	"VCTRL_ImageOutputEnable[Chan %d] failed!\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
