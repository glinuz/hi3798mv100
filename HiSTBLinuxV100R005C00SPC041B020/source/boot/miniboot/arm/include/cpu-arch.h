/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Cai Zhiyong
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

#ifndef CPUARCHH
#define CPUARCHH
/******************************************************************************/
#include <stddef.h>

#define cpu_enable_irq() \
	asm volatile("mrs	r0, cpsr\n" \
		     "bic	r0, r0, #0x80\n" \
		     "msr	cpsr, r0\n" \
		     ::: "r0", "cc")

#define cpu_disable_irq() \
	asm volatile("mrs	r0, cpsr\n" \
		     "orr	r0, r0, #0x80\n" \
		     "msr	cpsr, r0\n" \
		     ::: "r0", "cc")

#define cpu_irq_save() \
	({int _flags;                               \
	asm volatile("mrs	r0, cpsr\n"         \
		     "mov	%0, r0\n"           \
		     "orr	r0, r0, #0x80\n"    \
		     "msr	cpsr, r0\n"         \
		     :"=r"(_flags) : : "r0", "cc"); \
	_flags;})

#define cpu_irq_restore(_flags) \
	asm volatile("msr	cpsr, %0\n" \
		     : : "r"(_flags) : "cc")

#define SCTLR_V               0x2000

#define GET_SCTLR()  ({ uint32 _value; \
	asm volatile("mrc	p15, 0, %0, c1, c0, 0\n" \
		:"=r"(_value)::"memory"); _value;})

#define SET_SCTLR(_value)  do { \
	asm volatile("mcr	p15, 0, %0, c1, c0, 0\n" \
		::"r"(_value):"memory");\
} while (0)

struct regs_t {
	uint32 r0;
	uint32 r1;
	uint32 r2;
	uint32 r3;
	uint32 r4;
	uint32 r5;
	uint32 r6;
	uint32 r7;
	uint32 r8;
	uint32 r9;
	uint32 r10;
	uint32 r11;
	uint32 r12;
	uint32 sp;
	uint32 lr;
	uint32 pc;
	uint32 spsr;
	uint32 vector;
};

/******************************************************************************/
#endif /* CPUARCHH */
