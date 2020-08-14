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
	.file	"fsp.c"
	.text
	.align	2
	.type	FSP_IsSpecialFrm.part.0, %function
FSP_IsSpecialFrm.part.0:
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
	ldr	r3, .L12
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	sub	r3, r1, #1
	cmn	r3, #3
	bhi	.L6
	add	r3, r0, #311296
	ldr	lr, [r3, #2040]
	cmp	lr, #0
	beq	.L6
	ldr	r3, [r3, #1532]
	cmp	r1, r3
	beq	.L9
	mov	r2, #0
	b	.L3
.L4:
	ldr	r3, [r0, r3]
	cmp	r1, r3
	beq	.L9
.L3:
	add	r2, r2, #1
	movw	r3, #50684
	cmp	r2, lr
	movt	r3, 4
	mov	ip, r2, asl #4
	add	r3, ip, r3
	bne	.L4
.L6:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L9:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L13:
	.align	2
.L12:
	.word	s_pstVfmwChan
	UNWIND(.fnend)
	.size	FSP_IsSpecialFrm.part.0, .-FSP_IsSpecialFrm.part.0
	.align	2
	.global	FSP_Init
	.type	FSP_Init, %function
FSP_Init:
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
	ldr	r4, .L17
	mov	r2, #64
	mov	r1, #0
	ldr	r0, .L17+4
	ldr	r3, [r4, #48]
	blx	r3
	ldr	r3, [r4, #48]
	mov	r2, #4224
	mov	r1, #0
	ldr	r0, .L17+8
	blx	r3
	ldr	r3, .L17+8
	mvn	r1, #0
	add	r2, r3, #4224
.L15:
	strh	r1, [r3, #2]	@ movhi
	add	r3, r3, #264
	cmp	r3, r2
	bne	.L15
	ldr	lr, .L17+12
	mov	r3, #0
	ldr	ip, [r4, #48]
	mov	r1, r3
	mov	r2, #16
	ldr	r0, .L17+16
	str	r3, [lr, #-3896]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	ip
.L18:
	.align	2
.L17:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LANCHOR0+64
	.word	.LANCHOR1
	.word	.LANCHOR1-3892
	UNWIND(.fnend)
	.size	FSP_Init, .-FSP_Init
	.align	2
	.global	FSP_ConfigAttribute
	.type	FSP_ConfigAttribute, %function
FSP_ConfigAttribute:
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
	bhi	.L23
	ldr	r3, .L25
	ldr	r3, [r3, r0, asl #2]
	ldr	r2, [r3, #4]
	cmp	r2, #1
	bne	.L24
	ldrb	r2, [r1]	@ zero_extendqisi2
	mov	r0, #0
	strb	r2, [r3, #1]
	ldmfd	sp, {fp, sp, pc}
.L24:
	ldr	r3, .L25+4
	mov	r2, #234
	ldr	r1, .L25+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L23:
	ldr	r3, .L25+12
	mov	r2, #233
	ldr	r1, .L25+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L26:
	.align	2
.L25:
	.word	.LANCHOR0
	.word	.LC2
	.word	.LC1
	.word	.LC0
	UNWIND(.fnend)
	.size	FSP_ConfigAttribute, .-FSP_ConfigAttribute
	.align	2
	.global	FSP_ClearContext
	.type	FSP_ClearContext, %function
FSP_ClearContext:
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
	subs	r5, r0, #0
	beq	.L30
	ldr	r6, .L31
	add	r3, r5, #24576
	mov	r4, #0
	mov	r2, #32
	strb	r4, [r5, #8]
	mov	r1, r4
	strb	r4, [r5, #9]
	add	r0, r5, #20
	strb	r4, [r5, #11]
	strb	r4, [r5, #12]
	strb	r4, [r5, #13]
	strb	r4, [r5, #14]
	str	r4, [r5, #16]
	str	r4, [r3, #3812]
	ldr	ip, [r6, #48]
	str	r4, [r3, #3980]
	str	r4, [r3, #3984]
	str	r4, [r3, #3988]
	str	r4, [r3, #3976]
	blx	ip
	add	r0, r5, #21248
	ldr	r3, [r6, #48]
	mov	r1, r4
	mov	r2, #100
	add	r0, r0, #56
	blx	r3
	add	r0, r5, #21248
	ldr	r3, [r6, #48]
	mov	r1, r4
	mov	r2, #3200
	add	r0, r0, #156
	blx	r3
	add	r0, r5, #24576
	ldr	r3, [r6, #48]
	mov	r1, r4
	mov	r2, #3200
	add	r0, r0, #28
	blx	r3
	add	r0, r5, #27648
	ldr	r3, [r6, #48]
	mov	r1, r4
	mov	r2, #200
	add	r0, r0, #156
	blx	r3
	add	r0, r5, #27904
	ldr	r3, [r6, #48]
	mov	r1, r4
	mov	r2, #240
	add	r0, r0, #100
	blx	r3
	add	r0, r5, #28160
	ldr	r3, [r6, #48]
	mov	r1, r4
	mov	r2, #160
	add	r0, r0, #232
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L30:
	ldr	r3, .L31+4
	mov	r2, #250
	ldr	r1, .L31+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L32:
	.align	2
.L31:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC3
	.word	.LC1
	UNWIND(.fnend)
	.size	FSP_ClearContext, .-FSP_ClearContext
	.align	2
	.global	FSP_ClearContextAll
	.type	FSP_ClearContextAll, %function
FSP_ClearContextAll:
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
	subs	r4, r0, #0
	beq	.L36
	add	r5, r4, #24576
	ldr	r3, .L37
	ldrb	r7, [r4, #10]	@ zero_extendqisi2
	movw	r2, #28568
	ldr	r6, [r5, #3976]
	mov	r1, #0
	ldrb	r10, [r4]	@ zero_extendqisi2
	ldrb	r9, [r4, #1]	@ zero_extendqisi2
	ldr	r8, [r4, #4]
	ldr	r3, [r3, #48]
	blx	r3
	strb	r10, [r4]
	strb	r9, [r4, #1]
	mov	r0, #0
	str	r8, [r4, #4]
	strb	r7, [r4, #10]
	str	r6, [r5, #3976]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L36:
	ldr	r3, .L37+4
	movw	r2, #286
	ldr	r1, .L37+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L38:
	.align	2
.L37:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC3
	.word	.LC1
	UNWIND(.fnend)
	.size	FSP_ClearContextAll, .-FSP_ClearContextAll
	.align	2
	.global	FSP_EnableInstance
	.type	FSP_EnableInstance, %function
FSP_EnableInstance:
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
	mov	r6, r0
	bhi	.L44
	ldr	r3, .L46
	ldr	r4, [r3, r0, asl #2]
	cmp	r4, #0
	beq	.L45
	ldr	r5, [r4, #4]
	cmp	r5, #0
	bne	.L43
	ldr	r3, .L46+4
	movw	r2, #28568
	mov	r1, r5
	mov	r0, r4
	ldr	r3, [r3, #48]
	blx	r3
	add	r3, r4, #24576
	strb	r6, [r4]
	mov	r2, #1
	mov	r0, r5
	str	r2, [r4, #4]
	str	r5, [r3, #3976]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L43:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L44:
	ldr	r3, .L46+8
	mov	r2, #312
	ldr	r1, .L46+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L45:
	mov	r0, r4
	ldr	r3, .L46+16
	movw	r2, #313
	ldr	r1, .L46+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L47:
	.align	2
.L46:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_EnableInstance, .-FSP_EnableInstance
	.align	2
	.global	FSP_DisableInstance
	.type	FSP_DisableInstance, %function
FSP_DisableInstance:
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
	bhi	.L52
	ldr	r3, .L54
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L53
	ldr	r3, [r0, #4]
	cmp	r3, #1
	moveq	r3, #0
	streq	r3, [r0, #4]
	ldmfd	sp, {fp, sp, pc}
.L52:
	ldr	r3, .L54+4
	movw	r2, #335
	ldr	r1, .L54+8
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L53:
	ldr	r3, .L54+12
	mov	r2, #336
	ldr	r1, .L54+8
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L55:
	.align	2
.L54:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_DisableInstance, .-FSP_DisableInstance
	.align	2
	.global	FSP_Exit
	.type	FSP_Exit, %function
FSP_Exit:
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
	ldr	r5, .L67
	mov	r4, #0
	b	.L58
.L57:
	add	r4, r4, #1
	cmp	r4, #16
	beq	.L66
.L58:
	ldr	r3, [r5, #4]!
	cmp	r3, #0
	beq	.L57
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L57
	mov	r0, r4
	add	r4, r4, #1
	bl	FSP_DisableInstance
	cmp	r4, #16
	bne	.L58
.L66:
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L68:
	.align	2
.L67:
	.word	.LANCHOR0-4
	UNWIND(.fnend)
	.size	FSP_Exit, .-FSP_Exit
	.align	2
	.global	FSP_ResetInstance
	.type	FSP_ResetInstance, %function
FSP_ResetInstance:
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
	bhi	.L90
	ldr	r3, .L96
	ldr	r7, [r3, r0, asl #2]
	cmp	r7, #0
	beq	.L91
	ldr	r9, .L96+4
	add	r4, r7, #21248
	add	r5, r7, #24576
	add	r4, r4, #158
	add	r5, r5, #30
	mov	r8, r9
	b	.L79
.L76:
	ldrsb	r3, [r4]
	cmp	r3, #0
	bne	.L92
.L74:
	add	r4, r4, #100
	cmp	r4, r5
	beq	.L93
.L79:
	ldr	r1, [r4, #14]
	cmp	r1, #0
	beq	.L74
	ldr	r3, [r9, #-3896]
	cmp	r3, #0
	beq	.L74
	ldrsb	r3, [r4, #-2]
	cmp	r3, #0
	beq	.L76
	ldr	r3, [r8, #-3880]
	cmp	r3, #0
	beq	.L94
	mov	r2, #0
	mov	r0, r6
	blx	r3
	ldrsb	r3, [r4]
	cmp	r3, #0
	beq	.L74
.L92:
	ldr	r3, [r9, #-3884]
	ldr	r1, [r4, #14]
	cmp	r3, #0
	beq	.L95
	mov	r0, r6
	add	r4, r4, #100
	blx	r3
	cmp	r4, r5
	bne	.L79
.L93:
	mov	r0, r7
	bl	FSP_ClearContext
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L90:
	ldr	r3, .L96+8
	mov	r2, #356
	ldr	r1, .L96+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L95:
	mov	r0, r3
	ldr	r1, .L96+16
	bl	dprint_vfmw
	b	.L74
.L94:
	mov	r0, r3
	ldr	r1, .L96+20
	bl	dprint_vfmw
	b	.L76
.L91:
	mov	r0, r7
	ldr	r3, .L96+24
	movw	r2, #357
	ldr	r1, .L96+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L97:
	.align	2
.L96:
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	.LC0
	.word	.LC1
	.word	.LC6
	.word	.LC5
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_ResetInstance, .-FSP_ResetInstance
	.align	2
	.global	FSP_EmptyInstance
	.type	FSP_EmptyInstance, %function
FSP_EmptyInstance:
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
	cmp	r0, #15
	mov	r8, r0
	bhi	.L134
	ldr	r3, .L140
	ldr	r7, [r3, r0, asl #2]
	cmp	r7, #0
	beq	.L135
	ldr	r3, .L140+4
	mov	r2, #21248
	mov	r1, #0
	add	r0, r7, #56
	ldr	r3, [r3, #48]
	blx	r3
	ldrsb	r3, [r7, #10]
	cmp	r3, #0
	ble	.L112
	ldr	r10, .L140+8
	add	r4, r7, #21248
	mov	r6, #0
	add	r4, r4, #248
	mov	r5, r6
	mov	r9, r10
	b	.L111
.L109:
	strb	r5, [r4, #-90]
.L110:
	strb	r5, [r4, #-92]
	add	r6, r6, #1
	strb	r5, [r4]
	add	r4, r4, #100
	str	r5, [r4, #-104]
	ldrsb	r3, [r7, #10]
	cmp	r3, r6
	ble	.L112
.L111:
	ldr	r1, [r4, #-76]
	cmp	r1, #0
	beq	.L105
	ldr	r3, [r10, #-3896]
	cmp	r3, #0
	beq	.L105
	ldrsb	r3, [r4, #-92]
	cmp	r3, #0
	bne	.L136
.L106:
	ldrsb	r3, [r4, #-90]
	cmp	r3, #0
	bne	.L137
.L105:
	cmp	r8, #15
	bgt	.L109
	mov	r0, r8
	bl	FSP_IsSpecialFrm.part.0
	cmp	r0, #1
	bne	.L109
	mov	r3, #4
	strb	r3, [r4, #-90]
	b	.L110
.L112:
	ldrsb	r10, [r7, #11]
	cmp	r10, #0
	ble	.L104
	add	r4, r7, #24576
	mov	r6, #0
	add	r4, r4, #44
	mov	r5, r6
	mov	r9, #4
	b	.L117
.L115:
	strb	r5, [r4, #-14]
.L116:
	add	r6, r6, #1
	strb	r5, [r4, #-16]
	cmp	r6, r10
	strb	r5, [r4, #76]
	str	r5, [r4, #72]
	add	r4, r4, #100
	beq	.L104
.L117:
	cmp	r8, #15
	ldr	r1, [r4]
	bgt	.L115
	mov	r0, r8
	bl	FSP_IsSpecialFrm.part.0
	cmp	r0, #1
	bne	.L115
	strb	r9, [r4, #-14]
	b	.L116
.L104:
	ldrsb	r1, [r7, #13]
	cmp	r1, #0
	ble	.L114
	add	r3, r7, #28160
	mov	r2, #0
	add	r3, r3, #236
	mov	ip, r2
	mov	r0, #1
.L118:
	add	r2, r2, #1
	strb	r0, [r3]
	cmp	r1, r2
	strb	ip, [r3, #1]
	add	r3, r3, #8
	bne	.L118
.L114:
	mov	r0, #0
	str	r0, [r7, #16]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L137:
	ldr	r3, [r10, #-3884]
	cmp	r3, #0
	beq	.L138
	mov	r0, r8
	blx	r3
	ldr	r1, [r4, #-76]
	b	.L105
.L136:
	ldr	r3, [r9, #-3880]
	cmp	r3, #0
	beq	.L139
	mov	r2, #0
	mov	r0, r8
	blx	r3
	ldr	r1, [r4, #-76]
	b	.L106
.L134:
	ldr	r3, .L140+12
	movw	r2, #411
	ldr	r1, .L140+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L135:
	mov	r0, r7
	ldr	r3, .L140+20
	mov	r2, #412
	ldr	r1, .L140+16
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L139:
	ldr	r1, .L140+24
	mov	r0, r3
	bl	dprint_vfmw
	ldr	r1, [r4, #-76]
	b	.L106
.L138:
	ldr	r1, .L140+28
	mov	r0, r3
	bl	dprint_vfmw
	ldr	r1, [r4, #-76]
	b	.L105
.L141:
	.align	2
.L140:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1
	.word	.LC0
	.word	.LC1
	.word	.LC4
	.word	.LC5
	.word	.LC6
	UNWIND(.fnend)
	.size	FSP_EmptyInstance, .-FSP_EmptyInstance
	.align	2
	.global	FSP_GetUsedBufferNum
	.type	FSP_GetUsedBufferNum, %function
FSP_GetUsedBufferNum:
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
	ldr	r2, .L148
	movw	r3, #2082
	ldr	r2, [r2, r0, asl #2]
	add	r1, r2, #278528
	ldrsb	ip, [r1, r3]
	cmp	ip, #0
	ble	.L146
	movw	r3, #39862
	mov	r0, #0
	movt	r3, 4
	add	r3, r2, r3
	mov	r2, r0
.L145:
	ldrsb	r1, [r3]
	add	r2, r2, #1
	add	r3, r3, #100
	cmp	r1, #3
	addeq	r0, r0, #1
	cmp	r2, ip
	bne	.L145
	ldmfd	sp, {fp, sp, pc}
.L146:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L149:
	.align	2
.L148:
	.word	s_pstVfmwChan
	UNWIND(.fnend)
	.size	FSP_GetUsedBufferNum, .-FSP_GetUsedBufferNum
	.align	2
	.global	FSP_ConfigInstance
	.type	FSP_ConfigInstance, %function
FSP_ConfigInstance:
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
	bhi	.L160
	cmp	r1, #0
	beq	.L161
	ldr	r3, .L163
	ldr	r4, [r3, r0, asl #2]
	cmp	r4, #0
	beq	.L154
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne	.L154
	ldr	r3, .L163+4
	ldr	r3, [r3, r0, asl #2]
	add	r3, r3, #307200
	ldr	r3, [r3, #1968]
	cmp	r3, #0
	beq	.L162
.L156:
	ldr	r3, .L163+8
	add	r0, r4, #20
	mov	r2, #32
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, #0
.L152:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L162:
	mov	r0, r4
	str	r1, [fp, #-24]
	bl	FSP_ClearContext
	ldr	r1, [fp, #-24]
	b	.L156
.L154:
	ldr	r3, .L163+12
	movw	r2, #539
	ldr	r1, .L163+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L152
.L160:
	ldr	r3, .L163+20
	mov	r2, #536
	ldr	r1, .L163+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L152
.L161:
	mov	r0, r1
	ldr	r3, .L163+24
	movw	r2, #537
	ldr	r1, .L163+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L152
.L164:
	.align	2
.L163:
	.word	.LANCHOR0
	.word	s_pstVfmwChan
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC8
	.word	.LC1
	.word	.LC0
	.word	.LC7
	UNWIND(.fnend)
	.size	FSP_ConfigInstance, .-FSP_ConfigInstance
	.align	2
	.global	FSP_CheckCapLevel
	.type	FSP_CheckCapLevel, %function
FSP_CheckCapLevel:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
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
	ldr	r3, .L177
	ldr	r1, .L177+4
	ldr	r3, [r3, r0, asl #2]
	ldr	r5, [r1, r0, asl #2]
	add	r3, r3, #311296
	ldr	r4, [r3, #2052]
	cmp	r4, #0
	beq	.L166
.L170:
	mov	r0, #0
.L174:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L166:
	bl	VCTRL_GetChanCapLevel
	cmp	r0, #32
	bhi	.L168
	ldr	r2, .L177+8
	mov	r3, r0, asl #5
	sub	r3, r3, r0, asl #2
	ldr	r1, [r5, #28]
	add	lr, r2, r3
	ldr	ip, [r5, #32]
	ldr	r3, [r2, r3]
	ldr	lr, [lr, #4]
	mul	r6, ip, r1
	mul	r2, lr, r3
	cmp	r0, #25
	cmpne	r6, r2
	ble	.L170
	ldrsb	r2, [r5, #22]
	cmp	r2, #1
	beq	.L170
	stmib	sp, {r1, ip}
	mov	r2, r0
	str	lr, [sp]
	mov	r0, r4
	ldr	r1, .L177+12
	bl	dprint_vfmw
	b	.L171
.L168:
	ldrsb	r3, [r5, #22]
	cmp	r3, #1
	beq	.L170
	mov	r2, r0
	ldr	r1, .L177+16
	mov	r0, r4
	bl	dprint_vfmw
.L171:
	mov	r3, #0
	mvn	r0, #0
	strb	r3, [r5, #9]
	b	.L174
.L178:
	.align	2
.L177:
	.word	s_pstVfmwChan
	.word	.LANCHOR0
	.word	CapItem
	.word	.LC10
	.word	.LC9
	UNWIND(.fnend)
	.size	FSP_CheckCapLevel, .-FSP_CheckCapLevel
	.align	2
	.global	FSP_RecordSpecialFrame
	.type	FSP_RecordSpecialFrame, %function
FSP_RecordSpecialFrame:
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
	ldr	r2, .L188
	ldr	r3, .L188+4
	ldr	r4, [r2, r0, asl #2]
	ldr	r1, [r3, r0, asl #2]
	add	lr, r4, #311296
	ldr	r2, [lr, #2044]
	cmp	r2, #0
	ldmnefd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, [lr, #2040]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	add	r3, r1, #24576
	mov	ip, #100
	mov	r5, #4
	ldr	r3, [r3, #3980]
	mla	ip, ip, r3, r1
	add	r3, ip, #21248
	add	ip, r3, #152
	ldr	r0, [r3, #172]
.L183:
	mov	r1, r2, asl #4
	movw	r3, #50684
	movt	r3, 4
	add	r3, r1, r3
	add	r2, r2, #1
	ldr	r3, [r4, r3]
	cmp	r0, r3
	streqb	r5, [ip, #6]
	ldr	r3, [lr, #2040]
	cmp	r3, r2
	bhi	.L183
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L189:
	.align	2
.L188:
	.word	s_pstVfmwChan
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	FSP_RecordSpecialFrame, .-FSP_RecordSpecialFrame
	.align	2
	.global	FSP_ConfigFrameBuf
	.type	FSP_ConfigFrameBuf, %function
FSP_ConfigFrameBuf:
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
	mov	r7, r0
	ldr	r0, .L249
	mov	r4, r1
	mov	r9, r2
	mov	r1, #0
	movw	r2, #1272
	str	r3, [fp, #-56]
	ldr	ip, [r0, #48]
	ldr	r0, .L249+4
	blx	ip
	cmp	r7, #15
	bhi	.L242
	ldr	r3, .L249+8
	ldr	r5, [r3, r7, asl #2]
	cmp	r5, #0
	beq	.L243
	mov	r0, r7
	bl	FSP_CheckCapLevel
	subs	r3, r0, #0
	str	r3, [fp, #-52]
	bne	.L239
	ldr	r6, .L249+12
	ldr	ip, [r5, #28]
	ldr	lr, [r5, #32]
	ldr	r1, [r6, r7, asl #2]
	mul	r10, lr, ip
	ldr	r2, [r1, #160]
	ldr	r3, [r1, #164]
	mul	r8, r3, r2
	cmp	r10, r8
	bgt	.L244
	ldr	r3, [r1, #1440]
	ldrsb	r8, [r5, #23]
	cmp	r3, #1
	add	r3, r1, #307200
	beq	.L245
.L197:
	ldr	r2, [r3, #1976]
	ldr	r0, .L249+16
	cmp	r8, r2
	ldr	ip, [r0, #64]
	movne	r2, #1
	moveq	r2, #0
	str	r2, [r3, #1992]
	ldrsb	r2, [r5, #23]
	cmp	ip, #0
	str	r2, [r3, #1976]
	beq	.L201
	ldr	lr, .L249+4
	mov	r0, r4
	ldr	r3, [r5, #32]
	mov	r4, #32
	ldr	r2, [r5, #28]
	mov	r1, r9
	str	lr, [sp, #16]
	str	r7, [sp, #12]
	ldrsb	lr, [r5, #22]
	str	r4, [sp, #4]
	ldr	r9, .L249+12
	str	lr, [sp, #8]
	ldrsb	lr, [r5, #26]
	ldr	r4, .L249+20
	str	lr, [sp]
	blx	ip
	ldr	r3, [r6, r7, asl #2]
	add	r3, r3, #311296
	ldr	r3, [r3, #1456]
	cmp	r3, #20
	movge	r3, #20
	strb	r3, [r5, #13]
	cmp	r0, #2
	mov	r10, r0
	beq	.L246
	cmp	r0, #0
	bne	.L204
	ldr	r3, [r4, #-2796]
	mov	r0, #31
	ldr	r1, .L249+24
	strb	r3, [r5, #9]
	ldr	r3, [r9, r7, asl #2]
	str	r10, [r3, #1480]
	bl	dprint_vfmw
	ldr	r3, [r4, #-2796]
	cmp	r8, r3
	bgt	.L247
	ldrsb	r3, [r5, #1]
	strb	r10, [r5, #11]
	cmp	r3, #0
	strb	r10, [r5, #12]
	bne	.L248
.L207:
	ldr	r3, [r4, #-2796]
	cmp	r3, #32
	movge	r3, #32
	strb	r3, [r5, #10]
.L208:
	ldr	r3, .L249
	mov	r0, #8
	add	r6, r5, #24576
	ldr	r3, [r3, #12]
	blx	r3
	ldr	r3, [r4, #-2624]
	cmp	r3, #0
	beq	.L213
	ldr	r8, .L249+4
	mov	r1, #0
	ldr	r0, [r6, #3980]
	mov	r9, r1
	add	r10, r8, #388
	str	r7, [fp, #-48]
	mov	r7, r6
	mov	r6, r10
	mov	r10, r8
	mov	r8, r1
.L212:
	mov	r3, #100
	ldr	r1, [r4, #-3876]
	mla	r3, r3, r0, r5
	add	r8, r8, #1
	add	r2, r3, #21248
	mov	ip, r2
	str	r0, [r2, #164]
	ldr	r2, [r10, #8]
	mov	r0, ip
	str	r2, [ip, #172]
	mov	r2, ip
	ldr	lr, [r10, #12]!
	str	lr, [ip, #176]
	ldr	lr, [r6, #4]!
	str	lr, [r0, #184]
	mov	r0, ip
	str	r1, [r2, #188]
	mov	lr, ip
	ldr	r1, [r4, #-2824]
	str	r1, [ip, #204]
	ldr	r1, [r4, #-2816]
	str	r1, [ip, #208]
	ldr	r1, [r4, #-2812]
	str	r1, [r0, #212]
	ldr	r1, [r4, #-2832]
	str	r1, [r0, #216]
	ldr	r1, [r4, #-2828]
	str	r1, [r0, #220]
	ldr	r0, [r6, #512]
	ldr	r1, [r4, #-2840]
	str	r0, [r2, #192]
	ldr	r2, [r4, #-2844]
	str	r1, [ip, #200]
	ldr	r1, [r4, #-2808]
	str	r2, [ip, #196]
	mov	r2, ip
	ldr	r0, [r5, #28]
	str	r0, [lr, #224]
	ldr	lr, [r5, #32]
	str	r1, [r2, #232]
	ldr	r0, [fp, #-48]
	str	lr, [r2, #228]
	ldr	r2, [r4, #-2804]
	str	r2, [ip, #236]
	ldr	r2, [r4, #-2800]
	strb	r9, [ip, #156]
	strb	r9, [ip, #157]
	str	r2, [ip, #240]
	strb	r9, [ip, #158]
	bl	FSP_RecordSpecialFrame
	ldr	r3, [r7, #3980]
	mov	r1, #100
	mov	r2, #1
	add	r0, r3, r2
	ldr	ip, [r4, #-2624]
	mla	r3, r1, r3, r5
	cmp	ip, r8
	add	r3, r3, #21248
	str	r2, [r3, #252]
	str	r0, [r7, #3980]
	bhi	.L212
	mov	r6, r7
.L213:
	ldr	r0, [r4, #-2616]
	cmp	r0, #0
	beq	.L211
	ldr	ip, [r6, #3988]
	mov	r1, #100
	ldr	r2, .L249+4
	mov	r3, #0
	mla	r1, r1, ip, r5
	add	r1, r1, #21248
	add	r1, r1, #180
.L216:
	ldr	lr, [r2, #520]
	add	r3, r3, #1
	cmp	r3, r0
	add	r2, r2, #12
	str	lr, [r1], #100
	bne	.L216
	add	r3, ip, r3
	str	r3, [r6, #3988]
.L211:
	mov	lr, #0
	mov	r7, #1
	strb	r7, [r5, #12]
	str	lr, [r6, #3236]
	ldrsb	r3, [r5, #10]
	ldr	r2, [r4, #-2824]
	cmp	r3, #31
	ldr	r1, [r4, #-2812]
	ldr	r0, [r4, #-2816]
	movge	r3, #31
	str	r2, [r6, #3276]
	mov	r2, r3, asl #4
	str	r1, [r6, #3284]
	sub	r3, r2, r3, asl #2
	ldr	r8, [r4, #-3876]
	add	r3, r4, r3
	str	r0, [r6, #3280]
	ldr	r2, [r4, #-2808]
	ldr	r1, [r3, #-3868]
	str	r8, [r6, #3260]
	ldr	r3, [r4, #-2628]
	str	r1, [r6, #3244]
	ldr	r1, [r5, #28]
	cmp	r3, #20
	ldr	r0, [r4, #-2800]
	movge	r3, #20
	mov	ip, r1
	str	r1, [fp, #-48]
	str	ip, [r6, #3296]
	ldr	r1, [r4, #-2804]
	ldr	r10, [r5, #32]
	strb	lr, [r6, #3228]
	strb	lr, [r6, #3229]
	str	r10, [r6, #3300]
	strb	lr, [r6, #3230]
	str	r7, [r6, #3324]
	str	r2, [r6, #3304]
	str	r1, [r6, #3308]
	str	r0, [r6, #3312]
	strb	r3, [r5, #13]
	ldr	r9, [r4, #-2620]
	cmp	r9, lr
	beq	.L215
	ldr	r3, [r6, #3984]
	ldr	r0, .L249+4
	add	r2, r3, #3536
	add	r9, r3, r9
	add	r2, r2, #13
	add	r1, r5, r2, lsl #3
.L217:
	ldr	ip, [r0, #1084]
	add	r3, r3, #1
	cmp	r3, r9
	add	r0, r0, #8
	add	r1, r1, #8
	str	ip, [r1, #-8]
	add	ip, r5, r2, lsl #3
	add	r2, r3, #3536
	strb	lr, [ip, #5]
	add	r2, r2, #13
	strb	r7, [r1, #-4]
	bne	.L217
	str	r3, [r6, #3984]
.L215:
	ldr	r0, [r4, #-2632]
	add	r5, r5, #20480
	ldr	ip, [r4, #-2824]
	mov	r3, #0
	ldr	r1, [r4, #-2816]
	mov	r2, #1
	str	r0, [r6, #3812]
	mov	r0, #8
	str	r8, [r5, #856]
	ldr	r8, [fp, #-48]
	ldr	r6, [r4, #-2812]
	ldr	r7, [r4, #-2808]
	ldr	r9, [r4, #-2800]
	ldr	lr, [r5, #940]
	str	r8, [r5, #892]
	str	r3, [r5, #832]
	ldr	r8, [r4, #-2804]
	strb	r3, [r5, #824]
	strb	r3, [r5, #825]
	strb	r3, [r5, #826]
	ldr	r4, [r4, #-3872]
	ldr	r3, [fp, #-56]
	str	r10, [r5, #896]
	str	lr, [r5, #840]
	str	ip, [r5, #872]
	str	r1, [r5, #876]
	str	r6, [r5, #880]
	str	r7, [r5, #900]
	str	r8, [r5, #904]
	str	r9, [r5, #908]
	str	r2, [r5, #920]
	str	r4, [r3]
	ldr	r3, .L249
	ldr	r3, [r3, #16]
	blx	r3
	ldr	r0, [fp, #-52]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L244:
	stmia	sp, {ip, lr}
	ldr	r1, .L249+28
	bl	dprint_vfmw
	ldr	r3, .L249+32
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L239
	ldr	r3, [fp, #-52]
	mov	r0, r7
	mov	r1, #102
	mov	r2, r3
	blx	ip
.L239:
	mvn	r3, #0
	str	r3, [fp, #-52]
.L222:
	ldr	r0, [fp, #-52]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L246:
	ldr	r3, [r4, #-2796]
	mov	r2, #1
	str	r0, [fp, #-52]
	ldr	r0, [fp, #-52]
	strb	r3, [r5, #9]
	ldr	r3, [r9, r7, asl #2]
	str	r2, [r3, #1480]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L245:
	ldr	r2, [r3, #1952]
	bics	r0, r2, #2
	beq	.L197
	cmp	r2, #1
	beq	.L222
	ldrb	r3, [fp, #-52]	@ zero_extendqisi2
	mvn	r2, #0
	ldr	r1, .L249+36
	mov	r0, #31
	str	r2, [fp, #-52]
	strb	r3, [r5, #9]
	ldr	r3, [r6, r7, asl #2]
	add	r3, r3, #307200
	ldr	r2, [r3, #1952]
	bl	dprint_vfmw
	b	.L222
.L201:
	add	r1, r1, #311296
	mvn	r10, #0
	ldr	r3, [r1, #1456]
	cmp	r3, #20
	movge	r3, #20
	strb	r3, [r5, #13]
.L204:
	mov	r3, #0
	strb	r3, [r5, #9]
	ldr	ip, [r6, r7, asl #2]
	mov	r2, r10
	mov	r0, r3
	ldr	r1, .L249+40
	mvn	lr, #0
	str	lr, [fp, #-52]
	str	r3, [ip, #1480]
	bl	dprint_vfmw
	b	.L222
.L248:
	ldr	r3, [r9, r7, asl #2]
	ldr	r3, [r3, #1440]
	cmp	r3, #1
	beq	.L207
	ldrsb	r3, [r5, #23]
	ldr	r2, [r4, #-2796]
	add	r3, r3, #7
	cmp	r3, r2
	movge	r3, r2
	strb	r3, [r5, #10]
	b	.L208
.L242:
	ldr	r3, .L249+44
	movw	r2, #653
	ldr	r1, .L249+48
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r3, #0
	str	r3, [fp, #-52]
	b	.L222
.L243:
	ldr	r3, .L249+52
	mov	r0, r5
	movw	r2, #654
	ldr	r1, .L249+48
	bl	dprint_vfmw
	mvn	r3, #0
	str	r3, [fp, #-52]
	b	.L222
.L247:
	str	r3, [sp]
	mov	r2, r10
	mov	r3, r8
	mov	r0, r10
	ldr	r1, .L249+56
	mvn	ip, #0
	str	ip, [fp, #-52]
	bl	dprint_vfmw
	ldr	r3, .L249
	mov	r0, #8
	ldr	r3, [r3, #16]
	blx	r3
	b	.L222
.L250:
	.align	2
.L249:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1-3876
	.word	.LANCHOR0
	.word	s_pstVfmwChan
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR1
	.word	.LC13
	.word	.LC11
	.word	g_event_report
	.word	.LC12
	.word	.LC14
	.word	.LC0
	.word	.LC1
	.word	.LC4
	.word	.LC15
	UNWIND(.fnend)
	.size	FSP_ConfigFrameBuf, .-FSP_ConfigFrameBuf
	.align	2
	.global	FSP_GetDecodeFrameTable
	.type	FSP_GetDecodeFrameTable, %function
FSP_GetDecodeFrameTable:
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
	bhi	.L262
	cmp	r1, #0
	moveq	r0, r1
	ldreq	r3, .L264
	movweq	r2, #881
	beq	.L261
	cmp	r2, #0
	moveq	r0, r2
	ldreq	r3, .L264+4
	movweq	r2, #882
	beq	.L261
	ldr	r3, .L264+8
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L263
	ldrsb	r3, [r0, #10]
	str	r3, [r2]
	ldrsb	r3, [r0, #10]
	cmp	r3, #0
	ldmlefd	sp, {fp, sp, pc}
	add	r2, r0, #21248
	sub	r1, r1, #4
	add	r2, r2, #172
	mov	r3, #0
.L258:
	ldr	ip, [r2], #100
	add	r3, r3, #1
	str	ip, [r1, #4]!
	ldrsb	ip, [r0, #10]
	cmp	ip, r3
	bgt	.L258
	ldmfd	sp, {fp, sp, pc}
.L262:
	ldr	r3, .L264+12
	mov	r2, #880
	ldr	r1, .L264+16
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L263:
	ldr	r3, .L264+20
	movw	r2, #883
.L261:
	ldr	r1, .L264+16
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L265:
	.align	2
.L264:
	.word	.LC16
	.word	.LC17
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_GetDecodeFrameTable, .-FSP_GetDecodeFrameTable
	.align	2
	.global	FSP_GetDecodePmvTable
	.type	FSP_GetDecodePmvTable, %function
FSP_GetDecodePmvTable:
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
	bhi	.L277
	cmp	r1, #0
	moveq	r0, r1
	ldreq	r3, .L279
	movweq	r2, #907
	beq	.L276
	cmp	r2, #0
	moveq	r0, r2
	ldreq	r3, .L279+4
	moveq	r2, #908
	beq	.L276
	ldr	r3, .L279+8
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L278
	ldrsb	r3, [r0, #13]
	str	r3, [r2]
	ldrsb	r3, [r0, #13]
	cmp	r3, #0
	ldmlefd	sp, {fp, sp, pc}
	sub	r1, r1, #4
	mov	r2, #0
.L273:
	mov	r3, r2, asl #3
	add	r2, r2, #1
	add	r3, r3, #28160
	add	r3, r3, #232
	ldr	r3, [r0, r3]
	str	r3, [r1, #4]!
	ldrsb	r3, [r0, #13]
	cmp	r3, r2
	bgt	.L273
	ldmfd	sp, {fp, sp, pc}
.L277:
	ldr	r3, .L279+12
	movw	r2, #906
	ldr	r1, .L279+16
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L278:
	ldr	r3, .L279+20
	movw	r2, #909
.L276:
	ldr	r1, .L279+16
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L280:
	.align	2
.L279:
	.word	.LC16
	.word	.LC17
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_GetDecodePmvTable, .-FSP_GetDecodePmvTable
	.align	2
	.global	FSP_IsFsLeak
	.type	FSP_IsFsLeak, %function
FSP_IsFsLeak:
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
	ldr	r3, .L284
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L282
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L282:
	ldr	r1, .L284+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L285:
	.align	2
.L284:
	.word	.LANCHOR0
	.word	.LC18
	UNWIND(.fnend)
	.size	FSP_IsFsLeak, .-FSP_IsFsLeak
	.align	2
	.global	FSP_IsFsStatusReady
	.type	FSP_IsFsStatusReady, %function
FSP_IsFsStatusReady:
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
	ldr	r3, [r1, #96]
	cmp	r3, #1
	beq	.L292
.L290:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L292:
	ldr	r2, [r1]
	bic	r2, r2, #-16777216
	bic	r2, r2, #65280
	cmp	r2, #0
	bne	.L290
	cmp	r0, #15
	ldr	r1, [r1, #16]
	ble	.L293
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L293:
	bl	FSP_IsSpecialFrm.part.0
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	FSP_IsFsStatusReady, .-FSP_IsFsStatusReady
	.align	2
	.global	FindLeastTickPhyFs
	.type	FindLeastTickPhyFs, %function
FindLeastTickPhyFs:
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
	mov	r2, r0
	mov	r0, #0
	ble	.L298
	mvn	lr, #0
	mov	r3, r0
.L297:
	ldr	ip, [r2, #88]
	add	r2, r2, #96
	cmp	ip, lr
	movcc	r0, r3
	add	r3, r3, #1
	movcc	lr, ip
	cmp	r3, r1
	bne	.L297
	ldmfd	sp, {fp, sp, pc}
.L298:
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	FindLeastTickPhyFs, .-FindLeastTickPhyFs
	.align	2
	.global	FSP_FindEmptyLogicFs
	.type	FSP_FindEmptyLogicFs, %function
FSP_FindEmptyLogicFs:
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
	mov	r2, #0
	mov	r3, r0
.L303:
	ldrsb	r1, [r3, #56]
	cmp	r1, #0
	bne	.L301
	ldrsb	r1, [r3, #57]
	cmp	r1, #0
	beq	.L305
.L301:
	add	r2, r2, #1
	add	r3, r3, #664
	cmp	r2, #32
	bne	.L303
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L305:
	mov	r3, #664
	mul	r3, r3, r2
	add	ip, r0, r3
	add	r3, r3, #56
	add	r0, r0, r3
	strb	r2, [ip, #59]
	strb	r1, [ip, #58]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	FSP_FindEmptyLogicFs, .-FSP_FindEmptyLogicFs
	.align	2
	.global	FSP_FindEmptyDecodeFs
	.type	FSP_FindEmptyDecodeFs, %function
FSP_FindEmptyDecodeFs:
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
	ldr	r3, .L317
	mov	r7, r0
	mov	r5, r1
	ldr	r4, [r3, r0, asl #2]
	bl	VCTRL_GetVidStd
	cmp	r0, #17
	beq	.L307
	ldrsb	r8, [r4, #10]
	cmp	r8, #0
	ldmlefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r4, r4, #21248
	mov	r6, #0
	add	r4, r4, #156
	b	.L310
.L309:
	cmp	r6, r8
	add	r4, r4, #100
	beq	.L315
.L310:
	mov	r1, r4
	mov	r0, r7
	bl	FSP_IsFsStatusReady
	add	r6, r6, #1
	cmp	r0, #0
	beq	.L309
	str	r4, [r5, #640]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L315:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L307:
	ldr	r1, .L317+4
	add	r3, r4, #21248
	add	r3, r3, #252
	mov	r2, #0
	ldr	r1, [r1, r7, asl #2]
	add	r1, r1, #606208
	ldr	r0, [r1, #844]
	b	.L312
.L311:
	add	r2, r2, #1
	add	r3, r3, #100
	cmp	r2, #32
	beq	.L316
.L312:
	ldr	r1, [r3]
	cmp	r1, #1
	bne	.L311
	ldrsb	r1, [r3, #-96]
	cmp	r1, #0
	bne	.L311
	ldrsb	r1, [r3, #-94]
	cmp	r1, #0
	bne	.L311
	ldr	r1, [r3, #-76]
	cmp	r1, r0
	bne	.L311
	mov	r3, #100
	mla	r4, r3, r2, r4
	add	r3, r4, #21248
	add	r3, r3, #156
	str	r3, [r5, #640]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L316:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L318:
	.align	2
.L317:
	.word	.LANCHOR0
	.word	s_pstVfmwChan
	UNWIND(.fnend)
	.size	FSP_FindEmptyDecodeFs, .-FSP_FindEmptyDecodeFs
	.align	2
	.global	FSP_NewLogicFs
	.type	FSP_NewLogicFs, %function
FSP_NewLogicFs:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 48
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
	cmp	r0, #15
	mov	r9, r0
	bhi	.L353
	ldr	r4, .L359
	ldr	r0, [r4, r0, asl #2]
	cmp	r0, #0
	beq	.L354
	ldr	r3, .L359+4
	mov	r0, #3
	str	r1, [fp, #-80]
	mov	r6, #0
	ldr	r3, [r3, #12]
	blx	r3
	ldr	r5, [r4, r9, asl #2]
	ldr	r1, [fp, #-80]
	mov	r3, r5
.L325:
	ldrsb	r2, [r3, #56]
	cmp	r2, #0
	bne	.L323
	ldrsb	r7, [r3, #57]
	cmp	r7, #0
	beq	.L355
.L323:
	add	r6, r6, #1
	add	r3, r3, #664
	cmp	r6, #32
	bne	.L325
.L326:
	ldr	r1, .L359+8
	mov	r0, #0
	bl	dprint_vfmw
.L352:
	ldr	r3, .L359+4
	mov	r0, #3
	ldr	r3, [r3, #16]
	blx	r3
	mvn	r0, #0
.L351:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L355:
	mov	r4, #664
	mul	r4, r4, r6
	add	r10, r5, r4
	add	r4, r4, #56
	add	r2, r10, #56
	adds	r4, r5, r4
	strb	r6, [r2, #3]
	strb	r7, [r2, #2]
	beq	.L326
	add	ip, r10, #704
	cmp	r1, #0
	str	r7, [r10, #696]
	str	r7, [r10, #700]
	str	r7, [r10, #704]
	str	r7, [ip, #4]
	bne	.L327
	add	r1, r5, #24576
	add	r3, r5, #21248
	add	r3, r3, #56
	str	r3, [r10, #696]
	str	r3, [r10, #700]
	mov	r0, #1
	str	r3, [r10, #704]
	ldr	r3, [r1, #3816]
	str	r3, [ip, #4]
	ldrb	r3, [r5, #13]	@ zero_extendqisi2
	strb	r0, [r2, #2]
	sub	r3, r3, #1
	strb	r3, [r2, #4]
.L328:
	mov	r0, #664
	add	r3, r4, #512
	mul	r6, r0, r6
	str	r3, [fp, #-80]
	mov	lr, #1
	mov	r1, #0
	mov	r2, #624
	mov	r7, r1
	add	r3, r5, r6
	add	r0, r6, #64
	add	ip, r3, #56
	add	r6, r4, #544
	strb	lr, [ip, #1]
	add	r0, r5, r0
	mov	r9, r6
	ldr	r6, [r3, #700]
	mov	r8, ip
	ldr	ip, .L359+4
	mov	r10, r9
	strb	lr, [r6, #2]
	str	lr, [r3, #692]
	ldr	r6, [r3, #696]
	ldr	lr, [r5, #16]
	str	lr, [r3, #688]
	str	lr, [r6, #88]
	ldr	lr, [r3, #700]
	ldr	r6, [r3, #688]
	str	r6, [lr, #88]
	ldr	lr, [r5, #16]
	add	lr, lr, #1
	str	lr, [r5, #16]
	ldr	r5, [r3, #696]
	ldr	r6, [r3, #700]
	ldr	r3, [r3, #704]
	str	r3, [fp, #-92]
	ldr	r3, [ip, #48]
	blx	r3
	ldrsb	r2, [r8, #3]
	str	r8, [fp, #-84]
	str	r2, [r4, #164]
	ldr	r2, [r5, #68]
	str	r2, [r4, #120]
	ldr	r2, [r5, #72]
	str	r2, [r4, #124]
	ldr	r2, [r6, #32]
	str	r2, [r4, #148]
	ldr	r2, [r6, #60]
	str	r2, [r4, #152]
	ldr	r2, [r6, #64]
	str	r2, [r4, #156]
	ldr	r1, [r5, #16]
	str	r1, [r4, #52]
	mov	r0, r1
	ldr	r2, [r5, #76]
	add	r2, r2, r1
	str	r2, [r4, #56]
	bl	MEM_Phy2Vir
	ldr	r8, [fp, #-80]
	str	r0, [r4, #512]
	str	r7, [r8, #4]
	ldr	r0, [r4, #56]
	bl	MEM_Phy2Vir
	str	r7, [r4, #524]
	str	r0, [r4, #520]
	ldr	r0, [r5, #24]
	str	r0, [r4, #236]
	bl	MEM_Phy2Vir
	str	r7, [r4, #244]
	str	r0, [r4, #240]
	ldr	r0, [r6, #16]
	str	r0, [r4, #36]
	ldr	r2, [r6, #76]
	add	r2, r0, r2
	str	r2, [r4, #40]
	ldr	r1, [r6, #32]
	add	r1, r1, r0
	str	r1, [r4, #44]
	ldr	r1, [r6, #32]
	add	r2, r2, r1
	str	r2, [r4, #48]
	ldr	r1, [r6, #16]
	str	r1, [r4, #68]
	mov	r0, r1
	ldr	r2, [r6, #76]
	add	r2, r2, r1
	str	r2, [r4, #72]
	bl	MEM_Phy2Vir
	str	r7, [r4, #532]
	str	r0, [r4, #528]
	ldr	r0, [r4, #72]
	bl	MEM_Phy2Vir
	ldr	r3, [fp, #-92]
	str	r7, [r9, #-4]
	add	r1, r4, #560
	str	r8, [fp, #-80]
	str	r0, [r9, #-8]
	ldr	r2, [r3, #16]
	ldr	r0, [r4, #52]
	str	r2, [r4, #60]
	ldr	r3, [r5, #76]
	add	r3, r3, r2
	str	r3, [r4, #64]
	ldr	r3, [r5, #48]
	str	r3, [r4, #160]
	ldr	r2, [r5, #52]
	add	r2, r2, r0
	str	r2, [r4, #76]
	ldr	r2, [r5, #52]
	ldr	r3, [r5, #56]
	add	r3, r2, r3
	add	r3, r3, r0
	str	r3, [r4, #80]
	ldrd	r2, [r8]
	ldr	r0, [r5, #52]
	mov	r8, r2
	adds	r2, r2, r0
	mov	r9, r3
	adc	r3, r3, r0, asr #31
	strd	r2, [r1, #-8]
	ldr	r2, [r5, #56]
	ldr	r0, [r5, #52]
	strd	r8, [fp, #-92]
	mov	r9, r2, asr #31
	adds	r2, r2, r0
	adc	r3, r9, r0, asr #31
	ldrd	r8, [fp, #-92]
	adds	r8, r8, r2
	adc	r9, r9, r3
	mov	r2, r8
	mov	r3, r9
	strd	r2, [r1]
	ldr	r3, [r5, #28]
	str	r3, [r4, #228]
	ldr	r0, [r5, #28]
	bl	MEM_Phy2Vir
	str	r0, [r4, #544]
	str	r7, [r10, #4]
	ldr	r3, [r6, #40]
	str	r3, [r4, #96]
	ldr	r3, [r6, #44]
	str	r3, [r4, #100]
	ldr	r3, [r5, #36]
	str	r3, [r4, #84]
	ldr	r0, [r5, #36]
	bl	MEM_Phy2Vir
	ldr	r2, [fp, #-80]
	ldr	r3, .L359+4
	str	r7, [r2, #-4]
	ldr	r3, [r3, #16]
	str	r0, [r2, #-8]
	mov	r0, #3
	str	r7, [r4, #88]
	str	r7, [r4, #92]
	blx	r3
	ldr	ip, [fp, #-84]
	ldrsb	r0, [ip, #3]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L327:
	mov	r0, r9
	mov	r1, r4
	str	ip, [fp, #-92]
	str	r2, [fp, #-80]
	bl	FSP_FindEmptyDecodeFs
	ldr	r0, [r10, #696]
	ldr	r2, [fp, #-80]
	cmp	r0, #0
	ldr	ip, [fp, #-92]
	beq	.L356
	add	r3, r5, #27648
	str	ip, [fp, #-92]
	add	r3, r3, #156
	str	r3, [r10, #704]
	ldrsb	r3, [r5, #13]
	str	r2, [fp, #-80]
	cmp	r3, #0
	ble	.L331
	mov	r0, r9
	bl	VCTRL_GetVidStd
	ldr	r2, [fp, #-80]
	ldr	ip, [fp, #-92]
	cmp	r0, #17
	beq	.L332
	ldrsb	r1, [r5, #13]
	cmp	r1, #0
	addgt	r3, r5, #28160
	addgt	r3, r3, #236
	bgt	.L338
	b	.L334
.L335:
	add	r7, r7, #1
	add	r3, r3, #8
	cmp	r7, r1
	beq	.L334
.L338:
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L335
	ldrb	r2, [r3, #1]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L335
	mov	r3, #664
	add	r2, r7, #3536
	mla	r3, r3, r6, r5
	add	r2, r2, #13
	add	ip, r5, #24576
	cmp	r7, r1
	strb	r7, [r3, #60]
	ldr	r2, [r5, r2, asl #3]
	str	r2, [r3, #708]
	ldr	lr, [ip, #3812]
	str	lr, [r3, #712]
	bge	.L336
.L331:
	mov	r7, #664
	ldr	r10, .L359+12
	mla	r7, r7, r6, r5
	ldr	r3, [r10, #-3896]
	cmp	r3, #0
	ldr	r3, [r7, #696]
	str	r3, [r7, #700]
	beq	.L328
	mov	r0, r9
	bl	VCTRL_GetImageWidth
	str	r0, [fp, #-80]
	mov	r0, r9
	bl	VCTRL_GetImageHeight
	ldr	ip, [r10, #-3888]
	ldr	r2, [fp, #-80]
	cmp	ip, #0
	mov	r3, r0
	beq	.L357
	mov	r0, r9
	sub	r1, fp, #72
	blx	ip
	cmp	r0, #0
	bne	.L342
	ldr	r9, [fp, #-68]
	cmp	r9, #0
	beq	.L358
	ldr	r3, [r7, #696]
	str	r9, [r3, #16]
	ldrsb	r3, [r7, #60]
	ldr	r2, [fp, #-52]
	add	r3, r3, #3536
	add	r3, r3, #13
	str	r2, [r7, #708]
	str	r2, [r5, r3, asl #3]
	b	.L328
.L334:
	add	ip, r5, #24576
	cmp	r1, #0
	ldr	lr, [ip, #3812]
	ble	.L340
.L336:
	mov	r2, #0
	mov	r0, r2
.L339:
	mov	r3, r2, asl #3
	add	r2, r2, #1
	add	r3, r3, #28160
	cmp	r2, r1
	add	r3, r3, #237
	strb	r0, [r5, r3]
	bne	.L339
.L340:
	mov	r3, #664
	mov	r2, #0
	mla	r3, r3, r6, r5
	mov	r0, r2
	ldr	r1, .L359+16
	strb	r2, [r3, #60]
	ldr	r2, [ip, #3816]
	str	lr, [r3, #712]
	str	r2, [r3, #708]
	bl	dprint_vfmw
	b	.L331
.L332:
	add	r3, r5, #24576
	strb	r7, [r2, #4]
	ldr	r2, [r3, #3816]
	str	r2, [ip, #4]
	ldr	r3, [r3, #3812]
	str	r3, [r10, #712]
	b	.L331
.L353:
	ldr	r3, .L359+20
	movw	r2, #1279
	ldr	r1, .L359+24
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L351
.L354:
	ldr	r3, .L359+28
	mov	r2, #1280
	ldr	r1, .L359+24
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L351
.L358:
	ldr	r3, .L359+4
	mov	r0, #3
	ldr	r3, [r3, #16]
	blx	r3
	mov	r0, r9
	ldr	r1, .L359+32
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L351
.L357:
	mov	r0, ip
	ldr	r1, .L359+36
	bl	dprint_vfmw
.L342:
	ldr	r3, .L359+4
	mov	r0, #3
	ldr	r3, [r3, #16]
	blx	r3
	ldr	r1, .L359+40
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L351
.L356:
	ldr	r2, .L359+44
	ldr	r1, .L359+48
	bl	dprint_vfmw
	b	.L352
.L360:
	.align	2
.L359:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC19
	.word	.LANCHOR1
	.word	.LC21
	.word	.LC0
	.word	.LC1
	.word	.LC4
	.word	.LC23
	.word	.LC5
	.word	.LC22
	.word	.LANCHOR2
	.word	.LC20
	UNWIND(.fnend)
	.size	FSP_NewLogicFs, .-FSP_NewLogicFs
	.align	2
	.global	FSP_GetPhyFsNum
	.type	FSP_GetPhyFsNum, %function
FSP_GetPhyFsNum:
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
	ldr	r6, .L369
	mov	r7, r0
	mov	r0, #3
	mov	r4, r1
	mov	r5, #0
	ldr	r3, [r6, #12]
	blx	r3
	ldr	r3, .L369+4
	ldr	ip, [r3, r7, asl #2]
	add	r3, ip, #21248
	add	ip, ip, #24576
	add	r3, r3, #252
	add	ip, ip, #124
	b	.L363
.L362:
	add	r3, r3, #100
	cmp	r3, ip
	beq	.L368
.L363:
	ldr	r2, [r3]
	cmp	r2, #1
	bne	.L362
	ldr	r2, [r3, #-76]
	add	r3, r3, #100
	cmp	r4, #0
	cmpne	r2, r4
	addeq	r5, r5, #1
	cmp	r3, ip
	bne	.L363
.L368:
	ldr	r3, [r6, #16]
	mov	r0, #3
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L370:
	.align	2
.L369:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	FSP_GetPhyFsNum, .-FSP_GetPhyFsNum
	.align	2
	.global	FSP_RelsePhyFs
	.type	FSP_RelsePhyFs, %function
FSP_RelsePhyFs:
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
	ldr	r7, .L398
	mov	r6, r0
	mov	r0, #3
	mov	r8, r1
	mov	r4, #0
	ldr	r3, [r7, #12]
	blx	r3
	ldr	r3, .L398+4
	ldr	r1, [r3, r6, asl #2]
	add	r3, r1, #21248
	add	r3, r3, #252
	b	.L375
.L372:
	add	r4, r4, #1
	add	r3, r3, #100
	cmp	r4, #32
	beq	.L395
.L375:
	ldr	r2, [r3]
	cmp	r2, #1
	bne	.L372
	ldrsb	r2, [r3, #-96]
	cmp	r2, #0
	bne	.L372
	ldrsb	r5, [r3, #-94]
	cmp	r5, #0
	bne	.L372
	ldr	r2, [r3, #-76]
	cmp	r2, r8
	beq	.L372
	mov	r3, #100
	mla	r10, r3, r4, r1
	add	r3, r10, #21248
	add	r3, r3, #168
	ldr	r8, [r3, #4]
	str	r3, [fp, #-64]
	mov	r0, r8
	bl	MEM_Phy2Vir
	ldr	r3, [fp, #-64]
	cmp	r4, #31
	ldr	r2, [r3, #8]
	mov	r9, r0
	bgt	.L395
	add	r3, r10, #21248
	mov	r1, r0
	mov	r0, r8
	str	r5, [r3, #252]
	bl	MEM_DelMemRecord
	ldr	r3, [r7, #16]
	mov	r0, #3
	blx	r3
	ldr	r3, [r7, #12]
	mov	r0, #8
	blx	r3
	ldr	r2, .L398+8
	mov	r3, r5
	ldr	r0, [r2, r6, asl #2]
	add	ip, r0, #311296
	add	r2, ip, #224
	b	.L379
.L377:
	add	r3, r3, #1
	add	r2, r2, #12
	cmp	r3, #32
	beq	.L380
.L379:
	ldrb	r1, [r2]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L377
	ldr	r1, [r2, #-8]
	cmp	r8, r1
	bne	.L377
	cmp	r3, #31
	movle	r2, r3, asl #4
	suble	r3, r2, r3, asl #2
	movle	r1, #0
	addle	ip, ip, r3
	strleb	r1, [ip, #224]
.L380:
	add	r3, r0, #307200
	mov	r4, #0
	add	r3, r3, #2000
	b	.L383
.L381:
	add	r4, r4, #1
	add	r3, r3, #72
	cmp	r4, #32
	beq	.L384
.L383:
	ldr	r2, [r3]
	cmp	r2, #255
	beq	.L381
	ldr	r2, [r3, #32]
	cmp	r8, r2
	bne	.L381
	cmp	r4, #31
	bgt	.L384
	ldr	r1, .L398+12
	mov	r3, r4, asl #6
	add	r3, r3, r4, lsl #3
	mov	r2, #255
	add	r3, r0, r3
	ldr	ip, [r1]
	add	r3, r3, #307200
	cmp	ip, #0
	str	r2, [r3, #2000]
	beq	.L384
	mov	r2, #0
	str	r8, [fp, #-60]
	str	r2, [fp, #-56]
	mov	r0, r6
	str	r2, [fp, #-48]
	mov	r3, #16
	str	r9, [fp, #-52]
	sub	r2, fp, #60
	mov	r1, #123
	blx	ip
.L384:
	ldr	r3, [r7, #16]
	mov	r0, #8
	blx	r3
	cmp	r4, #31
	movle	r0, #0
	mvngt	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L395:
	ldr	r3, [r7, #16]
	mov	r0, #3
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L399:
	.align	2
.L398:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	s_pstVfmwChan
	.word	g_event_report
	UNWIND(.fnend)
	.size	FSP_RelsePhyFs, .-FSP_RelsePhyFs
	.align	2
	.global	FSP_RemovePmv
	.type	FSP_RemovePmv, %function
FSP_RemovePmv:
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
	ldrhi	r3, .L409
	movwhi	r2, #1595
	bhi	.L406
	cmp	r1, #31
	bhi	.L407
	ldr	r3, .L409+4
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L408
	mov	r3, #664
	mla	r3, r3, r1, r0
	ldr	r2, [r3, #692]
	cmp	r2, #2
	ldmnefd	sp, {fp, sp, pc}
	ldrb	r3, [r3, #60]	@ zero_extendqisi2
	cmp	r3, #19
	bhi	.L405
	sxtb	r3, r3
	mov	r2, #0
	add	r3, r3, #3536
	add	r3, r3, #13
	add	r3, r0, r3, lsl #3
	strb	r2, [r3, #5]
.L405:
	mov	r2, #664
	mov	r3, #0
	mla	r1, r2, r1, r0
	str	r3, [r1, #692]
	ldmfd	sp, {fp, sp, pc}
.L407:
	ldr	r3, .L409+8
	movw	r2, #1596
.L406:
	ldr	r1, .L409+12
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L408:
	ldr	r3, .L409+16
	movw	r2, #1597
	ldr	r1, .L409+12
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L410:
	.align	2
.L409:
	.word	.LC0
	.word	.LANCHOR0
	.word	.LC24
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_RemovePmv, .-FSP_RemovePmv
	.align	2
	.global	FSP_SetRef
	.type	FSP_SetRef, %function
FSP_SetRef:
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
	mov	r3, r0
	mov	r6, r2
	mov	r4, r1
	ldrhi	r3, .L453
	movwhi	r2, #1625
	bhi	.L442
	cmp	r1, #31
	bhi	.L443
	ldr	r1, .L453+4
	ldr	r7, [r1, r3, asl #2]
	cmp	r7, #0
	beq	.L444
	mov	r5, #664
	uxtb	r8, r6
	mla	r5, r5, r4, r7
	ldrsb	r3, [r5, #58]
	strb	r8, [r5, #56]
	cmp	r3, #0
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r3, [r5, #696]
	cmp	r3, #0
	beq	.L417
	ldr	ip, [r3, #88]
	ldr	r1, [r5, #688]
	cmp	ip, r1
	beq	.L445
.L417:
	mov	r2, #664
	mla	r2, r2, r4, r7
	ldrb	r3, [r2, #60]	@ zero_extendqisi2
	cmp	r3, #19
	bhi	.L420
	cmp	r6, #1
	beq	.L446
	cmp	r6, #0
	bne	.L423
	ldr	r0, [r2, #692]
	cmp	r0, #2
	beq	.L447
.L429:
	mov	r3, #664
	mla	r3, r3, r4, r7
	ldr	r3, [r3, #700]
	cmp	r3, #0
	beq	.L434
	ldrsb	r2, [r3, #2]
	cmp	r2, #1
	moveq	r2, #0
	streqb	r2, [r3, #2]
.L434:
	mov	r3, #664
	mla	r4, r3, r4, r7
	str	r6, [r4, #696]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L443:
	ldr	r3, .L453+8
	movw	r2, #1626
.L442:
	ldr	r1, .L453+12
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L446:
	ldr	r1, [r2, #692]
	add	r2, r2, #688
	cmp	r1, #1
	beq	.L448
.L422:
	mov	r3, #664
	mla	r3, r3, r4, r7
	ldr	r2, [r3, #700]
	cmp	r2, #0
	beq	.L449
.L430:
	mov	r3, #664
	ldr	r0, [r2, #88]
	mla	r3, r3, r4, r7
	ldr	r1, [r3, #688]
	cmp	r6, #1
	cmpeq	r0, r1
	beq	.L450
.L426:
	cmp	r6, #0
	beq	.L434
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L420:
	sxtb	r3, r3
	movw	r2, #1678
	ldr	r1, .L453+16
	mov	r0, #0
	bl	dprint_vfmw
	cmp	r6, #0
	beq	.L429
	b	.L422
.L448:
	sxtb	r3, r3
	mov	r1, #2
	add	r3, r3, #3536
	add	r3, r3, #13
	add	r3, r7, r3, lsl #3
	strb	r6, [r3, #5]
	str	r1, [r2, #4]
.L423:
	mov	r3, #664
	mla	r3, r3, r4, r7
	ldr	r2, [r3, #700]
	cmp	r2, #0
	bne	.L430
	b	.L426
.L445:
	ldrsb	r1, [r3]
	cmp	r6, r1
	beq	.L418
	ldr	r1, .L453+20
	ldr	ip, [r1, #-3896]
	cmp	ip, #0
	bne	.L451
.L418:
	strb	r8, [r3]
	b	.L417
.L451:
	ldr	ip, [r1, #-3880]
	ldr	r1, [r3, #16]
	cmp	ip, #0
	beq	.L452
	blx	ip
	ldr	r3, [r5, #696]
	b	.L418
.L447:
	sxtb	r3, r3
	add	r3, r3, #3536
	add	r3, r3, #13
	add	r3, r7, r3, lsl #3
	strb	r6, [r3, #5]
	str	r6, [r2, #708]
	str	r6, [r2, #692]
	b	.L429
.L444:
	mov	r0, r7
	ldr	r3, .L453+24
	movw	r2, #1627
	ldr	r1, .L453+12
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L450:
	ldrsb	r1, [r2, #2]
	cmp	r1, #1
	moveq	r1, #2
	streqb	r1, [r2, #2]
	streqb	r1, [r3, #57]
	b	.L426
.L449:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L452:
	mov	r0, ip
	ldr	r1, .L453+28
	bl	dprint_vfmw
	ldr	r3, [r5, #696]
	b	.L418
.L454:
	.align	2
.L453:
	.word	.LC0
	.word	.LANCHOR0
	.word	.LC24
	.word	.LC1
	.word	.LC25
	.word	.LANCHOR1
	.word	.LC4
	.word	.LC5
	UNWIND(.fnend)
	.size	FSP_SetRef, .-FSP_SetRef
	.align	2
	.global	FSP_GetRef
	.type	FSP_GetRef, %function
FSP_GetRef:
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
	bhi	.L460
	cmp	r1, #31
	bhi	.L461
	ldr	r3, .L463
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L462
	mov	r0, #664
	mla	r1, r0, r1, r3
	ldrsb	r0, [r1, #56]
	ldmfd	sp, {fp, sp, pc}
.L460:
	ldr	r3, .L463+4
	movw	r2, #1717
	ldr	r1, .L463+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L461:
	ldr	r3, .L463+12
	movw	r2, #1718
	ldr	r1, .L463+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L462:
	mov	r0, r3
	movw	r2, #1719
	ldr	r3, .L463+16
	ldr	r1, .L463+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L464:
	.align	2
.L463:
	.word	.LANCHOR0
	.word	.LC26
	.word	.LC1
	.word	.LC27
	.word	.LC28
	UNWIND(.fnend)
	.size	FSP_GetRef, .-FSP_GetRef
	.align	2
	.global	FSP_SetDisplay
	.type	FSP_SetDisplay, %function
FSP_SetDisplay:
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
	mov	r8, r2
	mov	r5, r0
	mov	r4, r1
	ldrhi	r3, .L514
	movwhi	r2, #1736
	bhi	.L506
	cmp	r1, #31
	bhi	.L507
	ldr	r9, .L514+4
	ldr	r0, [r9, r0, asl #2]
	cmp	r0, #0
	beq	.L508
	ldr	r7, .L514+8
	mov	r0, #3
	clz	r6, r8
	ldr	r3, [r7, #12]
	mov	r6, r6, lsr #5
	blx	r3
	ldr	r9, [r9, r5, asl #2]
	mov	r3, #664
	mla	r3, r3, r4, r9
	add	r2, r3, #56
	ldrsb	r1, [r3, #58]
	cmp	r1, #0
	cmpeq	r8, #0
	bne	.L469
	ldr	r3, [r3, #700]
	cmp	r3, #0
	beq	.L469
	ldrsb	r2, [r2, #1]
	cmp	r2, #0
	beq	.L469
	ldr	r2, .L514+12
	ldr	r1, [r2, #-3896]
	cmp	r1, #0
	bne	.L509
.L469:
	mov	r2, #664
	cmp	r6, #0
	mla	r2, r2, r4, r9
	movne	r3, #0
	moveq	r3, #3
	add	r1, r2, #56
	ldrsb	ip, [r1, #2]
	strb	r3, [r1, #1]
	cmp	ip, #0
	bne	.L480
	add	r8, r2, #696
	ldr	r3, [r8, #4]
	cmp	r3, #0
	beq	.L480
	ldr	r3, [r3, #88]
	ldr	r2, [r2, #688]
	cmp	r3, r2
	bne	.L510
	cmp	r6, #0
	beq	.L475
	strb	ip, [r1, #1]
	mov	r0, r5
	ldr	r3, [r8, #4]
	strb	ip, [r3, #2]
	bl	VCTRL_GetVidStd
	cmp	r0, #17
	beq	.L511
.L477:
	mov	r2, #664
	mov	r3, #0
	mla	r4, r2, r4, r9
	add	r4, r4, #696
	str	r3, [r4, #4]
.L480:
	ldr	r3, [r7, #16]
	mov	r0, #3
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L507:
	ldr	r3, .L514+16
	movw	r2, #1737
.L506:
	ldr	r1, .L514+20
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L475:
	mov	r0, r5
	bl	VCTRL_GetVidStd
	cmp	r0, #17
	beq	.L512
.L479:
	mov	r3, #664
	mov	r2, #3
	mla	r4, r3, r4, r9
	ldr	r3, [r4, #700]
	strb	r2, [r4, #57]
	strb	r2, [r3, #2]
	b	.L480
.L510:
	mov	r0, ip
	ldr	r1, .L514+24
	bl	dprint_vfmw
	b	.L480
.L508:
	ldr	r3, .L514+28
	movw	r2, #1738
	ldr	r1, .L514+20
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L509:
	ldr	r2, [r2, #-3884]
	ldr	r1, [r3, #16]
	cmp	r2, #0
	beq	.L513
	mov	r0, r5
	blx	r2
	b	.L469
.L511:
	ldr	r3, [r8, #4]
	cmp	r3, #0
	beq	.L477
	ldr	r2, [r3, #4]
	cmp	r2, #0
	beq	.L477
	sub	r2, r2, #1
	str	r2, [r3, #4]
	ldr	r3, [r8, #4]
	ldr	r2, [r3, #4]
	cmp	r2, #0
	movne	r2, #3
	strneb	r2, [r3, #2]
	b	.L477
.L512:
	ldr	r3, [r8, #4]
	cmp	r3, #0
	ldrne	r2, [r3, #4]
	addne	r2, r2, #1
	strne	r2, [r3, #4]
	b	.L479
.L513:
	mov	r0, r2
	ldr	r1, .L514+32
	bl	dprint_vfmw
	b	.L469
.L515:
	.align	2
.L514:
	.word	.LC0
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1
	.word	.LC24
	.word	.LC1
	.word	.LC29
	.word	.LC4
	.word	.LC6
	UNWIND(.fnend)
	.size	FSP_SetDisplay, .-FSP_SetDisplay
	.align	2
	.global	FSP_ClearNotInVoQueue
	.type	FSP_ClearNotInVoQueue, %function
FSP_ClearNotInVoQueue:
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
	mov	r7, r1
	bhi	.L527
	cmp	r1, #0
	beq	.L528
	ldr	r3, .L531
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	addne	r6, r0, #21248
	addne	r4, r0, #64
	addne	r6, r6, #64
	bne	.L522
	b	.L529
.L521:
	add	r4, r4, #664
	cmp	r4, r6
	beq	.L530
.L522:
	mov	r2, #0
	ldr	r1, [r4, #156]
	mov	r0, r5
	bl	FSP_SetRef
	mov	r1, r4
	mov	r0, r7
	bl	IsImageInVoQueue
	cmp	r0, #0
	beq	.L521
	ldr	r1, [r4, #156]
	mov	r2, #0
	mov	r0, r5
	add	r4, r4, #664
	bl	FSP_SetDisplay
	cmp	r4, r6
	bne	.L522
.L530:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L527:
	ldr	r3, .L531+4
	mov	r2, #508
	ldr	r1, .L531+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L529:
	ldr	r3, .L531+12
	movw	r2, #510
	ldr	r1, .L531+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L528:
	mov	r0, r1
	ldr	r3, .L531+16
	movw	r2, #509
	ldr	r1, .L531+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L532:
	.align	2
.L531:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC4
	.word	.LC30
	UNWIND(.fnend)
	.size	FSP_ClearNotInVoQueue, .-FSP_ClearNotInVoQueue
	.align	2
	.global	FSP_GetDisplay
	.type	FSP_GetDisplay, %function
FSP_GetDisplay:
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
	ldr	r3, .L537
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L536
	mov	r0, #664
	mla	r1, r0, r1, r3
	ldrsb	r0, [r1, #57]
	ldmfd	sp, {fp, sp, pc}
.L536:
	mov	r0, r3
	movw	r2, #1829
	ldr	r3, .L537+4
	ldr	r1, .L537+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L538:
	.align	2
.L537:
	.word	.LANCHOR0
	.word	.LC4
	.word	.LC1
	UNWIND(.fnend)
	.size	FSP_GetDisplay, .-FSP_GetDisplay
	.align	2
	.global	FSP_SetStoreType
	.type	FSP_SetStoreType, %function
FSP_SetStoreType:
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
	bhi	.L547
	ldr	ip, .L550
	ldr	ip, [ip, r0, asl #2]
	cmp	ip, #0
	beq	.L548
	cmp	r1, #0
	bne	.L543
	ldrsb	r0, [ip, #10]
	cmp	r0, r2
	ble	.L546
	mov	lr, #100
	mov	r0, r1
	mla	r2, lr, r2, ip
	add	r2, r2, #21248
	str	r3, [r2, #168]
	ldmfd	sp, {fp, sp, pc}
.L543:
	cmp	r1, #1
	beq	.L549
.L546:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L549:
	ldrsb	r1, [ip, #11]
	cmp	r2, r1
	bge	.L546
	mov	r1, #100
	mov	r0, #0
	mla	r2, r1, r2, ip
	add	r2, r2, #24576
	str	r3, [r2, #40]
	ldmfd	sp, {fp, sp, pc}
.L547:
	ldr	r3, .L550+4
	movw	r2, #1845
	ldr	r1, .L550+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L548:
	mov	r0, ip
	ldr	r3, .L550+12
	movw	r2, #1846
	ldr	r1, .L550+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L551:
	.align	2
.L550:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_SetStoreType, .-FSP_SetStoreType
	.align	2
	.global	FSP_GetStoreType
	.type	FSP_GetStoreType, %function
FSP_GetStoreType:
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
	bhi	.L560
	ldr	r3, .L563
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L561
	cmp	r1, #0
	bne	.L556
	ldrsb	r3, [r0, #10]
	cmp	r3, r2
	ble	.L559
	mov	r3, #100
	mla	r2, r3, r2, r0
	add	r2, r2, #21248
	ldr	r0, [r2, #168]
	ldmfd	sp, {fp, sp, pc}
.L556:
	cmp	r1, #1
	beq	.L562
.L559:
	mov	r0, #2
	ldmfd	sp, {fp, sp, pc}
.L560:
	ldr	r3, .L563+4
	movw	r2, #1878
	ldr	r1, .L563+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L561:
	ldr	r3, .L563+12
	movw	r2, #1879
	ldr	r1, .L563+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L562:
	ldrsb	r3, [r0, #11]
	cmp	r2, r3
	bge	.L559
	mov	r3, #100
	mla	r2, r3, r2, r0
	add	r2, r2, #24576
	ldr	r0, [r2, #40]
	ldmfd	sp, {fp, sp, pc}
.L564:
	.align	2
.L563:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_GetStoreType, .-FSP_GetStoreType
	.align	2
	.global	FSP_ClearLogicFs
	.type	FSP_ClearLogicFs, %function
FSP_ClearLogicFs:
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
	cmpls	r1, #31
	mov	r4, r0
	mov	r5, r1
	ldmhifd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r6, .L575
	ldr	r3, [r6, r0, asl #2]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r2, #0
	bl	FSP_SetRef
	ldr	r2, [r6, r4, asl #2]
	mov	r3, #664
	mla	r3, r3, r5, r2
	ldrsb	r3, [r3, #57]
	cmp	r3, #3
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r1, r5
	mov	r0, r4
	mov	r2, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	FSP_SetDisplay
.L576:
	.align	2
.L575:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	FSP_ClearLogicFs, .-FSP_ClearLogicFs
	.align	2
	.global	FSP_GetLogicFs
	.type	FSP_GetLogicFs, %function
FSP_GetLogicFs:
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
	cmpls	r1, #31
	movls	r3, #1
	movhi	r3, #0
	bhi	.L579
	ldr	r3, .L581
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, #664
	mlane	r1, r3, r1, r0
	addne	r0, r1, #56
	ldmfd	sp, {fp, sp, pc}
.L579:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L582:
	.align	2
.L581:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	FSP_GetLogicFs, .-FSP_GetLogicFs
	.align	2
	.global	FSP_GetFsImagePtr
	.type	FSP_GetFsImagePtr, %function
FSP_GetFsImagePtr:
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
	cmpls	r1, #31
	movls	r3, #1
	movhi	r3, #0
	bhi	.L585
	ldr	r3, .L587
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, #664
	mlane	r1, r3, r1, r0
	addne	r0, r1, #64
	ldmfd	sp, {fp, sp, pc}
.L585:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L588:
	.align	2
.L587:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	FSP_GetFsImagePtr, .-FSP_GetFsImagePtr
	.align	2
	.global	FSP_GetDecPhyFs
	.type	FSP_GetDecPhyFs, %function
FSP_GetDecPhyFs:
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
	cmpls	r1, #31
	movls	r3, #1
	movhi	r3, #0
	bhi	.L591
	ldr	r3, .L593
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, #664
	mlane	r0, r3, r1, r0
	ldrne	r0, [r0, #696]
	ldmfd	sp, {fp, sp, pc}
.L591:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L594:
	.align	2
.L593:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	FSP_GetDecPhyFs, .-FSP_GetDecPhyFs
	.align	2
	.global	FSP_GetDispPhyFs
	.type	FSP_GetDispPhyFs, %function
FSP_GetDispPhyFs:
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
	cmpls	r1, #31
	movls	r3, #1
	movhi	r3, #0
	bhi	.L597
	ldr	r3, .L599
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, #664
	mlane	r0, r3, r1, r0
	ldrne	r0, [r0, #700]
	ldmfd	sp, {fp, sp, pc}
.L597:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L600:
	.align	2
.L599:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	FSP_GetDispPhyFs, .-FSP_GetDispPhyFs
	.align	2
	.global	FSP_GetPmvAddr
	.type	FSP_GetPmvAddr, %function
FSP_GetPmvAddr:
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
	cmpls	r1, #31
	movls	r3, #1
	movhi	r3, #0
	bhi	.L603
	ldr	r3, .L605
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, #664
	mlane	r0, r3, r1, r0
	ldrne	r0, [r0, #708]
	ldmfd	sp, {fp, sp, pc}
.L603:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L606:
	.align	2
.L605:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	FSP_GetPmvAddr, .-FSP_GetPmvAddr
	.align	2
	.global	FSP_GetDecFsAddrTab
	.type	FSP_GetDecFsAddrTab, %function
FSP_GetDecFsAddrTab:
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
	cmp	r0, #15
	bhi	.L618
	cmp	r2, #0
	moveq	r0, r2
	ldreq	r3, .L620
	movweq	r2, #2026
	beq	.L617
	cmp	r1, #0
	moveq	r0, r1
	ldreq	r3, .L620+4
	movweq	r2, #2027
	beq	.L617
	ldr	ip, .L620+8
	ldr	lr, [ip, r0, asl #2]
	cmp	lr, #0
	beq	.L619
	mov	lr, #0
	str	lr, [r1]
	ldr	r4, [ip, r0, asl #2]
	ldrsb	r4, [r4, #10]
	cmp	r4, lr
	str	r4, [r1]
	ldr	ip, [ip, r0, asl #2]
	add	ip, ip, #21248
	add	ip, ip, #156
	ldmlefd	sp, {r4, fp, sp, pc}
	sub	r2, r2, #4
	sub	r3, r3, #4
.L614:
	ldr	r0, [ip, #16]
	add	lr, lr, #1
	add	ip, ip, #100
	str	r0, [r2, #4]!
	ldr	r0, [ip, #-72]
	str	r0, [r3, #4]!
	ldr	r4, [r1]
	cmp	r4, lr
	bgt	.L614
	ldmfd	sp, {r4, fp, sp, pc}
.L618:
	ldr	r3, .L620+12
	movw	r2, #2025
	ldr	r1, .L620+16
	mov	r0, #0
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	dprint_vfmw
.L619:
	ldr	r3, .L620+20
	mov	r0, lr
	movw	r2, #2028
.L617:
	ldr	r1, .L620+16
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	dprint_vfmw
.L621:
	.align	2
.L620:
	.word	.LC31
	.word	.LC32
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_GetDecFsAddrTab, .-FSP_GetDecFsAddrTab
	.align	2
	.global	FSP_GetPmvAddrTab
	.type	FSP_GetPmvAddrTab, %function
FSP_GetPmvAddrTab:
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
	bhi	.L633
	cmp	r2, #0
	moveq	r0, r2
	ldreq	r3, .L635
	movweq	r2, #2059
	beq	.L632
	cmp	r1, #0
	moveq	r0, r1
	ldreq	r3, .L635+4
	movweq	r2, #2060
	beq	.L632
	ldr	ip, .L635+8
	ldr	r3, [ip, r0, asl #2]
	cmp	r3, #0
	beq	.L634
	mov	r3, #0
	str	r3, [r1]
	ldr	ip, [ip, r0, asl #2]
	ldrsb	r0, [ip, #13]
	str	r0, [r1]
	ldrsb	r1, [ip, #13]
	cmp	r1, r3
	ldmlefd	sp, {fp, sp, pc}
	sub	r2, r2, #4
	mov	r1, r3
.L629:
	mov	r3, r1, asl #3
	add	r1, r1, #1
	add	r3, r3, #28160
	add	r3, r3, #232
	ldr	r3, [ip, r3]
	str	r3, [r2, #4]!
	ldrsb	r3, [ip, #13]
	cmp	r3, r1
	bgt	.L629
	ldmfd	sp, {fp, sp, pc}
.L633:
	ldr	r3, .L635+12
	movw	r2, #2058
	ldr	r1, .L635+16
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L634:
	mov	r0, r3
	ldr	r3, .L635+20
	movw	r2, #2061
.L632:
	ldr	r1, .L635+16
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L636:
	.align	2
.L635:
	.word	.LC33
	.word	.LC34
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_GetPmvAddrTab, .-FSP_GetPmvAddrTab
	.align	2
	.global	FSP_GetInstInfo
	.type	FSP_GetInstInfo, %function
FSP_GetInstInfo:
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
	ldr	r3, .L640
	mov	ip, r1
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L639
	ldrb	lr, [r3, #10]	@ zero_extendqisi2
	add	r4, r3, #20480
	mov	r0, r1
	ldr	r5, .L640+4
	add	r1, r3, #21248
	mov	r2, #3200
	strb	lr, [ip, #1]
	add	r1, r1, #156
	ldr	lr, [r4, #1040]
	ldr	r4, [r4, #940]
	rsb	lr, r4, lr
	str	lr, [ip, #3208]
	ldrb	lr, [r3, #11]	@ zero_extendqisi2
	strb	lr, [ip, #2]
	ldrb	lr, [r3, #12]	@ zero_extendqisi2
	strb	lr, [ip, #3]
	ldrb	lr, [r3]	@ zero_extendqisi2
	strb	lr, [ip, #4]
	ldrb	r3, [r3, #13]	@ zero_extendqisi2
	strb	r3, [r0], #8
	ldr	r3, [r5, #52]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L639:
	mov	r0, r3
	movw	r2, #2079
	ldr	r3, .L640+8
	ldr	r1, .L640+12
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L641:
	.align	2
.L640:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC4
	.word	.LC1
	UNWIND(.fnend)
	.size	FSP_GetInstInfo, .-FSP_GetInstInfo
	.align	2
	.global	FSP_GetLogicIdByPhyAddr
	.type	FSP_GetLogicIdByPhyAddr, %function
FSP_GetLogicIdByPhyAddr:
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
	bhi	.L653
	ldr	r3, .L655
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, r0
	movne	r0, #0
	beq	.L654
.L647:
	ldr	r2, [r3, #696]
	add	r3, r3, #664
	cmp	r2, #0
	beq	.L646
	ldr	r2, [r2, #16]
	cmp	r2, r1
	ldmeqfd	sp, {fp, sp, pc}
.L646:
	add	r0, r0, #1
	cmp	r0, #32
	bne	.L647
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L653:
	ldr	r3, .L655+4
	movw	r2, #2102
	ldr	r1, .L655+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L654:
	ldr	r3, .L655+12
	movw	r2, #2103
	ldr	r1, .L655+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L656:
	.align	2
.L655:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_GetLogicIdByPhyAddr, .-FSP_GetLogicIdByPhyAddr
	.align	2
	.global	FSP_PrintContext
	.type	FSP_PrintContext, %function
FSP_PrintContext:
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
	bhi	.L687
	ldr	r3, .L690
	ldr	r7, [r3, r0, asl #2]
	cmp	r7, #0
	beq	.L688
	ldr	r1, .L690+4
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r7, #4]
	ldr	r2, .L690+8
	mov	r0, #0
	ldr	r1, .L690+12
	mov	r5, r7
	bl	dprint_vfmw
	ldr	r3, [r7, #28]
	ldr	r2, .L690+16
	mov	r0, #0
	ldr	r1, .L690+12
	mov	r6, #1
	bl	dprint_vfmw
	mov	r0, #0
	mov	r4, r0
	ldr	r3, [r7, #32]
	ldr	r2, .L690+20
	ldr	r1, .L690+12
	bl	dprint_vfmw
	ldr	r3, [r7, #44]
	ldr	r2, .L690+24
	mov	r0, r4
	ldr	r1, .L690+12
	bl	dprint_vfmw
	ldr	r3, [r7, #48]
	ldr	r2, .L690+28
	mov	r0, r4
	ldr	r1, .L690+12
	bl	dprint_vfmw
	ldrsb	r3, [r7, #23]
	ldr	r2, .L690+32
	mov	r0, r4
	ldr	r1, .L690+12
	bl	dprint_vfmw
	ldrsb	r3, [r7, #25]
	ldr	r2, .L690+36
	mov	r0, r4
	ldr	r1, .L690+12
	bl	dprint_vfmw
	ldrsb	r3, [r7, #26]
	ldr	r2, .L690+40
	mov	r0, r4
	ldr	r1, .L690+12
	bl	dprint_vfmw
	ldrsb	r3, [r7, #10]
	ldr	r2, .L690+44
	mov	r0, r4
	ldr	r1, .L690+12
	bl	dprint_vfmw
	ldrsb	r3, [r7, #11]
	ldr	r2, .L690+48
	mov	r0, r4
	ldr	r1, .L690+12
	bl	dprint_vfmw
	ldrsb	r3, [r7, #13]
	ldr	r2, .L690+52
	mov	r0, r4
	ldr	r1, .L690+12
	bl	dprint_vfmw
	mov	r2, #32
	ldr	r1, .L690+56
	mov	r0, r4
	bl	dprint_vfmw
	b	.L660
.L662:
	ldrsb	r3, [r5, #57]
	mov	r0, #0
	ldrsb	r2, [r5, #56]
	ldr	r1, .L690+60
	bl	dprint_vfmw
	cmp	r6, #31
	bgt	.L689
.L663:
	add	r4, r4, #1
	add	r6, r6, #1
	add	r5, r5, #664
.L660:
	cmp	r4, #0
	beq	.L661
	ands	r0, r4, #7
	bne	.L662
	ldr	r1, .L690+64
	bl	dprint_vfmw
	b	.L662
.L689:
	ldrsb	r2, [r7, #10]
	mov	r0, #0
	ldr	r1, .L690+68
	bl	dprint_vfmw
	ldrsb	r3, [r7, #10]
	cmp	r3, #0
	addgt	r5, r7, #21248
	movgt	r4, #0
	addgt	r5, r5, #156
	ble	.L670
.L667:
	ldrsb	r3, [r5, #2]
	mov	r0, #0
	ldrsb	r2, [r5]
	add	r4, r4, #1
	ldr	r1, .L690+60
	add	r5, r5, #100
	bl	dprint_vfmw
	ldrsb	r3, [r7, #10]
	cmp	r3, r4
	ble	.L670
	cmp	r4, #0
	beq	.L667
	ands	r0, r4, #7
	bne	.L667
	ldr	r1, .L690+64
	bl	dprint_vfmw
	b	.L667
.L670:
	ldrsb	r2, [r7, #11]
	mov	r0, #0
	ldr	r1, .L690+72
	bl	dprint_vfmw
	ldrsb	r3, [r7, #11]
	cmp	r3, #0
	addgt	r5, r7, #24576
	movgt	r4, #0
	addgt	r5, r5, #28
	ble	.L666
.L673:
	ldrsb	r3, [r5, #2]
	mov	r0, #0
	ldrsb	r2, [r5]
	add	r4, r4, #1
	ldr	r1, .L690+60
	add	r5, r5, #100
	bl	dprint_vfmw
	ldrsb	r3, [r7, #11]
	cmp	r3, r4
	ble	.L666
	cmp	r4, #0
	beq	.L673
	ands	r0, r4, #7
	bne	.L673
	ldr	r1, .L690+64
	bl	dprint_vfmw
	b	.L673
.L666:
	ldrsb	r2, [r7, #13]
	mov	r0, #0
	ldr	r1, .L690+76
	bl	dprint_vfmw
	ldrsb	r3, [r7, #13]
	cmp	r3, #0
	ble	.L671
	mov	r4, #0
.L672:
	mov	r3, r4, asl #3
	ldr	r1, .L690+80
	add	r3, r3, #28160
	mov	r0, #0
	add	r3, r3, #237
	add	r4, r4, #1
	ldrb	r2, [r7, r3]	@ zero_extendqisi2
	bl	dprint_vfmw
	ldrsb	r3, [r7, #13]
	cmp	r3, r4
	ble	.L671
	cmp	r4, #0
	beq	.L672
	ands	r0, r4, #15
	bne	.L672
	ldr	r1, .L690+64
	bl	dprint_vfmw
	b	.L672
.L671:
	ldr	r1, .L690+84
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L687:
	ldr	r3, .L690+88
	movw	r2, #2134
	ldr	r1, .L690+92
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L688:
	mov	r0, r7
	ldr	r3, .L690+96
	movw	r2, #2135
	ldr	r1, .L690+92
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L661:
	ldrsb	r3, [r5, #57]
	mov	r0, r4
	ldrsb	r2, [r5, #56]
	ldr	r1, .L690+60
	bl	dprint_vfmw
	b	.L663
.L691:
	.align	2
.L690:
	.word	.LANCHOR0
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.word	.LC38
	.word	.LC39
	.word	.LC40
	.word	.LC41
	.word	.LC42
	.word	.LC43
	.word	.LC44
	.word	.LC45
	.word	.LC46
	.word	.LC47
	.word	.LC48
	.word	.LC50
	.word	.LC49
	.word	.LC51
	.word	.LC52
	.word	.LC53
	.word	.LC55
	.word	.LC54
	.word	.LC0
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_PrintContext, .-FSP_PrintContext
	.align	2
	.global	FSP_IsRefLeak
	.type	FSP_IsRefLeak, %function
FSP_IsRefLeak:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	ldr	r2, [r0, #28]
	mov	r3, #0
	mov	r5, r0
	cmp	r2, #1920
	mov	r8, r1
	str	r3, [fp, #-48]
	str	r3, [fp, #-44]
	movgt	r7, #1
	str	r3, [fp, #-40]
	bgt	.L693
	ldr	r3, [r0, #32]
	cmp	r3, #1088
	movgt	r7, #1
	movle	r7, #4
.L693:
	ldrsb	r1, [r5, #10]
	cmp	r1, #0
	ble	.L702
	mov	r6, #0
	add	r3, r5, #21248
	add	r3, r3, #252
	mov	r4, r6
	mov	r2, r6
	b	.L697
.L695:
	add	r2, r2, #1
	add	r3, r3, #100
	cmp	r2, r1
	beq	.L694
.L697:
	ldr	ip, [r3]
	cmp	ip, #1
	bne	.L695
	ldrb	r0, [r3, #-96]	@ zero_extendqisi2
	cmp	r0, #1
	addeq	r4, r4, #1
	beq	.L695
	ldrsb	ip, [r3, #-94]
	cmp	ip, #2
	bne	.L695
	cmp	r0, #0
	add	r2, r2, #1
	addeq	r6, r6, #1
	cmp	r2, r1
	add	r3, r3, #100
	bne	.L697
.L694:
	sub	r3, fp, #44
	sub	r2, fp, #40
	sub	r1, fp, #48
	mov	r0, r8
	bl	VCTRL_GetChanImgNum
	ldr	r3, [fp, #-48]
	ldrsb	r2, [r5, #10]
	add	r3, r7, r3
	cmp	r3, r2
	bcc	.L705
	add	r4, r4, r6
	add	r7, r4, r7
	cmp	r2, r7
	bhi	.L705
	ldr	r3, .L710
	ldr	r3, [r3, r8, asl #2]
	ldr	r3, [r3, #620]
	cmp	r3, #2
	beq	.L705
	mov	r0, r8
	bl	FSP_PrintContext
	mov	r0, #1
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L705:
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L702:
	mov	r6, #0
	mov	r4, r6
	b	.L694
.L711:
	.align	2
.L710:
	.word	s_pstVfmwChan
	UNWIND(.fnend)
	.size	FSP_IsRefLeak, .-FSP_IsRefLeak
	.align	2
	.global	FSP_IsNewFsAvalible
	.type	FSP_IsNewFsAvalible, %function
FSP_IsNewFsAvalible:
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
	mov	r7, r0
	bhi	.L761
	ldr	r3, .L767
	ldr	r4, [r3, r0, asl #2]
	cmp	r4, #0
	beq	.L762
	ldr	r3, .L767+4
	ldr	r2, [r3, #-3896]
	cmp	r2, #0
	bne	.L716
.L720:
	ldrsb	r3, [r4, #10]
	cmp	r3, #0
	bne	.L763
.L717:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L763:
	mov	r0, r7
	bl	VCTRL_GetVidStd
	cmp	r0, #17
	beq	.L721
	mov	r0, r7
	bl	VCTRL_GetVidStd
	cmp	r0, #16
	beq	.L722
.L725:
	ldrsb	r8, [r4, #10]
	cmp	r8, #0
	addgt	r5, r4, #21248
	movgt	r6, #0
	addgt	r5, r5, #156
	bgt	.L727
	b	.L724
.L764:
	add	r6, r6, #1
	cmp	r6, r8
	beq	.L724
.L727:
	mov	r1, r5
	mov	r0, r7
	bl	FSP_IsFsStatusReady
	add	r5, r5, #100
	cmp	r0, #0
	beq	.L764
	cmp	r8, r6
	ble	.L724
.L732:
	mov	r0, r7
	bl	VCTRL_GetVidStd
	cmp	r0, #17
	beq	.L717
	ldrsb	r0, [r4, #13]
	cmp	r0, #0
	ble	.L740
	add	r3, r4, #28160
	mov	r2, #0
	add	r3, r3, #236
	b	.L736
.L734:
	add	r2, r2, #1
	add	r3, r3, #8
	cmp	r2, r0
	beq	.L737
.L736:
	ldrb	r1, [r3]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L734
	ldrb	r1, [r3, #1]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L734
	cmp	r2, r0
	blt	.L717
.L737:
	cmp	r0, r2
	beq	.L765
.L724:
	mov	r0, r7
	bl	FSP_IsFsLeak
	cmp	r0, #1
	movne	r0, #0
	mvneq	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L716:
	ldr	r5, [r3, #-3892]
	cmp	r5, #0
	beq	.L766
	blx	r5
	cmp	r0, #0
	beq	.L720
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L721:
	ldr	r3, .L767+8
	add	r5, r4, #21248
	add	r5, r5, #156
	mov	r6, #0
	ldr	r3, [r3, r7, asl #2]
	add	r3, r3, #606208
	ldr	r8, [r3, #844]
.L731:
	mov	r1, r5
	mov	r0, r7
	bl	FSP_IsFsStatusReady
	cmp	r0, #0
	beq	.L729
	ldr	r3, [r5, #20]
	cmp	r3, r8
	beq	.L730
.L729:
	add	r6, r6, #1
	add	r5, r5, #100
	cmp	r6, #32
	bne	.L731
	b	.L724
.L730:
	cmp	r6, #31
	ble	.L732
	b	.L724
.L765:
	mov	r0, r7
	bl	VCTRL_GetVidStd
	cmp	r0, #0
	beq	.L717
	mov	r0, r7
	bl	VCTRL_GetVidStd
	cmp	r0, #16
	beq	.L717
	mov	r0, r7
	bl	VCTRL_GetVidStd
	cmp	r0, #15
	bne	.L724
	b	.L717
.L722:
	mov	r1, r7
	mov	r0, r4
	bl	FSP_IsRefLeak
	cmp	r0, #0
	beq	.L725
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L761:
	ldr	r3, .L767+12
	movw	r2, #1066
	ldr	r1, .L767+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L740:
	mov	r2, #0
	b	.L737
.L762:
	mov	r0, r4
	ldr	r3, .L767+20
	movw	r2, #1067
	ldr	r1, .L767+16
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L766:
	ldr	r1, .L767+24
	mov	r0, r5
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L768:
	.align	2
.L767:
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	s_pstVfmwChan
	.word	.LC0
	.word	.LC1
	.word	.LC4
	.word	.LC5
	UNWIND(.fnend)
	.size	FSP_IsNewFsAvalible, .-FSP_IsNewFsAvalible
	.align	2
	.global	FSP_GetQueSwitchFlag
	.type	FSP_GetQueSwitchFlag, %function
FSP_GetQueSwitchFlag:
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
	bhi	.L772
	mov	r2, r0, asl #8
	ldr	r3, .L773
	add	r0, r2, r0, lsl #3
	add	r0, r3, r0
	ldrb	r0, [r0, #64]	@ zero_extendqisi2
	ldmfd	sp, {fp, sp, pc}
.L772:
	ldr	r3, .L773+4
	movw	r2, #2223
	ldr	r1, .L773+8
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #255
	ldmfd	sp, {fp, sp, pc}
.L774:
	.align	2
.L773:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	UNWIND(.fnend)
	.size	FSP_GetQueSwitchFlag, .-FSP_GetQueSwitchFlag
	.align	2
	.global	FSP_SetQueSwitchFlag
	.type	FSP_SetQueSwitchFlag, %function
FSP_SetQueSwitchFlag:
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
	bhi	.L777
	mov	r2, r0, asl #8
	ldr	r3, .L778
	add	r0, r2, r0, lsl #3
	mov	r2, #1
	add	r0, r3, r0
	strb	r2, [r0, #64]
	ldmfd	sp, {fp, sp, pc}
.L777:
	ldr	r3, .L778+4
	movw	r2, #2233
	ldr	r1, .L778+8
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L779:
	.align	2
.L778:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	UNWIND(.fnend)
	.size	FSP_SetQueSwitchFlag, .-FSP_SetQueSwitchFlag
	.align	2
	.global	FSP_IsFsInVO
	.type	FSP_IsFsInVO, %function
FSP_IsFsInVO:
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
	ldr	ip, .L794
	ldr	r2, [ip, r0, asl #2]
	cmp	r2, #0
	beq	.L789
	mov	r3, r0, asl #8
	add	r3, r3, r0, lsl #3
	add	r3, ip, r3
	add	r3, r3, #64
	ldrsh	lr, [r3, #2]
	cmn	lr, #1
	beq	.L789
	ldrh	r3, [r3, #4]
	cmp	r3, lr
	beq	.L789
	ldr	r2, [r2, #4]
	cmp	r2, #1
	beq	.L793
.L789:
	mov	r0, #0
	ldmfd	sp, {r4, fp, sp, pc}
.L793:
	and	r3, r3, #63
	cmp	lr, r3
	beq	.L789
	mov	r4, r0, asl #6
	add	r0, r4, r0, lsl #1
	add	r4, r0, r3
	add	r4, ip, r4, lsl #2
	ldr	r4, [r4, #72]
	cmp	r4, r1
	bne	.L783
	b	.L791
.L784:
	ldr	r2, [r2, #72]
	cmp	r2, r1
	beq	.L791
.L783:
	add	r3, r3, #1
	and	r3, r3, #63
	add	r2, r0, r3
	cmp	lr, r3
	add	r2, ip, r2, lsl #2
	bne	.L784
	b	.L789
.L791:
	mov	r0, #1
	ldmfd	sp, {r4, fp, sp, pc}
.L795:
	.align	2
.L794:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	FSP_IsFsInVO, .-FSP_IsFsInVO
	.align	2
	.global	FSP_ProcFspQue
	.type	FSP_ProcFspQue, %function
FSP_ProcFspQue:
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
	cmp	r1, #0
	beq	.L817
	cmp	r2, #1
	bhi	.L818
	beq	.L819
	mov	r3, r0, asl #8
	mov	r10, r0, asl #3
	ldr	r5, .L820
	add	r9, r10, r3
	str	r3, [fp, #-48]
	add	r9, r5, r9
	ldrh	r7, [r9, #68]
	ldrh	r8, [r9, #70]
	and	r3, r7, #63
	cmp	r8, r3
	beq	.L801
	mov	r6, r0, asl #6
	add	r6, r6, r0, lsl #1
.L806:
	add	r2, r6, r3
	ldr	r0, [r1, #28]
	add	r2, r5, r2, lsl #2
	ldr	r2, [r2, #72]
	cmp	r0, r2
	beq	.L807
	cmp	r2, #0
	movne	ip, r3
	bne	.L803
	mov	lr, #2
.L802:
	mov	ip, r7
.L805:
	cmp	r3, #0
	mov	r4, #63
	subne	r4, r3, #1
	add	r2, r6, r3
	add	r0, r6, r4
	cmp	r3, r7
	add	r2, r5, r2, lsl #2
	mov	r3, r4
	add	r0, r5, r0, lsl #2
	ldr	r0, [r0, #72]
	str	r0, [r2, #72]
	bne	.L805
	cmp	lr, #1
	add	r7, r7, #1
	and	r7, r7, #63
	strh	r7, [r9, #68]	@ movhi
	beq	.L801
.L803:
	add	r3, ip, #1
	and	r3, r3, #63
	cmp	r3, r8
	bne	.L806
.L801:
	ldr	r3, [fp, #-48]
	add	r10, r10, r3
	add	r5, r5, r10
	ldrsh	r2, [r5, #66]
	cmp	r7, r2
	moveq	r1, #0
	mvneq	r2, #0
	streqb	r1, [r5, #64]
	streqh	r2, [r5, #66]	@ movhi
.L796:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L807:
	mov	lr, #1
	b	.L802
.L818:
	ldr	r3, .L820+4
	movw	r2, #2294
	ldr	r1, .L820+8
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L819:
	ldr	ip, .L820
	mov	r3, r0, asl #8
	add	r3, r3, r0, lsl #3
	mov	r2, r0, asl #6
	add	r3, ip, r3
	add	r0, r2, r0, lsl #1
	ldr	lr, [r1, #28]
	ldrh	r2, [r3, #70]
	add	r1, r0, r2
	add	r2, r2, #1
	and	r2, r2, #63
	strh	r2, [r3, #70]	@ movhi
	add	r1, ip, r1, lsl #2
	str	lr, [r1, #72]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L817:
	mov	r0, r1
	ldr	r3, .L820+12
	movw	r2, #2293
	ldr	r1, .L820+8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L821:
	.align	2
.L820:
	.word	.LANCHOR0
	.word	.LC57
	.word	.LC1
	.word	.LC56
	UNWIND(.fnend)
	.size	FSP_ProcFspQue, .-FSP_ProcFspQue
	.align	2
	.global	FSP_GetNullLogicFs
	.type	FSP_GetNullLogicFs, %function
FSP_GetNullLogicFs:
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
	ldr	r3, .L830
	ldr	ip, [r3, r0, asl #2]
	add	r3, ip, #20480
	ldrsb	r3, [r3, #161]
	cmp	r3, #0
	beq	.L827
	add	r2, ip, #19968
	mov	r3, #30
	add	r2, r2, #9
	b	.L826
.L824:
	subs	r3, r3, #1
	bcc	.L829
.L826:
	ldrsb	r1, [r2]
	sub	r2, r2, #664
	cmp	r1, #0
	bne	.L824
.L825:
	mov	r2, #664
	mov	r0, r3
	mla	r3, r2, r3, ip
	mov	r2, #0
	str	r2, [r3, #696]
	str	r2, [r3, #700]
	ldmfd	sp, {fp, sp, pc}
.L829:
	mov	r3, #0
	b	.L825
.L827:
	mov	r3, #31
	b	.L825
.L831:
	.align	2
.L830:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	FSP_GetNullLogicFs, .-FSP_GetNullLogicFs
	.align	2
	.global	FSP_SwitchNotify
	.type	FSP_SwitchNotify, %function
FSP_SwitchNotify:
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
	ldmlsfd	sp, {fp, sp, pc}
	ldr	r3, .L834
	movw	r2, #2405
	ldr	r1, .L834+4
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L835:
	.align	2
.L834:
	.word	.LC0
	.word	.LC1
	UNWIND(.fnend)
	.size	FSP_SwitchNotify, .-FSP_SwitchNotify
	.align	2
	.global	FSP_GetFspType
	.type	FSP_GetFspType, %function
FSP_GetFspType:
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
	bhi	.L840
	ldr	r3, .L842
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L841
	ldr	r0, [r0, #4]
	subs	r0, r0, #1
	mvnne	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L840:
	ldr	r3, .L842+4
	movw	r2, #2436
	ldr	r1, .L842+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L841:
	ldr	r3, .L842+12
	movw	r2, #2437
	ldr	r1, .L842+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L843:
	.align	2
.L842:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_GetFspType, .-FSP_GetFspType
	.align	2
	.global	FSP_GetTotalValidFsNum
	.type	FSP_GetTotalValidFsNum, %function
FSP_GetTotalValidFsNum:
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
	bhi	.L848
	ldr	r3, .L850
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L849
	ldrsb	r0, [r0, #9]
	ldmfd	sp, {fp, sp, pc}
.L848:
	ldr	r3, .L850+4
	movw	r2, #2454
	ldr	r1, .L850+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L849:
	ldr	r3, .L850+12
	movw	r2, #2455
	ldr	r1, .L850+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L851:
	.align	2
.L850:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_GetTotalValidFsNum, .-FSP_GetTotalValidFsNum
	.global	s_FspVoQue
	.global	s_pFspInst
	.section	.rodata
	.align	2
.LANCHOR2 = . + 0
	.type	__func__.14482, %object
	.size	__func__.14482, 15
__func__.14482:
	.ascii	"FSP_NewLogicFs\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"InstID out of range\000" )
.LC1:
	ASCII(.ascii	"fsp.c,L%d: %s\012\000" )
	.space	1
.LC2:
	ASCII(.ascii	"fsp instance not in used\000" )
	.space	3
.LC3:
	ASCII(.ascii	"pFspInst = NULL\000" )
.LC4:
	ASCII(.ascii	"fsp is not inited\000" )
	.space	2
.LC5:
	ASCII(.ascii	"%s : AllocFrame func null\012\000" )
	.space	1
.LC6:
	ASCII(.ascii	"%s : RleaseFrame func null\012\000" )
.LC7:
	ASCII(.ascii	"pstFspInstCfg = NULL\000" )
	.space	3
.LC8:
	ASCII(.ascii	"This inst is not active\000" )
.LC9:
	ASCII(.ascii	"eChanCapLevel = %d is not expected!\012\000" )
	.space	3
.LC10:
	ASCII(.ascii	"decode channel CapLevel is %d(w=%d,h=%d), this stre" )
	ASCII(.ascii	"am (w=%d,h=%d) can NOT be decoded!\012\000" )
	.space	1
.LC11:
	ASCII(.ascii	"user set decode channel max resolution is (w=%d,h=%" )
	ASCII(.ascii	"d), this stream (w=%d,h=%d) can NOT be decoded!\012" )
	ASCII(.ascii	"\000" )
.LC12:
	ASCII(.ascii	"DFS, Wrong Partition State. State: 0x%x\012\000" )
	.space	3
.LC13:
	ASCII(.ascii	"DFS, Mem Element Fill Successed! Ready to Decode!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC14:
	ASCII(.ascii	"Fsp arrange failed, return %d\012\000" )
	.space	1
.LC15:
	ASCII(.ascii	"arrange ret %d, min = %d, actual = %d\012\000" )
	.space	1
.LC16:
	ASCII(.ascii	"PhyAddr is NULL\012\000" )
	.space	3
.LC17:
	ASCII(.ascii	"pFsNum is NULL\012\000" )
.LC18:
	ASCII(.ascii	"FSP_IsFsLeak: fsp is not inited \012\000" )
	.space	2
.LC19:
	ASCII(.ascii	"no logic frame store!!!\012\000" )
	.space	3
.LC20:
	ASCII(.ascii	"%s no decode phy fs\012\000" )
	.space	3
.LC21:
	ASCII(.ascii	"no pmv memory\012\000" )
	.space	1
.LC22:
	ASCII(.ascii	"FSP_AllocFrame failed\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"invalid stFrameInfo PhyAddr\012\000" )
	.space	3
.LC24:
	ASCII(.ascii	"LogicFrameID out of range\000" )
	.space	2
.LC25:
	ASCII(.ascii	"fsp.c, line %d: pstLogicFs->PmvIdc out of range(=%d" )
	ASCII(.ascii	")\012\000" )
	.space	2
.LC26:
	ASCII(.ascii	"FSP_GetRef: InstID out of range\000" )
.LC27:
	ASCII(.ascii	"FSP_GetRef: LogicFrameID out of range\000" )
	.space	2
.LC28:
	ASCII(.ascii	"FSP_GetRef: Fsp is not inited\000" )
	.space	2
.LC29:
	ASCII(.ascii	"FSP_SetDisplay: LogicTick=%d, DispTick=%d!\012\000" )
.LC30:
	ASCII(.ascii	"pstVoQue is NULL\000" )
	.space	3
.LC31:
	ASCII(.ascii	"DecFsAddrTab is NULL\012\000" )
	.space	2
.LC32:
	ASCII(.ascii	"DecFsNum is NULL\012\000" )
	.space	2
.LC33:
	ASCII(.ascii	"PmvAddrTab is NULL\012\000" )
.LC34:
	ASCII(.ascii	"PmvNum is NULL\012\000" )
.LC35:
	ASCII(.ascii	"FSP CONTEXT:\012\000" )
	.space	2
.LC36:
	ASCII(.ascii	"FspInstState\000" )
	.space	3
.LC37:
	ASCII(.ascii	"%-35s :%d\012\000" )
	.space	1
.LC38:
	ASCII(.ascii	"DecFsWidth\000" )
	.space	1
.LC39:
	ASCII(.ascii	"DecFsHeight\000" )
.LC40:
	ASCII(.ascii	"DispFsWidth\000" )
.LC41:
	ASCII(.ascii	"DispFsHeight\000" )
	.space	3
.LC42:
	ASCII(.ascii	"ExpectedDecFsNum\000" )
	.space	3
.LC43:
	ASCII(.ascii	"ExpectedDispFsNum\000" )
	.space	2
.LC44:
	ASCII(.ascii	"ExpectedPmvNum\000" )
	.space	1
.LC45:
	ASCII(.ascii	"DecFsNum\000" )
	.space	3
.LC46:
	ASCII(.ascii	"DispFsNum\000" )
	.space	2
.LC47:
	ASCII(.ascii	"PmvNum\000" )
	.space	1
.LC48:
	ASCII(.ascii	"logic fs: %d\012\000" )
	.space	2
.LC49:
	ASCII(.ascii	"\012\000" )
	.space	2
.LC50:
	ASCII(.ascii	"%d%d \000" )
	.space	2
.LC51:
	ASCII(.ascii	"\012dec fs: %d\012\000" )
	.space	3
.LC52:
	ASCII(.ascii	"\012disp fs: %d\012\000" )
	.space	2
.LC53:
	ASCII(.ascii	"\012pmv: %d\012\000" )
	.space	2
.LC54:
	ASCII(.ascii	"\012\012\000" )
	.space	1
.LC55:
	ASCII(.ascii	"%d \000" )
.LC56:
	ASCII(.ascii	"p_image NULL\000" )
	.space	3
.LC57:
	ASCII(.ascii	"Flag Invalid\000" )
	.bss
	.align	2
.LANCHOR0 = . + 0
.LANCHOR1 = . + 8184
	.type	s_pFspInst, %object
	.size	s_pFspInst, 64
s_pFspInst:
	.space	64
	.type	s_FspVoQue, %object
	.size	s_FspVoQue, 4224
s_FspVoQue:
	.space	4224
	.type	g_IsFrameIntfValid, %object
	.size	g_IsFrameIntfValid, 4
g_IsFrameIntfValid:
	.space	4
	.type	g_FspFrameIntf, %object
	.size	g_FspFrameIntf, 16
g_FspFrameIntf:
	.space	16
	.type	MemArrange.14362, %object
	.size	MemArrange.14362, 1272
MemArrange.14362:
	.space	1272
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
