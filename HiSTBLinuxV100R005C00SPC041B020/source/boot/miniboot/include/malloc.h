/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Lai Yingjun, Czyong.
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

#ifndef MALLOCH
#define MALLOCH
/******************************************************************************/

#include <stddef.h>

void malloc_init(uint32 start, uint32 len);

void *__malloc(uint32 bytes, char *file, int line);
#define malloc(bytes) __malloc(bytes, __FILE__, __LINE__)

void *__zmalloc(uint32 bytes, char *file, int line);
#define zmalloc(bytes) __zmalloc(bytes, __FILE__, __LINE__)

void __free(void *mem, char *file, int line);
#define free(mem) __free(mem, __FILE__, __LINE__)

void *__memalign(uint32 alignment, uint32 bytes, char *file, int line);
#define memalign(alignment, bytes) __memalign(alignment, bytes, __FILE__, __LINE__)

void dump_malloc(void);

/*****************************************************************************/

void devmalloc_init(uint32 start, uint32 len);
void *devmalloc(uint32 bytes);
void devfree(void *mem);
void *devmemalign(uint32 alignment, uint32 bytes);

/*****************************************************************************/

void resmalloc_init(uint32 start, uint32 len);

void resmem_dump(int verbose);

void *__resmalloc(uint32 bytes, char *file, int line);
#define resmalloc(_bytes) __resmalloc(_bytes, __FILE__, __LINE__)

void *__resmemalign(uint32 alignment, uint32 bytes, char *file, int line);
#define resmemalign(_alignment, _bytes) __resmemalign(_alignment, _bytes, __FILE__, __LINE__)

/* void *reserve_mem_alloc(unsigned int size, unsigned int *allocsize) */
#define reserve_mem_alloc(_size, _allocsize) ({ \
	char *addr = NULL;uint32 __size = (_size + 4096 - 1) & ~(4096 - 1); \
	addr = __resmemalign(4096, __size, __FILE__, __LINE__); \
	*(_allocsize) = (__size); addr; })

void resfree(void *mem);

/******************************************************************************/
#endif /* MALLOCH */
