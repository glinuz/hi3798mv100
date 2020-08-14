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
#include <boot.h>
#include <config.h>
#include <compile.h>
#include <mmu.h>
#include <malloc.h>
#include <irq.h>
#include <task.h>
#include <string.h>

#define MOULE_NAME "meminit:"
#include <debug.h>

/*****************************************************************************/
/*
 *  +---------------------+-- DDR start
 *  | SVC stack           |
 *  +---------------------+
 *  | malloc              |
 *  +---------------------+
 *  | global              |
 *  +---------------------+
 *  | reserve for ca      |
 *  +---------------------+-- __text_start
 *  | .TEXT               |
 *  +-------+-------------+-- __text_end
 *  |       | other       |
 *  |       +-------------+
 *  |       | .BSS        |
 *  | .DATA +-------------+
 *  |       | page table  |
 *  |       +-------------+
 *  |       | IRQ stack   |
 *  +-------+-------------+-- __end
 *  | DMA malloc          |
 *  +---------------------+
 *  | free for kernel     |
 *  +---------------------+
 *  | reserve memory      |
 *  +---------------------+-- DDR end
 */

#define DDRLAYOUT_AUTO       (uint32)-1

struct ddrlayout_t {
	uint32 start;
	uint32 end;
	uint32 size;
	uint32 attribue;
	char *name;
	void (*init)(struct ddrlayout_t *ddrlayout);
};

char *__global_start = NULL;
char *__global_end = NULL;
/*****************************************************************************/
#if 0
static void ddrlayout_resmem_init(struct ddrlayout_t *ddr)
{
	resmalloc_init(ddr->start, ddr->size);
}
#endif
/*****************************************************************************/

static void ddrlayout_dma_init(struct ddrlayout_t *ddr)
{
	devmalloc_init(ddr->start, ddr->size);
}
/*****************************************************************************/

static void ddrlayout_malloc_init(struct ddrlayout_t *ddr)
{
	malloc_init(ddr->start, ddr->size);
}
/*****************************************************************************/

static void ddrlayout_resmem_init(struct ddrlayout_t *ddr)
{
	resmalloc_init(ddr->start, ddr->size);
}
/*****************************************************************************/

static void ddrlayout_global_init(struct ddrlayout_t *ddr)
{
	__global_start = (char *)ddr->start;
	__global_end = (char *)ddr->end;
}
/*****************************************************************************/

static struct ddrlayout_t ddrlayout[] = {
	{
		.start = MEM_BASE_DDR,
		.end = (uint32)__text_start - CONFIG_BOOTHEAD_GAP -
			CONFIG_SYS_MALLOC_LEN,
		.size = DDRLAYOUT_AUTO,
		.attribue = PAGE_B | PAGE_C,
		.name = "stack",
		.init = NULL,
	}, {
		.start = DDRLAYOUT_AUTO,
		.end = DDRLAYOUT_AUTO,
		.size = CONFIG_SYS_MALLOC_LEN - CONFIG_GLOBAL_DATA_SIZE,
		.attribue = PAGE_B | PAGE_C,
		.name = "malloc",
		.init = ddrlayout_malloc_init,
	}, {
		.start = DDRLAYOUT_AUTO,
		.end = DDRLAYOUT_AUTO,
		.size = CONFIG_GLOBAL_DATA_SIZE,
		.attribue = PAGE_B | PAGE_C,
		.name = "global",
		.init = ddrlayout_global_init,
	}, {
		.start = DDRLAYOUT_AUTO,
		.end = (uint32)__text_start,
		.size = DDRLAYOUT_AUTO,
		.attribue = PAGE_B | PAGE_C,
		.name = "reserve",
		.init = NULL,
	}, {
		.start = (uint32)__text_start,
		.end = (uint32)__text_end,
		.size = DDRLAYOUT_AUTO,
		.attribue = PAGE_B | PAGE_C,
		.name = ".text",
		.init = NULL,
	}, {
		.start = DDRLAYOUT_AUTO,
		.end = (uint32)__bss_end,
		.size = DDRLAYOUT_AUTO,
		.attribue = PAGE_B | PAGE_C,
		.name = ".data",
		.init = NULL,
	}, {
		.start = (uint32)__page_table_start,
		.end = (uint32)__page_table_end,
		.size = DDRLAYOUT_AUTO,
		.attribue = PAGE_B | PAGE_C,
		.name = "pagetable",
		.init = NULL,
	}, {
		.start = (uint32)__irq_stack_start,
		.end = (uint32)__irq_stack_end,
		.size = DDRLAYOUT_AUTO,
		.attribue = PAGE_B | PAGE_C,
		.name = "irqstack",
		.init = NULL,

	}, {
		.start = (uint32)__end,
		.end = DDRLAYOUT_AUTO,
		.size = CONFIG_DMA_MALLOC_LEN,
		.attribue = 0,
		.name = "DMA",
		.init = ddrlayout_dma_init,
	}, {
		.start = DDRLAYOUT_AUTO,
		.end = DDRLAYOUT_AUTO,
		.size = CONFIG_RESERVE_MEMORY_GAP,
		.attribue = 0,
		.name = "free",
		.init = NULL,
	}, {
		.start = DDRLAYOUT_AUTO,
		.end = DDRLAYOUT_AUTO,
		.size = DDRLAYOUT_AUTO,
		.attribue = 0,
		.name = "resmem",
		.init = ddrlayout_resmem_init,
	}, {
		.name = NULL,
	}
};
/*****************************************************************************/

static struct ddrlayout_t *ddrlayout_init(struct ddrlayout_t *ddr)
{
	uint32 ddrsize  = get_ddr_size();
	uint32 ddrend = ddr->start + ddrsize;
	struct ddrlayout_t *prev = NULL;

	for (; ddr->name; ddr++) {

		if (ddr->name && (!strncmp(ddr->name,"resmem",6))) {
			if (ddrsize >= _2G) {
				ddr->start = ddrend - _512M;
				ddr->size = _512M;
			} else if (ddrsize >= _1G) {
				ddr->start = ddrend - _256M;
				ddr->size = _256M;
			} else if (ddrsize >= _512M) {
				ddr->start = ddrend - _64M;
				ddr->size = _64M;
			} else if (ddrsize >= _256M) {
				ddr->start = ddrend - _32M;
				ddr->size = _32M;
			}
		}

		if (ddr->start == DDRLAYOUT_AUTO) {
			ASSERT(prev != NULL);
			ddr->start = prev->end;
		}

		if (ddr->end == DDRLAYOUT_AUTO) {
			if (ddr->size == DDRLAYOUT_AUTO) {
				if (ddr[1].name == NULL)
					ddr->size = ddrend - ddr->start;
				else {
					ASSERT(ddr[1].start != DDRLAYOUT_AUTO);
					ddr->size = ddr[1].start - ddr->start;
				}
			}
			ddr->end = ddr->start + ddr->size;
		}

		if (ddr->size == DDRLAYOUT_AUTO) {
			ASSERT(ddr->end != DDRLAYOUT_AUTO);
			ddr->size = ddr->end - ddr->start;
		}

		prev = ddr;

		ASSERT(!(ddr->start & 0x3) && !(ddr->size & 0x03));
		ASSERT1(ddr->start + ddr->size == ddr->end,
			"start:0x%08x,size:0x%08x,end:0x%08x\n",
			ddr->start, ddr->start, ddr->end);
	}

	ASSERT(prev != NULL);

	return prev;
}
/*****************************************************************************/

void sysmem_init(void)
{
	char *ptr;
	char buf[4];
	uint32 ptbtail;
	uint32 ddr_end;
	struct ddrlayout_t *ddr, *tail;
	struct ddrlayout_t *first = ddrlayout;
	const char *fmt = "%16s: 0x%08x - 0x%08x [%2s] ";

	tail = ddrlayout_init(first);

	printf("\nThe DDR Layout:\n");
	printf(fmt, "Total DDR",first->start, tail->end, "");
	printf(u32tohstr(tail->end - first->start, "B\n"));

	ptbtail = mmu_init((uint32)__page_table_start);
	ASSERT(ptbtail <= (uint32)__page_table_end);

	ddr_end = first->start + get_ddr_size();
	for (ddr = first; ddr <= tail; ddr++) {
		if (!ddr->size)
			continue;

		ptr = buf;
		if (ddr->attribue & PAGE_C)
			*ptr++ = 'C';
		if (ddr->attribue & PAGE_B)
			*ptr++ = 'B';
		*ptr = '\0';

		printf(fmt, ddr->name, ddr->start, ddr->end, buf);
		printf(u32tohstr((ddr->end - ddr->start), "B\n"));

		ASSERT1(ddr->start < ddr_end, "%s: start:0x%x, ddr_end:0x%x\n",
			ddr->name, ddr->start, ddr_end);
		ASSERT1(ddr->end <= ddr_end, "%s: end:0x%x, ddr_end:0x%x\n",
			ddr->name, ddr->end, ddr_end);
		ASSERT1(ddr->start < ddr->end, "%s: start:0x%x, end:0x%x\n",
			ddr->name, ddr->start, ddr->end);

		if (ddr->init)
			ddr->init(ddr);
	}

	mmu_set(first->start, (uint32)__end - first->start, PAGE_C | PAGE_B);
	mmu_cache_enable();
	printf("\n");
}
