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

#include <hinfc_gen.h>
#include <linux/mtd/nand.h>
#include <common.h>

#include "hiflash.h"
/******************************************************************************/

#define MAX_TOSHIBA2012_READ_RETRIES   6

static u8 rr_data[MAX_TOSHIBA2012_READ_RETRIES] = {
	0x00, 0x04, 0x7c, 0x78, 0x74, 0x08,
};
/******************************************************************************/

static int setup_toshiba_v2012(struct nand_read_retry_info *info,
			       int retry_mode)
{
	struct flash_regop_info regop_info = {0};

	if (retry_mode >= MAX_TOSHIBA2012_READ_RETRIES) {
		PR_OUT("read retry out of range\n");
		return -1;
	}

	regop_info.priv = info->drv->priv;

	regop_info.cmd = 0x55;
	regop_info.nr_cmd = 1;
	regop_info.buf = &rr_data[retry_mode];
	regop_info.sz_buf = 1;
	regop_info.wait_ready = false;
	regop_info.addr_cycle = 1;

	regop_info.addrl = 0x04;
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0x05;
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0x06;
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0x07;
	info->drv->write_reg(&regop_info);

	regop_info.cmd = 0x5d26;
	regop_info.nr_cmd = 2;
	regop_info.addr_cycle = 0;
	regop_info.sz_buf = 0;
	info->drv->write_reg(&regop_info);

	return 0;
}
/******************************************************************************/

static int entry_toshiba_v2012(struct nand_read_retry_info *info)
{
	struct flash_regop_info regop_info = {0};

	regop_info.priv = info->drv->priv;

	regop_info.cmd = 0xc55c;
	regop_info.nr_cmd = 2;
	regop_info.addr_cycle = 0;
	regop_info.sz_buf = 0;
	info->drv->write_reg(&regop_info);

	return 0;
}
/******************************************************************************/

static int exit_toshiba_v2012(struct nand_read_retry_info *info)
{
	struct flash_regop_info regop_info = {0};

	regop_info.priv = info->drv->priv;

	regop_info.cmd = NAND_CMD_RESET;
	regop_info.nr_cmd = 1;
	regop_info.addr_cycle = 0;
	regop_info.sz_buf = 0;
	regop_info.wait_ready = true;
	info->drv->write_reg(&regop_info);

	return 0;
}
/******************************************************************************/

struct nand_read_retry nand_read_retry_toshiba_v2012 = {
	.type = NAND_RR_TOSHIBA_V2012,
	.count = MAX_TOSHIBA2012_READ_RETRIES,
	.setup = setup_toshiba_v2012,
	.entry = entry_toshiba_v2012,
	.exit = exit_toshiba_v2012,
};
