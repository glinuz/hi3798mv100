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

#include <stddef.h>
#include <console.h>

#define KEY_CTRL_C                     0x03

static struct console *__current = NULL;

int getchar(void)
{
#ifdef CONFIG_DISABLE_CONSOLE_INPUT
	return 0;
#endif
	return __current ? __current->getchar() : 0;
}

void putchar(int ch)
{
	if (__current)
		__current->putchar(ch);
}

int tstchar(void)
{
#ifdef CONFIG_DISABLE_CONSOLE_INPUT
	return 0;
#endif
	if (__current)
		return __current->tstchar();
	else
		return 0;
}

int isbreak(void)
{
	if (!tstchar())
		return 0;
	return (getchar() == KEY_CTRL_C);
}

void console_init(struct console *console)
{
	__current = console;
}
