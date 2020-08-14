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

/*****************************************************************************/

void flash_show_cmd(char *operate, uint64 address, uint32 mem, uint64 size)
{
	switch (*operate) {
	case 'R':
		printf("%s from (%s) to ", operate, u64tohhstr(address));
		printf("ddr(%s) ", u32tohhstr(mem));
		break;
	case 'W':
		printf("%s to (%s) from ", operate, u64tohhstr(address));
		printf("ddr(%s) ", u32tohhstr(mem));
		break;
	case 'E':
		printf("%s from (%s) ", operate, u64tohhstr(address));
		break;

	}
	printf("size(%s)\n", u64tohhstr(size));
}
