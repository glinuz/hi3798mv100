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
	.file	"syntax.c"
	.text
	.align	2
	.global	InitVoQueue
	.type	InitVoQueue, %function
InitVoQueue:
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
	ldr	r1, .L2
	mov	r4, r0
	mov	r0, #12
	bl	dprint_vfmw
	mov	r2, #0
	mov	r3, #64
	strh	r2, [r4, #2]	@ movhi
	strh	r3, [r4]	@ movhi
	mov	r0, #1
	mvn	r3, #0
	strh	r2, [r4, #4]	@ movhi
	strh	r2, [r4, #6]	@ movhi
	strh	r2, [r4, #8]	@ movhi
	str	r2, [r4, #344]
	str	r2, [r4, #316]
	str	r3, [r4, #272]
	str	r3, [r4, #292]
	str	r3, [r4, #288]
	str	r3, [r4, #284]
	str	r3, [r4, #276]
	str	r3, [r4, #280]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3:
	.align	2
.L2:
	.word	.LC0
	UNWIND(.fnend)
	.size	InitVoQueue, .-InitVoQueue
	.align	2
	.global	PrintVoQueue
	.type	PrintVoQueue, %function
PrintVoQueue:
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
	UNWIND(.pad #8)
	sub	sp, sp, #8
	mov	r5, r0
	ldrh	ip, [r5, #8]
	ldrh	r3, [r0, #4]
	mov	r0, #1
	ldrh	r2, [r5, #6]
	ldr	r1, .L10
	str	ip, [sp]
	bl	dprint_vfmw
	ldrh	r3, [r5, #4]
	ldrh	r2, [r5, #8]
	and	r4, r3, #63
	cmp	r2, r4
	bne	.L8
	b	.L7
.L6:
	add	r3, r5, r4, lsl #2
	ldr	r1, .L10+4
	mov	r0, #1
	ldr	r3, [r3, #12]
	ldr	r2, [r3, #156]
	bl	dprint_vfmw
	add	r3, r4, #1
	ldrh	r2, [r5, #8]
	and	r4, r3, #63
	cmp	r2, r4
	beq	.L7
.L8:
	tst	r3, #7
	bne	.L6
	ldr	r1, .L10+8
	mov	r0, #1
	bl	dprint_vfmw
	b	.L6
.L7:
	ldr	r1, .L10+8
	mov	r0, #1
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L11:
	.align	2
.L10:
	.word	.LC1
	.word	.LC3
	.word	.LC2
	UNWIND(.fnend)
	.size	PrintVoQueue, .-PrintVoQueue
	.align	2
	.global	ResetVoQueue
	.type	ResetVoQueue, %function
ResetVoQueue:
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
	ldr	r5, .L17
	mov	r4, r0
	mov	r0, #2
	ldr	r3, [r5, #12]
	blx	r3
	ldrh	r2, [r4, #6]
	ldrh	r3, [r4, #4]
	ldrh	r1, [r4, #8]
	cmp	r2, r3
	beq	.L16
.L14:
	str	r1, [sp]
	mov	r0, #1
	ldr	r1, .L17+4
	bl	dprint_vfmw
.L15:
	ldr	r2, [r4, #344]
	add	r1, r4, #336
	mov	r3, #0
	mov	r0, #64
	strh	r3, [r4, #2]	@ movhi
	add	r2, r2, #100
	strh	r0, [r4]	@ movhi
	mvn	r8, #0
	str	r2, [r4, #344]
	mvn	r9, #0
	mvn	r2, #0
	strh	r3, [r4, #4]	@ movhi
	strh	r3, [r4, #6]	@ movhi
	mov	r6, #0
	strh	r3, [r4, #8]	@ movhi
	mov	r7, #0
	str	r3, [r4, #316]
	mov	r0, #2
	str	r2, [r4, #272]
	str	r2, [r4, #292]
	str	r2, [r4, #288]
	str	r2, [r4, #284]
	str	r2, [r4, #276]
	str	r2, [r4, #280]
	ldr	ip, [r5, #16]
	strd	r8, [r1, #-8]
	str	r3, [r4, #324]
	strd	r6, [r1]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	ip
.L16:
	cmp	r2, r1
	bne	.L14
	b	.L15
.L18:
	.align	2
.L17:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC4
	UNWIND(.fnend)
	.size	ResetVoQueue, .-ResetVoQueue
	.align	2
	.global	ReturnImageToSyntax
	.type	ReturnImageToSyntax, %function
ReturnImageToSyntax:
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
	mov	r3, r1
	cmp	r0, #20
	ldrls	pc, [pc, r0, asl #2]
	b	.L19
.L22:
	.word	.L21
	.word	.L23
	.word	.L24
	.word	.L25
	.word	.L19
	.word	.L26
	.word	.L27
	.word	.L19
	.word	.L28
	.word	.L29
	.word	.L30
	.word	.L30
	.word	.L30
	.word	.L31
	.word	.L19
	.word	.L32
	.word	.L33
	.word	.L34
	.word	.L19
	.word	.L19
	.word	.L35
.L35:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	USERDEC_RecycleImage
.L21:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_RecycleImage
.L23:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VC1DEC_RecycleImage
.L24:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_RecycleImage
.L25:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG2DEC_RecycleImage
.L26:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	DIVX3DEC_RecycleImage
.L27:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	AVSDEC_RecycleImage
.L28:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL8DEC_RecycleImage
.L29:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL9DEC_RecycleImage
.L30:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP6DEC_RecycleImage
.L31:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP8DEC_RecycleImage
.L32:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MVC_DEC_RecycleImage
.L33:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVC_DECRecycleImage
.L34:
	ldr	r1, [r2, #156]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP9DEC_RecycleImage
.L19:
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	ReturnImageToSyntax, .-ReturnImageToSyntax
	.align	2
	.global	ProduceTimeStamp
	.type	ProduceTimeStamp, %function
ProduceTimeStamp:
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
	mov	r3, r1
	cmp	r2, #20
	add	r1, r1, #608
	ldrd	r6, [r1, #-8]
	beq	.L37
	add	r0, r0, #272
	sub	r2, r2, #8
	ldrd	r4, [r0, #56]
	subs	r0, r6, r4
	and	r4, r4, r5
	sbc	r1, r7, r5
	cmp	r1, #0
	cmpeq	r0, #5
	movls	r1, #1
	movhi	r1, #0
	adds	r4, r4, #1
	movne	r4, #1
	tst	r1, r4
	beq	.L47
	cmp	r2, #1
	mvnhi	r6, #0
	mvnhi	r7, #0
	bls	.L39
.L37:
	mov	r0, r6
	mov	r1, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L47:
	cmp	r2, #1
	bhi	.L37
	ldr	r2, [r3, #108]
	and	r2, r2, #3
	cmp	r2, #2
	bne	.L37
.L39:
	mvn	r1, #0
	str	r1, [r3, #592]
	mov	r6, r1
	mov	r7, r1
	str	r1, [r3, #596]
	b	.L37
	UNWIND(.fnend)
	.size	ProduceTimeStamp, .-ProduceTimeStamp
	.global	__aeabi_uidiv
	.align	2
	.global	InsertImgToVoQueue
	.type	InsertImgToVoQueue, %function
InsertImgToVoQueue:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	ldr	r5, [fp, #4]
	str	r0, [fp, #-56]
	mov	r8, r1
	cmp	r5, #0
	mov	r10, r2
	mov	r4, r3
	beq	.L111
	ldr	r1, .L117
	mov	r0, #2
	bl	dprint_vfmw
	ldr	r3, .L117+4
	mov	r0, #2
	ldr	r3, [r3, #12]
	blx	r3
	ldrh	r9, [r4, #4]
	ldrh	r3, [r4, #8]
	and	r9, r9, #63
	cmp	r3, r9
	ldrne	r7, [fp, #-56]
	bne	.L56
	b	.L55
.L54:
	add	r9, r9, #1
	ldrh	r3, [r4, #8]
	and	r9, r9, #63
	cmp	r3, r9
	beq	.L112
.L56:
	add	r3, r4, r9, lsl #2
	ldr	r1, [r5, #156]
	ldr	r3, [r3, #12]
	ldr	r2, [r3, #156]
	cmp	r2, r1
	bne	.L54
	ldr	r1, .L117+8
	mov	r0, #0
	bl	dprint_vfmw
	mov	r3, #1000
	str	r3, [r5, #160]
	b	.L54
.L112:
	str	r7, [fp, #-56]
.L55:
	ldr	r2, [r5, #160]
	cmp	r2, #100
	bhi	.L52
	ldr	r3, [r5, #112]
	cmp	r3, #31
	bhi	.L113
.L52:
	ldr	r3, [r5, #112]
	ldr	r1, [r5, #116]
.L57:
	str	r1, [sp]
	mov	r0, #0
	ldr	r1, .L117+12
	bl	dprint_vfmw
	ldr	r3, [r5, #160]
	cmp	r3, #1000
	beq	.L59
	mov	r2, r5
	mov	r1, r10
	mov	r0, r8
	bl	ReturnImageToSyntax
.L59:
	ldr	r3, .L117+4
	mov	r0, #2
	ldr	r3, [r3, #16]
	blx	r3
	mov	r0, #1
.L50:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L113:
	ldr	r1, [r5, #116]
	cmp	r1, #31
	bls	.L57
	add	r10, r5, #608
	add	r9, r5, #592
	mov	r2, r8
	mov	r1, r5
	ldrd	r6, [r10, #-8]
	mov	r0, r4
	strd	r6, [r9]
	bl	ProduceTimeStamp
	mov	r6, r0
	mov	r7, r1
	ldrd	r0, [r10]
	strd	r6, [r10, #-8]
	ldrd	r2, [r9]
	strd	r0, [fp, #-68]
	mov	r0, #29
	strd	r6, [sp]
	ldrd	r6, [fp, #-68]
	ldr	r1, .L117+16
	strd	r6, [sp, #8]
	bl	dprint_vfmw
	ldr	r3, [r5, #120]
	cmp	r3, #0
	beq	.L60
	ldr	r3, [r5, #124]
	cmp	r3, #0
	bne	.L114
.L60:
	ldr	r2, [r5, #112]
	ldr	r3, [r5, #116]
	mov	r1, r2, lsr #1
	str	r2, [r5, #120]
	str	r3, [r5, #124]
	mov	r3, r3, lsr #1
	str	r1, [r5, #128]
	str	r3, [r5, #132]
.L61:
	str	r2, [r4, #300]
	ldr	r3, [r5, #116]
	ldr	r2, [r4, #280]
	str	r3, [r4, #304]
	ldr	r3, [r5, #108]
	ubfx	r3, r3, #5, #3
	str	r3, [r4, #308]
	ldr	r3, [r5, #108]
	str	r3, [r4, #312]
	ldr	r3, [r5, #164]
	ldr	r1, [r5, #128]
	cmp	r3, r2
	ldrh	r2, [r5, #132]
	ldr	r8, [r5, #108]
	ldr	r9, [r5, #124]
	orr	r2, r2, r1, asl #16
	ldrh	r0, [r5, #120]
	ubfx	r10, r8, #14, #3
	strne	r3, [r4, #320]
	mov	r7, r2
	ldr	r3, [r4, #316]
	orr	r9, r0, r9, asl #16
	ubfx	r8, r8, #8, #2
	add	r2, r3, #1
	str	r2, [r4, #316]
	str	r3, [r5, #168]
	ldr	r3, [r4, #272]
	cmp	r3, r10
	ldreq	r3, .L117+20
	moveq	r6, r3
	beq	.L63
	ldr	r2, .L117+20
	ldr	ip, [r2]
	mov	r6, r2
	cmp	ip, #0
	beq	.L64
	strb	r3, [fp, #-52]
	sub	r2, fp, #52
	mov	r3, #2
	strb	r10, [fp, #-51]
	mov	r1, #7
	ldr	r0, [fp, #-56]
	blx	ip
.L64:
	str	r10, [r4, #272]
.L63:
	ldr	r3, [r4, #276]
	cmp	r3, r8
	beq	.L65
	ldr	ip, [r6]
	cmp	ip, #0
	beq	.L66
	sub	r2, fp, #44
	mov	r3, #1
	mov	r1, #4
	ldr	r0, [fp, #-56]
	strb	r8, [r2, #-8]!
	blx	ip
.L66:
	str	r8, [r4, #276]
.L65:
	ldr	r3, [r4, #288]
	cmp	r3, r9
	beq	.L115
.L67:
	ldr	ip, [r6]
	str	r9, [r4, #288]
	cmp	ip, #0
	str	r7, [r4, #292]
	beq	.L68
	ldr	r9, [r5, #120]
	mov	r3, #8
	ldr	r8, [r5, #124]
	sub	r2, fp, #52
	ldr	lr, [r5, #128]
	mov	r1, #1
	ldr	r10, [r5, #132]
	ldr	r0, [fp, #-56]
	strh	r9, [fp, #-52]	@ movhi
	strh	r8, [fp, #-50]	@ movhi
	strh	lr, [fp, #-48]	@ movhi
	strh	r10, [fp, #-46]	@ movhi
	blx	ip
.L68:
	ldr	r1, [r5, #48]
	mov	r0, #12
	ldr	r2, [r5, #44]
	mov	r9, #300
	ldr	r3, [r5, #140]
	str	r1, [r5, #32]
	str	r2, [r5, #28]
	add	r2, r3, r2
	add	r3, r1, r3
	str	r2, [r5, #36]
	str	r3, [r5, #40]
	ldr	r1, [r5, #156]
	ldrh	ip, [r4, #8]
	ldrh	r3, [r4, #6]
	ldrh	r2, [r4, #4]
	str	ip, [sp]
	str	r1, [sp, #4]
	ldr	r1, .L117+24
	bl	dprint_vfmw
	ldr	r8, .L117+28
	ldr	r7, [fp, #-56]
	mov	r2, #0
	ldrh	r3, [r4, #8]
	mov	r1, #5
	ldr	r10, .L117+32
	mla	r9, r9, r7, r8
	add	r0, r4, r3, lsl #2
	add	r3, r3, #1
	str	r5, [r0, #12]
	and	r3, r3, #63
	strh	r3, [r4, #8]	@ movhi
	mov	r0, r7
	ldr	r3, [r5, #96]
	str	r2, [r5, #348]
	ldr	r2, [r10, r7, asl #2]
	str	r3, [r9, #20]
	ldr	r3, [r5, #156]
	add	r3, r3, #2
	str	r3, [r4, #296]
	bl	VDEC_Lowdelay_Event_Time
	ldr	r2, .L117+4
	ldr	r3, [r10, r7, asl #2]
	mov	r0, #2
	add	r3, r3, #1
	ldr	r2, [r2, #16]
	str	r3, [r10, r7, asl #2]
	blx	r2
	ldr	r3, .L117+36
	mov	r2, r7
	ldr	r3, [r3, r7, asl #2]
	ldr	r3, [r3, #144]
	cmp	r3, #0
	beq	.L70
	ldr	r3, [r9, #8]
	cmp	r3, #0
	movne	r2, #100
	mulne	r3, r2, r3
	strne	r3, [r5, #136]
.L70:
	ldr	ip, [r6]
	cmp	ip, #0
	beq	.L71
	mov	r2, r5
	mov	r3, #624
	mov	r1, #5
	ldr	r0, [fp, #-56]
	blx	ip
.L71:
	mov	r0, #8
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	bne	.L116
.L73:
	ldr	r3, .L117+4
	mov	r4, #300
	ldr	r2, [r3]
	ldr	r3, [fp, #-56]
	mul	r4, r4, r3
	add	r5, r8, r4
	ldr	r3, [r5, #4]
	add	r3, r3, #1
	str	r3, [r5, #4]
	blx	r2
	ldr	r1, [r8, r4]
	rsb	r1, r1, r0
	mov	r6, r0
	cmp	r1, #300
	movcc	r0, #1
	bcc	.L50
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq	.L75
	movw	r0, #10000
	mul	r0, r0, r3
	bl	__aeabi_uidiv
	str	r0, [r5, #8]
.L75:
	ldr	r1, [fp, #-56]
	mov	r3, #300
	mov	r2, #0
	mov	r0, #1
	mul	r7, r3, r1
	add	r3, r8, r7
	str	r6, [r8, r7]
	str	r2, [r3, #4]
	b	.L50
.L114:
	sub	r3, r8, #15
	cmp	r8, #0
	cmpne	r3, #1
	ldrls	r2, [r5, #112]
	bhi	.L60
	b	.L61
.L116:
	ldr	r3, .L117+4
	ldr	r3, [r3]
	blx	r3
	ldr	ip, .L117+40
	ldr	r3, [fp, #-56]
	ldr	r2, .L117+44
	ldr	r3, [ip, r3, asl #2]
	ldr	r2, [r2]
	rsb	r3, r3, r0
	mov	lr, r0
	cmp	r3, r2
	bls	.L73
	ldr	r3, [fp, #-56]
	mov	r1, #636
	ldr	r0, .L117+48
	mov	r2, r3, asl #24
	str	lr, [ip, r3, asl #2]
	add	r2, r2, #524288
	bl	DBG_AddTrace
	b	.L73
.L115:
	ldr	r3, [r4, #292]
	cmp	r3, r7
	bne	.L67
	b	.L68
.L111:
	mov	r0, r5
	ldr	r1, .L117+52
	bl	dprint_vfmw
	mvn	r0, #1
	b	.L50
.L118:
	.align	2
.L117:
	.word	.LC6
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	g_event_report
	.word	.LC10
	.word	g_VfmwChanStat
	.word	g_LowDelaySeqIndex
	.word	s_pstVfmwChan
	.word	.LANCHOR0
	.word	g_TraceFramePeriod
	.word	.LANCHOR1
	.word	.LC5
	UNWIND(.fnend)
	.size	InsertImgToVoQueue, .-InsertImgToVoQueue
	.align	2
	.global	GetQueueImgNum
	.type	GetQueueImgNum, %function
GetQueueImgNum:
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
	ldrh	r3, [r0, #6]
	ldrh	ip, [r0, #4]
	ldrh	r0, [r0, #8]
	cmp	r3, ip
	addcc	lr, r3, #64
	rsbcs	ip, ip, r3
	rsbcc	ip, ip, lr
	cmp	r3, r0
	addhi	r0, r0, #64
	str	ip, [r1]
	rsb	r3, r3, r0
	str	r3, [r2]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	GetQueueImgNum, .-GetQueueImgNum
	.align	2
	.global	SYNTAX_ReceiveImage
	.type	SYNTAX_ReceiveImage, %function
SYNTAX_ReceiveImage:
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
	cmp	r0, #0
	cmpne	r1, #0
	mov	r7, r1
	beq	.L145
	ldr	r3, [r0]
	cmp	r3, #20
	ldrls	pc, [pc, r3, asl #2]
	b	.L145
.L128:
	.word	.L141
	.word	.L129
	.word	.L130
	.word	.L131
	.word	.L145
	.word	.L132
	.word	.L133
	.word	.L145
	.word	.L134
	.word	.L135
	.word	.L136
	.word	.L136
	.word	.L136
	.word	.L137
	.word	.L145
	.word	.L138
	.word	.L139
	.word	.L140
	.word	.L145
	.word	.L145
	.word	.L141
.L141:
	add	r4, r0, #16
.L142:
	ldr	r6, .L147
	mov	r0, #2
	ldr	r3, [r6, #12]
	blx	r3
	ldrh	r3, [r4, #6]
	ldrh	r2, [r4, #8]
	cmp	r2, r3
	mvneq	r5, #2
	beq	.L143
	add	r3, r4, r3, lsl #2
	ldr	ip, [r6, #52]
	mov	r2, #624
	mov	r0, r7
	ldr	r1, [r3, #12]
	mov	r5, #1
	blx	ip
	ldr	r1, [r7, #156]
	ldrh	r3, [r4, #6]
	mov	r0, #12
	ldrh	r2, [r4, #4]
	str	r1, [sp, #4]
	ldrh	ip, [r4, #8]
	ldr	r1, .L147+4
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r7, #156]
	ldr	r2, [r4, #344]
	add	r3, r3, r2
	str	r3, [r7, #156]
	ldrh	r3, [r4, #6]
	add	r3, r3, r5
	and	r3, r3, #63
	strh	r3, [r4, #6]	@ movhi
.L143:
	ldr	r3, [r6, #16]
	mov	r0, #2
	blx	r3
	mov	r0, r5
.L126:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L129:
	add	r4, r0, #49152
	add	r4, r4, #72
	b	.L142
.L130:
	add	r4, r0, #22528
	add	r4, r4, #112
	b	.L142
.L131:
	add	r4, r0, #87040
	add	r4, r4, #680
	b	.L142
.L132:
	add	r4, r0, #41472
	add	r4, r4, #40
	b	.L142
.L133:
	add	r4, r0, #16128
	add	r4, r4, #8
	b	.L142
.L134:
	add	r4, r0, #53760
	add	r4, r4, #80
	b	.L142
.L135:
	add	r4, r0, #53760
	add	r4, r4, #64
	b	.L142
.L136:
	add	r4, r0, #12672
	add	r4, r4, #8
	b	.L142
.L137:
	add	r4, r0, #8192
	b	.L142
.L138:
	add	r4, r0, #592
	b	.L142
.L139:
	add	r4, r0, #61696
	add	r4, r4, #72
	b	.L142
.L140:
	add	r4, r0, #270336
	add	r4, r4, #872
	b	.L142
.L145:
	mvn	r0, #1
	b	.L126
.L148:
	.align	2
.L147:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC11
	UNWIND(.fnend)
	.size	SYNTAX_ReceiveImage, .-SYNTAX_ReceiveImage
	.align	2
	.global	SYNTAX_ReleaseImage
	.type	SYNTAX_ReleaseImage, %function
SYNTAX_ReleaseImage:
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
	subs	r6, r1, #0
	mov	r8, r0
	beq	.L238
	ldr	r3, [r0]
	cmp	r3, #20
	ldrls	pc, [pc, r3, asl #2]
	b	.L194
.L153:
	.word	.L166
	.word	.L154
	.word	.L155
	.word	.L156
	.word	.L194
	.word	.L157
	.word	.L158
	.word	.L194
	.word	.L159
	.word	.L160
	.word	.L161
	.word	.L161
	.word	.L161
	.word	.L162
	.word	.L194
	.word	.L163
	.word	.L164
	.word	.L165
	.word	.L194
	.word	.L194
	.word	.L166
.L166:
	add	r5, r0, #16
.L167:
	ldr	r7, .L239
	mov	r0, #2
	ldr	r3, [r7, #12]
	blx	r3
	ldrh	r4, [r5, #4]
	ldrh	r3, [r5, #6]
	and	r4, r4, #63
	ldr	r9, [r6, #156]
	cmp	r3, r4
	ldr	r3, [r5, #344]
	rsb	r9, r3, r9
	bne	.L193
	b	.L192
.L172:
	add	r4, r4, #1
	ldrh	r3, [r5, #6]
	and	r4, r4, #63
	cmp	r3, r4
	beq	.L192
.L193:
	add	r10, r4, #2
	add	r3, r5, r10, lsl #2
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L170
	ldr	r0, [r3, #156]
	rsb	r0, r0, r9
	clz	r0, r0
	mov	r0, r0, lsr #5
	cmp	r0, #0
	beq	.L172
.L171:
	ldrh	r1, [r5, #4]
	b	.L174
.L196:
	mov	r4, ip
.L174:
	cmp	r4, #0
	mov	ip, #63
	subne	ip, r4, #1
	add	r2, r5, r10, lsl #2
	add	r3, ip, #2
	cmp	r4, r1
	add	lr, r5, r3, lsl #2
	mov	r10, r3
	ldr	r3, [lr, #4]
	str	r3, [r2, #4]
	bne	.L196
	ldr	r3, [r6, #156]
	cmp	r0, #1
	ldr	r2, [r5, #344]
	rsb	r3, r2, r3
	str	r3, [r6, #156]
	ldrh	r2, [r5, #4]
	add	r2, r2, #1
	and	r2, r2, #63
	strh	r2, [r5, #4]	@ movhi
	bne	.L172
	ldrh	lr, [r5, #8]
	mov	r0, #12
	ldr	ip, [r6, #156]
	ldrh	r3, [r5, #6]
	ldr	r1, .L239+4
	str	lr, [sp]
	str	ip, [sp, #4]
	bl	dprint_vfmw
	ldr	r3, [r8]
	cmp	r3, #20
	ldrls	pc, [pc, r3, asl #2]
	b	.L198
.L176:
	.word	.L175
	.word	.L177
	.word	.L178
	.word	.L179
	.word	.L198
	.word	.L180
	.word	.L181
	.word	.L198
	.word	.L182
	.word	.L183
	.word	.L184
	.word	.L184
	.word	.L184
	.word	.L185
	.word	.L198
	.word	.L186
	.word	.L187
	.word	.L188
	.word	.L198
	.word	.L198
	.word	.L189
.L154:
	add	r5, r0, #49152
	add	r5, r5, #72
	b	.L167
.L155:
	add	r5, r0, #22528
	add	r5, r5, #112
	b	.L167
.L156:
	add	r5, r0, #87040
	add	r5, r5, #680
	b	.L167
.L157:
	add	r5, r0, #41472
	add	r5, r5, #40
	b	.L167
.L158:
	add	r5, r0, #16128
	add	r5, r5, #8
	b	.L167
.L159:
	add	r5, r0, #53760
	add	r5, r5, #80
	b	.L167
.L160:
	add	r5, r0, #53760
	add	r5, r5, #64
	b	.L167
.L161:
	add	r5, r0, #12672
	add	r5, r5, #8
	b	.L167
.L162:
	add	r5, r0, #8192
	b	.L167
.L163:
	add	r5, r0, #592
	b	.L167
.L164:
	add	r5, r0, #61696
	add	r5, r5, #72
	b	.L167
.L165:
	add	r5, r0, #270336
	add	r5, r5, #872
	b	.L167
.L170:
	mov	r2, r4
	ldr	r1, .L239+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #2
	b	.L171
.L192:
	mov	r2, r9
	ldr	r1, .L239+12
	mov	r0, #1
	mvn	r4, #1
	bl	dprint_vfmw
.L169:
	ldr	r3, [r7, #16]
	mov	r0, #2
	blx	r3
	mov	r0, r4
.L151:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L194:
	mvn	r0, #1
	b	.L151
.L189:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	USERDEC_RecycleImage
	cmp	r0, #1
	beq	.L191
.L190:
	mvn	r4, #19
	b	.L169
.L188:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	VP9DEC_RecycleImage
	cmp	r0, #0
	bne	.L190
.L191:
	mov	r4, #1
	b	.L169
.L187:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	HEVC_DECRecycleImage
	cmp	r0, #0
	beq	.L191
	b	.L190
.L186:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	MVC_DEC_RecycleImage
	cmp	r0, #0
	beq	.L191
	b	.L190
.L185:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	VP8DEC_RecycleImage
	cmp	r0, #1
	bne	.L190
	b	.L191
.L184:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	VP6DEC_RecycleImage
	cmp	r0, #1
	bne	.L190
	b	.L191
.L183:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	REAL9DEC_RecycleImage
	cmp	r0, #1
	bne	.L190
	b	.L191
.L182:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	REAL8DEC_RecycleImage
	cmp	r0, #1
	bne	.L190
	b	.L191
.L181:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	AVSDEC_RecycleImage
	cmp	r0, #1
	bne	.L190
	b	.L191
.L180:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	DIVX3DEC_RecycleImage
	cmp	r0, #1
	bne	.L190
	b	.L191
.L179:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	MPEG2DEC_RecycleImage
	cmp	r0, #1
	bne	.L190
	b	.L191
.L178:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	MPEG4DEC_RecycleImage
	cmp	r0, #1
	bne	.L190
	b	.L191
.L177:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	VC1DEC_RecycleImage
	cmp	r0, #1
	bne	.L190
	b	.L191
.L175:
	ldr	r1, [r6, #156]
	add	r0, r8, #8
	bl	H264DEC_RecycleImage
	cmp	r0, #0
	bne	.L190
	b	.L191
.L198:
	mvn	r4, #1
	b	.L169
.L238:
	mov	r0, r6
	ldr	r1, .L239+16
	bl	dprint_vfmw
	mvn	r0, #1
	b	.L151
.L240:
	.align	2
.L239:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC15
	.word	.LC14
	.word	.LC13
	.word	.LC12
	UNWIND(.fnend)
	.size	SYNTAX_ReleaseImage, .-SYNTAX_ReleaseImage
	.align	2
	.global	SYNTAX_ReturnImage
	.type	SYNTAX_ReturnImage, %function
SYNTAX_ReturnImage:
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
	ldr	r3, [r0]
	cmp	r3, #20
	ldrls	pc, [pc, r3, asl #2]
	b	.L259
.L244:
	.word	.L257
	.word	.L245
	.word	.L246
	.word	.L247
	.word	.L259
	.word	.L248
	.word	.L249
	.word	.L259
	.word	.L250
	.word	.L251
	.word	.L252
	.word	.L252
	.word	.L252
	.word	.L253
	.word	.L259
	.word	.L254
	.word	.L255
	.word	.L256
	.word	.L259
	.word	.L259
	.word	.L257
.L257:
	add	r4, r0, #16
.L258:
	ldr	r5, .L260
	mov	r0, #2
	ldr	r3, [r5, #12]
	blx	r3
	ldrh	r3, [r4, #6]
	mov	r0, #2
	sub	r3, r3, #1
	and	r3, r3, #63
	strh	r3, [r4, #6]	@ movhi
	ldr	r3, [r5, #16]
	blx	r3
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L245:
	add	r4, r0, #49152
	add	r4, r4, #72
	b	.L258
.L246:
	add	r4, r0, #22528
	add	r4, r4, #112
	b	.L258
.L247:
	add	r4, r0, #87040
	add	r4, r4, #680
	b	.L258
.L248:
	add	r4, r0, #41472
	add	r4, r4, #40
	b	.L258
.L249:
	add	r4, r0, #16128
	add	r4, r4, #8
	b	.L258
.L250:
	add	r4, r0, #53760
	add	r4, r4, #80
	b	.L258
.L251:
	add	r4, r0, #53760
	add	r4, r4, #64
	b	.L258
.L252:
	add	r4, r0, #12672
	add	r4, r4, #8
	b	.L258
.L253:
	add	r4, r0, #8192
	b	.L258
.L254:
	add	r4, r0, #592
	b	.L258
.L255:
	add	r4, r0, #61696
	add	r4, r4, #72
	b	.L258
.L256:
	add	r4, r0, #270336
	add	r4, r4, #872
	b	.L258
.L259:
	mvn	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L261:
	.align	2
.L260:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	SYNTAX_ReturnImage, .-SYNTAX_ReturnImage
	.align	2
	.global	SegMan
	.type	SegMan, %function
SegMan:
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
	subs	r4, r1, #0
	mov	r6, r0
	beq	.L263
	ldr	r3, [r4, #8]
	cmp	r3, #0
	beq	.L263
	ldr	r3, .L287
	ldr	r3, [r3, r0, asl #2]
	ldr	r2, [r3, #1440]
	cmp	r2, #1
	beq	.L283
.L266:
	ldrb	r5, [r4, #14]	@ zero_extendqisi2
	mov	r7, #0
	strb	r7, [r4, #408]
	cmp	r5, #1
	beq	.L284
	cmp	r5, #0
	beq	.L285
.L271:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L285:
	ldr	r9, .L287+4
	add	r8, r4, #88
	add	r7, r4, #168
	mov	r2, #80
	add	r1, r4, #8
	mov	r0, r8
	ldr	r3, [r9, #52]
	blx	r3
	ldr	r3, [r9, #48]
	mov	r0, r7
	mov	r2, #80
	mov	r1, r5
	blx	r3
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	add	r3, r3, #1
	uxtb	r3, r3
	strb	r3, [r4, #2]
	cmp	r3, #1
	bls	.L271
	ldr	r1, [r4, #108]
	mov	r0, r6
	bl	SM_ReleaseStreamSeg
	ldr	r1, [r4, #188]
	mov	r0, r6
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r9, #48]
	mov	r2, #80
	mov	r1, r5
	mov	r0, r8
	blx	r3
	ldr	r3, [r9, #48]
	mov	r2, #80
	mov	r1, r5
	mov	r0, r7
	blx	r3
	ldr	r1, .L287+8
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L287+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L270
	mov	r0, r6
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L270:
	mov	r3, #0
	mvn	r0, #0
	strb	r3, [r4, #2]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L283:
	ldr	r3, [r3, #1480]
	cmp	r3, #1
	bne	.L266
	ldrb	r3, [r4, #408]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L266
	mov	r2, #160
	add	r1, r4, #248
	add	r0, r4, #88
	bl	memcpy
	mov	r3, #0
	mov	r0, r3
	strb	r3, [r4, #408]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L284:
	ldrb	r5, [r4, #2]	@ zero_extendqisi2
	cmp	r5, r7
	beq	.L286
	cmp	r5, #1
	bne	.L271
	ldr	r3, [r4, #104]
	cmp	r3, #1024
	bcs	.L272
	ldr	r1, .L287+16
	mov	r0, #22
	ldr	r8, .L287+4
	bl	dprint_vfmw
	ldr	r2, [r4, #104]
	ldr	r0, [r4, #8]
	ldr	r3, [r8, #52]
	rsb	r0, r2, r0
	ldr	r1, [r4, #88]
	blx	r3
	ldr	r1, [r4, #104]
	ldr	ip, [r4, #8]
	mov	r0, r6
	ldr	r2, [r4, #20]
	ldr	r3, [r4, #24]
	rsb	ip, r1, ip
	rsb	r2, r1, r2
	str	ip, [r4, #88]
	add	r3, r3, r1
	str	r2, [r4, #100]
	str	r3, [r4, #104]
	ldr	r1, [r4, #108]
	strb	r5, [r4, #94]
	bl	SM_ReleaseStreamSeg
	ldr	ip, [r4, #28]
	ldr	r3, [r8, #48]
	mov	r1, r7
	mov	r2, #80
	add	r0, r4, #168
	str	ip, [r4, #108]
	blx	r3
.L273:
	mov	r5, #0
	mov	r3, #1
	strb	r5, [r4, #2]
	mov	r2, #160
	strb	r3, [r4, #408]
	add	r1, r4, #88
	add	r0, r4, #248
	bl	memcpy
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L286:
	ldr	r6, .L287+4
	add	r1, r4, #8
	mov	r2, #80
	add	r0, r4, #88
	ldr	r3, [r6, #52]
	blx	r3
	ldr	r3, [r6, #48]
	mov	r2, #80
	mov	r1, r5
	add	r0, r4, #168
	blx	r3
	strb	r5, [r4, #2]
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L272:
	ldr	r1, .L287+20
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r3, .L287+4
	mov	r2, #80
	add	r1, r4, #8
	add	r0, r4, #168
	ldr	r3, [r3, #52]
	blx	r3
	b	.L273
.L263:
	ldr	r1, .L287+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L288:
	.align	2
.L287:
	.word	s_pstVfmwChan
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC17
	.word	g_event_report
	.word	.LC18
	.word	.LC19
	.word	.LC16
	UNWIND(.fnend)
	.size	SegMan, .-SegMan
	.align	2
	.global	DecodeStream
	.type	DecodeStream, %function
DecodeStream:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
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
	mov	r4, r0
	mov	r5, r2
	cmp	r1, #17
	ldrls	pc, [pc, r1, asl #2]
	b	.L289
.L292:
	.word	.L291
	.word	.L293
	.word	.L294
	.word	.L295
	.word	.L289
	.word	.L296
	.word	.L297
	.word	.L289
	.word	.L298
	.word	.L299
	.word	.L300
	.word	.L300
	.word	.L300
	.word	.L301
	.word	.L289
	.word	.L302
	.word	.L303
	.word	.L304
.L293:
	ldrb	r2, [r0, #14]	@ zero_extendqisi2
	mov	r0, #22
	ldr	r1, .L405
	bl	dprint_vfmw
	ldr	r2, [r4, #20]
	ldr	r1, .L405+4
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r2, [r4, #24]
	ldr	r1, .L405+8
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r2, [r5]
	ldr	r3, [r2, #556]
	cmp	r3, #1
	bhi	.L386
	ldr	r1, [r2, #560]
	cmp	r1, #8
	bgt	.L387
	add	r6, r5, #12288
	uxtb	r3, r3
	cmp	r3, #1
	strb	r3, [r6, #3536]
	ldr	r3, [r2, #560]
	strb	r3, [r6, #3538]
	beq	.L388
	ldrb	r3, [r4, #15]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L389
	ldr	r6, .L405+12
	add	r7, r4, #88
	mov	r2, #80
	add	r1, r4, #8
	mov	r0, r7
	ldr	r3, [r6, #52]
	blx	r3
	ldr	r3, [r6, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	ldr	r3, [r4, #104]
	cmp	r3, #15
	bhi	.L325
	ldr	r1, .L405+16
	add	r5, r5, #49152
.L385:
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, [r4, #108]
	ldr	r0, [r5, #416]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L405+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L289
	mov	r3, #0
	ldr	r0, [r5, #416]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L289:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L304:
	ldrb	r3, [r0, #15]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L390
	ldr	r6, .L405+12
	add	r8, r0, #88
	add	r1, r0, #8
	mov	r2, #80
	mov	r0, r8
	add	r7, r5, #290816
	ldr	r3, [r6, #52]
	blx	r3
	ldr	r3, [r6, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	mov	r1, r8
	mov	r0, r5
	bl	VP9DEC_DecodePacket
	ldr	r3, [r7, #2888]
	cmp	r3, #0
	bne	.L391
.L313:
	ldr	r3, [r7, #2884]
	cmp	r3, #0
	beq	.L382
.L314:
	mov	r3, #1
.L382:
	strb	r3, [r4, #1]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bne	.L289
	add	r5, r5, #262144
	ldrb	r5, [r5, #1036]	@ zero_extendqisi2
	cmp	r5, #0
	bne	.L289
	ldr	r3, [r6, #48]
	mov	r2, #160
	mov	r1, r5
	mov	r0, r8
	blx	r3
	ldr	r1, [r4, #28]
	ldr	r0, [r7, #2980]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L405+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L289
	ldr	r0, [r7, #2980]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
	b	.L289
.L291:
	ldrd	r2, [r0, #32]
	sub	r1, fp, #76
	ldr	r10, [r0, #8]
	mov	r0, r5
	ldr	r9, [r4, #20]
	ldr	r8, [r4, #24]
	ldrb	r7, [r4, #14]	@ zero_extendqisi2
	ldr	r6, [r4, #28]
	ldrb	lr, [r4, #16]	@ zero_extendqisi2
	ldrsb	ip, [r4, #13]
	strd	r2, [fp, #-52]
	str	r10, [fp, #-72]
	str	r9, [fp, #-68]
	str	r8, [fp, #-64]
	strb	r7, [fp, #-76]
	str	r6, [fp, #-60]
	strb	lr, [fp, #-75]
	str	ip, [fp, #-56]
	bl	H264DEC_DecodePacket
	cmn	r0, #2
	moveq	r3, #1
	movne	r3, #0
	strb	r3, [r4, #1]
	ldr	r4, [r5, #360]
	cmp	r4, #0
	bne	.L289
	add	r6, r5, #12288
	ldrb	r3, [r6, #1272]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L289
	add	r5, r5, #61440
	ldr	r0, [r5, #1144]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r6, #1272]
	b	.L289
.L294:
	add	r6, r2, #20480
	mov	r0, r2
	ldr	r7, [r6, #2504]
	bl	MP4_GetPacketState
	cmp	r0, #3
	beq	.L392
.L331:
	mov	r1, r4
	ldr	r0, [r6, #2504]
	bl	SegMan
	cmp	r0, #0
	beq	.L393
.L333:
	ldr	r4, [r5, #1036]
	cmp	r4, #0
	bne	.L289
	ldrb	r3, [r6, #2540]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L289
	ldr	r0, [r6, #2504]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r6, #2540]
	b	.L289
.L295:
	add	r6, r2, #86016
	mov	r0, r2
	ldr	r7, [r6, #2048]
	bl	MP2_GetPacketState
	cmp	r0, #2
	beq	.L394
.L327:
	mov	r1, r4
	ldr	r0, [r6, #2048]
	bl	SegMan
	cmp	r0, #0
	beq	.L395
.L329:
	ldr	r4, [r5, #4]
	cmp	r4, #0
	bne	.L289
	ldrb	r3, [r6, #2066]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L289
	ldr	r0, [r6, #2048]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r6, #2066]
	b	.L289
.L296:
	ldrb	r3, [r0, #15]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L396
	ldr	r6, .L405+12
	add	r7, r0, #88
	add	r1, r0, #8
	mov	r2, #80
	mov	r0, r7
	ldr	r3, [r6, #52]
	blx	r3
	ldr	r3, [r6, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	mov	r1, r7
	mov	r0, r5
	bl	DIVX3DEC_Decode
	cmn	r0, #2
	moveq	r3, #1
	movne	r3, #0
	strb	r3, [r4, #1]
	ldr	r8, [r5, #4]
	cmp	r8, #0
	bne	.L289
	add	r5, r5, #40960
	ldr	r3, [r6, #48]
	mov	r2, #160
	mov	r1, r8
	mov	r0, r7
	blx	r3
	ldr	r1, [r4, #28]
	ldr	r0, [r5, #896]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L405+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L289
	ldr	r0, [r5, #896]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	blx	ip
	b	.L289
.L297:
	mov	r0, r2
	ldr	r6, [r2, #4]
	bl	AVS_GetPacketState
	cmp	r0, #0
	bne	.L334
	ldr	r3, .L405+24
	ldr	r3, [r3, r6, asl #2]
	ldr	r2, [r3, #1440]
	cmp	r2, #1
	bne	.L335
	ldr	r3, [r3, #1480]
	cmp	r3, #0
	bne	.L334
.L335:
	add	r1, r5, #16384
	mov	r0, r5
	add	r1, r1, #104
	bl	AVSDEC_Decode
.L334:
	mov	r1, r4
	ldr	r0, [r5, #4]
	bl	SegMan
	cmp	r0, #0
	beq	.L397
.L336:
	ldr	r4, [r5, #16]
	cmp	r4, #0
	bne	.L289
	ldrb	r3, [r5]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L289
	ldr	r0, [r5, #4]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r5]
	b	.L289
.L298:
	ldrb	r3, [r0, #15]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L398
	ldr	r6, .L405+12
	add	r7, r0, #88
	add	r1, r0, #8
	mov	r2, #80
	mov	r0, r7
	ldr	r3, [r6, #52]
	blx	r3
	ldr	r3, [r6, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	mov	r1, r7
	mov	r0, r5
	bl	REAL8DEC_Decode
	cmn	r0, #2
	moveq	r3, #1
	movne	r3, #0
	strb	r3, [r4, #1]
	ldr	r8, [r5, #4]
	cmp	r8, #0
	bne	.L289
	add	r5, r5, #53248
	ldr	r3, [r6, #48]
	mov	r2, #160
	mov	r1, r8
	mov	r0, r7
	blx	r3
	ldr	r1, [r4, #28]
	ldr	r0, [r5, #936]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L405+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L289
	ldr	r0, [r5, #936]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	blx	ip
	b	.L289
.L299:
	ldrb	r3, [r0, #15]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L399
	ldr	r6, .L405+12
	add	r7, r0, #88
	add	r1, r0, #8
	mov	r2, #80
	mov	r0, r7
	ldr	r3, [r6, #52]
	blx	r3
	ldr	r3, [r6, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	mov	r1, r7
	mov	r0, r5
	bl	REAL9DEC_Decode
	cmn	r0, #2
	moveq	r3, #1
	movne	r3, #0
	strb	r3, [r4, #1]
	ldr	r8, [r5, #4]
	cmp	r8, #0
	bne	.L289
	add	r5, r5, #53248
	ldr	r3, [r6, #48]
	mov	r2, #160
	mov	r1, r8
	mov	r0, r7
	blx	r3
	ldr	r1, [r4, #28]
	ldr	r0, [r5, #920]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L405+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L289
	ldr	r0, [r5, #920]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	blx	ip
	b	.L289
.L300:
	ldrb	r3, [r0, #15]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L400
	ldr	r6, .L405+12
	add	r7, r0, #88
	add	r1, r0, #8
	mov	r2, #80
	mov	r0, r7
	ldr	r3, [r6, #52]
	blx	r3
	ldr	r3, [r6, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	mov	r1, r7
	mov	r0, r5
	bl	VP6DEC_Decode
	cmn	r0, #2
	moveq	r3, #1
	movne	r3, #0
	strb	r3, [r4, #1]
	ldr	r8, [r5, #4]
	cmp	r8, #0
	bne	.L289
	add	r5, r5, #12288
	ldr	r3, [r6, #48]
	mov	r2, #160
	mov	r1, r8
	mov	r0, r7
	blx	r3
	ldr	r1, [r4, #28]
	ldr	r0, [r5, #736]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L405+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L289
	ldr	r0, [r5, #736]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	blx	ip
	b	.L289
.L301:
	ldrb	r3, [r0, #15]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L401
	ldr	r6, .L405+12
	add	r7, r0, #88
	add	r1, r0, #8
	mov	r2, #80
	mov	r0, r7
	ldr	r3, [r6, #52]
	blx	r3
	ldr	r3, [r6, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	mov	r1, r7
	mov	r0, r5
	bl	VP8DEC_Decode
	cmn	r0, #2
	moveq	r3, #1
	movne	r3, #0
	strb	r3, [r4, #1]
	ldr	r8, [r5, #4]
	cmp	r8, #0
	bne	.L289
	add	r5, r5, #8192
	ldr	r3, [r6, #48]
	mov	r2, #160
	mov	r1, r8
	mov	r0, r7
	blx	r3
	ldr	r1, [r4, #28]
	ldr	r0, [r5, #344]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L405+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L289
	ldr	r0, [r5, #344]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	blx	ip
	b	.L289
.L302:
	ldrd	r2, [r0, #32]
	sub	r1, fp, #76
	ldr	r8, [r0, #8]
	mov	r0, r5
	ldr	r7, [r4, #20]
	ldr	r6, [r4, #24]
	ldrb	lr, [r4, #14]	@ zero_extendqisi2
	ldr	ip, [r4, #28]
	strd	r2, [fp, #-52]
	str	r8, [fp, #-72]
	str	r7, [fp, #-68]
	str	r6, [fp, #-64]
	strb	lr, [fp, #-76]
	str	ip, [fp, #-60]
	bl	MVC_DEC_DecodePacket
	cmn	r0, #2
	moveq	r3, #1
	movne	r3, #0
	strb	r3, [r4, #1]
	ldr	r4, [r5, #228]
	cmp	r4, #0
	bne	.L289
	ldrb	r3, [r5, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L289
	ldr	r0, [r5, #120]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r5, #1]
	b	.L289
.L303:
	ldrd	r2, [r0, #32]
	sub	r1, fp, #76
	ldr	r10, [r0, #8]
	mov	r0, r5
	ldr	r9, [r4, #20]
	ldr	r8, [r4, #24]
	ldrb	r7, [r4, #14]	@ zero_extendqisi2
	ldr	r6, [r4, #28]
	ldrb	lr, [r4, #16]	@ zero_extendqisi2
	ldrsb	ip, [r4, #13]
	strd	r2, [fp, #-52]
	str	r10, [fp, #-72]
	str	r9, [fp, #-68]
	str	r8, [fp, #-64]
	strb	r7, [fp, #-76]
	str	r6, [fp, #-60]
	strb	lr, [fp, #-75]
	str	ip, [fp, #-56]
	bl	HEVCDEC_DecodePacket
	cmn	r0, #2
	beq	.L309
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	cmp	r3, #0
	streqb	r3, [r4, #1]
	beq	.L311
.L309:
	mov	r3, #1
	strb	r3, [r4, #1]
.L311:
	ldr	r4, [r5, #2360]
	cmp	r4, #0
	bne	.L289
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L289
	ldr	r0, [r5, #276]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r5, #7]
	b	.L289
.L386:
	ldr	r1, .L405+28
	mov	r0, #0
	bl	dprint_vfmw
	b	.L289
.L391:
	ldr	r2, [r7, #2928]
	ldr	r3, [r7, #2924]
	cmp	r2, r3
	bne	.L314
	b	.L313
.L397:
	add	r1, r4, #88
	mov	r0, r5
	bl	AVSDEC_Decode
	b	.L336
.L395:
	add	r1, r4, #88
	mov	r0, r5
	bl	MPEG2DEC_Decode
	ldrb	r3, [r6, #2065]	@ zero_extendqisi2
	cmp	r3, #1
	movne	r3, #0
	strb	r3, [r4, #1]
	b	.L329
.L393:
	add	r1, r4, #88
	mov	r0, r5
	bl	MPEG4DEC_Decode
	b	.L333
.L394:
	ldr	r3, .L405+24
	ldr	r3, [r3, r7, asl #2]
	ldr	r2, [r3, #1440]
	cmp	r2, #1
	bne	.L328
	ldr	r3, [r3, #1480]
	cmp	r3, #0
	bne	.L327
.L328:
	add	r1, r5, #87040
	mov	r0, r5
	add	r1, r1, #128
	bl	MPEG2DEC_Decode
	b	.L327
.L392:
	ldr	r3, .L405+24
	ldr	r3, [r3, r7, asl #2]
	ldr	r2, [r3, #1440]
	cmp	r2, #1
	bne	.L332
	ldr	r3, [r3, #1480]
	cmp	r3, #0
	bne	.L331
.L332:
	add	r1, r5, #15680
	mov	r0, r5
	add	r1, r1, #48
	bl	MPEG4DEC_Decode
	b	.L331
.L396:
	add	r5, r2, #40960
	ldr	r0, [r5, #896]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #896]
	ldr	r1, [r4, #28]
	bl	SM_ReleaseStreamSeg
	b	.L289
.L401:
	add	r5, r2, #8192
	ldr	r0, [r5, #344]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #344]
	ldr	r1, [r4, #28]
	bl	SM_ReleaseStreamSeg
	b	.L289
.L400:
	add	r5, r2, #12288
	ldr	r0, [r5, #736]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #736]
	ldr	r1, [r4, #28]
	bl	SM_ReleaseStreamSeg
	b	.L289
.L399:
	add	r5, r2, #53248
	ldr	r0, [r5, #920]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #920]
	ldr	r1, [r4, #28]
	bl	SM_ReleaseStreamSeg
	b	.L289
.L398:
	add	r5, r2, #53248
	ldr	r0, [r5, #936]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #936]
	ldr	r1, [r4, #28]
	bl	SM_ReleaseStreamSeg
	b	.L289
.L390:
	add	r5, r2, #290816
	ldr	r0, [r5, #2980]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #2980]
	ldr	r1, [r4, #28]
	bl	SM_ReleaseStreamSeg
	b	.L289
.L325:
	add	r2, r5, #15808
	mov	r1, #16
	ldr	r0, [r4, #88]
	add	r2, r2, r1
	bl	Vc1SMPSeqHdr
	subs	r8, r0, #0
	beq	.L402
	add	r5, r5, #49152
	ldr	r1, .L405+32
	b	.L385
.L387:
	ldr	r1, .L405+36
	mov	r0, #0
	bl	dprint_vfmw
	b	.L289
.L389:
	add	r5, r5, #49152
	ldr	r0, [r5, #416]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #416]
	ldr	r1, [r4, #28]
	bl	SM_ReleaseStreamSeg
	b	.L289
.L388:
	add	r7, r5, #49152
	mov	r0, r5
	ldr	r8, [r7, #416]
	bl	VC1_GetPacketState
	cmp	r0, #2
	beq	.L403
.L321:
	mov	r1, r4
	ldr	r0, [r7, #416]
	bl	SegMan
	subs	r8, r0, #0
	beq	.L404
.L323:
	ldr	r4, [r5, #4]
	cmp	r4, #0
	bne	.L289
	ldrb	r3, [r6, #447]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L289
	ldr	r0, [r7, #416]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r6, #447]
	b	.L289
.L402:
	ldr	ip, [r4, #104]
	mov	r1, r7
	ldr	r2, [r4, #88]
	mov	r0, r5
	ldr	r3, [r4, #100]
	sub	ip, ip, #16
	add	r2, r2, #16
	str	ip, [r4, #104]
	str	r2, [r4, #88]
	add	r3, r3, #16
	str	r3, [r4, #100]
	bl	VC1DEC_Decode
	ldr	r3, [r6, #48]
	mov	r1, r8
	mov	r0, r7
	mov	r2, #160
	blx	r3
	b	.L289
.L404:
	add	r4, r4, #88
	mov	r0, r5
	mov	r1, r4
	bl	VC1DEC_Decode
	ldr	r3, .L405+12
	mov	r0, r4
	mov	r1, r8
	mov	r2, #160
	ldr	r3, [r3, #48]
	blx	r3
	b	.L323
.L403:
	ldr	r3, .L405+24
	ldr	r3, [r3, r8, asl #2]
	ldr	r2, [r3, #1440]
	cmp	r2, #1
	bne	.L322
	ldr	r3, [r3, #1480]
	cmp	r3, #0
	bne	.L321
.L322:
	add	r1, r5, #12544
	mov	r0, r5
	add	r1, r1, #24
	bl	VC1DEC_Decode
	b	.L321
.L406:
	.align	2
.L405:
	.word	.LC20
	.word	.LC21
	.word	.LC22
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC25
	.word	g_event_report
	.word	s_pstVfmwChan
	.word	.LC23
	.word	.LC26
	.word	.LC24
	UNWIND(.fnend)
	.size	DecodeStream, .-DecodeStream
	.align	2
	.global	ReleasePacket
	.type	ReleasePacket, %function
ReleasePacket:
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
	sub	r3, r0, #5
	cmp	r3, #12
	ldrls	pc, [pc, r3, asl #2]
	b	.L408
.L410:
	.word	.L409
	.word	.L408
	.word	.L408
	.word	.L411
	.word	.L412
	.word	.L413
	.word	.L413
	.word	.L413
	.word	.L414
	.word	.L408
	.word	.L408
	.word	.L408
	.word	.L415
.L415:
	ldr	r4, [r1]
	add	r1, r1, #290816
	ldr	r0, [r1, #2980]
	ldr	r1, [r4, #244]
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r4, #230]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L421
	ldr	r1, .L431
	mov	r0, #1
	bl	dprint_vfmw
.L421:
	ldr	r3, .L431+4
	add	r0, r4, #224
	mov	r2, #160
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L409:
	ldr	r4, [r1]
	ldrb	r3, [r4, #230]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L421
	add	r3, r1, #40960
	ldr	r1, [r4, #244]
	ldr	r0, [r3, #896]
	bl	SM_ReleaseStreamSeg
	b	.L421
.L411:
	ldr	r4, [r1]
	ldrb	r3, [r4, #230]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L421
	add	r3, r1, #53248
	ldr	r1, [r4, #244]
	ldr	r0, [r3, #936]
	bl	SM_ReleaseStreamSeg
	b	.L421
.L412:
	ldr	r4, [r1]
	ldrb	r3, [r4, #230]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L421
	add	r3, r1, #53248
	ldr	r1, [r4, #244]
	ldr	r0, [r3, #920]
	bl	SM_ReleaseStreamSeg
	b	.L421
.L413:
	ldr	r4, [r1]
	add	r1, r1, #12288
	ldr	r0, [r1, #736]
	ldr	r1, [r4, #244]
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r4, #230]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L421
	ldr	r1, .L431+8
	mov	r0, #1
	bl	dprint_vfmw
	b	.L421
.L414:
	ldr	r4, [r1]
	add	r1, r1, #8192
	ldr	r0, [r1, #344]
	ldr	r1, [r4, #244]
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r4, #230]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L421
	ldr	r1, .L431+12
	mov	r0, #1
	bl	dprint_vfmw
	b	.L421
.L408:
	mov	r2, r0
	ldr	r1, .L431+16
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L432:
	.align	2
.L431:
	.word	.LC29
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC28
	.word	.LC27
	.word	.LC30
	UNWIND(.fnend)
	.size	ReleasePacket, .-ReleasePacket
	.align	2
	.global	ReleaseCurPic
	.type	ReleaseCurPic, %function
ReleaseCurPic:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	cmp	r0, #17
	ldrls	pc, [pc, r0, asl #2]
	b	.L434
.L436:
	.word	.L435
	.word	.L437
	.word	.L438
	.word	.L439
	.word	.L434
	.word	.L440
	.word	.L441
	.word	.L434
	.word	.L442
	.word	.L443
	.word	.L444
	.word	.L444
	.word	.L444
	.word	.L445
	.word	.L434
	.word	.L446
	.word	.L447
	.word	.L448
.L448:
	ldr	r4, [r1]
	add	r1, r1, #290816
	ldr	r0, [r1, #2980]
	ldr	r1, [r4, #244]
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r4, #230]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L454
	ldr	r1, .L465
	mov	r0, #1
	bl	dprint_vfmw
	b	.L454
.L435:
	mov	r0, r1
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	H264_ClearCurrPic
.L438:
	mov	r0, r1
	str	r1, [fp, #-24]
	bl	MP4_ClearCurPic
	ldr	r1, [fp, #-24]
	mov	r0, r1
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	MP4_Clean_HeadPacket
.L439:
	mov	r0, r1
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	MP2_ClearCurPic
.L437:
	add	r3, r1, #49152
	add	r0, r1, #12672
	add	r0, r0, #56
	ldr	r1, [r3, #416]
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	VC1_ClearCurPic
.L440:
	ldr	r4, [r1]
	ldrb	r3, [r4, #230]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L464
.L454:
	ldr	r3, .L465+4
	add	r0, r4, #224
	mov	r2, #160
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	bx	r3
.L441:
	mov	r0, r1
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	AVS_ClearCurPic
.L442:
	ldr	r4, [r1]
	ldrb	r3, [r4, #230]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L454
	add	r3, r1, #53248
	ldr	r1, [r4, #244]
	ldr	r0, [r3, #936]
	bl	SM_ReleaseStreamSeg
	b	.L454
.L443:
	ldr	r4, [r1]
	ldrb	r3, [r4, #230]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L454
	add	r3, r1, #53248
	ldr	r1, [r4, #244]
	ldr	r0, [r3, #920]
	bl	SM_ReleaseStreamSeg
	b	.L454
.L444:
	ldr	r4, [r1]
	add	r1, r1, #12288
	ldr	r0, [r1, #736]
	ldr	r1, [r4, #244]
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r4, #230]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L454
	ldr	r1, .L465+8
	mov	r0, #1
	bl	dprint_vfmw
	b	.L454
.L445:
	ldr	r4, [r1]
	add	r1, r1, #8192
	ldr	r0, [r1, #344]
	ldr	r1, [r4, #244]
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r4, #230]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L454
	ldr	r1, .L465+12
	mov	r0, #1
	bl	dprint_vfmw
	b	.L454
.L446:
	mov	r0, r1
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	MVC_ClearCurrPic
.L447:
	mov	r0, r1
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	HEVC_ClearCurrPic
.L434:
	mov	r2, r0
	ldr	r1, .L465+16
	mov	r0, #0
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	dprint_vfmw
.L464:
	add	r3, r1, #40960
	ldr	r1, [r4, #244]
	ldr	r0, [r3, #896]
	bl	SM_ReleaseStreamSeg
	b	.L454
.L466:
	.align	2
.L465:
	.word	.LC29
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC28
	.word	.LC27
	.word	.LC30
	UNWIND(.fnend)
	.size	ReleaseCurPic, .-ReleaseCurPic
	.align	2
	.global	IsNewpicSeg
	.type	IsNewpicSeg, %function
IsNewpicSeg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	cmp	r1, #2
	ldr	ip, [r2]
	movne	lr, #0
	beq	.L511
.L468:
	ldrb	r2, [ip]	@ zero_extendqisi2
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	cmp	r2, #0
	ldrb	r4, [ip, #2]	@ zero_extendqisi2
	bne	.L471
	cmp	r3, #0
	bne	.L471
	cmp	r4, #1
	bne	.L471
	cmp	r1, #16
	ldrls	pc, [pc, r1, asl #2]
	b	.L491
.L476:
	.word	.L475
	.word	.L477
	.word	.L478
	.word	.L479
	.word	.L491
	.word	.L491
	.word	.L480
	.word	.L491
	.word	.L491
	.word	.L491
	.word	.L491
	.word	.L491
	.word	.L491
	.word	.L491
	.word	.L491
	.word	.L481
	.word	.L482
.L511:
	ldrb	lr, [r2, #72]	@ zero_extendqisi2
	ands	lr, lr, #1
	beq	.L468
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	cmp	r2, #179
	cmpne	r2, #176
	add	r3, r2, #75
	moveq	r1, #1
	movne	r1, #0
	cmp	r2, #254
	orreq	r1, r1, #1
	uxtb	r3, r3
	cmp	r2, #47
	orrls	r1, r1, #1
	cmp	r3, #1
	movhi	r3, r1
	orrls	r3, r1, #1
	cmp	r3, #0
	bne	.L489
.L485:
	mov	r2, #1
	str	r2, [r0]
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	mov	r3, #0
.L486:
	cmp	r2, #182
	beq	.L509
.L502:
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L489:
	ldr	r3, [r0]
	cmp	r3, #0
	beq	.L485
	b	.L484
.L475:
	mov	r1, ip
	bl	H264_IsNewPicNal
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r3, r0, #0
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L477:
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	sub	r3, r2, #12
	cmp	r2, #10
	cmpne	r3, #3
	bls	.L512
.L498:
	mvn	r3, #0
.L488:
	cmp	r2, #13
	bne	.L502
	b	.L509
.L478:
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	cmp	r2, #179
	cmpne	r2, #176
	add	r4, r2, #75
	moveq	r3, #1
	movne	r3, #0
	cmp	r2, #254
	orreq	r3, r3, #1
	cmp	r2, #47
	movhi	r1, r3
	orrls	r1, r3, #1
	uxtb	r3, r4
	cmp	r3, #1
	movhi	r3, r1
	orrls	r3, r1, #1
	cmp	r3, #0
	bne	.L489
.L484:
	cmp	lr, #1
	mvnne	r3, #0
	bne	.L486
	b	.L485
.L479:
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	add	r3, r2, #73
	cmp	r2, #0
	cmpne	r2, #179
	uxtb	r3, r3
	moveq	r1, #1
	movne	r1, #0
	cmp	r3, #1
	movhi	r3, r1
	orrls	r3, r1, #1
	cmp	r3, #0
	bne	.L513
.L493:
	mvn	r3, #0
.L483:
	cmp	r2, #0
	bne	.L502
.L510:
	str	r2, [r0]
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L480:
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	sub	r1, r2, #179
	add	r3, r2, #74
	clz	r1, r1
	uxtb	r3, r3
	mov	r1, r1, lsr #5
	cmp	r2, #176
	movne	lr, r1
	orreq	lr, r1, #1
	cmp	r3, #1
	movhi	r3, lr
	orrls	r3, lr, #1
	cmp	r3, #0
	bne	.L514
.L496:
	mvn	r3, #0
.L487:
	cmp	r2, #182
	movne	r2, r1
	orreq	r2, r1, #1
	cmp	r2, #0
	beq	.L502
.L509:
	mov	r2, #0
	b	.L510
.L481:
	mov	r1, ip
	bl	MVC_IsNewPicNal
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r3, r0, #0
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L482:
	mov	r1, ip
	bl	HEVC_IsNewPicNal
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r3, r0, #0
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L491:
	mvn	r3, #0
	b	.L502
.L471:
	str	r4, [sp]
	mov	r0, #22
	ldr	r1, .L515
	bl	dprint_vfmw
	mvn	r3, #0
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L512:
	ldr	r3, [r0]
	cmp	r3, #0
	bne	.L498
	mov	r2, #1
	str	r2, [r0]
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	b	.L488
.L514:
	ldr	r3, [r0]
	cmp	r3, #0
	bne	.L496
	mov	r2, #1
	str	r2, [r0]
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	sub	r1, r2, #179
	clz	r1, r1
	mov	r1, r1, lsr #5
	b	.L487
.L513:
	ldr	r3, [r0]
	cmp	r3, #0
	bne	.L493
	mov	r2, #1
	str	r2, [r0]
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	b	.L483
.L516:
	.align	2
.L515:
	.word	.LC31
	UNWIND(.fnend)
	.size	IsNewpicSeg, .-IsNewpicSeg
	.align	2
	.global	OpenUsdPool
	.type	OpenUsdPool, %function
OpenUsdPool:
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
	ldr	r4, .L524
	add	r4, r4, r0, lsl #2
	ldr	ip, [r4, #64]
	cmp	ip, #0
	beq	.L523
.L518:
	add	r1, ip, #203776
	mov	r0, #0
	add	r1, r1, #512
	mov	r3, ip
	mov	r2, r0
	str	r0, [ip]
.L520:
	str	r2, [r3, #1048]
	add	r3, r3, #1056
	str	r2, [r3, #-12]
	add	r3, r3, #8
	cmp	r3, r1
	bne	.L520
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L523:
	ldr	r3, .L524+4
	movw	r0, #7688
	movt	r0, 3
	ldr	r3, [r3, #196]
	blx	r3
	cmp	r0, #0
	str	r0, [r4, #64]
	movne	ip, r0
	bne	.L518
.L521:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L525:
	.align	2
.L524:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	OpenUsdPool, .-OpenUsdPool
	.align	2
	.global	CloseUsdPool
	.type	CloseUsdPool, %function
CloseUsdPool:
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
	ldr	r4, .L531
	add	r4, r4, r0, lsl #2
	ldr	r0, [r4, #64]
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, .L531+4
	ldr	r3, [r3, #200]
	blx	r3
	mov	r3, #0
	str	r3, [r4, #64]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L532:
	.align	2
.L531:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	CloseUsdPool, .-CloseUsdPool
	.align	2
	.global	OpenUsdPool_WithMem
	.type	OpenUsdPool_WithMem, %function
OpenUsdPool_WithMem:
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
	ldr	r3, .L540
	add	r0, r3, r0, lsl #2
	ldr	ip, [r0, #64]
	cmp	ip, #0
	beq	.L539
.L534:
	add	r1, ip, #203776
	mov	r0, #0
	add	r1, r1, #512
	mov	r3, ip
	mov	r2, r0
	str	r0, [ip]
.L536:
	str	r2, [r3, #1048]
	add	r3, r3, #1056
	str	r2, [r3, #-12]
	add	r3, r3, #8
	cmp	r3, r1
	bne	.L536
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L539:
	cmp	r1, #0
	str	r1, [r0, #64]
	movne	ip, r1
	bne	.L534
.L537:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L541:
	.align	2
.L540:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	OpenUsdPool_WithMem, .-OpenUsdPool_WithMem
	.align	2
	.global	CloseUsdPool_WithMem
	.type	CloseUsdPool_WithMem, %function
CloseUsdPool_WithMem:
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
	ldr	r3, .L547
	add	r0, r3, r0, lsl #2
	ldr	r3, [r0, #64]
	cmp	r3, #0
	movne	r3, #0
	strne	r3, [r0, #64]
	ldmfd	sp, {fp, sp, pc}
.L548:
	.align	2
.L547:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	CloseUsdPool_WithMem, .-CloseUsdPool_WithMem
	.align	2
	.global	InitUsdPool
	.type	InitUsdPool, %function
InitUsdPool:
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
	ldr	r3, .L554
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #64]
	cmp	r0, #0
	beq	.L552
	add	r1, r0, #203776
	mov	ip, #0
	add	r1, r1, #512
	mov	r3, r0
	mov	r2, ip
	str	ip, [r0]
.L551:
	str	r2, [r3, #1048]
	add	r3, r3, #1056
	str	r2, [r3, #-12]
	add	r3, r3, #8
	cmp	r3, r1
	bne	.L551
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L552:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L555:
	.align	2
.L554:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	InitUsdPool, .-InitUsdPool
	.align	2
	.global	GetUsd
	.type	GetUsd, %function
GetUsd:
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
	ldr	r3, .L563
	add	r0, r3, r0, lsl #2
	ldr	ip, [r0, #64]
	cmp	ip, #0
	beq	.L560
	mov	r3, ip
	mov	r2, #0
	b	.L559
.L558:
	add	r2, r2, #1
	add	r3, r3, #1056
	cmp	r2, #192
	add	r3, r3, #8
	beq	.L562
.L559:
	ldr	r1, [r3, #1048]
	cmp	r1, #0
	bne	.L558
	ldr	r1, [r3, #1044]
	cmp	r1, #0
	bne	.L558
	movw	r0, #1064
	mov	r4, #0
	mul	r3, r0, r2
	mov	r5, #0
	mla	r2, r2, r0, r0
	add	r6, ip, r3
	add	r3, r3, #8
	add	lr, r6, #1024
	add	r0, ip, r3
	str	r1, [r6, #1044]
	add	lr, lr, #8
	strd	r4, [ip, r2]
	str	r1, [r6, #1060]
	str	r1, [r6, #1056]
	str	r1, [r6, #1052]
	strb	r1, [r6, #1034]
	strb	r1, [r6, #1035]
	strb	r1, [r6, #1036]
	strh	r1, [lr, #6]	@ movhi
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L562:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L560:
	mov	r0, ip
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L564:
	.align	2
.L563:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	GetUsd, .-GetUsd
	.align	2
	.global	FreeUsdByDec
	.type	FreeUsdByDec, %function
FreeUsdByDec:
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
	cmp	r1, #0
	movne	r3, #0
	strne	r3, [r1, #1040]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	FreeUsdByDec, .-FreeUsdByDec
	.align	2
	.global	MarkUsdByDnr
	.type	MarkUsdByDnr, %function
MarkUsdByDnr:
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
	cmp	r1, #0
	movne	r3, #1
	strne	r3, [r1, #1036]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MarkUsdByDnr, .-MarkUsdByDnr
	.align	2
	.global	UnMarkUsdByDnr
	.type	UnMarkUsdByDnr, %function
UnMarkUsdByDnr:
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
	cmp	r1, #0
	movne	r3, #0
	strne	r3, [r1, #1036]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	UnMarkUsdByDnr, .-UnMarkUsdByDnr
	.align	2
	.global	IsImageInVoQueue
	.type	IsImageInVoQueue, %function
IsImageInVoQueue:
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
	cmp	r1, #0
	beq	.L591
	ldrh	r3, [r0, #4]
	ldrh	lr, [r0, #8]
	and	r3, r3, #63
	cmp	lr, r3
	beq	.L585
.L584:
	add	r2, r0, r3, lsl #2
	add	r3, r3, #1
	and	r3, r3, #63
	ldr	r2, [r2, #12]
	cmp	r2, #0
	beq	.L583
	ldr	ip, [r2, #156]
	ldr	r2, [r1, #156]
	cmp	ip, r2
	beq	.L586
.L583:
	cmp	r3, lr
	bne	.L584
.L585:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L586:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L591:
	mov	r0, r1
	ldr	r1, .L592
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L593:
	.align	2
.L592:
	.word	.LC5
	UNWIND(.fnend)
	.size	IsImageInVoQueue, .-IsImageInVoQueue
	.align	2
	.global	IsVoQueueEmpty
	.type	IsVoQueueEmpty, %function
IsVoQueueEmpty:
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
	subs	r4, r0, #0
	beq	.L598
	ldrh	r0, [r4, #8]
	ldrh	r3, [r4, #6]
	cmp	r0, r3
	ldreqh	r3, [r4, #4]
	rsbeq	r0, r0, r3
	clzeq	r0, r0
	moveq	r0, r0, lsr #5
	movne	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L598:
	ldr	r2, .L599
	ldr	r1, .L599+4
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L600:
	.align	2
.L599:
	.word	.LANCHOR1+20
	.word	.LC32
	UNWIND(.fnend)
	.size	IsVoQueueEmpty, .-IsVoQueueEmpty
	.align	2
	.global	SetAspectRatio
	.type	SetAspectRatio, %function
SetAspectRatio:
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
	mov	r3, r0
	cmp	r1, #5
	ldrls	pc, [pc, r1, asl #2]
	b	.L602
.L604:
	.word	.L603
	.word	.L605
	.word	.L606
	.word	.L607
	.word	.L608
	.word	.L609
.L609:
	mov	r1, #0
	mov	r2, #2
	stmia	r0, {r1, r2}
	ldmfd	sp, {fp, sp, pc}
.L603:
	mov	r1, #0
	mov	r2, #1
	stmia	r0, {r1, r2}
	ldmfd	sp, {fp, sp, pc}
.L605:
	mov	r1, #4
	mov	r2, #3
	stmia	r0, {r1, r2}
	ldmfd	sp, {fp, sp, pc}
.L606:
	mov	r1, #16
	mov	r2, #9
	stmia	r0, {r1, r2}
	ldmfd	sp, {fp, sp, pc}
.L607:
	mov	r1, #221
	mov	r2, #100
	stmia	r0, {r1, r2}
	ldmfd	sp, {fp, sp, pc}
.L608:
	mov	r1, #235
	mov	r2, #100
	stmia	r0, {r1, r2}
	ldmfd	sp, {fp, sp, pc}
.L602:
	mov	r0, #1
	mov	r2, r1
	mov	ip, #0
	str	r0, [r3, #4]
	ldr	r1, .L611
	str	ip, [r3]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L612:
	.align	2
.L611:
	.word	.LC33
	UNWIND(.fnend)
	.size	SetAspectRatio, .-SetAspectRatio
	.align	2
	.global	GetVoLastImageID
	.type	GetVoLastImageID, %function
GetVoLastImageID:
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
	ldr	r0, [r0, #296]
	cmp	r0, #1
	mvnls	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	GetVoLastImageID, .-GetVoLastImageID
	.align	2
	.global	IsMain10Profile
	.type	IsMain10Profile, %function
IsMain10Profile:
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
	add	r0, r0, #81920
	ldr	r3, [r0, #3848]
	cmp	r3, #1
	beq	.L620
	ldr	r2, [r0, #3852]
	cmp	r2, #1
	beq	.L620
	cmp	r3, #2
	moveq	r3, #1
	ldrne	r3, [r0, #3856]
	subne	r3, r3, #1
	clzne	r3, r3
	movne	r3, r3, lsr #5
.L618:
	ldr	r0, [r0, #3856]
	cmp	r0, #1
	movne	r0, r3
	orreq	r0, r3, #1
	ldmfd	sp, {fp, sp, pc}
.L620:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	IsMain10Profile, .-IsMain10Profile
	.align	2
	.global	MetadataMapToSec
	.type	MetadataMapToSec, %function
MetadataMapToSec:
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
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MetadataMapToSec, .-MetadataMapToSec
	.align	2
	.global	MetadataUnMapToSec
	.type	MetadataUnMapToSec, %function
MetadataUnMapToSec:
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
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MetadataUnMapToSec, .-MetadataUnMapToSec
	.global	pstVdecUsdPool
	.global	g_DnrEnable
	.section	.rodata
	.align	2
.LANCHOR1 = . + 0
	.type	__FUNCTION__.15367, %object
	.size	__FUNCTION__.15367, 19
__FUNCTION__.15367:
	.ascii	"InsertImgToVoQueue\000"
	.space	1
	.type	__func__.15613, %object
	.size	__func__.15613, 15
__func__.15613:
	.ascii	"IsVoQueueEmpty\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"Initializing VO queue\012\000" )
	.space	1
.LC1:
	ASCII(.ascii	"VoQueue state: head(%d) history(%d) tail(%d)\000" )
	.space	3
.LC2:
	ASCII(.ascii	"\012\000" )
	.space	2
.LC3:
	ASCII(.ascii	"%d \000" )
.LC4:
	ASCII(.ascii	"ResetVoQueue but queue not empty: head(%d) history(" )
	ASCII(.ascii	"%d) tail(%d)\012\000" )
	.space	3
.LC5:
	ASCII(.ascii	"image point NULL\012\000" )
	.space	2
.LC6:
	ASCII(.ascii	"call InsertVOQueue\012\000" )
.LC7:
	ASCII(.ascii	"FATAL: p_image(id=%d) has already in the VoQueue!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC8:
	ASCII(.ascii	"error=%d,W=%d,H=%d,not acceptable!\012\000" )
.LC9:
	ASCII(.ascii	"out_pts: %lld -> %lld Usertag %lld\012\000" )
.LC10:
	ASCII(.ascii	"Queue=(%02d,%02d,%02d) + F%d\012\000" )
	.space	2
.LC11:
	ASCII(.ascii	"Queue=(%02d,%02d,%02d) - F%d\012\000" )
	.space	2
.LC12:
	ASCII(.ascii	"SYNTAX_ReleaseImage p_image is null\012\000" )
	.space	3
.LC13:
	ASCII(.ascii	"Image %d not found in vo queue.\012\000" )
	.space	3
.LC14:
	ASCII(.ascii	"image[%d] null\012\000" )
.LC15:
	ASCII(.ascii	"Queue=(%02d,%02d,%02d) x F%d\012\000" )
	.space	2
.LC16:
	ASCII(.ascii	"pSegMan error!\012\000" )
.LC17:
	ASCII(.ascii	"More than two packet!!!ERROR!!!\012\000" )
	.space	3
.LC18:
	ASCII(.ascii	"*****************copy packets tobe one*************" )
	ASCII(.ascii	"****\012\000" )
	.space	3
.LC19:
	ASCII(.ascii	"*****************two packet*****************\012\000" )
	.space	2
.LC20:
	ASCII(.ascii	"pSegMan->CurrSeg.IsLastSeg=%d\012\000" )
	.space	1
.LC21:
	ASCII(.ascii	"pSegMan->CurrSeg.PhyAddr=%#x\012\000" )
	.space	2
.LC22:
	ASCII(.ascii	"pSegMan->CurrSeg.LenInByte=%#x\012\000" )
.LC23:
	ASCII(.ascii	"ERROR: Wrong PreParseInfo about Vc1 IsAdvProfile!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC24:
	ASCII(.ascii	"ERROR: Wrong PreParseInfo about Vc1 CodecVersion!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC25:
	ASCII(.ascii	"LENTH ERROR: VC1 S/MP BS is wrong!\012\000" )
.LC26:
	ASCII(.ascii	"SYNTAX ERROR: VC1 S/MP BS is wrong!\012\000" )
	.space	3
.LC27:
	ASCII(.ascii	"ERROR: VFMW_VP8 one packet stream, but IsLastSeg ==" )
	ASCII(.ascii	" 0\012\000" )
	.space	1
.LC28:
	ASCII(.ascii	"ERROR: VFMW_VP6 one packet stream, but IsLastSeg ==" )
	ASCII(.ascii	" 0\012\000" )
	.space	1
.LC29:
	ASCII(.ascii	"ERROR: VFMW_VP9 one packet stream, but IsLastSeg ==" )
	ASCII(.ascii	" 0\012\000" )
	.space	1
.LC30:
	ASCII(.ascii	"VidStd=%d Error!\012\000" )
	.space	2
.LC31:
	ASCII(.ascii	"pu8[0] = %x,pu8[1] = %x, pu8[2] = %x\012\000" )
	.space	2
.LC32:
	ASCII(.ascii	"%s pstVoQue NULL\012\000" )
	.space	2
.LC33:
	ASCII(.ascii	"WARNNING: aspect ration (%d) is not expected!\012\000" )
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	last_rec_pos_time.15359, %object
	.size	last_rec_pos_time.15359, 64
last_rec_pos_time.15359:
	.space	64
	.type	pstVdecUsdPool, %object
	.size	pstVdecUsdPool, 64
pstVdecUsdPool:
	.space	64
	.type	g_DnrEnable, %object
	.size	g_DnrEnable, 4
g_DnrEnable:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
