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

static int hinfc610_samsung_set_rr_reg(struct hinfc_host *host,int param)
{
#define SAMSUNG_RR_CMD     0xA1
	int opval;

	unsigned char samsung_rr_params[15][4] = {
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

	if (param >= 15)
		param = (param % 15);

	host->enable_ecc_randomizer(host, DISABLE, DISABLE);

	/* no need to config WAIT_READY_EN, here not config WAIT_READY_EN bit */
	opval = (HINFC610_IS_SYNC(host) ? HINFC610_WRITE_1CMD_2ADD_DATA_SYNC
		: HINFC610_WRITE_1CMD_2ADD_DATA);

	hinfc_write(host, 1, HINFC610_DATA_NUM);

	writel(samsung_rr_params[param][0], host->chip->IO_ADDR_R);
	hinfc_write(host, 0xA700, HINFC610_ADDRL);
	hinfc_write(host, SAMSUNG_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH();

	writel(samsung_rr_params[param][1], host->chip->IO_ADDR_R);
	hinfc_write(host, 0xA400, HINFC610_ADDRL);
	hinfc_write(host, SAMSUNG_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH();

	writel(samsung_rr_params[param][2], host->chip->IO_ADDR_R);
	hinfc_write(host, 0xA500, HINFC610_ADDRL);
	hinfc_write(host, SAMSUNG_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH();

	writel(samsung_rr_params[param][3], host->chip->IO_ADDR_R);
	hinfc_write(host, 0xA600, HINFC610_ADDRL);
	hinfc_write(host, SAMSUNG_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH();

	host->enable_ecc_randomizer(host, ENABLE, ENABLE);

	return 0;

#undef SAMSUNG_RR_CMD
}
/*****************************************************************************/

static int hinfc610_samsung_set_rr_param(struct hinfc_host *host,int param)
{
	return hinfc610_samsung_set_rr_reg(host, param);
}
/*****************************************************************************/

static int hinfc610_samsung_reset_rr_param(struct hinfc_host *host)
{
	return hinfc610_samsung_set_rr_reg(host, 0);
}
/*****************************************************************************/

struct read_retry_t hinfc610_samsung_read_retry = {
	.type = NAND_RR_SAMSUNG,
	.count = 15,
	.set_rr_param = hinfc610_samsung_set_rr_param,
	.get_rr_param = NULL,
	.reset_rr_param = hinfc610_samsung_reset_rr_param,
	.enable_enhanced_slc = NULL,
};

