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
	.file	"vfmw.c"
	.text
	.align	2
	.global	VDM_OpenHardware
	.type	VDM_OpenHardware, %function
VDM_OpenHardware:
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
	ldr	r2, .L2
	mov	r0, #30
	ldr	r3, .L2+4
	ldr	r2, [r2]
	ldr	r1, [r3, #116]
	ldr	r3, [r2, #120]
	orr	r3, r3, #1
	bfc	r3, #4, #1
	bfc	r3, #6, #1
	bfc	r3, #8, #2
	bfc	r3, #12, #5
	bfc	r3, #17, #1
	str	r3, [r2, #120]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r1	@ indirect register sibling call
.L3:
	.align	2
.L2:
	.word	g_pstRegCrg
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDM_OpenHardware, .-VDM_OpenHardware
	.align	2
	.global	DSP_OpenHardware
	.type	DSP_OpenHardware, %function
DSP_OpenHardware:
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
	ldr	r2, .L5
	mov	r0, #30
	ldr	r3, .L5+4
	ldr	r2, [r2]
	ldr	r1, [r3, #116]
	ldr	r3, [r2, #120]
	orr	r3, r3, #2
	str	r3, [r2, #120]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r1	@ indirect register sibling call
.L6:
	.align	2
.L5:
	.word	g_pstRegCrg
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	DSP_OpenHardware, .-DSP_OpenHardware
	.align	2
	.global	VDM_CloseHardware
	.type	VDM_CloseHardware, %function
VDM_CloseHardware:
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
	ldr	r4, .L11
	ldr	r6, .L11+4
	ldr	r0, [r4, #276]
	ldr	r3, [r6]
	cmp	r0, #0
	ldr	r5, [r3, #120]
	beq	.L8
	blx	r0
.L9:
	ldr	r2, [r6]
	bfc	r5, #0, #1
	ldr	r3, [r4, #116]
	mov	r0, #30
	str	r5, [r2, #120]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L8:
	ldr	r3, [r4, #68]
	mov	r2, #245
	ldr	r1, .L11+8
	blx	r3
	b	.L9
.L12:
	.align	2
.L11:
	.word	.LANCHOR0
	.word	g_pstRegCrg
	.word	.LC0
	UNWIND(.fnend)
	.size	VDM_CloseHardware, .-VDM_CloseHardware
	.align	2
	.global	DSP_CloseHardware
	.type	DSP_CloseHardware, %function
DSP_CloseHardware:
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
	ldr	r2, .L14
	mov	r0, #30
	ldr	r3, .L14+4
	ldr	r2, [r2]
	ldr	r1, [r3, #116]
	ldr	r3, [r2, #120]
	bfc	r3, #1, #1
	str	r3, [r2, #120]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r1	@ indirect register sibling call
.L15:
	.align	2
.L14:
	.word	g_pstRegCrg
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	DSP_CloseHardware, .-DSP_CloseHardware
	.align	2
	.global	SCD_OpenHardware
	.type	SCD_OpenHardware, %function
SCD_OpenHardware:
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
	ldr	r3, .L17
	ldr	r2, [r3]
	ldr	r3, [r2, #120]
	bfc	r3, #5, #1
	str	r3, [r2, #120]
	ldmfd	sp, {fp, sp, pc}
.L18:
	.align	2
.L17:
	.word	g_pstRegCrg
	UNWIND(.fnend)
	.size	SCD_OpenHardware, .-SCD_OpenHardware
	.align	2
	.global	SCD_CloseHardware
	.type	SCD_CloseHardware, %function
SCD_CloseHardware:
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
	ldr	r3, .L20
	ldr	r2, [r3]
	ldr	r3, [r2, #120]
	orr	r3, r3, #32
	str	r3, [r2, #120]
	ldmfd	sp, {fp, sp, pc}
.L21:
	.align	2
.L20:
	.word	g_pstRegCrg
	UNWIND(.fnend)
	.size	SCD_CloseHardware, .-SCD_CloseHardware
	.align	2
	.global	BPD_OpenHardware
	.type	BPD_OpenHardware, %function
BPD_OpenHardware:
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
	ldr	r3, .L23
	mov	r0, #0
	ldr	r2, [r3]
	ldr	r3, [r2, #136]
	orr	r3, r3, #1
	bfc	r3, #4, #1
	str	r3, [r2, #136]
	ldmfd	sp, {fp, sp, pc}
.L24:
	.align	2
.L23:
	.word	g_pstRegCrg
	UNWIND(.fnend)
	.size	BPD_OpenHardware, .-BPD_OpenHardware
	.align	2
	.global	BPD_CloseHardware
	.type	BPD_CloseHardware, %function
BPD_CloseHardware:
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
	ldr	r3, .L26
	mov	r0, #0
	ldr	r2, [r3]
	ldr	r3, [r2, #136]
	bic	r3, r3, #17
	orr	r3, r3, #16
	str	r3, [r2, #136]
	ldmfd	sp, {fp, sp, pc}
.L27:
	.align	2
.L26:
	.word	g_pstRegCrg
	UNWIND(.fnend)
	.size	BPD_CloseHardware, .-BPD_CloseHardware
	.align	2
	.global	VDEC_Lowdelay_Event_Time
	.type	VDEC_Lowdelay_Event_Time, %function
VDEC_Lowdelay_Event_Time:
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
	sub	ip, fp, #28
	mov	r4, r0
	mov	r3, #0
	str	r3, [ip, #-20]!
	mov	r0, ip
	mov	r5, r2
	mov	r6, r1
	bl	HI_DRV_SYS_GetTimeStampMs
	ldr	r3, .L31
	ldr	r0, [fp, #-48]
	add	r2, r3, r4
	add	r3, r3, r4, lsl #2
	str	r6, [fp, #-44]
	ldrb	r2, [r2, #404]	@ zero_extendqisi2
	ldr	r3, [r3, #340]
	cmp	r2, #1
	str	r5, [fp, #-36]
	str	r0, [fp, #-32]
	str	r3, [fp, #-40]
	beq	.L30
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L30:
	sub	r0, fp, #44
	bl	HI_DRV_LD_Notify_Event
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L32:
	.align	2
.L31:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDEC_Lowdelay_Event_Time, .-VDEC_Lowdelay_Event_Time
	.align	2
	.global	VDEC_OpenModule
	.type	VDEC_OpenModule, %function
VDEC_OpenModule:
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
	ldr	r4, .L34
	ldr	r3, [r4, #20]
	blx	r3
	ldr	r3, [r4, #196]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L35:
	.align	2
.L34:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDEC_OpenModule, .-VDEC_OpenModule
	.align	2
	.global	VDEC_ExitModule
	.type	VDEC_ExitModule, %function
VDEC_ExitModule:
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
	ldr	r3, .L37
	ldr	r3, [r3, #200]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L38:
	.align	2
.L37:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDEC_ExitModule, .-VDEC_ExitModule
	.align	2
	.global	VDM_ResetClock
	.type	VDM_ResetClock, %function
VDM_ResetClock:
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
	ldr	r3, .L40
	ldr	r2, [r3]
	ldr	r3, [r2, #120]
	bfc	r3, #12, #5
	bfc	r3, #17, #1
	str	r3, [r2, #120]
	orr	r3, r3, #131072
	str	r3, [r2, #120]
	ldmfd	sp, {fp, sp, pc}
.L41:
	.align	2
.L40:
	.word	g_pstRegCrg
	UNWIND(.fnend)
	.size	VDM_ResetClock, .-VDM_ResetClock
	.align	2
	.global	VDEC_SpinLockInit
	.type	VDEC_SpinLockInit, %function
VDEC_SpinLockInit:
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
	ldr	r4, .L43
	mov	r0, #0
	ldr	r3, [r4, #8]
	blx	r3
	ldr	r3, [r4, #8]
	mov	r0, #1
	blx	r3
	ldr	r3, [r4, #8]
	mov	r0, #2
	blx	r3
	ldr	r3, [r4, #8]
	mov	r0, #3
	blx	r3
	ldr	r3, [r4, #8]
	mov	r0, #8
	blx	r3
	ldr	r3, [r4, #8]
	mov	r0, #4
	blx	r3
	ldr	r3, [r4, #8]
	mov	r0, #5
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L44:
	.align	2
.L43:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDEC_SpinLockInit, .-VDEC_SpinLockInit
	.align	2
	.global	VDEC_InitWithOperation
	.type	VDEC_InitWithOperation, %function
VDEC_InitWithOperation:
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
	subs	r5, r0, #0
	beq	.L58
	ldr	r4, .L60
	ldr	r3, [r4, #24]
	blx	r3
	ldr	r3, [r4, #420]
	cmp	r3, #1
	beq	.L59
	ldr	r3, .L60+4
	mov	r10, #0
	ldr	r9, .L60+8
	ldr	r8, .L60+12
	str	r3, [r4, #252]
	ldr	r3, .L60+16
	str	r10, [r9]
	ldr	r7, .L60+20
	str	r3, [r4, #256]
	ldr	r3, .L60+24
	ldr	r6, .L60+28
	ldr	lr, .L60+32
	str	r3, [r4, #260]
	ldr	r3, .L60+36
	ldr	ip, .L60+40
	ldr	r0, .L60+44
	str	r3, [r4, #268]
	ldr	r3, .L60+48
	ldr	r1, .L60+52
	ldr	r2, .L60+56
	str	r3, [r4, #272]
	ldr	r3, .L60+60
	ldr	r9, .L60+64
	str	r8, [r4, #216]
	str	r3, [r4, #276]
	ldr	r3, .L60+68
	str	r7, [r4, #220]
	str	r6, [r4, #224]
	str	r3, [r4, #284]
	ldr	r3, .L60+72
	str	lr, [r4, #228]
	str	ip, [r4, #232]
	str	r3, [r4, #288]
	ldr	r3, .L60+76
	str	r0, [r4, #236]
	ldr	ip, .L60+80
	str	r1, [r4, #240]
	ldr	lr, .L60+84
	str	r2, [r4, #244]
	str	r9, [r4, #264]
	ldr	r0, .L60+88
	ldr	r1, .L60+92
	ldr	r2, .L60+96
	str	r3, [r4, #292]
	ldr	r6, .L60+100
	ldr	r3, .L60+104
	ldr	r7, .L60+108
	ldr	r8, .L60+112
	ldr	r10, .L60+116
	ldr	r9, .L60+120
	str	r3, [r4, #332]
	str	ip, [r4, #296]
	str	lr, [r4, #300]
	str	r6, [r4, #304]
	str	r7, [r4, #308]
	str	r8, [r4, #312]
	str	r10, [r4, #316]
	str	r9, [r4, #320]
	str	r0, [r4, #324]
	str	r1, [r4, #328]
	str	r2, [r4, #248]
	bl	VDEC_SpinLockInit
	ldr	r3, [r4, #248]
	cmp	r3, #0
	beq	.L50
	blx	r3
.L50:
	mov	r0, r5
	ldr	r7, .L60
	bl	MEM_ManagerWithOperation
	bl	VCTRL_OpenVfmw
	ldr	r3, [r4, #68]
	ldr	r1, .L60+124
	mov	r6, r0
	mov	r2, r0
	mov	r0, #2
	blx	r3
	cmp	r6, #0
	ldr	r0, [r4, #276]
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	str	r3, [r7, #420]
	str	r3, [r7, #424]
	ldr	r3, .L60
	beq	.L53
	blx	r0
.L54:
	ldr	r3, .L60+128
	movw	r1, #8695
	movt	r1, 1
	ldr	r0, [r3]
	add	r0, r0, #4
	bl	MEM_WritePhyWord
	adds	r6, r6, #0
	movne	r6, #1
	rsb	r6, r6, #0
.L49:
	ldmia	r5, {r0, r1}
	bl	VCTRL_SetCallBack
	ldr	r3, [r4, #28]
	blx	r3
.L47:
	mov	r0, r6
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L59:
	ldr	r3, [r4, #424]
	mov	r6, #0
	add	r3, r3, #1
	str	r3, [r4, #424]
	b	.L49
.L53:
	ldr	r3, [r3, #68]
	movw	r2, #499
	ldr	r1, .L60+132
	blx	r3
	b	.L54
.L58:
	ldr	r3, .L60
	mov	r0, #1
	ldr	r1, .L60+136
	mvn	r6, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L47
.L61:
	.align	2
.L60:
	.word	.LANCHOR0
	.word	VDMHAL_V300R001_GetHalMemSize
	.word	AcceleratorCharacter
	.word	VDM_OpenHardware
	.word	VDMHAL_V300R001_OpenHAL
	.word	VDM_CloseHardware
	.word	VDMHAL_V300R001_CloseHAL
	.word	DSP_OpenHardware
	.word	DSP_CloseHardware
	.word	VDMHAL_V300R001_ArrangeMem_BTL
	.word	SCD_OpenHardware
	.word	SCD_CloseHardware
	.word	VDMHAL_V300R001_ResetVdm
	.word	BPD_OpenHardware
	.word	BPD_CloseHardware
	.word	VDMHAL_V300R001_GlbReset
	.word	VDMHAL_V300R001_ArrangeMem
	.word	VDMHAL_V300R001_ClearIntState
	.word	VDMHAL_V300R001_MaskInt
	.word	VDMHAL_V300R001_EnableInt
	.word	VDMHAL_V300R001_CheckReg
	.word	VDMHAL_V300R001_StartHwRepair
	.word	VDMHAL_V300R001_MakeDecReport
	.word	VDMHAL_V300R001_BackupInfo
	.word	VDMHAL_V300R001_GetCharacter
	.word	VDMHAL_V300R001_StartHwDecode
	.word	VDMHAL_V300R001_WriteBigTitle1DYuv
	.word	VDMHAL_V300R001_PrepareDec
	.word	VDMHAL_V300R001_IsVdmReady
	.word	VDMHAL_V300R001_IsVdmRun
	.word	VDMHAL_V300R001_PrepareRepair
	.word	.LC2
	.word	s_RegPhyBaseAddr
	.word	.LC0
	.word	.LC1
	UNWIND(.fnend)
	.size	VDEC_InitWithOperation, .-VDEC_InitWithOperation
	.align	2
	.global	VDEC_Init
	.type	VDEC_Init, %function
VDEC_Init:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	ldr	r3, .L63
	mov	r4, r0
	mov	r2, #16
	mov	r1, #0
	sub	r0, fp, #36
	ldr	r3, [r3, #48]
	blx	r3
	sub	r0, fp, #36
	mov	r3, #0
	str	r4, [fp, #-32]
	str	r3, [fp, #-28]
	str	r3, [fp, #-24]
	bl	VDEC_InitWithOperation
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L64:
	.align	2
.L63:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDEC_Init, .-VDEC_Init
	.align	2
	.global	VDEC_Exit
	.type	VDEC_Exit, %function
VDEC_Exit:
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
	ldr	r4, .L73
	ldr	r3, [r4, #24]
	blx	r3
	cmp	r0, #0
	bne	.L72
.L66:
	ldr	r3, [r4, #424]
	ldr	r5, .L73
	sub	r3, r3, #1
	str	r3, [r4, #424]
	cmp	r3, #0
	ble	.L67
	ldr	r3, [r5, #28]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L67:
	bl	VCTRL_CloseVfmw
	mov	r4, r0
	bl	MEM_ManagerClearOperation
	mov	r2, #0
	ldr	r3, [r5, #28]
	str	r2, [r5, #420]
	blx	r3
	adds	r0, r4, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L72:
	ldr	r3, [r4, #68]
	mov	r0, #0
	ldr	r1, .L73+4
	blx	r3
	b	.L66
.L74:
	.align	2
.L73:
	.word	.LANCHOR0
	.word	.LC3
	UNWIND(.fnend)
	.size	VDEC_Exit, .-VDEC_Exit
	.align	2
	.global	GetVdecCapability
	.type	GetVdecCapability, %function
GetVdecCapability:
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
	ldr	r3, .L76
	mov	r2, #148
	mov	r1, #0
	mov	r4, r0
	ldr	r3, [r3, #48]
	blx	r3
	mov	r3, #0
	mov	r2, #50
	mov	r1, #4096
	movt	r3, 6480
	str	r2, [r4, #4]
	str	r1, [r4, #8]
	mov	r2, #2304
	mov	r1, #0
	str	r2, [r4, #12]
	str	r3, [r4, #16]
	mov	r2, #3
	mov	r3, #2
	str	r2, [r4, #24]
	str	r3, [r4, #28]
	mov	r2, #6
	mov	r3, #8
	str	r2, [r4, #32]
	str	r3, [r4, #36]
	mov	r2, #9
	mov	r3, #1
	str	r2, [r4, #40]
	str	r3, [r4, #44]
	mov	r2, #5
	mov	r3, #4
	str	r2, [r4, #48]
	str	r3, [r4, #52]
	mov	r2, #10
	mov	r3, #15
	str	r2, [r4, #56]
	str	r3, [r4, #60]
	mov	r2, #11
	mov	r3, #12
	str	r2, [r4, #64]
	str	r3, [r4, #68]
	mov	r2, #13
	mov	r3, #17
	str	r2, [r4, #72]
	str	r3, [r4, #80]
	mov	r2, #18
	mov	r3, #19
	str	r2, [r4, #84]
	str	r3, [r4, #88]
	mov	r2, #20
	mov	r3, #16
	str	r1, [r4, #20]
	str	r2, [r4, #92]
	str	r3, [r4]
	str	r3, [r4, #76]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L77:
	.align	2
.L76:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	GetVdecCapability, .-GetVdecCapability
	.align	2
	.global	VDEC_Control
	.type	VDEC_Control, %function
VDEC_Control:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
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
	ldr	r4, .L210
	mov	r6, r0
	mov	r7, r2
	ldr	r3, [r4, #420]
	cmp	r3, #1
	bne	.L183
	cmp	r1, #60
	ldrls	pc, [pc, r1, asl #2]
	b	.L81
.L83:
	.word	.L82
	.word	.L84
	.word	.L82
	.word	.L82
	.word	.L85
	.word	.L86
	.word	.L82
	.word	.L82
	.word	.L87
	.word	.L88
	.word	.L82
	.word	.L82
	.word	.L82
	.word	.L82
	.word	.L89
	.word	.L90
	.word	.L91
	.word	.L81
	.word	.L92
	.word	.L93
	.word	.L81
	.word	.L94
	.word	.L82
	.word	.L82
	.word	.L95
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L96
	.word	.L97
	.word	.L81
	.word	.L98
	.word	.L99
	.word	.L82
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L82
	.word	.L82
	.word	.L100
	.word	.L101
	.word	.L102
	.word	.L103
	.word	.L104
	.word	.L105
	.word	.L106
	.word	.L81
	.word	.L107
.L82:
	cmp	r1, #60
	ldrls	pc, [pc, r1, asl #2]
	b	.L81
.L134:
	.word	.L164
	.word	.L108
	.word	.L164
	.word	.L164
	.word	.L135
	.word	.L136
	.word	.L137
	.word	.L138
	.word	.L114
	.word	.L115
	.word	.L139
	.word	.L140
	.word	.L141
	.word	.L142
	.word	.L116
	.word	.L117
	.word	.L118
	.word	.L81
	.word	.L119
	.word	.L120
	.word	.L81
	.word	.L121
	.word	.L143
	.word	.L144
	.word	.L109
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L123
	.word	.L124
	.word	.L81
	.word	.L122
	.word	.L81
	.word	.L145
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L81
	.word	.L164
	.word	.L164
	.word	.L125
	.word	.L126
	.word	.L127
	.word	.L128
	.word	.L129
	.word	.L130
	.word	.L131
	.word	.L81
	.word	.L146
.L99:
	cmp	r2, #0
	bne	.L81
	mov	r0, r2
	ldr	r3, [r4, #68]
	movw	r2, #815
	ldr	r1, .L210+4
	blx	r3
.L81:
	mvn	r5, #0
.L173:
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L97:
	cmp	r2, #0
	beq	.L184
.L124:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_PutUsrdecFrame
.L176:
	adds	r0, r0, #0
	movne	r0, #1
.L177:
	rsb	r5, r0, #0
.L178:
	bl	VCTRL_EnableAllInt
	ldr	r3, [r4, #16]
	mov	r0, #0
	blx	r3
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L96:
	cmp	r2, #0
	beq	.L185
.L123:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r0, r6
	sub	r1, fp, #44
	bl	VCTRL_GetUsrdecFrame
	ldr	r2, [fp, #-44]
	ldr	r3, [fp, #-36]
	stmia	r7, {r2, r3}
	b	.L176
.L95:
	cmp	r2, #0
	beq	.L186
.L109:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r5, #0
	ldmia	r7, {r0, r1}
	bl	VCTRL_SetVdecExtra
	bl	VCTRL_EnableAllInt
	ldr	r3, [r4, #16]
	mov	r0, r5
	blx	r3
	b	.L173
.L98:
	cmp	r2, #0
	beq	.L187
.L122:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_SetDiscardPicParam
.L179:
	mov	r5, r0
	b	.L178
.L107:
	cmp	r2, #0
	beq	.L188
.L146:
	mov	r1, #1
	mov	r0, r6
	bl	VCTRT_SetChanCtrlState
	subs	r5, r0, #0
	bne	.L81
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_SetSingleFrameBufToChan
	subs	r7, r0, #0
	bne	.L161
.L181:
	ldr	r3, [r4, #12]
	blx	r3
	mov	r1, #2
	mov	r0, r6
	bl	VCTRL_SetChanFsPartitionState
	ldr	r3, [r4, #68]
	ldr	r1, .L210+8
	mov	r0, #31
	blx	r3
	ldr	r3, [r4, #16]
	mov	r0, r7
	blx	r3
.L162:
	mov	r0, r6
	mov	r1, #0
	bl	VCTRT_SetChanCtrlState
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r5, r0, #0
	b	.L173
.L94:
	cmp	r2, #0
	beq	.L189
.L121:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	ldr	r0, [r7]
	bl	VCTRL_GetChanIDByMemAddr
	cmn	r0, #1
	str	r0, [r7]
	movne	r0, #0
	moveq	r0, #1
	b	.L177
.L91:
	cmp	r2, #0
	beq	.L190
.L118:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_GetStreamSize
	b	.L179
.L90:
	cmp	r2, #0
	beq	.L191
.L117:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	ldr	r2, .L210+12
	ldr	r3, .L210+16
	mov	r5, #0
	str	r6, [r7]
	stmib	r7, {r2, r3}
	bl	VCTRL_EnableAllInt
	ldr	r3, [r4, #16]
	mov	r0, r5
	blx	r3
	b	.L173
.L89:
	cmp	r2, #0
	beq	.L192
.L116:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_SetStreamInterface
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r5, r0, #0
	bl	VCTRL_EnableAllInt
	ldr	r3, [r4, #16]
	mov	r0, #0
	blx	r3
	b	.L173
.L87:
	cmp	r2, #0
	beq	.L193
.L114:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_GetChanCfg
	b	.L176
.L103:
	cmp	r2, #0
	beq	.L194
.L128:
	ldr	r3, .L210+20
	mov	r5, #0
	ldr	r2, [r7]
	ldr	r3, [r3, r6, asl #2]
	str	r2, [r3, #1448]
	b	.L173
.L104:
	cmp	r2, #0
	beq	.L195
.L129:
	add	r3, r4, r6
	mov	r2, #1
	add	r4, r4, r6, lsl #2
	mov	r5, #0
	strb	r2, [r3, #404]
	ldr	r3, [r7]
	str	r3, [r4, #340]
	b	.L173
.L86:
	cmp	r2, #0
	beq	.L196
	ldr	r0, [r2]
	cmp	r0, #33
	bgt	.L197
.L113:
	mov	r3, #0
	mov	r2, #2
	ldr	r1, [r7, #4]
	bl	VCTRL_CreateChanWithOption
	mov	r5, r0, asr #31
	str	r0, [r7]
	b	.L173
.L85:
	cmp	r2, #0
	beq	.L198
	ldr	r0, [r2]
	cmp	r0, #33
	bgt	.L199
.L111:
	add	r1, r7, #4
	bl	VCTRL_CreateChan
	ldr	r3, [r4, #68]
	ldr	r1, .L210+24
	mov	r6, r0
	mov	r2, r0
	mov	r0, #2
	mov	r5, r6, asr #31
	blx	r3
	str	r6, [r7]
	b	.L173
.L105:
	cmp	r2, #0
	beq	.L200
.L130:
	add	r2, r4, r6
	add	r4, r4, r6, lsl #2
	mov	r3, #0
	mov	r5, r3
	strb	r3, [r2, #404]
	str	r3, [r4, #340]
	b	.L173
.L100:
	cmp	r2, #0
	beq	.L201
.L125:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	sub	r1, fp, #44
	mov	r0, r6
	mov	r3, #0
	mov	r2, #1
	str	r3, [fp, #-40]
	str	r2, [fp, #-44]
	str	r3, [fp, #-36]
	bl	VCTRL_ResetChanWithOption
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r5, r0, #0
	cmp	r5, #0
	bne	.L178
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_SeekPts
	b	.L176
.L93:
	cmp	r2, #0
	beq	.L202
.L120:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r3, #1
	sub	r2, fp, #44
	ldmia	r7, {r0, r1}
	bl	VCTRL_GetChanMemSizeWithOption
	cmp	r0, #0
	bne	.L166
	ldr	r1, [fp, #-44]
	mov	r5, r0
	ldr	r2, [fp, #-40]
	ldr	r3, [fp, #-36]
	stmia	r7, {r1, r2, r3}
	b	.L178
.L92:
	cmp	r2, #0
	beq	.L203
.L119:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	sub	r2, fp, #44
	sub	r1, fp, #56
	ldr	r0, [r7]
	bl	VCTRL_GetChanMemSize
	cmp	r0, #0
	bne	.L166
	ldr	r3, [fp, #-56]
	mov	r5, r0
	ldr	r1, [fp, #-44]
	ldr	r2, [fp, #-52]
	ldr	ip, [fp, #-40]
	add	r1, r3, r1
	ldr	r0, [fp, #-36]
	ldr	r3, [fp, #-48]
	add	r2, r2, ip
	stmia	r7, {r1, r2}
	add	r3, r3, r0
	str	r3, [r7, #8]
	b	.L178
.L101:
	cmp	r2, #0
	beq	.L204
.L126:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_ConfigFFFBSpeed
	b	.L176
.L102:
	cmp	r2, #0
	beq	.L205
.L127:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_ConfigPVRInfo
	b	.L176
.L88:
	cmp	r2, #0
	beq	.L206
.L115:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_ConfigChan
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r5, r0, #0
	cmp	r5, #0
	bne	.L178
	mov	r0, r6
	bl	VCTRL_LoadDspCode
	b	.L176
.L106:
	cmp	r2, #0
	beq	.L207
.L131:
	mov	r1, #1
	mov	r0, r6
	bl	VCTRT_SetChanCtrlState
	cmp	r0, #0
	bne	.L81
	mov	r0, r6
	bl	VCTRL_RlsAllFrameNode
	ldr	r3, [r4, #68]
	mov	r2, r6
	subs	r5, r0, #0
	beq	.L158
	ldr	r1, .L210+28
	mov	r0, #31
	blx	r3
	b	.L173
.L84:
	cmp	r2, #0
	beq	.L208
.L108:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r0, r7
	bl	GetVdecCapability
.L174:
	mov	r5, #0
	bl	VCTRL_EnableAllInt
	ldr	r3, [r4, #16]
	mov	r0, r5
	blx	r3
	b	.L173
.L164:
	mov	r5, #0
	b	.L173
.L138:
	mov	r5, #500
	b	.L147
.L151:
	ldr	r3, [r4, #120]
	sub	r5, r5, #1
	blx	r3
.L147:
	mov	r1, #2
	mov	r0, r6
	bl	VCTRT_SetChanCtrlState
	adds	r3, r5, #0
	movne	r3, #1
	cmn	r0, #1
	mov	r0, #1
	movne	r3, #0
	cmp	r3, #0
	bne	.L151
	cmp	r5, #0
	ble	.L81
	mov	r0, r6
	bl	VCTRL_DestroyChanWithOption
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r5, r0, #0
	b	.L173
.L137:
	mov	r5, #500
	b	.L148
.L149:
	ldr	r3, [r4, #120]
	sub	r5, r5, #1
	blx	r3
.L148:
	mov	r1, #2
	mov	r0, r6
	bl	VCTRT_SetChanCtrlState
	adds	r3, r5, #0
	movne	r3, #1
	cmn	r0, #1
	mov	r0, #1
	movne	r3, #0
	cmp	r3, #0
	bne	.L149
	cmp	r5, #0
	ble	.L209
	mov	r0, r6
	bl	VCTRL_DestroyChan
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r5, r0, #0
	b	.L173
.L140:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r0, r6
	bl	VCTRL_StartChan
	b	.L176
.L139:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_GetChanState
	b	.L174
.L135:
	ldr	r0, [r2]
	b	.L111
.L144:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r1, r7
	mov	r0, r6
	bl	VCTRL_ResetChanWithOption
	b	.L176
.L143:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r0, r6
	bl	VCTRL_ReleaseStream
	b	.L176
.L145:
	ldr	r3, [r4, #12]
	mov	r5, #0
	mov	r0, #0
	blx	r3
	bl	VCTRL_ExtraWakeUpThread
	bl	VCTRL_EnableAllInt
	ldr	r3, [r4, #16]
	mov	r0, r5
	blx	r3
	b	.L173
.L136:
	ldr	r0, [r2]
	b	.L113
.L142:
	ldr	r3, [r4, #12]
	mov	r0, #0
	blx	r3
	mov	r0, r6
	bl	VCTRL_ResetChan
	b	.L176
.L141:
	bl	VCTRL_StopChanWithCheck
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r5, r0, #0
	b	.L173
.L183:
	ldr	r3, [r4, #68]
	mov	r0, #0
	ldr	r1, .L210+32
	mvn	r5, #0
	blx	r3
	b	.L173
.L166:
	mvn	r5, #0
	b	.L178
.L197:
	ldr	r3, [r4, #68]
	movw	r2, #642
	ldr	r1, .L210+36
	mov	r0, #0
	blx	r3
	b	.L81
.L199:
	ldr	r3, [r4, #68]
	movw	r2, #623
	ldr	r1, .L210+36
	mov	r0, #0
	blx	r3
	b	.L81
.L209:
	ldr	ip, [r4, #68]
	mov	r0, r3
	mvn	r2, #0
	ldr	r1, .L210+40
	blx	ip
	mvn	r5, #0
	b	.L173
.L158:
	ldr	r1, .L210+44
	mov	r0, #31
	blx	r3
	mov	r1, r7
	mov	r0, r6
	ldr	r4, .L210
	bl	VCTRL_SetFsParamToChan
	mov	r0, r6
	bl	VCTRL_RequestChanMem
	subs	r7, r0, #0
	beq	.L181
.L161:
	mov	r0, r6
	bl	VCTRL_GetCurChanFsSize
	cmp	r0, #0
	beq	.L162
	mov	r2, r6
	ldr	r3, [r4, #68]
	ldr	r1, .L210+48
	mov	r0, #31
	blx	r3
	ldr	r3, [r4, #12]
	mov	r0, r5
	blx	r3
	mov	r0, r6
	mov	r1, #3
	bl	VCTRL_SetChanFsPartitionState
	ldr	r3, [r4, #68]
	ldr	r1, .L210+52
	mov	r0, #31
	blx	r3
	ldr	r3, [r4, #16]
	mov	r0, r5
	blx	r3
	b	.L162
.L207:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	mov	r2, #852
	blx	r3
	b	.L81
.L206:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #667
	blx	r3
	b	.L81
.L191:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #705
	blx	r3
	b	.L81
.L188:
	ldr	r3, [r4, #68]
	movw	r2, #862
	ldr	r1, .L210+56
	mov	r0, r7
	blx	r3
	b	.L146
.L203:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #723
	blx	r3
	b	.L81
.L186:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+60
	mov	r2, #604
	blx	r3
	b	.L81
.L185:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #767
	blx	r3
	b	.L81
.L205:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #803
	blx	r3
	b	.L81
.L204:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #794
	blx	r3
	b	.L81
.L202:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	mov	r2, #732
	blx	r3
	b	.L81
.L200:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #842
	blx	r3
	b	.L81
.L208:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #589
	blx	r3
	b	.L81
.L195:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #833
	blx	r3
	b	.L81
.L190:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #714
	blx	r3
	b	.L81
.L189:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #741
	blx	r3
	b	.L81
.L184:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	mov	r2, #776
	blx	r3
	b	.L81
.L187:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #751
	blx	r3
	b	.L81
.L194:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	mov	r2, #824
	blx	r3
	b	.L81
.L201:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #785
	blx	r3
	b	.L81
.L196:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #633
	blx	r3
	b	.L81
.L198:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #614
	blx	r3
	b	.L81
.L193:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	movw	r2, #658
	blx	r3
	b	.L81
.L192:
	ldr	r3, [r4, #68]
	mov	r0, r2
	ldr	r1, .L210+4
	mov	r2, #696
	blx	r3
	b	.L81
.L211:
	.align	2
.L210:
	.word	.LANCHOR0
	.word	.LC5
	.word	.LC13
	.word	VCTRL_GetChanImage
	.word	VCTRL_ReleaseChanImage
	.word	s_pstVfmwChan
	.word	.LC9
	.word	.LC11
	.word	.LC4
	.word	.LC7
	.word	.LC10
	.word	.LC12
	.word	.LC14
	.word	.LC15
	.word	.LC8
	.word	.LC6
	UNWIND(.fnend)
	.size	VDEC_Control, .-VDEC_Control
	.align	2
	.global	VDEC_Suspend
	.type	VDEC_Suspend, %function
VDEC_Suspend:
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
	bl	VCTRL_Suspend
	ldr	r3, .L213
	ldr	r1, .L213+4
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L214:
	.align	2
.L213:
	.word	.LANCHOR0
	.word	.LC16
	UNWIND(.fnend)
	.size	VDEC_Suspend, .-VDEC_Suspend
	.align	2
	.global	VDEC_Resume
	.type	VDEC_Resume, %function
VDEC_Resume:
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
	bl	VCTRL_Resume
	ldr	r3, .L216
	ldr	r1, .L216+4
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L217:
	.align	2
.L216:
	.word	.LANCHOR0
	.word	.LC17
	UNWIND(.fnend)
	.size	VDEC_Resume, .-VDEC_Resume
	.global	g_LowDelaySeqIndex
	.global	g_LowDelayCalcHandle
	.global	g_LowDelayCalcStat
	.global	g_vdm_hal_fun_ptr
	.global	g_ChipMaxHeight
	.global	g_ChipMaxWidth
	.global	g_VfmwInitCount
	.global	g_VfmwEnableFlag
	.global	vfmw_Osal_Func_Ptr_S
	.data
	.align	2
	.type	g_ChipMaxHeight, %object
	.size	g_ChipMaxHeight, 4
g_ChipMaxHeight:
	.word	8192
	.type	g_ChipMaxWidth, %object
	.size	g_ChipMaxWidth, 4
g_ChipMaxWidth:
	.word	8192
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"VDMHAL_NULL_FUN_PRINT,L%d\012\000" )
	.space	1
.LC1:
	ASCII(.ascii	"VDEC_InitWithOperation has NULL parameter, error!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC2:
	ASCII(.ascii	"VCTRL_OpenVfmw() return %d\012\000" )
.LC3:
	ASCII(.ascii	"OSAL_DOWN_INTERRUPTIBLE ERR\012\000" )
	.space	3
.LC4:
	ASCII(.ascii	"VFMW has been destroyed, so any operation is unusef" )
	ASCII(.ascii	"ul!\012\000" )
.LC5:
	ASCII(.ascii	"vfmw.c,L%d: pArgs is NULL!\012\000" )
.LC6:
	ASCII(.ascii	"vfmw.c, L%d: pArgs is NULL!\012\000" )
	.space	3
.LC7:
	ASCII(.ascii	"vfmw.c,L%d: capability level out of range\012\000" )
	.space	1
.LC8:
	ASCII(.ascii	"vfmw.c, Line(%d): pArgs is NULL!\012\000" )
	.space	2
.LC9:
	ASCII(.ascii	"VCTRL_CreateChan() return %d\012\000" )
	.space	2
.LC10:
	ASCII(.ascii	"VCTRT_SetChanCtrlState ERR: %d\012\000" )
.LC11:
	ASCII(.ascii	"DFS, rls chan %d frame store fail!\012\000" )
.LC12:
	ASCII(.ascii	"DFS, rls chan %d frame store success!\012\000" )
	.space	1
.LC13:
	ASCII(.ascii	"DFS, set decoding thread state to success!\012 \000" )
	.space	3
.LC14:
	ASCII(.ascii	"DFS, request frame store fail!, chanid: %d\012\000" )
.LC15:
	ASCII(.ascii	"DFS, set decoding thread state to fail\012\000" )
.LC16:
	ASCII(.ascii	"VDEC_Suspend ok !\012\000" )
	.space	1
.LC17:
	ASCII(.ascii	"VDEC_Resume ok !\012\000" )
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	vfmw_Osal_Func_Ptr_S, %object
	.size	vfmw_Osal_Func_Ptr_S, 216
vfmw_Osal_Func_Ptr_S:
	.space	216
	.type	g_vdm_hal_fun_ptr, %object
	.size	g_vdm_hal_fun_ptr, 124
g_vdm_hal_fun_ptr:
	.space	124
	.type	g_LowDelayCalcHandle, %object
	.size	g_LowDelayCalcHandle, 64
g_LowDelayCalcHandle:
	.space	64
	.type	g_LowDelayCalcStat, %object
	.size	g_LowDelayCalcStat, 16
g_LowDelayCalcStat:
	.space	16
	.type	g_VfmwEnableFlag, %object
	.size	g_VfmwEnableFlag, 4
g_VfmwEnableFlag:
	.space	4
	.type	g_VfmwInitCount, %object
	.size	g_VfmwInitCount, 4
g_VfmwInitCount:
	.space	4
	.type	g_LowDelaySeqIndex, %object
	.size	g_LowDelaySeqIndex, 64
g_LowDelaySeqIndex:
	.space	64
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
