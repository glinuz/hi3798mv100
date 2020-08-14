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
	.file	"dsp_mem.c"
	.text
	.align	2
	.global	HI_ADSP_MALLOC
	.type	HI_ADSP_MALLOC, %function
HI_ADSP_MALLOC:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L2
	ldr	r3, [r3, #40]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L3:
	.align	2
.L2:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	HI_ADSP_MALLOC, .-HI_ADSP_MALLOC
	.align	2
	.global	HI_ADSP_FREE
	.type	HI_ADSP_FREE, %function
HI_ADSP_FREE:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r3, .L7
	ldr	r3, [r3, #44]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L8:
	.align	2
.L7:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	HI_ADSP_FREE, .-HI_ADSP_FREE
	.align	2
	.global	HI_ADSP_V_MALLOC
	.type	HI_ADSP_V_MALLOC, %function
HI_ADSP_V_MALLOC:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L10
	ldr	r3, [r3, #48]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L11:
	.align	2
.L10:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	HI_ADSP_V_MALLOC, .-HI_ADSP_V_MALLOC
	.align	2
	.global	HI_ADSP_V_FREE
	.type	HI_ADSP_V_FREE, %function
HI_ADSP_V_FREE:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r3, .L14
	ldr	r3, [r3, #52]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L15:
	.align	2
.L14:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	HI_ADSP_V_FREE, .-HI_ADSP_V_FREE
	.global	__aeabi_uidivmod
	.align	2
	.global	HI_ADSP_MALLOC_Aligned
	.type	HI_ADSP_MALLOC_Aligned, %function
HI_ADSP_MALLOC_Aligned:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	sub	r3, r2, #2
	mov	r4, r2
	cmp	r3, #254
	mov	r7, r0
	mov	r6, r1
	bhi	.L21
	cmp	r2, #1
	bls	.L23
	mov	r3, #0
.L20:
	add	r3, r3, #1
	mov	ip, r4, lsr r3
	cmp	ip, #1
	bhi	.L20
	mvn	r5, #0
	mov	r5, r5, asl r3
	bics	r3, r4, r5
	bne	.L21
.L19:
	cmp	r4, r6
	bcs	.L22
	mov	r1, r4
	mov	r0, r6
	bl	__aeabi_uidivmod
	cmp	r1, #0
	bne	.L21
.L22:
	ldr	r3, .L28
	add	r1, r4, r6
	mov	r0, r7
	ldr	r3, [r3, #40]
	blx	r3
	cmp	r0, #0
	beq	.L21
	add	r4, r0, r4
	and	r4, r4, r5
	rsb	r3, r0, r4
	mov	r0, r4
	strb	r3, [r4, #-1]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L21:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L23:
	mvn	r5, #0
	b	.L19
.L29:
	.align	2
.L28:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	HI_ADSP_MALLOC_Aligned, .-HI_ADSP_MALLOC_Aligned
	.align	2
	.global	HI_ADSP_FREE_Aligned
	.type	HI_ADSP_FREE_Aligned, %function
HI_ADSP_FREE_Aligned:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldrb	r3, [r1, #-1]
	subs	r1, r1, r3
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r3, .L38
	ldr	r3, [r3, #44]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L39:
	.align	2
.L38:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	HI_ADSP_FREE_Aligned, .-HI_ADSP_FREE_Aligned
	.align	2
	.global	HI_ADSP_VMALLOC_Aligned
	.type	HI_ADSP_VMALLOC_Aligned, %function
HI_ADSP_VMALLOC_Aligned:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	sub	r3, r2, #2
	mov	r4, r2
	cmp	r3, #254
	mov	r7, r0
	mov	r6, r1
	bhi	.L45
	cmp	r2, #1
	bls	.L47
	mov	r3, #0
.L44:
	add	r3, r3, #1
	mov	ip, r4, lsr r3
	cmp	ip, #1
	bhi	.L44
	mvn	r5, #0
	mov	r5, r5, asl r3
	bics	r3, r4, r5
	bne	.L45
.L43:
	cmp	r4, r6
	bcs	.L46
	mov	r1, r4
	mov	r0, r6
	bl	__aeabi_uidivmod
	cmp	r1, #0
	bne	.L45
.L46:
	ldr	r3, .L52
	add	r1, r4, r6
	mov	r0, r7
	ldr	r3, [r3, #48]
	blx	r3
	cmp	r0, #0
	beq	.L45
	add	r4, r0, r4
	and	r4, r4, r5
	rsb	r3, r0, r4
	mov	r0, r4
	strb	r3, [r4, #-1]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L45:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L47:
	mvn	r5, #0
	b	.L43
.L53:
	.align	2
.L52:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	HI_ADSP_VMALLOC_Aligned, .-HI_ADSP_VMALLOC_Aligned
	.align	2
	.global	HI_ADSP_VFREE_Aligned
	.type	HI_ADSP_VFREE_Aligned, %function
HI_ADSP_VFREE_Aligned:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldrb	r3, [r1, #-1]
	subs	r1, r1, r3
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r3, .L62
	ldr	r3, [r3, #52]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L63:
	.align	2
.L62:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	HI_ADSP_VFREE_Aligned, .-HI_ADSP_VFREE_Aligned
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
