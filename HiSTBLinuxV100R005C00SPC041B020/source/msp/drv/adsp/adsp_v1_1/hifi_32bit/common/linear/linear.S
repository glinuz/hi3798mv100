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
	.file	"linear.c"
	.global	__aeabi_idiv
	.text
	.align	2
	.global	LSRC_Create
	.type	LSRC_Create, %function
LSRC_Create:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	movw	ip, #60927
	sub	lr, r1, #1
	movt	ip, 2
	cmp	lr, ip
	mov	r4, #0
	mov	r7, r1
	mov	r8, r0
	mov	r5, r2
	mov	r9, r3
	ldr	r6, [fp, #4]
	str	r4, [r0]
	ldmhifd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	sub	r3, r2, #1
	cmp	r3, ip
	ldmhifd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	mov	r0, r2
	bl	__aeabi_idiv
	cmp	r0, #6
	ldmgtfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	mov	r1, #76
	mov	r0, #26
	bl	HI_ADSP_MALLOC
	subs	r10, r0, #0
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	ldr	r3, .L18
	mov	r1, r4
	mov	r2, #76
	ldr	r3, [r3, #20]
	blx	r3
	cmp	r5, #8000
	ldr	r1, .L18+4
	beq	.L11
	mov	ip, #1
	mov	r3, r1
.L6:
	ldr	r2, [r3, #4]!
	cmp	r2, r5
	beq	.L5
	add	ip, ip, #1
	cmp	ip, #13
	bne	.L6
.L10:
	cmp	r6, #0
	addne	r3, r10, #32
	movne	ip, #0
	movne	r2, ip
	beq	.L9
.L8:
	add	ip, ip, #1
	str	r2, [r3, #4]!
	cmp	ip, r6
	bne	.L8
.L9:
	str	r9, [r10]
	stmib	r10, {r6, r7}
	str	r7, [r10, #12]
	str	r5, [r10, #16]
	str	r10, [r8]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L11:
	mov	ip, r4
.L5:
	add	ip, r1, ip, lsl #2
	ldr	r3, [ip, #52]
	str	r3, [r10, #32]
	b	.L10
.L19:
	.align	2
.L18:
	.word	g_AdspOsalFunc
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	LSRC_Create, .-LSRC_Create
	.align	2
	.global	LSRC_Destroy
	.type	LSRC_Destroy, %function
LSRC_Destroy:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r1, r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	mov	r0, #26
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HI_ADSP_FREE
	UNWIND(.fnend)
	.size	LSRC_Destroy, .-LSRC_Destroy
	.align	2
	.global	LSRC_GetMaxOutputNum
	.type	LSRC_GetMaxOutputNum, %function
LSRC_GetMaxOutputNum:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r3, r0, #0
	mov	r2, r1
	beq	.L24
	ldr	r0, [r3, #16]
	ldr	r1, [r3, #8]
	mul	r0, r0, r2
	bl	__aeabi_idiv
	add	r0, r0, #1
	ldmfd	sp, {fp, sp, pc}
.L24:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	LSRC_GetMaxOutputNum, .-LSRC_GetMaxOutputNum
	.global	__aeabi_idivmod
	.align	2
	.global	LSRC_GetMinInputNum
	.type	LSRC_GetMinInputNum, %function
LSRC_GetMinInputNum:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	beq	.L27
	ldr	r3, [r0, #8]
	ldr	r6, [r0, #16]
	mul	r4, r3, r1
	mov	r1, r6
	mov	r0, r4
	bl	__aeabi_idiv
	mov	r1, r6
	mov	r5, r0
	mov	r0, r4
	bl	__aeabi_idivmod
	cmp	r1, #0
	beq	.L28
	add	r0, r5, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L28:
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L27:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	LSRC_GetMinInputNum, .-LSRC_GetMinInputNum
	.align	2
	.global	LSRC_CheckUpdate
	.type	LSRC_CheckUpdate, %function
LSRC_CheckUpdate:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldmia	r0, {r5, lr}
	ldr	ip, [fp, #4]
	ldr	r4, [r0, #12]
	subs	ip, lr, ip
	ldr	lr, [r0, #16]
	movne	ip, #1
	cmp	r5, r3
	moveq	r3, ip
	addne	r3, ip, #1
	cmp	r4, r1
	moveq	r1, r3
	addne	r1, r3, #1
	cmp	lr, r2
	moveq	r2, r1
	addne	r2, r1, #1
	adds	r0, r2, #0
	movne	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	LSRC_CheckUpdate, .-LSRC_CheckUpdate
	.align	2
	.global	LSRC_UpdateChange
	.type	LSRC_UpdateChange, %function
LSRC_UpdateChange:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldmia	r0, {r6, ip}
	ldr	r4, [fp, #4]
	ldr	r5, [r0, #12]
	subs	lr, ip, r4
	movne	lr, #1
	cmp	r6, r3
	moveq	ip, lr
	addne	ip, lr, #1
	ldr	lr, [r0, #16]
	cmp	r5, r1
	addne	ip, ip, #1
	cmp	lr, r2
	addne	ip, ip, #1
	cmp	ip, #0
	ldmeqfd	sp, {r4, r5, r6, fp, sp, pc}
	cmp	r2, #8000
	stmia	r0, {r3, r4}
	ldr	lr, .L44
	mov	r3, #0
	str	r1, [r0, #8]
	str	r1, [r0, #12]
	str	r2, [r0, #16]
	str	r3, [r0, #20]
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	beq	.L32
	mov	r3, #1
	mov	r1, lr
.L33:
	ldr	ip, [r1, #4]!
	cmp	r2, ip
	beq	.L32
	add	r3, r3, #1
	cmp	r3, #13
	bne	.L33
	cmp	r4, #0
	ldmeqfd	sp, {r4, r5, r6, fp, sp, pc}
.L43:
	mov	r3, #0
	add	r0, r0, #32
	mov	r2, r3
.L35:
	add	r3, r3, #1
	str	r2, [r0, #4]!
	cmp	r3, r4
	bne	.L35
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L32:
	add	r3, lr, r3, lsl #2
	cmp	r4, #0
	ldr	r3, [r3, #52]
	str	r3, [r0, #32]
	bne	.L43
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L45:
	.align	2
.L44:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	LSRC_UpdateChange, .-LSRC_UpdateChange
	.align	2
	.global	LSRC_Flush
	.type	LSRC_Flush, %function
LSRC_Flush:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	ip, [r0, #16]
	mov	r3, #0
	ldr	lr, .L56
	cmp	ip, #8000
	str	r3, [r0, #20]
	str	r3, [r0, #24]
	beq	.L47
	mov	r3, #1
	mov	r2, lr
.L48:
	ldr	r1, [r2, #4]!
	cmp	r1, ip
	beq	.L47
	add	r3, r3, #1
	cmp	r3, #13
	bne	.L48
.L52:
	ldr	r2, [r0, #4]
	cmp	r2, #0
	ldmeqfd	sp, {fp, sp, pc}
	mov	r3, #0
	add	r0, r0, #32
	mov	r1, r3
.L50:
	add	r3, r3, #1
	str	r1, [r0, #4]!
	cmp	r3, r2
	bne	.L50
	ldmfd	sp, {fp, sp, pc}
.L47:
	add	r3, lr, r3, lsl #2
	ldr	r3, [r3, #52]
	str	r3, [r0, #32]
	b	.L52
.L57:
	.align	2
.L56:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	LSRC_Flush, .-LSRC_Flush
	.align	2
	.global	LSRC_CheckDoSRC
	.type	LSRC_CheckDoSRC, %function
LSRC_CheckDoSRC:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0, #8]
	ldr	r3, [r0, #16]
	cmp	r2, r3
	beq	.L61
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L61:
	ldr	r0, [r0, #24]
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	LSRC_CheckDoSRC, .-LSRC_CheckDoSRC
	.align	2
	.global	LSRC_SetSpeedAdjust
	.type	LSRC_SetSpeedAdjust, %function
LSRC_SetSpeedAdjust:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0, #28]
	add	ip, r1, #100
	rsb	r3, r1, r2
	clz	r3, r3
	mov	r3, r3, lsr #5
	cmp	ip, #200
	orrhi	r3, r3, #1
	cmp	r3, #0
	ldmnefd	sp, {r4, r5, fp, sp, pc}
	adds	r3, r1, #0
	add	ip, r1, #1000
	movw	r4, #19923
	str	r1, [r0, #28]
	movne	r3, #1
	cmp	r2, #0
	movt	r4, 4194
	movne	r3, #0
	cmp	r3, #0
	movne	r3, #1
	strne	r3, [r0, #24]
	clz	r3, r1
	cmp	r2, #0
	mov	r2, #0
	str	r2, [r0, #20]
	mov	r3, r3, lsr #5
	moveq	r3, #0
	cmp	r3, #0
	movne	r3, #1
	strne	r3, [r0, #24]
	ldr	r3, [r0, #12]
	mul	r3, r3, ip
	smull	r4, r5, r3, r4
	mov	r3, r3, asr #31
	rsb	r3, r3, r5, asr #6
	str	r3, [r0, #8]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	LSRC_SetSpeedAdjust, .-LSRC_SetSpeedAdjust
	.align	2
	.global	LSRC_ProcessFrame
	.type	LSRC_ProcessFrame, %function
LSRC_ProcessFrame:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #52)
	sub	sp, sp, #52
	str	r1, [fp, #-80]
	ldr	r1, [r0]
	mov	r10, r0
	str	r2, [fp, #-72]
	cmp	r1, #16
	str	r3, [fp, #-76]
	beq	.L158
	ldr	r3, [fp, #-76]
	ldr	r2, [r0, #24]
	cmp	r3, #0
	ldr	r9, [r0, #4]
	movgt	r3, #1
	movle	r3, #0
	cmp	r2, #0
	moveq	r3, #0
	andne	r3, r3, #1
	cmp	r3, #0
	beq	.L159
	ldr	r3, [r0, #8]
	ldr	lr, [r0, #16]
	cmp	r3, lr
	beq	.L104
	cmp	r9, #0
	ble	.L110
	ldr	r3, [fp, #-80]
	add	r2, r0, #32
	sub	r1, r3, #4
	mov	r3, #0
.L109:
	add	r3, r3, #1
	ldr	r0, [r1, #4]!
	cmp	r3, r9
	str	r0, [r2, #4]!
	bne	.L109
.L110:
	ldr	r2, [fp, #-76]
	mov	r3, #0
	str	r3, [r10, #24]
	sub	r2, r2, #1
	str	r3, [fp, #-48]
	str	r2, [fp, #-76]
	mov	r3, #1
	str	r3, [fp, #-60]
.L106:
	ldr	r3, [fp, #-76]
	cmp	r3, #0
	ble	.L137
	ldr	r2, [r10, #20]
	cmp	r2, lr
	bge	.L115
	ldr	r3, [fp, #-48]
	ldr	r1, [fp, #-72]
	mul	r8, r3, r9
	mov	r3, r9, asl #2
	str	r3, [fp, #-52]
	ldr	r3, [fp, #-60]
	mul	r3, r9, r3
	add	r8, r1, r8, lsl #2
	ldr	r1, [fp, #-80]
	add	r3, r1, r3, lsl #2
	str	r3, [fp, #-56]
.L116:
	cmp	r9, #0
	ble	.L119
	ldr	r4, [fp, #-56]
	add	r6, r10, #32
	mov	r5, r8
	mov	r0, #0
.L120:
	ldr	r7, [r10, #32]
	ldr	ip, [r4], #4
	mul	r3, r2, r7
	mov	r1, r3
#APP
	smull r3,r1,ip,r1
	rsb	r2, r2, lr
	ldr	ip, [r6, #4]!
	mul	r3, r2, r7
#APP
	smull r2,r3,ip,r3
	add	r3, r1, r3
	add	r0, r0, #1
	mov	r1, r3, asr #31
	mov	r2, r3, asr #14
	cmp	r1, r3, asr #29
	eorne	r2, r1, #32512
	eorne	r2, r2, #255
	cmp	r0, r9
	mov	r2, r2, asl #16
	str	r2, [r5], #4
	ldr	r2, [r10, #20]
	ldr	lr, [r10, #16]
	bne	.L120
.L119:
	ldr	r3, [r10, #8]
	ldr	r1, [fp, #-48]
	add	r2, r2, r3
	str	r2, [r10, #20]
	add	r1, r1, #1
	cmp	r2, lr
	str	r1, [fp, #-48]
	ldr	r1, [fp, #-52]
	add	r8, r8, r1
	blt	.L116
.L115:
	ldr	r1, [fp, #-76]
	rsb	r8, lr, r2
	str	r8, [r10, #20]
	cmp	r1, #1
	ble	.L137
	ldr	r0, [fp, #-60]
	mov	r2, r9, asl #2
	ldr	ip, [fp, #-80]
	add	r3, r0, #1
	str	r3, [fp, #-92]
	str	r3, [fp, #-68]
	add	r1, r0, r1
	mla	r3, r3, r2, ip
	str	r1, [fp, #-88]
	rsb	r1, r2, #0
	str	r1, [fp, #-84]
	ldr	r1, [fp, #-48]
	str	r2, [fp, #-56]
	mov	r7, r1
	mov	r2, r1
	str	r3, [fp, #-52]
.L121:
	ldr	ip, [r10, #16]
	cmp	ip, r8
	ble	.L124
	mul	r3, r2, r9
	ldr	r1, [fp, #-52]
	rsb	r2, r7, r2
	ldr	r0, [fp, #-84]
	str	r2, [fp, #-60]
	ldr	r2, [fp, #-72]
	add	r1, r1, r0
	str	r1, [fp, #-64]
	add	r3, r2, r3, lsl #2
	str	r3, [fp, #-48]
.L127:
	cmp	r9, #0
	ble	.L126
	ldr	r5, [fp, #-48]
	mov	r1, #0
	ldr	r4, [fp, #-64]
	ldr	lr, [fp, #-52]
.L125:
	ldr	r6, [r10, #32]
	ldr	r0, [lr], #4
	mul	r3, r8, r6
	mov	r2, r3
#APP
	smull r3,r2,r0,r2
	rsb	r8, r8, ip
	ldr	r0, [r4], #4
	mul	r3, r8, r6
#APP
	smull ip,r3,r0,r3
	add	r3, r2, r3
	add	r1, r1, #1
	mov	r0, r3, asr #31
	mov	r2, r3, asr #14
	cmp	r0, r3, asr #29
	eorne	r2, r0, #32512
	eorne	r2, r2, #255
	cmp	r1, r9
	mov	r2, r2, asl #16
	str	r2, [r5], #4
	ldr	r8, [r10, #20]
	ldr	ip, [r10, #16]
	bne	.L125
.L126:
	ldr	r3, [r10, #8]
	add	r7, r7, #1
	ldr	r2, [fp, #-48]
	ldr	r1, [fp, #-56]
	add	r8, r8, r3
	cmp	r8, ip
	str	r8, [r10, #20]
	add	r2, r2, r1
	str	r2, [fp, #-48]
	ldr	r2, [fp, #-60]
	add	r2, r2, r7
	blt	.L127
.L124:
	ldr	r3, [fp, #-68]
	rsb	r8, ip, r8
	ldr	r1, [fp, #-88]
	add	r3, r3, #1
	str	r3, [fp, #-68]
	cmp	r3, r1
	ldr	r3, [fp, #-52]
	ldr	r1, [fp, #-56]
	str	r8, [r10, #20]
	add	r3, r3, r1
	str	r3, [fp, #-52]
	bne	.L121
	ldr	r3, [fp, #-76]
	mov	r0, r7
	ldr	r2, [fp, #-92]
	sub	r3, r3, #2
	add	r3, r3, r2
	str	r3, [fp, #-60]
.L114:
	cmp	r9, #0
	ble	.L83
	ldr	r3, [fp, #-60]
	mov	r1, #0
	ldr	ip, [fp, #-80]
	mul	r2, r3, r9
	add	r3, r10, #32
	add	r2, ip, r2, lsl #2
.L128:
	add	r1, r1, #1
	ldr	ip, [r2], #4
	cmp	r1, r9
	str	ip, [r3, #4]!
	bne	.L128
.L83:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L159:
	str	r3, [fp, #-48]
	ldr	lr, [r0, #16]
	ldr	r3, [r0, #8]
.L103:
	cmp	r3, lr
	movne	r3, #0
	strne	r3, [fp, #-60]
	bne	.L106
	ldr	r3, [fp, #-76]
	cmp	r3, #0
	ble	.L135
	ldr	r2, [fp, #-48]
	mov	r4, r9, asl #2
	ldr	r6, [fp, #-80]
	mov	ip, r3
	ldr	r7, [fp, #-72]
	mov	r0, #0
	mul	lr, r9, r2
	mov	lr, lr, asl #2
.L111:
	cmp	r9, #0
	ble	.L113
	add	r2, r0, lr
	add	r1, r6, r0
	add	r2, r7, r2
	mov	r3, #0
.L112:
	add	r3, r3, #1
	ldr	r5, [r1], #4
	cmp	r3, r9
	str	r5, [r2], #4
	bne	.L112
.L113:
	subs	ip, ip, #1
	add	r0, r0, r4
	bne	.L111
	ldr	r3, [fp, #-48]
	ldr	r2, [fp, #-76]
	add	r0, r3, r2
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L158:
	cmp	r3, #0
	ldr	r3, [r10, #4]
	ldr	r0, [r0, #24]
	ldr	r4, [r10, #16]
	str	r3, [fp, #-48]
	movgt	r3, #1
	movle	r3, #0
	cmp	r0, #0
	moveq	r0, #0
	andne	r0, r3, #1
	cmp	r0, #0
	ldr	r3, [r10, #8]
	beq	.L75
	cmp	r3, r4
	beq	.L76
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ble	.L82
	ldr	r1, [fp, #-48]
	add	r2, r10, #32
	ldr	r3, [fp, #-80]
	add	r1, r3, r1, lsl #1
.L81:
	ldrsh	r0, [r3], #2
	cmp	r3, r1
	str	r0, [r2, #4]!
	bne	.L81
.L82:
	ldr	r3, [fp, #-76]
	mov	r0, #0
	mov	r9, #1
	str	r0, [r10, #24]
	sub	r3, r3, #1
	str	r3, [fp, #-76]
	b	.L78
.L76:
	ldr	r1, [fp, #-48]
	cmp	r1, #0
	ble	.L129
	ldr	r3, [fp, #-72]
	add	r2, r10, #32
	add	r1, r3, r1, lsl #1
.L80:
	ldr	r0, [r2, #4]!
	strh	r0, [r3], #2
	cmp	r3, r1
	bne	.L80
	ldr	r3, [r10, #8]
	ldr	r4, [r10, #16]
.L79:
	mov	r0, #1
	mov	r2, #0
	str	r2, [r10, #24]
.L75:
	cmp	r3, r4
	movne	r9, #0
	beq	.L160
.L78:
	ldr	r3, [fp, #-76]
	cmp	r3, #0
	ble	.L87
	ldr	r2, [r10, #20]
	cmp	r2, r4
	bge	.L88
	ldr	r8, [fp, #-48]
	str	r9, [fp, #-64]
	mul	r1, r0, r8
	mov	ip, r8, asl #1
	mul	r3, r8, r9
	str	ip, [fp, #-56]
	ldr	ip, [fp, #-72]
	add	r1, ip, r1, lsl #1
	str	r1, [fp, #-52]
	ldr	r1, [fp, #-80]
	add	r3, r1, r3, lsl #1
	str	r3, [fp, #-60]
.L89:
	cmp	r8, #0
	ble	.L92
	add	r7, r10, #32
	ldr	r6, [fp, #-52]
	ldr	r5, [fp, #-60]
	mov	ip, #0
	mov	r3, r4
	b	.L93
.L91:
	ldr	r3, [r10, #16]
.L93:
	ldr	r9, [r10, #32]
	ldrsh	lr, [r5], #2
	mul	r1, r2, r9
#APP
	smull r4,r1,lr,r1
	rsb	r2, r2, r3
	ldr	lr, [r7, #4]!
	mul	r3, r2, r9
#APP
	smull r2,r3,lr,r3
	add	r3, r1, r3
	add	ip, ip, #1
	mov	r3, r3, asl #2
	mov	r2, r3, asr #31
	cmp	r2, r3, asr #15
	eorne	r3, r2, #32512
	eorne	r3, r3, #255
	cmp	ip, r8
	strh	r3, [r6], #2
	ldr	r2, [r10, #20]
	bne	.L91
	ldr	r4, [r10, #16]
.L92:
	ldr	r3, [r10, #8]
	add	r0, r0, #1
	ldr	r1, [fp, #-52]
	add	r2, r2, r3
	ldr	ip, [fp, #-56]
	cmp	r2, r4
	str	r2, [r10, #20]
	add	r1, r1, ip
	str	r1, [fp, #-52]
	blt	.L89
	ldr	r9, [fp, #-64]
.L88:
	ldr	r1, [fp, #-76]
	rsb	r5, r4, r2
	str	r5, [r10, #20]
	cmp	r1, #1
	ble	.L87
	ldr	r2, [fp, #-48]
	add	r1, r9, r1
	str	r1, [fp, #-88]
	add	r3, r9, #1
	str	r3, [fp, #-92]
	mov	r8, r0
	mov	r2, r2, asl #1
	str	r3, [fp, #-68]
	rsb	r1, r2, #0
	str	r1, [fp, #-84]
	ldr	r1, [fp, #-80]
	str	r2, [fp, #-52]
	mla	r3, r3, r2, r1
	mov	r2, r0
	str	r3, [fp, #-56]
.L94:
	ldr	ip, [r10, #16]
	cmp	ip, r5
	ble	.L161
	ldr	r3, [fp, #-48]
	ldr	r1, [fp, #-84]
	mul	r9, r2, r3
	ldr	r3, [fp, #-56]
	rsb	r2, r8, r2
	str	r2, [fp, #-60]
	ldr	r2, [fp, #-52]
	add	r1, r3, r1
	str	r1, [fp, #-64]
	add	r6, r2, r3
	ldr	r3, [fp, #-72]
	add	r9, r3, r9, lsl #1
.L100:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ble	.L99
	mov	r4, r9
	ldr	lr, [fp, #-64]
	ldr	r1, [fp, #-56]
	mov	r3, ip
	b	.L98
.L96:
	ldr	r3, [r10, #16]
.L98:
	ldr	r7, [r10, #32]
	ldrsh	r0, [r1], #2
	mul	r2, r5, r7
#APP
	smull ip,r2,r0,r2
	rsb	r5, r5, r3
	ldrsh	r0, [lr], #2
	mul	r3, r5, r7
#APP
	smull ip,r3,r0,r3
	add	r3, r2, r3
	mov	r3, r3, asl #2
	mov	r2, r3, asr #31
	cmp	r2, r3, asr #15
	eorne	r3, r2, #32512
	eorne	r3, r3, #255
	cmp	r1, r6
	strh	r3, [r4], #2
	ldr	r5, [r10, #20]
	bne	.L96
	ldr	ip, [r10, #16]
.L99:
	ldr	r3, [r10, #8]
	add	r8, r8, #1
	ldr	r2, [fp, #-52]
	add	r5, r5, r3
	str	r5, [r10, #20]
	add	r9, r9, r2
	cmp	r5, ip
	ldr	r2, [fp, #-60]
	add	r2, r2, r8
	blt	.L100
.L97:
	ldr	r3, [fp, #-68]
	rsb	r5, ip, r5
	ldr	r1, [fp, #-88]
	add	r3, r3, #1
	str	r5, [r10, #20]
	cmp	r3, r1
	str	r3, [fp, #-68]
	str	r6, [fp, #-56]
	bne	.L94
	ldr	r3, [fp, #-76]
	mov	r0, r8
	sub	r9, r3, #2
	ldr	r3, [fp, #-92]
	add	r9, r3, r9
.L87:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ble	.L83
	mul	r2, r9, r3
	ldr	ip, [fp, #-80]
	add	r1, r10, #32
	add	r3, r3, r2
	add	r2, ip, r2, lsl #1
	add	r3, ip, r3, lsl #1
.L101:
	ldrsh	ip, [r2], #2
	cmp	r2, r3
	str	ip, [r1, #4]!
	bne	.L101
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L104:
	cmp	r9, #0
	ble	.L133
	ldr	r3, [fp, #-72]
	add	r1, r0, #32
	sub	r2, r3, #4
	mov	r3, #0
.L108:
	add	r3, r3, #1
	ldr	r0, [r1, #4]!
	cmp	r3, r9
	str	r0, [r2, #4]!
	bne	.L108
	ldr	r3, [r10, #8]
	ldr	lr, [r10, #16]
.L107:
	mov	r2, #0
	mov	r1, #1
	str	r2, [r10, #24]
	str	r1, [fp, #-48]
	b	.L103
.L161:
	ldr	r3, [fp, #-56]
	ldr	r1, [fp, #-52]
	add	r6, r3, r1
	b	.L97
.L137:
	ldr	r0, [fp, #-48]
	b	.L114
.L160:
	ldr	r4, [fp, #-76]
	cmp	r4, #0
	ble	.L83
	ldr	r2, [fp, #-48]
	ldr	r3, [fp, #-80]
	mul	lr, r0, r2
	mov	r5, r2, asl #1
	ldr	r2, [fp, #-72]
	add	lr, r2, lr, lsl #1
.L84:
	ldr	r2, [fp, #-48]
	add	ip, r5, r3
	cmp	r2, #0
	movgt	r2, lr
	ble	.L86
.L85:
	ldrh	r1, [r3], #2
	cmp	ip, r3
	strh	r1, [r2], #2
	bne	.L85
.L86:
	subs	r4, r4, #1
	mov	r3, ip
	add	lr, lr, r5
	bne	.L84
	ldr	r3, [fp, #-76]
	add	r0, r0, r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L129:
	mov	r4, r3
	b	.L79
.L133:
	mov	lr, r3
	b	.L107
.L135:
	ldr	r0, [fp, #-48]
	b	.L83
	UNWIND(.fnend)
	.size	LSRC_ProcessFrame, .-LSRC_ProcessFrame
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	g_s32SampleRateTab, %object
	.size	g_s32SampleRateTab, 52
g_s32SampleRateTab:
	.word	8000
	.word	11025
	.word	12000
	.word	16000
	.word	22050
	.word	24000
	.word	32000
	.word	44100
	.word	48000
	.word	88200
	.word	96000
	.word	176400
	.word	192000
	.type	g_s32IvtSfTab, %object
	.size	g_s32IvtSfTab, 52
g_s32IvtSfTab:
	.word	134217
	.word	97391
	.word	89478
	.word	67109
	.word	48696
	.word	44739
	.word	33554
	.word	24348
	.word	22369
	.word	12174
	.word	11185
	.word	6087
	.word	5592
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
