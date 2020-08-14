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
	.file	"vdm_hal_mpeg4.c"
	.text
	.align	2
	.global	MP4HAL_V400R003_Log2bin
	.type	MP4HAL_V400R003_Log2bin, %function
MP4HAL_V400R003_Log2bin:
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
	ldmeqfd	sp, {fp, sp, pc}
	mov	r3, #0
.L3:
	add	r3, r3, #1
	movs	r0, r0, lsr #1
	uxth	r3, r3
	bne	.L3
	sxth	r0, r3
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MP4HAL_V400R003_Log2bin, .-MP4HAL_V400R003_Log2bin
	.align	2
	.global	MP4HAL_V400R003_InitHal
	.type	MP4HAL_V400R003_InitHal, %function
MP4HAL_V400R003_InitHal:
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
	.size	MP4HAL_V400R003_InitHal, .-MP4HAL_V400R003_InitHal
	.align	2
	.global	MP4HAL_V400R003_CfgReg
	.type	MP4HAL_V400R003_CfgReg, %function
MP4HAL_V400R003_CfgReg:
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
	subs	r4, r1, #0
	mov	r8, r0
	mov	r5, r3
	bgt	.L9
	add	r7, r0, #12288
	ldr	r0, [r7, #2288]
	cmp	r0, #0
	beq	.L10
	mov	ip, r8
	mov	r1, #0
	mvn	r6, #0
.L11:
	ldr	r3, [ip, #256]
	cmp	r3, #0
	beq	.L15
	ldr	lr, [ip, #248]
	bic	r3, r3, #15
	cmp	lr, #0
	beq	.L15
	cmp	r6, r3
	movcs	r6, r3
.L15:
	ldr	lr, [ip, #260]
	cmp	lr, #0
	beq	.L13
	ldr	r3, [ip, #252]
	bic	lr, lr, #15
	cmp	r3, #0
	beq	.L13
	cmp	r6, lr
	movcs	r6, lr
.L13:
	add	r1, r1, #1
	add	ip, ip, #28
	cmp	r1, r0
	bne	.L11
	cmn	r6, #1
	beq	.L10
	str	r6, [r2]
	mov	r3, r5
	ldrh	r1, [r8, #166]
	mov	r2, r4
	ldrh	r9, [r8, #164]
	mov	r0, #8
	mul	r9, r1, r9
	sub	r9, r9, #1
	ubfx	r9, r9, #0, #20
	orr	r9, r9, #1107296256
	orr	r9, r9, #4194304
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L34
	mov	r0, #3
	bl	dprint_vfmw
	ldrh	r3, [r8, #164]
	cmp	r3, #120
	ble	.L33
	ldr	r9, [r7, #2384]
	mov	r3, #0
	mov	ip, r3
	str	r3, [r7, #2392]
	and	r9, r9, #1
	mov	r9, r9, asl #30
.L19:
	ldr	r1, [r7, #2388]
	mov	r0, #12
	mov	r3, r5
	mov	r2, r4
	and	r1, r1, #1
	orr	r1, ip, r1, asl r0
	orr	r1, r1, #114688
	orr	r1, r1, #2
	orr	r9, r1, r9
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L34+4
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r1, .L34+8
	movw	r9, #1228
	mov	r3, r5
	mov	r2, r4
	mov	r0, #16
	mla	r9, r9, r4, r1
	ldr	r10, [r9, #56]
	bic	r10, r10, #15
	mov	r1, r10
	bl	MFDE_ConfigReg
	mov	r2, r10
	ldr	r1, .L34+12
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r10, [r9, #40]
	mov	r3, r5
	mov	r2, r4
	bic	r10, r10, #15
	mov	r0, #20
	mov	r1, r10
	bl	MFDE_ConfigReg
	mov	r2, r10
	ldr	r1, .L34+16
	mov	r0, #3
	bl	dprint_vfmw
	mov	r3, r5
	mov	r1, r6
	mov	r2, r4
	mov	r0, #24
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L34+20
	mov	r0, #3
	bl	dprint_vfmw
	ldrh	r1, [r8, #164]
	mov	r3, r5
	mov	r2, r4
	cmp	r1, #120
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
	ldr	r6, [r8, #212]
	mov	r3, r5
	mov	r2, r4
	bic	r6, r6, #15
	mov	r0, #96
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L34+24
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r6, [r7, #2316]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #100
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L34+28
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r6, [r7, #2320]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #104
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L34+32
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r6, [r7, #2328]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #108
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L34+36
	mov	r0, #3
	bl	dprint_vfmw
	mov	r3, r5
	mov	r1, r6
	mov	r2, r4
	mov	r0, #152
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L34+40
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r6, [r9, #1204]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #144
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L34+44
	mov	r0, #3
	bl	dprint_vfmw
	mov	r0, #32
	mov	r3, r5
	mov	r2, r4
	mvn	r1, #0
	bl	MFDE_ConfigReg
	mov	r0, #0
.L12:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L33:
	mov	r9, #0
	mov	r3, #1
	str	r9, [r7, #2384]
	mov	ip, #536870912
	str	r3, [r7, #2392]
	b	.L19
.L9:
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L34+48
	ldr	r1, .L34+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L12
.L10:
	mvn	r2, #0
	ldr	r1, .L34+56
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L12
.L35:
	.align	2
.L34:
	.word	.LC2
	.word	.LC3
	.word	g_HwMem
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	UNWIND(.fnend)
	.size	MP4HAL_V400R003_CfgReg, .-MP4HAL_V400R003_CfgReg
	.global	__aeabi_idiv
	.align	2
	.global	MP4HAL_V400R003_CfgDnMsg
	.type	MP4HAL_V400R003_CfgDnMsg, %function
MP4HAL_V400R003_CfgDnMsg:
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
	ldrh	r2, [r0, #166]
	ldrh	r3, [r0, #164]
	mov	r4, r0
	mov	r9, r1
	mul	r3, r3, r2
	cmp	r3, #0
	beq	.L59
	mov	r6, #0
.L38:
	add	r6, r6, #1
	movs	r3, r3, lsr #1
	uxth	r6, r6
	bne	.L38
.L37:
	ldr	r2, .L79
	movw	r3, #1228
	mla	r3, r3, r9, r2
	ldr	r0, [r3, #56]
	bl	MEM_Phy2Vir
	subs	r5, r0, #0
	beq	.L71
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L72
	mov	r7, #0
	ldr	r1, .L79+4
	mov	r2, r7
	str	r7, [r5]
	mov	r0, #4
	and	r6, r6, #15
	bl	dprint_vfmw
	ldrb	r2, [r4, #3]	@ zero_extendqisi2
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	mov	r0, #4
	and	r2, r2, #31
	ldr	r1, .L79+8
	mov	r3, r3, asl #31
	orr	r2, r3, r2, asl #26
	str	r2, [r5, #4]
	bl	dprint_vfmw
	ldrb	r1, [r4]	@ zero_extendqisi2
	ldrb	r3, [r4, #15]	@ zero_extendqisi2
	cmp	r1, #2
	streqb	r7, [r4, #12]
	ldrb	lr, [r4, #14]	@ zero_extendqisi2
	and	r3, r3, #1
	moveq	r10, r7
	add	r7, r4, #12288
	ldrb	ip, [r4, #13]	@ zero_extendqisi2
	and	lr, lr, #1
	ldr	r2, [r7, #2340]
	and	r1, r1, #3
	ldr	r0, [r7, #2344]
	and	ip, ip, #1
	ldr	r8, [r7, #2348]
	and	r2, r2, #3
	ldrneb	r10, [r4, #12]	@ zero_extendqisi2
	orr	r2, r3, r2, asl #26
	and	r8, r8, #3
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	orr	r2, r2, r0, asl #30
	ldrb	r0, [r4, #11]	@ zero_extendqisi2
	orr	r2, r2, r8, asl #28
	orr	r2, r2, lr, asl #1
	ldrb	r8, [r4, #9]	@ zero_extendqisi2
	and	r0, r0, #1
	orr	r2, r2, ip, asl #2
	mov	r3, r3, asl #7
	ldrb	lr, [r4, #7]	@ zero_extendqisi2
	orr	r0, r2, r0, asl #4
	and	r8, r8, #7
	uxtb	ip, r3
	orr	r1, r0, r1, asl #5
	ldrb	r0, [r4, #8]	@ zero_extendqisi2
	orr	r1, r1, ip
	and	r3, lr, #7
	ldrb	ip, [r4, #6]	@ zero_extendqisi2
	orr	r1, r1, r8, asl #8
	and	r0, r0, #7
	orr	r3, r1, r3, asl #11
	and	r2, ip, #31
	andne	r10, r10, #1
	orr	r3, r3, r0, asl #14
	orr	r2, r3, r2, asl #17
	ldr	r1, .L79+12
	movne	r10, r10, asl #3
	orr	r6, r2, r6, asl #22
	orr	r2, r6, r10
	mov	r0, #4
	str	r2, [r5, #8]
	bl	dprint_vfmw
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L73
.L42:
	ldrh	r3, [r4, #164]
	mov	r0, #4
	ldrh	r2, [r4, #166]
	sub	r3, r3, #1
	ldr	r1, .L79+16
	sub	r2, r2, #1
	uxth	r3, r3
	orr	r2, r3, r2, asl #16
	str	r2, [r5, #16]
	bl	dprint_vfmw
	ldr	r2, [r7, #2332]
	ldr	r3, [r7, #2336]
	mov	r0, #4
	ldr	r1, .L79+20
	orr	r2, r3, r2, asl #16
	str	r2, [r5, #20]
	bl	dprint_vfmw
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L74
.L45:
	mov	r6, #0
	ldr	r1, .L79+24
	mov	r2, r6
	str	r6, [r5, #32]
	mov	r0, #4
	movw	r8, #1228
	bl	dprint_vfmw
	mov	r2, r6
	ldr	r1, .L79+28
	mov	r0, #4
	str	r6, [r5, #36]
	bl	dprint_vfmw
	mov	r2, r6
	ldr	r1, .L79+32
	mov	r0, #4
	str	r6, [r5, #40]
	bl	dprint_vfmw
	mov	r2, r6
	ldr	r1, .L79+36
	mov	r0, #4
	str	r6, [r5, #44]
	bl	dprint_vfmw
	ldr	r2, [r7, #2304]
	ldr	r1, .L79+40
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #48]
	bl	dprint_vfmw
	ldr	r2, [r4, #220]
	ldr	r1, .L79+44
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #52]
	bl	dprint_vfmw
	ldr	r2, [r4, #224]
	ldr	r1, .L79+48
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #56]
	bl	dprint_vfmw
	ldr	r2, [r4, #228]
	ldr	r1, .L79+52
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #60]
	bl	dprint_vfmw
	ldr	r2, [r4, #232]
	ldr	r1, .L79+56
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #64]
	bl	dprint_vfmw
	ldr	r3, .L79
	ldr	r1, .L79+60
	mov	r0, #4
	mla	r8, r8, r9, r3
	ldr	r2, [r8, #1156]
	bic	r2, r2, #15
	str	r2, [r5, #68]
	bl	dprint_vfmw
	ldr	r2, [r8, #1148]
	ldr	r1, .L79+64
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #72]
	bl	dprint_vfmw
	ldrb	r3, [r4]	@ zero_extendqisi2
	strb	r6, [r4, #157]
	cmp	r3, #3
	beq	.L75
.L51:
	ldrb	r3, [r4, #153]	@ zero_extendqisi2
	and	lr, r3, #1
	strb	r3, [r4, #152]
	mov	r3, lr, asl #3
.L53:
	ldrb	r2, [r4, #156]	@ zero_extendqisi2
	mov	r0, #4
	ldrb	ip, [r4, #155]	@ zero_extendqisi2
	and	r2, r2, #1
	ldr	r1, .L79+68
	and	ip, ip, #1
	orr	r2, ip, r2, asl #1
	orr	r3, r2, r3
	orr	r2, r3, lr, asl #2
	str	r2, [r5, #76]
	bl	dprint_vfmw
	ldr	r2, [r7, #2352]
	ldr	r1, .L79+72
	mov	r0, #4
	str	r2, [r5, #80]
	bl	dprint_vfmw
	ldr	r2, [r7, #2356]
	ldr	r1, .L79+76
	mov	r0, #4
	str	r2, [r5, #84]
	bl	dprint_vfmw
	ldr	r2, [r7, #2360]
	ldr	r1, .L79+80
	mov	r0, #4
	str	r2, [r5, #88]
	bl	dprint_vfmw
	ldr	r2, [r7, #2364]
	ldr	r1, .L79+84
	mov	r0, #4
	str	r2, [r5, #92]
	bl	dprint_vfmw
	ldr	r2, [r7, #2368]
	ldr	r1, .L79+88
	mov	r0, #4
	str	r2, [r5, #96]
	bl	dprint_vfmw
	ldr	r2, [r7, #2372]
	ldr	r1, .L79+92
	mov	r0, #4
	str	r2, [r5, #100]
	bl	dprint_vfmw
	ldr	r2, [r7, #2376]
	ldr	r1, .L79+96
	mov	r0, #4
	str	r2, [r5, #104]
	bl	dprint_vfmw
	ldr	r2, [r7, #2380]
	ldr	r1, .L79+100
	mov	r0, #4
	str	r2, [r5, #108]
	bl	dprint_vfmw
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L76
.L54:
	ldr	r2, .L79
	movw	r3, #1228
	ldr	r1, .L79+104
	mov	r0, #4
	mla	r9, r3, r9, r2
	ldr	r3, [r9, #1144]
	bic	r3, r3, #15
	str	r3, [r5, #240]
	ldr	r2, [r9, #56]
	add	r2, r2, #256
	str	r2, [r5, #252]
	bl	dprint_vfmw
	mov	r0, #0
.L40:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L72:
	mov	r8, #4
	ldr	r1, .L79+4
	mov	r2, r8
	str	r8, [r5]
	mov	r0, r8
	add	r7, r4, #12288
	bl	dprint_vfmw
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	ldrb	ip, [r4, #3]	@ zero_extendqisi2
	mov	r0, r8
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	and	r2, r2, #127
	and	ip, ip, #31
	ldr	r1, .L79+8
	mov	r2, r2, asl #5
	sub	r3, r3, #1
	orr	r2, r2, ip, asl #26
	and	r3, r3, #31
	orr	r2, r2, r3
	str	r2, [r5, #4]
	bl	dprint_vfmw
	ldrb	r3, [r4]	@ zero_extendqisi2
	and	r2, r6, #15
	mov	r0, r8
	and	r3, r3, #3
	ldr	r1, .L79+12
	mov	r3, r3, asl #5
	orr	r2, r3, r2, asl #22
	str	r2, [r5, #8]
	bl	dprint_vfmw
	b	.L42
.L76:
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L54
	mov	r10, #0
	mov	r3, r4
	str	r4, [fp, #-48]
	mov	r4, r10
	mov	r10, r3
.L55:
	add	r8, r4, #112
	mov	r6, r10
	add	r8, r5, r8
	mov	r7, #0
.L56:
	ldrb	r3, [r6, #56]	@ zero_extendqisi2
	add	r2, r7, r7, lsr #31
	ldrb	ip, [r6, #40]	@ zero_extendqisi2
	add	r7, r7, #1
	ldrb	r1, [r6, #24]	@ zero_extendqisi2
	add	r2, r4, r2, asr #1
	ldrb	r0, [r6, #72]	@ zero_extendqisi2
	mov	r3, r3, asl #16
	orr	r3, r3, ip, asl #8
	add	r2, r2, #24
	orr	r3, r3, r1
	ldr	r1, .L79+108
	orr	r3, r3, r0, asl #24
	mov	r0, #4
	str	r3, [r8], #4
	add	r6, r6, #8
	bl	dprint_vfmw
	cmp	r7, #2
	bne	.L56
	add	r4, r4, #8
	add	r10, r10, #1
	cmp	r4, #64
	bne	.L55
	ldr	r4, [fp, #-48]
	mov	r10, #0
.L57:
	add	r8, r10, #176
	mov	r6, r4
	add	r8, r5, r8
	mov	r7, #0
.L58:
	ldrb	r3, [r6, #120]	@ zero_extendqisi2
	add	r2, r7, r7, lsr #31
	ldrb	ip, [r6, #104]	@ zero_extendqisi2
	add	r7, r7, #1
	ldrb	r1, [r6, #88]	@ zero_extendqisi2
	add	r2, r10, r2, asr #1
	ldrb	r0, [r6, #136]	@ zero_extendqisi2
	mov	r3, r3, asl #16
	orr	r3, r3, ip, asl #8
	add	r2, r2, #24
	orr	r3, r3, r1
	ldr	r1, .L79+108
	orr	r3, r3, r0, asl #24
	mov	r0, #4
	str	r3, [r8], #4
	add	r6, r6, #8
	bl	dprint_vfmw
	cmp	r7, #2
	bne	.L58
	add	r10, r10, #8
	add	r4, r4, #1
	cmp	r10, #64
	bne	.L57
	b	.L54
.L75:
	ldr	r1, [r7, #2344]
	cmp	r1, #1
	beq	.L77
	bhi	.L51
	ldrb	r3, [r4, #152]	@ zero_extendqisi2
	ldrb	r2, [r4, #153]	@ zero_extendqisi2
	and	r3, r3, #1
	and	lr, r2, #1
	mov	r3, r3, asl #3
	b	.L53
.L74:
	ldr	r3, [r7, #2344]
	cmp	r3, #1
	bne	.L45
	ldrb	r6, [r4, #154]	@ zero_extendqisi2
	cmp	r6, #1
	beq	.L78
	ldr	r2, [r7, #2368]
	mov	r0, #4
	ldrb	r1, [r4, #11]	@ zero_extendqisi2
	mov	r1, r2, asl r1
	ldr	r2, [r7, #2348]
	cmp	r1, #0
	mov	r3, r3, asl r2
	add	r3, r1, r3, asr #1
	ldr	r1, .L79+112
	suble	r3, r3, #1
	mov	r2, r3, asr r2
	str	r2, [r5, #24]
	bl	dprint_vfmw
	ldr	r3, [r7, #2372]
	ldrb	r2, [r4, #11]	@ zero_extendqisi2
	mov	r3, r3, asl r2
	cmp	r3, #0
	ble	.L49
	ldr	r1, [r7, #2348]
	mov	r2, #1
	mov	r2, r2, asl r1
	add	r3, r3, r2, asr #1
	mov	r2, r3, asr r1
.L50:
	str	r2, [r5, #28]
	mov	r0, #4
	ldr	r1, .L79+116
	bl	dprint_vfmw
	b	.L45
.L73:
	ldr	r3, [r4, #204]
	mov	r0, #4
	ldr	r2, [r4, #208]
	ldr	r1, .L79+120
	mov	r3, r3, asl #1
	uxth	r3, r3
	orr	r2, r3, r2, asl #17
	str	r2, [r5, #12]
	bl	dprint_vfmw
	b	.L42
.L59:
	mov	r6, r3
	b	.L37
.L77:
	ldrb	r2, [r4, #153]	@ zero_extendqisi2
	mov	r3, #8
	strb	r1, [r4, #152]
	and	lr, r2, #1
	b	.L53
.L49:
	ldr	r2, [r7, #2348]
	mov	r1, #1
	mov	r1, r1, asl r2
	add	r3, r3, r1, asr #1
	sub	r3, r3, #1
	mov	r2, r3, asr r2
	b	.L50
.L78:
	ldr	r3, [r7, #2348]
	ldrb	r1, [r4, #11]	@ zero_extendqisi2
	ldr	r0, [r7, #2368]
	rsb	r1, r1, r3
	mov	r1, r6, asl r1
	bl	__aeabi_idiv
	ldr	r1, .L79+112
	mov	r2, r0
	str	r0, [r5, #24]
	mov	r0, #4
	bl	dprint_vfmw
	ldr	r3, [r7, #2348]
	ldrb	r1, [r4, #11]	@ zero_extendqisi2
	ldr	r0, [r7, #2372]
	rsb	r1, r1, r3
	mov	r1, r6, asl r1
	bl	__aeabi_idiv
	ldr	r1, .L79+116
	mov	r2, r0
	str	r0, [r5, #28]
	mov	r0, #4
	bl	dprint_vfmw
	b	.L45
.L71:
	movw	r2, #333
	ldr	r1, .L79+124
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L40
.L80:
	.align	2
.L79:
	.word	g_HwMem
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC18
	.word	.LC19
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.word	.LC25
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.word	.LC38
	.word	.LC39
	.word	.LC40
	.word	.LC41
	.word	.LC43
	.word	.LC42
	.word	.LC20
	.word	.LC21
	.word	.LC17
	.word	.LC13
	UNWIND(.fnend)
	.size	MP4HAL_V400R003_CfgDnMsg, .-MP4HAL_V400R003_CfgDnMsg
	.align	2
	.global	MP4HAL_V400R003_WriteSlicMsg
	.type	MP4HAL_V400R003_WriteSlicMsg, %function
MP4HAL_V400R003_WriteSlicMsg:
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
	UNWIND(.pad #36)
	sub	sp, sp, #36
	ldr	ip, .L108
	movw	r3, #1228
	mov	lr, r0
	str	r0, [fp, #-68]
	add	r0, r0, #12288
	mla	r1, r3, r1, ip
	ldr	r7, [r0, #2288]
	str	r2, [fp, #-60]
	mov	r6, #0
	add	r10, lr, #240
	str	r6, [fp, #-48]
	ldr	r4, [r1, #56]
	add	r5, r4, #256
	mov	r0, r5
	bl	MEM_Phy2Vir
	subs	r8, r0, #0
	beq	.L106
	ldr	r3, [fp, #-68]
	ldr	r3, [r3, #264]
	cmp	r3, #0
	bne	.L107
.L84:
	cmp	r7, #0
	ble	.L101
	mov	r3, r3, asl #5
	mov	r4, #0
	str	r3, [fp, #-64]
	add	r3, r3, r5
	str	r3, [fp, #-72]
.L100:
	mov	r5, r4, asl #5
	cmp	r4, #0
	sub	r5, r5, r4, asl #2
	add	r5, r10, r5
	ble	.L89
	ldr	r2, [r5, #24]
	ldr	r3, [r5, #-4]
	cmp	r2, r3
	bls	.L90
.L89:
	ldr	r3, [r5, #16]
	mov	r9, #0
	ldrb	r2, [r5, #6]	@ zero_extendqisi2
	mov	r0, r9
	and	ip, r3, #15
	ldr	lr, [r5, #8]
	str	r3, [fp, #-56]
	mov	r1, #0
	add	r2, r2, ip, lsl #3
	bfi	r0, lr, #0, #24
	bfi	r1, r2, #0, #7
	str	r0, [fp, #-48]
	ldr	r3, [fp, #-64]
	mov	r0, #4
	strb	r1, [fp, #-45]
	ldr	r2, [fp, #-48]
	add	r6, r3, r4, lsl #3
	ldr	r1, .L108+4
	add	r4, r4, #1
	str	r2, [r8, r6, asl #2]
	add	r6, r6, #1
	bl	dprint_vfmw
	ldr	r3, [fp, #-56]
	ldr	r2, [fp, #-60]
	mov	ip, r9
	bic	r3, r3, #15
	ldr	r1, .L108+8
	rsb	r3, r2, r3
	mov	r0, #4
	bfi	ip, r3, #0, #24
	str	ip, [fp, #-48]
	str	ip, [r8, r6, asl #2]
	mov	r6, r6, asl #2
	mov	r2, ip
	bl	dprint_vfmw
	ldr	r3, [r5, #20]
	ldrb	r2, [r5, #7]	@ zero_extendqisi2
	mov	r0, r9
	and	ip, r3, #15
	ldr	r1, [r5, #12]
	str	r3, [fp, #-56]
	add	r2, r2, ip, lsl #3
	bfi	r0, r1, #0, #24
	mov	r1, #0
	str	r0, [fp, #-48]
	bfi	r1, r2, #0, #7
	strb	r1, [fp, #-45]
	ldr	r2, [fp, #-48]
	add	ip, r6, #4
	ldr	r1, .L108+12
	mov	r0, #4
	str	r2, [r8, ip]
	bl	dprint_vfmw
	ldr	r2, [r5, #20]
	ldr	r3, [fp, #-56]
	mov	r0, #4
	cmp	r2, r9
	ldr	r1, .L108+16
	bicne	r3, r3, #15
	streq	r2, [fp, #-48]
	ldrne	r2, [fp, #-60]
	rsbne	r3, r2, r3
	bfine	r9, r3, #0, #24
	strne	r9, [fp, #-48]
	ldr	r2, [fp, #-48]
	add	r3, r6, #8
	mov	r9, #0
	str	r2, [r8, r3]
	bl	dprint_vfmw
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	ldrb	r2, [r5, #4]	@ zero_extendqisi2
	add	ip, r6, #12
	and	r3, r3, #31
	str	r9, [fp, #-48]
	bfi	r3, r2, #5, #2
	strb	r3, [fp, #-48]
	ldrb	r3, [r5, #3]	@ zero_extendqisi2
	mov	r0, #4
	ldrh	r2, [fp, #-48]
	ldrb	r1, [r5, #1]	@ zero_extendqisi2
	bfi	r2, r3, #7, #3
	ldrb	lr, [r5, #2]	@ zero_extendqisi2
	strh	r2, [fp, #-48]	@ movhi
	mov	r3, r2, lsr #8
	bfi	r3, r1, #2, #3
	ldr	r1, .L108+20
	bfi	r3, lr, #5, #3
	strb	r3, [fp, #-47]
	ldr	r2, [fp, #-48]
	str	r2, [r8, ip]
	bl	dprint_vfmw
	ldr	r1, [r5, #24]
	add	r3, r6, #16
	mov	r2, r9
	mov	r0, #4
	bfi	r2, r1, #0, #20
	ldr	r1, .L108+24
	str	r2, [fp, #-48]
	str	r2, [r8, r3]
	bl	dprint_vfmw
	cmp	r7, r4
	ble	.L93
	mov	r3, r4, asl #5
	ldr	r2, [r5, #24]
	sub	r3, r3, r4, asl #2
	add	r1, r10, r3
	ldr	r1, [r1, #24]
	cmp	r2, r1
	bcc	.L93
	sub	r3, r3, #28
	add	r3, r10, r3
	b	.L94
.L96:
	ldr	r1, [r3, #52]
	cmp	r1, r2
	bhi	.L98
.L94:
	add	r4, r4, #1
	add	r3, r3, #28
	cmp	r7, r4
	bne	.L96
.L102:
	ldr	r3, [fp, #-68]
	mov	r9, #0
	mov	r5, r9
	ldrh	r1, [r3, #164]
	ldrh	r3, [r3, #166]
	mul	r1, r3, r1
	sub	r1, r1, #1
.L99:
	add	r3, r6, #20
	add	r6, r6, #24
	mov	r2, #0
	mov	r0, #4
	bfi	r2, r1, #0, #20
	ldr	r1, .L108+28
	str	r2, [fp, #-48]
	sub	r4, r4, #1
	str	r2, [r8, r3]
	bl	dprint_vfmw
	mov	r2, r5
	ldr	r1, .L108+32
	mov	r0, #4
	str	r9, [r8, r6]
	str	r5, [fp, #-48]
	bl	dprint_vfmw
.L90:
	add	r4, r4, #1
	cmp	r7, r4
	bgt	.L100
.L101:
	mov	r0, #0
.L83:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L93:
	cmp	r7, r4
	beq	.L102
.L98:
	mov	r5, r4, asl #5
	ldr	r2, [fp, #-72]
	sub	r3, r5, r4, asl #2
	add	r3, r10, r3
	add	r5, r5, r2
	ldr	r1, [r3, #24]
	mov	r9, r5
	sub	r1, r1, #1
	b	.L99
.L107:
	ldr	r2, [fp, #-68]
	mov	r0, #4
	ldr	r3, [fp, #-48]
	add	r4, r4, #288
	ldr	r9, [r2, #256]
	bfi	r3, r6, #0, #24
	ldrb	r2, [r2, #246]	@ zero_extendqisi2
	and	r1, r9, #15
	str	r3, [fp, #-48]
	mov	r3, r3, lsr #24
	add	r2, r2, r1, lsl #3
	ldr	r1, .L108+4
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r8]
	bl	dprint_vfmw
	ldr	r1, [fp, #-60]
	bic	r3, r9, #15
	mov	r2, r6
	rsb	r3, r1, r3
	mov	r0, #4
	bfi	r2, r3, #0, #24
	ldr	r1, .L108+8
	str	r2, [fp, #-48]
	str	r2, [r8, #4]
	bl	dprint_vfmw
	ldr	r3, [fp, #-68]
	mov	r2, #0
	ldr	r1, .L108+12
	mov	r0, #4
	ldr	r9, [r3, #260]
	ldrb	r3, [r3, #247]	@ zero_extendqisi2
	and	ip, r9, #15
	str	r6, [fp, #-48]
	add	r3, r3, ip, lsl #3
	bfi	r2, r3, #0, #7
	strb	r2, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r8, #8]
	bl	dprint_vfmw
	ldr	r3, [fp, #-68]
	ldr	r1, .L108+16
	mov	r0, #4
	ldr	r3, [r3, #260]
	cmp	r3, #0
	bicne	r3, r9, #15
	streq	r3, [fp, #-48]
	ldrne	r2, [fp, #-60]
	rsbne	r3, r2, r3
	bfine	r6, r3, #0, #24
	strne	r6, [fp, #-48]
	ldr	r2, [fp, #-48]
	mov	r6, #0
	str	r2, [r8, #12]
	bl	dprint_vfmw
	ldr	r9, [fp, #-68]
	str	r6, [fp, #-48]
	mov	r0, #4
	ldr	r1, .L108+20
	ldrb	r3, [r9, #245]	@ zero_extendqisi2
	ldrb	r2, [r9, #244]	@ zero_extendqisi2
	and	r3, r3, #31
	ldrb	ip, [r9, #241]	@ zero_extendqisi2
	bfi	r3, r2, #5, #2
	ldrb	r2, [r9, #243]	@ zero_extendqisi2
	strb	r3, [fp, #-48]
	ldrh	r3, [fp, #-48]
	bfi	r3, r2, #7, #3
	ldrb	r2, [r9, #242]	@ zero_extendqisi2
	strh	r3, [fp, #-48]	@ movhi
	mov	r3, r3, lsr #8
	bfi	r3, ip, #2, #3
	bfi	r3, r2, #5, #3
	strb	r3, [fp, #-47]
	ldr	r2, [fp, #-48]
	str	r2, [r8, #16]
	bl	dprint_vfmw
	ldr	r1, .L108+24
	mov	r2, r6
	mov	r0, #4
	bfi	r2, r6, #0, #20
	str	r2, [fp, #-48]
	str	r2, [r8, #20]
	bl	dprint_vfmw
	ldr	r3, [r9, #264]
	ldr	r1, .L108+28
	mov	r2, r6
	sub	r3, r3, #1
	mov	r0, #4
	bfi	r2, r3, #0, #20
	str	r2, [fp, #-48]
	str	r2, [r8, #24]
	bl	dprint_vfmw
	ldr	r1, .L108+32
	mov	r0, #4
	str	r4, [r8, #28]
	mov	r2, r4
	str	r4, [fp, #-48]
	bl	dprint_vfmw
	mov	r3, #1
	b	.L84
.L106:
	ldr	r2, .L108+36
	ldr	r1, .L108+40
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L83
.L109:
	.align	2
.L108:
	.word	g_HwMem
	.word	.LC45
	.word	.LC46
	.word	.LC47
	.word	.LC48
	.word	.LC49
	.word	.LC50
	.word	.LC51
	.word	.LC52
	.word	.LANCHOR0+24
	.word	.LC44
	UNWIND(.fnend)
	.size	MP4HAL_V400R003_WriteSlicMsg, .-MP4HAL_V400R003_WriteSlicMsg
	.align	2
	.global	MP4HAL_V400R003_StartDec
	.type	MP4HAL_V400R003_StartDec, %function
MP4HAL_V400R003_StartDec:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
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
	ldrh	ip, [r0, #164]
	mov	r6, #0
	mov	r4, r0
	cmp	ip, #512
	mov	r5, r1
	str	r6, [fp, #-32]
	bhi	.L113
	ldrh	ip, [r0, #166]
	cmp	ip, #512
	bhi	.L113
	mov	r3, r2
	sub	r2, fp, #32
	bl	MP4HAL_V400R003_CfgReg
	subs	r7, r0, #0
	bne	.L117
	mov	r1, r5
	mov	r0, r4
	bl	MP4HAL_V400R003_CfgDnMsg
	subs	r6, r0, #0
	bne	.L118
	mov	r1, r5
	mov	r0, r4
	ldr	r2, [fp, #-32]
	bl	MP4HAL_V400R003_WriteSlicMsg
	cmp	r0, #0
	bne	.L119
.L112:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L113:
	ldr	r3, .L120
	mov	r0, #0
	ldr	r2, .L120+4
	ldr	r1, .L120+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L112
.L117:
	mov	r0, r6
	ldr	r1, .L120+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L112
.L118:
	mov	r0, r7
	ldr	r1, .L120+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L112
.L119:
	mov	r0, r6
	ldr	r1, .L120+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L112
.L121:
	.align	2
.L120:
	.word	.LC53
	.word	.LANCHOR0+56
	.word	.LC54
	.word	.LC55
	.word	.LC56
	.word	.LC57
	UNWIND(.fnend)
	.size	MP4HAL_V400R003_StartDec, .-MP4HAL_V400R003_StartDec
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.14686, %object
	.size	__func__.14686, 23
__func__.14686:
	.ascii	"MP4HAL_V400R003_CfgReg\000"
	.space	1
	.type	__func__.14737, %object
	.size	__func__.14737, 29
__func__.14737:
	.ascii	"MP4HAL_V400R003_WriteSlicMsg\000"
	.space	3
	.type	__func__.14674, %object
	.size	__func__.14674, 25
__func__.14674:
	.ascii	"MP4HAL_V400R003_StartDec\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC1:
	ASCII(.ascii	"stream_base_addr = %#x\012\000" )
.LC2:
	ASCII(.ascii	"BASIC_V400R003_CFG0=0x%x\012\000" )
	.space	2
.LC3:
	ASCII(.ascii	"BASIC_V400R003_CFG1=0x%x\012\000" )
	.space	2
.LC4:
	ASCII(.ascii	"AVM_V400R003_ADDR=0x%x\012\000" )
.LC5:
	ASCII(.ascii	"VAM_V400R003_ADDR=0x%x\012\000" )
.LC6:
	ASCII(.ascii	"STREAM_V400R003_BASE_ADDR=0x%x\012\000" )
.LC7:
	ASCII(.ascii	"YSTADDR_V200R003_1D=0x%x\012\000" )
	.space	2
.LC8:
	ASCII(.ascii	"YSTRIDE_V200R003_1D=0x%x\012\000" )
	.space	2
.LC9:
	ASCII(.ascii	"UVOFFSET_V200R003_1D=0x%x\012\000" )
	.space	1
.LC10:
	ASCII(.ascii	"PRCNUM_1D_CNT=0x%x\012\000" )
.LC11:
	ASCII(.ascii	"PRCMEM2_1D_CNT=0x%x\012\000" )
	.space	3
.LC12:
	ASCII(.ascii	"DNR_MBINFO_STADDR=0x%x\012\000" )
.LC13:
	ASCII(.ascii	"line: %d ,pMsgBlock = NULL is not expected value!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC14:
	ASCII(.ascii	"D0=0x%x\012\000" )
	.space	3
.LC15:
	ASCII(.ascii	"D1=0x%x\012\000" )
	.space	3
.LC16:
	ASCII(.ascii	"D2=0x%x\012\000" )
	.space	3
.LC17:
	ASCII(.ascii	"D3=0x%x\012\000" )
	.space	3
.LC18:
	ASCII(.ascii	"D4=0x%x\012\000" )
	.space	3
.LC19:
	ASCII(.ascii	"D5=0x%x\012\000" )
	.space	3
.LC20:
	ASCII(.ascii	"D6=0x%x\012\000" )
	.space	3
.LC21:
	ASCII(.ascii	"D7=0x%x\012\000" )
	.space	3
.LC22:
	ASCII(.ascii	"D8=0x%x\012\000" )
	.space	3
.LC23:
	ASCII(.ascii	"D9=0x%x\012\000" )
	.space	3
.LC24:
	ASCII(.ascii	"D10=0x%x\012\000" )
	.space	2
.LC25:
	ASCII(.ascii	"D11=0x%x\012\000" )
	.space	2
.LC26:
	ASCII(.ascii	"D12= 0x%x\012\000" )
	.space	1
.LC27:
	ASCII(.ascii	"D13= 0x%x\012\000" )
	.space	1
.LC28:
	ASCII(.ascii	"D14= 0x%x\012\000" )
	.space	1
.LC29:
	ASCII(.ascii	"D15= 0x%x\012\000" )
	.space	1
.LC30:
	ASCII(.ascii	"D16= 0x%x\012\000" )
	.space	1
.LC31:
	ASCII(.ascii	"D17= 0x%x\012\000" )
	.space	1
.LC32:
	ASCII(.ascii	"D18= 0x%x\012\000" )
	.space	1
.LC33:
	ASCII(.ascii	"D19= 0x%x\012\000" )
	.space	1
.LC34:
	ASCII(.ascii	"D20= 0x%x\012\000" )
	.space	1
.LC35:
	ASCII(.ascii	"D21= 0x%x\012\000" )
	.space	1
.LC36:
	ASCII(.ascii	"D22= 0x%x\012\000" )
	.space	1
.LC37:
	ASCII(.ascii	"D23= 0x%x\012\000" )
	.space	1
.LC38:
	ASCII(.ascii	"D24= 0x%x\012\000" )
	.space	1
.LC39:
	ASCII(.ascii	"D25= 0x%x\012\000" )
	.space	1
.LC40:
	ASCII(.ascii	"D26= 0x%x\012\000" )
	.space	1
.LC41:
	ASCII(.ascii	"D27= 0x%x\012\000" )
	.space	1
.LC42:
	ASCII(.ascii	"D%d= 0x%x\012\000" )
	.space	1
.LC43:
	ASCII(.ascii	"D63= 0x%x\012\000" )
	.space	1
.LC44:
	ASCII(.ascii	"%s: SlcDnMsgVirAddr = NULL\012\000" )
.LC45:
	ASCII(.ascii	"D0 = %#x \012\000" )
	.space	1
.LC46:
	ASCII(.ascii	"D1 = %#x \012\000" )
	.space	1
.LC47:
	ASCII(.ascii	"D2 = %#x \012\000" )
	.space	1
.LC48:
	ASCII(.ascii	"D3 = %#x \012\000" )
	.space	1
.LC49:
	ASCII(.ascii	"D4 = %#x \012\000" )
	.space	1
.LC50:
	ASCII(.ascii	"D5 = %#x \012\000" )
	.space	1
.LC51:
	ASCII(.ascii	"D6 = %#x \012\000" )
	.space	1
.LC52:
	ASCII(.ascii	"D7 = %#x \012\000" )
	.space	1
.LC53:
	ASCII(.ascii	"picture width out of range\000" )
	.space	1
.LC54:
	ASCII(.ascii	"%s: %s\012\000" )
.LC55:
	ASCII(.ascii	"MP4HAL_V200R003_CfgReg failed!\012\000" )
.LC56:
	ASCII(.ascii	"MP4HAL_V200R003_CfgDnMsg failed!\012\000" )
	.space	2
.LC57:
	ASCII(.ascii	"MP4HAL_V200R003_WriteSlicMsg failed!\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
