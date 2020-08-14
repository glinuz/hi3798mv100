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
#include <boot.h>
#include <mmu.h>
#include <stdio.h>

#include "common.h"

typedef void (*entry_fn)(void);

static void *malloc_start = (void *)(MEM_BASE_DDR + _16M + _4M);

void * malloc(unsigned int size)
{
	void *ptr = malloc_start;
	malloc_start += size;
	return ptr;
}

void free(void *ptr)
{
}

int arm_start(void)
{
	unsigned char *uncompbuf = (unsigned char *)CONFIG_UNCOMP_ADDR;
	unsigned int sz_uncompbuf = CONFIG_UNCMOP_MAX_SIZE;
	entry_fn entry = (entry_fn)uncompbuf;

	early_puts("\r\n\r\nUncompress miniboot...");

	mmu_init(MEM_BASE_DDR + CONFIG_UNCMOP_MAX_SIZE);
	mmu_set(MEM_BASE_DDR, CONFIG_UNCOMP_ADDR - MEM_BASE_DDR + CONFIG_UNCMOP_MAX_SIZE, PAGE_B | PAGE_C);

	mmu_cache_enable();

	uncompress(uncompbuf, &sz_uncompbuf, data_begin, data_end - data_begin);
	early_puts("OK");

	mmu_cache_disable();

	entry();

	return 0;
}