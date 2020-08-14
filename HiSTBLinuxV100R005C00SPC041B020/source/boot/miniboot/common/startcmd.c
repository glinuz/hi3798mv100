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
#include <string.h>
#include <malloc.h>

fn_init cmdline_entry = NULL;

void start_cmdline(void)
{
	int timeout = CONFIG_START_TIMEOUT;

	env_get_number("bootdelay", &timeout);

	if (timeout > 99)
		timeout = 99;

#ifndef CONFIG_DISABLE_CONSOLE_INPUT
	printf("\nPress Ctrl+C quit autoboot %-2d\b\b", timeout);
#endif

	if (isbreak())
		timeout = -1;

	while (timeout-- > 0) {
		int ix = 100;
		while (ix-- > 0 && timeout >= 0) {
			mdelay(10);
			if (isbreak())
				timeout = -1;
		}
		if (timeout >= 0)
			printf("%-2d\b\b", timeout);
	}

	if (timeout == -1) {
		char *value = env_get("bootcmd");
		if (value) {
			printf("\nRun command: \"%s\"\n", value);
			run_cmd(value, strlen(value));
		}
	}

	printf("\n");

	if (cmdline_entry)
		cmdline_entry();
}
