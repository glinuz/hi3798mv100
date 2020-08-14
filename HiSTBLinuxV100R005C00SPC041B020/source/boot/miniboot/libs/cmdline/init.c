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
#include <module.h>

#define MOULE_NAME "cmdline: "
#include <debug.h>

#define SET_TYPE_ID(_entry, _id)  do { \
	(_entry)->word->type = (((_entry)->word->type & 0x00FFFFFF) \
		| (((_id) & 0xFF) << 24)); \
} while (0)

#define GET_TYPE_ID(_entry)  (((_entry)->word->type >> 24) & MAX_GROUP_NUM)

static struct cmd_t __cmd;
struct cmd_t *pcmd = &__cmd;
/*****************************************************************************/

static int init_subentry(struct cmd_entry_t *entry)
{
	int ix;
	int nr_group = 0;
	struct cmd_entry_t *subentry;
	struct cmd_entry_t *next;
	struct cmd_entry_t **subgroup;
	struct cmd_entry_t *first_subentry = NULL;

	/* mark the different comamnd */
	for (next = entry; next; next = next->next_entry) {
		subentry = next + 1;
		if (subentry->word && !GET_TYPE_ID(subentry)) {
			SET_TYPE_ID(subentry, ++nr_group);
			if (!first_subentry)
				first_subentry = subentry;
		}
		if (nr_group >= MAX_GROUP_NUM)
			pr_error("it is only support %d command.\n",
				 MAX_GROUP_NUM);
	}

	/* not found one group */
	if (!nr_group)
		return 0;

	subgroup = &(entry + 1)->next_group;
	for (ix = 1; ix <= nr_group; ix++) {
		struct cmd_entry_t *first = NULL;
		struct cmd_entry_t **subnext = NULL;

		/* link the same comamnd */
		for (next = entry; next; next = next->next_entry) {
			subentry = next + 1;

			if (!subentry->word || GET_TYPE_ID(subentry) != ix)
				continue;

			if (!first) {
				first = subentry;
				subnext = &first->next_entry;
				continue;
			}

			*subnext = subentry;
			subnext = &(*subnext)->next_entry;
		}
		*subnext = NULL;

		SET_TYPE_ID(first, 0);

		*subgroup = first;
		subgroup = &(*subgroup)->next_group;
	}
	*subgroup = NULL;

	while(first_subentry) {
		init_subentry(first_subentry);
		first_subentry = first_subentry->next_group;
	}

	return 0;
}

static int init_entry(struct cmd_entry_t **root)
{
	int ix;
	int nr_group = 0;
	uint32 *ptr;
	uint32 *pcmd_start, *pcmd_end;
	struct cmd_entry_t *entry;
	struct cmd_entry_t **group;

	extern unsigned int __cmd_start;
	extern unsigned int __cmd_end;

#ifdef WIN32
	pcmd_start = (void *)__cmd_start;
	pcmd_end = (void *)__cmd_end;
#else
	pcmd_start = &__cmd_start;
	pcmd_end = &__cmd_end;
#endif

	for (ptr = pcmd_start; ptr < pcmd_end; ptr++) {
		struct cmd_entry_t *entry = *(struct cmd_entry_t **)ptr;
		if (entry->word && !GET_TYPE_ID(entry))
			SET_TYPE_ID(entry, ++nr_group);
		if (nr_group >= MAX_GROUP_NUM)
			pr_error("it is only support %d command.\n",
				MAX_GROUP_NUM);
	}

	if (!nr_group)
		return 0;

	*root = *(struct cmd_entry_t **)pcmd_start;
	group = &(*root)->next_group;

	for (ix = 1; ix <= nr_group; ix++) {
		struct cmd_entry_t *first = NULL;
		struct cmd_entry_t **next = NULL;

		for (ptr = pcmd_start; ptr < pcmd_end; ptr++) {
			entry = *(struct cmd_entry_t **)ptr;

			if (!entry->word || GET_TYPE_ID(entry) != ix)
				continue;

			if (!first) {
				first = entry;
				next = &first->next_entry;
				continue;
			}
			
			*next = entry;
			next = &(*next)->next_entry;
		}
		*next = NULL;

		SET_TYPE_ID(first, 0);
		*group = first;
		group = &(*group)->next_group;
	}
	*group = NULL;

	for (entry = *root; entry; entry = entry->next_group)
		init_subentry(entry);

	return 0;
}

static void cmdline_init(void *ptr)
{
	init_entry(&pcmd->root);
}

static struct module_t module_cmdline = {
	.priority = MODULE_PRI_ENV,
	.init = cmdline_init,
};
MODULE(module_cmdline);
