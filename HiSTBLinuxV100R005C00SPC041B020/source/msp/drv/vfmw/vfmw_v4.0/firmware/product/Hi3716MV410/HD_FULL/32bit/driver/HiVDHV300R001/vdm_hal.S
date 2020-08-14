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
	.file	"vdm_hal.c"
	.text
	.align	2
	.global	VDMHAL_V300R001_GetHalMemSize
	.type	VDMHAL_V300R001_GetHalMemSize, %function
VDMHAL_V300R001_GetHalMemSize:
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
	mov	r0, #36864
	movt	r0, 65
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_GetHalMemSize, .-VDMHAL_V300R001_GetHalMemSize
	.align	2
	.global	VDMHAL_V300R001_OpenHAL
	.type	VDMHAL_V300R001_OpenHAL, %function
VDMHAL_V300R001_OpenHAL:
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
	UNWIND(.pad #20)
	sub	sp, sp, #20
	subs	r3, r0, #0
	ldreq	r1, .L24
	ldreq	r3, .L24+4
	beq	.L17
	ldr	r8, [r3]
	ldr	r6, [r3, #4]
	cmp	r8, #0
	ldr	r4, [r3, #8]
	beq	.L19
	movw	r3, #36863
	movt	r3, 65
	cmp	r6, r3
	ble	.L20
	cmp	r4, #1
	bhi	.L21
	cmp	r4, #0
	bgt	.L22
	movw	r2, #1208
	ldr	r5, .L24+8
	mul	r10, r2, r4
	ldr	r7, .L24
	mov	r1, #0
	add	r9, r5, r10
	ldr	r3, [r7, #48]
	mov	r0, r9
	blx	r3
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	ldr	r3, [r7, #68]
	ldr	r1, .L24+12
	mov	r2, r0
	str	r0, [r5, r10]
	mov	r0, #22
	blx	r3
	mov	r0, #0
	movt	r0, 63686
	bl	MEM_Phy2Vir
	ldr	r3, [r7, #68]
	ldr	r1, .L24+16
	mov	r2, r0
	str	r0, [r9, #8]
	mov	r0, #22
	blx	r3
	add	ip, r8, #1020
	add	ip, ip, #3
	add	r3, r10, #28
	bic	ip, ip, #1020
	add	r1, r10, #868
	bic	ip, ip, #3
	add	r3, r5, r3
	add	r1, r5, r1
	add	r0, r6, r8
	mov	r2, ip
	rsb	r0, ip, r0
	mov	lr, #1024
	str	ip, [r9, #16]
	str	r0, [r9, #20]
	str	lr, [r9, #24]
.L10:
	str	r2, [r3, #4]!
	cmp	r3, r1
	add	r2, r2, #1280
	bne	.L10
	movw	r1, #45567
	movw	r2, #1208
	movt	r1, 4
	add	r1, ip, r1
	bic	r1, r1, #32512
	add	r3, ip, #270336
	mla	r2, r2, r4, r5
	bic	r1, r1, #255
	add	lr, r1, #1769472
	add	r0, r1, #348160
	add	r9, lr, #5248
	add	r0, r0, #2048
	cmp	r9, r0
	rsbcs	r8, r8, r9
	rsbcc	r8, r8, r0
	add	lr, lr, #2048
	str	r8, [fp, #-48]
	add	r3, r3, #3584
	mov	r0, r8
	cmp	r6, r0
	add	r0, ip, #266240
	add	ip, r1, #290816
	add	ip, ip, #3072
	str	r9, [r2, #1128]
	str	ip, [r2, #1168]
	add	r9, r1, #294912
	add	ip, r1, #344064
	add	r9, r9, #2048
	add	ip, ip, #2048
	add	r8, r2, #1088
	str	lr, [r2, #1120]
	add	r0, r0, #2560
	str	lr, [r2, #1124]
	mov	lr, #210
	str	r9, [r2, #1172]
	add	r9, r1, #589824
	str	lr, [r2, #1056]
	add	lr, r1, #33792
	str	ip, [r2, #1176]
	add	ip, r1, #311296
	str	lr, [r2, #1140]
	add	lr, r1, #123904
	str	lr, [r2, #1152]
	add	lr, r1, #191488
	str	lr, [r2, #1156]
	movw	lr, #799
	str	lr, [r2, #1160]
	add	lr, r1, #1179648
	add	ip, ip, #2048
	add	lr, lr, #2048
	str	r1, [r2, #1136]
	add	r8, r8, #8
	str	r1, [r2, #1084]
	str	r1, [r2, #1088]
	str	r1, [r2, #1092]
	add	r1, r1, #78848
	str	r9, [r2, #1096]
	str	r9, [r2, #1104]
	add	r9, r9, #2048
	str	ip, [r2, #1184]
	str	ip, [r2, #1180]
	mov	ip, #0
	str	r9, [r2, #1108]
	str	lr, [r2, #1112]
	str	lr, [r2, #1116]
	str	r1, [r2, #1144]
	str	r1, [r2, #1148]
	str	r3, [r2, #1100]
	str	r0, [r2, #1080]
	str	ip, [r2, #1132]
	bcc	.L11
	bl	H264HAL_V300R001_InitHal
	mov	r6, r0
	ldr	r0, [r8, #4]
	bl	HEVCHAL_V300R001_InitHal
	cmp	r0, #0
	mvnne	r6, #0
	beq	.L23
.L12:
	movw	r3, #1208
	mla	r4, r3, r4, r5
	ldr	r0, [r4, #1080]
	bl	H264HAL_V300R001_InitHal
	cmp	r0, #0
	moveq	r0, r6
	mvnne	r0, #0
.L4:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L23:
	adds	r6, r6, #0
	movne	r6, #1
	rsb	r6, r6, #0
	b	.L12
.L22:
	ldr	r1, .L24
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L24+20
	ldr	ip, [r1, #68]
	ldr	r1, .L24+24
	blx	ip
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L19:
	ldr	r1, .L24
	mov	r0, r8
	ldr	r3, .L24+28
.L17:
	ldr	ip, [r1, #68]
	ldr	r2, .L24+20
	ldr	r1, .L24+32
	blx	ip
	mvn	r0, #0
	b	.L4
.L11:
	ldr	r4, [r7, #68]
	mov	r3, r6
	ldr	r2, [fp, #-48]
	mov	r0, ip
	ldr	r1, .L24+36
	blx	r4
	mvn	r0, #0
	b	.L4
.L20:
	ldr	r1, .L24
	mov	r0, #0
	ldr	r3, .L24+40
	b	.L17
.L21:
	ldr	r3, .L24
	mov	r0, #0
	ldr	r1, .L24+44
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L4
.L25:
	.align	2
.L24:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	g_HwMem
	.word	.LC6
	.word	.LC7
	.word	.LANCHOR0
	.word	.LC5
	.word	.LC2
	.word	.LC1
	.word	.LC8
	.word	.LC3
	.word	.LC4
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_OpenHAL, .-VDMHAL_V300R001_OpenHAL
	.align	2
	.global	VDMHAL_V300R001_CloseHAL
	.type	VDMHAL_V300R001_CloseHAL, %function
VDMHAL_V300R001_CloseHAL:
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
	cmp	r0, #1
	mov	r4, r0
	bhi	.L32
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, .L33
	ldr	r3, [r3, #56]
	cmp	r3, #0
	beq	.L30
	mov	r0, r4
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L32:
	ldr	r3, .L33+4
	mov	r0, #0
	ldr	r1, .L33+8
	ldr	r3, [r3, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L30:
	ldr	ip, .L33+4
	mov	r0, r3
	movw	r2, #379
	ldr	r1, .L33+12
	ldr	r3, [ip, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L34:
	.align	2
.L33:
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC9
	.word	.LC10
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_CloseHAL, .-VDMHAL_V300R001_CloseHAL
	.global	__aeabi_idiv
	.align	2
	.global	VDMHAL_V300R001_ArrangeMem
	.type	VDMHAL_V300R001_ArrangeMem, %function
VDMHAL_V300R001_ArrangeMem:
	UNWIND(.fnstart)
	@ args = 20, pretend = 0, frame = 48
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #60)
	sub	sp, sp, #60
	ldr	r8, [fp, #16]
	mov	r4, r0
	str	r1, [fp, #-68]
	cmn	r8, #2
	mov	r7, r2
	str	r3, [fp, #-64]
	ldr	r10, [fp, #4]
	ldr	r6, [fp, #20]
	beq	.L110
	cmp	r8, #0
	blt	.L111
	mov	r0, r8
	ldr	r5, .L180
	bl	VCTRL_GetVidStd
	mov	r9, r0
	ldr	r0, [r5, r8, asl #2]
	ldr	r3, [r0, #1388]
	cmp	r3, #1
	beq	.L159
.L37:
	cmp	r4, #0
	subne	r2, r7, #32
	movwne	r3, #8160
	beq	.L160
	cmp	r2, r3
	bhi	.L73
	ldr	r2, [fp, #-64]
	sub	r2, r2, #32
	cmp	r2, r3
	bhi	.L73
	cmp	r6, #0
	beq	.L161
	ldr	r3, .L180+4
	cmp	r10, #20
	mov	r2, #812
	mov	r1, #0
	mov	r0, r6
	movge	r10, #20
	str	r3, [fp, #-72]
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r2, [fp, #-64]
	add	r3, r7, #15
	add	r0, r2, #15
	ldr	r2, .L180+8
	mov	r3, r3, asr #4
	mov	r0, r0, asr #4
	cmp	r3, #45
	cmple	r0, #36
	ldrb	r5, [r2]	@ zero_extendqisi2
	mov	r1, r0, asl #4
	mov	r2, r3, asl #4
	movle	lr, #64
	movgt	lr, #32
	cmp	r5, #1
	moveq	lr, #64
	cmp	r9, #17
	beq	.L79
	mul	r5, r3, lr
	ldr	r3, [fp, #-68]
	mul	r5, r0, r5
	add	r5, r5, #127
	bic	r5, r5, #127
	mul	ip, r10, r5
	cmp	ip, r3
	bge	.L108
	add	r3, r2, #255
	str	r10, [r6, #800]
	bic	r10, r3, #255
	mov	r3, r5, asr #1
	str	r3, [r6, #796]
.L107:
	sub	r3, r2, #1
	mov	r0, r10, asl #4
	cmp	r3, #2048
	str	r0, [r6]
	movcc	r2, #16
	bcc	.L81
	sub	r3, r2, #2048
	sub	r3, r3, #1
	cmp	r3, #2048
	movcc	r2, #32
	bcc	.L81
	sub	r3, r2, #4096
	sub	r3, r3, #1
	cmp	r3, #2048
	movcc	r2, #48
	bcs	.L162
.L81:
	ldr	r3, [fp, #12]
	cmp	r3, #1
	beq	.L163
	cmp	r9, #17
	beq	.L164
	adds	r3, r1, #31
	addmi	r3, r1, #62
	cmp	r8, #0
	mov	r3, r3, asr #5
	mov	r0, r3
	mov	lr, r3, asl #7
	sub	lr, lr, r0, asl #5
	mul	r3, r10, r3
	mul	lr, r2, lr
	mov	r7, r3, asl #6
	sub	r7, r7, r3, asl #4
	add	r7, r7, lr, lsr #1
	blt	.L87
.L83:
	mov	r0, r8
	str	r1, [fp, #-88]
	str	ip, [fp, #-84]
	str	r2, [fp, #-80]
	bl	VCTRL_GetChanWidth
	str	r0, [fp, #-76]
	mov	r0, r8
	bl	VCTRL_GetChanHeight
	ldr	r3, [fp, #-76]
	ldr	r2, [fp, #-80]
	ldr	ip, [fp, #-84]
	ldr	r1, [fp, #-88]
	cmn	r0, #1
	cmnne	r3, #1
	beq	.L165
	add	lr, r3, #15
	add	r0, r0, #15
	bic	lr, lr, #15
	bic	r3, r0, #15
	sub	r8, lr, #1
	add	r0, lr, #255
	cmp	r8, #2048
	bic	r0, r0, #255
	movcc	lr, #16
	bcc	.L89
	sub	r8, lr, #2048
	sub	r8, r8, #1
	cmp	r8, #2048
	movcc	lr, #32
	bcs	.L166
.L89:
	ldr	r8, [fp, #12]
	cmp	r8, #1
	beq	.L167
	adds	r8, r3, #31
	addmi	r8, r3, #62
	mov	r3, r8, asr #5
	mov	r8, r3, asl #7
	mul	r0, r0, r3
	sub	r3, r8, r3, asl #5
	mul	lr, lr, r3
	mov	r3, r0, asl #6
	sub	r0, r3, r0, asl #4
	add	lr, r0, lr, lsr #1
.L91:
	cmp	lr, r7
	blt	.L168
	cmp	r9, #17
	beq	.L92
.L171:
	adds	r0, r1, #31
	addmi	r0, r1, #62
	mov	r0, r0, asr #5
	mul	r3, r0, r10
.L87:
	mov	r2, r2, asl #5
	mov	r3, r3, asl #5
	mla	r0, r0, r2, r3
	str	r0, [r6, #408]
.L93:
	ldr	r3, [fp, #8]
	cmp	r3, #0
	streq	r3, [r6, #420]
	bne	.L169
.L95:
	ldr	r3, [fp, #-72]
	mov	r0, #0
	ldr	r2, .L180+12
	ldr	r1, .L180+16
	ldr	ip, [r3, #68]
	ldr	r3, .L180+20
	blx	ip
	mvn	r3, #0
.L146:
	mov	r0, r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L111:
	mov	r9, #20
	b	.L37
.L169:
	ldr	r3, [fp, #8]
	add	r8, r4, #1020
	add	r8, r8, #3
	mov	r1, r7
	cmp	r3, #32
	ldr	r3, [fp, #-68]
	bic	r8, r8, #1020
	rsb	r0, ip, r3
	bic	r8, r8, #3
	ldr	r3, [fp, #8]
	rsb	r10, r4, r8
	rsb	r0, r10, r0
	movge	r9, #32
	movlt	r9, r3
	bl	__aeabi_idiv
	cmp	r9, r0
	movlt	r0, r9
	cmp	r0, #0
	str	r0, [r6, #420]
	beq	.L95
	ldrgt	r1, [r6, #804]
	addgt	r2, r0, #1
	movgt	r3, #1
	ble	.L170
.L98:
	str	r8, [r6, r3, asl #3]
	add	r3, r3, #1
	cmp	r3, r2
	add	r8, r8, r7
	bne	.L98
	mla	r7, r7, r0, r10
	add	r3, r0, r1
	add	r1, r6, #260
	str	r3, [r6, #804]
	mov	r2, #0
	add	r3, r7, r4
.L100:
	add	r2, r2, #1
	str	r3, [r1, #4]!
	cmp	r2, r0
	add	r3, r3, #32
	bne	.L100
.L106:
	ldr	r1, [r6, #800]
	add	r7, r7, r0, lsl #5
	add	r4, r7, r4
	cmp	r1, #0
	ldrgt	r0, [r6, #808]
	movgt	r2, r6
	movgt	r3, #0
	ble	.L103
.L102:
	add	r3, r3, #1
	str	r4, [r2, #556]
	cmp	r3, r1
	add	r4, r4, r5
	add	r2, r2, #12
	bne	.L102
	add	r3, r1, r0
	str	r3, [r6, #808]
.L103:
	mla	r7, r1, r5, r7
	mov	r3, #0
	mov	r0, r3
	str	r7, [r6, #4]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L110:
	mov	r9, #17
	b	.L37
.L79:
	mul	r5, r2, r0
	ldr	r3, [fp, #-68]
	add	r5, r5, #127
	bic	r5, r5, #127
	mul	ip, r10, r5
	cmp	ip, r3
	bge	.L108
	add	r7, r7, #255
	str	r10, [r6, #800]
	mov	r3, r5, asr #1
	bic	r10, r7, #255
	str	r3, [r6, #796]
	b	.L107
.L163:
	mul	r7, r1, r10
	cmp	r8, #0
	add	r7, r7, r7, lsl #1
	bge	.L83
	cmp	r9, #17
	bne	.L171
.L92:
	ldr	r3, [fp, #-64]
	mul	r3, r3, r10
.L86:
	str	r3, [r6, #408]
	b	.L93
.L166:
	sub	r8, lr, #4096
	sub	r8, r8, #1
	cmp	r8, #2048
	movcc	lr, #48
	bcc	.L89
	sub	lr, lr, #6144
	sub	lr, lr, #1
	cmp	lr, #2048
	movcs	lr, #16
	movcc	lr, #64
	b	.L89
.L162:
	sub	r2, r2, #6144
	sub	r2, r2, #1
	cmp	r2, #2048
	movcs	r2, #16
	movcc	r2, #64
	b	.L81
.L164:
	ldr	r3, [fp, #-64]
	cmp	r8, #0
	mul	r3, r3, r10
	add	r7, r3, r3, lsl #1
	mov	r7, r7, asr #1
	bge	.L83
	b	.L86
.L167:
	mul	lr, r3, r0
	add	lr, lr, lr, lsl #1
	b	.L91
.L159:
	sub	r1, r7, #32
	movw	r2, #8160
	add	r3, r0, #225280
	cmp	r1, r2
	ldr	ip, [r3, #3272]
	bhi	.L38
	ldr	r3, [fp, #-64]
	sub	r3, r3, #32
	cmp	r3, r2
	bls	.L39
.L38:
	ldr	r3, .L180+4
	mov	r0, #0
	ldr	r2, .L180+24
	mvn	r4, #0
	mov	r1, r3
	str	r3, [fp, #-72]
	ldr	ip, [r1, #68]
	ldr	r3, .L180+28
	ldr	r1, .L180+16
	blx	ip
.L40:
	ldr	r3, [fp, #-72]
	mov	r2, r4
	ldr	r1, .L180+32
	mov	r0, #31
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r3, #0
	b	.L146
.L170:
	mla	r7, r7, r0, r10
	b	.L106
.L73:
	ldr	r1, .L180+4
	mov	r0, #0
	ldr	r3, .L180+28
.L152:
	ldr	ip, [r1, #68]
	ldr	r2, .L180+12
	ldr	r1, .L180+16
	blx	ip
	mvn	r3, #0
	b	.L146
.L39:
	ldr	r3, [fp, #-64]
	add	r2, r7, #15
	ldr	r1, .L180+8
	add	r3, r3, #15
	mov	r2, r2, asr #4
	mov	r3, r3, asr #4
	ldrb	r4, [r1]	@ zero_extendqisi2
	cmp	r2, #45
	cmple	r3, #36
	mov	lr, r2, asl #4
	mov	r10, r3, asl #4
	str	r10, [fp, #-76]
	movle	r1, #64
	movgt	r1, #32
	cmp	r4, #1
	moveq	r1, #64
	cmp	r9, #17
	beq	.L172
	mul	r2, r1, r2
	add	r10, lr, #255
	bic	r10, r10, #255
	mul	r3, r2, r3
	add	r3, r3, #127
	bic	r3, r3, #127
	str	r3, [fp, #-68]
.L104:
	sub	r3, lr, #1
	cmp	r3, #2048
	movcc	r4, #16
	bcc	.L45
	sub	r3, lr, #2048
	sub	r3, r3, #1
	cmp	r3, #2048
	movcc	r4, #32
	bcc	.L45
	sub	r3, lr, #4096
	sub	r3, r3, #1
	cmp	r3, #2048
	movcc	r4, #48
	bcc	.L45
	sub	lr, lr, #6144
	sub	lr, lr, #1
	cmp	lr, #2048
	movcs	r4, #0
	movcc	r4, #64
.L45:
	ldr	r3, [fp, #12]
	cmp	r3, #1
	beq	.L173
	cmp	r9, #17
	beq	.L174
	ldr	r2, [fp, #-76]
	add	r4, r10, r4
	add	r0, r0, #228352
	str	ip, [fp, #-84]
	adds	r3, r2, #31
	add	r0, r0, #872
	addmi	r3, r2, #62
	ldr	r2, .L180+4
	mov	r10, r10, asl #4
	mov	r3, r3, asr #5
	mov	r1, r2
	str	r2, [fp, #-72]
	mul	r4, r3, r4
	mov	r2, #820
	ldr	r9, [r1, #48]
	mov	r1, #0
	mov	r3, r4, asl #6
	sub	r3, r3, r4, asl #4
	str	r3, [fp, #-80]
	blx	r9
	ldr	r3, [fp, #-68]
	ldr	r2, [r5, r8, asl #2]
	ldr	ip, [fp, #-84]
	mov	r1, r3, asr #1
	add	r0, r2, #225280
	add	r3, r2, #229376
	str	r1, [r3, #644]
	ldr	r1, [r2, #1396]
	ldr	r2, [r0, #3276]
	add	r2, r2, r1
	cmp	r2, #20
	movge	r2, #20
	str	r2, [r3, #648]
	str	r10, [r0, #3944]
.L51:
	mov	r4, r4, asl #5
.L151:
	str	r4, [r3, #256]
	ldr	r2, [fp, #-68]
	ldr	r1, [fp, #-80]
	add	r4, r2, r1
	str	r2, [r3, #664]
	adds	r4, r4, #1056
	str	r1, [r3, #660]
	bmi	.L40
	ldr	r3, [r5, r8, asl #2]
	cmp	ip, r4
	add	ip, r3, #225280
	ldr	r9, [ip, #3276]
	beq	.L175
.L55:
	add	r10, r3, #229376
	add	r0, r3, #228352
	str	r4, [ip, #3272]
	add	r1, r0, #232
	ldr	r3, [r10, #712]
	cmp	r3, #1
	addeq	r6, r0, #872
	moveq	r3, r1
	beq	.L59
.L105:
	add	r0, r0, #872
	mvn	r3, #0
.L60:
	strb	r3, [r1, #16]
	add	r1, r1, #20
	cmp	r1, r0
	bne	.L60
	ldr	r6, .L180+36
	ldr	ip, [r6]
	cmp	ip, #0
	beq	.L62
	ldr	lr, [r5, r8, asl #2]
	mov	r3, #16
	mov	r1, #122
	sub	r2, fp, #60
	add	lr, lr, #229376
	mov	r0, r8
	ldr	r10, [lr, #700]
	str	r10, [fp, #-60]
	ldr	r10, [lr, #704]
	str	r10, [fp, #-56]
	ldr	r10, [lr, #708]
	str	r10, [fp, #-52]
	ldr	lr, [lr, #712]
	str	lr, [fp, #-48]
	blx	ip
	ldr	ip, [r6]
	cmp	ip, #0
	beq	.L62
	ldr	r0, [fp, #-64]
	sub	r2, fp, #60
	str	r9, [fp, #-60]
	mov	r3, #16
	str	r4, [fp, #-56]
	mov	r1, #121
	str	r0, [fp, #-48]
	mov	r0, r8
	str	r7, [fp, #-52]
	blx	ip
.L62:
	ldr	r1, [r5, r8, asl #2]
	mov	r2, r4
	mov	r6, #0
	ldr	r4, [fp, #-72]
	add	lr, r1, #229376
	add	r1, r1, #225280
	mov	r3, r9
	mov	r0, #31
	str	r6, [lr, #692]
	ldr	lr, [r1, #3296]
	ldr	ip, [r4, #68]
	ldr	r1, .L180+40
	str	lr, [sp]
	blx	ip
	ldr	r3, [r4]
	ldr	r4, [r5, r8, asl #2]
	blx	r3
	ldr	r2, [r5, r8, asl #2]
	mov	r1, #1
	add	r4, r4, #229376
	mov	r3, #2
	add	ip, r2, #225280
	add	r2, r2, #229376
	str	r0, [r4, #668]
	str	r1, [ip, #3256]
	str	r1, [r2, #716]
	b	.L146
.L57:
	add	r3, r3, #20
	cmp	r3, r6
	beq	.L105
.L59:
	ldrb	r2, [r3, #16]	@ zero_extendqisi2
	cmp	r2, #255
	beq	.L57
	ldr	ip, [r10, #700]
	ldr	r2, [r3, #8]
	cmp	ip, r2
	bcc	.L57
	ldr	lr, [r3, #12]
	add	lr, r2, lr
	cmp	ip, lr
	bcs	.L57
	str	r2, [r10, #700]
	mov	r2, #1
	ldr	ip, [r3, #4]
	str	ip, [r10, #704]
	ldr	r3, [r3, #12]
	str	r2, [r10, #712]
	str	r3, [r10, #708]
	b	.L105
.L175:
	ldr	r2, [ip, #3296]
	cmp	r2, #1
	beq	.L55
	cmp	r6, #0
	beq	.L176
	add	r10, r3, #229376
	cmp	r9, #0
	add	r2, r3, #228352
	ldr	r1, [r10, #660]
	add	r2, r2, #872
	str	r2, [fp, #-76]
	str	r1, [fp, #-80]
	beq	.L177
.L65:
	ldr	r2, [r5, r8, asl #2]
	ldr	r4, .L180
	add	r2, r2, #225280
	ldr	r2, [r2, #3280]
	cmp	r2, #30
	bgt	.L178
.L66:
	str	r3, [fp, #-64]
	mov	r0, #8
	ldr	r3, [fp, #-72]
	str	r2, [r10, #268]
	str	ip, [fp, #-68]
	ldr	r2, [r3, #12]
	blx	r2
	ldr	r5, [r5, r8, asl #2]
	ldr	r3, [fp, #-64]
	add	r7, r5, #229376
	ldr	ip, [fp, #-68]
	ldr	r2, [r7, #692]
	cmp	r2, #0
	beq	.L70
	ldr	r2, [r10, #652]
	add	r5, r5, #228352
	ldr	r1, [r10, #648]
	add	r5, r5, #240
	add	lr, r2, #28416
	add	r9, r2, #57344
	add	lr, lr, #238
	add	r9, r9, #27
	add	r2, r2, #1
	str	ip, [fp, #-68]
	ldr	ip, [fp, #-80]
	add	lr, r3, lr, lsl #3
	add	r9, r3, r9, lsl #2
	mov	r4, #0
	mov	r8, r2
	str	r1, [fp, #-64]
	str	r6, [fp, #20]
.L69:
	ldr	r2, [r5]
	mov	r3, #1
	str	r3, [lr, #4]
	add	r1, r8, r4
	add	r3, r2, #1020
	rsb	r6, r2, ip
	add	r3, r3, #3
	add	r0, r2, #32
	bic	r3, r3, #1020
	add	r5, r5, #20
	bic	r3, r3, #3
	str	r3, [lr]
	add	r3, r6, r3
	add	lr, lr, #8
	add	r2, r3, r2
	str	r2, [r9, #4]!
	str	r1, [r10, #652]
	add	r3, r0, r3
	ldr	r2, [r10, #656]
	add	r4, r4, #1
	ldr	r1, [fp, #-64]
	add	r0, r2, #1
	cmp	r2, r1
	mov	r1, r2, asl #4
	sub	r2, r1, r2, asl #2
	bcs	.L68
	ldr	r1, [fp, #-68]
	add	r2, r1, r2
	add	r2, r2, #4480
	str	r3, [r2, #20]
	mov	r3, #1
	str	r3, [r2, #24]
	str	r0, [r10, #656]
.L68:
	ldr	r3, [r7, #692]
	cmp	r4, r3
	bcc	.L69
	ldr	r6, [fp, #20]
.L70:
	ldr	r4, [fp, #-72]
	mov	r2, #812
	ldr	r1, [fp, #-76]
	mov	r0, r6
	ldr	r3, [r4, #52]
	blx	r3
	ldr	r3, [r4, #16]
	mov	r0, #8
	blx	r3
	mov	r3, #0
	b	.L146
.L160:
	ldr	r1, .L180+4
	mov	r0, r4
	ldr	r3, .L180+44
	b	.L152
.L108:
	ldr	r3, [fp, #-72]
	mov	r0, #0
	ldr	r2, .L180+12
	ldr	r1, .L180+16
	ldr	ip, [r3, #68]
	ldr	r3, .L180+48
	blx	ip
	mvn	r3, #0
	b	.L146
.L168:
	ldr	r3, [fp, #-72]
	mov	r0, #0
	ldr	r2, .L180+12
	ldr	r1, .L180+16
	ldr	ip, [r3, #68]
	ldr	r3, .L180+52
	blx	ip
	mvn	r3, #0
	b	.L146
.L165:
	ldr	r3, [fp, #-72]
	mov	r0, #0
	ldr	r1, .L180+56
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r3, #0
	b	.L146
.L161:
	ldr	r1, .L180+4
	mov	r0, r6
	ldr	r3, .L180+60
	b	.L152
.L172:
	mul	r3, lr, r3
	add	r10, r7, #255
	bic	r10, r10, #255
	add	r3, r3, #127
	bic	r3, r3, #127
	str	r3, [fp, #-68]
	b	.L104
.L173:
	ldr	r3, [fp, #-76]
	add	r0, r0, #228352
	ldr	r2, .L180+4
	mov	r1, #0
	str	ip, [fp, #-84]
	add	r0, r0, #872
	mul	r3, r10, r3
	mov	ip, r2
	str	r2, [fp, #-72]
	mov	r2, #820
	ldr	ip, [ip, #48]
	add	r3, r3, r3, lsl #1
	str	r3, [fp, #-80]
	blx	ip
	ldr	r3, [fp, #-68]
	ldr	r2, [r5, r8, asl #2]
	mov	lr, r10, asl #4
	ldr	ip, [fp, #-84]
	mov	r0, r3, asr #1
	add	r1, r2, #225280
	add	r3, r2, #229376
	str	r0, [r3, #644]
	ldr	r0, [r2, #1396]
	ldr	r2, [r1, #3276]
	add	r2, r2, r0
	cmp	r2, #20
	movge	r2, #20
	cmp	r9, #17
	str	r2, [r3, #648]
	str	lr, [r1, #3944]
	beq	.L179
	ldr	r1, [fp, #-76]
	add	r4, r10, r4
	adds	r2, r1, #31
	addmi	r2, r1, #62
	mov	r2, r2, asr #5
	mul	r4, r2, r4
	b	.L51
.L178:
	str	r3, [fp, #-64]
	mov	r0, #31
	ldr	r3, [fp, #-72]
	ldr	r1, .L180+64
	str	ip, [fp, #-68]
	ldr	r7, [r3, #68]
	blx	r7
	ldr	r1, [r4, r8, asl #2]
	mov	r0, #30
	ldr	ip, [fp, #-68]
	add	r1, r1, #225280
	mov	r2, r0
	ldr	r3, [fp, #-64]
	str	r0, [r1, #3280]
	b	.L66
.L177:
	str	r3, [fp, #-64]
	mov	r0, #31
	ldr	r3, [fp, #-72]
	ldr	r1, .L180+68
	str	ip, [fp, #-68]
	ldr	r2, [r3, #68]
	blx	r2
	ldr	ip, [fp, #-68]
	ldr	r3, [fp, #-64]
	b	.L65
.L174:
	ldr	r3, [fp, #-64]
	add	r0, r0, #228352
	mov	r2, #820
	mov	r1, #0
	add	r0, r0, #872
	str	ip, [fp, #-76]
	mul	r4, r10, r3
	ldr	r3, .L180+4
	mov	r10, r10, asl #4
	str	r3, [fp, #-72]
	ldr	r9, [r3, #48]
	add	r3, r4, r4, lsl #1
	mov	r3, r3, asr #1
	str	r3, [fp, #-80]
	blx	r9
	ldr	r3, [fp, #-68]
	ldr	r2, [r5, r8, asl #2]
	ldr	ip, [fp, #-76]
	mov	r1, r3, asr #1
	add	r0, r2, #225280
	add	r3, r2, #229376
	str	r1, [r3, #644]
	ldr	r1, [r2, #1396]
	ldr	r2, [r0, #3276]
	add	r2, r2, r1
	cmp	r2, #20
	movge	r2, #20
	str	r2, [r3, #648]
	str	r10, [r0, #3944]
	b	.L151
.L179:
	ldr	r2, [fp, #-64]
	mul	r4, r10, r2
	b	.L151
.L176:
	ldr	r3, [fp, #-72]
	mov	r0, r6
	ldr	r2, .L180+72
	ldr	r1, .L180+16
	ldr	ip, [r3, #68]
	ldr	r3, .L180+76
	blx	ip
	mvn	r3, #0
	b	.L146
.L181:
	.align	2
.L180:
	.word	s_pstVfmwChan
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_not_direct_8x8_inference_flag
	.word	.LANCHOR0+88
	.word	.LC1
	.word	.LC22
	.word	.LANCHOR0+24
	.word	.LC11
	.word	.LC12
	.word	g_event_report
	.word	.LC13
	.word	.LC17
	.word	.LC19
	.word	.LC21
	.word	.LC20
	.word	.LC18
	.word	.LC16
	.word	.LC15
	.word	.LANCHOR0+52
	.word	.LC14
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_ArrangeMem, .-VDMHAL_V300R001_ArrangeMem
	.align	2
	.global	VDMHAL_V300R001_ArrangeMem_BTL
	.type	VDMHAL_V300R001_ArrangeMem_BTL, %function
VDMHAL_V300R001_ArrangeMem_BTL:
	UNWIND(.fnstart)
	@ args = 28, pretend = 0, frame = 16
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
	subs	r4, r0, #0
	str	r1, [fp, #-48]
	mov	r5, r3
	mov	r8, r2
	ldr	r7, [fp, #4]
	ldr	r6, [fp, #20]
	ldreq	r1, .L235
	ldreq	r3, .L235+4
	beq	.L227
	ldr	r3, [fp, #16]
	sub	r2, r2, #32
	cmp	r3, #1
	movw	r3, #8160
	beq	.L230
	cmp	r2, r3
	bhi	.L188
	sub	r2, r5, #32
	cmp	r2, r3
	bhi	.L188
.L187:
	cmp	r6, #0
	beq	.L231
	ldr	r10, .L235
	cmp	r7, #20
	mov	r2, #812
	mov	r1, #0
	mov	r0, r6
	movge	r7, #20
	ldr	r3, [r10, #48]
	blx	r3
	add	r3, r5, #15
	ldr	r1, .L235+8
	add	r2, r8, #15
	mov	r3, r3, asr #4
	mov	r0, r2, asr #4
	cmp	r0, #45
	cmple	r3, #36
	ldrb	ip, [r1]	@ zero_extendqisi2
	mov	r1, r0, asl #4
	movle	lr, #64
	movgt	lr, #32
	cmp	ip, #1
	mov	ip, r3, asl #4
	moveq	lr, #64
	mul	r0, r0, lr
	mul	r0, r3, r0
	ldr	r3, [fp, #-48]
	add	r5, r0, #127
	bic	r5, r5, #127
	mul	r9, r7, r5
	cmp	r9, r3
	bge	.L232
	add	r3, r4, #1020
	sub	r0, r1, #1
	add	r10, r1, #255
	add	r3, r3, #3
	cmp	r0, #2048
	bic	r3, r3, #1020
	bic	r10, r10, #255
	bic	r3, r3, #3
	rsb	r0, r4, r3
	str	r7, [r6, #800]
	str	r0, [fp, #-52]
	movcc	lr, #16
	mov	r0, r10, asl #4
	str	r0, [r6]
	mov	r0, r5, asr #1
	str	r0, [r6, #796]
	bcc	.L193
	sub	r0, r1, #2048
	sub	r0, r0, #1
	cmp	r0, #2048
	movcc	lr, #32
	bcc	.L193
	sub	r0, r1, #4096
	sub	r0, r0, #1
	cmp	r0, #2048
	movcc	lr, #48
	bcs	.L233
.L193:
	adds	r0, ip, #31
	ldr	r1, [fp, #28]
	addmi	r0, ip, #62
	cmp	r1, #0
	mov	r1, r0, asr #5
	ldreq	r2, [fp, #28]
	mul	lr, r1, lr
	mul	r0, r10, r1
	mov	r10, lr, asl #5
	add	r1, r10, r0, lsl #5
	str	r1, [r6, #408]
	beq	.L194
	add	r1, r2, #15
	cmp	r2, #0
	movlt	r2, r1
	mov	r1, r2, asr #4
	adds	r2, r1, #7
	addmi	r2, r1, #14
	mov	r2, r2, asr #3
	mov	r2, r2, asl #4
	mul	r2, ip, r2
	add	r1, r2, #7
	cmp	r2, #0
	movlt	r2, r1
	mov	r2, r2, asr #3
	add	r2, r2, r2, lsl #1
	add	r2, r2, r2, lsr #31
	mov	r2, r2, asr #1
	adds	r1, r2, #127
	addmi	r1, r2, #254
	bic	r2, r1, #127
.L194:
	ldr	r1, [fp, #24]
	cmp	r1, #0
	bne	.L234
	ldr	r1, [fp, #12]
	cmp	r1, #0
	beq	.L210
	ldr	r1, [fp, #12]
	ldr	ip, [fp, #12]
	cmp	r1, #6
	ldr	r1, [fp, #24]
	movge	r8, ip
	movlt	r8, #6
	str	r8, [r6, #552]
.L199:
	add	r1, r2, r1
	cmp	r8, #0
	mulle	r2, r8, r1
	ble	.L197
	add	ip, r6, #420
	mov	r2, #0
.L200:
	add	r2, r2, #1
	str	r3, [ip, #4]!
	cmp	r2, r8
	add	r3, r3, r1
	bne	.L200
	mul	r2, r1, r2
.L197:
	ldr	r3, [fp, #8]
	cmp	r3, #0
	ldr	r3, [fp, #-52]
	add	r8, r3, r2
	bne	.L201
	ldr	r3, [fp, #8]
	str	r3, [r6, #420]
.L202:
	cmp	r7, #0
	add	r8, r8, r3
	add	r4, r8, r4
	movgt	ip, r6
	movgt	r0, #0
	ble	.L209
.L208:
	add	r0, r0, #1
	str	r4, [ip, #556]
	cmp	r0, r7
	add	r4, r4, r5
	add	ip, ip, #12
	bne	.L208
.L209:
	mov	r0, #0
	add	r8, r9, r8
	str	r8, [r6, #4]
.L224:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L234:
	adds	r1, r8, #127
	addmi	r1, r8, #254
	ldr	r8, [fp, #12]
	mov	r1, r1, asr #7
	cmp	r8, #0
	mov	r8, r1, asl #9
	sub	r1, r8, r1, asl #7
	mul	r1, ip, r1
	mov	r1, r1, asr #1
	beq	.L210
	ldr	ip, [fp, #12]
	str	ip, [r6, #552]
	mov	r8, ip
	b	.L199
.L201:
	ldr	r3, [fp, #24]
	rsb	lr, r10, lr, lsl #7
	mov	r10, r0, asl #6
	cmp	r3, #0
	ldr	r3, [fp, #8]
	sub	r0, r10, r0, asl #4
	ldreq	r2, [fp, #12]
	add	r3, r3, #2
	add	r10, r0, lr, lsr #1
	addeq	r3, r3, r2
	str	r3, [fp, #-56]
	ldr	r3, [fp, #-48]
	mov	r1, r10
	rsb	r2, r9, r3
	ldr	r3, [fp, #-52]
	rsb	r0, r3, r2
	rsb	r0, r8, r0
	bl	__aeabi_idiv
	ldr	r3, [fp, #-56]
	cmp	r0, #32
	movge	r0, #32
	cmp	r0, r3
	movge	r0, r3
	cmp	r0, #0
	str	r0, [r6, #420]
	addgt	r1, r8, r4
	addgt	ip, r0, #1
	movgt	r2, #1
	ble	.L226
.L206:
	str	r1, [r6, r2, asl #3]
	add	r2, r2, #1
	cmp	r2, ip
	add	r1, r1, r10
	bne	.L206
.L226:
	mul	r3, r0, r10
	b	.L202
.L210:
	mov	r1, #0
	str	r1, [r6, #552]
	mov	r2, r1
	b	.L197
.L230:
	cmp	r2, r3
	bhi	.L188
	sub	r2, r5, #32
	movw	r3, #16352
	cmp	r2, r3
	bls	.L187
.L188:
	ldr	r1, .L235
	mov	r0, #0
	ldr	r3, .L235+12
.L227:
	ldr	ip, [r1, #68]
	ldr	r2, .L235+16
	ldr	r1, .L235+20
	blx	ip
	mvn	r0, #0
	b	.L224
.L233:
	sub	r1, r1, #6144
	sub	r1, r1, #1
	cmp	r1, #2048
	movcs	lr, #16
	movcc	lr, #64
	b	.L193
.L232:
	ldr	ip, [r10, #68]
	mov	r0, #0
	ldr	r3, .L235+24
	ldr	r2, .L235+16
	ldr	r1, .L235+20
	blx	ip
	mvn	r0, #0
	b	.L224
.L231:
	ldr	r1, .L235
	mov	r0, r6
	ldr	r3, .L235+28
	b	.L227
.L236:
	.align	2
.L235:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC23
	.word	g_not_direct_8x8_inference_flag
	.word	.LC11
	.word	.LANCHOR0+116
	.word	.LC1
	.word	.LC19
	.word	.LC18
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_ArrangeMem_BTL, .-VDMHAL_V300R001_ArrangeMem_BTL
	.align	2
	.global	VDMHAL_V300R001_ResetVdm
	.type	VDMHAL_V300R001_ResetVdm, %function
VDMHAL_V300R001_ResetVdm:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	subs	r4, r0, #0
	beq	.L239
	cmp	r4, #1
	bne	.L258
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	ldr	r2, .L261
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r1, .L261+4
	ldr	ip, [r2, #68]
	ldr	r2, .L261+8
	blx	ip
.L237:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L258:
	ldr	r3, .L261
	mov	r0, #0
	ldr	r1, .L261+12
	ldr	r3, [r3, #68]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	bx	r3
.L239:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r2, r0, #0
	beq	.L259
	ldr	r6, .L261+16
	ldr	r7, .L261+20
	ldr	r3, [r6]
	ldr	r1, [r7]
	ldr	r8, [r3, #36]
	ldr	r5, [r1, #120]
	tst	r5, #1
	beq	.L237
	orr	r5, r5, #64
	str	r5, [r1, #120]
	ldr	r3, [r1, #372]
	tst	r3, #4
	bne	.L244
	mov	r3, #1
	movw	r0, #10000
	b	.L246
.L260:
	add	r3, r3, #1
	cmp	r3, r0
	beq	.L247
.L246:
	ldr	r2, [r1, #372]
	tst	r2, #4
	beq	.L260
	movw	r2, #10000
	cmp	r3, r2
	beq	.L247
.L248:
	bfc	r5, #6, #1
	str	r5, [r1, #120]
.L249:
	ldr	r3, [r6]
	str	r8, [r3, #36]
	b	.L237
.L247:
	ldr	r3, .L261
	mov	r2, #0
	ldr	r1, .L261+24
	mov	r0, r2
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r1, [r7]
	b	.L248
.L259:
	ldr	r3, .L261
	ldr	r1, .L261+28
	ldr	r3, [r3, #68]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	bx	r3
.L244:
	bfi	r5, r4, #6, #1
	str	r5, [r1, #120]
	b	.L249
.L262:
	.align	2
.L261:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC5
	.word	.LANCHOR0+148
	.word	.LC24
	.word	g_HwMem
	.word	g_pstRegCrg
	.word	.LC26
	.word	.LC25
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_ResetVdm, .-VDMHAL_V300R001_ResetVdm
	.align	2
	.global	VDMHAL_V300R001_GlbReset
	.type	VDMHAL_V300R001_GlbReset, %function
VDMHAL_V300R001_GlbReset:
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
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r2, r0, #0
	beq	.L274
	ldr	r5, .L278
	ldr	r2, [r5]
	ldr	r4, [r2, #120]
	tst	r4, #1
	beq	.L275
	orr	r4, r4, #16
	str	r4, [r2, #120]
	ldr	r3, [r2, #372]
	tst	r3, #1
	bne	.L269
	movw	r0, #9999
	b	.L267
.L277:
	subs	r0, r0, #1
	beq	.L276
.L267:
	ldr	r3, [r2, #372]
	tst	r3, #1
	beq	.L277
.L269:
	bfc	r4, #4, #1
	str	r4, [r2, #120]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L275:
	ldr	r3, .L278+4
	mov	r0, #3
	ldr	r1, .L278+8
	ldr	r3, [r3, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L276:
	ldr	r3, .L278+4
	ldr	r1, .L278+12
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r2, [r5]
	b	.L269
.L274:
	ldr	r3, .L278+4
	ldr	r1, .L278+16
	ldr	r3, [r3, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L279:
	.align	2
.L278:
	.word	g_pstRegCrg
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC27
	.word	.LC28
	.word	.LC25
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_GlbReset, .-VDMHAL_V300R001_GlbReset
	.align	2
	.global	VDMHAL_V300R001_ClearIntState
	.type	VDMHAL_V300R001_ClearIntState, %function
VDMHAL_V300R001_ClearIntState:
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
	UNWIND(.pad #8)
	sub	sp, sp, #8
	ldr	r7, .L301
	mov	r5, r0, asl #3
	mov	r6, r0, asl #6
	rsb	r3, r5, r6
	ldr	r2, .L301+4
	add	r3, r7, r3
	ldr	r3, [r3, #8]
	ldr	r3, [r2, r3, asl #2]
	cmp	r3, #0
	ldrne	r4, [r3, #1232]
	moveq	r4, r3
	cmp	r0, #1
	bhi	.L297
	cmp	r0, #0
	bgt	.L298
	movw	r8, #1208
	ldr	r9, .L301+8
	mul	r8, r8, r0
	ldr	r3, [r9, r8]
	cmp	r3, #0
	beq	.L299
.L286:
	cmp	r4, #1
	beq	.L300
.L294:
	mvn	r2, #0
	str	r2, [r3, #32]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L298:
	ldr	r1, .L301+12
	mov	r2, #0
	mov	r3, r0
	str	r2, [sp]
	mov	r0, r2
	ldr	r2, .L301+16
	ldr	ip, [r1, #68]
	ldr	r1, .L301+20
	blx	ip
.L280:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L297:
	ldr	r3, .L301+12
	mov	r0, #0
	ldr	r1, .L301+24
.L295:
	ldr	r3, [r3, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L300:
	rsb	r5, r5, r6
	add	r7, r7, r5
	ldr	r2, [r7, #52]
	cmp	r2, #1
	beq	.L294
	cmp	r2, #2
	ldr	r1, [r3, #28]
	mvneq	r2, #11
	streq	r2, [r3, #32]
	beq	.L280
	cmp	r2, #3
	mvneq	r2, #14
	streq	r2, [r3, #32]
	b	.L280
.L299:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r9, r8]
	bne	.L286
.L287:
	ldr	r3, .L301+12
	ldr	r1, .L301+28
	b	.L295
.L302:
	.align	2
.L301:
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+176
	.word	.LC5
	.word	.LC29
	.word	.LC30
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_ClearIntState, .-VDMHAL_V300R001_ClearIntState
	.align	2
	.global	VDMHAL_V300R001_MaskInt
	.type	VDMHAL_V300R001_MaskInt, %function
VDMHAL_V300R001_MaskInt:
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
	UNWIND(.pad #8)
	sub	sp, sp, #8
	cmp	r0, #1
	bhi	.L312
	cmp	r0, #0
	bgt	.L313
	movw	r4, #1208
	ldr	r5, .L315
	mul	r4, r4, r0
	ldr	r3, [r5, r4]
	cmp	r3, #0
	beq	.L314
.L308:
	mvn	r2, #0
	str	r2, [r3, #36]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L313:
	ldr	r1, .L315+4
	mov	r2, #0
	mov	r3, r0
	str	r2, [sp]
	mov	r0, r2
	ldr	r2, .L315+8
	ldr	ip, [r1, #68]
	ldr	r1, .L315+12
	blx	ip
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L312:
	ldr	r3, .L315+4
	mov	r0, #0
	ldr	r1, .L315+16
.L311:
	ldr	r3, [r3, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L314:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r5, r4]
	bne	.L308
.L309:
	ldr	r3, .L315+4
	ldr	r1, .L315+20
	b	.L311
.L316:
	.align	2
.L315:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+208
	.word	.LC5
	.word	.LC31
	.word	.LC30
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_MaskInt, .-VDMHAL_V300R001_MaskInt
	.align	2
	.global	VDMHAL_V300R001_EnableInt
	.type	VDMHAL_V300R001_EnableInt, %function
VDMHAL_V300R001_EnableInt:
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r2, .L335
	mov	r3, r0, asl #6
	ldr	r1, .L335+4
	sub	r3, r3, r0, asl #3
	add	r3, r2, r3
	ldr	r3, [r3, #8]
	ldr	r3, [r1, r3, asl #2]
	cmp	r3, #0
	ldrne	r4, [r3, #1232]
	moveq	r4, r3
	cmp	r0, #1
	bhi	.L332
	cmp	r0, #0
	bgt	.L333
	movw	r5, #1208
	ldr	r6, .L335+8
	mul	r5, r5, r0
	ldr	r3, [r6, r5]
	cmp	r3, #0
	beq	.L334
.L323:
	cmp	r4, #1
	mvnne	r2, #1
	strne	r2, [r3, #36]
	beq	.L331
.L317:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L333:
	ldr	r1, .L335+12
	mov	r2, #0
	mov	r3, r0
	str	r2, [sp]
	mov	r0, r2
	ldr	r2, .L335+16
	ldr	ip, [r1, #68]
	ldr	r1, .L335+20
	blx	ip
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L332:
	ldr	r3, .L335+12
	mov	r0, #0
	ldr	r1, .L335+24
.L330:
	ldr	r3, [r3, #68]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	bx	r3
.L331:
	mvn	r2, #5
	str	r2, [r3, #36]
	b	.L317
.L334:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r6, r5]
	bne	.L323
.L324:
	ldr	r3, .L335+12
	ldr	r1, .L335+28
	b	.L330
.L336:
	.align	2
.L335:
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+232
	.word	.LC5
	.word	.LC32
	.word	.LC30
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_EnableInt, .-VDMHAL_V300R001_EnableInt
	.align	2
	.global	VDMHAL_V300R001_CheckReg
	.type	VDMHAL_V300R001_CheckReg, %function
VDMHAL_V300R001_CheckReg:
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
	subs	r5, r1, #0
	mov	r7, r0
	bgt	.L351
	movw	r4, #1208
	ldr	r6, .L353
	mul	r4, r4, r5
	ldr	r3, [r6, r4]
	cmp	r3, #0
	beq	.L352
.L340:
	sub	r2, r7, #1
	cmp	r2, #3
	ldrls	pc, [pc, r2, asl #2]
	b	.L342
.L344:
	.word	.L348
	.word	.L345
	.word	.L346
	.word	.L349
.L349:
	mov	r3, #40
.L343:
	movw	r1, #1208
	mul	r5, r1, r5
	ldr	r2, [r6, r5]
	ldr	r0, [r2, r3]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L348:
	mov	r3, #28
	b	.L343
.L346:
	mov	r3, #36
	b	.L343
.L345:
	mov	r3, #32
	b	.L343
.L342:
	ldr	ip, .L353+4
	mov	r3, r7
	ldr	r2, .L353+8
	mov	r0, #0
	ldr	r1, .L353+12
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L352:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r8, r0, #0
	beq	.L341
	str	r8, [r6, r4]
	b	.L340
.L351:
	ldr	ip, .L353+4
	mov	r3, r5
	ldr	r2, .L353+8
	mov	r0, #0
	ldr	r1, .L353+16
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L341:
	ldr	r3, .L353+4
	ldr	r2, .L353+8
	ldr	r1, .L353+20
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L354:
	.align	2
.L353:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+260
	.word	.LC35
	.word	.LC33
	.word	.LC34
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_CheckReg, .-VDMHAL_V300R001_CheckReg
	.align	2
	.global	VDMHAL_V300R001_PrepareDec
	.type	VDMHAL_V300R001_PrepareDec, %function
VDMHAL_V300R001_PrepareDec:
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
	subs	r7, r1, #0
	mov	r5, r0
	mov	r4, r2
	beq	.L356
	ldr	r6, .L383
	cmp	r0, #6
	moveq	r1, #1
	movne	r1, #0
	ldr	r0, [r6]
	bl	MEM_WritePhyWord
	cmp	r4, #0
	beq	.L360
	cmp	r4, #1
	bne	.L382
	ldr	r3, .L383+4
	mov	r1, r4
	ldr	r0, [r3]
	add	r0, r0, #8
	bl	MEM_WritePhyWord
.L363:
	ldr	r1, .L383+8
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L383+12
	ldr	ip, [r1, #68]
	ldr	r1, .L383+16
	blx	ip
.L356:
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L382:
	cmp	r4, #0
	bgt	.L363
.L362:
	cmp	r5, #17
	ldrls	pc, [pc, r5, asl #2]
	b	.L356
.L365:
	.word	.L375
	.word	.L366
	.word	.L367
	.word	.L368
	.word	.L356
	.word	.L369
	.word	.L370
	.word	.L356
	.word	.L371
	.word	.L372
	.word	.L373
	.word	.L373
	.word	.L373
	.word	.L374
	.word	.L356
	.word	.L356
	.word	.L375
	.word	.L376
.L360:
	ldr	r0, [r6]
	mov	r1, #1
	add	r0, r0, #8
	bl	MEM_WritePhyWord
	b	.L362
.L375:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	H264HAL_V300R001_StartDec
.L374:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VP8HAL_V300R001_StartDec
.L373:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VP6HAL_V300R001_StartDec
.L372:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	RV9HAL_V300R001_StartDec
.L371:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	RV8HAL_V300R001_StartDec
.L370:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	AVSHAL_V300R001_StartDec
.L369:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	DIVX3HAL_V300R001_StartDec
.L368:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	MP2HAL_V300R001_StartDec
.L367:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	MP4HAL_V300R001_StartDec
.L366:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VC1HAL_V300R001_StartDec
.L376:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	HEVCHAL_V300R001_StartDec
.L384:
	.align	2
.L383:
	.word	s_RegPhyBaseAddr
	.word	s_RegPhyBaseAddr_1
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+288
	.word	.LC5
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_PrepareDec, .-VDMHAL_V300R001_PrepareDec
	.align	2
	.global	VDMHAL_V300R001_IsVdmReady
	.type	VDMHAL_V300R001_IsVdmReady, %function
VDMHAL_V300R001_IsVdmReady:
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
	UNWIND(.pad #8)
	sub	sp, sp, #8
	movw	r2, #1208
	mul	r2, r2, r0
	ldr	r3, .L392
	ldr	r3, [r3, r2]
	cmp	r3, #0
	beq	.L390
	cmp	r0, #0
	ble	.L391
	ldr	r1, .L392+4
	mov	r3, r0
	ldr	r2, .L392+8
	mov	r0, #1
	str	r0, [sp]
	mov	r0, #32
	ldr	ip, [r1, #68]
	ldr	r1, .L392+12
	blx	ip
	mov	r0, #0
.L387:
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L391:
	ldr	r0, [r3, #28]
	ubfx	r0, r0, #17, #1
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L390:
	ldr	r1, .L392+4
	mov	r0, r3
	ldr	r2, .L392+8
	ldr	r3, .L392+16
	ldr	ip, [r1, #68]
	ldr	r1, .L392+20
	blx	ip
	mvn	r0, #0
	b	.L387
.L393:
	.align	2
.L392:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+316
	.word	.LC37
	.word	.LC36
	.word	.LC1
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_IsVdmReady, .-VDMHAL_V300R001_IsVdmReady
	.align	2
	.global	VDMHAL_V300R001_IsVdmRun
	.type	VDMHAL_V300R001_IsVdmRun, %function
VDMHAL_V300R001_IsVdmRun:
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	movw	r2, #1208
	mul	r2, r2, r0
	ldr	r3, .L401
	ldr	r4, [r3, r2]
	cmp	r4, #0
	beq	.L399
	cmp	r0, #0
	ble	.L400
	ldr	r1, .L401+4
	mov	r3, r0
	mov	r4, #1
	ldr	r2, .L401+8
	str	r4, [sp]
	mov	r0, #32
	ldr	ip, [r1, #68]
	ldr	r1, .L401+12
	blx	ip
	mov	r0, r4
.L396:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L400:
	ldr	r0, [r4, #40]
	subs	r0, r0, #1
	movne	r0, #1
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L399:
	ldr	r3, .L401+4
	mov	r0, r4
	ldr	r1, .L401+16
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	b	.L396
.L402:
	.align	2
.L401:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+344
	.word	.LC37
	.word	.LC38
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_IsVdmRun, .-VDMHAL_V300R001_IsVdmRun
	.align	2
	.global	VDMHAL_V300R001_BackupInfo
	.type	VDMHAL_V300R001_BackupInfo, %function
VDMHAL_V300R001_BackupInfo:
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
	UNWIND(.pad #8)
	sub	sp, sp, #8
	subs	r2, r0, #0
	ldreq	r1, .L441
	ldreq	r3, .L441+4
	beq	.L434
	ldr	r3, [r2, #4]
	ldr	r5, [r2]
	cmp	r3, #1
	bhi	.L437
	cmp	r3, #0
	bne	.L408
	ldr	r3, .L441+8
	ldr	r2, [r3]
	cmp	r2, #1
	addne	r1, r3, #60
	beq	.L425
.L411:
	ldr	r2, [r3, #4]!
	cmp	r2, #1
	beq	.L425
	cmp	r3, r1
	bne	.L411
	mov	r6, #0
.L409:
	ldr	r8, .L441+12
	ldr	lr, [r8]
	cmp	lr, #0
	beq	.L438
.L412:
	ldr	r7, [lr, #12]
	cmp	r6, #1
	ldr	r4, .L441+16
	str	r7, [r4]
	ldr	r3, [lr, #28]
	str	r3, [r4, #4]
	beq	.L439
	add	r3, lr, #4096
	movw	r2, #4116
	cmp	r6, #0
	ldr	r3, [r3]
	str	r3, [r4, #16]
	ldr	r3, [lr, r2]
	str	r3, [r4, #20]
	ldr	r8, [lr, #208]
	str	r8, [r4, #8]
	ldr	r9, [lr, #212]
	str	r9, [r4, #12]
	bne	.L416
	ldr	r3, [lr, #176]
	str	r3, [r4, #24]
	ldr	r3, [lr, #180]
	str	r3, [r4, #28]
	ldr	r3, [lr, #184]
	str	r3, [r4, #32]
	ldr	r3, [lr, #188]
	str	r3, [r4, #36]
	ldr	r3, [lr, #192]
	str	r3, [r4, #40]
.L416:
	rsb	r1, r4, #32768
	ldr	r3, .L441+20
	add	r1, r1, #208
	add	r1, r1, lr
	add	r0, r3, #128
.L417:
	ldr	r2, [r1, r3]
	ldr	ip, .L441+16
	str	r2, [r3, #4]!
	cmp	r3, r0
	bne	.L417
	cmp	r6, #0
	beq	.L422
.L423:
	and	r7, r7, #15
	cmp	r7, #5
	andeq	r9, r9, #1
	andne	r8, r8, #31
	moveq	r0, #0
	movne	r0, #0
	streq	r9, [r5]
	strne	r8, [r5]
.L429:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L422:
	movw	r2, #33152
	movw	r3, #33156
	ldr	r2, [lr, r2]
	str	r2, [ip, #48]
	ldr	r3, [lr, r3]
	str	r3, [ip, #44]
	b	.L423
.L425:
	mov	r6, r2
	b	.L409
.L439:
	ldr	r0, [r8, #32]
	ubfx	r7, r3, #0, #17
	bl	MEM_Phy2Vir
	subs	r2, r0, #0
	beq	.L440
	sub	r3, r7, #1
	ldr	lr, [r8]
	cmp	r3, #199
	subls	r3, r7, #-1073741823
	ldr	r7, [r4]
	movhi	r3, #0
	movhi	r1, #8
	movls	r3, r3, asl #4
	addls	r1, r3, #8
	ldr	r3, [r2, r3]
	str	r3, [r4, #16]
	ldr	r3, [r2, r1]
	str	r3, [r4, #20]
	ldr	r8, [lr, #208]
	str	r8, [r4, #8]
	ldr	r9, [lr, #212]
	str	r9, [r4, #12]
	b	.L416
.L438:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	lr, r0, #0
	beq	.L413
	str	lr, [r8]
	b	.L412
.L408:
	ldr	r1, .L441
	mov	r0, #0
	ldr	r2, .L441+24
	str	r0, [sp]
	ldr	ip, [r1, #68]
	ldr	r1, .L441+28
	blx	ip
	mvn	r0, #0
	b	.L429
.L437:
	ldr	r3, .L441
	mov	r0, #0
	ldr	r1, .L441+32
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L429
.L440:
	ldr	r1, .L441
	ldr	r3, .L441+36
.L434:
	ldr	ip, [r1, #68]
	ldr	r2, .L441+24
	ldr	r1, .L441+40
	blx	ip
	mvn	r0, #0
	b	.L429
.L413:
	ldr	r3, .L441
	ldr	r1, .L441+44
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L429
.L442:
	.align	2
.L441:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC39
	.word	g_DSPState
	.word	g_HwMem
	.word	g_BackUp
	.word	g_BackUp+48
	.word	.LANCHOR0+372
	.word	.LC5
	.word	.LC40
	.word	.LC41
	.word	.LC1
	.word	.LC30
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_BackupInfo, .-VDMHAL_V300R001_BackupInfo
	.align	2
	.global	VDMHAL_V300R001_ReadMsgSlot
	.type	VDMHAL_V300R001_ReadMsgSlot, %function
VDMHAL_V300R001_ReadMsgSlot:
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
	cmp	r1, #0
	cmpne	r0, #0
	mov	r3, r1
	moveq	r4, #1
	movne	r4, #0
	beq	.L447
	cmp	r2, #800
	ldr	r3, .L449
	bhi	.L448
	mov	r2, r2, asl #2
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L448:
	mov	r0, r4
	ldr	r1, .L449+4
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L447:
	ldr	ip, .L449
	mov	r2, r0
	ldr	r1, .L449+8
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L450:
	.align	2
.L449:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC43
	.word	.LC42
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_ReadMsgSlot, .-VDMHAL_V300R001_ReadMsgSlot
	.align	2
	.global	VDMHAL_V300R001_WriteMsgSlot
	.type	VDMHAL_V300R001_WriteMsgSlot, %function
VDMHAL_V300R001_WriteMsgSlot:
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
	cmp	r1, #0
	cmpne	r0, #0
	moveq	r4, #1
	movne	r4, #0
	beq	.L452
	sub	r3, r2, #1
	cmp	r3, #255
	bhi	.L452
	ldr	r3, .L455
	mov	r2, r2, asl #2
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L452:
	ldr	r3, .L455
	mov	r0, #0
	ldr	r1, .L455+4
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L456:
	.align	2
.L455:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC44
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_WriteMsgSlot, .-VDMHAL_V300R001_WriteMsgSlot
	.global	__aeabi_uidiv
	.global	__aeabi_uidivmod
	.align	2
	.global	VDMHAL_V300R001_CfgRpMsg
	.type	VDMHAL_V300R001_CfgRpMsg, %function
VDMHAL_V300R001_CfgRpMsg:
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
	str	r0, [fp, #-56]
	ldr	r0, [r1, #40]
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	str	r3, [fp, #-68]
	beq	.L490
	ldr	r3, [fp, #-56]
	ldr	r2, [r3, #840]
	sub	r3, r2, #1
	cmp	r3, #199
	bhi	.L491
	ldr	r3, [fp, #-56]
	ldr	r1, [r3, #24]
	ldr	r0, [r3, #864]
	ldr	ip, [r3]
	mov	r1, r1, asl r0
	ldr	r2, [r3, #28]
	add	r3, r1, #255
	cmp	ip, #17
	bic	lr, r3, #255
	mov	r3, r2, asl r0
	mov	r0, lr, asl #4
	beq	.L461
	adds	r2, r3, #31
	addmi	r2, r3, #62
	adds	ip, r1, #127
	addmi	r1, r1, #254
	movpl	r1, ip
	mov	ip, r2, asr #5
	mov	r1, r1, asr #7
	adds	r2, r1, #15
	addmi	r2, r1, #30
	mul	r3, lr, ip
	mov	r2, r2, asr #4
	mov	r2, r2, asl #9
	mul	r2, r2, ip
	add	r3, r2, r3, lsl #5
.L462:
	ldr	r4, [fp, #-56]
	mov	lr, #0	@ movhi
	ldr	r5, [fp, #-68]
	mov	ip, lr	@ movhi
	ldr	r1, [r4, #16]
	str	r1, [r5]
	ldr	r1, [r4, #16]
	add	r1, r3, r1
	str	r1, [r5, #4]
	ldr	r1, [r4, #4]
	str	r1, [r5, #8]
	ldr	r1, [r4, #4]
	add	r3, r3, r1
	str	r3, [r5, #12]
	str	r0, [r5, #16]
	str	r2, [r5, #20]
	ldr	r2, [r4, #24]
	ldr	r3, [r4, #28]
	sub	r2, r2, #1
	sub	r3, r3, #1
	bfi	lr, r2, #0, #9
	bfi	ip, r3, #0, #9
	strh	lr, [fp, #-48]	@ movhi
	strh	ip, [fp, #-46]	@ movhi
	ldr	r3, [fp, #-48]
	str	r3, [r5, #24]
	ldr	r3, [r4, #852]
	ldr	r2, [r4, #24]
	sub	r3, r3, #1
	cmp	r3, #1
	str	r2, [fp, #-76]
	movls	r3, r4
	ldrhi	r3, [fp, #-56]
	ldrls	r3, [r3, #28]
	ldrhi	r3, [r3, #28]
	addls	r3, r3, r3, lsr #31
	movls	r3, r3, asr #1
	str	r3, [fp, #-88]
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #864]
	cmp	r3, #6
	moveq	r4, #2
	beq	.L465
	cmp	r3, #5
	moveq	r4, #4
	beq	.L465
	cmp	r3, #4
	moveq	r4, #8
	beq	.L465
	ldr	r3, .L495
	mov	r0, #1
	ldr	r1, .L495+4
	mov	r4, r0
	ldr	r3, [r3, #68]
	blx	r3
.L465:
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #856]
	cmp	r3, #0
	bne	.L483
	ldr	r2, [fp, #-56]
	ldr	r8, [r2, #840]
	cmp	r8, #0
	ble	.L484
	ldr	r2, [fp, #-76]
	mov	r6, r3
	ldr	r1, [fp, #-88]
	sub	r7, r4, #1
	str	r3, [fp, #-60]
	str	r3, [fp, #-72]
	mul	r2, r2, r1
	sub	r3, r2, #1
	str	r3, [fp, #-84]
	str	r2, [fp, #-80]
	b	.L478
.L469:
	ldr	r3, [fp, #-56]
	add	r6, r6, #1
	ldr	r3, [r3, #840]
	cmp	r3, r6
	mov	r8, r3
	ble	.L492
.L478:
	ldr	r3, [fp, #-56]
	mov	r1, r4
	add	r5, r3, r6, lsl #2
	ldrsh	r9, [r5, #42]
	ldrh	r3, [r5, #40]
	sub	r0, r9, #1
	add	r0, r0, r4
	str	r3, [fp, #-64]
	bl	__aeabi_uidiv
	cmp	r6, #0
	mul	r10, r4, r0
	ble	.L468
	ldrsh	r0, [r5, #38]
	mov	r1, r4
	sub	r0, r0, #1
	add	r0, r0, r4
	bl	__aeabi_uidiv
	mul	r3, r4, r0
	cmp	r10, r3
	str	r3, [fp, #-72]
	ble	.L469
	b	.L470
.L483:
	cmp	r3, #1
	mov	r2, #0
	str	r2, [fp, #-60]
	beq	.L477
.L467:
	ldr	r3, [fp, #-60]
	sub	r3, r3, #1
	uxth	r3, r3
.L479:
	ldr	lr, [fp, #-56]
	mov	r0, #0
	ldrb	r2, [fp, #-46]	@ zero_extendqisi2
	strh	r3, [fp, #-48]	@ movhi
	ldr	ip, [lr, #848]
	ldr	r1, [lr, #852]
	bfi	r2, ip, #0, #1
	ldr	ip, [lr, #864]
	and	r1, r1, #3
	ldrb	lr, [fp, #-45]	@ zero_extendqisi2
	bfi	r2, r1, #4, #2
	sub	ip, ip, #4
	bfi	lr, ip, #0, #2
	strb	lr, [fp, #-45]
	mov	r3, r2
	bfi	r3, r1, #6, #2
	strb	r3, [fp, #-46]
	ldr	r3, [fp, #-48]
	ldr	r2, [fp, #-68]
	str	r3, [r2, #28]
.L489:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L471:
	bl	__aeabi_uidiv
	mov	r1, r4
	mul	r9, r4, r0
	ldrsh	r0, [r5, #44]
	bl	__aeabi_uidiv
	mul	r0, r4, r0
	cmp	r9, r0
	add	r2, r0, #1
	bhi	.L473
	cmp	r9, r2
	mov	r1, r4
	beq	.L473
	ldrsh	r0, [r5, #38]
	add	r0, r0, r7
	bl	__aeabi_uidiv
	mul	r0, r4, r0
	cmp	r9, r0
	bhi	.L493
.L473:
	ldrh	r9, [r5, #46]
	add	r5, r5, #4
	mov	r6, r10
	sxth	r9, r9
.L470:
	add	r10, r6, #1
	mov	r1, r4
	cmp	r10, r8
	add	r0, r7, r9
	blt	.L471
	bl	__aeabi_uidiv
	mul	r10, r4, r0
.L472:
	ldrsh	r0, [fp, #-64]
	mov	r1, r4
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-80]
	ldr	r3, [fp, #-84]
	cmp	r2, r10
	movls	r10, r3
	ldr	r3, [fp, #-60]
	add	r3, r3, #1
	str	r3, [fp, #-60]
	mul	r5, r4, r0
	cmp	r10, r5
	movlt	r5, #0
	cmp	r3, #252
	bgt	.L494
	ldr	r9, [fp, #-76]
	mov	r0, r5
	mov	r1, r9
	bl	__aeabi_uidivmod
	mov	r2, #0
	mov	ip, r2, lsr #16
	mov	r0, r5
	mov	r3, #0
	mov	r5, r2	@ movhi
	mov	r8, ip
	str	ip, [fp, #-64]
	bfi	r3, r1, #0, #9
	mov	r1, r9
	strh	r3, [fp, #-48]	@ movhi
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-60]
	mov	r1, r9
	ldr	ip, [fp, #-64]
	add	r3, r3, #3
	mov	r2, r3, asl #3
	str	r2, [fp, #-64]
	ldr	r2, [fp, #-68]
	bfi	ip, r0, #0, #9
	strh	ip, [fp, #-46]	@ movhi
	ldr	ip, [fp, #-48]
	mov	r0, r10
	str	ip, [r2, r3, asl #3]
	bl	__aeabi_uidivmod
	mov	r0, r10
	bfi	r5, r1, #0, #9
	mov	r1, r9
	strh	r5, [fp, #-48]	@ movhi
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-64]
	ldr	r1, [fp, #-68]
	add	r2, r2, #4
	bfi	r8, r0, #0, #9
	strh	r8, [fp, #-46]	@ movhi
	ldr	r3, [fp, #-48]
	str	r3, [r1, r2]
	b	.L469
.L468:
	ldr	r3, [fp, #-72]
	cmp	r10, r3
	movgt	r3, #0
	movle	r3, #1
	cmp	r6, #0
	moveq	r3, #0
	cmp	r3, #0
	bne	.L469
	b	.L472
.L493:
	mov	r10, r9
	b	.L472
.L492:
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #856]
	cmp	r3, #1
	bne	.L467
.L477:
	ldr	r3, [fp, #-76]
	mov	ip, #0	@ movhi
	mov	r0, ip	@ movhi
	sub	r2, r3, #1
	ldr	r3, [fp, #-88]
	sub	r1, r3, #1
	mov	r3, #0
	bfi	ip, r3, #0, #9
	bfi	r0, r3, #0, #9
	strh	ip, [fp, #-48]	@ movhi
	strh	r0, [fp, #-46]	@ movhi
	mov	r0, #0	@ movhi
	ldr	ip, [fp, #-48]
	bfi	r0, r2, #0, #9
	strh	r0, [fp, #-48]	@ movhi
	mov	r2, #0	@ movhi
	ldr	r0, [fp, #-56]
	bfi	r2, r1, #0, #9
	strh	r2, [fp, #-46]	@ movhi
	mov	r1, #1
	ldr	r2, [fp, #-48]
	str	r1, [r0, #840]
	ldr	r1, [fp, #-68]
	str	ip, [r1, #32]
	str	r2, [r1, #36]
	b	.L479
.L461:
	mul	r3, r3, lr
	mov	r2, #0
	b	.L462
.L494:
	ldr	r2, [fp, #-56]
	mov	r3, #1
	str	r3, [r2, #856]
	b	.L477
.L484:
	str	r3, [fp, #-60]
	b	.L467
.L491:
	ldr	r3, .L495
	mov	r0, #0
	ldr	r1, .L495+8
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L489
.L490:
	ldr	r1, .L495
	ldr	r3, .L495+12
	ldr	r2, .L495+16
	ldr	ip, [r1, #68]
	ldr	r1, .L495+20
	blx	ip
	mvn	r0, #0
	b	.L489
.L496:
	.align	2
.L495:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC47
	.word	.LC46
	.word	.LC45
	.word	.LANCHOR0+400
	.word	.LC1
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_CfgRpMsg, .-VDMHAL_V300R001_CfgRpMsg
	.align	2
	.global	VDMHAL_V300R001_CfgRpReg
	.type	VDMHAL_V300R001_CfgRpReg, %function
VDMHAL_V300R001_CfgRpReg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #16)
	sub	sp, sp, #16
	cmp	r3, #1
	mov	r4, r3
	bhi	.L506
	cmp	r3, #0
	bgt	.L507
	ldr	r3, [r1]
	cmp	r3, #0
	beq	.L508
.L502:
	movw	r3, #1208
	ldr	r2, .L509
	mul	r3, r3, r4
	ldr	ip, [r1, #40]
	mov	r4, #7
	movw	lr, #49667
	bic	ip, ip, #15
	movt	lr, 8192
	movw	r1, #3075
	mov	r0, #0
	movt	r1, 48
	ldr	r5, [r2, r3]
	str	ip, [r5, #16]
	ldr	ip, [r2, r3]
	str	r4, [ip, #36]
	ldr	ip, [r2, r3]
	str	lr, [ip, #12]
	ldr	ip, [r2, r3]
	str	r1, [ip, #60]
	ldr	ip, [r2, r3]
	str	r1, [ip, #64]
	ldr	ip, [r2, r3]
	str	r1, [ip, #68]
	ldr	ip, [r2, r3]
	str	r1, [ip, #72]
	ldr	ip, [r2, r3]
	str	r1, [ip, #76]
	ldr	ip, [r2, r3]
	str	r1, [ip, #80]
	ldr	r3, [r2, r3]
	str	r1, [r3, #84]
.L505:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L508:
	mov	r0, #0
	str	r1, [fp, #-24]
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L503
	ldr	r1, [fp, #-24]
	str	r3, [r1]
	b	.L502
.L507:
	ldr	r1, .L509+4
	mov	r2, #0
	mov	r0, r2
	str	r2, [sp]
	ldr	r2, .L509+8
	ldr	ip, [r1, #68]
	ldr	r1, .L509+12
	blx	ip
	mvn	r0, #0
	b	.L505
.L506:
	ldr	r3, .L509+4
	mov	r0, #0
	ldr	r1, .L509+16
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L505
.L503:
	ldr	r3, .L509+4
	ldr	r1, .L509+20
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L505
.L510:
	.align	2
.L509:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+428
	.word	.LC5
	.word	.LC48
	.word	.LC30
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_CfgRpReg, .-VDMHAL_V300R001_CfgRpReg
	.align	2
	.global	VDMHAL_V300R001_MakeDecReport
	.type	VDMHAL_V300R001_MakeDecReport, %function
VDMHAL_V300R001_MakeDecReport:
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
	UNWIND(.pad #16)
	sub	sp, sp, #16
	subs	r3, r0, #0
	ldreq	r1, .L544
	ldreq	r3, .L544+4
	beq	.L536
	ldmia	r3, {r8, r9}
	ldr	r5, [r3, #8]
	cmp	r9, #0
	beq	.L538
	movw	r3, #1208
	ldr	r7, .L544+8
	mul	r3, r3, r5
	ldr	r0, [r7, r3]
	cmp	r0, #0
	beq	.L515
	ldr	r3, .L544+12
	ldr	r2, [r3]
	cmp	r2, #1
	addne	r1, r3, #60
	beq	.L530
.L518:
	ldr	r2, [r3, #4]!
	cmp	r2, #1
	beq	.L530
	cmp	r3, r1
	bne	.L518
	mov	r4, #0
.L516:
	ldr	r6, .L544
	mov	r2, #816
	mov	r1, #0
	mov	r0, r9
	ldr	r3, [r6, #48]
	blx	r3
	movw	r2, #1208
	mla	r2, r2, r5, r7
	ldr	r1, .L544+16
	mov	r3, #180
	mla	r3, r3, r5, r1
	ldr	r2, [r2, #24]
	str	r2, [r9, #4]
	ldr	r3, [r3, #4]
	mov	r3, r3, lsr #17
	and	r2, r3, #3
	cmp	r2, #1
	moveq	r3, #0
	beq	.L519
	eor	r3, r3, #1
	and	r3, r3, #1
	cmp	r8, #3
	orrne	r3, r3, #1
.L519:
	mov	r2, #180
	str	r3, [r9]
	mla	r3, r2, r5, r1
	bics	r8, r8, #16
	ldr	r2, [r3, #4]
	ubfx	r2, r2, #0, #17
	str	r2, [r9, #8]
	beq	.L539
.L520:
	cmp	r2, #200
	bhi	.L540
.L522:
	movw	r3, #1208
	mla	r7, r3, r5, r7
	ldr	r8, [r7, #32]
	mov	r0, r8
	bl	MEM_Phy2Vir
	subs	r1, r0, #0
	beq	.L541
	ldr	r7, .L544+20
	mov	r3, #3200
	ldr	r2, [r9, #8]
	mla	r4, r3, r5, r7
	mov	r2, r2, asl #2
	mov	r0, r4
	bl	VDMHAL_V300R001_ReadMsgSlot
	ldr	ip, [r9, #8]
	cmp	ip, #0
	movne	r3, r4
	movne	r2, r9
	movne	r1, #0
	beq	.L527
.L526:
	ldr	r0, [r3, #4]
	add	r1, r1, #1
	cmp	r1, ip
	add	r3, r3, #16
	add	r2, r2, #4
	strh	r0, [r2, #8]	@ movhi
	ldr	r0, [r3, #-8]
	strh	r0, [r2, #10]	@ movhi
	bne	.L526
.L527:
	mov	r0, #6
	bl	IsDprintTypeEnable
	cmp	r0, #0
	bne	.L542
.L513:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L539:
	cmp	r4, #1
	beq	.L543
	ldr	r1, .L544+24
	ldrb	r1, [r1]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L520
	ldrb	r3, [r3, #19]	@ zero_extendqisi2
	and	r3, r3, #3
	cmp	r3, #2
	bne	.L520
	mov	r3, #1
	strh	r1, [r9, #12]	@ movhi
	mov	r0, r1
	strh	r1, [r9, #14]	@ movhi
	str	r3, [r9, #8]
	b	.L513
.L530:
	mov	r4, r2
	b	.L516
.L542:
	ldr	r3, [r6, #68]
	mov	r0, #6
	ldr	r2, [r9, #8]
	mov	r4, #0
	ldr	r1, .L544+28
	blx	r3
	ldr	r3, [r6, #68]
	mov	r2, r8
	mov	r0, #6
	ldr	r1, .L544+32
	blx	r3
	mov	r3, #3200
	mla	r5, r3, r5, r7
.L528:
	ldr	ip, [r5, #8]
	ldr	r1, [r5, #4]
	mov	r2, r4
	ldr	lr, [r5, #12]
	mov	r0, #6
	ldr	r3, [r5]
	add	r4, r4, #4
	str	ip, [sp, #4]
	add	r5, r5, #16
	str	r1, [sp]
	str	lr, [sp, #8]
	ldr	ip, [r6, #68]
	ldr	r1, .L544+36
	blx	ip
	ldr	r3, [r9, #8]
	ldr	r2, .L544
	mov	r3, r3, asl #2
	sub	r3, r3, #3
	cmp	r3, r4
	bhi	.L528
	ldr	r3, [r2, #68]
	mov	r0, #6
	ldr	r1, .L544+40
	blx	r3
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L540:
	mov	r3, #200
	ldr	ip, [r6, #68]
	ldr	r1, .L544+44
	mov	r0, #1
	blx	ip
	mov	r3, #0
	str	r3, [r9, #8]
	b	.L522
.L543:
	ldr	r3, [r3, #16]
	ubfx	r3, r3, #21, #2
	cmp	r3, #2
	bne	.L520
	str	r4, [r9, #8]
	mov	r0, r8
	strh	r8, [r9, #12]	@ movhi
	strh	r8, [r9, #14]	@ movhi
	b	.L513
.L538:
	ldr	r1, .L544
	mov	r0, r9
	ldr	r3, .L544+48
.L536:
	ldr	ip, [r1, #68]
	ldr	r2, .L544+52
	ldr	r1, .L544+56
	blx	ip
	mvn	r0, #0
	b	.L513
.L541:
	ldr	ip, [r6, #68]
	ldr	r3, .L544+60
	ldr	r2, .L544+52
	ldr	r1, .L544+56
	blx	ip
	mvn	r0, #0
	b	.L513
.L515:
	ldr	r1, .L544
	ldr	r3, .L544+64
	b	.L536
.L545:
	.align	2
.L544:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC49
	.word	g_HwMem
	.word	g_DSPState
	.word	g_BackUp
	.word	g_UpMsg
	.word	g_not_allow_H264FullPictureRepair_flag
	.word	.LC52
	.word	.LC53
	.word	.LC54
	.word	.LC55
	.word	.LC51
	.word	.LC50
	.word	.LANCHOR0+456
	.word	.LC1
	.word	.LC41
	.word	.LC36
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_MakeDecReport, .-VDMHAL_V300R001_MakeDecReport
	.align	2
	.global	VDMHAL_V300R001_PrepareRepair
	.type	VDMHAL_V300R001_PrepareRepair, %function
VDMHAL_V300R001_PrepareRepair:
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
	movw	r5, #1208
	mul	r5, r5, r3
	ldr	r6, .L566
	cmp	r3, #1
	mov	r4, r3
	mov	r8, r0
	mov	r9, r1
	add	r7, r5, r6
	bhi	.L562
	cmp	r3, #0
	bgt	.L563
	ldr	r3, [r5, r6]
	cmp	r3, #0
	beq	.L564
.L551:
	cmp	r2, #0
	bne	.L553
	ldr	r5, .L566+4
	mov	r3, #1744
	mla	r3, r3, r4, r5
	ldr	r1, [r3, #840]
	cmp	r1, #0
	ble	.L554
	cmp	r8, #6
	bne	.L555
	ldr	r1, [r9, #32]
	cmp	r1, #1
	moveq	r2, r1
	str	r2, [r3, #1724]
.L555:
	mov	r2, r9
	mov	r3, r4
	mov	r1, r7
	mov	r0, r8
	bl	VDMHAL_V300R001_CfgRpReg
	mov	r0, #1744
	mov	r1, r7
	mov	r2, r4
	mla	r0, r0, r4, r5
	bl	VDMHAL_V300R001_CfgRpMsg
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L553:
	cmp	r2, #1
	movne	r0, #0
	beq	.L565
.L549:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L565:
	cmp	r8, #6
	bne	.L557
	ldr	r3, [r9, #32]
	cmp	r3, #1
	bne	.L557
	ldr	r5, .L566+4
	mov	r3, #1744
	mla	r3, r3, r4, r5
	ldr	r2, [r3, #1712]
	cmp	r2, #0
	ble	.L557
	ldr	r2, .L566+8
	mov	r1, #2
	str	r1, [r3, #1724]
	ldr	r3, [r2, #56]
	cmp	r3, #0
	beq	.L558
	mov	r0, r4
	blx	r3
.L559:
	mov	r2, r9
	mov	r3, r4
	mov	r1, r7
	mov	r0, #6
	bl	VDMHAL_V300R001_CfgRpReg
	mov	r0, #1744
	mla	r0, r0, r4, r5
	mov	r1, r7
	mov	r2, r4
	add	r0, r0, #872
	bl	VDMHAL_V300R001_CfgRpMsg
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L564:
	mov	r0, #0
	str	r2, [fp, #-40]
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L552
	str	r3, [r5, r6]
	ldr	r2, [fp, #-40]
	b	.L551
.L563:
	ldr	r1, .L566+12
	mov	r2, #0
	mov	r0, r2
	str	r2, [sp]
	ldr	r2, .L566+16
	ldr	ip, [r1, #68]
	ldr	r1, .L566+20
	blx	ip
	mvn	r0, #0
	b	.L549
.L557:
	ldr	r3, .L566+12
	mov	r0, #0
	ldr	r1, .L566+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L549
.L558:
	ldr	ip, .L566+12
	mov	r0, r3
	movw	r2, #2872
	ldr	r1, .L566+28
	ldr	r3, [ip, #68]
	blx	r3
	b	.L559
.L562:
	ldr	r3, .L566+12
	mov	r0, #0
	ldr	r1, .L566+32
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L549
.L554:
	ldr	r3, .L566+12
	mov	r0, r2
	ldr	r1, .L566+36
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L549
.L552:
	ldr	r3, .L566+12
	ldr	r1, .L566+40
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L549
.L567:
	.align	2
.L566:
	.word	g_HwMem
	.word	g_RepairParam
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+488
	.word	.LC5
	.word	.LC59
	.word	.LC10
	.word	.LC56
	.word	.LC58
	.word	.LC57
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_PrepareRepair, .-VDMHAL_V300R001_PrepareRepair
	.align	2
	.global	VDMHAL_V300R001_StartHwRepair
	.type	VDMHAL_V300R001_StartHwRepair, %function
VDMHAL_V300R001_StartHwRepair:
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	subs	r4, r0, #0
	ble	.L571
	ldr	r5, .L572
	mov	r6, #1
	mov	r3, r4
	ldr	r2, .L572+4
	str	r6, [sp]
	mov	r0, #32
	ldr	ip, [r5, #68]
	ldr	r1, .L572+8
	blx	ip
	movw	r0, #49156
	movt	r0, 63683
	bl	MEM_ReadPhyWord
	ldr	r3, .L572+12
	ldr	r3, [r3]
	uxth	r1, r0
	add	r0, r3, #4
	bl	MEM_WritePhyWord
	str	r6, [sp]
	mov	r3, r4
	ldr	ip, [r5, #68]
	ldr	r2, .L572+4
	mov	r0, #32
	ldr	r1, .L572+16
	blx	ip
	ldr	r3, [r5, #112]
	blx	r3
	str	r6, [sp]
	mov	r3, r4
	ldr	ip, [r5, #68]
	ldr	r2, .L572+4
	mov	r0, #32
	ldr	r1, .L572+16
	blx	ip
	str	r6, [sp]
	mov	r3, r4
	ldr	ip, [r5, #68]
	ldr	r2, .L572+4
	mov	r0, #32
	ldr	r1, .L572+16
	blx	ip
	str	r6, [sp]
	mov	r3, r4
	ldr	ip, [r5, #68]
	ldr	r2, .L572+4
	mov	r0, #32
	ldr	r1, .L572+16
	blx	ip
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L571:
	movw	r3, #1208
	ldr	r5, .L572+20
	mul	r4, r3, r4
	movw	r0, #49156
	movt	r0, 63683
	ldr	r3, [r5, r4]
	ldr	r6, [r3, #8]
	bl	MEM_ReadPhyWord
	ldr	r3, .L572+12
	orr	r6, r6, #67108864
	ldr	r3, [r3]
	uxth	r1, r0
	add	r0, r3, #4
	bl	MEM_WritePhyWord
	ldr	r2, [r5, r4]
	ldr	r3, .L572
	str	r6, [r2, #8]
	ldr	r3, [r3, #112]
	blx	r3
	ldr	r2, [r5, r4]
	mov	r3, #0
	mov	r1, #1
	str	r3, [r2]
	ldr	r2, [r5, r4]
	str	r1, [r2]
	ldr	r2, [r5, r4]
	str	r3, [r2]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L573:
	.align	2
.L572:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+520
	.word	.LC37
	.word	s_RegPhyBaseAddr
	.word	.LC60
	.word	g_HwMem
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_StartHwRepair, .-VDMHAL_V300R001_StartHwRepair
	.align	2
	.global	VDMHAL_V300R001_SetVdhClkSkip
	.type	VDMHAL_V300R001_SetVdhClkSkip, %function
VDMHAL_V300R001_SetVdhClkSkip:
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
	ldr	r9, .L603
	ldr	r4, [r9, #64]
	cmp	r4, #0
	blt	.L597
	movw	r10, #56367
	add	r6, r9, #64
	add	r9, r9, #124
	movt	r10, 1315
	mov	r8, #30
	mov	r7, #0
.L576:
	mov	r0, r4
	bl	VCTRL_IsChanActive
	cmp	r0, #0
	bne	.L579
	ldr	r3, .L603+4
	mov	r0, r4
	ldr	r2, .L603+8
	ldr	r3, [r3, r4, asl #2]
	ldr	ip, [r2, r4, asl #2]
	ldr	r1, [r3, #44]
	ldr	r2, [r3, #48]
	adds	r4, r1, #15
	ldr	r3, [ip, #1448]
	addmi	r4, r1, #30
	adds	r5, r2, #15
	addmi	r5, r2, #30
	cmp	r3, #30
	mov	r4, r4, asr #4
	movgt	r8, r3
	mov	r5, r5, asr #4
	bl	VCTRL_GetVidStd
	cmp	r0, #17
	ldrls	pc, [pc, r0, asl #2]
	b	.L579
.L582:
	.word	.L591
	.word	.L591
	.word	.L591
	.word	.L591
	.word	.L579
	.word	.L591
	.word	.L591
	.word	.L579
	.word	.L591
	.word	.L591
	.word	.L591
	.word	.L591
	.word	.L591
	.word	.L591
	.word	.L579
	.word	.L579
	.word	.L592
	.word	.L591
.L591:
	mul	r4, r5, r4
	mul	r2, r8, r4
	ubfx	r2, r2, #1, #26
	umull	r2, r3, r2, r10
	add	r7, r7, r3, lsr #7
.L579:
	cmp	r6, r9
	beq	.L595
	ldr	r4, [r6, #4]!
	cmp	r4, #0
	bge	.L576
.L595:
	rsb	r7, r7, #32
	cmp	r7, #25
	movge	r7, #25
	cmp	r7, #0
	andgt	r7, r7, #31
	ble	.L602
.L575:
	ldr	r3, .L603+12
	ldr	r2, [r3]
	ldr	r3, [r2, #120]
	bfi	r3, r7, #12, #5
.L600:
	bfc	r3, #17, #1
	str	r3, [r2, #120]
	ldr	r3, [r2, #120]
	orr	r3, r3, #131072
	str	r3, [r2, #120]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L592:
	mul	r4, r5, r4
	mul	r2, r8, r4
	bic	r2, r2, #-67108864
	umull	r2, r3, r2, r10
	add	r7, r7, r3, lsr #7
	b	.L579
.L602:
	ldr	r3, .L603+12
	ldr	r2, [r3]
	ldr	r3, [r2, #120]
	bfc	r3, #12, #5
	b	.L600
.L597:
	mov	r7, #25
	b	.L575
.L604:
	.align	2
.L603:
	.word	g_ChanCtx
	.word	s_pFspInst
	.word	s_pstVfmwChan
	.word	g_pstRegCrg
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_SetVdhClkSkip, .-VDMHAL_V300R001_SetVdhClkSkip
	.align	2
	.global	VDMHAL_V300R001_StartHwDecode
	.type	VDMHAL_V300R001_StartHwDecode, %function
VDMHAL_V300R001_StartHwDecode:
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r4, .L624
	mov	r6, r0, asl #3
	mov	r5, r0, asl #6
	rsb	r3, r6, r5
	ldr	r9, .L624+4
	add	r3, r4, r3
	ldr	r10, .L624+8
	mov	r7, r0
	ldr	r8, .L624+12
	ldr	r2, [r3, #8]
	ldr	r0, [r3, #12]
	ldr	r3, [r10]
	ldr	r1, [r9, r2, asl #2]
	ldr	r2, [r3, #120]
	ldr	r1, [r1, #564]
	sub	r1, r1, #10
	cmp	r1, #13
	cmpls	r0, #0
	mov	r0, #30
	orreq	r2, r2, #768
	movne	r1, #0
	bfine	r2, r1, #8, #2
	str	r2, [r3, #120]
	moveq	r1, #1
	ldr	r3, [r8, #116]
	blx	r3
	rsb	r3, r6, r5
	add	r3, r4, r3
	ldr	r3, [r3, #8]
	ldr	r3, [r9, r3, asl #2]
	ldr	r2, [r3, #1232]
	cmp	r2, #1
	beq	.L608
	ldr	r3, [r3, #1220]
	bics	r3, r3, #1024
	beq	.L609
.L608:
	ldr	r3, [r10]
	ldr	r2, [r3, #120]
	bfc	r2, #12, #5
	bfc	r2, #17, #1
	str	r2, [r3, #120]
	ldr	r2, [r3, #120]
	orr	r2, r2, #131072
	str	r2, [r3, #120]
.L610:
	cmp	r7, #0
	beq	.L612
	cmp	r7, #1
	bne	.L623
	ldr	r3, .L624+16
	mov	r1, #3
	ldr	r0, [r3]
	add	r0, r0, #12
	bl	MEM_WritePhyWord
	ldr	r3, [r8, #112]
	blx	r3
.L615:
	mov	r0, #0
	ldr	ip, [r8, #68]
	mov	r3, r7
	str	r0, [sp]
	ldr	r2, .L624+20
	ldr	r1, .L624+24
	blx	ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L623:
	ldr	r3, [r8, #68]
	mov	r0, #0
	ldr	r1, .L624+28
	blx	r3
	ldr	r3, [r8, #112]
	blx	r3
	cmp	r7, #0
	bgt	.L615
.L614:
	movw	r3, #1208
	ldr	r2, .L624+32
	mul	r7, r3, r7
	mov	r3, #0
	rsb	r5, r6, r5
	mov	lr, #1
	add	r4, r4, r5
	ldr	ip, .L624+36
	mov	r1, #4
	ldr	r0, [r2, r7]
	str	r3, [r0]
	ldr	r0, [r2, r7]
	str	lr, [r0]
	ldr	r2, [r2, r7]
	str	r3, [r2]
	ldr	r0, [r4, #8]
	ldr	r2, [ip, r0, asl #2]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	VDEC_Lowdelay_Event_Time
.L612:
	ldr	r3, .L624+40
	mov	r1, #3
	ldr	r0, [r3]
	add	r0, r0, #12
	bl	MEM_WritePhyWord
	ldr	r3, [r8, #112]
	blx	r3
	b	.L614
.L609:
	mov	r0, r7
	bl	VDMHAL_V300R001_SetVdhClkSkip
	b	.L610
.L625:
	.align	2
.L624:
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	g_pstRegCrg
	.word	vfmw_Osal_Func_Ptr_S
	.word	s_RegPhyBaseAddr_1
	.word	.LANCHOR0+552
	.word	.LC5
	.word	.LC61
	.word	g_HwMem
	.word	g_LowDelaySeqIndex
	.word	s_RegPhyBaseAddr
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_StartHwDecode, .-VDMHAL_V300R001_StartHwDecode
	.align	2
	.global	VDMHAL_V300R001_GetCharacter
	.type	VDMHAL_V300R001_GetCharacter, %function
VDMHAL_V300R001_GetCharacter:
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
	ldr	r1, .L627
	mov	r0, #15
	ldr	r3, .L627+4
	mov	r2, #4
	str	r0, [r1]
	str	r2, [r3]
	ldmfd	sp, {fp, sp, pc}
.L628:
	.align	2
.L627:
	.word	g_VdmCharacter
	.word	g_eVdmVersion
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_GetCharacter, .-VDMHAL_V300R001_GetCharacter
	.align	2
	.global	VDMHAL_V300R001_WriteBigTitle1DYuv
	.type	VDMHAL_V300R001_WriteBigTitle1DYuv, %function
VDMHAL_V300R001_WriteBigTitle1DYuv:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 72
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #84)
	sub	sp, sp, #84
	mov	r9, r3
	ubfx	r3, r3, #29, #2
	mov	r4, #0
	cmp	r3, #1
	str	r0, [fp, #-92]
	mov	r7, r1
	mov	r6, r2
	str	r4, [fp, #-76]
	str	r4, [fp, #-72]
	str	r4, [fp, #-68]
	str	r4, [fp, #-64]
	str	r4, [fp, #-60]
	str	r4, [fp, #-56]
	str	r4, [fp, #-52]
	str	r4, [fp, #-48]
	beq	.L630
	cmp	r3, #2
	moveq	r4, #1
	movne	r4, #2
.L630:
	ldr	r3, [fp, #-92]
	cmp	r3, #0
	beq	.L629
	mov	r3, #0
	mov	r2, #4194304
	mov	r1, r3
	str	r3, [sp]
	ldr	r0, .L723
	sub	r3, fp, #76
	bl	MEM_AllocMemBlock
	subs	r1, r0, #0
	beq	.L719
.L716:
	ldr	r3, .L723+4
	mov	r0, #1
	ldr	r1, .L723+8
	ldr	r3, [r3, #68]
	blx	r3
.L629:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L719:
	str	r1, [sp]
	mov	r2, #4194304
	sub	r3, fp, #60
	ldr	r0, .L723+12
	bl	MEM_AllocMemBlock
	cmp	r0, #0
	bne	.L716
	add	r10, r9, #15
	add	r5, r6, #15
	bic	r3, r10, #15
	str	r3, [fp, #-84]
	bic	r5, r5, #15
	mov	ip, r9, lsr #1
	mov	r1, r3
	adds	r3, r3, #31
	addmi	r3, r1, #62
	add	r2, r5, #255
	bic	r2, r2, #255
	ldr	r1, [fp, #-56]
	mov	r3, r3, asr #5
	cmp	r4, #0
	cmpne	r4, #3
	str	ip, [fp, #-100]
	mov	ip, r6, lsr #1
	mla	r3, r3, r2, r7
	str	ip, [fp, #-104]
	str	r1, [fp, #-96]
	mov	ip, r2, asl #4
	ldr	r6, [fp, #-72]
	str	ip, [fp, #-88]
	str	r3, [fp, #-112]
	add	r3, r1, #2097152
	str	r3, [fp, #-108]
	bne	.L635
	ldr	r3, [fp, #-84]
	cmp	r3, #0
	movne	r8, r0
	ldrne	r10, .L723+4
	strne	r8, [fp, #-80]
	beq	.L720
.L636:
	cmp	r5, #0
	beq	.L640
	ldr	r2, [fp, #-80]
	mov	r4, #0
	and	r9, r2, #15
	mov	r3, r2, lsr #4
	ldr	r2, [fp, #-88]
	mul	r3, r2, r3
	add	r9, r3, r9, lsl #8
.L638:
	mov	r1, r4, lsr #8
	add	r0, r8, r4
	add	r0, r6, r0
	add	r4, r4, #256
	add	r1, r9, r1, lsl #12
	ldr	r3, [r10, #52]
	add	r1, r7, r1
	mov	r2, #256
	blx	r3
	cmp	r5, r4
	bhi	.L638
.L640:
	ldr	r3, [fp, #-80]
	add	r8, r8, r5
	ldr	r2, [fp, #-84]
	add	r3, r3, #1
	str	r3, [fp, #-80]
	cmp	r2, r3
	bne	.L636
	ldr	r4, [fp, #-84]
	mov	r0, r6
	ldr	ip, [r10, #44]
	ldr	r2, [fp, #-92]
	mul	r1, r4, r5
	blx	ip
	movs	r3, r4, lsr #1
	str	r3, [fp, #-84]
	beq	.L672
	ldr	r3, [fp, #-88]
	mov	r7, #0
	ldr	r8, [fp, #-112]
	str	r7, [fp, #-80]
	mov	r3, r3, asr #1
	str	r3, [fp, #-88]
.L642:
	cmp	r5, #0
	beq	.L644
	ldr	r2, [fp, #-80]
	mov	r4, #0
	and	r9, r2, #7
	mov	r3, r2, lsr #3
	ldr	r2, [fp, #-88]
	mul	r3, r2, r3
	add	r9, r3, r9, lsl #8
.L643:
	mov	r1, r4, lsr #8
	add	r0, r7, r4
	add	r0, r6, r0
	add	r4, r4, #256
	add	r1, r9, r1, lsl #11
	ldr	r3, [r10, #52]
	add	r1, r8, r1
	mov	r2, #256
	blx	r3
	cmp	r5, r4
	bhi	.L643
.L644:
	ldr	r3, [fp, #-80]
	add	r7, r7, r5
	ldr	r2, [fp, #-84]
	add	r3, r3, #1
	str	r3, [fp, #-80]
	cmp	r3, r2
	bne	.L642
.L672:
	ldr	r3, [fp, #4]
	cmp	r3, #1
	beq	.L721
	ldr	r3, [fp, #4]
	cmp	r3, #0
	beq	.L669
.L664:
	ldr	r3, [fp, #-100]
	ldr	r2, [fp, #-104]
	ldr	r5, [fp, #-92]
	ldr	ip, [r10, #44]
	mul	r4, r3, r2
	ldr	r0, [fp, #-96]
	mov	r2, r5
	mov	r1, r4
	blx	ip
	mov	r2, r5
	ldr	ip, [r10, #44]
	mov	r1, r4
	ldr	r0, [fp, #-108]
	blx	ip
	ldr	r1, [fp, #-72]
	ldr	r0, [fp, #-68]
	bl	MEM_ReleaseMemBlock
	ldr	ip, [r10, #48]
	mov	r2, #16
	sub	r0, fp, #76
	mov	r1, #0
	blx	ip
	ldr	r1, [fp, #-56]
	ldr	r0, [fp, #-52]
	bl	MEM_ReleaseMemBlock
	ldr	r3, [r10, #48]
	sub	r0, fp, #60
	mov	r2, #16
	mov	r1, #0
	blx	r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L635:
	subs	r3, r4, #1
	ldr	r2, [fp, #-84]
	ldr	r10, .L723+4
	movne	r3, #1
	cmp	r2, r3
	str	r3, [fp, #-116]
	ldrhi	r3, [fp, #-116]
	strhi	r3, [fp, #-80]
	bls	.L654
.L656:
	cmp	r5, #0
	beq	.L657
	ldr	r2, [fp, #-80]
	mov	r4, #0
	and	r8, r2, #15
	mov	r3, r2, lsr #4
	mov	r9, r2, lsr #1
	ldr	r2, [fp, #-88]
	mul	r9, r5, r9
	mul	r3, r2, r3
	add	r8, r3, r8, lsl #8
.L655:
	mov	r1, r4, lsr #8
	add	r0, r4, r9
	add	r0, r6, r0
	add	r4, r4, #256
	add	r1, r8, r1, lsl #12
	ldr	r3, [r10, #52]
	add	r1, r7, r1
	mov	r2, #256
	blx	r3
	cmp	r5, r4
	bhi	.L655
.L657:
	ldr	r3, [fp, #-80]
	ldr	r2, [fp, #-84]
	add	r3, r3, #2
	str	r3, [fp, #-80]
	cmp	r2, r3
	bhi	.L656
.L654:
	ldr	r4, [fp, #-84]
	mov	r0, r6
	ldr	ip, [r10, #44]
	ldr	r2, [fp, #-92]
	mul	r1, r4, r5
	mov	r1, r1, lsr #1
	blx	ip
	movs	r3, r4, lsr #1
	str	r3, [fp, #-84]
	beq	.L658
	ldr	r3, [fp, #-88]
	mov	r7, #0
	ldr	r8, [fp, #-112]
	str	r7, [fp, #-80]
	mov	r3, r3, asr #1
	str	r3, [fp, #-88]
.L659:
	cmp	r5, #0
	beq	.L661
	ldr	r2, [fp, #-80]
	mov	r4, #0
	and	r9, r2, #7
	mov	r3, r2, lsr #3
	ldr	r2, [fp, #-88]
	mul	r3, r2, r3
	add	r9, r3, r9, lsl #8
.L660:
	mov	r1, r4, lsr #8
	add	r0, r7, r4
	add	r0, r6, r0
	add	r4, r4, #256
	add	r1, r9, r1, lsl #11
	ldr	r3, [r10, #52]
	add	r1, r8, r1
	mov	r2, #256
	blx	r3
	cmp	r5, r4
	bhi	.L660
.L661:
	ldr	r3, [fp, #-80]
	add	r7, r7, r5
	ldr	r2, [fp, #-84]
	add	r3, r3, #1
	str	r3, [fp, #-80]
	cmp	r3, r2
	bne	.L659
.L658:
	ldr	r3, [fp, #4]
	cmp	r3, #1
	beq	.L722
	ldr	r3, [fp, #4]
	cmp	r3, #0
	bne	.L664
	ldr	r2, [fp, #-100]
	ldr	r3, [fp, #-116]
	cmp	r2, r3
	bls	.L669
.L670:
	add	r3, r3, #2
	cmp	r2, r3
	bhi	.L670
.L669:
	ldr	ip, [r10, #48]
	mov	r2, #1048576
	mov	r1, #128
	ldr	r0, [fp, #-108]
	blx	ip
	ldr	ip, [r10, #48]
	mov	r2, #1048576
	mov	r1, #128
	ldr	r0, [fp, #-96]
	blx	ip
	b	.L664
.L722:
	ldr	r3, [fp, #-100]
	cmp	r3, #0
	beq	.L664
	ldr	r2, [fp, #-116]
	mov	r4, #0
	ldr	r0, [fp, #-104]
	mov	lr, r3
	ldr	ip, [fp, #-96]
	mla	r6, r5, r2, r6
	mov	r5, r5, asl #1
	add	r6, r6, #1
.L665:
	cmp	r0, #0
	beq	.L668
	mov	r3, r4, lsr #1
	mov	r2, r6
	mul	r3, r0, r3
	sub	r1, r3, #-67108863
	add	r8, r3, r0
	sub	r1, r1, #65011712
	add	r8, ip, r8
	add	r1, ip, r1
	add	r3, ip, r3
.L666:
	ldrb	r7, [r2, #-1]	@ zero_extendqisi2
	strb	r7, [r1, #1]!
	ldrb	r7, [r2], #2	@ zero_extendqisi2
	strb	r7, [r3], #1
	cmp	r3, r8
	bne	.L666
.L668:
	add	r4, r4, #2
	add	r6, r6, r5
	cmp	lr, r4
	bhi	.L665
	b	.L664
.L721:
	ldr	r3, [fp, #-100]
	cmp	r3, #0
	beq	.L664
	ldr	r2, [fp, #-96]
	add	r6, r6, #1
	ldr	lr, [fp, #-104]
	mov	r7, r3
	mov	r4, #0
	mov	r3, r2
.L648:
	cmp	lr, #0
	moveq	ip, r3
	beq	.L651
	sub	r1, r3, #-67108863
	add	ip, r3, lr
	sub	r1, r1, #65011712
	mov	r2, r6
.L649:
	ldrb	r0, [r2, #-1]	@ zero_extendqisi2
	strb	r0, [r1, #1]!
	ldrb	r0, [r2], #2	@ zero_extendqisi2
	strb	r0, [r3], #1
	cmp	ip, r3
	bne	.L649
.L651:
	add	r4, r4, #1
	mov	r3, ip
	cmp	r4, r7
	add	r6, r6, r5
	bne	.L648
	b	.L664
.L720:
	ldr	r10, .L723+4
	mov	r0, r6
	ldr	r1, [fp, #-84]
	ldr	r2, [fp, #-92]
	ldr	ip, [r10, #44]
	blx	ip
	b	.L672
.L724:
	.align	2
.L723:
	.word	.LC62
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC63
	.word	.LC64
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_WriteBigTitle1DYuv, .-VDMHAL_V300R001_WriteBigTitle1DYuv
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.33698, %object
	.size	__func__.33698, 24
__func__.33698:
	.ascii	"VDMHAL_V300R001_OpenHAL\000"
	.type	__func__.33733, %object
	.size	__func__.33733, 27
__func__.33733:
	.ascii	"VDMHAL_V300R001_CalcFsSize\000"
	.space	1
	.type	__func__.33745, %object
	.size	__func__.33745, 35
__func__.33745:
	.ascii	"VDMHAL_V300R001_FillMemArrangeInfo\000"
	.space	1
	.type	__func__.33797, %object
	.size	__func__.33797, 27
__func__.33797:
	.ascii	"VDMHAL_V300R001_ArrangeMem\000"
	.space	1
	.type	__func__.33837, %object
	.size	__func__.33837, 31
__func__.33837:
	.ascii	"VDMHAL_V300R001_ArrangeMem_BTL\000"
	.space	1
	.type	__func__.33858, %object
	.size	__func__.33858, 25
__func__.33858:
	.ascii	"VDMHAL_V300R001_ResetVdm\000"
	.space	3
	.type	__func__.33885, %object
	.size	__func__.33885, 30
__func__.33885:
	.ascii	"VDMHAL_V300R001_ClearIntState\000"
	.space	2
	.type	__func__.33895, %object
	.size	__func__.33895, 24
__func__.33895:
	.ascii	"VDMHAL_V300R001_MaskInt\000"
	.type	__func__.33907, %object
	.size	__func__.33907, 26
__func__.33907:
	.ascii	"VDMHAL_V300R001_EnableInt\000"
	.space	2
	.type	__func__.33916, %object
	.size	__func__.33916, 25
__func__.33916:
	.ascii	"VDMHAL_V300R001_CheckReg\000"
	.space	3
	.type	__func__.33933, %object
	.size	__func__.33933, 27
__func__.33933:
	.ascii	"VDMHAL_V300R001_PrepareDec\000"
	.space	1
	.type	__func__.33954, %object
	.size	__func__.33954, 27
__func__.33954:
	.ascii	"VDMHAL_V300R001_IsVdmReady\000"
	.space	1
	.type	__func__.33959, %object
	.size	__func__.33959, 25
__func__.33959:
	.ascii	"VDMHAL_V300R001_IsVdmRun\000"
	.space	3
	.type	__func__.33973, %object
	.size	__func__.33973, 27
__func__.33973:
	.ascii	"VDMHAL_V300R001_BackupInfo\000"
	.space	1
	.type	__func__.34018, %object
	.size	__func__.34018, 25
__func__.34018:
	.ascii	"VDMHAL_V300R001_CfgRpMsg\000"
	.space	3
	.type	__func__.34039, %object
	.size	__func__.34039, 25
__func__.34039:
	.ascii	"VDMHAL_V300R001_CfgRpReg\000"
	.space	3
	.type	__func__.34052, %object
	.size	__func__.34052, 30
__func__.34052:
	.ascii	"VDMHAL_V300R001_MakeDecReport\000"
	.space	2
	.type	__func__.34077, %object
	.size	__func__.34077, 30
__func__.34077:
	.ascii	"VDMHAL_V300R001_PrepareRepair\000"
	.space	2
	.type	__func__.34085, %object
	.size	__func__.34085, 30
__func__.34085:
	.ascii	"VDMHAL_V300R001_StartHwRepair\000"
	.space	2
	.type	__func__.34133, %object
	.size	__func__.34133, 30
__func__.34133:
	.ascii	"VDMHAL_V300R001_StartHwDecode\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"pOpenParam = NULL error!\000" )
	.space	3
.LC1:
	ASCII(.ascii	"%s: %s\012\000" )
.LC2:
	ASCII(.ascii	"MemBaseAddr = 0 error!\000" )
	.space	1
.LC3:
	ASCII(.ascii	"VDMHAL_V300R001_OpenHAL: Size error!\000" )
	.space	3
.LC4:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V300R001_OpenHAL\012\000" )
	.space	3
.LC5:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC6:
	ASCII(.ascii	"g_VdmRegVirAddr, g_VdmResetVirAddr = %p\012\000" )
	.space	3
.LC7:
	ASCII(.ascii	"BPDRegVirAddr %p\012\000" )
	.space	2
.LC8:
	ASCII(.ascii	"!!!!!! HAL memory not enouph! need %d, have %d\012\000" )
.LC9:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V300R001_CloseHAL\012\000" )
	.space	2
.LC10:
	ASCII(.ascii	"VDMHAL_NULL_FUN_PRINT,L%d\012\000" )
	.space	1
.LC11:
	ASCII(.ascii	"image size out of range\000" )
.LC12:
	ASCII(.ascii	"DFS, CurFsSize < 0, CurFsSize: %d\012\000" )
	.space	1
.LC13:
	ASCII(.ascii	"DFS, report event. Size: %#x, Num: %#x, RefChanged:" )
	ASCII(.ascii	" %#x\012\000" )
	.space	3
.LC14:
	ASCII(.ascii	"pVdmMemArrange is NULL\000" )
	.space	1
.LC15:
	ASCII(.ascii	"DFS, no ref frame!\012\000" )
.LC16:
	ASCII(.ascii	"DFS, Frame number = %d > 30, Then, Frame num = 30, " )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC17:
	ASCII(.ascii	"VDMHAL_V300R001_ArrangeMem Mem addr is NULL\000" )
.LC18:
	ASCII(.ascii	"'pVdmMemArrange' is NULL\000" )
	.space	3
.LC19:
	ASCII(.ascii	"MemSize not enough for pmv slot\000" )
.LC20:
	ASCII(.ascii	"VDMHAL_V200R003_ArrangeMem get ChanWidth/ChanHeight" )
	ASCII(.ascii	" failed!\012\000" )
	.space	3
.LC21:
	ASCII(.ascii	"ImgSlotLen > ChanSlotLen\000" )
	.space	3
.LC22:
	ASCII(.ascii	"cann't allocate img slot\000" )
	.space	3
.LC23:
	ASCII(.ascii	"Mem addr is 0\000" )
	.space	2
.LC24:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V300R001_ResetVdm\012\000" )
	.space	2
.LC25:
	ASCII(.ascii	"VDMHAL_V300R001_ResetVdm: map vdm register fail, vi" )
	ASCII(.ascii	"r(reg) = (%p)\012\000" )
	.space	2
.LC26:
	ASCII(.ascii	"VDH: %d VDMHAL_V300R001_ResetVdm ERROR!!!\012\000" )
	.space	1
.LC27:
	ASCII(.ascii	"Ignore the reset request when the VDH clock is clos" )
	ASCII(.ascii	"ed.\012\000" )
.LC28:
	ASCII(.ascii	"GLB Reset failed!\012\000" )
	.space	1
.LC29:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200R003_ClearIntState\012\000" )
	.space	1
.LC30:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
.LC31:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200_MaskInt\012\000" )
	.space	3
.LC32:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200R003_EnableInt\012\000" )
	.space	1
.LC33:
	ASCII(.ascii	"%s: VdhId(%d) Invalid!\012\000" )
.LC34:
	ASCII(.ascii	"%s: vdm register virtual address not mapped, reset " )
	ASCII(.ascii	"failed!\012\000" )
.LC35:
	ASCII(.ascii	"%s: unkown reg_id = %d\012\000" )
.LC36:
	ASCII(.ascii	"VDM register not mapped yet!\000" )
	.space	3
.LC37:
	ASCII(.ascii	"%s: RD_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\012\000" )
	.space	3
.LC38:
	ASCII(.ascii	"VDM register not mapped yet!\012\000" )
	.space	2
.LC39:
	ASCII(.ascii	"pBackUpInfo = NULL!\000" )
.LC40:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V300R001_BackupInfo\012\000" )
.LC41:
	ASCII(.ascii	"can NOT map vir addr for up-msg\000" )
.LC42:
	ASCII(.ascii	"ReadUpMsgSlot error! pDst=%p, pSrc=%p\012\000" )
	.space	1
.LC43:
	ASCII(.ascii	"ReadUpMsgSlot error! upmsg_size(%d) > 512\012\000" )
	.space	1
.LC44:
	ASCII(.ascii	"WriteMsgSlot error!\012\000" )
	.space	3
.LC45:
	ASCII(.ascii	"can not map repair msg virtual address!\000" )
.LC46:
	ASCII(.ascii	"ValidGroupNum=%d out of range!\012\000" )
.LC47:
	ASCII(.ascii	"align_mb error\012\000" )
.LC48:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V300_CfgRpReg\012\000" )
	.space	2
.LC49:
	ASCII(.ascii	"'pMakeDecReport' is NULL\000" )
	.space	3
.LC50:
	ASCII(.ascii	"'pDecReport' is NULL\000" )
	.space	3
.LC51:
	ASCII(.ascii	"pDecReport->DecSliceNum(%d) > %d, set to 0 for full" )
	ASCII(.ascii	" repair.\012\000" )
	.space	3
.LC52:
	ASCII(.ascii	"\012***** UpMsg DecSliceNum=%d\012\000" )
	.space	3
.LC53:
	ASCII(.ascii	"\012***** Up Msg (phy addr: %#8x) *****\012\000" )
	.space	2
.LC54:
	ASCII(.ascii	"\0120x%02x 0x%08x 0x%08x 0x%08x 0x%08x\012\000" )
	.space	3
.LC55:
	ASCII(.ascii	"\012***** Up Msg print finished *****\012\000" )
.LC56:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200R003_PrepareRepair\012\000" )
	.space	1
.LC57:
	ASCII(.ascii	"vdm register virtual address not mapped, VDMHAL_V20" )
	ASCII(.ascii	"0R003_PrepareRepair failed!\012\000" )
.LC58:
	ASCII(.ascii	"FIRST_REPAIR Parameter Error!\012\000" )
	.space	1
.LC59:
	ASCII(.ascii	"SECOND_REPAIR Parameter Error!\012\000" )
.LC60:
	ASCII(.ascii	"%s: WR_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\012\000" )
	.space	3
.LC61:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V300R001_StartHwDecode\012\000" )
	.space	1
.LC62:
	ASCII(.ascii	"BigTile1d_y\000" )
.LC63:
	ASCII(.ascii	"failed mem_allocMemBlock BigTile_yuv save!\012\000" )
.LC64:
	ASCII(.ascii	"BigTile1d_uv\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
