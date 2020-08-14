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
@******************************************************************************
@
@  void uart_early_init(void);
@
.text
.align	2
.global	uart_early_init
.type	uart_early_init, %function
uart_early_init:
	ldr	a4, uart_base_addr_L0
	mov	a3, #0
	/* Disable UART */
	str	a3, [a4, #48] 
	/* Set baud rate to 115200, uart clock:fpga-54M, asic-86M */
	add	a3, a3, #46
	str	a3, [a4, #36]
	mov	a3, #32
	str	a3, [a4, #40]
	/* Set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled. */
	movw	a3, #112
	str	a3, [a4, #44]
	/* Enable UART */
	movw	a3, #769
	str	a3, [a4, #48]
	bx	lr
uart_base_addr_L0:
	.word CONFIG_CUR_UART_BASE

@******************************************************************************
@
@  void uart_early_puts(const char *ss);
@
.align	2
.global	uart_early_puts
.type	uart_early_puts, %function
uart_early_puts:
#if defined(CONFIG_PRINT)
	ldr	a2, uart_base_addr_L1
	b	next_char
output:
	ldr	a4, [a2, #24]
	tst	a4, #32
	bne	output
	str	a3, [a2, #0]
	add	a1, a1, #1
next_char:
	ldrb	a3, [a1]
	cmp	a3, #0
	bne	output
#endif /* CONFIG_PRINT */
	bx	lr
uart_base_addr_L1:
	.word CONFIG_CUR_UART_BASE

@******************************************************************************
@
@  void uart_early_put_hex(int hex);
@
@  call example:
@    mov	r0, sp
@    bl	uart_early_put_hex
@
.align	2
.global	uart_early_put_hex
.type	uart_early_put_hex, %function
uart_early_put_hex:
#if defined(CONFIG_PRINT)
	ldr	a2, uart_base_addr_L2
	mov	a3, #28
wait2:
	ldr	a4, [a2, #24]
	tst	a4, #32
	bne	wait2

	mov	a4, #0xF
	and	a4, a4, a1, lsr a3
	cmp	a4, #9
	addle	a4, a4, #0x30	@ a4 = a4 + '0'
	addgt	a4, a4, #55	@ a4 = a4 - 10 + 'A'
	str	a4, [a2, #0]
	cmp	a3, #0
	beq	exit2
	sub	a3, a3, #4
	b	wait2
exit2:
#endif /* CONFIG_PRINT */
	bx	lr
uart_base_addr_L2:
	.word CONFIG_CUR_UART_BASE

@******************************************************************************
@
@  void uart_early_putc(int chr);
@
@  call example:
@    mov	r0, #'A'
@    bl	uart_early_putc
@
.align	2
.global	uart_early_putc
.type	uart_early_putc, %function
uart_early_putc:
#if defined(CONFIG_PRINT)
	ldr	a2, uart_base_addr_L3
wait3:
	ldr	a4, [a2, #24]
	tst	a4, #32
	bne	wait3
	str	a1, [a2, #0]

#endif /* CONFIG_PRINT */
	bx	lr
uart_base_addr_L3:
	.word CONFIG_CUR_UART_BASE

