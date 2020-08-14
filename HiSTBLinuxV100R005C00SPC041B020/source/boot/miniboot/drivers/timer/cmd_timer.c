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
#include <command.h>
#include <cmd-words.h>

/*****************************************************************************/

static unsigned int tick_value = 0;

static int do_show_timer(struct cmd_t *cmd)
{
	unsigned int sec, msec, usec;
	unsigned int this_tick = get_ticks();

	if (tick_value <= this_tick)
		usec = this_tick - tick_value;
	else
		usec = 0xFFFFFFFF - tick_value + this_tick;

	msec = usec / 1000;
	usec = usec % 1000;
	sec = msec / 1000;
	msec = msec % 1000;

	printf("%u.%u.%u\n", sec, msec, usec);

	return 0;
}
CMD({&cw_show, NULL},
    {&cw_timer, _T("show current timer value"), do_show_timer})

static int do_reset_timer(struct cmd_t *cmd)
{
	tick_value = get_ticks();

	return 0;
}
CMD({&cw_reset, NULL},
    {&cw_timer, _T("Reset timer value to 0"), do_reset_timer})
