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
	.file	"vdm_hal_hevc.c"
	.text
	.align	2
	.global	HEVCWriteCabacTab
	.type	HEVCWriteCabacTab, %function
HEVCWriteCabacTab:
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
	ldr	r6, .L19
	mov	r9, #0
	ldr	r10, .L19+4
	mov	r1, r9
	mov	r8, r9
	mov	r7, r9
	mov	r5, #16
.L2:
	add	lr, r5, r9
	mov	r4, r1, asl #1
	rsb	r5, r1, r5
	add	ip, r6, r4
	add	lr, r10, lr, lsl #2
.L6:
	ldr	r3, [lr], #4
	add	r1, r1, #1
	add	ip, ip, #2
	cmn	r3, #1
	and	r2, r3, #15
	mov	r3, r3, asr #4
	streqb	r7, [r6, r4]
	mov	r2, r2, asl #3
	streqb	r7, [ip, #-1]
	addne	r3, r3, r3, lsl #2
	subne	r2, r2, #16
	subne	r3, r3, #45
	strneb	r3, [r6, r4]
	add	r3, r5, r1
	strneb	r2, [ip, #-1]
	cmp	r3, #159
	mov	r4, r1, asl #1
	ble	.L6
	add	r8, r8, #1
	cmp	r8, #3
	beq	.L17
	cmp	r8, #0
	add	r9, r9, #160
	moveq	r5, #16
	movne	r5, #0
	b	.L2
.L17:
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	addne	r3, r3, #2
	ldrne	r2, .L19+8
	addne	r0, r2, #928
	beq	.L18
.L10:
	ldrb	r1, [r2, #-1]	@ zero_extendqisi2
	add	r3, r3, #2
	strb	r1, [r3, #-3]
	ldrb	r1, [r2], #2	@ zero_extendqisi2
	cmp	r2, r0
	strb	r1, [r3, #-4]
	bne	.L10
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L18:
	ldr	r3, .L19+12
	ldr	r2, .L19+16
	ldr	r1, .L19+20
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L20:
	.align	2
.L19:
	.word	.LANCHOR0
	.word	.LANCHOR1+20
	.word	.LANCHOR0+1
	.word	.LC0
	.word	.LANCHOR1
	.word	.LC1
	UNWIND(.fnend)
	.size	HEVCWriteCabacTab, .-HEVCWriteCabacTab
	.align	2
	.global	HEVCHAL_V400R003_InitHal
	.type	HEVCHAL_V400R003_InitHal, %function
HEVCHAL_V400R003_InitHal:
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
	ldr	r0, [r0, #1108]
	bl	HEVCWriteCabacTab
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r1, .L26
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L27:
	.align	2
.L26:
	.word	.LC2
	UNWIND(.fnend)
	.size	HEVCHAL_V400R003_InitHal, .-HEVCHAL_V400R003_InitHal
	.align	2
	.global	HEVCGet_V400R003_VirAddr
	.type	HEVCGet_V400R003_VirAddr, %function
HEVCGet_V400R003_VirAddr:
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
	.size	HEVCGet_V400R003_VirAddr, .-HEVCGet_V400R003_VirAddr
	.align	2
	.global	HEVCGet_V400R003_PhyAddr
	.type	HEVCGet_V400R003_PhyAddr, %function
HEVCGet_V400R003_PhyAddr:
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
	.size	HEVCGet_V400R003_PhyAddr, .-HEVCGet_V400R003_PhyAddr
	.align	2
	.global	HEVCHAL_V400R003_CutSliceChain
	.type	HEVCHAL_V400R003_CutSliceChain, %function
HEVCHAL_V400R003_CutSliceChain:
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
	beq	.L33
	mov	r0, #0
	str	r0, [r3, #252]
	ldmfd	sp, {fp, sp, pc}
.L33:
	ldr	r3, .L34
	ldr	r2, .L34+4
	ldr	r1, .L34+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L35:
	.align	2
.L34:
	.word	.LC3
	.word	.LANCHOR1+1940
	.word	.LC1
	UNWIND(.fnend)
	.size	HEVCHAL_V400R003_CutSliceChain, .-HEVCHAL_V400R003_CutSliceChain
	.align	2
	.global	HEVC_WriteQmatrix
	.type	HEVC_WriteQmatrix, %function
HEVC_WriteQmatrix:
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
	subs	r9, r1, #0
	mov	r8, r0
	mov	r10, r2
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	sub	r6, r3, #4
	sub	r7, r0, #4
	mov	r4, #0
.L38:
	ldr	r3, [r7, #4]!
	mov	r2, r4
	ldr	r1, .L41
	mov	r0, #4
	add	r4, r4, #1
	mov	r5, r7
	str	r3, [r6, #4]!
	ldr	r3, [r5], r10
	bl	dprint_vfmw
	ldr	r3, [r6]
	rsb	r2, r8, r5
	ldr	r1, .L41+4
	mov	r0, #4
	bl	dprint_vfmw
	cmp	r4, r9
	bne	.L38
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L42:
	.align	2
.L41:
	.word	.LC4
	.word	.LC5
	UNWIND(.fnend)
	.size	HEVC_WriteQmatrix, .-HEVC_WriteQmatrix
	.align	2
	.global	HEVC_WriteTileInfo
	.type	HEVC_WriteTileInfo, %function
HEVC_WriteTileInfo:
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
	str	r0, [fp, #-52]
	mov	r4, r0
	ldr	r0, [r1, #1112]
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	str	r3, [fp, #-48]
	beq	.L44
	add	r7, r3, #1024
	add	r8, r3, #4
	add	r7, r7, #4
	mov	r5, #0
	mov	r9, r4
	str	r4, [fp, #-56]
.L45:
	mov	r10, r9
	mov	r6, r8
	mov	r4, #0
	b	.L49
.L65:
	ldrb	r3, [r10, #1818]	@ zero_extendqisi2
	ldrb	ip, [r10, #1819]	@ zero_extendqisi2
	ldrb	r1, [r10, #1816]	@ zero_extendqisi2
	ldrb	r2, [r10, #1817]	@ zero_extendqisi2
	mov	r3, r3, asl #16
	orr	r3, r3, ip, asl #24
	orr	r3, r3, r1
	ldr	r1, .L66
	orr	r2, r3, r2, asl #8
	str	r2, [r6, #-4]
	bl	dprint_vfmw
.L48:
	add	r4, r4, #4
	add	r6, r6, #4
	cmp	r4, #512
	add	r10, r10, #4
	beq	.L64
.L49:
	cmp	r4, #252
	mov	r2, #0
	ldr	r1, .L66
	mov	r0, #4
	ble	.L65
	str	r5, [r6, #-4]
	bl	dprint_vfmw
	b	.L48
.L64:
	add	r8, r8, #512
	add	r9, r9, #256
	cmp	r8, r7
	bne	.L45
	ldr	r6, [fp, #-56]
	mov	r3, #0
	ldr	r2, [fp, #-52]
	mov	ip, r3
.L50:
	cmp	r3, #9
	add	r3, r3, #1
	strgt	ip, [r7, #-4]
	add	r2, r2, #8
	ldrle	r0, [r2, #2324]
	add	r7, r7, #4
	ldrle	r1, [r2, #2320]
	orrle	r1, r1, r0, asl #16
	strle	r1, [r7, #-8]
	cmp	r3, #20
	bne	.L50
	ldr	r3, [fp, #-48]
	add	r2, r3, #1104
	mov	r3, #0
	add	r2, r2, #4
	mov	ip, r3
.L56:
	cmp	r3, #10
	add	r3, r3, #1
	strgt	ip, [r2, #-4]
	add	r6, r6, #8
	ldrle	r0, [r6, #2404]
	add	r2, r2, #4
	ldrle	r1, [r6, #2400]
	orrle	r1, r1, r0, asl #16
	strle	r1, [r2, #-8]
	cmp	r3, #22
	bne	.L56
	mov	r0, #0
.L61:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L44:
	ldr	r3, .L66+4
	ldr	r2, .L66+8
	ldr	r1, .L66+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L61
.L67:
	.align	2
.L66:
	.word	.LC7
	.word	.LC6
	.word	.LANCHOR1+1972
	.word	.LC1
	UNWIND(.fnend)
	.size	HEVC_WriteTileInfo, .-HEVC_WriteTileInfo
	.align	2
	.global	HEVCHAL_V400R003_SetPicMsg
	.type	HEVCHAL_V400R003_SetPicMsg, %function
HEVCHAL_V400R003_SetPicMsg:
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
	mov	r4, r0
	mov	r0, r2
	mov	r6, r2
	mov	r8, r1
	str	r3, [fp, #-48]
	bl	MEM_Phy2Vir
	subs	r5, r0, #0
	beq	.L83
	ldr	r3, .L85
	mov	r2, #1280
	mov	r1, #0
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r1, .L85+4
	mov	r0, #4
	bl	dprint_vfmw
	ldrb	r1, [r4]	@ zero_extendqisi2
	ldrb	ip, [r4, #1]	@ zero_extendqisi2
	mov	r2, r6
	ldrb	r7, [r4, #2]	@ zero_extendqisi2
	mov	r0, #4
	ldr	r3, [r4, #16]
	mov	r1, r1, asl #25
	ldr	lr, [r4, #8]
	orr	r1, r1, ip, asl #24
	ldrb	ip, [r4, #3]	@ zero_extendqisi2
	orr	r1, r1, r7, asl #23
	ldrb	r10, [r4, #4]	@ zero_extendqisi2
	mov	r3, r3, asl #9
	ldr	r9, [r4, #20]
	orr	r3, r3, lr, asl #19
	ldrb	r7, [r4, #12]	@ zero_extendqisi2
	orr	r1, r1, ip, asl #22
	orr	lr, r3, r9
	orr	ip, r1, r10, asl #21
	ldr	r1, .L85+8
	orr	r3, lr, r7, asl #18
	orr	r3, r3, ip
	str	r3, [r5]
	ldr	r3, [r5]
	bl	dprint_vfmw
	ldr	r3, [r4, #24]
	str	r3, [r5, #4]
	ldr	r2, [r4, #36]
	ldr	r1, [r4, #28]
	ldr	ip, [r4, #68]
	ldr	r3, [r4, #40]
	mov	r2, r2, asl #24
	ldr	r0, [r4, #44]
	orr	r2, r2, r1, asl #29
	ldr	lr, [r4, #48]
	orr	r2, r2, ip
	ldr	r1, [r4, #52]
	orr	r3, r2, r3, asl #21
	ldr	ip, [r4, #56]
	orr	r3, r3, r0, asl #18
	ldr	r2, [r4, #60]
	orr	r0, r3, lr, asl #15
	orr	r1, r0, r1, asl #12
	ldr	r0, [r4, #64]
	ldrb	r3, [r4, #32]	@ zero_extendqisi2
	orr	r1, r1, ip, asl #9
	orr	r2, r1, r2, asl #6
	ldrb	r1, [r4, #33]	@ zero_extendqisi2
	orr	r2, r2, r0, asl #3
	orr	r3, r2, r3, asl #28
	orr	r3, r3, r1, asl #27
	str	r3, [r5, #8]
	ldr	r0, [r4, #76]
	ldr	r2, [r4, #72]
	ldr	r1, [r4, #92]
	ldr	ip, [r4, #80]
	mov	r0, r0, asl #16
	ldr	r3, [r4, #84]
	orr	r2, r0, r2, asl #22
	orr	r2, r2, r1
	ldr	r1, [r4, #88]
	orr	r2, r2, ip, asl #12
	orr	r3, r2, r3, asl #8
	orr	r3, r3, r1, asl #4
	str	r3, [r5, #12]
	ldr	r3, [r8, #1092]
	str	r3, [r5, #16]
	ldr	r3, [r8, #1096]
	str	r3, [r5, #20]
	ldr	r3, [r4, #1284]
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #1468]
	str	r3, [r5, #24]
	ldr	r3, [r8, #1104]
	str	r3, [r5, #28]
	ldr	r2, [r4, #2560]
	cmp	r2, #0
	beq	.L71
	add	r0, r4, #2480
	add	ip, r5, #32
	add	r0, r0, #12
	mov	r1, #0
.L72:
	ldr	r3, [r0, #4]!
	add	r1, r1, #1
	add	r3, r3, #332
	add	r3, r3, #2
	ldr	r3, [r4, r3, asl #2]
	str	r3, [ip], #4
	ldr	r2, [r4, #2560]
	cmp	r2, r1
	bhi	.L72
	cmp	r2, #15
	bhi	.L75
.L71:
	add	r1, r2, #8
	add	r1, r5, r1, lsl #2
.L74:
	ldr	r3, [r4, #2496]
	add	r2, r2, #1
	cmp	r2, #15
	add	r3, r3, #332
	add	r3, r3, #2
	ldr	r3, [r4, r3, asl #2]
	str	r3, [r1], #4
	bls	.L74
.L75:
	ldr	r1, [r8, #1108]
	add	r3, r4, #116
	add	r2, r5, #104
	add	r0, r4, #180
	str	r1, [r5, #100]
.L73:
	ldr	r1, [r3, #4]!
	cmp	r3, r0
	str	r1, [r2], #4
	bne	.L73
	ldr	r3, [r8, #1100]
	mov	r1, r8
	mov	r0, r4
	str	r3, [r5, #172]
	ldr	r3, [r4, #1280]
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #1684]
	str	r3, [r5, #208]
	ldr	r3, [r4, #1812]
	str	r3, [r5, #212]
	ldr	r2, [r4, #184]
	ldr	r3, [r4, #188]
	orr	r3, r3, r2, asl #16
	str	r3, [r5, #216]
	bl	HEVC_WriteTileInfo
	subs	r7, r0, #0
	bne	.L84
	ldr	r3, [r8, #1112]
	mov	r1, r5
	mov	r0, #64
	str	r3, [r5, #220]
	ldr	r3, [r8, #1124]
	str	r3, [r5, #224]
	ldr	r3, [r8, #1116]
	str	r3, [r5, #228]
	ldr	r3, [r8, #1128]
	str	r3, [r5, #232]
	ldr	r3, [r8, #1120]
	str	r3, [r5, #236]
	ldr	r2, [r4, #220]
	ldr	lr, [r4, #224]
	ldr	ip, [r4, #236]
	ldrb	r3, [r4, #212]	@ zero_extendqisi2
	mov	r2, r2, asl #16
	ldrb	r10, [r4, #213]	@ zero_extendqisi2
	orr	lr, r2, lr, asl #13
	and	ip, ip, #31
	ldrb	r9, [r4, #228]	@ zero_extendqisi2
	orr	ip, lr, ip
	ldrb	lr, [r4, #215]	@ zero_extendqisi2
	ldrb	r2, [r4, #229]	@ zero_extendqisi2
	mov	r3, r3, asl #24
	ldrb	r8, [r4, #214]	@ zero_extendqisi2
	orr	r3, r3, r10, asl #23
	ldr	r10, [r4, #232]
	orr	ip, ip, r9, asl #12
	str	lr, [fp, #-52]
	orr	r2, ip, r2, asl #11
	and	ip, r10, #31
	ldr	r10, [fp, #-52]
	ldrb	lr, [r4, #230]	@ zero_extendqisi2
	orr	r8, r3, r8, asl #22
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	ldrb	r9, [r4, #217]	@ zero_extendqisi2
	orr	r8, r8, r10, asl #21
	orr	r2, r2, lr, asl #10
	orr	r3, r8, r3, asl #20
	orr	ip, r2, ip, asl #5
	orr	r9, r3, r9, asl #19
	orr	ip, ip, r9
	str	ip, [r5, #240]
	ldr	r2, [r4, #240]
	ldr	r3, [r4, #244]
	orr	r3, r3, r2, asl #16
	str	r3, [r5, #244]
	ldr	r3, [fp, #-48]
	str	r3, [r5, #252]
	bl	HEVC_GetVirAddr
	mov	r1, r6
	mov	r5, r0
	mov	r0, #64
	bl	HEVC_GetPhyAddr
	mov	r3, r5
	mov	r1, #256
	mov	r2, r0
	add	r0, r4, #252
	bl	HEVC_WriteQmatrix
.L70:
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L84:
	ldr	r1, .L85+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L70
.L83:
	ldr	r3, .L85+16
	mvn	r7, #0
	ldr	r2, .L85+20
	ldr	r1, .L85+24
	bl	dprint_vfmw
	b	.L70
.L86:
	.align	2
.L85:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC8
	.word	.LANCHOR1+1992
	.word	.LC1
	UNWIND(.fnend)
	.size	HEVCHAL_V400R003_SetPicMsg, .-HEVCHAL_V400R003_SetPicMsg
	.global	__aeabi_uidiv
	.global	__aeabi_uidivmod
	.align	2
	.global	HEVCHAL_V400R003_SetSliceMsg
	.type	HEVCHAL_V400R003_SetSliceMsg, %function
HEVCHAL_V400R003_SetSliceMsg:
	UNWIND(.fnstart)
	@ args = 24, pretend = 0, frame = 16
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
	str	r0, [fp, #-48]
	mov	r0, r3
	mov	r6, r3
	mov	r4, r2
	bl	MEM_Phy2Vir
	subs	r5, r0, #0
	beq	.L134
	ldr	r3, .L136
	mov	r1, #0
	mov	r8, r1
	mov	r2, #1280
	add	r9, r4, #16
	add	r10, r5, #8
	ldr	r3, [r3, #48]
	mov	r7, r5
	blx	r3
	str	r5, [fp, #-56]
	mov	r5, r8
	str	r4, [fp, #-52]
.L94:
	cmp	r5, #0
	mov	r3, r6
	mov	r2, r8
	ldr	r1, .L136+4
	mov	r0, #4
	beq	.L135
	ldr	r4, [r9, #-16]
	cmp	r4, #0
	beq	.L92
.L91:
	ldr	r2, [fp, #-48]
	mov	r3, r6
	ldr	ip, [r9]
	ldr	r1, .L136+4
	ldr	r0, [r2, #1324]
	bic	ip, ip, #15
	mov	r2, r8
	bic	r0, r0, #15
	rsb	r0, r0, ip
	str	r0, [r7]
	ldr	ip, [r10, #-8]
	mov	r0, #4
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	ip, [r9]
	ldr	r0, [r9, #-8]
	add	r3, r6, #4
	add	r2, r8, #1
	ldr	r1, .L136+4
	add	ip, r0, ip, lsl #3
	mov	r0, #4
	and	ip, ip, #127
	str	ip, [r7, #4]
	ldr	ip, [r7, #4]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	ip, [r9, #-16]
	add	r3, r6, #8
	add	r2, r8, #2
	ldr	r1, .L136+4
	mov	r0, #4
	str	ip, [r10]
	ldr	ip, [r7, #8]
	str	ip, [sp]
	bl	dprint_vfmw
.L93:
	add	r5, r5, #1
	add	r9, r9, #4
	cmp	r5, #2
	add	r7, r7, #12
	add	r8, r8, #3
	add	r6, r6, #12
	add	r10, r10, #12
	bne	.L94
	ldr	r4, [fp, #-52]
	ldr	r5, [fp, #-56]
	ldr	r0, [fp, #12]
	ldr	ip, [r4, #28]
	ldrb	r2, [r4, #32]	@ zero_extendqisi2
	ldrb	r3, [r4, #24]	@ zero_extendqisi2
	and	ip, ip, #127
	ldrb	lr, [r4, #25]	@ zero_extendqisi2
	ldr	r1, [r4, #36]
	mov	r2, r2, asl #18
	orr	r2, r2, ip, asl #19
	mov	r3, r3, asl #27
	orr	lr, r3, lr, asl #26
	orr	r1, r2, r1
	orr	r1, r1, lr
	str	r1, [r5, #24]
	ldr	r3, [r4, #44]
	ldr	r2, [r4, #56]
	ldr	ip, [r4, #76]
	ldr	lr, [r4, #60]
	mov	r3, r3, asl #24
	ldr	r1, .L136+8
	orr	r3, r3, r2, asl #20
	ldr	r2, [r4, #72]
	add	r1, r1, ip, lsl #2
	ldr	ip, [r4, #68]
	orr	r3, r3, lr, asl #16
	ldrb	lr, [r4, #64]	@ zero_extendqisi2
	ldr	r6, [r1, #2052]
	orr	r3, r3, r2, asl #8
	ldr	r1, [r4, #40]
	orr	r2, r3, ip, asl #2
	orr	r3, r2, r6
	ldrb	ip, [r4, #65]	@ zero_extendqisi2
	ldrb	r2, [r4, #52]	@ zero_extendqisi2
	orr	r3, r3, lr, asl #15
	mov	r1, r1, asl #28
	orr	r3, r3, ip, asl #14
	orr	r2, r1, r2, asl #27
	orr	r2, r3, r2
	ldr	r3, [fp, #-48]
	str	r2, [r5, #28]
	str	r0, [r5, #32]
	ldr	r6, [r3, #20]
	add	r6, r6, #1
	mov	r1, r6
	bl	__aeabi_uidiv
	mov	r1, r6
	mov	r6, r0, asl #16
	ldr	r0, [fp, #12]
	bl	__aeabi_uidivmod
	ldr	r3, [fp, #16]
	orr	r1, r6, r1
	str	r1, [r5, #36]
	str	r3, [r5, #40]
	ldr	r2, [r4, #96]
	ldr	r3, [r4, #100]
	and	r2, r2, #31
	and	r3, r3, #31
	orr	r3, r3, r2, asl #8
	str	r3, [r5, #44]
	ldr	r0, [r4, #108]
	ldr	r2, [r4, #104]
	and	r0, r0, #15
	ldrb	r1, [r4, #115]	@ zero_extendqisi2
	and	r2, r2, #15
	ldrb	ip, [r4, #113]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	ldrb	r3, [r4, #112]	@ zero_extendqisi2
	orr	r2, r0, r2, asl #16
	ldrb	r0, [r4, #114]	@ zero_extendqisi2
	orr	r2, r2, r1
	orr	r2, r2, ip, asl #3
	orr	r3, r2, r3, asl #2
	orr	r3, r3, r0, asl #1
	str	r3, [r5, #48]
	ldr	ip, [r4, #68]
	cmp	ip, #0
	beq	.L119
	mov	r2, #0
	add	r1, r4, #112
	mov	r3, r2
.L96:
	ldr	r0, [r1, #4]!
	orr	r2, r2, r0, asl r3
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L96
.L95:
	str	r2, [r5, #52]
	ldr	ip, [r4, #72]
	cmp	ip, #0
	beq	.L120
	mov	r2, #0
	add	r1, r4, #176
	mov	r3, r2
.L98:
	ldr	r0, [r1, #4]!
	orr	r2, r2, r0, asl r3
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L98
.L97:
	str	r2, [r5, #56]
	ldr	r2, [r4, #68]
	cmp	r2, #0
	addne	ip, r4, #368
	movne	r0, #0
	movne	r6, #15
	movne	r1, r0
	beq	.L105
.L104:
	and	r3, r1, #7
	ldr	lr, [ip, #4]!
	cmp	r3, #7
	sub	r2, r2, #1
	mov	r3, r3, asl #2
	orr	r0, r0, lr, asl r3
	streq	r0, [r5, r6, asl #2]
	addeq	r6, r6, #1
	moveq	r0, #0
	beq	.L103
	cmp	r2, r1
	streq	r0, [r5, r6, asl #2]
.L103:
	ldr	r2, [r4, #68]
	add	r1, r1, #1
	cmp	r2, r1
	bhi	.L104
.L105:
	ldr	r2, [r4, #72]
	cmp	r2, #0
	addne	ip, r4, #432
	movne	r0, #0
	movne	r6, #17
	movne	r1, r0
	beq	.L101
.L110:
	and	r3, r1, #7
	ldr	lr, [ip, #4]!
	cmp	r3, #7
	sub	r2, r2, #1
	mov	r3, r3, asl #2
	orr	r0, r0, lr, asl r3
	streq	r0, [r5, r6, asl #2]
	addeq	r6, r6, #1
	moveq	r0, #0
	beq	.L109
	cmp	r2, r1
	streq	r0, [r5, r6, asl #2]
.L109:
	ldr	r2, [r4, #72]
	add	r1, r1, #1
	cmp	r2, r1
	bhi	.L110
.L101:
	ldr	r3, [r4, #564]
	ldr	r2, [fp, #-48]
	str	r3, [r5, #92]
	ldr	r3, [r2, #76]
	mov	r3, r3, lsr #1
	mov	r3, r3, asl #8
	orr	r3, r3, #16384
	orr	r3, r3, #10
	str	r3, [r5, #96]
	ldr	r3, [fp, #20]
	str	r3, [r5, #172]
	ldr	r3, [fp, #24]
	str	r3, [r5, #176]
	ldr	r3, [fp, #8]
	str	r3, [r5, #252]
	ldrb	r3, [r2, #33]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L106
.L107:
	ldr	r3, [fp, #-48]
	ldrb	r3, [r3, #32]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L112
.L113:
	mov	r0, #0
.L89:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L92:
	str	r4, [r7]
	ldr	lr, [r10, #-8]
	str	lr, [sp]
	bl	dprint_vfmw
	str	r4, [r7, #4]
	ldr	lr, [r7, #4]
	add	r3, r6, #4
	add	r2, r8, #1
	ldr	r1, .L136+4
	mov	r0, #4
	str	lr, [sp]
	bl	dprint_vfmw
	str	r4, [r10]
	ldr	ip, [r7, #8]
	add	r3, r6, #8
	add	r2, r8, #2
	ldr	r1, .L136+4
	mov	r0, #4
	str	ip, [sp]
	bl	dprint_vfmw
	b	.L93
.L106:
	ldr	r3, [r4, #76]
	cmp	r3, #1
	bne	.L107
.L111:
	mov	r1, r5
	mov	r0, #64
	bl	HEVC_GetVirAddr
	mov	r1, r5
	mov	r6, r0
	mov	r0, #128
	bl	HEVC_GetVirAddr
	ldr	r3, [r4, #68]
	cmp	r3, #0
	beq	.L117
	mov	lr, #0
	add	r9, r4, #712
	add	r8, r4, #584
	mov	r7, lr
	mov	ip, r4
.L116:
	ldr	r1, [r9, #4]!
	add	r2, lr, #128
	ldr	r10, [r8, #4]!
	add	r7, r7, #1
	ldr	r5, [r4, #580]
	add	ip, ip, #8
	uxtb	r1, r1
	ubfx	r10, r10, #0, #9
	and	r5, r5, #7
	mov	r1, r1, asl #12
	orr	r3, r1, r10, asl #3
	orr	r3, r3, r5
	str	r3, [r6, lr]
	ldr	r5, [ip, #836]
	ldrb	r3, [ip, #1092]	@ zero_extendqisi2
	ldr	r1, [r4, #584]
	ubfx	r5, r5, #0, #9
	mov	r3, r3, asl #12
	and	r1, r1, #7
	orr	r3, r3, r5, asl #3
	orr	r3, r3, r1
	str	r3, [r0, lr]
	ldr	r3, [ip, #840]
	add	lr, lr, #4
	ldrb	r1, [ip, #1096]	@ zero_extendqisi2
	ubfx	r3, r3, #0, #9
	orr	r3, r3, r1, asl #9
	str	r3, [r0, r2]
	ldr	r3, [r4, #68]
	cmp	r3, r7
	bhi	.L116
.L117:
	ldr	lr, [r4, #76]
	cmp	lr, #0
	bne	.L113
	ldr	r3, [r4, #72]
	cmp	r3, #0
	beq	.L113
	add	r9, r4, #776
	add	r8, r4, #648
	mov	r7, lr
	mov	r1, r4
.L118:
	ldr	ip, [r9, #4]!
	add	r10, lr, #64
	ldr	r3, [r8, #4]!
	add	r2, lr, #192
	ldr	r5, [r4, #580]
	add	r7, r7, #1
	uxtb	ip, ip
	ubfx	r3, r3, #0, #9
	and	r5, r5, #7
	add	r1, r1, #8
	mov	ip, ip, asl #12
	add	lr, lr, #4
	orr	r3, ip, r3, asl #3
	orr	r3, r3, r5
	str	r3, [r6, r10]
	ldr	r3, [r1, #964]
	ldrb	r5, [r1, #1220]	@ zero_extendqisi2
	ldr	ip, [r4, #584]
	ubfx	r3, r3, #0, #9
	mov	r5, r5, asl #12
	and	ip, ip, #7
	orr	r3, r5, r3, asl #3
	orr	r3, r3, ip
	str	r3, [r0, r10]
	ldr	r3, [r1, #968]
	ldrb	ip, [r1, #1224]	@ zero_extendqisi2
	ubfx	r3, r3, #0, #9
	orr	r3, r3, ip, asl #9
	str	r3, [r0, r2]
	ldr	r3, [r4, #72]
	cmp	r3, r7
	bhi	.L118
	b	.L113
.L112:
	ldr	r3, [r4, #76]
	cmp	r3, #0
	beq	.L111
	b	.L113
.L119:
	mov	r2, ip
	b	.L95
.L120:
	mov	r2, ip
	b	.L97
.L134:
	ldr	r3, .L136+12
	ldr	r2, .L136+16
	ldr	r1, .L136+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L89
.L135:
	ldr	r3, [fp, #-52]
	ldr	r3, [r3]
	cmp	r3, #0
	bne	.L91
	mov	r0, r5
	ldr	r2, .L136+16
	ldr	r1, .L136+24
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L89
.L137:
	.align	2
.L136:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC14
	.word	.LANCHOR1
	.word	.LC12
	.word	.LANCHOR1+2020
	.word	.LC1
	.word	.LC13
	UNWIND(.fnend)
	.size	HEVCHAL_V400R003_SetSliceMsg, .-HEVCHAL_V400R003_SetSliceMsg
	.align	2
	.global	HEVCHAL_V400R003_CfgVdmReg
	.type	HEVCHAL_V400R003_CfgVdmReg, %function
HEVCHAL_V400R003_CfgVdmReg:
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
	mov	r5, r2
	mov	ip, r5, asl #6
	ldr	r2, .L152
	sub	ip, ip, r5, asl #3
	ldr	r6, .L152+4
	add	ip, r2, ip
	ldr	lr, .L152+8
	mov	r10, r1
	mov	r4, r0
	ldr	r1, [ip, #8]
	ldrb	r2, [r6]	@ zero_extendqisi2
	mov	r6, r3
	subs	r2, r2, #1
	ldr	r1, [lr, r1, asl #2]
	movne	r2, #1
	cmp	r3, #0
	movne	r0, #0
	andeq	r0, r2, #1
	cmp	r0, #0
	ldr	r0, [r1, #1204]
	str	r0, [fp, #-56]
	bne	.L150
	cmp	r1, #0
	mov	r8, #0
	mov	r0, #1
	str	r8, [fp, #-48]
	ldrne	ip, [r1, #1224]
	moveq	ip, r1
	moveq	r3, ip
	streq	ip, [fp, #-60]
	mov	r9, #3
	uxtbne	r3, ip
	str	ip, [fp, #-64]
	andne	r1, r3, #1
	strne	r1, [fp, #-60]
	cmp	r2, #0
	strneb	r3, [r6, #1]
	ldr	r3, [r4, #20]
	mov	r2, #0
	ldr	r1, [r4, #16]
	bfi	r2, r8, #7, #1
	ldrb	lr, [r4, #1]	@ zero_extendqisi2
	strb	r2, [fp, #-46]
	bfi	r0, lr, #6, #1
	mla	r2, r1, r3, r3
	and	r0, r0, #111
	bfi	r0, r8, #5, #1
	strb	r0, [fp, #-45]
	ldr	r7, [fp, #-48]
	mov	r3, r6
	add	r1, r2, r1
	mov	r0, #8
	bfi	r7, r1, #0, #20
	mov	r2, r5
	str	r7, [r4, #2580]
	mov	r1, r7
	str	r7, [fp, #-48]
	bl	MFDE_ConfigReg
	mov	r2, r7
	ldr	r1, .L152+12
	mov	r0, r9
	bl	dprint_vfmw
	ldr	r3, [fp, #-56]
	ldr	r1, [r4, #1320]
	mov	r2, #0
	bfi	r2, r3, #3, #1
	strh	r9, [fp, #-46]	@ movhi
	bfi	r2, r1, #4, #1
	mov	r3, #0
	bfi	r3, r8, #5, #1
	strb	r3, [fp, #-45]
	ldr	lr, [r4, #1332]
	and	r2, r2, #223
	ldr	r0, [r4, #1316]
	uxtb	r1, r3
	bfi	r2, lr, #6, #1
	mov	r3, #13
	bfi	r1, r0, #6, #1
	ldr	r0, [fp, #-60]
	mvn	r2, r2, asl #25
	and	r1, r1, #127
	strb	r3, [fp, #-48]
	bfi	r1, r0, #4, #1
	mvn	r2, r2, lsr #25
	strb	r1, [fp, #-45]
	strb	r2, [fp, #-47]
	mov	r3, r6
	ldr	r8, [fp, #-48]
	mov	r2, r5
	mov	r0, #12
	movw	r7, #3075
	movt	r7, 48
	mov	r1, r8
	str	r8, [r4, #2584]
	bl	MFDE_ConfigReg
	mov	r2, r8
	ldr	r1, .L152+16
	mov	r0, r9
	bl	dprint_vfmw
	ldr	r8, [r10, #56]
	mov	r3, r6
	mov	r2, r5
	bic	r8, r8, #15
	mov	r0, #16
	str	r8, [r4, #2588]
	mov	r1, r8
	bl	MFDE_ConfigReg
	mov	r2, r8
	ldr	r1, .L152+20
	mov	r0, r9
	bl	dprint_vfmw
	ldr	r1, [r10, #40]
	mov	r3, r6
	mov	r2, r5
	bic	r1, r1, #15
	mov	r0, #20
	str	r1, [r4, #2592]
	bl	MFDE_ConfigReg
	ldr	r1, [r4, #1324]
	mov	r3, r6
	mov	r2, r5
	bic	r1, r1, #15
	mov	r0, #24
	str	r1, [r4, #2596]
	bl	MFDE_ConfigReg
	mov	r1, r7
	mov	r3, r6
	mov	r2, r5
	mov	r0, #60
	str	r7, [r4, #2604]
	str	r7, [r4, #2608]
	str	r7, [r4, #2612]
	str	r7, [r4, #2616]
	str	r7, [r4, #2620]
	str	r7, [r4, #2624]
	str	r7, [r4, #2628]
	bl	MFDE_ConfigReg
	mov	r3, r6
	mov	r2, r5
	mov	r1, r7
	mov	r0, #64
	bl	MFDE_ConfigReg
	mov	r3, r6
	mov	r2, r5
	mov	r1, r7
	mov	r0, #68
	bl	MFDE_ConfigReg
	mov	r3, r6
	mov	r2, r5
	mov	r1, r7
	mov	r0, #72
	bl	MFDE_ConfigReg
	mov	r3, r6
	mov	r2, r5
	mov	r1, r7
	mov	r0, #76
	bl	MFDE_ConfigReg
	mov	r3, r6
	mov	r2, r5
	mov	r1, r7
	mov	r0, #80
	bl	MFDE_ConfigReg
	mov	r3, r6
	mov	r2, r5
	mov	r1, r7
	mov	r0, #84
	bl	MFDE_ConfigReg
	ldr	r1, [r4, #1280]
	mov	r3, r6
	mov	r2, r5
	add	r1, r1, #332
	mov	r0, #96
	add	r1, r1, #2
	ldr	r1, [r4, r1, asl #2]
	bic	r1, r1, #255
	str	r1, [r4, #2632]
	str	r1, [fp, #-48]
	bl	MFDE_ConfigReg
	ldr	ip, [fp, #-64]
	cmp	ip, #1
	beq	.L151
.L143:
	ldr	r8, [r4, #1288]
	mov	r3, r6
	mov	r2, r5
	mov	r0, #100
	mov	r7, #0
	str	r8, [r4, #2636]
	mov	r1, r8
	bl	MFDE_ConfigReg
	mov	r2, r8
	ldr	r1, .L152+24
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r8, [r4, #1292]
	mov	r3, r6
	mov	r2, r5
	mov	r0, #104
	str	r8, [r4, #2640]
	mov	r1, r8
	bl	MFDE_ConfigReg
	mov	r2, r8
	ldr	r1, .L152+28
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r1, [r4, #1300]
	mov	r3, r6
	mov	r2, r5
	mov	r0, #108
	bl	MFDE_ConfigReg
	ldr	r1, [r4, #1304]
	mov	r3, r6
	mov	r2, r5
	mov	r0, #116
	str	r1, [r4, #2656]
	bl	MFDE_ConfigReg
	ldr	r1, [r4, #1308]
	mov	r3, r6
	mov	r2, r5
	mov	r0, #120
	str	r1, [r4, #2660]
	bl	MFDE_ConfigReg
	ldr	r1, [r4, #1312]
	mov	r3, r6
	mov	r2, r5
	mov	r0, #124
	str	r1, [r4, #2664]
	bl	MFDE_ConfigReg
	str	r7, [r4, #2680]
	mov	r1, r7
	mov	r3, r6
	mov	r2, r5
	mov	r0, #128
	str	r7, [fp, #-48]
	bl	MFDE_ConfigReg
	strh	r7, [fp, #-48]	@ movhi
	mov	r3, r6
	ldr	r1, [fp, #-48]
	mov	r2, r5
	mov	r0, #132
	str	r1, [r4, #2684]
	bl	MFDE_ConfigReg
	ldr	r3, [r4, #188]
	mov	r2, r5
	mov	r0, #4
	cmp	r3, #4096
	mov	r3, r6
	movls	r1, #1
	movhi	r1, r7
	mov	r4, r1
	bl	SCD_ConfigReg
	mov	r2, r4
	ldr	r1, .L152+32
	mov	r0, #3
	bl	dprint_vfmw
	mov	r3, r6
	mov	r2, r5
	mov	r1, #2
	mov	r0, #152
	bl	MFDE_ConfigReg
	mov	r2, #2
	ldr	r1, .L152+36
	mov	r0, #3
	bl	dprint_vfmw
	mov	r0, #32
	mov	r3, r6
	mov	r2, r5
	mvn	r1, #0
	bl	MFDE_ConfigReg
	mov	r0, #0
.L140:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L151:
	mov	r3, r6
	mov	r2, r5
	mov	r1, #60
	mov	r0, #92
	bl	MFDE_ConfigReg
	mov	r2, #60
	ldr	r1, .L152+40
	mov	r0, r9
	bl	dprint_vfmw
	ldr	r1, [r4, #1280]
	mov	r2, r5
	mov	r3, r6
	mov	r0, #112
	add	r1, r4, r1, lsl #2
	ldr	r7, [r1, #1556]
	mov	r1, r7
	bl	MFDE_ConfigReg
	mov	r2, r7
	mov	r0, r9
	ldr	r1, .L152+44
	bl	dprint_vfmw
	b	.L143
.L150:
	ldr	r2, .L152+48
	mov	r0, #0
	ldr	r1, .L152+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L140
.L153:
	.align	2
.L152:
	.word	g_VdmDrvParam
	.word	g_HalDisable
	.word	s_pstVfmwChan
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.word	.LC19
	.word	.LC20
	.word	.LANCHOR1+2064
	.word	.LC15
	UNWIND(.fnend)
	.size	HEVCHAL_V400R003_CfgVdmReg, .-HEVCHAL_V400R003_CfgVdmReg
	.align	2
	.global	HEVCHAL_V400R003_StartDec
	.type	HEVCHAL_V400R003_StartDec, %function
HEVCHAL_V400R003_StartDec:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
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
	ldr	r8, .L226
	movw	ip, #1228
	cmp	r1, #0
	mov	r7, r0
	mla	r3, ip, r1, r8
	str	r3, [fp, #-48]
	bgt	.L212
	bne	.L206
	cmp	r0, #0
	beq	.L213
	ldr	r3, [r0, #2560]
	cmp	r3, #16
	bhi	.L214
	ldr	r3, [r8]
	cmp	r3, #0
	beq	.L215
.L162:
	ldr	r4, [fp, #-48]
	mov	r3, r2
	mov	r0, r7
	mov	r2, #0
	mov	r1, r4
	bl	HEVCHAL_V400R003_CfgVdmReg
	ldr	r3, [r8, #60]
	ldr	r2, [r8, #56]
	mov	r1, r4
	mov	r0, r7
	bl	HEVCHAL_V400R003_SetPicMsg
	ldr	r6, [r7, #2568]
	cmp	r6, #0
	beq	.L216
	ldr	r9, [r6, #92]
	cmp	r9, #0
	bne	.L217
	mov	r10, r9
.L165:
	ldr	r1, [r7, #20]
	ldr	r3, [r7, #16]
	ldr	r2, [r7, #2564]
	mla	r3, r1, r3, r3
	cmp	r2, #0
	add	r3, r3, r1
	str	r3, [fp, #-64]
	ble	.L168
	add	r1, r10, #14
	add	r3, r10, #15
	cmp	r6, #0
	add	r0, r10, #4
	ldr	r1, [r8, r1, asl #2]
	ldr	r3, [r8, r3, asl #2]
	str	r0, [fp, #-68]
	str	r1, [fp, #-52]
	add	r1, r10, #5
	str	r3, [fp, #-56]
	mov	ip, r1
	str	r1, [fp, #-60]
	beq	.L190
	ldr	r1, [r6, #92]
	mov	r9, #0
	mov	r5, r9
	str	r10, [fp, #-72]
.L170:
	add	r5, r5, #1
	ldr	r0, [r6, #80]
	cmp	r5, r2
	bge	.L192
	ldr	r4, [r6, #1356]
	cmp	r4, #0
	bne	.L210
	b	.L173
.L175:
	add	r5, r5, #1
	cmp	r5, r2
	beq	.L187
	ldr	r4, [r4, #1356]
	cmp	r4, #0
	beq	.L173
.L210:
	ldr	r3, [r4, #92]
	cmp	r3, r1
	bls	.L175
.L172:
	cmp	ip, #209
	cmpne	r5, r2
	beq	.L218
	ldrb	r2, [r7, #217]	@ zero_extendqisi2
	ldr	r3, [r4, #92]
	cmp	r2, #0
	ldr	r10, [fp, #-64]
	sub	r3, r3, #1
	ldr	ip, [fp, #-60]
	ldrne	r2, [r4, #568]
	moveq	r2, r3
	cmp	r10, r3
	add	ip, r5, ip
	movge	lr, #0
	movlt	lr, #1
	cmp	r2, r10
	orrgt	lr, lr, #1
	add	ip, ip, #10
	cmp	lr, #0
	ldr	r10, [r8, ip, asl #2]
	bne	.L219
.L179:
	cmp	r3, r1
	str	r3, [r6, #572]
	bcc	.L220
	ldr	lr, [r7, #2564]
	sub	ip, r5, #1
	sub	lr, lr, #1
	cmp	lr, ip
	moveq	ip, #0
	streq	ip, [r6, #1356]
	ldr	ip, [fp, #-52]
	str	r3, [sp, #20]
	str	r2, [sp, #16]
	mov	r2, r6
	str	r1, [sp, #12]
	str	r0, [sp, #8]
	mov	r0, r7
	ldr	r3, [fp, #-56]
	ldr	r1, [fp, #-48]
	str	r10, [sp, #4]
	str	ip, [sp]
	bl	HEVCHAL_V400R003_SetSliceMsg
	cmp	r0, #0
	bne	.L221
	cmp	r10, #0
	add	r9, r9, #1
	ldr	r3, [r6, #572]
	beq	.L222
	ldr	r2, [r7, #2564]
	cmp	r2, r5
	ble	.L223
	ldr	r1, [fp, #-68]
	cmp	r4, #0
	add	r0, r5, r1
	ldr	r1, [fp, #-60]
	add	r0, r0, #10
	add	ip, r1, r5
	add	r1, ip, #10
	ldr	r0, [r8, r0, asl #2]
	ldr	r1, [r8, r1, asl #2]
	str	r0, [fp, #-52]
	str	r1, [fp, #-56]
	beq	.L169
	ldr	r1, [r4, #92]
	cmp	r1, r3
	movcs	r0, #0
	movcc	r0, #1
	cmp	r5, #0
	movle	r0, #0
	cmp	r0, #0
	bne	.L224
	mov	r6, r4
	b	.L170
.L217:
	ldrb	r3, [r7, #217]	@ zero_extendqisi2
	mov	r10, #1
	ldr	r2, [r6, #8]
	sub	ip, r9, #1
	cmp	r3, #0
	ldr	r3, [r6]
	ldr	r1, [r6, #12]
	mov	r0, r7
	str	r10, [r6]
	moveq	lr, ip
	str	r3, [fp, #-52]
	ldr	r3, [r6, #4]
	ldrne	lr, [r6, #568]
	ldr	r5, [r6, #16]
	str	r3, [fp, #-56]
	mov	r3, #0
	str	r3, [r6, #4]
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	ldr	r4, [r7, #1324]
	str	r2, [fp, #-60]
	mov	r2, r6
	str	r1, [fp, #-64]
	str	r4, [r6, #16]
	ldr	r4, [r6, #20]
	str	r3, [r6, #20]
	str	ip, [sp, #20]
	str	lr, [sp, #16]
	ldr	ip, [r8, #64]
	ldr	lr, [r8, #56]
	str	r3, [sp, #12]
	str	r3, [sp, #8]
	ldr	r1, [fp, #-48]
	ldr	r3, [r8, #60]
	str	ip, [sp, #4]
	str	lr, [sp]
	bl	HEVCHAL_V400R003_SetSliceMsg
	cmp	r0, #0
	bne	.L225
	ldr	r3, [fp, #-52]
	str	r9, [r6, #92]
	str	r5, [r6, #16]
	str	r3, [r6]
	ldr	r3, [fp, #-56]
	str	r4, [r6, #20]
	str	r3, [r6, #4]
	ldr	r3, [fp, #-60]
	str	r3, [r6, #8]
	ldr	r3, [fp, #-64]
	str	r3, [r6, #12]
	ldr	r6, [r7, #2568]
	b	.L165
.L173:
	ldr	r2, .L226+4
	mov	r0, #1
	ldr	r1, .L226+8
	mvn	ip, #0
	str	ip, [fp, #-48]
	bl	dprint_vfmw
	ldr	ip, [fp, #-48]
.L156:
	mov	r0, ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L218:
	mov	r2, r5
.L187:
	mov	r3, #0
	str	r3, [r6, #1356]
	ldr	r3, [fp, #-64]
	mov	r5, r2
	mov	r10, #0
	mov	r2, r3
	b	.L179
.L215:
	mov	r0, #0
	str	r2, [fp, #-52]
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L163
	str	r3, [r8]
	ldr	r2, [fp, #-52]
	b	.L162
.L192:
	mov	r4, r6
	b	.L172
.L212:
	mov	r3, r1
	ldr	r2, .L226+4
	mov	r1, #1
	mov	r0, #0
	str	r1, [sp]
	mvn	ip, #0
	ldr	r1, .L226+12
	str	ip, [fp, #-48]
	bl	dprint_vfmw
	ldr	ip, [fp, #-48]
	b	.L156
.L190:
	mov	r5, r6
.L169:
	mov	r3, r5
	ldr	r2, .L226+4
	ldr	r1, .L226+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	ip, #0
	b	.L156
.L219:
	ldr	r1, [fp, #-64]
	mov	r0, #1
	mvn	ip, #0
	str	ip, [fp, #-48]
	str	r1, [sp]
	ldr	r1, .L226+20
	bl	dprint_vfmw
	ldr	ip, [fp, #-48]
	b	.L156
.L220:
	mov	r2, r1
	mov	r0, #1
	ldr	r1, .L226+24
	bl	dprint_vfmw
	mvn	ip, #0
	b	.L156
.L221:
	ldr	r2, .L226+4
	mov	r0, #1
	ldr	r1, .L226+28
	mvn	ip, #0
	str	ip, [fp, #-48]
	bl	dprint_vfmw
	ldr	ip, [fp, #-48]
	b	.L156
.L222:
	ldr	r10, [fp, #-72]
	mov	ip, r0
	ldr	r2, [r7, #2564]
.L185:
	cmp	r2, #0
	str	ip, [fp, #-48]
	movgt	r3, #1
	movle	r3, #0
	cmp	r9, #0
	andgt	r3, r3, #1
	movle	r3, #0
	cmp	r3, #0
	beq	.L168
	add	r2, r10, r2
	add	r3, r2, #14
	ldr	r0, [r8, r3, asl #2]
	bl	HEVCHAL_V400R003_CutSliceChain
	ldr	ip, [fp, #-48]
	b	.L156
.L223:
	ldr	r10, [fp, #-72]
	mov	ip, r0
	b	.L185
.L224:
	mov	r2, r1
	mov	r0, #1
	ldr	r1, .L226+32
	bl	dprint_vfmw
	mvn	ip, #0
	b	.L156
.L168:
	ldr	r1, .L226+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	ip, #0
	b	.L156
.L214:
	mov	r0, r1
	ldr	r3, .L226+40
	ldr	r2, .L226+4
	mvn	ip, #0
	ldr	r1, .L226+44
	str	ip, [fp, #-48]
	bl	dprint_vfmw
	ldr	ip, [fp, #-48]
	b	.L156
.L206:
	ldr	r1, .L226+48
	mov	r0, #0
	bl	dprint_vfmw
	mvn	ip, #0
	b	.L156
.L225:
	mov	r0, r10
	ldr	r2, .L226+4
	ldr	r1, .L226+52
	mvn	ip, #0
	str	ip, [fp, #-48]
	bl	dprint_vfmw
	ldr	ip, [fp, #-48]
	b	.L156
.L216:
	mov	r0, r6
	ldr	r2, .L226+4
	ldr	r1, .L226+56
	mvn	ip, #0
	str	ip, [fp, #-48]
	bl	dprint_vfmw
	ldr	ip, [fp, #-48]
	b	.L156
.L213:
	ldr	r3, .L226+60
	mvn	ip, #0
	ldr	r2, .L226+4
	ldr	r1, .L226+44
	str	ip, [fp, #-48]
	bl	dprint_vfmw
	ldr	ip, [fp, #-48]
	b	.L156
.L163:
	ldr	r2, .L226+4
	mvn	ip, #0
	ldr	r1, .L226+64
	str	ip, [fp, #-48]
	bl	dprint_vfmw
	ldr	ip, [fp, #-48]
	b	.L156
.L227:
	.align	2
.L226:
	.word	g_HwMem
	.word	.LANCHOR1+2092
	.word	.LC34
	.word	.LC25
	.word	.LC32
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.word	.LC33
	.word	.LC38
	.word	.LC28
	.word	.LC1
	.word	.LC26
	.word	.LC31
	.word	.LC30
	.word	.LC27
	.word	.LC29
	UNWIND(.fnend)
	.size	HEVCHAL_V400R003_StartDec, .-HEVCHAL_V400R003_StartDec
	.section	.rodata
	.align	2
.LANCHOR1 = . + 0
	.type	__func__.14575, %object
	.size	__func__.14575, 18
__func__.14575:
	.ascii	"HEVCWriteCabacTab\000"
	.space	2
	.type	s_InitValue, %object
	.size	s_InitValue, 1920
s_InitValue:
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	153
	.word	200
	.word	139
	.word	141
	.word	157
	.word	154
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	184
	.word	-1
	.word	-1
	.word	-1
	.word	184
	.word	63
	.word	94
	.word	138
	.word	182
	.word	154
	.word	111
	.word	141
	.word	154
	.word	154
	.word	139
	.word	139
	.word	138
	.word	153
	.word	136
	.word	167
	.word	152
	.word	152
	.word	110
	.word	110
	.word	124
	.word	125
	.word	140
	.word	153
	.word	125
	.word	127
	.word	140
	.word	109
	.word	111
	.word	143
	.word	127
	.word	111
	.word	79
	.word	108
	.word	123
	.word	63
	.word	110
	.word	110
	.word	124
	.word	125
	.word	140
	.word	153
	.word	125
	.word	127
	.word	140
	.word	109
	.word	111
	.word	143
	.word	127
	.word	111
	.word	79
	.word	108
	.word	123
	.word	63
	.word	91
	.word	171
	.word	134
	.word	141
	.word	140
	.word	92
	.word	137
	.word	138
	.word	140
	.word	152
	.word	138
	.word	139
	.word	153
	.word	74
	.word	149
	.word	92
	.word	139
	.word	107
	.word	122
	.word	152
	.word	140
	.word	179
	.word	166
	.word	182
	.word	140
	.word	227
	.word	122
	.word	197
	.word	111
	.word	111
	.word	125
	.word	110
	.word	110
	.word	94
	.word	124
	.word	108
	.word	124
	.word	107
	.word	125
	.word	141
	.word	179
	.word	153
	.word	125
	.word	107
	.word	125
	.word	141
	.word	179
	.word	153
	.word	125
	.word	107
	.word	125
	.word	141
	.word	179
	.word	153
	.word	125
	.word	140
	.word	139
	.word	182
	.word	182
	.word	152
	.word	136
	.word	152
	.word	136
	.word	153
	.word	136
	.word	139
	.word	111
	.word	136
	.word	139
	.word	111
	.word	-1
	.word	153
	.word	138
	.word	138
	.word	-1
	.word	-1
	.word	79
	.word	110
	.word	122
	.word	95
	.word	79
	.word	63
	.word	31
	.word	31
	.word	153
	.word	153
	.word	168
	.word	140
	.word	198
	.word	-1
	.word	-1
	.word	-1
	.word	153
	.word	185
	.word	107
	.word	139
	.word	126
	.word	154
	.word	197
	.word	185
	.word	201
	.word	149
	.word	154
	.word	139
	.word	154
	.word	154
	.word	154
	.word	152
	.word	149
	.word	107
	.word	167
	.word	154
	.word	153
	.word	111
	.word	154
	.word	154
	.word	139
	.word	139
	.word	107
	.word	167
	.word	91
	.word	122
	.word	107
	.word	167
	.word	125
	.word	110
	.word	94
	.word	110
	.word	95
	.word	79
	.word	125
	.word	111
	.word	110
	.word	78
	.word	110
	.word	111
	.word	111
	.word	95
	.word	94
	.word	108
	.word	123
	.word	108
	.word	125
	.word	110
	.word	94
	.word	110
	.word	95
	.word	79
	.word	125
	.word	111
	.word	110
	.word	78
	.word	110
	.word	111
	.word	111
	.word	95
	.word	94
	.word	108
	.word	123
	.word	108
	.word	121
	.word	140
	.word	61
	.word	154
	.word	154
	.word	196
	.word	196
	.word	167
	.word	154
	.word	152
	.word	167
	.word	182
	.word	182
	.word	134
	.word	149
	.word	136
	.word	153
	.word	121
	.word	136
	.word	137
	.word	169
	.word	194
	.word	166
	.word	167
	.word	154
	.word	167
	.word	137
	.word	182
	.word	155
	.word	154
	.word	139
	.word	153
	.word	139
	.word	123
	.word	123
	.word	63
	.word	153
	.word	166
	.word	183
	.word	140
	.word	136
	.word	153
	.word	154
	.word	166
	.word	183
	.word	140
	.word	136
	.word	153
	.word	154
	.word	166
	.word	183
	.word	140
	.word	136
	.word	153
	.word	154
	.word	170
	.word	153
	.word	123
	.word	123
	.word	107
	.word	121
	.word	107
	.word	121
	.word	167
	.word	151
	.word	183
	.word	140
	.word	151
	.word	183
	.word	140
	.word	-1
	.word	124
	.word	138
	.word	94
	.word	-1
	.word	-1
	.word	79
	.word	154
	.word	137
	.word	95
	.word	79
	.word	63
	.word	31
	.word	31
	.word	153
	.word	153
	.word	168
	.word	169
	.word	198
	.word	-1
	.word	-1
	.word	-1
	.word	153
	.word	160
	.word	107
	.word	139
	.word	126
	.word	154
	.word	197
	.word	185
	.word	201
	.word	134
	.word	154
	.word	139
	.word	154
	.word	154
	.word	183
	.word	152
	.word	149
	.word	92
	.word	167
	.word	154
	.word	153
	.word	111
	.word	154
	.word	154
	.word	139
	.word	139
	.word	107
	.word	167
	.word	91
	.word	107
	.word	107
	.word	167
	.word	125
	.word	110
	.word	124
	.word	110
	.word	95
	.word	94
	.word	125
	.word	111
	.word	111
	.word	79
	.word	125
	.word	126
	.word	111
	.word	111
	.word	79
	.word	108
	.word	123
	.word	93
	.word	125
	.word	110
	.word	124
	.word	110
	.word	95
	.word	94
	.word	125
	.word	111
	.word	111
	.word	79
	.word	125
	.word	126
	.word	111
	.word	111
	.word	79
	.word	108
	.word	123
	.word	93
	.word	121
	.word	140
	.word	61
	.word	154
	.word	154
	.word	196
	.word	167
	.word	167
	.word	154
	.word	152
	.word	167
	.word	182
	.word	182
	.word	134
	.word	149
	.word	136
	.word	153
	.word	121
	.word	136
	.word	122
	.word	169
	.word	208
	.word	166
	.word	167
	.word	154
	.word	152
	.word	167
	.word	182
	.word	170
	.word	154
	.word	139
	.word	153
	.word	139
	.word	123
	.word	123
	.word	63
	.word	124
	.word	166
	.word	183
	.word	140
	.word	136
	.word	153
	.word	154
	.word	166
	.word	183
	.word	140
	.word	136
	.word	153
	.word	154
	.word	166
	.word	183
	.word	140
	.word	136
	.word	153
	.word	154
	.word	170
	.word	153
	.word	138
	.word	138
	.word	122
	.word	121
	.word	122
	.word	121
	.word	167
	.word	151
	.word	183
	.word	140
	.word	151
	.word	183
	.word	140
	.word	-1
	.word	224
	.word	167
	.word	122
	.word	-1
	.word	-1
	.type	__func__.14596, %object
	.size	__func__.14596, 31
__func__.14596:
	.ascii	"HEVCHAL_V400R003_CutSliceChain\000"
	.space	1
	.type	__func__.14615, %object
	.size	__func__.14615, 19
__func__.14615:
	.ascii	"HEVC_WriteTileInfo\000"
	.space	1
	.type	__func__.14641, %object
	.size	__func__.14641, 27
__func__.14641:
	.ascii	"HEVCHAL_V400R003_SetPicMsg\000"
	.space	1
	.type	__func__.14678, %object
	.size	__func__.14678, 29
__func__.14678:
	.ascii	"HEVCHAL_V400R003_SetSliceMsg\000"
	.space	3
	.type	s_SliceTypeForPMV, %object
	.size	s_SliceTypeForPMV, 12
s_SliceTypeForPMV:
	.word	2
	.word	1
	.word	0
	.type	__func__.14711, %object
	.size	__func__.14711, 27
__func__.14711:
	.ascii	"HEVCHAL_V400R003_CfgVdmReg\000"
	.space	1
	.type	__func__.14739, %object
	.size	__func__.14739, 26
__func__.14739:
	.ascii	"HEVCHAL_V400R003_StartDec\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"can not map mn virtual address!\012\000" )
	.space	3
.LC1:
	ASCII(.ascii	"%s: %s\012\000" )
.LC2:
	ASCII(.ascii	"HEVCWriteCabacTab return error.\012\000" )
	.space	3
.LC3:
	ASCII(.ascii	"can not map slice msg virtual address!\012\000" )
.LC4:
	ASCII(.ascii	"qmatrix[%d] = 0x%x\012\000" )
.LC5:
	ASCII(.ascii	"picmsg Dxx addr 0x%x = 0x%x\012\000" )
	.space	3
.LC6:
	ASCII(.ascii	"can not map slice segment info virtual address!\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC7:
	ASCII(.ascii	"TileId: 0x%x\012\000" )
	.space	2
.LC8:
	ASCII(.ascii	"can not map down msg virtual address!\012\000" )
	.space	1
.LC9:
	ASCII(.ascii	"pic msg burst0:\012\000" )
	.space	3
.LC10:
	ASCII(.ascii	"picmsg D[0] addr 0x%x = 0x%x\012\000" )
	.space	2
.LC11:
	ASCII(.ascii	"HEVC_WriteTileInfo return error.\012\000" )
	.space	2
.LC12:
	ASCII(.ascii	"HEVCHAL_V400R003_SetSliceMsg can not map slice msg " )
	ASCII(.ascii	"virtual address!\012\000" )
	.space	3
.LC13:
	ASCII(.ascii	"%s FATAL: i=0, valid_bitlen=0\012\000" )
	.space	1
.LC14:
	ASCII(.ascii	"slicemsg D[%d] addr 0x%x = 0x%x\012\000" )
	.space	3
.LC15:
	ASCII(.ascii	"%s: pMfdeTask(%p) = NULL\012\000" )
	.space	2
.LC16:
	ASCII(.ascii	"HEVC_BASIC_CFG0 = 0x%x\012\000" )
.LC17:
	ASCII(.ascii	"HEVC_BASIC_CFG1 = 0x%x\012\000" )
.LC18:
	ASCII(.ascii	"AVM_ADDR = 0x%x\012\000" )
	.space	3
.LC19:
	ASCII(.ascii	"VREG_V200R003_PART_DEC_OVER_INT_LEVEL=0x%x\012\000" )
.LC20:
	ASCII(.ascii	"VREG_LINE_NUM_STADDR = 0x%x\012\000" )
	.space	3
.LC21:
	ASCII(.ascii	"HEVC_VFMW_YSTADDR_1D = 0x%x\012\000" )
	.space	3
.LC22:
	ASCII(.ascii	"HEVC_VFMW_UVOFFSET_1D = 0x%x\012\000" )
	.space	2
.LC23:
	ASCII(.ascii	"SCD REG_EMAR_ID = 0x%x\012\000" )
.LC24:
	ASCII(.ascii	"VREG_FF_APT_EN = 0x%x\012\000" )
	.space	1
.LC25:
	ASCII(.ascii	"%s VdhId %d >= %d\012\000" )
	.space	1
.LC26:
	ASCII(.ascii	"VdhId is wrong! HEVC4HAL_V200R003_StartDec\012\000" )
.LC27:
	ASCII(.ascii	"pParam is null\012\000" )
.LC28:
	ASCII(.ascii	"FATAL: ApcSize > 16\012\000" )
	.space	3
.LC29:
	ASCII(.ascii	"%s vdm register virtual address not mapped, reset f" )
	ASCII(.ascii	"ailed!\012\000" )
	.space	1
.LC30:
	ASCII(.ascii	"%s pFirstSlice = NULL!\012\000" )
.LC31:
	ASCII(.ascii	"%s 0: set slice msg failed!\012\000" )
	.space	3
.LC32:
	ASCII(.ascii	"%s: i = %d, pSlicePara = NULL!\012\000" )
.LC33:
	ASCII(.ascii	"SliceSegmentTileAddress %d < pre_end_ctb_in_slice_t" )
	ASCII(.ascii	"ile %d\012\000" )
	.space	1
.LC34:
	ASCII(.ascii	"%s pTargetSlicePara is NULL!\012\000" )
	.space	2
.LC35:
	ASCII(.ascii	"end_ctb_in_slice_raster(%d)/end_ctb_in_slice_tile(%" )
	ASCII(.ascii	"d) > pic_max_ctb(%d)\012\000" )
	.space	3
.LC36:
	ASCII(.ascii	"SliceSegmentTileAddress %d > end_ctb_in_slice_tile " )
	ASCII(.ascii	"%d\012\000" )
	.space	1
.LC37:
	ASCII(.ascii	"%s 1: set slice msg failed!\012\000" )
	.space	3
.LC38:
	ASCII(.ascii	"No slice to dec, add up msg report.\012\000" )
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	s_CabacMN, %object
	.size	s_CabacMN, 928
s_CabacMN:
	.space	928
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
