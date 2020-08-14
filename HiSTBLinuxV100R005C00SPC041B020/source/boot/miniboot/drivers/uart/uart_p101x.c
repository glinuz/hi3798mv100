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

#include <stdio.h>
#include <compile.h>
#include <config.h>
#include <boot.h>
#include <module.h>
#include <console.h>
#include <irq.h>

#include <asm/io.h>

/*****************************************************************************/

#define UART_DR                                  0x00
#define UART_RSR                                 0x04
#define UART_FR                                  0x18
#define UART_FR_TXFF                  (1 << 5)
#define UART_RF_RXFE                  (1 << 4)

#define UART_IBRD                                0x24
#define UART_FBRD                                0x28
#define UART_LCR_H                               0x2C
#define UART_LCR_H_8BIT                (3 << 5)
#define UART_LCR_H_FIFO_EN             (1 << 4)

#define UART_CR                                  0x30
#define UART_CR_EN                     (1 << 0)
#define UART_CR_SEND_EN                (1 << 8)
#define UART_CR_REV_EN                 (1 << 9)

/*****************************************************************************/

static int uart_base = CONFIG_CUR_UART_BASE;
/*****************************************************************************/

static void uart_putc(int ch)
{
	while (readl(uart_base + UART_FR) & UART_FR_TXFF)
		;

	writel((ch & 0xFF), (uart_base + UART_DR));
}
/*****************************************************************************/

static int uart_getc(void)
{
	int nodata;
	uint32 data;
	int irqflags;

	do {
		irqflags = cpu_irq_save();
		nodata = readl(uart_base + UART_FR) & UART_RF_RXFE;
		cpu_irq_restore(irqflags);
	} while (nodata);

	irqflags = cpu_irq_save();
	data = readl(uart_base + UART_DR);
	if (data & ~0xFF) {
		writel(0xFFFFFFFF, (uart_base + UART_RSR));
		cpu_irq_restore(irqflags);
		return -1;
	}

	cpu_irq_restore(irqflags);
	return (int)data;
}
/*****************************************************************************/

static int uart_tstc(void)
{
	int irqflags = cpu_irq_save();
	int ret = !(readl(uart_base + UART_FR) & UART_RF_RXFE);
	cpu_irq_restore(irqflags);

	return ret;
}
/*****************************************************************************/

static struct console console_uart = {
	.getchar = uart_getc,
	.putchar = uart_putc,
	.tstchar = uart_tstc,
};
/*****************************************************************************/

void uart_init(void *ptr)
{
	uint32 tmp;
	uint32 ibrd, fbrd;

	tmp = (CONFIG_UART_BAUD << 4);
	ibrd = CONFIG_UART_CLK / tmp;
	tmp = ((CONFIG_UART_CLK % tmp) << 3) / CONFIG_UART_BAUD;
	fbrd = (tmp >> 1) + (tmp & 1);

	writel(0, (uart_base + UART_CR));

	writel(ibrd, (uart_base + UART_IBRD));
	writel(fbrd, (uart_base + UART_FBRD));

	writel((UART_LCR_H_8BIT | UART_LCR_H_FIFO_EN),
		(uart_base + UART_LCR_H));

	writel((UART_CR_EN | UART_CR_SEND_EN | UART_CR_REV_EN),
		(uart_base + UART_CR));

	console_init(&console_uart);

	puts("Serial initialize finish\n");
}
