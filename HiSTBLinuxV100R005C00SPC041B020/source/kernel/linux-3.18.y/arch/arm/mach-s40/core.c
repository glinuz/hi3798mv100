/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.6.13
 *
******************************************************************************/

#include <linux/init.h>
#include <asm/irq.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <mach/hardware.h>
#include <mach/early-debug.h>
#include <linux/irqchip/arm-gic.h>
#include <linux/delay.h>
#include <linux/memblock.h>
#include <linux/of_platform.h>

#include "platsmp.h"

#ifdef CONFIG_DMA_CMA
extern int hisi_declare_heap_memory(void);
#endif

/*****************************************************************************/

static struct map_desc s40_io_desc[] __initdata = {
	/* S40_IOCH1 */
	{
		.virtual	= S40_IOCH1_VIRT,
		.pfn		= __phys_to_pfn(S40_IOCH1_PHYS),
		.length		= S40_IOCH1_SIZE,
		.type		= MT_DEVICE
	},
	/* S40_IOCH2 */
	{
		.virtual        = S40_IOCH2_VIRT,
		.pfn            = __phys_to_pfn(S40_IOCH2_PHYS),
		.length         = S40_IOCH2_SIZE,
		.type           = MT_DEVICE
	},
};
/******************************************************************************/

static void __init s40_map_io(void)
{
	int i;

	iotable_init(s40_io_desc, ARRAY_SIZE(s40_io_desc));

	for (i = 0; i < ARRAY_SIZE(s40_io_desc); i++) {
		edb_putstr(" V: ");	edb_puthex(s40_io_desc[i].virtual);
		edb_putstr(" P: ");	edb_puthex(s40_io_desc[i].pfn);
		edb_putstr(" S: ");	edb_puthex(s40_io_desc[i].length);
		edb_putstr(" T: ");	edb_putul(s40_io_desc[i].type);
		edb_putstr("\n");
	}

	edb_trace();
}
/*****************************************************************************/

static void __init s40_reserve(void)
{
#ifdef CONFIG_SUPPORT_DSP_RUN_MEM
	/* Reserve memory for DSP */
	BUG_ON(memblock_reserve(CONFIG_DSP_RUN_MEM_ADDR,
		CONFIG_DSP_RUN_MEM_SIZE));

	printk(KERN_NOTICE "DSP run memory space at 0x%08X, size: 0x%08x Bytes.\n",
		CONFIG_DSP_RUN_MEM_ADDR,
		CONFIG_DSP_RUN_MEM_SIZE);
#endif

#ifdef CONFIG_DMA_CMA
	hisi_declare_heap_memory();
#endif
}
/******************************************************************************/

static void __init s40_init_early(void)
{
	/*
	 * 1. enable L1 prefetch                       [2]
	 * 4. enable allocation in one cache way only. [8]
	 */
	asm volatile (
	"	mrc	p15, 0, r0, c1, c0, 1\n"
	"	orr	r0, r0, #0x104\n"
	"	mcr	p15, 0, r0, c1, c0, 1\n"
	  :
	  :
	  : "r0", "cc");
}
/*****************************************************************************/

static void s40_restart(enum reboot_mode mode, const char *cmd)
{
	unsigned int val;

	printk(KERN_INFO "CPU will restart.");

	mdelay(200);

	local_irq_disable();
	/* enable the wdg0 crg clock */
	val = readl(__io_address(REG_BASE_CRG + REG_PERI_CRG94));
	val &= ~(1<<4);
	val |= 1;
	writel(val, __io_address(REG_BASE_CRG + REG_PERI_CRG94));
	udelay(1);

	/* unclock wdg */
	writel(0x1ACCE551,  __io_address(REG_BASE_WDG0 + 0xc00));
	/* wdg load value */
	writel(0x00000100,  __io_address(REG_BASE_WDG0 + 0x0));
	/* bit0: int enable bit1: reboot enable */
	writel(0x00000003,  __io_address(REG_BASE_WDG0 + 0x8));

	while (1);

	BUG();
}
/*****************************************************************************/
extern void __init s40_timer_init(void);

static const char * const s40_dt_board_compat[] = {
	"hi3716cv200-series",
	"hi3716mv410-series",
	NULL
};

DT_MACHINE_START(S40, "bigfish")
	.dt_compat    = s40_dt_board_compat,
	.atag_offset  = 0x100,
	.map_io       = s40_map_io,
	.init_early   = s40_init_early,
	.smp          = smp_ops(s40_smp_ops),
	.reserve      = s40_reserve,
	.restart      = s40_restart,
MACHINE_END
