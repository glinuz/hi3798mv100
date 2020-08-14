/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Cai Zhiyong
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

#include <compile.h>
#include <irq.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <irqchip.h>
#include <cache.h>
#include <cpu-arch.h>
#include <boot.h>
#include <trace.h>

#define MOULE_NAME "irq: "
#include <debug.h>

struct irq_handle_t {
	uint32 irqnr;
	void (*handle)(void *);
	void *arg;
};

static struct irq_handle_t __irq_handle[CONFIG_MAX_IRQ];
#define GET_IRQ(_irq) (&__irq_handle[_irq])

/*****************************************************************************/

int irq_connect(uint32 irqnr, void (*handle)(void *), void *arg)
{
	int irqflags;
	struct irq_handle_t *irq;

	if (irqnr >= CONFIG_MAX_IRQ)
		return -EINVAL;

	irq = GET_IRQ(irqnr);
	if (irq->handle)
		return -EEXIST;

	irqflags = cpu_irq_save();

	irq->irqnr = irqnr;
	irq->handle = handle;
	irq->arg = arg;

	irq_ctrl_enable_irq(irqnr);

	cpu_irq_restore(irqflags);

	return 0;
}
/*****************************************************************************/

void irq_handle(uint32 irqnr)
{
	struct irq_handle_t *irq;

	if (irqnr >= CONFIG_MAX_IRQ) {
		pr_warn("Receive invalid irq: %d\n", irqnr);
		return;
	}

	irq = GET_IRQ(irqnr);
	if (!irq->handle) {
		pr_warn("Irq %d nobody card\n", irqnr);
		return;
	}

	irq->handle(irq->arg);
}
/*****************************************************************************/

void __entry_irq_handle(void *base)
{
	irq_ctrl_handle();
}
/*****************************************************************************/

void __entry_exception_handle(uint32 base)
{
	int ix;
	char *ptr;
	uint32 entry;
	uint32 *Rn = (uint32 *)base;
	struct regs_t *regs = (struct regs_t *)base;
	char *vectors[] = {
		"Reset",
		"Undefined Instruction",
		"Software Interrupt",
		"Prefetch Abort",
		"Data Abort",
		"Unused",
		"Interrupt",
		"Fast interrupt",
		"Invalid",
	};

	if (regs->vector > ARRAY_SIZE(vectors))
		regs->vector = ARRAY_SIZE(vectors) - 1;
	printf("\nException: %s\n", vectors[regs->vector]);

	regs->pc -= 8;

	for (ix = 0; ix < 13; ix++) {
		printf("r%-2d: 0x%08X  ", ix, Rn[ix]);
		if ((ix & 0x03) == 0x03)
			printf("\n");
	}
	printf("\nsp:0x%08x  lr:0x%08x  pc:0x%08x  cpsr:0x%08x\n",
		regs->sp, regs->lr, regs->pc, regs->spsr);

	ptr = find_symbol(regs->pc, &entry, NULL, NULL);
	if (ptr)
		printf("PC is at %s(0x%08x)\n", ptr, entry);

	ptr = find_symbol(regs->lr, &entry, NULL, NULL);
	if (ptr)
		printf("LR is at %s(0x%08x)\n", ptr, entry);

	printf("\nSP: 0x%08x\n", regs->sp);
	dump_hex(regs->sp, (char *)regs->sp, 128, sizeof(int));

	printf("\nLR: 0x%08x\n", regs->lr);
	dump_hex(regs->lr, (char *)regs->lr, 128, sizeof(int));

	printf("\nPC: 0x%08x\n", regs->pc);
	dump_hex(regs->pc, (char *)regs->pc, 128, sizeof(int));

	printf("\nException Stack:\n");
	backtracking(Rn[11]);

	reboot();
}
/*****************************************************************************/

static void set_irq_vector(uint32 base)
{
	extern char __vector_start[];
	extern char __vector_end[];

	uint32 sctlr;
	char *from = __vector_start;
	char *to = (char *)base;

	ASSERT(base == 0x0 || base == 0xFFFF0000);

	*(uint32 *)to = *(uint32 *)from;
	memcpy(to + 4, from + 4, __vector_end - __vector_start - 4);

	sctlr = GET_SCTLR();
	if (base == 0x0)
		sctlr &= ~SCTLR_V;
	else
		sctlr |= SCTLR_V;
	SET_SCTLR(sctlr);

	invalidIcache();
}
/*****************************************************************************/

void irq_init(void)
{
	set_irq_stack((uint32)__irq_stack_end);
	set_irq_vector(CONFIG_VECTOR_BASE);
	irq_ctrl_init();
}
