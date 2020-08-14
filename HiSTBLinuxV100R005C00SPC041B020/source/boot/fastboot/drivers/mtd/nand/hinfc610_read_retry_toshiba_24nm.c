/******************************************************************************
 *    NAND Flash Controller V610 Device Driver
 *    Copyright (c) 2009-2010 by Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By Czyong.
 *
******************************************************************************/

#include <common.h>
#include <nand.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <malloc.h>
#include <linux/mtd/nand.h>
#include "hinfc610.h"

/*****************************************************************************/

static int hinfc610_toshiba_24nm_set_rr_reg(struct hinfc_host *host, int param)
{
#define TOSHIBA_RR_CMD     0x55
	int opval;

	static char toshiba_rr_param[] = {0x00, 0x04, 0x7c, 0x78, 0x74, 0x08};

	if (!param) {
		host->send_cmd_reset(host, host->chipselect);
		return 0;
	}

	if (param >= 6)
		param = (param % 6);

	/* 
	 * no need to config WAIT_READY_EN, here not config WAIT_READY_EN
	 */
	opval = (HINFC610_IS_SYNC(host) ? HINFC610_WRITE_1CMD_1ADD_DATA_SYNC
		: HINFC610_WRITE_1CMD_1ADD_DATA);

	hinfc_write(host, 1, HINFC610_DATA_NUM);

	writel(toshiba_rr_param[param], host->chip->IO_ADDR_R);
	hinfc_write(host, 0x4, HINFC610_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH();

	writel(toshiba_rr_param[param], host->chip->IO_ADDR_R);
	hinfc_write(host, 0x5, HINFC610_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH();

	writel(toshiba_rr_param[param], host->chip->IO_ADDR_R);
	hinfc_write(host, 0x6, HINFC610_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH();

	writel(toshiba_rr_param[param], host->chip->IO_ADDR_R);
	hinfc_write(host, 0x7, HINFC610_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH();

	return 0;

#undef TOSHIBA_RR_CMD
}
/*****************************************************************************/

static int hinfc610_toshiba_24nm_set_rr_param(struct hinfc_host *host, int param)
{
	int opval;
	int regval;

	regval = hinfc_read(host, HINFC610_PWIDTH);
	hinfc_write(host, 0xFFF, HINFC610_PWIDTH);

	host->enable_ecc_randomizer(host, DISABLE, DISABLE);

	opval = (HINFC610_IS_SYNC(host) ? HINFC610_WRITE_2CMD_0ADD_NODATA_SYNC
		: HINFC610_WRITE_2CMD_0ADD_NODATA);

	hinfc_write(host, HINFC_CMD_SEQ(0x5C, 0xC5), HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH();

	hinfc610_toshiba_24nm_set_rr_reg(host, param);

	hinfc_write(host, HINFC_CMD_SEQ(0x26, 0x5D), HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH();

	host->enable_ecc_randomizer(host, ENABLE, ENABLE);
	hinfc_write(host, regval, HINFC610_PWIDTH);

	return 0;
}
/*****************************************************************************/

static int hinfc610_toshiba_24nm_reset_rr_param(struct hinfc_host *host)
{
	return hinfc610_toshiba_24nm_set_rr_reg(host, 0);
}
/*****************************************************************************/
struct read_retry_t hinfc610_toshiba_24nm_read_retry = {
	.type = NAND_RR_TOSHIBA_V2012,
	.count = 6,
	.set_rr_param = hinfc610_toshiba_24nm_set_rr_param,
	.get_rr_param = NULL,
	.reset_rr_param = hinfc610_toshiba_24nm_reset_rr_param,
	.enable_enhanced_slc = NULL,
};
