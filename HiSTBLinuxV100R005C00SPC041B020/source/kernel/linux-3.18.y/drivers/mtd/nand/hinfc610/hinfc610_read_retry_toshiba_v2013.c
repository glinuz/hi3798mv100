/******************************************************************************
 *    NAND Flash Controller V610 Device Driver
 *    Copyright (c) 2009-2010 by Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By Czyong.
 *    
******************************************************************************/

#include "hinfc610_os.h"
#include "hinfc610.h"

/*****************************************************************************/

static int hinfc610_toshiba_19nm_set_rr_reg(struct hinfc_host *host, int param)
{
#define TOSHIBA_RR_CMD          0x55
#define TOSHIBA_RR_DATA_NUM     0x5
	unsigned int opval;

	static char toshiba_rr_param[][TOSHIBA_RR_DATA_NUM] = 
	{
		{0x00, 0x00, 0x00, 0x00, 0x00},
		{0x04, 0x04, 0x7c, 0x7e, 0x00},
		{0x00, 0x7c, 0x78, 0x78, 0x00},
		{0x7c, 0x76, 0x74, 0x72, 0x00},
		{0x08, 0x08, 0x00, 0x00, 0x00},
		{0x0b, 0x7e, 0x76, 0x74, 0x00},
		{0x10, 0x76, 0x72, 0x70, 0x00},
		{0x02, 0x7c, 0x7e, 0x70, 0x00},
	};

	if (param > 7)
		param = (param % 7);

	/* 
	 * no need to config WAIT_READY_EN, here not config WAIT_READY_EN
	 */
	opval = (HINFC610_IS_SYNC(host) ? HINFC610_WRITE_1CMD_1ADD_DATA_SYNC(host)
		: HINFC610_WRITE_1CMD_1ADD_DATA(host));

	hinfc_write(host, 1, HINFC610_DATA_NUM);

	writel(toshiba_rr_param[param][0], host->chip->IO_ADDR_R);
	hinfc_write(host, 0x4, HINFC610_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);

	writel(toshiba_rr_param[param][1], host->chip->IO_ADDR_R);
	hinfc_write(host, 0x5, HINFC610_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);

	writel(toshiba_rr_param[param][2], host->chip->IO_ADDR_R);
	hinfc_write(host, 0x6, HINFC610_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);

	writel(toshiba_rr_param[param][3], host->chip->IO_ADDR_R);
	hinfc_write(host, 0x7, HINFC610_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);

	writel(toshiba_rr_param[param][4], host->chip->IO_ADDR_R);
	hinfc_write(host, 0xd, HINFC610_ADDRL);
	hinfc_write(host, TOSHIBA_RR_CMD, HINFC610_CMD);
	hinfc_write(host, opval, HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);

	return 0;

#undef TOSHIBA_RR_CMD
#undef TOSHIBA_RR_DATA_NUM
}
/*****************************************************************************/

static int hinfc610_toshiba_19nm_set_rr_param(void *host, int param)
{
	static int entry_rr_flag = 0;
	int regval, opval;
	struct hinfc_host *hinfc_host = (struct hinfc_host *)host;

	regval = hinfc_read(hinfc_host, HINFC610_PWIDTH);
	hinfc_write(hinfc_host, 0xFFF, HINFC610_PWIDTH);

	hinfc_host->enable_ecc_randomizer(hinfc_host, DISABLE, DISABLE);

	opval = (HINFC610_IS_SYNC(hinfc_host) ? HINFC610_WRITE_2CMD_0ADD_NODATA_SYNC(hinfc_host)
		: HINFC610_WRITE_2CMD_0ADD_NODATA(hinfc_host));

	/* first entry RR mode. */
	if (!entry_rr_flag) {
		hinfc_write(hinfc_host, HINFC_CMD_SEQ(0x5C, 0xC5), HINFC610_CMD);
		hinfc_write(hinfc_host, opval, HINFC610_OP);
		WAIT_CONTROLLER_FINISH(hinfc_host);

		entry_rr_flag = 1;
	}

	hinfc610_toshiba_19nm_set_rr_reg(hinfc_host, param);

	/* according to 19nm rr spec, 4th rr need send 0xb3 cmd befor 0x5d26 cmd */
	if (param == 4) {
		opval = (HINFC610_IS_SYNC(hinfc_host) ? HINFC610_WRITE_1CMD_0ADD_NODATA_SYNC(hinfc_host)
			: HINFC610_WRITE_1CMD_0ADD_NODATA(hinfc_host));

		hinfc_write(hinfc_host, 0xB3, HINFC610_CMD);
		hinfc_write(hinfc_host, opval, HINFC610_OP);
		WAIT_CONTROLLER_FINISH(hinfc_host);
	}

	if (param) {
		opval = (HINFC610_IS_SYNC(hinfc_host) ? HINFC610_WRITE_2CMD_0ADD_NODATA_SYNC(hinfc_host)
			: HINFC610_WRITE_2CMD_0ADD_NODATA(hinfc_host));

		hinfc_write(hinfc_host, HINFC_CMD_SEQ(0x26, 0x5D), HINFC610_CMD);
		hinfc_write(hinfc_host, opval, HINFC610_OP);
		WAIT_CONTROLLER_FINISH(hinfc_host);
	} else {
		/* exit RR mode. */
		hinfc_host->send_cmd_reset(hinfc_host, hinfc_host->chipselect);
		entry_rr_flag = 0;
	}

	hinfc_host->enable_ecc_randomizer(hinfc_host, ENABLE, ENABLE);
	hinfc_write(hinfc_host, regval, HINFC610_PWIDTH);

	return 0;
}
/*****************************************************************************/

static int hinfc610_toshiba_19nm_reset_rr_param(void *host)
{
	return hinfc610_toshiba_19nm_set_rr_param(host, 0);
}
/*****************************************************************************/
struct read_retry_t hinfc610_toshiba_19nm_read_retry = {
	.type = NAND_RR_TOSHIBA_V2013,
	.count = 8,
	.set_rr_param = hinfc610_toshiba_19nm_set_rr_param,
	.get_rr_param = NULL,
	.reset_rr_param = hinfc610_toshiba_19nm_reset_rr_param,
};
