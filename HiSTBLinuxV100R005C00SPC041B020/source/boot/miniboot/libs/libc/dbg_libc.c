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

#include <stddef.h>
#include <debug.h>

#include "dbg_libc.h"

int memcmp(const void *s1, const void *s2, size_t n)
{
	ASSERT(s1 != NULL && s2 != NULL);
	return ENTRY_NAME(memcmp)(s1, s2, n);
}

int memcpy(void *dest, void *src, size_t n)
{
	//ASSERT(dest != NULL && src != NULL);
	ASSERT1(dest <= src || dest >= src + n, "dest:0x%08x,src:0x%08x,n:%d\n", dest, src, n);
	return ENTRY_NAME(memcpy)(dest, src, n);
}

void *memset(void *s, int c, size_t n)
{
	ASSERT(s != NULL);
	return ENTRY_NAME(memset)(s, c, n);
}

int strcmp(const char *s1, const char *s2)
{
	ASSERT(s1 != NULL && s2 != NULL);
	return ENTRY_NAME(strcmp)(s1, s2);
}

size_t strlen(const char *s)
{
	ASSERT(s != NULL);
	return ENTRY_NAME(strlen)(s);
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	ASSERT(s1 != NULL && s2 != NULL);
	return ENTRY_NAME(strncmp)(s1, s2, n);
}
