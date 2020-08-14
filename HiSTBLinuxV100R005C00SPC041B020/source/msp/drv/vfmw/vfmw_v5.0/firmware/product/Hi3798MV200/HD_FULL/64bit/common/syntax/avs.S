#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"avs.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	avs_f_v
	.type	avs_f_v, %function
avs_f_v:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	mov	x21, x2
	bl	_mcount
	mov	w1, w20
	mov	x0, x19
	bl	BsGet
	mov	w19, w0
	ldr	x1, .LC1
	mov	w4, w20
	mov	w3, w0
	mov	x2, x21
	mov	w0, 7
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	avs_f_v, .-avs_f_v
	.align	3
.LC1:
	.xword	.LC0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	avs_u_v
	.type	avs_u_v, %function
avs_u_v:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	mov	x21, x2
	bl	_mcount
	mov	x2, x21
	mov	w1, w20
	mov	x0, x19
	bl	avs_f_v
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	avs_u_v, .-avs_u_v
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	avs_ue_v
	.type	avs_ue_v, %function
avs_ue_v:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	x22, x1
	bl	_mcount
	mov	w1, 32
	mov	x0, x21
	bl	BsShow
	bl	ZerosMS_32
	cmp	w0, 31
	mov	w19, w0
	bls	.L7
	mov	w20, -1
	mov	w4, 32
.L4:
	ldr	x1, .LC2
	mov	w3, w20
	mov	x2, x22
	mov	w0, 7
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L7:
	mov	w1, w0
	mov	x0, x21
	add	w23, w19, 1
	bl	BsSkip
	mov	w1, w23
	mov	x0, x21
	bl	BsShow
	sub	w20, w0, #1
	mov	w1, w23
	mov	x0, x21
	bl	BsSkip
	lsl	w4, w19, 1
	add	w4, w4, 1
	b	.L4
	.size	avs_ue_v, .-avs_ue_v
	.align	3
.LC2:
	.xword	.LC0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	avs_se_v
	.type	avs_se_v, %function
avs_se_v:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	mov	w1, 32
	mov	x0, x20
	bl	BsShow
	bl	ZerosMS_32
	cmp	w0, 31
	mov	w19, w0
	bls	.L13
	mov	w20, 2147483647
	mov	w4, 32
.L9:
	ldr	x1, .LC3
	mov	w3, w20
	mov	x2, x21
	mov	w0, 7
	bl	dprint_vfmw
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L13:
	mov	w1, w0
	mov	x0, x20
	add	w22, w19, 1
	bl	BsSkip
	mov	w1, w22
	mov	x0, x20
	bl	BsShow
	sub	w0, w0, #1
	and	w1, w0, 1
	asr	w3, w0, 1
	cmp	w1, wzr
	mov	x0, x20
	mov	w1, w22
	neg	w20, w3
	csinc	w20, w20, w3, eq
	bl	BsSkip
	lsl	w4, w19, 1
	add	w4, w4, 1
	b	.L9
	.size	avs_se_v, .-avs_se_v
	.align	3
.LC3:
	.xword	.LC0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_next_start_code
	.type	AVS_next_start_code, %function
AVS_next_start_code:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w19, 0
	bl	_mcount
	mov	x0, x20
	bl	BsToNextByte
	mov	x0, x20
	bl	BsResidBits
	cmp	w0, wzr
	add	w1, w0, 7
	csel	w21, w1, w0, lt
	asr	w21, w21, 3
	sub	w21, w21, #3
	cmp	w21, wzr
	bgt	.L21
	b	.L18
	.p2align 3
.L24:
	bl	BsSkip
	cmp	w19, w21
	beq	.L18
.L21:
	mov	w1, 24
	mov	x0, x20
	add	w19, w19, 1
	bl	BsShow
	mov	w2, w0
	cmp	w2, 1
	mov	w1, 8
	mov	x0, x20
	bne	.L24
	mov	w0, w2
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L18:
	mov	w2, 0
	ldr	x21, [sp, 32]
	mov	w0, w2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	AVS_next_start_code, .-AVS_next_start_code
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_sequence_header
	.type	AVS_sequence_header, %function
AVS_sequence_header:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x23, x0
	mov	x0, x30
	add	x21, x23, 12288
	mov	x19, x1
	bl	_mcount
	ldr	x2, .LC5
	mov	w1, 32
	ldr	x20, [x21, 3160]
	ldr	w4, [x21, 2772]
	mov	x0, x20
	and	w4, w4, -65537
	str	w4, [x21, 2772]
	bl	avs_f_v
	ldr	x2, .LC7
	mov	w1, 8
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 16]
	ldr	x2, .LC9
	mov	w1, 8
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 20]
	ldr	x2, .LC11
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 15]
	ldr	x2, .LC13
	mov	w1, 14
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 24]
	ldr	x2, .LC15
	mov	w1, 14
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 28]
	ldr	x2, .LC17
	mov	w1, 2
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 11]
	ldr	x2, .LC19
	mov	w1, 3
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 12]
	ldr	x2, .LC21
	mov	w1, 4
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 13]
	ldr	x2, .LC23
	mov	w1, 4
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 14]
	ldr	x2, .LC25
	mov	w1, 18
	mov	x0, x20
	bl	avs_f_v
	ldr	x22, .LC27
	mov	w1, 1
	str	w0, [x19, 32]
	mov	x2, x22
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 10]
	ldr	x2, .LC29
	mov	w1, 12
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 36]
	ldr	x2, .LC31
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 9]
	mov	x2, x22
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 10]
	ldr	x2, .LC33
	mov	w1, 18
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 40]
	ldr	x2, .LC35
	mov	w1, 3
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 8]
	ldr	w0, [x19, 24]
	ldr	w1, [x21, 2812]
	cmp	w0, w1
	beq	.L32
.L26:
	ldr	x0, .LC36
	mov	w2, 1
	str	w2, [x21, 2784]
	ldr	x4, [x0]
	cbz	x4, .L27
	ldp	w3, w2, [x19, 24]
	ldr	w5, [x21, 2816]
	ldr	w0, [x23, 4]
	strh	w1, [x29, 72]
	mov	w1, 2
	strh	w3, [x29, 76]
	mov	w3, 8
	strh	w2, [x29, 78]
	add	x2, x29, 72
	strh	w5, [x29, 74]
	blr	x4
.L27:
	mov	x1, x19
	mov	x2, 372
	add	x0, x21, 2788
	bl	memcpy
	ldr	w1, [x21, 2772]
	mov	w0, 1
	ldr	w2, [x21, 2780]
	str	w0, [x21, 2776]
	orr	w1, w1, 65536
	add	w2, w2, w0
	str	w1, [x21, 2772]
	str	w2, [x21, 2780]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L32:
	ldr	w3, [x19, 28]
	ldr	w0, [x21, 2816]
	cmp	w3, w0
	bne	.L26
	b	.L27
	.size	AVS_sequence_header, .-AVS_sequence_header
	.align	3
.LC5:
	.xword	.LC4
	.align	3
.LC7:
	.xword	.LC6
	.align	3
.LC9:
	.xword	.LC8
	.align	3
.LC11:
	.xword	.LC10
	.align	3
.LC13:
	.xword	.LC12
	.align	3
.LC15:
	.xword	.LC14
	.align	3
.LC17:
	.xword	.LC16
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
.LC29:
	.xword	.LC28
	.align	3
.LC31:
	.xword	.LC30
	.align	3
.LC33:
	.xword	.LC32
	.align	3
.LC35:
	.xword	.LC34
	.align	3
.LC36:
	.xword	g_event_report
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_i_picture_header
	.type	AVS_i_picture_header, %function
AVS_i_picture_header:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x3, .LC39
	mov	x2, 272
	mov	w1, 0
	add	x0, x19, 76
	ldr	x20, [x21, 512]
	ldr	x3, [x3, 96]
	blr	x3
	ldr	w3, [x21, 124]
	mov	w1, 32
	ldr	x2, .LC41
	mov	x0, x20
	and	w3, w3, -262145
	str	w3, [x21, 124]
	bl	avs_f_v
	ldr	x2, .LC43
	mov	w1, 16
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 92]
	ldr	w1, [x19, 16]
	ldr	x22, .LC44
	cmp	w1, 72
	beq	.L67
	ldr	x2, .LC48
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 86]
	beq	.L68
.L35:
	mov	x2, x22
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 104]
	ldr	x2, .LC52
	mov	w1, 8
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 108]
	ldrb	w1, [x19, 9]
	cmp	w1, 1
	beq	.L69
.L36:
	ldr	x2, .LC56
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	uxtb	w0, w0
	ldrb	w1, [x19, 15]
	strb	w0, [x19, 76]
	cmp	w1, 1
	beq	.L70
	cbz	w0, .L71
.L38:
	mov	w0, 1
	strb	w0, [x19, 77]
.L39:
	ldr	x2, .LC60
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 78]
	ldr	x2, .LC62
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 79]
	ldr	x2, .LC64
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 80]
	ldr	x2, .LC66
	mov	w1, 6
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 81]
	ldrh	w1, [x19, 76]
	cbz	w1, .L72
.L40:
	ldr	x2, .LC69
	mov	w1, 4
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 83]
	ldr	x2, .LC71
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	uxtb	w0, w0
	stp	wzr, wzr, [x19, 116]
	strb	w0, [x19, 84]
	cbz	w0, .L73
.L42:
	ldr	w0, [x19, 16]
	cmp	w0, 72
	beq	.L74
.L44:
	str	wzr, [x19, 344]
	mov	w0, 1
	strb	w0, [x19, 89]
	ldr	w0, [x21, 124]
	orr	w0, w0, 262144
	str	w0, [x21, 124]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L71:
	ldr	x2, .LC58
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 77]
	b	.L39
	.p2align 3
.L73:
	ldr	x2, .LC73
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	uxtb	w0, w0
	strb	w0, [x19, 85]
	cbz	w0, .L42
	ldr	x1, .LC75
	mov	x0, x20
	bl	avs_se_v
	str	w0, [x19, 116]
	ldr	x1, .LC77
	mov	x0, x20
	bl	avs_se_v
	str	w0, [x19, 120]
	b	.L42
	.p2align 3
.L72:
	ldr	x2, .LC68
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 82]
	b	.L40
	.p2align 3
.L70:
	strb	w1, [x19, 76]
	b	.L38
	.p2align 3
.L74:
	ldr	x2, .LC79
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 124]
	cbnz	w0, .L45
.L53:
	ldr	x2, .LC81
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 328]
	b	.L44
	.p2align 3
.L67:
	mov	w1, 1
	mov	x2, x22
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 104]
	ldr	x2, .LC46
	mov	w1, 7
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 96]
	ldr	w1, [x19, 92]
	mov	w2, 1
	strb	w2, [x19]
	ldr	x2, .LC48
	orr	w0, w0, w1, lsl 7
	mov	w1, 1
	str	w0, [x19, 92]
	mov	x0, x20
	bl	avs_f_v
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 86]
	bne	.L35
	.p2align 2
.L68:
	ldr	x2, .LC50
	mov	w1, 24
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 100]
	b	.L35
	.p2align 3
.L69:
	ldr	x1, .LC54
	mov	x0, x20
	bl	avs_ue_v
	str	w0, [x19, 112]
	b	.L36
	.p2align 3
.L45:
	ldr	x2, .LC83
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 128]
	ldr	x2, .LC85
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 132]
	cbz	w0, .L75
.L46:
	ldr	x2, .LC91
	mov	w1, 2
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 144]
	ldr	x2, .LC93
	mov	w1, 2
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 148]
	ldr	w1, [x19, 144]
	cmp	w1, 1
	beq	.L47
	ldr	w0, [x19, 128]
	cmp	w0, wzr
	ccmp	w1, 3, 0, eq
	beq	.L47
	cmp	w1, 2
	add	x22, x19, 176
	bne	.L53
.L52:
	ldr	x23, .LC97
	add	x24, x19, 200
	.p2align 2
.L51:
	mov	x1, x23
	mov	x0, x20
	bl	avs_se_v
	str	w0, [x22], 4
	cmp	x22, x24
	bne	.L51
	b	.L53
	.p2align 3
.L75:
	ldr	x1, .LC87
	mov	x0, x20
	bl	avs_se_v
	str	w0, [x19, 136]
	ldr	x1, .LC89
	mov	x0, x20
	bl	avs_se_v
	str	w0, [x19, 140]
	b	.L46
	.p2align 3
.L47:
	ldr	x24, .LC95
	add	x23, x19, 152
	add	x22, x19, 176
	.p2align 2
.L49:
	mov	x1, x24
	mov	x0, x20
	bl	avs_se_v
	str	w0, [x23], 4
	cmp	x23, x22
	bne	.L49
	ldr	w0, [x19, 144]
	cmp	w0, 2
	beq	.L52
	ldr	w1, [x19, 128]
	cmp	w1, wzr
	ccmp	w0, 3, 0, eq
	beq	.L52
	b	.L53
	.size	AVS_i_picture_header, .-AVS_i_picture_header
	.align	3
.LC39:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC41:
	.xword	.LC40
	.align	3
.LC43:
	.xword	.LC42
	.align	3
.LC44:
	.xword	.LC26
	.align	3
.LC46:
	.xword	.LC45
	.align	3
.LC48:
	.xword	.LC47
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
.LC60:
	.xword	.LC59
	.align	3
.LC62:
	.xword	.LC61
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
	.xword	.LC34
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_pb_picture_header
	.type	AVS_pb_picture_header, %function
AVS_pb_picture_header:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x3, .LC98
	mov	x2, 272
	mov	w1, 0
	add	x0, x19, 76
	ldr	x20, [x21, 512]
	ldr	x3, [x3, 96]
	blr	x3
	ldr	w3, [x21, 124]
	mov	w1, 32
	ldr	x2, .LC100
	mov	x0, x20
	and	w3, w3, -262145
	str	w3, [x21, 124]
	bl	avs_f_v
	ldr	x2, .LC101
	mov	w1, 16
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 92]
	ldr	w1, [x19, 16]
	cmp	w1, 72
	beq	.L121
.L77:
	ldr	x2, .LC105
	mov	w1, 2
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 87]
	ldr	x2, .LC106
	mov	w1, 8
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 108]
	ldrb	w1, [x19, 9]
	cmp	w1, 1
	beq	.L122
.L78:
	ldr	x2, .LC108
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	uxtb	w0, w0
	strb	w0, [x19, 76]
	cbz	w0, .L123
	mov	w0, 1
	strb	w0, [x19, 77]
.L80:
	ldr	x2, .LC112
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 78]
	ldr	x2, .LC113
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 79]
	ldr	x2, .LC114
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 80]
	ldr	x2, .LC115
	mov	w1, 6
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 81]
	ldrb	w1, [x19, 87]
	cmp	w1, 2
	beq	.L124
.L81:
	ldr	x2, .LC117
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 89]
.L83:
	ldr	x2, .LC119
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	str	wzr, [x19, 336]
	ldrb	w1, [x19, 87]
	strb	w0, [x19, 90]
	cmp	w1, 2
	str	wzr, [x19, 340]
	beq	.L125
	ldr	x2, .LC123
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 336]
.L85:
	ldrb	w0, [x19, 77]
	cmp	w0, 1
	beq	.L86
	ldr	w0, [x19, 16]
	cmp	w0, 72
	beq	.L87
.L86:
	str	wzr, [x19, 336]
	str	wzr, [x19, 340]
.L87:
	ldr	x2, .LC124
	mov	w1, 2
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 83]
	ldr	x2, .LC125
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 82]
	ldr	x2, .LC126
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	uxtb	w0, w0
	stp	wzr, wzr, [x19, 116]
	strb	w0, [x19, 84]
	cbz	w0, .L126
.L89:
	ldr	w0, [x19, 16]
	cmp	w0, 72
	beq	.L127
.L91:
	ldrb	w0, [x19, 87]
	cmp	w0, 1
	beq	.L120
	mov	w0, 2
.L120:
	str	w0, [x19, 344]
	ldr	w0, [x21, 124]
	orr	w0, w0, 262144
	str	w0, [x21, 124]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L124:
	ldrb	w0, [x19, 77]
	cmp	w0, 1
	bne	.L81
	strb	w0, [x19, 89]
	b	.L83
	.p2align 3
.L123:
	ldr	x2, .LC109
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	uxtb	w0, w0
	strb	w0, [x19, 77]
	cbnz	w0, .L80
	ldr	x2, .LC111
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	strb	w0, [x19, 88]
	b	.L80
	.p2align 3
.L126:
	ldr	x2, .LC127
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	uxtb	w0, w0
	strb	w0, [x19, 85]
	cbz	w0, .L89
	ldr	x1, .LC128
	mov	x0, x20
	bl	avs_se_v
	str	w0, [x19, 116]
	ldr	x1, .LC129
	mov	x0, x20
	bl	avs_se_v
	str	w0, [x19, 120]
	b	.L89
	.p2align 3
.L125:
	ldr	x2, .LC121
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 340]
	b	.L85
	.p2align 3
.L127:
	ldr	x2, .LC130
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	cmp	w0, 1
	str	w0, [x19, 124]
	mov	w22, w0
	beq	.L92
.L100:
	ldr	x2, .LC131
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 328]
	b	.L91
	.p2align 3
.L121:
	ldr	x2, .LC102
	mov	w1, 1
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 104]
	ldr	x2, .LC103
	mov	w1, 7
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 96]
	ldr	w1, [x19, 92]
	mov	w2, 1
	strb	w2, [x19]
	orr	w0, w0, w1, lsl 7
	str	w0, [x19, 92]
	b	.L77
	.p2align 3
.L122:
	ldr	x1, .LC107
	mov	x0, x20
	bl	avs_ue_v
	str	w0, [x19, 112]
	b	.L78
	.p2align 3
.L92:
	ldr	x2, .LC132
	mov	w1, w0
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 128]
	ldr	x2, .LC133
	mov	w1, w22
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 132]
	cbz	w0, .L128
.L93:
	ldr	x2, .LC136
	mov	w1, 2
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 144]
	ldr	x2, .LC137
	mov	w1, 2
	mov	x0, x20
	bl	avs_f_v
	str	w0, [x19, 148]
	ldr	w1, [x19, 144]
	cmp	w1, 1
	beq	.L94
	ldr	w0, [x19, 128]
	cmp	w0, wzr
	ccmp	w1, 3, 0, eq
	beq	.L94
	cmp	w1, 2
	add	x22, x19, 176
	bne	.L100
.L99:
	ldr	x23, .LC139
	add	x24, x19, 200
	.p2align 2
.L98:
	mov	x1, x23
	mov	x0, x20
	bl	avs_se_v
	str	w0, [x22], 4
	cmp	x22, x24
	bne	.L98
	b	.L100
.L94:
	ldr	x24, .LC138
	add	x23, x19, 152
	add	x22, x19, 176
	.p2align 2
.L96:
	mov	x1, x24
	mov	x0, x20
	bl	avs_se_v
	str	w0, [x23], 4
	cmp	x23, x22
	bne	.L96
	ldr	w0, [x19, 144]
	cmp	w0, 2
	beq	.L99
	ldr	w1, [x19, 128]
	cmp	w1, wzr
	ccmp	w0, 3, 0, eq
	beq	.L99
	b	.L100
.L128:
	ldr	x1, .LC134
	mov	x0, x20
	bl	avs_se_v
	str	w0, [x19, 136]
	ldr	x1, .LC135
	mov	x0, x20
	bl	avs_se_v
	str	w0, [x19, 140]
	b	.L93
	.size	AVS_pb_picture_header, .-AVS_pb_picture_header
	.align	3
.LC98:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC100:
	.xword	.LC99
	.align	3
.LC101:
	.xword	.LC42
	.align	3
.LC102:
	.xword	.LC26
	.align	3
.LC103:
	.xword	.LC45
	.align	3
.LC105:
	.xword	.LC104
	.align	3
.LC106:
	.xword	.LC51
	.align	3
.LC107:
	.xword	.LC53
	.align	3
.LC108:
	.xword	.LC55
	.align	3
.LC109:
	.xword	.LC57
	.align	3
.LC111:
	.xword	.LC110
	.align	3
.LC112:
	.xword	.LC59
	.align	3
.LC113:
	.xword	.LC61
	.align	3
.LC114:
	.xword	.LC63
	.align	3
.LC115:
	.xword	.LC65
	.align	3
.LC117:
	.xword	.LC116
	.align	3
.LC119:
	.xword	.LC118
	.align	3
.LC121:
	.xword	.LC120
	.align	3
.LC123:
	.xword	.LC122
	.align	3
.LC124:
	.xword	.LC34
	.align	3
.LC125:
	.xword	.LC67
	.align	3
.LC126:
	.xword	.LC70
	.align	3
.LC127:
	.xword	.LC72
	.align	3
.LC128:
	.xword	.LC74
	.align	3
.LC129:
	.xword	.LC76
	.align	3
.LC130:
	.xword	.LC78
	.align	3
.LC131:
	.xword	.LC80
	.align	3
.LC132:
	.xword	.LC82
	.align	3
.LC133:
	.xword	.LC84
	.align	3
.LC134:
	.xword	.LC86
	.align	3
.LC135:
	.xword	.LC88
	.align	3
.LC136:
	.xword	.LC90
	.align	3
.LC137:
	.xword	.LC92
	.align	3
.LC138:
	.xword	.LC94
	.align	3
.LC139:
	.xword	.LC96
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_user_data
	.type	AVS_user_data, %function
AVS_user_data:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 12288
	mov	w22, w1
	add	x20, x21, 2648
	bl	_mcount
	ldr	x2, .LC141
	mov	w1, 32
	ldr	x0, [x20, 512]
	bl	avs_f_v
	ldr	w0, [x20, 124]
	orr	w0, w0, 1048576
	str	w0, [x20, 124]
	cbnz	w22, .L130
	ldr	x1, [x20, 816]
	cbz	x1, .L131
	ldr	w0, [x19, 4]
	bl	FreeUsdByDec
	str	xzr, [x20, 816]
.L131:
	ldr	w0, [x19, 4]
	bl	GetUsd
	str	x0, [x21, 3464]
	add	x1, x21, 2648
	cbz	x0, .L141
	ldr	x3, [x19, 16]
	mov	w2, 9
	add	x20, x19, 16384
	ldr	x3, [x3, 56]
	str	x3, [x0, 1056]
	ldr	x0, [x1, 816]
	str	w2, [x0, 1040]
	ldr	x0, [x1, 816]
	ldr	w2, [x1, 132]
	str	w2, [x0, 1044]
	ldr	x2, [x1, 816]
	ldr	w0, [x20, 452]
	sub	w0, w0, #4
	str	w0, [x2, 1052]
	ldr	x0, [x1, 816]
	ldr	w2, [x0, 1052]
	cmp	w2, 1024
	ble	.L134
	mov	w2, 1024
	str	w2, [x0, 1052]
	ldr	x0, [x1, 816]
	ldr	w2, [x0, 1052]
.L134:
	ldr	x22, .LC142
	sxtw	x2, w2
	ldr	x1, [x19, 16816]
	ldr	x3, [x22, 104]
	add	x1, x1, 4
	blr	x3
	ldrb	w0, [x20, 442]
	cbnz	w0, .L136
	ldr	x2, [x21, 3464]
	ldr	w0, [x20, 540]
	ldr	w3, [x2, 1052]
	add	w0, w3, w0
	str	w0, [x2, 1052]
	ldr	x3, [x21, 3464]
	ldr	w2, [x3, 1052]
	cmp	w2, 1024
	ble	.L137
	mov	w0, 1024
	str	w0, [x3, 1052]
	ldr	x3, [x21, 3464]
	ldr	w2, [x3, 1052]
.L137:
	ldr	w1, [x20, 452]
	sub	w2, w2, w1
	cmn	w2, #4
	beq	.L136
	uxtw	x1, w1
	ldr	x4, [x22, 104]
	sub	x0, x1, #4
	ldr	x1, [x19, 16904]
	add	w2, w2, 4
	add	x0, x3, x0
	blr	x4
.L136:
	ldr	x0, .LC143
	ldr	x4, [x0]
	cbz	x4, .L166
	ldr	x2, [x21, 3464]
	mov	w3, 1064
	ldr	w0, [x19, 4]
	mov	w1, 6
	blr	x4
.L166:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L130:
	ldr	x1, [x20, 824]
	cbz	x1, .L140
	ldr	w0, [x19, 4]
	bl	FreeUsdByDec
	str	xzr, [x20, 824]
.L140:
	ldr	w0, [x19, 4]
	bl	GetUsd
	str	x0, [x21, 3472]
	cbz	x0, .L141
	ldr	x3, [x19, 16]
	mov	w2, 10
	add	x20, x19, 16384
	ldr	x3, [x3, 56]
	str	x3, [x0, 1056]
	ldr	x0, [x21, 3472]
	str	w2, [x0, 1040]
	ldr	x0, [x21, 3472]
	ldr	w2, [x21, 2780]
	str	w2, [x0, 1044]
	ldr	x2, [x21, 3472]
	ldr	w0, [x20, 452]
	sub	w0, w0, #4
	str	w0, [x2, 1052]
	ldr	x0, [x21, 3472]
	ldr	w2, [x0, 1052]
	cmp	w2, 1024
	ble	.L142
	mov	w2, 1024
	str	w2, [x0, 1052]
	ldr	x0, [x21, 3472]
	ldr	w2, [x0, 1052]
.L142:
	ldr	x22, .LC142
	sxtw	x2, w2
	ldr	x1, [x19, 16816]
	ldr	x3, [x22, 104]
	add	x1, x1, 4
	blr	x3
	ldrb	w0, [x20, 442]
	cbnz	w0, .L144
	ldr	x2, [x21, 3472]
	ldr	w0, [x20, 540]
	ldr	w3, [x2, 1052]
	add	w0, w3, w0
	str	w0, [x2, 1052]
	ldr	x3, [x21, 3472]
	ldr	w2, [x3, 1052]
	cmp	w2, 1024
	ble	.L145
	mov	w0, 1024
	str	w0, [x3, 1052]
	ldr	x3, [x21, 3472]
	ldr	w2, [x3, 1052]
.L145:
	ldr	w1, [x20, 452]
	sub	w2, w2, w1
	cmn	w2, #4
	beq	.L144
	uxtw	x1, w1
	ldr	x4, [x22, 104]
	sub	x0, x1, #4
	ldr	x1, [x19, 16904]
	add	w2, w2, 4
	add	x0, x3, x0
	blr	x4
.L144:
	ldr	x0, .LC143
	ldr	x4, [x0]
	cbz	x4, .L166
	ldr	x2, [x21, 3472]
	mov	w3, 1064
	ldr	w0, [x19, 4]
	mov	w1, 6
	blr	x4
	b	.L166
	.p2align 3
.L141:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	AVS_user_data, .-AVS_user_data
	.align	3
.LC141:
	.xword	.LC140
	.align	3
.LC142:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC143:
	.xword	g_event_report
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_sequence_display_extension
	.type	AVS_sequence_display_extension, %function
AVS_sequence_display_extension:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x19, [x21, 512]
	mov	w1, 4
	ldr	x2, .LC145
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC147
	mov	w1, 3
	mov	x0, x19
	bl	avs_f_v
	strb	w0, [x20, 46]
	ldr	x2, .LC149
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	strb	w0, [x20, 47]
	ldr	x2, .LC151
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x20, 48]
	beq	.L170
.L169:
	ldr	x2, .LC159
	mov	w1, 14
	mov	x0, x19
	bl	avs_f_v
	str	w0, [x20, 64]
	ldr	x2, .LC160
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	strb	w0, [x20, 45]
	ldr	x2, .LC162
	mov	w1, 14
	mov	x0, x19
	bl	avs_f_v
	str	w0, [x20, 68]
	ldr	x2, .LC163
	mov	w1, 2
	mov	x0, x19
	bl	avs_f_v
	strb	w0, [x20, 44]
	mov	x0, x19
	bl	AVS_next_start_code
	ldr	w1, [x21, 124]
	mov	w0, 1
	orr	w1, w1, 131072
	str	w1, [x21, 124]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L170:
	ldr	x2, .LC153
	mov	w1, 8
	mov	x0, x19
	bl	avs_f_v
	str	w0, [x20, 52]
	ldr	x2, .LC155
	mov	w1, 8
	mov	x0, x19
	bl	avs_f_v
	str	w0, [x20, 56]
	ldr	x2, .LC157
	mov	w1, 8
	mov	x0, x19
	bl	avs_f_v
	str	w0, [x20, 60]
	b	.L169
	.size	AVS_sequence_display_extension, .-AVS_sequence_display_extension
	.align	3
.LC145:
	.xword	.LC144
	.align	3
.LC147:
	.xword	.LC146
	.align	3
.LC149:
	.xword	.LC148
	.align	3
.LC151:
	.xword	.LC150
	.align	3
.LC153:
	.xword	.LC152
	.align	3
.LC155:
	.xword	.LC154
	.align	3
.LC157:
	.xword	.LC156
	.align	3
.LC159:
	.xword	.LC158
	.align	3
.LC160:
	.xword	.LC26
	.align	3
.LC162:
	.xword	.LC161
	.align	3
.LC163:
	.xword	.LC34
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_copyright_extension
	.type	AVS_copyright_extension, %function
AVS_copyright_extension:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x19, [x20, 512]
	mov	w1, 4
	ldr	x2, .LC164
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC166
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC168
	mov	w1, 8
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC170
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC171
	mov	w1, 7
	mov	x0, x19
	bl	avs_f_v
	ldr	x21, .LC172
	mov	w1, 1
	mov	x0, x19
	mov	x2, x21
	bl	avs_f_v
	ldr	x2, .LC174
	mov	w1, 20
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x21
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC176
	mov	w1, 22
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x21
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC178
	mov	w1, 22
	mov	x0, x19
	bl	avs_f_v
	mov	x0, x19
	bl	AVS_next_start_code
	ldr	w1, [x20, 124]
	mov	w0, 1
	orr	w1, w1, 2097152
	str	w1, [x20, 124]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	AVS_copyright_extension, .-AVS_copyright_extension
	.align	3
.LC164:
	.xword	.LC144
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
.LC171:
	.xword	.LC34
	.align	3
.LC172:
	.xword	.LC26
	.align	3
.LC174:
	.xword	.LC173
	.align	3
.LC176:
	.xword	.LC175
	.align	3
.LC178:
	.xword	.LC177
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_camera_parameters_extension
	.type	AVS_camera_parameters_extension, %function
AVS_camera_parameters_extension:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	bl	_mcount
	ldr	x19, [x21, 512]
	mov	w1, 4
	ldr	x2, .LC179
	mov	x0, x19
	bl	avs_f_v
	ldr	x22, .LC180
	mov	w1, 1
	mov	x0, x19
	mov	x2, x22
	bl	avs_f_v
	ldr	x2, .LC182
	mov	w1, 7
	mov	x0, x19
	bl	avs_f_v
	ldr	x20, .LC183
	mov	w1, 1
	mov	x0, x19
	mov	x2, x20
	bl	avs_f_v
	ldr	x2, .LC185
	mov	w1, 22
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC187
	mov	w1, 22
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC189
	mov	w1, 22
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC191
	mov	w1, 22
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC193
	mov	w1, 16
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC195
	mov	w1, 16
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC197
	mov	w1, 16
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC199
	mov	w1, 16
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC201
	mov	w1, 16
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC203
	mov	w1, 16
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC205
	mov	w1, 22
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC207
	mov	w1, 22
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC209
	mov	w1, 22
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC211
	mov	w1, 22
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC213
	mov	w1, 22
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	x2, .LC215
	mov	w1, 22
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x20
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x22
	mov	w1, 32
	mov	x0, x19
	bl	avs_f_v
	mov	x0, x19
	bl	AVS_next_start_code
	ldr	w1, [x21, 124]
	mov	w0, 1
	orr	w1, w1, 4194304
	str	w1, [x21, 124]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	AVS_camera_parameters_extension, .-AVS_camera_parameters_extension
	.align	3
.LC179:
	.xword	.LC144
	.align	3
.LC180:
	.xword	.LC34
	.align	3
.LC182:
	.xword	.LC181
	.align	3
.LC183:
	.xword	.LC26
	.align	3
.LC185:
	.xword	.LC184
	.align	3
.LC187:
	.xword	.LC186
	.align	3
.LC189:
	.xword	.LC188
	.align	3
.LC191:
	.xword	.LC190
	.align	3
.LC193:
	.xword	.LC192
	.align	3
.LC195:
	.xword	.LC194
	.align	3
.LC197:
	.xword	.LC196
	.align	3
.LC199:
	.xword	.LC198
	.align	3
.LC201:
	.xword	.LC200
	.align	3
.LC203:
	.xword	.LC202
	.align	3
.LC205:
	.xword	.LC204
	.align	3
.LC207:
	.xword	.LC206
	.align	3
.LC209:
	.xword	.LC208
	.align	3
.LC211:
	.xword	.LC210
	.align	3
.LC213:
	.xword	.LC212
	.align	3
.LC215:
	.xword	.LC214
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_picture_display_extension
	.type	AVS_picture_display_extension, %function
AVS_picture_display_extension:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x26, x0
	mov	x0, x30
	mov	x22, x1
	bl	_mcount
	ldr	x19, [x26, 512]
	mov	w1, 4
	ldr	x2, .LC216
	mov	x0, x19
	bl	avs_f_v
	ldrb	w0, [x22, 15]
	cmp	w0, 1
	beq	.L190
	ldrb	w0, [x22, 77]
	cbnz	w0, .L191
.L175:
	mov	w0, 1
	str	w0, [x22, 4]
.L177:
	ldr	x25, .LC218
	mov	w20, 0
	ldr	x23, .LC219
	ldr	x24, .LC221
	.p2align 2
.L179:
	add	x21, x22, x20, sxtw 2
	mov	x2, x25
	mov	w1, 16
	mov	x0, x19
	add	w20, w20, 1
	bl	avs_f_v
	str	w0, [x21, 348]
	mov	w1, 1
	mov	x2, x23
	mov	x0, x19
	bl	avs_f_v
	mov	x2, x24
	mov	w1, 16
	mov	x0, x19
	bl	avs_f_v
	str	w0, [x21, 360]
	mov	x2, x23
	mov	w1, 1
	mov	x0, x19
	bl	avs_f_v
	ldr	w0, [x22, 4]
	cmp	w0, w20
	bgt	.L179
	mov	x0, x19
	bl	AVS_next_start_code
	ldr	w0, [x26, 124]
	orr	w0, w0, 524288
	str	w0, [x26, 124]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L191:
	ldrb	w0, [x22, 79]
	cmp	w0, 1
	beq	.L178
.L176:
	mov	w0, 2
	str	w0, [x22, 4]
	b	.L177
	.p2align 3
.L190:
	ldrb	w0, [x22, 79]
	cmp	w0, 1
	bne	.L175
	ldrb	w0, [x22, 78]
	cmp	w0, 1
	bne	.L176
	.p2align 2
.L178:
	mov	w0, 3
	str	w0, [x22, 4]
	b	.L177
	.size	AVS_picture_display_extension, .-AVS_picture_display_extension
	.align	3
.LC216:
	.xword	.LC144
	.align	3
.LC218:
	.xword	.LC217
	.align	3
.LC219:
	.xword	.LC26
	.align	3
.LC221:
	.xword	.LC220
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_extension
	.type	AVS_extension, %function
AVS_extension:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	ldr	x19, [x20, 512]
	mov	w1, 32
	ldr	x2, .LC223
	mov	x0, x19
	bl	avs_f_v
	mov	w1, 4
	mov	x0, x19
	bl	BsShow
	sub	w0, w0, #2
	cmp	w0, 9
	bls	.L200
.L193:
	mov	w1, 4
	mov	x0, x19
	bl	BsSkip
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L200:
	ldr	x1, .LC224
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx195
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx195:
	.section	.rodata
	.align	0
	.align	2
.L195:
	.byte	(.L194 - .Lrtx195) / 4
	.byte	(.L193 - .Lrtx195) / 4
	.byte	(.L196 - .Lrtx195) / 4
	.byte	(.L193 - .Lrtx195) / 4
	.byte	(.L193 - .Lrtx195) / 4
	.byte	(.L197 - .Lrtx195) / 4
	.byte	(.L193 - .Lrtx195) / 4
	.byte	(.L193 - .Lrtx195) / 4
	.byte	(.L193 - .Lrtx195) / 4
	.byte	(.L198 - .Lrtx195) / 4
	.text
	.p2align 3
.L198:
	mov	x1, x21
	mov	x0, x20
	bl	AVS_camera_parameters_extension
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L197:
	mov	x1, x21
	mov	x0, x20
	bl	AVS_picture_display_extension
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L196:
	mov	x1, x21
	mov	x0, x20
	bl	AVS_copyright_extension
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L194:
	mov	x1, x21
	mov	x0, x20
	bl	AVS_sequence_display_extension
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	AVS_extension, .-AVS_extension
	.align	3
.LC223:
	.xword	.LC222
	.align	3
.LC224:
	.xword	.L195
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_extension_and_userdata
	.type	AVS_extension_and_userdata, %function
AVS_extension_and_userdata:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x23, x0
	mov	x0, x30
	add	x21, x23, 12288
	mov	w24, w1
	add	x22, x21, 2648
	bl	_mcount
	mov	w20, 178
	add	x21, x21, 3536
	ldr	x19, [x22, 512]
	b	.L205
	.p2align 3
.L204:
	mov	w1, 32
	mov	x0, x19
	bl	BsShow
	uxtb	w0, w0
	cmp	w0, 178
	beq	.L209
.L205:
	mov	x0, x19
	bl	BsResidBits
	cmp	w0, 40
	mov	x0, x19
	ble	.L210
.L206:
	bl	AVS_next_start_code
	mov	w1, 32
	mov	x0, x19
	bl	BsShow
	uxtb	w0, w0
	cmp	w0, 181
	ccmp	w0, w20, 4, ne
	bne	.L207
	cmp	w0, 181
	bne	.L204
	mov	x1, x21
	mov	x0, x22
	bl	AVS_extension
	mov	w1, 32
	mov	x0, x19
	bl	BsShow
	uxtb	w0, w0
	cmp	w0, 178
	bne	.L205
	.p2align 2
.L209:
	mov	w1, w24
	mov	x0, x23
	bl	AVS_user_data
	mov	x0, x19
	bl	BsResidBits
	cmp	w0, 40
	mov	x0, x19
	bgt	.L206
.L210:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L207:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	AVS_extension_and_userdata, .-AVS_extension_and_userdata
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_AllocateVHB
	.type	AVS_AllocateVHB, %function
AVS_AllocateVHB:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w22, [x19, 15104]
	ldr	w21, [x19, 15100]
	cmp	w22, 31
	ccmp	w21, 31, 0, hi
	bls	.L244
	ldr	x0, [x19, 16]
	ldr	w20, [x0, 748]
	ldr	w1, [x0, 28]
	cmp	w1, 25
	cbnz	w20, .L214
	beq	.L245
.L233:
	mov	w20, 4
	mov	w24, 5
	mov	w23, w20
.L215:
	ldr	x0, .LC229
	mov	x2, 32
	mov	w1, 0
	ldr	x3, [x0, 96]
	add	x0, x29, 80
	blr	x3
	strb	wzr, [x29, 81]
	ldr	x0, [x19, 16]
	stp	w21, w22, [x29, 104]
	stp	w21, w22, [x29, 88]
	ldr	w0, [x0, 28]
	cmp	w0, 24
	beq	.L224
	strb	w23, [x29, 83]
	sxtb	w20, w20
	strb	w24, [x29, 85]
.L228:
	ldr	w0, [x19, 4]
	add	x1, x29, 80
	strb	w20, [x29, 86]
	strb	wzr, [x29, 82]
	bl	FSP_ConfigInstance
	mov	w20, w0
	cbnz	w0, .L225
	ldr	x1, [x19, 16]
	add	x3, x29, 76
	ldr	w0, [x19, 4]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	beq	.L246
	cbnz	w0, .L247
	mov	w20, 1
	str	w20, [x19, 8]
	b	.L236
	.p2align 3
.L244:
	ldr	x1, .LC226
	mov	w3, w22
	mov	w2, w21
	mov	w0, 0
	mov	w20, 0
	bl	dprint_vfmw
.L236:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L214:
	bne	.L233
	ldr	w4, [x0, 708]
	ldr	w5, [x0, 712]
	cmp	w21, w4
	bhi	.L221
	cmp	w22, w5
	bhi	.L221
.L222:
	ldr	w2, [x0, 736]
	mov	w20, 1
	ldr	w1, [x0, 732]
	add	w23, w1, 2
	cbnz	w2, .L248
	ldrsb	w24, [x0, 756]
	b	.L215
	.p2align 3
.L225:
	ldr	x1, .LC233
	mov	w0, 0
	mov	w20, 0
	bl	dprint_vfmw
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L245:
	ldr	w4, [x0, 708]
	ldr	w5, [x0, 712]
	cmp	w21, w4
	bhi	.L217
	cmp	w5, w22
	bcc	.L217
	ldr	w20, [x19, 8]
	cmp	w20, 1
	bne	.L222
	b	.L236
	.p2align 3
.L224:
	mov	w0, 1
	strb	w24, [x29, 85]
	strb	w0, [x29, 83]
	mov	w20, 1
	b	.L228
	.p2align 3
.L221:
	ldr	x1, .LC228
	mov	w3, w22
	mov	w2, w21
	mov	w0, 0
	mov	w20, 0
	bl	dprint_vfmw
	b	.L236
	.p2align 3
.L247:
	ldr	x1, .LC231
	mov	w0, 0
	bl	dprint_vfmw
	b	.L236
	.p2align 3
.L248:
	add	w20, w1, w20
	mov	w1, 17
	cmp	w20, w1
	ldrsb	w24, [x0, 756]
	csel	w20, w20, w1, le
	b	.L215
	.p2align 3
.L217:
	ldr	x1, .LC228
	mov	w3, w22
	mov	w2, w21
	mov	w0, 0
	bl	dprint_vfmw
	b	.L236
	.p2align 3
.L246:
	add	x0, x19, 12288
	mov	w20, 3
	add	x0, x0, 3912
	bl	ResetVoQueue
	b	.L236
	.size	AVS_AllocateVHB, .-AVS_AllocateVHB
	.align	3
.LC226:
	.xword	.LC225
	.align	3
.LC228:
	.xword	.LC227
	.align	3
.LC229:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC231:
	.xword	.LC230
	.align	3
.LC233:
	.xword	.LC232
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_StartDecode
	.type	AVS_StartDecode, %function
AVS_StartDecode:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x22, x21, 16384
	add	x19, x21, 32
	bl	_mcount
	add	x20, x22, 608
	ldr	w0, [x22, 608]
	strb	w0, [x21, 32]
	ldr	w0, [x20, 4]
	strb	w0, [x19, 1]
	ldr	w0, [x20, 8]
	strb	w0, [x19, 2]
	ldr	w0, [x20, 12]
	strb	w0, [x19, 3]
	ldr	w0, [x20, 16]
	strb	w0, [x19, 4]
	ldr	w0, [x20, 28]
	str	w0, [x19, 8]
	ldr	w0, [x20, 32]
	str	w0, [x19, 68]
	ldr	w0, [x20, 52]
	str	w0, [x19, 12]
	ldr	w0, [x20, 180]
	str	w0, [x19, 80]
	ldr	w0, [x20, 192]
	str	w0, [x19, 84]
	ldr	w0, [x20, 184]
	str	w0, [x19, 88]
	ldr	w0, [x20, 196]
	str	w0, [x19, 92]
	ldr	w0, [x20, 188]
	str	w0, [x19, 96]
	ldr	w0, [x20, 200]
	str	w0, [x19, 100]
	ldr	w0, [x20, 60]
	strh	w0, [x19, 64]
	ldr	w0, [x20, 56]
	strh	w0, [x19, 66]
	ldr	w0, [x20, 64]
	strb	w0, [x19, 39]
	ldr	w0, [x20, 68]
	strb	w0, [x19, 40]
	ldr	w0, [x20, 104]
	strb	w0, [x19, 16]
	ldr	w0, [x20, 108]
	strb	w0, [x19, 17]
	ldr	w0, [x20, 72]
	strb	w0, [x19, 18]
	ldr	w0, [x20, 76]
	strb	w0, [x19, 19]
	ldr	w0, [x20, 84]
	strb	w0, [x19, 20]
	ldr	w0, [x20, 88]
	strb	w0, [x19, 21]
	ldr	w0, [x20, 92]
	strb	w0, [x19, 22]
	ldr	w0, [x20, 96]
	strb	w0, [x19, 23]
	ldr	w0, [x20, 100]
	strb	w0, [x19, 24]
	ldr	w0, [x20, 112]
	strb	w0, [x19, 25]
	ldr	w0, [x20, 116]
	strb	w0, [x19, 26]
	ldr	w0, [x20, 120]
	strb	w0, [x19, 27]
	ldr	w0, [x20, 124]
	strb	w0, [x19, 28]
	ldr	w0, [x20, 128]
	strb	w0, [x19, 29]
	ldr	w0, [x20, 172]
	strb	w0, [x19, 30]
	ldr	w0, [x20, 80]
	strb	w0, [x19, 31]
	ldr	w0, [x20, 136]
	strb	w0, [x19, 32]
	ldr	w0, [x20, 132]
	strb	w0, [x19, 33]
	ldr	w0, [x20, 152]
	strb	w0, [x19, 34]
	ldr	w0, [x20, 148]
	strb	w0, [x19, 35]
	ldr	w0, [x20, 144]
	strb	w0, [x19, 36]
	ldr	w0, [x20, 140]
	strb	w0, [x19, 37]
	ldr	w0, [x20, 176]
	strb	w0, [x19, 38]
	ldr	w0, [x20, 156]
	str	w0, [x19, 14444]
	ldr	w0, [x20, 160]
	str	w0, [x19, 14448]
	ldr	w0, [x20, 164]
	str	w0, [x19, 14452]
	ldr	w0, [x20, 232]
	str	w0, [x19, 14460]
	ldr	w0, [x20, 244]
	str	w0, [x19, 14464]
	ldr	x1, [x21, 15016]
	ldr	w0, [x21, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetLogicFs
	cbz	x0, .L264
	ldr	x1, [x0, 648]
	add	x2, x21, 12288
	add	x5, x2, 2348
	add	x3, x22, 988
	add	x4, x2, 2604
	ldr	w1, [x1, 32]
	str	w1, [x19, 14860]
	ldr	x1, [x0, 648]
	ldr	w1, [x1, 76]
	str	w1, [x19, 14868]
	ldr	x1, [x0, 648]
	ldr	w1, [x1, 60]
	str	w1, [x19, 14872]
	mov	x1, x5
	ldr	x0, [x0, 648]
	ldr	w0, [x0, 64]
	str	w0, [x19, 14876]
	ldr	w0, [x20, 52]
	str	w0, [x19, 14456]
	ldr	w0, [x20, 168]
	str	w0, [x19, 14468]
	ldr	w0, [x20, 376]
	str	w0, [x19, 14472]
	ldr	w0, [x20, 204]
	strb	w0, [x19, 41]
	ldr	w0, [x20, 208]
	strb	w0, [x19, 42]
	ldr	w0, [x20, 216]
	strb	w0, [x19, 43]
	ldr	w0, [x20, 212]
	strb	w0, [x19, 44]
	ldr	w0, [x20, 220]
	strb	w0, [x19, 45]
	ldr	w0, [x20, 224]
	strb	w0, [x19, 46]
	ldr	w0, [x20, 228]
	strb	w0, [x19, 47]
	.p2align 2
.L252:
	ldrsh	w0, [x3], 2
	str	w0, [x1], 4
	cmp	x1, x4
	bne	.L252
	ldr	w0, [x22, 644]
	str	w0, [x19, 14880]
	add	x0, x2, 2220
	ldr	w1, [x22, 648]
	str	w1, [x19, 14884]
	ldr	w1, [x22, 652]
	str	w1, [x19, 14888]
	ldr	w1, [x22, 656]
	str	w1, [x19, 14892]
	.p2align 2
.L253:
	ldr	w1, [x0, 2732]
	str	w1, [x0], 4
	cmp	x0, x5
	bne	.L253
	ldr	w0, [x21, 15744]
	ldr	x1, [x21, 16]
	add	w0, w0, 1
	str	x19, [x21, 24]
	str	w0, [x21, 15744]
	ldr	w0, [x1, 904]
	cmp	w0, 1
	beq	.L265
	cmp	w0, 2
	beq	.L266
.L255:
	ldr	w2, [x1, 632]
	mov	w0, 1
	str	w2, [x19, 14896]
	ldr	w1, [x1, 672]
	str	w1, [x19, 14900]
.L251:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L265:
	cbz	x19, .L255
	ldr	x3, [x21, 15016]
	ldrb	w0, [x3, 1]
	cmp	w0, 2
	bne	.L255
	str	xzr, [x21, 24]
	mov	w2, 0
	ldr	w0, [x21, 4]
	ldr	w1, [x3, 4]
	bl	FSP_SetDisplay
	mov	w0, 0
	b	.L251
	.p2align 3
.L266:
	ldr	w0, [x1, 908]
	cmp	w0, wzr
	ccmp	x19, xzr, 4, gt
	beq	.L255
	ldr	x3, [x21, 15016]
	ldrb	w0, [x3, 1]
	cmp	w0, 2
	bne	.L255
	str	xzr, [x21, 24]
	mov	w2, 0
	ldr	w0, [x21, 4]
	ldr	w1, [x3, 4]
	bl	FSP_SetDisplay
	ldr	x2, [x21, 16]
	mov	w0, 0
	ldr	w1, [x2, 908]
	sub	w1, w1, #1
	str	w1, [x2, 908]
	b	.L251
.L264:
	ldr	x1, .LC237
	mov	w2, 2200
	bl	dprint_vfmw
	mov	w0, 0
	b	.L251
	.size	AVS_StartDecode, .-AVS_StartDecode
	.align	3
.LC237:
	.xword	.LC236
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_SetImgFormat
	.type	AVS_SetImgFormat, %function
AVS_SetImgFormat:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x25, x0
	mov	x0, x30
	add	x21, x25, 12288
	bl	_mcount
	ldrb	w1, [x21, 2803]
	ldrb	w23, [x21, 2834]
	cmp	w1, 1
	ldr	w22, [x21, 3132]
	beq	.L277
	ldrb	w0, [x21, 2864]
	cmp	w0, 1
	cset	w20, ne
.L268:
	ldrb	w0, [x21, 2801]
	mov	w26, 0
	ldrb	w24, [x21, 2866]
	sub	w0, w0, #1
	uxtb	w0, w0
	cmp	w0, 3
	bhi	.L269
	ldr	x1, .LC238
	uxtb	x0, w0
	ldr	w26, [x1, x0, lsl 2]
.L269:
	ldr	x1, [x21, 2728]
	ldr	w0, [x25, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L282
	and	w22, w22, 3
	ubfiz	w24, w24, 12, 2
	ubfiz	w23, w23, 5, 2
	orr	w0, w22, 3072
	ubfiz	w1, w26, 14, 3
	orr	w23, w24, w23
	orr	w20, w1, w20, lsl 8
	orr	w23, w23, w0
	orr	w0, w23, w20
	str	w0, [x19, 108]
	ldr	w1, [x25, 15052]
	cmp	w1, 1920
	ble	.L283
	and	w0, w0, -131073
	str	w0, [x19, 108]
	mov	w1, 0
	ldr	x2, [x25, 16]
	ldr	w2, [x2, 632]
	cmp	w2, 1
	beq	.L284
.L274:
	orr	w0, w0, w1
	str	w0, [x19, 108]
	mov	w1, w26
	mov	x0, x19
	ldr	w2, [x21, 3448]
	str	w2, [x19, 160]
	stp	w22, w22, [x19, 100]
	bl	SetAspectRatio
	ldr	w1, [x21, 2812]
	str	w1, [x19, 112]
	ldr	w0, [x21, 2816]
	str	w0, [x19, 116]
	ldr	x1, [x25, 16]
	ldr	w1, [x1, 28]
	cmp	w1, 25
	beq	.L285
.L275:
	mov	w0, 6
	str	w0, [x19, 376]
	mov	w0, 4
	mov	w1, 8
	ldrb	w3, [x21, 2865]
	mov	w2, 1
	str	w1, [x19, 380]
	cmp	w3, 1
	strb	w2, [x19, 371]
	csel	w0, w0, wzr, ne
	strb	w2, [x19, 369]
	strb	w0, [x19, 364]
	str	w1, [x19, 384]
	str	w1, [x19, 388]
.L267:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L283:
	orr	w0, w0, 131072
	str	w0, [x19, 108]
	mov	w1, 0
	ldr	x2, [x25, 16]
	ldr	w2, [x2, 632]
	cmp	w2, 1
	bne	.L274
.L284:
	and	w2, w0, 131072
	mov	w1, 262144
	cmp	w2, wzr
	csel	w1, wzr, w1, ne
	b	.L274
	.p2align 3
.L277:
	mov	w20, 0
	b	.L268
	.p2align 3
.L285:
	ldr	w3, [x19, 140]
	add	w0, w0, 15
	lsr	w2, w0, 4
	ldr	w0, [x19, 44]
	lsl	w1, w3, 4
	ldr	x4, [x19, 504]
	add	w5, w3, w0
	str	w0, [x19, 28]
	mul	w1, w2, w1
	str	w0, [x19, 60]
	add	w0, w1, w0
	str	w5, [x19, 36]
	add	x1, x4, x1, sxtw
	add	w3, w3, w0
	str	x1, [x19, 512]
	str	x4, [x19, 520]
	str	w0, [x19, 32]
	str	w3, [x19, 40]
	str	w0, [x19, 48]
	bl	MEM_Phy2Vir
	str	x0, [x19, 528]
	ldr	w1, [x19, 32]
	str	w1, [x19, 64]
	b	.L275
	.p2align 3
.L282:
	ldr	x1, .LC240
	mov	w2, 2324
	mov	w0, 0
	bl	dprint_vfmw
	b	.L267
	.size	AVS_SetImgFormat, .-AVS_SetImgFormat
	.align	3
.LC238:
	.xword	.LANCHOR0
	.align	3
.LC240:
	.xword	.LC239
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_GetImageBuffer
	.type	AVS_GetImageBuffer, %function
AVS_GetImageBuffer:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	add	x22, x20, 12288
	bl	_mcount
	ldr	w0, [x20, 16168]
	cmp	w0, 2
	beq	.L287
	ldr	w19, [x22, 3176]
	add	x2, x22, 2648
	add	x21, x22, 2648
	add	w1, w19, 1
	sxtw	x19, w19
	asr	w3, w1, 31
	lsl	x0, x19, 2
	lsr	w3, w3, 30
	sub	x19, x0, x19
	add	w1, w1, w3
	and	w1, w1, 3
	add	x19, x2, x19, lsl 2
	sub	w1, w1, w3
	add	x19, x19, 20
	str	w1, [x22, 3176]
	str	x19, [x22, 2728]
	mov	w1, 1
	ldr	w0, [x20, 4]
	bl	FSP_NewLogicFs
	str	w0, [x19, 4]
	ldr	x0, [x21, 80]
	ldr	w1, [x0, 4]
	tbnz	w1, #31, .L306
.L289:
	ldr	w0, [x20, 4]
	bl	FSP_GetLogicFs
	mov	x19, x0
	cbz	x0, .L307
	ldr	x2, [x21, 80]
	mov	w0, 18
	ldr	x1, .LC245
	ldr	w2, [x2, 4]
	bl	dprint_vfmw
	ldr	x0, [x19, 640]
	cbz	x0, .L293
	ldr	x1, [x19, 648]
	cbz	x1, .L293
	ldr	x2, [x19, 656]
	cbz	x2, .L293
	ldr	w3, [x1, 16]
	ldr	w4, [x2, 16]
	ldr	x1, .LC247
	ldr	w2, [x0, 16]
	mov	w0, 18
	bl	dprint_vfmw
.L293:
	mov	w0, 1
	str	wzr, [x19, 168]
.L291:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L287:
	add	x19, x22, 2716
	str	x19, [x22, 2728]
	mov	w1, 1
	add	x21, x22, 2648
	ldr	w0, [x20, 4]
	bl	FSP_NewLogicFs
	str	w0, [x19, 4]
	ldr	x0, [x21, 80]
	ldr	w1, [x0, 4]
	tbz	w1, #31, .L289
.L306:
	ldr	x1, .LC242
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, [x21, 88]
	ldr	w0, [x20, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L290
	ldr	x1, [x21, 80]
	mov	w2, 1
	ldr	w0, [x20, 4]
	add	x23, x22, 2656
	ldr	w1, [x1, 4]
	bl	FSP_ClearLogicFs
	ldr	x1, [x21, 88]
	mov	w2, 1
	ldr	w0, [x20, 4]
	ldr	w1, [x1, 4]
	bl	FSP_ClearLogicFs
	ldr	x1, [x21, 96]
	mov	w2, 1
	ldr	w0, [x20, 4]
	ldr	w1, [x1, 4]
	bl	FSP_ClearLogicFs
	ldr	x1, [x21, 104]
	mov	w2, 1
	ldr	w0, [x20, 4]
	ldr	w1, [x1, 4]
	bl	FSP_ClearLogicFs
	ldr	w0, [x20, 4]
	ldr	x1, [x19, 560]
	bl	FreeUsdByDec
	ldr	w0, [x20, 4]
	ldr	x1, [x19, 568]
	bl	FreeUsdByDec
	ldr	w0, [x20, 4]
	ldr	x1, [x19, 576]
	bl	FreeUsdByDec
	ldr	w0, [x20, 4]
	ldr	x1, [x19, 584]
	bl	FreeUsdByDec
	add	x0, x19, 512
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	stp	x23, x23, [x21, 80]
	stp	x23, x23, [x21, 96]
	ldr	w0, [x20, 4]
	bl	FSP_GetNullLogicFs
	str	w0, [x23, 4]
.L290:
	str	wzr, [x22, 3444]
	add	x1, x22, 3912
	ldr	w0, [x20, 4]
	bl	FSP_ClearNotInVoQueue
	ldr	x23, [sp, 48]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L307:
	ldr	x1, .LC243
	mov	w2, 2441
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L291
	.size	AVS_GetImageBuffer, .-AVS_GetImageBuffer
	.align	3
.LC242:
	.xword	.LC241
	.align	3
.LC243:
	.xword	.LC236
	.align	3
.LC245:
	.xword	.LC244
	.align	3
.LC247:
	.xword	.LC246
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_OutputOneFrame
	.type	AVS_OutputOneFrame, %function
AVS_OutputOneFrame:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 4]
	ldr	w1, [x20, 4]
	bl	FSP_GetFsImagePtr
	mov	x21, x0
	cbz	x0, .L314
	ldr	w0, [x19, 4]
	mov	w2, 1
	ldr	w1, [x20, 4]
	bl	FSP_SetDisplay
	add	x3, x19, 12288
	ldr	w0, [x19, 4]
	mov	x4, x21
	add	x3, x3, 3912
	mov	x2, x19
	mov	w1, 6
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L310
	ldr	w0, [x19, 4]
	mov	w2, 0
	ldr	w1, [x20, 4]
	bl	FSP_SetDisplay
	ldr	x1, .LC251
	mov	w0, 1
	ldr	w2, [x20, 4]
	bl	dprint_vfmw
	mov	w0, 0
.L310:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L314:
	ldr	x1, .LC249
	mov	w2, 2482
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	AVS_OutputOneFrame, .-AVS_OutputOneFrame
	.align	3
.LC249:
	.xword	.LC248
	.align	3
.LC251:
	.xword	.LC250
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_UpdateTableIdx2Dist
	.type	AVS_UpdateTableIdx2Dist, %function
AVS_UpdateTableIdx2Dist:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w0, [x19, 217]
	cmp	w0, 1
	ldr	w0, [x19, 484]
	beq	.L327
	cbnz	w0, .L321
	ldr	x0, [x19, 88]
	ldr	w0, [x0, 8]
	lsl	w0, w0, 1
	str	w0, [x19, 568]
.L320:
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L321:
	cmp	w0, 1
	bne	.L322
	ldr	x0, [x19, 96]
	add	x1, x19, 8
	cmp	x0, x1
	beq	.L328
	ldr	w0, [x0, 8]
	lsl	w0, w0, 1
	str	w0, [x19, 564]
.L324:
	ldp	x2, x1, [x19, 80]
	add	w0, w0, 1
	str	w0, [x19, 560]
	ldr	w1, [x1, 8]
	str	w0, [x19, 580]
	lsl	w0, w1, 1
	str	w0, [x19, 556]
	add	w1, w0, 1
	str	w0, [x19, 576]
	str	w1, [x19, 552]
	str	w1, [x19, 572]
	ldr	w0, [x2, 8]
	lsl	w0, w0, 1
	str	w0, [x19, 568]
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L327:
	cmp	w0, 1
	beq	.L329
	cmp	w0, 2
	bne	.L320
	ldr	x0, [x19, 96]
	add	x1, x19, 8
	cmp	x0, x1
	beq	.L319
	ldr	w0, [x0, 8]
	lsl	w0, w0, 1
	str	w0, [x19, 552]
.L319:
	ldr	x0, [x19, 88]
	ldr	w0, [x0, 8]
	lsl	w0, w0, 1
	str	w0, [x19, 560]
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L322:
	cmp	w0, 2
	bne	.L320
	ldr	x0, [x19, 96]
	add	x1, x19, 8
	cmp	x0, x1
	beq	.L330
	ldr	w1, [x0, 8]
	lsl	w1, w1, 1
	str	w1, [x19, 556]
.L326:
	ldr	x0, [x19, 88]
	add	w2, w1, 1
	str	w2, [x19, 552]
	ldr	w0, [x0, 8]
	str	w1, [x19, 572]
	lsl	w0, w0, 1
	str	w2, [x19, 568]
	str	w0, [x19, 560]
	add	w1, w0, 1
	str	w0, [x19, 576]
	mov	w0, 1
	str	w1, [x19, 564]
	str	w1, [x19, 580]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L329:
	ldp	x0, x1, [x19, 88]
	add	x2, x19, 8
	cmp	x1, x2
	ldr	w0, [x0, 8]
	lsl	w0, w0, 1
	str	w0, [x19, 552]
	beq	.L320
	ldr	w0, [x1, 8]
	lsl	w0, w0, 1
	str	w0, [x19, 556]
	b	.L320
	.p2align 3
.L328:
	ldr	w0, [x19, 564]
	b	.L324
	.p2align 3
.L330:
	ldr	w1, [x19, 556]
	b	.L326
	.size	AVS_UpdateTableIdx2Dist, .-AVS_UpdateTableIdx2Dist
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_UpdateTableMvEScale
	.type	AVS_UpdateTableMvEScale, %function
AVS_UpdateTableMvEScale:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w0, [x19, 217]
	cmp	w0, 1
	beq	.L391
	ldr	w5, [x19, 484]
	cbz	w5, .L392
	cmp	w5, 1
	bne	.L344
	ldr	x12, [x19, 80]
	mov	w9, 0
	mov	w7, 4
	mov	w11, 0
	mov	w10, 512
.L349:
	ldr	w6, [x12, 8]
	lsl	w5, w9, 2
	sub	w5, w5, w9
	mov	w8, w9
	add	w6, w11, w6, lsl 1
.L345:
	add	x0, x19, x8, sxtw 2
	mov	w1, w9
	ldr	w4, [x0, 552]
	sub	w4, w6, w4
	and	w4, w4, 511
.L347:
	add	x0, x19, x1, sxtw 2
	add	w3, w5, w1
	mov	w2, 1
	add	x3, x19, x3, sxtw 2
	add	w1, w1, w2
	ldr	w0, [x0, 552]
	sub	w0, w6, w0
	ands	w0, w0, 511
	beq	.L346
	sdiv	w2, w10, w0
.L346:
	mul	w2, w4, w2
	cmp	w1, w7
	str	w2, [x3, 584]
	bne	.L347
	add	w8, w8, 1
	add	w5, w5, 4
	cmp	w8, w7
	bne	.L345
	add	w11, w11, 1
	add	w7, w7, 4
	cmp	w11, 2
	add	w9, w9, 4
	bne	.L349
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L392:
	mov	w0, 512
	str	w0, [x19, 648]
.L336:
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L391:
	ldr	x1, [x19, 80]
	ldr	w2, [x19, 484]
	ldr	w1, [x1, 8]
	cmp	w2, 1
	lsl	w1, w1, 1
	beq	.L393
	cmp	w2, 2
	bne	.L336
	ldr	w2, [x19, 552]
	sub	w2, w1, w2
	ands	w2, w2, 511
	beq	.L341
	mov	w0, 512
	sdiv	w0, w0, w2
.L341:
	ldr	w3, [x19, 560]
	mul	w0, w2, w0
	sub	w1, w3, w1
	str	w0, [x19, 584]
	ands	w1, w1, 511
	mov	w0, 1
	beq	.L342
	mov	w0, 512
	sdiv	w0, w0, w1
.L342:
	mul	w1, w1, w0
	str	w1, [x19, 600]
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L344:
	cmp	w5, 2
	bne	.L336
	ldr	x9, [x19, 80]
	add	x4, x19, 564
	mov	w7, 9
	mov	w6, 0
	mov	w8, 512
.L359:
	ldr	w1, [x9, 8]
	lsl	w10, w6, 2
	lsl	w3, w6, 3
	add	w1, w6, w1, lsl 1
.L350:
	add	x11, x19, x10, sxtw 2
	ldr	w2, [x4, -12]
	mov	w12, 1
	add	x13, x19, x3, sxtw 2
	sub	w2, w1, w2
	ldr	w0, [x11, 552]
	ands	w2, w2, 511
	mov	w11, w12
	sub	w0, w1, w0
	and	w0, w0, 511
	beq	.L352
	sdiv	w11, w8, w2
.L352:
	mul	w11, w11, w0
	str	w11, [x13, 584]
	ldr	w2, [x4, -8]
	sub	w2, w1, w2
	ands	w2, w2, 511
	beq	.L353
	sdiv	w12, w8, w2
.L353:
	add	w2, w3, 1
	add	w10, w10, 1
	mul	w0, w0, w12
	add	x2, x19, x2, sxtw 2
	cmp	w5, w10
	add	w3, w3, 2
	str	w0, [x2, 584]
	bne	.L350
	sub	w10, w7, #4
	mov	w12, w5
.L351:
	add	x11, x19, x12, sxtw 2
	ldr	w2, [x4, -4]
	sub	w3, w10, #1
	mov	w13, 1
	sub	w2, w2, w1
	ldr	w0, [x11, 552]
	ands	w2, w2, 511
	add	x3, x19, x3, sxtw 2
	mov	w11, w13
	sub	w0, w0, w1
	and	w0, w0, 511
	beq	.L356
	sdiv	w11, w8, w2
.L356:
	mul	w11, w11, w0
	str	w11, [x3, 584]
	ldr	w2, [x4]
	sub	w2, w2, w1
	ands	w2, w2, 511
	beq	.L357
	sdiv	w13, w8, w2
.L357:
	add	x2, x19, x10, sxtw 2
	add	w10, w10, 2
	mul	w0, w0, w13
	cmp	w10, w7
	add	w12, w12, 1
	str	w0, [x2, 584]
	bne	.L351
	add	w6, w6, 1
	add	w7, w7, 8
	cmp	w6, 2
	add	x4, x4, 16
	add	w5, w5, 4
	bne	.L359
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L393:
	add	x5, x19, 552
	add	x3, x19, 588
	add	x7, x19, 620
	mov	w8, 512
.L335:
	ldr	w2, [x19, 552]
	mov	w6, 1
	ldr	w0, [x5]
	mov	w4, w6
	sub	w2, w1, w2
	ands	w2, w2, 511
	sub	w0, w1, w0
	and	w0, w0, 511
	beq	.L337
	sdiv	w4, w8, w2
.L337:
	mul	w4, w4, w0
	str	w4, [x3, -4]
	ldr	w2, [x19, 556]
	sub	w2, w1, w2
	ands	w2, w2, 511
	beq	.L338
	sdiv	w6, w8, w2
.L338:
	mul	w0, w0, w6
	str	w0, [x3], 16
	add	x5, x5, 4
	cmp	x7, x3
	bne	.L335
	b	.L336
	.size	AVS_UpdateTableMvEScale, .-AVS_UpdateTableMvEScale
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_UpdateTableDirectScale
	.type	AVS_UpdateTableDirectScale, %function
AVS_UpdateTableDirectScale:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w0, [x19, 217]
	cmp	w0, 1
	beq	.L417
	ldr	x7, [x19, 88]
	add	x5, x19, 8
	add	x3, x19, 712
	mov	w1, 1
	mov	w2, 0
	mov	w9, 16384
	ldr	w6, [x7, 8]
	mov	w8, 32
	lsl	w6, w6, 1
.L401:
	sub	w0, w1, #1
	and	w4, w1, 1
	add	w0, w0, w0, lsr 31
	asr	w0, w0, 1
	add	w0, w0, 1
	add	x0, x19, x0, sxtw 3
	ldr	x0, [x0, 88]
	cmp	x0, x5
	beq	.L399
	ldr	w2, [x0, 8]
	add	w2, w4, w2, lsl 1
	sub	w0, w6, w2
	ands	w0, w0, 511
	bne	.L400
	str	w8, [x3]
.L399:
	add	w1, w1, 1
	add	x3, x3, 4
	cmp	w1, 5
	bne	.L401
	ldr	w9, [x19, 840]
	ldr	w8, [x7, 8]
	lsl	w8, w8, 1
	cbz	w9, .L418
	add	x6, x19, 728
	mov	w3, 1
	mov	w4, 0
	mov	w10, 32
	mov	w11, 16384
	.p2align 2
.L411:
	cbz	w9, .L404
	add	w0, w4, w4, lsr 31
	asr	w0, w0, 1
	add	w0, w0, 1
	add	x0, x19, x0, sxtw 3
	ldr	x0, [x0, 88]
	cmp	x0, x5
	beq	.L405
	ldr	w2, [x0, 8]
	and	w0, w3, 1
	add	w2, w0, w2, lsl 1
	sub	w1, w8, w2
	add	w1, w1, 512
	asr	w0, w1, 31
	lsr	w7, w0, 23
	add	w1, w1, w7
	and	w0, w1, 511
	subs	w0, w0, w7
	bne	.L406
	str	w10, [x6, -16]
.L407:
	str	w10, [x6]
.L410:
	add	w3, w3, 1
	add	w4, w4, 1
	cmp	w3, 5
	add	x6, x6, 4
	bne	.L411
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L404:
	add	w0, w3, w3, lsr 31
	asr	w0, w0, 1
	add	x0, x19, x0, sxtw 3
	ldr	x0, [x0, 88]
	cmp	x0, x5
	beq	.L405
	ldr	w2, [x0, 8]
	and	w0, w4, 1
	add	w2, w0, w2, lsl 1
.L405:
	sub	w1, w8, w2
	add	w1, w1, 512
	asr	w0, w1, 31
	lsr	w7, w0, 23
	add	w1, w1, w7
	and	w0, w1, 511
	subs	w0, w0, w7
	beq	.L407
	sdiv	w0, w11, w0
	str	w0, [x6]
	b	.L410
	.p2align 3
.L400:
	sdiv	w0, w9, w0
	str	w0, [x3]
	b	.L399
	.p2align 3
.L406:
	sdiv	w0, w11, w0
	str	w0, [x6, -16]
	str	w0, [x6]
	b	.L410
	.p2align 3
.L418:
	add	w8, w8, 1
	add	x6, x19, 728
	mov	w3, 1
	mov	w4, 0
	mov	w10, 32
	mov	w11, 16384
	b	.L411
	.p2align 3
.L417:
	ldr	x0, [x19, 88]
	add	x4, x19, 8
	add	x2, x19, 96
	add	x1, x19, 712
	mov	w6, 16384
	mov	w5, 32
	ldr	w3, [x0, 8]
	add	x19, x19, 720
	lsl	w3, w3, 1
.L398:
	ldr	x0, [x2]
	cmp	x0, x4
	beq	.L396
	ldr	w0, [x0, 8]
	sub	w0, w3, w0, lsl 1
	ands	w0, w0, 511
	bne	.L397
	str	w5, [x1]
.L396:
	add	x1, x1, 4
	add	x2, x2, 8
	cmp	x19, x1
	bne	.L398
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L397:
	sdiv	w0, w6, w0
	str	w0, [x1]
	b	.L396
	.size	AVS_UpdateTableDirectScale, .-AVS_UpdateTableDirectScale
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_UpdateTableSymScale
	.type	AVS_UpdateTableSymScale, %function
AVS_UpdateTableSymScale:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, [x19, 96]
	add	x0, x19, 8
	cmp	x1, x0
	beq	.L426
	ldrb	w0, [x19, 217]
	cmp	w0, 1
	beq	.L421
	ldr	x12, [x19, 80]
	mov	w6, 0
	mov	w11, 512
.L424:
	ldr	w5, [x12, 8]
	lsl	w8, w6, 2
	lsl	w10, w6, 1
	mov	x2, 0
	add	w9, w8, 3
	add	w5, w6, w5, lsl 1
.L423:
	add	w3, w8, w2
	sub	w4, w9, w2
	add	w1, w2, w10
	mov	w7, 1
	add	x3, x19, x3, sxtw 2
	add	x4, x19, x4, sxtw 2
	add	x2, x2, 1
	add	x1, x19, x1, sxtw 2
	ldr	w3, [x3, 552]
	ldr	w0, [x4, 552]
	sub	w3, w5, w3
	ands	w3, w3, 511
	sub	w0, w0, w5
	and	w0, w0, 511
	beq	.L422
	sdiv	w7, w11, w3
.L422:
	mul	w0, w0, w7
	cmp	x2, 2
	str	w0, [x1, 744]
	bne	.L423
	add	w6, w6, 1
	cmp	w6, 2
	bne	.L424
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L421:
	ldp	x4, x2, [x19, 80]
	ldr	w3, [x1, 8]
	ldr	w1, [x4, 8]
	ldr	w2, [x2, 8]
	lsl	w4, w1, 1
	lsl	w1, w2, 1
	sub	w2, w4, w3, lsl 1
	ands	w2, w2, 511
	sub	w1, w1, w4
	and	w1, w1, 511
	bne	.L433
.L425:
	mul	w1, w1, w0
	str	w1, [x19, 744]
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L433:
	mov	w0, 512
	sdiv	w0, w0, w2
	b	.L425
	.p2align 3
.L426:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AVS_UpdateTableSymScale, .-AVS_UpdateTableSymScale
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_UpdateTabBBlkDist
	.type	AVS_UpdateTabBBlkDist, %function
AVS_UpdateTabBBlkDist:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, [x19, 96]
	add	x0, x19, 8
	cmp	x1, x0
	beq	.L437
	ldrb	w0, [x19, 217]
	cmp	w0, 1
	beq	.L438
	ldr	x7, [x19, 80]
	add	x0, x19, 512
	ldr	w5, [x19, 568]
	ldr	w4, [x19, 576]
	ldp	w3, w1, [x0, 48]
	ldp	w6, w2, [x0, 40]
	ldr	w0, [x7, 8]
	lsl	w0, w0, 1
	sub	w3, w3, w0
	sub	w2, w0, w2
	sub	w6, w0, w6
	sub	w0, w1, w0
	and	w1, w3, 511
	and	w6, w6, 511
	str	w1, [x19, 768]
	and	w0, w0, 511
	str	w6, [x19, 760]
	and	w1, w2, 511
	str	w0, [x19, 772]
	mov	w0, 1
	str	w1, [x19, 764]
	ldr	w3, [x19, 572]
	ldr	w1, [x7, 8]
	ldr	w2, [x19, 580]
	lsl	w1, w1, 1
	add	w1, w1, w0
	sub	w3, w1, w3
	sub	w5, w1, w5
	sub	w4, w4, w1
	sub	w1, w2, w1
	and	w2, w3, 511
	add	x3, x19, 768
	and	w5, w5, 511
	and	w4, w4, 511
	and	w1, w1, 511
	stp	w5, w2, [x3, 8]
	stp	w4, w1, [x3, 16]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L438:
	ldp	x3, x4, [x19, 80]
	ldr	w2, [x1, 8]
	ldr	w3, [x3, 8]
	ldr	w1, [x4, 8]
	lsl	w3, w3, 1
	lsl	w1, w1, 1
	sub	w2, w3, w2, lsl 1
	sub	w1, w1, w3
	and	w2, w2, 511
	and	w1, w1, 511
	str	w2, [x19, 760]
	str	w1, [x19, 768]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L437:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AVS_UpdateTabBBlkDist, .-AVS_UpdateTabBBlkDist
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_UpdatePmvTable
	.type	AVS_UpdatePmvTable, %function
AVS_UpdatePmvTable:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x20, 484]
	cmp	w1, 1
	beq	.L441
	cbz	w1, .L442
	cmp	w1, 2
	beq	.L443
	mov	w19, 0
.L440:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L443:
	mov	x0, x20
	mov	w19, 1
	bl	AVS_UpdateTableIdx2Dist
	mov	x0, x20
	bl	AVS_UpdateTableMvEScale
	mov	x0, x20
	bl	AVS_UpdateTableDirectScale
	mov	x0, x20
	bl	AVS_UpdateTabBBlkDist
	mov	x0, x20
	bl	AVS_UpdateTableSymScale
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L442:
	ldrb	w0, [x20, 217]
	mov	w19, 1
	cmp	w0, w19
	beq	.L440
	mov	x0, x20
	bl	AVS_UpdateTableIdx2Dist
	mov	x0, x20
	bl	AVS_UpdateTableMvEScale
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L441:
	mov	x0, x20
	mov	w19, w1
	bl	AVS_UpdateTableIdx2Dist
	mov	x0, x20
	bl	AVS_UpdateTableMvEScale
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AVS_UpdatePmvTable, .-AVS_UpdatePmvTable
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_GetStreamInf
	.type	AVS_GetStreamInf, %function
AVS_GetStreamInf:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 12288
	bl	_mcount
	ldr	w1, [x20, 2784]
	ldrb	w2, [x20, 2800]
	cmp	w1, 1
	str	w2, [x20, 3484]
	beq	.L450
	ldr	w0, [x19, 8]
	cmp	w0, 1
	beq	.L451
.L450:
	ldr	x1, .LC255
	mov	w0, 22
	bl	dprint_vfmw
	add	x3, x29, 80
	ldr	w0, [x19, 4]
	add	x2, x29, 72
	add	x1, x29, 68
	stp	wzr, wzr, [x29, 68]
	str	wzr, [x3, -4]!
	bl	VCTRL_GetChanImgNum
	ldr	x0, [x19, 16]
	ldr	w2, [x0, 748]
	cmp	w2, 1
	beq	.L539
.L453:
	add	x0, x20, 2788
	add	x1, x20, 2648
	ldr	w2, [x0, 24]
	add	w2, w2, 15
	lsr	w2, w2, 4
	str	w2, [x1, 116]
	ldrb	w3, [x0, 15]
	cmp	w3, 1
	ldr	w3, [x0, 28]
	beq	.L540
	add	w3, w3, 31
	lsr	w3, w3, 5
	lsl	w3, w3, 1
	str	w3, [x1, 120]
.L459:
	cmp	w2, 512
	bgt	.L460
	cmp	w3, 512
	ble	.L461
.L460:
	ldr	x1, .LC260
	mov	w0, 18
	mov	w22, 0
	bl	dprint_vfmw
.L456:
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L461:
	add	x22, x20, 2648
	ldr	x1, [x22, 88]
	ldr	w0, [x19, 4]
	add	x21, x20, 2656
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	mov	x23, x0
	cbz	x0, .L463
	ldr	x1, [x22, 80]
	mov	w2, 1
	ldr	w0, [x19, 4]
	add	x21, x20, 2656
	ldr	w1, [x1, 4]
	bl	FSP_ClearLogicFs
	ldr	x1, [x22, 88]
	mov	w2, 1
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_ClearLogicFs
	ldr	x1, [x22, 96]
	mov	w2, 1
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_ClearLogicFs
	ldr	x1, [x22, 104]
	mov	w2, 1
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_ClearLogicFs
	ldr	w0, [x19, 4]
	ldr	x1, [x23, 560]
	bl	FreeUsdByDec
	ldr	w0, [x19, 4]
	ldr	x1, [x23, 568]
	bl	FreeUsdByDec
	ldr	w0, [x19, 4]
	ldr	x1, [x23, 576]
	bl	FreeUsdByDec
	ldr	w0, [x19, 4]
	ldr	x1, [x23, 584]
	bl	FreeUsdByDec
	add	x0, x23, 512
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	stp	x21, x21, [x22, 80]
	stp	x21, x21, [x22, 96]
	ldr	w0, [x19, 4]
	bl	FSP_GetNullLogicFs
	str	w0, [x21, 4]
.L463:
	str	wzr, [x20, 3444]
	add	x22, x20, 3912
	mov	x1, x22
	ldr	w0, [x19, 4]
	bl	FSP_ClearNotInVoQueue
	ldr	x0, [x19, 16]
	ldr	w1, [x0, 748]
	cbnz	w1, .L464
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L541
.L464:
	ldr	w0, [x19, 4]
	bl	FSP_GetFspType
	cbz	w0, .L537
.L466:
	add	x23, x20, 2648
	mov	x0, x19
	str	wzr, [x23, 136]
	mov	w22, 3
	stp	x21, x21, [x23, 80]
	stp	x21, x21, [x23, 96]
	bl	AVS_AllocateVHB
	cmp	w0, w22
	beq	.L456
	cbz	w0, .L542
	str	wzr, [x23, 136]
.L451:
	add	x21, x20, 2788
	add	x23, x20, 2648
	ldrb	w2, [x21, 11]
	mov	x0, x19
	ldr	x1, [x23, 80]
	str	w2, [x23, 832]
	ldrb	w2, [x21, 77]
	strb	w2, [x1]
	bl	AVS_GetImageBuffer
	mov	w22, w0
	cbz	w0, .L543
	ldr	x1, [x23, 80]
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	mov	x2, x0
	cbz	x0, .L544
	ldr	x3, [x19, 16]
	mov	x4, -1
	ldr	x1, .LC267
	mov	w0, 29
	ldr	x3, [x3, 56]
	str	x3, [x2, 600]
	ldr	x3, [x19, 16]
	ldr	x3, [x3, 72]
	str	x3, [x2, 608]
	ldr	x3, [x19, 16]
	ldr	x3, [x3, 88]
	str	x3, [x2, 616]
	ldr	x3, [x19, 16]
	ldr	w3, [x3, 104]
	str	w3, [x2, 8]
	ldr	x3, [x19, 16]
	ldr	w3, [x3, 108]
	str	w3, [x2, 12]
	ldr	x3, [x19, 16]
	ldr	w3, [x3, 112]
	str	w3, [x2, 16]
	ldr	x3, [x19, 16]
	ldr	w3, [x3, 128]
	str	w3, [x2, 20]
	ldrb	w3, [x21, 77]
	cmp	w3, wzr
	cset	w3, eq
	str	w3, [x2, 96]
	ldr	x3, [x19, 16]
	str	x4, [x3, 56]
	ldr	x3, [x2, 608]
	ldr	x2, [x2, 600]
	bl	dprint_vfmw
	ldr	x0, [x23, 80]
	mov	w1, 1
	ldr	w2, [x21, 108]
	str	w2, [x0, 8]
	ldr	x0, [x23, 80]
	ldrb	w2, [x21, 77]
	strb	w2, [x0]
	ldr	x0, [x23, 80]
	ldr	w2, [x21, 344]
	strb	w2, [x0, 1]
	ldrb	w0, [x21, 77]
	ldr	x2, [x23, 80]
	cbz	w0, .L471
	ldrsb	w1, [x21, 78]
.L471:
	strb	w1, [x2, 2]
	ldrb	w1, [x20, 2865]
	cbnz	w1, .L472
	ldr	w0, [x20, 3132]
	cbz	w0, .L545
.L472:
	add	x22, x20, 2788
	add	x23, x20, 2648
	ldr	w1, [x22, 340]
	mov	x0, x23
	str	w1, [x20, 3488]
	add	x21, x19, 16384
	bl	AVS_UpdatePmvTable
	ldr	w4, [x20, 2764]
	add	x1, x21, 608
	ldr	w3, [x20, 2768]
	mov	w2, 1
	mul	w0, w4, w3
	sub	w0, w0, #1
	str	w0, [x21, 608]
	stp	wzr, w2, [x1, 4]
	ldr	w2, [x20, 3188]
	stp	w3, w4, [x1, 56]
	cmp	w2, wzr
	add	w0, w2, 63
	csel	w0, w0, w2, lt
	mov	w2, 6
	asr	w0, w0, 6
	mov	w3, 3
	stp	w0, w2, [x1, 12]
	add	x0, x20, 2988
	add	x2, x20, 3116
	ldr	w4, [x20, 3484]
	str	w4, [x1, 64]
	ldr	w4, [x20, 3480]
	str	w4, [x1, 68]
	ldrb	w4, [x22, 90]
	str	w4, [x1, 72]
	ldrb	w4, [x22, 89]
	str	w4, [x1, 76]
	ldrb	w4, [x22, 88]
	str	w4, [x1, 80]
	ldrb	w4, [x22, 82]
	str	w4, [x1, 84]
	ldrb	w4, [x22, 81]
	str	w4, [x1, 88]
	ldrb	w4, [x22, 80]
	str	w4, [x1, 92]
	ldrb	w4, [x22, 79]
	str	w4, [x1, 96]
	ldrb	w4, [x22, 78]
	str	w4, [x1, 100]
	ldr	w4, [x22, 344]
	str	w4, [x1, 104]
	ldrb	w4, [x22, 77]
	cmp	w4, wzr
	csinc	w3, w3, wzr, ne
	str	w3, [x1, 108]
	ldrb	w3, [x22, 76]
	str	w3, [x1, 112]
	ldr	w3, [x22, 120]
	str	w3, [x1, 116]
	ldr	w3, [x22, 116]
	str	w3, [x1, 120]
	ldrb	w3, [x22, 85]
	str	w3, [x1, 124]
	ldrb	w3, [x22, 84]
	str	w3, [x1, 128]
	ldr	w3, [x22, 124]
	str	w3, [x1, 204]
	ldr	w3, [x22, 132]
	str	w3, [x1, 208]
	ldr	w3, [x22, 136]
	str	w3, [x1, 216]
	ldr	w3, [x22, 140]
	str	w3, [x1, 212]
	.p2align 2
.L475:
	ldrh	w1, [x0]
	add	x0, x0, 2
	cmp	x0, x2
	strh	w1, [x0, 2094]
	bne	.L475
	add	x22, x21, 608
	ldr	w1, [x20, 3116]
	add	x23, x20, 2648
	str	w1, [x22, 220]
	ldr	w1, [x20, 3128]
	str	w1, [x22, 228]
	ldr	w0, [x20, 3124]
	str	w0, [x22, 224]
	ldp	x0, x1, [x23, 80]
	ldrb	w0, [x0, 1]
	ldr	w1, [x1, 4]
	cmp	w0, 2
	ldr	w0, [x19, 4]
	beq	.L546
	bl	FSP_GetFsImagePtr
	cbz	x0, .L547
	ldr	x1, [x23, 88]
	mov	w24, 3
	ldrb	w2, [x1]
	cmp	w2, 1
	csinc	w2, w24, wzr, eq
	str	w2, [x22, 132]
	ldrsb	w2, [x1, 2]
	str	w2, [x22, 136]
	ldr	w2, [x0, 44]
	str	w2, [x22, 156]
	ldrb	w2, [x1]
	cmp	w2, 1
	csinc	w2, w24, wzr, eq
	str	w2, [x22, 148]
	ldrsb	w1, [x1, 2]
	str	w1, [x22, 152]
	ldr	w0, [x0, 44]
	str	w0, [x22, 160]
	ldr	x1, [x23, 96]
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L548
	ldr	x1, [x23, 96]
.L538:
	ldrb	w2, [x1]
	add	x23, x20, 2648
	cmp	w2, 1
	csinc	w24, w24, wzr, eq
	str	w24, [x22, 140]
	ldrsb	w1, [x1, 2]
	str	w1, [x22, 144]
	ldr	w0, [x0, 44]
	str	w0, [x22, 164]
	ldr	x1, [x23, 88]
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L549
	add	x22, x21, 608
	ldr	w0, [x0, 96]
	str	w0, [x22, 36]
	ldr	x1, [x23, 96]
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L550
	ldr	w0, [x0, 96]
	str	w0, [x22, 40]
	ldr	x1, [x23, 104]
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L551
	ldr	w0, [x0, 96]
	str	w0, [x22, 44]
	ldr	x1, [x23, 80]
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L552
	ldr	w2, [x0, 96]
	mov	w1, 3
	str	w2, [x22, 48]
	ldr	w2, [x0, 44]
	str	w2, [x22, 52]
	ldr	w0, [x0, 60]
	str	w0, [x22, 168]
	ldr	x2, [x23, 88]
	ldrb	w0, [x2]
	cmp	w0, 1
	csinc	w0, w1, wzr, eq
	str	w0, [x22, 172]
	ldrb	w0, [x2, 1]
	cmp	w0, wzr
	cset	w0, ne
	str	w0, [x22, 176]
	ldr	x1, [x23, 80]
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetPmvAddr
	str	w0, [x22, 232]
	ldr	x1, [x23, 88]
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetPmvAddr
	str	w0, [x22, 244]
	mov	w7, 4
	mov	w4, 8
	ldr	w5, [x20, 3132]
	mov	w1, 16
	mov	w3, 32
	mov	w2, 0
	cmp	w5, 2
	add	x0, x20, 3232
	csel	w7, w7, wzr, eq
	csel	w4, w4, wzr, eq
	csel	w3, w1, w3, eq
	.p2align 2
.L496:
	ldr	w1, [x0]
	add	x0, x0, 4
	add	w2, w2, 1
	and	w1, w1, 262143
	cmp	w3, w2
	str	w1, [x0, 1716]
	bgt	.L496
	cbz	w4, .L501
	add	x2, x20, 3360
	add	w4, w4, 16
	mov	w0, 16
	add	x5, x21, 608
	.p2align 2
.L500:
	add	x3, x5, x0, sxtw 2
	ldr	w1, [x2], 4
	add	w0, w0, 1
	and	w1, w1, 32767
	cmp	w4, w0
	str	w1, [x3, 248]
	bne	.L500
.L501:
	cbz	w7, .L499
	lsl	w9, w7, 1
	mov	w4, 24
	mov	w2, 0
	add	x8, x21, 608
	add	x5, x20, 2648
	.p2align 2
.L502:
	add	w1, w2, 1
	add	x0, x5, x2, sxtw 2
	add	x6, x8, x4, sxtw 2
	add	w2, w2, 2
	add	x1, x5, x1, sxtw 2
	cmp	w2, w9
	ldr	w0, [x0, 760]
	add	w4, w4, 1
	ldr	w3, [x1, 760]
	and	w1, w0, 511
	ubfiz	w0, w3, 9, 9
	orr	w0, w0, w1
	str	w0, [x6, 248]
	bne	.L502
	add	x2, x20, 3392
	add	w4, w7, 28
	mov	w0, 28
	add	x5, x21, 608
	.p2align 2
.L503:
	add	x3, x5, x0, sxtw 2
	ldr	w1, [x2], 4
	add	w0, w0, 1
	and	w1, w1, 262143
	cmp	w4, w0
	str	w1, [x3, 248]
	bne	.L503
.L499:
	ldr	w0, [x20, 3184]
	mov	w22, 1
	str	w0, [x21, 984]
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L539:
	ldr	w0, [x29, 76]
	cmp	w0, wzr
	ble	.L553
.L454:
	mov	w22, 3
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L540:
	add	w3, w3, 15
	lsr	w3, w3, 4
	str	w3, [x1, 120]
	b	.L459
	.p2align 3
.L543:
	ldr	x1, .LC264
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
.L541:
	ldr	w0, [x19, 8]
	cbnz	w0, .L466
.L537:
	mov	x0, x22
	bl	ResetVoQueue
	b	.L466
.L553:
	ldr	w0, [x29, 72]
	cmp	w0, 32
	bgt	.L454
	ldr	x0, [x19, 24]
	cbnz	x0, .L454
	ldr	w3, [x19, 15068]
	cbz	w3, .L453
	ldr	w7, [x19, 15848]
	cbz	w7, .L453
	add	x21, x19, 16384
	ldr	w5, [x21, 1116]
	cmp	w7, w5
	beq	.L453
	ldr	w6, [x19, 15852]
	ldr	w4, [x21, 1120]
	cmp	w6, w4
	beq	.L453
	ldr	x1, .LC257
	mov	w0, w2
	bl	dprint_vfmw
	ldr	w0, [x19, 15848]
	ldr	x1, .LC258
	str	w0, [x21, 1116]
	ldr	x4, [x1]
	ldr	w0, [x19, 15852]
	str	w0, [x21, 1120]
	cbz	x4, .L453
	ldr	w0, [x19, 4]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 120
	blr	x4
	b	.L453
	.p2align 3
.L546:
	bl	FSP_GetFsImagePtr
	cbz	x0, .L554
	ldr	x1, [x23, 88]
	mov	w24, 3
	ldrb	w2, [x1]
	cmp	w2, 1
	csinc	w2, w24, wzr, eq
	str	w2, [x22, 132]
	ldrsb	w1, [x1, 2]
	str	w1, [x22, 136]
	ldr	w0, [x0, 44]
	str	w0, [x22, 156]
	ldr	x1, [x23, 96]
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L555
	ldr	x1, [x23, 96]
	ldrb	w2, [x1]
	cmp	w2, 1
	csinc	w2, w24, wzr, eq
	str	w2, [x22, 148]
	ldrsb	w1, [x1, 2]
	str	w1, [x22, 152]
	ldr	w0, [x0, 44]
	str	w0, [x22, 160]
	ldr	x1, [x23, 104]
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L556
	ldr	x1, [x23, 104]
	b	.L538
.L545:
	ldr	x1, [x20, 2728]
	mov	w2, 1
	ldr	w0, [x19, 4]
	ldr	w1, [x1, 4]
	bl	FSP_SetRef
	ldr	x0, [x20, 2752]
	add	x1, x20, 2656
	cmp	x0, x1
	beq	.L473
	ldr	w1, [x0, 4]
	mov	w2, 0
	ldr	w0, [x19, 4]
	bl	FSP_SetRef
.L473:
	ldr	x2, [x20, 2736]
	ldr	x3, [x20, 2744]
	ldr	x1, [x20, 2728]
	str	x1, [x20, 2736]
	str	x2, [x20, 2744]
	str	x3, [x20, 2752]
	b	.L472
.L544:
	ldr	x1, .LC265
	mov	w2, 1872
	mov	w0, 0
	mov	w22, 0
	bl	dprint_vfmw
	b	.L456
.L542:
	ldr	x1, .LC262
	mov	w0, 1
	mov	w22, 0
	bl	dprint_vfmw
	b	.L456
.L549:
	ldr	x1, .LC265
	mov	w2, 2003
	mov	w22, 0
	bl	dprint_vfmw
	b	.L456
.L550:
	ldr	x1, .LC265
	mov	w2, 2007
	mov	w22, 0
	bl	dprint_vfmw
	b	.L456
.L551:
	ldr	x1, .LC265
	mov	w2, 2011
	mov	w22, 0
	bl	dprint_vfmw
	b	.L456
.L548:
	ldr	x1, .LC265
	mov	w2, 1996
	mov	w22, 0
	bl	dprint_vfmw
	b	.L456
.L547:
	ldr	x1, .LC265
	mov	w2, 1987
	mov	w22, 0
	bl	dprint_vfmw
	b	.L456
.L552:
	ldr	x1, .LC265
	mov	w2, 2015
	mov	w22, 0
	bl	dprint_vfmw
	b	.L456
.L556:
	ldr	x1, .LC265
	mov	w2, 1979
	mov	w22, 0
	bl	dprint_vfmw
	b	.L456
.L555:
	ldr	x1, .LC265
	mov	w2, 1973
	mov	w22, 0
	bl	dprint_vfmw
	b	.L456
.L554:
	ldr	x1, .LC265
	mov	w2, 1967
	mov	w22, 0
	bl	dprint_vfmw
	b	.L456
	.size	AVS_GetStreamInf, .-AVS_GetStreamInf
	.align	3
.LC255:
	.xword	.LC254
	.align	3
.LC257:
	.xword	.LC256
	.align	3
.LC258:
	.xword	g_event_report
	.align	3
.LC260:
	.xword	.LC259
	.align	3
.LC262:
	.xword	.LC261
	.align	3
.LC264:
	.xword	.LC263
	.align	3
.LC265:
	.xword	.LC248
	.align	3
.LC267:
	.xword	.LC266
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_PrepareDecode
	.type	AVS_PrepareDecode, %function
AVS_PrepareDecode:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	AVS_GetStreamInf
	cmp	w0, 3
	beq	.L558
	cmp	w0, wzr
	cset	w0, ne
.L558:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AVS_PrepareDecode, .-AVS_PrepareDecode
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_VerifyMemPara
	.type	AVS_VerifyMemPara, %function
AVS_VerifyMemPara:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	AVS_VerifyMemPara, .-AVS_VerifyMemPara
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVSDEC_Init
	.type	AVSDEC_Init, %function
AVSDEC_Init:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x21, x0
	mov	x0, x30
	mov	x24, x1
	add	x20, x21, 16384
	bl	_mcount
	ldr	x23, .LC268
	mov	x2, 17512
	mov	w1, 0
	mov	x0, x21
	add	x19, x21, 12288
	ldr	x3, [x23, 96]
	add	x22, x19, 3912
	ldr	w25, [x20, 416]
	blr	x3
	str	w25, [x20, 416]
	mov	x0, x22
	add	x20, x19, 2648
	add	x25, x19, 3496
	bl	ResetVoQueue
	str	x24, [x21, 16]
	mov	x0, x21
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x21, 4]
	beq	.L564
	ldr	x3, [x23, 96]
	mov	w1, 0
	mov	x2, 848
	mov	x0, x20
	add	x19, x19, 2656
	blr	x3
	str	x25, [x20, 512]
	mov	w0, 1024
	str	x22, [x20, 544]
	str	w0, [x20, 540]
	mov	w0, 1
	stp	x19, x19, [x20, 80]
	stp	x19, x19, [x20, 96]
	ldr	x1, [x21, 16]
	str	wzr, [x1, 912]
.L563:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L564:
	ldr	x1, .LC270
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L563
	.size	AVSDEC_Init, .-AVSDEC_Init
	.align	3
.LC268:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC270:
	.xword	.LC269
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVSDEC_Destroy
	.type	AVSDEC_Destroy, %function
AVSDEC_Destroy:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	AVSDEC_Destroy, .-AVSDEC_Destroy
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVSDEC_RecycleImage
	.type	AVSDEC_RecycleImage, %function
AVSDEC_RecycleImage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cbz	x19, .L568
	ldr	w0, [x19, 4]
	mov	w1, w20
	mov	w2, 0
	bl	FSP_SetDisplay
	ldr	w0, [x19, 4]
	mov	w1, w20
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L569
	ldr	w0, [x19, 4]
	ldr	x1, [x20, 560]
	bl	FreeUsdByDec
	str	xzr, [x20, 560]
	ldr	x1, [x20, 568]
	ldr	w0, [x19, 4]
	bl	FreeUsdByDec
	str	xzr, [x20, 568]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L568:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L569:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AVSDEC_RecycleImage, .-AVSDEC_RecycleImage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVSDEC_GetRemainImg
	.type	AVSDEC_GetRemainImg, %function
AVSDEC_GetRemainImg:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	add	x24, x20, 12288
	bl	_mcount
	ldr	x0, [x20, 16]
	ldr	w0, [x0, 12]
	cmp	w0, 1
	beq	.L577
	mov	w21, 3
	add	x19, x24, 2752
	mov	w23, -1
	add	x25, x24, 3912
.L576:
	ldr	x1, [x19]
	ldr	w0, [x20, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L573
	ldr	x1, [x19]
	mov	w23, 1
	ldr	w0, [x20, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetDisplay
	cmp	w0, 2
	beq	.L585
.L573:
	subs	w21, w21, #1
	sub	x19, x19, #8
	bne	.L576
	cbnz	w23, .L577
	mov	w0, w23
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L577:
	add	x0, x24, 3912
	bl	GetVoLastImageID
	mov	w23, w0
	mov	w0, w23
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L585:
	mov	x0, x20
	mov	w23, -1
	bl	AVS_SetImgFormat
	mov	w2, 1
	cmp	w21, w2
	beq	.L586
.L574:
	ldr	x1, [x19]
	ldr	w0, [x20, 4]
	ldr	w1, [x1, 4]
	bl	FSP_SetDisplay
	ldr	w0, [x20, 4]
	mov	x4, x22
	mov	x3, x25
	mov	x2, x20
	mov	w1, 6
	bl	InsertImgToVoQueue
	cmp	w0, 1
	mov	x0, x20
	beq	.L575
	ldr	w1, [x22, 156]
	bl	AVSDEC_RecycleImage
	ldr	w0, [x20, 4]
	ldr	x1, [x22, 560]
	bl	FreeUsdByDec
	str	xzr, [x22, 560]
	ldr	x1, [x22, 568]
	ldr	w0, [x20, 4]
	bl	FreeUsdByDec
	str	xzr, [x22, 568]
	b	.L573
	.p2align 3
.L575:
	ldr	w0, [x22, 160]
	mov	w23, -1
	cmp	w0, 100
	bhi	.L573
	ldr	w0, [x22, 112]
	cmp	w0, 31
	bls	.L573
	ldr	w0, [x22, 116]
	cmp	w0, 31
	csetm	w23, ls
	b	.L573
.L586:
	str	w21, [x22, 188]
	b	.L574
	.size	AVSDEC_GetRemainImg, .-AVSDEC_GetRemainImg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVSDEC_GetImageBuffer
	.type	AVSDEC_GetImageBuffer, %function
AVSDEC_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x20, 4]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	mov	w19, w0
	beq	.L588
	ldr	w0, [x20, 4]
	mov	w19, 0
	bl	FSP_IsNewFsAvalible
	cmn	w0, #1
	beq	.L593
.L588:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L593:
	add	x1, x20, 12288
	ldr	w0, [x20, 4]
	add	x1, x1, 3912
	bl	FSP_ClearNotInVoQueue
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AVSDEC_GetImageBuffer, .-AVSDEC_GetImageBuffer
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ClearSlice
	.type	ClearSlice, %function
ClearSlice:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w22, w0
	mov	x0, x30
	mov	x20, x1
	add	x19, x1, 8
	add	x21, x1, 16
	bl	_mcount
.L596:
	ldr	w0, [x19]
	cmp	w0, wzr
	ble	.L595
	ldr	w0, [x19, -8]
	cbnz	w0, .L601
.L595:
	str	wzr, [x19, -8]
	add	x19, x19, 4
	str	wzr, [x19, -4]
	cmp	x19, x21
	str	wzr, [x20, 16]
	bne	.L596
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L601:
	ldr	w1, [x19, 12]
	mov	w0, w22
	bl	SM_ReleaseStreamSeg
	b	.L595
	.size	ClearSlice, .-ClearSlice
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	AVS_DecodeArbiter.part.3, %function
AVS_DecodeArbiter.part.3:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x21, 16]
	ldr	w0, [x0, 8]
	cmp	w0, 4
	bls	.L631
.L624:
	mov	w1, 1
	add	x22, x21, 12288
.L603:
	str	w0, [x22, 3440]
	cbnz	w1, .L610
	add	x20, x21, 32
	ldr	w0, [x20, 14440]
	mov	w19, 0
	cmp	w0, wzr
	ble	.L623
	.p2align 2
.L625:
	sxtw	x2, w19
	ldr	w0, [x21, 4]
	lsl	x1, x2, 3
	add	w19, w19, 1
	sub	x1, x1, x2
	add	x1, x20, x1, lsl 2
	add	x1, x1, 104
	bl	ClearSlice
	ldr	w0, [x20, 14440]
	cmp	w19, w0
	blt	.L625
.L623:
	str	wzr, [x20, 14440]
	mov	w0, 16383
	ldr	x2, .LC272
	movk	w0, 0xfffb, lsl 16
	ldr	w1, [x22, 2772]
	ldr	x4, [x2]
	and	w0, w1, w0
	str	w0, [x22, 2772]
	cbz	x4, .L630
	ldr	w0, [x21, 4]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L630:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L604:
	add	x22, x21, 12288
	ldr	w2, [x22, 3440]
	cmp	w2, 2
	bne	.L608
	str	wzr, [x22, 3444]
.L608:
	str	w0, [x22, 3440]
.L610:
	ldr	w0, [x22, 3880]
	cmp	w0, 1
	beq	.L632
	cmp	w0, 2
	bne	.L618
	ldr	w0, [x22, 3444]
	cmp	w0, 1
	ble	.L621
.L618:
	add	x1, x22, 3536
	mov	x2, 372
	add	x0, x22, 2788
	bl	memcpy
	ldrb	w4, [x22, 2865]
	mov	w0, 18
	ldr	w3, [x22, 3132]
	ldr	w2, [x22, 3456]
	ldr	x1, .LC274
	bl	dprint_vfmw
	ldr	w2, [x22, 2772]
	mov	w1, 16383
	movk	w1, 0xfffb, lsl 16
	mov	w0, 1
	and	w1, w2, w1
	str	w1, [x22, 2772]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L631:
	ldr	x1, .LC271
	ldrb	w1, [x1,w0,uxtw]
	adr	x2, .Lrtx605
	add	x1, x2, w1, sxtb #2
	br	x1
.Lrtx605:
	.section	.rodata
	.align	0
	.align	2
.L605:
	.byte	(.L604 - .Lrtx605) / 4
	.byte	(.L606 - .Lrtx605) / 4
	.byte	(.L607 - .Lrtx605) / 4
	.byte	(.L624 - .Lrtx605) / 4
	.byte	(.L604 - .Lrtx605) / 4
	.text
	.p2align 3
.L607:
	add	x22, x21, 12288
	ldr	w1, [x22, 3880]
	cmp	w1, wzr
	cset	w1, eq
	b	.L603
	.p2align 3
.L606:
	add	x22, x21, 12288
	ldr	w2, [x22, 3440]
	cmp	w2, 2
	beq	.L633
.L609:
	ldr	w1, [x22, 3880]
	cmp	w1, 2
	cset	w1, ne
	b	.L603
	.p2align 3
.L632:
	ldr	w0, [x22, 3444]
	cmp	w0, wzr
	bgt	.L618
.L621:
	add	x20, x21, 32
	ldr	w0, [x20, 14440]
	mov	w19, 0
	cmp	w0, wzr
	ble	.L623
	.p2align 2
.L626:
	sxtw	x2, w19
	ldr	w0, [x21, 4]
	lsl	x1, x2, 3
	add	w19, w19, 1
	sub	x1, x1, x2
	add	x1, x20, x1, lsl 2
	add	x1, x1, 104
	bl	ClearSlice
	ldr	w0, [x20, 14440]
	cmp	w19, w0
	blt	.L626
	b	.L623
	.p2align 3
.L633:
	str	wzr, [x22, 3444]
	b	.L609
	.size	AVS_DecodeArbiter.part.3, .-AVS_DecodeArbiter.part.3
	.align	3
.LC271:
	.xword	.L605
	.align	3
.LC272:
	.xword	g_event_report
	.align	3
.LC274:
	.xword	.LC273
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_DecodeArbiter
	.type	AVS_DecodeArbiter, %function
AVS_DecodeArbiter:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	w0, 49152
	ldr	w1, [x19, 15060]
	movk	w0, 0x5, lsl 16
	and	w0, w1, w0
	cmp	w0, 376832
	beq	.L637
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L637:
	mov	x0, x19
	bl	AVS_DecodeArbiter.part.3
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AVS_DecodeArbiter, .-AVS_DecodeArbiter
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVSDEC_VDMPostProc
	.type	AVSDEC_VDMPostProc, %function
AVSDEC_VDMPostProc:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 32
	mov	w22, w1
	bl	_mcount
	mov	w19, 0
	ldr	w0, [x20, 14440]
	cmp	w0, wzr
	ble	.L643
	.p2align 2
.L683:
	sxtw	x3, w19
	ldr	w0, [x21, 4]
	lsl	x1, x3, 3
	add	w19, w19, 1
	sub	x1, x1, x3
	add	x1, x20, x1, lsl 2
	add	x1, x1, 104
	bl	ClearSlice
	ldr	w0, [x20, 14440]
	cmp	w19, w0
	blt	.L683
.L643:
	ldr	x0, .LC275
	add	x19, x21, 12288
	add	x23, x19, 2788
	mov	x2, 14904
	mov	w1, 0
	str	wzr, [x20, 14440]
	ldr	x3, [x0, 96]
	mov	x0, x20
	blr	x3
	ldr	w0, [x23, 344]
	cbnz	w0, .L696
	str	w22, [x19, 3448]
	mov	w2, w22
	ldr	x0, [x21, 16]
	ldr	w3, [x0]
	cmp	w2, w3
	bgt	.L697
.L648:
	mov	x0, x21
	add	x20, x19, 2648
	bl	AVS_SetImgFormat
	ldr	x1, [x20, 80]
	ldr	w0, [x21, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L698
	ldr	x1, [x20, 816]
	cbz	x1, .L651
	str	x1, [x0, 560]
.L651:
	ldr	x2, [x19, 3472]
	str	xzr, [x19, 3464]
	cbz	x2, .L652
	str	x2, [x0, 568]
.L652:
	add	x20, x19, 2648
	str	xzr, [x20, 824]
	ldr	w0, [x19, 3132]
	cmp	w0, 1
	bls	.L699
	ldr	x20, [x20, 80]
.L658:
	ldr	w1, [x20, 4]
	ldr	w0, [x21, 4]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L700
	add	x0, x19, 2656
	add	x19, x19, 2648
	cmp	x20, x0
	beq	.L663
	ldr	x0, [x21, 16]
	ldr	w2, [x19, 800]
	ldr	w3, [x0, 4]
	cmp	w3, w2
	bge	.L661
	ldr	x1, .LC280
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x20, 4]
	mov	w2, 0
	ldr	w0, [x21, 4]
	bl	FSP_SetDisplay
	ldr	x1, [x22, 560]
	cbnz	x1, .L701
.L662:
	ldr	x1, [x22, 568]
	cbnz	x1, .L702
.L663:
	mov	w19, 1
.L646:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L661:
	mov	x1, x21
	mov	x0, x20
	bl	AVS_OutputOneFrame
	mov	w19, w0
	cbnz	w0, .L663
	ldr	x1, [x22, 560]
	cbnz	x1, .L703
.L664:
	ldr	x1, [x22, 568]
	cbz	x1, .L646
	ldr	w0, [x21, 4]
	bl	FreeUsdByDec
	str	xzr, [x22, 568]
	b	.L646
	.p2align 3
.L696:
	add	x20, x19, 2648
	ldr	x1, [x20, 88]
	ldr	w0, [x21, 4]
	ldr	w1, [x1, 4]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L704
	ldr	x2, [x20, 96]
	ldr	w24, [x0, 160]
	ldr	w0, [x21, 4]
	ldr	w1, [x2, 4]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L705
	ldr	w2, [x0, 160]
	mov	w0, 100
	add	w2, w24, w2
	add	w2, w2, w22
	cmp	w2, w0
	csel	w2, w2, w0, le
	str	w2, [x20, 800]
	ldr	w0, [x23, 344]
	cmp	w0, 2
	beq	.L648
	ldr	x0, [x21, 16]
	ldr	w3, [x0]
	cmp	w2, w3
	ble	.L648
	.p2align 2
.L697:
	ldr	x1, .LC278
	mov	w0, 1
	bl	dprint_vfmw
	str	wzr, [x19, 3444]
	ldr	x0, [x19, 2736]
	ldr	w1, [x0, 4]
	ldr	w0, [x21, 4]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L649
	ldr	x1, [x19, 2728]
	mov	w2, 1
	ldr	w0, [x21, 4]
	add	x23, x19, 2656
	ldr	w1, [x1, 4]
	bl	FSP_ClearLogicFs
	ldr	x1, [x19, 2736]
	mov	w2, 1
	ldr	w0, [x21, 4]
	ldr	w1, [x1, 4]
	bl	FSP_ClearLogicFs
	ldr	x1, [x19, 2744]
	mov	w2, 1
	ldr	w0, [x21, 4]
	ldr	w1, [x1, 4]
	bl	FSP_ClearLogicFs
	ldr	x1, [x19, 2752]
	mov	w2, 1
	ldr	w0, [x21, 4]
	ldr	w1, [x1, 4]
	bl	FSP_ClearLogicFs
	ldr	w0, [x21, 4]
	ldr	x1, [x22, 560]
	bl	FreeUsdByDec
	ldr	w0, [x21, 4]
	ldr	x1, [x22, 568]
	bl	FreeUsdByDec
	ldr	w0, [x21, 4]
	ldr	x1, [x22, 576]
	bl	FreeUsdByDec
	ldr	w0, [x21, 4]
	ldr	x1, [x22, 584]
	bl	FreeUsdByDec
	add	x0, x22, 512
	add	x1, x19, 2560
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	stp	x23, x23, [x1, 168]
	stp	x23, x23, [x1, 184]
	ldr	w0, [x21, 4]
	bl	FSP_GetNullLogicFs
	str	w0, [x23, 4]
.L649:
	str	wzr, [x19, 3444]
	mov	w19, 0
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L702:
	ldr	w0, [x21, 4]
	mov	w19, 1
	bl	FreeUsdByDec
	str	xzr, [x22, 568]
	b	.L646
	.p2align 3
.L699:
	ldr	w2, [x20, 796]
	cmp	w2, 1
	csinc	w2, w2, w2, gt
	str	w2, [x20, 796]
	ldrb	w1, [x19, 2865]
	cmp	w1, 1
	ccmp	w0, wzr, 0, ne
	beq	.L655
	ldr	x1, [x20, 80]
	mov	w2, 1
	ldr	w0, [x21, 4]
	ldr	w1, [x1, 4]
	bl	FSP_SetRef
	ldr	x0, [x20, 104]
	add	x1, x19, 2656
	cmp	x0, x1
	beq	.L656
	ldr	w1, [x0, 4]
	mov	w2, 0
	ldr	w0, [x21, 4]
	bl	FSP_SetRef
.L656:
	ldr	x2, [x19, 2736]
	ldr	x3, [x19, 2744]
	ldr	x1, [x19, 2728]
	str	x1, [x19, 2736]
	str	x2, [x19, 2744]
	str	x3, [x19, 2752]
.L655:
	ldr	x0, [x21, 16]
	ldr	w0, [x0, 12]
	cbnz	w0, .L657
	ldr	x20, [x19, 2744]
	b	.L658
.L657:
	ldr	x20, [x19, 2728]
	b	.L658
.L701:
	ldr	w0, [x21, 4]
	bl	FreeUsdByDec
	str	xzr, [x22, 560]
	b	.L662
.L703:
	ldr	w0, [x21, 4]
	bl	FreeUsdByDec
	str	xzr, [x22, 560]
	b	.L664
.L698:
	ldr	x1, .LC276
	mov	w2, 3300
	mov	w19, 0
	bl	dprint_vfmw
	b	.L646
.L700:
	ldr	x1, .LC276
	mov	w2, 3340
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
	b	.L646
.L704:
	ldr	x1, .LC276
	mov	w2, 3275
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
	b	.L646
.L705:
	ldr	x1, .LC276
	mov	w2, 3279
	mov	w19, 0
	bl	dprint_vfmw
	b	.L646
	.size	AVSDEC_VDMPostProc, .-AVSDEC_VDMPostProc
	.align	3
.LC275:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC276:
	.xword	.LC248
	.align	3
.LC278:
	.xword	.LC277
	.align	3
.LC280:
	.xword	.LC279
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_ClearCurPic
	.type	AVS_ClearCurPic, %function
AVS_ClearCurPic:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 32
	mov	w19, 0
	bl	_mcount
	ldr	w0, [x20, 14440]
	cmp	w0, wzr
	ble	.L709
	.p2align 2
.L710:
	sxtw	x2, w19
	ldr	w0, [x21, 4]
	lsl	x1, x2, 3
	add	w19, w19, 1
	sub	x1, x1, x2
	add	x1, x20, x1, lsl 2
	add	x1, x1, 104
	bl	ClearSlice
	ldr	w0, [x20, 14440]
	cmp	w0, w19
	bgt	.L710
.L709:
	str	wzr, [x20, 14440]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	AVS_ClearCurPic, .-AVS_ClearCurPic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ClearPacket
	.type	ClearPacket, %function
ClearPacket:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 16384
	bl	_mcount
	ldr	w0, [x20, 4]
	ldr	w1, [x19, 456]
	bl	SM_ReleaseStreamSeg
	ldrb	w0, [x19, 442]
	cbnz	w0, .L714
	ldrb	w0, [x19, 530]
	cmp	w0, 1
	beq	.L718
.L714:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L718:
	ldr	w0, [x19, 536]
	cbz	w0, .L714
	ldr	w1, [x19, 544]
	ldr	w0, [x20, 4]
	bl	SM_ReleaseStreamSeg
	b	.L714
	.size	ClearPacket, .-ClearPacket
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	WriteAvsSliceMsg
	.type	WriteAvsSliceMsg, %function
WriteAvsSliceMsg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x6, x19, 32
	ldr	w2, [x6, 14440]
	cmp	w2, 511
	bgt	.L727
	cmp	w2, wzr
	ble	.L722
	sub	w0, w2, #1
	add	x5, x19, 12288
	ldr	x8, [x19, 16816]
	sxtw	x0, w0
	ldr	w4, [x5, 3560]
	lsl	x1, x0, 3
	sub	x0, x1, x0
	add	w4, w4, 15
	ldrb	w7, [x8, 3]
	lsr	w4, w4, 4
	add	x0, x6, x0, lsl 2
	mul	w7, w7, w4
	ldr	w3, [x0, 120]
	cmp	w7, w3
	bls	.L728
.L723:
	sxtw	x1, w2
	add	x19, x19, 16384
	lsl	x0, x1, 3
	add	x5, x5, 2788
	sub	x1, x0, x1
	mov	w0, 0
	ldr	w3, [x19, 448]
	add	x1, x6, x1, lsl 2
	str	w3, [x1, 104]
	ldr	w3, [x19, 456]
	str	w3, [x1, 124]
	ldr	w3, [x19, 452]
	str	w3, [x1, 112]
	ldrb	w7, [x8, 3]
	mul	w7, w7, w4
	str	w7, [x1, 120]
	ldr	w3, [x5, 24]
	ldr	w5, [x5, 28]
	add	w3, w3, 15
	add	w5, w5, 15
	lsr	w4, w3, 4
	lsr	w3, w5, 4
	mul	w3, w4, w3
	sub	w3, w3, #1
	cmp	w7, w3
	bhi	.L721
	ldrb	w0, [x19, 442]
	cmp	w0, 1
	beq	.L729
	ldr	w0, [x19, 536]
	str	w0, [x1, 108]
	ldr	w0, [x19, 544]
	str	w0, [x1, 128]
	ldr	w0, [x19, 540]
	str	w0, [x1, 116]
.L725:
	add	w2, w2, 1
	mov	w0, 1
	str	w2, [x6, 14440]
.L721:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L722:
	ldr	w4, [x19, 15848]
	add	x5, x19, 12288
	ldr	x8, [x19, 16816]
	add	w4, w4, 15
	lsr	w4, w4, 4
	b	.L723
	.p2align 3
.L727:
	ldr	x1, .LC282
	mov	w3, 512
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L728:
	ldr	x1, .LC284
	mov	w2, w7
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	ClearPacket
	ldr	x19, [sp, 16]
	mov	w0, 1
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L729:
	str	wzr, [x1, 108]
	str	wzr, [x1, 116]
	str	wzr, [x1, 128]
	b	.L725
	.size	WriteAvsSliceMsg, .-WriteAvsSliceMsg
	.align	3
.LC282:
	.xword	.LC281
	.align	3
.LC284:
	.xword	.LC283
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_GetPacketState
	.type	AVS_GetPacketState, %function
AVS_GetPacketState:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 16384
	bl	_mcount
	ldr	w0, [x19, 424]
	ldr	x19, [sp, 16]
	cmp	w0, 1
	cset	w0, ne
	ldp	x29, x30, [sp], 32
	ret
	.size	AVS_GetPacketState, .-AVS_GetPacketState
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Avs_Vfmw_WQMatrix
	.type	Avs_Vfmw_WQMatrix, %function
Avs_Vfmw_WQMatrix:
	stp	x29, x30, [sp, -352]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	str	xzr, [x29, 40]
	ldr	w8, [x19, 144]
	str	wzr, [x29, 48]
	cmp	w8, 1
	str	xzr, [x29, 56]
	str	wzr, [x29, 64]
	ldr	w6, [x19, 128]
	beq	.L733
	cmp	w8, 3
	ccmp	w6, wzr, 0, eq
	beq	.L733
	cmp	w8, 2
	beq	.L804
.L737:
	cbz	w6, .L739
	mov	w0, 128
	strh	w0, [x29, 72]
	strh	w0, [x29, 74]
	strh	w0, [x29, 76]
	strh	w0, [x29, 78]
	strh	w0, [x29, 80]
	strh	w0, [x29, 82]
	strh	w0, [x29, 84]
	strh	w0, [x29, 86]
	strh	w0, [x29, 88]
	strh	w0, [x29, 90]
	strh	w0, [x29, 92]
	strh	w0, [x29, 94]
	cbnz	w8, .L805
	ldr	x3, .LC285
	add	x1, x29, 84
	add	x0, x3, 12
	add	x3, x3, 24
	.p2align 2
.L753:
	ldrh	w2, [x0], 2
	strh	w2, [x1], 2
	cmp	x0, x3
	bne	.L753
.L748:
	add	x3, x19, 200
	add	x2, x19, 328
	mov	x0, x3
	mov	w1, 128
	.p2align 2
.L745:
	strh	w1, [x0], 2
	cmp	x0, x2
	bne	.L745
	ldr	w0, [x19, 124]
	cbz	w0, .L757
	ldr	x6, .LC286
	mov	w9, 0
	ldr	w0, [x19, 148]
	add	x6, x6, 16
	add	x6, x6, x0, lsl 6
.L758:
	uxtw	x5, w9
	mov	w1, 0
	lsl	x0, x5, 2
	lsl	x7, x5, 6
	sub	x5, x0, x5
	lsl	x5, x5, 1
	.p2align 2
.L766:
	add	w4, w1, 8
	.p2align 2
.L764:
	uxtw	x0, w1
	add	x10, x29, 352
	add	x2, x7, x0
	add	w1, w1, 1
	cmp	w1, w4
	ldrb	w0, [x6, x0]
	add	x2, x10, x2, lsl 1
	add	x0, x5, x0
	add	x0, x10, x0, lsl 1
	sub	x0, x0, #4096
	ldrh	w0, [x0, 3816]
	strh	w0, [x2, -256]
	bne	.L764
	cmp	w1, 64
	bne	.L766
	add	w9, w9, 1
	cmp	w9, 2
	bne	.L758
	cbnz	w8, .L767
.L809:
	add	x0, x29, 224
	add	x2, x29, 352
	.p2align 2
.L768:
	ldrh	w1, [x0], 2
	strh	w1, [x3], 2
	cmp	x2, x0
	bne	.L768
.L731:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 352
	ret
.L739:
	ldr	x0, .LC285
	mov	x1, 0
	add	x4, x29, 72
	.p2align 2
.L742:
	ldrh	w2, [x0, x1]
	strh	w2, [x4, x1]
	add	x1, x1, 2
	cmp	x1, 12
	bne	.L742
	add	x6, x4, 12
	add	x1, x0, 12
	mov	x2, x6
	add	x0, x0, 24
	.p2align 2
.L743:
	ldrh	w3, [x1], 2
	strh	w3, [x2], 2
	cmp	x1, x0
	bne	.L743
	cbz	w8, .L748
	cmp	w8, 1
	beq	.L806
	cmp	w8, 2
	beq	.L807
	cmp	w8, 3
	bne	.L748
	mov	x0, 0
	add	x5, x29, 40
.L751:
	ldrh	w1, [x5, x0]
	strh	w1, [x4, x0]
	add	x0, x0, 2
	cmp	x0, 12
	bne	.L751
	mov	x0, 0
	add	x5, x29, 56
.L752:
	ldrh	w1, [x5, x0]
	add	x0, x0, 2
	strh	w1, [x6], 2
	cmp	x0, 12
	bne	.L752
	b	.L748
.L805:
	cmp	w8, 1
	beq	.L808
	cmp	w8, 2
	bne	.L748
	add	x1, x29, 84
	mov	x0, 0
	add	x5, x29, 56
.L756:
	ldrh	w2, [x5, x0]
	add	x0, x0, 2
	strh	w2, [x1], 2
	cmp	x0, 12
	bne	.L756
	b	.L748
	.p2align 3
.L757:
	mov	w6, 0
	mov	w5, 128
.L761:
	mov	w0, 0
	ubfiz	x4, x6, 6, 32
	.p2align 2
.L763:
	add	w2, w0, 8
	.p2align 2
.L759:
	add	x1, x4, x0, uxtw
	add	x7, x29, 352
	add	w0, w0, 1
	add	x1, x7, x1, lsl 1
	cmp	w2, w0
	strh	w5, [x1, -256]
	bne	.L759
	cmp	w2, 64
	mov	w0, w2
	bne	.L763
	add	w6, w6, 1
	cmp	w6, 2
	bne	.L761
	cbz	w8, .L809
.L767:
	cmp	w8, 1
	beq	.L810
	cmp	w8, 2
	bne	.L731
	add	x0, x29, 224
	add	x2, x29, 352
	.p2align 2
.L772:
	ldrh	w1, [x0], 2
	strh	w1, [x3], 2
	cmp	x2, x0
	bne	.L772
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 352
	ret
.L733:
	ldr	x0, .LC285
	add	x3, x19, 152
	mov	x1, 0
	add	x5, x29, 40
	.p2align 2
.L735:
	ldrh	w2, [x0, x1]
	ldr	w4, [x3], 4
	add	w2, w4, w2
	strh	w2, [x5, x1]
	add	x1, x1, 2
	cmp	x1, 12
	bne	.L735
	cmp	w8, 2
	beq	.L736
	cmp	w8, 3
	ccmp	w6, wzr, 0, eq
	bne	.L737
.L736:
	add	x3, x19, 176
	add	x0, x0, 12
	mov	x1, 0
	add	x5, x29, 56
	.p2align 2
.L738:
	ldr	w4, [x3], 4
	ldrh	w2, [x0], 2
	add	w2, w4, w2
	strh	w2, [x5, x1]
	add	x1, x1, 2
	cmp	x1, 12
	bne	.L738
	b	.L737
.L804:
	ldr	x0, .LC285
	b	.L736
.L808:
	mov	x0, 0
	add	x5, x29, 40
	add	x4, x29, 72
.L755:
	ldrh	w1, [x5, x0]
	strh	w1, [x4, x0]
	add	x0, x0, 2
	cmp	x0, 12
	bne	.L755
	b	.L748
.L806:
	mov	x0, 0
	add	x5, x29, 40
.L747:
	ldrh	w1, [x5, x0]
	strh	w1, [x4, x0]
	add	x0, x0, 2
	cmp	x0, 12
	bne	.L747
	b	.L748
.L810:
	add	x0, x29, 96
	add	x2, x0, 128
	.p2align 2
.L770:
	ldrh	w1, [x0], 2
	strh	w1, [x3], 2
	cmp	x2, x0
	bne	.L770
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 352
	ret
.L807:
	mov	x0, 0
	add	x5, x29, 56
.L750:
	ldrh	w1, [x5, x0]
	add	x0, x0, 2
	strh	w1, [x6], 2
	cmp	x0, 12
	bne	.L750
	b	.L748
	.size	Avs_Vfmw_WQMatrix, .-Avs_Vfmw_WQMatrix
	.align	3
.LC285:
	.xword	.LANCHOR1
	.align	3
.LC286:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVS_ParseHeader
	.type	AVS_ParseHeader, %function
AVS_ParseHeader:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 12288
	add	x21, x20, 16384
	add	x23, x19, 2648
	bl	_mcount
	mov	w1, 32
	add	x24, x19, 3536
	ldr	x22, [x23, 512]
	str	wzr, [x21, 424]
	mov	x0, x22
	bl	BsShow
	uxtb	w0, w0
	sub	w1, w0, #176
	cmp	w1, 78
	bls	.L861
.L812:
	cmp	w0, 175
	bhi	.L836
	ldr	w1, [x19, 2772]
	mov	w0, 327680
	add	x19, x19, 2648
	and	w0, w1, w0
	cmp	w0, 327680
	beq	.L862
.L836:
	mov	x0, x20
	bl	ClearPacket
.L853:
	mov	w0, 1
.L824:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L861:
	ldr	x2, .LC289
	ldrb	w1, [x2,w1,uxtw]
	adr	x2, .Lrtx814
	add	x1, x2, w1, sxtb #2
	br	x1
.Lrtx814:
	.section	.rodata
	.align	0
	.align	2
.L814:
	.byte	(.L813 - .Lrtx814) / 4
	.byte	(.L815 - .Lrtx814) / 4
	.byte	(.L816 - .Lrtx814) / 4
	.byte	(.L817 - .Lrtx814) / 4
	.byte	(.L836 - .Lrtx814) / 4
	.byte	(.L819 - .Lrtx814) / 4
	.byte	(.L820 - .Lrtx814) / 4
	.byte	(.L815 - .Lrtx814) / 4
	.byte	(.L836 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L812 - .Lrtx814) / 4
	.byte	(.L822 - .Lrtx814) / 4
	.text
	.p2align 3
.L815:
	add	x19, x19, 2648
	ldr	w1, [x19, 124]
	tbnz	x1, 15, .L863
	mov	w0, 327680
	tst	w1, w0
	beq	.L853
	mov	w2, 16383
	mov	x0, x20
	movk	w2, 0xfffb, lsl 16
	and	w1, w1, w2
	str	w1, [x19, 124]
	bl	ClearPacket
	mov	w0, 0
	b	.L824
	.p2align 3
.L863:
	mov	w0, 1
	str	w0, [x21, 424]
	orr	w1, w1, 16384
	str	w1, [x19, 124]
	b	.L824
	.p2align 3
.L813:
	add	x19, x19, 2648
	ldr	w2, [x19, 124]
	tbnz	x2, 15, .L864
	tbz	x2, 16, .L825
	mov	w0, 16383
	mov	x1, x24
	movk	w0, 0xfffb, lsl 16
	and	w2, w2, w0
	mov	x0, x20
	str	w2, [x19, 124]
	bl	AVS_sequence_header
	mov	x0, x20
	bl	ClearPacket
	mov	w0, 0
	b	.L824
	.p2align 3
.L822:
	mov	w1, 32
	mov	x0, x22
	bl	BsSkip
	mov	w1, 32
	mov	x0, x22
	bl	BsGet
	mov	w23, w0
	mov	w1, 32
	mov	x0, x22
	bl	BsGet
	mov	w2, 20553
	mov	w1, 20036
	movk	w2, 0x4853, lsl 16
	movk	w1, 0x4345, lsl 16
	cmp	w23, w2
	ccmp	w0, w1, 0, eq
	bne	.L857
	add	x2, x19, 2648
	mov	w0, 1
	strb	w0, [x20]
	mov	w19, 1
	str	w19, [x21, 424]
	mov	x0, x20
	ldr	w1, [x2, 124]
	orr	w1, w1, 16384
	str	w1, [x2, 124]
	bl	ClearPacket
	mov	w0, w19
	b	.L824
	.p2align 3
.L816:
	ldr	w0, [x19, 2772]
	mov	w1, 1
	tbnz	x0, 18, .L854
	tbz	x0, 16, .L857
	mov	w1, 0
.L854:
	mov	x0, x20
	bl	AVS_user_data
	mov	x0, x20
	bl	ClearPacket
	mov	w0, 1
	b	.L824
	.p2align 3
.L819:
	ldr	w1, [x19, 2772]
	mov	w0, 327680
	tst	w1, w0
	beq	.L857
	mov	x1, x24
	mov	x0, x23
	bl	AVS_extension
	mov	x0, x20
	bl	ClearPacket
	mov	w0, 1
	b	.L824
	.p2align 3
.L817:
	add	x19, x19, 2648
	ldr	w0, [x19, 124]
	tbnz	x0, 15, .L852
	tbz	x0, 18, .L827
	mov	w2, 16383
	mov	x1, x24
	movk	w2, 0xfffb, lsl 16
	and	w2, w0, w2
	mov	x0, x23
	str	w2, [x19, 124]
	bl	AVS_i_picture_header
	b	.L856
	.p2align 3
.L820:
	add	x19, x19, 2648
	ldr	w0, [x19, 124]
	tbnz	x0, 15, .L852
	tbz	x0, 18, .L829
	mov	w2, 16383
	mov	x1, x24
	movk	w2, 0xfffb, lsl 16
	and	w2, w0, w2
	mov	x0, x23
	str	w2, [x19, 124]
	bl	AVS_pb_picture_header
.L856:
	mov	x0, x24
	bl	Avs_Vfmw_WQMatrix
.L857:
	mov	x0, x20
	bl	ClearPacket
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L829:
	mov	x1, x24
	mov	x0, x23
	bl	AVS_pb_picture_header
.L858:
	mov	x0, x24
	bl	Avs_Vfmw_WQMatrix
	mov	x0, x20
	bl	ClearPacket
	mov	w0, 1
	b	.L824
	.p2align 3
.L827:
	mov	x1, x24
	mov	x0, x23
	bl	AVS_i_picture_header
	b	.L858
	.p2align 3
.L825:
	mov	x1, x24
	mov	x0, x20
	bl	AVS_sequence_header
	mov	x0, x20
	bl	ClearPacket
	mov	w0, 1
	b	.L824
	.p2align 3
.L852:
	mov	w20, 1
	str	w20, [x21, 424]
	orr	w1, w0, 16384
	mov	x0, x24
	str	w1, [x19, 124]
	bl	Avs_Vfmw_WQMatrix
	mov	w0, w20
	b	.L824
	.p2align 3
.L864:
	mov	w0, 1
	str	w0, [x21, 424]
	orr	w2, w2, 16384
	str	w2, [x19, 124]
	b	.L824
.L862:
	orr	w1, w1, 32768
	mov	x0, x20
	str	w1, [x19, 124]
	bl	WriteAvsSliceMsg
	cbnz	w0, .L853
	b	.L857
	.size	AVS_ParseHeader, .-AVS_ParseHeader
	.align	3
.LC289:
	.xword	.L814
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVSDEC_Decode
	.type	AVSDEC_Decode, %function
AVSDEC_Decode:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x19, x1
	mov	x0, x30
	add	x22, x21, 12288
	add	x20, x22, 2648
	bl	_mcount
	ldr	w2, [x19, 20]
	add	x0, x22, 3496
	ldr	x1, [x19]
	bl	BsInit
	ldr	x3, .LC290
	add	x0, x21, 16384
	mov	x2, 176
	mov	x1, x19
	add	x0, x0, 432
	ldr	x3, [x3, 104]
	blr	x3
	ldr	w0, [x20, 124]
	str	w0, [x20, 844]
	mov	x0, x21
	bl	AVS_ParseHeader
	cbz	w0, .L888
	mov	w0, 49152
	ldr	w1, [x20, 124]
	movk	w0, 0x5, lsl 16
	and	w0, w1, w0
	cmp	w0, 376832
	beq	.L871
.L872:
	mov	w2, 0
.L868:
	mov	w0, w2
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L888:
	ldr	x1, .LC292
	add	x20, x21, 32
	mov	w0, 1
	mov	w19, 0
	bl	dprint_vfmw
	ldr	w0, [x20, 14440]
	cmp	w0, wzr
	ble	.L870
	.p2align 2
.L881:
	sxtw	x2, w19
	ldr	w0, [x21, 4]
	lsl	x1, x2, 3
	add	w19, w19, 1
	sub	x1, x1, x2
	add	x1, x20, x1, lsl 2
	add	x1, x1, 104
	bl	ClearSlice
	ldr	w0, [x20, 14440]
	cmp	w19, w0
	blt	.L881
.L870:
	mov	w2, 0
	str	wzr, [x20, 14440]
.L890:
	mov	w0, w2
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L871:
	mov	x0, x21
	bl	AVS_DecodeArbiter.part.3
	cbz	w0, .L872
	mov	x0, x21
	bl	AVS_GetStreamInf
	cmp	w0, 3
	beq	.L873
	cbnz	w0, .L889
	ldr	x1, .LC294
	add	x20, x21, 32
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x20, 14440]
	cmp	w0, wzr
	ble	.L870
	mov	w19, 0
	.p2align 2
.L876:
	sxtw	x2, w19
	ldr	w0, [x21, 4]
	lsl	x1, x2, 3
	add	w19, w19, 1
	sub	x1, x1, x2
	add	x1, x20, x1, lsl 2
	add	x1, x1, 104
	bl	ClearSlice
	ldr	w0, [x20, 14440]
	cmp	w19, w0
	blt	.L876
	mov	w2, 0
	str	wzr, [x20, 14440]
	b	.L890
	.p2align 3
.L889:
	mov	x0, x21
	bl	AVS_StartDecode
	mov	w2, 1
	cbnz	w0, .L868
	ldr	x1, .LC296
	add	x20, x21, 32
	mov	w0, w2
	bl	dprint_vfmw
	ldr	w0, [x20, 14440]
	cmp	w0, wzr
	ble	.L870
	mov	w19, 0
	.p2align 2
.L877:
	sxtw	x2, w19
	ldr	w0, [x21, 4]
	lsl	x1, x2, 3
	add	w19, w19, 1
	sub	x1, x1, x2
	add	x1, x20, x1, lsl 2
	add	x1, x1, 104
	bl	ClearSlice
	ldr	w0, [x20, 14440]
	cmp	w19, w0
	blt	.L877
	b	.L870
	.p2align 3
.L873:
	ldr	w1, [x20, 844]
	mov	w2, w0
	str	w1, [x20, 124]
	b	.L868
	.size	AVSDEC_Decode, .-AVSDEC_Decode
	.align	3
.LC290:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC292:
	.xword	.LC291
	.align	3
.LC294:
	.xword	.LC293
	.align	3
.LC296:
	.xword	.LC295
	.global	g_tmp
	.global	wq_param_default
	.global	g_AvsCfg
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	CSWTCH.56, %object
	.size	CSWTCH.56, 16
CSWTCH.56:
	.word	5
	.word	1
	.word	2
	.word	3
	.type	WeightQuantModel, %object
	.size	WeightQuantModel, 256
WeightQuantModel:
	.byte	0
	.byte	0
	.byte	0
	.byte	4
	.byte	4
	.byte	4
	.byte	5
	.byte	5
	.byte	0
	.byte	0
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	5
	.byte	5
	.byte	0
	.byte	3
	.byte	2
	.byte	2
	.byte	1
	.byte	1
	.byte	5
	.byte	5
	.byte	4
	.byte	3
	.byte	2
	.byte	2
	.byte	1
	.byte	5
	.byte	5
	.byte	5
	.byte	4
	.byte	3
	.byte	1
	.byte	1
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	4
	.byte	3
	.byte	1
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	0
	.byte	0
	.byte	0
	.byte	4
	.byte	4
	.byte	4
	.byte	5
	.byte	5
	.byte	0
	.byte	0
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	5
	.byte	5
	.byte	0
	.byte	3
	.byte	2
	.byte	2
	.byte	2
	.byte	1
	.byte	5
	.byte	5
	.byte	3
	.byte	3
	.byte	2
	.byte	2
	.byte	1
	.byte	5
	.byte	5
	.byte	5
	.byte	3
	.byte	3
	.byte	2
	.byte	1
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	3
	.byte	3
	.byte	1
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	0
	.byte	0
	.byte	0
	.byte	4
	.byte	4
	.byte	3
	.byte	5
	.byte	5
	.byte	0
	.byte	0
	.byte	4
	.byte	4
	.byte	3
	.byte	2
	.byte	5
	.byte	5
	.byte	0
	.byte	4
	.byte	4
	.byte	3
	.byte	2
	.byte	1
	.byte	5
	.byte	5
	.byte	4
	.byte	4
	.byte	3
	.byte	2
	.byte	1
	.byte	5
	.byte	5
	.byte	5
	.byte	4
	.byte	3
	.byte	2
	.byte	1
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	3
	.byte	2
	.byte	1
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	0
	.byte	0
	.byte	0
	.byte	3
	.byte	2
	.byte	1
	.byte	5
	.byte	5
	.byte	0
	.byte	0
	.byte	4
	.byte	3
	.byte	2
	.byte	1
	.byte	5
	.byte	5
	.byte	0
	.byte	4
	.byte	4
	.byte	3
	.byte	2
	.byte	1
	.byte	5
	.byte	5
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	2
	.byte	5
	.byte	5
	.byte	5
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	1
	.byte	1
	.byte	1
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.data
	.align	3
.LANCHOR1 = . + 0
	.type	wq_param_default, %object
	.size	wq_param_default, 24
wq_param_default:
	.hword	135
	.hword	143
	.hword	143
	.hword	160
	.hword	160
	.hword	213
	.hword	128
	.hword	98
	.hword	106
	.hword	116
	.hword	116
	.hword	128
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC0:
	ASCII(.string	"%-40s     %#-8x(%db)\n" )
	.zero	2
.LC4:
	ASCII(.string	"video_sequence_start_code" )
	.zero	6
.LC6:
	ASCII(.string	"profile_id" )
	.zero	5
.LC8:
	ASCII(.string	"level_id" )
	.zero	7
.LC10:
	ASCII(.string	"progressive_sequence" )
	.zero	3
.LC12:
	ASCII(.string	"horizontal_size" )
.LC14:
	ASCII(.string	"vertical_size" )
	.zero	2
.LC16:
	ASCII(.string	"chroma_format" )
	.zero	2
.LC18:
	ASCII(.string	"sample_precision" )
	.zero	7
.LC20:
	ASCII(.string	"aspect_ratio" )
	.zero	3
.LC22:
	ASCII(.string	"frame_rate_code" )
.LC24:
	ASCII(.string	"bit_rate_lower" )
	.zero	1
.LC26:
	ASCII(.string	"marker_bit" )
	.zero	5
.LC28:
	ASCII(.string	"bit_rate_upper" )
	.zero	1
.LC30:
	ASCII(.string	"low_delay" )
	.zero	6
.LC32:
	ASCII(.string	"bbv_buffer_size" )
.LC34:
	ASCII(.string	"reserved_bits" )
	.zero	2
.LC40:
	ASCII(.string	"i_picture_start_code" )
	.zero	3
.LC42:
	ASCII(.string	"bbv_delay" )
	.zero	6
.LC45:
	ASCII(.string	"bbv_delay_extension" )
	.zero	4
.LC47:
	ASCII(.string	"time_code_flag" )
	.zero	1
.LC49:
	ASCII(.string	"time_code" )
	.zero	6
.LC51:
	ASCII(.string	"picture_distance" )
	.zero	7
.LC53:
	ASCII(.string	"bbv_check_times" )
.LC55:
	ASCII(.string	"progressive_frame" )
	.zero	6
.LC57:
	ASCII(.string	"picture_structure" )
	.zero	6
.LC59:
	ASCII(.string	"top_field_first" )
.LC61:
	ASCII(.string	"repeat_first_field" )
	.zero	5
.LC63:
	ASCII(.string	"fixed_picture_qp" )
	.zero	7
.LC65:
	ASCII(.string	"picture_qp" )
	.zero	5
.LC67:
	ASCII(.string	"skip_mode_flag" )
	.zero	1
.LC70:
	ASCII(.string	"loop_filter_disable" )
	.zero	4
.LC72:
	ASCII(.string	"loop_filter_parameter_flag" )
	.zero	5
.LC74:
	ASCII(.string	"alpha_c_offset" )
	.zero	1
.LC76:
	ASCII(.string	"beta_offset" )
	.zero	4
.LC78:
	ASCII(.string	"weighting_quant_flag" )
	.zero	3
.LC80:
	ASCII(.string	"aec_enable" )
	.zero	5
.LC82:
	ASCII(.string	"mb_adapt_weighting_quant_disable" )
	.zero	7
.LC84:
	ASCII(.string	"chroma_quant_param_disable" )
	.zero	5
.LC86:
	ASCII(.string	"chroma_quant_param_delta_u" )
	.zero	5
.LC88:
	ASCII(.string	"chroma_quant_param_delta_v" )
	.zero	5
.LC90:
	ASCII(.string	"weighting_quant_param_index" )
	.zero	4
.LC92:
	ASCII(.string	"weighting_quant_model" )
	.zero	2
.LC94:
	ASCII(.string	"weighting_quant_param_deltal1" )
	.zero	2
.LC96:
	ASCII(.string	"weighting_quant_param_deltal2" )
	.zero	2
.LC99:
	ASCII(.string	"pb_picture_start_code" )
	.zero	2
.LC104:
	ASCII(.string	"picture_coding_type" )
	.zero	4
.LC110:
	ASCII(.string	"advanced_pred_mode_disable" )
	.zero	5
.LC116:
	ASCII(.string	"picture_reference_flag" )
	.zero	1
.LC118:
	ASCII(.string	"no_forward_reference_flag" )
	.zero	6
.LC120:
	ASCII(.string	"b_filed_enhanced_flag" )
	.zero	2
.LC122:
	ASCII(.string	"p_filed_enhanced_flag" )
	.zero	2
.LC140:
	ASCII(.string	"user_data_start_code" )
	.zero	3
.LC144:
	ASCII(.string	"extension_ID" )
	.zero	3
.LC146:
	ASCII(.string	"video_format" )
	.zero	3
.LC148:
	ASCII(.string	"sample_range" )
	.zero	3
.LC150:
	ASCII(.string	"colour_description" )
	.zero	5
.LC152:
	ASCII(.string	"colour_primaries" )
	.zero	7
.LC154:
	ASCII(.string	"transfer_characteristics" )
	.zero	7
.LC156:
	ASCII(.string	"matrix_coefficients" )
	.zero	4
.LC158:
	ASCII(.string	"display_horizontal_size" )
.LC161:
	ASCII(.string	"display_vertical_size" )
	.zero	2
.LC165:
	ASCII(.string	"copyright_flag" )
	.zero	1
.LC167:
	ASCII(.string	"copyright_id" )
	.zero	3
.LC169:
	ASCII(.string	"original_or_copy" )
	.zero	7
.LC173:
	ASCII(.string	"copyright_number_1" )
	.zero	5
.LC175:
	ASCII(.string	"copyright_number_2" )
	.zero	5
.LC177:
	ASCII(.string	"copyright_number_3" )
	.zero	5
.LC181:
	ASCII(.string	"camera_id" )
	.zero	6
.LC184:
	ASCII(.string	"height_of_image_device" )
	.zero	1
.LC186:
	ASCII(.string	"focal_length" )
	.zero	3
.LC188:
	ASCII(.string	"f_number" )
	.zero	7
.LC190:
	ASCII(.string	"vertical_angle_of_view" )
	.zero	1
.LC192:
	ASCII(.string	"camera_position_x_upper" )
.LC194:
	ASCII(.string	"camera_position_x_lower" )
.LC196:
	ASCII(.string	"camera_position_y_upper" )
.LC198:
	ASCII(.string	"camera_position_y_lower" )
.LC200:
	ASCII(.string	"camera_position_z_upper" )
.LC202:
	ASCII(.string	"camera_position_z_lower" )
.LC204:
	ASCII(.string	"camera_direction_x" )
	.zero	5
.LC206:
	ASCII(.string	"camera_direction_y" )
	.zero	5
.LC208:
	ASCII(.string	"camera_direction_z" )
	.zero	5
.LC210:
	ASCII(.string	"image_plane_vertical_x" )
	.zero	1
.LC212:
	ASCII(.string	"image_plane_vertical_y" )
	.zero	1
.LC214:
	ASCII(.string	"image_plane_vertical_z" )
	.zero	1
.LC217:
	ASCII(.string	"frame_centre_horizontal_offset" )
	.zero	1
.LC220:
	ASCII(.string	"frame_centre_vertical_offset" )
	.zero	3
.LC222:
	ASCII(.string	"extension_start_code" )
	.zero	3
.LC225:
	ASCII(.string	"ERROR: image size abnormal(%dx%d)\n" )
	.zero	5
.LC227:
	ASCII(.string	"avs actual frame size(%dx%d) exeed max config(%dx%d)\n" )
	.zero	2
.LC230:
	ASCII(.string	"ERROR: partition fs memory fail!\n" )
	.zero	6
.LC232:
	ASCII(.string	"ERROR: FSP_ConfigInstance fail!\n" )
	.zero	7
.LC236:
	ASCII(.string	"line: %d, pImage is NULL!\n" )
	.zero	5
.LC239:
	ASCII(.string	"line: %d pImage is null\n" )
	.zero	7
.LC241:
	ASCII(.string	"ERROR: get frame store fail!\n" )
	.zero	2
.LC244:
	ASCII(.string	"get image buffer ok: LogicFsID = %d\n" )
	.zero	3
.LC246:
	ASCII(.string	"decode %p, disp %p, tf %p\n" )
	.zero	5
.LC248:
	ASCII(.string	"line: %d\n" )
	.zero	6
.LC250:
	ASCII(.string	"ERROR: AVS_OutputOneFrame is err(%d)\n" )
	.zero	2
.LC254:
	ASCII(.string	"size change\n" )
	.zero	3
.LC256:
	ASCII(.string	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d,new w/h = %d/%d\n" )
	.zero	3
.LC259:
	ASCII(.string	"MbWidth = %d, MbHeight = %d, don't support!\n" )
	.zero	3
.LC261:
	ASCII(.string	"ERROR: allocate vhb\n" )
	.zero	3
.LC263:
	ASCII(.string	"ERROR: get img buf err\n" )
.LC266:
	ASCII(.string	"dec_pts: %lld Usertag = %lld\n" )
	.zero	2
.LC269:
	ASCII(.string	"-1 == VCTRL_GetChanIDByCtx() Err! \n" )
	.zero	4
.LC273:
	ASCII(.string	"-----Pic %5d, type %d, struct %d-----\n" )
	.zero	1
.LC277:
	ASCII(.string	"ERROR: err_level(%d) over ref_thr(%d)\n" )
	.zero	1
.LC279:
	ASCII(.string	"ERROR: err_level(%d) over out_thr(%d)\n" )
	.zero	1
.LC281:
	ASCII(.string	"SlcNum %d > %d Write slice msg failed\n" )
	.zero	1
.LC283:
	ASCII(.string	"This slice's start mbn %d <= LastSliceFirstMb %d, Write slice msg failed\n" )
	.zero	6
.LC291:
	ASCII(.string	"ERROR: paseHeader err\n" )
	.zero	1
.LC293:
	ASCII(.string	"ERROR: AVS_PrepareDecode err\n" )
	.zero	2
.LC295:
	ASCII(.string	"ERROR: AVS_StartDecode err\n" )
	.bss
	.align	3
	.type	g_tmp, %object
	.size	g_tmp, 1024
g_tmp:
	.zero	1024
	.type	g_AvsCfg, %object
	.size	g_AvsCfg, 508
g_AvsCfg:
	.zero	508
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
