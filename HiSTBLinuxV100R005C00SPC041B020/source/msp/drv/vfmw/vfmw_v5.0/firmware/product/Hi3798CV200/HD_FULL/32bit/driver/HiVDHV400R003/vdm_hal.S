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
	mov	r8, r0
	str	r3, [fp, #-60]
	bhi	.L27
	sub	r3, r2, #32
	mov	r10, r2
	cmp	r3, ip
	bhi	.L27
	ldr	r0, .L61
	add	r3, r1, #15
	add	lr, r2, #15
	mov	r3, r3, asr #4
	mov	lr, lr, asr #4
	ldrb	r4, [r0]	@ zero_extendqisi2
	cmp	r3, #45
	cmple	lr, #36
	mov	r0, r3, asl #4
	movle	ip, #64
	movgt	ip, #32
	cmp	r4, #1
	ldr	r4, [fp, #4]
	moveq	ip, #64
	cmp	r4, #16
	beq	.L32
	cmp	r4, #17
	beq	.L54
	mul	r3, r3, ip
	ldr	r9, .L61+4
	add	ip, r0, #255
	mov	r0, #0
	mov	r4, r0
	add	r10, r2, #31
	str	r0, [fp, #-76]
	bic	r0, ip, #255
	str	r0, [fp, #-48]
	bic	r10, r10, #31
	mul	r3, r3, lr
	ldr	r0, [r9, r8, asl #2]
	str	r4, [fp, #-72]
	add	r3, r3, #143
	bic	r5, r3, #127
	mov	r3, r5, asr #1
	str	r3, [fp, #-64]
.L44:
	ldr	r3, [r0, #1200]
	cmp	r3, #1
	movne	r3, #0
	strne	r3, [fp, #-68]
	movne	r7, r3
	beq	.L55
.L35:
	ldr	ip, [fp, #4]
	ldr	r4, [fp, #4]
	cmp	ip, #3
	cmpne	ip, #0
	sub	r4, r4, #16
	movne	ip, #1
	moveq	ip, #0
	cmp	r4, #1
	movls	ip, #0
	andhi	ip, ip, #1
	cmp	ip, #0
	beq	.L56
	mov	r2, #1024
	str	r2, [fp, #-56]
	ldr	r2, [fp, #-60]
	mov	r6, #0
	str	r6, [fp, #-52]
	cmp	r2, #1
	beq	.L57
.L37:
	ldr	r2, [fp, #-48]
	add	ip, r7, r7, lsl #1
	ldr	r1, [fp, #-72]
	mul	r10, r2, r10
	ldr	r2, [fp, #-76]
	mul	r4, r2, r1
	add	r1, r10, r10, lsl #1
	mov	r1, r1, lsr #1
	add	r4, r4, r4, lsl #1
	add	r2, r1, r4, lsr #1
	add	r4, r2, ip, lsr #1
	add	r2, r0, #290816
	ldr	r1, [r2, #1840]
	cmp	r1, #0
	beq	.L58
.L39:
	ldr	r2, [fp, #4]
	ldr	r1, [fp, #-68]
	cmp	r2, #17
	add	r2, r0, #294912
	str	r3, [r2, #1128]
	ldr	r3, [fp, #-52]
	str	r4, [r2, #1352]
	str	r1, [r2, #1124]
	str	r3, [r2, #1112]
	ldr	r3, [fp, #-64]
	str	r6, [r2, #1116]
	str	r3, [r2, #1324]
	beq	.L40
	add	r3, r0, #290816
	ldr	r1, [fp, #-48]
	ldr	r3, [r3, #1848]
	mov	ip, r1, asl #4
	add	r1, r10, r7
	cmp	r3, #20
	str	r1, [r2, #1148]
	ldr	r1, [fp, #4]
	movge	r3, #20
	str	ip, [r2, #80]
	cmp	r1, #16
	str	r3, [r2, #1328]
	beq	.L59
.L42:
	ldr	r3, [r9, r8, asl #2]
	mov	r0, #1024
	ldr	r2, [fp, #-56]
	add	r4, r4, r2
	add	r2, r3, #294912
	add	r4, r4, #32
	str	r5, [r2, #1356]
	add	r1, r4, r5
	str	r0, [r2, #1364]
	ldrsb	r3, [r3, #168]
	cmp	r3, #1
	ldr	r3, [fp, #8]
	addeq	r1, r1, r0
	cmp	r3, #0
	beq	.L49
	str	r4, [r3, #4]
	mov	r0, #0
	ldr	r3, [r9, r8, asl #2]
	mov	r2, #1024
	ldr	ip, [fp, #8]
	add	r3, r3, #290816
	ldr	r3, [r3, #1848]
	str	r5, [ip, #12]
	str	r3, [ip, #8]
	ldr	r3, [r9, r8, asl #2]
	add	r3, r3, #294912
	ldr	r3, [r3, #1328]
	str	r1, [ip]
	str	r2, [ip, #20]
	str	r3, [ip, #16]
.L50:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L56:
	add	r1, r1, #255
	cmp	r1, #0
	add	ip, r1, #255
	movlt	r1, ip
	adds	r6, r2, #63
	addmi	r6, r2, #126
	mov	r1, r1, asr #8
	mov	r6, r6, asr #6
	mov	r2, r1, asl #6
	str	r2, [fp, #-52]
	mov	r6, r6, asl #3
	mul	r6, r2, r6
	add	r2, r6, #1024
	str	r2, [fp, #-56]
	ldr	r2, [fp, #-60]
	cmp	r2, #1
	bne	.L37
.L57:
	ldr	r2, [fp, #-48]
	mov	r4, lr, asl #5
	add	r4, r4, lr, lsl #4
	mul	r10, r2, r10
	mul	r4, r2, r4
	add	r2, r0, #290816
	ldr	r1, [r2, #1840]
	cmp	r1, #0
	bne	.L39
.L58:
	ldr	ip, .L61+8
	add	r0, r0, #294912
	add	r0, r0, #80
	movw	r2, #1288
	str	r3, [fp, #-60]
	ldr	ip, [ip, #48]
	blx	ip
	ldr	r0, [r9, r8, asl #2]
	ldr	r3, [fp, #-60]
	b	.L39
.L32:
	ldr	r9, .L61+4
	add	ip, r1, #255
	mul	r3, r0, lr
	bic	r0, ip, #255
	str	r0, [fp, #-48]
	ldr	r0, [r9, r8, asl #2]
	add	ip, r3, #143
	bic	r5, ip, #127
	ldr	r3, [r0, #1492]
	cmp	r3, #8
	ble	.L60
	ldr	r3, [fp, #-48]
	add	r7, r2, #31
	mov	r3, r3, lsr #2
	str	r3, [fp, #-72]
	bic	r3, r7, #31
	str	r3, [fp, #-76]
	mov	r3, r5, asr #1
	str	r3, [fp, #-64]
	b	.L44
.L55:
	add	r3, r1, #508
	add	ip, r1, #1020
	add	r3, r3, #3
	add	ip, ip, #2
	cmp	r3, #0
	movlt	r3, ip
	adds	r7, r2, #63
	addmi	r7, r2, #126
	mov	r3, r3, asr #9
	mov	r7, r7, asr #6
	mov	r3, r3, asl #4
	str	r3, [fp, #-68]
	mov	r7, r7, asl #5
	mul	r7, r3, r7
	mov	r3, r7
	b	.L35
.L59:
	ldr	r3, [r0, #1492]
	cmp	r3, #8
	ble	.L42
	ldr	r0, [fp, #-72]
	add	r3, r7, r7, lsl #1
	ldr	r1, [fp, #-76]
	add	r10, r10, r10, lsl #1
	mov	r3, r3, lsr #1
	add	r10, r3, r10, lsr #1
	str	r10, [r2, #1140]
	mul	r1, r0, r1
	mov	r0, r0, asl #5
	str	r0, [r2, #1132]
	mov	r0, #31
	add	r7, r1, r7, lsr #1
	ldr	r1, .L61+12
	str	r7, [r2, #1144]
	bl	dprint_vfmw
	b	.L42
.L40:
	ldr	r3, [fp, #-48]
	add	r7, r10, r7
	str	r7, [r2, #1148]
	mov	ip, r3, asl #4
	mov	r3, #1
	str	ip, [r2, #80]
	str	r3, [r2, #1328]
	b	.L42
.L54:
	ldr	r9, .L61+4
	mov	r0, #0
	add	ip, r1, #255
	add	r10, r2, #63
	mov	r3, #64
	mov	r5, #128
	mov	r4, r0
	str	r0, [fp, #-76]
	bic	ip, ip, #255
	movt	r3, 18
	ldr	r0, [r9, r8, asl #2]
	bic	r10, r10, #63
	str	ip, [fp, #-48]
	movt	r5, 36
	str	r3, [fp, #-64]
	str	r4, [fp, #-72]
	b	.L44
.L60:
	mov	r3, #0
	mov	ip, r5, asr #1
	str	r3, [fp, #-76]
	str	ip, [fp, #-64]
	str	r3, [fp, #-72]
	b	.L44
.L49:
	ldr	r0, [fp, #8]
	b	.L50
.L27:
	ldr	r3, .L61+16
	mov	r0, #0
	ldr	r2, .L61+20
	ldr	r1, .L61+24
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L50
.L62:
	.align	2
.L61:
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
	@ args = 20, pretend = 0, frame = 96
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #108)
	sub	sp, sp, #108
	mov	r4, r2
	ldr	r2, [fp, #16]
	mov	r8, #0
	str	r0, [fp, #-104]
	mov	r10, r3
	cmn	r2, #2
	str	r1, [fp, #-112]
	ldr	r5, [fp, #4]
	str	r8, [fp, #-96]
	str	r8, [fp, #-92]
	str	r8, [fp, #-88]
	str	r8, [fp, #-84]
	str	r8, [fp, #-80]
	str	r8, [fp, #-76]
	ldr	r7, [fp, #20]
	beq	.L133
	cmp	r2, r8
	blt	.L134
	mov	r0, r2
	ldr	r6, .L198
	bl	VCTRL_GetVidStd
	ldr	r3, [fp, #16]
	ldr	r3, [r6, r3, asl #2]
	ldr	r2, [r3, #1440]
	cmp	r2, #1
	mov	ip, r0
	beq	.L181
.L66:
	ldr	r3, [fp, #-104]
	cmp	r3, #0
	subne	r2, r4, #32
	movwne	r3, #8160
	beq	.L182
	cmp	r2, r3
	bhi	.L91
	sub	r2, r10, #32
	cmp	r2, r3
	bhi	.L91
	cmp	r7, #0
	beq	.L183
	ldr	r3, .L198+4
	cmp	r5, #20
	movw	r2, #1272
	mov	r1, #0
	mov	r0, r7
	str	ip, [fp, #-108]
	ldr	r3, [r3, #48]
	movlt	r8, r5
	movge	r8, #20
	blx	r3
	ldr	r1, .L198+8
	add	r2, r4, #15
	add	r3, r10, #15
	ldr	ip, [fp, #-108]
	mov	r2, r2, asr #4
	mov	r3, r3, asr #4
	ldrb	lr, [r1]	@ zero_extendqisi2
	cmp	r2, #45
	cmple	r3, #36
	mov	r1, r2, asl #4
	movle	r0, #64
	movgt	r0, #32
	cmp	lr, #1
	moveq	r0, #64
	cmp	ip, #16
	beq	.L184
	cmp	ip, #17
	beq	.L99
	mul	r6, r2, r0
	ldr	r0, [fp, #-112]
	mul	r6, r6, r3
	add	r6, r6, #127
	bic	r6, r6, #127
	mul	r2, r6, r8
	cmp	r2, r0
	str	r2, [fp, #-116]
	bge	.L97
	ldr	r0, [fp, #-104]
	add	r9, r4, #255
	add	r2, r10, #31
	add	r1, r1, #255
	add	r5, r0, #1020
	mov	lr, #0
	add	r5, r5, #3
	bic	r1, r1, #255
	bic	r5, r5, #1020
	str	r1, [fp, #-108]
	bic	r5, r5, #3
	str	lr, [fp, #-128]
	rsb	r1, r0, r5
	str	r8, [r7, #1248]
	add	r0, r10, #63
	str	r1, [fp, #-120]
	bic	r10, r2, #31
	str	lr, [fp, #-124]
	mov	r2, r6, asr #1
	str	r2, [r7, #1244]
.L101:
	add	r2, r4, #508
	add	r4, r4, #1020
	add	r2, r2, #3
	add	r4, r4, #2
	cmp	r2, #0
	add	r1, r0, #63
	add	r8, r9, #255
	movlt	r2, r4
	cmp	r0, #0
	mov	r2, r2, asr #9
	movlt	r0, r1
	cmp	r9, #0
	mov	r0, r0, asr #6
	ldr	r1, [fp, #12]
	movge	r8, r9
	mov	r9, r2, asl #9
	mov	r8, r8, asr #8
	cmp	r1, #1
	mov	lr, r0, asl #3
	mov	r2, r2, asl #4
	mov	r1, r8, asl #6
	str	r2, [r7, #1044]
	mul	r9, r0, r9
	ldr	r2, [fp, #-108]
	mul	r8, r1, lr
	str	r1, [r7, #1032]
	mov	r2, r2, asl #4
	str	r2, [r7]
	str	r9, [r7, #1048]
	str	r8, [r7, #1036]
	beq	.L185
	ldr	r2, [fp, #-124]
	add	r4, r9, r9, lsl #1
	ldr	r3, [fp, #-128]
	mul	r3, r3, r2
	ldr	r2, [fp, #-108]
	mul	r2, r10, r2
	add	r3, r3, r3, lsl #1
	add	r3, r3, r3, lsr #31
	add	r2, r2, r2, lsl #1
	mov	r3, r3, asr #1
	add	r3, r3, r2, asr #1
	add	r4, r3, r4, lsr #1
.L104:
	ldr	r3, [fp, #16]
	cmp	r3, #0
	blt	.L105
	mov	r0, r3
	str	ip, [fp, #-136]
	bl	VCTRL_GetChanWidth
	str	r0, [fp, #-132]
	ldr	r0, [fp, #16]
	bl	VCTRL_GetChanHeight
	ldr	r2, [fp, #-132]
	ldr	ip, [fp, #-136]
	cmn	r0, #1
	cmnne	r2, #1
	beq	.L186
	ldr	r3, [fp, #12]
	add	r2, r2, #15
	bic	r2, r2, #15
	cmp	r3, #1
	add	r3, r0, #15
	add	r0, r2, #255
	bic	r3, r3, #15
	bic	r0, r0, #255
	beq	.L187
	add	r1, r2, #508
	adds	lr, r3, #63
	add	r1, r1, #3
	addmi	lr, r3, #126
	add	r2, r2, #1020
	cmp	r1, #0
	add	r2, r2, #2
	mov	lr, lr, asr #6
	movge	r2, r1
	mov	r2, r2, asr #9
	mov	r3, lr, asl #7
	mul	r0, lr, r0
	mov	r2, r2, asl #4
	sub	lr, r3, lr, asl #5
	mul	r2, r2, lr
	mov	r1, r0, asl #7
	sub	r0, r1, r0, asl #5
	add	r2, r0, r2, lsr #1
.L108:
	cmp	r4, r2
	ldrgt	r3, .L198+12
	bgt	.L178
	ldr	r3, [fp, #-108]
	cmp	ip, #16
	mul	r10, r10, r3
	add	r3, r9, r10
	str	r3, [r7, #1068]
	bne	.L138
	ldr	r3, .L198
	ldr	r2, [fp, #16]
	str	ip, [fp, #-108]
	ldr	r0, [r3, r2, asl #2]
	add	r0, r0, #294912
	add	r0, r0, #1968
	bl	IsMain10Profile
	ldr	ip, [fp, #-108]
	cmp	r0, #0
	bne	.L188
.L138:
	mov	r9, #1
	b	.L110
.L134:
	mov	ip, #21
	b	.L66
.L105:
	ldr	r3, [fp, #-108]
	mla	r10, r10, r3, r9
	mov	r9, #0
	str	r10, [r7, #1068]
.L110:
	ldr	r3, [fp, #8]
	cmp	r3, #0
	streq	r3, [r7, #1080]
	streq	r3, [r7, #1040]
	bne	.L189
.L112:
	ldr	r3, .L198+16
.L178:
	mov	r0, #0
	ldr	r2, .L198+20
	ldr	r1, .L198+24
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L169
.L189:
	cmp	ip, #3
	cmpne	ip, #0
	sub	ip, ip, #16
	ldr	r2, [fp, #-116]
	movne	r3, #1
	moveq	r3, #0
	cmp	ip, #1
	movls	ip, #0
	andhi	ip, r3, #1
	ldr	r3, [fp, #-112]
	cmp	ip, #0
	rsb	r0, r2, r3
	ldr	r3, [fp, #-120]
	rsb	r0, r3, r0
	ldr	r3, [fp, #8]
	beq	.L113
	cmp	r3, #32
	mov	r1, r4
	movlt	r10, r3
	movge	r10, #32
	bl	__aeabi_idiv
	mov	r3, #0
	str	r3, [r7, #1040]
	cmp	r10, r0
	movlt	r3, r10
	movge	r3, r0
	str	r3, [r7, #1080]
.L114:
	cmp	r3, #0
	beq	.L112
	ldrgt	r0, [r7, #1252]
	movgt	r1, r7
	movgt	r2, #0
	ble	.L190
.L117:
	add	r2, r2, #1
	str	r5, [r1, #8]
	cmp	r2, r3
	add	r5, r5, r4
	add	r1, r1, #12
	bne	.L117
	ldr	r2, [fp, #-120]
	mov	r1, #0
	mla	r4, r3, r4, r2
	add	r2, r3, r0
	str	r2, [r7, #1252]
	add	r0, r7, #388
	ldr	r2, [fp, #-104]
	add	r2, r4, r2
.L119:
	add	r1, r1, #1
	str	r2, [r0, #4]!
	cmp	r1, r3
	add	r2, r2, #32
	bne	.L119
.L129:
	ldr	ip, [r7, #1040]
	add	r4, r4, r3, lsl #5
	cmp	ip, #0
	beq	.L121
	ldr	r2, [fp, #-104]
	add	r0, r7, #900
	add	r1, r4, r2
	mov	r2, #0
.L122:
	add	r2, r2, #1
	str	r1, [r0, #4]!
	cmp	r2, ip
	add	r1, r1, r8
	bne	.L122
	mul	ip, r8, ip
.L121:
	ldr	lr, [r7, #1248]
	ldr	r2, [fp, #-104]
	add	r4, ip, r4
	cmp	lr, #0
	add	ip, r4, r2
	ble	.L127
	ldr	r5, [r7, #1256]
	mov	r0, r7
	mov	r1, ip
	mov	r2, #0
.L126:
	add	r2, r2, #1
	str	r1, [r0, #1084]
	cmp	r2, lr
	add	r1, r1, r6
	add	r0, r0, #8
	bne	.L126
	add	r2, lr, r5
	str	r2, [r7, #1256]
.L127:
	mla	r6, r6, lr, r4
	cmp	r9, #0
	str	r6, [r7, #4]
	beq	.L125
	ldr	r2, .L198
	ldr	r1, [fp, #16]
	ldr	r2, [r2, r1, asl #2]
	cmp	r2, #0
	beq	.L125
	cmp	r3, #0
	ble	.L125
	add	r2, r6, ip
	mov	r0, r7
	mov	r1, #0
.L128:
	add	r1, r1, #1
	str	r2, [r0, #520]
	cmp	r1, r3
	add	r2, r2, #1024
	add	r0, r0, #12
	bne	.L128
.L125:
	add	r6, r6, r3, lsl #10
	mov	r3, #0
	str	r6, [r7, #4]
.L169:
	mov	r0, r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L133:
	mov	ip, #16
	b	.L66
.L113:
	cmp	r3, #32
	add	r1, r4, r8
	movlt	r10, r3
	movge	r10, #32
	bl	__aeabi_idiv
	cmp	r10, r0
	movlt	r3, r10
	movge	r3, r0
	str	r3, [r7, #1080]
	str	r3, [r7, #1040]
	b	.L114
.L184:
	mul	r6, r1, r3
	ldr	r1, [fp, #-112]
	add	r6, r6, #127
	bic	r6, r6, #127
	mul	r2, r6, r8
	cmp	r2, r1
	str	r2, [fp, #-116]
	bge	.L97
	ldr	r2, [fp, #-104]
	add	r9, r4, #255
	ldr	r1, [fp, #16]
	add	r5, r2, #1020
	str	r8, [r7, #1248]
	add	r5, r5, #3
	cmp	r1, #0
	bic	r5, r5, #1020
	bic	r1, r9, #255
	bic	r5, r5, #3
	str	r1, [fp, #-108]
	rsb	r2, r2, r5
	str	r2, [fp, #-120]
	mov	r2, r6, asr #1
	str	r2, [r7, #1244]
	blt	.L174
	str	r3, [fp, #-124]
	ldr	r2, .L198
	ldr	r3, [fp, #16]
	str	ip, [fp, #-128]
	ldr	r0, [r2, r3, asl #2]
	add	r0, r0, #294912
	add	r0, r0, #1968
	bl	IsMain10Profile
	ldr	r3, [fp, #-124]
	ldr	ip, [fp, #-128]
	cmp	r0, #0
	beq	.L174
	ldr	r0, [fp, #-108]
	add	r1, r10, #31
	bic	r1, r1, #31
	str	r1, [fp, #-128]
	cmp	r0, #0
	add	r2, r0, #3
	movge	r2, r0
	add	r0, r10, #63
	mov	r2, r2, asr #2
	str	r2, [fp, #-124]
	b	.L101
.L181:
	cmp	r0, #20
	bhi	.L191
	add	r1, r3, #290816
	sub	lr, fp, #96
	ldr	r3, [fp, #12]
	mov	r2, r10
	ldr	r5, [r1, #1844]
	mov	r1, r4
	str	r0, [sp]
	str	lr, [sp, #4]
	ldr	r0, [fp, #16]
	bl	VDMHAL_V400R003_CalcFsSize
	cmp	r0, #0
	bne	.L192
	ldr	r3, [fp, #16]
	ldr	r8, [fp, #-96]
	ldr	r3, [r6, r3, asl #2]
	cmp	r5, r8
	str	r3, [fp, #-112]
	add	r3, r3, #290816
	ldr	r5, [r3, #1848]
	ldr	r2, [r3, #1864]
	bne	.L71
	cmp	r2, #1
	beq	.L71
	ldr	r3, [r3, #1840]
	cmp	r3, #1
	beq	.L71
	cmp	r7, #0
	beq	.L193
	ldr	r3, [fp, #-112]
	cmp	r5, #0
	add	r4, r3, #294912
	add	r3, r4, #80
	str	r3, [fp, #-136]
	ldr	r3, [r4, #1352]
	str	r3, [fp, #-116]
	ldrne	r3, [fp, #-112]
	beq	.L194
.L82:
	add	r3, r3, #290816
	ldr	r2, [r3, #1852]
	cmp	r2, #30
	bgt	.L195
.L83:
	ldr	r3, .L198+4
	mov	r0, #8
	str	r2, [r4, #1160]
	str	r3, [fp, #-132]
	ldr	r3, [r3, #12]
	blx	r3
	ldr	r3, [fp, #16]
	ldr	r0, [r6, r3, asl #2]
	add	r10, r0, #294912
	ldr	r3, [r10, #1392]
	cmp	r3, #0
	beq	.L88
	ldr	r2, [r4, #1116]
	mov	lr, #0
	ldr	r3, [r4, #1332]
	ldr	r1, [r4, #1328]
	str	r2, [fp, #-120]
	add	r2, r3, #73728
	mov	r8, r3, asl #4
	add	r2, r2, #245
	str	r1, [fp, #-124]
	sub	r8, r8, r3, asl #2
	ldr	r1, [fp, #-112]
	mov	r2, r2, asl #2
	add	r3, r3, #1
	str	r3, [fp, #-128]
	add	r8, r1, r8
	sub	r3, r2, #512
	add	r3, r1, r3
	add	r2, r1, r2
	str	r3, [fp, #-104]
	str	r2, [fp, #-140]
	str	r7, [fp, #20]
.L87:
	movw	r3, #30556
	add	r7, r0, #290816
	movt	r3, 4
	add	r3, r0, r3
	ldr	r2, [r7, #1904]
	add	r5, r8, #294912
	ldr	r3, [r3]
	mov	r9, #1
	ldr	r1, [fp, #-116]
	add	r8, r8, #12
	str	r9, [fp, #-108]
	str	r3, [r5, #92]
	add	r3, r2, #1020
	add	r3, r3, #3
	rsb	ip, r2, r1
	bic	r3, r3, #1020
	ldrb	r9, [fp, #-108]	@ zero_extendqisi2
	bic	r3, r3, #3
	str	r3, [r5, #88]
	add	ip, ip, r3
	ldr	r3, [fp, #-120]
	strb	r9, [r5, #96]
	movw	r1, #30548
	add	r3, r2, r3
	add	r2, ip, r2
	add	ip, r3, ip
	ldr	r3, [fp, #-140]
	movt	r1, 4
	add	r1, r0, r1
	add	r0, r0, #72
	str	r2, [r3, #4]!
	ldr	r2, [fp, #-104]
	str	r3, [fp, #-140]
	ldr	r3, [fp, #-128]
	str	ip, [r2, #4]!
	add	r3, r3, lr
	str	r3, [r4, #1332]
	ldr	r3, [r1]
	add	lr, lr, #1
	str	r2, [fp, #-104]
	cmp	r3, #255
	beq	.L85
	ldr	r2, [r4, #1340]
	ldr	ip, [r7, #1920]
	add	r1, r2, #1
	mov	r3, r2, asl #4
	sub	r3, r3, r2, asl #2
	add	r3, r4, r3
	str	ip, [r3, #600]
	str	r1, [r4, #1340]
.L85:
	ldr	r3, [r4, #1336]
	ldr	r2, [fp, #-124]
	ldr	r1, [fp, #-112]
	cmp	r3, r2
	add	r2, r3, #1
	add	r3, r1, r3, lsl #3
	ldrcc	r1, [r7, #1936]
	add	r3, r3, #294912
	movcc	ip, #1
	strccb	ip, [r3, #1168]
	strcc	r1, [r3, #1164]
	strcc	r2, [r4, #1336]
	ldr	r3, [r10, #1392]
	cmp	lr, r3
	bcc	.L87
	ldr	r7, [fp, #20]
.L88:
	ldr	r4, [fp, #-132]
	movw	r2, #1272
	ldr	r1, [fp, #-136]
	mov	r0, r7
	ldr	r3, [r4, #52]
	blx	r3
	ldr	r3, [fp, #16]
	mov	r1, #1
	ldr	r2, [r4, #16]
	mov	r0, #8
	ldr	r3, [r6, r3, asl #2]
	add	r3, r3, #294912
	str	r1, [r3, #1396]
	blx	r2
	mov	r3, #0
	b	.L169
.L185:
	mov	r4, r3, asl #4
	ldr	r3, [fp, #-108]
	mul	r4, r4, r3
	add	r4, r4, r4, lsl #1
	b	.L104
.L71:
	str	r2, [sp, #4]
	mov	r3, r8
	ldr	r1, .L198+28
	mov	r0, #31
	str	r5, [sp]
	ldr	r2, .L198+32
	bl	dprint_vfmw
	ldr	r3, [fp, #16]
	ldr	r1, [r6, r3, asl #2]
	add	r1, r1, #290816
	ldr	r7, [r1, #1840]
	str	r8, [r1, #1844]
	cmp	r7, #0
	beq	.L196
	ldr	r3, .L198+36
	ldr	r0, [fp, #-88]
	ldr	ip, [r3]
	str	r0, [r1, #1852]
	cmp	ip, #0
	beq	.L176
	ldr	lr, [fp, #-80]
	mov	r3, #28
	ldr	r7, [fp, #-92]
	sub	r2, fp, #72
	ldr	r9, [fp, #-84]
	mov	r1, #122
	str	lr, [fp, #-64]
	ldr	lr, [fp, #-76]
	str	r0, [fp, #-72]
	str	r4, [fp, #-52]
	ldr	r0, [fp, #16]
	str	r10, [fp, #-48]
	str	r7, [fp, #-68]
	str	r9, [fp, #-60]
	str	lr, [fp, #-56]
	blx	ip
.L176:
	ldr	r3, .L198+4
	str	r3, [fp, #-132]
.L80:
	ldr	r3, [fp, #16]
	mov	r2, r8
	ldr	r1, .L198+40
	mov	r0, #31
	ldr	ip, [r6, r3, asl #2]
	mov	r3, r5
	add	ip, ip, #290816
	ldr	ip, [ip, #1864]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [fp, #-132]
	ldr	r2, [fp, #16]
	ldr	r3, [r3]
	ldr	r4, [r6, r2, asl #2]
	blx	r3
	mov	r3, #2
	add	r4, r4, #294912
	str	r0, [r4, #1368]
	mov	r0, r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L174:
	mov	r2, #0
	add	r0, r10, #63
	str	r2, [fp, #-128]
	str	r2, [fp, #-124]
	b	.L101
.L99:
	mov	r2, r8, asl #21
	ldr	r1, [fp, #-112]
	add	r2, r2, r8, lsl #18
	str	r2, [fp, #-116]
	cmp	r2, r1
	bge	.L97
	ldr	r2, [fp, #-104]
	add	r0, r10, #63
	add	r9, r4, #255
	mov	r1, #0
	add	r5, r2, #1020
	str	r1, [fp, #-128]
	add	r5, r5, #3
	str	r8, [r7, #1248]
	bic	r5, r5, #1020
	bic	r10, r0, #63
	bic	r5, r5, #3
	str	r1, [fp, #-124]
	rsb	r2, r2, r5
	mov	r6, #2359296
	str	r2, [fp, #-120]
	bic	r2, r9, #255
	str	r2, [fp, #-108]
	mov	r2, #1179648
	str	r2, [r7, #1244]
	b	.L101
.L187:
	mul	r2, r3, r0
	add	r2, r2, r2, lsl #1
	b	.L108
.L190:
	ldr	r2, [fp, #-120]
	mla	r4, r3, r4, r2
	b	.L129
.L188:
	ldr	r3, [fp, #-124]
	add	r10, r10, r10, lsl #1
	ldr	r2, [fp, #-128]
	add	r1, r9, r9, lsl #1
	mul	r0, r2, r3
	mov	r2, r3, asl #5
	mov	r3, r10, asr #1
	str	r2, [r7, #1052]
	add	r3, r3, r1, lsr #1
	ldr	r1, .L198+44
	add	r0, r0, r9, lsr #1
	str	r3, [r7, #1060]
	str	r0, [r7, #1064]
	mov	r0, #0
	bl	dprint_vfmw
	mov	r9, #1
	ldr	ip, [fp, #-108]
	b	.L110
.L191:
	ldr	r3, .L198+48
	mov	r0, r8
	ldr	r2, .L198+20
	ldr	r1, .L198+24
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L169
.L196:
	add	r1, r1, #1872
	mov	r0, r3
	bl	DelAllFrameMemRecord
	cmp	r0, #0
	bne	.L197
.L74:
	ldr	r3, [fp, #16]
	mov	r1, #255
	ldr	r2, [r6, r3, asl #2]
	mov	r3, #0
	add	r2, r2, #290816
	add	r2, r2, #1872
.L75:
	str	r1, [r2, r3]
	add	r3, r3, #72
	cmp	r3, #2304
	bne	.L75
	cmp	r4, #1920
	cmple	r10, #1088
	ldr	r0, [fp, #-88]
	bgt	.L78
	ldr	r3, [fp, #16]
	ldr	r3, [r6, r3, asl #2]
	ldr	r2, [r3, #44]
	cmp	r2, #24
	beq	.L78
	add	r0, r0, #4
	str	r0, [fp, #-88]
.L77:
	ldr	r1, .L198+36
	add	r2, r3, #290816
	ldr	r3, [r3, #1448]
	ldr	ip, [r1]
	add	r3, r0, r3
	str	r3, [r2, #1852]
	cmp	ip, #0
	beq	.L79
	ldr	lr, [fp, #-80]
	mov	r3, #28
	ldr	r7, [fp, #-92]
	sub	r2, fp, #72
	ldr	r9, [fp, #-84]
	mov	r1, #121
	str	lr, [fp, #-64]
	ldr	lr, [fp, #-76]
	str	r0, [fp, #-72]
	str	r4, [fp, #-52]
	ldr	r0, [fp, #16]
	str	r10, [fp, #-48]
	str	r7, [fp, #-68]
	str	r9, [fp, #-60]
	str	lr, [fp, #-56]
	blx	ip
.L79:
	ldr	r3, .L198+4
	movw	r0, #34168
	mov	r2, #512
	mov	r1, #0
	movt	r0, 4
	mov	r4, #0
	mov	ip, r3
	str	r3, [fp, #-132]
	ldr	r3, [fp, #16]
	ldr	r3, [r6, r3, asl #2]
	add	r0, r3, r0
	ldr	r3, [ip, #48]
	blx	r3
	ldr	r3, [fp, #16]
	ldr	r2, .L198+52
	ldr	r1, [fp, #16]
	ldr	r3, [r6, r3, asl #2]
	add	r3, r3, #294912
	ldr	r0, [r2, r1, asl #2]
	str	r4, [r3, #1912]
	bl	FSP_ClearContextAll
	ldr	r3, [fp, #16]
	mov	r1, #1
	ldr	r3, [r6, r3, asl #2]
	add	r2, r3, #294912
	add	r3, r3, #290816
	str	r4, [r2, #1392]
	str	r1, [r3, #1824]
	str	r1, [r2, #1916]
	b	.L80
.L91:
	ldr	r3, .L198+56
	b	.L178
.L78:
	ldr	r3, [fp, #16]
	ldr	r3, [r6, r3, asl #2]
	b	.L77
.L197:
	mov	r0, r7
	ldr	r1, .L198+60
	bl	dprint_vfmw
	b	.L74
.L182:
	mov	r0, r3
	ldr	r2, .L198+20
	ldr	r3, .L198+64
	ldr	r1, .L198+24
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L169
.L97:
	ldr	r3, .L198+68
	b	.L178
.L195:
	ldr	r1, .L198+72
	mov	r0, #31
	bl	dprint_vfmw
	ldr	r3, [fp, #16]
	mov	r1, #30
	mov	r2, r1
	ldr	r3, [r6, r3, asl #2]
	add	r3, r3, #290816
	str	r1, [r3, #1852]
	b	.L83
.L194:
	ldr	r1, .L198+76
	mov	r0, #31
	bl	dprint_vfmw
	ldr	r3, [fp, #16]
	ldr	r3, [r6, r3, asl #2]
	b	.L82
.L186:
	ldr	r1, .L198+80
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L169
.L183:
	ldr	r3, .L198+84
	mov	r0, r7
	ldr	r2, .L198+20
	ldr	r1, .L198+24
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L169
.L192:
	ldr	r1, .L198+88
	mov	r0, #31
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L169
.L193:
	ldr	r3, .L198+92
	ldr	r2, .L198+96
	ldr	r1, .L198+24
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L169
.L199:
	.align	2
.L198:
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
	.word	.LC10
	.word	.LC11
	.word	s_pFspInst
	.word	.LC9
	.word	.LC14
	.word	.LC19
	.word	.LC21
	.word	.LC18
	.word	.LC17
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
	bgt	.L219
	bne	.L216
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r2, r0, #0
	beq	.L220
	ldr	r7, .L223
	ldr	r9, .L223+4
	ldr	r2, [r7]
	ldr	r3, [r9]
	ldr	r8, [r2, #36]
	ldr	r6, [r3, #120]
	tst	r6, #1
	bne	.L221
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L216:
	ldr	r1, .L223+8
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L219:
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L223+12
	ldr	r1, .L223+16
	bl	dprint_vfmw
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L221:
	ldr	r5, .L223+20
	mov	r2, #2
	bfi	r6, r2, #8, #2
	str	r6, [r3, #120]
	orr	r6, r6, #64
	ldr	r3, [r5, #108]
	blx	r3
	ldr	r3, [r9]
	str	r6, [r3, #120]
	b	.L208
.L222:
	add	r4, r4, #1
	cmp	r4, #1000
	beq	.L209
.L208:
	ldr	r3, [r5, #112]
	mov	r0, #30
	blx	r3
	ldr	r3, [r9]
	ldr	r3, [r3, #372]
	tst	r3, #4
	beq	.L222
	cmp	r4, #1000
	bge	.L209
	mov	r3, #0
	ldr	r2, .L223+12
	mov	r0, r3
	ldr	r1, .L223+24
	bl	dprint_vfmw
.L211:
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
.L209:
	mov	r3, #0
	ldr	r2, .L223+12
	mov	r0, r3
	ldr	r1, .L223+28
	bl	dprint_vfmw
	b	.L211
.L220:
	ldr	r1, .L223+32
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L224:
	.align	2
.L223:
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
	ldr	r3, .L231
	mov	r4, r0
	ldr	ip, [r3, #176]
	cmp	ip, #0
	beq	.L225
	sub	r2, fp, #24
	sub	r1, fp, #28
	sub	r0, fp, #32
	blx	ip
	cmp	r4, #0
	bne	.L227
	ldr	r3, .L231+4
	ldr	r0, [fp, #-32]
	ldr	r1, [fp, #-28]
	ldr	r3, [r3]
	ldr	r2, [fp, #-24]
	add	r3, r3, #61440
	str	r0, [r3, #524]
	str	r1, [r3, #772]
	str	r2, [r3, #776]
.L225:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L227:
	mov	r5, #1
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L231+8
	ldr	r1, .L231+12
	bl	dprint_vfmw
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L231+8
	ldr	r1, .L231+12
	bl	dprint_vfmw
	str	r5, [sp]
	mov	r3, r4
	ldr	r2, .L231+8
	ldr	r1, .L231+12
	mov	r0, #32
	bl	dprint_vfmw
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L232:
	.align	2
.L231:
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
	ldr	r3, .L234
	mov	r2, #8
	ldr	r3, [r3]
	add	r3, r3, #61440
	str	r2, [r3]
	ldmfd	sp, {fp, sp, pc}
.L235:
	.align	2
.L234:
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
	beq	.L252
	cmp	r4, #0
	bne	.L251
	ldr	r5, .L254
	ldr	r3, [r5]
	ldr	r6, [r3, #120]
	tst	r6, #1
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r7, .L254+4
	mov	r2, #2
	bfi	r6, r2, #8, #2
	str	r6, [r3, #120]
	orr	r6, r6, #16
	ldr	r3, [r7, #108]
	blx	r3
	ldr	r3, [r5]
	str	r6, [r3, #120]
	b	.L242
.L253:
	add	r4, r4, #1
	cmp	r4, #1000
	beq	.L243
.L242:
	ldr	r3, [r7, #112]
	mov	r0, #30
	blx	r3
	ldr	r3, [r5]
	ldr	r3, [r3, #372]
	tst	r3, #1
	beq	.L253
	cmp	r4, #1000
	bge	.L243
	mov	r3, #0
	ldr	r2, .L254+8
	mov	r0, r3
	ldr	r1, .L254+12
	bl	dprint_vfmw
.L245:
	ldr	r2, [r5]
	bfc	r6, #4, #1
	ldr	r3, [r7, #108]
	str	r6, [r2, #120]
	bfc	r6, #8, #2
	blx	r3
	ldr	r3, [r5]
	str	r6, [r3, #120]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L251:
	ldr	r1, .L254+16
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L243:
	mov	r3, #0
	ldr	r2, .L254+8
	mov	r0, r3
	ldr	r1, .L254+20
	bl	dprint_vfmw
	b	.L245
.L252:
	mov	r3, #0
	ldr	r2, .L254+8
	movt	r3, 63683
	ldr	r1, .L254+24
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L255:
	.align	2
.L254:
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
	beq	.L258
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDMHAL_V400R003_GlbResetX
.L258:
	mov	r3, #0
	ldr	r2, .L259
	movt	r3, 63683
	ldr	r1, .L259+4
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L260:
	.align	2
.L259:
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
	ldr	r5, .L280
	mov	r3, r0, asl #6
	sub	r3, r3, r0, asl #3
	ldr	r2, .L280+4
	add	r3, r5, r3
	ldr	r3, [r3, #8]
	ldr	r3, [r2, r3, asl #2]
	cmp	r3, #0
	beq	.L262
	cmp	r0, #0
	ldr	r4, [r3, #1224]
	bne	.L263
.L264:
	ldr	r6, .L280+8
	ldr	r3, [r6]
	cmp	r3, #0
	beq	.L278
.L265:
	cmp	r4, #1
	beq	.L279
.L276:
	mvn	r2, #0
	str	r2, [r3, #32]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L262:
	cmp	r0, #0
	moveq	r4, r0
	beq	.L264
.L263:
	ldr	r1, .L280+12
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L279:
	ldr	r2, [r5, #44]
	cmp	r2, #1
	beq	.L276
	cmp	r2, #2
	ldr	r1, [r3, #28]
	mvneq	r2, #11
	streq	r2, [r3, #32]
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	cmp	r2, #3
	mvneq	r2, #14
	streq	r2, [r3, #32]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L278:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r6]
	bne	.L265
.L266:
	ldr	r1, .L280+16
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L281:
	.align	2
.L280:
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
	bne	.L288
	ldr	r4, .L290
	ldr	r3, [r4]
	cmp	r3, #0
	beq	.L289
.L285:
	add	r3, r3, #61440
	mov	r2, #7
	str	r2, [r3, #44]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L288:
	ldr	r1, .L290+4
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L289:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r4]
	bne	.L285
.L286:
	ldr	r1, .L290+8
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L291:
	.align	2
.L290:
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
	bne	.L298
	ldr	r4, .L300
	ldr	r3, [r4]
	cmp	r3, #0
	beq	.L299
.L295:
	mvn	r2, #0
	str	r2, [r3, #36]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L298:
	ldr	r1, .L300+4
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L299:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r4]
	bne	.L295
.L296:
	ldr	r1, .L300+8
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L301:
	.align	2
.L300:
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
	ldr	r2, .L320
	mov	r3, r0, asl #6
	sub	r3, r3, r0, asl #3
	ldr	r1, .L320+4
	add	r3, r2, r3
	ldr	r3, [r3, #8]
	ldr	r3, [r1, r3, asl #2]
	cmp	r3, #0
	beq	.L306
	cmp	r0, #0
	ldr	r4, [r3, #1224]
	bne	.L307
.L308:
	ldr	r5, .L320+8
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L319
.L309:
	cmp	r4, #1
	mvneq	r2, #5
	mvnne	r2, #1
	str	r2, [r3, #36]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L306:
	cmp	r0, #0
	moveq	r4, r0
	beq	.L308
.L307:
	ldr	r1, .L320+12
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L319:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r5]
	bne	.L309
.L310:
	ldr	r1, .L320+16
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L321:
	.align	2
.L320:
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
	bgt	.L344
	bne	.L342
	ldr	r4, .L346
	ldr	r3, [r4]
	cmp	r3, #0
	beq	.L345
.L328:
	sub	r2, r5, #1
	cmp	r2, #6
	ldrls	pc, [pc, r2, asl #2]
	b	.L334
.L336:
	.word	.L339
	.word	.L337
	.word	.L338
	.word	.L340
	.word	.L330
	.word	.L332
	.word	.L333
.L340:
	mov	r3, #40
.L335:
	ldr	r2, [r4]
	ldr	r0, [r2, r3]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L339:
	mov	r3, #28
	b	.L335
.L338:
	mov	r3, #36
	b	.L335
.L333:
	movw	r3, #62272
	b	.L335
.L337:
	mov	r3, #32
	b	.L335
.L330:
	movw	r3, #61480
	b	.L335
.L332:
	movw	r3, #62304
	b	.L335
.L342:
	ldr	r2, .L346+4
	mov	r0, #0
	ldr	r1, .L346+8
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L334:
	mov	r3, r5
	ldr	r2, .L346+4
	ldr	r1, .L346+12
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L345:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r6, r0, #0
	strne	r6, [r4]
	bne	.L328
.L329:
	ldr	r2, .L346+4
	ldr	r1, .L346+16
	bl	dprint_vfmw
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L344:
	mov	r3, r1
	ldr	r2, .L346+4
	ldr	r1, .L346+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L347:
	.align	2
.L346:
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
	ble	.L351
	mov	r1, #1
	ldr	r2, .L352
	str	r1, [sp]
	mov	r0, #32
	ldr	r1, .L352+4
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L351:
	movw	r0, #1228
	ldr	r2, .L352+8
	mul	r3, r0, r3
	ldr	r3, [r2, r3]
	add	r3, r3, #61440
	ldr	r0, [r3, #32]
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L353:
	.align	2
.L352:
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
	ble	.L357
	mov	r1, #1
	ldr	r2, .L358
	str	r1, [sp]
	mov	r0, #32
	ldr	r1, .L358+4
	bl	dprint_vfmw
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L357:
	movw	r1, #1228
	ldr	r2, .L358+8
	mul	r3, r1, r3
	ldr	r3, [r2, r3]
	add	r3, r3, #61440
	str	r0, [r3, #32]
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L359:
	.align	2
.L358:
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
	beq	.L380
	cmp	r2, #0
	bgt	.L381
	cmp	r0, #6
	mov	r2, #0
	beq	.L382
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
	b	.L362
.L367:
	.word	.L376
	.word	.L368
	.word	.L369
	.word	.L370
	.word	.L362
	.word	.L371
	.word	.L365
	.word	.L362
	.word	.L372
	.word	.L373
	.word	.L374
	.word	.L374
	.word	.L374
	.word	.L375
	.word	.L362
	.word	.L376
	.word	.L377
	.word	.L378
.L382:
	mov	r0, r2
	mov	r1, #1
	bl	SCD_ConfigReg
	mov	r3, r6
	mov	r2, #0
	mov	r1, #1
	mov	r0, #8
	bl	SCD_ConfigReg
.L365:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	AVSHAL_V400R003_StartDec
.L376:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	H264HAL_V400R003_StartDec
.L377:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	HEVCHAL_V400R003_StartDec
.L378:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VP9HAL_V400R003_StartDec
.L368:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VC1HAL_V400R003_StartDec
.L369:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	MP4HAL_V400R003_StartDec
.L370:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	MP2HAL_V400R003_StartDec
.L371:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	DIVX3HAL_V400R003_StartDec
.L372:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	RV8HAL_V400R003_StartDec
.L373:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	RV9HAL_V400R003_StartDec
.L374:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VP6HAL_V400R003_StartDec
.L375:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VP8HAL_V400R003_StartDec
.L380:
	mov	r3, r7
	mov	r0, r7
	ldr	r2, .L383
	ldr	r1, .L383+4
	bl	dprint_vfmw
.L362:
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L381:
	mov	r3, r2
	mov	r0, #0
	ldr	r2, .L383
	str	r0, [sp]
	ldr	r1, .L383+8
	bl	dprint_vfmw
	b	.L362
.L384:
	.align	2
.L383:
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
	ldr	r3, .L392
	ldr	r3, [r3, r2]
	cmp	r3, #0
	beq	.L390
	cmp	r0, #0
	ble	.L391
	mov	r3, r0
	mov	r1, #1
	ldr	r2, .L392+4
	mov	r0, #32
	str	r1, [sp]
	ldr	r1, .L392+8
	bl	dprint_vfmw
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
	mov	r0, r3
	ldr	r2, .L392+4
	ldr	r3, .L392+12
	ldr	r1, .L392+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L387
.L393:
	.align	2
.L392:
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
	ldr	r3, .L401
	ldr	r4, [r3, r2]
	cmp	r4, #0
	beq	.L399
	cmp	r0, #0
	ble	.L400
	mov	r3, r0
	mov	r4, #1
	ldr	r2, .L401+4
	mov	r0, #32
	str	r4, [sp]
	ldr	r1, .L401+8
	bl	dprint_vfmw
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
	ldr	r1, .L401+12
	mov	r0, r4
	bl	dprint_vfmw
	mov	r0, r4
	b	.L396
.L402:
	.align	2
.L401:
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
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r3, r0, #0
	beq	.L451
	ldr	r2, [r3, #4]
	ldr	r4, [r3]
	cmp	r2, #0
	bne	.L441
	ldr	r3, .L455
	ldr	r2, [r3]
	cmp	r2, #1
	addne	r1, r3, #60
	beq	.L408
.L410:
	ldr	r2, [r3, #4]!
	cmp	r2, #1
	beq	.L408
	cmp	r3, r1
	bne	.L410
	ldr	r5, .L455+4
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L452
	ldr	r8, [ip, #12]
	mov	r7, #0
	ldr	r6, .L455+8
	ldr	r3, [ip, #28]
	str	r8, [r6]
	str	r3, [r6, #4]
.L414:
	add	r2, ip, #4096
	cmp	r7, #0
	ldr	r3, .L455+8
	ldr	r1, [r2]
	ldr	r2, [r2, #20]
	ldr	lr, [ip, #208]
	ldr	r5, [ip, #212]
	str	r1, [r6, #16]
	str	r2, [r6, #20]
	str	lr, [r6, #8]
	str	r5, [r6, #12]
	bne	.L417
	ldr	r10, [ip, #176]
	ldr	r9, [ip, #180]
	ldr	r0, [ip, #184]
	ldr	r1, [ip, #188]
	ldr	r2, [ip, #192]
	str	r10, [r3, #24]
	str	r9, [r3, #28]
	str	r0, [r3, #32]
	str	r1, [r3, #36]
	str	r2, [r3, #40]
.L417:
	ldr	r3, .L455+12
	add	r2, ip, #33024
	add	r0, r3, #128
.L418:
	ldr	r1, [r2], #4
	str	r1, [r3, #4]!
	cmp	r3, r0
	bne	.L418
	cmp	r7, #0
	bne	.L422
	add	ip, ip, #32768
	ldr	r2, [ip, #384]
	ldr	r3, [ip, #388]
	str	r2, [r6, #48]
	str	r3, [r6, #44]
.L422:
	and	r3, r8, #15
	cmp	r3, #5
	andeq	r5, r5, #1
	andne	lr, lr, #31
	moveq	r0, #0
	streq	r5, [r4]
	strne	lr, [r4]
	movne	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L408:
	ldr	r5, .L455+4
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L453
	ldr	r2, [r3, #12]
	mov	r7, #1
	ldr	r3, [r3, #28]
	ldr	r6, .L455+8
	stmia	r6, {r2, r3}
.L413:
	ldr	r0, [r5, #40]
	ubfx	r8, r3, #0, #17
	bl	MEM_Phy2Vir
	subs	r2, r0, #0
	beq	.L454
	sub	r3, r8, #1
	ldr	ip, [r5]
	cmp	r3, #199
	subls	r3, r8, #-1073741823
	ldr	r8, [r6]
	movhi	r3, #0
	movhi	r1, #8
	movls	r3, r3, asl #4
	ldr	lr, [ip, #208]
	addls	r1, r3, #8
	ldr	r5, [ip, #212]
	ldr	r3, [r2, r3]
	str	r3, [r6, #16]
	ldr	r3, [r2, r1]
	str	lr, [r6, #8]
	str	r5, [r6, #12]
	str	r3, [r6, #20]
	b	.L417
.L452:
	mov	r7, ip
.L423:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	ip, r0, #0
	beq	.L412
	ldr	r8, [ip, #12]
	cmp	r7, #1
	ldr	r6, .L455+8
	ldr	r3, [ip, #28]
	str	ip, [r5]
	str	r8, [r6]
	str	r3, [r6, #4]
	bne	.L414
	b	.L413
.L453:
	mov	r7, #1
	b	.L423
.L441:
	ldr	r1, .L455+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L451:
	ldr	r3, .L455+20
	ldr	r2, .L455+24
	ldr	r1, .L455+28
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L412:
	ldr	r1, .L455+32
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L454:
	ldr	r3, .L455+36
	ldr	r2, .L455+24
	ldr	r1, .L455+28
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L456:
	.align	2
.L455:
	.word	g_DSPState
	.word	g_HwMem
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
	beq	.L461
	cmp	r2, #800
	bhi	.L462
	ldr	r3, .L463
	mov	r2, r2, asl #2
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L462:
	mov	r0, r4
	ldr	r1, .L463+4
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L461:
	mov	r2, r0
	ldr	r1, .L463+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L464:
	.align	2
.L463:
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
	beq	.L466
	sub	r3, r2, #1
	cmp	r3, #255
	bhi	.L466
	ldr	r3, .L469
	mov	r2, r2, asl #2
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L466:
	ldr	r1, .L469+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L470:
	.align	2
.L469:
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
	str	r0, [fp, #-56]
	ldr	r0, [r1, #48]
	str	r2, [fp, #-76]
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	str	r3, [fp, #-72]
	beq	.L510
	ldr	r3, [fp, #-56]
	ldr	r2, [r3, #840]
	sub	r3, r2, #1
	cmp	r3, #199
	bhi	.L511
	ldr	r3, [fp, #-56]
	ldr	r1, [r3, #24]
	ldr	r2, [r3, #864]
	ldr	ip, [r3, #848]
	mov	r1, r1, asl r2
	ldr	r3, [r3, #28]
	add	r0, r1, #255
	cmp	ip, #1
	bic	r0, r0, #255
	mov	r2, r3, asl r2
	movne	r1, #0
	mov	lr, r0, asl #4
	beq	.L512
.L475:
	ldr	r3, [fp, #-56]
	ldr	r3, [r3]
	cmp	r3, #16
	beq	.L476
	add	r3, r2, #31
	mov	r2, #8
	bic	r3, r3, #31
	str	r2, [fp, #-92]
	mla	r3, r3, r0, r1
.L497:
	ldr	r4, [fp, #-56]
	ldr	r5, [fp, #-72]
	mov	ip, #0	@ movhi
	mov	r0, ip	@ movhi
	ldr	r2, [r4, #16]
	str	r2, [r5]
	ldr	r2, [r4, #16]
	add	r2, r3, r2
	str	r2, [r5, #4]
	ldr	r2, [r4, #4]
	str	r2, [r5, #8]
	ldr	r2, [r4, #4]
	add	r3, r3, r2
	str	r3, [r5, #12]
	str	lr, [r5, #16]
	str	r1, [r5, #20]
	ldr	r2, [r4, #24]
	ldr	r3, [r4, #28]
	sub	r2, r2, #1
	sub	r3, r3, #1
	bfi	ip, r2, #0, #9
	bfi	r0, r3, #0, #9
	strh	ip, [fp, #-48]	@ movhi
	strh	r0, [fp, #-46]	@ movhi
	ldr	r3, [fp, #-48]
	str	r3, [r5, #24]
	ldr	r3, [r4, #852]
	ldr	r2, [r4, #24]
	sub	r3, r3, #1
	cmp	r3, #1
	str	r2, [fp, #-88]
	movls	r3, r4
	ldrhi	r3, [fp, #-56]
	ldrls	r3, [r3, #28]
	ldrhi	r3, [r3, #28]
	addls	r3, r3, r3, lsr #31
	movls	r3, r3, asr #1
	str	r3, [fp, #-96]
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #864]
	cmp	r3, #6
	moveq	r4, #2
	beq	.L479
	cmp	r3, #5
	moveq	r4, #4
	beq	.L479
	cmp	r3, #4
	moveq	r4, #8
	beq	.L479
	ldr	r1, .L518
	mov	r0, #1
	bl	dprint_vfmw
	mov	r4, #1
.L479:
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #856]
	cmp	r3, #0
	bne	.L502
	ldr	r2, [fp, #-56]
	ldr	r7, [r2, #840]
	cmp	r7, #0
	strle	r3, [fp, #-64]
	ble	.L482
	ldr	r2, [fp, #-88]
	mov	r6, r3
	ldr	r1, [fp, #-96]
	sub	r8, r4, #1
	str	r3, [fp, #-64]
	str	r3, [fp, #-68]
	mul	r2, r2, r1
	sub	r3, r2, #1
	str	r3, [fp, #-84]
	str	r2, [fp, #-80]
	b	.L483
.L485:
	ldr	r3, [fp, #-56]
	add	r6, r6, #1
	ldr	r3, [r3, #840]
	cmp	r3, r6
	mov	r7, r3
	ble	.L513
	cmp	r6, #199
	bgt	.L514
.L483:
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
	ble	.L484
	ldrsh	r0, [r5, #38]
	mov	r1, r4
	sub	r0, r0, #1
	add	r0, r0, r4
	bl	__aeabi_uidiv
	mul	r3, r4, r0
	cmp	r10, r3
	str	r3, [fp, #-68]
	movgt	r10, r7
	bgt	.L486
	b	.L485
.L502:
	mov	r2, #0
	str	r2, [fp, #-64]
.L480:
	cmp	r3, #1
	beq	.L494
.L482:
	ldr	r3, [fp, #-64]
	sub	r3, r3, #1
	uxth	r3, r3
.L496:
	ldr	lr, [fp, #-56]
	mov	r0, #0
	ldrb	r2, [fp, #-46]	@ zero_extendqisi2
	strh	r3, [fp, #-48]	@ movhi
	ldr	r1, [lr, #848]
	ldr	ip, [lr, #852]
	ldr	lr, [lr, #864]
	bfi	r2, r1, #0, #1
	ldrb	r1, [fp, #-45]	@ zero_extendqisi2
	and	ip, ip, #3
	sub	lr, lr, #4
	bfi	r2, ip, #4, #2
	bfi	r1, lr, #0, #2
	mov	r3, r2
	bfi	r3, ip, #6, #2
	mov	r2, r1
	ldr	r1, [fp, #-92]
	strb	r3, [fp, #-46]
	bfi	r2, r1, #2, #4
	strb	r2, [fp, #-45]
	ldr	r3, [fp, #-48]
	ldr	r2, [fp, #-72]
	str	r3, [r2, #28]
.L508:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L489:
	bl	__aeabi_uidiv
	mov	r1, r4
	mul	r9, r4, r0
	ldrsh	r0, [r5, #44]
	bl	__aeabi_uidiv
	mul	r0, r4, r0
	cmp	r9, r0
	add	r2, r0, #1
	bhi	.L490
	cmp	r9, r2
	mov	r1, r4
	beq	.L490
	ldrsh	r0, [r5, #38]
	add	r0, r0, r8
	bl	__aeabi_uidiv
	mul	r0, r4, r0
	cmp	r9, r0
	bhi	.L515
.L490:
	cmp	r7, #198
	mov	r6, r7
	add	r5, r5, #4
	bgt	.L516
	ldrsh	r9, [r5, #42]
.L486:
	add	r7, r6, #1
	mov	r1, r4
	cmp	r7, r10
	add	r0, r8, r9
	blt	.L489
	bl	__aeabi_uidiv
	mul	r10, r4, r0
.L488:
	ldrsh	r0, [fp, #-60]
	mov	r1, r4
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-80]
	ldr	r7, [fp, #-84]
	cmp	r3, r10
	ldr	r3, [fp, #-64]
	movhi	r7, r10
	add	r3, r3, #1
	str	r3, [fp, #-64]
	mul	r5, r4, r0
	cmp	r7, r5
	movlt	r5, #0
	cmp	r3, #252
	bgt	.L517
	ldr	r3, [fp, #-76]
	cmp	r3, #0
	bne	.L485
	ldr	r9, [fp, #-88]
	mov	r0, r5
	mov	r1, r9
	bl	__aeabi_uidivmod
	ldr	r2, [fp, #-76]
	mov	r0, r5
	mov	ip, r2, lsr #16
	mov	r3, r2	@ movhi
	mov	r5, r2	@ movhi
	str	ip, [fp, #-60]
	mov	r10, ip
	bfi	r3, r1, #0, #9
	mov	r1, r9
	strh	r3, [fp, #-48]	@ movhi
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-64]
	mov	r1, r9
	ldr	ip, [fp, #-60]
	add	r3, r3, #5
	mov	r2, r3, asl #3
	str	r2, [fp, #-60]
	ldr	r2, [fp, #-72]
	bfi	ip, r0, #0, #9
	strh	ip, [fp, #-46]	@ movhi
	ldr	ip, [fp, #-48]
	mov	r0, r7
	str	ip, [r2, r3, asl #3]
	bl	__aeabi_uidivmod
	mov	r0, r7
	bfi	r5, r1, #0, #9
	mov	r1, r9
	strh	r5, [fp, #-48]	@ movhi
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-60]
	ldr	r1, [fp, #-72]
	add	r2, r2, #4
	bfi	r10, r0, #0, #9
	strh	r10, [fp, #-46]	@ movhi
	ldr	r3, [fp, #-48]
	str	r3, [r1, r2]
	b	.L485
.L484:
	ldr	r3, [fp, #-68]
	cmp	r10, r3
	movgt	r3, #0
	movle	r3, #1
	cmp	r6, #0
	moveq	r3, #0
	cmp	r3, #0
	bne	.L485
	b	.L488
.L516:
	ldr	r3, [fp, #-56]
	mov	r1, r4
	add	r2, r3, r7, lsl #2
	ldrsh	r0, [r2, #42]
	sub	r0, r0, #1
	add	r0, r0, r4
	bl	__aeabi_uidiv
	mul	r10, r4, r0
	b	.L488
.L515:
	mov	r10, r9
	b	.L488
.L476:
	mla	r3, r0, r2, r1
	mov	r2, #10
	str	r2, [fp, #-92]
	b	.L497
.L512:
	add	r3, r1, #508
	adds	ip, r2, #63
	add	r3, r3, #3
	add	r1, r1, #1020
	add	r1, r1, #2
	addmi	ip, r2, #126
	cmp	r3, #0
	movlt	r3, r1
	mov	r1, ip, asr #6
	mov	r3, r3, asr #9
	mov	r1, r1, asl #5
	mov	r3, r3, asl #4
	mul	r1, r3, r1
	b	.L475
.L517:
	ldr	r2, [fp, #-56]
	mov	r3, #1
	str	r3, [r2, #856]
.L494:
	ldr	r3, [fp, #-76]
	ldr	r2, [fp, #-56]
	cmp	r3, #0
	mov	r3, #1
	str	r3, [r2, #840]
	movne	r3, #0
	bne	.L496
	ldr	lr, [fp, #-76]
	ldr	r3, [fp, #-88]
	ldr	r2, [fp, #-96]
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
	ldr	r1, [fp, #-72]
	ldr	r2, [fp, #-48]
	str	ip, [r1, #48]
	str	r2, [r1, #52]
	b	.L496
.L513:
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #856]
	b	.L480
.L514:
	str	r6, [sp]
	movw	r3, #2505
	ldr	r2, .L518+4
	mov	r0, #0
	ldr	r1, .L518+8
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L511:
	ldr	r1, .L518+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L508
.L510:
	ldr	r3, .L518+16
	ldr	r2, .L518+4
	ldr	r1, .L518+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L508
.L519:
	.align	2
.L518:
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
	bne	.L555
	ldr	r3, [r1]
	cmp	r3, #0
	beq	.L556
.L524:
	ldr	r4, .L558
	ldr	r3, [r1, #48]
	ldrb	r2, [r4]	@ zero_extendqisi2
	bic	r3, r3, #15
	str	r3, [fp, #-24]
	cmp	r2, #1
	bne	.L557
	ldr	r2, .L558+4
	ldr	r1, [r2]
	str	r3, [r1, #16]
.L527:
	ldr	r1, [r2]
	movw	r3, #53763
	movt	r3, 8192
	str	r3, [r1, #12]
.L529:
	ldr	r1, [r2]
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r1, #60]
.L531:
	ldr	r1, [r2]
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r1, #64]
.L533:
	ldr	r1, [r2]
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r1, #68]
.L535:
	ldr	r1, [r2]
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r1, #72]
.L537:
	ldr	r1, [r2]
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r1, #76]
.L539:
	ldr	r1, [r2]
	movw	r3, #3075
	movt	r3, 48
	str	r3, [r1, #80]
.L541:
	ldr	r2, [r2]
	movw	r3, #3075
	mov	r0, #0
	movt	r3, 48
	str	r3, [r2, #84]
.L544:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L557:
	mov	r2, #16
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r2, [r4]	@ zero_extendqisi2
	movw	r3, #53763
	cmp	r2, #1
	movt	r3, 8192
	str	r3, [fp, #-24]
	ldreq	r2, .L558+4
	beq	.L527
	mov	r2, #12
	mov	r1, #2
	ldr	r0, [fp, #4]
	movw	r5, #3075
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	movt	r5, 48
	str	r5, [fp, #-24]
	cmp	r3, #1
	ldreq	r2, .L558+4
	beq	.L529
	mov	r3, r5
	mov	r2, #60
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r2, .L558+4
	beq	.L531
	mov	r3, r5
	mov	r2, #64
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r2, .L558+4
	beq	.L533
	mov	r3, r5
	mov	r2, #68
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r2, .L558+4
	beq	.L535
	mov	r3, r5
	mov	r2, #72
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r2, .L558+4
	beq	.L537
	mov	r3, r5
	mov	r2, #76
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r2, .L558+4
	beq	.L539
	mov	r3, r5
	mov	r2, #80
	mov	r1, #2
	ldr	r0, [fp, #4]
	bl	VDH_Record_RegData
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r2, .L558+4
	beq	.L541
	ldr	r0, [fp, #4]
	mov	r3, r5
	mov	r2, #84
	mov	r1, #2
	bl	VDH_Record_RegData
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L556:
	mov	r0, #0
	str	r1, [fp, #-32]
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L525
	ldr	r1, [fp, #-32]
	str	r3, [r1]
	b	.L524
.L555:
	ldr	r1, .L558+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L544
.L525:
	ldr	r1, .L558+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L544
.L559:
	.align	2
.L558:
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
	beq	.L589
	ldmia	r3, {r7, r8}
	ldr	r5, [r3, #8]
	cmp	r8, #0
	beq	.L590
	movw	r3, #1228
	ldr	r6, .L596
	mul	r3, r3, r5
	ldr	r0, [r6, r3]
	cmp	r0, #0
	beq	.L564
	ldr	r3, .L596+4
	ldr	r2, [r3]
	cmp	r2, #1
	addne	r1, r3, #60
	beq	.L579
.L567:
	ldr	r2, [r3, #4]!
	cmp	r2, #1
	beq	.L579
	cmp	r3, r1
	bne	.L567
	mov	r4, #0
.L565:
	ldr	r3, .L596+8
	mov	r2, #816
	mov	r1, #0
	mov	r0, r8
	ldr	r3, [r3, #48]
	blx	r3
	movw	r3, #1228
	mla	r3, r3, r5, r6
	ldr	r1, .L596+12
	mov	r2, #180
	mla	r2, r2, r5, r1
	ldr	r3, [r3, #24]
	str	r3, [r8, #4]
	ldr	r3, [r2, #4]
	mov	r3, r3, lsr #17
	and	r2, r3, #3
	cmp	r2, #1
	moveq	r3, #0
	beq	.L568
	eor	r3, r3, #1
	and	r3, r3, #1
	cmp	r7, #3
	orrne	r3, r3, #1
.L568:
	mov	r2, #180
	str	r3, [r8]
	mla	r3, r2, r5, r1
	cmp	r7, #0
	cmpne	r7, #15
	ldr	r2, [r3, #4]
	ubfx	r2, r2, #0, #17
	str	r2, [r8, #8]
	beq	.L591
.L569:
	cmp	r2, #200
	bhi	.L592
.L571:
	movw	r3, #1228
	mla	r6, r3, r5, r6
	ldr	r7, [r6, #40]
	mov	r0, r7
	bl	MEM_Phy2Vir
	subs	r1, r0, #0
	beq	.L593
	ldr	r6, .L596+16
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
	beq	.L576
.L575:
	ldr	r0, [r3, #4]
	add	r2, r2, #1
	cmp	r2, ip
	add	r3, r3, #16
	add	r1, r1, #4
	strh	r0, [r1, #8]	@ movhi
	ldr	r0, [r3, #-8]
	strh	r0, [r1, #10]	@ movhi
	bne	.L575
.L576:
	mov	r0, #6
	bl	IsDprintTypeEnable
	cmp	r0, #0
	bne	.L594
.L562:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L591:
	cmp	r4, #1
	beq	.L595
	ldr	r1, .L596+20
	ldrb	r1, [r1]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L569
	ldrb	r3, [r3, #19]	@ zero_extendqisi2
	and	r3, r3, #3
	cmp	r3, #2
	bne	.L569
	mov	r3, #1
	strh	r1, [r8, #12]	@ movhi
	mov	r0, r1
	strh	r1, [r8, #14]	@ movhi
	str	r3, [r8, #8]
	b	.L562
.L579:
	mov	r4, r2
	b	.L565
.L594:
	ldr	r2, [r8, #8]
	mov	r0, #6
	ldr	r1, .L596+24
	mov	r4, #0
	bl	dprint_vfmw
	mov	r2, r7
	ldr	r1, .L596+28
	mov	r0, #6
	bl	dprint_vfmw
	mov	r3, #3200
	mla	r5, r3, r5, r6
.L577:
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
	ldr	r1, .L596+32
	bl	dprint_vfmw
	ldr	r3, [r8, #8]
	mov	r3, r3, asl #2
	sub	r3, r3, #3
	cmp	r3, r4
	bhi	.L577
	ldr	r1, .L596+36
	mov	r0, #6
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L592:
	mov	r3, #200
	ldr	r1, .L596+40
	mov	r0, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r8, #8]
	b	.L571
.L595:
	ldr	r3, [r3, #16]
	ubfx	r3, r3, #21, #2
	cmp	r3, #2
	bne	.L569
	mov	r3, #0
	str	r4, [r8, #8]
	strh	r3, [r8, #12]	@ movhi
	mov	r0, r3
	strh	r3, [r8, #14]	@ movhi
	b	.L562
.L593:
	ldr	r3, .L596+44
	ldr	r2, .L596+48
	ldr	r1, .L596+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L562
.L564:
	ldr	r3, .L596+56
	ldr	r2, .L596+48
	ldr	r1, .L596+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L562
.L590:
	mov	r0, r8
	ldr	r3, .L596+60
	ldr	r2, .L596+48
	ldr	r1, .L596+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L562
.L589:
	ldr	r3, .L596+64
	ldr	r2, .L596+48
	ldr	r1, .L596+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L562
.L597:
	.align	2
.L596:
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
	ldr	r4, .L613
	cmp	r3, #0
	movw	r5, #1228
	mov	r6, r0
	mla	r5, r5, r3, r4
	bne	.L610
	ldr	r3, [r4]
	cmp	r3, #0
	beq	.L611
.L602:
	cmp	r2, #0
	bne	.L604
	ldr	r3, .L613+4
	ldr	r0, [r3, #840]
	cmp	r0, #0
	ble	.L605
	cmp	r6, #6
	bne	.L606
	ldrb	r2, [r1, #17]	@ zero_extendqisi2
	cmp	r2, #1
	movne	r2, #0
	str	r2, [r3, #1728]
.L606:
	ldr	r3, [fp, #4]
	mov	r2, r1
	mov	r0, r6
	mov	r1, r5
	str	r3, [sp]
	mov	r3, #0
	bl	VDMHAL_V400R003_CfgRpReg
	mov	r1, r5
	mov	r2, #0
	ldr	r0, .L613+4
	bl	VDMHAL_V400R003_CfgRpMsg
	mov	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L604:
	cmp	r2, #1
	movne	r0, #0
	beq	.L612
.L601:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L612:
	cmp	r6, #6
	bne	.L608
	ldrb	r3, [r1, #17]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L608
	ldr	r4, .L613+4
	ldr	r3, [r4, #1716]
	cmp	r3, #0
	ble	.L608
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
.L611:
	mov	r0, #0
	str	r2, [fp, #-36]
	movt	r0, 63683
	str	r1, [fp, #-32]
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L603
	str	r3, [r4]
	ldr	r1, [fp, #-32]
	ldr	r2, [fp, #-36]
	b	.L602
.L608:
	ldr	r1, .L613+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L601
.L610:
	ldr	r1, .L613+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L601
.L605:
	mov	r0, r2
	ldr	r1, .L613+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L601
.L603:
	ldr	r1, .L613+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L601
.L614:
	.align	2
.L613:
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
	ldr	r6, .L645
	mov	r5, r0
	mov	r4, r1
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L639
	cmp	r0, #0
	ble	.L640
	str	r3, [sp]
	mov	r3, r0
	ldr	r2, .L645+4
	mov	r0, #32
	ldr	r1, .L645+8
	bl	dprint_vfmw
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L641
.L623:
	mov	r2, #1
	mov	r3, r5
	str	r2, [sp]
	mov	r0, #32
	ldr	r2, .L645+4
	ldr	r1, .L645+8
	bl	dprint_vfmw
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L635
.L618:
	cmp	r4, #0
	beq	.L642
	mov	r3, r5, asl #6
	ldr	r2, .L645+12
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
.L640:
	movw	r3, #1228
	ldr	r4, .L645+16
	mul	r3, r3, r0
	mov	r2, #67108864
	ldr	r3, [r4, r3]
	str	r2, [r3, #8]
.L621:
	movw	r3, #1228
	ldr	r2, .L645+20
	mul	r3, r3, r5
	mvn	r1, #1
	ldr	r3, [r4, r3]
	str	r1, [r3, #36]
	ldr	r3, [r2, #108]
	blx	r3
.L630:
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
.L615:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L639:
	mov	r3, #67108864
	mov	r2, #8
	mov	r1, #2
	mov	r0, r4
	bl	VDH_Record_RegData
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L643
	cmp	r5, #0
	ldrle	r4, .L645+16
	bgt	.L623
	b	.L621
.L643:
	mvn	r3, #1
	mov	r2, #36
	mov	r1, #2
	mov	r0, r4
	bl	VDH_Record_RegData
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L618
	ldr	r3, .L645+20
	ldr	r3, [r3, #108]
	blx	r3
	cmp	r5, #0
	ble	.L644
.L627:
	mov	r4, #1
	mov	r3, r5
	str	r4, [sp]
	mov	r0, #32
	ldr	r2, .L645+24
	ldr	r1, .L645+8
	bl	dprint_vfmw
	mov	r3, r5
	str	r4, [sp]
	mov	r0, #32
	ldr	r2, .L645+24
	ldr	r1, .L645+8
	bl	dprint_vfmw
	str	r4, [sp]
	mov	r3, r5
	ldr	r2, .L645+24
	ldr	r1, .L645+8
	mov	r0, #32
	bl	dprint_vfmw
	b	.L615
.L641:
	mvn	r3, #1
	mov	r2, #36
	mov	r1, #2
	mov	r0, r4
	bl	VDH_Record_RegData
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L618
.L635:
	ldr	r3, .L645+20
	ldr	r3, [r3, #108]
	blx	r3
	b	.L627
.L642:
	mov	r3, r4
	mov	r0, r4
	ldr	r2, .L645+24
	ldr	r1, .L645+28
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	dprint_vfmw
.L644:
	ldr	r4, .L645+16
	b	.L630
.L646:
	.align	2
.L645:
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
	ldr	r9, .L672
	ldr	r4, [r9, #64]
	cmp	r4, #0
	blt	.L668
	ldr	r10, .L672+4
	add	r6, r9, #64
	add	r9, r9, #124
	mov	r7, #30
	mov	r8, #0
.L649:
	mov	r0, r4
	bl	VCTRL_IsChanActive
	cmp	r0, #0
	bne	.L671
	ldr	r2, .L672+8
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
	b	.L651
.L654:
	.word	.L663
	.word	.L663
	.word	.L663
	.word	.L663
	.word	.L651
	.word	.L663
	.word	.L663
	.word	.L651
	.word	.L663
	.word	.L663
	.word	.L663
	.word	.L663
	.word	.L663
	.word	.L663
	.word	.L651
	.word	.L664
	.word	.L663
	.word	.L663
.L663:
	mul	r4, r5, r4
	mla	r8, r7, r4, r8
.L651:
	cmp	r6, r9
	beq	.L670
	ldr	r4, [r6, #4]!
	cmp	r4, #0
	bge	.L649
.L670:
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
.L671:
	mov	r3, r4
	ldr	r2, .L672+12
	ldr	r1, .L672+16
	mov	r0, #1
	bl	dprint_vfmw
	b	.L651
.L664:
	mul	r4, r5, r4
	mul	r4, r7, r4
	add	r8, r8, r4, lsl #1
	b	.L651
.L668:
	mov	r0, #25
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L673:
	.align	2
.L672:
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
	ldr	r2, .L682
	sub	r0, r3, r0, asl #3
	cmp	r1, #3
	ldrls	pc, [pc, r1, asl #2]
	b	.L675
.L677:
	.word	.L675
	.word	.L678
	.word	.L679
	.word	.L680
.L675:
	add	r3, r2, r0
	mov	r1, #500
	mov	r0, r1
	str	r1, [r3, #16]
	ldmfd	sp, {fp, sp, pc}
.L679:
	add	r3, r2, r0
	mov	r1, #100
	mov	r0, r1
	str	r1, [r3, #16]
	ldmfd	sp, {fp, sp, pc}
.L678:
	add	r3, r2, r0
	mov	r1, #540
	mov	r0, r1
	str	r1, [r3, #16]
	ldmfd	sp, {fp, sp, pc}
.L680:
	add	r3, r2, r0
	mov	r1, #600
	mov	r0, r1
	str	r1, [r3, #16]
	ldmfd	sp, {fp, sp, pc}
.L683:
	.align	2
.L682:
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
	ble	.L685
	mov	r0, #0
	mov	r3, r5
	str	r0, [sp]
	ldr	r2, .L723
	ldr	r1, .L723+4
	bl	dprint_vfmw
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L685:
	ldr	r6, .L723+8
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L720
	ldr	r2, .L723+12
	ldr	r8, .L723+16
	ldr	r10, .L723+20
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
.L689:
	ldr	r2, [r2]
	ldr	r3, [r2, #120]
	bfc	r3, #12, #5
	bfc	r3, #17, #1
	str	r3, [r2, #120]
	ldr	r3, [r2, #120]
	orr	r3, r3, #131072
	str	r3, [r2, #120]
.L691:
	ldr	r1, [r10, r7, asl #2]
	mov	r2, r5
	mov	r3, r4
	mov	r0, #12
	ldr	r10, .L723+8
	ldr	r1, [r1, #600]
	cmp	r1, #16
	movne	r1, #3
	moveq	r1, #0
	bl	SCD_ConfigReg
	ldrb	r2, [r6]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L721
	movw	r2, #1228
	ldr	r3, .L723+24
	mul	r2, r2, r5
	ldr	r4, .L723+28
	ldr	r1, [r3]
	movw	r3, #43690
	bfi	r3, r3, #16, #16
	cmp	r1, #1
	ldr	r2, [r4, r2]
	moveq	r1, #7
	movne	r1, #0
	str	r3, [r2, #156]
.L694:
	movw	r2, #1228
	cmp	r9, #1
	mul	r2, r2, r5
	mvneq	r3, #5
	mvnne	r3, #1
	ldr	r2, [r4, r2]
	add	r2, r2, #61440
	str	r1, [r2, #32]
.L696:
	movw	r2, #1228
	mul	r2, r2, r5
	ldr	r2, [r4, r2]
	str	r3, [r2, #36]
.L698:
	ldr	r6, .L723+32
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
	ldr	ip, .L723+36
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
.L721:
	movw	r3, #43690
	mov	r1, #2
	mov	r2, #156
	movt	r3, 43690
	mov	r0, r4
	bl	VDH_Record_RegData
	ldr	r3, .L723+24
	ldrb	r2, [r10]	@ zero_extendqisi2
	ldr	r3, [r3]
	cmp	r3, #1
	moveq	r1, #7
	movne	r1, #0
	cmp	r2, #1
	ldreq	r4, .L723+28
	beq	.L694
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
	ldreq	r4, .L723+28
	beq	.L696
	mov	r0, r4
	mov	r2, #36
	mov	r1, #2
	bl	VDH_Record_RegData
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r4, .L723+28
	beq	.L698
	cmp	r4, #0
	beq	.L722
	mov	r3, #1
	str	r7, [r4, #8]
	strb	r5, [r4]
	mov	r0, r4
	str	r3, [r4, #4]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	VDH_PostTask
.L720:
	mov	r3, #0
	mov	r0, r1
	mov	r2, r3
	mov	r1, r3
	bl	VDH_Record_RegData
	ldr	r8, .L723+16
	mov	r3, r5, asl #6
	ldr	r10, .L723+20
	sub	r3, r3, r5, asl #3
	ldrb	r1, [r6]	@ zero_extendqisi2
	add	r3, r8, r3
	mov	r2, #500
	cmp	r1, #1
	ldr	r7, [r3, #8]
	str	r2, [r3, #16]
	ldreq	r2, .L723+12
	ldr	r3, [r10, r7, asl #2]
	ldr	r9, [r3, #1224]
	beq	.L689
	mov	r3, #0
	mov	r2, #1
	mov	r1, r3
	mov	r0, r4
	bl	VDH_Record_RegData
	b	.L691
.L722:
	mov	r3, r4
	mov	r0, r4
	ldr	r2, .L723
	ldr	r1, .L723+40
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L724:
	.align	2
.L723:
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
	ldr	r1, .L726
	mov	r0, #15
	ldr	r3, .L726+4
	mov	r2, #4
	str	r0, [r1]
	str	r2, [r3]
	ldmfd	sp, {fp, sp, pc}
.L727:
	.align	2
.L726:
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
	beq	.L729
	cmp	r3, #2
	moveq	r4, #1
	movne	r4, #2
.L729:
	ldr	r3, [fp, #-108]
	cmp	r3, #0
	beq	.L728
	mov	r3, #0
	mov	r2, #4194304
	mov	r1, r3
	str	r3, [sp]
	ldr	r0, .L819
	sub	r3, fp, #92
	bl	MEM_AllocMemBlock
	subs	r1, r0, #0
	beq	.L731
.L733:
	ldr	r1, .L819+4
	mov	r0, #1
	bl	dprint_vfmw
.L728:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L731:
	str	r1, [sp]
	mov	r2, #4194304
	sub	r3, fp, #68
	ldr	r0, .L819+8
	bl	MEM_AllocMemBlock
	cmp	r0, #0
	bne	.L733
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
	bne	.L734
	ldr	r3, [fp, #-100]
	cmp	r3, #0
	movne	r8, r0
	ldrne	r10, .L819+12
	strne	r8, [fp, #-96]
	beq	.L816
.L735:
	cmp	r5, #0
	beq	.L739
	ldr	r2, [fp, #-96]
	mov	r4, #0
	and	r9, r2, #15
	mov	r3, r2, lsr #4
	ldr	r2, [fp, #-104]
	mul	r3, r2, r3
	add	r9, r3, r9, lsl #8
.L737:
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
	bhi	.L737
.L739:
	ldr	r3, [fp, #-96]
	add	r8, r8, r5
	ldr	r2, [fp, #-100]
	add	r3, r3, #1
	str	r3, [fp, #-96]
	cmp	r2, r3
	bne	.L735
	ldr	r4, [fp, #-100]
	mov	r0, r6
	ldr	ip, [r10, #44]
	ldr	r2, [fp, #-108]
	mul	r1, r4, r5
	blx	ip
	movs	r3, r4, lsr #1
	str	r3, [fp, #-100]
	beq	.L770
	ldr	r3, [fp, #-104]
	mov	r7, #0
	ldr	r8, [fp, #-128]
	str	r7, [fp, #-96]
	mov	r3, r3, asr #1
	str	r3, [fp, #-104]
.L741:
	cmp	r5, #0
	beq	.L743
	ldr	r2, [fp, #-96]
	mov	r4, #0
	and	r9, r2, #7
	mov	r3, r2, lsr #3
	ldr	r2, [fp, #-104]
	mul	r3, r2, r3
	add	r9, r3, r9, lsl #8
.L742:
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
	bhi	.L742
.L743:
	ldr	r3, [fp, #-96]
	add	r7, r7, r5
	ldr	r2, [fp, #-100]
	add	r3, r3, #1
	str	r3, [fp, #-96]
	cmp	r3, r2
	bne	.L741
.L770:
	ldr	r3, [fp, #4]
	cmp	r3, #1
	beq	.L817
	ldr	r3, [fp, #4]
	cmp	r3, #0
	beq	.L768
.L763:
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
.L734:
	subs	r3, r4, #1
	ldr	r2, [fp, #-100]
	ldr	r10, .L819+12
	movne	r3, #1
	cmp	r2, r3
	str	r3, [fp, #-132]
	ldrhi	r3, [fp, #-132]
	strhi	r3, [fp, #-96]
	bls	.L753
.L755:
	cmp	r5, #0
	beq	.L756
	ldr	r2, [fp, #-96]
	mov	r4, #0
	and	r8, r2, #15
	mov	r3, r2, lsr #4
	mov	r9, r2, lsr #1
	ldr	r2, [fp, #-104]
	mul	r9, r5, r9
	mul	r3, r2, r3
	add	r8, r3, r8, lsl #8
.L754:
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
	bhi	.L754
.L756:
	ldr	r3, [fp, #-96]
	ldr	r2, [fp, #-100]
	add	r3, r3, #2
	str	r3, [fp, #-96]
	cmp	r2, r3
	bhi	.L755
.L753:
	ldr	r4, [fp, #-100]
	mov	r0, r6
	ldr	ip, [r10, #44]
	ldr	r2, [fp, #-108]
	mul	r1, r4, r5
	mov	r1, r1, lsr #1
	blx	ip
	movs	r3, r4, lsr #1
	str	r3, [fp, #-100]
	beq	.L757
	ldr	r3, [fp, #-104]
	mov	r7, #0
	ldr	r8, [fp, #-128]
	str	r7, [fp, #-96]
	mov	r3, r3, asr #1
	str	r3, [fp, #-104]
.L758:
	cmp	r5, #0
	beq	.L760
	ldr	r2, [fp, #-96]
	mov	r4, #0
	and	r9, r2, #7
	mov	r3, r2, lsr #3
	ldr	r2, [fp, #-104]
	mul	r3, r2, r3
	add	r9, r3, r9, lsl #8
.L759:
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
	bhi	.L759
.L760:
	ldr	r3, [fp, #-96]
	add	r7, r7, r5
	ldr	r2, [fp, #-100]
	add	r3, r3, #1
	str	r3, [fp, #-96]
	cmp	r3, r2
	bne	.L758
.L757:
	ldr	r3, [fp, #4]
	cmp	r3, #1
	beq	.L818
	ldr	r3, [fp, #4]
	cmp	r3, #0
	bne	.L763
	ldr	r2, [fp, #-116]
	ldr	r3, [fp, #-132]
	cmp	r2, r3
	bls	.L768
.L769:
	add	r3, r3, #2
	cmp	r2, r3
	bhi	.L769
.L768:
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
	b	.L763
.L818:
	ldr	r3, [fp, #-116]
	cmp	r3, #0
	beq	.L763
	ldr	r2, [fp, #-132]
	mov	r4, #0
	ldr	r0, [fp, #-120]
	mov	lr, r3
	ldr	ip, [fp, #-112]
	mla	r6, r5, r2, r6
	mov	r5, r5, asl #1
	add	r6, r6, #1
.L764:
	cmp	r0, #0
	beq	.L767
	mov	r3, r4, lsr #1
	mov	r2, r6
	mul	r3, r0, r3
	sub	r1, r3, #-67108863
	add	r8, r3, r0
	sub	r1, r1, #65011712
	add	r8, ip, r8
	add	r1, ip, r1
	add	r3, ip, r3
.L765:
	ldrb	r7, [r2, #-1]	@ zero_extendqisi2
	strb	r7, [r1, #1]!
	ldrb	r7, [r2], #2	@ zero_extendqisi2
	strb	r7, [r3], #1
	cmp	r3, r8
	bne	.L765
.L767:
	add	r4, r4, #2
	add	r6, r6, r5
	cmp	lr, r4
	bhi	.L764
	b	.L763
.L817:
	ldr	r3, [fp, #-116]
	cmp	r3, #0
	beq	.L763
	ldr	r2, [fp, #-112]
	add	r6, r6, #1
	ldr	lr, [fp, #-120]
	mov	r7, r3
	mov	r4, #0
	mov	r3, r2
.L747:
	cmp	lr, #0
	moveq	ip, r3
	beq	.L750
	sub	r1, r3, #-67108863
	add	ip, r3, lr
	sub	r1, r1, #65011712
	mov	r2, r6
.L748:
	ldrb	r0, [r2, #-1]	@ zero_extendqisi2
	strb	r0, [r1, #1]!
	ldrb	r0, [r2], #2	@ zero_extendqisi2
	strb	r0, [r3], #1
	cmp	ip, r3
	bne	.L748
.L750:
	add	r4, r4, #1
	mov	r3, ip
	cmp	r4, r7
	add	r6, r6, r5
	bne	.L747
	b	.L763
.L816:
	ldr	r10, .L819+12
	mov	r0, r6
	ldr	r1, [fp, #-100]
	ldr	r2, [fp, #-108]
	ldr	ip, [r10, #44]
	blx	ip
	b	.L770
.L820:
	.align	2
.L819:
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
	ldr	ip, .L828
	mov	lr, r2
	ldrb	ip, [ip]	@ zero_extendqisi2
	cmp	ip, #1
	bne	.L827
	cmp	r0, #0
	bne	.L823
	ldr	r3, .L828+4
	ldr	r3, [r3]
	ldr	r2, [r3, #120]
	ubfx	r0, r2, #8, #2
	cmp	r1, r0
	bfine	r2, r1, #8, #2
	strne	r2, [r3, #120]
	ldmfd	sp, {fp, sp, pc}
.L823:
	cmp	r0, #1
	ldmnefd	sp, {fp, sp, pc}
	ldr	r3, .L828+4
	ldr	r3, [r3]
	ldr	r2, [r3, #120]
	bfi	r2, r1, #12, #5
	bfc	r2, #17, #1
	str	r2, [r3, #120]
	ldr	r2, [r3, #120]
	orr	r2, r2, #131072
	str	r2, [r3, #120]
	ldmfd	sp, {fp, sp, pc}
.L827:
	mov	r3, r1
	mov	r2, r0
	mov	r1, #0
	mov	r0, lr
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDH_Record_RegData
.L829:
	.align	2
.L828:
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
	ldr	ip, .L836
	mov	lr, r3
	ldrb	ip, [ip]	@ zero_extendqisi2
	cmp	ip, #1
	bne	.L834
	cmp	r2, #0
	ble	.L835
	mov	r3, r2
	str	ip, [sp]
	ldr	r2, .L836+4
	mov	r0, #32
	ldr	r1, .L836+8
	bl	dprint_vfmw
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L835:
	movw	r3, #1228
	ldr	ip, .L836+12
	mul	r2, r3, r2
	ldr	r3, [ip, r2]
	str	r1, [r3, r0]
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L834:
	mov	r3, r1
	mov	r2, r0
	mov	r1, #2
	mov	r0, lr
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDH_Record_RegData
.L837:
	.align	2
.L836:
	.word	g_HalDisable
	.word	.LANCHOR0+528
	.word	.LC30
	.word	g_HwMem
	UNWIND(.fnend)
	.size	MFDE_ConfigReg, .-MFDE_ConfigReg
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.57008, %object
	.size	__func__.57008, 24
__func__.57008:
	.ascii	"VDMHAL_V400R003_OpenHAL\000"
	.type	__func__.57045, %object
	.size	__func__.57045, 27
__func__.57045:
	.ascii	"VDMHAL_V400R003_CalcFsSize\000"
	.space	1
	.type	__func__.57112, %object
	.size	__func__.57112, 27
__func__.57112:
	.ascii	"VDMHAL_V400R003_ArrangeMem\000"
	.space	1
	.type	__FUNCTION__.57113, %object
	.size	__FUNCTION__.57113, 27
__FUNCTION__.57113:
	.ascii	"VDMHAL_V400R003_ArrangeMem\000"
	.space	1
	.type	__func__.57057, %object
	.size	__func__.57057, 35
__func__.57057:
	.ascii	"VDMHAL_V400R003_FillMemArrangeInfo\000"
	.space	1
	.type	__func__.57142, %object
	.size	__func__.57142, 25
__func__.57142:
	.ascii	"VDMHAL_V400R003_ResetVdm\000"
	.space	3
	.type	__func__.57155, %object
	.size	__func__.57155, 37
__func__.57155:
	.ascii	"VDMHAL_V400R003_SetSmmuPageTableAddr\000"
	.space	3
	.type	__func__.57176, %object
	.size	__func__.57176, 26
__func__.57176:
	.ascii	"VDMHAL_V400R003_GlbResetX\000"
	.space	2
	.type	__func__.57165, %object
	.size	__func__.57165, 25
__func__.57165:
	.ascii	"VDMHAL_V400R003_GlbReset\000"
	.space	3
	.type	__func__.57237, %object
	.size	__func__.57237, 25
__func__.57237:
	.ascii	"VDMHAL_V400R003_CheckReg\000"
	.space	3
	.type	__func__.57254, %object
	.size	__func__.57254, 28
__func__.57254:
	.ascii	"VDMHAL_V400R003_ReadMMUMask\000"
	.type	__func__.57259, %object
	.size	__func__.57259, 29
__func__.57259:
	.ascii	"VDMHAL_V400R003_WriteMMUMask\000"
	.space	3
	.type	__func__.57266, %object
	.size	__func__.57266, 27
__func__.57266:
	.ascii	"VDMHAL_V400R003_PrepareDec\000"
	.space	1
	.type	__func__.57288, %object
	.size	__func__.57288, 27
__func__.57288:
	.ascii	"VDMHAL_V400R003_IsVdmReady\000"
	.space	1
	.type	__func__.57293, %object
	.size	__func__.57293, 25
__func__.57293:
	.ascii	"VDMHAL_V400R003_IsVdmRun\000"
	.space	3
	.type	__func__.57307, %object
	.size	__func__.57307, 27
__func__.57307:
	.ascii	"VDMHAL_V400R003_BackupInfo\000"
	.space	1
	.type	__func__.57353, %object
	.size	__func__.57353, 25
__func__.57353:
	.ascii	"VDMHAL_V400R003_CfgRpMsg\000"
	.space	3
	.type	__func__.57387, %object
	.size	__func__.57387, 30
__func__.57387:
	.ascii	"VDMHAL_V400R003_MakeDecReport\000"
	.space	2
	.type	__func__.57558, %object
	.size	__func__.57558, 15
__func__.57558:
	.ascii	"MFDE_ConfigReg\000"
	.space	1
	.type	__func__.57421, %object
	.size	__func__.57421, 30
__func__.57421:
	.ascii	"VDMHAL_V400R003_StartHwRepair\000"
	.space	2
	.type	__func__.57437, %object
	.size	__func__.57437, 30
__func__.57437:
	.ascii	"VDMHAL_V400R003_CalVdhClkSkip\000"
	.space	2
	.type	__func__.57476, %object
	.size	__func__.57476, 30
__func__.57476:
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
	ASCII(.ascii	"VDMHAL ArrangeMem HEVC 10 bit\012\000" )
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
