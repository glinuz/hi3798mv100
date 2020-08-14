#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"bitplane.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	BPD_CfgReg
	.type	BPD_CfgReg, %function
BPD_CfgReg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x3
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	mov	x19, x2
	bl	_mcount
	str	wzr, [x29, 60]
	ldr	x0, [x22, 16]
	cbz	x0, .L31
.L2:
	ldr	x5, .LC2
	mov	w1, -1
	mov	w0, -2
	ldr	x7, [x5, 16]
	str	w1, [x7, 68]
	str	w0, [x7, 68]
	ldp	w2, w3, [x21, 28]
	ldrsb	w4, [x20, 3104]
	sub	w1, w2, w3
	ubfx	x0, x3, 3, 29
	ubfx	x1, x1, 3, 2
	ldr	x6, [x21]
	add	x1, x1, x0
	add	x0, x6, x1
	cbz	w4, .L5
	ldrb	w1, [x19, 374]
	cbz	w1, .L6
	ldrh	w1, [x19, 374]
	cmp	w1, 4
	beq	.L6
	ldrb	w1, [x19, 197]
	cmp	w1, 1
	beq	.L32
.L8:
	ldrb	w1, [x19, 361]
	mov	w8, 3
	ldrb	w4, [x29, 61]
	cmp	w1, 4
	and	w1, w1, 3
	csel	w1, w1, w8, ne
	ldrb	w2, [x29, 62]
	ldr	w6, [x19, 404]
	bfi	w4, w1, 6, 2
	ldrb	w1, [x19, 16]
	bfi	w2, w6, 0, 2
	strb	w4, [x29, 61]
	bfi	w2, w1, 2, 2
	cmp	w6, w8
	strb	w2, [x29, 62]
	beq	.L33
.L11:
	add	x2, x20, 20480
	mov	w1, 21846
	movk	w1, 0x5555, lsl 16
	ldrb	w8, [x29, 61]
	ldr	w4, [x2, 3548]
	smull	x1, w4, w1
	lsr	x1, x1, 32
	sub	w1, w1, w4, asr 31
	lsl	w6, w1, 2
	sub	w1, w6, w1
	sub	w4, w4, w1
	bfi	w8, w4, 2, 2
	strb	w8, [x29, 61]
.L15:
	ldr	w4, [x2, 3544]
	mov	w1, 21846
	movk	w1, 0x5555, lsl 16
	and	w6, w3, 7
	ldrb	w8, [x29, 61]
	smull	x1, w4, w1
	lsr	x1, x1, 32
	sub	w1, w1, w4, asr 31
	lsl	w3, w1, 2
	sub	w3, w3, w1
	add	x1, x20, 36864
	sub	w4, w4, w3
	bfi	w8, w4, 0, 2
	strb	w8, [x29, 61]
	ldr	x3, [x1, 1592]
	ldr	w4, [x1, 1612]
	sub	x3, x0, x3
	add	x3, x3, x4
	ldrb	w4, [x29, 62]
	and	w3, w3, 15
	and	w4, w4, -17
	add	w3, w6, w3, lsl 3
	orr	w4, w4, 32
	strb	w3, [x29, 60]
	strb	w4, [x29, 62]
	ldr	w3, [x29, 60]
	str	w3, [x7, 4]
	ldr	x6, [x1, 1592]
	ldr	w4, [x1, 1612]
	ldr	x3, [x5, 16]
	sub	x0, x0, x6
	add	w0, w4, w0
	and	w0, w0, -16
	str	w0, [x3, 8]
	ldrsb	w0, [x20, 3104]
	cbz	w0, .L16
	ldr	w0, [x19, 404]
	cmp	w0, 3
	beq	.L34
.L16:
	ldr	w0, [x2, 3548]
	sub	w0, w0, #1
	strh	w0, [x29, 62]
.L17:
	ldr	w0, [x2, 3544]
	mov	w4, 3
	ldr	x3, [x5, 16]
	mov	w6, 0
	sub	w0, w0, #1
	strh	w0, [x29, 60]
	bfi	w6, w4, 4, 4
	mov	w0, 0
	ldr	w7, [x29, 60]
	str	w7, [x3, 12]
	str	wzr, [x29, 60]
	ldr	w2, [x2, 3544]
	ldr	x7, [x5, 16]
	add	w3, w2, 254
	adds	w2, w2, 127
	csel	w2, w3, w2, mi
	ubfx	x2, x2, 7, 12
	ubfiz	w2, w2, 4, 8
	strh	w2, [x29, 60]
	ldr	w2, [x29, 60]
	str	w2, [x7, 16]
	str	wzr, [x29, 60]
	ldr	x2, [x5, 16]
	strb	w4, [x29, 61]
	ldr	w4, [x1, 1688]
	str	w4, [x2, 20]
	strb	w6, [x29, 60]
	ldr	x2, [x5, 16]
	ldr	w4, [x1, 1692]
	ldr	w3, [x29, 60]
	str	w4, [x2, 24]
	ldr	w4, [x1, 1696]
	str	w4, [x2, 28]
	ldr	w4, [x1, 1700]
	str	w4, [x2, 32]
	ldr	w4, [x1, 1704]
	str	w4, [x2, 36]
	ldr	w4, [x1, 1708]
	str	w4, [x2, 40]
	ldr	w1, [x1, 1712]
	str	w1, [x2, 44]
	str	w3, [x2, 48]
.L4:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L5:
	ldrb	w1, [x19, 259]
	cbz	w1, .L12
	ldr	x1, [x19, 256]
	mov	x2, 67108864
	and	x1, x1, 1099494850560
	cmp	x1, x2
	beq	.L12
.L13:
	ldrb	w6, [x19, 251]
	mov	w8, 3
	ldrb	w1, [x29, 62]
	ldrb	w4, [x29, 61]
	and	w2, w6, w8
	cmp	w6, 4
	ldrb	w6, [x19]
	csel	w2, w2, w8, ne
	and	w1, w1, 252
	bfi	w4, w2, 6, 2
	bfi	w1, w6, 2, 2
	strb	w4, [x29, 61]
	strb	w1, [x29, 62]
	b	.L11
	.p2align 3
.L12:
	ldrb	w1, [x29, 61]
	orr	w1, w1, 16
	strb	w1, [x29, 61]
	b	.L13
	.p2align 3
.L6:
	ldrb	w1, [x29, 61]
	orr	w1, w1, 16
	strb	w1, [x29, 61]
	ldrb	w1, [x19, 197]
	cmp	w1, 1
	bne	.L8
.L32:
	ldr	w1, [x19, 476]
	cmp	w1, 8
	bgt	.L8
	ldrb	w1, [x29, 61]
	orr	w1, w1, 32
	strb	w1, [x29, 61]
	b	.L8
	.p2align 3
.L34:
	ldr	w0, [x2, 3548]
	add	w0, w0, 1
	add	w0, w0, w0, lsr 31
	asr	w0, w0, 1
	sub	w0, w0, #1
	strh	w0, [x29, 62]
	b	.L17
	.p2align 3
.L33:
	add	x2, x20, 20480
	ldr	w1, [x2, 3548]
	add	w1, w1, 1
	add	w6, w1, w1, lsr 31
	mov	w1, 21846
	asr	w8, w6, 1
	movk	w1, 0x5555, lsl 16
	smull	x1, w8, w1
	lsr	x1, x1, 32
	sub	w1, w1, w6, asr 31
	lsl	w6, w1, 2
	sub	w1, w6, w1
	sub	w8, w8, w1
	bfi	w4, w8, 2, 2
	strb	w4, [x29, 61]
	b	.L15
	.p2align 3
.L31:
	mov	w0, 53248
	movk	w0, 0xf8c3, lsl 16
	bl	MEM_Phy2Vir
	cbz	x0, .L3
	str	x0, [x22, 16]
	b	.L2
.L3:
	ldr	x1, .LC1
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4
	.size	BPD_CfgReg, .-BPD_CfgReg
	.align	3
.LC1:
	.xword	.LC0
	.align	3
.LC2:
	.xword	g_HwMem
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BPD_Reset
	.type	BPD_Reset, %function
BPD_Reset:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x0, x30
	bl	_mcount
	mov	w0, 53248
	movk	w0, 0xf8c3, lsl 16
	bl	MEM_Phy2Vir
	cbz	x0, .L50
	ldr	x20, .LC5
	ldr	x2, [x20]
	ldr	w19, [x2, 136]
	orr	w19, w19, 16
	str	w19, [x2, 136]
	ldr	w0, [x2, 372]
	tbnz	x0, 3, .L38
	mov	w0, 9999
	b	.L39
	.p2align 3
.L52:
	subs	w0, w0, #1
	beq	.L51
.L39:
	ldr	w1, [x2, 372]
	tbz	x1, 3, .L52
.L38:
	ldr	x0, .LC6
	and	w19, w19, -17
	str	w19, [x2, 136]
	mov	w1, -2
	ldr	x0, [x0, 16]
	str	w1, [x0, 52]
.L35:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L51:
	ldr	x1, .LC8
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x2, [x20]
	b	.L38
.L50:
	ldr	x1, .LC4
	mov	x2, 0
	bl	dprint_vfmw
	b	.L35
	.size	BPD_Reset, .-BPD_Reset
	.align	3
.LC4:
	.xword	.LC3
	.align	3
.LC5:
	.xword	g_pstRegCrg
	.align	3
.LC6:
	.xword	g_HwMem
	.align	3
.LC8:
	.xword	.LC7
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BPD_Start
	.type	BPD_Start, %function
BPD_Start:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC9
	mov	w2, 1
	ldr	x1, [x0, 16]
	str	wzr, [x1]
	ldr	x1, [x0, 16]
	str	w2, [x1]
	ldr	x0, [x0, 16]
	str	wzr, [x0]
	ldp	x29, x30, [sp], 16
	ret
	.size	BPD_Start, .-BPD_Start
	.align	3
.LC9:
	.xword	g_HwMem
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	IsBpd_Ready
	.type	IsBpd_Ready, %function
IsBpd_Ready:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC10
	ldr	x0, [x0, 16]
	cbz	x0, .L57
	ldr	w0, [x0, 64]
	and	w0, w0, 1
.L56:
	ldp	x29, x30, [sp], 16
	ret
.L57:
	ldr	x3, .LC12
	ldr	x2, .LC13
	ldr	x1, .LC15
	bl	dprint_vfmw
	mov	w0, -1
	b	.L56
	.size	IsBpd_Ready, .-IsBpd_Ready
	.align	3
.LC10:
	.xword	g_HwMem
	.align	3
.LC12:
	.xword	.LC11
	.align	3
.LC13:
	.xword	.LANCHOR0
	.align	3
.LC15:
	.xword	.LC14
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BPDDRV_WaitBpdReadyIfNoIsr
	.type	BPDDRV_WaitBpdReadyIfNoIsr, %function
BPDDRV_WaitBpdReadyIfNoIsr:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x0, x30
	mov	w19, 0
	bl	_mcount
	ldr	x21, .LC16
	ldr	x0, [x21]
	blr	x0
	ldr	x22, .LC17
	mov	w20, w0
	b	.L62
	.p2align 3
.L59:
	ldr	w0, [x0, 64]
	tbnz	x0, 0, .L60
	ldr	x0, [x21]
	blr	x0
	cmp	w0, w20
	csel	w20, w20, wzr, cs
	sub	w19, w0, w20
	cmp	w19, 3999
	bhi	.L64
.L62:
	ldr	x0, [x22, 16]
	cbnz	x0, .L59
	ldr	x3, .LC18
	ldr	x2, .LC19
	ldr	x1, .LC20
	bl	dprint_vfmw
.L60:
	cmp	w19, 3999
	bhi	.L64
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L64:
	ldr	x1, .LC22
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	BPDDRV_WaitBpdReadyIfNoIsr, .-BPDDRV_WaitBpdReadyIfNoIsr
	.align	3
.LC16:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC17:
	.xword	g_HwMem
	.align	3
.LC18:
	.xword	.LC11
	.align	3
.LC19:
	.xword	.LANCHOR0
	.align	3
.LC20:
	.xword	.LC14
	.align	3
.LC22:
	.xword	.LC21
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BPD_GetParam
	.type	BPD_GetParam, %function
BPD_GetParam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x2
	mov	x19, x1
	bl	_mcount
	ldr	x0, .LC23
	ldr	w4, [x20, 24]
	ldr	w3, [x20, 32]
	ldr	x0, [x0, 16]
	lsl	w1, w4, 3
	sub	w1, w1, w3
	ldr	w2, [x0, 80]
	ldr	w0, [x0, 84]
	cmp	w2, w1
	bcs	.L71
	ldrsb	w1, [x21, 3104]
	cbnz	w1, .L72
	and	w3, w0, 15
	ubfx	x1, x0, 8, 4
	str	w3, [x19, 340]
	ubfx	x0, x0, 4, 4
	str	w1, [x19, 344]
	str	w0, [x19, 348]
.L70:
	mov	w1, w2
	mov	x0, x20
	bl	BsLongSkip
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L72:
	ubfx	x1, x0, 12, 4
	ubfx	x7, x0, 0, 4
	ubfx	x6, x0, 16, 4
	ubfx	x5, x0, 20, 4
	ubfx	x4, x0, 8, 4
	ubfx	x3, x0, 24, 4
	strb	w1, [x19, 397]
	ubfx	x1, x0, 4, 4
	strb	w7, [x19, 394]
	ubfx	x0, x0, 28, 2
	strb	w6, [x19, 398]
	strb	w5, [x19, 399]
	strb	w4, [x19, 395]
	strb	w3, [x19, 400]
	strb	w1, [x19, 396]
	strb	w0, [x19, 390]
	b	.L70
	.p2align 3
.L71:
	ldr	x1, .LC25
	sub	w3, w4, w3
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	BPD_GetParam, .-BPD_GetParam
	.align	3
.LC23:
	.xword	g_HwMem
	.align	3
.LC25:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BPD_Drv
	.type	BPD_Drv, %function
BPD_Drv:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	mov	x20, x1
	mov	x21, x2
	bl	_mcount
	add	x1, x20, 20480
	mov	w0, 2
	mov	w2, 1
	str	w2, [x29, 72]
	stp	w0, wzr, [x29, 64]
	ldr	w3, [x1, 3548]
	ldr	w4, [x1, 3544]
	cmp	w3, w0
	ble	.L75
	sub	w5, w4, #3
	mov	w1, 128
	cmp	w5, 125
	ccmp	w3, w1, 0, ls
	bgt	.L75
	ldr	x24, .LC28
	ldrb	w1, [x24]
	cmp	w1, 1
	beq	.L78
	mov	w1, 0
	mov	w4, w2
	mov	w3, 400
	bl	VDH_Acquire_Usage
	add	x1, x29, 64
	mov	w0, 0
	bl	VDH_Set_Parameter
.L78:
	bl	BPD_Reset
	ldr	x23, .LC29
	mov	x2, x21
	mov	x1, x22
	mov	x3, x23
	mov	x0, x20
	bl	BPD_CfgReg
	cbnz	w0, .L79
	ldr	x0, [x23, 16]
	mov	w19, 1
	str	wzr, [x0]
	ldr	x0, [x23, 16]
	str	w19, [x0]
	ldr	x0, [x23, 16]
	str	wzr, [x0]
	bl	BPDDRV_WaitBpdReadyIfNoIsr
	cbz	w0, .L87
.L80:
	ldrb	w0, [x24]
	cmp	w0, 1
	beq	.L77
	add	x1, x29, 64
	mov	w0, 0
	str	wzr, [x29, 72]
	bl	VDH_Set_Parameter
	mov	w2, 1
	mov	w1, 0
	mov	w0, 2
	bl	VDH_Loose_Usage
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L75:
	ldr	x1, .LC27
	lsl	w3, w3, 4
	lsl	w2, w4, 4
	mov	w0, 0
	mov	w19, 1
	bl	dprint_vfmw
.L77:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L79:
	ldr	x1, .LC31
	mov	w0, 0
	mov	w19, 1
	bl	dprint_vfmw
	b	.L80
	.p2align 3
.L87:
	mov	x2, x22
	mov	x1, x21
	mov	x0, x20
	bl	BPD_GetParam
	cmp	w0, wzr
	cset	w19, ne
	b	.L80
	.size	BPD_Drv, .-BPD_Drv
	.align	3
.LC27:
	.xword	.LC26
	.align	3
.LC28:
	.xword	g_HalDisable
	.align	3
.LC29:
	.xword	g_HwMem
	.align	3
.LC31:
	.xword	.LC30
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.46084, %object
	.size	__func__.46084, 12
__func__.46084:
	.string	"IsBpd_Ready"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC0:
	ASCII(.string	"BPD register virtual address not mapped, reset failed!\n" )
.LC3:
	ASCII(.string	"VDMHAL_ResetBPD: map BPD register fail, vir(reg) = (%p)\n" )
	.zero	7
.LC7:
	ASCII(.string	"Reset BPD Failed!\n" )
	.zero	5
.LC11:
	ASCII(.string	"BPD register not mapped yet!" )
	.zero	3
.LC14:
	ASCII(.string	"%s: %s\n" )
.LC21:
	ASCII(.string	"BPD TimeOut!\n" )
	.zero	2
.LC24:
	ASCII(.string	" BPD ERROR: EAT TOO MUCH BS %#x, but available bs num %#x!!!\n" )
	.zero	2
.LC26:
	ASCII(.string	"NOT SUPPORT SIZE %d*%d\n" )
.LC30:
	ASCII(.string	"BPD error: no map reg!\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
