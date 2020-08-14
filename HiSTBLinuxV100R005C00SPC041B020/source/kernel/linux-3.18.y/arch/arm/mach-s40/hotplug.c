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

#include <linux/kernel.h>
#include <linux/smp.h>
#include <linux/completion.h>
#include <mach/hardware.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/smp_plat.h>
#include <linux/hikapi.h>

/*****************************************************************************/

static inline void s40_scu_power_off(int cpu)
{

	for (;;) {
		smp_mb();
		/*
		 * here's the WFI
		 */
		asm(".word	0xe320f003\n"
		    :
		    :
		    : "memory", "cc");

		if (pen_release == cpu_logical_map(cpu)) {
			/*
			 * OK, proper wakeup, we're done
			 */
			break;
		}
	}
}
/******************************************************************************/

static inline void s5_scu_power_off(int cpu)
{
	for (;;) {
		smp_mb();
		/*
		 * here's the WFI
		 */
		asm(".word	0xe320f003\n"
		    :
		    :
		    : "memory", "cc");

		if (pen_release == cpu_logical_map(cpu)) {
			/*
			 * OK, proper wakeup, we're done
			 */
			break;
		}
	}

}
/*****************************************************************************/

void s40_cpu_die(unsigned int cpu)
{
	u64 chipid = get_chipid(0ULL);

	flush_cache_all();

	if (chipid == _HI3798CV100A ||
	    chipid == _HI3798CV100 ||
	    chipid == _HI3796CV100) {
		s5_scu_power_off(cpu);
	} else {
		s40_scu_power_off(cpu);
	}
}
/*****************************************************************************/
/*
 * copy startup code to sram, and flash cache.
 * @start_addr: slave start phy address
 * @jump_addr: slave jump phy address
 */
void set_scu_boot_addr(unsigned int start_addr, unsigned int jump_addr)
{
	unsigned int *virtaddr;
	unsigned int *p_virtaddr;

	p_virtaddr = virtaddr = ioremap(start_addr, PAGE_SIZE);

	*p_virtaddr++ = 0xe51ff004; /* ldr  pc, [pc, #-4] */
	*p_virtaddr++ = jump_addr;  /* pc jump phy address */

	smp_wmb();
	__cpuc_flush_dcache_area((void *)virtaddr,
		(size_t)((char *)p_virtaddr - (char *)virtaddr));
	outer_clean_range(__pa(virtaddr), __pa(p_virtaddr));

	iounmap(virtaddr);
}
/*****************************************************************************/

void s40_scu_power_up(int cpu)
{
	unsigned int regval;
	static int init_flags = 0;

	if (!init_flags) {
		writel(0x3, __io_address(REG_PERI_PMC3));
		writel(0xffff88, __io_address(REG_PERI_PMC1));
		init_flags++;
	}

	regval = readl(__io_address(REG_BASE_PMC));
	/* a9_core1_pd_req=0, enable core1 power*/
	regval &= ~(1 << 3);
	/* a9_core1_wait_mtcoms_ack=0, no wait ack */
	regval &= ~(1 << 8);
	/* a9_core1_mtcmos_reg=1, core1 mtcoms power on */
	regval |= (1 << 0);
	writel(regval, __io_address(REG_BASE_PMC));

	/* clear the slave cpu reset */
	regval = readl(__io_address(A9_REG_BASE_RST));
	regval &= ~(1 << 17);
	writel(regval, __io_address(A9_REG_BASE_RST));
}
/******************************************************************************/

void s5_scu_power_up(int cpu)
{
	unsigned int regval;
	static int init_flags = 0;

	if (!init_flags) {
		writel(0x3, __io_address(REG_PERI_PMC3));
		writel(0xffff88, __io_address(REG_PERI_PMC1));
		init_flags++;
	}

	/* clear the slave cpu reset */
	regval = readl(__io_address(A9_REG_BASE_RST));
	if (regval & (1 << (12 + cpu))) {
		regval &= ~(1 << (12 + cpu));
		writel(regval, __io_address(A9_REG_BASE_RST));
	}
}

