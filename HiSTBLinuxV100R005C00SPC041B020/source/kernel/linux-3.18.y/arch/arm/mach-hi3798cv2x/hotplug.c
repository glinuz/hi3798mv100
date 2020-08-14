/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.6.13
 *
******************************************************************************/

#include <linux/kernel.h>
#include <linux/smp.h>
#include <linux/completion.h>
#include <mach/hardware.h>
#include <asm/cp15.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/smp_plat.h>
#include <linux/hikapi.h>

static DECLARE_COMPLETION(cpu_killed);

/*****************************************************************************/
static inline void cpu_leave_lowpower(void)
{
	unsigned int v;

	asm volatile(
	"mrc	p15, 0, %0, c1, c0, 0\n"
	"	orr	%0, %0, %1\n"
	"	mcr	p15, 0, %0, c1, c0, 0\n"
	"	mrc	p15, 0, %0, c1, c0, 1\n"
	"	orr	%0, %0, %2\n"
	"	mcr	p15, 0, %0, c1, c0, 1\n"
	"	isb \n"
	"	dsb \n"
	  : "=&r" (v)
	  : "Ir" (CR_C), "Ir" (0x40)
	  : "cc");
}

/*****************************************************************************/
static inline void platform_do_lowpower(unsigned int cpu, int *spurious)
{
	for (;;) {
		/*
		 * here's the WFI
		 */

		wfi();

	}
}
/*****************************************************************************/
/*
 * platform-specific code to shutdown a CPU
 *
 * Called with IRQs disabled
 */
void hi3798cv2x_cpu_die(unsigned int cpu)
{
	int spurious = 0;

	v7_exit_coherency_flush(louis);
	asm volatile( \
		".arch	armv7-a \n\t" \
		"mrrc	p15, 1, r0, r1, c15	@ get ACTLR \n\t" \
		"bic	r0, #(1 << 6)	        @ disable local coherency \n\t" \
		"mcrr	p15, 1, r0, r1, c15	@ set ACTLR \n\t" \
		"isb	\n\t" \
		"dsb	\n\t" \
		: : : "r0", "r1", "memory" );

	platform_do_lowpower(cpu, &spurious);

}

/*****************************************************************************/
/*
 * copy startup code to sram, and flash cache.
 * @start_addr: slave start phy address
 * @jump_addr: slave jump phy address
 */
void set_scu_boot_addr(unsigned int start_addr, unsigned int jump_addr)
{
	unsigned int *virtaddr;
#ifdef CONFIG_SECURE_EXTENSION

	virtaddr = ioremap(REG_BASE_SCTL + REG_SC_GEN1, PAGE_SIZE);

	*virtaddr = jump_addr;  /* pc jump phy address */

	smp_wmb();
	__cpuc_flush_dcache_area((void *)virtaddr,
				(size_t)sizeof(*virtaddr));
	outer_clean_range(__pa(virtaddr), __pa(virtaddr + sizeof(*virtaddr)));

	iounmap(virtaddr);
#else
	unsigned int *p_virtaddr;

	p_virtaddr = virtaddr = ioremap(start_addr, PAGE_SIZE);

	*p_virtaddr++ = 0xe51ff004; /* ldr  pc, [pc, #-4] */
	*p_virtaddr++ = jump_addr;  /* pc jump phy address */

	smp_wmb();
	__cpuc_flush_dcache_area((void *)virtaddr,
		(size_t)((char *)p_virtaddr - (char *)virtaddr));
	outer_clean_range(__pa(virtaddr), __pa(p_virtaddr));

	iounmap(virtaddr);
#endif
}


