/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/system.h>
#include <asm/sizes.h>
#include <asm/arch/platform.h>

extern void mmu_pagedir_init(unsigned int);
extern void mmu_pagedir_cached_range(unsigned int, unsigned int, unsigned int);
extern void mmu_startup(unsigned int);
extern void dcache_start(void);
extern void dcache_stop(void);

static unsigned int transform_table_base = 1;

int mmu_init(unsigned int ttb, unsigned int ddr_start, unsigned int ddr_size)
{
	if (ttb < ddr_start || ttb > (ddr_start + ddr_size)) {
		DBG_BUG(("transform table base address(0x%08X)"
		         " out of ddr range[0x%08X, 0x%08X].\n",
		         ttb, ddr_start, (ddr_start + ddr_size)));
		return 0;
	}
	transform_table_base = ttb;

	/* init page dir table in main mem 0- 16k */
	mmu_pagedir_init(transform_table_base);

	/* init cache page flag */
	mmu_pagedir_cached_range(transform_table_base, ddr_start, ddr_size);

	/* page table is 16K */
	return (0x4000);
}

/*
 * the parameter 'ddrsize' don't care, it is only used for compatibility the
 * old source.
 */
void dcache_enable(uint32_t unused)
{
	if (transform_table_base == 1) {
		DBG_BUG(("mmu has not be initialized.\n"));
		return;
	}
	/* enable mmu */
	mmu_startup(transform_table_base);
	dcache_start();
}

void dcache_disable(void)
{
	dcache_stop();
}
