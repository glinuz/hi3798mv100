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

#define DSB(_r)  		mcr	p15, 0, _r, c7, c10, 4
#define ISB(_r)  		mcr	p15, 0, _r, c7, c5, 4
#define DMB(_r)  		mcr	p15, 0, _r, c7, c10, 5
#define invalidateICache(_r)	mcr	p15, 0, _r, c7, c5, 0
#define invalidateTLB(_r) 	mcr	p15, 0, _r, c8, c7, 0
#define invalidateBTC(_r) 	mcr	p15, 0, _r, c7, c5, 6

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@    int __mmu_cache_enable(unsigned int pagetable);
@
.text
.align	2
.global	__mmu_cache_enable
.type	__mmu_cache_enable, %function
__mmu_cache_enable:
	push	{r0 - r3, lr}

	/* set page table */
	mcr	p15, 0, r0, c2, c0, 0
	mov	r0, #-1
	mcr	p15, 0, r0, c3, c0, 0

	/* enable mmu dcache rr */
	mrc	p15, 0, r0, c1, c0
	orr	r0, r0, #0x4000
	orr	r0, r0, #5
	mcr	p15, 0, r0, c1, c0

	mov	r1, #0
	invalidateTLB(r1)
	invalidateICache(r1)
	ISB(r1)

	pop	{r0 - r3, pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@    int __mmu_cache_disable(void);
@
.text
.align	2
.global	__mmu_cache_disable
.type	__mmu_cache_disable, %function
__mmu_cache_disable:
	push	{r4 - r12, lr}

	/* disable mmu dcache rr */
	mrc	p15, 0, r0, c1, c0
	bic	r0, r0, #0x4000
	bic	r0, r0, #5
	mcr	p15, 0, r0, c1, c0

	bl	__cache_flush_all

	mov	r3, #0
	invalidateTLB(r3)
	invalidateBTC(r3)
	DSB(r3)
	ISB(r3)

	pop	{r4 - r12, pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@    int __cache_flush_all(void);
@
.text
.align	2
.global	__cache_flush_all
.type	__cache_flush_all, %function
__cache_flush_all:
	mrc	p15, 1, r0, c0, c0, 1		@ read clidr
	ands	r3, r0, #0x7000000		@ extract loc from clidr
	mov	r3, r3, lsr #23			@ left align loc bit field
	beq	finished			@ if loc is 0, then no need to clean
	mov	r10, #0				@ start clean at cache level 0
flush_levels:
	add	r2, r10, r10, lsr #1		@ work out 3x current cache level
	mov	r1, r0, lsr r2			@ extract cache type bits from clidr
	and	r1, r1, #7			@ mask of the bits for current cache only
	cmp	r1, #2				@ see what cache we have at this level
	blt	skip				@ skip if no cache, or just i-cache

	mcr	p15, 2, r10, c0, c0, 0		@ select current cache level in cssr
	isb					@ isb to sych the new cssr&csidr
	mrc	p15, 1, r1, c0, c0, 0		@ read the new csidr

	and	r2, r1, #7			@ extract the length of the cache lines
	add	r2, r2, #4			@ add 4 (line length offset)
	ldr	r4, =0x3ff
	ands	r4, r4, r1, lsr #3		@ find maximum number on the way size
	clz	r5, r4				@ find bit position of way size increment
	ldr	r7, =0x7fff
	ands	r7, r7, r1, lsr #13		@ extract max number of the index size
loop1:
	mov	r9, r4				@ create working copy of max way size
loop2:
	orr	r11, r10, r9, lsl r5		@ factor way and cache number into r11
	orr	r11, r11, r7, lsl r2		@ factor index number into r11

	mcr	p15, 0, r11, c7, c14, 2		@ clean & invalidate by set/way
	subs	r9, r9, #1			@ decrement the way
	bge	loop2
	subs	r7, r7, #1			@ decrement the index
	bge	loop1
skip:
	add	r10, r10, #2			@ increment cache number
	cmp	r3, r10
	bgt	flush_levels
finished:
	mov	r10, #0				@ swith back to cache level 0
	mcr	p15, 2, r10, c0, c0, 0		@ select current cache level in cssr
	dsb
	isb
	mov	pc, lr
