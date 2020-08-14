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
	.file	"hal_venc.c"
	.text
	.align	2
	.global	VENC_HAL_ClrAllInt
	.type	VENC_HAL_ClrAllInt, %function
VENC_HAL_ClrAllInt:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mvn	r3, #0
	str	r3, [r0, #12]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VENC_HAL_ClrAllInt, .-VENC_HAL_ClrAllInt
	.align	2
	.global	VENC_HAL_DisableAllInt
	.type	VENC_HAL_DisableAllInt, %function
VENC_HAL_DisableAllInt:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r3, #0
	str	r3, [r0, #4]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VENC_HAL_DisableAllInt, .-VENC_HAL_DisableAllInt
	.align	2
	.global	VENC_HAL_ReadReg_Smmu
	.type	VENC_HAL_ReadReg_Smmu, %function
VENC_HAL_ReadReg_Smmu:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #16]
	mvn	r1, #0
	add	r3, r3, #61440
	ldr	r2, [r3, #40]
	and	r2, r2, #1
	str	r2, [r0, #2672]
	ldr	r2, [r3, #40]
	ubfx	r2, r2, #1, #1
	str	r2, [r0, #2676]
	ldr	r2, [r3, #40]
	ubfx	r2, r2, #2, #1
	str	r2, [r0, #2680]
	ldr	r2, [r3, #40]
	ubfx	r2, r2, #3, #1
	str	r2, [r0, #2684]
	ldr	r2, [r3, #24]
	and	r2, r2, #1
	str	r2, [r0, #2656]
	ldr	r2, [r3, #24]
	ubfx	r2, r2, #1, #1
	str	r2, [r0, #2660]
	ldr	r2, [r3, #24]
	ubfx	r2, r2, #2, #1
	str	r2, [r0, #2664]
	ldr	r2, [r3, #24]
	ubfx	r2, r2, #3, #1
	str	r2, [r0, #2668]
	str	r1, [r3, #28]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VENC_HAL_ReadReg_Smmu, .-VENC_HAL_ReadReg_Smmu
	.align	2
	.global	VENC_HAL_CfgSmmuReg
	.type	VENC_HAL_CfgSmmuReg, %function
VENC_HAL_CfgSmmuReg:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r2, r0
	mov	r0, #0
	ldr	r3, [r2, #16]
	ldr	r1, [r2, #2628]
	add	r3, r3, #61440
	str	r1, [r3, #520]
	ldr	r1, [r2, #2628]
	str	r1, [r3, #524]
	ldr	r1, [r2, #2636]
	str	r1, [r3, #752]
	ldr	r1, [r2, #2632]
	str	r1, [r3, #756]
	ldr	r1, [r2, #2636]
	str	r1, [r3, #772]
	ldr	r2, [r2, #2632]
	str	r2, [r3, #776]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VENC_HAL_CfgSmmuReg, .-VENC_HAL_CfgSmmuReg
	.global	__aeabi_uidiv
	.align	2
	.global	VENC_HAL_ReadReg
	.type	VENC_HAL_ReadReg, %function
VENC_HAL_ReadReg:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	ip, [r0, #16]
	mov	r4, r0
	ldr	r3, [ip]
	and	r3, r3, #1
	str	r3, [r0, #2640]
	ldr	r3, [ip]
	ubfx	r3, r3, #1, #1
	str	r3, [r0, #2644]
	ldr	r0, [ip]
	ubfx	r0, r0, #3, #1
	str	r0, [r4, #2648]
	orrs	r3, r0, r3
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r5, ip, #4096
	ldr	r0, [r4, #20]
	ldr	r3, [r5, #1644]
	cmp	r0, #0
	bic	r3, r3, #-67108864
	str	r3, [r4, #2652]
	beq	.L13
	ldr	r3, [r5, #3712]
	mov	lr, r3, asl #3
	str	lr, [r1, #24]
	ldr	r3, [r5, #1540]
	ldr	ip, [r5, #1572]
	ubfx	r0, r3, #0, #19
	ubfx	r3, ip, #0, #17
	add	r3, r0, r3
.L8:
	add	r0, r3, #3
	cmp	r3, #0
	movlt	r3, r0
	mov	r3, r3, asr #2
	str	r3, [r1, #108]
	ldr	r3, [r2, #104]
	add	r3, lr, r3
	str	r3, [r2, #104]
	ldr	r0, [r1, #24]
	cmp	r3, r0
	strls	r0, [r2, #104]
	ldrhi	r3, [r2, #108]
	movls	r3, #1
	addhi	r3, r3, #1
	str	r3, [r2, #108]
	ldr	r3, [r4, #20]
	cmp	r3, #0
	ldr	r3, [r1, #16]
	ldr	r1, [r1, #12]
	addeq	r3, r3, #15
	addne	r3, r3, #7
	addeq	r1, r1, #15
	addne	r1, r1, #7
	moveq	r3, r3, lsr #4
	movne	r3, r3, lsr #3
	moveq	r1, r1, lsr #4
	movne	r1, r1, lsr #3
	ldreq	r0, [r4, #2652]
	ldrne	r0, [r4, #2652]
	mul	r1, r1, r3
	bl	__aeabi_uidiv
	str	r0, [r4, #2652]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L13:
	add	ip, ip, #8192
	ldr	r3, [ip, #132]
	mov	lr, r3, asl #3
	str	lr, [r1, #24]
	ldr	r0, [r5, #1544]
	ldr	r6, [r5, #1548]
	ldr	r3, [r5, #1576]
	ubfx	ip, r0, #0, #19
	ubfx	r0, r6, #0, #19
	add	r0, ip, r0
	ubfx	r3, r3, #0, #19
	add	r3, r0, r3
	b	.L8
	UNWIND(.fnend)
	.size	VENC_HAL_ReadReg, .-VENC_HAL_ReadReg
	.align	2
	.global	VENC_HAL_VEDU_Cfg_CABAC_VLC
	.type	VENC_HAL_VEDU_Cfg_CABAC_VLC, %function
VENC_HAL_VEDU_Cfg_CABAC_VLC:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, [r1, #44]
	movw	r1, #513
	ldr	r3, [r0, #16]
	movw	ip, #9729
	ldr	lr, [r0, #2344]
	cmp	r4, #0
	add	r2, r3, #4096
	moveq	ip, r1
	mov	r1, #0
	bfi	r1, lr, #8, #3
	bfi	r1, ip, #16, #16
	str	r1, [r2, #3584]
	ldr	r4, [r0, #248]
	mov	ip, #0
	ldr	lr, [r0, #256]
	mov	r1, ip
	bfi	ip, r4, #0, #5
	mov	r4, r1
	bfi	ip, lr, #16, #16
	str	ip, [r2, #3588]
	ldr	r5, [r0, #252]
	mov	lr, r1
	ldr	ip, [r0, #260]
	bfi	r1, r5, #0, #5
	bfi	r1, ip, #16, #16
	str	r1, [r2, #3628]
	ldr	ip, [r0, #240]
	ldr	r1, [r0, #248]
	rsb	r1, r1, #16
	mov	r1, ip, asl r1
	bfi	r4, r1, #0, #16
	str	r4, [r2, #3592]
	ldr	r1, [r0, #252]
	ldr	ip, [r0, #244]
	rsb	r1, r1, #16
	mov	r1, ip, asl r1
	bfi	lr, r1, #0, #16
	str	lr, [r2, #3632]
	ldr	r1, [r0, #20]
	cmp	r1, #4
	beq	.L31
	ldr	r1, [r0, #288]
	add	r3, r3, #8192
	str	r1, [r3, #4]
	ldr	r1, [r0, #292]
	str	r1, [r3, #8]
	ldr	r1, [r0, #296]
	str	r1, [r3, #12]
	ldr	r1, [r0, #300]
	str	r1, [r3, #16]
	ldr	r1, [r0, #304]
	str	r1, [r3, #20]
	ldr	r1, [r0, #308]
	str	r1, [r3, #24]
	ldr	r1, [r0, #312]
	str	r1, [r3, #28]
	ldr	r1, [r0, #316]
	str	r1, [r3, #32]
	ldr	r1, [r0, #136]
	ldr	ip, [r3, #36]
	sub	r1, r1, #1
	bfi	ip, r1, #16, #7
	str	ip, [r3, #36]
	ldr	r1, [r0, #136]
	ldr	ip, [r3, #36]
	mov	r1, r1, lsr #8
	sub	r1, r1, #1
	bfi	ip, r1, #8, #6
	str	ip, [r3, #36]
	ldrh	r1, [r0, #138]
	ldr	ip, [r3, #36]
	sub	r1, r1, #1
	bfi	ip, r1, #0, #6
	str	ip, [r3, #36]
	ldr	r1, [r0, #20]
	cmp	r1, #4
	beq	.L32
.L24:
	ldr	r2, [r0, #144]
	str	r2, [r3, #44]
	ldr	r2, [r0, #148]
	str	r2, [r3, #48]
	ldr	r2, [r0, #152]
	str	r2, [r3, #52]
	ldr	r2, [r0, #156]
	str	r2, [r3, #56]
	ldr	r2, [r0, #160]
	str	r2, [r3, #60]
	ldr	r2, [r0, #164]
	str	r2, [r3, #64]
	ldr	r2, [r0, #168]
	str	r2, [r3, #68]
	ldr	r2, [r0, #172]
	str	r2, [r3, #72]
	ldr	r2, [r0, #140]
	ldr	r1, [r3, #76]
	sub	r2, r2, #1
	bfi	r1, r2, #16, #7
	str	r1, [r3, #76]
	ldr	r2, [r0, #140]
	ldr	r1, [r3, #76]
	mov	r2, r2, lsr #8
	sub	r2, r2, #1
	bfi	r1, r2, #8, #6
	str	r1, [r3, #76]
	ldrh	r2, [r0, #142]
	ldr	r1, [r3, #76]
	sub	r2, r2, #1
	bfi	r1, r2, #0, #6
	str	r1, [r3, #76]
.L30:
	ldr	r2, [r3, #40]
	mov	r1, #0
	bfc	r2, #1, #24
	str	r2, [r3, #40]
	ldr	r2, [r3, #40]
	bfc	r2, #0, #1
	str	r2, [r3, #40]
	str	r1, [r3, #40]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L31:
	ldr	r1, [r0, #256]
	add	r3, r3, #8192
	cmp	r1, #31
	rsbls	r1, r1, #32
	ldrls	ip, [r0, #176]
	ldrhi	r1, [r0, #176]
	movls	r1, ip, asl r1
	str	r1, [r2, #3596]
	ldr	r1, [r0, #256]
	cmp	r1, #63
	rsbls	r1, r1, #64
	ldrhi	r1, [r0, #180]
	ldrls	ip, [r0, #180]
	movls	r1, ip, asl r1
	str	r1, [r2, #3600]
	ldr	r1, [r0, #256]
	cmp	r1, #64
	rsbhi	r1, r1, #96
	ldrhi	ip, [r0, #184]
	movhi	r1, ip, asl r1
	strhi	r1, [r2, #3604]
	ldr	r1, [r0, #188]
	str	r1, [r2, #3608]
	ldr	r1, [r0, #192]
	str	r1, [r2, #3612]
	ldr	r1, [r0, #196]
	str	r1, [r2, #3616]
	ldr	r1, [r0, #200]
	str	r1, [r2, #3620]
	ldr	r1, [r0, #204]
	str	r1, [r2, #3624]
	ldr	r1, [r0, #20]
	cmp	r1, #4
	bne	.L24
.L32:
	ldr	r1, [r0, #260]
	cmp	r1, #31
	rsbls	r1, r1, #32
	ldrls	ip, [r0, #208]
	ldrhi	r1, [r0, #208]
	movls	r1, ip, asl r1
	str	r1, [r2, #3636]
	ldr	r1, [r0, #260]
	cmp	r1, #63
	rsbls	r1, r1, #64
	ldrhi	r1, [r0, #212]
	ldrls	ip, [r0, #212]
	movls	r1, ip, asl r1
	str	r1, [r2, #3640]
	ldr	r1, [r0, #260]
	cmp	r1, #64
	rsbhi	r1, r1, #96
	ldrhi	ip, [r0, #216]
	movhi	r1, ip, asl r1
	strhi	r1, [r2, #3644]
	ldr	r1, [r0, #220]
	str	r1, [r2, #3648]
	ldr	r1, [r0, #224]
	str	r1, [r2, #3652]
	ldr	r1, [r0, #228]
	str	r1, [r2, #3656]
	ldr	r1, [r0, #232]
	str	r1, [r2, #3660]
	ldr	r1, [r0, #236]
	str	r1, [r2, #3664]
	b	.L30
	UNWIND(.fnend)
	.size	VENC_HAL_VEDU_Cfg_CABAC_VLC, .-VENC_HAL_VEDU_Cfg_CABAC_VLC
	.align	2
	.global	VENC_HAL_CfgReg
	.type	VENC_HAL_CfgReg, %function
VENC_HAL_CfgReg:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #84)
	sub	sp, sp, #84
	mov	r9, r1
	ldr	r1, [r1, #68]
	mov	r2, #11
	ldr	r10, [r0, #16]
	mov	r4, r0
	bfi	r2, r1, #10, #1
	mov	r3, #0
	bfc	r2, #16, #1
	bfc	r3, #0, #1
	bfc	r2, #17, #1
	orr	r3, r3, #2
	orr	r2, r2, #134217728
	bfc	r3, #2, #1
	bfc	r2, #30, #1
	str	r2, [r10, #4]
	ldr	r0, [r0, #20]
	mov	r6, #0
	ldr	r1, [r9, #68]
	bfc	r6, #0, #9
	subs	r2, r0, #4
	ldr	lr, [r4, #2340]
	ldr	r5, [r4, #700]
	bfc	r6, #16, #9
	movne	r2, #1
	bfi	r3, r2, #3, #2
	bfi	r3, r1, #5, #1
	ldr	r2, [r4, #2336]
	ldr	r1, [r9, #44]
	bfi	r3, lr, #6, #1
	bfi	r3, r2, #7, #1
	ldr	lr, [r4, #2332]
	clz	r2, r1
	bfc	r3, #8, #1
	bfc	r3, #24, #1
	ldr	r7, [r4, #2328]
	mov	r2, r2, lsr #5
	bfi	r3, r5, #9, #1
	bfi	r3, r2, #10, #2
	cmp	r0, #4
	bfi	r3, lr, #12, #1
	ldr	lr, [r4, #420]
	bfi	r3, r1, #16, #1
	moveq	r2, #2
	orr	r3, r3, #131072
	movne	r2, #0
	bfi	r3, r7, #18, #2
	mov	r5, #0
	bfc	r3, #21, #1
	mov	r1, r5
	bfc	r3, #22, #1
	mov	r0, r5
	bfc	r3, #25, #1
	bfc	r0, #0, #5
	bfi	r3, r2, #26, #2
	mov	r2, r5
	bfi	r3, lr, #28, #2
	mov	lr, r5
	bfc	r3, #30, #1
	bfi	lr, r5, #8, #2
	bfc	r3, #31, #1
	str	r3, [r10, #20]
	str	r6, [r10, #24]
	bfi	r2, r5, #0, #1
	ldrh	r3, [r4, #24]
	bfi	lr, r5, #4, #4
	ldrb	r6, [r4]
	bfc	r0, #5, #5
	bfi	r5, r3, #17, #1
	ldr	r3, [r4, #28]
	bfi	r5, r6, #16, #1
	orr	r2, r2, #2
	bfi	r5, r3, #0, #16
	str	r5, [r10, #28]
	ldr	r3, [r9, #172]
	bfi	r2, r1, #2, #1
	ldr	r5, [r4, #2208]
	bfi	lr, r1, #0, #4
	bfi	r0, r3, #10, #6
	ldr	r3, [r4, #2188]
	bfi	r0, r5, #18, #1
	bfi	r2, r1, #3, #1
	bfi	r0, r3, #19, #1
	str	r0, [r10, #32]
	str	r1, [r10, #36]
	orr	r2, r2, #458752
	str	r1, [r10, #40]
	mov	r0, r1
	str	lr, [r10, #44]
	bfi	r0, r1, #16, #9
	ldr	r6, [r4, #412]
	mov	r5, r1
	ldr	lr, [r4, #416]
	bfi	r0, r1, #0, #9
	bfi	r2, r6, #19, #2
	mov	r3, r1
	bfi	r2, lr, #21, #1
	bfi	r2, r1, #23, #1
	str	r2, [r10, #48]
	ldr	r1, [r9, #16]
	ldr	r2, [r9, #12]
	sub	r1, r1, #1
	bfi	r5, r1, #16, #13
	sub	r2, r2, #1
	bfi	r5, r2, #0, #13
	str	r5, [r10, #52]
	str	r0, [r10, #56]
	ldr	r2, [r4, #428]
	str	r2, [r10, #60]
	ldr	r1, [r4, #436]
	ldr	r2, [r4, #440]
	bfi	r3, r1, #4, #4
	bfi	r3, r2, #0, #4
	str	r3, [r10, #64]
	ldr	r3, [r4, #20]
	cmp	r3, #4
	beq	.L58
	cmp	r3, #0
	moveq	r3, #15
	moveq	ip, #0
	bfieq	ip, r3, #16, #6
	moveq	r3, #31
	bfieq	ip, r3, #0, #7
.L38:
	mov	r1, #3
	mov	r3, #0
	bfi	r3, r1, #0, #3
	mov	r2, #1
	bfi	r3, r2, #3, #2
	str	ip, [r10, #88]
	bfi	r3, r1, #5, #3
	str	r1, [r10, #92]
	bfi	r3, r1, #8, #3
	mov	r2, #1
	str	r2, [r10, #96]
	mov	r5, #0
	str	r3, [r10, #100]
	mov	ip, r5
	ldr	r6, [r4, #408]
	mov	r0, r5
	mov	r2, r5
	mov	lr, r5
	mov	r3, r5
	bfc	r3, #0, #1
	str	r6, [r10, #184]
	bfc	r3, #1, #1
	ldr	r6, [r4, #64]
	bfc	r3, #2, #1
	bfc	r3, #3, #3
	str	r6, [r10, #188]
	ldr	r6, [r4, #68]
	str	r6, [r10, #192]
	ldr	r6, [r4, #72]
	str	r6, [r10, #196]
	ldr	r7, [r4, #48]
	ldr	r6, [r4, #52]
	bfi	r5, r7, #16, #16
	bfi	r5, r6, #0, #16
	str	r5, [r10, #208]
	ldr	r5, [r9, #52]
	add	r5, r4, r5, lsl #2
	ldr	r5, [r5, #124]
	str	r5, [r10, #212]
	ldr	r5, [r9, #48]
	add	r5, r4, r5, lsl #2
	ldr	r5, [r5, #76]
	str	r5, [r10, #216]
	ldr	r5, [r9, #48]
	add	r5, r4, r5, lsl #2
	ldr	r5, [r5, #84]
	str	r5, [r10, #220]
	ldr	r6, [r4, #272]
	ldr	r5, [r4, #276]
	bfi	r2, r6, #0, #16
	bfi	r2, r5, #16, #16
	str	r2, [r10, #224]
	ldr	r2, [r9, #48]
	add	r2, r4, r2, lsl #2
	ldr	r2, [r2, #92]
	str	r2, [r10, #228]
	ldr	r2, [r9, #48]
	add	r2, r4, r2, lsl #2
	ldr	r2, [r2, #100]
	str	r2, [r10, #232]
	ldr	r2, [r4, #280]
	bfi	lr, r2, #0, #16
	str	lr, [r10, #236]
	ldr	r2, [r9, #48]
	cmp	r2, ip
	moveq	r2, #76
	movne	r2, #72
	add	r2, r4, r2
	ldr	r2, [r2, #4]
	str	r2, [r10, #240]
	ldr	r2, [r9, #48]
	cmp	r2, ip
	moveq	r2, #84
	movne	r2, #80
	add	r2, r4, r2
	ldr	r2, [r2, #4]
	str	r2, [r10, #244]
	ldr	lr, [r4, #276]
	ldr	r2, [r4, #272]
	bfi	ip, lr, #0, #16
	bfi	ip, r2, #16, #16
	str	ip, [r10, #248]
	ldr	r2, [r9, #48]
	cmp	r2, r0
	moveq	r2, #92
	movne	r2, #88
	add	r2, r4, r2
	ldr	r2, [r2, #4]
	str	r2, [r10, #252]
	ldr	r2, [r9, #48]
	cmp	r2, r0
	moveq	r2, #100
	movne	r2, #96
	add	r2, r4, r2
	ldr	r2, [r2, #4]
	str	r2, [r10, #256]
	ldr	ip, [r4, #280]
	ldr	r2, [r4, #284]
	bfi	r0, ip, #16, #16
	bfi	r0, r2, #0, #16
	str	r0, [r10, #260]
	ldr	r2, [r9, #48]
	cmp	r2, #0
	moveq	r2, #116
	movne	r2, #112
	add	r2, r4, r2
	ldr	r2, [r2, #4]
	str	r2, [r10, #264]
	ldr	r2, [r9, #48]
	add	r2, r4, r2, lsl #2
	ldr	r2, [r2, #116]
	str	r2, [r10, #268]
	ldr	r2, [r9, #48]
	add	r2, r4, r2, lsl #2
	ldr	r2, [r2, #108]
	str	r2, [r10, #276]
	ldr	r2, [r9, #48]
	cmp	r2, #0
	moveq	r2, #108
	movne	r2, #104
	add	r2, r4, r2
	ldr	r2, [r2, #4]
	str	r2, [r10, #280]
	ldr	r2, [r4, #32]
	str	r2, [r10, #284]
	ldr	r2, [r4, #40]
	str	r2, [r10, #288]
	ldr	r2, [r4, #36]
	str	r2, [r10, #292]
	ldr	r2, [r4, #56]
	cmp	r2, #2
	beq	.L39
	cmp	r2, #1
	beq	.L53
	cmp	r2, #0
	bfc	r3, #6, #4
	bfc	r3, #14, #1
	bne	.L40
	ldr	r2, [r4, #60]
	clz	r2, r2
	mov	r2, r2, lsr #5
.L41:
	bfi	r3, r2, #16, #8
	str	r3, [r10, #296]
	ldr	r2, [r4, #2324]
	mov	r1, #5
	ldr	r0, [r4, #2320]
	mov	r3, #0
	bfi	r3, r2, #0, #3
	mov	r2, #0
	bfi	r2, r1, #24, #8
	bfi	r3, r0, #4, #1
	bfi	r2, r1, #16, #8
	mov	r0, #15
	bfc	r3, #5, #1
	bfi	r2, r0, #8, #8
	mov	r1, #3
	mov	r0, #0
	bfc	r1, #2, #1
	orr	r3, r3, #30464
	mov	ip, #15
	str	r3, [r10, #316]
	bfi	r2, ip, #0, #8
	orr	r1, r1, #24
	mov	r3, #3
	movt	r0, 100
	str	r3, [r10, #320]
	mov	r3, #500
	str	r1, [r10, #324]
	bfi	r0, r3, #0, #16
	str	r0, [r10, #328]
	mov	ip, #0
	str	r2, [r10, #332]
	mov	r3, ip
	ldr	r2, [r4, #712]
	mov	r0, ip
	ldr	r1, [r4, #708]
	mov	r5, r3
	bfi	ip, r2, #0, #1
	ldr	r2, [r4, #704]
	bfi	ip, r1, #1, #1
	mov	lr, r3
	bfi	ip, r2, #2, #1
	mov	r1, r3
	bfc	ip, #3, #1
	mvn	r2, #127
	bfc	ip, #4, #1
	str	ip, [r10, #352]
	ldr	ip, [r4, #8]
	bfi	r1, r2, #0, #8
	bfc	r0, #1, #1
	mov	r7, #2
	mov	r6, #128
	bfi	r1, r2, #8, #8
	str	ip, [r10, #392]
	bfi	r0, r7, #16, #8
	ldr	ip, [r4, #2180]
	bfi	r5, r2, #0, #8
	ldr	r8, [r4, #2184]
	bfi	lr, r6, #0, #10
	bfi	r3, ip, #0, #1
	ldr	ip, [r4, #2196]
	bfi	r3, r8, #1, #1
	ldr	r8, [r4, #2204]
	bfi	r3, ip, #2, #1
	ldr	ip, [r4, #2176]
	bfi	r3, r8, #4, #1
	ldr	r8, [r4, #2172]
	bfi	r3, ip, #8, #1
	ldr	ip, [r4, #2252]
	bfi	r3, r8, #9, #1
	ldr	r8, [r4, #2256]
	bfi	r3, ip, #10, #1
	ldr	ip, [r4, #2260]
	bfi	r3, r8, #11, #1
	ldr	r8, [r4, #2168]
	bfi	r3, ip, #12, #1
	ldr	ip, [r4, #2164]
	bfi	r3, r8, #13, #1
	ldr	r8, [r4, #2264]
	bfi	r3, ip, #14, #1
	mov	ip, #0
	bfc	ip, #0, #1
	bfi	r3, r8, #15, #1
	bfi	r0, r7, #24, #8
	str	r3, [r10, #440]
	bfi	r1, r2, #16, #8
	str	r0, [r10, #444]
	bfi	r5, r2, #8, #8
	str	r1, [r10, #464]
	bfi	lr, r6, #10, #10
	str	r5, [r10, #468]
	bfc	ip, #1, #8
	str	lr, [r10, #472]
	str	ip, [r10, #476]
	movw	r2, #43691
	ldr	r3, [r9, #52]
	movt	r2, 43690
	mov	r1, #1
	mov	r0, #0
	add	r3, r3, r7
	str	r0, [fp, #-68]
	mov	lr, r0
	mov	ip, r0
	umull	r6, r7, r3, r2
	mov	r8, r0
	str	r0, [fp, #-72]
	mov	r5, r7, lsr r1
	add	r5, r5, r5, lsl #1
	rsb	r3, r5, r3
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #124]
	str	r3, [r10, #480]
	ldr	r5, [r9, #52]
	add	r5, r5, r1
	umull	r2, r3, r5, r2
	mov	r3, r3, lsr r1
	add	r3, r3, r3, lsl #1
	rsb	r3, r3, r5
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #124]
	str	r3, [r10, #484]
	str	r1, [r10, #492]
	ldrb	r3, [r4, #464]
	ldrb	r5, [r4, #472]
	ldrb	r1, [r4, #468]
	mov	r2, r3
	ldrb	r6, [r4, #460]
	orr	r3, r1, r5
	ldrb	r7, [r4, #456]
	str	r2, [fp, #-52]
	orr	r3, r2, r3
	ldrb	r2, [r4, #452]
	orr	r3, r6, r3
	str	r1, [fp, #-48]
	orr	r3, r7, r3
	ldrb	r1, [r4, #448]
	orr	r3, r2, r3
	str	r2, [fp, #-56]
	ldrb	r2, [r4, #444]
	orr	r3, r1, r3
	str	r1, [fp, #-60]
	orr	r3, r2, r3
	bfi	r0, r3, #24, #1
	ldr	r3, [r4, #696]
	str	r2, [fp, #-64]
	mov	r2, lr
	bfi	r0, r3, #23, #1
	ldr	r3, [r4, #692]
	bfi	r0, r3, #22, #1
	ldr	r3, [r4, #688]
	bfi	r0, r3, #21, #1
	ldr	r3, [r4, #684]
	bfi	r0, r3, #20, #1
	ldr	r3, [r4, #680]
	bfi	r0, r3, #19, #1
	ldr	r3, [r4, #676]
	bfi	r0, r3, #18, #1
	ldr	r3, [r4, #672]
	bfi	r0, r3, #17, #1
	ldr	r3, [r4, #668]
	bfi	r0, r3, #16, #1
	ldr	r3, [r4, #504]
	bfi	r0, r3, #15, #1
	ldr	r3, [r4, #500]
	bfi	r0, r3, #14, #1
	ldr	r3, [r4, #496]
	bfi	r0, r3, #13, #1
	ldr	r3, [r4, #492]
	bfi	r0, r3, #12, #1
	ldr	r3, [r4, #488]
	bfi	r0, r3, #11, #1
	ldr	r3, [r4, #484]
	ldr	r1, [fp, #-48]
	bfi	r0, r3, #10, #1
	ldr	r3, [r4, #480]
	str	lr, [fp, #-48]
	bfi	r0, r3, #9, #1
	ldr	r3, [r4, #476]
	bfi	r0, r3, #8, #1
	ldr	r3, [fp, #-52]
	bfi	r0, r5, #7, #1
	ldr	r5, [fp, #-64]
	bfi	r0, r1, #6, #1
	mov	r1, lr
	bfi	r0, r3, #5, #1
	ldr	r3, [fp, #-56]
	bfi	r0, r6, #4, #1
	str	ip, [fp, #-52]
	bfi	r0, r7, #3, #1
	ldr	r7, [fp, #-60]
	bfi	r0, r3, #2, #1
	mov	r3, lr
	bfi	r0, r7, #1, #1
	mov	r7, lr
	bfi	r0, r5, #0, #1
	str	r0, [r10, #512]
	ldr	r0, [r4, #520]
	ldr	r5, [r4, #516]
	ldr	r6, [r4, #512]
	bfi	lr, r0, #24, #7
	bfi	lr, r5, #16, #7
	ldr	r0, [r4, #508]
	bfi	lr, r6, #8, #7
	mov	r6, r1
	bfi	lr, r0, #0, #7
	str	lr, [r10, #516]
	ldr	r0, [r4, #536]
	ldr	lr, [r4, #532]
	ldr	r5, [r4, #528]
	bfi	ip, r0, #24, #7
	bfi	ip, lr, #16, #7
	ldr	r0, [r4, #524]
	bfi	ip, r5, #8, #7
	mov	r5, r8
	bfi	ip, r0, #0, #7
	str	ip, [r10, #520]
	ldr	ip, [r4, #572]
	mov	lr, r1
	ldr	r0, [r4, #540]
	bfi	r8, ip, #16, #10
	bfi	r8, r0, #0, #10
	str	r8, [r10, #524]
	ldr	ip, [r4, #576]
	mov	r8, r1
	ldr	r0, [r4, #544]
	bfi	r8, ip, #16, #10
	mov	ip, r1
	bfi	r8, r0, #0, #10
	str	r8, [r10, #528]
	ldr	r8, [r4, #580]
	ldr	r0, [r4, #548]
	bfi	r6, r8, #16, #10
	bfi	r6, r0, #0, #10
	str	r6, [r10, #532]
	ldr	r6, [r4, #584]
	mov	r0, r1
	ldr	r8, [r4, #552]
	bfi	r1, r6, #16, #10
	bfi	r1, r8, #0, #10
	str	r1, [r10, #536]
	ldr	r8, [r4, #556]
	mov	r1, r2
	ldr	r6, [r4, #588]
	bfi	r2, r6, #16, #10
	bfi	r2, r8, #0, #10
	str	r2, [r10, #540]
	ldr	r6, [r4, #592]
	mov	r2, r3
	ldr	r8, [r4, #560]
	bfi	r3, r6, #16, #10
	bfi	r3, r8, #0, #10
	str	r3, [r10, #544]
	ldr	r6, [r4, #596]
	mov	r3, r7
	ldr	r8, [r4, #564]
	bfi	r7, r6, #16, #10
	mov	r6, r5
	bfi	r7, r8, #0, #10
	str	r7, [r10, #548]
	ldr	r8, [r4, #600]
	ldr	r7, [r4, #568]
	bfi	r6, r8, #16, #10
	bfi	r6, r7, #0, #10
	str	r6, [r10, #552]
	ldr	r8, [r4, #636]
	mov	r6, r5
	ldr	r7, [r4, #604]
	bfi	r6, r8, #16, #9
	bfi	r6, r7, #0, #9
	str	r6, [r10, #556]
	ldr	r7, [r4, #640]
	ldr	r6, [r4, #608]
	bfi	r5, r7, #16, #9
	bfi	r5, r6, #0, #9
	str	r5, [r10, #560]
	ldr	r6, [r4, #644]
	ldr	r5, [r4, #612]
	bfi	lr, r6, #16, #9
	bfi	lr, r5, #0, #9
	str	lr, [r10, #564]
	ldr	r5, [r4, #648]
	ldr	lr, [r4, #616]
	bfi	ip, r5, #16, #9
	bfi	ip, lr, #0, #9
	str	ip, [r10, #568]
	ldr	lr, [r4, #652]
	ldr	ip, [r4, #620]
	bfi	r0, lr, #16, #9
	bfi	r0, ip, #0, #9
	str	r0, [r10, #572]
	ldr	ip, [r4, #656]
	ldr	r0, [r4, #624]
	bfi	r1, ip, #16, #9
	bfi	r1, r0, #0, #9
	str	r1, [r10, #576]
	ldr	r0, [r4, #660]
	ldr	r1, [r4, #628]
	bfi	r2, r0, #16, #9
	bfi	r2, r1, #0, #9
	str	r2, [r10, #580]
	ldr	r1, [r4, #664]
	ldr	r2, [r4, #632]
	bfi	r3, r1, #16, #9
	bfi	r3, r2, #0, #9
	str	r3, [r10, #584]
	ldr	r3, [r4, #20]
	ldr	r0, [r9, #12]
	cmp	r3, #0
	sub	r0, r0, #1
	moveq	r5, #16
	movne	r5, #64
	mov	r1, r5
	add	r0, r0, r5
	bl	__aeabi_uidiv
	ldr	r3, [r9, #16]
	mov	r1, r5
	mov	r6, r0
	sub	r0, r3, #1
	add	r0, r0, r5
	bl	__aeabi_uidiv
	mul	r1, r6, r0
	ldr	r0, [r9, #168]
	bl	__aeabi_uidiv
	mov	r3, #0
	mov	r8, r3
	mov	r2, r3
	mov	r6, r3
	mov	r1, r3
	str	r3, [fp, #-60]
	mov	ip, r3
	str	r3, [fp, #-64]
	mov	lr, r3
	str	r3, [fp, #-68]
	mov	r5, r3
	str	r3, [fp, #-72]
	mov	r7, r3
	str	r3, [fp, #-76]
	str	r3, [fp, #-80]
	str	r3, [fp, #-84]
	str	r3, [fp, #-88]
	str	r3, [fp, #-92]
	str	r3, [fp, #-96]
	str	r3, [fp, #-100]
	str	r3, [fp, #-104]
	str	r3, [fp, #-108]
	str	r3, [fp, #-112]
	str	r3, [fp, #-116]
	str	r3, [fp, #-120]
	bfi	r3, r0, #0, #20
	str	r3, [r10, #588]
	ldr	r0, [r4, #716]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #720]
	bfi	r3, r0, #0, #10
	str	r3, [r10, #640]
	ldr	r0, [r4, #724]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #728]
	bfi	r3, r0, #0, #8
	str	r3, [r10, #644]
	ldr	r0, [r4, #732]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #736]
	bfi	r3, r0, #0, #10
	str	r3, [r10, #648]
	ldr	r0, [r4, #740]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #744]
	bfi	r3, r0, #0, #8
	str	r3, [r10, #652]
	ldr	r0, [r4, #748]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #752]
	bfi	r3, r0, #0, #10
	str	r3, [r10, #656]
	ldr	r0, [r4, #756]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #760]
	bfi	r3, r0, #0, #8
	str	r3, [r10, #660]
	ldr	r0, [r4, #764]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #768]
	bfi	r3, r0, #0, #10
	str	r3, [r10, #664]
	ldr	r0, [r4, #772]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #776]
	bfi	r3, r0, #0, #8
	str	r3, [r10, #668]
	ldr	r0, [r4, #780]
	mov	r3, r6
	bfi	r5, r0, #12, #4
	ldr	r0, [r4, #784]
	bfi	r5, r0, #8, #4
	ldr	r0, [r4, #788]
	bfi	r5, r0, #4, #4
	ldr	r0, [r4, #792]
	bfi	r5, r0, #0, #4
	ldr	r0, [r4, #796]
	bfi	r5, r0, #28, #4
	ldr	r0, [r4, #800]
	bfi	r5, r0, #24, #4
	ldr	r0, [r4, #804]
	bfi	r5, r0, #20, #4
	ldr	r0, [r4, #808]
	bfi	r5, r0, #16, #4
	str	r5, [r10, #672]
	ldr	r0, [r4, #812]
	mov	r5, r8
	bfi	lr, r0, #12, #4
	ldr	r0, [r4, #816]
	bfi	lr, r0, #8, #4
	ldr	r0, [r4, #820]
	bfi	lr, r0, #4, #4
	ldr	r0, [r4, #824]
	bfi	lr, r0, #0, #4
	ldr	r0, [r4, #828]
	bfi	lr, r0, #28, #4
	ldr	r0, [r4, #832]
	bfi	lr, r0, #24, #4
	ldr	r0, [r4, #836]
	bfi	lr, r0, #20, #4
	ldr	r0, [r4, #840]
	bfi	lr, r0, #16, #4
	str	lr, [r10, #676]
	ldr	r0, [r4, #844]
	mov	lr, r8
	bfi	ip, r0, #12, #4
	ldr	r0, [r4, #848]
	bfi	ip, r0, #8, #4
	ldr	r0, [r4, #852]
	bfi	ip, r0, #4, #4
	ldr	r0, [r4, #856]
	bfi	ip, r0, #0, #4
	ldr	r0, [r4, #860]
	bfi	ip, r0, #28, #4
	ldr	r0, [r4, #864]
	bfi	ip, r0, #24, #4
	ldr	r0, [r4, #868]
	bfi	ip, r0, #20, #4
	ldr	r0, [r4, #872]
	bfi	ip, r0, #16, #4
	str	ip, [r10, #680]
	ldr	r0, [r4, #876]
	mov	ip, r6
	bfi	r1, r0, #12, #4
	ldr	r0, [r4, #880]
	bfi	r1, r0, #8, #4
	ldr	r0, [r4, #884]
	bfi	r1, r0, #4, #4
	ldr	r0, [r4, #888]
	bfi	r1, r0, #0, #4
	ldr	r0, [r4, #892]
	bfi	r1, r0, #28, #4
	ldr	r0, [r4, #896]
	bfi	r1, r0, #24, #4
	ldr	r0, [r4, #900]
	bfi	r1, r0, #20, #4
	ldr	r0, [r4, #904]
	bfi	r1, r0, #16, #4
	str	r1, [r10, #684]
	ldr	r1, [r4, #908]
	mov	r0, r8
	bfi	r2, r1, #12, #4
	ldr	r1, [r4, #912]
	bfi	r2, r1, #8, #4
	ldr	r1, [r4, #916]
	bfi	r2, r1, #4, #4
	ldr	r1, [r4, #920]
	bfi	r2, r1, #0, #4
	ldr	r1, [r4, #924]
	bfi	r2, r1, #28, #4
	ldr	r1, [r4, #928]
	bfi	r2, r1, #24, #4
	ldr	r1, [r4, #932]
	bfi	r2, r1, #20, #4
	ldr	r1, [r4, #936]
	bfi	r2, r1, #16, #4
	str	r2, [r10, #688]
	ldr	r2, [r4, #940]
	mov	r1, r8
	bfi	r3, r2, #12, #4
	ldr	r2, [r4, #944]
	bfi	r3, r2, #8, #4
	ldr	r2, [r4, #948]
	bfi	r3, r2, #4, #4
	ldr	r2, [r4, #952]
	bfi	r3, r2, #0, #4
	ldr	r2, [r4, #956]
	bfi	r3, r2, #28, #4
	ldr	r2, [r4, #960]
	bfi	r3, r2, #24, #4
	ldr	r2, [r4, #964]
	bfi	r3, r2, #20, #4
	ldr	r2, [r4, #968]
	bfi	r3, r2, #16, #4
	str	r3, [r10, #692]
	ldr	r3, [r4, #972]
	mov	r2, r6
	bfi	r2, r3, #12, #4
	ldr	r3, [r4, #976]
	bfi	r2, r3, #8, #4
	ldr	r3, [r4, #980]
	bfi	r2, r3, #4, #4
	ldr	r3, [r4, #984]
	bfi	r2, r3, #0, #4
	ldr	r3, [r4, #988]
	bfi	r2, r3, #28, #4
	ldr	r3, [r4, #992]
	bfi	r2, r3, #24, #4
	ldr	r3, [r4, #996]
	bfi	r2, r3, #20, #4
	ldr	r3, [r4, #1000]
	bfi	r2, r3, #16, #4
	str	r2, [r10, #696]
	ldr	r3, [r4, #1004]
	mov	r2, r8
	bfi	r7, r3, #12, #4
	ldr	r3, [r4, #1008]
	bfi	r7, r3, #8, #4
	ldr	r3, [r4, #1012]
	bfi	r7, r3, #4, #4
	ldr	r3, [r4, #1016]
	bfi	r7, r3, #0, #4
	ldr	r3, [r4, #1020]
	bfi	r7, r3, #28, #4
	ldr	r3, [r4, #1024]
	bfi	r7, r3, #24, #4
	ldr	r3, [r4, #1028]
	bfi	r7, r3, #20, #4
	ldr	r3, [r4, #1032]
	bfi	r7, r3, #16, #4
	str	r7, [r10, #700]
	ldr	r3, [r4, #1036]
	mov	r7, r8
	bfi	r6, r3, #12, #4
	ldr	r3, [r4, #1040]
	bfi	r6, r3, #8, #4
	ldr	r3, [r4, #1044]
	bfi	r6, r3, #4, #4
	ldr	r3, [r4, #1048]
	bfi	r6, r3, #0, #4
	ldr	r3, [r4, #1052]
	bfi	r6, r3, #28, #4
	ldr	r3, [r4, #1056]
	bfi	r6, r3, #24, #4
	ldr	r3, [r4, #1060]
	bfi	r6, r3, #20, #4
	ldr	r3, [r4, #1064]
	bfi	r6, r3, #16, #4
	str	r6, [r10, #704]
	ldr	r3, [r4, #1068]
	mov	r6, r8
	bfi	r2, r3, #0, #3
	ldr	r3, [r4, #1072]
	bfi	r2, r3, #3, #7
	ldr	r3, [r4, #1076]
	bfi	r2, r3, #10, #3
	ldr	r3, [r4, #1080]
	bfi	r2, r3, #13, #5
	ldr	r3, [r4, #1084]
	bfi	r2, r3, #18, #4
	ldr	r3, [r4, #1088]
	bfi	r2, r3, #22, #4
	ldr	r3, [r4, #1092]
	bfi	r2, r3, #26, #4
	str	r2, [r10, #708]
	ldr	r3, [r4, #1096]
	mov	r2, r8
	bfi	r1, r3, #0, #3
	ldr	r3, [r4, #1100]
	bfi	r1, r3, #3, #7
	ldr	r3, [r4, #1104]
	bfi	r1, r3, #10, #3
	ldr	r3, [r4, #1108]
	bfi	r1, r3, #13, #5
	ldr	r3, [r4, #1112]
	bfi	r1, r3, #18, #4
	ldr	r3, [r4, #1116]
	bfi	r1, r3, #22, #4
	ldr	r3, [r4, #1120]
	bfi	r1, r3, #26, #4
	str	r1, [r10, #712]
	ldr	r3, [r4, #1124]
	mov	r1, r8
	bfi	r1, r3, #0, #3
	ldr	r3, [r4, #1128]
	bfi	r1, r3, #3, #7
	ldr	r3, [r4, #1132]
	bfi	r1, r3, #10, #3
	ldr	r3, [r4, #1136]
	bfi	r1, r3, #13, #5
	ldr	r3, [r4, #1140]
	bfi	r1, r3, #18, #4
	ldr	r3, [r4, #1144]
	bfi	r1, r3, #22, #4
	ldr	r3, [r4, #1148]
	bfi	r1, r3, #26, #4
	str	r1, [r10, #716]
	ldr	r3, [r4, #1424]
	mov	r1, r8
	bfi	r5, r3, #0, #3
	ldr	r3, [r4, #1428]
	bfi	r5, r3, #3, #7
	ldr	r3, [r4, #1432]
	bfi	r5, r3, #10, #3
	ldr	r3, [r4, #1436]
	bfi	r5, r3, #13, #5
	ldr	r3, [r4, #1440]
	bfi	r5, r3, #18, #4
	ldr	r3, [r4, #1444]
	bfi	r5, r3, #22, #4
	ldr	r3, [r4, #1448]
	bfi	r5, r3, #26, #4
	str	r5, [r10, #720]
	ldr	r3, [r4, #1452]
	mov	r5, r8
	bfi	r5, r3, #0, #3
	ldr	r3, [r4, #1456]
	bfi	r5, r3, #3, #7
	ldr	r3, [r4, #1460]
	bfi	r5, r3, #10, #3
	ldr	r3, [r4, #1464]
	bfi	r5, r3, #13, #5
	ldr	r3, [r4, #1468]
	bfi	r5, r3, #18, #4
	ldr	r3, [r4, #1472]
	bfi	r5, r3, #22, #4
	ldr	r3, [r4, #1476]
	bfi	r5, r3, #26, #4
	str	r5, [r10, #724]
	ldr	r5, [r4, #1480]
	mov	r3, r8
	bfi	r7, r5, #0, #3
	ldr	r5, [r4, #1484]
	bfi	r7, r5, #3, #7
	ldr	r5, [r4, #1488]
	bfi	r7, r5, #10, #3
	ldr	r5, [r4, #1492]
	bfi	r7, r5, #13, #5
	ldr	r5, [r4, #1496]
	bfi	r7, r5, #18, #4
	ldr	r5, [r4, #1500]
	bfi	r7, r5, #22, #4
	ldr	r5, [r4, #1504]
	bfi	r7, r5, #26, #4
	str	r7, [r10, #728]
	ldr	r5, [r4, #1508]
	mov	r7, r8
	bfi	r7, r5, #0, #3
	ldr	r5, [r4, #1512]
	bfi	r7, r5, #3, #7
	ldr	r5, [r4, #1516]
	bfi	r7, r5, #10, #3
	ldr	r5, [r4, #1520]
	bfi	r7, r5, #13, #5
	ldr	r5, [r4, #1524]
	bfi	r7, r5, #18, #4
	ldr	r5, [r4, #1528]
	bfi	r7, r5, #22, #4
	ldr	r5, [r4, #1532]
	bfi	r7, r5, #26, #4
	str	r7, [r10, #732]
	ldr	r5, [r4, #1152]
	mov	r7, r8
	bfi	r8, r5, #0, #4
	ldr	r5, [r4, #1156]
	bfi	r8, r5, #4, #4
	ldr	r5, [r4, #1160]
	bfi	r8, r5, #8, #4
	ldr	r5, [r4, #1164]
	bfi	r8, r5, #12, #4
	ldr	r5, [r4, #1168]
	bfi	r8, r5, #16, #4
	ldr	r5, [r4, #1172]
	bfi	r8, r5, #20, #4
	ldr	r5, [r4, #1176]
	bfi	r8, r5, #24, #4
	ldr	r5, [r4, #1180]
	bfi	r8, r5, #28, #4
	str	r8, [r10, #736]
	ldr	r5, [r4, #1184]
	mov	r8, ip
	bfi	r7, r5, #0, #4
	ldr	r5, [r4, #1188]
	bfi	r7, r5, #4, #4
	ldr	r5, [r4, #1192]
	bfi	r7, r5, #8, #4
	ldr	r5, [r4, #1196]
	bfi	r7, r5, #12, #4
	ldr	r5, [r4, #1200]
	bfi	r7, r5, #16, #4
	ldr	r5, [r4, #1204]
	bfi	r7, r5, #20, #4
	str	r7, [r10, #740]
	ldr	r5, [r4, #1208]
	mov	r7, ip
	bfi	ip, r5, #0, #10
	ldr	r5, [r4, #1212]
	bfi	ip, r5, #16, #5
	str	ip, [r10, #744]
	ldr	ip, [r4, #1216]
	mov	r5, lr
	bfi	lr, ip, #0, #8
	ldr	ip, [r4, #1220]
	bfi	lr, ip, #16, #5
	str	lr, [r10, #748]
	ldr	ip, [r4, #1224]
	mov	lr, r2
	bfi	r2, ip, #0, #10
	ldr	ip, [r4, #1228]
	bfi	r2, ip, #16, #5
	str	r2, [r10, #752]
	ldr	r2, [r4, #1232]
	mov	ip, r0
	bfi	r0, r2, #0, #8
	ldr	r2, [r4, #1236]
	bfi	r0, r2, #16, #5
	str	r0, [r10, #756]
	ldr	r2, [r4, #1240]
	mov	r0, r1
	bfi	r1, r2, #0, #10
	ldr	r2, [r4, #1244]
	bfi	r1, r2, #16, #5
	str	r1, [r10, #760]
	ldr	r2, [r4, #1248]
	mov	r1, r6
	bfi	r6, r2, #0, #8
	ldr	r2, [r4, #1252]
	bfi	r6, r2, #16, #5
	str	r6, [r10, #764]
	ldr	r6, [r4, #1536]
	mov	r2, r3
	bfi	r3, r6, #0, #4
	ldr	r6, [r4, #1540]
	bfi	r3, r6, #4, #4
	ldr	r6, [r4, #1544]
	bfi	r3, r6, #8, #4
	ldr	r6, [r4, #1548]
	bfi	r3, r6, #12, #4
	ldr	r6, [r4, #1552]
	bfi	r3, r6, #16, #4
	ldr	r6, [r4, #1556]
	bfi	r3, r6, #20, #4
	str	r3, [r10, #768]
	ldr	r6, [r4, #1560]
	mov	r3, r8
	bfi	r7, r6, #0, #12
	ldr	r6, [r4, #1564]
	bfi	r7, r6, #12, #12
	ldr	r6, [r4, #1568]
	bfi	r7, r6, #24, #8
	mov	r6, r8
	str	r7, [r10, #772]
	bfc	r6, #14, #1
	str	r6, [fp, #-48]
	mov	r7, r8
	ldr	r6, [r4, #1572]
	bfi	r7, r6, #0, #12
	ldr	r6, [fp, #-48]
	bfc	r6, #15, #1
	str	r6, [fp, #-48]
	ldr	r6, [r4, #1576]
	bfi	r7, r6, #12, #12
	ldr	r6, [r4, #1580]
	bfi	r7, r6, #24, #8
	str	r7, [r10, #776]
	ldr	r6, [r4, #1584]
	mov	r7, r8
	bfc	r7, #0, #8
	bfi	r8, r6, #0, #12
	ldr	r6, [r4, #1588]
	orr	r7, r7, #65280
	bfi	r8, r6, #12, #12
	ldr	r6, [r4, #1592]
	movt	r7, 65280
	str	r7, [fp, #-52]
	bfi	r8, r6, #24, #8
	str	r8, [r10, #780]
	ldr	r8, [r4, #1596]
	mov	r7, r5
	bfi	r7, r8, #0, #8
	ldr	r8, [r4, #1600]
	bfi	r7, r8, #8, #8
	ldr	r8, [r4, #1604]
	bfi	r7, r8, #16, #8
	str	r7, [r10, #784]
	ldr	r7, [r4, #1256]
	mov	r8, r5
	mov	r6, r8
	bfi	r5, r7, #0, #1
	ldr	r7, [r4, #1260]
	bfi	r5, r7, #1, #3
	ldr	r7, [r4, #1264]
	bfi	r5, r7, #4, #6
	ldr	r7, [r4, #1268]
	bfi	r5, r7, #10, #5
	ldr	r7, [r4, #1272]
	bfi	r5, r7, #15, #6
	ldr	r7, [r4, #1276]
	bfi	r5, r7, #21, #5
	str	r5, [r10, #788]
	ldr	r5, [r4, #1280]
	mov	r7, lr
	bfi	lr, r5, #0, #1
	ldr	r5, [r4, #1284]
	bfi	lr, r5, #1, #3
	ldr	r5, [r4, #1288]
	bfi	lr, r5, #4, #6
	ldr	r5, [r4, #1292]
	bfi	lr, r5, #10, #5
	ldr	r5, [r4, #1296]
	bfi	lr, r5, #15, #6
	ldr	r5, [r4, #1300]
	bfi	lr, r5, #21, #5
	str	lr, [r10, #792]
	ldr	lr, [r4, #1304]
	mov	r5, ip
	bfi	ip, lr, #0, #1
	ldr	lr, [r4, #1308]
	bfi	ip, lr, #1, #3
	ldr	lr, [r4, #1312]
	bfi	ip, lr, #4, #6
	ldr	lr, [r4, #1316]
	bfi	ip, lr, #10, #5
	ldr	lr, [r4, #1320]
	bfi	ip, lr, #15, #6
	ldr	lr, [r4, #1324]
	bfi	ip, lr, #21, #5
	str	ip, [r10, #796]
	ldr	ip, [r4, #1328]
	mov	lr, r8
	bfi	r0, ip, #0, #1
	ldr	ip, [r4, #1332]
	bfi	r0, ip, #1, #3
	ldr	ip, [r4, #1336]
	bfi	r0, ip, #4, #6
	ldr	ip, [r4, #1340]
	bfi	r0, ip, #10, #5
	ldr	ip, [r4, #1344]
	bfi	r0, ip, #15, #6
	ldr	ip, [r4, #1348]
	bfi	r0, ip, #21, #5
	str	r0, [r10, #800]
	ldr	r0, [r4, #1352]
	mov	ip, r8
	bfi	r1, r0, #0, #1
	ldr	r0, [r4, #1356]
	bfi	r1, r0, #1, #3
	ldr	r0, [r4, #1360]
	bfi	r1, r0, #4, #6
	ldr	r0, [r4, #1364]
	bfi	r1, r0, #10, #5
	ldr	r0, [r4, #1368]
	bfi	r1, r0, #15, #6
	ldr	r0, [r4, #1372]
	bfi	r1, r0, #21, #5
	str	r1, [r10, #804]
	ldr	r0, [r4, #1376]
	mov	r1, r2
	bfi	r2, r0, #0, #1
	ldr	r0, [r4, #1380]
	bfi	r2, r0, #1, #3
	ldr	r0, [r4, #1384]
	bfi	r2, r0, #4, #6
	ldr	r0, [r4, #1388]
	bfi	r2, r0, #10, #5
	ldr	r0, [r4, #1392]
	bfi	r2, r0, #15, #6
	ldr	r0, [r4, #1396]
	bfi	r2, r0, #21, #5
	str	r2, [r10, #808]
	ldr	r2, [r4, #1400]
	mov	r0, r7
	bfi	r3, r2, #0, #1
	ldr	r2, [r4, #1404]
	bfi	r3, r2, #1, #3
	ldr	r2, [r4, #1408]
	bfi	r3, r2, #4, #6
	ldr	r2, [r4, #1412]
	bfi	r3, r2, #10, #5
	ldr	r2, [r4, #1416]
	bfi	r3, r2, #15, #6
	ldr	r2, [r4, #1420]
	bfi	r3, r2, #21, #5
	str	r3, [r10, #812]
	ldr	r3, [r4, #404]
	ldr	r2, [fp, #-48]
	str	r7, [fp, #-48]
	bfi	r2, r3, #20, #8
	ldr	r3, [r9, #68]
	bfi	r2, r3, #28, #1
	ldr	r3, [fp, #-52]
	str	r2, [r10, #1024]
	mov	r2, r8
	str	lr, [fp, #-52]
	str	r3, [r10, #1120]
	ldr	r3, [r4, #2376]
	bfi	r2, r3, #0, #1
	str	r2, [r10, #1536]
	ldr	r3, [r4, #2380]
	mov	r2, r7
	bfi	r8, r3, #0, #16
	str	r8, [r10, #1540]
	ldr	r3, [r4, #2384]
	mov	r8, r7
	bfi	r2, r3, #0, #16
	str	r2, [r10, #1544]
	ldr	r3, [r4, #2388]
	mov	r2, r7
	bfi	r2, r3, #0, #16
	str	r2, [r10, #1548]
	ldr	r3, [r4, #2392]
	mov	r2, r7
	bfi	r5, r3, #18, #9
	ldr	r3, [r4, #2396]
	bfi	r5, r3, #9, #9
	ldr	r3, [r4, #2400]
	bfi	r5, r3, #0, #9
	str	r5, [r10, #1556]
	ldr	r3, [r4, #2404]
	mov	r5, r7
	bfi	r7, r3, #16, #8
	ldr	r3, [r4, #2408]
	bfi	r7, r3, #8, #8
	ldr	r3, [r4, #2412]
	bfi	r7, r3, #0, #8
	str	r7, [r10, #1560]
	ldr	r3, [r4, #2416]
	mov	r7, #20
	bfi	r1, r3, #8, #4
	ldr	r3, [r4, #2420]
	bfi	r1, r3, #4, #4
	ldr	r3, [r4, #2424]
	bfi	r1, r3, #0, #4
	mov	r3, #3
	bfi	r1, r3, #20, #8
	mov	r3, r6
	bfi	r1, r7, #12, #8
	str	r1, [r10, #1564]
	ldr	r1, [r4, #2556]
	mov	r7, r6
	bfi	r6, r1, #0, #16
	mov	r1, lr
	bfc	r6, #16, #8
	bfc	r6, #24, #8
	str	r6, [r10, #1568]
	ldr	r6, [r4, #2572]
	str	r6, [fp, #-56]
	ldr	r6, [fp, #-56]
	bfi	lr, r6, #0, #8
	ldr	r6, [r4, #2568]
	bfi	lr, r6, #8, #8
	ldr	r6, [r4, #2564]
	str	r0, [fp, #-56]
	bfi	lr, r6, #16, #8
	ldr	r6, [r4, #2560]
	bfi	lr, r6, #24, #8
	str	lr, [r10, #1572]
	ldr	r6, [r4, #2432]
	mov	lr, ip
	bfi	ip, r6, #24, #8
	ldr	r6, [r4, #2436]
	bfi	ip, r6, #16, #8
	ldr	r6, [r4, #2440]
	bfi	ip, r6, #8, #8
	ldr	r6, [r4, #2444]
	bfi	ip, r6, #0, #8
	str	ip, [r10, #1584]
	ldr	ip, [r4, #2448]
	bfi	r0, ip, #0, #8
	ldr	ip, [r4, #2452]
	bfi	r0, ip, #8, #8
	ldr	ip, [r4, #2456]
	bfi	r0, ip, #16, #1
	ldr	ip, [r4, #2600]
	bfi	r0, ip, #17, #1
	str	r0, [r10, #1588]
	ldr	ip, [r4, #2460]
	mov	r0, r8
	bfi	r8, ip, #0, #16
	ldr	ip, [r4, #2464]
	bfi	r8, ip, #16, #16
	str	r8, [r10, #1592]
	ldr	r6, [r4, #2468]
	mov	ip, r2
	mov	r8, r5
	bfi	r2, r6, #22, #9
	ldr	r6, [r4, #2472]
	bfi	r2, r6, #17, #5
	ldr	r6, [r4, #2476]
	bfi	r2, r6, #9, #8
	ldr	r6, [r4, #2428]
	bfi	r2, r6, #0, #9
	str	r2, [r10, #1596]
	ldr	r6, [r4, #2480]
	mov	r2, r5
	bfi	r8, r6, #8, #14
	ldr	r6, [r4, #2484]
	bfi	r8, r6, #0, #8
	str	r8, [r10, #1732]
	ldr	r8, [r4, #2488]
	mov	r6, r5
	bfi	r5, r8, #16, #8
	ldr	r8, [r4, #2492]
	bfi	r5, r8, #8, #8
	ldr	r8, [r4, #2496]
	bfi	r5, r8, #0, #8
	str	r5, [r10, #1736]
	ldr	r8, [r4, #2504]
	mov	r5, r3
	bfi	r3, r8, #17, #1
	ldr	r8, [r4, #2508]
	bfi	r3, r8, #12, #2
	ldr	r8, [r4, #2512]
	bfi	r3, r8, #4, #4
	ldr	r8, [r4, #2516]
	bfi	r3, r8, #0, #4
	str	r3, [r10, #1740]
	ldr	r3, [r4, #2520]
	mov	r8, #1
	bfi	r7, r3, #0, #10
	ldr	r3, [r4, #2524]
	bfi	r7, r3, #16, #9
	str	r7, [r10, #1744]
	ldr	r7, [r4, #2528]
	mov	r3, r1
	bfi	r3, r8, #0, #8
	bfi	r1, r7, #0, #10
	ldr	r7, [r4, #2532]
	bfi	r3, r8, #8, #8
	mov	r8, lr
	bfi	r1, r7, #16, #9
	str	r1, [r10, #1748]
	ldr	r1, [r4, #2536]
	movt	r3, 200
	ldr	r7, [r4, #2540]
	bfi	r8, r1, #0, #10
	mov	r1, lr
	bfi	r8, r7, #16, #9
	str	r8, [r10, #1752]
	ldr	r8, [r4, #2544]
	ldr	r7, [r4, #2548]
	bfi	lr, r8, #0, #10
	bfi	lr, r7, #16, #9
	str	lr, [r10, #1756]
	ldr	lr, [r4, #2552]
	mov	r7, r0
	bfi	r7, lr, #0, #16
	str	r7, [r10, #1776]
	ldr	r7, [r4, #2576]
	ldr	lr, [r4, #2580]
	bfi	r0, r7, #0, #13
	bfi	r0, lr, #13, #1
	str	r0, [r10, #1780]
	ldr	r0, [r4, #2584]
	bfi	ip, r0, #0, #18
	str	ip, [r10, #1784]
	ldr	r0, [r4, #2588]
	bfi	r2, r0, #0, #18
	str	r2, [r10, #1788]
	ldr	r0, [r4, #2612]
	ldr	r2, [r4, #2616]
	bfi	r6, r0, #16, #7
	bfi	r6, r2, #23, #7
	str	r6, [r10, #1792]
	ldr	r2, [r4, #2604]
	ldr	r0, [r4, #2608]
	bfi	r5, r2, #0, #8
	ldr	r2, [r4, #2592]
	bfi	r5, r0, #8, #8
	bfi	r5, r2, #16, #8
	str	r5, [r10, #1796]
	ldr	r2, [r4, #2596]
	ldr	r0, [r4, #2620]
	bfi	r1, r2, #12, #8
	ldr	r2, [r4, #2624]
	bfi	r1, r0, #8, #4
	bfi	r1, r2, #0, #8
	str	r1, [r10, #1800]
	str	r3, [r10, #1804]
	ldr	r2, [r4, #20]
	cmp	r2, #0
	bne	.L43
	mov	r1, #23
	mov	r3, #0
	bfi	r3, r1, #0, #5
	mov	r0, #15
	bfi	r3, r0, #5, #5
	bfi	r3, r1, #10, #5
	bfi	r3, r0, #15, #5
	bfi	r3, r2, #20, #1
	str	r3, [r10, #2052]
.L44:
	mov	r1, #16
	mov	r3, #0
	mov	r2, #0
	bfc	r3, #16, #16
	bfi	r2, r1, #6, #6
	bfc	r3, #0, #16
	mov	r0, #2
	mov	r1, #10
	str	r0, [r10, #2560]
	bfi	r2, r1, #0, #6
	str	r3, [r10, #2592]
	mov	r3, #0
	str	r2, [r10, #2596]
	mov	r1, #51
	ldr	ip, [r9, #8]
	mov	r2, #4
	ldr	r0, [r9, #4]
	bfi	r3, ip, #8, #6
	orr	r3, r3, #64
	bfi	r3, r0, #0, #6
	bfi	r3, r1, #16, #6
	bfi	r3, r2, #24, #4
	str	r3, [r10, #3076]
	ldr	r3, [r4, #20]
	ldr	r0, [r9, #12]
	cmp	r3, #0
	sub	r0, r0, #1
	moveq	r5, #16
	movne	r5, #64
	mov	r1, r5
	add	r0, r0, r5
	bl	__aeabi_uidiv
	ldr	r3, [r9, #16]
	mov	r1, r5
	mov	r6, r0
	sub	r0, r3, #1
	add	r0, r0, r5
	bl	__aeabi_uidiv
	mul	r1, r6, r0
	ldr	r0, [r9, #168]
	bl	__aeabi_uidiv
	mov	r3, #0
	mov	r8, r3
	add	r2, r10, #3152
	str	r2, [fp, #-48]
	mov	r2, r8
	mov	r7, r3
	mov	r6, r3
	str	r3, [fp, #-76]
	mov	r5, r3
	str	r3, [fp, #-52]
	mov	ip, r3
	str	r3, [fp, #-56]
	mov	lr, r3
	str	r3, [fp, #-60]
	add	r1, r4, #2128
	str	r3, [fp, #-64]
	add	r1, r1, #12
	str	r3, [fp, #-68]
	str	r3, [fp, #-72]
	add	r3, r4, #1808
	add	r3, r3, #12
	bfi	r2, r0, #0, #16
	ldr	r0, [r4, #1788]
	bfi	r2, r0, #16, #6
	str	r2, [r10, #3080]
	ldr	r0, [r4, #1820]
	mov	r2, ip
	bfi	r8, r0, #0, #6
	ldr	r0, [r4, #1780]
	bfi	r8, r0, #24, #6
	ldr	r0, [r4, #1784]
	bfi	r8, r0, #16, #6
	ldr	r0, [r4, #1792]
	bfi	r8, r0, #8, #6
	str	r8, [r10, #3084]
	ldr	r0, [r4, #1640]
	bfi	r7, r0, #24, #8
	ldr	r0, [r4, #1636]
	bfi	r7, r0, #16, #8
	ldr	r0, [r4, #1632]
	bfi	r7, r0, #8, #8
	ldr	r0, [r4, #1628]
	bfi	r7, r0, #0, #8
	str	r7, [r10, #3088]
	ldr	r0, [r4, #1656]
	bfi	r6, r0, #24, #8
	ldr	r0, [r4, #1652]
	bfi	r6, r0, #16, #8
	ldr	r0, [r4, #1648]
	bfi	r6, r0, #8, #8
	ldr	r0, [r4, #1644]
	bfi	r6, r0, #0, #8
	str	r6, [r10, #3092]
	ldr	r0, [r4, #1672]
	bfi	r5, r0, #24, #8
	ldr	r0, [r4, #1668]
	bfi	r5, r0, #16, #8
	ldr	r0, [r4, #1664]
	bfi	r5, r0, #8, #8
	ldr	r0, [r4, #1660]
	bfi	r5, r0, #0, #8
	str	r5, [r10, #3096]
	ldr	r0, [r4, #1688]
	mov	r5, ip
	bfi	r5, r0, #24, #8
	ldr	r0, [r4, #1684]
	bfi	r5, r0, #16, #8
	ldr	r0, [r4, #1680]
	bfi	r5, r0, #8, #8
	ldr	r0, [r4, #1676]
	bfi	r5, r0, #0, #8
	str	r5, [r10, #3072]
	ldr	r5, [r4, #1812]
	ldr	r0, [r4, #1816]
	bfi	r2, r5, #0, #4
	mov	r5, ip
	bfi	r2, r0, #4, #4
	str	r2, [r10, #3100]
	ldr	r0, [r4, #1764]
	mov	r2, ip
	bfi	r5, r0, #0, #4
	str	r5, [r10, #3104]
	ldr	r5, [r4, #1760]
	ldr	r0, [r4, #1768]
	bfi	r2, r5, #0, #4
	bfi	r2, r0, #4, #4
	str	r2, [r10, #3108]
	ldr	r0, [r4, #1772]
	mov	r2, ip
	bfi	r2, r0, #0, #4
	str	r2, [r10, #3112]
	ldr	r5, [r4, #1692]
	mov	r2, lr
	ldr	r0, [r4, #1696]
	bfi	ip, r5, #0, #2
	ldr	r5, [r4, #1700]
	bfi	ip, r0, #2, #2
	ldr	r0, [r4, #1704]
	bfi	ip, r5, #4, #2
	ldr	r5, [r4, #1708]
	bfi	ip, r0, #6, #2
	ldr	r0, [r4, #1712]
	bfi	ip, r5, #8, #2
	ldr	r5, [r4, #1716]
	bfi	ip, r0, #10, #2
	ldr	r0, [r4, #1720]
	bfi	ip, r5, #12, #2
	ldr	r5, [r4, #1724]
	bfi	ip, r0, #14, #2
	ldr	r0, [r4, #1728]
	bfi	ip, r5, #16, #2
	ldr	r5, [r4, #1732]
	bfi	ip, r0, #18, #2
	ldr	r0, [r4, #1736]
	bfi	ip, r5, #20, #2
	ldr	r5, [r4, #1740]
	bfi	ip, r0, #22, #2
	ldr	r0, [r4, #1744]
	bfi	ip, r5, #24, #2
	ldr	r5, [r4, #1748]
	bfi	ip, r0, #26, #2
	ldr	r0, [r4, #1752]
	bfi	ip, r5, #28, #2
	bfi	ip, r0, #30, #2
	str	ip, [r10, #3116]
	ldr	r0, [r4, #1756]
	bfi	r2, r0, #0, #5
	str	r2, [r10, #3120]
	ldr	r0, [r4, #1800]
	ldr	ip, [r4, #1804]
	bfi	lr, r0, #24, #4
	ldr	r0, [r4, #1808]
	bfi	lr, ip, #28, #4
	ldr	r2, [fp, #-48]
	ldr	ip, [r4, #1776]
	bfi	lr, r0, #16, #6
	ldr	r0, [r4, #1796]
	bfi	lr, ip, #8, #4
	bfi	lr, r0, #0, #6
	str	lr, [r10, #3124]
.L46:
	ldr	r0, [r3, #4]!
	add	r2, r2, #4
	cmp	r3, r1
	str	r0, [r2, #-4]
	bne	.L46
	add	r3, r10, #4096
	mov	r5, #0
	mov	r0, r5
	mov	r2, r5
	bfi	r0, r5, #0, #16
	bfi	r2, r5, #0, #16
	mov	r1, r5
	bfi	r1, r5, #16, #16
	str	r5, [r10, #3660]
	bfi	r0, r5, #16, #16
	bfi	r2, r5, #16, #16
	bfi	r1, r5, #0, #16
	mov	lr, r5
	mov	ip, r5
	bfi	lr, r5, #0, #1
	bfi	ip, r5, #0, #1
	str	lr, [r3, #512]
	mov	lr, #13
	str	ip, [r3, #516]
	mov	ip, r5
	str	r5, [r3, #520]
	bfi	ip, lr, #0, #4
	str	ip, [r3, #524]
	mvn	ip, #0
	str	r5, [r3, #528]
	mov	r8, r5
	str	r0, [r3, #532]
	mov	r6, r5
	str	r2, [r3, #536]
	mov	r2, #7
	str	r1, [r3, #540]
	mov	r7, r5
	str	ip, [r3, #544]
	mov	lr, r5
	str	r2, [r3, #548]
	mov	r1, r9
	str	ip, [r3, #552]
	mov	r0, r4
	str	r2, [r3, #556]
	str	ip, [r3, #560]
	str	r2, [r3, #564]
	str	ip, [r3, #568]
	str	r2, [r3, #572]
	ldr	r2, [r4, #2348]
	bfi	r8, r2, #0, #1
	str	r8, [r3, #1212]
	ldr	r2, [r4, #2352]
	str	r2, [r3, #1188]
	ldr	r2, [r4, #2356]
	str	r2, [r3, #1192]
	ldr	r2, [r4, #2360]
	str	r2, [r3, #1196]
	ldr	r2, [r4, #2364]
	str	r2, [r3, #1200]
	ldr	r2, [r4, #2368]
	str	r2, [r3, #1204]
	ldr	r2, [r4, #2372]
	str	r2, [r3, #1208]
	ldr	ip, [r4, #1608]
	ldr	r2, [r4, #1612]
	bfi	r6, ip, #0, #16
	bfi	r6, r2, #16, #16
	str	r6, [r3, #1668]
	ldr	r2, [r4, #1624]
	bfi	r7, r2, #16, #16
	str	r7, [r3, #1672]
	ldr	ip, [r4, #1616]
	ldr	r2, [r4, #1620]
	bfi	lr, ip, #0, #16
	bfi	lr, r2, #16, #16
	str	lr, [r3, #1676]
	bl	VENC_HAL_VEDU_Cfg_CABAC_VLC
	ldr	r0, [r4, #44]
	add	r3, r10, #8192
	mov	r1, r5
	mov	r2, r5
	bfi	r2, r5, #16, #8
	str	r0, [r3, #1028]
	mov	ip, #85
	ldr	r0, [r4, #424]
	bfi	r1, r0, #0, #1
	str	r1, [r3, #1040]
	ldr	r1, [r4, #432]
	str	r1, [r3, #1044]
	ldr	r1, [r9, #44]
	cmp	r1, r5
	bfieq	r2, r1, #0, #1
	orrne	r2, r2, #1
	str	r2, [r3, #1048]
	mov	r1, #0
	ldr	r0, [r4, #264]
	mvn	r2, #84
	bfi	r1, r2, #0, #8
	mov	r2, #0
	bfi	r1, ip, #8, #8
	mov	r7, r2
	str	r0, [r3, #1052]
	mov	ip, r2
	str	r2, [r3, #1056]
	mov	lr, r2
	str	r2, [r3, #1060]
	mov	r6, r2
	str	r2, [r3, #1064]
	mov	r0, r2
	str	r1, [r3, #1536]
	mov	r1, r2
	ldr	r9, [r4, #2144]
	ldr	r5, [r4, #2148]
	bfi	r7, r9, #0, #6
	ldr	r9, [r4, #2152]
	bfi	r7, r5, #8, #2
	ldr	r5, [r4, #2156]
	bfi	r7, r9, #10, #1
	ldr	r9, [r4, #2160]
	bfi	r7, r5, #12, #4
	str	r2, [fp, #-48]
	bfi	r7, r9, #16, #5
	str	r7, [r3, #4048]
	ldr	r8, [r4, #2212]
	mov	r5, r2
	bfi	r5, r2, #0, #1
	add	r9, r10, #61440
	mov	r7, r2
	bfi	r5, r2, #1, #1
	str	r8, [r3, #4052]
	bfi	r5, r2, #2, #1
	ldr	r8, [r4, #2216]
	bfi	r5, r2, #3, #1
	str	r8, [r3, #4056]
	ldr	r8, [r4, #2220]
	bfi	ip, r8, #0, #16
	ldr	r8, [r4, #2224]
	bfi	ip, r8, #16, #2
	ldr	r8, [r4, #2228]
	bfi	ip, r8, #18, #2
	ldr	r8, [r4, #2232]
	bfi	ip, r8, #20, #4
	ldr	r8, [r4, #2236]
	bfi	ip, r8, #24, #1
	ldr	r8, [r4, #2244]
	bfi	ip, r8, #25, #2
	ldr	r8, [r4, #2248]
	bfi	ip, r8, #27, #1
	ldr	r8, [r4, #2240]
	bfi	ip, r8, #28, #1
	str	ip, [r3, #4060]
	ldr	ip, [r4, #2280]
	bfi	lr, ip, #0, #1
	ldr	ip, [r4, #2276]
	bfi	lr, ip, #2, #2
	ldr	ip, [r4, #2272]
	bfi	lr, ip, #4, #2
	ldr	ip, [r4, #2268]
	bfi	lr, ip, #6, #1
	ldr	ip, [r4, #2288]
	bfi	lr, ip, #8, #6
	ldr	ip, [r4, #2292]
	bfi	lr, ip, #24, #6
	str	lr, [r3, #4064]
	ldr	ip, [r4, #2296]
	ldr	lr, [r4, #2300]
	bfi	r6, ip, #0, #6
	ldr	ip, [r4, #2308]
	bfi	r6, lr, #8, #6
	ldr	lr, [r4, #2304]
	bfi	r6, lr, #16, #6
	bfi	r6, ip, #24, #6
	str	r6, [r3, #4068]
	ldr	lr, [r4, #2312]
	mov	r6, r2
	ldr	ip, [r4, #2316]
	bfi	r6, lr, #0, #6
	mov	lr, r6
	bfi	lr, ip, #6, #25
	str	lr, [r3, #4072]
	ldr	r3, [r4, #324]
	ldr	ip, [r4, #340]
	bfi	r1, r3, #10, #1
	ldr	r3, [r4, #320]
	bfi	r1, ip, #9, #1
	ldr	ip, [r4, #344]
	bfi	r1, r3, #8, #1
	ldr	r3, [r4, #348]
	bfi	r1, ip, #7, #1
	ldr	ip, [r4, #332]
	bfi	r1, r3, #6, #1
	ldr	r3, [r4, #328]
	bfi	r1, ip, #5, #1
	ldr	ip, [r4, #352]
	bfi	r1, r3, #4, #1
	ldr	r3, [r4, #356]
	bfi	r1, ip, #3, #1
	ldr	ip, [r4, #360]
	bfi	r1, r3, #2, #1
	ldr	r3, [r4, #364]
	bfi	r1, ip, #1, #1
	bfi	r1, r3, #0, #1
	str	r1, [r10, #504]
	ldr	r1, [r4, #336]
	ldr	r3, [r4, #368]
	bfi	r0, r1, #7, #1
	ldr	r1, [r4, #372]
	bfi	r0, r3, #6, #1
	ldr	r3, [r4, #376]
	bfi	r0, r1, #5, #1
	ldr	r1, [r4, #380]
	bfi	r0, r3, #4, #1
	ldr	r3, [r4, #384]
	bfi	r0, r1, #3, #1
	ldr	r1, [r4, #388]
	bfi	r0, r3, #2, #1
	ldr	r3, [r4, #392]
	bfi	r0, r1, #1, #1
	bfi	r0, r3, #0, #1
	str	r0, [r10, #508]
	ldr	r1, [r4, #396]
	ldr	r3, [r4, #400]
	bfi	r7, r1, #0, #1
	bfi	r7, r3, #3, #1
	mov	r3, #3
	bfi	r7, r3, #16, #4
	str	r7, [r9]
	str	r5, [r9, #32]
	ldr	r3, [r4, #16]
	ldr	r1, [r4, #2628]
	add	r3, r3, #61440
	str	r1, [r3, #520]
	ldr	r1, [r4, #2628]
	str	r1, [r3, #524]
	ldr	r1, [r4, #2636]
	str	r1, [r3, #752]
	ldr	r1, [r4, #2632]
	str	r1, [r3, #756]
	ldr	r1, [r4, #2636]
	str	r1, [r3, #772]
	ldr	r1, [r4, #2632]
	str	r1, [r3, #776]
	mov	r3, #1
	str	r2, [r9, #8]
	str	r2, [r10, #16]
	str	r3, [r10, #16]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L43:
	mov	r2, #15
	mov	r3, #0
	bfi	r3, r2, #0, #5
	bfi	r3, r2, #5, #5
	bfi	r3, r2, #10, #5
	bfi	r3, r2, #15, #5
	bfc	r3, #20, #1
	str	r3, [r10, #2052]
	b	.L44
.L53:
	mov	r1, #6
.L39:
	bfi	r3, r1, #6, #4
	bfc	r3, #14, #1
.L40:
	ldrb	r2, [r4, #60]
	b	.L41
.L58:
	ldr	r2, [r9, #16]
	ldr	r3, [r9, #12]
	mov	r2, r2, lsr #2
	sub	r2, r2, #16
	mov	r3, r3, lsr #2
	sub	r3, r3, #16
	mov	r2, r2, lsr #1
	cmp	r2, #32
	mov	r3, r3, lsr #1
	movcs	r2, #32
	cmp	r3, #72
	bic	r2, r2, #-2147483645
	movcs	r3, #72
	cmp	r2, #7
	bic	r3, r3, #-2147483645
	movls	r2, #7
	bls	.L36
	cmp	r2, #32
	movcs	r2, #32
	sub	r2, r2, #1
	and	r2, r2, #63
.L36:
	cmp	r3, #7
	movls	r3, #7
	bls	.L37
	cmp	r3, #72
	movcs	r3, #72
	sub	r3, r3, #1
	and	r3, r3, #127
.L37:
	mov	ip, #0
	bfi	ip, r2, #16, #6
	bfi	ip, r3, #0, #7
	b	.L38
	UNWIND(.fnend)
	.size	VENC_HAL_CfgReg, .-VENC_HAL_CfgReg
	.align	2
	.global	VENC_HAL_VEDU_DDRCfg_CABAC_VLC
	.type	VENC_HAL_VEDU_DDRCfg_CABAC_VLC, %function
VENC_HAL_VEDU_DDRCfg_CABAC_VLC:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0, #20]
	ldr	r3, [r0, #4]
	cmp	r2, #4
	beq	.L76
	ldr	r2, [r0, #288]
	mov	r1, #0
	mov	ip, r1
	bfc	ip, #0, #1
	str	r2, [r3, #256]
	ldr	r2, [r0, #292]
	str	r2, [r3, #260]
	ldr	r2, [r0, #296]
	str	r2, [r3, #264]
	ldr	r2, [r0, #300]
	str	r2, [r3, #268]
	ldr	r2, [r0, #304]
	str	r2, [r3, #272]
	ldr	r2, [r0, #308]
	str	r2, [r3, #276]
	ldr	r2, [r0, #312]
	str	r2, [r3, #280]
	ldr	r2, [r0, #316]
	str	r2, [r3, #284]
	ldr	r2, [r0, #136]
	str	ip, [r3, #292]
	sub	lr, r2, #1
	mov	ip, r2, lsr #8
	bfi	r1, lr, #16, #7
	sub	ip, ip, #1
	mov	r2, r2, lsr #16
	sub	r2, r2, #1
	bfi	r1, ip, #8, #6
	bfi	r1, r2, #0, #6
	str	r1, [r3, #288]
	ldr	r2, [r0, #20]
	cmp	r2, #4
	beq	.L77
.L69:
	ldr	r2, [r0, #144]
	mov	r1, #0
	str	r2, [r3, #296]
	ldr	r2, [r0, #148]
	str	r2, [r3, #300]
	ldr	r2, [r0, #152]
	str	r2, [r3, #304]
	ldr	r2, [r0, #156]
	str	r2, [r3, #308]
	ldr	r2, [r0, #160]
	str	r2, [r3, #312]
	ldr	r2, [r0, #164]
	str	r2, [r3, #316]
	ldr	r2, [r0, #168]
	str	r2, [r3, #320]
	ldr	r2, [r0, #172]
	str	r2, [r3, #324]
	ldr	r2, [r0, #140]
	sub	ip, r2, #1
	mov	r0, r2, lsr #8
	bfi	r1, ip, #16, #7
	sub	r0, r0, #1
	mov	r2, r2, lsr #16
	bfi	r1, r0, #8, #6
	sub	r2, r2, #1
	bfi	r1, r2, #0, #6
	str	r1, [r3, #328]
	ldmfd	sp, {r4, fp, sp, pc}
.L76:
	ldr	r4, [r1, #44]
	mov	r2, #0
	ldr	lr, [r0, #2344]
	movw	r1, #513
	cmp	r4, r2
	movw	ip, #9729
	bfi	r2, lr, #8, #3
	moveq	ip, r1
	bfi	r2, ip, #16, #16
	str	r2, [r3, #256]
	mov	r1, #0
	ldr	ip, [r0, #248]
	mov	r2, r1
	ldr	r4, [r0, #256]
	mov	lr, r1
	bfi	r1, ip, #0, #5
	mov	ip, r2
	bfi	r1, r4, #16, #16
	str	r1, [r3, #260]
	ldr	r4, [r0, #252]
	ldr	r1, [r0, #260]
	bfi	r2, r4, #0, #5
	bfi	r2, r1, #16, #16
	str	r2, [r3, #300]
	ldr	r1, [r0, #240]
	ldr	r2, [r0, #248]
	rsb	r2, r2, #16
	mov	r2, r1, asl r2
	bfi	lr, r2, #0, #16
	str	lr, [r3, #264]
	ldr	r1, [r0, #244]
	ldr	r2, [r0, #252]
	rsb	r2, r2, #16
	mov	r2, r1, asl r2
	bfi	ip, r2, #0, #16
	str	ip, [r3, #304]
	ldr	r2, [r0, #256]
	cmp	r2, #31
	rsbls	r2, r2, #32
	ldrls	r1, [r0, #176]
	ldrhi	r2, [r0, #176]
	movls	r2, r1, asl r2
	str	r2, [r3, #268]
	ldr	r2, [r0, #256]
	cmp	r2, #63
	rsbls	r2, r2, #64
	ldrhi	r2, [r0, #180]
	ldrls	r1, [r0, #180]
	movls	r2, r1, asl r2
	str	r2, [r3, #272]
	ldr	r2, [r0, #256]
	cmp	r2, #64
	rsbhi	r2, r2, #96
	ldrhi	r1, [r0, #184]
	movhi	r2, r1, asl r2
	strhi	r2, [r3, #276]
	ldr	r2, [r0, #188]
	str	r2, [r3, #280]
	ldr	r2, [r0, #192]
	str	r2, [r3, #284]
	ldr	r2, [r0, #196]
	str	r2, [r3, #288]
	ldr	r2, [r0, #200]
	str	r2, [r3, #292]
	ldr	r2, [r0, #204]
	str	r2, [r3, #296]
	ldr	r2, [r0, #20]
	cmp	r2, #4
	bne	.L69
.L77:
	ldr	r2, [r0, #260]
	cmp	r2, #31
	rsbls	r2, r2, #32
	ldrls	r1, [r0, #208]
	ldrhi	r2, [r0, #208]
	movls	r2, r1, asl r2
	str	r2, [r3, #308]
	ldr	r2, [r0, #260]
	cmp	r2, #63
	rsbls	r2, r2, #64
	ldrhi	r2, [r0, #212]
	ldrls	r1, [r0, #212]
	movls	r2, r1, asl r2
	str	r2, [r3, #312]
	ldr	r2, [r0, #260]
	cmp	r2, #64
	rsbhi	r2, r2, #96
	ldrhi	r1, [r0, #216]
	movhi	r2, r1, asl r2
	strhi	r2, [r3, #316]
	ldr	r2, [r0, #220]
	str	r2, [r3, #320]
	ldr	r2, [r0, #224]
	str	r2, [r3, #324]
	ldr	r2, [r0, #228]
	str	r2, [r3, #328]
	ldr	r2, [r0, #232]
	str	r2, [r3, #332]
	ldr	r2, [r0, #236]
	str	r2, [r3, #336]
	ldmfd	sp, {r4, fp, sp, pc}
	UNWIND(.fnend)
	.size	VENC_HAL_VEDU_DDRCfg_CABAC_VLC, .-VENC_HAL_VEDU_DDRCfg_CABAC_VLC
	.align	2
	.global	VENC_HAL_DDRCfgReg
	.type	VENC_HAL_DDRCfgReg, %function
VENC_HAL_DDRCfgReg:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #92)
	sub	sp, sp, #92
	mov	r9, r1
	str	r1, [fp, #-52]
	mov	r2, #11
	ldr	r1, [r1, #68]
	mov	r4, r0
	ldr	r3, [r0, #16]
	mov	r6, #0
	bfi	r2, r1, #10, #1
	ldr	r10, [r0, #4]
	bfc	r2, #16, #1
	bfc	r6, #0, #9
	bfc	r2, #17, #1
	mov	r8, r3
	orr	r2, r2, #134217728
	str	r3, [fp, #-48]
	bfc	r2, #30, #1
	str	r2, [r8, #4]
	ldr	lr, [r0, #20]
	mov	r3, #0
	bfc	r3, #0, #1
	ldr	r0, [r9, #68]
	subs	r2, lr, #4
	orr	r3, r3, #6
	ldr	r1, [r4, #2340]
	bfc	r6, #16, #9
	movne	r2, #1
	bfi	r3, r2, #3, #2
	bfi	r3, r0, #5, #1
	ldr	r2, [r4, #2336]
	ldr	r0, [r9, #44]
	bfi	r3, r1, #6, #1
	bfi	r3, r2, #7, #1
	ldr	r1, [r4, #700]
	clz	r2, r0
	bfc	r3, #8, #1
	bfc	r3, #24, #1
	ldr	r7, [r4, #2332]
	mov	r2, r2, lsr #5
	bfi	r3, r1, #9, #1
	bfi	r3, r2, #10, #2
	ldr	r1, [r4, #2328]
	bfi	r3, r7, #12, #1
	cmp	lr, #4
	bfi	r3, r0, #16, #1
	ldr	lr, [r4, #420]
	orr	r3, r3, #131072
	moveq	r2, #2
	bfi	r3, r1, #18, #2
	movne	r2, #0
	bfc	r3, #21, #1
	mov	r7, #0
	bfc	r3, #22, #1
	mov	r1, r7
	bfc	r3, #25, #1
	mov	r0, r7
	bfi	r3, r2, #26, #2
	mov	r2, r7
	bfi	r3, lr, #28, #2
	mov	lr, r7
	bfc	r3, #30, #1
	bfi	lr, r7, #8, #2
	bfc	r3, #31, #1
	str	r3, [r8, #20]
	str	r6, [r10, #512]
	bfi	r2, r7, #0, #1
	ldrh	r3, [r4, #24]
	bfi	lr, r7, #4, #4
	ldrb	r6, [r4]
	bfc	r0, #0, #5
	bfi	r7, r3, #17, #1
	ldr	r3, [r4, #28]
	bfi	r7, r6, #16, #1
	bfc	r0, #5, #5
	bfi	r7, r3, #0, #16
	str	r7, [r10, #516]
	ldr	r3, [r9, #172]
	orr	r2, r2, #2
	ldr	r6, [r4, #2208]
	bfi	r2, r1, #2, #1
	bfi	r0, r3, #10, #6
	ldr	r3, [r4, #2188]
	bfi	r0, r6, #18, #1
	bfi	lr, r1, #0, #4
	bfi	r0, r3, #19, #1
	bfi	r2, r1, #3, #1
	str	r0, [r8, #32]
	orr	r2, r2, #458752
	str	r1, [r8, #36]
	mov	r0, r1
	str	r1, [r8, #40]
	bfi	r0, r1, #16, #9
	str	lr, [r10, #520]
	mov	r6, r1
	ldr	r7, [r4, #412]
	bfi	r0, r1, #0, #9
	ldr	lr, [r4, #416]
	mov	r3, r1
	bfi	r2, r7, #19, #2
	bfi	r2, lr, #21, #1
	bfi	r2, r1, #23, #1
	str	r2, [r8, #48]
	ldr	r1, [r9, #16]
	ldr	r2, [r9, #12]
	sub	r1, r1, #1
	bfi	r6, r1, #16, #13
	sub	r2, r2, #1
	bfi	r6, r2, #0, #13
	str	r6, [r10]
	str	r0, [r8, #56]
	ldr	r2, [r4, #428]
	str	r2, [r8, #60]
	ldr	r1, [r4, #436]
	ldr	r2, [r4, #440]
	bfi	r3, r1, #4, #4
	bfi	r3, r2, #0, #4
	str	r3, [r8, #64]
	ldr	r3, [r4, #20]
	cmp	r3, #4
	beq	.L105
	cmp	r3, #0
	moveq	r3, #15
	moveq	ip, #0
	bfieq	ip, r3, #16, #6
	moveq	r3, #31
	bfieq	ip, r3, #0, #7
.L83:
	mov	r2, #3
	mov	r3, #0
	bfi	r3, r2, #0, #3
	mov	r1, #1
	bfi	r3, r1, #3, #2
	mov	r1, #1
	str	r1, [r10, #524]
	bfi	r3, r2, #5, #3
	ldr	r1, [fp, #-48]
	bfi	r3, r2, #8, #3
	str	ip, [r10, #16]
	mov	lr, #0
	str	r2, [r10, #20]
	mov	r0, lr
	str	r3, [r1, #100]
	mov	ip, lr
	ldr	r7, [r4, #408]
	mov	r1, lr
	mov	r6, lr
	mov	r3, lr
	bfc	r3, #0, #1
	str	r7, [r10, #128]
	bfc	r3, #1, #1
	ldr	r7, [r4, #64]
	bfc	r3, #2, #1
	bfc	r3, #3, #3
	str	r7, [r10, #132]
	ldr	r7, [r4, #68]
	str	r7, [r10, #136]
	ldr	r7, [r4, #72]
	str	r7, [r10, #140]
	ldr	r8, [r4, #48]
	ldr	r7, [r4, #52]
	bfi	lr, r8, #16, #16
	ldr	r8, [fp, #-52]
	bfi	lr, r7, #0, #16
	str	lr, [r10, #152]
	ldr	lr, [r8, #52]
	add	lr, r4, lr, lsl #2
	ldr	lr, [lr, #124]
	str	lr, [r10, #156]
	ldr	lr, [r8, #48]
	add	lr, r4, lr, lsl #2
	ldr	lr, [lr, #76]
	str	lr, [r10, #160]
	ldr	lr, [r8, #48]
	add	lr, r4, lr, lsl #2
	ldr	lr, [lr, #84]
	str	lr, [r10, #164]
	ldr	r7, [r4, #272]
	ldr	lr, [r4, #276]
	bfi	ip, r7, #0, #16
	bfi	ip, lr, #16, #16
	str	ip, [r10, #168]
	ldr	ip, [r8, #48]
	add	ip, r4, ip, lsl #2
	ldr	ip, [ip, #92]
	str	ip, [r10, #172]
	ldr	ip, [r8, #48]
	add	ip, r4, ip, lsl #2
	ldr	ip, [ip, #100]
	str	ip, [r10, #176]
	ldr	ip, [r4, #280]
	bfi	r6, ip, #0, #16
	str	r6, [r10, #180]
	ldr	ip, [r8, #48]
	cmp	ip, r0
	moveq	ip, #76
	movne	ip, #72
	add	ip, r4, ip
	ldr	ip, [ip, #4]
	str	ip, [r10, #184]
	ldr	ip, [r8, #48]
	cmp	ip, r0
	moveq	ip, #84
	movne	ip, #80
	add	ip, r4, ip
	ldr	ip, [ip, #4]
	str	ip, [r10, #188]
	ldr	lr, [r4, #276]
	ldr	ip, [r4, #272]
	bfi	r0, lr, #0, #16
	bfi	r0, ip, #16, #16
	str	r0, [r10, #192]
	ldr	r0, [r8, #48]
	cmp	r0, r1
	moveq	r0, #92
	movne	r0, #88
	add	r0, r4, r0
	ldr	r0, [r0, #4]
	str	r0, [r10, #196]
	ldr	r0, [r8, #48]
	cmp	r0, r1
	moveq	r0, #100
	movne	r0, #96
	add	r0, r4, r0
	ldr	r0, [r0, #4]
	str	r0, [r10, #200]
	ldr	ip, [r4, #280]
	ldr	r0, [r4, #284]
	bfi	r1, ip, #16, #16
	bfi	r1, r0, #0, #16
	str	r1, [r10, #204]
	ldr	r1, [r8, #48]
	cmp	r1, #0
	moveq	r1, #116
	movne	r1, #112
	add	r1, r4, r1
	ldr	r1, [r1, #4]
	str	r1, [r10, #208]
	ldr	r1, [r8, #48]
	add	r1, r4, r1, lsl #2
	ldr	r1, [r1, #116]
	str	r1, [r10, #212]
	ldr	r1, [r8, #48]
	add	r1, r4, r1, lsl #2
	ldr	r1, [r1, #108]
	str	r1, [r10, #48]
	ldr	r1, [r8, #48]
	cmp	r1, #0
	moveq	r1, #108
	movne	r1, #104
	add	r1, r4, r1
	ldr	r1, [r1, #4]
	str	r1, [r10, #52]
	ldr	r1, [r4, #32]
	str	r1, [r10, #56]
	ldr	r1, [r4, #40]
	str	r1, [r10, #60]
	ldr	r1, [r4, #36]
	str	r1, [r10, #64]
	ldr	r1, [r4, #56]
	cmp	r1, #2
	beq	.L84
	cmp	r1, #1
	beq	.L99
	cmp	r1, #0
	bfc	r3, #6, #4
	bfc	r3, #14, #1
	bne	.L85
	ldr	r2, [r4, #60]
	clz	r2, r2
	mov	r2, r2, lsr #5
.L86:
	bfi	r3, r2, #16, #8
	str	r3, [r10, #4]
	ldr	r2, [r4, #2324]
	mov	r1, #5
	ldr	r0, [r4, #2320]
	mov	r3, #0
	bfi	r3, r2, #0, #3
	mov	r2, #0
	bfi	r2, r1, #24, #8
	bfi	r3, r0, #4, #1
	bfi	r2, r1, #16, #8
	mov	r0, #15
	bfc	r3, #5, #1
	bfi	r2, r0, #8, #8
	mov	r1, #3
	mov	r0, #0
	bfc	r1, #2, #1
	orr	r3, r3, #30464
	mov	ip, #15
	str	r3, [r10, #528]
	bfi	r2, ip, #0, #8
	orr	r1, r1, #24
	str	r2, [r10, #544]
	mov	r3, #3
	str	r1, [r10, #532]
	mov	r2, #500
	str	r3, [r10, #72]
	movt	r0, 100
	bfi	r0, r2, #0, #16
	str	r0, [r10, #536]
	ldr	r2, [r4, #712]
	mov	ip, #0
	ldr	r1, [r4, #708]
	mov	r3, ip
	mov	r0, ip
	bfi	ip, r2, #0, #1
	ldr	r2, [r4, #704]
	bfi	ip, r1, #1, #1
	ldr	r5, [fp, #-48]
	mov	r1, r3
	bfi	ip, r2, #2, #1
	mov	r6, r3
	bfc	ip, #3, #1
	mov	lr, r3
	bfc	ip, #4, #1
	str	ip, [r5, #352]
	ldr	ip, [r4, #8]
	mvn	r2, #127
	bfc	r0, #1, #1
	bfi	r1, r2, #0, #8
	mov	r8, #2
	mov	r7, #128
	str	ip, [r5, #392]
	bfi	r1, r2, #8, #8
	ldr	ip, [r4, #2180]
	bfi	r0, r8, #16, #8
	ldr	r9, [r4, #2184]
	bfi	r6, r2, #0, #8
	bfi	r3, ip, #0, #1
	ldr	ip, [r4, #2196]
	bfi	r3, r9, #1, #1
	ldr	r9, [r4, #2204]
	bfi	r3, ip, #2, #1
	ldr	ip, [r4, #2176]
	bfi	r3, r9, #4, #1
	ldr	r9, [r4, #2172]
	bfi	r3, ip, #8, #1
	ldr	ip, [r4, #2252]
	bfi	r3, r9, #9, #1
	ldr	r9, [r4, #2256]
	bfi	r3, ip, #10, #1
	ldr	ip, [r4, #2260]
	bfi	r3, r9, #11, #1
	ldr	r9, [r4, #2168]
	bfi	r3, ip, #12, #1
	ldr	ip, [r4, #2164]
	bfi	r3, r9, #13, #1
	ldr	r9, [r4, #2264]
	bfi	r3, ip, #14, #1
	bfi	lr, r7, #0, #10
	mov	ip, #0
	bfc	ip, #0, #1
	bfi	r3, r9, #15, #1
	bfi	r0, r8, #24, #8
	str	r3, [r5, #440]
	bfi	r1, r2, #16, #8
	bfi	r6, r2, #8, #8
	bfi	lr, r7, #10, #10
	str	r0, [r10, #540]
	bfc	ip, #1, #8
	str	r1, [r10, #1760]
	movw	r0, #43691
	str	r6, [r10, #1764]
	movt	r0, 43690
	str	lr, [r10, #1768]
	mov	r8, r5
	str	ip, [r10, #32]
	mov	r5, #1
	ldr	r9, [fp, #-52]
	mov	r3, #0
	mov	ip, r3
	mov	r2, r3
	str	r3, [fp, #-68]
	ldr	r1, [r9, #52]
	str	r3, [fp, #-72]
	add	r1, r1, #2
	str	r3, [fp, #-76]
	umull	r6, r7, r1, r0
	mov	r6, r7, lsr r5
	add	r6, r6, r6, lsl #1
	rsb	r1, r6, r1
	add	r1, r4, r1, lsl #2
	ldr	r1, [r1, #124]
	str	r1, [r10, #216]
	ldr	r6, [r9, #52]
	add	r6, r6, r5
	umull	r0, r1, r6, r0
	mov	r1, r1, lsr r5
	add	r1, r1, r1, lsl #1
	rsb	r1, r1, r6
	add	r1, r4, r1, lsl #2
	ldr	r1, [r1, #124]
	str	r1, [r10, #220]
	str	r5, [r8, #492]
	ldrb	lr, [r4, #472]
	ldrb	r0, [r4, #468]
	ldrb	r5, [r4, #464]
	ldrb	r7, [r4, #460]
	orr	r1, r0, lr
	ldrb	r9, [r4, #456]
	orr	r1, r5, r1
	ldrb	r8, [r4, #452]
	orr	r1, r7, r1
	ldrb	r6, [r4, #448]
	orr	r1, r9, r1
	orr	r1, r8, r1
	str	r8, [fp, #-56]
	orr	r1, r6, r1
	ldrb	r8, [r4, #444]
	str	r6, [fp, #-60]
	orr	r1, r8, r1
	bfi	r3, r1, #24, #1
	ldr	r1, [r4, #696]
	str	r8, [fp, #-64]
	mov	r8, ip
	bfi	r3, r1, #23, #1
	ldr	r1, [r4, #692]
	bfi	r3, r1, #22, #1
	ldr	r1, [r4, #688]
	bfi	r3, r1, #21, #1
	ldr	r1, [r4, #684]
	bfi	r3, r1, #20, #1
	ldr	r1, [r4, #680]
	bfi	r3, r1, #19, #1
	ldr	r1, [r4, #676]
	bfi	r3, r1, #18, #1
	ldr	r1, [r4, #672]
	bfi	r3, r1, #17, #1
	ldr	r1, [r4, #668]
	bfi	r3, r1, #16, #1
	ldr	r1, [r4, #504]
	bfi	r3, r1, #15, #1
	ldr	r1, [r4, #500]
	bfi	r3, r1, #14, #1
	ldr	r1, [r4, #496]
	bfi	r3, r1, #13, #1
	ldr	r1, [r4, #492]
	ldr	r6, [fp, #-56]
	bfi	r3, r1, #12, #1
	ldr	r1, [r4, #488]
	str	ip, [fp, #-56]
	bfi	r3, r1, #11, #1
	ldr	r1, [r4, #484]
	bfi	r3, r1, #10, #1
	ldr	r1, [r4, #480]
	bfi	r3, r1, #9, #1
	ldr	r1, [r4, #476]
	bfi	r3, r1, #8, #1
	bfi	r3, lr, #7, #1
	ldr	lr, [fp, #-64]
	bfi	r3, r0, #6, #1
	ldr	r0, [fp, #-60]
	bfi	r3, r5, #5, #1
	str	ip, [fp, #-60]
	bfi	r3, r7, #4, #1
	mov	r7, ip
	bfi	r3, r9, #3, #1
	mov	r9, r8
	bfi	r3, r6, #2, #1
	mov	r6, ip
	bfi	r3, r0, #1, #1
	bfi	r3, lr, #0, #1
	str	r3, [r10, #1024]
	ldr	r3, [r4, #520]
	mov	lr, ip
	ldr	r1, [r4, #516]
	mov	r5, lr
	ldr	r0, [r4, #512]
	bfi	ip, r3, #24, #7
	bfi	ip, r1, #16, #7
	ldr	r3, [r4, #508]
	bfi	ip, r0, #8, #7
	bfi	ip, r3, #0, #7
	str	ip, [r10, #1028]
	ldr	r3, [r4, #536]
	mov	ip, r8
	ldr	r1, [r4, #532]
	ldr	r0, [r4, #528]
	bfi	r2, r3, #24, #7
	bfi	r2, r1, #16, #7
	ldr	r3, [r4, #524]
	bfi	r2, r0, #8, #7
	mov	r0, r8
	bfi	r2, r3, #0, #7
	str	r2, [r10, #1032]
	ldr	r2, [r4, #572]
	mov	r1, r8
	ldr	r3, [r4, #540]
	bfi	ip, r2, #16, #10
	bfi	ip, r3, #0, #10
	str	ip, [r10, #1036]
	ldr	r2, [r4, #576]
	mov	ip, r8
	ldr	r3, [r4, #544]
	bfi	r0, r2, #16, #10
	mov	r2, r0
	bfi	r2, r3, #0, #10
	str	r2, [r10, #1040]
	mov	r0, r8
	ldr	r2, [r4, #580]
	ldr	r3, [r4, #548]
	bfi	r1, r2, #16, #10
	mov	r2, r1
	bfi	r2, r3, #0, #10
	str	r2, [r10, #1044]
	mov	r1, r8
	ldr	r2, [r4, #584]
	ldr	r3, [r4, #552]
	bfi	r8, r2, #16, #10
	bfi	r8, r3, #0, #10
	str	r8, [r10, #1048]
	ldr	r2, [r4, #588]
	mov	r8, r7
	ldr	r3, [r4, #556]
	bfi	r7, r2, #16, #10
	mov	r2, r6
	bfi	r7, r3, #0, #10
	str	r7, [r10, #1052]
	ldr	r7, [r4, #592]
	ldr	r3, [r4, #560]
	bfi	r6, r7, #16, #10
	bfi	r6, r3, #0, #10
	str	r6, [r10, #1056]
	ldr	r7, [r4, #596]
	mov	r3, lr
	ldr	r6, [r4, #564]
	bfi	r3, r7, #16, #10
	bfi	r3, r6, #0, #10
	str	r3, [r10, #1060]
	ldr	r7, [r4, #600]
	mov	r3, lr
	ldr	r6, [r4, #568]
	bfi	r3, r7, #16, #10
	bfi	r3, r6, #0, #10
	str	r3, [r10, #1064]
	ldr	r7, [r4, #636]
	ldr	r6, [r4, #604]
	bfi	lr, r7, #16, #9
	bfi	lr, r6, #0, #9
	str	lr, [r10, #1068]
	ldr	r6, [r4, #640]
	ldr	lr, [r4, #608]
	bfi	r9, r6, #16, #9
	bfi	r9, lr, #0, #9
	str	r9, [r10, #1072]
	ldr	r6, [r4, #644]
	ldr	lr, [r4, #612]
	bfi	ip, r6, #16, #9
	bfi	ip, lr, #0, #9
	str	ip, [r10, #1076]
	ldr	lr, [r4, #648]
	ldr	ip, [r4, #616]
	bfi	r0, lr, #16, #9
	bfi	r0, ip, #0, #9
	str	r0, [r10, #1080]
	ldr	ip, [r4, #652]
	ldr	r0, [r4, #620]
	bfi	r1, ip, #16, #9
	bfi	r1, r0, #0, #9
	str	r1, [r10, #1084]
	ldr	r0, [r4, #656]
	ldr	r1, [r4, #624]
	bfi	r8, r0, #16, #9
	bfi	r8, r1, #0, #9
	str	r8, [r10, #1088]
	ldr	r0, [r4, #660]
	ldr	r1, [r4, #628]
	bfi	r2, r0, #16, #9
	bfi	r2, r1, #0, #9
	str	r2, [r10, #1092]
	ldr	r1, [r4, #664]
	ldr	r2, [r4, #632]
	ldr	r9, [fp, #-52]
	bfi	r5, r1, #16, #9
	bfi	r5, r2, #0, #9
	str	r5, [r10, #1096]
	ldr	r3, [r4, #20]
	ldr	r0, [r9, #12]
	cmp	r3, #0
	sub	r0, r0, #1
	moveq	r6, #16
	movne	r6, #64
	mov	r1, r6
	add	r0, r0, r6
	bl	__aeabi_uidiv
	ldr	r3, [r9, #16]
	mov	r1, r6
	mov	r7, r0
	sub	r0, r3, #1
	add	r0, r0, r6
	bl	__aeabi_uidiv
	str	r9, [fp, #-52]
	mul	r1, r7, r0
	ldr	r0, [r9, #168]
	bl	__aeabi_uidiv
	mov	r2, #0
	mov	r3, r2
	str	r2, [fp, #-60]
	str	r2, [fp, #-64]
	mov	lr, r2
	str	r2, [fp, #-68]
	mov	ip, r2
	str	r2, [fp, #-72]
	mov	r1, r2
	str	r2, [fp, #-76]
	mov	r7, r2
	str	r2, [fp, #-80]
	mov	r6, r2
	str	r2, [fp, #-84]
	mov	r5, r2
	str	r2, [fp, #-88]
	mov	r9, r2
	str	r2, [fp, #-92]
	mov	r8, r2
	str	r2, [fp, #-96]
	str	r2, [fp, #-100]
	str	r2, [fp, #-104]
	str	r2, [fp, #-108]
	str	r2, [fp, #-112]
	str	r2, [fp, #-116]
	str	r2, [fp, #-120]
	str	r2, [fp, #-124]
	bfi	r3, r0, #0, #20
	ldr	r0, [fp, #-48]
	str	r3, [r0, #588]
	mov	r3, r2
	ldr	r0, [r4, #716]
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #720]
	bfi	r3, r0, #0, #10
	str	r3, [r10, #1536]
	ldr	r0, [r4, #724]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #728]
	bfi	r3, r0, #0, #8
	str	r3, [r10, #1540]
	ldr	r0, [r4, #732]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #736]
	bfi	r3, r0, #0, #10
	str	r3, [r10, #1544]
	ldr	r0, [r4, #740]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #744]
	bfi	r3, r0, #0, #8
	str	r3, [r10, #1548]
	ldr	r0, [r4, #748]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #752]
	bfi	r3, r0, #0, #10
	str	r3, [r10, #1552]
	ldr	r0, [r4, #756]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #760]
	bfi	r3, r0, #0, #8
	str	r3, [r10, #1556]
	ldr	r0, [r4, #764]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #768]
	bfi	r3, r0, #0, #10
	str	r3, [r10, #1560]
	ldr	r0, [r4, #772]
	mov	r3, r2
	bfi	r3, r0, #16, #5
	ldr	r0, [r4, #776]
	bfi	r3, r0, #0, #8
	str	r3, [r10, #1564]
	ldr	r0, [r4, #780]
	bfi	lr, r0, #12, #4
	ldr	r0, [r4, #784]
	bfi	lr, r0, #8, #4
	ldr	r0, [r4, #788]
	bfi	lr, r0, #4, #4
	ldr	r0, [r4, #792]
	bfi	lr, r0, #0, #4
	ldr	r0, [r4, #796]
	bfi	lr, r0, #28, #4
	ldr	r0, [r4, #800]
	bfi	lr, r0, #24, #4
	ldr	r0, [r4, #804]
	bfi	lr, r0, #20, #4
	ldr	r0, [r4, #808]
	bfi	lr, r0, #16, #4
	str	lr, [r10, #1568]
	ldr	r0, [r4, #812]
	mov	lr, r7
	bfi	ip, r0, #12, #4
	ldr	r0, [r4, #816]
	bfi	ip, r0, #8, #4
	ldr	r0, [r4, #820]
	bfi	ip, r0, #4, #4
	ldr	r0, [r4, #824]
	bfi	ip, r0, #0, #4
	ldr	r0, [r4, #828]
	bfi	ip, r0, #28, #4
	ldr	r0, [r4, #832]
	bfi	ip, r0, #24, #4
	ldr	r0, [r4, #836]
	bfi	ip, r0, #20, #4
	ldr	r0, [r4, #840]
	bfi	ip, r0, #16, #4
	str	ip, [r10, #1572]
	ldr	r0, [r4, #844]
	mov	ip, r6
	bfi	r1, r0, #12, #4
	ldr	r0, [r4, #848]
	bfi	r1, r0, #8, #4
	ldr	r0, [r4, #852]
	bfi	r1, r0, #4, #4
	ldr	r0, [r4, #856]
	bfi	r1, r0, #0, #4
	ldr	r0, [r4, #860]
	bfi	r1, r0, #28, #4
	ldr	r0, [r4, #864]
	bfi	r1, r0, #24, #4
	ldr	r0, [r4, #868]
	bfi	r1, r0, #20, #4
	ldr	r0, [r4, #872]
	bfi	r1, r0, #16, #4
	str	r1, [r10, #1576]
	ldr	r1, [r4, #876]
	mov	r0, r7
	bfi	r2, r1, #12, #4
	ldr	r1, [r4, #880]
	bfi	r2, r1, #8, #4
	ldr	r1, [r4, #884]
	bfi	r2, r1, #4, #4
	ldr	r1, [r4, #888]
	bfi	r2, r1, #0, #4
	ldr	r1, [r4, #892]
	bfi	r2, r1, #28, #4
	ldr	r1, [r4, #896]
	bfi	r2, r1, #24, #4
	ldr	r1, [r4, #900]
	bfi	r2, r1, #20, #4
	ldr	r1, [r4, #904]
	bfi	r2, r1, #16, #4
	str	r2, [r10, #1580]
	ldr	r2, [r4, #908]
	mov	r1, r7
	bfi	r5, r2, #12, #4
	ldr	r2, [r4, #912]
	bfi	r5, r2, #8, #4
	ldr	r2, [r4, #916]
	bfi	r5, r2, #4, #4
	ldr	r2, [r4, #920]
	bfi	r5, r2, #0, #4
	ldr	r2, [r4, #924]
	bfi	r5, r2, #28, #4
	ldr	r2, [r4, #928]
	bfi	r5, r2, #24, #4
	ldr	r2, [r4, #932]
	bfi	r5, r2, #20, #4
	ldr	r2, [r4, #936]
	bfi	r5, r2, #16, #4
	str	r5, [r10, #1584]
	ldr	r3, [r4, #940]
	mov	r2, r6
	mov	r5, r7
	bfi	r2, r3, #12, #4
	ldr	r3, [r4, #944]
	bfi	r2, r3, #8, #4
	ldr	r3, [r4, #948]
	bfi	r2, r3, #4, #4
	ldr	r3, [r4, #952]
	bfi	r2, r3, #0, #4
	ldr	r3, [r4, #956]
	bfi	r2, r3, #28, #4
	ldr	r3, [r4, #960]
	bfi	r2, r3, #24, #4
	ldr	r3, [r4, #964]
	bfi	r2, r3, #20, #4
	ldr	r3, [r4, #968]
	bfi	r2, r3, #16, #4
	str	r2, [r10, #1588]
	ldr	r3, [r4, #972]
	mov	r2, r7
	bfi	r9, r3, #12, #4
	ldr	r3, [r4, #976]
	bfi	r9, r3, #8, #4
	ldr	r3, [r4, #980]
	bfi	r9, r3, #4, #4
	ldr	r3, [r4, #984]
	bfi	r9, r3, #0, #4
	ldr	r3, [r4, #988]
	bfi	r9, r3, #28, #4
	ldr	r3, [r4, #992]
	bfi	r9, r3, #24, #4
	ldr	r3, [r4, #996]
	bfi	r9, r3, #20, #4
	ldr	r3, [r4, #1000]
	bfi	r9, r3, #16, #4
	str	r9, [r10, #1592]
	ldr	r3, [r4, #1004]
	mov	r9, r7
	bfi	r8, r3, #12, #4
	ldr	r3, [r4, #1008]
	bfi	r8, r3, #8, #4
	ldr	r3, [r4, #1012]
	bfi	r8, r3, #4, #4
	ldr	r3, [r4, #1016]
	bfi	r8, r3, #0, #4
	ldr	r3, [r4, #1020]
	bfi	r8, r3, #28, #4
	ldr	r3, [r4, #1024]
	bfi	r8, r3, #24, #4
	ldr	r3, [r4, #1028]
	bfi	r8, r3, #20, #4
	ldr	r3, [r4, #1032]
	bfi	r8, r3, #16, #4
	str	r8, [r10, #1596]
	ldr	r3, [r4, #1036]
	mov	r8, r7
	bfi	r6, r3, #12, #4
	ldr	r3, [r4, #1040]
	bfi	r6, r3, #8, #4
	ldr	r3, [r4, #1044]
	bfi	r6, r3, #4, #4
	ldr	r3, [r4, #1048]
	bfi	r6, r3, #0, #4
	ldr	r3, [r4, #1052]
	bfi	r6, r3, #28, #4
	ldr	r3, [r4, #1056]
	bfi	r6, r3, #24, #4
	ldr	r3, [r4, #1060]
	bfi	r6, r3, #20, #4
	ldr	r3, [r4, #1064]
	bfi	r6, r3, #16, #4
	str	r6, [r10, #1600]
	ldr	r3, [r4, #1068]
	mov	r6, r7
	bfi	r2, r3, #0, #3
	ldr	r3, [r4, #1072]
	bfi	r2, r3, #3, #7
	ldr	r3, [r4, #1076]
	bfi	r2, r3, #10, #3
	ldr	r3, [r4, #1080]
	bfi	r2, r3, #13, #5
	ldr	r3, [r4, #1084]
	bfi	r2, r3, #18, #4
	ldr	r3, [r4, #1088]
	bfi	r2, r3, #22, #4
	ldr	r3, [r4, #1092]
	bfi	r2, r3, #26, #4
	str	r2, [r10, #1604]
	ldr	r3, [r4, #1096]
	mov	r2, r7
	bfi	r2, r3, #0, #3
	ldr	r3, [r4, #1100]
	bfi	r2, r3, #3, #7
	ldr	r3, [r4, #1104]
	bfi	r2, r3, #10, #3
	ldr	r3, [r4, #1108]
	bfi	r2, r3, #13, #5
	ldr	r3, [r4, #1112]
	bfi	r2, r3, #18, #4
	ldr	r3, [r4, #1116]
	bfi	r2, r3, #22, #4
	ldr	r3, [r4, #1120]
	bfi	r2, r3, #26, #4
	str	r2, [r10, #1608]
	ldr	r3, [r4, #1124]
	mov	r2, r7
	bfi	r2, r3, #0, #3
	ldr	r3, [r4, #1128]
	bfi	r2, r3, #3, #7
	ldr	r3, [r4, #1132]
	bfi	r2, r3, #10, #3
	ldr	r3, [r4, #1136]
	bfi	r2, r3, #13, #5
	ldr	r3, [r4, #1140]
	bfi	r2, r3, #18, #4
	ldr	r3, [r4, #1144]
	bfi	r2, r3, #22, #4
	ldr	r3, [r4, #1148]
	bfi	r2, r3, #26, #4
	str	r2, [r10, #1612]
	ldr	r3, [r4, #1424]
	mov	r2, r7
	bfi	r2, r3, #0, #3
	ldr	r3, [r4, #1428]
	bfi	r2, r3, #3, #7
	ldr	r3, [r4, #1432]
	bfi	r2, r3, #10, #3
	ldr	r3, [r4, #1436]
	bfi	r2, r3, #13, #5
	ldr	r3, [r4, #1440]
	bfi	r2, r3, #18, #4
	ldr	r3, [r4, #1444]
	bfi	r2, r3, #22, #4
	ldr	r3, [r4, #1448]
	bfi	r2, r3, #26, #4
	str	r2, [r10, #1616]
	ldr	r3, [r4, #1452]
	mov	r2, r7
	bfi	r6, r3, #0, #3
	ldr	r3, [r4, #1456]
	bfi	r6, r3, #3, #7
	ldr	r3, [r4, #1460]
	bfi	r6, r3, #10, #3
	ldr	r3, [r4, #1464]
	bfi	r6, r3, #13, #5
	ldr	r3, [r4, #1468]
	bfi	r6, r3, #18, #4
	ldr	r3, [r4, #1472]
	bfi	r6, r3, #22, #4
	ldr	r3, [r4, #1476]
	bfi	r6, r3, #26, #4
	str	r6, [r10, #1620]
	ldr	r6, [r4, #1480]
	mov	r3, r7
	bfi	r9, r6, #0, #3
	ldr	r6, [r4, #1484]
	bfi	r9, r6, #3, #7
	ldr	r6, [r4, #1488]
	bfi	r9, r6, #10, #3
	ldr	r6, [r4, #1492]
	bfi	r9, r6, #13, #5
	ldr	r6, [r4, #1496]
	bfi	r9, r6, #18, #4
	ldr	r6, [r4, #1500]
	bfi	r9, r6, #22, #4
	ldr	r6, [r4, #1504]
	bfi	r9, r6, #26, #4
	str	r9, [r10, #1624]
	ldr	r6, [r4, #1508]
	mov	r9, r7
	bfi	r8, r6, #0, #3
	ldr	r6, [r4, #1512]
	bfi	r8, r6, #3, #7
	ldr	r6, [r4, #1516]
	bfi	r8, r6, #10, #3
	ldr	r6, [r4, #1520]
	bfi	r8, r6, #13, #5
	ldr	r6, [r4, #1524]
	bfi	r8, r6, #18, #4
	ldr	r6, [r4, #1528]
	bfi	r8, r6, #22, #4
	ldr	r6, [r4, #1532]
	bfi	r8, r6, #26, #4
	str	r8, [r10, #1628]
	ldr	r6, [r4, #1152]
	mov	r8, r7
	bfi	r7, r6, #0, #4
	ldr	r6, [r4, #1156]
	bfi	r7, r6, #4, #4
	ldr	r6, [r4, #1160]
	bfi	r7, r6, #8, #4
	ldr	r6, [r4, #1164]
	bfi	r7, r6, #12, #4
	ldr	r6, [r4, #1168]
	bfi	r7, r6, #16, #4
	ldr	r6, [r4, #1172]
	bfi	r7, r6, #20, #4
	ldr	r6, [r4, #1176]
	bfi	r7, r6, #24, #4
	ldr	r6, [r4, #1180]
	bfi	r7, r6, #28, #4
	str	r7, [r10, #1632]
	ldr	r6, [r4, #1184]
	mov	r7, ip
	bfi	r7, r6, #0, #4
	ldr	r6, [r4, #1188]
	bfi	r7, r6, #4, #4
	ldr	r6, [r4, #1192]
	bfi	r7, r6, #8, #4
	ldr	r6, [r4, #1196]
	bfi	r7, r6, #12, #4
	ldr	r6, [r4, #1200]
	bfi	r7, r6, #16, #4
	ldr	r6, [r4, #1204]
	bfi	r7, r6, #20, #4
	str	r7, [r10, #1636]
	ldr	r6, [r4, #1208]
	mov	r7, ip
	bfi	ip, r6, #0, #10
	ldr	r6, [r4, #1212]
	bfi	ip, r6, #16, #5
	str	ip, [r10, #1640]
	ldr	ip, [r4, #1216]
	mov	r6, lr
	bfi	lr, ip, #0, #8
	ldr	ip, [r4, #1220]
	bfi	lr, ip, #16, #5
	str	lr, [r10, #1644]
	ldr	ip, [r4, #1224]
	mov	lr, r0
	bfi	r0, ip, #0, #10
	ldr	ip, [r4, #1228]
	bfi	r0, ip, #16, #5
	str	r0, [r10, #1648]
	ldr	r0, [r4, #1232]
	mov	ip, r1
	bfi	r7, r0, #0, #8
	ldr	r0, [r4, #1236]
	bfi	r7, r0, #16, #5
	str	r7, [r10, #1652]
	ldr	r7, [r4, #1240]
	mov	r0, r1
	bfi	r1, r7, #0, #10
	ldr	r7, [r4, #1244]
	bfi	r1, r7, #16, #5
	str	r1, [r10, #1656]
	ldr	r7, [r4, #1248]
	mov	r1, r5
	bfi	r2, r7, #0, #8
	ldr	r7, [r4, #1252]
	bfi	r2, r7, #16, #5
	str	r2, [r10, #1660]
	ldr	r7, [r4, #1536]
	bfi	r3, r7, #0, #4
	ldr	r7, [r4, #1540]
	bfi	r3, r7, #4, #4
	ldr	r7, [r4, #1544]
	bfi	r3, r7, #8, #4
	ldr	r7, [r4, #1548]
	bfi	r3, r7, #12, #4
	ldr	r7, [r4, #1552]
	bfi	r3, r7, #16, #4
	ldr	r7, [r4, #1556]
	bfi	r3, r7, #20, #4
	str	r3, [r10, #1664]
	ldr	r7, [r4, #1560]
	mov	r3, r5
	bfi	r9, r7, #0, #12
	ldr	r7, [r4, #1564]
	bfi	r9, r7, #12, #12
	ldr	r7, [r4, #1568]
	bfi	r9, r7, #24, #8
	str	r9, [r10, #1668]
	ldr	r2, [r4, #1572]
	mov	r7, r5
	mov	r9, r5
	bfc	r9, #14, #1
	bfi	r7, r2, #0, #12
	ldr	r2, [r4, #1576]
	bfc	r9, #15, #1
	bfi	r7, r2, #12, #12
	ldr	r2, [r4, #1580]
	bfi	r7, r2, #24, #8
	str	r7, [r10, #1672]
	ldr	r2, [r4, #1584]
	mov	r7, r5
	bfc	r7, #0, #8
	bfi	r8, r2, #0, #12
	ldr	r2, [r4, #1588]
	orr	r7, r7, #65280
	bfi	r8, r2, #12, #12
	ldr	r2, [r4, #1592]
	movt	r7, 65280
	str	r7, [fp, #-56]
	bfi	r8, r2, #24, #8
	str	r8, [r10, #1676]
	ldr	r8, [r4, #1596]
	mov	r2, r5
	bfi	r2, r8, #0, #8
	ldr	r8, [r4, #1600]
	bfi	r2, r8, #8, #8
	ldr	r8, [r4, #1604]
	bfi	r2, r8, #16, #8
	str	r2, [r10, #1680]
	ldr	r7, [r4, #1256]
	mov	r8, r5
	bfi	r6, r7, #0, #1
	ldr	r7, [r4, #1260]
	bfi	r6, r7, #1, #3
	ldr	r7, [r4, #1264]
	bfi	r6, r7, #4, #6
	ldr	r7, [r4, #1268]
	bfi	r6, r7, #10, #5
	ldr	r7, [r4, #1272]
	bfi	r6, r7, #15, #6
	ldr	r7, [r4, #1276]
	bfi	r6, r7, #21, #5
	str	r6, [r10, #1684]
	ldr	r6, [r4, #1280]
	bfi	lr, r6, #0, #1
	ldr	r6, [r4, #1284]
	bfi	lr, r6, #1, #3
	ldr	r6, [r4, #1288]
	bfi	lr, r6, #4, #6
	ldr	r6, [r4, #1292]
	bfi	lr, r6, #10, #5
	ldr	r6, [r4, #1296]
	bfi	lr, r6, #15, #6
	ldr	r6, [r4, #1300]
	bfi	lr, r6, #21, #5
	str	lr, [r10, #1688]
	ldr	lr, [r4, #1304]
	mov	r6, r5
	bfi	ip, lr, #0, #1
	ldr	lr, [r4, #1308]
	bfi	ip, lr, #1, #3
	ldr	lr, [r4, #1312]
	bfi	ip, lr, #4, #6
	ldr	lr, [r4, #1316]
	bfi	ip, lr, #10, #5
	ldr	lr, [r4, #1320]
	bfi	ip, lr, #15, #6
	ldr	lr, [r4, #1324]
	bfi	ip, lr, #21, #5
	str	ip, [r10, #1692]
	ldr	ip, [r4, #1328]
	mov	lr, r5
	bfi	r0, ip, #0, #1
	ldr	ip, [r4, #1332]
	bfi	r0, ip, #1, #3
	ldr	ip, [r4, #1336]
	bfi	r0, ip, #4, #6
	ldr	ip, [r4, #1340]
	bfi	r0, ip, #10, #5
	ldr	ip, [r4, #1344]
	bfi	r0, ip, #15, #6
	ldr	ip, [r4, #1348]
	bfi	r0, ip, #21, #5
	str	r0, [r10, #1696]
	ldr	r0, [r4, #1352]
	mov	ip, r8
	bfi	r1, r0, #0, #1
	ldr	r0, [r4, #1356]
	bfi	r1, r0, #1, #3
	ldr	r0, [r4, #1360]
	bfi	r1, r0, #4, #6
	ldr	r0, [r4, #1364]
	bfi	r1, r0, #10, #5
	ldr	r0, [r4, #1368]
	bfi	r1, r0, #15, #6
	ldr	r0, [r4, #1372]
	bfi	r1, r0, #21, #5
	str	r1, [r10, #1700]
	ldr	r0, [r4, #1376]
	mov	r1, r5
	bfi	r5, r0, #0, #1
	ldr	r0, [r4, #1380]
	bfi	r5, r0, #1, #3
	ldr	r0, [r4, #1384]
	bfi	r5, r0, #4, #6
	ldr	r0, [r4, #1388]
	bfi	r5, r0, #10, #5
	ldr	r0, [r4, #1392]
	bfi	r5, r0, #15, #6
	ldr	r0, [r4, #1396]
	bfi	r5, r0, #21, #5
	str	r5, [r10, #1704]
	ldr	r2, [r4, #1400]
	mov	r5, #20
	mov	r0, r8
	bfi	r3, r2, #0, #1
	ldr	r2, [r4, #1404]
	bfi	r3, r2, #1, #3
	ldr	r2, [r4, #1408]
	bfi	r3, r2, #4, #6
	ldr	r2, [r4, #1412]
	bfi	r3, r2, #10, #5
	ldr	r2, [r4, #1416]
	bfi	r3, r2, #15, #6
	ldr	r2, [r4, #1420]
	bfi	r3, r2, #21, #5
	str	r3, [r10, #1708]
	ldr	r3, [r4, #404]
	mov	r2, r8
	bfi	r9, r3, #20, #8
	ldr	r3, [fp, #-52]
	ldr	r7, [fp, #-56]
	str	r8, [fp, #-56]
	ldr	r3, [r3, #68]
	str	r7, [r10, #12]
	mov	r7, r8
	bfi	r9, r3, #28, #1
	str	r9, [r10, #8]
	ldr	r3, [r4, #2376]
	mov	r9, r8
	bfi	r7, r3, #0, #1
	str	r7, [r10, #1824]
	ldr	r3, [r4, #2380]
	mov	r7, r8
	bfi	r2, r3, #0, #16
	str	r2, [r10, #1828]
	ldr	r3, [r4, #2384]
	mov	r2, r8
	bfi	r2, r3, #0, #16
	str	r2, [r10, #1832]
	ldr	r3, [r4, #2388]
	mov	r2, r8
	bfi	r2, r3, #0, #16
	str	r2, [r10, #1836]
	ldr	r3, [r4, #2392]
	ldr	r2, [r4, #2396]
	bfi	r6, r3, #18, #9
	ldr	r3, [r4, #2400]
	bfi	r6, r2, #9, #9
	mov	r2, r8
	bfi	r6, r3, #0, #9
	str	r6, [r10, #1844]
	ldr	r3, [r4, #2404]
	ldr	r6, [r4, #2408]
	bfi	lr, r3, #16, #8
	ldr	r3, [r4, #2412]
	bfi	lr, r6, #8, #8
	mov	r6, r8
	bfi	lr, r3, #0, #8
	str	lr, [r10, #1848]
	ldr	r3, [r4, #2416]
	ldr	lr, [r4, #2420]
	bfi	r1, r3, #8, #4
	ldr	r3, [r4, #2424]
	bfi	r1, lr, #4, #4
	mov	lr, r8
	bfi	r1, r3, #0, #4
	mov	r3, #3
	bfi	r1, r3, #20, #8
	mov	r3, r8
	bfi	r1, r5, #12, #8
	str	r1, [r10, #1852]
	ldr	r5, [r4, #2556]
	bfi	r8, r5, #0, #16
	mov	r5, #1
	bfc	r8, #16, #8
	bfc	r8, #24, #8
	str	r8, [r10, #1856]
	ldr	r1, [r4, #2572]
	mov	r8, r7
	bfi	r7, r1, #0, #8
	ldr	r1, [r4, #2568]
	bfi	r7, r1, #8, #8
	ldr	r1, [r4, #2564]
	bfi	r7, r1, #16, #8
	ldr	r1, [r4, #2560]
	bfi	r7, r1, #24, #8
	str	r7, [r10, #1860]
	ldr	r1, [r4, #2432]
	mov	r7, ip
	bfi	ip, r1, #24, #8
	ldr	r1, [r4, #2436]
	bfi	ip, r1, #16, #8
	ldr	r1, [r4, #2440]
	bfi	ip, r1, #8, #8
	ldr	r1, [r4, #2444]
	bfi	ip, r1, #0, #8
	str	ip, [r10, #1872]
	ldr	r1, [r4, #2448]
	bfi	r0, r1, #0, #8
	ldr	r1, [r4, #2452]
	bfi	r0, r1, #8, #8
	ldr	r1, [r4, #2456]
	bfi	r0, r1, #16, #1
	ldr	r1, [r4, #2600]
	bfi	r0, r1, #17, #1
	str	r0, [r10, #1876]
	ldr	r0, [r4, #2460]
	mov	r1, r9
	bfi	r9, r0, #0, #16
	ldr	r0, [r4, #2464]
	bfi	r9, r0, #16, #16
	str	r9, [r10, #1880]
	ldr	r0, [r4, #2468]
	mov	r9, r2
	bfi	r2, r0, #22, #9
	ldr	r0, [r4, #2472]
	bfi	r2, r0, #17, #5
	ldr	r0, [r4, #2476]
	bfi	r2, r0, #9, #8
	ldr	r0, [r4, #2428]
	bfi	r2, r0, #0, #9
	str	r2, [r10, #1884]
	ldr	r0, [r4, #2480]
	mov	r2, r6
	bfi	r6, r0, #8, #14
	ldr	r0, [r4, #2484]
	bfi	r6, r0, #0, #8
	str	r6, [r10, #1792]
	ldr	ip, [r4, #2488]
	mov	r0, lr
	mov	r6, r8
	bfi	lr, ip, #16, #8
	ldr	ip, [r4, #2492]
	bfi	lr, ip, #8, #8
	ldr	ip, [r4, #2496]
	bfi	lr, ip, #0, #8
	str	lr, [r10, #1796]
	ldr	lr, [r4, #2504]
	mov	ip, r3
	bfi	r3, lr, #17, #1
	ldr	lr, [r4, #2508]
	bfi	r3, lr, #12, #2
	ldr	lr, [r4, #2512]
	bfi	r3, lr, #4, #4
	ldr	lr, [r4, #2516]
	bfi	r3, lr, #0, #4
	str	r3, [r10, #1800]
	ldr	r3, [r4, #2520]
	mov	lr, r8
	bfi	r6, r3, #0, #10
	ldr	r3, [r4, #2524]
	bfi	r6, r3, #16, #9
	str	r6, [r10, #1808]
	ldr	r6, [r4, #2528]
	mov	r3, r8
	bfi	r3, r5, #0, #8
	bfi	lr, r6, #0, #10
	ldr	r6, [r4, #2532]
	bfi	r3, r5, #8, #8
	bfi	lr, r6, #16, #9
	str	lr, [r10, #1812]
	ldr	lr, [r4, #2536]
	movt	r3, 200
	ldr	r6, [r4, #2540]
	bfi	r8, lr, #0, #10
	mov	lr, r7
	bfi	r8, r6, #16, #9
	str	r8, [r10, #1816]
	ldr	r8, [r4, #2544]
	ldr	r6, [r4, #2548]
	bfi	r7, r8, #0, #10
	ldr	r8, [fp, #-48]
	bfi	r7, r6, #16, #9
	str	r7, [r10, #1820]
	ldr	r6, [r4, #2552]
	mov	r7, r1
	bfi	r7, r6, #0, #16
	str	r7, [r10, #1804]
	ldr	r7, [r4, #2576]
	ldr	r6, [r4, #2580]
	bfi	r1, r7, #0, #13
	bfi	r1, r6, #13, #1
	str	r1, [r10, #36]
	ldr	r1, [r4, #2584]
	bfi	r9, r1, #0, #18
	str	r9, [r10, #40]
	ldr	r1, [r4, #2588]
	bfi	r2, r1, #0, #18
	str	r2, [r10, #44]
	ldr	r1, [r4, #2612]
	ldr	r2, [r4, #2616]
	bfi	r0, r1, #16, #7
	bfi	r0, r2, #23, #7
	str	r0, [r8, #1792]
	ldr	r2, [r4, #2604]
	ldr	r1, [r4, #2608]
	bfi	ip, r2, #0, #8
	ldr	r2, [r4, #2592]
	bfi	ip, r1, #8, #8
	bfi	ip, r2, #16, #8
	str	ip, [r8, #1796]
	ldr	r2, [r4, #2596]
	ldr	r1, [r4, #2620]
	bfi	lr, r2, #12, #8
	ldr	r2, [r4, #2624]
	bfi	lr, r1, #8, #4
	bfi	lr, r2, #0, #8
	str	lr, [r8, #1800]
	str	r3, [r8, #1804]
	ldr	r2, [r4, #20]
	cmp	r2, #0
	bne	.L88
	mov	r1, #23
	mov	r3, #0
	bfi	r3, r1, #0, #5
	mov	r0, #15
	bfi	r3, r0, #5, #5
	bfi	r3, r1, #10, #5
	bfi	r3, r0, #15, #5
	bfi	r3, r2, #20, #1
	str	r3, [r10, #24]
.L89:
	ldr	r8, [fp, #-52]
	mov	r1, #16
	mov	r2, #0
	mov	r3, #0
	bfc	r2, #16, #16
	bfi	r3, r1, #6, #6
	bfc	r2, #0, #16
	mov	r1, #10
	str	r2, [r10, #552]
	bfi	r3, r1, #0, #6
	mov	r2, #2
	str	r3, [r10, #556]
	str	r2, [r10, #548]
	mov	r3, #0
	ldr	ip, [r8, #8]
	mov	r1, #51
	ldr	r0, [r8, #4]
	mov	r2, #4
	bfi	r3, ip, #8, #6
	orr	r3, r3, #64
	bfi	r3, r0, #0, #6
	bfi	r3, r1, #16, #6
	bfi	r3, r2, #24, #4
	ldr	r2, [fp, #-48]
	str	r3, [r2, #3076]
	ldr	r3, [r4, #20]
	ldr	r0, [r8, #12]
	cmp	r3, #0
	sub	r0, r0, #1
	moveq	r6, #16
	movne	r6, #64
	mov	r1, r6
	add	r0, r0, r6
	bl	__aeabi_uidiv
	ldr	r3, [r8, #16]
	mov	r1, r6
	mov	r7, r0
	sub	r0, r3, #1
	add	r0, r0, r6
	bl	__aeabi_uidiv
	mul	r1, r7, r0
	ldr	r0, [r8, #168]
	bl	__aeabi_uidiv
	mov	r3, #0
	ldr	r5, [r4, #1788]
	mov	lr, r3
	mov	r8, r3
	mov	r7, r3
	mov	r6, r3
	mov	r1, r3
	mov	r9, r3
	mov	ip, r3
	str	r3, [fp, #-60]
	add	r2, r4, #1808
	str	r3, [fp, #-64]
	add	r2, r2, #12
	str	r3, [fp, #-68]
	str	r3, [fp, #-72]
	str	r3, [fp, #-76]
	str	r3, [fp, #-80]
	add	r3, r10, #1088
	add	r3, r3, #12
	str	r3, [fp, #-56]
	add	r3, r4, #1872
	add	r3, r3, #12
	bfi	lr, r0, #0, #16
	bfi	lr, r5, #16, #6
	ldr	r5, [fp, #-48]
	str	lr, [r5, #3080]
	mov	lr, r1
	ldr	r0, [r4, #1820]
	bfi	r8, r0, #0, #6
	ldr	r0, [r4, #1780]
	bfi	r8, r0, #24, #6
	ldr	r0, [r4, #1784]
	bfi	r8, r0, #16, #6
	ldr	r0, [r4, #1792]
	bfi	r8, r0, #8, #6
	str	r8, [r5, #3084]
	ldr	r0, [r4, #1640]
	bfi	r7, r0, #24, #8
	ldr	r0, [r4, #1636]
	bfi	r7, r0, #16, #8
	ldr	r0, [r4, #1632]
	bfi	r7, r0, #8, #8
	ldr	r0, [r4, #1628]
	bfi	r7, r0, #0, #8
	str	r7, [r5, #3088]
	ldr	r0, [r4, #1656]
	bfi	r6, r0, #24, #8
	ldr	r0, [r4, #1652]
	bfi	r6, r0, #16, #8
	ldr	r0, [r4, #1648]
	bfi	r6, r0, #8, #8
	ldr	r0, [r4, #1644]
	bfi	r6, r0, #0, #8
	str	r6, [r5, #3092]
	ldr	r0, [r4, #1672]
	mov	r6, r9
	bfi	r6, r0, #24, #8
	ldr	r0, [r4, #1668]
	bfi	r6, r0, #16, #8
	ldr	r0, [r4, #1664]
	bfi	r6, r0, #8, #8
	ldr	r0, [r4, #1660]
	bfi	r6, r0, #0, #8
	str	r6, [r5, #3096]
	ldr	r0, [r4, #1688]
	mov	r6, r1
	bfi	r9, r0, #24, #8
	ldr	r0, [r4, #1684]
	bfi	r9, r0, #16, #8
	ldr	r0, [r4, #1680]
	bfi	r9, r0, #8, #8
	ldr	r0, [r4, #1676]
	bfi	r9, r0, #0, #8
	str	r9, [r5, #3072]
	ldr	r0, [r4, #1812]
	bfi	lr, r0, #0, #4
	ldr	r0, [r4, #1816]
	bfi	lr, r0, #4, #4
	str	lr, [r5, #3100]
	ldr	r0, [r4, #1764]
	mov	lr, r1
	bfi	r6, r0, #0, #4
	str	r6, [r5, #3104]
	ldr	r6, [r4, #1760]
	ldr	r0, [r4, #1768]
	bfi	lr, r6, #0, #4
	bfi	lr, r0, #4, #4
	str	lr, [r5, #3108]
	ldr	r0, [r4, #1772]
	mov	lr, r1
	bfi	lr, r0, #0, #4
	str	lr, [r5, #3112]
	ldr	r6, [r4, #1692]
	mov	lr, ip
	ldr	r0, [r4, #1696]
	bfi	r1, r6, #0, #2
	ldr	r6, [r4, #1700]
	bfi	r1, r0, #2, #2
	ldr	r0, [r4, #1704]
	bfi	r1, r6, #4, #2
	ldr	r6, [r4, #1708]
	bfi	r1, r0, #6, #2
	ldr	r0, [r4, #1712]
	bfi	r1, r6, #8, #2
	ldr	r6, [r4, #1716]
	bfi	r1, r0, #10, #2
	ldr	r0, [r4, #1720]
	bfi	r1, r6, #12, #2
	ldr	r6, [r4, #1724]
	bfi	r1, r0, #14, #2
	ldr	r0, [r4, #1728]
	bfi	r1, r6, #16, #2
	ldr	r6, [r4, #1732]
	bfi	r1, r0, #18, #2
	ldr	r0, [r4, #1736]
	bfi	r1, r6, #20, #2
	ldr	r6, [r4, #1740]
	bfi	r1, r0, #22, #2
	ldr	r0, [r4, #1744]
	bfi	r1, r6, #24, #2
	ldr	r6, [r4, #1748]
	bfi	r1, r0, #26, #2
	ldr	r0, [r4, #1752]
	bfi	r1, r6, #28, #2
	bfi	r1, r0, #30, #2
	str	r1, [r5, #3116]
	ldr	r1, [r4, #1756]
	bfi	lr, r1, #0, #5
	str	lr, [r5, #3120]
	ldr	r1, [r4, #1800]
	ldr	r0, [r4, #1804]
	bfi	ip, r1, #24, #4
	ldr	r1, [r4, #1808]
	bfi	ip, r0, #28, #4
	ldr	r0, [r4, #1776]
	bfi	ip, r1, #16, #6
	ldr	r1, [r4, #1796]
	bfi	ip, r0, #8, #4
	ldr	lr, [fp, #-56]
	bfi	ip, r1, #0, #6
	str	ip, [r5, #3124]
	ldr	r5, [fp, #-128]
.L91:
	ldr	r1, [r2, #4]!
	cmp	r2, r3
	bfi	r5, r1, #0, #20
	str	r5, [lr, #4]!
	bne	.L91
	add	r2, r10, #1264
	add	r0, r4, #2128
	add	r2, r2, #12
	add	r0, r0, #12
.L92:
	ldr	r1, [r3, #4]!
	cmp	r3, r0
	bfi	r5, r1, #0, #20
	str	r5, [r2, #4]!
	bne	.L92
	ldr	r7, [fp, #-48]
	mov	r5, #0
	mov	r1, r5
	mov	r2, r5
	bfi	r1, r5, #0, #16
	bfi	r2, r5, #0, #16
	mov	r3, r5
	bfi	r3, r5, #16, #16
	str	r5, [r7, #3660]
	bfi	r1, r5, #16, #16
	str	r5, [r10, #572]
	bfi	r2, r5, #16, #16
	str	r5, [r10, #580]
	bfi	r3, r5, #0, #16
	str	r1, [r10, #584]
	mov	r0, r5
	str	r2, [r10, #588]
	bfi	r0, r5, #0, #1
	str	r3, [r10, #592]
	mvn	r2, #0
	str	r0, [r10, #564]
	mov	r3, #7
	str	r2, [r10, #596]
	mov	r1, r5
	str	r2, [r10, #604]
	bfi	r1, r5, #0, #1
	str	r2, [r10, #612]
	mov	r6, r5
	str	r1, [r10, #568]
	mov	r1, #13
	str	r2, [r10, #620]
	mov	ip, r5
	str	r1, [r10, #576]
	mov	lr, r5
	str	r3, [r10, #600]
	mov	r2, r5
	str	r3, [r10, #608]
	mov	r0, r4
	str	r3, [r10, #616]
	str	r3, [r10, #624]
	ldr	r3, [r4, #2348]
	ldr	r8, [fp, #-52]
	bfi	r6, r3, #0, #1
	str	r6, [r10, #120]
	ldr	r3, [r4, #2352]
	mov	r1, r8
	str	r3, [r10, #96]
	ldr	r3, [r4, #2356]
	str	r3, [r10, #100]
	ldr	r3, [r4, #2360]
	str	r3, [r10, #104]
	ldr	r3, [r4, #2364]
	str	r3, [r10, #108]
	ldr	r3, [r4, #2368]
	str	r3, [r10, #112]
	ldr	r3, [r4, #2372]
	str	r3, [r10, #116]
	ldr	r6, [r4, #1608]
	ldr	r3, [r4, #1612]
	bfi	ip, r6, #0, #16
	bfi	ip, r3, #16, #16
	str	ip, [r10, #1728]
	ldr	r3, [r4, #1624]
	bfi	lr, r3, #16, #16
	str	lr, [r10, #1732]
	ldr	ip, [r4, #1616]
	ldr	r3, [r4, #1620]
	bfi	r2, ip, #0, #16
	bfi	r2, r3, #16, #16
	str	r2, [r10, #1736]
	bl	VENC_HAL_VEDU_DDRCfg_CABAC_VLC
	ldr	r1, [r4, #44]
	add	r0, r7, #8192
	mov	r2, r5
	bfi	r2, r5, #0, #1
	mov	r3, r5
	str	r1, [r10, #68]
	bfi	r3, r5, #16, #8
	str	r2, [r0, #1040]
	mov	ip, #85
	ldr	r2, [r4, #432]
	str	r2, [r0, #1044]
	ldr	r2, [r8, #44]
	cmp	r2, r5
	bfieq	r3, r2, #0, #1
	orrne	r3, r3, #1
	str	r3, [r10, #76]
	mov	r2, #0
	ldr	r1, [r4, #264]
	mvn	r3, #84
	bfi	r2, r3, #0, #8
	mov	r3, #0
	bfi	r2, ip, #8, #8
	str	r3, [r10, #84]
	str	r1, [r10, #80]
	mov	r7, r3
	str	r2, [r10, #28]
	mov	ip, r3
	str	r3, [r10, #88]
	mov	lr, r3
	str	r3, [r10, #92]
	mov	r6, r3
	ldr	r8, [r4, #2144]
	mov	r9, r3
	ldr	r5, [r4, #2148]
	mov	r2, r3
	bfi	r7, r8, #0, #6
	ldr	r8, [r4, #2152]
	bfi	r7, r5, #8, #2
	ldr	r5, [r4, #2156]
	bfi	r7, r8, #10, #1
	ldr	r8, [r4, #2160]
	bfi	r7, r5, #12, #4
	mov	r1, r3
	bfi	r7, r8, #16, #5
	str	r7, [r0, #4048]
	ldr	r10, [r4, #2212]
	mov	r5, r3
	ldr	r8, [fp, #-48]
	bfi	r5, r3, #0, #1
	mov	r7, r3
	bfi	r5, r3, #1, #1
	str	r10, [r0, #4052]
	add	r8, r8, #61440
	ldr	r10, [r4, #2216]
	bfi	r5, r3, #2, #1
	bfi	r5, r3, #3, #1
	str	r10, [r0, #4056]
	ldr	r10, [r4, #2220]
	bfi	ip, r10, #0, #16
	ldr	r10, [r4, #2224]
	bfi	ip, r10, #16, #2
	ldr	r10, [r4, #2228]
	bfi	ip, r10, #18, #2
	ldr	r10, [r4, #2232]
	bfi	ip, r10, #20, #4
	ldr	r10, [r4, #2236]
	bfi	ip, r10, #24, #1
	ldr	r10, [r4, #2244]
	bfi	ip, r10, #25, #2
	ldr	r10, [r4, #2248]
	bfi	ip, r10, #27, #1
	ldr	r10, [r4, #2240]
	bfi	ip, r10, #28, #1
	str	ip, [r0, #4060]
	ldr	r10, [r4, #2280]
	mov	ip, #1
	bfi	lr, r10, #0, #1
	ldr	r10, [r4, #2276]
	bfi	lr, r10, #2, #2
	ldr	r10, [r4, #2272]
	bfi	lr, r10, #4, #2
	ldr	r10, [r4, #2268]
	bfi	lr, r10, #6, #1
	ldr	r10, [r4, #2288]
	bfi	lr, r10, #8, #6
	ldr	r10, [r4, #2292]
	bfi	lr, r10, #24, #6
	str	lr, [r0, #4064]
	ldr	lr, [r4, #2296]
	ldr	r10, [r4, #2304]
	bfi	r6, lr, #0, #6
	ldr	lr, [r4, #2300]
	bfi	r6, lr, #8, #6
	ldr	lr, [r4, #2308]
	bfi	r6, r10, #16, #6
	bfi	r6, lr, #24, #6
	str	r6, [r0, #4068]
	ldr	r6, [r4, #2312]
	ldr	lr, [r4, #2316]
	bfi	r9, r6, #0, #6
	bfi	r9, lr, #6, #25
	str	r9, [r0, #4072]
	ldr	r0, [r4, #324]
	ldr	lr, [r4, #340]
	bfi	r2, r0, #10, #1
	ldr	r0, [r4, #320]
	bfi	r2, lr, #9, #1
	ldr	lr, [r4, #344]
	bfi	r2, r0, #8, #1
	ldr	r0, [r4, #348]
	bfi	r2, lr, #7, #1
	ldr	lr, [r4, #332]
	bfi	r2, r0, #6, #1
	ldr	r0, [r4, #328]
	bfi	r2, lr, #5, #1
	ldr	lr, [r4, #352]
	bfi	r2, r0, #4, #1
	ldr	r0, [r4, #356]
	bfi	r2, lr, #3, #1
	ldr	lr, [r4, #360]
	bfi	r2, r0, #2, #1
	ldr	r0, [r4, #364]
	bfi	r2, lr, #1, #1
	ldr	lr, [fp, #-48]
	bfi	r2, r0, #0, #1
	str	r2, [lr, #504]
	ldr	r0, [r4, #336]
	ldr	r2, [r4, #368]
	bfi	r1, r0, #7, #1
	ldr	r0, [r4, #372]
	bfi	r1, r2, #6, #1
	ldr	r2, [r4, #376]
	bfi	r1, r0, #5, #1
	ldr	r0, [r4, #380]
	bfi	r1, r2, #4, #1
	ldr	r2, [r4, #384]
	bfi	r1, r0, #3, #1
	ldr	r0, [r4, #388]
	bfi	r1, r2, #2, #1
	ldr	r2, [r4, #392]
	bfi	r1, r0, #1, #1
	bfi	r1, r2, #0, #1
	str	r1, [lr, #508]
	ldr	r1, [r4, #396]
	ldr	r2, [r4, #400]
	bfi	r7, r1, #0, #1
	bfi	r7, r2, #3, #1
	mov	r2, #3
	bfi	r7, r2, #16, #4
	str	r7, [r8]
	str	r5, [r8, #32]
	ldr	r2, [r4, #16]
	ldr	r1, [r4, #2628]
	add	r2, r2, #61440
	str	r1, [r2, #520]
	ldr	r1, [r4, #2628]
	str	r1, [r2, #524]
	ldr	r1, [r4, #2636]
	str	r1, [r2, #752]
	ldr	r1, [r4, #2632]
	str	r1, [r2, #756]
	ldr	r1, [r4, #2636]
	str	r1, [r2, #772]
	ldr	r1, [r4, #2632]
	str	r1, [r2, #776]
	str	r3, [r8, #8]
	str	r3, [lr, #16]
	str	ip, [lr, #16]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L88:
	mov	r2, #15
	mov	r3, #0
	bfi	r3, r2, #0, #5
	bfi	r3, r2, #5, #5
	bfi	r3, r2, #10, #5
	bfi	r3, r2, #15, #5
	bfc	r3, #20, #1
	str	r3, [r10, #24]
	b	.L89
.L99:
	mov	r2, #6
.L84:
	bfi	r3, r2, #6, #4
	bfc	r3, #14, #1
.L85:
	ldrb	r2, [r4, #60]
	b	.L86
.L105:
	ldr	r2, [r9, #16]
	ldr	r3, [r9, #12]
	mov	r2, r2, lsr #2
	sub	r2, r2, #16
	mov	r3, r3, lsr #2
	sub	r3, r3, #16
	mov	r2, r2, lsr #1
	cmp	r2, #32
	mov	r3, r3, lsr #1
	movcs	r2, #32
	cmp	r3, #72
	bic	r2, r2, #-2147483645
	movcs	r3, #72
	cmp	r2, #7
	bic	r3, r3, #-2147483645
	movls	r2, #7
	bls	.L81
	cmp	r2, #32
	movcs	r2, #32
	sub	r2, r2, #1
	and	r2, r2, #63
.L81:
	cmp	r3, #7
	movls	r3, #7
	bls	.L82
	cmp	r3, #72
	movcs	r3, #72
	sub	r3, r3, #1
	and	r3, r3, #127
.L82:
	mov	ip, #0
	bfi	ip, r2, #16, #6
	bfi	ip, r3, #0, #7
	b	.L83
	UNWIND(.fnend)
	.size	VENC_HAL_DDRCfgReg, .-VENC_HAL_DDRCfgReg
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
