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

#define MICRON_RR_ADDR         0x89

static int hinfc504_micron_set_rr_reg(struct hinfc_host *host, int rr)
{
	host->set_ecc_randomizer(host, DISABLE, DISABLE);

#define MICRON_SET_RR          0xEF
	hinfc_write(host, 1, HINFC504_DATA_NUM);

	writel(rr, host->iobase);
	hinfc_write(host, MICRON_RR_ADDR, HINFC504_ADDRL);
	hinfc_write(host, MICRON_SET_RR, HINFC504_CMD);
	hinfc_write(host, HINFC504_WRITE_1CMD_1ADD_DATA, HINFC504_OP);
	WAIT_CONTROLLER_FINISH();
#undef MICRON_SET_RR

	host->set_ecc_randomizer(host, ENABLE, ENABLE);

	return 0;
}
/*****************************************************************************/

#if 0
static int hinfc504_micron_get_rr_param(struct hinfc_host *host)
{
	host->set_ecc_randomizer(host, DISABLE, DISABLE);

#define MICRON_GET_RR          0xEE
	hinfc_write(host, 1, HINFC504_DATA_NUM);

	hinfc_write(host, MICRON_RR_ADDR, HINFC504_ADDRL);
	hinfc_write(host, MICRON_GET_RR, HINFC504_CMD);
	hinfc_write(host, HINFC504_READ_1CMD_1ADD_DATA, HINFC504_OP);
	WAIT_CONTROLLER_FINISH();
	hinfc_write(host, NAND_CMD_READ0, HINFC504_CMD);
	hinfc_write(host, HINFC504_READ_1CMD_0ADD_NODATA, HINFC504_OP);
#undef MICRON_GET_RR

	host->set_ecc_randomizer(host, ENABLE, ENABLE);

	return readl(host->iobase) & 0xff;
}
#endif
#undef MICRON_RR_ADDR
/*****************************************************************************/

static int hinfc504_micron_set_rr_param(struct hinfc_host *host, int rr_option)
{
	return hinfc504_micron_set_rr_reg(host, rr_option);
}
/*****************************************************************************/

struct read_retry_t hinfc504_micron_read_retry = {
	.type = NAND_RR_MICRON,
	.count = 8,
	.set_rr_param = hinfc504_micron_set_rr_param,
	.get_rr_param = NULL,
	.enable_enhanced_slc = NULL,
};
