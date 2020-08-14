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

#ifndef CACHEH
#define CACHEH

#define invalidIcache()  \
	asm volatile("mcr	p15, 0, r0, c7, c5, 0" : : : "r0","cc")

#define DSB() \
	asm volatile("mcr	p15, 0, r0, c7, c10, 4" : : : "r0","cc")

#define ISB() \
	asm volatile("mcr	p15, 0, r0, c7, c5, 4" : : : "r0","cc")

#endif /* CACHEH */
