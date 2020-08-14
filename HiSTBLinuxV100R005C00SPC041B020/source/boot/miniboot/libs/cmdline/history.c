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
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <command.h>
#include <module.h>

#define MAX_HIST                       10

struct hist_t {
	int pos;
	int num;
	char str[MAX_HIST][MAX_CMD_LENGTH];
};

static struct hist_t __hist;
static struct hist_t *phist = &__hist;

static void hist_init(void *ptr)
{
	phist->pos = 0;
	phist->num = 0;
}

static struct module_t module_hist = {
	.priority = MODULE_PRI_ENV,
	.init = hist_init,
};
MODULE(module_hist);

void hist_save(char *cmdline, int length)
{
	if (!length)
		return;

	length = min(MAX_CMD_LENGTH - 1, length);

	if (phist->num) {
		int pos = phist->pos - 1;
		if (pos < 0)
			pos = phist->num - 1;
		if (!strncmp(phist->str[pos], cmdline, length))
			return;
	}

	strncpy(phist->str[phist->pos], cmdline, length);
	phist->str[phist->pos][length] = 0;

	if (phist->num < MAX_HIST)
		phist->num++;

	if (++(phist->pos) == MAX_HIST)
		phist->pos = 0;
}

char *hist_load(int pos)
{
	if (!pos || !phist->num)
		return NULL;

	pos += phist->pos;

	while (pos >= phist->num)
		pos -= phist->num;

	while (pos < 0)
		pos += phist->num;

	return phist->str[pos];
}
