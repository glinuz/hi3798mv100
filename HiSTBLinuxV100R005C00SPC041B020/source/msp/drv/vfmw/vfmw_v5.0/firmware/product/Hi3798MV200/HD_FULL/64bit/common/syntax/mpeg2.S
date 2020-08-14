#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"mpeg2.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.type	MP2_SearchStartCode.part.1, %function
MP2_SearchStartCode.part.1:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	add	x4, x20, x19, sxtw
	add	x2, x20, 2
	.p2align 2
.L12:
	cmp	x4, x2
	bls	.L2
.L15:
	ldrb	w3, [x2]
	cmp	w3, 1
	bls	.L14
.L8:
	add	x2, x2, 3
	cmp	x4, x2
	bhi	.L15
.L2:
	sub	w0, w19, #3
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L14:
	cbnz	w3, .L5
	ldrb	w3, [x2, -1]
	cbnz	w3, .L6
	add	x2, x2, 1
	b	.L12
	.p2align 3
.L5:
	bne	.L12
	ldrb	w0, [x2, -2]
	cbnz	w0, .L8
	ldrb	w0, [x2, -1]
	cbnz	w0, .L8
	sub	x2, x2, x20
	sub	w0, w2, #2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L6:
	ldrb	w1, [x2, 1]
	add	x0, x2, 2
	add	x2, x2, 4
	cmp	w1, wzr
	csel	x2, x2, x0, ne
	b	.L12
	.size	MP2_SearchStartCode.part.1, .-MP2_SearchStartCode.part.1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_VerifyMemPara
	.type	MP2_VerifyMemPara, %function
MP2_VerifyMemPara:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	MP2_VerifyMemPara, .-MP2_VerifyMemPara
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_ArrangeVHBMem
	.type	MP2_ArrangeVHBMem, %function
MP2_ArrangeVHBMem:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 86016
	bl	_mcount
	ldr	x0, [x20, 1768]
	ldp	w2, w3, [x0, 48]
	cmp	w2, 31
	bls	.L19
	cmp	w3, 31
	bls	.L19
	ldr	x0, [x19]
	ldr	w1, [x0, 28]
	cmp	w1, 25
	beq	.L47
	mov	w23, w2
	mov	w22, w3
	mov	w25, 5
	mov	w21, 2
	mov	w24, 3
.L28:
	ldr	x0, .LC5
	mov	x2, 32
	mov	w1, 0
	ldr	x3, [x0, 96]
	add	x0, x29, 96
	blr	x3
	strb	wzr, [x29, 97]
	ldr	x0, [x19]
	stp	w23, w22, [x29, 120]
	stp	w23, w22, [x29, 104]
	ldr	w0, [x0, 28]
	cmp	w0, 24
	beq	.L33
	strb	w24, [x29, 99]
	sxtb	w2, w21
	strb	w25, [x29, 101]
.L38:
	ldr	w0, [x20, 2392]
	add	x1, x29, 96
	strb	w2, [x29, 102]
	strb	wzr, [x29, 98]
	bl	FSP_ConfigInstance
	cbnz	w0, .L34
	ldr	x1, [x19]
	add	x3, x29, 92
	ldr	w0, [x20, 2392]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	mov	w1, w0
	beq	.L48
	mov	w0, 1
	cbz	w1, .L41
.L36:
	ldr	x1, .LC7
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L41
	.p2align 3
.L19:
	ldr	x1, .LC2
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
.L41:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L34:
	ldr	x1, .LC9
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	w0, 0
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L47:
	ldr	w1, [x0, 748]
	cbnz	w1, .L23
	ldr	w23, [x0, 708]
	cmp	w2, w23
	bhi	.L49
	ldr	w22, [x0, 712]
	cmp	w3, w22
	bhi	.L50
	ldr	w2, [x0, 736]
	mov	w21, 1
	ldr	w1, [x0, 732]
	add	w24, w1, 2
	cbz	w2, .L32
.L45:
	add	w1, w1, w21
	mov	w2, 17
	cmp	w1, w2
	csel	w21, w1, w2, le
.L32:
	ldrsb	w25, [x0, 756]
	b	.L28
	.p2align 3
.L23:
	ldr	w4, [x0, 708]
	ldr	w5, [x0, 712]
	cmp	w2, w4
	bhi	.L30
	cmp	w3, w5
	bhi	.L30
	ldr	w4, [x0, 736]
	mov	w23, w2
	ldr	w1, [x0, 732]
	mov	w22, w3
	mov	w21, 1
	add	w24, w1, 2
	cbnz	w4, .L45
	ldrsb	w25, [x0, 756]
	b	.L28
	.p2align 3
.L33:
	mov	w0, 1
	strb	w25, [x29, 101]
	strb	w0, [x29, 99]
	mov	w2, 1
	b	.L38
	.p2align 3
.L30:
	ldr	x1, .LC4
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L41
	.p2align 3
.L50:
	mov	w5, w22
.L25:
	ldr	x1, .LC4
	mov	w4, w23
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L41
	.p2align 3
.L48:
	add	x0, x20, 1784
	bl	ResetVoQueue
	b	.L36
	.p2align 3
.L49:
	ldr	w5, [x0, 712]
	b	.L25
	.size	MP2_ArrangeVHBMem, .-MP2_ArrangeVHBMem
	.align	3
.LC2:
	.xword	.LC1
	.align	3
.LC4:
	.xword	.LC3
	.align	3
.LC5:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC7:
	.xword	.LC6
	.align	3
.LC9:
	.xword	.LC8
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_DecodingDecide
	.type	MP2_DecodingDecide, %function
MP2_DecodingDecide:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x23, x0
	mov	x0, x30
	add	x19, x23, 45056
	mov	w20, -1
	bl	_mcount
	add	x0, x19, 312
	add	x0, x0, 40960
	ldr	w3, [x0, 76]
	tbnz	x3, 23, .L131
.L52:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L131:
	add	x25, x23, 86016
	ldr	x21, [x25, 1768]
	ldrb	w1, [x21, 9]
	cmp	w1, 3
	beq	.L53
	ldr	w0, [x0, 88]
	cmp	w0, 1
	beq	.L53
	ands	w20, w3, 255
	bne	.L132
.L55:
	add	x22, x19, 312
	add	x22, x22, 40960
	ldr	w0, [x22, 144]
	cbz	w0, .L133
	ldr	w0, [x22, 64]
	sub	w0, w0, #2
	cmp	w0, 1
	ldr	x0, [x23]
	ldr	w2, [x0, 8]
	bls	.L134
.L59:
	add	x0, x19, 312
	sub	w1, w2, #1
	add	x0, x0, 40960
	cmp	w1, 3
	str	w2, [x0, 64]
	bls	.L135
	ldrb	w27, [x21, 22]
	mov	w24, 1
	ldrb	w28, [x21, 9]
.L70:
	add	x22, x19, 312
	add	x22, x22, 40960
	ldr	w0, [x22, 8]
	ldr	w26, [x22, 148]
	ldr	w2, [x22, 84]
	add	w26, w26, w0
	add	x0, x25, 664
	str	w2, [x29, 108]
	bl	BsPos
	add	w5, w26, w0, lsr 3
	ldr	x1, .LC19
	mov	w4, w28
	ldr	w2, [x29, 108]
	mov	w3, w27
	mov	w0, 18
	bl	dprint_vfmw
	ldr	w1, [x22, 76]
	mov	w0, -14155777
	and	w0, w1, w0
	str	w0, [x22, 76]
	ldr	x0, [x23]
	ldr	w0, [x0, 652]
	cmp	w0, 1
	beq	.L136
	ldr	w0, [x22, 136]
	cbz	w0, .L137
	cmp	w0, 1
	bls	.L138
.L81:
	add	x22, x19, 312
	ldr	w0, [x25, 2392]
	add	x22, x22, 40960
	ldr	w1, [x22, 172]
	bl	FSP_GetFsImagePtr
	mov	x26, x0
	ldr	w1, [x22, 176]
	ldr	w0, [x25, 2392]
	bl	FSP_GetFsImagePtr
	cmp	x26, xzr
	ccmp	x0, xzr, 4, ne
	beq	.L139
	ldrb	w1, [x21, 22]
	cmp	w1, 2
	beq	.L140
	cmp	w1, 3
	beq	.L141
.L89:
	cbnz	w24, .L103
	ldrb	w0, [x21, 9]
	cmp	w0, 3
	beq	.L95
	add	x0, x19, 312
	add	x0, x0, 40960
	ldr	w0, [x0, 88]
	cbnz	w0, .L52
.L95:
	ldrb	w0, [x21, 27]
	cmp	w0, 1
	beq	.L142
	ldrb	w0, [x21, 18]
	cmp	w0, 1
	beq	.L143
	add	x1, x19, 312
	add	x1, x1, 40960
	ldr	w0, [x1, 260]
	add	w0, w0, 1
	str	w0, [x1, 260]
	lsl	w0, w0, 1
	str	w0, [x1, 256]
.L97:
	ldr	w0, [x25, 564]
	add	x19, x19, 312
	add	x19, x19, 40960
	add	w0, w0, 1
	str	w0, [x25, 564]
	ldr	w0, [x19, 84]
	add	w0, w0, 1
	str	w0, [x19, 84]
	b	.L52
	.p2align 3
.L132:
	ldr	x0, .LC10
	ldr	x4, [x0]
	cbz	x4, .L56
	ldr	w0, [x25, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 103
	blr	x4
	add	x0, x19, 312
	add	x0, x0, 40960
	ldr	w3, [x0, 76]
.L56:
	add	x19, x19, 312
	ldr	x1, .LC12
	add	x19, x19, 40960
	mov	w0, 22
	mov	w20, 0
	ldr	w2, [x19, 84]
	bl	dprint_vfmw
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L53:
	ldr	w0, [x25, 2396]
	ands	w20, w3, 255
	add	w0, w0, 1
	str	w0, [x25, 2396]
	beq	.L55
	b	.L132
	.p2align 3
.L135:
	ldr	x0, .LC17
	ldrb	w0, [x0,w1,uxtw]
	adr	x1, .Lrtx66
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx66:
	.section	.rodata
	.align	0
	.align	2
.L66:
	.byte	(.L65 - .Lrtx66) / 4
	.byte	(.L67 - .Lrtx66) / 4
	.byte	(.L68 - .Lrtx66) / 4
	.byte	(.L69 - .Lrtx66) / 4
	.text
	.p2align 3
.L137:
	ldrb	w1, [x21, 22]
	sub	w0, w1, #2
	uxtb	w0, w0
	cmp	w0, 1
	bls	.L144
.L84:
	cmp	w1, 3
	bne	.L81
	ldr	x1, .LC27
	mov	w0, 1
	mov	w24, 0
	bl	dprint_vfmw
	b	.L81
	.p2align 3
.L141:
	cbz	x26, .L145
	ldr	x1, [x23]
	ldr	w2, [x26, 160]
	ldr	w3, [x1]
	cmp	w2, w3
	bhi	.L91
	ldr	w2, [x0, 160]
	cbz	x0, .L94
	cmp	w3, w2
	bcs	.L89
.L94:
	ldr	x1, .LC38
	mov	w0, 1
	bl	dprint_vfmw
	b	.L52
	.p2align 3
.L68:
	ldrb	w27, [x21, 22]
	mov	w24, 0
	ldrb	w28, [x21, 9]
	b	.L70
	.p2align 3
.L67:
	ldrb	w27, [x21, 22]
	sub	w0, w27, #2
	uxtb	w0, w0
	cmp	w0, 1
	bls	.L146
.L128:
	ldrb	w28, [x21, 9]
	mov	w24, 1
	b	.L70
	.p2align 3
.L65:
	ldrb	w27, [x21, 22]
	cmp	w27, 3
	bne	.L128
	ldrb	w27, [x21, 9]
	cmp	w27, 3
	beq	.L147
.L76:
	add	x0, x19, 312
	mov	w24, 0
	add	x0, x0, 40960
	ldr	w1, [x0, 88]
	cmp	w1, wzr
	cset	w1, eq
	str	w1, [x0, 88]
	ldrb	w27, [x21, 22]
	ldrb	w28, [x21, 9]
	b	.L70
	.p2align 3
.L69:
	ldrb	w0, [x21, 22]
	cmp	w0, 1
	mov	w27, w0
	beq	.L148
	cmp	w0, 3
	beq	.L149
.L75:
	cmp	w0, 2
	beq	.L79
	mov	w27, w0
	b	.L128
	.p2align 3
.L136:
	ldr	w1, [x22, 136]
	cbnz	w1, .L81
	ldrb	w1, [x21, 22]
	cmp	w1, 3
	beq	.L150
	cmp	w1, 2
	bne	.L81
	ldr	x1, .LC23
	bl	dprint_vfmw
	b	.L81
	.p2align 3
.L134:
	cmp	w2, 1
	ccmp	w2, 4, 4, hi
	bne	.L59
	ldr	x1, .LC16
	mov	w0, 2
	bl	dprint_vfmw
	ldrb	w0, [x21, 22]
	cmp	w0, 1
	beq	.L124
	ldr	w0, [x22, 76]
	ldr	w1, [x22, 176]
	orr	w0, w0, 128
	str	w0, [x22, 76]
	ldr	w0, [x25, 2392]
	bl	FSP_GetFsImagePtr
	mov	x24, x0
	cbz	x0, .L62
	ldr	w0, [x25, 2392]
	ldr	x1, [x24, 560]
	bl	FreeUsdByDec
	ldr	w0, [x25, 2392]
	ldr	x1, [x24, 568]
	bl	FreeUsdByDec
	ldr	w0, [x25, 2392]
	ldr	x1, [x24, 576]
	bl	FreeUsdByDec
	ldr	w0, [x25, 2392]
	ldr	x1, [x24, 584]
	bl	FreeUsdByDec
	add	x0, x24, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w1, [x22, 176]
	ldr	w0, [x25, 2392]
	bl	FSP_ClearLogicFs
	ldr	w0, [x25, 2392]
	mov	w2, 1
	ldr	w1, [x22, 172]
	bl	FSP_ClearLogicFs
	stp	wzr, wzr, [x22, 172]
	ldr	w0, [x25, 2392]
	bl	FSP_GetFspType
	cbnz	w0, .L151
.L62:
	add	x0, x19, 312
	add	x0, x0, 40960
	str	wzr, [x0, 136]
.L124:
	ldr	x0, [x23]
	ldr	w2, [x0, 8]
	b	.L59
	.p2align 3
.L133:
	ldr	x1, .LC14
	mov	w0, 1
	ldr	w2, [x22, 84]
	bl	dprint_vfmw
	b	.L52
	.p2align 3
.L140:
	cbz	x26, .L87
	ldr	x0, [x23]
	ldr	w2, [x26, 160]
	ldr	w3, [x0]
	cmp	w2, w3
	bls	.L89
	ldr	x1, .LC34
	mov	w0, 1
	bl	dprint_vfmw
	b	.L52
	.p2align 3
.L103:
	mov	w20, 1
	b	.L52
	.p2align 3
.L139:
	ldr	x2, .LC28
	mov	w3, 882
	ldr	x1, .LC30
	mov	w0, 1
	bl	dprint_vfmw
	b	.L52
.L142:
	add	x0, x19, 312
	ldrb	w1, [x21, 10]
	add	x0, x0, 40960
	ldrb	w2, [x21, 16]
	add	w2, w2, w1
	ldr	w1, [x0, 260]
	add	w2, w2, 1
	add	w1, w2, w1
	str	w1, [x0, 260]
	lsl	w1, w1, 1
	str	w1, [x0, 256]
	b	.L97
.L145:
	ldr	x0, [x23]
	ldr	w2, [x26, 160]
	ldr	w3, [x0]
.L91:
	ldr	x1, .LC36
	mov	w0, 1
	bl	dprint_vfmw
	b	.L52
.L148:
	str	wzr, [x25, 2404]
	ldrb	w0, [x21, 22]
	cmp	w0, 3
	bne	.L75
.L78:
	ldrb	w28, [x21, 9]
	cmp	w28, 3
	bne	.L76
	mov	w27, w28
	mov	w24, 0
	b	.L70
.L146:
	cmp	w27, 2
	beq	.L152
.L73:
	ldrb	w28, [x21, 9]
	cmp	w28, 3
	bne	.L76
	mov	w24, 0
	b	.L70
.L143:
	add	x0, x19, 312
	add	x0, x0, 40960
	ldr	w2, [x0, 260]
	ldr	w1, [x0, 256]
	add	w2, w2, 1
	str	w2, [x0, 260]
	ldrb	w2, [x21, 16]
	add	w2, w2, 2
	add	w1, w2, w1
	str	w1, [x0, 256]
	b	.L97
.L79:
	mov	w24, 1
	str	w24, [x25, 2404]
	ldrb	w27, [x21, 22]
	ldrb	w28, [x21, 9]
	b	.L70
.L144:
	ldr	x1, .LC25
	mov	w0, 1
	mov	w24, 0
	bl	dprint_vfmw
	b	.L81
.L149:
	ldr	w0, [x25, 2404]
	cbnz	w0, .L128
	b	.L78
.L87:
	ldr	x2, .LC28
	mov	w3, 892
	ldr	x1, .LC32
	mov	w0, 1
	bl	dprint_vfmw
	b	.L52
.L150:
	ldr	x1, .LC21
	mov	w24, 0
	bl	dprint_vfmw
	b	.L81
.L152:
	ldrb	w0, [x19, 313]
	cmp	w0, 3
	beq	.L73
	ldrb	w24, [x19, 312]
	cmp	w24, 1
	bne	.L73
	ldrb	w28, [x21, 9]
	b	.L70
.L147:
	mov	w28, w27
	mov	w24, 0
	b	.L70
.L151:
	ldr	w0, [x25, 2392]
	bl	FSP_GetNullLogicFs
	stp	w0, w0, [x22, 172]
	b	.L62
.L138:
	ldrb	w1, [x21, 22]
	b	.L84
	.size	MP2_DecodingDecide, .-MP2_DecodingDecide
	.align	3
.LC10:
	.xword	g_event_report
	.align	3
.LC12:
	.xword	.LC11
	.align	3
.LC14:
	.xword	.LC13
	.align	3
.LC16:
	.xword	.LC15
	.align	3
.LC17:
	.xword	.L66
	.align	3
.LC19:
	.xword	.LC18
	.align	3
.LC21:
	.xword	.LC20
	.align	3
.LC23:
	.xword	.LC22
	.align	3
.LC25:
	.xword	.LC24
	.align	3
.LC27:
	.xword	.LC26
	.align	3
.LC28:
	.xword	.LANCHOR0
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_EstimatePicQp
	.type	MP2_EstimatePicQp, %function
MP2_EstimatePicQp:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC39
	mov	x2, 128
	add	x0, x29, 32
	add	x1, x1, 24
	bl	memcpy
	add	x1, x19, 86016
	ldr	w0, [x19, 328]
	ldr	w2, [x1, 444]
	cmp	w2, 1
	beq	.L154
	ldr	x1, [x1, 1768]
	ldrb	w1, [x1, 13]
	cbnz	w1, .L159
	lsl	w0, w0, 1
.L154:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 160
	ret
	.p2align 3
.L159:
	add	x1, x29, 160
	ldr	x19, [sp, 16]
	add	x0, x1, x0, sxtw 2
	ldr	w0, [x0, -128]
	ldp	x29, x30, [sp], 160
	ret
	.size	MP2_EstimatePicQp, .-MP2_EstimatePicQp
	.align	3
.LC39:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_GetSequenceInfo
	.type	MP2_GetSequenceInfo, %function
MP2_GetSequenceInfo:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x24, x0
	mov	x0, x30
	add	x19, x24, 45056
	add	x20, x24, 86016
	bl	_mcount
	str	wzr, [x29, 100]
	add	x0, x19, 312
	add	x0, x0, 40960
	ldr	x21, [x20, 1768]
	stp	wzr, wzr, [x29, 92]
	ldr	x25, [x20, 1776]
	ldr	w2, [x0, 132]
	cmp	w2, 1
	beq	.L162
	cmp	w2, 2
	bne	.L229
	ldr	w1, [x0, 76]
	mov	w0, 1769472
	and	w0, w1, w0
	cmp	w0, 589824
	beq	.L165
.L164:
	ldr	x1, .LC41
	mov	w0, 17
	bl	dprint_vfmw
	add	x0, x19, 312
	add	x0, x0, 40960
	ldr	w0, [x0, 132]
	cmp	w0, 1
	beq	.L230
.L167:
	ldr	w0, [x21, 48]
	add	x2, x19, 312
	add	x2, x2, 40960
	add	w0, w0, 15
	lsr	w0, w0, 4
	str	w0, [x21, 56]
	ldr	w1, [x2, 132]
	cmp	w1, 2
	beq	.L231
	ldr	w3, [x21, 52]
	add	w1, w3, 15
	lsr	w1, w1, 4
	stp	w1, wzr, [x21, 60]
	ldr	w2, [x2, 132]
	cmp	w2, 2
	beq	.L232
.L170:
	add	x2, x19, 312
	add	x2, x2, 40960
	ldr	w2, [x2, 144]
	cbz	w2, .L172
	ldr	w3, [x25, 56]
	cmp	w0, w3
	beq	.L233
.L172:
	add	x22, x19, 312
	ldr	w0, [x20, 2392]
	add	x22, x22, 40960
	ldr	w1, [x22, 176]
	bl	FSP_GetFsImagePtr
	mov	x23, x0
	cbz	x0, .L175
	ldr	w0, [x20, 2392]
	ldr	x1, [x23, 560]
	bl	FreeUsdByDec
	ldr	w0, [x20, 2392]
	ldr	x1, [x23, 568]
	bl	FreeUsdByDec
	ldr	w0, [x20, 2392]
	ldr	x1, [x23, 576]
	bl	FreeUsdByDec
	ldr	w0, [x20, 2392]
	ldr	x1, [x23, 584]
	bl	FreeUsdByDec
	add	x0, x23, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w0, [x20, 2392]
	ldr	w1, [x22, 176]
	bl	FSP_ClearLogicFs
	ldr	w0, [x20, 2392]
	mov	w2, 1
	ldr	w1, [x22, 172]
	bl	FSP_ClearLogicFs
	stp	wzr, wzr, [x22, 172]
	ldr	w0, [x20, 2392]
	bl	FSP_GetFspType
	cbnz	w0, .L234
.L175:
	add	x22, x19, 312
	add	x1, x20, 1784
	add	x22, x22, 40960
	str	wzr, [x22, 136]
	ldr	w0, [x20, 2392]
	bl	FSP_ClearNotInVoQueue
	ldr	x0, [x24]
	ldr	w1, [x0, 748]
	cbnz	w1, .L177
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L235
.L177:
	add	x0, x19, 312
	add	x0, x0, 40960
	ldr	w1, [x0, 144]
	cbz	w1, .L179
	ldr	w1, [x25, 56]
	cbz	w1, .L179
	ldr	w1, [x25, 60]
	cbnz	w1, .L236
	.p2align 2
.L179:
	ldr	w0, [x20, 2392]
	bl	FSP_GetFspType
	cbz	w0, .L220
	ldr	x23, .LC46
.L180:
	add	x22, x19, 312
	mov	w1, 0
	add	x22, x22, 40960
	mov	x0, x24
	str	wzr, [x22, 144]
	str	wzr, [x22, 160]
	bl	MP2_ArrangeVHBMem
	str	w0, [x22, 144]
	ldr	x1, .LC48
	mov	w0, 1
	ldp	w2, w3, [x21, 56]
	lsl	w3, w3, 4
	lsl	w2, w2, 4
	bl	dprint_vfmw
.L178:
	ldr	x4, [x23]
	cbz	x4, .L227
	ldp	w2, w1, [x21, 56]
	ldp	w5, w3, [x25, 56]
	ldr	w0, [x20, 2392]
	strh	w2, [x29, 108]
	add	x2, x29, 104
	strh	w1, [x29, 110]
	mov	w1, 2
	strh	w3, [x29, 106]
	mov	w3, 8
	strh	w5, [x29, 104]
	blr	x4
.L227:
	add	x0, x19, 312
	add	x0, x0, 40960
	ldr	w2, [x0, 144]
.L173:
	add	x19, x19, 312
	add	x19, x19, 40960
	str	w2, [x19, 72]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L229:
	ldr	w1, [x0, 76]
	mov	w2, 3538944
	tst	w1, w2
	beq	.L165
	mov	w2, 2
	str	w2, [x0, 132]
	tbnz	x1, 17, .L166
	orr	w1, w1, w2
	str	w1, [x0, 76]
.L166:
	mov	w2, 2
	tbnz	x1, 20, .L164
	add	x0, x19, 312
	orr	w1, w1, 16
	add	x0, x0, 40960
	str	w1, [x0, 76]
	b	.L164
	.p2align 3
.L165:
	add	x0, x19, 312
	mov	w1, 1
	add	x0, x0, 40960
	mov	w2, w1
	str	w1, [x0, 132]
	mov	w0, 17
	ldr	x1, .LC41
	bl	dprint_vfmw
	add	x0, x19, 312
	add	x0, x0, 40960
	ldr	w0, [x0, 132]
	cmp	w0, 1
	bne	.L167
.L230:
	mov	w2, 3
	mov	w1, 5
	strb	w0, [x21, 27]
	strb	w0, [x21, 18]
	strb	w2, [x21, 9]
	strb	w0, [x21, 11]
	strb	w0, [x21, 28]
	strb	wzr, [x21, 12]
	strb	wzr, [x21, 13]
	strb	wzr, [x21, 14]
	strb	wzr, [x21, 15]
	strb	wzr, [x21, 8]
	str	w1, [x21, 144]
	b	.L167
	.p2align 3
.L220:
	ldr	w0, [x20, 2392]
	add	x2, x29, 96
	add	x3, x29, 100
	add	x1, x29, 92
	bl	VCTRL_GetChanImgNum
	ldr	x0, [x24]
	ldr	w2, [x0, 748]
	cmp	w2, 1
	beq	.L237
.L225:
	ldr	x23, .LC46
.L182:
	strb	wzr, [x20, 2409]
	b	.L180
	.p2align 3
.L162:
	mov	w1, 18
	ldr	w3, [x0, 76]
	movk	w1, 0x12, lsl 16
	and	w1, w3, w1
	cmp	w1, 1179648
	bne	.L164
	mov	w2, 2
	str	w2, [x0, 132]
	b	.L164
	.p2align 3
.L231:
	ldrb	w1, [x21, 27]
	cbnz	w1, .L169
	ldr	w3, [x21, 52]
	add	w1, w3, 31
	lsr	w1, w1, 5
	lsl	w1, w1, 1
	stp	w1, wzr, [x21, 60]
	ldr	w2, [x2, 132]
	cmp	w2, 2
	bne	.L170
	add	w2, w3, 15
	mov	w3, w1
	lsr	w2, w2, 4
	b	.L171
	.p2align 3
.L232:
	ldrb	w2, [x21, 27]
	cbnz	w2, .L170
	add	w3, w3, 31
	mov	w2, w1
	lsr	w3, w3, 5
	lsl	w3, w3, 1
.L171:
	sub	w2, w3, w2
	str	w2, [x21, 64]
	b	.L170
	.p2align 3
.L169:
	ldr	w1, [x21, 52]
	add	w1, w1, 15
	lsr	w1, w1, 4
	stp	w1, wzr, [x21, 60]
	b	.L170
	.p2align 3
.L237:
	ldr	w0, [x29, 100]
	cmp	w0, wzr
	ble	.L238
.L183:
	mov	w0, 1
	strb	w0, [x20, 2409]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L236:
	ldr	x1, .LC43
	str	wzr, [x0, 144]
	mov	w0, 1
	bl	dprint_vfmw
	b	.L179
	.p2align 3
.L233:
	ldr	w0, [x25, 60]
	cmp	w0, w1
	bne	.L172
	b	.L173
	.p2align 3
.L234:
	ldr	w0, [x20, 2392]
	bl	FSP_GetNullLogicFs
	stp	w0, w0, [x22, 172]
	b	.L175
	.p2align 3
.L235:
	ldr	w0, [x22, 144]
	cbz	w0, .L239
	ldr	x23, .LC46
	b	.L178
	.p2align 3
.L239:
	mov	w1, 0
	mov	x0, x24
	bl	MP2_ArrangeVHBMem
	str	wzr, [x22, 160]
	str	w0, [x22, 144]
	ldr	x23, .LC46
	b	.L178
	.p2align 3
.L238:
	ldr	w0, [x29, 96]
	cmp	w0, 32
	bgt	.L183
	ldr	x0, [x24, 8]
	cbnz	x0, .L183
	ldr	w3, [x20, 2396]
	cbz	w3, .L225
	ldr	w6, [x21, 56]
	cbz	w6, .L225
	ldr	w5, [x20, 2416]
	ldr	w7, [x21, 60]
	cmp	w7, w5
	beq	.L225
	ldr	w4, [x20, 2412]
	ldr	x23, .LC46
	cmp	w6, w4
	beq	.L182
	ldr	x1, .LC45
	mov	w0, w2
	bl	dprint_vfmw
	ldr	w0, [x21, 60]
	str	w0, [x20, 2416]
	ldr	x4, [x23]
	ldr	w0, [x21, 56]
	str	w0, [x20, 2412]
	cbz	x4, .L182
	ldr	w0, [x20, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 120
	blr	x4
	strb	wzr, [x20, 2409]
	b	.L180
	.size	MP2_GetSequenceInfo, .-MP2_GetSequenceInfo
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
.LC48:
	.xword	.LC47
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_CalcRealEncFrameRate
	.type	MP2_CalcRealEncFrameRate, %function
MP2_CalcRealEncFrameRate:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 86016
	ldr	x3, [x0, 1768]
	ldrb	w0, [x3, 22]
	cmp	w0, 1
	beq	.L245
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L245:
	add	x19, x19, 45056
	ldr	w5, [x3, 72]
	add	x2, x19, 312
	add	x2, x2, 40960
	ldp	w0, w6, [x2, 228]
	ldp	w1, w4, [x2, 240]
	add	w0, w0, w1
	add	w4, w6, w4
	cmp	w0, wzr
	ccmp	w5, wzr, 4, gt
	ccmp	w4, wzr, 4, gt
	ble	.L242
	ldrb	w1, [x3, 27]
	cmp	w1, 1
	beq	.L246
	lsl	w2, w0, 1
	mul	w0, w2, w5
	sdiv	w1, w0, w4
.L244:
	lsl	w0, w1, 2
	add	x19, x19, 312
	sub	w0, w0, w1
	add	x19, x19, 40960
	lsl	w2, w0, 5
	add	w0, w0, w2
	add	w0, w0, w1
	add	w1, w0, 1023
	cmp	w0, wzr
	csel	w0, w1, w0, lt
	asr	w0, w0, 10
	str	w0, [x19, 264]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L242:
	str	wzr, [x2, 264]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L246:
	ldr	w3, [x2, 236]
	ldr	w1, [x2, 248]
	mul	w0, w0, w5
	add	w1, w3, w1
	sdiv	w1, w0, w1
	b	.L244
	.size	MP2_CalcRealEncFrameRate, .-MP2_CalcRealEncFrameRate
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_CalcDar
	.type	MP2_CalcDar, %function
MP2_CalcDar:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 86016
	bl	_mcount
	ldr	x0, [x19, 1768]
	ldr	w0, [x0, 84]
	sub	w0, w0, #1
	cmp	w0, 2
	bhi	.L249
	ldr	x1, .LC49
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ldr	w0, [x1, w0, uxtw 2]
	ret
	.p2align 3
.L249:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MP2_CalcDar, .-MP2_CalcDar
	.align	3
.LC49:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_SetImgFormat
	.type	MP2_SetImgFormat, %function
MP2_SetImgFormat:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	add	x21, x22, 45056
	add	x23, x22, 86016
	bl	_mcount
	add	x1, x21, 312
	add	x1, x1, 40960
	ldr	w0, [x23, 2392]
	ldr	x20, [x23, 1768]
	ldr	w1, [x1, 180]
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L250
	ldrb	w4, [x20, 22]
	cmp	w4, 4
	beq	.L266
	sub	w4, w4, #1
	and	w4, w4, 3
.L252:
	ldrb	w1, [x20, 27]
	mov	w6, 4096
	ldr	w2, [x19, 108]
	ldrb	w3, [x20, 9]
	cmp	w1, wzr
	ubfx	x2, x2, 10, 2
	cset	w5, eq
	orr	w2, w3, w2
	ldr	w0, [x20, 128]
	cbnz	w1, .L253
	cmp	w3, 3
	beq	.L281
	add	x1, x21, 312
	cmp	w3, 1
	add	x1, x1, 40960
	ldr	w1, [x1, 88]
	cbnz	w1, .L255
	cset	w3, eq
	lsl	w6, w3, 12
.L253:
	ldr	x1, [x23, 1768]
	ldr	w1, [x1, 84]
	sub	w1, w1, #1
	cmp	w1, 2
	bhi	.L268
.L284:
	ldr	x3, .LC50
	ldr	w1, [x3, w1, uxtw 2]
	ubfiz	w3, w1, 14, 3
.L256:
	ubfiz	w0, w0, 5, 2
	ubfiz	w2, w2, 10, 2
	orr	w2, w2, w4
	orr	w0, w0, w5, lsl 8
	orr	w0, w0, w2
	orr	w3, w6, w3
	orr	w0, w0, w3
	str	w0, [x19, 108]
	ldr	w2, [x20, 56]
	lsl	w2, w2, 4
	cmp	w2, 1920
	bhi	.L257
	orr	w0, w0, 131072
	str	w0, [x19, 108]
	mov	w2, 0
	ldr	x3, [x22]
	ldr	w3, [x3, 632]
	cmp	w3, 1
	beq	.L282
.L259:
	orr	w0, w0, w2
	str	w0, [x19, 108]
	stp	w4, w4, [x19, 100]
	mov	x0, x19
	bl	SetAspectRatio
	mov	w0, 3
	str	w0, [x19, 376]
	mov	w0, 4
	mov	w2, 1
	ldrb	w1, [x20, 9]
	strb	w2, [x19, 371]
	cmp	w1, 3
	csel	w0, w0, wzr, ne
	strb	w0, [x19, 364]
	ldr	x0, [x22, 8]
	cbz	x0, .L261
	ldrb	w0, [x0, 144]
	str	w0, [x19, 380]
.L262:
	str	w0, [x19, 384]
	str	w0, [x19, 388]
	mov	w0, 1
	strb	w0, [x19, 369]
	ldr	w0, [x20, 72]
	str	w0, [x19, 136]
	ldr	w2, [x20, 48]
	str	w2, [x19, 112]
	ldr	w1, [x20, 52]
	str	w1, [x19, 116]
	ldr	x0, [x22]
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L283
.L263:
	lsr	w0, w1, 1
	lsr	w3, w2, 1
	stp	w3, w0, [x19, 128]
	ldr	w0, [x20, 148]
	cbz	w0, .L264
	ldr	w3, [x20, 152]
	cbz	w3, .L264
	cmp	w0, w2
	csel	w2, w0, w2, ls
	str	w2, [x19, 120]
	ldr	w0, [x20, 152]
	cmp	w0, w1
	csel	w1, w0, w1, ls
	str	w1, [x19, 124]
.L265:
	ldrb	w1, [x20, 27]
	add	x21, x21, 312
	str	w1, [x19, 476]
	mov	x0, x22
	add	x21, x21, 40960
	ldrb	w1, [x20, 18]
	str	w1, [x19, 480]
	ldr	w1, [x20, 144]
	str	w1, [x19, 488]
	bl	MP2_CalcRealEncFrameRate
	ldr	w0, [x21, 264]
	str	w0, [x19, 484]
.L250:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L268:
	mov	w1, 0
	mov	w3, 0
	b	.L256
	.p2align 3
.L264:
	stp	w2, w1, [x19, 120]
	b	.L265
	.p2align 3
.L257:
	and	w0, w0, -131073
	str	w0, [x19, 108]
	mov	w2, 0
	ldr	x3, [x22]
	ldr	w3, [x3, 632]
	cmp	w3, 1
	bne	.L259
.L282:
	and	w3, w0, 131072
	mov	w2, 262144
	cmp	w3, wzr
	csel	w2, wzr, w2, ne
	b	.L259
	.p2align 3
.L266:
	mov	w4, 0
	b	.L252
	.p2align 3
.L255:
	ldr	x1, [x23, 1768]
	cset	w3, ne
	lsl	w6, w3, 12
	ldr	w1, [x1, 84]
	sub	w1, w1, #1
	cmp	w1, 2
	bhi	.L268
	b	.L284
	.p2align 3
.L261:
	mov	w0, 32
	strb	wzr, [x19, 371]
	str	w0, [x19, 380]
	b	.L262
	.p2align 3
.L281:
	ldr	x1, [x23, 1768]
	ldrb	w3, [x20, 10]
	ldr	w1, [x1, 84]
	ubfiz	w6, w3, 12, 1
	sub	w1, w1, #1
	cmp	w1, 2
	bhi	.L268
	b	.L284
	.p2align 3
.L283:
	ldr	w3, [x19, 140]
	add	w1, w1, 15
	lsr	w0, w1, 4
	ldr	w2, [x19, 44]
	lsl	w1, w3, 4
	ldr	x4, [x19, 504]
	str	w2, [x19, 60]
	mul	w1, w0, w1
	str	x4, [x19, 520]
	add	w0, w1, w2
	str	w0, [x19, 48]
	add	x1, x4, x1, sxtw
	str	x1, [x19, 512]
	stp	w2, w0, [x19, 28]
	add	w2, w3, w2
	add	w3, w3, w0
	stp	w2, w3, [x19, 36]
	bl	MEM_Phy2Vir
	str	x0, [x19, 528]
	ldr	w3, [x19, 32]
	ldp	w2, w1, [x19, 112]
	str	w3, [x19, 64]
	b	.L263
	.size	MP2_SetImgFormat, .-MP2_SetImgFormat
	.align	3
.LC50:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_SearchStartCode
	.type	MP2_SearchStartCode, %function
MP2_SearchStartCode:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cmp	w19, 4
	mov	w0, 0
	ble	.L286
	mov	w1, w19
	mov	x0, x20
	bl	MP2_SearchStartCode.part.1
.L286:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MP2_SearchStartCode, .-MP2_SearchStartCode
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_ProtocolPPSCheck
	.type	MP2_ProtocolPPSCheck, %function
MP2_ProtocolPPSCheck:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 45056
	add	x19, x19, 86016
	bl	_mcount
	add	x1, x20, 312
	add	x1, x1, 40960
	ldr	x3, [x19, 1768]
	ldr	w0, [x1, 132]
	cmp	w0, 1
	beq	.L290
	cmp	w0, 2
	beq	.L291
	ldr	w0, [x1, 76]
	orr	w0, w0, 63
	str	w0, [x1, 76]
	ldrb	w2, [x3, 22]
.L297:
	tst	w0, 24
	beq	.L299
.L315:
	cmp	w2, 3
	bne	.L300
.L288:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L291:
	ldr	w0, [x1, 76]
	tbnz	x0, 20, .L298
	orr	w0, w0, 16
	str	w0, [x1, 76]
.L298:
	ldrb	w2, [x3, 22]
	sub	w1, w2, #1
	uxtb	w1, w1
	cmp	w1, 2
	bls	.L297
.L313:
	add	x1, x20, 312
	orr	w0, w0, 8
	add	x1, x1, 40960
	tst	w0, 24
	str	w0, [x1, 76]
	ldrb	w2, [x3, 22]
	bne	.L315
.L299:
	cmp	w2, 1
	bne	.L288
	add	x20, x20, 312
	and	w0, w0, -193
	add	x20, x20, 40960
	str	w0, [x20, 76]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L290:
	ldrb	w2, [x3, 22]
	sub	w0, w2, #1
	uxtb	w0, w0
	cmp	w0, 3
	ldr	w0, [x1, 76]
	bhi	.L316
.L293:
	sub	w1, w2, #2
	uxtb	w1, w1
	cmp	w1, 1
	bls	.L317
.L294:
	cmp	w2, 3
	beq	.L318
	tst	w0, 24
	beq	.L299
.L300:
	add	x21, x20, 312
	orr	w0, w0, 64
	add	x21, x21, 40960
	str	w0, [x21, 76]
	ldr	w1, [x21, 176]
	ldr	w0, [x19, 2392]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L303
	ldr	w0, [x19, 2392]
	ldr	x1, [x22, 560]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2392]
	ldr	x1, [x22, 568]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2392]
	ldr	x1, [x22, 576]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2392]
	ldr	x1, [x22, 584]
	bl	FreeUsdByDec
	add	x0, x22, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w0, [x19, 2392]
	ldr	w1, [x21, 176]
	bl	FSP_ClearLogicFs
	ldr	w0, [x19, 2392]
	mov	w2, 1
	ldr	w1, [x21, 172]
	bl	FSP_ClearLogicFs
	stp	wzr, wzr, [x21, 172]
	ldr	w0, [x19, 2392]
	bl	FSP_GetFspType
	cbnz	w0, .L319
.L303:
	add	x20, x20, 312
	add	x20, x20, 40960
	str	wzr, [x20, 136]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L316:
	orr	w0, w0, 8
	str	w0, [x1, 76]
	ldrb	w2, [x3, 22]
	b	.L293
	.p2align 3
.L319:
	ldr	w0, [x19, 2392]
	bl	FSP_GetNullLogicFs
	stp	w0, w0, [x21, 172]
	b	.L303
	.p2align 3
.L318:
	ldrb	w1, [x3, 26]
	cbnz	w1, .L288
	b	.L313
	.p2align 3
.L317:
	ldrb	w1, [x3, 24]
	cbnz	w1, .L294
	add	x1, x20, 312
	orr	w0, w0, 8
	add	x1, x1, 40960
	str	w0, [x1, 76]
	ldrb	w2, [x3, 22]
	b	.L294
	.size	MP2_ProtocolPPSCheck, .-MP2_ProtocolPPSCheck
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_GetImageBuffer
	.type	MP2_GetImageBuffer, %function
MP2_GetImageBuffer:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x1, x0
	mov	x0, x30
	add	x21, x1, 86016
	add	x19, x1, 45056
	add	x20, x19, 312
	bl	_mcount
	add	x20, x20, 40960
	mov	w1, 1
	ldr	w0, [x21, 2392]
	bl	FSP_NewLogicFs
	str	w0, [x20, 180]
	mov	w1, w0
	tbnz	w0, #31, .L333
	ldr	w0, [x21, 2392]
	bl	FSP_GetLogicFs
	mov	x19, x0
	cbz	x0, .L334
	ldr	x1, .LC56
	mov	w0, 18
	ldr	w2, [x20, 180]
	bl	dprint_vfmw
	ldr	x0, [x19, 640]
	cbz	x0, .L329
	ldr	x1, [x19, 648]
	cbz	x1, .L329
	ldr	x2, [x19, 656]
	cbz	x2, .L329
	ldr	w3, [x1, 16]
	ldr	w4, [x2, 16]
	ldr	x1, .LC58
	ldr	w2, [x0, 16]
	mov	w0, 18
	bl	dprint_vfmw
	mov	w0, 1
.L325:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L329:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L333:
	ldr	x1, .LC52
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x20, 176]
	ldr	w0, [x21, 2392]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L323
	ldr	w0, [x21, 2392]
	ldr	x1, [x22, 560]
	bl	FreeUsdByDec
	ldr	w0, [x21, 2392]
	ldr	x1, [x22, 568]
	bl	FreeUsdByDec
	ldr	w0, [x21, 2392]
	ldr	x1, [x22, 576]
	bl	FreeUsdByDec
	ldr	w0, [x21, 2392]
	ldr	x1, [x22, 584]
	bl	FreeUsdByDec
	add	x0, x22, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w1, [x20, 176]
	ldr	w0, [x21, 2392]
	bl	FSP_ClearLogicFs
	ldr	w0, [x21, 2392]
	mov	w2, 1
	ldr	w1, [x20, 172]
	bl	FSP_ClearLogicFs
	stp	wzr, wzr, [x20, 172]
	ldr	w0, [x21, 2392]
	bl	FSP_GetFspType
	cbnz	w0, .L335
.L323:
	add	x19, x19, 312
	add	x1, x21, 1784
	add	x19, x19, 40960
	str	wzr, [x19, 136]
	ldr	w0, [x21, 2392]
	bl	FSP_ClearNotInVoQueue
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L335:
	ldr	w0, [x21, 2392]
	bl	FSP_GetNullLogicFs
	stp	w0, w0, [x20, 172]
	b	.L323
	.p2align 3
.L334:
	ldr	x1, .LC54
	mov	w2, 1903
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L325
	.size	MP2_GetImageBuffer, .-MP2_GetImageBuffer
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_GetPictureInfo
	.type	MP2_GetPictureInfo, %function
MP2_GetPictureInfo:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x23, x0
	mov	x0, x30
	add	x22, x23, 45056
	add	x19, x23, 86016
	bl	_mcount
	add	x0, x22, 312
	add	x0, x0, 40960
	ldr	x21, [x19, 1768]
	ldr	x24, [x19, 1776]
	ldr	w0, [x0, 132]
	cmp	w0, 1
	beq	.L391
.L337:
	ldrb	w0, [x21, 9]
	cmp	w0, 3
	beq	.L338
	add	x20, x22, 312
	add	x20, x20, 40960
	ldr	w0, [x20, 88]
	ldr	w1, [x20, 140]
	cmp	w0, wzr
	cset	w0, eq
	str	w0, [x20, 88]
	ldr	w2, [x21, 156]
	cmp	w2, w1
	cset	w1, ne
	tst	w0, w1
	bne	.L392
.L340:
	ldrb	w2, [x21, 9]
	add	x0, x22, 312
	strb	w2, [x22, 313]
	add	x1, x0, 40960
	add	x20, x23, 16
	ldr	w0, [x21, 156]
	str	w0, [x1, 140]
	ldr	w0, [x21, 56]
	strh	w0, [x20, 152]
	ldrb	w0, [x21, 9]
	cmp	w0, 3
	ldr	w0, [x21, 60]
	beq	.L345
	lsr	w0, w0, 1
	strh	w0, [x20, 148]
	ldr	w0, [x21, 60]
	strh	w0, [x20, 150]
.L346:
	ldrb	w1, [x21, 11]
	add	x0, x22, 312
	strb	w1, [x20, 5]
	add	x0, x0, 40960
	ldrb	w1, [x21, 9]
	strb	w1, [x20, 3]
	ldr	w1, [x0, 88]
	strb	w1, [x20, 4]
	ldrb	w2, [x21, 12]
	strb	w2, [x20, 7]
	ldrb	w2, [x21, 22]
	strb	w2, [x23, 16]
	ldr	w0, [x0, 132]
	cmp	w0, 1
	cset	w2, eq
	str	w2, [x20, 180]
	beq	.L393
.L347:
	ldrb	w0, [x21, 4]
	strb	w0, [x20, 12]
	ldrb	w0, [x21, 5]
	strb	w0, [x20, 13]
	ldrb	w0, [x21, 6]
	strb	w0, [x20, 14]
	ldrb	w0, [x21, 7]
	strb	w0, [x20, 15]
	ldrb	w0, [x21, 10]
	strb	w0, [x20, 6]
	ldrb	w0, [x21, 8]
	strb	w0, [x20, 11]
	ldrb	w0, [x21, 13]
	strb	w0, [x20, 10]
	ldrb	w0, [x21, 14]
	strb	w0, [x20, 9]
	ldrb	w0, [x21, 15]
	strb	w0, [x20, 8]
	ldrb	w0, [x21, 9]
	cmp	w0, 3
	ccmp	w1, wzr, 4, ne
	bne	.L349
	mov	x1, 0
	mov	x0, x23
	bl	MP2_GetImageBuffer
	mov	w24, w0
	cbz	w0, .L394
	add	x1, x22, 312
	ldr	w0, [x19, 2392]
	add	x1, x1, 40960
	ldr	w1, [x1, 180]
	bl	FSP_GetFsImagePtr
	mov	x24, x0
	cbz	x0, .L352
	ldr	x2, [x23]
	mov	x3, -1
	ldr	x1, .LC69
	mov	w0, 29
	ldr	x2, [x2, 56]
	str	x2, [x24, 600]
	ldr	x2, [x23]
	ldr	x2, [x2, 72]
	str	x2, [x24, 608]
	ldr	x2, [x23]
	ldr	x2, [x2, 88]
	str	x2, [x24, 616]
	ldr	x2, [x23]
	ldr	w2, [x2, 104]
	str	w2, [x24, 8]
	ldr	x2, [x23]
	ldr	w2, [x2, 108]
	str	w2, [x24, 12]
	ldr	x2, [x23]
	ldr	w2, [x2, 112]
	str	w2, [x24, 16]
	ldr	x2, [x23]
	ldr	w2, [x2, 128]
	str	w2, [x24, 20]
	ldr	x2, [x23]
	str	x3, [x2, 56]
	ldr	x2, [x24, 600]
	ldr	x3, [x24, 608]
	bl	dprint_vfmw
	ldrb	w0, [x21, 9]
	cmp	w0, 3
	cset	w0, ne
	str	w0, [x24, 96]
.L352:
	ldrb	w0, [x21, 22]
	cmp	w0, 3
	beq	.L349
	add	x21, x22, 312
	mov	w0, 2
	add	x21, x21, 40960
	ldr	w1, [x21, 136]
	ldp	w2, w3, [x21, 172]
	cmp	w1, wzr
	csinc	w0, w0, wzr, ne
	cmp	w2, w3
	str	w0, [x21, 136]
	beq	.L355
	ldr	x1, .LC71
	mov	w0, 13
	bl	dprint_vfmw
	ldr	w0, [x19, 2392]
	mov	w2, 0
	ldr	w1, [x21, 172]
	bl	FSP_SetRef
	ldr	w3, [x21, 176]
.L355:
	add	x21, x22, 312
	mov	w2, 1
	add	x21, x21, 40960
	ldr	w1, [x21, 180]
	stp	w3, w1, [x21, 172]
	ldr	w0, [x19, 2392]
	bl	FSP_SetRef
	ldr	w0, [x19, 2392]
	ldr	w1, [x21, 172]
	bl	FSP_RemovePmv
	ldr	x1, .LC73
	mov	w0, 13
	ldr	w2, [x21, 180]
	bl	dprint_vfmw
.L349:
	add	x24, x22, 312
	ldr	w0, [x19, 2392]
	add	x24, x24, 40960
	ldr	w1, [x24, 180]
	bl	FSP_GetLogicFs
	mov	x21, x0
	ldr	w1, [x24, 172]
	ldr	w0, [x19, 2392]
	bl	FSP_GetLogicFs
	mov	x25, x0
	ldr	w1, [x24, 176]
	ldr	w0, [x19, 2392]
	bl	FSP_GetLogicFs
	mov	x26, x0
	cmp	x21, xzr
	ccmp	x25, xzr, 4, ne
	ccmp	x0, xzr, 4, ne
	beq	.L395
	ldr	x1, [x21, 640]
	cbz	x1, .L357
	ldr	x0, [x21, 656]
	cbz	x0, .L357
	ldr	x0, [x21, 648]
	cbz	x0, .L357
	ldr	x0, [x26, 640]
	cbz	x0, .L359
	ldr	w5, [x0, 16]
.L360:
	str	w5, [x20, 192]
	ldr	x0, [x25, 640]
	cbz	x0, .L396
.L388:
	ldr	w4, [x0, 16]
	str	w4, [x20, 196]
	ldr	x0, [x21, 640]
	ldr	w2, [x0, 16]
	str	w2, [x20, 200]
	ldr	w6, [x21, 664]
	str	w6, [x20, 260]
	ldr	x0, [x21, 640]
	ldr	w0, [x0, 36]
	str	w0, [x20, 232]
	ldr	x0, [x21, 640]
	ldr	w0, [x0, 40]
	str	w0, [x20, 236]
	ldr	x0, [x26, 656]
	cbz	x0, .L397
.L389:
	ldr	w0, [x0, 16]
	str	w0, [x20, 212]
	ldr	x0, [x25, 656]
	cbz	x0, .L398
.L390:
	ldr	w0, [x0, 16]
	add	x7, x20, 45056
	str	w0, [x20, 216]
	ldr	x1, .LC79
	ldr	x0, [x21, 656]
	ldr	w0, [x0, 16]
	str	w0, [x20, 220]
	mov	w0, 13
	ldr	x3, [x21, 648]
	ldr	w3, [x3, 16]
	str	w3, [x20, 224]
	ldr	w8, [x26, 104]
	str	w8, [x20, 204]
	ldr	w8, [x25, 104]
	str	w8, [x20, 208]
	ldr	x8, [x21, 656]
	ldr	w8, [x8, 16]
	str	w8, [x7, 276]
	bl	dprint_vfmw
	ldr	x2, [x21, 648]
	add	x0, x19, 704
	add	x1, x23, 96
	add	x3, x19, 960
	ldr	w2, [x2, 32]
	str	w2, [x20, 240]
	ldr	x2, [x21, 648]
	ldr	w2, [x2, 76]
	str	w2, [x20, 244]
	ldr	x2, [x21, 648]
	ldr	w2, [x2, 60]
	str	w2, [x20, 248]
	ldr	x2, [x21, 648]
	ldr	w2, [x2, 64]
	str	w2, [x20, 252]
	.p2align 2
.L367:
	ldr	w2, [x0]
	add	x0, x0, 4
	strb	w2, [x1]
	add	x1, x1, 1
	cmp	x0, x3
	ldr	w2, [x0, 252]
	strb	w2, [x1, -65]
	bne	.L367
	mov	x0, x23
	mov	w24, 1
	bl	MP2_EstimatePicQp
	strb	w0, [x20, 144]
	mov	w0, w24
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L394:
	add	x2, x22, 312
	ldr	x1, .LC67
	add	x2, x2, 40960
	ldr	w2, [x2, 84]
	bl	dprint_vfmw
	mov	w0, w24
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L393:
	ldrb	w0, [x21, 25]
	strb	w0, [x20, 1]
	ldrb	w0, [x21, 23]
	strb	w0, [x20, 2]
	b	.L347
	.p2align 3
.L345:
	ldr	w1, [x21, 64]
	sub	w0, w0, w1
	strh	w0, [x20, 148]
	ldr	w0, [x21, 60]
	strh	w0, [x20, 150]
	b	.L346
	.p2align 3
.L338:
	ldrb	w4, [x22, 313]
	add	x0, x22, 312
	cmp	w4, 3
	beq	.L343
	add	x20, x0, 40960
	ldr	w0, [x20, 88]
	cbz	w0, .L399
.L343:
	add	x0, x22, 312
	mov	w1, 1
	add	x0, x0, 40960
	str	w1, [x0, 88]
	b	.L340
	.p2align 3
.L391:
	ldrb	w1, [x21, 26]
	ldrb	w0, [x21, 24]
	strb	w1, [x21, 7]
	strb	w1, [x21, 6]
	strb	w0, [x21, 5]
	strb	w0, [x21, 4]
	b	.L337
.L395:
	ldr	w0, [x19, 2392]
	mov	w2, 1
	ldr	w1, [x24, 180]
	mov	w24, 0
	bl	FSP_ClearLogicFs
	ldr	x1, .LC75
	mov	x5, x26
	mov	x4, x25
	mov	x3, x21
	mov	w2, 1183
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, w24
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L399:
	ldr	x1, .LC65
	mov	w3, 0
	ldr	w2, [x20, 84]
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x19, 2392]
	mov	w2, 1
	ldr	w1, [x20, 180]
	bl	FSP_ClearLogicFs
	ldrb	w0, [x24, 22]
	cmp	w0, 3
	beq	.L343
	ldr	w0, [x20, 76]
	orr	w0, w0, 192
	str	w0, [x20, 76]
	b	.L343
.L357:
	add	x22, x22, 312
	ldr	w0, [x19, 2392]
	add	x19, x22, 40960
	mov	w2, 1
	mov	w24, 0
	ldr	w1, [x19, 180]
	bl	FSP_ClearLogicFs
	ldp	w2, w4, [x19, 172]
	ldr	w3, [x19, 180]
	mov	w0, 0
	ldr	x5, [x21, 640]
	ldr	x7, [x21, 648]
	ldr	x6, [x21, 656]
	ldr	x1, .LC77
	bl	dprint_vfmw
	mov	w0, w24
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L392:
	ldr	x1, .LC63
	mov	w0, 1
	ldr	w2, [x20, 84]
	bl	dprint_vfmw
	str	wzr, [x20, 88]
	ldr	w1, [x20, 180]
	mov	w2, 1
	ldr	w0, [x19, 2392]
	bl	FSP_ClearLogicFs
	ldrb	w0, [x24, 22]
	cmp	w0, 3
	beq	.L340
	ldr	w0, [x20, 76]
	orr	w0, w0, 192
	str	w0, [x20, 76]
	b	.L340
.L398:
	ldr	x0, [x21, 656]
	b	.L390
.L359:
	ldr	w5, [x1, 16]
	b	.L360
.L397:
	ldr	x0, [x21, 656]
	b	.L389
.L396:
	ldr	x0, [x21, 640]
	b	.L388
	.size	MP2_GetPictureInfo, .-MP2_GetPictureInfo
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_SetVDMInf
	.type	MP2_SetVDMInf, %function
MP2_SetVDMInf:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, 20792
	mov	x19, x0
	mov	x0, x30
	movk	x20, 0x1, lsl 16
	add	x20, x19, x20
	bl	_mcount
	add	x0, x19, 86016
	add	x0, x0, 664
	ldrsb	w1, [x20, 16]
	ldr	w3, [x20, 8]
	cmp	w1, 1
	ldr	w4, [x0, 32]
	ldr	w1, [x20, 20]
	lsr	w2, w4, 3
	beq	.L403
	add	w3, w3, w2
	add	w2, w1, w2
	str	w3, [x20, 8]
	add	w1, w4, w1, lsl 3
	ldr	w21, [x20, 24]
	and	w2, w2, -4
	and	w1, w1, 31
	str	w2, [x19, 172]
	str	w1, [x19, 180]
	and	w21, w21, -4
	bl	BsResidBits
	str	w21, [x19, 176]
	str	w0, [x19, 188]
	ldr	w0, [x20, 24]
	ubfiz	w0, w0, 3, 2
	str	w0, [x19, 184]
	ldr	w0, [x20, 32]
	lsl	w0, w0, 3
	str	w0, [x19, 192]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L403:
	add	w3, w3, w2
	add	w2, w1, w2
	str	w3, [x20, 8]
	add	w1, w4, w1, lsl 3
	and	w2, w2, -4
	and	w1, w1, 31
	str	w2, [x19, 172]
	str	w1, [x19, 180]
	bl	BsResidBits
	str	wzr, [x19, 176]
	stp	wzr, w0, [x19, 184]
	str	wzr, [x19, 192]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MP2_SetVDMInf, .-MP2_SetVDMInf
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_DecNewPic
	.type	MP2_DecNewPic, %function
MP2_DecNewPic:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	MP2_SetVDMInf
	ldr	x0, [x19]
	add	x2, x19, 45056
	add	x1, x19, 86016
	ldr	w3, [x0, 632]
	str	w3, [x2, 296]
	ldr	w3, [x0, 672]
	str	w3, [x2, 300]
	ldr	w3, [x0, 636]
	str	w3, [x2, 304]
	ldr	x2, [x1, 1768]
	ldrb	w3, [x2, 22]
	cmp	w3, 3
	beq	.L422
	cmp	w3, 2
	beq	.L423
.L406:
	add	x4, x19, 16
	str	x4, [x19, 8]
	ldr	w3, [x0, 904]
	cmp	w3, 1
	beq	.L424
.L408:
	cmp	w3, 2
	beq	.L425
.L409:
	ldr	w0, [x1, 388]
	mov	w20, 1
	and	w0, w0, -16252929
	str	w0, [x1, 388]
	ldrb	w0, [x2, 22]
	cmp	w0, w20
	beq	.L426
.L407:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L423:
	mov	w3, 1
	str	w3, [x1, 2404]
	add	x4, x19, 16
	str	x4, [x19, 8]
	ldr	w3, [x0, 904]
	cmp	w3, 1
	bne	.L408
.L424:
	cbz	x4, .L409
	ldrb	w0, [x2, 22]
	cmp	w0, 3
	bne	.L409
	mov	w20, 0
	str	xzr, [x19, 8]
	b	.L407
	.p2align 3
.L426:
	ldr	x0, .LC80
	ldr	x5, [x0]
	cbz	x5, .L407
	ldp	w4, w3, [x19, 188]
	add	x2, x29, 48
	ldr	w0, [x1, 2392]
	add	w4, w4, w3
	mov	w3, 4
	cmp	w4, wzr
	add	w1, w4, 7
	csel	w4, w1, w4, lt
	mov	w1, 20
	asr	w4, w4, 3
	str	w4, [x2, -8]!
	blr	x5
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L422:
	ldr	w3, [x1, 2404]
	cbnz	w3, .L406
	ldr	w3, [x1, 448]
	cmp	w3, 2
	beq	.L406
	ldrb	w0, [x2, 9]
	cmp	w0, 3
	beq	.L410
	ldr	w0, [x1, 400]
	mov	w20, 0
	cmp	w0, wzr
	cset	w0, eq
	str	w0, [x1, 400]
	b	.L407
	.p2align 3
.L425:
	ldr	w3, [x0, 908]
	cmp	w3, wzr
	ccmp	x4, xzr, 4, gt
	beq	.L409
	ldrb	w3, [x2, 22]
	cmp	w3, 3
	bne	.L409
	str	xzr, [x19, 8]
	mov	w20, 0
	ldr	w1, [x0, 908]
	sub	w1, w1, #1
	str	w1, [x0, 908]
	b	.L407
	.p2align 3
.L410:
	mov	w20, 0
	b	.L407
	.size	MP2_DecNewPic, .-MP2_DecNewPic
	.align	3
.LC80:
	.xword	g_event_report
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_RecordVFMWState
	.type	MP2_RecordVFMWState, %function
MP2_RecordVFMWState:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 86016
	ldr	x2, [x19]
	ldr	x1, [x0, 1768]
	ldr	w3, [x1, 40]
	str	w3, [x2, 608]
	ldr	x2, [x19]
	ldr	w1, [x1, 44]
	str	w1, [x2, 612]
	ldr	w2, [x0, 2396]
	ldr	w1, [x0, 2400]
	ldr	x3, [x19]
	sub	w0, w2, w1
	cmp	w2, w1
	csel	w0, w0, wzr, ge
	str	w0, [x3, 616]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MP2_RecordVFMWState, .-MP2_RecordVFMWState
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_GetUsrData
	.type	MP2_GetUsrData, %function
MP2_GetUsrData:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x24, x0
	mov	x0, x30
	add	x23, x24, 86016
	bl	_mcount
	add	x20, x23, 664
	mov	x0, x20
	bl	BsToNextByte
	mov	x0, x20
	bl	BsResidBits
	cmp	w0, wzr
	add	w1, w0, 7
	csel	w22, w1, w0, lt
	asr	w22, w22, 3
	cmp	w22, wzr
	ble	.L449
	ldr	x25, .LC83
	mov	w21, 0
	mov	w19, 0
	b	.L435
	.p2align 3
.L466:
	bl	BsGet
	add	w19, w19, 1
	strb	w0, [x25, w21, sxtw]
	cmp	w22, w19
	add	w21, w21, 1
	beq	.L431
.L435:
	mov	w1, 24
	mov	x0, x20
	bl	BsShow
	and	w2, w0, 16777215
	cmp	w2, 1
	mov	w1, 8
	mov	x0, x20
	beq	.L450
	cmp	w21, 1023
	ble	.L466
	mov	w1, 8
	mov	x0, x20
	add	w19, w19, 1
	bl	BsGet
	cmp	w22, w19
	bne	.L435
.L431:
	add	x22, x24, 45056
	add	x25, x22, 312
	add	x25, x25, 40960
	ldrsb	w0, [x25, 16]
	cbz	w0, .L448
.L465:
	mov	w19, 0
.L432:
	add	x0, x22, 312
	add	x0, x0, 40960
	ldr	w20, [x0, 128]
	cmp	w20, 3
	ble	.L440
	add	x20, x23, 408
	add	x25, x23, 440
.L441:
	ldr	x1, [x20]
	ldr	w0, [x23, 2392]
	bl	FreeUsdByDec
	str	xzr, [x20], 8
	cmp	x20, x25
	bne	.L441
	add	x0, x22, 312
	mov	w20, 0
	add	x0, x0, 40960
	str	wzr, [x0, 128]
.L440:
	ldr	w0, [x23, 2392]
	add	x20, x24, x20, uxtw 3
	add	x20, x20, 86016
	bl	GetUsd
	add	x3, x22, 312
	str	x0, [x20, 408]
	add	x4, x3, 40960
	ldr	w2, [x4, 128]
	sxtw	x1, w2
	add	x1, x1, 4096
	add	x1, x1, 1036
	ldr	x1, [x3, x1, lsl 3]
	cbz	x1, .L442
	ldr	x1, [x24]
	ldr	x1, [x1, 56]
	str	x1, [x0, 1056]
	ldr	x0, [x20, 408]
	ldr	w1, [x4, 80]
	str	w1, [x0, 1044]
	ldr	x0, [x20, 408]
	str	w21, [x0, 1052]
	ldr	x1, [x23, 1768]
	ldr	x0, [x20, 408]
	ldrb	w1, [x1, 22]
	strb	w1, [x0, 1024]
	ldr	x0, [x20, 408]
	ldr	w1, [x23, 2396]
	str	w1, [x0, 1032]
	ldr	x1, [x23, 1768]
	ldr	x0, [x20, 408]
	ldrb	w1, [x1, 10]
	strb	w1, [x0, 1025]
	ldr	w0, [x4, 76]
	tbz	x0, 19, .L443
	ldr	x0, [x20, 408]
	mov	w1, 3
	str	w1, [x0, 1040]
.L444:
	ldr	x0, [x20, 408]
	ldr	w2, [x0, 1052]
	cmp	w2, 1024
	ble	.L445
	mov	w1, 1024
	str	w1, [x0, 1052]
	ldr	x0, [x20, 408]
	ldr	w2, [x0, 1052]
.L445:
	ldr	x3, .LC86
	sxtw	x2, w2
	ldr	x1, .LC83
	ldr	x3, [x3, 104]
	blr	x3
	ldr	x0, .LC87
	ldr	x4, [x0]
	cbz	x4, .L446
	ldr	w0, [x23, 2392]
	mov	w3, 1064
	ldr	x2, [x20, 408]
	mov	w1, 6
	blr	x4
.L446:
	add	x22, x22, 312
	add	x22, x22, 40960
	ldr	w0, [x22, 128]
	add	w0, w0, 1
	str	w0, [x22, 128]
.L464:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L448:
	ldrsb	w0, [x25, 17]
	mov	w19, 0
	cbz	w0, .L432
	ldr	w19, [x25, 32]
	cbz	w19, .L432
	ldr	x1, .LC85
	mov	w0, 0
	mov	w19, 1
	bl	dprint_vfmw
	ldr	x2, .LC86
	ldrsw	x1, [x25, 28]
	ldp	x3, x0, [x25, 48]
	sub	x1, x1, #3
	add	x1, x3, x1
	ldr	x3, [x2, 104]
	sub	x0, x0, #3
	mov	x2, 3
	blr	x3
	strb	w19, [x25, 16]
	ldr	w0, [x25, 24]
	ldr	w2, [x25, 32]
	ldr	x1, [x25, 56]
	sub	w0, w0, #3
	add	w2, w2, 3
	strb	wzr, [x25, 17]
	sub	x1, x1, #3
	stp	w2, wzr, [x25, 28]
	stp	x1, xzr, [x25, 48]
	stp	w0, wzr, [x25, 20]
	mov	x0, x20
	bl	BsInit
	strb	w19, [x25, 17]
	strb	wzr, [x25, 16]
	mov	x0, x20
	bl	BsToNextByte
	mov	x0, x20
	bl	BsResidBits
	cmp	w0, wzr
	add	w1, w0, 7
	csel	w19, w1, w0, lt
	asr	w19, w19, 3
	sub	w19, w19, #3
	cmp	w19, wzr
	ble	.L465
	ldr	x26, .LC83
	mov	w25, 0
	b	.L438
	.p2align 3
.L467:
	bl	BsGet
	strb	w0, [x26, w21, sxtw]
	add	w21, w21, 1
.L437:
	add	w25, w25, 1
	cmp	w19, w25
	beq	.L465
.L438:
	mov	w1, 24
	mov	x0, x20
	bl	BsShow
	and	w2, w0, 16777215
	cmp	w2, 1
	mov	w1, 8
	mov	x0, x20
	beq	.L453
	cmp	w21, 1023
	ble	.L467
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	b	.L437
	.p2align 3
.L450:
	mov	w19, w2
	add	x22, x24, 45056
	b	.L432
	.p2align 3
.L443:
	ldr	x0, [x20, 408]
	mov	w1, 1
	str	w1, [x0, 1040]
	b	.L444
.L442:
	ldr	x1, .LC89
	mov	w0, 0
	bl	dprint_vfmw
	b	.L464
.L449:
	mov	w21, 0
	b	.L431
.L453:
	mov	w19, w2
	b	.L432
	.size	MP2_GetUsrData, .-MP2_GetUsrData
	.align	3
.LC83:
	.xword	.LANCHOR2
	.align	3
.LC85:
	.xword	.LC84
	.align	3
.LC86:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC87:
	.xword	g_event_report
	.align	3
.LC89:
	.xword	.LC88
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	next_start_code
	.type	next_start_code, %function
next_start_code:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	add	x20, x22, 86016
	bl	_mcount
	add	x20, x20, 664
	mov	x0, x20
	bl	BsToNextByte
	mov	x0, x20
	bl	BsGetNextBytePtr
	mov	x19, x0
	mov	x0, x20
	bl	BsResidBits
	cmp	w0, wzr
	add	w1, w0, 7
	csel	w2, w1, w0, lt
	mov	w1, 0
	asr	w2, w2, 3
	cmp	w2, 4
	bgt	.L494
.L469:
	mov	x0, x20
	mov	w19, 0
	bl	BsLongSkip
	mov	x0, x20
	bl	BsResidBits
	cmp	w0, wzr
	add	w1, w0, 7
	csel	w21, w1, w0, lt
	asr	w21, w21, 3
	sub	w21, w21, #3
	cmp	w21, wzr
	bgt	.L488
	b	.L476
	.p2align 3
.L473:
	bl	BsSkip
	cmp	w19, w21
	beq	.L476
.L488:
	mov	w1, 24
	mov	x0, x20
	add	w19, w19, 1
	bl	BsShow
	cmp	w0, 1
	mov	w1, 8
	mov	x0, x20
	bne	.L473
.L478:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L476:
	mov	x0, 20792
	movk	x0, 0x1, lsl 16
	add	x22, x22, x0
	ldrsb	w0, [x22, 16]
	cbnz	w0, .L472
	ldrsb	w0, [x22, 17]
	cbz	w0, .L472
	ldr	w0, [x22, 32]
	cbz	w0, .L472
	ldr	x1, .LC90
	mov	w0, 7
	mov	w19, 1
	bl	dprint_vfmw
	ldr	x3, .LC91
	mov	x2, 3
	ldrsw	x1, [x22, 28]
	ldp	x4, x0, [x22, 48]
	ldr	x3, [x3, 104]
	sub	x1, x1, #3
	add	x1, x4, x1
	sub	x0, x0, #3
	blr	x3
	strb	w19, [x22, 16]
	ldr	w3, [x22, 32]
	mov	x0, x20
	ldr	x4, [x22, 56]
	ldr	w5, [x22, 24]
	add	w3, w3, 3
	sub	x4, x4, #3
	mov	w2, w3
	sub	w5, w5, #3
	mov	x1, x4
	stp	w5, wzr, [x22, 20]
	stp	w3, wzr, [x22, 28]
	stp	x4, xzr, [x22, 48]
	strb	wzr, [x22, 17]
	bl	BsInit
	strb	w19, [x22, 17]
	strb	wzr, [x22, 16]
	mov	x0, x20
	bl	BsToNextByte
	mov	x0, x20
	bl	BsGetNextBytePtr
	mov	x19, x0
	mov	x0, x20
	bl	BsResidBits
	cmp	w0, wzr
	add	w1, w0, 7
	csel	w2, w1, w0, lt
	mov	w1, 0
	asr	w2, w2, 3
	cmp	w2, 4
	bgt	.L495
.L477:
	mov	x0, x20
	bl	BsLongSkip
	mov	x0, x20
	bl	BsResidBits
	cmp	w0, wzr
	add	w1, w0, 7
	csel	w21, w1, w0, lt
	asr	w21, w21, 3
	sub	w21, w21, #3
	cmp	w21, wzr
	ble	.L472
	mov	w19, 0
	b	.L479
	.p2align 3
.L496:
	bl	BsSkip
	cmp	w19, w21
	beq	.L472
.L479:
	mov	w1, 24
	mov	x0, x20
	add	w19, w19, 1
	bl	BsShow
	cmp	w0, 1
	mov	w1, 8
	mov	x0, x20
	bne	.L496
	b	.L478
	.p2align 3
.L472:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L494:
	mov	w1, w2
	mov	x0, x19
	bl	MP2_SearchStartCode.part.1
	lsl	w1, w0, 3
	b	.L469
.L495:
	mov	w1, w2
	mov	x0, x19
	bl	MP2_SearchStartCode.part.1
	lsl	w1, w0, 3
	b	.L477
	.size	next_start_code, .-next_start_code
	.align	3
.LC90:
	.xword	.LC84
	.align	3
.LC91:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetSliceHdr
	.type	GetSliceHdr, %function
GetSliceHdr:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x22, 20792
	mov	x21, x0
	mov	x0, x30
	add	x23, x21, 86016
	movk	x22, 0x1, lsl 16
	add	x20, x23, 664
	bl	_mcount
	add	x22, x21, x22
	b	.L498
	.p2align 3
.L505:
	bl	BsShow
	mov	w19, w0
	mov	x0, x20
	sub	w19, w19, #257
	bl	BsPos
	str	w0, [x22, 156]
	cmp	w19, 174
	mov	w1, 32
	mov	x0, x20
	bls	.L513
	bl	BsSkip
.L498:
	mov	x0, x21
	bl	next_start_code
	mov	w2, w0
	cmp	w2, 1
	mov	w1, 32
	mov	x0, x20
	beq	.L505
	cmn	w2, #1
	mov	w19, w2
	beq	.L500
.L503:
	mov	w0, w19
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L507:
	mov	w19, -1
.L500:
	ldr	x0, .LC92
	ldr	x4, [x0]
	cbz	x4, .L503
	ldr	w0, [x23, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	ldr	x23, [sp, 48]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L513:
	bl	BsResidBits
	cmp	w0, 40
	ble	.L507
	ldr	w0, [x22, 72]
	str	wzr, [x22, 152]
	cbnz	w0, .L504
	mov	x0, x21
	mov	w19, 0
	bl	MP2_GetSequenceInfo
	ldr	w0, [x22, 144]
	cbz	w0, .L503
.L504:
	mov	x0, x21
	mov	w19, 1
	bl	MP2_ProtocolPPSCheck
	ldr	x23, [sp, 48]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.size	GetSliceHdr, .-GetSliceHdr
	.align	3
.LC92:
	.xword	g_event_report
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	sequence_header
	.type	sequence_header, %function
sequence_header:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x19, 20792
	mov	x21, x0
	movk	x19, 0x1, lsl 16
	mov	x0, x30
	add	x23, x21, 86016
	add	x19, x21, x19
	bl	_mcount
	add	x20, x23, 664
	mov	w2, 65536
	mov	w1, 12
	ldr	x24, [x23, 1768]
	mov	x0, x20
	stp	wzr, w2, [x19, 72]
	bl	BsGet
	str	w0, [x24, 48]
	mov	w1, 12
	mov	x0, x20
	bl	BsGet
	str	w0, [x24, 52]
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	str	w0, [x24, 84]
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	str	w0, [x24, 88]
	mov	w1, 18
	mov	x0, x20
	bl	BsGet
	str	w0, [x24, 92]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cbnz	w0, .L515
	ldr	w0, [x19, 76]
	orr	w0, w0, 1
	str	w0, [x19, 76]
.L515:
	mov	w1, 10
	mov	x0, x20
	bl	BsGet
	str	w0, [x24, 96]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x24, 1]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, 1
	str	w0, [x24, 76]
	ldr	x22, .LC95
	beq	.L532
	add	x0, x22, 64
	add	x1, x23, 704
	add	x3, x0, 64
	.p2align 2
.L520:
	ldrb	w2, [x0], 1
	str	w2, [x1], 4
	cmp	x0, x3
	bne	.L520
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, 1
	str	w0, [x24, 80]
	beq	.L533
.L529:
	add	x0, x23, 960
	mov	w1, 16
	add	x23, x23, 1216
	.p2align 2
.L523:
	str	w1, [x0], 4
	cmp	x0, x23
	bne	.L523
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L532:
	mov	x26, x22
	add	x25, x22, 64
	.p2align 2
.L517:
	ldrb	w19, [x26], 1
	mov	w1, 8
	mov	x0, x20
	add	x19, x19, 20480
	add	x19, x19, 1200
	bl	BsGet
	cmp	x26, x25
	str	w0, [x21, x19, lsl 2]
	bne	.L517
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, 1
	str	w0, [x24, 80]
	bne	.L529
.L533:
	add	x23, x22, 64
	.p2align 2
.L522:
	ldrb	w19, [x22], 1
	mov	w1, 8
	mov	x0, x20
	add	x19, x19, 20480
	add	x19, x19, 1264
	bl	BsGet
	cmp	x22, x23
	str	w0, [x21, x19, lsl 2]
	bne	.L522
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.size	sequence_header, .-sequence_header
	.align	3
.LC95:
	.xword	.LANCHOR3
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	group_of_pictures_header
	.type	group_of_pictures_header, %function
group_of_pictures_header:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 86016
	add	x20, x19, 664
	bl	_mcount
	mov	w1, 1
	mov	x0, x20
	ldr	x21, [x19, 1768]
	bl	BsGet
	mov	w1, 5
	strb	w0, [x21, 2]
	mov	x0, x20
	bl	BsGet
	str	w0, [x21, 100]
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	str	w0, [x21, 104]
	mov	w1, 1
	mov	x0, x20
	bl	BsSkip
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	str	w0, [x21, 108]
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	str	w0, [x21, 112]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	str	w0, [x21, 116]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	str	w0, [x21, 120]
	add	x1, x19, 540
	add	x2, x19, 564
	ldr	x3, [x19, 552]
	ldr	x4, [x1]
	str	x4, [x19, 528]
	ldr	w4, [x19, 548]
	str	w4, [x19, 536]
	str	x3, [x1]
	ldr	w3, [x19, 560]
	str	w3, [x19, 548]
	ldr	x1, [x2]
	str	x1, [x19, 552]
	ldr	w1, [x19, 572]
	str	wzr, [x19, 564]
	str	wzr, [x19, 568]
	str	w1, [x19, 560]
	str	wzr, [x19, 572]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	group_of_pictures_header, .-group_of_pictures_header
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	sequence_extension
	.type	sequence_extension, %function
sequence_extension:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x22, x0
	mov	x0, x30
	add	x23, x22, 45056
	add	x22, x22, 86016
	add	x21, x23, 312
	bl	_mcount
	add	x21, x21, 40960
	add	x20, x22, 664
	ldr	x19, [x22, 1768]
	mov	w1, 8
	mov	x0, x20
	ldr	w2, [x21, 76]
	orr	w2, w2, 131072
	stp	wzr, w2, [x21, 72]
	bl	BsGet
	str	w0, [x19, 124]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 27]
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 28]
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	mov	w25, w0
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	mov	w26, w0
	mov	w1, 12
	mov	x0, x20
	bl	BsGet
	mov	w24, w0
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cbnz	w0, .L536
	ldr	w0, [x21, 76]
	orr	w0, w0, 2
	str	w0, [x21, 76]
.L535:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L536:
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	mov	w21, w0
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 29]
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 30]
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	and	w2, w0, 255
	ldrb	w1, [x19, 30]
	add	w2, w2, 1
	strb	w0, [x19, 31]
	add	w0, w1, 1
	ldr	x1, .LC98
	sdiv	w0, w0, w2
	add	x1, x1, 16
	ldr	w2, [x19, 88]
	ldr	w2, [x1, x2, lsl 2]
	ldr	w1, [x19, 124]
	mul	w0, w0, w2
	str	w0, [x19, 72]
	tbnz	x1, 7, .L559
	ubfx	x0, x1, 4, 3
	and	w1, w1, 15
	stp	w0, w1, [x19, 40]
.L539:
	ldp	w0, w1, [x19, 48]
	and	w1, w1, 4095
	and	w0, w0, 4095
	orr	w26, w1, w26, lsl 12
	orr	w25, w0, w25, lsl 12
	cmp	w26, wzr
	ccmp	w25, wzr, 4, ne
	stp	w25, w26, [x19, 48]
	bne	.L540
	add	x0, x23, 312
	add	x0, x0, 40960
	ldr	w1, [x0, 76]
	orr	w1, w1, 1
	str	w1, [x0, 76]
	ldr	w25, [x19, 48]
.L540:
	cmp	w25, 8192
	bhi	.L541
	ldr	w0, [x19, 52]
	cmp	w0, 8192
	bls	.L542
.L541:
	add	x23, x23, 312
	ldr	x0, .LC99
	add	x23, x23, 40960
	ldr	x4, [x0]
	ldr	w0, [x23, 76]
	orr	w0, w0, 1
	str	w0, [x23, 76]
	cbz	x4, .L542
	ldr	w0, [x22, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
.L542:
	ldp	w0, w3, [x19, 92]
	mov	w1, 400
	ldr	w2, [x19, 40]
	add	w24, w0, w24, lsl 18
	add	w21, w3, w21, lsl 10
	sub	w0, w2, #4
	stp	w24, w21, [x19, 92]
	mul	w24, w24, w1
	cmp	w0, 1
	str	w24, [x19, 68]
	bls	.L544
	ldr	x0, .LC99
	ldr	x4, [x0]
	cbz	x4, .L545
	ldr	w0, [x22, 2392]
	mov	x2, 0
	mov	w3, 0
	mov	w1, 102
	blr	x4
	ldr	w2, [x19, 40]
.L545:
	ldr	x1, .LC101
	mov	w0, 1
	bl	dprint_vfmw
.L544:
	ldr	w2, [x19, 44]
	sub	w0, w2, #4
	cmp	w0, 6
	bls	.L535
	ldr	x0, .LC99
	ldr	x4, [x0]
	cbz	x4, .L546
	ldr	w0, [x22, 2392]
	mov	x2, 0
	mov	w3, 0
	mov	w1, 102
	blr	x4
	ldr	w2, [x19, 44]
.L546:
	ldr	x1, .LC103
	mov	w0, 1
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L559:
	and	w1, w1, 15
	cmp	w1, 5
	bne	.L539
	mov	w1, 133
	mov	w0, 8
	stp	w1, w0, [x19, 40]
	b	.L539
	.size	sequence_extension, .-sequence_extension
	.align	3
.LC98:
	.xword	.LANCHOR1
	.align	3
.LC99:
	.xword	g_event_report
	.align	3
.LC101:
	.xword	.LC100
	.align	3
.LC103:
	.xword	.LC102
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	sequence_display_extension
	.type	sequence_display_extension, %function
sequence_display_extension:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 45056
	bl	_mcount
	add	x3, x20, 312
	add	x2, x19, 86016
	add	x3, x3, 40960
	add	x19, x2, 664
	mov	w1, 3
	mov	x0, x19
	ldr	x21, [x2, 1768]
	ldr	w4, [x3, 76]
	orr	w2, w4, 262144
	str	w2, [x3, 76]
	bl	BsGet
	str	w0, [x21, 128]
	mov	w1, 1
	mov	x0, x19
	bl	BsGet
	str	w0, [x21, 132]
	cbnz	w0, .L567
.L561:
	mov	w1, 14
	mov	x0, x19
	bl	BsGet
	str	w0, [x21, 148]
	mov	w1, 1
	mov	x0, x19
	bl	BsGet
	cbnz	w0, .L562
	add	x20, x20, 312
	add	x20, x20, 40960
	ldr	w0, [x20, 76]
	orr	w0, w0, 4
	str	w0, [x20, 76]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L562:
	mov	w1, 14
	mov	x0, x19
	bl	BsGet
	str	w0, [x21, 152]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L567:
	mov	w1, 8
	mov	x0, x19
	bl	BsGet
	str	w0, [x21, 136]
	mov	w1, 8
	mov	x0, x19
	bl	BsGet
	str	w0, [x21, 140]
	mov	w1, 8
	mov	x0, x19
	bl	BsGet
	str	w0, [x21, 144]
	b	.L561
	.size	sequence_display_extension, .-sequence_display_extension
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	quant_matrix_extension
	.type	quant_matrix_extension, %function
quant_matrix_extension:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	bl	_mcount
	add	x2, x21, 86016
	mov	w1, 1
	add	x20, x2, 664
	mov	x0, x20
	ldr	x24, [x2, 1768]
	bl	BsGet
	cmp	w0, 1
	str	w0, [x24, 76]
	beq	.L569
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, 1
	str	w0, [x24, 80]
	beq	.L587
.L571:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cbz	w0, .L575
	mov	w19, 64
	.p2align 2
.L579:
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	subs	w19, w19, #1
	bne	.L579
.L575:
	mov	w1, 1
	mov	x0, x20
	mov	w19, 64
	bl	BsGet
	cbz	w0, .L568
	.p2align 2
.L581:
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	subs	w19, w19, #1
	bne	.L581
.L568:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L569:
	ldr	x22, .LC105
	add	x23, x22, 64
	.p2align 2
.L572:
	ldrb	w19, [x22], 1
	mov	w1, 8
	mov	x0, x20
	add	x19, x19, 20480
	add	x19, x19, 1200
	bl	BsGet
	cmp	x22, x23
	str	w0, [x21, x19, lsl 2]
	bne	.L572
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, 1
	str	w0, [x24, 80]
	bne	.L571
.L587:
	ldr	x22, .LC105
	add	x23, x22, 64
	.p2align 2
.L576:
	ldrb	w19, [x22], 1
	mov	w1, 8
	mov	x0, x20
	add	x19, x19, 20480
	add	x19, x19, 1264
	bl	BsGet
	cmp	x23, x22
	str	w0, [x21, x19, lsl 2]
	bne	.L576
	b	.L571
	.size	quant_matrix_extension, .-quant_matrix_extension
	.align	3
.LC105:
	.xword	.LANCHOR3
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	picture_display_extension
	.type	picture_display_extension, %function
picture_display_extension:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	add	x23, x19, 45056
	add	x19, x19, 86016
	bl	_mcount
	add	x20, x19, 664
	add	x0, x23, 312
	add	x0, x0, 40960
	ldr	x19, [x19, 1768]
	ldr	w1, [x0, 76]
	orr	w1, w1, 2097152
	str	w1, [x0, 76]
	ldrb	w0, [x19, 27]
	cbz	w0, .L589
	ldrb	w0, [x19, 16]
	mov	w22, 1
	cbnz	w0, .L602
.L590:
	add	x19, x19, 164
	mov	w21, 0
	b	.L594
	.p2align 3
.L603:
	mov	w1, 16
	mov	x0, x20
	add	x19, x19, 4
	bl	BsGet
	mov	w1, 1
	str	w0, [x19, 8]
	mov	x0, x20
	bl	BsGet
	cbz	w0, .L601
	cmp	w21, w22
	bge	.L588
.L594:
	mov	w1, 16
	mov	x0, x20
	add	w21, w21, 1
	bl	BsGet
	str	w0, [x19]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cbnz	w0, .L603
.L601:
	add	x0, x23, 312
	add	x0, x0, 40960
	ldr	w1, [x0, 76]
	orr	w1, w1, 32
	str	w1, [x0, 76]
.L588:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L589:
	ldrb	w0, [x19, 9]
	mov	w22, 1
	cmp	w0, 3
	bne	.L590
	ldrb	w1, [x19, 16]
	mov	w22, 2
	cmp	w1, wzr
	csel	w22, w22, w0, eq
	b	.L590
	.p2align 3
.L602:
	ldrb	w1, [x19, 10]
	mov	w0, 2
	mov	w22, 3
	cmp	w1, wzr
	csel	w22, w0, w22, eq
	b	.L590
	.size	picture_display_extension, .-picture_display_extension
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	picture_coding_extension
	.type	picture_coding_extension, %function
picture_coding_extension:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x19, x0
	mov	x0, x30
	add	x22, x19, 45056
	add	x19, x19, 86016
	bl	_mcount
	add	x20, x19, 664
	add	x1, x22, 312
	add	x1, x1, 40960
	ldr	x21, [x19, 1768]
	mov	x23, x1
	mov	x25, x21
	add	x24, x21, 4
	ldr	w0, [x1, 76]
	orr	w0, w0, 1048576
	str	w0, [x1, 76]
.L605:
	mov	x19, 0
.L607:
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	sub	w2, w0, #10
	add	x1, x25, x19
	add	x19, x19, 1
	uxtb	w2, w2
	cmp	w2, 4
	strb	w0, [x1, 4]
	ccmp	w0, wzr, 4, hi
	bne	.L606
	ldr	w0, [x23, 76]
	orr	w0, w0, 16
	str	w0, [x23, 76]
.L606:
	cmp	x19, 2
	bne	.L607
	add	x25, x25, 2
	cmp	x25, x24
	bne	.L605
	mov	w1, w19
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 8]
	mov	w1, w19
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x21, 9]
	cbnz	w0, .L609
	add	x22, x22, 312
	add	x22, x22, 40960
	ldr	w0, [x22, 76]
	orr	w0, w0, 16
	str	w0, [x22, 76]
.L604:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L609:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 10]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 11]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 12]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 13]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 14]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 15]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 16]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 17]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 18]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x21, 3]
	cbz	w0, .L604
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 19]
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 20]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 21]
	mov	w1, 7
	mov	x0, x20
	bl	BsGet
	str	w0, [x21, 32]
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	str	w0, [x21, 36]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.size	picture_coding_extension, .-picture_coding_extension
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	extra_bit_information
	.type	extra_bit_information, %function
extra_bit_information:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 86016
	mov	w20, 0
	bl	_mcount
	add	x19, x19, 664
	b	.L619
	.p2align 3
.L621:
	mov	w1, 8
	mov	x0, x19
	add	w20, w20, 1
	bl	BsSkip
	mov	x0, x19
	bl	BsResidBits
	cmp	w0, 8
	ble	.L620
.L619:
	mov	w1, 1
	mov	x0, x19
	bl	BsGet
	cbnz	w0, .L621
.L620:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	extra_bit_information, .-extra_bit_information
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	picture_header
	.type	picture_header, %function
picture_header:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	bl	_mcount
	mov	x3, 20792
	add	x4, x21, 86016
	movk	x3, 0x1, lsl 16
	add	x20, x4, 664
	add	x3, x21, x3
	mov	w1, 10
	ldr	x19, [x4, 1768]
	mov	x0, x20
	ldr	w2, [x3, 76]
	and	w2, w2, -3670073
	orr	w2, w2, 524288
	str	w2, [x3, 76]
	bl	BsGet
	str	w0, [x19, 156]
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 22]
	mov	w1, 16
	mov	x0, x20
	bl	BsGet
	ldrb	w22, [x19, 22]
	str	w0, [x19, 160]
	sub	w0, w22, #2
	uxtb	w0, w0
	cmp	w0, 1
	bls	.L625
.L623:
	cmp	w22, 3
	beq	.L626
	mov	x0, x21
	bl	extra_bit_information
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L626:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 25]
	mov	w1, w22
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 26]
	mov	x0, x21
	bl	extra_bit_information
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L625:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 23]
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	ldrb	w22, [x19, 22]
	strb	w0, [x19, 24]
	b	.L623
	.size	picture_header, .-picture_header
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	copyright_extension
	.type	copyright_extension, %function
copyright_extension:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 86016
	mov	w1, 32
	add	x0, x0, 664
	bl	BsSkip
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	copyright_extension, .-copyright_extension
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	picture_temporal_scalable_extension
	.type	picture_temporal_scalable_extension, %function
picture_temporal_scalable_extension:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	copyright_extension
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	picture_temporal_scalable_extension, .-picture_temporal_scalable_extension
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	picture_spatial_scalable_extension
	.type	picture_spatial_scalable_extension, %function
picture_spatial_scalable_extension:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	copyright_extension
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	picture_spatial_scalable_extension, .-picture_spatial_scalable_extension
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	sequence_scalable_extension
	.type	sequence_scalable_extension, %function
sequence_scalable_extension:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	copyright_extension
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	sequence_scalable_extension, .-sequence_scalable_extension
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	extension
	.type	extension, %function
extension:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x2, x19, 86016
	mov	w1, 4
	add	x20, x2, 664
	mov	x0, x20
	ldr	x2, [x2, 1768]
	stp	wzr, wzr, [x2, 128]
	stp	wzr, wzr, [x2, 136]
	stp	wzr, wzr, [x2, 144]
	str	wzr, [x2, 152]
	bl	BsGet
	sub	w0, w0, #1
	cmp	w0, 9
	bhi	.L632
	ldr	x1, .LC107
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx634
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx634:
	.section	.rodata
	.align	0
	.align	2
.L634:
	.byte	(.L633 - .Lrtx634) / 4
	.byte	(.L635 - .Lrtx634) / 4
	.byte	(.L636 - .Lrtx634) / 4
	.byte	(.L637 - .Lrtx634) / 4
	.byte	(.L637 - .Lrtx634) / 4
	.byte	(.L632 - .Lrtx634) / 4
	.byte	(.L639 - .Lrtx634) / 4
	.byte	(.L640 - .Lrtx634) / 4
	.byte	(.L637 - .Lrtx634) / 4
	.byte	(.L637 - .Lrtx634) / 4
	.text
	.p2align 3
.L637:
	mov	w1, 32
	mov	x0, x20
	bl	BsSkip
.L632:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L636:
	mov	x0, x19
	bl	quant_matrix_extension
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L640:
	mov	x0, x19
	bl	picture_coding_extension
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L639:
	mov	x0, x19
	bl	picture_display_extension
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L633:
	mov	x0, x19
	bl	sequence_extension
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L635:
	mov	x0, x19
	bl	sequence_display_extension
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	extension, .-extension
	.align	3
.LC107:
	.xword	.L634
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG2DEC_Init
	.type	MPEG2DEC_Init, %function
MPEG2DEC_Init:
	sub	sp, sp, #896
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x1
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 86016
	bl	_mcount
	ldr	x22, .LC108
	ldr	w0, [x21, 648]
	cmp	w0, 1
	beq	.L650
.L644:
	ldr	x3, [x22, 96]
	mov	x2, 22904
	movk	x2, 0x1, lsl 16
	mov	w1, 0
	mov	x0, x20
	ldr	w24, [x19, 2384]
	mov	w23, 1
	blr	x3
	str	w24, [x19, 2384]
	add	x0, x19, 1784
	bl	ResetVoQueue
	str	x21, [x20]
	add	x0, x20, 45056
	str	wzr, [x19, 2404]
	add	x0, x0, 312
	add	x3, x19, 1392
	add	x1, x0, 40960
	str	x3, [x19, 1776]
	add	x2, x19, 1580
	str	x2, [x19, 1768]
	mov	w2, 3
	str	w23, [x1, 88]
	strb	w2, [x0, 1]
	mov	x0, x20
	str	wzr, [x1, 152]
	ldr	w2, [x21, 8]
	str	w2, [x1, 64]
	str	wzr, [x1, 144]
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x19, 2392]
	beq	.L651
	ldr	w20, [x21, 648]
	cmp	w20, 1
	beq	.L652
	mov	w20, w23
.L646:
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 896
	ret
	.p2align 3
.L652:
	ldr	x3, [x22, 104]
	add	x1, x29, 448
	mov	x2, 256
	add	x0, x19, 704
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x29, 704
	mov	x2, 256
	add	x0, x19, 960
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x29, 64
	ldr	x0, [x19, 1768]
	mov	x2, 188
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x29, 256
	ldr	x0, [x19, 1776]
	mov	x2, 188
	blr	x3
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 896
	ret
	.p2align 3
.L650:
	ldr	x3, [x22, 104]
	add	x1, x19, 704
	mov	x2, 256
	add	x0, x29, 448
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x19, 960
	mov	x2, 256
	add	x0, x29, 704
	blr	x3
	ldr	x1, [x19, 1768]
	mov	x2, 188
	ldr	x3, [x22, 104]
	add	x0, x29, 64
	blr	x3
	ldr	x1, [x19, 1776]
	mov	x2, 188
	ldr	x3, [x22, 104]
	add	x0, x29, 256
	blr	x3
	b	.L644
.L651:
	ldr	x1, .LC110
	mov	w0, 0
	mov	w20, -20
	bl	dprint_vfmw
	b	.L646
	.size	MPEG2DEC_Init, .-MPEG2DEC_Init
	.align	3
.LC108:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC110:
	.xword	.LC109
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG2DEC_Destroy
	.type	MPEG2DEC_Destroy, %function
MPEG2DEC_Destroy:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, 20792
	ldr	x1, .LC112
	movk	x0, 0x1, lsl 16
	add	x2, x19, x0
	mov	w0, 2
	ldr	w2, [x2, 84]
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, 1
	ldp	x29, x30, [sp], 32
	ret
	.size	MPEG2DEC_Destroy, .-MPEG2DEC_Destroy
	.align	3
.LC112:
	.xword	.LC111
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG2DEC_RecycleImage
	.type	MPEG2DEC_RecycleImage, %function
MPEG2DEC_RecycleImage:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cbz	x20, .L659
	add	x20, x20, 86016
	mov	w1, w19
	mov	w2, 0
	ldr	w0, [x20, 2392]
	bl	FSP_SetDisplay
	ldr	w0, [x20, 2392]
	mov	w1, w19
	bl	FSP_GetFsImagePtr
	add	x19, x0, 560
	add	x21, x0, 592
	cbz	x0, .L658
.L660:
	ldr	x1, [x19]
	ldr	w0, [x20, 2392]
	bl	FreeUsdByDec
	str	xzr, [x19], 8
	cmp	x19, x21
	bne	.L660
.L658:
	mov	w0, 1
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L659:
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	MPEG2DEC_RecycleImage, .-MPEG2DEC_RecycleImage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG2DEC_GetRemainImg
	.type	MPEG2DEC_GetRemainImg, %function
MPEG2DEC_GetRemainImg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 86016
	bl	_mcount
	ldr	x0, [x19]
	ldr	w0, [x0, 12]
	cmp	w0, 1
	beq	.L663
	add	x23, x19, 45056
	add	x20, x19, 86016
	add	x21, x23, 312
	add	x21, x21, 40960
	ldr	w0, [x20, 2392]
	ldr	w1, [x21, 176]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L663
	ldr	w0, [x20, 2392]
	ldr	w1, [x21, 176]
	bl	FSP_GetDisplay
	cmp	w0, 2
	beq	.L682
.L663:
	add	x21, x20, 1784
.L669:
	mov	x0, x21
	bl	GetVoLastImageID
.L677:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L682:
	mov	x0, x19
	bl	MP2_SetImgFormat
	add	x1, x20, 408
	add	x0, x22, 560
	add	x3, x22, 592
.L665:
	ldr	x2, [x1]
	cbz	x2, .L664
	str	x2, [x0]
.L664:
	add	x0, x0, 8
	str	xzr, [x1], 8
	cmp	x0, x3
	bne	.L665
	add	x23, x23, 312
	mov	w0, 1
	add	x23, x23, 40960
	mov	w2, w0
	add	x21, x20, 1784
	str	wzr, [x23, 128]
	str	w0, [x22, 188]
	ldr	w1, [x22, 156]
	ldr	w0, [x20, 2392]
	bl	FSP_SetDisplay
	ldr	w0, [x20, 2392]
	mov	x4, x22
	mov	x3, x21
	mov	x2, x19
	mov	w1, 3
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L666
	ldr	w1, [x22, 156]
	mov	x0, x19
	bl	MPEG2DEC_RecycleImage
	b	.L669
	.p2align 3
.L666:
	ldr	w0, [x22, 160]
	cmp	w0, 100
	bhi	.L663
	ldr	w0, [x22, 112]
	cmp	w0, 31
	bls	.L663
	ldr	w1, [x22, 116]
	mov	w0, 0
	cmp	w1, 31
	bhi	.L677
	b	.L669
	.size	MPEG2DEC_GetRemainImg, .-MPEG2DEC_GetRemainImg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG2DEC_GetImageBuffer
	.type	MPEG2DEC_GetImageBuffer, %function
MPEG2DEC_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 45056
	add	x0, x0, 312
	ldrb	w1, [x0, 1]
	cmp	w1, 3
	beq	.L684
	add	x0, x0, 40960
	ldr	w0, [x0, 88]
	cbz	w0, .L686
.L684:
	add	x19, x19, 86016
	ldr	w0, [x19, 2392]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	beq	.L686
	ldr	w0, [x19, 2392]
	mov	w20, 0
	bl	FSP_IsNewFsAvalible
	cmn	w0, #1
	bne	.L685
	ldr	w0, [x19, 2392]
	add	x1, x19, 1784
	bl	FSP_ClearNotInVoQueue
	b	.L685
	.p2align 3
.L686:
	mov	w20, 1
.L685:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MPEG2DEC_GetImageBuffer, .-MPEG2DEC_GetImageBuffer
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_ClearCurPacket
	.type	MP2_ClearCurPacket, %function
MP2_ClearCurPacket:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 20]
	mov	w0, w20
	bl	SM_ReleaseStreamSeg
	ldrsb	w0, [x19]
	cmp	w0, 1
	beq	.L695
	ldr	w1, [x19, 24]
	mov	w0, w20
	bl	SM_ReleaseStreamSeg
.L695:
	ldr	x3, .LC113
	mov	x2, 48
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MP2_ClearCurPacket, .-MP2_ClearCurPacket
	.align	3
.LC113:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_ReceivePacket
	.type	MP2_ReceivePacket, %function
MP2_ReceivePacket:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	mov	w22, w2
	bl	_mcount
	cbz	x20, .L705
	add	x1, x19, 40960
	str	wzr, [x1, 8]
	ldr	w0, [x20, 20]
	str	w0, [x1, 68]
	ldr	w0, [x20, 16]
	str	w0, [x1, 20]
	ldr	x0, [x20]
	str	x0, [x1, 48]
	ldr	w0, [x20, 20]
	str	w0, [x1, 28]
	ldrb	w0, [x20, 10]
	strb	w0, [x1, 16]
	ldr	w0, [x20, 24]
	str	w0, [x1, 36]
	ldrb	w21, [x20, 10]
	cmp	w21, 1
	beq	.L702
	ldr	w0, [x20, 104]
	mov	w21, 1
	str	w0, [x1, 24]
	ldr	x0, [x20, 88]
	str	x0, [x1, 56]
	ldr	w0, [x20, 108]
	str	w0, [x1, 32]
	ldrb	w0, [x20, 98]
	strb	w0, [x1, 17]
	ldr	w0, [x20, 112]
	str	w0, [x1, 40]
	ldrb	w0, [x20, 98]
	cmp	w0, w21
	beq	.L701
	add	x1, x1, 16
	mov	w0, w22
	bl	MP2_ClearCurPacket
	ldrb	w2, [x20, 98]
	mov	w0, w21
	ldr	x1, .LC117
	mov	w21, 0
	bl	dprint_vfmw
.L701:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L702:
	strb	wzr, [x1, 17]
	mov	w0, w21
	str	wzr, [x1, 24]
	str	wzr, [x1, 32]
	str	wzr, [x1, 40]
	str	xzr, [x1, 56]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L705:
	add	x19, x19, 40960
	ldr	x1, .LC115
	mov	w0, 1
	mov	w21, 0
	ldr	w2, [x19, 84]
	bl	dprint_vfmw
	b	.L701
	.size	MP2_ReceivePacket, .-MP2_ReceivePacket
	.align	3
.LC115:
	.xword	.LC114
	.align	3
.LC117:
	.xword	.LC116
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_ClearCurPic
	.type	MP2_ClearCurPic, %function
MP2_ClearCurPic:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	add	x22, x21, 16
	mov	w20, 0
	bl	_mcount
	add	x21, x21, 86016
	add	x0, x22, 45056
	mov	x23, x0
	ldr	w0, [x0, 272]
	cbnz	w0, .L715
	b	.L710
	.p2align 3
.L708:
	ldr	w0, [x23, 272]
	cmp	w0, w20
	bls	.L710
.L715:
	sxtw	x19, w20
	ldr	w0, [x21, 2392]
	lsl	x1, x19, 2
	add	w20, w20, 1
	sub	x1, x1, x19
	lsl	x1, x1, 2
	sub	x19, x1, x19
	add	x19, x22, x19, lsl 2
	ldr	w1, [x19, 296]
	bl	SM_ReleaseStreamSeg
	ldr	w0, [x19, 276]
	cbz	w0, .L708
	ldr	w0, [x19, 284]
	cbz	w0, .L708
	ldr	w1, [x19, 300]
	ldr	w0, [x21, 2392]
	bl	SM_ReleaseStreamSeg
	ldr	w0, [x23, 272]
	cmp	w0, w20
	bhi	.L715
.L710:
	add	x22, x22, 45056
	str	wzr, [x22, 272]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	MP2_ClearCurPic, .-MP2_ClearCurPic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG2DEC_VDMPostProc
	.type	MPEG2DEC_VDMPostProc, %function
MPEG2DEC_VDMPostProc:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	add	x21, x20, 45056
	add	x19, x20, 86016
	add	x22, x21, 312
	mov	w25, w1
	add	x22, x22, 40960
	bl	_mcount
	ldrb	w0, [x20, 160]
	mov	w2, w25
	ldr	x1, .LC119
	add	x23, x20, 16
	ldr	x24, [x19, 1768]
	strb	w0, [x22, 276]
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x0, [x19, 1768]
	ldr	x1, [x20]
	ldr	w2, [x0, 40]
	str	w2, [x1, 608]
	ldr	x1, [x20]
	ldr	w0, [x0, 44]
	str	w0, [x1, 612]
	ldr	w1, [x19, 2400]
	ldr	w0, [x19, 2396]
	ldr	x2, [x20]
	cmp	w0, w1
	sub	w0, w0, w1
	csel	w1, w0, wzr, ge
	mov	x0, x20
	str	w1, [x2, 616]
	bl	MP2_ClearCurPic
	ldr	x3, .LC120
	mov	x2, 45348
	mov	w1, 0
	mov	x0, x23
	ldr	x3, [x3, 96]
	blr	x3
	ldr	w1, [x22, 180]
	ldr	w0, [x19, 2392]
	bl	FSP_GetFsImagePtr
	mov	x23, x0
	ldr	w1, [x22, 176]
	ldr	w0, [x19, 2392]
	bl	FSP_GetFsImagePtr
	mov	x26, x0
	ldr	w1, [x22, 172]
	ldr	w0, [x19, 2392]
	bl	FSP_GetFsImagePtr
	cbz	x23, .L785
	ldrb	w0, [x24, 22]
	strb	w0, [x21, 312]
	str	w25, [x23, 160]
	tbnz	w25, #31, .L724
	cmp	w25, 100
	mov	w1, 100
	csel	w1, w25, w1, le
	str	w1, [x23, 160]
	ldrb	w0, [x24, 22]
	cmp	w0, 3
	beq	.L725
	ldr	x0, [x20]
	ldr	w0, [x0]
	cmp	w1, w0
	bhi	.L786
.L725:
	ldr	x1, .LC124
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x20
	bl	MP2_SetImgFormat
	ldr	x0, [x20]
	ldr	w0, [x0, 636]
	cmp	w0, 1
	beq	.L787
	stp	wzr, wzr, [x23, 80]
.L731:
	ldrb	w0, [x24, 9]
	cmp	w0, 3
	beq	.L732
	add	x1, x21, 312
	mov	w0, 1
	add	x1, x1, 40960
	ldr	w1, [x1, 88]
	cmp	w1, w0
	beq	.L732
.L771:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L786:
	ldr	w0, [x19, 2392]
	ldr	w1, [x22, 176]
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L727
	ldr	w0, [x19, 2392]
	ldr	x1, [x20, 560]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2392]
	ldr	x1, [x20, 568]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2392]
	ldr	x1, [x20, 576]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2392]
	ldr	x1, [x20, 584]
	bl	FreeUsdByDec
	add	x0, x20, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w0, [x19, 2392]
	ldr	w1, [x22, 176]
	bl	FSP_ClearLogicFs
	ldr	w0, [x19, 2392]
	mov	w2, 1
	ldr	w1, [x22, 172]
	bl	FSP_ClearLogicFs
	stp	wzr, wzr, [x22, 172]
	ldr	w0, [x19, 2392]
	bl	FSP_GetFspType
	cbz	w0, .L727
	ldr	w0, [x19, 2392]
	bl	FSP_GetNullLogicFs
	stp	w0, w0, [x22, 172]
.L727:
	add	x21, x21, 312
	mov	w0, 1
	add	x21, x21, 40960
	str	wzr, [x21, 136]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L732:
	add	x0, x23, 560
	add	x1, x19, 408
	add	x23, x23, 592
.L734:
	ldr	x2, [x1]
	cbz	x2, .L733
	str	x2, [x0]
.L733:
	add	x0, x0, 8
	str	xzr, [x1], 8
	cmp	x0, x23
	bne	.L734
	add	x0, x21, 312
	add	x0, x0, 40960
	str	wzr, [x0, 128]
	ldr	x1, [x20]
	ldr	w1, [x1, 12]
	cmp	w1, 1
	beq	.L735
	ldrb	w1, [x24, 22]
	cmp	w1, 3
	beq	.L735
	ldr	w1, [x0, 172]
	str	w1, [x0, 184]
.L736:
	cmn	w1, #1
	beq	.L738
	ldr	w0, [x19, 2392]
	bl	FSP_GetDisplay
	cmp	w0, 1
	beq	.L742
	add	x1, x21, 312
	ldr	w0, [x19, 2392]
	add	x1, x1, 40960
	ldr	w1, [x1, 184]
	bl	FSP_GetDisplay
	cmp	w0, 2
	beq	.L742
.L738:
	ldrb	w0, [x24, 27]
	cmp	w0, 1
	beq	.L788
	ldrb	w0, [x24, 18]
	cmp	w0, 1
	beq	.L789
	add	x1, x21, 312
	add	x1, x1, 40960
	ldr	w0, [x1, 260]
	add	w0, w0, 1
	str	w0, [x1, 260]
	lsl	w0, w0, 1
	str	w0, [x1, 256]
.L750:
	ldr	w1, [x19, 564]
	add	x21, x21, 312
	mov	w0, 1
	add	x21, x21, 40960
	add	w1, w1, w0
	str	w1, [x19, 564]
	ldr	w1, [x21, 84]
	add	w1, w1, w0
	str	w1, [x21, 84]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L724:
	str	wzr, [x23, 160]
	b	.L725
	.p2align 3
.L787:
	ldr	x1, .LC125
	ldr	x1, [x1]
	ldr	w2, [x1, 4180]
	ldr	w1, [x1, 4184]
	cmp	w1, wzr
	csinc	w1, w1, wzr, ne
	udiv	w1, w2, w1
	stp	w0, w1, [x23, 80]
	b	.L731
	.p2align 3
.L789:
	add	x0, x21, 312
	add	x0, x0, 40960
	ldr	w1, [x0, 260]
	ldr	w2, [x0, 256]
	add	w1, w1, 1
	str	w1, [x0, 260]
	ldrb	w1, [x24, 16]
	add	w1, w1, 2
	add	w1, w1, w2
	str	w1, [x0, 256]
	b	.L750
	.p2align 3
.L742:
	add	x1, x21, 312
	ldr	w0, [x19, 2392]
	add	x1, x1, 40960
	ldr	w1, [x1, 184]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L790
	ldr	w2, [x0, 160]
	cbz	w2, .L744
	ldr	x0, [x20]
	ldr	w3, [x0, 4]
	cmp	w2, w3
	bhi	.L745
.L744:
	add	x23, x21, 312
	ldr	w0, [x19, 2392]
	add	x23, x23, 40960
	ldr	w25, [x23, 184]
	mov	w1, w25
	bl	FSP_GetRef
	mov	w3, w0
	ldr	x1, .LC128
	mov	w2, w25
	mov	w0, 12
	bl	dprint_vfmw
	ldr	w0, [x19, 2392]
	mov	w2, 1
	ldr	w1, [x23, 184]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 2392]
	mov	x4, x22
	add	x3, x19, 1784
	mov	x2, x20
	mov	w1, 3
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L746
	add	x20, x22, 560
	add	x22, x22, 592
.L747:
	ldr	x1, [x20]
	ldr	w0, [x19, 2392]
	bl	FreeUsdByDec
	str	xzr, [x20], 8
	cmp	x20, x22
	bne	.L747
	add	x1, x21, 312
	ldr	w0, [x19, 2392]
	add	x1, x1, 40960
	mov	w2, 0
	ldr	w1, [x1, 184]
	bl	FSP_SetDisplay
	mov	w0, 0
	b	.L771
	.p2align 3
.L735:
	add	x0, x21, 312
	add	x0, x0, 40960
	ldr	w1, [x0, 180]
	str	w1, [x0, 184]
	b	.L736
	.p2align 3
.L788:
	add	x0, x21, 312
	ldrb	w2, [x24, 16]
	add	x0, x0, 40960
	ldrb	w1, [x24, 10]
	add	w1, w2, w1
	ldr	w2, [x0, 260]
	add	w1, w1, 1
	add	w1, w1, w2
	str	w1, [x0, 260]
	lsl	w1, w1, 1
	str	w1, [x0, 256]
	b	.L750
	.p2align 3
.L745:
	ldr	x1, .LC130
	mov	w0, 1
	add	x20, x22, 560
	add	x22, x22, 592
	bl	dprint_vfmw
.L748:
	ldr	x1, [x20]
	ldr	w0, [x19, 2392]
	bl	FreeUsdByDec
	str	xzr, [x20], 8
	cmp	x20, x22
	bne	.L748
	add	x1, x21, 312
	ldr	w0, [x19, 2392]
	add	x1, x1, 40960
	mov	w2, 0
	ldr	w1, [x1, 184]
	bl	FSP_SetDisplay
	b	.L738
	.p2align 3
.L785:
	ldr	x1, .LC122
	mov	x4, x0
	mov	x3, x26
	mov	x2, 0
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L771
	.p2align 3
.L746:
	ldr	w0, [x19, 2400]
	add	w0, w0, 1
	str	w0, [x19, 2400]
	b	.L738
.L790:
	ldr	x1, .LC126
	mov	w2, 3552
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L771
	.size	MPEG2DEC_VDMPostProc, .-MPEG2DEC_VDMPostProc
	.align	3
.LC119:
	.xword	.LC118
	.align	3
.LC120:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC122:
	.xword	.LC121
	.align	3
.LC124:
	.xword	.LC123
	.align	3
.LC125:
	.xword	g_HwMem
	.align	3
.LC126:
	.xword	.LC53
	.align	3
.LC128:
	.xword	.LC127
	.align	3
.LC130:
	.xword	.LC129
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_u_v
	.type	MP2_u_v, %function
MP2_u_v:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x22, x2
	mov	w21, w1
	bl	_mcount
	mov	w1, w21
	mov	x0, x20
	bl	BsShow
	mov	w19, w0
	mov	w1, w21
	mov	x0, x20
	bl	BsSkip
	ldr	x1, .LC132
	mov	w3, w19
	mov	x2, x22
	mov	w0, 2
	bl	dprint_vfmw
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MP2_u_v, .-MP2_u_v
	.align	3
.LC132:
	.xword	.LC131
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_DecSliceHeader
	.type	MP2_DecSliceHeader, %function
MP2_DecSliceHeader:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x20, 4]
	mov	w1, 1
	str	wzr, [x20, 24]
	mov	w4, 0
	cmp	w0, 175
	str	w1, [x20, 16]
	ldr	x19, [x20, 56]
	bls	.L793
	ldr	w0, [x20]
	cbz	w0, .L804
.L793:
	ldr	w3, [x20, 20]
	mov	w1, 5
	ldr	w5, [x20, 8]
	mov	x0, x19
	sub	w3, w3, #1
	ldr	x2, .LC136
	add	w4, w3, w4
	mul	w3, w4, w5
	cmp	w3, wzr
	cset	w4, ne
	sub	w3, w3, w4
	str	w3, [x20, 28]
	bl	MP2_u_v
	str	wzr, [x20, 32]
	str	w0, [x20, 44]
	mov	w1, 1
	mov	x0, x19
	bl	BsShow
	cbnz	w0, .L795
	ldr	x20, .LC138
.L797:
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	MP2_u_v
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L795:
	ldr	x2, .LC140
	mov	w1, 1
	mov	x0, x19
	bl	MP2_u_v
	ldr	x2, .LC142
	mov	w1, 1
	mov	x0, x19
	bl	MP2_u_v
	str	w0, [x20, 32]
	ldr	x2, .LC144
	mov	w1, 7
	mov	x0, x19
	bl	MP2_u_v
	ldr	x20, .LC138
	ldr	x21, .LC146
	b	.L796
	.p2align 3
.L805:
	bl	BsShow
	cbz	w0, .L797
	mov	w1, 1
	mov	x2, x20
	mov	x0, x19
	bl	MP2_u_v
	mov	x2, x21
	mov	w1, 8
	mov	x0, x19
	bl	MP2_u_v
.L796:
	mov	x0, x19
	bl	BsResidBits
	cmp	w0, wzr
	mov	w1, 1
	mov	x0, x19
	bgt	.L805
	b	.L797
	.p2align 3
.L804:
	ldr	x2, .LC134
	mov	w1, 3
	mov	x0, x19
	bl	MP2_u_v
	lsl	w4, w0, 7
	str	w0, [x20, 24]
	b	.L793
	.size	MP2_DecSliceHeader, .-MP2_DecSliceHeader
	.align	3
.LC134:
	.xword	.LC133
	.align	3
.LC136:
	.xword	.LC135
	.align	3
.LC138:
	.xword	.LC137
	.align	3
.LC140:
	.xword	.LC139
	.align	3
.LC142:
	.xword	.LC141
	.align	3
.LC144:
	.xword	.LC143
	.align	3
.LC146:
	.xword	.LC145
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_ExitMb
	.type	MP2_ExitMb, %function
MP2_ExitMb:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	str	wzr, [x19, 16]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MP2_ExitMb, .-MP2_ExitMb
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_WriteSliceInfo
	.type	MP2_WriteSliceInfo, %function
MP2_WriteSliceInfo:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x21, x20, 16
	bl	_mcount
	add	x2, x21, 45056
	add	x0, x20, 86016
	ldr	w2, [x2, 272]
	ldr	w4, [x0, 632]
	cmp	w2, 1023
	uxtw	x3, w2
	lsl	x19, x3, 2
	sub	x19, x19, x3
	lsl	x19, x19, 2
	sub	x19, x19, x3
	add	x19, x21, x19, lsl 2
	str	w4, [x19, 308]
	ldr	w3, [x0, 644]
	str	w3, [x19, 312]
	ldr	w1, [x0, 640]
	str	w1, [x19, 304]
	bhi	.L813
	mov	x1, 20792
	add	x0, x0, 664
	movk	x1, 0x1, lsl 16
	add	x20, x20, x1
	ldr	w1, [x0, 32]
	ldr	w2, [x20, 20]
	and	w3, w1, 7
	str	w3, [x19, 288]
	add	w1, w2, w1, lsr 3
	str	w1, [x19, 272]
	ldr	w1, [x20, 36]
	str	w1, [x19, 296]
	bl	BsResidBits
	str	w0, [x19, 280]
	ldrsh	w0, [x20, 16]
	cmp	w0, 1
	beq	.L814
	cmp	w0, 256
	bne	.L812
	ldr	w0, [x20, 24]
	str	w0, [x19, 276]
	str	wzr, [x19, 292]
	ldr	w0, [x20, 40]
	str	w0, [x19, 300]
	ldr	w0, [x20, 32]
	lsl	w0, w0, 3
	str	w0, [x19, 284]
.L811:
	add	x21, x21, 45056
	mov	w0, 1
	ldr	w1, [x21, 272]
	add	w1, w1, w0
	str	w1, [x21, 272]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L813:
	ldr	x1, .LC148
	mov	w3, 1024
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x20
	bl	MP2_ClearCurPic
	mov	x0, 20792
	mov	w1, -5767169
	movk	x0, 0x1, lsl 16
	add	x20, x20, x0
	mov	w0, 0
	ldr	w2, [x20, 76]
	str	wzr, [x20, 152]
	and	w1, w2, w1
	str	w1, [x20, 76]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L814:
	str	wzr, [x19, 276]
	str	wzr, [x19, 284]
	str	wzr, [x19, 292]
	str	wzr, [x19, 300]
	b	.L811
	.p2align 3
.L812:
	ldrsb	w3, [x20, 17]
	mov	w0, 1
	ldrsb	w2, [x20, 16]
	ldr	x1, .LC150
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	MP2_WriteSliceInfo, .-MP2_WriteSliceInfo
	.align	3
.LC148:
	.xword	.LC147
	.align	3
.LC150:
	.xword	.LC149
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_MbAddrInc
	.type	MP2_MbAddrInc, %function
MP2_MbAddrInc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	bl	_mcount
	mov	w1, 4
	mov	x0, x21
	bl	BsShow
	ldr	x20, .LC151
	ubfiz	x0, x0, 3, 32
	add	x1, x20, 80
	add	x2, x1, x0
	ldr	w19, [x1, x0]
	ldr	w1, [x2, 4]
	cmp	w19, 6
	bls	.L819
	cmp	w19, 9
	beq	.L820
	mov	w1, 11
	mov	x0, x21
	add	x20, x20, 208
	bl	BsShow
	and	w0, w0, 127
	sub	w1, w0, #24
	ldr	x2, .LC153
	lsl	x1, x1, 3
	mov	x0, x21
	add	x3, x20, x1
	ldr	w19, [x20, x1]
	ldr	w1, [x3, 4]
	bl	MP2_u_v
	ldr	x21, [sp, 32]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L819:
	ldr	x2, .LC153
	mov	x0, x21
	bl	MP2_u_v
	ldr	x21, [sp, 32]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L820:
	ldr	x2, .LC153
	mov	x0, x21
	bl	MP2_u_v
	ldr	x21, [sp, 32]
	sub	w0, w19, w0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	MP2_MbAddrInc, .-MP2_MbAddrInc
	.align	3
.LC151:
	.xword	.LANCHOR1
	.align	3
.LC153:
	.xword	.LC152
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_InitMb
	.type	MP2_InitMb, %function
MP2_InitMb:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	mov	w20, 0
	bl	_mcount
	ldr	x21, .LC159
	ldr	x19, [x22, 56]
	b	.L822
	.p2align 3
.L826:
	cmp	w0, 15
	beq	.L823
	cmp	w0, 8
	add	w20, w20, 33
	bne	.L834
.L823:
	mov	x2, x21
	mov	w1, 11
	mov	x0, x19
	bl	MP2_u_v
.L822:
	mov	w1, 11
	mov	x0, x19
	bl	BsShow
	cmp	w0, 23
	bls	.L826
	mov	x0, x19
	bl	MP2_MbAddrInc
	ldr	w1, [x22, 16]
	cbz	w1, .L827
	ldr	w2, [x22, 28]
	add	w0, w20, w0
	cbnz	w2, .L835
	sub	w2, w0, #1
.L829:
	ldr	w3, [x22, 12]
	mov	w0, 1
	str	w2, [x22, 40]
	sub	w3, w3, #1
	cmp	w2, w3
	bls	.L825
	ldr	x1, .LC161
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L834:
	ldr	x1, .LC157
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
.L825:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L835:
	add	w2, w0, w2
	b	.L829
	.p2align 3
.L827:
	ldr	x1, .LC163
	mov	w2, 0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MP2_InitMb, .-MP2_InitMb
	.align	3
.LC157:
	.xword	.LC156
	.align	3
.LC159:
	.xword	.LC158
	.align	3
.LC161:
	.xword	.LC160
	.align	3
.LC163:
	.xword	.LC162
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_GetFirstMbInSlice
	.type	MP2_GetFirstMbInSlice, %function
MP2_GetFirstMbInSlice:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	MP2_DecSliceHeader
	mov	x0, x19
	bl	MP2_InitMb
	str	wzr, [x19, 16]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MP2_GetFirstMbInSlice, .-MP2_GetFirstMbInSlice
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_DecSlice
	.type	MP2_DecSlice, %function
MP2_DecSlice:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w22, w1
	add	x20, x21, 86016
	bl	_mcount
	add	x19, x20, 600
	mov	x2, 20792
	add	x4, x20, 664
	movk	x2, 0x1, lsl 16
	ldr	x3, [x20, 1768]
	add	x2, x21, x2
	and	w22, w22, 255
	mov	x0, x19
	ldr	x1, [x2, 48]
	str	x1, [x20, 648]
	ldr	w1, [x2, 28]
	str	w1, [x20, 636]
	ldr	w1, [x2, 132]
	cmp	w1, 1
	cset	w1, eq
	str	w1, [x20, 600]
	ldr	w1, [x3, 60]
	str	w1, [x20, 604]
	ldr	w1, [x3, 56]
	str	w1, [x20, 608]
	ldp	w2, w1, [x3, 48]
	str	w22, [x20, 620]
	add	w2, w2, 15
	add	w1, w1, 15
	lsr	w2, w2, 4
	lsr	w1, w1, 4
	str	x4, [x20, 656]
	mul	w1, w2, w1
	str	w1, [x20, 612]
	bl	MP2_DecSliceHeader
	mov	x0, x19
	bl	MP2_InitMb
	str	wzr, [x20, 616]
	cmp	w0, 1
	mov	w22, w0
	beq	.L838
	ldr	x1, .LC165
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC166
	ldr	x4, [x0]
	cbz	x4, .L848
.L849:
	ldr	w0, [x20, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
.L848:
	mov	w0, 0
.L840:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L838:
	mov	x0, x21
	bl	MP2_WriteSliceInfo
	cmp	w0, 1
	beq	.L840
	ldr	x1, .LC168
	mov	w0, w22
	bl	dprint_vfmw
	ldr	x0, .LC166
	ldr	x4, [x0]
	cbnz	x4, .L849
	b	.L848
	.size	MP2_DecSlice, .-MP2_DecSlice
	.align	3
.LC165:
	.xword	.LC164
	.align	3
.LC166:
	.xword	g_event_report
	.align	3
.LC168:
	.xword	.LC167
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_GetHDR
	.type	MP2_GetHDR, %function
MP2_GetHDR:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x27, x0
	mov	x0, x30
	add	x20, x27, 86016
	add	x23, x27, 45056
	bl	_mcount
	add	x21, x20, 664
	mov	w1, 32
	mov	x0, x21
	strb	wzr, [x20, 2408]
	add	x22, x23, 312
	add	x22, x22, 40960
	ldr	x24, [x20, 1768]
	ldr	x25, [x20, 1776]
	bl	BsShow
	mov	w19, w0
	mov	x0, x21
	ldr	w26, [x22, 76]
	bl	BsPos
	str	w0, [x22, 156]
	mov	w1, 32
	mov	x0, x21
	bl	BsSkip
	cmp	w19, 439
	beq	.L920
	mov	w0, 440
	cmp	w19, 435
	ccmp	w19, w0, 4, ne
	bne	.L857
.L853:
	add	x0, x23, 312
	add	x0, x0, 40960
	ldr	w1, [x0, 152]
	cmp	w1, 1
	beq	.L921
.L854:
	cmp	w19, 435
	beq	.L859
.L924:
	ble	.L922
	cmp	w19, 437
	beq	.L863
	blt	.L880
	cmp	w19, 439
	beq	.L880
	cmp	w19, 440
	bne	.L858
	add	x19, x23, 312
	mov	x0, x21
	add	x19, x19, 40960
	ldr	w1, [x19, 8]
	ldr	w22, [x19, 148]
	add	w22, w22, w1
	bl	BsPos
	ldr	x1, .LC176
	add	w2, w22, w0, lsr 3
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x27
	bl	group_of_pictures_header
	mov	x0, x21
	bl	BsResidBits
	tbnz	w0, #31, .L923
.L880:
	mov	w21, 0
.L856:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L857:
	mov	w0, 439
	cmp	w19, 256
	ccmp	w19, w0, 4, ne
	beq	.L853
	cmp	w19, 435
	bne	.L924
.L859:
	add	x19, x23, 312
	add	x19, x19, 40960
	ldr	w0, [x19, 8]
	ldr	w22, [x19, 148]
	add	w22, w22, w0
	mov	x0, x21
	bl	BsPos
	add	w2, w22, w0, lsr 3
	ldr	x1, .LC171
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x27
	bl	sequence_header
	mov	x0, x21
	bl	BsResidBits
	tbnz	w0, #31, .L925
	ldr	w0, [x19, 76]
	and	w0, w0, 7
	cbnz	w0, .L868
	mov	w0, 1
	mov	x2, 188
	strb	w0, [x24]
	mov	x1, x24
	mov	x0, x25
	mov	w21, 0
	bl	memcpy
	ldr	w0, [x19, 80]
	add	w0, w0, 1
	str	w0, [x19, 80]
	b	.L856
	.p2align 3
.L922:
	cmp	w19, 256
	beq	.L861
	blt	.L858
	cmp	w19, 433
	blt	.L858
	add	x23, x23, 312
	mov	x0, x21
	add	x23, x23, 40960
	ldr	w1, [x23, 8]
	ldr	w19, [x23, 148]
	add	w19, w19, w1
	bl	BsPos
	ldr	x1, .LC184
	add	w2, w19, w0, lsr 3
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x27
	bl	MP2_GetUsrData
	mov	w21, w0
	cbnz	w0, .L874
	ldr	w0, [x20, 2392]
	add	x1, x23, 16
	bl	MP2_ClearCurPacket
	ldr	x0, .LC172
	ldr	x4, [x0]
	cbz	x4, .L881
	ldr	w0, [x20, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	mov	w21, -1
	blr	x4
	b	.L856
	.p2align 3
.L861:
	add	x19, x23, 312
	mov	x0, x21
	add	x19, x19, 40960
	ldr	w1, [x19, 8]
	ldr	w22, [x19, 148]
	add	w22, w22, w1
	bl	BsPos
	ldr	x1, .LC180
	add	w2, w22, w0, lsr 3
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x27
	bl	picture_header
	mov	x0, x21
	bl	BsResidBits
	tbz	w0, #31, .L880
	ldr	x0, .LC172
	str	w26, [x19, 76]
	ldr	x4, [x0]
	cbz	x4, .L873
	ldr	w0, [x20, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L873:
	ldr	w0, [x20, 2392]
	mov	x1, 41288
	add	x1, x23, x1
	mov	w21, -1
	bl	MP2_ClearCurPacket
	ldr	x1, .LC182
	mov	w2, 256
	mov	w0, 1
	bl	dprint_vfmw
	b	.L856
	.p2align 3
.L921:
	ldr	w2, [x0, 76]
	tbz	x2, 22, .L854
	strb	w1, [x20, 2408]
	orr	w2, w2, 8388608
	str	wzr, [x0, 152]
	mov	w21, w1
	str	w2, [x0, 76]
	mov	x0, x27
	bl	MP2_ProtocolPPSCheck
	b	.L856
	.p2align 3
.L920:
	ldr	x0, .LC169
	mov	x2, 40
	mov	x1, x21
	ldr	x3, [x0, 104]
	add	x0, x29, 104
	blr	x3
	mov	w1, 32
	add	x0, x29, 104
	bl	BsGet
	mov	w28, w0
	mov	w1, 32
	add	x0, x29, 104
	bl	BsGet
	mov	w1, 20553
	movk	w1, 0x4853, lsl 16
	cmp	w28, w1
	mov	w1, 20036
	movk	w1, 0x4345, lsl 16
	ccmp	w0, w1, 0, eq
	bne	.L853
	ldr	w21, [x22, 152]
	cmp	w21, 1
	beq	.L926
.L855:
	mov	w0, 1
	mov	w21, 0
	strb	w0, [x20, 2410]
	b	.L856
	.p2align 3
.L858:
	sub	w0, w19, #257
	cmp	w0, 174
	bhi	.L876
	add	x22, x23, 312
	add	x22, x22, 40960
	ldr	w0, [x22, 72]
	cbz	w0, .L877
.L879:
	add	x23, x23, 312
	mov	w1, 589824
	add	x23, x23, 40960
	ldr	w0, [x23, 76]
	tst	w0, w1
	beq	.L880
	orr	w0, w0, 4194304
	mov	w20, 1
	str	w0, [x23, 76]
	mov	w1, w19
	str	w20, [x23, 152]
	mov	x0, x27
	bl	MP2_DecSlice
	cmp	w0, w20
	mov	w21, w0
	beq	.L856
	ldr	x1, .LC190
	mov	w2, w19
	mov	w0, w20
	mov	w21, 0
	bl	dprint_vfmw
	b	.L856
	.p2align 3
.L863:
	add	x19, x23, 312
	mov	x0, x21
	add	x19, x19, 40960
	ldr	w1, [x19, 8]
	ldr	w22, [x19, 148]
	add	w22, w22, w1
	bl	BsPos
	ldr	x1, .LC188
	add	w2, w22, w0, lsr 3
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x27
	bl	extension
	mov	x0, x21
	bl	BsResidBits
	tbz	w0, #31, .L880
	ldr	x0, .LC172
	str	w26, [x19, 76]
	ldr	x4, [x0]
	cbz	x4, .L875
	ldr	w0, [x20, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L875:
	mov	x1, 41288
	ldr	w0, [x20, 2392]
	add	x1, x23, x1
	mov	w21, -1
	bl	MP2_ClearCurPacket
	b	.L856
	.p2align 3
.L868:
	ldrb	w0, [x25]
	cmp	w0, 1
	beq	.L927
.L876:
	ldr	x0, .LC172
	ldr	x4, [x0]
	cbz	x4, .L880
.L916:
	ldr	w0, [x20, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	mov	w21, 0
	blr	x4
	b	.L856
	.p2align 3
.L874:
	ldr	x1, .LC186
	mov	w0, 1
	bl	dprint_vfmw
	b	.L856
	.p2align 3
.L926:
	ldr	w0, [x22, 76]
	tbz	x0, 22, .L855
	strb	w21, [x20, 2410]
	orr	w0, w0, 8388608
	strb	w21, [x20, 2408]
	str	w0, [x22, 76]
	mov	x0, x27
	str	wzr, [x22, 152]
	bl	MP2_ProtocolPPSCheck
	b	.L856
	.p2align 3
.L881:
	mov	w21, -1
	b	.L856
	.p2align 3
.L877:
	mov	x0, x27
	mov	w21, 0
	bl	MP2_GetSequenceInfo
	ldr	w0, [x22, 144]
	cbz	w0, .L856
	ldrb	w0, [x20, 2409]
	cmp	w0, 1
	bne	.L879
	b	.L856
	.p2align 3
.L927:
	mov	x1, x25
	mov	x2, 188
	mov	x0, x24
	bl	memcpy
	ldr	x1, .LC172
	ldr	w0, [x19, 76]
	ldr	x4, [x1]
	and	w0, w0, -8
	str	w0, [x19, 76]
	cbnz	x4, .L916
	b	.L880
.L923:
	ldr	x0, .LC172
	str	w26, [x19, 76]
	ldr	x4, [x0]
	cbz	x4, .L872
	ldr	w0, [x20, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L872:
	ldr	w0, [x20, 2392]
	mov	x1, 41288
	add	x1, x23, x1
	mov	w21, -1
	bl	MP2_ClearCurPacket
	ldr	x1, .LC178
	mov	w2, 440
	mov	w0, 1
	bl	dprint_vfmw
	b	.L856
.L925:
	ldr	x0, .LC172
	str	w26, [x19, 76]
	ldr	x4, [x0]
	cbz	x4, .L867
	ldr	w0, [x20, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L867:
	ldr	w0, [x20, 2392]
	mov	x1, 41288
	add	x1, x23, x1
	mov	w21, -1
	bl	MP2_ClearCurPacket
	ldr	x1, .LC174
	mov	w2, 435
	mov	w0, 1
	bl	dprint_vfmw
	b	.L856
	.size	MP2_GetHDR, .-MP2_GetHDR
	.align	3
.LC169:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC171:
	.xword	.LC170
	.align	3
.LC172:
	.xword	g_event_report
	.align	3
.LC174:
	.xword	.LC173
	.align	3
.LC176:
	.xword	.LC175
	.align	3
.LC178:
	.xword	.LC177
	.align	3
.LC180:
	.xword	.LC179
	.align	3
.LC182:
	.xword	.LC181
	.align	3
.LC184:
	.xword	.LC183
	.align	3
.LC186:
	.xword	.LC185
	.align	3
.LC188:
	.xword	.LC187
	.align	3
.LC190:
	.xword	.LC189
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG2DEC_Decode
	.type	MPEG2DEC_Decode, %function
MPEG2DEC_Decode:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	add	x19, x20, 86016
	bl	_mcount
	add	x22, x19, 1216
	ldr	x3, .LC191
	mov	x1, x21
	mov	x2, 176
	mov	x0, x22
	add	x23, x20, 45056
	ldr	x3, [x3, 104]
	add	x21, x23, 312
	blr	x3
	ldr	w2, [x19, 2392]
	mov	x1, x22
	mov	x0, x21
	bl	MP2_ReceivePacket
	cmp	w0, 1
	beq	.L929
	ldr	x1, .LC193
	mov	w0, 1
	mov	w21, 0
	bl	dprint_vfmw
.L930:
	mov	w0, w21
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L929:
	add	x21, x21, 40960
	add	x0, x19, 664
	ldr	x1, [x21, 48]
	ldr	w2, [x21, 28]
	bl	BsInit
	ldr	x0, [x20]
	ldr	w1, [x0, 872]
	cmp	w1, 1
	beq	.L960
.L931:
	ldr	w0, [x19, 388]
	str	w0, [x19, 592]
.L932:
	mov	x0, x20
	bl	MP2_GetHDR
	add	w0, w0, 1
	cmp	w0, 1
	bls	.L961
	mov	x0, x20
	bl	MP2_DecodingDecide
	cbz	w0, .L962
	cmn	w0, #1
	beq	.L935
	mov	x0, x20
	bl	MP2_GetPictureInfo
	cbnz	w0, .L938
	ldr	w0, [x19, 2392]
	mov	x1, 41288
	add	x1, x23, x1
	bl	MP2_ClearCurPacket
	mov	x0, x20
	bl	MP2_ClearCurPic
	ldr	x0, .LC194
	ldr	x4, [x0]
	cbz	x4, .L939
	ldr	w0, [x19, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L939:
	ldr	x1, .LC198
	mov	w0, 1
	mov	w21, 0
	bl	dprint_vfmw
	b	.L930
	.p2align 3
.L961:
	ldr	x0, [x20]
	ldr	w1, [x0, 872]
	cmp	w1, 1
	beq	.L963
.L934:
	ldrb	w0, [x19, 2409]
	cmp	w0, 1
	beq	.L935
	mov	x1, 41288
	ldr	w0, [x19, 2392]
	add	x1, x23, x1
	mov	w21, 0
	bl	MP2_ClearCurPacket
	b	.L930
	.p2align 3
.L963:
	ldr	w0, [x0, 912]
	cbz	w0, .L934
.L935:
	mov	w21, 0
	ldr	x23, [sp, 48]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L960:
	ldr	w0, [x0, 912]
	cmp	w0, 1
	bne	.L931
	ldr	w0, [x19, 592]
	str	w0, [x19, 388]
	b	.L932
	.p2align 3
.L962:
	ldr	w0, [x19, 2392]
	mov	x1, 41288
	add	x1, x23, x1
	bl	MP2_ClearCurPacket
	mov	x0, x20
	bl	MP2_ClearCurPic
	ldr	x0, .LC194
	ldr	x4, [x0]
	cbz	x4, .L937
	ldr	w0, [x19, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L937:
	ldr	x1, .LC196
	mov	w0, 22
	mov	w21, 0
	bl	dprint_vfmw
	b	.L930
	.p2align 3
.L938:
	mov	x0, x20
	bl	MP2_DecNewPic
	mov	w21, w0
	cbz	w0, .L964
	add	x0, x23, 312
	add	x0, x0, 40960
	ldr	w2, [x0, 148]
	ldr	w21, [x0, 8]
	ldr	w1, [x0, 28]
	lsl	w21, w21, 3
	add	w1, w2, w1
	str	w1, [x0, 148]
	b	.L930
	.p2align 3
.L964:
	add	x22, x23, 312
	ldr	w0, [x19, 2392]
	add	x22, x22, 40960
	mov	w2, 1
	ldr	w1, [x22, 180]
	bl	FSP_ClearLogicFs
	ldr	w0, [x19, 2392]
	add	x1, x22, 16
	bl	MP2_ClearCurPacket
	mov	x0, x20
	bl	MP2_ClearCurPic
	ldr	x0, .LC194
	ldr	x4, [x0]
	cbz	x4, .L941
	ldr	w0, [x19, 2392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L941:
	ldr	x1, .LC200
	mov	w0, 1
	bl	dprint_vfmw
	b	.L930
	.size	MPEG2DEC_Decode, .-MPEG2DEC_Decode
	.align	3
.LC191:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC193:
	.xword	.LC192
	.align	3
.LC194:
	.xword	g_event_report
	.align	3
.LC196:
	.xword	.LC195
	.align	3
.LC198:
	.xword	.LC197
	.align	3
.LC200:
	.xword	.LC199
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP2_GetPacketState
	.type	MP2_GetPacketState, %function
MP2_GetPacketState:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 86016
	bl	_mcount
	mov	w0, 2
	ldrb	w1, [x19, 2408]
	ldr	x19, [sp, 16]
	cmp	w1, 1
	csel	w0, w0, wzr, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	MP2_GetPacketState, .-MP2_GetPacketState
	.global	g_tmpusd
	.global	zigzag
	.global	default_intra_quantizer_matrix
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__FUNCTION__.11731, %object
	.size	__FUNCTION__.11731, 19
__FUNCTION__.11731:
	.string	"MP2_DecodingDecide"
	.zero	5
.LC0:
	.word	0
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	6
	.word	7
	.word	8
	.word	10
	.word	12
	.word	14
	.word	16
	.word	18
	.word	20
	.word	22
	.word	24
	.word	28
	.word	32
	.word	36
	.word	40
	.word	44
	.word	48
	.word	52
	.word	56
	.word	64
	.word	72
	.word	80
	.word	88
	.word	96
	.word	104
	.word	112
	.align	3
.LANCHOR1 = . + 0
	.type	CSWTCH.46, %object
	.size	CSWTCH.46, 12
CSWTCH.46:
	.word	5
	.word	1
	.word	2
	.zero	4
	.type	frame_rate_Table, %object
	.size	frame_rate_Table, 64
frame_rate_Table:
	.word	0
	.word	24551
	.word	24576
	.word	25600
	.word	30689
	.word	30720
	.word	51200
	.word	61378
	.word	61440
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.type	tab_for_first_4bit, %object
	.size	tab_for_first_4bit, 128
tab_for_first_4bit:
	.word	8
	.word	4
	.word	9
	.word	5
	.word	5
	.word	4
	.word	4
	.word	4
	.word	3
	.word	3
	.word	3
	.word	3
	.word	2
	.word	3
	.word	2
	.word	3
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.type	tab_for_last_7bit, %object
	.size	tab_for_last_7bit, 832
tab_for_last_7bit:
	.word	33
	.word	11
	.word	32
	.word	11
	.word	31
	.word	11
	.word	30
	.word	11
	.word	29
	.word	11
	.word	28
	.word	11
	.word	27
	.word	11
	.word	26
	.word	11
	.word	25
	.word	11
	.word	24
	.word	11
	.word	23
	.word	11
	.word	22
	.word	11
	.word	21
	.word	10
	.word	21
	.word	10
	.word	20
	.word	10
	.word	20
	.word	10
	.word	19
	.word	10
	.word	19
	.word	10
	.word	18
	.word	10
	.word	18
	.word	10
	.word	17
	.word	10
	.word	17
	.word	10
	.word	16
	.word	10
	.word	16
	.word	10
	.word	15
	.word	8
	.word	15
	.word	8
	.word	15
	.word	8
	.word	15
	.word	8
	.word	15
	.word	8
	.word	15
	.word	8
	.word	15
	.word	8
	.word	15
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.data
	.align	3
.LANCHOR3 = . + 0
	.type	zigzag, %object
	.size	zigzag, 64
zigzag:
	.byte	0
	.byte	1
	.byte	8
	.byte	16
	.byte	9
	.byte	2
	.byte	3
	.byte	10
	.byte	17
	.byte	24
	.byte	32
	.byte	25
	.byte	18
	.byte	11
	.byte	4
	.byte	5
	.byte	12
	.byte	19
	.byte	26
	.byte	33
	.byte	40
	.byte	48
	.byte	41
	.byte	34
	.byte	27
	.byte	20
	.byte	13
	.byte	6
	.byte	7
	.byte	14
	.byte	21
	.byte	28
	.byte	35
	.byte	42
	.byte	49
	.byte	56
	.byte	57
	.byte	50
	.byte	43
	.byte	36
	.byte	29
	.byte	22
	.byte	15
	.byte	23
	.byte	30
	.byte	37
	.byte	44
	.byte	51
	.byte	58
	.byte	59
	.byte	52
	.byte	45
	.byte	38
	.byte	31
	.byte	39
	.byte	46
	.byte	53
	.byte	60
	.byte	61
	.byte	54
	.byte	47
	.byte	55
	.byte	62
	.byte	63
	.type	default_intra_quantizer_matrix, %object
	.size	default_intra_quantizer_matrix, 64
default_intra_quantizer_matrix:
	.byte	8
	.byte	16
	.byte	19
	.byte	22
	.byte	26
	.byte	27
	.byte	29
	.byte	34
	.byte	16
	.byte	16
	.byte	22
	.byte	24
	.byte	27
	.byte	29
	.byte	34
	.byte	37
	.byte	19
	.byte	22
	.byte	26
	.byte	27
	.byte	29
	.byte	34
	.byte	34
	.byte	38
	.byte	22
	.byte	22
	.byte	26
	.byte	27
	.byte	29
	.byte	34
	.byte	37
	.byte	40
	.byte	22
	.byte	26
	.byte	27
	.byte	29
	.byte	32
	.byte	35
	.byte	40
	.byte	48
	.byte	26
	.byte	27
	.byte	29
	.byte	32
	.byte	35
	.byte	40
	.byte	48
	.byte	58
	.byte	26
	.byte	27
	.byte	29
	.byte	34
	.byte	38
	.byte	46
	.byte	56
	.byte	69
	.byte	27
	.byte	29
	.byte	35
	.byte	38
	.byte	46
	.byte	56
	.byte	69
	.byte	83
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC1:
	ASCII(.string	"image size abnormal(%dx%d)\n" )
	.zero	4
.LC3:
	ASCII(.string	"mp2 actual frame size(%dx%d) exeed max config(%dx%d)\n" )
	.zero	2
.LC6:
	ASCII(.string	"partition fs memory fail!\n" )
	.zero	5
.LC8:
	ASCII(.string	"FSP_ConfigInstance fail!\n" )
	.zero	6
.LC11:
	ASCII(.string	"ERROR: frm %d, integrality: %#x\n" )
	.zero	7
.LC13:
	ASCII(.string	"ERROR: frm %d, No Img buffer is allocated\n" )
	.zero	5
.LC15:
	ASCII(.string	"----------------- mp2 Imode -> %d ------------------\n" )
	.zero	2
.LC18:
	ASCII(.string	"frm %d pic type %d struct is %d total pos %#x\n" )
	.zero	1
.LC20:
	ASCII(.string	"NOTE: Drop non-I, cur is B\n" )
	.zero	4
.LC22:
	ASCII(.string	"NOTE: Drop non-I, cur is P\n" )
	.zero	4
.LC24:
	ASCII(.string	"NOTE: Drop non-I\n" )
	.zero	6
.LC26:
	ASCII(.string	"NOTE: Drop B\n" )
	.zero	2
.LC29:
	ASCII(.string	"%s %d pFwdImg = NULL,  pBwdImg = NULL!!\n" )
	.zero	7
.LC31:
	ASCII(.string	"fuction return value is null,%s %d !!\n" )
	.zero	1
.LC33:
	ASCII(.string	"dec P frame, fwd ref err(%d) > ref_thr(%d)\n" )
	.zero	4
.LC35:
	ASCII(.string	"dec B frame, bwd ref err(%d) > ref_thr(%d)\n" )
	.zero	4
.LC37:
	ASCII(.string	"dec B frame, fwd ref err(%d) > ref_thr(%d)\n" )
	.zero	4
.LC40:
	ASCII(.string	"Protocol %d\n" )
	.zero	3
.LC42:
	ASCII(.string	"seq size change, call vpss to release all frames.\n" )
	.zero	5
.LC44:
	ASCII(.string	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d,new w/h = %d/%d\n" )
	.zero	3
.LC47:
	ASCII(.string	"new seq size %dx%d\n" )
	.zero	4
.LC51:
	ASCII(.string	"get frame store fail!\n" )
	.zero	1
.LC53:
	ASCII(.string	"line: %d, pImage is NULL!\n" )
	.zero	5
.LC55:
	ASCII(.string	"get image buffer ok: LogicFsID = %d\n" )
	.zero	3
.LC57:
	ASCII(.string	"decode %p, disp %p, tf %p\n" )
	.zero	5
.LC62:
	ASCII(.string	"ERROR: frm %d, lost one field\n" )
	.zero	1
.LC64:
	ASCII(.string	"ERROR: frm %d, lost one field flg = %d, last_struct = %d\n" )
	.zero	6
.LC66:
	ASCII(.string	"Fatal Error: frm%d, get image buffer failed\n" )
	.zero	3
.LC68:
	ASCII(.string	"dec_pts: %lld Usertag: %lld\n" )
	.zero	3
.LC70:
	ASCII(.string	"---------- unref %d\n" )
	.zero	3
.LC72:
	ASCII(.string	"++++++++++ ref %d\n" )
	.zero	5
.LC74:
	ASCII(.string	"line: %d(pCur, pFwd, pBwd)(%p, %p, %p)\n" )
.LC76:
	ASCII(.string	"CurFsID(fwd%d,%d,bwd%d) abnormal!(%p,%p,%p)\n" )
	.zero	3
.LC78:
	ASCII(.string	"dec addr: cur,cur2d,fwd,bwd,pmv = %p,%p,%p,%p,%p\n" )
	.zero	6
.LC84:
	ASCII(.string	"\nmpeg2 copy last 3 bytes of the first packet!\n\n" )
.LC88:
	ASCII(.string	"pMp2CodecInfo->pUsrDatArray[%d] is null\n" )
	.zero	7
.LC100:
	ASCII(.string	"ERROR: unsupported profile %d\n" )
	.zero	1
.LC102:
	ASCII(.string	"ERROR: unsupported level %d\n" )
	.zero	3
.LC109:
	ASCII(.string	"-1 == VCTRL_GetChanIDByCtx() Err! \n" )
	.zero	4
.LC111:
	ASCII(.string	"Decoding quits at frm %d\n" )
	.zero	6
.LC114:
	ASCII(.string	"ERROR: frm %d, Invalid packet pointer!\n" )
.LC116:
	ASCII(.string	"line %d, pPacket->StreamPack[1].IsLastSeg = %d\n" )
.LC118:
	ASCII(.string	"ErrRatio = %d\n" )
	.zero	1
.LC121:
	ASCII(.string	"pImg is null, pCurImg %#x pBwdImg %#x pFwdImg %#x\n" )
	.zero	5
.LC123:
	ASCII(.string	"DEBUG: DEC over\n" )
	.zero	7
.LC127:
	ASCII(.string	"insert buf %d ref %d\n" )
	.zero	2
.LC129:
	ASCII(.string	"err(%d) > out_thr(%d)\n" )
	.zero	1
.LC131:
	ASCII(.string	"%-40s%20d\n" )
	.zero	5
.LC133:
	ASCII(.string	"slice_vertical_position" )
.LC135:
	ASCII(.string	"quantiser_scale_code" )
	.zero	3
.LC137:
	ASCII(.string	"extra_bit_slice" )
.LC139:
	ASCII(.string	"intra_slice_flag" )
	.zero	7
.LC141:
	ASCII(.string	"intra_slice" )
	.zero	4
.LC143:
	ASCII(.string	"reserved_bits" )
	.zero	2
.LC145:
	ASCII(.string	"extra_infrmation_slice" )
	.zero	1
.LC147:
	ASCII(.string	"Slice num = %d >= %d\n" )
	.zero	2
.LC149:
	ASCII(.string	"seg info is wrong! IsLastSeg[0] = %d, IsLastSeg[1] = %d\n" )
	.zero	7
.LC152:
	ASCII(.string	"macroblock_address_increment" )
	.zero	3
.LC156:
	ASCII(.string	"macroblock_escape ERROR\n" )
	.zero	7
.LC158:
	ASCII(.string	"macroblock_escape" )
	.zero	6
.LC160:
	ASCII(.string	"SlcStartMbNum = %d > PicEndMum = %d\n" )
	.zero	3
.LC162:
	ASCII(.string	"FirstMbInSliceFlag = %d\n" )
	.zero	7
.LC164:
	ASCII(.string	"MP2_GetFirstMbInSlice Failed!\n" )
	.zero	1
.LC167:
	ASCII(.string	"MP2_WriteSliceInfo Failed!\n" )
	.zero	4
.LC170:
	ASCII(.string	"found sh %#x\n" )
	.zero	2
.LC173:
	ASCII(.string	"SEQUENCE_HEADER_CODE Bitstream is not enough!\n" )
	.zero	1
.LC175:
	ASCII(.string	"found gh %#x\n" )
	.zero	2
.LC177:
	ASCII(.string	"GROUP_START_CODE Bitstream is not enough!\n" )
	.zero	5
.LC179:
	ASCII(.string	"found ph %#x\n" )
	.zero	2
.LC181:
	ASCII(.string	"PICTURE_START_CODE Bitstream is not enough!\n" )
	.zero	3
.LC183:
	ASCII(.string	"found ud %#x\n" )
	.zero	2
.LC185:
	ASCII(.string	"cann't get ud pointer\n" )
	.zero	1
.LC187:
	ASCII(.string	"found ext %#x\n" )
	.zero	1
.LC189:
	ASCII(.string	"MP2_DecSlice is wrong!\n" )
.LC192:
	ASCII(.string	"line %d, MP2_ReceivePacket receive packet failed!\n" )
	.zero	5
.LC195:
	ASCII(.string	"MP2 Decode Decide is wrong!\n" )
	.zero	3
.LC197:
	ASCII(.string	"MP2 get picture info is wrong!\n" )
.LC199:
	ASCII(.string	"MP2 dec new pic failed!\n" )
	.bss
	.align	3
.LANCHOR2 = . + 0
	.type	g_tmpusd, %object
	.size	g_tmpusd, 1024
g_tmpusd:
	.zero	1024
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
