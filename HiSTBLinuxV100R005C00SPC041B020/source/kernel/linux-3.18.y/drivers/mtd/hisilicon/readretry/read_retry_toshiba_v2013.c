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

#define MAX_TOSHIBA2013_READ_RETRIES  8
#define TOSHIBA_RR_CMD                0x55

static u8 rr_data[MAX_TOSHIBA2013_READ_RETRIES][5] = {
	{0x00, 0x00, 0x00, 0x00, 0x00},
	{0x04, 0x04, 0x7c, 0x7e, 0x00},
	{0x00, 0x7c, 0x78, 0x78, 0x00},
	{0x7c, 0x76, 0x74, 0x72, 0x00},
	{0x08, 0x08, 0x00, 0x00, 0x00},
	{0x0b, 0x7e, 0x76, 0x74, 0x00},
	{0x10, 0x76, 0x72, 0x70, 0x00},
	{0x02, 0x7c, 0x7e, 0x70, 0x00},
};
/******************************************************************************/

static int setup_toshiba_v2013(struct nand_read_retry_info *info, int retry_mode)
{
	struct flash_regop_info regop_info = {0};

	if (retry_mode >= MAX_TOSHIBA2013_READ_RETRIES) {
		pr_err("read retry out of range\n");
		return -1;
	}

	regop_info.priv = info->drv->priv;

	regop_info.cmd = TOSHIBA_RR_CMD;
	regop_info.nr_cmd = 1;
	regop_info.sz_buf = 1;
	regop_info.addr_cycle = 1;

	regop_info.addrl = 0x04;
	regop_info.buf = &rr_data[retry_mode][0];
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0x05;
	regop_info.buf = &rr_data[retry_mode][1];
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0x06;
	regop_info.buf = &rr_data[retry_mode][2];
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0x07;
	regop_info.buf = &rr_data[retry_mode][3];
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0x0d;
	regop_info.buf = &rr_data[retry_mode][4];
	info->drv->write_reg(&regop_info);

	if (retry_mode == 4) {
		regop_info.cmd = 0xb3;
		regop_info.nr_cmd = 1;
		regop_info.addr_cycle = 0;
		regop_info.sz_buf = 0;
		info->drv->write_reg(&regop_info);
	}

	regop_info.cmd = 0x5d26;
	regop_info.nr_cmd = 2;
	regop_info.addr_cycle = 0;
	regop_info.sz_buf = 0;
	info->drv->write_reg(&regop_info);

	return 0;
}
/******************************************************************************/

static int entry_toshiba_v2013(struct nand_read_retry_info *info)
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

static int exit_toshiba_v2013(struct nand_read_retry_info *info)
{
	int retry_mode = 0;
	struct flash_regop_info regop_info = {0};

	regop_info.priv = info->drv->priv;

	regop_info.cmd = TOSHIBA_RR_CMD;
	regop_info.nr_cmd = 1;
	regop_info.sz_buf = 1;
	regop_info.addr_cycle = 1;

	regop_info.addrl = 0x04;
	regop_info.buf = &rr_data[retry_mode][0];
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0x05;
	regop_info.buf = &rr_data[retry_mode][1];
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0x06;
	regop_info.buf = &rr_data[retry_mode][2];
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0x07;
	regop_info.buf = &rr_data[retry_mode][3];
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0x0d;
	regop_info.buf = &rr_data[retry_mode][4];
	info->drv->write_reg(&regop_info);

	regop_info.cmd = NAND_CMD_RESET;
	regop_info.nr_cmd = 1;
	regop_info.addr_cycle = 0;
	regop_info.sz_buf = 0;
	regop_info.wait_ready = true;
	info->drv->write_reg(&regop_info);

	return 0;
}
/******************************************************************************/

struct nand_read_retry nand_read_retry_toshiba_v2013 = {
	.type = NAND_RR_TOSHIBA_V2013,
	.count = MAX_TOSHIBA2013_READ_RETRIES,
	.setup = setup_toshiba_v2013,
	.exit  = exit_toshiba_v2013,
	.entry = entry_toshiba_v2013,
};
