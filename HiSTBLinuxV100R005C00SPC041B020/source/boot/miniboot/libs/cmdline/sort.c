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
#include <command.h>
#include <debug.h>

static int nr_group = 0;
static struct cmd_entry_t *list[MAX_GROUP_NUM + 1] = {0};
/*****************************************************************************/

static struct cmd_entry_t **sort_cmd(void)
{
	int ret;
	int ix, jx;
	struct cmd_entry_t *tmp;

	if (nr_group <= 1)
		return list;

	for (ix = 0; ix < nr_group; ix++) {
		for (jx = nr_group-1; jx > ix; jx--) {
			ret = strncmp(list[jx]->word->keyword,
				list[jx-1]->word->keyword, MAX_KEYWORD_SIZE);

			if (ret < 0) {
				tmp = list[jx];
				list[jx] = list[jx-1];
				list[jx-1] = tmp;
			}
		}
	}

	return list;
}
/*****************************************************************************/

static void pr_one_entry(struct cmd_entry_t *entry)
{
#define HELP_FORMAT "%-12s%s"

	/* it is not a common comment, so the command should be a leaf */
	if (entry->help && !entry->next_entry) {
		printf(HELP_FORMAT"\n", entry->word->keyword, entry->help);
	} else {
		printf(HELP_FORMAT"\n", entry->word->keyword,
			entry->word->help);
	}
}
/*****************************************************************************/

void sort_clear(void)
{
	nr_group = 0;
	list[nr_group] = NULL;
}
/*****************************************************************************/

void sort_add(struct cmd_entry_t *entry)
{
	ASSERT(nr_group < MAX_GROUP_NUM);
	list[nr_group++] = entry;
	list[nr_group] = NULL;
}
/*****************************************************************************/

void pr_cmd_help(struct cmd_entry_t *parent)
{
	int do_cmd = 1;
	struct cmd_entry_t **first = sort_cmd();

	while (parent && !parent->do_cmd)
		parent = parent->next_entry;

	if (!parent || !parent->do_cmd)
		do_cmd = 0;

	while (*first) {
		pr_one_entry(*first);
		first++;
	}

	if (do_cmd)
		printf("<cr>\n");
}
