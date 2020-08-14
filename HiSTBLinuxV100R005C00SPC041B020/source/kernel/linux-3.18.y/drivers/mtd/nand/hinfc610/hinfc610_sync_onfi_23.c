/******************************************************************************
 *    NAND Flash Controller V610 Device Driver
 *    Copyright (c) 2009-2010 by Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By Czyong.
 *    
******************************************************************************/
#include <asm/io.h>
#include "hinfc610_os.h"
#include "hinfc610.h"

/*****************************************************************************/

static int hinfc610_onfi_enable_sync(struct nand_chip *chip)
{
	struct hinfc_host *host = chip->priv;
	unsigned char micron_sync_param[4] = {
		0x14, /* set sync mode timing */ 0x00, 0x00, 0x00,
	};

	host->enable_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, 1, HINFC610_DATA_NUM);
	hinfc_write(host, 0xEF, HINFC610_CMD);
	hinfc_write(host, 0x01, HINFC610_ADDRL);
	writel(micron_sync_param[0], host->chip->IO_ADDR_R);
	hinfc_write(host, HINFC610_WRITE_1CMD_1ADD_DATA(host), HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);

	writel(micron_sync_param[1], host->chip->IO_ADDR_R);
	hinfc_write(host, HINFC610_WRITE_0CMD_1ADD_DATA(host), HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);

	writel(micron_sync_param[2], host->chip->IO_ADDR_R);
	hinfc_write(host, HINFC610_WRITE_0CMD_1ADD_DATA(host), HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);

	/* need to config WAIT_READY_EN, here config this bit. */
	writel(micron_sync_param[3], host->chip->IO_ADDR_R);
	hinfc_write(host, HINFC610_WRITE_0CMD_1ADD_DATA_WAIT_READY(host), HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);

	host->enable_ecc_randomizer(host, ENABLE, ENABLE);

	return 0;
}

/*****************************************************************************/

static int hinfc610_onfi_disable_sync(struct nand_chip *chip)
{
	struct hinfc_host *host = chip->priv;
	unsigned char micron_sync_param[4] = {
		0x00, 0x00, 0x00, 0x00,
	};

	host->enable_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, 1, HINFC610_DATA_NUM);
	hinfc_write(host, 0xEF, HINFC610_CMD);
	hinfc_write(host, 0x01, HINFC610_ADDRL);
	writel(micron_sync_param[0], host->chip->IO_ADDR_R);
	hinfc_write(host, HINFC610_WRITE_1CMD_1ADD_DATA_SYNC(host), HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);

	writel(micron_sync_param[1], host->chip->IO_ADDR_R);
	hinfc_write(host, HINFC610_WRITE_0CMD_1ADD_DATA_SYNC(host), HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);
	
	writel(micron_sync_param[2], host->chip->IO_ADDR_R);
	hinfc_write(host, HINFC610_WRITE_0CMD_1ADD_DATA_SYNC(host), HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);

	writel(micron_sync_param[3], host->chip->IO_ADDR_R);
	hinfc_write(host, HINFC610_WRITE_0CMD_1ADD_DATA_SYNC_WAIT_READY(host), HINFC610_OP);
	WAIT_CONTROLLER_FINISH(host);

	host->enable_ecc_randomizer(host, ENABLE, ENABLE);

	return 0;
}
/*****************************************************************************/

struct nand_sync hinfc610_sync_onfi_23 = {
	.type    = NAND_TYPE_ONFI_23,
	.enable  = hinfc610_onfi_enable_sync,
	.disable = hinfc610_onfi_disable_sync,
};

struct nand_sync hinfc610_sync_onfi_30 = {
	.type    = NAND_TYPE_ONFI_30,
	.enable  = hinfc610_onfi_enable_sync,
	.disable = hinfc610_onfi_disable_sync,
};
