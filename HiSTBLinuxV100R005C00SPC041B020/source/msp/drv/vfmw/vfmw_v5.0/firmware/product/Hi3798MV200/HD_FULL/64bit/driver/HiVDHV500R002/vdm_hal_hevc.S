#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vdm_hal_hevc.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	HEVCWriteCabacTab
	.type	HEVCWriteCabacTab, %function
HEVCWriteCabacTab:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC3
	ldr	x8, .LC2
	mov	w2, 16
	mov	w3, 153
	mov	w4, 0
	mov	w9, 0
	mov	w11, w2
	mov	x10, x8
.L2:
	sxtw	x0, w9
	lsl	x7, x0, 7
	add	x7, x7, x0, lsl 5
	b	.L12
	.p2align 3
.L5:
	strb	w0, [x19, x5]
	add	w2, w2, 1
	strb	w1, [x6, 1]
	cmp	w2, 159
	add	w4, w4, 1
	add	x0, x7, x2, sxtw
	bgt	.L17
.L7:
	ldr	w3, [x8, x0, lsl 2]
.L12:
	asr	w0, w3, 4
	and	w1, w3, 15
	cmn	w3, #1
	sbfiz	x5, x4, 1, 32
	add	w0, w0, w0, lsl 2
	ubfiz	w1, w1, 3, 5
	add	x6, x19, x5
	sub	w0, w0, #45
	sub	w1, w1, #16
	bne	.L5
	add	w2, w2, 1
	strb	wzr, [x19, x5]
	strb	wzr, [x6, 1]
	cmp	w2, 159
	add	w4, w4, 1
	add	x0, x7, x2, sxtw
	ble	.L7
.L17:
	add	w9, w9, 1
	cmp	w9, 3
	beq	.L18
	sxtw	x0, w9
	cmp	w9, wzr
	csel	w2, w11, wzr, eq
	lsl	x1, x0, 7
	add	x0, x1, x0, lsl 5
	add	x0, x0, x2, sxtw
	ldr	w3, [x10, x0, lsl 2]
	b	.L2
.L18:
	mov	w0, w20
	bl	MEM_Phy2Vir
	mov	x1, x19
	add	x3, x19, 928
	cbz	x0, .L19
	.p2align 2
.L13:
	ldrsb	w2, [x1]
	add	x1, x1, 2
	strb	w2, [x0, 1]
	cmp	x3, x1
	ldrsb	w2, [x1, -1]
	strb	w2, [x0], 2
	bne	.L13
	mov	w0, 0
.L15:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L19:
	ldr	x2, .LC2
	ldr	x3, .LC5
	ldr	x1, .LC7
	add	x2, x2, 1920
	bl	dprint_vfmw
	mov	w0, -1
	b	.L15
	.size	HEVCWriteCabacTab, .-HEVCWriteCabacTab
	.align	3
.LC2:
	.xword	.LANCHOR0
	.align	3
.LC3:
	.xword	.LANCHOR1
	.align	3
.LC5:
	.xword	.LC4
	.align	3
.LC7:
	.xword	.LC6
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCHAL_V400R003_InitHal
	.type	HEVCHAL_V400R003_InitHal, %function
HEVCHAL_V400R003_InitHal:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 1124]
	bl	HEVCWriteCabacTab
	cbnz	w0, .L25
.L21:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L25:
	ldr	x1, .LC10
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L21
	.size	HEVCHAL_V400R003_InitHal, .-HEVCHAL_V400R003_InitHal
	.align	3
.LC10:
	.xword	.LC9
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCGet_V400R003_VirAddr
	.type	HEVCGet_V400R003_VirAddr, %function
HEVCGet_V400R003_VirAddr:
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
	.size	HEVCGet_V400R003_VirAddr, .-HEVCGet_V400R003_VirAddr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCGet_V400R003_PhyAddr
	.type	HEVCGet_V400R003_PhyAddr, %function
HEVCGet_V400R003_PhyAddr:
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
	.size	HEVCGet_V400R003_PhyAddr, .-HEVCGet_V400R003_PhyAddr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCHAL_V400R003_CutSliceChain
	.type	HEVCHAL_V400R003_CutSliceChain, %function
HEVCHAL_V400R003_CutSliceChain:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	mov	w0, w19
	bl	MEM_Phy2Vir
	cbz	x0, .L31
	str	wzr, [x0, 252]
	mov	w0, 0
.L30:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L31:
	ldr	x2, .LC13
	ldr	x3, .LC12
	ldr	x1, .LC14
	add	x2, x2, 1944
	bl	dprint_vfmw
	mov	w0, -1
	b	.L30
	.size	HEVCHAL_V400R003_CutSliceChain, .-HEVCHAL_V400R003_CutSliceChain
	.align	3
.LC12:
	.xword	.LC11
	.align	3
.LC13:
	.xword	.LANCHOR0
	.align	3
.LC14:
	.xword	.LC6
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_WriteQmatrix
	.type	HEVC_WriteQmatrix, %function
HEVC_WriteQmatrix:
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
	cbz	w23, .L32
	sub	w23, w23, #1
	ldr	x25, .LC18
	ldr	x24, .LC20
	add	x23, x23, 1
	mov	x19, 0
	.p2align 2
.L34:
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
	bne	.L34
.L32:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.size	HEVC_WriteQmatrix, .-HEVC_WriteQmatrix
	.align	3
.LC18:
	.xword	.LC17
	.align	3
.LC20:
	.xword	.LC19
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_WriteTileInfo
	.type	HEVC_WriteTileInfo, %function
HEVC_WriteTileInfo:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x1
	mov	x25, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 1128]
	bl	MEM_Phy2Vir
	mov	x24, x0
	cbz	x0, .L38
	ldr	x21, .LC26
	add	x26, x25, 1818
	add	x27, x0, 1024
	mov	x22, x0
.L39:
	add	x28, x22, 4
	add	x23, x22, 516
	mov	x20, x26
	mov	x19, 0
	b	.L43
	.p2align 3
.L55:
	ldrb	w5, [x20, 1]
	add	x28, x28, 4
	ldrb	w4, [x20]
	add	x20, x20, 4
	ldrb	w3, [x20, -5]
	ldrb	w2, [x20, -6]
	lsl	w4, w4, 16
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3, lsl 8
	orr	w2, w4, w2
	str	w2, [x22, x19]
	add	x19, x19, 4
	bl	dprint_vfmw
	cmp	x23, x28
	beq	.L54
.L43:
	add	w0, w19, 3
	mov	x1, x21
	cmp	w0, 255
	mov	w2, 0
	mov	w0, 4
	ble	.L55
	str	wzr, [x22, x19]
	mov	x1, x21
	mov	w0, 4
	add	x28, x28, 4
	add	x19, x19, 4
	add	x20, x20, 4
	bl	dprint_vfmw
	cmp	x23, x28
	bne	.L43
.L54:
	add	x22, x22, 512
	add	x26, x26, 256
	cmp	x27, x22
	bne	.L39
	add	x2, x24, 1028
	add	x1, x25, 2332
	mov	w0, 0
	b	.L47
	.p2align 3
.L57:
	ldp	w4, w3, [x1, -4]
	add	w0, w0, 1
	add	x2, x2, 4
	cmp	w0, 20
	add	x1, x1, 8
	orr	w3, w4, w3, lsl 16
	str	w3, [x2, -8]
	beq	.L56
.L47:
	cmp	w0, 9
	ble	.L57
	add	w0, w0, 1
	str	wzr, [x2, -4]
	cmp	w0, 20
	add	x2, x2, 4
	add	x1, x1, 8
	bne	.L47
.L56:
	add	x24, x24, 1108
	add	x25, x25, 2412
	mov	w0, 0
	b	.L50
	.p2align 3
.L59:
	ldp	w2, w1, [x25, -4]
	add	w0, w0, 1
	add	x24, x24, 4
	cmp	w0, 22
	add	x25, x25, 8
	orr	w1, w2, w1, lsl 16
	str	w1, [x24, -8]
	beq	.L58
.L50:
	cmp	w0, 10
	ble	.L59
	add	w0, w0, 1
	str	wzr, [x24, -4]
	cmp	w0, 22
	add	x24, x24, 4
	add	x25, x25, 8
	bne	.L50
.L58:
	mov	w0, 0
.L40:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L38:
	ldr	x2, .LC23
	mov	w0, 0
	ldr	x3, .LC22
	ldr	x1, .LC24
	add	x2, x2, 1976
	bl	dprint_vfmw
	mov	w0, -1
	b	.L40
	.size	HEVC_WriteTileInfo, .-HEVC_WriteTileInfo
	.align	3
.LC22:
	.xword	.LC21
	.align	3
.LC23:
	.xword	.LANCHOR0
	.align	3
.LC24:
	.xword	.LC6
	.align	3
.LC26:
	.xword	.LC25
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCHAL_V400R003_SetPicMsg
	.type	HEVCHAL_V400R003_SetPicMsg, %function
HEVCHAL_V400R003_SetPicMsg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	w21, w2
	mov	x23, x1
	mov	w22, w3
	bl	_mcount
	mov	w0, w21
	bl	MEM_Phy2Vir
	mov	x20, x0
	cbz	x0, .L75
	ldr	x3, .LC31
	mov	x2, 1280
	mov	w1, 0
	ldr	x3, [x3, 96]
	blr	x3
	ldr	x1, .LC33
	mov	w0, 4
	bl	dprint_vfmw
	ldrb	w5, [x19]
	ldrb	w3, [x19, 2]
	ldrb	w1, [x19, 1]
	ldrb	w2, [x19, 3]
	ldrb	w0, [x19, 4]
	lsl	w1, w1, 24
	lsl	w2, w2, 22
	orr	w6, w1, w5, lsl 25
	orr	w2, w2, w3, lsl 23
	ldr	w4, [x19, 8]
	orr	w1, w6, w2
	orr	w3, w1, w0, lsl 21
	ldrb	w1, [x19, 12]
	ldp	w5, w2, [x19, 16]
	lsl	w5, w5, 9
	orr	w2, w2, w1, lsl 18
	orr	w0, w5, w4, lsl 19
	ldr	x1, .LC35
	orr	w0, w0, w2
	mov	w2, w21
	orr	w0, w3, w0
	str	w0, [x20]
	mov	w0, 4
	ldr	w3, [x20]
	bl	dprint_vfmw
	ldr	w0, [x19, 24]
	str	w0, [x20, 4]
	ldp	w5, w1, [x19, 36]
	ldp	w0, w3, [x19, 44]
	ldr	w2, [x19, 28]
	lsl	w5, w5, 24
	lsl	w0, w0, 18
	ldr	w4, [x19, 60]
	orr	w0, w0, w1, lsl 21
	orr	w5, w5, w2, lsl 29
	ldp	w2, w1, [x19, 52]
	orr	w0, w5, w0
	lsl	w5, w2, 12
	orr	w2, w5, w3, lsl 15
	orr	w5, w0, w2
	lsl	w0, w4, 6
	ldp	w3, w2, [x19, 64]
	orr	w4, w0, w1, lsl 9
	ldrb	w1, [x19, 32]
	orr	w2, w2, w3, lsl 3
	ldrb	w3, [x19, 33]
	orr	w0, w5, w4
	orr	w2, w0, w2
	lsl	w0, w3, 27
	orr	w0, w0, w1, lsl 28
	orr	w0, w2, w0
	str	w0, [x20, 8]
	ldp	w3, w2, [x19, 72]
	ldp	w5, w4, [x19, 80]
	ldp	w1, w0, [x19, 88]
	lsl	w4, w4, 8
	lsl	w2, w2, 16
	orr	w4, w4, w5, lsl 12
	orr	w2, w2, w3, lsl 22
	orr	w2, w4, w2
	orr	w0, w0, w1, lsl 4
	orr	w0, w2, w0
	str	w0, [x20, 12]
	ldr	w0, [x23, 1108]
	str	w0, [x20, 16]
	ldr	w0, [x23, 1112]
	str	w0, [x20, 20]
	ldr	w0, [x19, 1284]
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 1468]
	str	w0, [x20, 24]
	ldr	w0, [x23, 1120]
	str	w0, [x20, 28]
	ldr	w0, [x19, 2560]
	cbz	w0, .L66
	mov	w1, 0
	.p2align 2
.L64:
	add	x0, x19, x1, uxtw 2
	add	w2, w1, 8
	add	w1, w1, 1
	ldr	w0, [x0, 2496]
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 1336]
	str	w0, [x20, w2, uxtw 2]
	ldr	w0, [x19, 2560]
	cmp	w0, w1
	bhi	.L64
	cmp	w0, 15
	bhi	.L67
	.p2align 2
.L66:
	ldr	w1, [x19, 2496]
	add	w2, w0, 8
	add	w0, w0, 1
	cmp	w0, 15
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 1336]
	str	w1, [x20, w2, uxtw 2]
	bls	.L66
.L67:
	ldr	w1, [x23, 1124]
	add	x0, x19, 120
	str	w1, [x20, 100]
	add	x3, x19, 184
	add	x1, x20, 104
	.p2align 2
.L65:
	ldr	w2, [x0], 4
	str	w2, [x1], 4
	cmp	x3, x0
	bne	.L65
	ldr	w0, [x23, 1116]
	mov	x1, x23
	str	w0, [x20, 172]
	mov	x0, x19
	ldr	w2, [x19, 1280]
	add	x2, x19, x2, lsl 2
	ldr	w2, [x2, 1684]
	str	w2, [x20, 208]
	ldr	w2, [x19, 1812]
	str	w2, [x20, 212]
	ldp	w2, w3, [x19, 184]
	orr	w2, w3, w2, lsl 16
	str	w2, [x20, 216]
	bl	HEVC_WriteTileInfo
	mov	w24, w0
	cbnz	w0, .L76
	ldr	w0, [x23, 1128]
	mov	x1, x20
	str	w0, [x20, 220]
	mov	w0, 64
	ldr	w2, [x23, 1140]
	str	w2, [x20, 224]
	ldr	w2, [x23, 1132]
	str	w2, [x20, 228]
	ldr	w2, [x23, 1144]
	str	w2, [x20, 232]
	ldr	w2, [x23, 1136]
	str	w2, [x20, 236]
	ldp	w4, w3, [x19, 220]
	ldrb	w2, [x19, 212]
	ldrb	w5, [x19, 213]
	lsl	w3, w3, 13
	lsl	w5, w5, 23
	orr	w5, w5, w2, lsl 24
	orr	w2, w3, w4, lsl 16
	orr	w3, w5, w2
	ldrb	w4, [x19, 214]
	ldrb	w5, [x19, 215]
	lsl	w5, w5, 21
	orr	w2, w5, w4, lsl 22
	ldrb	w4, [x19, 216]
	orr	w5, w3, w2
	ldrb	w3, [x19, 217]
	lsl	w3, w3, 19
	orr	w2, w3, w4, lsl 20
	ldrb	w4, [x19, 228]
	orr	w3, w5, w2
	ldrb	w5, [x19, 229]
	lsl	w5, w5, 11
	orr	w2, w5, w4, lsl 12
	orr	w5, w3, w2
	ldp	w4, w2, [x19, 232]
	ubfiz	w3, w4, 5, 5
	ldrb	w4, [x19, 230]
	and	w2, w2, 31
	orr	w3, w3, w4, lsl 10
	orr	w3, w5, w3
	orr	w2, w3, w2
	str	w2, [x20, 240]
	ldp	w2, w3, [x19, 240]
	orr	w2, w3, w2, lsl 16
	str	w2, [x20, 244]
	str	w22, [x20, 252]
	bl	HEVC_GetVirAddr
	mov	x20, x0
	mov	w1, w21
	mov	w0, 64
	bl	HEVC_GetPhyAddr
	mov	w2, w0
	mov	x3, x20
	add	x0, x19, 252
	mov	w1, 256
	bl	HEVC_WriteQmatrix
.L62:
	mov	w0, w24
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L76:
	ldr	x1, .LC37
	mov	w0, 1
	mov	w24, -1
	bl	dprint_vfmw
	b	.L62
.L75:
	ldr	x2, .LC29
	mov	w0, 0
	ldr	x3, .LC28
	mov	w24, -1
	ldr	x1, .LC30
	add	x2, x2, 2000
	bl	dprint_vfmw
	b	.L62
	.size	HEVCHAL_V400R003_SetPicMsg, .-HEVCHAL_V400R003_SetPicMsg
	.align	3
.LC28:
	.xword	.LC27
	.align	3
.LC29:
	.xword	.LANCHOR0
	.align	3
.LC30:
	.xword	.LC6
	.align	3
.LC31:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC33:
	.xword	.LC32
	.align	3
.LC35:
	.xword	.LC34
	.align	3
.LC37:
	.xword	.LC36
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCHAL_V400R003_SetSliceMsg
	.type	HEVCHAL_V400R003_SetSliceMsg, %function
HEVCHAL_V400R003_SetSliceMsg:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x27, x0
	mov	x0, x30
	mov	w21, w3
	mov	x19, x2
	stp	w7, w6, [x29, 100]
	str	w5, [x29, 108]
	bl	_mcount
	mov	w0, w21
	bl	MEM_Phy2Vir
	mov	x28, x0
	cbz	x0, .L125
	ldr	x1, .LC42
	mov	x2, 1280
	mov	x26, x0
	add	x24, x19, 16
	mov	w22, 1
	mov	w20, 0
	ldr	x3, [x1, 96]
	mov	w1, 0
	blr	x3
	ldr	x23, .LC46
	mov	x25, x23
.L84:
	lsl	w2, w20, 2
	mov	w3, w21
	sub	w2, w2, w20
	mov	x1, x23
	mov	w0, 4
	lsl	w8, w20, 2
	cbz	w20, .L126
	ldr	w4, [x24, -16]
	cbz	w4, .L82
.L81:
	ldr	w4, [x24]
	and	w8, w4, -16
	ldr	w4, [x27, 1324]
	and	w4, w4, -16
	sub	w4, w8, w4
	str	w4, [x26]
	ldr	w4, [x26]
	bl	dprint_vfmw
	ldr	w0, [x24]
	add	w3, w21, 4
	ldr	w1, [x24, -8]
	mov	w2, w22
	add	w0, w1, w0, lsl 3
	mov	x1, x23
	and	w0, w0, 127
	str	w0, [x26, 4]
	mov	w0, 4
	ldr	w4, [x26, 4]
	bl	dprint_vfmw
	ldr	w0, [x24, -16]
	add	w3, w21, 8
	str	w0, [x26, 8]
	add	w2, w22, 1
	mov	x1, x23
	mov	w0, 4
	ldr	w4, [x26, 8]
	bl	dprint_vfmw
.L83:
	add	w20, w20, 1
	add	x24, x24, 4
	cmp	w20, 2
	add	x26, x26, 12
	add	w21, w21, 12
	add	w22, w22, 3
	bne	.L84
	ldrb	w0, [x19, 24]
	ldrb	w2, [x19, 25]
	ldrb	w1, [x19, 32]
	lsl	w2, w2, 26
	orr	w2, w2, w0, lsl 27
	ldr	w0, [x19, 36]
	orr	w2, w2, w0
	ldr	w0, [x19, 28]
	ubfiz	w0, w0, 19, 7
	orr	w0, w0, w1, lsl 18
	orr	w0, w2, w0
	str	w0, [x28, 24]
	ldp	w2, w4, [x19, 56]
	ldr	w0, [x19, 44]
	lsl	w2, w2, 20
	ldr	w1, [x19, 76]
	orr	w5, w2, w0, lsl 24
	ldr	x0, .LC40
	add	x0, x0, 2064
	ldr	w3, [x0, x1, lsl 2]
	ldr	w1, [x19, 72]
	lsl	w1, w1, 8
	orr	w1, w1, w4, lsl 16
	orr	w2, w5, w1
	ldr	w1, [x19, 68]
	orr	w0, w3, w1, lsl 2
	ldrb	w1, [x19, 64]
	orr	w3, w2, w0
	ldrb	w0, [x19, 65]
	lsl	w0, w0, 14
	orr	w0, w0, w1, lsl 15
	ldr	w1, [x19, 40]
	orr	w2, w3, w0
	ldrb	w0, [x19, 52]
	lsl	w0, w0, 27
	orr	w0, w0, w1, lsl 28
	orr	w0, w2, w0
	ldr	w2, [x29, 104]
	str	w0, [x28, 28]
	str	w2, [x28, 32]
	ldr	w0, [x27, 20]
	add	w0, w0, 1
	udiv	w1, w2, w0
	msub	w24, w1, w0, w2
	ldr	w0, [x29, 100]
	orr	w24, w24, w1, lsl 16
	str	w24, [x28, 36]
	str	w0, [x28, 40]
	ldp	w1, w0, [x19, 96]
	ubfiz	w1, w1, 8, 5
	and	w0, w0, 31
	orr	w0, w1, w0
	str	w0, [x28, 44]
	ldrb	w1, [x19, 113]
	ldrb	w2, [x19, 112]
	ldp	w0, w3, [x19, 104]
	lsl	w2, w2, 2
	orr	w2, w2, w1, lsl 3
	ubfiz	w0, w0, 16, 4
	ldrb	w1, [x19, 114]
	ubfiz	w3, w3, 8, 4
	orr	w3, w0, w3
	ldrb	w0, [x19, 115]
	orr	w2, w3, w2
	orr	w0, w0, w1, lsl 1
	orr	w0, w2, w0
	str	w0, [x28, 48]
	ldr	w4, [x19, 68]
	cbz	w4, .L109
	add	x3, x19, 116
	mov	w2, 0
	mov	w0, 0
	.p2align 2
.L86:
	ldr	w1, [x3], 4
	lsl	w1, w1, w0
	add	w0, w0, 1
	cmp	w0, w4
	orr	w2, w2, w1
	bne	.L86
.L85:
	str	w2, [x28, 52]
	ldr	w4, [x19, 72]
	cbz	w4, .L110
	add	x3, x19, 180
	mov	w2, 0
	mov	w0, 0
	.p2align 2
.L88:
	ldr	w1, [x3], 4
	lsl	w1, w1, w0
	add	w0, w0, 1
	cmp	w0, w4
	orr	w2, w2, w1
	bne	.L88
.L87:
	str	w2, [x28, 56]
	ldr	w4, [x19, 68]
	cbz	w4, .L95
	mov	w3, 0
	mov	w0, 0
	mov	w7, 15
	b	.L94
	.p2align 3
.L92:
	cmp	w6, w0
	beq	.L127
.L93:
	add	w0, w0, 1
	cmp	w4, w0
	bls	.L95
.L94:
	add	x2, x19, x0, uxtw 2
	and	w1, w0, 7
	lsl	w5, w1, 2
	cmp	w1, 7
	sub	w6, w4, #1
	ldr	w1, [x2, 372]
	lsl	w1, w1, w5
	orr	w3, w3, w1
	bne	.L92
	str	w3, [x28, w7, uxtw 2]
	add	w0, w0, 1
	add	w7, w7, 1
	mov	w3, 0
	ldr	w4, [x19, 68]
	cmp	w4, w0
	bhi	.L94
.L95:
	ldr	w4, [x19, 72]
	cbz	w4, .L91
	mov	w3, 0
	mov	w0, 0
	mov	w7, 17
	b	.L100
	.p2align 3
.L98:
	cmp	w6, w0
	beq	.L128
.L99:
	add	w0, w0, 1
	cmp	w4, w0
	bls	.L91
.L100:
	add	x2, x19, x0, uxtw 2
	and	w1, w0, 7
	lsl	w5, w1, 2
	cmp	w1, 7
	sub	w6, w4, #1
	ldr	w1, [x2, 436]
	lsl	w1, w1, w5
	orr	w3, w3, w1
	bne	.L98
	str	w3, [x28, w7, uxtw 2]
	add	w0, w0, 1
	add	w7, w7, 1
	mov	w3, 0
	ldr	w4, [x19, 72]
	cmp	w4, w0
	bhi	.L100
.L91:
	ldr	w0, [x19, 564]
	mov	w1, 16394
	str	w0, [x28, 92]
	ldr	w0, [x27, 76]
	lsr	w0, w0, 1
	orr	w0, w1, w0, lsl 8
	str	w0, [x28, 96]
	ldr	w0, [x29, 112]
	str	w0, [x28, 172]
	ldr	w0, [x29, 120]
	str	w0, [x28, 176]
	ldr	w0, [x29, 108]
	str	w0, [x28, 252]
	ldrb	w0, [x27, 33]
	cbz	w0, .L97
	ldr	w0, [x19, 76]
	cmp	w0, 1
	beq	.L101
.L97:
	ldrb	w0, [x27, 32]
	cmp	w0, 1
	beq	.L102
.L103:
	mov	w0, 0
.L79:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L127:
	str	w3, [x28, w7, uxtw 2]
	ldr	w4, [x19, 68]
	b	.L93
	.p2align 3
.L128:
	str	w3, [x28, w7, uxtw 2]
	ldr	w4, [x19, 72]
	b	.L99
	.p2align 3
.L82:
	str	wzr, [x26]
	sub	w2, w8, w20
	mov	w3, w21
	mov	x1, x25
	ldr	w4, [x26]
	mov	w0, 4
	bl	dprint_vfmw
	str	wzr, [x26, 4]
	add	w3, w21, 4
	mov	w2, w22
	ldr	w4, [x26, 4]
	mov	x1, x25
	mov	w0, 4
	bl	dprint_vfmw
	str	wzr, [x26, 8]
	add	w3, w21, 8
	add	w2, w22, 1
	ldr	w4, [x26, 8]
	mov	x1, x25
	mov	w0, 4
	bl	dprint_vfmw
	b	.L83
	.p2align 3
.L102:
	ldr	w0, [x19, 76]
	cbnz	w0, .L103
.L101:
	mov	x1, x28
	mov	w0, 64
	bl	HEVC_GetVirAddr
	mov	x20, x0
	mov	x1, x28
	mov	w0, 128
	bl	HEVC_GetVirAddr
	ldr	w1, [x19, 68]
	mov	w2, 0
	cbz	w1, .L107
	.p2align 2
.L115:
	uxtw	x1, w2
	ldr	w3, [x19, 580]
	add	w7, w2, 32
	add	x6, x19, x1, lsl 2
	and	w4, w3, 7
	add	x1, x19, x1, lsl 3
	ldrb	w5, [x6, 716]
	ldr	w3, [x6, 588]
	ubfiz	w3, w3, 3, 9
	orr	w3, w3, w5, lsl 12
	orr	w3, w3, w4
	str	w3, [x20, w2, uxtw 2]
	ldrb	w5, [x1, 1100]
	ldr	w4, [x1, 844]
	ldr	w3, [x19, 584]
	ubfiz	w4, w4, 3, 9
	orr	w4, w4, w5, lsl 12
	and	w3, w3, 7
	orr	w3, w4, w3
	str	w3, [x0, w2, uxtw 2]
	add	w2, w2, 1
	ldrb	w3, [x1, 1104]
	ldr	w1, [x1, 848]
	and	w1, w1, 511
	orr	w1, w1, w3, lsl 9
	str	w1, [x0, w7, uxtw 2]
	ldr	w1, [x19, 68]
	cmp	w1, w2
	bhi	.L115
.L107:
	ldr	w1, [x19, 76]
	cbnz	w1, .L103
	ldr	w1, [x19, 72]
	cbz	w1, .L103
	mov	w2, 0
	.p2align 2
.L108:
	uxtw	x1, w2
	ldr	w3, [x19, 580]
	add	w6, w2, 16
	add	w8, w2, 48
	add	x7, x19, x1, lsl 2
	and	w4, w3, 7
	add	x1, x19, x1, lsl 3
	add	w2, w2, 1
	ldrb	w5, [x7, 780]
	ldr	w3, [x7, 652]
	ubfiz	w3, w3, 3, 9
	orr	w3, w3, w5, lsl 12
	orr	w3, w3, w4
	str	w3, [x20, w6, uxtw 2]
	ldrb	w5, [x1, 1228]
	ldr	w4, [x1, 972]
	ldr	w3, [x19, 584]
	ubfiz	w4, w4, 3, 9
	orr	w4, w4, w5, lsl 12
	and	w3, w3, 7
	orr	w3, w4, w3
	str	w3, [x0, w6, uxtw 2]
	ldrb	w3, [x1, 1232]
	ldr	w1, [x1, 976]
	and	w1, w1, 511
	orr	w1, w1, w3, lsl 9
	str	w1, [x0, w8, uxtw 2]
	ldr	w1, [x19, 72]
	cmp	w1, w2
	bhi	.L108
	b	.L103
.L109:
	mov	w2, 0
	b	.L85
.L110:
	mov	w2, 0
	b	.L87
.L125:
	ldr	x2, .LC40
	mov	w0, 0
	ldr	x3, .LC39
	ldr	x1, .LC41
	add	x2, x2, 2032
	bl	dprint_vfmw
	mov	w0, -1
	b	.L79
.L126:
	ldr	w4, [x19]
	cbnz	w4, .L81
	ldr	x2, .LC40
	mov	w0, 0
	ldr	x1, .LC44
	add	x2, x2, 2032
	bl	dprint_vfmw
	mov	w0, -1
	b	.L79
	.size	HEVCHAL_V400R003_SetSliceMsg, .-HEVCHAL_V400R003_SetSliceMsg
	.align	3
.LC39:
	.xword	.LC38
	.align	3
.LC40:
	.xword	.LANCHOR0
	.align	3
.LC41:
	.xword	.LC6
	.align	3
.LC42:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC44:
	.xword	.LC43
	.align	3
.LC46:
	.xword	.LC45
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCHAL_V400R003_CfgVdmReg
	.type	HEVCHAL_V400R003_CfgVdmReg, %function
HEVCHAL_V400R003_CfgVdmReg:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x19, x0
	mov	x0, x30
	mov	w21, w2
	mov	x20, x3
	mov	x22, x1
	bl	_mcount
	ldr	x3, .LC49
	sbfiz	x0, x21, 6, 32
	ldr	x1, .LC50
	cmp	x20, xzr
	add	x0, x3, x0
	ldr	x2, .LC48
	ldrb	w1, [x1]
	ldrsw	x0, [x0, 8]
	ccmp	w1, 1, 4, eq
	ldr	x0, [x2, x0, lsl 3]
	ldr	w26, [x0, 1316]
	bne	.L143
	cbz	x0, .L138
	ldr	w23, [x0, 1336]
	uxtb	w0, w23
	and	w25, w0, 1
.L132:
	cmp	w1, 1
	beq	.L133
	strb	w0, [x20, 1]
.L133:
	ldr	w1, [x19, 5284]
	mov	w0, 1
	ldrb	w3, [x19, 1]
	cmp	w1, 1
	ldr	w2, [x19, 16]
	csetm	w1, eq
	str	wzr, [x29, 92]
	bfi	w0, w1, 2, 2
	ldr	w1, [x19, 20]
	bfi	w0, w3, 6, 1
	strb	wzr, [x29, 94]
	and	w0, w0, 127
	mov	x3, x20
	and	w0, w0, -49
	strb	w0, [x29, 95]
	madd	w2, w1, w2, w2
	ldr	w24, [x29, 92]
	mov	w0, 8
	add	w1, w2, w1
	mov	w2, w21
	bfi	w24, w1, 0, 20
	str	w24, [x19, 2584]
	mov	w1, w24
	str	w24, [x29, 92]
	bl	MFDE_ConfigReg
	ldr	x1, .LC55
	mov	w2, w24
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w1, [x19, 1320]
	mov	w0, 0
	bfi	w0, w26, 3, 1
	ldr	w2, [x19, 1332]
	bfi	w0, w1, 4, 1
	ldr	w1, [x19, 188]
	and	w0, w0, -33
	mov	w3, 13
	bfi	w0, w2, 6, 1
	strb	w3, [x29, 92]
	orr	w0, w0, -128
	mov	w2, 3
	strb	w0, [x29, 93]
	cmp	w1, 1920
	strh	w2, [x29, 94]
	ldrb	w0, [x29, 95]
	bls	.L144
	ldr	w1, [x19, 1316]
	and	w0, w0, -33
	strb	w0, [x29, 95]
	and	w1, w1, 1
.L135:
	ldrb	w0, [x29, 95]
	mov	x3, x20
	mov	w2, w21
	bfi	w0, w1, 6, 1
	and	w0, w0, 127
	bfi	w0, w25, 4, 1
	strb	w0, [x29, 95]
	mov	w0, 12
	ldr	w24, [x29, 92]
	str	w24, [x19, 2588]
	mov	w1, w24
	bl	MFDE_ConfigReg
	ldr	x1, .LC57
	mov	w2, w24
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w24, [x22, 72]
	mov	x3, x20
	mov	w0, 16
	and	w24, w24, -16
	mov	w2, w21
	mov	w1, w24
	str	w24, [x19, 2592]
	bl	MFDE_ConfigReg
	ldr	x1, .LC59
	mov	w2, w24
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w4, [x22, 56]
	mov	x3, x20
	mov	w2, w21
	mov	w0, 20
	and	w4, w4, -16
	str	w4, [x19, 2596]
	mov	w1, w4
	mov	w22, 3075
	movk	w22, 0x30, lsl 16
	bl	MFDE_ConfigReg
	ldr	w4, [x19, 1324]
	mov	x3, x20
	mov	w2, w21
	mov	w0, 24
	and	w4, w4, -16
	str	w4, [x19, 2600]
	mov	w1, w4
	bl	MFDE_ConfigReg
	add	x4, x19, 2560
	mov	w0, 60
	mov	x3, x20
	mov	w2, w21
	mov	w1, w22
	stp	w22, w22, [x4, 48]
	stp	w22, w22, [x4, 56]
	str	w22, [x19, 2624]
	str	w22, [x19, 2628]
	str	w22, [x19, 2632]
	bl	MFDE_ConfigReg
	mov	x3, x20
	mov	w2, w21
	mov	w1, w22
	mov	w0, 64
	bl	MFDE_ConfigReg
	mov	x3, x20
	mov	w2, w21
	mov	w1, w22
	mov	w0, 68
	bl	MFDE_ConfigReg
	mov	x3, x20
	mov	w2, w21
	mov	w1, w22
	mov	w0, 72
	bl	MFDE_ConfigReg
	mov	x3, x20
	mov	w2, w21
	mov	w1, w22
	mov	w0, 76
	bl	MFDE_ConfigReg
	mov	x3, x20
	mov	w2, w21
	mov	w1, w22
	mov	w0, 80
	bl	MFDE_ConfigReg
	mov	x3, x20
	mov	w2, w21
	mov	w1, w22
	mov	w0, 84
	bl	MFDE_ConfigReg
	ldr	w1, [x19, 1280]
	mov	x3, x20
	mov	w2, w21
	mov	w0, 96
	add	x1, x19, x1, lsl 2
	ldr	w4, [x1, 1336]
	and	w4, w4, -256
	str	w4, [x19, 2636]
	mov	w1, w4
	str	w4, [x29, 92]
	bl	MFDE_ConfigReg
	cmp	w23, 1
	beq	.L145
.L136:
	ldr	w22, [x19, 1288]
	mov	x3, x20
	mov	w0, 100
	mov	w2, w21
	mov	w1, w22
	str	w22, [x19, 2640]
	bl	MFDE_ConfigReg
	ldr	x1, .LC65
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w22, [x19, 1292]
	mov	x3, x20
	mov	w0, 104
	mov	w2, w21
	mov	w1, w22
	str	w22, [x19, 2644]
	bl	MFDE_ConfigReg
	ldr	x1, .LC67
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w1, [x19, 1300]
	mov	x3, x20
	mov	w2, w21
	mov	w0, 108
	bl	MFDE_ConfigReg
	ldr	w4, [x19, 1304]
	mov	x3, x20
	mov	w2, w21
	mov	w0, 116
	mov	w1, w4
	str	w4, [x19, 2660]
	bl	MFDE_ConfigReg
	ldr	w4, [x19, 1308]
	mov	x3, x20
	mov	w2, w21
	mov	w0, 120
	mov	w1, w4
	str	w4, [x19, 2664]
	bl	MFDE_ConfigReg
	ldr	w4, [x19, 1312]
	mov	x3, x20
	str	w4, [x19, 2668]
	mov	w2, w21
	mov	w1, w4
	mov	w0, 124
	bl	MFDE_ConfigReg
	str	wzr, [x19, 2684]
	mov	x3, x20
	mov	w2, w21
	mov	w1, 0
	mov	w0, 128
	str	wzr, [x29, 92]
	bl	MFDE_ConfigReg
	strh	wzr, [x29, 92]
	mov	w0, 132
	mov	x3, x20
	ldr	w1, [x29, 92]
	mov	w2, w21
	str	w1, [x19, 2688]
	bl	MFDE_ConfigReg
	ldr	w0, [x19, 188]
	cmp	w0, 4096
	bls	.L139
	mov	w1, 0
	mov	w19, 0
.L137:
	mov	x3, x20
	mov	w0, 4
	mov	w2, w21
	bl	SCD_ConfigReg
	ldr	x1, .LC69
	mov	w2, w19
	mov	w0, 3
	bl	dprint_vfmw
	mov	x3, x20
	mov	w2, w21
	mov	w1, 0
	mov	w0, 152
	bl	MFDE_ConfigReg
	ldr	x1, .LC71
	mov	w2, 0
	mov	w0, 3
	bl	dprint_vfmw
	mov	w0, 32
	mov	x3, x20
	mov	w2, w21
	mov	w1, -1
	bl	MFDE_ConfigReg
	mov	w0, 0
.L131:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L144:
	str	wzr, [x19, 1316]
	orr	w0, w0, 32
	mov	w1, 0
	strb	w0, [x29, 95]
	b	.L135
	.p2align 3
.L139:
	mov	w1, 1
	mov	w19, w1
	b	.L137
	.p2align 3
.L138:
	mov	w25, 0
	mov	w0, 0
	mov	w23, 0
	b	.L132
	.p2align 3
.L145:
	mov	x3, x20
	mov	w2, w21
	mov	w1, 60
	mov	w0, 92
	bl	MFDE_ConfigReg
	ldr	x1, .LC61
	mov	w2, 60
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w1, [x19, 1280]
	mov	w2, w21
	mov	w0, 112
	mov	x3, x20
	add	x1, x19, x1, lsl 2
	ldr	w22, [x1, 1556]
	mov	w1, w22
	bl	MFDE_ConfigReg
	ldr	x1, .LC63
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	b	.L136
.L143:
	ldr	x2, .LC51
	mov	x3, x20
	ldr	x1, .LC53
	mov	w0, 0
	add	x2, x2, 2080
	bl	dprint_vfmw
	mov	w0, -1
	b	.L131
	.size	HEVCHAL_V400R003_CfgVdmReg, .-HEVCHAL_V400R003_CfgVdmReg
	.align	3
.LC48:
	.xword	s_pstVfmwChan
	.align	3
.LC49:
	.xword	g_VdmDrvParam
	.align	3
.LC50:
	.xword	g_HalDisable
	.align	3
.LC51:
	.xword	.LANCHOR0
	.align	3
.LC53:
	.xword	.LC52
	.align	3
.LC55:
	.xword	.LC54
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCHAL_V400R003_StartDec
	.type	HEVCHAL_V400R003_StartDec, %function
HEVCHAL_V400R003_StartDec:
	sub	sp, sp, #128
	stp	x29, x30, [sp, 16]
	add	x29, sp, 16
	stp	x19, x20, [sp, 32]
	stp	x21, x22, [sp, 48]
	stp	x23, x24, [sp, 64]
	stp	x25, x26, [sp, 80]
	stp	x27, x28, [sp, 96]
	mov	x23, x0
	mov	x0, x30
	mov	w19, w1
	mov	x20, x2
	bl	_mcount
	ldr	x24, .LC72
	sxtw	x0, w19
	cmp	w19, wzr
	lsl	x21, x0, 6
	sub	x21, x21, x0
	lsl	x21, x21, 2
	sub	x0, x21, x0
	add	x21, x21, x0, lsl 2
	add	x21, x24, x21
	bgt	.L219
	bne	.L211
	cbz	x23, .L220
	ldr	w0, [x23, 2560]
	cmp	w0, 16
	bhi	.L221
	ldr	x0, [x24]
	cbz	x0, .L222
.L154:
	mov	x3, x20
	mov	w2, 0
	mov	x1, x21
	mov	x0, x23
	bl	HEVCHAL_V400R003_CfgVdmReg
	ldp	w2, w3, [x24, 72]
	mov	x1, x21
	mov	x0, x23
	bl	HEVCHAL_V400R003_SetPicMsg
	ldr	x19, [x23, 2568]
	cbz	x19, .L223
	ldr	w0, [x23, 5284]
	cmp	w0, 1
	beq	.L224
	ldr	w22, [x19, 92]
	cbnz	w22, .L225
	mov	x22, x19
	mov	w27, 0
.L168:
	ldp	w26, w0, [x23, 16]
	mov	w2, 0
	mov	w28, 0
	mov	w3, 0
	add	w25, w27, 5
	madd	w0, w26, w0, w0
	add	w26, w0, w26
	add	w0, w27, 4
	str	w0, [x29, 104]
.L171:
	ldr	w1, [x23, 2564]
	cmp	w3, w1
	bge	.L185
	cbz	x22, .L226
	ldr	w7, [x22, 92]
	cmp	w3, wzr
	ccmp	w7, w2, 2, gt
	bcc	.L227
	add	w20, w3, 1
	cmp	w1, w20
	ble	.L191
	ldr	x19, [x22, 1360]
	cbnz	x19, .L215
	b	.L175
	.p2align 3
.L177:
	add	w20, w20, 1
	cmp	w1, w20
	ble	.L174
	ldr	x19, [x19, 1360]
	cbz	x19, .L175
.L215:
	ldr	w0, [x19, 92]
	cmp	w7, w0
	bcs	.L177
.L174:
	ldr	w2, [x29, 104]
	add	w0, w3, w25
	cmp	w0, 209
	ldr	w6, [x22, 80]
	add	w3, w2, w3
	add	x0, x24, x0, sxtw 2
	ccmp	w1, w20, 4, ne
	add	x3, x24, x3, sxtw 2
	ldr	w4, [x3, 56]
	ldr	w3, [x0, 56]
	beq	.L228
	ldr	w0, [x19, 92]
	ldrb	w1, [x23, 217]
	sub	w0, w0, #1
	mov	w2, w0
	cbz	w1, .L181
	ldr	w2, [x19, 568]
.L181:
	add	w1, w25, w20
	cmp	w2, w26
	ccmp	w26, w0, 1, le
	add	x1, x24, x1, sxtw 2
	ldr	w9, [x1, 56]
	blt	.L229
.L180:
	cmp	w7, w0
	str	w0, [x22, 572]
	bhi	.L230
	ldr	w1, [x23, 2564]
	sub	w5, w20, #1
	sub	w1, w1, #1
	cmp	w5, w1
	beq	.L231
.L183:
	mov	w5, w9
	str	w2, [sp]
	str	w0, [sp, 8]
	mov	x1, x21
	mov	x2, x22
	mov	x0, x23
	str	w9, [x29, 108]
	bl	HEVCHAL_V400R003_SetSliceMsg
	ldr	w9, [x29, 108]
	cbnz	w0, .L232
	add	w28, w28, 1
	ldr	w2, [x22, 572]
	cbz	w9, .L233
	mov	x22, x19
	mov	w3, w20
	b	.L171
	.p2align 3
.L225:
	ldrb	w2, [x23, 217]
	sub	w0, w22, #1
	mov	w1, w0
	cbnz	w2, .L234
.L169:
	ldp	w27, w28, [x19]
	ldp	w26, w25, [x19, 8]
	mov	w20, 1
	mov	w7, 0
	stp	w20, wzr, [x19]
	stp	wzr, wzr, [x19, 8]
	ldp	w2, w3, [x19, 16]
	stp	w3, w2, [x29, 104]
	ldr	w2, [x23, 1324]
	mov	w6, 0
	stp	w2, wzr, [x19, 16]
	str	w1, [sp]
	mov	x2, x19
	str	w0, [sp, 8]
	mov	x1, x21
	mov	x0, x23
	ldp	w4, w3, [x24, 72]
	ldr	w5, [x24, 80]
	bl	HEVCHAL_V400R003_SetSliceMsg
	cbnz	w0, .L170
	ldp	w3, w2, [x29, 104]
	stp	w27, w28, [x19]
	stp	w26, w25, [x19, 8]
	stp	w2, w3, [x19, 16]
	str	w22, [x19, 92]
	mov	w27, w20
	ldr	x22, [x23, 2568]
	b	.L168
	.p2align 3
.L175:
	ldr	x2, .LC73
	mov	w0, 1
	ldr	x1, .LC96
	add	x2, x2, 2112
	bl	dprint_vfmw
	mov	w0, -1
.L148:
	add	sp, x29, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L231:
	str	xzr, [x22, 1360]
	b	.L183
	.p2align 3
.L224:
	ldr	w2, [x23, 2564]
	cmp	w2, wzr
	ble	.L158
	mov	w26, 0
	mov	x20, 0
	mov	w0, 0
	.p2align 2
.L160:
	ldrb	w1, [x19, 25]
	cmp	w1, wzr
	csel	w26, w26, w0, ne
	add	w0, w0, 1
	csel	x20, x20, x19, ne
	cmp	w0, w2
	ldr	x19, [x19, 1360]
	bne	.L160
	cbz	x20, .L158
	sub	w0, w0, w26
	mov	w19, 0
	cmp	w0, wzr
	mov	x25, x24
	mov	w22, 205
	bgt	.L202
	b	.L217
	.p2align 3
.L235:
	ldr	w2, [x1, 56]
	ldr	x1, [x20, 1360]
	ldrb	w4, [x23, 217]
	mov	w5, w2
	ldr	w0, [x1, 92]
	sub	w0, w0, #1
	cbz	w4, .L213
	ldr	w1, [x1, 568]
	mov	w5, w2
	.p2align 2
.L164:
	str	w1, [sp]
	mov	x2, x20
	str	w0, [sp, 8]
	mov	w4, 0
	mov	x1, x21
	mov	x0, x23
	add	w19, w19, 1
	bl	HEVCHAL_V400R003_SetSliceMsg
	cbnz	w0, .L170
	ldr	w0, [x23, 2564]
	ldr	x20, [x20, 1360]
	sub	w0, w0, w26
	cmp	w0, w19
	ble	.L217
.L202:
	add	w1, w19, 5
	sub	w0, w0, #1
	cmp	w0, w19
	mov	w0, w1
	add	w2, w19, 6
	ccmp	w19, w22, 0, gt
	add	x0, x24, x0, sxtw 2
	mov	w1, w2
	mov	w5, 0
	ldr	w6, [x20, 80]
	add	x1, x25, x1, sxtw 2
	ldr	w7, [x20, 92]
	ldr	w3, [x0, 56]
	ble	.L235
	ldp	w1, w0, [x23, 16]
	madd	w0, w1, w0, w0
	add	w0, w0, w1
.L213:
	mov	w1, w0
	b	.L164
	.p2align 3
.L234:
	ldr	w1, [x19, 568]
	b	.L169
.L233:
	ldr	w1, [x23, 2564]
.L185:
	cmp	w28, wzr
	ccmp	w1, wzr, 4, gt
	ble	.L187
	add	w0, w27, w1
	add	w0, w0, 4
	add	x0, x24, x0, sxtw 2
	ldr	w0, [x0, 56]
	bl	HEVCHAL_V400R003_CutSliceChain
.L217:
	add	sp, x29, 0
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
.L228:
	mov	w2, w26
	mov	w0, w26
	mov	w9, 0
	str	xzr, [x22, 1360]
	b	.L180
.L222:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L155
	str	x0, [x24]
	b	.L154
.L170:
	ldr	x2, .LC73
	mov	w0, 1
	ldr	x1, .LC90
	add	x2, x2, 2112
	bl	dprint_vfmw
	add	sp, x29, 0
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
.L187:
	ldr	x1, .LC104
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L148
	.p2align 3
.L191:
	mov	x19, x22
	b	.L174
.L219:
	ldr	x2, .LC73
	mov	w4, 1
	ldr	x1, .LC75
	mov	w3, w19
	add	x2, x2, 2112
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L148
.L226:
	ldr	x2, .LC73
	mov	w0, 1
	ldr	x1, .LC92
	add	x2, x2, 2112
	bl	dprint_vfmw
	mov	w0, -1
	b	.L148
.L229:
	ldr	x1, .LC98
	mov	w3, w0
	mov	w4, w26
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L148
.L230:
	ldr	x1, .LC100
	mov	w3, w0
	mov	w2, w7
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L148
.L232:
	ldr	x2, .LC73
	mov	w0, 1
	ldr	x1, .LC102
	add	x2, x2, 2112
	bl	dprint_vfmw
	mov	w0, -1
	b	.L148
.L227:
	ldr	x1, .LC94
	mov	w3, w2
	mov	w0, 1
	mov	w2, w7
	bl	dprint_vfmw
	mov	w0, -1
	b	.L148
.L211:
	ldr	x1, .LC77
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L148
.L221:
	ldr	x2, .LC73
	ldr	x3, .LC82
.L216:
	ldr	x1, .LC80
	mov	w0, 0
	add	x2, x2, 2112
	bl	dprint_vfmw
	mov	w0, -1
	b	.L148
.L220:
	ldr	x2, .LC73
	ldr	x3, .LC79
	b	.L216
.L223:
	ldr	x2, .LC73
	mov	w0, 0
	ldr	x1, .LC86
	add	x2, x2, 2112
	bl	dprint_vfmw
	mov	w0, -1
	b	.L148
.L158:
	ldr	x2, .LC73
	mov	w0, 1
	ldr	x1, .LC88
	add	x2, x2, 2112
	bl	dprint_vfmw
	mov	w0, -1
	b	.L148
.L155:
	ldr	x2, .LC73
	mov	w0, 0
	ldr	x1, .LC84
	add	x2, x2, 2112
	bl	dprint_vfmw
	mov	w0, -1
	b	.L148
	.size	HEVCHAL_V400R003_StartDec, .-HEVCHAL_V400R003_StartDec
	.align	3
.LC72:
	.xword	g_HwMem
	.align	3
.LC73:
	.xword	.LANCHOR0
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
.LC80:
	.xword	.LC6
	.align	3
.LC82:
	.xword	.LC81
	.align	3
.LC84:
	.xword	.LC83
	.align	3
.LC86:
	.xword	.LC85
	.align	3
.LC88:
	.xword	.LC87
	.align	3
.LC90:
	.xword	.LC89
	.align	3
.LC92:
	.xword	.LC91
	.align	3
.LC94:
	.xword	.LC93
	.align	3
.LC96:
	.xword	.LC95
	.align	3
.LC98:
	.xword	.LC97
	.align	3
.LC100:
	.xword	.LC99
	.align	3
.LC102:
	.xword	.LC101
	.align	3
.LC104:
	.xword	.LC103
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
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
	.type	__func__.11955, %object
	.size	__func__.11955, 18
__func__.11955:
	.string	"HEVCWriteCabacTab"
	.zero	6
	.type	__func__.11976, %object
	.size	__func__.11976, 31
__func__.11976:
	.string	"HEVCHAL_V400R003_CutSliceChain"
	.zero	1
	.type	__func__.11995, %object
	.size	__func__.11995, 19
__func__.11995:
	.string	"HEVC_WriteTileInfo"
	.zero	5
	.type	__func__.12021, %object
	.size	__func__.12021, 27
__func__.12021:
	.string	"HEVCHAL_V400R003_SetPicMsg"
	.zero	5
	.type	__func__.12058, %object
	.size	__func__.12058, 29
__func__.12058:
	.string	"HEVCHAL_V400R003_SetSliceMsg"
	.zero	3
	.type	s_SliceTypeForPMV, %object
	.size	s_SliceTypeForPMV, 12
s_SliceTypeForPMV:
	.word	2
	.word	1
	.word	0
	.zero	4
	.type	__func__.12092, %object
	.size	__func__.12092, 27
__func__.12092:
	.string	"HEVCHAL_V400R003_CfgVdmReg"
	.zero	5
	.type	__func__.12120, %object
	.size	__func__.12120, 26
__func__.12120:
	.string	"HEVCHAL_V400R003_StartDec"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC4:
	ASCII(.string	"can not map mn virtual address!\n" )
	.zero	7
.LC6:
	ASCII(.string	"%s: %s\n" )
.LC9:
	ASCII(.string	"HEVCWriteCabacTab return error.\n" )
	.zero	7
.LC11:
	ASCII(.string	"can not map slice msg virtual address!\n" )
.LC17:
	ASCII(.string	"qmatrix[%d] = 0x%x\n" )
	.zero	4
.LC19:
	ASCII(.string	"picmsg Dxx addr 0x%x = 0x%x\n" )
	.zero	3
.LC21:
	ASCII(.string	"can not map slice segment info virtual address!\n" )
	.zero	7
.LC25:
	ASCII(.string	"TileId: 0x%x\n" )
	.zero	2
.LC27:
	ASCII(.string	"can not map down msg virtual address!\n" )
	.zero	1
.LC32:
	ASCII(.string	"pic msg burst0:\n" )
	.zero	7
.LC34:
	ASCII(.string	"picmsg D[0] addr 0x%x = 0x%x\n" )
	.zero	2
.LC36:
	ASCII(.string	"HEVC_WriteTileInfo return error.\n" )
	.zero	6
.LC38:
	ASCII(.string	"HEVCHAL_V400R003_SetSliceMsg can not map slice msg virtual address!\n" )
	.zero	3
.LC43:
	ASCII(.string	"%s FATAL: i=0, valid_bitlen=0\n" )
	.zero	1
.LC45:
	ASCII(.string	"slicemsg D[%d] addr 0x%x = 0x%x\n" )
	.zero	7
.LC52:
	ASCII(.string	"%s: pMfdeTask(%p) = NULL\n" )
	.zero	6
.LC54:
	ASCII(.string	"HEVC_BASIC_CFG0 = 0x%x\n" )
.LC56:
	ASCII(.string	"HEVC_BASIC_CFG1 = 0x%x\n" )
.LC58:
	ASCII(.string	"AVM_ADDR = 0x%x\n" )
	.zero	7
.LC60:
	ASCII(.string	"VREG_V200R003_PART_DEC_OVER_INT_LEVEL=0x%x\n" )
	.zero	4
.LC62:
	ASCII(.string	"VREG_LINE_NUM_STADDR = 0x%x\n" )
	.zero	3
.LC64:
	ASCII(.string	"HEVC_VFMW_YSTADDR_1D = 0x%x\n" )
	.zero	3
.LC66:
	ASCII(.string	"HEVC_VFMW_UVOFFSET_1D = 0x%x\n" )
	.zero	2
.LC68:
	ASCII(.string	"SCD REG_EMAR_ID = 0x%x\n" )
.LC70:
	ASCII(.string	"VREG_FF_APT_EN = 0x%x\n" )
	.zero	1
.LC74:
	ASCII(.string	"%s VdhId %d >= %d\n" )
	.zero	5
.LC76:
	ASCII(.string	"VdhId is wrong! HEVC4HAL_V200R003_StartDec\n" )
	.zero	4
.LC78:
	ASCII(.string	"pParam is null\n" )
.LC81:
	ASCII(.string	"FATAL: ApcSize > 16\n" )
	.zero	3
.LC83:
	ASCII(.string	"%s vdm register virtual address not mapped, reset failed!\n" )
	.zero	5
.LC85:
	ASCII(.string	"%s pFirstSlice = NULL!\n" )
.LC87:
	ASCII(.string	"%s : pTargetSlicePara == NULL\n" )
	.zero	1
.LC89:
	ASCII(.string	"%s 0: set slice msg failed!\n" )
	.zero	3
.LC91:
	ASCII(.string	"%s: i = %d, pSlicePara = NULL!\n" )
.LC93:
	ASCII(.string	"SliceSegmentTileAddress %d < pre_end_ctb_in_slice_tile %d\n" )
	.zero	5
.LC95:
	ASCII(.string	"%s pTargetSlicePara is NULL!\n" )
	.zero	2
.LC97:
	ASCII(.string	"end_ctb_in_slice_raster(%d)/end_ctb_in_slice_tile(%d) > pic_max_ctb(%d)\n" )
	.zero	7
.LC99:
	ASCII(.string	"SliceSegmentTileAddress %d > end_ctb_in_slice_tile %d\n" )
	.zero	1
.LC101:
	ASCII(.string	"%s 1: set slice msg failed!\n" )
	.zero	3
.LC103:
	ASCII(.string	"No slice to dec, add up msg report.\n" )
	.bss
	.align	3
.LANCHOR1 = . + 0
	.type	s_CabacMN, %object
	.size	s_CabacMN, 928
s_CabacMN:
	.zero	928
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
