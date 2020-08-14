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
#include <cmd-words.h>
#include <debug.h>

#include "parse.h"
#include "linepro.h"
#include "sort.h"

/*****************************************************************************/

static struct cmd_entry_t *find_cmd_group(struct cmd_entry_t *entry,
					  const char *cmdline)
{
	int type;
	const char *keyword;

	for (; entry; entry = entry->next_group) {
		if (!entry->word)
			continue;

		type = (entry->word->type & CMDWORD_TYPE_MASK);

		if (!type)
			continue;

		keyword = entry->word->keyword;
		if (type == CMDWORD_TYPE_KEYWORD &&
		    strncmp(keyword, cmdline, MAX_CMD_WORD))
			continue;

		return entry;
	}
	return NULL;
}
/*****************************************************************************/

static int find_cmd_entry(const struct cmd_t *cmd, struct cmd_entry_t **entry)
{
	int level;
	struct cmd_entry_t *gourp = cmd->root;

	for (level = 0; level < cmd->nr_param; level++, gourp++) {
		const char *keyword = cmd->params[level].keyword;

		*entry = find_cmd_group(gourp, keyword);
		if (!*entry) {
			if (!gourp->next_group)
				return -EAGAIN; /* input too many parameter */
			else
				return -EINVAL; /* no find this command */
		}
		gourp = *entry;
	}

	return 0;
}
/*****************************************************************************/

struct pptable_t {
	uint32 type;
	int (*parse)(struct cmd_parm_t *param);
};
/*****************************************************************************/

static int pp_str(struct cmd_parm_t *param)
{
	param->str = param->keyword;
	return 0;
}
/*****************************************************************************/

static int pp_size(struct cmd_parm_t *param)
{
	char *retptr;
	param->val64 = memparse(param->keyword, &retptr);
	if (*retptr != '\0') {
		printf("Input invalid parameter\n");
		return -EINVAL;
	}
	return 0;
}
/*****************************************************************************/

static int pp_ipaddr(struct cmd_parm_t *param)
{
	char *retptr;
	int loop = 4;
	char *ptr = param->keyword;

	while (loop--) {
		int val = (int)memparse(ptr, &retptr);

		if (val < 0 || val > 255)
			goto fail;

		if (*retptr != '\0' && *retptr != '.')
			goto fail;

		if (!loop || !*retptr)
			break;

		ptr = retptr + 1;
	}

	if (loop || *retptr)
		goto fail;

	param->ipaddr = param->keyword;

	return 0;
fail:
	printf("Input invalid ip address.\n");
	return -EINVAL;
}
/*****************************************************************************/

static int pp_mac(struct cmd_parm_t *param)
{
	int loop = 6;
	uint8 *mac = param->mac;
	char *ptr = param->keyword;

	while (loop-- > 0) {
		int val = tohex(*ptr);
		if (val < 0)
			goto fail;
		*mac = val;
		ptr++;

		val = tohex(*ptr);
		if (val < 0)
			goto fail;
		*mac = ((*mac << 4) | val);
		ptr++;

		if (*ptr != ':')
			break;
		ptr++;
		mac++;
	}

	if (*ptr || loop)
		goto fail;

	return 0;
fail:
	printf("Input invalid ethernet mac.\n");
	return -EINVAL;
}
/*****************************************************************************/

static struct pptable_t pptabls[] = {
	{
		.type = CMDWORD_TYPE_SIZE,
		.parse = pp_size,
	}, {
		.type = CMDWORD_TYPE_STRING,
		.parse = pp_str,
	}, {
		.type = CMDWORD_TYPE_IPADDR,
		.parse = pp_ipaddr,
	}, {
		.type = CMDWORD_TYPE_MAC,
		.parse = pp_mac,
	}, 
	{ 0 },
};
/*****************************************************************************/

static int parse_param(struct cmd_t *cmd, struct cmd_entry_t *entry)
{
	int level = cmd->nr_param;
	struct pptable_t *ppt;

	cmd->do_cmd = entry->do_cmd;
	while (level > 0) {
		int type = (entry--)->word->type;
		struct cmd_parm_t *param = &cmd->params[--level];

		ppt = pptabls;
		while (ppt->parse) {
			if (ppt->type & type) {
				if (ppt->parse(param))
					return -EINVAL;
				param->str = param->keyword;
				break;
			}
			ppt++;
		}
	}
	cmd->entry = entry + 1;

	return 0;
}
/*****************************************************************************/

static int parse_entry(struct cmd_t *cmd)
{
	int ret;
	struct cmd_entry_t *entry;

	ret = find_cmd_entry(cmd, &entry);
	switch (ret) {
	case -EAGAIN:
		printf("Input too many parameter.\n");
		return ret;
	case -EINVAL:
		printf("No find this command.\n");
		return ret;
	}

	/* suppose at least one entry has do_cmd */
	while (entry && !entry->do_cmd)
		entry = entry->next_entry;

	if (!entry) {
		printf("Command is incomplete.\n");
		return -EINVAL;
	}

	ret = parse_param(cmd, entry);
	if (ret)
		return ret;

	return 0;
}
/*****************************************************************************/

static int parse_cmd_word(struct cmd_t *cmd)
{
	char dem;
	int nr_param = 0;
	struct cmd_parm_t *param;
	char *ptr = cmd->cmdline;

	while (*ptr && nr_param < MAX_CMD_PARAMS) {
		while (isspace(*ptr))
			ptr++;
		if (!*ptr)
			break;

		dem = (*ptr == '\'' || *ptr == '\"') ? *ptr++ : 0;

		param = &cmd->params[nr_param++];
		param->keyword = ptr;

		if (dem) {
			while (*ptr && *ptr != dem)
				ptr++;
		} else {
			while (*ptr && !isspace(*ptr))
				ptr++;
		}

		if (*ptr)
			*ptr++ = 0;
	}
	cmd->nr_param = nr_param;
	return nr_param;
}
/*****************************************************************************/

int show_cmd_help(struct cmd_t *cmd)
{
	int ret;
	struct cmd_entry_t *entry, *parent;

	parse_cmd_word(cmd);

	if (cmd->nr_param) {
		ret = find_cmd_entry(cmd, &entry);
		if (ret)
			return ret;
		parent = entry++;
	} else {
		entry = cmd->root;
		parent = NULL;
	}

	sort_clear();
	for (; entry; entry = entry->next_group) {
		if (!entry->word)
			continue;
		sort_add(entry);
	}

	pr_cmd_help(parent);

	return 0;
}
/*****************************************************************************/

int fill_cmd_word(struct cmd_t *cmd, char *output, int *length)
{
	int ret;
	int minlen = 0;
	int input_len;
	char *input;
	char keyword[MAX_CMD_WORD];
	struct cmd_entry_t *first;
	struct cmd_entry_t *entry;
	struct cmd_entry_t *parent;

	parse_cmd_word(cmd);
	if (!cmd->nr_param)
		return -EINVAL;

	input = cmd->params[--cmd->nr_param].keyword;

	if (cmd->nr_param) {
		ret = find_cmd_entry(cmd, &entry);
		if (ret)
			return ret;
		parent = entry++;
	} else {
		entry = cmd->root;
		parent = NULL;
	}

	first = NULL;
	input_len = strlen(input);

	for (; entry; entry = entry->next_group) {
		int keylen;
		struct cmd_word_t *word = entry->word;

		if (!word || !(word->type & CMDWORD_TYPE_KEYWORD))
			continue;

		keylen = strlen(word->keyword);
		if (input_len > keylen)
			continue;

		if (memcmp(input, word->keyword, input_len))
			continue;

		if (first)
			break;

		first = entry;
		minlen = keylen;
	}

	if (!first)
		return -EINVAL;

	/* only find one entry: 'first' */
	if (!entry) {
		if (minlen > input_len) {
			strncpy(output, first->word->keyword + input_len,
				*length);
			output[*length - 1] = 0;
		}
		*length = (minlen - input_len);
		return 0;
	}

	/* find at least 2 entry: 'first', 'entry' */
	strncpy(keyword, first->word->keyword, sizeof(keyword));
	keyword[sizeof(keyword) - 1] = '\0';
	minlen = min(sizeof(keyword), minlen);

	printf("\n");
	sort_clear();
	sort_add(first);

	for (; entry; entry = entry->next_group) {
		int ix;
		int keylen;
		struct cmd_word_t *word = entry->word;

		if (!word || !(word->type & CMDWORD_TYPE_KEYWORD))
			continue;

		keylen = strlen(word->keyword);
		if (input_len > keylen)
			continue;

		if (memcmp(input, word->keyword, input_len))
			continue;

		for (ix = input_len; ix < minlen; ix++) {
			if (keyword[ix] != word->keyword[ix])
				break;
		}
		if (ix < minlen)
			minlen = ix;

		sort_add(entry);
	}

	pr_cmd_help(parent);

	if (minlen > input_len) {
		strncpy(output, first->word->keyword + input_len,
			*length);
		output[*length - 1] = 0;
	}
	*length = (minlen - input_len);

	return -EEXIST;
}
/*****************************************************************************/

int run_one_cmd(struct cmd_t *cmd)
{
	int ret;

	ret = parse_cmd_word(cmd);
	if (!ret)
		return 0;

	ret = parse_entry(cmd);
	if (ret)
		return ret;

	ret = cmd->do_cmd(cmd);
	if (ret)
		printf("Run command fail. code(%d)\n", ret);
	return ret;
}
/*****************************************************************************/
/* cmd is split by ";" */
int run_mutli_cmd(struct cmd_t *cmd, char *cmdline, int length)
{
	int ret = 0;
	int size;
	char *ptr;
	char *next = cmdline - 1;

	if (!*cmdline || !length)
		return 0;

	do {
		next++;
		size = length + cmdline - next;
		if (size <= 0)
			return 0;

		ptr = get_next_cmd(&next, size);
		if (!ptr)
			break;
		size = min_t(unsigned int, (next - ptr),
			     sizeof(cmd->cmdline)-1);
		strncpy(cmd->cmdline, ptr, size);
		cmd->cmdline[size] = '\0';

		ret = run_one_cmd(cmd);
		if (ret)
			break;
	} while (*next);

	return ret;
}
/*****************************************************************************/

int run_cmd(char *cmdline, int length)
{
	return run_mutli_cmd(pcmd, cmdline, length);
}
/*****************************************************************************/
/* get first parameter position */
int param_first(struct cmd_t *cmd)
{
	int ix = 0;

	/* first parameter is command, not parameter. */
	while (++ix < cmd->nr_param) {
		if (cmd->entry[ix].word->type & CMDWORD_TYPE_MASK_PARAM)
			break;
	}
	return ix;
}
/*****************************************************************************/

uint64 param_int64(struct cmd_t *cmd, int index)
{
	ASSERT(index >= 0 && index < cmd->nr_param);
	return (cmd->params[index]).val64;
}
/*****************************************************************************/

uint32 param_int32(struct cmd_t *cmd, int index)
{
	ASSERT(index >= 0 && index < cmd->nr_param);
	return (uint32)(cmd->params[index]).val64;
}
/*****************************************************************************/

char *param_str(struct cmd_t *cmd, int index)
{
	ASSERT(index >= 0 && index < cmd->nr_param);
	return (cmd->params[index]).str;
}
/*****************************************************************************/

char *param_ipaddr(struct cmd_t *cmd, int index)
{
	ASSERT(index >= 0 && index < cmd->nr_param);
	return (cmd->params[index]).ipaddr;
}
/*****************************************************************************/

uint8 *param_mac(struct cmd_t *cmd, int index)
{
	ASSERT(index >= 0 && index < cmd->nr_param);
	return (cmd->params[index]).mac;
}
/*****************************************************************************/

const char *param_keyword(struct cmd_t *cmd, int index)
{
	ASSERT(index >= 0 && index < cmd->nr_param);
	return (cmd->entry[index]).word->keyword;
}
/*****************************************************************************/

int param_count(struct cmd_t *cmd)
{
	return (cmd->nr_param);
}
