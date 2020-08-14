@/******************************************************************************
@ *    Copyright (c) 2009-2012 by Hisi.
@ *    All rights reserved.
@ * ***
@ *    Create by Czyong. 2012-09-07
@ *
@******************************************************************************/

#include <config.h>

#ifndef CONFIG_UNIFIED_BOOT
#include "start_fpga.S"
#else
/******************************************************************************/
#define STACK_TOP               0xFFFF0C00
/******************************************************************************/
.macro step, num
#ifdef CONFIG_BOOT_SIMULATE
        @push {r11, r12}
        ldr r11, =REG_BASE_SCTL
        mov r12, #\num
        str r12, [r11, #REG_SC_GEN31]
        @pop {r11, r12}
#endif
.endm

.globl _start
_start:

#ifndef CONFIG_NO_HEAD
/*
 * +-----------------------------------------+
 * | 1. Head Area (Key and Param)            |
 * +-----------------------------------------+
 */
	b	_checked_area_start

/* 1.1 auxarea address */
.=CONFIG_AUXAREA_ADR_POS
	.word _auxcode_start-TEXT_BASE

/* 1.2 auxarea length */
.=CONFIG_AUXAREA_LEN_POS
	.word CONFIG_AUXAREA_LEN

/* 1.3 boot entry point */
.=CONFIG_BOOT_ENTRY_POS
	.word _checked_area_start

/*
 * +-----------------------------------------+
 * | 2. Param Area                           |
 * +-----------------------------------------+
 */
.=CONFIG_PARAM_AREA_POS

/* 2.1 SCS Hashed area offset */
.=CONFIG_SCS_HASHED_AREA_OFF_POS
	.word CONFIG_SCS_HASHED_AREA_OFF_VAL

/* 2.2 SCS Hashed area length */
.=CONFIG_SCS_HASHED_AREA_LEN_POS
	.word _checked_area_end - _checked_area_start

/* 2.3 Total boot area length */
.=CONFIG_TOTAL_BOOT_AREA_LEN_POS
	.word _total_boot_area_end - _total_boot_area_start

/* 2.4 Default boot reg */
.=CONFIG_DEFAULT_BOOT_REG_POS
	.incbin	CONFIG_BOOTREG_DEFAULT

/* 2.5 Param signature */
.=CONFIG_PARAM_AREA_SIG_POS
	.fill 0x100,1,0

/*
 * +-----------------------------------------+
 * | 3. Unchecked area for hisi              |
 * +-----------------------------------------+
 */
.=CONFIG_UNCHK_AREA_HISI_POS
/* 3.1 SCS simulation flag */
.=CONFIG_SCS_SIM_FLAG_POS
	.word CONFIG_SCS_SIM_FLAG_VAL

/* 3.2 Boot flag */
.=CONFIG_BOOT_FLAG_POS
	.word CONFIG_BOOT_FLAG_VAL

/* 3.3 auxcode encrypt flag */
.=CONFIG_AUX_ENC_FLAG_POS
	.word CONFIG_AUX_ENC_FLAG_VAL

/* 3.4 Support multi param */
.=CONFIG_SUPPORT_MULTI_PARAM_POS
#if (REGNUM > 1)
	.word 0x1
#endif

/* 3.5 param start address */
.=CONFIG_PARAM_START_ADDR_POS
	.word reg_list_start-TEXT_BASE

/* 3.6 param item length */
.=CONFIG_PARAM_ITEM_LEN_POS
	.word CONFIG_REG_SIZE

/* 3.7 Boot store address */
.=CONFIG_BOOT_STORE_ADDR_POS
	.word CONFIG_BOOT_STRORE_ADDR_VAL

/*
 * +-----------------------------------------+
 * | 4. Auxiliary Code Area                  |
 * +-----------------------------------------+
 */
 .=CONFIG_AUXCODE_AREA_POS
_auxcode_start:
	.incbin   CONFIG_AUXIMAGE
.align	8 /* align to 0x100 */
_auxcode_end:
 .=CONFIG_AUXCODE_AREA_POS + CONFIG_AUXAREA_LEN

/*
 * +-----------------------------------------+
 * | 5. Unchecked Area                       |
 * +-----------------------------------------+
 */
 _total_boot_area_start:
_unchecked_area:
	.fill CONFIG_UNCHK_SIZE,1,0

/*
 * +-----------------------------------------+
 * | 6. Checked Area (fastboot)              |
 * +-----------------------------------------+
 */
#endif
.=CONFIG_CHECKED_AREA_START
_checked_area_start:
	b	reset
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq

_undefined_instruction: .word irq_undefined_instruction
_software_interrupt:    .word irq_software_interrupt
_prefetch_abort:        .word irq_prefetch_abort
_data_abort:            .word irq_data_abort
_not_used:              .word irq_not_used
_irq:                   .word irq_irq
_fiq:                   .word irq_firq
_pad1:                  .word 0x435a590d

.=_checked_area_start+0x40

.balignl 16,0xdeadbeef
__start:
	.word _start


_TEXT_BASE:
	.word TEXT_BASE

.globl _armboot_start
_armboot_start:
	.word _start

/*
 * These are defined in the board-specific linker script.
 */
.globl _bss_start
_bss_start:
	.word __bss_start

.globl _bss_end
_bss_end:
	.word __bss_end

.globl _relocate
_relocate:
	.word relocate

.globl _copy_abort_code
_copy_abort_code:
	.word  copy_abort_code

.globl __checked_area_start
__checked_area_start:
	.word _checked_area_start

/*
 * the actual reset code
 */
reset:
	step 41
	isb

	/*
	 *  read and write system register REG_SC_GEN1
	 *  judge BUS mux whether or not OK
	 */
	ldr	r0, =REG_BASE_SCTL
	mov	r1, #0
wait_mux_finish:
	add	r1, r1, #1
	str	r1, [r0, #REG_SC_GEN1]
	ldr	r2, [r0, #REG_SC_GEN1]
	cmp	r1, r2
	bne	wait_mux_finish

	step 42

normal_start_flow:
	/* set the cpu to SVC32 mode */
	mrs	r0, cpsr
	bic	r0, r0, #0x1f  /* r0 = ((~0x1F) & r0) */
	orr	r0, r0, #0xd3  /* r0 = (0xd3 | r0)    */
	msr	cpsr, r0

	/* Enable SMP mode for CPU0, by setting bit 6 of CPU Extend Ctl reg */
	mrrc	p15,1,r0,r1,c15
	orr	r0, r0, #(1 << 6)
	mcrr	p15,1,r0,r1,c15

	step 46
	bl	flash_cache_all
	step 47

main_cpu:
	/* init serial and printf a string. */
	bl	uart_early_init
	step 48
	bl	msg_main_cpu_startup


	ldr	sp, =STACK_TOP

	/* enable I-Cache now */
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #0x00001000     /* set bit 12 (I) I-Cache */
	mcr	p15, 0, r0, c1, c0, 0
	isb	@ add ISB or I-cache enabled is  not efficient right now

#ifdef CONFIG_GPIO_LIGHT_SET
enable_green_light:
	mov	r0, #CONFIG_GPIO_LIGHT_SET  /* GPIO group num * 8 + GPIO bit num */
	sub	r0, r0, #40
	ldr	r1, =REG_BASE_GPIO5 /* GPIO5 will no power off when suspending */
	ldr	r2, [r1, #0x400]
	mov	r3, #0x1
	orr	r2, r3, LSL r0
	str	r2, [r1, #0x400]
	mov	r2, #0
	mov	r3, #0x4
	mov	r3, r3, LSL r0
	str	r2, [r1, r3]
#endif

	/* relocate Boot */
	bl	display_relocate

relocate:                            /* relocate U-Boot to RAM */
	step 64
	mov	r2, pc
	sub	r2, r2, #8       /* Get the ddr address of relocate label */
	ldr	r1, _armboot_start
	ldr	r0, _relocate
	sub	r1, r0, r1
	sub	r0, r2, r1       /* Get the ddr address where the fastboot loaded */
	ldr	r4, =REG_BASE_SCTL
	str	r0, [r4, #REG_SC_GEN14]


	/* now, r0 stores __reset offset from where we get started */
	ldr	r1, _TEXT_BASE   /* r1 stores where we will copy uboot to */

	/* compare source and target address, if equal no copy to target address */
	cmp	r0, r1
	beq	copy_abort_code

	step 75
	ldr	r2, _armboot_start
	ldr	r3, _bss_start
	sub	r2, r3, r2      /* r2 <- size of armboot */

	/* memcpy(r1, r0, r2) */
	bl	memcpy

copy_abort_code:
	ldr	r4, =REG_BASE_SCTL
	ldr	r0, __checked_area_start
	str	r0, [r4, #REG_SC_GEN14]

	step 68
	ldr	r1, =0xFFFF0000
	mov	r2, #0x4000
	/* memcpy(r1, r0, r2) */
	bl	memcpy
	step 76
	/* Set up the stack */
stack_setup:
	ldr	r0, _TEXT_BASE                      @ upper 128 KiB: relocated uboot
	sub	r0, r0, #CONFIG_BOOTHEAD_GAP        @ boot head gap
	sub	r0, r0, #CONFIG_SYS_MALLOC_LEN      @ malloc area
	sub	r0, r0, #CONFIG_SYS_GBL_DATA_SIZE   @ bdinfo
	sub	sp, r0, #12              @ leave 3 words for abort-stack
	and	sp, sp, #~7              @ 8 byte alinged for (ldr/str)d

	step 77

	/* Clear BSS (if any). Is below tx (watch load addr - need space) */
clear_bss:
	ldr	r0, _bss_start      @ find start of bss segment
	ldr	r1, _bss_end        @ stop here
	mov	r2, #0x00000000     @ clear value

	step 78

clear_bss_loop:
	str	r2, [r0]        @ clear BSS location
	cmp	r0, r1          @ are we at the end yet
	add	r0, r0, #4      @ increment clear index pointer
	bne	clear_bss_loop  @ keep clearing till at end

	step 79
	mov	r0, #0

	bl	display_jump_to_ddr

	/* Data and Instruction sync and invalidate I-cache */
	dsb
	isb
	step 80
	ldr	pc, _arm_start  @ jump to C code

_arm_start: .word arm_start

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void memcpy(r1, r0, r2);
@
.align	2
memcpy:
	add	r2, r0, r2
memcpy_loop:
	ldmia	r0!, {r3 - r10}
	stmia	r1!, {r3 - r10}
	cmp	r0, r2
	ble	memcpy_loop
	mov	pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@  corrupt: r0, r1, r2, r3
@
.align	2
flash_cache_all:

	/* disable MMU stuff and caches */
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #0x00002000      /* clear bits 13 (--V-)   */
	bic	r0, r0, #0x00000007      /* clear bits 2:0 (-CAM)  */
	orr	r0, r0, #0x00000002      /* set bit 1 (--A-) Align */
	orr	r0, r0, #0x00000800      /* set bit 12 (Z---) BTB  */
	mcr	p15, 0, r0, c1, c0, 0

	/*
	 * Invalidate L1 I/D
	 */
	mov	r0, #0                   /* set up for MCR */
	mcr	p15, 0, r0, c8, c7, 0    /* invalidate TLBs */
	mcr	p15, 0, r0, c7, c5, 0    /* invalidate icache */

	/* Invalidate L1 D-cache */
	mcr	p15, 2, r0, c0, c0, 0    /* select L1 data cache */
	mrc	p15, 1, r3, c0, c0, 0    /* Read Current Cache Size Identification Register */
	ldr	r1, =0x1ff
	and	r3, r1, r3, LSR #13      /* r3 = (number of sets -1) */
	mov	r0, #0
way_loop:
	mov	r1, #0                  /* r1->set counter */
line_loop:
	mov	r2, r0, LSL #30
	orr	r2, r1, LSL #5           /* r2->set/way cache-op format */
	mcr     p15, 0, r2, c7, c6, 2    /* Invalidate line described by r2 */
	add	r1, r1, #1               /* Increment set counter */
	cmp	r1, r3                   /* Check if the last set is reached... */
	ble	line_loop                /* if not, continue the set_loop */
	add	r0, r0, #1               /* else, Increment way counter */
	cmp	r0, #4                   /* Check if the last way is reached... */
	blt	way_loop                 /* if not, continue the way_loop */

	mov	pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@  void display_relocate(void);
@
.align	2
.global	display_relocate
.type	display_relocate, %function
display_relocate:
	push	{r0, lr}
	add	r0, pc, #4
	bl	uart_early_puts
	pop	{r0, pc}
#ifndef CONFIG_SUPPORT_CA_RELEASE
	.ascii "\r\n\r\nRelocate Boot\0"
#else
	.ascii "\r\n\0"
#endif

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@  void display_jump_to_ddr(void);
@
.align	2
.global	display_jump_to_ddr
.type	display_jump_to_ddr, %function
display_jump_to_ddr:
	push	{r0, lr}
	add	r0, pc, #4
	bl	uart_early_puts
	pop	{r0, pc}
#ifndef CONFIG_SUPPORT_CA_RELEASE
	.ascii "\r\n\r\nJump to C code\r\n\0"
#else
	.ascii "\r\n\0"
#endif


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align	2
msg_main_cpu_startup:
	mov	r5, lr
	add	r0, pc, #4
	bl	uart_early_puts
	mov	pc, r5
L10:
#ifndef CONFIG_SUPPORT_CA_RELEASE
	.ascii "\r\n\r\nSystem startup\r\n\0"
#else
	.ascii "\r\n\r\n\r\n\0"
#endif

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align	2
.globl _text_end
_text_end: .word __text_end

#endif
