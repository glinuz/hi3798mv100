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

#ifndef COMPILEH
#define COMPILEH

#include <stddef.h>

#define inline

#define ARRAY_SIZE(x)  (sizeof(x) / sizeof(*(x)))

#define SWAP32(_x) ((uint32)(\
	((((uint32)(_x)) & 0x000000FF) << 24) | \
	((((uint32)(_x)) & 0x0000FF00) << 8)  | \
	((((uint32)(_x)) & 0xFF000000) >> 24) | \
	((((uint32)(_x)) & 0x00FF0000) >> 8)))

#define SWAP16(_x) ((uint16)(\
	((((uint16)(_x)) & 0xFF00) >> 8) | \
	((((uint16)(_x)) & 0x00FF) << 8)))

#define AROUND(size, align)     ((size + align - 1) & (~(align - 1)))

#define roundup(x, y)           ((((x) + ((y) - 1)) / (y)) * (y))

#include <config.h>

#ifndef CONFIG_NO_STRING
#  define _T(x)     x
#else
#  define _T(x)     0
#endif

#endif /* COMPILEH */
