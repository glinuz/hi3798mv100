
#include <asm/sizes.h>

#define P15_CTRL_RR	(1<<14)			/* cache replace method bit */
#define P15_CTRL_IC	(1<<12)			/* I-cache enable bit */
#define P15_CTRL_DC	(1<<2)			/* D-cache enable bit */
#define P15_CTRL_MMU	(1<<0)			/* MMU enable bit */
#define MMU_SEC_DESC	((3<<10) | (0<<5) | (1<<4) | (2<<0))	/* A:permit |Domain|Not Use|Not Use*/
#define MMU_SEC_CB	((1<<3) | (1<<2))	/* cached, write back */
#define MMU_SEC_NCB	~((1<<3) | (1<<2))	/* no cached ,no writebuf*/


/* r0: page directory address, must align to 16KB */
.global mmu_pagedir_init
mmu_pagedir_init:
	/*
	*init page dir param for  setction address
	*/
	add	r3, r0, #SZ_16K
	ldr	r2, =MMU_SEC_DESC	/* param */
1:
	str	r2, [r0], #4		/* section 1:1 mapping */
	add	r2, r2, #SZ_1M		/* setction base address */
	teq	r0, r3
	bne	1b

	mov	pc,lr

	/*
	*r0: page directory address, must align to 16KB
	*r1: start of cached MEM, must align to 1MB
	*r2: size of cached MEM, must align to 1MB
	*/
.global mmu_pagedir_cached_range
mmu_pagedir_cached_range:
	/*
	*init page dir param for cached , writebuffer
	*/
	add	r1, r0, r1, lsr #18	/* r1 = r0 + r1>>18  the start index addr of map mem */
	add	r2, r1, r2, lsr #18	/* the end index addr of map mem */

1:
	ldr	r0, [r1]
	orr	r0, #MMU_SEC_CB
	str	r0, [r1], #4		/*store and index addr ++ */
	
	cmp	r1, r2
	blt	1b
	
	mov	pc,lr
	
	
	/*
	*r0: page directory address, must align to 16KB
	*r1: start of cached MEM, must align to 1MB
	*r2: size of cached MEM, must align to 1MB
	*/
.global mmu_pagedir_nocached_range
mmu_pagedir_nocached_range:
clean_loop:
	/*
	*init page dir param for no cache ,no writebuffer
	*/
	mrc	p15, 0, r15, c7, c14, 3
	bne	clean_loop		/* test , clean and invalidate D-cache */
	
	mov	r3, #0
	mcr	p15, 0, r3, c8, c7, 0	/* flush TLBs */

	add	r1, r0, r1, lsr #18	/* the start section index of map mem */
	add	r2, r1, r2, lsr #18	/* the end section index of map mem */

1:
	ldr	r0, [r1]
	and	r0, #MMU_SEC_NCB	/* the setcion is no cache,no writebuf */
	str	r0, [r1], #4

	cmp	r1, r2
	blt	1b
	
	mov	pc,lr	


.global flush_cache_off
flush_cache_off:

	/*
	*invalidate(flush) TLB
	*/
	mrc	p15, 0, r0, c1, c0, 0	/* read control reg >> r0 */
	mcr	p15, 0, r0, c1, c0, 0	/* write r0 >> control reg */

	mov	r0, #0
	mcr	p15, 0, r0, c8, c7, 0	/* flush TLBs */

	mov	pc,lr


/* r0: page directory address, must align to 16KB */
.global mmu_startup
mmu_startup:

	/*
	*enable mmu 
	*/
	stmdb	sp!, {r0, lr}
	bl	flush_cache_off		/* r0,lr >> stack */
	ldmia	sp!, {r0, lr}

	mrc	p15, 0, r3, c1, c0, 0	/* read control reg */
	bic	r3, r3, #P15_CTRL_RR	/* cache replace method */
	orr	r3, r3, #P15_CTRL_MMU	/* mmu enable bit */

	mov	r2, #0
	mov	r1, #-1

	mcr	p15, 0, r0, c2, c0, 0	/* write page table pointer to Base  Reg */
	mcr	p15, 0, r1, c3, c0, 0	/* write domain access control reg */
	mcr	p15, 0, r3, c1, c0, 0	/* enable mmu */
	mcr	p15, 0, r2, c8, c7, 0	/* flush TLBs */
	
	mov	pc,lr
	
.global icache_disable
icache_disable:
	
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #P15_CTRL_IC
	mcr	p15, 0, r0, c1, c0, 0
	
	mov	pc,lr

.global dcache_start
dcache_start:	
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #P15_CTRL_DC
	mcr	p15, 0, r0, c1, c0, 0
	isb

	mov	pc,lr

.global dcache_stop
dcache_stop:
	push	{r4 - r12, lr}

	/* disable mmu dcache rr */
	mrc	p15, 0, r0, c1, c0
	bic	r0, r0, #P15_CTRL_DC	/* disable d-cache bit */
	bic	r0, r0, #P15_CTRL_MMU	/* disable mmu bit */
	mcr	p15, 0, r0, c1, c0
	isb

	bl	__cache_flush_all
	pop	{r4 - r12, pc}

.global __cache_flush_all
__cache_flush_all:
	mrc	p15, 1, r0, c0, c0, 1	/* read clidr */
	ands	r3, r0, #0x7000000	/* extract loc from clidr */
	mov	r3, r3, lsr #23	/* left align loc bit field */
	beq	finished	/* if loc is 0, then no need to clean */
	mov	r10, #0		/* start clean at cache level 0 */
flush_levels:
	add	r2, r10, r10, lsr #1	/* work out 3x current cache level */
	mov	r1, r0, lsr r2	/* extract cache type bits from clidr */
	and	r1, r1, #7	/* mask of the bits for current cache only */
	cmp	r1, #2		/* see what cache we have at this level */
	blt	skip		/* skip if no cache, or just i-cache */

	mcr	p15, 2, r10, c0, c0, 0	/* select current cache level in cssr */
	isb	/* isb to sych the new cssr&csidr */
	mrc	p15, 1, r1, c0, c0, 0	/* read the new csidr */

	and	r2, r1, #7	/* extract the length of the cache lines */
	add	r2, r2, #4	/* add 4 (line length offset) */
	ldr	r4, =0x3ff
	ands	r4, r4, r1, lsr #3	/* find maximum number on the way size */
	clz	r5, r4	/* find bit position of way size increment */
	ldr	r7, =0x7fff
	ands	r7, r7, r1, lsr #13	/* extract max number of the index size */
loop1:
	mov	r9, r4	/* create working copy of max way size */
loop2:
	orr	r11, r10, r9, lsl r5	/* factor way and cache number into r11 */
	orr	r11, r11, r7, lsl r2	/* factor index number into r11 */

	mcr	p15, 0, r11, c7, c14, 2	/* clean & invalidate by set/way */
	subs	r9, r9, #1	/* decrement the way */
	bge	loop2
	subs	r7, r7, #1	/* decrement the index */
	bge	loop1
skip:
	add	r10, r10, #2	/* increment cache number */
	cmp	r3, r10
	bgt	flush_levels
finished:
	mov	r10, #0	/* swith back to cache level 0 */
	mcr	p15, 2, r10, c0, c0, 0	/* select current cache level in cssr */
	dsb
	isb
	mov	pc, lr

