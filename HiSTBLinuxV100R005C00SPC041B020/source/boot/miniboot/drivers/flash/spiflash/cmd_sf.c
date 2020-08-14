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

#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <command.h>
#include <cmd-words.h>
#include <sf_if.h>

#include "flash_gen.h"

#define MOULE_NAME                     "SPIFlash: "
#include <debug.h>

/*****************************************************************************/

static int do_spiflash_probe(struct cmd_t *cmd)
{
	/* only for hitools */
	return 0;
}
CMD({&cw_sf, NULL},
    {&cw_probe, _T("init spiflash"), NULL},
    {&cw_devnum, 0, do_spiflash_probe})
/*****************************************************************************/

static int do_spiflash_dump(struct cmd_t *cmd)
{
	int ret = 0;
	char buf[_16K];
	uint32 steplen;
	uint32 offset = param_int32(cmd, 2);
	uint32 length = param_int32(cmd, 3);

	ret = spiflash_check_range(offset, &length);
	if (ret)
		return ret;

	if (offset & 0xF) { /* align with 16bytes. */
		steplen = 0x10 - (offset % 0x10);
		if (steplen > length)
			steplen = length;
		ret = spiflash_read(offset, buf, steplen);
		if (ret) {
			pr_error("read failed(%d).\n", ret);
			return -1;
		}
		if (dump_hex(offset, buf, steplen, sizeof(int)))
			return 0;
		length -= steplen;
		offset += steplen;
	}

	steplen = sizeof(buf);
	while (length) {
		if (steplen > length)
			steplen = length;
		ret = spiflash_read(offset, buf, steplen);
		if (ret) {
			pr_error("read failed(%d).\n", ret);
			return -1;
		}

		if (isbreak() || dump_hex(offset, buf, steplen, sizeof(int)))
			return 0;

		length -= steplen;
		offset += steplen;
	}
	return 0;
}
CMD({&cw_sf, NULL},
    {&cw_dump, NULL},
    {&cw_offset, NULL},
    {&cw_length, 0, do_spiflash_dump})
/*****************************************************************************/

static int do_spiflash_read(struct cmd_t *cmd)
{
	int ret = 0;
	uint32 steplen;
	uint32 mem_addr = param_int32(cmd, 2);
	uint32 offset = param_int32(cmd, 3);
	uint32 length = param_int32(cmd, 4);

	ret = spiflash_check_range(offset, &length);
	if (ret)
		return ret;

	flash_show_cmd("Read", (uint64)offset, mem_addr, (uint64)length);

	steplen = get_spiflash_info()->blocksize << 2;
	while (length) {
		if (steplen > length)
			steplen = length;
		ret = spiflash_read(offset, (char*)mem_addr, steplen);
		if (ret) {
			pr_error("read failed(%d).\n", ret);
			return -1;
		}
		if (isbreak()) {
			printf("spiflash read breaking was due to Ctrl+C.\n");
			return 0;
		}
		putc('.');
		length -= steplen;
		mem_addr += steplen;
		offset += steplen;
	}
	printf("OK.\n");

	return 0;
}
CMD({&cw_sf, NULL},
    {&cw_read, NULL},
    {&cw_addr, NULL},
    {&cw_offset, NULL},
    {&cw_length, 0, do_spiflash_read})
/*****************************************************************************/

static int do_spiflash_write(struct cmd_t *cmd)
{
	int ret = 0;
	uint32 steplen;
	uint32 mem_addr = param_int32(cmd, 2);
	uint32 offset = param_int32(cmd, 3);
	uint32 length = param_int32(cmd, 4);

	ret = spiflash_check_range(offset, &length);
	if (ret)
		return ret;

	flash_show_cmd("Write", (uint64)offset, mem_addr, (uint64)length);

	steplen = get_spiflash_info()->blocksize << 2;
	while (length) {
		if (steplen > length)
			steplen = length;
		ret = spiflash_write(offset, (char*)mem_addr, steplen);
		if (ret) {
			pr_error("write failed(%d).\n", ret);
			return -1;
		}
		if (isbreak()) {
			printf("spiflash write breaking was due to Ctrl+C.\n");
			return 0;
		}
		putc('.');
		length -= steplen;
		mem_addr += steplen;
		offset += steplen;
	}
	printf("OK.\n");

	return 0;
}
CMD({&cw_sf, NULL},
    {&cw_write, NULL},
    {&cw_addr, NULL},
    {&cw_offset, NULL},
    {&cw_length, 0, do_spiflash_write})
/*****************************************************************************/

static int do_spiflash_erase(struct cmd_t *cmd)
{
	int ret = 0;
	uint32 steplen;
	uint32 offset = param_int32(cmd, 2);
	uint32 length = param_int32(cmd, 3);

	ret = spiflash_check_align(offset, &length);
	if (ret)
		return ret;

	flash_show_cmd("Erase", (uint64)offset, 0, (uint64)length);

	steplen = get_spiflash_info()->blocksize;
	while (length) {
		if (steplen > length)
			steplen = length;
		ret = spiflash_erase(offset, steplen);
		if (ret) {
			pr_error("erase failed(%d).\n", ret);
			return -1;
		}
		if (isbreak()) {
			printf("spiflash erase breaking was due to Ctrl+C.\n");
			return 0;
		}
		putc('.');
		length -= steplen;
		offset += steplen;
	}
	printf("OK.\n");

	return 0;
}
CMD({&cw_sf, NULL},
    {&cw_erase, NULL},
    {&cw_offset, NULL},
    {&cw_length, 0, do_spiflash_erase})
/*****************************************************************************/

static int do_show_spiflash(struct cmd_t *cmd)
{
	struct flash_info_t *spiinfo;

	spiinfo = get_spiflash_info();
	if (spiinfo->type != FT_SPIFLASH) {
		printf("No found SPIFlash chip.\n");
		return 0;
	}

	flash_show_info(spiinfo);

	return 0;
}
CMD({&cw_show, NULL}, {&cw_spiflash, 0, do_show_spiflash})
CMD({&cw_getinfo, NULL}, {&cw_spiflash, 0, do_show_spiflash})
