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
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <command.h>
#include <debug.h>
#include <cmd-words.h>

#define SIZE_DEFAULT                   64

/*****************************************************************************/
static uint32 param_int32_def(struct cmd_t *cmd, int index, uint32 defval)
{
	return (index >= cmd->nr_param) ? defval : param_int32(cmd, index);
}

static int do_mem_write(struct cmd_t *cmd)
{
	int ix = 0;
	uint32 addr, data;
	uint32 len = 4;
	int param_index = param_first(cmd);

	addr = param_int32(cmd, param_index++);
	if (addr & 0x03) {
		printf("%s should be aligned to 4 bytes.\n",
			param_keyword(cmd, 1));
		return 0;
	}

	data = param_int32(cmd, param_index++);
	len = param_int32_def(cmd, param_index++, 4);
	if (len & 0x03) {
		printf("%s should be aligned to 4 bytes.\n",
			param_keyword(cmd, 3));
		return 0;
	}

	while (len && !isbreak()) {
		*(volatile uint32 *)addr = data;
		addr += 4;
		len -= 4;
		if (ix++ > 0x10000) {
			ix = 0;
			putc('.');
		}
	}
	return 0;
}
CMD({&cw_mm, NULL},
    {&cw_write, NULL},
    {&cw_addr, NULL},
    {&cw_data, 0, do_mem_write},
    {&cw_length, 0, do_mem_write})

CMD({&cw_mw, NULL},
    {&cw_addr, NULL},
    {&cw_data, 0, do_mem_write},
    {&cw_length, 0, do_mem_write})
/*****************************************************************************/

static int do_mem_show(struct cmd_t *cmd)
{
	int param_index = param_first(cmd);
	uint32 addr = param_int32(cmd, param_index++);
	uint32 length = param_int32_def(cmd, param_index++, SIZE_DEFAULT);

	if (addr & 0x03 || length & 0x03) {
		printf("bad paramter. address or length should align to 4bytes.\n");
		return 0;
	}

	dump_hex(addr, (char *)addr, length, sizeof(int));

	return 0;
}
CMD({&cw_mm, NULL},
    {&cw_show, NULL}, 
    {&cw_addr, 0, do_mem_show},
    {&cw_length, 0, do_mem_show})

CMD({&cw_md, NULL},
    {&cw_addr, 0, do_mem_show},
    {&cw_length, 0, do_mem_show})
/*****************************************************************************/

static int do_mem_compare(struct cmd_t *cmd)
{
	uint32 *addr0 = (uint32 *)param_int32(cmd, 2);
	uint32 *addr1 = (uint32 *)param_int32(cmd, 3);
	uint32 size = param_int32(cmd, 4) >> 2;

	if ((uint32)addr0 & 0x03 || (uint32)addr1 & 0x03) {
		printf("bad address. address should align to 4bytes.\n");
		return 0;
	}

	while (size-- > 0 ) {
		if (*addr0 != *addr1) {
			printf("0x%08x(address:0x%08x) != 0x%08x(address:0x%08x)\n",
				*addr0, (uint32)addr0, *addr1, (uint32)addr1);
			break;
		}
		addr0++;
		addr1++;
	}

	return 0;
}
CMD({&cw_mm, NULL},
    {&cw_cmp, NULL},
    {&cw_addr, NULL},
    {&cw_addr, NULL},
    {&cw_length, 0, do_mem_compare})
/*****************************************************************************/
