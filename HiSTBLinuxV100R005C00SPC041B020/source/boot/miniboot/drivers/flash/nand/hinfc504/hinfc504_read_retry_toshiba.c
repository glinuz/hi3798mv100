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

static int hinfc504_toshiba_24nm_set_rr_reg(struct hinfc_host *host, int param)
{
#define TOSHIBA_RR_CMD     0x55

	static char toshiba_rr_param[] = {0x00, 0x04, 0x7c, 0x78, 0x74, 0x08};

	if (!param) {
		host->send_cmd_reset(host, host->chipselect);
		return 0;
	}

	if (param >= 6)
		param = (param % 6);

	host->set_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, 1, HINFC504_DATA_NUM);

	writel(toshiba_rr_param[param], host->iobase);
	hinfc_write(host, 0x4, HINFC504_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC504_CMD);
	hinfc_write(host, HINFC504_WRITE_1CMD_1ADD_DATA, HINFC504_OP);
	WAIT_CONTROLLER_FINISH();

	writel(toshiba_rr_param[param], host->iobase);
	hinfc_write(host, 0x5, HINFC504_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC504_CMD);
	hinfc_write(host, HINFC504_WRITE_1CMD_1ADD_DATA, HINFC504_OP);
	WAIT_CONTROLLER_FINISH();

	writel(toshiba_rr_param[param], host->iobase);
	hinfc_write(host, 0x6, HINFC504_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC504_CMD);
	hinfc_write(host, HINFC504_WRITE_1CMD_1ADD_DATA, HINFC504_OP);
	WAIT_CONTROLLER_FINISH();

	writel(toshiba_rr_param[param], host->iobase);
	hinfc_write(host, 0x7, HINFC504_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC504_CMD);
	hinfc_write(host, HINFC504_WRITE_1CMD_1ADD_DATA, HINFC504_OP);
	WAIT_CONTROLLER_FINISH();

	host->set_ecc_randomizer(host, ENABLE, ENABLE);

	return 0;

#undef TOSHIBA_RR_CMD
}
/*****************************************************************************/

static int hinfc504_toshiba_24nm_set_rr_param(struct hinfc_host *host, int param)
{
	host->set_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, HINFC_CMD_SEQ(0x5C, 0xC5), HINFC504_CMD);
	hinfc_write(host, HINFC504_WRITE_2CMD_0ADD_NODATA, HINFC504_OP);
	WAIT_CONTROLLER_FINISH();

	hinfc504_toshiba_24nm_set_rr_reg(host, param);

	hinfc_write(host, HINFC_CMD_SEQ(0x26, 0x5D), HINFC504_CMD);
	hinfc_write(host, HINFC504_WRITE_2CMD_0ADD_NODATA, HINFC504_OP);
	WAIT_CONTROLLER_FINISH();

	return 0;
}
/*****************************************************************************/

struct read_retry_t hinfc504_toshiba_24nm_read_retry = {
	.type = NAND_RR_TOSHIBA_V2012,
	.count = 6,
	.set_rr_param = hinfc504_toshiba_24nm_set_rr_param,
	.get_rr_param = NULL,
	.enable_enhanced_slc = NULL,
};
