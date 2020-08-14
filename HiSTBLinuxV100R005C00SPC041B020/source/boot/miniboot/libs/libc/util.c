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

/*-
 * Copyright (c) 1998 Robert Nordier
 * Copyright (c) 2010 Pawel Jakub Dawidek <pjd@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are freely
 * permitted provided that the above copyright notice and this
 * paragraph and the following disclaimer are duplicated in all
 * such forms.
 *
 * This software is provided "AS IS" and without any express or
 * implied warranties, including, without limitation, the implied
 * warranties of merchantability and fitness for a particular
 * purpose.
 */

#include <stddef.h>
#include <string.h>
#include <debug.h>

char const hex2ascii_data[] = "0123456789abcdefghijklmnopqrstuvwxyz";
/******************************************************************************/

#ifndef CONFIG_NO_UNSAFE_FUNCTION
void strcpy(char *dst, const char *src)
{
	ASSERT(dst != NULL && src != NULL);
	while (*src != '\0')
		*dst++ = *src++;
	*dst = '\0';
}
#endif /* #ifndef CONFIG_NO_UNSAFE_FUNCTION */
/******************************************************************************/

void strcat(char *dst, const char *src)
{
	ASSERT(dst != NULL && src != NULL);
	while (*dst != '\0')
		dst++;
	while (*src != '\0')
		*dst++ = *src++;
	*dst = '\0';
}
/******************************************************************************/

char *strncat(char *dst, const char *src, size_t n)
{
	char *d = dst;

	ASSERT(dst != NULL && src != NULL && n > 0);

	if (!n)
		return dst;

	while (*d != '\0')
		d++;

	while (*src != '\0' && n--)
		*d++ = *src++;

	*d = '\0';

	return dst;
}
/******************************************************************************/

char *strchr(const char *s, char ch)
{
	ASSERT(s != NULL);
	for (; *s != '\0'; s++) {
		if (*s == ch)
			return ((char *)(uintptr_t)(const void *)s);
	}
	return (NULL);
}
/******************************************************************************/

#ifndef CONFIG_NO_UNSAFE_FUNCTION
char *strstr(const char *s1, const char *s2)
{
	ASSERT(s1 != NULL && s2 != NULL);
	int l1 = strlen(s1);
	int l2 = strlen(s2);

	while (l1 >= l2) {
		if (memcmp(s1, s2, l2) == 0) {
			return (char *)s1;
		}
		s1++;
		l1--;
	}
	return (char *)0;
}
#endif
/******************************************************************************/

uint64 memparse(const char *ptr, char **retptr)
{
	uint64 ret;
	char *endptr;

	ASSERT(ptr != NULL);

	ret = (uint64)strtoull(ptr, &endptr, 0);

	switch (*endptr) {
	case 'G': case 'g': ret <<= 10;
	case 'M': case 'm': ret <<= 10;
	case 'K': case 'k': ret <<= 10;
		endptr++;
	default:
		break;
	}

	if (retptr)
		*retptr = endptr;

	return ret;
}
/******************************************************************************/

uint32 strnlen(const char *s, uint32 len)
{
	const char *ptr;
	ASSERT(s != NULL && len > 0);
	for (ptr = s; *ptr != '\0' && len; ptr++, len--)
		;
	return ptr - s;
}
