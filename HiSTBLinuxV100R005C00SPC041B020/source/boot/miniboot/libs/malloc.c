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

#include <malloc.h>
#include <string.h>
#include "memlib.h"

//#define MODULE_DEBUG
#include <debug.h>

static struct memctrl_t *memctrl = NULL;
/*****************************************************************************/

void malloc_init(uint32 start, uint32 len)
{
	if (!memctrl)
		memctrl = memlib_init(start, len);
}
/*****************************************************************************/

void *__malloc(uint32 bytes, char *file, int line)
{
	if (!memctrl) {
		pr_debug("malloc module uninitializtion.\n");
		return NULL;
	}

	dbgout(_YELLOW "%s(%d): malloc:0x%x\n" _NONE, file, line, bytes);

	return memlib_malloc(memctrl, bytes);
}
/*****************************************************************************/

void *__zmalloc(uint32 bytes, char *file, int line)
{
	char *ptr;

	if (!memctrl) {
		pr_debug("malloc module uninitializtion.\n");
		return NULL;
	}

	ptr = memlib_malloc(memctrl, bytes);
	if (ptr)
		memset(ptr, 0, bytes);

	dbgout(_YELLOW "%s(%d): zmalloc:0x%x\n" _NONE, file, line, bytes);

	return ptr;
}
/*****************************************************************************/

void __free(void *mem, char *file, int line)
{
	if (!memctrl) {
		pr_debug("malloc module uninitializtion.\n");
		return;
	}

	dbgout(_YELLOW "%s(%d): free\n" _NONE, file, line);

	return memlib_free(memctrl, mem);
}
/*****************************************************************************/

void *__memalign(uint32 alignment, uint32 bytes, char *file, int line)
{
	if (!memctrl) {
		pr_debug("malloc module uninitializtion.\n");
		return NULL;
	}

	dbgout(_YELLOW "%s(%d): memalign:0x%x\n" _NONE, file, line, bytes);

	return memlib_memalign(memctrl, alignment, bytes);
}
/*****************************************************************************/

void dump_malloc(void)
{
	dump_memlib(memctrl);
}

