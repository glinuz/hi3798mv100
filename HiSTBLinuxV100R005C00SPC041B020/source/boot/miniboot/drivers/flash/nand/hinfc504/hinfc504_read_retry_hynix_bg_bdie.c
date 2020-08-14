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
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <asm/io.h>

#include "hinfc504.h"

/*****************************************************************************/

static int hynix_bg_bdie_rr_org_exist = 0;
static char hynix_bg_bdie_rr_org[4] = {0};

/*****************************************************************************/

static int hinfc504_hynix_bg_bdie_set_rr_reg(struct hinfc_host *host, int index)
{
	int ix;
	char HYNIX_BG_BDIE_RR_REG[4] = {0xA7,  0xAD,  0xAE,  0xAF};
	char value_offset[7][4] = {
		{0x00,  0x00,  0x00,  0x00},
		{0x00,  0x06,  0x0A,  0x06},
		{0x7F, -0x03, -0x07, -0x08},
		{0x7F, -0x06, -0x0D, -0x0F},
		{0x7F, -0x09, -0x14, -0x17},
		{0x7F,  0x7F, -0x1A, -0x1E},
		{0x7F,  0x7F, -0x20, -0x25}
	};
	char *value = &value_offset[index][0];

	host->set_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, 1, HINFC504_DATA_NUM);

	if (!hynix_bg_bdie_rr_org_exist) {

		for (ix = 0; ix < 4; ix++) {

			memset(host->iobase, 0xff, 32);

			hinfc_write(host, 0x37, HINFC504_CMD);
			hinfc_write(host, HYNIX_BG_BDIE_RR_REG[ix],
				HINFC504_ADDRL);
			hinfc_write(host, HINFC504_READ_1CMD_1ADD_DATA,
				HINFC504_OP);

			WAIT_CONTROLLER_FINISH();

			hynix_bg_bdie_rr_org[ix]
				= (char)(readl(host->iobase) & 0xff);
		}
		hynix_bg_bdie_rr_org_exist = 1;
	}

	for (ix = 0; ix < 4; ix++) {
		if (value[ix] == 0x7F)
			value[ix] = 0x00;
		else
			value[ix] += hynix_bg_bdie_rr_org[ix];
	}

	writel(value[0], host->iobase);
	hinfc_write(host, HYNIX_BG_BDIE_RR_REG[0], HINFC504_ADDRL);
	hinfc_write(host, 0x36, HINFC504_CMD);
	hinfc_write(host, HINFC504_WRITE_1CMD_1ADD_DATA, HINFC504_OP);
	WAIT_CONTROLLER_FINISH();

	for (ix = 1; ix < 4; ix++) {
		writel(value[ix], host->iobase);
		hinfc_write(host, HYNIX_BG_BDIE_RR_REG[ix], HINFC504_ADDRL);
		hinfc_write(host, HINFC504_WRITE_0CMD_1ADD_DATA, HINFC504_OP);
		WAIT_CONTROLLER_FINISH();
	}

	hinfc_write(host, 0x16, HINFC504_CMD);
	hinfc_write(host, HINFC504_WRITE_2CMD_0ADD_NODATA, HINFC504_OP);
	WAIT_CONTROLLER_FINISH();

	return 0;
}
/*****************************************************************************/

static int hinfc504_hynix_bg_bdie_set_rr_param(struct hinfc_host *host, int param)
{
	if (!param)
		return 0;
	return hinfc504_hynix_bg_bdie_set_rr_reg(host, param);
}
/*****************************************************************************/

struct read_retry_t hinfc504_hynix_bg_bdie_read_retry = {
	.type = NAND_RR_HYNIX_BG_BDIE,
	.count = 7,
	.set_rr_param = hinfc504_hynix_bg_bdie_set_rr_param,
	.get_rr_param = NULL,
	.enable_enhanced_slc = NULL,
};
