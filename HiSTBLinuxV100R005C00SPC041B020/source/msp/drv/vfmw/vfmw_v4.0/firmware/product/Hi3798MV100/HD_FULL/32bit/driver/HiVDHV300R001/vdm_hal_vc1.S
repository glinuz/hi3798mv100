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
	.global	VC1HAL_V300R001_InitHal
	.type	VC1HAL_V300R001_InitHal, %function
VC1HAL_V300R001_InitHal:
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
	.size	VC1HAL_V300R001_InitHal, .-VC1HAL_V300R001_InitHal
	.align	2
	.global	VC1HAL_V300R001_CfgSliceMsg
	.type	VC1HAL_V300R001_CfgSliceMsg, %function
VC1HAL_V300R001_CfgSliceMsg:
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
	ldr	r3, [r1, #52]
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
	ldr	r6, .L64
	ldr	r1, .L64+4
	str	r0, [r3, #172]
	mov	r0, #4
	ldr	r3, [r6, #68]
	blx	r3
	ldrb	lr, [r10, #70]	@ zero_extendqisi2
	cmp	lr, #0
	ble	.L2
	mov	r7, #0
	mov	r8, r10
	str	r10, [fp, #-88]
	mov	r9, r4
	mov	r5, r7
	mov	r10, r7
.L24:
	ldr	r3, [fp, #-48]
	ldr	r2, [r8, #96]
	ldr	r1, [r8, #88]
	ldr	r3, [r3, #172]
	ldr	r4, [r8, #100]
	cmp	r1, #0
	rsb	r2, r3, r2
	ldr	r0, [r8, #104]
	and	ip, r2, #15
	bic	r2, r2, #15
	str	r2, [fp, #-64]
	add	ip, r4, ip, lsl #3
	beq	.L43
	ldr	r2, [r8, #112]
	ldr	r1, [r8, #116]
	rsb	r3, r3, r2
	str	r3, [fp, #-60]
	mov	r2, r3
	ldr	r3, [r8, #120]
	add	r1, r1, r2, lsl #3
	str	r2, [fp, #-76]
	add	r3, r3, r1, lsl #25
	str	r3, [fp, #-68]
	str	r3, [fp, #-72]
.L11:
	cmp	r10, #0
	bne	.L12
	ldr	r3, [fp, #-88]
	cmp	lr, #1
	ldr	r5, [r3, #108]
	beq	.L44
	ble	.L14
	ldr	r3, [r3, #156]
	cmp	r5, r3
	blt	.L14
	ldr	r3, [fp, #-52]
	mov	r7, #1
	b	.L15
.L17:
	ldr	r2, [r3, #72]
	cmp	r5, r2
	blt	.L39
.L15:
	add	r7, r7, #1
	add	r3, r3, #48
	cmp	r7, lr
	bne	.L17
	mov	r3, #0
	mov	r4, r3
.L18:
	mov	r2, #0
	str	r2, [fp, #-84]
.L13:
	add	r2, r0, ip, lsl #25
	ldr	r1, .L64+8
	str	r2, [r9]
	mov	r0, #4
	ldr	ip, [r6, #68]
	add	r10, r10, #1
	str	r3, [fp, #-80]
	blx	ip
	ldr	r3, [fp, #-64]
	mov	r0, #4
	ldr	r1, .L64+12
	add	r8, r8, #48
	add	r9, r9, #256
	str	r3, [r9, #-252]
	mov	r2, r3
	ldr	ip, [r6, #68]
	blx	ip
	ldr	r3, [fp, #-72]
	ldr	r2, [fp, #-68]
	mov	r0, #4
	ldr	r1, .L64+16
	str	r3, [r9, #-248]
	ldr	ip, [r6, #68]
	blx	ip
	ldr	r3, [fp, #-76]
	ldr	r2, [fp, #-60]
	mov	r0, #4
	ldr	r1, .L64+20
	str	r3, [r9, #-244]
	ldr	ip, [r6, #68]
	blx	ip
	ldr	r3, [fp, #-84]
	ldr	r1, .L64+24
	mov	r0, #4
	add	r2, r3, r5, lsl #16
	str	r2, [r9, #-240]
	ldr	ip, [r6, #68]
	blx	ip
	ldr	r3, [fp, #-80]
	mov	r2, r4
	ldr	r1, .L64+28
	mov	r0, #4
	str	r3, [r9, #-4]
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [fp, #-88]
	ldrb	lr, [r3, #70]	@ zero_extendqisi2
	cmp	lr, r10
	bgt	.L24
.L2:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L12:
	ldr	r2, [fp, #-52]
	mov	r3, r7, asl #6
	add	r4, r7, #1
	sub	r3, r3, r7, asl #4
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
	add	r1, r5, #1
	mov	r5, r2
	str	r1, [fp, #-84]
	b	.L13
.L19:
	cmp	r4, lr
	beq	.L46
.L57:
	ldr	r3, [fp, #-56]
	mov	r7, r4
	add	r4, r3, r4, lsl #8
	mov	r3, r4
	b	.L23
.L14:
	cmp	lr, #1
	beq	.L45
	mov	r7, #1
.L39:
	ldr	r3, [fp, #-56]
	add	r4, r3, r7, lsl #8
	mov	r3, r4
	b	.L18
.L43:
	str	r1, [fp, #-76]
	str	r1, [fp, #-72]
	str	r1, [fp, #-68]
	str	r1, [fp, #-60]
	b	.L11
.L44:
	mov	r3, r10
	str	r10, [fp, #-84]
	mov	r4, r10
	b	.L13
.L61:
	ldr	r3, .L64
	ldr	r2, .L64+32
	ldr	r1, .L64+36
	ldr	r3, [r3, #68]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	r3
.L45:
	mov	r3, #0
	mov	r7, lr
	mov	r4, r3
	b	.L18
.L48:
	mov	r1, r0
	b	.L58
.L65:
	.align	2
.L64:
	.word	vfmw_Osal_Func_Ptr_S
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
	.size	VC1HAL_V300R001_CfgSliceMsg, .-VC1HAL_V300R001_CfgSliceMsg
	.align	2
	.global	VC1HAL_V300R001_CfgReg
	.type	VC1HAL_V300R001_CfgReg, %function
VC1HAL_V300R001_CfgReg:
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
	UNWIND(.pad #8)
	sub	sp, sp, #8
	subs	r4, r2, #0
	mov	r5, r0
	ble	.L85
	ldr	r6, .L88
	mov	r7, #1
	mov	r3, r4
	str	r7, [sp]
	ldr	r2, .L88+4
	mov	r0, #32
	ldr	ip, [r6, #68]
	ldr	r1, .L88+8
	blx	ip
	str	r7, [sp]
	mov	r3, r4
	ldr	ip, [r6, #68]
	ldr	r2, .L88+4
	mov	r0, #32
	ldr	r1, .L88+8
	blx	ip
	str	r7, [sp]
	mov	r3, r4
	ldr	ip, [r6, #68]
	ldr	r2, .L88+4
	mov	r0, #32
	ldr	r1, .L88+8
	blx	ip
	str	r7, [sp]
	mov	r3, r4
	ldr	ip, [r6, #68]
	ldr	r2, .L88+4
	mov	r0, #32
	ldr	r1, .L88+8
	blx	ip
	str	r7, [sp]
	mov	r3, r4
	ldr	ip, [r6, #68]
	ldr	r2, .L88+4
	mov	r0, #32
	ldr	r1, .L88+8
	blx	ip
	str	r7, [sp]
	mov	r3, r4
	ldr	ip, [r6, #68]
	ldr	r2, .L88+4
	mov	r0, #32
	ldr	r1, .L88+8
	blx	ip
.L68:
	ldrh	r1, [r5, #78]
	movw	r2, #30480
	movt	r2, 1
	movw	r3, #30480
	cmp	r1, #256
	movhi	r1, r3
	movls	r1, r2
	cmp	r4, #0
	beq	.L71
	cmp	r4, #1
	bne	.L86
	ldr	r3, .L88+12
	ldr	r0, [r3]
	add	r0, r0, #4
	bl	MEM_WritePhyWord
.L74:
	ldr	r1, .L88
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L88+4
	ldr	ip, [r1, #68]
	ldr	r1, .L88+16
	blx	ip
	mvn	r0, #0
	b	.L83
.L86:
	cmp	r4, #0
	bgt	.L74
.L73:
	movw	r3, #1208
	ldr	r1, .L88+20
	mul	r3, r3, r4
	movw	r2, #3075
	movt	r2, 48
	add	r6, r5, #12288
	ldr	r0, [r1, r3]
	str	r2, [r0, #60]
	ldr	r0, [r1, r3]
	str	r2, [r0, #64]
	ldr	r0, [r1, r3]
	str	r2, [r0, #68]
	ldr	r0, [r1, r3]
	str	r2, [r0, #72]
	ldr	r0, [r1, r3]
	str	r2, [r0, #76]
	ldr	r0, [r1, r3]
	str	r2, [r0, #80]
	ldr	r3, [r1, r3]
	str	r2, [r3, #84]
	ldr	ip, [r6, #176]
	str	ip, [r6, #216]
	ldrh	r3, [r5, #74]
	sub	r2, r3, #1
	cmp	r2, #2048
	movcc	lr, #16
	bcs	.L87
.L76:
	ldrh	r0, [r5, #76]
	movw	r3, #1208
	mul	r3, r3, r4
	adds	r2, r0, #31
	addmi	r2, r0, #62
	add	ip, lr, ip, lsr #4
	bic	r2, r2, #31
	ldr	r4, [r6, #212]
	mov	r0, #0
	mul	r2, ip, r2
	ldr	ip, [r1, r3]
	str	r2, [r6, #220]
	str	r4, [ip, #96]
	ldr	r2, [r1, r3]
	ldr	ip, [r6, #216]
	str	ip, [r2, #100]
	ldr	r2, [r1, r3]
	ldr	ip, [r6, #220]
	str	ip, [r2, #104]
	ldr	r2, [r1, r3]
	ldrb	ip, [r5, #69]	@ zero_extendqisi2
	str	ip, [r2, #152]
	ldr	r2, [r1, r3]
	ldrb	ip, [r5, #68]	@ zero_extendqisi2
	str	ip, [r2, #148]
	ldrh	r2, [r5, #76]
	ldr	r1, [r1, r3]
	adds	r3, r2, #31
	addmi	r3, r2, #62
	bic	r3, r3, #31
	mul	r3, lr, r3
	str	r3, [r1, #108]
.L83:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L87:
	sub	r2, r3, #2048
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	lr, #32
	bcc	.L76
	sub	r2, r3, #4096
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	lr, #48
	bcc	.L76
	sub	r3, r3, #6144
	sub	r3, r3, #1
	cmp	r3, #2048
	movcs	lr, #16
	movcc	lr, #64
	b	.L76
.L85:
	movw	r3, #1208
	ldr	r2, .L88+20
	mul	r3, r3, r4
	add	ip, r0, #12288
	mvn	lr, #0
	ldr	r0, [r2, r3]
	str	lr, [r0, #32]
	ldr	lr, [r2, r3]
	ldr	r0, [ip, #164]
	ubfx	r0, r0, #0, #20
	orr	r0, r0, #1090519040
	str	r0, [lr, #8]
	ldr	r0, [r2, r3]
	ldr	lr, [ip, #168]
	str	lr, [r0, #12]
	ldr	r0, [r2, r3]
	ldr	lr, [r1, #48]
	str	lr, [r0, #16]
	ldr	r0, [r1, #32]
	ldr	r1, [r2, r3]
	str	r0, [r1, #20]
	ldr	r3, [r2, r3]
	ldr	r2, [ip, #172]
	str	r2, [r3, #24]
	b	.L68
.L71:
	ldr	r3, .L88+24
	ldr	r0, [r3]
	add	r0, r0, #4
	bl	MEM_WritePhyWord
	b	.L73
.L89:
	.align	2
.L88:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+28
	.word	.LC8
	.word	s_RegPhyBaseAddr_1
	.word	.LC9
	.word	g_HwMem
	.word	s_RegPhyBaseAddr
	UNWIND(.fnend)
	.size	VC1HAL_V300R001_CfgReg, .-VC1HAL_V300R001_CfgReg
	.align	2
	.global	VC1HAL_V300R001_CfgDnMsg
	.type	VC1HAL_V300R001_CfgDnMsg, %function
VC1HAL_V300R001_CfgDnMsg:
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
	ldr	r0, [r1, #48]
	mov	r6, r1
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L94
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
	ldr	r2, [r6, #1136]
	str	r2, [r3, #84]
	ldr	r2, [r6, #1140]
	str	r2, [r3, #88]
	ldr	r2, [r6, #1148]
	str	r2, [r3, #92]
	ldr	r2, [r6, #1152]
	str	r2, [r3, #96]
	ldr	r2, [r6, #1168]
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
	ldr	r2, [r6, #52]
	str	r2, [r3, #252]
	ldr	r0, [r6, #1168]
	str	r2, [fp, #-48]
	bl	MEM_Phy2Vir
	ldr	r3, .L96
	cmp	r0, r5
	beq	.L95
	mov	r2, #3072
	ldr	r1, [r4, #64]
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, r5
.L92:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L94:
	ldr	r3, .L96
	ldr	r2, .L96+4
	ldr	r1, .L96+8
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L92
.L95:
	ldr	r2, .L96+4
	ldr	r1, .L96+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L92
.L97:
	.align	2
.L96:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+52
	.word	.LC0
	.word	.LC10
	UNWIND(.fnend)
	.size	VC1HAL_V300R001_CfgDnMsg, .-VC1HAL_V300R001_CfgDnMsg
	.align	2
	.global	VC1HAL_V300R001_StartDec
	.type	VC1HAL_V300R001_StartDec, %function
VC1HAL_V300R001_StartDec:
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
	movw	r5, #1208
	ldr	r8, .L114
	mul	r5, r5, r1
	cmp	r1, #1
	mov	r7, r1
	mov	r4, r0
	add	r6, r5, r8
	bhi	.L110
	cmp	r0, #0
	ldreq	r1, .L114+4
	ldreq	r3, .L114+8
	beq	.L109
	ldrh	r3, [r0, #78]
	cmp	r3, #512
	bhi	.L111
	ldrh	r3, [r0, #80]
	cmp	r3, #512
	bhi	.L112
	ldr	r0, [r6, #48]
	bl	MEM_Phy2Vir
	cmp	r0, #0
	ldreq	r1, .L114+4
	ldreq	r3, .L114+12
	beq	.L109
	ldr	r3, [r5, r8]
	cmp	r3, #0
	beq	.L113
.L106:
	mov	r1, r6
	mov	r0, r4
	bl	VC1HAL_V300R001_CfgSliceMsg
	mov	r2, r7
	mov	r1, r6
	mov	r0, r4
	bl	VC1HAL_V300R001_CfgReg
	mov	r1, r6
	mov	r0, r4
	bl	VC1HAL_V300R001_CfgDnMsg
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L113:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L107
	str	r3, [r5, r8]
	b	.L106
.L111:
	ldr	r1, .L114+4
	mov	r0, #0
	ldr	r3, .L114+16
.L109:
	ldr	ip, [r1, #68]
	ldr	r2, .L114+20
	ldr	r1, .L114+24
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L112:
	ldr	r1, .L114+4
	mov	r0, #0
	ldr	r3, .L114+28
	b	.L109
.L110:
	ldr	r3, .L114+4
	mov	r0, #0
	ldr	r1, .L114+32
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L107:
	ldr	r3, .L114+4
	ldr	r1, .L114+36
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L115:
	.align	2
.L114:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC12
	.word	.LC16
	.word	.LC14
	.word	.LANCHOR0+80
	.word	.LC13
	.word	.LC15
	.word	.LC11
	.word	.LC17
	UNWIND(.fnend)
	.size	VC1HAL_V300R001_StartDec, .-VC1HAL_V300R001_StartDec
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.14281, %object
	.size	__func__.14281, 28
__func__.14281:
	.ascii	"VC1HAL_V300R001_CfgSliceMsg\000"
	.type	__func__.14310, %object
	.size	__func__.14310, 23
__func__.14310:
	.ascii	"VC1HAL_V300R001_CfgReg\000"
	.space	1
	.type	__func__.14322, %object
	.size	__func__.14322, 25
__func__.14322:
	.ascii	"VC1HAL_V300R001_CfgDnMsg\000"
	.space	3
	.type	__func__.14257, %object
	.size	__func__.14257, 25
__func__.14257:
	.ascii	"VC1HAL_V300R001_StartDec\000"
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
	ASCII(.ascii	"%s: WR_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\012\000" )
	.space	3
.LC9:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC10:
	ASCII(.ascii	"%s: u8Tmp = NULL\012\000" )
	.space	2
.LC11:
	ASCII(.ascii	"VdhId is wrong! VC1HAL_V200R003_StartDec\012\000" )
	.space	2
.LC12:
	ASCII(.ascii	"point of picture para null\012\000" )
.LC13:
	ASCII(.ascii	"%s: %s\012\000" )
.LC14:
	ASCII(.ascii	"picture width out of range\000" )
	.space	1
.LC15:
	ASCII(.ascii	"picture height out of range\000" )
.LC16:
	ASCII(.ascii	"can not map down msg virtual address!\000" )
	.space	2
.LC17:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
