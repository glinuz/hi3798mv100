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
	.file	"vdh_module.c"
	.text
	.align	2
	.global	VDH_Reset_Global
	.type	VDH_Reset_Global, %function
VDH_Reset_Global:
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
	ldr	r4, .L14
	ldr	r3, [r4, #4]
	cmp	r3, #0
	beq	.L2
	mov	r0, #0
	blx	r3
	ldr	r3, [r4, #88]
	cmp	r3, #0
	beq	.L4
.L12:
	mov	r0, #0
	blx	r3
	ldr	r3, [r4, #72]
	cmp	r3, #0
	beq	.L6
.L13:
	mov	r0, #0
	blx	r3
.L7:
	ldr	r0, [r4, #76]
	cmp	r0, #0
	beq	.L8
	blx	r0
.L9:
	ldr	r3, [r4, #8]
	cmp	r3, #0
	beq	.L10
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L10:
	mov	r0, r3
	mov	r2, #54
	ldr	r1, .L14+4
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L8:
	mov	r2, #52
	ldr	r1, .L14+4
	bl	dprint_vfmw
	b	.L9
.L2:
	mov	r0, r3
	mov	r2, #48
	ldr	r1, .L14+4
	bl	dprint_vfmw
	ldr	r3, [r4, #88]
	cmp	r3, #0
	bne	.L12
.L4:
	mov	r0, r3
	mov	r2, #49
	ldr	r1, .L14+4
	bl	dprint_vfmw
	ldr	r3, [r4, #72]
	cmp	r3, #0
	bne	.L13
.L6:
	mov	r0, r3
	mov	r2, #51
	ldr	r1, .L14+4
	bl	dprint_vfmw
	b	.L7
.L15:
	.align	2
.L14:
	.word	g_vdm_hal_fun_ptr
	.word	.LC0
	UNWIND(.fnend)
	.size	VDH_Reset_Global, .-VDH_Reset_Global
	.align	2
	.global	VDH_Start_Mfde_Repair
	.type	VDH_Start_Mfde_Repair, %function
VDH_Start_Mfde_Repair:
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
	mov	r6, #0
	movt	r6, 63683
	ldrb	r4, [r0]	@ zero_extendqisi2
	mov	r5, r0
	mov	r0, r6
	bl	MEM_Phy2Vir
	cmp	r0, #0
	beq	.L20
	cmp	r4, #0
	bne	.L19
	ldr	r6, .L21
	movw	r3, #3075
	ldr	r0, [r5, #108]
	movt	r3, 48
	ldr	r1, .L21+4
	ldr	r2, [r6]
	str	r0, [r2, #16]
	ldr	r0, [r5, #124]
	str	r0, [r2, #36]
	ldr	r0, [r5, #104]
	str	r0, [r2, #12]
	str	r3, [r2, #60]
	str	r3, [r2, #64]
	str	r3, [r2, #68]
	str	r3, [r2, #72]
	str	r3, [r2, #76]
	str	r3, [r2, #80]
	str	r3, [r2, #84]
	ldr	r3, [r5, #100]
	str	r3, [r2, #8]
	ldr	r3, [r1, #108]
	blx	r3
	ldr	r3, [r6]
	mov	r2, #1
	str	r4, [r3]
	ldr	r3, [r6]
	str	r2, [r3]
	ldr	r3, [r6]
	str	r4, [r3]
.L16:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L19:
	mov	r5, #1
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	bl	dprint_vfmw
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	bl	dprint_vfmw
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	bl	dprint_vfmw
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	bl	dprint_vfmw
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	bl	dprint_vfmw
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	bl	dprint_vfmw
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	bl	dprint_vfmw
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	bl	dprint_vfmw
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	bl	dprint_vfmw
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	bl	dprint_vfmw
	mov	r3, r4
	ldr	r2, .L21+8
	mov	r0, #32
	ldr	r1, .L21+12
	str	r5, [sp]
	bl	dprint_vfmw
	ldr	r3, .L21+4
	ldr	r3, [r3, #108]
	blx	r3
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	bl	dprint_vfmw
	mov	r3, r4
	str	r5, [sp]
	mov	r0, #32
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	bl	dprint_vfmw
	str	r5, [sp]
	mov	r3, r4
	ldr	r2, .L21+8
	ldr	r1, .L21+12
	mov	r0, #32
	bl	dprint_vfmw
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L20:
	str	r6, [sp]
	mov	r3, r4
	ldr	r2, .L21+8
	ldr	r1, .L21+16
	bl	dprint_vfmw
	b	.L16
.L22:
	.align	2
.L21:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VDH_Start_Mfde_Repair, .-VDH_Start_Mfde_Repair
	.align	2
	.global	VDH_Start_Mfde_Decode
	.type	VDH_Start_Mfde_Decode, %function
VDH_Start_Mfde_Decode:
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
	mov	r6, #0
	movt	r6, 63683
	ldrb	r5, [r0]	@ zero_extendqisi2
	mov	r4, r0
	mov	r0, r6
	bl	MEM_Phy2Vir
	cmp	r0, #0
	beq	.L42
	ldr	r6, .L46
	ldr	r1, [r4, #56]
	ldr	r0, [r6]
	bl	MEM_WritePhyWord
	ldr	r0, [r6]
	ldr	r1, [r4, #64]
	add	r0, r0, #8
	bl	MEM_WritePhyWord
	cmp	r5, #0
	bne	.L26
	ldr	r3, .L46+4
	ldr	r2, [r4, #100]
	ldr	r3, [r3]
	str	r2, [r3, #8]
	ldr	r2, [r4, #104]
	str	r2, [r3, #12]
	ldr	r2, [r4, #108]
	str	r2, [r3, #16]
	ldr	r2, [r4, #112]
	str	r2, [r3, #20]
	ldr	r2, [r4, #116]
	str	r2, [r3, #24]
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L43
.L28:
	ldr	r3, [r4, #60]
	ldr	r0, [r6]
	cmp	r3, #0
	add	r0, r0, #4
	beq	.L44
	bl	MEM_ReadPhyWord
	orr	r1, r0, #65536
.L32:
	ldr	r0, [r6]
	add	r0, r0, #4
	bl	MEM_WritePhyWord
	cmp	r5, #0
	bne	.L33
	ldr	r3, .L46+4
	movw	r2, #3075
	movt	r2, 48
	ldr	r3, [r3]
	str	r2, [r3, #60]
	str	r2, [r3, #64]
	str	r2, [r3, #68]
	str	r2, [r3, #72]
	str	r2, [r3, #76]
	str	r2, [r3, #80]
	str	r2, [r3, #84]
	ldr	r2, [r4, #132]
	str	r2, [r3, #96]
	ldr	r2, [r4, #136]
	str	r2, [r3, #100]
	ldr	r2, [r4, #140]
	str	r2, [r3, #104]
	ldr	r2, [r4, #144]
	str	r2, [r3, #108]
	ldr	r2, [r4, #152]
	str	r2, [r3, #116]
	ldr	r2, [r4, #156]
	str	r2, [r3, #120]
	ldr	r2, [r4, #160]
	str	r2, [r3, #124]
	ldr	r2, [r4, #164]
	str	r2, [r3, #128]
	ldr	r2, [r4, #168]
	str	r2, [r3, #132]
	ldr	r2, [r4, #172]
	str	r2, [r3, #148]
	ldr	r2, [r4, #176]
	str	r2, [r3, #152]
	ldr	r2, [r4, #180]
	str	r2, [r3, #156]
	ldr	r3, .L46+8
	ldr	r3, [r3]
	cmp	r3, #0
	beq	.L35
.L45:
	ldr	r0, [r4, #20]
	blx	r3
.L36:
	ldr	r3, .L46+12
	ldr	r1, [r4, #16]
	ldr	r0, [r6]
	ldr	r2, [r3]
	add	r0, r0, #12
	ldr	r3, [r2, #120]
	bfi	r3, r1, #8, #2
	str	r3, [r2, #120]
	ldr	r1, [r4, #68]
	bl	MEM_WritePhyWord
	cmp	r5, #0
	bne	.L37
	ldr	r6, .L46+4
	mov	r0, #30
	ldr	r1, [r4, #120]
	ldr	r7, .L46+16
	ldr	r3, [r6]
	add	r2, r3, #61440
	str	r1, [r3, #32]
	ldr	r1, [r4, #124]
	str	r1, [r3, #36]
	ldr	r1, [r4, #184]
	ldr	r3, [r7, #112]
	str	r1, [r2, #32]
	blx	r3
	ldr	r3, [r7, #108]
	blx	r3
	ldr	r3, [r6]
	mov	r2, #1
	str	r5, [r3]
	ldr	r3, [r6]
	str	r2, [r3]
	ldr	r3, [r6]
	str	r5, [r3]
.L38:
	ldr	r3, [r4, #8]
	mov	r1, #4
	ldr	r2, .L46+20
	mov	r0, r3
	ldr	r2, [r2, r3, asl #2]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VDEC_Lowdelay_Event_Time
.L44:
	bl	MEM_ReadPhyWord
	bic	r1, r0, #65536
	b	.L32
.L26:
	mov	r7, #1
	mov	r3, r5
	str	r7, [sp]
	mov	r0, #32
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	bl	dprint_vfmw
	str	r7, [sp]
	mov	r3, r5
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	mov	r0, #32
	bl	dprint_vfmw
	str	r7, [sp]
	mov	r3, r5
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	mov	r0, #32
	bl	dprint_vfmw
	str	r7, [sp]
	mov	r3, r5
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	mov	r0, #32
	bl	dprint_vfmw
	str	r7, [sp]
	mov	r3, r5
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	mov	r0, #32
	bl	dprint_vfmw
	ldrb	r7, [r4, #1]	@ zero_extendqisi2
	cmp	r7, #1
	bne	.L28
	mov	r3, r5
	str	r7, [sp]
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	bl	dprint_vfmw
	str	r7, [sp]
	mov	r3, r5
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	mov	r0, #32
	bl	dprint_vfmw
	b	.L28
.L37:
	mov	r6, #1
	mov	r3, r5
	ldr	r7, .L46+16
	mov	r0, #32
	str	r6, [sp]
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	bl	dprint_vfmw
	mov	r3, r5
	str	r6, [sp]
	mov	r0, #32
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	bl	dprint_vfmw
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	mov	r3, r5
	str	r6, [sp]
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r3, [r7, #112]
	mov	r0, #30
	blx	r3
	ldr	r3, [r7, #108]
	blx	r3
	mov	r3, r5
	str	r6, [sp]
	mov	r0, #32
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	bl	dprint_vfmw
	mov	r3, r5
	str	r6, [sp]
	mov	r0, #32
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	bl	dprint_vfmw
	str	r6, [sp]
	mov	r3, r5
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	mov	r0, #32
	bl	dprint_vfmw
	b	.L38
.L33:
	mov	r7, #1
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	ldr	r2, .L46+24
	mov	r0, #32
	ldr	r1, .L46+28
	str	r7, [sp]
	bl	dprint_vfmw
	mov	r3, r5
	str	r7, [sp]
	mov	r0, #32
	ldr	r2, .L46+24
	ldr	r1, .L46+28
	bl	dprint_vfmw
	ldr	r3, .L46+8
	ldr	r3, [r3]
	cmp	r3, #0
	bne	.L45
.L35:
	mov	r0, r3
	mov	r2, #171
	ldr	r1, .L46+32
	bl	dprint_vfmw
	b	.L36
.L43:
	ldr	r2, [r4, #128]
	str	r2, [r3, #92]
	ldr	r2, [r4, #148]
	str	r2, [r3, #112]
	b	.L28
.L42:
	str	r6, [sp]
	mov	r3, r5
	ldr	r2, .L46+24
	ldr	r1, .L46+36
	bl	dprint_vfmw
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L47:
	.align	2
.L46:
	.word	s_ScdRegPhyBaseAddr
	.word	g_HwMem
	.word	g_vdm_hal_fun_ptr
	.word	g_pstRegCrg
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_LowDelaySeqIndex
	.word	.LANCHOR0+24
	.word	.LC2
	.word	.LC0
	.word	.LC1
	UNWIND(.fnend)
	.size	VDH_Start_Mfde_Decode, .-VDH_Start_Mfde_Decode
	.align	2
	.global	VDH_Init_Module
	.type	VDH_Init_Module, %function
VDH_Init_Module:
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
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDH_Reset_Global
	UNWIND(.fnend)
	.size	VDH_Init_Module, .-VDH_Init_Module
	.align	2
	.global	VDH_Start_Mfde_Module
	.type	VDH_Start_Mfde_Module, %function
VDH_Start_Mfde_Module:
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
	ldrb	r3, [r0, #2]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L51
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDH_Start_Mfde_Decode
.L51:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDH_Start_Mfde_Repair
	UNWIND(.fnend)
	.size	VDH_Start_Mfde_Module, .-VDH_Start_Mfde_Module
	.align	2
	.global	VDH_Start_Scd_Module
	.type	VDH_Start_Scd_Module, %function
VDH_Start_Scd_Module:
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
	mov	r5, r0
	mov	r0, #49152
	movt	r0, 63683
	ldrb	r4, [r5]	@ zero_extendqisi2
	bl	MEM_Phy2Vir
	cmp	r0, #0
	beq	.L58
	ldr	r6, .L60
	mov	r0, r4
	bl	ResetSCD
	ldr	r1, [r5, #24]
	ldr	r0, [r6, r4, asl #2]
	add	r0, r0, #2048
	add	r0, r0, #4
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #28]
	add	r0, r0, #2048
	add	r0, r0, #8
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #32]
	add	r0, r0, #2048
	add	r0, r0, #12
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #36]
	add	r0, r0, #2064
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #40]
	add	r0, r0, #2064
	add	r0, r0, #4
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #44]
	add	r0, r0, #2064
	add	r0, r0, #8
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #52]
	add	r0, r0, #2080
	bl	MEM_WritePhyWord
	ldrb	r3, [r5, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L59
.L54:
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #72]
	add	r0, r0, #2080
	add	r0, r0, #8
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #76]
	add	r0, r0, #2080
	add	r0, r0, #12
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #80]
	add	r0, r0, #2096
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #84]
	add	r0, r0, #2096
	add	r0, r0, #4
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #88]
	add	r0, r0, #2096
	add	r0, r0, #8
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #92]
	add	r0, r0, #2096
	add	r0, r0, #12
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #96]
	add	r0, r0, #2112
	add	r0, r0, #4
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	ldr	r1, [r5, #48]
	add	r0, r0, #2064
	add	r0, r0, #12
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	mov	r1, #0
	add	r0, r0, #2048
	bl	MEM_WritePhyWord
	ldr	r0, [r6, r4, asl #2]
	mov	r1, #1
	add	r0, r0, #2048
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	MEM_WritePhyWord
.L59:
	ldr	r1, [r5, #56]
	ldr	r0, [r6, r4, asl #2]
	bl	MEM_WritePhyWord
	b	.L54
.L58:
	mov	r3, #49152
	ldr	r2, .L60+4
	movt	r3, 63683
	ldr	r1, .L60+8
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L61:
	.align	2
.L60:
	.word	s_ScdRegPhyBaseAddr
	.word	.LANCHOR0+48
	.word	.LC3
	UNWIND(.fnend)
	.size	VDH_Start_Scd_Module, .-VDH_Start_Scd_Module
	.align	2
	.global	VDH_Record_CrgRegData
	.type	VDH_Record_CrgRegData, %function
VDH_Record_CrgRegData:
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
	streq	r2, [r0]
	beq	.L62
	cmp	r3, #1
	streq	r2, [r0, #4]
	beq	.L62
	str	r2, [sp]
	mov	r0, #1
	ldr	r2, .L66
	ldr	r1, .L66+4
	bl	dprint_vfmw
.L62:
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L67:
	.align	2
.L66:
	.word	.LANCHOR0+72
	.word	.LC4
	UNWIND(.fnend)
	.size	VDH_Record_CrgRegData, .-VDH_Record_CrgRegData
	.align	2
	.global	VDH_Record_ScdRegData
	.type	VDH_Record_ScdRegData, %function
VDH_Record_ScdRegData:
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
	movw	r3, #2072
	cmp	r1, r3
	beq	.L70
	bls	.L97
	cmp	r1, #2096
	beq	.L84
	bhi	.L85
	cmp	r1, #2080
	streq	r2, [r0, #28]
	beq	.L68
	bls	.L98
	movw	r3, #2088
	cmp	r1, r3
	streq	r2, [r0, #48]
	beq	.L68
	movw	r3, #2092
	cmp	r1, r3
	streq	r2, [r0, #52]
	bne	.L69
.L68:
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L97:
	movw	r3, #2052
	cmp	r1, r3
	beq	.L72
	bhi	.L73
	cmp	r1, #4
	streq	r2, [r0, #36]
	beq	.L68
	bls	.L99
	cmp	r1, #8
	streq	r2, [r0, #40]
	beq	.L68
	cmp	r1, #12
	streq	r2, [r0, #44]
	beq	.L68
.L69:
	str	r2, [sp]
	mov	r3, r1
	ldr	r2, .L102
	mov	r0, #1
	ldr	r1, .L102+4
	bl	dprint_vfmw
	b	.L68
.L73:
	movw	r3, #2060
	cmp	r1, r3
	streq	r2, [r0, #8]
	beq	.L68
	bls	.L100
	cmp	r1, #2064
	streq	r2, [r0, #12]
	beq	.L68
	movw	r3, #2068
	cmp	r1, r3
	streq	r2, [r0, #16]
	beq	.L68
	b	.L69
.L85:
	movw	r3, #2104
	cmp	r1, r3
	streq	r2, [r0, #64]
	beq	.L68
	bls	.L101
	movw	r3, #2108
	cmp	r1, r3
	streq	r2, [r0, #68]
	beq	.L68
	movw	r3, #2116
	cmp	r1, r3
	streq	r2, [r0, #72]
	beq	.L68
	b	.L69
.L100:
	movw	r3, #2056
	cmp	r1, r3
	streq	r2, [r0, #4]
	beq	.L68
	b	.L69
.L101:
	movw	r3, #2100
	cmp	r1, r3
	streq	r2, [r0, #60]
	beq	.L68
	b	.L69
.L99:
	cmp	r1, #0
	streq	r2, [r0, #32]
	beq	.L68
	b	.L69
.L98:
	movw	r3, #2076
	cmp	r1, r3
	streq	r2, [r0, #24]
	beq	.L68
	b	.L69
.L84:
	str	r2, [r0, #56]
	b	.L68
.L72:
	str	r2, [r0]
	b	.L68
.L70:
	str	r2, [r0, #20]
	b	.L68
.L103:
	.align	2
.L102:
	.word	.LANCHOR0+96
	.word	.LC4
	UNWIND(.fnend)
	.size	VDH_Record_ScdRegData, .-VDH_Record_ScdRegData
	.align	2
	.global	VDH_Record_MfdeRegData
	.type	VDH_Record_MfdeRegData, %function
VDH_Record_MfdeRegData:
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
	cmp	r1, #92
	beq	.L106
	bls	.L164
	cmp	r1, #124
	beq	.L119
	bhi	.L120
	cmp	r1, #108
	streq	r2, [r0, #44]
	beq	.L104
	bls	.L165
	cmp	r1, #116
	streq	r2, [r0, #52]
	beq	.L104
	cmp	r1, #120
	streq	r2, [r0, #56]
	beq	.L104
	cmp	r1, #112
	bne	.L105
	str	r2, [r0, #48]
.L104:
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L120:
	cmp	r1, #148
	streq	r2, [r0, #72]
	beq	.L104
	bls	.L166
	cmp	r1, #156
	streq	r2, [r0, #80]
	beq	.L104
	movw	r3, #61472
	cmp	r1, r3
	streq	r2, [r0, #84]
	beq	.L104
	cmp	r1, #152
	bne	.L105
	str	r2, [r0, #76]
	b	.L104
.L164:
	cmp	r1, #36
	beq	.L108
	bhi	.L109
	cmp	r1, #16
	streq	r2, [r0, #8]
	beq	.L104
	bls	.L167
	cmp	r1, #24
	streq	r2, [r0, #16]
	beq	.L104
	cmp	r1, #32
	streq	r2, [r0, #20]
	beq	.L104
	cmp	r1, #20
	bne	.L105
	str	r2, [r0, #12]
	b	.L104
.L165:
	cmp	r1, #100
	streq	r2, [r0, #36]
	beq	.L104
	cmp	r1, #104
	streq	r2, [r0, #40]
	beq	.L104
	cmp	r1, #96
	bne	.L105
	str	r2, [r0, #32]
	b	.L104
.L166:
	cmp	r1, #132
	streq	r2, [r0, #68]
	beq	.L104
	cmp	r1, #144
	beq	.L104
	cmp	r1, #128
	streq	r2, [r0, #64]
	beq	.L104
	b	.L105
.L167:
	cmp	r1, #8
	streq	r2, [r0]
	beq	.L104
	cmp	r1, #12
	streq	r2, [r0, #4]
	beq	.L104
.L105:
	str	r2, [sp]
	mov	r3, r1
	ldr	r2, .L168
	mov	r0, #1
	ldr	r1, .L168+4
	bl	dprint_vfmw
	b	.L104
.L109:
	cmp	r1, #72
	beq	.L104
	bhi	.L118
	cmp	r1, #64
	beq	.L104
	cmp	r1, #68
	beq	.L104
	cmp	r1, #60
	bne	.L105
	b	.L104
.L108:
	str	r2, [r0, #24]
	b	.L104
.L119:
	str	r2, [r0, #60]
	b	.L104
.L106:
	str	r2, [r0, #28]
	b	.L104
.L118:
	cmp	r1, #80
	beq	.L104
	cmp	r1, #84
	beq	.L104
	cmp	r1, #76
	bne	.L105
	b	.L104
.L169:
	.align	2
.L168:
	.word	.LANCHOR0+120
	.word	.LC4
	UNWIND(.fnend)
	.size	VDH_Record_MfdeRegData, .-VDH_Record_MfdeRegData
	.align	2
	.global	VDH_Init_Hardware
	.type	VDH_Init_Hardware, %function
VDH_Init_Hardware:
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
	ldr	r4, .L178
	ldr	r3, [r4, #36]
	cmp	r3, #0
	beq	.L171
	mov	r0, #0
	blx	r3
.L171:
	ldr	r3, [r4, #72]
	cmp	r3, #0
	beq	.L172
	mov	r0, #0
	blx	r3
.L173:
	ldr	r0, [r4, #76]
	cmp	r0, #0
	beq	.L174
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r0	@ indirect register sibling call
.L174:
	movw	r2, #498
	ldr	r1, .L178+4
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L172:
	mov	r0, r3
	movw	r2, #497
	ldr	r1, .L178+4
	bl	dprint_vfmw
	b	.L173
.L179:
	.align	2
.L178:
	.word	g_vdm_hal_fun_ptr
	.word	.LC0
	UNWIND(.fnend)
	.size	VDH_Init_Hardware, .-VDH_Init_Hardware
	.align	2
	.global	VDH_Enable_Scd_Module
	.type	VDH_Enable_Scd_Module, %function
VDH_Enable_Scd_Module:
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
	ldr	r3, .L182
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq	.L181
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L181:
	mov	r0, r3
	movw	r2, #510
	ldr	r1, .L182+4
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L183:
	.align	2
.L182:
	.word	g_vdm_hal_fun_ptr
	.word	.LC0
	UNWIND(.fnend)
	.size	VDH_Enable_Scd_Module, .-VDH_Enable_Scd_Module
	.align	2
	.global	VDH_Disable_Scd_Module
	.type	VDH_Disable_Scd_Module, %function
VDH_Disable_Scd_Module:
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
	ldr	r3, .L186
	ldr	r3, [r3, #24]
	cmp	r3, #0
	beq	.L185
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L185:
	mov	r0, r3
	movw	r2, #521
	ldr	r1, .L186+4
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L187:
	.align	2
.L186:
	.word	g_vdm_hal_fun_ptr
	.word	.LC0
	UNWIND(.fnend)
	.size	VDH_Disable_Scd_Module, .-VDH_Disable_Scd_Module
	.align	2
	.global	VDH_Reset_Scd_Module
	.type	VDH_Reset_Scd_Module, %function
VDH_Reset_Scd_Module:
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
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	ResetSCD
	UNWIND(.fnend)
	.size	VDH_Reset_Scd_Module, .-VDH_Reset_Scd_Module
	.align	2
	.global	VDH_Enable_Mfde_Module
	.type	VDH_Enable_Mfde_Module, %function
VDH_Enable_Mfde_Module:
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
	ldr	r3, .L191
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L190
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L190:
	mov	r0, r3
	movw	r2, #542
	ldr	r1, .L191+4
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L192:
	.align	2
.L191:
	.word	g_vdm_hal_fun_ptr
	.word	.LC0
	UNWIND(.fnend)
	.size	VDH_Enable_Mfde_Module, .-VDH_Enable_Mfde_Module
	.align	2
	.global	VDH_Disable_Mfde_Module
	.type	VDH_Disable_Mfde_Module, %function
VDH_Disable_Mfde_Module:
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
	ldr	r3, .L196
	mov	r4, r0
	ldr	r3, [r3, #8]
	cmp	r3, #0
	beq	.L194
	blx	r3
.L195:
	mov	r0, r4
	bl	SCDDRV_SetStateIdle
	mov	r0, r4
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	VDMDRV_SetStateIdle
.L194:
	mov	r0, r3
	mov	r2, #552
	ldr	r1, .L196+4
	bl	dprint_vfmw
	b	.L195
.L197:
	.align	2
.L196:
	.word	g_vdm_hal_fun_ptr
	.word	.LC0
	UNWIND(.fnend)
	.size	VDH_Disable_Mfde_Module, .-VDH_Disable_Mfde_Module
	.align	2
	.global	VDH_Reset_Mfde_Module
	.type	VDH_Reset_Mfde_Module, %function
VDH_Reset_Mfde_Module:
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
	ldr	r3, .L200
	ldr	r3, [r3, #80]
	cmp	r3, #0
	beq	.L199
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L199:
	mov	r0, r3
	mov	r2, #564
	ldr	r1, .L200+4
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L201:
	.align	2
.L200:
	.word	g_vdm_hal_fun_ptr
	.word	.LC0
	UNWIND(.fnend)
	.size	VDH_Reset_Mfde_Module, .-VDH_Reset_Mfde_Module
	.align	2
	.global	VDH_Enable_Bpd_Module
	.type	VDH_Enable_Bpd_Module, %function
VDH_Enable_Bpd_Module:
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
	ldr	r3, .L204
	ldr	r3, [r3, #28]
	cmp	r3, #0
	ldmeqfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L205:
	.align	2
.L204:
	.word	g_vdm_hal_fun_ptr
	UNWIND(.fnend)
	.size	VDH_Enable_Bpd_Module, .-VDH_Enable_Bpd_Module
	.align	2
	.global	VDH_Disable_Bpd_Module
	.type	VDH_Disable_Bpd_Module, %function
VDH_Disable_Bpd_Module:
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
	ldr	r3, .L208
	ldr	r3, [r3, #32]
	cmp	r3, #0
	ldmeqfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L209:
	.align	2
.L208:
	.word	g_vdm_hal_fun_ptr
	UNWIND(.fnend)
	.size	VDH_Disable_Bpd_Module, .-VDH_Disable_Bpd_Module
	.align	2
	.global	VDH_Enable_Dsp_Module
	.type	VDH_Enable_Dsp_Module, %function
VDH_Enable_Dsp_Module:
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
	ldr	r3, .L212
	ldr	r3, [r3, #12]
	cmp	r3, #0
	beq	.L211
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L211:
	mov	r0, r3
	movw	r2, #594
	ldr	r1, .L212+4
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L213:
	.align	2
.L212:
	.word	g_vdm_hal_fun_ptr
	.word	.LC0
	UNWIND(.fnend)
	.size	VDH_Enable_Dsp_Module, .-VDH_Enable_Dsp_Module
	.align	2
	.global	VDH_Disable_Dsp_Module
	.type	VDH_Disable_Dsp_Module, %function
VDH_Disable_Dsp_Module:
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
	ldr	r3, .L216
	ldr	r3, [r3, #16]
	cmp	r3, #0
	beq	.L215
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L215:
	mov	r0, r3
	mov	r2, #604
	ldr	r1, .L216+4
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L217:
	.align	2
.L216:
	.word	g_vdm_hal_fun_ptr
	.word	.LC0
	UNWIND(.fnend)
	.size	VDH_Disable_Dsp_Module, .-VDH_Disable_Dsp_Module
	.align	2
	.global	VDH_Load_Dsp_Code
	.type	VDH_Load_Dsp_Code, %function
VDH_Load_Dsp_Code:
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
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	SCDDRV_LoadDspCode
	UNWIND(.fnend)
	.size	VDH_Load_Dsp_Code, .-VDH_Load_Dsp_Code
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.46190, %object
	.size	__func__.46190, 22
__func__.46190:
	.ascii	"VDH_Start_Mfde_Repair\000"
	.space	2
	.type	__func__.46198, %object
	.size	__func__.46198, 22
__func__.46198:
	.ascii	"VDH_Start_Mfde_Decode\000"
	.space	2
	.type	__func__.46210, %object
	.size	__func__.46210, 21
__func__.46210:
	.ascii	"VDH_Start_Scd_Module\000"
	.space	3
	.type	__func__.46216, %object
	.size	__func__.46216, 22
__func__.46216:
	.ascii	"VDH_Record_CrgRegData\000"
	.space	2
	.type	__func__.46243, %object
	.size	__func__.46243, 22
__func__.46243:
	.ascii	"VDH_Record_ScdRegData\000"
	.space	2
	.type	__func__.46281, %object
	.size	__func__.46281, 23
__func__.46281:
	.ascii	"VDH_Record_MfdeRegData\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"VDMHAL_NULL_FUN_PRINT,L%d\012\000" )
	.space	1
.LC1:
	ASCII(.ascii	"%s: map mfde %d register 0x%x failed!\012\000" )
	.space	1
.LC2:
	ASCII(.ascii	"%s: WR_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\012\000" )
	.space	3
.LC3:
	ASCII(.ascii	"%s: map scd register 0x%x failed!\012\000" )
	.space	1
.LC4:
	ASCII(.ascii	"%s: unrecord reg 0x%x data %d!\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
