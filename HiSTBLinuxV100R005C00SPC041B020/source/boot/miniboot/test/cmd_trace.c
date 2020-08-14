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

#include <compile.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <command.h>
#include <memory.h>
#include <cmd-words.h>
#include <trace.h>

/*****************************************************************************/

static int do_dumpsymbol(struct cmd_t *cmd)
{
	dump_symbol();
	return 0;
}
/*****************************************************************************/

static int do_dumpstack(struct cmd_t *cmd)
{
	dump_stack();
	return 0;
}
/*****************************************************************************/

static struct cmd_word_t cw_symbol = {
	"symbol", CMDWORD_TYPE_KEYWORD, _T("")
};

CMD({&cw_dump, NULL},
    {&cw_symbol, _T("dump image all symbol"), do_dumpsymbol})
/*****************************************************************************/

static struct cmd_word_t cw_stack = {
	"stack", CMDWORD_TYPE_KEYWORD, _T("")
};

CMD({&cw_dump, NULL},
    {&cw_stack, _T("dump stack for test"), do_dumpstack})