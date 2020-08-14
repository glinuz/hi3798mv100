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

#include <stddef.h>
#include <early_uart.h>
#include <mmu.h>
#include <stdio.h>

#define SECTION_COUNT      4096
#define SECTION_SIZE       _1M
#define SECTION_SHIFT      20

extern int __cache_flush_all(void);
extern int __mmu_cache_disable(void);
extern int __mmu_cache_enable(unsigned int pagetable);

static unsigned int pagetable_entry = 1;
unsigned int cache_enabled = 0;

/*****************************************************************************/

int mmu_init(unsigned int pgt_offset)
{
	int ix = 0;
	uint32 addr = 0;
	uint32 *ptable = (uint32 *)pgt_offset;
	uint32 attrib = PAGE_RW | PAGE_DOMAIN | PAGE_SECTION | PAGE_XN;

	if (pagetable_entry != 1)
		return  pagetable_entry + (SECTION_COUNT * sizeof(int));

	pagetable_entry = (unsigned int)pgt_offset;

	while (ix++ < SECTION_COUNT) {
		*ptable++ = addr | attrib;
		addr += SECTION_SIZE;
	}

	return pagetable_entry + (SECTION_COUNT * sizeof(int));
}
/*****************************************************************************/

void mmu_set(uint32 start, uint32 size, uint32 attrib)
{
	uint32 *ptable = (uint32 *)pagetable_entry;

	if (start & (SECTION_SIZE - 1) || size & (SECTION_SIZE - 1)) {
		early_puts("mmu: page table should align to section size.\r\n");
		return;
	}

	start >>= SECTION_SHIFT;
	size >>= SECTION_SHIFT;
	ptable += start;

	while (size-- > 0)
		*ptable++ |= attrib;
}
/*****************************************************************************/

int mmu_cache_enable(void)
{
	if (pagetable_entry == 1) {
		early_puts("mmu: page table is not initialized.\r\n");
		return -1;
	}

	if (cache_enabled == 1)
		return 0;
	cache_enabled = 1;

	__mmu_cache_enable(pagetable_entry);

	return 0;
}
/*****************************************************************************/

int mmu_cache_disable(void)
{
	if (pagetable_entry == 1) {
		early_puts("mmu: page table is not initialized.\r\n");
		return -1;
	}

	if (cache_enabled == 0)
		return 0;
	cache_enabled = 0;

	__mmu_cache_disable();

	return 0;
}
/*****************************************************************************/

int cache_flush_all(void)
{
	if (pagetable_entry == 1) {
		early_puts("mmu: page table is not initialized.\r\n");
		return -1;
	}

	if (cache_enabled == 0)
		return 0;

	__cache_flush_all();

	return 0;
}
/*****************************************************************************/
