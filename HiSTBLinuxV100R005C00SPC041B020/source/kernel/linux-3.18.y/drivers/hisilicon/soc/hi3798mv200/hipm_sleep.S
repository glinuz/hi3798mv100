#include <mach/platform.h>
#include <mach/hardware.h>
#include <asm/memory.h>
#include <linux/linkage.h>
#include <mach/io.h>
#include <asm/assembler.h>

#define __virt_to_phys(x)   ((x) - PAGE_OFFSET + PLAT_PHYS_OFFSET)
#define __phys_to_virt(x)   ((x) - PLAT_PHYS_OFFSET + PAGE_OFFSET)


#define MCU_START_CTRL  0xf840f000	/* mcu start control */
#define OTP_IDWORD_ADDR 0xf8ab0128	/* OTP shadow register to indicate if it is advca chipset */
#define OTP_CA_ID_WORD  0x6EDBE953	/* ID_WORD for CA chipset */

.extern unsigned long hi_ca_ddrwakeupcheck_phys;

.macro D5_LED_ON, rd, rb
	ldr	\rb, =IO_ADDRESS(0xf8004010)
	ldr	\rd, =0x4
	str	\rd, [\rb]
.endm

.macro D5_LED_OFF, rd, rb
	ldr	\rb, =IO_ADDRESS(0xf8004010)
	ldr	\rd, =0x0
	str	\rd, [\rb]
.endm


.macro LED1_ON, rd, rb
	ldr	\rb, =IO_ADDRESS(0xf8004008)
	ldr	\rd, =0x2
	str	\rd, [\rb]
.endm

.macro LED1_OFF, rd, rb
	ldr	\rb, =IO_ADDRESS(0xf8004008)
	ldr	\rd, =0x0
	str	\rd, [\rb]
.endm

.macro LED_GPIO_INIT, rd, rb
	ldr	\rb, =IO_ADDRESS(0xf8004400)
	ldr	\rd, =0x6
	str	\rd, [\rb]
.endm

ENTRY(hi_pm_cpu_resume)
	safe_svcmode_maskall	r1

_serial_init:
	mov	r3, #0
	movt	r3, #0xf8b0
	mov	r2, #0
	str	r2, [r3, #48]

	add	r2, r2, #40
	str	r2, [r3, #36]
	mov	r2, #44
	str	r2, [r3, #40]

	movw	a3, #112
	str	r2, [r3, #44]
	movw	r2, #769
	str	r2, [r3, #48]

	/* printf Kernel Resuming... */
	mov	r4, #0x0
	str	r4, [r3]
	mov	r4, #0x0
	str	r4, [r3]
	mov	r4, #0x4B
	str	r4, [r3]
	mov	r4, #0x65
	str	r4, [r3]
	mov	r4, #0x72
	str	r4, [r3]
	mov	r4, #0x6E
	str	r4, [r3]
	mov	r4, #0x65
	str	r4, [r3]
	mov	r4, #0x6C
	str	r4, [r3]
	mov	r4, #0x0
	str	r4, [r3]
	mov	r4, #0x0
	str	r4, [r3]
	mov	r4, #0x52
	str	r4, [r3]
	mov	r4, #0x65
	str	r4, [r3]
	mov	r4, #0x73
	str	r4, [r3]
	mov	r4, #0x75
	str	r4, [r3]
	mov	r4, #0x6D
	str	r4, [r3]
	mov	r4, #0x65
	str	r4, [r3]
	mov	r4, #0x2E
	str	r4, [r3]
	mov	r4, #0x2E
	str	r4, [r3]
	mov	r4, #0x2E
	str	r4, [r3]
	/* \r\n */
	mov	r4, #0x0d
	str	r4, [r3]
	mov	r4, #0x0a
	str	r4, [r3]

	/* Set CPUECTLR.SMPEN */
	mrrc	p15, 1, r0, r2, c15
	orr	r0, #(1 << 6)
	mcrr	p15, 1, r0, r2, c15

	b	cpu_resume
ENDPROC(hi_pm_cpu_resume)

/*
 *-------------------------------------------------------------------------
 *   Function: hilpm_godpsleep
 *
 *   this function is the low level interface when deep sleep.
 *
 */
ENTRY (hi_pm_sleep)
store_current:
//	LED_GPIO_INIT r10, r11
//	D5_LED_OFF r10, r11
//	LED1_OFF r10, r11
//	D5_LED_ON r10, r11

	/* disable C bit*/
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #(1 << 2)		@ Disable the C bit
	mcr	p15, 0, r0, c1, c0, 0
	isb

	/*
	 * Flush all data from the L1 data cache before disabling
	 * SCTLR.C bit.
	 * Used Register: R0/R1/R2/R3/R4/R5/R7/R9/R10
	 */
	bl	v7_flush_dcache_all

	isb
	dsb

	/* SMP->AMP*/
	/* clear SMP mode for CPU0, by setting bit 6 of CPU Extend Ctl reg */
	mrrc	p15,1,r0,r1,c15
	bic	r0, r0, #(1 << 6)
	mcrr	p15,1,r0,r1,c15

	isb
	dsb

store_resume:
	ldr	r5, =(__virt_to_phys(hi_pm_cpu_resume))
	ldr	r4, =hi_sc_virtbase
	ldr	r4, [r4]

	/*Store resume address for non-advca chipset*/
	str	r5, [r4, #REG_SC_GEN30]
	str	r5, [r4, #REG_SC_GEN9]

	/* if it is not advca chipset, jump to enable_mcu */
	ldr	r1, =hi_otp_idword_addr
	ldr	r1, [r1]
	ldr	r2, [r1]
	ldr	r1, =OTP_CA_ID_WORD
	cmp	r1, r2
	bne	enable_mcu

	/* Wakeup process for advca chipset */
ca_pm_suspend:
	/* disable MMU stuff and caches */
	ldr	r2, =hi_ca_ddrwakeupcheck_phys
	ldr	lr, [r2]

	/* init UART */
	ldr	r3, =hi_uart_virtbase
	ldr	r3, [r3]
	mov	r2, #0
	str	r2, [r3, #48]

	add	r2, r2, #40
	str	r2, [r3, #36]
	mov	r2, #44
	str	r2, [r3, #40]

	movw	a3, #112
	str	r2, [r3, #44]
	movw	r2, #769
	str	r2, [r3, #48]

	/* printf enter CA */
	mov	r4, #0x0
	str	r4, [r3]
	mov	r4, #0x0
	str	r4, [r3]
	mov	r4, #0x65
	str	r4, [r3]
	mov	r4, #0x6E
	str	r4, [r3]
	mov	r4, #0x74
	str	r4, [r3]
	mov	r4, #0x65
	str	r4, [r3]
	mov	r4, #0x72
	str	r4, [r3]
	mov	r4, #0x20
	str	r4, [r3]
	mov	r4, #0x43
	str	r4, [r3]
	mov	r4, #0x41
	str	r4, [r3]
	/* \r\n */
	mov	r4, #0x0d
	str	r4, [r3]
	mov	r4, #0x0a
	str	r4, [r3]

	/* disable MMU */
	mrc p15, 0, r1, c1, c0, 0 
	bic r1, #1
	mcr p15, 0, r1, c1, c0, 0

	bx	lr

enable_mcu:
//	LED1_ON r10, r11
	ldr	r1, =hi_mcu_start_ctrl
	ldr	r1, [r1]
	ldr	r2, [r1]
	orr	r2, #0x1
	str	r2, [r1]
	/* When poweron enter standby directly, should set flag here to enter standby. */
	ldr	r1, =hi_sc_virtbase
	ldr	r1, [r1]
	ldr	r2, =0x12345678
	str	r2, [r1, #REG_SC_GEN13]

go_wfi:
	mov	r1, #0x10
loop_wfi:
	dsb
	isb
	wfi
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	ldr	r4, =hi_sc_virtbase
	ldr	r4, [r4]
	add	r1, #0x1
	str	r1, [r4, #REG_SC_GEN8]
	b	loop_wfi

	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
ENDPROC(hi_pm_sleep)

.section .data

.global hi_otp_idword_addr
hi_otp_idword_addr:
.word hi_otp_idword_addr

.global hi_mcu_start_ctrl
hi_mcu_start_ctrl:
.word hi_mcu_start_ctrl
