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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #24)
	sub	sp, sp, #24
	ldr	ip, [r3, #8]
	mov	r4, r0
	mov	r0, #0
	cmp	ip, r0
	str	r0, [fp, #-40]
	beq	.L31
.L2:
	ldr	lr, .L36
	mvn	ip, #0
	mvn	r0, #1
	ldr	r3, [lr, #8]
	str	ip, [r3, #68]
	ldr	r3, [lr, #8]
	str	r0, [r3, #68]
	ldr	r0, [r1, #24]
	ldr	r6, [r1, #20]
	adds	ip, r0, #31
	add	r3, r0, #7
	addmi	ip, r0, #62
	cmp	r0, #0
	bic	ip, ip, #31
	ldrb	r5, [r4, #3096]	@ zero_extendqisi2
	add	ip, ip, r6
	ldr	r6, [r1]
	rsb	r1, r0, ip
	movge	r3, r0
	mov	ip, r3, asr #3
	mov	r3, r1, asr #31
	mov	r3, r3, lsr #27
	add	r1, r1, r3
	and	r1, r1, #31
	rsb	r3, r3, r1
	cmp	r3, #0
	add	r1, r3, #7
	movlt	r3, r1
	cmp	r5, #0
	add	r3, ip, r3, asr #3
	add	r3, r6, r3
	beq	.L5
	ldrb	r1, [r2, #362]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L6
	movw	r1, #362
	ldrh	r1, [r2, r1]
	cmp	r1, #4
	beq	.L6
	ldrb	r1, [r2, #197]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L32
.L8:
	ldrb	r1, [r2, #349]	@ zero_extendqisi2
	ldr	r5, [r2, #392]
	cmp	r1, #4
	ldrb	r6, [r2, #16]	@ zero_extendqisi2
	andne	ip, r1, #3
	ldrb	r1, [fp, #-38]	@ zero_extendqisi2
	moveq	ip, #3
	cmp	r5, #3
	bfi	r1, r5, #0, #2
	ldrb	r5, [fp, #-39]	@ zero_extendqisi2
	bfi	r1, r6, #2, #2
	strb	r1, [fp, #-38]
	bfi	r5, ip, #6, #2
	strb	r5, [fp, #-39]
	beq	.L33
.L11:
	add	ip, r4, #20480
	movw	r6, #21846
	movt	r6, 21845
	ldrb	r8, [fp, #-39]	@ zero_extendqisi2
	ldr	r1, [ip, #2756]
	smull	r6, r7, r1, r6
	sub	r5, r7, r1, asr #31
	add	r5, r5, r5, lsl #1
	rsb	r1, r5, r1
	bfi	r8, r1, #2, #2
	strb	r8, [fp, #-39]
.L15:
	ldr	r8, [ip, #2752]
	movw	r6, #21846
	movt	r6, 21845
	add	r1, r4, #32768
	ldrb	r9, [fp, #-39]	@ zero_extendqisi2
	and	r0, r0, #7
	smull	r6, r7, r8, r6
	ldr	r5, [r1, #2844]
	ldr	r6, [r1, #2856]
	rsb	r5, r5, r3
	sub	r7, r7, r8, asr #31
	add	r5, r5, r6
	ldr	r6, [lr, #8]
	and	r5, r5, #15
	add	r7, r7, r7, lsl #1
	rsb	r8, r7, r8
	add	r0, r0, r5, lsl #3
	bfi	r9, r8, #0, #2
	strb	r0, [fp, #-40]
	strb	r9, [fp, #-39]
	ldr	r0, [fp, #-40]
	str	r0, [r6, #4]
	ldr	r6, [r1, #2844]
	ldr	r5, [r1, #2856]
	ldr	r0, [lr, #8]
	rsb	r3, r6, r3
	add	r3, r3, r5
	bic	r3, r3, #15
	str	r3, [r0, #8]
	ldrb	r3, [r4, #3096]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L16
	ldr	r3, [r2, #392]
	cmp	r3, #3
	beq	.L34
.L16:
	ldr	r3, [ip, #2756]
	sub	r3, r3, #1
	strh	r3, [fp, #-38]	@ movhi
.L17:
	ldr	r3, [ip, #2752]
	mov	r4, #3
	ldr	r6, [lr, #8]
	mov	r2, #0
	sub	r3, r3, #1
	strh	r3, [fp, #-40]	@ movhi
	ldr	r3, [fp, #-40]
	mov	r0, r2
	str	r2, [fp, #-40]
	mov	r5, #0
	bfi	r5, r4, #4, #4
	str	r3, [r6, #12]
	ldr	ip, [ip, #2752]
	ldr	r6, [lr, #8]
	adds	r3, ip, #127
	addmi	r3, ip, #254
	mov	r3, r3, lsr r4
	and	r3, r3, #4080
	strh	r3, [fp, #-40]	@ movhi
	ldr	r3, [fp, #-40]
	str	r2, [fp, #-40]
	str	r3, [r6, #16]
	ldr	r3, [lr, #8]
	ldr	r2, [r1, #2928]
	str	r2, [r3, #20]
	ldr	r3, [lr, #8]
	ldr	r2, [r1, #2932]
	str	r2, [r3, #24]
	ldr	r3, [lr, #8]
	ldr	r2, [r1, #2936]
	str	r2, [r3, #28]
	ldr	r3, [lr, #8]
	ldr	r2, [r1, #2940]
	str	r2, [r3, #32]
	ldr	r3, [lr, #8]
	ldr	r2, [r1, #2944]
	str	r2, [r3, #36]
	ldr	r3, [lr, #8]
	ldr	r2, [r1, #2948]
	str	r2, [r3, #40]
	ldr	r2, [r1, #2952]
	ldr	r3, [lr, #8]
	str	r2, [r3, #44]
	strb	r5, [fp, #-40]
	strb	r4, [fp, #-39]
	ldr	r3, [lr, #8]
	ldr	r2, [fp, #-40]
	str	r2, [r3, #48]
.L4:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L5:
	ldrb	r1, [r2, #259]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L12
	cmp	r1, #4
	beq	.L35
.L13:
	ldrb	r1, [r2, #251]	@ zero_extendqisi2
	ldrb	r5, [fp, #-39]	@ zero_extendqisi2
	cmp	r1, #4
	ldrb	r6, [r2]	@ zero_extendqisi2
	andne	ip, r1, #3
	ldrb	r1, [fp, #-38]	@ zero_extendqisi2
	moveq	ip, #3
	bfi	r5, ip, #6, #2
	and	r1, r1, #252
	strb	r5, [fp, #-39]
	bfi	r1, r6, #2, #2
	strb	r1, [fp, #-38]
	b	.L11
.L6:
	ldrb	r1, [fp, #-39]	@ zero_extendqisi2
	orr	r1, r1, #16
	strb	r1, [fp, #-39]
	ldrb	r1, [r2, #197]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L8
.L32:
	ldr	r1, [r2, #464]
	cmp	r1, #8
	ldrleb	r1, [fp, #-39]	@ zero_extendqisi2
	orrle	r1, r1, #32
	strleb	r1, [fp, #-39]
	b	.L8
.L35:
	ldrb	r1, [r2, #260]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L13
.L12:
	ldrb	r1, [fp, #-39]	@ zero_extendqisi2
	orr	r1, r1, #16
	strb	r1, [fp, #-39]
	b	.L13
.L34:
	ldr	r3, [ip, #2756]
	add	r3, r3, #1
	add	r3, r3, r3, lsr #31
	mov	r3, r3, asr #1
	sub	r3, r3, #1
	strh	r3, [fp, #-38]	@ movhi
	b	.L17
.L33:
	add	ip, r4, #20480
	movw	r6, #21846
	movt	r6, 21845
	ldrb	r5, [fp, #-39]	@ zero_extendqisi2
	ldr	r1, [ip, #2756]
	add	r1, r1, #1
	add	r1, r1, r1, lsr #31
	mov	r8, r1, asr #1
	smull	r6, r7, r8, r6
	sub	r1, r7, r1, asr #31
	add	r1, r1, r1, lsl #1
	rsb	r1, r1, r8
	bfi	r5, r1, #2, #2
	strb	r5, [fp, #-39]
	b	.L15
.L31:
	movt	r0, 63686
	str	r3, [fp, #-56]
	str	r2, [fp, #-52]
	str	r1, [fp, #-48]
	bl	MEM_Phy2Vir
	subs	ip, r0, #0
	beq	.L3
	ldr	r3, [fp, #-56]
	ldr	r1, [fp, #-48]
	ldr	r2, [fp, #-52]
	str	ip, [r3, #8]
	b	.L2
.L3:
	ldr	r3, .L36+4
	ldr	r1, .L36+8
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L4
.L37:
	.align	2
.L36:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r0, #0
	movt	r0, 63686
	bl	MEM_Phy2Vir
	subs	r2, r0, #0
	beq	.L53
	ldr	r7, .L56
	mov	r0, #22
	ldr	r6, .L56+4
	ldr	r1, .L56+8
	ldr	r3, [r7, #68]
	ldr	r5, .L56+12
	blx	r3
	ldr	ip, [r6]
	ldr	r1, [r5, #8]
	ldr	r4, [ip, #136]
	add	r2, r1, #64
	orr	r4, r4, #16
	str	r4, [ip, #136]
	ldr	r3, [r1, #64]
	tst	r3, #65536
	beq	.L40
	movw	r0, #9999
	b	.L41
.L55:
	subs	r0, r0, #1
	beq	.L54
.L41:
	ldr	r3, [r2]
	tst	r3, #65536
	bne	.L55
.L40:
	bfc	r4, #4, #1
	mvn	r3, #1
	str	r4, [ip, #136]
	str	r3, [r1, #52]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L54:
	ldr	r1, .L56+16
	ldr	r3, [r7, #68]
	blx	r3
	ldr	ip, [r6]
	ldr	r1, [r5, #8]
	b	.L40
.L53:
	ldr	r3, .L56
	ldr	r1, .L56+20
	ldr	r3, [r3, #68]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	r3
.L57:
	.align	2
.L56:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_pstRegCrg
	.word	.LC2
	.word	g_HwMem
	.word	.LC3
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
	ldr	r1, .L65+4
	ldr	r3, .L65+8
	ldr	r2, .L65+12
	ldr	ip, [r1, #68]
	ldr	r1, .L65+16
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L66:
	.align	2
.L65:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC4
	.word	.LANCHOR0
	.word	.LC5
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
	ldr	r3, [r6, #68]
	mov	r0, #0
	ldr	r1, .L75+4
	blx	r3
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L76:
	.align	2
.L75:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
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
	ldr	ip, .L84+4
	rsb	r3, r3, r4
	mov	r2, lr
	ldr	r1, .L84+8
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L85:
	.align	2
.L84:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC7
	UNWIND(.fnend)
	.size	BPD_GetParam, .-BPD_GetParam
	.align	2
	.global	BPD_Drv
	.type	BPD_Drv, %function
BPD_Drv:
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
	add	ip, r1, #20480
	mov	r6, r2
	mov	r5, r1
	mov	r7, r0
	ldr	r3, [ip, #2756]
	ldr	r2, [ip, #2752]
	cmp	r3, #2
	ble	.L88
	cmp	r3, #128
	sub	r4, r2, #3
	movle	lr, #0
	movgt	lr, #1
	cmp	r4, #125
	movls	r4, lr
	orrhi	r4, lr, #1
	cmp	r4, #0
	beq	.L89
.L88:
	ldr	lr, .L94
	mov	r3, r3, asl #4
	mov	r2, r2, asl #4
	ldr	r1, .L94+4
	mov	r0, #0
	ldr	ip, [lr, #68]
	blx	ip
.L90:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L89:
	ldr	r9, .L94
	mov	r0, #22
	ldr	r8, .L94+8
	ldr	r1, .L94+12
	ldr	r3, [r9, #68]
	blx	r3
	bl	BPD_Reset
	mov	r3, r8
	mov	r2, r6
	mov	r1, r7
	mov	r0, r5
	bl	BPD_CfgReg
	cmn	r0, #1
	beq	.L93
	ldr	r3, [r8, #8]
	mov	r2, #1
	str	r4, [r3]
	ldr	r3, [r8, #8]
	str	r2, [r3]
	ldr	r3, [r8, #8]
	str	r4, [r3]
	bl	BPDDRV_WaitBpdReadyIfNoIsr
	cmp	r0, #0
	bne	.L90
	mov	r2, r7
	mov	r1, r6
	mov	r0, r5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	BPD_GetParam
.L93:
	ldr	r3, [r9, #68]
	mov	r0, r4
	ldr	r1, .L94+16
	blx	r3
	b	.L90
.L95:
	.align	2
.L94:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC8
	.word	g_HwMem
	.word	.LC9
	.word	.LC10
	UNWIND(.fnend)
	.size	BPD_Drv, .-BPD_Drv
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.20074, %object
	.size	__func__.20074, 12
__func__.20074:
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
	ASCII(.ascii	"*****************reset BPD!!!!******************\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC3:
	ASCII(.ascii	"Reset BPD Err______________________________________" )
	ASCII(.ascii	"_-\012\000" )
	.space	1
.LC4:
	ASCII(.ascii	"BPD register not mapped yet!\000" )
	.space	3
.LC5:
	ASCII(.ascii	"%s: %s\012\000" )
.LC6:
	ASCII(.ascii	"======>>>>> BPD TimeOut!!!\012\000" )
.LC7:
	ASCII(.ascii	" BPD ERROR: EAT TOO MUCH BS %#x, but available bs n" )
	ASCII(.ascii	"um %#x!!!\012\000" )
	.space	2
.LC8:
	ASCII(.ascii	"NOT SUPPORT SIZE %d*%d\012\000" )
.LC9:
	ASCII(.ascii	"----------------BPD_V200_DRV\012\000" )
	.space	2
.LC10:
	ASCII(.ascii	"BPD error: no map reg!\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
