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
	.global	VDMHAL_V400R003_GetHalMemSize
	.type	VDMHAL_V400R003_GetHalMemSize, %function
VDMHAL_V400R003_GetHalMemSize:
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
	mov	r0, #6291456
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_GetHalMemSize, .-VDMHAL_V400R003_GetHalMemSize
	.align	2
	.global	VDMHAL_V400R003_OpenHAL
	.type	VDMHAL_V400R003_OpenHAL, %function
VDMHAL_V400R003_OpenHAL:
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
	subs	r3, r0, #0
	beq	.L18
	ldr	r6, [r3]
	ldr	r5, [r3, #4]
	cmp	r6, #0
	ldr	r7, [r3, #8]
	beq	.L19
	cmp	r5, #6291456
	blt	.L20
	cmp	r7, #0
	bgt	.L21
	bne	.L16
	ldr	r9, .L23
	mov	r2, #140
	ldr	r8, .L23+4
	mov	r1, r7
	ldr	r4, .L23+8
	ldr	r3, [r9, #48]
	mov	r0, r8
	blx	r3
	ldr	r3, [r9, #48]
	mov	r1, r7
	mov	r2, #4
	mov	r0, r4
	str	r2, [r8]
	movw	r2, #1228
	blx	r3
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	ldr	r1, .L23+12
	mov	r3, r0
	mov	r2, r0
	str	r3, [r4]
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, #53248
	movt	r0, 63683
	bl	MEM_Phy2Vir
	ldr	r1, .L23+16
	mov	r3, r0
	mov	r2, r0
	str	r3, [r4, #8]
	mov	r0, #22
	bl	dprint_vfmw
	add	r0, r6, #1020
	add	r0, r0, #3
	add	r3, r5, r6
	bic	r0, r0, #1020
	add	r1, r4, #876
	bic	r0, r0, #3
	mov	r2, #1024
	rsb	r3, r0, r3
	str	r3, [r4, #20]
	add	r3, r4, #36
	str	r2, [r4, #24]
	mov	r2, r0
	str	r0, [r4, #16]
.L10:
	str	r2, [r3, #4]!
	cmp	r3, r1
	add	r2, r2, #1280
	bne	.L10
	mov	r3, #210
	str	r3, [r4, #1064]
	add	r3, r0, #274432
	add	r0, r0, #266240
	add	r0, r0, #3072
	str	r3, [r4, #1108]
	add	r2, r0, #38656
	str	r0, [r4, #1088]
	add	r2, r2, #255
	mov	r0, #0
	bic	r2, r2, #32512
	str	r0, [r4, #1168]
	bic	r2, r2, #255
	str	r0, [r4, #1140]
	add	r1, r2, #4390912
	add	r10, r2, #1769472
	add	ip, r10, #5248
	add	r3, r1, #37888
	cmp	ip, r3
	rsbcs	r6, r6, ip
	rsbcc	r6, r6, r3
	add	lr, r2, #4325376
	cmp	r5, r6
	add	r3, r2, #589824
	add	r9, r2, #1081344
	add	r8, r2, #2129920
	add	r7, r2, #3178496
	str	r6, [fp, #-48]
	str	ip, [r4, #1136]
	add	r6, r2, #4194304
	add	ip, r2, #1179648
	add	r10, r10, #2048
	add	r8, r8, #12288
	add	ip, ip, #2048
	str	r2, [r4, #1156]
	add	r9, r9, #12288
	str	r2, [r4, #1092]
	add	r7, r7, #12288
	str	r2, [r4, #1096]
	add	r6, r6, #45056
	str	r2, [r4, #1100]
	add	r2, r2, #45056
	str	r10, [r4, #1128]
	str	r10, [r4, #1132]
	add	r10, r1, #1024
	str	r8, [r4, #1152]
	add	r1, r1, #33792
	add	r8, r3, #2048
	str	r1, [r4, #1184]
	str	r10, [r4, #1204]
	add	r1, lr, #47104
	str	r10, [r4, #1188]
	add	lr, lr, #50176
	str	r9, [r4, #1148]
	str	r7, [r4, #1160]
	str	r6, [r4, #1192]
	str	r1, [r4, #1176]
	str	lr, [r4, #1180]
	str	r3, [r4, #1104]
	str	r3, [r4, #1112]
	str	r8, [r4, #1116]
	str	ip, [r4, #1120]
	str	ip, [r4, #1124]
	str	r2, [r4, #1144]
	bcc	.L11
	ldr	r0, .L23+8
	bl	H264HAL_V400R003_InitHal
	mov	r4, r0
	ldr	r0, .L23+8
	bl	HEVCHAL_V400R003_InitHal
	cmp	r0, #0
	mvnne	r4, #0
	beq	.L22
.L12:
	ldr	r0, .L23+8
	bl	H264HAL_V400R003_InitHal
	cmp	r0, #0
	moveq	r0, r4
	mvnne	r0, #0
.L4:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L22:
	adds	r4, r4, #0
	movne	r4, #1
	rsb	r4, r4, #0
	b	.L12
.L21:
	ldr	r1, .L23+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4
.L16:
	ldr	r1, .L23+24
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4
.L11:
	mov	r3, r5
	ldr	r2, [fp, #-48]
	ldr	r1, .L23+28
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4
.L19:
	mov	r0, r6
	ldr	r3, .L23+32
	ldr	r2, .L23+36
	ldr	r1, .L23+40
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4
.L20:
	ldr	r3, .L23+44
	mov	r0, #0
	ldr	r2, .L23+36
	ldr	r1, .L23+40
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4
.L18:
	ldr	r3, .L23+48
	ldr	r2, .L23+36
	ldr	r1, .L23+40
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4
.L24:
	.align	2
.L23:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_VdmExtParam
	.word	g_HwMem
	.word	.LC5
	.word	.LC6
	.word	.LC4
	.word	.LC7
	.word	.LC8
	.word	.LC2
	.word	.LANCHOR0
	.word	.LC1
	.word	.LC3
	.word	.LC0
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_OpenHAL, .-VDMHAL_V400R003_OpenHAL
	.align	2
	.global	VDMHAL_V400R003_CloseHAL
	.type	VDMHAL_V400R003_CloseHAL, %function
VDMHAL_V400R003_CloseHAL:
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
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_CloseHAL, .-VDMHAL_V400R003_CloseHAL
	.align	2
	.global	VDMHAL_V400R003_CalcFsSize
	.type	VDMHAL_V400R003_CalcFsSize, %function
VDMHAL_V400R003_CalcFsSize:
	UNWIND(.fnstart)
	@ args = 8, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	sub	lr, r1, #32
	movw	ip, #8160
	cmp	lr, ip
	mov	r7, r0
	str	r3, [fp, #-64]
	ldr	r6, [fp, #8]
	bhi	.L27
	sub	r3, r2, #32
	mov	r4, r2
	cmp	r3, ip
	bhi	.L27
	ldr	r3, .L65
	add	r5, r1, #15
	add	lr, r2, #15
	mov	r5, r5, asr #4
	mov	lr, lr, asr #4
	ldrb	ip, [r3]	@ zero_extendqisi2
	cmp	r5, #45
	cmple	lr, #36
	mov	r3, r5, asl #4
	movle	r0, #64
	movgt	r0, #32
	cmp	ip, #1
	ldr	ip, [fp, #4]
	moveq	r0, #64
	cmp	ip, #16
	beq	.L32
	cmp	ip, #17
	beq	.L58
	mul	r5, r5, r0
	ldr	r8, .L65+4
	add	r0, r3, #255
	add	r3, r2, #31
	bic	r0, r0, #255
	str	r0, [fp, #-48]
	ldr	r0, [r8, r7, asl #2]
	bic	r4, r3, #31
	mov	ip, #0
	str	ip, [fp, #-76]
	mul	r5, r5, lr
	str	ip, [fp, #-72]
	add	r5, r5, #143
	bic	r5, r5, #127
.L35:
	ldr	r3, [r0, #1200]
	cmp	r3, #1
	beq	.L37
.L63:
	mov	r9, #0
	mov	r3, r5, asr #1
	mov	r10, r9
	str	r3, [fp, #-56]
	str	r9, [fp, #-60]
.L38:
	ldr	r3, [fp, #4]
	sub	r3, r3, #16
	str	r3, [fp, #-68]
	mov	ip, r3
	ldr	r3, [fp, #4]
	cmp	r3, #3
	cmpne	r3, #0
	movne	r3, #1
	moveq	r3, #0
	cmp	ip, #1
	movls	r3, #0
	andhi	r3, r3, #1
	cmp	r3, #0
	movne	r3, #1024
	strne	r3, [fp, #-52]
	bne	.L40
	add	r1, r1, #255
	adds	ip, r2, #63
	add	r3, r1, #255
	addmi	ip, r2, #126
	cmp	r1, #0
	mov	ip, ip, asr #6
	movlt	r1, r3
	mov	r1, r1, asr #8
	mov	ip, ip, asl #3
	mov	r1, r1, asl #6
	mul	ip, r1, ip
	add	r3, ip, #1024
	str	r3, [fp, #-52]
.L40:
	ldr	r3, [fp, #-64]
	cmp	r3, #1
	beq	.L59
	ldr	r3, [fp, #-76]
	add	r1, r10, r10, lsl #1
	ldr	r2, [fp, #-72]
	mul	r2, r3, r2
	ldr	r3, [fp, #-48]
	mul	r3, r3, r4
	add	r2, r2, r2, lsl #1
	mov	r2, r2, lsr #1
	add	r4, r3, r3, lsl #1
	add	r2, r2, r4, lsr #1
	add	r4, r2, r1, lsr #1
.L42:
	add	r2, r0, #307200
	ldr	r1, [r2, #1968]
	cmp	r1, #0
	beq	.L60
.L43:
	ldr	r2, [fp, #4]
	ldr	r1, [fp, #-60]
	cmp	r2, #17
	add	r2, r0, #311296
	str	r1, [r2, #1252]
	ldr	r1, [fp, #-56]
	str	r4, [r2, #1480]
	str	r9, [r2, #1256]
	str	r1, [r2, #1452]
	beq	.L44
	add	ip, r0, #307200
	ldr	r1, [ip, #1976]
	cmp	r1, #20
	movge	r1, #20
	str	r1, [r2, #1456]
	ldr	r1, [r0, #620]
	cmp	r1, #2
	ldreq	r1, [ip, #1980]
	streq	r1, [r2, #1456]
.L45:
	ldr	r1, [fp, #-68]
	cmp	r1, #1
	ldr	r1, [fp, #-48]
	mov	ip, r1, asl #4
	add	r1, r3, r10
	str	ip, [r2, #208]
	str	r1, [r2, #1276]
	bls	.L61
.L46:
	ldr	r3, [r8, r7, asl #2]
	mov	r0, #1024
	ldr	r2, [fp, #-52]
	add	r4, r4, r2
	add	r2, r3, #311296
	add	r4, r4, #32
	str	r5, [r2, #1484]
	add	r1, r4, r5
	str	r0, [r2, #1492]
	ldrsb	r3, [r3, #168]
	cmp	r3, #1
	addeq	r1, r1, r0
	cmp	r6, #0
	beq	.L53
	str	r4, [r6, #4]
	mov	r2, #1024
	ldr	r3, [r8, r7, asl #2]
	mov	r0, #0
	add	r3, r3, #307200
	ldr	r3, [r3, #1976]
	str	r5, [r6, #12]
	str	r3, [r6, #8]
	ldr	r3, [r8, r7, asl #2]
	add	r3, r3, #311296
	ldr	r3, [r3, #1456]
	str	r1, [r6]
	str	r2, [r6, #20]
	str	r3, [r6, #16]
	ldr	r3, [r8, r7, asl #2]
	add	r3, r3, #311296
	str	r4, [r3, #1488]
.L55:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L60:
	ldr	ip, .L65+8
	add	r0, r0, #311296
	add	r0, r0, #208
	movw	r2, #1288
	str	r3, [fp, #-64]
	ldr	ip, [ip, #48]
	blx	ip
	ldr	r0, [r8, r7, asl #2]
	ldr	r3, [fp, #-64]
	b	.L43
.L32:
	ldr	r8, .L65+4
	add	r0, r1, #255
	mul	r5, r3, lr
	bic	r3, r0, #255
	ldr	r0, [r8, r7, asl #2]
	str	r3, [fp, #-48]
	add	r5, r5, #143
	ldr	ip, [r0, #1492]
	bic	r5, r5, #127
	cmp	ip, #8
	ble	.L62
	ldr	r3, [fp, #-48]
	add	r10, r2, #31
	mov	r3, r3, lsr #2
	str	r3, [fp, #-72]
	bic	r3, r10, #31
	str	r3, [fp, #-76]
	ldr	r3, [r0, #1200]
	cmp	r3, #1
	bne	.L63
.L37:
	ldr	r3, [fp, #4]
	sub	r9, r3, #17
	clz	r9, r9
	mov	r9, r9, lsr #5
	cmp	r1, #1920
	orrgt	r9, r9, #1
	cmp	r9, #0
	beq	.L64
	add	r10, r2, #63
	mov	r3, r5, asr #1
	str	r3, [fp, #-56]
.L50:
	add	r3, r1, #2032
	add	ip, r1, #4080
	add	r3, r3, #15
	add	ip, ip, #14
	cmp	r3, #0
	movlt	r3, ip
	add	ip, r10, #63
	cmp	r10, #0
	mov	r3, r3, asr #11
	movlt	r10, ip
	mov	ip, r10, asr #6
	mov	r10, r3, asl #9
	mov	r3, r3, asl #4
	str	r3, [fp, #-60]
	mul	r10, ip, r10
	mov	r9, r10
	b	.L38
.L59:
	mov	r3, lr, asl #5
	ldr	r2, [fp, #-48]
	add	lr, r3, lr, lsl #4
	mul	r3, r2, r4
	mul	r4, r2, lr
	b	.L42
.L44:
	mov	r1, #1
	str	r1, [r2, #1456]
	b	.L45
.L61:
	ldr	r1, [r0, #1492]
	cmp	r1, #8
	ble	.L46
	ldr	ip, [fp, #-72]
	add	r1, r10, r10, lsl #1
	ldr	r0, [fp, #-76]
	add	r3, r3, r3, lsl #1
	mov	r1, r1, lsr #1
	add	r3, r1, r3, lsr #1
	ldr	r1, .L65+12
	mul	r0, ip, r0
	str	r3, [r2, #1268]
	mov	ip, ip, asl #4
	str	ip, [r2, #1260]
	add	r10, r0, r10, lsr #1
	mov	r0, #31
	str	r10, [r2, #1272]
	bl	dprint_vfmw
	b	.L46
.L58:
	ldr	r8, .L65+4
	add	r3, r1, #255
	bic	r3, r3, #255
	str	r3, [fp, #-48]
	add	r10, r2, #63
	ldr	r0, [r8, r7, asl #2]
	bic	r4, r10, #63
	ldr	r3, [r0, #1492]
	cmp	r3, #8
	bgt	.L48
	mov	r3, #0
	str	r3, [fp, #-76]
	str	r3, [fp, #-72]
.L36:
	ldr	r3, [r0, #1200]
	cmp	r3, #1
	mov	r3, #64
	beq	.L54
	mov	r9, #0
	mov	r5, #128
	movt	r3, 18
	str	r9, [fp, #-60]
	str	r3, [fp, #-56]
	mov	r10, r9
	movt	r5, 36
	b	.L38
.L64:
	mov	r3, r5, asr #1
	str	r9, [fp, #-60]
	str	r3, [fp, #-56]
	mov	r10, r9
	b	.L38
.L62:
	mov	r3, #0
	str	r3, [fp, #-76]
	str	r3, [fp, #-72]
	b	.L35
.L53:
	mov	r0, r6
	b	.L55
.L48:
	ldr	ip, [fp, #-48]
	add	r3, r4, #63
	bic	r3, r3, #63
	str	r3, [fp, #-76]
	mov	ip, ip, lsr #2
	str	ip, [fp, #-72]
	b	.L36
.L27:
	ldr	r3, .L65+16
	mov	r0, #0
	ldr	r2, .L65+20
	ldr	r1, .L65+24
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L55
.L54:
	mov	r5, #128
	movt	r3, 18
	movt	r5, 36
	str	r3, [fp, #-56]
	b	.L50
.L66:
	.align	2
.L65:
	.word	g_not_direct_8x8_inference_flag
	.word	s_pstVfmwChan
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC10
	.word	.LC9
	.word	.LANCHOR0+24
	.word	.LC1
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_CalcFsSize, .-VDMHAL_V400R003_CalcFsSize
	.align	2
	.global	VDMHAL_V400R003_GetRpuSize
	.type	VDMHAL_V400R003_GetRpuSize, %function
VDMHAL_V400R003_GetRpuSize:
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
	mov	r0, #1024
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_GetRpuSize, .-VDMHAL_V400R003_GetRpuSize
	.global	__aeabi_idiv
	.align	2
	.global	VDMHAL_V400R003_ArrangeMem
	.type	VDMHAL_V400R003_ArrangeMem, %function
VDMHAL_V400R003_ArrangeMem:
	UNWIND(.fnstart)
	@ args = 20, pretend = 0, frame = 104
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #116)
	sub	sp, sp, #116
	ldr	r5, [fp, #16]
	mov	r6, #0
	str	r0, [fp, #-108]
	cmn	r5, #2
	mov	r10, r1
	mov	r7, r2
	mov	r8, r3
	str	r6, [fp, #-96]
	str	r6, [fp, #-92]
	str	r6, [fp, #-88]
	str	r6, [fp, #-84]
	str	r6, [fp, #-80]
	str	r6, [fp, #-76]
	ldr	r4, [fp, #20]
	beq	.L147
	cmp	r5, r6
	blt	.L148
	mov	r0, r5
	ldr	r9, .L227
	bl	VCTRL_GetVidStd
	ldr	r2, [r9, r5, asl #2]
	ldr	r1, [r2, #1440]
	cmp	r1, #1
	str	r0, [fp, #-104]
	beq	.L208
.L70:
	ldr	r3, [fp, #-108]
	cmp	r3, #0
	subne	r2, r7, #32
	movwne	r3, #8160
	beq	.L209
	cmp	r2, r3
	bhi	.L96
	sub	r2, r8, #32
	cmp	r2, r3
	bhi	.L96
	cmp	r4, #0
	beq	.L210
	ldr	r2, [fp, #4]
	mov	r1, #0
	ldr	r3, .L227+4
	mov	r0, r4
	ldr	ip, [fp, #4]
	cmp	r2, #20
	movw	r2, #1272
	ldr	r3, [r3, #48]
	movge	r6, #20
	movlt	r6, ip
	blx	r3
	ldr	r2, .L227+8
	add	r3, r7, #15
	add	r1, r8, #15
	ldr	ip, [fp, #-104]
	mov	r3, r3, asr #4
	mov	r1, r1, asr #4
	ldrb	lr, [r2]	@ zero_extendqisi2
	cmp	r3, #45
	cmple	r1, #36
	mov	r0, r3, asl #4
	movle	r2, #64
	movgt	r2, #32
	cmp	lr, #1
	moveq	r2, #64
	cmp	ip, #16
	beq	.L211
	ldr	ip, [fp, #-104]
	cmp	ip, #17
	beq	.L104
	mul	r2, r3, r2
	mul	r2, r2, r1
	add	r2, r2, #127
	bic	r2, r2, #127
	mul	lr, r2, r6
	cmp	lr, r10
	bge	.L102
	ldr	r9, [fp, #-108]
	add	r0, r0, #255
	bic	ip, r0, #255
	cmp	r5, #0
	add	r3, r9, #1020
	rsb	lr, lr, r10
	add	r3, r3, #3
	str	r6, [r4, #1248]
	bic	r3, r3, #1020
	bic	r3, r3, #3
	rsb	r0, r9, r3
	str	r0, [fp, #-136]
	mov	r9, r0
	add	r0, r8, #31
	rsb	lr, r9, lr
	bic	r0, r0, #31
	str	lr, [fp, #-140]
	mov	lr, ip, asl #4
	str	r0, [fp, #-112]
	mov	r0, r2, asr #1
	str	lr, [r4]
	str	r0, [r4, #1244]
	blt	.L212
	ldr	r9, .L227
	mov	lr, #0
	str	lr, [fp, #-120]
	add	r0, r8, #63
	str	lr, [fp, #-116]
	add	r10, r7, #255
	ldr	lr, [r9, r5, asl #2]
	ldr	lr, [lr, #1200]
	cmp	lr, #1
	beq	.L109
.L225:
	mov	lr, #0
	cmp	r0, lr
	add	r7, r0, #63
	mov	r6, lr
	movge	r7, r0
	mov	r8, lr
	mov	r7, r7, asr #6
.L108:
	add	r0, r10, #255
	cmp	r10, #0
	mov	r7, r7, asl #3
	str	r6, [r4, #1044]
	movlt	r10, r0
	ldr	r0, [fp, #12]
	mov	r10, r10, asr #8
	str	lr, [r4, #1048]
	cmp	r0, #1
	mov	r10, r10, asl #6
	str	r10, [r4, #1032]
	mul	r7, r10, r7
	str	r7, [r4, #1036]
	beq	.L213
	ldr	r0, [fp, #-116]
	add	r6, r8, r8, lsl #1
	ldr	r1, [fp, #-120]
	mul	r1, r1, r0
	ldr	r0, [fp, #-112]
	mul	r0, r0, ip
	add	r1, r1, r1, lsl #1
	add	r1, r1, r1, lsr #31
	add	r0, r0, r0, lsl #1
	mov	r1, r1, asr #1
	add	r1, r1, r0, asr #1
	add	r6, r1, r6, lsr #1
.L112:
	cmp	r5, #0
	blt	.L113
	mov	r0, r5
	str	r2, [fp, #-132]
	str	r3, [fp, #-128]
	str	ip, [fp, #-124]
	bl	VCTRL_GetChanWidth
	mov	r10, r0
	mov	r0, r5
	bl	VCTRL_GetChanHeight
	sub	r2, fp, #132
	ldmia	r2, {r2, r3, ip}
	cmn	r0, #1
	cmnne	r10, #1
	moveq	r9, #1
	movne	r9, #0
	beq	.L214
	ldr	r1, [fp, #12]
	add	lr, r10, #15
	bic	lr, lr, #15
	add	r0, r0, #15
	cmp	r1, #1
	add	r1, lr, #255
	bic	r0, r0, #15
	bic	r1, r1, #255
	beq	.L215
	ldr	r10, .L227
	ldr	r10, [r10, r5, asl #2]
	ldr	r10, [r10, #1200]
	cmp	r10, #1
	beq	.L117
	adds	lr, r0, #63
	addmi	lr, r0, #126
	mov	r0, lr, asr #6
.L118:
	mul	r1, r0, r1
	mov	r10, r1, asl #7
	sub	r1, r10, r1, asl #5
	add	r9, r1, r9
.L116:
	cmp	r6, r9
	ldrgt	r3, .L227+12
	bgt	.L201
.L113:
	ldr	r1, [fp, #-112]
	ldr	r0, [fp, #-104]
	mul	r1, r1, ip
	sub	r0, r0, #16
	cmp	r0, #1
	add	r0, r1, r8
	str	r0, [r4, #1068]
	bls	.L216
	ldr	r1, [fp, #8]
	cmp	r1, #0
	beq	.L122
	ldr	r1, [fp, #-104]
	cmp	r1, #3
	cmpne	r1, #0
	beq	.L120
	str	r3, [fp, #-104]
	mov	r1, r6
	ldr	r3, [fp, #8]
	ldr	r0, [fp, #-140]
	cmp	r3, #32
	str	r2, [fp, #-112]
	movlt	r8, r3
	movge	r8, #32
	bl	__aeabi_idiv
	ldr	r3, [fp, #-104]
	ldr	r2, [fp, #-112]
	mov	r1, #0
	str	r1, [r4, #1040]
	cmp	r8, r0
	movlt	r1, r8
	movge	r1, r0
	str	r1, [r4, #1080]
.L123:
	cmp	r1, #0
	beq	.L121
	ldrgt	lr, [r4, #1252]
	movgt	ip, r4
	movgt	r0, #0
	ble	.L217
.L126:
	add	r0, r0, #1
	str	r3, [ip, #8]
	cmp	r0, r1
	add	r3, r3, r6
	add	ip, ip, #12
	bne	.L126
	ldr	r3, [fp, #-136]
	add	ip, r4, #388
	mov	r0, #0
	mla	r6, r1, r6, r3
	add	r3, r1, lr
	str	r3, [r4, #1252]
	ldr	r3, [fp, #-108]
	add	r3, r6, r3
.L128:
	add	r0, r0, #1
	str	r3, [ip, #4]!
	cmp	r0, r1
	add	r3, r3, #32
	bne	.L128
.L139:
	ldr	lr, [r4, #1040]
	add	r6, r6, r1, lsl #5
	cmp	lr, #0
	beq	.L130
	ldr	r3, [fp, #-108]
	add	ip, r4, #900
	add	r0, r6, r3
	mov	r3, #0
.L131:
	add	r3, r3, #1
	str	r0, [ip, #4]!
	cmp	r3, lr
	add	r0, r0, r7
	bne	.L131
	mul	lr, r7, lr
.L130:
	ldr	r7, [r4, #1248]
	ldr	r3, [fp, #-108]
	add	r6, lr, r6
	cmp	r7, #0
	add	lr, r6, r3
	ble	.L136
	ldr	r8, [r4, #1256]
	mov	ip, r4
	mov	r0, lr
	mov	r3, #0
.L135:
	add	r3, r3, #1
	str	r0, [ip, #1084]
	cmp	r3, r7
	add	r0, r0, r2
	add	ip, ip, #8
	bne	.L135
	add	r3, r7, r8
	str	r3, [r4, #1256]
.L136:
	mla	r2, r2, r7, r6
	cmp	r5, #0
	str	r2, [r4, #4]
	blt	.L134
	ldr	r3, .L227
	ldr	r3, [r3, r5, asl #2]
	cmp	r3, #0
	beq	.L134
	cmp	r1, #0
	ble	.L134
	add	r3, r2, lr
	mov	ip, r4
	mov	r0, #0
.L137:
	add	r0, r0, #1
	str	r3, [ip, #520]
	cmp	r0, r1
	add	r3, r3, #1024
	add	ip, ip, #12
	bne	.L137
.L134:
	mov	r3, #0
	add	r2, r2, r1, lsl #10
	str	r2, [r4, #4]
.L191:
	mov	r0, r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L148:
	mov	r3, #21
	str	r3, [fp, #-104]
	b	.L70
.L216:
	ldr	ip, [fp, #8]
	mov	r0, r8, lsr #1
	ldr	lr, [fp, #-120]
	add	r8, r8, r8, lsl #1
	cmp	ip, #0
	ldr	ip, [fp, #-116]
	add	r1, r1, r1, lsl #1
	mov	r8, r8, lsr #1
	mla	r0, lr, ip, r0
	add	r8, r8, r1, asr #1
	mov	r1, ip, asl #4
	str	r8, [r4, #1060]
	str	r1, [r4, #1052]
	str	r0, [r4, #1064]
	bne	.L120
.L122:
	mov	r3, #0
	str	r3, [r4, #1080]
	str	r3, [r4, #1040]
.L121:
	ldr	r3, .L227+16
.L201:
	mov	r0, #0
	ldr	r2, .L227+20
	ldr	r1, .L227+24
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L191
.L147:
	mov	r3, #16
	str	r3, [fp, #-104]
	b	.L70
.L211:
	mul	r2, r0, r1
	add	r2, r2, #127
	bic	r2, r2, #127
	mul	r0, r2, r6
	cmp	r0, r10
	bge	.L102
	ldr	ip, [fp, #-108]
	cmp	r5, #0
	rsb	r0, r0, r10
	add	r10, r7, #255
	add	r3, ip, #1020
	str	r6, [r4, #1248]
	add	r3, r3, #3
	bic	r3, r3, #1020
	bic	r3, r3, #3
	rsb	ip, ip, r3
	str	ip, [fp, #-136]
	mov	lr, ip
	bic	ip, r10, #255
	rsb	r0, lr, r0
	str	r0, [fp, #-140]
	mov	r0, r2, asr #1
	str	r0, [r4, #1244]
	blt	.L218
	ldr	r9, .L227
	str	r2, [fp, #-124]
	str	r3, [fp, #-120]
	ldr	r0, [r9, r5, asl #2]
	str	r1, [fp, #-116]
	add	r0, r0, #311296
	str	ip, [fp, #-112]
	add	r0, r0, #2096
	bl	IsMain10Profile
	ldr	ip, [fp, #-112]
	ldr	r1, [fp, #-116]
	ldr	r3, [fp, #-120]
	ldr	r2, [fp, #-124]
	cmp	r0, #0
	beq	.L106
	cmp	ip, #0
	add	lr, ip, #3
	add	r0, r8, #31
	str	r8, [fp, #-112]
	movge	lr, ip
	bic	r0, r0, #31
	mov	lr, lr, asr #2
	str	r0, [fp, #-120]
	str	lr, [fp, #-116]
	add	r0, r8, #63
	b	.L107
.L208:
	cmp	r0, #20
	bhi	.L219
	ldr	r0, [fp, #-104]
	add	r1, r2, #307200
	sub	lr, fp, #96
	ldr	r3, [fp, #12]
	ldr	r10, [r1, #1972]
	mov	r2, r8
	str	r0, [sp]
	mov	r1, r7
	mov	r0, r5
	str	lr, [sp, #4]
	bl	VDMHAL_V400R003_CalcFsSize
	cmp	r0, #0
	bne	.L220
	ldr	r3, [r9, r5, asl #2]
	ldr	r6, [fp, #-96]
	str	r3, [fp, #-108]
	cmp	r10, r6
	add	r3, r3, #307200
	ldr	r10, [r3, #1976]
	ldr	r2, [r3, #1992]
	bne	.L75
	cmp	r2, #1
	beq	.L75
	ldr	r3, [r3, #1968]
	cmp	r3, #1
	beq	.L75
	cmp	r4, #0
	beq	.L221
	ldr	r3, [fp, #-108]
	cmp	r10, #0
	add	r6, r3, #311296
	add	r3, r6, #208
	str	r3, [fp, #-132]
	ldr	r3, [r6, #1480]
	str	r3, [fp, #-112]
	ldrne	r3, [fp, #-108]
	beq	.L222
.L87:
	add	r3, r3, #307200
	ldr	r2, [r3, #1980]
	cmp	r2, #30
	bgt	.L223
.L88:
	ldr	r3, .L227+4
	mov	r0, #8
	str	r2, [r6, #1288]
	str	r3, [fp, #-128]
	ldr	r3, [r3, #12]
	blx	r3
	ldr	r0, [r9, r5, asl #2]
	add	r3, r0, #311296
	mov	ip, r3
	ldr	r3, [r3, #1520]
	cmp	r3, #0
	beq	.L93
	ldr	r1, [r6, #1244]
	movw	r2, #12565
	ldr	r3, [r6, #1460]
	movt	r2, 1
	str	r9, [fp, #-136]
	mov	lr, #0
	str	r1, [fp, #-116]
	add	r2, r3, r2
	ldr	r1, [r6, #1456]
	mov	r8, r3, asl #4
	mov	r2, r2, asl #2
	sub	r8, r8, r3, asl #2
	mov	r10, ip
	str	r5, [fp, #16]
	str	r1, [fp, #-120]
	add	r1, r3, #1
	str	r1, [fp, #-124]
	sub	r3, r2, #512
	ldr	r1, [fp, #-108]
	str	r4, [fp, #20]
	add	r2, r1, r2
	add	r8, r1, r8
	add	r3, r1, r3
	str	r3, [fp, #-144]
	mov	r9, r2
.L92:
	movw	r3, #47068
	add	r5, r0, #307200
	movt	r3, 4
	add	r3, r0, r3
	ldr	r2, [r5, #2032]
	add	r4, r8, #311296
	ldr	r3, [r3]
	mov	r7, #1
	ldr	r1, [fp, #-112]
	add	r8, r8, #12
	str	r7, [fp, #-104]
	str	r3, [r4, #220]
	add	r3, r2, #1020
	add	r3, r3, #3
	rsb	ip, r2, r1
	bic	r3, r3, #1020
	ldrb	r7, [fp, #-104]	@ zero_extendqisi2
	bic	r3, r3, #3
	str	r3, [r4, #216]
	add	ip, ip, r3
	ldr	r3, [fp, #-116]
	strb	r7, [r4, #224]
	movw	r1, #47060
	add	r3, r2, r3
	add	r2, ip, r2
	str	r2, [r9, #4]!
	add	ip, r3, ip
	ldr	r2, [fp, #-144]
	movt	r1, 4
	ldr	r3, [fp, #-124]
	add	r1, r0, r1
	add	r0, r0, #72
	add	r3, r3, lr
	str	ip, [r2, #4]!
	str	r3, [r6, #1460]
	add	lr, lr, #1
	ldr	r3, [r1]
	str	r2, [fp, #-144]
	cmp	r3, #255
	beq	.L90
	ldr	r2, [r6, #1468]
	ldr	ip, [r5, #2048]
	add	r1, r2, #1
	mov	r3, r2, asl #4
	sub	r3, r3, r2, asl #2
	add	r3, r6, r3
	str	ip, [r3, #728]
	str	r1, [r6, #1468]
.L90:
	ldr	r3, [r6, #1464]
	ldr	r2, [fp, #-120]
	ldr	r1, [fp, #-108]
	cmp	r3, r2
	add	r2, r3, #1
	add	r3, r1, r3, lsl #3
	ldrcc	r1, [r5, #2064]
	add	r3, r3, #311296
	movcc	ip, #1
	strccb	ip, [r3, #1296]
	strcc	r1, [r3, #1292]
	strcc	r2, [r6, #1464]
	ldr	r3, [r10, #1520]
	cmp	lr, r3
	bcc	.L92
	ldr	r5, [fp, #16]
	ldr	r4, [fp, #20]
	ldr	r9, [fp, #-136]
.L93:
	mov	r0, r4
	ldr	r4, [fp, #-128]
	ldr	r1, [fp, #-132]
	movw	r2, #1272
	ldr	r3, [r4, #52]
	blx	r3
	ldr	r3, [r9, r5, asl #2]
	mov	r1, #1
	ldr	r2, [r4, #16]
	add	r3, r3, #311296
	mov	r0, #8
	str	r1, [r3, #1524]
	blx	r2
	mov	r3, #0
	b	.L191
.L213:
	mov	r6, r1, asl #4
	mul	r6, r6, ip
	add	r6, r6, r6, lsl #1
	b	.L112
.L117:
	add	r10, lr, #2032
	adds	r9, r0, #63
	add	r10, r10, #15
	addmi	r9, r0, #126
	add	lr, lr, #4080
	cmp	r10, #0
	add	lr, lr, #14
	mov	r0, r9, asr #6
	movge	lr, r10
	mov	lr, lr, asr #11
	mov	r9, r0, asl #7
	sub	r9, r9, r0, asl #5
	mov	lr, lr, asl #4
	mul	r9, lr, r9
	mov	r9, r9, lsr #1
	b	.L118
.L75:
	str	r2, [sp, #4]
	mov	r3, r6
	ldr	r1, .L227+28
	mov	r0, #31
	str	r10, [sp]
	ldr	r2, .L227+32
	bl	dprint_vfmw
	ldr	r1, [r9, r5, asl #2]
	add	r1, r1, #307200
	ldr	r4, [r1, #1968]
	str	r6, [r1, #1972]
	cmp	r4, #0
	beq	.L224
	ldr	r3, .L227+36
	ldr	r0, [fp, #-88]
	ldr	ip, [r3]
	str	r0, [r1, #1980]
	cmp	ip, #0
	beq	.L199
	ldr	lr, [fp, #-80]
	mov	r3, #28
	ldr	r4, [fp, #-92]
	sub	r2, fp, #72
	str	r0, [fp, #-72]
	mov	r1, #122
	str	lr, [fp, #-64]
	mov	r0, r5
	ldr	lr, [fp, #-84]
	str	r7, [fp, #-52]
	str	r8, [fp, #-48]
	str	lr, [fp, #-60]
	ldr	lr, [fp, #-76]
	str	r4, [fp, #-68]
	str	lr, [fp, #-56]
	blx	ip
.L199:
	ldr	r3, .L227+4
	str	r3, [fp, #-128]
.L85:
	ldr	ip, [r9, r5, asl #2]
	mov	r3, r10
	mov	r2, r6
	ldr	r1, .L227+40
	add	ip, ip, #307200
	mov	r0, #31
	ldr	ip, [ip, #1992]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [fp, #-128]
	ldr	r4, [r9, r5, asl #2]
	ldr	r3, [r3]
	add	r4, r4, #311296
	blx	r3
	mov	r3, #2
	str	r0, [r4, #1496]
	mov	r0, r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L120:
	str	r3, [fp, #-104]
	add	r1, r6, r7
	ldr	r3, [fp, #8]
	ldr	r0, [fp, #-140]
	cmp	r3, #32
	str	r2, [fp, #-112]
	movlt	r8, r3
	movge	r8, #32
	bl	__aeabi_idiv
	ldr	r2, [fp, #-112]
	ldr	r3, [fp, #-104]
	cmp	r8, r0
	movlt	r1, r8
	movge	r1, r0
	str	r1, [r4, #1080]
	str	r1, [r4, #1040]
	b	.L123
.L104:
	mov	r2, r6, asl #21
	add	r2, r2, r6, lsl #18
	cmp	r2, r10
	bge	.L102
	ldr	ip, [fp, #-108]
	cmp	r5, #0
	rsb	r2, r2, r10
	add	r0, r8, #63
	add	r3, ip, #1020
	add	r10, r7, #255
	add	r3, r3, #3
	str	r6, [r4, #1248]
	bic	r3, r3, #1020
	bic	r3, r3, #3
	rsb	ip, ip, r3
	str	ip, [fp, #-136]
	rsb	r2, ip, r2
	bic	ip, r10, #255
	str	r2, [fp, #-140]
	bic	r2, r0, #63
	str	r2, [fp, #-112]
	mov	r2, #1179648
	str	r2, [r4, #1244]
	movlt	r2, #2359296
	blt	.L145
	ldr	r9, .L227
	ldr	r2, [r9, r5, asl #2]
	ldr	r2, [r2, #1492]
	cmp	r2, #8
	ble	.L151
	ldr	r2, [fp, #-112]
	cmp	ip, #0
	add	lr, ip, #3
	add	r2, r2, #63
	movge	lr, ip
	bic	r2, r2, #63
	str	r2, [fp, #-120]
	mov	r2, #2359296
	mov	lr, lr, asr #2
	str	lr, [fp, #-116]
.L107:
	mov	lr, ip, asl #4
	str	lr, [r4]
	ldr	lr, [r9, r5, asl #2]
	ldr	lr, [lr, #1200]
	cmp	lr, #1
	bne	.L225
.L109:
	cmp	r7, #1920
	ldr	r6, [fp, #-104]
	movle	lr, #0
	movgt	lr, #1
	cmp	r6, #17
	orreq	lr, lr, #1
	cmp	lr, #0
	bne	.L110
	cmp	r0, #0
	add	r7, r0, #63
	mov	r6, lr
	mov	r8, lr
	movge	r7, r0
	mov	r7, r7, asr #6
	b	.L108
.L218:
	add	r0, r8, #63
	str	r8, [fp, #-112]
.L145:
	mov	lr, #0
	cmp	r0, lr
	add	r7, r0, #63
	mov	r6, lr
	movge	r7, r0
	str	lr, [fp, #-120]
	mov	r0, ip, asl #4
	str	lr, [fp, #-116]
	mov	r7, r7, asr #6
	mov	r8, lr
	str	r0, [r4]
	b	.L108
.L215:
	mul	r9, r0, r1
	add	r9, r9, r9, lsl #1
	b	.L116
.L110:
	add	lr, r7, #2032
	add	r6, r7, #4080
	add	lr, lr, #15
	add	r6, r6, #14
	cmp	lr, #0
	add	r7, r0, #63
	movlt	lr, r6
	cmp	r0, #0
	mov	r6, lr, asr #11
	movge	r7, r0
	mov	r8, r6, asl #9
	mov	r7, r7, asr #6
	mov	r6, r6, asl #4
	mul	r8, r7, r8
	mov	lr, r8
	b	.L108
.L217:
	ldr	r3, [fp, #-136]
	mla	r6, r1, r6, r3
	b	.L139
.L212:
	adds	r0, r8, #63
	mov	lr, #0
	addmi	r0, r8, #126
	add	r10, r7, #255
	mov	r6, lr
	str	lr, [fp, #-120]
	mov	r7, r0, asr #6
	str	lr, [fp, #-116]
	mov	r8, lr
	b	.L108
.L106:
	mov	lr, r0
	str	r0, [fp, #-120]
	str	lr, [fp, #-116]
	add	r0, r8, #63
	str	r8, [fp, #-112]
	b	.L107
.L151:
	mov	r2, #0
	str	r2, [fp, #-120]
	mov	lr, r2
	mov	r2, #2359296
	str	lr, [fp, #-116]
	b	.L107
.L219:
	ldr	r3, .L227+44
	mov	r0, r6
	ldr	r2, .L227+20
	ldr	r1, .L227+24
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L191
.L224:
	add	r1, r1, #2000
	mov	r0, r5
	bl	DelAllFrameMemRecord
	cmp	r0, #0
	bne	.L226
.L78:
	ldr	r2, [r9, r5, asl #2]
	mov	r3, #0
	mov	r1, #255
	add	r2, r2, #307200
	add	r2, r2, #2000
.L79:
	str	r1, [r2, r3]
	add	r3, r3, #72
	cmp	r3, #2304
	bne	.L79
	cmp	r7, #1920
	cmple	r8, #1088
	ldr	r0, [fp, #-88]
	bgt	.L82
	ldr	r3, [r9, r5, asl #2]
	ldr	r2, [r3, #44]
	cmp	r2, #24
	beq	.L82
	add	r0, r0, #4
	str	r0, [fp, #-88]
.L81:
	ldr	r2, [r3, #1448]
	add	r1, r3, #307200
	ldr	ip, [fp, #-104]
	add	r2, r0, r2
	str	r2, [r1, #1980]
	subs	ip, ip, #17
	ldr	lr, [r3, #620]
	add	r3, r3, #311296
	movne	ip, #1
	cmp	lr, #2
	mov	lr, #0
	str	lr, [r3, #1520]
	movne	ip, #0
	cmp	ip, #0
	ldr	ip, .L227+36
	strne	r2, [fp, #-80]
	mov	r2, #1
	str	r2, [r1, #1952]
	ldr	ip, [ip]
	str	r2, [r3, #2044]
	cmp	ip, lr
	beq	.L84
	ldr	lr, [fp, #-80]
	mov	r3, #28
	ldr	r4, [fp, #-92]
	sub	r2, fp, #72
	str	r0, [fp, #-72]
	mov	r1, #121
	str	lr, [fp, #-64]
	mov	r0, r5
	ldr	lr, [fp, #-84]
	str	r7, [fp, #-52]
	str	r8, [fp, #-48]
	str	lr, [fp, #-60]
	ldr	lr, [fp, #-76]
	str	r4, [fp, #-68]
	str	lr, [fp, #-56]
	blx	ip
.L84:
	ldr	r3, .L227+4
	movw	r0, #50680
	mov	r2, #512
	mov	r1, #0
	movt	r0, 4
	mov	ip, r3
	str	r3, [fp, #-128]
	ldr	r3, [r9, r5, asl #2]
	add	r0, r3, r0
	ldr	r3, [ip, #48]
	blx	r3
	ldr	r3, [r9, r5, asl #2]
	ldr	r1, .L227+48
	mov	r2, #0
	add	r3, r3, #311296
	ldr	r0, [r1, r5, asl #2]
	str	r2, [r3, #2040]
	bl	FSP_ClearContextAll
	b	.L85
.L96:
	ldr	r3, .L227+52
	b	.L201
.L82:
	ldr	r3, [r9, r5, asl #2]
	b	.L81
.L209:
	mov	r0, r3
	ldr	r2, .L227+20
	ldr	r3, .L227+56
	ldr	r1, .L227+24
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L191
.L226:
	mov	r0, r4
	ldr	r1, .L227+60
	bl	dprint_vfmw
	b	.L78
.L102:
	ldr	r3, .L227+64
	b	.L201
.L222:
	ldr	r1, .L227+68
	mov	r0, #31
	bl	dprint_vfmw
	ldr	r3, [r9, r5, asl #2]
	b	.L87
.L223:
	ldr	r1, .L227+72
	mov	r0, #31
	bl	dprint_vfmw
	ldr	r3, [r9, r5, asl #2]
	mov	r1, #30
	add	r3, r3, #307200
	mov	r2, r1
	str	r1, [r3, #1980]
	b	.L88
.L214:
	ldr	r1, .L227+76
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L191
.L210:
	ldr	r3, .L227+80
	mov	r0, r4
	ldr	r2, .L227+20
	ldr	r1, .L227+24
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L191
.L220:
	ldr	r1, .L227+84
	mov	r0, #31
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L191
.L221:
	ldr	r3, .L227+88
	ldr	r2, .L227+92
	ldr	r1, .L227+24
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L191
.L228:
	.align	2
.L227:
	.word	s_pstVfmwChan
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_not_direct_8x8_inference_flag
	.word	.LC23
	.word	.LC24
	.word	.LANCHOR0+52
	.word	.LC1
	.word	.LC13
	.word	.LANCHOR0+80
	.word	g_event_report
	.word	.LC15
	.word	.LC11
	.word	s_pFspInst
	.word	.LC9
	.word	.LC19
	.word	.LC14
	.word	.LC21
	.word	.LC17
	.word	.LC18
	.word	.LC22
	.word	.LC20
	.word	.LC12
	.word	.LC16
	.word	.LANCHOR0+108
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_ArrangeMem, .-VDMHAL_V400R003_ArrangeMem
	.align	2
	.global	VDMHAL_V400R003_ResetVdm
	.type	VDMHAL_V400R003_ResetVdm, %function
VDMHAL_V400R003_ResetVdm:
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
	subs	r4, r0, #0
	bgt	.L248
	bne	.L245
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r2, r0, #0
	beq	.L249
	ldr	r7, .L252
	ldr	r9, .L252+4
	ldr	r2, [r7]
	ldr	r3, [r9]
	ldr	r8, [r2, #36]
	ldr	r6, [r3, #120]
	tst	r6, #1
	bne	.L250
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L245:
	ldr	r1, .L252+8
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L248:
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L252+12
	ldr	r1, .L252+16
	bl	dprint_vfmw
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L250:
	ldr	r5, .L252+20
	mov	r2, #2
	bfi	r6, r2, #8, #2
	str	r6, [r3, #120]
	orr	r6, r6, #64
	ldr	r3, [r5, #108]
	blx	r3
	ldr	r3, [r9]
	str	r6, [r3, #120]
	b	.L237
.L251:
	add	r4, r4, #1
	cmp	r4, #1000
	beq	.L238
.L237:
	ldr	r3, [r5, #112]
	mov	r0, #30
	blx	r3
	ldr	r3, [r9]
	ldr	r3, [r3, #372]
	tst	r3, #4
	beq	.L251
	cmp	r4, #1000
	bge	.L238
	mov	r3, #0
	ldr	r2, .L252+12
	mov	r0, r3
	ldr	r1, .L252+24
	bl	dprint_vfmw
.L240:
	ldr	r2, [r9]
	bfc	r6, #6, #1
	ldr	r3, [r5, #108]
	str	r6, [r2, #120]
	bfc	r6, #8, #2
	blx	r3
	ldr	r2, [r9]
	ldr	r3, [r7]
	str	r6, [r2, #120]
	str	r8, [r3, #36]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L238:
	mov	r3, #0
	ldr	r2, .L252+12
	mov	r0, r3
	ldr	r1, .L252+28
	bl	dprint_vfmw
	b	.L240
.L249:
	ldr	r1, .L252+32
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L253:
	.align	2
.L252:
	.word	g_HwMem
	.word	g_pstRegCrg
	.word	.LC26
	.word	.LANCHOR0+144
	.word	.LC25
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC29
	.word	.LC28
	.word	.LC27
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_ResetVdm, .-VDMHAL_V400R003_ResetVdm
	.align	2
	.global	VDMHAL_V400R003_SetSmmuPageTableAddr
	.type	VDMHAL_V400R003_SetSmmuPageTableAddr, %function
VDMHAL_V400R003_SetSmmuPageTableAddr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #24)
	sub	sp, sp, #24
	ldr	r3, .L260
	mov	r4, r0
	ldr	ip, [r3, #176]
	cmp	ip, #0
	beq	.L254
	sub	r2, fp, #24
	sub	r1, fp, #28
	sub	r0, fp, #32
	blx	ip
	cmp	r4, #0
	bne	.L256
	ldr	r3, .L260+4
	ldr	r0, [fp, #-32]
	ldr	r1, [fp, #-28]
	ldr	r3, [r3]
	ldr	r2, [fp, #-24]
	add	r3, r3, #61440
	str	r0, [r3, #524]
	str	r1, [r3, #772]
	str	r2, [r3, #776]
.L254:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L256:
	mov	r5, #1
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L260+8
	ldr	r1, .L260+12
	bl	dprint_vfmw
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L260+8
	ldr	r1, .L260+12
	bl	dprint_vfmw
	str	r5, [sp]
	mov	r3, r4
	ldr	r2, .L260+8
	ldr	r1, .L260+12
	mov	r0, #32
	bl	dprint_vfmw
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L261:
	.align	2
.L260:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_HwMem
	.word	.LANCHOR0+172
	.word	.LC30
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_SetSmmuPageTableAddr, .-VDMHAL_V400R003_SetSmmuPageTableAddr
	.align	2
	.global	VDMHAL_V400R003_EnableSmmu
	.type	VDMHAL_V400R003_EnableSmmu, %function
VDMHAL_V400R003_EnableSmmu:
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
	ldr	r3, .L263
	mov	r2, #8
	movt	r2, 3
	ldr	r3, [r3]
	add	r3, r3, #61440
	str	r2, [r3]
	ldmfd	sp, {fp, sp, pc}
.L264:
	.align	2
.L263:
	.word	g_HwMem
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_EnableSmmu, .-VDMHAL_V400R003_EnableSmmu
	.align	2
	.global	VDMHAL_V400R003_GlbResetX
	.type	VDMHAL_V400R003_GlbResetX, %function
VDMHAL_V400R003_GlbResetX:
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
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	cmp	r0, #0
	beq	.L281
	cmp	r4, #0
	bne	.L280
	ldr	r5, .L283
	ldr	r3, [r5]
	ldr	r6, [r3, #120]
	tst	r6, #1
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r7, .L283+4
	mov	r2, #2
	bfi	r6, r2, #8, #2
	str	r6, [r3, #120]
	orr	r6, r6, #16
	ldr	r3, [r7, #108]
	blx	r3
	ldr	r3, [r5]
	str	r6, [r3, #120]
	b	.L271
.L282:
	add	r4, r4, #1
	cmp	r4, #1000
	beq	.L272
.L271:
	ldr	r3, [r7, #112]
	mov	r0, #30
	blx	r3
	ldr	r3, [r5]
	ldr	r3, [r3, #372]
	tst	r3, #1
	beq	.L282
	cmp	r4, #1000
	bge	.L272
	mov	r3, #0
	ldr	r2, .L283+8
	mov	r0, r3
	ldr	r1, .L283+12
	bl	dprint_vfmw
.L274:
	ldr	r2, [r5]
	bfc	r6, #4, #1
	ldr	r3, [r7, #108]
	str	r6, [r2, #120]
	bfc	r6, #8, #2
	blx	r3
	ldr	r3, [r5]
	str	r6, [r3, #120]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L280:
	ldr	r1, .L283+16
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L272:
	mov	r3, #0
	ldr	r2, .L283+8
	mov	r0, r3
	ldr	r1, .L283+20
	bl	dprint_vfmw
	b	.L274
.L281:
	mov	r3, #0
	ldr	r2, .L283+8
	movt	r3, 63683
	ldr	r1, .L283+24
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L284:
	.align	2
.L283:
	.word	g_pstRegCrg
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+212
	.word	.LC33
	.word	.LC34
	.word	.LC32
	.word	.LC31
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_GlbResetX, .-VDMHAL_V400R003_GlbResetX
	.align	2
	.global	VDMHAL_V400R003_GlbReset
	.type	VDMHAL_V400R003_GlbReset, %function
VDMHAL_V400R003_GlbReset:
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
	movt	r0, 63683
	bl	MEM_Phy2Vir
	cmp	r0, #0
	beq	.L287
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDMHAL_V400R003_GlbResetX
.L287:
	mov	r3, #0
	ldr	r2, .L288
	movt	r3, 63683
	ldr	r1, .L288+4
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L289:
	.align	2
.L288:
	.word	.LANCHOR0+240
	.word	.LC31
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_GlbReset, .-VDMHAL_V400R003_GlbReset
	.align	2
	.global	VDMHAL_V400R003_ClearIntState
	.type	VDMHAL_V400R003_ClearIntState, %function
VDMHAL_V400R003_ClearIntState:
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
	ldr	r5, .L309
	mov	r3, r0, asl #6
	sub	r3, r3, r0, asl #3
	ldr	r2, .L309+4
	add	r3, r5, r3
	ldr	r3, [r3, #8]
	ldr	r3, [r2, r3, asl #2]
	cmp	r3, #0
	beq	.L291
	cmp	r0, #0
	ldr	r4, [r3, #1224]
	bne	.L292
.L293:
	ldr	r6, .L309+8
	ldr	r3, [r6]
	cmp	r3, #0
	beq	.L307
.L294:
	cmp	r4, #1
	beq	.L308
.L305:
	mvn	r2, #0
	str	r2, [r3, #32]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L291:
	cmp	r0, #0
	moveq	r4, r0
	beq	.L293
.L292:
	ldr	r1, .L309+12
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L308:
	ldr	r2, [r5, #44]
	cmp	r2, #1
	beq	.L305
	cmp	r2, #2
	ldr	r1, [r3, #28]
	mvneq	r2, #11
	streq	r2, [r3, #32]
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	cmp	r2, #3
	mvneq	r2, #14
	streq	r2, [r3, #32]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L307:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r6]
	bne	.L294
.L295:
	ldr	r1, .L309+16
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L310:
	.align	2
.L309:
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	g_HwMem
	.word	.LC35
	.word	.LC36
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_ClearIntState, .-VDMHAL_V400R003_ClearIntState
	.align	2
	.global	VDMHAL_V400R003_ClearMMUIntState
	.type	VDMHAL_V400R003_ClearMMUIntState, %function
VDMHAL_V400R003_ClearMMUIntState:
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
	cmp	r0, #0
	bne	.L317
	ldr	r4, .L319
	ldr	r3, [r4]
	cmp	r3, #0
	beq	.L318
.L314:
	add	r3, r3, #61440
	mov	r2, #7
	str	r2, [r3, #44]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L317:
	ldr	r1, .L319+4
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L318:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r4]
	bne	.L314
.L315:
	ldr	r1, .L319+8
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L320:
	.align	2
.L319:
	.word	g_HwMem
	.word	.LC35
	.word	.LC36
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_ClearMMUIntState, .-VDMHAL_V400R003_ClearMMUIntState
	.align	2
	.global	VDMHAL_V400R003_MaskInt
	.type	VDMHAL_V400R003_MaskInt, %function
VDMHAL_V400R003_MaskInt:
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
	cmp	r0, #0
	bne	.L327
	ldr	r4, .L329
	ldr	r3, [r4]
	cmp	r3, #0
	beq	.L328
.L324:
	mvn	r2, #0
	str	r2, [r3, #36]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L327:
	ldr	r1, .L329+4
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L328:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r4]
	bne	.L324
.L325:
	ldr	r1, .L329+8
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L330:
	.align	2
.L329:
	.word	g_HwMem
	.word	.LC37
	.word	.LC36
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_MaskInt, .-VDMHAL_V400R003_MaskInt
	.align	2
	.global	VDMHAL_V400R003_GetIntMaskCfg
	.type	VDMHAL_V400R003_GetIntMaskCfg, %function
VDMHAL_V400R003_GetIntMaskCfg:
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
	cmp	r0, #1
	mvneq	r0, #5
	mvnne	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_GetIntMaskCfg, .-VDMHAL_V400R003_GetIntMaskCfg
	.align	2
	.global	VDMHAL_V400R003_EnableInt
	.type	VDMHAL_V400R003_EnableInt, %function
VDMHAL_V400R003_EnableInt:
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
	ldr	r2, .L349
	mov	r3, r0, asl #6
	sub	r3, r3, r0, asl #3
	ldr	r1, .L349+4
	add	r3, r2, r3
	ldr	r3, [r3, #8]
	ldr	r3, [r1, r3, asl #2]
	cmp	r3, #0
	beq	.L335
	cmp	r0, #0
	ldr	r4, [r3, #1224]
	bne	.L336
.L337:
	ldr	r5, .L349+8
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L348
.L338:
	cmp	r4, #1
	mvneq	r2, #5
	mvnne	r2, #1
	str	r2, [r3, #36]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L335:
	cmp	r0, #0
	moveq	r4, r0
	beq	.L337
.L336:
	ldr	r1, .L349+12
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L348:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r5]
	bne	.L338
.L339:
	ldr	r1, .L349+16
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L350:
	.align	2
.L349:
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	g_HwMem
	.word	.LC38
	.word	.LC36
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_EnableInt, .-VDMHAL_V400R003_EnableInt
	.align	2
	.global	VDMHAL_V400R003_CheckReg
	.type	VDMHAL_V400R003_CheckReg, %function
VDMHAL_V400R003_CheckReg:
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
	cmp	r1, #0
	mov	r5, r0
	bgt	.L373
	bne	.L371
	ldr	r4, .L375
	ldr	r3, [r4]
	cmp	r3, #0
	beq	.L374
.L357:
	sub	r2, r5, #1
	cmp	r2, #6
	ldrls	pc, [pc, r2, asl #2]
	b	.L363
.L365:
	.word	.L368
	.word	.L366
	.word	.L367
	.word	.L369
	.word	.L359
	.word	.L361
	.word	.L362
.L369:
	mov	r3, #40
.L364:
	ldr	r2, [r4]
	ldr	r0, [r2, r3]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L368:
	mov	r3, #28
	b	.L364
.L367:
	mov	r3, #36
	b	.L364
.L362:
	movw	r3, #62272
	b	.L364
.L366:
	mov	r3, #32
	b	.L364
.L359:
	movw	r3, #61480
	b	.L364
.L361:
	movw	r3, #62304
	b	.L364
.L371:
	ldr	r2, .L375+4
	mov	r0, #0
	ldr	r1, .L375+8
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L363:
	mov	r3, r5
	ldr	r2, .L375+4
	ldr	r1, .L375+12
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L374:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r6, r0, #0
	strne	r6, [r4]
	bne	.L357
.L358:
	ldr	r2, .L375+4
	ldr	r1, .L375+16
	bl	dprint_vfmw
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L373:
	mov	r3, r1
	ldr	r2, .L375+4
	ldr	r1, .L375+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L376:
	.align	2
.L375:
	.word	g_HwMem
	.word	.LANCHOR0+268
	.word	.LC40
	.word	.LC42
	.word	.LC41
	.word	.LC39
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_CheckReg, .-VDMHAL_V400R003_CheckReg
	.align	2
	.global	VDMHAL_V400R003_ReadMMUMask
	.type	VDMHAL_V400R003_ReadMMUMask, %function
VDMHAL_V400R003_ReadMMUMask:
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
	subs	r3, r0, #0
	ble	.L380
	mov	r1, #1
	ldr	r2, .L381
	str	r1, [sp]
	mov	r0, #32
	ldr	r1, .L381+4
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L380:
	movw	r0, #1228
	ldr	r2, .L381+8
	mul	r3, r0, r3
	ldr	r3, [r2, r3]
	add	r3, r3, #61440
	ldr	r0, [r3, #32]
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L382:
	.align	2
.L381:
	.word	.LANCHOR0+296
	.word	.LC43
	.word	g_HwMem
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_ReadMMUMask, .-VDMHAL_V400R003_ReadMMUMask
	.align	2
	.global	VDMHAL_V400R003_WriteMMUMask
	.type	VDMHAL_V400R003_WriteMMUMask, %function
VDMHAL_V400R003_WriteMMUMask:
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
	subs	r3, r1, #0
	ble	.L386
	mov	r1, #1
	ldr	r2, .L387
	str	r1, [sp]
	mov	r0, #32
	ldr	r1, .L387+4
	bl	dprint_vfmw
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L386:
	movw	r1, #1228
	ldr	r2, .L387+8
	mul	r3, r1, r3
	ldr	r3, [r2, r3]
	add	r3, r3, #61440
	str	r0, [r3, #32]
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L388:
	.align	2
.L387:
	.word	.LANCHOR0+324
	.word	.LC30
	.word	g_HwMem
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_WriteMMUMask, .-VDMHAL_V400R003_WriteMMUMask
	.align	2
	.global	VDMHAL_V400R003_PrepareDec
	.type	VDMHAL_V400R003_PrepareDec, %function
VDMHAL_V400R003_PrepareDec:
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
	mov	r4, r0
	mov	r5, r2
	mov	r6, r3
	beq	.L409
	cmp	r2, #0
	bgt	.L410
	cmp	r0, #6
	mov	r2, #0
	beq	.L411
	mov	r1, r2
	mov	r0, r2
	bl	SCD_ConfigReg
	mov	r3, r6
	mov	r2, #0
	mov	r1, #1
	mov	r0, #8
	bl	SCD_ConfigReg
	cmp	r4, #17
	ldrls	pc, [pc, r4, asl #2]
	b	.L391
.L396:
	.word	.L405
	.word	.L397
	.word	.L398
	.word	.L399
	.word	.L391
	.word	.L400
	.word	.L394
	.word	.L391
	.word	.L401
	.word	.L402
	.word	.L403
	.word	.L403
	.word	.L403
	.word	.L404
	.word	.L391
	.word	.L405
	.word	.L406
	.word	.L407
.L411:
	mov	r0, r2
	mov	r1, #1
	bl	SCD_ConfigReg
	mov	r3, r6
	mov	r2, #0
	mov	r1, #1
	mov	r0, #8
	bl	SCD_ConfigReg
.L394:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	AVSHAL_V400R003_StartDec
.L405:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	H264HAL_V400R003_StartDec
.L406:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	HEVCHAL_V400R003_StartDec
.L407:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VP9HAL_V400R003_StartDec
.L397:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VC1HAL_V400R003_StartDec
.L398:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	MP4HAL_V400R003_StartDec
.L399:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	MP2HAL_V400R003_StartDec
.L400:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	DIVX3HAL_V400R003_StartDec
.L401:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	RV8HAL_V400R003_StartDec
.L402:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	RV9HAL_V400R003_StartDec
.L403:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VP6HAL_V400R003_StartDec
.L404:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VP8HAL_V400R003_StartDec
.L409:
	mov	r3, r7
	mov	r0, r7
	ldr	r2, .L412
	ldr	r1, .L412+4
	bl	dprint_vfmw
.L391:
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L410:
	mov	r3, r2
	mov	r0, #0
	ldr	r2, .L412
	str	r0, [sp]
	ldr	r1, .L412+8
	bl	dprint_vfmw
	b	.L391
.L413:
	.align	2
.L412:
	.word	.LANCHOR0+356
	.word	.LC44
	.word	.LC25
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_PrepareDec, .-VDMHAL_V400R003_PrepareDec
	.align	2
	.global	VDMHAL_V400R003_IsVdmReady
	.type	VDMHAL_V400R003_IsVdmReady, %function
VDMHAL_V400R003_IsVdmReady:
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
	movw	r2, #1228
	mul	r2, r2, r0
	ldr	r3, .L421
	ldr	r3, [r3, r2]
	cmp	r3, #0
	beq	.L419
	cmp	r0, #0
	ble	.L420
	mov	r3, r0
	mov	r1, #1
	ldr	r2, .L421+4
	mov	r0, #32
	str	r1, [sp]
	ldr	r1, .L421+8
	bl	dprint_vfmw
	mov	r0, #0
.L416:
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L420:
	ldr	r0, [r3, #28]
	ubfx	r0, r0, #17, #1
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L419:
	mov	r0, r3
	ldr	r2, .L421+4
	ldr	r3, .L421+12
	ldr	r1, .L421+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L416
.L422:
	.align	2
.L421:
	.word	g_HwMem
	.word	.LANCHOR0+384
	.word	.LC43
	.word	.LC45
	.word	.LC1
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_IsVdmReady, .-VDMHAL_V400R003_IsVdmReady
	.align	2
	.global	VDMHAL_V400R003_IsVdmRun
	.type	VDMHAL_V400R003_IsVdmRun, %function
VDMHAL_V400R003_IsVdmRun:
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
	movw	r2, #1228
	mul	r2, r2, r0
	ldr	r3, .L430
	ldr	r4, [r3, r2]
	cmp	r4, #0
	beq	.L428
	cmp	r0, #0
	ble	.L429
	mov	r3, r0
	mov	r4, #1
	ldr	r2, .L430+4
	mov	r0, #32
	str	r4, [sp]
	ldr	r1, .L430+8
	bl	dprint_vfmw
	mov	r0, r4
.L425:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L429:
	ldr	r0, [r4, #40]
	subs	r0, r0, #1
	movne	r0, #1
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L428:
	ldr	r1, .L430+12
	mov	r0, r4
	bl	dprint_vfmw
	mov	r0, r4
	b	.L425
.L431:
	.align	2
.L430:
	.word	g_HwMem
	.word	.LANCHOR0+412
	.word	.LC43
	.word	.LC46
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_IsVdmRun, .-VDMHAL_V400R003_IsVdmRun
	.align	2
	.global	VDMHAL_V400R003_BackupInfo
	.type	VDMHAL_V400R003_BackupInfo, %function
VDMHAL_V400R003_BackupInfo:
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
	subs	r3, r0, #0
	beq	.L465
	ldr	r2, [r3, #4]
	ldr	r5, [r3]
	cmp	r2, #0
	bne	.L459
	ldr	r3, .L470
	ldr	r2, [r3]
	cmp	r2, #1
	addne	r1, r3, #60
	beq	.L454
.L439:
	ldr	r2, [r3, #4]!
	cmp	r2, #1
	beq	.L454
	cmp	r3, r1
	bne	.L439
	mov	r4, #0
.L437:
	ldr	r8, .L470+4
	ldr	ip, [r8]
	cmp	ip, #0
	beq	.L466
.L440:
	ldr	r3, .L470+8
	ldr	lr, [ip, #12]
	ldr	r6, .L470+12
	ldr	r2, [r3, #52]
	ldr	r3, [ip, #28]
	cmp	r2, #1
	str	lr, [r6]
	str	r3, [r6, #4]
	beq	.L467
	cmp	r4, #1
	beq	.L468
	add	r3, ip, #4096
	cmp	r4, #0
	ldr	r2, [r3]
	ldr	r3, [r3, #12]
	ldr	r7, [ip, #208]
	ldr	r8, [ip, #212]
	str	r2, [r6, #16]
	str	r3, [r6, #20]
	str	r7, [r6, #8]
	str	r8, [r6, #12]
	bne	.L445
	ldr	r9, [ip, #176]
	ldr	r0, [ip, #180]
	ldr	r1, [ip, #184]
	ldr	r2, [ip, #188]
	ldr	r3, [ip, #192]
	str	r9, [r6, #24]
	str	r0, [r6, #28]
	str	r1, [r6, #32]
	str	r2, [r6, #36]
	str	r3, [r6, #40]
.L445:
	ldr	r3, .L470+16
	add	r2, ip, #33024
	add	r0, r3, #128
.L446:
	ldr	r1, [r2], #4
	str	r1, [r3, #4]!
	cmp	r3, r0
	bne	.L446
	cmp	r4, #0
	bne	.L452
	add	ip, ip, #32768
	ldr	r2, [ip, #384]
	ldr	r3, [ip, #388]
	str	r2, [r6, #48]
	str	r3, [r6, #44]
.L452:
	and	lr, lr, #15
	cmp	lr, #5
	andeq	r8, r8, #1
	andne	r7, r7, #31
	moveq	r0, #0
	streq	r8, [r5]
	strne	r7, [r5]
	movne	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L454:
	mov	r4, r2
	b	.L437
.L467:
	add	r3, ip, #4096
	mov	r0, #0
	ldr	r3, [r3, #12]
	ldr	r2, [ip, #176]
	ubfx	r3, r3, #0, #20
	str	r3, [r6, #20]
	str	r2, [r6, #24]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L468:
	ldr	r0, [r8, #40]
	ubfx	r7, r3, #0, #17
	bl	MEM_Phy2Vir
	subs	r2, r0, #0
	beq	.L469
	sub	r3, r7, #1
	ldr	ip, [r8]
	cmp	r3, #199
	ldr	lr, [r6]
	subls	r3, r7, #-1073741823
	movhi	r3, #0
	movhi	r1, #8
	movls	r3, r3, asl #4
	ldr	r7, [ip, #208]
	addls	r1, r3, #8
	ldr	r8, [ip, #212]
	ldr	r3, [r2, r3]
	str	r3, [r6, #16]
	ldr	r3, [r2, r1]
	str	r7, [r6, #8]
	str	r8, [r6, #12]
	str	r3, [r6, #20]
	b	.L445
.L466:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	ip, r0, #0
	beq	.L441
	str	ip, [r8]
	b	.L440
.L459:
	ldr	r1, .L470+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L465:
	ldr	r3, .L470+24
	ldr	r2, .L470+28
	ldr	r1, .L470+32
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L441:
	ldr	r1, .L470+36
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L469:
	ldr	r3, .L470+40
	ldr	r2, .L470+28
	ldr	r1, .L470+32
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L471:
	.align	2
.L470:
	.word	g_DSPState
	.word	g_HwMem
	.word	g_VdmDrvParam
	.word	g_BackUp
	.word	g_BackUp+48
	.word	.LC48
	.word	.LC47
	.word	.LANCHOR0+440
	.word	.LC1
	.word	.LC36
	.word	.LC49
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_BackupInfo, .-VDMHAL_V400R003_BackupInfo
	.align	2
	.global	VDMHAL_V400R003_ReadMsgSlot
	.type	VDMHAL_V400R003_ReadMsgSlot, %function
VDMHAL_V400R003_ReadMsgSlot:
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
	beq	.L476
	cmp	r2, #800
	bhi	.L477
	ldr	r3, .L478
	mov	r2, r2, asl #2
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L477:
	mov	r0, r4
	ldr	r1, .L478+4
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L476:
	mov	r2, r0
	ldr	r1, .L478+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L479:
	.align	2
.L478:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC51
	.word	.LC50
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_ReadMsgSlot, .-VDMHAL_V400R003_ReadMsgSlot
	.align	2
	.global	VDMHAL_V400R003_WriteMsgSlot
	.type	VDMHAL_V400R003_WriteMsgSlot, %function
VDMHAL_V400R003_WriteMsgSlot:
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
	beq	.L481
	sub	r3, r2, #1
	cmp	r3, #255
	bhi	.L481
	ldr	r3, .L484
	mov	r2, r2, asl #2
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L481:
	ldr	r1, .L484+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L485:
	.align	2
.L484:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC52
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_WriteMsgSlot, .-VDMHAL_V400R003_WriteMsgSlot
	.global	__aeabi_uidiv
	.global	__aeabi_uidivmod
	.align	2
	.global	VDMHAL_V400R003_CfgRpMsg
	.type	VDMHAL_V400R003_CfgRpMsg, %function
VDMHAL_V400R003_CfgRpMsg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 56
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
	mov	r9, r0
	ldr	r0, [r1, #48]
	str	r2, [fp, #-72]
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	str	r3, [fp, #-76]
	beq	.L536
	ldr	r2, [r9, #840]
	sub	r3, r2, #1
	cmp	r3, #199
	bhi	.L537
	ldr	r2, [r9, #24]
	ldr	r3, [r9, #864]
	ldr	r0, [r9, #28]
	mov	r2, r2, asl r3
	cmp	r2, #1920
	add	r1, r2, #255
	mov	r3, r0, asl r3
	bic	r1, r1, #255
	ble	.L490
	ldr	r0, [r9, #848]
	ldr	ip, [r9]
	cmp	r0, #1
	movne	r2, #0
	beq	.L491
.L492:
	cmp	ip, #16
	beq	.L494
.L517:
	add	r3, r3, #31
	mov	r0, #8
	bic	r3, r3, #31
	str	r0, [fp, #-96]
	mla	r3, r3, r1, r2
.L518:
	ldr	r0, [r9, #16]
	ldr	ip, [fp, #-76]
	str	r0, [ip]
	ldr	r0, [r9, #16]
	add	r0, r3, r0
	str	r0, [ip, #4]
	ldr	r0, [r9, #4]
	str	r0, [ip, #8]
	ldr	r0, [r9, #4]
	add	r3, r3, r0
	str	r3, [ip, #12]
	ldr	ip, [fp, #-76]
	mov	r0, #0	@ movhi
	ldr	r3, [r9, #872]
	cmp	r3, #0
	moveq	r1, r1, asl #4
	str	r1, [ip, #16]
	str	r2, [ip, #20]
	mov	r1, r0	@ movhi
	ldr	r2, [r9, #24]
	ldr	r3, [r9, #28]
	sub	r2, r2, #1
	sub	r3, r3, #1
	bfi	r0, r2, #0, #9
	bfi	r1, r3, #0, #9
	strh	r0, [fp, #-48]	@ movhi
	strh	r1, [fp, #-46]	@ movhi
	ldr	r3, [fp, #-48]
	str	r3, [ip, #24]
	ldr	r3, [r9, #852]
	ldr	r2, [r9, #24]
	sub	r3, r3, #1
	cmp	r3, #1
	ldr	r3, [r9, #28]
	str	r2, [fp, #-92]
	addls	r3, r3, r3, lsr #31
	movls	r3, r3, asr #1
	str	r3, [fp, #-100]
	ldr	r3, [r9, #864]
	cmp	r3, #6
	moveq	r4, #2
	beq	.L499
	cmp	r3, #5
	moveq	r4, #4
	beq	.L499
	cmp	r3, #4
	moveq	r4, #8
	beq	.L499
	ldr	r1, .L543
	mov	r0, #1
	bl	dprint_vfmw
	mov	r4, #1
.L499:
	ldr	r3, [r9, #856]
	cmp	r3, #0
	bne	.L522
	ldr	r7, [r9, #840]
	cmp	r7, #0
	strle	r3, [fp, #-64]
	ble	.L502
	ldr	r1, [fp, #-100]
	mov	r6, r3
	ldr	r2, [fp, #-92]
	str	r3, [fp, #-64]
	str	r3, [fp, #-68]
	mul	r2, r2, r1
	str	r9, [fp, #-56]
	sub	r1, r4, #1
	str	r1, [fp, #-80]
	sub	r3, r2, #1
	str	r3, [fp, #-88]
	str	r2, [fp, #-84]
	b	.L503
.L505:
	ldr	r3, [fp, #-56]
	add	r6, r6, #1
	ldr	r7, [r3, #840]
	cmp	r7, r6
	ble	.L538
	cmp	r6, #199
	bgt	.L539
.L503:
	ldr	r3, [fp, #-56]
	mov	r1, r4
	add	r5, r3, r6, lsl #2
	ldrsh	r9, [r5, #42]
	ldrh	r3, [r5, #40]
	sub	r0, r9, #1
	add	r0, r0, r4
	str	r3, [fp, #-60]
	bl	__aeabi_uidiv
	cmp	r6, #0
	mul	r10, r4, r0
	ble	.L504
	ldrsh	r0, [r5, #38]
	mov	r1, r4
	sub	r0, r0, #1
	add	r0, r0, r4
	bl	__aeabi_uidiv
	mul	r3, r4, r0
	cmp	r10, r3
	str	r3, [fp, #-68]
	ldrgt	r10, [fp, #-80]
	bgt	.L506
	b	.L505
.L509:
	bl	__aeabi_uidiv
	mov	r1, r4
	mul	r9, r4, r0
	ldrsh	r0, [r5, #44]
	bl	__aeabi_uidiv
	mul	r0, r4, r0
	cmp	r9, r0
	add	r2, r0, #1
	bhi	.L510
	cmp	r9, r2
	mov	r1, r4
	beq	.L510
	ldrsh	r0, [r5, #38]
	add	r0, r0, r10
	bl	__aeabi_uidiv
	mul	r0, r4, r0
	cmp	r9, r0
	bhi	.L540
.L510:
	cmp	r8, #198
	mov	r6, r8
	add	r5, r5, #4
	bgt	.L541
	ldrsh	r9, [r5, #42]
.L506:
	add	r8, r6, #1
	mov	r1, r4
	cmp	r8, r7
	add	r0, r10, r9
	blt	.L509
	ldr	r3, [fp, #-80]
	add	r0, r3, r9
	bl	__aeabi_uidiv
	mul	r10, r4, r0
.L508:
	ldrsh	r0, [fp, #-60]
	mov	r1, r4
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-84]
	ldr	r7, [fp, #-88]
	cmp	r3, r10
	ldr	r3, [fp, #-64]
	movhi	r7, r10
	add	r3, r3, #1
	str	r3, [fp, #-64]
	mul	r5, r4, r0
	cmp	r7, r5
	movlt	r5, #0
	cmp	r3, #252
	bgt	.L542
	ldr	r3, [fp, #-72]
	cmp	r3, #0
	bne	.L505
	ldr	r8, [fp, #-92]
	mov	r0, r5
	mov	r1, r8
	bl	__aeabi_uidivmod
	ldr	r2, [fp, #-72]
	mov	r0, r5
	mov	ip, r2, lsr #16
	mov	r3, r2	@ movhi
	mov	r5, r2	@ movhi
	str	ip, [fp, #-60]
	mov	r10, ip
	bfi	r3, r1, #0, #9
	mov	r1, r8
	strh	r3, [fp, #-48]	@ movhi
	bl	__aeabi_uidiv
	ldr	ip, [fp, #-60]
	mov	r1, r8
	ldr	r3, [fp, #-64]
	ldr	r9, [fp, #-76]
	add	r3, r3, #5
	mov	r2, r3, asl #3
	str	r2, [fp, #-60]
	bfi	ip, r0, #0, #9
	strh	ip, [fp, #-46]	@ movhi
	ldr	ip, [fp, #-48]
	mov	r0, r7
	str	ip, [r9, r3, asl #3]
	bl	__aeabi_uidivmod
	mov	r0, r7
	bfi	r5, r1, #0, #9
	mov	r1, r8
	strh	r5, [fp, #-48]	@ movhi
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-60]
	add	r2, r2, #4
	bfi	r10, r0, #0, #9
	strh	r10, [fp, #-46]	@ movhi
	ldr	r3, [fp, #-48]
	str	r3, [r9, r2]
	b	.L505
.L490:
	ldr	ip, [r9]
	cmp	ip, #17
	beq	.L493
	cmp	ip, #16
	mov	r2, #0
	mov	r0, #1
	str	r2, [r9, #848]
	str	r0, [r9, #872]
	bne	.L517
.L494:
	mov	r0, #10
	str	r0, [fp, #-96]
	mla	r3, r1, r3, r2
	b	.L518
.L504:
	ldr	r3, [fp, #-68]
	cmp	r10, r3
	movgt	r3, #0
	movle	r3, #1
	cmp	r6, #0
	moveq	r3, #0
	cmp	r3, #0
	bne	.L505
	b	.L508
.L541:
	ldr	r3, [fp, #-56]
	mov	r1, r4
	add	r2, r3, r8, lsl #2
	ldrsh	r0, [r2, #42]
	sub	r0, r0, #1
	add	r0, r0, r4
	bl	__aeabi_uidiv
	mul	r10, r4, r0
	b	.L508
.L522:
	mov	r2, #0
	str	r2, [fp, #-64]
.L500:
	cmp	r3, #1
	beq	.L514
.L502:
	ldr	r3, [fp, #-64]
	sub	r3, r3, #1
	uxth	r3, r3
.L516:
	ldr	ip, [r9, #864]
	mov	r0, #0
	ldr	lr, [r9, #848]
	ldrb	r1, [fp, #-45]	@ zero_extendqisi2
	sub	ip, ip, #4
	ldrb	r2, [fp, #-46]	@ zero_extendqisi2
	bfi	r1, ip, #0, #2
	ldr	ip, [r9, #852]
	bfi	r2, lr, #0, #1
	ldr	lr, [fp, #-96]
	and	ip, ip, #3
	strh	r3, [fp, #-48]	@ movhi
	bfi	r1, lr, #2, #4
	bfi	r2, ip, #4, #2
	strb	r1, [fp, #-45]
	ldr	lr, [r9, #872]
	uxtb	r1, r1
	mov	r3, r2
	bfi	r3, ip, #6, #2
	bfi	r1, lr, #6, #1
	strb	r3, [fp, #-46]
	strb	r1, [fp, #-45]
	ldr	r3, [fp, #-48]
	ldr	r2, [fp, #-76]
	str	r3, [r2, #28]
.L534:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L542:
	ldr	r9, [fp, #-56]
	mov	r3, #1
	str	r3, [r9, #856]
.L514:
	ldr	r3, [fp, #-72]
	cmp	r3, #0
	mov	r3, #1
	str	r3, [r9, #840]
	movne	r3, #0
	bne	.L516
	ldr	lr, [fp, #-72]
	ldr	r3, [fp, #-92]
	ldr	r2, [fp, #-100]
	mov	r0, lr, lsr #16
	strh	lr, [fp, #-48]	@ movhi
	sub	r3, r3, #1
	sub	r2, r2, #1
	bfi	r0, lr, #0, #9
	mov	r1, lr, lsr #16
	strh	r0, [fp, #-46]	@ movhi
	bfi	r1, r2, #0, #9
	ldr	ip, [fp, #-48]
	mov	r0, lr	@ movhi
	strh	r1, [fp, #-46]	@ movhi
	bfi	r0, r3, #0, #9
	strh	r0, [fp, #-48]	@ movhi
	mov	r3, lr
	ldr	r1, [fp, #-76]
	ldr	r2, [fp, #-48]
	str	ip, [r1, #48]
	str	r2, [r1, #52]
	b	.L516
.L540:
	mov	r10, r9
	b	.L508
.L493:
	ldr	r0, [r9, #848]
	cmp	r0, #1
	movne	r2, #0
	bne	.L517
.L491:
	add	r0, r2, #2032
	adds	lr, r3, #63
	add	r0, r0, #15
	addmi	lr, r3, #126
	add	r2, r2, #4080
	cmp	r0, #0
	add	r2, r2, #14
	movge	r2, r0
	mov	r0, lr, asr #6
	mov	r2, r2, asr #11
	mov	r0, r0, asl #5
	mov	r2, r2, asl #4
	mul	r2, r2, r0
	b	.L492
.L538:
	mov	r9, r3
	ldr	r3, [r3, #856]
	b	.L500
.L539:
	str	r6, [sp]
	movw	r3, #2550
	ldr	r2, .L543+4
	mov	r0, #0
	ldr	r1, .L543+8
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L537:
	ldr	r1, .L543+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L534
.L536:
	ldr	r3, .L543+16
	ldr	r2, .L543+4
	ldr	r1, .L543+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L534
.L544:
	.align	2
.L543:
	.word	.LC55
	.word	.LANCHOR0+468
	.word	.LC56
	.word	.LC54
	.word	.LC53
	.word	.LC1
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_CfgRpMsg, .-VDMHAL_V400R003_CfgRpMsg
	.align	2
	.global	VDMHAL_V400R003_CfgRpReg
	.type	VDMHAL_V400R003_CfgRpReg, %function
VDMHAL_V400R003_CfgRpReg:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 16
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
	cmp	r3, #0
	mov	r0, #0
	str	r0, [fp, #-24]
	bne	.L580
	ldr	r3, [r1]
	cmp	r3, #0
	beq	.L581
.L549:
	ldr	r4, .L583
	ldr	r3, [r1, #48]
	ldrb	r2, [r4]	@ zero_extendqisi2
	bic	r3, r3, #15
	str	r3, [fp, #-24]
	cmp	r2, #1
	bne	.L582
	ldr	r2, .L583+4
	ldr	r1, [r2]
	str	r3, [r1, #16]
.L552:
	ldr	r1, [r2]
	movw	r3, #53763
	movt	r3, 8192
	str	r3, [r1, #12]
.L554:
	ldr	r1, [r2]
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r1, #60]
.L556:
	ldr	r1, [r2]
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r1, #64]
.L558:
	ldr	r1, [r2]
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r1, #68]
.L560:
	ldr	r1, [r2]
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r1, #72]
.L562:
	ldr	r1, [r2]
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r1, #76]
.L564:
	ldr	r1, [r2]
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r1, #80]
.L566:
	ldr	r2, [r2]
	movw	r3, #3075
	mov	r0, #0
	movt	r3, 48
	str	r3, [r2, #84]
.L569:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L582:
	mov	r2, #16
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r2, [r4]	@ zero_extendqisi2
	movw	r3, #53763
	cmp	r2, #1
	movt	r3, 8192
	str	r3, [fp, #-24]
	ldreq	r2, .L583+4
	beq	.L552
	mov	r2, #12
	mov	r1, #2
	ldr	r0, [fp, #4]
	movw	r5, #3075
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	movt	r5, 48
	str	r5, [fp, #-24]
	cmp	r3, #1
	ldreq	r2, .L583+4
	beq	.L554
	mov	r3, r5
	mov	r2, #60
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r2, .L583+4
	beq	.L556
	mov	r3, r5
	mov	r2, #64
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r2, .L583+4
	beq	.L558
	mov	r3, r5
	mov	r2, #68
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r2, .L583+4
	beq	.L560
	mov	r3, r5
	mov	r2, #72
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r2, .L583+4
	beq	.L562
	mov	r3, r5
	mov	r2, #76
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r2, .L583+4
	beq	.L564
	mov	r3, r5
	mov	r2, #80
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r2, .L583+4
	beq	.L566
	ldr	r0, [fp, #4]
	mov	r3, r5
	mov	r2, #84
	mov	r1, #2
	bl	VDH_Record_RegData
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L581:
	mov	r0, #0
	str	r1, [fp, #-32]
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L550
	ldr	r1, [fp, #-32]
	str	r3, [r1]
	b	.L549
.L580:
	ldr	r1, .L583+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L569
.L550:
	ldr	r1, .L583+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L569
.L584:
	.align	2
.L583:
	.word	g_HalDisable
	.word	g_HwMem
	.word	.LC57
	.word	.LC36
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_CfgRpReg, .-VDMHAL_V400R003_CfgRpReg
	.align	2
	.global	VDMHAL_V400R003_MakeDecReport
	.type	VDMHAL_V400R003_MakeDecReport, %function
VDMHAL_V400R003_MakeDecReport:
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
	UNWIND(.pad #20)
	sub	sp, sp, #20
	subs	r3, r0, #0
	beq	.L614
	ldmia	r3, {r7, r8}
	ldr	r5, [r3, #8]
	cmp	r8, #0
	beq	.L615
	movw	r3, #1228
	ldr	r6, .L621
	mul	r3, r3, r5
	ldr	r0, [r6, r3]
	cmp	r0, #0
	beq	.L589
	ldr	r3, .L621+4
	ldr	r2, [r3]
	cmp	r2, #1
	addne	r1, r3, #60
	beq	.L604
.L592:
	ldr	r2, [r3, #4]!
	cmp	r2, #1
	beq	.L604
	cmp	r3, r1
	bne	.L592
	mov	r4, #0
.L590:
	ldr	r3, .L621+8
	mov	r2, #816
	mov	r1, #0
	mov	r0, r8
	ldr	r3, [r3, #48]
	blx	r3
	movw	r3, #1228
	mla	r3, r3, r5, r6
	ldr	r1, .L621+12
	mov	r2, #180
	mla	r2, r2, r5, r1
	ldr	r3, [r3, #24]
	str	r3, [r8, #4]
	ldr	r3, [r2, #4]
	mov	r3, r3, lsr #17
	and	r2, r3, #3
	cmp	r2, #1
	moveq	r3, #0
	beq	.L593
	eor	r3, r3, #1
	and	r3, r3, #1
	cmp	r7, #3
	orrne	r3, r3, #1
.L593:
	mov	r2, #180
	str	r3, [r8]
	mla	r3, r2, r5, r1
	cmp	r7, #0
	cmpne	r7, #15
	ldr	r2, [r3, #4]
	ubfx	r2, r2, #0, #17
	str	r2, [r8, #8]
	beq	.L616
.L594:
	cmp	r2, #200
	bhi	.L617
.L596:
	movw	r3, #1228
	mla	r6, r3, r5, r6
	ldr	r7, [r6, #40]
	mov	r0, r7
	bl	MEM_Phy2Vir
	subs	r1, r0, #0
	beq	.L618
	ldr	r6, .L621+16
	mov	r3, #3200
	ldr	r2, [r8, #8]
	mla	r4, r3, r5, r6
	mov	r2, r2, asl #2
	mov	r0, r4
	bl	VDMHAL_V400R003_ReadMsgSlot
	ldr	ip, [r8, #8]
	cmp	ip, #0
	movne	r3, r4
	movne	r1, r8
	movne	r2, #0
	beq	.L601
.L600:
	ldr	r0, [r3, #4]
	add	r2, r2, #1
	cmp	r2, ip
	add	r3, r3, #16
	add	r1, r1, #4
	strh	r0, [r1, #8]	@ movhi
	ldr	r0, [r3, #-8]
	strh	r0, [r1, #10]	@ movhi
	bne	.L600
.L601:
	mov	r0, #6
	bl	IsDprintTypeEnable
	cmp	r0, #0
	bne	.L619
.L587:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L616:
	cmp	r4, #1
	beq	.L620
	ldr	r1, .L621+20
	ldrb	r1, [r1]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L594
	ldrb	r3, [r3, #19]	@ zero_extendqisi2
	and	r3, r3, #3
	cmp	r3, #2
	bne	.L594
	mov	r3, #1
	strh	r1, [r8, #12]	@ movhi
	mov	r0, r1
	strh	r1, [r8, #14]	@ movhi
	str	r3, [r8, #8]
	b	.L587
.L604:
	mov	r4, r2
	b	.L590
.L619:
	ldr	r2, [r8, #8]
	mov	r0, #6
	ldr	r1, .L621+24
	mov	r4, #0
	bl	dprint_vfmw
	mov	r2, r7
	ldr	r1, .L621+28
	mov	r0, #6
	bl	dprint_vfmw
	mov	r3, #3200
	mla	r5, r3, r5, r6
.L602:
	ldr	r1, [r5, #4]
	ldr	lr, [r5, #12]
	mov	r2, r4
	ldr	ip, [r5, #8]
	mov	r0, #6
	ldr	r3, [r5]
	add	r4, r4, #4
	str	r1, [sp]
	add	r5, r5, #16
	str	lr, [sp, #8]
	str	ip, [sp, #4]
	ldr	r1, .L621+32
	bl	dprint_vfmw
	ldr	r3, [r8, #8]
	mov	r3, r3, asl #2
	sub	r3, r3, #3
	cmp	r3, r4
	bhi	.L602
	ldr	r1, .L621+36
	mov	r0, #6
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L617:
	mov	r3, #200
	ldr	r1, .L621+40
	mov	r0, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r8, #8]
	b	.L596
.L620:
	ldr	r3, [r3, #16]
	ubfx	r3, r3, #21, #2
	cmp	r3, #2
	bne	.L594
	mov	r3, #0
	str	r4, [r8, #8]
	strh	r3, [r8, #12]	@ movhi
	mov	r0, r3
	strh	r3, [r8, #14]	@ movhi
	b	.L587
.L618:
	ldr	r3, .L621+44
	ldr	r2, .L621+48
	ldr	r1, .L621+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L587
.L589:
	ldr	r3, .L621+56
	ldr	r2, .L621+48
	ldr	r1, .L621+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L587
.L615:
	mov	r0, r8
	ldr	r3, .L621+60
	ldr	r2, .L621+48
	ldr	r1, .L621+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L587
.L614:
	ldr	r3, .L621+64
	ldr	r2, .L621+48
	ldr	r1, .L621+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L587
.L622:
	.align	2
.L621:
	.word	g_HwMem
	.word	g_DSPState
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_BackUp
	.word	g_UpMsg
	.word	g_not_allow_H264FullPictureRepair_flag
	.word	.LC61
	.word	.LC62
	.word	.LC63
	.word	.LC64
	.word	.LC60
	.word	.LC49
	.word	.LANCHOR0+496
	.word	.LC1
	.word	.LC45
	.word	.LC59
	.word	.LC58
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_MakeDecReport, .-VDMHAL_V400R003_MakeDecReport
	.align	2
	.global	VDMHAL_V400R003_PrepareRepair
	.type	VDMHAL_V400R003_PrepareRepair, %function
VDMHAL_V400R003_PrepareRepair:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	ldr	r4, .L638
	cmp	r3, #0
	movw	r5, #1228
	mov	r6, r0
	mla	r5, r5, r3, r4
	bne	.L635
	ldr	r3, [r4]
	cmp	r3, #0
	beq	.L636
.L627:
	cmp	r2, #0
	bne	.L629
	ldr	r3, .L638+4
	ldr	r0, [r3, #840]
	cmp	r0, #0
	ble	.L630
	cmp	r6, #6
	bne	.L631
	ldrb	r2, [r1, #17]	@ zero_extendqisi2
	cmp	r2, #1
	movne	r2, #0
	str	r2, [r3, #1728]
.L631:
	ldr	r3, [fp, #4]
	mov	r2, r1
	mov	r0, r6
	mov	r1, r5
	str	r3, [sp]
	mov	r3, #0
	bl	VDMHAL_V400R003_CfgRpReg
	mov	r1, r5
	mov	r2, #0
	ldr	r0, .L638+4
	bl	VDMHAL_V400R003_CfgRpMsg
	mov	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L629:
	cmp	r2, #1
	movne	r0, #0
	beq	.L637
.L626:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L637:
	cmp	r6, #6
	bne	.L633
	ldrb	r3, [r1, #17]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L633
	ldr	r4, .L638+4
	ldr	r3, [r4, #1716]
	cmp	r3, #0
	ble	.L633
	ldr	r3, [fp, #4]
	mov	ip, #2
	mov	r2, r1
	mov	r0, r6
	mov	r1, r5
	str	r3, [sp]
	mov	r3, #0
	str	ip, [r4, #1728]
	bl	VDMHAL_V400R003_CfgRpReg
	mov	r1, r5
	add	r0, r4, #876
	mov	r2, #0
	bl	VDMHAL_V400R003_CfgRpMsg
	mov	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L636:
	mov	r0, #0
	str	r2, [fp, #-36]
	movt	r0, 63683
	str	r1, [fp, #-32]
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L628
	str	r3, [r4]
	ldr	r1, [fp, #-32]
	ldr	r2, [fp, #-36]
	b	.L627
.L633:
	ldr	r1, .L638+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L626
.L635:
	ldr	r1, .L638+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L626
.L630:
	mov	r0, r2
	ldr	r1, .L638+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L626
.L628:
	ldr	r1, .L638+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L626
.L639:
	.align	2
.L638:
	.word	g_HwMem
	.word	g_RepairParam
	.word	.LC68
	.word	.LC65
	.word	.LC67
	.word	.LC66
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_PrepareRepair, .-VDMHAL_V400R003_PrepareRepair
	.align	2
	.global	VDMHAL_V400R003_StartHwRepair
	.type	VDMHAL_V400R003_StartHwRepair, %function
VDMHAL_V400R003_StartHwRepair:
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
	ldr	r6, .L670
	mov	r5, r0
	mov	r4, r1
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L664
	cmp	r0, #0
	ble	.L665
	str	r3, [sp]
	mov	r3, r0
	ldr	r2, .L670+4
	mov	r0, #32
	ldr	r1, .L670+8
	bl	dprint_vfmw
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L666
.L648:
	mov	r2, #1
	mov	r3, r5
	str	r2, [sp]
	mov	r0, #32
	ldr	r2, .L670+4
	ldr	r1, .L670+8
	bl	dprint_vfmw
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L660
.L643:
	cmp	r4, #0
	beq	.L667
	mov	r3, r5, asl #6
	ldr	r2, .L670+12
	sub	r3, r3, r5, asl #3
	mov	r1, #1
	add	r3, r2, r3
	str	r1, [r4, #4]
	mov	r0, r4
	ldr	r3, [r3, #8]
	strb	r1, [r4, #2]
	strb	r5, [r4]
	str	r3, [r4, #8]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	VDH_PostTask
.L665:
	movw	r3, #1228
	ldr	r4, .L670+16
	mul	r3, r3, r0
	mov	r2, #67108864
	ldr	r3, [r4, r3]
	str	r2, [r3, #8]
.L646:
	movw	r3, #1228
	ldr	r2, .L670+20
	mul	r3, r3, r5
	mvn	r1, #1
	ldr	r3, [r4, r3]
	str	r1, [r3, #36]
	ldr	r3, [r2, #108]
	blx	r3
.L655:
	movw	r3, #1228
	mov	r2, #0
	mul	r5, r3, r5
	mov	r1, #1
	ldr	r3, [r4, r5]
	str	r2, [r3]
	ldr	r3, [r4, r5]
	str	r1, [r3]
	ldr	r3, [r4, r5]
	str	r2, [r3]
.L640:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L664:
	mov	r3, #67108864
	mov	r2, #8
	mov	r1, #2
	mov	r0, r4
	bl	VDH_Record_RegData
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L668
	cmp	r5, #0
	ldrle	r4, .L670+16
	bgt	.L648
	b	.L646
.L668:
	mvn	r3, #1
	mov	r2, #36
	mov	r1, #2
	mov	r0, r4
	bl	VDH_Record_RegData
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L643
	ldr	r3, .L670+20
	ldr	r3, [r3, #108]
	blx	r3
	cmp	r5, #0
	ble	.L669
.L652:
	mov	r4, #1
	mov	r3, r5
	str	r4, [sp]
	mov	r0, #32
	ldr	r2, .L670+24
	ldr	r1, .L670+8
	bl	dprint_vfmw
	mov	r3, r5
	str	r4, [sp]
	mov	r0, #32
	ldr	r2, .L670+24
	ldr	r1, .L670+8
	bl	dprint_vfmw
	str	r4, [sp]
	mov	r3, r5
	ldr	r2, .L670+24
	ldr	r1, .L670+8
	mov	r0, #32
	bl	dprint_vfmw
	b	.L640
.L666:
	mvn	r3, #1
	mov	r2, #36
	mov	r1, #2
	mov	r0, r4
	bl	VDH_Record_RegData
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L643
.L660:
	ldr	r3, .L670+20
	ldr	r3, [r3, #108]
	blx	r3
	b	.L652
.L667:
	mov	r3, r4
	mov	r0, r4
	ldr	r2, .L670+24
	ldr	r1, .L670+28
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	dprint_vfmw
.L669:
	ldr	r4, .L670+16
	b	.L655
.L671:
	.align	2
.L670:
	.word	g_HalDisable
	.word	.LANCHOR0+528
	.word	.LC30
	.word	g_VdmDrvParam
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+544
	.word	.LC69
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_StartHwRepair, .-VDMHAL_V400R003_StartHwRepair
	.align	2
	.global	VDMHAL_V400R003_CalVdhClkSkip
	.type	VDMHAL_V400R003_CalVdhClkSkip, %function
VDMHAL_V400R003_CalVdhClkSkip:
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
	ldr	r9, .L697
	ldr	r4, [r9, #64]
	cmp	r4, #0
	blt	.L693
	ldr	r10, .L697+4
	add	r6, r9, #64
	add	r9, r9, #124
	mov	r7, #30
	mov	r8, #0
.L674:
	mov	r0, r4
	bl	VCTRL_IsChanActive
	cmp	r0, #0
	bne	.L696
	ldr	r2, .L697+8
	mov	r0, r4
	ldr	r3, [r10, r4, asl #2]
	ldr	r2, [r2, r4, asl #2]
	ldr	r1, [r3, #28]
	ldr	r3, [r3, #32]
	adds	r4, r1, #15
	ldr	r2, [r2, #1512]
	addmi	r4, r1, #30
	adds	r5, r3, #15
	addmi	r5, r3, #30
	cmp	r2, #30
	mov	r4, r4, asr #4
	movgt	r7, r2
	mov	r5, r5, asr #4
	bl	VCTRL_GetVidStd
	cmp	r0, #17
	ldrls	pc, [pc, r0, asl #2]
	b	.L676
.L679:
	.word	.L688
	.word	.L688
	.word	.L688
	.word	.L688
	.word	.L676
	.word	.L688
	.word	.L688
	.word	.L676
	.word	.L688
	.word	.L688
	.word	.L688
	.word	.L688
	.word	.L688
	.word	.L688
	.word	.L676
	.word	.L689
	.word	.L688
	.word	.L688
.L688:
	mul	r4, r5, r4
	mla	r8, r7, r4, r8
.L676:
	cmp	r6, r9
	beq	.L695
	ldr	r4, [r6, #4]!
	cmp	r4, #0
	bge	.L674
.L695:
	mov	r3, r8, asl #8
	movw	r2, #23813
	sub	r8, r3, r8, asl #6
	movt	r2, 56143
	umull	r8, r9, r8, r2
	mov	r0, r9, lsr #21
	rsb	r0, r0, #32
	cmp	r0, #25
	movge	r0, #25
	bic	r0, r0, r0, asr #31
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L696:
	mov	r3, r4
	ldr	r2, .L697+12
	ldr	r1, .L697+16
	mov	r0, #1
	bl	dprint_vfmw
	b	.L676
.L689:
	mul	r4, r5, r4
	mul	r4, r7, r4
	add	r8, r8, r4, lsl #1
	b	.L676
.L693:
	mov	r0, #25
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L698:
	.align	2
.L697:
	.word	g_ChanCtx
	.word	s_pFspInst
	.word	s_pstVfmwChan
	.word	.LANCHOR0+576
	.word	.LC70
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_CalVdhClkSkip, .-VDMHAL_V400R003_CalVdhClkSkip
	.align	2
	.global	VDMHAL_V400R003_GetVdmClk
	.type	VDMHAL_V400R003_GetVdmClk, %function
VDMHAL_V400R003_GetVdmClk:
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
	mov	r3, r0, asl #6
	ldr	r2, .L707
	sub	r0, r3, r0, asl #3
	cmp	r1, #3
	ldrls	pc, [pc, r1, asl #2]
	b	.L700
.L702:
	.word	.L700
	.word	.L703
	.word	.L704
	.word	.L705
.L700:
	add	r3, r2, r0
	mov	r1, #500
	mov	r0, r1
	str	r1, [r3, #16]
	ldmfd	sp, {fp, sp, pc}
.L704:
	add	r3, r2, r0
	mov	r1, #100
	mov	r0, r1
	str	r1, [r3, #16]
	ldmfd	sp, {fp, sp, pc}
.L703:
	add	r3, r2, r0
	mov	r1, #540
	mov	r0, r1
	str	r1, [r3, #16]
	ldmfd	sp, {fp, sp, pc}
.L705:
	add	r3, r2, r0
	mov	r1, #600
	mov	r0, r1
	str	r1, [r3, #16]
	ldmfd	sp, {fp, sp, pc}
.L708:
	.align	2
.L707:
	.word	g_VdmDrvParam
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_GetVdmClk, .-VDMHAL_V400R003_GetVdmClk
	.align	2
	.global	VDMHAL_V400R003_StartHwDecode
	.type	VDMHAL_V400R003_StartHwDecode, %function
VDMHAL_V400R003_StartHwDecode:
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
	subs	r5, r0, #0
	mov	r4, r1
	ble	.L710
	mov	r0, #0
	mov	r3, r5
	str	r0, [sp]
	ldr	r2, .L748
	ldr	r1, .L748+4
	bl	dprint_vfmw
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L710:
	ldr	r6, .L748+8
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L745
	ldr	r2, .L748+12
	ldr	r8, .L748+16
	ldr	r10, .L748+20
	ldr	r1, [r2]
	ldr	r3, [r1, #120]
	tst	r3, #768
	bfcne	r3, #8, #2
	strne	r3, [r1, #120]
	mov	r3, r5, asl #6
	mov	r1, #500
	sub	r3, r3, r5, asl #3
	add	r3, r8, r3
	ldr	r7, [r3, #8]
	str	r1, [r3, #16]
	ldr	r3, [r10, r7, asl #2]
	ldr	r9, [r3, #1224]
.L714:
	ldr	r2, [r2]
	ldr	r3, [r2, #120]
	bfc	r3, #12, #5
	bfc	r3, #17, #1
	str	r3, [r2, #120]
	ldr	r3, [r2, #120]
	orr	r3, r3, #131072
	str	r3, [r2, #120]
.L716:
	ldr	r1, [r10, r7, asl #2]
	mov	r2, r5
	mov	r3, r4
	mov	r0, #12
	ldr	r10, .L748+8
	ldr	r1, [r1, #600]
	cmp	r1, #16
	movne	r1, #3
	moveq	r1, #0
	bl	SCD_ConfigReg
	ldrb	r2, [r6]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L746
	movw	r2, #1228
	ldr	r3, .L748+24
	mul	r2, r2, r5
	ldr	r4, .L748+28
	ldr	r1, [r3]
	movw	r3, #43690
	bfi	r3, r3, #16, #16
	cmp	r1, #1
	ldr	r2, [r4, r2]
	moveq	r1, #15
	movne	r1, #0
	str	r3, [r2, #156]
.L719:
	movw	r2, #1228
	cmp	r9, #1
	mul	r2, r2, r5
	mvneq	r3, #5
	mvnne	r3, #1
	ldr	r2, [r4, r2]
	add	r2, r2, #61440
	str	r1, [r2, #32]
.L721:
	movw	r2, #1228
	mul	r2, r2, r5
	ldr	r2, [r4, r2]
	str	r3, [r2, #36]
.L723:
	ldr	r6, .L748+32
	mov	r0, #30
	ldr	r3, [r6, #112]
	blx	r3
	ldr	r3, [r6, #108]
	blx	r3
	movw	r3, #1228
	mul	r3, r3, r5
	mov	r1, #56
	mov	r2, #0
	mov	lr, #1
	mla	r5, r1, r5, r8
	ldr	ip, .L748+36
	mov	r1, #4
	ldr	r0, [r4, r3]
	str	r2, [r0]
	ldr	r0, [r4, r3]
	str	lr, [r0]
	ldr	r3, [r4, r3]
	str	r2, [r3]
	ldr	r0, [r5, #8]
	ldr	r2, [ip, r0, asl #2]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	VDEC_Lowdelay_Event_Time
.L746:
	movw	r3, #43690
	mov	r1, #2
	mov	r2, #156
	movt	r3, 43690
	mov	r0, r4
	bl	VDH_Record_RegData
	ldr	r3, .L748+24
	ldrb	r2, [r10]	@ zero_extendqisi2
	ldr	r3, [r3]
	cmp	r3, #1
	moveq	r1, #15
	movne	r1, #0
	cmp	r2, #1
	ldreq	r4, .L748+28
	beq	.L719
	mov	r3, r1
	movw	r2, #61472
	mov	r0, r4
	mov	r1, #2
	bl	VDH_Record_RegData
	cmp	r9, #1
	ldrb	r2, [r6]	@ zero_extendqisi2
	mvneq	r3, #5
	mvnne	r3, #1
	cmp	r2, #1
	ldreq	r4, .L748+28
	beq	.L721
	mov	r0, r4
	mov	r2, #36
	mov	r1, #2
	bl	VDH_Record_RegData
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r4, .L748+28
	beq	.L723
	cmp	r4, #0
	beq	.L747
	mov	r3, #1
	str	r7, [r4, #8]
	strb	r5, [r4]
	mov	r0, r4
	str	r3, [r4, #4]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	VDH_PostTask
.L745:
	mov	r3, #0
	mov	r0, r1
	mov	r2, r3
	mov	r1, r3
	bl	VDH_Record_RegData
	ldr	r8, .L748+16
	mov	r3, r5, asl #6
	ldr	r10, .L748+20
	sub	r3, r3, r5, asl #3
	ldrb	r1, [r6]	@ zero_extendqisi2
	add	r3, r8, r3
	mov	r2, #500
	cmp	r1, #1
	ldr	r7, [r3, #8]
	str	r2, [r3, #16]
	ldreq	r2, .L748+12
	ldr	r3, [r10, r7, asl #2]
	ldr	r9, [r3, #1224]
	beq	.L714
	mov	r3, #0
	mov	r2, #1
	mov	r1, r3
	mov	r0, r4
	bl	VDH_Record_RegData
	b	.L716
.L747:
	mov	r3, r4
	mov	r0, r4
	ldr	r2, .L748
	ldr	r1, .L748+40
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L749:
	.align	2
.L748:
	.word	.LANCHOR0+608
	.word	.LC25
	.word	g_HalDisable
	.word	g_pstRegCrg
	.word	g_VdmDrvParam
	.word	s_pstVfmwChan
	.word	mask_mmu_err_int
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_LowDelaySeqIndex
	.word	.LC69
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_StartHwDecode, .-VDMHAL_V400R003_StartHwDecode
	.align	2
	.global	VDMHAL_V400R003_GetCharacter
	.type	VDMHAL_V400R003_GetCharacter, %function
VDMHAL_V400R003_GetCharacter:
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
	ldr	r1, .L751
	mov	r0, #15
	ldr	r3, .L751+4
	mov	r2, #4
	str	r0, [r1]
	str	r2, [r3]
	ldmfd	sp, {fp, sp, pc}
.L752:
	.align	2
.L751:
	.word	g_VdmCharacter
	.word	g_eVdmVersion
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_GetCharacter, .-VDMHAL_V400R003_GetCharacter
	.align	2
	.global	VDMHAL_V400R003_WriteBigTitle1DYuv
	.type	VDMHAL_V400R003_WriteBigTitle1DYuv, %function
VDMHAL_V400R003_WriteBigTitle1DYuv:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 88
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #100)
	sub	sp, sp, #100
	mov	r9, r3
	ubfx	r3, r3, #29, #2
	mov	r4, #0
	cmp	r3, #1
	str	r0, [fp, #-108]
	mov	r7, r1
	mov	r6, r2
	str	r4, [fp, #-92]
	str	r4, [fp, #-88]
	str	r4, [fp, #-84]
	str	r4, [fp, #-80]
	str	r4, [fp, #-76]
	str	r4, [fp, #-72]
	str	r4, [fp, #-68]
	str	r4, [fp, #-64]
	str	r4, [fp, #-60]
	str	r4, [fp, #-56]
	str	r4, [fp, #-52]
	str	r4, [fp, #-48]
	beq	.L754
	cmp	r3, #2
	moveq	r4, #1
	movne	r4, #2
.L754:
	ldr	r3, [fp, #-108]
	cmp	r3, #0
	beq	.L753
	mov	r3, #0
	mov	r2, #4194304
	mov	r1, r3
	str	r3, [sp]
	ldr	r0, .L844
	sub	r3, fp, #92
	bl	MEM_AllocMemBlock
	subs	r1, r0, #0
	beq	.L756
.L758:
	ldr	r1, .L844+4
	mov	r0, #1
	bl	dprint_vfmw
.L753:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L756:
	str	r1, [sp]
	mov	r2, #4194304
	sub	r3, fp, #68
	ldr	r0, .L844+8
	bl	MEM_AllocMemBlock
	cmp	r0, #0
	bne	.L758
	add	r10, r9, #15
	add	r5, r6, #15
	bic	r3, r10, #15
	str	r3, [fp, #-100]
	bic	r5, r5, #15
	mov	ip, r9, lsr #1
	mov	r1, r3
	adds	r3, r3, #31
	addmi	r3, r1, #62
	add	r2, r5, #255
	bic	r2, r2, #255
	ldr	r1, [fp, #-64]
	mov	r3, r3, asr #5
	cmp	r4, #0
	cmpne	r4, #3
	str	ip, [fp, #-116]
	mov	ip, r6, lsr #1
	mla	r3, r3, r2, r7
	str	ip, [fp, #-120]
	str	r1, [fp, #-112]
	mov	ip, r2, asl #4
	ldr	r6, [fp, #-88]
	str	ip, [fp, #-104]
	str	r3, [fp, #-128]
	add	r3, r1, #2097152
	str	r3, [fp, #-124]
	bne	.L759
	ldr	r3, [fp, #-100]
	cmp	r3, #0
	movne	r8, r0
	ldrne	r10, .L844+12
	strne	r8, [fp, #-96]
	beq	.L841
.L760:
	cmp	r5, #0
	beq	.L764
	ldr	r2, [fp, #-96]
	mov	r4, #0
	and	r9, r2, #15
	mov	r3, r2, lsr #4
	ldr	r2, [fp, #-104]
	mul	r3, r2, r3
	add	r9, r3, r9, lsl #8
.L762:
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
	bhi	.L762
.L764:
	ldr	r3, [fp, #-96]
	add	r8, r8, r5
	ldr	r2, [fp, #-100]
	add	r3, r3, #1
	str	r3, [fp, #-96]
	cmp	r2, r3
	bne	.L760
	ldr	r4, [fp, #-100]
	mov	r0, r6
	ldr	ip, [r10, #44]
	ldr	r2, [fp, #-108]
	mul	r1, r4, r5
	blx	ip
	movs	r3, r4, lsr #1
	str	r3, [fp, #-100]
	beq	.L795
	ldr	r3, [fp, #-104]
	mov	r7, #0
	ldr	r8, [fp, #-128]
	str	r7, [fp, #-96]
	mov	r3, r3, asr #1
	str	r3, [fp, #-104]
.L766:
	cmp	r5, #0
	beq	.L768
	ldr	r2, [fp, #-96]
	mov	r4, #0
	and	r9, r2, #7
	mov	r3, r2, lsr #3
	ldr	r2, [fp, #-104]
	mul	r3, r2, r3
	add	r9, r3, r9, lsl #8
.L767:
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
	bhi	.L767
.L768:
	ldr	r3, [fp, #-96]
	add	r7, r7, r5
	ldr	r2, [fp, #-100]
	add	r3, r3, #1
	str	r3, [fp, #-96]
	cmp	r3, r2
	bne	.L766
.L795:
	ldr	r3, [fp, #4]
	cmp	r3, #1
	beq	.L842
	ldr	r3, [fp, #4]
	cmp	r3, #0
	beq	.L793
.L788:
	ldr	r3, [fp, #-116]
	ldr	r2, [fp, #-120]
	ldr	r5, [fp, #-108]
	ldr	ip, [r10, #44]
	mul	r4, r3, r2
	ldr	r0, [fp, #-112]
	mov	r2, r5
	mov	r1, r4
	blx	ip
	mov	r2, r5
	ldr	ip, [r10, #44]
	mov	r1, r4
	ldr	r0, [fp, #-124]
	blx	ip
	ldr	r1, [fp, #-88]
	ldr	r0, [fp, #-84]
	bl	MEM_ReleaseMemBlock
	ldr	ip, [r10, #48]
	mov	r2, #24
	sub	r0, fp, #92
	mov	r1, #0
	blx	ip
	ldr	r1, [fp, #-64]
	ldr	r0, [fp, #-60]
	bl	MEM_ReleaseMemBlock
	ldr	r3, [r10, #48]
	sub	r0, fp, #68
	mov	r2, #24
	mov	r1, #0
	blx	r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L759:
	subs	r3, r4, #1
	ldr	r2, [fp, #-100]
	ldr	r10, .L844+12
	movne	r3, #1
	cmp	r2, r3
	str	r3, [fp, #-132]
	ldrhi	r3, [fp, #-132]
	strhi	r3, [fp, #-96]
	bls	.L778
.L780:
	cmp	r5, #0
	beq	.L781
	ldr	r2, [fp, #-96]
	mov	r4, #0
	and	r8, r2, #15
	mov	r3, r2, lsr #4
	mov	r9, r2, lsr #1
	ldr	r2, [fp, #-104]
	mul	r9, r5, r9
	mul	r3, r2, r3
	add	r8, r3, r8, lsl #8
.L779:
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
	bhi	.L779
.L781:
	ldr	r3, [fp, #-96]
	ldr	r2, [fp, #-100]
	add	r3, r3, #2
	str	r3, [fp, #-96]
	cmp	r2, r3
	bhi	.L780
.L778:
	ldr	r4, [fp, #-100]
	mov	r0, r6
	ldr	ip, [r10, #44]
	ldr	r2, [fp, #-108]
	mul	r1, r4, r5
	mov	r1, r1, lsr #1
	blx	ip
	movs	r3, r4, lsr #1
	str	r3, [fp, #-100]
	beq	.L782
	ldr	r3, [fp, #-104]
	mov	r7, #0
	ldr	r8, [fp, #-128]
	str	r7, [fp, #-96]
	mov	r3, r3, asr #1
	str	r3, [fp, #-104]
.L783:
	cmp	r5, #0
	beq	.L785
	ldr	r2, [fp, #-96]
	mov	r4, #0
	and	r9, r2, #7
	mov	r3, r2, lsr #3
	ldr	r2, [fp, #-104]
	mul	r3, r2, r3
	add	r9, r3, r9, lsl #8
.L784:
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
	bhi	.L784
.L785:
	ldr	r3, [fp, #-96]
	add	r7, r7, r5
	ldr	r2, [fp, #-100]
	add	r3, r3, #1
	str	r3, [fp, #-96]
	cmp	r3, r2
	bne	.L783
.L782:
	ldr	r3, [fp, #4]
	cmp	r3, #1
	beq	.L843
	ldr	r3, [fp, #4]
	cmp	r3, #0
	bne	.L788
	ldr	r2, [fp, #-116]
	ldr	r3, [fp, #-132]
	cmp	r2, r3
	bls	.L793
.L794:
	add	r3, r3, #2
	cmp	r2, r3
	bhi	.L794
.L793:
	ldr	ip, [r10, #48]
	mov	r2, #1048576
	mov	r1, #128
	ldr	r0, [fp, #-124]
	blx	ip
	ldr	ip, [r10, #48]
	mov	r2, #1048576
	mov	r1, #128
	ldr	r0, [fp, #-112]
	blx	ip
	b	.L788
.L843:
	ldr	r3, [fp, #-116]
	cmp	r3, #0
	beq	.L788
	ldr	r2, [fp, #-132]
	mov	r4, #0
	ldr	r0, [fp, #-120]
	mov	lr, r3
	ldr	ip, [fp, #-112]
	mla	r6, r5, r2, r6
	mov	r5, r5, asl #1
	add	r6, r6, #1
.L789:
	cmp	r0, #0
	beq	.L792
	mov	r3, r4, lsr #1
	mov	r2, r6
	mul	r3, r0, r3
	sub	r1, r3, #-67108863
	add	r8, r3, r0
	sub	r1, r1, #65011712
	add	r8, ip, r8
	add	r1, ip, r1
	add	r3, ip, r3
.L790:
	ldrb	r7, [r2, #-1]	@ zero_extendqisi2
	strb	r7, [r1, #1]!
	ldrb	r7, [r2], #2	@ zero_extendqisi2
	strb	r7, [r3], #1
	cmp	r3, r8
	bne	.L790
.L792:
	add	r4, r4, #2
	add	r6, r6, r5
	cmp	lr, r4
	bhi	.L789
	b	.L788
.L842:
	ldr	r3, [fp, #-116]
	cmp	r3, #0
	beq	.L788
	ldr	r2, [fp, #-112]
	add	r6, r6, #1
	ldr	lr, [fp, #-120]
	mov	r7, r3
	mov	r4, #0
	mov	r3, r2
.L772:
	cmp	lr, #0
	moveq	ip, r3
	beq	.L775
	sub	r1, r3, #-67108863
	add	ip, r3, lr
	sub	r1, r1, #65011712
	mov	r2, r6
.L773:
	ldrb	r0, [r2, #-1]	@ zero_extendqisi2
	strb	r0, [r1, #1]!
	ldrb	r0, [r2], #2	@ zero_extendqisi2
	strb	r0, [r3], #1
	cmp	ip, r3
	bne	.L773
.L775:
	add	r4, r4, #1
	mov	r3, ip
	cmp	r4, r7
	add	r6, r6, r5
	bne	.L772
	b	.L788
.L841:
	ldr	r10, .L844+12
	mov	r0, r6
	ldr	r1, [fp, #-100]
	ldr	r2, [fp, #-108]
	ldr	ip, [r10, #44]
	blx	ip
	b	.L795
.L845:
	.align	2
.L844:
	.word	.LC71
	.word	.LC72
	.word	.LC73
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VDMHAL_V400R003_WriteBigTitle1DYuv, .-VDMHAL_V400R003_WriteBigTitle1DYuv
	.align	2
	.global	CRG_ConfigReg
	.type	CRG_ConfigReg, %function
CRG_ConfigReg:
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
	ldr	ip, .L853
	mov	lr, r2
	ldrb	ip, [ip]	@ zero_extendqisi2
	cmp	ip, #1
	bne	.L852
	cmp	r0, #0
	bne	.L848
	ldr	r3, .L853+4
	ldr	r3, [r3]
	ldr	r2, [r3, #120]
	ubfx	r0, r2, #8, #2
	cmp	r1, r0
	bfine	r2, r1, #8, #2
	strne	r2, [r3, #120]
	ldmfd	sp, {fp, sp, pc}
.L848:
	cmp	r0, #1
	ldmnefd	sp, {fp, sp, pc}
	ldr	r3, .L853+4
	ldr	r3, [r3]
	ldr	r2, [r3, #120]
	bfi	r2, r1, #12, #5
	bfc	r2, #17, #1
	str	r2, [r3, #120]
	ldr	r2, [r3, #120]
	orr	r2, r2, #131072
	str	r2, [r3, #120]
	ldmfd	sp, {fp, sp, pc}
.L852:
	mov	r3, r1
	mov	r2, r0
	mov	r1, #0
	mov	r0, lr
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDH_Record_RegData
.L854:
	.align	2
.L853:
	.word	g_HalDisable
	.word	g_pstRegCrg
	UNWIND(.fnend)
	.size	CRG_ConfigReg, .-CRG_ConfigReg
	.align	2
	.global	MFDE_ConfigReg
	.type	MFDE_ConfigReg, %function
MFDE_ConfigReg:
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
	ldr	ip, .L861
	mov	lr, r3
	ldrb	ip, [ip]	@ zero_extendqisi2
	cmp	ip, #1
	bne	.L859
	cmp	r2, #0
	ble	.L860
	mov	r3, r2
	str	ip, [sp]
	ldr	r2, .L861+4
	mov	r0, #32
	ldr	r1, .L861+8
	bl	dprint_vfmw
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L860:
	movw	r3, #1228
	ldr	ip, .L861+12
	mul	r2, r3, r2
	ldr	r3, [ip, r2]
	str	r1, [r3, r0]
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L859:
	mov	r3, r1
	mov	r2, r0
	mov	r1, #2
	mov	r0, lr
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDH_Record_RegData
.L862:
	.align	2
.L861:
	.word	g_HalDisable
	.word	.LANCHOR0+528
	.word	.LC30
	.word	g_HwMem
	UNWIND(.fnend)
	.size	MFDE_ConfigReg, .-MFDE_ConfigReg
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.54934, %object
	.size	__func__.54934, 24
__func__.54934:
	.ascii	"VDMHAL_V400R003_OpenHAL\000"
	.type	__func__.54971, %object
	.size	__func__.54971, 27
__func__.54971:
	.ascii	"VDMHAL_V400R003_CalcFsSize\000"
	.space	1
	.type	__func__.55040, %object
	.size	__func__.55040, 27
__func__.55040:
	.ascii	"VDMHAL_V400R003_ArrangeMem\000"
	.space	1
	.type	__FUNCTION__.55041, %object
	.size	__FUNCTION__.55041, 27
__FUNCTION__.55041:
	.ascii	"VDMHAL_V400R003_ArrangeMem\000"
	.space	1
	.type	__func__.54983, %object
	.size	__func__.54983, 35
__func__.54983:
	.ascii	"VDMHAL_V400R003_FillMemArrangeInfo\000"
	.space	1
	.type	__func__.55070, %object
	.size	__func__.55070, 25
__func__.55070:
	.ascii	"VDMHAL_V400R003_ResetVdm\000"
	.space	3
	.type	__func__.55083, %object
	.size	__func__.55083, 37
__func__.55083:
	.ascii	"VDMHAL_V400R003_SetSmmuPageTableAddr\000"
	.space	3
	.type	__func__.55104, %object
	.size	__func__.55104, 26
__func__.55104:
	.ascii	"VDMHAL_V400R003_GlbResetX\000"
	.space	2
	.type	__func__.55093, %object
	.size	__func__.55093, 25
__func__.55093:
	.ascii	"VDMHAL_V400R003_GlbReset\000"
	.space	3
	.type	__func__.55165, %object
	.size	__func__.55165, 25
__func__.55165:
	.ascii	"VDMHAL_V400R003_CheckReg\000"
	.space	3
	.type	__func__.55182, %object
	.size	__func__.55182, 28
__func__.55182:
	.ascii	"VDMHAL_V400R003_ReadMMUMask\000"
	.type	__func__.55187, %object
	.size	__func__.55187, 29
__func__.55187:
	.ascii	"VDMHAL_V400R003_WriteMMUMask\000"
	.space	3
	.type	__func__.55194, %object
	.size	__func__.55194, 27
__func__.55194:
	.ascii	"VDMHAL_V400R003_PrepareDec\000"
	.space	1
	.type	__func__.55216, %object
	.size	__func__.55216, 27
__func__.55216:
	.ascii	"VDMHAL_V400R003_IsVdmReady\000"
	.space	1
	.type	__func__.55221, %object
	.size	__func__.55221, 25
__func__.55221:
	.ascii	"VDMHAL_V400R003_IsVdmRun\000"
	.space	3
	.type	__func__.55235, %object
	.size	__func__.55235, 27
__func__.55235:
	.ascii	"VDMHAL_V400R003_BackupInfo\000"
	.space	1
	.type	__func__.55281, %object
	.size	__func__.55281, 25
__func__.55281:
	.ascii	"VDMHAL_V400R003_CfgRpMsg\000"
	.space	3
	.type	__func__.55315, %object
	.size	__func__.55315, 30
__func__.55315:
	.ascii	"VDMHAL_V400R003_MakeDecReport\000"
	.space	2
	.type	__func__.55486, %object
	.size	__func__.55486, 15
__func__.55486:
	.ascii	"MFDE_ConfigReg\000"
	.space	1
	.type	__func__.55349, %object
	.size	__func__.55349, 30
__func__.55349:
	.ascii	"VDMHAL_V400R003_StartHwRepair\000"
	.space	2
	.type	__func__.55365, %object
	.size	__func__.55365, 30
__func__.55365:
	.ascii	"VDMHAL_V400R003_CalVdhClkSkip\000"
	.space	2
	.type	__func__.55404, %object
	.size	__func__.55404, 30
__func__.55404:
	.ascii	"VDMHAL_V400R003_StartHwDecode\000"
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
	ASCII(.ascii	"VDMHAL_V400R003_OpenHAL: Size error!\000" )
	.space	3
.LC4:
	ASCII(.ascii	"VdhId is wrong!!!\012\000" )
	.space	1
.LC5:
	ASCII(.ascii	"g_VdmRegVirAddr, g_VdmResetVirAddr = %p\012\000" )
	.space	3
.LC6:
	ASCII(.ascii	"BPDRegVirAddr %p\012\000" )
	.space	2
.LC7:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200R003_OpenHAL\012\000" )
	.space	3
.LC8:
	ASCII(.ascii	"!!!!!! HAL memory not enouph! need %d, have %d\012\000" )
.LC9:
	ASCII(.ascii	"image size out of range\000" )
.LC10:
	ASCII(.ascii	"VDMHAL ArrangeMem HEVC/VP9 10 bit\012\000" )
	.space	1
.LC11:
	ASCII(.ascii	"VidStd Invalid\000" )
	.space	1
.LC12:
	ASCII(.ascii	"VDMHAL_V400R003_CalcFsSize err!\012\000" )
	.space	3
.LC13:
	ASCII(.ascii	"%s, need arrange, Size: %#x, Num: %#x, RefChanged: " )
	ASCII(.ascii	"%#x\012\000" )
.LC14:
	ASCII(.ascii	"DelAllFrameMemRecord err in VDMHAL_V400R003_Arrange" )
	ASCII(.ascii	"Mem!\012\000" )
	.space	3
.LC15:
	ASCII(.ascii	"DFS, report event. Size: %#x, Num: %#x, RefChanged:" )
	ASCII(.ascii	" %#x\012\000" )
	.space	3
.LC16:
	ASCII(.ascii	"pVdmMemArrange is NULL\000" )
	.space	1
.LC17:
	ASCII(.ascii	"DFS, no ref frame!\012\000" )
.LC18:
	ASCII(.ascii	"DFS, Frame number = %d > 30, Then, Frame num = 30, " )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC19:
	ASCII(.ascii	"VDMHAL_V400R003_ArrangeMem Mem addr is NULL\000" )
.LC20:
	ASCII(.ascii	"'pVdmMemArrange' is NULL\000" )
	.space	3
.LC21:
	ASCII(.ascii	"MemSize not enough for pmv slot\000" )
.LC22:
	ASCII(.ascii	"VDMHAL_V200R003_ArrangeMem get ChanWidth/ChanHeight" )
	ASCII(.ascii	" failed!\012\000" )
	.space	3
.LC23:
	ASCII(.ascii	"ImgSlotLen > ChanSlotLen\000" )
	.space	3
.LC24:
	ASCII(.ascii	"cann't allocate img slot\000" )
	.space	3
.LC25:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC26:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V400R003_ResetVdm\012\000" )
	.space	2
.LC27:
	ASCII(.ascii	"VDMHAL_V400R003_ResetVdm: map vdm register fail, vi" )
	ASCII(.ascii	"r(reg) = (%p)\012\000" )
	.space	2
.LC28:
	ASCII(.ascii	"%s module id %d failed!\012\000" )
	.space	3
.LC29:
	ASCII(.ascii	"%s module id %d success!\012\000" )
	.space	2
.LC30:
	ASCII(.ascii	"%s: WR_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\012\000" )
	.space	3
.LC31:
	ASCII(.ascii	"%s: map vdm register 0x%x failed!\012\000" )
	.space	1
.LC32:
	ASCII(.ascii	"%s VdhId %d failed!\012\000" )
	.space	3
.LC33:
	ASCII(.ascii	"%s VdhId %d success!\012\000" )
	.space	2
.LC34:
	ASCII(.ascii	"GLB Reset failed, wrong VDH ID!\012\000" )
	.space	3
.LC35:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200R003_ClearIntState\012\000" )
	.space	1
.LC36:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
.LC37:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200_MaskInt\012\000" )
	.space	3
.LC38:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200R003_EnableInt\012\000" )
	.space	1
.LC39:
	ASCII(.ascii	"%s: VdhId(%d) Invalid!\012\000" )
.LC40:
	ASCII(.ascii	"%s: VdhId is wrong!\012\000" )
	.space	3
.LC41:
	ASCII(.ascii	"%s: vdm register virtual address not mapped, reset " )
	ASCII(.ascii	"failed!\012\000" )
.LC42:
	ASCII(.ascii	"%s: unkown reg_id = %d\012\000" )
.LC43:
	ASCII(.ascii	"%s: RD_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\012\000" )
	.space	3
.LC44:
	ASCII(.ascii	"%s: pDecParam(%p) = NULL\012\000" )
	.space	2
.LC45:
	ASCII(.ascii	"VDM register not mapped yet!\000" )
	.space	3
.LC46:
	ASCII(.ascii	"VDM register not mapped yet!\012\000" )
	.space	2
.LC47:
	ASCII(.ascii	"pBackUpInfo = NULL!\000" )
.LC48:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200R003_BackupInfo\012\000" )
.LC49:
	ASCII(.ascii	"can NOT map vir addr for up-msg\000" )
.LC50:
	ASCII(.ascii	"ReadUpMsgSlot error! pDst=%p, pSrc=%p\012\000" )
	.space	1
.LC51:
	ASCII(.ascii	"ReadUpMsgSlot error! upmsg_size(%d) > 512\012\000" )
	.space	1
.LC52:
	ASCII(.ascii	"WriteMsgSlot error!\012\000" )
	.space	3
.LC53:
	ASCII(.ascii	"can not map repair msg virtual address!\000" )
.LC54:
	ASCII(.ascii	"ValidGroupNum=%d out of range!\012\000" )
.LC55:
	ASCII(.ascii	"align_mb error\012\000" )
.LC56:
	ASCII(.ascii	"[%s][%d]sclie_num is wrong! %d \012\000" )
	.space	3
.LC57:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V400_CfgRpReg\012\000" )
	.space	2
.LC58:
	ASCII(.ascii	"'pMakeDecReport' is NULL\000" )
	.space	3
.LC59:
	ASCII(.ascii	"'pDecReport' is NULL\000" )
	.space	3
.LC60:
	ASCII(.ascii	"pDecReport->DecSliceNum(%d) > %d, set to 0 for full" )
	ASCII(.ascii	" repair.\012\000" )
	.space	3
.LC61:
	ASCII(.ascii	"\012***** UpMsg DecSliceNum=%d\012\000" )
	.space	3
.LC62:
	ASCII(.ascii	"\012***** Up Msg (phy addr: %#8x) *****\012\000" )
	.space	2
.LC63:
	ASCII(.ascii	"\0120x%02x 0x%08x 0x%08x 0x%08x 0x%08x\012\000" )
	.space	3
.LC64:
	ASCII(.ascii	"\012***** Up Msg print finished *****\012\000" )
.LC65:
	ASCII(.ascii	"VdhId is wrong! VDMHAL_V200R003_PrepareRepair\012\000" )
	.space	1
.LC66:
	ASCII(.ascii	"vdm register virtual address not mapped, VDMHAL_V20" )
	ASCII(.ascii	"0R003_PrepareRepair failed!\012\000" )
.LC67:
	ASCII(.ascii	"FIRST_REPAIR Parameter Error!\012\000" )
	.space	1
.LC68:
	ASCII(.ascii	"SECOND_REPAIR Parameter Error!\012\000" )
.LC69:
	ASCII(.ascii	"%s: pMfdeTask(%p) = NULL\012\000" )
	.space	2
.LC70:
	ASCII(.ascii	"%s The channel %d is not active\012\000" )
	.space	3
.LC71:
	ASCII(.ascii	"BigTile1d_y\000" )
.LC72:
	ASCII(.ascii	"failed mem_allocMemBlock BigTile_yuv save!\012\000" )
.LC73:
	ASCII(.ascii	"BigTile1d_uv\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
