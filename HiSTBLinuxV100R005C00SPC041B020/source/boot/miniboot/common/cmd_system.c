/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by JiJiagang
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
#include <boot.h>
#include <stddef.h>
#include <command.h>
#include <cmd-words.h>

static int do_reset_system(struct cmd_t *cmd)
{
	reboot();
	return 0;
}

CMD({&cw_reboot, 0, do_reset_system})
CMD({&cw_reset, 0, do_reset_system})
