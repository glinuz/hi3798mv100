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
/******************************************************************************/

#define MAX_RR_HYNIX_READ_RETRIES  8
/******************************************************************************/

static int __read_retry_hynix_cg_adie(struct nand_read_retry_info *info, u8 *rr_data,
				      int retry_mode)
{
	int ix;
	u8 *ptr;
	struct flash_regop_info regop_info = {0};

	if (!retry_mode)
		return 0;

	if (retry_mode >= MAX_RR_HYNIX_READ_RETRIES) {
		PR_OUT("read retry out of range\n");
		return -1;
	}

	ptr = &info->buf[retry_mode << 3];

	regop_info.priv = info->drv->priv;
	regop_info.sz_buf = 1;
	regop_info.addr_cycle = 1;

	regop_info.cmd = 0x36;
	regop_info.nr_cmd = 1;
	regop_info.buf = &ptr[0];
	regop_info.addrl = rr_data[0];
	info->drv->write_reg(&regop_info);

	regop_info.nr_cmd = 0;
	for (ix = 1; ix < 8; ix++) {
		regop_info.buf = &ptr[ix];
		regop_info.addrl = rr_data[ix];
		info->drv->write_reg(&regop_info);
	}

	regop_info.cmd = 0x16;
	regop_info.nr_cmd = 1;
	regop_info.sz_buf = 0;
	regop_info.addr_cycle = 0;
	info->drv->write_reg(&regop_info);

	return 0;
}
/******************************************************************************/

static int setup_hynix_cg_adie(struct nand_read_retry_info *info, int retry_mode)
{
	u8 rr_data[MAX_RR_HYNIX_READ_RETRIES] = {
		0xCC, 0xBF, 0xAA, 0xAB, 0xCD, 0xAD, 0xAE, 0xAF,
	};

	return __read_retry_hynix_cg_adie(info, rr_data, retry_mode);
}
/******************************************************************************/

static int setup_hynix_bc_cdie(struct nand_read_retry_info *info, int retry_mode)
{
	u8 rr_data[MAX_RR_HYNIX_READ_RETRIES] = {
		0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
	};

	return __read_retry_hynix_cg_adie(info, rr_data, retry_mode);
}
/******************************************************************************/

struct nand_read_retry nand_read_retry_hynix_cg_adie = {
	.type = NAND_RR_HYNIX_CG_ADIE,
	.count = MAX_RR_HYNIX_READ_RETRIES,
	.setup = setup_hynix_cg_adie,
};
/******************************************************************************/

struct nand_read_retry nand_read_retry_hynix_bc_cdie = {
	.type = NAND_RR_HYNIX_BG_CDIE,
	.count = MAX_RR_HYNIX_READ_RETRIES,
	.setup = setup_hynix_bc_cdie,
};
