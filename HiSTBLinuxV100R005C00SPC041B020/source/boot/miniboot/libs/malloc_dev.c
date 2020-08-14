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
#include <debug.h>

#include "memlib.h"

static struct memctrl_t *memctrl = NULL;
/*****************************************************************************/

void devmalloc_init(uint32 start, uint32 len)
{
	if (!memctrl)
		memctrl = memlib_init(start, len);
}
/*****************************************************************************/

void *devmalloc(uint32 bytes)
{
	if (!memctrl) {
		pr_debug("device malloc module uninitializtion.\n");
		return NULL;
	}

	return memlib_malloc(memctrl, bytes);
}
/*****************************************************************************/

void devfree(void *mem)
{
	if (!memctrl) {
		pr_debug("device malloc module uninitializtion.\n");
		return;
	}

	return memlib_free(memctrl, mem);
}
/*****************************************************************************/

void *devmemalign(uint32 alignment, uint32 bytes)
{
	if (!memctrl) {
		pr_debug("device malloc module uninitializtion.\n");
		return NULL;
	}

	return memlib_memalign(memctrl, alignment, bytes);
}
/*****************************************************************************/
