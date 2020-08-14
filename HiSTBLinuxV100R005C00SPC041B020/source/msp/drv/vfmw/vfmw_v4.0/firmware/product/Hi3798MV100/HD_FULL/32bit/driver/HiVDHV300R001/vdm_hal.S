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
	UNWIND(.pad #68)
	sub	sp, sp, #68
	ldr	r7, [fp, #16]
	mov	r4, r0
	str	r1, [fp, #-68]
	cmn	r7, #2
	mov	r8, r2
	str	r3, [fp, #-64]
	ldr	r5, [fp, #4]
	ldr	r6, [fp, #20]
	beq	.L105
	cmp	r7, #0
	blt	.L106
	mov	r0, r7
	ldr	r10, .L171
	bl	VCTRL_GetVidStd
	mov	r9, r0
	ldr	r0, [r10, r7, asl #2]
	ldr	r2, [r0, #1388]
	cmp	r2, #1
	beq	.L149
.L37:
	cmp	r4, #0
	subne	r1, r8, #32
	movwne	r2, #8160
	beq	.L150
	cmp	r1, r2
	bhi	.L69
	ldr	r3, [fp, #-64]
	sub	r1, r3, #32
	cmp	r1, r2
	bhi	.L69
	cmp	r6, #0
	beq	.L151
	ldr	r3, .L171+4
	cmp	r5, #20
	mov	r2, #564
	mov	r1, #0
	mov	r0, r6
	movlt	r10, r5
	ldr	ip, [r3, #48]
	movge	r10, #20
	str	r3, [fp, #-72]
	blx	ip
	ldr	r3, [fp, #-64]
	add	r2, r8, #15
	ldr	r1, .L171+8
	add	r0, r3, #15
	mov	r2, r2, asr #4
	mov	r0, r0, asr #4
	ldrb	r5, [r1]	@ zero_extendqisi2
	cmp	r2, #45
	cmple	r0, #36
	mov	lr, r2, asl #4
	mov	r1, r0, asl #4
	movle	ip, #64
	movgt	ip, #32
	cmp	r5, #1
	moveq	ip, #64
	cmp	r9, #17
	beq	.L75
	mul	r5, r2, ip
	ldr	r2, [fp, #-68]
	mul	r5, r5, r0
	add	r5, r5, #127
	bic	r5, r5, #127
	mul	r3, r10, r5
	cmp	r3, r2
	bge	.L103
	add	ip, lr, #255
	mov	r2, r5, asr #1
	bic	ip, ip, #255
	str	r10, [r6, #552]
	str	r2, [r6, #548]
.L102:
	sub	r2, lr, #1
	mov	r0, ip, asl #4
	cmp	r2, #2048
	str	r0, [r6]
	movcc	r10, #16
	bcc	.L77
	sub	r2, lr, #2048
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r10, #32
	bcc	.L77
	sub	r2, lr, #4096
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r10, #48
	bcs	.L152
.L77:
	ldr	r2, [fp, #12]
	cmp	r2, #1
	beq	.L153
	cmp	r9, #17
	beq	.L154
	adds	r2, r1, #31
	addmi	r2, r1, #62
	cmp	r7, #0
	mov	r2, r2, asr #5
	mov	r0, r2
	mov	lr, r2, asl #7
	sub	lr, lr, r0, asl #5
	mul	r2, ip, r2
	mul	lr, r10, lr
	mov	r8, r2, asl #6
	sub	r8, r8, r2, asl #4
	add	r8, r8, lr, lsr #1
	blt	.L83
.L79:
	mov	r0, r7
	str	r1, [fp, #-88]
	str	r3, [fp, #-84]
	str	ip, [fp, #-80]
	bl	VCTRL_GetChanWidth
	str	r0, [fp, #-76]
	mov	r0, r7
	bl	VCTRL_GetChanHeight
	ldr	r2, [fp, #-76]
	sub	r1, fp, #88
	ldmia	r1, {r1, r3, ip}
	cmn	r0, #1
	cmnne	r2, #1
	beq	.L155
	add	lr, r2, #15
	add	r0, r0, #15
	bic	lr, lr, #15
	bic	r2, r0, #15
	sub	r7, lr, #1
	add	r0, lr, #255
	cmp	r7, #2048
	bic	r0, r0, #255
	movcc	lr, #16
	bcc	.L85
	sub	r7, lr, #2048
	sub	r7, r7, #1
	cmp	r7, #2048
	movcc	lr, #32
	bcs	.L156
.L85:
	ldr	r7, [fp, #12]
	cmp	r7, #1
	beq	.L157
	adds	r7, r2, #31
	addmi	r7, r2, #62
	mov	r2, r7, asr #5
	mov	r7, r2, asl #7
	mul	r0, r0, r2
	sub	r2, r7, r2, asl #5
	mul	lr, lr, r2
	mov	r2, r0, asl #6
	sub	r0, r2, r0, asl #4
	add	lr, r0, lr, lsr #1
.L87:
	cmp	lr, r8
	blt	.L158
	cmp	r9, #17
	beq	.L88
.L161:
	adds	r0, r1, #31
	addmi	r0, r1, #62
	mov	r0, r0, asr #5
	mul	r2, r0, ip
.L83:
	mov	r10, r10, asl #5
	mov	r2, r2, asl #5
	mla	r0, r0, r10, r2
	str	r0, [r6, #280]
.L89:
	ldr	r2, [fp, #8]
	cmp	r2, #0
	moveq	r3, r2
	streq	r3, [r6, #292]
	bne	.L159
.L91:
	ldr	r3, [fp, #-72]
	mov	r0, #0
	ldr	r2, .L171+12
	ldr	r1, .L171+16
	ldr	ip, [r3, #68]
	ldr	r3, .L171+20
	blx	ip
	mvn	r3, #0
.L134:
	mov	r0, r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L106:
	mov	r9, #20
	b	.L37
.L159:
	ldr	r2, [fp, #8]
	add	r7, r4, #1020
	add	r7, r7, #3
	mov	r1, r8
	cmp	r2, #32
	ldr	r2, [fp, #-68]
	bic	r7, r7, #1020
	rsb	r0, r3, r2
	bic	r7, r7, #3
	ldr	r3, [fp, #8]
	rsb	r9, r4, r7
	rsb	r0, r9, r0
	movge	r10, #32
	movlt	r10, r3
	bl	__aeabi_idiv
	cmp	r10, r0
	movlt	r1, r10
	movge	r1, r0
	cmp	r1, #0
	str	r1, [r6, #292]
	beq	.L91
	addgt	r2, r6, #4
	movgt	r3, #0
	ble	.L160
.L94:
	add	r3, r3, #1
	str	r7, [r2, #4]!
	cmp	r3, r1
	add	r7, r7, r8
	bne	.L94
	mla	r8, r8, r1, r9
	add	r0, r6, #132
	mov	r2, #0
	add	r3, r8, r4
.L96:
	add	r2, r2, #1
	str	r3, [r0, #4]!
	cmp	r2, r1
	add	r3, r3, #32
	bne	.L96
.L101:
	ldr	r0, [r6, #552]
	add	r1, r8, r1, lsl #5
	add	r4, r1, r4
	cmp	r0, #0
	addgt	r2, r6, #424
	movgt	r3, #0
	ble	.L99
.L98:
	add	r3, r3, #1
	str	r4, [r2, #4]!
	cmp	r3, r0
	add	r4, r4, r5
	bne	.L98
.L99:
	mla	r1, r0, r5, r1
	mov	r3, #0
	mov	r0, r3
	str	r1, [r6, #4]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L105:
	mov	r9, #17
	b	.L37
.L75:
	mul	r5, lr, r0
	ldr	r2, [fp, #-68]
	add	r5, r5, #127
	bic	r5, r5, #127
	mul	r3, r10, r5
	cmp	r3, r2
	bge	.L103
	add	ip, r8, #255
	mov	r2, r5, asr #1
	str	r10, [r6, #552]
	bic	ip, ip, #255
	str	r2, [r6, #548]
	b	.L102
.L153:
	mul	r8, r1, ip
	cmp	r7, #0
	add	r8, r8, r8, lsl #1
	bge	.L79
	cmp	r9, #17
	bne	.L161
.L88:
	ldr	r2, [fp, #-64]
	mul	r2, r2, ip
.L82:
	str	r2, [r6, #280]
	b	.L89
.L156:
	sub	r7, lr, #4096
	sub	r7, r7, #1
	cmp	r7, #2048
	movcc	lr, #48
	bcc	.L85
	sub	lr, lr, #6144
	sub	lr, lr, #1
	cmp	lr, #2048
	movcs	lr, #16
	movcc	lr, #64
	b	.L85
.L152:
	sub	lr, lr, #6144
	sub	lr, lr, #1
	cmp	lr, #2048
	movcs	r10, #16
	movcc	r10, #64
	b	.L77
.L154:
	ldr	r2, [fp, #-64]
	cmp	r7, #0
	mul	r2, r2, ip
	add	r8, r2, r2, lsl #1
	mov	r8, r8, asr #1
	bge	.L79
	b	.L82
.L157:
	mul	lr, r2, r0
	add	lr, lr, lr, lsl #1
	b	.L87
.L149:
	sub	ip, r8, #32
	movw	r1, #8160
	add	r2, r0, #225280
	cmp	ip, r1
	ldr	r5, [r2, #2856]
	bhi	.L38
	ldr	r3, [fp, #-64]
	sub	r2, r3, #32
	cmp	r2, r1
	bls	.L39
.L38:
	ldr	r3, .L171+4
	mov	r0, #0
	ldr	r2, .L171+24
	mvn	r4, #0
	mov	r1, r3
	str	r3, [fp, #-72]
	ldr	ip, [r1, #68]
	ldr	r3, .L171+28
	ldr	r1, .L171+16
	blx	ip
.L40:
	ldr	r3, [fp, #-72]
	mov	r2, r4
	ldr	r1, .L171+32
	mov	r0, #31
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r3, #0
	b	.L134
.L160:
	mla	r8, r8, r1, r9
	b	.L101
.L69:
	ldr	r1, .L171+4
	mov	r0, #0
	ldr	r3, .L171+28
.L141:
	ldr	ip, [r1, #68]
	ldr	r2, .L171+12
	ldr	r1, .L171+16
	blx	ip
	mvn	r3, #0
	b	.L134
.L39:
	ldr	r3, [fp, #-64]
	add	r1, r8, #15
	ldr	ip, .L171+8
	add	r2, r3, #15
	mov	r1, r1, asr #4
	mov	r2, r2, asr #4
	ldrb	r4, [ip]	@ zero_extendqisi2
	cmp	r1, #45
	cmple	r2, #36
	mov	lr, r1, asl #4
	mov	r3, r2, asl #4
	str	r3, [fp, #-80]
	movle	ip, #64
	movgt	ip, #32
	cmp	r4, #1
	moveq	ip, #64
	cmp	r9, #17
	beq	.L162
	mul	r1, ip, r1
	add	ip, lr, #255
	bic	r3, ip, #255
	str	r3, [fp, #-68]
	mul	r2, r1, r2
	add	r2, r2, #127
	bic	r3, r2, #127
	str	r3, [fp, #-76]
.L100:
	sub	r2, lr, #1
	cmp	r2, #2048
	movcc	r4, #16
	bcc	.L45
	sub	r2, lr, #2048
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r4, #32
	bcc	.L45
	sub	r2, lr, #4096
	sub	r2, r2, #1
	cmp	r2, #2048
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
	beq	.L163
	cmp	r9, #17
	beq	.L164
	ldr	r1, [fp, #-80]
	add	r0, r0, #228352
	ldr	r9, [fp, #-68]
	add	r0, r0, #456
	adds	r2, r1, #31
	add	r3, r9, r4
	addmi	r4, r1, #62
	movpl	r4, r2
	ldr	r2, .L171+4
	mov	r4, r4, asr #5
	mul	r4, r4, r3
	mov	r1, r2
	ldr	r1, [r1, #48]
	str	r2, [fp, #-72]
	mov	r2, #572
	mov	lr, r1
	mov	r1, #0
	mov	r3, lr
	mov	ip, r4, asl #6
	sub	ip, ip, r4, asl #4
	str	ip, [fp, #-68]
	blx	r3
	ldr	r1, [r10, r7, asl #2]
	ldr	r3, [fp, #-76]
	mov	r0, r9, asl #4
	ldr	ip, [fp, #-68]
	add	r2, r1, #225280
	mov	r3, r3, asr #1
	str	r0, [r2, #3528]
	str	r3, [r2, #4076]
.L51:
	mov	r4, r4, asl #5
.L140:
	str	r4, [r2, #3808]
	add	r1, r1, #229376
	ldr	r3, [fp, #-76]
	str	ip, [r2, #4092]
	add	r4, r3, ip
	adds	r4, r4, #1056
	str	r3, [r1]
	bmi	.L40
	ldr	r9, [r10, r7, asl #2]
	cmp	r5, r4
	add	r5, r9, #225280
	ldr	ip, [r5, #2860]
	beq	.L165
.L55:
	ldr	r3, .L171+36
	str	r4, [r5, #2856]
	ldr	r5, [r3]
	cmp	r5, #0
	beq	.L57
	ldr	r0, [fp, #-64]
	mov	r3, #16
	str	ip, [fp, #-60]
	sub	r2, fp, #60
	str	r4, [fp, #-56]
	mov	r1, #118
	str	r0, [fp, #-48]
	mov	r0, r7
	str	r8, [fp, #-52]
	str	ip, [fp, #-68]
	blx	r5
	ldr	ip, [fp, #-68]
.L57:
	ldr	r1, [r10, r7, asl #2]
	mov	r3, ip
	ldr	r5, [fp, #-72]
	mov	r2, r4
	add	ip, r1, #225280
	mov	r0, #31
	ldr	r1, .L171+40
	ldr	ip, [ip, #2880]
	ldr	r4, [r5, #68]
	str	ip, [sp]
	blx	r4
	ldr	r3, [r5]
	ldr	r4, [r10, r7, asl #2]
	blx	r3
	ldr	r2, [r10, r7, asl #2]
	mov	r1, #1
	add	r4, r4, #229376
	mov	r3, #2
	add	r2, r2, #225280
	str	r0, [r4, #4]
	str	r1, [r2, #2844]
	b	.L134
.L165:
	ldr	r2, [r5, #2880]
	cmp	r2, #1
	beq	.L55
	cmp	r6, #0
	beq	.L166
	ldr	r3, [r5, #4092]
	add	r2, r9, #229376
	cmp	ip, #0
	ldr	r4, [r2]
	str	r3, [fp, #-64]
	beq	.L167
.L59:
	ldr	r0, [r10, r7, asl #2]
	ldr	r10, .L171
	add	r8, r0, #225280
	ldr	lr, [r8, #2864]
	cmp	lr, #30
	bgt	.L168
.L60:
	cmp	lr, #0
	str	lr, [r5, #3820]
	addle	r10, r9, #228352
	ble	.L61
	ldr	r1, [r8, #2896]
	cmp	r1, #0
	beq	.L62
	add	r2, r1, #1020
	ldr	r3, [fp, #-64]
	add	r2, r2, #3
	bic	r2, r2, #1020
	bic	r2, r2, #3
	str	r2, [r5, #3536]
	rsb	r2, r1, r2
	add	r2, r3, r2
	add	ip, r2, #32
	add	r2, r2, r1
	add	r7, ip, r1
	str	r2, [r5, #3664]
	str	r7, [r5, #3956]
	add	ip, r4, ip
	ldr	r2, [r8, #2856]
	cmp	ip, r2
	bgt	.L112
	add	r10, r9, #228352
	add	r0, r0, #227328
	str	r6, [fp, #20]
	add	r0, r0, #868
	add	r7, r10, #464
	add	r9, r10, #592
	mov	r3, #0
	ldr	r6, [fp, #-64]
	b	.L64
.L66:
	ldr	r1, [r0]
	add	r0, r0, #20
	add	r2, r1, #1020
	cmp	r1, #0
	add	r2, r2, #3
	bic	r2, r2, #1020
	beq	.L169
	bic	r2, r2, #3
	str	r2, [r7, #4]!
	rsb	r2, r1, r2
	add	r2, r6, r2
	add	ip, r2, #32
	add	r2, r2, r1
	str	r2, [r9, #4]!
	add	r2, ip, r1
	str	r2, [r7, #420]
	add	ip, r4, ip
	ldr	r2, [r8, #2856]
	cmp	ip, r2
	bgt	.L63
.L64:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L66
	ldr	r6, [fp, #20]
.L61:
	ldr	ip, [r8, #2856]
	add	r1, r10, #456
	str	lr, [r5, #4080]
	mov	r0, r6
	ldr	r3, [fp, #-72]
	mov	r2, #564
	mul	lr, lr, ip
	ldr	r3, [r3, #52]
	str	lr, [r5, #3532]
	blx	r3
	mov	r3, #0
	b	.L134
.L150:
	ldr	r1, .L171+4
	mov	r0, r4
	ldr	r3, .L171+44
	b	.L141
.L103:
	ldr	r3, [fp, #-72]
	mov	r0, #0
	ldr	r2, .L171+12
	ldr	r1, .L171+16
	ldr	ip, [r3, #68]
	ldr	r3, .L171+48
	blx	ip
	mvn	r3, #0
	b	.L134
.L169:
	ldr	r2, [r8, #2856]
.L63:
	str	r2, [sp, #4]
	mov	r0, #1
	ldr	r2, [fp, #-72]
	str	r1, [sp, #8]
	str	ip, [sp]
	ldr	r1, .L171+52
	ldr	ip, [r2, #68]
	ldr	r2, .L171+56
	blx	ip
	mvn	r3, #0
	b	.L134
.L158:
	ldr	r3, [fp, #-72]
	mov	r0, #0
	ldr	r2, .L171+12
	ldr	r1, .L171+16
	ldr	ip, [r3, #68]
	ldr	r3, .L171+60
	blx	ip
	mvn	r3, #0
	b	.L134
.L155:
	ldr	r3, [fp, #-72]
	mov	r0, #0
	ldr	r1, .L171+64
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r3, #0
	b	.L134
.L151:
	ldr	r1, .L171+4
	mov	r0, r6
	ldr	r3, .L171+68
	b	.L141
.L162:
	mul	r2, lr, r2
	add	r1, r8, #255
	bic	r3, r1, #255
	str	r3, [fp, #-68]
	add	r2, r2, #127
	bic	r3, r2, #127
	str	r3, [fp, #-76]
	b	.L100
.L163:
	ldr	r3, [fp, #-68]
	add	r0, r0, #228352
	ldr	r2, [fp, #-80]
	mov	r1, #0
	add	r0, r0, #456
	mul	ip, r3, r2
	ldr	r2, .L171+4
	mov	lr, r2
	str	r2, [fp, #-72]
	ldr	lr, [lr, #48]
	mov	r2, #572
	add	ip, ip, ip, lsl #1
	str	ip, [fp, #-84]
	mov	ip, lr
	blx	ip
	ldr	r2, [fp, #-76]
	ldr	r1, [r10, r7, asl #2]
	cmp	r9, #17
	ldr	r3, [fp, #-68]
	mov	r0, r2, asr #1
	add	r2, r1, #225280
	ldr	ip, [fp, #-84]
	mov	r3, r3, asl #4
	str	r0, [r2, #4076]
	str	r3, [r2, #3528]
	beq	.L170
	ldr	lr, [fp, #-80]
	ldr	r3, [fp, #-68]
	adds	r0, lr, #31
	addmi	r0, lr, #62
	add	r3, r3, r4
	mov	r0, r0, asr #5
	mul	r4, r0, r3
	b	.L51
.L168:
	ldr	r3, [fp, #-72]
	mov	r2, lr
	ldr	r1, .L171+72
	mov	r0, #31
	ldr	ip, [r3, #68]
	blx	ip
	ldr	r0, [r10, r7, asl #2]
	mov	r1, #30
	add	r2, r0, #225280
	mov	lr, r1
	mov	r8, r2
	str	r1, [r2, #2864]
	b	.L60
.L167:
	ldr	r3, [fp, #-72]
	mov	r0, #31
	ldr	r1, .L171+76
	ldr	r2, [r3, #68]
	blx	r2
	b	.L59
.L164:
	ldr	r9, [fp, #-68]
	add	r0, r0, #228352
	ldr	r2, [fp, #-64]
	mov	r1, #0
	ldr	r3, .L171+4
	add	r0, r0, #456
	mul	r4, r9, r2
	str	r3, [fp, #-72]
	mov	r2, #572
	ldr	r3, [r3, #48]
	add	ip, r4, r4, lsl #1
	mov	ip, ip, asr #1
	str	ip, [fp, #-68]
	blx	r3
	ldr	r1, [r10, r7, asl #2]
	ldr	r3, [fp, #-76]
	mov	r0, r9, asl #4
	add	r2, r1, #225280
	ldr	ip, [fp, #-68]
	mov	r3, r3, asr #1
	str	r0, [r2, #3528]
	str	r3, [r2, #4076]
	b	.L140
.L170:
	ldr	r3, [fp, #-68]
	ldr	r0, [fp, #-64]
	mul	r4, r3, r0
	b	.L140
.L166:
	ldr	r3, [fp, #-72]
	mov	r0, r6
	ldr	r2, .L171+80
	ldr	r1, .L171+16
	ldr	ip, [r3, #68]
	ldr	r3, .L171+84
	blx	ip
	mvn	r3, #0
	b	.L134
.L112:
	mov	r3, #0
	b	.L63
.L62:
	ldr	r2, [r8, #2856]
	mov	r3, r1
	mov	ip, r1
	b	.L63
.L172:
	.align	2
.L171:
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
	.word	.LC23
	.word	.LANCHOR0+116
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
	ldr	r6, [fp, #4]
	ldr	r7, [fp, #20]
	ldreq	r1, .L225
	ldreq	r3, .L225+4
	beq	.L217
	ldr	r3, [fp, #16]
	sub	r2, r2, #32
	cmp	r3, #1
	movw	r3, #8160
	beq	.L220
	cmp	r2, r3
	bhi	.L179
	sub	r2, r5, #32
	cmp	r2, r3
	bhi	.L179
.L178:
	cmp	r7, #0
	beq	.L221
	ldr	r10, .L225
	cmp	r6, #20
	mov	r2, #564
	mov	r1, #0
	mov	r0, r7
	movge	r6, #20
	ldr	r3, [r10, #48]
	blx	r3
	add	r3, r5, #15
	ldr	r1, .L225+8
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
	mul	r9, r6, r5
	cmp	r9, r3
	bge	.L222
	add	r3, r4, #1020
	sub	r0, r1, #1
	add	r10, r1, #255
	add	r3, r3, #3
	cmp	r0, #2048
	bic	r3, r3, #1020
	bic	r10, r10, #255
	bic	r3, r3, #3
	rsb	r0, r4, r3
	str	r6, [r7, #552]
	str	r0, [fp, #-52]
	movcc	lr, #16
	mov	r0, r10, asl #4
	str	r0, [r7]
	mov	r0, r5, asr #1
	str	r0, [r7, #548]
	bcc	.L184
	sub	r0, r1, #2048
	sub	r0, r0, #1
	cmp	r0, #2048
	movcc	lr, #32
	bcc	.L184
	sub	r0, r1, #4096
	sub	r0, r0, #1
	cmp	r0, #2048
	movcc	lr, #48
	bcs	.L223
.L184:
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
	str	r1, [r7, #280]
	beq	.L185
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
.L185:
	ldr	r1, [fp, #24]
	cmp	r1, #0
	bne	.L224
	ldr	r1, [fp, #12]
	cmp	r1, #0
	beq	.L201
	ldr	r1, [fp, #12]
	ldr	ip, [fp, #12]
	cmp	r1, #6
	ldr	r1, [fp, #24]
	movge	r8, ip
	movlt	r8, #6
	str	r8, [r7, #424]
.L190:
	add	r1, r2, r1
	cmp	r8, #0
	mulle	r2, r8, r1
	ble	.L188
	add	ip, r7, #292
	mov	r2, #0
.L191:
	add	r2, r2, #1
	str	r3, [ip, #4]!
	cmp	r2, r8
	add	r3, r3, r1
	bne	.L191
	mul	r2, r1, r2
.L188:
	ldr	r3, [fp, #8]
	cmp	r3, #0
	ldr	r3, [fp, #-52]
	add	r8, r3, r2
	bne	.L192
	ldr	r3, [fp, #8]
	str	r3, [r7, #292]
.L193:
	cmp	r6, #0
	add	r3, r8, r3
	add	r4, r3, r4
	addgt	ip, r7, #424
	movgt	r0, #0
	ble	.L200
.L199:
	add	r0, r0, #1
	str	r4, [ip, #4]!
	cmp	r0, r6
	add	r4, r4, r5
	bne	.L199
.L200:
	mov	r0, #0
	add	r3, r9, r3
	str	r3, [r7, #4]
.L215:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L224:
	adds	r1, r8, #127
	addmi	r1, r8, #254
	ldr	r8, [fp, #12]
	mov	r1, r1, asr #7
	cmp	r8, #0
	mov	r8, r1, asl #9
	sub	r1, r8, r1, asl #7
	mul	r1, ip, r1
	mov	r1, r1, asr #1
	beq	.L201
	ldr	ip, [fp, #12]
	str	ip, [r7, #424]
	mov	r8, ip
	b	.L190
.L192:
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
	str	r0, [r7, #292]
	mulle	r3, r0, r10
	ble	.L193
	add	r2, r8, r4
	add	r1, r7, #4
	mov	r3, #0
.L197:
	add	r3, r3, #1
	str	r2, [r1, #4]!
	cmp	r3, r0
	add	r2, r2, r10
	bne	.L197
	mul	r3, r3, r10
	b	.L193
.L201:
	mov	r1, #0
	str	r1, [r7, #424]
	mov	r2, r1
	b	.L188
.L220:
	cmp	r2, r3
	bhi	.L179
	sub	r2, r5, #32
	movw	r3, #16352
	cmp	r2, r3
	bls	.L178
.L179:
	ldr	r1, .L225
	mov	r0, #0
	ldr	r3, .L225+12
.L217:
	ldr	ip, [r1, #68]
	ldr	r2, .L225+16
	ldr	r1, .L225+20
	blx	ip
	mvn	r0, #0
	b	.L215
.L223:
	sub	r1, r1, #6144
	sub	r1, r1, #1
	cmp	r1, #2048
	movcs	lr, #16
	movcc	lr, #64
	b	.L184
.L222:
	ldr	ip, [r10, #68]
	mov	r0, #0
	ldr	r3, .L225+24
	ldr	r2, .L225+16
	ldr	r1, .L225+20
	blx	ip
	mvn	r0, #0
	b	.L215
.L221:
	ldr	r1, .L225
	mov	r0, r7
	ldr	r3, .L225+28
	b	.L217
.L226:
	.align	2
.L225:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC24
	.word	g_not_direct_8x8_inference_flag
	.word	.LC11
	.word	.LANCHOR0+152
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
	beq	.L229
	cmp	r4, #1
	bne	.L248
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	ldr	r2, .L251
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r1, .L251+4
	ldr	ip, [r2, #68]
	ldr	r2, .L251+8
	blx	ip
.L227:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L248:
	ldr	r3, .L251
	mov	r0, #0
	ldr	r1, .L251+12
	ldr	r3, [r3, #68]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	bx	r3
.L229:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r2, r0, #0
	beq	.L249
	ldr	r6, .L251+16
	ldr	r7, .L251+20
	ldr	r3, [r6]
	ldr	r1, [r7]
	ldr	r8, [r3, #36]
	ldr	r5, [r1, #120]
	tst	r5, #1
	beq	.L227
	orr	r5, r5, #64
	str	r5, [r1, #120]
	ldr	r3, [r1, #372]
	tst	r3, #4
	bne	.L234
	mov	r3, #1
	movw	r0, #10000
	b	.L236
.L250:
	add	r3, r3, #1
	cmp	r3, r0
	beq	.L237
.L236:
	ldr	r2, [r1, #372]
	tst	r2, #4
	beq	.L250
	movw	r2, #10000
	cmp	r3, r2
	beq	.L237
.L238:
	bfc	r5, #6, #1
	str	r5, [r1, #120]
.L239:
	ldr	r3, [r6]
	str	r8, [r3, #36]
	b	.L227
.L237:
	ldr	r3, .L251
	mov	r2, #0
	ldr	r1, .L251+24
	mov	r0, r2
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r1, [r7]
	b	.L238
.L249:
	ldr	r3, .L251
	ldr	r1, .L251+28
	ldr	r3, [r3, #68]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	bx	r3
.L234:
	bfi	r5, r4, #6, #1
	str	r5, [r1, #120]
	b	.L239
.L252:
	.align	2
.L251:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC5
	.word	.LANCHOR0+184
	.word	.LC25
	.word	g_HwMem
	.word	g_pstRegCrg
	.word	.LC27
	.word	.LC26
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
	beq	.L264
	ldr	r5, .L268
	ldr	r2, [r5]
	ldr	r4, [r2, #120]
	tst	r4, #1
	beq	.L265
	orr	r4, r4, #16
	str	r4, [r2, #120]
	ldr	r3, [r2, #372]
	tst	r3, #1
	bne	.L259
	movw	r0, #9999
	b	.L257
.L267:
	subs	r0, r0, #1
	beq	.L266
.L257:
	ldr	r3, [r2, #372]
	tst	r3, #1
	beq	.L267
.L259:
	bfc	r4, #4, #1
	str	r4, [r2, #120]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L265:
	ldr	r3, .L268+4
	mov	r0, #3
	ldr	r1, .L268+8
	ldr	r3, [r3, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L266:
	ldr	r3, .L268+4
	ldr	r1, .L268+12
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r2, [r5]
	b	.L259
.L264:
	ldr	r3, .L268+4
	ldr	r1, .L268+16
	ldr	r3, [r3, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L269:
	.align	2
.L268:
	.word	g_pstRegCrg
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC28
	.word	.LC29
	.word	.LC26
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
	ldr	r7, .L291
	mov	r5, r0, asl #3
	mov	r6, r0, asl #6
	rsb	r3, r5, r6
	ldr	r2, .L291+4
	add	r3, r7, r3
	ldr	r3, [r3, #8]
	ldr	r3, [r2, r3, asl #2]
	cmp	r3, #0
	ldrne	r4, [r3, #1232]
	moveq	r4, r3
	cmp	r0, #1
	bhi	.L287
	cmp	r0, #0
	bgt	.L288
	movw	r8, #1208
	ldr	r9, .L291+8
	mul	r8, r8, r0
	ldr	r3, [r9, r8]
	cmp	r3, #0
	beq	.L289
.L276:
	cmp	r4, #1
	beq	.L290
.L284:
	mvn	r2, #0
	str	r2, [r3, #32]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L288:
	ldr	r1, .L291+12
	mov	r2, #0
	mov	r3, r0
	str	r2, [sp]
	mov	r0, r2
	ldr	r2, .L291+16
	ldr	ip, [r1, #68]
	ldr	r1, .L291+20
	blx	ip
.L270:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L287:
	ldr	r3, .L291+12
	mov	r0, #0
	ldr	r1, .L291+24
.L285:
	ldr	r3, [r3, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L290:
	rsb	r5, r5, r6
	add	r7, r7, r5
	ldr	r2, [r7, #52]
	cmp	r2, #1
	beq	.L284
	cmp	r2, #2
	ldr	r1, [r3, #28]
	mvneq	r2, #11
	streq	r2, [r3, #32]
	beq	.L270
	cmp	r2, #3
	mvneq	r2, #14
	streq	r2, [r3, #32]
	b	.L270
.L289:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r9, r8]
	bne	.L276
.L277:
	ldr	r3, .L291+12
	ldr	r1, .L291+28
	b	.L285
.L292:
	.align	2
.L291:
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+212
	.word	.LC5
	.word	.LC30
	.word	.LC31
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
	bhi	.L302
	cmp	r0, #0
	bgt	.L303
	movw	r4, #1208
	ldr	r5, .L305
	mul	r4, r4, r0
	ldr	r3, [r5, r4]
	cmp	r3, #0
	beq	.L304
.L298:
	mvn	r2, #0
	str	r2, [r3, #36]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L303:
	ldr	r1, .L305+4
	mov	r2, #0
	mov	r3, r0
	str	r2, [sp]
	mov	r0, r2
	ldr	r2, .L305+8
	ldr	ip, [r1, #68]
	ldr	r1, .L305+12
	blx	ip
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L302:
	ldr	r3, .L305+4
	mov	r0, #0
	ldr	r1, .L305+16
.L301:
	ldr	r3, [r3, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L304:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r5, r4]
	bne	.L298
.L299:
	ldr	r3, .L305+4
	ldr	r1, .L305+20
	b	.L301
.L306:
	.align	2
.L305:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+244
	.word	.LC5
	.word	.LC32
	.word	.LC31
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
	ldr	r2, .L325
	mov	r3, r0, asl #6
	ldr	r1, .L325+4
	sub	r3, r3, r0, asl #3
	add	r3, r2, r3
	ldr	r3, [r3, #8]
	ldr	r3, [r1, r3, asl #2]
	cmp	r3, #0
	ldrne	r4, [r3, #1232]
	moveq	r4, r3
	cmp	r0, #1
	bhi	.L322
	cmp	r0, #0
	bgt	.L323
	movw	r5, #1208
	ldr	r6, .L325+8
	mul	r5, r5, r0
	ldr	r3, [r6, r5]
	cmp	r3, #0
	beq	.L324
.L313:
	cmp	r4, #1
	mvnne	r2, #1
	strne	r2, [r3, #36]
	beq	.L321
.L307:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L323:
	ldr	r1, .L325+12
	mov	r2, #0
	mov	r3, r0
	str	r2, [sp]
	mov	r0, r2
	ldr	r2, .L325+16
	ldr	ip, [r1, #68]
	ldr	r1, .L325+20
	blx	ip
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L322:
	ldr	r3, .L325+12
	mov	r0, #0
	ldr	r1, .L325+24
.L320:
	ldr	r3, [r3, #68]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	bx	r3
.L321:
	mvn	r2, #5
	str	r2, [r3, #36]
	b	.L307
.L324:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r6, r5]
	bne	.L313
.L314:
	ldr	r3, .L325+12
	ldr	r1, .L325+28
	b	.L320
.L326:
	.align	2
.L325:
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+268
	.word	.LC5
	.word	.LC33
	.word	.LC31
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
	bgt	.L341
	movw	r4, #1208
	ldr	r6, .L343
	mul	r4, r4, r5
	ldr	r3, [r6, r4]
	cmp	r3, #0
	beq	.L342
.L330:
	sub	r2, r7, #1
	cmp	r2, #3
	ldrls	pc, [pc, r2, asl #2]
	b	.L332
.L334:
	.word	.L338
	.word	.L335
	.word	.L336
	.word	.L339
.L339:
	mov	r3, #40
.L333:
	movw	r1, #1208
	mul	r5, r1, r5
	ldr	r2, [r6, r5]
	ldr	r0, [r2, r3]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L338:
	mov	r3, #28
	b	.L333
.L336:
	mov	r3, #36
	b	.L333
.L335:
	mov	r3, #32
	b	.L333
.L332:
	ldr	ip, .L343+4
	mov	r3, r7
	ldr	r2, .L343+8
	mov	r0, #0
	ldr	r1, .L343+12
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L342:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r8, r0, #0
	beq	.L331
	str	r8, [r6, r4]
	b	.L330
.L341:
	ldr	ip, .L343+4
	mov	r3, r5
	ldr	r2, .L343+8
	mov	r0, #0
	ldr	r1, .L343+16
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L331:
	ldr	r3, .L343+4
	ldr	r2, .L343+8
	ldr	r1, .L343+20
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L344:
	.align	2
.L343:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+296
	.word	.LC36
	.word	.LC34
	.word	.LC35
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
	beq	.L346
	ldr	r6, .L373
	cmp	r0, #6
	moveq	r1, #1
	movne	r1, #0
	ldr	r0, [r6]
	bl	MEM_WritePhyWord
	cmp	r4, #0
	beq	.L350
	cmp	r4, #1
	bne	.L372
	ldr	r3, .L373+4
	mov	r1, r4
	ldr	r0, [r3]
	add	r0, r0, #8
	bl	MEM_WritePhyWord
.L353:
	ldr	r1, .L373+8
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L373+12
	ldr	ip, [r1, #68]
	ldr	r1, .L373+16
	blx	ip
.L346:
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L372:
	cmp	r4, #0
	bgt	.L353
.L352:
	cmp	r5, #17
	ldrls	pc, [pc, r5, asl #2]
	b	.L346
.L355:
	.word	.L365
	.word	.L356
	.word	.L357
	.word	.L358
	.word	.L346
	.word	.L359
	.word	.L360
	.word	.L346
	.word	.L361
	.word	.L362
	.word	.L363
	.word	.L363
	.word	.L363
	.word	.L364
	.word	.L346
	.word	.L346
	.word	.L365
	.word	.L366
.L350:
	ldr	r0, [r6]
	mov	r1, #1
	add	r0, r0, #8
	bl	MEM_WritePhyWord
	b	.L352
.L365:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	H264HAL_V300R001_StartDec
.L364:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VP8HAL_V300R001_StartDec
.L363:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VP6HAL_V300R001_StartDec
.L362:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	RV9HAL_V300R001_StartDec
.L361:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	RV8HAL_V300R001_StartDec
.L360:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	AVSHAL_V300R001_StartDec
.L359:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	DIVX3HAL_V300R001_StartDec
.L358:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	MP2HAL_V300R001_StartDec
.L357:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	MP4HAL_V300R001_StartDec
.L356:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VC1HAL_V300R001_StartDec
.L366:
	mov	r1, r4
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	HEVCHAL_V300R001_StartDec
.L374:
	.align	2
.L373:
	.word	s_RegPhyBaseAddr
	.word	s_RegPhyBaseAddr_1
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+324
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
	ldr	r3, .L382
	ldr	r3, [r3, r2]
	cmp	r3, #0
	beq	.L380
	cmp	r0, #0
	ble	.L381
	ldr	r1, .L382+4
	mov	r3, r0
	ldr	r2, .L382+8
	mov	r0, #1
	str	r0, [sp]
	mov	r0, #32
	ldr	ip, [r1, #68]
	ldr	r1, .L382+12
	blx	ip
	mov	r0, #0
.L377:
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L381:
	ldr	r0, [r3, #28]
	ubfx	r0, r0, #17, #1
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L380:
	ldr	r1, .L382+4
	mov	r0, r3
	ldr	r2, .L382+8
	ldr	r3, .L382+16
	ldr	ip, [r1, #68]
	ldr	r1, .L382+20
	blx	ip
	mvn	r0, #0
	b	.L377
.L383:
	.align	2
.L382:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+352
	.word	.LC38
	.word	.LC37
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
	ldr	r3, .L391
	ldr	r4, [r3, r2]
	cmp	r4, #0
	beq	.L389
	cmp	r0, #0
	ble	.L390
	ldr	r1, .L391+4
	mov	r3, r0
	mov	r4, #1
	ldr	r2, .L391+8
	str	r4, [sp]
	mov	r0, #32
	ldr	ip, [r1, #68]
	ldr	r1, .L391+12
	blx	ip
	mov	r0, r4
.L386:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L390:
	ldr	r0, [r4, #40]
	subs	r0, r0, #1
	movne	r0, #1
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L389:
	ldr	r3, .L391+4
	mov	r0, r4
	ldr	r1, .L391+16
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	b	.L386
.L392:
	.align	2
.L391:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+380
	.word	.LC38
	.word	.LC39
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
	ldreq	r1, .L431
	ldreq	r3, .L431+4
	beq	.L424
	ldr	r3, [r2, #4]
	ldr	r5, [r2]
	cmp	r3, #1
	bhi	.L427
	cmp	r3, #0
	bne	.L398
	ldr	r3, .L431+8
	ldr	r2, [r3]
	cmp	r2, #1
	addne	r1, r3, #60
	beq	.L415
.L401:
	ldr	r2, [r3, #4]!
	cmp	r2, #1
	beq	.L415
	cmp	r3, r1
	bne	.L401
	mov	r6, #0
.L399:
	ldr	r8, .L431+12
	ldr	lr, [r8]
	cmp	lr, #0
	beq	.L428
.L402:
	ldr	r7, [lr, #12]
	cmp	r6, #1
	ldr	r4, .L431+16
	str	r7, [r4]
	ldr	r3, [lr, #28]
	str	r3, [r4, #4]
	beq	.L429
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
	bne	.L406
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
.L406:
	rsb	r1, r4, #32768
	ldr	r3, .L431+20
	add	r1, r1, #208
	add	r1, r1, lr
	add	r0, r3, #128
.L407:
	ldr	r2, [r1, r3]
	ldr	ip, .L431+16
	str	r2, [r3, #4]!
	cmp	r3, r0
	bne	.L407
	cmp	r6, #0
	beq	.L412
.L413:
	and	r7, r7, #15
	cmp	r7, #5
	andeq	r9, r9, #1
	andne	r8, r8, #31
	moveq	r0, #0
	movne	r0, #0
	streq	r9, [r5]
	strne	r8, [r5]
.L419:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L412:
	movw	r2, #33152
	movw	r3, #33156
	ldr	r2, [lr, r2]
	str	r2, [ip, #48]
	ldr	r3, [lr, r3]
	str	r3, [ip, #44]
	b	.L413
.L415:
	mov	r6, r2
	b	.L399
.L429:
	ldr	r0, [r8, #32]
	ubfx	r7, r3, #0, #17
	bl	MEM_Phy2Vir
	subs	r2, r0, #0
	beq	.L430
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
	b	.L406
.L428:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	lr, r0, #0
	beq	.L403
	str	lr, [r8]
	b	.L402
.L398:
	ldr	r1, .L431
	mov	r0, #0
	ldr	r2, .L431+24
	str	r0, [sp]
	ldr	ip, [r1, #68]
	ldr	r1, .L431+28
	blx	ip
	mvn	r0, #0
	b	.L419
.L427:
	ldr	r3, .L431
	mov	r0, #0
	ldr	r1, .L431+32
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L419
.L430:
	ldr	r1, .L431
	ldr	r3, .L431+36
.L424:
	ldr	ip, [r1, #68]
	ldr	r2, .L431+24
	ldr	r1, .L431+40
	blx	ip
	mvn	r0, #0
	b	.L419
.L403:
	ldr	r3, .L431
	ldr	r1, .L431+44
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L419
.L432:
	.align	2
.L431:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC40
	.word	g_DSPState
	.word	g_HwMem
	.word	g_BackUp
	.word	g_BackUp+48
	.word	.LANCHOR0+408
	.word	.LC5
	.word	.LC41
	.word	.LC42
	.word	.LC1
	.word	.LC31
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
	beq	.L437
	cmp	r2, #800
	ldr	r3, .L439
	bhi	.L438
	mov	r2, r2, asl #2
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L438:
	mov	r0, r4
	ldr	r1, .L439+4
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L437:
	ldr	ip, .L439
	mov	r2, r0
	ldr	r1, .L439+8
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L440:
	.align	2
.L439:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC44
	.word	.LC43
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
	beq	.L442
	sub	r3, r2, #1
	cmp	r3, #255
	bhi	.L442
	ldr	r3, .L445
	mov	r2, r2, asl #2
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L442:
	ldr	r3, .L445
	mov	r0, #0
	ldr	r1, .L445+4
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L446:
	.align	2
.L445:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC45
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
	beq	.L480
	ldr	r3, [fp, #-56]
	ldr	r2, [r3, #840]
	sub	r3, r2, #1
	cmp	r3, #199
	bhi	.L481
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
	beq	.L451
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
.L452:
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
	beq	.L455
	cmp	r3, #5
	moveq	r4, #4
	beq	.L455
	cmp	r3, #4
	moveq	r4, #8
	beq	.L455
	ldr	r3, .L485
	mov	r0, #1
	ldr	r1, .L485+4
	mov	r4, r0
	ldr	r3, [r3, #68]
	blx	r3
.L455:
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #856]
	cmp	r3, #0
	bne	.L473
	ldr	r2, [fp, #-56]
	ldr	r8, [r2, #840]
	cmp	r8, #0
	ble	.L474
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
	b	.L468
.L459:
	ldr	r3, [fp, #-56]
	add	r6, r6, #1
	ldr	r3, [r3, #840]
	cmp	r3, r6
	mov	r8, r3
	ble	.L482
.L468:
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
	ble	.L458
	ldrsh	r0, [r5, #38]
	mov	r1, r4
	sub	r0, r0, #1
	add	r0, r0, r4
	bl	__aeabi_uidiv
	mul	r3, r4, r0
	cmp	r10, r3
	str	r3, [fp, #-72]
	ble	.L459
	b	.L460
.L473:
	cmp	r3, #1
	mov	r2, #0
	str	r2, [fp, #-60]
	beq	.L467
.L457:
	ldr	r3, [fp, #-60]
	sub	r3, r3, #1
	uxth	r3, r3
.L469:
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
.L479:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L461:
	bl	__aeabi_uidiv
	mov	r1, r4
	mul	r9, r4, r0
	ldrsh	r0, [r5, #44]
	bl	__aeabi_uidiv
	mul	r0, r4, r0
	cmp	r9, r0
	add	r2, r0, #1
	bhi	.L463
	cmp	r9, r2
	mov	r1, r4
	beq	.L463
	ldrsh	r0, [r5, #38]
	add	r0, r0, r7
	bl	__aeabi_uidiv
	mul	r0, r4, r0
	cmp	r9, r0
	bhi	.L483
.L463:
	ldrh	r9, [r5, #46]
	add	r5, r5, #4
	mov	r6, r10
	sxth	r9, r9
.L460:
	add	r10, r6, #1
	mov	r1, r4
	cmp	r10, r8
	add	r0, r7, r9
	blt	.L461
	bl	__aeabi_uidiv
	mul	r10, r4, r0
.L462:
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
	bgt	.L484
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
	b	.L459
.L458:
	ldr	r3, [fp, #-72]
	cmp	r10, r3
	movgt	r3, #0
	movle	r3, #1
	cmp	r6, #0
	moveq	r3, #0
	cmp	r3, #0
	bne	.L459
	b	.L462
.L483:
	mov	r10, r9
	b	.L462
.L482:
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #856]
	cmp	r3, #1
	bne	.L457
.L467:
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
	b	.L469
.L451:
	mul	r3, r3, lr
	mov	r2, #0
	b	.L452
.L484:
	ldr	r2, [fp, #-56]
	mov	r3, #1
	str	r3, [r2, #856]
	b	.L467
.L474:
	str	r3, [fp, #-60]
	b	.L457
.L481:
	ldr	r3, .L485
	mov	r0, #0
	ldr	r1, .L485+8
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L479
.L480:
	ldr	r1, .L485
	ldr	r3, .L485+12
	ldr	r2, .L485+16
	ldr	ip, [r1, #68]
	ldr	r1, .L485+20
	blx	ip
	mvn	r0, #0
	b	.L479
.L486:
	.align	2
.L485:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC48
	.word	.LC47
	.word	.LC46
	.word	.LANCHOR0+436
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
	bhi	.L496
	cmp	r3, #0
	bgt	.L497
	ldr	r3, [r1]
	cmp	r3, #0
	beq	.L498
.L492:
	movw	r3, #1208
	ldr	r2, .L499
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
.L495:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L498:
	mov	r0, #0
	str	r1, [fp, #-24]
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L493
	ldr	r1, [fp, #-24]
	str	r3, [r1]
	b	.L492
.L497:
	ldr	r1, .L499+4
	mov	r2, #0
	mov	r0, r2
	str	r2, [sp]
	ldr	r2, .L499+8
	ldr	ip, [r1, #68]
	ldr	r1, .L499+12
	blx	ip
	mvn	r0, #0
	b	.L495
.L496:
	ldr	r3, .L499+4
	mov	r0, #0
	ldr	r1, .L499+16
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L495
.L493:
	ldr	r3, .L499+4
	ldr	r1, .L499+20
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L495
.L500:
	.align	2
.L499:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+464
	.word	.LC5
	.word	.LC49
	.word	.LC31
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
	ldreq	r1, .L534
	ldreq	r3, .L534+4
	beq	.L526
	ldmia	r3, {r8, r9}
	ldr	r5, [r3, #8]
	cmp	r9, #0
	beq	.L528
	movw	r3, #1208
	ldr	r7, .L534+8
	mul	r3, r3, r5
	ldr	r0, [r7, r3]
	cmp	r0, #0
	beq	.L505
	ldr	r3, .L534+12
	ldr	r2, [r3]
	cmp	r2, #1
	addne	r1, r3, #60
	beq	.L520
.L508:
	ldr	r2, [r3, #4]!
	cmp	r2, #1
	beq	.L520
	cmp	r3, r1
	bne	.L508
	mov	r4, #0
.L506:
	ldr	r6, .L534
	mov	r2, #816
	mov	r1, #0
	mov	r0, r9
	ldr	r3, [r6, #48]
	blx	r3
	movw	r2, #1208
	mla	r2, r2, r5, r7
	ldr	r1, .L534+16
	mov	r3, #180
	mla	r3, r3, r5, r1
	ldr	r2, [r2, #24]
	str	r2, [r9, #4]
	ldr	r3, [r3, #4]
	mov	r3, r3, lsr #17
	and	r2, r3, #3
	cmp	r2, #1
	moveq	r3, #0
	beq	.L509
	eor	r3, r3, #1
	and	r3, r3, #1
	cmp	r8, #3
	orrne	r3, r3, #1
.L509:
	mov	r2, #180
	str	r3, [r9]
	mla	r3, r2, r5, r1
	bics	r8, r8, #16
	ldr	r2, [r3, #4]
	ubfx	r2, r2, #0, #17
	str	r2, [r9, #8]
	beq	.L529
.L510:
	cmp	r2, #200
	bhi	.L530
.L512:
	movw	r3, #1208
	mla	r7, r3, r5, r7
	ldr	r8, [r7, #32]
	mov	r0, r8
	bl	MEM_Phy2Vir
	subs	r1, r0, #0
	beq	.L531
	ldr	r7, .L534+20
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
	beq	.L517
.L516:
	ldr	r0, [r3, #4]
	add	r1, r1, #1
	cmp	r1, ip
	add	r3, r3, #16
	add	r2, r2, #4
	strh	r0, [r2, #8]	@ movhi
	ldr	r0, [r3, #-8]
	strh	r0, [r2, #10]	@ movhi
	bne	.L516
.L517:
	mov	r0, #6
	bl	IsDprintTypeEnable
	cmp	r0, #0
	bne	.L532
.L503:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L529:
	cmp	r4, #1
	beq	.L533
	ldr	r1, .L534+24
	ldrb	r1, [r1]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L510
	ldrb	r3, [r3, #19]	@ zero_extendqisi2
	and	r3, r3, #3
	cmp	r3, #2
	bne	.L510
	mov	r3, #1
	strh	r1, [r9, #12]	@ movhi
	mov	r0, r1
	strh	r1, [r9, #14]	@ movhi
	str	r3, [r9, #8]
	b	.L503
.L520:
	mov	r4, r2
	b	.L506
.L532:
	ldr	r3, [r6, #68]
	mov	r0, #6
	ldr	r2, [r9, #8]
	mov	r4, #0
	ldr	r1, .L534+28
	blx	r3
	ldr	r3, [r6, #68]
	mov	r2, r8
	mov	r0, #6
	ldr	r1, .L534+32
	blx	r3
	mov	r3, #3200
	mla	r5, r3, r5, r7
.L518:
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
	ldr	r1, .L534+36
	blx	ip
	ldr	r3, [r9, #8]
	ldr	r2, .L534
	mov	r3, r3, asl #2
	sub	r3, r3, #3
	cmp	r3, r4
	bhi	.L518
	ldr	r3, [r2, #68]
	mov	r0, #6
	ldr	r1, .L534+40
	blx	r3
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L530:
	mov	r3, #200
	ldr	ip, [r6, #68]
	ldr	r1, .L534+44
	mov	r0, #1
	blx	ip
	mov	r3, #0
	str	r3, [r9, #8]
	b	.L512
.L533:
	ldr	r3, [r3, #16]
	ubfx	r3, r3, #21, #2
	cmp	r3, #2
	bne	.L510
	str	r4, [r9, #8]
	mov	r0, r8
	strh	r8, [r9, #12]	@ movhi
	strh	r8, [r9, #14]	@ movhi
	b	.L503
.L528:
	ldr	r1, .L534
	mov	r0, r9
	ldr	r3, .L534+48
.L526:
	ldr	ip, [r1, #68]
	ldr	r2, .L534+52
	ldr	r1, .L534+56
	blx	ip
	mvn	r0, #0
	b	.L503
.L531:
	ldr	ip, [r6, #68]
	ldr	r3, .L534+60
	ldr	r2, .L534+52
	ldr	r1, .L534+56
	blx	ip
	mvn	r0, #0
	b	.L503
.L505:
	ldr	r1, .L534
	ldr	r3, .L534+64
	b	.L526
.L535:
	.align	2
.L534:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC50
	.word	g_HwMem
	.word	g_DSPState
	.word	g_BackUp
	.word	g_UpMsg
	.word	g_not_allow_H264FullPictureRepair_flag
	.word	.LC53
	.word	.LC54
	.word	.LC55
	.word	.LC56
	.word	.LC52
	.word	.LC51
	.word	.LANCHOR0+492
	.word	.LC1
	.word	.LC42
	.word	.LC37
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
	ldr	r6, .L556
	cmp	r3, #1
	mov	r4, r3
	mov	r8, r0
	mov	r9, r1
	add	r7, r5, r6
	bhi	.L552
	cmp	r3, #0
	bgt	.L553
	ldr	r3, [r5, r6]
	cmp	r3, #0
	beq	.L554
.L541:
	cmp	r2, #0
	bne	.L543
	ldr	r5, .L556+4
	mov	r3, #1744
	mla	r3, r3, r4, r5
	ldr	r1, [r3, #840]
	cmp	r1, #0
	ble	.L544
	cmp	r8, #6
	bne	.L545
	ldr	r1, [r9, #32]
	cmp	r1, #1
	moveq	r2, r1
	str	r2, [r3, #1724]
.L545:
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
.L543:
	cmp	r2, #1
	movne	r0, #0
	beq	.L555
.L539:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L555:
	cmp	r8, #6
	bne	.L547
	ldr	r3, [r9, #32]
	cmp	r3, #1
	bne	.L547
	ldr	r5, .L556+4
	mov	r3, #1744
	mla	r3, r3, r4, r5
	ldr	r2, [r3, #1712]
	cmp	r2, #0
	ble	.L547
	ldr	r2, .L556+8
	mov	r1, #2
	str	r1, [r3, #1724]
	ldr	r3, [r2, #56]
	cmp	r3, #0
	beq	.L548
	mov	r0, r4
	blx	r3
.L549:
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
.L554:
	mov	r0, #0
	str	r2, [fp, #-40]
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L542
	str	r3, [r5, r6]
	ldr	r2, [fp, #-40]
	b	.L541
.L553:
	ldr	r1, .L556+12
	mov	r2, #0
	mov	r0, r2
	str	r2, [sp]
	ldr	r2, .L556+16
	ldr	ip, [r1, #68]
	ldr	r1, .L556+20
	blx	ip
	mvn	r0, #0
	b	.L539
.L547:
	ldr	r3, .L556+12
	mov	r0, #0
	ldr	r1, .L556+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L539
.L548:
	ldr	ip, .L556+12
	mov	r0, r3
	movw	r2, #2872
	ldr	r1, .L556+28
	ldr	r3, [ip, #68]
	blx	r3
	b	.L549
.L552:
	ldr	r3, .L556+12
	mov	r0, #0
	ldr	r1, .L556+32
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L539
.L544:
	ldr	r3, .L556+12
	mov	r0, r2
	ldr	r1, .L556+36
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L539
.L542:
	ldr	r3, .L556+12
	ldr	r1, .L556+40
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L539
.L557:
	.align	2
.L556:
	.word	g_HwMem
	.word	g_RepairParam
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+524
	.word	.LC5
	.word	.LC60
	.word	.LC10
	.word	.LC57
	.word	.LC59
	.word	.LC58
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
	ble	.L561
	ldr	r5, .L562
	mov	r6, #1
	mov	r3, r4
	ldr	r2, .L562+4
	str	r6, [sp]
	mov	r0, #32
	ldr	ip, [r5, #68]
	ldr	r1, .L562+8
	blx	ip
	movw	r0, #49156
	movt	r0, 63683
	bl	MEM_ReadPhyWord
	ldr	r3, .L562+12
	ldr	r3, [r3]
	uxth	r1, r0
	add	r0, r3, #4
	bl	MEM_WritePhyWord
	str	r6, [sp]
	mov	r3, r4
	ldr	ip, [r5, #68]
	ldr	r2, .L562+4
	mov	r0, #32
	ldr	r1, .L562+16
	blx	ip
	ldr	r3, [r5, #112]
	blx	r3
	str	r6, [sp]
	mov	r3, r4
	ldr	ip, [r5, #68]
	ldr	r2, .L562+4
	mov	r0, #32
	ldr	r1, .L562+16
	blx	ip
	str	r6, [sp]
	mov	r3, r4
	ldr	ip, [r5, #68]
	ldr	r2, .L562+4
	mov	r0, #32
	ldr	r1, .L562+16
	blx	ip
	str	r6, [sp]
	mov	r3, r4
	ldr	ip, [r5, #68]
	ldr	r2, .L562+4
	mov	r0, #32
	ldr	r1, .L562+16
	blx	ip
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L561:
	movw	r3, #1208
	ldr	r5, .L562+20
	mul	r4, r3, r4
	movw	r0, #49156
	movt	r0, 63683
	ldr	r3, [r5, r4]
	ldr	r6, [r3, #8]
	bl	MEM_ReadPhyWord
	ldr	r3, .L562+12
	orr	r6, r6, #67108864
	ldr	r3, [r3]
	uxth	r1, r0
	add	r0, r3, #4
	bl	MEM_WritePhyWord
	ldr	r2, [r5, r4]
	ldr	r3, .L562
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
.L563:
	.align	2
.L562:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+556
	.word	.LC38
	.word	s_RegPhyBaseAddr
	.word	.LC61
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
	ldr	r9, .L593
	ldr	r4, [r9, #64]
	cmp	r4, #0
	blt	.L587
	movw	r10, #56367
	add	r6, r9, #64
	add	r9, r9, #124
	movt	r10, 1315
	mov	r8, #30
	mov	r7, #0
.L566:
	mov	r0, r4
	bl	VCTRL_IsChanActive
	cmp	r0, #0
	bne	.L569
	ldr	r3, .L593+4
	mov	r0, r4
	ldr	r2, .L593+8
	ldr	r3, [r3, r4, asl #2]
	ldr	ip, [r2, r4, asl #2]
	ldr	r1, [r3, #64]
	ldr	r2, [r3, #68]
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
	b	.L569
.L572:
	.word	.L581
	.word	.L581
	.word	.L581
	.word	.L581
	.word	.L569
	.word	.L581
	.word	.L581
	.word	.L569
	.word	.L581
	.word	.L581
	.word	.L581
	.word	.L581
	.word	.L581
	.word	.L581
	.word	.L569
	.word	.L569
	.word	.L582
	.word	.L581
.L581:
	mul	r4, r5, r4
	mul	r2, r8, r4
	ubfx	r2, r2, #1, #26
	umull	r2, r3, r2, r10
	add	r7, r7, r3, lsr #7
.L569:
	cmp	r6, r9
	beq	.L585
	ldr	r4, [r6, #4]!
	cmp	r4, #0
	bge	.L566
.L585:
	rsb	r7, r7, #32
	cmp	r7, #25
	movge	r7, #25
	cmp	r7, #0
	andgt	r7, r7, #31
	ble	.L592
.L565:
	ldr	r3, .L593+12
	ldr	r2, [r3]
	ldr	r3, [r2, #120]
	bfi	r3, r7, #12, #5
.L590:
	bfc	r3, #17, #1
	str	r3, [r2, #120]
	ldr	r3, [r2, #120]
	orr	r3, r3, #131072
	str	r3, [r2, #120]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L582:
	mul	r4, r5, r4
	mul	r2, r8, r4
	bic	r2, r2, #-67108864
	umull	r2, r3, r2, r10
	add	r7, r7, r3, lsr #7
	b	.L569
.L592:
	ldr	r3, .L593+12
	ldr	r2, [r3]
	ldr	r3, [r2, #120]
	bfc	r3, #12, #5
	b	.L590
.L587:
	mov	r7, #25
	b	.L565
.L594:
	.align	2
.L593:
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
	ldr	r4, .L616
	mov	r6, r0, asl #3
	mov	r5, r0, asl #6
	rsb	r3, r6, r5
	ldr	r8, .L616+4
	add	r3, r4, r3
	ldr	r10, .L616+8
	mov	r7, r0
	ldr	r9, .L616+12
	ldr	r2, [r3, #8]
	ldr	r0, [r3, #12]
	ldr	r3, [r10]
	ldr	r1, [r8, r2, asl #2]
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
	ldr	r3, [r9, #116]
	blx	r3
	rsb	r3, r6, r5
	add	r3, r4, r3
	ldr	r1, [r3, #8]
	ldr	r3, [r8, r1, asl #2]
	ldr	r2, [r3, #1232]
	cmp	r2, #1
	beq	.L598
	ldr	r3, [r3, #1220]
	bics	r3, r3, #1024
	beq	.L599
.L598:
	ldr	r3, [r10]
	ldr	r2, [r3, #120]
	bfc	r2, #12, #5
	bfc	r2, #17, #1
	str	r2, [r3, #120]
	ldr	r2, [r3, #120]
	orr	r2, r2, #131072
	str	r2, [r3, #120]
.L600:
	ldr	r3, [r8, r1, asl #2]
	ldr	r3, [r3, #528]
	cmp	r3, #17
	moveq	r1, #0
	movne	r1, #3
	cmp	r7, #0
	beq	.L603
	cmp	r7, #1
	bne	.L615
	ldr	r3, .L616+16
	ldr	r0, [r3]
	add	r0, r0, #12
	bl	MEM_WritePhyWord
	ldr	r3, [r9, #112]
	blx	r3
.L606:
	mov	r0, #0
	ldr	ip, [r9, #68]
	mov	r3, r7
	str	r0, [sp]
	ldr	r2, .L616+20
	ldr	r1, .L616+24
	blx	ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L615:
	ldr	r3, [r9, #68]
	mov	r0, #0
	ldr	r1, .L616+28
	blx	r3
	ldr	r3, [r9, #112]
	blx	r3
	cmp	r7, #0
	bgt	.L606
.L605:
	movw	r3, #1208
	ldr	r2, .L616+32
	mul	r7, r3, r7
	mov	r3, #0
	rsb	r5, r6, r5
	mov	lr, #1
	add	r4, r4, r5
	ldr	ip, .L616+36
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
.L603:
	ldr	r3, .L616+40
	ldr	r0, [r3]
	add	r0, r0, #12
	bl	MEM_WritePhyWord
	ldr	r3, [r9, #112]
	blx	r3
	b	.L605
.L599:
	mov	r0, r7
	str	r1, [fp, #-48]
	bl	VDMHAL_V300R001_SetVdhClkSkip
	ldr	r1, [fp, #-48]
	b	.L600
.L617:
	.align	2
.L616:
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	g_pstRegCrg
	.word	vfmw_Osal_Func_Ptr_S
	.word	s_RegPhyBaseAddr_1
	.word	.LANCHOR0+588
	.word	.LC5
	.word	.LC62
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
	ldr	r1, .L619
	mov	r0, #15
	ldr	r3, .L619+4
	mov	r2, #4
	str	r0, [r1]
	str	r2, [r3]
	ldmfd	sp, {fp, sp, pc}
.L620:
	.align	2
.L619:
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
	beq	.L622
	cmp	r3, #2
	moveq	r4, #1
	movne	r4, #2
.L622:
	ldr	r3, [fp, #-92]
	cmp	r3, #0
	beq	.L621
	mov	r3, #0
	mov	r2, #4194304
	mov	r1, r3
	str	r3, [sp]
	ldr	r0, .L715
	sub	r3, fp, #76
	bl	MEM_AllocMemBlock
	subs	r1, r0, #0
	beq	.L711
.L708:
	ldr	r3, .L715+4
	mov	r0, #1
	ldr	r1, .L715+8
	ldr	r3, [r3, #68]
	blx	r3
.L621:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L711:
	str	r1, [sp]
	mov	r2, #4194304
	sub	r3, fp, #60
	ldr	r0, .L715+12
	bl	MEM_AllocMemBlock
	cmp	r0, #0
	bne	.L708
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
	bne	.L627
	ldr	r3, [fp, #-84]
	cmp	r3, #0
	movne	r8, r0
	ldrne	r10, .L715+4
	strne	r8, [fp, #-80]
	beq	.L712
.L628:
	cmp	r5, #0
	beq	.L632
	ldr	r2, [fp, #-80]
	mov	r4, #0
	and	r9, r2, #15
	mov	r3, r2, lsr #4
	ldr	r2, [fp, #-88]
	mul	r3, r2, r3
	add	r9, r3, r9, lsl #8
.L630:
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
	bhi	.L630
.L632:
	ldr	r3, [fp, #-80]
	add	r8, r8, r5
	ldr	r2, [fp, #-84]
	add	r3, r3, #1
	str	r3, [fp, #-80]
	cmp	r2, r3
	bne	.L628
	ldr	r4, [fp, #-84]
	mov	r0, r6
	ldr	ip, [r10, #44]
	ldr	r2, [fp, #-92]
	mul	r1, r4, r5
	blx	ip
	movs	r3, r4, lsr #1
	str	r3, [fp, #-84]
	beq	.L664
	ldr	r3, [fp, #-88]
	mov	r7, #0
	ldr	r8, [fp, #-112]
	str	r7, [fp, #-80]
	mov	r3, r3, asr #1
	str	r3, [fp, #-88]
.L634:
	cmp	r5, #0
	beq	.L636
	ldr	r2, [fp, #-80]
	mov	r4, #0
	and	r9, r2, #7
	mov	r3, r2, lsr #3
	ldr	r2, [fp, #-88]
	mul	r3, r2, r3
	add	r9, r3, r9, lsl #8
.L635:
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
	bhi	.L635
.L636:
	ldr	r3, [fp, #-80]
	add	r7, r7, r5
	ldr	r2, [fp, #-84]
	add	r3, r3, #1
	str	r3, [fp, #-80]
	cmp	r3, r2
	bne	.L634
.L664:
	ldr	r3, [fp, #4]
	cmp	r3, #1
	beq	.L713
	ldr	r3, [fp, #4]
	cmp	r3, #0
	beq	.L661
.L656:
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
.L627:
	subs	r3, r4, #1
	ldr	r2, [fp, #-84]
	ldr	r10, .L715+4
	movne	r3, #1
	cmp	r2, r3
	str	r3, [fp, #-116]
	ldrhi	r3, [fp, #-116]
	strhi	r3, [fp, #-80]
	bls	.L646
.L648:
	cmp	r5, #0
	beq	.L649
	ldr	r2, [fp, #-80]
	mov	r4, #0
	and	r8, r2, #15
	mov	r3, r2, lsr #4
	mov	r9, r2, lsr #1
	ldr	r2, [fp, #-88]
	mul	r9, r5, r9
	mul	r3, r2, r3
	add	r8, r3, r8, lsl #8
.L647:
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
	bhi	.L647
.L649:
	ldr	r3, [fp, #-80]
	ldr	r2, [fp, #-84]
	add	r3, r3, #2
	str	r3, [fp, #-80]
	cmp	r2, r3
	bhi	.L648
.L646:
	ldr	r4, [fp, #-84]
	mov	r0, r6
	ldr	ip, [r10, #44]
	ldr	r2, [fp, #-92]
	mul	r1, r4, r5
	mov	r1, r1, lsr #1
	blx	ip
	movs	r3, r4, lsr #1
	str	r3, [fp, #-84]
	beq	.L650
	ldr	r3, [fp, #-88]
	mov	r7, #0
	ldr	r8, [fp, #-112]
	str	r7, [fp, #-80]
	mov	r3, r3, asr #1
	str	r3, [fp, #-88]
.L651:
	cmp	r5, #0
	beq	.L653
	ldr	r2, [fp, #-80]
	mov	r4, #0
	and	r9, r2, #7
	mov	r3, r2, lsr #3
	ldr	r2, [fp, #-88]
	mul	r3, r2, r3
	add	r9, r3, r9, lsl #8
.L652:
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
	bhi	.L652
.L653:
	ldr	r3, [fp, #-80]
	add	r7, r7, r5
	ldr	r2, [fp, #-84]
	add	r3, r3, #1
	str	r3, [fp, #-80]
	cmp	r3, r2
	bne	.L651
.L650:
	ldr	r3, [fp, #4]
	cmp	r3, #1
	beq	.L714
	ldr	r3, [fp, #4]
	cmp	r3, #0
	bne	.L656
	ldr	r2, [fp, #-100]
	ldr	r3, [fp, #-116]
	cmp	r2, r3
	bls	.L661
.L662:
	add	r3, r3, #2
	cmp	r2, r3
	bhi	.L662
.L661:
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
	b	.L656
.L714:
	ldr	r3, [fp, #-100]
	cmp	r3, #0
	beq	.L656
	ldr	r2, [fp, #-116]
	mov	r4, #0
	ldr	r0, [fp, #-104]
	mov	lr, r3
	ldr	ip, [fp, #-96]
	mla	r6, r5, r2, r6
	mov	r5, r5, asl #1
	add	r6, r6, #1
.L657:
	cmp	r0, #0
	beq	.L660
	mov	r3, r4, lsr #1
	mov	r2, r6
	mul	r3, r0, r3
	sub	r1, r3, #-67108863
	add	r8, r3, r0
	sub	r1, r1, #65011712
	add	r8, ip, r8
	add	r1, ip, r1
	add	r3, ip, r3
.L658:
	ldrb	r7, [r2, #-1]	@ zero_extendqisi2
	strb	r7, [r1, #1]!
	ldrb	r7, [r2], #2	@ zero_extendqisi2
	strb	r7, [r3], #1
	cmp	r3, r8
	bne	.L658
.L660:
	add	r4, r4, #2
	add	r6, r6, r5
	cmp	lr, r4
	bhi	.L657
	b	.L656
.L713:
	ldr	r3, [fp, #-100]
	cmp	r3, #0
	beq	.L656
	ldr	r2, [fp, #-96]
	add	r6, r6, #1
	ldr	lr, [fp, #-104]
	mov	r7, r3
	mov	r4, #0
	mov	r3, r2
.L640:
	cmp	lr, #0
	moveq	ip, r3
	beq	.L643
	sub	r1, r3, #-67108863
	add	ip, r3, lr
	sub	r1, r1, #65011712
	mov	r2, r6
.L641:
	ldrb	r0, [r2, #-1]	@ zero_extendqisi2
	strb	r0, [r1, #1]!
	ldrb	r0, [r2], #2	@ zero_extendqisi2
	strb	r0, [r3], #1
	cmp	ip, r3
	bne	.L641
.L643:
	add	r4, r4, #1
	mov	r3, ip
	cmp	r4, r7
	add	r6, r6, r5
	bne	.L640
	b	.L656
.L712:
	ldr	r10, .L715+4
	mov	r0, r6
	ldr	r1, [fp, #-84]
	ldr	r2, [fp, #-92]
	ldr	ip, [r10, #44]
	blx	ip
	b	.L664
.L716:
	.align	2
.L715:
	.word	.LC63
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC64
	.word	.LC65
	UNWIND(.fnend)
	.size	VDMHAL_V300R001_WriteBigTitle1DYuv, .-VDMHAL_V300R001_WriteBigTitle1DYuv
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.28061, %object
	.size	__func__.28061, 24
__func__.28061:
	.ascii	"VDMHAL_V300R001_OpenHAL\000"
	.type	__func__.28096, %object
	.size	__func__.28096, 27
__func__.28096:
	.ascii	"VDMHAL_V300R001_CalcFsSize\000"
	.space	1
	.type	__func__.28109, %object
	.size	__func__.28109, 34
__func__.28109:
	.ascii	"VDMHAL_V300R001_FillVdmMemElement\000"
	.space	2
	.type	__func__.28153, %object
	.size	__func__.28153, 27
__func__.28153:
	.ascii	"VDMHAL_V300R001_ArrangeMem\000"
	.space	1
	.type	__FUNCTION__.28113, %object
	.size	__FUNCTION__.28113, 34
__FUNCTION__.28113:
	.ascii	"VDMHAL_V300R001_FillVdmMemElement\000"
	.space	2
	.type	__func__.28193, %object
	.size	__func__.28193, 31
__func__.28193:
	.ascii	"VDMHAL_V300R001_ArrangeMem_BTL\000"
	.space	1
	.type	__func__.28214, %object
	.size	__func__.28214, 25
__func__.28214:
	.ascii	"VDMHAL_V300R001_ResetVdm\000"
	.space	3
	.type	__func__.28241, %object
	.size	__func__.28241, 30
__func__.28241:
	.ascii	"VDMHAL_V300R001_ClearIntState\000"
	.space	2
	.type	__func__.28251, %object
	.size	__func__.28251, 24
__func__.28251:
	.ascii	"VDMHAL_V300R001_MaskInt\000"
	.type	__func__.28263, %object
	.size	__func__.28263, 26
__func__.28263:
	.ascii	"VDMHAL_V300R001_EnableInt\000"
	.space	2
	.type	__func__.28272, %object
	.size	__func__.28272, 25
__func__.28272:
	.ascii	"VDMHAL_V300R001_CheckReg\000"
	.space	3
	.type	__func__.28289, %object
	.size	__func__.28289, 27
__func__.28289:
	.ascii	"VDMHAL_V300R001_PrepareDec\000"
	.space	1
	.type	__func__.28310, %object
	.size	__func__.28310, 27
__func__.28310:
	.ascii	"VDMHAL_V300R001_IsVdmReady\000"
	.space	1
	.type	__func__.28315, %object
	.size	__func__.28315, 25
__func__.28315:
	.ascii	"VDMHAL_V300R001_IsVdmRun\000"
	.space	3
	.type	__func__.28329, %object
	.size	__func__.28329, 27
__func__.28329:
	.ascii	"VDMHAL_V300R001_BackupInfo\000"
	.space	1
	.type	__func__.28374, %object
	.size	__func__.28374, 25
__func__.28374:
	.ascii	"VDMHAL_V300R001_CfgRpMsg\000"
	.space	3
	.type	__func__.28395, %object
	.size	__func__.28395, 25
__func__.28395:
	.ascii	"VDMHAL_V300R001_CfgRpReg\000"
	.space	3
	.type	__func__.28408, %object
	.size	__func__.28408, 30
__func__.28408:
	.ascii	"VDMHAL_V300R001_MakeDecReport\000"
	.space	2
	.type	__func__.28433, %object
	.size	__func__.28433, 30
__func__.28433:
	.ascii	"VDMHAL_V300R001_PrepareRepair\000"
	.space	2
	.type	__func__.28441, %object
	.size	__func__.28441, 30
__func__.28441:
	.ascii	"VDMHAL_V300R001_StartHwRepair\000"
	.space	2
	.type	__func__.28489, %object
	.size	__func__.28489, 30
__func__.28489:
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
	ASCII(.ascii	"%s, FrameStore %d over bountry, Offset: 0x%x, curFs" )
	ASCII(.ascii	"Size: 0x%x, phyadd: 0x%x\012\000" )
	.space	3
.LC24:
	ASCII(.ascii	"Mem addr is 0\000" )
	.space	2
.LC25:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V300R001_ResetVdm\012\000" )
	.space	2
.LC26:
	ASCII(.ascii	"VDMHAL_V300R001_ResetVdm: map vdm register fail, vi" )
	ASCII(.ascii	"r(reg) = (%p)\012\000" )
	.space	2
.LC27:
	ASCII(.ascii	"VDH: %d VDMHAL_V300R001_ResetVdm ERROR!!!\012\000" )
	.space	1
.LC28:
	ASCII(.ascii	"Ignore the reset request when the VDH clock is clos" )
	ASCII(.ascii	"ed.\012\000" )
.LC29:
	ASCII(.ascii	"GLB Reset failed!\012\000" )
	.space	1
.LC30:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200R003_ClearIntState\012\000" )
	.space	1
.LC31:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
.LC32:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200_MaskInt\012\000" )
	.space	3
.LC33:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200R003_EnableInt\012\000" )
	.space	1
.LC34:
	ASCII(.ascii	"%s: VdhId(%d) Invalid!\012\000" )
.LC35:
	ASCII(.ascii	"%s: vdm register virtual address not mapped, reset " )
	ASCII(.ascii	"failed!\012\000" )
.LC36:
	ASCII(.ascii	"%s: unkown reg_id = %d\012\000" )
.LC37:
	ASCII(.ascii	"VDM register not mapped yet!\000" )
	.space	3
.LC38:
	ASCII(.ascii	"%s: RD_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\012\000" )
	.space	3
.LC39:
	ASCII(.ascii	"VDM register not mapped yet!\012\000" )
	.space	2
.LC40:
	ASCII(.ascii	"pBackUpInfo = NULL!\000" )
.LC41:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V300R001_BackupInfo\012\000" )
.LC42:
	ASCII(.ascii	"can NOT map vir addr for up-msg\000" )
.LC43:
	ASCII(.ascii	"ReadUpMsgSlot error! pDst=%p, pSrc=%p\012\000" )
	.space	1
.LC44:
	ASCII(.ascii	"ReadUpMsgSlot error! upmsg_size(%d) > 512\012\000" )
	.space	1
.LC45:
	ASCII(.ascii	"WriteMsgSlot error!\012\000" )
	.space	3
.LC46:
	ASCII(.ascii	"can not map repair msg virtual address!\000" )
.LC47:
	ASCII(.ascii	"ValidGroupNum=%d out of range!\012\000" )
.LC48:
	ASCII(.ascii	"align_mb error\012\000" )
.LC49:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V300_CfgRpReg\012\000" )
	.space	2
.LC50:
	ASCII(.ascii	"'pMakeDecReport' is NULL\000" )
	.space	3
.LC51:
	ASCII(.ascii	"'pDecReport' is NULL\000" )
	.space	3
.LC52:
	ASCII(.ascii	"pDecReport->DecSliceNum(%d) > %d, set to 0 for full" )
	ASCII(.ascii	" repair.\012\000" )
	.space	3
.LC53:
	ASCII(.ascii	"\012***** UpMsg DecSliceNum=%d\012\000" )
	.space	3
.LC54:
	ASCII(.ascii	"\012***** Up Msg (phy addr: %#8x) *****\012\000" )
	.space	2
.LC55:
	ASCII(.ascii	"\0120x%02x 0x%08x 0x%08x 0x%08x 0x%08x\012\000" )
	.space	3
.LC56:
	ASCII(.ascii	"\012***** Up Msg print finished *****\012\000" )
.LC57:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200R003_PrepareRepair\012\000" )
	.space	1
.LC58:
	ASCII(.ascii	"vdm register virtual address not mapped, VDMHAL_V20" )
	ASCII(.ascii	"0R003_PrepareRepair failed!\012\000" )
.LC59:
	ASCII(.ascii	"FIRST_REPAIR Parameter Error!\012\000" )
	.space	1
.LC60:
	ASCII(.ascii	"SECOND_REPAIR Parameter Error!\012\000" )
.LC61:
	ASCII(.ascii	"%s: WR_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\012\000" )
	.space	3
.LC62:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V300R001_StartHwDecode\012\000" )
	.space	1
.LC63:
	ASCII(.ascii	"BigTile1d_y\000" )
.LC64:
	ASCII(.ascii	"failed mem_allocMemBlock BigTile_yuv save!\012\000" )
.LC65:
	ASCII(.ascii	"BigTile1d_uv\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
