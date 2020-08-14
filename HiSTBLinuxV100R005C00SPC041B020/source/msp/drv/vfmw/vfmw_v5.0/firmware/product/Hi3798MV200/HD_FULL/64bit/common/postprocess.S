#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"postprocess.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	PostPro_GetRepairStratage
	.type	PostPro_GetRepairStratage, %function
PostPro_GetRepairStratage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cbz	w19, .L4
	cmp	w19, 15
	beq	.L4
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L4:
	ldrb	w0, [x20, 3]
	ldp	x19, x20, [sp, 16]
	cmp	w0, wzr
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	PostPro_GetRepairStratage, .-PostPro_GetRepairStratage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_CopyYuv
	.type	PostPro_CopyYuv, %function
PostPro_CopyYuv:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w21, w1
	mov	x20, x2
	bl	_mcount
	sxtw	x5, w21
	ldr	x1, .LC0
	lsl	x6, x5, 3
	ldr	w2, [x19, 14604]
	sub	x6, x6, x5
	ldr	w7, [x19, 14616]
	lsl	x6, x6, 3
	ldr	w4, [x19, 14684]
	sub	x6, x6, x5
	mov	w0, 1
	lsl	x6, x6, 2
	mov	w3, 4
	sub	x5, x6, x5
	ldr	x6, .LC1
	add	x5, x1, x5, lsl 3
	ldr	w1, [x19, 14692]
	str	w7, [x5, 12]
	stp	w2, w2, [x5, 4]
	ldr	x7, [x6, 280]
	ldr	w2, [x19, 232]
	str	w2, [x5, 16]
	ldr	w2, [x19, 240]
	str	w2, [x5, 20]
	ldrh	w6, [x19, 176]
	str	w6, [x5, 24]
	ldrh	w2, [x19, 178]
	stp	w2, w0, [x5, 28]
	strh	wzr, [x5, 40]
	mul	w6, w6, w2
	str	wzr, [x5, 36]
	sub	w6, w6, #1
	str	w0, [x5, 840]
	strh	w6, [x5, 42]
	str	w4, [x5, 848]
	str	wzr, [x5, 852]
	str	w0, [x5, 856]
	str	w3, [x5, 864]
	str	w1, [x5, 872]
	cbz	x7, .L10
	mov	x4, x20
	mov	w3, w21
	mov	w2, 0
	mov	x1, x19
	mov	w0, 2
	blr	x7
	cmp	w0, wzr
	cset	w0, ne
.L10:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	PostPro_CopyYuv, .-PostPro_CopyYuv
	.align	3
.LC0:
	.xword	g_RepairParam
	.align	3
.LC1:
	.xword	g_vdm_hal_fun_ptr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_CopyYuv_Vc1
	.type	PostPro_CopyYuv_Vc1, %function
PostPro_CopyYuv_Vc1:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	w21, w1
	mov	x22, x2
	bl	_mcount
	mov	w24, 16
	ldrh	w1, [x20, 78]
	sub	w0, w1, #1
	uxth	w0, w0
	cmp	w0, 2047
	bls	.L13
	sub	w0, w1, #2049
	mov	w24, 32
	uxth	w0, w0
	cmp	w0, 2047
	bhi	.L20
.L13:
	sxtw	x19, w21
	ldr	x5, .LC2
	lsl	x0, x19, 3
	ldr	w1, [x20, 14564]
	sub	x0, x0, x19
	ldr	w4, [x20, 14436]
	lsl	x0, x0, 3
	ldr	w2, [x20, 14604]
	sub	x0, x0, x19
	ldr	w3, [x20, 14608]
	lsl	x0, x0, 2
	mov	w23, 1
	sub	x0, x0, x19
	add	x19, x5, x0, lsl 3
	ldr	w0, [x20, 14448]
	stp	w1, w4, [x19, 4]
	ldr	w1, [x20, 14440]
	str	w1, [x19, 16]
	ldr	x1, .LC4
	stp	w0, w2, [x19, 20]
	stp	w3, w23, [x19, 28]
	str	wzr, [x19, 36]
	mov	w0, 22
	bl	dprint_vfmw
	strh	wzr, [x19, 40]
	ldp	w0, w3, [x19, 24]
	ldr	x1, .LC5
	ldr	w2, [x20, 14596]
	mul	w0, w0, w3
	ldr	x5, [x1, 280]
	add	x1, x19, 768
	sub	w0, w0, #1
	strh	w0, [x19, 42]
	mov	w0, 4
	stp	w23, wzr, [x1, 72]
	stp	w2, wzr, [x1, 80]
	str	w23, [x19, 856]
	str	w24, [x19, 860]
	str	w0, [x19, 864]
	ldrb	w0, [x20, 75]
	str	w0, [x19, 872]
	cbz	x5, .L19
	mov	x4, x22
	mov	w3, w21
	mov	x1, x20
	mov	w2, 0
	mov	w0, w23
	blr	x5
	cmp	w0, wzr
	cset	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L20:
	sub	w1, w1, #4096
	mov	w24, 48
	sub	w0, w1, #1
	uxth	w0, w0
	cmp	w0, 2047
	bls	.L13
	sub	w1, w1, #2049
	mov	w24, 16
	mov	w0, 64
	uxth	w1, w1
	cmp	w1, 2047
	csel	w24, w24, w0, hi
	b	.L13
	.p2align 3
.L19:
	mov	w0, w23
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	PostPro_CopyYuv_Vc1, .-PostPro_CopyYuv_Vc1
	.align	3
.LC2:
	.xword	g_RepairParam
	.align	3
.LC4:
	.xword	.LC3
	.align	3
.LC5:
	.xword	g_vdm_hal_fun_ptr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_CopyYuv_Vp6
	.type	PostPro_CopyYuv_Vp6, %function
PostPro_CopyYuv_Vp6:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	w22, w1
	mov	x23, x2
	bl	_mcount
	ldr	x19, .LC6
	sxtw	x2, w22
	ldr	w3, [x20, 228]
	lsl	x1, x2, 3
	mov	w21, 1
	sub	x1, x1, x2
	mov	w0, 22
	lsl	x1, x1, 3
	sub	x1, x1, x2
	lsl	x1, x1, 2
	sub	x2, x1, x2
	ldr	x1, .LC7
	add	x19, x19, x2, lsl 3
	str	w3, [x19, 4]
	ldr	w2, [x20, 228]
	str	w2, [x19, 8]
	ldr	w2, [x20, 236]
	str	w2, [x19, 16]
	ldr	w2, [x20, 240]
	str	w2, [x19, 20]
	ldrh	w2, [x20, 106]
	str	w2, [x19, 24]
	ldrh	w3, [x20, 108]
	stp	w3, w21, [x19, 28]
	str	wzr, [x19, 36]
	bl	dprint_vfmw
	strh	wzr, [x19, 40]
	ldp	w0, w3, [x19, 24]
	str	w21, [x19, 840]
	mov	w1, 4
	str	wzr, [x19, 844]
	mul	w0, w0, w3
	ldr	x2, .LC8
	sub	w0, w0, #1
	strh	w0, [x19, 42]
	ldr	w0, [x20, 260]
	str	w0, [x19, 848]
	str	wzr, [x19, 852]
	str	w21, [x19, 856]
	str	w1, [x19, 864]
	ldr	x5, [x2, 280]
	ldrb	w0, [x20, 104]
	str	w0, [x19, 872]
	cbz	x5, .L23
	mov	x4, x23
	mov	w3, w22
	mov	x1, x20
	mov	w2, 0
	mov	w0, 10
	blr	x5
	cmp	w0, wzr
	cset	w0, ne
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L23:
	mov	w0, w21
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.size	PostPro_CopyYuv_Vp6, .-PostPro_CopyYuv_Vp6
	.align	3
.LC6:
	.xword	g_RepairParam
	.align	3
.LC7:
	.xword	.LC3
	.align	3
.LC8:
	.xword	g_vdm_hal_fun_ptr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_CopyYuv_Vp9
	.type	PostPro_CopyYuv_Vp9, %function
PostPro_CopyYuv_Vp9:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x24, x0
	mov	x0, x30
	sxtw	x19, w1
	mov	x23, x2
	bl	_mcount
	add	x20, x24, 266240
	lsl	x0, x19, 3
	ldr	x5, .LC9
	sub	x0, x0, x19
	mov	x22, x19
	lsl	x0, x0, 3
	ldr	w1, [x20, 1920]
	sub	x0, x0, x19
	ldr	w2, [x20, 1576]
	lsl	x0, x0, 2
	ldr	w3, [x20, 1580]
	sub	x0, x0, x19
	ldr	w4, [x20, 1928]
	mov	w21, 1
	add	x19, x5, x0, lsl 3
	ldr	w0, [x20, 1936]
	stp	w1, w1, [x19, 4]
	ldr	x1, .LC10
	stp	w4, w0, [x19, 16]
	stp	w2, w3, [x19, 24]
	stp	w21, wzr, [x19, 32]
	mov	w0, 22
	bl	dprint_vfmw
	strh	wzr, [x19, 40]
	ldp	w0, w4, [x19, 24]
	ldr	x2, .LC11
	ldr	w3, [x20, 1948]
	mul	w0, w0, w4
	ldrb	w1, [x20, 1569]
	sub	w0, w0, #1
	strh	w0, [x19, 42]
	add	x0, x19, 768
	ldr	x5, [x2, 280]
	mov	w2, 6
	stp	w21, wzr, [x0, 72]
	stp	w3, wzr, [x0, 80]
	str	w21, [x19, 856]
	str	w2, [x19, 864]
	str	w1, [x19, 872]
	cbz	x5, .L26
	mov	x4, x23
	mov	w3, w22
	mov	x1, x24
	mov	w2, 0
	mov	w0, 17
	blr	x5
	cmp	w0, wzr
	cset	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L26:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	PostPro_CopyYuv_Vp9, .-PostPro_CopyYuv_Vp9
	.align	3
.LC9:
	.xword	g_RepairParam
	.align	3
.LC10:
	.xword	.LC3
	.align	3
.LC11:
	.xword	g_vdm_hal_fun_ptr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetHevcMbGroupOfTile
	.type	PostPro_GetHevcMbGroupOfTile, %function
PostPro_GetHevcMbGroupOfTile:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w3
	mov	x22, x1
	mov	x21, x2
	bl	_mcount
	ldr	x2, .LC13
	sbfiz	x0, x19, 6, 32
	ldr	x1, .LC12
	add	x0, x2, x0
	str	wzr, [x20, 840]
	ldp	w19, w2, [x20, 24]
	str	wzr, [x20, 856]
	ldrsw	x0, [x0, 8]
	mul	w19, w19, w2
	ldr	x0, [x1, x0, lsl 3]
	add	x0, x0, 327680
	add	x0, x0, 2760
	bl	HEVC_CalcTileMap
	ldr	w1, [x22, 8]
	mov	w7, 0
	cbz	w1, .L34
	ldr	x9, .LC14
	mov	w2, 1
	ldr	x8, .LC15
	.p2align 2
.L57:
	add	x5, x22, x7, uxtw 2
	ldrh	w4, [x5, 12]
	ldrh	w3, [x5, 14]
	mov	w0, w4
	cmp	w4, w3
	bhi	.L31
	mov	x6, x5
	b	.L32
	.p2align 3
.L64:
	ldrh	w4, [x5, 12]
.L32:
	cmp	w19, w4
	blt	.L35
	cmp	w19, w3
	blt	.L35
	ldr	w3, [x8, w0, uxtw 2]
	strb	w2, [x9, x3]
.L35:
	ldrh	w3, [x6, 14]
	add	w0, w0, 1
	cmp	w3, w0
	bcs	.L64
	ldr	w1, [x22, 8]
.L31:
	add	w7, w7, 1
	cmp	w1, w7
	bhi	.L57
.L34:
	ldr	w2, [x21, 2564]
	mov	w9, 0
	cbz	w2, .L30
	ldr	x5, .LC15
	ldr	x4, .LC14
	.p2align 2
.L56:
	uxtw	x8, w9
	add	x1, x21, x8, lsl 3
	ldr	w6, [x1, 3688]
	ldr	w1, [x1, 3684]
	cmp	w1, w6
	bhi	.L41
	ldr	w7, [x5, w1, uxtw 2]
	ldrb	w0, [x4, x7]
	cbz	w0, .L38
	mov	w0, w1
	b	.L40
	.p2align 3
.L43:
	ldr	w3, [x5, w0, uxtw 2]
	ldrb	w3, [x4, x3]
	cbz	w3, .L38
.L40:
	add	w0, w0, 1
	cmp	w0, w6
	bls	.L43
.L41:
	add	w9, w9, 1
	cmp	w2, w9
	bhi	.L56
.L30:
	ldr	x3, .LC14
	mov	w4, 0
	.p2align 2
.L48:
	cmp	w4, w19
	bcs	.L49
	ldrb	w0, [x3, w4, uxtw]
	mov	w1, w4
	cmp	w0, 1
	bne	.L62
	b	.L66
	.p2align 3
.L67:
	ldrb	w2, [x3, w0, uxtw]
	cbnz	w2, .L46
	mov	w1, w0
.L62:
	add	w0, w1, 1
	cmp	w19, w0
	bhi	.L67
.L46:
	ldr	w2, [x20, 840]
	add	w5, w2, 1
	add	x2, x20, x2, sxtw 2
	strh	w4, [x2, 40]
	mov	w4, w0
	strh	w1, [x2, 42]
	str	w5, [x20, 840]
.L45:
	cmp	w5, 199
	ble	.L48
	ldr	x1, .LC17
	mov	w0, 1
	sub	w19, w19, #1
	bl	dprint_vfmw
	strh	wzr, [x20, 40]
	mov	w0, 1
	strh	w19, [x20, 42]
	str	w0, [x20, 840]
.L49:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L38:
	add	x3, x21, x8, lsl 3
	b	.L42
	.p2align 3
.L65:
	ldr	w7, [x5, w1, uxtw 2]
.L42:
	strb	wzr, [x4, x7]
	add	w1, w1, 1
	ldr	w0, [x3, 3688]
	cmp	w0, w1
	bcs	.L65
	ldr	w2, [x21, 2564]
	add	w9, w9, 1
	cmp	w2, w9
	bhi	.L56
	b	.L30
	.p2align 3
.L66:
	add	w4, w4, 1
	ldr	w5, [x20, 840]
	b	.L45
	.size	PostPro_GetHevcMbGroupOfTile, .-PostPro_GetHevcMbGroupOfTile
	.align	3
.LC12:
	.xword	s_pstVfmwChan
	.align	3
.LC13:
	.xword	g_VdmDrvParam
	.align	3
.LC14:
	.xword	g_IsDeced
	.align	3
.LC15:
	.xword	g_TsToRsMap
	.align	3
.LC17:
	.xword	.LC16
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetHevcMbGroupOfNoTile
	.type	PostPro_GetHevcMbGroupOfNoTile, %function
PostPro_GetHevcMbGroupOfNoTile:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x1
	mov	w19, w3
	bl	_mcount
	sbfiz	x19, x19, 6, 32
	ldr	x0, .LC19
	ldr	x1, .LC18
	add	x0, x0, x19
	str	wzr, [x21, 840]
	ldp	w19, w2, [x21, 24]
	str	wzr, [x21, 856]
	ldrsw	x0, [x0, 8]
	mul	w19, w19, w2
	ldr	x0, [x1, x0, lsl 3]
	add	x0, x0, 327680
	add	x0, x0, 2760
	bl	HEVC_CalcTileMap
	ldr	w2, [x20, 8]
	mov	w1, -1
	uxth	w9, w19
	mov	w6, w1
	mov	w0, 0
	add	w10, w19, w1
	add	x2, x20, x2, lsl 2
	strh	w9, [x2, 14]
	strh	w9, [x2, 12]
	b	.L77
	.p2align 3
.L85:
	cmp	w2, w1
	csel	w1, w1, w6, ge
	add	w3, w1, 1
	cmp	w2, w3
	ble	.L71
	ldr	w3, [x21, 840]
	cmp	w3, 199
	bgt	.L82
	add	x8, x21, x3, sxtw 2
	add	w6, w6, w6, lsr 31
	sub	w1, w1, #100
	sub	w7, w2, #1
	sxth	w6, w6
	strh	w7, [x8, 42]
	cmp	w1, w6
	strh	w6, [x8, 40]
	ble	.L74
	strh	w1, [x8, 40]
.L74:
	add	w3, w3, 1
	str	w3, [x21, 840]
.L71:
	ldr	w3, [x20, 8]
	sub	w1, w3, #1
	cmp	w1, w0
	beq	.L83
.L80:
	add	w0, w0, 1
	mov	w1, w4
	cmp	w0, w3
	mov	w6, w2
	bhi	.L84
.L77:
	sxtw	x5, w0
	add	x3, x20, x5, lsl 2
	ldrh	w2, [x3, 12]
	ldrh	w4, [x3, 14]
	cmp	w19, w2
	ccmp	w19, w4, 1, ge
	ccmp	w2, w4, 0, ge
	ble	.L85
	ldr	w3, [x20, 8]
	add	w0, w0, 1
	cmp	w0, w3
	bls	.L77
.L84:
	mov	w0, 0
.L73:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L83:
	add	x5, x20, x5, lsl 2
	ldrh	w1, [x5, 14]
	cmp	w1, w10
	bge	.L80
	ldr	w3, [x21, 840]
	cmp	w3, 199
	bgt	.L86
	add	x2, x21, x3, sxtw 2
	sub	w9, w9, #1
	add	w3, w3, 1
	mov	w0, 0
	strh	w1, [x2, 40]
	strh	w9, [x2, 42]
	str	w3, [x21, 840]
	b	.L73
.L82:
	ldr	x2, .LC20
	mov	w0, 1
	ldr	x1, .LC22
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L86:
	ldr	x2, .LC20
	mov	w0, 1
	ldr	x1, .LC24
	bl	dprint_vfmw
	mov	w0, 1
	b	.L73
	.size	PostPro_GetHevcMbGroupOfNoTile, .-PostPro_GetHevcMbGroupOfNoTile
	.align	3
.LC18:
	.xword	s_pstVfmwChan
	.align	3
.LC19:
	.xword	g_VdmDrvParam
	.align	3
.LC20:
	.xword	.LANCHOR0
	.align	3
.LC22:
	.xword	.LC21
	.align	3
.LC24:
	.xword	.LC23
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetHevcMbGroup
	.type	PostPro_GetHevcMbGroup, %function
PostPro_GetHevcMbGroup:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	mov	x21, x2
	mov	w22, w3
	bl	_mcount
	ldr	w2, [x19, 8]
	sub	w1, w2, #1
	cmp	w1, 199
	bhi	.L91
	ldr	w0, [x20, 868]
	mov	w3, w22
	mov	x2, x21
	mov	x1, x19
	cbnz	w0, .L92
	mov	x0, x20
	bl	PostPro_GetHevcMbGroupOfNoTile
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L92:
	mov	x0, x20
	bl	PostPro_GetHevcMbGroupOfTile
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L91:
	ldr	x1, .LC26
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	PostPro_GetHevcMbGroup, .-PostPro_GetHevcMbGroup
	.align	3
.LC26:
	.xword	.LC25
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetHEVCRepairParam
	.type	PostPro_GetHEVCRepairParam, %function
PostPro_GetHEVCRepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	mov	x22, x2
	mov	w21, w3
	bl	_mcount
	ldr	w0, [x19, 1280]
	mov	w4, 1
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 1336]
	str	w0, [x20, 4]
	ldr	w0, [x19, 2792]
	str	w0, [x20, 8]
	ldr	x0, [x19, 2568]
	ldr	w0, [x0, 372]
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 1336]
	str	w0, [x20, 16]
	ldr	w0, [x19, 1284]
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 1468]
	str	w0, [x20, 20]
	ldr	w3, [x19, 68]
	str	w3, [x20, 864]
	lsl	w2, w4, w3
	ldr	w0, [x19, 1316]
	str	w0, [x20, 848]
	ldr	w0, [x19, 188]
	sub	w0, w0, #1
	add	w0, w0, w2
	lsr	w0, w0, w3
	str	w0, [x20, 24]
	ldr	w0, [x19, 184]
	sub	w0, w0, #1
	add	w2, w0, w2
	lsr	w2, w2, w3
	str	w2, [x20, 28]
	ldrb	w0, [x19, 217]
	str	w0, [x20, 868]
	str	w4, [x20, 32]
	ldr	w0, [x19, 8]
	cmp	w0, wzr
	cset	w0, eq
	str	w0, [x20, 36]
	ldr	w0, [x19, 1332]
	cbnz	w0, .L106
	str	wzr, [x22, 8]
.L95:
	ldr	w2, [x20, 4]
	ldr	w0, [x20, 16]
	cmp	w2, w0
	beq	.L107
.L96:
	str	wzr, [x20, 852]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L107:
	ldr	w3, [x19, 1464]
	cbz	w3, .L96
	ldr	w1, [x19, 1336]
	mov	w0, 0
	cmp	w2, w1
	bne	.L97
	.p2align 2
.L99:
	add	w0, w0, 1
	cmp	w3, w0
	add	x1, x19, x0, sxtw 2
	beq	.L96
	ldr	w1, [x1, 1336]
	cmp	w2, w1
	beq	.L99
.L97:
	str	w1, [x20, 16]
	str	wzr, [x20, 852]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L106:
	mov	w3, w21
	mov	x2, x19
	mov	x1, x22
	mov	x0, x20
	bl	PostPro_GetHevcMbGroup
	b	.L95
	.size	PostPro_GetHEVCRepairParam, .-PostPro_GetHEVCRepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetVp9MbGroup
	.type	PostPro_GetVp9MbGroup, %function
PostPro_GetVp9MbGroup:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 8]
	sub	w0, w0, #1
	cmp	w0, 199
	bhi	.L122
	str	wzr, [x20, 840]
	mov	w1, -1
	str	wzr, [x20, 856]
	mov	w0, 0
	ldp	w2, w5, [x20, 24]
	ldr	w3, [x19, 8]
	mov	w7, w1
	mul	w5, w2, w5
	add	x2, x19, x3, lsl 2
	add	w10, w5, w1
	uxth	w9, w5
	strh	w9, [x2, 14]
	strh	w9, [x2, 12]
	b	.L117
	.p2align 3
.L126:
	cmp	w2, w1
	csel	w1, w1, w7, ge
	add	w1, w1, 1
	cmp	w2, w1
	beq	.L113
	add	w7, w7, w7, lsr 31
	sub	w6, w2, #1
	cmp	w7, w6
	bge	.L113
	ldr	w1, [x20, 840]
	cmp	w1, 199
	bgt	.L123
	add	x8, x20, x1, sxtw 2
	add	w1, w1, 1
	strh	w7, [x8, 40]
	strh	w6, [x8, 42]
	str	w1, [x20, 840]
.L113:
	ldr	w6, [x19, 8]
	sub	w1, w6, #1
	cmp	w1, w0
	beq	.L124
.L120:
	add	w0, w0, 1
	mov	w1, w3
	cmp	w0, w6
	mov	w7, w2
	bhi	.L125
.L117:
	uxtw	x4, w0
	add	x3, x19, x4, lsl 2
	ldrh	w2, [x3, 12]
	ldrh	w3, [x3, 14]
	cmp	w5, w2
	ccmp	w5, w3, 1, ge
	ccmp	w2, w3, 0, ge
	ble	.L126
	ldr	w6, [x19, 8]
	add	w0, w0, 1
	cmp	w0, w6
	bls	.L117
.L125:
	mov	w0, 0
.L110:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L124:
	add	x4, x19, x4, lsl 2
	ldrh	w1, [x4, 14]
	cmp	w1, w10
	bge	.L120
	ldr	w0, [x20, 840]
	cmp	w0, 199
	bgt	.L127
	add	x3, x20, x0, sxtw 2
	sub	w9, w9, #1
	add	w2, w0, 1
	mov	w0, 0
	strh	w1, [x3, 40]
	strh	w9, [x3, 42]
	str	w2, [x20, 840]
	b	.L110
.L122:
	ldr	x1, .LC28
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L123:
	ldr	x1, .LC30
	mov	w2, 1198
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	b	.L110
.L127:
	ldr	x1, .LC30
	mov	w2, 1213
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	b	.L110
	.size	PostPro_GetVp9MbGroup, .-PostPro_GetVp9MbGroup
	.align	3
.LC28:
	.xword	.LC27
	.align	3
.LC30:
	.xword	.LC29
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetVP9RepairParam
	.type	PostPro_GetVP9RepairParam, %function
PostPro_GetVP9RepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	x21, x2
	bl	_mcount
	add	x3, x20, 266240
	mov	w5, 1
	mov	w4, 6
	mov	x1, x21
	mov	x0, x19
	ldr	w2, [x3, 1920]
	str	w2, [x19, 4]
	ldr	w2, [x3, 1920]
	str	w2, [x19, 8]
	ldr	w2, [x3, 1928]
	str	w2, [x19, 16]
	ldr	w2, [x3, 1936]
	str	w2, [x19, 20]
	ldr	w2, [x3, 1576]
	str	w2, [x19, 24]
	ldr	w2, [x3, 1580]
	stp	w2, w5, [x19, 28]
	str	wzr, [x19, 36]
	str	wzr, [x19, 852]
	ldr	w2, [x3, 1948]
	str	w2, [x19, 848]
	str	w4, [x19, 864]
	bl	PostPro_GetVp9MbGroup
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	PostPro_GetVP9RepairParam, .-PostPro_GetVP9RepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetMp24MbGroup
	.type	PostPro_GetMp24MbGroup, %function
PostPro_GetMp24MbGroup:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w2, [x19, 8]
	sub	w0, w2, #1
	cmp	w0, 199
	bhi	.L149
	str	wzr, [x20, 840]
	str	wzr, [x20, 856]
	ldp	w5, w1, [x20, 24]
	ldr	w0, [x19, 8]
	mul	w5, w5, w1
	add	x1, x19, x0, uxtw 2
	uxth	w10, w5
	strh	w10, [x1, 14]
	strh	w10, [x1, 12]
	cbnz	w0, .L150
.L132:
	mov	w7, -1
	mov	w0, 0
	mov	w6, w7
	add	w11, w5, w7
	b	.L141
	.p2align 3
.L154:
	cmp	w2, w3
	ccmp	w2, w6, 1, le
	blt	.L133
	cmp	w2, w7
	csel	w7, w7, w6, ge
	add	w1, w7, 1
	cmp	w2, w1
	beq	.L137
	add	w6, w6, w6, lsr 31
	sub	w8, w2, #1
	cmp	w6, w8
	bge	.L137
	ldr	w1, [x20, 840]
	cmp	w1, 199
	bgt	.L151
	add	x9, x20, x1, sxtw 2
	sxth	w6, w6
	sub	w7, w7, #100
	cmp	w7, w6
	strh	w6, [x9, 40]
	strh	w8, [x9, 42]
	ble	.L139
	strh	w7, [x9, 40]
.L139:
	add	w1, w1, 1
	str	w1, [x20, 840]
.L137:
	ldr	w1, [x19, 8]
	sub	w6, w1, #1
	cmp	w6, w0
	beq	.L152
.L144:
	add	w0, w0, 1
	mov	w7, w3
	cmp	w0, w1
	mov	w6, w2
	bhi	.L153
.L141:
	uxtw	x4, w0
	add	x3, x19, x4, lsl 2
	ldrh	w2, [x3, 12]
	ldrh	w3, [x3, 14]
	cmp	w5, w2
	ccmp	w5, w3, 1, ge
	bge	.L154
.L133:
	ldr	w1, [x19, 8]
	add	w0, w0, 1
	cmp	w0, w1
	bls	.L141
.L153:
	mov	w0, 0
.L131:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L152:
	add	x4, x19, x4, lsl 2
	ldrh	w4, [x4, 14]
	cmp	w4, w11
	bge	.L144
	ldr	w0, [x20, 840]
	cmp	w0, 199
	bgt	.L155
	add	x2, x20, x0, sxtw 2
	sub	w10, w10, #1
	add	w1, w0, 1
	mov	w0, 0
	strh	w4, [x2, 40]
	strh	w10, [x2, 42]
	str	w1, [x20, 840]
	b	.L131
	.p2align 3
.L150:
	sub	w0, w0, #1
	sub	w1, w5, #10
	add	x0, x19, x0, lsl 2
	ldrh	w2, [x0, 14]
	cmp	w2, w1
	blt	.L132
	sub	w1, w10, #1
	strh	w1, [x0, 14]
	b	.L132
.L149:
	ldr	x1, .LC31
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L151:
	ldr	x1, .LC32
	mov	w2, 1560
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	b	.L131
.L155:
	ldr	x1, .LC32
	mov	w2, 1591
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	b	.L131
	.size	PostPro_GetMp24MbGroup, .-PostPro_GetMp24MbGroup
	.align	3
.LC31:
	.xword	.LC25
	.align	3
.LC32:
	.xword	.LC29
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetMPEG2RepairParam
	.type	PostPro_GetMPEG2RepairParam, %function
PostPro_GetMPEG2RepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x1
	mov	x19, x0
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	ldr	w0, [x20, 200]
	str	w0, [x19, 4]
	ldr	w0, [x20, 224]
	str	w0, [x19, 8]
	ldr	w0, [x20, 196]
	str	w0, [x19, 16]
	ldr	w0, [x20, 260]
	str	w0, [x19, 20]
	ldrh	w0, [x20, 152]
	str	w0, [x19, 24]
	ldrh	w0, [x20, 148]
	str	w0, [x19, 28]
	ldrb	w0, [x20, 3]
	cmp	w0, 3
	cset	w0, eq
	stp	w0, wzr, [x19, 32]
	ldrb	w0, [x20, 3]
	cmp	w0, 1
	beq	.L160
	cmp	w0, 2
	beq	.L160
	cmp	w0, 3
	beq	.L161
.L158:
	add	x20, x20, 45056
	mov	w3, 4
	mov	x1, x21
	mov	x0, x19
	ldr	w2, [x20, 280]
	str	w2, [x19, 848]
	str	w3, [x19, 864]
	bl	PostPro_GetMp24MbGroup
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L161:
	str	wzr, [x19, 852]
	b	.L158
	.p2align 3
.L160:
	str	w0, [x19, 852]
	b	.L158
	.size	PostPro_GetMPEG2RepairParam, .-PostPro_GetMPEG2RepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetMPEG4RepairParam
	.type	PostPro_GetMPEG4RepairParam, %function
PostPro_GetMPEG4RepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	ldr	w0, [x20, 224]
	mov	w4, 1
	str	w0, [x19, 4]
	mov	w3, 4
	mov	x1, x21
	mov	x0, x19
	ldr	w2, [x20, 14604]
	str	w2, [x19, 8]
	ldr	w2, [x20, 232]
	str	w2, [x19, 16]
	ldr	w2, [x20, 240]
	str	w2, [x19, 20]
	ldrh	w2, [x20, 176]
	str	w2, [x19, 24]
	ldrh	w2, [x20, 178]
	stp	w2, w4, [x19, 28]
	str	wzr, [x19, 36]
	str	wzr, [x19, 852]
	ldr	w2, [x20, 14684]
	str	w2, [x19, 848]
	str	w3, [x19, 864]
	bl	PostPro_GetMp24MbGroup
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	PostPro_GetMPEG4RepairParam, .-PostPro_GetMPEG4RepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetREAL8RepairParam
	.type	PostPro_GetREAL8RepairParam, %function
PostPro_GetREAL8RepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	ldr	w0, [x20, 56]
	mov	w4, 1
	str	w0, [x19, 4]
	mov	w3, 4
	mov	x1, x21
	mov	x0, x19
	ldr	w2, [x20, 52]
	str	w2, [x19, 16]
	ldr	w2, [x20, 72]
	str	w2, [x19, 20]
	ldr	w2, [x20, 4]
	str	w2, [x19, 24]
	ldr	w2, [x20, 8]
	stp	w2, w4, [x19, 28]
	str	wzr, [x19, 36]
	str	wzr, [x19, 852]
	ldr	w2, [x20, 80]
	str	w2, [x19, 848]
	str	w3, [x19, 864]
	bl	PostPro_GetMp24MbGroup
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	PostPro_GetREAL8RepairParam, .-PostPro_GetREAL8RepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetREAL9RepairParam
	.type	PostPro_GetREAL9RepairParam, %function
PostPro_GetREAL9RepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	ldr	w0, [x20, 52]
	mov	w4, 1
	str	w0, [x19, 4]
	mov	w3, 4
	mov	x1, x21
	mov	x0, x19
	ldr	w2, [x20, 48]
	str	w2, [x19, 16]
	ldr	w2, [x20, 68]
	str	w2, [x19, 20]
	ldr	w2, [x20, 4]
	str	w2, [x19, 24]
	ldr	w2, [x20, 8]
	stp	w2, w4, [x19, 28]
	str	wzr, [x19, 36]
	str	wzr, [x19, 852]
	ldr	w2, [x20, 76]
	str	w2, [x19, 848]
	str	w3, [x19, 864]
	bl	PostPro_GetMp24MbGroup
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	PostPro_GetREAL9RepairParam, .-PostPro_GetREAL9RepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetVc1MbGroup
	.type	PostPro_GetVc1MbGroup, %function
PostPro_GetVc1MbGroup:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 8]
	sub	w0, w0, #1
	cmp	w0, 199
	bhi	.L179
	str	wzr, [x20, 840]
	mov	w1, -1
	str	wzr, [x20, 856]
	mov	w0, 0
	ldp	w2, w5, [x20, 24]
	ldr	w3, [x19, 8]
	mov	w7, w1
	mul	w5, w2, w5
	add	x2, x19, x3, lsl 2
	add	w10, w5, w1
	uxth	w9, w5
	strh	w9, [x2, 14]
	strh	w9, [x2, 12]
	b	.L174
	.p2align 3
.L183:
	cmp	w2, w1
	csel	w1, w1, w7, ge
	add	w1, w1, 1
	cmp	w2, w1
	beq	.L170
	add	w7, w7, w7, lsr 31
	sub	w6, w2, #1
	cmp	w7, w6
	bge	.L170
	ldr	w1, [x20, 840]
	cmp	w1, 199
	bgt	.L180
	add	x8, x20, x1, sxtw 2
	add	w1, w1, 1
	strh	w7, [x8, 40]
	strh	w6, [x8, 42]
	str	w1, [x20, 840]
.L170:
	ldr	w6, [x19, 8]
	sub	w1, w6, #1
	cmp	w1, w0
	beq	.L181
.L177:
	add	w0, w0, 1
	mov	w1, w3
	cmp	w0, w6
	mov	w7, w2
	bhi	.L182
.L174:
	uxtw	x4, w0
	add	x3, x19, x4, lsl 2
	ldrh	w2, [x3, 12]
	ldrh	w3, [x3, 14]
	cmp	w5, w2
	ccmp	w5, w3, 1, ge
	ccmp	w2, w3, 0, ge
	ble	.L183
	ldr	w6, [x19, 8]
	add	w0, w0, 1
	cmp	w0, w6
	bls	.L174
.L182:
	mov	w0, 0
.L167:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L181:
	add	x4, x19, x4, lsl 2
	ldrh	w1, [x4, 14]
	cmp	w1, w10
	bge	.L177
	ldr	w0, [x20, 840]
	cmp	w0, 199
	bgt	.L184
	add	x3, x20, x0, sxtw 2
	sub	w9, w9, #1
	add	w2, w0, 1
	mov	w0, 0
	strh	w1, [x3, 40]
	strh	w9, [x3, 42]
	str	w2, [x20, 840]
	b	.L167
.L179:
	ldr	x1, .LC33
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L180:
	ldr	x1, .LC34
	mov	w2, 1655
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	b	.L167
.L184:
	ldr	x1, .LC34
	mov	w2, 1675
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	b	.L167
	.size	PostPro_GetVc1MbGroup, .-PostPro_GetVc1MbGroup
	.align	3
.LC33:
	.xword	.LC27
	.align	3
.LC34:
	.xword	.LC29
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetVC1RepairParam
	.type	PostPro_GetVC1RepairParam, %function
PostPro_GetVC1RepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	ldr	w0, [x20, 14564]
	mov	x1, x21
	str	w0, [x19, 4]
	mov	x0, x19
	ldr	w2, [x20, 14436]
	str	w2, [x19, 8]
	ldr	w2, [x20, 14440]
	str	w2, [x19, 16]
	ldr	w2, [x20, 14448]
	str	w2, [x19, 20]
	ldrh	w2, [x20, 82]
	add	w2, w2, 1
	str	w2, [x19, 24]
	ldrh	w2, [x20, 84]
	add	w2, w2, 1
	str	w2, [x19, 28]
	ldrb	w2, [x20, 5]
	and	w2, w2, -3
	uxtb	w2, w2
	cmp	w2, wzr
	cset	w2, eq
	stp	w2, wzr, [x19, 32]
	bl	PostPro_GetVc1MbGroup
	ldrb	w0, [x20, 72]
	ldr	w1, [x19, 32]
	and	w0, w0, 3
	str	w0, [x19, 844]
	cmp	w1, 1
	beq	.L189
	ldrb	w0, [x20, 5]
	cmp	w0, 3
	beq	.L190
.L187:
	mov	w0, 4
	str	w0, [x19, 864]
	ldr	w0, [x20, 14596]
	str	w0, [x19, 848]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L190:
	ldrb	w0, [x20, 8]
	cbz	w0, .L191
	cmp	w0, 1
	bne	.L187
	mov	w0, 2
	str	w0, [x19, 852]
	mov	w0, 4
	str	w0, [x19, 864]
	ldr	w0, [x20, 14596]
	str	w0, [x19, 848]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L191:
	mov	w0, 1
	str	w0, [x19, 852]
	b	.L187
	.p2align 3
.L189:
	str	wzr, [x19, 852]
	mov	w0, 4
	str	w0, [x19, 864]
	ldr	w0, [x20, 14596]
	str	w0, [x19, 848]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	PostPro_GetVC1RepairParam, .-PostPro_GetVC1RepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetVp6MbGroup
	.type	PostPro_GetVp6MbGroup, %function
PostPro_GetVp6MbGroup:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 8]
	sub	w0, w0, #1
	cmp	w0, 199
	bhi	.L206
	str	wzr, [x20, 840]
	mov	w1, -1
	str	wzr, [x20, 856]
	mov	w0, 0
	ldp	w2, w5, [x20, 24]
	ldr	w3, [x19, 8]
	mov	w7, w1
	mul	w5, w2, w5
	add	x2, x19, x3, lsl 2
	add	w10, w5, w1
	uxth	w9, w5
	strh	w9, [x2, 14]
	strh	w9, [x2, 12]
	b	.L201
	.p2align 3
.L210:
	cmp	w2, w1
	csel	w1, w1, w7, ge
	add	w1, w1, 1
	cmp	w2, w1
	beq	.L197
	add	w7, w7, w7, lsr 31
	sub	w6, w2, #1
	cmp	w7, w6
	bge	.L197
	ldr	w1, [x20, 840]
	cmp	w1, 199
	bgt	.L207
	add	x8, x20, x1, sxtw 2
	add	w1, w1, 1
	strh	w7, [x8, 40]
	strh	w6, [x8, 42]
	str	w1, [x20, 840]
.L197:
	ldr	w6, [x19, 8]
	sub	w1, w6, #1
	cmp	w1, w0
	beq	.L208
.L204:
	add	w0, w0, 1
	mov	w1, w3
	cmp	w0, w6
	mov	w7, w2
	bhi	.L209
.L201:
	uxtw	x4, w0
	add	x3, x19, x4, lsl 2
	ldrh	w2, [x3, 12]
	ldrh	w3, [x3, 14]
	cmp	w5, w2
	ccmp	w5, w3, 1, ge
	ccmp	w2, w3, 0, ge
	ble	.L210
	ldr	w6, [x19, 8]
	add	w0, w0, 1
	cmp	w0, w6
	bls	.L201
.L209:
	mov	w0, 0
.L194:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L208:
	add	x4, x19, x4, lsl 2
	ldrh	w1, [x4, 14]
	cmp	w1, w10
	bge	.L204
	ldr	w0, [x20, 840]
	cmp	w0, 199
	bgt	.L211
	add	x3, x20, x0, sxtw 2
	sub	w9, w9, #1
	add	w2, w0, 1
	mov	w0, 0
	strh	w1, [x3, 40]
	strh	w9, [x3, 42]
	str	w2, [x20, 840]
	b	.L194
.L206:
	ldr	x1, .LC35
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L207:
	ldr	x1, .LC36
	mov	w2, 1740
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	b	.L194
.L211:
	ldr	x1, .LC36
	mov	w2, 1760
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	b	.L194
	.size	PostPro_GetVp6MbGroup, .-PostPro_GetVp6MbGroup
	.align	3
.LC35:
	.xword	.LC27
	.align	3
.LC36:
	.xword	.LC29
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetVP6RepairParam
	.type	PostPro_GetVP6RepairParam, %function
PostPro_GetVP6RepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	ldr	w0, [x20, 228]
	mov	w3, 1
	str	w0, [x19, 4]
	mov	x1, x21
	mov	x0, x19
	ldr	w2, [x20, 228]
	str	w2, [x19, 8]
	ldr	w2, [x20, 236]
	str	w2, [x19, 16]
	ldr	w2, [x20, 240]
	str	w2, [x19, 20]
	ldrh	w2, [x20, 106]
	str	w2, [x19, 24]
	ldrh	w2, [x20, 108]
	stp	w2, w3, [x19, 28]
	str	wzr, [x19, 36]
	bl	PostPro_GetVp6MbGroup
	str	wzr, [x19, 844]
	str	wzr, [x19, 852]
	mov	w1, 4
	ldr	w0, [x20, 260]
	str	w0, [x19, 848]
	str	w1, [x19, 864]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	PostPro_GetVP6RepairParam, .-PostPro_GetVP6RepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetVp8MbGroup
	.type	PostPro_GetVp8MbGroup, %function
PostPro_GetVp8MbGroup:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 8]
	sub	w0, w0, #1
	cmp	w0, 199
	bhi	.L227
	str	wzr, [x20, 840]
	mov	w1, -1
	str	wzr, [x20, 856]
	mov	w0, 0
	ldp	w2, w5, [x20, 24]
	ldr	w3, [x19, 8]
	mov	w7, w1
	mul	w5, w2, w5
	add	x2, x19, x3, lsl 2
	add	w10, w5, w1
	uxth	w9, w5
	strh	w9, [x2, 14]
	strh	w9, [x2, 12]
	b	.L222
	.p2align 3
.L231:
	cmp	w2, w1
	csel	w1, w1, w7, ge
	add	w1, w1, 1
	cmp	w2, w1
	beq	.L218
	add	w7, w7, w7, lsr 31
	sub	w6, w2, #1
	cmp	w7, w6
	bge	.L218
	ldr	w1, [x20, 840]
	cmp	w1, 199
	bgt	.L228
	add	x8, x20, x1, sxtw 2
	add	w1, w1, 1
	strh	w7, [x8, 40]
	strh	w6, [x8, 42]
	str	w1, [x20, 840]
.L218:
	ldr	w6, [x19, 8]
	sub	w1, w6, #1
	cmp	w1, w0
	beq	.L229
.L225:
	add	w0, w0, 1
	mov	w1, w3
	cmp	w0, w6
	mov	w7, w2
	bhi	.L230
.L222:
	uxtw	x4, w0
	add	x3, x19, x4, lsl 2
	ldrh	w2, [x3, 12]
	ldrh	w3, [x3, 14]
	cmp	w5, w2
	ccmp	w5, w3, 1, ge
	ccmp	w2, w3, 0, ge
	ble	.L231
	ldr	w6, [x19, 8]
	add	w0, w0, 1
	cmp	w0, w6
	bls	.L222
.L230:
	mov	w0, 0
.L215:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L229:
	add	x4, x19, x4, lsl 2
	ldrh	w1, [x4, 14]
	cmp	w1, w10
	bge	.L225
	ldr	w0, [x20, 840]
	cmp	w0, 199
	bgt	.L232
	add	x3, x20, x0, sxtw 2
	sub	w9, w9, #1
	add	w2, w0, 1
	mov	w0, 0
	strh	w1, [x3, 40]
	strh	w9, [x3, 42]
	str	w2, [x20, 840]
	b	.L215
.L227:
	ldr	x1, .LC37
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L228:
	ldr	x1, .LC38
	mov	w2, 1824
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	b	.L215
.L232:
	ldr	x1, .LC38
	mov	w2, 1844
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	b	.L215
	.size	PostPro_GetVp8MbGroup, .-PostPro_GetVp8MbGroup
	.align	3
.LC37:
	.xword	.LC27
	.align	3
.LC38:
	.xword	.LC29
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetVP8RepairParam
	.type	PostPro_GetVP8RepairParam, %function
PostPro_GetVP8RepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	ldr	w0, [x20, 3116]
	mov	w4, 1
	str	w0, [x19, 4]
	mov	w3, 4
	mov	x1, x21
	mov	x0, x19
	ldr	w2, [x20, 3116]
	str	w2, [x19, 8]
	ldr	w2, [x20, 3128]
	str	w2, [x19, 16]
	ldr	w2, [x20, 3132]
	str	w2, [x19, 20]
	ldr	w2, [x20, 2784]
	str	w2, [x19, 24]
	ldr	w2, [x20, 2788]
	stp	w2, w4, [x19, 28]
	str	wzr, [x19, 36]
	str	wzr, [x19, 852]
	ldr	w2, [x20, 3140]
	str	w2, [x19, 848]
	str	w3, [x19, 864]
	bl	PostPro_GetVp8MbGroup
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	PostPro_GetVP8RepairParam, .-PostPro_GetVP8RepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetAvsMbGroup
	.type	PostPro_GetAvsMbGroup, %function
PostPro_GetAvsMbGroup:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w2, [x19, 8]
	sub	w0, w2, #1
	cmp	w0, 199
	bhi	.L246
	ldp	w4, w2, [x20, 24]
	str	wzr, [x20, 856]
	mov	w6, -1
	mov	x10, x20
	mov	w0, 0
	ldr	w1, [x19, 8]
	mov	w11, 0
	mul	w4, w4, w2
	mov	w9, w6
	add	x16, x20, 876
	add	x1, x19, x1, lsl 2
	add	w15, w4, w6
	add	w12, w4, w4, lsr 31
	uxth	w14, w4
	asr	w12, w12, 1
	add	w13, w12, w6
	strh	w14, [x1, 14]
	strh	w14, [x1, 12]
	b	.L241
	.p2align 3
.L249:
	cmp	w2, w6
	uxth	w1, w11
	csel	w6, w6, w9, ge
	lsr	w7, w9, 31
	add	w6, w6, 1
	mov	w8, w1
	cmp	w2, w6
	sub	w7, w7, w1
	sub	w1, w2, #1
	add	w7, w7, w9
	sub	w1, w1, w8
	mov	w9, w2
	mov	w6, w3
	ble	.L239
	ldr	w8, [x10, 840]
	cmp	w2, w13
	add	w17, w8, 1
	add	x8, x10, x8, sxtw 2
	strh	w7, [x8, 40]
	strh	w1, [x8, 42]
	str	w17, [x10, 840]
	ldr	w1, [x10, 32]
	cbnz	w1, .L239
	csel	x10, x10, x16, lt
	csel	w11, w11, w12, lt
.L239:
	ldr	w1, [x19, 8]
	sub	w7, w1, #1
	cmp	w7, w0
	beq	.L247
.L240:
	add	w0, w0, 1
	cmp	w0, w1
	bhi	.L248
.L241:
	uxtw	x5, w0
	add	x3, x19, x5, lsl 2
	ldrh	w2, [x3, 12]
	ldrh	w3, [x3, 14]
	cmp	w4, w2
	ccmp	w4, w3, 1, ge
	ccmp	w2, w3, 0, ge
	ble	.L249
	ldr	w1, [x19, 8]
	add	w0, w0, 1
	cmp	w0, w1
	bls	.L241
.L248:
	mov	w0, 0
.L236:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L247:
	add	x5, x19, x5, lsl 2
	mov	w9, w2
	mov	w6, w3
	ldrh	w2, [x5, 14]
	cmp	w2, w15
	bge	.L240
	ldr	w1, [x20, 840]
	sub	w14, w14, #1
	mov	w0, 0
	add	w3, w1, 1
	add	x1, x20, x1, sxtw 2
	strh	w2, [x1, 40]
	strh	w14, [x1, 42]
	str	w3, [x20, 840]
	b	.L236
	.p2align 3
.L246:
	ldr	x1, .LC39
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	PostPro_GetAvsMbGroup, .-PostPro_GetAvsMbGroup
	.align	3
.LC39:
	.xword	.LC25
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetAVSRepairParam
	.type	PostPro_GetAVSRepairParam, %function
PostPro_GetAVSRepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	ldr	w4, [x20, 14456]
	add	x0, x19, 768
	str	w4, [x19, 4]
	ldr	w1, [x20, 14468]
	str	w1, [x19, 8]
	ldr	w7, [x20, 14448]
	str	w7, [x19, 16]
	ldr	w2, [x20, 14460]
	str	w2, [x19, 20]
	ldrh	w6, [x20, 64]
	str	w6, [x19, 24]
	ldrh	w1, [x20, 66]
	str	w1, [x19, 28]
	lsr	w8, w1, 1
	ldrb	w3, [x20, 17]
	cmp	w3, 3
	cset	w5, eq
	stp	w5, wzr, [x19, 32]
	ldr	w3, [x21, 4]
	str	w7, [x19, 892]
	add	w4, w3, w4
	str	w4, [x19, 880]
	ldr	w3, [x21, 4]
	stp	w1, w5, [x0, 136]
	add	w3, w3, w3, lsl 3
	madd	w3, w3, w8, w2
	stp	w3, w6, [x0, 128]
	str	wzr, [x19, 912]
	ldrb	w0, [x20, 17]
	cmp	w0, 3
	beq	.L253
	cmp	w0, 1
	beq	.L254
.L252:
	ldr	w1, [x20, 14896]
	mov	w0, 4
	str	w1, [x19, 848]
	mov	x1, x21
	str	w0, [x19, 864]
	mov	x0, x19
	bl	PostPro_GetAvsMbGroup
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L254:
	str	w0, [x19, 852]
	b	.L252
	.p2align 3
.L253:
	str	wzr, [x19, 852]
	b	.L252
	.size	PostPro_GetAVSRepairParam, .-PostPro_GetAVSRepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetDivx3MbGroup
	.type	PostPro_GetDivx3MbGroup, %function
PostPro_GetDivx3MbGroup:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w2, [x19, 8]
	sub	w0, w2, #1
	cmp	w0, 3
	bhi	.L267
	ldp	w4, w2, [x20, 24]
	str	wzr, [x20, 856]
	mov	w6, -1
	mov	x10, x20
	mov	w0, 0
	ldr	w1, [x19, 8]
	mov	w11, 0
	mul	w4, w4, w2
	mov	w9, w6
	add	x16, x20, 876
	add	x1, x19, x1, lsl 2
	add	w15, w4, w6
	add	w12, w4, w4, lsr 31
	uxth	w14, w4
	asr	w12, w12, 1
	add	w13, w12, w6
	strh	w14, [x1, 14]
	strh	w14, [x1, 12]
	b	.L262
	.p2align 3
.L270:
	cmp	w2, w6
	uxth	w1, w11
	csel	w6, w6, w9, ge
	lsr	w7, w9, 31
	add	w6, w6, 1
	mov	w8, w1
	cmp	w2, w6
	sub	w7, w7, w1
	sub	w1, w2, #1
	add	w7, w7, w9
	sub	w1, w1, w8
	mov	w9, w2
	mov	w6, w3
	ble	.L260
	ldr	w8, [x10, 840]
	cmp	w2, w13
	add	w17, w8, 1
	add	x8, x10, x8, sxtw 2
	strh	w7, [x8, 40]
	strh	w1, [x8, 42]
	str	w17, [x10, 840]
	ldr	w1, [x10, 32]
	cbnz	w1, .L260
	csel	x10, x10, x16, lt
	csel	w11, w11, w12, lt
.L260:
	ldr	w1, [x19, 8]
	sub	w7, w1, #1
	cmp	w7, w0
	beq	.L268
.L261:
	add	w0, w0, 1
	cmp	w0, w1
	bhi	.L269
.L262:
	uxtw	x5, w0
	add	x3, x19, x5, lsl 2
	ldrh	w2, [x3, 12]
	ldrh	w3, [x3, 14]
	cmp	w4, w2
	ccmp	w4, w3, 1, ge
	ccmp	w2, w3, 0, ge
	ble	.L270
	ldr	w1, [x19, 8]
	add	w0, w0, 1
	cmp	w0, w1
	bls	.L262
.L269:
	mov	w0, 0
.L257:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L268:
	add	x5, x19, x5, lsl 2
	mov	w9, w2
	mov	w6, w3
	ldrh	w2, [x5, 14]
	cmp	w2, w15
	bge	.L261
	ldr	w1, [x20, 840]
	sub	w14, w14, #1
	mov	w0, 0
	add	w3, w1, 1
	add	x1, x20, x1, sxtw 2
	strh	w2, [x1, 40]
	strh	w14, [x1, 42]
	str	w3, [x20, 840]
	b	.L257
	.p2align 3
.L267:
	ldr	x1, .LC40
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	PostPro_GetDivx3MbGroup, .-PostPro_GetDivx3MbGroup
	.align	3
.LC40:
	.xword	.LC25
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetDIVX3RepairParam
	.type	PostPro_GetDIVX3RepairParam, %function
PostPro_GetDIVX3RepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	ldr	w0, [x20, 128]
	mov	w4, 1
	str	w0, [x19, 4]
	mov	w3, 4
	mov	x1, x21
	mov	x0, x19
	ldr	w2, [x20, 124]
	str	w2, [x19, 16]
	ldr	w2, [x20, 156]
	str	w2, [x19, 20]
	ldr	w2, [x20, 8]
	str	w2, [x19, 24]
	ldr	w2, [x20, 12]
	stp	w2, w4, [x19, 28]
	str	wzr, [x19, 36]
	str	wzr, [x19, 852]
	ldr	w2, [x20, 164]
	str	w2, [x19, 848]
	str	w3, [x19, 864]
	bl	PostPro_GetDivx3MbGroup
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	PostPro_GetDIVX3RepairParam, .-PostPro_GetDIVX3RepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_BubbleSort
	.type	PostPro_BubbleSort, %function
PostPro_BubbleSort:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w9, [x19, 8]
	mov	w8, 1
	add	x10, x19, 12
	cmp	w9, w8
	bls	.L272
	.p2align 2
.L281:
	mov	w1, 0
	sub	w5, w9, w8
	cmp	w1, w5
	add	w1, w1, 1
	mov	x0, x10
	mov	w7, 0
	add	x2, x19, x1, uxtw 2
	beq	.L284
	.p2align 2
.L276:
	ldrh	w3, [x0]
	ldrh	w4, [x2, 12]
	cmp	w3, w4
	bls	.L275
	strh	w4, [x0]
	mov	w7, 1
	ldrh	w4, [x0, 2]
	ldrh	w6, [x2, 14]
	strh	w6, [x0, 2]
	strh	w3, [x2, 12]
	strh	w4, [x2, 14]
.L275:
	cmp	w1, w5
	add	w1, w1, 1
	add	x0, x0, 4
	add	x2, x19, x1, uxtw 2
	bne	.L276
.L284:
	cbz	w7, .L272
	add	w8, w8, 1
	cmp	w9, w8
	ccmp	w7, 1, 0, hi
	beq	.L281
.L272:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	PostPro_BubbleSort, .-PostPro_BubbleSort
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetH264MbOfMBAFF
	.type	PostPro_GetH264MbOfMBAFF, %function
PostPro_GetH264MbOfMBAFF:
	sub	sp, sp, #800
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 8]
	ldp	w10, w11, [x20, 24]
	lsl	w8, w10, 1
	cbz	w0, .L300
	sub	w0, w0, #1
	mov	x9, 6
	add	x2, x29, 32
	add	x1, x19, 12
	add	x9, x9, x0, uxtw 2
	add	x0, x29, 34
	add	x9, x2, x9
	.p2align 2
.L287:
	ldrh	w3, [x1]
	add	x1, x1, 4
	ldrh	w2, [x1, -2]
	strh	w3, [x0, -2]
	strh	w2, [x0], 4
	cmp	x0, x9
	bne	.L287
	ldrh	w0, [x29, 34]
	ldrh	w3, [x29, 32]
	orr	w0, w0, 1
	and	w3, w3, 65534
	strh	w3, [x29, 32]
	uxth	w0, w0
	strh	w0, [x29, 34]
	cbz	w8, .L300
	add	x6, x29, 38
	mov	w7, 0
	sub	w12, w8, #2
	uxth	w13, w8
	b	.L296
	.p2align 3
.L306:
	cmp	w4, w1
	lsr	w2, w2, 1
	lsr	w3, w3, 1
	beq	.L291
	mul	w4, w13, w4
	add	x15, x19, x7, uxtw 2
	add	w14, w7, 1
	cmp	w14, 199
	uxth	w4, w4
	add	w2, w4, w2
	uxth	w2, w2
	strh	w2, [x15, 12]
	ldr	w0, [x20, 24]
	sub	w0, w0, #1
	add	w4, w4, w0
	strh	w4, [x15, 14]
	bhi	.L292
	mul	w1, w13, w1
	add	x14, x19, x14, uxtw 2
	ldr	w0, [x20, 24]
	uxth	w5, w3
	add	w4, w7, 2
	uxth	w1, w1
	add	w2, w2, w0
	cmp	w4, 199
	add	w0, w1, w5
	strh	w2, [x14, 12]
	strh	w0, [x14, 14]
	bhi	.L292
	ldr	w0, [x20, 24]
	add	x4, x19, x4, uxtw 2
	add	w7, w7, 3
	add	w1, w1, w0
	cmp	w7, 199
	uxth	w1, w1
	strh	w1, [x4, 12]
	add	w1, w5, w1
	strh	w1, [x4, 14]
	bhi	.L292
.L293:
	cmp	x6, x9
	beq	.L295
.L297:
	ldrh	w0, [x6]
	ldrh	w3, [x6, -2]
	orr	w0, w0, 1
	and	w3, w3, 65534
	strh	w3, [x6, -2]
	uxth	w0, w0
	strh	w0, [x6], 4
.L296:
	udiv	w4, w3, w8
	mul	w5, w4, w8
	udiv	w1, w0, w8
	sub	w2, w3, w5
	cmp	w2, 1
	bls	.L289
	msub	w3, w1, w8, w0
	cmp	w12, w3
	bhi	.L306
	uxth	w5, w5
	add	x4, x19, x7, uxtw 2
	add	w2, w5, w2, lsr 1
	add	w3, w7, 1
	cmp	w3, 199
	uxth	w2, w2
	strh	w2, [x4, 12]
	ldr	w1, [x20, 24]
	sub	w1, w1, #1
	add	w5, w5, w1
	strh	w5, [x4, 14]
	bhi	.L292
	add	x3, x19, x3, uxtw 2
	ldr	w1, [x20, 24]
	add	w7, w7, 2
	add	w2, w2, w1
	cmp	w7, 199
	strh	w2, [x3, 12]
	strh	w0, [x3, 14]
	bls	.L293
.L292:
	mul	w10, w10, w11
	mov	w0, 1
	strh	wzr, [x19, 12]
	sub	w10, w10, #1
	str	w0, [x19, 8]
	strh	w10, [x19, 14]
	mov	x0, x19
	bl	PostPro_BubbleSort
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	add	sp, sp, 800
	ret
	.p2align 3
.L289:
	mul	w1, w1, w8
	sub	w2, w0, w1
	cmp	w12, w2
	bls	.L294
	add	x0, x19, x7, uxtw 2
	uxth	w1, w1
	ubfx	x2, x2, 1, 16
	add	w4, w7, 1
	add	w5, w1, w2
	cmp	w4, 199
	strh	w3, [x0, 12]
	strh	w5, [x0, 14]
	bhi	.L292
	ldr	w0, [x20, 24]
	add	x4, x19, x4, uxtw 2
	add	w7, w7, 2
	add	w1, w1, w0
	cmp	w7, 199
	uxth	w1, w1
	strh	w1, [x4, 12]
	add	w1, w2, w1
	strh	w1, [x4, 14]
	bhi	.L292
	cmp	x6, x9
	bne	.L297
	.p2align 2
.L295:
	str	w7, [x19, 8]
.L307:
	mov	x0, x19
	bl	PostPro_BubbleSort
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	add	sp, sp, 800
	ret
	.p2align 3
.L294:
	add	x1, x19, x7, uxtw 2
	add	w7, w7, 1
	cmp	w7, 199
	strh	w3, [x1, 12]
	strh	w0, [x1, 14]
	bls	.L293
	b	.L292
	.p2align 3
.L291:
	uxth	w5, w5
	add	x1, x19, x7, uxtw 2
	add	w2, w5, w2
	add	w5, w5, w3
	add	w0, w7, 1
	uxth	w2, w2
	uxth	w5, w5
	strh	w2, [x1, 12]
	cmp	w0, 199
	strh	w5, [x1, 14]
	bhi	.L292
	add	x0, x19, x0, uxtw 2
	ldr	w1, [x20, 24]
	add	w7, w7, 2
	add	w2, w2, w1
	cmp	w7, 199
	strh	w2, [x0, 12]
	ldr	w1, [x20, 24]
	add	w5, w5, w1
	strh	w5, [x0, 14]
	bls	.L293
	b	.L292
.L300:
	mov	w7, 0
	str	w7, [x19, 8]
	b	.L307
	.size	PostPro_GetH264MbOfMBAFF, .-PostPro_GetH264MbOfMBAFF
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetH264MbGroup
	.type	PostPro_GetH264MbGroup, %function
PostPro_GetH264MbGroup:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x1
	mov	x21, x0
	mov	x0, x30
	mov	w22, w2
	bl	_mcount
	ldr	w2, [x20, 8]
	sub	w1, w2, #1
	cmp	w1, 199
	bhi	.L327
	ldp	w19, w0, [x21, 24]
	mul	w19, w19, w0
	cbnz	w22, .L328
.L311:
	str	wzr, [x21, 840]
	mov	w5, -1
	str	wzr, [x21, 856]
	uxth	w9, w19
	mov	w0, 0
	mov	w6, w5
	ldr	w1, [x20, 8]
	add	w10, w19, w5
	add	x1, x20, x1, lsl 2
	strh	w9, [x1, 14]
	strh	w9, [x1, 12]
	b	.L319
	.p2align 3
.L332:
	cmp	w1, w5
	csel	w5, w5, w6, ge
	add	w4, w5, 1
	cmp	w1, w4
	ble	.L314
	ldr	w4, [x21, 840]
	cmp	w4, 199
	bgt	.L329
	add	x8, x21, x4, sxtw 2
	add	w6, w6, w6, lsr 31
	sub	w5, w5, #100
	sub	w7, w1, #1
	sxth	w6, w6
	strh	w7, [x8, 42]
	cmp	w5, w6
	strh	w6, [x8, 40]
	ble	.L316
	strh	w5, [x8, 40]
.L316:
	add	w4, w4, 1
	str	w4, [x21, 840]
.L314:
	ldr	w4, [x20, 8]
	sub	w5, w4, #1
	cmp	w5, w0
	beq	.L330
.L322:
	add	w0, w0, 1
	mov	w5, w2
	cmp	w0, w4
	mov	w6, w1
	bhi	.L331
.L319:
	uxtw	x3, w0
	add	x2, x20, x3, lsl 2
	ldrh	w1, [x2, 12]
	ldrh	w2, [x2, 14]
	cmp	w19, w1
	ccmp	w19, w2, 1, ge
	ccmp	w1, w2, 0, ge
	ble	.L332
	ldr	w4, [x20, 8]
	add	w0, w0, 1
	cmp	w0, w4
	bls	.L319
.L331:
	mov	w0, 0
.L310:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L330:
	add	x3, x20, x3, lsl 2
	ldrh	w3, [x3, 14]
	cmp	w3, w10
	bge	.L322
	ldr	w0, [x21, 840]
	cmp	w0, 199
	bgt	.L333
	add	x2, x21, x0, sxtw 2
	sub	w9, w9, #1
	add	w1, w0, 1
	mov	w0, 0
	strh	w3, [x2, 40]
	strh	w9, [x2, 42]
	str	w1, [x21, 840]
	b	.L310
	.p2align 3
.L328:
	mov	w2, w22
	mov	x1, x20
	mov	x0, x21
	bl	PostPro_GetH264MbOfMBAFF
	b	.L311
.L327:
	ldr	x1, .LC43
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L329:
	ldr	x1, .LC44
	mov	w2, 1455
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	b	.L310
.L333:
	ldr	x1, .LC44
	mov	w2, 1483
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	b	.L310
	.size	PostPro_GetH264MbGroup, .-PostPro_GetH264MbGroup
	.align	3
.LC43:
	.xword	.LC25
	.align	3
.LC44:
	.xword	.LC29
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetMVCRepairParam
	.type	PostPro_GetMVCRepairParam, %function
PostPro_GetMVCRepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	ldr	w0, [x19, 292]
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 308]
	str	w0, [x20, 4]
	ldr	w0, [x19, 296]
	str	w0, [x20, 8]
	ldr	x0, [x19, 1008]
	ldr	w4, [x0, 1640]
	add	x0, x19, x4, uxtw 2
	ldr	w0, [x0, 308]
	str	w0, [x20, 16]
	ldr	w0, [x19, 300]
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 700]
	str	w0, [x20, 20]
	ldr	w0, [x19, 28]
	str	w0, [x20, 24]
	ldr	w0, [x19, 20]
	str	w0, [x20, 28]
	ldrb	w0, [x19]
	cmp	w0, wzr
	cset	w0, eq
	str	w0, [x20, 32]
	ldrb	w0, [x19, 1]
	cmp	w0, wzr
	cset	w0, eq
	str	w0, [x20, 36]
	ldr	w5, [x19, 1000]
	cbz	w5, .L335
	ldr	w0, [x19, 1208]
	cmp	w4, w0
	beq	.L372
	mov	w0, 0
	b	.L338
	.p2align 3
.L339:
	add	x3, x19, x2, lsl 2
	ldr	w3, [x3, 1208]
	cmp	w4, w3
	beq	.L336
.L338:
	add	w0, w0, 1
	cmp	w0, w5
	sxtw	x2, w0
	bne	.L339
.L335:
	ldr	w2, [x19, 32]
	cbz	w2, .L340
	ldrb	w0, [x19]
	cmp	w0, wzr
	cset	w2, eq
.L340:
	ldrsb	w0, [x19, 2]
	cbnz	w0, .L373
	str	wzr, [x21, 8]
	ldr	w2, [x20, 4]
	ldr	w0, [x20, 16]
	cmp	w2, w0
	beq	.L374
.L343:
	ldrb	w0, [x19]
	cbz	w0, .L352
	cmp	w0, 1
	beq	.L370
	cmp	w0, 2
	beq	.L370
	cmp	w0, 3
	bne	.L349
.L352:
	str	wzr, [x20, 852]
.L349:
	ldr	w1, [x19, 1272]
	mov	w0, 4
	str	w1, [x20, 848]
	str	w0, [x20, 864]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L370:
	str	w0, [x20, 852]
	mov	w0, 4
	ldr	w1, [x19, 1272]
	str	w1, [x20, 848]
	str	w0, [x20, 864]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L372:
	mov	x2, 0
	.p2align 2
.L336:
	add	x2, x19, x2, lsl 2
	ldr	w0, [x2, 1144]
	str	w0, [x20, 844]
	b	.L335
	.p2align 3
.L373:
	mov	x1, x21
	mov	x0, x20
	bl	PostPro_GetH264MbGroup
	ldr	w2, [x20, 4]
	ldr	w0, [x20, 16]
	cmp	w2, w0
	bne	.L343
.L374:
	ldr	w3, [x19, 696]
	cbz	w3, .L343
	ldr	w1, [x19, 308]
	mov	w0, 0
	cmp	w2, w1
	bne	.L344
	.p2align 2
.L346:
	add	w0, w0, 1
	cmp	w3, w0
	add	x1, x19, x0, sxtw 2
	beq	.L343
	ldr	w1, [x1, 308]
	cmp	w2, w1
	beq	.L346
.L344:
	str	w1, [x20, 16]
	b	.L343
	.size	PostPro_GetMVCRepairParam, .-PostPro_GetMVCRepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetRepairParam
	.type	PostPro_GetRepairParam, %function
PostPro_GetRepairParam:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x26, x1
	mov	w20, w0
	add	x25, x1, 876
	mov	x0, x30
	mov	x19, x2
	mov	x21, x3
	mov	w22, w4
	bl	_mcount
	str	w20, [x26]
	cmp	w20, 17
	str	wzr, [x26, 840]
	str	wzr, [x25, 840]
	bls	.L405
.L376:
	ldr	x2, .LC46
	mov	w3, 916
	ldr	x1, .LC48
	mov	w0, 0
	add	x2, x2, 32
	bl	dprint_vfmw
	.p2align 2
.L391:
	ldp	w20, w1, [x26, 24]
	ldr	w0, [x26, 840]
	mul	w20, w20, w1
	cbz	w0, .L397
	ldr	x24, .LC50
	mov	w22, 0
	mov	w23, 0
	.p2align 2
.L393:
	add	x19, x26, x23, uxtw 2
	mov	x1, x24
	mov	w0, 22
	add	w23, w23, 1
	ldrsh	w2, [x19, 42]
	ldrsh	w3, [x19, 40]
	bl	dprint_vfmw
	ldrsh	w0, [x19, 42]
	ldrsh	w2, [x19, 40]
	ldr	w1, [x26, 840]
	sub	w0, w0, w2
	cmp	w1, w23
	add	w0, w0, 1
	add	w22, w22, w0
	bhi	.L393
.L392:
	ldr	w0, [x25, 840]
	cbz	w0, .L394
	sub	w0, w0, #1
	mov	x2, 922
	add	x1, x26, 918
	add	x0, x2, x0, uxtw 2
	add	x26, x26, x0
	.p2align 2
.L395:
	ldrsh	w0, [x1]
	add	x1, x1, 4
	ldrsh	w2, [x1, -6]
	cmp	x26, x1
	sub	w0, w0, w2
	add	w0, w0, 1
	add	w22, w22, w0
	bne	.L395
.L394:
	mov	w19, 0
	cbz	w20, .L396
	lsl	w19, w22, 2
	mov	w1, 100
	sub	w19, w19, w22
	lsl	w0, w19, 5
	add	w19, w19, w0
	add	w19, w19, w22
	udiv	w19, w19, w20
	cmp	w19, w1
	csel	w19, w19, w1, ls
.L396:
	ldr	x1, .LC52
	mov	w3, w22
	mov	w2, w20
	str	wzr, [x21, 8]
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L405:
	ldr	x0, .LC45
	ldrh	w0, [x0,w20,uxtw #1]
	adr	x1, .Lrtx378
	add	x0, x1, w0, sxth #2
	br	x0
.Lrtx378:
	.section	.rodata
	.align	0
	.align	2
.L378:
	.2byte	(.L388 - .Lrtx378) / 4
	.2byte	(.L379 - .Lrtx378) / 4
	.2byte	(.L380 - .Lrtx378) / 4
	.2byte	(.L381 - .Lrtx378) / 4
	.2byte	(.L376 - .Lrtx378) / 4
	.2byte	(.L382 - .Lrtx378) / 4
	.2byte	(.L383 - .Lrtx378) / 4
	.2byte	(.L376 - .Lrtx378) / 4
	.2byte	(.L384 - .Lrtx378) / 4
	.2byte	(.L385 - .Lrtx378) / 4
	.2byte	(.L386 - .Lrtx378) / 4
	.2byte	(.L386 - .Lrtx378) / 4
	.2byte	(.L386 - .Lrtx378) / 4
	.2byte	(.L387 - .Lrtx378) / 4
	.2byte	(.L376 - .Lrtx378) / 4
	.2byte	(.L388 - .Lrtx378) / 4
	.2byte	(.L389 - .Lrtx378) / 4
	.2byte	(.L390 - .Lrtx378) / 4
	.text
.L388:
	mov	w3, w22
	mov	x2, x21
	mov	x1, x19
	mov	x0, x26
	bl	PostPro_GetMVCRepairParam
	b	.L391
.L389:
	mov	w3, w22
	mov	x2, x21
	mov	x1, x19
	mov	x0, x26
	bl	PostPro_GetHEVCRepairParam
	b	.L391
.L390:
	add	x19, x19, 266240
	mov	w4, 1
	mov	w3, 6
	mov	x1, x21
	mov	x0, x26
	ldr	w2, [x19, 1920]
	str	w2, [x26, 4]
	ldr	w2, [x19, 1920]
	str	w2, [x26, 8]
	ldr	w2, [x19, 1928]
	str	w2, [x26, 16]
	ldr	w2, [x19, 1936]
	str	w2, [x26, 20]
	ldr	w2, [x19, 1576]
	str	w2, [x26, 24]
	ldr	w2, [x19, 1580]
	stp	w2, w4, [x26, 28]
	str	wzr, [x26, 36]
	str	wzr, [x26, 852]
	ldr	w2, [x19, 1948]
	str	w2, [x26, 848]
	str	w3, [x26, 864]
	bl	PostPro_GetVp9MbGroup
	b	.L391
.L379:
	mov	w3, w22
	mov	x2, x21
	mov	x1, x19
	mov	x0, x26
	bl	PostPro_GetVC1RepairParam
	b	.L391
.L380:
	ldr	w0, [x19, 224]
	mov	w4, 1
	str	w0, [x26, 4]
	mov	w3, 4
	mov	x1, x21
	mov	x0, x26
	ldr	w2, [x19, 14604]
	str	w2, [x26, 8]
	ldr	w2, [x19, 232]
	str	w2, [x26, 16]
	ldr	w2, [x19, 240]
	str	w2, [x26, 20]
	ldrh	w2, [x19, 176]
	str	w2, [x26, 24]
	ldrh	w2, [x19, 178]
	stp	w2, w4, [x26, 28]
	str	wzr, [x26, 36]
	str	wzr, [x26, 852]
	ldr	w2, [x19, 14684]
	str	w2, [x26, 848]
	str	w3, [x26, 864]
	bl	PostPro_GetMp24MbGroup
	b	.L391
.L381:
	mov	w3, w22
	mov	x2, x21
	mov	x1, x19
	mov	x0, x26
	bl	PostPro_GetMPEG2RepairParam
	b	.L391
.L382:
	ldr	w0, [x19, 128]
	mov	w4, 1
	str	w0, [x26, 4]
	mov	w3, 4
	mov	x1, x21
	mov	x0, x26
	ldr	w2, [x19, 124]
	str	w2, [x26, 16]
	ldr	w2, [x19, 156]
	str	w2, [x26, 20]
	ldr	w2, [x19, 8]
	str	w2, [x26, 24]
	ldr	w2, [x19, 12]
	stp	w2, w4, [x26, 28]
	str	wzr, [x26, 36]
	str	wzr, [x26, 852]
	ldr	w2, [x19, 164]
	str	w2, [x26, 848]
	str	w3, [x26, 864]
	bl	PostPro_GetDivx3MbGroup
	b	.L391
.L383:
	mov	w3, w22
	mov	x2, x21
	mov	x1, x19
	mov	x0, x26
	bl	PostPro_GetAVSRepairParam
	b	.L391
.L384:
	ldr	w0, [x19, 56]
	mov	w4, 1
	str	w0, [x26, 4]
	mov	w3, 4
	mov	x1, x21
	mov	x0, x26
	ldr	w2, [x19, 52]
	str	w2, [x26, 16]
	ldr	w2, [x19, 72]
	str	w2, [x26, 20]
	ldr	w2, [x19, 4]
	str	w2, [x26, 24]
	ldr	w2, [x19, 8]
	stp	w2, w4, [x26, 28]
	str	wzr, [x26, 36]
	str	wzr, [x26, 852]
	ldr	w2, [x19, 80]
	str	w2, [x26, 848]
	str	w3, [x26, 864]
	bl	PostPro_GetMp24MbGroup
	b	.L391
.L385:
	ldr	w0, [x19, 52]
	mov	w4, 1
	str	w0, [x26, 4]
	mov	w3, 4
	mov	x1, x21
	mov	x0, x26
	ldr	w2, [x19, 48]
	str	w2, [x26, 16]
	ldr	w2, [x19, 68]
	str	w2, [x26, 20]
	ldr	w2, [x19, 4]
	str	w2, [x26, 24]
	ldr	w2, [x19, 8]
	stp	w2, w4, [x26, 28]
	str	wzr, [x26, 36]
	str	wzr, [x26, 852]
	ldr	w2, [x19, 76]
	str	w2, [x26, 848]
	str	w3, [x26, 864]
	bl	PostPro_GetMp24MbGroup
	b	.L391
.L386:
	mov	w3, w22
	mov	x2, x21
	mov	x1, x19
	mov	x0, x26
	bl	PostPro_GetVP6RepairParam
	b	.L391
.L387:
	ldr	w0, [x19, 3116]
	mov	w4, 1
	str	w0, [x26, 4]
	mov	w3, 4
	mov	x1, x21
	mov	x0, x26
	ldr	w2, [x19, 3116]
	str	w2, [x26, 8]
	ldr	w2, [x19, 3128]
	str	w2, [x26, 16]
	ldr	w2, [x19, 3132]
	str	w2, [x26, 20]
	ldr	w2, [x19, 2784]
	str	w2, [x26, 24]
	ldr	w2, [x19, 2788]
	stp	w2, w4, [x26, 28]
	str	wzr, [x26, 36]
	str	wzr, [x26, 852]
	ldr	w2, [x19, 3140]
	str	w2, [x26, 848]
	str	w3, [x26, 864]
	bl	PostPro_GetVp8MbGroup
	b	.L391
.L397:
	mov	w22, 0
	b	.L392
	.size	PostPro_GetRepairParam, .-PostPro_GetRepairParam
	.align	3
.LC45:
	.xword	.L378
	.align	3
.LC46:
	.xword	.LANCHOR0
	.align	3
.LC48:
	.xword	.LC47
	.align	3
.LC50:
	.xword	.LC49
	.align	3
.LC52:
	.xword	.LC51
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetErrRatio
	.type	PostPro_GetErrRatio, %function
PostPro_GetErrRatio:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L411
	ldr	x3, .LC55
	mov	w1, 0
	ldp	w23, w21, [x19]
	mov	x2, 24
	add	x0, x29, 72
	ldr	x3, [x3, 96]
	ldr	x22, [x19, 8]
	sxtw	x19, w23
	lsl	x20, x19, 2
	blr	x3
	sub	x20, x20, x19
	lsl	x0, x19, 3
	ldr	x1, .LC58
	sub	x0, x0, x19
	add	x20, x20, x20, lsl 4
	lsl	x0, x0, 3
	ldr	x3, .LC57
	sub	x0, x0, x19
	ldr	x1, [x1, 288]
	lsl	x0, x0, 2
	add	x20, x3, x20, lsl 4
	sub	x0, x0, x19
	str	w21, [x29, 72]
	ldr	x19, .LC56
	str	x20, [x29, 80]
	str	w23, [x29, 88]
	add	x19, x19, x0, lsl 3
	cbz	x1, .L409
	add	x0, x29, 72
	blr	x1
.L410:
	mov	w4, w23
	mov	x3, x20
	mov	x1, x19
	mov	x2, x22
	mov	w0, w21
	bl	PostPro_GetRepairParam
	mov	w19, w0
	ldr	x1, .LC62
	mov	w2, w0
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L409:
	ldr	x1, .LC60
	mov	w2, 130
	mov	w0, 0
	bl	dprint_vfmw
	b	.L410
	.p2align 3
.L411:
	ldr	x1, .LC54
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, 1
	ldp	x29, x30, [sp], 96
	ret
	.size	PostPro_GetErrRatio, .-PostPro_GetErrRatio
	.align	3
.LC54:
	.xword	.LC53
	.align	3
.LC55:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC56:
	.xword	g_RepairParam
	.align	3
.LC57:
	.xword	g_DecReport
	.align	3
.LC58:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC60:
	.xword	.LC59
	.align	3
.LC62:
	.xword	.LC61
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_GetH264RepairParam
	.type	PostPro_GetH264RepairParam, %function
PostPro_GetH264RepairParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	x21, x2
	mov	w22, w3
	bl	_mcount
	mov	w3, w22
	mov	x2, x21
	mov	x1, x20
	mov	x0, x19
	bl	PostPro_GetMVCRepairParam
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	PostPro_GetH264RepairParam, .-PostPro_GetH264RepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_SetFullRepair
	.type	PostPro_SetFullRepair, %function
PostPro_SetFullRepair:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w2
	mov	x21, x1
	bl	_mcount
	sxtw	x3, w19
	ldr	x6, .LC63
	lsl	x5, x3, 2
	lsl	x0, x3, 3
	sub	x5, x5, x3
	sub	x0, x0, x3
	lsl	x0, x0, 3
	ldr	x1, .LC64
	add	x5, x5, x5, lsl 4
	sub	x0, x0, x3
	lsl	x5, x5, 4
	lsl	x0, x0, 2
	add	x7, x6, x5
	sub	x0, x0, x3
	mov	w4, w19
	mov	x3, x7
	mov	x2, x21
	add	x1, x1, x0, lsl 3
	str	wzr, [x7, 8]
	mov	w0, w20
	mov	w7, 1
	str	w7, [x6, x5]
	bl	PostPro_GetRepairParam
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	PostPro_SetFullRepair, .-PostPro_SetFullRepair
	.align	3
.LC63:
	.xword	g_DecReport
	.align	3
.LC64:
	.xword	g_RepairParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PostPro_SetDecReport
	.type	PostPro_SetDecReport, %function
PostPro_SetDecReport:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x2
	mov	x21, x1
	bl	_mcount
	ldr	x3, .LC65
	mov	x2, 816
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	cmp	w20, 17
	bls	.L432
.L415:
	ldr	x2, .LC67
	mov	w3, 2253
	ldr	x1, .LC68
	mov	w0, 0
	bl	dprint_vfmw
.L430:
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L432:
	ldr	x0, .LC66
	ldrb	w0, [x0,w20,uxtw]
	adr	x1, .Lrtx417
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx417:
	.section	.rodata
	.align	0
	.align	2
.L417:
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L415 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L422 - .Lrtx417) / 4
	.byte	(.L415 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L415 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.byte	(.L426 - .Lrtx417) / 4
	.text
	.p2align 3
.L422:
	ldrb	w0, [x21, 17]
	cmp	w0, 1
	beq	.L433
.L426:
	strh	wzr, [x19, 12]
	mov	w0, 1
	strh	wzr, [x19, 14]
	str	w0, [x19, 8]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L433:
	strh	wzr, [x19, 12]
	mov	w0, 2
	strh	wzr, [x19, 14]
	str	w0, [x19, 8]
	ldrh	w1, [x21, 66]
	ldrh	w0, [x21, 64]
	mul	w0, w0, w1
	add	w0, w0, w0, lsr 31
	asr	w0, w0, 1
	strh	w0, [x19, 16]
	ldrh	w0, [x21, 64]
	ldrh	w1, [x21, 66]
	mul	w0, w0, w1
	add	w0, w0, w0, lsr 31
	asr	w0, w0, 1
	strh	w0, [x19, 18]
	b	.L430
	.size	PostPro_SetDecReport, .-PostPro_SetDecReport
	.align	3
.LC65:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC66:
	.xword	.L417
	.align	3
.LC67:
	.xword	.LANCHOR1
	.align	3
.LC68:
	.xword	.LC47
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.11853, %object
	.size	__func__.11853, 31
__func__.11853:
	.string	"PostPro_GetHevcMbGroupOfNoTile"
	.zero	1
	.type	__func__.11796, %object
	.size	__func__.11796, 23
__func__.11796:
	.string	"PostPro_GetRepairParam"
	.align	3
.LANCHOR1 = . + 0
	.type	__func__.12060, %object
	.size	__func__.12060, 21
__func__.12060:
	.string	"PostPro_SetDecReport"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC3:
	ASCII(.string	"pRepairParam->ImageWidth=%d,pRepairParam->ImageHeight=%d\n" )
	.zero	6
.LC16:
	ASCII(.string	"pRepairParam->ValidGroupNum >(MAX_UP_MSG_SLICE_NUM - 1)\n" )
	.zero	7
.LC21:
	ASCII(.string	"%s:0 pRepairParam->ValidGroupNum(%d) > (MAX_UP_MSG_SLICE_NUM - 1)\n" )
	.zero	5
.LC23:
	ASCII(.string	"%s:1 pRepairParam->ValidGroupNum(%d) > (MAX_UP_MSG_SLICE_NUM - 1)\n" )
	.zero	5
.LC25:
	ASCII(.string	"DecSliceNum(=%d) out of range!\n" )
.LC27:
	ASCII(.string	"DecSliceNum out of range!\n" )
	.zero	5
.LC29:
	ASCII(.string	"line:%d! pRepairParam->ValidGroupNum > (MAX_UP_MSG_SLICE_NUM - 1)\n" )
	.zero	5
.LC47:
	ASCII(.string	"%s %d VidStd Err!\n" )
	.zero	5
.LC49:
	ASCII(.string	"pRepairParam[0].MbGroup[i].EndMbn=%d,pRepairParam[0].MbGroup[i].StartMbn=%d\n" )
	.zero	3
.LC51:
	ASCII(.string	"total_mbs=%d,err_mbs=%d\n" )
	.zero	7
.LC53:
	ASCII(.string	"PostPro_GetErrRatio: pParam = NULL\n" )
	.zero	4
.LC59:
	ASCII(.string	"VDMHAL_NULL_FUN_PRINT,L%d\n" )
	.zero	5
.LC61:
	ASCII(.string	"err_ratio=%d\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
