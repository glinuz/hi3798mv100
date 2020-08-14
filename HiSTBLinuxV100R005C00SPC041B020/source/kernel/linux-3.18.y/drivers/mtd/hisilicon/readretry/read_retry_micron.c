/******************************************************************************
 *  Copyright (C) 2015 Cai Zhiyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiying 2015.7.27
 *
******************************************************************************/

#include <linux/mtd/nand.h>
#include <hiflash.h>

/******************************************************************************/

#define MAX_MICRON_READ_RETRIES        8
/*****************************************************************************/

static int setup_micron(struct nand_read_retry_info *info, int retry_mode)
{
	struct flash_regop_info regop_info = {0};

	if (retry_mode >= MAX_MICRON_READ_RETRIES) {
		pr_err("out of range\n");
		return -1;
	}

	regop_info.priv = info->drv->priv;
	regop_info.cmd = 0xEF;
	regop_info.nr_cmd = 1;
	regop_info.addrl = 0x89;
	regop_info.addr_cycle = 1;
	regop_info.buf = (u8*)&retry_mode;
	regop_info.sz_buf = 1;
	regop_info.wait_ready = true;

	info->drv->write_reg(&regop_info);

	return 0;
}
/******************************************************************************/

static int exit_micron(struct nand_read_retry_info *info)
{
	return setup_micron(info, 0);
}
/******************************************************************************/

struct nand_read_retry nand_read_retry_micron = {
	.type = NAND_RR_MICRON,
	.count = MAX_MICRON_READ_RETRIES,
	.setup = setup_micron,
	.exit = exit_micron,
};
