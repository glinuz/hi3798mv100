/**
 * hi3798cvx-thermal.c
 *
 * Copyright (c) 2009-2015, HiSilicon Technologies Co., Ltd. 
 * All rights reserved.
 */

#include <linux/cpu.h>
#include <linux/io.h>
#include <linux/pm_opp.h> /* dev_pm_opp_add */
#include <linux/delay.h> /* mdelay */

#include "hisi_thermal.h"


/* only config CLUSTER_BIG when not support big.LITTLE */
struct cluster_power_coefficients cluster_data[] = {
	[CLUSTER_BIG] = {
		.dyn_coeff = 140,
		.static_coeff = 1,
		.cache_static_coeff = 0,
	},
};

/**
 * cpu_opp_table_init
 * 
 * Used for build_dyn_power_table()
 *
 * TODO: 1)The opp table should be config in 
 *		   Code/source/msp/drv/pm/opp.c: hi_init_opp_table()
 *       2)Coulde be parsed by DTS.
 */
static void cpu_opp_table_init(void)
{
	int cpu;
	struct device *dev = NULL;
	int ret = 0;

	for_each_possible_cpu(cpu) {
		dev = get_cpu_device(cpu);
		if (!dev)
			continue;

		/** 
		 * cat sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies 
		 */
		ret = dev_pm_opp_add(dev, 400 * 1000 * 1000, 840 * 1000);
		ret += dev_pm_opp_add(dev, 600 * 1000 * 1000, 840 * 1000);
		ret += dev_pm_opp_add(dev, 800 * 1000 * 1000, 900 * 1000);
		ret += dev_pm_opp_add(dev, 1200 * 1000 * 1000, 950 * 1000);
		ret += dev_pm_opp_add(dev, 1600 * 1000 * 1000, 1150 * 1000);
		                   
		if (ret)
			pr_warn("cpu[%d] opp add failed \n",cpu);
		else
			break; /* only need one cpu dev */
	}

}

/**
 * get_sensor_value
 * @val
 *
 * TODO: Could be parsed by DTS.
 */
int get_sensor_value(u32 *val)
{
	#define PMC_BASE    ioremap(0xF8A23000, 0x100)
	#define PERI_PMC10  (PMC_BASE + 0x28)
	#define PERI_PMC12  (PMC_BASE + 0x30)

	u32 u32RegVal;
	u8 i, j;
	u32 u32TsensorAverage = 0;
	u32 u32TempValue;

	writel(0x6005, PERI_PMC10);
	mdelay(16);
	for (j = 0; j < 4; j++)
	{
		u32RegVal = readl(PERI_PMC12 + 0x4 * j);
		for (i = 0; i < 2; i++)
		{
			u32TsensorAverage += ((u32RegVal >> (16 * i)) & 0x3ff);
		}
	}
	writel(0x0, PERI_PMC10);

	u32TsensorAverage = (u32TsensorAverage / 8);
	u32TempValue = ((u32TsensorAverage - 125) * 165 / 806) - 40;

	*val = u32TempValue * 1000; /* mC */

	return 0;
}

void platform_thermal_init(void)
{
	cpu_opp_table_init();
}
