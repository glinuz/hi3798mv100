#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vdm_hal_h264.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	Write_V400R003_CabacTab
	.type	Write_V400R003_CabacTab, %function
Write_V400R003_CabacTab:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	mov	w0, w19
	bl	MEM_Phy2Vir
	cbz	x0, .L5
	ldr	x3, .LC2
	mov	x2, 5120
	ldr	x1, .LC3
	ldr	x3, [x3, 104]
	blr	x3
	mov	w0, 0
.L3:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L5:
	ldr	x1, .LC1
	mov	w2, 121
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3
	.size	Write_V400R003_CabacTab, .-Write_V400R003_CabacTab
	.align	3
.LC1:
	.xword	.LC0
	.align	3
.LC2:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC3:
	.xword	g_CabacMN
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264HAL_V400R003_InitHal
	.type	H264HAL_V400R003_InitHal, %function
H264HAL_V400R003_InitHal:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 1104]
	bl	Write_V400R003_CabacTab
	cbnz	w0, .L11
.L7:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L11:
	ldr	x1, .LC5
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L7
	.size	H264HAL_V400R003_InitHal, .-H264HAL_V400R003_InitHal
	.align	3
.LC5:
	.xword	.LC4
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Get_V400R003_VirAddr
	.type	Get_V400R003_VirAddr, %function
Get_V400R003_VirAddr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	add	x0, x19, x20, uxtw 2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Get_V400R003_VirAddr, .-Get_V400R003_VirAddr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Get_V400R003_PhyAddr
	.type	Get_V400R003_PhyAddr, %function
Get_V400R003_PhyAddr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	add	w0, w19, w20, lsl 2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Get_V400R003_PhyAddr, .-Get_V400R003_PhyAddr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264HAL_V400R003_CutSliceChain
	.type	H264HAL_V400R003_CutSliceChain, %function
H264HAL_V400R003_CutSliceChain:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	mov	w0, w19
	bl	MEM_Phy2Vir
	cbz	x0, .L17
	str	wzr, [x0, 252]
	mov	w0, 0
.L16:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L17:
	ldr	x3, .LC7
	ldr	x2, .LC8
	ldr	x1, .LC10
	bl	dprint_vfmw
	mov	w0, -1
	b	.L16
	.size	H264HAL_V400R003_CutSliceChain, .-H264HAL_V400R003_CutSliceChain
	.align	3
.LC7:
	.xword	.LC6
	.align	3
.LC8:
	.xword	.LANCHOR0
	.align	3
.LC10:
	.xword	.LC9
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Write_V400R003_Qmatrix
	.type	Write_V400R003_Qmatrix, %function
Write_V400R003_Qmatrix:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x21, x0
	mov	w23, w1
	mov	x0, x30
	mov	w20, w2
	mov	x22, x3
	bl	_mcount
	cbz	w23, .L18
	sub	w23, w23, #1
	ldr	x25, .LC14
	ldr	x24, .LC16
	add	x23, x23, 1
	mov	x19, 0
	.p2align 2
.L20:
	ldr	w0, [x21, x19, lsl 2]
	mov	w2, w19
	str	w0, [x22, x19, lsl 2]
	mov	x1, x25
	mov	w0, 4
	ldr	w3, [x21, x19, lsl 2]
	bl	dprint_vfmw
	ldr	w3, [x22, x19, lsl 2]
	mov	w2, w20
	mov	w0, 4
	mov	x1, x24
	add	x19, x19, 1
	add	w20, w20, w0
	bl	dprint_vfmw
	cmp	x19, x23
	bne	.L20
.L18:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.size	Write_V400R003_Qmatrix, .-Write_V400R003_Qmatrix
	.align	3
.LC14:
	.xword	.LC13
	.align	3
.LC16:
	.xword	.LC15
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264HAL_V400R003_SetPicMsg
	.type	H264HAL_V400R003_SetPicMsg, %function
H264HAL_V400R003_SetPicMsg:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	mov	w23, w2
	mov	w21, w3
	str	x1, [x29, 96]
	bl	_mcount
	add	w1, w23, 112
	mov	w0, w23
	str	w1, [x29, 108]
	bl	MEM_Phy2Vir
	mov	x19, x0
	cbz	x0, .L55
	ldp	w3, w2, [x20, 36]
	ldr	w0, [x20, 16]
	mov	w1, -2147483648
	lsl	w2, w2, 27
	ldr	w5, [x20, 44]
	cmp	w0, wzr
	csel	w0, w0, w1, eq
	orr	w1, w2, w3, lsl 28
	ldrb	w2, [x20, 1]
	ldr	w3, [x20, 32]
	cmp	w2, 1
	ldrb	w2, [x20]
	add	x3, x3, x2, lsl 1
	ldr	x2, .LC19
	add	x2, x2, 64
	ldr	w4, [x2, x3, lsl 2]
	ldr	w3, [x20, 20]
	sub	w3, w3, #1
	lsl	w3, w3, 16
	orr	w3, w3, w4, lsl 14
	ldr	w4, [x20, 28]
	sub	w4, w4, #1
	orr	w4, w4, w5, lsl 26
	orr	w2, w4, w1
	ldr	x1, .LC22
	orr	w4, w3, w2
	mov	w3, 33554432
	csel	w3, wzr, w3, ne
	mov	w2, w23
	orr	w0, w0, w3
	orr	w0, w4, w0
	str	w0, [x19]
	mov	w0, 4
	ldr	w3, [x19]
	bl	dprint_vfmw
	ldr	w0, [x20, 296]
	add	w2, w23, 4
	ldr	x1, .LC24
	and	w0, w0, -16
	str	w0, [x19, 4]
	mov	w0, 4
	ldr	w3, [x19, 4]
	bl	dprint_vfmw
	ldr	w0, [x20, 48]
	add	w2, w23, 8
	str	w0, [x19, 8]
	mov	w0, 4
	ldr	x1, .LC26
	ldr	w3, [x19, 8]
	bl	dprint_vfmw
	ldr	w0, [x20, 32]
	cbnz	w0, .L56
	str	wzr, [x19, 12]
	add	w2, w23, 12
	ldr	x1, .LC28
	mov	w0, 4
	ldr	w3, [x19, 12]
	str	wzr, [x19, 16]
.L53:
	bl	dprint_vfmw
	ldr	x1, .LC30
	add	w2, w23, 16
	ldr	w3, [x19, 16]
	mov	w0, 4
	bl	dprint_vfmw
	ldr	x22, [x29, 96]
	ldr	x1, .LC32
	add	w2, w23, 20
	mov	w0, 4
	ldr	w3, [x22, 1160]
	and	w3, w3, -16
	str	w3, [x19, 20]
	ldr	w3, [x19, 20]
	bl	dprint_vfmw
	ldr	w0, [x22, 1164]
	add	w2, w23, 24
	ldr	x1, .LC34
	and	w0, w0, -16
	str	w0, [x19, 24]
	mov	w0, 4
	ldr	w3, [x19, 24]
	bl	dprint_vfmw
	ldrb	w0, [x20]
	cmp	w0, 2
	beq	.L30
	ldr	w0, [x20, 300]
	add	x0, x20, x0, lsl 2
	ldr	w0, [x0, 700]
	and	w0, w0, -16
.L31:
	str	w0, [x19, 28]
	add	w2, w23, 28
	ldr	x1, .LC36
	mov	w0, 4
	ldr	w3, [x19, 28]
	and	w21, w21, -16
	bl	dprint_vfmw
	ldr	x0, [x29, 96]
	add	w2, w23, 32
	ldr	x1, .LC38
	ldr	w0, [x0, 1168]
	and	w0, w0, -16
	str	w0, [x19, 32]
	mov	w0, 4
	ldr	w3, [x19, 32]
	bl	dprint_vfmw
	str	w21, [x19, 36]
	ldr	x1, .LC40
	add	w2, w23, 36
	ldr	w3, [x19, 36]
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w21, [x20, 1000]
	ldr	x22, .LC42
	cbz	w21, .L32
	add	w27, w23, 40
	mov	w24, 0
	.p2align 2
.L33:
	uxtw	x0, w24
	mov	w3, w27
	add	x5, x0, 10
	add	w2, w24, 10
	add	x4, x20, x0, lsl 2
	mov	x1, x22
	mov	w0, 4
	add	w24, w24, 1
	add	w27, w27, w0
	ldr	w4, [x4, 808]
	add	x4, x20, x4, lsl 2
	ldr	w4, [x4, 308]
	and	w4, w4, -16
	str	w4, [x19, x5, lsl 2]
	ldr	w4, [x19, x5, lsl 2]
	bl	dprint_vfmw
	ldr	w21, [x20, 1000]
	cmp	w21, w24
	bhi	.L33
	cmp	w21, 15
	bhi	.L37
.L32:
	add	w2, w21, 10
	add	w24, w23, w2, lsl 2
	.p2align 2
.L36:
	ldr	w5, [x20, 808]
	uxtw	x4, w21
	add	x4, x4, 10
	mov	w3, w24
	mov	w0, 4
	mov	x1, x22
	add	x5, x20, x5, lsl 2
	add	w21, w21, 1
	add	w24, w24, w0
	ldr	w5, [x5, 308]
	and	w5, w5, -16
	str	w5, [x19, x4, lsl 2]
	ldr	w4, [x19, x4, lsl 2]
	bl	dprint_vfmw
	cmp	w21, 15
	add	w2, w21, 10
	bls	.L36
.L37:
	ldr	x0, [x29, 96]
	add	w2, w23, 104
	ldr	x1, .LC44
	ldr	w3, [x0, 1104]
	mov	w0, 4
	and	w3, w3, -16
	str	w3, [x19, 104]
	ldr	w3, [x19, 104]
	bl	dprint_vfmw
	ldr	w21, [x20, 1000]
	cbz	w21, .L57
	add	w24, w23, 108
	mov	w28, 27
	mov	w26, w24
	mov	w27, 0
	.p2align 2
.L39:
	add	x25, x20, x27, uxtw 2
	ubfiz	x4, x27, 1, 31
	add	x4, x4, 27
	mov	w3, w26
	lsl	x21, x4, 2
	mov	w2, w28
	ldr	w6, [x25, 872]
	mov	x1, x22
	str	w6, [x19, x4, lsl 2]
	mov	w0, 4
	add	w27, w27, 1
	ldr	w4, [x19, w28, uxtw 2]
	bl	dprint_vfmw
	add	x4, x21, 4
	add	w6, w28, 1
	ldr	w0, [x25, 936]
	add	w3, w26, 4
	mov	w2, w6
	mov	x1, x22
	str	w0, [x19, x4]
	mov	w0, 4
	add	w28, w28, 2
	add	w26, w26, 8
	ldr	w4, [x19, w6, uxtw 2]
	bl	dprint_vfmw
	ldr	w21, [x20, 1000]
	cmp	w21, w27
	bhi	.L39
	cmp	w21, 15
	bhi	.L41
.L38:
	lsl	w27, w21, 1
	add	w28, w24, w21, lsl 3
	.p2align 2
.L40:
	uxtw	x4, w27
	add	w6, w27, 27
	add	x4, x4, 27
	mov	w3, w28
	lsl	x26, x4, 2
	mov	w2, w6
	mov	x1, x22
	mov	w0, 4
	str	wzr, [x19, x4, lsl 2]
	add	w21, w21, 1
	ldr	w4, [x19, w6, uxtw 2]
	bl	dprint_vfmw
	add	x5, x26, 4
	add	w4, w27, 28
	ldr	w0, [x29, 108]
	sub	w3, w28, w24
	mov	w2, w4
	mov	x1, x22
	str	wzr, [x19, x5]
	add	w3, w3, w0
	mov	w0, 4
	add	w27, w27, 2
	ldr	w4, [x19, w4, uxtw 2]
	add	w28, w28, 8
	bl	dprint_vfmw
	cmp	w21, 15
	bls	.L40
.L41:
	ldr	x0, [x29, 96]
	add	x3, x19, 256
	add	w2, w23, 256
	mov	w1, 56
	ldr	w0, [x0, 1176]
	and	w0, w0, -16
	str	w0, [x19, 236]
	add	x0, x20, 60
	ldr	w4, [x20, 292]
	add	x4, x20, x4, lsl 2
	ldr	w4, [x4, 564]
	and	w4, w4, -16
	str	w4, [x19, 240]
	ldr	w4, [x20, 692]
	str	w4, [x19, 244]
	bl	Write_V400R003_Qmatrix
	mov	w0, 0
.L25:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L56:
	ldr	w0, [x20, 52]
	add	w2, w23, 12
	str	w0, [x19, 12]
	mov	w0, 4
	ldr	x1, .LC28
	ldr	w3, [x20, 56]
	str	w3, [x19, 16]
	ldr	w3, [x19, 12]
	b	.L53
	.p2align 3
.L30:
	ldr	w1, [x20, 300]
	ldr	w0, [x20, 784]
	add	x1, x20, x1, lsl 2
	ldr	w1, [x1, 700]
	add	w0, w1, w0
	and	w0, w0, -16
	b	.L31
.L57:
	add	w24, w23, 108
	b	.L38
.L55:
	ldr	x2, .LC19
	mov	w0, 0
	ldr	x3, .LC18
	ldr	x1, .LC20
	add	x2, x2, 32
	bl	dprint_vfmw
	mov	w0, -1
	b	.L25
	.size	H264HAL_V400R003_SetPicMsg, .-H264HAL_V400R003_SetPicMsg
	.align	3
.LC18:
	.xword	.LC17
	.align	3
.LC19:
	.xword	.LANCHOR0
	.align	3
.LC20:
	.xword	.LC9
	.align	3
.LC22:
	.xword	.LC21
	.align	3
.LC24:
	.xword	.LC23
	.align	3
.LC26:
	.xword	.LC25
	.align	3
.LC28:
	.xword	.LC27
	.align	3
.LC30:
	.xword	.LC29
	.align	3
.LC32:
	.xword	.LC31
	.align	3
.LC34:
	.xword	.LC33
	.align	3
.LC36:
	.xword	.LC35
	.align	3
.LC38:
	.xword	.LC37
	.align	3
.LC40:
	.xword	.LC39
	.align	3
.LC42:
	.xword	.LC41
	.align	3
.LC44:
	.xword	.LC43
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264HAL_V400R003_SetSliceMsg
	.type	H264HAL_V400R003_SetSliceMsg, %function
H264HAL_V400R003_SetSliceMsg:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x22, x0
	mov	x0, x30
	mov	w21, w3
	mov	x20, x2
	stp	w7, w5, [x29, 108]
	bl	_mcount
	mov	w0, w21
	bl	MEM_Phy2Vir
	mov	x26, x0
	cbz	x0, .L214
	ldr	x24, .LC52
	add	x6, x20, 8
	add	x28, x0, 4
	mov	x27, x0
	add	w23, w21, 4
	add	x25, x0, 8
	mov	w19, 0
.L63:
	ldr	w4, [x6]
	add	w11, w19, 1
	str	w11, [x29, 136]
	mov	w2, w11
	ldr	x1, .LC50
	mov	w0, 4
	sub	w3, w23, #4
	cbz	w4, .L61
	ldr	w10, [x22, 800]
	ldr	w4, [x6, 8]
	and	w13, w10, -16
	ldr	w10, [x6, 16]
	str	w13, [x29, 116]
	mov	w5, w10
	str	x6, [x29, 120]
	add	w12, w4, w10, lsl 3
	and	w12, w12, 127
	mov	w3, w12
	stp	w10, w12, [x29, 128]
	bl	dprint_vfmw
	ldp	w12, w11, [x29, 132]
	str	w12, [x28]
	mov	w3, w23
	mov	w2, w11
	mov	x1, x24
	ldr	w4, [x28]
	mov	w0, 4
	bl	dprint_vfmw
	ldr	x6, [x29, 120]
	add	w3, w23, 4
	add	w2, w19, 2
	mov	x1, x24
	str	x6, [x29, 136]
	ldr	w0, [x6]
	str	w0, [x25]
	mov	w0, 4
	ldr	w4, [x25]
	bl	dprint_vfmw
	ldr	w10, [x29, 128]
	sub	w3, w23, #4
	ldr	w13, [x29, 116]
	mov	w2, w19
	and	w10, w10, -16
	mov	x1, x24
	sub	w10, w10, w13
	str	w10, [x27]
	mov	w0, 4
	ldr	w4, [x27]
	bl	dprint_vfmw
	ldr	x6, [x29, 136]
.L62:
	add	w19, w19, 3
	add	x6, x6, 4
	cmp	w19, 6
	add	x28, x28, 12
	add	x27, x27, 12
	add	w23, w23, 12
	add	x25, x25, 12
	bne	.L63
	ldr	w1, [x20, 48]
	add	w2, w21, 24
	ldp	w3, w0, [x20, 32]
	and	w1, w1, 1048575
	orr	w1, w1, w3, lsl 26
	ubfiz	w0, w0, 24, 2
	orr	w0, w1, w0
	str	w0, [x26, 24]
	ldr	x1, .LC54
	mov	w0, 4
	ldr	w3, [x26, 24]
	bl	dprint_vfmw
	ldr	x0, .LC55
	ldrb	w4, [x20, 3]
	ldrb	w0, [x0]
	cbnz	w0, .L65
	cbz	w4, .L215
.L65:
	ldp	w2, w3, [x20, 52]
	ldp	w0, w1, [x20, 40]
	ldr	x23, .LC47
	lsl	w2, w2, 2
	ldrb	w5, [x20]
	ubfiz	w1, w1, 16, 5
	add	x19, x23, 128
	ubfiz	w0, w0, 21, 5
	orr	w0, w0, w1
	orr	w1, w2, w3, lsl 8
	orr	w2, w0, w1
	ldrb	w1, [x20, 2]
	ldr	w0, [x19, w5, sxtw 2]
	orr	w1, w0, w1, lsl 15
	orr	w0, w2, w1
	ldr	x1, .LC57
	orr	w0, w0, w4, lsl 14
	str	w0, [x26, 28]
	add	w2, w21, 28
	mov	w0, 4
	ldr	w3, [x26, 28]
	bl	dprint_vfmw
	ldrb	w0, [x20]
	ldr	w0, [x19, w0, sxtw 2]
	cmp	w0, 2
	beq	.L216
	mov	w3, 0
	mov	w0, 0
.L66:
	str	w0, [x26, 32]
	add	w2, w21, 32
	ldr	x1, .LC61
	mov	w0, 4
	str	w3, [x26, 36]
	ldr	w3, [x26, 32]
	bl	dprint_vfmw
	ldr	x1, .LC63
	add	w2, w21, 36
	ldr	w3, [x26, 36]
	mov	w0, 4
	bl	dprint_vfmw
	ldrb	w2, [x20, 4]
	ldp	w0, w1, [x20, 60]
	ubfiz	w0, w0, 5, 5
	and	w1, w1, 31
	orr	w0, w0, w2, lsl 16
	add	w2, w21, 40
	orr	w0, w0, w1
	str	w0, [x26, 40]
	ldr	x1, .LC65
	mov	w0, 4
	ldr	w3, [x26, 40]
	bl	dprint_vfmw
	ldp	w0, w1, [x20, 68]
	ldr	w2, [x20, 76]
	ubfiz	w1, w1, 8, 4
	ubfiz	w0, w0, 16, 4
	orr	w0, w0, w1
	ldr	x1, .LC67
	orr	w0, w0, w2
	str	w0, [x26, 44]
	add	w2, w21, 44
	mov	w0, 4
	ldr	w3, [x26, 44]
	bl	dprint_vfmw
	ldr	x0, [x20, 4056]
	cbz	x0, .L71
	ldr	w25, [x29, 108]
.L72:
	ldr	w0, [x29, 112]
	str	w25, [x26, 176]
	and	w27, w0, -16
	str	w27, [x26, 252]
	add	x0, x23, 128
	ldrb	w1, [x20]
	ldr	w0, [x0, w1, sxtw 2]
	cbz	w0, .L135
	ldrb	w0, [x22]
	ldr	w2, [x20, 52]
	cbz	w0, .L217
	ldr	x1, .LC75
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w0, [x20, 52]
	cbz	w0, .L93
	mov	w28, 0
	mov	w24, 0
	mov	w25, 0
	mov	w19, 20
	mov	w7, 12
	mov	w9, 8
	mov	w8, 4
	b	.L111
	.p2align 3
.L94:
	cmp	w2, 2
	beq	.L102
	and	w27, w25, 3
	and	w2, w25, 7
.L101:
	cmp	w2, 7
	beq	.L218
.L109:
	cmp	w27, 3
	beq	.L219
.L110:
	ldr	w0, [x20, 52]
	add	w25, w25, 1
	cmp	w0, w25
	bls	.L220
.L111:
	uxtw	x1, w25
	lsl	x0, x1, 5
	add	x0, x0, x1, lsl 2
	add	x0, x20, x0
	ldrb	w2, [x0, 1624]
	cmp	w2, 1
	bne	.L94
	ldrb	w2, [x0, 1625]
	cmp	w2, 3
	beq	.L95
	ldrb	w2, [x0, 1628]
	ldrb	w0, [x0, 1629]
	sbfiz	x3, x2, 1, 32
	cmp	w0, 1
	ldr	w0, [x22, 32]
	add	x0, x3, x0
	add	x3, x23, 176
	ldr	w0, [x3, x0, lsl 2]
	and	w4, w0, 3
	csel	w0, wzr, w9, ne
	cmp	w2, 1
.L210:
	csel	w3, wzr, w8, ne
	and	w2, w25, 7
	orr	w0, w0, w3
	lsl	w3, w2, 2
	orr	w0, w4, w0
	and	w27, w25, 3
	lsl	w0, w0, w3
	cmp	w2, 7
	orr	w24, w24, w0
	lsl	x0, x1, 5
	add	x1, x0, x1, lsl 2
	add	x1, x20, x1
	ldr	w0, [x1, 1644]
	add	w1, w27, w27, lsl 2
	ubfiz	w0, w0, 1, 4
	lsl	w0, w0, w1
	orr	w28, w28, w0
	bne	.L109
.L218:
	str	w24, [x26, w7, uxtw 2]
	add	w3, w21, w7, lsl 2
	ldr	x1, .LC71
	mov	w2, w7
	ldr	w4, [x26, w7, uxtw 2]
	mov	w0, 4
	add	w7, w7, 1
	str	w7, [x29, 136]
	stp	w8, w9, [x29, 128]
	bl	dprint_vfmw
	mov	w24, 0
	cmp	w27, 3
	ldr	w7, [x29, 136]
	ldp	w8, w9, [x29, 128]
	bne	.L110
	.p2align 2
.L219:
	str	w28, [x26, w19, uxtw 2]
	add	w3, w21, w19, lsl 2
	ldr	x1, .LC71
	mov	w2, w19
	ldr	w4, [x26, w19, uxtw 2]
	mov	w0, 4
	stp	w8, w9, [x29, 128]
	str	w7, [x29, 136]
	add	w25, w25, 1
	bl	dprint_vfmw
	add	w19, w19, 1
	ldr	w0, [x20, 52]
	mov	w28, 0
	ldp	w8, w9, [x29, 128]
	cmp	w0, w25
	ldr	w7, [x29, 136]
	bhi	.L111
	.p2align 2
.L220:
	and	w0, w0, 7
	cbnz	w0, .L221
.L93:
	add	x0, x23, 128
	ldrb	w1, [x20]
	ldr	w0, [x0, w1, sxtw 2]
	cmp	w0, 2
	bne	.L85
	ldr	x1, .LC77
	mov	w0, 4
	ldr	w2, [x20, 56]
	bl	dprint_vfmw
	ldr	w0, [x20, 56]
	cbz	w0, .L85
	mov	w28, 0
	mov	w24, 0
	mov	w25, 0
	mov	w19, 28
	mov	w7, 16
	mov	w9, 8
	mov	w8, 4
	b	.L130
	.p2align 3
.L113:
	cmp	w2, 2
	beq	.L121
	and	w27, w25, 3
	and	w2, w25, 7
.L120:
	cmp	w2, 7
	beq	.L222
.L128:
	cmp	w27, 3
	beq	.L223
.L129:
	ldr	w0, [x20, 56]
	add	w25, w25, 1
	cmp	w0, w25
	bls	.L224
.L130:
	uxtw	x1, w25
	lsl	x0, x1, 5
	add	x0, x0, x1, lsl 2
	add	x0, x20, x0
	ldrb	w2, [x0, 2776]
	cmp	w2, 1
	bne	.L113
	ldrb	w2, [x0, 2777]
	cmp	w2, 3
	beq	.L114
	ldrb	w2, [x0, 2780]
	ldrb	w0, [x0, 2781]
	sbfiz	x3, x2, 1, 32
	cmp	w0, 1
	ldr	w0, [x22, 32]
	add	x0, x3, x0
	add	x3, x23, 176
	ldr	w0, [x3, x0, lsl 2]
	and	w4, w0, 3
	csel	w0, wzr, w9, ne
	cmp	w2, 1
.L212:
	csel	w3, wzr, w8, ne
	and	w2, w25, 7
	orr	w0, w0, w3
	lsl	w3, w2, 2
	orr	w0, w4, w0
	and	w27, w25, 3
	lsl	w0, w0, w3
	orr	w24, w24, w0
	lsl	x0, x1, 5
	add	x0, x0, x1, lsl 2
	add	w1, w27, w27, lsl 2
	add	x0, x20, x0
	ldr	w0, [x0, 2796]
	ubfiz	w0, w0, 1, 4
	lsl	w0, w0, w1
	orr	w28, w28, w0
	b	.L120
	.p2align 3
.L61:
	str	wzr, [x27]
	mov	w2, w19
	mov	x1, x24
	mov	w0, 4
	ldr	w4, [x27]
	str	x6, [x29, 136]
	bl	dprint_vfmw
	str	wzr, [x28]
	mov	w3, w23
	add	w2, w19, 1
	ldr	w4, [x28]
	mov	x1, x24
	mov	w0, 4
	bl	dprint_vfmw
	str	wzr, [x25]
	add	w3, w23, 4
	add	w2, w19, 2
	ldr	w4, [x25]
	mov	x1, x24
	mov	w0, 4
	bl	dprint_vfmw
	ldr	x6, [x29, 136]
	b	.L62
	.p2align 3
.L230:
	ldr	w0, [x22, 284]
	cbz	w0, .L133
	add	x0, x23, 128
	ldrb	w1, [x20]
	ldr	w0, [x0, w1, sxtw 2]
	cmp	w0, 1
	beq	.L134
.L133:
	ldr	w0, [x22, 288]
	cmp	w0, 1
	bne	.L135
	add	x0, x23, 128
	ldrb	w1, [x20]
	ldr	w0, [x0, w1, sxtw 2]
	cmp	w0, 2
	bne	.L135
.L134:
	ldr	x1, .LC79
	mov	w0, 4
	ldr	w2, [x20, 44]
	add	x28, x26, 256
	add	x27, x26, 512
	add	w25, w21, 256
	add	x26, x26, 768
	mov	w19, 0
	bl	dprint_vfmw
	ldr	x24, .LC71
	.p2align 2
.L136:
	add	x22, x20, x19, uxtw 2
	ldr	w4, [x20, 80]
	mov	w3, w25
	add	w2, w19, 64
	and	w5, w4, 7
	mov	x1, x24
	ldrb	w6, [x22, 856]
	mov	w0, 4
	ldr	w4, [x22, 88]
	ubfiz	w4, w4, 3, 9
	orr	w4, w4, w6, lsl 12
	orr	w4, w4, w5
	str	w4, [x28, w19, uxtw 2]
	ldr	w4, [x28, w19, uxtw 2]
	bl	dprint_vfmw
	ldrb	w1, [x22, 984]
	add	w3, w25, 256
	ldr	w0, [x22, 216]
	add	w2, w19, 128
	ldr	w4, [x20, 84]
	ubfiz	w0, w0, 3, 9
	and	w4, w4, 7
	orr	w0, w0, w1, lsl 12
	orr	w0, w0, w4
	str	w0, [x27, w19, uxtw 2]
	mov	x1, x24
	mov	w0, 4
	ldr	w4, [x27, w19, uxtw 2]
	bl	dprint_vfmw
	ldrb	w5, [x22, 1112]
	add	w3, w25, 512
	ldr	w4, [x22, 344]
	add	w2, w19, 192
	mov	w0, 4
	mov	x1, x24
	and	w4, w4, 511
	add	w25, w25, w0
	orr	w4, w4, w5, lsl 9
	str	w4, [x26, w19, uxtw 2]
	ldr	w4, [x26, w19, uxtw 2]
	add	w19, w19, 1
	bl	dprint_vfmw
	ldr	w0, [x20, 44]
	cmp	w0, w19
	bcs	.L136
	ldr	x1, .LC81
	mov	w0, 4
	ldr	w2, [x20, 80]
	mov	w19, 0
	bl	dprint_vfmw
	ldr	x25, .LC83
	.p2align 2
.L137:
	add	x22, x20, x19, uxtw 2
	mov	x1, x25
	mov	w0, 4
	add	w19, w19, 1
	ldr	w2, [x22, 88]
	bl	dprint_vfmw
	ldr	x1, .LC85
	mov	w0, 4
	ldr	w2, [x22, 856]
	bl	dprint_vfmw
	ldr	w0, [x20, 44]
	cmp	w0, w19
	bcs	.L137
	add	x0, x23, 128
	ldrb	w1, [x20]
	ldr	w0, [x0, w1, sxtw 2]
	cmp	w0, 2
	beq	.L138
.L141:
	ldr	x1, .LC87
	mov	w0, 4
	ldr	w2, [x20, 84]
	mov	w19, 0
	bl	dprint_vfmw
	ldr	x25, .LC93
	.p2align 2
.L139:
	add	x22, x20, x19, uxtw 2
	mov	x1, x25
	mov	w0, 4
	add	w19, w19, 1
	ldr	w2, [x22, 216]
	bl	dprint_vfmw
	ldr	x1, .LC95
	mov	w0, 4
	ldr	w2, [x22, 984]
	bl	dprint_vfmw
	ldr	w0, [x20, 44]
	cmp	w0, w19
	bcs	.L139
	add	x0, x23, 128
	ldrb	w1, [x20]
	ldr	w0, [x0, w1, sxtw 2]
	cmp	w0, 2
	beq	.L142
.L145:
	ldr	x25, .LC101
	mov	w19, 0
	.p2align 2
.L143:
	add	x22, x20, x19, uxtw 2
	mov	x1, x25
	mov	w0, 4
	add	w19, w19, 1
	ldr	w2, [x22, 344]
	bl	dprint_vfmw
	ldr	x1, .LC103
	mov	w0, 4
	ldr	w2, [x22, 1112]
	bl	dprint_vfmw
	ldr	w0, [x20, 44]
	cmp	w0, w19
	bcs	.L143
	add	x0, x23, 128
	ldrb	w1, [x20]
	ldr	w0, [x0, w1, sxtw 2]
	cmp	w0, 2
	beq	.L225
	.p2align 2
.L135:
	mov	w0, 0
.L60:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L215:
	ldp	w1, w0, [x22, 24]
	mul	w0, w0, w1
	cmp	w0, 1620
	bls	.L65
	mov	w0, -1
	b	.L60
	.p2align 3
.L217:
	ldr	x1, .LC69
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w0, [x20, 52]
	cbz	w0, .L78
	mov	w27, 0
	mov	w28, 0
	mov	w25, 0
	mov	w19, 20
	mov	w7, 12
	mov	w24, 8
	b	.L82
	.p2align 3
.L80:
	cmp	w8, 3
	beq	.L226
.L81:
	ldr	w0, [x20, 52]
	cmp	w0, w25
	bls	.L227
.L82:
	uxtw	x0, w25
	ldr	w4, [x22, 32]
	lsl	x1, x0, 5
	add	x10, x23, 144
	add	x0, x1, x0, lsl 2
	and	w8, w25, 3
	add	x0, x20, x0
	and	w3, w25, 7
	lsl	w9, w3, 2
	add	w11, w8, w8, lsl 2
	add	w25, w25, 1
	ldrb	w1, [x0, 1626]
	ldrb	w12, [x0, 1627]
	ldr	w2, [x0, 1644]
	add	x4, x4, x1, lsl 1
	cmp	w12, 1
	csel	w1, wzr, w24, ne
	ubfiz	w2, w2, 1, 4
	cmp	w3, 7
	lsl	w2, w2, w11
	ldr	w0, [x10, x4, lsl 2]
	orr	w27, w27, w2
	and	w0, w0, 3
	orr	w0, w0, w1
	lsl	w0, w0, w9
	orr	w28, w28, w0
	bne	.L80
	str	w28, [x26, w7, uxtw 2]
	add	w3, w21, w7, lsl 2
	ldr	x1, .LC71
	mov	w2, w7
	ldr	w4, [x26, w7, uxtw 2]
	mov	w0, 4
	add	w7, w7, 1
	mov	w28, 0
	stp	w8, w7, [x29, 132]
	bl	dprint_vfmw
	ldp	w8, w7, [x29, 132]
	cmp	w8, 3
	bne	.L81
	.p2align 2
.L226:
	str	w27, [x26, w19, uxtw 2]
	add	w3, w21, w19, lsl 2
	ldr	x1, .LC71
	mov	w2, w19
	ldr	w4, [x26, w19, uxtw 2]
	mov	w0, 4
	str	w7, [x29, 136]
	add	w19, w19, 1
	mov	w27, 0
	bl	dprint_vfmw
	ldr	w0, [x20, 52]
	ldr	w7, [x29, 136]
	cmp	w0, w25
	bhi	.L82
	.p2align 2
.L227:
	and	w0, w0, 7
	cbnz	w0, .L228
.L78:
	ldr	x1, .LC73
	mov	w0, 4
	ldr	w2, [x20, 56]
	bl	dprint_vfmw
	add	x0, x23, 128
	ldrb	w1, [x20]
	ldr	w0, [x0, w1, sxtw 2]
	cmp	w0, 2
	beq	.L229
.L85:
	ldr	w2, [x20, 3928]
	add	x25, x20, 3932
	ldr	x24, .LC71
	mov	w19, 0
	mov	w27, 36
	and	w2, w2, 31
	.p2align 2
.L91:
	add	w19, w19, 1
	and	w1, w19, 3
	cmp	w19, 32
	add	w3, w1, w1, lsl 2
	beq	.L230
	ldr	w0, [x25]
	cmp	w1, 3
	add	x25, x25, 4
	and	w0, w0, 31
	lsl	w0, w0, w3
	orr	w2, w2, w0
	bne	.L91
	str	w2, [x26, w27, uxtw 2]
	add	w3, w21, w27, lsl 2
	mov	w2, w27
	mov	x1, x24
	ldr	w4, [x26, w27, uxtw 2]
	mov	w0, 4
	add	w27, w27, 1
	bl	dprint_vfmw
	mov	w2, 0
	b	.L91
	.p2align 3
.L102:
	ldrb	w2, [x0, 1625]
	cmp	w2, 3
	beq	.L103
	ldrb	w2, [x0, 1630]
	ldrb	w0, [x0, 1631]
	sbfiz	x3, x2, 1, 32
	cmp	w0, 1
	ldr	w0, [x22, 32]
	add	x0, x3, x0
	add	x3, x23, 176
	ldr	w0, [x3, x0, lsl 2]
	and	w4, w0, 3
	csel	w0, wzr, w9, ne
	cmp	w2, 1
.L211:
	csel	w3, wzr, w8, ne
	and	w2, w25, 7
	orr	w0, w0, w3
	lsl	w3, w2, 2
	orr	w0, w4, w0
	and	w27, w25, 3
	lsl	w0, w0, w3
	orr	w24, w24, w0
	lsl	x0, x1, 5
	add	x0, x0, x1, lsl 2
	add	w1, w27, w27, lsl 2
	add	x0, x20, x0
	ldr	w0, [x0, 1644]
	ubfiz	w0, w0, 1, 4
	orr	w0, w0, 1
	lsl	w0, w0, w1
	orr	w28, w28, w0
	b	.L101
	.p2align 3
.L95:
	ldrb	w2, [x0, 1626]
	ldr	w3, [x22, 32]
	add	x2, x3, x2, lsl 1
	ldrb	w3, [x0, 1629]
	cmp	w3, 1
	ldrb	w3, [x0, 1628]
	add	x0, x23, 176
	ldr	w0, [x0, x2, lsl 2]
	and	w4, w0, 3
	csel	w0, wzr, w9, ne
	cmp	w3, 1
	b	.L210
.L228:
	str	w28, [x26, w7, uxtw 2]
	add	w3, w21, w7, lsl 2
	ldr	x24, .LC71
	mov	w2, w7
	ldr	w4, [x26, w7, uxtw 2]
	mov	w0, 4
	mov	x1, x24
	bl	dprint_vfmw
	str	w27, [x26, w19, uxtw 2]
	add	w3, w21, w19, lsl 2
	mov	w2, w19
	ldr	w4, [x26, w19, uxtw 2]
	mov	x1, x24
	mov	w0, 4
	bl	dprint_vfmw
	b	.L78
.L221:
	str	w24, [x26, w7, uxtw 2]
	add	w3, w21, w7, lsl 2
	ldr	x24, .LC71
	mov	w2, w7
	ldr	w4, [x26, w7, uxtw 2]
	mov	w0, 4
	mov	x1, x24
	bl	dprint_vfmw
	str	w28, [x26, w19, uxtw 2]
	add	w3, w21, w19, lsl 2
	mov	w2, w19
	ldr	w4, [x26, w19, uxtw 2]
	mov	x1, x24
	mov	w0, 4
	bl	dprint_vfmw
	b	.L93
.L103:
	ldrb	w2, [x0, 1626]
	ldr	w3, [x22, 32]
	add	x2, x3, x2, lsl 1
	ldrb	w3, [x0, 1631]
	cmp	w3, 1
	ldrb	w3, [x0, 1630]
	add	x0, x23, 176
	ldr	w0, [x0, x2, lsl 2]
	and	w4, w0, 3
	csel	w0, wzr, w9, ne
	cmp	w3, 1
	b	.L211
.L71:
	ldr	w0, [x22, 804]
	ldrb	w1, [x20, 1]
	cmp	w0, 262144
	ble	.L74
	add	w25, w1, 1
	mov	w0, 262143
	sdiv	w25, w0, w25
	b	.L72
.L216:
	ldr	x1, .LC59
	mov	w0, 4
	ldr	w2, [x20, 2788]
	bl	dprint_vfmw
	ldrb	w0, [x22]
	cbnz	w0, .L67
	ldrb	w0, [x20, 2778]
	cmp	w0, 3
	ldr	w0, [x20, 2788]
	beq	.L231
.L208:
	add	x0, x22, x0, lsl 2
	mov	w3, 0
	ldr	w0, [x0, 700]
	and	w0, w0, -16
	b	.L66
.L229:
	ldr	w0, [x20, 56]
	cbz	w0, .L85
	mov	w27, 0
	mov	w28, 0
	mov	w25, 0
	mov	w19, 28
	mov	w6, 16
	mov	w24, 8
	b	.L90
	.p2align 3
.L88:
	cmp	w8, 3
	beq	.L232
.L89:
	ldr	w0, [x20, 56]
	cmp	w0, w25
	bls	.L233
.L90:
	uxtw	x0, w25
	ldr	w9, [x22, 32]
	lsl	x1, x0, 5
	and	w3, w25, 7
	add	x0, x1, x0, lsl 2
	add	x1, x23, 144
	add	x0, x20, x0
	and	w8, w25, 3
	lsl	w4, w3, 2
	add	w25, w25, 1
	ldrb	w2, [x0, 2778]
	add	x2, x9, x2, lsl 1
	ldr	w1, [x1, x2, lsl 2]
	ldrb	w2, [x0, 2779]
	and	w1, w1, 3
	ldr	w0, [x0, 2796]
	cmp	w2, 1
	csel	w2, wzr, w24, ne
	ubfiz	w0, w0, 1, 4
	orr	w1, w1, w2
	cmp	w3, 7
	lsl	w1, w1, w4
	orr	w28, w28, w1
	add	w1, w8, w8, lsl 2
	lsl	w0, w0, w1
	orr	w27, w27, w0
	bne	.L88
	str	w28, [x26, w6, uxtw 2]
	add	w3, w21, w6, lsl 2
	ldr	x1, .LC71
	mov	w2, w6
	ldr	w4, [x26, w6, uxtw 2]
	mov	w0, 4
	add	w6, w6, 1
	mov	w28, 0
	stp	w8, w6, [x29, 132]
	bl	dprint_vfmw
	ldp	w8, w6, [x29, 132]
	cmp	w8, 3
	bne	.L89
.L232:
	str	w27, [x26, w19, uxtw 2]
	add	w3, w21, w19, lsl 2
	ldr	x1, .LC71
	mov	w2, w19
	ldr	w4, [x26, w19, uxtw 2]
	mov	w0, 4
	str	w6, [x29, 136]
	add	w19, w19, 1
	mov	w27, 0
	bl	dprint_vfmw
	ldr	w6, [x29, 136]
	b	.L89
.L233:
	and	w0, w0, 7
	cbz	w0, .L85
	str	w28, [x26, w6, uxtw 2]
	add	w3, w21, w6, lsl 2
	ldr	x24, .LC71
	mov	w2, w6
	ldr	w4, [x26, w6, uxtw 2]
	mov	w0, 4
	mov	x1, x24
	bl	dprint_vfmw
	str	w27, [x26, w19, uxtw 2]
.L209:
	ldr	w4, [x26, w19, uxtw 2]
	add	w3, w21, w19, lsl 2
	mov	w2, w19
	mov	x1, x24
	mov	w0, 4
	bl	dprint_vfmw
	b	.L85
.L74:
	sub	w0, w0, #1
	add	w25, w1, 1
	sdiv	w25, w0, w25
	b	.L72
.L67:
	ldr	w0, [x20, 2776]
	and	w0, w0, 16776960
	cmp	w0, 768
	beq	.L207
	ldrb	w0, [x20, 2776]
	cmp	w0, 1
	beq	.L207
	ldr	w1, [x20, 2788]
	mov	w3, 0
	ldr	w0, [x22, 784]
	add	x1, x22, x1, lsl 2
	ldr	w1, [x1, 700]
	add	w0, w1, w0
	and	w0, w0, -16
	b	.L66
.L225:
	ldr	x25, .LC105
	mov	w19, 0
	.p2align 2
.L146:
	add	x22, x20, x19, uxtw 2
	mov	x1, x25
	mov	w0, 4
	add	w19, w19, 1
	ldr	w2, [x22, 728]
	bl	dprint_vfmw
	ldr	w2, [x22, 1496]
	mov	w0, 4
	ldr	x1, .LC107
	bl	dprint_vfmw
	ldr	w2, [x20, 40]
	cmp	w2, w19
	bcs	.L146
	add	x23, x23, 128
	ldrb	w0, [x20]
	ldr	w0, [x23, w0, sxtw 2]
	cmp	w0, 2
	bne	.L135
	ldr	x1, .LC109
	mov	w0, 4
	add	w21, w21, 384
	mov	w22, 0
	bl	dprint_vfmw
	.p2align 2
.L147:
	uxtw	x19, w22
	mov	w3, w21
	add	x23, x19, 32
	add	w2, w22, 96
	add	x19, x20, x19, lsl 2
	ldrb	w0, [x19, 1240]
	ldr	w1, [x19, 472]
	ubfiz	w1, w1, 3, 9
	orr	w1, w1, w0, lsl 12
	ldr	w0, [x20, 80]
	and	w0, w0, 7
	orr	w0, w1, w0
	str	w0, [x28, x23, lsl 2]
	mov	x1, x24
	mov	w0, 4
	ldr	w4, [x28, x23, lsl 2]
	bl	dprint_vfmw
	ldrb	w0, [x19, 1368]
	add	w3, w21, 256
	ldr	w1, [x19, 600]
	add	w2, w22, 160
	ubfiz	w1, w1, 3, 9
	orr	w1, w1, w0, lsl 12
	ldr	w0, [x20, 84]
	and	w0, w0, 7
	orr	w0, w1, w0
	str	w0, [x27, x23, lsl 2]
	mov	x1, x24
	mov	w0, 4
	ldr	w4, [x27, x23, lsl 2]
	bl	dprint_vfmw
	ldrb	w0, [x19, 1496]
	add	w3, w21, 512
	ldr	w1, [x19, 728]
	add	w2, w22, 224
	add	w22, w22, 1
	add	w21, w21, 4
	and	w1, w1, 511
	orr	w0, w1, w0, lsl 9
	str	w0, [x26, x23, lsl 2]
	mov	x1, x24
	mov	w0, 4
	ldr	w4, [x26, x23, lsl 2]
	bl	dprint_vfmw
	ldr	w0, [x20, 40]
	cmp	w0, w22
	bcs	.L147
	b	.L135
.L207:
	ldr	w0, [x20, 2788]
	b	.L208
.L231:
	add	x0, x22, x0, lsl 2
	ldr	w3, [x22, 784]
	ldr	w0, [x0, 700]
	add	w3, w0, w3
	and	w0, w0, -16
	and	w3, w3, -16
	b	.L66
.L121:
	ldrb	w2, [x0, 2777]
	cmp	w2, 3
	beq	.L122
	ldrb	w2, [x0, 2782]
	ldrb	w0, [x0, 2783]
	sbfiz	x3, x2, 1, 32
	cmp	w0, 1
	ldr	w0, [x22, 32]
	add	x0, x3, x0
	add	x3, x23, 176
	ldr	w0, [x3, x0, lsl 2]
	and	w4, w0, 3
	csel	w0, wzr, w9, ne
	cmp	w2, 1
.L213:
	csel	w3, wzr, w8, ne
	and	w2, w25, 7
	orr	w0, w0, w3
	lsl	w3, w2, 2
	orr	w0, w4, w0
	and	w27, w25, 3
	lsl	w0, w0, w3
	orr	w24, w24, w0
	lsl	x0, x1, 5
	add	x0, x0, x1, lsl 2
	add	w1, w27, w27, lsl 2
	add	x0, x20, x0
	ldr	w0, [x0, 2796]
	ubfiz	w0, w0, 1, 4
	orr	w0, w0, 1
	lsl	w0, w0, w1
	orr	w28, w28, w0
	b	.L120
.L222:
	str	w24, [x26, w7, uxtw 2]
	add	w3, w21, w7, lsl 2
	ldr	x1, .LC71
	mov	w2, w7
	ldr	w4, [x26, w7, uxtw 2]
	mov	w0, 4
	add	w7, w7, 1
	str	w7, [x29, 136]
	stp	w8, w9, [x29, 128]
	bl	dprint_vfmw
	mov	w24, 0
	ldp	w8, w9, [x29, 128]
	ldr	w7, [x29, 136]
	b	.L128
.L223:
	str	w28, [x26, w19, uxtw 2]
	add	w3, w21, w19, lsl 2
	ldr	x1, .LC71
	mov	w2, w19
	ldr	w4, [x26, w19, uxtw 2]
	mov	w0, 4
	stp	w8, w9, [x29, 128]
	str	w7, [x29, 136]
	add	w19, w19, 1
	bl	dprint_vfmw
	mov	w28, 0
	ldp	w8, w9, [x29, 128]
	ldr	w7, [x29, 136]
	b	.L129
.L142:
	ldr	x25, .LC97
	mov	w19, 0
	.p2align 2
.L144:
	add	x22, x20, x19, uxtw 2
	mov	x1, x25
	mov	w0, 4
	add	w19, w19, 1
	ldr	w2, [x22, 600]
	bl	dprint_vfmw
	ldr	x1, .LC99
	mov	w0, 4
	ldr	w2, [x22, 1368]
	bl	dprint_vfmw
	ldr	w0, [x20, 40]
	cmp	w0, w19
	bcs	.L144
	b	.L145
.L138:
	ldr	x25, .LC89
	mov	w19, 0
	.p2align 2
.L140:
	add	x22, x20, x19, uxtw 2
	mov	x1, x25
	mov	w0, 4
	add	w19, w19, 1
	ldr	w2, [x22, 472]
	bl	dprint_vfmw
	ldr	x1, .LC91
	mov	w0, 4
	ldr	w2, [x22, 1240]
	bl	dprint_vfmw
	ldr	w0, [x20, 40]
	cmp	w0, w19
	bcs	.L140
	b	.L141
.L224:
	and	w0, w0, 7
	cbz	w0, .L85
	str	w24, [x26, w7, uxtw 2]
	add	w3, w21, w7, lsl 2
	ldr	x24, .LC71
	mov	w2, w7
	ldr	w4, [x26, w7, uxtw 2]
	mov	w0, 4
	mov	x1, x24
	bl	dprint_vfmw
	str	w28, [x26, w19, uxtw 2]
	b	.L209
.L114:
	ldrb	w2, [x0, 2778]
	ldr	w3, [x22, 32]
	add	x2, x3, x2, lsl 1
	ldrb	w3, [x0, 2781]
	cmp	w3, 1
	ldrb	w3, [x0, 2780]
	add	x0, x23, 176
	ldr	w0, [x0, x2, lsl 2]
	and	w4, w0, 3
	csel	w0, wzr, w9, ne
	cmp	w3, 1
	b	.L212
.L122:
	ldrb	w2, [x0, 2778]
	ldr	w3, [x22, 32]
	add	x2, x3, x2, lsl 1
	ldrb	w3, [x0, 2783]
	cmp	w3, 1
	ldrb	w3, [x0, 2782]
	add	x0, x23, 176
	ldr	w0, [x0, x2, lsl 2]
	and	w4, w0, 3
	csel	w0, wzr, w9, ne
	cmp	w3, 1
	b	.L213
.L214:
	ldr	x2, .LC47
	mov	w0, 0
	ldr	x3, .LC46
	ldr	x1, .LC48
	add	x2, x2, 96
	bl	dprint_vfmw
	mov	w0, -1
	b	.L60
	.size	H264HAL_V400R003_SetSliceMsg, .-H264HAL_V400R003_SetSliceMsg
	.align	3
.LC46:
	.xword	.LC6
	.align	3
.LC47:
	.xword	.LANCHOR0
	.align	3
.LC48:
	.xword	.LC9
	.align	3
.LC50:
	.xword	.LC49
	.align	3
.LC52:
	.xword	.LC51
	.align	3
.LC54:
	.xword	.LC53
	.align	3
.LC55:
	.xword	g_not_direct_8x8_inference_flag
	.align	3
.LC57:
	.xword	.LC56
	.align	3
.LC59:
	.xword	.LC58
	.align	3
.LC61:
	.xword	.LC60
	.align	3
.LC63:
	.xword	.LC62
	.align	3
.LC65:
	.xword	.LC64
	.align	3
.LC67:
	.xword	.LC66
	.align	3
.LC69:
	.xword	.LC68
	.align	3
.LC71:
	.xword	.LC70
	.align	3
.LC73:
	.xword	.LC72
	.align	3
.LC75:
	.xword	.LC74
	.align	3
.LC77:
	.xword	.LC76
	.align	3
.LC79:
	.xword	.LC78
	.align	3
.LC81:
	.xword	.LC80
	.align	3
.LC83:
	.xword	.LC82
	.align	3
.LC85:
	.xword	.LC84
	.align	3
.LC87:
	.xword	.LC86
	.align	3
.LC89:
	.xword	.LC88
	.align	3
.LC91:
	.xword	.LC90
	.align	3
.LC93:
	.xword	.LC92
	.align	3
.LC95:
	.xword	.LC94
	.align	3
.LC97:
	.xword	.LC96
	.align	3
.LC99:
	.xword	.LC98
	.align	3
.LC101:
	.xword	.LC100
	.align	3
.LC103:
	.xword	.LC102
	.align	3
.LC105:
	.xword	.LC104
	.align	3
.LC107:
	.xword	.LC106
	.align	3
.LC109:
	.xword	.LC108
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264HAL_V400R003_StartDec
	.type	H264HAL_V400R003_StartDec, %function
H264HAL_V400R003_StartDec:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x21, x0
	mov	x0, x30
	mov	w19, w1
	mov	x22, x2
	bl	_mcount
	ldr	x2, .LC114
	sbfiz	x0, x19, 6, 32
	ldr	x1, .LC115
	cmp	x22, xzr
	add	x0, x2, x0
	ldr	x2, .LC113
	ldrb	w1, [x1]
	ldrsw	x0, [x0, 8]
	ccmp	w1, 1, 4, eq
	ldr	x0, [x2, x0, lsl 3]
	ldr	w24, [x0, 1276]
	bne	.L306
	cbz	x0, .L269
	ldr	w23, [x0, 1296]
.L237:
	cmp	w1, 1
	beq	.L305
	strb	w23, [x22, 1]
.L305:
	cbz	w19, .L240
	cmp	w19, 1
	beq	.L241
	ldr	x1, .LC120
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
.L298:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L240:
	cbz	x21, .L307
	ldr	w0, [x21, 28]
	cmp	w0, 512
	bhi	.L244
	ldr	w0, [x21, 20]
	cmp	w0, 512
	bhi	.L244
	ldr	x0, [x21, 1008]
	cbz	x0, .L308
	ldr	x20, .LC130
	ldr	x0, [x20]
	cbz	x0, .L309
.L246:
	ldr	x3, .LC133
	mov	w0, 3
	ldr	x1, .LC135
	ldr	w2, [x3]
	add	w2, w2, 1
	str	w2, [x3]
	bl	dprint_vfmw
	str	wzr, [x29, 124]
	ldr	w0, [x21, 804]
	mov	w1, 262143
	cmp	w0, 262144
	bgt	.L248
	sub	w1, w0, #1
	and	w1, w1, 1048575
.L248:
	ldr	w0, [x29, 124]
	mov	x3, x22
	mov	w2, 0
	mov	w19, 1
	bfi	w0, w1, 0, 20
	str	w0, [x29, 124]
	ubfx	x1, x0, 24, 6
	ubfx	x0, x0, 16, 6
	orr	w1, w1, 64
	strb	w0, [x29, 126]
	orr	w1, w1, 1
	mov	w0, 8
	and	w1, w1, -3
	strb	w1, [x29, 127]
	ldr	w25, [x29, 124]
	str	w25, [x21, 1024]
	mov	w1, w25
	bl	MFDE_ConfigReg
	ldr	x1, .LC137
	mov	w2, w25
	mov	w0, 3
	bl	dprint_vfmw
	strh	w19, [x29, 126]
	ldr	w2, [x21, 1276]
	mov	w0, 0
	bfi	w0, w24, 3, 1
	mov	w1, 0
	ldrb	w3, [x21, 2]
	bfi	w0, w2, 4, 1
	bfi	w1, w23, 4, 1
	ldr	w2, [x21, 1272]
	and	w1, w1, -33
	and	w0, w0, -33
	bfi	w0, w3, 6, 1
	bfi	w1, w2, 6, 1
	strb	wzr, [x29, 124]
	orr	w0, w0, -128
	and	w1, w1, 127
	strb	w0, [x29, 125]
	strb	w1, [x29, 127]
	mov	x3, x22
	mov	w0, 12
	mov	w2, 0
	ldr	w24, [x29, 124]
	str	w24, [x21, 1028]
	mov	w1, w24
	bl	MFDE_ConfigReg
	ldr	x1, .LC139
	mov	w2, w24
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w24, [x20, 72]
	mov	x3, x22
	mov	w0, 16
	and	w24, w24, -16
	mov	w2, 0
	mov	w1, w24
	str	w24, [x21, 1032]
	bl	MFDE_ConfigReg
	ldr	x1, .LC141
	mov	w2, w24
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w24, [x20, 56]
	mov	x3, x22
	mov	w0, 20
	and	w24, w24, -16
	mov	w2, 0
	mov	w1, w24
	str	w24, [x21, 1036]
	bl	MFDE_ConfigReg
	ldr	x1, .LC143
	mov	w2, w24
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w24, [x21, 800]
	mov	x3, x22
	mov	w0, 24
	and	w24, w24, -16
	mov	w2, 0
	mov	w1, w24
	str	w24, [x21, 1040]
	str	w24, [x29, 124]
	bl	MFDE_ConfigReg
	ldr	x1, .LC145
	mov	w2, w24
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w0, [x21, 28]
	mov	w1, 0
	cmp	w0, 256
	bls	.L310
.L249:
	mov	x3, x22
	mov	w2, 0
	mov	w0, 4
	mov	w19, 3075
	movk	w19, 0x30, lsl 16
	bl	SCD_ConfigReg
	str	w19, [x29, 124]
	mov	w2, 0
	mov	w1, w19
	mov	w0, 60
	mov	x3, x22
	bl	MFDE_ConfigReg
	mov	x3, x22
	mov	w2, 0
	mov	w1, w19
	mov	w0, 64
	bl	MFDE_ConfigReg
	mov	x3, x22
	mov	w2, 0
	mov	w1, w19
	mov	w0, 68
	bl	MFDE_ConfigReg
	mov	x3, x22
	mov	w2, 0
	mov	w1, w19
	mov	w0, 72
	bl	MFDE_ConfigReg
	mov	x3, x22
	mov	w2, 0
	mov	w1, w19
	mov	w0, 76
	bl	MFDE_ConfigReg
	mov	x3, x22
	mov	w2, 0
	mov	w1, w19
	mov	w0, 80
	bl	MFDE_ConfigReg
	mov	x3, x22
	mov	w2, 0
	mov	w1, w19
	mov	w0, 84
	bl	MFDE_ConfigReg
	cmp	w23, 1
	beq	.L311
.L250:
	ldr	w1, [x21, 292]
	mov	x3, x22
	mov	w0, 96
	mov	w2, 0
	add	x1, x21, x1, lsl 2
	ldr	w19, [x1, 308]
	and	w19, w19, -16
	str	w19, [x21, 1076]
	mov	w1, w19
	bl	MFDE_ConfigReg
	ldr	x1, .LC152
	mov	w2, w19
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w19, [x21, 788]
	mov	x3, x22
	mov	w0, 100
	mov	w1, w19
	mov	w2, 0
	str	w19, [x21, 1080]
	bl	MFDE_ConfigReg
	ldr	x1, .LC154
	mov	w2, w19
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w19, [x21, 304]
	mov	x3, x22
	mov	w0, 104
	mov	w1, w19
	mov	w2, 0
	str	w19, [x21, 1084]
	bl	MFDE_ConfigReg
	ldr	x1, .LC156
	mov	w2, w19
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w19, [x21, 796]
	mov	x3, x22
	mov	w0, 108
	mov	w1, w19
	mov	w2, 0
	bl	MFDE_ConfigReg
	ldr	x1, .LC158
	mov	w2, w19
	mov	w0, 3
	mov	w19, 0
	bl	dprint_vfmw
	ldr	w23, [x20, 1180]
	mov	x3, x22
	mov	w0, 128
	and	w23, w23, -16
	mov	w2, 0
	mov	w1, w23
	str	w23, [x21, 1100]
	bl	MFDE_ConfigReg
	ldr	x1, .LC160
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	str	wzr, [x29, 124]
	ldr	w0, [x20, 1184]
	mov	x3, x22
	strh	w0, [x29, 124]
	mov	w2, 0
	mov	w0, 132
	ldr	w23, [x29, 124]
	str	w23, [x21, 1104]
	mov	w1, w23
	bl	MFDE_ConfigReg
	ldr	x1, .LC162
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	add	x2, x21, 1144
	mov	w1, 0
	.p2align 2
.L251:
	ldr	w0, [x2], 4
	and	w0, w0, 3
	lsl	w0, w0, w1
	add	w1, w1, 2
	cmp	w1, 32
	orr	w19, w0, w19
	bne	.L251
	mov	x3, x22
	mov	w1, w19
	mov	w0, 148
	mov	w2, 0
	str	w19, [x29, 124]
	bl	MFDE_ConfigReg
	ldr	x1, .LC164
	mov	w2, w19
	mov	w0, 3
	bl	dprint_vfmw
	ldrb	w1, [x21]
	ldr	x0, .LC116
	ldr	w2, [x21, 32]
	add	x0, x0, 64
	add	x1, x2, x1, lsl 1
	ldr	w0, [x0, x1, lsl 2]
	cbz	w0, .L274
	mov	w1, 0
	mov	w19, 0
.L252:
	mov	x3, x22
	mov	w0, 152
	mov	w2, 0
	mov	w23, -1
	bl	MFDE_ConfigReg
	ldr	x1, .LC166
	mov	w2, w19
	mov	w0, 3
	bl	dprint_vfmw
	str	w23, [x29, 124]
	mov	x3, x22
	mov	w2, 0
	mov	w1, w23
	mov	w0, 32
	bl	MFDE_ConfigReg
	ldp	w2, w3, [x20, 72]
	mov	x1, x20
	mov	x0, x21
	bl	H264HAL_V400R003_SetPicMsg
	ldr	x19, [x21, 1008]
	cbz	x19, .L312
	ldr	w24, [x19, 48]
	cbnz	w24, .L313
	mov	x2, x19
	mov	w22, 0
.L254:
	ldr	w5, [x21, 1004]
	cmp	w5, wzr
	ble	.L256
	add	w23, w22, 4
	add	w26, w22, 5
	mov	w27, 0
	mov	w24, 0
	mov	x25, x20
.L268:
	ldr	w8, [x2, 48]
	cmp	w24, wzr
	ccmp	w8, w27, 2, gt
	bhi	.L314
.L257:
	add	w24, w24, 1
	mov	w0, w5
	cmp	w5, w24
	bgt	.L268
.L267:
	cmp	w0, wzr
	ble	.L256
	add	w0, w0, 4
	add	w22, w0, w22
	add	x20, x20, x22, sxtw 2
	ldr	w0, [x20, 56]
	bl	H264HAL_V400R003_CutSliceChain
	mov	w0, 0
	b	.L298
	.p2align 3
.L274:
	mov	w1, 2
	mov	w19, w1
	b	.L252
	.p2align 3
.L310:
	cmp	w0, 119
	mov	w1, w19
	bls	.L249
	ldr	w0, [x21, 1280]
	cmp	w0, wzr
	cset	w1, eq
	b	.L249
	.p2align 3
.L314:
	add	w0, w24, 1
	cmp	w0, w5
	bge	.L276
	ldr	x19, [x2, 4056]
	cbnz	x19, .L302
	b	.L259
	.p2align 3
.L261:
	add	w0, w0, 1
	cmp	w0, w5
	bge	.L258
	ldr	x19, [x19, 4056]
	cbz	x19, .L259
.L302:
	ldr	w1, [x19, 48]
	cmp	w8, w1
	bcs	.L261
.L258:
	add	w1, w24, w26
	add	w24, w24, w23
	cmp	w1, 140
	add	x24, x20, x24, sxtw 2
	add	x1, x20, x1, sxtw 2
	ccmp	w0, w5, 4, ne
	ldr	w3, [x1, 56]
	ldr	w4, [x24, 56]
	beq	.L315
	add	w1, w0, w26
	ldr	w7, [x19, 48]
	add	x1, x25, x1, sxtw 2
	sub	w7, w7, #1
	ldr	w28, [x1, 56]
.L264:
	sub	w24, w0, #1
	sub	w5, w5, #1
	cmp	w24, w5
	beq	.L316
.L265:
	mov	w6, w8
	mov	w5, w28
	mov	x1, x25
	mov	x0, x21
	mov	w27, w8
	bl	H264HAL_V400R003_SetSliceMsg
	cbnz	w0, .L317
	cbz	w28, .L318
	mov	x2, x19
	ldr	w5, [x21, 1004]
	b	.L257
	.p2align 3
.L259:
	ldr	x1, .LC172
	mov	w2, 1275
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L316:
	str	xzr, [x2, 4056]
	b	.L265
	.p2align 3
.L313:
	ldp	w9, w8, [x19, 8]
	str	wzr, [x19, 48]
	mov	w22, 1
	ldp	w28, w27, [x19, 16]
	stp	w22, wzr, [x19, 8]
	stp	wzr, wzr, [x19, 16]
	ldp	w26, w25, [x19, 24]
	stp	wzr, wzr, [x19, 24]
	sub	w7, w24, #1
	mov	w6, 0
	ldp	w4, w3, [x20, 72]
	ldr	w5, [x20, 80]
	mov	x2, x19
	mov	x1, x20
	mov	x0, x21
	stp	w9, w8, [x29, 104]
	bl	H264HAL_V400R003_SetSliceMsg
	ldp	w9, w8, [x29, 104]
	cbnz	w0, .L319
	stp	w9, w8, [x19, 8]
	stp	w28, w27, [x19, 16]
	stp	w26, w25, [x19, 24]
	str	w24, [x19, 48]
	ldr	x2, [x21, 1008]
	b	.L254
.L315:
	ldr	w7, [x21, 20]
	mov	w28, 0
	ldr	w1, [x21, 28]
	str	xzr, [x2, 4056]
	mul	w7, w1, w7
	ldr	w5, [x21, 1004]
	sub	w7, w7, #1
	b	.L264
.L269:
	mov	w23, 0
	b	.L237
.L309:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L247
	str	x0, [x20]
	b	.L246
.L311:
	ldr	x1, .LC146
	mov	x3, x22
	mov	w0, 92
	mov	w2, 0
	ldr	w19, [x1]
	mov	w1, w19
	bl	MFDE_ConfigReg
	ldr	x1, .LC148
	mov	w2, w19
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w1, [x21, 292]
	mov	w0, 112
	mov	x3, x22
	mov	w2, 0
	add	x1, x21, x1, lsl 2
	ldr	w19, [x1, 436]
	mov	w1, w19
	bl	MFDE_ConfigReg
	ldr	x1, .LC150
	mov	w2, w19
	mov	w0, 3
	bl	dprint_vfmw
	b	.L250
	.p2align 3
.L241:
	ldr	x2, .LC116
	mov	w4, 0
	ldr	x1, .LC122
	mov	w3, 1
	add	x2, x2, 208
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L298
.L276:
	mov	x19, x2
	b	.L258
.L318:
	ldr	w0, [x21, 1004]
	b	.L267
.L317:
	ldr	x1, .LC170
	mov	w2, 1301
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L298
.L244:
	ldr	x2, .LC116
	ldr	x3, .LC127
.L303:
	ldr	x1, .LC125
	mov	w0, 0
	add	x2, x2, 208
	bl	dprint_vfmw
	mov	w0, -1
	b	.L298
.L256:
	ldr	x1, .LC174
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L298
.L319:
	ldr	x1, .LC170
	mov	w2, 1231
	mov	w0, w22
	bl	dprint_vfmw
	mov	w0, w23
	b	.L298
.L308:
	ldr	x2, .LC116
	ldr	x3, .LC129
	ldr	x1, .LC125
	add	x2, x2, 208
	bl	dprint_vfmw
	mov	w0, -1
	b	.L298
.L307:
	ldr	x2, .LC116
	ldr	x3, .LC124
	b	.L303
.L306:
	ldr	x2, .LC116
	mov	x3, x22
	ldr	x1, .LC118
	mov	w0, 0
	add	x2, x2, 208
	bl	dprint_vfmw
	mov	w0, -1
	b	.L298
.L312:
	ldr	x1, .LC168
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, w23
	b	.L298
.L247:
	ldr	x1, .LC132
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L298
	.size	H264HAL_V400R003_StartDec, .-H264HAL_V400R003_StartDec
	.align	3
.LC113:
	.xword	s_pstVfmwChan
	.align	3
.LC114:
	.xword	g_VdmDrvParam
	.align	3
.LC115:
	.xword	g_HalDisable
	.align	3
.LC116:
	.xword	.LANCHOR0
	.align	3
.LC118:
	.xword	.LC117
	.align	3
.LC120:
	.xword	.LC119
	.align	3
.LC122:
	.xword	.LC121
	.align	3
.LC124:
	.xword	.LC123
	.align	3
.LC125:
	.xword	.LC9
	.align	3
.LC127:
	.xword	.LC126
	.align	3
.LC129:
	.xword	.LC128
	.align	3
.LC130:
	.xword	g_HwMem
	.align	3
.LC132:
	.xword	.LC131
	.align	3
.LC133:
	.xword	.LANCHOR1
	.align	3
.LC135:
	.xword	.LC134
	.align	3
.LC137:
	.xword	.LC136
	.align	3
.LC139:
	.xword	.LC138
	.align	3
.LC141:
	.xword	.LC140
	.align	3
.LC143:
	.xword	.LC142
	.align	3
.LC145:
	.xword	.LC144
	.align	3
.LC146:
	.xword	g_TunnelLineNumber
	.align	3
.LC148:
	.xword	.LC147
	.align	3
.LC150:
	.xword	.LC149
	.align	3
.LC152:
	.xword	.LC151
	.align	3
.LC154:
	.xword	.LC153
	.align	3
.LC156:
	.xword	.LC155
	.align	3
.LC158:
	.xword	.LC157
	.align	3
.LC160:
	.xword	.LC159
	.align	3
.LC162:
	.xword	.LC161
	.align	3
.LC164:
	.xword	.LC163
	.align	3
.LC166:
	.xword	.LC165
	.align	3
.LC168:
	.xword	.LC167
	.align	3
.LC170:
	.xword	.LC169
	.align	3
.LC172:
	.xword	.LC171
	.align	3
.LC174:
	.xword	.LC173
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.11911, %object
	.size	__func__.11911, 31
__func__.11911:
	.string	"H264HAL_V400R003_CutSliceChain"
	.zero	1
	.type	__func__.11934, %object
	.size	__func__.11934, 27
__func__.11934:
	.string	"H264HAL_V400R003_SetPicMsg"
	.zero	5
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
	.type	__func__.11977, %object
	.size	__func__.11977, 29
__func__.11977:
	.string	"H264HAL_V400R003_SetSliceMsg"
	.zero	3
	.type	g_SliceTypeForPMV, %object
	.size	g_SliceTypeForPMV, 12
g_SliceTypeForPMV:
	.word	1
	.word	2
	.word	0
	.zero	4
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
	.type	__func__.12049, %object
	.size	__func__.12049, 26
__func__.12049:
	.string	"H264HAL_V400R003_StartDec"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC0:
	ASCII(.string	"line: %d, p = NULL is not expected value!\n" )
	.zero	5
.LC4:
	ASCII(.string	"H264HAL_V400R003_InitHal return error.\n" )
.LC6:
	ASCII(.string	"can not map slice msg virtual address!\n" )
.LC9:
	ASCII(.string	"%s: %s\n" )
.LC13:
	ASCII(.string	"qmatrix[%d] = 0x%x\n" )
	.zero	4
.LC15:
	ASCII(.string	"picmsg Dxx addr 0x%x = 0x%x\n" )
	.zero	3
.LC17:
	ASCII(.string	"can not map down msg virtual address!\n" )
	.zero	1
.LC21:
	ASCII(.string	"picmsg D[0] addr 0x%x = 0x%x\n" )
	.zero	2
.LC23:
	ASCII(.string	"picmsg D[1] addr 0x%x = 0x%x\n" )
	.zero	2
.LC25:
	ASCII(.string	"picmsg D[2] addr 0x%x = 0x%x\n" )
	.zero	2
.LC27:
	ASCII(.string	"picmsg D[3] addr 0x%x = 0x%x\n" )
	.zero	2
.LC29:
	ASCII(.string	"picmsg D[4] addr 0x%x = 0x%x\n" )
	.zero	2
.LC31:
	ASCII(.string	"picmsg D[5] addr 0x%x = 0x%x\n" )
	.zero	2
.LC33:
	ASCII(.string	"picmsg D[6] addr 0x%x = 0x%x\n" )
	.zero	2
.LC35:
	ASCII(.string	"picmsg D[7] addr 0x%x = 0x%x\n" )
	.zero	2
.LC37:
	ASCII(.string	"picmsg D[8] addr 0x%x = 0x%x\n" )
	.zero	2
.LC39:
	ASCII(.string	"picmsg D[9] addr 0x%x = 0x%x\n" )
	.zero	2
.LC41:
	ASCII(.string	"picmsg D[%d] addr 0x%x = 0x%x\n" )
	.zero	1
.LC43:
	ASCII(.string	"picmsg D[26] addr 0x%x = 0x%x\n" )
	.zero	1
.LC49:
	ASCII(.string	"Before slicemsg D[%d] tmp =0x%x,bitsoffset =0x%x,bytespos = 0x%x\n" )
	.zero	6
.LC51:
	ASCII(.string	"slicemsg D[%d] addr 0x%x = 0x%x\n" )
	.zero	7
.LC53:
	ASCII(.string	"slicemsg D[4] addr 0x%x = 0x%x\n" )
.LC56:
	ASCII(.string	"slicemsg D[5] addr 0x%x = 0x%x\n" )
.LC58:
	ASCII(.string	"slicemsg g_pListX[1][0]->frame_store->pmv_address_idc = %d\n" )
	.zero	4
.LC60:
	ASCII(.string	"slicemsg D[6] addr 0x%x = 0x%x\n" )
.LC62:
	ASCII(.string	"slicemsg D[7] addr 0x%x = 0x%x\n" )
.LC64:
	ASCII(.string	"slicemsg D[8] addr 0x%x = 0x%x\n" )
.LC66:
	ASCII(.string	"slicemsg D[9] addr 0x%x = 0x%x\n" )
.LC68:
	ASCII(.string	"slicemsg D10~D13  D18~D25 list0size = %d\n" )
	.zero	6
.LC70:
	ASCII(.string	"slicemsg D[%d] address 0x%x = 0x%x\n" )
	.zero	4
.LC72:
	ASCII(.string	"slicemsg D14~D17  D26~D33 list1size = %d\n" )
	.zero	6
.LC74:
	ASCII(.string	"slicemsg D10~D13, D18~D25 list0size = %d\n" )
	.zero	6
.LC76:
	ASCII(.string	"slicemsg D14~D17 D26 ~D33 list1size = %d\n" )
	.zero	6
.LC78:
	ASCII(.string	"slicemsg burst1 D0~D31, burst2 D0~31, burst3 D0~D31 list0size = %d\n" )
	.zero	4
.LC80:
	ASCII(.string	"luma_lg = %d\n" )
	.zero	2
.LC82:
	ASCII(.string	"wp_weight_l0_y = %d\n" )
	.zero	3
.LC84:
	ASCII(.string	"wp_offset_l0_y = %d\n" )
	.zero	3
.LC86:
	ASCII(.string	"chroma_lg = %d\n" )
.LC88:
	ASCII(.string	"wp_weight_l1_y = %d\n" )
	.zero	3
.LC90:
	ASCII(.string	"wp_offset_l1_y = %d\n" )
	.zero	3
.LC92:
	ASCII(.string	"wp_weight_l0_u = %d\n" )
	.zero	3
.LC94:
	ASCII(.string	"wp_offset_l0_u = %d\n" )
	.zero	3
.LC96:
	ASCII(.string	"wp_weight_l1_u = %d\n" )
	.zero	3
.LC98:
	ASCII(.string	"wp_offset_l1_u = %d\n" )
	.zero	3
.LC100:
	ASCII(.string	"wp_weight_l0_v = %d\n" )
	.zero	3
.LC102:
	ASCII(.string	"wp_offset_l0_v = %d\n" )
	.zero	3
.LC104:
	ASCII(.string	"wp_weight_l1_v = %d\n" )
	.zero	3
.LC106:
	ASCII(.string	"wp_offset_l1_v = %d\n" )
	.zero	3
.LC108:
	ASCII(.string	"slicemsg burst1 D32~D63, burst2 D32~63, burst3 D32~D63 list1size = %d\n" )
	.zero	1
.LC117:
	ASCII(.string	"%s: pMfdeTask(%p) = NULL\n" )
	.zero	6
.LC119:
	ASCII(.string	"VdhId is wrong! H264HAL_V200R003_StartDec\n" )
	.zero	5
.LC121:
	ASCII(.string	"%s: VdhId(%d) > %d\n" )
	.zero	4
.LC123:
	ASCII(.string	"point of picture para null\n" )
	.zero	4
.LC126:
	ASCII(.string	"picture width out of range\n" )
	.zero	4
.LC128:
	ASCII(.string	"point of fst slice para null\n" )
	.zero	2
.LC131:
	ASCII(.string	"vdm register virtual address not mapped, reset failed!\n" )
.LC134:
	ASCII(.string	"\n\n***********************H264HAL_V200R003_StartDec is Now %d***********************\n" )
	.zero	3
.LC136:
	ASCII(.string	"BASIC_V400R003_CFG0 = 0x%x\n" )
	.zero	4
.LC138:
	ASCII(.string	"BASIC_V400R003_CFG1 = 0x%x\n" )
	.zero	4
.LC140:
	ASCII(.string	"AVM_V400R003_ADDR = 0x%x\n" )
	.zero	6
.LC142:
	ASCII(.string	"VAM_V400R003_ADDR = 0x%x\n" )
	.zero	6
.LC144:
	ASCII(.string	"STREAM_V400R003_BASE_ADDR = 0x%x\n" )
	.zero	6
.LC147:
	ASCII(.string	"VREG_PART_DEC_OVER_INT_LEVEL=0x%x\n" )
	.zero	5
.LC149:
	ASCII(.string	"VREG_LINE_NUM_STADDR = 0x%x\n" )
	.zero	3
.LC151:
	ASCII(.string	"YSTADDR_V400R003_1D = 0x%x\n" )
	.zero	4
.LC153:
	ASCII(.string	"YSTRIDE_V400R003_1D = 0x%x\n" )
	.zero	4
.LC155:
	ASCII(.string	"UVOFFSET_V400R003_1D = 0x%x\n" )
	.zero	3
.LC157:
	ASCII(.string	"HEAD_INF_OFFSET_V400R003_1D = 0x%x\n" )
	.zero	4
.LC159:
	ASCII(.string	"PPFD_V400R003_BUF_ADDR = 0x%x\n" )
	.zero	1
.LC161:
	ASCII(.string	"PPFD_V400R003_BUF_LEN = 0x%x\n" )
	.zero	2
.LC163:
	ASCII(.string	"REF_V400R003_PIC_TYPE = 0x%x\n" )
	.zero	2
.LC165:
	ASCII(.string	"FF_V400R003_APT_EN = 0x%x\n" )
	.zero	5
.LC167:
	ASCII(.string	"pSlicePara = NULL!\n" )
	.zero	4
.LC169:
	ASCII(.string	"line %d Set slice msg not wrong!\n" )
	.zero	6
.LC171:
	ASCII(.string	"line %d tmp_pSlicePara is NULL!\n" )
	.zero	7
.LC173:
	ASCII(.string	"!!! no slice to dec, add up msg report." )
	.bss
	.align	2
.LANCHOR1 = . + 0
	.type	StartCnt.12043, %object
	.size	StartCnt.12043, 4
StartCnt.12043:
	.zero	4
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
