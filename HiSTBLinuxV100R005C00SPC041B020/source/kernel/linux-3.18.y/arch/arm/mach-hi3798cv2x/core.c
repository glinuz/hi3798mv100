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
#include <linux/hisilicon/himisc.h>

#include "platsmp.h"

#ifdef CONFIG_DMA_CMA
extern int hisi_declare_heap_memory(void);
#endif

void __iomem *s40_gic_cpu_base_addr = IOMEM(CFG_GIC_CPU_BASE);

/*****************************************************************************/

static void __maybe_unused __init hi3798cv2x_gic_init_irq(void)
{

	edb_trace();

	gic_init(0, HISI_GIC_IRQ_START, IOMEM(CFG_GIC_DIST_BASE),
		 IOMEM(CFG_GIC_CPU_BASE));

}
/*****************************************************************************/

static struct map_desc hi3798cv2x_io_desc[] __initdata = {
	/* HI3798CV2X_IOCH1 */
	{
		.virtual	= HI3798CV2X_IOCH1_VIRT,
		.pfn		= __phys_to_pfn(HI3798CV2X_IOCH1_PHYS),
		.length 	= HI3798CV2X_IOCH1_SIZE,
		.type		= MT_DEVICE
	},
};
/******************************************************************************/

static void __init hi3798cv2x_map_io(void)
{
	int i;

	iotable_init(hi3798cv2x_io_desc, ARRAY_SIZE(hi3798cv2x_io_desc));

	printk(KERN_DEBUG "-------------Fixed IO Mapping----------\n");
	printk(KERN_DEBUG "Virt,            Phys,             Size\n");
	for (i = 0; i < ARRAY_SIZE(hi3798cv2x_io_desc); i++) {
		printk(KERN_DEBUG "0x%08lX,    0x%08X,    0x%08lX\n",
			hi3798cv2x_io_desc[i].virtual,
			__pfn_to_phys(hi3798cv2x_io_desc[i].pfn),
			hi3798cv2x_io_desc[i].length);
		edb_putstr(" V: ");	edb_puthex(hi3798cv2x_io_desc[i].virtual);
		edb_putstr(" P: ");	edb_puthex(hi3798cv2x_io_desc[i].pfn);
		edb_putstr(" S: ");	edb_puthex(hi3798cv2x_io_desc[i].length);
		edb_putstr(" T: ");	edb_putul(hi3798cv2x_io_desc[i].type);
		edb_putstr("\n");
	}

	printk(KERN_DEBUG "--------------------------------------\n");

	edb_trace();
}
/*****************************************************************************/

static void __init hi3798cv2x_reserve(void)
{
	initfile_reserve_memory();

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

static void __init hi3798cv2x_init_early(void)
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

#ifdef CONFIG_HIMCIV200_SDIO_SYNOPSYS
static int hi3798cv2x_mci_quirk(void)
{
	void *base = __io_address(REG_BASE_SDIO1);
	unsigned int reg_data = 0;

	writel(0x1affe, base + 0x44);
	writel(0, base + 0x20);
	writel(0, base + 0x1c);
	writel(0, base + 0x28);

	writel(0xa000414c, base + 0x2c);

	mdelay(100);

	reg_data = readl(base + 0x00);
	reg_data |= 7;
	writel(reg_data, base + 0x00);

	writel(0, base + 0x78);
	writel(0, base + 0x04);
	mdelay(2);
	writel(1, base + 0x78);

	return 0;
}
#endif

static void hi3798cv2x_restart(enum reboot_mode mode, const char *cmd)
{
	printk(KERN_INFO "CPU will restart.");

	mdelay(200);

	local_irq_disable();

#ifdef CONFIG_HIMCIV200_SDIO_SYNOPSYS
	hi3798cv2x_mci_quirk();
#endif

	/* unclock wdg */
	writel(0x1ACCE551,  __io_address(REG_BASE_WDG0 + 0xc00));
	/* wdg load value */
	writel(0x00000100,  __io_address(REG_BASE_WDG0 + 0x0));
	/* bit0: int enable bit1: reboot enable */
	writel(0x00000003,  __io_address(REG_BASE_WDG0 + 0x8));

	while (1)
		;

	BUG();
}
/*****************************************************************************/

static const char * const hi3798cv2x_dt_board_compat[] = {
	"hi3798cv200-series",
	NULL
};

#ifdef CONFIG_USE_OF
DT_MACHINE_START(HI3798CV2X, "bigfish")
	.dt_compat    = hi3798cv2x_dt_board_compat,
#else
MACHINE_START(HI3798CV2X, "bigfish")
	.init_irq     = hi3798cv2x_gic_init_irq,
#endif
	.atag_offset  = 0x100,
	.map_io       = hi3798cv2x_map_io,
	.init_early   = hi3798cv2x_init_early,
	.smp          = smp_ops(hi3798cv2x_smp_ops),
	.reserve      = hi3798cv2x_reserve,
	.restart      = hi3798cv2x_restart,
MACHINE_END
