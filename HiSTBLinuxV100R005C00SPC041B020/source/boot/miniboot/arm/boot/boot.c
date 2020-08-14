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

#include <early_uart.h>
#include <asm/io.h>
#include <boot.h>
#include <stdio.h>
#include <string.h>
#include <trace.h>
#include <cpuinfo.h>

int arm_start(void)
{
	char *str = NULL;

	early_printf( "\r\n\r\nminiboot %d.%d.%d (%s@%s) (%s %s)\r\n\r\n",
		VERSION, PATCHLEVEL, SUBLEVEL,
		USER, HOSTNAME, __TIME__, __DATE__);

	early_printf("CPU:          %s\r\n", get_cpu_name());
	get_bootmedia(&str, NULL);
	early_printf("Boot Media:   %s\r\n", str);
	early_printf("DDR Size:     %s\r\n", u64tohstr(get_ddr_size(), "B"));
	early_printf("\r\n");

	symbol_init();

	boot_start();

	return 0;
}
