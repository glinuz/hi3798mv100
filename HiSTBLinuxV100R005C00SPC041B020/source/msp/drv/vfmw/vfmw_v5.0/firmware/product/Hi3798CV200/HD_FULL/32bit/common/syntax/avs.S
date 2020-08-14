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
	mov	r6, r1
	mov	r5, r2
	bl	BsGet
	str	r6, [sp]
	mov	r2, r5
	ldr	r1, .L2
	mov	r4, r0
	mov	r3, r0
	mov	r0, #7
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3:
	.align	2
.L2:
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
	mov	r6, r1
	mov	r5, r2
	bl	BsGet
	str	r6, [sp]
	mov	r2, r5
	ldr	r1, .L5
	mov	r4, r0
	mov	r3, r0
	mov	r0, #7
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L6:
	.align	2
.L5:
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
	str	r4, [sp]
	mov	r2, r6
	mov	r3, r5
	ldr	r1, .L12
	mov	r0, #7
	bl	dprint_vfmw
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
	str	r4, [sp]
	mov	r2, r6
	mov	r3, r5
	ldr	r1, .L20
	mov	r0, #7
	bl	dprint_vfmw
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
	mov	r5, r0, asr #1
	mov	r0, r7
	addne	r5, r5, #1
	rsbeq	r5, r5, #0
	bl	BsSkip
	b	.L15
.L21:
	.align	2
.L20:
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
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r6, r0, #12288
	mov	r4, r1
	ldr	r2, .L40
	ldr	r5, [r6, #3120]
	mov	r1, #32
	ldr	r3, [r6, #2732]
	mov	r7, r0
	bic	r3, r3, #65536
	mov	r0, r5
	str	r3, [r6, #2732]
	bl	avs_f_v
	ldr	r2, .L40+4
	mov	r1, #8
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+8
	mov	r1, #8
	str	r0, [r4, #16]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+12
	mov	r1, #1
	str	r0, [r4, #20]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+16
	mov	r1, #14
	strb	r0, [r4, #15]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+20
	mov	r1, #14
	str	r0, [r4, #24]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+24
	mov	r1, #2
	str	r0, [r4, #28]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+28
	mov	r1, #3
	strb	r0, [r4, #11]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+32
	mov	r1, #4
	strb	r0, [r4, #12]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+36
	mov	r1, #4
	strb	r0, [r4, #13]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+40
	mov	r1, #18
	strb	r0, [r4, #14]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+44
	mov	r1, #1
	str	r0, [r4, #32]
	mov	r0, r5
	bl	avs_f_v
	ldr	r2, .L40+48
	mov	r1, #12
	strb	r0, [r4, #10]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+52
	mov	r1, #1
	str	r0, [r4, #36]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+44
	mov	r1, #1
	strb	r0, [r4, #9]
	mov	r0, r5
	bl	avs_f_v
	ldr	r2, .L40+56
	mov	r1, #18
	strb	r0, [r4, #10]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L40+60
	mov	r1, #3
	str	r0, [r4, #40]
	mov	r0, r5
	bl	avs_u_v
	ldr	r3, [r4, #24]
	strb	r0, [r4, #8]
	ldr	r0, [r6, #2772]
	cmp	r3, r0
	beq	.L39
.L33:
	ldr	r3, .L40+64
	mov	r2, #1
	str	r2, [r6, #2744]
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L34
	ldr	r8, [r6, #2776]
	mov	r3, #8
	ldr	r5, [r4, #24]
	sub	r2, fp, #44
	ldr	lr, [r4, #28]
	mov	r1, #2
	strh	r0, [fp, #-44]	@ movhi
	strh	r8, [fp, #-42]	@ movhi
	ldr	r0, [r7, #4]
	strh	r5, [fp, #-40]	@ movhi
	strh	lr, [fp, #-38]	@ movhi
	blx	ip
.L34:
	add	r0, r6, #2736
	mov	r1, r4
	mov	r2, #372
	add	r0, r0, #12
	bl	memcpy
	ldr	r2, [r6, #2740]
	ldr	r3, [r6, #2732]
	mov	r0, #1
	add	r2, r2, r0
	str	r0, [r6, #2736]
	orr	r3, r3, #65536
	str	r2, [r6, #2740]
	str	r3, [r6, #2732]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L39:
	ldr	r2, [r4, #28]
	ldr	r3, [r6, #2776]
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
	add	r0, r1, #76
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
	ldr	r3, [r4, #16]
	cmp	r3, #72
	str	r0, [r4, #92]
	beq	.L73
.L43:
	ldr	r2, .L81+12
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	uxtb	r0, r0
	strb	r0, [r4, #86]
	cmp	r0, #1
	beq	.L74
.L44:
	ldr	r2, .L81+16
	mov	r1, #1
	mov	r0, r5
	bl	avs_f_v
	ldr	r2, .L81+20
	mov	r1, #8
	str	r0, [r4, #104]
	mov	r0, r5
	bl	avs_u_v
	ldrb	r3, [r4, #9]	@ zero_extendqisi2
	cmp	r3, #1
	str	r0, [r4, #108]
	beq	.L75
.L45:
	ldr	r2, .L81+24
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	ldrb	r3, [r4, #15]	@ zero_extendqisi2
	cmp	r3, #1
	uxtb	r0, r0
	strb	r0, [r4, #76]
	streqb	r3, [r4, #76]
	beq	.L47
	cmp	r0, #0
	beq	.L76
.L47:
	mov	r3, #1
	strb	r3, [r4, #77]
.L48:
	ldr	r2, .L81+28
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L81+32
	mov	r1, #1
	strb	r0, [r4, #78]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L81+36
	mov	r1, #1
	strb	r0, [r4, #79]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L81+40
	mov	r1, #6
	strb	r0, [r4, #80]
	mov	r0, r5
	bl	avs_u_v
	ldrh	r3, [r4, #76]
	cmp	r3, #0
	strb	r0, [r4, #81]
	beq	.L77
.L49:
	ldr	r2, .L81+44
	mov	r1, #4
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L81+48
	mov	r1, #1
	strb	r0, [r4, #83]
	mov	r0, r5
	bl	avs_u_v
	mov	r3, #0
	str	r3, [r4, #116]
	str	r3, [r4, #120]
	uxtb	r0, r0
	strb	r0, [r4, #84]
	cmp	r0, r3
	beq	.L78
.L51:
	ldr	r3, [r4, #16]
	cmp	r3, #72
	beq	.L79
.L53:
	mov	r0, #1
	mov	r3, #0
	strb	r0, [r4, #89]
	str	r3, [r4, #344]
	ldr	r3, [r6, #104]
	orr	r3, r3, #262144
	str	r3, [r6, #104]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L76:
	ldr	r2, .L81+52
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	strb	r0, [r4, #77]
	b	.L48
.L78:
	ldr	r2, .L81+56
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	uxtb	r0, r0
	strb	r0, [r4, #85]
	cmp	r0, #0
	beq	.L51
	ldr	r1, .L81+60
	mov	r0, r5
	bl	avs_se_v
	ldr	r1, .L81+64
	str	r0, [r4, #116]
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r4, #120]
	b	.L51
.L77:
	ldr	r2, .L81+68
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	strb	r0, [r4, #82]
	b	.L49
.L79:
	ldr	r2, .L81+72
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	cmp	r0, #0
	str	r0, [r4, #124]
	bne	.L54
.L63:
	mov	r0, r5
	ldr	r2, .L81+76
	mov	r1, #1
	bl	avs_u_v
	str	r0, [r4, #328]
	b	.L53
.L73:
	ldr	r2, .L81+16
	mov	r1, #1
	mov	r0, r5
	bl	avs_f_v
	ldr	r2, .L81+80
	mov	r1, #7
	str	r0, [r4, #104]
	mov	r0, r5
	bl	avs_u_v
	ldr	r3, [r4, #92]
	mov	r2, #1
	strb	r2, [r4]
	str	r0, [r4, #96]
	orr	r0, r0, r3, asl #7
	str	r0, [r4, #92]
	b	.L43
.L74:
	ldr	r2, .L81+84
	mov	r1, #24
	mov	r0, r5
	bl	avs_u_v
	str	r0, [r4, #100]
	b	.L44
.L75:
	ldr	r1, .L81+88
	mov	r0, r5
	bl	avs_ue_v
	str	r0, [r4, #112]
	b	.L45
.L54:
	ldr	r2, .L81+92
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L81+96
	mov	r1, #1
	str	r0, [r4, #128]
	mov	r0, r5
	bl	avs_u_v
	cmp	r0, #0
	str	r0, [r4, #132]
	beq	.L80
.L55:
	ldr	r2, .L81+100
	mov	r1, #2
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L81+104
	mov	r1, #2
	str	r0, [r4, #144]
	mov	r0, r5
	bl	avs_u_v
	ldr	r3, [r4, #144]
	cmp	r3, #1
	str	r0, [r4, #148]
	beq	.L58
	ldr	r2, [r4, #128]
	cmp	r2, #0
	cmpeq	r3, #3
	beq	.L58
	cmp	r3, #2
	addeq	r7, r4, #172
	bne	.L63
.L62:
	add	r8, r4, #196
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
	str	r0, [r4, #136]
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r4, #140]
	b	.L55
.L58:
	add	r8, r4, #148
	add	r7, r4, #172
.L57:
	ldr	r1, .L81+120
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r8, #4]!
	cmp	r8, r7
	bne	.L57
	ldr	r3, [r4, #144]
	cmp	r3, #2
	beq	.L62
	ldr	r2, [r4, #128]
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
	add	r0, r1, #76
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
	ldr	r3, [r4, #16]
	cmp	r3, #72
	str	r0, [r4, #92]
	beq	.L127
.L84:
	ldr	r2, .L135+12
	mov	r1, #2
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+16
	mov	r1, #8
	strb	r0, [r4, #87]
	mov	r0, r5
	bl	avs_u_v
	ldrb	r3, [r4, #9]	@ zero_extendqisi2
	cmp	r3, #1
	str	r0, [r4, #108]
	beq	.L128
.L85:
	ldr	r2, .L135+20
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	uxtb	r0, r0
	strb	r0, [r4, #76]
	cmp	r0, #0
	movne	r3, #1
	strneb	r3, [r4, #77]
	beq	.L129
.L87:
	ldr	r2, .L135+24
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+28
	mov	r1, #1
	strb	r0, [r4, #78]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+32
	mov	r1, #1
	strb	r0, [r4, #79]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+36
	mov	r1, #6
	strb	r0, [r4, #80]
	mov	r0, r5
	bl	avs_u_v
	ldrb	r3, [r4, #87]	@ zero_extendqisi2
	cmp	r3, #2
	strb	r0, [r4, #81]
	beq	.L130
.L88:
	ldr	r2, .L135+40
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	strb	r0, [r4, #89]
.L90:
	ldr	r2, .L135+44
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	ldrb	r2, [r4, #87]	@ zero_extendqisi2
	mov	r3, #0
	str	r3, [r4, #336]
	cmp	r2, #2
	str	r3, [r4, #340]
	strb	r0, [r4, #90]
	beq	.L131
	ldr	r2, .L135+48
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	str	r0, [r4, #336]
.L92:
	ldrb	r3, [r4, #77]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L93
	ldr	r3, [r4, #16]
	cmp	r3, #72
	beq	.L94
.L93:
	mov	r3, #0
	str	r3, [r4, #336]
	str	r3, [r4, #340]
.L94:
	ldr	r2, .L135+52
	mov	r1, #2
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+56
	mov	r1, #1
	strb	r0, [r4, #83]
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+60
	mov	r1, #1
	strb	r0, [r4, #82]
	mov	r0, r5
	bl	avs_u_v
	mov	r3, #0
	str	r3, [r4, #116]
	str	r3, [r4, #120]
	uxtb	r0, r0
	strb	r0, [r4, #84]
	cmp	r0, r3
	beq	.L132
.L96:
	ldr	r3, [r4, #16]
	cmp	r3, #72
	beq	.L133
.L98:
	ldrb	r3, [r4, #87]	@ zero_extendqisi2
	mov	r0, #1
	cmp	r3, #1
	movne	r3, #2
	str	r3, [r4, #344]
	ldr	r3, [r6, #104]
	orr	r3, r3, #262144
	str	r3, [r6, #104]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L130:
	ldrb	r3, [r4, #77]	@ zero_extendqisi2
	cmp	r3, #1
	streqb	r3, [r4, #89]
	bne	.L88
	b	.L90
.L129:
	ldr	r2, .L135+64
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	uxtb	r0, r0
	strb	r0, [r4, #77]
	cmp	r0, #0
	bne	.L87
	ldr	r2, .L135+68
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	strb	r0, [r4, #88]
	b	.L87
.L132:
	ldr	r2, .L135+72
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	uxtb	r0, r0
	strb	r0, [r4, #85]
	cmp	r0, #0
	beq	.L96
	ldr	r1, .L135+76
	mov	r0, r5
	bl	avs_se_v
	ldr	r1, .L135+80
	str	r0, [r4, #116]
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r4, #120]
	b	.L96
.L131:
	ldr	r2, .L135+84
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	str	r0, [r4, #340]
	b	.L92
.L133:
	ldr	r2, .L135+88
	mov	r1, #1
	mov	r0, r5
	bl	avs_u_v
	cmp	r0, #1
	mov	r7, r0
	str	r0, [r4, #124]
	beq	.L99
.L108:
	mov	r0, r5
	ldr	r2, .L135+92
	mov	r1, #1
	bl	avs_u_v
	str	r0, [r4, #328]
	b	.L98
.L127:
	ldr	r2, .L135+96
	mov	r1, #1
	mov	r0, r5
	bl	avs_f_v
	ldr	r2, .L135+100
	mov	r1, #7
	str	r0, [r4, #104]
	mov	r0, r5
	bl	avs_u_v
	ldr	r3, [r4, #92]
	mov	r2, #1
	strb	r2, [r4]
	str	r0, [r4, #96]
	orr	r0, r0, r3, asl #7
	str	r0, [r4, #92]
	b	.L84
.L128:
	ldr	r1, .L135+104
	mov	r0, r5
	bl	avs_ue_v
	str	r0, [r4, #112]
	b	.L85
.L99:
	mov	r1, r0
	ldr	r2, .L135+108
	mov	r0, r5
	bl	avs_u_v
	mov	r1, r7
	ldr	r2, .L135+112
	str	r0, [r4, #128]
	mov	r0, r5
	bl	avs_u_v
	cmp	r0, #0
	str	r0, [r4, #132]
	beq	.L134
.L100:
	ldr	r2, .L135+116
	mov	r1, #2
	mov	r0, r5
	bl	avs_u_v
	ldr	r2, .L135+120
	mov	r1, #2
	str	r0, [r4, #144]
	mov	r0, r5
	bl	avs_u_v
	ldr	r3, [r4, #144]
	cmp	r3, #1
	str	r0, [r4, #148]
	beq	.L103
	ldr	r2, [r4, #128]
	cmp	r2, #0
	cmpeq	r3, #3
	beq	.L103
	cmp	r3, #2
	addeq	r7, r4, #172
	bne	.L108
.L107:
	add	r8, r4, #196
.L106:
	ldr	r1, .L135+124
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r7, #4]!
	cmp	r7, r8
	bne	.L106
	b	.L108
.L103:
	add	r8, r4, #148
	add	r7, r4, #172
.L102:
	ldr	r1, .L135+128
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r8, #4]!
	cmp	r8, r7
	bne	.L102
	ldr	r3, [r4, #144]
	cmp	r3, #2
	beq	.L107
	ldr	r2, [r4, #128]
	cmp	r2, #0
	cmpeq	r3, #3
	bne	.L108
	b	.L107
.L134:
	ldr	r1, .L135+132
	mov	r0, r5
	bl	avs_se_v
	ldr	r1, .L135+136
	str	r0, [r4, #136]
	mov	r0, r5
	bl	avs_se_v
	str	r0, [r4, #140]
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
	mov	r6, r1
	mov	r5, r0
	ldr	r2, .L170
	ldr	r0, [r4, #3120]
	mov	r1, #32
	bl	avs_f_v
	ldr	r3, [r4, #2732]
	cmp	r6, #0
	orr	r3, r3, #1048576
	str	r3, [r4, #2732]
	bne	.L138
	ldr	r1, [r4, #3412]
	cmp	r1, #0
	beq	.L139
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	str	r6, [r4, #3412]
.L139:
	ldr	r0, [r5, #4]
	bl	GetUsd
	cmp	r0, #0
	str	r0, [r4, #3412]
	beq	.L149
	ldr	r3, [r5, #12]
	add	r0, r0, #1056
	mov	r1, #9
	add	r6, r5, #16384
	ldr	r7, .L170+4
	ldrd	r2, [r3, #56]
	strd	r2, [r0]
	ldr	r3, [r4, #3412]
	str	r1, [r3, #1040]
	ldr	r3, [r4, #3412]
	ldr	r2, [r4, #2740]
	str	r2, [r3, #1044]
	ldr	r2, [r4, #3412]
	ldr	r3, [r6, #116]
	sub	r3, r3, #4
	str	r3, [r2, #1052]
	ldr	r0, [r4, #3412]
	ldr	r2, [r0, #1052]
	cmp	r2, #1024
	movgt	r3, #1024
	strgt	r3, [r0, #1052]
	ldrgt	r0, [r4, #3412]
	ldr	r1, [r6, #104]
	ldr	r3, [r7, #52]
	ldrgt	r2, [r0, #1052]
	add	r1, r1, #4
	blx	r3
	ldrb	r3, [r6, #109]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L144
	ldr	r2, [r4, #3412]
	ldr	r1, [r6, #196]
	ldr	r3, [r2, #1052]
	add	r3, r3, r1
	str	r3, [r2, #1052]
	ldr	r1, [r4, #3412]
	ldr	r2, [r1, #1052]
	cmp	r2, #1024
	movgt	r3, #1024
	strgt	r3, [r1, #1052]
	ldrgt	r1, [r4, #3412]
	ldr	r0, [r6, #116]
	ldrgt	r2, [r1, #1052]
	rsb	r3, r0, #4
	add	r2, r3, r2
	cmp	r2, #0
	ble	.L144
	sub	r0, r0, #4
	ldr	r3, [r7, #52]
	add	r0, r1, r0
	ldr	r1, [r6, #184]
	blx	r3
.L144:
	ldr	r3, .L170+8
	ldr	ip, [r3]
	cmp	ip, #0
	ldrne	r2, [r4, #3412]
	bne	.L169
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L138:
	ldr	r1, [r4, #3416]
	cmp	r1, #0
	beq	.L148
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	mov	r3, #0
	str	r3, [r4, #3416]
.L148:
	ldr	r0, [r5, #4]
	bl	GetUsd
	cmp	r0, #0
	str	r0, [r4, #3416]
	beq	.L149
	ldr	r3, [r5, #12]
	add	r0, r0, #1056
	mov	r1, #10
	add	r6, r5, #16384
	ldr	r7, .L170+4
	ldrd	r2, [r3, #56]
	strd	r2, [r0]
	ldr	r3, [r4, #3416]
	str	r1, [r3, #1040]
	ldr	r3, [r4, #3416]
	ldr	r2, [r4, #2740]
	str	r2, [r3, #1044]
	ldr	r2, [r4, #3416]
	ldr	r3, [r6, #116]
	sub	r3, r3, #4
	str	r3, [r2, #1052]
	ldr	r0, [r4, #3416]
	ldr	r2, [r0, #1052]
	cmp	r2, #1024
	movgt	r3, #1024
	strgt	r3, [r0, #1052]
	ldrgt	r0, [r4, #3416]
	ldr	r1, [r6, #104]
	ldr	r3, [r7, #52]
	ldrgt	r2, [r0, #1052]
	add	r1, r1, #4
	blx	r3
	ldrb	r3, [r6, #109]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L152
	ldr	r2, [r4, #3416]
	ldr	r1, [r6, #196]
	ldr	r3, [r2, #1052]
	add	r3, r3, r1
	str	r3, [r2, #1052]
	ldr	r1, [r4, #3416]
	ldr	r2, [r1, #1052]
	cmp	r2, #1024
	movgt	r3, #1024
	strgt	r3, [r1, #1052]
	ldrgt	r1, [r4, #3416]
	ldr	r0, [r6, #116]
	ldrgt	r2, [r1, #1052]
	rsb	r3, r0, #4
	add	r2, r3, r2
	cmp	r2, #0
	ble	.L152
	sub	r0, r0, #4
	ldr	r3, [r7, #52]
	add	r0, r1, r0
	ldr	r1, [r6, #184]
	blx	r3
.L152:
	ldr	r3, .L170+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L168
	ldr	r2, [r4, #3416]
.L169:
	movw	r3, #1064
	ldr	r0, [r5, #4]
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
	strb	r0, [r5, #46]
	mov	r0, r4
	bl	avs_u_v
	ldr	r2, .L175+12
	mov	r1, #1
	strb	r0, [r5, #47]
	mov	r0, r4
	bl	avs_u_v
	uxtb	r0, r0
	strb	r0, [r5, #48]
	cmp	r0, #1
	beq	.L174
.L173:
	ldr	r2, .L175+16
	mov	r1, #14
	mov	r0, r4
	bl	avs_u_v
	ldr	r2, .L175+20
	mov	r1, #1
	str	r0, [r5, #64]
	mov	r0, r4
	bl	avs_f_v
	ldr	r2, .L175+24
	mov	r1, #14
	strb	r0, [r5, #45]
	mov	r0, r4
	bl	avs_u_v
	ldr	r2, .L175+28
	mov	r1, #2
	str	r0, [r5, #68]
	mov	r0, r4
	bl	avs_u_v
	strb	r0, [r5, #44]
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
	str	r0, [r5, #52]
	mov	r0, r4
	bl	avs_u_v
	ldr	r2, .L175+40
	mov	r1, #8
	str	r0, [r5, #56]
	mov	r0, r4
	bl	avs_u_v
	str	r0, [r5, #60]
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
	ldrb	r3, [r7, #15]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L200
	ldrb	r3, [r7, #77]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L201
.L185:
	mov	r3, #1
	str	r3, [r7, #4]
.L187:
	add	r6, r7, #344
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
	ldr	r3, [r7, #4]
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
	ldrb	r3, [r7, #79]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L188
.L186:
	mov	r3, #2
	str	r3, [r7, #4]
	b	.L187
.L200:
	ldrb	r3, [r7, #79]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L185
	ldrb	r3, [r7, #78]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L186
.L188:
	mov	r3, #3
	str	r3, [r7, #4]
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
	add	r6, r0, #14912
	add	r5, r0, #15744
	mov	r7, r0
	mov	r8, r1
	ldr	r4, [r3, #3120]
	add	r6, r6, #4
	add	r5, r5, #12
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
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #48)
	sub	sp, sp, #48
	add	r3, r0, #12288
	mov	r4, r0
	ldr	r6, [r3, #2772]
	ldr	r7, [r3, #2776]
	cmp	r6, #31
	bls	.L224
	cmp	r7, #31
	bls	.L224
	ldr	r3, [r0, #12]
	ldr	r5, [r3, #708]
	ldr	r2, [r3, #28]
	cmp	r5, #0
	bne	.L227
	cmp	r2, #25
	beq	.L254
.L246:
	mov	r5, #4
	mov	r9, #5
	mov	r8, r5
.L228:
	ldr	r3, .L257
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #68
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r2, [r4, #12]
	str	r7, [fp, #-40]
	mov	r3, #0
	str	r6, [fp, #-60]
	ldr	r2, [r2, #28]
	str	r7, [fp, #-56]
	cmp	r2, #24
	str	r6, [fp, #-44]
	strb	r3, [fp, #-67]
	beq	.L237
	uxtb	r5, r5
	strb	r8, [fp, #-65]
	strb	r9, [fp, #-63]
.L241:
	sub	r1, fp, #68
	ldr	r0, [r4, #4]
	strb	r5, [fp, #-62]
	mov	r6, #0
	strb	r6, [fp, #-66]
	bl	FSP_ConfigInstance
	subs	r5, r0, #0
	bne	.L238
	ldr	r1, [r4, #12]
	sub	r3, fp, #72
	ldr	r0, [r4, #4]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	beq	.L255
	cmp	r0, #0
	moveq	r5, #1
	streq	r5, [r4, #8]
	beq	.L249
	ldr	r1, .L257+4
	mov	r0, r5
	bl	dprint_vfmw
	b	.L249
.L224:
	mov	r3, r7
	mov	r2, r6
	ldr	r1, .L257+8
	mov	r0, #0
	bl	dprint_vfmw
	mov	r5, #0
.L249:
	mov	r0, r5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L227:
	cmp	r2, #25
	bne	.L246
	ldr	r2, [r3, #668]
	ldr	r1, [r3, #672]
	cmp	r2, r6
	bcc	.L234
	cmp	r1, r7
	bcc	.L234
	ldr	r2, [r3, #696]
	ldr	r5, [r3, #692]
	cmp	r2, #0
	add	r8, r5, #2
	bne	.L256
.L247:
	mov	r5, #1
	b	.L236
.L238:
	ldr	r1, .L257+12
	mov	r0, r6
	bl	dprint_vfmw
	mov	r5, r6
	mov	r0, r5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L254:
	ldr	r2, [r3, #668]
	ldr	r1, [r3, #672]
	cmp	r2, r6
	bcc	.L230
	cmp	r1, r7
	bcc	.L230
	ldr	r5, [r0, #8]
	cmp	r5, #1
	beq	.L249
	ldr	r1, [r3, #696]
	ldr	r2, [r3, #692]
	cmp	r1, #0
	add	r8, r2, #2
	beq	.L247
	add	r5, r2, #1
	cmp	r5, #17
	movge	r5, #17
.L236:
	ldrb	r9, [r3, #716]	@ zero_extendqisi2
	b	.L228
.L237:
	mov	r5, #1
	strb	r9, [fp, #-63]
	strb	r5, [fp, #-65]
	b	.L241
.L234:
	str	r1, [sp, #4]
	mov	r3, r7
	str	r2, [sp]
	mov	r0, #0
	mov	r2, r6
	ldr	r1, .L257+16
	bl	dprint_vfmw
	mov	r5, #0
	b	.L249
.L256:
	add	r5, r5, #1
	cmp	r5, #17
	movge	r5, #17
	b	.L236
.L230:
	str	r1, [sp, #4]
	mov	r3, r7
	str	r2, [sp]
	mov	r0, #0
	mov	r2, r6
	ldr	r1, .L257+16
	bl	dprint_vfmw
	b	.L249
.L255:
	add	r0, r4, #16128
	mov	r5, #3
	bl	ResetVoQueue
	b	.L249
.L258:
	.align	2
.L257:
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
	ldr	r3, [r4, #264]
	strb	r3, [r0, #20]
	ldr	r3, [r4, #268]
	strb	r3, [r0, #21]
	ldr	r3, [r4, #272]
	strb	r3, [r0, #22]
	ldr	r3, [r4, #276]
	strb	r3, [r0, #23]
	ldr	r3, [r4, #280]
	strb	r3, [r0, #24]
	ldr	r3, [r4, #292]
	str	r3, [r0, #28]
	ldr	r3, [r4, #296]
	str	r3, [r0, #80]
	ldr	r3, [r4, #316]
	str	r3, [r0, #32]
	ldr	r3, [r4, #444]
	str	r3, [r0, #92]
	ldr	r3, [r4, #456]
	str	r3, [r0, #96]
	ldr	r3, [r4, #448]
	str	r3, [r0, #100]
	ldr	r3, [r4, #460]
	str	r3, [r0, #104]
	ldr	r3, [r4, #452]
	str	r3, [r0, #108]
	ldr	r3, [r4, #464]
	str	r3, [r0, #112]
	ldr	r3, [r4, #324]
	strh	r3, [r0, #76]	@ movhi
	ldr	r3, [r4, #320]
	strh	r3, [r0, #78]	@ movhi
	ldr	r3, [r4, #328]
	strb	r3, [r0, #59]
	ldr	r3, [r4, #332]
	strb	r3, [r0, #60]
	ldr	r3, [r4, #368]
	strb	r3, [r0, #36]
	ldr	r3, [r4, #372]
	strb	r3, [r0, #37]
	ldr	r3, [r4, #336]
	strb	r3, [r0, #38]
	ldr	r3, [r4, #340]
	strb	r3, [r0, #39]
	ldr	r3, [r4, #348]
	strb	r3, [r0, #40]
	ldr	r3, [r4, #352]
	strb	r3, [r0, #41]
	ldr	r3, [r4, #356]
	strb	r3, [r0, #42]
	ldr	r3, [r4, #360]
	strb	r3, [r0, #43]
	ldr	r3, [r4, #364]
	strb	r3, [r0, #44]
	ldr	r3, [r4, #376]
	strb	r3, [r0, #45]
	ldr	r3, [r4, #380]
	strb	r3, [r0, #46]
	ldr	r3, [r4, #384]
	strb	r3, [r0, #47]
	ldr	r3, [r4, #388]
	strb	r3, [r0, #48]
	ldr	r3, [r4, #392]
	strb	r3, [r0, #49]
	ldr	r3, [r4, #436]
	strb	r3, [r0, #50]
	ldr	r3, [r4, #344]
	strb	r3, [r0, #51]
	ldr	r3, [r4, #400]
	strb	r3, [r0, #52]
	ldr	r3, [r4, #396]
	strb	r3, [r0, #53]
	ldr	r3, [r4, #416]
	strb	r3, [r0, #54]
	ldr	r3, [r4, #412]
	strb	r3, [r0, #55]
	ldr	r3, [r4, #408]
	strb	r3, [r0, #56]
	ldr	r3, [r4, #404]
	strb	r3, [r0, #57]
	ldr	r3, [r4, #440]
	strb	r3, [r0, #58]
	ldr	r2, [r4, #420]
	ldr	r3, [r6, #2704]
	str	r2, [r6, #2168]
	ldr	r2, [r4, #424]
	str	r2, [r6, #2172]
	ldr	r2, [r4, #428]
	str	r2, [r6, #2176]
	ldr	r2, [r4, #496]
	str	r2, [r6, #2184]
	ldr	r2, [r4, #508]
	str	r2, [r6, #2188]
	ldr	r1, [r3, #4]
	ldr	r0, [r0, #4]
	bl	FSP_GetLogicFs
	subs	r7, r0, #0
	beq	.L274
	ldr	r2, [r7, #644]
	add	r0, r5, #14592
	add	r0, r0, #20
	add	r3, r5, #16896
	add	ip, r5, #17152
	add	r3, r3, #130
	ldr	r1, [r2, #32]
	add	ip, ip, #2
	mov	r2, r0
	str	r1, [r6, #2584]
	ldr	r1, [r7, #644]
	ldr	r1, [r1, #76]
	str	r1, [r6, #2592]
	ldr	r1, [r7, #644]
	ldr	r1, [r1, #60]
	str	r1, [r6, #2596]
	ldr	r1, [r7, #644]
	ldr	r1, [r1, #64]
	str	r1, [r6, #2600]
	ldr	r1, [r4, #316]
	str	r1, [r6, #2180]
	ldr	r1, [r4, #432]
	str	r1, [r6, #2192]
	ldr	r1, [r4, #640]
	str	r1, [r6, #2196]
	ldr	r1, [r4, #468]
	strb	r1, [r5, #61]
	ldr	r1, [r4, #472]
	strb	r1, [r5, #62]
	ldr	r1, [r4, #480]
	strb	r1, [r5, #63]
	ldr	r1, [r4, #476]
	strb	r1, [r5, #64]
	ldr	r1, [r4, #484]
	strb	r1, [r5, #65]
	ldr	r1, [r4, #488]
	strb	r1, [r5, #66]
	ldr	r1, [r4, #492]
	strb	r1, [r5, #67]
.L262:
	ldrsh	r1, [r3, #2]!
	cmp	r3, ip
	str	r1, [r2, #4]!
	bne	.L262
	ldr	r1, [r4, #300]
	add	r2, r5, #16640
	add	r3, r5, #14464
	add	r2, r2, #252
	add	r3, r3, #20
	str	r1, [r6, #2604]
	ldr	r1, [r4, #304]
	str	r1, [r6, #2608]
	ldr	r1, [r4, #308]
	str	r1, [r6, #2612]
	ldr	r1, [r4, #312]
	str	r1, [r6, #2616]
.L263:
	ldr	r1, [r2, #4]!
	str	r1, [r3, #4]!
	cmp	r3, r0
	bne	.L263
	ldr	r3, [r6, #3408]
	add	r1, r5, #20
	add	r3, r3, #1
	str	r3, [r6, #3408]
	ldr	r3, [r5, #12]
	str	r1, [r5, #16]
	ldr	r2, [r3, #864]
	cmp	r2, #1
	beq	.L275
	cmp	r2, #2
	beq	.L276
.L265:
	ldr	r2, [r3, #592]
	mov	r0, #1
	str	r2, [r6, #2620]
	ldr	r3, [r3, #632]
	str	r3, [r6, #2624]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L275:
	cmp	r1, #0
	beq	.L265
	ldr	r1, [r6, #2704]
	ldrb	r2, [r1, #1]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L265
	mov	r7, #0
	ldr	r0, [r5, #4]
	str	r7, [r5, #16]
	mov	r2, r7
	ldr	r1, [r1, #4]
	bl	FSP_SetDisplay
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L276:
	ldr	r2, [r3, #868]
	cmp	r1, #0
	cmpne	r2, #0
	ble	.L265
	ldr	r1, [r6, #2704]
	ldrb	r2, [r1, #1]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L265
	mov	r7, #0
	ldr	r0, [r5, #4]
	str	r7, [r5, #16]
	mov	r2, r7
	ldr	r1, [r1, #4]
	bl	FSP_SetDisplay
	ldr	r2, [r5, #12]
	mov	r0, r7
	ldr	r3, [r2, #868]
	sub	r3, r3, #1
	str	r3, [r2, #868]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L274:
	movw	r2, #2200
	ldr	r1, .L277
	bl	dprint_vfmw
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L278:
	.align	2
.L277:
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
	ldrb	r3, [r5, #2763]	@ zero_extendqisi2
	ldr	r10, [r5, #3092]
	cmp	r3, #1
	ldrb	r7, [r5, #2794]	@ zero_extendqisi2
	beq	.L286
	ldrb	r3, [r5, #2824]	@ zero_extendqisi2
	subs	r3, r3, #1
	movne	r3, #1
	str	r3, [fp, #-52]
.L280:
	ldrb	r3, [r5, #2761]	@ zero_extendqisi2
	ldr	r0, [r6, #4]
	sub	r3, r3, #1
	ldrb	r9, [r5, #2826]	@ zero_extendqisi2
	uxtb	r3, r3
	cmp	r3, #3
	ldrls	r2, .L292
	movhi	r8, #0
	ldrls	r8, [r2, r3, asl #2]
	ldr	r2, [r5, #2704]
	ldr	r1, [r2, #4]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L290
	and	ip, r10, #3
	and	r9, r9, #3
	orr	r1, ip, #3072
	ldr	r3, [fp, #-52]
	and	r7, r7, #3
	orr	r9, r1, r9, asl #12
	and	lr, r8, #7
	mov	r1, r8
	orr	r2, r9, r7, asl #5
	orr	r3, r2, r3, asl #8
	orr	r3, r3, lr, asl #14
	str	r3, [r4, #108]
	ldr	r2, [r6, #12]
	ldr	r2, [r2, #592]
	cmp	r2, #1
	moveq	r2, #262144
	movne	r2, #0
	orr	r3, r2, r3
	str	r3, [r4, #108]
	ldr	r3, [r5, #3400]
	str	ip, [r4, #100]
	str	ip, [r4, #104]
	str	r3, [r4, #160]
	bl	SetAspectRatio
	ldr	r3, [r5, #2772]
	str	r3, [r4, #112]
	ldr	r3, [r5, #2776]
	str	r3, [r4, #116]
	ldr	r2, [r6, #12]
	ldr	r2, [r2, #28]
	cmp	r2, #25
	beq	.L291
.L284:
	mov	r3, #6
	str	r3, [r4, #376]
	ldrb	r1, [r5, #2825]	@ zero_extendqisi2
	mov	r2, #1
	mov	r3, #8
	strb	r2, [r4, #371]
	cmp	r1, r2
	strb	r2, [r4, #369]
	str	r3, [r4, #380]
	moveq	r2, #0
	movne	r2, #4
	str	r3, [r4, #388]
	strb	r2, [r4, #364]
	str	r3, [r4, #384]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L286:
	mov	r3, #0
	str	r3, [fp, #-52]
	b	.L280
.L291:
	ldr	r2, [r4, #140]
	add	r3, r3, #15
	ldr	ip, [r4, #44]
	add	r8, r4, #512
	mov	r3, r3, lsr #4
	add	r1, r4, #528
	mov	r0, r2, asl #4
	add	r9, ip, r2
	mul	r3, r3, r0
	add	lr, r3, ip
	str	lr, [r4, #48]
	ldrd	r6, [r8, #-8]
	add	r2, lr, r2
	mov	r0, lr
	strd	r6, [fp, #-52]
	adds	r6, r6, r3
	adc	r7, r7, r3, asr #31
	strd	r6, [r8]
	str	r2, [r4, #40]
	mov	r6, r1
	ldrd	r2, [fp, #-52]
	str	lr, [r4, #32]
	str	ip, [r4, #28]
	str	r9, [r4, #36]
	strd	r2, [r1, #-8]
	str	ip, [r4, #60]
	bl	MEM_Phy2Vir
	mov	r3, #0
	str	r0, [r4, #528]
	str	r3, [r6, #4]
	ldr	r3, [r4, #32]
	str	r3, [r4, #64]
	b	.L284
.L290:
	movw	r2, #2324
	ldr	r1, .L292+4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L293:
	.align	2
.L292:
	.word	.LANCHOR0
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r4, r0, #12288
	mov	r5, r0
	ldr	r3, [r4, #3812]
	cmp	r3, #2
	beq	.L295
	ldr	r6, [r4, #3132]
	add	r7, r0, #14912
	add	r7, r7, #4
	add	r2, r6, #1
	mov	r3, r6, asl #4
	mov	r1, r2, asr #31
	sub	r6, r3, r6, asl #2
	add	r6, r6, #16
	mov	r3, r1, lsr #30
	add	r6, r7, r6
	add	r2, r2, r3
	str	r6, [r4, #2704]
	and	r2, r2, #3
	rsb	r2, r3, r2
	str	r2, [r4, #3132]
.L296:
	mov	r1, #1
	ldr	r0, [r5, #4]
	bl	FSP_NewLogicFs
	str	r0, [r6, #4]
	ldr	r3, [r4, #2704]
	ldr	r1, [r3, #4]
	cmp	r1, #0
	blt	.L314
	ldr	r0, [r5, #4]
	bl	FSP_GetLogicFs
	subs	r5, r0, #0
	beq	.L315
	ldr	r3, [r4, #2704]
	mov	r0, #18
	ldr	r1, .L316
	ldr	r2, [r3, #4]
	bl	dprint_vfmw
	ldr	r2, [r5, #640]
	cmp	r2, #0
	beq	.L301
	ldr	r3, [r5, #644]
	cmp	r3, #0
	beq	.L301
	ldr	r1, [r5, #648]
	cmp	r1, #0
	beq	.L301
	ldr	ip, [r1, #16]
	mov	r0, #18
	ldr	r3, [r3, #16]
	ldr	r2, [r2, #16]
	ldr	r1, .L316+4
	str	ip, [sp]
	bl	dprint_vfmw
.L301:
	mov	r0, #1
	mov	r3, #0
	str	r3, [r5, #168]
.L299:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L295:
	add	r7, r0, #14912
	add	r6, r7, #68
	add	r7, r7, #4
	str	r6, [r4, #2704]
	b	.L296
.L314:
	ldr	r1, .L316+8
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r4, #2708]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	r10, r0, #0
	beq	.L298
	ldr	r3, [r4, #2704]
	mov	r2, #1
	ldr	r0, [r5, #4]
	add	r8, r10, #576
	add	r6, r10, #592
	add	r9, r10, #560
	ldr	r1, [r3, #4]
	add	r7, r7, #4
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2708]
	mov	r2, #1
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2712]
	mov	r2, #1
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2716]
	mov	r2, #1
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r1, [r10, #560]
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	ldr	r1, [r10, #568]
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	ldr	r1, [r10, #576]
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	ldr	r1, [r10, #584]
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	mov	r2, #0
	mov	r3, #0
	strd	r2, [r9]
	strd	r2, [r8, #-8]
	strd	r2, [r8]
	strd	r2, [r6, #-8]
	str	r7, [r4, #2704]
	str	r7, [r4, #2716]
	str	r7, [r4, #2712]
	str	r7, [r4, #2708]
	ldr	r0, [r5, #4]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #2636]
.L298:
	mov	r6, #0
	add	r1, r5, #16128
	str	r6, [r4, #3396]
	ldr	r0, [r5, #4]
	bl	FSP_ClearNotInVoQueue
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L315:
	movw	r2, #2441
	ldr	r1, .L316+12
	bl	dprint_vfmw
	mov	r0, r5
	b	.L299
.L317:
	.align	2
.L316:
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
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	mov	r6, r0
	mov	r4, r1
	ldr	r0, [r1, #4]
	ldr	r1, [r6, #4]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L324
	mov	r2, #1
	ldr	r1, [r6, #4]
	ldr	r0, [r4, #4]
	bl	FSP_SetDisplay
	ldr	r0, [r4, #4]
	add	r3, r4, #16128
	str	r5, [sp]
	mov	r2, r4
	mov	r1, #6
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L320
	ldr	r0, [r4, #4]
	mov	r2, #0
	ldr	r1, [r6, #4]
	bl	FSP_SetDisplay
	ldr	r2, [r6, #4]
	ldr	r1, .L325
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
.L320:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L324:
	movw	r2, #2482
	ldr	r1, .L325+4
	bl	dprint_vfmw
	mov	r0, r5
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L326:
	.align	2
.L325:
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
	ldrb	r3, [r0, #197]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r0, #464]
	beq	.L341
	cmp	r3, #0
	beq	.L342
	cmp	r3, #1
	bne	.L336
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
.L332:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L342:
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #8]
	mov	r3, r3, asl #1
	str	r3, [r0, #540]
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L341:
	cmp	r3, #1
	beq	.L343
	cmp	r3, #2
	bne	.L332
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
.L336:
	cmp	r3, #2
	bne	.L332
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
.L343:
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
	b	.L332
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
	ldrb	r0, [r0, #197]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L395
	ldr	r3, [r6, #464]
	cmp	r3, #0
	moveq	r3, #512
	streq	r3, [r6, #620]
	beq	.L381
	cmp	r3, #1
	bne	.L371
	ldr	r3, [r6, #76]
	add	r10, r6, #536
	str	r3, [fp, #-60]
	add	r3, r6, #520
	str	r3, [fp, #-48]
	add	r3, r6, #552
	str	r3, [fp, #-52]
	mov	r3, #0
	str	r3, [fp, #-56]
.L360:
	ldr	r3, [fp, #-60]
	mov	r8, r10
	ldr	r7, [fp, #-52]
	ldr	r6, [fp, #-48]
	ldr	r2, [r3, #8]
	ldr	r3, [fp, #-56]
	add	r9, r3, r2, lsl #1
	mov	r10, r9
.L356:
	ldr	r5, [r6, #4]!
	mov	r9, r7
	ldr	r4, [fp, #-48]
	rsb	r5, r5, r10
	ubfx	r5, r5, #0, #9
.L358:
	ldr	r1, [r4, #4]!
	mov	r0, #512
	mov	r3, #1
	rsb	r1, r1, r10
	ubfx	r1, r1, #0, #9
	cmp	r1, #0
	beq	.L357
	bl	__aeabi_idiv
	mov	r3, r0
.L357:
	mul	r3, r3, r5
	cmp	r8, r4
	str	r3, [r9, #4]!
	bne	.L358
	cmp	r8, r6
	add	r7, r7, #16
	bne	.L356
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
	bne	.L360
.L381:
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L395:
	ldr	r2, [r6, #76]
	ldr	r3, [r6, #464]
	ldr	r5, [r2, #8]
	cmp	r3, #1
	addeq	r9, r6, #520
	addeq	r7, r6, #32
	moveq	r8, r6
	mov	r5, r5, asl #1
	beq	.L347
	cmp	r3, #2
	bne	.L381
	ldr	r3, [r6, #524]
	rsb	r3, r3, r5
	ubfx	r4, r3, #0, #9
	cmp	r4, #0
	beq	.L353
	mov	r1, r4
	mov	r0, #512
	bl	__aeabi_idiv
.L353:
	mul	r3, r0, r4
	ldr	r4, [r6, #532]
	rsb	r4, r5, r4
	ubfx	r4, r4, #0, #9
	cmp	r4, #0
	str	r3, [r6, #556]
	moveq	r0, #1
	beq	.L354
	mov	r1, r4
	mov	r0, #512
	bl	__aeabi_idiv
.L354:
	mul	r4, r0, r4
	mov	r0, #1
	str	r4, [r6, #572]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L347:
	ldr	r1, [r6, #524]
	mov	r0, #512
	ldr	r4, [r9, #4]!
	mov	r3, #1
	rsb	r1, r1, r5
	ubfx	r1, r1, #0, #9
	rsb	r4, r4, r5
	cmp	r1, #0
	ubfx	r4, r4, #0, #9
	beq	.L349
	bl	__aeabi_idiv
	mov	r3, r0
.L349:
	mul	r3, r3, r4
	mov	r0, #1
	str	r3, [r8, #556]
	ldr	r1, [r6, #528]
	rsb	r1, r1, r5
	ubfx	r1, r1, #0, #9
	cmp	r1, #0
	beq	.L350
	mov	r0, #512
	bl	__aeabi_idiv
.L350:
	mul	r0, r0, r4
	add	r8, r8, #16
	str	r0, [r8, #544]
	cmp	r8, r7
	bne	.L347
	b	.L381
.L371:
	cmp	r3, #2
	bne	.L381
	ldr	r3, [r6, #76]
	mov	r7, r6
	add	r6, r6, #16
	add	r10, r7, #520
	mov	r8, #0
	str	r3, [fp, #-52]
.L370:
	ldr	r3, [fp, #-52]
	mov	r9, r10
	ldr	r4, [r3, #8]
	mov	r3, r7
	add	r4, r8, r4, lsl #1
.L361:
	ldr	r1, [r10, #4]
	mov	r0, #512
	ldr	r5, [r9, #4]!
	mov	ip, #1
	rsb	r1, r1, r4
	ubfx	r1, r1, #0, #9
	rsb	r5, r5, r4
	cmp	r1, #0
	ubfx	r5, r5, #0, #9
	beq	.L363
	str	r3, [fp, #-48]
	bl	__aeabi_idiv
	ldr	r3, [fp, #-48]
	mov	ip, r0
.L363:
	mul	ip, ip, r5
	mov	r0, #1
	str	ip, [r3, #556]
	ldr	r1, [r10, #8]
	rsb	r1, r1, r4
	ubfx	r1, r1, #0, #9
	cmp	r1, #0
	beq	.L364
	mov	r0, #512
	str	r3, [fp, #-48]
	bl	__aeabi_idiv
	ldr	r3, [fp, #-48]
.L364:
	mul	r0, r0, r5
	add	r3, r3, #8
	str	r0, [r3, #552]
	cmp	r3, r6
	bne	.L361
	add	r2, r10, #8
	mov	r9, r7
.L362:
	ldr	r1, [r10, #12]
	mov	r0, #512
	ldr	r5, [r2, #4]!
	mov	ip, #1
	rsb	r1, r4, r1
	ubfx	r1, r1, #0, #9
	rsb	r5, r4, r5
	cmp	r1, #0
	ubfx	r5, r5, #0, #9
	beq	.L367
	str	r2, [fp, #-48]
	bl	__aeabi_idiv
	ldr	r2, [fp, #-48]
	mov	ip, r0
.L367:
	mul	ip, ip, r5
	mov	r0, #1
	str	ip, [r9, #572]
	ldr	r1, [r10, #16]
	rsb	r1, r4, r1
	ubfx	r1, r1, #0, #9
	cmp	r1, #0
	beq	.L368
	mov	r0, #512
	str	r2, [fp, #-48]
	bl	__aeabi_idiv
	ldr	r2, [fp, #-48]
.L368:
	mul	r0, r0, r5
	add	r9, r9, #8
	str	r0, [r9, #568]
	cmp	r6, r9
	bne	.L362
	add	r8, r8, #1
	add	r6, r6, #32
	cmp	r8, #2
	add	r7, r7, #32
	add	r10, r10, #16
	bne	.L370
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
	ldrb	r3, [r0, #197]	@ zero_extendqisi2
	mov	r6, r0
	cmp	r3, #1
	beq	.L419
	ldr	r8, [r0, #80]
	mov	r5, #1
	add	r9, r0, #4
	add	r7, r0, #684
	mov	r4, #0
	ldr	r10, [r8, #8]
	mov	r10, r10, asl r5
.L403:
	sub	r3, r5, #1
	mov	r0, #16384
	and	r1, r5, #1
	add	r3, r3, r3, lsr #31
	mov	r3, r3, asr #1
	add	r3, r3, #21
	ldr	r3, [r6, r3, asl #2]
	cmp	r9, r3
	beq	.L401
	ldr	r4, [r3, #8]
	add	r4, r1, r4, lsl #1
	rsb	r1, r4, r10
	ubfx	r1, r1, #0, #9
	cmp	r1, #0
	moveq	r3, #32
	streq	r3, [r7]
	beq	.L401
	bl	__aeabi_idiv
	str	r0, [r7]
.L401:
	add	r5, r5, #1
	add	r7, r7, #4
	cmp	r5, #5
	bne	.L403
	ldr	r2, [r8, #8]
	add	r10, r6, #700
	ldr	ip, [r6, #800]
	mov	r5, #1
	mov	r7, #0
	cmp	ip, #0
	moveq	r2, r2, asl #1
	movne	r8, r2, asl #1
	addeq	r8, r2, #1
.L413:
	cmp	ip, #0
	beq	.L406
	add	r3, r7, r7, lsr #31
	mov	r3, r3, asr #1
	add	r3, r3, #21
	ldr	r3, [r6, r3, asl #2]
	cmp	r9, r3
	beq	.L407
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
	beq	.L409
	mov	r0, #16384
	str	ip, [fp, #-48]
	bl	__aeabi_idiv
	ldr	ip, [fp, #-48]
	str	r0, [r10, #-16]
.L410:
	str	r0, [r10]
.L412:
	add	r5, r5, #1
	add	r7, r7, #1
	cmp	r5, #5
	add	r10, r10, #4
	bne	.L413
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L406:
	add	r3, r5, r5, lsr #31
	mov	r3, r3, asr #1
	add	r3, r3, #20
	ldr	r3, [r6, r3, asl #2]
	cmp	r9, r3
	ldrne	r4, [r3, #8]
	andne	r3, r7, #1
	addne	r4, r3, r4, lsl #1
.L407:
	rsb	r0, r4, r8
	add	r0, r0, #512
	mov	r1, r0, asr #31
	mov	r1, r1, lsr #23
	add	r0, r0, r1
	ubfx	r3, r0, #0, #9
	subs	r1, r3, r1
	beq	.L409
	mov	r0, #16384
	str	ip, [fp, #-48]
	bl	__aeabi_idiv
	ldr	ip, [fp, #-48]
	b	.L410
.L409:
	mov	r3, #32
	str	r3, [r10]
	b	.L412
.L419:
	ldr	r3, [r0, #80]
	add	r7, r0, #4
	add	r4, r0, #80
	add	r6, r0, #88
	mov	r8, #32
	ldr	r5, [r3, #8]
	mov	r5, r5, asl #1
.L400:
	ldr	r3, [r4, #4]!
	mov	r0, #16384
	cmp	r3, r7
	beq	.L398
	ldr	r1, [r3, #8]
	sub	r1, r5, r1, asl #1
	ubfx	r1, r1, #0, #9
	cmp	r1, #0
	streq	r8, [r4, #600]
	beq	.L398
	bl	__aeabi_idiv
	str	r0, [r4, #600]
.L398:
	cmp	r4, r6
	bne	.L400
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
	beq	.L427
	ldrb	r0, [r0, #197]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L422
	ldr	ip, [r5, #76]
	add	r2, r5, #520
	add	r5, r5, #712
	mov	r3, #0
.L425:
	ldr	r6, [ip, #8]
	add	r10, r2, #20
	mov	r9, r5
	mov	r8, r2
	mov	r7, #2
	add	r6, r3, r6, lsl #1
.L424:
	ldr	r1, [r8, #4]!
	mov	r0, #512
	ldr	r4, [r10, #-4]!
	mov	lr, #1
	rsb	r1, r1, r6
	ubfx	r1, r1, #0, #9
	rsb	r4, r6, r4
	cmp	r1, #0
	ubfx	r4, r4, #0, #9
	beq	.L423
	str	ip, [fp, #-56]
	str	r3, [fp, #-52]
	str	r2, [fp, #-48]
	bl	__aeabi_idiv
	ldr	r2, [fp, #-48]
	ldr	r3, [fp, #-52]
	ldr	ip, [fp, #-56]
	mov	lr, r0
.L423:
	mul	r4, lr, r4
	subs	r7, r7, #1
	str	r4, [r9, #4]!
	bne	.L424
	add	r3, r3, #1
	add	r2, r2, #16
	cmp	r3, #2
	add	r5, r5, #8
	bne	.L425
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L422:
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
	bne	.L432
.L426:
	mul	r3, r0, r4
	mov	r0, #1
	str	r3, [r5, #716]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L432:
	mov	r0, #512
	bl	__aeabi_idiv
	b	.L426
.L427:
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
	beq	.L436
	ldrb	r0, [r0, #197]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L437
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
.L437:
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
.L436:
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
	ldr	r4, [r0, #464]
	mov	r5, r0
	cmp	r4, #1
	beq	.L447
	bcc	.L441
	cmp	r4, #2
	bne	.L449
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
.L449:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L441:
	ldrb	r4, [r0, #197]	@ zero_extendqisi2
	cmp	r4, #1
	beq	.L446
.L447:
	bl	AVS_UpdateTableIdx2Dist
	mov	r0, r5
	bl	AVS_UpdateTableMvEScale
.L446:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	AVS_UpdatePmvTable, .-AVS_UpdatePmvTable
	.align	2
	.global	AVS_GetStreamInf
	.type	AVS_GetStreamInf, %function
AVS_GetStreamInf:
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
	UNWIND(.pad #36)
	sub	sp, sp, #36
	add	r4, r0, #12288
	mov	r5, r0
	ldr	r2, [r4, #2744]
	ldrb	r3, [r4, #2760]	@ zero_extendqisi2
	cmp	r2, #1
	str	r3, [r4, #3424]
	beq	.L451
	ldr	r3, [r0, #8]
	cmp	r3, #1
	beq	.L452
.L451:
	ldr	r1, .L559
	mov	r0, #22
	bl	dprint_vfmw
	sub	r3, fp, #44
	mov	ip, #0
	sub	r2, fp, #52
	str	ip, [r3, #-4]!
	sub	r1, fp, #56
	ldr	r0, [r5, #4]
	str	ip, [fp, #-56]
	str	ip, [fp, #-52]
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r5, #12]
	ldr	r2, [r3, #708]
	cmp	r2, #1
	beq	.L544
.L454:
	ldrb	r3, [r4, #2763]	@ zero_extendqisi2
	ldr	r2, [r4, #2772]
	cmp	r3, #1
	ldr	r3, [r4, #2776]
	add	r2, r2, #15
	addne	r3, r3, #31
	addeq	r3, r3, #15
	mov	r2, r2, lsr #4
	str	r2, [r4, #2724]
	movne	r3, r3, lsr #5
	moveq	r3, r3, lsr #4
	movne	r3, r3, asl #1
	cmp	r2, #512
	str	r3, [r4, #2728]
	bgt	.L461
	cmp	r3, #512
	ble	.L462
.L461:
	ldr	r1, .L559+4
	mov	r0, #18
	bl	dprint_vfmw
	mov	r7, #0
.L457:
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L462:
	ldr	r3, [r4, #2708]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	addeq	r6, r5, #14912
	addeq	r6, r6, #8
	beq	.L464
	ldr	r3, [r4, #2704]
	mov	r2, #1
	ldr	r0, [r5, #4]
	add	r9, r7, #576
	add	r8, r7, #592
	add	r10, r7, #560
	ldr	r1, [r3, #4]
	add	r6, r5, #14912
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2708]
	mov	r2, #1
	ldr	r0, [r5, #4]
	add	r6, r6, #8
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2712]
	mov	r2, #1
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2716]
	mov	r2, #1
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r1, [r7, #560]
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	ldr	r1, [r7, #568]
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	ldr	r1, [r7, #576]
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	ldr	r1, [r7, #584]
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	mov	r2, #0
	mov	r3, #0
	strd	r2, [r10]
	strd	r2, [r9, #-8]
	strd	r2, [r9]
	strd	r2, [r8, #-8]
	str	r6, [r4, #2704]
	str	r6, [r4, #2716]
	str	r6, [r4, #2712]
	str	r6, [r4, #2708]
	ldr	r0, [r5, #4]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #2636]
.L464:
	add	r7, r5, #16128
	mov	r3, #0
	str	r3, [r4, #3396]
	mov	r1, r7
	ldr	r0, [r5, #4]
	bl	FSP_ClearNotInVoQueue
	ldr	r3, [r5, #12]
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L465
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L545
.L465:
	ldr	r0, [r5, #4]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L541
.L467:
	mov	r7, #0
	str	r6, [r4, #2704]
	str	r7, [r4, #2744]
	mov	r0, r5
	str	r6, [r4, #2716]
	str	r6, [r4, #2712]
	str	r6, [r4, #2708]
	bl	AVS_AllocateVHB
	cmp	r0, #3
	mov	ip, r0
	moveq	r7, r0
	beq	.L457
	cmp	r0, r7
	strne	r7, [r4, #2744]
	beq	.L546
.L452:
	ldrb	r1, [r4, #2759]	@ zero_extendqisi2
	mov	r0, r5
	ldr	r3, [r4, #2704]
	ldrb	r2, [r4, #2825]	@ zero_extendqisi2
	str	r1, [r4, #3420]
	strb	r2, [r3]
	bl	AVS_GetImageBuffer
	subs	r7, r0, #0
	beq	.L547
	ldr	r3, [r4, #2704]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L548
	ldr	r3, [r5, #12]
	add	lr, ip, #608
	add	r8, ip, #624
	mvn	r6, #0
	mvn	r7, #0
	ldr	r1, .L559+8
	ldrd	r2, [r3, #56]
	mov	r0, #29
	strd	r2, [lr, #-8]
	ldr	r3, [r5, #12]
	ldrd	r2, [r3, #72]
	strd	r2, [lr]
	ldr	r3, [r5, #12]
	ldrd	r2, [r3, #88]
	strd	r2, [r8, #-8]
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #104]
	str	r3, [ip, #8]
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #108]
	str	r3, [ip, #12]
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #112]
	str	r3, [ip, #16]
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #128]
	str	r3, [ip, #20]
	ldrb	r3, [r4, #2825]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [ip, #96]
	ldr	r3, [r5, #12]
	strd	r6, [r3, #56]
	ldrd	r6, [lr]
	ldrd	r2, [lr, #-8]
	strd	r6, [sp]
	bl	dprint_vfmw
	ldr	r3, [r4, #2704]
	ldr	r2, [r4, #2856]
	str	r2, [r3, #8]
	ldr	r3, [r4, #2704]
	ldrb	r2, [r4, #2825]	@ zero_extendqisi2
	strb	r2, [r3]
	ldr	r3, [r4, #2704]
	ldr	r2, [r4, #3092]
	strb	r2, [r3, #1]
	ldrb	r2, [r4, #2825]	@ zero_extendqisi2
	ldr	r3, [r4, #2704]
	cmp	r2, #0
	ldrneb	r2, [r4, #2826]	@ zero_extendqisi2
	moveq	r2, #1
	strb	r2, [r3, #2]
	ldrb	r3, [r4, #2825]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L542
	ldr	r7, [r4, #3092]
	cmp	r7, #0
	beq	.L539
.L542:
	add	r6, r5, #14912
	add	r6, r6, #4
.L473:
	ldr	r3, [r4, #3088]
	mov	r0, r6
	add	r6, r5, #16384
	mov	r7, #6
	str	r3, [r4, #3428]
	bl	AVS_UpdatePmvTable
	ldr	lr, [r4, #2724]
	ldr	ip, [r4, #2728]
	mov	r2, #0
	mov	r3, #1
	str	r2, [r6, #268]
	str	r3, [r6, #272]
	add	r2, r5, #16896
	mul	r1, ip, lr
	add	r3, r5, #15232
	add	r0, r5, #15360
	add	r3, r3, #2
	add	r2, r2, #130
	add	r0, r0, #2
	sub	r1, r1, #1
	str	r1, [r6, #264]
	ldr	r1, [r4, #3144]
	str	lr, [r6, #324]
	cmp	r1, #0
	add	lr, r1, #63
	str	ip, [r6, #320]
	movlt	r1, lr
	str	r7, [r6, #280]
	mov	r1, r1, asr r7
	str	r1, [r6, #276]
	ldr	r1, [r4, #3424]
	str	r1, [r6, #328]
	ldr	r1, [r4, #3420]
	str	r1, [r6, #332]
	ldrb	r1, [r4, #2838]	@ zero_extendqisi2
	str	r1, [r6, #336]
	ldrb	r1, [r4, #2837]	@ zero_extendqisi2
	str	r1, [r6, #340]
	ldrb	r1, [r4, #2836]	@ zero_extendqisi2
	str	r1, [r6, #344]
	ldrb	r1, [r4, #2830]	@ zero_extendqisi2
	str	r1, [r6, #348]
	ldrb	r1, [r4, #2829]	@ zero_extendqisi2
	str	r1, [r6, #352]
	ldrb	r1, [r4, #2828]	@ zero_extendqisi2
	str	r1, [r6, #356]
	ldrb	r1, [r4, #2827]	@ zero_extendqisi2
	str	r1, [r6, #360]
	ldrb	r1, [r4, #2826]	@ zero_extendqisi2
	str	r1, [r6, #364]
	ldr	r1, [r4, #3092]
	str	r1, [r6, #368]
	ldrb	r1, [r4, #2825]	@ zero_extendqisi2
	cmp	r1, #0
	moveq	r1, #1
	movne	r1, #3
	str	r1, [r6, #372]
	ldrb	r1, [r4, #2824]	@ zero_extendqisi2
	str	r1, [r6, #376]
	ldr	r1, [r4, #2868]
	str	r1, [r6, #380]
	ldr	r1, [r4, #2864]
	str	r1, [r6, #384]
	ldrb	r1, [r4, #2833]	@ zero_extendqisi2
	str	r1, [r6, #388]
	ldrb	r1, [r4, #2832]	@ zero_extendqisi2
	str	r1, [r6, #392]
	ldr	r1, [r4, #2872]
	str	r1, [r6, #468]
	ldr	r1, [r4, #2880]
	str	r1, [r6, #472]
	ldr	r1, [r4, #2884]
	str	r1, [r6, #480]
	ldr	r1, [r4, #2888]
	str	r1, [r6, #476]
.L476:
	ldrh	r1, [r3, #2]!
	cmp	r3, r0
	strh	r1, [r2, #2]!	@ movhi
	bne	.L476
	ldr	r3, [r4, #3076]
	str	r3, [r6, #484]
	ldr	r3, [r4, #3088]
	str	r3, [r6, #492]
	ldr	r3, [r4, #3084]
	str	r3, [r6, #488]
	ldr	r3, [r4, #2704]
	ldr	r0, [r5, #4]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	cmp	r3, #2
	ldr	r3, [r4, #2708]
	ldr	r1, [r3, #4]
	beq	.L549
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L550
	ldr	r3, [r4, #2708]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r2, #3
	movne	r2, #1
	str	r2, [r6, #396]
	ldrsb	r2, [r3, #2]
	str	r2, [r6, #400]
	ldr	r2, [ip, #44]
	str	r2, [r6, #420]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r2, #3
	movne	r2, #1
	str	r2, [r6, #412]
	ldrsb	r3, [r3, #2]
	str	r3, [r6, #416]
	ldr	r3, [ip, #44]
	str	r3, [r6, #424]
	ldr	r3, [r4, #2712]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	ldrne	r3, [r4, #2712]
	beq	.L551
.L543:
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r2, #3
	movne	r2, #1
	str	r2, [r6, #404]
	ldrsb	r3, [r3, #2]
	str	r3, [r6, #408]
	ldr	r3, [ip, #44]
	str	r3, [r6, #428]
	ldr	r3, [r4, #2708]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L552
	ldr	r3, [ip, #96]
	str	r3, [r6, #300]
	ldr	r3, [r4, #2712]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L553
	ldr	r3, [ip, #96]
	str	r3, [r6, #304]
	ldr	r3, [r4, #2716]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L554
	ldr	r3, [ip, #96]
	str	r3, [r6, #308]
	ldr	r3, [r4, #2704]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L555
	ldr	r3, [ip, #96]
	str	r3, [r6, #312]
	ldr	r3, [ip, #44]
	str	r3, [r6, #316]
	ldr	r3, [ip, #60]
	str	r3, [r6, #432]
	ldr	r3, [r4, #2708]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r2, #3
	movne	r2, #1
	str	r2, [r6, #436]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	adds	r3, r3, #0
	movne	r3, #1
	str	r3, [r6, #440]
	ldr	r3, [r4, #2704]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetPmvAddr
	str	r0, [r6, #496]
	ldr	r3, [r4, #2708]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetPmvAddr
	add	ip, r5, #15424
	add	ip, ip, #44
	mov	r2, #0
	str	r0, [r6, #508]
	add	r0, r5, #16640
	ldr	r3, [r4, #3092]
	add	r0, r0, #252
	cmp	r3, #2
	movne	r1, #0
	moveq	r1, #4
	moveq	lr, #8
	moveq	r7, #16
	movne	r7, #32
	movne	lr, r1
.L497:
	add	r2, r2, #1
	ldr	r3, [ip, #4]!
	cmp	r2, r7
	ubfx	r3, r3, #0, #18
	str	r3, [r0, #4]!
	blt	.L497
	cmp	lr, #0
	ble	.L502
	add	ip, r5, #15552
	add	r0, r5, #16896
	add	ip, ip, #44
	add	r0, r0, #60
	mov	r2, #0
.L501:
	add	r2, r2, #1
	ldr	r3, [ip, #4]!
	cmp	r2, lr
	ubfx	r3, r3, #0, #15
	str	r3, [r0, #4]!
	bne	.L501
.L502:
	cmp	r1, #0
	ble	.L500
	add	r0, r5, #15616
	add	lr, r5, #16896
	add	r0, r0, #32
	add	lr, lr, #92
	mov	ip, #0
.L503:
	ldr	r2, [r0, #4]
	add	ip, ip, #1
	ldr	r3, [r0], #8
	cmp	ip, r1
	ubfx	r2, r2, #0, #9
	ubfx	r3, r3, #0, #9
	orr	r3, r3, r2, asl #9
	str	r3, [lr, #4]!
	bne	.L503
	add	r0, r5, #15616
	add	r5, r5, #16896
	add	r0, r0, #12
	add	r5, r5, #108
	mov	r2, #0
.L504:
	add	r2, r2, #1
	ldr	r3, [r0, #4]!
	cmp	r2, r1
	ubfx	r3, r3, #0, #18
	str	r3, [r5, #4]!
	bne	.L504
.L500:
	ldr	r3, [r4, #3140]
	mov	r7, #1
	mov	r0, r7
	str	r3, [r6, #640]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L544:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ble	.L556
.L455:
	mov	r7, #3
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L547:
	ldr	r1, .L559+12
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L545:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bne	.L467
.L541:
	mov	r0, r7
	bl	ResetVoQueue
	b	.L467
.L556:
	ldr	r3, [fp, #-52]
	cmp	r3, #32
	bgt	.L455
	ldr	r7, [r5, #16]
	cmp	r7, #0
	bne	.L455
	ldr	r3, [r4, #2740]
	cmp	r3, #0
	beq	.L454
	ldr	r1, [r4, #3492]
	cmp	r1, #0
	beq	.L454
	add	r6, r5, #16384
	ldr	r0, [r6, #772]
	cmp	r1, r0
	beq	.L454
	ldr	lr, [r4, #3496]
	ldr	ip, [r6, #776]
	cmp	lr, ip
	beq	.L454
	str	r1, [sp, #12]
	stmib	sp, {r0, lr}
	mov	r0, r2
	str	ip, [sp]
	ldr	r1, .L559+16
	bl	dprint_vfmw
	ldr	r2, .L559+20
	ldr	r3, [r4, #3492]
	ldr	ip, [r2]
	str	r3, [r6, #772]
	ldr	r3, [r4, #3496]
	cmp	ip, #0
	str	r3, [r6, #776]
	beq	.L454
	mov	r3, r7
	mov	r2, r7
	mov	r1, #120
	ldr	r0, [r5, #4]
	blx	ip
	b	.L454
.L546:
	ldr	r1, .L559+24
	mov	r0, #1
	mov	r7, ip
	bl	dprint_vfmw
	b	.L457
.L549:
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L557
	ldr	r3, [r4, #2708]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r2, #3
	movne	r2, #1
	str	r2, [r6, #396]
	ldrsb	r3, [r3, #2]
	str	r3, [r6, #400]
	ldr	r3, [ip, #44]
	str	r3, [r6, #420]
	ldr	r3, [r4, #2712]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L558
	ldr	r3, [r4, #2712]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r2, #3
	movne	r2, #1
	str	r2, [r6, #412]
	ldrsb	r3, [r3, #2]
	str	r3, [r6, #416]
	ldr	r3, [ip, #44]
	str	r3, [r6, #424]
	ldr	r3, [r4, #2716]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	ldrne	r3, [r4, #2716]
	bne	.L543
	movw	r2, #1979
	ldr	r1, .L559+28
	mov	r7, ip
	bl	dprint_vfmw
	b	.L457
.L539:
	ldr	r3, [r4, #2704]
	mov	r2, #1
	ldr	r0, [r5, #4]
	add	r6, r5, #14912
	add	r6, r6, #4
	ldr	r1, [r3, #4]
	bl	FSP_SetRef
	ldr	r3, [r4, #2716]
	add	r2, r6, #4
	cmp	r3, r2
	beq	.L474
	mov	r2, r7
	ldr	r1, [r3, #4]
	ldr	r0, [r5, #4]
	bl	FSP_SetRef
.L474:
	ldr	r1, [r4, #2712]
	ldr	r2, [r4, #2708]
	ldr	r3, [r4, #2704]
	str	r1, [r4, #2716]
	str	r2, [r4, #2712]
	str	r3, [r4, #2708]
	b	.L473
.L548:
	mov	r2, #1872
	ldr	r1, .L559+28
	mov	r7, ip
	bl	dprint_vfmw
	b	.L457
.L552:
	movw	r2, #2003
	ldr	r1, .L559+28
	mov	r7, ip
	bl	dprint_vfmw
	b	.L457
.L553:
	movw	r2, #2007
	ldr	r1, .L559+28
	mov	r7, ip
	bl	dprint_vfmw
	b	.L457
.L554:
	movw	r2, #2011
	ldr	r1, .L559+28
	mov	r7, ip
	bl	dprint_vfmw
	b	.L457
.L551:
	movw	r2, #1996
	ldr	r1, .L559+28
	mov	r7, ip
	bl	dprint_vfmw
	b	.L457
.L550:
	movw	r2, #1987
	ldr	r1, .L559+28
	mov	r7, ip
	bl	dprint_vfmw
	b	.L457
.L555:
	movw	r2, #2015
	ldr	r1, .L559+28
	mov	r7, ip
	bl	dprint_vfmw
	b	.L457
.L558:
	movw	r2, #1973
	ldr	r1, .L559+28
	mov	r7, ip
	bl	dprint_vfmw
	b	.L457
.L557:
	movw	r2, #1967
	ldr	r1, .L559+28
	mov	r7, ip
	bl	dprint_vfmw
	b	.L457
.L560:
	.align	2
.L559:
	.word	.LC99
	.word	.LC101
	.word	.LC104
	.word	.LC103
	.word	.LC100
	.word	g_event_report
	.word	.LC102
	.word	.LC97
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
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r6, r0, #16384
	ldr	r8, .L569
	add	r7, r0, #16128
	movw	r2, #17168
	ldr	r10, [r6, #88]
	mov	r9, r1
	ldr	r3, [r8, #48]
	mov	r1, #0
	mov	r4, r0
	add	r5, r0, #14912
	blx	r3
	str	r10, [r6, #88]
	mov	r0, r7
	add	r6, r4, #15680
	bl	ResetVoQueue
	str	r9, [r4, #12]
	mov	r0, r4
	add	r5, r5, #4
	bl	VCTRL_GetChanIDByCtx
	add	r6, r6, #44
	cmn	r0, #1
	str	r0, [r4, #4]
	beq	.L568
	ldr	r3, [r8, #48]
	mov	r2, #808
	mov	r1, #0
	mov	r0, r5
	blx	r3
	add	r3, r4, #12288
	add	r5, r5, #4
	mov	r1, #1024
	str	r5, [r3, #2704]
	mov	r2, #0
	str	r6, [r3, #3120]
	mov	r0, #1
	str	r5, [r3, #2716]
	str	r5, [r3, #2712]
	str	r5, [r3, #2708]
	str	r7, [r3, #3148]
	str	r1, [r3, #3144]
	ldr	r3, [r4, #12]
	str	r2, [r3, #872]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L568:
	ldr	r1, .L569+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L570:
	.align	2
.L569:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC105
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r4, r0, #0
	mov	r5, r1
	beq	.L574
	mov	r2, #0
	ldr	r0, [r4, #4]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #4]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	beq	.L575
	add	r5, r0, #576
	add	r8, r0, #560
	ldr	r1, [r0, #560]
	mov	r6, #0
	ldr	r0, [r4, #4]
	mov	r7, #0
	bl	FreeUsdByDec
	strd	r6, [r8]
	ldr	r0, [r4, #4]
	ldr	r1, [r5, #-8]
	bl	FreeUsdByDec
	strd	r6, [r5, #-8]
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L574:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L575:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	AVSDEC_RecycleImage, .-AVSDEC_RecycleImage
	.align	2
	.global	AVSDEC_GetRemainImg
	.type	AVSDEC_GetRemainImg, %function
AVSDEC_GetRemainImg:
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
	UNWIND(.pad #20)
	sub	sp, sp, #20
	ldr	r3, [r0, #12]
	mov	r4, r0
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L582
	add	r10, r0, #14976
	mov	r8, #0
	add	r10, r10, #32
	mov	r9, #0
	mov	r5, #3
	mvn	r6, #0
.L581:
	ldr	r3, [r10, #-4]!
	ldr	r0, [r4, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L578
	ldr	r3, [r10]
	ldr	r0, [r4, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetDisplay
	cmp	r0, #2
	movne	r6, #1
	beq	.L590
.L578:
	subs	r5, r5, #1
	bne	.L581
	cmp	r6, #0
	bne	.L582
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L582:
	add	r0, r4, #16128
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	GetVoLastImageID
.L590:
	mov	r0, r4
	bl	AVS_SetImgFormat
	cmp	r5, #1
	streq	r5, [r7, #188]
	mov	r2, #1
	ldr	r3, [r10]
	ldr	r0, [r4, #4]
	ldr	r1, [r3, #4]
	bl	FSP_SetDisplay
	ldr	r0, [r4, #4]
	add	r3, r4, #16128
	str	r7, [sp]
	mov	r2, r4
	mov	r1, #6
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L580
	ldr	r1, [r7, #156]
	mov	r0, r4
	add	r3, r7, #560
	str	r3, [fp, #-52]
	add	r3, r7, #576
	str	r3, [fp, #-48]
	bl	AVSDEC_RecycleImage
	ldr	r1, [r7, #560]
	ldr	r0, [r4, #4]
	mvn	r6, #0
	bl	FreeUsdByDec
	ldr	r7, [fp, #-48]
	ldr	r3, [fp, #-52]
	strd	r8, [r3]
	ldr	r1, [r7, #-8]
	ldr	r0, [r4, #4]
	bl	FreeUsdByDec
	strd	r8, [r7, #-8]
	b	.L578
.L580:
	ldr	r3, [r7, #160]
	cmp	r3, #100
	bhi	.L585
	ldr	r3, [r7, #112]
	cmp	r3, #31
	bhi	.L591
.L585:
	mvn	r6, #0
	b	.L578
.L591:
	ldr	r6, [r7, #116]
	cmp	r6, #31
	movhi	r6, #0
	mvnls	r6, #0
	b	.L578
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
	mov	r4, r0
	ldr	r0, [r0, #4]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #4]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L598
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L598:
	add	r1, r4, #16128
	ldr	r0, [r4, #4]
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
.L601:
	ldr	r3, [r4, #4]!
	cmp	r3, #0
	ble	.L600
	ldr	r3, [r6]
	cmp	r3, #0
	bne	.L606
.L600:
	cmp	r4, r8
	str	r5, [r4]
	str	r5, [r6], #4
	str	r5, [r7, #16]
	bne	.L601
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L606:
	ldr	r1, [r4, #12]
	mov	r0, r9
	bl	SM_ReleaseStreamSeg
	b	.L600
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
	add	r6, r0, #12288
	mov	r7, r0
	ldr	r3, [r6, #2164]
	cmp	r3, #0
	ble	.L610
	add	r5, r0, #116
	mov	r4, #0
.L609:
	mov	r1, r5
	ldr	r0, [r7, #4]
	bl	ClearSlice
	ldr	r3, [r6, #2164]
	add	r4, r4, #1
	add	r5, r5, #28
	cmp	r3, r4
	bgt	.L609
.L610:
	mov	r3, #0
	mov	r0, #1
	str	r3, [r6, #2164]
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
	ldr	r3, [r0, #12]
	mov	r6, r0
	add	r4, r0, #12288
	ldr	r3, [r3, #8]
	cmp	r3, #4
	ldrls	pc, [pc, r3, asl #2]
	b	.L628
.L616:
	.word	.L615
	.word	.L617
	.word	.L618
	.word	.L628
	.word	.L615
.L618:
	ldr	r5, [r4, #3812]
	clz	r5, r5
	mov	r5, r5, lsr #5
.L614:
	cmp	r5, #0
	str	r3, [r4, #3392]
	beq	.L634
.L621:
	ldr	r3, [r4, #3812]
	cmp	r3, #1
	beq	.L635
.L624:
	cmp	r3, #2
	bne	.L626
	ldr	r3, [r4, #3396]
	cmp	r3, #1
	ble	.L625
.L626:
	add	r1, r4, #3456
	add	r0, r4, #2736
	add	r1, r1, #12
	mov	r2, #372
	add	r0, r0, #12
	bl	memcpy
	ldrb	ip, [r4, #2825]	@ zero_extendqisi2
	ldr	r3, [r4, #3092]
	mov	r0, #18
	ldr	r2, [r4, #3408]
	ldr	r1, .L636
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r4, #2732]
	mov	r0, #1
	bic	r3, r3, #311296
	str	r3, [r4, #2732]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L617:
	ldr	r2, [r4, #3392]
	ldr	r5, [r4, #3812]
	cmp	r2, #2
	str	r3, [r4, #3392]
	moveq	r2, #0
	streq	r2, [r4, #3396]
	subs	r5, r5, #2
	movne	r5, #1
	cmp	r5, #0
	bne	.L621
.L634:
	mov	r0, r6
	bl	AVS_ClearCurPic
	ldr	r2, .L636+4
	ldr	r3, [r4, #2732]
	ldr	ip, [r2]
	bic	r3, r3, #311296
	str	r3, [r4, #2732]
	cmp	ip, #0
	beq	.L633
	ldr	r0, [r6, #4]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
	mov	r0, r5
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L615:
	ldr	r2, [r4, #3392]
	str	r3, [r4, #3392]
	cmp	r2, #2
	ldr	r3, [r4, #3812]
	moveq	r2, #0
	streq	r2, [r4, #3396]
	cmp	r3, #1
	bne	.L624
.L635:
	ldr	r3, [r4, #3396]
	cmp	r3, #0
	bgt	.L626
.L625:
	mov	r0, r6
	bl	AVS_ClearCurPic
	ldr	r2, .L636+4
	ldr	r3, [r4, #2732]
	ldr	ip, [r2]
	bic	r3, r3, #311296
	str	r3, [r4, #2732]
	cmp	ip, #0
	beq	.L633
	mov	r3, #0
	ldr	r0, [r6, #4]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L633:
	mov	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L628:
	mov	r5, #1
	b	.L614
.L637:
	.align	2
.L636:
	.word	.LC106
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
	ldr	r3, [r3, #2732]
	and	r3, r3, #376832
	cmp	r3, #376832
	beq	.L640
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L640:
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
	mov	r5, r0
	mov	r6, r1
	bl	AVS_ClearCurPic
	ldr	r3, .L700
	add	r4, r5, #12288
	add	r0, r5, #20
	movw	r2, #14896
	mov	r1, #0
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r3, [r4, #3092]
	cmp	r3, #0
	bne	.L642
	str	r6, [r4, #3400]
	ldr	r3, [r5, #12]
	ldr	r2, [r4, #3400]
	ldr	r3, [r3]
	cmp	r3, r2
	blt	.L691
.L647:
	mov	r0, r5
	bl	AVS_SetImgFormat
	ldr	r3, [r4, #2704]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	beq	.L692
	ldr	r1, [r4, #3412]
	mov	r2, #0
	cmp	r1, #0
	strne	r1, [r3, #560]
	movne	r1, #0
	strne	r1, [r3, #564]
	ldr	r1, [r4, #3416]
	str	r2, [r4, #3412]
	cmp	r1, r2
	strne	r2, [r3, #572]
	strne	r1, [r3, #568]
	mov	r2, #0
	ldr	r3, [r4, #3092]
	str	r2, [r4, #3416]
	cmp	r3, #1
	bls	.L693
.L652:
	ldr	r7, [r4, #2704]
.L657:
	ldr	r1, [r7, #4]
	ldr	r0, [r5, #4]
	bl	FSP_GetFsImagePtr
	subs	r6, r0, #0
	beq	.L694
	add	r3, r5, #14912
	add	r3, r3, #8
	cmp	r7, r3
	beq	.L662
	ldr	r3, [r5, #12]
	ldr	r2, [r4, #3400]
	ldr	r3, [r3, #4]
	cmp	r3, r2
	bge	.L660
	ldr	r1, .L700+4
	mov	r0, #1
	add	r4, r6, #560
	bl	dprint_vfmw
	ldr	r1, [r7, #4]
	mov	r2, #0
	ldr	r0, [r5, #4]
	bl	FSP_SetDisplay
	ldrd	r2, [r4]
	orrs	r1, r2, r3
	bne	.L695
.L661:
	add	r6, r6, #576
	ldrd	r2, [r6, #-8]
	orrs	r1, r2, r3
	bne	.L696
.L662:
	mov	r4, #1
.L645:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L660:
	mov	r0, r7
	mov	r1, r5
	bl	AVS_OutputOneFrame
	subs	r4, r0, #0
	bne	.L662
	add	r7, r6, #560
	ldrd	r2, [r7]
	orrs	r1, r2, r3
	bne	.L697
.L663:
	add	r6, r6, #576
	ldrd	r2, [r6, #-8]
	orrs	r1, r2, r3
	beq	.L645
	mov	r1, r2
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	mov	r2, #0
	mov	r3, #0
	strd	r2, [r6, #-8]
	b	.L645
.L642:
	ldr	r3, [r4, #2708]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	beq	.L698
	ldr	r2, [r4, #2712]
	ldr	r0, [r5, #4]
	ldr	r7, [r3, #160]
	ldr	r1, [r2, #4]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L699
	ldr	r1, [ip, #160]
	ldr	r2, [r4, #3092]
	add	r3, r7, r1
	add	r1, r6, r3
	cmp	r1, #100
	movge	r1, #100
	str	r1, [r4, #3400]
	ldr	r3, [r5, #12]
	cmp	r2, #2
	ldr	r3, [r3]
	beq	.L647
	ldr	r2, [r4, #3400]
	cmp	r3, r2
	bge	.L647
.L691:
	ldr	r1, .L700+8
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r4, #2708]
	mov	r2, #0
	str	r2, [r4, #3396]
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_GetFsImagePtr
	subs	r6, r0, #0
	beq	.L648
	ldr	r3, [r4, #2704]
	mov	r2, #1
	ldr	r0, [r5, #4]
	add	r8, r6, #576
	add	r7, r6, #592
	add	r9, r6, #560
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2708]
	mov	r2, #1
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2712]
	mov	r2, #1
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2716]
	mov	r2, #1
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_ClearLogicFs
	ldr	r1, [r6, #560]
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	ldr	r1, [r6, #568]
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	ldr	r1, [r6, #576]
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	ldr	r1, [r6, #584]
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	add	r3, r5, #14912
	mov	r0, #0
	mov	r1, #0
	add	r3, r3, #8
	strd	r0, [r9]
	strd	r0, [r8, #-8]
	strd	r0, [r8]
	strd	r0, [r7, #-8]
	str	r3, [r4, #2704]
	str	r3, [r4, #2716]
	str	r3, [r4, #2712]
	str	r3, [r4, #2708]
	ldr	r0, [r5, #4]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #2636]
.L648:
	mov	r3, #0
	str	r3, [r4, #3396]
	mov	r4, r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L693:
	ldr	r2, [r4, #3396]
	ldrb	r1, [r4, #2825]	@ zero_extendqisi2
	cmp	r2, #1
	addle	r2, r2, #1
	adds	r3, r3, #0
	str	r2, [r4, #3396]
	movne	r3, #1
	cmp	r1, #1
	orreq	r3, r3, #1
	cmp	r3, #0
	beq	.L654
	ldr	r3, [r4, #2704]
	mov	r2, #1
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #4]
	bl	FSP_SetRef
	add	r3, r5, #14912
	ldr	r2, [r4, #2716]
	add	r3, r3, #8
	cmp	r2, r3
	beq	.L655
	ldr	r1, [r2, #4]
	mov	r2, #0
	ldr	r0, [r5, #4]
	bl	FSP_SetRef
.L655:
	ldr	r1, [r4, #2712]
	ldr	r2, [r4, #2708]
	ldr	r3, [r4, #2704]
	str	r1, [r4, #2716]
	str	r2, [r4, #2712]
	str	r3, [r4, #2708]
.L654:
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	ldreq	r7, [r4, #2712]
	beq	.L657
	b	.L652
.L696:
	mov	r1, r2
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	mov	r4, #1
	mov	r2, #0
	mov	r3, #0
	strd	r2, [r6, #-8]
	b	.L645
.L695:
	mov	r1, r2
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	mov	r2, #0
	mov	r3, #0
	strd	r2, [r4]
	b	.L661
.L697:
	mov	r1, r2
	ldr	r0, [r5, #4]
	bl	FreeUsdByDec
	mov	r2, #0
	mov	r3, #0
	strd	r2, [r7]
	b	.L663
.L692:
	movw	r2, #3300
	ldr	r1, .L700+12
	mov	r4, r3
	bl	dprint_vfmw
	b	.L645
.L694:
	movw	r2, #3340
	ldr	r1, .L700+12
	mov	r4, r6
	bl	dprint_vfmw
	b	.L645
.L698:
	movw	r2, #3275
	ldr	r1, .L700+12
	mov	r4, r3
	bl	dprint_vfmw
	b	.L645
.L699:
	movw	r2, #3279
	ldr	r1, .L700+12
	mov	r4, ip
	bl	dprint_vfmw
	b	.L645
.L701:
	.align	2
.L700:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC108
	.word	.LC107
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
	mov	r5, r0
	ldr	r0, [r0, #4]
	ldr	r1, [r4, #120]
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r4, #109]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L703
	ldrb	r3, [r4, #189]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L707
.L703:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L707:
	ldr	r3, [r4, #192]
	cmp	r3, #0
	beq	.L703
	ldr	r1, [r4, #200]
	ldr	r0, [r5, #4]
	bl	SM_ReleaseStreamSeg
	b	.L703
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
	ldr	r2, [r5, #2164]
	cmp	r2, #512
	bge	.L716
	cmp	r2, #0
	ble	.L711
	add	lr, r0, #16384
	sub	r3, r2, #1
	ldr	r1, [r5, #3492]
	ldr	ip, [lr, #104]
	mov	r0, r3, asl #5
	add	r1, r1, #15
	sub	r3, r0, r3, asl #2
	add	r3, r4, r3
	ldrb	r0, [ip, #3]	@ zero_extendqisi2
	mov	r1, r1, lsr #4
	ldr	r3, [r3, #132]
	mul	r0, r0, r1
	cmp	r0, r3
	bls	.L717
.L712:
	mov	r0, r2, asl #5
	ldr	r3, [lr, #112]
	sub	r0, r0, r2, asl #2
	add	r0, r4, r0
	str	r3, [r0, #116]
	ldr	r3, [lr, #120]
	str	r3, [r0, #136]
	ldr	r3, [lr, #116]
	str	r3, [r0, #124]
	ldrb	r3, [ip, #3]	@ zero_extendqisi2
	mul	r1, r1, r3
	str	r1, [r0, #132]
	ldr	r3, [r5, #2772]
	ldr	r4, [r5, #2776]
	add	ip, r3, #15
	add	r4, r4, #15
	mov	r3, ip, lsr #4
	mov	r4, r4, lsr #4
	mul	r3, r3, r4
	sub	r3, r3, #1
	cmp	r1, r3
	bhi	.L715
	ldrb	r3, [lr, #109]	@ zero_extendqisi2
	add	r2, r2, #1
	cmp	r3, #1
	ldrne	r3, [lr, #192]
	moveq	r3, #0
	streq	r3, [r0, #120]
	streq	r3, [r0, #140]
	strne	r3, [r0, #120]
	ldrne	r3, [lr, #200]
	strne	r3, [r0, #140]
	ldrne	r3, [lr, #196]
	str	r3, [r0, #128]
	mov	r0, #1
	str	r2, [r5, #2164]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L715:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L711:
	ldr	r1, [r5, #3492]
	add	lr, r0, #16384
	add	r1, r1, #15
	ldr	ip, [lr, #104]
	mov	r1, r1, lsr #4
	b	.L712
.L716:
	mov	r3, #512
	ldr	r1, .L718
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L717:
	mov	r2, r0
	ldr	r1, .L718+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L719:
	.align	2
.L718:
	.word	.LC109
	.word	.LC110
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
	ldr	r0, [r0, #96]
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
	ldr	r8, [r0, #144]
	mov	r3, #0
	ldr	lr, [r0, #128]
	cmp	r8, #1
	str	r3, [fp, #-348]
	str	r3, [fp, #-344]
	str	r3, [fp, #-340]
	str	r3, [fp, #-336]
	str	r3, [fp, #-332]
	str	r3, [fp, #-328]
	beq	.L724
	clz	r3, lr
	cmp	r8, #3
	cmpeq	lr, #0
	mov	r3, r3, lsr #5
	beq	.L724
	cmp	r8, #2
	addeq	r3, r0, #172
	beq	.L726
	cmp	r3, #0
	bne	.L758
.L759:
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
	bne	.L808
	ldr	r3, .L814
	add	r2, r4, #10
	add	ip, r3, #12
.L740:
	ldrh	r1, [r3, #2]!
	cmp	r3, ip
	strh	r1, [r2, #2]!	@ movhi
	bne	.L740
.L733:
	add	ip, r0, #198
	add	lr, r0, #324
	add	lr, lr, #2
	mov	r2, #128
	mov	r3, ip
.L744:
	strh	r2, [r3, #2]!	@ movhi
	cmp	r3, lr
	bne	.L744
	ldr	r3, [r0, #124]
	cmp	r3, #0
	beq	.L761
	ldr	r10, [r0, #148]
	mov	r9, #0
	ldr	r3, .L814+4
	mov	r10, r10, asl #6
	add	r7, r10, #64
	add	r10, r3, r10
	add	r7, r3, r7
.L762:
	sub	r3, fp, #300
	mov	r5, r9, asl #3
	add	r6, r3, r9, lsl #7
	sub	r5, r5, r9, asl #1
	sub	r6, r6, #2
	mov	r2, r10
.L752:
	add	r0, r2, #8
	mov	r1, r6
.L750:
	ldrb	r3, [r2], #1	@ zero_extendqisi2
	add	r3, r5, r3
	cmp	r0, r2
	mov	r3, r3, asl #1
	ldrh	r3, [r4, r3]
	strh	r3, [r1, #2]!	@ movhi
	bne	.L750
	cmp	r0, r7
	add	r6, r6, #16
	mov	r2, r0
	bne	.L752
	add	r9, r9, #1
	cmp	r9, #2
	bne	.L762
.L748:
	cmp	r8, #0
	subeq	r3, fp, #174
	subeq	r1, fp, #46
	beq	.L753
	cmp	r8, #1
	beq	.L809
	cmp	r8, #2
	beq	.L810
.L721:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L724:
	ldr	r4, .L814+8
	add	r1, r0, #148
	sub	ip, fp, #348
	add	r3, r0, #172
.L723:
	ldr	r5, [r1, #4]!
	ldrh	r2, [r4], #2
	cmp	r1, r3
	add	r2, r2, r5
	strh	r2, [ip], #2	@ movhi
	bne	.L723
	cmp	r8, #2
	beq	.L726
	cmp	lr, #0
	cmpeq	r8, #3
	bne	.L727
.L726:
	ldr	ip, .L814
	sub	r1, fp, #336
	add	r5, r0, #196
.L728:
	ldr	r4, [r3, #4]!
	ldrh	r2, [ip, #2]!
	cmp	r3, r5
	add	r2, r2, r4
	strh	r2, [r1], #2	@ movhi
	bne	.L728
.L727:
	cmp	lr, #0
	bne	.L759
.L758:
	ldr	r3, .L814+8
	sub	r4, fp, #324
	add	ip, r3, #12
	mov	r5, r4
	mov	r2, r4
.L731:
	ldrh	r1, [r3], #2
	cmp	r3, ip
	strh	r1, [r2], #2	@ movhi
	bne	.L731
	ldr	r3, .L814
	add	lr, r4, #10
	add	ip, r3, #12
	mov	r2, lr
.L732:
	ldrh	r1, [r3, #2]!
	cmp	r3, ip
	strh	r1, [r2, #2]!	@ movhi
	bne	.L732
	cmp	r8, #0
	beq	.L733
	cmp	r8, #1
	beq	.L811
	cmp	r8, #2
	beq	.L812
	cmp	r8, #3
	bne	.L733
	sub	r3, fp, #348
	sub	r1, fp, #336
.L738:
	ldrh	r2, [r3], #2
	cmp	r3, r1
	strh	r2, [r5], #2	@ movhi
	bne	.L738
	add	r1, r4, #22
.L739:
	ldrh	r2, [r3], #2
	strh	r2, [lr, #2]!	@ movhi
	cmp	lr, r1
	bne	.L739
	b	.L733
.L753:
	ldrh	r2, [r3, #2]!
	cmp	r3, r1
	strh	r2, [ip, #2]!	@ movhi
	beq	.L721
	ldrh	r2, [r3, #2]!
	cmp	r3, r1
	strh	r2, [ip, #2]!	@ movhi
	bne	.L753
	b	.L721
.L761:
	sub	r0, fp, #174
	add	r4, fp, #82
	mov	r1, #128
.L763:
	sub	r3, r0, #128
.L749:
	add	r2, r3, #16
.L746:
	strh	r1, [r3, #2]!	@ movhi
	cmp	r2, r3
	bne	.L746
	cmp	r2, r0
	mov	r3, r2
	bne	.L749
	add	r0, r0, #128
	cmp	r0, r4
	bne	.L763
	b	.L748
.L808:
	cmp	r8, #1
	beq	.L813
	cmp	r8, #2
	bne	.L733
	sub	r2, fp, #336
	add	r3, r4, #10
	add	ip, r4, #22
.L743:
	ldrh	r1, [r2], #2
	strh	r1, [r3, #2]!	@ movhi
	cmp	r3, ip
	bne	.L743
	b	.L733
.L813:
	sub	r3, fp, #348
	sub	ip, fp, #336
	mov	r2, r4
.L742:
	ldrh	r1, [r3], #2
	cmp	r3, ip
	strh	r1, [r2], #2	@ movhi
	bne	.L742
	b	.L733
.L811:
	sub	r3, fp, #348
	sub	r1, fp, #336
.L735:
	ldrh	r2, [r3], #2
	cmp	r3, r1
	strh	r2, [r5], #2	@ movhi
	bne	.L735
	b	.L733
.L810:
	sub	r3, fp, #174
	sub	r1, fp, #46
.L757:
	ldrh	r2, [r3, #2]!
	cmp	r3, r1
	strh	r2, [ip, #2]!	@ movhi
	bne	.L757
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L809:
	sub	r3, fp, #300
	sub	r3, r3, #2
.L755:
	ldrh	r2, [r3, #2]!
	strh	r2, [ip, #2]!	@ movhi
	cmp	lr, ip
	bne	.L755
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L812:
	sub	r3, fp, #336
	add	r1, r4, #22
.L737:
	ldrh	r2, [r3], #2
	strh	r2, [lr, #2]!	@ movhi
	cmp	lr, r1
	bne	.L737
	b	.L733
.L815:
	.align	2
.L814:
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
	add	r9, r0, #14912
	ldr	r6, [r7, #3120]
	add	r5, r0, #15744
	str	r3, [r8, #96]
	mov	r4, r0
	mov	r1, #32
	add	r9, r9, #4
	mov	r0, r6
	add	r5, r5, #12
	bl	BsShow
	uxtb	r0, r0
	sub	r3, r0, #176
	cmp	r3, #78
	ldrls	pc, [pc, r3, asl #2]
	b	.L817
.L819:
	.word	.L818
	.word	.L820
	.word	.L821
	.word	.L822
	.word	.L841
	.word	.L824
	.word	.L825
	.word	.L820
	.word	.L841
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L817
	.word	.L827
.L820:
	ldr	r3, [r7, #2732]
	ands	r5, r3, #32768
	bne	.L857
	tst	r3, #327680
	bne	.L865
.L860:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L817:
	cmp	r0, #175
	bhi	.L841
	ldr	r3, [r7, #2732]
	and	r2, r3, #327680
	cmp	r2, #327680
	beq	.L866
.L841:
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L824:
	ldr	r3, [r7, #2732]
	tst	r3, #327680
	beq	.L836
	mov	r1, r5
	mov	r0, r9
	bl	AVS_extension
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L821:
	ldr	r3, [r7, #2732]
	ands	r1, r3, #262144
	movne	r1, #1
	bne	.L859
	tst	r3, #65536
	beq	.L836
.L859:
	mov	r0, r4
	bl	AVS_user_data
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L822:
	ldr	r3, [r7, #2732]
	ands	r6, r3, #32768
	bne	.L858
	tst	r3, #262144
	beq	.L832
	bic	r3, r3, #311296
	mov	r0, r9
	str	r3, [r7, #2732]
	mov	r1, r5
	bl	AVS_i_picture_header
	b	.L862
.L825:
	ldr	r3, [r7, #2732]
	ands	r6, r3, #32768
	bne	.L858
	tst	r3, #262144
	beq	.L834
	bic	r3, r3, #311296
	mov	r0, r9
	str	r3, [r7, #2732]
	mov	r1, r5
	bl	AVS_pb_picture_header
.L862:
	mov	r0, r5
	bl	Avs_Vfmw_WQMatrix
	mov	r0, r4
	bl	ClearPacket
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L827:
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
	bne	.L836
	mov	r5, #1
	strb	r5, [r4]
	str	r5, [r8, #96]
	mov	r0, r4
	ldr	r3, [r7, #2732]
	orr	r3, r3, #16384
	str	r3, [r7, #2732]
	bl	ClearPacket
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L818:
	ldr	r3, [r7, #2732]
	ands	r6, r3, #32768
	bne	.L857
	tst	r3, #65536
	beq	.L830
	bic	r3, r3, #311296
	mov	r1, r5
	str	r3, [r7, #2732]
	mov	r0, r4
	bl	AVS_sequence_header
	mov	r0, r4
	bl	ClearPacket
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L857:
	mov	r0, #1
	orr	r3, r3, #16384
	str	r0, [r8, #96]
	str	r3, [r7, #2732]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L866:
	orr	r3, r3, #32768
	mov	r0, r4
	str	r3, [r7, #2732]
	bl	WriteAvsSliceMsg
	cmp	r0, #0
	bne	.L860
.L836:
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L834:
	mov	r0, r9
	mov	r1, r5
	bl	AVS_pb_picture_header
.L861:
	mov	r0, r5
	bl	Avs_Vfmw_WQMatrix
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L832:
	mov	r0, r9
	mov	r1, r5
	bl	AVS_i_picture_header
	b	.L861
.L830:
	mov	r1, r5
	mov	r0, r4
	bl	AVS_sequence_header
	mov	r0, r4
	bl	ClearPacket
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L858:
	orr	r3, r3, #16384
	mov	r0, r5
	mov	r4, #1
	str	r4, [r8, #96]
	str	r3, [r7, #2732]
	bl	Avs_Vfmw_WQMatrix
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L865:
	bic	r3, r3, #311296
	mov	r0, r4
	str	r3, [r7, #2732]
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
	mov	r4, r0
	add	r0, r0, #15680
	mov	r5, r1
	ldr	r2, [r1, #12]
	add	r0, r0, #44
	ldr	r1, [r1]
	bl	BsInit
	ldr	r3, .L882
	add	r6, r4, #12288
	add	r0, r4, #16384
	mov	r1, r5
	mov	r2, #160
	ldr	r3, [r3, #52]
	add	r0, r0, #104
	blx	r3
	ldr	r3, [r6, #2732]
	mov	r0, r4
	str	r3, [r6, #3432]
	bl	AVS_ParseHeader
	subs	r5, r0, #0
	beq	.L880
	ldr	r3, [r6, #2732]
	and	r3, r3, #376832
	cmp	r3, #376832
	beq	.L870
.L871:
	mov	r5, #0
.L869:
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L880:
	ldr	r1, .L882+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	AVS_ClearCurPic
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L870:
	mov	r0, r4
	bl	AVS_DecodeArbiter.part.3
	cmp	r0, #0
	beq	.L871
	mov	r0, r4
	bl	AVS_GetStreamInf
	cmp	r0, #3
	beq	.L872
	adds	r5, r0, #0
	movne	r5, #1
	cmp	r5, #0
	beq	.L881
	mov	r0, r4
	bl	AVS_StartDecode
	subs	r6, r0, #0
	bne	.L869
	ldr	r1, .L882+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	AVS_ClearCurPic
	mov	r5, r6
	b	.L869
.L881:
	ldr	r1, .L882+12
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	AVS_ClearCurPic
	b	.L869
.L872:
	ldr	r3, [r6, #3432]
	mov	r5, r0
	str	r3, [r6, #2732]
	b	.L869
.L883:
	.align	2
.L882:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC111
	.word	.LC113
	.word	.LC112
	UNWIND(.fnend)
	.size	AVSDEC_Decode, .-AVSDEC_Decode
	.global	g_tmp
	.global	wq_param_default
	.global	g_AvsCfg
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	CSWTCH.54, %object
	.size	CSWTCH.54, 16
CSWTCH.54:
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
	ASCII(.ascii	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d" )
	ASCII(.ascii	",new w/h = %d/%d\012\000" )
	.space	3
.LC101:
	ASCII(.ascii	"MbWidth = %d, MbHeight = %d, don't support!\012\000" )
	.space	3
.LC102:
	ASCII(.ascii	"ERROR: allocate vhb\012\000" )
	.space	3
.LC103:
	ASCII(.ascii	"ERROR: get img buf err\012\000" )
.LC104:
	ASCII(.ascii	"dec_pts: %lld Usertag = %lld\012\000" )
	.space	2
.LC105:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC106:
	ASCII(.ascii	"-----Pic %5d, type %d, struct %d-----\012\000" )
	.space	1
.LC107:
	ASCII(.ascii	"ERROR: err_level(%d) over ref_thr(%d)\012\000" )
	.space	1
.LC108:
	ASCII(.ascii	"ERROR: err_level(%d) over out_thr(%d)\012\000" )
	.space	1
.LC109:
	ASCII(.ascii	"SlcNum %d > %d Write slice msg failed\012\000" )
	.space	1
.LC110:
	ASCII(.ascii	"This slice's start mbn %d <= LastSliceFirstMb %d, W" )
	ASCII(.ascii	"rite slice msg failed\012\000" )
	.space	2
.LC111:
	ASCII(.ascii	"ERROR: paseHeader err\012\000" )
	.space	1
.LC112:
	ASCII(.ascii	"ERROR: AVS_PrepareDecode err\012\000" )
	.space	2
.LC113:
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
