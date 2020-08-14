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
	ldr	r3, .L2
	mov	r4, r0
	ldr	r1, .L2+4
	mov	r0, #12
	ldr	r3, [r3, #68]
	blx	r3
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
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	mov	r5, r0
	ldrh	lr, [r5, #8]
	ldr	r6, .L10
	ldrh	r3, [r0, #4]
	mov	r0, #1
	ldrh	r2, [r5, #6]
	ldr	ip, [r6, #68]
	ldr	r1, .L10+4
	str	lr, [sp]
	blx	ip
	ldrh	r3, [r5, #4]
	ldrh	r2, [r5, #8]
	and	r4, r3, #63
	cmp	r2, r4
	bne	.L8
	b	.L7
.L6:
	add	r2, r5, r4, lsl #2
	ldr	r3, [r6, #68]
	ldr	r1, .L10+8
	mov	r0, #1
	ldr	r2, [r2, #12]
	ldr	r2, [r2, #196]
	blx	r3
	add	r3, r4, #1
	ldrh	r2, [r5, #8]
	and	r4, r3, #63
	cmp	r2, r4
	beq	.L7
.L8:
	tst	r3, #7
	bne	.L6
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L10+12
	blx	r3
	b	.L6
.L7:
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L10+12
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	bx	r3
.L11:
	.align	2
.L10:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	ip, [r5, #68]
	ldr	r1, .L17+4
	blx	ip
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
	cmp	r0, #19
	ldrls	pc, [pc, r0, asl #2]
	b	.L19
.L22:
	.word	.L21
	.word	.L23
	.word	.L24
	.word	.L25
	.word	.L26
	.word	.L27
	.word	.L28
	.word	.L19
	.word	.L29
	.word	.L30
	.word	.L31
	.word	.L31
	.word	.L31
	.word	.L32
	.word	.L19
	.word	.L26
	.word	.L33
	.word	.L34
	.word	.L19
	.word	.L35
.L35:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	USERDEC_RecycleImage
.L21:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_RecycleImage
.L23:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VC1DEC_RecycleImage
.L24:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_RecycleImage
.L25:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG2DEC_RecycleImage
.L26:
	ldr	r1, .L39
	ldr	ip, [r1, #16]
	cmp	ip, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L27:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	DIVX3DEC_RecycleImage
.L28:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	AVSDEC_RecycleImage
.L29:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL8DEC_RecycleImage
.L30:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL9DEC_RecycleImage
.L31:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP6DEC_RecycleImage
.L32:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP8DEC_RecycleImage
.L33:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MVC_DEC_RecycleImage
.L34:
	ldr	r1, [r2, #196]
	mov	r0, r3
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVC_DECRecycleImage
.L19:
	ldmfd	sp, {fp, sp, pc}
.L40:
	.align	2
.L39:
	.word	g_SdecFunc
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
	cmp	r2, #19
	mov	r3, r1
	ldrd	r6, [r1, #8]
	beq	.L42
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
	beq	.L52
	cmp	r2, #1
	mvnhi	r6, #0
	mvnhi	r7, #0
	bls	.L44
.L42:
	mov	r0, r6
	mov	r1, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L52:
	cmp	r2, #1
	bhi	.L42
	ldr	r2, [r3, #160]
	and	r2, r2, #3
	cmp	r2, #2
	bne	.L42
.L44:
	mvn	r1, #0
	str	r1, [r3]
	mov	r6, r1
	mov	r7, r1
	str	r1, [r3, #4]
	b	.L42
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
	mov	r7, r0
	str	r1, [fp, #-56]
	cmp	r5, #0
	mov	r10, r2
	mov	r4, r3
	beq	.L119
	ldr	r6, .L125
	mov	r0, #2
	ldr	r1, .L125+4
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r6, #12]
	mov	r0, #2
	blx	r3
	ldrh	r9, [r4, #4]
	ldrh	r3, [r4, #8]
	and	r9, r9, #63
	cmp	r3, r9
	ldrne	r8, [fp, #-56]
	bne	.L61
	b	.L60
.L59:
	add	r9, r9, #1
	ldrh	r3, [r4, #8]
	and	r9, r9, #63
	cmp	r3, r9
	beq	.L120
.L61:
	add	r3, r4, r9, lsl #2
	ldr	r3, [r3, #12]
	cmp	r3, #0
	beq	.L59
	ldr	r2, [r3, #196]
	ldr	r3, [r5, #196]
	cmp	r2, r3
	bne	.L59
	ldr	r3, [r6, #68]
	mov	r0, #0
	ldr	r1, .L125+8
	blx	r3
	mov	r3, #1000
	str	r3, [r5, #200]
	b	.L59
.L120:
	str	r8, [fp, #-56]
.L60:
	ldr	r2, [r5, #200]
	cmp	r2, #100
	bhi	.L57
	ldr	r3, [r5, #164]
	cmp	r3, #31
	bhi	.L121
.L57:
	ldr	r3, [r5, #164]
	ldr	r1, [r5, #168]
.L62:
	str	r1, [sp]
	mov	r0, #0
	ldr	ip, [r6, #68]
	ldr	r1, .L125+12
	blx	ip
	ldr	r3, [r5, #200]
	cmp	r3, #1000
	beq	.L64
	mov	r2, r5
	mov	r1, r10
	ldr	r0, [fp, #-56]
	bl	ReturnImageToSyntax
.L64:
	ldr	r3, [r6, #16]
	mov	r0, #2
	blx	r3
	mov	r0, #1
.L55:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L121:
	ldr	r1, [r5, #168]
	cmp	r1, #31
	bls	.L62
	ldrd	r8, [r5, #8]
	mov	r1, r5
	ldr	r2, [fp, #-56]
	mov	r0, r4
	strd	r8, [r5]
	bl	ProduceTimeStamp
	ldrd	r2, [r5]
	mov	r8, r0
	mov	r9, r1
	ldrd	r0, [r5, #16]
	strd	r8, [r5, #8]
	strd	r8, [sp]
	strd	r0, [fp, #-68]
	mov	r0, #29
	ldrd	r8, [fp, #-68]
	ldr	ip, [r6, #68]
	ldr	r1, .L125+16
	strd	r8, [sp, #8]
	blx	ip
	ldr	r3, [r5, #172]
	cmp	r3, #0
	beq	.L65
	ldr	r3, [r5, #176]
	cmp	r3, #0
	bne	.L122
.L65:
	ldr	r2, [r5, #164]
	ldr	r3, [r5, #168]
	mov	r1, r2, lsr #1
	str	r2, [r5, #172]
	str	r3, [r5, #176]
	mov	r3, r3, lsr #1
	str	r1, [r5, #180]
	str	r3, [r5, #184]
.L66:
	str	r2, [r4, #300]
	ldr	r3, [r5, #168]
	ldr	r2, [r4, #280]
	str	r3, [r4, #304]
	ldr	r3, [r5, #160]
	ubfx	r3, r3, #5, #3
	str	r3, [r4, #308]
	ldr	r3, [r5, #160]
	str	r3, [r4, #312]
	ldr	r3, [r5, #204]
	ldr	r8, [r5, #160]
	cmp	r3, r2
	ldr	r9, [r5, #176]
	ldr	r1, [r5, #180]
	ubfx	r10, r8, #14, #3
	ldrh	r0, [r5, #172]
	ubfx	r8, r8, #8, #2
	ldrh	r2, [r5, #184]
	strne	r3, [r4, #320]
	orr	r9, r0, r9, asl #16
	ldr	r3, [r4, #316]
	orr	r2, r2, r1, asl #16
	str	r2, [fp, #-68]
	add	r2, r3, #1
	str	r2, [r4, #316]
	str	r3, [r5, #208]
	ldr	r3, [r4, #272]
	cmp	r3, r10
	ldreq	r3, .L125+20
	streq	r3, [fp, #-56]
	beq	.L68
	ldr	r2, .L125+20
	ldr	ip, [r2]
	str	r2, [fp, #-56]
	cmp	ip, #0
	beq	.L69
	strb	r3, [fp, #-52]
	sub	r2, fp, #52
	mov	r3, #2
	strb	r10, [fp, #-51]
	mov	r1, #7
	mov	r0, r7
	blx	ip
.L69:
	str	r10, [r4, #272]
.L68:
	ldr	r3, [r4, #276]
	cmp	r3, r8
	beq	.L70
	ldr	r3, [fp, #-56]
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L71
	sub	r2, fp, #44
	mov	r3, #1
	mov	r1, #4
	mov	r0, r7
	strb	r8, [r2, #-8]!
	blx	ip
.L71:
	str	r8, [r4, #276]
.L70:
	ldr	r3, [r4, #288]
	cmp	r3, r9
	beq	.L123
.L72:
	ldr	r3, [fp, #-56]
	str	r9, [r4, #288]
	ldr	ip, [r3]
	ldr	r3, [fp, #-68]
	cmp	ip, #0
	str	r3, [r4, #292]
	beq	.L73
	ldr	r9, [r5, #172]
	mov	r3, #8
	ldr	r8, [r5, #176]
	sub	r2, fp, #52
	ldr	lr, [r5, #180]
	mov	r1, #1
	ldr	r10, [r5, #184]
	mov	r0, r7
	strh	r9, [fp, #-52]	@ movhi
	strh	r8, [fp, #-50]	@ movhi
	strh	lr, [fp, #-48]	@ movhi
	strh	r10, [fp, #-46]	@ movhi
	blx	ip
.L73:
	ldr	r1, [r5, #108]
	mov	r0, #12
	ldr	r2, [r5, #104]
	mov	r9, #300
	ldr	r3, [r5, #192]
	str	r1, [r5, #92]
	str	r2, [r5, #88]
	add	r2, r3, r2
	ldr	ip, [r5, #196]
	add	r3, r1, r3
	str	r2, [r5, #96]
	str	r3, [r5, #100]
	ldrh	r1, [r4, #8]
	ldrh	r3, [r4, #6]
	ldrh	r2, [r4, #4]
	ldr	r8, .L125+24
	str	ip, [sp, #4]
	ldr	ip, [r6, #68]
	str	r1, [sp]
	ldr	r1, .L125+28
	blx	ip
	ldrh	r3, [r4, #8]
	mla	r9, r9, r7, r8
	ldr	r10, .L125+32
	add	r2, r4, r3, lsl #2
	add	r3, r3, #1
	and	r3, r3, #63
	mov	r1, #5
	str	r5, [r2, #12]
	mov	r0, r7
	strh	r3, [r4, #8]	@ movhi
	ldr	r3, [r5, #148]
	ldr	r2, [r10, r7, asl #2]
	str	r3, [r9, #20]
	ldr	r3, [r5, #196]
	add	r3, r3, #2
	str	r3, [r4, #296]
	bl	VDEC_Lowdelay_Event_Time
	ldr	r3, [r10, r7, asl #2]
	ldr	r2, [r6, #16]
	mov	r0, #2
	add	r3, r3, #1
	str	r3, [r10, r7, asl #2]
	blx	r2
	ldr	r3, .L125+36
	ldr	r3, [r3, r7, asl #2]
	ldr	r3, [r3, #128]
	cmp	r3, #0
	beq	.L75
	ldr	r3, [r9, #8]
	cmp	r3, #0
	movne	r2, #100
	mulne	r3, r2, r3
	strne	r3, [r5, #188]
.L75:
	ldr	r3, [fp, #-56]
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L76
	mov	r2, r5
	mov	r3, #504
	mov	r1, #5
	mov	r0, r7
	blx	ip
.L76:
	mov	r0, #8
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	bne	.L124
.L78:
	mov	r4, #300
	ldr	r2, [r6]
	mul	r4, r4, r7
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
	bcc	.L55
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq	.L80
	movw	r0, #10000
	mul	r0, r0, r3
	bl	__aeabi_uidiv
	str	r0, [r5, #8]
.L80:
	mov	r3, #300
	mov	r2, #0
	mul	r7, r3, r7
	mov	r0, #1
	add	r3, r8, r7
	str	r6, [r8, r7]
	str	r2, [r3, #4]
	b	.L55
.L122:
	ldr	r2, [fp, #-56]
	sub	r3, r2, #16
	cmp	r2, #0
	cmpne	r3, #1
	ldrls	r2, [r5, #164]
	bhi	.L65
	b	.L66
.L124:
	ldr	r3, [r6]
	blx	r3
	ldr	ip, .L125+40
	ldr	r2, .L125+44
	ldr	r3, [ip, r7, asl #2]
	ldr	r2, [r2]
	rsb	r3, r3, r0
	mov	lr, r0
	cmp	r3, r2
	bls	.L78
	mov	r2, r7, asl #24
	movw	r1, #589
	add	r2, r2, #524288
	ldr	r0, .L125+48
	str	lr, [ip, r7, asl #2]
	bl	DBG_AddTrace
	b	.L78
.L123:
	ldr	r3, [r4, #292]
	ldr	r2, [fp, #-68]
	cmp	r3, r2
	bne	.L72
	b	.L73
.L119:
	ldr	r3, .L125
	mov	r0, r5
	ldr	r1, .L125+52
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #1
	b	.L55
.L126:
	.align	2
.L125:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	g_event_report
	.word	g_VfmwChanStat
	.word	.LC10
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
	beq	.L153
	ldr	r3, [r0]
	cmp	r3, #19
	ldrls	pc, [pc, r3, asl #2]
	b	.L153
.L136:
	.word	.L149
	.word	.L137
	.word	.L138
	.word	.L139
	.word	.L140
	.word	.L141
	.word	.L142
	.word	.L153
	.word	.L143
	.word	.L144
	.word	.L145
	.word	.L145
	.word	.L145
	.word	.L146
	.word	.L153
	.word	.L140
	.word	.L147
	.word	.L148
	.word	.L153
	.word	.L149
.L149:
	add	r4, r0, #16
.L150:
	ldr	r6, .L155
	mov	r0, #2
	ldr	r3, [r6, #12]
	blx	r3
	ldrh	r3, [r4, #6]
	ldrh	r2, [r4, #8]
	cmp	r2, r3
	mvneq	r5, #2
	beq	.L151
	add	r3, r4, r3, lsl #2
	ldr	ip, [r6, #52]
	mov	r2, #504
	mov	r0, r7
	ldr	r1, [r3, #12]
	mov	r5, #1
	blx	ip
	ldr	r1, [r7, #196]
	ldrh	r3, [r4, #6]
	mov	r0, #12
	ldrh	r2, [r4, #4]
	str	r1, [sp, #4]
	ldrh	lr, [r4, #8]
	ldr	ip, [r6, #68]
	ldr	r1, .L155+4
	str	lr, [sp]
	blx	ip
	ldr	r3, [r7, #196]
	ldr	r2, [r4, #344]
	add	r3, r3, r2
	str	r3, [r7, #196]
	ldrh	r3, [r4, #6]
	add	r3, r3, r5
	and	r3, r3, #63
	strh	r3, [r4, #6]	@ movhi
.L151:
	ldr	r3, [r6, #16]
	mov	r0, #2
	blx	r3
	mov	r0, r5
.L134:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L137:
	add	r4, r0, #48384
	add	r4, r4, #104
	b	.L150
.L138:
	add	r4, r0, #22528
	add	r4, r4, #88
	b	.L150
.L139:
	add	r4, r0, #79872
	add	r4, r4, #648
	b	.L150
.L140:
	add	r4, r0, #25856
	add	r4, r4, #16
	b	.L150
.L141:
	add	r4, r0, #33792
	add	r4, r4, #24
	b	.L150
.L142:
	add	r4, r0, #16192
	add	r4, r4, #24
	b	.L150
.L143:
	add	r4, r0, #46080
	add	r4, r4, #64
	b	.L150
.L144:
	add	r4, r0, #46080
	add	r4, r4, #48
	b	.L150
.L145:
	add	r4, r0, #12608
	add	r4, r4, #48
	b	.L150
.L146:
	add	r4, r0, #8128
	add	r4, r4, #48
	b	.L150
.L147:
	add	r4, r0, #592
	b	.L150
.L148:
	add	r4, r0, #50944
	add	r4, r4, #64
	b	.L150
.L153:
	mvn	r0, #1
	b	.L134
.L156:
	.align	2
.L155:
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	subs	r6, r1, #0
	mov	r8, r0
	beq	.L247
	ldr	r3, [r0]
	cmp	r3, #19
	ldrls	pc, [pc, r3, asl #2]
	b	.L202
.L161:
	.word	.L174
	.word	.L162
	.word	.L163
	.word	.L164
	.word	.L165
	.word	.L166
	.word	.L167
	.word	.L202
	.word	.L168
	.word	.L169
	.word	.L170
	.word	.L170
	.word	.L170
	.word	.L171
	.word	.L202
	.word	.L165
	.word	.L172
	.word	.L173
	.word	.L202
	.word	.L174
.L174:
	add	r5, r0, #16
.L175:
	ldr	r7, .L249
	mov	r0, #2
	ldr	r3, [r7, #12]
	blx	r3
	ldr	r3, [r6, #196]
	ldr	r2, [r5, #344]
	rsb	r2, r2, r3
	str	r2, [r6, #196]
	ldrh	r4, [r5, #4]
	ldrh	r3, [r5, #6]
	and	r4, r4, #63
	cmp	r3, r4
	bne	.L200
	b	.L201
.L180:
	add	r4, r4, #1
	ldrh	r3, [r5, #6]
	and	r4, r4, #63
	cmp	r3, r4
	beq	.L248
.L200:
	add	r9, r4, #2
	add	r3, r5, r9, lsl #2
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L178
	ldr	r3, [r3, #196]
	ldr	r0, [r6, #196]
	rsb	r0, r3, r0
	clz	r0, r0
	mov	r0, r0, lsr #5
	cmp	r0, #0
	beq	.L180
.L179:
	ldrh	r2, [r5, #4]
	b	.L182
.L204:
	mov	r4, ip
.L182:
	cmp	r4, #0
	mov	ip, #63
	subne	ip, r4, #1
	add	r1, r5, r9, lsl #2
	add	r3, ip, #2
	cmp	r2, r4
	add	lr, r5, r3, lsl #2
	mov	r9, r3
	ldr	r3, [lr, #4]
	str	r3, [r1, #4]
	bne	.L204
	cmp	r0, #1
	add	r2, r2, #1
	and	r2, r2, #63
	strh	r2, [r5, #4]	@ movhi
	bne	.L180
	ldr	r1, [r6, #196]
	mov	r0, #12
	ldrh	lr, [r5, #8]
	ldrh	r3, [r5, #6]
	ldr	ip, [r7, #68]
	str	r1, [sp, #4]
	str	lr, [sp]
	ldr	r1, .L249+4
	blx	ip
	ldr	r3, [r8]
	cmp	r3, #19
	ldrls	pc, [pc, r3, asl #2]
	b	.L205
.L184:
	.word	.L183
	.word	.L185
	.word	.L186
	.word	.L187
	.word	.L188
	.word	.L189
	.word	.L190
	.word	.L205
	.word	.L191
	.word	.L192
	.word	.L193
	.word	.L193
	.word	.L193
	.word	.L194
	.word	.L205
	.word	.L188
	.word	.L195
	.word	.L196
	.word	.L205
	.word	.L197
.L162:
	add	r5, r0, #48384
	add	r5, r5, #104
	b	.L175
.L163:
	add	r5, r0, #22528
	add	r5, r5, #88
	b	.L175
.L164:
	add	r5, r0, #79872
	add	r5, r5, #648
	b	.L175
.L165:
	add	r5, r0, #25856
	add	r5, r5, #16
	b	.L175
.L166:
	add	r5, r0, #33792
	add	r5, r5, #24
	b	.L175
.L167:
	add	r5, r0, #16192
	add	r5, r5, #24
	b	.L175
.L168:
	add	r5, r0, #46080
	add	r5, r5, #64
	b	.L175
.L169:
	add	r5, r0, #46080
	add	r5, r5, #48
	b	.L175
.L170:
	add	r5, r0, #12608
	add	r5, r5, #48
	b	.L175
.L171:
	add	r5, r0, #8128
	add	r5, r5, #48
	b	.L175
.L172:
	add	r5, r0, #592
	b	.L175
.L173:
	add	r5, r0, #50944
	add	r5, r5, #64
	b	.L175
.L178:
	ldr	r3, [r7, #68]
	mov	r2, r4
	ldr	r1, .L249+8
	mov	r0, #1
	blx	r3
	mov	r0, #2
	b	.L179
.L248:
	ldr	r2, [r6, #196]
.L201:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L249+12
	mvn	r4, #1
	blx	r3
.L177:
	ldr	r3, [r7, #16]
	mov	r0, #2
	blx	r3
	mov	r0, r4
.L159:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L202:
	mvn	r0, #1
	b	.L159
.L197:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	USERDEC_RecycleImage
	cmp	r0, #1
	beq	.L199
.L206:
	mvn	r4, #19
	b	.L177
.L196:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	HEVC_DECRecycleImage
	cmp	r0, #0
	bne	.L206
.L199:
	mov	r4, #1
	b	.L177
.L195:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	MVC_DEC_RecycleImage
	cmp	r0, #0
	beq	.L199
	b	.L206
.L194:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	VP8DEC_RecycleImage
	cmp	r0, #1
	bne	.L206
	b	.L199
.L193:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	VP6DEC_RecycleImage
	cmp	r0, #1
	bne	.L206
	b	.L199
.L192:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	REAL9DEC_RecycleImage
	cmp	r0, #1
	bne	.L206
	b	.L199
.L191:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	REAL8DEC_RecycleImage
	cmp	r0, #1
	bne	.L206
	b	.L199
.L190:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	AVSDEC_RecycleImage
	cmp	r0, #1
	bne	.L206
	b	.L199
.L189:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	DIVX3DEC_RecycleImage
	cmp	r0, #1
	bne	.L206
	b	.L199
.L188:
	ldr	r3, .L249+16
	ldr	r3, [r3, #16]
	cmp	r3, #0
	beq	.L206
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	blx	r3
	cmp	r0, #0
	beq	.L199
	b	.L206
.L187:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	MPEG2DEC_RecycleImage
	cmp	r0, #1
	bne	.L206
	b	.L199
.L186:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	MPEG4DEC_RecycleImage
	cmp	r0, #1
	bne	.L206
	b	.L199
.L185:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	VC1DEC_RecycleImage
	cmp	r0, #1
	bne	.L206
	b	.L199
.L183:
	ldr	r1, [r6, #196]
	add	r0, r8, #8
	bl	H264DEC_RecycleImage
	cmp	r0, #0
	bne	.L206
	b	.L199
.L205:
	mvn	r4, #1
	b	.L177
.L247:
	ldr	r3, .L249
	mov	r0, r6
	ldr	r1, .L249+20
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #1
	b	.L159
.L250:
	.align	2
.L249:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC15
	.word	.LC14
	.word	.LC13
	.word	g_SdecFunc
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
	cmp	r3, #19
	ldrls	pc, [pc, r3, asl #2]
	b	.L269
.L254:
	.word	.L267
	.word	.L255
	.word	.L256
	.word	.L257
	.word	.L258
	.word	.L259
	.word	.L260
	.word	.L269
	.word	.L261
	.word	.L262
	.word	.L263
	.word	.L263
	.word	.L263
	.word	.L264
	.word	.L269
	.word	.L258
	.word	.L265
	.word	.L266
	.word	.L269
	.word	.L267
.L267:
	add	r4, r0, #16
.L268:
	ldr	r5, .L270
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
.L255:
	add	r4, r0, #48384
	add	r4, r4, #104
	b	.L268
.L256:
	add	r4, r0, #22528
	add	r4, r4, #88
	b	.L268
.L257:
	add	r4, r0, #79872
	add	r4, r4, #648
	b	.L268
.L258:
	add	r4, r0, #25856
	add	r4, r4, #16
	b	.L268
.L259:
	add	r4, r0, #33792
	add	r4, r4, #24
	b	.L268
.L260:
	add	r4, r0, #16192
	add	r4, r4, #24
	b	.L268
.L261:
	add	r4, r0, #46080
	add	r4, r4, #64
	b	.L268
.L262:
	add	r4, r0, #46080
	add	r4, r4, #48
	b	.L268
.L263:
	add	r4, r0, #12608
	add	r4, r4, #48
	b	.L268
.L264:
	add	r4, r0, #8128
	add	r4, r4, #48
	b	.L268
.L265:
	add	r4, r0, #592
	b	.L268
.L266:
	add	r4, r0, #50944
	add	r4, r4, #64
	b	.L268
.L269:
	mvn	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L271:
	.align	2
.L270:
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
	beq	.L273
	ldr	r3, [r4, #8]
	cmp	r3, #0
	beq	.L273
	ldr	r3, .L299
	ldr	r3, [r3, r0, asl #2]
	ldr	r2, [r3, #1388]
	cmp	r2, #1
	beq	.L295
.L276:
	ldrb	r5, [r4, #13]	@ zero_extendqisi2
	mov	r7, #0
	strb	r7, [r4, #408]
	cmp	r5, #1
	beq	.L296
	cmp	r5, #0
	beq	.L297
.L281:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L297:
	ldr	r9, .L299+4
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
	bls	.L281
	ldr	r1, [r4, #104]
	mov	r0, r6
	bl	SM_ReleaseStreamSeg
	ldr	r1, [r4, #184]
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
	ldr	r3, [r9, #68]
	ldr	r1, .L299+8
	mov	r0, #1
	blx	r3
	ldr	r3, .L299+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L280
	mov	r0, r6
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L280:
	mov	r3, #0
	mvn	r0, #0
	strb	r3, [r4, #2]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L295:
	ldr	r2, [r3, #1428]
	cmp	r2, #1
	bne	.L276
	ldrb	r2, [r4, #408]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L276
	mov	r2, #160
	add	r1, r4, #248
	add	r0, r4, #88
	bl	memcpy
	mov	r3, #0
	mov	r0, r3
	strb	r3, [r4, #408]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L296:
	ldrb	r5, [r4, #2]	@ zero_extendqisi2
	cmp	r5, r7
	beq	.L298
	cmp	r5, #1
	bne	.L281
	ldr	r1, [r3, #528]
	mov	r3, #64512
	ldr	r2, [r4, #100]
	movt	r3, 15
	cmp	r1, #6
	movne	r3, #1024
	cmp	r3, r2
	ble	.L283
	ldr	r8, .L299+4
	mov	r0, #22
	ldr	r1, .L299+16
	ldr	r3, [r8, #68]
	blx	r3
	ldr	r2, [r4, #100]
	ldr	r0, [r4, #8]
	ldr	r3, [r8, #52]
	rsb	r0, r2, r0
	ldr	r1, [r4, #88]
	blx	r3
	ldr	r3, [r4, #100]
	ldr	ip, [r4, #20]
	mov	r0, r6
	ldr	r1, [r4, #8]
	ldr	r2, [r4, #16]
	rsb	r1, r3, r1
	strb	r5, [r4, #93]
	rsb	r2, r3, r2
	str	r1, [r4, #88]
	add	r3, r3, ip
	str	r2, [r4, #96]
	str	r3, [r4, #100]
	ldr	r1, [r4, #104]
	bl	SM_ReleaseStreamSeg
	ldr	ip, [r4, #24]
	ldr	r3, [r8, #48]
	mov	r1, r7
	mov	r2, #80
	add	r0, r4, #168
	str	ip, [r4, #104]
	blx	r3
.L284:
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
.L298:
	ldr	r6, .L299+4
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
.L283:
	ldr	r5, .L299+4
	mov	r0, #22
	ldr	r1, .L299+20
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r3, [r5, #52]
	mov	r2, #80
	add	r1, r4, #8
	add	r0, r4, #168
	blx	r3
	b	.L284
.L273:
	ldr	r3, .L299+4
	mov	r0, #1
	ldr	r1, .L299+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L300:
	.align	2
.L299:
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #32)
	sub	sp, sp, #32
	mov	r4, r0
	mov	r5, r2
	cmp	r1, #17
	ldrls	pc, [pc, r1, asl #2]
	b	.L301
.L304:
	.word	.L303
	.word	.L305
	.word	.L306
	.word	.L307
	.word	.L308
	.word	.L309
	.word	.L310
	.word	.L301
	.word	.L311
	.word	.L312
	.word	.L313
	.word	.L313
	.word	.L313
	.word	.L314
	.word	.L301
	.word	.L308
	.word	.L315
	.word	.L316
.L305:
	ldr	r6, .L392
	mov	r0, #22
	ldrb	r2, [r4, #13]	@ zero_extendqisi2
	ldr	r1, .L392+4
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r6, #68]
	ldr	r2, [r4, #16]
	mov	r0, #22
	ldr	r1, .L392+8
	blx	r3
	ldr	r3, [r6, #68]
	ldr	r2, [r4, #20]
	mov	r0, #22
	ldr	r1, .L392+12
	blx	r3
	ldr	r2, [r5]
	ldr	r3, [r2, #548]
	cmp	r3, #1
	bhi	.L375
	ldr	r1, [r2, #552]
	cmp	r1, #8
	bgt	.L376
	add	r7, r5, #12288
	uxtb	r3, r3
	cmp	r3, #1
	strb	r3, [r7, #3520]
	ldr	r3, [r2, #552]
	strb	r3, [r7, #3522]
	beq	.L377
	ldrb	r3, [r4, #14]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L378
	add	r7, r4, #88
	ldr	r3, [r6, #52]
	mov	r2, #80
	add	r1, r4, #8
	mov	r0, r7
	blx	r3
	ldr	r3, [r6, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	ldr	r3, [r4, #100]
	cmp	r3, #15
	bgt	.L332
	ldr	r3, [r6, #68]
	add	r5, r5, #45056
	ldr	r1, .L392+16
.L374:
	mov	r0, #0
	blx	r3
	ldr	r1, [r4, #104]
	ldr	r0, [r5, #3776]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L392+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L301
	mov	r3, #0
	ldr	r0, [r5, #3776]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L301:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L316:
	ldrd	r2, [r0, #32]
	sub	r1, fp, #68
	ldr	r9, [r0, #8]
	mov	r0, r5
	ldr	r8, [r4, #16]
	ldr	r7, [r4, #20]
	ldrb	r6, [r4, #13]	@ zero_extendqisi2
	ldr	lr, [r4, #24]
	ldrb	ip, [r4, #15]	@ zero_extendqisi2
	strd	r2, [fp, #-44]
	str	r9, [fp, #-64]
	str	r8, [fp, #-60]
	str	r7, [fp, #-56]
	strb	r6, [fp, #-68]
	str	lr, [fp, #-52]
	strb	ip, [fp, #-67]
	bl	HEVCDEC_DecodePacket
	cmn	r0, #2
	moveq	r3, #1
	movne	r3, #0
	strb	r3, [r4, #1]
	ldr	r4, [r5, #2312]
	cmp	r4, #0
	bne	.L301
	ldrb	r3, [r5, #6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L301
	ldr	r0, [r5, #244]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r5, #6]
	b	.L301
.L303:
	ldrd	r2, [r0, #32]
	sub	r1, fp, #68
	ldr	r9, [r0, #8]
	mov	r0, r5
	ldr	r8, [r4, #16]
	ldr	r7, [r4, #20]
	ldrb	r6, [r4, #13]	@ zero_extendqisi2
	ldr	lr, [r4, #24]
	ldrb	ip, [r4, #15]	@ zero_extendqisi2
	strd	r2, [fp, #-44]
	str	r9, [fp, #-64]
	str	r8, [fp, #-60]
	str	r7, [fp, #-56]
	strb	r6, [fp, #-68]
	str	lr, [fp, #-52]
	strb	ip, [fp, #-67]
	bl	H264DEC_DecodePacket
	cmn	r0, #2
	moveq	r3, #1
	movne	r3, #0
	strb	r3, [r4, #1]
	ldr	r4, [r5, #360]
	cmp	r4, #0
	bne	.L301
	add	r6, r5, #12288
	ldrb	r3, [r6, #1272]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L301
	add	r5, r5, #53248
	ldr	r0, [r5, #4056]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r6, #1272]
	b	.L301
.L306:
	add	r6, r2, #20480
	mov	r0, r2
	ldr	r7, [r6, #2480]
	bl	MP4_GetPacketState
	cmp	r0, #3
	beq	.L379
.L338:
	mov	r1, r4
	ldr	r0, [r6, #2480]
	bl	SegMan
	cmp	r0, #0
	beq	.L380
.L340:
	ldr	r4, [r5, #1036]
	cmp	r4, #0
	bne	.L301
	add	r5, r5, #53248
	ldrb	r3, [r5, #3560]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L301
	ldr	r0, [r6, #2480]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r5, #3560]
	b	.L301
.L307:
	add	r6, r2, #110592
	mov	r0, r2
	ldr	r7, [r6, #4080]
	bl	MP2_GetPacketState
	cmp	r0, #2
	beq	.L381
.L334:
	mov	r1, r4
	ldr	r0, [r6, #4080]
	bl	SegMan
	cmp	r0, #0
	beq	.L382
.L336:
	ldr	r4, [r5, #4]
	cmp	r4, #0
	bne	.L301
	add	r5, r5, #114688
	ldrb	r3, [r5, #2]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L301
	ldr	r0, [r6, #4080]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r5, #2]
	b	.L301
.L308:
	ldr	r3, .L392
	add	r6, r0, #88
	mov	r1, #1
	mov	r2, #80
	strb	r1, [r0, #1]
	mov	r0, r6
	ldr	r3, [r3, #52]
	add	r1, r4, #8
	blx	r3
	ldr	r3, .L392+24
	ldr	r3, [r3, #12]
	cmp	r3, #0
	beq	.L350
	mov	r1, r6
	mov	r0, r5
	blx	r3
	cmn	r0, #2
	beq	.L301
.L350:
	ldr	r0, [r5]
	ldr	r1, [r4, #24]
	bl	SM_ReleaseStreamSeg
	mov	r3, #0
	strb	r3, [r4, #1]
	b	.L301
.L309:
	ldrb	r3, [r0, #14]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L383
	ldr	r7, .L392
	add	r6, r0, #88
	add	r1, r0, #8
	mov	r2, #80
	mov	r0, r6
	ldr	r3, [r7, #52]
	blx	r3
	ldr	r3, [r7, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	mov	r1, r6
	mov	r0, r5
	bl	DIVX3DEC_Decode
	ldr	r8, [r5, #4]
	cmp	r8, #0
	bne	.L301
	add	r5, r5, #65536
	ldr	r3, [r7, #48]
	mov	r2, #160
	mov	r1, r8
	mov	r0, r6
	blx	r3
	ldr	r1, [r4, #24]
	ldr	r0, [r5, #2432]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L392+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L301
	ldr	r0, [r5, #2432]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	blx	ip
	b	.L301
.L310:
	add	r6, r2, #16384
	mov	r0, r2
	ldr	r7, [r6, #344]
	bl	AVS_GetPacketState
	cmp	r0, #0
	bne	.L341
	ldr	r3, .L392+28
	ldr	r3, [r3, r7, asl #2]
	ldr	r2, [r3, #1388]
	cmp	r2, #1
	bne	.L342
	ldr	r3, [r3, #1428]
	cmp	r3, #0
	bne	.L341
.L342:
	add	r1, r5, #16384
	mov	r0, r5
	add	r1, r1, #184
	bl	AVSDEC_Decode
.L341:
	mov	r1, r4
	ldr	r0, [r6, #344]
	bl	SegMan
	cmp	r0, #0
	beq	.L384
.L343:
	ldr	r4, [r5, #4]
	cmp	r4, #0
	bne	.L301
	ldrb	r3, [r6, #860]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L301
	ldr	r0, [r6, #344]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r6, #860]
	b	.L301
.L311:
	ldrb	r3, [r0, #14]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L385
	ldr	r7, .L392
	add	r6, r0, #88
	add	r1, r0, #8
	mov	r2, #80
	mov	r0, r6
	ldr	r3, [r7, #52]
	blx	r3
	ldr	r3, [r7, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	mov	r1, r6
	mov	r0, r5
	bl	REAL8DEC_Decode
	ldr	r8, [r5, #4]
	cmp	r8, #0
	bne	.L301
	add	r5, r5, #77824
	ldr	r3, [r7, #48]
	mov	r2, #160
	mov	r1, r8
	mov	r0, r6
	blx	r3
	ldr	r1, [r4, #24]
	ldr	r0, [r5, #2472]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L392+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L301
	ldr	r0, [r5, #2472]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	blx	ip
	b	.L301
.L312:
	ldrb	r3, [r0, #14]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L386
	ldr	r7, .L392
	add	r6, r0, #88
	add	r1, r0, #8
	mov	r2, #80
	mov	r0, r6
	ldr	r3, [r7, #52]
	blx	r3
	ldr	r3, [r7, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	mov	r1, r6
	mov	r0, r5
	bl	REAL9DEC_Decode
	ldr	r8, [r5, #4]
	cmp	r8, #0
	bne	.L301
	add	r5, r5, #77824
	ldr	r3, [r7, #48]
	mov	r2, #160
	mov	r1, r8
	mov	r0, r6
	blx	r3
	ldr	r1, [r4, #24]
	ldr	r0, [r5, #2456]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L392+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L301
	ldr	r0, [r5, #2456]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	blx	ip
	b	.L301
.L313:
	ldrb	r3, [r0, #14]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L387
	ldr	r7, .L392
	add	r6, r0, #88
	add	r1, r0, #8
	mov	r2, #80
	mov	r0, r6
	ldr	r3, [r7, #52]
	blx	r3
	ldr	r3, [r7, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	mov	r1, r6
	mov	r0, r5
	bl	VP6DEC_Decode
	ldr	r8, [r5, #4]
	cmp	r8, #0
	bne	.L301
	add	r5, r5, #12288
	ldr	r3, [r7, #48]
	mov	r2, #160
	mov	r1, r8
	mov	r0, r6
	blx	r3
	ldr	r1, [r4, #24]
	ldr	r0, [r5, #712]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L392+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L301
	ldr	r0, [r5, #712]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	blx	ip
	b	.L301
.L314:
	ldrb	r3, [r0, #14]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L388
	ldr	r7, .L392
	add	r6, r0, #88
	add	r1, r0, #8
	mov	r2, #80
	mov	r0, r6
	ldr	r3, [r7, #52]
	blx	r3
	ldr	r3, [r7, #48]
	mov	r2, #80
	mov	r1, #0
	add	r0, r4, #168
	blx	r3
	mov	r1, r6
	mov	r0, r5
	bl	VP8DEC_Decode
	ldr	r8, [r5, #4]
	cmp	r8, #0
	bne	.L301
	add	r5, r5, #8192
	ldr	r3, [r7, #48]
	mov	r2, #160
	mov	r1, r8
	mov	r0, r6
	blx	r3
	ldr	r1, [r4, #24]
	ldr	r0, [r5, #328]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L392+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L301
	ldr	r0, [r5, #328]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	blx	ip
	b	.L301
.L315:
	ldrd	r2, [r0, #32]
	sub	r1, fp, #68
	ldr	r8, [r0, #8]
	mov	r0, r5
	ldr	r7, [r4, #16]
	ldr	r6, [r4, #20]
	ldrb	lr, [r4, #13]	@ zero_extendqisi2
	ldr	ip, [r4, #24]
	strd	r2, [fp, #-44]
	str	r8, [fp, #-64]
	str	r7, [fp, #-60]
	str	r6, [fp, #-56]
	strb	lr, [fp, #-68]
	str	ip, [fp, #-52]
	bl	MVC_DEC_DecodePacket
	cmn	r0, #2
	moveq	r3, #1
	movne	r3, #0
	strb	r3, [r4, #1]
	ldr	r4, [r5, #228]
	cmp	r4, #0
	bne	.L301
	ldrb	r3, [r5, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L301
	ldr	r0, [r5, #120]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r5, #1]
	b	.L301
.L375:
	ldr	r3, [r6, #68]
	mov	r0, #0
	ldr	r1, .L392+32
	blx	r3
	b	.L301
.L384:
	add	r1, r4, #88
	mov	r0, r5
	bl	AVSDEC_Decode
	b	.L343
.L380:
	add	r1, r4, #88
	mov	r0, r5
	bl	MPEG4DEC_Decode
	b	.L340
.L382:
	add	r1, r4, #88
	mov	r0, r5
	bl	MPEG2DEC_Decode
	add	r3, r5, #114688
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	cmp	r3, #1
	movne	r3, #0
	strb	r3, [r4, #1]
	b	.L336
.L381:
	ldr	r3, .L392+28
	ldr	r3, [r3, r7, asl #2]
	ldr	r2, [r3, #1388]
	cmp	r2, #1
	bne	.L335
	ldr	r3, [r3, #1428]
	cmp	r3, #0
	bne	.L334
.L335:
	add	r1, r5, #79872
	mov	r0, r5
	add	r1, r1, #96
	bl	MPEG2DEC_Decode
	b	.L334
.L379:
	ldr	r3, .L392+28
	ldr	r3, [r3, r7, asl #2]
	ldr	r2, [r3, #1388]
	cmp	r2, #1
	bne	.L339
	ldr	r3, [r3, #1428]
	cmp	r3, #0
	bne	.L338
.L339:
	add	r1, r5, #15680
	mov	r0, r5
	add	r1, r1, #24
	bl	MPEG4DEC_Decode
	b	.L338
.L383:
	add	r5, r2, #65536
	ldr	r0, [r5, #2432]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #2432]
	ldr	r1, [r4, #24]
	bl	SM_ReleaseStreamSeg
	b	.L301
.L386:
	add	r5, r2, #77824
	ldr	r0, [r5, #2456]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #2456]
	ldr	r1, [r4, #24]
	bl	SM_ReleaseStreamSeg
	b	.L301
.L385:
	add	r5, r2, #77824
	ldr	r0, [r5, #2472]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #2472]
	ldr	r1, [r4, #24]
	bl	SM_ReleaseStreamSeg
	b	.L301
.L388:
	add	r5, r2, #8192
	ldr	r0, [r5, #328]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #328]
	ldr	r1, [r4, #24]
	bl	SM_ReleaseStreamSeg
	b	.L301
.L387:
	add	r5, r2, #12288
	ldr	r0, [r5, #712]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #712]
	ldr	r1, [r4, #24]
	bl	SM_ReleaseStreamSeg
	b	.L301
.L332:
	add	r2, r5, #15808
	mov	r1, #16
	ldr	r0, [r4, #88]
	bl	Vc1SMPSeqHdr
	subs	r8, r0, #0
	beq	.L389
	add	r5, r5, #45056
	ldr	r3, [r6, #68]
	ldr	r1, .L392+36
	b	.L374
.L376:
	ldr	r3, [r6, #68]
	mov	r0, #0
	ldr	r1, .L392+40
	blx	r3
	b	.L301
.L378:
	add	r5, r5, #45056
	ldr	r0, [r5, #3776]
	bl	VCTRL_OutputLastFrame
	ldr	r0, [r5, #3776]
	ldr	r1, [r4, #24]
	bl	SM_ReleaseStreamSeg
	b	.L301
.L377:
	add	r8, r5, #45056
	mov	r0, r5
	ldr	r9, [r8, #3776]
	bl	VC1_GetPacketState
	cmp	r0, #2
	beq	.L390
.L328:
	mov	r1, r4
	ldr	r0, [r8, #3776]
	bl	SegMan
	subs	r9, r0, #0
	beq	.L391
.L330:
	ldr	r4, [r5, #4]
	cmp	r4, #0
	bne	.L301
	ldrb	r3, [r7, #431]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L301
	ldr	r0, [r8, #3776]
	bl	VCTRL_OutputLastFrame
	strb	r4, [r7, #431]
	b	.L301
.L389:
	ldr	ip, [r4, #100]
	mov	r1, r7
	ldr	r2, [r4, #88]
	mov	r0, r5
	ldr	r3, [r4, #96]
	sub	ip, ip, #16
	add	r2, r2, #16
	str	ip, [r4, #100]
	str	r2, [r4, #88]
	add	r3, r3, #16
	str	r3, [r4, #96]
	bl	VC1DEC_Decode
	ldr	r3, [r6, #48]
	mov	r1, r8
	mov	r0, r7
	mov	r2, #160
	blx	r3
	b	.L301
.L391:
	add	r4, r4, #88
	mov	r0, r5
	mov	r1, r4
	bl	VC1DEC_Decode
	mov	r0, r4
	ldr	r3, [r6, #48]
	mov	r1, r9
	mov	r2, #160
	blx	r3
	b	.L330
.L390:
	ldr	r3, .L392+28
	ldr	r3, [r3, r9, asl #2]
	ldr	r2, [r3, #1388]
	cmp	r2, #1
	bne	.L329
	ldr	r3, [r3, #1428]
	cmp	r3, #0
	bne	.L328
.L329:
	add	r1, r5, #12544
	mov	r0, r5
	add	r1, r1, #8
	bl	VC1DEC_Decode
	b	.L328
.L393:
	.align	2
.L392:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC20
	.word	.LC21
	.word	.LC22
	.word	.LC25
	.word	g_event_report
	.word	g_SdecFunc
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
	cmp	r3, #8
	ldrls	pc, [pc, r3, asl #2]
	b	.L395
.L397:
	.word	.L396
	.word	.L395
	.word	.L395
	.word	.L398
	.word	.L399
	.word	.L400
	.word	.L400
	.word	.L400
	.word	.L401
.L401:
	ldr	r5, [r1]
	add	r1, r1, #8192
	ldr	r4, .L414
	ldr	r0, [r1, #328]
	ldr	r1, [r5, #232]
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r5, #221]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L406
	mov	r0, #1
	ldr	r1, .L414+4
	ldr	r3, [r4, #68]
	blx	r3
.L406:
	ldr	r3, [r4, #48]
	add	r0, r5, #216
	mov	r2, #160
	mov	r1, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L396:
	ldr	r4, [r1]
	ldrb	r3, [r4, #221]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L413
.L404:
	ldr	r3, .L414
	add	r0, r4, #216
	mov	r2, #160
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L398:
	ldr	r4, [r1]
	ldrb	r3, [r4, #221]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L404
	add	r3, r1, #77824
	ldr	r1, [r4, #232]
	ldr	r0, [r3, #2472]
	bl	SM_ReleaseStreamSeg
	b	.L404
.L399:
	ldr	r4, [r1]
	ldrb	r3, [r4, #221]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L404
	add	r3, r1, #77824
	ldr	r1, [r4, #232]
	ldr	r0, [r3, #2456]
	bl	SM_ReleaseStreamSeg
	b	.L404
.L400:
	ldr	r5, [r1]
	add	r1, r1, #12288
	ldr	r4, .L414
	ldr	r0, [r1, #712]
	ldr	r1, [r5, #232]
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r5, #221]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L406
	ldr	r1, .L414+8
	mov	r0, #1
	ldr	r3, [r4, #68]
	blx	r3
	b	.L406
.L395:
	ldr	r3, .L414
	mov	r2, r0
	ldr	r1, .L414+12
	mov	r0, #0
	ldr	r3, [r3, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L413:
	add	r3, r1, #65536
	ldr	r1, [r4, #232]
	ldr	r0, [r3, #2432]
	bl	SM_ReleaseStreamSeg
	b	.L404
.L415:
	.align	2
.L414:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC27
	.word	.LC28
	.word	.LC29
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
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	cmp	r0, #17
	ldrls	pc, [pc, r0, asl #2]
	b	.L417
.L419:
	.word	.L418
	.word	.L420
	.word	.L421
	.word	.L422
	.word	.L417
	.word	.L423
	.word	.L424
	.word	.L417
	.word	.L425
	.word	.L426
	.word	.L427
	.word	.L427
	.word	.L427
	.word	.L428
	.word	.L417
	.word	.L417
	.word	.L429
	.word	.L430
.L430:
	mov	r0, r1
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	HEVC_ClearCurrPic
.L418:
	mov	r0, r1
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	H264_ClearCurrPic
.L420:
	add	r3, r1, #45056
	add	r0, r1, #12672
	add	r0, r0, #40
	ldr	r1, [r3, #3776]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	VC1_ClearCurPic
.L421:
	mov	r0, r1
	str	r1, [fp, #-24]
	bl	MP4_ClearCurPic
	ldr	r1, [fp, #-24]
	mov	r0, r1
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	MP4_Clean_HeadPacket
.L422:
	mov	r0, r1
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	MP2_ClearCurPic
.L423:
	ldr	r4, [r1]
	ldrb	r3, [r4, #221]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L442
.L433:
	ldr	r3, .L443
	add	r0, r4, #216
	mov	r2, #160
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L424:
	mov	r0, r1
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	AVS_ClearCurPic
.L425:
	ldr	r4, [r1]
	ldrb	r3, [r4, #221]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L433
	add	r3, r1, #77824
	ldr	r1, [r4, #232]
	ldr	r0, [r3, #2472]
	bl	SM_ReleaseStreamSeg
	b	.L433
.L426:
	ldr	r4, [r1]
	ldrb	r3, [r4, #221]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L433
	add	r3, r1, #77824
	ldr	r1, [r4, #232]
	ldr	r0, [r3, #2456]
	bl	SM_ReleaseStreamSeg
	b	.L433
.L427:
	ldr	r5, [r1]
	add	r1, r1, #12288
	ldr	r4, .L443
	ldr	r0, [r1, #712]
	ldr	r1, [r5, #232]
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r5, #221]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L435
	mov	r0, #1
	ldr	r1, .L443+4
	ldr	r3, [r4, #68]
	blx	r3
.L435:
	ldr	r3, [r4, #48]
	add	r0, r5, #216
	mov	r2, #160
	mov	r1, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L428:
	ldr	r5, [r1]
	add	r1, r1, #8192
	ldr	r4, .L443
	ldr	r0, [r1, #328]
	ldr	r1, [r5, #232]
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r5, #221]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L435
	ldr	r1, .L443+8
	mov	r0, #1
	ldr	r3, [r4, #68]
	blx	r3
	b	.L435
.L429:
	mov	r0, r1
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	MVC_ClearCurrPic
.L417:
	ldr	r3, .L443
	mov	r2, r0
	ldr	r1, .L443+12
	mov	r0, #0
	ldr	r3, [r3, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L442:
	add	r3, r1, #65536
	ldr	r1, [r4, #232]
	ldr	r0, [r3, #2432]
	bl	SM_ReleaseStreamSeg
	b	.L433
.L444:
	.align	2
.L443:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC28
	.word	.LC27
	.word	.LC29
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
	beq	.L489
.L446:
	ldrb	r2, [ip]	@ zero_extendqisi2
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	cmp	r2, #0
	ldrb	r4, [ip, #2]	@ zero_extendqisi2
	bne	.L449
	cmp	r3, #0
	bne	.L449
	cmp	r4, #1
	bne	.L449
	cmp	r1, #17
	ldrls	pc, [pc, r1, asl #2]
	b	.L469
.L454:
	.word	.L453
	.word	.L455
	.word	.L456
	.word	.L457
	.word	.L469
	.word	.L469
	.word	.L458
	.word	.L469
	.word	.L469
	.word	.L469
	.word	.L469
	.word	.L469
	.word	.L469
	.word	.L469
	.word	.L469
	.word	.L469
	.word	.L459
	.word	.L460
.L489:
	ldrb	lr, [r2, #72]	@ zero_extendqisi2
	ands	lr, lr, #1
	beq	.L446
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
	bne	.L467
.L463:
	mov	r2, #1
	str	r2, [r0]
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	mov	r3, #0
.L464:
	cmp	r2, #182
	beq	.L487
.L480:
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L467:
	ldr	r3, [r0]
	cmp	r3, #0
	beq	.L463
	b	.L462
.L453:
	mov	r1, ip
	bl	H264_IsNewPicNal
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r3, r0, #0
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L455:
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	sub	r3, r2, #12
	cmp	r2, #10
	cmpne	r3, #3
	bls	.L490
.L476:
	mvn	r3, #0
.L466:
	cmp	r2, #13
	bne	.L480
	b	.L487
.L456:
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
	bne	.L467
.L462:
	cmp	lr, #1
	mvnne	r3, #0
	bne	.L464
	b	.L463
.L457:
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
	bne	.L491
.L471:
	mvn	r3, #0
.L461:
	cmp	r2, #0
	bne	.L480
.L488:
	str	r2, [r0]
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L458:
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
	bne	.L492
.L474:
	mvn	r3, #0
.L465:
	cmp	r2, #182
	movne	r2, r1
	orreq	r2, r1, #1
	cmp	r2, #0
	beq	.L480
.L487:
	mov	r2, #0
	b	.L488
.L459:
	mov	r1, ip
	bl	MVC_IsNewPicNal
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r3, r0, #0
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L460:
	mov	r1, ip
	bl	HEVC_IsNewPicNal
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r3, r0, #0
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L469:
	mvn	r3, #0
	b	.L480
.L449:
	ldr	ip, .L493
	mov	r0, #22
	str	r4, [sp]
	ldr	r1, .L493+4
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r3, #0
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L490:
	ldr	r3, [r0]
	cmp	r3, #0
	bne	.L476
	mov	r2, #1
	str	r2, [r0]
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	b	.L466
.L492:
	ldr	r3, [r0]
	cmp	r3, #0
	bne	.L474
	mov	r2, #1
	str	r2, [r0]
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	sub	r1, r2, #179
	clz	r1, r1
	mov	r1, r1, lsr #5
	b	.L465
.L491:
	ldr	r3, [r0]
	cmp	r3, #0
	bne	.L471
	mov	r2, #1
	str	r2, [r0]
	ldrb	r2, [ip, #3]	@ zero_extendqisi2
	b	.L461
.L494:
	.align	2
.L493:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC30
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
	ldr	r4, .L502
	add	r4, r4, r0, lsl #2
	ldr	ip, [r4, #64]
	cmp	ip, #0
	beq	.L501
.L496:
	mov	r0, #0
	add	r1, ip, #205824
	mov	r3, ip
	mov	r2, r0
	str	r0, [ip]
.L498:
	str	r2, [r3, #1044]
	add	r3, r3, #1072
	str	r2, [r3, #-32]
	cmp	r3, r1
	bne	.L498
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L501:
	ldr	r3, .L502+4
	movw	r0, #9224
	movt	r0, 3
	ldr	r3, [r3, #188]
	blx	r3
	cmp	r0, #0
	str	r0, [r4, #64]
	movne	ip, r0
	bne	.L496
.L499:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L503:
	.align	2
.L502:
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
	ldr	r4, .L509
	add	r4, r4, r0, lsl #2
	ldr	r0, [r4, #64]
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, .L509+4
	ldr	r3, [r3, #192]
	blx	r3
	mov	r3, #0
	str	r3, [r4, #64]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L510:
	.align	2
.L509:
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
	ldr	r3, .L518
	add	r0, r3, r0, lsl #2
	ldr	ip, [r0, #64]
	cmp	ip, #0
	beq	.L517
.L512:
	mov	r0, #0
	add	r1, ip, #205824
	mov	r3, ip
	mov	r2, r0
	str	r0, [ip]
.L514:
	str	r2, [r3, #1044]
	add	r3, r3, #1072
	str	r2, [r3, #-32]
	cmp	r3, r1
	bne	.L514
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L517:
	cmp	r1, #0
	str	r1, [r0, #64]
	movne	ip, r1
	bne	.L512
.L515:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L519:
	.align	2
.L518:
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
	ldr	r3, .L525
	add	r0, r3, r0, lsl #2
	ldr	r3, [r0, #64]
	cmp	r3, #0
	movne	r3, #0
	strne	r3, [r0, #64]
	ldmfd	sp, {fp, sp, pc}
.L526:
	.align	2
.L525:
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
	ldr	r3, .L532
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #64]
	cmp	r0, #0
	beq	.L530
	mov	ip, #0
	add	r1, r0, #205824
	mov	r3, r0
	mov	r2, ip
	str	ip, [r0]
.L529:
	str	r2, [r3, #1044]
	add	r3, r3, #1072
	str	r2, [r3, #-32]
	cmp	r3, r1
	bne	.L529
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L530:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L533:
	.align	2
.L532:
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
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L541
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #64]
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	mov	r3, r0
	mov	r2, #0
	b	.L537
.L536:
	add	r2, r2, #1
	add	r3, r3, #1072
	cmp	r2, #192
	beq	.L540
.L537:
	ldr	r1, [r3, #1044]
	cmp	r1, #0
	bne	.L536
	ldr	r1, [r3, #1040]
	cmp	r1, #0
	bne	.L536
	mov	r3, #1072
	movw	lr, #1064
	mul	r2, r3, r2
	movw	ip, #1076
	mov	r4, #0
	mov	r5, #0
	add	r3, r0, r2
	add	r2, r2, #8
	add	r0, r0, r2
	str	r1, [r3, #1040]
	str	r1, [r3, #1056]
	str	r1, [r3, #1052]
	str	r1, [r3, #1048]
	strb	r1, [r3, #1072]
	strb	r1, [r3, #1073]
	strb	r1, [r3, #1074]
	strd	r4, [r3, lr]
	strh	r1, [r3, ip]	@ movhi
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L540:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L542:
	.align	2
.L541:
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
	strne	r3, [r1, #1036]
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
	strne	r3, [r1, #1032]
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
	strne	r3, [r1, #1032]
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
	beq	.L569
	ldrh	r3, [r0, #4]
	ldrh	lr, [r0, #8]
	and	r3, r3, #63
	cmp	lr, r3
	beq	.L563
.L562:
	add	r2, r0, r3, lsl #2
	add	r3, r3, #1
	and	r3, r3, #63
	ldr	r2, [r2, #12]
	cmp	r2, #0
	beq	.L561
	ldr	ip, [r2, #196]
	ldr	r2, [r1, #196]
	cmp	ip, r2
	beq	.L564
.L561:
	cmp	r3, lr
	bne	.L562
.L563:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L564:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L569:
	ldr	r3, .L570
	mov	r0, r1
	ldr	r1, .L570+4
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L571:
	.align	2
.L570:
	.word	vfmw_Osal_Func_Ptr_S
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
	beq	.L576
	ldrh	r0, [r4, #8]
	ldrh	r3, [r4, #6]
	cmp	r0, r3
	ldreqh	r3, [r4, #4]
	rsbeq	r0, r0, r3
	clzeq	r0, r0
	moveq	r0, r0, lsr #5
	movne	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L576:
	ldr	r3, .L577
	ldr	r2, .L577+4
	ldr	r1, .L577+8
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L578:
	.align	2
.L577:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+20
	.word	.LC31
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
	b	.L580
.L582:
	.word	.L581
	.word	.L583
	.word	.L584
	.word	.L585
	.word	.L586
	.word	.L587
.L587:
	mov	r1, #0
	mov	r2, #2
	str	r1, [r0, #60]
	str	r2, [r0, #64]
	ldmfd	sp, {fp, sp, pc}
.L581:
	mov	r1, #0
	mov	r2, #1
	str	r1, [r0, #60]
	str	r2, [r0, #64]
	ldmfd	sp, {fp, sp, pc}
.L583:
	mov	r1, #4
	mov	r2, #3
	str	r1, [r0, #60]
	str	r2, [r0, #64]
	ldmfd	sp, {fp, sp, pc}
.L584:
	mov	r1, #16
	mov	r2, #9
	str	r1, [r0, #60]
	str	r2, [r0, #64]
	ldmfd	sp, {fp, sp, pc}
.L585:
	mov	r1, #221
	mov	r2, #100
	str	r1, [r0, #60]
	str	r2, [r0, #64]
	ldmfd	sp, {fp, sp, pc}
.L586:
	mov	r1, #235
	mov	r2, #100
	str	r1, [r0, #60]
	str	r2, [r0, #64]
	ldmfd	sp, {fp, sp, pc}
.L580:
	ldr	ip, .L589
	mov	r0, #1
	mov	r2, r1
	str	r0, [r3, #64]
	mov	lr, #0
	ldr	r1, .L589+4
	str	lr, [r3, #60]
	ldr	r3, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L590:
	.align	2
.L589:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC32
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
	add	r0, r0, #69632
	ldr	r3, [r0, #2824]
	cmp	r3, #1
	beq	.L598
	ldr	r2, [r0, #2828]
	cmp	r2, #1
	beq	.L598
	cmp	r3, #2
	moveq	r3, #1
	ldrne	r3, [r0, #2832]
	subne	r3, r3, #1
	clzne	r3, r3
	movne	r3, r3, lsr #5
.L596:
	ldr	r0, [r0, #2832]
	cmp	r0, #1
	movne	r0, r3
	orreq	r0, r3, #1
	ldmfd	sp, {fp, sp, pc}
.L598:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	IsMain10Profile, .-IsMain10Profile
	.global	pstVdecUsdPool
	.global	g_DnrEnable
	.section	.rodata
	.align	2
.LANCHOR1 = . + 0
	.type	__FUNCTION__.14687, %object
	.size	__FUNCTION__.14687, 19
__FUNCTION__.14687:
	.ascii	"InsertImgToVoQueue\000"
	.space	1
	.type	__func__.14936, %object
	.size	__func__.14936, 15
__func__.14936:
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
	ASCII(.ascii	"VidStd=%d Error!\012\000" )
	.space	2
.LC30:
	ASCII(.ascii	"pu8[0] = %x,pu8[1] = %x, pu8[2] = %x\012\000" )
	.space	2
.LC31:
	ASCII(.ascii	"%s pstVoQue NULL\012\000" )
	.space	2
.LC32:
	ASCII(.ascii	"WARNNING: aspect ration (%d) is not expected!\012\000" )
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	last_rec_pos_time.14678, %object
	.size	last_rec_pos_time.14678, 64
last_rec_pos_time.14678:
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
