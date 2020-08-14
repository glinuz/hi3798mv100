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
	.file	"avs.c"
	.text
	.align	2
	.global	avs_u_v
	.type	avs_u_v, %function
avs_u_v:
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
	mov	r5, r1
	mov	r6, r2
	bl	BsGet
	ldr	ip, .L2
	mov	r2, r6
	str	r5, [sp]
	ldr	r1, .L2+4
	ldr	ip, [ip, #68]
	mov	r4, r0
	mov	r3, r0
	mov	r0, #7
	blx	ip
	mov	r0, r4
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3:
	.align	2
.L2:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	UNWIND(.fnend)
	.size	avs_u_v, .-avs_u_v
	.align	2
	.global	avs_f_v
	.type	avs_f_v, %function
avs_f_v:
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
	mov	r5, r1
	mov	r6, r2
	bl	BsGet
	ldr	ip, .L5
	mov	r2, r6
	str	r5, [sp]
	ldr	r1, .L5+4
	ldr	ip, [ip, #68]
	mov	r4, r0
	mov	r3, r0
	mov	r0, #7
	blx	ip
	mov	r0, r4
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L6:
	.align	2
.L5:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	UNWIND(.fnend)
	.size	avs_f_v, .-avs_f_v
	.align	2
	.global	avs_ue_v
	.type	avs_ue_v, %function
avs_ue_v:
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
	mov	r6, r1
	mov	r1, #32
	mov	r7, r0
	bl	BsShow
	bl	ZerosMS_32
	cmp	r0, #31
	mov	r4, r0
	mvnhi	r5, #0
	movhi	r4, #32
	bls	.L11
.L8:
	ldr	ip, .L12
	mov	r2, r6
	str	r4, [sp]
	mov	r3, r5
	ldr	r1, .L12+4
	mov	r0, #7
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L11:
	add	r5, r4, #1
	mov	r1, r4
	mov	r0, r7
	mov	r4, r4, asl #1
	bl	BsSkip
	mov	r1, r5
	mov	r0, r7
	add	r4, r4, #1
	bl	BsShow
	mov	r1, r5
	sub	r5, r0, #1
	mov	r0, r7
	bl	BsSkip
	b	.L8
.L13:
	.align	2
.L12:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	UNWIND(.fnend)
	.size	avs_ue_v, .-avs_ue_v
	.align	2
	.global	avs_se_v
	.type	avs_se_v, %function
avs_se_v:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	mov	r6, r1
	mov	r1, #32
	mov	r7, r0
	bl	BsShow
	bl	ZerosMS_32
	cmp	r0, #31
	mov	r4, r0
	mvnhi	r5, #-2147483648
	movhi	r4, #32
	bls	.L19
.L15:
	ldr	ip, .L20
	mov	r2, r6
	str	r4, [sp]
	mov	r3, r5
	ldr	r1, .L20+4
	mov	r0, #7
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r5
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L19:
	add	r8, r4, #1
	mov	r1, r4
	mov	r0, r7
	mov	r4, r4, asl #1
	bl	BsSkip
	mov	r1, r8
	mov	r0, r7
	add	r4, r4, #1
	bl	BsShow
	mov	r1, r8
	sub	r0, r0, #1
	tst	r0, #1
	mov	r0, r0, asr #1
	addne	r5, r0, #1
	rsbeq	r5, r0, #0
	mov	r0, r7
	bl	BsSkip
	b	.L15
.L21:
	.align	2
.L20:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	UNWIND(.fnend)
	.size	avs_se_v, .-avs_se_v
	.align	2
	.global	AVS_next_start_code
	.type	AVS_next_start_code, %function
AVS_next_start_code:
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
	bl	BsToNextByte
	mov	r0, r5
	bl	BsResidBits
	add	r6, r0, #7
	cmp	r0, #0
	movlt	r0, r6
	mov	r6, r0, asr #3
	cmp	r6, #3
	subgt	r6, r6, #3
	movgt	r4, #0
	bgt	.L25
	b	.L26
.L31:
	bl	BsSkip
	cmp	r4, r6
	beq	.L26
.L25:
	mov	r1, #24
	mov	r0, r5
	bl	BsShow
	add	r4, r4, #1
	mov	r1, #8
	mov	r3, r0
	cmp	r3, #1
	mov	r0, r5
	bne	.L31
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L26:
	mov	r3, #0
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	AVS_next_start_code, .-AVS_next_start_code
	.align	2
	.global	AVS_sequence_header
	.type	AVS_sequence_header, %function
AVS_sequence_header:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	add	r6, r0, #12288
	mov	r4, r1
	ldr	r2, .L40
	ldr	r5, [r6, #3200]
	mov	r1, #32
	ldr	r3, [r6, #2812]
	mov	r7, r0
	bic	r3, r3, #65536
	mov	r0, r5
	str	r3, [r6, #2812]
	bl	avs_f_v
	ldr	r2, .L40+4
	mov	r1, #8
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+8
	mov	r1, #8
	str	r0, [r4, #12]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+12
	mov	r1, #1
	str	r0, [r4, #16]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+16
	mov	r1, #14
	strb	r0, [r4, #11]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+20
	mov	r1, #14
	str	r0, [r4, #20]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+24
	mov	r1, #2
	str	r0, [r4, #24]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+28
	mov	r1, #3
	strb	r0, [r4, #7]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+32
	mov	r1, #4
	strb	r0, [r4, #8]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+36
	mov	r1, #4
	strb	r0, [r4, #9]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+40
	mov	r1, #18
	strb	r0, [r4, #10]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+44
	mov	r1, #1
	str	r0, [r4, #28]
	mov	r0, r5
	bl	avs_f_v
	ldr	r2, .L40+48
	mov	r1, #12
	strb	r0, [r4, #6]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+52
	mov	r1, #1
	str	r0, [r4, #32]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+44
	mov	r1, #1
	strb	r0, [r4, #5]
	mov	r0, r5
	bl	avs_f_v
	ldr	r2, .L40+56
	mov	r1, #18
	strb	r0, [r4, #6]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+60
	mov	r1, #3
	str	r0, [r4, #36]
	mov	r0, r5
	bl	avs_u_v
	ldr	r3, [r4, #20]
	strb	r0, [r4, #4]
	ldr	ip, [r6, #2848]
	cmp	r3, ip
	beq	.L39
.L33:
	ldr	r3, .L40+64
	mov	r2, #1
	str	r2, [r6, #2824]
	ldr	r5, [r3]
	cmp	r5, #0
	beq	.L34
	ldr	r9, [r6, #2852]
	add	r7, r7, #16384
	ldr	r8, [r4, #20]
	mov	r3, #8
	ldr	lr, [r4, #24]
	sub	r2, fp, #44
	ldr	r0, [r7, #344]
	mov	r1, #2
	strh	ip, [fp, #-44]	@ movhi
	strh	r9, [fp, #-42]	@ movhi
	strh	r8, [fp, #-40]	@ movhi
	strh	lr, [fp, #-38]	@ movhi
	blx	r5
.L34:
	add	r0, r6, #2816
	mov	r1, r4
	mov	r2, #372
	add	r0, r0, #12
	bl	memcpy
	ldr	r2, [r6, #2820]
	ldr	r3, [r6, #2812]
	mov	r0, #1
	add	r2, r2, r0
	str	r0, [r6, #2816]
	orr	r3, r3, #65536
	str	r2, [r6, #2820]
	str	r3, [r6, #2812]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L39:
	ldr	r2, [r4, #24]
	ldr	r3, [r6, #2852]
	cmp	r2, r3
	bne	.L33
	b	.L34
.L41:
	.align	2
.L40:
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	g_event_report
	UNWIND(.fnend)
	.size	AVS_sequence_header, .-AVS_sequence_header
	.align	2
	.global	AVS_i_picture_header
	.type	AVS_i_picture_header, %function
AVS_i_picture_header:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L81
	mov	r6, r0
	ldr	r5, [r0, #492]
	mov	r4, r1
	add	r0, r1, #72
	mov	r2, #272
	ldr	r3, [r3, #48]
	mov	r1, #0
	blx	r3
	ldr	r3, [r6, #104]
	ldr	r2, .L81+4
	mov	r1, #32
	bic	r3, r3, #262144
	mov	r0, r5
	str	r3, [r6, #104]
	bl	avs_f_v
	mov	r0, r5
	ldr	r2, .L81+8
	mov	r1, #16
	bl	avs_u_v
	ldr	r3, [r4, #12]
	cmp	r3, #72
	str	r0, [r4, #88]
	beq	.L73
.L43:
	ldr	r2, .L81+12
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	uxtb	r0, r0
	strb	r0, [r4, #82]
	cmp	r0, #1
	beq	.L74
.L44:
	ldr	r2, .L81+16
	mov	r1, #1
	mov	r0, r5
	bl	avs_f_v
	ldr	r2, .L81+20
	mov	r1, #8
	str	r0, [r4, #100]
	mov	r0, r5
	bl	avs_u_v
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	str	r0, [r4, #104]
	beq	.L75
.L45:
	ldr	r2, .L81+24
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	ldrb	r3, [r4, #11]	@ zero_extendqisi2
	cmp	r3, #1
	uxtb	r0, r0
	strb	r0, [r4, #72]
	streqb	r3, [r4, #72]
	beq	.L47
	cmp	r0, #0
	beq	.L76
.L47:
	mov	r3, #1
	strb	r3, [r4, #73]
.L48:
	ldr	r2, .L81+28
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L81+32
	mov	r1, #1
	strb	r0, [r4, #74]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L81+36
	mov	r1, #1
	strb	r0, [r4, #75]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L81+40
	mov	r1, #6
	strb	r0, [r4, #76]
	mov	r0, r5
	bl	avs_u_v
	ldrh	r3, [r4, #72]
	cmp	r3, #0
	strb	r0, [r4, #77]
	beq	.L77
.L49:
	ldr	r2, .L81+44
	mov	r1, #4
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L81+48
	mov	r1, #1
	strb	r0, [r4, #79]
	mov	r0, r5
	bl	avs_u_v
	mov	r3, #0
	str	r3, [r4, #112]
	str	r3, [r4, #116]
	uxtb	r0, r0
	strb	r0, [r4, #80]
	cmp	r0, r3
	beq	.L78
.L51:
	ldr	r3, [r4, #12]
	cmp	r3, #72
	beq	.L79
.L53:
	mov	r0, #1
	mov	r3, #0
	strb	r0, [r4, #85]
	str	r3, [r4, #340]
	ldr	r3, [r6, #104]
	orr	r3, r3, #262144
	str	r3, [r6, #104]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L76:
	ldr	r2, .L81+52
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	strb	r0, [r4, #73]
	b	.L48
.L78:
	ldr	r2, .L81+56
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	uxtb	r0, r0
	strb	r0, [r4, #81]
	cmp	r0, #0
	beq	.L51
	ldr	r1, .L81+60
	mov	r0, r5
	bl	avs_se_v
	ldr	r1, .L81+64
	str	r0, [r4, #112]
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r4, #116]
	b	.L51
.L77:
	ldr	r2, .L81+68
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	strb	r0, [r4, #78]
	b	.L49
.L79:
	ldr	r2, .L81+72
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	cmp	r0, #0
	str	r0, [r4, #120]
	bne	.L54
.L63:
	mov	r0, r5
	ldr	r2, .L81+76
	mov	r1, #1
	bl	avs_u_v
	str	r0, [r4, #324]
	b	.L53
.L73:
	ldr	r2, .L81+16
	mov	r1, #1
	mov	r0, r5
	bl	avs_f_v
	ldr	r2, .L81+80
	mov	r1, #7
	str	r0, [r4, #100]
	mov	r0, r5
	bl	avs_u_v
	ldr	r3, [r4, #88]
	mov	r2, #1
	strb	r2, [r4, #368]
	str	r0, [r4, #92]
	orr	r0, r0, r3, asl #7
	str	r0, [r4, #88]
	b	.L43
.L74:
	ldr	r2, .L81+84
	mov	r1, #24
	mov	r0, r5
	bl	avs_u_v
	str	r0, [r4, #96]
	b	.L44
.L75:
	ldr	r1, .L81+88
	mov	r0, r5
	bl	avs_ue_v
	str	r0, [r4, #108]
	b	.L45
.L54:
	ldr	r2, .L81+92
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L81+96
	mov	r1, #1
	str	r0, [r4, #124]
	mov	r0, r5
	bl	avs_u_v
	cmp	r0, #0
	str	r0, [r4, #128]
	beq	.L80
.L55:
	ldr	r2, .L81+100
	mov	r1, #2
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L81+104
	mov	r1, #2
	str	r0, [r4, #140]
	mov	r0, r5
	bl	avs_u_v
	ldr	r3, [r4, #140]
	cmp	r3, #1
	str	r0, [r4, #144]
	beq	.L58
	ldr	r2, [r4, #124]
	cmp	r2, #0
	cmpeq	r3, #3
	beq	.L58
	cmp	r3, #2
	addeq	r7, r4, #168
	bne	.L63
.L62:
	add	r8, r4, #192
.L61:
	ldr	r1, .L81+108
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r7, #4]!
	cmp	r7, r8
	bne	.L61
	b	.L63
.L80:
	ldr	r1, .L81+112
	mov	r0, r5
	bl	avs_se_v
	ldr	r1, .L81+116
	str	r0, [r4, #132]
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r4, #136]
	b	.L55
.L58:
	add	r8, r4, #144
	add	r7, r4, #168
.L57:
	ldr	r1, .L81+120
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r8, #4]!
	cmp	r8, r7
	bne	.L57
	ldr	r3, [r4, #140]
	cmp	r3, #2
	beq	.L62
	ldr	r2, [r4, #124]
	cmp	r2, #0
	cmpeq	r3, #3
	bne	.L63
	b	.L62
.L82:
	.align	2
.L81:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC17
	.word	.LC18
	.word	.LC20
	.word	.LC12
	.word	.LC22
	.word	.LC24
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.word	.LC16
	.word	.LC31
	.word	.LC25
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC30
	.word	.LC35
	.word	.LC36
	.word	.LC19
	.word	.LC21
	.word	.LC23
	.word	.LC37
	.word	.LC38
	.word	.LC41
	.word	.LC42
	.word	.LC44
	.word	.LC39
	.word	.LC40
	.word	.LC43
	UNWIND(.fnend)
	.size	AVS_i_picture_header, .-AVS_i_picture_header
	.align	2
	.global	AVS_pb_picture_header
	.type	AVS_pb_picture_header, %function
AVS_pb_picture_header:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L135
	mov	r6, r0
	ldr	r5, [r0, #492]
	mov	r4, r1
	add	r0, r1, #72
	mov	r2, #272
	ldr	r3, [r3, #48]
	mov	r1, #0
	blx	r3
	ldr	r3, [r6, #104]
	ldr	r2, .L135+4
	mov	r1, #32
	bic	r3, r3, #262144
	mov	r0, r5
	str	r3, [r6, #104]
	bl	avs_f_v
	mov	r0, r5
	ldr	r2, .L135+8
	mov	r1, #16
	bl	avs_u_v
	ldr	r3, [r4, #12]
	cmp	r3, #72
	str	r0, [r4, #88]
	beq	.L127
.L84:
	ldr	r2, .L135+12
	mov	r1, #2
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+16
	mov	r1, #8
	strb	r0, [r4, #83]
	mov	r0, r5
	bl	avs_u_v
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	str	r0, [r4, #104]
	beq	.L128
.L85:
	ldr	r2, .L135+20
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	uxtb	r0, r0
	strb	r0, [r4, #72]
	cmp	r0, #0
	movne	r3, #1
	strneb	r3, [r4, #73]
	beq	.L129
.L87:
	ldr	r2, .L135+24
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+28
	mov	r1, #1
	strb	r0, [r4, #74]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+32
	mov	r1, #1
	strb	r0, [r4, #75]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+36
	mov	r1, #6
	strb	r0, [r4, #76]
	mov	r0, r5
	bl	avs_u_v
	ldrb	r3, [r4, #83]	@ zero_extendqisi2
	cmp	r3, #2
	strb	r0, [r4, #77]
	beq	.L130
.L88:
	ldr	r2, .L135+40
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	strb	r0, [r4, #85]
.L90:
	ldr	r2, .L135+44
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	ldrb	r2, [r4, #83]	@ zero_extendqisi2
	mov	r3, #0
	str	r3, [r4, #332]
	cmp	r2, #2
	str	r3, [r4, #336]
	strb	r0, [r4, #86]
	beq	.L131
	ldr	r2, .L135+48
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	str	r0, [r4, #332]
.L92:
	ldrb	r3, [r4, #73]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L93
	ldr	r3, [r4, #12]
	cmp	r3, #72
	beq	.L94
.L93:
	mov	r3, #0
	str	r3, [r4, #332]
	str	r3, [r4, #336]
.L94:
	ldr	r2, .L135+52
	mov	r1, #2
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+56
	mov	r1, #1
	strb	r0, [r4, #79]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+60
	mov	r1, #1
	strb	r0, [r4, #78]
	mov	r0, r5
	bl	avs_u_v
	mov	r3, #0
	str	r3, [r4, #112]
	str	r3, [r4, #116]
	uxtb	r0, r0
	strb	r0, [r4, #80]
	cmp	r0, r3
	beq	.L132
.L96:
	ldr	r3, [r4, #12]
	cmp	r3, #72
	beq	.L133
.L98:
	ldrb	r3, [r4, #83]	@ zero_extendqisi2
	mov	r0, #1
	cmp	r3, #1
	movne	r3, #2
	str	r3, [r4, #340]
	ldr	r3, [r6, #104]
	orr	r3, r3, #262144
	str	r3, [r6, #104]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L130:
	ldrb	r3, [r4, #73]	@ zero_extendqisi2
	cmp	r3, #1
	streqb	r3, [r4, #85]
	bne	.L88
	b	.L90
.L129:
	ldr	r2, .L135+64
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	uxtb	r0, r0
	strb	r0, [r4, #73]
	cmp	r0, #0
	bne	.L87
	ldr	r2, .L135+68
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	strb	r0, [r4, #84]
	b	.L87
.L132:
	ldr	r2, .L135+72
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	uxtb	r0, r0
	strb	r0, [r4, #81]
	cmp	r0, #0
	beq	.L96
	ldr	r1, .L135+76
	mov	r0, r5
	bl	avs_se_v
	ldr	r1, .L135+80
	str	r0, [r4, #112]
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r4, #116]
	b	.L96
.L131:
	ldr	r2, .L135+84
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	str	r0, [r4, #336]
	b	.L92
.L133:
	ldr	r2, .L135+88
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	cmp	r0, #1
	mov	r7, r0
	str	r0, [r4, #120]
	beq	.L99
.L108:
	mov	r0, r5
	ldr	r2, .L135+92
	mov	r1, #1
	bl	avs_u_v
	str	r0, [r4, #324]
	b	.L98
.L127:
	ldr	r2, .L135+96
	mov	r1, #1
	mov	r0, r5
	bl	avs_f_v
	ldr	r2, .L135+100
	mov	r1, #7
	str	r0, [r4, #100]
	mov	r0, r5
	bl	avs_u_v
	ldr	r3, [r4, #88]
	mov	r2, #1
	strb	r2, [r4, #368]
	str	r0, [r4, #92]
	orr	r0, r0, r3, asl #7
	str	r0, [r4, #88]
	b	.L84
.L128:
	ldr	r1, .L135+104
	mov	r0, r5
	bl	avs_ue_v
	str	r0, [r4, #108]
	b	.L85
.L99:
	mov	r1, r0
	ldr	r2, .L135+108
	mov	r0, r5
	bl	avs_u_v
	mov	r1, r7
	ldr	r2, .L135+112
	str	r0, [r4, #124]
	mov	r0, r5
	bl	avs_u_v
	cmp	r0, #0
	str	r0, [r4, #128]
	beq	.L134
.L100:
	ldr	r2, .L135+116
	mov	r1, #2
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+120
	mov	r1, #2
	str	r0, [r4, #140]
	mov	r0, r5
	bl	avs_u_v
	ldr	r3, [r4, #140]
	cmp	r3, #1
	str	r0, [r4, #144]
	beq	.L103
	ldr	r2, [r4, #124]
	cmp	r2, #0
	cmpeq	r3, #3
	beq	.L103
	cmp	r3, #2
	addeq	r7, r4, #168
	bne	.L108
.L107:
	add	r8, r4, #192
.L106:
	ldr	r1, .L135+124
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r7, #4]!
	cmp	r7, r8
	bne	.L106
	b	.L108
.L103:
	add	r8, r4, #144
	add	r7, r4, #168
.L102:
	ldr	r1, .L135+128
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r8, #4]!
	cmp	r8, r7
	bne	.L102
	ldr	r3, [r4, #140]
	cmp	r3, #2
	beq	.L107
	ldr	r2, [r4, #124]
	cmp	r2, #0
	cmpeq	r3, #3
	bne	.L108
	b	.L107
.L134:
	ldr	r1, .L135+132
	mov	r0, r5
	bl	avs_se_v
	ldr	r1, .L135+136
	str	r0, [r4, #132]
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r4, #136]
	b	.L100
.L136:
	.align	2
.L135:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC45
	.word	.LC18
	.word	.LC46
	.word	.LC22
	.word	.LC24
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.word	.LC48
	.word	.LC49
	.word	.LC51
	.word	.LC16
	.word	.LC30
	.word	.LC31
	.word	.LC25
	.word	.LC47
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC50
	.word	.LC35
	.word	.LC36
	.word	.LC12
	.word	.LC19
	.word	.LC23
	.word	.LC37
	.word	.LC38
	.word	.LC41
	.word	.LC42
	.word	.LC44
	.word	.LC43
	.word	.LC39
	.word	.LC40
	UNWIND(.fnend)
	.size	AVS_pb_picture_header, .-AVS_pb_picture_header
	.align	2
	.global	AVS_user_data
	.type	AVS_user_data, %function
AVS_user_data:
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
	add	r4, r0, #12288
	mov	r7, r1
	mov	r6, r0
	ldr	r2, .L170
	ldr	r0, [r4, #3200]
	mov	r1, #32
	bl	avs_f_v
	ldr	r3, [r4, #2812]
	cmp	r7, #0
	orr	r3, r3, #1048576
	str	r3, [r4, #2812]
	bne	.L138
	ldr	r1, [r4, #3492]
	add	r5, r6, #16384
	cmp	r1, #0
	beq	.L139
	ldr	r0, [r5, #344]
	bl	FreeUsdByDec
	str	r7, [r4, #3492]
.L139:
	ldr	r0, [r5, #344]
	bl	GetUsd
	cmp	r0, #0
	str	r0, [r4, #3492]
	beq	.L149
	ldr	r3, [r6]
	add	r0, r0, #1056
	mov	r1, #9
	ldr	r6, .L170+4
	ldrd	r2, [r3, #48]
	strd	r2, [r0]
	ldr	r3, [r4, #3492]
	str	r1, [r3, #1036]
	ldr	r3, [r4, #3492]
	ldr	r2, [r4, #2820]
	str	r2, [r3, #1040]
	ldr	r2, [r4, #3492]
	ldr	r3, [r5, #196]
	sub	r3, r3, #4
	str	r3, [r2, #1048]
	ldr	r0, [r4, #3492]
	ldr	r2, [r0, #1048]
	cmp	r2, #1024
	movgt	r3, #1024
	strgt	r3, [r0, #1048]
	ldrgt	r0, [r4, #3492]
	ldr	r1, [r5, #184]
	ldr	r3, [r6, #52]
	ldrgt	r2, [r0, #1048]
	add	r1, r1, #4
	blx	r3
	ldrb	r3, [r5, #189]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L144
	ldr	r2, [r4, #3492]
	ldr	r1, [r5, #276]
	ldr	r3, [r2, #1048]
	add	r3, r3, r1
	str	r3, [r2, #1048]
	ldr	r1, [r4, #3492]
	ldr	r2, [r1, #1048]
	cmp	r2, #1024
	movgt	r3, #1024
	strgt	r3, [r1, #1048]
	ldrgt	r1, [r4, #3492]
	ldr	r0, [r5, #196]
	ldrgt	r2, [r1, #1048]
	rsb	r3, r0, #4
	add	r2, r3, r2
	cmp	r2, #0
	ble	.L144
	sub	r0, r0, #4
	ldr	r3, [r6, #52]
	add	r0, r1, r0
	ldr	r1, [r5, #264]
	blx	r3
.L144:
	ldr	r3, .L170+8
	ldr	ip, [r3]
	cmp	ip, #0
	ldrne	r2, [r4, #3492]
	bne	.L169
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L138:
	ldr	r1, [r4, #3496]
	add	r5, r6, #16384
	cmp	r1, #0
	beq	.L148
	ldr	r0, [r5, #344]
	bl	FreeUsdByDec
	mov	r3, #0
	str	r3, [r4, #3496]
.L148:
	ldr	r0, [r5, #344]
	bl	GetUsd
	cmp	r0, #0
	str	r0, [r4, #3496]
	beq	.L149
	ldr	r3, [r6]
	add	r0, r0, #1056
	mov	r1, #10
	ldr	r6, .L170+4
	ldrd	r2, [r3, #48]
	strd	r2, [r0]
	ldr	r3, [r4, #3496]
	str	r1, [r3, #1036]
	ldr	r3, [r4, #3496]
	ldr	r2, [r4, #2820]
	str	r2, [r3, #1040]
	ldr	r2, [r4, #3496]
	ldr	r3, [r5, #196]
	sub	r3, r3, #4
	str	r3, [r2, #1048]
	ldr	r0, [r4, #3496]
	ldr	r2, [r0, #1048]
	cmp	r2, #1024
	movgt	r3, #1024
	strgt	r3, [r0, #1048]
	ldrgt	r0, [r4, #3496]
	ldr	r1, [r5, #184]
	ldr	r3, [r6, #52]
	ldrgt	r2, [r0, #1048]
	add	r1, r1, #4
	blx	r3
	ldrb	r3, [r5, #189]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L152
	ldr	r2, [r4, #3496]
	ldr	r1, [r5, #276]
	ldr	r3, [r2, #1048]
	add	r3, r3, r1
	str	r3, [r2, #1048]
	ldr	r1, [r4, #3496]
	ldr	r2, [r1, #1048]
	cmp	r2, #1024
	movgt	r3, #1024
	strgt	r3, [r1, #1048]
	ldrgt	r1, [r4, #3496]
	ldr	r0, [r5, #196]
	ldrgt	r2, [r1, #1048]
	rsb	r3, r0, #4
	add	r2, r3, r2
	cmp	r2, #0
	ble	.L152
	sub	r0, r0, #4
	ldr	r3, [r6, #52]
	add	r0, r1, r0
	ldr	r1, [r5, #264]
	blx	r3
.L152:
	ldr	r3, .L170+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L168
	ldr	r2, [r4, #3496]
.L169:
	mov	r3, #1072
	ldr	r0, [r5, #344]
	mov	r1, #6
	blx	ip
.L168:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L149:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L171:
	.align	2
.L170:
	.word	.LC52
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_event_report
	UNWIND(.fnend)
	.size	AVS_user_data, .-AVS_user_data
	.align	2
	.global	AVS_sequence_display_extension
	.type	AVS_sequence_display_extension, %function
AVS_sequence_display_extension:
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
	ldr	r4, [r0, #492]
	mov	r5, r1
	ldr	r2, .L175
	mov	r1, #4
	mov	r6, r0
	mov	r0, r4
	bl	avs_u_v
	ldr	r2, .L175+4
	mov	r1, #3
	mov	r0, r4
	bl	avs_u_v
	ldr	r2, .L175+8
	mov	r1, #1
	strb	r0, [r5, #42]
	mov	r0, r4
	bl	avs_u_v
	ldr	r2, .L175+12
	mov	r1, #1
	strb	r0, [r5, #43]
	mov	r0, r4
	bl	avs_u_v
	uxtb	r0, r0
	strb	r0, [r5, #44]
	cmp	r0, #1
	beq	.L174
.L173:
	ldr	r2, .L175+16
	mov	r1, #14
	mov	r0, r4
	bl	avs_u_v
	ldr	r2, .L175+20
	mov	r1, #1
	str	r0, [r5, #60]
	mov	r0, r4
	bl	avs_f_v
	ldr	r2, .L175+24
	mov	r1, #14
	strb	r0, [r5, #41]
	mov	r0, r4
	bl	avs_u_v
	ldr	r2, .L175+28
	mov	r1, #2
	str	r0, [r5, #64]
	mov	r0, r4
	bl	avs_u_v
	strb	r0, [r5, #40]
	mov	r0, r4
	bl	AVS_next_start_code
	ldr	r3, [r6, #104]
	mov	r0, #1
	orr	r3, r3, #131072
	str	r3, [r6, #104]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L174:
	ldr	r2, .L175+32
	mov	r1, #8
	mov	r0, r4
	bl	avs_u_v
	ldr	r2, .L175+36
	mov	r1, #8
	str	r0, [r5, #48]
	mov	r0, r4
	bl	avs_u_v
	ldr	r2, .L175+40
	mov	r1, #8
	str	r0, [r5, #52]
	mov	r0, r4
	bl	avs_u_v
	str	r0, [r5, #56]
	b	.L173
.L176:
	.align	2
.L175:
	.word	.LC53
	.word	.LC54
	.word	.LC55
	.word	.LC56
	.word	.LC60
	.word	.LC12
	.word	.LC61
	.word	.LC16
	.word	.LC57
	.word	.LC58
	.word	.LC59
	UNWIND(.fnend)
	.size	AVS_sequence_display_extension, .-AVS_sequence_display_extension
	.align	2
	.global	AVS_copyright_extension
	.type	AVS_copyright_extension, %function
AVS_copyright_extension:
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
	ldr	r4, [r0, #492]
	mov	r5, r0
	ldr	r2, .L178
	mov	r1, #4
	ldr	r6, .L178+4
	mov	r0, r4
	bl	avs_u_v
	mov	r0, r4
	ldr	r2, .L178+8
	mov	r1, #1
	bl	avs_u_v
	mov	r0, r4
	ldr	r2, .L178+12
	mov	r1, #8
	bl	avs_u_v
	mov	r0, r4
	ldr	r2, .L178+16
	mov	r1, #1
	bl	avs_u_v
	mov	r0, r4
	ldr	r2, .L178+20
	mov	r1, #7
	bl	avs_u_v
	mov	r2, r6
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L178+24
	mov	r1, #20
	bl	avs_u_v
	mov	r2, r6
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L178+28
	mov	r1, #22
	bl	avs_u_v
	mov	r2, r6
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	ldr	r2, .L178+32
	mov	r1, #22
	mov	r0, r4
	bl	avs_u_v
	mov	r0, r4
	bl	AVS_next_start_code
	ldr	r3, [r5, #104]
	mov	r0, #1
	orr	r3, r3, #2097152
	str	r3, [r5, #104]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L179:
	.align	2
.L178:
	.word	.LC53
	.word	.LC12
	.word	.LC62
	.word	.LC63
	.word	.LC64
	.word	.LC16
	.word	.LC65
	.word	.LC66
	.word	.LC67
	UNWIND(.fnend)
	.size	AVS_copyright_extension, .-AVS_copyright_extension
	.align	2
	.global	AVS_camera_parameters_extension
	.type	AVS_camera_parameters_extension, %function
AVS_camera_parameters_extension:
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
	ldr	r4, [r0, #492]
	mov	r6, r0
	ldr	r7, .L181
	mov	r1, #4
	ldr	r5, .L181+4
	ldr	r2, .L181+8
	mov	r0, r4
	bl	avs_u_v
	mov	r2, r7
	mov	r0, r4
	mov	r1, #1
	bl	avs_u_v
	mov	r0, r4
	ldr	r2, .L181+12
	mov	r1, #7
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+16
	mov	r1, #22
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+20
	mov	r1, #22
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+24
	mov	r1, #22
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+28
	mov	r1, #22
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+32
	mov	r1, #16
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+36
	mov	r1, #16
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+40
	mov	r1, #16
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+44
	mov	r1, #16
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+48
	mov	r1, #16
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+52
	mov	r1, #16
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+56
	mov	r1, #22
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+60
	mov	r1, #22
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+64
	mov	r1, #22
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+68
	mov	r1, #22
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+72
	mov	r1, #22
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r0, r4
	ldr	r2, .L181+76
	mov	r1, #22
	bl	avs_u_v
	mov	r2, r5
	mov	r0, r4
	mov	r1, #1
	bl	avs_f_v
	mov	r2, r7
	mov	r1, #32
	mov	r0, r4
	bl	avs_u_v
	mov	r0, r4
	bl	AVS_next_start_code
	ldr	r3, [r6, #104]
	mov	r0, #1
	orr	r3, r3, #4194304
	str	r3, [r6, #104]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L182:
	.align	2
.L181:
	.word	.LC16
	.word	.LC12
	.word	.LC53
	.word	.LC68
	.word	.LC69
	.word	.LC70
	.word	.LC71
	.word	.LC72
	.word	.LC73
	.word	.LC74
	.word	.LC75
	.word	.LC76
	.word	.LC77
	.word	.LC78
	.word	.LC79
	.word	.LC80
	.word	.LC81
	.word	.LC82
	.word	.LC83
	.word	.LC84
	UNWIND(.fnend)
	.size	AVS_camera_parameters_extension, .-AVS_camera_parameters_extension
	.align	2
	.global	AVS_picture_display_extension
	.type	AVS_picture_display_extension, %function
AVS_picture_display_extension:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, [r0, #492]
	mov	r7, r1
	mov	r8, r0
	mov	r1, #4
	ldr	r2, .L202
	mov	r0, r4
	bl	avs_u_v
	ldrb	r3, [r7, #11]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L200
	ldrb	r3, [r7, #73]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L201
.L185:
	mov	r3, #1
	str	r3, [r7]
.L187:
	add	r6, r7, #340
	mov	r5, #0
.L189:
	ldr	r2, .L202+4
	mov	r1, #16
	mov	r0, r4
	add	r5, r5, #1
	bl	avs_u_v
	ldr	r2, .L202+8
	mov	r1, #1
	str	r0, [r6, #4]!
	mov	r0, r4
	bl	avs_f_v
	ldr	r2, .L202+12
	mov	r1, #16
	mov	r0, r4
	bl	avs_u_v
	ldr	r2, .L202+8
	mov	r1, #1
	str	r0, [r6, #12]
	mov	r0, r4
	bl	avs_f_v
	ldr	r3, [r7]
	cmp	r3, r5
	bgt	.L189
	mov	r0, r4
	bl	AVS_next_start_code
	ldr	r3, [r8, #104]
	mov	r0, #1
	orr	r3, r3, #524288
	str	r3, [r8, #104]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L201:
	ldrb	r3, [r7, #75]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L188
.L186:
	mov	r3, #2
	str	r3, [r7]
	b	.L187
.L200:
	ldrb	r3, [r7, #75]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L185
	ldrb	r3, [r7, #74]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L186
.L188:
	mov	r3, #3
	str	r3, [r7]
	b	.L187
.L203:
	.align	2
.L202:
	.word	.LC53
	.word	.LC85
	.word	.LC12
	.word	.LC86
	UNWIND(.fnend)
	.size	AVS_picture_display_extension, .-AVS_picture_display_extension
	.align	2
	.global	AVS_extension
	.type	AVS_extension, %function
AVS_extension:
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
	ldr	r4, [r0, #492]
	mov	r5, r0
	ldr	r2, .L211
	mov	r6, r1
	mov	r1, #32
	mov	r0, r4
	bl	avs_f_v
	mov	r0, r4
	mov	r1, #4
	bl	BsShow
	sub	r0, r0, #2
	cmp	r0, #9
	ldrls	pc, [pc, r0, asl #2]
	b	.L205
.L207:
	.word	.L206
	.word	.L205
	.word	.L208
	.word	.L205
	.word	.L205
	.word	.L209
	.word	.L205
	.word	.L205
	.word	.L205
	.word	.L210
.L210:
	mov	r1, r6
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	AVS_camera_parameters_extension
.L209:
	mov	r1, r6
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	AVS_picture_display_extension
.L208:
	mov	r1, r6
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	AVS_copyright_extension
.L206:
	mov	r1, r6
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	AVS_sequence_display_extension
.L205:
	mov	r0, r4
	mov	r1, #4
	bl	BsSkip
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L212:
	.align	2
.L211:
	.word	.LC87
	UNWIND(.fnend)
	.size	AVS_extension, .-AVS_extension
	.align	2
	.global	AVS_extension_and_userdata
	.type	AVS_extension_and_userdata, %function
AVS_extension_and_userdata:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #12288
	add	r6, r0, #14976
	add	r5, r0, #15808
	mov	r7, r0
	mov	r8, r1
	ldr	r4, [r3, #3200]
	add	r6, r6, #20
	add	r5, r5, #28
	b	.L217
.L216:
	mov	r1, #32
	mov	r0, r4
	bl	BsShow
	uxtb	r0, r0
	cmp	r0, #178
	beq	.L221
.L217:
	mov	r0, r4
	bl	BsResidBits
	cmp	r0, #40
	mov	r0, r4
	ble	.L222
	bl	AVS_next_start_code
	mov	r1, #32
	mov	r0, r4
	bl	BsShow
	uxtb	r0, r0
	cmp	r0, #178
	cmpne	r0, #181
	bne	.L219
	cmp	r0, #181
	bne	.L216
	mov	r1, r5
	mov	r0, r6
	bl	AVS_extension
	b	.L216
.L222:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L219:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L221:
	mov	r1, r8
	mov	r0, r7
	bl	AVS_user_data
	b	.L217
	UNWIND(.fnend)
	.size	AVS_extension_and_userdata, .-AVS_extension_and_userdata
	.align	2
	.global	AVS_AllocateVHB
	.type	AVS_AllocateVHB, %function
AVS_AllocateVHB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #60)
	sub	sp, sp, #60
	add	r3, r0, #12288
	mov	r6, r0
	ldr	r5, [r3, #2848]
	ldr	r7, [r3, #2852]
	cmp	r5, #31
	bls	.L224
	cmp	r7, #31
	bls	.L224
	ldr	r3, [r0]
	ldr	r4, [r3, #776]
	ldr	r2, [r3, #28]
	cmp	r4, #0
	bne	.L227
	cmp	r2, #25
	beq	.L251
	add	r8, r0, #16384
	mov	r10, #5
	mov	r4, #4
	str	r4, [fp, #-88]
.L228:
	ldr	r9, .L254
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r9, #48]
	blx	r3
	ldr	r2, [r6]
	str	r7, [fp, #-48]
	mov	r3, #0
	str	r5, [fp, #-68]
	ldr	r2, [r2, #28]
	str	r7, [fp, #-64]
	cmp	r2, #24
	str	r5, [fp, #-52]
	strb	r3, [fp, #-75]
	beq	.L237
	ldrb	r3, [fp, #-88]	@ zero_extendqisi2
	uxtb	ip, r4
	strb	r10, [fp, #-70]
	strb	r3, [fp, #-72]
.L241:
	sub	r1, fp, #76
	ldr	r0, [r8, #344]
	strb	ip, [fp, #-69]
	mov	r5, #0
	strb	r5, [fp, #-73]
	bl	FSP_ConfigInstance
	subs	r4, r0, #0
	bne	.L238
	ldr	r1, [r6]
	sub	r3, fp, #80
	ldr	r0, [r8, #344]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	moveq	r4, #3
	beq	.L248
	cmp	r0, r5
	moveq	r4, #1
	streq	r4, [r8, #856]
	beq	.L248
	ldr	r3, [r9, #68]
	mov	r0, r4
	ldr	r1, .L254+4
	blx	r3
	b	.L248
.L224:
	ldr	ip, .L254
	mov	r0, #0
	mov	r3, r7
	mov	r2, r5
	ldr	r1, .L254+8
	mov	r4, r0
	ldr	ip, [ip, #68]
	blx	ip
.L248:
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L227:
	cmp	r2, #25
	beq	.L252
	mov	r4, #4
	add	r8, r0, #16384
	str	r4, [fp, #-88]
	mov	r10, #5
	b	.L228
.L252:
	ldr	r1, [r3, #736]
	ldr	r2, [r3, #740]
	cmp	r1, r5
	bcc	.L234
	cmp	r2, r7
	bcc	.L234
	ldr	r2, [r3, #764]
	ldr	ip, [r3, #760]
	cmp	r2, #0
	add	r2, ip, #2
	str	r2, [fp, #-88]
	moveq	r4, #1
	bne	.L253
.L236:
	ldrb	r10, [r3, #784]	@ zero_extendqisi2
	add	r8, r6, #16384
	b	.L228
.L251:
	ldr	r1, [r3, #736]
	ldr	r2, [r3, #740]
	cmp	r1, r5
	bcc	.L230
	cmp	r2, r7
	bcc	.L230
	add	r8, r0, #16384
	ldr	r4, [r8, #856]
	cmp	r4, #1
	beq	.L248
	ldr	r1, [r3, #764]
	ldr	r2, [r3, #760]
	cmp	r1, #0
	add	r1, r2, #2
	str	r1, [fp, #-88]
	moveq	r4, #1
	beq	.L232
	add	ip, r2, #1
	cmp	ip, #17
	movlt	r4, ip
	movge	r4, #17
.L232:
	ldrb	r10, [r3, #784]	@ zero_extendqisi2
	b	.L228
.L238:
	ldr	r3, [r9, #68]
	mov	r0, r5
	ldr	r1, .L254+12
	mov	r4, r5
	blx	r3
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L237:
	strb	r3, [fp, #-72]
	mov	ip, #1
	strb	r10, [fp, #-70]
	b	.L241
.L234:
	ldr	ip, .L254
	mov	r3, r7
	str	r2, [sp, #4]
	mov	r0, #0
	str	r1, [sp]
	mov	r2, r5
	ldr	ip, [ip, #68]
	mov	r4, r0
	ldr	r1, .L254+16
	blx	ip
	b	.L248
.L253:
	add	ip, ip, #1
	cmp	ip, #17
	movlt	r4, ip
	movge	r4, #17
	b	.L236
.L230:
	ldr	ip, .L254
	mov	r3, r7
	str	r2, [sp, #4]
	mov	r0, #0
	str	r1, [sp]
	mov	r2, r5
	ldr	ip, [ip, #68]
	ldr	r1, .L254+16
	blx	ip
	b	.L248
.L255:
	.align	2
.L254:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC90
	.word	.LC88
	.word	.LC91
	.word	.LC89
	UNWIND(.fnend)
	.size	AVS_AllocateVHB, .-AVS_AllocateVHB
	.align	2
	.global	AVS_StartDecode
	.type	AVS_StartDecode, %function
AVS_StartDecode:
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
	add	r4, r0, #16384
	add	r6, r0, #12288
	mov	r5, r0
	ldr	r3, [r4, #348]
	str	r3, [r0, #8]
	ldr	r3, [r4, #352]
	str	r3, [r0, #12]
	ldr	r3, [r4, #356]
	str	r3, [r0, #16]
	ldr	r3, [r4, #360]
	str	r3, [r0, #20]
	ldr	r3, [r4, #364]
	str	r3, [r0, #24]
	ldr	r3, [r4, #376]
	str	r3, [r0, #28]
	ldr	r3, [r4, #380]
	str	r3, [r0, #180]
	ldr	r3, [r4, #400]
	str	r3, [r0, #32]
	ldr	r3, [r4, #528]
	str	r3, [r0, #192]
	ldr	r3, [r4, #540]
	str	r3, [r0, #196]
	ldr	r3, [r4, #532]
	str	r3, [r0, #200]
	ldr	r3, [r4, #544]
	str	r3, [r0, #204]
	ldr	r3, [r4, #536]
	str	r3, [r0, #208]
	ldr	r3, [r4, #548]
	str	r3, [r0, #212]
	ldr	r3, [r4, #408]
	str	r3, [r0, #172]
	ldr	r3, [r4, #404]
	str	r3, [r0, #176]
	ldr	r3, [r4, #412]
	str	r3, [r0, #128]
	ldr	r3, [r4, #416]
	str	r3, [r0, #132]
	ldr	r3, [r4, #452]
	str	r3, [r0, #36]
	ldr	r3, [r4, #456]
	str	r3, [r0, #40]
	ldr	r3, [r4, #420]
	str	r3, [r0, #44]
	ldr	r3, [r4, #424]
	str	r3, [r0, #48]
	ldr	r3, [r4, #432]
	str	r3, [r0, #52]
	ldr	r3, [r4, #436]
	str	r3, [r0, #56]
	ldr	r3, [r4, #440]
	str	r3, [r0, #60]
	ldr	r3, [r4, #444]
	str	r3, [r0, #64]
	ldr	r3, [r4, #448]
	str	r3, [r0, #68]
	ldr	r3, [r4, #460]
	str	r3, [r0, #72]
	ldr	r3, [r4, #464]
	str	r3, [r0, #76]
	ldr	r3, [r4, #468]
	str	r3, [r0, #80]
	ldr	r3, [r4, #472]
	str	r3, [r0, #84]
	ldr	r3, [r4, #476]
	str	r3, [r0, #88]
	ldr	r3, [r4, #520]
	str	r3, [r0, #92]
	ldr	r3, [r4, #428]
	str	r3, [r0, #96]
	ldr	r3, [r4, #484]
	str	r3, [r0, #100]
	ldr	r3, [r4, #480]
	str	r3, [r0, #104]
	ldr	r3, [r4, #500]
	str	r3, [r0, #108]
	ldr	r3, [r4, #496]
	str	r3, [r0, #112]
	ldr	r3, [r4, #492]
	str	r3, [r0, #116]
	ldr	r3, [r4, #488]
	str	r3, [r0, #120]
	ldr	r3, [r4, #524]
	str	r3, [r0, #124]
	ldr	r2, [r4, #504]
	ldr	r3, [r6, #2784]
	str	r2, [r6, #2268]
	ldr	r2, [r4, #508]
	str	r2, [r6, #2272]
	ldr	r2, [r4, #512]
	str	r2, [r6, #2276]
	ldr	r2, [r4, #580]
	str	r2, [r6, #2284]
	ldr	r2, [r4, #592]
	str	r2, [r6, #2288]
	ldr	r1, [r3, #4]
	ldr	r0, [r4, #344]
	bl	FSP_GetLogicFs
	subs	r7, r0, #0
	beq	.L271
	ldr	r2, [r7, #524]
	add	r0, r5, #14656
	add	r0, r0, #56
	add	r3, r5, #16896
	add	ip, r5, #17152
	add	r3, r3, #214
	ldr	r1, [r2, #20]
	add	ip, ip, #86
	mov	r2, r0
	str	r1, [r6, #2684]
	ldr	r1, [r4, #400]
	str	r1, [r6, #2280]
	ldr	r1, [r4, #516]
	str	r1, [r6, #2292]
	ldr	r1, [r4, #724]
	str	r1, [r6, #2296]
	ldr	r1, [r4, #552]
	str	r1, [r5, #136]
	ldr	r1, [r4, #556]
	str	r1, [r5, #140]
	ldr	r1, [r4, #564]
	str	r1, [r5, #144]
	ldr	r1, [r4, #560]
	str	r1, [r5, #148]
	ldr	r1, [r4, #568]
	str	r1, [r5, #152]
	ldr	r1, [r4, #572]
	str	r1, [r5, #156]
	ldr	r1, [r4, #576]
	str	r1, [r5, #160]
.L259:
	ldrsh	r1, [r3, #2]!
	cmp	r3, ip
	str	r1, [r2, #4]!
	bne	.L259
	ldr	r1, [r4, #384]
	add	r2, r5, #16896
	add	r3, r5, #14528
	add	r2, r2, #80
	add	r3, r3, #56
	str	r1, [r6, #2688]
	ldr	r1, [r4, #388]
	str	r1, [r6, #2692]
	ldr	r1, [r4, #392]
	str	r1, [r6, #2696]
	ldr	r1, [r4, #396]
	str	r1, [r6, #2700]
.L260:
	ldr	r1, [r2, #4]!
	str	r1, [r3, #4]!
	cmp	r3, r0
	bne	.L260
	ldr	r3, [r6, #3488]
	add	r1, r5, #8
	add	r3, r3, #1
	str	r3, [r6, #3488]
	ldr	r3, [r5]
	str	r1, [r5, #4]
	ldr	r2, [r3, #884]
	cmp	r2, #1
	beq	.L272
	cmp	r2, #2
	beq	.L273
.L262:
	ldr	r3, [r3, #676]
	mov	r0, #1
	str	r3, [r6, #2704]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L272:
	cmp	r1, #0
	beq	.L262
	ldr	r1, [r6, #2784]
	ldrb	r2, [r1, #1]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L262
	mov	r6, #0
	str	r6, [r5, #4]
	mov	r2, r6
	ldr	r0, [r4, #344]
	ldr	r1, [r1, #4]
	bl	FSP_SetDisplay
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L273:
	ldr	r2, [r3, #888]
	cmp	r1, #0
	cmpne	r2, #0
	ble	.L262
	ldr	r1, [r6, #2784]
	ldrb	r2, [r1, #1]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L262
	mov	r6, #0
	str	r6, [r5, #4]
	mov	r2, r6
	ldr	r0, [r4, #344]
	ldr	r1, [r1, #4]
	bl	FSP_SetDisplay
	ldr	r2, [r5]
	mov	r0, r6
	ldr	r3, [r2, #888]
	sub	r3, r3, #1
	str	r3, [r2, #888]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L271:
	ldr	r3, .L274
	movw	r2, #2188
	ldr	r1, .L274+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L275:
	.align	2
.L274:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC92
	UNWIND(.fnend)
	.size	AVS_StartDecode, .-AVS_StartDecode
	.align	2
	.global	AVS_SetImgFormat
	.type	AVS_SetImgFormat, %function
AVS_SetImgFormat:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r5, r0, #12288
	mov	r6, r0
	ldrb	r3, [r5, #2839]	@ zero_extendqisi2
	ldr	r10, [r5, #3168]
	cmp	r3, #1
	ldrb	r7, [r5, #2870]	@ zero_extendqisi2
	beq	.L282
	ldrb	r3, [r5, #2900]	@ zero_extendqisi2
	subs	r3, r3, #1
	movne	r3, #1
	str	r3, [fp, #-48]
.L277:
	ldrb	r3, [r5, #2837]	@ zero_extendqisi2
	add	r1, r6, #16384
	ldrb	r9, [r5, #2902]	@ zero_extendqisi2
	sub	r3, r3, #1
	ldr	r0, [r1, #344]
	uxtb	r3, r3
	cmp	r3, #3
	ldrls	r2, .L287
	movhi	r8, #0
	ldrls	r8, [r2, r3, asl #2]
	ldr	r2, [r5, #2784]
	ldr	r1, [r2, #4]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L285
	and	ip, r10, #3
	and	r9, r9, #3
	orr	r1, ip, #3072
	ldr	r3, [fp, #-48]
	and	r7, r7, #3
	orr	r9, r1, r9, asl #12
	and	lr, r8, #7
	mov	r1, r8
	orr	r2, r9, r7, asl #5
	orr	r3, r2, r3, asl #8
	orr	r3, r3, lr, asl #14
	str	r3, [r4, #160]
	ldr	r3, [r5, #3480]
	str	ip, [r4, #152]
	str	ip, [r4, #156]
	str	r3, [r4, #200]
	bl	SetAspectRatio
	ldr	r3, [r5, #2848]
	str	r3, [r4, #164]
	ldr	r3, [r5, #2852]
	str	r3, [r4, #168]
	ldr	r2, [r6]
	ldr	r2, [r2, #28]
	cmp	r2, #25
	beq	.L286
.L280:
	mov	r3, #6
	str	r3, [r4, #384]
	ldrb	r1, [r5, #2901]	@ zero_extendqisi2
	mov	r2, #1
	mov	r3, #8
	strb	r2, [r4, #379]
	cmp	r1, r2
	strb	r2, [r4, #377]
	str	r3, [r4, #388]
	moveq	r2, #0
	movne	r2, #4
	str	r3, [r4, #396]
	strb	r2, [r4, #372]
	str	r3, [r4, #392]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L282:
	mov	r3, #0
	str	r3, [fp, #-48]
	b	.L277
.L286:
	ldr	r2, [r4, #192]
	add	r3, r3, #15
	ldr	r0, [r4, #104]
	mov	r3, r3, lsr #4
	ldr	r1, [r4, #32]
	mov	lr, r2, asl #4
	add	ip, r0, r2
	str	r0, [r4, #88]
	mul	r3, r3, lr
	str	r0, [r4, #120]
	str	ip, [r4, #96]
	str	r1, [r4, #48]
	add	r0, r3, r0
	add	r3, r1, r3
	add	r2, r0, r2
	str	r3, [r4, #36]
	str	r0, [r4, #108]
	str	r0, [r4, #92]
	str	r2, [r4, #100]
	bl	MEM_Phy2Vir
	ldr	r3, [r4, #92]
	str	r3, [r4, #124]
	str	r0, [r4, #52]
	b	.L280
.L285:
	ldr	r3, .L287+4
	movw	r2, #2312
	ldr	r1, .L287+8
	ldr	r3, [r3, #68]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	r3
.L288:
	.align	2
.L287:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC93
	UNWIND(.fnend)
	.size	AVS_SetImgFormat, .-AVS_SetImgFormat
	.align	2
	.global	AVS_GetImageBuffer
	.type	AVS_GetImageBuffer, %function
AVS_GetImageBuffer:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r4, r0, #12288
	mov	r7, r0
	ldr	r3, [r4, #3888]
	cmp	r3, #2
	beq	.L290
	ldr	r5, [r4, #3212]
	add	r6, r0, #14976
	add	r6, r6, #20
	add	r2, r5, #1
	mov	r3, r5, asl #4
	mov	r1, r2, asr #31
	sub	r5, r3, r5, asl #2
	add	r5, r5, #16
	mov	r3, r1, lsr #30
	add	r5, r6, r5
	add	r2, r2, r3
	str	r5, [r4, #2784]
	and	r2, r2, #3
	rsb	r2, r3, r2
	str	r2, [r4, #3212]
.L291:
	add	r8, r7, #16384
	mov	r1, #1
	ldr	r0, [r8, #344]
	bl	FSP_NewLogicFs
	str	r0, [r5, #4]
	ldr	r3, [r4, #2784]
	ldr	r1, [r3, #4]
	cmp	r1, #0
	blt	.L309
	ldr	r0, [r8, #344]
	bl	FSP_GetLogicFs
	subs	r5, r0, #0
	beq	.L310
	ldr	r3, [r4, #2784]
	mov	r0, #18
	ldr	r4, .L311
	ldr	r1, .L311+4
	ldr	r2, [r3, #4]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r5, #520]
	cmp	r2, #0
	beq	.L296
	ldr	r3, [r5, #524]
	cmp	r3, #0
	beq	.L296
	ldr	r1, [r5, #528]
	cmp	r1, #0
	beq	.L296
	ldr	lr, [r1, #12]
	mov	r0, #18
	ldr	r3, [r3, #12]
	ldr	r2, [r2, #12]
	ldr	ip, [r4, #68]
	ldr	r1, .L311+8
	str	lr, [sp]
	blx	ip
.L296:
	mov	r0, #1
	mov	r3, #0
	str	r3, [r5, #208]
.L294:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L290:
	add	r6, r0, #14976
	add	r5, r6, #84
	add	r6, r6, #20
	str	r5, [r4, #2784]
	b	.L291
.L309:
	ldr	r3, .L311
	mov	r0, #0
	ldr	r1, .L311+12
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [r4, #2788]
	ldr	r0, [r8, #344]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L293
	ldr	r3, [r4, #2784]
	mov	r2, #1
	ldr	r0, [r8, #344]
	add	r6, r6, #4
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2788]
	mov	r2, #1
	ldr	r0, [r8, #344]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2792]
	mov	r2, #1
	ldr	r0, [r8, #344]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2796]
	mov	r2, #1
	ldr	r0, [r8, #344]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r1, [r5, #212]
	ldr	r0, [r8, #344]
	bl	FreeUsdByDec
	ldr	r1, [r5, #216]
	ldr	r0, [r8, #344]
	bl	FreeUsdByDec
	ldr	r1, [r5, #220]
	ldr	r0, [r8, #344]
	bl	FreeUsdByDec
	ldr	r1, [r5, #224]
	ldr	r0, [r8, #344]
	bl	FreeUsdByDec
	mov	r3, #0
	str	r3, [r5, #212]
	str	r3, [r5, #216]
	str	r3, [r5, #220]
	str	r3, [r5, #224]
	str	r6, [r4, #2784]
	str	r6, [r4, #2796]
	str	r6, [r4, #2792]
	str	r6, [r4, #2788]
	ldr	r0, [r8, #344]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #2716]
.L293:
	add	r1, r7, #16192
	mov	r5, #0
	add	r1, r1, #16
	str	r5, [r4, #3476]
	ldr	r0, [r8, #344]
	bl	FSP_ClearNotInVoQueue
	mov	r0, r5
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L310:
	ldr	r3, .L311
	movw	r2, #2415
	ldr	r1, .L311+16
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	b	.L294
.L312:
	.align	2
.L311:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC95
	.word	.LC96
	.word	.LC94
	.word	.LC92
	UNWIND(.fnend)
	.size	AVS_GetImageBuffer, .-AVS_GetImageBuffer
	.align	2
	.global	AVS_OutputOneFrame
	.type	AVS_OutputOneFrame, %function
AVS_OutputOneFrame:
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
	add	r5, r1, #16384
	mov	r7, r1
	mov	r6, r0
	ldr	r1, [r0, #4]
	ldr	r0, [r5, #344]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L319
	mov	r2, #1
	ldr	r1, [r6, #4]
	ldr	r0, [r5, #344]
	bl	FSP_SetDisplay
	add	r3, r7, #16192
	ldr	r0, [r5, #344]
	add	r3, r3, #16
	str	r4, [sp]
	mov	r2, r7
	mov	r1, #6
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L315
	ldr	r0, [r5, #344]
	mov	r2, #0
	ldr	r1, [r6, #4]
	bl	FSP_SetDisplay
	ldr	r3, .L320
	ldr	r2, [r6, #4]
	mov	r0, #1
	ldr	r1, .L320+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
.L315:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L319:
	ldr	r3, .L320
	movw	r2, #2456
	ldr	r1, .L320+8
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L321:
	.align	2
.L320:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC98
	.word	.LC97
	UNWIND(.fnend)
	.size	AVS_OutputOneFrame, .-AVS_OutputOneFrame
	.align	2
	.global	AVS_UpdateTableIdx2Dist
	.type	AVS_UpdateTableIdx2Dist, %function
AVS_UpdateTableIdx2Dist:
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
	ldrb	r3, [r0, #193]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r0, #460]
	beq	.L336
	cmp	r3, #0
	beq	.L337
	cmp	r3, #1
	bne	.L331
	ldr	r3, [r0, #84]
	add	r2, r0, #4
	ldr	r1, [r0, #76]
	cmp	r3, r2
	ldrne	r2, [r3, #8]
	ldr	r3, [r0, #80]
	ldreq	r2, [r0, #536]
	movne	r2, r2, asl #1
	strne	r2, [r0, #536]
	add	r2, r2, #1
	str	r2, [r0, #532]
	ldr	r3, [r3, #8]
	str	r2, [r0, #552]
	mov	r3, r3, asl #1
	str	r3, [r0, #528]
	str	r3, [r0, #548]
	add	r3, r3, #1
	str	r3, [r0, #524]
	str	r3, [r0, #544]
	ldr	r3, [r1, #8]
	mov	r3, r3, asl #1
	str	r3, [r0, #540]
.L327:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L337:
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #8]
	mov	r3, r3, asl #1
	str	r3, [r0, #540]
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L336:
	cmp	r3, #1
	beq	.L338
	cmp	r3, #2
	bne	.L327
	ldr	r3, [r0, #84]
	add	r2, r0, #4
	cmp	r3, r2
	ldrne	r3, [r3, #8]
	movne	r3, r3, asl #1
	strne	r3, [r0, #524]
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #8]
	mov	r3, r3, asl #1
	str	r3, [r0, #532]
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L331:
	cmp	r3, #2
	bne	.L327
	ldr	r3, [r0, #84]
	add	r2, r0, #4
	cmp	r3, r2
	ldrne	r2, [r3, #8]
	ldr	r3, [r0, #80]
	ldreq	r2, [r0, #528]
	movne	r2, r2, asl #1
	strne	r2, [r0, #528]
	add	r1, r2, #1
	str	r1, [r0, #524]
	ldr	r3, [r3, #8]
	str	r1, [r0, #540]
	str	r2, [r0, #544]
	mov	r3, r3, asl #1
	str	r3, [r0, #532]
	str	r3, [r0, #548]
	add	r3, r3, #1
	str	r3, [r0, #536]
	str	r3, [r0, #552]
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L338:
	ldr	r3, [r0, #80]
	add	r2, r0, #4
	ldr	r1, [r0, #84]
	ldr	r3, [r3, #8]
	cmp	r1, r2
	mov	r3, r3, asl #1
	str	r3, [r0, #524]
	ldrne	r3, [r1, #8]
	movne	r3, r3, asl #1
	strne	r3, [r0, #528]
	b	.L327
	UNWIND(.fnend)
	.size	AVS_UpdateTableIdx2Dist, .-AVS_UpdateTableIdx2Dist
	.global	__aeabi_idiv
	.align	2
	.global	AVS_UpdateTableMvEScale
	.type	AVS_UpdateTableMvEScale, %function
AVS_UpdateTableMvEScale:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	mov	r6, r0
	ldrb	r0, [r0, #193]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L390
	ldr	r3, [r6, #460]
	cmp	r3, #0
	moveq	r3, #512
	streq	r3, [r6, #620]
	beq	.L376
	cmp	r3, #1
	bne	.L366
	ldr	r3, [r6, #76]
	add	r10, r6, #536
	str	r3, [fp, #-60]
	add	r3, r6, #520
	str	r3, [fp, #-48]
	add	r3, r6, #552
	str	r3, [fp, #-52]
	mov	r3, #0
	str	r3, [fp, #-56]
.L355:
	ldr	r3, [fp, #-60]
	mov	r8, r10
	ldr	r7, [fp, #-52]
	ldr	r6, [fp, #-48]
	ldr	r2, [r3, #8]
	ldr	r3, [fp, #-56]
	add	r9, r3, r2, lsl #1
	mov	r10, r9
.L351:
	ldr	r5, [r6, #4]!
	mov	r9, r7
	ldr	r4, [fp, #-48]
	rsb	r5, r5, r10
	ubfx	r5, r5, #0, #9
.L353:
	ldr	r1, [r4, #4]!
	mov	r0, #512
	mov	r3, #1
	rsb	r1, r1, r10
	ubfx	r1, r1, #0, #9
	cmp	r1, #0
	beq	.L352
	bl	__aeabi_idiv
	mov	r3, r0
.L352:
	mul	r3, r3, r5
	cmp	r8, r4
	str	r3, [r9, #4]!
	bne	.L353
	cmp	r8, r6
	add	r7, r7, #16
	bne	.L351
	ldr	r3, [fp, #-56]
	add	r10, r8, #16
	add	r3, r3, #1
	str	r3, [fp, #-56]
	cmp	r3, #2
	ldr	r3, [fp, #-48]
	add	r3, r3, #16
	str	r3, [fp, #-48]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	str	r3, [fp, #-52]
	bne	.L355
.L376:
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L390:
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #460]
	ldr	r5, [r2, #8]
	cmp	r3, #1
	addeq	r9, r6, #520
	addeq	r7, r6, #32
	moveq	r8, r6
	mov	r5, r5, asl #1
	beq	.L342
	cmp	r3, #2
	bne	.L376
	ldr	r3, [r6, #524]
	rsb	r3, r3, r5
	ubfx	r4, r3, #0, #9
	cmp	r4, #0
	beq	.L348
	mov	r1, r4
	mov	r0, #512
	bl	__aeabi_idiv
.L348:
	mul	r3, r0, r4
	ldr	r4, [r6, #532]
	rsb	r4, r5, r4
	ubfx	r4, r4, #0, #9
	cmp	r4, #0
	str	r3, [r6, #556]
	moveq	r0, #1
	beq	.L349
	mov	r1, r4
	mov	r0, #512
	bl	__aeabi_idiv
.L349:
	mul	r4, r0, r4
	mov	r0, #1
	str	r4, [r6, #572]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L342:
	ldr	r1, [r6, #524]
	mov	r0, #512
	ldr	r4, [r9, #4]!
	mov	r3, #1
	rsb	r1, r1, r5
	ubfx	r1, r1, #0, #9
	rsb	r4, r4, r5
	cmp	r1, #0
	ubfx	r4, r4, #0, #9
	beq	.L344
	bl	__aeabi_idiv
	mov	r3, r0
.L344:
	mul	r3, r3, r4
	mov	r0, #1
	str	r3, [r8, #556]
	ldr	r1, [r6, #528]
	rsb	r1, r1, r5
	ubfx	r1, r1, #0, #9
	cmp	r1, #0
	beq	.L345
	mov	r0, #512
	bl	__aeabi_idiv
.L345:
	mul	r0, r0, r4
	add	r8, r8, #16
	str	r0, [r8, #544]
	cmp	r8, r7
	bne	.L342
	b	.L376
.L366:
	cmp	r3, #2
	bne	.L376
	ldr	r3, [r6, #76]
	mov	r7, r6
	add	r6, r6, #16
	add	r10, r7, #520
	mov	r8, #0
	str	r3, [fp, #-52]
.L365:
	ldr	r3, [fp, #-52]
	mov	r9, r10
	ldr	r4, [r3, #8]
	mov	r3, r7
	add	r4, r8, r4, lsl #1
.L356:
	ldr	r1, [r10, #4]
	mov	r0, #512
	ldr	r5, [r9, #4]!
	mov	ip, #1
	rsb	r1, r1, r4
	ubfx	r1, r1, #0, #9
	rsb	r5, r5, r4
	cmp	r1, #0
	ubfx	r5, r5, #0, #9
	beq	.L358
	str	r3, [fp, #-48]
	bl	__aeabi_idiv
	ldr	r3, [fp, #-48]
	mov	ip, r0
.L358:
	mul	ip, ip, r5
	mov	r0, #1
	str	ip, [r3, #556]
	ldr	r1, [r10, #8]
	rsb	r1, r1, r4
	ubfx	r1, r1, #0, #9
	cmp	r1, #0
	beq	.L359
	mov	r0, #512
	str	r3, [fp, #-48]
	bl	__aeabi_idiv
	ldr	r3, [fp, #-48]
.L359:
	mul	r0, r0, r5
	add	r3, r3, #8
	str	r0, [r3, #552]
	cmp	r3, r6
	bne	.L356
	add	r2, r10, #8
	mov	r9, r7
.L357:
	ldr	r1, [r10, #12]
	mov	r0, #512
	ldr	r5, [r2, #4]!
	mov	ip, #1
	rsb	r1, r4, r1
	ubfx	r1, r1, #0, #9
	rsb	r5, r4, r5
	cmp	r1, #0
	ubfx	r5, r5, #0, #9
	beq	.L362
	str	r2, [fp, #-48]
	bl	__aeabi_idiv
	ldr	r2, [fp, #-48]
	mov	ip, r0
.L362:
	mul	ip, ip, r5
	mov	r0, #1
	str	ip, [r9, #572]
	ldr	r1, [r10, #16]
	rsb	r1, r4, r1
	ubfx	r1, r1, #0, #9
	cmp	r1, #0
	beq	.L363
	mov	r0, #512
	str	r2, [fp, #-48]
	bl	__aeabi_idiv
	ldr	r2, [fp, #-48]
.L363:
	mul	r0, r0, r5
	add	r9, r9, #8
	str	r0, [r9, #568]
	cmp	r6, r9
	bne	.L357
	add	r8, r8, #1
	add	r6, r6, #32
	cmp	r8, #2
	add	r7, r7, #32
	add	r10, r10, #16
	bne	.L365
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	UNWIND(.fnend)
	.size	AVS_UpdateTableMvEScale, .-AVS_UpdateTableMvEScale
	.align	2
	.global	AVS_UpdateTableDirectScale
	.type	AVS_UpdateTableDirectScale, %function
AVS_UpdateTableDirectScale:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldrb	r3, [r0, #193]	@ zero_extendqisi2
	mov	r6, r0
	cmp	r3, #1
	beq	.L414
	ldr	r8, [r0, #80]
	mov	r5, #1
	add	r9, r0, #4
	add	r7, r0, #684
	mov	r4, #0
	ldr	r10, [r8, #8]
	mov	r10, r10, asl r5
.L398:
	sub	r3, r5, #1
	mov	r0, #16384
	and	r1, r5, #1
	add	r3, r3, r3, lsr #31
	mov	r3, r3, asr #1
	add	r3, r3, #21
	ldr	r3, [r6, r3, asl #2]
	cmp	r9, r3
	beq	.L396
	ldr	r4, [r3, #8]
	add	r4, r1, r4, lsl #1
	rsb	r1, r4, r10
	ubfx	r1, r1, #0, #9
	cmp	r1, #0
	moveq	r3, #32
	streq	r3, [r7]
	beq	.L396
	bl	__aeabi_idiv
	str	r0, [r7]
.L396:
	add	r5, r5, #1
	add	r7, r7, #4
	cmp	r5, #5
	bne	.L398
	ldr	r2, [r8, #8]
	add	r10, r6, #700
	ldr	ip, [r6, #800]
	mov	r5, #1
	mov	r7, #0
	cmp	ip, #0
	moveq	r2, r2, asl #1
	movne	r8, r2, asl #1
	addeq	r8, r2, #1
.L408:
	cmp	ip, #0
	beq	.L401
	add	r3, r7, r7, lsr #31
	mov	r3, r3, asr #1
	add	r3, r3, #21
	ldr	r3, [r6, r3, asl #2]
	cmp	r9, r3
	beq	.L402
	ldr	r4, [r3, #8]
	and	r3, r5, #1
	add	r4, r3, r4, lsl #1
	rsb	r0, r4, r8
	add	r0, r0, #512
	mov	r1, r0, asr #31
	mov	r1, r1, lsr #23
	add	r0, r0, r1
	ubfx	r3, r0, #0, #9
	subs	r1, r3, r1
	moveq	r3, #32
	streq	r3, [r10, #-16]
	beq	.L404
	mov	r0, #16384
	str	ip, [fp, #-48]
	bl	__aeabi_idiv
	ldr	ip, [fp, #-48]
	str	r0, [r10, #-16]
.L405:
	str	r0, [r10]
.L407:
	add	r5, r5, #1
	add	r7, r7, #1
	cmp	r5, #5
	add	r10, r10, #4
	bne	.L408
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L401:
	add	r3, r5, r5, lsr #31
	mov	r3, r3, asr #1
	add	r3, r3, #20
	ldr	r3, [r6, r3, asl #2]
	cmp	r9, r3
	ldrne	r4, [r3, #8]
	andne	r3, r7, #1
	addne	r4, r3, r4, lsl #1
.L402:
	rsb	r0, r4, r8
	add	r0, r0, #512
	mov	r1, r0, asr #31
	mov	r1, r1, lsr #23
	add	r0, r0, r1
	ubfx	r3, r0, #0, #9
	subs	r1, r3, r1
	beq	.L404
	mov	r0, #16384
	str	ip, [fp, #-48]
	bl	__aeabi_idiv
	ldr	ip, [fp, #-48]
	b	.L405
.L404:
	mov	r3, #32
	str	r3, [r10]
	b	.L407
.L414:
	ldr	r3, [r0, #80]
	add	r7, r0, #4
	add	r4, r0, #80
	add	r6, r0, #88
	mov	r8, #32
	ldr	r5, [r3, #8]
	mov	r5, r5, asl #1
.L395:
	ldr	r3, [r4, #4]!
	mov	r0, #16384
	cmp	r3, r7
	beq	.L393
	ldr	r1, [r3, #8]
	sub	r1, r5, r1, asl #1
	ubfx	r1, r1, #0, #9
	cmp	r1, #0
	streq	r8, [r4, #600]
	beq	.L393
	bl	__aeabi_idiv
	str	r0, [r4, #600]
.L393:
	cmp	r4, r6
	bne	.L395
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	UNWIND(.fnend)
	.size	AVS_UpdateTableDirectScale, .-AVS_UpdateTableDirectScale
	.align	2
	.global	AVS_UpdateTableSymScale
	.type	AVS_UpdateTableSymScale, %function
AVS_UpdateTableSymScale:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	ldr	r3, [r0, #84]
	add	r2, r0, #4
	mov	r5, r0
	cmp	r3, r2
	beq	.L422
	ldrb	r0, [r0, #193]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L417
	ldr	ip, [r5, #76]
	add	r2, r5, #520
	add	r5, r5, #712
	mov	r3, #0
.L420:
	ldr	r6, [ip, #8]
	add	r10, r2, #20
	mov	r9, r5
	mov	r8, r2
	mov	r7, #2
	add	r6, r3, r6, lsl #1
.L419:
	ldr	r1, [r8, #4]!
	mov	r0, #512
	ldr	r4, [r10, #-4]!
	mov	lr, #1
	rsb	r1, r1, r6
	ubfx	r1, r1, #0, #9
	rsb	r4, r6, r4
	cmp	r1, #0
	ubfx	r4, r4, #0, #9
	beq	.L418
	str	ip, [fp, #-56]
	str	r3, [fp, #-52]
	str	r2, [fp, #-48]
	bl	__aeabi_idiv
	ldr	r2, [fp, #-48]
	ldr	r3, [fp, #-52]
	ldr	ip, [fp, #-56]
	mov	lr, r0
.L418:
	mul	r4, lr, r4
	subs	r7, r7, #1
	str	r4, [r9, #4]!
	bne	.L419
	add	r3, r3, #1
	add	r2, r2, #16
	cmp	r3, #2
	add	r5, r5, #8
	bne	.L420
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L417:
	ldr	r2, [r5, #76]
	ldr	r1, [r3, #8]
	ldr	r3, [r5, #80]
	ldr	r2, [r2, #8]
	ldr	r4, [r3, #8]
	mov	r3, r2, asl #1
	sub	r1, r3, r1, asl #1
	ubfx	r1, r1, #0, #9
	rsb	r3, r3, r4, lsl #1
	cmp	r1, #0
	ubfx	r4, r3, #0, #9
	bne	.L427
.L421:
	mul	r3, r0, r4
	mov	r0, #1
	str	r3, [r5, #716]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L427:
	mov	r0, #512
	bl	__aeabi_idiv
	b	.L421
.L422:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	UNWIND(.fnend)
	.size	AVS_UpdateTableSymScale, .-AVS_UpdateTableSymScale
	.align	2
	.global	AVS_UpdateTabBBlkDist
	.type	AVS_UpdateTabBBlkDist, %function
AVS_UpdateTabBBlkDist:
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
	ldr	r2, [r0, #84]
	add	r1, r0, #4
	mov	r3, r0
	cmp	r2, r1
	beq	.L431
	ldrb	r0, [r0, #193]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L432
	ldr	r5, [r3, #76]
	mov	r0, #1
	ldr	r1, [r3, #536]
	ldr	r6, [r3, #524]
	ldr	r2, [r5, #8]
	ldr	lr, [r3, #532]
	ldr	ip, [r3, #528]
	mov	r2, r2, asl r0
	ldr	r4, [r3, #540]
	rsb	r6, r6, r2
	rsb	ip, ip, r2
	rsb	lr, r2, lr
	rsb	r2, r2, r1
	ubfx	lr, lr, #0, #9
	ubfx	r1, r6, #0, #9
	str	lr, [r3, #740]
	ubfx	r2, r2, #0, #9
	str	r1, [r3, #732]
	ubfx	r1, ip, #0, #9
	str	r2, [r3, #744]
	str	r1, [r3, #736]
	ldr	r2, [r5, #8]
	ldr	r1, [r3, #552]
	ldr	lr, [r3, #548]
	ldr	ip, [r3, #544]
	mov	r2, r2, asl r0
	add	r2, r2, r0
	rsb	r4, r4, r2
	rsb	ip, ip, r2
	rsb	lr, r2, lr
	rsb	r2, r2, r1
	ubfx	lr, lr, #0, #9
	ubfx	r1, r4, #0, #9
	ubfx	r2, r2, #0, #9
	str	r1, [r3, #748]
	str	lr, [r3, #756]
	ubfx	r1, ip, #0, #9
	str	r2, [r3, #760]
	str	r1, [r3, #752]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L432:
	ldr	r1, [r3, #76]
	ldr	lr, [r3, #80]
	ldr	ip, [r2, #8]
	ldr	r1, [r1, #8]
	ldr	lr, [lr, #8]
	mov	r2, r1, asl #1
	sub	r1, r2, ip, asl #1
	rsb	r2, r2, lr, lsl #1
	ubfx	r1, r1, #0, #9
	ubfx	r2, r2, #0, #9
	str	r1, [r3, #732]
	str	r2, [r3, #740]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L431:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	AVS_UpdateTabBBlkDist, .-AVS_UpdateTabBBlkDist
	.align	2
	.global	AVS_UpdatePmvTable
	.type	AVS_UpdatePmvTable, %function
AVS_UpdatePmvTable:
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
	ldr	r4, [r0, #460]
	mov	r5, r0
	cmp	r4, #1
	beq	.L442
	bcc	.L436
	cmp	r4, #2
	bne	.L444
	bl	AVS_UpdateTableIdx2Dist
	mov	r0, r5
	bl	AVS_UpdateTableMvEScale
	mov	r0, r5
	bl	AVS_UpdateTableDirectScale
	mov	r0, r5
	bl	AVS_UpdateTabBBlkDist
	mov	r0, r5
	bl	AVS_UpdateTableSymScale
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L444:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L436:
	ldrb	r4, [r0, #193]	@ zero_extendqisi2
	cmp	r4, #1
	beq	.L441
.L442:
	bl	AVS_UpdateTableIdx2Dist
	mov	r0, r5
	bl	AVS_UpdateTableMvEScale
.L441:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	AVS_UpdatePmvTable, .-AVS_UpdatePmvTable
	.align	2
	.global	AVS_GetStreamInf
	.type	AVS_GetStreamInf, %function
AVS_GetStreamInf:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	add	r4, r0, #12288
	mov	r6, r0
	ldr	r7, .L537
	ldr	r2, [r4, #2824]
	ldrb	r3, [r4, #2836]	@ zero_extendqisi2
	cmp	r2, #1
	str	r3, [r4, #3504]
	beq	.L446
	add	r5, r0, #16384
	ldr	r3, [r5, #856]
	cmp	r3, #1
	beq	.L447
.L446:
	ldr	r3, [r7, #68]
	mov	r0, #22
	ldr	r1, .L537+4
	blx	r3
	ldrb	r3, [r4, #2839]	@ zero_extendqisi2
	ldr	r2, [r4, #2848]
	cmp	r3, #1
	ldr	r3, [r4, #2852]
	add	r2, r2, #15
	addne	r3, r3, #31
	addeq	r3, r3, #15
	mov	r2, r2, lsr #4
	str	r2, [r4, #2804]
	movne	r3, r3, lsr #5
	moveq	r3, r3, lsr #4
	movne	r3, r3, asl #1
	cmp	r2, #512
	str	r3, [r4, #2808]
	bgt	.L450
	cmp	r3, #512
	ble	.L451
.L450:
	ldr	ip, [r7, #68]
	mov	r0, #18
	ldr	r1, .L537+8
	mov	r8, #0
	blx	ip
.L452:
	mov	r0, r8
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L451:
	ldr	r3, [r4, #2788]
	add	r5, r6, #16384
	ldr	r0, [r5, #344]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	addeq	r9, r6, #14976
	addeq	r9, r9, #24
	beq	.L454
	ldr	r3, [r4, #2784]
	mov	r2, #1
	ldr	r0, [r5, #344]
	add	r9, r6, #14976
	add	r9, r9, #24
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2788]
	mov	r2, #1
	ldr	r0, [r5, #344]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2792]
	mov	r2, #1
	ldr	r0, [r5, #344]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2796]
	mov	r2, #1
	ldr	r0, [r5, #344]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r1, [r8, #212]
	ldr	r0, [r5, #344]
	bl	FreeUsdByDec
	ldr	r1, [r8, #216]
	ldr	r0, [r5, #344]
	bl	FreeUsdByDec
	ldr	r1, [r8, #220]
	ldr	r0, [r5, #344]
	bl	FreeUsdByDec
	ldr	r1, [r8, #224]
	ldr	r0, [r5, #344]
	bl	FreeUsdByDec
	mov	r3, #0
	str	r3, [r8, #212]
	str	r3, [r8, #216]
	str	r3, [r8, #220]
	str	r3, [r8, #224]
	str	r9, [r4, #2784]
	str	r9, [r4, #2796]
	str	r9, [r4, #2792]
	str	r9, [r4, #2788]
	ldr	r0, [r5, #344]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #2716]
.L454:
	add	r8, r6, #16192
	mov	r3, #0
	add	r8, r8, #16
	str	r3, [r4, #3476]
	ldr	r0, [r5, #344]
	mov	r1, r8
	bl	FSP_ClearNotInVoQueue
	ldr	r3, [r6]
	ldr	r2, [r3, #776]
	cmp	r2, #0
	bne	.L455
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L524
.L455:
	ldr	r0, [r5, #344]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L521
.L457:
	mov	r8, #0
	str	r9, [r4, #2784]
	str	r8, [r4, #2824]
	mov	r0, r6
	str	r9, [r4, #2796]
	str	r9, [r4, #2792]
	str	r9, [r4, #2788]
	bl	AVS_AllocateVHB
	cmp	r0, #3
	mov	ip, r0
	moveq	r8, r0
	beq	.L452
	cmp	r0, r8
	strne	r8, [r4, #2824]
	beq	.L525
.L447:
	ldrb	r1, [r4, #2835]	@ zero_extendqisi2
	mov	r0, r6
	ldr	r3, [r4, #2784]
	ldrb	r2, [r4, #2901]	@ zero_extendqisi2
	str	r1, [r4, #3500]
	strb	r2, [r3]
	bl	AVS_GetImageBuffer
	subs	r8, r0, #0
	beq	.L526
	ldr	r3, [r4, #2784]
	ldr	r0, [r5, #344]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L527
	ldr	lr, [r6]
	mvn	r2, #0
	mvn	r3, #0
	ldr	r1, .L537+12
	mov	r0, #29
	ldrd	r8, [lr, #48]
	strd	r8, [ip, #8]
	ldr	lr, [r6]
	ldrd	r8, [lr, #64]
	strd	r8, [ip, #16]
	ldr	lr, [r6]
	ldrd	r8, [lr, #80]
	strd	r8, [ip, #24]
	ldr	lr, [r6]
	ldr	lr, [lr, #96]
	str	lr, [ip, #68]
	ldr	lr, [r6]
	ldr	lr, [lr, #100]
	str	lr, [ip, #72]
	ldr	lr, [r6]
	ldr	lr, [lr, #104]
	str	lr, [ip, #76]
	ldr	lr, [r6]
	ldr	lr, [lr, #120]
	str	lr, [ip, #80]
	ldrb	lr, [r4, #2901]	@ zero_extendqisi2
	clz	lr, lr
	mov	lr, lr, lsr #5
	str	lr, [ip, #148]
	ldr	lr, [r6]
	strd	r2, [lr, #48]
	ldrd	r8, [ip, #16]
	ldrd	r2, [ip, #8]
	ldr	ip, [r7, #68]
	strd	r8, [sp]
	blx	ip
	ldr	r3, [r4, #2784]
	ldr	r2, [r4, #2932]
	str	r2, [r3, #8]
	ldr	r3, [r4, #2784]
	ldrb	r2, [r4, #2901]	@ zero_extendqisi2
	strb	r2, [r3]
	ldr	r3, [r4, #2784]
	ldr	r2, [r4, #3168]
	strb	r2, [r3, #1]
	ldrb	r2, [r4, #2901]	@ zero_extendqisi2
	ldr	r3, [r4, #2784]
	cmp	r2, #0
	ldrneb	r2, [r4, #2902]	@ zero_extendqisi2
	moveq	r2, #1
	strb	r2, [r3, #2]
	ldrb	r3, [r4, #2901]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L522
	ldr	r8, [r4, #3168]
	cmp	r8, #0
	beq	.L519
.L522:
	add	r9, r6, #14976
	add	r9, r9, #20
.L463:
	ldr	r3, [r4, #3164]
	mov	r0, r9
	str	r3, [r4, #3508]
	bl	AVS_UpdatePmvTable
	ldr	r8, [r4, #2804]
	ldr	ip, [r4, #2808]
	mov	r2, #0
	mov	r3, #1
	str	r2, [r5, #352]
	str	r3, [r5, #356]
	mov	lr, #6
	mul	r1, ip, r8
	add	r3, r6, #15296
	add	r2, r6, #16896
	add	r0, r6, #15424
	add	r3, r3, #14
	add	r2, r2, #214
	sub	r1, r1, #1
	str	r1, [r5, #348]
	ldr	r1, [r4, #3224]
	add	r0, r0, #14
	str	r8, [r5, #408]
	cmp	r1, #0
	add	r8, r1, #63
	str	ip, [r5, #404]
	movlt	r1, r8
	str	lr, [r5, #364]
	mov	r1, r1, asr lr
	str	r1, [r5, #360]
	ldr	r1, [r4, #3504]
	str	r1, [r5, #412]
	ldr	r1, [r4, #3500]
	str	r1, [r5, #416]
	ldrb	r1, [r4, #2914]	@ zero_extendqisi2
	str	r1, [r5, #420]
	ldrb	r1, [r4, #2913]	@ zero_extendqisi2
	str	r1, [r5, #424]
	ldrb	r1, [r4, #2912]	@ zero_extendqisi2
	str	r1, [r5, #428]
	ldrb	r1, [r4, #2906]	@ zero_extendqisi2
	str	r1, [r5, #432]
	ldrb	r1, [r4, #2905]	@ zero_extendqisi2
	str	r1, [r5, #436]
	ldrb	r1, [r4, #2904]	@ zero_extendqisi2
	str	r1, [r5, #440]
	ldrb	r1, [r4, #2903]	@ zero_extendqisi2
	str	r1, [r5, #444]
	ldrb	r1, [r4, #2902]	@ zero_extendqisi2
	str	r1, [r5, #448]
	ldr	r1, [r4, #3168]
	str	r1, [r5, #452]
	ldrb	r1, [r4, #2901]	@ zero_extendqisi2
	cmp	r1, #0
	moveq	r1, #1
	movne	r1, #3
	str	r1, [r5, #456]
	ldrb	r1, [r4, #2900]	@ zero_extendqisi2
	str	r1, [r5, #460]
	ldr	r1, [r4, #2944]
	str	r1, [r5, #464]
	ldr	r1, [r4, #2940]
	str	r1, [r5, #468]
	ldrb	r1, [r4, #2909]	@ zero_extendqisi2
	str	r1, [r5, #472]
	ldrb	r1, [r4, #2908]	@ zero_extendqisi2
	str	r1, [r5, #476]
	ldr	r1, [r4, #2948]
	str	r1, [r5, #552]
	ldr	r1, [r4, #2956]
	str	r1, [r5, #556]
	ldr	r1, [r4, #2960]
	str	r1, [r5, #564]
	ldr	r1, [r4, #2964]
	str	r1, [r5, #560]
.L466:
	ldrh	r1, [r3, #2]!
	cmp	r3, r0
	strh	r1, [r2, #2]!	@ movhi
	bne	.L466
	ldr	r3, [r4, #3152]
	ldr	r0, [r5, #344]
	str	r3, [r5, #568]
	ldr	r3, [r4, #3164]
	str	r3, [r5, #576]
	ldr	r3, [r4, #3160]
	str	r3, [r5, #572]
	ldr	r3, [r4, #2784]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	cmp	r3, #2
	ldr	r3, [r4, #2788]
	ldr	r1, [r3, #4]
	beq	.L528
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L529
	ldr	r3, [r4, #2788]
	ldr	r0, [r5, #344]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r2, #3
	movne	r2, #1
	str	r2, [r5, #480]
	ldrsb	r2, [r3, #2]
	str	r2, [r5, #484]
	ldr	r2, [ip, #104]
	str	r2, [r5, #504]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r2, #3
	movne	r2, #1
	str	r2, [r5, #496]
	ldrsb	r3, [r3, #2]
	str	r3, [r5, #500]
	ldr	r3, [ip, #104]
	str	r3, [r5, #508]
	ldr	r3, [r4, #2792]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	ldrne	r3, [r4, #2792]
	beq	.L530
.L523:
	ldrb	r2, [r3]	@ zero_extendqisi2
	ldr	r0, [r5, #344]
	cmp	r2, #1
	moveq	r2, #3
	movne	r2, #1
	str	r2, [r5, #488]
	ldrsb	r3, [r3, #2]
	str	r3, [r5, #492]
	ldr	r3, [ip, #104]
	str	r3, [r5, #512]
	ldr	r3, [r4, #2788]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L531
	ldr	r3, [ip, #148]
	ldr	r0, [r5, #344]
	str	r3, [r5, #384]
	ldr	r3, [r4, #2792]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L532
	ldr	r3, [ip, #148]
	ldr	r0, [r5, #344]
	str	r3, [r5, #388]
	ldr	r3, [r4, #2796]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L533
	ldr	r3, [ip, #148]
	ldr	r0, [r5, #344]
	str	r3, [r5, #392]
	ldr	r3, [r4, #2784]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L534
	ldr	r3, [ip, #148]
	ldr	r0, [r5, #344]
	str	r3, [r5, #396]
	ldr	r3, [ip, #104]
	str	r3, [r5, #400]
	ldr	r3, [ip, #120]
	str	r3, [r5, #516]
	ldr	r3, [r4, #2788]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r2, #3
	movne	r2, #1
	str	r2, [r5, #520]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	adds	r3, r3, #0
	movne	r3, #1
	str	r3, [r5, #524]
	ldr	r3, [r4, #2784]
	ldr	r1, [r3, #4]
	bl	FSP_GetPmvAddr
	str	r0, [r5, #580]
	ldr	r3, [r4, #2788]
	ldr	r0, [r5, #344]
	ldr	r1, [r3, #4]
	bl	FSP_GetPmvAddr
	add	ip, r6, #15488
	add	ip, ip, #60
	mov	r2, #0
	str	r0, [r5, #592]
	add	r0, r6, #16896
	ldr	r3, [r4, #3168]
	add	r0, r0, #80
	cmp	r3, #2
	movne	r1, #0
	moveq	r1, #4
	moveq	lr, #8
	moveq	r7, #16
	movne	r7, #32
	movne	lr, r1
.L487:
	add	r2, r2, #1
	ldr	r3, [ip, #4]!
	cmp	r2, r7
	ubfx	r3, r3, #0, #18
	str	r3, [r0, #4]!
	blt	.L487
	cmp	lr, #0
	ble	.L492
	add	ip, r6, #15616
	add	r0, r6, #16896
	add	ip, ip, #60
	add	r0, r0, #144
	mov	r2, #0
.L491:
	add	r2, r2, #1
	ldr	r3, [ip, #4]!
	cmp	r2, lr
	ubfx	r3, r3, #0, #15
	str	r3, [r0, #4]!
	bne	.L491
.L492:
	cmp	r1, #0
	ble	.L490
	add	r0, r6, #15680
	add	lr, r6, #16896
	add	r0, r0, #48
	add	lr, lr, #176
	mov	ip, #0
.L493:
	ldr	r2, [r0, #4]
	add	ip, ip, #1
	ldr	r3, [r0], #8
	cmp	ip, r1
	ubfx	r2, r2, #0, #9
	ubfx	r3, r3, #0, #9
	orr	r3, r3, r2, asl #9
	str	r3, [lr, #4]!
	bne	.L493
	add	r0, r6, #15680
	add	r6, r6, #16896
	add	r0, r0, #28
	add	r6, r6, #192
	mov	r2, #0
.L494:
	add	r2, r2, #1
	ldr	r3, [r0, #4]!
	cmp	r2, r1
	ubfx	r3, r3, #0, #18
	str	r3, [r6, #4]!
	bne	.L494
.L490:
	ldr	r3, [r4, #3220]
	mov	r8, #1
	mov	r0, r8
	str	r3, [r5, #724]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L526:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L537+16
	blx	r3
	mov	r0, r8
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L524:
	ldr	r3, [r5, #856]
	cmp	r3, #0
	bne	.L457
.L521:
	mov	r0, r8
	bl	ResetVoQueue
	b	.L457
.L528:
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L535
	ldr	r3, [r4, #2788]
	ldr	r0, [r5, #344]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r2, #3
	movne	r2, #1
	str	r2, [r5, #480]
	ldrsb	r3, [r3, #2]
	str	r3, [r5, #484]
	ldr	r3, [ip, #104]
	str	r3, [r5, #504]
	ldr	r3, [r4, #2792]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L536
	ldr	r3, [r4, #2792]
	ldr	r0, [r5, #344]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r2, #3
	movne	r2, #1
	str	r2, [r5, #496]
	ldrsb	r3, [r3, #2]
	str	r3, [r5, #500]
	ldr	r3, [ip, #104]
	str	r3, [r5, #508]
	ldr	r3, [r4, #2796]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	ldrne	r3, [r4, #2796]
	bne	.L523
	ldr	r3, [r7, #68]
	movw	r2, #1967
	ldr	r1, .L537+20
	mov	r8, ip
	blx	r3
	b	.L452
.L525:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L537+24
	mov	r8, ip
	blx	r3
	b	.L452
.L527:
	ldr	r3, [r7, #68]
	movw	r2, #1860
	ldr	r1, .L537+20
	mov	r8, ip
	blx	r3
	b	.L452
.L536:
	ldr	r3, [r7, #68]
	movw	r2, #1961
	ldr	r1, .L537+20
	mov	r8, ip
	blx	r3
	b	.L452
.L519:
	ldr	r3, [r4, #2784]
	mov	r2, #1
	ldr	r0, [r5, #344]
	add	r9, r6, #14976
	add	r9, r9, #20
	ldr	r1, [r3, #4]
	bl	FSP_SetRef
	ldr	r3, [r4, #2796]
	add	r2, r9, #4
	cmp	r3, r2
	beq	.L464
	mov	r2, r8
	ldr	r1, [r3, #4]
	ldr	r0, [r5, #344]
	bl	FSP_SetRef
.L464:
	ldr	r1, [r4, #2792]
	ldr	r2, [r4, #2788]
	ldr	r3, [r4, #2784]
	str	r1, [r4, #2796]
	str	r2, [r4, #2792]
	str	r3, [r4, #2788]
	b	.L463
.L531:
	ldr	r3, [r7, #68]
	movw	r2, #1991
	ldr	r1, .L537+20
	mov	r8, ip
	blx	r3
	b	.L452
.L532:
	ldr	r3, [r7, #68]
	movw	r2, #1995
	ldr	r1, .L537+20
	mov	r8, ip
	blx	r3
	b	.L452
.L533:
	ldr	r3, [r7, #68]
	movw	r2, #1999
	ldr	r1, .L537+20
	mov	r8, ip
	blx	r3
	b	.L452
.L529:
	ldr	r3, [r7, #68]
	movw	r2, #1975
	ldr	r1, .L537+20
	mov	r8, ip
	blx	r3
	b	.L452
.L530:
	ldr	r3, [r7, #68]
	mov	r2, #1984
	ldr	r1, .L537+20
	mov	r8, ip
	blx	r3
	b	.L452
.L534:
	ldr	r3, [r7, #68]
	movw	r2, #2003
	ldr	r1, .L537+20
	mov	r8, ip
	blx	r3
	b	.L452
.L535:
	ldr	r3, [r7, #68]
	movw	r2, #1955
	ldr	r1, .L537+20
	mov	r8, ip
	blx	r3
	b	.L452
.L538:
	.align	2
.L537:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC99
	.word	.LC100
	.word	.LC103
	.word	.LC102
	.word	.LC97
	.word	.LC101
	UNWIND(.fnend)
	.size	AVS_GetStreamInf, .-AVS_GetStreamInf
	.align	2
	.global	AVS_PrepareDecode
	.type	AVS_PrepareDecode, %function
AVS_PrepareDecode:
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
	bl	AVS_GetStreamInf
	cmp	r0, #3
	ldmeqfd	sp, {fp, sp, pc}
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	AVS_PrepareDecode, .-AVS_PrepareDecode
	.align	2
	.global	AVS_VerifyMemPara
	.type	AVS_VerifyMemPara, %function
AVS_VerifyMemPara:
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
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	AVS_VerifyMemPara, .-AVS_VerifyMemPara
	.align	2
	.global	AVSDEC_Init
	.type	AVSDEC_Init, %function
AVSDEC_Init:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r7, r0, #16384
	ldr	r9, .L547
	add	r6, r0, #16192
	mov	r8, r1
	ldr	r4, [r7, #168]
	add	r6, r6, #16
	movw	r2, #17248
	ldr	r3, [r9, #48]
	mov	r1, #0
	mov	r5, r0
	blx	r3
	str	r4, [r7, #168]
	mov	r0, r6
	add	r4, r5, #14976
	bl	ResetVoQueue
	str	r8, [r5]
	mov	r0, r5
	add	r8, r5, #15744
	bl	VCTRL_GetChanIDByCtx
	add	r4, r4, #20
	add	r8, r8, #60
	cmn	r0, #1
	str	r0, [r7, #344]
	beq	.L546
	ldr	r3, [r9, #48]
	mov	r2, #808
	mov	r1, #0
	mov	r0, r4
	blx	r3
	add	r3, r5, #12288
	add	r4, r4, #4
	mov	r1, #1024
	str	r4, [r3, #2784]
	mov	r2, #0
	str	r8, [r3, #3200]
	mov	r0, #1
	str	r4, [r3, #2796]
	str	r4, [r3, #2792]
	str	r4, [r3, #2788]
	str	r6, [r3, #3228]
	str	r1, [r3, #3224]
	ldr	r3, [r5]
	str	r2, [r3, #892]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L546:
	ldr	r3, [r9, #68]
	mov	r0, #0
	ldr	r1, .L547+4
	blx	r3
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L548:
	.align	2
.L547:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC104
	UNWIND(.fnend)
	.size	AVSDEC_Init, .-AVSDEC_Init
	.align	2
	.global	AVSDEC_Destroy
	.type	AVSDEC_Destroy, %function
AVSDEC_Destroy:
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
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	AVSDEC_Destroy, .-AVSDEC_Destroy
	.align	2
	.global	AVSDEC_RecycleImage
	.type	AVSDEC_RecycleImage, %function
AVSDEC_RecycleImage:
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
	cmp	r0, #0
	mov	r5, r1
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r4, r0, #16384
	mov	r2, #0
	ldr	r0, [r4, #344]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #344]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L553
	ldr	r1, [r5, #212]
	mov	r6, #0
	ldr	r0, [r4, #344]
	bl	FreeUsdByDec
	str	r6, [r5, #212]
	ldr	r1, [r5, #216]
	ldr	r0, [r4, #344]
	bl	FreeUsdByDec
	str	r6, [r5, #216]
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L553:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	AVSDEC_RecycleImage, .-AVSDEC_RecycleImage
	.align	2
	.global	AVSDEC_GetRemainImg
	.type	AVSDEC_GetRemainImg, %function
AVSDEC_GetRemainImg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r3, [r0]
	mov	r6, r0
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L560
	add	r10, r0, #15040
	add	r9, r0, #16192
	add	r10, r10, #48
	add	r9, r9, #16
	add	r5, r0, #16384
	mov	r4, #3
	mvn	r7, #0
.L559:
	ldr	r3, [r10, #-4]!
	ldr	r0, [r5, #344]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L556
	ldr	r3, [r10]
	ldr	r0, [r5, #344]
	ldr	r1, [r3, #4]
	bl	FSP_GetDisplay
	cmp	r0, #2
	movne	r7, #1
	beq	.L568
.L556:
	subs	r4, r4, #1
	bne	.L559
	cmp	r7, #0
	bne	.L560
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L560:
	add	r0, r6, #16192
	add	r0, r0, #16
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	GetVoLastImageID
.L568:
	mov	r0, r6
	bl	AVS_SetImgFormat
	cmp	r4, #1
	streq	r4, [r8, #244]
	mov	r2, #1
	ldr	r3, [r10]
	ldr	r0, [r5, #344]
	ldr	r1, [r3, #4]
	bl	FSP_SetDisplay
	ldr	r0, [r5, #344]
	mov	r3, r9
	str	r8, [sp]
	mov	r2, r6
	mov	r1, #6
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L558
	ldr	r1, [r8, #196]
	mov	r0, r6
	bl	AVSDEC_RecycleImage
	ldr	r1, [r8, #212]
	ldr	r0, [r5, #344]
	mvn	r7, #0
	bl	FreeUsdByDec
	mov	r3, #0
	ldr	r1, [r8, #216]
	str	r3, [r8, #212]
	ldr	r0, [r5, #344]
	bl	FreeUsdByDec
	mov	r3, #0
	str	r3, [r8, #216]
	b	.L556
.L558:
	ldr	r3, [r8, #200]
	cmp	r3, #100
	bhi	.L563
	ldr	r3, [r8, #164]
	cmp	r3, #31
	bhi	.L569
.L563:
	mvn	r7, #0
	b	.L556
.L569:
	ldr	r7, [r8, #168]
	cmp	r7, #31
	movhi	r7, #0
	mvnls	r7, #0
	b	.L556
	UNWIND(.fnend)
	.size	AVSDEC_GetRemainImg, .-AVSDEC_GetRemainImg
	.align	2
	.global	AVSDEC_GetImageBuffer
	.type	AVSDEC_GetImageBuffer, %function
AVSDEC_GetImageBuffer:
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
	add	r4, r0, #16384
	mov	r5, r0
	ldr	r0, [r4, #344]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #344]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L576
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L576:
	add	r1, r5, #16192
	ldr	r0, [r4, #344]
	add	r1, r1, #16
	bl	FSP_ClearNotInVoQueue
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	AVSDEC_GetImageBuffer, .-AVSDEC_GetImageBuffer
	.align	2
	.global	ClearSlice
	.type	ClearSlice, %function
ClearSlice:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r4, r1, #4
	add	r8, r1, #12
	mov	r7, r1
	mov	r9, r0
	mov	r6, r1
	mov	r5, #0
.L579:
	ldr	r3, [r4, #4]!
	cmp	r3, #0
	ble	.L578
	ldr	r3, [r6]
	cmp	r3, #0
	bne	.L584
.L578:
	cmp	r4, r8
	str	r5, [r4]
	str	r5, [r6], #4
	str	r5, [r7, #16]
	bne	.L579
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L584:
	ldr	r1, [r4, #12]
	mov	r0, r9
	bl	SM_ReleaseStreamSeg
	b	.L578
	UNWIND(.fnend)
	.size	ClearSlice, .-ClearSlice
	.align	2
	.global	AVS_ClearCurPic
	.type	AVS_ClearCurPic, %function
AVS_ClearCurPic:
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
	add	r7, r0, #12288
	ldr	r3, [r7, #2264]
	cmp	r3, #0
	ble	.L588
	add	r5, r0, #216
	add	r6, r0, #16384
	mov	r4, #0
.L587:
	mov	r1, r5
	ldr	r0, [r6, #344]
	bl	ClearSlice
	ldr	r3, [r7, #2264]
	add	r4, r4, #1
	add	r5, r5, #28
	cmp	r3, r4
	bgt	.L587
.L588:
	mov	r3, #0
	mov	r0, #1
	str	r3, [r7, #2264]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	AVS_ClearCurPic, .-AVS_ClearCurPic
	.align	2
	.type	AVS_DecodeArbiter.part.3, %function
AVS_DecodeArbiter.part.3:
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
	ldr	r3, [r0]
	mov	r6, r0
	add	r4, r0, #12288
	ldr	r3, [r3, #8]
	cmp	r3, #4
	ldrls	pc, [pc, r3, asl #2]
	b	.L606
.L594:
	.word	.L593
	.word	.L595
	.word	.L596
	.word	.L606
	.word	.L593
.L596:
	ldr	r5, [r4, #3888]
	clz	r5, r5
	mov	r5, r5, lsr #5
.L592:
	cmp	r5, #0
	str	r3, [r4, #3472]
	beq	.L612
.L599:
	ldr	r3, [r4, #3888]
	cmp	r3, #1
	beq	.L613
.L602:
	cmp	r3, #2
	bne	.L604
	ldr	r3, [r4, #3476]
	cmp	r3, #1
	ble	.L603
.L604:
	add	r1, r4, #3536
	add	r0, r4, #2816
	add	r1, r1, #12
	mov	r2, #372
	add	r0, r0, #12
	bl	memcpy
	ldrb	lr, [r4, #2901]	@ zero_extendqisi2
	ldr	r1, .L614
	mov	r0, #18
	ldr	r3, [r4, #3168]
	ldr	r2, [r4, #3488]
	ldr	ip, [r1, #68]
	ldr	r1, .L614+4
	str	lr, [sp]
	blx	ip
	ldr	r3, [r4, #2812]
	mov	r0, #1
	bic	r3, r3, #311296
	str	r3, [r4, #2812]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L595:
	ldr	r2, [r4, #3472]
	ldr	r5, [r4, #3888]
	cmp	r2, #2
	str	r3, [r4, #3472]
	moveq	r2, #0
	streq	r2, [r4, #3476]
	subs	r5, r5, #2
	movne	r5, #1
	cmp	r5, #0
	bne	.L599
.L612:
	mov	r0, r6
	bl	AVS_ClearCurPic
	ldr	r2, .L614+8
	ldr	r3, [r4, #2812]
	ldr	ip, [r2]
	bic	r3, r3, #311296
	str	r3, [r4, #2812]
	cmp	ip, #0
	beq	.L611
	add	r6, r6, #16384
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	ldr	r0, [r6, #344]
	blx	ip
	mov	r0, r5
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L593:
	ldr	r2, [r4, #3472]
	str	r3, [r4, #3472]
	cmp	r2, #2
	ldr	r3, [r4, #3888]
	moveq	r2, #0
	streq	r2, [r4, #3476]
	cmp	r3, #1
	bne	.L602
.L613:
	ldr	r3, [r4, #3476]
	cmp	r3, #0
	bgt	.L604
.L603:
	mov	r0, r6
	bl	AVS_ClearCurPic
	ldr	r2, .L614+8
	ldr	r3, [r4, #2812]
	ldr	ip, [r2]
	bic	r3, r3, #311296
	str	r3, [r4, #2812]
	cmp	ip, #0
	beq	.L611
	add	r6, r6, #16384
	mov	r3, #0
	mov	r2, r3
	mov	r1, #113
	ldr	r0, [r6, #344]
	blx	ip
.L611:
	mov	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L606:
	mov	r5, #1
	b	.L592
.L615:
	.align	2
.L614:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC105
	.word	g_event_report
	UNWIND(.fnend)
	.size	AVS_DecodeArbiter.part.3, .-AVS_DecodeArbiter.part.3
	.align	2
	.global	AVS_DecodeArbiter
	.type	AVS_DecodeArbiter, %function
AVS_DecodeArbiter:
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
	add	r3, r0, #12288
	ldr	r3, [r3, #2812]
	and	r3, r3, #376832
	cmp	r3, #376832
	beq	.L618
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L618:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	AVS_DecodeArbiter.part.3
	UNWIND(.fnend)
	.size	AVS_DecodeArbiter, .-AVS_DecodeArbiter
	.align	2
	.global	AVSDEC_VDMPostProc
	.type	AVSDEC_VDMPostProc, %function
AVSDEC_VDMPostProc:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r7, .L675
	add	r4, r0, #12288
	mov	r5, r0
	mov	r8, r1
	bl	AVS_ClearCurPic
	add	r0, r5, #8
	ldr	r3, [r7, #48]
	movw	r2, #14988
	mov	r1, #0
	blx	r3
	ldr	r3, [r4, #3168]
	cmp	r3, #0
	bne	.L620
	str	r8, [r4, #3480]
	add	r6, r5, #16384
	ldr	r3, [r5]
	ldr	r2, [r4, #3480]
	ldr	r3, [r3]
	cmp	r3, r2
	blt	.L669
.L625:
	mov	r0, r5
	bl	AVS_SetImgFormat
	ldr	r3, [r4, #2784]
	ldr	r0, [r6, #344]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L670
	ldr	r3, [r4, #3492]
	mov	r2, #0
	cmp	r3, #0
	strne	r3, [ip, #212]
	ldr	r3, [r4, #3496]
	str	r2, [r4, #3492]
	cmp	r3, r2
	mov	r2, #0
	strne	r3, [ip, #216]
	ldr	r3, [r4, #3168]
	str	r2, [r4, #3496]
	cmp	r3, #1
	bls	.L671
.L630:
	ldr	r9, [r4, #2784]
.L635:
	ldr	r1, [r9, #4]
	ldr	r0, [r6, #344]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L672
	add	r3, r5, #14976
	add	r3, r3, #24
	cmp	r9, r3
	beq	.L640
	ldr	r3, [r5]
	ldr	r2, [r4, #3480]
	ldr	r3, [r3, #4]
	cmp	r3, r2
	bge	.L638
	ldr	ip, [r7, #68]
	mov	r0, #1
	ldr	r1, .L675+4
	blx	ip
	ldr	r1, [r9, #4]
	mov	r2, #0
	ldr	r0, [r6, #344]
	bl	FSP_SetDisplay
	ldr	r1, [r8, #212]
	cmp	r1, #0
	beq	.L639
	ldr	r0, [r6, #344]
	bl	FreeUsdByDec
	mov	r3, #0
	str	r3, [r8, #212]
.L639:
	ldr	r1, [r8, #216]
	cmp	r1, #0
	beq	.L640
	ldr	r0, [r6, #344]
	mov	r4, #1
	bl	FreeUsdByDec
	mov	r3, #0
	str	r3, [r8, #216]
.L623:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L638:
	mov	r1, r5
	mov	r0, r9
	bl	AVS_OutputOneFrame
	subs	r4, r0, #0
	bne	.L640
	ldr	r1, [r8, #212]
	cmp	r1, #0
	beq	.L641
	ldr	r0, [r6, #344]
	bl	FreeUsdByDec
	str	r4, [r8, #212]
.L641:
	ldr	r1, [r8, #216]
	cmp	r1, #0
	beq	.L623
	ldr	r0, [r6, #344]
	bl	FreeUsdByDec
	mov	r3, #0
	mov	r0, r4
	str	r3, [r8, #216]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L640:
	mov	r4, #1
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L620:
	ldr	r3, [r4, #2788]
	add	r6, r5, #16384
	ldr	r0, [r6, #344]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	beq	.L673
	ldr	r2, [r4, #2792]
	ldr	r0, [r6, #344]
	ldr	r9, [r3, #200]
	ldr	r1, [r2, #4]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	beq	.L674
	ldr	r1, [r3, #200]
	ldr	r2, [r4, #3168]
	add	r1, r9, r1
	add	r1, r8, r1
	cmp	r1, #100
	movge	r1, #100
	str	r1, [r4, #3480]
	ldr	r3, [r5]
	cmp	r2, #2
	ldr	r3, [r3]
	beq	.L625
	ldr	r2, [r4, #3480]
	cmp	r3, r2
	bge	.L625
.L669:
	ldr	ip, [r7, #68]
	mov	r0, #1
	ldr	r1, .L675+8
	mov	r8, #0
	blx	ip
	ldr	r3, [r4, #2788]
	str	r8, [r4, #3476]
	ldr	r0, [r6, #344]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L626
	ldr	r3, [r4, #2784]
	mov	r2, #1
	ldr	r0, [r6, #344]
	add	r5, r5, #14976
	add	r5, r5, #24
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2788]
	mov	r2, #1
	ldr	r0, [r6, #344]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2792]
	mov	r2, #1
	ldr	r0, [r6, #344]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2796]
	mov	r2, #1
	ldr	r0, [r6, #344]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r1, [r7, #212]
	ldr	r0, [r6, #344]
	bl	FreeUsdByDec
	ldr	r1, [r7, #216]
	ldr	r0, [r6, #344]
	bl	FreeUsdByDec
	ldr	r1, [r7, #220]
	ldr	r0, [r6, #344]
	bl	FreeUsdByDec
	ldr	r1, [r7, #224]
	ldr	r0, [r6, #344]
	bl	FreeUsdByDec
	str	r8, [r7, #212]
	str	r8, [r7, #216]
	str	r8, [r7, #220]
	str	r8, [r7, #224]
	str	r5, [r4, #2784]
	str	r5, [r4, #2796]
	str	r5, [r4, #2792]
	str	r5, [r4, #2788]
	ldr	r0, [r6, #344]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #2716]
.L626:
	mov	r3, #0
	str	r3, [r4, #3476]
	mov	r4, r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L671:
	ldr	r2, [r4, #3476]
	ldrb	r1, [r4, #2901]	@ zero_extendqisi2
	cmp	r2, #1
	addle	r2, r2, #1
	adds	r3, r3, #0
	str	r2, [r4, #3476]
	movne	r3, #1
	cmp	r1, #1
	orreq	r3, r3, #1
	cmp	r3, #0
	beq	.L632
	ldr	r3, [r4, #2784]
	mov	r2, #1
	ldr	r0, [r6, #344]
	ldr	r1, [r3, #4]
	bl	FSP_SetRef
	add	r3, r5, #14976
	ldr	r2, [r4, #2796]
	add	r3, r3, #24
	cmp	r2, r3
	beq	.L633
	ldr	r1, [r2, #4]
	mov	r2, #0
	ldr	r0, [r6, #344]
	bl	FSP_SetRef
.L633:
	ldr	r1, [r4, #2792]
	ldr	r2, [r4, #2788]
	ldr	r3, [r4, #2784]
	str	r1, [r4, #2796]
	str	r2, [r4, #2792]
	str	r3, [r4, #2788]
.L632:
	ldr	r3, [r5]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	ldreq	r9, [r4, #2792]
	beq	.L635
	b	.L630
.L670:
	ldr	r3, [r7, #68]
	movw	r2, #3274
	ldr	r1, .L675+12
	mov	r4, ip
	blx	r3
	b	.L623
.L672:
	ldr	r3, [r7, #68]
	movw	r2, #3314
	ldr	r1, .L675+12
	mov	r4, r8
	blx	r3
	b	.L623
.L673:
	ldr	ip, [r7, #68]
	movw	r2, #3249
	ldr	r1, .L675+12
	mov	r4, r3
	blx	ip
	b	.L623
.L674:
	ldr	ip, [r7, #68]
	movw	r2, #3253
	ldr	r1, .L675+12
	mov	r4, r3
	blx	ip
	b	.L623
.L676:
	.align	2
.L675:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC107
	.word	.LC106
	.word	.LC97
	UNWIND(.fnend)
	.size	AVSDEC_VDMPostProc, .-AVSDEC_VDMPostProc
	.align	2
	.global	ClearPacket
	.type	ClearPacket, %function
ClearPacket:
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
	add	r4, r0, #16384
	ldr	r1, [r4, #200]
	ldr	r0, [r4, #344]
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r4, #189]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L678
	ldrb	r3, [r4, #269]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L682
.L678:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L682:
	ldr	r3, [r4, #272]
	cmp	r3, #0
	beq	.L678
	ldr	r1, [r4, #280]
	ldr	r0, [r4, #344]
	bl	SM_ReleaseStreamSeg
	b	.L678
	UNWIND(.fnend)
	.size	ClearPacket, .-ClearPacket
	.align	2
	.global	WriteAvsSliceMsg
	.type	WriteAvsSliceMsg, %function
WriteAvsSliceMsg:
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
	add	r5, r0, #12288
	mov	r4, r0
	ldr	r2, [r5, #2264]
	cmp	r2, #512
	bge	.L691
	cmp	r2, #0
	ble	.L686
	add	lr, r0, #16384
	sub	r3, r2, #1
	ldr	r1, [r5, #3568]
	ldr	ip, [lr, #184]
	mov	r0, r3, asl #5
	add	r1, r1, #15
	sub	r3, r0, r3, asl #2
	add	r3, r4, r3
	ldrb	r0, [ip, #3]	@ zero_extendqisi2
	mov	r1, r1, lsr #4
	ldr	r3, [r3, #232]
	mul	r0, r0, r1
	cmp	r0, r3
	bls	.L692
.L687:
	mov	r0, r2, asl #5
	ldr	r3, [lr, #192]
	sub	r0, r0, r2, asl #2
	add	r0, r4, r0
	str	r3, [r0, #216]
	ldr	r3, [lr, #200]
	str	r3, [r0, #236]
	ldr	r3, [lr, #196]
	str	r3, [r0, #224]
	ldrb	r3, [ip, #3]	@ zero_extendqisi2
	mul	r1, r1, r3
	str	r1, [r0, #232]
	ldr	r3, [r5, #2848]
	ldr	r4, [r5, #2852]
	add	ip, r3, #15
	add	r4, r4, #15
	mov	r3, ip, lsr #4
	mov	r4, r4, lsr #4
	mul	r3, r3, r4
	sub	r3, r3, #1
	cmp	r1, r3
	bhi	.L690
	ldrb	r3, [lr, #189]	@ zero_extendqisi2
	add	r2, r2, #1
	cmp	r3, #1
	ldrne	r3, [lr, #272]
	moveq	r3, #0
	streq	r3, [r0, #220]
	streq	r3, [r0, #240]
	strne	r3, [r0, #220]
	ldrne	r3, [lr, #280]
	strne	r3, [r0, #240]
	ldrne	r3, [lr, #276]
	str	r3, [r0, #228]
	mov	r0, #1
	str	r2, [r5, #2264]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L690:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L686:
	ldr	r1, [r5, #3568]
	add	lr, r0, #16384
	add	r1, r1, #15
	ldr	ip, [lr, #184]
	mov	r1, r1, lsr #4
	b	.L687
.L691:
	ldr	ip, .L693
	mov	r3, #512
	ldr	r1, .L693+4
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L692:
	ldr	ip, .L693
	mov	r2, r0
	ldr	r1, .L693+8
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L694:
	.align	2
.L693:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC108
	.word	.LC109
	UNWIND(.fnend)
	.size	WriteAvsSliceMsg, .-WriteAvsSliceMsg
	.align	2
	.global	AVS_GetPacketState
	.type	AVS_GetPacketState, %function
AVS_GetPacketState:
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
	add	r0, r0, #16384
	ldr	r0, [r0, #176]
	subs	r0, r0, #1
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	AVS_GetPacketState, .-AVS_GetPacketState
	.align	2
	.global	Avs_Vfmw_WQMatrix
	.type	Avs_Vfmw_WQMatrix, %function
Avs_Vfmw_WQMatrix:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 304
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #308)
	sub	sp, sp, #308
	ldr	r8, [r0, #140]
	mov	r3, #0
	ldr	lr, [r0, #124]
	cmp	r8, #1
	str	r3, [fp, #-348]
	str	r3, [fp, #-344]
	str	r3, [fp, #-340]
	str	r3, [fp, #-336]
	str	r3, [fp, #-332]
	str	r3, [fp, #-328]
	beq	.L699
	clz	r3, lr
	cmp	r8, #3
	cmpeq	lr, #0
	mov	r3, r3, lsr #5
	beq	.L699
	cmp	r8, #2
	addeq	r3, r0, #168
	beq	.L701
	cmp	r3, #0
	bne	.L733
.L734:
	sub	r4, fp, #324
	cmp	r8, #0
	mov	r3, #128
	strh	r3, [r4]	@ movhi
	strh	r3, [r4, #2]	@ movhi
	strh	r3, [r4, #4]	@ movhi
	strh	r3, [r4, #6]	@ movhi
	strh	r3, [r4, #8]	@ movhi
	strh	r3, [r4, #10]	@ movhi
	strh	r3, [r4, #12]	@ movhi
	strh	r3, [r4, #14]	@ movhi
	strh	r3, [r4, #16]	@ movhi
	strh	r3, [r4, #18]	@ movhi
	strh	r3, [r4, #20]	@ movhi
	strh	r3, [r4, #22]	@ movhi
	bne	.L783
	ldr	r3, .L789
	add	r2, r4, #10
	add	ip, r3, #12
.L715:
	ldrh	r1, [r3, #2]!
	cmp	r3, ip
	strh	r1, [r2, #2]!	@ movhi
	bne	.L715
.L708:
	add	ip, r0, #194
	add	lr, r0, #320
	add	lr, lr, #2
	mov	r2, #128
	mov	r3, ip
.L719:
	strh	r2, [r3, #2]!	@ movhi
	cmp	r3, lr
	bne	.L719
	ldr	r3, [r0, #120]
	cmp	r3, #0
	beq	.L736
	ldr	r10, [r0, #144]
	mov	r9, #0
	ldr	r3, .L789+4
	mov	r10, r10, asl #6
	add	r7, r10, #64
	add	r10, r3, r10
	add	r7, r3, r7
.L737:
	sub	r3, fp, #300
	mov	r5, r9, asl #3
	add	r6, r3, r9, lsl #7
	sub	r5, r5, r9, asl #1
	sub	r6, r6, #2
	mov	r2, r10
.L727:
	add	r0, r2, #8
	mov	r1, r6
.L725:
	ldrb	r3, [r2], #1	@ zero_extendqisi2
	add	r3, r5, r3
	cmp	r0, r2
	mov	r3, r3, asl #1
	ldrh	r3, [r4, r3]
	strh	r3, [r1, #2]!	@ movhi
	bne	.L725
	cmp	r0, r7
	add	r6, r6, #16
	mov	r2, r0
	bne	.L727
	add	r9, r9, #1
	cmp	r9, #2
	bne	.L737
.L723:
	cmp	r8, #0
	subeq	r3, fp, #174
	subeq	r1, fp, #46
	beq	.L728
	cmp	r8, #1
	beq	.L784
	cmp	r8, #2
	beq	.L785
.L696:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L699:
	ldr	r4, .L789+8
	add	r1, r0, #144
	sub	ip, fp, #348
	add	r3, r0, #168
.L698:
	ldr	r5, [r1, #4]!
	ldrh	r2, [r4], #2
	cmp	r1, r3
	add	r2, r2, r5
	strh	r2, [ip], #2	@ movhi
	bne	.L698
	cmp	r8, #2
	beq	.L701
	cmp	lr, #0
	cmpeq	r8, #3
	bne	.L702
.L701:
	ldr	ip, .L789
	sub	r1, fp, #336
	add	r5, r0, #192
.L703:
	ldr	r4, [r3, #4]!
	ldrh	r2, [ip, #2]!
	cmp	r3, r5
	add	r2, r2, r4
	strh	r2, [r1], #2	@ movhi
	bne	.L703
.L702:
	cmp	lr, #0
	bne	.L734
.L733:
	ldr	r3, .L789+8
	sub	r4, fp, #324
	add	ip, r3, #12
	mov	r5, r4
	mov	r2, r4
.L706:
	ldrh	r1, [r3], #2
	cmp	r3, ip
	strh	r1, [r2], #2	@ movhi
	bne	.L706
	ldr	r3, .L789
	add	lr, r4, #10
	add	ip, r3, #12
	mov	r2, lr
.L707:
	ldrh	r1, [r3, #2]!
	cmp	r3, ip
	strh	r1, [r2, #2]!	@ movhi
	bne	.L707
	cmp	r8, #0
	beq	.L708
	cmp	r8, #1
	beq	.L786
	cmp	r8, #2
	beq	.L787
	cmp	r8, #3
	bne	.L708
	sub	r3, fp, #348
	sub	r1, fp, #336
.L713:
	ldrh	r2, [r3], #2
	cmp	r3, r1
	strh	r2, [r5], #2	@ movhi
	bne	.L713
	add	r1, r4, #22
.L714:
	ldrh	r2, [r3], #2
	strh	r2, [lr, #2]!	@ movhi
	cmp	lr, r1
	bne	.L714
	b	.L708
.L728:
	ldrh	r2, [r3, #2]!
	cmp	r3, r1
	strh	r2, [ip, #2]!	@ movhi
	beq	.L696
	ldrh	r2, [r3, #2]!
	cmp	r3, r1
	strh	r2, [ip, #2]!	@ movhi
	bne	.L728
	b	.L696
.L736:
	sub	r0, fp, #174
	add	r4, fp, #82
	mov	r1, #128
.L738:
	sub	r3, r0, #128
.L724:
	add	r2, r3, #16
.L721:
	strh	r1, [r3, #2]!	@ movhi
	cmp	r2, r3
	bne	.L721
	cmp	r2, r0
	mov	r3, r2
	bne	.L724
	add	r0, r0, #128
	cmp	r0, r4
	bne	.L738
	b	.L723
.L783:
	cmp	r8, #1
	beq	.L788
	cmp	r8, #2
	bne	.L708
	sub	r2, fp, #336
	add	r3, r4, #10
	add	ip, r4, #22
.L718:
	ldrh	r1, [r2], #2
	strh	r1, [r3, #2]!	@ movhi
	cmp	r3, ip
	bne	.L718
	b	.L708
.L788:
	sub	r3, fp, #348
	sub	ip, fp, #336
	mov	r2, r4
.L717:
	ldrh	r1, [r3], #2
	cmp	r3, ip
	strh	r1, [r2], #2	@ movhi
	bne	.L717
	b	.L708
.L786:
	sub	r3, fp, #348
	sub	r1, fp, #336
.L710:
	ldrh	r2, [r3], #2
	cmp	r3, r1
	strh	r2, [r5], #2	@ movhi
	bne	.L710
	b	.L708
.L785:
	sub	r3, fp, #174
	sub	r1, fp, #46
.L732:
	ldrh	r2, [r3, #2]!
	cmp	r3, r1
	strh	r2, [ip, #2]!	@ movhi
	bne	.L732
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L784:
	sub	r3, fp, #300
	sub	r3, r3, #2
.L730:
	ldrh	r2, [r3, #2]!
	strh	r2, [ip, #2]!	@ movhi
	cmp	lr, ip
	bne	.L730
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L787:
	sub	r3, fp, #336
	add	r1, r4, #22
.L712:
	ldrh	r2, [r3], #2
	strh	r2, [lr, #2]!	@ movhi
	cmp	lr, r1
	bne	.L712
	b	.L708
.L790:
	.align	2
.L789:
	.word	.LANCHOR1+10
	.word	.LANCHOR0+16
	.word	.LANCHOR1
	UNWIND(.fnend)
	.size	Avs_Vfmw_WQMatrix, .-Avs_Vfmw_WQMatrix
	.align	2
	.global	AVS_ParseHeader
	.type	AVS_ParseHeader, %function
AVS_ParseHeader:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r7, r0, #12288
	add	r8, r0, #16384
	mov	r3, #0
	add	r9, r0, #14976
	ldr	r6, [r7, #3200]
	add	r5, r0, #15808
	str	r3, [r8, #176]
	mov	r4, r0
	mov	r1, #32
	add	r9, r9, #20
	mov	r0, r6
	add	r5, r5, #28
	bl	BsShow
	uxtb	r0, r0
	sub	r3, r0, #176
	cmp	r3, #78
	ldrls	pc, [pc, r3, asl #2]
	b	.L792
.L794:
	.word	.L793
	.word	.L795
	.word	.L796
	.word	.L797
	.word	.L816
	.word	.L799
	.word	.L800
	.word	.L795
	.word	.L816
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L792
	.word	.L802
.L795:
	ldr	r3, [r7, #2812]
	ands	r5, r3, #32768
	bne	.L832
	tst	r3, #327680
	bne	.L840
.L835:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L792:
	cmp	r0, #175
	bhi	.L816
	ldr	r3, [r7, #2812]
	and	r2, r3, #327680
	cmp	r2, #327680
	beq	.L841
.L816:
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L799:
	ldr	r3, [r7, #2812]
	tst	r3, #327680
	beq	.L811
	mov	r1, r5
	mov	r0, r9
	bl	AVS_extension
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L796:
	ldr	r3, [r7, #2812]
	ands	r1, r3, #262144
	movne	r1, #1
	bne	.L834
	tst	r3, #65536
	beq	.L811
.L834:
	mov	r0, r4
	bl	AVS_user_data
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L797:
	ldr	r3, [r7, #2812]
	ands	r6, r3, #32768
	bne	.L833
	tst	r3, #262144
	beq	.L807
	bic	r3, r3, #311296
	mov	r0, r9
	str	r3, [r7, #2812]
	mov	r1, r5
	bl	AVS_i_picture_header
	b	.L837
.L800:
	ldr	r3, [r7, #2812]
	ands	r6, r3, #32768
	bne	.L833
	tst	r3, #262144
	beq	.L809
	bic	r3, r3, #311296
	mov	r0, r9
	str	r3, [r7, #2812]
	mov	r1, r5
	bl	AVS_pb_picture_header
.L837:
	mov	r0, r5
	bl	Avs_Vfmw_WQMatrix
	mov	r0, r4
	bl	ClearPacket
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L802:
	mov	r1, #32
	mov	r0, r6
	bl	BsSkip
	mov	r1, #32
	mov	r0, r6
	bl	BsGet
	mov	r1, #32
	mov	r5, r0
	mov	r0, r6
	bl	BsGet
	movw	r2, #20036
	movw	r3, #20553
	movt	r2, 17221
	movt	r3, 18515
	cmp	r0, r2
	cmpeq	r5, r3
	bne	.L811
	mov	r5, #1
	strb	r5, [r8, #860]
	str	r5, [r8, #176]
	mov	r0, r4
	ldr	r3, [r7, #2812]
	orr	r3, r3, #16384
	str	r3, [r7, #2812]
	bl	ClearPacket
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L793:
	ldr	r3, [r7, #2812]
	ands	r6, r3, #32768
	bne	.L832
	tst	r3, #65536
	beq	.L805
	bic	r3, r3, #311296
	mov	r1, r5
	str	r3, [r7, #2812]
	mov	r0, r4
	bl	AVS_sequence_header
	mov	r0, r4
	bl	ClearPacket
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L832:
	mov	r0, #1
	orr	r3, r3, #16384
	str	r0, [r8, #176]
	str	r3, [r7, #2812]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L841:
	orr	r3, r3, #32768
	mov	r0, r4
	str	r3, [r7, #2812]
	bl	WriteAvsSliceMsg
	cmp	r0, #0
	bne	.L835
.L811:
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L809:
	mov	r0, r9
	mov	r1, r5
	bl	AVS_pb_picture_header
.L836:
	mov	r0, r5
	bl	Avs_Vfmw_WQMatrix
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L807:
	mov	r0, r9
	mov	r1, r5
	bl	AVS_i_picture_header
	b	.L836
.L805:
	mov	r1, r5
	mov	r0, r4
	bl	AVS_sequence_header
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L833:
	orr	r3, r3, #16384
	mov	r0, r5
	mov	r4, #1
	str	r4, [r8, #176]
	str	r3, [r7, #2812]
	bl	Avs_Vfmw_WQMatrix
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L840:
	bic	r3, r3, #311296
	mov	r0, r4
	str	r3, [r7, #2812]
	bl	ClearPacket
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	AVS_ParseHeader, .-AVS_ParseHeader
	.align	2
	.global	AVSDEC_Decode
	.type	AVSDEC_Decode, %function
AVSDEC_Decode:
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
	ldr	r7, .L857
	mov	r4, r0
	add	r0, r0, #15744
	mov	r5, r1
	ldr	r2, [r1, #12]
	add	r0, r0, #60
	ldr	r1, [r1]
	add	r6, r4, #12288
	bl	BsInit
	add	r0, r4, #16384
	mov	r1, r5
	ldr	r3, [r7, #52]
	mov	r2, #160
	add	r0, r0, #184
	blx	r3
	ldr	r3, [r6, #2812]
	mov	r0, r4
	str	r3, [r6, #3512]
	bl	AVS_ParseHeader
	subs	r5, r0, #0
	beq	.L855
	ldr	r3, [r6, #2812]
	and	r3, r3, #376832
	cmp	r3, #376832
	beq	.L845
.L846:
	mov	r5, #0
.L844:
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L855:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L857+4
	blx	r3
	mov	r0, r4
	bl	AVS_ClearCurPic
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L845:
	mov	r0, r4
	bl	AVS_DecodeArbiter.part.3
	cmp	r0, #0
	beq	.L846
	mov	r0, r4
	bl	AVS_GetStreamInf
	cmp	r0, #3
	beq	.L847
	adds	r5, r0, #0
	movne	r5, #1
	cmp	r5, #0
	beq	.L856
	mov	r0, r4
	bl	AVS_StartDecode
	subs	r3, r0, #0
	bne	.L844
	ldr	r2, [r7, #68]
	mov	r0, #1
	ldr	r1, .L857+8
	mov	r5, r3
	blx	r2
	mov	r0, r4
	bl	AVS_ClearCurPic
	b	.L844
.L856:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L857+12
	blx	r3
	mov	r0, r4
	bl	AVS_ClearCurPic
	b	.L844
.L847:
	ldr	r3, [r6, #3512]
	mov	r5, r0
	str	r3, [r6, #2812]
	b	.L844
.L858:
	.align	2
.L857:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC110
	.word	.LC112
	.word	.LC111
	UNWIND(.fnend)
	.size	AVSDEC_Decode, .-AVSDEC_Decode
	.global	g_tmp
	.global	wq_param_default
	.global	g_AvsCfg
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	CSWTCH.51, %object
	.size	CSWTCH.51, 16
CSWTCH.51:
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
	.align	2
.LANCHOR1 = . + 0
	.type	wq_param_default, %object
	.size	wq_param_default, 24
wq_param_default:
	.short	135
	.short	143
	.short	143
	.short	160
	.short	160
	.short	213
	.short	128
	.short	98
	.short	106
	.short	116
	.short	116
	.short	128
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"%-40s     %#-8x(%db)\012\000" )
	.space	2
.LC1:
	ASCII(.ascii	"video_sequence_start_code\000" )
	.space	2
.LC2:
	ASCII(.ascii	"profile_id\000" )
	.space	1
.LC3:
	ASCII(.ascii	"level_id\000" )
	.space	3
.LC4:
	ASCII(.ascii	"progressive_sequence\000" )
	.space	3
.LC5:
	ASCII(.ascii	"horizontal_size\000" )
.LC6:
	ASCII(.ascii	"vertical_size\000" )
	.space	2
.LC7:
	ASCII(.ascii	"chroma_format\000" )
	.space	2
.LC8:
	ASCII(.ascii	"sample_precision\000" )
	.space	3
.LC9:
	ASCII(.ascii	"aspect_ratio\000" )
	.space	3
.LC10:
	ASCII(.ascii	"frame_rate_code\000" )
.LC11:
	ASCII(.ascii	"bit_rate_lower\000" )
	.space	1
.LC12:
	ASCII(.ascii	"marker_bit\000" )
	.space	1
.LC13:
	ASCII(.ascii	"bit_rate_upper\000" )
	.space	1
.LC14:
	ASCII(.ascii	"low_delay\000" )
	.space	2
.LC15:
	ASCII(.ascii	"bbv_buffer_size\000" )
.LC16:
	ASCII(.ascii	"reserved_bits\000" )
	.space	2
.LC17:
	ASCII(.ascii	"i_picture_start_code\000" )
	.space	3
.LC18:
	ASCII(.ascii	"bbv_delay\000" )
	.space	2
.LC19:
	ASCII(.ascii	"bbv_delay_extension\000" )
.LC20:
	ASCII(.ascii	"time_code_flag\000" )
	.space	1
.LC21:
	ASCII(.ascii	"time_code\000" )
	.space	2
.LC22:
	ASCII(.ascii	"picture_distance\000" )
	.space	3
.LC23:
	ASCII(.ascii	"bbv_check_times\000" )
.LC24:
	ASCII(.ascii	"progressive_frame\000" )
	.space	2
.LC25:
	ASCII(.ascii	"picture_structure\000" )
	.space	2
.LC26:
	ASCII(.ascii	"top_field_first\000" )
.LC27:
	ASCII(.ascii	"repeat_first_field\000" )
	.space	1
.LC28:
	ASCII(.ascii	"fixed_picture_qp\000" )
	.space	3
.LC29:
	ASCII(.ascii	"picture_qp\000" )
	.space	1
.LC30:
	ASCII(.ascii	"skip_mode_flag\000" )
	.space	1
.LC31:
	ASCII(.ascii	"loop_filter_disable\000" )
.LC32:
	ASCII(.ascii	"loop_filter_parameter_flag\000" )
	.space	1
.LC33:
	ASCII(.ascii	"alpha_c_offset\000" )
	.space	1
.LC34:
	ASCII(.ascii	"beta_offset\000" )
.LC35:
	ASCII(.ascii	"weighting_quant_flag\000" )
	.space	3
.LC36:
	ASCII(.ascii	"aec_enable\000" )
	.space	1
.LC37:
	ASCII(.ascii	"mb_adapt_weighting_quant_disable\000" )
	.space	3
.LC38:
	ASCII(.ascii	"chroma_quant_param_disable\000" )
	.space	1
.LC39:
	ASCII(.ascii	"chroma_quant_param_delta_u\000" )
	.space	1
.LC40:
	ASCII(.ascii	"chroma_quant_param_delta_v\000" )
	.space	1
.LC41:
	ASCII(.ascii	"weighting_quant_param_index\000" )
.LC42:
	ASCII(.ascii	"weighting_quant_model\000" )
	.space	2
.LC43:
	ASCII(.ascii	"weighting_quant_param_deltal1\000" )
	.space	2
.LC44:
	ASCII(.ascii	"weighting_quant_param_deltal2\000" )
	.space	2
.LC45:
	ASCII(.ascii	"pb_picture_start_code\000" )
	.space	2
.LC46:
	ASCII(.ascii	"picture_coding_type\000" )
.LC47:
	ASCII(.ascii	"advanced_pred_mode_disable\000" )
	.space	1
.LC48:
	ASCII(.ascii	"picture_reference_flag\000" )
	.space	1
.LC49:
	ASCII(.ascii	"no_forward_reference_flag\000" )
	.space	2
.LC50:
	ASCII(.ascii	"b_filed_enhanced_flag\000" )
	.space	2
.LC51:
	ASCII(.ascii	"p_filed_enhanced_flag\000" )
	.space	2
.LC52:
	ASCII(.ascii	"user_data_start_code\000" )
	.space	3
.LC53:
	ASCII(.ascii	"extension_ID\000" )
	.space	3
.LC54:
	ASCII(.ascii	"video_format\000" )
	.space	3
.LC55:
	ASCII(.ascii	"sample_range\000" )
	.space	3
.LC56:
	ASCII(.ascii	"colour_description\000" )
	.space	1
.LC57:
	ASCII(.ascii	"colour_primaries\000" )
	.space	3
.LC58:
	ASCII(.ascii	"transfer_characteristics\000" )
	.space	3
.LC59:
	ASCII(.ascii	"matrix_coefficients\000" )
.LC60:
	ASCII(.ascii	"display_horizontal_size\000" )
.LC61:
	ASCII(.ascii	"display_vertical_size\000" )
	.space	2
.LC62:
	ASCII(.ascii	"copyright_flag\000" )
	.space	1
.LC63:
	ASCII(.ascii	"copyright_id\000" )
	.space	3
.LC64:
	ASCII(.ascii	"original_or_copy\000" )
	.space	3
.LC65:
	ASCII(.ascii	"copyright_number_1\000" )
	.space	1
.LC66:
	ASCII(.ascii	"copyright_number_2\000" )
	.space	1
.LC67:
	ASCII(.ascii	"copyright_number_3\000" )
	.space	1
.LC68:
	ASCII(.ascii	"camera_id\000" )
	.space	2
.LC69:
	ASCII(.ascii	"height_of_image_device\000" )
	.space	1
.LC70:
	ASCII(.ascii	"focal_length\000" )
	.space	3
.LC71:
	ASCII(.ascii	"f_number\000" )
	.space	3
.LC72:
	ASCII(.ascii	"vertical_angle_of_view\000" )
	.space	1
.LC73:
	ASCII(.ascii	"camera_position_x_upper\000" )
.LC74:
	ASCII(.ascii	"camera_position_x_lower\000" )
.LC75:
	ASCII(.ascii	"camera_position_y_upper\000" )
.LC76:
	ASCII(.ascii	"camera_position_y_lower\000" )
.LC77:
	ASCII(.ascii	"camera_position_z_upper\000" )
.LC78:
	ASCII(.ascii	"camera_position_z_lower\000" )
.LC79:
	ASCII(.ascii	"camera_direction_x\000" )
	.space	1
.LC80:
	ASCII(.ascii	"camera_direction_y\000" )
	.space	1
.LC81:
	ASCII(.ascii	"camera_direction_z\000" )
	.space	1
.LC82:
	ASCII(.ascii	"image_plane_vertical_x\000" )
	.space	1
.LC83:
	ASCII(.ascii	"image_plane_vertical_y\000" )
	.space	1
.LC84:
	ASCII(.ascii	"image_plane_vertical_z\000" )
	.space	1
.LC85:
	ASCII(.ascii	"frame_centre_horizontal_offset\000" )
	.space	1
.LC86:
	ASCII(.ascii	"frame_centre_vertical_offset\000" )
	.space	3
.LC87:
	ASCII(.ascii	"extension_start_code\000" )
	.space	3
.LC88:
	ASCII(.ascii	"ERROR: image size abnormal(%dx%d)\012\000" )
	.space	1
.LC89:
	ASCII(.ascii	"avs actual frame size(%dx%d) exeed max config(%dx%d" )
	ASCII(.ascii	")\012\000" )
	.space	2
.LC90:
	ASCII(.ascii	"ERROR: partition fs memory fail!\012\000" )
	.space	2
.LC91:
	ASCII(.ascii	"ERROR: FSP_ConfigInstance fail!\012\000" )
	.space	3
.LC92:
	ASCII(.ascii	"line: %d, pImage is NULL!\012\000" )
	.space	1
.LC93:
	ASCII(.ascii	"line: %d pImage is null\012\000" )
	.space	3
.LC94:
	ASCII(.ascii	"ERROR: get frame store fail!\012\000" )
	.space	2
.LC95:
	ASCII(.ascii	"get image buffer ok: LogicFsID = %d\012\000" )
	.space	3
.LC96:
	ASCII(.ascii	"decode %p, disp %p, tf %p\012\000" )
	.space	1
.LC97:
	ASCII(.ascii	"line: %d\012\000" )
	.space	2
.LC98:
	ASCII(.ascii	"ERROR: AVS_OutputOneFrame is err(%d)\012\000" )
	.space	2
.LC99:
	ASCII(.ascii	"size change\012\000" )
	.space	3
.LC100:
	ASCII(.ascii	"MbWidth = %d, MbHeight = %d, don't support!\012\000" )
	.space	3
.LC101:
	ASCII(.ascii	"ERROR: allocate vhb\012\000" )
	.space	3
.LC102:
	ASCII(.ascii	"ERROR: get img buf err\012\000" )
.LC103:
	ASCII(.ascii	"dec_pts: %lld Usertag = %lld\012\000" )
	.space	2
.LC104:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC105:
	ASCII(.ascii	"-----Pic %5d, type %d, struct %d-----\012\000" )
	.space	1
.LC106:
	ASCII(.ascii	"ERROR: err_level(%d) over ref_thr(%d)\012\000" )
	.space	1
.LC107:
	ASCII(.ascii	"ERROR: err_level(%d) over out_thr(%d)\012\000" )
	.space	1
.LC108:
	ASCII(.ascii	"SlcNum %d > %d Write slice msg failed\012\000" )
	.space	1
.LC109:
	ASCII(.ascii	"This slice's start mbn %d <= LastSliceFirstMb %d, W" )
	ASCII(.ascii	"rite slice msg failed\012\000" )
	.space	2
.LC110:
	ASCII(.ascii	"ERROR: paseHeader err\012\000" )
	.space	1
.LC111:
	ASCII(.ascii	"ERROR: AVS_PrepareDecode err\012\000" )
	.space	2
.LC112:
	ASCII(.ascii	"ERROR: AVS_StartDecode err\012\000" )
	.bss
	.align	2
	.type	g_tmp, %object
	.size	g_tmp, 1024
g_tmp:
	.space	1024
	.type	g_AvsCfg, %object
	.size	g_AvsCfg, 508
g_AvsCfg:
	.space	508
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
