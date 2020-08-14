@/******************************************************************************
@ *    Copyright (c) 2009-2016 by Hisi.
@ *    All rights reserved.
@ * ***
@ *    Create by Lyj. 2014-05-13
@ *
@******************************************************************************/

#include <config.h>

/* exception handlers */
#define _MODE_SVC              0x13
#define _REG_SPACE             72

_irq_stack_start:
	.word TEXT_BASE

.macro irq_save_to_stack
	ldr	sp, _irq_stack_start
	str	lr, [sp, #-4]
	mrs	lr, spsr
	str	lr, [sp, #-8]
	mov	lr, #_MODE_SVC
	msr	spsr, lr
	mov	lr, pc
	movs	pc, lr
.endm

.macro irq_save_regs_to_stack
	ldr	sp, _irq_stack_start
	sub	sp, sp, #_REG_SPACE
	stm	sp, {r0 - r12}
	ldr	r2, _irq_stack_start
	sub	r2, r2, #8
	ldmia	r2, {r2 - r3}          /* r2 = lr; r3 = spsr */
	mov	r0, sp                 /* r0 = register store start address */
	add	r5, sp, #52
	mov	r1, lr
	stm	r5, {r0 - r3}
.endm

.macro get_bad_stack_swi
	sub	sp, sp, #4
	str	r0, [sp]
	ldr	r0, _irq_stack_start
	str	lr, [r0]
	mrs	r0, spsr
	str	lr, [r0, #4]
	ldr	r0, [sp]
	add	sp, sp, #4
.endm

.align	5
.global	irq_undefined_instruction
.type	irq_undefined_instruction, %function
irq_undefined_instruction:
	irq_save_to_stack
	irq_save_regs_to_stack
	stmdb	sp!, {r0 - r3}
	add	r0, pc, #8
	bl	uart_early_puts
	ldmia	sp!, {r0 - r3}
	bl	do_undefined_instruction
str_undefined_instruction:
#ifndef CONFIG_SUPPORT_CA_RELEASE
	.ascii "\r\n*** irq: undefined instruction\r\n\0"
#else
	.ascii "\r\n***\r\n\0"
#endif

.align	5
.global	irq_software_interrupt
.type	irq_software_interrupt, %function
irq_software_interrupt:
	get_bad_stack_swi
	irq_save_regs_to_stack
	stmdb	sp!, {r0 - r3}
	add	r0, pc, #8
	bl	uart_early_puts
	ldmia	sp!, {r0 - r3}
	bl	do_software_interrupt
str_software_interrupt:
#ifndef CONFIG_SUPPORT_CA_RELEASE
	.ascii "\r\n*** irq: software interrupt\r\n\0"
#else
	.ascii "\r\n***\r\n\0"
#endif

.align	5
.global	irq_prefetch_abort
.type	irq_prefetch_abort, %function
irq_prefetch_abort:
	irq_save_to_stack
	irq_save_regs_to_stack
	stmdb	sp!, {r0 - r3}
	add	r0, pc, #8
	bl	uart_early_puts
	ldmia	sp!, {r0 - r3}
	bl	do_prefetch_abort
str_prefetch_abort:
#ifndef CONFIG_SUPPORT_CA_RELEASE
	.ascii "\r\n*** irq: prefetch abort\r\n\0"
#else
	.ascii "\r\n***\r\n\0"
#endif

.align	5
.global	irq_data_abort
.type	irq_data_abort, %function
irq_data_abort:
	irq_save_to_stack
	irq_save_regs_to_stack
	stmdb	sp!, {r0 - r3}
	add	r0, pc, #8
	bl	uart_early_puts
	ldmia	sp!, {r0 - r3}
	bl	do_data_abort
str_data_abort:
#ifndef CONFIG_SUPPORT_CA_RELEASE
	.ascii "\r\n*** irq: data abort\r\n\0"
#else
	.ascii "\r\n***\r\n\0"
#endif

.align	5
.global	irq_not_used
.type	irq_not_used, %function
irq_not_used:
	irq_save_to_stack
	irq_save_regs_to_stack
	stmdb	sp!, {r0 - r3}
	add	r0, pc, #8
	bl	uart_early_puts
	ldmia	sp!, {r0 - r3}
	bl	do_not_used
str_not_used:
#ifndef CONFIG_SUPPORT_CA_RELEASE
	.ascii "\r\n*** irq: not used\r\n\0"
#else
	.ascii "\r\n***\r\n\0"
#endif

.align	5
.global	irq_irq
.type	irq_irq, %function
irq_irq:
	irq_save_to_stack
	irq_save_regs_to_stack
	stmdb	sp!, {r0 - r3}
	add	r0, pc, #8
	bl	uart_early_puts
	ldmia	sp!, {r0 - r3}
	bl	do_irq
str_irq:
#ifndef CONFIG_SUPPORT_CA_RELEASE
	.ascii "\r\n*** irq: interrupt request\r\n\0"
#else
	.ascii "\r\n***\r\n\0"
#endif

.align	5
.global	irq_firq
.type	irq_firq, %function
irq_firq:
	irq_save_to_stack
	irq_save_regs_to_stack
	stmdb	sp!, {r0 - r3}
	add	r0, pc, #8
	bl	uart_early_puts
	ldmia	sp!, {r0 - r3}
	bl	do_fiq
str_fip:
#ifndef CONFIG_SUPPORT_CA_RELEASE
	.ascii "\r\n*** irq: fast interrupt request\r\n\0"
#else
	.ascii "\r\n***\r\n\0"
#endif

