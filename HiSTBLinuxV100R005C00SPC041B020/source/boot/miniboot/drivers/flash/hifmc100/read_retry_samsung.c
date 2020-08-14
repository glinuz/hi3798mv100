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
#include "../nand/nand_chip.h"
#include "hiflash.h"

/*****************************************************************************/

#define MAX_SAMSUNG_READ_RETRIES   15

static u8 rr_data[MAX_SAMSUNG_READ_RETRIES][4] = {
	{0x00, 0x00, 0x00, 0x00},
	{0x05, 0x0A, 0x00, 0x00},
	{0x28, 0x00, 0xEC, 0xD8},
	{0xED, 0xF5, 0xED, 0xE6},
	{0x0A, 0x0F, 0x05, 0x00},
	{0x0F, 0x0A, 0xFB, 0xEC},
	{0xE8, 0xEF, 0xE8, 0xDC},
	{0xF1, 0xFB, 0xFE, 0xF0},
	{0x0A, 0x00, 0xFB, 0xEC},
	{0xD0, 0xE2, 0xD0, 0xC2},
	{0x14, 0x0F, 0xFB, 0xEC},
	{0xE8, 0xFB, 0xE8, 0xDC},
	{0x1E, 0x14, 0xFB, 0xEC},
	{0xFB, 0xFF, 0xFB, 0xF8},
	{0x07, 0x0C, 0x02, 0x00}
};
/******************************************************************************/

static int setup_samsung(struct nand_read_retry_info *info, int retry_mode)
{
	struct flash_regop_info regop_info = {0};

	if (retry_mode >= MAX_SAMSUNG_READ_RETRIES) {
		PR_OUT("read retry out of range\n");
		return -1;
	}

	regop_info.priv = info->drv->priv;
	regop_info.cmd = 0xA1;
	regop_info.nr_cmd = 1;
	regop_info.addr_cycle = 2;
	regop_info.sz_buf = 1;

	regop_info.addrl = 0xA700;
	regop_info.buf = &rr_data[retry_mode][0];
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0xA400;
	regop_info.buf = &rr_data[retry_mode][1];
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0xA500;
	regop_info.buf = &rr_data[retry_mode][2];
	info->drv->write_reg(&regop_info);

	regop_info.addrl = 0xA600;
	regop_info.buf = &rr_data[retry_mode][3];
	info->drv->write_reg(&regop_info);

	return 0;
}
/******************************************************************************/

static int exit_samsung(struct nand_read_retry_info *info)
{
	return setup_samsung(info, 0);
}
/******************************************************************************/

struct nand_read_retry nand_read_retry_samsung = {
	.type = NAND_RR_SAMSUNG,
	.count = MAX_SAMSUNG_READ_RETRIES,
	.setup = setup_samsung,
	.exit = exit_samsung,
};
