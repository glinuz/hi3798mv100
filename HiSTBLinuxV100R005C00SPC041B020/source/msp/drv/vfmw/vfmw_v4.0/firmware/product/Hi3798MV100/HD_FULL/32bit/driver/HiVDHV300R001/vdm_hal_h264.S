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
	.file	"vdm_hal_h264.c"
	.text
	.align	2
	.global	Write_V300R001_CabacTab
	.type	Write_V300R001_CabacTab, %function
Write_V300R001_CabacTab:
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
	bl	MEM_Phy2Vir
	ldr	r3, .L6
	cmp	r0, #0
	beq	.L5
	mov	r2, #5120
	ldr	r1, .L6+4
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L5:
	mov	r2, #87
	ldr	r1, .L6+8
	ldr	r3, [r3, #68]
	mov	r0, #22
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L7:
	.align	2
.L6:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_CabacMN
	.word	.LC0
	UNWIND(.fnend)
	.size	Write_V300R001_CabacTab, .-Write_V300R001_CabacTab
	.align	2
	.global	H264HAL_V300R001_InitHal
	.type	H264HAL_V300R001_InitHal, %function
H264HAL_V300R001_InitHal:
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
	bl	Write_V300R001_CabacTab
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r3, .L13
	mov	r0, #1
	ldr	r1, .L13+4
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L14:
	.align	2
.L13:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC1
	UNWIND(.fnend)
	.size	H264HAL_V300R001_InitHal, .-H264HAL_V300R001_InitHal
	.align	2
	.global	Get_V300R001_VirAddr
	.type	Get_V300R001_VirAddr, %function
Get_V300R001_VirAddr:
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
	add	r0, r1, r0, lsl #2
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Get_V300R001_VirAddr, .-Get_V300R001_VirAddr
	.align	2
	.global	Get_V300R001_PhyAddr
	.type	Get_V300R001_PhyAddr, %function
Get_V300R001_PhyAddr:
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
	add	r0, r1, r0, lsl #2
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Get_V300R001_PhyAddr, .-Get_V300R001_PhyAddr
	.align	2
	.global	H264HAL_V300R001_CutSliceChain
	.type	H264HAL_V300R001_CutSliceChain, %function
H264HAL_V300R001_CutSliceChain:
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
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L20
	mov	r0, #0
	str	r0, [r3, #252]
	ldmfd	sp, {fp, sp, pc}
.L20:
	ldr	r1, .L21
	ldr	r3, .L21+4
	ldr	r2, .L21+8
	ldr	ip, [r1, #68]
	ldr	r1, .L21+12
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L22:
	.align	2
.L21:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LANCHOR0
	.word	.LC3
	UNWIND(.fnend)
	.size	H264HAL_V300R001_CutSliceChain, .-H264HAL_V300R001_CutSliceChain
	.align	2
	.global	Write_V300R001_Qmatrix
	.type	Write_V300R001_Qmatrix, %function
Write_V300R001_Qmatrix:
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
	ldmeqfd	sp, {fp, sp, pc}
	sub	r0, r0, #4
	sub	r3, r3, #4
	mov	r2, #0
.L25:
	add	r2, r2, #1
	ldr	ip, [r0, #4]!
	cmp	r2, r1
	str	ip, [r3, #4]!
	bne	.L25
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Write_V300R001_Qmatrix, .-Write_V300R001_Qmatrix
	.align	2
	.global	H264HAL_V300R001_SetPicMsg
	.type	H264HAL_V300R001_SetPicMsg, %function
H264HAL_V300R001_SetPicMsg:
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
	mov	r4, r0
	mov	r0, r2
	mov	r5, r1
	mov	r6, r3
	bl	MEM_Phy2Vir
	subs	ip, r0, #0
	beq	.L57
	add	r2, r4, #32
	ldrb	r7, [r4]	@ zero_extendqisi2
	ldr	r1, [r4, #44]
	bic	r6, r6, #15
	ldmia	r2, {r2, r3, lr}
	ldr	r0, [r4, #28]
	add	r2, r2, r7, lsl #1
	ldr	r7, .L58
	mov	r3, r3, asl #28
	sub	r0, r0, #1
	add	r7, r7, r2, lsl #2
	ldr	r2, [r4, #20]
	orr	lr, r3, lr, asl #27
	ldr	r3, [r4, #16]
	orr	r1, lr, r1, asl #26
	sub	r2, r2, #1
	ldr	lr, [r7, #60]
	cmp	r3, #0
	orr	r1, r1, r0
	ldrb	r0, [r4, #1]	@ zero_extendqisi2
	orr	r1, r1, r2, asl #16
	movne	r3, #-2147483648
	orr	r2, r1, lr, asl #14
	cmp	r0, #1
	orr	r2, r2, r3
	moveq	r3, #33554432
	movne	r3, #0
	orr	r2, r2, r3
	str	r2, [ip]
	ldr	r3, [r4, #296]
	bic	r3, r3, #15
	str	r3, [ip, #4]
	ldr	r3, [r4, #48]
	str	r3, [ip, #8]
	ldr	r3, [r4, #32]
	cmp	r3, #0
	streq	r3, [ip, #12]
	ldrne	r3, [r4, #52]
	strne	r3, [ip, #12]
	ldrne	r3, [r4, #56]
	str	r3, [ip, #16]
	ldr	r3, [r5, #1136]
	bic	r3, r3, #15
	str	r3, [ip, #20]
	ldr	r3, [r5, #1140]
	bic	r3, r3, #15
	str	r3, [ip, #24]
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #2
	ldreq	r2, [r4, #300]
	ldrne	r3, [r4, #300]
	addeq	r2, r2, #142
	ldreq	r1, [r4, #652]
	addne	r3, r3, #142
	ldreq	r3, [r4, r2, asl #2]
	ldrne	r3, [r4, r3, asl #2]
	addeq	r3, r3, r1
	bic	r3, r3, #15
	str	r3, [ip, #28]
	ldr	r3, [r5, #1144]
	bic	r3, r3, #15
	str	r3, [ip, #32]
	str	r6, [ip, #36]
	ldr	r3, [r4, #860]
	cmp	r3, #0
	beq	.L37
	add	r0, r4, #664
	add	r1, ip, #40
	mov	r2, #0
.L38:
	ldr	r3, [r0, #4]!
	add	r2, r2, #1
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #308]
	bic	r3, r3, #15
	str	r3, [r1], #4
	ldr	r3, [r4, #860]
	cmp	r3, r2
	bhi	.L38
	cmp	r3, #15
	bhi	.L42
.L37:
	add	r1, r3, #10
	add	r1, ip, r1, lsl #2
.L41:
	ldr	r2, [r4, #668]
	add	r3, r3, #1
	cmp	r3, #15
	add	r2, r4, r2, lsl #2
	ldr	r2, [r2, #308]
	bic	r2, r2, #15
	str	r2, [r1], #4
	bls	.L41
.L42:
	ldr	r3, [r5, #1080]
	bic	r3, r3, #15
	str	r3, [ip, #104]
	ldr	r3, [r4, #860]
	cmp	r3, #0
	addne	lr, r4, #728
	addne	r2, ip, #108
	addne	r0, r4, #792
	movne	r1, #0
	beq	.L40
.L43:
	ldr	r3, [lr, #4]!
	add	r1, r1, #1
	add	r2, r2, #8
	str	r3, [r2, #-8]
	ldr	r3, [r0, #4]!
	str	r3, [r2, #-4]
	ldr	r3, [r4, #860]
	cmp	r3, r1
	bhi	.L43
	cmp	r3, #15
	bhi	.L46
.L40:
	add	r2, ip, r3, lsl #3
	mov	r1, #0
	add	r2, r2, #108
.L45:
	add	r3, r3, #1
	str	r1, [r2]
	cmp	r3, #15
	str	r1, [r2, #4]
	add	r2, r2, #8
	bls	.L45
.L46:
	ldr	r1, [r5, #1152]
	add	r2, r4, #56
	add	r3, ip, #252
	add	r0, ip, #476
	bic	r1, r1, #15
	str	r1, [ip, #236]
.L44:
	ldr	r1, [r2, #4]!
	str	r1, [r3, #4]!
	cmp	r3, r0
	bne	.L44
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L57:
	ldr	r1, .L58+4
	ldr	r3, .L58+8
	ldr	r2, .L58+12
	ldr	ip, [r1, #68]
	ldr	r1, .L58+16
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L59:
	.align	2
.L58:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC4
	.word	.LANCHOR0+32
	.word	.LC3
	UNWIND(.fnend)
	.size	H264HAL_V300R001_SetPicMsg, .-H264HAL_V300R001_SetPicMsg
	.global	__aeabi_idiv
	.align	2
	.global	H264HAL_V300R001_SetSliceMsg
	.type	H264HAL_V300R001_SetSliceMsg, %function
H264HAL_V300R001_SetSliceMsg:
	UNWIND(.fnstart)
	@ args = 16, pretend = 0, frame = 8
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
	mov	r6, r0
	mov	r0, r3
	mov	r4, r2
	bl	MEM_Phy2Vir
	subs	r5, r0, #0
	beq	.L207
	ldr	r3, [r4, #8]
	cmp	r3, #0
	bne	.L63
	str	r3, [r5]
	str	r3, [r5, #4]
	str	r3, [r5, #8]
	ldr	r3, [r4, #12]
	cmp	r3, #0
	beq	.L65
.L210:
	ldr	r0, [r4, #28]
	ldr	r3, [r4, #20]
	ldr	r1, [r6, #660]
	bic	r2, r0, #15
	add	r3, r3, r0, lsl #3
	and	r3, r3, #127
	str	r3, [r5, #16]
	ldr	r0, [r4, #12]
	bic	r3, r1, #15
	rsb	r3, r3, r2
	str	r0, [r5, #20]
	str	r3, [r5, #12]
.L66:
	ldr	r0, .L216
	ldr	r2, [r4, #48]
	ldr	r3, [r4, #32]
	ldr	r1, [r4, #36]
	ubfx	r2, r2, #0, #20
	ldrb	r0, [r0]	@ zero_extendqisi2
	and	r1, r1, #3
	orr	r3, r2, r3, asl #26
	cmp	r0, #0
	orr	r3, r3, r1, asl #24
	str	r3, [r5, #24]
	ldrb	r0, [r4, #3]	@ zero_extendqisi2
	bne	.L68
	cmp	r0, #0
	bne	.L68
	ldr	r1, [r6, #24]
	movw	r2, #1620
	ldr	r3, [r6, #28]
	mul	r3, r3, r1
	cmp	r3, r2
	mvnhi	r0, #0
	bhi	.L62
.L68:
	ldr	r1, [r4, #56]
	ldrb	lr, [r4]	@ zero_extendqisi2
	ldr	r2, [r4, #52]
	ldr	r3, [r4, #40]
	mov	r1, r1, asl #8
	ldr	r10, .L216+4
	ldr	ip, [r4, #44]
	and	r3, r3, #31
	add	lr, r10, lr, lsl #2
	orr	r2, r1, r2, asl #2
	and	ip, ip, #31
	orr	r2, r2, r3, asl #21
	ldr	r1, [lr, #124]
	ldrb	lr, [r4, #2]	@ zero_extendqisi2
	orr	r3, r2, ip, asl #16
	orr	r3, r3, r1
	orr	r3, r3, lr, asl #15
	orr	r3, r3, r0, asl #14
	str	r3, [r5, #28]
	ldrb	r3, [r4]	@ zero_extendqisi2
	add	r3, r10, r3, lsl #2
	ldr	r3, [r3, #124]
	cmp	r3, #2
	movne	r3, #0
	movne	r2, r3
	beq	.L208
.L69:
	str	r2, [r5, #32]
	str	r3, [r5, #36]
	ldr	r2, [r4, #60]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	and	r2, r2, #31
	ldr	r1, [r4, #64]
	mov	r2, r2, asl #5
	and	r1, r1, #31
	orr	r3, r2, r3, asl #16
	orr	r3, r3, r1
	str	r3, [r5, #40]
	ldr	r2, [r4, #68]
	ldr	r3, [r4, #72]
	and	r2, r2, #15
	ldr	r1, [r4, #76]
	and	r3, r3, #15
	mov	r2, r2, asl #16
	orr	r3, r2, r3, asl #8
	orr	r3, r3, r1
	str	r3, [r5, #44]
	ldr	r3, [r4, #4056]
	cmp	r3, #0
	beq	.L73
	ldr	r0, [fp, #16]
.L74:
	ldr	r3, [fp, #8]
	str	r0, [r5, #176]
	bic	r3, r3, #15
	str	r3, [r5, #252]
	ldrb	r3, [r4]	@ zero_extendqisi2
	add	r3, r10, r3, lsl #2
	ldr	r3, [r3, #124]
	cmp	r3, #0
	beq	.L136
	ldrb	r8, [r6]	@ zero_extendqisi2
	ldr	r2, [r4, #52]
	cmp	r8, #0
	beq	.L78
	cmp	r2, #0
	beq	.L80
	mov	ip, #0
	mov	r1, #20
	mov	r3, r4
	mov	r7, ip
	mov	r2, ip
	str	r1, [fp, #-48]
	mov	r8, #12
	b	.L111
.L94:
	cmp	r1, #2
	beq	.L102
	and	r1, r2, #3
	and	r0, r2, #7
.L101:
	cmp	r0, #7
	streq	r7, [r5, r8, asl #2]
	addeq	r8, r8, #1
	add	r2, r2, #1
	moveq	r7, #0
	cmp	r1, #3
	add	r3, r3, #36
	ldreq	r1, [fp, #-48]
	streq	ip, [r5, r1, asl #2]
	addeq	r1, r1, #1
	streq	r1, [fp, #-48]
	moveq	ip, #0
	ldr	r1, [r4, #52]
	cmp	r1, r2
	bls	.L209
.L111:
	ldrb	r1, [r3, #1624]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L94
	ldrb	r1, [r3, #1625]	@ zero_extendqisi2
	cmp	r1, #3
	beq	.L95
	ldrb	r9, [r3, #1628]	@ zero_extendqisi2
	and	r0, r2, #7
	ldr	r1, [r6, #32]
	ldrb	lr, [r3, #1629]	@ zero_extendqisi2
	add	r1, r1, r9, lsl #1
.L202:
	cmp	lr, #1
	add	r1, r10, r1, lsl #2
	moveq	lr, #8
	movne	lr, #0
	ldr	r1, [r1, #168]
	cmp	r9, #1
	and	r1, r1, #3
	moveq	r9, #4
	movne	r9, #0
	orr	lr, r9, lr
	orr	lr, lr, r1
	mov	r9, r0, asl #2
	and	r1, r2, #3
	orr	r7, r7, lr, asl r9
	ldr	lr, [r3, #1644]
	add	r9, r1, r1, lsl #2
	mov	lr, lr, asl #1
	and	lr, lr, #31
	orr	ip, ip, lr, asl r9
	b	.L101
.L63:
	ldr	r0, [r4, #24]
	ldr	r3, [r4, #16]
	ldr	r1, [r6, #660]
	bic	r2, r0, #15
	add	r3, r3, r0, lsl #3
	and	r3, r3, #127
	str	r3, [r5, #4]
	ldr	r0, [r4, #8]
	bic	r3, r1, #15
	rsb	r3, r3, r2
	str	r0, [r5, #8]
	str	r3, [r5]
	ldr	r3, [r4, #12]
	cmp	r3, #0
	bne	.L210
.L65:
	str	r3, [r5, #12]
	str	r3, [r5, #16]
	str	r3, [r5, #20]
	b	.L66
.L207:
	ldr	r1, .L216+8
	ldr	r3, .L216+12
	ldr	r2, .L216+16
	ldr	ip, [r1, #68]
	ldr	r1, .L216+20
	blx	ip
	mvn	r0, #0
.L62:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L209:
	tst	r1, #7
	strne	r7, [r5, r8, asl #2]
	ldrne	r3, [fp, #-48]
	strne	ip, [r5, r3, asl #2]
	ldrb	r3, [r4]	@ zero_extendqisi2
	add	r3, r10, r3, lsl #2
	ldr	r3, [r3, #124]
.L80:
	cmp	r3, #2
	beq	.L211
.L88:
	mov	ip, r4
	mov	r3, #0
	ldr	r0, [ip, #3928]!
	mov	r7, #36
	and	r0, r0, #31
	b	.L131
.L133:
	ldr	r1, [ip, #4]!
	cmp	r2, #3
	and	r2, r1, #31
	orr	r0, r0, r2, asl lr
	streq	r0, [r5, r7, asl #2]
	addeq	r7, r7, #1
	moveq	r0, #0
.L131:
	add	r3, r3, #1
	and	r2, r3, #3
	cmp	r3, #32
	add	lr, r2, r2, lsl #2
	bne	.L133
	ldr	r3, [r6, #284]
	cmp	r3, #0
	beq	.L134
	ldrb	r3, [r4]	@ zero_extendqisi2
	add	r3, r10, r3, lsl #2
	ldr	r3, [r3, #124]
	cmp	r3, #1
	beq	.L135
.L134:
	ldr	r3, [r6, #288]
	cmp	r3, #1
	bne	.L136
	ldrb	r3, [r4]	@ zero_extendqisi2
	add	r3, r10, r3, lsl #2
	ldr	r3, [r3, #124]
	cmp	r3, #2
	bne	.L136
.L135:
	mov	r1, #0
	add	r9, r4, #852
	add	r8, r4, #84
	add	r7, r4, #980
	mov	r6, r1
	mov	r0, r4
	str	r4, [fp, #-48]
.L137:
	ldr	r3, [r9, #4]!
	add	lr, r1, #256
	ldr	ip, [r8, #4]!
	add	r6, r6, #1
	ldr	r2, [r4, #80]
	add	r0, r0, #4
	uxtb	r3, r3
	ubfx	ip, ip, #0, #9
	and	r2, r2, #7
	mov	r3, r3, asl #12
	orr	r3, r3, ip, asl #3
	add	ip, r1, #512
	orr	r3, r3, r2
	str	r3, [r5, lr]
	ldr	r3, [r7, #4]!
	add	r2, r1, #768
	ldr	lr, [r0, #212]
	add	r1, r1, #4
	uxtb	r3, r3
	ubfx	lr, lr, #0, #9
	mov	r3, r3, asl #12
	orr	r3, r3, lr, asl #3
	ldr	lr, [r4, #84]
	and	lr, lr, #7
	orr	r3, r3, lr
	str	r3, [r5, ip]
	ldr	r3, [r0, #340]
	ldrb	ip, [r0, #1108]	@ zero_extendqisi2
	ubfx	r3, r3, #0, #9
	orr	r3, r3, ip, asl #9
	str	r3, [r5, r2]
	ldr	r3, [r4, #44]
	cmp	r3, r6
	bcs	.L137
	ldrb	r3, [r4]	@ zero_extendqisi2
	add	r3, r10, r3, lsl #2
	ldr	r3, [r3, #124]
	cmp	r3, #2
	beq	.L212
.L138:
	ldr	r3, .L216+8
	mov	r0, #4
	ldr	r2, [r4, #84]
	ldr	r1, .L216+24
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r2, [r4, #44]
	mov	r3, #0
.L140:
	add	r3, r3, #1
	cmp	r3, r2
	bls	.L140
	ldrb	r3, [r4]	@ zero_extendqisi2
	add	r3, r10, r3, lsl #2
	ldr	r3, [r3, #124]
	cmp	r3, #2
	beq	.L213
.L136:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L102:
	ldrb	r1, [r3, #1625]	@ zero_extendqisi2
	cmp	r1, #3
	beq	.L103
	ldrb	r9, [r3, #1630]	@ zero_extendqisi2
	and	r0, r2, #7
	ldr	r1, [r6, #32]
	ldrb	lr, [r3, #1631]	@ zero_extendqisi2
	add	r1, r1, r9, lsl #1
.L203:
	cmp	lr, #1
	add	r1, r10, r1, lsl #2
	moveq	lr, #8
	movne	lr, #0
	ldr	r1, [r1, #168]
	cmp	r9, #1
	and	r1, r1, #3
	moveq	r9, #4
	movne	r9, #0
	orr	lr, r9, lr
	orr	lr, lr, r1
	mov	r9, r0, asl #2
	and	r1, r2, #3
	orr	r7, r7, lr, asl r9
	ldr	lr, [r3, #1644]
	add	r9, r1, r1, lsl #2
	mov	lr, lr, asl #1
	and	lr, lr, #30
	orr	lr, lr, #1
	orr	ip, ip, lr, asl r9
	b	.L101
.L78:
	cmp	r2, #0
	beq	.L81
	mov	r0, r4
	mov	r9, r8
	mov	lr, r8
	mov	r3, #20
	str	r3, [fp, #-52]
	mov	r3, #12
	str	r3, [fp, #-48]
.L85:
	ldrb	r1, [r0, #1626]	@ zero_extendqisi2
	and	r3, lr, #7
	ldr	r2, [r6, #32]
	and	ip, lr, #3
	add	lr, lr, #1
	add	r0, r0, #36
	add	r1, r2, r1, lsl #1
	ldrb	r2, [r0, #1591]	@ zero_extendqisi2
	add	r1, r10, r1, lsl #2
	cmp	r2, #1
	ldr	r2, [r0, #1608]
	ldr	r1, [r1, #136]
	moveq	r7, #8
	movne	r7, #0
	cmp	r3, #7
	and	r1, r1, #3
	mov	r3, r3, asl #2
	orr	r1, r7, r1
	mov	r2, r2, asl #1
	and	r2, r2, #31
	add	r7, ip, ip, lsl #2
	orr	r9, r9, r1, asl r3
	ldreq	r3, [fp, #-48]
	orr	r8, r8, r2, asl r7
	streq	r9, [r5, r3, asl #2]
	addeq	r3, r3, #1
	moveq	r9, #0
	streq	r3, [fp, #-48]
	cmp	ip, #3
	ldreq	r3, [fp, #-52]
	streq	r8, [r5, r3, asl #2]
	addeq	r3, r3, #1
	streq	r3, [fp, #-52]
	moveq	r8, #0
	ldr	r3, [r4, #52]
	cmp	r3, lr
	bhi	.L85
	tst	r3, #7
	ldrne	r3, [fp, #-48]
	strne	r9, [r5, r3, asl #2]
	ldrne	r3, [fp, #-52]
	strne	r8, [r5, r3, asl #2]
	ldrb	r3, [r4]	@ zero_extendqisi2
	add	r3, r10, r3, lsl #2
	ldr	r3, [r3, #124]
.L81:
	cmp	r3, #2
	bne	.L88
	ldr	r3, [r4, #56]
	cmp	r3, #0
	beq	.L88
	mov	r8, #0
	mov	lr, r4
	mov	r9, r8
	mov	ip, r8
	mov	r3, #28
	str	r3, [fp, #-52]
	mov	r3, #16
	str	r3, [fp, #-48]
.L92:
	ldrb	r1, [lr, #2778]	@ zero_extendqisi2
	and	r3, ip, #7
	ldr	r2, [r6, #32]
	and	r0, ip, #3
	add	ip, ip, #1
	add	lr, lr, #36
	add	r1, r2, r1, lsl #1
	ldrb	r2, [lr, #2743]	@ zero_extendqisi2
	add	r1, r10, r1, lsl #2
	cmp	r2, #1
	ldr	r2, [lr, #2760]
	ldr	r1, [r1, #136]
	moveq	r7, #8
	movne	r7, #0
	cmp	r3, #7
	and	r1, r1, #3
	mov	r3, r3, asl #2
	orr	r1, r7, r1
	mov	r2, r2, asl #1
	and	r2, r2, #31
	add	r7, r0, r0, lsl #2
	orr	r9, r9, r1, asl r3
	ldreq	r3, [fp, #-48]
	orr	r8, r8, r2, asl r7
	streq	r9, [r5, r3, asl #2]
	addeq	r3, r3, #1
	moveq	r9, #0
	streq	r3, [fp, #-48]
	cmp	r0, #3
	ldreq	r3, [fp, #-52]
	streq	r8, [r5, r3, asl #2]
	addeq	r3, r3, #1
	streq	r3, [fp, #-52]
	moveq	r8, #0
	ldr	r3, [r4, #56]
	cmp	r3, ip
	bhi	.L92
	tst	r3, #7
	ldrne	r3, [fp, #-48]
	strne	r9, [r5, r3, asl #2]
	ldrne	r3, [fp, #-52]
	strne	r8, [r5, r3, asl #2]
	b	.L88
.L208:
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L214
	ldr	r3, [r4, #2776]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #768
	beq	.L72
	ldrb	r3, [r4, #2776]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L72
	ldr	r1, [r4, #2788]
	mov	r3, #0
	ldr	r0, [r6, #652]
	add	r1, r1, #142
	ldr	r2, [r6, r1, asl #2]
	add	r2, r2, r0
	bic	r2, r2, #15
	b	.L69
.L95:
	ldrb	r9, [r3, #1626]	@ zero_extendqisi2
	and	r0, r2, #7
	ldr	r1, [r6, #32]
	ldrb	lr, [r3, #1629]	@ zero_extendqisi2
	add	r1, r1, r9, lsl #1
	ldrb	r9, [r3, #1628]	@ zero_extendqisi2
	b	.L202
.L214:
	ldrb	r3, [r4, #2778]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L71
.L72:
	ldr	r2, [r4, #2788]
	mov	r3, #0
	add	r2, r2, #142
	ldr	r2, [r6, r2, asl #2]
	bic	r2, r2, #15
	b	.L69
.L103:
	ldrb	r9, [r3, #1626]	@ zero_extendqisi2
	and	r0, r2, #7
	ldr	r1, [r6, #32]
	ldrb	lr, [r3, #1631]	@ zero_extendqisi2
	add	r1, r1, r9, lsl #1
	ldrb	r9, [r3, #1630]	@ zero_extendqisi2
	b	.L203
.L73:
	ldr	r0, [r6, #664]
	ldrb	r1, [r4, #1]	@ zero_extendqisi2
	cmp	r0, #262144
	addgt	r1, r1, #1
	addle	r1, r1, #1
	movwgt	r0, #65535
	suble	r0, r0, #1
	movtgt	r0, 3
	bl	__aeabi_idiv
	b	.L74
.L211:
	ldr	r3, [r4, #56]
	cmp	r3, #0
	beq	.L88
	mov	ip, #0
	mov	r1, #28
	mov	r2, r4
	mov	r7, ip
	mov	r3, ip
	str	r1, [fp, #-48]
	mov	r8, #16
	b	.L130
.L113:
	cmp	r1, #2
	beq	.L121
	and	r1, r3, #3
	and	r0, r3, #7
.L120:
	cmp	r0, #7
	streq	r7, [r5, r8, asl #2]
	addeq	r8, r8, #1
	add	r3, r3, #1
	moveq	r7, #0
	cmp	r1, #3
	add	r2, r2, #36
	ldreq	r1, [fp, #-48]
	streq	ip, [r5, r1, asl #2]
	addeq	r1, r1, #1
	streq	r1, [fp, #-48]
	moveq	ip, #0
	ldr	r1, [r4, #56]
	cmp	r1, r3
	bls	.L215
.L130:
	ldrb	r1, [r2, #2776]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L113
	ldrb	r1, [r2, #2777]	@ zero_extendqisi2
	cmp	r1, #3
	beq	.L114
	ldrb	r9, [r2, #2780]	@ zero_extendqisi2
	and	r0, r3, #7
	ldr	r1, [r6, #32]
	ldrb	lr, [r2, #2781]	@ zero_extendqisi2
	add	r1, r1, r9, lsl #1
.L205:
	cmp	lr, #1
	add	r1, r10, r1, lsl #2
	moveq	lr, #8
	movne	lr, #0
	ldr	r1, [r1, #168]
	cmp	r9, #1
	and	r1, r1, #3
	moveq	r9, #4
	movne	r9, #0
	orr	lr, r9, lr
	orr	lr, lr, r1
	mov	r9, r0, asl #2
	and	r1, r3, #3
	orr	r7, r7, lr, asl r9
	ldr	lr, [r2, #2796]
	add	r9, r1, r1, lsl #2
	mov	lr, lr, asl #1
	and	lr, lr, #31
	orr	ip, ip, lr, asl r9
	b	.L120
.L121:
	ldrb	r1, [r2, #2777]	@ zero_extendqisi2
	cmp	r1, #3
	beq	.L122
	ldrb	r9, [r2, #2782]	@ zero_extendqisi2
	and	r0, r3, #7
	ldr	r1, [r6, #32]
	ldrb	lr, [r2, #2783]	@ zero_extendqisi2
	add	r1, r1, r9, lsl #1
.L206:
	cmp	lr, #1
	add	r1, r10, r1, lsl #2
	moveq	lr, #8
	movne	lr, #0
	ldr	r1, [r1, #168]
	cmp	r9, #1
	and	r1, r1, #3
	moveq	r9, #4
	movne	r9, #0
	orr	lr, r9, lr
	orr	lr, lr, r1
	mov	r9, r0, asl #2
	and	r1, r3, #3
	orr	r7, r7, lr, asl r9
	ldr	lr, [r2, #2796]
	add	r9, r1, r1, lsl #2
	mov	lr, lr, asl #1
	and	lr, lr, #30
	orr	lr, lr, #1
	orr	ip, ip, lr, asl r9
	b	.L120
.L215:
	tst	r1, #7
	strne	r7, [r5, r8, asl #2]
	ldrne	r3, [fp, #-48]
	strne	ip, [r5, r3, asl #2]
	b	.L88
.L114:
	ldrb	r9, [r2, #2778]	@ zero_extendqisi2
	and	r0, r3, #7
	ldr	r1, [r6, #32]
	ldrb	lr, [r2, #2781]	@ zero_extendqisi2
	add	r1, r1, r9, lsl #1
	ldrb	r9, [r2, #2780]	@ zero_extendqisi2
	b	.L205
.L213:
	ldr	r2, [r4, #40]
	mov	r3, #0
.L141:
	add	r3, r3, #1
	cmp	r3, r2
	bls	.L141
	add	r9, r4, #1232
	add	r8, r4, #1360
	ldr	r1, [fp, #-48]
	mov	r0, #0
	add	r9, r9, #4
	add	r8, r8, #4
	add	r10, r4, #468
	mov	r7, r0
.L142:
	ldr	r3, [r9, #4]!
	add	lr, r0, #384
	ldr	r6, [r10, #4]!
	add	ip, r0, #640
	ldr	r2, [r4, #80]
	add	r7, r7, #1
	uxtb	r3, r3
	ubfx	r6, r6, #0, #9
	and	r2, r2, #7
	add	r1, r1, #4
	mov	r3, r3, asl #12
	orr	r3, r3, r6, asl #3
	orr	r2, r3, r2
	str	r2, [r5, lr]
	ldr	r2, [r8, #4]!
	ldr	r6, [r1, #596]
	ldr	lr, [r4, #84]
	uxtb	r2, r2
	ubfx	r6, r6, #0, #9
	and	lr, lr, #7
	mov	r2, r2, asl #12
	orr	r3, r2, r6, asl #3
	orr	r3, r3, lr
	str	r3, [r5, ip]
	ldrb	r2, [r1, #1492]	@ zero_extendqisi2
	ldr	r3, [r1, #724]
	ubfx	r3, r3, #0, #9
	orr	r3, r3, r2, asl #9
	add	r2, r0, #896
	add	r0, r0, #4
	str	r3, [r5, r2]
	ldr	r3, [r4, #40]
	cmp	r3, r7
	bcs	.L142
	b	.L136
.L122:
	ldrb	r9, [r2, #2778]	@ zero_extendqisi2
	and	r0, r3, #7
	ldr	r1, [r6, #32]
	ldrb	lr, [r2, #2783]	@ zero_extendqisi2
	add	r1, r1, r9, lsl #1
	ldrb	r9, [r2, #2782]	@ zero_extendqisi2
	b	.L206
.L212:
	ldr	r2, [r4, #40]
	mov	r3, #0
.L139:
	add	r3, r3, #1
	cmp	r3, r2
	bls	.L139
	b	.L138
.L71:
	ldr	r2, [r4, #2788]
	ldr	r3, [r6, #652]
	add	r2, r2, #142
	ldr	r2, [r6, r2, asl #2]
	add	r3, r2, r3
	bic	r2, r2, #15
	bic	r3, r3, #15
	b	.L69
.L217:
	.align	2
.L216:
	.word	g_not_direct_8x8_inference_flag
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LANCHOR0+92
	.word	.LC3
	.word	.LC5
	UNWIND(.fnend)
	.size	H264HAL_V300R001_SetSliceMsg, .-H264HAL_V300R001_SetSliceMsg
	.align	2
	.global	H264HAL_V300R001_StartDec
	.type	H264HAL_V300R001_StartDec, %function
H264HAL_V300R001_StartDec:
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
	UNWIND(.pad #68)
	sub	sp, sp, #68
	ldr	r2, .L302
	mov	ip, r1
	mov	r3, r1, asl #6
	sub	r3, r3, ip, asl #3
	str	r1, [fp, #-56]
	add	r3, r2, r3
	ldr	r1, .L302+4
	ldr	r7, .L302+8
	movw	r2, #1208
	ldr	r3, [r3, #8]
	mov	r6, r0
	mla	r2, r2, ip, r7
	ldr	r4, [r1, r3, asl #2]
	ldr	r3, [fp, #-56]
	cmp	r4, #0
	ldrne	r4, [r4, #1232]
	cmp	r3, #1
	str	r2, [fp, #-72]
	bhi	.L289
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	bgt	.L290
	cmp	r0, #0
	beq	.L291
	ldr	r3, [r0, #28]
	cmp	r3, #512
	bhi	.L286
	ldr	r3, [r0, #20]
	cmp	r3, #512
	bhi	.L286
	ldr	r0, [r0, #868]
	cmp	r0, #0
	ldreq	r1, .L302+12
	ldreq	r3, .L302+16
	beq	.L285
	ldr	r3, [fp, #-56]
	movw	r5, #1208
	ldr	r8, .L302+8
	mul	r5, r5, r3
	ldr	r3, [r7, r5]
	cmp	r3, #0
	beq	.L292
.L228:
	ldr	ip, .L302+20
	mov	r0, #3
	ldr	r8, .L302+12
	ldr	r1, .L302+24
	ldr	r2, [ip]
	ldr	r3, [r8, #68]
	add	r2, r2, #1
	str	r2, [ip]
	blx	r3
	ldr	r2, [r6, #664]
	mov	r3, #0
	str	r3, [fp, #-48]
	cmp	r2, #262144
	suble	r2, r2, #1
	ubfxle	r2, r2, #0, #20
	movwgt	r2, #65535
	movtgt	r2, 3
.L230:
	ldr	r3, [fp, #-48]
	movw	r5, #1208
	ldr	r1, [fp, #-56]
	mov	r0, #3
	bfi	r3, r2, #0, #20
	str	r3, [fp, #-48]
	mov	r9, #1
	mov	r2, r3, lsr #24
	mov	r3, r3, lsr #16
	mul	r5, r5, r1
	and	r2, r2, #62
	and	r3, r3, #191
	orr	r2, r2, #65
	bfc	r3, #7, #1
	bfc	r2, #1, #1
	strb	r3, [fp, #-46]
	add	r10, r7, r5
	strb	r2, [fp, #-45]
	ldr	r3, [fp, #-48]
	ldr	r1, .L302+28
	str	r3, [r6, #880]
	mov	r2, r3
	ldr	ip, [r7, r5]
	str	r3, [ip, #8]
	ldr	r3, [r8, #68]
	blx	r3
	mov	r2, #0
	str	r2, [fp, #-48]
	mov	r1, #0
	ldrh	r3, [fp, #-46]
	bfi	r1, r2, #0, #4
	ldr	lr, [r6, #656]
	mov	r0, #3
	bfi	r3, r9, #0, #12
	strh	r3, [fp, #-46]	@ movhi
	strb	r1, [fp, #-48]
	mov	r3, r3, lsr #8
	ldrh	r1, [fp, #-48]
	bfi	r3, r4, #4, #1
	mov	lr, lr, lsr #6
	bfi	r1, lr, #4, #10
	ldr	ip, [r6, #1128]
	uxtb	r3, r3
	strh	r1, [fp, #-48]	@ movhi
	orr	r3, r3, #32
	strb	r3, [fp, #-45]
	uxtb	lr, r3
	mov	r3, r1, lsr #8
	ldrb	r1, [r6, #2]	@ zero_extendqisi2
	bfi	lr, ip, #6, #1
	bfi	lr, r2, #7, #1
	strb	lr, [fp, #-45]
	bfi	r3, r1, #6, #1
	ldr	r1, .L302+32
	bfi	r3, r2, #7, #1
	strb	r3, [fp, #-47]
	ldr	r3, [fp, #-48]
	str	r3, [r6, #884]
	mov	r2, r3
	ldr	ip, [r7, r5]
	str	r3, [ip, #12]
	ldr	r3, [r8, #68]
	blx	r3
	ldr	r3, [r10, #48]
	ldr	r1, .L302+36
	mov	r0, #3
	bic	r3, r3, #15
	str	r3, [r6, #888]
	ldr	ip, [r7, r5]
	mov	r2, r3
	str	r3, [ip, #16]
	ldr	r3, [r8, #68]
	blx	r3
	ldr	r3, [r10, #32]
	ldr	r1, .L302+40
	mov	r0, #3
	bic	r3, r3, #15
	str	r3, [r6, #892]
	ldr	ip, [r7, r5]
	mov	r2, r3
	str	r3, [ip, #20]
	ldr	r3, [r8, #68]
	blx	r3
	ldr	r3, [r6, #660]
	ldr	r1, .L302+44
	mov	r0, #3
	bic	r3, r3, #15
	str	r3, [r6, #896]
	ldr	ip, [r7, r5]
	mov	r2, r3
	str	r3, [ip, #24]
	ldr	r3, [r8, #68]
	blx	r3
	ldr	r3, [r6, #28]
	cmp	r3, #256
	movwhi	r1, #30480
	bhi	.L231
	cmp	r3, #119
	movwls	r1, #30480
	movtls	r1, 1
	bls	.L231
	ldr	r1, [r6, #1132]
	movw	r3, #30480
	mov	r2, r3
	bfi	r2, r9, #16, #16
	cmp	r1, #0
	movne	r1, r3
	moveq	r1, r2
.L231:
	ldr	r3, .L302+48
	ldr	r0, [r3]
	add	r0, r0, #4
	bl	MEM_WritePhyWord
	ldr	r3, [fp, #-56]
	movw	r2, #1208
	cmp	r4, #1
	ldr	ip, .L302+8
	mul	r2, r2, r3
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r6, #904]
	str	r3, [r6, #908]
	str	r3, [r6, #912]
	str	r3, [r6, #916]
	str	r3, [r6, #920]
	str	r3, [r6, #924]
	str	r3, [r6, #928]
	ldr	r1, [r7, r2]
	str	r3, [fp, #-48]
	str	r3, [r1, #60]
	ldr	r1, [r7, r2]
	str	r3, [r1, #64]
	ldr	r1, [r7, r2]
	str	r3, [r1, #68]
	ldr	r1, [r7, r2]
	str	r3, [r1, #72]
	ldr	r1, [r7, r2]
	str	r3, [r1, #76]
	ldr	r1, [r7, r2]
	str	r3, [r1, #80]
	ldr	r1, [r7, r2]
	str	r3, [r1, #84]
	beq	.L293
.L254:
	ldr	r1, [fp, #-56]
	movw	r9, #1208
	ldr	r3, [r6, #292]
	mov	r0, #3
	mul	r9, r9, r1
	add	r3, r6, r3, lsl #2
	ldr	r1, .L302+52
	ldr	r3, [r3, #308]
	bic	r3, r3, #15
	str	r3, [r6, #932]
	ldr	ip, [r7, r9]
	mov	r2, r3
	str	r3, [ip, #96]
	ldr	r3, [r8, #68]
	blx	r3
	ldr	r3, [r6, #656]
	ldr	r5, [r6, #24]
	mov	r0, #3
	ldr	r1, .L302+56
	str	r3, [r6, #936]
	mov	r2, r3
	ldr	ip, [r7, r9]
	str	r3, [ip, #100]
	ldr	r3, [r8, #68]
	blx	r3
	ldr	r3, [r6, #28]
	mov	r3, r3, asl #4
	sub	r2, r3, #1
	cmp	r2, #2048
	movcc	r10, #16
	bcs	.L294
.L232:
	ldr	r3, [r6, #24]
	add	r2, r5, #1
	ldr	lr, [r6, #656]
	movw	ip, #1208
	mov	r2, r2, lsr #1
	ldr	r1, .L302+60
	mov	r3, r3, asl #4
	mov	r0, #3
	add	r3, r3, #31
	mul	r2, lr, r2
	mov	r3, r3, lsr #5
	ldr	lr, [fp, #-56]
	mov	r3, r3, asl #4
	mul	r9, ip, lr
	mla	r3, r10, r3, r2
	add	r5, r7, r9
	mov	r3, r3, asl #1
	str	r3, [r6, #940]
	ldr	lr, [r7, r9]
	mov	r2, r3
	str	r3, [lr, #104]
	ldr	r3, [r8, #68]
	blx	r3
	ldr	r3, [r6, #24]
	cmp	r4, #1
	mov	r3, r3, asl #4
	add	r3, r3, #31
	bic	r3, r3, #31
	mul	r10, r10, r3
	str	r10, [r6, #944]
	ldr	r3, [r7, r9]
	str	r10, [fp, #-48]
	str	r10, [r3, #108]
	beq	.L233
	ldr	r3, [r5, #1156]
	bic	r3, r3, #15
	str	r3, [r6, #956]
.L256:
	ldr	r1, [fp, #-56]
	movw	r5, #1208
	mov	r2, r3
	mov	r0, #3
	mov	r4, #0
	mul	r5, r5, r1
	ldr	r1, .L302+64
	add	r9, r7, r5
	ldr	ip, [r7, r5]
	str	r3, [ip, #128]
	ldr	r3, [r8, #68]
	blx	r3
	ldr	r3, [r9, #1160]
	str	r4, [fp, #-48]
	mov	r0, #3
	ldr	r1, .L302+68
	strh	r3, [fp, #-48]	@ movhi
	ldr	r3, [fp, #-48]
	str	r3, [r6, #960]
	mov	r2, r3
	ldr	ip, [r7, r5]
	str	r3, [ip, #132]
	ldr	r3, [r8, #68]
	blx	r3
	add	r2, r6, #996
	mov	r3, r4
	mov	ip, r4
.L234:
	ldr	r1, [r2, #4]!
	and	r1, r1, #3
	orr	ip, ip, r1, asl r3
	add	r3, r3, #2
	cmp	r3, #32
	bne	.L234
	ldr	r1, [fp, #-56]
	movw	r3, #1208
	mov	r2, ip
	mov	r0, #3
	str	ip, [fp, #-48]
	movw	r4, #1208
	mul	r3, r3, r1
	ldr	r1, .L302+72
	mvn	r5, #0
	ldr	r3, [r7, r3]
	str	ip, [r3, #148]
	ldr	r3, [r8, #68]
	blx	r3
	ldrb	r1, [r6]	@ zero_extendqisi2
	ldr	r3, [r6, #32]
	mov	r0, #3
	ldr	r2, .L302+76
	add	r3, r3, r1, lsl #1
	ldr	r1, [fp, #-56]
	add	r3, r2, r3, lsl #2
	mul	r4, r4, r1
	ldr	r3, [r3, #60]
	ldr	r1, .L302+80
	cmp	r3, #0
	add	r10, r7, r4
	moveq	r3, #2
	movne	r3, #0
	ldr	ip, [r7, r4]
	mov	r2, r3
	str	r3, [fp, #-48]
	str	r3, [ip, #152]
	ldr	r3, [r8, #68]
	blx	r3
	ldr	r3, [r7, r4]
	ldr	r1, [fp, #-72]
	mov	r0, r6
	str	r5, [r3, #32]
	ldr	r3, [r10, #52]
	ldr	r2, [r10, #48]
	bl	H264HAL_V300R001_SetPicMsg
	ldr	r4, [r6, #868]
	cmp	r4, #0
	beq	.L295
	ldr	r9, [r4, #48]
	cmp	r9, #0
	streq	r9, [fp, #-76]
	moveq	r10, r4
	bne	.L296
.L237:
	ldr	r1, [r6, #864]
	cmp	r1, #0
	ble	.L239
	ldr	ip, [fp, #-56]
	movw	r2, #302
	ldr	r0, [fp, #-76]
	mul	r9, r2, ip
	mov	r2, #0
	mov	ip, r2
	add	r3, r0, #4
	add	r2, r0, #5
	mov	r4, ip
	str	r2, [fp, #-60]
	mov	r2, r10
	mov	r10, ip
	add	r3, r9, r3
	str	r9, [fp, #-64]
	str	r3, [fp, #-68]
.L252:
	ldr	lr, [r2, #48]
	ldr	r3, [fp, #-60]
	cmp	lr, r10
	ldr	r0, [fp, #-68]
	add	r9, r3, r4
	ldr	r3, [fp, #-64]
	movhi	ip, #0
	movls	ip, #1
	cmp	r4, #0
	add	r3, r3, r9
	add	r0, r0, r4
	add	r3, r3, #8
	movle	ip, #0
	add	r0, r0, #8
	cmp	ip, #0
	ldr	r3, [r7, r3, asl #2]
	ldr	r0, [r7, r0, asl #2]
	beq	.L297
.L240:
	ldr	r3, [r6, #864]
	add	r4, r4, #1
	cmp	r3, r4
	mov	r1, r3
	bgt	.L252
.L251:
	cmp	r3, #0
	ble	.L239
	ldr	r2, [fp, #-56]
	movw	r9, #302
	ldr	r1, [fp, #-76]
	mla	r9, r9, r2, r1
	add	r3, r9, r3
	add	r3, r3, #12
	ldr	r0, [r7, r3, asl #2]
	bl	H264HAL_V300R001_CutSliceChain
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L294:
	sub	r2, r3, #2048
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r10, #32
	bcc	.L232
	sub	r2, r3, #4096
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r10, #48
	bcc	.L232
	sub	r3, r3, #6144
	sub	r3, r3, #1
	cmp	r3, #2048
	movcs	r10, #16
	movcc	r10, #64
	b	.L232
.L297:
	add	ip, r4, #1
	cmp	ip, r1
	bge	.L269
	ldr	r5, [r2, #4056]
	cmp	r5, #0
	bne	.L284
	b	.L242
.L244:
	add	ip, ip, #1
	cmp	ip, r1
	beq	.L259
	ldr	r5, [r5, #4056]
	cmp	r5, #0
	beq	.L242
.L284:
	ldr	r4, [r5, #48]
	cmp	lr, r4
	bcs	.L244
.L241:
	cmp	r9, #140
	cmpne	ip, r1
	beq	.L298
	ldr	r1, [fp, #-64]
	ldr	r4, [fp, #-60]
	add	r4, r1, r4
	ldr	r1, [r5, #48]
	add	r4, r4, ip
	add	r4, r4, #8
	sub	r1, r1, #1
	ldr	r9, [r7, r4, asl #2]
	mov	r10, r9
.L248:
	sub	r4, ip, #1
	ldr	ip, [r6, #864]
	sub	ip, ip, #1
	cmp	ip, r4
	moveq	ip, #0
	streq	ip, [r2, #4056]
	str	r1, [sp, #12]
	stmib	sp, {r10, lr}
	mov	r10, lr
	str	r0, [sp]
	mov	r0, r6
	ldr	r1, [fp, #-72]
	bl	H264HAL_V300R001_SetSliceMsg
	cmp	r0, #0
	bne	.L299
	cmp	r9, #0
	beq	.L300
	mov	r2, r5
	b	.L240
.L242:
	ldr	r3, [r8, #68]
	movw	r2, #1386
	ldr	r1, .L302+84
	mov	r0, #1
	blx	r3
	mvn	r0, #0
.L280:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L296:
	ldr	r3, [r4, #8]
	mov	r1, #0
	mov	ip, #1
	str	r1, [r4, #48]
	str	ip, [r4, #8]
	sub	lr, r9, #1
	str	r3, [fp, #-60]
	mov	r2, r4
	ldr	r3, [r4, #12]
	str	r1, [r4, #12]
	str	ip, [fp, #-76]
	str	r3, [fp, #-64]
	ldr	r3, [r4, #16]
	str	r1, [r4, #16]
	str	r3, [fp, #-68]
	ldr	r3, [r4, #20]
	str	r1, [r4, #20]
	str	r3, [fp, #-80]
	ldr	r3, [r4, #24]
	str	r1, [r4, #24]
	str	r3, [fp, #-84]
	ldr	r3, [r4, #28]
	str	r1, [r4, #28]
	ldr	r0, [r10, #56]
	str	r3, [fp, #-88]
	ldr	r3, [r10, #52]
	ldr	r10, [r10, #48]
	stmib	sp, {r0, r1}
	mov	r0, r6
	ldr	r1, [fp, #-72]
	str	r10, [sp]
	str	lr, [sp, #12]
	bl	H264HAL_V300R001_SetSliceMsg
	ldr	ip, [fp, #-76]
	cmp	r0, #0
	bne	.L301
	ldr	r3, [fp, #-60]
	str	r9, [r4, #48]
	str	ip, [fp, #-76]
	str	r3, [r4, #8]
	ldr	r3, [fp, #-64]
	str	r3, [r4, #12]
	ldr	r3, [fp, #-68]
	str	r3, [r4, #16]
	ldr	r3, [fp, #-80]
	str	r3, [r4, #20]
	ldr	r3, [fp, #-84]
	str	r3, [r4, #24]
	ldr	r3, [fp, #-88]
	str	r3, [r4, #28]
	ldr	r10, [r6, #868]
	b	.L237
.L298:
	mov	r1, ip
.L259:
	ldr	ip, [r6, #20]
	mov	r10, #0
	ldr	r4, [r6, #28]
	mov	r9, r10
	str	r10, [r2, #4056]
	str	ip, [fp, #-80]
	mov	ip, r1
	ldr	r1, [fp, #-80]
	mul	r1, r4, r1
	sub	r1, r1, #1
	b	.L248
.L292:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L229
	str	r3, [r8, r5]
	b	.L228
.L269:
	mov	r5, r2
	b	.L241
.L290:
	ldr	r1, .L302+12
	mov	r0, #0
	ldr	r2, .L302+88
	str	r0, [sp]
	ldr	ip, [r1, #68]
	ldr	r1, .L302+92
	blx	ip
	mvn	r0, #0
	b	.L280
.L286:
	ldr	r1, .L302+12
	mov	r0, #0
	ldr	r3, .L302+96
.L285:
	ldr	ip, [r1, #68]
	ldr	r2, .L302+88
	ldr	r1, .L302+100
	blx	ip
	mvn	r0, #0
	b	.L280
.L300:
	ldr	r3, [r6, #864]
	b	.L251
.L299:
	ldr	r3, [r8, #68]
	movw	r2, #1528
	ldr	r1, .L302+104
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	b	.L280
.L239:
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L302+108
	blx	r3
	mvn	r0, #0
	b	.L280
.L289:
	ldr	r3, .L302+12
	mov	r0, #0
	ldr	r1, .L302+112
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L280
.L301:
	ldr	r3, .L302+12
	mov	r0, ip
	movw	r2, #1335
	ldr	r1, .L302+104
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	b	.L280
.L295:
	ldr	r3, .L302+12
	mov	r0, r4
	ldr	r1, .L302+116
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	b	.L280
.L291:
	ldr	r1, .L302+12
	ldr	r3, .L302+120
	b	.L285
.L229:
	ldr	r3, .L302+12
	ldr	r1, .L302+124
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L280
.L293:
	ldr	lr, .L302+128
	mov	r0, #3
	ldr	r3, [ip, r2]
	ldr	r1, .L302+132
	ldr	r2, [lr]
	str	r2, [r3, #92]
	ldr	r3, [r8, #68]
	blx	r3
	b	.L254
.L233:
	ldr	r3, [r6, #292]
	mov	r0, #3
	ldr	r2, .L302+8
	ldr	r1, .L302+136
	add	r3, r6, r3, lsl #2
	ldr	ip, [r2, r9]
	ldr	r2, [r3, #436]
	ldr	r3, .L302+12
	str	r2, [ip, #112]
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [r5, #1156]
	bic	r3, r3, #15
	str	r3, [r6, #956]
	b	.L256
.L303:
	.align	2
.L302:
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC10
	.word	.LANCHOR1
	.word	.LC12
	.word	.LC23
	.word	.LC24
	.word	.LC25
	.word	.LC26
	.word	.LC27
	.word	s_RegPhyBaseAddr
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LANCHOR0
	.word	.LC30
	.word	.LC21
	.word	.LANCHOR0+200
	.word	.LC7
	.word	.LC9
	.word	.LC3
	.word	.LC20
	.word	.LC22
	.word	.LC6
	.word	.LC19
	.word	.LC8
	.word	.LC11
	.word	g_TunnelLineNumber
	.word	.LC28
	.word	.LC29
	UNWIND(.fnend)
	.size	H264HAL_V300R001_StartDec, .-H264HAL_V300R001_StartDec
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.13865, %object
	.size	__func__.13865, 31
__func__.13865:
	.ascii	"H264HAL_V300R001_CutSliceChain\000"
	.space	1
	.type	__func__.13888, %object
	.size	__func__.13888, 27
__func__.13888:
	.ascii	"H264HAL_V300R001_SetPicMsg\000"
	.space	1
	.type	g_StructTrans, %object
	.size	g_StructTrans, 32
g_StructTrans:
	.word	0
	.word	3
	.word	1
	.word	1
	.word	2
	.word	2
	.word	3
	.word	3
	.type	__func__.13926, %object
	.size	__func__.13926, 29
__func__.13926:
	.ascii	"H264HAL_V300R001_SetSliceMsg\000"
	.space	3
	.type	g_SliceTypeForPMV, %object
	.size	g_SliceTypeForPMV, 12
g_SliceTypeForPMV:
	.word	1
	.word	2
	.word	0
	.type	g_ListStructFrame, %object
	.size	g_ListStructFrame, 32
g_ListStructFrame:
	.word	0
	.word	1
	.word	3
	.word	3
	.word	3
	.word	3
	.word	2
	.word	2
	.type	g_ListStructField, %object
	.size	g_ListStructField, 32
g_ListStructField:
	.word	0
	.word	1
	.word	2
	.word	2
	.word	2
	.word	2
	.word	2
	.word	2
	.type	__func__.14001, %object
	.size	__func__.14001, 26
__func__.14001:
	.ascii	"H264HAL_V300R001_StartDec\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"line: %d, p = NULL is not expected value!\012\000" )
	.space	1
.LC1:
	ASCII(.ascii	"H264HAL_V300R001_InitHal return error.\012\000" )
.LC2:
	ASCII(.ascii	"can not map slice msg virtual address!\012\000" )
.LC3:
	ASCII(.ascii	"%s: %s\012\000" )
.LC4:
	ASCII(.ascii	"can not map down msg virtual address!\012\000" )
	.space	1
.LC5:
	ASCII(.ascii	"chroma_lg = %d\012\000" )
.LC6:
	ASCII(.ascii	"VdhId is wrong! H264HAL_V200R003_StartDec\012\000" )
	.space	1
.LC7:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC8:
	ASCII(.ascii	"point of picture para null\012\000" )
.LC9:
	ASCII(.ascii	"picture width out of range\012\000" )
.LC10:
	ASCII(.ascii	"point of fst slice para null\012\000" )
	.space	2
.LC11:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
.LC12:
	ASCII(.ascii	"\012\012***********************H264HAL_V200R003_Sta" )
	ASCII(.ascii	"rtDec is Now %d***********************\012\000" )
	.space	3
.LC13:
	ASCII(.ascii	"YSTADDR_V300R001_1D = 0x%x\012\000" )
.LC14:
	ASCII(.ascii	"YSTRIDE_V300R001_1D = 0x%x\012\000" )
.LC15:
	ASCII(.ascii	"UVOFFSET_V300R001_1D = 0x%x\012\000" )
	.space	3
.LC16:
	ASCII(.ascii	"PPFD_V300R001_BUF_ADDR = 0x%x\012\000" )
	.space	1
.LC17:
	ASCII(.ascii	"PPFD_V300R001_BUF_LEN = 0x%x\012\000" )
	.space	2
.LC18:
	ASCII(.ascii	"REF_V300R001_PIC_TYPE = 0x%x\012\000" )
	.space	2
.LC19:
	ASCII(.ascii	"pSlicePara = NULL!\012\000" )
.LC20:
	ASCII(.ascii	"line %d Set slice msg not wrong!\012\000" )
	.space	2
.LC21:
	ASCII(.ascii	"line %d tmp_pSlicePara is NULL!\012\000" )
	.space	3
.LC22:
	ASCII(.ascii	"!!! no slice to dec, add up msg report.\000" )
.LC23:
	ASCII(.ascii	"BASIC_V200R003_CFG0 = 0x%x\012\000" )
.LC24:
	ASCII(.ascii	"BASIC_V300R001_CFG1 = 0x%x\012\000" )
.LC25:
	ASCII(.ascii	"AVM_V300R001_ADDR = 0x%x\012\000" )
	.space	2
.LC26:
	ASCII(.ascii	"VAM_V300R001_ADDR = 0x%x\012\000" )
	.space	2
.LC27:
	ASCII(.ascii	"STREAM_V200R003_BASE_ADDR = 0x%x\012\000" )
	.space	2
.LC28:
	ASCII(.ascii	"VREG_V300R001_PART_DEC_OVER_INT_LEVEL=0x%x\012\000" )
.LC29:
	ASCII(.ascii	"VREG_LINE_NUM_STADDR = 0x%x\012\000" )
	.space	3
.LC30:
	ASCII(.ascii	"FF_V300R001_APT_EN = 0x%x\012\000" )
	.bss
	.align	2
.LANCHOR1 = . + 0
	.type	StartCnt.13992, %object
	.size	StartCnt.13992, 4
StartCnt.13992:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
