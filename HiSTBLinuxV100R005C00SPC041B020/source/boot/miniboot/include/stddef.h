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

#ifndef __STDDEFH__
#define __STDDEFH__

#include <compile.h>
#include <sys/types.h>
#include <sys/ctype.h>

#ifndef NULL
#define NULL                           (void*)0
#endif

#define _512B                          (0x200)
#define _1K                            (0x400)
#define _2K                            (0x800)
#define _4K                            (0x1000)
#define _8K                            (0x2000)
#define _16K                           (0x4000)
#define _32K                           (0x8000)
#define _64K                           (0x10000)
#define _128K                          (0x20000)
#define _256K                          (0x40000)
#define _512K                          (0x80000)
#define _1M                            (0x100000)
#define _2M                            (0x200000)
#define _4M                            (0x400000)
#define _8M                            (0x800000)
#define _16M                           (0x1000000)
#define _32M                           (0x2000000)
#define _64M                           (0x4000000)
#define _128M                          (0x8000000)
#define _256M                          (0x10000000)
#define _512M                          (0x20000000)
#define _750M                          (0x2EE00000)
#define _1G                            (0x40000000)
#define _2G                            (0x80000000)
#define _3G                            (0xC0000000)
#define _4G                            (0x100000000ULL)
#define _8G                            (0x200000000ULL)
#define _16G                           (0x400000000ULL)
#define _32G                           (0x800000000ULL)
#define _64G                           (0x1000000000ULL)

typedef int      BOOL;
#define TRUE     1
#define FALSE    0

#endif /* __STDDEFH__ */
