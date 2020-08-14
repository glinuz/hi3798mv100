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
	.file	"bitplane.c"
	.text
	.align	2
	.global	BPD_CfgReg
	.type	BPD_CfgReg, %function
BPD_CfgReg:
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
	ldr	ip, [r3, #8]
	mov	r4, r0
	mov	r0, #0
	cmp	ip, r0
	str	r0, [fp, #-48]
	beq	.L31
.L2:
	ldr	r0, .L36
	mvn	ip, #0
	mvn	r3, #1
	ldr	lr, [r0, #8]
	str	ip, [lr, #68]
	str	r3, [lr, #68]
	ldr	r5, [r1, #24]
	ldr	r3, [r1, #20]
	ldrb	ip, [r4, #3096]	@ zero_extendqisi2
	rsb	r3, r5, r3
	ldr	r1, [r1]
	ubfx	r3, r3, #3, #2
	cmp	ip, #0
	add	r3, r3, r5, lsr #3
	add	r1, r1, r3
	beq	.L5
	ldrb	r3, [r2, #362]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L6
	movw	r3, #362
	ldrh	r3, [r2, r3]
	cmp	r3, #4
	beq	.L6
	ldrb	r3, [r2, #197]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L32
.L8:
	ldrb	r3, [r2, #349]	@ zero_extendqisi2
	ldr	r6, [r2, #392]
	cmp	r3, #4
	ldrb	r7, [r2, #16]	@ zero_extendqisi2
	andne	ip, r3, #3
	ldrb	r3, [fp, #-46]	@ zero_extendqisi2
	moveq	ip, #3
	cmp	r6, #3
	bfi	r3, r6, #0, #2
	ldrb	r6, [fp, #-47]	@ zero_extendqisi2
	bfi	r3, r7, #2, #2
	strb	r3, [fp, #-46]
	bfi	r6, ip, #6, #2
	strb	r6, [fp, #-47]
	beq	.L33
.L11:
	add	ip, r4, #20480
	movw	r6, #21846
	movt	r6, 21845
	ldrb	r8, [fp, #-47]	@ zero_extendqisi2
	ldr	r3, [ip, #3476]
	smull	r6, r7, r3, r6
	sub	r6, r7, r3, asr #31
	add	r6, r6, r6, lsl #1
	rsb	r3, r6, r3
	bfi	r8, r3, #2, #2
	strb	r8, [fp, #-47]
.L15:
	ldr	r9, [ip, #3472]
	movw	r6, #21846
	movt	r6, 21845
	add	r3, r4, #32768
	ldrb	r8, [fp, #-46]	@ zero_extendqisi2
	and	r5, r5, #7
	smull	r6, r7, r9, r6
	ldr	r6, [r3, #3564]
	ldr	r10, [r3, #3576]
	and	r8, r8, #239
	rsb	r6, r6, r1
	sub	r7, r7, r9, asr #31
	add	r6, r6, r10
	ldrb	r10, [fp, #-47]	@ zero_extendqisi2
	and	r6, r6, #15
	add	r7, r7, r7, lsl #1
	rsb	r9, r7, r9
	orr	r8, r8, #32
	add	r5, r5, r6, lsl #3
	bfi	r10, r9, #0, #2
	strb	r5, [fp, #-48]
	strb	r10, [fp, #-47]
	strb	r8, [fp, #-46]
	ldr	r5, [fp, #-48]
	str	r5, [lr, #4]
	ldr	r6, [r3, #3564]
	ldr	r5, [r3, #3576]
	ldr	lr, [r0, #8]
	rsb	r1, r6, r1
	add	r1, r1, r5
	bic	r1, r1, #15
	str	r1, [lr, #8]
	ldrb	r1, [r4, #3096]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L16
	ldr	r2, [r2, #392]
	cmp	r2, #3
	beq	.L34
.L16:
	ldr	r2, [ip, #3476]
	sub	r2, r2, #1
	strh	r2, [fp, #-46]	@ movhi
.L17:
	ldr	r1, [ip, #3472]
	mov	r4, #3
	ldr	r2, [r0, #8]
	mov	lr, #0
	sub	r1, r1, #1
	strh	r1, [fp, #-48]	@ movhi
	ldr	r1, [fp, #-48]
	mov	r5, #0
	str	lr, [fp, #-48]
	bfi	r5, r4, #4, #4
	mov	r0, lr
	str	r1, [r2, #12]
	ldr	ip, [ip, #3472]
	adds	r1, ip, #127
	addmi	r1, ip, #254
	mov	r1, r1, lsr r4
	and	r1, r1, #4080
	strh	r1, [fp, #-48]	@ movhi
	ldr	r1, [fp, #-48]
	str	lr, [fp, #-48]
	strb	r5, [fp, #-48]
	str	r1, [r2, #16]
	ldr	ip, [r3, #3648]
	strb	r4, [fp, #-47]
	ldr	r1, [fp, #-48]
	str	ip, [r2, #20]
	ldr	ip, [r3, #3652]
	str	ip, [r2, #24]
	ldr	ip, [r3, #3656]
	str	ip, [r2, #28]
	ldr	ip, [r3, #3660]
	str	ip, [r2, #32]
	ldr	ip, [r3, #3664]
	str	ip, [r2, #36]
	ldr	ip, [r3, #3668]
	str	ip, [r2, #40]
	ldr	r3, [r3, #3672]
	str	r3, [r2, #44]
	str	r1, [r2, #48]
.L4:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L5:
	ldrb	r3, [r2, #259]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L12
	cmp	r3, #4
	beq	.L35
.L13:
	ldrb	r3, [r2, #251]	@ zero_extendqisi2
	ldrb	r6, [fp, #-47]	@ zero_extendqisi2
	cmp	r3, #4
	ldrb	r7, [r2]	@ zero_extendqisi2
	andne	ip, r3, #3
	ldrb	r3, [fp, #-46]	@ zero_extendqisi2
	moveq	ip, #3
	bfi	r6, ip, #6, #2
	and	r3, r3, #252
	strb	r6, [fp, #-47]
	bfi	r3, r7, #2, #2
	strb	r3, [fp, #-46]
	b	.L11
.L6:
	ldrb	r3, [fp, #-47]	@ zero_extendqisi2
	orr	r3, r3, #16
	strb	r3, [fp, #-47]
	ldrb	r3, [r2, #197]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L8
.L32:
	ldr	r3, [r2, #464]
	cmp	r3, #8
	ldrleb	r3, [fp, #-47]	@ zero_extendqisi2
	orrle	r3, r3, #32
	strleb	r3, [fp, #-47]
	b	.L8
.L35:
	ldrb	r3, [r2, #260]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L13
.L12:
	ldrb	r3, [fp, #-47]	@ zero_extendqisi2
	orr	r3, r3, #16
	strb	r3, [fp, #-47]
	b	.L13
.L34:
	ldr	r2, [ip, #3476]
	add	r2, r2, #1
	add	r2, r2, r2, lsr #31
	mov	r2, r2, asr #1
	sub	r2, r2, #1
	strh	r2, [fp, #-46]	@ movhi
	b	.L17
.L33:
	add	ip, r4, #20480
	movw	r6, #21846
	movt	r6, 21845
	ldrb	r8, [fp, #-47]	@ zero_extendqisi2
	ldr	r3, [ip, #3476]
	add	r3, r3, #1
	add	r3, r3, r3, lsr #31
	mov	r9, r3, asr #1
	smull	r6, r7, r9, r6
	sub	r3, r7, r3, asr #31
	add	r3, r3, r3, lsl #1
	rsb	r3, r3, r9
	bfi	r8, r3, #2, #2
	strb	r8, [fp, #-47]
	b	.L15
.L31:
	mov	r0, #53248
	str	r3, [fp, #-64]
	movt	r0, 63683
	str	r2, [fp, #-60]
	str	r1, [fp, #-56]
	bl	MEM_Phy2Vir
	subs	ip, r0, #0
	beq	.L3
	ldr	r3, [fp, #-64]
	ldr	r1, [fp, #-56]
	ldr	r2, [fp, #-60]
	str	ip, [r3, #8]
	b	.L2
.L3:
	ldr	r1, .L36+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4
.L37:
	.align	2
.L36:
	.word	g_HwMem
	.word	.LC0
	UNWIND(.fnend)
	.size	BPD_CfgReg, .-BPD_CfgReg
	.align	2
	.global	BPD_Reset
	.type	BPD_Reset, %function
BPD_Reset:
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
	mov	r0, #53248
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r2, r0, #0
	beq	.L53
	ldr	r5, .L56
	ldr	r2, [r5]
	ldr	r4, [r2, #136]
	orr	r4, r4, #16
	str	r4, [r2, #136]
	ldr	r3, [r2, #372]
	tst	r3, #8
	bne	.L40
	movw	r0, #9999
	b	.L41
.L55:
	subs	r0, r0, #1
	beq	.L54
.L41:
	ldr	r3, [r2, #372]
	tst	r3, #8
	beq	.L55
.L40:
	ldr	r3, .L56+4
	bfc	r4, #4, #1
	mvn	r1, #1
	str	r4, [r2, #136]
	ldr	r3, [r3, #8]
	str	r1, [r3, #52]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L54:
	ldr	r1, .L56+8
	bl	dprint_vfmw
	ldr	r2, [r5]
	b	.L40
.L53:
	ldr	r1, .L56+12
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L57:
	.align	2
.L56:
	.word	g_pstRegCrg
	.word	g_HwMem
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	BPD_Reset, .-BPD_Reset
	.align	2
	.global	BPD_Start
	.type	BPD_Start, %function
BPD_Start:
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
	ldr	r3, .L59
	mov	r2, #0
	mov	r0, #1
	ldr	r1, [r3, #8]
	str	r2, [r1]
	ldr	r1, [r3, #8]
	str	r0, [r1]
	ldr	r3, [r3, #8]
	str	r2, [r3]
	ldmfd	sp, {fp, sp, pc}
.L60:
	.align	2
.L59:
	.word	g_HwMem
	UNWIND(.fnend)
	.size	BPD_Start, .-BPD_Start
	.align	2
	.global	IsBpd_Ready
	.type	IsBpd_Ready, %function
IsBpd_Ready:
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
	ldr	r3, .L65
	ldr	r0, [r3, #8]
	cmp	r0, #0
	beq	.L64
	ldr	r0, [r0, #64]
	and	r0, r0, #1
	ldmfd	sp, {fp, sp, pc}
.L64:
	ldr	r3, .L65+4
	ldr	r2, .L65+8
	ldr	r1, .L65+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L66:
	.align	2
.L65:
	.word	g_HwMem
	.word	.LC3
	.word	.LANCHOR0
	.word	.LC4
	UNWIND(.fnend)
	.size	IsBpd_Ready, .-IsBpd_Ready
	.align	2
	.global	BPDDRV_WaitBpdReadyIfNoIsr
	.type	BPDDRV_WaitBpdReadyIfNoIsr, %function
BPDDRV_WaitBpdReadyIfNoIsr:
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
	ldr	r6, .L75
	mov	r4, #0
	ldr	r3, [r6]
	blx	r3
	mov	r5, r0
	b	.L70
.L74:
	ldr	r3, [r6]
	blx	r3
	cmp	r0, r5
	movcc	r5, #0
	rsb	r4, r5, r0
	cmp	r4, #4000
	bcs	.L72
.L70:
	bl	IsBpd_Ready
	cmp	r0, #0
	beq	.L74
	cmp	r4, #4000
	bcs	.L72
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L72:
	ldr	r1, .L75+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L76:
	.align	2
.L75:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC5
	UNWIND(.fnend)
	.size	BPDDRV_WaitBpdReadyIfNoIsr, .-BPDDRV_WaitBpdReadyIfNoIsr
	.align	2
	.global	BPD_GetParam
	.type	BPD_GetParam, %function
BPD_GetParam:
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
	ldr	ip, .L84
	ldr	r4, [r2, #16]
	ldr	r3, [r2, #24]
	ldr	ip, [ip, #8]
	rsb	r5, r3, r4, lsl #3
	ldr	lr, [ip, #80]
	ldr	ip, [ip, #84]
	cmp	lr, r5
	bcs	.L82
	ldrb	r3, [r0, #3096]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L83
	and	r0, ip, #15
	ubfx	r3, ip, #8, #4
	str	r0, [r1, #328]
	ubfx	ip, ip, #4, #4
	str	r3, [r1, #332]
	str	ip, [r1, #336]
.L81:
	mov	r1, lr
	mov	r0, r2
	bl	BsLongSkip
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L83:
	ubfx	r3, ip, #0, #4
	ubfx	r0, ip, #12, #4
	strb	r3, [r1, #382]
	ubfx	r3, ip, #16, #4
	strb	r0, [r1, #385]
	ubfx	r0, ip, #20, #4
	strb	r3, [r1, #386]
	ubfx	r3, ip, #8, #4
	strb	r0, [r1, #387]
	ubfx	r0, ip, #24, #4
	strb	r3, [r1, #383]
	ubfx	r3, ip, #4, #4
	strb	r0, [r1, #388]
	ubfx	ip, ip, #28, #2
	strb	r3, [r1, #384]
	strb	ip, [r1, #378]
	b	.L81
.L82:
	rsb	r3, r3, r4
	mov	r2, lr
	ldr	r1, .L84+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L85:
	.align	2
.L84:
	.word	g_HwMem
	.word	.LC6
	UNWIND(.fnend)
	.size	BPD_GetParam, .-BPD_GetParam
	.align	2
	.global	BPD_Drv
	.type	BPD_Drv, %function
BPD_Drv:
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
	UNWIND(.pad #24)
	sub	sp, sp, #24
	add	ip, r1, #20480
	mov	r7, r0
	mov	r0, #2
	ldr	r3, [ip, #3476]
	mov	r6, r2
	mov	r5, r1
	mov	r2, #0
	cmp	r3, r0
	str	r2, [fp, #-44]
	str	r0, [fp, #-48]
	mov	r2, #1
	ldr	ip, [ip, #3472]
	str	r2, [fp, #-40]
	ble	.L88
	cmp	r3, #128
	sub	r4, ip, #3
	movle	r1, #0
	movgt	r1, #1
	cmp	r4, #125
	movls	r4, r1
	orrhi	r4, r1, #1
	cmp	r4, #0
	bne	.L88
	ldr	r8, .L100
	ldrb	r3, [r8]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L91
	mov	r1, r4
	str	r2, [sp]
	mov	r3, #400
	bl	VDH_Acquire_Usage
	mov	r0, r4
	sub	r1, fp, #48
	bl	VDH_Set_Parameter
.L91:
	ldr	r9, .L100+4
	bl	BPD_Reset
	mov	r2, r6
	mov	r1, r7
	mov	r0, r5
	mov	r3, r9
	bl	BPD_CfgReg
	cmp	r0, #0
	bne	.L92
	ldr	r3, [r9, #8]
	mov	r4, #1
	str	r0, [r3]
	ldr	r3, [r9, #8]
	str	r4, [r3]
	ldr	r3, [r9, #8]
	str	r0, [r3]
	bl	BPDDRV_WaitBpdReadyIfNoIsr
	cmp	r0, #0
	beq	.L99
.L93:
	ldrb	r3, [r8]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r0, r4
	beq	.L90
	mov	r5, #0
	sub	r1, fp, #48
	mov	r0, r5
	str	r5, [fp, #-40]
	bl	VDH_Set_Parameter
	mov	r1, r5
	mov	r2, #1
	mov	r0, #2
	bl	VDH_Loose_Usage
	mov	r0, r4
	b	.L90
.L88:
	mov	r3, r3, asl #4
	mov	r2, ip, asl #4
	ldr	r1, .L100+8
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #1
.L90:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L92:
	ldr	r1, .L100+12
	mov	r0, #0
	bl	dprint_vfmw
	mov	r4, #1
	b	.L93
.L99:
	mov	r2, r7
	mov	r1, r6
	mov	r0, r5
	bl	BPD_GetParam
	adds	r4, r0, #0
	movne	r4, #1
	b	.L93
.L101:
	.align	2
.L100:
	.word	g_HalDisable
	.word	g_HwMem
	.word	.LC7
	.word	.LC8
	UNWIND(.fnend)
	.size	BPD_Drv, .-BPD_Drv
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.46646, %object
	.size	__func__.46646, 12
__func__.46646:
	.ascii	"IsBpd_Ready\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"BPD register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
.LC1:
	ASCII(.ascii	"VDMHAL_ResetBPD: map BPD register fail, vir(reg) = " )
	ASCII(.ascii	"(%p)\012\000" )
	.space	3
.LC2:
	ASCII(.ascii	"Reset BPD Failed!\012\000" )
	.space	1
.LC3:
	ASCII(.ascii	"BPD register not mapped yet!\000" )
	.space	3
.LC4:
	ASCII(.ascii	"%s: %s\012\000" )
.LC5:
	ASCII(.ascii	"BPD TimeOut!\012\000" )
	.space	2
.LC6:
	ASCII(.ascii	" BPD ERROR: EAT TOO MUCH BS %#x, but available bs n" )
	ASCII(.ascii	"um %#x!!!\012\000" )
	.space	2
.LC7:
	ASCII(.ascii	"NOT SUPPORT SIZE %d*%d\012\000" )
.LC8:
	ASCII(.ascii	"BPD error: no map reg!\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
