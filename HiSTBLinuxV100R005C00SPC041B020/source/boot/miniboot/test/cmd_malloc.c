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
#include <stddef.h>
#include <cmd-words.h>
#include <command.h>
#include <malloc.h>

static int do_dump_malloc(struct cmd_t *cmd)
{
	dump_malloc();
	return 0;
}
/*****************************************************************************/

static struct cmd_word_t cw_malloc = {
	"malloc", CMDWORD_TYPE_KEYWORD, NULL,
};

CMD({&cw_test, NULL},
    {&cw_dump, NULL},
    {&cw_malloc, _T("dump memory"), do_dump_malloc})

