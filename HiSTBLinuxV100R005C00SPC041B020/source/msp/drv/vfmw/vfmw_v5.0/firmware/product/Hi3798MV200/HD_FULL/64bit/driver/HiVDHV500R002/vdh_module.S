#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vdh_module.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	VDH_Reset_Global
	.type	VDH_Reset_Global, %function
VDH_Reset_Global:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC0
	ldr	x1, [x19, 8]
	cbz	x1, .L2
	mov	w0, 0
	blr	x1
	ldr	x1, [x19, 176]
	cbz	x1, .L4
.L13:
	mov	w0, 0
	blr	x1
	ldr	x1, [x19, 144]
	cbz	x1, .L6
.L14:
	mov	w0, 0
	blr	x1
.L7:
	ldr	x0, [x19, 152]
	cbz	x0, .L8
	blr	x0
.L9:
	ldr	x1, [x19, 16]
	cbz	x1, .L10
	mov	w0, 0
	blr	x1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L10:
	ldr	x1, .LC2
	mov	w2, 54
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L8:
	ldr	x1, .LC2
	mov	w2, 52
	mov	w0, 0
	bl	dprint_vfmw
	b	.L9
	.p2align 3
.L2:
	ldr	x1, .LC2
	mov	w2, 48
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, [x19, 176]
	cbnz	x1, .L13
	.p2align 2
.L4:
	ldr	x1, .LC2
	mov	w2, 49
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, [x19, 144]
	cbnz	x1, .L14
	.p2align 2
.L6:
	ldr	x1, .LC2
	mov	w2, 51
	mov	w0, 0
	bl	dprint_vfmw
	b	.L7
	.size	VDH_Reset_Global, .-VDH_Reset_Global
	.align	3
.LC0:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC2:
	.xword	.LC1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Start_Mfde_Repair
	.type	VDH_Start_Mfde_Repair, %function
VDH_Start_Mfde_Repair:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	mov	w0, -121438208
	ldrb	w19, [x20]
	bl	MEM_Phy2Vir
	cbz	x0, .L19
	cbnz	w19, .L18
	ldr	x19, .LC6
	mov	w1, 3075
	ldr	w3, [x20, 108]
	movk	w1, 0x30, lsl 16
	ldr	x2, .LC7
	ldr	x0, [x19]
	str	w3, [x0, 16]
	ldr	w3, [x20, 124]
	str	w3, [x0, 36]
	ldr	w3, [x20, 104]
	str	w3, [x0, 12]
	str	w1, [x0, 60]
	str	w1, [x0, 64]
	str	w1, [x0, 68]
	str	w1, [x0, 72]
	str	w1, [x0, 76]
	str	w1, [x0, 80]
	str	w1, [x0, 84]
	ldr	w1, [x20, 100]
	str	w1, [x0, 8]
	ldr	x0, [x2, 216]
	blr	x0
	ldr	x0, [x19]
	mov	w1, 1
	str	wzr, [x0]
	ldr	x0, [x19]
	str	w1, [x0]
	ldr	x0, [x19]
	str	wzr, [x0]
.L15:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L18:
	ldr	x21, .LC3
	mov	w3, w19
	ldr	x20, .LC9
	mov	w4, 1
	mov	x2, x21
	mov	w0, 32
	mov	x1, x20
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x0, .LC7
	ldr	x0, [x0, 216]
	blr	x0
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x21
	mov	x1, x20
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L19:
	ldr	x2, .LC3
	mov	w4, -121438208
	ldr	x1, .LC5
	mov	w3, w19
	bl	dprint_vfmw
	b	.L15
	.size	VDH_Start_Mfde_Repair, .-VDH_Start_Mfde_Repair
	.align	3
.LC3:
	.xword	.LANCHOR0
	.align	3
.LC5:
	.xword	.LC4
	.align	3
.LC6:
	.xword	g_HwMem
	.align	3
.LC7:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC9:
	.xword	.LC8
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Start_Mfde_Decode
	.type	VDH_Start_Mfde_Decode, %function
VDH_Start_Mfde_Decode:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	w0, -121438208
	ldrb	w20, [x19]
	bl	MEM_Phy2Vir
	cbz	x0, .L38
	ldr	x22, .LC12
	ldr	w1, [x19, 56]
	ldr	w0, [x22]
	bl	MEM_WritePhyWord
	ldr	w0, [x22]
	ldr	w1, [x19, 64]
	add	w0, w0, 8
	bl	MEM_WritePhyWord
	cbnz	w20, .L23
	ldr	x0, .LC13
	ldr	w1, [x19, 100]
	ldr	x0, [x0]
	str	w1, [x0, 8]
	ldr	w1, [x19, 104]
	str	w1, [x0, 12]
	ldr	w1, [x19, 108]
	str	w1, [x0, 16]
	ldr	w1, [x19, 112]
	str	w1, [x0, 20]
	ldr	w1, [x19, 116]
	str	w1, [x0, 24]
	ldrb	w1, [x19, 1]
	cmp	w1, 1
	beq	.L39
.L25:
	ldr	w0, [x19, 60]
	cbz	w0, .L40
.L28:
	ldr	w0, [x22]
	add	w0, w0, 4
	bl	MEM_ReadPhyWord
	orr	w1, w0, 65536
	ldr	w0, [x22]
	add	w0, w0, 4
	bl	MEM_WritePhyWord
	cbnz	w20, .L30
.L41:
	ldr	x0, .LC13
	mov	w1, 3075
	movk	w1, 0x30, lsl 16
	ldr	x0, [x0]
	str	w1, [x0, 60]
	str	w1, [x0, 64]
	str	w1, [x0, 68]
	str	w1, [x0, 72]
	str	w1, [x0, 76]
	str	w1, [x0, 80]
	str	w1, [x0, 84]
	ldr	w1, [x19, 132]
	str	w1, [x0, 96]
	ldr	w1, [x19, 136]
	str	w1, [x0, 100]
	ldr	w1, [x19, 140]
	str	w1, [x0, 104]
	ldr	w1, [x19, 144]
	str	w1, [x0, 108]
	ldr	w1, [x19, 152]
	str	w1, [x0, 116]
	ldr	w1, [x19, 156]
	str	w1, [x0, 120]
	ldr	w1, [x19, 160]
	str	w1, [x0, 124]
	ldr	w1, [x19, 164]
	str	w1, [x0, 128]
	ldr	w1, [x19, 168]
	str	w1, [x0, 132]
	ldr	w1, [x19, 172]
	str	w1, [x0, 148]
	ldr	w1, [x19, 176]
	str	w1, [x0, 152]
	ldr	w1, [x19, 180]
	str	w1, [x0, 156]
	ldr	x0, .LC15
	ldr	x1, [x0]
	cbz	x1, .L32
.L42:
	ldr	w0, [x19, 20]
	blr	x1
.L33:
	ldr	x1, .LC17
	ldr	w3, [x19, 16]
	ldr	w0, [x22]
	ldr	x2, [x1]
	add	w0, w0, 12
	ldr	w1, [x2, 120]
	bfi	w1, w3, 8, 2
	str	w1, [x2, 120]
	ldr	w1, [x19, 68]
	bl	MEM_WritePhyWord
	cbnz	w20, .L34
	ldr	x20, .LC13
	mov	x0, 30
	ldr	w3, [x19, 120]
	ldr	x21, .LC18
	ldr	x1, [x20]
	add	x2, x1, 61440
	str	w3, [x1, 32]
	ldr	w3, [x19, 124]
	str	w3, [x1, 36]
	ldr	w3, [x19, 184]
	ldr	x1, [x21, 224]
	str	w3, [x2, 32]
	blr	x1
	ldr	x0, [x21, 216]
	blr	x0
	ldr	x0, [x20]
	mov	w1, 1
	str	wzr, [x0]
	ldr	x0, [x20]
	str	w1, [x0]
	ldr	x0, [x20]
	str	wzr, [x0]
.L35:
	ldr	w2, [x19, 8]
	mov	w1, 4
	ldr	x3, .LC19
	mov	w0, w2
	ldr	w2, [x3, w2, sxtw 2]
	bl	VDEC_Lowdelay_Event_Time
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L40:
	ldr	w0, [x22]
	add	w0, w0, 4
	bl	MEM_ReadPhyWord
	and	w1, w0, -65537
	ldr	w0, [x22]
	add	w0, w0, 4
	bl	MEM_WritePhyWord
	cbz	w20, .L41
.L30:
	ldr	x21, .LC10
	mov	w4, 1
	ldr	x23, .LC14
	mov	w3, w20
	add	x21, x21, 24
	mov	w0, 32
	mov	x1, x23
	mov	x2, x21
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	x1, x23
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x0, .LC15
	ldr	x1, [x0]
	cbnz	x1, .L42
.L32:
	ldr	x1, .LC16
	mov	w2, 171
	mov	w0, 0
	bl	dprint_vfmw
	b	.L33
	.p2align 3
.L23:
	ldr	x21, .LC10
	mov	w4, 1
	ldr	x23, .LC14
	mov	w3, w20
	add	x21, x21, 24
	mov	w0, 32
	mov	x1, x23
	mov	x2, x21
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	ldrb	w24, [x19, 1]
	cmp	w24, 1
	bne	.L25
	mov	w4, w24
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, w24
	mov	w3, w20
	mov	x2, x21
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	b	.L25
	.p2align 3
.L34:
	ldr	x21, .LC10
	mov	w3, w20
	ldr	x22, .LC14
	mov	w4, 1
	add	x21, x21, 24
	mov	w0, 32
	mov	x1, x22
	mov	x2, x21
	bl	dprint_vfmw
	mov	w3, w20
	mov	x2, x21
	mov	x1, x22
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w20
	mov	x2, x21
	mov	w4, 1
	mov	x1, x22
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x23, .LC18
	mov	x0, 30
	ldr	x1, [x23, 224]
	blr	x1
	ldr	x0, [x23, 216]
	blr	x0
	mov	w3, w20
	mov	x2, x21
	mov	x1, x22
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w20
	mov	x2, x21
	mov	x1, x22
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x21
	mov	x1, x22
	mov	w0, 32
	bl	dprint_vfmw
	b	.L35
	.p2align 3
.L39:
	ldr	w1, [x19, 128]
	str	w1, [x0, 92]
	ldr	w1, [x19, 148]
	str	w1, [x0, 112]
	ldr	w0, [x19, 60]
	cbnz	w0, .L28
	b	.L40
	.p2align 3
.L38:
	ldr	x2, .LC10
	mov	w3, w20
	ldr	x1, .LC11
	mov	w4, -121438208
	add	x2, x2, 24
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	VDH_Start_Mfde_Decode, .-VDH_Start_Mfde_Decode
	.align	3
.LC10:
	.xword	.LANCHOR0
	.align	3
.LC11:
	.xword	.LC4
	.align	3
.LC12:
	.xword	s_ScdRegPhyBaseAddr
	.align	3
.LC13:
	.xword	g_HwMem
	.align	3
.LC14:
	.xword	.LC8
	.align	3
.LC15:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC16:
	.xword	.LC1
	.align	3
.LC17:
	.xword	g_pstRegCrg
	.align	3
.LC18:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC19:
	.xword	g_LowDelaySeqIndex
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Init_Module
	.type	VDH_Init_Module, %function
VDH_Init_Module:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	bl	VDH_Reset_Global
	ldp	x29, x30, [sp], 16
	ret
	.size	VDH_Init_Module, .-VDH_Init_Module
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Start_Mfde_Module
	.type	VDH_Start_Mfde_Module, %function
VDH_Start_Mfde_Module:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w0, [x19, 2]
	cmp	w0, 1
	mov	x0, x19
	beq	.L47
	bl	VDH_Start_Mfde_Decode
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L47:
	bl	VDH_Start_Mfde_Repair
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDH_Start_Mfde_Module, .-VDH_Start_Mfde_Module
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Start_Scd_Module
	.type	VDH_Start_Scd_Module, %function
VDH_Start_Scd_Module:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	mov	w0, 49152
	ldrb	w19, [x20]
	movk	w0, 0xf8c3, lsl 16
	bl	MEM_Phy2Vir
	cbz	x0, .L55
	mov	w0, w19
	bl	ResetSCD
	ldr	x21, .LC24
	ldr	w1, [x20, 24]
	ldr	w0, [x21, w19, uxtw 2]
	add	w0, w0, 2052
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 28]
	add	w0, w0, 2056
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 32]
	add	w0, w0, 2060
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 36]
	add	w0, w0, 2064
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 40]
	add	w0, w0, 2068
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 44]
	add	w0, w0, 2072
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 52]
	add	w0, w0, 2080
	bl	MEM_WritePhyWord
	ldrb	w0, [x20, 1]
	cbnz	w0, .L56
.L51:
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 72]
	add	w0, w0, 2088
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 76]
	add	w0, w0, 2092
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 80]
	add	w0, w0, 2096
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 84]
	add	w0, w0, 2100
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 88]
	add	w0, w0, 2104
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 92]
	add	w0, w0, 2108
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 96]
	add	w0, w0, 2116
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 48]
	add	w0, w0, 2076
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	mov	w1, 0
	add	w0, w0, 2048
	bl	MEM_WritePhyWord
	ldr	w0, [x21, w19, uxtw 2]
	mov	w1, 1
	add	w0, w0, 2048
	bl	MEM_WritePhyWord
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L56:
	ldr	w0, [x21, w19, uxtw 2]
	ldr	w1, [x20, 56]
	bl	MEM_WritePhyWord
	b	.L51
	.p2align 3
.L55:
	ldr	x2, .LC21
	mov	w3, 49152
	ldr	x1, .LC23
	movk	w3, 0xf8c3, lsl 16
	add	x2, x2, 48
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	VDH_Start_Scd_Module, .-VDH_Start_Scd_Module
	.align	3
.LC21:
	.xword	.LANCHOR0
	.align	3
.LC23:
	.xword	.LC22
	.align	3
.LC24:
	.xword	s_ScdRegPhyBaseAddr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Record_CrgRegData
	.type	VDH_Record_CrgRegData, %function
VDH_Record_CrgRegData:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	w19, w1
	mov	x0, x30
	mov	w20, w2
	bl	_mcount
	cbz	w19, .L61
	cmp	w19, 1
	beq	.L62
	ldr	x2, .LC25
	mov	w4, w20
	ldr	x1, .LC27
	mov	w3, w19
	add	x2, x2, 72
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L62:
	str	w20, [x21, 4]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L61:
	str	w20, [x21]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	VDH_Record_CrgRegData, .-VDH_Record_CrgRegData
	.align	3
.LC25:
	.xword	.LANCHOR0
	.align	3
.LC27:
	.xword	.LC26
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Record_ScdRegData
	.type	VDH_Record_ScdRegData, %function
VDH_Record_ScdRegData:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w19, w1
	mov	w20, w2
	bl	_mcount
	cmp	w19, 2072
	beq	.L65
	bls	.L92
	cmp	w19, 2096
	beq	.L79
	bhi	.L80
	cmp	w19, 2080
	beq	.L81
	bls	.L93
	cmp	w19, 2088
	beq	.L84
	cmp	w19, 2092
	bne	.L64
	str	w20, [x21, 52]
	b	.L63
	.p2align 3
.L92:
	cmp	w19, 2052
	beq	.L67
	bhi	.L68
	cmp	w19, 4
	beq	.L69
	bls	.L94
	cmp	w19, 8
	beq	.L72
	cmp	w19, 12
	bne	.L64
	str	w20, [x21, 44]
	b	.L63
	.p2align 3
.L68:
	cmp	w19, 2060
	beq	.L74
	bls	.L95
	cmp	w19, 2064
	beq	.L77
	cmp	w19, 2068
	bne	.L64
	str	w20, [x21, 16]
	b	.L63
	.p2align 3
.L80:
	cmp	w19, 2104
	beq	.L86
	bls	.L96
	cmp	w19, 2108
	beq	.L89
	cmp	w19, 2116
	bne	.L64
	str	w20, [x21, 72]
.L63:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L74:
	str	w20, [x21, 8]
	b	.L63
	.p2align 3
.L69:
	str	w20, [x21, 36]
	b	.L63
	.p2align 3
.L86:
	str	w20, [x21, 64]
	b	.L63
	.p2align 3
.L81:
	str	w20, [x21, 28]
	b	.L63
	.p2align 3
.L94:
	cbnz	w19, .L64
	str	w20, [x21, 32]
	b	.L63
	.p2align 3
.L96:
	cmp	w19, 2100
	bne	.L64
	str	w20, [x21, 60]
	b	.L63
	.p2align 3
.L95:
	cmp	w19, 2056
	bne	.L64
	str	w20, [x21, 4]
	b	.L63
	.p2align 3
.L93:
	cmp	w19, 2076
	bne	.L64
	str	w20, [x21, 24]
	b	.L63
	.p2align 3
.L77:
	str	w20, [x21, 12]
	b	.L63
	.p2align 3
.L84:
	str	w20, [x21, 48]
	b	.L63
	.p2align 3
.L67:
	str	w20, [x21]
	b	.L63
	.p2align 3
.L65:
	str	w20, [x21, 20]
	b	.L63
	.p2align 3
.L72:
	str	w20, [x21, 40]
	b	.L63
	.p2align 3
.L89:
	str	w20, [x21, 68]
	b	.L63
	.p2align 3
.L64:
	ldr	x2, .LC28
	mov	w4, w20
	ldr	x1, .LC29
	mov	w3, w19
	add	x2, x2, 96
	mov	w0, 1
	bl	dprint_vfmw
	b	.L63
	.p2align 3
.L79:
	str	w20, [x21, 56]
	b	.L63
	.size	VDH_Record_ScdRegData, .-VDH_Record_ScdRegData
	.align	3
.LC28:
	.xword	.LANCHOR0
	.align	3
.LC29:
	.xword	.LC26
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Record_MfdeRegData
	.type	VDH_Record_MfdeRegData, %function
VDH_Record_MfdeRegData:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w19, w1
	mov	w20, w2
	bl	_mcount
	cmp	w19, 92
	beq	.L99
	bls	.L157
	cmp	w19, 124
	beq	.L112
	bhi	.L113
	cmp	w19, 108
	beq	.L114
	bls	.L158
	cmp	w19, 116
	beq	.L119
	cmp	w19, 120
	beq	.L120
	cmp	w19, 112
	bne	.L98
	str	w20, [x21, 48]
.L97:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L113:
	cmp	w19, 148
	beq	.L122
	bls	.L159
	cmp	w19, 156
	beq	.L126
	mov	w0, 61472
	cmp	w19, w0
	beq	.L127
	cmp	w19, 152
	beq	.L160
	.p2align 2
.L98:
	ldr	x2, .LC30
	mov	w4, w20
	ldr	x1, .LC31
	mov	w3, w19
	add	x2, x2, 120
	mov	w0, 1
	bl	dprint_vfmw
	b	.L97
	.p2align 3
.L157:
	cmp	w19, 36
	beq	.L101
	bhi	.L102
	cmp	w19, 16
	beq	.L103
	bls	.L161
	cmp	w19, 24
	beq	.L107
	cmp	w19, 32
	beq	.L108
	cmp	w19, 20
	bne	.L98
	str	w20, [x21, 12]
	b	.L97
	.p2align 3
.L158:
	cmp	w19, 100
	beq	.L116
	cmp	w19, 104
	beq	.L117
	cmp	w19, 96
	bne	.L98
	str	w20, [x21, 32]
	b	.L97
	.p2align 3
.L159:
	cmp	w19, 132
	beq	.L124
	cmp	w19, 144
	beq	.L97
	cmp	w19, 128
	bne	.L98
	str	w20, [x21, 64]
	b	.L97
	.p2align 3
.L161:
	cmp	w19, 8
	beq	.L105
	cmp	w19, 12
	bne	.L98
	str	w20, [x21, 4]
	b	.L97
	.p2align 3
.L117:
	str	w20, [x21, 40]
	b	.L97
	.p2align 3
.L108:
	str	w20, [x21, 20]
	b	.L97
	.p2align 3
.L120:
	str	w20, [x21, 56]
	b	.L97
	.p2align 3
.L105:
	str	w20, [x21]
	b	.L97
	.p2align 3
.L127:
	str	w20, [x21, 84]
	b	.L97
	.p2align 3
.L102:
	cmp	w19, 72
	beq	.L97
	bhi	.L111
	cmp	w19, 64
	beq	.L97
	cmp	w19, 68
	beq	.L97
	cmp	w19, 60
	bne	.L98
	b	.L97
	.p2align 3
.L107:
	str	w20, [x21, 16]
	b	.L97
	.p2align 3
.L160:
	str	w20, [x21, 76]
	b	.L97
	.p2align 3
.L124:
	str	w20, [x21, 68]
	b	.L97
	.p2align 3
.L119:
	str	w20, [x21, 52]
	b	.L97
	.p2align 3
.L126:
	str	w20, [x21, 80]
	b	.L97
	.p2align 3
.L114:
	str	w20, [x21, 44]
	b	.L97
	.p2align 3
.L122:
	str	w20, [x21, 72]
	b	.L97
	.p2align 3
.L103:
	str	w20, [x21, 8]
	b	.L97
	.p2align 3
.L116:
	str	w20, [x21, 36]
	b	.L97
	.p2align 3
.L112:
	str	w20, [x21, 60]
	b	.L97
	.p2align 3
.L99:
	str	w20, [x21, 28]
	b	.L97
	.p2align 3
.L101:
	str	w20, [x21, 24]
	b	.L97
	.p2align 3
.L111:
	cmp	w19, 80
	beq	.L97
	cmp	w19, 84
	beq	.L97
	cmp	w19, 76
	bne	.L98
	b	.L97
	.size	VDH_Record_MfdeRegData, .-VDH_Record_MfdeRegData
	.align	3
.LC30:
	.xword	.LANCHOR0
	.align	3
.LC31:
	.xword	.LC26
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Init_Hardware
	.type	VDH_Init_Hardware, %function
VDH_Init_Hardware:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC32
	ldr	x1, [x19, 72]
	cbz	x1, .L163
	mov	w0, 0
	blr	x1
.L163:
	ldr	x1, [x19, 144]
	cbz	x1, .L164
	mov	w0, 0
	blr	x1
.L165:
	ldr	x0, [x19, 152]
	cbz	x0, .L166
	blr	x0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L166:
	ldr	x1, .LC33
	mov	w2, 498
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L164:
	ldr	x1, .LC33
	mov	w2, 497
	mov	w0, 0
	bl	dprint_vfmw
	b	.L165
	.size	VDH_Init_Hardware, .-VDH_Init_Hardware
	.align	3
.LC32:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC33:
	.xword	.LC1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Enable_Scd_Module
	.type	VDH_Enable_Scd_Module, %function
VDH_Enable_Scd_Module:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC34
	ldr	x1, [x0, 40]
	cbz	x1, .L172
	mov	w0, w19
	blr	x1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L172:
	ldr	x1, .LC35
	mov	w2, 510
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDH_Enable_Scd_Module, .-VDH_Enable_Scd_Module
	.align	3
.LC34:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC35:
	.xword	.LC1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Disable_Scd_Module
	.type	VDH_Disable_Scd_Module, %function
VDH_Disable_Scd_Module:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC36
	ldr	x1, [x0, 48]
	cbz	x1, .L175
	mov	w0, w19
	blr	x1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L175:
	ldr	x1, .LC37
	mov	w2, 521
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDH_Disable_Scd_Module, .-VDH_Disable_Scd_Module
	.align	3
.LC36:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC37:
	.xword	.LC1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Reset_Scd_Module
	.type	VDH_Reset_Scd_Module, %function
VDH_Reset_Scd_Module:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	mov	w0, w19
	bl	ResetSCD
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDH_Reset_Scd_Module, .-VDH_Reset_Scd_Module
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Enable_Mfde_Module
	.type	VDH_Enable_Mfde_Module, %function
VDH_Enable_Mfde_Module:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC38
	ldr	x1, [x0, 8]
	cbz	x1, .L179
	mov	w0, w19
	blr	x1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L179:
	ldr	x1, .LC39
	mov	w2, 542
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDH_Enable_Mfde_Module, .-VDH_Enable_Mfde_Module
	.align	3
.LC38:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC39:
	.xword	.LC1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Disable_Mfde_Module
	.type	VDH_Disable_Mfde_Module, %function
VDH_Disable_Mfde_Module:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC40
	ldr	x1, [x0, 16]
	cbz	x1, .L182
	mov	w0, w19
	blr	x1
	mov	w0, w19
	bl	SCDDRV_SetStateIdle
	mov	w0, w19
	bl	VDMDRV_SetStateIdle
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L182:
	ldr	x1, .LC41
	mov	w2, 552
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, w19
	bl	SCDDRV_SetStateIdle
	mov	w0, w19
	bl	VDMDRV_SetStateIdle
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDH_Disable_Mfde_Module, .-VDH_Disable_Mfde_Module
	.align	3
.LC40:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC41:
	.xword	.LC1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Reset_Mfde_Module
	.type	VDH_Reset_Mfde_Module, %function
VDH_Reset_Mfde_Module:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC42
	ldr	x1, [x0, 160]
	cbz	x1, .L185
	mov	w0, w19
	blr	x1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L185:
	ldr	x1, .LC43
	mov	w2, 564
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDH_Reset_Mfde_Module, .-VDH_Reset_Mfde_Module
	.align	3
.LC42:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC43:
	.xword	.LC1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Enable_Bpd_Module
	.type	VDH_Enable_Bpd_Module, %function
VDH_Enable_Bpd_Module:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC44
	ldr	x1, [x0, 56]
	cbz	x1, .L187
	mov	w0, w19
	blr	x1
.L187:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDH_Enable_Bpd_Module, .-VDH_Enable_Bpd_Module
	.align	3
.LC44:
	.xword	g_vdm_hal_fun_ptr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Disable_Bpd_Module
	.type	VDH_Disable_Bpd_Module, %function
VDH_Disable_Bpd_Module:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC45
	ldr	x1, [x0, 64]
	cbz	x1, .L192
	mov	w0, w19
	blr	x1
.L192:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDH_Disable_Bpd_Module, .-VDH_Disable_Bpd_Module
	.align	3
.LC45:
	.xword	g_vdm_hal_fun_ptr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Enable_Dsp_Module
	.type	VDH_Enable_Dsp_Module, %function
VDH_Enable_Dsp_Module:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC46
	ldr	x1, [x0, 24]
	cbz	x1, .L198
	mov	w0, w19
	blr	x1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L198:
	ldr	x1, .LC47
	mov	w2, 594
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDH_Enable_Dsp_Module, .-VDH_Enable_Dsp_Module
	.align	3
.LC46:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC47:
	.xword	.LC1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Disable_Dsp_Module
	.type	VDH_Disable_Dsp_Module, %function
VDH_Disable_Dsp_Module:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC48
	ldr	x1, [x0, 32]
	cbz	x1, .L201
	mov	w0, w19
	blr	x1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L201:
	ldr	x1, .LC49
	mov	w2, 604
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDH_Disable_Dsp_Module, .-VDH_Disable_Dsp_Module
	.align	3
.LC48:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC49:
	.xword	.LC1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDH_Load_Dsp_Code
	.type	VDH_Load_Dsp_Code, %function
VDH_Load_Dsp_Code:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	mov	w1, w20
	mov	w0, w19
	bl	SCDDRV_LoadDspCode
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDH_Load_Dsp_Code, .-VDH_Load_Dsp_Code
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.43521, %object
	.size	__func__.43521, 22
__func__.43521:
	.string	"VDH_Start_Mfde_Repair"
	.zero	2
	.type	__func__.43529, %object
	.size	__func__.43529, 22
__func__.43529:
	.string	"VDH_Start_Mfde_Decode"
	.zero	2
	.type	__func__.43541, %object
	.size	__func__.43541, 21
__func__.43541:
	.string	"VDH_Start_Scd_Module"
	.zero	3
	.type	__func__.43547, %object
	.size	__func__.43547, 22
__func__.43547:
	.string	"VDH_Record_CrgRegData"
	.zero	2
	.type	__func__.43574, %object
	.size	__func__.43574, 22
__func__.43574:
	.string	"VDH_Record_ScdRegData"
	.zero	2
	.type	__func__.43612, %object
	.size	__func__.43612, 23
__func__.43612:
	.string	"VDH_Record_MfdeRegData"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC1:
	ASCII(.string	"VDMHAL_NULL_FUN_PRINT,L%d\n" )
	.zero	5
.LC4:
	ASCII(.string	"%s: map mfde %d register 0x%x failed!\n" )
	.zero	1
.LC8:
	ASCII(.string	"%s: WR_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\n" )
	.zero	3
.LC22:
	ASCII(.string	"%s: map scd register 0x%x failed!\n" )
	.zero	5
.LC26:
	ASCII(.string	"%s: unrecord reg 0x%x data %d!\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
