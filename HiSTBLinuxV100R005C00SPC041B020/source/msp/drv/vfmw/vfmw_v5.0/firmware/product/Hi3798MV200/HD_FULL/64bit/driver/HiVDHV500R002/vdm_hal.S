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
	mov	w26, w1
	mov	w23, w2
	mov	w21, w3
	mov	w27, w4
	mov	x25, x5
	bl	_mcount
	sub	w1, w26, #32
	mov	w0, 8160
	cmp	w1, w0
	bhi	.L26
	sub	w1, w23, #32
	mov	w7, w23
	cmp	w1, w0
	bhi	.L26
	add	w0, w26, 15
	add	w13, w23, 15
	asr	w6, w0, 4
	ldr	x0, .LC30
	asr	w13, w13, 4
	cmp	w6, 45
	mov	w1, 64
	lsl	w28, w6, 4
	ldrb	w2, [x0]
	mov	w0, 36
	ccmp	w13, w0, 0, le
	cset	w0, le
	cmp	w0, wzr
	mov	w0, 32
	csel	w0, w0, w1, eq
	cmp	w2, 1
	csel	w0, w0, w1, ne
	cmp	w27, 16
	beq	.L31
	cmp	w27, 17
	beq	.L59
	mul	w0, w0, w6
	ldr	x4, .LC32
	add	w28, w28, 255
	add	w7, w23, 31
	and	w28, w28, -256
	mul	w0, w0, w13
	and	w7, w7, -32
	mov	w9, 0
	add	w0, w0, 143
	mov	w10, 0
	and	w22, w0, -128
	ldr	x0, [x4, w24, sxtw 3]
.L34:
	ldr	w1, [x0, 1312]
	cmp	w1, 1
	beq	.L60
.L57:
	asr	w12, w22, 1
	mov	w11, 0
	mov	w8, 0
	mov	w5, 0
.L37:
	cmp	w27, wzr
	sub	w6, w27, #16
	ccmp	w27, 3, 4, ne
	mov	w19, 1024
	ccmp	w6, 1, 0, ne
	cset	w1, hi
	cbnz	w1, .L39
	adds	w19, w23, 63
	add	w23, w23, 126
	csel	w19, w23, w19, mi
	and	w19, w19, -64
	cmp	w19, wzr
	add	w3, w19, 7
	csel	w19, w3, w19, lt
	add	w3, w26, 510
	asr	w2, w19, 3
	adds	w19, w26, 255
	csel	w19, w3, w19, mi
	asr	w19, w19, 8
	lsl	w19, w19, 6
	mul	w19, w2, w19
	add	w19, w19, 1024
.L39:
	cmp	w21, 1
	mul	w21, w28, w7
	beq	.L61
	mul	w1, w10, w9
	lsl	w20, w21, 2
	lsl	w3, w5, 2
	lsl	w2, w1, 2
	sub	w20, w20, w21
	sub	w2, w2, w1
	lsr	w20, w20, 1
	sub	w3, w3, w5
	add	w1, w20, w2, lsr 1
	add	w20, w1, w3, lsr 1
.L41:
	add	x1, x0, 323584
	ldr	w1, [x1, 2632]
	cbz	w1, .L62
	add	x2, x0, 327680
	cmp	w27, 17
	str	w8, [x2, 1916]
	str	w11, [x2, 1920]
	str	w12, [x2, 2116]
	str	w20, [x2, 2144]
	beq	.L43
.L65:
	add	x3, x0, 323584
	mov	w7, 20
	ldr	w1, [x3, 2640]
	cmp	w1, w7
	csel	w1, w1, w7, le
	str	w1, [x2, 2120]
	ldr	w1, [x0, 692]
	cmp	w1, 2
	beq	.L63
.L44:
	lsl	w28, w28, 4
	add	w1, w5, w21
	cmp	w6, 1
	str	w28, [x2, 872]
	str	w1, [x2, 1940]
	bls	.L64
.L45:
	ldr	x0, [x4, w24, sxtw 3]
	mov	w5, 1024
	add	w1, w20, w19
	add	x3, x0, 327680
	add	w1, w1, 32
	add	w2, w1, w22
	add	w6, w2, w5
	str	w22, [x3, 2148]
	str	w5, [x3, 2156]
	ldrsb	w0, [x0, 168]
	cmp	w0, 1
	csel	w2, w6, w2, eq
	cbz	x25, .L53
	str	w1, [x25, 4]
	mov	w0, 0
	ldr	x3, [x4, w24, sxtw 3]
	add	x3, x3, 323584
	ldr	w3, [x3, 2640]
	stp	w3, w22, [x25, 8]
	ldr	x3, [x4, w24, sxtw 3]
	add	x3, x3, 327680
	ldr	w3, [x3, 2120]
	stp	w3, w5, [x25, 16]
	str	w2, [x25]
	ldr	x2, [x4, w24, sxtw 3]
	add	x2, x2, 327680
	str	w1, [x2, 2152]
.L55:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L62:
	ldr	x1, .LC31
	add	x0, x0, 327680
	mov	x2, 1288
	add	x0, x0, 872
	str	x4, [x29, 104]
	ldr	x3, [x1, 96]
	mov	w1, 0
	stp	w12, w6, [x29, 112]
	stp	w5, w9, [x29, 120]
	stp	w10, w8, [x29, 128]
	str	w11, [x29, 136]
	blr	x3
	ldr	x4, [x29, 104]
	cmp	w27, 17
	ldp	w12, w6, [x29, 112]
	ldp	w10, w8, [x29, 128]
	ldr	x0, [x4, w24, sxtw 3]
	ldr	w11, [x29, 136]
	add	x2, x0, 327680
	ldp	w5, w9, [x29, 120]
	str	w8, [x2, 1916]
	str	w11, [x2, 1920]
	str	w12, [x2, 2116]
	str	w20, [x2, 2144]
	bne	.L65
.L43:
	mov	w1, 1
	lsl	w28, w28, 4
	str	w1, [x2, 2120]
	cmp	w6, 1
	add	w1, w5, w21
	str	w28, [x2, 872]
	str	w1, [x2, 1940]
	bhi	.L45
.L64:
	ldr	w0, [x0, 1604]
	cmp	w0, 8
	ble	.L45
	lsl	w7, w21, 2
	ldr	x1, .LC34
	sub	w7, w7, w21
	lsl	w21, w5, 2
	sub	w21, w21, w5
	lsr	w5, w5, 1
	lsr	w21, w21, 1
	mov	w0, 31
	madd	w9, w10, w9, w5
	add	w7, w21, w7, lsr 1
	lsl	w10, w10, 4
	str	w7, [x2, 1932]
	str	w10, [x2, 1924]
	str	w9, [x2, 1936]
	str	x4, [x29, 136]
	bl	dprint_vfmw
	ldr	x4, [x29, 136]
	b	.L45
	.p2align 3
.L31:
	mul	w0, w13, w28
	ldr	x4, .LC32
	add	w28, w26, 255
	add	w0, w0, 143
	and	w28, w28, -256
	and	w22, w0, -128
	ldr	x0, [x4, w24, sxtw 3]
	ldr	w1, [x0, 1604]
	cmp	w1, 8
	ble	.L66
	ldr	w1, [x0, 1312]
	add	w9, w23, 31
	and	w9, w9, -32
	lsr	w10, w28, 2
	cmp	w1, 1
	bne	.L57
	.p2align 2
.L60:
	cmp	w27, 17
	mov	w1, 1920
	ccmp	w26, w1, 0, ne
	ble	.L57
	add	w2, w23, 63
	asr	w12, w22, 1
.L49:
	adds	w8, w26, 2047
	add	w1, w26, 4094
	csel	w1, w1, w8, mi
	add	w5, w2, 63
	and	w1, w1, -2048
	cmp	w1, wzr
	add	w8, w1, 127
	csel	w8, w8, w1, lt
	cmp	w2, wzr
	csel	w2, w5, w2, lt
	asr	w8, w8, 7
	asr	w2, w2, 6
	lsl	w5, w8, 5
	mul	w5, w2, w5
	mov	w11, w5
	b	.L37
	.p2align 3
.L61:
	lsl	w1, w13, 6
	sub	w1, w1, w13, lsl 4
	mul	w20, w1, w28
	b	.L41
	.p2align 3
.L59:
	ldr	x4, .LC32
	add	w28, w26, 255
	add	w2, w23, 63
	and	w28, w28, -256
	and	w7, w2, -64
	ldr	x0, [x4, w24, sxtw 3]
	ldr	w1, [x0, 1604]
	cmp	w1, 8
	bgt	.L47
	mov	w9, 0
	mov	w10, 0
.L35:
	ldr	w1, [x0, 1312]
	mov	w12, 64
	movk	w12, 0x12, lsl 16
	cmp	w1, 1
	beq	.L54
	mov	w22, 128
	mov	w11, 0
	mov	w8, 0
	movk	w22, 0x24, lsl 16
	mov	w5, 0
	b	.L37
	.p2align 3
.L66:
	mov	w9, 0
	mov	w10, 0
	b	.L34
	.p2align 3
.L63:
	ldr	w1, [x3, 2644]
	str	w1, [x2, 2120]
	b	.L44
	.p2align 3
.L53:
	mov	w0, 0
	b	.L55
	.p2align 3
.L47:
	add	w9, w7, 63
	lsr	w10, w28, 2
	and	w9, w9, -64
	b	.L35
	.p2align 3
.L26:
	ldr	x2, .LC28
	mov	w0, 0
	ldr	x3, .LC27
	ldr	x1, .LC29
	add	x2, x2, 24
	bl	dprint_vfmw
	mov	w0, -1
	b	.L55
	.p2align 3
.L54:
	mov	w22, 128
	movk	w22, 0x24, lsl 16
	b	.L49
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
	stp	x29, x30, [sp, -208]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	w23, w0
	mov	x0, x30
	mov	w25, w7
	mov	w20, w1
	mov	w24, w2
	mov	w22, w3
	mov	w21, w4
	mov	w26, w6
	ldr	x19, [x29, 208]
	str	w5, [x29, 136]
	bl	_mcount
	add	x0, x29, 152
	cmn	w25, #2
	str	xzr, [x29, 168]
	stp	xzr, xzr, [x0]
	beq	.L147
	tbnz	w25, #31, .L148
	mov	w0, w25
	bl	VCTRL_GetVidStd
	mov	w27, w0
	ldr	x10, .LC35
	ldr	x0, [x10, w25, sxtw 3]
	ldr	w1, [x0, 1552]
	cmp	w1, 1
	beq	.L205
.L70:
	cbz	w23, .L206
	sub	w1, w24, #32
	mov	w0, 8160
	cmp	w1, w0
	bhi	.L96
	sub	w1, w22, #32
	cmp	w1, w0
	bhi	.L96
	cbz	x19, .L207
	ldr	x0, .LC47
	cmp	w21, 20
	mov	x2, 1272
	mov	w1, 0
	mov	w28, 20
	ldr	x3, [x0, 96]
	mov	x0, x19
	csel	w28, w21, w28, le
	add	w21, w24, 15
	asr	w21, w21, 4
	blr	x3
	ldr	x0, .LC62
	add	w3, w22, 15
	cmp	w21, 45
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
	lsl	w2, w21, 4
	beq	.L208
	cmp	w27, 17
	beq	.L104
	mul	w21, w0, w21
	mul	w21, w21, w3
	add	w21, w21, 127
	and	w21, w21, -128
	mul	w5, w28, w21
	cmp	w20, w5
	ble	.L102
	add	w2, w2, 255
	sub	w5, w20, w5
	asr	w0, w21, 1
	and	w2, w2, -256
	add	w20, w23, 1023
	str	w0, [x19, 1244]
	and	w20, w20, -1024
	str	w28, [x19, 1248]
	lsl	w0, w2, 4
	str	w0, [x19]
	add	w4, w22, 31
	sub	w28, w20, w23
	and	w4, w4, -32
	sub	w5, w5, w28
	tbnz	w25, #31, .L209
	ldr	x10, .LC35
	add	w11, w22, 63
	add	w7, w24, 255
	mov	w9, 0
	mov	w8, 0
	ldr	x0, [x10, w25, sxtw 3]
	ldr	w0, [x0, 1312]
	cmp	w0, 1
	beq	.L109
.L222:
	cmp	w11, wzr
	add	w0, w11, 63
	csel	w0, w0, w11, lt
	mov	w1, 0
	asr	w0, w0, 6
	mov	w12, 0
	mov	w10, 0
.L108:
	cmp	w7, wzr
	add	w24, w7, 255
	csel	w24, w24, w7, lt
	lsl	w0, w0, 6
	asr	w24, w24, 8
	cmp	w0, wzr
	str	w1, [x19, 1048]
	lsl	w1, w24, 6
	add	w24, w0, 7
	str	w12, [x19, 1044]
	csel	w24, w24, w0, lt
	str	w1, [x19, 1032]
	asr	w24, w24, 3
	cmp	w26, 1
	mul	w24, w24, w1
	str	w24, [x19, 1036]
	beq	.L210
	mul	w1, w2, w4
	mul	w3, w8, w9
	lsl	w0, w1, 2
	lsl	w22, w10, 2
	sub	w1, w0, w1
	lsl	w0, w3, 2
	sub	w0, w0, w3
	sub	w22, w22, w10
	asr	w0, w0, 1
	add	w0, w0, w1, asr 1
	add	w22, w0, w22, lsr 1
.L112:
	tbnz	w25, #31, .L113
	mov	w0, w25
	stp	w9, w8, [x29, 104]
	stp	w4, w5, [x29, 112]
	stp	w10, w2, [x29, 120]
	bl	VCTRL_GetChanWidth
	mov	w1, w0
	mov	w0, w25
	str	w1, [x29, 128]
	bl	VCTRL_GetChanHeight
	ldr	w1, [x29, 128]
	ldp	w9, w8, [x29, 104]
	cmn	w1, #1
	ccmn	w0, #1, 4, ne
	ldp	w4, w5, [x29, 112]
	ldp	w10, w2, [x29, 120]
	beq	.L211
	add	w1, w1, 15
	cmp	w26, 1
	and	w3, w1, -16
	add	w0, w0, 15
	add	w7, w3, 255
	and	w0, w0, -16
	and	w7, w7, -256
	beq	.L212
	ldr	x1, .LC35
	ldr	x1, [x1, w25, sxtw 3]
	ldr	w1, [x1, 1312]
	cmp	w1, 1
	beq	.L117
	add	w1, w0, 126
	adds	w0, w0, 63
	csel	w0, w1, w0, mi
	mov	w1, 0
	asr	w0, w0, 6
.L118:
	mul	w0, w7, w0
	lsl	w3, w0, 2
	sub	w0, w3, w0
	lsl	w0, w0, 6
	add	w0, w1, w0, asr 1
.L116:
	cmp	w22, w0
	bgt	.L213
.L113:
	mul	w2, w2, w4
	sub	w0, w27, #16
	cmp	w0, 1
	add	w0, w2, w10
	str	w0, [x19, 1068]
	bls	.L214
	ldr	w1, [x29, 136]
	cbz	w1, .L122
	cmp	w27, wzr
	ccmp	w27, 3, 4, ne
	beq	.L120
	sdiv	w4, w5, w22
	cmp	w1, 32
	mov	w0, 32
	str	wzr, [x19, 1040]
	csel	w27, w1, w0, le
	cmp	w4, w27
	csel	w4, w4, w27, le
	str	w4, [x19, 1080]
.L123:
	cmp	w4, wzr
	beq	.L121
	ble	.L215
	sub	w0, w4, #1
	mov	x2, 20
	mov	w1, 12
	ldr	w3, [x19, 1252]
	umaddl	x1, w0, w1, x2
	uxtw	x2, w0
	add	x0, x19, 8
	add	x1, x19, x1
	.p2align 2
.L126:
	str	w20, [x0], 12
	add	w20, w20, w22
	cmp	x1, x0
	bne	.L126
	madd	w22, w22, w4, w28
	add	x2, x2, 99
	add	w0, w4, w3
	add	w1, w22, w23
	str	w0, [x19, 1252]
	add	x2, x19, x2, lsl 2
	add	x0, x19, 392
	.p2align 2
.L128:
	str	w1, [x0], 4
	add	w1, w1, 32
	cmp	x0, x2
	bne	.L128
.L139:
	ldr	w3, [x19, 1040]
	add	w22, w22, w4, lsl 5
	add	w1, w23, w22
	mov	w0, 0
	cbz	w3, .L130
	.p2align 2
.L173:
	add	x2, x19, x0, sxtw 2
	add	w0, w0, 1
	cmp	w0, w3
	str	w1, [x2, 904]
	add	w1, w1, w24
	bne	.L173
	mul	w3, w3, w24
.L130:
	ldr	w5, [x19, 1248]
	add	w22, w22, w3
	add	w23, w23, w22
	cmp	w5, wzr
	ble	.L136
	sub	w0, w5, #1
	mov	x1, 1092
	ldr	w3, [x19, 1256]
	mov	w2, w23
	add	x0, x1, x0, uxtw 3
	add	x1, x19, 1084
	add	x0, x19, x0
	.p2align 2
.L135:
	str	w2, [x1], 8
	add	w2, w2, w21
	cmp	x0, x1
	bne	.L135
	add	w0, w5, w3
	str	w0, [x19, 1256]
.L136:
	madd	w22, w5, w21, w22
	str	w22, [x19, 4]
	tbnz	w25, #31, .L134
	ldr	x0, .LC35
	ldr	x0, [x0, w25, sxtw 3]
	cbz	x0, .L134
	cmp	w4, wzr
	ble	.L134
	sub	w3, w4, #1
	mov	w5, 12
	mov	x2, 532
	add	w0, w22, w23
	add	x1, x19, 520
	umaddl	x2, w3, w5, x2
	add	x2, x19, x2
	.p2align 2
.L137:
	str	w0, [x1], 12
	add	w0, w0, 1024
	cmp	x1, x2
	bne	.L137
.L134:
	add	w22, w22, w4, lsl 10
	mov	w1, 0
	str	w22, [x19, 4]
.L190:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, w1
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L148:
	mov	w27, 21
	b	.L70
	.p2align 3
.L205:
	cmp	w27, 20
	bhi	.L216
	add	x0, x0, 323584
	add	x5, x29, 152
	mov	w4, w27
	mov	w3, w26
	mov	w2, w22
	mov	w1, w24
	ldr	w20, [x0, 2636]
	mov	w0, w25
	str	x10, [x29, 136]
	bl	VDMHAL_V400R003_CalcFsSize
	ldr	x10, [x29, 136]
	cbnz	w0, .L217
	ldr	x23, [x10, w25, sxtw 3]
	ldr	w21, [x29, 152]
	add	x1, x23, 323584
	cmp	w20, w21
	ldr	w5, [x1, 2656]
	ldr	w20, [x1, 2640]
	beq	.L218
.L75:
	ldr	x2, .LC38
	mov	w4, w20
	ldr	x1, .LC43
	mov	w3, w21
	add	x2, x2, 88
	mov	w0, 31
	str	x10, [x29, 136]
	bl	dprint_vfmw
	ldr	x10, [x29, 136]
	ldr	x1, [x10, w25, sxtw 3]
	add	x1, x1, 323584
	ldr	w0, [x1, 2632]
	str	w21, [x1, 2636]
	cbz	w0, .L219
	ldr	x2, .LC46
	ldr	w0, [x29, 160]
	str	w0, [x1, 2644]
	ldr	x4, [x2]
	cbz	x4, .L220
	ldr	w1, [x29, 156]
	mov	w3, 28
	ldr	w2, [x29, 168]
	stp	w0, w1, [x29, 176]
	ldr	w1, [x29, 164]
	ldr	w0, [x29, 172]
	stp	w1, w0, [x29, 188]
	str	w2, [x29, 184]
	mov	w1, 122
	stp	w24, w22, [x29, 196]
	add	x2, x29, 176
	mov	w0, w25
	str	x10, [x29, 136]
	blr	x4
	ldr	x22, .LC47
	ldr	x10, [x29, 136]
.L85:
	ldr	x4, [x10, w25, sxtw 3]
	mov	w3, w20
	ldr	x1, .LC50
	mov	w2, w21
	add	x4, x4, 323584
	mov	w0, 31
	str	x10, [x29, 136]
	ldr	w4, [x4, 2656]
	bl	dprint_vfmw
	ldr	x10, [x29, 136]
	ldr	x0, [x22]
	ldr	x19, [x10, w25, sxtw 3]
	blr	x0
	add	x19, x19, 327680
	mov	w1, 2
	str	w0, [x19, 2160]
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L214:
	lsl	w0, w2, 2
	sub	w2, w0, w2
	lsl	w0, w10, 2
	sub	w0, w0, w10
	lsr	w10, w10, 1
	lsr	w0, w0, 1
	add	w0, w0, w2, asr 1
	str	w0, [x19, 1060]
	madd	w9, w8, w9, w10
	ldr	w0, [x29, 136]
	lsl	w8, w8, 4
	str	w9, [x19, 1064]
	str	w8, [x19, 1052]
	cbnz	w0, .L120
.L122:
	str	wzr, [x19, 1040]
	str	wzr, [x19, 1080]
.L121:
	ldr	x2, .LC38
	ldr	x3, .LC70
.L200:
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
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L147:
	mov	w27, 16
	b	.L70
	.p2align 3
.L120:
	ldr	w1, [x29, 136]
	add	w4, w22, w24
	mov	w0, 32
	sdiv	w4, w5, w4
	cmp	w1, 32
	csel	w27, w1, w0, le
	cmp	w4, w27
	csel	w4, w4, w27, le
	str	w4, [x19, 1040]
	str	w4, [x19, 1080]
	b	.L123
	.p2align 3
.L208:
	mul	w21, w3, w2
	add	w21, w21, 127
	and	w21, w21, -128
	mul	w5, w28, w21
	cmp	w20, w5
	ble	.L102
	sub	w5, w20, w5
	add	w20, w23, 1023
	and	w20, w20, -1024
	asr	w0, w21, 1
	str	w28, [x19, 1248]
	add	w7, w24, 255
	str	w0, [x19, 1244]
	sub	w28, w20, w23
	and	w2, w7, -256
	sub	w5, w5, w28
	tbnz	w25, #31, .L221
	ldr	x10, .LC35
	stp	w7, w5, [x29, 112]
	stp	w3, w2, [x29, 120]
	ldr	x0, [x10, w25, sxtw 3]
	str	x10, [x29, 128]
	add	x0, x0, 327680
	add	x0, x0, 2760
	bl	IsMain10Profile
	ldp	w7, w5, [x29, 112]
	ldp	w3, w2, [x29, 120]
	ldr	x10, [x29, 128]
	cbz	w0, .L106
	cmp	w2, wzr
	add	w8, w2, 3
	csel	w8, w8, w2, lt
	add	w9, w22, 31
	asr	w8, w8, 2
	and	w9, w9, -32
	add	w11, w22, 63
	mov	w4, w22
	b	.L107
	.p2align 3
.L210:
	lsl	w0, w3, 4
	mul	w0, w0, w2
	lsl	w22, w0, 2
	sub	w22, w22, w0
	b	.L112
	.p2align 3
.L104:
	lsl	w0, w28, 21
	add	w0, w0, w28, lsl 18
	cmp	w20, w0
	ble	.L102
	sub	w0, w20, w0
	add	w20, w23, 1023
	and	w20, w20, -1024
	mov	w1, 1179648
	str	w28, [x19, 1248]
	add	w11, w22, 63
	str	w1, [x19, 1244]
	add	w7, w24, 255
	sub	w28, w20, w23
	and	w2, w7, -256
	and	w4, w11, -64
	sub	w5, w0, w28
	mov	w21, 2359296
	tbnz	w25, #31, .L143
	ldr	x10, .LC35
	ldr	x0, [x10, w25, sxtw 3]
	ldr	w0, [x0, 1604]
	cmp	w0, 8
	ble	.L152
	cmp	w2, wzr
	add	w8, w2, 3
	csel	w8, w8, w2, lt
	add	w9, w4, 63
	asr	w8, w8, 2
	and	w9, w9, -64
	mov	w21, 2359296
.L107:
	lsl	w0, w2, 4
	str	w0, [x19]
	ldr	x0, [x10, w25, sxtw 3]
	ldr	w0, [x0, 1312]
	cmp	w0, 1
	bne	.L222
.L109:
	cmp	w27, 17
	mov	w0, 1920
	ccmp	w24, w0, 0, ne
	bgt	.L110
	add	w1, w11, 63
	cmp	w11, wzr
	csel	w0, w1, w11, lt
	mov	w12, 0
	asr	w0, w0, 6
	mov	w1, 0
	mov	w10, 0
	b	.L108
	.p2align 3
.L221:
	add	w11, w22, 63
	mov	w4, w22
.L143:
	add	w1, w11, 63
	cmp	w11, wzr
	csel	w0, w1, w11, lt
	lsl	w1, w2, 4
	asr	w0, w0, 6
	str	w1, [x19]
.L199:
	mov	w1, 0
	mov	w12, 0
	mov	w9, 0
	mov	w8, 0
	mov	w10, 0
	b	.L108
	.p2align 3
.L117:
	adds	w11, w0, 63
	add	w1, w0, 126
	csel	w1, w1, w11, mi
	asr	w0, w1, 6
	adds	w1, w3, 2047
	add	w3, w3, 4094
	csel	w3, w3, w1, mi
	and	w3, w3, -2048
	cmp	w3, wzr
	add	w1, w3, 127
	csel	w1, w1, w3, lt
	lsl	w3, w0, 2
	sub	w3, w3, w0
	asr	w1, w1, 7
	lsl	w3, w3, 5
	mul	w1, w1, w3
	lsr	w1, w1, 1
	b	.L118
	.p2align 3
.L212:
	mul	w0, w0, w7
	lsl	w1, w0, 2
	sub	w0, w1, w0
	b	.L116
	.p2align 3
.L110:
	adds	w12, w24, 2047
	add	w24, w24, 4094
	csel	w12, w24, w12, mi
	add	w1, w11, 63
	and	w12, w12, -2048
	cmp	w12, wzr
	add	w0, w12, 127
	csel	w12, w0, w12, lt
	cmp	w11, wzr
	csel	w0, w1, w11, lt
	asr	w12, w12, 7
	asr	w0, w0, 6
	lsl	w10, w12, 5
	mul	w10, w10, w0
	mov	w1, w10
	b	.L108
.L215:
	madd	w22, w22, w4, w28
	b	.L139
.L218:
	cmp	w5, 1
	beq	.L75
	ldr	w1, [x1, 2632]
	cmp	w1, 1
	beq	.L75
	cbz	x19, .L223
	add	x26, x23, 327680
	mov	x0, x23
	add	x21, x26, 872
	ldr	w24, [x26, 2144]
	cbz	w20, .L224
.L87:
	add	x0, x0, 323584
	ldr	w2, [x0, 2644]
	cmp	w2, 30
	bgt	.L225
.L88:
	ldr	x22, .LC47
	add	x20, x26, 872
	mov	w0, 8
	str	w2, [x20, 1080]
	str	x10, [x29, 136]
	ldr	x1, [x22, 24]
	blr	x1
	ldr	x10, [x29, 136]
	ldr	x7, [x10, w25, sxtw 3]
	add	x9, x7, 327680
	ldr	w0, [x9, 2184]
	cbz	w0, .L93
	ldr	w12, [x20, 1036]
	mov	x4, x20
	ldr	w11, [x20, 1248]
	mov	w5, 0
	ldr	w6, [x20, 1252]
	mov	w8, 1
	.p2align 2
.L92:
	uxtw	x1, w5
	uxtw	x0, w6
	lsl	x13, x1, 6
	lsl	x2, x0, 2
	add	x13, x13, x1, lsl 3
	sub	x0, x2, x0
	add	x13, x7, x13
	add	x2, x23, x2
	add	x13, x13, 323584
	add	x0, x23, x0, lsl 2
	add	x0, x0, 327680
	add	x14, x2, 327680
	add	w6, w6, 1
	add	w5, w5, 1
	ldr	w3, [x13, 2696]
	mov	x1, x13
	ldr	w15, [x13, 2676]
	add	w2, w3, 1023
	strb	w8, [x0, 888]
	str	w15, [x0, 884]
	and	w2, w2, -1024
	str	w2, [x0, 880]
	sub	w0, w24, w3
	add	w0, w0, w2
	add	w2, w3, w12
	add	w3, w3, w0
	add	w0, w2, w0
	str	w3, [x14, 1776]
	str	w0, [x14, 1264]
	str	w6, [x4, 1252]
	ldr	w0, [x13, 2668]
	cmp	w0, 255
	beq	.L90
	ldr	w0, [x4, 1260]
	ldr	w13, [x13, 2712]
	add	w3, w0, 1
	uxtw	x2, w0
	lsl	x0, x2, 2
	sub	x0, x0, x2
	add	x0, x4, x0, lsl 2
	str	w13, [x0, 520]
	str	w3, [x4, 1260]
.L90:
	ldr	w0, [x4, 1256]
	cmp	w0, w11
	bcs	.L91
	ldr	w2, [x1, 2728]
	add	x1, x23, x0, uxtw 3
	add	x1, x1, 327680
	add	w0, w0, 1
	strb	w8, [x1, 1960]
	str	w2, [x1, 1956]
	str	w0, [x4, 1256]
.L91:
	ldr	w0, [x9, 2184]
	cmp	w5, w0
	bcc	.L92
.L93:
	ldr	x3, [x22, 104]
	mov	x2, 1272
	mov	x1, x21
	mov	x0, x19
	str	x10, [x29, 136]
	blr	x3
	ldr	x10, [x29, 136]
	mov	w3, 1
	ldr	x2, [x22, 32]
	mov	w0, 8
	ldr	x1, [x10, w25, sxtw 3]
	add	x1, x1, 327680
	str	w3, [x1, 2188]
	blr	x2
	mov	w1, 0
	b	.L190
.L209:
	add	w0, w22, 126
	adds	w22, w22, 63
	csel	w0, w0, w22, mi
	add	w7, w24, 255
	asr	w0, w0, 6
	b	.L199
.L152:
	mov	w21, 2359296
	mov	w9, 0
	mov	w8, 0
	b	.L107
.L106:
	add	w11, w22, 63
	mov	w4, w22
	mov	w9, 0
	mov	w8, 0
	b	.L107
.L219:
	add	x1, x1, 2664
	mov	w0, w25
	bl	DelAllFrameMemRecord
	ldr	x10, [x29, 136]
	cbnz	w0, .L226
.L78:
	ldr	x1, [x10, w25, sxtw 3]
	mov	w2, 255
	add	x0, x1, 323584
	add	x1, x1, 327680
	add	x0, x0, 2664
	add	x1, x1, 872
	.p2align 2
.L79:
	str	w2, [x0], 72
	cmp	x0, x1
	bne	.L79
	mov	w0, 1088
	cmp	w24, 1920
	ccmp	w22, w0, 0, le
	ldr	x0, [x10, w25, sxtw 3]
	ble	.L80
	ldr	w2, [x29, 160]
.L81:
	add	x3, x0, 323584
	ldr	w1, [x0, 1560]
	add	w1, w2, w1
	str	w1, [x3, 2644]
	ldr	w4, [x0, 692]
	cmp	w4, 2
	ccmp	w27, 17, 4, eq
	beq	.L83
	str	w1, [x29, 168]
.L83:
	add	x0, x0, 327680
	mov	w1, 1
	str	wzr, [x0, 2184]
	str	w1, [x3, 2616]
	ldr	x3, .LC46
	str	w1, [x0, 2708]
	ldr	x4, [x3]
	cbz	x4, .L84
	ldr	w0, [x29, 156]
	ldp	w1, w3, [x29, 164]
	stp	w2, w0, [x29, 176]
	ldr	w0, [x29, 172]
	add	x2, x29, 176
	stp	w3, w1, [x29, 184]
	str	w0, [x29, 192]
	mov	w3, 28
	stp	w24, w22, [x29, 196]
	mov	w1, 121
	mov	w0, w25
	str	x10, [x29, 136]
	blr	x4
	ldr	x10, [x29, 136]
.L84:
	ldr	x22, .LC47
	mov	x2, 512
	ldr	x0, [x10, w25, sxtw 3]
	mov	w1, 0
	str	x10, [x29, 136]
	ldr	x3, [x22, 96]
	add	x0, x0, 327680
	add	x0, x0, 2192
	blr	x3
	ldr	x10, [x29, 136]
	ldr	x0, .LC48
	ldr	x1, [x10, w25, sxtw 3]
	ldr	x0, [x0, w25, sxtw 3]
	add	x1, x1, 327680
	str	wzr, [x1, 2704]
	bl	FSP_ClearContextAll
	ldr	x10, [x29, 136]
	b	.L85
.L80:
	ldr	w1, [x0, 44]
	ldr	w2, [x29, 160]
	cmp	w1, 24
	beq	.L81
	add	w2, w2, 4
	str	w2, [x29, 160]
	b	.L81
.L226:
	ldr	x1, .LC45
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x10, [x29, 136]
	b	.L78
.L216:
	ldr	x2, .LC38
	ldr	x3, .LC37
	b	.L200
.L213:
	ldr	x2, .LC38
	ldr	x3, .LC68
	b	.L200
.L96:
	ldr	x2, .LC38
	ldr	x3, .LC59
	b	.L200
.L220:
	ldr	x22, .LC47
	b	.L85
.L206:
	ldr	x2, .LC38
	ldr	x3, .LC58
	b	.L200
.L102:
	ldr	x2, .LC38
	ldr	x3, .LC64
	b	.L200
.L211:
	ldr	x1, .LC66
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L190
.L207:
	ldr	x2, .LC38
	ldr	x3, .LC61
	b	.L200
.L225:
	ldr	x1, .LC56
	mov	w0, 31
	str	x10, [x29, 136]
	bl	dprint_vfmw
	ldr	x10, [x29, 136]
	mov	w1, 30
	mov	w2, w1
	ldr	x0, [x10, w25, sxtw 3]
	add	x0, x0, 323584
	str	w1, [x0, 2644]
	b	.L88
.L217:
	ldr	x1, .LC41
	mov	w0, 31
	bl	dprint_vfmw
	mov	w1, -1
	b	.L190
.L224:
	ldr	x1, .LC54
	mov	w0, 31
	str	x10, [x29, 136]
	bl	dprint_vfmw
	ldr	x10, [x29, 136]
	ldr	x0, [x10, w25, sxtw 3]
	b	.L87
.L223:
	ldr	x2, .LC38
	ldr	x1, .LC39
	ldr	x3, .LC52
	add	x2, x2, 120
	bl	dprint_vfmw
	mov	w1, -1
	b	.L190
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
.LC70:
	.xword	.LC69
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
	bgt	.L246
	bne	.L243
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L247
	ldr	x23, .LC78
	ldr	x20, .LC79
	ldr	x1, [x23]
	ldr	x0, [x20]
	ldr	w24, [x1, 36]
	ldr	w22, [x0, 120]
	tbnz	x22, 0, .L248
.L227:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L243:
	ldr	x1, .LC75
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L246:
	ldr	x2, .LC71
	mov	w3, w19
	ldr	x1, .LC73
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
.L248:
	ldr	x21, .LC80
	mov	w1, 2
	bfi	w22, w1, 8, 2
	str	w22, [x0, 120]
	mov	w19, 0
	orr	w22, w22, 64
	ldr	x0, [x21, 216]
	blr	x0
	ldr	x0, [x20]
	str	w22, [x0, 120]
	b	.L235
	.p2align 3
.L249:
	add	w19, w19, 1
	cmp	w19, 1000
	beq	.L236
.L235:
	ldr	x1, [x21, 224]
	mov	x0, 30
	blr	x1
	ldr	x0, [x20]
	ldr	w0, [x0, 372]
	tbz	x0, 2, .L249
	cmp	w19, 999
	bgt	.L236
	ldr	x2, .LC71
	mov	w3, 0
	ldr	x1, .LC84
	mov	w0, 0
	add	x2, x2, 160
	bl	dprint_vfmw
.L238:
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
.L236:
	ldr	x2, .LC71
	mov	w3, 0
	ldr	x1, .LC82
	mov	w0, 0
	add	x2, x2, 160
	bl	dprint_vfmw
	b	.L238
.L247:
	ldr	x1, .LC77
	mov	x2, 0
	mov	w0, 0
	bl	dprint_vfmw
	b	.L227
	.size	VDMHAL_V400R003_ResetVdm, .-VDMHAL_V400R003_ResetVdm
	.align	3
.LC71:
	.xword	.LANCHOR0
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
.LC78:
	.xword	g_HwMem
	.align	3
.LC79:
	.xword	g_pstRegCrg
	.align	3
.LC80:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC82:
	.xword	.LC81
	.align	3
.LC84:
	.xword	.LC83
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
	ldr	x0, .LC85
	ldr	x3, [x0, 352]
	cbz	x3, .L250
	add	x2, x29, 60
	add	x1, x29, 56
	add	x0, x29, 52
	blr	x3
	cbnz	w19, .L252
	ldr	x0, .LC86
	ldp	w3, w2, [x29, 52]
	ldr	w1, [x29, 60]
	ldr	x0, [x0]
	add	x0, x0, 61440
	str	w3, [x0, 524]
	str	w2, [x0, 772]
	str	w1, [x0, 776]
.L250:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L252:
	ldr	x20, .LC87
	mov	w3, w19
	ldr	x21, .LC89
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
.LC85:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC86:
	.xword	g_HwMem
	.align	3
.LC87:
	.xword	.LANCHOR0
	.align	3
.LC89:
	.xword	.LC88
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
	ldr	x0, .LC91
	mov	w1, 8
	movk	w1, 0x3, lsl 16
	ldr	x0, [x0]
	add	x0, x0, 61440
	str	w1, [x0]
	ldp	x29, x30, [sp], 16
	ret
	.size	VDMHAL_V400R003_EnableSmmu, .-VDMHAL_V400R003_EnableSmmu
	.align	3
.LC91:
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
	cbz	x0, .L273
	cbnz	w19, .L272
	ldr	x20, .LC95
	ldr	x0, [x20]
	ldr	w22, [x0, 120]
	tbnz	x22, 0, .L274
.L257:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L274:
	ldr	x21, .LC96
	mov	w1, 2
	bfi	w22, w1, 8, 2
	str	w22, [x0, 120]
	orr	w22, w22, 16
	ldr	x0, [x21, 216]
	blr	x0
	ldr	x0, [x20]
	str	w22, [x0, 120]
	b	.L264
	.p2align 3
.L275:
	add	w19, w19, 1
	cmp	w19, 1000
	beq	.L265
.L264:
	ldr	x1, [x21, 224]
	mov	x0, 30
	blr	x1
	ldr	x0, [x20]
	ldr	w0, [x0, 372]
	tbz	x0, 0, .L275
	cmp	w19, 999
	bgt	.L265
	ldr	x2, .LC92
	mov	w3, 0
	ldr	x1, .LC100
	mov	w0, 0
	add	x2, x2, 232
	bl	dprint_vfmw
.L267:
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
.L272:
	ldr	x1, .LC102
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L265:
	ldr	x2, .LC92
	mov	w3, 0
	ldr	x1, .LC98
	mov	w0, 0
	add	x2, x2, 232
	bl	dprint_vfmw
	b	.L267
.L273:
	ldr	x2, .LC92
	mov	w3, -121438208
	ldr	x1, .LC94
	add	x2, x2, 232
	bl	dprint_vfmw
	b	.L257
	.size	VDMHAL_V400R003_GlbResetX, .-VDMHAL_V400R003_GlbResetX
	.align	3
.LC92:
	.xword	.LANCHOR0
	.align	3
.LC94:
	.xword	.LC93
	.align	3
.LC95:
	.xword	g_pstRegCrg
	.align	3
.LC96:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC98:
	.xword	.LC97
	.align	3
.LC100:
	.xword	.LC99
	.align	3
.LC102:
	.xword	.LC101
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
	cbz	x0, .L279
	mov	w0, 0
	bl	VDMHAL_V400R003_GlbResetX
.L276:
	ldp	x29, x30, [sp], 16
	ret
.L279:
	ldr	x2, .LC103
	mov	w3, -121438208
	ldr	x1, .LC104
	add	x2, x2, 264
	bl	dprint_vfmw
	b	.L276
	.size	VDMHAL_V400R003_GlbReset, .-VDMHAL_V400R003_GlbReset
	.align	3
.LC103:
	.xword	.LANCHOR0
	.align	3
.LC104:
	.xword	.LC93
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
	ldr	x20, .LC106
	sbfiz	x0, x19, 6, 32
	ldr	x1, .LC105
	add	x0, x20, x0
	ldrsw	x0, [x0, 8]
	ldr	x0, [x1, x0, lsl 3]
	cbz	x0, .L281
	ldr	w21, [x0, 1336]
	cbnz	w19, .L282
.L283:
	ldr	x19, .LC109
	ldr	x0, [x19]
	cbz	x0, .L297
.L285:
	cmp	w21, 1
	bne	.L296
	ldr	w1, [x20, 44]
	cmp	w1, 1
	beq	.L296
	ldr	w2, [x0, 28]
	cmp	w1, 2
	beq	.L298
	cmp	w1, 3
	bne	.L280
	mov	w1, -15
	str	w1, [x0, 32]
	b	.L280
	.p2align 3
.L281:
	mov	w21, 0
	cbz	w19, .L283
	.p2align 2
.L282:
	ldr	x1, .LC108
	mov	w0, 0
	bl	dprint_vfmw
.L280:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L296:
	mov	w1, -1
	str	w1, [x0, 32]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L297:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L286
	str	x0, [x19]
	b	.L285
	.p2align 3
.L298:
	mov	w1, -12
	str	w1, [x0, 32]
	b	.L280
.L286:
	ldr	x1, .LC111
	mov	w0, 0
	bl	dprint_vfmw
	b	.L280
	.size	VDMHAL_V400R003_ClearIntState, .-VDMHAL_V400R003_ClearIntState
	.align	3
.LC105:
	.xword	s_pstVfmwChan
	.align	3
.LC106:
	.xword	g_VdmDrvParam
	.align	3
.LC108:
	.xword	.LC107
	.align	3
.LC109:
	.xword	g_HwMem
	.align	3
.LC111:
	.xword	.LC110
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
	cbnz	w19, .L305
	ldr	x19, .LC113
	ldr	x0, [x19]
	cbz	x0, .L306
.L303:
	add	x0, x0, 61440
	mov	w1, 7
	str	w1, [x0, 44]
.L299:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L305:
	ldr	x1, .LC112
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L306:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L304
	str	x0, [x19]
	b	.L303
.L304:
	ldr	x1, .LC114
	mov	w0, 0
	bl	dprint_vfmw
	b	.L299
	.size	VDMHAL_V400R003_ClearMMUIntState, .-VDMHAL_V400R003_ClearMMUIntState
	.align	3
.LC112:
	.xword	.LC107
	.align	3
.LC113:
	.xword	g_HwMem
	.align	3
.LC114:
	.xword	.LC110
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
	cbnz	w19, .L313
	ldr	x19, .LC117
	ldr	x0, [x19]
	cbz	x0, .L314
.L311:
	mov	w1, -1
	str	w1, [x0, 36]
.L307:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L313:
	ldr	x1, .LC116
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L314:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L312
	str	x0, [x19]
	b	.L311
.L312:
	ldr	x1, .LC118
	mov	w0, 0
	bl	dprint_vfmw
	b	.L307
	.size	VDMHAL_V400R003_MaskInt, .-VDMHAL_V400R003_MaskInt
	.align	3
.LC116:
	.xword	.LC115
	.align	3
.LC117:
	.xword	g_HwMem
	.align	3
.LC118:
	.xword	.LC110
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
	ldr	x2, .LC120
	sbfiz	x0, x19, 6, 32
	ldr	x1, .LC119
	add	x0, x2, x0
	ldrsw	x0, [x0, 8]
	ldr	x0, [x1, x0, lsl 3]
	cbz	x0, .L319
	ldr	w20, [x0, 1336]
	cbnz	w19, .L320
.L321:
	ldr	x19, .LC123
	ldr	x0, [x19]
	cbz	x0, .L332
.L323:
	mov	w1, -6
	mov	w2, -2
	cmp	w20, 1
	csel	w1, w1, w2, eq
	str	w1, [x0, 36]
.L318:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L319:
	mov	w20, 0
	cbz	w19, .L321
.L320:
	ldr	x1, .LC122
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L332:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L324
	str	x0, [x19]
	b	.L323
.L324:
	ldr	x1, .LC124
	mov	w0, 0
	bl	dprint_vfmw
	b	.L318
	.size	VDMHAL_V400R003_EnableInt, .-VDMHAL_V400R003_EnableInt
	.align	3
.LC119:
	.xword	s_pstVfmwChan
	.align	3
.LC120:
	.xword	g_VdmDrvParam
	.align	3
.LC122:
	.xword	.LC121
	.align	3
.LC123:
	.xword	g_HwMem
	.align	3
.LC124:
	.xword	.LC110
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
	bgt	.L355
	bne	.L353
	ldr	x19, .LC128
	ldr	x0, [x19]
	cbz	x0, .L356
.L339:
	sub	w0, w20, #1
	cmp	w0, 6
	bls	.L357
	ldr	x2, .LC125
	mov	w3, w20
	ldr	x1, .LC134
	mov	w0, 0
	add	x2, x2, 296
	bl	dprint_vfmw
	mov	w0, 0
.L352:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L357:
	ldr	x1, .LC135
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx346
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx346:
	.section	.rodata
	.align	0
	.align	2
.L346:
	.byte	(.L350 - .Lrtx346) / 4
	.byte	(.L347 - .Lrtx346) / 4
	.byte	(.L348 - .Lrtx346) / 4
	.byte	(.L349 - .Lrtx346) / 4
	.byte	(.L351 - .Lrtx346) / 4
	.byte	(.L341 - .Lrtx346) / 4
	.byte	(.L343 - .Lrtx346) / 4
	.text
	.p2align 3
.L351:
	mov	x0, 61480
.L345:
	ldr	x1, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ldr	w0, [x1, x0]
	ret
	.p2align 3
.L350:
	mov	x0, 28
	b	.L345
	.p2align 3
.L341:
	mov	x0, 62304
	b	.L345
	.p2align 3
.L347:
	mov	x0, 32
	b	.L345
	.p2align 3
.L349:
	mov	x0, 40
	b	.L345
	.p2align 3
.L348:
	mov	x0, 36
	b	.L345
	.p2align 3
.L343:
	mov	x0, 62272
	b	.L345
	.p2align 3
.L353:
	ldr	x2, .LC125
	mov	w0, 0
	ldr	x1, .LC130
	add	x2, x2, 296
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L356:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L340
	str	x0, [x19]
	b	.L339
.L340:
	ldr	x2, .LC125
	mov	w0, 0
	ldr	x1, .LC132
	add	x2, x2, 296
	bl	dprint_vfmw
	mov	w0, 0
	b	.L352
	.p2align 3
.L355:
	ldr	x2, .LC125
	mov	w3, w19
	ldr	x1, .LC127
	mov	w0, 0
	add	x2, x2, 296
	bl	dprint_vfmw
	mov	w0, -1
	b	.L352
	.size	VDMHAL_V400R003_CheckReg, .-VDMHAL_V400R003_CheckReg
	.align	3
.LC125:
	.xword	.LANCHOR0
	.align	3
.LC127:
	.xword	.LC126
	.align	3
.LC128:
	.xword	g_HwMem
	.align	3
.LC130:
	.xword	.LC129
	.align	3
.LC132:
	.xword	.LC131
	.align	3
.LC134:
	.xword	.LC133
	.align	3
.LC135:
	.xword	.L346
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
	ble	.L361
	ldr	x2, .LC137
	mov	w3, w19
	ldr	x1, .LC139
	mov	w4, 1
	mov	w0, 32
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L361:
	lsl	x0, x19, 6
	ldr	x1, .LC136
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
.LC136:
	.xword	g_HwMem
	.align	3
.LC137:
	.xword	.LANCHOR1
	.align	3
.LC139:
	.xword	.LC138
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
	ble	.L365
	ldr	x2, .LC141
	mov	w3, w19
	ldr	x1, .LC142
	mov	w4, 1
	add	x2, x2, 32
	mov	w0, 32
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L365:
	lsl	x0, x19, 6
	ldr	x1, .LC140
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
.LC140:
	.xword	g_HwMem
	.align	3
.LC141:
	.xword	.LANCHOR1
	.align	3
.LC142:
	.xword	.LC88
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
	cbz	x22, .L386
	cmp	w21, wzr
	bgt	.L387
	cmp	w20, 6
	beq	.L388
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
	bls	.L389
.L385:
	mov	w0, -1
	b	.L368
	.p2align 3
.L389:
	ldr	x0, .LC147
	ldrb	w0, [x0,w20,uxtw]
	adr	x1, .Lrtx373
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx373:
	.section	.rodata
	.align	0
	.align	2
.L373:
	.byte	(.L382 - .Lrtx373) / 4
	.byte	(.L374 - .Lrtx373) / 4
	.byte	(.L375 - .Lrtx373) / 4
	.byte	(.L376 - .Lrtx373) / 4
	.byte	(.L385 - .Lrtx373) / 4
	.byte	(.L377 - .Lrtx373) / 4
	.byte	(.L371 - .Lrtx373) / 4
	.byte	(.L385 - .Lrtx373) / 4
	.byte	(.L378 - .Lrtx373) / 4
	.byte	(.L379 - .Lrtx373) / 4
	.byte	(.L380 - .Lrtx373) / 4
	.byte	(.L380 - .Lrtx373) / 4
	.byte	(.L380 - .Lrtx373) / 4
	.byte	(.L381 - .Lrtx373) / 4
	.byte	(.L385 - .Lrtx373) / 4
	.byte	(.L382 - .Lrtx373) / 4
	.byte	(.L383 - .Lrtx373) / 4
	.byte	(.L384 - .Lrtx373) / 4
	.text
	.p2align 3
.L388:
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
.L371:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	AVSHAL_V400R003_StartDec
.L368:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L382:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	H264HAL_V400R003_StartDec
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L377:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	DIVX3HAL_V400R003_StartDec
	b	.L368
	.p2align 3
.L376:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	MP2HAL_V400R003_StartDec
	b	.L368
	.p2align 3
.L375:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	MP4HAL_V400R003_StartDec
	b	.L368
	.p2align 3
.L374:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	VC1HAL_V400R003_StartDec
	b	.L368
	.p2align 3
.L384:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	VP9HAL_V400R003_StartDec
	b	.L368
	.p2align 3
.L383:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	HEVCHAL_V400R003_StartDec
	b	.L368
	.p2align 3
.L381:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	VP8HAL_V400R003_StartDec
	b	.L368
	.p2align 3
.L380:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	VP6HAL_V400R003_StartDec
	b	.L368
	.p2align 3
.L379:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	RV9HAL_V400R003_StartDec
	b	.L368
	.p2align 3
.L378:
	mov	x2, x19
	mov	w1, w21
	mov	x0, x22
	bl	RV8HAL_V400R003_StartDec
	b	.L368
	.p2align 3
.L387:
	ldr	x2, .LC143
	mov	w4, 0
	ldr	x1, .LC146
	mov	w3, w21
	add	x2, x2, 64
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L368
.L386:
	ldr	x2, .LC143
	mov	x3, 0
	ldr	x1, .LC145
	mov	w0, 0
	add	x2, x2, 64
	bl	dprint_vfmw
	mov	w0, -1
	b	.L368
	.size	VDMHAL_V400R003_PrepareDec, .-VDMHAL_V400R003_PrepareDec
	.align	3
.LC143:
	.xword	.LANCHOR1
	.align	3
.LC145:
	.xword	.LC144
	.align	3
.LC146:
	.xword	.LC72
	.align	3
.LC147:
	.xword	.L373
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
	ldr	x2, .LC148
	lsl	x1, x0, 6
	sub	x1, x1, x0
	lsl	x1, x1, 2
	sub	x0, x1, x0
	add	x0, x1, x0, lsl 2
	ldr	x0, [x2, x0]
	cbz	x0, .L395
	cmp	w19, wzr
	ble	.L396
	ldr	x2, .LC151
	mov	w4, 1
	ldr	x1, .LC153
	mov	w3, w19
	add	x2, x2, 96
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, 0
.L392:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L396:
	ldr	w0, [x0, 28]
	ldr	x19, [sp, 16]
	ubfx	x0, x0, 17, 1
	ldp	x29, x30, [sp], 32
	ret
.L395:
	ldr	x2, .LC151
	ldr	x3, .LC150
	ldr	x1, .LC152
	add	x2, x2, 96
	bl	dprint_vfmw
	mov	w0, -1
	b	.L392
	.size	VDMHAL_V400R003_IsVdmReady, .-VDMHAL_V400R003_IsVdmReady
	.align	3
.LC148:
	.xword	g_HwMem
	.align	3
.LC150:
	.xword	.LC149
	.align	3
.LC151:
	.xword	.LANCHOR1
	.align	3
.LC152:
	.xword	.LC7
	.align	3
.LC153:
	.xword	.LC138
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
	ldr	x2, .LC154
	lsl	x1, x0, 6
	sub	x1, x1, x0
	lsl	x1, x1, 2
	sub	x0, x1, x0
	add	x0, x1, x0, lsl 2
	ldr	x0, [x2, x0]
	cbz	x0, .L402
	cmp	w19, wzr
	ble	.L403
	ldr	x2, .LC157
	mov	w4, 1
	ldr	x1, .LC158
	mov	w3, w19
	add	x2, x2, 128
	mov	w0, 32
	bl	dprint_vfmw
	mov	w0, 1
.L399:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L403:
	ldr	w0, [x0, 40]
	ldr	x19, [sp, 16]
	cmp	w0, 1
	cset	w0, ne
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L402:
	ldr	x1, .LC156
	bl	dprint_vfmw
	mov	w0, 0
	b	.L399
	.size	VDMHAL_V400R003_IsVdmRun, .-VDMHAL_V400R003_IsVdmRun
	.align	3
.LC154:
	.xword	g_HwMem
	.align	3
.LC156:
	.xword	.LC155
	.align	3
.LC157:
	.xword	.LANCHOR1
	.align	3
.LC158:
	.xword	.LC138
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
	cbz	x19, .L433
	ldr	w0, [x19, 8]
	ldr	x22, [x19]
	cbnz	w0, .L429
	ldr	x2, .LC163
	add	x0, x2, 4
	add	x2, x2, 64
	ldr	w1, [x2, -64]
	cmp	w1, 1
	beq	.L424
	.p2align 2
.L411:
	ldr	w1, [x0]
	add	x0, x0, 4
	cmp	w1, 1
	beq	.L424
	cmp	x0, x2
	bne	.L411
	mov	w19, 0
.L409:
	ldr	x21, .LC168
	ldr	x1, [x21]
	cbz	x1, .L434
.L412:
	ldr	x0, .LC175
	ldr	x23, .LC170
	ldr	w5, [x1, 12]
	ldr	w0, [x0, 56]
	ldr	w2, [x1, 28]
	cmp	w0, 1
	stp	w5, w2, [x23]
	beq	.L435
	cmp	w19, 1
	beq	.L436
	ldr	w2, [x1, 4096]
	ldr	w0, [x1, 4108]
	ldr	w6, [x1, 208]
	ldr	w7, [x1, 212]
	stp	w6, w7, [x23, 8]
	stp	w2, w0, [x23, 16]
	cbnz	w19, .L417
	ldr	w2, [x1, 176]
	ldr	w0, [x1, 180]
	stp	w2, w0, [x23, 24]
	ldr	w3, [x1, 184]
	ldr	w2, [x1, 188]
	ldr	w0, [x1, 192]
	str	w3, [x23, 32]
	stp	w2, w0, [x23, 36]
.L417:
	ldr	x0, .LC173
	add	x4, x1, 32768
	add	x1, x4, 256
	add	x3, x0, 128
	.p2align 2
.L418:
	ldr	w2, [x1], 4
	str	w2, [x0], 4
	cmp	x3, x0
	bne	.L418
	cbnz	w19, .L419
	ldr	w1, [x4, 384]
	ldr	w0, [x4, 388]
	stp	w0, w1, [x23, 44]
.L419:
	and	w5, w5, 15
	cmp	w5, 5
	beq	.L437
	and	w6, w6, 31
	mov	w0, 0
	str	w6, [x22]
.L428:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L424:
	mov	w19, w1
	b	.L409
.L435:
	ldr	w2, [x1, 4108]
	ldr	w0, [x1, 176]
	and	w1, w2, 1048575
	stp	w1, w0, [x23, 20]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L437:
	and	w7, w7, 1
	str	w7, [x22]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L436:
	ldr	w0, [x21, 56]
	and	w20, w2, 131071
	bl	MEM_Phy2Vir
	cbz	x0, .L438
	sub	w1, w20, #1
	cmp	w1, 199
	bhi	.L425
	lsl	w1, w20, 2
	sub	w1, w1, #4
	lsl	x1, x1, 2
	add	x2, x1, 8
.L416:
	ldr	w3, [x0, x1]
	ldr	x1, [x21]
	str	w3, [x23, 16]
	ldr	w5, [x23]
	ldr	w6, [x1, 208]
	ldr	w0, [x0, x2]
	ldr	w7, [x1, 212]
	stp	w6, w7, [x23, 8]
	str	w0, [x23, 20]
	b	.L417
.L434:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L413
	mov	x1, x0
	str	x0, [x21]
	b	.L412
.L425:
	mov	x2, 8
	mov	x1, 0
	b	.L416
.L429:
	ldr	x1, .LC165
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L428
.L433:
	ldr	x2, .LC161
	mov	w0, 0
	ldr	x3, .LC160
	ldr	x1, .LC162
	add	x2, x2, 160
	bl	dprint_vfmw
	mov	w0, -1
	b	.L428
.L413:
	ldr	x1, .LC169
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L428
.L438:
	ldr	x2, .LC161
	ldr	x3, .LC172
	ldr	x1, .LC162
	add	x2, x2, 160
	bl	dprint_vfmw
	mov	w0, -1
	b	.L428
	.size	VDMHAL_V400R003_BackupInfo, .-VDMHAL_V400R003_BackupInfo
	.align	3
.LC160:
	.xword	.LC159
	.align	3
.LC161:
	.xword	.LANCHOR1
	.align	3
.LC162:
	.xword	.LC7
	.align	3
.LC163:
	.xword	g_DSPState
	.align	3
.LC165:
	.xword	.LC164
	.align	3
.LC168:
	.xword	g_HwMem
	.align	3
.LC169:
	.xword	.LC110
	.align	3
.LC170:
	.xword	g_BackUp
	.align	3
.LC172:
	.xword	.LC171
	.align	3
.LC173:
	.xword	g_BackUp+52
	.align	3
.LC175:
	.xword	g_VdmDrvParam
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
	beq	.L443
	cmp	w19, 800
	bhi	.L444
	ldr	x3, .LC180
	ubfiz	x2, x19, 2, 30
	mov	x1, x21
	mov	x0, x20
	ldr	x3, [x3, 104]
	blr	x3
	mov	w0, 0
.L441:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L444:
	ldr	x1, .LC179
	mov	w2, w19
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L441
	.p2align 3
.L443:
	ldr	x1, .LC177
	mov	x3, x21
	mov	x2, x20
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L441
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
	beq	.L446
	sub	w0, w21, #1
	cmp	w0, 255
	bhi	.L446
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
.L446:
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
	cbz	x0, .L498
	ldr	w2, [x19, 840]
	sub	w0, w2, #1
	cmp	w0, 199
	bhi	.L499
	ldr	w3, [x19, 864]
	ldp	w0, w2, [x19, 24]
	ldr	w6, [x19]
	lsl	w0, w0, w3
	lsl	w2, w2, w3
	add	w1, w0, 255
	cmp	w0, 1920
	and	w1, w1, -256
	lsl	w3, w1, 4
	bgt	.L454
	cmp	w6, 17
	beq	.L454
	mov	w0, 1
	mov	w4, 0
	str	wzr, [x19, 848]
	str	w0, [x19, 872]
.L455:
	cmp	w6, 16
	beq	.L456
.L508:
	add	w0, w2, 31
	mov	w24, 8
	and	w0, w0, -32
	madd	w0, w0, w1, w4
.L478:
	ldr	w2, [x19, 16]
	mov	w1, w3
	str	w2, [x20]
	ldr	w2, [x19, 16]
	add	w2, w0, w2
	str	w2, [x20, 4]
	ldr	w2, [x19, 4]
	str	w2, [x20, 8]
	ldr	w2, [x19, 4]
	add	w0, w0, w2
	str	w0, [x20, 12]
	ldr	w0, [x19, 872]
	cbnz	w0, .L500
.L458:
	str	w1, [x20, 16]
	mov	w3, 0
	str	w4, [x20, 20]
	mov	w2, 0
	ldp	w1, w0, [x19, 24]
	sub	w0, w0, #1
	sub	w1, w1, #1
	bfi	w2, w0, 0, 9
	bfi	w3, w1, 0, 9
	strh	w2, [x29, 78]
	strh	w3, [x29, 76]
	ldr	w0, [x29, 76]
	str	w0, [x20, 24]
	ldr	w0, [x19, 852]
	ldr	w21, [x19, 24]
	sub	w0, w0, #1
	cmp	w0, 1
	bls	.L501
	ldr	w23, [x19, 28]
.L460:
	ldr	w0, [x19, 864]
	cmp	w0, 6
	beq	.L480
	cmp	w0, 5
	beq	.L481
	cmp	w0, 4
	mov	w3, 8
	beq	.L461
	ldr	x1, .LC191
	mov	w0, 1
	bl	dprint_vfmw
	mov	w3, 1
.L461:
	ldr	w0, [x19, 856]
	cbnz	w0, .L483
.L509:
	ldr	w9, [x19, 840]
	mov	w0, 65535
	cmp	w9, wzr
	ble	.L464
	mul	w12, w23, w21
	mov	w11, 0
	mov	w8, 0
	sub	w13, w12, #1
	mov	w4, 0
	sub	w2, w3, #1
	b	.L465
	.p2align 3
.L467:
	add	w4, w4, 1
	cmp	w9, w4
	ble	.L502
.L477:
	cmp	w4, 199
	bgt	.L503
.L465:
	add	x10, x19, x4, sxtw 2
	cmp	w4, wzr
	ldrsh	w1, [x10, 42]
	sub	w0, w1, #1
	add	w0, w0, w3
	udiv	w0, w0, w3
	mul	w0, w0, w3
	ble	.L466
	ldrsh	w8, [x10, 38]
	sub	w8, w8, #1
	add	w8, w8, w3
	udiv	w8, w8, w3
	mul	w8, w8, w3
	cmp	w0, w8
	ble	.L467
	add	w5, w4, 1
	add	x6, x19, x4, sxtw 2
	cmp	w5, w9
	add	w0, w2, w1
	add	w7, w5, 1
	bge	.L504
	.p2align 2
.L471:
	udiv	w0, w0, w3
	ldrsh	w1, [x6, 44]
	udiv	w1, w1, w3
	mul	w0, w0, w3
	mul	w1, w1, w3
	cmp	w0, w1
	add	w14, w1, 1
	bhi	.L472
	sub	w1, w4, #1
	cmp	w0, w14
	add	x1, x19, x1, sxtw 2
	beq	.L472
	ldrsh	w1, [x1, 42]
	sub	w1, w1, #1
	add	w1, w1, w3
	udiv	w1, w1, w3
	mul	w1, w1, w3
	cmp	w0, w1
	bhi	.L470
.L472:
	cmp	w7, 199
	mov	w4, w5
	bgt	.L505
	add	w5, w4, 1
	ldrsh	w1, [x6, 46]
	cmp	w5, w9
	add	x6, x19, x4, sxtw 2
	add	w0, w2, w1
	add	w7, w5, 1
	blt	.L471
.L504:
	udiv	w0, w0, w3
	mul	w0, w0, w3
.L470:
	ldrsh	w1, [x10, 40]
	cmp	w12, w0
	csel	w0, w0, w13, hi
	add	w6, w11, 1
	udiv	w1, w1, w3
	mul	w1, w1, w3
	cmp	w0, w1
	csel	w1, w1, wzr, ge
	cmp	w6, 252
	bgt	.L506
	cbnz	w22, .L485
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
	bgt	.L477
.L502:
	ldr	w0, [x19, 856]
	cmp	w0, 1
	bne	.L507
.L476:
	mov	w0, 1
	str	w0, [x19, 840]
	mov	w0, 0
	cbnz	w22, .L464
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
	b	.L464
	.p2align 3
.L454:
	ldr	w5, [x19, 848]
	mov	w4, 0
	cmp	w5, 1
	bne	.L455
	adds	w4, w0, 2047
	add	w0, w0, 4094
	csel	w0, w0, w4, mi
	add	w5, w2, 126
	and	w0, w0, -2048
	cmp	w0, wzr
	add	w4, w0, 127
	csel	w0, w4, w0, lt
	adds	w7, w2, 63
	csel	w4, w5, w7, mi
	asr	w0, w0, 7
	asr	w4, w4, 6
	cmp	w6, 16
	lsl	w4, w4, 5
	mul	w4, w0, w4
	bne	.L508
.L456:
	madd	w0, w2, w1, w4
	mov	w24, 10
	b	.L478
	.p2align 3
.L483:
	cmp	w0, 1
	mov	w11, 0
	beq	.L476
.L507:
	sub	w0, w11, #1
	uxth	w0, w0
.L464:
	ldr	w4, [x19, 864]
	ldr	w5, [x19, 848]
	ldr	w3, [x19, 852]
	sub	w4, w4, #4
	ldrb	w2, [x29, 78]
	ldrb	w1, [x29, 79]
	and	w3, w3, 3
	bfi	w2, w5, 0, 1
	strh	w0, [x29, 76]
	bfi	w1, w4, 0, 2
	ldr	w4, [x19, 872]
	bfi	w2, w3, 4, 2
	bfi	w1, w24, 2, 4
	bfi	w1, w4, 6, 1
	bfi	w2, w3, 6, 2
	strb	w1, [x29, 79]
	mov	w0, 0
	strb	w2, [x29, 78]
	ldr	w1, [x29, 76]
	str	w1, [x20, 28]
.L496:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L500:
	add	w0, w3, 15
	cmp	w3, wzr
	csel	w1, w0, w3, lt
	asr	w1, w1, 4
	b	.L458
	.p2align 3
.L466:
	ccmp	w0, w8, 0, ne
	ble	.L467
	b	.L470
	.p2align 3
.L485:
	mov	w11, w6
	b	.L467
	.p2align 3
.L505:
	add	x0, x19, x5, sxtw 2
	ldrsh	w0, [x0, 42]
	sub	w0, w0, #1
	add	w0, w0, w3
	udiv	w0, w0, w3
	mul	w0, w0, w3
	b	.L470
.L480:
	ldr	w0, [x19, 856]
	mov	w3, 2
	cbnz	w0, .L483
	b	.L509
.L501:
	ldr	w0, [x19, 28]
	add	w0, w0, w0, lsr 31
	asr	w23, w0, 1
	b	.L460
.L481:
	ldr	w0, [x19, 856]
	mov	w3, 4
	cbnz	w0, .L483
	b	.L509
.L503:
	ldr	x2, .LC186
	mov	w3, 2550
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
.L506:
	mov	w0, 1
	str	w0, [x19, 856]
	b	.L476
.L499:
	ldr	x1, .LC189
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L496
.L498:
	ldr	x2, .LC186
	mov	w0, 0
	ldr	x3, .LC185
	ldr	x1, .LC187
	add	x2, x2, 192
	bl	dprint_vfmw
	mov	w0, -1
	b	.L496
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
	cbnz	w19, .L555
	ldr	x0, [x21]
	cbz	x0, .L556
.L514:
	ldr	x19, .LC197
	ldr	w3, [x21, 64]
	ldrb	w0, [x19]
	and	w3, w3, -16
	str	w3, [x29, 60]
	cmp	w0, 1
	beq	.L557
	mov	x0, x20
	mov	w2, 16
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	mov	w3, 53763
	movk	w3, 0x2000, lsl 16
	str	w3, [x29, 60]
	cmp	w0, 1
	beq	.L558
	mov	x0, x20
	mov	w2, 12
	mov	w1, 2
	mov	w21, 3075
	movk	w21, 0x30, lsl 16
	bl	VDH_Record_RegData
	str	w21, [x29, 60]
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L559
	mov	x0, x20
	mov	w3, w21
	mov	w2, 60
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L560
	mov	x0, x20
	mov	w3, w21
	mov	w2, 64
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L561
	mov	x0, x20
	mov	w3, w21
	mov	w2, 68
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L562
	mov	x0, x20
	mov	w3, w21
	mov	w2, 72
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L563
	mov	x0, x20
	mov	w3, w21
	mov	w2, 76
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L564
	mov	x0, x20
	mov	w3, w21
	mov	w2, 80
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x19]
	cmp	w0, 1
	beq	.L565
	mov	x0, x20
	mov	w3, w21
	mov	w2, 84
	mov	w1, 2
	bl	VDH_Record_RegData
	mov	w0, 0
.L540:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L557:
	ldr	x0, .LC198
	ldr	x1, [x0]
	str	w3, [x1, 16]
.L517:
	ldr	x1, [x0]
	mov	w2, 53763
	movk	w2, 0x2000, lsl 16
	str	w2, [x1, 12]
.L519:
	ldr	x1, [x0]
	mov	w0, 3075
	movk	w0, 0x30, lsl 16
	str	w0, [x1, 60]
.L522:
	mov	w0, 3075
	movk	w0, 0x30, lsl 16
	str	w0, [x1, 64]
.L525:
	mov	w0, 3075
	movk	w0, 0x30, lsl 16
	str	w0, [x1, 68]
.L528:
	mov	w0, 3075
	movk	w0, 0x30, lsl 16
	str	w0, [x1, 72]
.L531:
	mov	w0, 3075
	movk	w0, 0x30, lsl 16
	str	w0, [x1, 76]
.L534:
	mov	w0, 3075
	movk	w0, 0x30, lsl 16
	str	w0, [x1, 80]
.L537:
	mov	w2, 3075
	mov	w0, 0
	movk	w2, 0x30, lsl 16
	str	w2, [x1, 84]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L556:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L515
	str	x0, [x21]
	b	.L514
	.p2align 3
.L555:
	ldr	x1, .LC195
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L540
.L515:
	ldr	x1, .LC196
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L540
.L558:
	ldr	x0, .LC198
	b	.L517
.L559:
	ldr	x0, .LC198
	b	.L519
.L561:
	ldr	x0, .LC198
	ldr	x1, [x0]
	b	.L525
.L562:
	ldr	x0, .LC198
	ldr	x1, [x0]
	b	.L528
.L560:
	ldr	x0, .LC198
	ldr	x1, [x0]
	b	.L522
.L565:
	ldr	x0, .LC198
	ldr	x1, [x0]
	b	.L537
.L564:
	ldr	x0, .LC198
	ldr	x1, [x0]
	b	.L534
.L563:
	ldr	x0, .LC198
	ldr	x1, [x0]
	b	.L531
	.size	VDMHAL_V400R003_CfgRpReg, .-VDMHAL_V400R003_CfgRpReg
	.align	3
.LC195:
	.xword	.LC194
	.align	3
.LC196:
	.xword	.LC110
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
	cbz	x19, .L596
	ldr	x22, [x19, 8]
	ldr	w23, [x19]
	ldr	w21, [x19, 16]
	cbz	x22, .L597
	sxtw	x24, w21
	ldr	x20, .LC207
	lsl	x0, x24, 6
	sub	x0, x0, x24
	lsl	x0, x0, 2
	sub	x1, x0, x24
	add	x0, x0, x1, lsl 2
	ldr	x0, [x20, x0]
	cbz	x0, .L570
	ldr	x2, .LC208
	add	x0, x2, 4
	add	x2, x2, 64
	ldr	w1, [x2, -64]
	cmp	w1, 1
	beq	.L585
	.p2align 2
.L573:
	ldr	w1, [x0]
	add	x0, x0, 4
	cmp	w1, 1
	beq	.L585
	cmp	x0, x2
	bne	.L573
	mov	w19, 0
.L571:
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
	beq	.L586
	and	w1, w1, 131072
	cmp	w1, wzr
	ccmp	w23, 3, 0, ne
	cset	w1, ne
.L574:
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
	beq	.L598
.L575:
	cmp	w2, 200
	bhi	.L599
.L577:
	lsl	x0, x24, 6
	sub	x0, x0, x24
	lsl	x0, x0, 2
	sub	x1, x0, x24
	add	x0, x0, x1, lsl 2
	add	x20, x20, x0
	ldr	w21, [x20, 56]
	mov	w0, w21
	bl	MEM_Phy2Vir
	cbz	x0, .L600
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
	cbz	w0, .L582
	sub	w2, w0, #1
	mov	x5, 18
	add	x1, x22, 14
	mov	w0, 2
	add	x5, x5, x2, uxtw 2
	lsl	x3, x20, 5
	add	x5, x22, x5
	.p2align 2
.L581:
	sub	w2, w0, #1
	add	x4, x3, x0, uxtw
	add	x2, x3, x2
	add	w0, w0, 4
	ldr	w2, [x19, x2, lsl 2]
	strh	w2, [x1, -2]
	ldr	w2, [x19, x4, lsl 2]
	strh	w2, [x1], 4
	cmp	x5, x1
	bne	.L581
.L582:
	mov	w0, 6
	bl	IsDprintTypeEnable
	cbnz	w0, .L601
.L568:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L598:
	cmp	w19, 1
	beq	.L602
	ldr	x1, .LC214
	ldrb	w1, [x1]
	cbnz	w1, .L575
	ldrb	w0, [x0, 19]
	and	w0, w0, 3
	cmp	w0, 2
	bne	.L575
	mov	w0, 1
	strh	wzr, [x22, 12]
	str	w0, [x22, 8]
	mov	w0, 0
	strh	wzr, [x22, 14]
	b	.L568
	.p2align 3
.L585:
	mov	w19, w1
	b	.L571
	.p2align 3
.L601:
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
.L583:
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
	bhi	.L583
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
.L586:
	mov	w1, 0
	b	.L574
	.p2align 3
.L599:
	ldr	x1, .LC216
	mov	w3, 200
	mov	w0, 1
	bl	dprint_vfmw
	str	wzr, [x22, 8]
	b	.L577
	.p2align 3
.L602:
	ldr	w0, [x0, 16]
	ubfx	x0, x0, 21, 2
	cmp	w0, 2
	bne	.L575
	strh	wzr, [x22, 12]
	mov	w0, 0
	strh	wzr, [x22, 14]
	str	w19, [x22, 8]
	b	.L568
.L600:
	ldr	x2, .LC203
	ldr	x3, .LC217
	ldr	x1, .LC204
	add	x2, x2, 224
	bl	dprint_vfmw
	mov	w0, -1
	b	.L568
.L570:
	ldr	x2, .LC203
	ldr	x3, .LC209
.L594:
	ldr	x1, .LC204
	mov	w0, 0
	add	x2, x2, 224
	bl	dprint_vfmw
	mov	w0, -1
	b	.L568
.L597:
	ldr	x2, .LC203
	ldr	x3, .LC206
	b	.L594
.L596:
	ldr	x2, .LC203
	ldr	x3, .LC202
	b	.L594
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
	.xword	.LC149
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
	.xword	.LC171
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
	cbnz	w21, .L615
	ldr	x0, [x20]
	cbz	x0, .L616
	cbnz	w24, .L609
.L618:
	ldr	x21, .LC232
	ldr	w0, [x21, 840]
	cmp	w0, wzr
	ble	.L610
	cmp	w25, 6
	bne	.L611
	ldrb	w0, [x22, 17]
	cmp	w0, 1
	csel	w0, w0, wzr, eq
	str	w0, [x21, 1728]
.L611:
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
.L609:
	cmp	w24, 1
	mov	w20, 0
	beq	.L617
.L606:
	mov	w0, w20
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L616:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L608
	str	x0, [x20]
	cbnz	w24, .L609
	b	.L618
	.p2align 3
.L617:
	cmp	w25, 6
	bne	.L613
	ldrb	w0, [x22, 17]
	cmp	w0, 1
	bne	.L613
	ldr	x21, .LC232
	ldr	w0, [x21, 1716]
	cmp	w0, wzr
	ble	.L613
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
.L613:
	ldr	x1, .LC237
	mov	w0, 0
	mov	w20, -1
	bl	dprint_vfmw
	b	.L606
	.p2align 3
.L615:
	ldr	x1, .LC229
	mov	w0, 0
	mov	w20, -1
	bl	dprint_vfmw
	b	.L606
.L610:
	ldr	x1, .LC234
	mov	w0, 0
	mov	w20, -1
	bl	dprint_vfmw
	b	.L606
.L608:
	ldr	x1, .LC231
	mov	w0, 0
	mov	w20, -1
	bl	dprint_vfmw
	b	.L606
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
	bne	.L643
	cmp	w20, wzr
	ble	.L644
	ldr	x22, .LC240
	mov	w3, w20
	ldr	x23, .LC241
	mov	w0, 32
	add	x2, x22, 256
	mov	x1, x23
	bl	dprint_vfmw
	ldrb	w0, [x21]
	cmp	w0, 1
	beq	.L627
	mov	x0, x19
	mov	w3, -2
	mov	w2, 36
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x21]
	cmp	w0, 1
	beq	.L640
.L622:
	cbz	x19, .L645
.L636:
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
.L619:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L644:
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
.L625:
	lsl	x1, x0, 6
	mov	w2, -2
	sub	x1, x1, x0
	lsl	x1, x1, 2
	sub	x0, x1, x0
	add	x0, x1, x0, lsl 2
	ldr	x0, [x3, x0]
	str	w2, [x0, 36]
.L623:
	ldr	x0, .LC245
	ldr	x0, [x0, 216]
	blr	x0
	cmp	w20, wzr
	ble	.L639
	ldr	x22, .LC240
	ldr	x23, .LC241
.L632:
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
	b	.L619
	.p2align 3
.L621:
	cmp	w20, wzr
	ble	.L637
	ldr	x22, .LC240
	ldr	x23, .LC241
.L627:
	mov	w4, 1
	mov	w3, w20
	add	x2, x22, 256
	mov	x1, x23
	mov	w0, 32
	bl	dprint_vfmw
	ldrb	w0, [x21]
	cmp	w0, 1
	bne	.L622
.L640:
	ldr	x0, .LC245
	ldr	x0, [x0, 216]
	blr	x0
	b	.L632
	.p2align 3
.L643:
	mov	x0, x19
	mov	w3, 67108864
	mov	w2, 8
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x21]
	cmp	w0, 1
	beq	.L621
	mov	x0, x19
	mov	w3, -2
	mov	w2, 36
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x21]
	cmp	w0, 1
	beq	.L623
	cbnz	x19, .L636
.L645:
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
.L639:
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
	b	.L619
.L637:
	ldr	x3, .LC239
	sxtw	x0, w20
	b	.L625
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
	.xword	.LC88
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
	tbnz	w19, #31, .L674
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
.L649:
	mov	w0, w19
	bl	VCTRL_IsChanActive
	cbnz	w0, .L675
	ldr	x2, [x26, w19, sxtw 3]
	mov	w0, w19
	ldr	x3, [x25, w19, sxtw 3]
	ldp	w1, w2, [x2, 28]
	ldr	w3, [x3, 1624]
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
	bhi	.L651
	ldr	x1, .LC255
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx654
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx654:
	.section	.rodata
	.align	0
	.align	2
.L654:
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L651 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L651 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L651 - .Lrtx654) / 4
	.byte	(.L664 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.byte	(.L663 - .Lrtx654) / 4
	.text
	.p2align 3
.L663:
	mul	w19, w19, w20
	madd	w23, w19, w22, w23
.L651:
	cmp	x24, x21
	beq	.L672
	ldr	w19, [x21], 4
	tbz	w19, #31, .L649
.L672:
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
.L647:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L675:
	mov	w3, w19
	mov	x2, x27
	mov	x1, x28
	mov	w0, 1
	bl	dprint_vfmw
	b	.L651
	.p2align 3
.L664:
	mul	w19, w19, w20
	mul	w19, w19, w22
	add	w23, w23, w19, lsl 1
	b	.L651
.L674:
	mov	w0, 25
	b	.L647
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
	.xword	.L654
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
	bls	.L684
.L677:
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
.L684:
	ldr	x0, .LC257
	ldrb	w0, [x0,w20,uxtw]
	adr	x1, .Lrtx679
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx679:
	.section	.rodata
	.align	0
	.align	2
.L679:
	.byte	(.L677 - .Lrtx679) / 4
	.byte	(.L680 - .Lrtx679) / 4
	.byte	(.L681 - .Lrtx679) / 4
	.byte	(.L682 - .Lrtx679) / 4
	.text
	.p2align 3
.L681:
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
.L680:
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
.L682:
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
	.xword	.L679
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
	ble	.L686
	ldr	x2, .LC259
	mov	w4, 0
	ldr	x1, .LC260
	mov	w3, w20
	add	x2, x2, 336
	mov	w0, 0
	bl	dprint_vfmw
.L685:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L686:
	ldr	x21, .LC261
	ldrb	w0, [x21]
	cmp	w0, 1
	bne	.L721
	ldr	x1, .LC264
	ldr	x2, [x1]
	ldr	w0, [x2, 120]
	ubfx	x3, x0, 8, 2
	cbz	w3, .L691
	and	w0, w0, -769
	str	w0, [x2, 120]
.L691:
	ldr	x25, .LC262
	sxtw	x22, w20
	ldr	x24, .LC263
	mov	w2, 500
	add	x0, x25, x22, lsl 6
	ldr	w23, [x0, 8]
	str	w2, [x0, 16]
	ldr	x0, [x24, w23, sxtw 3]
	ldr	w26, [x0, 1336]
.L690:
	ldr	x0, [x1]
	ldr	w1, [x0, 120]
	and	w1, w1, -258049
	str	w1, [x0, 120]
	ldr	w1, [x0, 120]
	orr	w1, w1, 131072
	str	w1, [x0, 120]
.L692:
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
	beq	.L716
	mov	w1, 2
	mov	x0, x19
	mov	w3, -1431655766
	mov	w2, 156
	bl	VDH_Record_RegData
	ldr	x1, .LC265
	ldrb	w0, [x21]
	ldr	w1, [x1]
	cmp	w1, 1
	beq	.L722
.L696:
	mov	w3, 0
.L695:
	cmp	w0, 1
	beq	.L723
	mov	x0, x19
	mov	w2, 61472
	mov	w1, 2
	bl	VDH_Record_RegData
	cmp	w26, 1
	ldrb	w0, [x21]
	beq	.L724
.L699:
	mov	w3, -2
.L698:
	cmp	w0, 1
	beq	.L725
	mov	x0, x19
	mov	w2, 36
	mov	w1, 2
	bl	VDH_Record_RegData
	ldrb	w0, [x21]
	cmp	w0, 1
	beq	.L726
	cbz	x19, .L727
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
.L724:
	mov	w3, -6
	b	.L698
	.p2align 3
.L722:
	mov	w3, 15
	b	.L695
	.p2align 3
.L721:
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
	ldr	w26, [x0, 1336]
	beq	.L728
	mov	w3, 0
	mov	w2, 1
	mov	w1, 0
	mov	x0, x19
	bl	VDH_Record_RegData
	b	.L692
	.p2align 3
.L716:
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
	bne	.L696
	mov	w3, 15
	b	.L697
	.p2align 3
.L723:
	ldr	x24, .LC266
.L697:
	lsl	x0, x22, 6
	cmp	w26, 1
	sub	x0, x0, x22
	lsl	x0, x0, 2
	sub	x1, x0, x22
	add	x0, x0, x1, lsl 2
	ldr	x0, [x24, x0]
	add	x0, x0, 61440
	str	w3, [x0, 32]
	beq	.L712
	mov	w0, 1
	b	.L699
	.p2align 3
.L725:
	ldr	x24, .LC266
.L700:
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
.L703:
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
	b	.L685
	.p2align 3
.L727:
	ldr	x2, .LC259
	mov	x3, 0
	ldr	x1, .LC268
	mov	w0, 0
	add	x2, x2, 336
	bl	dprint_vfmw
	b	.L685
	.p2align 3
.L712:
	mov	w3, -6
	b	.L700
.L728:
	ldr	x1, .LC264
	b	.L690
.L726:
	ldr	x19, .LC267
	mov	x0, 30
	ldr	x1, [x19, 224]
	blr	x1
	ldr	x0, [x19, 216]
	blr	x0
	ldr	x24, .LC266
	b	.L703
	.size	VDMHAL_V400R003_StartHwDecode, .-VDMHAL_V400R003_StartHwDecode
	.align	3
.LC259:
	.xword	.LANCHOR1
	.align	3
.LC260:
	.xword	.LC72
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
	beq	.L775
	mov	w23, 2
	cmp	w0, w23
	csinc	w23, w23, wzr, ne
.L731:
	ldr	x0, [x29, 112]
	cbz	x0, .L730
	ldr	x0, .LC273
	mov	w4, 0
	add	x3, x29, 144
	mov	w2, 4194304
	mov	w1, 0
	bl	MEM_AllocMemBlock
	cbz	w0, .L733
.L735:
	ldr	x1, .LC275
	mov	w0, 1
	bl	dprint_vfmw
.L730:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L733:
	ldr	x0, .LC277
	mov	w4, 0
	add	x3, x29, 176
	mov	w2, 4194304
	mov	w1, 0
	bl	MEM_AllocMemBlock
	cbnz	w0, .L735
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
	bne	.L736
	cbz	w3, .L812
	ldr	x27, .LC278
	mov	w28, 0
	str	wzr, [x29, 132]
	.p2align 2
.L737:
	cbz	w19, .L741
	ldr	w0, [x29, 124]
	lsr	w23, w28, 4
	ldr	w24, [x29, 132]
	mov	w25, 0
	mul	w0, w23, w0
	ubfiz	x23, x28, 8, 4
	add	x23, x0, x23
	.p2align 2
.L739:
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
	bhi	.L739
.L741:
	ldr	w0, [x29, 132]
	add	w28, w28, 1
	add	w0, w0, w19
	str	w0, [x29, 132]
	ldr	w0, [x29, 128]
	cmp	w0, w28
	bne	.L737
	ldr	w0, [x29, 128]
	ldr	x3, [x27, 88]
	lsr	w2, w0, 1
	mov	w22, w2
	ldr	x2, [x29, 112]
	mul	w1, w19, w0
	mov	x0, x20
	str	w22, [x29, 132]
	blr	x3
	cbz	w22, .L774
	ldr	w1, [x29, 124]
	mov	w25, 0
	mov	w24, 0
	asr	w0, w1, 1
	str	w0, [x29, 128]
	.p2align 2
.L745:
	cbz	w19, .L746
	ldr	w0, [x29, 128]
	lsr	w22, w24, 3
	uxtw	x23, w25
	mov	w28, 0
	mul	w0, w22, w0
	ubfiz	x22, x24, 8, 3
	add	x22, x0, x22
	.p2align 2
.L744:
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
	bhi	.L744
.L746:
	ldr	w0, [x29, 132]
	add	w24, w24, 1
	add	w25, w25, w19
	cmp	w24, w0
	bne	.L745
.L774:
	ldr	w0, [x29, 104]
	cmp	w0, 1
	beq	.L813
	ldr	w0, [x29, 104]
	cbz	w0, .L772
.L767:
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
.L775:
	mov	w23, 0
	b	.L731
.L736:
	cmp	w23, 1
	ldr	w1, [x29, 128]
	cset	w0, ne
	str	w0, [x29, 100]
	cmp	w1, w0
	ldr	x27, .LC278
	bls	.L756
	ldr	w28, [x29, 100]
	.p2align 2
.L758:
	cbz	w19, .L759
	ldr	w0, [x29, 124]
	lsr	w24, w28, 4
	lsr	w23, w28, 1
	mov	w25, 0
	mul	w0, w24, w0
	mul	w23, w23, w19
	ubfiz	x24, x28, 8, 4
	add	x24, x0, x24
	.p2align 2
.L757:
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
	bhi	.L757
.L759:
	ldr	w0, [x29, 128]
	add	w28, w28, 2
	cmp	w0, w28
	bhi	.L758
.L756:
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
	cbz	w22, .L761
	ldr	w1, [x29, 124]
	mov	w25, 0
	mov	w24, 0
	asr	w0, w1, 1
	str	w0, [x29, 128]
	.p2align 2
.L763:
	cbz	w19, .L764
	ldr	w0, [x29, 128]
	lsr	w22, w24, 3
	uxtw	x23, w25
	mov	w28, 0
	mul	w0, w22, w0
	ubfiz	x22, x24, 8, 3
	add	x22, x0, x22
	.p2align 2
.L762:
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
	bhi	.L762
.L764:
	ldr	w0, [x29, 132]
	add	w24, w24, 1
	add	w25, w25, w19
	cmp	w24, w0
	bne	.L763
.L761:
	ldr	w0, [x29, 104]
	cmp	w0, 1
	beq	.L814
	ldr	w0, [x29, 104]
	cbnz	w0, .L767
	ldr	w1, [x29, 100]
	ldr	w0, [x29, 120]
	cmp	w0, w1
	bls	.L772
	.p2align 2
.L773:
	ldr	w0, [x29, 100]
	ldr	w1, [x29, 120]
	add	w0, w0, 2
	str	w0, [x29, 100]
	cmp	w1, w0
	bhi	.L773
.L772:
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
	b	.L767
.L814:
	ldr	w0, [x29, 120]
	cbz	w0, .L767
	ldr	w0, [x29, 100]
	lsl	w7, w19, 1
	mov	w6, 0
	mul	w0, w19, w0
	add	x20, x20, x0
	.p2align 2
.L768:
	ldr	w2, [x29, 108]
	cbz	w2, .L771
	lsr	w0, w6, 1
	mov	w1, 1
	mul	w0, w0, w2
	add	w5, w2, w0
	.p2align 2
.L769:
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
	bne	.L769
.L771:
	ldr	w0, [x29, 120]
	add	w6, w6, 2
	add	x20, x20, x7
	cmp	w0, w6
	bhi	.L768
	b	.L767
.L813:
	ldr	w0, [x29, 120]
	cbz	w0, .L767
	mov	w0, 0
	mov	w6, 0
	.p2align 2
.L750:
	ldr	w1, [x29, 108]
	mov	w5, w0
	cbz	w1, .L753
	ldr	w1, [x29, 108]
	add	w5, w1, w0
	mov	w1, 1
	.p2align 2
.L751:
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
	bne	.L751
.L753:
	ldr	w0, [x29, 120]
	add	w6, w6, 1
	add	x20, x20, x19
	cmp	w0, w6
	mov	w0, w5
	bne	.L750
	b	.L767
.L812:
	ldr	x27, .LC278
	mov	w1, 0
	ldr	x2, [x29, 112]
	mov	x0, x20
	ldr	x3, [x27, 88]
	blr	x3
	b	.L774
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
	bne	.L822
	cbnz	w19, .L818
	ldr	x0, .LC280
	ldr	x1, [x0]
	ldr	w0, [x1, 120]
	ubfx	x2, x0, 8, 2
	cmp	w20, w2
	beq	.L815
	bfi	w0, w20, 8, 2
	str	w0, [x1, 120]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L818:
	cmp	w19, 1
	beq	.L823
.L815:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L822:
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
.L823:
	ldr	x0, .LC280
	ldr	x1, [x0]
	ldr	w0, [x1, 120]
	bfi	w0, w20, 12, 5
	and	w0, w0, -131073
	str	w0, [x1, 120]
	ldr	w0, [x1, 120]
	orr	w0, w0, 131072
	str	w0, [x1, 120]
	b	.L815
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
	bne	.L828
	cmp	w19, wzr
	ble	.L829
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
.L829:
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
.L828:
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
	.xword	.LC88
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.52265, %object
	.size	__func__.52265, 24
__func__.52265:
	.string	"VDMHAL_V400R003_OpenHAL"
	.type	__func__.52302, %object
	.size	__func__.52302, 27
__func__.52302:
	.string	"VDMHAL_V400R003_CalcFsSize"
	.zero	5
	.type	__func__.52371, %object
	.size	__func__.52371, 27
__func__.52371:
	.string	"VDMHAL_V400R003_ArrangeMem"
	.zero	5
	.type	__FUNCTION__.52372, %object
	.size	__FUNCTION__.52372, 27
__FUNCTION__.52372:
	.string	"VDMHAL_V400R003_ArrangeMem"
	.zero	5
	.type	__func__.52314, %object
	.size	__func__.52314, 35
__func__.52314:
	.string	"VDMHAL_V400R003_FillMemArrangeInfo"
	.zero	5
	.type	__func__.52401, %object
	.size	__func__.52401, 25
__func__.52401:
	.string	"VDMHAL_V400R003_ResetVdm"
	.zero	7
	.type	__func__.52414, %object
	.size	__func__.52414, 37
__func__.52414:
	.string	"VDMHAL_V400R003_SetSmmuPageTableAddr"
	.zero	3
	.type	__func__.52435, %object
	.size	__func__.52435, 26
__func__.52435:
	.string	"VDMHAL_V400R003_GlbResetX"
	.zero	6
	.type	__func__.52424, %object
	.size	__func__.52424, 25
__func__.52424:
	.string	"VDMHAL_V400R003_GlbReset"
	.zero	7
	.type	__func__.52496, %object
	.size	__func__.52496, 25
__func__.52496:
	.string	"VDMHAL_V400R003_CheckReg"
	.align	3
.LANCHOR1 = . + 0
	.type	__func__.52513, %object
	.size	__func__.52513, 28
__func__.52513:
	.string	"VDMHAL_V400R003_ReadMMUMask"
	.zero	4
	.type	__func__.52518, %object
	.size	__func__.52518, 29
__func__.52518:
	.string	"VDMHAL_V400R003_WriteMMUMask"
	.zero	3
	.type	__func__.52525, %object
	.size	__func__.52525, 27
__func__.52525:
	.string	"VDMHAL_V400R003_PrepareDec"
	.zero	5
	.type	__func__.52547, %object
	.size	__func__.52547, 27
__func__.52547:
	.string	"VDMHAL_V400R003_IsVdmReady"
	.zero	5
	.type	__func__.52552, %object
	.size	__func__.52552, 25
__func__.52552:
	.string	"VDMHAL_V400R003_IsVdmRun"
	.zero	7
	.type	__func__.52566, %object
	.size	__func__.52566, 27
__func__.52566:
	.string	"VDMHAL_V400R003_BackupInfo"
	.zero	5
	.type	__func__.52612, %object
	.size	__func__.52612, 25
__func__.52612:
	.string	"VDMHAL_V400R003_CfgRpMsg"
	.zero	7
	.type	__func__.52646, %object
	.size	__func__.52646, 30
__func__.52646:
	.string	"VDMHAL_V400R003_MakeDecReport"
	.zero	2
	.type	__func__.52817, %object
	.size	__func__.52817, 15
__func__.52817:
	.string	"MFDE_ConfigReg"
	.zero	1
	.type	__func__.52680, %object
	.size	__func__.52680, 30
__func__.52680:
	.string	"VDMHAL_V400R003_StartHwRepair"
	.zero	2
	.type	__func__.52696, %object
	.size	__func__.52696, 30
__func__.52696:
	.string	"VDMHAL_V400R003_CalVdhClkSkip"
	.zero	2
	.type	__func__.52735, %object
	.size	__func__.52735, 30
__func__.52735:
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
	ASCII(.string	"VDMHAL ArrangeMem HEVC/VP9 10 bit\n" )
	.zero	5
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
.LC69:
	ASCII(.string	"cann't allocate img slot" )
	.zero	7
.LC72:
	ASCII(.string	"%s: VdhId(%d) > %d\n" )
	.zero	4
.LC74:
	ASCII(.string	"VdhId is wrong! VDMHAL_V400R003_ResetVdm\n" )
	.zero	6
.LC76:
	ASCII(.string	"VDMHAL_V400R003_ResetVdm: map vdm register fail, vir(reg) = (%p)\n" )
	.zero	6
.LC81:
	ASCII(.string	"%s module id %d failed!\n" )
	.zero	7
.LC83:
	ASCII(.string	"%s module id %d success!\n" )
	.zero	6
.LC88:
	ASCII(.string	"%s: WR_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\n" )
	.zero	3
.LC93:
	ASCII(.string	"%s: map vdm register 0x%x failed!\n" )
	.zero	5
.LC97:
	ASCII(.string	"%s VdhId %d failed!\n" )
	.zero	3
.LC99:
	ASCII(.string	"%s VdhId %d success!\n" )
	.zero	2
.LC101:
	ASCII(.string	"GLB Reset failed, wrong VDH ID!\n" )
	.zero	7
.LC107:
	ASCII(.string	"VdhId is wrong! VDMHAL_V200R003_ClearIntState\n" )
	.zero	1
.LC110:
	ASCII(.string	"vdm register virtual address not mapped, reset failed!\n" )
.LC115:
	ASCII(.string	"VdhId is wrong! VDMHAL_V200_MaskInt\n" )
	.zero	3
.LC121:
	ASCII(.string	"VdhId is wrong! VDMHAL_V200R003_EnableInt\n" )
	.zero	5
.LC126:
	ASCII(.string	"%s: VdhId(%d) Invalid!\n" )
.LC129:
	ASCII(.string	"%s: VdhId is wrong!\n" )
	.zero	3
.LC131:
	ASCII(.string	"%s: vdm register virtual address not mapped, reset failed!\n" )
	.zero	4
.LC133:
	ASCII(.string	"%s: unkown reg_id = %d\n" )
.LC138:
	ASCII(.string	"%s: RD_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\n" )
	.zero	3
.LC144:
	ASCII(.string	"%s: pDecParam(%p) = NULL\n" )
	.zero	6
.LC149:
	ASCII(.string	"VDM register not mapped yet!" )
	.zero	3
.LC155:
	ASCII(.string	"VDM register not mapped yet!\n" )
	.zero	2
.LC159:
	ASCII(.string	"pBackUpInfo = NULL!" )
	.zero	4
.LC164:
	ASCII(.string	"VdhId is wrong! VDMHAL_V200R003_BackupInfo\n" )
	.zero	4
.LC171:
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
