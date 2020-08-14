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

#include <stdio.h>
#include <boot.h>
#include <config.h>
#include <compile.h>
#include <module.h>
#include <early_uart.h>
#include <mmu.h>
#include <command.h>
#include <env_set.h>
#include <delay.h>
#include <malloc.h>
#include <irqchip.h>
#include <timer.h>
#include <irq.h>
#include <task.h>
#include <kern_param.h>
#include <debug.h>
#include <board.h>
#include <params.h>

#ifdef CONFIG_PRODUCT_WITH_BOOT
extern const int fastapp_entry(int argc, char *argv[]);
#endif

#if (defined(CONFIG_BOOTROM_SUPPORT) || defined(CONFIG_BOOTROM_CA_SUPPORT))
extern void start_bootstrap(int (*handle)(void));
#endif

__attribute__((weak)) void test_init(void) { }
/*****************************************************************************/

int boot_start(void)
{
#ifdef CONFIG_UART
	uart_init(NULL);
#endif

	clock_tick_init(CONFIG_CLOCK_TICK);

	//irq_init();

	sysmem_init();

	clock_source_init(CONFIG_CLOCK_SOURCE, CONFIG_TASK_CYCLE);

	task_init(CONFIG_MAX_TASK);

	kern_param_init();

	module_init();

#if (defined(CONFIG_BOOTROM_SUPPORT) || defined(CONFIG_BOOTROM_CA_SUPPORT))
	start_bootstrap(NULL);
#endif

	cpu_enable_irq();

#ifdef CONFIG_NET
	eth_config_init();
#endif

#ifdef CONFIG_BOOTARGS_MERGE
	bootargs_prepare();
#endif

	app_init();

	test_init();

#ifdef CONFIG_PRODUCT_WITH_BOOT
	fastapp_entry(0, NULL);
#endif

	resmem_dump(
	#ifndef CONFIG_RESERVE_MEMORY_DEBUG
		0
	#else
		1
	#endif
	);

#ifdef CONFIG_CMDLINE
	start_cmdline();
#endif

	reset_cpu(0);

	return 0;
}
