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

#define MICRON_RR_ADDR         0x89

static int hinfc610_micron_set_rr_reg(struct hinfc_host *host, int rr)
{
	int regval;
	int reg_pwidth;

	reg_pwidth = hinfc_read(host, HINFC610_PWIDTH);
	hinfc_write(host, 0xFFF, HINFC610_PWIDTH);

	host->enable_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, 1, HINFC610_DATA_NUM);

	writel(rr, host->chip->IO_ADDR_W);
	hinfc_write(host, MICRON_RR_ADDR, HINFC610_ADDRL);
	/* set read retry */
	hinfc_write(host, 0xEF, HINFC610_CMD);

	/* need to config WAIT_READY_EN, here config WAIT_READY_EN bit. */
	regval = (HINFC610_IS_SYNC(host) ? 
		HINFC610_WRITE_1CMD_1ADD_DATA_SYNC_WAIT_READY(host) :
		HINFC610_WRITE_1CMD_1ADD_DATA_WAIT_READY(host));

	hinfc_write(host, regval, HINFC610_OP);

	WAIT_CONTROLLER_FINISH(host);

	host->enable_ecc_randomizer(host, ENABLE, ENABLE);
	hinfc_write(host, reg_pwidth, HINFC610_PWIDTH);

	return 0;
}
/*****************************************************************************/

#if 0
static int hinfc610_micron_get_rr_param(struct hinfc_host *host)
{
#define MICRON_GET_RR          0xEE
	hinfc_write(host, 1, HINFC610_DATA_NUM);

	hinfc_write(host, MICRON_RR_ADDR, HINFC610_ADDRL);
	hinfc_write(host, MICRON_GET_RR, HINFC610_CMD);
	hinfc_write(host, HINFC610_READ_1CMD_1ADD_DATA(host), HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);
	hinfc_write(host, NAND_CMD_READ0, HINFC610_CMD);
	hinfc_write(host, HINFC610_READ_1CMD_0ADD_NODATA(host), HINFC610_OP);
#undef MICRON_GET_RR
	return (readl(host->chip->IO_ADDR_R) & 0xff);
}
#endif
#undef MICRON_RR_ADDR
/*****************************************************************************/

static int hinfc610_micron_set_rr_param(void *host, int rr_option)
{
	return hinfc610_micron_set_rr_reg((struct hinfc_host *)host, rr_option);
}
/*****************************************************************************/

static int hinfc610_micron_reset_rr_param(void *host)
{
	return hinfc610_micron_set_rr_reg((struct hinfc_host *)host, 0);
}
/*****************************************************************************/

struct read_retry_t hinfc610_micron_read_retry = {
	.type = NAND_RR_MICRON,
	.count = 8,
	.set_rr_param = hinfc610_micron_set_rr_param,
	.get_rr_param = NULL,
	.reset_rr_param = hinfc610_micron_reset_rr_param,
};
