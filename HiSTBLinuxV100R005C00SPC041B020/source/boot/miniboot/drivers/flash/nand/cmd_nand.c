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

#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <malloc.h>
#include <cmd-words.h>

#include <flash.h>
#include <nfc_if.h>

#include "nand_ctrl.h"
#include "nand_chip.h"
#include "nand_drv.h"
#include "flash_gen.h"

#define MOULE_NAME                     "NAND: "
#include <debug.h>

/*****************************************************************************/
static uint64 param_int64_def(struct cmd_t *cmd, int index, uint64 defval)
{
	return (index >= cmd->nr_param) ? defval : param_int64(cmd, index);
}

/*
 *
 * nand dump <address>
 * nand dump oob <address>
 */
static int do_nand_dump(struct cmd_t *cmd)
{
	int ret = 0;
	char *buf;
	int onlyoob = 0;
	uint64 address, real_address;
	int param_index = param_first(cmd);
	struct flash_info_t *flash_info = get_nand_info();

	if (!flash_info->pagesize) {
		printf("No found NAND chip.\n");
		return 0;
	}

	onlyoob = (param_keyword(cmd, param_index - 1) == cw_oob.keyword);
	real_address = address = param_int64(cmd, param_index++);

	ret = nand_check_align64(address, 0, NULL, FALSE);
	if (ret)
		return ret;

	buf = malloc(flash_info->pagesize + flash_info->oobused);
	if (!buf) {
		pr_error("can't malloc memory.\n");
		return -1;
	}

	nand_read64(&address, 0, buf, (uint64)flash_info->pagesize, NULL,
		    TRUE, FALSE);

	if (onlyoob) {
		printf("dump nand oob address 0x%08llx oobsize %d:\n",
		       real_address, flash_info->oobused);
	} else {
		printf("dump nand page address 0x%08llx pagesize %s:\n",
		       real_address,
		       u32tohstr(flash_info->pagesize, NULL));

		dump_hex(0, buf, flash_info->pagesize, sizeof(int));

		printf("oobsize 0x%08x:\n", flash_info->oobused);
	}

	dump_hex(0, buf + flash_info->pagesize, flash_info->oobused, sizeof(int));

	free(buf);

	return 0;
}
CMD({&cw_nand, NULL},
    {&cw_dump, NULL},
    {&cw_offset, 0, do_nand_dump})

CMD({&cw_nand, NULL},
    {&cw_dump, NULL},
    {&cw_oob, _T("only dump oob data"), NULL},
    {&cw_offset, 0, do_nand_dump})

/*****************************************************************************/

static int check_align(struct flash_info_t *info, uint64 address,
		       uint64 *size, int withoob)
{
	if (withoob) {
		int sz_pageoob = info->pagesize + info->oobused;

		if ((*size) % sz_pageoob) {
			pr_error("The size(0x%llx) does not aligned to pagesize(0x%x) + oobsize(0x%x)\n",
				  *size, info->pagesize, info->oobused);
			return -1;
		}
		*size = (*size) / sz_pageoob * info->pagesize;
	}

	return nand_check_align64(address, 0, size, FALSE);
}
/*****************************************************************************/
/*
 * @size - size alibn to pagesize.
 * nand read <address> <offset> <size>
 * nand read withbb <address> <offset> <size>
 *
 * @size - size align to pagesize + oobsize.
 * nand read withoob <address> <offset> <size>
 * nand read.yaffs <address> <offset> <size>
 */
static int do_nand_read(struct cmd_t *cmd)
{
	int ret = 0;
	uint64 steplen;
	int withoob, skipbad;

	uint32 oobsize;
	int param_index = param_first(cmd);
	const char *prev_cmd = param_keyword(cmd, param_index - 1);
	uint32 mem_addr = param_int32(cmd, param_index++);
	uint64 readstart = param_int64(cmd, param_index++);
	uint64 readsize = param_int64(cmd, param_index++);
	uint64 address = readstart;
	uint64 size;

	struct flash_info_t *flash_info = get_nand_info();

	if (!flash_info->pagesize) {
		pr_error("No found NAND chip.\n");
		return 0;
	}

	skipbad = !(prev_cmd == cw_withbb.keyword);
	withoob = (prev_cmd == cw_read_yaffs.keyword ||
		prev_cmd == cw_withoob.keyword);

	ret = check_align(flash_info, readstart, &readsize, withoob);
	if (ret)
		return ret;
	size = readsize;

	flash_show_cmd("Read", address, mem_addr, size);

	steplen = (uint64)(flash_info->blocksize << 3);
	while (size) {
		if (steplen > size)
			steplen = size;
		ret = nand_read64(&address, 0, (char*)mem_addr, steplen,
				  &oobsize, withoob, skipbad);
		if (ret < 0) {
			pr_error("read failed(%d).\n", ret);
			return -1;
		}
		if (isbreak()) {
			printf("nand read breaking was due to Ctrl+C.\n");
			return 0;
		}
		putc('.');
		size -= steplen;
		mem_addr += steplen;
		if (withoob)
			mem_addr += oobsize;
	}

	pr_tool("pure data length is %llu, len_incl_bad is %llu\n",
		readsize, address - readstart);

	printf("OK.\n");

	return 0;
}
CMD({&cw_nand, NULL},
    {&cw_read, NULL},
    {&cw_addr, NULL},
    {&cw_offset, NULL},
    {&cw_size, 0, do_nand_read})

CMD({&cw_nand, NULL},
    {&cw_read, NULL},
    {&cw_withoob, _T("read NAND, include oob data."), NULL},
    {&cw_addr, NULL},
    {&cw_offset, NULL},
    {&cw_size, 0, do_nand_read})

CMD({&cw_nand, NULL},
    {&cw_read_yaffs, _T("read NAND, include oob data, used for read yaffs2."), NULL},
    {&cw_addr, NULL},
    {&cw_offset, NULL},
    {&cw_size, _T("read size, should align to pagesize + oobsize"), do_nand_read})

CMD({&cw_nand, NULL},
    {&cw_read, NULL},
    {&cw_withbb, _T("read NAND, don't skip bad block."), NULL},
    {&cw_addr, NULL},
    {&cw_offset, NULL},
    {&cw_size, 0, do_nand_read})
/*****************************************************************************/
/*
 * nand write <address> <offset> <size>
 * nand write skipempty <address> <offset> <size>
 * nand write ubi <address> <offset> <size>
 * nand write withoob <address> <offset> <size>
 * nand write yaffs <address> <offset> <size>
 */
static int do_nand_write(struct cmd_t *cmd)
{
	int ret = 0;
	uint64 steplen;
	int withoob, skipempty;

	uint32 oobsize;
	int param_index = param_first(cmd);
	const char *prev_cmd = param_keyword(cmd, param_index - 1);
	uint32 mem_addr = param_int32(cmd, param_index++);
	uint64 readstart = param_int64(cmd, param_index++);
	uint64 readsize = param_int64(cmd, param_index++);
	uint64 address = readstart;
	uint64 size;

	struct flash_info_t *flash_info = get_nand_info();

	if (!flash_info->pagesize) {
		printf("No found NAND chip.\n");
		return 0;
	}

	withoob = (prev_cmd == cw_write_yaffs.keyword ||
		prev_cmd == cw_withoob.keyword);

	skipempty = (prev_cmd == cw_skipempty.keyword ||
		prev_cmd == cw_ubi.keyword);

	ret = check_align(flash_info, readstart, &readsize, withoob);
	if (ret)
		return ret;
	size = readsize;

	flash_show_cmd("Write", address, mem_addr, size);

	steplen = (uint64)(flash_info->blocksize << 3);
	while (size) {
		if (steplen > size)
			steplen = size;

		ret = nand_write64(&address, 0, (char*)mem_addr, steplen,
				   &oobsize, withoob, skipempty);
		if (ret < 0) {
			pr_error("write failed(%d).\n", ret);
			return -1;
		}
		if (isbreak()) {
			printf("nand write breaking was due to Ctrl+C.\n");
			return 0;
		}
		putc('.');
		size -= steplen;
		mem_addr += steplen;
		if (withoob)
			mem_addr += oobsize;
	}

	pr_tool("pure data length is %llu, len_incl_bad is %llu\n",
		readsize, address - readstart);

	printf("OK.\n");

	return 0;
}
CMD({&cw_nand, NULL},
    {&cw_write, NULL},
    {&cw_addr, NULL},
    {&cw_offset, NULL},
    {&cw_size, 0, do_nand_write})

CMD({&cw_nand, NULL},
    {&cw_write, NULL},
    {&cw_withoob, _T("write to NAND, include oob data."), NULL},
    {&cw_addr, NULL},
    {&cw_offset, NULL},
    {&cw_size, 0, do_nand_write})

CMD({&cw_nand, NULL},
    {&cw_write_yaffs, _T("write to NAND, include oob data. used for write yaffs2"), NULL},
    {&cw_addr, NULL},
    {&cw_offset, NULL},
    {&cw_size, _T("write size, should align to pagesize + oobsize"), do_nand_write})

CMD({&cw_nand, NULL},
    {&cw_write, NULL},
    {&cw_skipempty, _T("write to NAND, skip empty page."), NULL},
    {&cw_addr, NULL},
    {&cw_offset, NULL},
    {&cw_size, 0, do_nand_write})

CMD({&cw_nand, NULL},
    {&cw_write, NULL},
    {&cw_ubi, _T("write to NAND, skip empty page, the same as skipempty."), NULL},
    {&cw_addr, NULL},
    {&cw_offset, NULL},
    {&cw_size, 0, do_nand_write})
/*****************************************************************************/
/*
 * nand erase <address> <size>
 * nand scrub <address> <size>
 */
static int do_nand_erase(struct cmd_t *cmd)
{
	int ret = 0;
	uint64 steplen;
	int force;

	uint64 address, size;
	int param_index = param_first(cmd);
	const char *prev_cmd = param_keyword(cmd, param_index - 1);

	struct flash_info_t *flash_info = get_nand_info();

	if (!flash_info->pagesize) {
		pr_error("No found NAND chip.\n");
		return 0;
	}

	force = (prev_cmd == cw_scrub.keyword);
	address = param_int64_def(cmd, param_index++, 0);
	size = param_int64_def(cmd, param_index++,
		flash_info->chipsize * flash_info->nr_chips);

	ret = nand_check_align64(address, 0, &size, TRUE);
	if (ret)
		return ret;

	flash_show_cmd("Erase", address, 0, size);

	steplen = (uint64)(flash_info->blocksize << 3);
	while (size) {
		if (steplen > size)
			steplen = size;
		ret = (int)nand_erase_block64(address, address + steplen, 
					      steplen, force);
		if (ret < 0) {
			pr_error("erase failed(%d).\n", ret);
			return -1;
		}
		if (isbreak()) {
			printf("nand erase breaking was due to Ctrl+C.\n");
			return 0;
		}
		putc('.');
		address += steplen;
		size -= steplen;
	}
	printf("OK.\n");

	return 0;
}
CMD({&cw_nand, NULL},
    {&cw_scrub, _T("erase NAND block, even if the block is bad."), NULL},
    {&cw_addr, 0, NULL},
    {&cw_size, 0, do_nand_erase})

CMD({&cw_nand, NULL},
    {&cw_erase, _T("erase NAND block, skip bad block."), NULL},
    {&cw_addr, 0, NULL},
    {&cw_size, 0, do_nand_erase})
/*****************************************************************************/
/*
 * nand show
 * show nand
 */
static int do_nand_show(struct cmd_t *cmd)
{
	struct flash_info_t *flash_info = get_nand_info();

	if (!flash_info->pagesize) {
		printf("No found NAND chip.\n");
		return 0;
	}

	flash_show_info(flash_info);

	return 0;
}
CMD({&cw_show, NULL}, {&cw_nand, _T("show NAND information."), do_nand_show})
CMD({&cw_getinfo, NULL}, {&cw_nand, _T("show NAND information."), do_nand_show})
/*****************************************************************************/
/*
 * nand bad mark <address>
 */
static int do_nand_bad_mark(struct cmd_t *cmd)
{
	uint64 address;
	int param_index = param_first(cmd);
	struct flash_info_t *info = get_nand_info();

	if (!info->pagesize) {
		printf("No found NAND chip.\n");
		return 0;
	}

	address = param_int64(cmd, param_index++);

	if (nand_check_align64(address, 0, NULL, TRUE))
		return 0;

	printf("Mark NAND block (%s) bad...", u64tohhstr(address));

	if (nand_mark_bad64(address))
		printf("fail.\n");
	else
		printf("ok.\n");

	return 0;
}
CMD({&cw_nand, NULL},
    {&cw_bad, NULL},
    {&cw_mark, _T("mark NAND block as bad block."), NULL},
    {&cw_addr, 0, do_nand_bad_mark})

/*****************************************************************************/
/*
 * nand bad show <address>
 */
static int do_nand_bad_show(struct cmd_t *cmd)
{
	uint64 address;
	int param_index = param_first(cmd);
	struct flash_info_t *info = get_nand_info();

	if (!info->pagesize) {
		printf("No found NAND chip.\n");
		return 0;
	}

	address = param_int64_def(cmd, param_index++, 0);

	address &= ~(info->blocksize - 1); // TODO: º”÷π blockmask

	printf("Show NAND bad block from (%s):\n", u64tohhstr(address));

	while (address < info->chipsize * info->nr_chips) {
		if (nand_bad_block64(address))
			printf("%s is bad block.\n", u64tohhstr(address));
		address += info->blocksize;

		if (isbreak()) {
			printf("breaking was due to Ctrl+C.\n");
			break;
		}
	}

	return 0;
}
CMD({&cw_nand, NULL},
    {&cw_bad, NULL},
    {&cw_show, _T("show NAND bad block."), do_nand_bad_show},
    {&cw_addr, 0, do_nand_bad_show})

CMD({&cw_show, NULL},
    {&cw_nand, NULL},
    {&cw_bad, _T("show NAND bad block."), do_nand_bad_show},
    {&cw_addr, 0, do_nand_bad_show})

