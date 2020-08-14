/******************************************************************************
 *  Copyright (C) 2015 Cai Zhiyong
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
 *
******************************************************************************/

#define pr_fmt(fmt) "cpuidle: " fmt

#include <linux/cpu_pm.h>
#include <linux/module.h>
#include <asm/suspend.h>
#include <linux/spinlock.h>
#include <linux/cpuidle.h>
#include <asm/cpuidle.h>
#include <mach/hardware.h>
#include <asm/cp15.h>
#include <linux/cpu.h>
#include <linux/irqchip/arm-gic.h>
#include <asm/cacheflush.h>
#include <linux/gpio.h>

#include <debug.h>
/******************************************************************************/

extern void flash_l1_cache_hi3798cv2x(void);
extern void cpu_powerup_hi3798cv2x(void);
extern void cpu_powerdown_hi3798cv2x(void);

struct cpu_pm_stat_t {
	u32 max_down;
	u32 max_up;
	u64 total_down;
	u64 total_up;
};

#define DEFAULT_LATENCY             170
#define DEFAULT_RESIDENCY           1

static u32 latency   = DEFAULT_LATENCY; //unit: us
static u32 residency = DEFAULT_RESIDENCY; //unit: us

static struct cpu_pm_stat_t cpu_pm_stat[CONFIG_NR_CPUS];

static struct cpumask cpuidle_cpu_mask = CPU_MASK_NONE;

static arch_spinlock_t pmc_powerdown_lock = __ARCH_SPIN_LOCK_UNLOCKED;
static arch_spinlock_t pmc_stat_lock = __ARCH_SPIN_LOCK_UNLOCKED;

static u32 __iomem *cpu_resume_vaddr;
/******************************************************************************/
static int __init latency_setup(char *s)
{
	int ret;

	if (!s)
		return 0;

	ret = kstrtoint(s, 10, &latency);
	if (ret)
		return ret;

	return 1;

}
__setup("cpuidle.latency=", latency_setup);

static int __init residency_setup(char *s)
{
	int ret;

	if (!s)
		return 0;

	ret = kstrtoint(s, 10, &residency);
	if (ret)
		return ret;

	return 1;

}
__setup("cpuidle.residency=", residency_setup);

static void __maybe_unused pmc_stat_enable(int cpuid)
{
	u32 regval;
	u32 *pmcaddr;

	arch_spin_lock(&pmc_stat_lock);

	pmcaddr= __io_address(REG_PERI_PMC64);

	regval = readl(pmcaddr);
	if (cpuid == 2) {
		regval |= (1 << 4);
		regval &= ~(1 << 5);
	} else if (cpuid == 3) {
		regval |= (1 << 0);
		regval &= ~(1 << 1);
	} else {
		BUG();
	}

	writel(regval, pmcaddr);

	arch_spin_unlock(&pmc_stat_lock);
}
/******************************************************************************/

static void __maybe_unused pmc_dump_stat(int cpuid)
{
	u32 regval;
	u32 *pmcaddr;
	u32 tdown;
	u32 tup;
	struct cpu_pm_stat_t *pmstat;

	arch_spin_lock(&pmc_stat_lock);

	pmcaddr= __io_address(REG_PERI_PMC64);

	regval = readl(pmcaddr);
	if (cpuid == 2) {
		regval |= (1 << 5);
	} else if (cpuid == 3) {
		regval |= (1 << 1);
	} else {
		BUG();
	}

	writel(regval, pmcaddr);

	arch_spin_unlock(&pmc_stat_lock);

	if (cpuid == 2) {
		tup = readl(__io_address(REG_PERI_PMC68));
		tdown = readl(__io_address(REG_PERI_PMC67));
	} else if (cpuid == 3) {
		tup = readl(__io_address(REG_PERI_PMC66));
		tdown = readl(__io_address(REG_PERI_PMC65));
	} else {
		BUG();
	}

	pmstat = &cpu_pm_stat[cpuid];

	if (tup > pmstat->max_up)
		pmstat->max_up = tup;

	if (tdown > pmstat->max_down)
		pmstat->max_down = tdown;

	pmstat->total_down += (u64)tdown;
	pmstat->total_up += (u64)tup;
}
/******************************************************************************/

static void pmc_powerdown(int cpuid)
{
	u32 regval;
	u32 *pmcaddr;

	arch_spin_lock(&pmc_powerdown_lock);

	pmcaddr = __io_address(REG_PERI_PMC1);
	regval = readl(pmcaddr);
	regval &= ~(0xff << 8);
	regval |= (0x40 << 8);
	writel(regval, pmcaddr);


	pmcaddr = __io_address(REG_PERI_PMC0);
	regval = readl(pmcaddr);
	if (cpuid == 2) {
		regval |= (1 << 23) | (1 << 22) | (1 << 19);
	} else if (cpuid == 3) {
		regval |= (1 << 31) | (1 << 30) | (1 << 27);
	} else {
		BUG();
	}
	writel(regval, pmcaddr);

	pmcaddr = __io_address(REG_PERI_PMC3);
	regval = readl(pmcaddr);
	if (cpuid == 2) {
		regval |= (1 << 6) | (1 << 10);
	} else if (cpuid == 3) {
		regval |= (1 << 7) | (1 << 11);
	} else {
		BUG();
	}
	regval &= ~0x3;
	regval |= 0x1;
	writel(regval, pmcaddr);

	arch_spin_unlock(&pmc_powerdown_lock);
}
/******************************************************************************/

static int notrace cpu_entry_powerdown(unsigned long arg)
{
	int cpuid = smp_processor_id();

	gic_cpu_if_down();

	pmc_powerdown(cpuid);

	//flash_l1_cache_hi3798cv2x();
	v7_exit_coherency_flush(louis);

	cpu_powerdown_hi3798cv2x();

	return 1;
}
/******************************************************************************/

//static DEFINE_PER_CPU(int, per_init) = { 0 };

static int hi3798cv2xx_cpuidle_entry(struct cpuidle_device *dev,
				     struct cpuidle_driver *drv, int idx)
{
	int ret;
	//int cpuid = smp_processor_id();

	//if (per_cpu(per_init, cpuid) == 0) {
	//	DBG_OUT("cpu%d first entry cpuidle.\n", cpuid);
	//	per_cpu(per_init, cpuid) = 1;
	//}
#ifdef CONFIG_SECURE_EXTENSION
	cpu_resume_vaddr[0] = (u32)virt_to_phys(cpu_powerup_hi3798cv2x);
#else
	cpu_resume_vaddr[0] = 0xe51ff004;
	cpu_resume_vaddr[1] = (u32)virt_to_phys(cpu_powerup_hi3798cv2x);
#endif
	isb();
	dsb();

	///* LED off */
	//if (cpuid == 2)
	//	writel(0x0, __io_address(0xf8004008));
	//else if (cpuid == 3)
	//	writel(0x0, __io_address(0xf8004010));

	ret = cpu_pm_enter();
	if (!ret) {
		cpu_suspend(0, cpu_entry_powerdown);
		cpu_pm_exit();
	} else {
		BUG();
	}

	///* LED on */
	//if (cpuid == 2)
	//	writel(0x2, __io_address(0xf8004008));
	//else if (cpuid == 3)
	//	writel(0x4, __io_address(0xf8004010));

	return ret ? -1 : idx;;
}
/******************************************************************************/

static struct cpuidle_driver hi3798cv2xx_cpuidle_driver = {
	.name = "Hi3798cv2x cpuidle",
	.owner = THIS_MODULE,
	.states = {
		[0] = ARM_CPUIDLE_WFI_STATE,
		[1] = {
			.enter            = hi3798cv2xx_cpuidle_entry,
			.exit_latency     = DEFAULT_LATENCY,
			.target_residency = DEFAULT_RESIDENCY,
			.power_usage      = 1,
			.flags            = CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TIMER_STOP,
			.name             = "C1",
			.desc             = "Hi3798cv2x CPU Power Down",
		},
	},
	.state_count = 2,
	.cpumask = &cpuidle_cpu_mask,
};
/******************************************************************************/

static int __init hi3798cv2xx_cpuidle_init(void)
{
	pr_info("CPU idle initializing\n");

	///* Set GPIO 5/1 5/2 to output mode */
	//writel(0x06, __io_address(0xf8004400));

	///* LED off */
	//writel(0x0, __io_address(0xf8004008));
	//writel(0x0, __io_address(0xf8004010));
#ifdef CONFIG_SECURE_EXTENSION
	cpu_resume_vaddr = ioremap_nocache(REG_BASE_SCTL + REG_SC_GEN1, PAGE_SIZE);
#else
	cpu_resume_vaddr = ioremap_nocache(0xFFFF0000, PAGE_SIZE);
#endif
	if (!cpu_resume_vaddr) {
		pr_err("ioremap_nocache fail.\n");
		return -ENOMEM;
	}
	memset(&cpu_pm_stat, 0, sizeof(cpu_pm_stat));

	cpumask_or(&cpuidle_cpu_mask, cpumask_of(2), cpumask_of(3));

	hi3798cv2xx_cpuidle_driver.states[1].exit_latency = latency;
	hi3798cv2xx_cpuidle_driver.states[1].target_residency = residency;

	return cpuidle_register(&hi3798cv2xx_cpuidle_driver, NULL);
}

device_initcall(hi3798cv2xx_cpuidle_init);
