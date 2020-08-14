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

#ifndef COMMANDH
#define COMMANDH

#include <stddef.h>

struct cmd_t;

typedef int (*fn_do_cmd)(struct cmd_t *cmd);

struct cmd_word_t {
#define MAX_KEYWORD_SIZE     32 
	const char *keyword;
	unsigned int type;
	const char *help;
};

/* command word */
#define CMDWORD_TYPE_KEYWORD                     0x01

/* command parameter */
#define CMDWORD_TYPE_SIZE                        0x02
#define CMDWORD_TYPE_STRING                      0x04
#define CMDWORD_TYPE_IPADDR                      0x08
#define CMDWORD_TYPE_MAC                         0x10

#define CMDWORD_TYPE_MASK_PARAM \
	(CMDWORD_TYPE_SIZE | CMDWORD_TYPE_STRING | \
	 CMDWORD_TYPE_IPADDR | CMDWORD_TYPE_MAC)

#define CMDWORD_TYPE_MASK                        0x1F

struct cmd_entry_t {
	struct cmd_word_t *word;
	const char *help;
	fn_do_cmd do_cmd;
#define MAX_GROUP_NUM         0xFF
	struct cmd_entry_t *next_group; /* next different cmd word */
	struct cmd_entry_t *next_entry; /* next same cmd word */
};

struct cmd_parm_t {
	union {
		uint64 val64;
		char *ipaddr;
		uint8 mac[6];
	};
	char *str;
	char *keyword;
};

#define MAX_CMD_WORD          8     /* size of keyword */
#define MAX_CMD_PARAMS        10    /* max number of param */
#define MAX_CMD_LENGTH        1024  /* max size of cmd line */

struct cmd_t {
	int nr_param;
	struct cmd_parm_t params[MAX_CMD_PARAMS];

	/* this parsed entry, point to the array begin address */
	struct cmd_entry_t *entry;

	struct cmd_entry_t *root;  /* all cmd entry */
	fn_do_cmd do_cmd;
	char cmdline[MAX_CMD_LENGTH];
};

int run_cmd(char *cmdline, int length);

int param_first(struct cmd_t *cmd);

uint64 param_int64(struct cmd_t *cmd, int index);
uint32 param_int32(struct cmd_t *cmd, int index);
char *param_str(struct cmd_t *cmd, int index);
char *param_ipaddr(struct cmd_t *cmd, int index);
uint8 *param_mac(struct cmd_t *cmd, int index);

#if 0
static uint64 param_int64_def(struct cmd_t *cmd, int index, uint64 defval)
{
	return (index >= cmd->nr_param) ? defval : param_int64(cmd, index);
}

static uint32 param_int32_def(struct cmd_t *cmd, int index, uint32 defval)
{
	return (index >= cmd->nr_param) ? defval : param_int32(cmd, index);
}

static char *param_str_def(struct cmd_t *cmd, int index, char *defval)
{
	return (index >= cmd->nr_param) ? defval : param_str(cmd, index);
}

static char *param_ipaddr_def(struct cmd_t *cmd, int index, char *defval)
{
	return (index >= cmd->nr_param) ? defval : param_ipaddr(cmd, index);
}

static uint8 *param_mac_def(struct cmd_t *cmd, int index, uint8 *defval)
{
	return (index >= cmd->nr_param) ? defval : param_mac(cmd, index);
}
#endif
const char *param_keyword(struct cmd_t *cmd, int index);
inline int param_count(struct cmd_t *cmd);

#define __pcmd_name(line) __pcmd_entry_##line
#define _pcmd_name(line) __pcmd_name(line)
#define __ppcmd_name(line) __ppcmd_entry_##line
#define _ppcmd_name(line) __ppcmd_name(line)

#ifdef CONFIG_CMDLINE
#define CMD(...) \
	static struct cmd_entry_t _pcmd_name(__LINE__) [] = { __VA_ARGS__, {0,0}}; \
	const static __attribute__ ((section("._cmd"),used)) \
		struct cmd_entry_t *_ppcmd_name(__LINE__) = _pcmd_name(__LINE__);

#ifdef CONFIG_PRINT
#define CONFIG_PROMPT  "miniboot# "
#else
#define CONFIG_PROMPT ""
#endif

#else
#define CMD(...)
#define CONFIG_PROMPT ""
#endif

#endif /* COMMANDH */
