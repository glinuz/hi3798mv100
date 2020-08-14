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

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <module.h>
#include <stddef.h>
#include <command.h>
#include <boot.h>

#include "parse.h"
#include "linepro.h"

static char cmdline[MAX_CMD_LENGTH];

static int entry(void)
{
	int ret;

	for (;;) {
		do {
			ret = get_line(pcmd, cmdline, sizeof(cmdline));
		} while (ret <= 0);

		run_mutli_cmd(pcmd, cmdline, ret);
	}
	return 0;
}

static void entry_register(void *ptr)
{
	cmdline_entry = entry;
}

static struct module_t module_entry = {
	.priority = MODULE_PRI_ENV,
	.init = entry_register,
};
MODULE(module_entry);
