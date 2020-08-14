/******************************************************************************
 *  Copyright (C) 2016 Cai Zhiyong
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
 * Create By Cai Zhiying 2016.1.12
 *
******************************************************************************/

#include <linux/init.h>
#include <linux/amba/bus.h>
#include <linux/amba/pl061.h>
#include <linux/of_platform.h>
#include "irqs.h"

#define PL061_PLAT_DATA(_gpio, _gpio_base) \
struct pl061_platform_data _gpio##_plat_data = { \
	.gpio_base = (_gpio_base), .irq_base = (_gpio_base) + VIRT_IRQS_START, \
}

static PL061_PLAT_DATA(gpio0,  0);
static PL061_PLAT_DATA(gpio1,  8);
static PL061_PLAT_DATA(gpio2,  16);
static PL061_PLAT_DATA(gpio3,  24);
static PL061_PLAT_DATA(gpio4,  32);
static PL061_PLAT_DATA(gpio5,  40);
static PL061_PLAT_DATA(gpio6,  48);
static PL061_PLAT_DATA(gpio7,  56);
static PL061_PLAT_DATA(gpio8,  64);
static PL061_PLAT_DATA(gpio9,  72);
static PL061_PLAT_DATA(gpio10, 80);
static PL061_PLAT_DATA(gpio11, 88);
static PL061_PLAT_DATA(gpio12, 96);

#define PL061_PLAT_DEV(_gpio, _phyaddr) \
	OF_DEV_AUXDATA("arm,pl061", _phyaddr, #_gpio, &_gpio##_plat_data)

static struct of_dev_auxdata hi3798cv200_auxdata_lookup[] __initdata = {
	PL061_PLAT_DEV(gpio0,  0xf8b20000),
	PL061_PLAT_DEV(gpio1,  0xf8b21000),
	PL061_PLAT_DEV(gpio2,  0xf8b22000),
	PL061_PLAT_DEV(gpio3,  0xf8b23000),
	PL061_PLAT_DEV(gpio4,  0xf8b24000),
	PL061_PLAT_DEV(gpio5,  0xf8004000),
	PL061_PLAT_DEV(gpio6,  0xf8b26000),
	PL061_PLAT_DEV(gpio7,  0xf8b27000),
	PL061_PLAT_DEV(gpio8,  0xf8b28000),
	PL061_PLAT_DEV(gpio9,  0xf8b29000),
	PL061_PLAT_DEV(gpio10, 0xf8b2a000),
	PL061_PLAT_DEV(gpio11, 0xf8b2b000),
	PL061_PLAT_DEV(gpio12, 0xf8b2c000),
	{},
};

u32 get_gpio_group_count(void)
{
	return 13;
}

static int __init hi3798cv200_device_tree_init(void)
{
	of_platform_populate(NULL, of_default_bus_match_table, hi3798cv200_auxdata_lookup, NULL);
	return 0;
}
postcore_initcall(hi3798cv200_device_tree_init);
