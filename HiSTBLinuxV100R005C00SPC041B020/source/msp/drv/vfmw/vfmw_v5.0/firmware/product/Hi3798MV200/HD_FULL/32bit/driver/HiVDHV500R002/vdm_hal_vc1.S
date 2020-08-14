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
	.file	"vdm_hal_vc1.c"
	.text
	.align	2
	.global	VC1HAL_V400R003_InitHal
	.type	VC1HAL_V400R003_InitHal, %function
VC1HAL_V400R003_InitHal:
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
	.size	VC1HAL_V400R003_InitHal, .-VC1HAL_V400R003_InitHal
	.align	2
	.global	VC1HAL_V400R003_CfgSliceMsg
	.type	VC1HAL_V400R003_CfgSliceMsg, %function
VC1HAL_V400R003_CfgSliceMsg:
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
	ldr	r3, [r1, #60]
	add	r2, r0, #84
	mov	r10, r0
	str	r2, [fp, #-52]
	mov	r0, r3
	str	r3, [fp, #-56]
	bl	MEM_Phy2Vir
	subs	r4, r0, #0
	beq	.L61
	ldrb	r3, [r10, #4]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L62
	ldr	lr, [r10, #96]
	add	r3, r10, #12288
	str	r3, [fp, #-48]
	bic	r8, lr, #15
	str	r8, [r3, #172]
	ldrb	r0, [r10, #70]	@ zero_extendqisi2
	cmp	r0, #0
	ble	.L2
	mov	r1, #0
	mov	ip, r4
	mov	r7, r1
	cmp	r7, #0
	add	r6, r10, #144
	mov	r4, r1
	bne	.L26
.L63:
	cmp	r0, #1
	ldr	r4, [r10, #108]
	beq	.L47
	ble	.L28
	ldr	r3, [r10, #156]
	cmp	r4, r3
	blt	.L28
	ldr	r3, [fp, #-52]
	mov	r1, #1
	b	.L29
.L31:
	ldr	r2, [r3, #72]
	cmp	r4, r2
	blt	.L41
.L29:
	add	r1, r1, #1
	add	r3, r3, #48
	cmp	r1, r0
	bne	.L31
.L58:
	mov	r2, #0
.L32:
	mov	r3, #0
.L27:
	ldr	r5, [r6, #-44]
	rsb	lr, r8, lr
	ldr	r0, [r6, #-40]
	add	r3, r3, r4, lsl #16
	add	r5, r5, lr, lsl #3
	bic	lr, lr, #15
	add	r7, r7, #1
	add	r6, r6, #48
	add	r0, r0, r5, lsl #25
	str	r0, [ip]
	str	lr, [ip, #4]
	mov	r0, #0
	str	r0, [ip, #8]
	add	ip, ip, #256
	str	r0, [ip, #-244]
	str	r3, [ip, #-240]
	str	r2, [ip, #-4]
	ldrb	r0, [r10, #70]	@ zero_extendqisi2
	cmp	r0, r7
	ble	.L2
	ldr	r3, [fp, #-48]
	cmp	r7, #0
	ldr	lr, [r6, #-48]
	ldr	r8, [r3, #172]
	beq	.L63
.L26:
	ldr	r5, [fp, #-52]
	mov	r3, r1, asl #6
	add	r2, r1, #1
	sub	r3, r3, r1, asl #4
	add	r3, r5, r3
	cmp	r2, r0
	ldr	r5, [r3, #24]
	bge	.L33
	ldr	r9, [r3, #72]
	cmp	r5, r9
	blt	.L33
	mov	r9, r1
	b	.L34
.L36:
	ldr	r1, [r3, #72]
	cmp	r5, r1
	blt	.L59
.L34:
	add	r2, r2, #1
	add	r3, r3, #48
	cmp	r2, r0
	bne	.L36
	mov	r1, r9
	mov	r2, #0
.L37:
	add	r3, r4, #1
	mov	r4, r5
	b	.L27
.L33:
	cmp	r2, r0
	moveq	r2, #0
	beq	.L37
.L59:
	ldr	r3, [fp, #-56]
	mov	r1, r2
	add	r2, r3, r2, lsl #8
	b	.L37
.L28:
	cmp	r0, #1
	beq	.L48
	mov	r1, #1
.L41:
	ldr	r3, [fp, #-56]
	add	r2, r3, r1, lsl #8
	b	.L32
.L47:
	mov	r2, r7
	mov	r3, r7
	b	.L27
.L62:
	ldr	r0, [r10, #96]
	add	r3, r10, #12288
	str	r3, [fp, #-48]
	mov	lr, r3
	str	r0, [r3, #172]
	ldrb	r2, [r10, #70]	@ zero_extendqisi2
	cmp	r2, #0
	ble	.L5
	mov	r3, r10
	mov	r1, #0
.L8:
	ldr	ip, [r3, #88]
	cmp	ip, #0
	beq	.L6
	ldr	ip, [r3, #112]
	cmp	ip, r0
	strcc	ip, [lr, #172]
	movcc	r0, ip
	bcc	.L7
.L6:
	ldr	ip, [r3, #96]
	cmp	ip, r0
	strcc	ip, [lr, #172]
	movcc	r0, ip
.L7:
	add	r1, r1, #1
	add	r3, r3, #48
	cmp	r1, r2
	bne	.L8
.L5:
	ldr	r3, [fp, #-48]
	bic	r0, r0, #15
	ldr	r1, .L64
	str	r0, [r3, #172]
	mov	r0, #4
	bl	dprint_vfmw
	ldrb	lr, [r10, #70]	@ zero_extendqisi2
	cmp	lr, #0
	ble	.L2
	mov	r5, #0
	mov	r9, r10
	str	r10, [fp, #-84]
	mov	r8, r4
	mov	r6, r5
	mov	r10, r5
.L24:
	ldr	r3, [fp, #-48]
	ldr	r0, [r9, #96]
	ldr	r1, [r9, #88]
	ldr	r3, [r3, #172]
	ldr	r4, [r9, #100]
	cmp	r1, #0
	rsb	r0, r3, r0
	and	r2, r0, #15
	bic	r0, r0, #15
	str	r0, [fp, #-60]
	add	ip, r4, r2, lsl #3
	ldr	r0, [r9, #104]
	beq	.L43
	ldr	r7, [r9, #112]
	ldr	r2, [r9, #116]
	rsb	r7, r3, r7
	ldr	r3, [r9, #120]
	str	r7, [fp, #-72]
	add	r2, r2, r7, lsl #3
	add	r3, r3, r2, lsl #25
	str	r3, [fp, #-64]
	str	r3, [fp, #-68]
.L11:
	cmp	r10, #0
	bne	.L12
	ldr	r3, [fp, #-84]
	cmp	lr, #1
	ldr	r6, [r3, #108]
	beq	.L44
	ble	.L14
	ldr	r3, [r3, #156]
	cmp	r6, r3
	blt	.L14
	ldr	r3, [fp, #-52]
	mov	r5, #1
	b	.L15
.L17:
	ldr	r2, [r3, #72]
	cmp	r6, r2
	blt	.L39
.L15:
	add	r5, r5, #1
	add	r3, r3, #48
	cmp	r5, lr
	bne	.L17
	mov	r3, #0
	mov	r4, r3
.L18:
	mov	r2, #0
	str	r2, [fp, #-80]
.L13:
	add	r2, r0, ip, lsl #25
	ldr	r1, .L64+4
	str	r2, [r8]
	mov	r0, #4
	str	r3, [fp, #-76]
	bl	dprint_vfmw
	ldr	r3, [fp, #-60]
	mov	r0, #4
	ldr	r1, .L64+8
	add	r10, r10, #1
	add	r9, r9, #48
	add	r8, r8, #256
	str	r3, [r8, #-252]
	mov	r2, r3
	bl	dprint_vfmw
	ldr	r3, [fp, #-68]
	ldr	r2, [fp, #-64]
	mov	r0, #4
	ldr	r1, .L64+12
	str	r3, [r8, #-248]
	bl	dprint_vfmw
	ldr	r3, [fp, #-72]
	mov	r2, r7
	ldr	r1, .L64+16
	mov	r0, #4
	str	r3, [r8, #-244]
	bl	dprint_vfmw
	ldr	r3, [fp, #-80]
	ldr	r1, .L64+20
	mov	r0, #4
	add	r2, r3, r6, lsl #16
	str	r2, [r8, #-240]
	bl	dprint_vfmw
	ldr	r3, [fp, #-76]
	mov	r2, r4
	ldr	r1, .L64+24
	mov	r0, #4
	str	r3, [r8, #-4]
	bl	dprint_vfmw
	ldr	r3, [fp, #-84]
	ldrb	lr, [r3, #70]	@ zero_extendqisi2
	cmp	lr, r10
	bgt	.L24
.L2:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L12:
	ldr	r2, [fp, #-52]
	mov	r3, r5, asl #6
	add	r4, r5, #1
	sub	r3, r3, r5, asl #4
	add	r3, r2, r3
	cmp	r4, lr
	ldr	r2, [r3, #24]
	bge	.L19
	ldr	r1, [r3, #72]
	cmp	r2, r1
	bge	.L20
	b	.L19
.L22:
	ldr	r1, [r3, #72]
	cmp	r2, r1
	blt	.L57
.L20:
	add	r4, r4, #1
	add	r3, r3, #48
	cmp	r4, lr
	bne	.L22
.L46:
	mov	r3, #0
	mov	r4, r3
.L23:
	add	r1, r6, #1
	mov	r6, r2
	str	r1, [fp, #-80]
	b	.L13
.L19:
	cmp	r4, lr
	beq	.L46
.L57:
	ldr	r3, [fp, #-56]
	mov	r5, r4
	add	r4, r3, r4, lsl #8
	mov	r3, r4
	b	.L23
.L14:
	cmp	lr, #1
	beq	.L45
	mov	r5, #1
.L39:
	ldr	r3, [fp, #-56]
	add	r4, r3, r5, lsl #8
	mov	r3, r4
	b	.L18
.L43:
	str	r1, [fp, #-72]
	mov	r7, r1
	str	r1, [fp, #-68]
	str	r1, [fp, #-64]
	b	.L11
.L44:
	mov	r3, r10
	str	r10, [fp, #-80]
	mov	r4, r10
	b	.L13
.L61:
	ldr	r2, .L64+28
	ldr	r1, .L64+32
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L45:
	mov	r3, #0
	mov	r5, lr
	mov	r4, r3
	b	.L18
.L48:
	mov	r1, r0
	b	.L58
.L65:
	.align	2
.L64:
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LANCHOR0
	.word	.LC0
	UNWIND(.fnend)
	.size	VC1HAL_V400R003_CfgSliceMsg, .-VC1HAL_V400R003_CfgSliceMsg
	.align	2
	.global	VC1HAL_V400R003_CfgReg
	.type	VC1HAL_V400R003_CfgReg, %function
VC1HAL_V400R003_CfgReg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #16)
	sub	sp, sp, #16
	subs	r4, r2, #0
	mov	r9, #0
	mov	r7, r0
	mov	r8, r1
	mov	r5, r3
	str	r9, [fp, #-40]
	bgt	.L73
	add	r6, r0, #12288
	mov	r0, #8
	ldr	r1, [r6, #164]
	ubfx	r1, r1, #0, #20
	orr	r1, r1, #1090519040
	bl	MFDE_ConfigReg
	ldrh	r1, [fp, #-38]
	ldrh	ip, [r7, #78]
	mov	r2, #1
	bfi	r1, r2, #0, #12
	ldr	r0, [r6, #260]
	cmp	ip, #120
	strh	r1, [fp, #-38]	@ movhi
	mov	ip, r1, lsr #8
	ldrb	r3, [fp, #-39]	@ zero_extendqisi2
	bfi	ip, r9, #4, #1
	strb	ip, [fp, #-37]
	strleb	r2, [r7, #71]
	movle	r1, r9
	strgtb	r9, [r7, #71]
	bfi	r3, r0, #4, #1
	ldrgt	r1, [r6, #256]
	and	r3, r3, #223
	strle	r9, [r6, #256]
	movle	r9, r2
	ldrb	ip, [fp, #-37]	@ zero_extendqisi2
	andgt	r1, r1, #1
	ldrb	r0, [fp, #-40]	@ zero_extendqisi2
	orr	r3, r3, #192
	bfi	ip, r9, #5, #1
	strb	r3, [fp, #-39]
	bfi	ip, r1, #6, #1
	bfi	r0, r2, #0, #4
	bfc	ip, #7, #1
	strb	r0, [fp, #-40]
	strb	ip, [fp, #-37]
	mov	r3, r5
	ldr	r1, [fp, #-40]
	mov	r2, r4
	mov	r0, #12
	str	r1, [r6, #168]
	bl	MFDE_ConfigReg
	mov	r3, r5
	mov	r2, r4
	ldr	r1, [r8, #56]
	mov	r0, #16
	bl	MFDE_ConfigReg
	mov	r3, r5
	mov	r2, r4
	ldr	r1, [r8, #40]
	mov	r0, #20
	bl	MFDE_ConfigReg
	ldr	ip, [r6, #172]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #24
	mov	r1, ip
	str	ip, [fp, #-40]
	bl	MFDE_ConfigReg
	ldrh	r1, [r7, #78]
	mov	r3, r5
	mov	r2, r4
	cmp	r1, #256
	mov	r0, #4
	movhi	r1, #0
	movls	r1, #1
	bl	SCD_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #60
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #64
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #68
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #72
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #76
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #80
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #84
	movt	r1, 48
	bl	MFDE_ConfigReg
	mov	r3, r5
	mov	r2, r4
	ldr	r1, [r6, #224]
	mov	r0, #96
	bl	MFDE_ConfigReg
	ldr	r1, [r6, #176]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #100
	str	r1, [r6, #228]
	bl	MFDE_ConfigReg
	ldr	r1, [r6, #180]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #104
	str	r1, [r6, #232]
	bl	MFDE_ConfigReg
	mov	r3, r5
	mov	r2, r4
	ldrb	r1, [r7, #69]	@ zero_extendqisi2
	mov	r0, #152
	bl	MFDE_ConfigReg
	mov	r3, r5
	mov	r2, r4
	ldrb	r1, [r7, #68]	@ zero_extendqisi2
	mov	r0, #148
	bl	MFDE_ConfigReg
	mov	r3, r5
	mov	r2, r4
	ldr	r1, [r6, #188]
	mov	r0, #108
	bl	MFDE_ConfigReg
	mov	r3, r5
	mov	r2, r4
	mvn	r1, #0
	mov	r0, #32
	bl	MFDE_ConfigReg
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L73:
	str	r9, [sp]
	mov	r3, r4
	mov	r0, r9
	ldr	r2, .L74
	ldr	r1, .L74+4
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L75:
	.align	2
.L74:
	.word	.LANCHOR0+28
	.word	.LC8
	UNWIND(.fnend)
	.size	VC1HAL_V400R003_CfgReg, .-VC1HAL_V400R003_CfgReg
	.align	2
	.global	VC1HAL_V400R003_CfgDnMsg
	.type	VC1HAL_V400R003_CfgDnMsg, %function
VC1HAL_V400R003_CfgDnMsg:
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
	mov	r4, r0
	ldr	r0, [r1, #56]
	mov	r6, r1
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L80
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	mov	r2, #0
	ldrb	r0, [r4, #3]	@ zero_extendqisi2
	mov	r5, #0
	ldrb	ip, [r4, #5]	@ zero_extendqisi2
	bfi	r2, r1, #4, #2
	str	r5, [fp, #-48]
	bfi	r2, r0, #0, #2
	mov	r1, #0
	strb	r2, [fp, #-48]
	bfi	r1, ip, #6, #2
	strb	r1, [fp, #-47]
	ldr	r1, [fp, #-48]
	mov	r2, #0
	str	r5, [fp, #-48]
	mov	r8, r2
	strb	r2, [fp, #-60]
	mov	lr, r2
	str	r1, [r3]
	mov	ip, r2
	ldrh	r10, [r4, #80]
	mov	r1, r2
	ldrh	r9, [r4, #78]
	mov	r0, r2
	mov	r7, r2
	strb	r2, [fp, #-68]
	strb	r10, [fp, #-46]
	mov	r10, r2
	strb	r9, [fp, #-48]
	ldr	r9, [fp, #-48]
	strb	r2, [fp, #-76]
	ldr	r2, [fp, #-60]
	str	r9, [r3, #4]
	ldrb	r9, [r4, #8]	@ zero_extendqisi2
	str	r5, [fp, #-48]
	bfi	r2, r9, #2, #1
	ldrb	r9, [r4, #6]	@ zero_extendqisi2
	bfi	r2, r9, #0, #1
	ldrb	r9, [r4, #9]	@ zero_extendqisi2
	bfi	r2, r9, #3, #1
	ldrb	r9, [r4, #7]	@ zero_extendqisi2
	bfi	r2, r9, #1, #1
	ldrb	r9, [r4, #10]	@ zero_extendqisi2
	bfi	r2, r9, #4, #2
	ldrb	r9, [r4, #11]	@ zero_extendqisi2
	bfi	r2, r9, #6, #2
	strb	r2, [fp, #-48]
	ldr	r2, [fp, #-48]
	str	r5, [fp, #-48]
	str	r2, [r3, #8]
	ldrb	r2, [r4, #15]	@ zero_extendqisi2
	bfi	r0, r2, #4, #2
	ldrb	r2, [r4, #13]	@ zero_extendqisi2
	bfi	r0, r2, #1, #1
	ldrb	r2, [r4, #14]	@ zero_extendqisi2
	bfi	r0, r2, #2, #1
	ldrb	r2, [r4, #12]	@ zero_extendqisi2
	strb	lr, [fp, #-84]
	bfi	r0, r2, #0, #1
	strb	r0, [fp, #-48]
	ldr	r0, [fp, #-48]
	mov	r2, r8
	strb	ip, [fp, #-92]
	str	r0, [r3, #12]
	ldrb	r0, [r4, #19]	@ zero_extendqisi2
	ldrb	r9, [r4, #18]	@ zero_extendqisi2
	bfi	r7, r0, #0, #1
	ldrb	r0, [r4, #17]	@ zero_extendqisi2
	strb	r7, [fp, #-45]
	bfi	r8, r9, #0, #5
	bfi	lr, r0, #0, #5
	ldrb	r0, [r4, #16]	@ zero_extendqisi2
	strb	r8, [fp, #-46]
	mov	r8, r1
	strb	lr, [fp, #-47]
	bfi	ip, r0, #0, #5
	strb	ip, [fp, #-48]
	add	r0, r4, #12288
	ldr	ip, [fp, #-48]
	str	r5, [fp, #-48]
	str	ip, [r3, #16]
	ldrb	ip, [r4, #24]	@ zero_extendqisi2
	ldrb	lr, [r4, #23]	@ zero_extendqisi2
	bfi	r1, ip, #4, #1
	ldrb	ip, [r4, #20]	@ zero_extendqisi2
	bfi	r1, lr, #0, #2
	ldrb	lr, [r4, #22]	@ zero_extendqisi2
	and	ip, ip, #1
	ldrb	r7, [r4, #25]	@ zero_extendqisi2
	bfi	ip, lr, #2, #2
	ldrb	lr, [r4, #21]	@ zero_extendqisi2
	bfi	r1, r7, #6, #1
	strb	r1, [fp, #-47]
	bfi	ip, lr, #1, #1
	ldrb	r1, [r4, #26]	@ zero_extendqisi2
	ldr	lr, [fp, #-68]
	mov	r7, r2
	strb	ip, [fp, #-48]
	bfi	lr, r1, #0, #4
	strb	lr, [fp, #-46]
	ldr	r1, [fp, #-48]
	mov	lr, r10
	str	r5, [fp, #-48]
	str	r1, [r3, #20]
	ldrb	r1, [r4, #27]	@ zero_extendqisi2
	ldrb	ip, [r4, #28]	@ zero_extendqisi2
	and	r1, r1, #7
	bfi	r1, ip, #4, #2
	strb	r1, [fp, #-48]
	ldr	r1, [fp, #-48]
	mov	ip, r10
	ldr	r9, [fp, #-76]
	str	r5, [fp, #-48]
	str	r1, [r3, #24]
	ldrb	r1, [r4, #34]	@ zero_extendqisi2
	bfi	r10, r1, #6, #1
	ldrb	r1, [r4, #33]	@ zero_extendqisi2
	bfi	r10, r1, #4, #2
	ldrb	r1, [r4, #30]	@ zero_extendqisi2
	strb	r10, [fp, #-47]
	bfi	r9, r1, #1, #1
	mov	r1, r9
	ldrb	r9, [r4, #29]	@ zero_extendqisi2
	bfi	r1, r9, #0, #1
	ldrb	r9, [r4, #31]	@ zero_extendqisi2
	bfi	r1, r9, #2, #2
	ldrb	r9, [r4, #32]	@ zero_extendqisi2
	strb	r1, [fp, #-48]
	ldrh	r1, [fp, #-48]
	bfi	r1, r9, #4, #5
	strh	r1, [fp, #-48]	@ movhi
	ldr	r1, [fp, #-48]
	str	r5, [fp, #-48]
	str	r1, [r3, #28]
	ldrb	r1, [r4, #35]	@ zero_extendqisi2
	ldrb	r9, [r4, #36]	@ zero_extendqisi2
	and	r1, r1, #1
	ldrb	r10, [r4, #39]	@ zero_extendqisi2
	bfi	r1, r9, #1, #1
	ldrb	r9, [r4, #37]	@ zero_extendqisi2
	bfi	r1, r9, #2, #1
	ldrb	r9, [r4, #38]	@ zero_extendqisi2
	bfi	r1, r9, #4, #2
	ldrb	r9, [r4, #40]	@ zero_extendqisi2
	bfi	r1, r10, #6, #2
	ldr	r10, [fp, #-60]
	strb	r1, [fp, #-48]
	bfi	r10, r9, #0, #2
	strb	r10, [fp, #-47]
	ldr	r1, [fp, #-48]
	str	r5, [fp, #-48]
	str	r1, [r3, #32]
	ldrb	r1, [r4, #46]	@ zero_extendqisi2
	ldrb	r9, [r4, #42]	@ zero_extendqisi2
	bfi	r2, r1, #5, #1
	ldrb	r1, [r4, #44]	@ zero_extendqisi2
	bfi	r2, r9, #1, #1
	ldrb	r9, [r4, #41]	@ zero_extendqisi2
	bfi	r2, r1, #3, #1
	ldrb	r1, [r4, #43]	@ zero_extendqisi2
	bfi	r2, r9, #0, #1
	ldrb	r9, [r4, #47]	@ zero_extendqisi2
	bfi	r2, r1, #2, #1
	ldrb	r1, [r4, #45]	@ zero_extendqisi2
	bfi	r2, r9, #6, #1
	ldr	r9, [fp, #-92]
	bfi	r2, r1, #4, #1
	strb	r2, [fp, #-48]
	ldr	r2, [fp, #-48]
	ldr	r1, [fp, #-84]
	str	r5, [fp, #-48]
	str	r2, [r3, #36]
	ldrb	r2, [r4, #50]	@ zero_extendqisi2
	bfi	r1, r2, #0, #7
	ldrb	r2, [r4, #48]	@ zero_extendqisi2
	strb	r1, [fp, #-46]
	ldrb	r1, [r4, #49]	@ zero_extendqisi2
	bfi	r8, r2, #0, #3
	strb	r8, [fp, #-48]
	bfi	r9, r1, #0, #3
	strb	r9, [fp, #-47]
	ldr	r2, [fp, #-48]
	str	r5, [fp, #-48]
	str	r2, [r3, #40]
	ldrb	r1, [r4, #53]	@ zero_extendqisi2
	ldrb	r2, [r4, #51]	@ zero_extendqisi2
	ldrb	r8, [r4, #54]	@ zero_extendqisi2
	and	r1, r1, #3
	ldrb	r9, [r4, #52]	@ zero_extendqisi2
	and	r2, r2, #7
	bfi	r1, r8, #4, #1
	strb	r1, [fp, #-47]
	bfi	r2, r9, #4, #2
	strb	r2, [fp, #-48]
	ldr	r2, [fp, #-48]
	str	r2, [r3, #44]
	ldr	r2, [r0, #84]
	str	r2, [r3, #48]
	ldr	r2, [r0, #88]
	str	r2, [r3, #52]
	ldr	r2, [r0, #92]
	str	r2, [r3, #56]
	ldrb	r1, [r4, #55]	@ zero_extendqisi2
	ldrb	r2, [r4, #57]	@ zero_extendqisi2
	ldrh	r8, [r4, #72]
	bfi	lr, r1, #4, #1
	ldrb	r9, [r4, #58]	@ zero_extendqisi2
	and	r2, r2, #1
	ldrb	r1, [r4, #56]	@ zero_extendqisi2
	strh	r8, [fp, #-48]	@ movhi
	bfi	r2, r9, #1, #3
	bfi	lr, r1, #5, #3
	strb	r2, [fp, #-45]
	strb	lr, [fp, #-46]
	ldr	r2, [fp, #-48]
	str	r2, [r3, #60]
	ldr	r2, [r0, #96]
	str	r2, [r3, #64]
	ldr	r2, [r0, #100]
	str	r2, [r3, #68]
	ldr	r2, [r0, #104]
	str	r2, [r3, #72]
	ldr	r2, [r0, #108]
	str	r2, [r3, #76]
	ldr	r2, [r0, #112]
	str	r2, [r3, #80]
	ldr	r2, [r6, #1144]
	str	r2, [r3, #84]
	ldr	r2, [r6, #1148]
	str	r2, [r3, #88]
	ldr	r2, [r6, #1156]
	str	r2, [r3, #92]
	ldr	r2, [r6, #1160]
	str	r2, [r3, #96]
	ldr	r2, [r6, #1176]
	str	r2, [r3, #104]
	ldrh	r1, [r4, #74]
	ldrh	r2, [r4, #76]
	strh	r1, [fp, #-48]	@ movhi
	strh	r2, [fp, #-46]	@ movhi
	ldr	r2, [fp, #-48]
	str	r5, [fp, #-48]
	str	r2, [r3, #108]
	ldrb	r2, [r4, #59]	@ zero_extendqisi2
	ldrb	r1, [r4, #63]	@ zero_extendqisi2
	bfi	ip, r2, #4, #1
	ldrb	r2, [r4, #60]	@ zero_extendqisi2
	bfi	r7, r1, #1, #3
	strb	r7, [fp, #-45]
	ldrb	lr, [r4, #61]	@ zero_extendqisi2
	bfi	ip, r2, #5, #1
	ldrb	r1, [r4, #62]	@ zero_extendqisi2
	uxtb	r2, r7
	bfi	ip, lr, #6, #1
	strb	ip, [fp, #-46]
	bfi	r2, r1, #0, #1
	strb	r2, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r3, #112]
	ldr	r2, [r0, #116]
	str	r2, [r3, #116]
	ldr	r2, [r0, #120]
	str	r2, [r3, #120]
	ldr	r2, [r0, #124]
	str	r2, [r3, #124]
	ldr	r2, [r0, #128]
	str	r2, [r3, #128]
	ldr	r2, [r0, #132]
	str	r2, [r3, #132]
	ldr	r2, [r0, #136]
	str	r2, [r3, #136]
	ldr	r2, [r0, #140]
	str	r2, [r3, #140]
	ldr	r2, [r0, #144]
	str	r2, [r3, #144]
	ldr	r2, [r6, #60]
	str	r2, [r3, #252]
	ldr	r0, [r6, #1176]
	str	r2, [fp, #-48]
	bl	MEM_Phy2Vir
	cmp	r0, r5
	beq	.L81
	ldr	r3, .L82
	mov	r2, #3072
	ldr	r1, [r4, #64]
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, r5
.L78:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L80:
	ldr	r2, .L82+4
	ldr	r1, .L82+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L78
.L81:
	ldr	r2, .L82+4
	ldr	r1, .L82+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L78
.L83:
	.align	2
.L82:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+52
	.word	.LC0
	.word	.LC9
	UNWIND(.fnend)
	.size	VC1HAL_V400R003_CfgDnMsg, .-VC1HAL_V400R003_CfgDnMsg
	.align	2
	.global	VC1HAL_V400R003_StartDec
	.type	VC1HAL_V400R003_StartDec, %function
VC1HAL_V400R003_StartDec:
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
	subs	r6, r1, #0
	mov	r4, r0
	mov	r8, r2
	beq	.L95
	cmp	r6, #1
	moveq	r9, #0
	movteq	r9, 63941
	bne	.L98
.L86:
	cmp	r4, #0
	beq	.L99
	ldrh	r3, [r4, #78]
	cmp	r3, #512
	ldrhi	r3, .L103
	bhi	.L97
	ldrh	r3, [r4, #80]
	cmp	r3, #512
	bhi	.L100
	movw	r7, #1228
	ldr	r5, .L103+4
	mul	r7, r7, r6
	add	r3, r5, r7
	ldr	r0, [r3, #56]
	bl	MEM_Phy2Vir
	cmp	r0, #0
	beq	.L101
	ldr	r3, [r5, r7]
	cmp	r3, #0
	beq	.L102
.L93:
	movw	ip, #1228
	mov	r0, r4
	mla	r5, ip, r6, r5
	mov	r1, r5
	bl	VC1HAL_V400R003_CfgSliceMsg
	mov	r3, r8
	mov	r2, r6
	mov	r1, r5
	mov	r0, r4
	bl	VC1HAL_V400R003_CfgReg
	mov	r1, r5
	mov	r0, r4
	bl	VC1HAL_V400R003_CfgDnMsg
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L98:
	ldr	r1, .L103+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L95:
	mov	r9, #0
	movt	r9, 63683
	b	.L86
.L102:
	mov	r0, r9
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L94
	str	r3, [r5, r7]
	b	.L93
.L100:
	ldr	r3, .L103+12
.L97:
	ldr	r2, .L103+16
	mov	r0, #0
	ldr	r1, .L103+20
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L99:
	mov	r0, r4
	ldr	r3, .L103+24
	ldr	r2, .L103+16
	ldr	r1, .L103+20
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L101:
	ldr	r3, .L103+28
	ldr	r2, .L103+16
	ldr	r1, .L103+20
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L94:
	ldr	r1, .L103+32
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L104:
	.align	2
.L103:
	.word	.LC13
	.word	g_HwMem
	.word	.LC10
	.word	.LC14
	.word	.LANCHOR0+80
	.word	.LC12
	.word	.LC11
	.word	.LC15
	.word	.LC16
	UNWIND(.fnend)
	.size	VC1HAL_V400R003_StartDec, .-VC1HAL_V400R003_StartDec
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.15044, %object
	.size	__func__.15044, 28
__func__.15044:
	.ascii	"VC1HAL_V400R003_CfgSliceMsg\000"
	.type	__func__.15073, %object
	.size	__func__.15073, 23
__func__.15073:
	.ascii	"VC1HAL_V400R003_CfgReg\000"
	.space	1
	.type	__func__.15081, %object
	.size	__func__.15081, 25
__func__.15081:
	.ascii	"VC1HAL_V400R003_CfgDnMsg\000"
	.space	3
	.type	__func__.15020, %object
	.size	__func__.15020, 25
__func__.15020:
	.ascii	"VC1HAL_V400R003_StartDec\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"%s: pMsgBase = NULL\012\000" )
	.space	3
.LC1:
	ASCII(.ascii	"pVc1DecParam->SlcNum = %d\012\000" )
	.space	1
.LC2:
	ASCII(.ascii	"D0 = 0x%08x\012\000" )
	.space	3
.LC3:
	ASCII(.ascii	"D1 = 0x%08x\012\000" )
	.space	3
.LC4:
	ASCII(.ascii	"D2 = 0x%08x\012\000" )
	.space	3
.LC5:
	ASCII(.ascii	"D3 = 0x%08x\012\000" )
	.space	3
.LC6:
	ASCII(.ascii	"D4 = 0x%08x\012\000" )
	.space	3
.LC7:
	ASCII(.ascii	"D63 = 0x%08x\012\000" )
	.space	2
.LC8:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC9:
	ASCII(.ascii	"%s: u8Tmp = NULL\012\000" )
	.space	2
.LC10:
	ASCII(.ascii	"VdhId is wrong! VC1HAL_V200R003_StartDec\012\000" )
	.space	2
.LC11:
	ASCII(.ascii	"point of picture para null\012\000" )
.LC12:
	ASCII(.ascii	"%s: %s\012\000" )
.LC13:
	ASCII(.ascii	"picture width out of range\000" )
	.space	1
.LC14:
	ASCII(.ascii	"picture height out of range\000" )
.LC15:
	ASCII(.ascii	"can not map down msg virtual address!\000" )
	.space	2
.LC16:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
