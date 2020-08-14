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
	.global	MP4HAL_V300R001_Log2bin
	.type	MP4HAL_V300R001_Log2bin, %function
MP4HAL_V300R001_Log2bin:
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
	.size	MP4HAL_V300R001_Log2bin, .-MP4HAL_V300R001_Log2bin
	.align	2
	.global	MP4HAL_V300R001_InitHal
	.type	MP4HAL_V300R001_InitHal, %function
MP4HAL_V300R001_InitHal:
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
	.size	MP4HAL_V300R001_InitHal, .-MP4HAL_V300R001_InitHal
	.align	2
	.global	MP4HAL_V300R001_CfgReg
	.type	MP4HAL_V300R001_CfgReg, %function
MP4HAL_V300R001_CfgReg:
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
	add	r9, r0, #12288
	mov	r8, r0
	mov	r7, r1
	ldr	ip, [r9, #2288]
	cmp	ip, #0
	beq	.L9
	mov	r3, r0
	mvn	r4, #0
	mov	r0, #0
.L10:
	ldr	r1, [r3, #256]
	cmp	r1, #0
	beq	.L13
	ldr	lr, [r3, #248]
	bic	r1, r1, #15
	cmp	lr, #0
	beq	.L13
	cmp	r4, r1
	movcs	r4, r1
.L13:
	ldr	r1, [r3, #260]
	cmp	r1, #0
	beq	.L11
	ldr	lr, [r3, #252]
	bic	r1, r1, #15
	cmp	lr, #0
	beq	.L11
	cmp	r4, r1
	movcs	r4, r1
.L11:
	add	r0, r0, #1
	add	r3, r3, #28
	cmp	r0, ip
	bne	.L10
	cmn	r4, #1
	beq	.L9
	str	r4, [r2]
	cmp	r7, #0
	ldrh	r10, [r8, #164]
	ldrh	r3, [r8, #166]
	mul	r10, r3, r10
	sub	r10, r10, #1
	ubfx	r10, r10, #0, #20
	orr	r10, r10, #1107296256
	orr	r10, r10, #4194304
	ble	.L55
	ldr	r6, .L58
	mov	r3, r7
	mov	r2, #1
	ldr	r1, .L58+4
	str	r2, [sp]
	mov	r0, #32
	ldr	ip, [r6, #68]
	ldr	r2, .L58+8
	blx	ip
	mov	r2, r10
	ldr	r3, [r6, #68]
	mov	r0, #3
	ldr	r1, .L58+12
	blx	r3
	ldr	ip, [r9, #2316]
	ldr	r10, [r9, #2372]
	mov	r2, #1
	ldr	r5, .L58+16
	mov	r3, r7
	mov	ip, ip, lsr #6
	and	r10, r10, r2
	str	r2, [sp]
	mov	r0, #32
	mov	ip, ip, asl #4
	ldr	r2, .L58+8
	orr	ip, ip, #536870914
	ldr	r1, .L58+4
	orr	ip, ip, #114688
	orr	r10, ip, r10, asl #30
	ldr	ip, [r6, #68]
	blx	ip
	ldr	r3, [r6, #68]
	mov	r2, r10
	movw	r10, #1208
	mla	r10, r10, r7, r5
	ldr	r1, .L58+20
	mov	r0, #3
	blx	r3
	mov	r3, #1
	str	r3, [sp]
	mov	r0, #32
	mov	r3, r7
	ldr	r2, .L58+8
	ldr	ip, [r10, #48]
	ldr	r1, .L58+4
	bic	ip, ip, #15
	str	ip, [fp, #-48]
	ldr	ip, [r6, #68]
	blx	ip
	ldr	ip, [fp, #-48]
	ldr	r3, [r6, #68]
	mov	r0, #3
	ldr	r1, .L58+24
	mov	r2, ip
	blx	r3
	ldr	r10, [r10, #32]
	ldr	ip, [r6, #68]
	mov	r3, #1
	bic	r10, r10, #15
	str	r3, [sp]
	ldr	r2, .L58+8
	mov	r3, r7
	ldr	r1, .L58+4
	mov	r0, #32
	blx	ip
	mov	r2, r10
	ldr	r3, [r6, #68]
	mov	r0, #3
	ldr	r1, .L58+28
	blx	r3
	mov	r3, #1
	ldr	ip, [r6, #68]
	mov	r0, #32
	str	r3, [sp]
	mov	r3, r7
	ldr	r2, .L58+8
	ldr	r1, .L58+4
	blx	ip
.L18:
	ldr	r3, [r6, #68]
	mov	r2, r4
	ldr	r1, .L58+32
	mov	r0, #3
	blx	r3
	ldrh	r3, [r8, #164]
	cmp	r3, #120
	bls	.L56
	cmp	r7, #0
	beq	.L30
	cmp	r7, #1
	beq	.L37
.L26:
	cmp	r7, #0
	bgt	.L25
.L21:
	movw	r4, #1208
	movw	r3, #3075
	mul	r4, r4, r7
	movt	r3, 48
	ldr	r1, .L58+36
	mov	r0, #3
	ldr	r2, [r5, r4]
	str	r3, [r2, #60]
	ldr	r2, [r5, r4]
	str	r3, [r2, #64]
	ldr	r2, [r5, r4]
	str	r3, [r2, #68]
	ldr	r2, [r5, r4]
	str	r3, [r2, #72]
	ldr	r2, [r5, r4]
	str	r3, [r2, #76]
	ldr	r2, [r5, r4]
	str	r3, [r2, #80]
	ldr	r2, [r5, r4]
	str	r3, [r2, #84]
	ldr	r3, [r5, r4]
	ldr	r2, [r8, #212]
	bic	r2, r2, #15
	str	r2, [r3, #96]
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r5, r4]
	ldr	r2, [r9, #2316]
	mov	r0, #3
	ldrh	r10, [r8, #166]
	ldr	r1, .L58+40
	str	r2, [r3, #100]
	ldr	r3, [r6, #68]
	blx	r3
	ldrh	r3, [r8, #164]
	cmp	r3, #0
	mov	r3, r3, asl #4
	ble	.L28
	cmp	r3, #2048
	movle	r4, #16
	ble	.L29
.L28:
	sub	r2, r3, #2048
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r4, #32
	bcc	.L29
	sub	r2, r3, #4096
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r4, #48
	bcs	.L57
.L29:
	mov	r2, r10, asl #4
	movw	r0, #1208
	add	r2, r2, #31
	add	r3, r10, #1
	mul	r7, r0, r7
	mov	r2, r2, lsr #5
	ldr	ip, [r9, #2316]
	mov	r3, r3, lsr #1
	mul	r4, r2, r4
	ldr	r1, .L58+44
	mul	r3, ip, r3
	ldr	ip, [r5, r7]
	mov	r0, #3
	mov	r8, #0
	add	r9, r5, r7
	add	r3, r3, r4, lsl #4
	mov	r4, r4, asl #5
	mov	r2, r3, asl #1
	str	r2, [ip, #104]
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r5, r7]
	mov	r2, r8
	ldr	r1, .L58+48
	mov	r0, #3
	str	r4, [r3, #108]
	ldr	r3, [r5, r7]
	str	r8, [r3, #152]
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r9, #1184]
	ldr	ip, [r5, r7]
	mov	r0, #3
	ldr	r1, .L58+52
	mov	r2, r3
	str	r3, [ip, #144]
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r5, r7]
	mov	r0, r8
	mvn	r2, #0
	str	r2, [r3, #32]
.L52:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L56:
	cmp	r7, #0
	beq	.L32
	cmp	r7, #1
	bne	.L26
	mov	r4, #65536
	b	.L22
.L55:
	movw	r3, #1208
	ldr	r5, .L58+16
	mul	r3, r3, r7
	ldr	r6, .L58
	mov	r2, r10
	ldr	r1, .L58+12
	mov	r0, #3
	add	ip, r5, r3
	str	ip, [fp, #-52]
	ldr	lr, [r5, r3]
	str	r3, [fp, #-48]
	str	r10, [lr, #8]
	ldr	r10, [r6, #68]
	blx	r10
	ldr	r2, [r9, #2316]
	ldr	r3, [fp, #-48]
	mov	r0, #3
	ldr	r10, [r9, #2372]
	mov	r2, r2, lsr #6
	ldr	r1, .L58+20
	ldr	lr, [r5, r3]
	and	r10, r10, #1
	mov	r2, r2, asl #4
	orr	r2, r2, #536870914
	orr	r2, r2, #114688
	orr	r2, r2, r10, asl #30
	str	r2, [lr, #12]
	ldr	r10, [r6, #68]
	blx	r10
	ldr	r3, [fp, #-48]
	ldr	ip, [fp, #-52]
	mov	r0, #3
	ldr	r1, .L58+24
	ldr	lr, [r5, r3]
	ldr	r2, [ip, #48]
	bic	r2, r2, #15
	str	r2, [lr, #16]
	ldr	r10, [r6, #68]
	blx	r10
	ldr	ip, [fp, #-52]
	ldr	r3, [fp, #-48]
	mov	r0, #3
	ldr	r1, .L58+28
	ldr	r2, [ip, #32]
	ldr	ip, [r5, r3]
	bic	r2, r2, #15
	str	r2, [ip, #20]
	ldr	ip, [r6, #68]
	blx	ip
	ldr	r3, [fp, #-48]
	ldr	r3, [r5, r3]
	str	r4, [r3, #24]
	b	.L18
.L37:
	mov	r4, #0
.L22:
	movw	r0, #49156
	movt	r0, 63683
	bl	MEM_ReadPhyWord
	ldr	r3, .L58+56
	cmp	r4, #0
	biceq	r1, r0, #65536
	orrne	r1, r0, #65536
	ldr	r0, [r3]
	add	r0, r0, #4
	bl	MEM_WritePhyWord
.L25:
	mov	r0, #0
	ldr	ip, [r6, #68]
	mov	r3, r7
	str	r0, [sp]
	ldr	r2, .L58+8
	ldr	r1, .L58+60
	blx	ip
	mvn	r0, #0
	b	.L52
.L57:
	sub	r3, r3, #6144
	sub	r3, r3, #1
	cmp	r3, #2048
	movcs	r4, #16
	movcc	r4, #64
	b	.L29
.L9:
	ldr	r3, .L58
	mvn	r2, #0
	ldr	r1, .L58+64
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L52
.L32:
	movw	r0, #49156
	movt	r0, 63683
	bl	MEM_ReadPhyWord
	orr	r1, r0, #65536
.L31:
	ldr	r3, .L58+68
	ldr	r0, [r3]
	add	r0, r0, #4
	bl	MEM_WritePhyWord
	b	.L21
.L30:
	movw	r0, #49156
	movt	r0, 63683
	bl	MEM_ReadPhyWord
	bic	r1, r0, #65536
	b	.L31
.L59:
	.align	2
.L58:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC5
	.word	.LANCHOR0
	.word	.LC1
	.word	g_HwMem
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC6
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	s_RegPhyBaseAddr_1
	.word	.LC7
	.word	.LC0
	.word	s_RegPhyBaseAddr
	UNWIND(.fnend)
	.size	MP4HAL_V300R001_CfgReg, .-MP4HAL_V300R001_CfgReg
	.global	__aeabi_idiv
	.align	2
	.global	MP4HAL_V300R001_CfgDnMsg
	.type	MP4HAL_V300R001_CfgDnMsg, %function
MP4HAL_V300R001_CfgDnMsg:
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
	mov	r7, r0
	str	r1, [fp, #-48]
	mul	r3, r3, r2
	cmp	r3, #0
	beq	.L83
	mov	r6, #0
.L62:
	add	r6, r6, #1
	movs	r3, r3, lsr #1
	uxth	r6, r6
	bne	.L62
.L61:
	ldr	r2, [fp, #-48]
	movw	r3, #1208
	ldr	r1, .L102
	mla	r3, r3, r2, r1
	ldr	r0, [r3, #48]
	bl	MEM_Phy2Vir
	subs	r5, r0, #0
	beq	.L94
	ldrb	r3, [r7, #2]	@ zero_extendqisi2
	ldr	r4, .L102+4
	cmp	r3, #1
	beq	.L95
	mov	r8, #0
	ldr	r1, .L102+8
	mov	r2, r8
	str	r8, [r5]
	mov	r0, #4
	ldr	r3, [r4, #68]
	blx	r3
	ldrb	r2, [r7, #3]	@ zero_extendqisi2
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	mov	r0, #4
	and	r2, r2, #31
	ldr	r1, .L102+12
	and	r6, r6, #15
	mov	r3, r3, asl #31
	orr	r2, r3, r2, asl #26
	str	r2, [r5, #4]
	ldr	r3, [r4, #68]
	blx	r3
	ldrb	r2, [r7]	@ zero_extendqisi2
	ldrb	r1, [r7, #15]	@ zero_extendqisi2
	cmp	r2, #2
	streqb	r8, [r7, #12]
	and	r0, r1, #1
	ldrb	lr, [r7, #14]	@ zero_extendqisi2
	moveq	ip, r8
	add	r8, r7, #12288
	ldrb	r9, [r7, #13]	@ zero_extendqisi2
	and	lr, lr, #1
	ldr	r3, [r8, #2328]
	and	r2, r2, #3
	ldr	r10, [r8, #2336]
	and	r9, r9, #1
	and	r3, r3, #3
	ldrneb	ip, [r7, #12]	@ zero_extendqisi2
	and	r10, r10, #3
	orr	r1, r0, r3, asl #26
	ldr	r3, [r8, #2332]
	ldrb	r0, [r7, #11]	@ zero_extendqisi2
	orr	r3, r1, r3, asl #30
	and	r0, r0, #1
	orr	r1, r3, r10, asl #28
	ldrb	r3, [r7, #10]	@ zero_extendqisi2
	orr	r1, r1, lr, asl #1
	ldrb	r10, [r7, #9]	@ zero_extendqisi2
	orr	r9, r1, r9, asl #2
	ldrb	lr, [r7, #7]	@ zero_extendqisi2
	mov	r3, r3, asl #7
	orr	r9, r9, r0, asl #4
	orr	r2, r9, r2, asl #5
	ldrb	r0, [r7, #8]	@ zero_extendqisi2
	uxtb	r1, r3
	and	r10, r10, #7
	ldrb	r9, [r7, #6]	@ zero_extendqisi2
	orr	r2, r2, r1
	and	lr, lr, #7
	orr	r10, r2, r10, asl #8
	and	r0, r0, #7
	and	r2, r9, #31
	orr	r3, r10, lr, asl #11
	ldr	r1, .L102+16
	orr	r3, r3, r0, asl #14
	mov	r0, #4
	orr	r2, r3, r2, asl #17
	ldr	r9, .L102+4
	orr	r6, r2, r6, asl #22
	andne	ip, ip, #1
	movne	ip, ip, asl #3
	orr	r2, r6, ip
	str	r2, [r5, #8]
	ldr	r3, [r4, #68]
	blx	r3
	ldrb	r3, [r7]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L96
.L66:
	ldrh	r3, [r7, #164]
	mov	r0, #4
	ldrh	r2, [r7, #166]
	sub	r3, r3, #1
	ldr	r1, .L102+20
	sub	r2, r2, #1
	ldr	r6, .L102+4
	uxth	r3, r3
	orr	r2, r3, r2, asl #16
	str	r2, [r5, #16]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r8, #2320]
	ldr	r3, [r8, #2324]
	mov	r0, #4
	ldr	r1, .L102+24
	orr	r2, r3, r2, asl #16
	str	r2, [r5, #20]
	ldr	r3, [r4, #68]
	blx	r3
	ldrb	r3, [r7]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L97
.L69:
	mov	r6, #0
	ldr	r1, .L102+28
	str	r6, [r5, #32]
	mov	r2, r6
	ldr	r3, [r4, #68]
	mov	r0, #4
	blx	r3
	str	r6, [r5, #36]
	mov	r2, r6
	ldr	r3, [r4, #68]
	ldr	r1, .L102+32
	mov	r0, #4
	blx	r3
	str	r6, [r5, #40]
	mov	r2, r6
	ldr	r3, [r4, #68]
	ldr	r1, .L102+36
	mov	r0, #4
	blx	r3
	str	r6, [r5, #44]
	mov	r2, r6
	ldr	r3, [r4, #68]
	ldr	r1, .L102+40
	mov	r0, #4
	blx	r3
	ldr	r2, [r8, #2304]
	ldr	r1, .L102+44
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #48]
	ldr	r3, [r4, #68]
	movw	r9, #1208
	blx	r3
	ldr	r2, [r7, #220]
	ldr	r1, .L102+48
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #52]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r7, #224]
	ldr	r1, .L102+52
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #56]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r7, #228]
	ldr	r1, .L102+56
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #60]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r7, #232]
	ldr	r1, .L102+60
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #64]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r3, [fp, #-48]
	ldr	r2, .L102
	mov	r0, #4
	ldr	r1, .L102+64
	mla	r9, r9, r3, r2
	ldr	r2, [r9, #1148]
	bic	r2, r2, #15
	str	r2, [r5, #68]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r9, #1140]
	ldr	r3, [r4, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L102+68
	str	r2, [r5, #72]
	blx	r3
	ldrb	r3, [r7]	@ zero_extendqisi2
	strb	r6, [r7, #157]
	cmp	r3, #3
	beq	.L98
.L75:
	ldrb	r3, [r7, #153]	@ zero_extendqisi2
	and	lr, r3, #1
	strb	r3, [r7, #152]
	mov	r3, lr, asl #3
.L77:
	ldrb	r6, [r7, #156]	@ zero_extendqisi2
	mov	r0, #4
	ldrb	r2, [r7, #155]	@ zero_extendqisi2
	and	r6, r6, #1
	ldr	ip, [r4, #68]
	and	r2, r2, #1
	ldr	r1, .L102+72
	orr	r2, r2, r6, asl #1
	orr	r3, r2, r3
	orr	r2, r3, lr, asl #2
	str	r2, [r5, #76]
	blx	ip
	ldr	r2, [r8, #2340]
	ldr	r3, [r4, #68]
	mov	r0, #4
	ldr	r1, .L102+76
	str	r2, [r5, #80]
	blx	r3
	ldr	r2, [r8, #2344]
	ldr	r3, [r4, #68]
	mov	r0, #4
	ldr	r1, .L102+80
	str	r2, [r5, #84]
	blx	r3
	ldr	r2, [r8, #2348]
	ldr	r3, [r4, #68]
	mov	r0, #4
	ldr	r1, .L102+84
	str	r2, [r5, #88]
	blx	r3
	ldr	r2, [r8, #2352]
	ldr	r3, [r4, #68]
	mov	r0, #4
	ldr	r1, .L102+88
	str	r2, [r5, #92]
	blx	r3
	ldr	r2, [r8, #2356]
	ldr	r3, [r4, #68]
	mov	r0, #4
	ldr	r1, .L102+92
	str	r2, [r5, #96]
	blx	r3
	ldr	r2, [r8, #2360]
	ldr	r3, [r4, #68]
	mov	r0, #4
	ldr	r1, .L102+96
	str	r2, [r5, #100]
	blx	r3
	ldr	r2, [r8, #2364]
	ldr	r3, [r4, #68]
	mov	r0, #4
	ldr	r1, .L102+100
	str	r2, [r5, #104]
	blx	r3
	ldr	r2, [r8, #2368]
	ldr	r3, [r4, #68]
	mov	r0, #4
	ldr	r1, .L102+104
	str	r2, [r5, #108]
	blx	r3
	ldrb	r3, [r7, #2]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L99
.L78:
	ldr	r2, [fp, #-48]
	movw	r3, #1208
	ldr	r1, .L102
	mov	r0, #4
	ldr	ip, [r4, #68]
	mla	r3, r3, r2, r1
	ldr	r1, .L102+108
	ldr	r2, [r3, #1136]
	bic	r2, r2, #15
	str	r2, [r5, #240]
	ldr	r2, [r3, #48]
	add	r2, r2, #256
	str	r2, [r5, #252]
	blx	ip
	mov	r0, #0
.L64:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L95:
	mov	r9, #4
	ldr	r1, .L102+8
	mov	r2, r9
	str	r9, [r5]
	mov	r0, r9
	ldr	r3, [r4, #68]
	blx	r3
	ldrb	ip, [r7, #4]	@ zero_extendqisi2
	ldrb	r2, [r7, #3]	@ zero_extendqisi2
	mov	r0, r9
	ldrb	r3, [r7, #5]	@ zero_extendqisi2
	and	ip, ip, #127
	and	r2, r2, #31
	ldr	r1, .L102+12
	mov	ip, ip, asl #5
	sub	r3, r3, #1
	orr	r2, ip, r2, asl #26
	and	r3, r3, #31
	orr	r2, r2, r3
	str	r2, [r5, #4]
	ldr	r3, [r4, #68]
	add	r8, r7, #12288
	blx	r3
	ldrb	r3, [r7]	@ zero_extendqisi2
	and	r2, r6, #15
	mov	r0, r9
	and	r3, r3, #3
	ldr	r1, .L102+16
	mov	r3, r3, asl #5
	orr	r2, r3, r2, asl #22
	str	r2, [r5, #8]
	ldr	r3, [r4, #68]
	blx	r3
	b	.L66
.L99:
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L78
	mov	r10, r7
	mov	r9, #0
	str	r7, [fp, #-52]
.L79:
	add	r8, r9, #112
	mov	r6, r10
	add	r8, r5, r8
	mov	r7, #0
.L80:
	ldrb	r3, [r6, #56]	@ zero_extendqisi2
	add	r2, r7, r7, lsr #31
	ldrb	ip, [r6, #40]	@ zero_extendqisi2
	add	r7, r7, #1
	ldrb	r1, [r6, #24]	@ zero_extendqisi2
	add	r2, r9, r2, asr #1
	ldrb	r0, [r6, #72]	@ zero_extendqisi2
	mov	r3, r3, asl #16
	orr	r3, r3, ip, asl #8
	add	r2, r2, #24
	orr	r3, r3, r1
	ldr	r1, .L102+112
	orr	r3, r3, r0, asl #24
	mov	r0, #4
	str	r3, [r8], #4
	add	r6, r6, #8
	ldr	ip, [r4, #68]
	blx	ip
	cmp	r7, #2
	bne	.L80
	add	r9, r9, #8
	add	r10, r10, #1
	cmp	r9, #64
	bne	.L79
	ldr	r10, [fp, #-52]
	mov	r9, #0
.L81:
	add	r8, r9, #176
	mov	r6, r10
	add	r8, r5, r8
	mov	r7, #0
.L82:
	ldrb	r3, [r6, #120]	@ zero_extendqisi2
	add	r2, r7, r7, lsr #31
	ldrb	ip, [r6, #104]	@ zero_extendqisi2
	add	r7, r7, #1
	ldrb	r1, [r6, #88]	@ zero_extendqisi2
	add	r2, r9, r2, asr #1
	ldrb	r0, [r6, #136]	@ zero_extendqisi2
	mov	r3, r3, asl #16
	orr	r3, r3, ip, asl #8
	add	r2, r2, #24
	orr	r3, r3, r1
	ldr	r1, .L102+112
	orr	r3, r3, r0, asl #24
	mov	r0, #4
	str	r3, [r8], #4
	add	r6, r6, #8
	ldr	ip, [r4, #68]
	blx	ip
	cmp	r7, #2
	bne	.L82
	add	r9, r9, #8
	add	r10, r10, #1
	cmp	r9, #64
	bne	.L81
	b	.L78
.L98:
	ldr	r1, [r8, #2332]
	cmp	r1, #1
	beq	.L100
	bhi	.L75
	ldrb	r3, [r7, #152]	@ zero_extendqisi2
	ldrb	r2, [r7, #153]	@ zero_extendqisi2
	and	r3, r3, #1
	and	lr, r2, #1
	mov	r3, r3, asl #3
	b	.L77
.L97:
	ldr	r3, [r8, #2332]
	cmp	r3, #1
	bne	.L69
	ldrb	r9, [r7, #154]	@ zero_extendqisi2
	cmp	r9, #1
	beq	.L101
	ldr	r2, [r8, #2356]
	mov	r0, #4
	ldrb	r1, [r7, #11]	@ zero_extendqisi2
	mov	r1, r2, asl r1
	ldr	r2, [r8, #2336]
	cmp	r1, #0
	mov	r3, r3, asl r2
	addle	r3, r1, r3, asr #1
	addgt	r1, r1, r3, asr #1
	suble	r3, r3, #1
	movgt	r2, r1, asr r2
	ldr	r1, .L102+116
	movle	r2, r3, asr r2
	str	r2, [r5, #24]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r3, [r8, #2360]
	ldrb	r2, [r7, #11]	@ zero_extendqisi2
	mov	r3, r3, asl r2
	cmp	r3, #0
	ble	.L73
	ldr	r1, [r8, #2336]
	mov	r2, #1
	mov	r2, r2, asl r1
	add	r3, r3, r2, asr #1
	mov	r2, r3, asr r1
.L74:
	str	r2, [r5, #28]
	mov	r0, #4
	ldr	r3, [r4, #68]
	ldr	r1, .L102+120
	blx	r3
	b	.L69
.L96:
	ldr	r3, [r7, #204]
	mov	r0, #4
	ldr	r2, [r7, #208]
	ldr	r1, .L102+124
	mov	r3, r3, asl #1
	uxth	r3, r3
	orr	r2, r3, r2, asl #17
	str	r2, [r5, #12]
	ldr	r3, [r9, #68]
	blx	r3
	b	.L66
.L83:
	mov	r6, r3
	b	.L61
.L100:
	ldrb	r2, [r7, #153]	@ zero_extendqisi2
	mov	r3, #8
	strb	r1, [r7, #152]
	and	lr, r2, #1
	b	.L77
.L73:
	ldr	r2, [r8, #2336]
	mov	r1, #1
	mov	r1, r1, asl r2
	add	r3, r3, r1, asr #1
	sub	r3, r3, #1
	mov	r2, r3, asr r2
	b	.L74
.L101:
	ldr	r3, [r8, #2336]
	ldrb	r1, [r7, #11]	@ zero_extendqisi2
	ldr	r0, [r8, #2356]
	rsb	r1, r1, r3
	mov	r1, r9, asl r1
	bl	__aeabi_idiv
	ldr	r1, .L102+116
	mov	r2, r0
	str	r0, [r5, #24]
	mov	r0, #4
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r8, #2336]
	ldrb	r1, [r7, #11]	@ zero_extendqisi2
	ldr	r0, [r8, #2360]
	rsb	r1, r1, r3
	mov	r1, r9, asl r1
	bl	__aeabi_idiv
	ldr	r1, .L102+120
	str	r0, [r5, #28]
	mov	r2, r0
	ldr	r3, [r6, #68]
	mov	r0, #4
	blx	r3
	b	.L69
.L94:
	ldr	r3, .L102+4
	movw	r2, #971
	ldr	r1, .L102+128
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L64
.L103:
	.align	2
.L102:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
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
	.size	MP4HAL_V300R001_CfgDnMsg, .-MP4HAL_V300R001_CfgDnMsg
	.align	2
	.global	MP4HAL_V300R001_WriteSlicMsg
	.type	MP4HAL_V300R001_WriteSlicMsg, %function
MP4HAL_V300R001_WriteSlicMsg:
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
	ldr	ip, .L129
	movw	r3, #1208
	mov	lr, r0
	str	r0, [fp, #-72]
	add	r0, r0, #12288
	mla	r1, r3, r1, ip
	ldr	r8, [r0, #2288]
	add	r3, lr, #240
	str	r2, [fp, #-64]
	mov	r5, #0
	str	r3, [fp, #-56]
	str	r5, [fp, #-48]
	ldr	r4, [r1, #48]
	add	r6, r4, #256
	mov	r0, r6
	bl	MEM_Phy2Vir
	subs	r9, r0, #0
	beq	.L127
	ldr	r3, [fp, #-72]
	ldr	r3, [r3, #264]
	cmp	r3, #0
	bne	.L128
.L107:
	cmp	r8, #0
	ble	.L122
	mov	r3, r3, asl #5
	ldr	r10, .L129+4
	str	r3, [fp, #-68]
	mov	r5, #0
	add	r3, r3, r6
	str	r3, [fp, #-76]
	b	.L121
.L111:
	add	r5, r5, #1
	cmp	r8, r5
	ble	.L122
.L121:
	ldr	r3, [fp, #-56]
	mov	r4, r5, asl #5
	sub	r4, r4, r5, asl #2
	cmp	r5, #0
	add	r4, r3, r4
	movgt	r3, #1
	movle	r3, #0
	ldr	r7, [r4, #24]
	ldr	r2, [r4, #-4]
	cmp	r7, r2
	movhi	r7, #0
	andls	r7, r3, #1
	cmp	r7, #0
	bne	.L111
	ldr	r3, [r4, #16]
	mov	r0, r7
	ldrb	r2, [r4, #6]	@ zero_extendqisi2
	mov	r1, #0
	and	ip, r3, #15
	ldr	lr, [r4, #8]
	str	r3, [fp, #-60]
	add	r2, r2, ip, lsl #3
	bfi	r0, lr, #0, #24
	ldr	r3, [fp, #-68]
	bfi	r1, r2, #0, #7
	str	r0, [fp, #-48]
	mov	r0, #4
	strb	r1, [fp, #-45]
	add	r6, r3, r5, lsl #3
	ldr	r2, [fp, #-48]
	add	r5, r5, #1
	ldr	r1, .L129+8
	str	r2, [r9, r6, asl #2]
	add	r6, r6, #1
	ldr	ip, [r10, #68]
	blx	ip
	ldr	r3, [fp, #-60]
	ldr	r2, [fp, #-64]
	mov	ip, r7
	bic	r3, r3, #15
	ldr	r1, .L129+12
	rsb	r3, r2, r3
	mov	r0, #4
	bfi	ip, r3, #0, #24
	str	ip, [fp, #-48]
	str	ip, [r9, r6, asl #2]
	mov	r6, r6, asl #2
	mov	r2, ip
	ldr	r3, [r10, #68]
	blx	r3
	ldr	r3, [r4, #20]
	ldrb	r2, [r4, #7]	@ zero_extendqisi2
	mov	r0, r7
	and	ip, r3, #15
	ldr	lr, [r4, #12]
	mov	r1, #0
	str	r3, [fp, #-60]
	add	r2, r2, ip, lsl #3
	bfi	r0, lr, #0, #24
	bfi	r1, r2, #0, #7
	str	r0, [fp, #-48]
	strb	r1, [fp, #-45]
	add	ip, r6, #4
	ldr	r2, [fp, #-48]
	mov	r0, #4
	ldr	r1, .L129+16
	str	r2, [r9, ip]
	ldr	ip, [r10, #68]
	blx	ip
	ldr	r2, [r4, #20]
	ldr	r3, [fp, #-60]
	mov	r0, #4
	cmp	r2, #0
	ldr	r1, .L129+20
	bicne	r3, r3, #15
	streq	r2, [fp, #-48]
	ldrne	r2, [fp, #-64]
	rsbne	r3, r2, r3
	bfine	r7, r3, #0, #24
	strne	r7, [fp, #-48]
	ldr	r2, [fp, #-48]
	add	r3, r6, #8
	mov	r7, #0
	str	r2, [r9, r3]
	ldr	r3, [r10, #68]
	blx	r3
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	add	ip, r6, #12
	and	r3, r3, #31
	str	r7, [fp, #-48]
	bfi	r3, r2, #5, #2
	ldrb	r2, [r4, #3]	@ zero_extendqisi2
	strb	r3, [fp, #-48]
	mov	r0, #4
	ldrh	r3, [fp, #-48]
	ldrb	r1, [r4, #1]	@ zero_extendqisi2
	bfi	r3, r2, #7, #3
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	strh	r3, [fp, #-48]	@ movhi
	mov	r3, r3, lsr #8
	bfi	r3, r1, #2, #3
	ldr	r1, .L129+24
	bfi	r3, r2, #5, #3
	strb	r3, [fp, #-47]
	ldr	r2, [fp, #-48]
	str	r2, [r9, ip]
	ldr	r3, [r10, #68]
	blx	r3
	ldr	r1, [r4, #24]
	add	r3, r6, #16
	mov	r2, r7
	mov	r0, #4
	bfi	r2, r1, #0, #20
	ldr	r1, .L129+28
	str	r2, [fp, #-48]
	str	r2, [r9, r3]
	ldr	r3, [r10, #68]
	blx	r3
	cmp	r8, r5
	ble	.L114
	ldr	r2, [r4, #24]
	ldr	r3, [r4, #52]
	cmp	r2, r3
	bcs	.L115
	b	.L114
.L117:
	ldr	r3, [r4, #52]
	cmp	r3, r2
	bhi	.L119
.L115:
	add	r5, r5, #1
	add	r4, r4, #28
	cmp	r8, r5
	bne	.L117
.L123:
	ldr	r3, [fp, #-72]
	mov	r7, #0
	mov	r4, r7
	ldrh	r1, [r3, #164]
	ldrh	r3, [r3, #166]
	mul	r1, r3, r1
	sub	r1, r1, #1
.L120:
	add	r3, r6, #20
	add	r6, r6, #24
	mov	r2, #0
	mov	r0, #4
	bfi	r2, r1, #0, #20
	sub	r5, r5, #1
	str	r2, [fp, #-48]
	add	r5, r5, #1
	str	r2, [r9, r3]
	ldr	r1, .L129+32
	ldr	r3, [r10, #68]
	blx	r3
	str	r7, [r9, r6]
	mov	r2, r4
	ldr	r1, .L129+36
	mov	r0, #4
	ldr	r3, [r10, #68]
	str	r4, [fp, #-48]
	blx	r3
	cmp	r8, r5
	bgt	.L121
.L122:
	mov	r0, #0
.L106:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L114:
	cmp	r8, r5
	beq	.L123
.L119:
	ldr	r2, [fp, #-76]
	mov	r4, r5, asl #5
	sub	r3, r4, r5, asl #2
	add	r4, r4, r2
	ldr	r2, [fp, #-56]
	add	r3, r2, r3
	mov	r7, r4
	ldr	r1, [r3, #24]
	sub	r1, r1, #1
	b	.L120
.L128:
	ldr	r2, [fp, #-72]
	mov	r0, #4
	ldr	r3, [fp, #-48]
	add	r4, r4, #288
	ldr	r7, .L129+4
	ldr	r10, [r2, #256]
	bfi	r3, r5, #0, #24
	ldrb	r2, [r2, #246]	@ zero_extendqisi2
	and	r1, r10, #15
	str	r3, [fp, #-48]
	mov	r3, r3, lsr #24
	add	r2, r2, r1, lsl #3
	ldr	r1, .L129+8
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r9]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r1, [fp, #-64]
	bic	r3, r10, #15
	mov	r2, r5
	rsb	r3, r1, r3
	mov	r0, #4
	bfi	r2, r3, #0, #24
	ldr	r1, .L129+12
	str	r2, [fp, #-48]
	str	r2, [r9, #4]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [fp, #-72]
	mov	r2, #0
	ldr	r1, .L129+16
	mov	r0, #4
	ldr	r10, [r3, #260]
	ldrb	r3, [r3, #247]	@ zero_extendqisi2
	and	ip, r10, #15
	str	r5, [fp, #-48]
	add	r3, r3, ip, lsl #3
	bfi	r2, r3, #0, #7
	strb	r2, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r9, #8]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [fp, #-72]
	ldr	r1, .L129+20
	mov	r0, #4
	ldr	r3, [r3, #260]
	cmp	r3, #0
	bicne	r3, r10, #15
	streq	r3, [fp, #-48]
	ldrne	r2, [fp, #-64]
	rsbne	r3, r2, r3
	bfine	r5, r3, #0, #24
	strne	r5, [fp, #-48]
	ldr	r2, [fp, #-48]
	mov	r5, #0
	str	r2, [r9, #12]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r10, [fp, #-72]
	str	r5, [fp, #-48]
	mov	r0, #4
	ldr	r1, .L129+24
	ldrb	r3, [r10, #245]	@ zero_extendqisi2
	ldrb	r2, [r10, #244]	@ zero_extendqisi2
	and	r3, r3, #31
	ldrb	ip, [r10, #241]	@ zero_extendqisi2
	bfi	r3, r2, #5, #2
	ldrb	r2, [r10, #243]	@ zero_extendqisi2
	strb	r3, [fp, #-48]
	ldrh	r3, [fp, #-48]
	bfi	r3, r2, #7, #3
	ldrb	r2, [r10, #242]	@ zero_extendqisi2
	strh	r3, [fp, #-48]	@ movhi
	mov	r3, r3, lsr #8
	bfi	r3, ip, #2, #3
	bfi	r3, r2, #5, #3
	strb	r3, [fp, #-47]
	ldr	r2, [fp, #-48]
	str	r2, [r9, #16]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r1, .L129+28
	mov	r2, r5
	mov	r0, #4
	bfi	r2, r5, #0, #20
	str	r2, [fp, #-48]
	str	r2, [r9, #20]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r10, #264]
	ldr	r1, .L129+32
	mov	r2, r5
	sub	r3, r3, #1
	mov	r0, #4
	bfi	r2, r3, #0, #20
	str	r2, [fp, #-48]
	str	r2, [r9, #24]
	ldr	r3, [r7, #68]
	blx	r3
	str	r4, [r9, #28]
	ldr	r1, .L129+36
	mov	r0, #4
	ldr	r3, [r7, #68]
	mov	r2, r4
	str	r4, [fp, #-48]
	blx	r3
	mov	r3, #1
	b	.L107
.L127:
	ldr	r3, .L129+4
	ldr	r2, .L129+40
	ldr	r1, .L129+44
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L106
.L130:
	.align	2
.L129:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
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
	.size	MP4HAL_V300R001_WriteSlicMsg, .-MP4HAL_V300R001_WriteSlicMsg
	.align	2
	.global	MP4HAL_V300R001_StartDec
	.type	MP4HAL_V300R001_StartDec, %function
MP4HAL_V300R001_StartDec:
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
	ldrh	r3, [r0, #164]
	mov	r6, #0
	mov	r4, r0
	cmp	r3, #512
	mov	r5, r1
	str	r6, [fp, #-32]
	bhi	.L138
	ldrh	r3, [r0, #166]
	cmp	r3, #512
	bhi	.L138
	sub	r2, fp, #32
	bl	MP4HAL_V300R001_CfgReg
	subs	r7, r0, #0
	bne	.L139
	mov	r1, r5
	mov	r0, r4
	bl	MP4HAL_V300R001_CfgDnMsg
	subs	r6, r0, #0
	bne	.L140
	mov	r1, r5
	mov	r0, r4
	ldr	r2, [fp, #-32]
	bl	MP4HAL_V300R001_WriteSlicMsg
	cmp	r0, #0
	bne	.L141
.L133:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L138:
	ldr	r1, .L142
	mov	r0, r6
	ldr	r3, .L142+4
	ldr	r2, .L142+8
	ldr	ip, [r1, #68]
	ldr	r1, .L142+12
	blx	ip
	mvn	r0, #0
	b	.L133
.L139:
	ldr	r3, .L142
	mov	r0, r6
	ldr	r1, .L142+16
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L133
.L140:
	ldr	r3, .L142
	mov	r0, r7
	ldr	r1, .L142+20
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L133
.L141:
	ldr	r3, .L142
	mov	r0, r6
	ldr	r1, .L142+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L133
.L143:
	.align	2
.L142:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC53
	.word	.LANCHOR0+56
	.word	.LC54
	.word	.LC55
	.word	.LC56
	.word	.LC57
	UNWIND(.fnend)
	.size	MP4HAL_V300R001_StartDec, .-MP4HAL_V300R001_StartDec
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.13928, %object
	.size	__func__.13928, 23
__func__.13928:
	.ascii	"MP4HAL_V300R001_CfgReg\000"
	.space	1
	.type	__func__.13977, %object
	.size	__func__.13977, 29
__func__.13977:
	.ascii	"MP4HAL_V300R001_WriteSlicMsg\000"
	.space	3
	.type	__func__.13908, %object
	.size	__func__.13908, 25
__func__.13908:
	.ascii	"MP4HAL_V300R001_StartDec\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"stream_base_addr = %#x\012\000" )
.LC1:
	ASCII(.ascii	"BASIC_V300R001_CFG0=0x%x\012\000" )
	.space	2
.LC2:
	ASCII(.ascii	"BASIC_V300R001_CFG1=0x%x\012\000" )
	.space	2
.LC3:
	ASCII(.ascii	"AVM_V300R001_ADDR=0x%x\012\000" )
.LC4:
	ASCII(.ascii	"VAM_V300R001_ADDR=0x%x\012\000" )
.LC5:
	ASCII(.ascii	"%s: WR_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\012\000" )
	.space	3
.LC6:
	ASCII(.ascii	"STREAM_V300R001_BASE_ADDR=0x%x\012\000" )
.LC7:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC8:
	ASCII(.ascii	"YSTADDR_V200R003_1D=0x%x\012\000" )
	.space	2
.LC9:
	ASCII(.ascii	"YSTRIDE_V200R003_1D=0x%x\012\000" )
	.space	2
.LC10:
	ASCII(.ascii	"UVOFFSET_V200R003_1D=0x%x\012\000" )
	.space	1
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
