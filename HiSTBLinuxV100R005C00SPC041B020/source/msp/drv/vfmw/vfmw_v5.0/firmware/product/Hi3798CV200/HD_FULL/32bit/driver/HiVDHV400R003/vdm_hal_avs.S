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
	.file	"vdm_hal_avs.c"
	.text
	.align	2
	.global	AVSHAL_V400R003_InitHal
	.type	AVSHAL_V400R003_InitHal, %function
AVSHAL_V400R003_InitHal:
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
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	AVSHAL_V400R003_InitHal, .-AVSHAL_V400R003_InitHal
	.align	2
	.global	AVSHAL_V400R003_WirteSlicMsg
	.type	AVSHAL_V400R003_WirteSlicMsg, %function
AVSHAL_V400R003_WirteSlicMsg:
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
	UNWIND(.pad #20)
	sub	sp, sp, #20
	ldr	ip, .L32
	movw	r3, #1228
	mov	r10, r0
	add	r7, r0, #96
	str	r1, [fp, #-48]
	mla	r2, r3, r2, ip
	ldr	r6, [r2, #56]
	add	r4, r6, #320
	mov	r0, r6
	bl	MEM_Phy2Vir
	ldr	r3, [r10, #112]
	cmp	r3, #0
	moveq	r2, r3
	add	r5, r0, #320
	beq	.L3
	ldr	r3, [r10, #96]
	mov	r1, #0
	ldr	r2, [fp, #-48]
	add	r6, r6, #576
	add	r3, r3, #4
	rsb	r3, r2, r3
	and	r2, r3, #15
	bic	r3, r3, #15
	mov	r2, r2, asl #3
	cmp	r2, #7
	addls	r2, r2, #120
	subhi	r2, r2, #8
	subls	r3, r3, #4
	mov	r2, r2, asl #25
	bic	r3, r3, #-16777216
	orr	r2, r2, #8
	str	r2, [r0, #320]
	str	r3, [r0, #324]
	mov	r2, #1
	str	r1, [r0, #328]
	str	r1, [r0, #332]
	ldr	r3, [r10, #112]
	sub	r3, r3, #1
	mov	r3, r3, asl #16
	str	r3, [r0, #336]
	str	r6, [r0, #572]
.L3:
	add	r8, r10, #12288
	ldr	r3, [r8, #2144]
	cmp	r3, #0
	ble	.L2
	mov	r0, #0
	add	r3, r4, r2, lsl #8
	mov	r6, r0
	str	r3, [fp, #-56]
	mov	r3, r0
	str	r10, [fp, #-52]
.L22:
	cmp	r6, #512
	bge	.L7
	mov	r9, r3, asl #2
	mov	r4, r3, asl #5
	cmp	r3, #0
	rsb	r1, r9, r4
	add	r1, r7, r1
	ble	.L9
	ldr	ip, [r1, #16]
	ldr	r2, [r1, #-12]
	cmp	ip, r2
	bls	.L10
.L9:
	ldr	r2, [r1]
	ldr	ip, [fp, #-48]
	ldr	r0, [r1, #8]
	add	r2, r2, #4
	ldr	lr, [r1, #4]
	rsb	r2, ip, r2
	and	ip, r2, #15
	sub	r0, r0, #4
	cmp	lr, #0
	bic	r2, r2, #15
	mov	r0, r0, asl #3
	mov	ip, ip, asl #3
	beq	.L30
	rsb	r4, r9, r4
	ldr	r9, [fp, #-52]
	add	r4, r9, r4
	ldr	r4, [r4, #108]
	cmp	r4, #0
	ble	.L26
	ldr	r4, [fp, #-48]
	cmp	r0, #0
	rsb	lr, r4, lr
	and	r4, lr, #15
	bic	r10, lr, #15
	mov	r9, r4, asl #3
	ldr	r4, [r1, #12]
	mov	r4, r4, asl #3
	beq	.L27
	bic	r4, r4, #-33554432
	bic	lr, lr, #-16777216
	orr	r4, r4, r9, asl #25
	bic	lr, lr, #15
.L11:
	cmp	ip, #7
	add	r9, r0, #8
	addls	r10, ip, #120
	subhi	r10, ip, #8
	bic	r9, r9, #-33554432
	mov	ip, r3, asl #8
	orr	r9, r9, r10, asl #25
	add	r0, ip, #4
	str	r9, [r5, r3, asl #8]
	add	r10, ip, #8
	add	r9, ip, #12
	subls	r2, r2, #4
	bic	r2, r2, #-16777216
	str	r2, [r5, r0]
	str	r4, [r5, r10]
	add	r0, r3, #1
	str	lr, [r5, r9]
	ldr	r2, [r8, #2144]
	cmp	r0, r2
	ldrge	r1, [r1, #16]
	bge	.L15
	mov	r3, r0, asl #5
	ldr	r1, [r1, #16]
	sub	r3, r3, r0, asl #2
	add	lr, r7, r3
	ldr	lr, [lr, #16]
	cmp	r1, lr
	bcc	.L15
	sub	r3, r3, #28
	add	r3, r7, r3
	b	.L16
.L18:
	ldr	lr, [r3, #44]
	cmp	lr, r1
	bhi	.L20
.L16:
	add	r0, r0, #1
	add	r3, r3, #28
	cmp	r0, r2
	bne	.L18
.L19:
	ldr	lr, [fp, #-52]
	mov	r0, r2
	ldrh	r3, [lr, #56]
	ldrh	r2, [lr, #58]
	mov	lr, #0
	mul	r3, r2, r3
	sub	r3, r3, #1
.L21:
	add	r2, ip, #16
	add	r6, r6, #1
	add	ip, ip, #252
	uxth	r1, r1
	orr	r3, r1, r3, asl #16
	str	r3, [r5, r2]
	str	lr, [r5, ip]
.L7:
	sub	r3, r0, #1
.L10:
	ldr	r2, [r8, #2144]
	add	r3, r3, #1
	cmp	r2, r3
	bgt	.L22
.L2:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L15:
	cmp	r0, r2
	beq	.L19
.L20:
	mov	r3, r0, asl #5
	ldr	r2, [fp, #-56]
	sub	r3, r3, r0, asl #2
	add	r3, r7, r3
	add	lr, r2, r0, lsl #8
	ldr	r3, [r3, #16]
	sub	r3, r3, #1
	b	.L21
.L27:
	mov	lr, #0
	mov	r2, r10
	mov	r0, r4
	mov	ip, r9
.L30:
	mov	r4, lr
	b	.L11
.L26:
	mov	lr, #0
	mov	r4, lr
	b	.L11
.L33:
	.align	2
.L32:
	.word	g_HwMem
	UNWIND(.fnend)
	.size	AVSHAL_V400R003_WirteSlicMsg, .-AVSHAL_V400R003_WirteSlicMsg
	.align	2
	.global	AVSHAL_V400R003_StartDec
	.type	AVSHAL_V400R003_StartDec, %function
AVSHAL_V400R003_StartDec:
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
	UNWIND(.pad #28)
	sub	sp, sp, #28
	ldr	r5, .L81
	mov	r4, r0
	clz	r0, r2
	mov	ip, r1, asl #6
	ldr	r3, .L81+4
	ldrb	lr, [r5]	@ zero_extendqisi2
	mov	r0, r0, lsr #5
	sub	ip, ip, r1, asl #3
	mov	r6, r2
	cmp	lr, #1
	add	r3, r3, ip
	moveq	r0, #0
	cmp	r0, #0
	ldr	r7, [r3, #8]
	bne	.L76
	cmp	r1, #0
	bgt	.L77
	ldrh	r3, [r4, #56]
	cmp	r3, #512
	bhi	.L39
	ldrh	r3, [r4, #58]
	cmp	r3, #512
	bhi	.L39
	cmp	r1, #0
	bne	.L74
	ldr	r9, .L81+8
	ldr	r3, [r9]
	cmp	r3, #0
	beq	.L78
.L43:
	ldr	r3, .L81+12
	ldr	r3, [r3, r7, asl #2]
	add	r7, r4, #12288
	cmp	r3, #0
	ldrne	r3, [r3, #1224]
	cmp	lr, #1
	str	r3, [fp, #-56]
	ldrneb	r3, [fp, #-56]	@ zero_extendqisi2
	strneb	r3, [r6, #1]
	ldr	r0, [r7, #2144]
	cmp	r0, #0
	ble	.L47
	mov	r3, r4
	mov	r1, #0
	mvn	r5, #0
.L48:
	ldr	r2, [r3, #96]
	cmp	r2, #0
	beq	.L51
	ldr	ip, [r3, #104]
	bic	r2, r2, #15
	cmp	ip, #0
	ble	.L51
	cmp	r5, r2
	movcs	r5, r2
.L51:
	ldr	r2, [r3, #100]
	cmp	r2, #0
	beq	.L49
	ldr	ip, [r3, #108]
	bic	r2, r2, #15
	cmp	ip, #0
	ble	.L49
	cmp	r5, r2
	movcs	r5, r2
.L49:
	add	r1, r1, #1
	add	r3, r3, #28
	cmp	r1, r0
	bne	.L48
	cmn	r5, #1
	beq	.L47
	ldrh	r0, [r4, #56]
	mov	r8, #0
	ldrh	ip, [r4, #58]
	mov	r1, r8
	mov	lr, #65
	bfi	lr, r8, #1, #1
	mov	r3, r6
	mov	r2, r8
	mul	ip, ip, r0
	mov	r0, #8
	movw	r10, #3075
	movt	r10, 48
	sub	ip, ip, #1
	bfi	r1, ip, #0, #20
	str	r1, [fp, #-48]
	strb	lr, [fp, #-45]
	mov	r1, r1, lsr #16
	orr	r1, r1, #64
	bfi	r1, r8, #7, #1
	strb	r1, [fp, #-46]
	ldr	ip, [fp, #-48]
	mov	r1, ip
	str	ip, [fp, #-60]
	bl	MFDE_ConfigReg
	ldr	ip, [fp, #-60]
	ldr	r1, .L81+16
	mov	r0, #3
	mov	r2, ip
	bl	dprint_vfmw
	mov	r3, #1
	mov	r1, #12288	@ movhi
	bfi	r1, r3, #0, #12
	ldr	ip, [r7, #2604]
	mov	r2, #0
	uxth	r3, r1
	strh	r1, [fp, #-46]	@ movhi
	bfi	r2, ip, #4, #1
	ldr	ip, [fp, #-56]
	ldrb	r1, [r4, #16]	@ zero_extendqisi2
	mov	r3, r3, lsr #8
	bfi	r3, ip, #4, #1
	and	r2, r2, #16
	cmp	r1, #3
	cmpne	r1, #1
	bfi	r3, r8, #5, #1
	ldr	r0, [r7, #2600]
	orr	r2, r2, #64
	movls	r1, #1
	movhi	r1, #0
	bfi	r2, r1, #7, #1
	uxtb	r1, r3
	bfi	r1, r0, #6, #1
	strb	r2, [fp, #-47]
	mov	r3, #6
	bfi	r1, r8, #7, #1
	strb	r3, [fp, #-48]
	mov	r2, r8
	strb	r1, [fp, #-45]
	mov	r3, r6
	ldr	ip, [fp, #-48]
	mov	r0, #12
	mov	r1, ip
	str	ip, [fp, #-60]
	bl	MFDE_ConfigReg
	ldr	ip, [fp, #-60]
	ldr	r1, .L81+20
	mov	r0, #3
	mov	r2, ip
	bl	dprint_vfmw
	ldr	ip, [r9, #56]
	mov	r3, r6
	mov	r2, r8
	bic	ip, ip, #15
	mov	r0, #16
	str	ip, [fp, #-60]
	mov	r1, ip
	bl	MFDE_ConfigReg
	ldr	ip, [fp, #-60]
	ldr	r1, .L81+24
	mov	r0, #3
	mov	r2, ip
	bl	dprint_vfmw
	ldr	ip, [r9, #40]
	mov	r3, r6
	mov	r2, r8
	bic	ip, ip, #15
	mov	r0, #20
	str	ip, [fp, #-60]
	mov	r1, ip
	bl	MFDE_ConfigReg
	ldr	ip, [fp, #-60]
	ldr	r1, .L81+28
	mov	r0, #3
	mov	r2, ip
	bl	dprint_vfmw
	mov	r3, r6
	mov	r2, r8
	mov	r1, r5
	mov	r0, #24
	str	r5, [fp, #-48]
	bl	MFDE_ConfigReg
	mov	r2, r5
	ldr	r1, .L81+32
	mov	r0, #3
	bl	dprint_vfmw
	ldrh	r1, [r4, #56]
	mov	r2, r8
	mov	r3, r6
	cmp	r1, #120
	mov	r0, #4
	movhi	r1, #0
	movls	r1, #1
	bl	SCD_ConfigReg
	mov	r1, r10
	mov	r2, r8
	mov	r3, r6
	mov	r0, #60
	str	r10, [fp, #-48]
	bl	MFDE_ConfigReg
	mov	r3, r6
	mov	r2, r8
	mov	r1, r10
	mov	r0, #64
	bl	MFDE_ConfigReg
	mov	r3, r6
	mov	r2, r8
	mov	r1, r10
	mov	r0, #68
	bl	MFDE_ConfigReg
	mov	r3, r6
	mov	r2, r8
	mov	r1, r10
	mov	r0, #72
	bl	MFDE_ConfigReg
	mov	r3, r6
	mov	r2, r8
	mov	r1, r10
	mov	r0, #76
	bl	MFDE_ConfigReg
	mov	r3, r6
	mov	r2, r8
	mov	r1, r10
	mov	r0, #80
	bl	MFDE_ConfigReg
	mov	r3, r6
	mov	r2, r8
	mov	r1, r10
	mov	r0, #84
	bl	MFDE_ConfigReg
	ldr	ip, [fp, #-56]
	cmp	ip, #1
	beq	.L79
.L55:
	ldr	ip, [r7, #2160]
	mov	r3, r6
	mov	r2, #0
	mov	r0, #96
	bic	ip, ip, #15
	str	ip, [fp, #-56]
	mvn	r8, #0
	mov	r1, ip
	bl	MFDE_ConfigReg
	ldr	ip, [fp, #-56]
	ldr	r1, .L81+36
	mov	r0, #3
	mov	r2, ip
	bl	dprint_vfmw
	ldr	ip, [r7, #2564]
	mov	r3, r6
	mov	r2, #0
	mov	r0, #100
	mov	r1, ip
	str	ip, [fp, #-56]
	bl	MFDE_ConfigReg
	ldr	ip, [fp, #-56]
	ldr	r1, .L81+40
	mov	r0, #3
	mov	r2, ip
	bl	dprint_vfmw
	ldr	ip, [r7, #2572]
	mov	r3, r6
	mov	r2, #0
	mov	r0, #104
	mov	r1, ip
	str	ip, [fp, #-56]
	bl	MFDE_ConfigReg
	ldr	ip, [fp, #-56]
	ldr	r1, .L81+44
	mov	r0, #3
	mov	r2, ip
	bl	dprint_vfmw
	ldr	ip, [r7, #2580]
	mov	r3, r6
	mov	r2, #0
	mov	r0, #108
	mov	r1, ip
	str	ip, [fp, #-56]
	bl	MFDE_ConfigReg
	ldr	ip, [fp, #-56]
	ldr	r1, .L81+48
	mov	r0, #3
	mov	r2, ip
	bl	dprint_vfmw
	ldr	ip, [r9, #1204]
	mov	r3, r6
	mov	r2, #0
	mov	r0, #144
	mov	r1, ip
	str	ip, [fp, #-56]
	bl	MFDE_ConfigReg
	ldr	ip, [fp, #-56]
	ldr	r1, .L81+52
	mov	r0, #3
	mov	r2, ip
	bl	dprint_vfmw
	mov	r2, #0
	mov	r3, r6
	mov	r1, r2
	mov	r0, #148
	bl	MFDE_ConfigReg
	mov	r2, #0
	ldr	r1, .L81+56
	mov	r0, #3
	bl	dprint_vfmw
	mov	r2, #0
	mov	r3, r6
	mov	r1, r2
	mov	r0, #152
	bl	MFDE_ConfigReg
	mov	r2, #0
	ldr	r1, .L81+60
	mov	r0, #3
	bl	dprint_vfmw
	mov	r3, r6
	mov	r1, r8
	mov	r2, #0
	mov	r0, #32
	str	r8, [fp, #-48]
	bl	MFDE_ConfigReg
	ldr	r0, [r9, #56]
	bl	MEM_Phy2Vir
	subs	r6, r0, #0
	beq	.L80
	ldrh	r3, [r4, #58]
	mov	r0, #4
	ldrh	r1, [fp, #-46]
	mov	r8, #0
	sub	r3, r3, #1
	ldrh	lr, [r4, #56]
	bfi	r1, r3, #0, #9
	ldrb	ip, [r4, #40]	@ zero_extendqisi2
	ldrh	r2, [fp, #-48]
	sub	lr, lr, #1
	mov	r3, r1, lsr #8
	strh	r1, [fp, #-46]	@ movhi
	bfi	r2, lr, #0, #9
	bfi	r3, ip, #1, #2
	ldrb	ip, [r4, #39]	@ zero_extendqisi2
	mov	r1, r2, lsr #8
	strh	r2, [fp, #-48]	@ movhi
	bfi	r3, ip, #3, #2
	bfc	r1, #1, #7
	bfc	r3, #5, #3
	strb	r1, [fp, #-47]
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	ldr	r1, .L81+64
	str	r2, [r6]
	bl	dprint_vfmw
	ldrb	r3, [r4, #25]	@ zero_extendqisi2
	ldrb	r1, [r4, #17]	@ zero_extendqisi2
	mov	r0, #4
	ldrb	r2, [r4, #16]	@ zero_extendqisi2
	and	r3, r3, #1
	ldrb	ip, [r4, #24]	@ zero_extendqisi2
	bfi	r3, r1, #1, #2
	bfi	r3, r2, #3, #2
	ldrb	r1, [r4, #19]	@ zero_extendqisi2
	ldrb	lr, [r4, #18]	@ zero_extendqisi2
	bfi	r3, ip, #5, #1
	ldrb	r2, [r4, #21]	@ zero_extendqisi2
	and	r1, r1, #1
	ldrb	ip, [r4, #23]	@ zero_extendqisi2
	bfi	r1, lr, #1, #1
	ldrb	r10, [r4, #20]	@ zero_extendqisi2
	and	r2, r2, #63
	ldrb	lr, [r4, #31]	@ zero_extendqisi2
	bfi	r3, ip, #6, #1
	str	r8, [fp, #-48]
	bfi	r2, r10, #6, #1
	strb	r1, [fp, #-46]
	bfi	r2, lr, #7, #1
	ldrb	r1, [r4, #22]	@ zero_extendqisi2
	ldrh	ip, [fp, #-46]
	bfi	r3, r1, #7, #1
	strb	r2, [fp, #-47]
	bfi	ip, r8, #2, #14
	strb	r3, [fp, #-48]
	strh	ip, [fp, #-46]	@ movhi
	ldr	r2, [fp, #-48]
	ldr	r1, .L81+68
	str	r2, [r6, #4]
	bl	dprint_vfmw
	ldrb	r3, [r4, #29]	@ zero_extendqisi2
	ldrb	r2, [r4, #28]	@ zero_extendqisi2
	mov	r0, #4
	ldrb	r1, [r4, #27]	@ zero_extendqisi2
	and	r3, r3, #1
	bfi	r3, r2, #1, #1
	str	r8, [fp, #-48]
	ldrb	r2, [r4, #26]	@ zero_extendqisi2
	bfi	r3, r1, #2, #5
	strb	r3, [fp, #-48]
	ldrh	r3, [fp, #-48]
	ldr	r1, .L81+72
	bfi	r3, r2, #7, #5
	strh	r3, [fp, #-48]	@ movhi
	ldr	r2, [fp, #-48]
	str	r2, [r6, #8]
	bl	dprint_vfmw
	ldrb	r3, [r4, #34]	@ zero_extendqisi2
	ldrb	r2, [r4, #35]	@ zero_extendqisi2
	mov	r0, #4
	ldrb	ip, [r4, #36]	@ zero_extendqisi2
	and	r3, r3, #1
	ldrb	r1, [r4, #37]	@ zero_extendqisi2
	bfi	r3, r2, #1, #2
	bfi	r3, ip, #3, #1
	ldrb	r2, [r4, #32]	@ zero_extendqisi2
	bfi	r3, r1, #4, #2
	ldrb	ip, [r4, #33]	@ zero_extendqisi2
	str	r8, [fp, #-48]
	bfi	r3, r2, #6, #1
	strb	r3, [fp, #-48]
	ldrh	r3, [fp, #-48]
	ldr	r1, .L81+76
	bfi	r3, ip, #7, #2
	strh	r3, [fp, #-48]	@ movhi
	ldr	r2, [fp, #-48]
	bfi	r2, r8, #9, #23
	str	r2, [fp, #-48]
	str	r2, [r6, #12]
	bl	dprint_vfmw
	ldr	r2, [r7, #2148]
	ldr	r1, .L81+80
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r6, #16]
	bl	dprint_vfmw
	ldr	r2, [r7, #2152]
	ldr	r1, .L81+84
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r6, #20]
	bl	dprint_vfmw
	ldr	r2, [r7, #2156]
	ldr	r1, .L81+88
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r6, #24]
	bl	dprint_vfmw
	ldr	r2, [r7, #2172]
	ldr	r1, .L81+92
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r6, #28]
	bl	dprint_vfmw
	ldrb	r3, [r4, #38]	@ zero_extendqisi2
	ldrb	r1, [r4, #30]	@ zero_extendqisi2
	mov	r2, #0
	str	r8, [fp, #-48]
	bfi	r2, r3, #0, #1
	mov	r3, #0
	strb	r2, [fp, #-48]
	bfi	r3, r1, #0, #2
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	mov	r0, #4
	ldr	r1, .L81+96
	str	r2, [r6, #32]
	bl	dprint_vfmw
	ldr	r2, [r4, #72]
	mov	r3, r8
	ldr	r1, .L81+100
	bfi	r3, r2, #0, #24
	mov	r0, #4
	str	r3, [fp, #-48]
	mov	r2, r3
	str	r3, [r6, #36]
	bl	dprint_vfmw
	ldr	r1, [r4, #88]
	ldr	r2, [r4, #80]
	mov	r3, r8
	bfi	r3, r1, #0, #24
	str	r3, [fp, #-48]
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	mov	r0, #4
	ldr	r1, .L81+104
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r6, #40]
	bl	dprint_vfmw
	ldr	r2, [r4, #76]
	mov	r3, r8
	ldr	r1, .L81+108
	bfi	r3, r2, #0, #24
	mov	r0, #4
	str	r3, [fp, #-48]
	mov	r2, r3
	str	r3, [r6, #44]
	bl	dprint_vfmw
	ldrb	r2, [r4, #41]	@ zero_extendqisi2
	ldrb	r3, [r4, #43]	@ zero_extendqisi2
	mov	r1, #0
	ldrb	ip, [r4, #42]	@ zero_extendqisi2
	and	r2, r2, #1
	ldrb	r0, [r4, #45]	@ zero_extendqisi2
	and	r3, r3, #63
	ldrb	lr, [r4, #44]	@ zero_extendqisi2
	bfi	r2, ip, #1, #1
	bfi	r3, r0, #6, #1
	ldrb	ip, [r4, #46]	@ zero_extendqisi2
	ldrb	r0, [r4, #47]	@ zero_extendqisi2
	bfi	r2, lr, #2, #6
	bfi	r3, ip, #7, #1
	str	r8, [fp, #-48]
	bfi	r1, r0, #0, #1
	strb	r3, [fp, #-47]
	strb	r1, [fp, #-46]
	mov	r0, #4
	strb	r2, [fp, #-48]
	ldr	r2, [fp, #-48]
	ldr	r1, .L81+112
	str	r2, [r6, #48]
	bl	dprint_vfmw
	ldr	r3, .L81+8
	ldr	r1, .L81+116
	mov	r0, #4
	ldr	r2, [r3, #1148]
	bic	r2, r2, #15
	str	r2, [r6, #52]
	bl	dprint_vfmw
	ldr	r3, .L81+8
	ldr	r1, .L81+120
	mov	r0, #4
	ldr	r2, [r3, #1152]
	bic	r2, r2, #15
	str	r2, [r6, #56]
	bl	dprint_vfmw
	ldr	r2, [r7, #2168]
	ldr	r1, .L81+124
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r6, #64]
	bl	dprint_vfmw
	add	r3, r4, #14464
	add	r2, r6, #68
	add	r0, r4, #14592
.L57:
	ldr	r1, [r3, #4]!
	cmp	r3, r0
	str	r1, [r2], #4
	bne	.L57
	ldr	r2, [r7, #2164]
	mov	r0, #4
	ldr	r1, .L81+128
	bic	r2, r2, #15
	str	r2, [r6, #196]
	bl	dprint_vfmw
	ldr	r2, [r9, #1160]
	ldr	r1, .L81+132
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r6, #200]
	bl	dprint_vfmw
	ldr	r2, [r9, #1144]
	ldr	r1, .L81+136
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r6, #204]
	bl	dprint_vfmw
	ldr	r2, [r9, #56]
	ldr	r1, .L81+140
	mov	r0, #4
	add	r2, r2, #320
	str	r2, [r6, #252]
	str	r2, [fp, #-48]
	bl	dprint_vfmw
	ldrb	lr, [r4, #41]	@ zero_extendqisi2
	cmp	lr, #1
	beq	.L58
.L62:
	mov	r0, r4
	mov	r1, r5
	mov	r2, #0
	bl	AVSHAL_V400R003_WirteSlicMsg
	mov	r0, #0
.L36:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L58:
	add	ip, r4, #14592
	add	r7, r6, #256
	add	ip, ip, #28
	add	r6, r6, #320
.L61:
	tst	lr, #1
	add	ip, ip, #16
	add	lr, lr, #1
	ldrneb	r1, [ip, #-24]	@ zero_extendqisi2
	ldreqb	r1, [ip, #-36]	@ zero_extendqisi2
	ldrneb	r3, [ip, #-16]	@ zero_extendqisi2
	ldreqb	r3, [ip, #-28]	@ zero_extendqisi2
	movne	r1, r1, asl #16
	ldrneb	r2, [ip, #-40]	@ zero_extendqisi2
	moveq	r1, r1, asl #16
	ldrneb	r0, [ip, #-32]	@ zero_extendqisi2
	ldreqb	r2, [ip, #-52]	@ zero_extendqisi2
	orr	r3, r1, r3, asl #24
	ldreqb	r0, [ip, #-44]	@ zero_extendqisi2
	orr	r3, r3, r2
	orr	r3, r3, r0, asl #8
	str	r3, [r7], #4
	cmp	r7, r6
	bne	.L61
	b	.L62
.L79:
	mov	r3, r6
	mov	r2, r8
	mov	r1, #60
	mov	r0, #92
	bl	MFDE_ConfigReg
	mov	r2, #60
	ldr	r1, .L81+144
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r10, [r7, #2568]
	mov	r2, r8
	mov	r3, r6
	mov	r0, #112
	mov	r1, r10
	bl	MFDE_ConfigReg
	mov	r2, r10
	ldr	r1, .L81+148
	mov	r0, #3
	bl	dprint_vfmw
	b	.L55
.L78:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L44
	str	r3, [r9]
	ldrb	lr, [r5]	@ zero_extendqisi2
	b	.L43
.L77:
	mov	r3, r1
	str	r0, [sp]
	ldr	r2, .L81+152
	ldr	r1, .L81+156
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L36
.L39:
	ldr	r3, .L81+160
	mov	r0, #0
	ldr	r2, .L81+152
	ldr	r1, .L81+164
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L36
.L74:
	ldr	r1, .L81+168
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L36
.L47:
	mvn	r2, #0
	ldr	r1, .L81+172
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L36
.L80:
	ldr	r3, .L81+176
	ldr	r2, .L81+152
	ldr	r1, .L81+164
	bl	dprint_vfmw
	mov	r0, r8
	b	.L36
.L76:
	mov	r3, r2
	ldr	r1, .L81+180
	ldr	r2, .L81+152
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L36
.L44:
	ldr	r1, .L81+184
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L36
.L82:
	.align	2
.L81:
	.word	g_HalDisable
	.word	g_VdmDrvParam
	.word	g_HwMem
	.word	s_pstVfmwChan
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.word	.LC25
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.word	.LC38
	.word	.LC39
	.word	.LC40
	.word	.LC41
	.word	.LC12
	.word	.LC13
	.word	.LANCHOR0
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC6
	.word	.LC21
	.word	.LC0
	.word	.LC5
	UNWIND(.fnend)
	.size	AVSHAL_V400R003_StartDec, .-AVSHAL_V400R003_StartDec
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.14708, %object
	.size	__func__.14708, 25
__func__.14708:
	.ascii	"AVSHAL_V400R003_StartDec\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"%s: pMfdeTask(%p) = NULL\012\000" )
	.space	2
.LC1:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC2:
	ASCII(.ascii	"picture width out of range\000" )
	.space	1
.LC3:
	ASCII(.ascii	"%s: %s\012\000" )
.LC4:
	ASCII(.ascii	"VdhId is wrong! AVSHAL_V200R003_StartDec\012\000" )
	.space	2
.LC5:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
.LC6:
	ASCII(.ascii	"stream_base_addr = %#x\012\000" )
.LC7:
	ASCII(.ascii	"BASIC_V400R003_CFG0 = 0x%x\012\000" )
.LC8:
	ASCII(.ascii	"BASIC_V400R003_CFG1 = 0x%x\012\000" )
.LC9:
	ASCII(.ascii	"AVM_V400R003_ADDR = 0x%x\012\000" )
	.space	2
.LC10:
	ASCII(.ascii	"VAM_V400R003_ADDR = 0x%x\012\000" )
	.space	2
.LC11:
	ASCII(.ascii	"STREAM_V400R003_BASE_ADDR = 0x%x\012\000" )
	.space	2
.LC12:
	ASCII(.ascii	"VREG_V200R003_PART_DEC_OVER_INT_LEVEL=0x%x\012\000" )
.LC13:
	ASCII(.ascii	"VREG_LINE_NUM_STADDR = 0x%x\012\000" )
	.space	3
.LC14:
	ASCII(.ascii	"YSTADDR_V400R003_1D = 0x%x\012\000" )
.LC15:
	ASCII(.ascii	"YSTRIDE_V400R003_1D = 0x%x\012\000" )
.LC16:
	ASCII(.ascii	"UVOFFSET_V400R003_1D = 0x%x\012\000" )
	.space	3
.LC17:
	ASCII(.ascii	"HEAD_INF_OFFSET = 0x%x\012\000" )
.LC18:
	ASCII(.ascii	"VREG_V400R003_DNR_MBINFO_STADDR = 0x%x\012\000" )
.LC19:
	ASCII(.ascii	"VREG_V200R003_REF_PIC_TYPE = 0x%x\012\000" )
	.space	1
.LC20:
	ASCII(.ascii	"VREG_V400R003_FF_APT_EN = 0x%x\012\000" )
.LC21:
	ASCII(.ascii	"can not map down msg virtual address!\000" )
	.space	2
.LC22:
	ASCII(.ascii	"D0 = 0x%x\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"D1 = 0x%x\012\000" )
	.space	1
.LC24:
	ASCII(.ascii	"D2 = 0x%x\012\000" )
	.space	1
.LC25:
	ASCII(.ascii	"D3 = 0x%x\012\000" )
	.space	1
.LC26:
	ASCII(.ascii	"D4 = 0x%x\012\000" )
	.space	1
.LC27:
	ASCII(.ascii	"D5 = 0x%x\012\000" )
	.space	1
.LC28:
	ASCII(.ascii	"D6 = 0x%x\012\000" )
	.space	1
.LC29:
	ASCII(.ascii	"D7 = 0x%x\012\000" )
	.space	1
.LC30:
	ASCII(.ascii	"D8 = 0x%x\012\000" )
	.space	1
.LC31:
	ASCII(.ascii	"D9 = 0x%x\012\000" )
	.space	1
.LC32:
	ASCII(.ascii	"D10 = 0x%x\012\000" )
.LC33:
	ASCII(.ascii	"D11 = 0x%x\012\000" )
.LC34:
	ASCII(.ascii	"D12 = 0x%x\012\000" )
.LC35:
	ASCII(.ascii	"D13 = 0x%x\012\000" )
.LC36:
	ASCII(.ascii	"D14 = 0x%x\012\000" )
.LC37:
	ASCII(.ascii	"D16 = 0x%x\012\000" )
.LC38:
	ASCII(.ascii	"D49 = 0x%x\012\000" )
.LC39:
	ASCII(.ascii	"D50 = 0x%x\012\000" )
.LC40:
	ASCII(.ascii	"D51 = 0x%x\012\000" )
.LC41:
	ASCII(.ascii	"D63 = 0x%x\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
