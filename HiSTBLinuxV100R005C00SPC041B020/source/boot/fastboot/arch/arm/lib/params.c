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
#include <common.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <malloc.h>
/******************************************************************************/

static char *skip_spaces(const char *str)
{
	while (isspace(*str))
		++str;
	return (char *)str;
}
/******************************************************************************/

static char *next_arg(char *args, char **param, char **val)
{
	unsigned int i;
	unsigned int equals = 0;
	int in_quote = 0;
	int quoted = 0;
	char *next;

	if (*args == '"') {
		args++;
		in_quote = 1;
		quoted = 1;
	}

	for (i = 0; args[i]; i++) {
		if (isspace(args[i]) && !in_quote)
			break;
		if (equals == 0) {
			if (args[i] == '=')
				equals = i;
		}
		if (args[i] == '"')
			in_quote = !in_quote;
	}

	*param = args;
	if (!equals)
		*val = NULL;
	else {
		args[equals] = '\0';
		*val = args + equals + 1;

		if (**val == '"') {
			(*val)++;
			if (args[i-1] == '"')
				args[i-1] = '\0';
		}
		if (quoted && args[i-1] == '"')
			args[i-1] = '\0';
	}

	if (args[i]) {
		args[i] = '\0';
		next = args + i + 1;
	} else
		next = args + i;

	return skip_spaces(next);
}
/******************************************************************************/

struct param_val_t {
	char *param;
	char *val;
};
/******************************************************************************/

static int parse_args(char *args, struct param_val_t *pv, int nr_pv)
{
	int ix;

	args = skip_spaces(args);

	for (ix = 0; *args && ix < nr_pv; ix++)
		args = next_arg(args, &pv[ix].param, &pv[ix].val);

	if (ix >= nr_pv)
		printf("params: too many params, max of params is %d.\n", nr_pv);

	return ix;
}
/******************************************************************************/

static int find_param_val(struct param_val_t *pv, int nr_pv, char *param,
			  char *val)
{
	int sz_param, sz_val = 0;

	sz_param = strlen(param) + 1;
	if (val)
		sz_val = strlen(val) + 1;

	while (nr_pv-- > 0) {
		if (!strncmp(pv[nr_pv].param, param, sz_param)) {
			if (!val || !strncmp(pv[nr_pv].val, val, sz_val))
				return 1; /* equate */

			return 0; /* conflict */
		}
	}
	return -1; /* no found */
}
/******************************************************************************/

char *merge_args(const char *args, const char *args_merge, char *args_name,
		 char *args_merge_name)
{
	int ix;
	int ret;
	int isprint;
	int count;
	int nr_args;
	int nr_args_merge;
	char *tmp_args = NULL;
	char *tmp_args_merge = NULL;
	char *cmdline = NULL;
	char *pcmdline = NULL;
	const int max_pv = 200;

	struct param_val_t *pv = NULL;
	struct param_val_t *pv_merge = NULL;
	int nr_pv, nr_pv_merge;

	if (!args & !args_merge)
		return NULL;

	if (!args)
		args = "";
	
	if (!args_merge)
		args_merge = "";

	nr_args = strlen(args) + 1;
	nr_args_merge = strlen(args_merge) + 1;

	tmp_args = (char *)malloc(nr_args + nr_args_merge);
	if (!tmp_args) {
		printf("params: Out of memory.\n");
		return NULL;
	}
	strncpy(tmp_args, args, nr_args);

	tmp_args_merge = tmp_args + nr_args;
	strncpy(tmp_args_merge, args_merge, nr_args_merge);

	count = nr_args + nr_args_merge;
	cmdline = (char *)malloc(count);
	if (!cmdline) {
		printf("params: Out of memory.\n");
		goto fail;
	}

	pv = (struct param_val_t *)malloc(sizeof(struct param_val_t) *
					  max_pv * 2);
	if (!pv) {
		printf("params: Out of memory.\n");
		goto fail;
	}
	pv_merge = pv + max_pv;

	pcmdline = cmdline;
	strncpy(pcmdline, args, nr_args);
	pcmdline += nr_args - 1;
	count -= nr_args - 1;

	nr_pv = parse_args(tmp_args, pv, max_pv);
	nr_pv_merge = parse_args(tmp_args_merge, pv_merge, max_pv);

	isprint = 0;
	for (ix = 0; ix < nr_pv_merge; ix++) {
		char *param, *val;

		/* for echo param in 'args_merge' */
		param = pv_merge[ix].param;
		val = pv_merge[ix].val;

		/* search param in 'args' */
		ret = find_param_val(pv, nr_pv, param, val);
		if (ret == 1) { /* equate */
			continue;
		} else if (ret == 0) { /* conflict */
			if (isprint == 0) {
				isprint = 1;
				printf("There are some conflict between \"%s\" and \"%s\".\n",
				       args_name, args_merge_name);
				printf("The param \"");
			}

			printf("%s ", param);

			continue;
		} else { /* no found */
			int num;
			char *space = (pcmdline == cmdline) ? "" : " ";

			num = snprintf(pcmdline, count, "%s%s", space, param);
			count -= num;
			pcmdline += num;

			if (val) {
				num = snprintf(pcmdline, count, "=%s", val);
				count -= num;
				pcmdline += num;
			}
		}
	}

	if (isprint == 1)
		printf("\" in \"%s\" will be ignored.\n\n", args_merge_name);

fail:
	if (tmp_args)
		free(tmp_args);

	if (!pcmdline && cmdline) {
		free(cmdline);
		cmdline = NULL;
	}

	if (pv)
		free(pv);

	return cmdline;
}
/******************************************************************************/
