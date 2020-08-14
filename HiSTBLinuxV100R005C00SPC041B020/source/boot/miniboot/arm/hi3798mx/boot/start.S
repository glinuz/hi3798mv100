/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#include <config.h>

/******************************************************************************/
#define STACK_TOP               0xFFFF0C00
#define BOOT_RUN_START_ADDR     0xFFFF0C00
/******************************************************************************/

.globl _start
_start:
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

.=0x40
__blank_zone_start:
	.incbin ".init.reg"
. = __blank_zone_start + 0x2000
__blank_zone_end:

.balignl 16,0xdeadbeef
__start:
	.word _start

.globl _blank_zone_start
_blank_zone_start:
	.word __blank_zone_start


.globl _blank_zone_end
_blank_zone_end:
	.word __blank_zone_end

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

_copy_abort_code:
	.word  copy_abort_code

/*
 * the actual reset code
 */
reset:
	/*
	 *  delay wait mcu switch BUS finish
	 *  prevent mcu and A9 compete BUS conflict
	 */
	ldr	r0, =0x100
delay_wait_mcu:
	sub	r0, #1
	cmp	r0, #0
	bne	delay_wait_mcu
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

bootstrap_check:
	/*
	 *  read system register REG_SC_GEN14
	 *  check if ziju flag
	 */
	ldr	r1, [r0, #REG_SC_GEN14]
	ldr	r2, =0x7a696a75          /* magic for "ziju" */
	cmp	r1, r2
	bne	normal_start_flow

bootstrap:
	mov	r1, sp                 /* save sp */
	str	r1, [r0, #REG_SC_GEN14]  /* clear ziju flag */

	/* init PLL/DDRC/pin mux/... */
	ldr	r0, _blank_zone_start
	ldr	r1, _TEXT_BASE
	sub	r0, r0, r1
	add	r0, r0, #RAM_START_ADRS
	mov	r1, #0x0                /* flags: 0->normal 1->pm */
	mov	r2, #INIT_REG_ITEM1
	bl	init_registers           /* init PLL/DDRC/... */

	ldr	r0, =REG_BASE_SCTL
	ldr	r1, [r0, #REG_SC_GEN14]

	mov	sp, r1                   /* restore sp */
	ldr	r1, [r0, #REG_SC_GEN15]
	mov	pc, r1                   /* return to bootrom */

normal_start_flow:
	/* set the cpu to SVC32 mode */
	mrs	r0, cpsr
	bic	r0, r0, #0x1f  /* r0 = ((~0x1F) & r0) */
	orr	r0, r0, #0xd3  /* r0 = (0xd3 | r0)    */
	msr	cpsr, r0

	/* Enable SMP mode for CPU0, by setting bit 6 of Auxiliary Ctl reg */
	mrc	p15, 0, r0, c1, c0, 1
	orr	r0, #(1 << 6)
	mcr	p15, 0, r0, c1, c0, 1

	bl	flash_cache_all

main_cpu:
	/* init serial and printf a string. */
	bl	uart_early_init
	bl	msg_main_cpu_startup

check_boot_type:
	/* check_boot_type */
	ldr	r0, =REG_BASE_PERI_CTRL
	ldr	r0, [r0, #REG_START_MODE]
	mov	r6, r0, lsr#9             /* r6 = (r0 >> 9) */
	and	r6, #0x3

	/*
	 * REG_SC_GEN14: save start addr
	 * REG_SC_GEN15: save run flag
	 */
	ldr	r4, =REG_BASE_SCTL

	/* all flash start from sram (base + 0xc00) */
	ldr	r2, =BOOT_RUN_START_ADDR
	str	r2, [r4, #REG_SC_GEN14]

	cmp	r6, #BOOT_FROM_NAND
	bne	check_from_spi
#ifdef CONFIG_GENERIC_NAND
check_from_nand:
	mov	r2, #BOOT_FROM_NAND
	str	r2, [r4, #REG_SC_GEN15]
	b	check_start_mode
#endif /* CONFIG_GENERIC_NAND */

check_from_spi:
	cmp	r6, #BOOT_FROM_SPI
	bne	check_from_emmc
#ifdef CONFIG_GENERIC_SF
	mov	r2, #BOOT_FROM_SPI
	str	r2, [r4, #REG_SC_GEN15]
	b	check_start_mode
#endif /* CONFIG_GENERIC_SF */
check_from_emmc:
	cmp	r6, #BOOT_FROM_EMMC
	bne	check_from_sd
#ifdef CONFIG_GENERIC_MMC
	ldr	r2, =(BOOT_RUN_START_ADDR + CONFIG_MMC_BOOT_ADDR)
	str	r2, [r4, #REG_SC_GEN14]
	mov	r2, #BOOT_FROM_EMMC
	str	r2, [r4, #REG_SC_GEN15]
	b	check_start_mode
#endif
check_from_sd:
	cmp	r6, #BOOT_FROM_SD
	bne	bad_start_type
#ifdef CONFIG_GENERIC_MMC
	ldr	r2, =(BOOT_RUN_START_ADDR + CONFIG_MMC_BOOT_ADDR)
	str	r2, [r4, #REG_SC_GEN14]
	mov	r2, #BOOT_FROM_SD
	str	r2, [r4, #REG_SC_GEN15]
	b	check_start_mode
#endif
bad_start_type:
	bl	msg_bad_start_type
check_start_mode:
	

	/* Check if I need jump to flash */
	mov	r0, pc, lsr#28             /* r0 = (pc >> 28) */
	cmp	r0, #0xF

	/* boot from flash, I need jump after ddr_init,not here!*/
	beq	do_clr_remap

check_ddr_run:
	mov	r2, #BOOT_FROM_DDR
	str	r2, [r4, #REG_SC_GEN15]

do_clr_remap:
	ldr	r0, [r4, #REG_SC_CTRL]  /* r4 = 0xF800_0000 */

	/* Set clear remap bit */
	orr	r0, #(1<<8)             /* r0 = (r0 | (1 << 8)) */
	str	r0, [r4, #REG_SC_CTRL]  /* p156 */
	ldr	sp, =STACK_TOP

	/* enable I-Cache now */
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #0x00001000     /* set bit 12 (I) I-Cache */
	mcr	p15, 0, r0, c1, c0, 0

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

	/* check if pmoc resume */
	ldr	r1, [r4, #REG_SC_GEN27]
	ldr	r2, =0x66031013          /* magic for pm */
	cmp	r1, r2
	bne	un_resume_start

resume_start:

	mov	r1, #0                   /* clear pm flag */
	str	r1, [r4, #REG_SC_GEN27]

	/* disable mcu */
	ldr	r0, =0xf840f000
	ldr	r1, [r0]
	bic	r1, r1, #0x1
	str	r1, [r0]

	/* disable timer0 */
	ldr	r0, =REG_BASE_TIMER01             /* r0 = 0x101e_2000 */
	mov	r1, #0
	str	r1, [r0, #REG_TIMER_CONTROL]

	/* disable WDG_RST */
	mov	r1, #0
	str	r1, [r4, #REG_SC_WDG_RST_CTRL]

	ldr	r0, _blank_zone_start
	ldr	r1, _TEXT_BASE
	sub	r0, r0, r1
	ldr	r1, [r4, #REG_SC_GEN14]
	add	r0, r0, r1
	mov	r1, #1                   /* flags: 0->normal 1->pm */
	mov	r2, #INIT_REG_ITEM1_ITEM2
	bl	init_registers

	/* Restore DDRPHY Register */
	mov	r4, #4
	ldr	r1, =0xf840e100
	ldr	r3, =0xf8a38210

copy_loop:
	sub	r4, r4, #1
	mov	r0, #20

restore_ddr_phy_reg1:
	sub	r0, r0, #1
	ldr	r2, [r1], #4
	str	r2, [r3], #4
	cmp	r0, #0
	bne	restore_ddr_phy_reg1

	add	r3, r3, #0x30
	cmp	r4, #0
	bne	copy_loop

	ldr	r0, =0xf8a38000
	ldr	r1, [r0, #0x70]
	orr	r1, r1, #0x100000
	str	r1, [r0, #0x70]
	bic	r1, r1, #0x100000
	str	r1, [r0, #0x70]
	mov	r1, #0x8000
	str	r1, [r0, #0x04]
	mov	r1, #0
	str	r1, [r0, #0x04]

	/* slave cpu init by kernel. */
	ldr	r4, =REG_BASE_SCTL
	ldr	r0, [r4, #REG_SC_GEN30] /* goto the kernel */
	mov	pc, r0

	/* Check if I'm running in ddr */
un_resume_start:
	ldr	r0, [r4, #REG_SC_GEN15]
	cmp	r0, #BOOT_FROM_DDR
	beq	relocate

normal_init_item1:
	ldr	r0, _blank_zone_start
	ldr	r1, _TEXT_BASE
	sub	r0, r0, r1
	ldr	r1, [r4, #REG_SC_GEN14]
	add	r0, r0, r1
	mov	r1, #0x0                 /* flags: 0->normal 1->pm */
	mov	r2, #INIT_REG_ITEM1
	bl	init_registers
	b	normal_init_item2

relocate:                            /* relocate U-Boot to RAM */
	mov	r2, pc
	sub	r2, r2, #8       /* Get the ddr address of relocate label */
	ldr	r1, _armboot_start
	ldr	r0, _relocate
	sub	r1, r0, r1
	sub	r0, r2, r1       /* Get the ddr address where the fastboot loaded */
	str	r0, [r4, #REG_SC_GEN14]

normal_init_item2:
	/* in any case , we will init item2 here */
	ldr	r0, _blank_zone_start
	ldr	r1, _TEXT_BASE
	sub	r0, r0, r1
	ldr	r1, [r4, #REG_SC_GEN14]
	add	r0, r0, r1
#ifdef CONFIG_SHOW_REG_INFO
	bl	display_register_info
#endif /* CONFIG_SHOW_REG_INFO */
	mov	r1, #0x0              /* flags: 0->normal 1->pm */
	mov	r2, #INIT_REG_ITEM2   /* only init item2 */
	bl	init_registers

check_boot_mode:
	ldr	r4, =REG_BASE_SCTL
	ldr	r0, [r4, #REG_SC_GEN15]
	cmp	r0, #BOOT_FROM_EMMC
	beq	emmc_boot
	cmp	r0, #BOOT_FROM_SD
	beq	sd_boot

	b	copy_to_ddr

emmc_boot:
#ifdef CONFIG_GENERIC_MMC
	ldr	r0, _TEXT_BASE
	ldr	r1, _bss_start
	sub	r1, r1, r0
	bl	emmc_boot_read
	b	jump_to_ddr
#endif
sd_boot:
#ifdef CONFIG_GENERIC_MMC
	ldr	r0, _TEXT_BASE
	ldr	r1, _bss_start
	sub	r1, r1, r0
	bl	sd_boot_read
	b	jump_to_ddr
#endif

copy_to_ddr:
	/* relocate Boot to DDR */
	bl	display_relocate_to_ddr
#ifdef CONFIG_GENERIC_NAND
	cmp	r0, #BOOT_FROM_NAND
	ldreq	r1, =MEM_BASE_NAND      /* 0xFE00_0000 */
	streq	r1, [r4, #REG_SC_GEN14]
#endif

#ifdef CONFIG_GENERIC_SF
	cmp	r0, #BOOT_FROM_SPI
	ldreq	r1, =MEM_BASE_SPI       /* 0xFA00_0000 */
	streq	r1, [r4, #REG_SC_GEN14]
#endif

	ldr	r0, [r4, #REG_SC_GEN14]

	/* now, r0 stores __reset offset from where we get started */
	ldr	r1, _TEXT_BASE   /* r1 stores where we will copy uboot to */

	/* compare source and target address, if equal no copy to target address */
	cmp	r0, r1
	beq	copy_abort_code

	ldr	r2, _armboot_start
	ldr	r3, _bss_start
	sub	r2, r3, r2      /* r2 <- size of armboot */
	/* memcpy(r1, r0, r2) */
	bl	memcpy

jump_to_ddr:
	ldr	r4, =REG_BASE_SCTL
	ldr	r0, _TEXT_BASE
	str	r0, [r4, #REG_SC_GEN14]
	ldr	pc, _copy_abort_code

copy_abort_code:

	ldr	r4, =REG_BASE_SCTL
	ldr	r0, [r4, #REG_SC_GEN14]
	ldr	r1, =0xFFFF0000
	mov	r2, #0x4000

	/* memcpy(r1, r0, r2) */
	bl	memcpy

	/* Set up the stack */
stack_setup:
	ldr	r0, _TEXT_BASE                      @ upper 128 KiB: relocated uboot
	sub	r0, r0, #CONFIG_BOOTHEAD_GAP        @ boot head gap
	sub	sp, r0, #CONFIG_SYS_MALLOC_LEN      @ malloc area
	and	sp, sp, #~7              @ 8 byte alinged for (ldr/str)d

	/* Clear BSS (if any). Is below tx (watch load addr - need space) */
clear_bss:
	ldr	r0, _bss_start      @ find start of bss segment
	ldr	r1, _bss_end        @ stop here
	mov	r2, #0x00000000     @ clear value

clear_bss_loop:
	str	r2, [r0]        @ clear BSS location
	cmp	r0, r1          @ are we at the end yet
	add	r0, r0, #4      @ increment clear index pointer
	bne	clear_bss_loop  @ keep clearing till at end

	mov	r0, #0
	/* Instruction sync and invalidate I-cache */
	mcr	p15, 0, r0, c7, c5, 4
	mcr	p15, 0, r0, c7, c5, 0
	mcr	p15, 0, r0, c7, c5, 6

	bl	display_jump_to_ddr
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
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align	2
display_relocate_to_ddr:
	push	{r0, lr}
	add	r0, pc, #4
	bl	uart_early_puts
	pop	{r0, pc}
#ifdef CONFIG_PRINT
	.ascii "\r\n\r\nRelocate Boot to DDR\0"
#else
	.ascii "\0"
#endif

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align	2
display_jump_to_ddr:
	push	{r0, lr}
	add	r0, pc, #4
	bl	uart_early_puts
	pop	{r0, pc}
#ifdef CONFIG_PRINT
	.ascii "\r\n\r\nJump to DDR\r\n\0"
#else
	.ascii "\0"
#endif

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align	2
msg_main_cpu_startup:
	mov	r5, lr
	add	r0, pc, #4
	bl	uart_early_puts
	mov	pc, r5
L10:
#ifdef CONFIG_PRINT
	.ascii "\r\n\r\nSystem startup\r\n\0"
#else
	.ascii "\0"
#endif

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align	2
msg_bad_start_type:
	add	r0, pc, #4
	bl	uart_early_puts
	b	reset
L30:
#ifdef CONFIG_PRINT
	.ascii "*** Not support current start mode\r\n\0"
#else
	.ascii "\0"
#endif
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#ifdef CONFIG_SHOW_REG_INFO
@
@  int display_regv110_info(unsigned int base);
@
.align	2
.global	display_regv110_info
.type	display_regv110_info, %function
display_regv110_info:
	push	{r0, r4, lr}
	mov	r4, r0
	add	r0, pc, #4
	bl	uart_early_puts
	b	display_regv110_info_L0
display_regv110_info_str0:
#ifdef CONFIG_PRINT
	.ascii "\r\nReg Version:  v1.1.0" "\r\nReg Time:     \0"
#else
	.ascii "\0"
#endif
.align 2
display_regv110_info_L0:
	add	r0, r4, #0x40
	bl	uart_early_puts
	add	r0, pc, #4
	bl	uart_early_puts
	b	display_regv110_info_L1
display_regv110_info_str1:
#ifdef CONFIG_PRINT
	.ascii "\r\nReg Name:     \0"
#else
	.ascii "\0"
#endif
.align 2
display_regv110_info_L1:
	add	r0, r4, #0x54
	bl	uart_early_puts
	pop	{r0, r4, pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@  int display_regunknow_info(unsigned int base);
@
.align	2
.global	display_regunknow_info
.type	display_regunknow_info, %function
display_regunknow_info:
	push	{r0, lr}
	add	r0, pc, #4
	bl	uart_early_puts
	b	display_regunknow_info_L0
display_regunknow_info_str0:
#ifdef CONFIG_PRINT
	.ascii "Unknow register table\r\n\0"
#else
	.ascii "\0"
#endif
.align 2
display_regunknow_info_L0:
	pop	{r0, pc}

#endif /* CONFIG_SHOW_REG_INFO */
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@  void emmc_timeout_print(int num);
@
.align	2
.global	emmc_timeout_print
.type	emmc_timeout_print, %function
emmc_timeout_print:
	push	{r0, lr}
#ifdef CONFIG_PRINT
	bl	uart_early_put_hex
#endif
	add	r0, pc, #4
	bl	uart_early_puts
	pop		{r0, pc}
display_timeout:
#ifdef CONFIG_PRINT
	.ascii "\r\nemmc: timeout\r\n\0"
#else
	.ascii "\0"
#endif
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align	2
.globl _text_end
_text_end: .word __text_end


