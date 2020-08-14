#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vdm_hal.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_GetHalMemSize
	.type	VDMHAL_V400R003_GetHalMemSize, %function
VDMHAL_V400R003_GetHalMemSize:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 6291456
	ldp	x29, x30, [sp], 16
	ret
	.size	VDMHAL_V400R003_GetHalMemSize, .-VDMHAL_V400R003_GetHalMemSize
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_OpenHAL
	.type	VDMHAL_V400R003_OpenHAL, %function
VDMHAL_V400R003_OpenHAL:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L19
	ldp	w20, w21, [x19]
	ldr	w0, [x19, 8]
	cbz	w20, .L20
	mov	w1, 6291455
	cmp	w21, w1
	ble	.L21
	cmp	w0, wzr
	bgt	.L22
	bne	.L16
	ldr	x23, .LC15
	mov	x2, 140
	ldr	x22, .LC16
	mov	w1, 0
	ldr	x3, [x23, 96]
	mov	x0, x22
	blr	x3
	ldr	x19, .LC1
	ldr	x3, [x23, 96]
	mov	w4, 4
	mov	x2, 1256
	str	w4, [x22]
	mov	w1, 0
	mov	x0, x19
	blr	x3
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	mov	x3, x0
	ldr	x1, .LC18
	mov	x2, x0
	str	x3, [x19]
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 53248
	movk	w0, 0xf8c3, lsl 16
	bl	MEM_Phy2Vir
	mov	x3, x0
	ldr	x1, .LC20
	mov	x2, x0
	str	x3, [x19, 16]
	mov	w0, 22
	bl	dprint_vfmw
	add	w4, w20, 1023
	add	w0, w20, w21
	and	w4, w4, -1024
	mov	w1, 1024
	sub	w0, w0, w4
	str	w1, [x19, 40]
	stp	w4, w0, [x19, 32]
	add	x2, x19, 896
	add	x1, x19, 56
	mov	w0, w4
	.p2align 2
.L10:
	str	w0, [x1], 4
	add	w0, w0, 1280
	cmp	x1, x2
	bne	.L10
	add	w0, w4, 266240
	mov	w1, 210
	add	w0, w0, 3072
	str	w1, [x19, 1080]
	add	w1, w0, 4096
	str	w0, [x19, 1104]
	add	w1, w1, 1024
	add	x2, x19, 1024
	add	w0, w1, 32768
	str	wzr, [x19, 1156]
	add	w0, w0, 1023
	str	wzr, [x19, 1184]
	and	w0, w0, -32768
	add	w5, w0, 589824
	add	w4, w0, 1773568
	stp	w0, w0, [x2, 84]
	stp	w0, w5, [x2, 92]
	str	w1, [x19, 1124]
	add	w1, w0, 4427776
	add	w4, w4, 1152
	add	w1, w1, 1024
	cmp	w1, w4
	add	w8, w0, 4370432
	csel	w1, w1, w4, cs
	str	w4, [x19, 1152]
	add	w4, w0, 45056
	sub	w2, w1, w20
	str	w4, [x19, 1160]
	add	w1, w8, 2048
	add	w4, w0, 1093632
	add	w6, w0, 4423680
	str	w1, [x19, 1192]
	add	w1, w0, 3190784
	str	w4, [x19, 1164]
	add	w4, w0, 2142208
	add	w6, w6, 1024
	add	w7, w0, 4374528
	add	w3, w0, 4390912
	str	w1, [x19, 1176]
	str	w4, [x19, 1168]
	add	w1, w0, 1769472
	add	w4, w0, 1179648
	add	w3, w3, 1024
	add	w1, w1, 2048
	add	w4, w4, 2048
	add	w7, w7, 1024
	str	w6, [x19, 1200]
	add	w8, w0, 4239360
	add	w6, w5, 2048
	str	w5, [x19, 1128]
	cmp	w21, w2
	str	w0, [x19, 1172]
	str	w7, [x19, 1196]
	str	w3, [x19, 1204]
	str	w3, [x19, 1220]
	str	w6, [x19, 1132]
	str	w8, [x19, 1208]
	str	w1, [x19, 1144]
	str	w1, [x19, 1148]
	str	w4, [x19, 1136]
	str	w4, [x19, 1140]
	bcc	.L11
	mov	x0, x19
	mov	w20, -1
	bl	H264HAL_V400R003_InitHal
	mov	w21, w0
	mov	x0, x19
	bl	HEVCHAL_V400R003_InitHal
	cbz	w0, .L23
.L12:
	mov	x0, x19
	bl	H264HAL_V400R003_InitHal
	cmp	w0, wzr
	csinv	w0, w20, wzr, eq
.L4:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L23:
	cmp	w21, wzr
	csetm	w20, ne
	b	.L12
.L22:
	ldr	x1, .LC14
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4
.L16:
	ldr	x1, .LC23
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4
.L11:
	ldr	x1, .LC25
	mov	w3, w21
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4
.L20:
	ldr	x3, .LC10
.L17:
	ldr	x2, .LC6
	mov	w0, 0
	ldr	x1, .LC8
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4
.L21:
	ldr	x3, .LC12
	b	.L17
.L19:
	ldr	x3, .LC5
	b	.L17
	.size	VDMHAL_V400R003_OpenHAL, .-VDMHAL_V400R003_OpenHAL
	.align	3
.LC1:
	.xword	g_HwMem
	.align	3
.LC5:
	.xword	.LC4
	.align	3
.LC6:
	.xword	.LANCHOR0
	.align	3
.LC8:
	.xword	.LC7
	.align	3
.LC10:
	.xword	.LC9
	.align	3
.LC12:
	.xword	.LC11
	.align	3
.LC14:
	.xword	.LC13
	.align	3
.LC15:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC16:
	.xword	g_VdmExtParam
	.align	3
.LC18:
	.xword	.LC17
	.align	3
.LC20:
	.xword	.LC19
	.align	3
.LC23:
	.xword	.LC22
	.align	3
.LC25:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_CloseHAL
	.type	VDMHAL_V400R003_CloseHAL, %function
VDMHAL_V400R003_CloseHAL:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	VDMHAL_V400R003_CloseHAL, .-VDMHAL_V400R003_CloseHAL
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_CalcFsSize
	.type	VDMHAL_V400R003_CalcFsSize, %function
VDMHAL_V400R003_CalcFsSize:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	w24, w0
	mov	x0, x30
	mov	w20, w1
	mov	w21, w2
	mov	w22, w3
	mov	w27, w4
	mov	x26, x5
	bl	_mcount
	sub	w1, w20, #32
	mov	w0, 8160
	cmp	w1, w0
	bhi	.L26
	sub	w1, w21, #32
	mov	w6, w21
	cmp	w1, w0
	bhi	.L26
	ldr	x0, .LC30
	add	w19, w20, 15
	asr	w19, w19, 4
	add	w3, w21, 15
	asr	w3, w3, 4
	cmp	w19, 45
	ldrb	w2, [x0]
	mov	w0, 36
	ccmp	w3, w0, 0, le
	mov	w1, 64
	cset	w0, le
	lsl	w28, w19, 4
	cmp	w0, wzr
	mov	w0, 32
	csel	w0, w0, w1, eq
	cmp	w2, 1
	csel	w0, w0, w1, ne
	cmp	w27, 16
	beq	.L31
	cmp	w27, 17
	beq	.L54
	mul	w19, w0, w19
	ldr	x4, .LC32
	add	w28, w28, 255
	add	w6, w21, 31
	and	w28, w28, -256
	mul	w19, w19, w3
	ldr	x0, [x4, w24, sxtw 3]
	and	w6, w6, -32
	add	w19, w19, 143
	mov	w7, 0
	and	w19, w19, -128
	mov	w8, 0
	asr	w9, w19, 1
.L43:
	ldr	w1, [x0, 1272]
	cmp	w1, 1
	beq	.L55
.L47:
	mov	w10, 0
	mov	w5, 0
	mov	w23, 0
.L34:
	cmp	w27, wzr
	sub	w1, w27, #16
	ccmp	w27, 3, 4, ne
	ccmp	w1, 1, 0, ne
	bls	.L56
	cmp	w22, 1
	mov	w21, 1024
	mov	w20, 0
	mov	w25, 0
	mul	w6, w28, w6
	beq	.L57
.L36:
	mul	w2, w8, w7
	lsl	w3, w23, 2
	lsl	w1, w2, 2
	sub	w3, w3, w23
	sub	w2, w1, w2
	lsl	w1, w6, 2
	sub	w1, w1, w6
	lsr	w1, w1, 1
	add	w1, w1, w2, lsr 1
	add	w22, w1, w3, lsr 1
	add	x1, x0, 290816
	ldr	w1, [x1, 2456]
	cbz	w1, .L58
.L38:
	add	x2, x0, 294912
	cmp	w27, 17
	str	w25, [x2, 1728]
	str	w20, [x2, 1732]
	str	w5, [x2, 1740]
	str	w10, [x2, 1744]
	str	w9, [x2, 1940]
	str	w22, [x2, 1968]
	beq	.L39
	add	x1, x0, 290816
	mov	w3, 20
	lsl	w28, w28, 4
	ldr	w1, [x1, 2464]
	str	w28, [x2, 696]
	cmp	w1, 20
	csel	w1, w1, w3, le
	cmp	w27, 16
	str	w1, [x2, 1944]
	add	w1, w23, w6
	str	w1, [x2, 1764]
	beq	.L59
.L41:
	ldr	x0, [x4, w24, sxtw 3]
	mov	w5, 1024
	add	w3, w22, w21
	add	x2, x0, 294912
	add	w3, w3, 32
	add	w1, w3, w19
	str	w19, [x2, 1972]
	str	w5, [x2, 1980]
	ldrsb	w0, [x0, 168]
	cmp	w0, 1
	add	w0, w1, w5
	csel	w1, w0, w1, eq
	cbz	x26, .L49
	str	w3, [x26, 4]
	mov	w0, 0
	ldr	x2, [x4, w24, sxtw 3]
	add	x2, x2, 290816
	ldr	w2, [x2, 2464]
	stp	w2, w19, [x26, 8]
	ldr	x2, [x4, w24, sxtw 3]
	add	x2, x2, 294912
	ldr	w2, [x2, 1944]
	stp	w2, w5, [x26, 16]
	str	w1, [x26]
.L50:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L56:
	adds	w25, w20, 255
	add	w20, w20, 510
	csel	w25, w20, w25, mi
	adds	w2, w21, 63
	add	w21, w21, 126
	asr	w25, w25, 8
	csel	w21, w21, w2, mi
	lsl	w25, w25, 6
	and	w21, w21, -64
	cmp	w21, wzr
	add	w20, w21, 7
	csel	w20, w20, w21, lt
	cmp	w22, 1
	asr	w20, w20, 3
	mul	w6, w28, w6
	mul	w20, w20, w25
	add	w21, w20, 1024
	bne	.L36
.L57:
	add	x1, x0, 290816
	lsl	w22, w3, 6
	sub	w3, w22, w3, lsl 4
	ldr	w1, [x1, 2456]
	mul	w22, w3, w28
	cbnz	w1, .L38
.L58:
	ldr	x1, .LC31
	add	x0, x0, 294912
	add	x0, x0, 696
	mov	x2, 1288
	str	x4, [x29, 104]
	ldr	x3, [x1, 96]
	mov	w1, 0
	stp	w10, w6, [x29, 116]
	stp	w5, w7, [x29, 124]
	stp	w8, w9, [x29, 132]
	blr	x3
	ldr	x4, [x29, 104]
	ldp	w10, w6, [x29, 116]
	ldp	w5, w7, [x29, 124]
	ldp	w8, w9, [x29, 132]
	ldr	x0, [x4, w24, sxtw 3]
	b	.L38
	.p2align 3
.L31:
	ldr	x4, .LC32
	mul	w19, w3, w28
	add	w28, w20, 255
	ldr	x0, [x4, w24, sxtw 3]
	add	w19, w19, 143
	and	w28, w28, -256
	and	w19, w19, -128
	ldr	w1, [x0, 1564]
	cmp	w1, 8
	ble	.L60
	ldr	w1, [x0, 1272]
	add	w7, w21, 31
	and	w7, w7, -32
	lsr	w8, w28, 2
	cmp	w1, 1
	asr	w9, w19, 1
	bne	.L47
	.p2align 2
.L55:
	adds	w2, w20, 511
	add	w1, w20, 1022
	csel	w1, w1, w2, mi
	add	w23, w21, 126
	and	w1, w1, -512
	cmp	w1, wzr
	add	w5, w1, 63
	csel	w5, w5, w1, lt
	adds	w1, w21, 63
	csel	w23, w23, w1, mi
	asr	w5, w5, 6
	asr	w23, w23, 6
	lsl	w5, w5, 1
	lsl	w23, w23, 5
	mul	w23, w23, w5
	mov	w10, w23
	b	.L34
	.p2align 3
.L59:
	ldr	w0, [x0, 1564]
	cmp	w0, 8
	ble	.L41
	lsl	w0, w6, 2
	ldr	x1, .LC34
	sub	w6, w0, w6
	lsl	w0, w23, 2
	sub	w0, w0, w23
	lsr	w23, w23, 1
	lsr	w0, w0, 1
	str	x4, [x29, 136]
	madd	w7, w8, w7, w23
	add	w6, w0, w6, lsr 1
	lsl	w8, w8, 5
	mov	w0, 31
	str	w8, [x2, 1748]
	str	w6, [x2, 1756]
	str	w7, [x2, 1760]
	bl	dprint_vfmw
	ldr	x4, [x29, 136]
	b	.L41
	.p2align 3
.L39:
	add	w6, w23, w6
	lsl	w28, w28, 4
	mov	w0, 1
	str	w28, [x2, 696]
	str	w6, [x2, 1764]
	str	w0, [x2, 1944]
	b	.L41
	.p2align 3
.L54:
	ldr	x4, .LC32
	add	w28, w20, 255
	add	w6, w21, 63
	mov	w9, 64
	mov	w19, 128
	and	w28, w28, -256
	and	w6, w6, -64
	movk	w9, 0x12, lsl 16
	ldr	x0, [x4, w24, sxtw 3]
	movk	w19, 0x24, lsl 16
	mov	w7, 0
	mov	w8, 0
	b	.L43
	.p2align 3
.L60:
	asr	w9, w19, 1
	mov	w7, 0
	mov	w8, 0
	b	.L43
	.p2align 3
.L49:
	mov	w0, 0
	b	.L50
	.p2align 3
.L26:
	ldr	x2, .LC28
	mov	w0, 0
	ldr	x3, .LC27
	ldr	x1, .LC29
	add	x2, x2, 24
	bl	dprint_vfmw
	mov	w0, -1
	b	.L50
	.size	VDMHAL_V400R003_CalcFsSize, .-VDMHAL_V400R003_CalcFsSize
	.align	3
.LC27:
	.xword	.LC26
	.align	3
.LC28:
	.xword	.LANCHOR0
	.align	3
.LC29:
	.xword	.LC7
	.align	3
.LC30:
	.xword	g_not_direct_8x8_inference_flag
	.align	3
.LC31:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC32:
	.xword	s_pstVfmwChan
	.align	3
.LC34:
	.xword	.LC33
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_GetRpuSize
	.type	VDMHAL_V400R003_GetRpuSize, %function
VDMHAL_V400R003_GetRpuSize:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 1024
	ldp	x29, x30, [sp], 16
	ret
	.size	VDMHAL_V400R003_GetRpuSize, .-VDMHAL_V400R003_GetRpuSize
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_ArrangeMem
	.type	VDMHAL_V400R003_ArrangeMem, %function
VDMHAL_V400R003_ArrangeMem:
	stp	x29, x30, [sp, -192]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	w21, w0
	mov	x0, x30
	mov	w20, w7
	mov	w23, w2
	mov	w28, w3
	mov	w22, w4
	mov	w26, w5
	mov	w25, w6
	ldr	x19, [x29, 192]
	str	w1, [x29, 124]
	bl	_mcount
	add	x0, x29, 136
	cmn	w20, #2
	str	xzr, [x29, 152]
	stp	xzr, xzr, [x0]
	beq	.L131
	tbnz	w20, #31, .L132
	mov	w0, w20
	bl	VCTRL_GetVidStd
	ldr	x24, .LC35
	mov	w27, w0
	ldr	x0, [x24, w20, sxtw 3]
	ldr	w1, [x0, 1512]
	cmp	w1, 1
	beq	.L180
.L64:
	cbz	w21, .L181
	sub	w1, w23, #32
	mov	w0, 8160
	cmp	w1, w0
	bhi	.L89
	sub	w1, w28, #32
	cmp	w1, w0
	bhi	.L89
	cbz	x19, .L182
	ldr	x0, .LC47
	cmp	w22, 20
	mov	x2, 1272
	mov	w1, 0
	mov	w24, 20
	ldr	x3, [x0, 96]
	mov	x0, x19
	csel	w24, w22, w24, le
	add	w22, w23, 15
	asr	w22, w22, 4
	blr	x3
	ldr	x0, .LC62
	add	w3, w28, 15
	cmp	w22, 45
	asr	w3, w3, 4
	mov	w1, 64
	ldrb	w2, [x0]
	mov	w0, 36
	ccmp	w3, w0, 0, le
	cset	w0, le
	cmp	w0, wzr
	mov	w0, 32
	csel	w0, w0, w1, eq
	cmp	w2, 1
	csel	w0, w0, w1, ne
	cmp	w27, 16
	lsl	w2, w22, 4
	beq	.L183
	cmp	w27, 17
	beq	.L97
	mul	w22, w0, w22
	ldr	w0, [x29, 124]
	mul	w22, w22, w3
	add	w22, w22, 127
	and	w22, w22, -128
	mul	w5, w24, w22
	cmp	w0, w5
	ble	.L95
	add	w2, w2, 255
	add	w8, w28, 31
	asr	w0, w22, 1
	str	w24, [x19, 1248]
	and	w2, w2, -256
	and	w8, w8, -32
	add	w1, w28, 63
	mov	w7, 0
	mov	w6, 0
	str	w0, [x19, 1244]
	add	w24, w23, 255
.L99:
	adds	w0, w23, 511
	add	w23, w23, 1022
	csel	w0, w23, w0, mi
	and	w0, w0, -512
	cmp	w0, wzr
	add	w9, w0, 63
	csel	w0, w9, w0, lt
	cmp	w1, wzr
	add	w9, w1, 63
	asr	w0, w0, 6
	csel	w28, w9, w1, lt
	cmp	w24, wzr
	add	w1, w24, 255
	asr	w9, w28, 6
	csel	w24, w1, w24, lt
	lsl	w28, w0, 6
	asr	w24, w24, 8
	lsl	w0, w0, 1
	lsl	w1, w24, 6
	lsl	w24, w9, 6
	cmp	w24, wzr
	add	w10, w24, 7
	csel	w24, w10, w24, lt
	str	w0, [x19, 1044]
	asr	w24, w24, 3
	lsl	w0, w2, 4
	mul	w28, w28, w9
	mul	w24, w24, w1
	cmp	w25, 1
	str	w1, [x19, 1032]
	str	w24, [x19, 1036]
	str	w28, [x19, 1048]
	str	w0, [x19]
	beq	.L184
	mul	w1, w2, w8
	mul	w3, w6, w7
	lsl	w0, w1, 2
	lsl	w23, w28, 2
	sub	w1, w0, w1
	lsl	w0, w3, 2
	sub	w0, w0, w3
	sub	w23, w23, w28
	asr	w0, w0, 1
	add	w0, w0, w1, asr 1
	add	w23, w0, w23, lsr 1
.L102:
	stp	w7, w6, [x29, 116]
	tbnz	w20, #31, .L103
	mov	w0, w20
	str	w2, [x29, 108]
	stp	w5, w8, [x29, 100]
	bl	VCTRL_GetChanWidth
	mov	w1, w0
	mov	w0, w20
	str	w1, [x29, 112]
	bl	VCTRL_GetChanHeight
	ldp	w2, w1, [x29, 108]
	ldp	w5, w8, [x29, 100]
	cmn	w1, #1
	ccmn	w0, #1, 4, ne
	ldp	w7, w6, [x29, 116]
	beq	.L185
	add	w1, w1, 15
	cmp	w25, 1
	and	w1, w1, -16
	add	w0, w0, 15
	add	w9, w1, 255
	and	w0, w0, -16
	and	w9, w9, -256
	beq	.L186
	adds	w3, w0, 63
	add	w0, w0, 126
	csel	w0, w0, w3, mi
	adds	w3, w1, 511
	add	w1, w1, 1022
	asr	w0, w0, 6
	csel	w1, w1, w3, mi
	and	w1, w1, -512
	add	w3, w1, 63
	cmp	w1, wzr
	csel	w1, w3, w1, lt
	asr	w1, w1, 6
	lsl	w3, w1, 1
	lsl	w1, w0, 2
	mul	w9, w9, w0
	sub	w0, w1, w0
	lsl	w0, w0, 5
	mul	w1, w3, w0
	lsl	w0, w9, 2
	sub	w0, w0, w9
	lsl	w0, w0, 6
	asr	w0, w0, 1
	add	w0, w0, w1, lsr 1
.L106:
	cmp	w23, w0
	bgt	.L187
	mul	w2, w2, w8
	cmp	w27, 16
	str	w2, [x29, 120]
	add	w0, w2, w28
	str	w0, [x19, 1068]
	stp	w7, w6, [x29, 112]
	mov	w25, 1
	bne	.L108
	ldr	x0, .LC35
	str	w5, [x29, 108]
	ldr	x0, [x0, w20, sxtw 3]
	add	x0, x0, 294912
	add	x0, x0, 2584
	bl	IsMain10Profile
	ldp	w5, w7, [x29, 108]
	ldp	w6, w2, [x29, 116]
	cbnz	w0, .L188
	.p2align 2
.L108:
	cbnz	w26, .L109
	str	wzr, [x19, 1040]
	str	wzr, [x19, 1080]
.L110:
	ldr	x2, .LC38
	ldr	x3, .LC71
.L177:
	ldr	x1, .LC39
	add	x2, x2, 56
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, w1
	ldp	x29, x30, [sp], 192
	ret
	.p2align 3
.L132:
	mov	w27, 21
	b	.L64
	.p2align 3
.L180:
	cmp	w27, 20
	bhi	.L189
	add	x0, x0, 290816
	add	x5, x29, 136
	mov	w4, w27
	mov	w3, w25
	mov	w2, w28
	mov	w1, w23
	ldr	w21, [x0, 2460]
	mov	w0, w20
	bl	VDMHAL_V400R003_CalcFsSize
	cbnz	w0, .L190
	ldr	x25, [x24, w20, sxtw 3]
	ldr	w22, [x29, 136]
	add	x1, x25, 290816
	cmp	w21, w22
	ldr	w5, [x1, 2480]
	ldr	w21, [x1, 2464]
	beq	.L191
.L69:
	ldr	x2, .LC38
	mov	w4, w21
	ldr	x1, .LC43
	mov	w3, w22
	add	x2, x2, 88
	mov	w0, 31
	bl	dprint_vfmw
	ldr	x1, [x24, w20, sxtw 3]
	add	x1, x1, 290816
	ldr	w0, [x1, 2456]
	str	w22, [x1, 2460]
	cbz	w0, .L192
	ldr	x2, .LC46
	ldr	w0, [x29, 144]
	str	w0, [x1, 2468]
	ldr	x4, [x2]
	cbz	x4, .L174
	ldr	w1, [x29, 140]
	mov	w3, 28
	ldr	w2, [x29, 152]
	stp	w0, w1, [x29, 160]
	ldr	w1, [x29, 148]
	ldr	w0, [x29, 156]
	stp	w1, w0, [x29, 172]
	str	w2, [x29, 168]
	mov	w1, 122
	stp	w23, w28, [x29, 180]
	add	x2, x29, 160
	mov	w0, w20
	blr	x4
.L174:
	ldr	x23, .LC47
.L78:
	ldr	x4, [x24, w20, sxtw 3]
	mov	w3, w21
	ldr	x1, .LC50
	mov	w2, w22
	add	x4, x4, 290816
	mov	w0, 31
	ldr	w4, [x4, 2480]
	bl	dprint_vfmw
	ldr	x19, [x24, w20, sxtw 3]
	ldr	x0, [x23]
	add	x19, x19, 294912
	blr	x0
	str	w0, [x19, 1984]
	mov	w1, 2
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 192
	ret
	.p2align 3
.L109:
	ldr	w0, [x29, 124]
	cmp	w27, wzr
	ccmp	w27, 3, 4, ne
	sub	w4, w27, #16
	sub	w5, w0, w5
	add	w0, w21, 1023
	and	w0, w0, -1024
	ccmp	w4, 1, 0, ne
	sub	w6, w0, w21
	sub	w5, w5, w6
	bls	.L111
	sdiv	w5, w5, w23
	cmp	w26, 32
	mov	w4, 32
	str	wzr, [x19, 1040]
	csel	w4, w26, w4, le
	cmp	w5, w4
	csel	w4, w5, w4, le
	str	w4, [x19, 1080]
.L112:
	cmp	w4, wzr
	beq	.L110
	ble	.L193
	sub	w1, w4, #1
	mov	w2, 12
	mov	x3, 20
	ldr	w5, [x19, 1252]
	umaddl	x3, w1, w2, x3
	uxtw	x2, w1
	add	x1, x19, 8
	add	x3, x19, x3
	.p2align 2
.L115:
	str	w0, [x1], 12
	add	w0, w0, w23
	cmp	x3, x1
	bne	.L115
	madd	w23, w23, w4, w6
	add	x2, x2, 99
	add	w0, w4, w5
	add	w1, w23, w21
	str	w0, [x19, 1252]
	add	x2, x19, x2, lsl 2
	add	x0, x19, 392
	.p2align 2
.L117:
	str	w1, [x0], 4
	add	w1, w1, 32
	cmp	x2, x0
	bne	.L117
.L127:
	ldr	w3, [x19, 1040]
	add	w23, w23, w4, lsl 5
	add	w1, w21, w23
	mov	w0, 0
	cbz	w3, .L119
	.p2align 2
.L152:
	add	x2, x19, x0, sxtw 2
	add	w0, w0, 1
	cmp	w0, w3
	str	w1, [x2, 904]
	add	w1, w1, w24
	bne	.L152
	mul	w3, w3, w24
.L119:
	ldr	w5, [x19, 1248]
	add	w23, w23, w3
	add	w21, w21, w23
	cmp	w5, wzr
	ble	.L125
	sub	w0, w5, #1
	mov	x1, 1092
	ldr	w3, [x19, 1256]
	add	x2, x19, 1084
	add	x0, x1, x0, uxtw 3
	mov	w1, w21
	add	x0, x19, x0
	.p2align 2
.L124:
	str	w1, [x2], 8
	add	w1, w1, w22
	cmp	x0, x2
	bne	.L124
	add	w0, w5, w3
	str	w0, [x19, 1256]
.L125:
	madd	w23, w22, w5, w23
	str	w23, [x19, 4]
	cbz	w25, .L123
	ldr	x0, .LC35
	ldr	x0, [x0, w20, sxtw 3]
	cbz	x0, .L123
	cmp	w4, wzr
	ble	.L123
	sub	w2, w4, #1
	mov	w3, 12
	mov	x1, 532
	add	w21, w23, w21
	add	x0, x19, 520
	umaddl	x1, w2, w3, x1
	add	x1, x19, x1
	.p2align 2
.L126:
	str	w21, [x0], 12
	add	w21, w21, 1024
	cmp	x1, x0
	bne	.L126
.L123:
	add	w23, w23, w4, lsl 10
	mov	w1, 0
	str	w23, [x19, 4]
.L167:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, w1
	ldp	x29, x30, [sp], 192
	ret
	.p2align 3
.L103:
	madd	w2, w2, w8, w28
	mov	w25, 0
	str	w2, [x19, 1068]
	b	.L108
	.p2align 3
.L131:
	mov	w27, 16
	b	.L64
	.p2align 3
.L111:
	add	w4, w23, w24
	cmp	w26, 32
	sdiv	w5, w5, w4
	mov	w4, 32
	csel	w4, w26, w4, le
	cmp	w5, w4
	csel	w4, w5, w4, le
	str	w4, [x19, 1040]
	str	w4, [x19, 1080]
	b	.L112
	.p2align 3
.L183:
	mul	w22, w3, w2
	ldr	w0, [x29, 124]
	add	w22, w22, 127
	and	w22, w22, -128
	mul	w5, w24, w22
	cmp	w0, w5
	ble	.L95
	asr	w0, w22, 1
	str	w24, [x19, 1248]
	str	w0, [x19, 1244]
	add	w24, w23, 255
	and	w2, w24, -256
	tbnz	w20, #31, .L176
	ldr	x0, .LC35
	stp	w5, w3, [x29, 112]
	str	w2, [x29, 120]
	ldr	x0, [x0, w20, sxtw 3]
	add	x0, x0, 294912
	add	x0, x0, 2584
	bl	IsMain10Profile
	ldp	w5, w3, [x29, 112]
	ldr	w2, [x29, 120]
	cbz	w0, .L176
	cmp	w2, wzr
	add	w6, w2, 3
	csel	w6, w6, w2, lt
	add	w7, w28, 31
	asr	w6, w6, 2
	and	w7, w7, -32
	add	w1, w28, 63
	mov	w8, w28
	b	.L99
	.p2align 3
.L184:
	lsl	w0, w3, 4
	mul	w0, w0, w2
	lsl	w23, w0, 2
	sub	w23, w23, w0
	b	.L102
	.p2align 3
.L97:
	lsl	w5, w24, 21
	ldr	w0, [x29, 124]
	add	w5, w5, w24, lsl 18
	cmp	w0, w5
	ble	.L95
	add	w1, w28, 63
	str	w24, [x19, 1248]
	add	w24, w23, 255
	mov	w0, 1179648
	and	w8, w1, -64
	mov	w22, 2359296
	mov	w7, 0
	mov	w6, 0
	str	w0, [x19, 1244]
	and	w2, w24, -256
	b	.L99
	.p2align 3
.L176:
	add	w1, w28, 63
	mov	w8, w28
	mov	w7, 0
	mov	w6, 0
	b	.L99
	.p2align 3
.L186:
	mul	w0, w0, w9
	lsl	w1, w0, 2
	sub	w0, w1, w0
	b	.L106
.L193:
	madd	w23, w23, w4, w6
	b	.L127
.L191:
	cmp	w5, 1
	beq	.L69
	ldr	w1, [x1, 2456]
	cmp	w1, 1
	beq	.L69
	cbz	x19, .L194
	add	x27, x25, 294912
	mov	x0, x25
	add	x22, x27, 696
	ldr	w26, [x27, 1968]
	cbz	w21, .L195
.L80:
	add	x0, x0, 290816
	ldr	w2, [x0, 2468]
	cmp	w2, 30
	bgt	.L196
.L81:
	ldr	x23, .LC47
	add	x21, x27, 696
	mov	w0, 8
	str	w2, [x21, 1080]
	ldr	x1, [x23, 24]
	blr	x1
	ldr	x8, [x24, w20, sxtw 3]
	add	x10, x8, 294912
	ldr	w0, [x10, 2008]
	cbz	w0, .L86
	ldr	w12, [x21, 1036]
	mov	x5, x21
	ldr	w11, [x21, 1248]
	mov	w6, 0
	ldr	w7, [x21, 1252]
	mov	w9, 1
	.p2align 2
.L85:
	uxtw	x1, w6
	uxtw	x0, w7
	lsl	x2, x1, 6
	lsl	x3, x0, 2
	add	x2, x2, x1, lsl 3
	sub	x0, x3, x0
	add	x2, x8, x2
	add	x3, x25, x3
	add	x2, x2, 290816
	add	x0, x25, x0, lsl 2
	add	x0, x0, 294912
	add	x4, x3, 294912
	add	w7, w7, 1
	add	w6, w6, 1
	ldr	w13, [x2, 2520]
	mov	x1, x2
	ldr	w14, [x2, 2500]
	add	w3, w13, 1023
	strb	w9, [x0, 712]
	str	w14, [x0, 708]
	and	w3, w3, -1024
	str	w3, [x0, 704]
	sub	w0, w26, w13
	add	w0, w0, w3
	add	w3, w13, w12
	add	w13, w13, w0
	add	w0, w3, w0
	str	w13, [x4, 1600]
	str	w0, [x4, 1088]
	str	w7, [x5, 1252]
	ldr	w0, [x2, 2492]
	cmp	w0, 255
	beq	.L83
	ldr	w0, [x5, 1260]
	ldr	w4, [x2, 2536]
	add	w3, w0, 1
	uxtw	x2, w0
	lsl	x0, x2, 2
	sub	x0, x0, x2
	add	x0, x5, x0, lsl 2
	str	w4, [x0, 520]
	str	w3, [x5, 1260]
.L83:
	ldr	w0, [x5, 1256]
	cmp	w0, w11
	bcs	.L84
	ldr	w2, [x1, 2552]
	add	x1, x25, x0, uxtw 3
	add	x1, x1, 294912
	add	w0, w0, 1
	strb	w9, [x1, 1784]
	str	w2, [x1, 1780]
	str	w0, [x5, 1256]
.L84:
	ldr	w0, [x10, 2008]
	cmp	w6, w0
	bcc	.L85
.L86:
	ldr	x3, [x23, 104]
	mov	x2, 1272
	mov	x1, x22
	mov	x0, x19
	blr	x3
	ldr	x1, [x24, w20, sxtw 3]
	mov	w3, 1
	ldr	x2, [x23, 32]
	mov	w0, 8
	add	x1, x1, 294912
	str	w3, [x1, 2012]
	blr	x2
	mov	w1, 0
	b	.L167
.L192:
	add	x1, x1, 2488
	mov	w0, w20
	bl	DelAllFrameMemRecord
	cbnz	w0, .L197
.L72:
	ldr	x1, [x24, w20, sxtw 3]
	mov	w2, 255
	add	x0, x1, 290816
	add	x1, x1, 294912
	add	x0, x0, 2488
	add	x1, x1, 696
	.p2align 2
.L73:
	str	w2, [x0], 72
	cmp	x0, x1
	bne	.L73
	mov	w0, 1088
	cmp	w23, 1920
	ccmp	w28, w0, 0, le
	ldr	x0, [x24, w20, sxtw 3]
	ble	.L74
	ldr	w1, [x29, 144]
.L75:
	add	x2, x0, 290816
	ldr	w0, [x0, 1520]
	add	w0, w0, w1
	str	w0, [x2, 2468]
	ldr	x0, .LC46
	ldr	x4, [x0]
	cbz	x4, .L77
	ldr	w0, [x29, 140]
	mov	w3, 28
	ldr	w2, [x29, 152]
	stp	w1, w0, [x29, 160]
	ldr	w1, [x29, 148]
	ldr	w0, [x29, 156]
	stp	w1, w0, [x29, 172]
	str	w2, [x29, 168]
	mov	w1, 121
	stp	w23, w28, [x29, 180]
	add	x2, x29, 160
	mov	w0, w20
	blr	x4
.L77:
	ldr	x23, .LC47
	mov	x2, 512
	ldr	x0, [x24, w20, sxtw 3]
	mov	w1, 0
	ldr	x3, [x23, 96]
	add	x0, x0, 294912
	add	x0, x0, 2016
	blr	x3
	ldr	x1, [x24, w20, sxtw 3]
	ldr	x0, .LC48
	add	x1, x1, 294912
	ldr	x0, [x0, w20, sxtw 3]
	str	wzr, [x1, 2528]
	bl	FSP_ClearContextAll
	ldr	x0, [x24, w20, sxtw 3]
	mov	w2, 1
	add	x1, x0, 294912
	add	x0, x0, 290816
	str	wzr, [x1, 2008]
	str	w2, [x0, 2440]
	str	w2, [x1, 2532]
	b	.L78
.L188:
	lsl	w0, w2, 2
	ldr	x1, .LC69
	sub	w2, w0, w2
	lsl	w0, w28, 2
	sub	w0, w0, w28
	lsr	w28, w28, 1
	lsr	w0, w0, 1
	str	w5, [x29, 120]
	madd	w7, w6, w7, w28
	add	w0, w0, w2, asr 1
	lsl	w6, w6, 5
	str	w0, [x19, 1060]
	mov	w0, 0
	str	w6, [x19, 1052]
	str	w7, [x19, 1064]
	bl	dprint_vfmw
	ldr	w5, [x29, 120]
	b	.L108
.L74:
	ldr	w1, [x0, 44]
	cmp	w1, 24
	ldr	w1, [x29, 144]
	beq	.L75
	add	w1, w1, 4
	str	w1, [x29, 144]
	b	.L75
.L197:
	ldr	x1, .LC45
	mov	w0, 0
	bl	dprint_vfmw
	b	.L72
.L189:
	ldr	x2, .LC38
	ldr	x3, .LC37
	b	.L177
.L89:
	ldr	x2, .LC38
	ldr	x3, .LC59
	b	.L177
.L181:
	ldr	x2, .LC38
	ldr	x3, .LC58
	b	.L177
.L95:
	ldr	x2, .LC38
	ldr	x3, .LC64
	b	.L177
.L187:
	ldr	x2, .LC38
	ldr	x3, .LC68
	b	.L177
.L185:
	ldr	x1, .LC66
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L167
.L182:
	ldr	x2, .LC38
	ldr	x3, .LC61
	b	.L177
.L196:
	ldr	x1, .LC56
	mov	w0, 31
	bl	dprint_vfmw
	ldr	x0, [x24, w20, sxtw 3]
	mov	w1, 30
	mov	w2, w1
	add	x0, x0, 290816
	str	w1, [x0, 2468]
	b	.L81
.L190:
	ldr	x1, .LC41
	mov	w0, 31
	bl	dprint_vfmw
	mov	w1, -1
	b	.L167
.L195:
	ldr	x1, .LC54
	mov	w0, 31
	bl	dprint_vfmw
	ldr	x0, [x24, w20, sxtw 3]
	b	.L80
.L194:
	ldr	x2, .LC38
	ldr	x1, .LC39
	ldr	x3, .LC52
	add	x2, x2, 120
	bl	dprint_vfmw
	mov	w1, -1
	b	.L167
	.size	VDMHAL_V400R003_ArrangeMem, .-VDMHAL_V400R003_ArrangeMem
	.align	3
.LC35:
	.xword	s_pstVfmwChan
	.align	3
.LC37:
	.xword	.LC36
	.align	3
.LC38:
	.xword	.LANCHOR0
	.align	3
.LC39:
	.xword	.LC7
	.align	3
.LC41:
	.xword	.LC40
	.align	3
.LC43:
	.xword	.LC42
	.align	3
.LC45:
	.xword	.LC44
	.align	3
.LC46:
	.xword	g_event_report
	.align	3
.LC47:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC48:
	.xword	s_pFspInst
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
.LC56:
	.xword	.LC55
	.align	3
.LC58:
	.xword	.LC57
	.align	3
.LC59:
	.xword	.LC26
	.align	3
.LC61:
	.xword	.LC60
	.align	3
.LC62:
	.xword	g_not_direct_8x8_inference_flag
	.align	3
.LC64:
	.xword	.LC63
	.align	3
.LC66:
	.xword	.LC65
	.align	3
.LC68:
	.xword	.LC67
	.align	3
.LC69:
	.xword	.LC33
	.align	3
.LC71:
	.xword	.LC70
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_ResetVdm
	.type	VDMHAL_V400R003_ResetVdm, %function
VDMHAL_V400R003_ResetVdm:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, wzr
	bgt	.L217
	bne	.L214
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L218
	ldr	x23, .LC79
	ldr	x20, .LC80
	ldr	x1, [x23]
	ldr	x0, [x20]
	ldr	w24, [x1, 36]
	ldr	w22, [x0, 120]
	tbnz	x22, 0, .L219
.L198:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L214:
	ldr	x1, .LC76
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L217:
	ldr	x2, .LC72
	mov	w3, w19
	ldr	x1, .LC74
	mov	w4, 0
	add	x2, x2, 160
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L219:
	ldr	x21, .LC81
	mov	w1, 2
	bfi	w22, w1, 8, 2
	str	w22, [x0, 120]
	mov	w19, 0
	orr	w22, w22, 64
	ldr	x0, [x21, 216]
	blr	x0
	ldr	x0, [x20]
	str	w22, [x0, 120]
	b	.L206
	.p2align 3
.L220:
	add	w19, w19, 1
	cmp	w19, 1000
	beq	.L207
.L206:
	ldr	x1, [x21, 224]
	mov	x0, 30
	blr	x1
	ldr	x0, [x20]
	ldr	w0, [x0, 372]
	tbz	x0, 2, .L220
	cmp	w19, 999
	bgt	.L207
	ldr	x2, .LC72
	mov	w3, 0
	ldr	x1, .LC85
	mov	w0, 0
	add	x2, x2, 160
	bl	dprint_vfmw
.L209:
	ldr	x0, [x20]
	and	w22, w22, -65
	str	w22, [x0, 120]
	and	w22, w22, -769
	ldr	x0, [x21, 216]
	blr	x0
	ldr	x1, [x20]
	ldr	x0, [x23]
	str	w22, [x1, 120]
	str	w24, [x0, 36]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L207:
	ldr	x2, .LC72
	mov	w3, 0
	ldr	x1, .LC83
	mov	w0, 0
	add	x2, x2, 160
	bl	dprint_vfmw
	b	.L209
.L218:
	ldr	x1, .LC78
	mov	x2, 0
	mov	w0, 0
	bl	dprint_vfmw
	b	.L198
	.size	VDMHAL_V400R003_ResetVdm, .-VDMHAL_V400R003_ResetVdm
	.align	3
.LC72:
	.xword	.LANCHOR0
	.align	3
.LC74:
	.xword	.LC73
	.align	3
.LC76:
	.xword	.LC75
	.align	3
.LC78:
	.xword	.LC77
	.align	3
.LC79:
	.xword	g_HwMem
	.align	3
.LC80:
	.xword	g_pstRegCrg
	.align	3
.LC81:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC83:
	.xword	.LC82
	.align	3
.LC85:
	.xword	.LC84
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_SetSmmuPageTableAddr
	.type	VDMHAL_V400R003_SetSmmuPageTableAddr, %function
VDMHAL_V400R003_SetSmmuPageTableAddr:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC86
	ldr	x3, [x0, 352]
	cbz	x3, .L221
	add	x2, x29, 60
	add	x1, x29, 56
	add	x0, x29, 52
	blr	x3
	cbnz	w19, .L223
	ldr	x0, .LC87
	ldp	w3, w2, [x29, 52]
	ldr	w1, [x29, 60]
	ldr	x0, [x0]
	add	x0, x0, 61440
	str	w3, [x0, 524]
	str	w2, [x0, 772]
	str	w1, [x0, 776]
.L221:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L223:
	ldr	x20, .LC88
	mov	w3, w19
	ldr	x21, .LC90
	mov	w4, 1
	add	x20, x20, 192
	mov	w0, 32
	mov	x1, x21
	mov	x2, x20
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x20
	mov	x1, x21
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w19
	mov	x2, x20
	mov	x1, x21
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	ret
	.size	VDMHAL_V400R003_SetSmmuPageTableAddr, .-VDMHAL_V400R003_SetSmmuPageTableAddr
	.align	3
.LC86:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC87:
	.xword	g_HwMem
	.align	3
.LC88:
	.xword	.LANCHOR0
	.align	3
.LC90:
	.xword	.LC89
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_EnableSmmu
	.type	VDMHAL_V400R003_EnableSmmu, %function
VDMHAL_V400R003_EnableSmmu:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC92
	mov	w1, 8
	ldr	x0, [x0]
	add	x0, x0, 61440
	str	w1, [x0]
	ldp	x29, x30, [sp], 16
	ret
	.size	VDMHAL_V400R003_EnableSmmu, .-VDMHAL_V400R003_EnableSmmu
	.align	3
.LC92:
	.xword	g_HwMem
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_GlbResetX
	.type	VDMHAL_V400R003_GlbResetX, %function
VDMHAL_V400R003_GlbResetX:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L244
	cbnz	w19, .L243
	ldr	x20, .LC96
	ldr	x0, [x20]
	ldr	w22, [x0, 120]
	tbnz	x22, 0, .L245
.L228:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L245:
	ldr	x21, .LC97
	mov	w1, 2
	bfi	w22, w1, 8, 2
	str	w22, [x0, 120]
	orr	w22, w22, 16
	ldr	x0, [x21, 216]
	blr	x0
	ldr	x0, [x20]
	str	w22, [x0, 120]
	b	.L235
	.p2align 3
.L246:
	add	w19, w19, 1
	cmp	w19, 1000
	beq	.L236
.L235:
	ldr	x1, [x21, 224]
	mov	x0, 30
	blr	x1
	ldr	x0, [x20]
	ldr	w0, [x0, 372]
	tbz	x0, 0, .L246
	cmp	w19, 999
	bgt	.L236
	ldr	x2, .LC93
	mov	w3, 0
	ldr	x1, .LC101
	mov	w0, 0
	add	x2, x2, 232
	bl	dprint_vfmw
.L238:
	ldr	x0, [x20]
	and	w22, w22, -17
	str	w22, [x0, 120]
	and	w22, w22, -769
	ldr	x0, [x21, 216]
	blr	x0
	ldr	x0, [x20]
	str	w22, [x0, 120]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L243:
	ldr	x1, .LC103
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L236:
	ldr	x2, .LC93
	mov	w3, 0
	ldr	x1, .LC99
	mov	w0, 0
	add	x2, x2, 232
	bl	dprint_vfmw
	b	.L238
.L244:
	ldr	x2, .LC93
	mov	w3, -121438208
	ldr	x1, .LC95
	add	x2, x2, 232
	bl	dprint_vfmw
	b	.L228
	.size	VDMHAL_V400R003_GlbResetX, .-VDMHAL_V400R003_GlbResetX
	.align	3
.LC93:
	.xword	.LANCHOR0
	.align	3
.LC95:
	.xword	.LC94
	.align	3
.LC96:
	.xword	g_pstRegCrg
	.align	3
.LC97:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC99:
	.xword	.LC98
	.align	3
.LC101:
	.xword	.LC100
	.align	3
.LC103:
	.xword	.LC102
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_GlbReset
	.type	VDMHAL_V400R003_GlbReset, %function
VDMHAL_V400R003_GlbReset:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L250
	mov	w0, 0
	bl	VDMHAL_V400R003_GlbResetX
.L247:
	ldp	x29, x30, [sp], 16
	ret
.L250:
	ldr	x2, .LC104
	mov	w3, -121438208
	ldr	x1, .LC105
	add	x2, x2, 264
	bl	dprint_vfmw
	b	.L247
	.size	VDMHAL_V400R003_GlbReset, .-VDMHAL_V400R003_GlbReset
	.align	3
.LC104:
	.xword	.LANCHOR0
	.align	3
.LC105:
	.xword	.LC94
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_ClearIntState
	.type	VDMHAL_V400R003_ClearIntState, %function
VDMHAL_V400R003_ClearIntState:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC107
	sbfiz	x0, x19, 6, 32
	ldr	x1, .LC106
	add	x0, x20, x0
	ldrsw	x0, [x0, 8]
	ldr	x0, [x1, x0, lsl 3]
	cbz	x0, .L252
	ldr	w21, [x0, 1296]
	cbnz	w19, .L253
.L254:
	ldr	x19, .LC110
	ldr	x0, [x19]
	cbz	x0, .L268
.L256:
	cmp	w21, 1
	bne	.L267
	ldr	w1, [x20, 44]
	cmp	w1, 1
	beq	.L267
	ldr	w2, [x0, 28]
	cmp	w1, 2
	beq	.L269
	cmp	w1, 3
	bne	.L251
	mov	w1, -15
	str	w1, [x0, 32]
	b	.L251
	.p2align 3
.L252:
	mov	w21, 0
	cbz	w19, .L254
	.p2align 2
.L253:
	ldr	x1, .LC109
	mov	w0, 0
	bl	dprint_vfmw
.L251:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L267:
	mov	w1, -1
	str	w1, [x0, 32]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L268:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L257
	str	x0, [x19]
	b	.L256
	.p2align 3
.L269:
	mov	w1, -12
	str	w1, [x0, 32]
	b	.L251
.L257:
	ldr	x1, .LC112
	mov	w0, 0
	bl	dprint_vfmw
	b	.L251
	.size	VDMHAL_V400R003_ClearIntState, .-VDMHAL_V400R003_ClearIntState
	.align	3
.LC106:
	.xword	s_pstVfmwChan
	.align	3
.LC107:
	.xword	g_VdmDrvParam
	.align	3
.LC109:
	.xword	.LC108
	.align	3
.LC110:
	.xword	g_HwMem
	.align	3
.LC112:
	.xword	.LC111
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_ClearMMUIntState
	.type	VDMHAL_V400R003_ClearMMUIntState, %function
VDMHAL_V400R003_ClearMMUIntState:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cbnz	w19, .L276
	ldr	x19, .LC114
	ldr	x0, [x19]
	cbz	x0, .L277
.L274:
	add	x0, x0, 61440
	mov	w1, 7
	str	w1, [x0, 44]
.L270:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L276:
	ldr	x1, .LC113
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L277:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L275
	str	x0, [x19]
	b	.L274
.L275:
	ldr	x1, .LC115
	mov	w0, 0
	bl	dprint_vfmw
	b	.L270
	.size	VDMHAL_V400R003_ClearMMUIntState, .-VDMHAL_V400R003_ClearMMUIntState
	.align	3
.LC113:
	.xword	.LC108
	.align	3
.LC114:
	.xword	g_HwMem
	.align	3
.LC115:
	.xword	.LC111
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_MaskInt
	.type	VDMHAL_V400R003_MaskInt, %function
VDMHAL_V400R003_MaskInt:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cbnz	w19, .L284
	ldr	x19, .LC118
	ldr	x0, [x19]
	cbz	x0, .L285
.L282:
	mov	w1, -1
	str	w1, [x0, 36]
.L278:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L284:
	ldr	x1, .LC117
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L285:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L283
	str	x0, [x19]
	b	.L282
.L283:
	ldr	x1, .LC119
	mov	w0, 0
	bl	dprint_vfmw
	b	.L278
	.size	VDMHAL_V400R003_MaskInt, .-VDMHAL_V400R003_MaskInt
	.align	3
.LC117:
	.xword	.LC116
	.align	3
.LC118:
	.xword	g_HwMem
	.align	3
.LC119:
	.xword	.LC111
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_GetIntMaskCfg
	.type	VDMHAL_V400R003_GetIntMaskCfg, %function
VDMHAL_V400R003_GetIntMaskCfg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 1
	mov	w1, -6
	mov	w0, -2
	ldr	x19, [sp, 16]
	csel	w0, w1, w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMHAL_V400R003_GetIntMaskCfg, .-VDMHAL_V400R003_GetIntMaskCfg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_EnableInt
	.type	VDMHAL_V400R003_EnableInt, %function
VDMHAL_V400R003_EnableInt:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x2, .LC121
	sbfiz	x0, x19, 6, 32
	ldr	x1, .LC120
	add	x0, x2, x0
	ldrsw	x0, [x0, 8]
	ldr	x0, [x1, x0, lsl 3]
	cbz	x0, .L290
	ldr	w20, [x0, 1296]
	cbnz	w19, .L291
.L292:
	ldr	x19, .LC124
	ldr	x0, [x19]
	cbz	x0, .L303
.L294:
	mov	w1, -6
	mov	w2, -2
	cmp	w20, 1
	csel	w1, w1, w2, eq
	str	w1, [x0, 36]
.L289:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L290:
	mov	w20, 0
	cbz	w19, .L292
.L291:
	ldr	x1, .LC123
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L303:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L295
	str	x0, [x19]
	b	.L294
.L295:
	ldr	x1, .LC125
	mov	w0, 0
	bl	dprint_vfmw
	b	.L289
	.size	VDMHAL_V400R003_EnableInt, .-VDMHAL_V400R003_EnableInt
	.align	3
.LC120:
	.xword	s_pstVfmwChan
	.align	3
.LC121:
	.xword	g_VdmDrvParam
	.align	3
.LC123:
	.xword	.LC122
	.align	3
.LC124:
	.xword	g_HwMem
	.align	3
.LC125:
	.xword	.LC111
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_CheckReg
	.type	VDMHAL_V400R003_CheckReg, %function
VDMHAL_V400R003_CheckReg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cmp	w19, wzr
	bgt	.L326
	bne	.L324
	ldr	x19, .LC129
	ldr	x0, [x19]
	cbz	x0, .L327
.L310:
	sub	w0, w20, #1
	cmp	w0, 6
	bls	.L328
	ldr	x2, .LC126
	mov	w3, w20
	ldr	x1, .LC135
	mov	w0, 0
	add	x2, x2, 296
	bl	dprint_vfmw
	mov	w0, 0
.L323:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L328:
	ldr	x1, .LC136
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx317
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx317:
	.section	.rodata
	.align	0
	.align	2
.L317:
	.byte	(.L321 - .Lrtx317) / 4
	.byte	(.L318 - .Lrtx317) / 4
	.byte	(.L319 - .Lrtx317) / 4
	.byte	(.L320 - .Lrtx317) / 4
	.byte	(.L322 - .Lrtx317) / 4
	.byte	(.L312 - .Lrtx317) / 4
	.byte	(.L314 - .Lrtx317) / 4
	.text
	.p2align 3
.L322:
	mov	x0, 61480
.L316:
	ldr	x1, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ldr	w0, [x1, x0]
	ret
	.p2align 3
.L321:
	mov	x0, 28
	b	.L316
	.p2align 3
.L312:
	mov	x0, 62304
	b	.L316
	.p2align 3
.L318:
	mov	x0, 32
	b	.L316
	.p2align 3
.L320:
	mov	x0, 40
	b	.L316
	.p2align 3
.L319:
	mov	x0, 36
	b	.L316
	.p2align 3
.L314:
	mov	x0, 62272
	b	.L316
	.p2align 3
.L324:
	ldr	x2, .LC126
	mov	w0, 0
	ldr	x1, .LC131
	add	x2, x2, 296
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L327:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L311
	str	x0, [x19]
	b	.L310
.L311:
	ldr	x2, .LC126
	mov	w0, 0
	ldr	x1, .LC133
	add	x2, x2, 296
	bl	dprint_vfmw
	mov	w0, 0
	b	.L323
	.p2align 3
.L326:
	ldr	x2, .LC126
	mov	w3, w19
	ldr	x1, .LC128
	mov	w0, 0
	add	x2, x2, 296
	bl	dprint_vfmw
	mov	w0, -1
	b	.L323
	.size	VDMHAL_V400R003_CheckReg, .-VDMHAL_V400R003_CheckReg
	.align	3
.LC126:
	.xword	.LANCHOR0
	.align	3
.LC128:
	.xword	.LC127
	.align	3
.LC129:
	.xword	g_HwMem
	.align	3
.LC131:
	.xword	.LC130
	.align	3
.LC133:
	.xword	.LC132
	.align	3
.LC135:
	.xword	.LC134
	.align	3
.LC136:
	.xword	.L317
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_ReadMMUMask
	.type	VDMHAL_V400R003_ReadMMUMask, %function
VDMHAL_V400R003_ReadMMUMask:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	sxtw	x19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, wzr
	ble	.L332
	ldr	x2, .LC138
	mov	w3, w19
	ldr	x1, .LC140
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L332:
	lsl	x0, x19, 6
	ldr	x1, .LC137
	sub	x0, x0, x19
	lsl	x0, x0, 2
	sub	x19, x0, x19
	add	x19, x0, x19, lsl 2
	ldr	x0, [x1, x19]
	ldr	x19, [sp, 16]
	add	x0, x0, 61440
	ldp	x29, x30, [sp], 32
	ldr	w0, [x0, 32]
	ret
	.size	VDMHAL_V400R003_ReadMMUMask, .-VDMHAL_V400R003_ReadMMUMask
	.align	3
.LC137:
	.xword	g_HwMem
	.align	3
.LC138:
	.xword	.LANCHOR1
	.align	3
.LC140:
	.xword	.LC139
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_WriteMMUMask
	.type	VDMHAL_V400R003_WriteMMUMask, %function
VDMHAL_V400R003_WriteMMUMask:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	sxtw	x19, w1
	bl	_mcount
	cmp	w19, wzr
	ble	.L336
	ldr	x2, .LC142
	mov	w3, w19
	ldr	x1, .LC143
	mov	w4, 1
	add	x2, x2, 32
	mov	w0, 32
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L336:
	lsl	x0, x19, 6
	ldr	x1, .LC141
	sub	x0, x0, x19
	lsl	x0, x0, 2
	sub	x19, x0, x19
	add	x19, x0, x19, lsl 2
	ldr	x0, [x1, x19]
	add	x0, x0, 61440
	str	w20, [x0, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMHAL_V400R003_WriteMMUMask, .-VDMHAL_V400R003_WriteMMUMask
	.align	3
.LC141:
	.xword	g_HwMem
	.align	3
.LC142:
	.xword	.LANCHOR1
	.align	3
.LC143:
	.xword	.LC89
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_PrepareDec
	.type	VDMHAL_V400R003_PrepareDec, %function
VDMHAL_V400R003_PrepareDec:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w20, w0
	mov	x22, x1
	mov	x0, x30
	mov	w21, w2
	mov	x19, x3
	bl	_mcount
	cbz	x22, .L357
	cmp	w21, wzr
	bgt	.L358
	cmp	w20, 6
	beq	.L359
	mov	w2, 0
	mov	w1, 0
	mov	w0, 0
	mov	x3, x19
	bl	SCD_ConfigReg
	mov	x3, x19
	mov	w2, 0
	mov	w1, 1
	mov	w0, 8
	bl	SCD_ConfigReg
	cmp	w20, 17
	bls	.L360
.L356:
	mov	w0, -1
	b	.L339
	.p2align 3
.L360:
	ldr	x0, .LC148
	ldrb	w0, [x0,w20,uxtw]
	adr	x1, .Lrtx344
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx344:
	.section	.rodata
	.align	0
	.align	2
.L344:
	.byte	(.L353 - .Lrtx344) / 4
	.byte	(.L345 - .Lrtx344) / 4
	.byte	(.L346 - .Lrtx344) / 4
	.byte	(.L347 - .Lrtx344) / 4
	.byte	(.L356 - .Lrtx344) / 4
	.byte	(.L348 - .Lrtx344) / 4
	.byte	(.L342 - .Lrtx344) / 4
	.byte	(.L356 - .Lrtx344) / 4
	.byte	(.L349 - .Lrtx344) / 4
	.byte	(.L350 - .Lrtx344) / 4
	.byte	(.L351 - .Lrtx344) / 4
	.byte	(.L351 - .Lrtx344) / 4
	.byte	(.L351 - .Lrtx344) / 4
	.byte	(.L352 - .Lrtx344) / 4
	.byte	(.L356 - .Lrtx344) / 4
	.byte	(.L353 - .Lrtx344) / 4
	.byte	(.L354 - .Lrtx344) / 4
	.byte	(.L355 - .Lrtx344) / 4
	.text
	.p2align 3
.L359:
	mov	x3, x19
	mov	w2, 0
	mov	w1, 1
	mov	w0, 0
	bl	SCD_ConfigReg
	mov	x3, x19
	mov	w2, 0
	mov	w1, 1
	mov	w0, 8
	bl	SCD_ConfigReg
.L342:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	AVSHAL_V400R003_StartDec
.L339:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L353:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	H264HAL_V400R003_StartDec
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L348:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	DIVX3HAL_V400R003_StartDec
	b	.L339
	.p2align 3
.L347:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	MP2HAL_V400R003_StartDec
	b	.L339
	.p2align 3
.L346:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	MP4HAL_V400R003_StartDec
	b	.L339
	.p2align 3
.L345:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	VC1HAL_V400R003_StartDec
	b	.L339
	.p2align 3
.L355:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	VP9HAL_V400R003_StartDec
	b	.L339
	.p2align 3
.L354:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	HEVCHAL_V400R003_StartDec
	b	.L339
	.p2align 3
.L352:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	VP8HAL_V400R003_StartDec
	b	.L339
	.p2align 3
.L351:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	VP6HAL_V400R003_StartDec
	b	.L339
	.p2align 3
.L350:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	RV9HAL_V400R003_StartDec
	b	.L339
	.p2align 3
.L349:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	RV8HAL_V400R003_StartDec
	b	.L339
	.p2align 3
.L358:
	ldr	x2, .LC144
	mov	w4, 0
	ldr	x1, .LC147
	mov	w3, w21
	add	x2, x2, 64
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L339
.L357:
	ldr	x2, .LC144
	mov	x3, 0
	ldr	x1, .LC146
	mov	w0, 0
	add	x2, x2, 64
	bl	dprint_vfmw
	mov	w0, -1
	b	.L339
	.size	VDMHAL_V400R003_PrepareDec, .-VDMHAL_V400R003_PrepareDec
	.align	3
.LC144:
	.xword	.LANCHOR1
	.align	3
.LC146:
	.xword	.LC145
	.align	3
.LC147:
	.xword	.LC73
	.align	3
.LC148:
	.xword	.L344
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_IsVdmReady
	.type	VDMHAL_V400R003_IsVdmReady, %function
VDMHAL_V400R003_IsVdmReady:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	sxtw	x0, w19
	ldr	x2, .LC149
	lsl	x1, x0, 6
	sub	x1, x1, x0
	lsl	x1, x1, 2
	sub	x0, x1, x0
	add	x0, x1, x0, lsl 2
	ldr	x0, [x2, x0]
	cbz	x0, .L366
	cmp	w19, wzr
	ble	.L367
	ldr	x2, .LC152
	mov	w4, 1
	ldr	x1, .LC154
	mov	w3, w19
	add	x2, x2, 96
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, 0
.L363:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L367:
	ldr	w0, [x0, 28]
	ldr	x19, [sp, 16]
	ubfx	x0, x0, 17, 1
	ldp	x29, x30, [sp], 32
	ret
.L366:
	ldr	x2, .LC152
	ldr	x3, .LC151
	ldr	x1, .LC153
	add	x2, x2, 96
	bl	dprint_vfmw
	mov	w0, -1
	b	.L363
	.size	VDMHAL_V400R003_IsVdmReady, .-VDMHAL_V400R003_IsVdmReady
	.align	3
.LC149:
	.xword	g_HwMem
	.align	3
.LC151:
	.xword	.LC150
	.align	3
.LC152:
	.xword	.LANCHOR1
	.align	3
.LC153:
	.xword	.LC7
	.align	3
.LC154:
	.xword	.LC139
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_IsVdmRun
	.type	VDMHAL_V400R003_IsVdmRun, %function
VDMHAL_V400R003_IsVdmRun:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	sxtw	x0, w19
	ldr	x2, .LC155
	lsl	x1, x0, 6
	sub	x1, x1, x0
	lsl	x1, x1, 2
	sub	x0, x1, x0
	add	x0, x1, x0, lsl 2
	ldr	x0, [x2, x0]
	cbz	x0, .L373
	cmp	w19, wzr
	ble	.L374
	ldr	x2, .LC158
	mov	w4, 1
	ldr	x1, .LC159
	mov	w3, w19
	add	x2, x2, 128
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, 1
.L370:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L374:
	ldr	w0, [x0, 40]
	ldr	x19, [sp, 16]
	cmp	w0, 1
	cset	w0, ne
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L373:
	ldr	x1, .LC157
	bl	dprint_vfmw
	mov	w0, 0
	b	.L370
	.size	VDMHAL_V400R003_IsVdmRun, .-VDMHAL_V400R003_IsVdmRun
	.align	3
.LC155:
	.xword	g_HwMem
	.align	3
.LC157:
	.xword	.LC156
	.align	3
.LC158:
	.xword	.LANCHOR1
	.align	3
.LC159:
	.xword	.LC139
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_BackupInfo
	.type	VDMHAL_V400R003_BackupInfo, %function
VDMHAL_V400R003_BackupInfo:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L419
	ldr	w0, [x19, 8]
	ldr	x22, [x19]
	cbnz	w0, .L411
	ldr	x2, .LC164
	ldr	w0, [x2]
	cmp	w0, 1
	beq	.L380
	add	x0, x2, 4
	add	x2, x2, 64
	.p2align 2
.L382:
	ldr	w1, [x0]
	add	x0, x0, 4
	cmp	w1, 1
	beq	.L380
	cmp	x0, x2
	bne	.L382
	ldr	x23, .LC169
	ldr	x1, [x23]
	cbz	x1, .L420
	ldr	x19, .LC170
	mov	w20, 0
	ldr	w6, [x1, 12]
	ldr	w0, [x1, 28]
	stp	w6, w0, [x19]
.L386:
	ldr	w2, [x1, 4096]
	ldr	w0, [x1, 4116]
	ldr	w5, [x1, 208]
	ldr	w7, [x1, 212]
	stp	w5, w7, [x19, 8]
	stp	w2, w0, [x19, 16]
	cbnz	w20, .L389
	ldr	w2, [x1, 176]
	ldr	w0, [x1, 180]
	stp	w2, w0, [x19, 24]
	ldr	w3, [x1, 184]
	ldr	w2, [x1, 188]
	ldr	w0, [x1, 192]
	str	w3, [x19, 32]
	stp	w2, w0, [x19, 36]
.L389:
	ldr	x0, .LC174
	add	x4, x1, 32768
	add	x1, x4, 256
	add	x3, x0, 128
	.p2align 2
.L390:
	ldr	w2, [x1], 4
	str	w2, [x0], 4
	cmp	x3, x0
	bne	.L390
	cbnz	w20, .L391
	ldr	w1, [x4, 384]
	ldr	w0, [x4, 388]
	stp	w0, w1, [x19, 44]
.L391:
	and	w6, w6, 15
	cmp	w6, 5
	beq	.L421
	and	w5, w5, 31
	mov	w0, 0
	str	w5, [x22]
.L410:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L380:
	ldr	x23, .LC169
	ldr	x0, [x23]
	cbz	x0, .L422
	ldr	x19, .LC170
	mov	w20, 1
	ldr	w2, [x0, 12]
	ldr	w1, [x0, 28]
	stp	w2, w1, [x19]
.L385:
	ldr	w0, [x23, 56]
	and	w21, w1, 131071
	bl	MEM_Phy2Vir
	cbz	x0, .L423
	sub	w1, w21, #1
	cmp	w1, 199
	bhi	.L396
	lsl	w1, w21, 2
	sub	w1, w1, #4
	lsl	x1, x1, 2
	add	x2, x1, 8
.L388:
	ldr	w3, [x0, x1]
	ldr	x1, [x23]
	str	w3, [x19, 16]
	ldr	w6, [x19]
	ldr	w5, [x1, 208]
	ldr	w0, [x0, x2]
	ldr	w7, [x1, 212]
	stp	w5, w7, [x19, 8]
	str	w0, [x19, 20]
	b	.L389
.L421:
	and	w7, w7, 1
	str	w7, [x22]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L420:
	mov	w20, 0
.L393:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L384
	ldr	x19, .LC170
	ldr	w6, [x0, 12]
	ldr	w1, [x0, 28]
	stp	w6, w1, [x19]
	str	x0, [x23]
	cbnz	w20, .L385
	mov	x1, x0
	b	.L386
	.p2align 3
.L396:
	mov	x2, 8
	mov	x1, 0
	b	.L388
.L411:
	ldr	x1, .LC166
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L410
.L419:
	ldr	x2, .LC162
	mov	w0, 0
	ldr	x3, .LC161
	ldr	x1, .LC163
	add	x2, x2, 160
	bl	dprint_vfmw
	mov	w0, -1
	b	.L410
.L423:
	ldr	x2, .LC162
	ldr	x3, .LC173
	ldr	x1, .LC163
	add	x2, x2, 160
	bl	dprint_vfmw
	mov	w0, -1
	b	.L410
.L384:
	ldr	x1, .LC171
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L410
.L422:
	mov	w20, 1
	b	.L393
	.size	VDMHAL_V400R003_BackupInfo, .-VDMHAL_V400R003_BackupInfo
	.align	3
.LC161:
	.xword	.LC160
	.align	3
.LC162:
	.xword	.LANCHOR1
	.align	3
.LC163:
	.xword	.LC7
	.align	3
.LC164:
	.xword	g_DSPState
	.align	3
.LC166:
	.xword	.LC165
	.align	3
.LC169:
	.xword	g_HwMem
	.align	3
.LC170:
	.xword	g_BackUp
	.align	3
.LC171:
	.xword	.LC111
	.align	3
.LC173:
	.xword	.LC172
	.align	3
.LC174:
	.xword	g_BackUp+52
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_ReadMsgSlot
	.type	VDMHAL_V400R003_ReadMsgSlot, %function
VDMHAL_V400R003_ReadMsgSlot:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	mov	w19, w2
	bl	_mcount
	cmp	x20, xzr
	ccmp	x21, xzr, 4, ne
	beq	.L428
	cmp	w19, 800
	bhi	.L429
	ldr	x3, .LC180
	ubfiz	x2, x19, 2, 30
	mov	x1, x21
	mov	x0, x20
	ldr	x3, [x3, 104]
	blr	x3
	mov	w0, 0
.L426:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L429:
	ldr	x1, .LC179
	mov	w2, w19
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L426
	.p2align 3
.L428:
	ldr	x1, .LC177
	mov	x3, x21
	mov	x2, x20
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L426
	.size	VDMHAL_V400R003_ReadMsgSlot, .-VDMHAL_V400R003_ReadMsgSlot
	.align	3
.LC177:
	.xword	.LC176
	.align	3
.LC179:
	.xword	.LC178
	.align	3
.LC180:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_WriteMsgSlot
	.type	VDMHAL_V400R003_WriteMsgSlot, %function
VDMHAL_V400R003_WriteMsgSlot:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	w21, w2
	bl	_mcount
	cmp	x19, xzr
	ccmp	x20, xzr, 4, ne
	beq	.L431
	sub	w0, w21, #1
	cmp	w0, 255
	bhi	.L431
	ldr	x3, .LC183
	ubfiz	x2, x21, 2, 30
	mov	x1, x20
	mov	x0, x19
	ldr	x3, [x3, 104]
	blr	x3
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L431:
	ldr	x1, .LC182
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	VDMHAL_V400R003_WriteMsgSlot, .-VDMHAL_V400R003_WriteMsgSlot
	.align	3
.LC182:
	.xword	.LC181
	.align	3
.LC183:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_CfgRpMsg
	.type	VDMHAL_V400R003_CfgRpMsg, %function
VDMHAL_V400R003_CfgRpMsg:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x1
	mov	x19, x0
	mov	x0, x30
	mov	w22, w2
	bl	_mcount
	ldr	w0, [x20, 64]
	bl	MEM_Phy2Vir
	mov	x20, x0
	cbz	x0, .L475
	ldr	w2, [x19, 840]
	sub	w0, w2, #1
	cmp	w0, 199
	bhi	.L476
	ldp	w2, w1, [x19, 24]
	ldr	w4, [x19, 864]
	mov	w3, 0
	ldr	w5, [x19, 848]
	lsl	w2, w2, w4
	lsl	w1, w1, w4
	add	w0, w2, 255
	cmp	w5, 1
	and	w0, w0, -256
	lsl	w5, w0, 4
	beq	.L477
.L438:
	ldr	w2, [x19]
	cmp	w2, 16
	beq	.L439
	add	w1, w1, 31
	mov	w24, 8
	and	w1, w1, -32
	madd	w0, w1, w0, w3
.L459:
	ldr	w1, [x19, 16]
	mov	w4, 0
	str	w1, [x20]
	mov	w2, 0
	ldr	w1, [x19, 16]
	add	w1, w0, w1
	str	w1, [x20, 4]
	ldr	w1, [x19, 4]
	str	w1, [x20, 8]
	ldr	w1, [x19, 4]
	add	w0, w0, w1
	str	w0, [x20, 12]
	str	w5, [x20, 16]
	str	w3, [x20, 20]
	ldp	w1, w0, [x19, 24]
	sub	w0, w0, #1
	sub	w1, w1, #1
	bfi	w2, w0, 0, 9
	bfi	w4, w1, 0, 9
	strh	w2, [x29, 78]
	strh	w4, [x29, 76]
	ldr	w0, [x29, 76]
	str	w0, [x20, 24]
	ldr	w0, [x19, 852]
	ldr	w21, [x19, 24]
	sub	w0, w0, #1
	cmp	w0, 1
	bls	.L478
	ldr	w23, [x19, 28]
.L441:
	ldr	w0, [x19, 864]
	cmp	w0, 6
	beq	.L461
	cmp	w0, 5
	beq	.L462
	cmp	w0, 4
	mov	w3, 8
	beq	.L442
	ldr	x1, .LC191
	mov	w0, 1
	bl	dprint_vfmw
	mov	w3, 1
.L442:
	ldr	w0, [x19, 856]
	cbnz	w0, .L464
.L485:
	ldr	w9, [x19, 840]
	mov	w0, 65535
	cmp	w9, wzr
	ble	.L445
	mul	w12, w23, w21
	mov	w11, 0
	mov	w8, 0
	sub	w13, w12, #1
	mov	w4, 0
	sub	w2, w3, #1
	b	.L446
	.p2align 3
.L448:
	add	w4, w4, 1
	cmp	w9, w4
	ble	.L479
.L458:
	cmp	w4, 199
	bgt	.L480
.L446:
	add	x10, x19, x4, sxtw 2
	cmp	w4, wzr
	ldrsh	w1, [x10, 42]
	sub	w0, w1, #1
	add	w0, w0, w3
	udiv	w0, w0, w3
	mul	w0, w0, w3
	ble	.L447
	ldrsh	w8, [x10, 38]
	sub	w8, w8, #1
	add	w8, w8, w3
	udiv	w8, w8, w3
	mul	w8, w8, w3
	cmp	w0, w8
	ble	.L448
	add	w5, w4, 1
	add	x6, x19, x4, sxtw 2
	cmp	w5, w9
	add	w0, w2, w1
	add	w7, w5, 1
	bge	.L481
	.p2align 2
.L452:
	udiv	w0, w0, w3
	ldrsh	w1, [x6, 44]
	udiv	w1, w1, w3
	mul	w0, w0, w3
	mul	w1, w1, w3
	cmp	w0, w1
	add	w14, w1, 1
	bhi	.L453
	sub	w1, w4, #1
	cmp	w0, w14
	add	x1, x19, x1, sxtw 2
	beq	.L453
	ldrsh	w1, [x1, 42]
	sub	w1, w1, #1
	add	w1, w1, w3
	udiv	w1, w1, w3
	mul	w1, w1, w3
	cmp	w0, w1
	bhi	.L451
.L453:
	cmp	w7, 199
	mov	w4, w5
	bgt	.L482
	add	w5, w4, 1
	ldrsh	w1, [x6, 46]
	cmp	w5, w9
	add	x6, x19, x4, sxtw 2
	add	w0, w2, w1
	add	w7, w5, 1
	blt	.L452
.L481:
	udiv	w0, w0, w3
	mul	w0, w0, w3
.L451:
	ldrsh	w1, [x10, 40]
	cmp	w12, w0
	csel	w0, w0, w13, hi
	add	w6, w11, 1
	udiv	w1, w1, w3
	mul	w1, w1, w3
	cmp	w0, w1
	csel	w1, w1, wzr, ge
	cmp	w6, 252
	bgt	.L483
	cbnz	w22, .L466
	udiv	w7, w1, w21
	sxtw	x5, w11
	mov	w11, w6
	mov	w9, 0
	add	x5, x5, 6
	add	w4, w4, 1
	lsl	x5, x5, 3
	udiv	w6, w0, w21
	msub	w1, w7, w21, w1
	bfi	w9, w1, 0, 9
	mov	w1, 0
	strh	w9, [x29, 76]
	bfi	w1, w7, 0, 9
	strh	w1, [x29, 78]
	mov	w1, 0
	ldr	w7, [x29, 76]
	msub	w0, w6, w21, w0
	str	w7, [x20, x5]
	add	x5, x5, 4
	bfi	w1, w0, 0, 9
	mov	w0, 0
	bfi	w0, w6, 0, 9
	strh	w1, [x29, 76]
	strh	w0, [x29, 78]
	ldr	w0, [x29, 76]
	str	w0, [x20, x5]
	ldr	w9, [x19, 840]
	cmp	w9, w4
	bgt	.L458
.L479:
	ldr	w0, [x19, 856]
	cmp	w0, 1
	bne	.L484
.L457:
	mov	w0, 1
	str	w0, [x19, 840]
	mov	w0, 0
	cbnz	w22, .L445
	strh	wzr, [x29, 76]
	sub	w21, w21, #1
	strh	wzr, [x29, 78]
	mov	w2, 0
	sub	w23, w23, #1
	mov	w1, 0
	ldr	w3, [x29, 76]
	bfi	w1, w23, 0, 9
	bfi	w2, w21, 0, 9
	strh	w1, [x29, 78]
	strh	w2, [x29, 76]
	str	w3, [x20, 48]
	ldr	w1, [x29, 76]
	str	w1, [x20, 52]
	b	.L445
	.p2align 3
.L464:
	cmp	w0, 1
	mov	w11, 0
	beq	.L457
.L484:
	sub	w0, w11, #1
	uxth	w0, w0
.L445:
	ldr	w5, [x19, 848]
	ldr	w3, [x19, 852]
	ldr	w4, [x19, 864]
	ldrb	w1, [x29, 78]
	and	w3, w3, 3
	ldrb	w2, [x29, 79]
	sub	w4, w4, #4
	bfi	w1, w5, 0, 1
	strh	w0, [x29, 76]
	bfi	w1, w3, 4, 2
	bfi	w2, w4, 0, 2
	bfi	w1, w3, 6, 2
	bfi	w2, w24, 2, 4
	strb	w1, [x29, 78]
	mov	w0, 0
	strb	w2, [x29, 79]
	ldr	w1, [x29, 76]
	str	w1, [x20, 28]
.L473:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L447:
	ccmp	w0, w8, 0, ne
	ble	.L448
	b	.L451
	.p2align 3
.L466:
	mov	w11, w6
	b	.L448
	.p2align 3
.L482:
	add	x0, x19, x5, sxtw 2
	ldrsh	w0, [x0, 42]
	sub	w0, w0, #1
	add	w0, w0, w3
	udiv	w0, w0, w3
	mul	w0, w0, w3
	b	.L451
.L461:
	ldr	w0, [x19, 856]
	mov	w3, 2
	cbnz	w0, .L464
	b	.L485
.L439:
	madd	w0, w1, w0, w3
	mov	w24, 10
	b	.L459
.L477:
	adds	w3, w2, 511
	add	w2, w2, 1022
	csel	w2, w2, w3, mi
	add	w4, w1, 126
	and	w2, w2, -512
	cmp	w2, wzr
	add	w3, w2, 63
	csel	w2, w3, w2, lt
	adds	w6, w1, 63
	csel	w3, w4, w6, mi
	asr	w2, w2, 6
	asr	w3, w3, 6
	lsl	w2, w2, 1
	lsl	w3, w3, 5
	mul	w3, w2, w3
	b	.L438
.L478:
	ldr	w0, [x19, 28]
	add	w0, w0, w0, lsr 31
	asr	w23, w0, 1
	b	.L441
.L462:
	ldr	w0, [x19, 856]
	mov	w3, 4
	cbnz	w0, .L464
	b	.L485
.L480:
	ldr	x2, .LC186
	mov	w3, 2505
	ldr	x1, .LC193
	mov	w0, 0
	add	x2, x2, 192
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
.L483:
	mov	w0, 1
	str	w0, [x19, 856]
	b	.L457
.L476:
	ldr	x1, .LC189
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L473
.L475:
	ldr	x2, .LC186
	mov	w0, 0
	ldr	x3, .LC185
	ldr	x1, .LC187
	add	x2, x2, 192
	bl	dprint_vfmw
	mov	w0, -1
	b	.L473
	.size	VDMHAL_V400R003_CfgRpMsg, .-VDMHAL_V400R003_CfgRpMsg
	.align	3
.LC185:
	.xword	.LC184
	.align	3
.LC186:
	.xword	.LANCHOR1
	.align	3
.LC187:
	.xword	.LC7
	.align	3
.LC189:
	.xword	.LC188
	.align	3
.LC191:
	.xword	.LC190
	.align	3
.LC193:
	.xword	.LC192
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_CfgRpReg
	.type	VDMHAL_V400R003_CfgRpReg, %function
VDMHAL_V400R003_CfgRpReg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x0, x30
	mov	x21, x1
	mov	w19, w3
	mov	x20, x4
	bl	_mcount
	str	wzr, [x29, 60]
	cbnz	w19, .L531
	ldr	x0, [x21]
	cbz	x0, .L532
.L490:
	ldr	x19, .LC197
	ldr	w3, [x21, 64]
	ldrb	w0, [x19]
	and	w3, w3, -16
	str	w3, [x29, 60]
	cmp	w0, 1
	beq	.L533
	mov	x0, x20
	mov	w2, 16
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	mov	w3, 53763
	movk	w3, 0x2000, lsl 16
	str	w3, [x29, 60]
	cmp	w0, 1
	beq	.L534
	mov	x0, x20
	mov	w2, 12
	mov	w1, 2
	mov	w21, 3075
	movk	w21, 0x30, lsl 16
	bl	VDH_Record_RegData
	str	w21, [x29, 60]
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L535
	mov	x0, x20
	mov	w3, w21
	mov	w2, 60
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L536
	mov	x0, x20
	mov	w3, w21
	mov	w2, 64
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L537
	mov	x0, x20
	mov	w3, w21
	mov	w2, 68
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L538
	mov	x0, x20
	mov	w3, w21
	mov	w2, 72
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L539
	mov	x0, x20
	mov	w3, w21
	mov	w2, 76
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L540
	mov	x0, x20
	mov	w3, w21
	mov	w2, 80
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L541
	mov	x0, x20
	mov	w3, w21
	mov	w2, 84
	mov	w1, 2
	bl	VDH_Record_RegData
	mov	w0, 0
.L516:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L533:
	ldr	x0, .LC198
	ldr	x1, [x0]
	str	w3, [x1, 16]
.L493:
	ldr	x1, [x0]
	mov	w2, 53763
	movk	w2, 0x2000, lsl 16
	str	w2, [x1, 12]
.L495:
	ldr	x1, [x0]
	mov	w0, 3075
	movk	w0, 0x30, lsl 16
	str	w0, [x1, 60]
.L498:
	mov	w0, 3075
	movk	w0, 0x30, lsl 16
	str	w0, [x1, 64]
.L501:
	mov	w0, 3075
	movk	w0, 0x30, lsl 16
	str	w0, [x1, 68]
.L504:
	mov	w0, 3075
	movk	w0, 0x30, lsl 16
	str	w0, [x1, 72]
.L507:
	mov	w0, 3075
	movk	w0, 0x30, lsl 16
	str	w0, [x1, 76]
.L510:
	mov	w0, 3075
	movk	w0, 0x30, lsl 16
	str	w0, [x1, 80]
.L513:
	mov	w2, 3075
	mov	w0, 0
	movk	w2, 0x30, lsl 16
	str	w2, [x1, 84]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L532:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L491
	str	x0, [x21]
	b	.L490
	.p2align 3
.L531:
	ldr	x1, .LC195
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L516
.L491:
	ldr	x1, .LC196
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L516
.L534:
	ldr	x0, .LC198
	b	.L493
.L535:
	ldr	x0, .LC198
	b	.L495
.L537:
	ldr	x0, .LC198
	ldr	x1, [x0]
	b	.L501
.L538:
	ldr	x0, .LC198
	ldr	x1, [x0]
	b	.L504
.L536:
	ldr	x0, .LC198
	ldr	x1, [x0]
	b	.L498
.L541:
	ldr	x0, .LC198
	ldr	x1, [x0]
	b	.L513
.L540:
	ldr	x0, .LC198
	ldr	x1, [x0]
	b	.L510
.L539:
	ldr	x0, .LC198
	ldr	x1, [x0]
	b	.L507
	.size	VDMHAL_V400R003_CfgRpReg, .-VDMHAL_V400R003_CfgRpReg
	.align	3
.LC195:
	.xword	.LC194
	.align	3
.LC196:
	.xword	.LC111
	.align	3
.LC197:
	.xword	g_HalDisable
	.align	3
.LC198:
	.xword	g_HwMem
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_MakeDecReport
	.type	VDMHAL_V400R003_MakeDecReport, %function
VDMHAL_V400R003_MakeDecReport:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L572
	ldr	x22, [x19, 8]
	ldr	w23, [x19]
	ldr	w21, [x19, 16]
	cbz	x22, .L573
	sxtw	x24, w21
	ldr	x20, .LC207
	lsl	x0, x24, 6
	sub	x0, x0, x24
	lsl	x0, x0, 2
	sub	x1, x0, x24
	add	x0, x0, x1, lsl 2
	ldr	x0, [x20, x0]
	cbz	x0, .L546
	ldr	x2, .LC208
	add	x0, x2, 4
	add	x2, x2, 64
	ldr	w1, [x2, -64]
	cmp	w1, 1
	beq	.L561
	.p2align 2
.L549:
	ldr	w1, [x0]
	add	x0, x0, 4
	cmp	w1, 1
	beq	.L561
	cmp	x0, x2
	bne	.L549
	mov	w19, 0
.L547:
	ldr	x3, .LC212
	mov	x2, 816
	mov	w1, 0
	mov	x0, x22
	ldr	x3, [x3, 96]
	blr	x3
	lsl	x0, x24, 6
	lsl	x1, x24, 2
	sub	x0, x0, x24
	sub	x1, x1, x24
	lsl	x0, x0, 2
	lsl	x2, x1, 4
	sub	x3, x0, x24
	sub	x1, x2, x1
	ldr	x4, .LC213
	add	x2, x0, x3, lsl 2
	add	x2, x20, x2
	add	x1, x4, x1, lsl 2
	ldr	w0, [x2, 40]
	str	w0, [x22, 4]
	ldr	w1, [x1, 4]
	ubfx	x0, x1, 17, 2
	cmp	w0, 1
	beq	.L562
	and	w1, w1, 131072
	cmp	w1, wzr
	ccmp	w23, 3, 0, ne
	cset	w1, ne
.L550:
	lsl	x0, x24, 2
	str	w1, [x22]
	sub	x0, x0, x24
	cmp	w23, wzr
	lsl	x1, x0, 4
	ccmp	w23, 15, 4, ne
	sub	x0, x1, x0
	add	x0, x4, x0, lsl 2
	ldr	w2, [x0, 4]
	and	w2, w2, 131071
	str	w2, [x22, 8]
	beq	.L574
.L551:
	cmp	w2, 200
	bhi	.L575
.L553:
	lsl	x0, x24, 6
	sub	x0, x0, x24
	lsl	x0, x0, 2
	sub	x1, x0, x24
	add	x0, x0, x1, lsl 2
	add	x20, x20, x0
	ldr	w21, [x20, 56]
	mov	w0, w21
	bl	MEM_Phy2Vir
	cbz	x0, .L576
	lsl	x3, x24, 2
	ldr	x19, .LC218
	sub	x3, x3, x24
	ldr	w2, [x22, 8]
	mov	x1, x0
	add	x20, x24, x3, lsl 3
	lsl	w2, w2, 2
	add	x0, x19, x20, lsl 7
	bl	VDMHAL_V400R003_ReadMsgSlot
	ldr	w0, [x22, 8]
	cbz	w0, .L558
	sub	w2, w0, #1
	mov	x5, 18
	add	x1, x22, 14
	mov	w0, 2
	add	x5, x5, x2, uxtw 2
	lsl	x3, x20, 5
	add	x5, x22, x5
	.p2align 2
.L557:
	sub	w2, w0, #1
	add	x4, x3, x0, uxtw
	add	x2, x3, x2
	add	w0, w0, 4
	ldr	w2, [x19, x2, lsl 2]
	strh	w2, [x1, -2]
	ldr	w2, [x19, x4, lsl 2]
	strh	w2, [x1], 4
	cmp	x5, x1
	bne	.L557
.L558:
	mov	w0, 6
	bl	IsDprintTypeEnable
	cbnz	w0, .L577
.L544:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L574:
	cmp	w19, 1
	beq	.L578
	ldr	x1, .LC214
	ldrb	w1, [x1]
	cbnz	w1, .L551
	ldrb	w0, [x0, 19]
	and	w0, w0, 3
	cmp	w0, 2
	bne	.L551
	mov	w0, 1
	strh	wzr, [x22, 12]
	str	w0, [x22, 8]
	mov	w0, 0
	strh	wzr, [x22, 14]
	b	.L544
	.p2align 3
.L561:
	mov	w19, w1
	b	.L547
	.p2align 3
.L577:
	ldr	x1, .LC220
	mov	w0, 6
	ldr	w2, [x22, 8]
	mov	w20, 0
	bl	dprint_vfmw
	ldr	x1, .LC222
	mov	w2, w21
	mov	w0, 6
	bl	dprint_vfmw
	ldr	x23, .LC224
	lsl	x0, x24, 2
	sub	x0, x0, x24
	add	x21, x24, x0, lsl 3
	lsl	x21, x21, 5
	.p2align 2
.L559:
	add	w1, w20, 3
	add	w3, w20, 2
	add	w0, w20, 1
	add	x7, x21, x20, uxtw
	add	x1, x21, x1
	add	x3, x21, x3
	add	x0, x21, x0
	mov	w2, w20
	add	w20, w20, 4
	ldr	w6, [x19, x1, lsl 2]
	mov	x1, x23
	ldr	w5, [x19, x3, lsl 2]
	ldr	w4, [x19, x0, lsl 2]
	mov	w0, 6
	ldr	w3, [x19, x7, lsl 2]
	bl	dprint_vfmw
	ldr	w0, [x22, 8]
	lsl	w0, w0, 2
	sub	w0, w0, #3
	cmp	w0, w20
	bhi	.L559
	ldr	x1, .LC226
	mov	w0, 6
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L562:
	mov	w1, 0
	b	.L550
	.p2align 3
.L575:
	ldr	x1, .LC216
	mov	w3, 200
	mov	w0, 1
	bl	dprint_vfmw
	str	wzr, [x22, 8]
	b	.L553
	.p2align 3
.L578:
	ldr	w0, [x0, 16]
	ubfx	x0, x0, 21, 2
	cmp	w0, 2
	bne	.L551
	strh	wzr, [x22, 12]
	mov	w0, 0
	strh	wzr, [x22, 14]
	str	w19, [x22, 8]
	b	.L544
.L576:
	ldr	x2, .LC203
	ldr	x3, .LC217
	ldr	x1, .LC204
	add	x2, x2, 224
	bl	dprint_vfmw
	mov	w0, -1
	b	.L544
.L546:
	ldr	x2, .LC203
	ldr	x3, .LC209
.L570:
	ldr	x1, .LC204
	mov	w0, 0
	add	x2, x2, 224
	bl	dprint_vfmw
	mov	w0, -1
	b	.L544
.L573:
	ldr	x2, .LC203
	ldr	x3, .LC206
	b	.L570
.L572:
	ldr	x2, .LC203
	ldr	x3, .LC202
	b	.L570
	.size	VDMHAL_V400R003_MakeDecReport, .-VDMHAL_V400R003_MakeDecReport
	.align	3
.LC202:
	.xword	.LC201
	.align	3
.LC203:
	.xword	.LANCHOR1
	.align	3
.LC204:
	.xword	.LC7
	.align	3
.LC206:
	.xword	.LC205
	.align	3
.LC207:
	.xword	g_HwMem
	.align	3
.LC208:
	.xword	g_DSPState
	.align	3
.LC209:
	.xword	.LC150
	.align	3
.LC212:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC213:
	.xword	g_BackUp
	.align	3
.LC214:
	.xword	g_not_allow_H264FullPictureRepair_flag
	.align	3
.LC216:
	.xword	.LC215
	.align	3
.LC217:
	.xword	.LC172
	.align	3
.LC218:
	.xword	g_UpMsg
	.align	3
.LC220:
	.xword	.LC219
	.align	3
.LC222:
	.xword	.LC221
	.align	3
.LC224:
	.xword	.LC223
	.align	3
.LC226:
	.xword	.LC225
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_PrepareRepair
	.type	VDMHAL_V400R003_PrepareRepair, %function
VDMHAL_V400R003_PrepareRepair:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	w25, w0
	mov	x0, x30
	mov	w21, w3
	mov	x22, x1
	mov	w24, w2
	mov	x23, x4
	bl	_mcount
	ldr	x20, .LC227
	sxtw	x0, w21
	lsl	x19, x0, 6
	sub	x19, x19, x0
	lsl	x19, x19, 2
	sub	x0, x19, x0
	add	x19, x19, x0, lsl 2
	add	x19, x19, x20
	cbnz	w21, .L591
	ldr	x0, [x20]
	cbz	x0, .L592
	cbnz	w24, .L585
.L594:
	ldr	x21, .LC232
	ldr	w0, [x21, 840]
	cmp	w0, wzr
	ble	.L586
	cmp	w25, 6
	bne	.L587
	ldrb	w0, [x22, 17]
	cmp	w0, 1
	csel	w0, w0, wzr, eq
	str	w0, [x21, 1728]
.L587:
	mov	x4, x23
	mov	w3, 0
	mov	x2, x22
	mov	x1, x19
	mov	w0, w25
	mov	w20, 0
	bl	VDMHAL_V400R003_CfgRpReg
	mov	x1, x19
	mov	w2, 0
	mov	x0, x21
	bl	VDMHAL_V400R003_CfgRpMsg
	ldr	x25, [sp, 64]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L585:
	cmp	w24, 1
	mov	w20, 0
	beq	.L593
.L582:
	mov	w0, w20
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L592:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L584
	str	x0, [x20]
	cbnz	w24, .L585
	b	.L594
	.p2align 3
.L593:
	cmp	w25, 6
	bne	.L589
	ldrb	w0, [x22, 17]
	cmp	w0, 1
	bne	.L589
	ldr	x21, .LC232
	ldr	w0, [x21, 1716]
	cmp	w0, wzr
	ble	.L589
	mov	x4, x23
	mov	w5, 2
	mov	x2, x22
	str	w5, [x21, 1728]
	mov	x1, x19
	mov	w3, 0
	mov	w0, w25
	bl	VDMHAL_V400R003_CfgRpReg
	mov	x1, x19
	mov	w2, 0
	add	x0, x21, 876
	bl	VDMHAL_V400R003_CfgRpMsg
	ldr	x25, [sp, 64]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L589:
	ldr	x1, .LC237
	mov	w0, 0
	mov	w20, -1
	bl	dprint_vfmw
	b	.L582
	.p2align 3
.L591:
	ldr	x1, .LC229
	mov	w0, 0
	mov	w20, -1
	bl	dprint_vfmw
	b	.L582
.L586:
	ldr	x1, .LC234
	mov	w0, 0
	mov	w20, -1
	bl	dprint_vfmw
	b	.L582
.L584:
	ldr	x1, .LC231
	mov	w0, 0
	mov	w20, -1
	bl	dprint_vfmw
	b	.L582
	.size	VDMHAL_V400R003_PrepareRepair, .-VDMHAL_V400R003_PrepareRepair
	.align	3
.LC227:
	.xword	g_HwMem
	.align	3
.LC229:
	.xword	.LC228
	.align	3
.LC231:
	.xword	.LC230
	.align	3
.LC232:
	.xword	g_RepairParam
	.align	3
.LC234:
	.xword	.LC233
	.align	3
.LC237:
	.xword	.LC236
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_StartHwRepair
	.type	VDMHAL_V400R003_StartHwRepair, %function
VDMHAL_V400R003_StartHwRepair:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	sxtw	x20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x21, .LC238
	ldrb	w4, [x21]
	cmp	w4, 1
	bne	.L619
	cmp	w20, wzr
	ble	.L620
	ldr	x22, .LC240
	mov	w3, w20
	ldr	x23, .LC241
	mov	w0, 32
	add	x2, x22, 256
	mov	x1, x23
	bl	dprint_vfmw
	ldrb	w0, [x21]
	cmp	w0, 1
	beq	.L603
	mov	x0, x19
	mov	w3, -2
	mov	w2, 36
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x21]
	cmp	w0, 1
	beq	.L616
.L598:
	cbz	x19, .L621
.L612:
	ldr	x1, .LC244
	sbfiz	x0, x20, 6, 32
	mov	w2, 1
	str	w2, [x19, 4]
	add	x1, x1, x0
	mov	x0, x19
	ldr	w1, [x1, 8]
	str	w1, [x19, 8]
	strb	w20, [x19]
	strb	w2, [x19, 2]
	bl	VDH_PostTask
.L595:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L620:
	sxtw	x0, w20
	ldr	x3, .LC239
	lsl	x1, x0, 6
	mov	w4, 67108864
	sub	x1, x1, x0
	lsl	x1, x1, 2
	sub	x2, x1, x0
	add	x1, x1, x2, lsl 2
	ldr	x1, [x3, x1]
	str	w4, [x1, 8]
.L601:
	lsl	x1, x0, 6
	mov	w2, -2
	sub	x1, x1, x0
	lsl	x1, x1, 2
	sub	x0, x1, x0
	add	x0, x1, x0, lsl 2
	ldr	x0, [x3, x0]
	str	w2, [x0, 36]
.L599:
	ldr	x0, .LC245
	ldr	x0, [x0, 216]
	blr	x0
	cmp	w20, wzr
	ble	.L615
	ldr	x22, .LC240
	ldr	x23, .LC241
.L608:
	add	x22, x22, 272
	mov	w3, w20
	mov	x2, x22
	mov	x1, x23
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w3, w20
	mov	x2, x22
	mov	x1, x23
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	mov	w4, 1
	mov	w3, w20
	mov	x2, x22
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	b	.L595
	.p2align 3
.L597:
	cmp	w20, wzr
	ble	.L613
	ldr	x22, .LC240
	ldr	x23, .LC241
.L603:
	mov	w4, 1
	mov	w3, w20
	add	x2, x22, 256
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	ldrb	w0, [x21]
	cmp	w0, 1
	bne	.L598
.L616:
	ldr	x0, .LC245
	ldr	x0, [x0, 216]
	blr	x0
	b	.L608
	.p2align 3
.L619:
	mov	x0, x19
	mov	w3, 67108864
	mov	w2, 8
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x21]
	cmp	w0, 1
	beq	.L597
	mov	x0, x19
	mov	w3, -2
	mov	w2, 36
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x21]
	cmp	w0, 1
	beq	.L599
	cbnz	x19, .L612
.L621:
	ldr	x2, .LC240
	mov	x3, 0
	ldr	x1, .LC243
	mov	w0, 0
	add	x2, x2, 272
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L615:
	lsl	x0, x20, 6
	ldr	x1, .LC239
	sub	x0, x0, x20
	mov	w3, 1
	lsl	x0, x0, 2
	sub	x20, x0, x20
	add	x0, x0, x20, lsl 2
	ldr	x2, [x1, x0]
	str	wzr, [x2]
	ldr	x2, [x1, x0]
	str	w3, [x2]
	ldr	x0, [x1, x0]
	str	wzr, [x0]
	b	.L595
.L613:
	ldr	x3, .LC239
	sxtw	x0, w20
	b	.L601
	.size	VDMHAL_V400R003_StartHwRepair, .-VDMHAL_V400R003_StartHwRepair
	.align	3
.LC238:
	.xword	g_HalDisable
	.align	3
.LC239:
	.xword	g_HwMem
	.align	3
.LC240:
	.xword	.LANCHOR1
	.align	3
.LC241:
	.xword	.LC89
	.align	3
.LC243:
	.xword	.LC242
	.align	3
.LC244:
	.xword	g_VdmDrvParam
	.align	3
.LC245:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_CalVdhClkSkip
	.type	VDMHAL_V400R003_CalVdhClkSkip, %function
VDMHAL_V400R003_CalVdhClkSkip:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x0, x30
	bl	_mcount
	ldr	x24, .LC247
	ldr	w19, [x24, 64]
	tbnz	w19, #31, .L650
	ldr	x27, .LC250
	add	x21, x24, 68
	ldr	x26, .LC253
	add	x24, x24, 128
	ldr	x25, .LC254
	mov	w22, 30
	ldr	x28, .LC252
	mov	w23, 0
	add	x27, x27, 304
	.p2align 2
.L625:
	mov	w0, w19
	bl	VCTRL_IsChanActive
	cbnz	w0, .L651
	ldr	x2, [x26, w19, sxtw 3]
	mov	w0, w19
	ldr	x3, [x25, w19, sxtw 3]
	ldp	w1, w2, [x2, 28]
	ldr	w3, [x3, 1584]
	adds	w19, w1, 15
	add	w1, w1, 30
	csel	w19, w1, w19, mi
	add	w20, w2, 30
	adds	w2, w2, 15
	asr	w19, w19, 4
	csel	w2, w20, w2, mi
	cmp	w3, 31
	csel	w22, w22, w3, lt
	asr	w20, w2, 4
	bl	VCTRL_GetVidStd
	cmp	w0, 17
	bhi	.L627
	ldr	x1, .LC255
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx630
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx630:
	.section	.rodata
	.align	0
	.align	2
.L630:
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L627 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L627 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L627 - .Lrtx630) / 4
	.byte	(.L640 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.byte	(.L639 - .Lrtx630) / 4
	.text
	.p2align 3
.L639:
	mul	w19, w19, w20
	madd	w23, w19, w22, w23
.L627:
	cmp	x24, x21
	beq	.L648
	ldr	w19, [x21], 4
	tbz	w19, #31, .L625
.L648:
	lsl	w1, w23, 2
	mov	w0, 23813
	sub	w23, w1, w23
	movk	w0, 0xdb4f, lsl 16
	lsl	w23, w23, 6
	mov	w1, 32
	mov	w2, 25
	umull	x0, w23, w0
	lsr	x0, x0, 53
	sub	w0, w1, w0
	cmp	w0, w2
	csel	w0, w0, w2, le
	cmp	w0, wzr
	csel	w0, w0, wzr, ge
.L623:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L651:
	mov	w3, w19
	mov	x2, x27
	mov	x1, x28
	mov	w0, 1
	bl	dprint_vfmw
	b	.L627
	.p2align 3
.L640:
	mul	w19, w19, w20
	mul	w19, w19, w22
	add	w23, w23, w19, lsl 1
	b	.L627
.L650:
	mov	w0, 25
	b	.L623
	.size	VDMHAL_V400R003_CalVdhClkSkip, .-VDMHAL_V400R003_CalVdhClkSkip
	.align	3
.LC247:
	.xword	g_ChanCtx
	.align	3
.LC250:
	.xword	.LANCHOR1
	.align	3
.LC252:
	.xword	.LC251
	.align	3
.LC253:
	.xword	s_pFspInst
	.align	3
.LC254:
	.xword	s_pstVfmwChan
	.align	3
.LC255:
	.xword	.L630
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_GetVdmClk
	.type	VDMHAL_V400R003_GetVdmClk, %function
VDMHAL_V400R003_GetVdmClk:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cmp	w20, 3
	bls	.L660
.L653:
	ldr	x0, .LC258
	sbfiz	x19, x19, 6, 32
	mov	w1, 500
	add	x19, x0, x19
	mov	w0, w1
	str	w1, [x19, 16]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L660:
	ldr	x0, .LC257
	ldrb	w0, [x0,w20,uxtw]
	adr	x1, .Lrtx655
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx655:
	.section	.rodata
	.align	0
	.align	2
.L655:
	.byte	(.L653 - .Lrtx655) / 4
	.byte	(.L656 - .Lrtx655) / 4
	.byte	(.L657 - .Lrtx655) / 4
	.byte	(.L658 - .Lrtx655) / 4
	.text
	.p2align 3
.L657:
	ldr	x0, .LC258
	sbfiz	x19, x19, 6, 32
	mov	w1, 100
	add	x19, x0, x19
	mov	w0, w1
	str	w1, [x19, 16]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L656:
	ldr	x0, .LC258
	sbfiz	x19, x19, 6, 32
	mov	w1, 540
	add	x19, x0, x19
	mov	w0, w1
	str	w1, [x19, 16]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L658:
	ldr	x0, .LC258
	sbfiz	x19, x19, 6, 32
	mov	w1, 600
	add	x19, x0, x19
	mov	w0, w1
	str	w1, [x19, 16]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VDMHAL_V400R003_GetVdmClk, .-VDMHAL_V400R003_GetVdmClk
	.align	3
.LC257:
	.xword	.L655
	.align	3
.LC258:
	.xword	g_VdmDrvParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_StartHwDecode
	.type	VDMHAL_V400R003_StartHwDecode, %function
VDMHAL_V400R003_StartHwDecode:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	cmp	w20, wzr
	ble	.L662
	ldr	x2, .LC259
	mov	w4, 0
	ldr	x1, .LC260
	mov	w3, w20
	add	x2, x2, 336
	mov	w0, 0
	bl	dprint_vfmw
.L661:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L662:
	ldr	x21, .LC261
	ldrb	w0, [x21]
	cmp	w0, 1
	bne	.L697
	ldr	x1, .LC264
	ldr	x2, [x1]
	ldr	w0, [x2, 120]
	ubfx	x3, x0, 8, 2
	cbz	w3, .L667
	and	w0, w0, -769
	str	w0, [x2, 120]
.L667:
	ldr	x25, .LC262
	sxtw	x22, w20
	ldr	x24, .LC263
	mov	w2, 500
	add	x0, x25, x22, lsl 6
	ldr	w23, [x0, 8]
	str	w2, [x0, 16]
	ldr	x0, [x24, w23, sxtw 3]
	ldr	w26, [x0, 1296]
.L666:
	ldr	x0, [x1]
	ldr	w1, [x0, 120]
	and	w1, w1, -258049
	str	w1, [x0, 120]
	ldr	w1, [x0, 120]
	orr	w1, w1, 131072
	str	w1, [x0, 120]
.L668:
	ldr	x0, [x24, w23, sxtw 3]
	mov	w1, 3
	mov	x3, x19
	mov	w2, w20
	ldr	w0, [x0, 672]
	cmp	w0, 16
	mov	w0, 12
	csel	w1, w1, wzr, ne
	bl	SCD_ConfigReg
	ldrb	w0, [x21]
	cmp	w0, 1
	beq	.L692
	mov	w1, 2
	mov	x0, x19
	mov	w3, -1431655766
	mov	w2, 156
	bl	VDH_Record_RegData
	ldr	x1, .LC265
	ldrb	w0, [x21]
	ldr	w1, [x1]
	cmp	w1, 1
	beq	.L698
.L672:
	mov	w3, 0
.L671:
	cmp	w0, 1
	beq	.L699
	mov	x0, x19
	mov	w2, 61472
	mov	w1, 2
	bl	VDH_Record_RegData
	cmp	w26, 1
	ldrb	w0, [x21]
	beq	.L700
.L675:
	mov	w3, -2
.L674:
	cmp	w0, 1
	beq	.L701
	mov	x0, x19
	mov	w2, 36
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x21]
	cmp	w0, 1
	beq	.L702
	cbz	x19, .L703
	mov	w0, 1
	strb	w20, [x19]
	stp	w0, w23, [x19, 4]
	mov	x0, x19
	bl	VDH_PostTask
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L700:
	mov	w3, -6
	b	.L674
	.p2align 3
.L698:
	mov	w3, 7
	b	.L671
	.p2align 3
.L697:
	mov	w2, 0
	mov	w1, 0
	mov	x0, x19
	mov	w3, 0
	sxtw	x22, w20
	bl	VDH_Record_RegData
	ldr	x25, .LC262
	ldr	x24, .LC263
	mov	w1, 500
	add	x0, x25, x22, lsl 6
	ldrb	w2, [x21]
	cmp	w2, 1
	ldr	w23, [x0, 8]
	str	w1, [x0, 16]
	ldr	x0, [x24, w23, sxtw 3]
	ldr	w26, [x0, 1296]
	beq	.L704
	mov	w3, 0
	mov	w2, 1
	mov	w1, 0
	mov	x0, x19
	bl	VDH_Record_RegData
	b	.L668
	.p2align 3
.L692:
	ldr	x2, .LC265
	lsl	x1, x22, 6
	sub	x1, x1, x22
	ldr	x24, .LC266
	lsl	x1, x1, 2
	ldr	w2, [x2]
	cmp	w2, 1
	sub	x2, x1, x22
	add	x1, x1, x2, lsl 2
	mov	w2, -1431655766
	ldr	x1, [x24, x1]
	str	w2, [x1, 156]
	bne	.L672
	mov	w3, 7
	b	.L673
	.p2align 3
.L699:
	ldr	x24, .LC266
.L673:
	lsl	x0, x22, 6
	cmp	w26, 1
	sub	x0, x0, x22
	lsl	x0, x0, 2
	sub	x1, x0, x22
	add	x0, x0, x1, lsl 2
	ldr	x0, [x24, x0]
	add	x0, x0, 61440
	str	w3, [x0, 32]
	beq	.L688
	mov	w0, 1
	b	.L675
	.p2align 3
.L701:
	ldr	x24, .LC266
.L676:
	lsl	x1, x22, 6
	ldr	x19, .LC267
	sub	x1, x1, x22
	mov	x0, 30
	lsl	x1, x1, 2
	sub	x2, x1, x22
	add	x1, x1, x2, lsl 2
	ldr	x1, [x24, x1]
	str	w3, [x1, 36]
	ldr	x1, [x19, 224]
	blr	x1
	ldr	x0, [x19, 216]
	blr	x0
.L679:
	lsl	x0, x22, 6
	mov	w2, 1
	sub	x1, x0, x22
	add	x25, x25, x0
	lsl	x0, x1, 2
	ldr	x3, .LC269
	sub	x22, x0, x22
	mov	w1, 4
	add	x22, x0, x22, lsl 2
	ldr	x0, [x24, x22]
	str	wzr, [x0]
	ldr	x0, [x24, x22]
	str	w2, [x0]
	ldr	x0, [x24, x22]
	str	wzr, [x0]
	ldr	w2, [x25, 8]
	mov	w0, w2
	ldr	w2, [x3, w2, sxtw 2]
	bl	VDEC_Lowdelay_Event_Time
	b	.L661
	.p2align 3
.L703:
	ldr	x2, .LC259
	mov	x3, 0
	ldr	x1, .LC268
	mov	w0, 0
	add	x2, x2, 336
	bl	dprint_vfmw
	b	.L661
	.p2align 3
.L688:
	mov	w3, -6
	b	.L676
.L704:
	ldr	x1, .LC264
	b	.L666
.L702:
	ldr	x19, .LC267
	mov	x0, 30
	ldr	x1, [x19, 224]
	blr	x1
	ldr	x0, [x19, 216]
	blr	x0
	ldr	x24, .LC266
	b	.L679
	.size	VDMHAL_V400R003_StartHwDecode, .-VDMHAL_V400R003_StartHwDecode
	.align	3
.LC259:
	.xword	.LANCHOR1
	.align	3
.LC260:
	.xword	.LC73
	.align	3
.LC261:
	.xword	g_HalDisable
	.align	3
.LC262:
	.xword	g_VdmDrvParam
	.align	3
.LC263:
	.xword	s_pstVfmwChan
	.align	3
.LC264:
	.xword	g_pstRegCrg
	.align	3
.LC265:
	.xword	mask_mmu_err_int
	.align	3
.LC266:
	.xword	g_HwMem
	.align	3
.LC267:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC268:
	.xword	.LC242
	.align	3
.LC269:
	.xword	g_LowDelaySeqIndex
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_GetCharacter
	.type	VDMHAL_V400R003_GetCharacter, %function
VDMHAL_V400R003_GetCharacter:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x2, .LC270
	mov	w3, 15
	ldr	x0, .LC271
	mov	w1, 4
	str	w3, [x2]
	str	w1, [x0]
	ldp	x29, x30, [sp], 16
	ret
	.size	VDMHAL_V400R003_GetCharacter, .-VDMHAL_V400R003_GetCharacter
	.align	3
.LC270:
	.xword	g_VdmCharacter
	.align	3
.LC271:
	.xword	g_eVdmVersion
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VDMHAL_V400R003_WriteBigTitle1DYuv
	.type	VDMHAL_V400R003_WriteBigTitle1DYuv, %function
VDMHAL_V400R003_WriteBigTitle1DYuv:
	stp	x29, x30, [sp, -208]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	str	x0, [x29, 112]
	mov	x0, x30
	mov	w20, w3
	mov	x22, x1
	mov	w19, w2
	str	w4, [x29, 104]
	bl	_mcount
	ubfx	x0, x20, 29, 2
	stp	xzr, xzr, [x29, 144]
	stp	xzr, xzr, [x29, 160]
	stp	xzr, xzr, [x29, 176]
	stp	xzr, xzr, [x29, 192]
	cmp	w0, 1
	beq	.L751
	mov	w23, 2
	cmp	w0, w23
	csinc	w23, w23, wzr, ne
.L707:
	ldr	x0, [x29, 112]
	cbz	x0, .L706
	ldr	x0, .LC273
	mov	w4, 0
	add	x3, x29, 144
	mov	w2, 4194304
	mov	w1, 0
	bl	MEM_AllocMemBlock
	cbz	w0, .L709
.L711:
	ldr	x1, .LC275
	mov	w0, 1
	bl	dprint_vfmw
.L706:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L709:
	ldr	x0, .LC277
	mov	w4, 0
	add	x3, x29, 176
	mov	w2, 4194304
	mov	w1, 0
	bl	MEM_AllocMemBlock
	cbnz	w0, .L711
	lsr	w0, w19, 1
	add	w19, w19, 15
	str	w0, [x29, 108]
	lsr	w0, w20, 1
	add	w20, w20, 15
	and	w19, w19, -16
	str	w0, [x29, 120]
	and	w0, w20, -16
	mov	w3, w0
	add	w0, w19, 255
	and	w0, w0, -256
	adds	w1, w3, 31
	lsl	w0, w0, 4
	ldr	x26, [x29, 184]
	mov	w2, w0
	add	w0, w3, 62
	csel	w0, w0, w1, mi
	add	w21, w2, 15
	cmp	w2, wzr
	asr	w0, w0, 5
	csel	w21, w21, w2, lt
	cmp	w23, wzr
	asr	w21, w21, 4
	ccmp	w23, 3, 4, ne
	stp	w2, w3, [x29, 124]
	mul	w21, w0, w21
	add	x0, x26, 2097152
	str	x0, [x29, 136]
	ldr	x20, [x29, 152]
	add	x21, x22, x21, sxtw
	bne	.L712
	cbz	w3, .L788
	ldr	x27, .LC278
	mov	w28, 0
	str	wzr, [x29, 132]
	.p2align 2
.L713:
	cbz	w19, .L717
	ldr	w0, [x29, 124]
	lsr	w23, w28, 4
	ldr	w24, [x29, 132]
	mov	w25, 0
	mul	w0, w23, w0
	ubfiz	x23, x28, 8, 4
	add	x23, x0, x23
	.p2align 2
.L715:
	lsr	w1, w25, 8
	add	x0, x24, x25, uxtw
	ubfiz	x1, x1, 12, 20
	ldr	x5, [x27, 104]
	add	x1, x1, x23
	mov	x2, 256
	add	x1, x22, x1
	add	x0, x20, x0
	add	w25, w25, 256
	blr	x5
	cmp	w19, w25
	bhi	.L715
.L717:
	ldr	w0, [x29, 132]
	add	w28, w28, 1
	add	w0, w0, w19
	str	w0, [x29, 132]
	ldr	w0, [x29, 128]
	cmp	w0, w28
	bne	.L713
	ldr	w0, [x29, 128]
	ldr	x3, [x27, 88]
	lsr	w2, w0, 1
	mov	w22, w2
	ldr	x2, [x29, 112]
	mul	w1, w19, w0
	mov	x0, x20
	str	w22, [x29, 132]
	blr	x3
	cbz	w22, .L750
	ldr	w1, [x29, 124]
	mov	w25, 0
	mov	w24, 0
	asr	w0, w1, 1
	str	w0, [x29, 128]
	.p2align 2
.L721:
	cbz	w19, .L722
	ldr	w0, [x29, 128]
	lsr	w22, w24, 3
	uxtw	x23, w25
	mov	w28, 0
	mul	w0, w22, w0
	ubfiz	x22, x24, 8, 3
	add	x22, x0, x22
	.p2align 2
.L720:
	lsr	w1, w28, 8
	add	x0, x23, x28, uxtw
	ubfiz	x1, x1, 11, 21
	ldr	x5, [x27, 104]
	add	x1, x1, x22
	mov	x2, 256
	add	x1, x21, x1
	add	x0, x20, x0
	add	w28, w28, 256
	blr	x5
	cmp	w19, w28
	bhi	.L720
.L722:
	ldr	w0, [x29, 132]
	add	w24, w24, 1
	add	w25, w25, w19
	cmp	w24, w0
	bne	.L721
.L750:
	ldr	w0, [x29, 104]
	cmp	w0, 1
	beq	.L789
	ldr	w0, [x29, 104]
	cbz	w0, .L748
.L743:
	ldr	w0, [x29, 108]
	ldr	w1, [x29, 120]
	ldr	x20, [x29, 112]
	ldr	x3, [x27, 88]
	mul	w19, w0, w1
	mov	x2, x20
	mov	x0, x26
	mov	w1, w19
	blr	x3
	ldr	x3, [x27, 88]
	mov	x2, x20
	ldr	x0, [x29, 136]
	mov	w1, w19
	blr	x3
	ldr	w0, [x29, 160]
	ldr	x1, [x29, 152]
	bl	MEM_ReleaseMemBlock
	ldr	x3, [x27, 96]
	mov	x2, 32
	mov	w1, 0
	add	x0, x29, 144
	blr	x3
	ldr	w0, [x29, 192]
	ldr	x1, [x29, 184]
	bl	MEM_ReleaseMemBlock
	ldr	x3, [x27, 96]
	mov	x2, 32
	mov	w1, 0
	add	x0, x29, 176
	blr	x3
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L751:
	mov	w23, 0
	b	.L707
.L712:
	cmp	w23, 1
	ldr	w1, [x29, 128]
	cset	w0, ne
	str	w0, [x29, 100]
	cmp	w1, w0
	ldr	x27, .LC278
	bls	.L732
	ldr	w28, [x29, 100]
	.p2align 2
.L734:
	cbz	w19, .L735
	ldr	w0, [x29, 124]
	lsr	w24, w28, 4
	lsr	w23, w28, 1
	mov	w25, 0
	mul	w0, w24, w0
	mul	w23, w23, w19
	ubfiz	x24, x28, 8, 4
	add	x24, x0, x24
	.p2align 2
.L733:
	lsr	w1, w25, 8
	add	x0, x23, x25, uxtw
	ubfiz	x1, x1, 12, 20
	ldr	x5, [x27, 104]
	add	x1, x1, x24
	mov	x2, 256
	add	x1, x22, x1
	add	x0, x20, x0
	add	w25, w25, 256
	blr	x5
	cmp	w19, w25
	bhi	.L733
.L735:
	ldr	w0, [x29, 128]
	add	w28, w28, 2
	cmp	w0, w28
	bhi	.L734
.L732:
	ldr	w0, [x29, 128]
	ldr	x3, [x27, 88]
	lsr	w2, w0, 1
	mul	w1, w19, w0
	mov	w22, w2
	ldr	x2, [x29, 112]
	lsr	w1, w1, 1
	mov	x0, x20
	str	w22, [x29, 132]
	blr	x3
	cbz	w22, .L737
	ldr	w1, [x29, 124]
	mov	w25, 0
	mov	w24, 0
	asr	w0, w1, 1
	str	w0, [x29, 128]
	.p2align 2
.L739:
	cbz	w19, .L740
	ldr	w0, [x29, 128]
	lsr	w22, w24, 3
	uxtw	x23, w25
	mov	w28, 0
	mul	w0, w22, w0
	ubfiz	x22, x24, 8, 3
	add	x22, x0, x22
	.p2align 2
.L738:
	lsr	w1, w28, 8
	add	x0, x23, x28, uxtw
	ubfiz	x1, x1, 11, 21
	ldr	x5, [x27, 104]
	add	x1, x1, x22
	mov	x2, 256
	add	x1, x21, x1
	add	x0, x20, x0
	add	w28, w28, 256
	blr	x5
	cmp	w19, w28
	bhi	.L738
.L740:
	ldr	w0, [x29, 132]
	add	w24, w24, 1
	add	w25, w25, w19
	cmp	w24, w0
	bne	.L739
.L737:
	ldr	w0, [x29, 104]
	cmp	w0, 1
	beq	.L790
	ldr	w0, [x29, 104]
	cbnz	w0, .L743
	ldr	w1, [x29, 100]
	ldr	w0, [x29, 120]
	cmp	w0, w1
	bls	.L748
	.p2align 2
.L749:
	ldr	w0, [x29, 100]
	ldr	w1, [x29, 120]
	add	w0, w0, 2
	str	w0, [x29, 100]
	cmp	w1, w0
	bhi	.L749
.L748:
	ldr	x3, [x27, 96]
	mov	x2, 1048576
	ldr	x0, [x29, 136]
	mov	w1, 128
	blr	x3
	ldr	x3, [x27, 96]
	mov	x2, 1048576
	mov	w1, 128
	mov	x0, x26
	blr	x3
	b	.L743
.L790:
	ldr	w0, [x29, 120]
	cbz	w0, .L743
	ldr	w0, [x29, 100]
	lsl	w7, w19, 1
	mov	w6, 0
	mul	w0, w19, w0
	add	x20, x20, x0
	.p2align 2
.L744:
	ldr	w2, [x29, 108]
	cbz	w2, .L747
	lsr	w0, w6, 1
	mov	w1, 1
	mul	w0, w0, w2
	add	w5, w2, w0
	.p2align 2
.L745:
	sub	w3, w1, #1
	uxtw	x2, w0
	ldr	x4, [x29, 136]
	add	w0, w0, 1
	cmp	w5, w0
	ldrb	w3, [x20, x3]
	strb	w3, [x4, x2]
	ldrb	w3, [x20, w1, uxtw]
	add	w1, w1, 2
	strb	w3, [x26, x2]
	bne	.L745
.L747:
	ldr	w0, [x29, 120]
	add	w6, w6, 2
	add	x20, x20, x7
	cmp	w0, w6
	bhi	.L744
	b	.L743
.L789:
	ldr	w0, [x29, 120]
	cbz	w0, .L743
	mov	w0, 0
	mov	w6, 0
	.p2align 2
.L726:
	ldr	w1, [x29, 108]
	mov	w5, w0
	cbz	w1, .L729
	ldr	w1, [x29, 108]
	add	w5, w1, w0
	mov	w1, 1
	.p2align 2
.L727:
	sub	w3, w1, #1
	uxtw	x2, w0
	ldr	x4, [x29, 136]
	add	w0, w0, 1
	cmp	w5, w0
	ldrb	w3, [x20, x3]
	strb	w3, [x4, x2]
	ldrb	w3, [x20, w1, uxtw]
	add	w1, w1, 2
	strb	w3, [x26, x2]
	bne	.L727
.L729:
	ldr	w0, [x29, 120]
	add	w6, w6, 1
	add	x20, x20, x19
	cmp	w0, w6
	mov	w0, w5
	bne	.L726
	b	.L743
.L788:
	ldr	x27, .LC278
	mov	w1, 0
	ldr	x2, [x29, 112]
	mov	x0, x20
	ldr	x3, [x27, 88]
	blr	x3
	b	.L750
	.size	VDMHAL_V400R003_WriteBigTitle1DYuv, .-VDMHAL_V400R003_WriteBigTitle1DYuv
	.align	3
.LC273:
	.xword	.LC272
	.align	3
.LC275:
	.xword	.LC274
	.align	3
.LC277:
	.xword	.LC276
	.align	3
.LC278:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CRG_ConfigReg
	.type	CRG_ConfigReg, %function
CRG_ConfigReg:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	mov	x21, x2
	bl	_mcount
	ldr	x0, .LC279
	ldrb	w0, [x0]
	cmp	w0, 1
	bne	.L798
	cbnz	w19, .L794
	ldr	x0, .LC280
	ldr	x1, [x0]
	ldr	w0, [x1, 120]
	ubfx	x2, x0, 8, 2
	cmp	w20, w2
	beq	.L791
	bfi	w0, w20, 8, 2
	str	w0, [x1, 120]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L794:
	cmp	w19, 1
	beq	.L799
.L791:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L798:
	mov	w3, w20
	mov	w2, w19
	mov	x0, x21
	mov	w1, 0
	bl	VDH_Record_RegData
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L799:
	ldr	x0, .LC280
	ldr	x1, [x0]
	ldr	w0, [x1, 120]
	bfi	w0, w20, 12, 5
	and	w0, w0, -131073
	str	w0, [x1, 120]
	ldr	w0, [x1, 120]
	orr	w0, w0, 131072
	str	w0, [x1, 120]
	b	.L791
	.size	CRG_ConfigReg, .-CRG_ConfigReg
	.align	3
.LC279:
	.xword	g_HalDisable
	.align	3
.LC280:
	.xword	g_pstRegCrg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MFDE_ConfigReg
	.type	MFDE_ConfigReg, %function
MFDE_ConfigReg:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	uxtw	x20, w0
	mov	x0, x30
	mov	w21, w1
	sxtw	x19, w2
	mov	x22, x3
	bl	_mcount
	ldr	x0, .LC281
	ldrb	w4, [x0]
	cmp	w4, 1
	bne	.L804
	cmp	w19, wzr
	ble	.L805
	ldr	x2, .LC283
	mov	w3, w19
	ldr	x1, .LC284
	mov	w0, 32
	add	x2, x2, 256
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L805:
	lsl	x0, x19, 6
	ldr	x1, .LC282
	sub	x0, x0, x19
	lsl	x0, x0, 2
	sub	x19, x0, x19
	add	x19, x0, x19, lsl 2
	ldr	x0, [x1, x19]
	str	w21, [x0, x20]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L804:
	mov	w3, w21
	mov	w2, w20
	mov	x0, x22
	mov	w1, 2
	bl	VDH_Record_RegData
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MFDE_ConfigReg, .-MFDE_ConfigReg
	.align	3
.LC281:
	.xword	g_HalDisable
	.align	3
.LC282:
	.xword	g_HwMem
	.align	3
.LC283:
	.xword	.LANCHOR1
	.align	3
.LC284:
	.xword	.LC89
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.54339, %object
	.size	__func__.54339, 24
__func__.54339:
	.string	"VDMHAL_V400R003_OpenHAL"
	.type	__func__.54376, %object
	.size	__func__.54376, 27
__func__.54376:
	.string	"VDMHAL_V400R003_CalcFsSize"
	.zero	5
	.type	__func__.54443, %object
	.size	__func__.54443, 27
__func__.54443:
	.string	"VDMHAL_V400R003_ArrangeMem"
	.zero	5
	.type	__FUNCTION__.54444, %object
	.size	__FUNCTION__.54444, 27
__FUNCTION__.54444:
	.string	"VDMHAL_V400R003_ArrangeMem"
	.zero	5
	.type	__func__.54388, %object
	.size	__func__.54388, 35
__func__.54388:
	.string	"VDMHAL_V400R003_FillMemArrangeInfo"
	.zero	5
	.type	__func__.54473, %object
	.size	__func__.54473, 25
__func__.54473:
	.string	"VDMHAL_V400R003_ResetVdm"
	.zero	7
	.type	__func__.54486, %object
	.size	__func__.54486, 37
__func__.54486:
	.string	"VDMHAL_V400R003_SetSmmuPageTableAddr"
	.zero	3
	.type	__func__.54507, %object
	.size	__func__.54507, 26
__func__.54507:
	.string	"VDMHAL_V400R003_GlbResetX"
	.zero	6
	.type	__func__.54496, %object
	.size	__func__.54496, 25
__func__.54496:
	.string	"VDMHAL_V400R003_GlbReset"
	.zero	7
	.type	__func__.54568, %object
	.size	__func__.54568, 25
__func__.54568:
	.string	"VDMHAL_V400R003_CheckReg"
	.align	3
.LANCHOR1 = . + 0
	.type	__func__.54585, %object
	.size	__func__.54585, 28
__func__.54585:
	.string	"VDMHAL_V400R003_ReadMMUMask"
	.zero	4
	.type	__func__.54590, %object
	.size	__func__.54590, 29
__func__.54590:
	.string	"VDMHAL_V400R003_WriteMMUMask"
	.zero	3
	.type	__func__.54597, %object
	.size	__func__.54597, 27
__func__.54597:
	.string	"VDMHAL_V400R003_PrepareDec"
	.zero	5
	.type	__func__.54619, %object
	.size	__func__.54619, 27
__func__.54619:
	.string	"VDMHAL_V400R003_IsVdmReady"
	.zero	5
	.type	__func__.54624, %object
	.size	__func__.54624, 25
__func__.54624:
	.string	"VDMHAL_V400R003_IsVdmRun"
	.zero	7
	.type	__func__.54638, %object
	.size	__func__.54638, 27
__func__.54638:
	.string	"VDMHAL_V400R003_BackupInfo"
	.zero	5
	.type	__func__.54684, %object
	.size	__func__.54684, 25
__func__.54684:
	.string	"VDMHAL_V400R003_CfgRpMsg"
	.zero	7
	.type	__func__.54718, %object
	.size	__func__.54718, 30
__func__.54718:
	.string	"VDMHAL_V400R003_MakeDecReport"
	.zero	2
	.type	__func__.54889, %object
	.size	__func__.54889, 15
__func__.54889:
	.string	"MFDE_ConfigReg"
	.zero	1
	.type	__func__.54752, %object
	.size	__func__.54752, 30
__func__.54752:
	.string	"VDMHAL_V400R003_StartHwRepair"
	.zero	2
	.type	__func__.54768, %object
	.size	__func__.54768, 30
__func__.54768:
	.string	"VDMHAL_V400R003_CalVdhClkSkip"
	.zero	2
	.type	__func__.54807, %object
	.size	__func__.54807, 30
__func__.54807:
	.string	"VDMHAL_V400R003_StartHwDecode"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC4:
	ASCII(.string	"pOpenParam = NULL error!" )
	.zero	7
.LC7:
	ASCII(.string	"%s: %s\n" )
.LC9:
	ASCII(.string	"MemBaseAddr = 0 error!" )
	.zero	1
.LC11:
	ASCII(.string	"VDMHAL_V400R003_OpenHAL: Size error!" )
	.zero	3
.LC13:
	ASCII(.string	"VdhId is wrong!!!\n" )
	.zero	5
.LC17:
	ASCII(.string	"g_VdmRegVirAddr, g_VdmResetVirAddr = %p\n" )
	.zero	7
.LC19:
	ASCII(.string	"BPDRegVirAddr %p\n" )
	.zero	6
.LC22:
	ASCII(.string	"VdhId is wrong! VDMHAL_V200R003_OpenHAL\n" )
	.zero	7
.LC24:
	ASCII(.string	"!!!!!! HAL memory not enouph! need %d, have %d\n" )
.LC26:
	ASCII(.string	"image size out of range" )
.LC33:
	ASCII(.string	"VDMHAL ArrangeMem HEVC 10 bit\n" )
	.zero	1
.LC36:
	ASCII(.string	"VidStd Invalid" )
	.zero	1
.LC40:
	ASCII(.string	"VDMHAL_V400R003_CalcFsSize err!\n" )
	.zero	7
.LC42:
	ASCII(.string	"%s, need arrange, Size: %#x, Num: %#x, RefChanged: %#x\n" )
.LC44:
	ASCII(.string	"DelAllFrameMemRecord err in VDMHAL_V400R003_ArrangeMem!\n" )
	.zero	7
.LC49:
	ASCII(.string	"DFS, report event. Size: %#x, Num: %#x, RefChanged: %#x\n" )
	.zero	7
.LC51:
	ASCII(.string	"pVdmMemArrange is NULL" )
	.zero	1
.LC53:
	ASCII(.string	"DFS, no ref frame!\n" )
	.zero	4
.LC55:
	ASCII(.string	"DFS, Frame number = %d > 30, Then, Frame num = 30, \n" )
	.zero	3
.LC57:
	ASCII(.string	"VDMHAL_V400R003_ArrangeMem Mem addr is NULL" )
	.zero	4
.LC60:
	ASCII(.string	"'pVdmMemArrange' is NULL" )
	.zero	7
.LC63:
	ASCII(.string	"MemSize not enough for pmv slot" )
.LC65:
	ASCII(.string	"VDMHAL_V200R003_ArrangeMem get ChanWidth/ChanHeight failed!\n" )
	.zero	3
.LC67:
	ASCII(.string	"ImgSlotLen > ChanSlotLen" )
	.zero	7
.LC70:
	ASCII(.string	"cann't allocate img slot" )
	.zero	7
.LC73:
	ASCII(.string	"%s: VdhId(%d) > %d\n" )
	.zero	4
.LC75:
	ASCII(.string	"VdhId is wrong! VDMHAL_V400R003_ResetVdm\n" )
	.zero	6
.LC77:
	ASCII(.string	"VDMHAL_V400R003_ResetVdm: map vdm register fail, vir(reg) = (%p)\n" )
	.zero	6
.LC82:
	ASCII(.string	"%s module id %d failed!\n" )
	.zero	7
.LC84:
	ASCII(.string	"%s module id %d success!\n" )
	.zero	6
.LC89:
	ASCII(.string	"%s: WR_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\n" )
	.zero	3
.LC94:
	ASCII(.string	"%s: map vdm register 0x%x failed!\n" )
	.zero	5
.LC98:
	ASCII(.string	"%s VdhId %d failed!\n" )
	.zero	3
.LC100:
	ASCII(.string	"%s VdhId %d success!\n" )
	.zero	2
.LC102:
	ASCII(.string	"GLB Reset failed, wrong VDH ID!\n" )
	.zero	7
.LC108:
	ASCII(.string	"VdhId is wrong! VDMHAL_V200R003_ClearIntState\n" )
	.zero	1
.LC111:
	ASCII(.string	"vdm register virtual address not mapped, reset failed!\n" )
.LC116:
	ASCII(.string	"VdhId is wrong! VDMHAL_V200_MaskInt\n" )
	.zero	3
.LC122:
	ASCII(.string	"VdhId is wrong! VDMHAL_V200R003_EnableInt\n" )
	.zero	5
.LC127:
	ASCII(.string	"%s: VdhId(%d) Invalid!\n" )
.LC130:
	ASCII(.string	"%s: VdhId is wrong!\n" )
	.zero	3
.LC132:
	ASCII(.string	"%s: vdm register virtual address not mapped, reset failed!\n" )
	.zero	4
.LC134:
	ASCII(.string	"%s: unkown reg_id = %d\n" )
.LC139:
	ASCII(.string	"%s: RD_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\n" )
	.zero	3
.LC145:
	ASCII(.string	"%s: pDecParam(%p) = NULL\n" )
	.zero	6
.LC150:
	ASCII(.string	"VDM register not mapped yet!" )
	.zero	3
.LC156:
	ASCII(.string	"VDM register not mapped yet!\n" )
	.zero	2
.LC160:
	ASCII(.string	"pBackUpInfo = NULL!" )
	.zero	4
.LC165:
	ASCII(.string	"VdhId is wrong! VDMHAL_V200R003_BackupInfo\n" )
	.zero	4
.LC172:
	ASCII(.string	"can NOT map vir addr for up-msg" )
.LC176:
	ASCII(.string	"ReadUpMsgSlot error! pDst=%p, pSrc=%p\n" )
	.zero	1
.LC178:
	ASCII(.string	"ReadUpMsgSlot error! upmsg_size(%d) > 512\n" )
	.zero	5
.LC181:
	ASCII(.string	"WriteMsgSlot error!\n" )
	.zero	3
.LC184:
	ASCII(.string	"can not map repair msg virtual address!" )
.LC188:
	ASCII(.string	"ValidGroupNum=%d out of range!\n" )
.LC190:
	ASCII(.string	"align_mb error\n" )
.LC192:
	ASCII(.string	"[%s][%d]sclie_num is wrong! %d \n" )
	.zero	7
.LC194:
	ASCII(.string	"VdhId is wrong! VDMHAL_V400_CfgRpReg\n" )
	.zero	2
.LC201:
	ASCII(.string	"'pMakeDecReport' is NULL" )
	.zero	7
.LC205:
	ASCII(.string	"'pDecReport' is NULL" )
	.zero	3
.LC215:
	ASCII(.string	"pDecReport->DecSliceNum(%d) > %d, set to 0 for full repair.\n" )
	.zero	3
.LC219:
	ASCII(.string	"\n***** UpMsg DecSliceNum=%d\n" )
	.zero	3
.LC221:
	ASCII(.string	"\n***** Up Msg (phy addr: %#8x) *****\n" )
	.zero	2
.LC223:
	ASCII(.string	"\n0x%02x 0x%08x 0x%08x 0x%08x 0x%08x\n" )
	.zero	3
.LC225:
	ASCII(.string	"\n***** Up Msg print finished *****\n" )
	.zero	4
.LC228:
	ASCII(.string	"VdhId is wrong! VDMHAL_V200R003_PrepareRepair\n" )
	.zero	1
.LC230:
	ASCII(.string	"vdm register virtual address not mapped, VDMHAL_V200R003_PrepareRepair failed!\n" )
.LC233:
	ASCII(.string	"FIRST_REPAIR Parameter Error!\n" )
	.zero	1
.LC236:
	ASCII(.string	"SECOND_REPAIR Parameter Error!\n" )
.LC242:
	ASCII(.string	"%s: pMfdeTask(%p) = NULL\n" )
	.zero	6
.LC251:
	ASCII(.string	"%s The channel %d is not active\n" )
	.zero	7
.LC272:
	ASCII(.string	"BigTile1d_y" )
	.zero	4
.LC274:
	ASCII(.string	"failed mem_allocMemBlock BigTile_yuv save!\n" )
	.zero	4
.LC276:
	ASCII(.string	"BigTile1d_uv" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
