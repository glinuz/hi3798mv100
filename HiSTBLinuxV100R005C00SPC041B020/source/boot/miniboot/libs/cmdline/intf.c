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
#include <delay.h>

#include "intf.h"

/*
 * up:    0x1b 0x5b 0x41
 * down:  0x1b 0x5b 0x42
 * left:  0x1b 0x5b 0x44
 * right: 0x1b 0x5b 0x43
 */
int cl_getc(void)
{
	int chr = getchar();

	if (chr == 0x1b) {
		int wait = 20;
		while (wait-- && !tstchar())
			mdelay(1);
		if (!wait)
			return 0x1b;

		chr = getchar();
		if (chr != 0x5b)
			return chr;

		wait = 20;
		while (wait-- && !tstchar())
			mdelay(1);
		if (!wait)
			return 0x5b;
		chr = 0x8000 | getchar();
	}
	return chr;
}

void cl_putc(int ch)
{
	putc(ch);
}

void cl_puts(const char *s)
{
	puts(s);
}
