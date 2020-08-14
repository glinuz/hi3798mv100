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
	cmp	r0, #0
	beq	.L4
	cmp	r0, #15
	beq	.L4
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L4:
	ldrb	r0, [r1, #3]	@ zero_extendqisi2
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	PostPro_GetRepairStratage, .-PostPro_GetRepairStratage
	.align	2
	.global	PostPro_CopyYuv
	.type	PostPro_CopyYuv, %function
PostPro_CopyYuv:
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
	ldr	ip, .L12
	movw	lr, #1752
	add	r5, r0, #12288
	ldr	r3, .L12+4
	mov	r8, #4
	mla	ip, lr, r1, ip
	ldr	r4, [r5, #2316]
	ldr	lr, [r5, #2304]
	ldr	r7, [r3, #140]
	ldr	r10, [r5, #2384]
	ldr	r9, [r5, #2392]
	add	r5, ip, #848
	str	r4, [ip, #12]
	cmp	r7, #0
	str	lr, [ip, #4]
	mov	r4, #1
	str	lr, [ip, #8]
	ldr	lr, [r0, #220]
	str	r5, [fp, #-48]
	mov	r5, #0
	ldr	r3, [fp, #-48]
	str	lr, [ip, #16]
	ldr	lr, [r0, #228]
	str	lr, [ip, #20]
	ldrh	lr, [r0, #164]
	str	lr, [ip, #24]
	ldrh	r6, [r0, #166]
	moveq	r0, r4
	str	r10, [ip, #848]
	str	r9, [ip, #872]
	smulbb	lr, r6, lr
	str	r6, [ip, #28]
	str	r8, [ip, #864]
	str	r5, [ip, #36]
	sub	lr, lr, #1
	strh	r5, [ip, #40]	@ movhi
	strh	lr, [ip, #42]	@ movhi
	str	r5, [r3, #4]
	str	r4, [ip, #32]
	str	r4, [ip, #840]
	str	r4, [ip, #856]
	beq	.L10
	str	r2, [sp]
	mov	r3, r1
	mov	r2, r5
	mov	r1, r0
	mov	r0, #2
	blx	r7
	subs	r0, r0, r5
	movne	r0, #1
.L10:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L13:
	.align	2
.L12:
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
	UNWIND(.pad #20)
	sub	sp, sp, #20
	ldrh	r3, [r0, #74]
	str	r2, [fp, #-52]
	mov	r8, r0
	sub	r2, r3, #1
	mov	r10, r1
	cmp	r2, #2048
	movcc	r3, #16
	strcc	r3, [fp, #-48]
	bcc	.L15
	sub	r2, r3, #2048
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r3, #32
	strcc	r3, [fp, #-48]
	bcs	.L22
.L15:
	ldr	r3, .L23
	movw	r4, #1752
	add	r5, r8, #12288
	mov	r7, #0
	mov	r6, #1
	ldr	r1, .L23+4
	mla	r4, r4, r10, r3
	ldr	ip, [r5, #96]
	ldr	lr, [r5, #264]
	mov	r0, #22
	ldr	r3, [r5, #268]
	ldr	r2, [r5, #224]
	str	ip, [r4, #8]
	ldr	ip, [r5, #100]
	str	r2, [r4, #4]
	mov	r2, lr
	str	r3, [r4, #28]
	str	ip, [r4, #16]
	ldr	ip, [r5, #108]
	str	lr, [r4, #24]
	str	r7, [r4, #36]
	str	ip, [r4, #20]
	str	r6, [r4, #32]
	bl	dprint_vfmw
	ldr	lr, [r4, #28]
	ldr	r9, [r4, #24]
	mov	r1, #4
	ldr	r0, .L23+8
	ldr	ip, [r5, #256]
	smulbb	lr, lr, r9
	ldr	r5, [r0, #140]
	ldr	r0, [fp, #-48]
	str	ip, [r4, #848]
	cmp	r5, r7
	str	r6, [r4, #840]
	sub	lr, lr, #1
	str	r0, [r4, #860]
	moveq	r0, r6
	strh	r7, [r4, #40]	@ movhi
	str	r6, [r4, #856]
	strh	lr, [r4, #42]	@ movhi
	str	r1, [r4, #864]
	str	r7, [r4, #844]
	str	r7, [r4, #852]
	ldrb	r3, [r8, #71]	@ zero_extendqisi2
	str	r3, [r4, #872]
	beq	.L16
	ldr	r3, [fp, #-52]
	mov	r2, r7
	mov	r1, r8
	mov	r0, r6
	str	r3, [sp]
	mov	r3, r10
	blx	r5
	subs	r0, r0, r7
	movne	r0, #1
.L16:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L22:
	sub	r2, r3, #4096
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r3, #48
	strcc	r3, [fp, #-48]
	bcc	.L15
	sub	r3, r3, #6144
	sub	r3, r3, #1
	cmp	r3, #2048
	movcs	r3, #16
	movcc	r3, #64
	str	r3, [fp, #-48]
	b	.L15
.L24:
	.align	2
.L23:
	.word	g_RepairParam
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r3, .L28
	movw	r4, #1752
	ldr	ip, [r0, #176]
	mov	r5, r0
	mov	r8, r2
	mla	r4, r4, r1, r3
	mov	r7, #0
	mov	r6, #1
	mov	r9, r1
	mov	r0, #22
	ldr	r1, .L28+4
	str	ip, [r4, #4]
	ldr	r2, [r5, #176]
	str	r2, [r4, #8]
	ldr	r2, [r5, #184]
	str	r2, [r4, #16]
	ldr	r2, [r5, #188]
	str	r2, [r4, #20]
	ldrh	r2, [r5, #54]
	str	r2, [r4, #24]
	ldrh	ip, [r5, #56]
	str	r7, [r4, #36]
	str	r6, [r4, #32]
	mov	r3, ip
	str	ip, [r4, #28]
	bl	dprint_vfmw
	ldr	r3, [r4, #24]
	ldr	r0, [r4, #28]
	mov	r1, #4
	ldr	ip, .L28+8
	add	lr, r4, #840
	str	r6, [r4, #840]
	smulbb	r0, r0, r3
	strh	r7, [r4, #40]	@ movhi
	str	r7, [r4, #844]
	ldr	ip, [ip, #140]
	sub	r0, r0, #1
	strh	r0, [r4, #42]	@ movhi
	ldr	r3, [r5, #208]
	cmp	ip, r7
	str	r6, [r4, #856]
	str	r1, [r4, #864]
	moveq	r0, r6
	str	r3, [r4, #848]
	str	r7, [r4, #852]
	ldrb	r3, [r5, #52]	@ zero_extendqisi2
	str	r3, [r4, #872]
	beq	.L26
	str	r8, [sp]
	mov	r3, r9
	mov	r2, r7
	mov	r1, r5
	mov	r0, #10
	blx	ip
	subs	r0, r0, r7
	movne	r0, #1
.L26:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L29:
	.align	2
.L28:
	.word	g_RepairParam
	.word	.LC0
	.word	g_vdm_hal_fun_ptr
	UNWIND(.fnend)
	.size	PostPro_CopyYuv_Vp6, .-PostPro_CopyYuv_Vp6
	.align	2
	.global	PostPro_CopyYuv_Vp9
	.type	PostPro_CopyYuv_Vp9, %function
PostPro_CopyYuv_Vp9:
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
	ldr	r3, .L33
	movw	r4, #1752
	add	r5, r0, #266240
	str	r2, [fp, #-48]
	mov	r10, r1
	mla	r4, r4, r1, r3
	ldr	r6, [r5, #1576]
	ldr	r1, [r5, #1580]
	mov	r7, #0
	ldr	r2, [r5, #1928]
	mov	r9, r0
	ldr	r3, [r5, #1936]
	mov	r0, #22
	ldr	ip, [r5, #1920]
	str	r2, [r4, #16]
	mov	r2, r6
	str	r3, [r4, #20]
	mov	r3, r1
	str	r6, [r4, #24]
	mov	r6, #1
	str	r1, [r4, #28]
	str	ip, [r4, #4]
	ldr	r1, .L33+4
	str	ip, [r4, #8]
	str	r7, [r4, #36]
	str	r6, [r4, #32]
	bl	dprint_vfmw
	ldr	r8, [r4, #28]
	ldr	lr, .L33+8
	mov	r0, #6
	ldr	r3, [r4, #24]
	str	r0, [r4, #864]
	str	r6, [r4, #840]
	smulbb	r3, r8, r3
	ldr	r8, [lr, #140]
	ldr	lr, [r5, #1948]
	ldrb	r5, [r5, #1569]	@ zero_extendqisi2
	cmp	r8, r7
	sub	r3, r3, #1
	strh	r7, [r4, #40]	@ movhi
	str	r6, [r4, #856]
	moveq	r0, r6
	strh	r3, [r4, #42]	@ movhi
	str	lr, [r4, #848]
	str	r5, [r4, #872]
	str	r7, [r4, #844]
	str	r7, [r4, #852]
	beq	.L31
	ldr	r3, [fp, #-48]
	mov	r2, r7
	mov	r1, r9
	mov	r0, #17
	str	r3, [sp]
	mov	r3, r10
	blx	r8
	subs	r0, r0, r7
	movne	r0, #1
.L31:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L34:
	.align	2
.L33:
	.word	g_RepairParam
	.word	.LC0
	.word	g_vdm_hal_fun_ptr
	UNWIND(.fnend)
	.size	PostPro_CopyYuv_Vp9, .-PostPro_CopyYuv_Vp9
	.align	2
	.global	PostPro_GetHevcMbGroupOfTile
	.type	PostPro_GetHevcMbGroupOfTile, %function
PostPro_GetHevcMbGroupOfTile:
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
	ldr	ip, .L70
	mov	lr, r3, asl #6
	mov	r5, r1
	sub	r3, lr, r3, asl #3
	ldr	r8, .L70+4
	add	ip, ip, r3
	ldr	r4, [r0, #28]
	ldr	r3, [r0, #24]
	mov	r6, #0
	ldr	r1, [ip, #8]
	mov	r7, r0
	str	r6, [r0, #840]
	str	r6, [r0, #856]
	ldr	r0, [r8, r1, asl #2]
	mul	r4, r4, r3
	add	r0, r0, #294912
	add	r0, r0, #1968
	str	r2, [fp, #-48]
	bl	HEVC_CalcTileMap
	ldr	r3, [r5, #8]
	cmp	r3, r6
	beq	.L42
	mov	lr, r6
	ldr	r8, .L70+8
	ldr	r6, .L70+12
	mov	r0, r5
	mov	r1, #1
.L41:
	ldrh	ip, [r0, #12]
	ldrh	r2, [r0, #14]
	cmp	ip, r2
	mov	r3, ip
	bls	.L40
	b	.L39
.L68:
	ldrh	ip, [r0, #12]
.L40:
	cmp	r4, ip
	blt	.L43
	cmp	r4, r2
	ldrge	r2, [r6, r3, asl #2]
	strgeb	r1, [r8, r2]
.L43:
	ldrh	r2, [r0, #14]
	add	r3, r3, #1
	cmp	r2, r3
	bcs	.L68
.L39:
	ldr	r3, [r5, #8]
	add	lr, lr, #1
	add	r0, r0, #4
	cmp	r3, lr
	bhi	.L41
.L42:
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #2564]
	cmp	r3, #0
	beq	.L38
	ldr	r6, [fp, #-48]
	mov	r10, #0
	ldr	lr, .L70+8
	mov	r9, r10
	str	r4, [fp, #-48]
	mov	r8, r6
.L45:
	ldr	r0, [r8, #3680]
	ldr	ip, [r8, #3684]
	cmp	r0, ip
	bhi	.L50
	ldr	r3, .L70+12
	mov	r4, r0, asl #2
	ldr	r5, [r3, r0, asl #2]
	ldrb	r3, [lr, r5]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r2, .L70+12
	movne	r3, r0
	addne	r2, r2, r4
	bne	.L49
	b	.L48
.L51:
	ldr	r1, [r2, #4]!
	ldrb	r1, [lr, r1]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L48
.L49:
	add	r3, r3, #1
	cmp	r3, ip
	bls	.L51
.L50:
	ldr	r3, [r6, #2564]
	add	r10, r10, #1
	add	r8, r8, #8
	cmp	r3, r10
	bhi	.L45
	ldr	r4, [fp, #-48]
.L38:
	ldr	lr, .L70+8
	mov	ip, #0
.L57:
	cmp	ip, r4
	bcs	.L58
	ldrb	r3, [lr, ip]	@ zero_extendqisi2
	cmp	r3, #1
	addeq	ip, ip, #1
	ldreq	r0, [r7, #840]
	beq	.L53
	add	r1, ip, lr
	mov	r2, ip
	b	.L55
.L54:
	ldrb	r0, [r1, #1]!	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L56
	mov	r2, r3
.L55:
	add	r3, r2, #1
	cmp	r4, r3
	bhi	.L54
.L56:
	ldr	r1, [r7, #840]
	add	r0, r1, #1
	add	r1, r7, r1, lsl #2
	strh	ip, [r1, #40]	@ movhi
	mov	ip, r3
	strh	r2, [r1, #42]	@ movhi
	str	r0, [r7, #840]
.L53:
	cmp	r0, #199
	ble	.L57
	ldr	r1, .L70+16
	mov	r0, #1
	bl	dprint_vfmw
	sub	r4, r4, #1
	mov	r3, #0
	strh	r4, [r7, #42]	@ movhi
	strh	r3, [r7, #40]	@ movhi
	mov	r3, #1
	str	r3, [r7, #840]
.L58:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L48:
	ldr	r3, .L70+12
	add	r4, r3, r4
	b	.L47
.L69:
	ldr	r5, [r4, #4]!
.L47:
	strb	r9, [lr, r5]
	add	r0, r0, #1
	ldr	r3, [r8, #3684]
	cmp	r3, r0
	bcs	.L69
	b	.L50
.L71:
	.align	2
.L70:
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	g_IsDeced
	.word	g_TsToRsMap
	.word	.LC1
	UNWIND(.fnend)
	.size	PostPro_GetHevcMbGroupOfTile, .-PostPro_GetHevcMbGroupOfTile
	.align	2
	.global	PostPro_GetHevcMbGroupOfNoTile
	.type	PostPro_GetHevcMbGroupOfNoTile, %function
PostPro_GetHevcMbGroupOfNoTile:
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
	ldr	r2, .L90
	mov	ip, r3, asl #6
	mov	r9, r0
	sub	r3, ip, r3, asl #3
	ldr	r0, [r0, #24]
	add	r2, r2, r3
	ldr	lr, .L90+4
	ldr	r5, [r9, #28]
	mov	r8, r1
	ldr	r3, [r2, #8]
	mov	r4, #0
	str	r4, [r9, #840]
	mvn	r6, #0
	mul	r5, r5, r0
	ldr	r0, [lr, r3, asl #2]
	str	r4, [r9, #856]
	mov	r7, r6
	add	r0, r0, #294912
	add	r0, r0, #1968
	bl	HEVC_CalcTileMap
	ldr	r3, [r8, #8]
	sub	r2, r5, #1
	uxth	r10, r5
	str	r2, [fp, #-48]
	mov	lr, r4
	add	r1, r8, r3, lsl #2
	mov	r2, r8
	mov	r3, r1
	strh	r10, [r1, #14]	@ movhi
	strh	r10, [r1, #12]	@ movhi
	b	.L81
.L88:
	cmp	r0, r6
	movlt	r6, r7
	add	r3, r6, #1
	cmp	r0, r3
	ble	.L75
	ldr	r3, [r9, #840]
	cmp	r3, #199
	bgt	.L85
	add	r4, r7, r7, lsr #31
	sub	r1, r6, #100
	add	r6, r9, r3, lsl #2
	add	r3, r3, #1
	sxth	r7, r4
	strh	r4, [r6, #40]	@ movhi
	cmp	r1, r7
	sub	r4, r0, #1
	strgth	r1, [r6, #40]	@ movhi
	strh	r4, [r6, #42]	@ movhi
	str	r3, [r9, #840]
.L75:
	ldr	r3, [r8, #8]
	sub	r1, r3, #1
	cmp	lr, r1
	beq	.L86
.L83:
	add	lr, lr, #1
	mov	r6, ip
	cmp	lr, r3
	mov	r7, r0
	add	r2, r2, #4
	bhi	.L87
.L81:
	ldrh	r0, [r2, #12]
	ldrh	ip, [r2, #14]
	cmp	r0, r5
	cmple	ip, r5
	movgt	r3, #1
	movle	r3, #0
	cmp	r0, ip
	orrgt	r3, r3, #1
	cmp	r3, #0
	beq	.L88
	ldr	r3, [r8, #8]
	add	lr, lr, #1
	add	r2, r2, #4
	cmp	lr, r3
	bls	.L81
.L87:
	mov	r0, #0
.L77:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L86:
	ldrh	r1, [r2, #14]
	ldr	r4, [fp, #-48]
	cmp	r1, r4
	bge	.L83
	ldr	r3, [r9, #840]
	cmp	r3, #199
	bgt	.L89
	add	r2, r9, r3, lsl #2
	sub	r10, r10, #1
	add	r3, r3, #1
	mov	r0, #0
	strh	r10, [r2, #42]	@ movhi
	strh	r1, [r2, #40]	@ movhi
	str	r3, [r9, #840]
	b	.L77
.L85:
	ldr	r2, .L90+8
	mov	r0, #1
	ldr	r1, .L90+12
	bl	dprint_vfmw
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L89:
	ldr	r2, .L90+8
	mov	r0, #1
	ldr	r1, .L90+16
	bl	dprint_vfmw
	mov	r0, #1
	b	.L77
.L91:
	.align	2
.L90:
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	.LANCHOR0
	.word	.LC2
	.word	.LC3
	UNWIND(.fnend)
	.size	PostPro_GetHevcMbGroupOfNoTile, .-PostPro_GetHevcMbGroupOfNoTile
	.align	2
	.global	PostPro_GetHevcMbGroup
	.type	PostPro_GetHevcMbGroup, %function
PostPro_GetHevcMbGroup:
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
	ldr	ip, [r1, #8]
	mov	r4, r0
	sub	lr, ip, #1
	cmp	lr, #199
	bhi	.L97
	ldr	ip, [r0, #868]
	cmp	ip, #0
	bne	.L98
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	PostPro_GetHevcMbGroupOfNoTile
.L98:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	PostPro_GetHevcMbGroupOfTile
.L97:
	mov	r2, ip
	ldr	r1, .L99
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L100:
	.align	2
.L99:
	.word	.LC4
	UNWIND(.fnend)
	.size	PostPro_GetHevcMbGroup, .-PostPro_GetHevcMbGroup
	.align	2
	.global	PostPro_GetHEVCRepairParam
	.type	PostPro_GetHEVCRepairParam, %function
PostPro_GetHEVCRepairParam:
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
	mov	r4, r1
	ldr	r1, [r1, #1280]
	mov	r6, #1
	mov	r5, r0
	add	r1, r1, #332
	add	r1, r1, #2
	ldr	r1, [r4, r1, asl #2]
	str	r1, [r0, #4]
	ldr	r1, [r4, #2788]
	str	r1, [r0, #8]
	ldr	r1, [r4, #2568]
	ldr	r1, [r1, #372]
	add	r1, r1, #332
	add	r1, r1, #2
	ldr	r1, [r4, r1, asl #2]
	str	r1, [r0, #16]
	ldr	r1, [r4, #1284]
	add	r1, r4, r1, lsl #2
	ldr	r1, [r1, #1468]
	str	r1, [r0, #20]
	ldr	r1, [r4, #68]
	mov	ip, r6, asl r1
	str	r1, [r0, #864]
	ldr	lr, [r4, #1316]
	str	lr, [r0, #848]
	ldr	lr, [r4, #188]
	sub	lr, lr, #1
	add	lr, lr, ip
	mov	lr, lr, lsr r1
	str	lr, [r0, #24]
	ldr	lr, [r4, #184]
	sub	lr, lr, #1
	add	ip, lr, ip
	mov	ip, ip, lsr r1
	str	ip, [r0, #28]
	ldrb	r1, [r4, #217]	@ zero_extendqisi2
	str	r6, [r0, #32]
	str	r1, [r0, #868]
	ldr	r1, [r4, #8]
	clz	r1, r1
	mov	r1, r1, lsr #5
	str	r1, [r0, #36]
	ldr	r1, [r4, #1332]
	cmp	r1, #0
	streq	r1, [r2, #8]
	bne	.L114
.L103:
	ldr	r1, [r5, #4]
	ldr	r3, [r5, #16]
	cmp	r1, r3
	beq	.L115
.L104:
	mov	r3, #0
	str	r3, [r5, #852]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L115:
	ldr	r0, [r4, #1464]
	cmp	r0, #0
	beq	.L104
	ldr	r2, [r4, #1336]
	cmp	r1, r2
	addeq	r4, r4, #1328
	moveq	r3, #0
	addeq	r4, r4, #8
	bne	.L105
.L107:
	add	r3, r3, #1
	cmp	r3, r0
	beq	.L104
	ldr	r2, [r4, #4]!
	cmp	r1, r2
	beq	.L107
.L105:
	str	r2, [r5, #16]
	b	.L104
.L114:
	mov	r1, r2
	mov	r2, r4
	bl	PostPro_GetHevcMbGroup
	b	.L103
	UNWIND(.fnend)
	.size	PostPro_GetHEVCRepairParam, .-PostPro_GetHEVCRepairParam
	.align	2
	.global	PostPro_GetVp9MbGroup
	.type	PostPro_GetVp9MbGroup, %function
PostPro_GetVp9MbGroup:
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
	bhi	.L129
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
	b	.L125
.L133:
	cmp	ip, r6
	movlt	r6, r7
	add	r6, r6, #1
	cmp	r6, ip
	beq	.L121
	add	r7, r7, r7, lsr #31
	sub	r6, ip, #1
	cmp	r7, r6
	bge	.L121
	ldr	r2, [r0, #840]
	cmp	r2, #199
	bgt	.L130
	add	r10, r0, r2, lsl #2
	add	r2, r2, #1
	strh	r7, [r10, #40]	@ movhi
	strh	r6, [r10, #42]	@ movhi
	str	r2, [r0, #840]
.L121:
	ldr	r2, [r1, #8]
	sub	r6, r2, #1
	cmp	r6, lr
	beq	.L131
.L127:
	add	lr, lr, #1
	mov	r6, r4
	cmp	lr, r2
	mov	r7, ip
	add	r3, r3, #4
	bhi	.L132
.L125:
	ldrh	ip, [r3, #12]
	ldrh	r4, [r3, #14]
	cmp	ip, r5
	cmple	r4, r5
	movgt	r2, #1
	movle	r2, #0
	cmp	ip, r4
	orrgt	r2, r2, #1
	cmp	r2, #0
	beq	.L133
	ldr	r2, [r1, #8]
	add	lr, lr, #1
	add	r3, r3, #4
	cmp	lr, r2
	bls	.L125
.L132:
	mov	r3, #0
.L118:
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L131:
	add	r6, r1, lr, lsl #2
	ldrh	r6, [r6, #14]
	cmp	r6, r9
	bge	.L127
	ldr	r3, [r0, #840]
	cmp	r3, #199
	bgt	.L134
	add	r1, r0, r3, lsl #2
	add	r2, r3, #1
	sub	r8, r8, #1
	mov	r3, #0
	strh	r8, [r1, #42]	@ movhi
	strh	r6, [r1, #40]	@ movhi
	str	r2, [r0, #840]
	b	.L118
.L129:
	ldr	r1, .L135
	mov	r0, #0
	bl	dprint_vfmw
	mov	r3, #1
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L130:
	movw	r2, #1198
	ldr	r1, .L135+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r3, #1
	b	.L118
.L134:
	movw	r2, #1213
	ldr	r1, .L135+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r3, #1
	b	.L118
.L136:
	.align	2
.L135:
	.word	.LC5
	.word	.LC6
	UNWIND(.fnend)
	.size	PostPro_GetVp9MbGroup, .-PostPro_GetVp9MbGroup
	.align	2
	.global	PostPro_GetVP9RepairParam
	.type	PostPro_GetVP9RepairParam, %function
PostPro_GetVP9RepairParam:
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
	add	ip, r1, #266240
	mov	r1, r2
	mov	lr, #0
	mov	r5, #1
	ldr	r6, [ip, #1920]
	mov	r4, #6
	str	r6, [r0, #4]
	ldr	r2, [ip, #1920]
	str	r2, [r0, #8]
	ldr	r2, [ip, #1928]
	str	r2, [r0, #16]
	ldr	r2, [ip, #1936]
	str	r2, [r0, #20]
	ldr	r2, [ip, #1576]
	str	r2, [r0, #24]
	ldr	r2, [ip, #1580]
	str	r5, [r0, #32]
	str	lr, [r0, #36]
	str	r2, [r0, #28]
	str	lr, [r0, #852]
	ldr	r2, [ip, #1948]
	str	r4, [r0, #864]
	str	r2, [r0, #848]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	PostPro_GetVp9MbGroup
	UNWIND(.fnend)
	.size	PostPro_GetVP9RepairParam, .-PostPro_GetVP9RepairParam
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
	bhi	.L158
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
	bne	.L159
.L141:
	mvn	r6, #0
	mov	r3, r1
	add	r2, r4, r6
	mov	r5, r6
	mov	ip, #0
	str	r2, [fp, #-48]
	b	.L150
.L163:
	cmp	r2, lr
	movle	r7, #0
	movgt	r7, #1
	cmp	r2, r5
	orrlt	r7, r7, #1
	cmp	r7, #0
	bne	.L142
	cmp	r2, r6
	movlt	r6, r5
	add	r7, r6, #1
	cmp	r7, r2
	beq	.L146
	add	r5, r5, r5, lsr #31
	sub	r8, r2, #1
	cmp	r5, r8
	bge	.L146
	ldr	r7, [r0, #840]
	cmp	r7, #199
	bgt	.L160
	sub	r6, r6, #100
	sxth	r9, r5
	cmp	r6, r9
	add	r9, r0, r7, lsl #2
	add	r7, r7, #1
	strh	r5, [r9, #40]	@ movhi
	strh	r8, [r9, #42]	@ movhi
	strgth	r6, [r9, #40]	@ movhi
	str	r7, [r0, #840]
.L146:
	ldr	r7, [r1, #8]
	sub	r5, r7, #1
	cmp	r5, ip
	beq	.L161
.L152:
	add	ip, ip, #1
	mov	r6, lr
	cmp	ip, r7
	mov	r5, r2
	add	r3, r3, #4
	bhi	.L162
.L150:
	ldrh	r2, [r3, #12]
	ldrh	lr, [r3, #14]
	cmp	r2, r4
	cmple	lr, r4
	ble	.L163
.L142:
	ldr	r7, [r1, #8]
	add	ip, ip, #1
	add	r3, r3, #4
	cmp	ip, r7
	bls	.L150
.L162:
	mov	r3, #0
.L157:
	mov	r0, r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L161:
	add	r5, r1, ip, lsl #2
	ldr	r6, [fp, #-48]
	ldrh	r5, [r5, #14]
	cmp	r5, r6
	bge	.L152
	ldr	r3, [r0, #840]
	cmp	r3, #199
	bgt	.L164
	add	r1, r0, r3, lsl #2
	add	r2, r3, #1
	sub	r10, r10, #1
	mov	r3, #0
	strh	r10, [r1, #42]	@ movhi
	strh	r5, [r1, #40]	@ movhi
	str	r2, [r0, #840]
	b	.L157
.L159:
	mov	r3, ip
	ldrh	ip, [ip, #10]
	sub	r2, r4, #10
	add	r3, r3, #6
	cmp	ip, r2
	subge	r2, r10, #1
	strgeh	r2, [r3, #4]	@ movhi
	b	.L141
.L158:
	ldr	r1, .L165
	mov	r0, #0
	bl	dprint_vfmw
	mov	r3, #1
	mov	r0, r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L160:
	movw	r2, #1560
	ldr	r1, .L165+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r3, #1
	b	.L157
.L164:
	movw	r2, #1591
	ldr	r1, .L165+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r3, #1
	b	.L157
.L166:
	.align	2
.L165:
	.word	.LC4
	.word	.LC6
	UNWIND(.fnend)
	.size	PostPro_GetMp24MbGroup, .-PostPro_GetMp24MbGroup
	.align	2
	.global	PostPro_GetMPEG2RepairParam
	.type	PostPro_GetMPEG2RepairParam, %function
PostPro_GetMPEG2RepairParam:
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
	mov	r3, r0
	ldr	r0, [r1, #200]
	mov	ip, #0
	str	r0, [r3, #4]
	ldr	r0, [r1, #224]
	str	r0, [r3, #8]
	ldr	r0, [r1, #196]
	str	r0, [r3, #16]
	ldr	r0, [r1, #260]
	str	r0, [r3, #20]
	ldrh	r0, [r1, #152]
	str	r0, [r3, #24]
	ldrh	r0, [r1, #148]
	str	r0, [r3, #28]
	ldrb	r0, [r1, #3]	@ zero_extendqisi2
	str	ip, [r3, #36]
	sub	r0, r0, #3
	clz	r0, r0
	mov	r0, r0, lsr #5
	str	r0, [r3, #32]
	ldrb	r0, [r1, #3]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L171
	cmp	r0, #2
	beq	.L171
	cmp	r0, #3
	streq	ip, [r3, #852]
.L169:
	add	ip, r1, #45056
	mov	r1, r2
	mov	lr, #4
	mov	r0, r3
	ldr	r2, [ip, #280]
	str	lr, [r3, #864]
	str	r2, [r3, #848]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	PostPro_GetMp24MbGroup
.L171:
	str	r0, [r3, #852]
	b	.L169
	UNWIND(.fnend)
	.size	PostPro_GetMPEG2RepairParam, .-PostPro_GetMPEG2RepairParam
	.align	2
	.global	PostPro_GetMPEG4RepairParam
	.type	PostPro_GetMPEG4RepairParam, %function
PostPro_GetMPEG4RepairParam:
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
	ldr	r7, [r1, #212]
	add	lr, r1, #12288
	mov	ip, r1
	mov	r1, r2
	mov	r4, #0
	mov	r6, #1
	str	r7, [r0, #4]
	mov	r5, #4
	ldr	r2, [lr, #2304]
	str	r2, [r0, #8]
	ldr	r2, [ip, #220]
	str	r2, [r0, #16]
	ldr	r2, [ip, #228]
	str	r2, [r0, #20]
	ldrh	r2, [ip, #164]
	str	r2, [r0, #24]
	ldrh	r2, [ip, #166]
	str	r6, [r0, #32]
	str	r4, [r0, #36]
	str	r2, [r0, #28]
	str	r4, [r0, #852]
	ldr	r2, [lr, #2384]
	str	r5, [r0, #864]
	str	r2, [r0, #848]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	PostPro_GetMp24MbGroup
	UNWIND(.fnend)
	.size	PostPro_GetMPEG4RepairParam, .-PostPro_GetMPEG4RepairParam
	.align	2
	.global	PostPro_GetREAL8RepairParam
	.type	PostPro_GetREAL8RepairParam, %function
PostPro_GetREAL8RepairParam:
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
	ldr	r6, [r1, #56]
	mov	ip, r1
	mov	r1, r2
	mov	lr, #0
	mov	r5, #1
	mov	r4, #4
	str	r6, [r0, #4]
	ldr	r2, [ip, #52]
	str	r2, [r0, #16]
	ldr	r2, [ip, #72]
	str	r2, [r0, #20]
	ldr	r2, [ip, #4]
	str	r2, [r0, #24]
	ldr	r2, [ip, #8]
	str	r5, [r0, #32]
	str	lr, [r0, #36]
	str	r2, [r0, #28]
	str	lr, [r0, #852]
	ldr	r2, [ip, #80]
	str	r4, [r0, #864]
	str	r2, [r0, #848]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	PostPro_GetMp24MbGroup
	UNWIND(.fnend)
	.size	PostPro_GetREAL8RepairParam, .-PostPro_GetREAL8RepairParam
	.align	2
	.global	PostPro_GetREAL9RepairParam
	.type	PostPro_GetREAL9RepairParam, %function
PostPro_GetREAL9RepairParam:
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
	ldr	r6, [r1, #52]
	mov	ip, r1
	mov	r1, r2
	mov	lr, #0
	mov	r5, #1
	mov	r4, #4
	str	r6, [r0, #4]
	ldr	r2, [ip, #48]
	str	r2, [r0, #16]
	ldr	r2, [ip, #68]
	str	r2, [r0, #20]
	ldr	r2, [ip, #4]
	str	r2, [r0, #24]
	ldr	r2, [ip, #8]
	str	r5, [r0, #32]
	str	lr, [r0, #36]
	str	r2, [r0, #28]
	str	lr, [r0, #852]
	ldr	r2, [ip, #76]
	str	r4, [r0, #864]
	str	r2, [r0, #848]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	PostPro_GetMp24MbGroup
	UNWIND(.fnend)
	.size	PostPro_GetREAL9RepairParam, .-PostPro_GetREAL9RepairParam
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
	bhi	.L188
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
	b	.L184
.L192:
	cmp	ip, r6
	movlt	r6, r7
	add	r6, r6, #1
	cmp	r6, ip
	beq	.L180
	add	r7, r7, r7, lsr #31
	sub	r6, ip, #1
	cmp	r7, r6
	bge	.L180
	ldr	r2, [r0, #840]
	cmp	r2, #199
	bgt	.L189
	add	r10, r0, r2, lsl #2
	add	r2, r2, #1
	strh	r7, [r10, #40]	@ movhi
	strh	r6, [r10, #42]	@ movhi
	str	r2, [r0, #840]
.L180:
	ldr	r2, [r1, #8]
	sub	r6, r2, #1
	cmp	r6, lr
	beq	.L190
.L186:
	add	lr, lr, #1
	mov	r6, r4
	cmp	lr, r2
	mov	r7, ip
	add	r3, r3, #4
	bhi	.L191
.L184:
	ldrh	ip, [r3, #12]
	ldrh	r4, [r3, #14]
	cmp	ip, r5
	cmple	r4, r5
	movgt	r2, #1
	movle	r2, #0
	cmp	ip, r4
	orrgt	r2, r2, #1
	cmp	r2, #0
	beq	.L192
	ldr	r2, [r1, #8]
	add	lr, lr, #1
	add	r3, r3, #4
	cmp	lr, r2
	bls	.L184
.L191:
	mov	r3, #0
.L177:
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L190:
	add	r6, r1, lr, lsl #2
	ldrh	r6, [r6, #14]
	cmp	r6, r9
	bge	.L186
	ldr	r3, [r0, #840]
	cmp	r3, #199
	bgt	.L193
	add	r1, r0, r3, lsl #2
	add	r2, r3, #1
	sub	r8, r8, #1
	mov	r3, #0
	strh	r8, [r1, #42]	@ movhi
	strh	r6, [r1, #40]	@ movhi
	str	r2, [r0, #840]
	b	.L177
.L188:
	ldr	r1, .L194
	mov	r0, #0
	bl	dprint_vfmw
	mov	r3, #1
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L189:
	movw	r2, #1655
	ldr	r1, .L194+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r3, #1
	b	.L177
.L193:
	movw	r2, #1675
	ldr	r1, .L194+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r3, #1
	b	.L177
.L195:
	.align	2
.L194:
	.word	.LC5
	.word	.LC6
	UNWIND(.fnend)
	.size	PostPro_GetVc1MbGroup, .-PostPro_GetVc1MbGroup
	.align	2
	.global	PostPro_GetVC1RepairParam
	.type	PostPro_GetVC1RepairParam, %function
PostPro_GetVC1RepairParam:
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
	add	r6, r1, #12288
	mov	r5, r1
	mov	r7, #0
	mov	r4, r0
	ldr	r3, [r6, #224]
	mov	r1, r2
	str	r3, [r0, #4]
	ldr	r3, [r6, #96]
	str	r3, [r0, #8]
	ldr	r3, [r6, #100]
	str	r3, [r0, #16]
	ldr	r3, [r6, #108]
	str	r3, [r0, #20]
	ldrh	r3, [r5, #78]
	add	r3, r3, #1
	str	r3, [r0, #24]
	ldrh	r3, [r5, #80]
	add	r3, r3, #1
	str	r3, [r0, #28]
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	str	r7, [r0, #36]
	tst	r3, #253
	moveq	r3, #1
	movne	r3, r7
	str	r3, [r0, #32]
	bl	PostPro_GetVc1MbGroup
	ldr	r2, [r4, #32]
	ldrb	r3, [r5, #68]	@ zero_extendqisi2
	cmp	r2, #1
	streq	r7, [r4, #852]
	and	r3, r3, #3
	str	r3, [r4, #844]
	beq	.L198
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L200
.L198:
	mov	r3, #4
	str	r3, [r4, #864]
	ldr	r3, [r6, #256]
	str	r3, [r4, #848]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L200:
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r3, #1
	streq	r3, [r4, #852]
	beq	.L198
	cmp	r3, #1
	moveq	r3, #2
	streq	r3, [r4, #852]
	b	.L198
	UNWIND(.fnend)
	.size	PostPro_GetVC1RepairParam, .-PostPro_GetVC1RepairParam
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
	bhi	.L214
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
	b	.L210
.L218:
	cmp	ip, r6
	movlt	r6, r7
	add	r6, r6, #1
	cmp	r6, ip
	beq	.L206
	add	r7, r7, r7, lsr #31
	sub	r6, ip, #1
	cmp	r7, r6
	bge	.L206
	ldr	r2, [r0, #840]
	cmp	r2, #199
	bgt	.L215
	add	r10, r0, r2, lsl #2
	add	r2, r2, #1
	strh	r7, [r10, #40]	@ movhi
	strh	r6, [r10, #42]	@ movhi
	str	r2, [r0, #840]
.L206:
	ldr	r2, [r1, #8]
	sub	r6, r2, #1
	cmp	r6, lr
	beq	.L216
.L212:
	add	lr, lr, #1
	mov	r6, r4
	cmp	lr, r2
	mov	r7, ip
	add	r3, r3, #4
	bhi	.L217
.L210:
	ldrh	ip, [r3, #12]
	ldrh	r4, [r3, #14]
	cmp	ip, r5
	cmple	r4, r5
	movgt	r2, #1
	movle	r2, #0
	cmp	ip, r4
	orrgt	r2, r2, #1
	cmp	r2, #0
	beq	.L218
	ldr	r2, [r1, #8]
	add	lr, lr, #1
	add	r3, r3, #4
	cmp	lr, r2
	bls	.L210
.L217:
	mov	r3, #0
.L203:
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L216:
	add	r6, r1, lr, lsl #2
	ldrh	r6, [r6, #14]
	cmp	r6, r9
	bge	.L212
	ldr	r3, [r0, #840]
	cmp	r3, #199
	bgt	.L219
	add	r1, r0, r3, lsl #2
	add	r2, r3, #1
	sub	r8, r8, #1
	mov	r3, #0
	strh	r8, [r1, #42]	@ movhi
	strh	r6, [r1, #40]	@ movhi
	str	r2, [r0, #840]
	b	.L203
.L214:
	ldr	r1, .L220
	mov	r0, #0
	bl	dprint_vfmw
	mov	r3, #1
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L215:
	movw	r2, #1740
	ldr	r1, .L220+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r3, #1
	b	.L203
.L219:
	mov	r2, #1760
	ldr	r1, .L220+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r3, #1
	b	.L203
.L221:
	.align	2
.L220:
	.word	.LC5
	.word	.LC6
	UNWIND(.fnend)
	.size	PostPro_GetVp6MbGroup, .-PostPro_GetVp6MbGroup
	.align	2
	.global	PostPro_GetVP6RepairParam
	.type	PostPro_GetVP6RepairParam, %function
PostPro_GetVP6RepairParam:
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
	ldr	r3, [r1, #176]
	mov	r5, r1
	mov	r4, r0
	mov	r6, #0
	mov	r1, r2
	mov	r2, #1
	str	r3, [r0, #4]
	ldr	r3, [r5, #176]
	str	r3, [r0, #8]
	ldr	r3, [r5, #184]
	str	r3, [r0, #16]
	ldr	r3, [r5, #188]
	str	r3, [r0, #20]
	ldrh	r3, [r5, #54]
	str	r3, [r0, #24]
	ldrh	r3, [r5, #56]
	str	r2, [r0, #32]
	str	r6, [r0, #36]
	str	r3, [r0, #28]
	bl	PostPro_GetVp6MbGroup
	str	r6, [r4, #844]
	str	r6, [r4, #852]
	mov	r2, #4
	ldr	r3, [r5, #208]
	str	r2, [r4, #864]
	str	r3, [r4, #848]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	PostPro_GetVP6RepairParam, .-PostPro_GetVP6RepairParam
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
	bhi	.L236
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
	b	.L232
.L240:
	cmp	ip, r6
	movlt	r6, r7
	add	r6, r6, #1
	cmp	r6, ip
	beq	.L228
	add	r7, r7, r7, lsr #31
	sub	r6, ip, #1
	cmp	r7, r6
	bge	.L228
	ldr	r2, [r0, #840]
	cmp	r2, #199
	bgt	.L237
	add	r10, r0, r2, lsl #2
	add	r2, r2, #1
	strh	r7, [r10, #40]	@ movhi
	strh	r6, [r10, #42]	@ movhi
	str	r2, [r0, #840]
.L228:
	ldr	r2, [r1, #8]
	sub	r6, r2, #1
	cmp	r6, lr
	beq	.L238
.L234:
	add	lr, lr, #1
	mov	r6, r4
	cmp	lr, r2
	mov	r7, ip
	add	r3, r3, #4
	bhi	.L239
.L232:
	ldrh	ip, [r3, #12]
	ldrh	r4, [r3, #14]
	cmp	ip, r5
	cmple	r4, r5
	movgt	r2, #1
	movle	r2, #0
	cmp	ip, r4
	orrgt	r2, r2, #1
	cmp	r2, #0
	beq	.L240
	ldr	r2, [r1, #8]
	add	lr, lr, #1
	add	r3, r3, #4
	cmp	lr, r2
	bls	.L232
.L239:
	mov	r3, #0
.L225:
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L238:
	add	r6, r1, lr, lsl #2
	ldrh	r6, [r6, #14]
	cmp	r6, r9
	bge	.L234
	ldr	r3, [r0, #840]
	cmp	r3, #199
	bgt	.L241
	add	r1, r0, r3, lsl #2
	add	r2, r3, #1
	sub	r8, r8, #1
	mov	r3, #0
	strh	r8, [r1, #42]	@ movhi
	strh	r6, [r1, #40]	@ movhi
	str	r2, [r0, #840]
	b	.L225
.L236:
	ldr	r1, .L242
	mov	r0, #0
	bl	dprint_vfmw
	mov	r3, #1
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L237:
	mov	r2, #1824
	ldr	r1, .L242+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r3, #1
	b	.L225
.L241:
	movw	r2, #1844
	ldr	r1, .L242+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r3, #1
	b	.L225
.L243:
	.align	2
.L242:
	.word	.LC5
	.word	.LC6
	UNWIND(.fnend)
	.size	PostPro_GetVp8MbGroup, .-PostPro_GetVp8MbGroup
	.align	2
	.global	PostPro_GetVP8RepairParam
	.type	PostPro_GetVP8RepairParam, %function
PostPro_GetVP8RepairParam:
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
	ldr	r6, [r1, #3116]
	mov	ip, r1
	mov	r1, r2
	mov	lr, #0
	mov	r5, #1
	mov	r4, #4
	str	r6, [r0, #4]
	ldr	r2, [ip, #3116]
	str	r2, [r0, #8]
	ldr	r2, [ip, #3128]
	str	r2, [r0, #16]
	ldr	r2, [ip, #3132]
	str	r2, [r0, #20]
	ldr	r2, [ip, #2784]
	str	r2, [r0, #24]
	ldr	r2, [ip, #2788]
	str	r5, [r0, #32]
	str	lr, [r0, #36]
	str	r2, [r0, #28]
	str	lr, [r0, #852]
	ldr	r2, [ip, #3140]
	str	r4, [r0, #864]
	str	r2, [r0, #848]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	PostPro_GetVp8MbGroup
	UNWIND(.fnend)
	.size	PostPro_GetVP8RepairParam, .-PostPro_GetVP8RepairParam
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
	bhi	.L256
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
	b	.L252
.L259:
	cmp	r2, r7
	movlt	r7, r6
	add	r7, r7, #1
	cmp	r2, r7
	ble	.L250
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
	bne	.L250
	ldr	r5, [fp, #-52]
	cmp	r2, r5
	addge	r8, r0, #876
	ldrge	r9, [fp, #-60]
.L250:
	ldr	r5, [r1, #8]
	sub	r6, r5, #1
	cmp	r6, ip
	beq	.L257
.L254:
	add	ip, ip, #1
	mov	r7, lr
	cmp	ip, r5
	mov	r6, r2
	add	r3, r3, #4
	bhi	.L258
.L252:
	ldrh	r2, [r3, #12]
	ldrh	lr, [r3, #14]
	cmp	r2, r4
	cmple	lr, r4
	movgt	r5, #1
	movle	r5, #0
	cmp	r2, lr
	orrgt	r5, r5, #1
	cmp	r5, #0
	beq	.L259
	ldr	r5, [r1, #8]
	add	ip, ip, #1
	add	r3, r3, #4
	cmp	ip, r5
	bls	.L252
.L258:
	mov	r2, #0
.L247:
	mov	r0, r2
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L257:
	add	r6, r1, ip, lsl #2
	ldr	r7, [fp, #-48]
	ldrh	r6, [r6, #14]
	cmp	r6, r7
	bge	.L254
	ldr	r1, [r0, #840]
	mov	r2, #0
	ldr	r3, [fp, #-64]
	add	ip, r1, #1
	add	r1, r0, r1, lsl #2
	sub	r3, r3, #1
	strh	r6, [r1, #40]	@ movhi
	strh	r3, [r1, #42]	@ movhi
	str	ip, [r0, #840]
	b	.L247
.L256:
	ldr	r1, .L260
	mov	r0, #0
	bl	dprint_vfmw
	mov	r2, #1
	mov	r0, r2
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L261:
	.align	2
.L260:
	.word	.LC4
	UNWIND(.fnend)
	.size	PostPro_GetAvsMbGroup, .-PostPro_GetAvsMbGroup
	.align	2
	.global	PostPro_GetAVSRepairParam
	.type	PostPro_GetAVSRepairParam, %function
PostPro_GetAVSRepairParam:
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
	add	ip, r1, #12288
	mov	r3, r0
	mov	r6, #0
	ldr	r5, [ip, #2160]
	str	r5, [r0, #4]
	ldr	r0, [ip, #2172]
	str	r0, [r3, #8]
	ldr	r10, [ip, #2152]
	str	r10, [r3, #16]
	ldr	r7, [ip, #2164]
	str	r7, [r3, #20]
	ldrh	r8, [r1, #56]
	str	r8, [r3, #24]
	ldrh	r4, [r1, #58]
	str	r4, [r3, #28]
	mov	r9, r4, lsr #1
	ldrb	lr, [r1, #17]	@ zero_extendqisi2
	str	r6, [r3, #36]
	sub	lr, lr, #3
	clz	lr, lr
	mov	lr, lr, lsr #5
	str	lr, [r3, #32]
	ldr	r0, [r2, #4]
	str	r10, [r3, #892]
	add	r5, r0, r5
	str	r5, [r3, #880]
	ldr	r0, [r2, #4]
	str	lr, [r3, #908]
	str	r4, [r3, #904]
	add	r0, r0, r0, lsl #3
	str	r8, [r3, #900]
	str	r6, [r3, #912]
	mla	r0, r9, r0, r7
	str	r0, [r3, #896]
	ldrb	r1, [r1, #17]	@ zero_extendqisi2
	cmp	r1, #3
	streq	r6, [r3, #852]
	beq	.L264
	cmp	r1, #1
	streq	r1, [r3, #852]
.L264:
	ldr	ip, [ip, #2600]
	mov	r1, r2
	mov	r0, r3
	mov	r2, #4
	str	r2, [r3, #864]
	str	ip, [r3, #848]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	PostPro_GetAvsMbGroup
	UNWIND(.fnend)
	.size	PostPro_GetAVSRepairParam, .-PostPro_GetAVSRepairParam
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
	bhi	.L276
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
	b	.L272
.L279:
	cmp	r2, r7
	movlt	r7, r6
	add	r7, r7, #1
	cmp	r2, r7
	ble	.L270
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
	bne	.L270
	ldr	r5, [fp, #-52]
	cmp	r2, r5
	addge	r8, r0, #876
	ldrge	r9, [fp, #-60]
.L270:
	ldr	r5, [r1, #8]
	sub	r6, r5, #1
	cmp	r6, ip
	beq	.L277
.L274:
	add	ip, ip, #1
	mov	r7, lr
	cmp	ip, r5
	mov	r6, r2
	add	r3, r3, #4
	bhi	.L278
.L272:
	ldrh	r2, [r3, #12]
	ldrh	lr, [r3, #14]
	cmp	r2, r4
	cmple	lr, r4
	movgt	r5, #1
	movle	r5, #0
	cmp	r2, lr
	orrgt	r5, r5, #1
	cmp	r5, #0
	beq	.L279
	ldr	r5, [r1, #8]
	add	ip, ip, #1
	add	r3, r3, #4
	cmp	ip, r5
	bls	.L272
.L278:
	mov	r2, #0
.L267:
	mov	r0, r2
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L277:
	add	r6, r1, ip, lsl #2
	ldr	r7, [fp, #-48]
	ldrh	r6, [r6, #14]
	cmp	r6, r7
	bge	.L274
	ldr	r1, [r0, #840]
	mov	r2, #0
	ldr	r3, [fp, #-64]
	add	ip, r1, #1
	add	r1, r0, r1, lsl #2
	sub	r3, r3, #1
	strh	r6, [r1, #40]	@ movhi
	strh	r3, [r1, #42]	@ movhi
	str	ip, [r0, #840]
	b	.L267
.L276:
	ldr	r1, .L280
	mov	r0, #0
	bl	dprint_vfmw
	mov	r2, #1
	mov	r0, r2
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L281:
	.align	2
.L280:
	.word	.LC4
	UNWIND(.fnend)
	.size	PostPro_GetDivx3MbGroup, .-PostPro_GetDivx3MbGroup
	.align	2
	.global	PostPro_GetDIVX3RepairParam
	.type	PostPro_GetDIVX3RepairParam, %function
PostPro_GetDIVX3RepairParam:
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
	ldr	r6, [r1, #128]
	mov	ip, r1
	mov	r1, r2
	mov	lr, #0
	mov	r5, #1
	mov	r4, #4
	str	r6, [r0, #4]
	ldr	r2, [ip, #124]
	str	r2, [r0, #16]
	ldr	r2, [ip, #156]
	str	r2, [r0, #20]
	ldr	r2, [ip, #8]
	str	r2, [r0, #24]
	ldr	r2, [ip, #12]
	str	r5, [r0, #32]
	str	lr, [r0, #36]
	str	r2, [r0, #28]
	str	lr, [r0, #852]
	ldr	r2, [ip, #164]
	str	r4, [r0, #864]
	str	r2, [r0, #848]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	PostPro_GetDivx3MbGroup
	UNWIND(.fnend)
	.size	PostPro_GetDIVX3RepairParam, .-PostPro_GetDIVX3RepairParam
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
	bls	.L295
	mov	r7, #1
.L288:
	mov	r4, #0
	rsb	lr, r7, r8
	mov	r2, r4
	mov	r3, r0
	b	.L289
.L287:
	ldrh	r1, [r3, #12]
	ldrh	ip, [r3, #16]
	cmp	r1, ip
	bls	.L286
	ldrh	r6, [r3, #14]
	mov	r4, #1
	ldrh	r5, [r3, #18]
	strh	ip, [r3, #12]	@ movhi
	strh	r1, [r3, #16]	@ movhi
	strh	r6, [r3, #18]	@ movhi
	strh	r5, [r3, #14]	@ movhi
.L286:
	add	r3, r3, #4
.L289:
	cmp	r2, lr
	add	r2, r2, #1
	bne	.L287
	cmp	r4, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	add	r7, r7, #1
	cmp	r8, r7
	movls	r4, #0
	andhi	r4, r4, #1
	cmp	r4, #0
	bne	.L288
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L295:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	PostPro_BubbleSort, .-PostPro_BubbleSort
	.global	__aeabi_uidivmod
	.global	__aeabi_uidiv
	.align	2
	.global	PostPro_GetH264MbOfMBAFF
	.type	PostPro_GetH264MbOfMBAFF, %function
PostPro_GetH264MbOfMBAFF:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 832
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #836)
	sub	sp, sp, #836
	ldr	r3, [r1, #8]
	ldr	r2, [r0, #24]
	mov	r9, r1
	cmp	r3, #0
	mov	r10, r0
	mov	r4, r3
	str	r3, [fp, #-856]
	ldr	r3, [r0, #28]
	mov	r6, r2, asl #1
	str	r2, [fp, #-868]
	str	r3, [fp, #-872]
	beq	.L309
	sub	lr, fp, #844
	mov	r3, r1
	add	r1, lr, #2
	mov	r2, #0
.L298:
	ldrh	ip, [r3, #12]
	add	r2, r2, #1
	ldrh	r0, [r3, #14]
	cmp	r2, r4
	add	r3, r3, #4
	strh	ip, [r1, #-2]	@ movhi
	strh	r0, [r1], #4	@ movhi
	bne	.L298
	ldrh	r8, [lr]
	cmp	r6, #0
	ldrh	r3, [lr, #2]
	bic	r8, r8, #1
	orr	r3, r3, #1
	str	r3, [fp, #-848]
	uxth	r8, r8
	strh	r3, [lr, #2]	@ movhi
	strh	r8, [lr]	@ movhi
	beq	.L310
	mov	r5, #0
	sub	r3, r6, #2
	mov	r7, r5
	str	r3, [fp, #-852]
	str	lr, [fp, #-876]
	uxth	r3, r6
	str	r3, [fp, #-864]
.L306:
	mov	r1, r6
	mov	r0, r8
	bl	__aeabi_uidivmod
	ldr	r0, [fp, #-848]
	mov	r4, r1
	cmp	r4, #1
	mov	r1, r6
	bls	.L300
	bl	__aeabi_uidivmod
	ldr	r3, [fp, #-852]
	cmp	r3, r1
	bls	.L301
	str	r1, [fp, #-860]
	mov	r0, r8
	mov	r1, r6
	mov	r4, r4, lsr #1
	bl	__aeabi_uidiv
	mov	r1, r6
	mov	r8, r0
	ldr	r0, [fp, #-848]
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-860]
	mov	r2, r2, lsr #1
	cmp	r0, r8
	mov	r3, r0
	beq	.L302
	ldr	r1, [fp, #-864]
	add	ip, r5, #1
	cmp	ip, #199
	smulbb	r0, r8, r1
	add	r1, r9, r5, lsl #2
	uxth	r0, r0
	add	r4, r0, r4
	uxth	r4, r4
	strh	r4, [r1, #12]	@ movhi
	ldr	lr, [r10, #24]
	sub	lr, lr, #1
	add	r0, r0, lr
	strh	r0, [r1, #14]	@ movhi
	bhi	.L303
	ldr	r0, [fp, #-864]
	add	lr, r5, #2
	add	ip, r9, ip, lsl #2
	cmp	lr, #199
	uxth	r2, r2
	smulbb	r3, r3, r0
	ldr	r0, [r10, #24]
	add	r4, r4, r0
	strh	r4, [r1, #16]	@ movhi
	uxth	r3, r3
	add	r0, r2, r3
	strh	r0, [ip, #14]	@ movhi
	bhi	.L303
	ldr	r0, [r10, #24]
	add	r5, r5, #3
	add	lr, r9, lr, lsl #2
	cmp	r5, #199
	add	r3, r3, r0
	uxth	r3, r3
	strh	r3, [r1, #20]	@ movhi
	add	r2, r3, r2
	strh	r2, [lr, #14]	@ movhi
	bhi	.L303
.L304:
	ldr	r3, [fp, #-856]
	add	r7, r7, #1
	cmp	r7, r3
	beq	.L299
.L307:
	ldr	r2, [fp, #-876]
	ldrh	r3, [r2, #6]
	ldrh	r8, [r2, #4]
	orr	r3, r3, #1
	str	r3, [fp, #-848]
	bic	r8, r8, #1
	strh	r8, [r2, #4]	@ movhi
	mov	r1, r3
	mov	r3, r2
	strh	r1, [r3, #6]	@ movhi
	add	r3, r3, #4
	str	r3, [fp, #-876]
	b	.L306
.L301:
	mov	r1, r6
	mov	r0, r8
	bl	__aeabi_uidiv
	add	r2, r9, r5, lsl #2
	add	r3, r5, #1
	cmp	r3, #199
	smulbb	r0, r0, r6
	uxth	r0, r0
	add	r4, r0, r4, lsr #1
	uxth	r4, r4
	strh	r4, [r2, #12]	@ movhi
	ldr	r1, [r10, #24]
	sub	r1, r1, #1
	add	r0, r0, r1
	strh	r0, [r2, #14]	@ movhi
	bhi	.L303
	ldr	r1, [r10, #24]
	add	r5, r5, #2
	add	r3, r9, r3, lsl #2
	cmp	r5, #199
	add	r4, r4, r1
	strh	r4, [r2, #16]	@ movhi
	ldr	r2, [fp, #-848]
	strh	r2, [r3, #14]	@ movhi
	bls	.L304
.L303:
	ldr	r2, [fp, #-868]
	mov	r1, #1
	ldr	r3, [fp, #-872]
	mov	r0, r9
	str	r1, [r9, #8]
	mul	r3, r3, r2
	mov	r2, #0
	strh	r2, [r9, #12]	@ movhi
	sub	r3, r3, #1
	strh	r3, [r9, #14]	@ movhi
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	PostPro_BubbleSort
.L300:
	bl	__aeabi_uidivmod
	ldr	r2, [fp, #-852]
	cmp	r2, r1
	bls	.L305
	add	r4, r9, r5, lsl #2
	str	r1, [fp, #-860]
	ldr	r0, [fp, #-848]
	mov	r1, r6
	strh	r8, [r4, #12]	@ movhi
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-860]
	add	r1, r5, #1
	cmp	r1, #199
	ubfx	r2, r3, #1, #16
	smulbb	r0, r0, r6
	uxth	r0, r0
	add	r3, r2, r0
	strh	r3, [r4, #14]	@ movhi
	bhi	.L303
	ldr	r3, [r10, #24]
	add	r5, r5, #2
	add	r1, r9, r1, lsl #2
	cmp	r5, #199
	add	r0, r0, r3
	uxth	r3, r0
	strh	r3, [r4, #16]	@ movhi
	add	r3, r3, r2
	strh	r3, [r1, #14]	@ movhi
	bhi	.L303
	ldr	r3, [fp, #-856]
	add	r7, r7, #1
	cmp	r7, r3
	bne	.L307
.L299:
	str	r5, [r9, #8]
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	PostPro_BubbleSort
.L305:
	add	r2, r9, r5, lsl #2
	add	r5, r5, #1
	cmp	r5, #199
	mov	r3, r2
	strh	r8, [r2, #12]	@ movhi
	ldr	r2, [fp, #-848]
	strh	r2, [r3, #14]	@ movhi
	bls	.L304
	b	.L303
.L302:
	smulbb	r3, r0, r6
	add	r1, r5, #1
	add	ip, r9, r5, lsl #2
	cmp	r1, #199
	uxth	r3, r3
	add	r4, r3, r4
	add	r2, r3, r2
	uxth	r4, r4
	uxth	r2, r2
	strh	r4, [ip, #12]	@ movhi
	strh	r2, [ip, #14]	@ movhi
	bhi	.L303
	ldr	r3, [r10, #24]
	add	r5, r5, #2
	add	r1, r9, r1, lsl #2
	cmp	r5, #199
	add	r4, r4, r3
	strh	r4, [ip, #16]	@ movhi
	ldr	r3, [r10, #24]
	add	r2, r2, r3
	strh	r2, [r1, #14]	@ movhi
	bls	.L304
	b	.L303
.L309:
	ldr	r5, [fp, #-856]
	b	.L299
.L310:
	mov	r5, r6
	b	.L299
	UNWIND(.fnend)
	.size	PostPro_GetH264MbOfMBAFF, .-PostPro_GetH264MbOfMBAFF
	.align	2
	.global	PostPro_GetH264MbGroup
	.type	PostPro_GetH264MbGroup, %function
PostPro_GetH264MbGroup:
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
	ldr	r3, [r1, #8]
	mov	r7, r1
	mov	r8, r0
	sub	ip, r3, #1
	mov	lr, r2
	cmp	ip, #199
	bhi	.L336
	ldr	r3, [r0, #24]
	cmp	r2, #0
	ldr	r4, [r0, #28]
	mul	r4, r4, r3
	bne	.L337
.L321:
	mov	ip, #0
	str	ip, [r8, #840]
	str	ip, [r8, #856]
	mvn	r5, #0
	ldr	r2, [r7, #8]
	add	r3, r4, r5
	uxth	r10, r4
	str	r3, [fp, #-48]
	mov	r6, r5
	mov	r3, r7
	add	r1, r7, r2, lsl #2
	strh	r10, [r1, #14]	@ movhi
	strh	r10, [r1, #12]	@ movhi
	b	.L329
.L341:
	cmp	r2, r5
	movlt	r5, r6
	add	r0, r5, #1
	cmp	r2, r0
	ble	.L324
	ldr	r9, [r8, #840]
	cmp	r9, #199
	bgt	.L338
	add	lr, r6, r6, lsr #31
	sub	r0, r5, #100
	add	r5, r8, r9, lsl #2
	add	r9, r9, #1
	sxth	r6, lr
	strh	lr, [r5, #40]	@ movhi
	cmp	r0, r6
	sub	lr, r2, #1
	strgth	r0, [r5, #40]	@ movhi
	strh	lr, [r5, #42]	@ movhi
	str	r9, [r8, #840]
.L324:
	ldr	r0, [r7, #8]
	sub	lr, r0, #1
	cmp	lr, ip
	beq	.L339
.L331:
	add	ip, ip, #1
	mov	r5, r1
	cmp	ip, r0
	mov	r6, r2
	add	r3, r3, #4
	bhi	.L340
.L329:
	ldrh	r2, [r3, #12]
	ldrh	r1, [r3, #14]
	cmp	r2, r4
	cmple	r1, r4
	movgt	r9, #1
	movle	r9, #0
	cmp	r2, r1
	orrgt	r9, r9, #1
	cmp	r9, #0
	beq	.L341
	ldr	r0, [r7, #8]
	add	ip, ip, #1
	add	r3, r3, #4
	cmp	ip, r0
	bls	.L329
.L340:
	mov	r0, #0
.L320:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L339:
	add	lr, r7, ip, lsl #2
	ldrh	r5, [lr, #14]
	ldr	lr, [fp, #-48]
	cmp	r5, lr
	bge	.L331
	ldr	r3, [r8, #840]
	cmp	r3, #199
	bgt	.L342
	add	r2, r8, r3, lsl #2
	sub	r10, r10, #1
	add	r3, r3, #1
	mov	r0, #0
	strh	r10, [r2, #42]	@ movhi
	strh	r5, [r2, #40]	@ movhi
	str	r3, [r8, #840]
	b	.L320
.L337:
	bl	PostPro_GetH264MbOfMBAFF
	b	.L321
.L336:
	mov	r2, r3
	ldr	r1, .L343
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L338:
	movw	r2, #1455
	ldr	r1, .L343+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, #1
	b	.L320
.L342:
	movw	r2, #1483
	ldr	r1, .L343+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, #1
	b	.L320
.L344:
	.align	2
.L343:
	.word	.LC4
	.word	.LC6
	UNWIND(.fnend)
	.size	PostPro_GetH264MbGroup, .-PostPro_GetH264MbGroup
	.align	2
	.global	PostPro_GetH264RepairParam
	.type	PostPro_GetH264RepairParam, %function
PostPro_GetH264RepairParam:
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
	ldr	r3, [r1, #292]
	mov	r4, r1
	mov	r5, r0
	mov	r1, r2
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #308]
	str	r3, [r0, #4]
	ldr	r3, [r4, #296]
	str	r3, [r0, #8]
	ldr	r3, [r4, #1008]
	ldr	r0, [r3, #1640]
	add	r3, r4, r0, lsl #2
	ldr	r3, [r3, #308]
	str	r3, [r5, #16]
	ldr	r3, [r4, #300]
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #700]
	str	r3, [r5, #20]
	ldr	r3, [r4, #28]
	str	r3, [r5, #24]
	ldr	r3, [r4, #20]
	str	r3, [r5, #28]
	ldrb	r3, [r4]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r5, #32]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r5, #36]
	ldr	r6, [r4, #1000]
	cmp	r6, #0
	beq	.L346
	ldr	r3, [r4, #1204]
	cmp	r3, r0
	addne	ip, r4, #1200
	movne	r3, #0
	addne	ip, ip, #4
	bne	.L349
	b	.L383
.L350:
	ldr	lr, [ip, #4]!
	cmp	lr, r0
	beq	.L347
.L349:
	add	r3, r3, #1
	cmp	r3, r6
	bne	.L350
.L346:
	ldr	r2, [r4, #32]
	ldrsb	r3, [r4, #2]
	cmp	r2, #0
	ldrneb	r2, [r4]	@ zero_extendqisi2
	clzne	r2, r2
	movne	r2, r2, lsr #5
	cmp	r3, #0
	streq	r3, [r1, #8]
	bne	.L384
	ldr	r0, [r5, #4]
	ldr	r3, [r5, #16]
	cmp	r0, r3
	beq	.L385
.L354:
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L363
	cmp	r3, #1
	beq	.L381
	cmp	r3, #2
	beq	.L381
	cmp	r3, #3
	bne	.L360
.L363:
	mov	r3, #0
	str	r3, [r5, #852]
.L360:
	ldr	r3, [r4, #1268]
	mov	r2, #4
	str	r2, [r5, #864]
	str	r3, [r5, #848]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L381:
	str	r3, [r5, #852]
	b	.L360
.L383:
	mov	r3, #0
.L347:
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #1140]
	str	r3, [r5, #844]
	b	.L346
.L384:
	mov	r0, r5
	bl	PostPro_GetH264MbGroup
	ldr	r0, [r5, #4]
	ldr	r3, [r5, #16]
	cmp	r0, r3
	bne	.L354
.L385:
	ldr	ip, [r4, #696]
	cmp	ip, #0
	beq	.L354
	ldr	r2, [r4, #308]
	cmp	r0, r2
	addeq	r1, r4, #308
	moveq	r3, #0
	bne	.L355
.L357:
	add	r3, r3, #1
	cmp	r3, ip
	beq	.L354
	ldr	r2, [r1, #4]!
	cmp	r0, r2
	beq	.L357
.L355:
	str	r2, [r5, #16]
	b	.L354
	UNWIND(.fnend)
	.size	PostPro_GetH264RepairParam, .-PostPro_GetH264RepairParam
	.align	2
	.global	PostPro_GetMVCRepairParam
	.type	PostPro_GetMVCRepairParam, %function
PostPro_GetMVCRepairParam:
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
	ldr	r3, [r1, #292]
	mov	r4, r1
	mov	r5, r0
	mov	r1, r2
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #308]
	str	r3, [r0, #4]
	ldr	r3, [r4, #296]
	str	r3, [r0, #8]
	ldr	r3, [r4, #1008]
	ldr	r0, [r3, #1640]
	add	r3, r4, r0, lsl #2
	ldr	r3, [r3, #308]
	str	r3, [r5, #16]
	ldr	r3, [r4, #300]
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #700]
	str	r3, [r5, #20]
	ldr	r3, [r4, #28]
	str	r3, [r5, #24]
	ldr	r3, [r4, #20]
	str	r3, [r5, #28]
	ldrb	r3, [r4]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r5, #32]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r5, #36]
	ldr	r6, [r4, #1000]
	cmp	r6, #0
	beq	.L387
	ldr	r3, [r4, #1204]
	cmp	r3, r0
	addne	ip, r4, #1200
	movne	r3, #0
	addne	ip, ip, #4
	bne	.L390
	b	.L424
.L391:
	ldr	lr, [ip, #4]!
	cmp	lr, r0
	beq	.L388
.L390:
	add	r3, r3, #1
	cmp	r3, r6
	bne	.L391
.L387:
	ldr	r2, [r4, #32]
	ldrsb	r3, [r4, #2]
	cmp	r2, #0
	ldrneb	r2, [r4]	@ zero_extendqisi2
	clzne	r2, r2
	movne	r2, r2, lsr #5
	cmp	r3, #0
	streq	r3, [r1, #8]
	bne	.L425
	ldr	r0, [r5, #4]
	ldr	r3, [r5, #16]
	cmp	r0, r3
	beq	.L426
.L395:
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L404
	cmp	r3, #1
	beq	.L422
	cmp	r3, #2
	beq	.L422
	cmp	r3, #3
	bne	.L401
.L404:
	mov	r3, #0
	str	r3, [r5, #852]
.L401:
	ldr	r3, [r4, #1268]
	mov	r2, #4
	str	r2, [r5, #864]
	str	r3, [r5, #848]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L422:
	str	r3, [r5, #852]
	b	.L401
.L424:
	mov	r3, #0
.L388:
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #1140]
	str	r3, [r5, #844]
	b	.L387
.L425:
	mov	r0, r5
	bl	PostPro_GetH264MbGroup
	ldr	r0, [r5, #4]
	ldr	r3, [r5, #16]
	cmp	r0, r3
	bne	.L395
.L426:
	ldr	ip, [r4, #696]
	cmp	ip, #0
	beq	.L395
	ldr	r2, [r4, #308]
	cmp	r0, r2
	addeq	r1, r4, #308
	moveq	r3, #0
	bne	.L396
.L398:
	add	r3, r3, #1
	cmp	r3, ip
	beq	.L395
	ldr	r2, [r1, #4]!
	cmp	r0, r2
	beq	.L398
.L396:
	str	r2, [r5, #16]
	b	.L395
	UNWIND(.fnend)
	.size	PostPro_GetMVCRepairParam, .-PostPro_GetMVCRepairParam
	.align	2
	.global	PostPro_GetRepairParam
	.type	PostPro_GetRepairParam, %function
PostPro_GetRepairParam:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r7, r3
	mov	r4, r1
	mov	r3, #0
	str	r0, [r1]
	str	r3, [r1, #840]
	str	r3, [r1, #1716]
	cmp	r0, #17
	ldrls	pc, [pc, r0, asl #2]
	b	.L428
.L430:
	.word	.L429
	.word	.L431
	.word	.L432
	.word	.L433
	.word	.L428
	.word	.L434
	.word	.L435
	.word	.L428
	.word	.L436
	.word	.L437
	.word	.L438
	.word	.L438
	.word	.L438
	.word	.L439
	.word	.L428
	.word	.L440
	.word	.L441
	.word	.L442
.L442:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetVP9RepairParam
.L443:
	ldr	r8, [r4, #840]
	ldr	r3, [r4, #24]
	ldr	r6, [r4, #28]
	cmp	r8, #0
	mul	r6, r6, r3
	beq	.L444
	mov	r8, #0
	mov	r5, r4
	mov	r9, r8
.L445:
	ldrsh	r3, [r5, #40]
	mov	r0, #22
	ldrsh	r2, [r5, #42]
	add	r9, r9, #1
	ldr	r1, .L456
	add	r5, r5, #4
	bl	dprint_vfmw
	ldrsh	r3, [r5, #38]
	ldrsh	r2, [r5, #36]
	ldr	r1, [r4, #840]
	rsb	r3, r2, r3
	cmp	r1, r9
	add	r3, r3, #1
	add	r8, r8, r3
	bhi	.L445
.L444:
	ldr	r1, [r4, #1716]
	cmp	r1, #0
	beq	.L446
	add	r0, r4, #916
	mov	r2, #0
	add	r0, r0, #2
.L447:
	ldrsh	r3, [r0]
	add	r2, r2, #1
	ldrsh	ip, [r0, #-2]
	cmp	r2, r1
	add	r0, r0, #4
	rsb	ip, ip, r3
	add	ip, ip, #1
	add	r8, r8, ip
	bne	.L447
.L446:
	cmp	r6, #0
	moveq	r4, r6
	beq	.L448
	mov	r0, #100
	mov	r1, r6
	mul	r0, r0, r8
	bl	__aeabi_uidiv
	cmp	r0, #100
	movcc	r4, r0
	movcs	r4, #100
.L448:
	mov	ip, #0
	mov	r3, r8
	mov	r2, r6
	ldr	r1, .L456+4
	str	ip, [r7, #8]
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L429:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetH264RepairParam
	b	.L443
.L431:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetVC1RepairParam
	b	.L443
.L432:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetMPEG4RepairParam
	b	.L443
.L433:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetMPEG2RepairParam
	b	.L443
.L434:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetDIVX3RepairParam
	b	.L443
.L435:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetAVSRepairParam
	b	.L443
.L436:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetREAL8RepairParam
	b	.L443
.L437:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetREAL9RepairParam
	b	.L443
.L438:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetVP6RepairParam
	b	.L443
.L439:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetVP8RepairParam
	b	.L443
.L440:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetMVCRepairParam
	b	.L443
.L441:
	mov	r1, r2
	ldr	r3, [fp, #4]
	mov	r2, r7
	mov	r0, r4
	bl	PostPro_GetHEVCRepairParam
	b	.L443
.L428:
	mov	r3, #916
	ldr	r2, .L456+8
	ldr	r1, .L456+12
	mov	r0, #0
	bl	dprint_vfmw
	b	.L443
.L457:
	.align	2
.L456:
	.word	.LC8
	.word	.LC9
	.word	.LANCHOR0+32
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
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #28)
	sub	sp, sp, #28
	subs	r3, r0, #0
	beq	.L463
	ldr	ip, .L464
	mov	r2, #12
	ldr	r5, [r3]
	mov	r1, #0
	ldr	r4, .L464+4
	sub	r0, fp, #48
	ldr	ip, [ip, #48]
	ldr	r7, [r3, #4]
	ldr	r8, [r3, #8]
	blx	ip
	mov	r3, #816
	ldr	r1, .L464+8
	mla	r4, r3, r5, r4
	ldr	r6, .L464+12
	ldr	r3, [r1, #144]
	movw	r2, #1752
	str	r7, [fp, #-48]
	cmp	r3, #0
	str	r5, [fp, #-40]
	mla	r6, r2, r5, r6
	str	r4, [fp, #-44]
	beq	.L461
	sub	r0, fp, #48
	blx	r3
.L462:
	mov	r3, r4
	mov	r2, r8
	mov	r1, r6
	str	r5, [sp]
	mov	r0, r7
	bl	PostPro_GetRepairParam
	ldr	r1, .L464+16
	mov	r4, r0
	mov	r2, r0
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L461:
	mov	r0, r3
	mov	r2, #130
	ldr	r1, .L464+20
	bl	dprint_vfmw
	b	.L462
.L463:
	ldr	r1, .L464+24
	bl	dprint_vfmw
	mov	r0, #1
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L465:
	.align	2
.L464:
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
	ldr	r5, .L467
	ldr	r3, .L467+4
	mov	lr, r2
	movw	r7, #1752
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
.L468:
	.align	2
.L467:
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
	ldr	r3, .L488
	mov	r5, r0
	mov	r4, r2
	mov	r6, r1
	mov	r0, r2
	mov	r1, #0
	mov	r2, #816
	ldr	r3, [r3, #48]
	blx	r3
	cmp	r5, #17
	ldrls	pc, [pc, r5, asl #2]
	b	.L470
.L472:
	.word	.L481
	.word	.L481
	.word	.L481
	.word	.L481
	.word	.L470
	.word	.L481
	.word	.L477
	.word	.L470
	.word	.L481
	.word	.L481
	.word	.L481
	.word	.L481
	.word	.L481
	.word	.L481
	.word	.L470
	.word	.L481
	.word	.L481
	.word	.L481
.L477:
	ldrb	r3, [r6, #17]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L487
.L481:
	mov	r3, #0
	mov	r2, #1
	strh	r3, [r4, #12]	@ movhi
	str	r2, [r4, #8]
	strh	r3, [r4, #14]	@ movhi
.L485:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L470:
	movw	r3, #2253
	ldr	r2, .L488+4
	ldr	r1, .L488+8
	mov	r0, #0
	bl	dprint_vfmw
	b	.L485
.L487:
	mov	r3, #0
	mov	r2, #2
	strh	r3, [r4, #12]	@ movhi
	str	r2, [r4, #8]
	strh	r3, [r4, #14]	@ movhi
	ldrh	r2, [r6, #56]
	ldrh	r3, [r6, #58]
	mul	r3, r3, r2
	add	r3, r3, r3, lsr #31
	mov	r3, r3, asr #1
	strh	r3, [r4, #16]	@ movhi
	ldrh	r2, [r6, #56]
	ldrh	r3, [r6, #58]
	mul	r3, r3, r2
	add	r3, r3, r3, lsr #31
	mov	r3, r3, asr #1
	strh	r3, [r4, #18]	@ movhi
	b	.L485
.L489:
	.align	2
.L488:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+56
	.word	.LC7
	UNWIND(.fnend)
	.size	PostPro_SetDecReport, .-PostPro_SetDecReport
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.14474, %object
	.size	__func__.14474, 31
__func__.14474:
	.ascii	"PostPro_GetHevcMbGroupOfNoTile\000"
	.space	1
	.type	__func__.14417, %object
	.size	__func__.14417, 23
__func__.14417:
	.ascii	"PostPro_GetRepairParam\000"
	.space	1
	.type	__func__.14681, %object
	.size	__func__.14681, 21
__func__.14681:
	.ascii	"PostPro_SetDecReport\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"pRepairParam->ImageWidth=%d,pRepairParam->ImageHeig" )
	ASCII(.ascii	"ht=%d\012\000" )
	.space	2
.LC1:
	ASCII(.ascii	"pRepairParam->ValidGroupNum >(MAX_UP_MSG_SLICE_NUM " )
	ASCII(.ascii	"- 1)\012\000" )
	.space	3
.LC2:
	ASCII(.ascii	"%s:0 pRepairParam->ValidGroupNum(%d) > (MAX_UP_MSG_" )
	ASCII(.ascii	"SLICE_NUM - 1)\012\000" )
	.space	1
.LC3:
	ASCII(.ascii	"%s:1 pRepairParam->ValidGroupNum(%d) > (MAX_UP_MSG_" )
	ASCII(.ascii	"SLICE_NUM - 1)\012\000" )
	.space	1
.LC4:
	ASCII(.ascii	"DecSliceNum(=%d) out of range!\012\000" )
.LC5:
	ASCII(.ascii	"DecSliceNum out of range!\012\000" )
	.space	1
.LC6:
	ASCII(.ascii	"line:%d! pRepairParam->ValidGroupNum > (MAX_UP_MSG_" )
	ASCII(.ascii	"SLICE_NUM - 1)\012\000" )
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
