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
	.file	"postprocess.c"
	.text
	.align	2
	.global	PostPro_GetRepairStratage
	.type	PostPro_GetRepairStratage, %function
PostPro_GetRepairStratage:
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
	bics	r3, r0, #16
	ldreqb	r0, [r1, #3]	@ zero_extendqisi2
	clzeq	r0, r0
	moveq	r0, r0, lsr #5
	movne	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	PostPro_GetRepairStratage, .-PostPro_GetRepairStratage
	.align	2
	.global	PostPro_CopyYuv
	.type	PostPro_CopyYuv, %function
PostPro_CopyYuv:
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
	ldr	r2, .L8
	mov	ip, #1744
	add	lr, r0, #12288
	ldr	r3, .L8+4
	mov	r4, #1
	mla	ip, ip, r1, r2
	ldr	r6, [lr, #2304]
	ldr	r9, [lr, #2316]
	mov	r2, #0
	ldr	r8, [lr, #2372]
	ldr	r5, [r3, #104]
	add	r7, ip, #32
	str	r9, [ip, #12]
	cmp	r5, r2
	str	r6, [ip, #4]
	str	r6, [ip, #8]
	ldr	lr, [r0, #220]
	str	lr, [ip, #16]
	ldr	lr, [r0, #228]
	str	lr, [ip, #20]
	ldrh	lr, [r0, #164]
	str	lr, [ip, #24]
	ldrh	r6, [r0, #166]
	str	r8, [ip, #848]
	str	r2, [ip, #36]
	smulbb	lr, r6, lr
	str	r6, [ip, #28]
	strh	r2, [ip, #40]	@ movhi
	str	r2, [ip, #852]
	sub	lr, lr, #1
	str	r4, [ip, #32]
	strh	lr, [ip, #42]	@ movhi
	str	r4, [ip, #840]
	str	r4, [ip, #856]
	beq	.L6
	mov	r3, r1
	mov	r1, r0
	mov	r0, #2
	blx	r5
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L6:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L9:
	.align	2
.L8:
	.word	g_RepairParam
	.word	g_vdm_hal_fun_ptr
	UNWIND(.fnend)
	.size	PostPro_CopyYuv, .-PostPro_CopyYuv
	.align	2
	.global	PostPro_CopyYuv_Vc1
	.type	PostPro_CopyYuv_Vc1, %function
PostPro_CopyYuv_Vc1:
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
	ldrh	r3, [r0, #74]
	mov	r8, r0
	mov	r10, r1
	sub	r2, r3, #1
	cmp	r2, #2048
	movcc	r3, #16
	strcc	r3, [fp, #-48]
	bcc	.L11
	sub	r2, r3, #2048
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r3, #32
	strcc	r3, [fp, #-48]
	bcs	.L18
.L11:
	ldr	r3, .L19
	mov	r4, #1744
	add	r5, r8, #12288
	mov	r6, #1
	ldr	ip, .L19+4
	mov	r7, #0
	mla	r4, r4, r10, r3
	ldr	lr, [r5, #100]
	ldr	r3, [r5, #248]
	mov	r0, #22
	ldr	r1, [r5, #212]
	ldr	r2, [r5, #96]
	ldr	ip, [ip, #68]
	str	lr, [r4, #16]
	ldr	lr, [r5, #108]
	stmib	r4, {r1, r2}
	mov	r2, r3
	str	r3, [r4, #24]
	str	lr, [r4, #20]
	ldrh	lr, [r8, #76]
	ldr	r1, .L19+8
	adds	r3, lr, #15
	str	r7, [r4, #36]
	addmi	r3, lr, #30
	str	r6, [r4, #32]
	mov	r3, r3, asr #4
	add	r3, r3, r6
	add	r3, r3, r3, lsr #31
	bic	r3, r3, #1
	str	r3, [r4, #28]
	blx	ip
	ldr	lr, [r4, #28]
	ldr	r3, [r4, #24]
	ldr	ip, .L19+12
	ldr	r0, [r5, #244]
	smulbb	r3, lr, r3
	ldr	ip, [ip, #104]
	ldr	lr, [fp, #-48]
	cmp	ip, r7
	str	r0, [r4, #848]
	sub	r3, r3, #1
	str	r6, [r4, #840]
	strh	r7, [r4, #40]	@ movhi
	moveq	r0, r6
	str	r6, [r4, #856]
	str	lr, [r4, #860]
	strh	r3, [r4, #42]	@ movhi
	str	r7, [r4, #844]
	str	r7, [r4, #852]
	beq	.L12
	mov	r3, r10
	mov	r2, r7
	mov	r1, r8
	mov	r0, r6
	blx	ip
	subs	r0, r0, r7
	movne	r0, #1
.L12:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L18:
	sub	r2, r3, #4096
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r3, #48
	strcc	r3, [fp, #-48]
	bcc	.L11
	sub	r3, r3, #6144
	sub	r3, r3, #1
	cmp	r3, #2048
	movcs	r3, #16
	movcc	r3, #64
	str	r3, [fp, #-48]
	b	.L11
.L20:
	.align	2
.L19:
	.word	g_RepairParam
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	g_vdm_hal_fun_ptr
	UNWIND(.fnend)
	.size	PostPro_CopyYuv_Vc1, .-PostPro_CopyYuv_Vc1
	.align	2
	.global	PostPro_CopyYuv_Vp6
	.type	PostPro_CopyYuv_Vp6, %function
PostPro_CopyYuv_Vp6:
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
	ldr	r2, .L24
	mov	r4, #1744
	ldr	r3, [r0, #172]
	mov	r5, r0
	mov	r7, #0
	mov	r6, #1
	mla	r4, r4, r1, r2
	ldr	r2, .L24+4
	mov	r8, r1
	mov	r0, #22
	ldr	r1, .L24+8
	ldr	ip, [r2, #68]
	add	r9, r4, #24
	str	r3, [r4, #4]
	ldr	r3, [r5, #172]
	str	r3, [r4, #8]
	ldr	r3, [r5, #180]
	str	r3, [r4, #16]
	ldr	r3, [r5, #184]
	str	r3, [r4, #20]
	ldrh	r2, [r5, #52]
	str	r2, [r4, #24]
	ldrh	r3, [r5, #54]
	str	r7, [r4, #36]
	str	r6, [r4, #32]
	str	r3, [r4, #28]
	blx	ip
	ldr	ip, [r4, #28]
	ldr	r0, [r4, #24]
	ldr	r1, .L24+12
	str	r6, [r4, #840]
	smulbb	r0, ip, r0
	ldr	ip, [r1, #104]
	strh	r7, [r4, #40]	@ movhi
	str	r7, [r4, #844]
	cmp	ip, r7
	sub	r0, r0, #1
	strh	r0, [r4, #42]	@ movhi
	ldr	r3, [r5, #192]
	str	r6, [r4, #856]
	str	r7, [r4, #852]
	str	r3, [r4, #848]
	beq	.L23
	mov	r3, r8
	mov	r2, r7
	mov	r1, r5
	mov	r0, #10
	blx	ip
	subs	r0, r0, r7
	movne	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L23:
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L25:
	.align	2
.L24:
	.word	g_RepairParam
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	g_vdm_hal_fun_ptr
	UNWIND(.fnend)
	.size	PostPro_CopyYuv_Vp6, .-PostPro_CopyYuv_Vp6
	.align	2
	.global	PostPro_GetHevcMbGroup
	.type	PostPro_GetHevcMbGroup, %function
PostPro_GetHevcMbGroup:
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
	mov	r7, r1
	ldr	r1, [r1, #8]
	mov	r8, r0
	str	r2, [fp, #-48]
	sub	r2, r1, #1
	cmp	r2, #199
	bhi	.L73
	ldr	lr, [r0, #868]
	mov	r5, #0
	ldr	r2, [r0, #24]
	ldr	r4, [r0, #28]
	cmp	lr, r5
	str	r5, [r0, #840]
	str	r5, [r0, #856]
	mul	r4, r4, r2
	bne	.L74
	ldr	r3, [r7, #8]
	mvn	r6, #0
	uxth	r10, r4
	add	r2, r4, r6
	mov	r1, r6
	str	r2, [fp, #-48]
	add	r0, r7, r3, lsl #2
	mov	r2, r7
	mov	r3, r0
	strh	r10, [r0, #14]	@ movhi
	strh	r10, [r0, #12]	@ movhi
	b	.L60
.L77:
	cmp	r0, r6
	movlt	r6, r1
	add	r3, r6, #1
	cmp	r0, r3
	ble	.L55
	ldr	r3, [r8, #840]
	cmp	r3, #199
	bgt	.L75
	add	r5, r1, r1, lsr #31
	sub	r1, r6, #100
	add	r6, r8, r3, lsl #2
	add	r3, r3, #1
	sxth	r9, r5
	strh	r5, [r6, #40]	@ movhi
	cmp	r1, r9
	sub	r5, r0, #1
	strgth	r1, [r6, #40]	@ movhi
	strh	r5, [r6, #42]	@ movhi
	str	r3, [r8, #840]
.L55:
	ldr	r3, [r7, #8]
	sub	r1, r3, #1
	cmp	r1, lr
	beq	.L76
.L64:
	add	lr, lr, #1
	mov	r6, ip
	cmp	r3, lr
	mov	r1, r0
	add	r2, r2, #4
	bcc	.L61
.L60:
	ldrh	r0, [r2, #12]
	ldrh	ip, [r2, #14]
	cmp	r0, r4
	cmple	ip, r4
	movgt	r3, #1
	movle	r3, #0
	cmp	r0, ip
	orrgt	r3, r3, #1
	cmp	r3, #0
	beq	.L77
	ldr	r3, [r7, #8]
	add	lr, lr, #1
	add	r2, r2, #4
	cmp	r3, lr
	bcs	.L60
.L61:
	mov	r0, #0
.L28:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L76:
	add	r1, r7, lr, lsl #2
	ldr	r5, [fp, #-48]
	ldrh	r1, [r1, #14]
	cmp	r1, r5
	bge	.L64
	ldr	r3, [r8, #840]
	cmp	r3, #199
	bgt	.L78
	add	r2, r8, r3, lsl #2
	sub	r10, r10, #1
	add	r3, r3, #1
	mov	r0, #0
	strh	r10, [r2, #42]	@ movhi
	strh	r1, [r2, #40]	@ movhi
	str	r3, [r8, #840]
	b	.L28
.L74:
	mov	r1, r3, asl #6
	ldr	r2, .L81
	sub	r3, r1, r3, asl #3
	ldr	r1, .L81+4
	add	r3, r2, r3
	ldr	r3, [r3, #8]
	ldr	r0, [r1, r3, asl #2]
	add	r0, r0, #229376
	add	r0, r0, #760
	bl	HEVC_CalcTileMap
	ldr	r3, [r7, #8]
	cmp	r3, r5
	beq	.L36
	mov	ip, r5
	ldr	r6, .L81+8
	ldr	r5, .L81+12
	mov	r1, r7
	mov	lr, #1
.L35:
	ldrh	r0, [r1, #12]
	ldrh	r2, [r1, #14]
	cmp	r0, r2
	mov	r3, r0
	bls	.L34
	b	.L33
.L79:
	ldrh	r0, [r1, #12]
.L34:
	cmp	r4, r0
	blt	.L37
	cmp	r4, r2
	ldrge	r2, [r5, r3, asl #2]
	strgeb	lr, [r6, r2]
.L37:
	ldrh	r2, [r1, #14]
	add	r3, r3, #1
	cmp	r2, r3
	bcs	.L79
.L33:
	ldr	r3, [r7, #8]
	add	ip, ip, #1
	add	r1, r1, #4
	cmp	r3, ip
	bhi	.L35
.L36:
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #2416]
	cmp	r3, #0
	beq	.L32
	ldr	r6, [fp, #-48]
	mov	r10, #0
	ldr	ip, .L81+8
	mov	r9, r10
	str	r4, [fp, #-48]
	mov	r7, r6
.L39:
	ldr	r0, [r7, #3532]
	ldr	lr, [r7, #3536]
	cmp	r0, lr
	bhi	.L44
	ldr	r3, .L81+12
	mov	r4, r0, asl #2
	ldr	r5, [r3, r0, asl #2]
	ldrb	r3, [ip, r5]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r2, .L81+12
	movne	r3, r0
	addne	r2, r2, r4
	bne	.L43
	b	.L42
.L45:
	ldr	r1, [r2, #4]!
	ldrb	r1, [ip, r1]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L42
.L43:
	add	r3, r3, #1
	cmp	r3, lr
	bls	.L45
.L44:
	ldr	r3, [r6, #2416]
	add	r10, r10, #1
	add	r7, r7, #8
	cmp	r3, r10
	bhi	.L39
	ldr	r4, [fp, #-48]
.L32:
	ldr	lr, .L81+8
	mov	ip, #0
.L51:
	cmp	ip, r4
	bcs	.L61
	ldrb	r3, [lr, ip]	@ zero_extendqisi2
	cmp	r3, #1
	addeq	ip, ip, #1
	ldreq	r0, [r8, #840]
	beq	.L47
	add	r1, ip, lr
	mov	r2, ip
	b	.L49
.L48:
	ldrb	r0, [r1, #1]!	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L50
	mov	r2, r3
.L49:
	add	r3, r2, #1
	cmp	r4, r3
	bhi	.L48
.L50:
	ldr	r1, [r8, #840]
	add	r0, r1, #1
	add	r1, r8, r1, lsl #2
	strh	ip, [r1, #40]	@ movhi
	mov	ip, r3
	strh	r2, [r1, #42]	@ movhi
	str	r0, [r8, #840]
.L47:
	cmp	r0, #199
	ble	.L51
	ldr	r3, .L81+16
	mov	r0, #1
	ldr	r1, .L81+20
	sub	r4, r4, #1
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	mov	r3, #1
	strh	r0, [r8, #40]	@ movhi
	strh	r4, [r8, #42]	@ movhi
	str	r3, [r8, #840]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L42:
	ldr	r3, .L81+12
	add	r4, r3, r4
	b	.L41
.L80:
	ldr	r5, [r4, #4]!
.L41:
	strb	r9, [ip, r5]
	add	r0, r0, #1
	ldr	r3, [r7, #3536]
	cmp	r3, r0
	bcs	.L80
	b	.L44
.L73:
	ldr	r3, .L81+16
	mov	r2, r1
	mov	r0, #0
	ldr	r1, .L81+24
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L75:
	ldr	ip, .L81+16
	mov	r0, #1
	ldr	r2, .L81+28
	ldr	r1, .L81+32
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #1
	b	.L28
.L78:
	ldr	ip, .L81+16
	mov	r0, #1
	ldr	r2, .L81+28
	ldr	r1, .L81+36
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #1
	b	.L28
.L82:
	.align	2
.L81:
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	g_IsDeced
	.word	g_TsToRsMap
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LC1
	.word	.LANCHOR0
	.word	.LC3
	.word	.LC4
	UNWIND(.fnend)
	.size	PostPro_GetHevcMbGroup, .-PostPro_GetHevcMbGroup
	.align	2
	.global	PostPro_GetMp24MbGroup
	.type	PostPro_GetMp24MbGroup, %function
PostPro_GetMp24MbGroup:
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
	ldr	r2, [r1, #8]
	sub	r3, r2, #1
	cmp	r3, #199
	bhi	.L104
	ldr	r2, [r0, #24]
	mov	r3, #0
	ldr	r4, [r0, #28]
	str	r3, [r0, #840]
	str	r3, [r0, #856]
	mul	r4, r4, r2
	ldr	r3, [r1, #8]
	cmp	r3, #0
	add	ip, r1, r3, lsl #2
	uxth	r10, r4
	strh	r10, [ip, #14]	@ movhi
	strh	r10, [ip, #12]	@ movhi
	bne	.L105
.L86:
	mvn	r6, #0
	mov	r3, r1
	add	r2, r4, r6
	mov	r5, r6
	mov	ip, #0
	str	r2, [fp, #-48]
	b	.L95
.L109:
	cmp	r2, lr
	movle	r7, #0
	movgt	r7, #1
	cmp	r2, r5
	orrlt	r7, r7, #1
	cmp	r7, #0
	bne	.L87
	cmp	r2, r6
	movlt	r6, r5
	add	r7, r6, #1
	cmp	r7, r2
	beq	.L91
	add	r5, r5, r5, lsr #31
	sub	r8, r2, #1
	cmp	r5, r8
	bge	.L91
	ldr	r7, [r0, #840]
	cmp	r7, #199
	bgt	.L106
	sub	r6, r6, #100
	sxth	r9, r5
	cmp	r6, r9
	add	r9, r0, r7, lsl #2
	add	r7, r7, #1
	strh	r5, [r9, #40]	@ movhi
	strh	r8, [r9, #42]	@ movhi
	strgth	r6, [r9, #40]	@ movhi
	str	r7, [r0, #840]
.L91:
	ldr	r7, [r1, #8]
	sub	r5, r7, #1
	cmp	r5, ip
	beq	.L107
.L97:
	add	ip, ip, #1
	mov	r6, lr
	cmp	ip, r7
	mov	r5, r2
	add	r3, r3, #4
	bhi	.L108
.L95:
	ldrh	r2, [r3, #12]
	ldrh	lr, [r3, #14]
	cmp	r2, r4
	cmple	lr, r4
	ble	.L109
.L87:
	ldr	r7, [r1, #8]
	add	ip, ip, #1
	add	r3, r3, #4
	cmp	ip, r7
	bls	.L95
.L108:
	mov	r3, #0
.L102:
	mov	r0, r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L107:
	add	r5, r1, ip, lsl #2
	ldr	r6, [fp, #-48]
	ldrh	r5, [r5, #14]
	cmp	r5, r6
	bge	.L97
	ldr	r3, [r0, #840]
	cmp	r3, #199
	bgt	.L110
	add	r1, r0, r3, lsl #2
	add	r2, r3, #1
	sub	r10, r10, #1
	mov	r3, #0
	strh	r10, [r1, #42]	@ movhi
	strh	r5, [r1, #40]	@ movhi
	str	r2, [r0, #840]
	b	.L102
.L105:
	mov	r3, ip
	ldrh	ip, [ip, #10]
	sub	r2, r4, #10
	add	r3, r3, #6
	cmp	ip, r2
	subge	r2, r10, #1
	strgeh	r2, [r3, #4]	@ movhi
	b	.L86
.L104:
	ldr	r3, .L111
	mov	r0, #0
	ldr	r1, .L111+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r3, #1
	mov	r0, r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L106:
	ldr	r3, .L111
	mov	r2, #1296
.L103:
	ldr	r3, [r3, #68]
	mov	r0, #22
	ldr	r1, .L111+8
	blx	r3
	mov	r3, #1
	b	.L102
.L110:
	ldr	r3, .L111
	movw	r2, #1327
	b	.L103
.L112:
	.align	2
.L111:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC1
	.word	.LC5
	UNWIND(.fnend)
	.size	PostPro_GetMp24MbGroup, .-PostPro_GetMp24MbGroup
	.align	2
	.global	PostPro_GetVc1MbGroup
	.type	PostPro_GetVc1MbGroup, %function
PostPro_GetVc1MbGroup:
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
	ldr	r3, [r1, #8]
	sub	r3, r3, #1
	cmp	r3, #199
	bhi	.L127
	ldr	r2, [r0, #24]
	mov	r3, #0
	ldr	r5, [r0, #28]
	mvn	r6, #0
	str	r3, [r0, #840]
	mov	lr, r3
	str	r3, [r0, #856]
	mov	r7, r6
	mul	r5, r5, r2
	ldr	r2, [r1, #8]
	mov	r3, r1
	add	ip, r1, r2, lsl #2
	add	r9, r5, r6
	uxth	r8, r5
	mov	r2, ip
	strh	r8, [ip, #14]	@ movhi
	strh	r8, [ip, #12]	@ movhi
	b	.L122
.L131:
	cmp	ip, r6
	movlt	r6, r7
	add	r6, r6, #1
	cmp	r6, ip
	beq	.L118
	add	r7, r7, r7, lsr #31
	sub	r6, ip, #1
	cmp	r7, r6
	bge	.L118
	ldr	r2, [r0, #840]
	cmp	r2, #199
	bgt	.L128
	add	r10, r0, r2, lsl #2
	add	r2, r2, #1
	strh	r7, [r10, #40]	@ movhi
	strh	r6, [r10, #42]	@ movhi
	str	r2, [r0, #840]
.L118:
	ldr	r2, [r1, #8]
	sub	r6, r2, #1
	cmp	r6, lr
	beq	.L129
.L124:
	add	lr, lr, #1
	mov	r6, r4
	cmp	lr, r2
	mov	r7, ip
	add	r3, r3, #4
	bhi	.L130
.L122:
	ldrh	ip, [r3, #12]
	ldrh	r4, [r3, #14]
	cmp	ip, r5
	cmple	r4, r5
	movgt	r2, #1
	movle	r2, #0
	cmp	ip, r4
	orrgt	r2, r2, #1
	cmp	r2, #0
	beq	.L131
	ldr	r2, [r1, #8]
	add	lr, lr, #1
	add	r3, r3, #4
	cmp	lr, r2
	bls	.L122
.L130:
	mov	r3, #0
.L115:
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L129:
	add	r6, r1, lr, lsl #2
	ldrh	r6, [r6, #14]
	cmp	r6, r9
	bge	.L124
	ldr	r3, [r0, #840]
	cmp	r3, #199
	bgt	.L132
	add	r1, r0, r3, lsl #2
	add	r2, r3, #1
	sub	r8, r8, #1
	mov	r3, #0
	strh	r8, [r1, #42]	@ movhi
	strh	r6, [r1, #40]	@ movhi
	str	r2, [r0, #840]
	b	.L115
.L127:
	ldr	r3, .L133
	mov	r0, #0
	ldr	r1, .L133+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r3, #1
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L128:
	ldr	r3, .L133
	movw	r2, #1391
.L126:
	ldr	r3, [r3, #68]
	mov	r0, #22
	ldr	r1, .L133+8
	blx	r3
	mov	r3, #1
	b	.L115
.L132:
	ldr	r3, .L133
	movw	r2, #1411
	b	.L126
.L134:
	.align	2
.L133:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
	.word	.LC5
	UNWIND(.fnend)
	.size	PostPro_GetVc1MbGroup, .-PostPro_GetVc1MbGroup
	.align	2
	.global	PostPro_GetVp6MbGroup
	.type	PostPro_GetVp6MbGroup, %function
PostPro_GetVp6MbGroup:
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
	ldr	r3, [r1, #8]
	sub	r3, r3, #1
	cmp	r3, #199
	bhi	.L149
	ldr	r2, [r0, #24]
	mov	r3, #0
	ldr	r5, [r0, #28]
	mvn	r6, #0
	str	r3, [r0, #840]
	mov	lr, r3
	str	r3, [r0, #856]
	mov	r7, r6
	mul	r5, r5, r2
	ldr	r2, [r1, #8]
	mov	r3, r1
	add	ip, r1, r2, lsl #2
	add	r9, r5, r6
	uxth	r8, r5
	mov	r2, ip
	strh	r8, [ip, #14]	@ movhi
	strh	r8, [ip, #12]	@ movhi
	b	.L144
.L153:
	cmp	ip, r6
	movlt	r6, r7
	add	r6, r6, #1
	cmp	r6, ip
	beq	.L140
	add	r7, r7, r7, lsr #31
	sub	r6, ip, #1
	cmp	r7, r6
	bge	.L140
	ldr	r2, [r0, #840]
	cmp	r2, #199
	bgt	.L150
	add	r10, r0, r2, lsl #2
	add	r2, r2, #1
	strh	r7, [r10, #40]	@ movhi
	strh	r6, [r10, #42]	@ movhi
	str	r2, [r0, #840]
.L140:
	ldr	r2, [r1, #8]
	sub	r6, r2, #1
	cmp	r6, lr
	beq	.L151
.L146:
	add	lr, lr, #1
	mov	r6, r4
	cmp	lr, r2
	mov	r7, ip
	add	r3, r3, #4
	bhi	.L152
.L144:
	ldrh	ip, [r3, #12]
	ldrh	r4, [r3, #14]
	cmp	ip, r5
	cmple	r4, r5
	movgt	r2, #1
	movle	r2, #0
	cmp	ip, r4
	orrgt	r2, r2, #1
	cmp	r2, #0
	beq	.L153
	ldr	r2, [r1, #8]
	add	lr, lr, #1
	add	r3, r3, #4
	cmp	lr, r2
	bls	.L144
.L152:
	mov	r3, #0
.L137:
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L151:
	add	r6, r1, lr, lsl #2
	ldrh	r6, [r6, #14]
	cmp	r6, r9
	bge	.L146
	ldr	r3, [r0, #840]
	cmp	r3, #199
	bgt	.L154
	add	r1, r0, r3, lsl #2
	add	r2, r3, #1
	sub	r8, r8, #1
	mov	r3, #0
	strh	r8, [r1, #42]	@ movhi
	strh	r6, [r1, #40]	@ movhi
	str	r2, [r0, #840]
	b	.L137
.L149:
	ldr	r3, .L155
	mov	r0, #0
	ldr	r1, .L155+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r3, #1
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L150:
	ldr	r3, .L155
	movw	r2, #1476
.L148:
	ldr	r3, [r3, #68]
	mov	r0, #22
	ldr	r1, .L155+8
	blx	r3
	mov	r3, #1
	b	.L137
.L154:
	ldr	r3, .L155
	movw	r2, #1496
	b	.L148
.L156:
	.align	2
.L155:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
	.word	.LC5
	UNWIND(.fnend)
	.size	PostPro_GetVp6MbGroup, .-PostPro_GetVp6MbGroup
	.align	2
	.global	PostPro_GetVp8MbGroup
	.type	PostPro_GetVp8MbGroup, %function
PostPro_GetVp8MbGroup:
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
	ldr	r3, [r1, #8]
	sub	r3, r3, #1
	cmp	r3, #199
	bhi	.L171
	ldr	r2, [r0, #24]
	mov	r3, #0
	ldr	r5, [r0, #28]
	mvn	r6, #0
	str	r3, [r0, #840]
	mov	lr, r3
	str	r3, [r0, #856]
	mov	r7, r6
	mul	r5, r5, r2
	ldr	r2, [r1, #8]
	mov	r3, r1
	add	ip, r1, r2, lsl #2
	add	r9, r5, r6
	uxth	r8, r5
	mov	r2, ip
	strh	r8, [ip, #14]	@ movhi
	strh	r8, [ip, #12]	@ movhi
	b	.L166
.L175:
	cmp	ip, r6
	movlt	r6, r7
	add	r6, r6, #1
	cmp	r6, ip
	beq	.L162
	add	r7, r7, r7, lsr #31
	sub	r6, ip, #1
	cmp	r7, r6
	bge	.L162
	ldr	r2, [r0, #840]
	cmp	r2, #199
	bgt	.L172
	add	r10, r0, r2, lsl #2
	add	r2, r2, #1
	strh	r7, [r10, #40]	@ movhi
	strh	r6, [r10, #42]	@ movhi
	str	r2, [r0, #840]
.L162:
	ldr	r2, [r1, #8]
	sub	r6, r2, #1
	cmp	r6, lr
	beq	.L173
.L168:
	add	lr, lr, #1
	mov	r6, r4
	cmp	lr, r2
	mov	r7, ip
	add	r3, r3, #4
	bhi	.L174
.L166:
	ldrh	ip, [r3, #12]
	ldrh	r4, [r3, #14]
	cmp	ip, r5
	cmple	r4, r5
	movgt	r2, #1
	movle	r2, #0
	cmp	ip, r4
	orrgt	r2, r2, #1
	cmp	r2, #0
	beq	.L175
	ldr	r2, [r1, #8]
	add	lr, lr, #1
	add	r3, r3, #4
	cmp	lr, r2
	bls	.L166
.L174:
	mov	r3, #0
.L159:
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L173:
	add	r6, r1, lr, lsl #2
	ldrh	r6, [r6, #14]
	cmp	r6, r9
	bge	.L168
	ldr	r3, [r0, #840]
	cmp	r3, #199
	bgt	.L176
	add	r1, r0, r3, lsl #2
	add	r2, r3, #1
	sub	r8, r8, #1
	mov	r3, #0
	strh	r8, [r1, #42]	@ movhi
	strh	r6, [r1, #40]	@ movhi
	str	r2, [r0, #840]
	b	.L159
.L171:
	ldr	r3, .L177
	mov	r0, #0
	ldr	r1, .L177+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r3, #1
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L172:
	ldr	r3, .L177
	movw	r2, #1560
.L170:
	ldr	r3, [r3, #68]
	mov	r0, #22
	ldr	r1, .L177+8
	blx	r3
	mov	r3, #1
	b	.L159
.L176:
	ldr	r3, .L177
	movw	r2, #1580
	b	.L170
.L178:
	.align	2
.L177:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
	.word	.LC5
	UNWIND(.fnend)
	.size	PostPro_GetVp8MbGroup, .-PostPro_GetVp8MbGroup
	.align	2
	.global	PostPro_GetAvsMbGroup
	.type	PostPro_GetAvsMbGroup, %function
PostPro_GetAvsMbGroup:
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
	ldr	r2, [r1, #8]
	sub	r3, r2, #1
	cmp	r3, #199
	bhi	.L190
	ldr	lr, [r0, #24]
	mov	r3, #0
	ldr	r4, [r0, #28]
	mvn	r7, #0
	str	r3, [r0, #856]
	mov	ip, r3
	ldr	r2, [r1, #8]
	mov	r9, r3
	mul	r4, r4, lr
	mov	r6, r7
	add	r5, r1, r2, lsl #2
	str	r1, [fp, #-56]
	mov	r8, r0
	add	lr, r4, r7
	str	lr, [fp, #-48]
	mov	r2, r5
	add	lr, r4, r4, lsr #31
	uxth	r10, r4
	strh	r10, [r5, #14]	@ movhi
	mov	lr, lr, asr #1
	str	r10, [fp, #-64]
	add	r3, lr, r7
	str	lr, [fp, #-60]
	str	r3, [fp, #-52]
	mov	r3, r1
	strh	r10, [r2, #12]	@ movhi
	b	.L186
.L193:
	cmp	r2, r7
	movlt	r7, r6
	add	r7, r7, #1
	cmp	r2, r7
	ble	.L184
	ldr	r7, [r8, #840]
	uxth	r5, r9
	rsb	r10, r5, r6, lsr #31
	rsb	r5, r5, r2
	add	r6, r10, r6
	sub	r5, r5, #1
	add	r10, r8, r7, lsl #2
	add	r7, r7, #1
	strh	r6, [r10, #40]	@ movhi
	strh	r5, [r10, #42]	@ movhi
	ldr	r5, [r8, #32]
	str	r7, [r8, #840]
	cmp	r5, #0
	bne	.L184
	ldr	r5, [fp, #-52]
	cmp	r2, r5
	addge	r8, r0, #872
	ldrge	r9, [fp, #-60]
.L184:
	ldr	r5, [r1, #8]
	sub	r6, r5, #1
	cmp	r6, ip
	beq	.L191
.L188:
	add	ip, ip, #1
	mov	r7, lr
	cmp	ip, r5
	mov	r6, r2
	add	r3, r3, #4
	bhi	.L192
.L186:
	ldrh	r2, [r3, #12]
	ldrh	lr, [r3, #14]
	cmp	r2, r4
	cmple	lr, r4
	movgt	r5, #1
	movle	r5, #0
	cmp	r2, lr
	orrgt	r5, r5, #1
	cmp	r5, #0
	beq	.L193
	ldr	r5, [r1, #8]
	add	ip, ip, #1
	add	r3, r3, #4
	cmp	ip, r5
	bls	.L186
.L192:
	mov	r2, #0
.L181:
	mov	r0, r2
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L191:
	add	r6, r1, ip, lsl #2
	ldr	r7, [fp, #-48]
	ldrh	r6, [r6, #14]
	cmp	r6, r7
	bge	.L188
	ldr	r1, [r0, #840]
	mov	r2, #0
	ldr	r3, [fp, #-64]
	add	ip, r1, #1
	add	r1, r0, r1, lsl #2
	sub	r3, r3, #1
	strh	r6, [r1, #40]	@ movhi
	strh	r3, [r1, #42]	@ movhi
	str	ip, [r0, #840]
	b	.L181
.L190:
	ldr	r3, .L194
	mov	r0, #0
	ldr	r1, .L194+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r2, #1
	mov	r0, r2
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L195:
	.align	2
.L194:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC1
	UNWIND(.fnend)
	.size	PostPro_GetAvsMbGroup, .-PostPro_GetAvsMbGroup
	.align	2
	.global	PostPro_GetDivx3MbGroup
	.type	PostPro_GetDivx3MbGroup, %function
PostPro_GetDivx3MbGroup:
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
	ldr	r2, [r1, #8]
	sub	r3, r2, #1
	cmp	r3, #3
	bhi	.L207
	ldr	lr, [r0, #24]
	mov	r3, #0
	ldr	r4, [r0, #28]
	mvn	r7, #0
	str	r3, [r0, #856]
	mov	ip, r3
	ldr	r2, [r1, #8]
	mov	r9, r3
	mul	r4, r4, lr
	mov	r6, r7
	add	r5, r1, r2, lsl #2
	str	r1, [fp, #-56]
	mov	r8, r0
	add	lr, r4, r7
	str	lr, [fp, #-48]
	mov	r2, r5
	add	lr, r4, r4, lsr #31
	uxth	r10, r4
	strh	r10, [r5, #14]	@ movhi
	mov	lr, lr, asr #1
	str	r10, [fp, #-64]
	add	r3, lr, r7
	str	lr, [fp, #-60]
	str	r3, [fp, #-52]
	mov	r3, r1
	strh	r10, [r2, #12]	@ movhi
	b	.L203
.L210:
	cmp	r2, r7
	movlt	r7, r6
	add	r7, r7, #1
	cmp	r2, r7
	ble	.L201
	ldr	r7, [r8, #840]
	uxth	r5, r9
	rsb	r10, r5, r6, lsr #31
	rsb	r5, r5, r2
	add	r6, r10, r6
	sub	r5, r5, #1
	add	r10, r8, r7, lsl #2
	add	r7, r7, #1
	strh	r6, [r10, #40]	@ movhi
	strh	r5, [r10, #42]	@ movhi
	ldr	r5, [r8, #32]
	str	r7, [r8, #840]
	cmp	r5, #0
	bne	.L201
	ldr	r5, [fp, #-52]
	cmp	r2, r5
	addge	r8, r0, #872
	ldrge	r9, [fp, #-60]
.L201:
	ldr	r5, [r1, #8]
	sub	r6, r5, #1
	cmp	r6, ip
	beq	.L208
.L205:
	add	ip, ip, #1
	mov	r7, lr
	cmp	ip, r5
	mov	r6, r2
	add	r3, r3, #4
	bhi	.L209
.L203:
	ldrh	r2, [r3, #12]
	ldrh	lr, [r3, #14]
	cmp	r2, r4
	cmple	lr, r4
	movgt	r5, #1
	movle	r5, #0
	cmp	r2, lr
	orrgt	r5, r5, #1
	cmp	r5, #0
	beq	.L210
	ldr	r5, [r1, #8]
	add	ip, ip, #1
	add	r3, r3, #4
	cmp	ip, r5
	bls	.L203
.L209:
	mov	r2, #0
.L198:
	mov	r0, r2
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L208:
	add	r6, r1, ip, lsl #2
	ldr	r7, [fp, #-48]
	ldrh	r6, [r6, #14]
	cmp	r6, r7
	bge	.L205
	ldr	r1, [r0, #840]
	mov	r2, #0
	ldr	r3, [fp, #-64]
	add	ip, r1, #1
	add	r1, r0, r1, lsl #2
	sub	r3, r3, #1
	strh	r6, [r1, #40]	@ movhi
	strh	r3, [r1, #42]	@ movhi
	str	ip, [r0, #840]
	b	.L198
.L207:
	ldr	r3, .L211
	mov	r0, #0
	ldr	r1, .L211+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r2, #1
	mov	r0, r2
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L212:
	.align	2
.L211:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC1
	UNWIND(.fnend)
	.size	PostPro_GetDivx3MbGroup, .-PostPro_GetDivx3MbGroup
	.align	2
	.global	PostPro_BubbleSort
	.type	PostPro_BubbleSort, %function
PostPro_BubbleSort:
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
	ldr	r8, [r0, #8]
	cmp	r8, #1
	bls	.L225
	mov	r7, #1
.L218:
	mov	r4, #0
	rsb	lr, r7, r8
	mov	r2, r4
	mov	r3, r0
	b	.L219
.L217:
	ldrh	r1, [r3, #12]
	ldrh	ip, [r3, #16]
	cmp	r1, ip
	bls	.L216
	ldrh	r6, [r3, #14]
	mov	r4, #1
	ldrh	r5, [r3, #18]
	strh	ip, [r3, #12]	@ movhi
	strh	r1, [r3, #16]	@ movhi
	strh	r6, [r3, #18]	@ movhi
	strh	r5, [r3, #14]	@ movhi
.L216:
	add	r3, r3, #4
.L219:
	cmp	r2, lr
	add	r2, r2, #1
	bne	.L217
	cmp	r4, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	add	r7, r7, #1
	cmp	r8, r7
	movls	r4, #0
	andhi	r4, r4, #1
	cmp	r4, #0
	bne	.L218
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L225:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	PostPro_BubbleSort, .-PostPro_BubbleSort
	.global	__aeabi_uidivmod
	.global	__aeabi_uidiv
	.align	2
	.global	PostPro_GetH264MbGroup
	.type	PostPro_GetH264MbGroup, %function
PostPro_GetH264MbGroup:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 824
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #828)
	sub	sp, sp, #828
	ldr	r7, [r1, #8]
	mov	r6, r1
	mov	r9, r0
	sub	r3, r7, #1
	cmp	r3, #199
	bhi	.L264
	ldr	r10, [r0, #24]
	cmp	r2, #0
	ldr	r4, [r0, #28]
	mul	r4, r4, r10
	uxtheq	r8, r4
	bne	.L265
.L230:
	mov	ip, #0
	str	ip, [r9, #840]
	str	ip, [r9, #856]
	mvn	r5, #0
	ldr	r2, [r6, #8]
	add	r3, r4, r5
	mov	r1, r5
	str	r3, [fp, #-848]
	mov	r3, r6
	add	r0, r6, r2, lsl #2
	strh	r8, [r0, #14]	@ movhi
	strh	r8, [r0, #12]	@ movhi
	b	.L247
.L269:
	cmp	r2, r5
	movlt	r5, r1
	add	lr, r5, #1
	cmp	r2, lr
	ble	.L242
	ldr	r7, [r9, #840]
	cmp	r7, #199
	bgt	.L266
	add	lr, r1, r1, lsr #31
	sub	r1, r5, #100
	add	r5, r9, r7, lsl #2
	add	r7, r7, #1
	sxth	r10, lr
	strh	lr, [r5, #40]	@ movhi
	cmp	r1, r10
	sub	lr, r2, #1
	strgth	r1, [r5, #40]	@ movhi
	strh	lr, [r5, #42]	@ movhi
	str	r7, [r9, #840]
.L242:
	ldr	lr, [r6, #8]
	sub	r1, lr, #1
	cmp	r1, ip
	beq	.L267
.L254:
	add	ip, ip, #1
	mov	r5, r0
	cmp	lr, ip
	mov	r1, r2
	add	r3, r3, #4
	bcc	.L268
.L247:
	ldrh	r2, [r3, #12]
	ldrh	r0, [r3, #14]
	cmp	r2, r4
	cmple	r0, r4
	movgt	r7, #1
	movle	r7, #0
	cmp	r2, r0
	orrgt	r7, r7, #1
	cmp	r7, #0
	beq	.L269
	ldr	lr, [r6, #8]
	add	ip, ip, #1
	add	r3, r3, #4
	cmp	lr, ip
	bcs	.L247
.L268:
	mov	r0, #0
.L257:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L267:
	add	r1, r6, ip, lsl #2
	ldr	r5, [fp, #-848]
	ldrh	r1, [r1, #14]
	cmp	r1, r5
	bge	.L254
	ldr	r3, [r9, #840]
	cmp	r3, #199
	bgt	.L270
	add	r2, r9, r3, lsl #2
	sub	r8, r8, #1
	add	r3, r3, #1
	mov	r0, #0
	strh	r8, [r2, #42]	@ movhi
	strh	r1, [r2, #40]	@ movhi
	str	r3, [r9, #840]
	b	.L257
.L265:
	cmp	r7, #0
	mov	r10, r10, asl #1
	beq	.L233
	sub	ip, fp, #844
	mov	r7, r7, asl #2
	add	lr, r1, r7
	mov	r3, r1
	mov	r2, ip
	str	ip, [fp, #-852]
.L232:
	ldrh	r0, [r3, #12]
	add	r3, r3, #4
	ldrh	r1, [r3, #10]
	cmp	r3, lr
	add	r2, r2, #4
	strh	r0, [r2, #-4]	@ movhi
	strh	r1, [r2, #-2]	@ movhi
	bne	.L232
	ldrh	r8, [ip]
	cmp	r10, #0
	ldrh	r3, [ip, #2]
	bic	r8, r8, #1
	orr	r3, r3, #1
	strh	r3, [ip, #2]	@ movhi
	uxth	r8, r8
	strh	r8, [ip]	@ movhi
	mov	r5, r3
	beq	.L252
	sub	r7, r7, #4
	sub	r3, r10, #2
	str	r3, [fp, #-848]
	add	r3, ip, r7
	mov	r7, #0
	str	r3, [fp, #-856]
	str	r4, [fp, #-868]
	uxth	r3, r10
	str	r3, [fp, #-864]
.L248:
	mov	r1, r10
	mov	r0, r8
	bl	__aeabi_uidivmod
	mov	r0, r5
	mov	r4, r1
	cmp	r4, #1
	mov	r1, r10
	bls	.L234
	bl	__aeabi_uidivmod
	ldr	r3, [fp, #-848]
	cmp	r3, r1
	bls	.L235
	str	r1, [fp, #-860]
	mov	r0, r8
	mov	r1, r10
	mov	r4, r4, lsr #1
	bl	__aeabi_uidiv
	mov	r1, r10
	mov	r8, r0
	mov	r0, r5
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-860]
	mov	r2, r2, lsr #1
	cmp	r0, r8
	mov	r3, r0
	beq	.L236
	ldr	r1, [fp, #-864]
	add	lr, r6, r7, lsl #2
	add	r5, r7, #1
	cmp	r5, #199
	smulbb	r1, r8, r1
	uxth	r1, r1
	add	r4, r1, r4
	uxth	ip, r4
	strh	ip, [lr, #12]	@ movhi
	ldr	r0, [r9, #24]
	sub	r0, r0, #1
	add	r1, r1, r0
	strh	r1, [lr, #14]	@ movhi
	bhi	.L262
	ldr	r1, [fp, #-864]
	add	r5, r6, r5, lsl #2
	ldr	r0, [r9, #24]
	uxth	r2, r2
	smulbb	r3, r3, r1
	add	r1, r7, #2
	cmp	r1, #199
	add	ip, ip, r0
	strh	ip, [lr, #16]	@ movhi
	uxth	r3, r3
	add	r0, r2, r3
	strh	r0, [r5, #14]	@ movhi
	bhi	.L262
	ldr	r0, [r9, #24]
	add	r7, r7, #3
	add	r1, r6, r1, lsl #2
	cmp	r7, #199
	add	r3, r3, r0
	uxth	r3, r3
	strh	r3, [lr, #20]	@ movhi
	add	r2, r3, r2
	strh	r2, [r1, #14]	@ movhi
	bhi	.L262
.L238:
	ldr	r3, [fp, #-852]
	ldr	r2, [fp, #-856]
	cmp	r3, r2
	beq	.L271
	ldr	r3, [fp, #-852]
	add	r3, r3, #4
	ldrh	r8, [r3]
	ldrh	r5, [r3, #2]
	bic	r8, r8, #1
	strh	r8, [r3]	@ movhi
	orr	r5, r5, #1
	strh	r5, [r3, #2]	@ movhi
	str	r3, [fp, #-852]
	b	.L248
.L235:
	mov	r1, r10
	mov	r0, r8
	bl	__aeabi_uidiv
	add	r2, r6, r7, lsl #2
	add	r3, r7, #1
	cmp	r3, #199
	smulbb	r0, r0, r10
	uxth	r0, r0
	add	r4, r0, r4, lsr #1
	uxth	r4, r4
	strh	r4, [r2, #12]	@ movhi
	ldr	r1, [r9, #24]
	sub	r1, r1, #1
	add	r0, r0, r1
	strh	r0, [r2, #14]	@ movhi
	bhi	.L262
	add	r7, r7, #2
	ldr	r1, [r9, #24]
	add	r3, r6, r3, lsl #2
	cmp	r7, #199
	add	r4, r4, r1
	strh	r4, [r2, #16]	@ movhi
	strh	r5, [r3, #14]	@ movhi
	bls	.L238
.L262:
	ldr	r4, [fp, #-868]
	mov	r2, #1
	str	r2, [r6, #8]
	uxth	r8, r4
	sub	r3, r8, #1
	strh	r3, [r6, #14]	@ movhi
	mov	r3, #0
	strh	r3, [r6, #12]	@ movhi
.L250:
	mov	r0, r6
	bl	PostPro_BubbleSort
	b	.L230
.L234:
	bl	__aeabi_uidivmod
	ldr	r2, [fp, #-848]
	cmp	r2, r1
	bls	.L239
	add	r4, r6, r7, lsl #2
	str	r1, [fp, #-860]
	mov	r0, r5
	mov	r1, r10
	strh	r8, [r4, #12]	@ movhi
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-860]
	add	ip, r7, #1
	cmp	ip, #199
	ubfx	r1, r3, #1, #16
	smulbb	r0, r0, r10
	uxth	r0, r0
	add	r3, r1, r0
	strh	r3, [r4, #14]	@ movhi
	bhi	.L262
	ldr	r3, [r9, #24]
	add	r7, r7, #2
	add	ip, r6, ip, lsl #2
	cmp	r7, #199
	add	r2, r0, r3
	uxth	r3, r2
	strh	r3, [r4, #16]	@ movhi
	add	r3, r3, r1
	strh	r3, [ip, #14]	@ movhi
	bls	.L238
	b	.L262
.L239:
	add	r2, r6, r7, lsl #2
	add	r7, r7, #1
	cmp	r7, #199
	strh	r8, [r2, #12]	@ movhi
	strh	r5, [r2, #14]	@ movhi
	bls	.L238
	b	.L262
.L236:
	smulbb	r3, r0, r10
	add	r1, r7, #1
	add	lr, r6, r7, lsl #2
	cmp	r1, #199
	uxth	r3, r3
	add	r4, r3, r4
	add	r2, r3, r2
	uxth	r4, r4
	uxth	r2, r2
	strh	r4, [lr, #12]	@ movhi
	strh	r2, [lr, #14]	@ movhi
	bhi	.L262
	ldr	r3, [r9, #24]
	add	r7, r7, #2
	add	r1, r6, r1, lsl #2
	cmp	r7, #199
	add	r4, r4, r3
	strh	r4, [lr, #16]	@ movhi
	ldr	r3, [r9, #24]
	add	r2, r2, r3
	strh	r2, [r1, #14]	@ movhi
	bls	.L238
	b	.L262
.L252:
	mov	r7, r10
.L233:
	uxth	r8, r4
	str	r7, [r6, #8]
	b	.L250
.L264:
	ldr	r3, .L272
	mov	r2, r7
	ldr	r1, .L272+4
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L271:
	ldr	r4, [fp, #-868]
	b	.L233
.L266:
	ldr	r3, .L272
	movw	r2, #1190
.L263:
	ldr	r1, .L272+8
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	b	.L257
.L270:
	ldr	r3, .L272
	movw	r2, #1219
	b	.L263
.L273:
	.align	2
.L272:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC1
	.word	.LC5
	UNWIND(.fnend)
	.size	PostPro_GetH264MbGroup, .-PostPro_GetH264MbGroup
	.align	2
	.global	PostPro_GetRepairParam
	.type	PostPro_GetRepairParam, %function
PostPro_GetRepairParam:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r8, r3
	mov	r4, r1
	mov	r3, #0
	mov	r5, r2
	str	r0, [r1]
	str	r3, [r1, #840]
	str	r3, [r1, #1712]
	cmp	r0, #17
	ldrls	pc, [pc, r0, asl #2]
	b	.L275
.L277:
	.word	.L276
	.word	.L278
	.word	.L279
	.word	.L280
	.word	.L275
	.word	.L281
	.word	.L282
	.word	.L275
	.word	.L283
	.word	.L284
	.word	.L285
	.word	.L285
	.word	.L285
	.word	.L286
	.word	.L275
	.word	.L275
	.word	.L287
	.word	.L288
.L288:
	ldr	r3, [r2, #1280]
	mov	r1, #1
	add	r3, r3, #328
	add	r3, r3, #2
	ldr	r3, [r2, r3, asl #2]
	str	r3, [r4, #4]
	ldr	r3, [r2, #2640]
	str	r3, [r4, #8]
	ldr	r3, [r2, #2420]
	ldr	r3, [r3, #372]
	add	r3, r3, #328
	add	r3, r3, #2
	ldr	r3, [r2, r3, asl #2]
	str	r3, [r4, #16]
	ldr	r3, [r2, #1284]
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #1452]
	str	r3, [r4, #20]
	ldr	r0, [r2, #68]
	mov	r3, r1, asl r0
	str	r0, [r4, #864]
	ldr	r2, [r2, #188]
	sub	r2, r2, #1
	add	r2, r2, r3
	mov	r2, r2, lsr r0
	str	r2, [r4, #24]
	ldr	r2, [r5, #184]
	sub	r2, r2, #1
	add	r3, r2, r3
	mov	r3, r3, lsr r0
	str	r3, [r4, #28]
	ldrb	r3, [r5, #217]	@ zero_extendqisi2
	str	r1, [r4, #32]
	str	r3, [r4, #868]
	ldr	r3, [r5, #8]
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r4, #36]
	ldr	r3, [r5, #1316]
	cmp	r3, #0
	streq	r3, [r8, #8]
	bne	.L401
.L309:
	ldr	r1, [r4, #4]
	ldr	r3, [r4, #16]
	cmp	r1, r3
	beq	.L402
.L310:
	ldr	r6, .L409
	mov	r3, #0
	str	r3, [r4, #852]
.L307:
	ldr	r9, [r4, #840]
	ldr	r3, [r4, #24]
	ldr	r7, [r4, #28]
	cmp	r9, #0
	mul	r7, r7, r3
	beq	.L341
	mov	r9, #0
	mov	r5, r4
	mov	r10, r9
.L342:
	ldrsh	r3, [r5, #40]
	mov	r0, #22
	ldrsh	r2, [r5, #42]
	add	r10, r10, #1
	ldr	r1, .L409+4
	add	r5, r5, #4
	ldr	ip, [r6, #68]
	blx	ip
	ldrsh	r3, [r5, #38]
	ldrsh	r2, [r5, #36]
	ldr	r1, [r4, #840]
	rsb	r3, r2, r3
	cmp	r1, r10
	add	r3, r3, #1
	add	r9, r9, r3
	bhi	.L342
.L341:
	ldr	r1, [r4, #1712]
	cmp	r1, #0
	beq	.L343
	add	r0, r4, #912
	mov	r2, #0
	add	r0, r0, #2
.L344:
	ldrsh	r3, [r0]
	add	r2, r2, #1
	ldrsh	ip, [r0, #-2]
	cmp	r2, r1
	add	r0, r0, #4
	rsb	ip, ip, r3
	add	ip, ip, #1
	add	r9, r9, ip
	bne	.L344
.L343:
	cmp	r7, #0
	moveq	r4, r7
	beq	.L345
	mov	r0, #100
	mov	r1, r7
	mul	r0, r0, r9
	bl	__aeabi_uidiv
	cmp	r0, #100
	movcc	r4, r0
	movcs	r4, #100
.L345:
	mov	r3, r9
	mov	r2, r7
	mov	ip, #0
	ldr	r1, .L409+8
	str	ip, [r8, #8]
	mov	r0, #22
	ldr	ip, [r6, #68]
	blx	ip
	mov	r0, r4
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L276:
	ldr	r3, [r2, #292]
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #308]
	str	r3, [r1, #4]
	ldr	r3, [r2, #296]
	str	r3, [r1, #8]
	ldr	r3, [r2, #868]
	ldr	r0, [r3, #1640]
	add	r3, r2, r0, lsl #2
	ldr	r3, [r3, #308]
	str	r3, [r1, #16]
	ldr	r3, [r2, #300]
	add	r3, r3, #142
	ldr	r3, [r2, r3, asl #2]
	str	r3, [r1, #20]
	ldr	r3, [r2, #28]
	str	r3, [r1, #24]
	ldr	r3, [r2, #20]
	str	r3, [r1, #28]
	ldrb	r3, [r2]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r1, #32]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r1, #36]
	ldr	ip, [r2, #860]
	cmp	ip, #0
	beq	.L289
	ldr	r3, [r2, #1064]
	cmp	r3, r0
	addne	r2, r2, #1056
	movne	r3, #0
	addne	r2, r2, #8
	bne	.L292
	b	.L403
.L293:
	ldr	r1, [r2, #4]!
	cmp	r1, r0
	beq	.L290
.L292:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L293
.L289:
	ldr	r2, [r5, #32]
	ldrsb	r3, [r5, #2]
	cmp	r2, #0
	ldrneb	r2, [r5]	@ zero_extendqisi2
	clzne	r2, r2
	movne	r2, r2, lsr #5
	cmp	r3, #0
	streq	r3, [r8, #8]
	bne	.L404
.L296:
	ldr	r0, [r4, #4]
	ldr	r3, [r4, #16]
	cmp	r0, r3
	bne	.L323
	ldr	ip, [r5, #564]
	cmp	ip, #0
	beq	.L323
	ldr	r2, [r5, #308]
	cmp	r0, r2
	addeq	r1, r5, #308
	moveq	r3, #0
	bne	.L324
.L300:
	add	r3, r3, #1
	cmp	r3, ip
	beq	.L323
	ldr	r2, [r1, #4]!
	cmp	r0, r2
	beq	.L300
.L324:
	str	r2, [r4, #16]
	b	.L323
.L278:
	add	r7, r2, #12288
	mov	r6, #0
	mov	r1, r8
	mov	r0, r4
	ldr	r3, [r7, #212]
	str	r3, [r4, #4]
	ldr	r3, [r7, #96]
	str	r3, [r4, #8]
	ldr	r3, [r7, #100]
	str	r3, [r4, #16]
	ldr	r3, [r7, #108]
	str	r3, [r4, #20]
	ldrh	r3, [r2, #78]
	add	r3, r3, #1
	str	r3, [r4, #24]
	ldrh	r3, [r2, #80]
	add	r3, r3, #1
	str	r3, [r4, #28]
	ldrb	r3, [r2, #5]	@ zero_extendqisi2
	str	r6, [r4, #36]
	tst	r3, #253
	moveq	r3, #1
	movne	r3, r6
	str	r3, [r4, #32]
	bl	PostPro_GetVc1MbGroup
	ldr	r2, [r4, #32]
	ldrb	r3, [r5, #68]	@ zero_extendqisi2
	cmp	r2, #1
	streq	r6, [r4, #852]
	and	r3, r3, #3
	str	r3, [r4, #844]
	beq	.L339
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L405
.L339:
	mov	r3, #4
	str	r3, [r4, #864]
	ldr	r3, [r7, #244]
	ldr	r6, .L409
	str	r3, [r4, #848]
	b	.L307
.L279:
	ldr	r0, [r2, #212]
	add	r3, r2, #12288
	mov	r7, #1
	mov	r2, #0
	mov	ip, #4
	mov	r1, r8
	str	r0, [r4, #4]
	mov	r0, r4
	ldr	lr, [r3, #2304]
	ldr	r6, .L409
	str	lr, [r4, #8]
	ldr	lr, [r5, #220]
	str	lr, [r4, #16]
	ldr	lr, [r5, #228]
	str	lr, [r4, #20]
	ldrh	lr, [r5, #164]
	str	lr, [r4, #24]
	ldrh	lr, [r5, #166]
	str	r7, [r4, #32]
	str	r2, [r4, #36]
	str	lr, [r4, #28]
	str	r2, [r4, #852]
	ldr	r3, [r3, #2372]
	str	ip, [r4, #864]
	str	r3, [r4, #848]
	bl	PostPro_GetMp24MbGroup
	b	.L307
.L280:
	ldr	r3, [r2, #200]
	mov	r2, #0
	str	r3, [r1, #4]
	ldr	r3, [r5, #224]
	str	r3, [r1, #8]
	ldr	r3, [r5, #196]
	str	r3, [r1, #16]
	ldr	r3, [r5, #240]
	str	r3, [r1, #20]
	ldrh	r3, [r5, #152]
	str	r3, [r1, #24]
	ldrh	r3, [r5, #148]
	str	r3, [r1, #28]
	ldrb	r3, [r5, #3]	@ zero_extendqisi2
	str	r2, [r1, #36]
	sub	r3, r3, #3
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r1, #32]
	ldrb	r3, [r5, #3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L394
	cmp	r3, #2
	beq	.L394
	cmp	r3, #3
	streq	r2, [r1, #852]
.L334:
	add	r5, r5, #45056
	mov	r2, #4
	mov	r1, r8
	mov	r0, r4
	ldr	r3, [r5, #260]
	str	r2, [r4, #864]
	ldr	r6, .L409
	str	r3, [r4, #848]
	bl	PostPro_GetMp24MbGroup
	b	.L307
.L281:
	ldr	ip, [r2, #128]
	mov	r3, #0
	mov	lr, #1
	mov	r2, #4
	mov	r1, r8
	mov	r0, r4
	str	ip, [r4, #4]
	ldr	ip, [r5, #124]
	ldr	r6, .L409
	str	ip, [r4, #16]
	ldr	ip, [r5, #144]
	str	ip, [r4, #20]
	ldr	ip, [r5, #8]
	str	ip, [r4, #24]
	ldr	ip, [r5, #12]
	str	lr, [r4, #32]
	str	r3, [r4, #36]
	str	ip, [r4, #28]
	str	r3, [r4, #852]
	ldr	r3, [r5, #152]
	str	r2, [r4, #864]
	str	r3, [r4, #848]
	bl	PostPro_GetDivx3MbGroup
	b	.L307
.L282:
	add	r7, r2, #12288
	mov	ip, #0
	ldr	r10, [r7, #2272]
	str	r10, [r1, #4]
	ldr	r3, [r7, #2284]
	str	r3, [r1, #8]
	ldr	r9, [r7, #2264]
	str	r9, [r1, #16]
	ldr	lr, [r7, #2276]
	str	lr, [r1, #20]
	ldr	r6, [r2, #164]
	str	r6, [r1, #24]
	ldr	r2, [r2, #168]
	str	r2, [r1, #28]
	add	r3, r2, r2, lsr #31
	ldr	r1, [r5, #32]
	mov	r0, r3, asr #1
	str	r10, [r4, #876]
	sub	r1, r1, #3
	str	r9, [r4, #888]
	clz	r1, r1
	str	ip, [r4, #36]
	mov	r1, r1, lsr #5
	str	r1, [r4, #32]
	ldr	r3, [r8, #4]
	str	r1, [r4, #904]
	str	r2, [r4, #900]
	add	r3, r3, r3, lsl #3
	str	r6, [r4, #896]
	str	ip, [r4, #908]
	mla	r3, r0, r3, lr
	str	r3, [r4, #892]
	ldr	r3, [r5, #32]
	cmp	r3, #3
	streq	ip, [r4, #852]
	beq	.L337
	cmp	r3, #1
	streq	r3, [r4, #852]
.L337:
	ldr	r2, [r7, #2696]
	mov	r3, #4
	str	r3, [r4, #864]
	mov	r1, r8
	mov	r0, r4
	ldr	r6, .L409
	str	r2, [r4, #848]
	ldr	r2, [r7, #2696]
	str	r3, [r4, #1736]
	str	r2, [r4, #1720]
	bl	PostPro_GetAvsMbGroup
	b	.L307
.L283:
	ldr	r0, [r2, #56]
	add	ip, r2, #8192
	mov	r3, #0
	mov	r7, #1
	mov	r2, #4
	mov	r1, r8
	str	r0, [r4, #4]
	mov	r0, r4
	ldr	lr, [r5, #52]
	ldr	r6, .L409
	str	lr, [r4, #16]
	ldr	lr, [r5, #72]
	str	lr, [r4, #20]
	ldr	lr, [r5, #4]
	str	lr, [r4, #24]
	ldr	lr, [r5, #8]
	str	r7, [r4, #32]
	str	r3, [r4, #36]
	str	lr, [r4, #28]
	str	r3, [r4, #852]
	ldr	r3, [ip, #3152]
	str	r2, [r4, #864]
	str	r3, [r4, #848]
	bl	PostPro_GetMp24MbGroup
	b	.L307
.L284:
	ldr	r0, [r2, #52]
	add	ip, r2, #8192
	mov	r3, #0
	mov	r7, #1
	mov	r2, #4
	mov	r1, r8
	str	r0, [r4, #4]
	mov	r0, r4
	ldr	lr, [r5, #48]
	ldr	r6, .L409
	str	lr, [r4, #16]
	ldr	lr, [r5, #68]
	str	lr, [r4, #20]
	ldr	lr, [r5, #4]
	str	lr, [r4, #24]
	ldr	lr, [r5, #8]
	str	r7, [r4, #32]
	str	r3, [r4, #36]
	str	lr, [r4, #28]
	str	r3, [r4, #852]
	ldr	r3, [ip, #3148]
	str	r2, [r4, #864]
	str	r3, [r4, #848]
	bl	PostPro_GetMp24MbGroup
	b	.L307
.L285:
	ldr	r3, [r2, #172]
	mov	r7, #0
	mov	r2, #1
	mov	r1, r8
	mov	r0, r4
	ldr	r6, .L409
	str	r3, [r4, #4]
	ldr	r3, [r5, #172]
	str	r3, [r4, #8]
	ldr	r3, [r5, #180]
	str	r3, [r4, #16]
	ldr	r3, [r5, #184]
	str	r3, [r4, #20]
	ldrh	r3, [r5, #52]
	str	r3, [r4, #24]
	ldrh	r3, [r5, #54]
	str	r2, [r4, #32]
	str	r7, [r4, #36]
	str	r3, [r4, #28]
	bl	PostPro_GetVp6MbGroup
	str	r7, [r4, #844]
	str	r7, [r4, #852]
	mov	r2, #4
	ldr	r3, [r5, #192]
	str	r2, [r4, #864]
	str	r3, [r4, #848]
	b	.L307
.L286:
	ldr	ip, [r2, #3116]
	mov	r3, #0
	mov	lr, #1
	mov	r2, #4
	mov	r1, r8
	mov	r0, r4
	str	ip, [r4, #4]
	ldr	ip, [r5, #3116]
	ldr	r6, .L409
	str	ip, [r4, #8]
	ldr	ip, [r5, #3128]
	str	ip, [r4, #16]
	ldr	ip, [r5, #3132]
	str	ip, [r4, #20]
	ldr	ip, [r5, #2784]
	str	ip, [r4, #24]
	ldr	ip, [r5, #2788]
	str	lr, [r4, #32]
	str	r3, [r4, #36]
	str	ip, [r4, #28]
	str	r3, [r4, #852]
	ldr	r3, [r5, #3140]
	str	r2, [r4, #864]
	str	r3, [r4, #848]
	bl	PostPro_GetVp8MbGroup
	b	.L307
.L287:
	ldr	r3, [r2, #292]
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #308]
	str	r3, [r1, #4]
	ldr	r3, [r2, #296]
	str	r3, [r1, #8]
	ldr	r3, [r2, #868]
	ldr	r0, [r3, #1640]
	add	r3, r2, r0, lsl #2
	ldr	r3, [r3, #308]
	str	r3, [r1, #16]
	ldr	r3, [r2, #300]
	add	r3, r3, #142
	ldr	r3, [r2, r3, asl #2]
	str	r3, [r1, #20]
	ldr	r3, [r2, #28]
	str	r3, [r1, #24]
	ldr	r3, [r2, #20]
	str	r3, [r1, #28]
	ldrb	r3, [r2]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r1, #32]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r1, #36]
	ldr	ip, [r2, #860]
	cmp	ip, #0
	beq	.L315
	ldr	r3, [r2, #1064]
	cmp	r3, r0
	addne	r2, r2, #1056
	movne	r3, #0
	addne	r2, r2, #8
	bne	.L318
	b	.L406
.L319:
	ldr	r1, [r2, #4]!
	cmp	r1, r0
	beq	.L316
.L318:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L319
.L315:
	ldr	r2, [r5, #32]
	ldrsb	r3, [r5, #2]
	cmp	r2, #0
	ldrneb	r2, [r5]	@ zero_extendqisi2
	clzne	r2, r2
	movne	r2, r2, lsr #5
	cmp	r3, #0
	streq	r3, [r8, #8]
	bne	.L407
.L322:
	ldr	r0, [r4, #4]
	ldr	r3, [r4, #16]
	cmp	r0, r3
	beq	.L408
.L323:
	ldrb	r3, [r5]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L332
	cmp	r3, #1
	beq	.L395
	cmp	r3, #2
	beq	.L395
	cmp	r3, #3
	bne	.L329
.L332:
	mov	r3, #0
.L395:
	str	r3, [r4, #852]
.L329:
	ldr	r3, [r5, #1128]
	mov	r2, #4
	ldr	r6, .L409
	str	r2, [r4, #864]
	str	r3, [r4, #848]
	b	.L307
.L275:
	ldr	r6, .L409
	movw	r3, #757
	ldr	r2, .L409+12
	mov	r0, #0
	ldr	r1, .L409+16
	ldr	ip, [r6, #68]
	blx	ip
	b	.L307
.L394:
	str	r3, [r4, #852]
	b	.L334
.L405:
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r3, #1
	streq	r3, [r4, #852]
	beq	.L339
	cmp	r3, #1
	moveq	r3, #2
	streq	r3, [r4, #852]
	b	.L339
.L403:
	mov	r3, #0
.L290:
	add	r3, r3, #250
	ldr	r3, [r5, r3, asl #2]
	str	r3, [r4, #844]
	b	.L289
.L406:
	mov	r3, #0
.L316:
	add	r3, r3, #250
	ldr	r3, [r5, r3, asl #2]
	str	r3, [r4, #844]
	b	.L315
.L407:
	mov	r1, r8
	mov	r0, r4
	bl	PostPro_GetH264MbGroup
	b	.L322
.L404:
	mov	r1, r8
	mov	r0, r4
	bl	PostPro_GetH264MbGroup
	b	.L296
.L401:
	ldr	r3, [fp, #4]
	mov	r2, r5
	mov	r1, r8
	mov	r0, r4
	bl	PostPro_GetHevcMbGroup
	b	.L309
.L402:
	ldr	r0, [r5, #1448]
	cmp	r0, #0
	beq	.L310
	ldr	r2, [r5, #1320]
	cmp	r1, r2
	addeq	r5, r5, #1312
	moveq	r3, #0
	addeq	r5, r5, #8
	bne	.L311
.L313:
	add	r3, r3, #1
	cmp	r3, r0
	beq	.L310
	ldr	r2, [r5, #4]!
	cmp	r1, r2
	beq	.L313
.L311:
	str	r2, [r4, #16]
	b	.L310
.L408:
	ldr	ip, [r5, #564]
	cmp	ip, #0
	beq	.L323
	ldr	r2, [r5, #308]
	cmp	r0, r2
	addeq	r1, r5, #308
	moveq	r3, #0
	bne	.L324
.L326:
	add	r3, r3, #1
	cmp	r3, ip
	beq	.L323
	ldr	r2, [r1, #4]!
	cmp	r0, r2
	beq	.L326
	b	.L324
.L410:
	.align	2
.L409:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC8
	.word	.LC9
	.word	.LANCHOR0+24
	.word	.LC7
	UNWIND(.fnend)
	.size	PostPro_GetRepairParam, .-PostPro_GetRepairParam
	.align	2
	.global	PostPro_GetErrRatio
	.type	PostPro_GetErrRatio, %function
PostPro_GetErrRatio:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #24)
	sub	sp, sp, #24
	subs	r3, r0, #0
	beq	.L416
	ldr	r7, .L417
	mov	r2, #12
	mov	r1, #0
	sub	r0, fp, #48
	ldr	r5, [r3, #8]
	mov	r4, #816
	ldr	ip, [r7, #48]
	ldr	r8, [r3]
	ldr	r9, [r3, #4]
	blx	ip
	ldr	r3, .L417+4
	mov	r2, #1744
	ldr	r1, .L417+8
	ldr	r6, .L417+12
	mla	r4, r4, r5, r3
	ldr	r3, [r1, #108]
	str	r8, [fp, #-48]
	cmp	r3, #0
	str	r5, [fp, #-40]
	mla	r6, r2, r5, r6
	str	r4, [fp, #-44]
	beq	.L414
	sub	r0, fp, #48
	blx	r3
.L415:
	mov	r3, r4
	mov	r2, r9
	mov	r1, r6
	str	r5, [sp]
	mov	r0, r8
	bl	PostPro_GetRepairParam
	ldr	r3, [r7, #68]
	ldr	r1, .L417+16
	mov	r4, r0
	mov	r2, r0
	mov	r0, #22
	blx	r3
	mov	r0, r4
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L414:
	mov	r0, r3
	mov	r2, #97
	ldr	r3, [r7, #68]
	ldr	r1, .L417+20
	blx	r3
	b	.L415
.L416:
	ldr	r3, .L417
	ldr	r1, .L417+24
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L418:
	.align	2
.L417:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_DecReport
	.word	g_vdm_hal_fun_ptr
	.word	g_RepairParam
	.word	.LC12
	.word	.LC11
	.word	.LC10
	UNWIND(.fnend)
	.size	PostPro_GetErrRatio, .-PostPro_GetErrRatio
	.align	2
	.global	PostPro_SetFullRepair
	.type	PostPro_SetFullRepair, %function
PostPro_SetFullRepair:
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
	mov	ip, #816
	mul	ip, ip, r2
	ldr	r5, .L420
	ldr	r3, .L420+4
	mov	lr, r2
	mov	r7, #1744
	str	r2, [sp]
	add	r4, r5, ip
	mov	r2, r1
	mov	r6, #1
	mla	r1, r7, lr, r3
	str	r6, [r5, ip]
	mov	r3, r4
	mov	ip, #0
	str	ip, [r4, #8]
	bl	PostPro_GetRepairParam
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L421:
	.align	2
.L420:
	.word	g_DecReport
	.word	g_RepairParam
	UNWIND(.fnend)
	.size	PostPro_SetFullRepair, .-PostPro_SetFullRepair
	.align	2
	.global	PostPro_SetDecReport
	.type	PostPro_SetDecReport, %function
PostPro_SetDecReport:
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
	ldr	r6, .L440
	mov	r5, r0
	mov	r4, r2
	mov	r7, r1
	mov	r0, r2
	mov	r1, #0
	mov	r2, #816
	ldr	r3, [r6, #48]
	blx	r3
	cmp	r5, #17
	ldrls	pc, [pc, r5, asl #2]
	b	.L423
.L425:
	.word	.L434
	.word	.L434
	.word	.L434
	.word	.L434
	.word	.L423
	.word	.L434
	.word	.L430
	.word	.L423
	.word	.L434
	.word	.L434
	.word	.L434
	.word	.L434
	.word	.L434
	.word	.L434
	.word	.L423
	.word	.L423
	.word	.L434
	.word	.L434
.L430:
	ldr	r3, [r7, #32]
	cmp	r3, #1
	beq	.L439
.L434:
	mov	r3, #0
	mov	r2, #1
	strh	r3, [r4, #12]	@ movhi
	str	r2, [r4, #8]
	strh	r3, [r4, #14]	@ movhi
.L437:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L423:
	ldr	ip, [r6, #68]
	movw	r3, #1980
	ldr	r2, .L440+4
	mov	r0, #0
	ldr	r1, .L440+8
	blx	ip
	b	.L437
.L439:
	mov	r3, #0
	mov	r2, #2
	strh	r3, [r4, #12]	@ movhi
	str	r2, [r4, #8]
	strh	r3, [r4, #14]	@ movhi
	ldr	r2, [r7, #168]
	ldr	r3, [r7, #164]
	mul	r3, r3, r2
	mov	r3, r3, lsr #1
	strh	r3, [r4, #16]	@ movhi
	ldr	r2, [r7, #168]
	ldr	r3, [r7, #164]
	mul	r3, r3, r2
	mov	r3, r3, lsr #1
	strh	r3, [r4, #18]	@ movhi
	b	.L437
.L441:
	.align	2
.L440:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+48
	.word	.LC7
	UNWIND(.fnend)
	.size	PostPro_SetDecReport, .-PostPro_SetDecReport
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.13729, %object
	.size	__func__.13729, 23
__func__.13729:
	.ascii	"PostPro_GetHevcMbGroup\000"
	.space	1
	.type	__func__.13682, %object
	.size	__func__.13682, 23
__func__.13682:
	.ascii	"PostPro_GetRepairParam\000"
	.space	1
	.type	__func__.13907, %object
	.size	__func__.13907, 21
__func__.13907:
	.ascii	"PostPro_SetDecReport\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"pRepairParam->ImageWidth=%d,pRepairParam->ImageHeig" )
	ASCII(.ascii	"ht=%d\012\000" )
	.space	2
.LC1:
	ASCII(.ascii	"DecSliceNum(=%d) out of range!\012\000" )
.LC2:
	ASCII(.ascii	"pRepairParam->ValidGroupNum >(MAX_UP_MSG_SLICE_NUM " )
	ASCII(.ascii	"- 1)\012\000" )
	.space	3
.LC3:
	ASCII(.ascii	"%s:0 pRepairParam->ValidGroupNum(%d) > (MAX_UP_MSG_" )
	ASCII(.ascii	"SLICE_NUM - 1)\012\000" )
	.space	1
.LC4:
	ASCII(.ascii	"%s:1 pRepairParam->ValidGroupNum(%d) > (MAX_UP_MSG_" )
	ASCII(.ascii	"SLICE_NUM - 1)\012\000" )
	.space	1
.LC5:
	ASCII(.ascii	"line:%d! pRepairParam->ValidGroupNum > (MAX_UP_MSG_" )
	ASCII(.ascii	"SLICE_NUM - 1)\012\000" )
	.space	1
.LC6:
	ASCII(.ascii	"DecSliceNum out of range!\012\000" )
	.space	1
.LC7:
	ASCII(.ascii	"%s %d VidStd Err!\012\000" )
	.space	1
.LC8:
	ASCII(.ascii	"pRepairParam[0].MbGroup[i].EndMbn=%d,pRepairParam[0" )
	ASCII(.ascii	"].MbGroup[i].StartMbn=%d\012\000" )
	.space	3
.LC9:
	ASCII(.ascii	"total_mbs=%d,err_mbs=%d\012\000" )
	.space	3
.LC10:
	ASCII(.ascii	"PostPro_GetErrRatio: pParam = NULL\012\000" )
.LC11:
	ASCII(.ascii	"VDMHAL_NULL_FUN_PRINT,L%d\012\000" )
	.space	1
.LC12:
	ASCII(.ascii	"err_ratio=%d\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
