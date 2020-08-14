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
	add	r6, r4, #40
	ldrb	r9, [r4, #10]	@ zero_extendqisi2
	ldrb	r8, [r4, #1]	@ zero_extendqisi2
	mov	r2, #16
	ldr	r10, [r4, #4]
	mov	r1, r6
	ldr	ip, [r5, #52]
	sub	r0, fp, #604
	blx	ip
	ldr	ip, [r5, #48]
	movw	r2, #21880
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
	add	r0, r0, #88
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
	add	r1, r1, #88
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
	movw	r2, #21880
	mov	r1, r5
	mov	r0, r4
	ldr	r3, [r3, #48]
	blx	r3
	add	r3, r4, #20480
	mov	r2, #1
	mov	r0, r5
	str	r2, [r4, #4]
	str	r5, [r3, #1388]
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
	add	r5, r7, #19456
	ldr	r9, .L94+8
	add	r4, r4, #154
	add	r5, r5, #154
	mov	r8, r10
	b	.L78
.L75:
	ldrsb	r3, [r4]
	cmp	r3, #0
	bne	.L91
.L73:
	add	r4, r4, #64
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
	add	r4, r4, #64
	ldr	r1, [r4, #-54]
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
	add	r0, r7, #88
	ldr	r3, [r9, #48]
	blx	r3
	ldrsb	r3, [r7, #10]
	cmp	r3, #0
	ble	.L110
	add	r4, r7, #17408
	ldr	r10, .L132+8
	mov	r5, #0
	add	r4, r4, #208
	mov	r6, r5
.L109:
	ldr	r1, [r4, #-44]
	cmp	r1, #0
	beq	.L104
	ldr	r2, [r10, #-3896]
	cmp	r2, #0
	beq	.L104
	ldrsb	r2, [r4, #-56]
	cmp	r2, #0
	bne	.L129
.L106:
	ldrsb	r2, [r4, #-54]
	cmp	r2, #0
	bne	.L130
.L104:
	strb	r6, [r4, #-56]
	add	r5, r5, #1
	strb	r6, [r4, #-54]
	str	r6, [r4], #64
	ldrsb	r2, [r7, #10]
	cmp	r2, r5
	bgt	.L109
.L110:
	ldrsb	r0, [r7, #11]
	cmp	r0, #0
	addgt	r3, r7, #19456
	movgt	r2, #0
	addgt	r3, r3, #152
	movgt	r1, r2
	ble	.L102
.L113:
	add	r2, r2, #1
	strb	r1, [r3]
	cmp	r0, r2
	strb	r1, [r3, #2]
	str	r1, [r3, #56]
	add	r3, r3, #64
	bne	.L113
.L102:
	ldrsb	r2, [r7, #13]
	cmp	r2, #0
	addgt	r2, r2, #13
	addgt	r3, r7, #13
	addgt	r2, r7, r2
	movgt	r1, #0
	ble	.L112
.L114:
	strb	r1, [r3, #1]!
	cmp	r3, r2
	bne	.L114
.L112:
	mov	r0, #0
	str	r0, [r7, #36]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L130:
	ldr	r2, [r10, #-3884]
	ldr	r1, [r4, #-44]
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
	ldr	ip, [r2, r0, asl #2]
	mov	r0, #0
	add	r2, ip, #204800
	ldrsb	lr, [r2, r3]
	cmp	lr, #0
	ble	.L138
	mov	r2, r0
.L137:
	mov	r1, r2, asl #6
	movw	r3, #27210
	movt	r3, 3
	add	r3, r1, r3
	add	r2, r2, #1
	ldrsb	r3, [ip, r3]
	cmp	r3, #3
	addeq	r0, r0, #1
	cmp	r2, lr
	bne	.L137
	ldmfd	sp, {fp, sp, pc}
.L138:
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
	add	r0, r4, #56
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
	.global	FSP_PartitionFsMemory
	.type	FSP_PartitionFsMemory, %function
FSP_PartitionFsMemory:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 600
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #628)
	sub	sp, sp, #628
	ldr	r7, .L234
	mov	r5, r0
	mov	r8, r1
	mov	r9, r2
	sub	r0, fp, #608
	ldr	ip, [r7, #48]
	mov	r2, #564
	mov	r1, #0
	str	r3, [fp, #-632]
	blx	ip
	cmp	r5, #15
	bhi	.L223
	ldr	r3, .L234+4
	ldr	r4, [r3, r5, asl #2]
	cmp	r4, #0
	beq	.L224
	ldr	r6, .L234+8
	ldr	r0, [r6, r5, asl #2]
	add	r3, r0, #229376
	ldr	r10, [r3, #56]
	cmp	r10, #0
	beq	.L225
	ldr	ip, [r4, #64]
	ldr	r1, [r4, #68]
	mul	lr, r1, ip
.L163:
	ldr	r2, [r0, #144]
	ldr	r3, [r0, #148]
	mul	r10, r3, r2
	cmp	r10, lr
	blt	.L226
	ldr	r3, [r0, #1388]
	add	r0, r0, #225280
	ldrsb	r10, [r4, #60]
	cmp	r3, #1
	beq	.L227
.L166:
	ldr	r3, [r0, #2860]
	ldr	r2, .L234+12
	cmp	r10, r3
	ldr	ip, [r2, #48]
	movne	r3, #1
	moveq	r3, #0
	str	r3, [r0, #2880]
	ldrsb	r3, [r4, #60]
	cmp	ip, #0
	str	r3, [r0, #2860]
	beq	.L200
	ldr	r3, [r4, #68]
	sub	r0, fp, #608
	ldr	r2, [r4, #64]
	mov	lr, #40
	str	r0, [sp, #16]
	mov	r0, r8
	str	r5, [sp, #12]
	mov	r1, r9
	ldrsb	r8, [r4, #59]
	str	lr, [sp, #4]
	str	r8, [sp, #8]
	ldrsb	lr, [r4, #63]
	str	lr, [sp]
	blx	ip
	cmp	r0, #2
	mov	r8, r0
	beq	.L228
	cmp	r0, #0
	bne	.L170
	ldr	r3, [fp, #-316]
	ldr	r9, .L234+8
	strb	r3, [r4, #9]
	ldr	r3, [r6, r5, asl #2]
	ldr	r2, [r3, #1388]
	str	r0, [r3, #1428]
	cmp	r2, #1
	beq	.L229
.L174:
	ldr	r3, [r7, #68]
	mov	r0, #31
	ldr	r1, .L234+16
	blx	r3
	ldr	r3, [fp, #-316]
	ldr	r1, .L234
	cmp	r10, r3
	bgt	.L230
	ldrsb	r2, [r4, #1]
	mov	r3, #0
	strb	r3, [r4, #11]
	cmp	r2, r3
	strb	r3, [r4, #12]
	bne	.L231
.L180:
	ldr	r0, [fp, #-316]
	cmp	r0, #40
	movge	r0, #40
	uxtb	r0, r0
	strb	r0, [r4, #10]
.L181:
	ldr	r5, [fp, #-328]
	sub	ip, fp, #44
	add	r3, r4, #17408
	ldr	lr, [fp, #-608]
	ldr	r1, [ip, #-556]!
	mov	r2, #0
	str	r5, [fp, #-616]
	sxtb	r0, r0
	ldr	r5, [fp, #-324]
	add	r3, r3, #168
	ldr	r7, [r4, #64]
	ldr	r6, [r4, #68]
	str	r5, [fp, #-620]
	ldr	r10, [fp, #-344]
	ldr	r9, [fp, #-336]
	ldr	r8, [fp, #-332]
	ldr	r5, [fp, #-320]
	str	r1, [fp, #-624]
	mov	r1, r2
	str	r4, [fp, #-628]
.L184:
	cmp	r0, r2
	strgt	r2, [r3, #-12]
	add	r2, r2, #1
	strle	r1, [r3, #-12]
	ldrgt	r4, [ip]
	add	r3, r3, #64
	ldrle	r4, [fp, #-624]
	add	ip, ip, #4
	strgt	r4, [r3, #-68]
	ldrgt	r4, [ip, #124]
	str	lr, [r3, #-60]
	str	r10, [r3, #-56]
	str	r9, [r3, #-52]
	str	r8, [r3, #-48]
	str	r7, [r3, #-44]
	str	r6, [r3, #-40]
	strgt	r4, [r3, #-64]
	strle	r4, [r3, #-68]
	cmp	r2, #32
	ldr	r4, [fp, #-616]
	str	r4, [r3, #-36]
	ldr	r4, [fp, #-620]
	str	r5, [r3, #-28]
	strb	r1, [r3, #-80]
	str	r4, [r3, #-32]
	strb	r1, [r3, #-79]
	strb	r1, [r3, #-78]
	bne	.L184
	ldr	r4, [fp, #-628]
	mov	ip, #1
	mov	r5, #0
	add	r3, r4, #20480
	str	r3, [fp, #-640]
	strb	ip, [r4, #12]
	mov	r2, r5
	mov	r8, r3
	sub	r3, fp, #44
	add	r3, r3, r0, lsl #2
	ldr	r10, [fp, #-344]
	str	r5, [r8, #1180]
	ldr	r1, [r3, #-556]
	add	r3, r4, #16384
	str	lr, [r8, #1196]
	mov	r9, r3
	str	r3, [fp, #-636]
	str	r1, [r8, #1188]
	mov	r3, r5
	str	r10, [r9, #1200]
	mov	r1, r5
	ldr	r10, [fp, #-336]
	str	r10, [r9, #1204]
	ldr	r10, [fp, #-332]
	str	r10, [r9, #1208]
	str	r6, [r8, #1216]
	ldr	r6, [fp, #-328]
	str	r7, [r8, #1212]
	strb	r5, [r8, #1176]
	str	r6, [r8, #1220]
	ldr	r6, [fp, #-324]
	strb	r5, [r8, #1177]
	strb	r5, [r8, #1178]
	str	r6, [r8, #1224]
	ldr	r6, [fp, #-320]
	str	r6, [r8, #1228]
	strb	r5, [r4, #11]
	ldr	r5, [fp, #-328]
	ldr	r8, [fp, #-344]
	ldr	r7, [fp, #-336]
	str	r5, [fp, #-616]
	ldr	r5, [fp, #-324]
	ldr	r6, [fp, #-332]
	str	r5, [fp, #-620]
	ldr	r5, [fp, #-320]
	ldr	r9, [fp, #-608]
	str	r5, [fp, #-624]
	str	r9, [fp, #-628]
	b	.L188
.L232:
	add	r0, r2, r0
	add	r3, r4, r2, lsl #6
	sxtab	ip, r0, ip
	add	r9, r3, #19456
	sub	r10, fp, #44
	add	ip, r10, ip, lsl #2
	mov	r0, r9
	str	r2, [r9, #156]
.L220:
	ldr	ip, [ip, #-556]
	mov	r10, r0
	add	r2, r2, #1
	cmp	r2, #32
	str	ip, [r0, #164]
	add	ip, r3, #17408
	str	lr, [r10, #172]
	mov	r9, ip
	str	r8, [ip, #176]
	str	r7, [r9, #180]
	add	ip, r3, #19456
	str	r6, [r9, #184]
	ldr	lr, [r4, #80]
	mov	r0, ip
	mov	r10, ip
	str	lr, [ip, #188]
	ldr	r5, [r4, #84]
	str	r5, [r0, #192]
	ldr	r5, [fp, #-616]
	str	r5, [r10, #196]
	ldr	r5, [fp, #-620]
	str	r5, [r10, #200]
	ldr	r5, [fp, #-624]
	str	r5, [r10, #204]
	strb	r1, [ip, #152]
	strb	r1, [ip, #153]
	strb	r1, [ip, #154]
	beq	.L187
	ldrb	r3, [r4, #11]	@ zero_extendqisi2
	ldr	lr, [fp, #-628]
	ldrb	ip, [r4, #12]	@ zero_extendqisi2
	ldrsb	r0, [r4, #10]
.L188:
	sxtb	r3, r3
	cmp	r3, r2
	bgt	.L232
	add	r3, r4, r2, lsl #6
	sxtab	ip, r0, ip
	add	r9, r3, #19456
	sub	r5, fp, #44
	add	ip, r5, ip, lsl #2
	mov	r0, r9
	str	r1, [r9, #156]
	b	.L220
.L187:
	ldr	r3, [fp, #-56]
	add	r1, r4, #21760
	mov	r2, #0
	add	r1, r1, #28
	cmp	r3, #20
	sub	ip, fp, #180
	add	r0, r4, #14
	mov	lr, r2
	movge	r3, #20
	uxtb	r3, r3
	strb	r3, [r4, #13]
	b	.L192
.L233:
	ldrb	r3, [r4, #13]	@ zero_extendqisi2
.L192:
	sxtb	r3, r3
	add	ip, ip, #4
	cmp	r3, r2
	add	r2, r2, #1
	add	r1, r1, #4
	add	r0, r0, #1
	ldrgt	r3, [ip, #-4]
	ldrle	r3, [fp, #-180]
	cmp	r2, #20
	str	r3, [r1, #-4]
	strb	lr, [r0, #-1]
	bne	.L233
	ldr	r6, [fp, #-636]
	mov	r3, #0
	ldr	r2, [fp, #-60]
	ldr	r1, [fp, #-640]
	ldr	lr, [fp, #-608]
	ldr	ip, [fp, #-344]
	str	r2, [r1, #1304]
	ldr	r1, [fp, #-336]
	ldr	r2, [fp, #-332]
	ldr	r0, [r6, #1188]
	str	lr, [r6, #1132]
	str	ip, [r6, #1136]
	str	r0, [r6, #1124]
	str	r1, [r6, #1140]
	str	r2, [r6, #1144]
	str	r3, [r6, #1116]
	ldr	r2, [r4, #64]
	ldr	r5, [fp, #-328]
	ldr	lr, [fp, #-324]
	str	r2, [r6, #1148]
	ldr	ip, [fp, #-320]
	ldr	r1, [r4, #68]
	strb	r3, [r6, #1112]
	strb	r3, [r6, #1113]
	str	r1, [r6, #1152]
	strb	r3, [r6, #1114]
	str	r5, [r6, #1156]
	str	lr, [r6, #1160]
	str	ip, [r6, #1164]
	ldr	r3, [r4, #40]
	cmp	r2, r3
	beq	.L193
	ldrsb	r3, [r4, #10]
.L194:
	str	r3, [r4, #48]
	ldr	r3, [fp, #-636]
	str	r2, [r4, #40]
	str	r1, [r4, #44]
	ldr	r3, [r3, #1252]
	rsb	r3, r0, r3
	str	r3, [r4, #52]
.L196:
	ldr	r3, [fp, #-604]
	mov	r0, #0
	ldr	r2, [fp, #-632]
	str	r3, [r2]
.L214:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L225:
	mov	r0, r5
	bl	VCTRL_GetChanCapLevel
	cmp	r0, #32
	mov	r2, r0
	bhi	.L161
	ldr	lr, .L234+20
	mov	r3, r0, asl #5
	sub	r3, r3, r0, asl #2
	ldr	ip, [r4, #64]
	add	r0, lr, r3
	ldr	r1, [r4, #68]
	ldr	r3, [lr, r3]
	ldr	r0, [r0, #4]
	mul	lr, r1, ip
	str	r3, [fp, #-616]
	mul	r3, r0, r3
	cmp	r2, #25
	cmpne	lr, r3
	ble	.L218
	ldrsb	r3, [r4, #59]
	cmp	r3, #1
	bne	.L198
.L218:
	ldr	r0, [r6, r5, asl #2]
	b	.L163
.L226:
	str	r1, [sp, #4]
	mov	r0, #0
	str	ip, [sp]
	ldr	r1, .L234+24
	ldr	ip, [r7, #68]
	blx	ip
	ldr	r3, .L234+28
	ldr	ip, [r3]
	cmp	ip, #0
	mvneq	r0, #0
	beq	.L214
	mov	r3, #0
	mov	r0, r5
	mov	r2, r3
	mov	r1, #102
	blx	ip
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L228:
	ldr	r3, [fp, #-316]
	mov	r2, #1
	strb	r3, [r4, #9]
	ldr	r3, [r6, r5, asl #2]
	str	r2, [r3, #1428]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L227:
	ldr	r3, [r0, #2844]
	bics	r2, r3, #2
	beq	.L166
	cmp	r3, #1
	moveq	r0, #0
	beq	.L214
	mov	r3, #0
	strb	r3, [r4, #9]
	ldr	r3, [r6, r5, asl #2]
	mov	r0, #31
	ldr	ip, [r7, #68]
	add	r3, r3, #225280
	ldr	r1, .L234+32
	ldr	r2, [r3, #2844]
	blx	ip
	mvn	r0, #0
	b	.L214
.L200:
	mvn	r8, #0
.L170:
	mov	r3, #0
	strb	r3, [r4, #9]
	ldr	lr, [r6, r5, asl #2]
	mov	r2, r8
	ldr	ip, [r7, #68]
	mov	r0, r3
	ldr	r1, .L234+36
	str	r3, [lr, #1428]
	blx	ip
	mvn	r0, #0
	b	.L214
.L231:
	ldr	r3, [r6, r5, asl #2]
	ldr	r3, [r3, #1388]
	cmp	r3, #1
	beq	.L180
	ldrsb	r0, [r4, #60]
	ldr	r3, [fp, #-316]
	add	r0, r0, #4
	cmp	r0, r3
	movge	r0, r3
	uxtb	r0, r0
	strb	r0, [r4, #10]
	b	.L181
.L161:
	ldrsb	r3, [r4, #59]
	cmp	r3, #1
	bne	.L197
	ldr	ip, [r4, #64]
	ldr	r1, [r4, #68]
	ldr	r0, [r6, r5, asl #2]
	mul	lr, r1, ip
	b	.L163
.L193:
	ldr	r3, [r4, #44]
	cmp	r1, r3
	ldrsb	r3, [r4, #10]
	bne	.L194
	ldr	ip, [r4, #48]
	cmp	r3, ip
	bne	.L194
	b	.L196
.L229:
	ldr	r3, [r7]
	blx	r3
	ldr	r3, [r9, r5, asl #2]
	add	r3, r3, #229376
	ldmib	r3, {r1, r2}
	ldr	ip, [r3, #12]
	cmp	r1, r2
	rsb	lr, r1, r0
	rsbls	r0, r1, r2
	ldr	r2, .L234+40
	movhi	r0, r8
	cmp	r1, ip
	str	lr, [r3, #24]
	rsbls	r1, r1, ip
	str	r0, [r3, #16]
	movhi	r1, #0
	str	r1, [r3, #20]
	ldr	r3, [r2]
	cmp	lr, r3
	strhi	lr, [r2]
	b	.L174
.L223:
	ldr	ip, [r7, #68]
	movw	r2, #643
	ldr	r3, .L234+44
	mov	r0, #0
	ldr	r1, .L234+48
	blx	ip
	mvn	r0, #0
	b	.L214
.L197:
	ldr	r3, [r7, #68]
	mov	r0, r10
	ldr	r1, .L234+52
	blx	r3
.L164:
	mov	r3, #0
	mvn	r0, #0
	strb	r3, [r4, #9]
	b	.L214
.L198:
	str	r1, [sp, #8]
	stmia	sp, {r0, ip}
	mov	r0, r10
	ldr	r3, [fp, #-616]
	ldr	ip, [r7, #68]
	ldr	r1, .L234+56
	blx	ip
	b	.L164
.L230:
	mov	r2, #0
	str	r3, [sp]
	mov	r0, r2
	ldr	ip, [r1, #68]
	mov	r3, r10
	ldr	r1, .L234+60
	blx	ip
	mvn	r0, #0
	b	.L214
.L224:
	ldr	ip, [r7, #68]
	mov	r0, r4
	ldr	r3, .L234+64
	mov	r2, #644
	ldr	r1, .L234+48
	blx	ip
	mvn	r0, #0
	b	.L214
.L235:
	.align	2
.L234:
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
	.word	g_u32DFSMaxAllocTime
	.word	.LC0
	.word	.LC1
	.word	.LC9
	.word	.LC10
	.word	.LC15
	.word	.LC4
	UNWIND(.fnend)
	.size	FSP_PartitionFsMemory, .-FSP_PartitionFsMemory
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
	bhi	.L247
	cmp	r1, #0
	beq	.L248
	cmp	r2, #0
	beq	.L249
	ldr	r3, .L251
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L250
	ldrsb	r3, [r0, #10]
	str	r3, [r2]
	ldrsb	r3, [r0, #10]
	cmp	r3, #0
	ldmlefd	sp, {fp, sp, pc}
	sub	r1, r1, #4
	mov	r2, #0
.L243:
	mov	r3, r2, asl #6
	add	r2, r2, #1
	add	r3, r3, #17408
	add	r3, r3, #164
	ldr	r3, [r0, r3]
	str	r3, [r1, #4]!
	ldrsb	r3, [r0, #10]
	cmp	r3, r2
	bgt	.L243
	ldmfd	sp, {fp, sp, pc}
.L247:
	ldr	ip, .L251+4
	movw	r2, #1276
	ldr	r3, .L251+8
	mov	r0, #0
.L246:
	ldr	r1, .L251+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L248:
	ldr	ip, .L251+4
	mov	r0, r1
	ldr	r3, .L251+16
	movw	r2, #1277
	b	.L246
.L249:
	mov	r0, r2
	ldr	ip, .L251+4
	ldr	r3, .L251+20
	movw	r2, #1278
	b	.L246
.L250:
	ldr	ip, .L251+4
	movw	r2, #1279
	ldr	r3, .L251+24
	b	.L246
.L252:
	.align	2
.L251:
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
	bhi	.L264
	cmp	r1, #0
	beq	.L265
	cmp	r2, #0
	beq	.L266
	ldr	r3, .L268
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L267
	ldrsb	r3, [r0, #13]
	str	r3, [r2]
	ldrsb	r3, [r0, #13]
	cmp	r3, #0
	ldmlefd	sp, {fp, sp, pc}
	add	r2, r0, #21760
	sub	r1, r1, #4
	add	r2, r2, #24
	mov	r3, #0
.L260:
	ldr	ip, [r2, #4]!
	add	r3, r3, #1
	str	ip, [r1, #4]!
	ldrsb	ip, [r0, #13]
	cmp	ip, r3
	bgt	.L260
	ldmfd	sp, {fp, sp, pc}
.L264:
	ldr	ip, .L268+4
	movw	r2, #1306
	ldr	r3, .L268+8
	mov	r0, #0
.L263:
	ldr	r1, .L268+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L265:
	ldr	ip, .L268+4
	mov	r0, r1
	ldr	r3, .L268+16
	movw	r2, #1307
	b	.L263
.L266:
	mov	r0, r2
	ldr	ip, .L268+4
	ldr	r3, .L268+20
	movw	r2, #1308
	b	.L263
.L267:
	ldr	ip, .L268+4
	movw	r2, #1309
	ldr	r3, .L268+24
	b	.L263
.L269:
	.align	2
.L268:
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
	ble	.L274
	mvn	lr, #0
	mov	r3, r0
.L273:
	ldr	ip, [r2, #56]
	add	r2, r2, #64
	cmp	ip, lr
	movcc	r0, r3
	add	r3, r3, #1
	movcc	lr, ip
	cmp	r3, r1
	bne	.L273
	ldmfd	sp, {fp, sp, pc}
.L274:
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
	ldr	lr, .L289
	ldr	r2, [lr, r0, asl #2]
	cmp	r2, #0
	beq	.L287
	mov	r3, r0, asl #8
	add	r3, r3, r0, lsl #3
	add	r3, lr, r3
	add	r3, r3, #64
	ldrsh	r4, [r3, #2]
	cmn	r4, #1
	beq	.L284
	ldrh	r3, [r3, #4]
	cmp	r3, r4
	beq	.L284
	ldr	r6, [r2, #4]
	add	ip, r2, #16384
	ldr	r5, [r2, #52]
	cmp	r6, #1
	ldr	r2, [ip, #1252]
	ldr	r6, [ip, #1188]
	beq	.L288
.L284:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L288:
	and	r3, r3, #63
	cmp	r4, r3
	beq	.L284
	rsb	r6, r6, r2
	mov	r2, r0, asl #6
	add	r6, r6, r1
	add	r0, r2, r0, lsl #1
.L280:
	add	r2, r0, r3
	add	r3, r3, #1
	and	r3, r3, #63
	add	r2, lr, r2, lsl #2
	ldr	r2, [r2, #72]
	add	ip, r2, r5
	cmp	ip, r1
	bcc	.L279
	cmp	r2, r6
	bls	.L285
.L279:
	cmp	r4, r3
	bne	.L280
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L285:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L287:
	ldr	ip, .L289+4
	mov	r0, r2
	ldr	r3, .L289+8
	movw	r2, #1574
	ldr	r1, .L289+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L290:
	.align	2
.L289:
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
	bhi	.L347
	ldr	r4, .L357
	ldr	r0, [r4, r0, asl #2]
	cmp	r0, #0
	beq	.L348
	ldr	r7, .L357+4
	mov	r0, #3
	ldr	r3, [r7, #12]
	blx	r3
	ldr	r5, [r4, r8, asl #2]
	mov	r2, #0
	mov	r3, r5
.L297:
	ldrsb	r1, [r3, #88]
	cmp	r1, #0
	bne	.L295
	ldrsb	r0, [r3, #89]
	cmp	r0, #0
	beq	.L349
.L295:
	add	r2, r2, #1
	add	r3, r3, #544
	cmp	r2, #32
	bne	.L297
.L298:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldr	r1, .L357+8
	blx	r3
	ldr	r3, [r7, #16]
	mov	r0, #3
	blx	r3
	mvn	r0, #0
.L339:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L349:
	mov	r6, r2, asl #5
	mov	r9, r2, asl #9
	add	r4, r6, r9
	add	r3, r5, r4
	add	r4, r4, #88
	add	ip, r3, #88
	adds	r4, r5, r4
	strb	r2, [ip, #3]
	strb	r0, [ip, #2]
	beq	.L298
	add	r1, r3, #608
	add	lr, r3, #616
	cmp	r10, #0
	str	r0, [r3, #608]
	str	r0, [r1, #4]
	str	r0, [r3, #616]
	str	r0, [lr, #4]
	beq	.L299
	ldrsb	r1, [r5, #10]
	cmp	r1, #0
	addgt	r3, r5, #17408
	addgt	r3, r3, #152
	ble	.L343
.L304:
	ldrsb	r2, [r3]
	cmp	r2, #0
	bne	.L303
	ldrsb	r2, [r3, #2]
	cmp	r2, #0
	beq	.L350
.L303:
	add	r0, r0, #1
	add	r3, r3, #64
	cmp	r0, r1
	bne	.L304
	add	r3, r6, r9
	add	r3, r5, r3
.L343:
	ldr	r3, [r3, #608]
.L301:
	cmp	r3, #0
	beq	.L351
.L305:
	add	r3, r6, r9
	add	r2, r5, #21504
	add	r3, r5, r3
	add	r2, r2, #152
	add	r0, r3, #616
	str	r2, [r3, #616]
	ldrsb	r1, [r5, #13]
	cmp	r1, #0
	ble	.L315
	ldrsb	r2, [r5, #14]
	cmp	r2, #0
	addne	r2, r5, #14
	movne	r3, #0
	bne	.L319
	b	.L352
.L322:
	ldrsb	r0, [r2, #1]!
	cmp	r0, #0
	beq	.L353
.L319:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L322
	add	r2, r5, #20480
	cmp	r1, #0
	ldr	ip, [r2, #1304]
	ble	.L324
.L320:
	add	r1, r1, #13
	add	r3, r5, #13
	add	r1, r5, r1
	mov	r0, #0
.L323:
	strb	r0, [r3, #1]!
	cmp	r3, r1
	bne	.L323
.L324:
	add	r3, r6, r9
	mov	lr, #0
	add	r3, r5, r3
	mov	r0, lr
	ldr	r1, .L357+12
	strb	lr, [r3, #92]
	ldr	r2, [r2, #1308]
	str	ip, [r3, #624]
	str	r2, [r3, #620]
	ldr	r3, [r7, #68]
	blx	r3
.L315:
	ldr	r1, .L357+16
	add	r10, r6, r9
	add	r10, r5, r10
	ldr	r2, [r1, #-3896]
	ldr	r3, [r10, #608]
	cmp	r2, #0
	str	r3, [r10, #612]
	beq	.L302
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
	beq	.L354
	mov	r3, r0
	sub	r1, fp, #72
	mov	r0, r8
	blx	ip
	cmp	r0, #0
	bne	.L326
	ldr	r8, [fp, #-72]
	cmp	r8, #0
	beq	.L355
	ldr	r3, [r10, #608]
	str	r8, [r3, #12]
	ldrsb	r3, [r10, #92]
	ldr	r2, [fp, #-56]
	add	r3, r3, #5440
	add	r3, r3, #6
	str	r2, [r10, #620]
	add	r3, r5, r3, lsl #2
	str	r2, [r3, #4]
	b	.L302
.L299:
	add	r0, r5, #20480
	add	r2, r5, #17408
	add	r2, r2, #88
	str	r2, [r3, #608]
	str	r2, [r1, #4]
	mov	r1, #1
	str	r2, [r3, #616]
	ldr	r3, [r0, #1308]
	str	r3, [lr, #4]
	ldrb	r3, [r5, #13]	@ zero_extendqisi2
	strb	r1, [ip, #2]
	sub	r3, r3, #1
	strb	r3, [ip, #4]
.L302:
	add	r6, r6, r9
	mov	lr, #1
	add	r3, r5, r6
	add	r0, r6, #96
	add	r0, r5, r0
	add	r8, r3, #88
	ldr	r6, [r3, #612]
	mov	r1, #0
	strb	lr, [r3, #89]
	mov	r2, #504
	strb	lr, [r6, #2]
	str	lr, [r3, #604]
	ldr	r6, [r3, #608]
	ldr	lr, [r5, #36]
	str	lr, [r3, #600]
	str	lr, [r6, #56]
	ldr	lr, [r3, #612]
	ldr	r6, [r3, #600]
	str	r6, [lr, #56]
	ldr	lr, [r5, #36]
	add	lr, lr, #1
	str	lr, [r5, #36]
	add	r5, r3, #608
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
.L350:
	add	r2, r6, r9
	add	r0, r5, r0, lsl #6
	add	r2, r5, r2
	add	r3, r0, #17408
	add	r3, r3, #152
	str	r3, [r2, #608]
	b	.L301
.L353:
	add	ip, r3, #5440
	add	r0, r6, r9
	add	ip, ip, #6
	add	r0, r5, r0
	cmp	r3, r1
	add	r2, r5, #20480
	add	ip, r5, ip, lsl #2
	strb	r3, [r0, #92]
	ldr	r3, [ip, #4]
	str	r3, [r0, #620]
	ldr	ip, [r2, #1304]
	str	ip, [r0, #624]
	blt	.L315
	b	.L320
.L351:
	mov	r0, r3
	ldr	r2, .L357+20
	ldr	ip, [r7, #68]
	ldr	r1, .L357+24
	str	r3, [fp, #-80]
	blx	ip
	ldrsb	r2, [r5, #10]
	ldr	r3, [fp, #-80]
	cmp	r2, #0
	ble	.L310
	add	r10, r5, #17408
	str	r4, [fp, #-80]
	add	r10, r10, #152
	mov	r4, r3
.L309:
	ldrsb	r3, [r10, #2]
	mov	r0, #0
	ldr	ip, [r7, #68]
	add	r4, r4, #1
	ldrsb	r2, [r10], #64
	ldr	r1, .L357+28
	blx	ip
	ldrsb	r3, [r5, #10]
	cmp	r3, r4
	bgt	.L309
	ldr	r4, [fp, #-80]
.L310:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldr	r1, .L357+32
	blx	r3
	ldrsb	r3, [r5, #9]
	ldr	r10, .L357+4
	cmp	r3, #0
	ble	.L356
	ldrsb	ip, [r5, #10]
	add	r3, r5, #17408
	add	r3, r3, #152
	cmp	ip, #0
	ble	.L311
	mov	lr, #0
	mvn	r0, #0
	mov	r2, lr
.L313:
	ldr	r1, [r3, #56]
	add	r3, r3, #64
	cmp	r1, r0
	movcc	lr, r2
	add	r2, r2, #1
	movcc	r0, r1
	cmp	ip, r2
	bne	.L313
	add	r3, r5, lr, lsl #6
	add	r3, r3, #17408
	add	r3, r3, #152
.L311:
	add	r2, r6, r9
	add	r2, r5, r2
	str	r3, [r2, #608]
	b	.L305
.L354:
	mov	r0, ip
	ldr	r3, [r7, #68]
	ldr	r1, .L357+36
	blx	r3
.L326:
	ldr	r3, [r7, #16]
	mov	r0, #3
	blx	r3
	ldr	r3, [r7, #68]
	ldr	r1, .L357+40
	mov	r0, #0
	blx	r3
	mvn	r0, #0
	b	.L339
.L347:
	ldr	ip, .L357+4
	movw	r2, #1636
	ldr	r3, .L357+44
	mov	r0, #0
	ldr	r1, .L357+48
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L339
.L352:
	add	r1, r5, #20480
	strb	r2, [r3, #92]
	ldr	r2, [r1, #1308]
	str	r2, [r0, #4]
	ldr	r2, [r1, #1304]
	str	r2, [r3, #624]
	b	.L315
.L348:
	ldr	ip, .L357+4
	movw	r2, #1637
	ldr	r3, .L357+52
	ldr	r1, .L357+48
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L339
.L355:
	ldr	r3, [r7, #16]
	mov	r0, #3
	blx	r3
	ldr	r3, [r7, #68]
	mov	r0, r8
	ldr	r1, .L357+56
	blx	r3
	mvn	r0, #0
	b	.L339
.L356:
	ldr	r3, [r10, #68]
	mov	r0, #0
	ldr	r2, .L357+20
	ldr	r1, .L357+60
	blx	r3
	ldr	r3, [r10, #16]
	mov	r0, #3
	blx	r3
	mvn	r0, #0
	b	.L339
.L358:
	.align	2
.L357:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC18
	.word	.LC23
	.word	.LANCHOR1
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
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #15
	bhi	.L366
	cmp	r1, #31
	bhi	.L367
	ldr	r3, .L369
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L368
	mov	r2, r1, asl #5
	mov	r1, r1, asl #9
	add	r3, r2, r1
	add	r3, r0, r3
	ldr	ip, [r3, #604]
	cmp	ip, #2
	ldmnefd	sp, {fp, sp, pc}
	ldrb	r3, [r3, #92]	@ zero_extendqisi2
	add	r1, r2, r1
	cmp	r3, #19
	sxtabls	r3, r0, r3
	add	r0, r0, r1
	movls	ip, #0
	strlsb	ip, [r3, #14]
	mov	r3, #0
	str	r3, [r0, #604]
	ldmfd	sp, {fp, sp, pc}
.L366:
	ldr	ip, .L369+4
	movw	r2, #2006
	ldr	r3, .L369+8
	mov	r0, #0
.L365:
	ldr	r1, .L369+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L367:
	ldr	ip, .L369+4
	movw	r2, #2007
	ldr	r3, .L369+16
	mov	r0, #0
	b	.L365
.L368:
	ldr	ip, .L369+4
	movw	r2, #2008
	ldr	r3, .L369+20
	b	.L365
.L370:
	.align	2
.L369:
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
	bhi	.L403
	cmp	r1, #31
	bhi	.L404
	ldr	ip, .L414
	ldr	r7, [ip, r0, asl #2]
	cmp	r7, #0
	beq	.L405
	mov	r8, r1, asl #5
	mov	r5, r1, asl #9
	add	r4, r8, r5
	uxtb	r9, r2
	add	r4, r7, r4
	ldrsb	r3, [r4, #90]
	strb	r9, [r4, #88]
	cmp	r3, #0
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r3, [r4, #608]
	cmp	r3, #0
	beq	.L377
	ldr	ip, [r3, #56]
	ldr	r1, [r4, #600]
	cmp	ip, r1
	beq	.L406
.L377:
	add	r3, r8, r5
	add	r3, r7, r3
	ldrb	r2, [r3, #92]	@ zero_extendqisi2
	cmp	r2, #19
	bhi	.L380
	cmp	r6, #1
	beq	.L407
	cmp	r6, #0
	bne	.L383
	ldr	r0, [r3, #604]
	cmp	r0, #2
	beq	.L408
.L390:
	add	r3, r8, r5
	add	r3, r7, r3
	ldr	r3, [r3, #612]
	cmp	r3, #0
	beq	.L394
	ldrsb	r2, [r3, #2]
	cmp	r2, #1
	moveq	r2, #0
	streqb	r2, [r3, #2]
.L394:
	add	r1, r8, r5
	add	r7, r7, r1
	str	r6, [r7, #608]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L403:
	ldr	ip, .L414+4
	movw	r2, #2040
	ldr	r3, .L414+8
	mov	r0, #0
.L402:
	ldr	r1, .L414+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	ip
.L407:
	ldr	r1, [r3, #604]
	add	r3, r3, #600
	cmp	r1, #1
	beq	.L409
.L382:
	add	r3, r8, r5
	add	r3, r7, r3
	ldr	r2, [r3, #612]
	cmp	r2, #0
	beq	.L410
.L389:
	add	r3, r8, r5
	ldr	r0, [r2, #56]
	add	r3, r7, r3
	ldr	r1, [r3, #600]
	cmp	r6, #1
	cmpeq	r0, r1
	beq	.L411
.L386:
	cmp	r6, #0
	beq	.L394
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L404:
	ldr	ip, .L414+4
	movw	r2, #2041
	ldr	r3, .L414+16
	mov	r0, #0
	b	.L402
.L380:
	ldr	r3, .L414+4
	sxtb	r2, r2
	ldr	r1, .L414+20
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	cmp	r6, #0
	beq	.L390
	b	.L382
.L406:
	ldrsb	r1, [r3]
	cmp	r2, r1
	beq	.L378
	ldr	r1, .L414+24
	ldr	ip, [r1, #-3896]
	cmp	ip, #0
	bne	.L412
.L378:
	strb	r9, [r3]
	b	.L377
.L409:
	sxtab	r2, r7, r2
	mov	r1, #2
	strb	r6, [r2, #14]
	str	r1, [r3, #4]
.L383:
	add	r3, r8, r5
	add	r3, r7, r3
	ldr	r2, [r3, #612]
	cmp	r2, #0
	bne	.L389
	b	.L386
.L412:
	ldr	ip, [r1, #-3880]
	ldr	r1, [r3, #12]
	cmp	ip, #0
	beq	.L413
	blx	ip
	ldr	r3, [r4, #608]
	b	.L378
.L408:
	sxtab	r2, r7, r2
	strb	r6, [r2, #14]
	str	r6, [r3, #620]
	str	r6, [r3, #604]
	b	.L390
.L405:
	ldr	ip, .L414+4
	mov	r0, r7
	ldr	r3, .L414+28
	movw	r2, #2042
	b	.L402
.L411:
	ldrsb	r1, [r2, #2]
	cmp	r1, #1
	moveq	r1, #2
	streqb	r1, [r2, #2]
	streqb	r1, [r3, #89]
	b	.L386
.L410:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L413:
	ldr	r3, .L414+4
	mov	r0, ip
	ldr	r1, .L414+32
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [r4, #608]
	b	.L378
.L415:
	.align	2
.L414:
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
	bhi	.L422
	cmp	r1, #31
	bhi	.L423
	ldr	r3, .L425
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L424
	mov	r0, r1, asl #9
	add	r1, r0, r1, lsl #5
	add	r3, r3, r1
	ldrsb	r0, [r3, #88]
	ldmfd	sp, {fp, sp, pc}
.L422:
	ldr	ip, .L425+4
	movw	r2, #2161
	ldr	r3, .L425+8
.L421:
	ldr	r1, .L425+12
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L423:
	ldr	ip, .L425+4
	movw	r2, #2162
	ldr	r3, .L425+16
	b	.L421
.L424:
	ldr	ip, .L425+4
	mov	r0, r3
	movw	r2, #2163
	ldr	r3, .L425+20
	ldr	r1, .L425+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L426:
	.align	2
.L425:
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
	bhi	.L454
	cmp	r1, #31
	bhi	.L455
	ldr	r4, .L460
	ldr	r0, [r4, r0, asl #2]
	cmp	r0, #0
	beq	.L456
	ldr	r8, .L460+4
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
	add	r2, r3, #88
	ldrsb	r1, [r3, #90]
	cmp	r1, #0
	cmpeq	r10, #0
	bne	.L431
	ldr	r3, [r3, #612]
	cmp	r3, #0
	beq	.L431
	ldrsb	r2, [r2, #1]
	cmp	r2, #0
	beq	.L431
	ldr	r2, .L460+8
	ldr	r1, [r2, #-3896]
	cmp	r1, #0
	bne	.L457
.L431:
	add	r1, r7, r5
	cmp	r9, #0
	add	r1, r4, r1
	add	ip, r1, #88
	movne	r3, #0
	ldrsb	r0, [ip, #2]
	moveq	r3, #3
	strb	r3, [ip, #1]
	cmp	r0, #0
	bne	.L438
	add	lr, r1, #608
	ldr	r3, [lr, #4]
	cmp	r3, #0
	beq	.L438
	ldr	r3, [r3, #56]
	ldr	r2, [r1, #600]
	cmp	r3, r2
	bne	.L458
	cmp	r9, #0
	strneb	r0, [ip, #1]
	moveq	r3, #3
	ldrne	r3, [lr, #4]
	streqb	r3, [ip, #1]
	ldreq	r2, [lr, #4]
	strneb	r0, [r3, #2]
	strne	r0, [lr, #4]
	streqb	r3, [r2, #2]
.L438:
	ldr	r3, [r8, #16]
	mov	r0, #3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	r3
.L454:
	ldr	ip, .L460+4
	movw	r2, #2180
	ldr	r3, .L460+12
.L452:
	mov	r0, #0
.L453:
	ldr	r1, .L460+16
	ldr	ip, [ip, #68]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	ip
.L455:
	ldr	ip, .L460+4
	movw	r2, #2181
	ldr	r3, .L460+20
	b	.L452
.L458:
	ldr	ip, [r8, #68]
	ldr	r1, .L460+24
	blx	ip
	b	.L438
.L456:
	ldr	ip, .L460+4
	movw	r2, #2182
	ldr	r3, .L460+28
	b	.L453
.L457:
	ldr	r2, [r2, #-3884]
	ldr	r1, [r3, #12]
	cmp	r2, #0
	beq	.L459
	mov	r0, r6
	blx	r2
	b	.L431
.L459:
	mov	r0, r2
	ldr	r3, [r8, #68]
	ldr	r1, .L460+32
	blx	r3
	b	.L431
.L461:
	.align	2
.L460:
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
	bhi	.L473
	cmp	r1, #0
	beq	.L474
	ldr	r3, .L477
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	addne	r6, r0, #17408
	addne	r4, r0, #96
	addne	r6, r6, #96
	bne	.L468
	b	.L475
.L467:
	add	r4, r4, #544
	cmp	r4, r6
	beq	.L476
.L468:
	mov	r2, #0
	ldr	r1, [r4, #196]
	mov	r0, r5
	bl	FSP_SetRef
	mov	r1, r4
	mov	r0, r7
	bl	IsImageInVoQueue
	cmp	r0, #0
	beq	.L467
	ldr	r1, [r4, #196]
	mov	r2, #0
	mov	r0, r5
	add	r4, r4, #544
	bl	FSP_SetDisplay
	cmp	r4, r6
	bne	.L468
.L476:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L473:
	ldr	ip, .L477+4
	movw	r2, #578
	ldr	r3, .L477+8
	mov	r0, #0
	ldr	r1, .L477+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L475:
	ldr	ip, .L477+4
	mov	r2, #580
	ldr	r3, .L477+16
	ldr	r1, .L477+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L474:
	ldr	ip, .L477+4
	mov	r0, r1
	ldr	r3, .L477+20
	movw	r2, #579
	ldr	r1, .L477+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L478:
	.align	2
.L477:
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
	ldr	r3, .L483
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L482
	mov	r2, r1, asl #9
	add	r1, r2, r1, lsl #5
	add	r3, r0, r1
	ldrsb	r0, [r3, #89]
	ldmfd	sp, {fp, sp, pc}
.L482:
	ldr	ip, .L483+4
	movw	r2, #2249
	ldr	r3, .L483+8
	ldr	r1, .L483+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L484:
	.align	2
.L483:
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
	bhi	.L493
	ldr	ip, .L496
	ldr	r0, [ip, r0, asl #2]
	cmp	r0, #0
	beq	.L494
	cmp	r1, #0
	bne	.L489
	ldrsb	ip, [r0, #10]
	cmp	ip, r2
	ble	.L492
	add	r2, r0, r2, lsl #6
	movw	ip, #17568
	mov	r0, r1
	str	r3, [r2, ip]
	ldmfd	sp, {fp, sp, pc}
.L489:
	cmp	r1, #1
	beq	.L495
.L492:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L495:
	ldrsb	r1, [r0, #11]
	cmp	r2, r1
	bge	.L492
	add	r2, r0, r2, lsl #6
	movw	r1, #19616
	mov	r0, #0
	str	r3, [r2, r1]
	ldmfd	sp, {fp, sp, pc}
.L493:
	ldr	ip, .L496+4
	movw	r2, #2265
	ldr	r3, .L496+8
	mov	r0, #0
	ldr	r1, .L496+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L494:
	ldr	ip, .L496+4
	movw	r2, #2266
	ldr	r3, .L496+16
	ldr	r1, .L496+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L497:
	.align	2
.L496:
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
	bhi	.L506
	ldr	r3, .L509
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L507
	cmp	r1, #0
	bne	.L502
	ldrsb	r3, [r0, #10]
	cmp	r3, r2
	ble	.L505
	add	r0, r0, r2, lsl #6
	movw	r3, #17568
	ldr	r0, [r0, r3]
	ldmfd	sp, {fp, sp, pc}
.L502:
	cmp	r1, #1
	beq	.L508
.L505:
	mov	r0, #2
	ldmfd	sp, {fp, sp, pc}
.L506:
	ldr	ip, .L509+4
	movw	r2, #2306
	ldr	r3, .L509+8
	mov	r0, #0
	ldr	r1, .L509+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L507:
	ldr	ip, .L509+4
	movw	r2, #2307
	ldr	r3, .L509+16
	ldr	r1, .L509+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L508:
	ldrsb	r3, [r0, #11]
	cmp	r2, r3
	bge	.L505
	add	r0, r0, r2, lsl #6
	movw	r3, #19616
	ldr	r0, [r0, r3]
	ldmfd	sp, {fp, sp, pc}
.L510:
	.align	2
.L509:
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
	ldr	r6, .L521
	ldr	r3, [r6, r0, asl #2]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r2, #0
	bl	FSP_SetRef
	ldr	r2, [r6, r4, asl #2]
	mov	r3, r5, asl #9
	add	r3, r3, r5, lsl #5
	add	r3, r2, r3
	ldrsb	r3, [r3, #89]
	cmp	r3, #3
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r1, r5
	mov	r0, r4
	mov	r2, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	FSP_SetDisplay
.L522:
	.align	2
.L521:
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
	bhi	.L526
	ldr	r3, .L527
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L526
	mov	r2, r1, asl #9
	add	r0, r2, r1, lsl #5
	add	r0, r0, #88
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L526:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L528:
	.align	2
.L527:
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
	bhi	.L532
	ldr	r3, .L533
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L532
	mov	r2, r1, asl #9
	add	r0, r2, r1, lsl #5
	add	r0, r0, #96
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L532:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L534:
	.align	2
.L533:
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
	bhi	.L537
	ldr	r3, .L539
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, r1, asl #9
	addne	r1, r3, r1, lsl #5
	addne	r0, r0, r1
	ldrne	r0, [r0, #608]
	ldmfd	sp, {fp, sp, pc}
.L537:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L540:
	.align	2
.L539:
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
	bhi	.L543
	ldr	r3, .L545
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, r1, asl #9
	addne	r1, r3, r1, lsl #5
	addne	r0, r0, r1
	ldrne	r0, [r0, #612]
	ldmfd	sp, {fp, sp, pc}
.L543:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L546:
	.align	2
.L545:
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
	bhi	.L549
	ldr	r3, .L551
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, r1, asl #9
	addne	r1, r3, r1, lsl #5
	addne	r0, r0, r1
	ldrne	r0, [r0, #620]
	ldmfd	sp, {fp, sp, pc}
.L549:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L552:
	.align	2
.L551:
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
	bhi	.L564
	cmp	r2, #0
	beq	.L565
	cmp	r1, #0
	beq	.L566
	ldr	ip, .L568
	ldr	lr, [ip, r0, asl #2]
	cmp	lr, #0
	beq	.L567
	mov	lr, #0
	str	lr, [r1]
	ldr	r4, [ip, r0, asl #2]
	ldrsb	r4, [r4, #10]
	cmp	r4, lr
	str	r4, [r1]
	ldr	ip, [ip, r0, asl #2]
	add	ip, ip, #17408
	add	ip, ip, #152
	ldmlefd	sp, {r4, fp, sp, pc}
	sub	r2, r2, #4
	sub	r3, r3, #4
.L560:
	ldr	r0, [ip, #12]
	add	lr, lr, #1
	add	ip, ip, #64
	str	r0, [r2, #4]!
	ldr	r0, [ip, #-48]
	str	r0, [r3, #4]!
	ldr	r4, [r1]
	cmp	r4, lr
	bgt	.L560
	ldmfd	sp, {r4, fp, sp, pc}
.L564:
	ldr	ip, .L568+4
	movw	r2, #2465
	ldr	r3, .L568+8
	mov	r0, #0
.L563:
	ldr	r1, .L568+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	bx	ip
.L565:
	mov	r0, r2
	ldr	ip, .L568+4
	ldr	r3, .L568+16
	movw	r2, #2466
	b	.L563
.L566:
	ldr	ip, .L568+4
	mov	r0, r1
	ldr	r3, .L568+20
	movw	r2, #2467
	b	.L563
.L567:
	ldr	ip, .L568+4
	mov	r0, lr
	ldr	r3, .L568+24
	movw	r2, #2468
	b	.L563
.L569:
	.align	2
.L568:
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
	bhi	.L581
	cmp	r2, #0
	beq	.L582
	cmp	r1, #0
	beq	.L583
	ldr	ip, .L585
	ldr	r3, [ip, r0, asl #2]
	cmp	r3, #0
	beq	.L584
	mov	r3, #0
	str	r3, [r1]
	ldr	ip, [ip, r0, asl #2]
	ldrsb	r0, [ip, #13]
	str	r0, [r1]
	ldrsb	r1, [ip, #13]
	cmp	r1, r3
	ldmlefd	sp, {fp, sp, pc}
	add	r1, ip, #21760
	sub	r2, r2, #4
	add	r1, r1, #24
.L577:
	ldr	r0, [r1, #4]!
	add	r3, r3, #1
	str	r0, [r2, #4]!
	ldrsb	r0, [ip, #13]
	cmp	r0, r3
	bgt	.L577
	ldmfd	sp, {fp, sp, pc}
.L581:
	ldr	ip, .L585+4
	movw	r2, #2510
	ldr	r3, .L585+8
	mov	r0, #0
.L580:
	ldr	r1, .L585+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L582:
	mov	r0, r2
	ldr	ip, .L585+4
	ldr	r3, .L585+16
	movw	r2, #2511
	b	.L580
.L583:
	ldr	ip, .L585+4
	mov	r0, r1
	ldr	r3, .L585+20
	mov	r2, #2512
	b	.L580
.L584:
	mov	r0, r3
	ldr	ip, .L585+4
	ldr	r3, .L585+24
	movw	r2, #2513
	b	.L580
.L586:
	.align	2
.L585:
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
	ldr	r3, .L590
	mov	ip, r1
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L589
	ldrb	lr, [r3, #10]	@ zero_extendqisi2
	add	r4, r3, #16384
	mov	r0, r1
	ldr	r5, .L590+4
	add	r1, r3, #17408
	mov	r2, #2048
	strb	lr, [ip, #1]
	add	r1, r1, #152
	ldr	lr, [r4, #1252]
	ldr	r4, [r4, #1188]
	rsb	lr, r4, lr
	str	lr, [ip, #2056]
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
.L589:
	ldr	ip, .L590+4
	mov	r0, r3
	movw	r2, #2535
	ldr	r3, .L590+8
	ldr	r1, .L590+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	ip
.L591:
	.align	2
.L590:
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
	bhi	.L603
	ldr	r3, .L605
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	movne	r3, r0
	movne	r0, #0
	beq	.L604
.L597:
	ldr	r2, [r3, #608]
	add	r3, r3, #544
	cmp	r2, #0
	beq	.L596
	ldr	r2, [r2, #12]
	cmp	r2, r1
	ldmeqfd	sp, {fp, sp, pc}
.L596:
	add	r0, r0, #1
	cmp	r0, #32
	bne	.L597
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L603:
	ldr	ip, .L605+4
	movw	r2, #2564
	ldr	r3, .L605+8
	mov	r0, #0
	ldr	r1, .L605+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L604:
	ldr	ip, .L605+4
	movw	r2, #2565
	ldr	r3, .L605+16
	ldr	r1, .L605+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L606:
	.align	2
.L605:
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
	bhi	.L639
	ldr	r3, .L642
	ldr	r8, [r3, r0, asl #2]
	cmp	r8, #0
	beq	.L640
	ldr	r7, .L642+4
	mov	r0, #0
	mov	r4, r0
	ldr	r1, .L642+8
	mov	r5, r8
	mov	r6, #1
	ldr	r3, [r7, #68]
	blx	r3
	ldr	ip, [r7, #68]
	ldr	r3, [r8, #4]
	mov	r0, r4
	ldr	r2, .L642+12
	ldr	r1, .L642+16
	blx	ip
	ldr	ip, [r7, #68]
	ldr	r3, [r8, #64]
	mov	r0, r4
	ldr	r2, .L642+20
	ldr	r1, .L642+16
	blx	ip
	ldr	ip, [r7, #68]
	ldr	r3, [r8, #68]
	mov	r0, r4
	ldr	r2, .L642+24
	ldr	r1, .L642+16
	blx	ip
	ldr	ip, [r7, #68]
	ldr	r3, [r8, #80]
	mov	r0, r4
	ldr	r2, .L642+28
	ldr	r1, .L642+16
	blx	ip
	ldr	ip, [r7, #68]
	ldr	r3, [r8, #84]
	mov	r0, r4
	ldr	r2, .L642+32
	ldr	r1, .L642+16
	blx	ip
	ldr	ip, [r7, #68]
	ldrsb	r3, [r8, #60]
	mov	r0, r4
	ldr	r2, .L642+36
	ldr	r1, .L642+16
	blx	ip
	ldr	ip, [r7, #68]
	ldrsb	r3, [r8, #62]
	mov	r0, r4
	ldr	r2, .L642+40
	ldr	r1, .L642+16
	blx	ip
	ldr	ip, [r7, #68]
	ldrsb	r3, [r8, #63]
	mov	r0, r4
	ldr	r2, .L642+44
	ldr	r1, .L642+16
	blx	ip
	ldr	ip, [r7, #68]
	ldrsb	r3, [r8, #10]
	mov	r0, r4
	ldr	r2, .L642+48
	ldr	r1, .L642+16
	blx	ip
	ldr	ip, [r7, #68]
	ldrsb	r3, [r8, #11]
	mov	r0, r4
	ldr	r2, .L642+52
	ldr	r1, .L642+16
	blx	ip
	ldrsb	r3, [r8, #13]
	ldr	ip, [r7, #68]
	mov	r0, r4
	ldr	r2, .L642+56
	ldr	r1, .L642+16
	blx	ip
	ldr	r3, [r7, #68]
	mov	r2, #32
	ldr	r1, .L642+60
	mov	r0, r4
	blx	r3
	b	.L610
.L612:
	ldr	ip, [r7, #68]
	mov	r0, #0
	ldrsb	r3, [r5, #89]
	ldrsb	r2, [r5, #88]
	ldr	r1, .L642+64
	blx	ip
	cmp	r6, #31
	bgt	.L641
.L613:
	add	r4, r4, #1
	add	r6, r6, #1
	add	r5, r5, #544
.L610:
	cmp	r4, #0
	ldr	r9, .L642+4
	beq	.L611
	ands	r0, r4, #7
	bne	.L612
	ldr	r3, [r7, #68]
	ldr	r1, .L642+68
	blx	r3
	b	.L612
.L641:
	ldr	r3, [r9, #68]
	mov	r0, #0
	ldrsb	r2, [r8, #10]
	ldr	r1, .L642+72
	blx	r3
	ldrsb	r3, [r8, #10]
	cmp	r3, #0
	addgt	r5, r8, #17408
	addgt	r5, r5, #152
	movgt	r4, #0
	ble	.L620
.L617:
	ldrsb	r3, [r5, #2]
	mov	r0, #0
	ldrsb	r2, [r5]
	add	r4, r4, #1
	ldr	ip, [r7, #68]
	add	r5, r5, #64
	ldr	r1, .L642+64
	blx	ip
	ldrsb	r3, [r8, #10]
	cmp	r3, r4
	ble	.L620
	cmp	r4, #0
	beq	.L617
	ands	r0, r4, #7
	bne	.L617
	ldr	r3, [r9, #68]
	ldr	r1, .L642+68
	blx	r3
	b	.L617
.L620:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldrsb	r2, [r8, #11]
	ldr	r1, .L642+76
	blx	r3
	ldrsb	r3, [r8, #11]
	cmp	r3, #0
	ble	.L616
	add	r5, r8, #19456
	ldr	r6, .L642+4
	add	r5, r5, #152
	mov	r4, #0
.L623:
	ldrsb	r3, [r5, #2]
	mov	r0, #0
	ldrsb	r2, [r5]
	add	r4, r4, #1
	ldr	ip, [r7, #68]
	add	r5, r5, #64
	ldr	r1, .L642+64
	blx	ip
	ldrsb	r3, [r8, #11]
	cmp	r3, r4
	ble	.L616
	cmp	r4, #0
	beq	.L623
	ands	r0, r4, #7
	bne	.L623
	ldr	r3, [r6, #68]
	ldr	r1, .L642+68
	blx	r3
	b	.L623
.L616:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldrsb	r2, [r8, #13]
	ldr	r1, .L642+80
	blx	r3
	ldrsb	r3, [r8, #13]
	cmp	r3, #0
	addgt	r5, r8, #13
	movgt	r4, #0
	ldrgt	r6, .L642+4
	ble	.L622
.L626:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldrsb	r2, [r5, #1]!
	add	r4, r4, #1
	ldr	r1, .L642+84
	blx	r3
	ldrsb	r3, [r8, #13]
	cmp	r3, r4
	ble	.L622
	cmp	r4, #0
	beq	.L626
	ands	r0, r4, #15
	bne	.L626
	ldr	r3, [r6, #68]
	ldr	r1, .L642+68
	blx	r3
	b	.L626
.L622:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldr	r1, .L642+88
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L639:
	ldr	ip, .L642+4
	movw	r2, #2596
	ldr	r3, .L642+92
	mov	r0, #0
.L638:
	ldr	r1, .L642+96
	ldr	ip, [ip, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	ip
.L640:
	ldr	ip, .L642+4
	mov	r0, r8
	ldr	r3, .L642+100
	movw	r2, #2597
	b	.L638
.L611:
	ldr	ip, [r7, #68]
	mov	r0, r4
	ldrsb	r3, [r5, #89]
	ldrsb	r2, [r5, #88]
	ldr	r1, .L642+64
	blx	ip
	b	.L613
.L643:
	.align	2
.L642:
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
	ldr	r3, .L655
	mov	r5, r0
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L653
	ldrsb	r4, [r3, #10]
	cmp	r4, #0
	ble	.L647
	add	r3, r3, #17408
	mov	r2, #0
	add	r3, r3, #152
.L650:
	ldrsb	r1, [r3]
	cmp	r1, #0
	bne	.L648
	ldrsb	r1, [r3, #2]
	cmp	r1, #0
	beq	.L649
.L648:
	add	r2, r2, #1
	add	r3, r3, #64
	cmp	r4, r2
	bne	.L650
.L647:
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
	bgt	.L654
.L651:
	mov	r0, #0
.L646:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L649:
	cmp	r4, r2
	bgt	.L651
	b	.L647
.L654:
	ldr	r1, .L655+4
	stmia	sp, {r0, r4}
	mov	r0, r6
	ldr	ip, [r1, #68]
	ldr	r1, .L655+8
	blx	ip
	mov	r0, r5
	bl	FSP_PrintContext
	mov	r0, #1
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L653:
	ldr	r3, .L655+4
	mov	r0, #1
	ldr	r1, .L655+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L646
.L656:
	.align	2
.L655:
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
	bhi	.L675
	ldr	r3, .L679
	ldr	r4, [r3, r0, asl #2]
	cmp	r4, #0
	beq	.L676
	ldr	r3, .L679+4
	ldr	r2, [r3, #-3896]
	cmp	r2, #0
	beq	.L664
	ldr	r5, [r3, #-3892]
	cmp	r5, #0
	beq	.L677
	blx	r5
	cmp	r0, #0
	bne	.L678
.L664:
	ldrb	ip, [r4, #10]	@ zero_extendqisi2
	cmp	ip, #0
	beq	.L671
	sxtb	ip, ip
	cmp	ip, #0
	ble	.L665
	add	r3, r4, #17408
	mov	r2, #0
	add	r3, r3, #152
.L668:
	ldrsb	r1, [r3]
	cmp	r1, #0
	bne	.L666
	ldrsb	r1, [r3, #2]
	cmp	r1, #0
	beq	.L667
.L666:
	add	r2, r2, #1
	add	r3, r3, #64
	cmp	r2, ip
	bne	.L668
.L665:
	mov	r0, r6
	bl	FSP_IsFsLeak
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L667:
	cmp	ip, r2
	ble	.L665
.L671:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L678:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L677:
	ldr	r3, .L679+8
	mov	r0, r5
	ldr	r1, .L679+12
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L675:
	ldr	ip, .L679+8
	movw	r2, #1338
	ldr	r3, .L679+16
	mov	r0, #0
	ldr	r1, .L679+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L676:
	ldr	ip, .L679+8
	mov	r0, r4
	ldr	r3, .L679+24
	movw	r2, #1339
	ldr	r1, .L679+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L680:
	.align	2
.L679:
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
	bhi	.L684
	mov	r2, r0, asl #8
	ldr	r3, .L685
	add	r0, r2, r0, lsl #3
	add	r0, r3, r0
	ldrb	r0, [r0, #64]	@ zero_extendqisi2
	ldmfd	sp, {fp, sp, pc}
.L684:
	ldr	ip, .L685+4
	movw	r2, #2697
	ldr	r3, .L685+8
	mov	r0, #0
	ldr	r1, .L685+12
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #255
	ldmfd	sp, {fp, sp, pc}
.L686:
	.align	2
.L685:
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
	bhi	.L689
	mov	r2, r0, asl #8
	ldr	r3, .L690
	add	r0, r2, r0, lsl #3
	mov	r2, #1
	add	r0, r3, r0
	strb	r2, [r0, #64]
	ldmfd	sp, {fp, sp, pc}
.L689:
	ldr	ip, .L690+4
	movw	r2, #2707
	ldr	r3, .L690+8
	mov	r0, #0
	ldr	r1, .L690+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L691:
	.align	2
.L690:
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
	ldr	ip, .L706
	ldr	r2, [ip, r0, asl #2]
	cmp	r2, #0
	beq	.L701
	mov	r3, r0, asl #8
	add	r3, r3, r0, lsl #3
	add	r3, ip, r3
	add	r3, r3, #64
	ldrsh	lr, [r3, #2]
	cmn	lr, #1
	beq	.L701
	ldrh	r3, [r3, #4]
	cmp	r3, lr
	beq	.L701
	ldr	r2, [r2, #4]
	cmp	r2, #1
	beq	.L705
.L701:
	mov	r0, #0
	ldmfd	sp, {r4, fp, sp, pc}
.L705:
	and	r3, r3, #63
	cmp	lr, r3
	beq	.L701
	mov	r4, r0, asl #6
	add	r0, r4, r0, lsl #1
	add	r4, r0, r3
	add	r4, ip, r4, lsl #2
	ldr	r4, [r4, #72]
	cmp	r4, r1
	bne	.L695
	b	.L703
.L696:
	ldr	r2, [r2, #72]
	cmp	r2, r1
	beq	.L703
.L695:
	add	r3, r3, #1
	and	r3, r3, #63
	add	r2, r0, r3
	cmp	lr, r3
	add	r2, ip, r2, lsl #2
	bne	.L696
	b	.L701
.L703:
	mov	r0, #1
	ldmfd	sp, {r4, fp, sp, pc}
.L707:
	.align	2
.L706:
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
	beq	.L730
	cmp	r2, #1
	bhi	.L731
	beq	.L732
	mov	r3, r0, asl #8
	mov	r10, r0, asl #3
	ldr	r5, .L733
	add	r9, r10, r3
	str	r3, [fp, #-48]
	add	r9, r5, r9
	ldrh	r7, [r9, #68]
	ldrh	r8, [r9, #70]
	and	r3, r7, #63
	cmp	r8, r3
	beq	.L713
	mov	r6, r0, asl #6
	add	r6, r6, r0, lsl #1
.L718:
	add	r2, r6, r3
	ldr	r0, [r1, #88]
	add	r2, r5, r2, lsl #2
	ldr	r2, [r2, #72]
	cmp	r0, r2
	beq	.L719
	cmp	r2, #0
	movne	ip, r3
	bne	.L715
	mov	lr, #2
.L714:
	mov	ip, r7
.L717:
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
	bne	.L717
	cmp	lr, #1
	add	r7, r7, #1
	and	r7, r7, #63
	strh	r7, [r9, #68]	@ movhi
	beq	.L713
.L715:
	add	r3, ip, #1
	and	r3, r3, #63
	cmp	r3, r8
	bne	.L718
.L713:
	ldr	r3, [fp, #-48]
	add	r10, r10, r3
	add	r5, r5, r10
	ldrsh	r2, [r5, #66]
	cmp	r7, r2
	moveq	r1, #0
	mvneq	r2, #0
	streqb	r1, [r5, #64]
	streqh	r2, [r5, #66]	@ movhi
.L708:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L719:
	mov	lr, #1
	b	.L714
.L731:
	ldr	ip, .L733+4
	mov	r2, #2768
	ldr	r3, .L733+8
	mov	r0, #0
.L728:
	ldr	r1, .L733+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	ip
.L732:
	ldr	ip, .L733
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
.L730:
	ldr	ip, .L733+4
	mov	r0, r1
	ldr	r3, .L733+16
	movw	r2, #2767
	b	.L728
.L734:
	.align	2
.L733:
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
	ldr	r3, .L743
	ldr	ip, [r3, r0, asl #2]
	add	r3, ip, #16896
	ldrsb	r3, [r3, #57]
	cmp	r3, #0
	beq	.L740
	add	r3, ip, #16384
	mov	r2, #30
	add	r3, r3, #25
	b	.L739
.L737:
	subs	r2, r2, #1
	bcc	.L742
.L739:
	ldrsb	r1, [r3]
	sub	r3, r3, #544
	cmp	r1, #0
	bne	.L737
.L738:
	mov	r3, r2, asl #9
	mov	r0, r2
	add	r2, r3, r2, lsl #5
	mov	r3, #0
	add	ip, ip, r2
	str	r3, [ip, #608]
	str	r3, [ip, #612]
	ldmfd	sp, {fp, sp, pc}
.L742:
	mov	r2, #0
	b	.L738
.L740:
	mov	r2, #31
	b	.L738
.L744:
	.align	2
.L743:
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
	ldr	ip, .L747
	movw	r2, #2879
	ldr	r3, .L747+4
	mov	r0, #0
	ldr	r1, .L747+8
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L748:
	.align	2
.L747:
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
	bhi	.L753
	ldr	r3, .L755
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L754
	ldr	r0, [r0, #4]
	subs	r0, r0, #1
	mvnne	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L753:
	ldr	ip, .L755+4
	movw	r2, #2910
	ldr	r3, .L755+8
	mov	r0, #0
	ldr	r1, .L755+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L754:
	ldr	ip, .L755+4
	movw	r2, #2911
	ldr	r3, .L755+16
	ldr	r1, .L755+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L756:
	.align	2
.L755:
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
	bhi	.L761
	ldr	r3, .L763
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L762
	ldrsb	r0, [r0, #9]
	ldmfd	sp, {fp, sp, pc}
.L761:
	ldr	ip, .L763+4
	mov	r2, #2928
	ldr	r3, .L763+8
	mov	r0, #0
	ldr	r1, .L763+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L762:
	ldr	ip, .L763+4
	movw	r2, #2929
	ldr	r3, .L763+16
	ldr	r1, .L763+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L764:
	.align	2
.L763:
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
	.type	__func__.13743, %object
	.size	__func__.13743, 15
__func__.13743:
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
