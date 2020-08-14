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

#ifndef MMUH
#define MMUH
/******************************************************************************/

#include <stddef.h>
#include <cache.h>

#define PAGE_RW           (3 << 10)
#define PAGE_DOMAIN       (0 << 5)
#define PAGE_SECTION      (2)
#define PAGE_XN           (1 << 4)
#define PAGE_B            (1 << 2)
#define PAGE_C            (1 << 3)

int mmu_init(unsigned int pgt_offset);

void mmu_set(uint32 start, uint32 size, uint32 attrib);

int mmu_cache_enable(void);

int mmu_cache_disable(void);

int cache_flush_all(void);

/******************************************************************************/
#endif /* MMUH */
