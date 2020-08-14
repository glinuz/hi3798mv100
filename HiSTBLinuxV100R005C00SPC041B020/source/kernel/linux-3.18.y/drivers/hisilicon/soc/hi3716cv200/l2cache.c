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

#define pr_fmt(fmt) "l2cache: " fmt

#include <linux/init.h>
#include <asm/cacheflush.h>
#include <asm/hardware/cache-l2x0.h>
#include <mach/hardware.h>
#include "l2cache.h"
#include <linux/hikapi.h>

static void __iomem *l2x0_viraddr = __io_address(REG_BASE_L2CACHE);

/*****************************************************************************/
#ifdef CONFIG_PM
/*
 *  hi_pm_disable_l2cache()/hi_pm_enable_l2cache() is designed to
 *  disable and enable l2-cache during Suspend-Resume phase
 */
void hi_pm_disable_l2cache(void)
{
	outer_disable();
}
/*****************************************************************************/

void hi_pm_enable_l2cache(void)
{
	outer_resume();
}
#endif /* CONFIG_PM */
/*****************************************************************************/

static int __init l2_cache_init(void)
{
	u32 val;
	u64 chipid = get_chipid(0ULL);

	/*
	 * Bits  Value Description
	 * [31]    0 : SBZ
	 * [30]    1 : Double linefill enable (L3)
	 * [29]    1 : Instruction prefetching enable
	 * [28]    1 : Data prefetching enabled
	 * [27]    0 : Double linefill on WRAP read enabled (L3)
	 * [26:25] 0 : SBZ
	 * [24]    1 : Prefetch drop enable (L3)
	 * [23]    0 : Incr double Linefill enable (L3)
	 * [22]    0 : SBZ
	 * [21]    0 : Not same ID on exclusive sequence enable (L3)
	 * [20:5]  0 : SBZ
	 * [4:0]   0 : use the Prefetch offset values 0.
	 */
	writel_relaxed(0x71000000, l2x0_viraddr + L310_PREFETCH_CTRL);

	val = __raw_readl(l2x0_viraddr + L2X0_AUX_CTRL);
	val |= (1 << 30); /* Early BRESP enabled */
	val |= (1 << 0);  /* Full Line of Zero Enable */
	writel_relaxed(val, l2x0_viraddr + L2X0_AUX_CTRL);

	if ((chipid == _HI3719MV100) || (chipid == _HI3718MV100)) {
		val = readl_relaxed(l2x0_viraddr + L310_TAG_LATENCY_CTRL);
		val &=0xfffff888;
		writel_relaxed(val, l2x0_viraddr + L310_TAG_LATENCY_CTRL);

		val = readl_relaxed(l2x0_viraddr + L310_DATA_LATENCY_CTRL);
		val &=0xfffff888;
		writel_relaxed(val, l2x0_viraddr + L310_DATA_LATENCY_CTRL);

		l2x0_init(l2x0_viraddr, 0x00440000, 0xFFB0FFFF);
	} else if (chipid == _HI3798CV100A ||
		   chipid == _HI3798CV100 ||
		   chipid == _HI3796CV100) {
		/* L2cache is 1M( 64KB * 16 Way = 1M ) bytes */
		l2x0_init(l2x0_viraddr, 0x00470000, 0xFFB0FFFF);
	} else if ((chipid == _HI3716MV410) || (chipid == _HI3716MV420)) {
		l2x0_init(l2x0_viraddr, 0x00440000, 0xFFB0FFFF);
	} else 
		l2x0_init(l2x0_viraddr, 0x00450000, 0xFFB0FFFF);

	/*
	 * 2. enable L2 prefetch hint                  [1]a
	 * 3. enable write full line of zeros mode.    [3]a
	 *   a: This feature must be enabled only when the slaves
	 *      connected on the Cortex-A9 AXI master port support it.
	 */
	asm volatile (
	"	mrc	p15, 0, r0, c1, c0, 1\n"
	"	orr	r0, r0, #0x02\n"
	"	mcr	p15, 0, r0, c1, c0, 1\n"
	  :
	  :
	  : "r0", "cc");

	return 0;
}
early_initcall(l2_cache_init);
