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
	ldr	r4, .L5
	mov	r2, #64
	mov	r1, #0
	ldr	r0, .L5+4
	ldr	r3, [r4, #48]
	blx	r3
	ldr	r3, [r4, #48]
	mov	r2, #4224
	mov	r1, #0
	ldr	r0, .L5+8
	blx	r3
	ldr	r3, .L5+8
	mvn	r1, #0
	add	r2, r3, #4224
.L2:
	strh	r1, [r3, #2]	@ movhi
	add	r3, r3, #264
	cmp	r3, r2
	bne	.L2
	ldr	lr, .L5+12
	mov	r3, #0
	ldr	ip, [r4, #48]
	mov	r1, r3
	mov	r2, #16
	ldr	r0, .L5+16
	str	r3, [lr, #-3896]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	ip
.L6:
	.align	2
.L5:
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
	bhi	.L12
	ldr	r3, .L14
	ldr	r3, [r3, r0, asl #2]
	ldr	r2, [r3, #4]
	cmp	r2, #1
	bne	.L13
	ldrb	r2, [r1]	@ zero_extendqisi2
	mov	r0, #0
	strb	r2, [r3, #1]
	ldmfd	sp, {fp, sp, pc}
.L13:
	ldr	ip, .L14+4
	mov	r2, #244
	ldr	r3, .L14+8
.L11:
	ldr	r1, .L14+12
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L12:
	ldr	ip, .L14+4
	mov	r2, #243
	ldr	r3, .L14+16
	b	.L11
.L15:
	.align	2
.L14:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	@ args = 0, pretend = 0, frame = 560
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #564)
	sub	sp, sp, #564
	subs	r4, r0, #0
	beq	.L33
	ldr	r5, .L35
	mov	r2, #544
	mov	r1, #0
	sub	r0, fp, #588
	ldr	r3, [r5, #48]
	blx	r3
	ldrsb	r0, [r4]
	bl	VCTRL_GetChanVoQue
	cmp	r0, #0
	beq	.L32
	ldrh	r3, [r0, #4]
	ldrh	r2, [r0, #6]
	cmp	r2, r3
	beq	.L32
	cmp	r3, #63
	bls	.L34
.L32:
	ldrb	r7, [r4]	@ zero_extendqisi2
.L20:
	add	r6, r4, #20
	ldrb	r9, [r4, #10]	@ zero_extendqisi2
	ldrb	r8, [r4, #1]	@ zero_extendqisi2
	mov	r2, #16
	ldr	r10, [r4, #4]
	mov	r1, r6
	ldr	ip, [r5, #52]
	sub	r0, fp, #604
	blx	ip
	ldr	ip, [r5, #48]
	movw	r2, #22296
	mov	r1, #0
	mov	r0, r4
	blx	ip
	strb	r8, [r4, #1]
	str	r10, [r4, #4]
	sub	r1, fp, #604
	strb	r9, [r4, #10]
	mov	r0, r6
	strb	r7, [r4]
	mov	r2, #16
	ldr	r3, [r5, #52]
	blx	r3
	ldr	r0, [fp, #-416]
	cmp	r0, #0
	beq	.L18
	ldr	r2, .L35
	mvn	r3, #540
	sub	r1, fp, #588
	ldr	ip, [r2, #52]
	sub	r2, fp, #44
	ldrsb	r0, [r2, r3]
	mov	r2, #544
	mov	r3, r0, asl #9
	add	r0, r3, r0, lsl #5
	add	r0, r0, #72
	add	r0, r4, r0
	blx	ip
	mov	r0, #0
.L18:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L34:
	add	r0, r0, r3, lsl #2
	ldrb	r7, [r4]	@ zero_extendqisi2
	ldr	r2, [r0, #12]
	sxtb	r3, r7
	ldr	r1, [r2, #196]
	cmp	r3, #15
	cmpls	r1, #31
	bhi	.L20
	ldr	r2, .L35+4
	ldr	r3, [r2, r3, asl #2]
	cmp	r3, #0
	beq	.L20
	mov	r2, r1, asl #9
	add	r1, r2, r1, lsl #5
	add	r1, r1, #72
	adds	r1, r3, r1
	beq	.L20
	ldr	r3, [r5, #52]
	mov	r2, #544
	sub	r0, fp, #588
	blx	r3
	b	.L32
.L33:
	ldr	ip, .L35
	movw	r2, #270
	ldr	r3, .L35+8
	ldr	r1, .L35+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L18
.L36:
	.align	2
.L35:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LC3
	.word	.LC1
	UNWIND(.fnend)
	.size	FSP_ClearContext, .-FSP_ClearContext
	.align	2
	.global	FSP_EnableInstance
	.type	FSP_EnableInstance, %function
FSP_EnableInstance:
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
	bhi	.L42
	ldr	r3, .L44
	ldr	r4, [r3, r0, asl #2]
	cmp	r4, #0
	beq	.L43
	ldr	r5, [r4, #4]
	cmp	r5, #0
	bne	.L41
	ldr	r3, .L44+4
	movw	r2, #22296
	mov	r1, r5
	mov	r0, r4
	ldr	r3, [r3, #48]
	blx	r3
	add	r3, r4, #20480
	mov	r2, #1
	mov	r0, r5
	str	r2, [r4, #4]
	str	r5, [r3, #1800]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L41:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L42:
	ldr	ip, .L44+4
	movw	r2, #330
	ldr	r3, .L44+8
	mov	r0, #0
	ldr	r1, .L44+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L43:
	ldr	ip, .L44+4
	mov	r0, r4
	ldr	r3, .L44+16
	movw	r2, #331
	ldr	r1, .L44+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L45:
	.align	2
.L44:
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
	bhi	.L51
	ldr	r3, .L53
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L52
	ldr	r3, [r0, #4]
	cmp	r3, #1
	moveq	r3, #0
	streq	r3, [r0, #4]
	ldmfd	sp, {fp, sp, pc}
.L51:
	ldr	ip, .L53+4
	movw	r2, #354
	ldr	r3, .L53+8
	mov	r0, #0
.L50:
	ldr	r1, .L53+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L52:
	ldr	ip, .L53+4
	movw	r2, #355
	ldr	r3, .L53+16
	b	.L50
.L54:
	.align	2
.L53:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r5, .L66
	mov	r4, #0
	b	.L57
.L56:
	add	r4, r4, #1
	cmp	r4, #16
	beq	.L65
.L57:
	ldr	r3, [r5, #4]!
	cmp	r3, #0
	beq	.L56
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L56
	mov	r0, r4
	add	r4, r4, #1
	bl	FSP_DisableInstance
	cmp	r4, #16
	bne	.L57
.L65:
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L67:
	.align	2
.L66:
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
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #15
	mov	r6, r0
	bhi	.L89
	ldr	r3, .L94
	ldr	r7, [r3, r0, asl #2]
	cmp	r7, #0
	beq	.L90
	ldr	r10, .L94+4
	add	r4, r7, #17408
	add	r5, r7, #19712
	ldr	r9, .L94+8
	add	r4, r4, #142
	add	r5, r5, #14
	mov	r8, r10
	b	.L78
.L75:
	ldrsb	r3, [r4]
	cmp	r3, #0
	bne	.L91
.L73:
	add	r4, r4, #68
	cmp	r4, r5
	beq	.L92
.L78:
	ldr	r1, [r4, #10]
	cmp	r1, #0
	beq	.L73
	ldr	r3, [r10, #-3896]
	cmp	r3, #0
	beq	.L73
	ldrsb	r3, [r4, #-2]
	cmp	r3, #0
	beq	.L75
	ldr	r3, [r8, #-3880]
	cmp	r3, #0
	beq	.L93
	mov	r2, #0
	mov	r0, r6
	blx	r3
	ldrsb	r3, [r4]
	cmp	r3, #0
	beq	.L73
.L91:
	ldr	r3, [r10, #-3884]
	add	r4, r4, #68
	ldr	r1, [r4, #-58]
	cmp	r3, #0
	moveq	r0, r3
	ldreq	r1, .L94+12
	ldreq	r3, [r9, #68]
	movne	r0, r6
	blx	r3
	cmp	r4, r5
	bne	.L78
.L92:
	mov	r0, r7
	bl	FSP_ClearContext
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L89:
	ldr	ip, .L94+8
	movw	r2, #375
	ldr	r3, .L94+16
	mov	r0, #0
	ldr	r1, .L94+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L93:
	mov	r0, r3
	ldr	r1, .L94+24
	ldr	r3, [r9, #68]
	blx	r3
	b	.L75
.L90:
	ldr	ip, .L94+8
	mov	r0, r7
	ldr	r3, .L94+28
	mov	r2, #376
	ldr	r1, .L94+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L95:
	.align	2
.L94:
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
	.word	.LC0
	.word	.LC1
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
	bhi	.L127
	ldr	r3, .L132
	ldr	r7, [r3, r0, asl #2]
	cmp	r7, #0
	beq	.L128
	ldr	r9, .L132+4
	mov	r2, #17408
	mov	r1, #0
	add	r0, r7, #72
	ldr	r3, [r9, #48]
	blx	r3
	ldrsb	r3, [r7, #10]
	cmp	r3, #0
	ble	.L110
	add	r4, r7, #17408
	ldr	r10, .L132+8
	mov	r6, #0
	add	r4, r4, #200
	mov	r5, r6
.L109:
	ldr	r1, [r4, #-48]
	cmp	r1, #0
	beq	.L104
	ldr	r2, [r10, #-3896]
	cmp	r2, #0
	beq	.L104
	ldrsb	r2, [r4, #-60]
	cmp	r2, #0
	bne	.L129
.L106:
	ldrsb	r2, [r4, #-58]
	cmp	r2, #0
	bne	.L130
.L104:
	strb	r5, [r4, #-60]
	add	r6, r6, #1
	strb	r5, [r4]
	add	r4, r4, #68
	strb	r5, [r4, #-126]
	str	r5, [r4, #-72]
	ldrsb	r2, [r7, #10]
	cmp	r2, r6
	bgt	.L109
.L110:
	ldrsb	r0, [r7, #11]
	cmp	r0, #0
	addgt	r3, r7, #19712
	movgt	r1, #0
	addgt	r3, r3, #12
	movgt	r2, r1
	ble	.L102
.L113:
	add	r1, r1, #1
	strb	r2, [r3]
	cmp	r0, r1
	strb	r2, [r3, #60]
	strb	r2, [r3, #2]
	add	r3, r3, #68
	str	r2, [r3, #-12]
	bne	.L113
.L102:
	ldrsb	r1, [r7, #13]
	cmp	r1, #0
	addgt	r2, r7, #22016
	movgt	r3, #0
	addgt	r2, r2, #24
	movgt	r0, r3
	ble	.L112
.L114:
	add	r3, r3, #1
	str	r0, [r2], #12
	cmp	r3, r1
	bne	.L114
.L112:
	mov	r0, #0
	str	r0, [r7, #16]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L130:
	ldr	r2, [r10, #-3884]
	ldr	r1, [r4, #-48]
	cmp	r2, #0
	moveq	r0, r2
	ldreq	r1, .L132+12
	ldreq	r2, [r9, #68]
	movne	r0, r8
	blx	r2
	b	.L104
.L129:
	ldr	r3, .L132+8
	ldr	ip, [r3, #-3880]
	cmp	ip, #0
	beq	.L131
	mov	r2, #0
	mov	r0, r8
	blx	ip
	b	.L106
.L127:
	ldr	ip, .L132+4
	movw	r2, #450
	ldr	r3, .L132+16
	mov	r0, #0
	ldr	r1, .L132+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L128:
	ldr	ip, .L132+4
	mov	r0, r7
	ldr	r3, .L132+24
	movw	r2, #451
	ldr	r1, .L132+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L131:
	mov	r0, ip
	ldr	r2, [r9, #68]
	ldr	r1, .L132+28
	blx	r2
	b	.L106
.L133:
	.align	2
.L132:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1
	.word	.LC6
	.word	.LC0
	.word	.LC1
	.word	.LC4
	.word	.LC5
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
	ldr	r2, .L140
	movw	r3, #1466
	ldr	r2, [r2, r0, asl #2]
	add	r1, r2, #204800
	ldrsb	ip, [r1, r3]
	cmp	ip, #0
	ble	.L138
	movw	r3, #27198
	mov	r0, #0
	movt	r3, 3
	add	r3, r2, r3
	mov	r2, r0
.L137:
	ldrsb	r1, [r3]
	add	r2, r2, #1
	add	r3, r3, #68
	cmp	r1, #3
	addeq	r0, r0, #1
	cmp	r2, ip
	bne	.L137
	ldmfd	sp, {fp, sp, pc}
.L138:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L141:
	.align	2
.L140:
	.word	s_pstVfmwChan
	UNWIND(.fnend)
	.size	FSP_GetUsedBufferNum, .-FSP_GetUsedBufferNum
	.align	2
	.global	FSP_ConfigInstance
	.type	FSP_ConfigInstance, %function
FSP_ConfigInstance:
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
	mov	r5, r1
	bhi	.L152
	cmp	r1, #0
	beq	.L153
	ldr	r3, .L154
	ldr	r4, [r3, r0, asl #2]
	cmp	r4, #0
	beq	.L146
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne	.L146
	mov	r0, r4
	bl	FSP_ClearContext
	ldr	r3, .L154+4
	mov	r1, r5
	add	r0, r4, #36
	mov	r2, #32
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L146:
	ldr	ip, .L154+4
	movw	r2, #609
	ldr	r3, .L154+8
.L151:
	ldr	r1, .L154+12
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L152:
	ldr	ip, .L154+4
	movw	r2, #606
	ldr	r3, .L154+16
	b	.L151
.L153:
	ldr	ip, .L154+4
	mov	r0, r1
	ldr	r3, .L154+20
	movw	r2, #607
	ldr	r1, .L154+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L155:
	.align	2
.L154:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC8
	.word	.LC1
	.word	.LC0
	.word	.LC7
	UNWIND(.fnend)
	.size	FSP_ConfigInstance, .-FSP_ConfigInstance
	.align	2
	.global	FSP_ConfigFrameBuf
	.type	FSP_ConfigFrameBuf, %function
FSP_ConfigFrameBuf:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 872
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #900)
	sub	sp, sp, #900
	mov	r8, r2
	ldr	r2, .L229
	mov	r5, r0
	mov	r6, r1
	sub	r0, fp, #856
	mov	r1, #0
	str	r3, [fp, #-908]
	ldr	ip, [r2, #48]
	mov	r2, #812
	blx	ip
	cmp	r5, #15
	bhi	.L217
	ldr	r3, .L229+4
	ldr	r4, [r3, r5, asl #2]
	cmp	r4, #0
	beq	.L218
	ldr	r7, .L229+8
	ldr	r0, [r7, r5, asl #2]
	add	r3, r0, #229376
	ldr	r9, [r3, #720]
	cmp	r9, #0
	beq	.L219
	ldr	ip, [r4, #44]
	ldr	lr, [r4, #48]
	mul	r1, lr, ip
.L163:
	ldr	r2, [r0, #144]
	ldr	r3, [r0, #148]
	mul	r9, r3, r2
	cmp	r9, r1
	blt	.L220
	ldr	r3, [r0, #1388]
	ldrsb	r9, [r4, #40]
	cmp	r3, #1
	add	r3, r0, #225280
	beq	.L221
.L166:
	ldr	r2, [r3, #3276]
	ldr	r1, .L229+12
	cmp	r9, r2
	ldr	ip, [r1, #48]
	movne	r2, #1
	moveq	r2, #0
	str	r2, [r3, #3296]
	ldrsb	r2, [r4, #40]
	cmp	ip, #0
	str	r2, [r3, #3276]
	beq	.L170
	ldr	r3, [r4, #48]
	sub	lr, fp, #856
	ldr	r2, [r4, #44]
	mov	r0, r6
	str	lr, [sp, #16]
	mov	r6, #40
	str	r5, [sp, #12]
	mov	r1, r8
	ldrsb	lr, [r4, #39]
	str	r6, [sp, #4]
	ldr	r8, .L229+8
	str	lr, [sp, #8]
	ldrsb	lr, [r4, #43]
	str	lr, [sp]
	blx	ip
	ldr	r3, [r7, r5, asl #2]
	add	r3, r3, #229376
	ldr	r3, [r3, #648]
	cmp	r3, #20
	movge	r3, #20
	strb	r3, [r4, #13]
	cmp	r0, #2
	mov	r6, r0
	beq	.L222
	cmp	r0, #0
	bne	.L173
	ldr	r3, [fp, #-436]
	mov	lr, #1
	ldr	r10, .L229
	mov	r0, #31
	ldr	r1, .L229+16
	strb	r3, [r4, #9]
	ldr	r3, [r8, r5, asl #2]
	ldr	r2, [r10, #68]
	add	ip, r3, #229376
	str	r6, [r3, #1428]
	str	lr, [ip, #696]
	blx	r2
	ldr	r3, [fp, #-436]
	cmp	r9, r3
	bgt	.L223
	ldrsb	r3, [r4, #1]
	strb	r6, [r4, #11]
	cmp	r3, #0
	strb	r6, [r4, #12]
	bne	.L224
.L176:
	ldr	r3, [fp, #-436]
	cmp	r3, #40
	movge	r3, #40
	strb	r3, [r4, #10]
.L177:
	ldr	r3, .L229
	mov	r0, #8
	ldr	r3, [r3, #12]
	blx	r3
	ldr	r2, [fp, #-52]
	cmp	r2, #0
	beq	.L225
	ldr	r1, [fp, #-856]
	add	r3, r4, #20480
	str	r3, [fp, #-880]
	sub	r8, fp, #596
	ldr	r10, [r3, #1804]
	str	r1, [fp, #-864]
	ldr	r1, [fp, #-452]
	add	r2, r2, r10
	ldr	r3, [fp, #-464]
	add	r2, r2, #1
	str	r2, [fp, #-904]
	str	r1, [fp, #-888]
	ldr	r1, [r4, #44]
	ldr	r2, [fp, #-448]
	str	r3, [fp, #-876]
	str	r1, [fp, #-868]
	ldr	r1, [r4, #48]
	ldr	r3, [fp, #-456]
	str	r2, [fp, #-892]
	str	r1, [fp, #-872]
	ldr	r1, [fp, #-440]
	ldr	r2, [fp, #-444]
	ldr	r9, [r7, r5, asl #2]
	mov	r7, #0
	str	r3, [fp, #-884]
	mov	r3, r10, asl #6
	add	r3, r3, r10, lsl #2
	str	r1, [fp, #-900]
	sub	r1, fp, #856
	str	r2, [fp, #-896]
	add	r9, r9, #229376
	add	r2, r10, #1
	add	r3, r4, r3
	sub	r10, r1, r10, asl #3
	str	r4, [fp, #-912]
	b	.L181
.L180:
	mov	r1, #1
	str	r1, [r0]
	ldr	r1, [fp, #-880]
	str	r2, [r1, #1804]
	add	r2, r2, #1
	ldr	r1, [fp, #-904]
	cmp	r2, r1
	beq	.L226
.L181:
	add	ip, r3, #17408
	sub	lr, r2, #1
	add	r3, r3, #68
	str	lr, [ip, #144]
	mov	r5, ip
	mov	r1, ip
	ldr	ip, [r10, r2, asl #3]
	mov	r4, r1
	mov	lr, r5
	add	r0, r1, #204
	add	r1, r1, #152
	str	ip, [r1]
	mov	ip, r5
	ldr	r6, [r8, #4]!
	str	r6, [r5, #156]
	ldr	r6, [fp, #-864]
	str	r6, [r4, #160]
	ldr	r4, [fp, #-876]
	str	r4, [lr, #164]
	mov	lr, r5
	ldr	r4, [fp, #-884]
	str	r4, [ip, #168]
	mov	ip, r5
	ldr	r4, [fp, #-888]
	str	r4, [r5, #172]
	mov	r4, r5
	ldr	r5, [fp, #-868]
	str	r5, [lr, #176]
	ldr	r5, [fp, #-872]
	str	r5, [ip, #180]
	mov	ip, lr
	ldr	r5, [fp, #-892]
	str	r5, [r4, #184]
	mov	r4, lr
	ldr	r5, [fp, #-896]
	str	r5, [lr, #188]
	ldr	r5, [fp, #-900]
	str	r5, [ip, #192]
	add	ip, lr, #142
	strb	r7, [r4, #140]
	strb	r7, [lr, #141]
	strb	r7, [lr, #142]
	ldr	lr, [r9, #712]
	cmp	lr, #1
	bne	.L180
	ldr	lr, [r1]
	ldr	r1, [r9, #700]
	cmp	lr, r1
	bcc	.L180
	ldr	r4, [r9, #708]
	add	r1, r1, r4
	cmp	lr, r1
	movcc	r1, #4
	strccb	r1, [ip]
	b	.L180
.L219:
	mov	r0, r5
	bl	VCTRL_GetChanCapLevel
	cmp	r0, #32
	bhi	.L161
	ldr	r1, .L229+20
	mov	r3, r0, asl #5
	sub	r3, r3, r0, asl #2
	ldr	ip, [r4, #44]
	add	r2, r1, r3
	ldr	lr, [r4, #48]
	ldr	r3, [r1, r3]
	ldr	r10, [r2, #4]
	mul	r1, lr, ip
	mul	r2, r10, r3
	cmp	r0, #25
	cmpne	r1, r2
	ble	.L212
	ldrsb	r2, [r4, #39]
	cmp	r2, #1
	bne	.L193
.L212:
	ldr	r0, [r7, r5, asl #2]
	b	.L163
.L220:
	ldr	r1, .L229
	mov	r0, #0
	stmia	sp, {ip, lr}
	ldr	ip, [r1, #68]
	ldr	r1, .L229+24
	blx	ip
	ldr	r3, .L229+28
	ldr	ip, [r3]
	cmp	ip, #0
	mvneq	r0, #0
	beq	.L206
	mov	r3, #0
	mov	r0, r5
	mov	r2, r3
	mov	r1, #102
	blx	ip
	mvn	r0, #0
.L206:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L222:
	ldr	r3, [fp, #-436]
	mov	r2, #1
	strb	r3, [r4, #9]
	ldr	r3, [r8, r5, asl #2]
	str	r2, [r3, #1428]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L221:
	ldr	r2, [r3, #3256]
	bics	r1, r2, #2
	beq	.L166
	cmp	r2, #1
	moveq	r0, #0
	beq	.L206
	mov	r3, #0
	strb	r3, [r4, #9]
	ldr	r3, [r7, r5, asl #2]
	mov	r0, #31
	ldr	r2, .L229
	add	r3, r3, #225280
	ldr	r1, .L229+32
	ldr	ip, [r2, #68]
	ldr	r2, [r3, #3256]
	blx	ip
	mvn	r0, #0
	b	.L206
.L170:
	add	r0, r0, #229376
	mvn	r6, #0
	ldr	r3, [r0, #648]
	cmp	r3, #20
	movge	r3, #20
	strb	r3, [r4, #13]
.L173:
	mov	r3, #0
	ldr	r1, .L229
	strb	r3, [r4, #9]
	mov	r2, r6
	ldr	lr, [r7, r5, asl #2]
	mov	r0, r3
	ldr	ip, [r1, #68]
	ldr	r1, .L229+36
	str	r3, [lr, #1428]
	blx	ip
	mvn	r0, #0
	b	.L206
.L224:
	ldr	r3, [r8, r5, asl #2]
	ldr	r3, [r3, #1388]
	cmp	r3, #1
	beq	.L176
	ldrsb	r3, [r4, #40]
	ldr	r2, [fp, #-436]
	add	r3, r3, #4
	cmp	r3, r2
	movge	r3, r2
	strb	r3, [r4, #10]
	b	.L177
.L161:
	ldrsb	r3, [r4, #39]
	cmp	r3, #1
	bne	.L192
	ldr	ip, [r4, #44]
	ldr	lr, [r4, #48]
	ldr	r0, [r7, r5, asl #2]
	mul	r1, lr, ip
	b	.L163
.L226:
	ldr	r4, [fp, #-912]
.L179:
	ldr	r10, [fp, #-880]
	mov	r5, #1
	mov	lr, #0
	strb	r5, [r4, #12]
	mov	ip, r5
	sub	r6, fp, #44
	str	lr, [r10, #1424]
	mov	r0, lr
	ldrsb	r7, [r4, #10]
	mov	r2, lr
	str	r5, [r10, #1484]
	mov	r1, lr
	ldr	r5, [fp, #-864]
	mov	r3, r4
	add	r6, r6, r7, lsl #3
	ldr	r9, [fp, #-452]
	ldr	r8, [fp, #-456]
	str	r5, [r10, #1440]
	ldr	r5, [fp, #-868]
	ldr	r6, [r6, #-804]
	strb	lr, [r10, #1420]
	str	r5, [r10, #1456]
	ldr	r5, [fp, #-872]
	strb	lr, [r10, #1421]
	strb	lr, [r10, #1422]
	str	r5, [r10, #1460]
	ldr	r5, [fp, #-464]
	str	r6, [r10, #1432]
	str	r9, [r10, #1452]
	str	r5, [r10, #1444]
	ldr	r5, [fp, #-448]
	str	r8, [r10, #1448]
	str	r5, [r10, #1464]
	ldr	r5, [fp, #-444]
	str	r5, [r10, #1468]
	ldr	r5, [fp, #-440]
	str	r5, [r10, #1472]
	strb	lr, [r4, #11]
	ldr	lr, [fp, #-452]
	ldr	r10, [fp, #-464]
	ldr	r9, [fp, #-456]
	str	lr, [fp, #-868]
	ldr	r8, [fp, #-448]
	ldr	lr, [fp, #-444]
	ldr	r6, [fp, #-440]
	str	lr, [fp, #-872]
	str	r6, [fp, #-876]
	b	.L185
.L227:
	add	r0, r7, r2
	add	lr, r3, #19712
	sxtab	r0, r0, ip
	sub	r5, fp, #44
	mov	ip, lr
	str	r2, [lr, #16]
	add	r0, r5, r0, lsl #3
.L213:
	ldr	r0, [r0, #-804]
	mov	r5, ip
	add	r2, r2, #1
	add	r3, r3, #68
	cmp	r2, #32
	str	r0, [ip, #24]
	mov	r0, ip
	ldr	r6, [fp, #-864]
	str	r10, [r5, #36]
	str	r9, [r5, #40]
	str	r6, [r5, #32]
	ldr	lr, [fp, #-868]
	str	lr, [ip, #44]
	mov	lr, ip
	ldr	r5, [r4, #60]
	str	r5, [r0, #48]
	mov	r0, ip
	ldr	r6, [r4, #64]
	mov	r5, ip
	str	r6, [lr, #52]
	str	r8, [ip, #56]
	ldr	r6, [fp, #-872]
	str	r6, [r0, #60]
	ldr	r6, [fp, #-876]
	strb	r1, [r5, #12]
	str	r6, [r5, #64]
	strb	r1, [ip, #13]
	strb	r1, [ip, #14]
	beq	.L184
	ldrb	r0, [r4, #11]	@ zero_extendqisi2
	ldrb	ip, [r4, #12]	@ zero_extendqisi2
.L185:
	sxtb	r0, r0
	cmp	r0, r2
	bhi	.L227
	sxtab	r0, r7, ip
	add	lr, r3, #19712
	sub	r5, fp, #44
	mov	ip, lr
	add	r0, r5, r0, lsl #3
	str	r1, [lr, #16]
	b	.L213
.L184:
	ldr	r3, [fp, #-56]
	cmp	r3, #20
	movge	r3, #20
	strb	r3, [r4, #13]
	ldr	lr, [fp, #-48]
	cmp	lr, #0
	beq	.L190
	ldr	r3, [fp, #-880]
	sub	r0, fp, #856
	mov	r6, #0
	mov	r5, #1
	ldr	r2, [r3, #1808]
	add	lr, r2, lr
	mov	ip, r2, asl #2
	mov	r3, r2, asl #4
	rsb	r1, ip, r3
	add	r1, r1, #22016
	add	r1, r1, #24
	add	r1, r4, r1
.L189:
	rsb	r3, ip, r3
	add	r2, r2, #1
	ldr	ip, [r0, #556]
	add	r3, r4, r3
	add	r3, r3, #22016
	cmp	r2, lr
	add	r0, r0, #12
	add	r1, r1, #12
	str	ip, [r1, #-12]
	mov	ip, r2, asl #2
	str	r6, [r3, #32]
	mov	r3, r2, asl #4
	str	r5, [r1, #-8]
	bne	.L189
	ldr	r3, [fp, #-880]
	str	r2, [r3, #1808]
.L190:
	ldr	r1, [fp, #-60]
	add	r3, r4, #16384
	ldr	ip, [fp, #-880]
	mov	r2, #0
	ldr	r5, [fp, #-464]
	mov	lr, #1
	ldr	r0, [fp, #-456]
	str	r1, [ip, #1556]
	ldr	r6, [fp, #-864]
	ldr	r1, [fp, #-452]
	ldr	ip, [r3, #1176]
	str	r6, [r3, #1116]
	str	r5, [r3, #1120]
	str	ip, [r3, #1108]
	str	r0, [r3, #1124]
	str	r1, [r3, #1128]
	str	r2, [r3, #1100]
	ldr	r1, [r4, #44]
	ldr	r8, [fp, #-448]
	ldr	r6, [fp, #-444]
	str	r1, [r3, #1132]
	ldr	r5, [fp, #-440]
	ldr	r0, [r4, #48]
	strb	r2, [r3, #1096]
	strb	r2, [r3, #1097]
	str	r0, [r3, #1136]
	strb	r2, [r3, #1098]
	str	r8, [r3, #1140]
	str	r6, [r3, #1144]
	str	r5, [r3, #1148]
	str	lr, [r3, #1160]
	ldr	r2, [r4, #20]
	cmp	r1, r2
	beq	.L228
.L187:
	str	r1, [r4, #20]
	str	r0, [r4, #24]
	str	r7, [r4, #28]
	ldr	r3, [r3, #1244]
	rsb	r3, ip, r3
	str	r3, [r4, #32]
.L191:
	ldr	r3, [fp, #-852]
	mov	r0, #8
	ldr	r2, [fp, #-908]
	str	r3, [r2]
	ldr	r3, .L229
	ldr	r3, [r3, #16]
	blx	r3
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L228:
	ldr	r2, [r4, #24]
	cmp	r0, r2
	bne	.L187
	ldr	r2, [r4, #28]
	cmp	r7, r2
	bne	.L187
	b	.L191
.L225:
	ldr	r3, [fp, #-856]
	str	r3, [fp, #-864]
	add	r3, r4, #20480
	str	r3, [fp, #-880]
	ldr	r3, [r4, #44]
	str	r3, [fp, #-868]
	ldr	r3, [r4, #48]
	str	r3, [fp, #-872]
	b	.L179
.L217:
	ldr	r3, .L229
	mov	r2, #968
	mov	r0, #0
	ldr	r1, .L229+40
	ldr	ip, [r3, #68]
	ldr	r3, .L229+44
	blx	ip
	mvn	r0, #0
	b	.L206
.L193:
	ldr	r1, .L229
	mov	r2, r0
	stmib	sp, {ip, lr}
	mov	r0, r9
	str	r10, [sp]
	ldr	ip, [r1, #68]
	ldr	r1, .L229+48
	blx	ip
.L164:
	mov	r3, #0
	mvn	r0, #0
	strb	r3, [r4, #9]
	b	.L206
.L192:
	ldr	r3, .L229
	mov	r2, r0
	ldr	r1, .L229+52
	mov	r0, r9
	ldr	r3, [r3, #68]
	blx	r3
	b	.L164
.L223:
	str	r3, [sp]
	mov	r2, r6
	mov	r3, r9
	ldr	ip, [r10, #68]
	mov	r0, r6
	ldr	r1, .L229+56
	blx	ip
	mvn	r0, #0
	b	.L206
.L218:
	ldr	r3, .L229
	mov	r0, r4
	movw	r2, #969
	ldr	r1, .L229+40
	ldr	ip, [r3, #68]
	ldr	r3, .L229+60
	blx	ip
	mvn	r0, #0
	b	.L206
.L230:
	.align	2
.L229:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	s_pstVfmwChan
	.word	g_vdm_hal_fun_ptr
	.word	.LC13
	.word	CapItem
	.word	.LC11
	.word	g_event_report
	.word	.LC12
	.word	.LC14
	.word	.LC1
	.word	.LC0
	.word	.LC10
	.word	.LC9
	.word	.LC15
	.word	.LC4
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
	bhi	.L242
	cmp	r1, #0
	beq	.L243
	cmp	r2, #0
	beq	.L244
	ldr	r3, .L246
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L245
	ldrsb	r3, [r0, #10]
	str	r3, [r2]
	ldrsb	r3, [r0, #10]
	cmp	r3, #0
	ldmlefd	sp, {fp, sp, pc}
	add	r2, r0, #17408
	sub	r1, r1, #4
	add	r2, r2, #152
	mov	r3, #0
.L238:
	ldr	ip, [r2], #68
	add	r3, r3, #1
	str	ip, [r1, #4]!
	ldrsb	ip, [r0, #10]
	cmp	ip, r3
	bgt	.L238
	ldmfd	sp, {fp, sp, pc}
.L242:
	ldr	ip, .L246+4
	movw	r2, #1276
	ldr	r3, .L246+8
	mov	r0, #0
.L241:
	ldr	r1, .L246+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L243:
	ldr	ip, .L246+4
	mov	r0, r1
	ldr	r3, .L246+16
	movw	r2, #1277
	b	.L241
.L244:
	mov	r0, r2
	ldr	ip, .L246+4
	ldr	r3, .L246+20
	movw	r2, #1278
	b	.L241
.L245:
	ldr	ip, .L246+4
	movw	r2, #1279
	ldr	r3, .L246+24
	b	.L241
.L247:
	.align	2
.L246:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	.LC1
	.word	.LC16
	.word	.LC17
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
	bhi	.L259
	cmp	r1, #0
	beq	.L260
	cmp	r2, #0
	beq	.L261
	ldr	r3, .L263
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L262
	ldrsb	r3, [r0, #13]
	str	r3, [r2]
	ldrsb	r3, [r0, #13]
	cmp	r3, #0
	ldmlefd	sp, {fp, sp, pc}
	add	r2, r0, #22016
	sub	r1, r1, #4
	add	r2, r2, #24
	mov	r3, #0
.L255:
	ldr	ip, [r2], #12
	add	r3, r3, #1
	str	ip, [r1, #4]!
	ldrsb	ip, [r0, #13]
	cmp	ip, r3
	bgt	.L255
	ldmfd	sp, {fp, sp, pc}
.L259:
	ldr	ip, .L263+4
	movw	r2, #1306
	ldr	r3, .L263+8
	mov	r0, #0
.L258:
	ldr	r1, .L263+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L260:
	ldr	ip, .L263+4
	mov	r0, r1
	ldr	r3, .L263+16
	movw	r2, #1307
	b	.L258
.L261:
	mov	r0, r2
	ldr	ip, .L263+4
	ldr	r3, .L263+20
	movw	r2, #1308
	b	.L258
.L262:
	ldr	ip, .L263+4
	movw	r2, #1309
	ldr	r3, .L263+24
	b	.L258
.L264:
	.align	2
.L263:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	.LC1
	.word	.LC16
	.word	.LC17
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_GetDecodePmvTable, .-FSP_GetDecodePmvTable
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
	ble	.L269
	mvn	lr, #0
	mov	r3, r0
.L268:
	ldr	ip, [r2, #56]
	add	r2, r2, #64
	cmp	ip, lr
	movcc	r0, r3
	add	r3, r3, #1
	movcc	lr, ip
	cmp	r3, r1
	bne	.L268
	ldmfd	sp, {fp, sp, pc}
.L269:
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	FindLeastTickPhyFs, .-FindLeastTickPhyFs
	.align	2
	.global	FSP_FirstFsCheck
	.type	FSP_FirstFsCheck, %function
FSP_FirstFsCheck:
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
	ldr	lr, .L284
	ldr	r2, [lr, r0, asl #2]
	cmp	r2, #0
	beq	.L282
	mov	r3, r0, asl #8
	add	r3, r3, r0, lsl #3
	add	r3, lr, r3
	add	r3, r3, #64
	ldrsh	r4, [r3, #2]
	cmn	r4, #1
	beq	.L279
	ldrh	r3, [r3, #4]
	cmp	r3, r4
	beq	.L279
	ldr	r6, [r2, #4]
	add	ip, r2, #16384
	ldr	r5, [r2, #32]
	cmp	r6, #1
	ldr	r2, [ip, #1244]
	ldr	r6, [ip, #1176]
	beq	.L283
.L279:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L283:
	and	r3, r3, #63
	cmp	r4, r3
	beq	.L279
	rsb	r6, r6, r2
	mov	r2, r0, asl #6
	add	r6, r6, r1
	add	r0, r2, r0, lsl #1
.L275:
	add	r2, r0, r3
	add	r3, r3, #1
	and	r3, r3, #63
	add	r2, lr, r2, lsl #2
	ldr	r2, [r2, #72]
	add	ip, r2, r5
	cmp	ip, r1
	bcc	.L274
	cmp	r2, r6
	bls	.L280
.L274:
	cmp	r4, r3
	bne	.L275
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L280:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L282:
	ldr	ip, .L284+4
	mov	r0, r2
	ldr	r3, .L284+8
	movw	r2, #1574
	ldr	r1, .L284+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L285:
	.align	2
.L284:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC4
	.word	.LC1
	UNWIND(.fnend)
	.size	FSP_FirstFsCheck, .-FSP_FirstFsCheck
	.align	2
	.global	FSP_NewLogicFs
	.type	FSP_NewLogicFs, %function
FSP_NewLogicFs:
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
	cmp	r0, #15
	mov	r8, r0
	mov	r10, r1
	bhi	.L339
	ldr	r4, .L348
	ldr	r0, [r4, r0, asl #2]
	cmp	r0, #0
	beq	.L340
	ldr	r7, .L348+4
	mov	r0, #3
	ldr	r3, [r7, #12]
	blx	r3
	ldr	r5, [r4, r8, asl #2]
	mov	r2, #0
	mov	r3, r5
.L292:
	ldrsb	r1, [r3, #72]
	cmp	r1, #0
	bne	.L290
	ldrsb	r0, [r3, #73]
	cmp	r0, #0
	beq	.L341
.L290:
	add	r2, r2, #1
	add	r3, r3, #544
	cmp	r2, #32
	bne	.L292
.L293:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldr	r1, .L348+8
	blx	r3
	ldr	r3, [r7, #16]
	mov	r0, #3
	blx	r3
	mvn	r0, #0
.L332:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L341:
	mov	r6, r2, asl #5
	mov	r9, r2, asl #9
	add	r4, r6, r9
	add	r3, r5, r4
	add	r4, r4, #72
	add	ip, r3, #72
	adds	r4, r5, r4
	strb	r2, [ip, #3]
	strb	r0, [ip, #2]
	beq	.L293
	add	lr, r3, #592
	add	r1, r3, #600
	cmp	r10, #0
	str	r0, [r3, #592]
	str	r0, [lr, #4]
	str	r0, [r3, #600]
	str	r0, [r1, #4]
	beq	.L294
	ldrsb	ip, [r5, #10]
	cmp	ip, #0
	addgt	r3, r5, #17408
	movgt	r2, r0
	addgt	r3, r3, #204
	bgt	.L299
	b	.L336
.L298:
	add	r2, r2, #1
	add	r3, r3, #68
	cmp	r2, ip
	beq	.L342
.L299:
	ldr	r1, [r3]
	cmp	r1, #1
	bne	.L298
	ldrsb	r1, [r3, #-64]
	cmp	r1, #0
	bne	.L298
	ldrsb	r1, [r3, #-62]
	cmp	r1, #0
	bne	.L298
	mov	r3, r2, asl #6
	add	r1, r6, r9
	add	r3, r3, r2, lsl #2
	add	r1, r5, r1
	add	r3, r5, r3
	add	r3, r3, #17408
	add	r3, r3, #140
	str	r3, [r1, #592]
	b	.L296
.L294:
	add	r0, r5, #20480
	add	r2, r5, #17408
	add	r2, r2, #72
	str	r2, [r3, #592]
	str	r2, [lr, #4]
	mov	lr, #1
	str	r2, [r3, #600]
	ldr	r3, [r0, #1560]
	str	r3, [r1, #4]
	ldrb	r3, [r5, #13]	@ zero_extendqisi2
	strb	lr, [ip, #2]
	sub	r3, r3, #1
	strb	r3, [ip, #4]
.L297:
	add	r6, r6, r9
	mov	lr, #1
	add	r3, r5, r6
	add	r0, r6, #80
	add	r0, r5, r0
	add	r8, r3, #72
	ldr	r6, [r3, #596]
	mov	r1, #0
	strb	lr, [r3, #73]
	mov	r2, #504
	strb	lr, [r6, #2]
	str	lr, [r3, #588]
	ldr	r6, [r3, #592]
	ldr	lr, [r5, #16]
	str	lr, [r3, #584]
	str	lr, [r6, #56]
	ldr	lr, [r3, #596]
	ldr	r6, [r3, #584]
	str	r6, [lr, #56]
	ldr	lr, [r5, #16]
	add	lr, lr, #1
	str	lr, [r5, #16]
	add	r5, r3, #592
	ldr	r10, [r7, #48]
	ldmia	r5, {r5, r6, r9}
	blx	r10
	ldrsb	r3, [r8, #3]
	str	r3, [r4, #204]
	ldr	r3, [r5, #36]
	str	r3, [r4, #172]
	ldr	r3, [r5, #40]
	str	r3, [r4, #176]
	ldr	r3, [r6, #20]
	str	r3, [r4, #200]
	ldr	r2, [r5, #12]
	str	r2, [r4, #112]
	mov	r0, r2
	ldr	r3, [r5, #44]
	add	r3, r3, r2
	str	r3, [r4, #116]
	bl	MEM_Phy2Vir
	str	r0, [r4, #40]
	ldr	r0, [r4, #116]
	bl	MEM_Phy2Vir
	str	r0, [r4, #44]
	ldr	r1, [r6, #12]
	str	r1, [r4, #96]
	ldr	r3, [r6, #44]
	add	r3, r1, r3
	str	r3, [r4, #100]
	ldr	r2, [r6, #20]
	add	r2, r2, r1
	str	r2, [r4, #104]
	ldr	r2, [r6, #20]
	add	r3, r3, r2
	str	r3, [r4, #108]
	ldr	r2, [r6, #12]
	str	r2, [r4, #128]
	mov	r0, r2
	ldr	r3, [r6, #44]
	add	r3, r3, r2
	str	r3, [r4, #132]
	bl	MEM_Phy2Vir
	str	r0, [r4, #56]
	ldr	r0, [r4, #132]
	bl	MEM_Phy2Vir
	str	r0, [r4, #60]
	ldr	r2, [r9, #12]
	str	r2, [r4, #120]
	mov	r0, r2
	ldr	r3, [r5, #44]
	add	r3, r3, r2
	str	r3, [r4, #124]
	bl	MEM_Phy2Vir
	str	r0, [r4, #48]
	ldr	r0, [r4, #124]
	bl	MEM_Phy2Vir
	ldr	ip, [r4, #112]
	ldr	r1, [r4, #40]
	str	r0, [r4, #52]
	ldr	r3, [r5, #24]
	str	r3, [r4, #152]
	ldr	r2, [r5, #28]
	add	r2, r2, ip
	str	r2, [r4, #140]
	ldr	r2, [r5, #28]
	ldr	r3, [r5, #32]
	add	r3, r2, r3
	add	r3, r3, ip
	str	r3, [r4, #148]
	ldr	r3, [r5, #28]
	add	r3, r1, r3
	str	r3, [r4, #136]
	ldr	r2, [r5, #28]
	ldr	r3, [r5, #32]
	add	r3, r3, r2
	add	r1, r1, r3
	str	r1, [r4, #144]
	ldr	r3, [r5, #16]
	str	r3, [r4, #284]
	ldr	r0, [r5, #16]
	bl	MEM_Phy2Vir
	ldr	r3, [r7, #16]
	str	r0, [r4, #64]
	mov	r0, #3
	blx	r3
	ldrsb	r0, [r8, #3]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L342:
	add	r3, r6, r9
	add	r3, r5, r3
.L336:
	ldr	r3, [r3, #592]
.L296:
	cmp	r3, #0
	beq	.L343
.L300:
	add	r3, r6, r9
	add	r2, r5, #21760
	add	r3, r5, r3
	add	r2, r2, #140
	str	r2, [r3, #600]
	ldrsb	r0, [r5, #13]
	cmp	r0, #0
	ble	.L310
	add	r3, r5, #22016
	mov	r1, #0
	add	r3, r3, #28
	b	.L315
.L312:
	add	r1, r1, #1
	add	r3, r3, #12
	cmp	r1, r0
	beq	.L344
.L315:
	ldr	r2, [r3]
	cmp	r2, #1
	bne	.L312
	ldr	r2, [r3, #4]
	cmp	r2, #0
	bne	.L312
	mov	r3, r1, asl #4
	add	lr, r6, r9
	sub	r3, r3, r1, asl #2
	add	lr, r5, lr
	add	r3, r5, r3
	movw	ip, #22040
	strb	r1, [lr, #76]
	add	r2, r5, #20480
	ldr	r3, [r3, ip]
	cmp	r1, r0
	str	r3, [lr, #604]
	ldr	ip, [r2, #1556]
	str	ip, [lr, #608]
	bge	.L313
.L310:
	ldr	r1, .L348+12
	add	r10, r6, r9
	add	r10, r5, r10
	ldr	r2, [r1, #-3896]
	ldr	r3, [r10, #592]
	cmp	r2, #0
	str	r3, [r10, #596]
	beq	.L297
	mov	r0, r8
	str	r1, [fp, #-84]
	bl	VCTRL_GetImageWidth
	str	r0, [fp, #-80]
	mov	r0, r8
	bl	VCTRL_GetImageHeight
	ldr	r1, [fp, #-84]
	ldr	r2, [fp, #-80]
	ldr	ip, [r1, #-3888]
	cmp	ip, #0
	beq	.L345
	mov	r3, r0
	sub	r1, fp, #72
	mov	r0, r8
	blx	ip
	cmp	r0, #0
	bne	.L319
	ldr	r8, [fp, #-72]
	cmp	r8, #0
	beq	.L346
	ldr	r3, [r10, #592]
	movw	r0, #22040
	str	r8, [r3, #12]
	ldrsb	r1, [r10, #76]
	ldr	r2, [fp, #-56]
	mov	r3, r1, asl #4
	sub	r3, r3, r1, asl #2
	str	r2, [r10, #604]
	add	r3, r5, r3
	str	r2, [r3, r0]
	b	.L297
.L344:
	add	r2, r5, #20480
	cmp	r0, #0
	ldr	ip, [r2, #1556]
	ble	.L317
.L313:
	add	r1, r5, #22016
	mov	r3, #0
	add	r1, r1, #32
	mov	lr, r3
.L316:
	add	r3, r3, #1
	str	lr, [r1], #12
	cmp	r3, r0
	bne	.L316
.L317:
	add	r3, r6, r9
	mov	lr, #0
	add	r3, r5, r3
	mov	r0, lr
	ldr	r1, .L348+16
	strb	lr, [r3, #76]
	ldr	r2, [r2, #1560]
	str	ip, [r3, #608]
	str	r2, [r3, #604]
	ldr	r3, [r7, #68]
	blx	r3
	b	.L310
.L343:
	mov	r0, r3
	ldr	r2, .L348+20
	ldr	ip, [r7, #68]
	ldr	r1, .L348+24
	str	r3, [fp, #-80]
	blx	ip
	ldrsb	r2, [r5, #10]
	ldr	r3, [fp, #-80]
	cmp	r2, #0
	ble	.L305
	add	r10, r5, #17408
	str	r4, [fp, #-80]
	add	r10, r10, #140
	mov	r4, r3
.L304:
	ldrsb	r3, [r10, #2]
	mov	r0, #0
	ldr	ip, [r7, #68]
	add	r4, r4, #1
	ldrsb	r2, [r10], #68
	ldr	r1, .L348+28
	blx	ip
	ldrsb	r3, [r5, #10]
	cmp	r3, r4
	bgt	.L304
	ldr	r4, [fp, #-80]
.L305:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldr	r1, .L348+32
	blx	r3
	ldrsb	r3, [r5, #9]
	ldr	r10, .L348+4
	cmp	r3, #0
	ble	.L347
	ldrsb	ip, [r5, #10]
	add	r3, r5, #17408
	add	r3, r3, #140
	cmp	ip, #0
	ble	.L306
	mov	lr, #0
	mvn	r0, #0
	mov	r2, lr
.L308:
	ldr	r1, [r3, #56]
	add	r3, r3, #64
	cmp	r1, r0
	movcc	lr, r2
	add	r2, r2, #1
	movcc	r0, r1
	cmp	ip, r2
	bne	.L308
	mov	r3, lr, asl #6
	add	r3, r3, lr, lsl #2
	add	r3, r5, r3
	add	r3, r3, #17408
	add	r3, r3, #140
.L306:
	add	r2, r6, r9
	add	r2, r5, r2
	str	r3, [r2, #592]
	b	.L300
.L345:
	mov	r0, ip
	ldr	r3, [r7, #68]
	ldr	r1, .L348+36
	blx	r3
.L319:
	ldr	r3, [r7, #16]
	mov	r0, #3
	blx	r3
	ldr	r3, [r7, #68]
	ldr	r1, .L348+40
	mov	r0, #0
	blx	r3
	mvn	r0, #0
	b	.L332
.L339:
	ldr	ip, .L348+4
	movw	r2, #1636
	ldr	r3, .L348+44
	mov	r0, #0
	ldr	r1, .L348+48
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L332
.L340:
	ldr	ip, .L348+4
	movw	r2, #1637
	ldr	r3, .L348+52
	ldr	r1, .L348+48
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L332
.L346:
	ldr	r3, [r7, #16]
	mov	r0, #3
	blx	r3
	ldr	r3, [r7, #68]
	mov	r0, r8
	ldr	r1, .L348+56
	blx	r3
	mvn	r0, #0
	b	.L332
.L347:
	ldr	r3, [r10, #68]
	mov	r0, #0
	ldr	r2, .L348+20
	ldr	r1, .L348+60
	blx	r3
	ldr	r3, [r10, #16]
	mov	r0, #3
	blx	r3
	mvn	r0, #0
	b	.L332
.L349:
	.align	2
.L348:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC18
	.word	.LANCHOR1
	.word	.LC23
	.word	.LANCHOR2
	.word	.LC19
	.word	.LC21
	.word	.LC20
	.word	.LC5
	.word	.LC24
	.word	.LC0
	.word	.LC1
	.word	.LC4
	.word	.LC25
	.word	.LC22
	UNWIND(.fnend)
	.size	FSP_NewLogicFs, .-FSP_NewLogicFs
	.align	2
	.global	FSP_RemovePmv
	.type	FSP_RemovePmv, %function
FSP_RemovePmv:
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
	bhi	.L357
	cmp	r1, #31
	bhi	.L358
	ldr	r3, .L360
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L359
	mov	r2, r1, asl #5
	mov	r1, r1, asl #9
	add	r3, r2, r1
	add	r3, r0, r3
	ldr	ip, [r3, #588]
	cmp	ip, #2
	ldmnefd	sp, {r4, fp, sp, pc}
	ldrb	r3, [r3, #76]	@ zero_extendqisi2
	cmp	r3, #19
	bhi	.L355
	sxtb	r3, r3
	movw	lr, #22048
	mov	r4, #0
	mov	ip, r3, asl #4
	sub	r3, ip, r3, asl #2
	add	r3, r0, r3
	str	r4, [r3, lr]
.L355:
	add	r1, r2, r1
	mov	r3, #0
	add	r0, r0, r1
	str	r3, [r0, #588]
	ldmfd	sp, {r4, fp, sp, pc}
.L357:
	ldr	ip, .L360+4
	movw	r2, #2006
	ldr	r3, .L360+8
	mov	r0, #0
.L356:
	ldr	r1, .L360+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	bx	ip
.L358:
	ldr	ip, .L360+4
	movw	r2, #2007
	ldr	r3, .L360+16
	mov	r0, #0
	b	.L356
.L359:
	ldr	ip, .L360+4
	movw	r2, #2008
	ldr	r3, .L360+20
	b	.L356
.L361:
	.align	2
.L360:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	.LC1
	.word	.LC26
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
	mov	r6, r2
	bhi	.L394
	cmp	r1, #31
	bhi	.L395
	ldr	ip, .L405
	ldr	r7, [ip, r0, asl #2]
	cmp	r7, #0
	beq	.L396
	mov	r8, r1, asl #5
	mov	r5, r1, asl #9
	add	r4, r8, r5
	uxtb	r9, r2
	add	r4, r7, r4
	ldrsb	r3, [r4, #74]
	strb	r9, [r4, #72]
	cmp	r3, #0
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r3, [r4, #592]
	cmp	r3, #0
	beq	.L368
	ldr	ip, [r3, #56]
	ldr	r1, [r4, #584]
	cmp	ip, r1
	beq	.L397
.L368:
	add	r3, r8, r5
	add	r3, r7, r3
	ldrb	r2, [r3, #76]	@ zero_extendqisi2
	cmp	r2, #19
	bhi	.L371
	cmp	r6, #1
	beq	.L398
	cmp	r6, #0
	bne	.L374
	ldr	r0, [r3, #588]
	cmp	r0, #2
	beq	.L399
.L381:
	add	r3, r8, r5
	add	r3, r7, r3
	ldr	r3, [r3, #596]
	cmp	r3, #0
	beq	.L385
	ldrsb	r2, [r3, #2]
	cmp	r2, #1
	moveq	r2, #0
	streqb	r2, [r3, #2]
.L385:
	add	r1, r8, r5
	add	r7, r7, r1
	str	r6, [r7, #592]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L394:
	ldr	ip, .L405+4
	movw	r2, #2040
	ldr	r3, .L405+8
	mov	r0, #0
.L393:
	ldr	r1, .L405+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	ip
.L398:
	ldr	r1, [r3, #588]
	add	r3, r3, #584
	cmp	r1, #1
	beq	.L400
.L373:
	add	r3, r8, r5
	add	r3, r7, r3
	ldr	r2, [r3, #596]
	cmp	r2, #0
	beq	.L401
.L380:
	add	r3, r8, r5
	ldr	r0, [r2, #56]
	add	r3, r7, r3
	ldr	r1, [r3, #584]
	cmp	r6, #1
	cmpeq	r0, r1
	beq	.L402
.L377:
	cmp	r6, #0
	beq	.L385
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L395:
	ldr	ip, .L405+4
	movw	r2, #2041
	ldr	r3, .L405+16
	mov	r0, #0
	b	.L393
.L371:
	ldr	r3, .L405+4
	sxtb	r2, r2
	ldr	r1, .L405+20
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	cmp	r6, #0
	beq	.L381
	b	.L373
.L397:
	ldrsb	r1, [r3]
	cmp	r2, r1
	beq	.L369
	ldr	r1, .L405+24
	ldr	ip, [r1, #-3896]
	cmp	ip, #0
	bne	.L403
.L369:
	strb	r9, [r3]
	b	.L368
.L400:
	sxtb	r2, r2
	movw	ip, #22048
	mov	r0, #2
	mov	r1, r2, asl #4
	sub	r2, r1, r2, asl #2
	add	r2, r7, r2
	str	r6, [r2, ip]
	str	r0, [r3, #4]
.L374:
	add	r3, r8, r5
	add	r3, r7, r3
	ldr	r2, [r3, #596]
	cmp	r2, #0
	bne	.L380
	b	.L377
.L403:
	ldr	ip, [r1, #-3880]
	ldr	r1, [r3, #12]
	cmp	ip, #0
	beq	.L404
	blx	ip
	ldr	r3, [r4, #592]
	b	.L369
.L399:
	sxtb	r2, r2
	movw	ip, #22048
	mov	r0, r2, asl #4
	sub	r2, r0, r2, asl #2
	add	r2, r7, r2
	str	r6, [r2, ip]
	str	r6, [r3, #604]
	str	r6, [r3, #588]
	b	.L381
.L396:
	ldr	ip, .L405+4
	mov	r0, r7
	ldr	r3, .L405+28
	movw	r2, #2042
	b	.L393
.L402:
	ldrsb	r1, [r2, #2]
	cmp	r1, #1
	moveq	r1, #2
	streqb	r1, [r2, #2]
	streqb	r1, [r3, #73]
	b	.L377
.L401:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L404:
	ldr	r3, .L405+4
	mov	r0, ip
	ldr	r1, .L405+32
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [r4, #592]
	b	.L369
.L406:
	.align	2
.L405:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	.LC1
	.word	.LC26
	.word	.LC27
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
	bhi	.L413
	cmp	r1, #31
	bhi	.L414
	ldr	r3, .L416
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L415
	mov	r0, r1, asl #9
	add	r1, r0, r1, lsl #5
	add	r3, r3, r1
	ldrsb	r0, [r3, #72]
	ldmfd	sp, {fp, sp, pc}
.L413:
	ldr	ip, .L416+4
	movw	r2, #2161
	ldr	r3, .L416+8
.L412:
	ldr	r1, .L416+12
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L414:
	ldr	ip, .L416+4
	movw	r2, #2162
	ldr	r3, .L416+16
	b	.L412
.L415:
	ldr	ip, .L416+4
	mov	r0, r3
	movw	r2, #2163
	ldr	r3, .L416+20
	ldr	r1, .L416+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L417:
	.align	2
.L416:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC28
	.word	.LC1
	.word	.LC29
	.word	.LC30
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #4)
	sub	sp, sp, #4
	cmp	r0, #15
	mov	r6, r0
	mov	r10, r2
	bhi	.L445
	cmp	r1, #31
	bhi	.L446
	ldr	r4, .L451
	ldr	r0, [r4, r0, asl #2]
	cmp	r0, #0
	beq	.L447
	ldr	r8, .L451+4
	mov	r0, #3
	mov	r7, r1, asl #5
	mov	r5, r1, asl #9
	clz	r9, r2
	ldr	r3, [r8, #12]
	blx	r3
	ldr	r4, [r4, r6, asl #2]
	add	r3, r7, r5
	mov	r9, r9, lsr #5
	add	r3, r4, r3
	add	r2, r3, #72
	ldrsb	r1, [r3, #74]
	cmp	r1, #0
	cmpeq	r10, #0
	bne	.L422
	ldr	r3, [r3, #596]
	cmp	r3, #0
	beq	.L422
	ldrsb	r2, [r2, #1]
	cmp	r2, #0
	beq	.L422
	ldr	r2, .L451+8
	ldr	r1, [r2, #-3896]
	cmp	r1, #0
	bne	.L448
.L422:
	add	r1, r7, r5
	cmp	r9, #0
	add	r1, r4, r1
	add	ip, r1, #72
	movne	r3, #0
	ldrsb	r0, [ip, #2]
	moveq	r3, #3
	strb	r3, [ip, #1]
	cmp	r0, #0
	bne	.L429
	add	lr, r1, #592
	ldr	r3, [lr, #4]
	cmp	r3, #0
	beq	.L429
	ldr	r3, [r3, #56]
	ldr	r2, [r1, #584]
	cmp	r3, r2
	bne	.L449
	cmp	r9, #0
	strneb	r0, [ip, #1]
	moveq	r3, #3
	ldrne	r3, [lr, #4]
	streqb	r3, [ip, #1]
	ldreq	r2, [lr, #4]
	strneb	r0, [r3, #2]
	strne	r0, [lr, #4]
	streqb	r3, [r2, #2]
.L429:
	ldr	r3, [r8, #16]
	mov	r0, #3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	r3
.L445:
	ldr	ip, .L451+4
	movw	r2, #2180
	ldr	r3, .L451+12
.L443:
	mov	r0, #0
.L444:
	ldr	r1, .L451+16
	ldr	ip, [ip, #68]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	ip
.L446:
	ldr	ip, .L451+4
	movw	r2, #2181
	ldr	r3, .L451+20
	b	.L443
.L449:
	ldr	ip, [r8, #68]
	ldr	r1, .L451+24
	blx	ip
	b	.L429
.L447:
	ldr	ip, .L451+4
	movw	r2, #2182
	ldr	r3, .L451+28
	b	.L444
.L448:
	ldr	r2, [r2, #-3884]
	ldr	r1, [r3, #12]
	cmp	r2, #0
	beq	.L450
	mov	r0, r6
	blx	r2
	b	.L422
.L450:
	mov	r0, r2
	ldr	r3, [r8, #68]
	ldr	r1, .L451+32
	blx	r3
	b	.L422
.L452:
	.align	2
.L451:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1
	.word	.LC0
	.word	.LC1
	.word	.LC26
	.word	.LC31
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
	bhi	.L464
	cmp	r1, #0
	beq	.L465
	ldr	r3, .L468
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	addne	r6, r0, #17408
	addne	r4, r0, #80
	addne	r6, r6, #80
	bne	.L459
	b	.L466
.L458:
	add	r4, r4, #544
	cmp	r4, r6
	beq	.L467
.L459:
	mov	r2, #0
	ldr	r1, [r4, #196]
	mov	r0, r5
	bl	FSP_SetRef
	mov	r1, r4
	mov	r0, r7
	bl	IsImageInVoQueue
	cmp	r0, #0
	beq	.L458
	ldr	r1, [r4, #196]
	mov	r2, #0
	mov	r0, r5
	add	r4, r4, #544
	bl	FSP_SetDisplay
	cmp	r4, r6
	bne	.L459
.L467:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L464:
	ldr	ip, .L468+4
	movw	r2, #578
	ldr	r3, .L468+8
	mov	r0, #0
	ldr	r1, .L468+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L466:
	ldr	ip, .L468+4
	mov	r2, #580
	ldr	r3, .L468+16
	ldr	r1, .L468+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L465:
	ldr	ip, .L468+4
	mov	r0, r1
	ldr	r3, .L468+20
	movw	r2, #579
	ldr	r1, .L468+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L469:
	.align	2
.L468:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	.LC1
	.word	.LC4
	.word	.LC32
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
	ldr	r3, .L474
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L473
	mov	r2, r1, asl #9
	add	r1, r2, r1, lsl #5
	add	r3, r0, r1
	ldrsb	r0, [r3, #73]
	ldmfd	sp, {fp, sp, pc}
.L473:
	ldr	ip, .L474+4
	movw	r2, #2249
	ldr	r3, .L474+8
	ldr	r1, .L474+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L475:
	.align	2
.L474:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	bhi	.L484
	ldr	ip, .L487
	ldr	ip, [ip, r0, asl #2]
	cmp	ip, #0
	beq	.L485
	cmp	r1, #0
	bne	.L480
	ldrsb	r0, [ip, #10]
	cmp	r0, r2
	ble	.L483
	mov	lr, r2, asl #6
	mov	r0, r1
	add	r2, lr, r2, lsl #2
	add	ip, ip, r2
	add	ip, ip, #17408
	str	r3, [ip, #148]
	ldmfd	sp, {fp, sp, pc}
.L480:
	cmp	r1, #1
	beq	.L486
.L483:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L486:
	ldrsb	r1, [ip, #11]
	cmp	r2, r1
	bge	.L483
	mov	r1, r2, asl #6
	mov	r0, #0
	add	r2, r1, r2, lsl #2
	add	ip, ip, r2
	add	ip, ip, #19712
	str	r3, [ip, #20]
	ldmfd	sp, {fp, sp, pc}
.L484:
	ldr	ip, .L487+4
	movw	r2, #2265
	ldr	r3, .L487+8
	mov	r0, #0
	ldr	r1, .L487+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L485:
	ldr	lr, .L487+4
	mov	r0, ip
	ldr	r3, .L487+16
	movw	r2, #2266
	ldr	r1, .L487+12
	ldr	ip, [lr, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L488:
	.align	2
.L487:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	bhi	.L497
	ldr	r3, .L500
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L498
	cmp	r1, #0
	bne	.L493
	ldrsb	r3, [r0, #10]
	cmp	r3, r2
	ble	.L496
	mov	r3, r2, asl #6
	add	r2, r3, r2, lsl #2
	add	r0, r0, r2
	add	r0, r0, #17408
	add	r0, r0, #144
	ldr	r0, [r0, #4]
	ldmfd	sp, {fp, sp, pc}
.L493:
	cmp	r1, #1
	beq	.L499
.L496:
	mov	r0, #2
	ldmfd	sp, {fp, sp, pc}
.L497:
	ldr	ip, .L500+4
	movw	r2, #2306
	ldr	r3, .L500+8
	mov	r0, #0
	ldr	r1, .L500+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L498:
	ldr	ip, .L500+4
	movw	r2, #2307
	ldr	r3, .L500+16
	ldr	r1, .L500+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L499:
	ldrsb	r3, [r0, #11]
	cmp	r2, r3
	bge	.L496
	mov	r3, r2, asl #6
	add	r2, r3, r2, lsl #2
	add	r0, r0, r2
	add	r0, r0, #19712
	add	r0, r0, #16
	ldr	r0, [r0, #4]
	ldmfd	sp, {fp, sp, pc}
.L501:
	.align	2
.L500:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r6, .L512
	ldr	r3, [r6, r0, asl #2]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r2, #0
	bl	FSP_SetRef
	ldr	r2, [r6, r4, asl #2]
	mov	r3, r5, asl #9
	add	r3, r3, r5, lsl #5
	add	r3, r2, r3
	ldrsb	r3, [r3, #73]
	cmp	r3, #3
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r1, r5
	mov	r0, r4
	mov	r2, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	FSP_SetDisplay
.L513:
	.align	2
.L512:
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
	bhi	.L517
	ldr	r3, .L518
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L517
	mov	r2, r1, asl #9
	add	r0, r2, r1, lsl #5
	add	r0, r0, #72
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L517:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L519:
	.align	2
.L518:
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
	bhi	.L523
	ldr	r3, .L524
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L523
	mov	r2, r1, asl #9
	add	r0, r2, r1, lsl #5
	add	r0, r0, #80
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L523:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L525:
	.align	2
.L524:
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
	bhi	.L528
	ldr	r3, .L530
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, r1, asl #9
	addne	r1, r3, r1, lsl #5
	addne	r0, r0, r1
	ldrne	r0, [r0, #592]
	ldmfd	sp, {fp, sp, pc}
.L528:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L531:
	.align	2
.L530:
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
	bhi	.L534
	ldr	r3, .L536
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, r1, asl #9
	addne	r1, r3, r1, lsl #5
	addne	r0, r0, r1
	ldrne	r0, [r0, #596]
	ldmfd	sp, {fp, sp, pc}
.L534:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L537:
	.align	2
.L536:
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
	bhi	.L540
	ldr	r3, .L542
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, r1, asl #9
	addne	r1, r3, r1, lsl #5
	addne	r0, r0, r1
	ldrne	r0, [r0, #604]
	ldmfd	sp, {fp, sp, pc}
.L540:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L543:
	.align	2
.L542:
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
	bhi	.L555
	cmp	r2, #0
	beq	.L556
	cmp	r1, #0
	beq	.L557
	ldr	ip, .L559
	ldr	lr, [ip, r0, asl #2]
	cmp	lr, #0
	beq	.L558
	mov	lr, #0
	str	lr, [r1]
	ldr	r4, [ip, r0, asl #2]
	ldrsb	r4, [r4, #10]
	cmp	r4, lr
	str	r4, [r1]
	ldr	ip, [ip, r0, asl #2]
	add	ip, ip, #17408
	add	ip, ip, #140
	ldmlefd	sp, {r4, fp, sp, pc}
	sub	r2, r2, #4
	sub	r3, r3, #4
.L551:
	ldr	r0, [ip, #12]
	add	lr, lr, #1
	add	ip, ip, #68
	str	r0, [r2, #4]!
	ldr	r0, [ip, #-52]
	str	r0, [r3, #4]!
	ldr	r4, [r1]
	cmp	r4, lr
	bgt	.L551
	ldmfd	sp, {r4, fp, sp, pc}
.L555:
	ldr	ip, .L559+4
	movw	r2, #2465
	ldr	r3, .L559+8
	mov	r0, #0
.L554:
	ldr	r1, .L559+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	bx	ip
.L556:
	mov	r0, r2
	ldr	ip, .L559+4
	ldr	r3, .L559+16
	movw	r2, #2466
	b	.L554
.L557:
	ldr	ip, .L559+4
	mov	r0, r1
	ldr	r3, .L559+20
	movw	r2, #2467
	b	.L554
.L558:
	ldr	ip, .L559+4
	mov	r0, lr
	ldr	r3, .L559+24
	movw	r2, #2468
	b	.L554
.L560:
	.align	2
.L559:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	.LC1
	.word	.LC33
	.word	.LC34
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
	bhi	.L572
	cmp	r2, #0
	beq	.L573
	cmp	r1, #0
	beq	.L574
	ldr	ip, .L576
	ldr	r3, [ip, r0, asl #2]
	cmp	r3, #0
	beq	.L575
	mov	r3, #0
	str	r3, [r1]
	ldr	ip, [ip, r0, asl #2]
	ldrsb	r0, [ip, #13]
	str	r0, [r1]
	ldrsb	r1, [ip, #13]
	cmp	r1, r3
	ldmlefd	sp, {fp, sp, pc}
	add	r1, ip, #22016
	sub	r2, r2, #4
	add	r1, r1, #24
.L568:
	ldr	r0, [r1], #12
	add	r3, r3, #1
	str	r0, [r2, #4]!
	ldrsb	r0, [ip, #13]
	cmp	r0, r3
	bgt	.L568
	ldmfd	sp, {fp, sp, pc}
.L572:
	ldr	ip, .L576+4
	movw	r2, #2510
	ldr	r3, .L576+8
	mov	r0, #0
.L571:
	ldr	r1, .L576+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L573:
	mov	r0, r2
	ldr	ip, .L576+4
	ldr	r3, .L576+16
	movw	r2, #2511
	b	.L571
.L574:
	ldr	ip, .L576+4
	mov	r0, r1
	ldr	r3, .L576+20
	mov	r2, #2512
	b	.L571
.L575:
	mov	r0, r3
	ldr	ip, .L576+4
	ldr	r3, .L576+24
	movw	r2, #2513
	b	.L571
.L577:
	.align	2
.L576:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	.LC1
	.word	.LC35
	.word	.LC36
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
	ldr	r3, .L581
	mov	ip, r1
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L580
	ldrb	lr, [r3, #10]	@ zero_extendqisi2
	add	r4, r3, #16384
	mov	r0, r1
	ldr	r5, .L581+4
	add	r1, r3, #17408
	mov	r2, #2176
	strb	lr, [ip, #1]
	add	r1, r1, #140
	ldr	lr, [r4, #1244]
	ldr	r4, [r4, #1176]
	rsb	lr, r4, lr
	str	lr, [ip, #2184]
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
.L580:
	ldr	ip, .L581+4
	mov	r0, r3
	movw	r2, #2535
	ldr	r3, .L581+8
	ldr	r1, .L581+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	ip
.L582:
	.align	2
.L581:
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
	bhi	.L594
	ldr	r3, .L596
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, r0
	movne	r0, #0
	beq	.L595
.L588:
	ldr	r2, [r3, #592]
	add	r3, r3, #544
	cmp	r2, #0
	beq	.L587
	ldr	r2, [r2, #12]
	cmp	r2, r1
	ldmeqfd	sp, {fp, sp, pc}
.L587:
	add	r0, r0, #1
	cmp	r0, #32
	bne	.L588
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L594:
	ldr	ip, .L596+4
	movw	r2, #2564
	ldr	r3, .L596+8
	mov	r0, #0
	ldr	r1, .L596+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L595:
	ldr	ip, .L596+4
	movw	r2, #2565
	ldr	r3, .L596+16
	ldr	r1, .L596+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L597:
	.align	2
.L596:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #15
	bhi	.L630
	ldr	r3, .L633
	ldr	r8, [r3, r0, asl #2]
	cmp	r8, #0
	beq	.L631
	ldr	r7, .L633+4
	mov	r0, #0
	mov	r4, r0
	ldr	r1, .L633+8
	mov	r5, r8
	mov	r6, #1
	ldr	r3, [r7, #68]
	blx	r3
	ldr	ip, [r7, #68]
	ldr	r3, [r8, #4]
	mov	r0, r4
	ldr	r2, .L633+12
	ldr	r1, .L633+16
	blx	ip
	ldr	ip, [r7, #68]
	ldr	r3, [r8, #44]
	mov	r0, r4
	ldr	r2, .L633+20
	ldr	r1, .L633+16
	blx	ip
	ldr	ip, [r7, #68]
	ldr	r3, [r8, #48]
	mov	r0, r4
	ldr	r2, .L633+24
	ldr	r1, .L633+16
	blx	ip
	ldr	ip, [r7, #68]
	ldr	r3, [r8, #60]
	mov	r0, r4
	ldr	r2, .L633+28
	ldr	r1, .L633+16
	blx	ip
	ldr	ip, [r7, #68]
	ldr	r3, [r8, #64]
	mov	r0, r4
	ldr	r2, .L633+32
	ldr	r1, .L633+16
	blx	ip
	ldr	ip, [r7, #68]
	ldrsb	r3, [r8, #40]
	mov	r0, r4
	ldr	r2, .L633+36
	ldr	r1, .L633+16
	blx	ip
	ldr	ip, [r7, #68]
	ldrsb	r3, [r8, #42]
	mov	r0, r4
	ldr	r2, .L633+40
	ldr	r1, .L633+16
	blx	ip
	ldr	ip, [r7, #68]
	ldrsb	r3, [r8, #43]
	mov	r0, r4
	ldr	r2, .L633+44
	ldr	r1, .L633+16
	blx	ip
	ldr	ip, [r7, #68]
	ldrsb	r3, [r8, #10]
	mov	r0, r4
	ldr	r2, .L633+48
	ldr	r1, .L633+16
	blx	ip
	ldr	ip, [r7, #68]
	ldrsb	r3, [r8, #11]
	mov	r0, r4
	ldr	r2, .L633+52
	ldr	r1, .L633+16
	blx	ip
	ldrsb	r3, [r8, #13]
	ldr	ip, [r7, #68]
	mov	r0, r4
	ldr	r2, .L633+56
	ldr	r1, .L633+16
	blx	ip
	ldr	r3, [r7, #68]
	mov	r2, #32
	ldr	r1, .L633+60
	mov	r0, r4
	blx	r3
	b	.L601
.L603:
	ldr	ip, [r7, #68]
	mov	r0, #0
	ldrsb	r3, [r5, #73]
	ldrsb	r2, [r5, #72]
	ldr	r1, .L633+64
	blx	ip
	cmp	r6, #31
	bgt	.L632
.L604:
	add	r4, r4, #1
	add	r6, r6, #1
	add	r5, r5, #544
.L601:
	cmp	r4, #0
	ldr	r9, .L633+4
	beq	.L602
	ands	r0, r4, #7
	bne	.L603
	ldr	r3, [r7, #68]
	ldr	r1, .L633+68
	blx	r3
	b	.L603
.L632:
	ldr	r3, [r9, #68]
	mov	r0, #0
	ldrsb	r2, [r8, #10]
	ldr	r1, .L633+72
	blx	r3
	ldrsb	r3, [r8, #10]
	cmp	r3, #0
	addgt	r5, r8, #17408
	addgt	r5, r5, #140
	movgt	r4, #0
	ble	.L611
.L608:
	ldrsb	r3, [r5, #2]
	mov	r0, #0
	ldrsb	r2, [r5]
	add	r4, r4, #1
	ldr	ip, [r7, #68]
	add	r5, r5, #68
	ldr	r1, .L633+64
	blx	ip
	ldrsb	r3, [r8, #10]
	cmp	r3, r4
	ble	.L611
	cmp	r4, #0
	beq	.L608
	ands	r0, r4, #7
	bne	.L608
	ldr	r3, [r9, #68]
	ldr	r1, .L633+68
	blx	r3
	b	.L608
.L611:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldrsb	r2, [r8, #11]
	ldr	r1, .L633+76
	blx	r3
	ldrsb	r3, [r8, #11]
	cmp	r3, #0
	ble	.L607
	add	r5, r8, #19712
	ldr	r6, .L633+4
	add	r5, r5, #12
	mov	r4, #0
.L614:
	ldrsb	r3, [r5, #2]
	mov	r0, #0
	ldrsb	r2, [r5]
	add	r4, r4, #1
	ldr	ip, [r7, #68]
	add	r5, r5, #68
	ldr	r1, .L633+64
	blx	ip
	ldrsb	r3, [r8, #11]
	cmp	r3, r4
	ble	.L607
	cmp	r4, #0
	beq	.L614
	ands	r0, r4, #7
	bne	.L614
	ldr	r3, [r6, #68]
	ldr	r1, .L633+68
	blx	r3
	b	.L614
.L607:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldrsb	r2, [r8, #13]
	ldr	r1, .L633+80
	blx	r3
	ldrsb	r3, [r8, #13]
	cmp	r3, #0
	ble	.L613
	add	r5, r8, #22016
	ldr	r6, .L633+4
	add	r5, r5, #32
	mov	r4, #0
.L617:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldr	r2, [r5]
	add	r4, r4, #1
	ldr	r1, .L633+84
	add	r5, r5, #12
	blx	r3
	ldrsb	r3, [r8, #13]
	cmp	r3, r4
	ble	.L613
	cmp	r4, #0
	beq	.L617
	ands	r0, r4, #15
	bne	.L617
	ldr	r3, [r6, #68]
	ldr	r1, .L633+68
	blx	r3
	b	.L617
.L613:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldr	r1, .L633+88
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L630:
	ldr	ip, .L633+4
	movw	r2, #2596
	ldr	r3, .L633+92
	mov	r0, #0
.L629:
	ldr	r1, .L633+96
	ldr	ip, [ip, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	ip
.L631:
	ldr	ip, .L633+4
	mov	r0, r8
	ldr	r3, .L633+100
	movw	r2, #2597
	b	.L629
.L602:
	ldr	ip, [r7, #68]
	mov	r0, r4
	ldrsb	r3, [r5, #73]
	ldrsb	r2, [r5, #72]
	ldr	r1, .L633+64
	blx	ip
	b	.L604
.L634:
	.align	2
.L633:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	.word	.LC49
	.word	.LC50
	.word	.LC21
	.word	.LC20
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
	.global	FSP_IsFsLeak
	.type	FSP_IsFsLeak, %function
FSP_IsFsLeak:
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
	UNWIND(.pad #28)
	sub	sp, sp, #28
	ldr	r3, .L646
	mov	r5, r0
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L644
	ldrsb	r4, [r3, #10]
	cmp	r4, #0
	ble	.L638
	add	r3, r3, #17408
	mov	r2, #0
	add	r3, r3, #140
.L641:
	ldrsb	r1, [r3]
	cmp	r1, #0
	bne	.L639
	ldrsb	r1, [r3, #2]
	cmp	r1, #0
	beq	.L640
.L639:
	add	r2, r2, #1
	add	r3, r3, #68
	cmp	r4, r2
	bne	.L641
.L638:
	sub	r3, fp, #36
	sub	r2, fp, #32
	sub	r1, fp, #40
	mov	r0, r5
	mov	r6, #0
	str	r6, [fp, #-36]
	str	r6, [fp, #-32]
	str	r6, [fp, #-40]
	bl	VCTRL_GetChanImgNum
	ldr	r2, [fp, #-40]
	ldr	r3, [fp, #-32]
	ldr	r0, [fp, #-36]
	add	r1, r2, r3
	add	r1, r1, r0
	add	r1, r1, #2
	cmp	r4, r1
	bgt	.L645
.L642:
	mov	r0, #0
.L637:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L640:
	cmp	r4, r2
	bgt	.L642
	b	.L638
.L645:
	ldr	r1, .L646+4
	stmia	sp, {r0, r4}
	mov	r0, r6
	ldr	ip, [r1, #68]
	ldr	r1, .L646+8
	blx	ip
	mov	r0, r5
	bl	FSP_PrintContext
	mov	r0, #1
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L644:
	ldr	r3, .L646+4
	mov	r0, #1
	ldr	r1, .L646+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L637
.L647:
	.align	2
.L646:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC57
	.word	.LC56
	UNWIND(.fnend)
	.size	FSP_IsFsLeak, .-FSP_IsFsLeak
	.align	2
	.global	FSP_IsNewFsAvalible
	.type	FSP_IsNewFsAvalible, %function
FSP_IsNewFsAvalible:
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
	bhi	.L666
	ldr	r3, .L670
	ldr	r4, [r3, r0, asl #2]
	cmp	r4, #0
	beq	.L667
	ldr	r3, .L670+4
	ldr	r2, [r3, #-3896]
	cmp	r2, #0
	beq	.L655
	ldr	r5, [r3, #-3892]
	cmp	r5, #0
	beq	.L668
	blx	r5
	cmp	r0, #0
	bne	.L669
.L655:
	ldrb	ip, [r4, #10]	@ zero_extendqisi2
	cmp	ip, #0
	beq	.L662
	sxtb	ip, ip
	cmp	ip, #0
	ble	.L656
	add	r3, r4, #17408
	mov	r2, #0
	add	r3, r3, #140
.L659:
	ldrsb	r1, [r3]
	cmp	r1, #0
	bne	.L657
	ldrsb	r1, [r3, #2]
	cmp	r1, #0
	beq	.L658
.L657:
	add	r2, r2, #1
	add	r3, r3, #68
	cmp	r2, ip
	bne	.L659
.L656:
	mov	r0, r6
	bl	FSP_IsFsLeak
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L658:
	cmp	ip, r2
	ble	.L656
.L662:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L669:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L668:
	ldr	r3, .L670+8
	mov	r0, r5
	ldr	r1, .L670+12
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L666:
	ldr	ip, .L670+8
	movw	r2, #1338
	ldr	r3, .L670+16
	mov	r0, #0
	ldr	r1, .L670+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L667:
	ldr	ip, .L670+8
	mov	r0, r4
	ldr	r3, .L670+24
	movw	r2, #1339
	ldr	r1, .L670+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L671:
	.align	2
.L670:
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC5
	.word	.LC0
	.word	.LC1
	.word	.LC4
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
	bhi	.L675
	mov	r2, r0, asl #8
	ldr	r3, .L676
	add	r0, r2, r0, lsl #3
	add	r0, r3, r0
	ldrb	r0, [r0, #64]	@ zero_extendqisi2
	ldmfd	sp, {fp, sp, pc}
.L675:
	ldr	ip, .L676+4
	movw	r2, #2697
	ldr	r3, .L676+8
	mov	r0, #0
	ldr	r1, .L676+12
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #255
	ldmfd	sp, {fp, sp, pc}
.L677:
	.align	2
.L676:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	bhi	.L680
	mov	r2, r0, asl #8
	ldr	r3, .L681
	add	r0, r2, r0, lsl #3
	mov	r2, #1
	add	r0, r3, r0
	strb	r2, [r0, #64]
	ldmfd	sp, {fp, sp, pc}
.L680:
	ldr	ip, .L681+4
	movw	r2, #2707
	ldr	r3, .L681+8
	mov	r0, #0
	ldr	r1, .L681+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L682:
	.align	2
.L681:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	ip, .L697
	ldr	r2, [ip, r0, asl #2]
	cmp	r2, #0
	beq	.L692
	mov	r3, r0, asl #8
	add	r3, r3, r0, lsl #3
	add	r3, ip, r3
	add	r3, r3, #64
	ldrsh	lr, [r3, #2]
	cmn	lr, #1
	beq	.L692
	ldrh	r3, [r3, #4]
	cmp	r3, lr
	beq	.L692
	ldr	r2, [r2, #4]
	cmp	r2, #1
	beq	.L696
.L692:
	mov	r0, #0
	ldmfd	sp, {r4, fp, sp, pc}
.L696:
	and	r3, r3, #63
	cmp	lr, r3
	beq	.L692
	mov	r4, r0, asl #6
	add	r0, r4, r0, lsl #1
	add	r4, r0, r3
	add	r4, ip, r4, lsl #2
	ldr	r4, [r4, #72]
	cmp	r4, r1
	bne	.L686
	b	.L694
.L687:
	ldr	r2, [r2, #72]
	cmp	r2, r1
	beq	.L694
.L686:
	add	r3, r3, #1
	and	r3, r3, #63
	add	r2, r0, r3
	cmp	lr, r3
	add	r2, ip, r2, lsl #2
	bne	.L687
	b	.L692
.L694:
	mov	r0, #1
	ldmfd	sp, {r4, fp, sp, pc}
.L698:
	.align	2
.L697:
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
	beq	.L721
	cmp	r2, #1
	bhi	.L722
	beq	.L723
	mov	r3, r0, asl #8
	mov	r10, r0, asl #3
	ldr	r5, .L724
	add	r9, r10, r3
	str	r3, [fp, #-48]
	add	r9, r5, r9
	ldrh	r7, [r9, #68]
	ldrh	r8, [r9, #70]
	and	r3, r7, #63
	cmp	r8, r3
	beq	.L704
	mov	r6, r0, asl #6
	add	r6, r6, r0, lsl #1
.L709:
	add	r2, r6, r3
	ldr	r0, [r1, #88]
	add	r2, r5, r2, lsl #2
	ldr	r2, [r2, #72]
	cmp	r0, r2
	beq	.L710
	cmp	r2, #0
	movne	ip, r3
	bne	.L706
	mov	lr, #2
.L705:
	mov	ip, r7
.L708:
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
	bne	.L708
	cmp	lr, #1
	add	r7, r7, #1
	and	r7, r7, #63
	strh	r7, [r9, #68]	@ movhi
	beq	.L704
.L706:
	add	r3, ip, #1
	and	r3, r3, #63
	cmp	r3, r8
	bne	.L709
.L704:
	ldr	r3, [fp, #-48]
	add	r10, r10, r3
	add	r5, r5, r10
	ldrsh	r2, [r5, #66]
	cmp	r7, r2
	moveq	r1, #0
	mvneq	r2, #0
	streqb	r1, [r5, #64]
	streqh	r2, [r5, #66]	@ movhi
.L699:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L710:
	mov	lr, #1
	b	.L705
.L722:
	ldr	ip, .L724+4
	mov	r2, #2768
	ldr	r3, .L724+8
	mov	r0, #0
.L719:
	ldr	r1, .L724+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	ip
.L723:
	ldr	ip, .L724
	mov	r3, r0, asl #8
	add	r3, r3, r0, lsl #3
	mov	r2, r0, asl #6
	add	r3, ip, r3
	add	r0, r2, r0, lsl #1
	ldr	lr, [r1, #88]
	ldrh	r2, [r3, #70]
	add	r1, r0, r2
	add	r2, r2, #1
	and	r2, r2, #63
	strh	r2, [r3, #70]	@ movhi
	add	r1, ip, r1, lsl #2
	str	lr, [r1, #72]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L721:
	ldr	ip, .L724+4
	mov	r0, r1
	ldr	r3, .L724+16
	movw	r2, #2767
	b	.L719
.L725:
	.align	2
.L724:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC59
	.word	.LC1
	.word	.LC58
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
	ldr	r3, .L734
	ldr	ip, [r3, r0, asl #2]
	add	r3, ip, #16896
	ldrsb	r3, [r3, #41]
	cmp	r3, #0
	beq	.L731
	add	r3, ip, #16384
	mov	r2, #30
	add	r3, r3, #9
	b	.L730
.L728:
	subs	r2, r2, #1
	bcc	.L733
.L730:
	ldrsb	r1, [r3]
	sub	r3, r3, #544
	cmp	r1, #0
	bne	.L728
.L729:
	mov	r3, r2, asl #9
	mov	r0, r2
	add	r2, r3, r2, lsl #5
	mov	r3, #0
	add	ip, ip, r2
	str	r3, [ip, #592]
	str	r3, [ip, #596]
	ldmfd	sp, {fp, sp, pc}
.L733:
	mov	r2, #0
	b	.L729
.L731:
	mov	r2, #31
	b	.L729
.L735:
	.align	2
.L734:
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
	ldr	ip, .L738
	movw	r2, #2879
	ldr	r3, .L738+4
	mov	r0, #0
	ldr	r1, .L738+8
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L739:
	.align	2
.L738:
	.word	vfmw_Osal_Func_Ptr_S
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
	bhi	.L744
	ldr	r3, .L746
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L745
	ldr	r0, [r0, #4]
	subs	r0, r0, #1
	mvnne	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L744:
	ldr	ip, .L746+4
	movw	r2, #2910
	ldr	r3, .L746+8
	mov	r0, #0
	ldr	r1, .L746+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L745:
	ldr	ip, .L746+4
	movw	r2, #2911
	ldr	r3, .L746+16
	ldr	r1, .L746+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L747:
	.align	2
.L746:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	bhi	.L752
	ldr	r3, .L754
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L753
	ldrsb	r0, [r0, #9]
	ldmfd	sp, {fp, sp, pc}
.L752:
	ldr	ip, .L754+4
	mov	r2, #2928
	ldr	r3, .L754+8
	mov	r0, #0
	ldr	r1, .L754+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L753:
	ldr	ip, .L754+4
	movw	r2, #2929
	ldr	r3, .L754+16
	ldr	r1, .L754+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L755:
	.align	2
.L754:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	.type	__func__.13749, %object
	.size	__func__.13749, 15
__func__.13749:
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
	ASCII(.ascii	"no logic frame store!!!\012\000" )
	.space	3
.LC19:
	ASCII(.ascii	"%s no decode phy fs\012\000" )
	.space	3
.LC20:
	ASCII(.ascii	"\012\000" )
	.space	2
.LC21:
	ASCII(.ascii	"%d%d \000" )
	.space	2
.LC22:
	ASCII(.ascii	"%s Not partition yet!\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"no pmv memory\012\000" )
	.space	1
.LC24:
	ASCII(.ascii	"FSP_AllocFrame failed\012\000" )
	.space	1
.LC25:
	ASCII(.ascii	"invalid stFrameInfo PhyAddr\012\000" )
	.space	3
.LC26:
	ASCII(.ascii	"LogicFrameID out of range\000" )
	.space	2
.LC27:
	ASCII(.ascii	"fsp.c, line %d: pstLogicFs->PmvIdc out of range(=%d" )
	ASCII(.ascii	")\012\000" )
	.space	2
.LC28:
	ASCII(.ascii	"FSP_GetRef: InstID out of range\000" )
.LC29:
	ASCII(.ascii	"FSP_GetRef: LogicFrameID out of range\000" )
	.space	2
.LC30:
	ASCII(.ascii	"FSP_GetRef: Fsp is not inited\000" )
	.space	2
.LC31:
	ASCII(.ascii	"FSP_SetDisplay: LogicTick=%d, DispTick=%d!\012\000" )
.LC32:
	ASCII(.ascii	"pstVoQue is NULL\000" )
	.space	3
.LC33:
	ASCII(.ascii	"DecFsAddrTab is NULL\012\000" )
	.space	2
.LC34:
	ASCII(.ascii	"DecFsNum is NULL\012\000" )
	.space	2
.LC35:
	ASCII(.ascii	"PmvAddrTab is NULL\012\000" )
.LC36:
	ASCII(.ascii	"PmvNum is NULL\012\000" )
.LC37:
	ASCII(.ascii	"FSP CONTEXT:\012\000" )
	.space	2
.LC38:
	ASCII(.ascii	"FspInstState\000" )
	.space	3
.LC39:
	ASCII(.ascii	"%-35s :%d\012\000" )
	.space	1
.LC40:
	ASCII(.ascii	"DecFsWidth\000" )
	.space	1
.LC41:
	ASCII(.ascii	"DecFsHeight\000" )
.LC42:
	ASCII(.ascii	"DispFsWidth\000" )
.LC43:
	ASCII(.ascii	"DispFsHeight\000" )
	.space	3
.LC44:
	ASCII(.ascii	"ExpectedDecFsNum\000" )
	.space	3
.LC45:
	ASCII(.ascii	"ExpectedDispFsNum\000" )
	.space	2
.LC46:
	ASCII(.ascii	"ExpectedPmvNum\000" )
	.space	1
.LC47:
	ASCII(.ascii	"DecFsNum\000" )
	.space	3
.LC48:
	ASCII(.ascii	"DispFsNum\000" )
	.space	2
.LC49:
	ASCII(.ascii	"PmvNum\000" )
	.space	1
.LC50:
	ASCII(.ascii	"logic fs: %d\012\000" )
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
	ASCII(.ascii	"FSP_IsFsLeak: fsp is not inited \012\000" )
	.space	2
.LC57:
	ASCII(.ascii	"No free fs: ref=%d,read=%d,new=%d,total=%d\012\000" )
.LC58:
	ASCII(.ascii	"p_image NULL\000" )
	.space	3
.LC59:
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
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
