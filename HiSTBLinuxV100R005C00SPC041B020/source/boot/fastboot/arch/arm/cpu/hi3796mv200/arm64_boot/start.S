/******************************************************************************
@ *    Copyright (c) 2009-2014 by Hisi.
@ *    All rights reserved.
@ * ***
@ *    Create by zengzhongliang. 2014-07-28
@ *
@******************************************************************************/
#include "macro.h"
#include "linkage.h"
#include <configs/hi3798cv2x.h>


/* Generic Interrupt Controller Definitions */
#define CONFIG_GICV2
#define GICD_BASE              (0xf1001000)
#define GICC_BASE              (0xf1002000)
#define ARM64_GICD_BASE		(GICD_BASE)
#define ARM64_BOOT_STACK 	0xffff0c00
#define CPU_RESET_ADDR                 (0xF8A22278)
/* Generic Timer Definitions */
#define ARM64_COUNTER_FREQ	0x1800000     /* 24MHz */
#define ARM64_CPU_RELEASE_ADDR	(0xc000000)

.section .text

.type _entry, %function

/*************************************************************************
 *
 * Startup Code (reset vector)
 *
 *************************************************************************/
.globl _start
_start:
	b	reset

reset:
	/*
	 * Could be EL3/EL2/EL1, Initial State:
	 * Little Endian, MMU Disabled, i/dCache Disabled
	 */
	switch_el x1, 3f, 2f, 1f
3:	mrs     x0, scr_el3
	orr     x0, x0, #0xf                    /* SCR_EL3.NS|IRQ|FIQ|EA */
	msr     scr_el3, x0
	msr     vbar_el3, x0
	msr     cptr_el3, xzr                   /* Enable FP/SIMD */
	ldr     x0, =ARM64_COUNTER_FREQ
	msr     cntfrq_el0, x0                  /* Initialize CNTFRQ */
	b       0f
2:	msr     vbar_el2, x0
	mov     x0, #0x33ff
	msr     cptr_el2, x0                    /* Enable FP/SIMD */
	b       0f
1:	msr     vbar_el1, x0
	mov     x0, #3 << 20
	msr     cpacr_el1, x0                   /* Enable FP/SIMD */
0:

	/*
	 * Cache/BPB/TLB Invalidate
	 * i-cache is invalidated before enabled in icache_enable()
	 * tlb is invalidated before mmu is enabled in dcache_enable()
	 * d-cache is invalidated before enabled in dcache_enable()
	 */

	/* if need relocation */
	/* bl	relocation */
	mrs x0, s3_1_c15_c2_1
	orr     x0, x0, #0x40
	msr s3_1_c15_c2_1,x0
	dsb sy

	/* Processor specific initialization */
	bl      lowlevel_init

_cpu_branch:
	branch_if_master x0, x1, master_cpu

	/*
	 * Slave CPUs
	 */
slave_cpu:
	/* Set ARM64_CPU_RELEASE_ADDR to 0xFFFFFFFF*/
	ldr	x0, =ARM64_CPU_RELEASE_ADDR  /* 0xC000000 */
	ldr	x1, =0xFFFFFFFF
	str	x1,[x0]
wait_for_release:
	wfe
	ldr	x0, =ARM64_CPU_RELEASE_ADDR
	ldr	x1, [x0]
	ldr	x2, =0xFFFFFFFF
	cmp	x1, x2
	beq	wait_for_release
	br	x1	/* branch to the given address */

	 /*
	  * Master CPU
	  */
master_cpu:
	bl	bss_setup

	bl	armv8_switch_to_el2 /* switch to EL2 */
#ifdef CONFIG_ARMV8_SWITCH_TO_EL1
	bl	armv8_switch_to_el1 /* switch to EL1 */
#endif
	mov	x0, 1
	bl	slave_cores_power_up
	mov	x0, 2
	bl	slave_cores_power_up
	mov	x0, 3
	bl	slave_cores_power_up
#if 0
/* #ifdef CONFIG_SMP */
//	ldr     x0, =CPU_RESET_ADDR  /*0xF8A22278 */
//	ldr     x1, =0x00000001
//	str     x1,[x0]
/* #endif */

/* check if pmoc resume */

	ldr	x0, =0xf80000e8
	ldr	x1, [x0]
	lsr	x1, x1, #32 /*alignment bug?*/
	ldr	x2, =0x66031013
	cmp	x1, x2
	bne	un_resume_start

resume_start:
	adr	x0, _resume_msg
	bl	show_msg

	ldr	x0, =0xf80000f8
	ldr	x1, [x0] /* goto the kernel */
	blr     x1
#endif
un_resume_start:
	bl	arm64boot_main /* jump to C code */

	adr	x0, _startup_msg
	bl	show_msg

_start_kernel:
	mov	x2, #0
	ldr	x0, [x2, #0] /* get physical address to FDT blob */
	ldr	x1, [x2, #8] /* get physical address to kernel image */
	blr	x1	/* jump to kernel(fdt address in x0) */

/*-----------------------------------------------------------------------*/
.align	3
WEAK(lowlevel_init)
	mov     x29, lr                 /* Save LR */

#if defined(CONFIG_GICV2) || defined(CONFIG_GICV3)
	branch_if_slave x0, 1f
	ldr     x0, =ARM64_GICD_BASE
	bl      gic_init_secure
1:
#if defined(CONFIG_GICV3)
	ldr     x0, =ARM64_GICR_BASE
	bl      gic_init_secure_percpu
#elif defined(CONFIG_GICV2)
	ldr     x0, =ARM64_GICD_BASE
	ldr     x1, =GICC_BASE
	bl      gic_init_secure_percpu
#endif

	branch_if_master x0, x1, 2f

	/*
	 * Slave should wait for master clearing spin table.
	 * This sync prevent salves observing incorrect
	 * value of spin table and jumping to wrong place.
	 */
#if defined(CONFIG_GICV2) || defined(CONFIG_GICV3)
#ifdef CONFIG_GICV2
	ldr     x0, =GICC_BASE
#endif
	/* bl      gic_wait_for_interrupt */
#endif

	/*
	 * All slaves will enter EL2 and optionally EL1.
	 */
	bl      armv8_switch_to_el2
#ifdef CONFIG_ARMV8_SWITCH_TO_EL1
	bl      armv8_switch_to_el1
#endif

2:
#endif
	mov     lr, x29                 /* Restore LR */
	ret
ENDPROC(lowlevel_init)

WEAK(relocation)
	mov     x29, lr

	adr	x0, relocation
	ldr	x3, =_start
	ldr	x4, =relocation

	sub	x4, x4, x3	/* offset from _start to relocation */
	sub	x0, x0, x4
	ldr	x1, =TEXT_BASE
	cmp	x0, x1
	b.ne	_relocate

	mov     lr, x29
	ret

_relocate:
	ldr	x4, =_bss_start
	sub	x4, x4, x3	/* offset from _start to _bss_start */
	add	x2, x0, x4	/* length to move */
_self_move:
	ldp     x3, x4, [x0], #16
	stp     x3, x4, [x1], #16
	cmp     x0, x2
	b.lo	_self_move

	ldr	lr, _relocated
	ret
ENDPROC(relocation)

WEAK(bss_setup)
	ldr	x0, =_bss_start
	ldr	x1, =_bss_end
	mov	x2, #0
_clear_bss_loop:
	mov     x29, lr

	str	x2, [x0]
	cmp	x0, x1
	add	x0, x0, #8
	b.ne	_clear_bss_loop

	ldr	x3, =ARM64_BOOT_STACK
	mov	sp, x3

	mov     lr, x29
	ret
ENDPROC(bss_setup)

WEAK(show_msg)	/* msg in x0 */
	mov	x29, lr

	ldr	x1, =CONFIG_CUR_UART_BASE	/* uart base address */
	b	next_char
output:
	ldtrb	w4, [x1, #24]
	tst	w4, #32	/* overflow flag */
	bne	output
	sttrb	w3, [x1, #0]
	add	x0, x0, #1
next_char:
	ldxrb	w3, [x0]
	cmp	w3, #0
	bne	output

	mov	lr, x29
	ret
ENDPROC(show_msg)

.section .data
_relocated:
	.dword TEXT_BASE + _cpu_branch - _start

_startup_msg:
	.ascii "\r\nStarting kernel...\r\n\r\n\0"

_resume_msg:
	.ascii "\r\nResume system...\r\n\r\n\0"
