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
	.file	"dsp_ringbuf.c"
	.text
	.align	2
	.global	RBUF_Create
	.type	RBUF_Create, %function
RBUF_Create:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r7, r0
	mov	r5, r1
	mov	r0, #26
	mov	r1, #84
	bl	HI_ADSP_MALLOC
	subs	r4, r0, #0
	beq	.L2
	ldr	r6, .L11
	mov	r2, #84
	mov	r1, #0
	ldr	r3, [r6, #20]
	blx	r3
	ldr	r3, [r6, #24]
	mov	r2, #36
	mov	r1, r5
	mov	r0, r4
	blx	r3
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L3
	ldr	r3, [r4, #4]
	ldr	r0, [r4, #8]
.L4:
	ldr	r1, [r4, #12]
	mov	r2, #0
	ldr	ip, [r4, #16]
	str	r3, [r4, #44]
	str	r0, [r4, #48]
	str	ip, [r4, #40]
	str	r1, [r4, #36]
	str	r2, [r1]
	ldr	r3, [r4, #40]
	str	r2, [r3]
	str	r4, [r7]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3:
	ldr	r2, [r5, #24]
	mov	r0, #26
	ldr	r1, [r5, #8]
	bl	HI_ADSP_VMALLOC_Aligned
	cmp	r0, #0
	beq	.L5
	str	r0, [r4, #4]
	mov	r3, r0
	ldr	r0, [r5, #8]
	str	r0, [r4, #8]
	b	.L4
.L5:
	mov	r1, r4
	mov	r0, #26
	bl	HI_ADSP_FREE
.L2:
	mov	r3, #0
	str	r3, [r7]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L12:
	.align	2
.L11:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	RBUF_Create, .-RBUF_Create
	.align	2
	.global	RBUF_Create_EXT
	.type	RBUF_Create_EXT, %function
RBUF_Create_EXT:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r7, r0
	mov	r5, r1
	mov	r0, #26
	mov	r1, #84
	bl	HI_ADSP_MALLOC
	subs	r4, r0, #0
	beq	.L14
	ldr	r6, .L22
	mov	r2, #84
	mov	r1, #0
	ldr	r3, [r6, #20]
	blx	r3
	ldr	r3, [r6, #24]
	mov	r2, #36
	mov	r1, r5
	mov	r0, r4
	blx	r3
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L15
	ldr	r3, [r4, #4]
	ldr	r2, [r4, #8]
.L16:
	ldr	r0, [r4, #16]
	ldr	r1, [r4, #12]
	str	r3, [r4, #44]
	str	r2, [r4, #48]
	str	r0, [r4, #40]
	str	r1, [r4, #36]
	str	r4, [r7]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L15:
	ldr	r2, [r5, #24]
	mov	r0, #26
	ldr	r1, [r5, #8]
	bl	HI_ADSP_VMALLOC_Aligned
	cmp	r0, #0
	beq	.L17
	str	r0, [r4, #4]
	mov	r3, r0
	ldr	r2, [r5, #8]
	str	r2, [r4, #8]
	b	.L16
.L17:
	mov	r1, r4
	mov	r0, #26
	bl	HI_ADSP_FREE
.L14:
	mov	r3, #0
	str	r3, [r7]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L23:
	.align	2
.L22:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	RBUF_Create_EXT, .-RBUF_Create_EXT
	.align	2
	.global	RBUF_Destroy
	.type	RBUF_Destroy, %function
RBUF_Destroy:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r4, r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, [r4]
	cmp	r3, #0
	beq	.L27
.L26:
	mov	r1, r4
	mov	r0, #26
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	HI_ADSP_FREE
.L27:
	ldr	r1, [r4, #4]
	mov	r0, #26
	bl	HI_ADSP_VFREE_Aligned
	b	.L26
	UNWIND(.fnend)
	.size	RBUF_Destroy, .-RBUF_Destroy
	.align	2
	.global	RBUF_ReadData
	.type	RBUF_ReadData, %function
RBUF_ReadData:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	subs	r5, r0, #0
	mov	r7, r1
	moveq	r0, r5
	beq	.L48
	ldr	ip, [r5, #40]
	mov	r0, #0
	ldr	r1, [r5, #36]
	ldr	r3, [r5, #56]
	str	r0, [fp, #-60]
	add	r3, r3, #1
	str	r3, [r5, #56]
	ldr	r4, [ip]
	ldr	r3, [r1]
	str	r0, [fp, #-56]
	cmp	r4, r3
	str	r4, [fp, #-52]
	str	r0, [fp, #-48]
	bhi	.L30
	add	r1, r2, r4
	cmp	r3, r1
	bcc	.L31
.L33:
	mov	r8, r2
	mov	r3, #0
	str	r2, [fp, #-60]
	str	r3, [fp, #-72]
.L32:
	cmp	r8, #0
	beq	.L36
	ldr	ip, .L51
	sub	r10, fp, #52
	sub	r9, fp, #60
	sub	r3, fp, #56
	mov	r6, r8
.L37:
	subs	r0, r7, #0
	mov	r2, r6
	ldr	r1, [r5, #44]
	ldr	r4, [r10], #4
	beq	.L35
	str	r3, [fp, #-68]
	add	r1, r1, r4
	ldr	r3, [ip, #24]
	add	r7, r7, r6
	str	ip, [fp, #-64]
	blx	r3
	ldr	r3, [fp, #-68]
	ldr	ip, [fp, #-64]
.L35:
	cmp	r9, r3
	add	r4, r4, r6
	beq	.L36
	ldr	r6, [r9, #4]!
	cmp	r6, #0
	bne	.L37
.L36:
	ldr	r2, [r5, #48]
	ldr	r3, [r5, #40]
	cmp	r2, r4
	ldr	r2, [fp, #-72]
	moveq	r4, #0
	adds	r0, r2, r8
	str	r4, [r3]
	ldreq	r3, [r5, #60]
	addeq	r3, r3, #1
	streq	r3, [r5, #60]
	ldr	r3, [r5, #52]
	add	r3, r3, r0
	str	r3, [r5, #52]
.L48:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L30:
	ldr	r8, [r5, #48]
	add	r1, r2, r4
	cmp	r1, r8
	bls	.L33
	rsb	r8, r4, r8
	str	r8, [fp, #-60]
	rsb	r2, r8, r2
	str	r2, [fp, #-72]
	cmp	r3, r2
	strcc	r3, [fp, #-56]
	strcc	r3, [fp, #-72]
	ldrcs	r3, [fp, #-72]
	strcs	r3, [fp, #-56]
	b	.L32
.L31:
	rsb	r8, r4, r3
	str	r0, [fp, #-72]
	str	r8, [fp, #-60]
	b	.L32
.L52:
	.align	2
.L51:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	RBUF_ReadData, .-RBUF_ReadData
	.align	2
	.global	RBUF_WriteData
	.type	RBUF_WriteData, %function
RBUF_WriteData:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	subs	r5, r0, #0
	mov	r8, r1
	moveq	r0, r2
	beq	.L71
	ldr	ip, [r5, #40]
	mov	r0, #0
	ldr	r1, [r5, #36]
	ldr	r3, [r5, #72]
	str	r0, [fp, #-60]
	add	r3, r3, #1
	str	r3, [r5, #72]
	ldr	r6, [ip]
	ldr	r4, [r1]
	str	r0, [fp, #-56]
	cmp	r6, r4
	str	r0, [fp, #-48]
	str	r4, [fp, #-52]
	bhi	.L55
	ldr	r6, [r5, #48]
	add	r3, r2, r4
	cmp	r6, r3
	bcc	.L56
.L58:
	mov	r6, r2
	mov	r3, #0
	str	r2, [fp, #-60]
	str	r3, [fp, #-72]
.L57:
	cmp	r6, #0
	beq	.L62
	ldr	r10, .L73
	mov	r7, r6
	mov	r9, #0
	sub	r3, fp, #52
	str	r3, [fp, #-68]
	sub	r3, fp, #60
	str	r3, [fp, #-64]
.L66:
	subs	r1, r8, #0
	ldr	r0, [r5, #44]
	mov	r2, r7
	add	r9, r9, #1
	add	r0, r0, r4
	ldreq	r3, [r10, #20]
	ldrne	r3, [r10, #24]
	addne	r8, r8, r7
	blx	r3
	cmp	r9, #2
	add	r4, r7, r4
	beq	.L72
	ldr	r3, [fp, #-64]
	ldr	r7, [r3, #4]!
	cmp	r7, #0
	str	r3, [fp, #-64]
	ldrne	r3, [fp, #-68]
	ldrne	r4, [r3, #4]!
	strne	r3, [fp, #-68]
	bne	.L66
.L72:
	ldr	r1, [r5, #36]
.L62:
	ldr	r3, [r5, #48]
	cmp	r3, r4
	ldr	r3, [fp, #-72]
	movls	r4, #0
	adds	r0, r6, r3
	str	r4, [r1]
	ldreq	r3, [r5, #76]
	addeq	r3, r3, #1
	streq	r3, [r5, #76]
	ldr	r3, [r5, #68]
	add	r3, r3, r0
	str	r3, [r5, #68]
.L71:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L55:
	add	r3, r2, r4
	cmp	r6, r3
	bhi	.L58
	rsb	r6, r4, r6
	str	r0, [fp, #-72]
	str	r6, [fp, #-60]
	b	.L57
.L56:
	rsb	r6, r4, r6
	str	r6, [fp, #-60]
	rsb	r3, r6, r2
	str	r3, [fp, #-72]
	str	r3, [fp, #-56]
	b	.L57
.L74:
	.align	2
.L73:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	RBUF_WriteData, .-RBUF_WriteData
	.align	2
	.global	RBUF_ReadDataComplete
	.type	RBUF_ReadDataComplete, %function
RBUF_ReadDataComplete:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	lr, [r0, #40]
	ldr	r3, [r0, #36]
	ldr	r2, [lr]
	ldr	ip, [r3]
	cmp	r2, ip
	bhi	.L77
	add	r3, r1, r2
	cmp	ip, r3
	rsbcc	r5, r2, ip
	ldrcs	r4, [r0, #48]
	movcs	r5, r1
	ldrcc	r4, [r0, #48]
.L79:
	cmp	r5, #0
	beq	.L90
.L84:
	add	r3, r5, r2
.L81:
	cmp	r4, r3
	moveq	r3, #0
	str	r3, [lr]
	ldr	r3, [r0, #52]
	add	r1, r3, r1
	str	r1, [r0, #52]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L77:
	ldr	r4, [r0, #48]
	add	r3, r1, r2
	cmp	r3, r4
	bls	.L85
	rsb	r5, r2, r4
	rsb	r3, r5, r1
	cmp	r3, ip
	movcs	r3, ip
	cmp	r5, #0
	beq	.L90
	cmp	r3, #0
	bne	.L81
	b	.L84
.L85:
	mov	r5, r1
	b	.L79
.L90:
	mov	r3, r2
	b	.L81
	UNWIND(.fnend)
	.size	RBUF_ReadDataComplete, .-RBUF_ReadDataComplete
	.align	2
	.global	RBUF_WriteDataComplete
	.type	RBUF_WriteDataComplete, %function
RBUF_WriteDataComplete:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	ldmeqfd	sp, {r4, fp, sp, pc}
	ldr	r3, [r0, #40]
	ldr	r2, [r0, #36]
	ldr	lr, [r0, #48]
	ldr	r3, [r3]
	ldr	ip, [r2]
	cmp	r3, ip
	bls	.L104
.L95:
	cmp	r1, #0
	movne	r4, r1
	beq	.L99
.L98:
	add	r3, r4, ip
.L96:
	cmp	lr, r3
	moveq	r3, #0
	str	r3, [r2]
	ldr	r3, [r0, #68]
	add	r1, r3, r1
	str	r1, [r0, #68]
	ldmfd	sp, {r4, fp, sp, pc}
.L104:
	add	r3, r1, ip
	cmp	lr, r3
	bcs	.L95
	rsb	r4, ip, lr
	cmp	r4, #0
	rsb	r3, r4, r1
	beq	.L99
	cmp	r3, #0
	bne	.L96
	b	.L98
.L99:
	mov	r3, ip
	b	.L96
	UNWIND(.fnend)
	.size	RBUF_WriteDataComplete, .-RBUF_WriteDataComplete
	.align	2
	.global	RBUF_QueryInfo
	.type	RBUF_QueryInfo, %function
RBUF_QueryInfo:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	mov	r3, r1
	beq	.L114
	ldr	r2, [r0, #28]
	ldr	lr, [r0, #24]
	str	r2, [r1, #32]
	ldr	r2, [r0, #44]
	str	r2, [r1]
	ldr	r2, [r0, #48]
	str	r2, [r1, #12]
	ldr	r2, [r0, #36]
	ldr	r2, [r2]
	str	r2, [r1, #8]
	ldr	r2, [r0, #40]
	ldr	r2, [r2]
	str	r2, [r1, #4]
	ldr	r1, [r0, #40]
	ldr	r2, [r0, #36]
	ldr	ip, [r1]
	ldr	r2, [r2]
	cmp	ip, r2
	rsbls	r2, ip, r2
	strls	r2, [r3, #16]
	ldrhi	r4, [r0, #48]
	addhi	r1, r2, r4
	rsbhi	r4, ip, r4
	rsbhi	r2, ip, r1
	strhi	r4, [r3, #16]
	str	r2, [r3, #20]
	ldr	r2, [r0, #40]
	ldr	r1, [r0, #36]
	ldr	r2, [r2]
	ldr	r1, [r1]
	cmp	r2, r1
	rsbhi	r2, r1, r2
	strhi	r2, [r3, #24]
	ldrls	r0, [r0, #48]
	addls	r2, r2, r0
	rsbls	r0, r1, r0
	rsbls	r2, r1, r2
	strls	r0, [r3, #24]
	cmp	lr, r2
	str	r2, [r3, #28]
	bcs	.L115
	ldr	r1, [r3, #28]
	rsb	r2, lr, r2
	ldr	r0, [r3, #24]
	str	r2, [r3, #28]
	rsb	r1, r0, r1
	cmp	lr, r1
	strhi	r2, [r3, #24]
	ldmfd	sp, {r4, fp, sp, pc}
.L115:
	mov	r2, #0
	str	r2, [r3, #24]
	str	r2, [r3, #28]
	ldmfd	sp, {r4, fp, sp, pc}
.L114:
	ldr	ip, .L116
	mov	r1, r0
	mov	r2, #36
	mov	r0, r3
	ldr	r3, [ip, #20]
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	bx	r3
.L117:
	.align	2
.L116:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	RBUF_QueryInfo, .-RBUF_QueryInfo
	.align	2
	.global	RBUF_QueryBusy_ProvideRptr
	.type	RBUF_QueryBusy_ProvideRptr, %function
RBUF_QueryBusy_ProvideRptr:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	ip, [r0, #40]
	ldr	r2, [r0, #36]
	ldr	r3, [r1]
	ldr	r1, [ip]
	ldr	r2, [r2]
	cmp	r3, r2
	rsbls	r0, r3, r2
	ldrhi	r0, [r0, #48]
	addhi	r0, r2, r0
	rsbhi	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	RBUF_QueryBusy_ProvideRptr, .-RBUF_QueryBusy_ProvideRptr
	.align	2
	.global	RBUF_QueryBusy
	.type	RBUF_QueryBusy, %function
RBUF_QueryBusy:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r3, [r0, #40]
	ldr	r2, [r0, #36]
	ldr	r3, [r3]
	ldr	r2, [r2]
	cmp	r3, r2
	rsbls	r0, r3, r2
	ldrhi	r0, [r0, #48]
	addhi	r0, r2, r0
	rsbhi	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	RBUF_QueryBusy, .-RBUF_QueryBusy
	.align	2
	.global	RBUF_QueryFree
	.type	RBUF_QueryFree, %function
RBUF_QueryFree:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r2, [r0, #40]
	ldr	r3, [r0, #36]
	ldr	r1, [r0, #24]
	ldr	r2, [r2]
	ldr	r3, [r3]
	cmp	r2, r3
	ldrls	r0, [r0, #48]
	addls	r2, r2, r0
	rsb	r3, r3, r2
	cmp	r1, r3
	rsbcc	r0, r1, r3
	movcs	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	RBUF_QueryFree, .-RBUF_QueryFree
	.align	2
	.global	RBUF_CheckFree
	.type	RBUF_CheckFree, %function
RBUF_CheckFree:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r3, r0, #0
	beq	.L138
	ldr	r0, [r3, #36]
	ldr	r2, [r3, #40]
	ldr	r4, [r3, #24]
	ldr	r2, [r2]
	ldr	ip, [r0]
	cmp	r2, ip
	rsbls	r0, r2, ip
	rsbhi	lr, ip, r2
	ldrls	lr, [r3, #48]
	ldrhi	r0, [r3, #48]
	addls	r2, r2, lr
	addhi	ip, ip, r0
	rsbls	lr, ip, r2
	rsbhi	r0, r2, ip
	cmp	r4, lr
	addcs	ip, r0, r1
	movcs	r1, #1
	bcc	.L139
.L137:
	ldr	r0, [r3, #28]
	cmp	r0, ip
	movcs	r0, r1
	orrcc	r0, r1, #1
	eor	r0, r0, #1
	ldmfd	sp, {r4, fp, sp, pc}
.L139:
	rsb	r2, r4, lr
	add	ip, r0, r1
	cmp	r1, r2
	movcc	r1, #0
	movcs	r1, #1
	b	.L137
.L138:
	mov	r0, r3
	ldmfd	sp, {r4, fp, sp, pc}
	UNWIND(.fnend)
	.size	RBUF_CheckFree, .-RBUF_CheckFree
	.align	2
	.global	RBUF_Flush
	.type	RBUF_Flush, %function
RBUF_Flush:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	ldrne	r2, [r0, #36]
	ldrne	r3, [r0, #40]
	ldrne	r2, [r2]
	strne	r2, [r3]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	RBUF_Flush, .-RBUF_Flush
	.align	2
	.global	RBUF_ReadDataDma
	.type	RBUF_ReadDataDma, %function
RBUF_ReadDataDma:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	subs	r6, r0, #0
	mov	r7, r1
	mov	r9, r3
	beq	.L161
	cmp	r3, #0
	ldr	r0, [r6, #40]
	beq	.L172
	ldr	r1, [r6, #36]
	mov	r3, #0
	ldr	r0, [r0]
	ldr	r4, [r9]
	ldr	r1, [r1]
	str	r3, [fp, #-60]
	str	r3, [fp, #-56]
	str	r3, [fp, #-48]
.L160:
	cmp	r4, r1
	str	r4, [fp, #-52]
	bhi	.L148
	add	r3, r4, r2
	cmp	r3, r1
	bls	.L151
	ldr	r3, [fp, #-56]
	rsb	r8, r4, r1
	str	r8, [fp, #-60]
	str	r3, [fp, #-68]
.L150:
	cmp	r8, #0
	beq	.L154
	sub	r3, fp, #52
	ldr	ip, .L174
	sub	r10, fp, #60
	mov	r5, r8
	str	r8, [fp, #-72]
	mov	r8, r3
.L155:
	subs	r0, r7, #0
	mov	r2, r5
	ldr	r1, [r6, #44]
	ldr	r4, [r8], #4
	beq	.L153
	ldr	r3, [ip, #24]
	add	r1, r1, r4
	str	ip, [fp, #-64]
	blx	r3
	ldr	ip, [fp, #-64]
	add	r7, r7, r5
.L153:
	sub	r3, fp, #56
	add	r4, r4, r5
	cmp	r10, r3
	beq	.L170
	ldr	r5, [r10, #4]!
	cmp	r5, #0
	bne	.L155
.L170:
	ldr	r8, [fp, #-72]
.L154:
	ldr	r3, [r6, #48]
	cmp	r3, r4
	moveq	r4, #0
	cmp	r9, #0
	strne	r4, [r9]
	ldrne	r3, [fp, #-68]
	addne	r0, r3, r8
	beq	.L173
.L169:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L148:
	ldr	r8, [r6, #48]
	add	r3, r4, r2
	cmp	r3, r8
	bls	.L151
	rsb	r8, r4, r8
	str	r8, [fp, #-60]
	rsb	r3, r8, r2
	str	r3, [fp, #-68]
	cmp	r3, r1
	strhi	r1, [fp, #-56]
	strhi	r1, [fp, #-68]
	ldrls	r3, [fp, #-68]
	strls	r3, [fp, #-56]
	b	.L150
.L151:
	ldr	r3, [fp, #-56]
	mov	r8, r2
	str	r2, [fp, #-60]
	str	r3, [fp, #-68]
	b	.L150
.L172:
	ldr	r1, [r6, #36]
	ldr	r3, [r6, #56]
	str	r9, [fp, #-60]
	add	r3, r3, #1
	str	r9, [fp, #-56]
	str	r3, [r6, #56]
	str	r9, [fp, #-48]
	ldr	r4, [r0]
	ldr	r1, [r1]
	b	.L160
.L161:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L173:
	ldr	r2, [fp, #-68]
	ldr	r3, [r6, #40]
	adds	r0, r2, r8
	str	r4, [r3]
	ldreq	r3, [r6, #60]
	addeq	r3, r3, #1
	streq	r3, [r6, #60]
	ldr	r3, [r6, #52]
	add	r3, r3, r0
	str	r3, [r6, #52]
	b	.L169
.L175:
	.align	2
.L174:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	RBUF_ReadDataDma, .-RBUF_ReadDataDma
	.align	2
	.global	RBUF_WriteDataDma
	.type	RBUF_WriteDataDma, %function
RBUF_WriteDataDma:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	subs	r5, r0, #0
	mov	r8, r1
	moveq	r0, r2
	beq	.L194
	ldr	ip, [r5, #40]
	mov	r0, #0
	ldr	r1, [r5, #36]
	ldr	r3, [r5, #72]
	str	r0, [fp, #-60]
	add	r3, r3, #1
	str	r3, [r5, #72]
	ldr	r6, [ip]
	ldr	r4, [r1]
	str	r0, [fp, #-56]
	cmp	r6, r4
	str	r0, [fp, #-48]
	str	r4, [fp, #-52]
	bhi	.L178
	ldr	r6, [r5, #48]
	add	r3, r2, r4
	cmp	r6, r3
	bcc	.L179
.L181:
	mov	r6, r2
	mov	r3, #0
	str	r2, [fp, #-60]
	str	r3, [fp, #-72]
.L180:
	cmp	r6, #0
	beq	.L185
	ldr	r10, .L196
	mov	r7, r6
	mov	r9, #0
	sub	r3, fp, #52
	str	r3, [fp, #-68]
	sub	r3, fp, #60
	str	r3, [fp, #-64]
.L189:
	subs	r1, r8, #0
	ldr	r0, [r5, #44]
	mov	r2, r7
	add	r9, r9, #1
	add	r0, r0, r4
	ldreq	r3, [r10, #20]
	ldrne	r3, [r10, #24]
	addne	r8, r8, r7
	blx	r3
	cmp	r9, #2
	add	r4, r7, r4
	beq	.L195
	ldr	r3, [fp, #-64]
	ldr	r7, [r3, #4]!
	cmp	r7, #0
	str	r3, [fp, #-64]
	ldrne	r3, [fp, #-68]
	ldrne	r4, [r3, #4]!
	strne	r3, [fp, #-68]
	bne	.L189
.L195:
	ldr	r1, [r5, #36]
.L185:
	ldr	r3, [r5, #48]
	cmp	r3, r4
	ldr	r3, [fp, #-72]
	movls	r4, #0
	adds	r0, r6, r3
	str	r4, [r1]
	ldreq	r3, [r5, #76]
	addeq	r3, r3, #1
	streq	r3, [r5, #76]
	ldr	r3, [r5, #68]
	add	r3, r3, r0
	str	r3, [r5, #68]
.L194:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L178:
	add	r3, r2, r4
	cmp	r6, r3
	bhi	.L181
	rsb	r6, r4, r6
	str	r0, [fp, #-72]
	str	r6, [fp, #-60]
	b	.L180
.L179:
	rsb	r6, r4, r6
	str	r6, [fp, #-60]
	rsb	r3, r6, r2
	str	r3, [fp, #-72]
	str	r3, [fp, #-56]
	b	.L180
.L197:
	.align	2
.L196:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	RBUF_WriteDataDma, .-RBUF_WriteDataDma
	.align	2
	.global	RBUF_CopyData
	.type	RBUF_CopyData, %function
RBUF_CopyData:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	cmp	r1, #0
	cmpne	r0, #0
	mov	r6, r0
	mov	r4, r2
	moveq	lr, #1
	movne	lr, #0
	mov	r5, r1
	moveq	r0, #0
	bne	.L219
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L219:
	ldr	r1, [r6, #40]
	ldr	r0, [r6, #36]
	ldr	r2, [r5, #36]
	ldr	ip, [r1]
	ldr	r3, [r2]
	ldr	r9, [r1]
	ldr	r1, [r0]
	ldr	r0, [r6, #44]
	cmp	r9, r1
	str	r0, [fp, #-52]
	ldr	r0, [r5, #44]
	str	r0, [fp, #-48]
	bls	.L220
	ldr	r0, [r6, #48]
	add	r7, r4, r9
	cmp	r7, r0
	bls	.L214
	rsb	r9, r9, r0
	mov	r0, #2
	rsb	r7, r9, r4
	cmp	r7, r1
	movcs	r7, r1
.L201:
	ldr	r1, [r5, #40]
	ldr	r8, [r1]
	ldr	r2, [r2]
	cmp	r8, r2
	bhi	.L202
	ldr	r8, [r5, #48]
	add	r1, r4, r2
	cmp	r8, r1
	bcs	.L216
	rsb	r8, r2, r8
	mov	r2, #2
	rsb	r10, r8, r4
.L204:
	cmp	r2, #2
	movne	lr, #0
	andeq	lr, lr, #1
	cmp	lr, #0
	bne	.L221
	cmp	r8, r9
	beq	.L222
	bcs	.L209
	ldr	r2, [fp, #-48]
	rsb	r9, r8, r9
	str	ip, [fp, #-56]
	add	r10, r9, r7
	add	r0, r2, r3
	ldr	r3, [fp, #-52]
	mov	r2, r8
	add	r1, r3, ip
	ldr	ip, .L224
	ldr	r3, [ip, #24]
	blx	r3
	ldr	ip, [fp, #-56]
	ldr	r0, .L224
	mov	r2, r9
	add	r1, r8, ip
	ldr	r8, [fp, #-48]
	ldr	r3, [fp, #-52]
	add	r1, r3, r1
	ldr	r3, [r0, #24]
	mov	r0, r8
	blx	r3
	ldr	r2, .L224
	add	r0, r8, r9
	ldr	r1, [fp, #-52]
	ldr	r3, [r2, #24]
	mov	r2, r7
	blx	r3
.L206:
	ldr	r1, [r6, #48]
	mov	r0, r4
	ldr	r3, [r6, #40]
	cmp	r1, r7
	ldr	r2, [r5, #48]
	moveq	r7, #0
	str	r7, [r3]
	ldr	r3, [r5, #36]
	cmp	r2, r10
	moveq	r10, #0
	str	r10, [r3]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L220:
	add	r0, r4, r9
	mov	lr, #1
	cmp	r1, r0
	bcs	.L218
	rsb	r9, r9, r1
	mov	r0, lr
	mov	r7, #0
	b	.L201
.L202:
	add	r1, r4, r2
	cmp	r8, r1
	rsbls	r8, r2, r8
	bhi	.L216
	cmp	lr, #0
	bne	.L223
.L205:
	cmp	r0, #2
	bne	.L217
	ldr	r8, .L224
	ldr	r2, [fp, #-52]
	ldr	r0, [fp, #-48]
	add	r1, r2, ip
	mov	r2, r9
	add	r0, r0, r3
	add	r9, r9, r3
	ldr	r3, [r8, #24]
	add	r10, r9, r7
	blx	r3
	ldr	r0, [fp, #-48]
	ldr	r3, [r8, #24]
	mov	r2, r7
	add	r0, r0, r9
	ldr	r1, [fp, #-52]
	blx	r3
	b	.L206
.L214:
	mov	lr, #1
.L218:
	mov	r9, r4
	mov	r0, lr
	mov	r7, #0
	b	.L201
.L216:
	cmp	lr, #0
	mov	r8, r4
	beq	.L205
.L223:
	ldr	r2, [fp, #-52]
	add	r10, r4, r3
	ldr	lr, .L224
	add	r7, r4, ip
	add	r1, r2, ip
	ldr	r2, [fp, #-48]
	add	r0, r2, r3
	mov	r2, r4
	ldr	r3, [lr, #24]
	blx	r3
	b	.L206
.L217:
	mov	r10, lr
	mov	r2, #1
	b	.L204
.L209:
	ldr	r2, [fp, #-52]
	rsb	r8, r9, r8
	ldr	r0, [fp, #-48]
	add	r7, r8, r10
	add	r1, r2, ip
	ldr	ip, .L224
	mov	r2, r9
	add	r0, r0, r3
	str	r3, [fp, #-56]
	ldr	ip, [ip, #24]
	blx	ip
	ldr	r3, [fp, #-56]
	ldr	r1, .L224
	mov	r2, r8
	add	r0, r9, r3
	ldr	r9, [fp, #-52]
	ldr	r3, [fp, #-48]
	add	r0, r3, r0
	ldr	r3, [r1, #24]
	mov	r1, r9
	blx	r3
	ldr	r2, .L224
	add	r1, r9, r8
	ldr	r0, [fp, #-48]
	ldr	r3, [r2, #24]
	mov	r2, r10
	blx	r3
	b	.L206
.L221:
	ldr	r2, [fp, #-48]
	ldr	r9, .L224
	add	r0, r2, r3
	ldr	r3, [fp, #-52]
	mov	r2, r8
	add	r8, r8, ip
	add	r1, r3, ip
	ldr	r3, [r9, #24]
	blx	r3
	ldr	r1, [fp, #-52]
	ldr	r3, [r9, #24]
	mov	r2, r10
	add	r1, r1, r8
	ldr	r0, [fp, #-48]
	add	r7, r8, r10
	blx	r3
	b	.L206
.L222:
	ldr	r9, .L224
	mov	r2, r8
	ldr	r0, [fp, #-48]
	mov	r7, r10
	ldr	r8, [fp, #-52]
	add	r0, r0, r3
	ldr	r3, [r9, #24]
	add	r1, r8, ip
	blx	r3
	ldr	r3, [r9, #24]
	mov	r1, r8
	ldr	r0, [fp, #-48]
	mov	r2, r10
	blx	r3
	b	.L206
.L225:
	.align	2
.L224:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	RBUF_CopyData, .-RBUF_CopyData
	.align	2
	.global	RBUF_GetCompressInfo
	.type	RBUF_GetCompressInfo, %function
RBUF_GetCompressInfo:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	streq	r0, [r1]
	streq	r0, [r2]
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r3, [r0, #40]
	ldr	ip, [r0, #44]
	ldr	r5, [r0, #48]
	ldr	r4, [r3]
	add	r3, r4, #2
	cmp	r3, r5
	ldrh	lr, [ip, r4]
	movcs	r3, #0
	cmp	lr, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	movw	r5, #65535
	cmp	lr, r5
	add	r5, ip, r3
	beq	.L230
	ldrh	r6, [ip, r3]
	cmp	r6, lr
	ldmccfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L230:
	str	lr, [r1]
	movw	r6, #65535
	ldrh	r7, [ip, r4]
	cmp	r7, r6
	beq	.L236
	ldrh	r3, [r5]
	movw	r1, #63602
	movt	r1, 65535
	movw	r0, #19999
	rsb	lr, lr, r3
	str	lr, [r2]
	strh	r1, [ip, r4]
	strh	r0, [r5]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L236:
	mov	ip, #0
	str	ip, [r1]
	ldrh	r1, [r5]
	add	r3, r3, #2
	str	r1, [r2]
	ldr	r1, [r0, #48]
	ldr	r2, [r0, #40]
	cmp	r3, r1
	movcs	r3, ip
	str	r3, [r2]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	RBUF_GetCompressInfo, .-RBUF_GetCompressInfo
	.align	2
	.global	RBUF_UpdateAttr
	.type	RBUF_UpdateAttr, %function
RBUF_UpdateAttr:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	ldrne	r3, [r1, #32]
	strne	r3, [r0, #32]
	ldrne	r3, [r1, #28]
	strne	r3, [r0, #28]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	RBUF_UpdateAttr, .-RBUF_UpdateAttr
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
