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

#ifndef DBG_LIBCH
#define DBG_LIBCH

#include <config.h>

#ifdef CONFIG_DEBUG
#  define ENTRY_NAME(name) dbg_##name
#else
#  define ENTRY_NAME(name) name
#endif

#define ENTRY(name) \
    .global ENTRY_NAME(name); \
    .align 2; \
    .type ENTRY_NAME(name), %function; \
    ENTRY_NAME(name):

#ifndef __ASSEMBLY__
extern int ENTRY_NAME(memcmp)(const void *s1, const void *s2, size_t n);
extern int ENTRY_NAME(strncmp)(const char *s1, const char *s2, size_t n);
extern int ENTRY_NAME(memcpy)(void *dest, void *src, size_t n);
extern size_t ENTRY_NAME(strlen)(const char *s);
extern void *ENTRY_NAME(memset)(void *s, int c, size_t n);
extern int ENTRY_NAME(strcmp)(const char *s1, const char *s2);
#endif /* __ASSEMBLY__ */

#endif /* DBG_LIBCH */
