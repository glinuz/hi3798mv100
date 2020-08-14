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

static char * hinfc610_hynix_bg_cdie_otp_check(char *otp)
{
	int index = 0;
	int ix, jx;
	char *ptr = NULL;
	int min, cur;
	char *otp_origin, *otp_inverse;

	min = 64;
	for (ix = 0; ix < 8; ix++, otp += 128) {

		otp_origin  = otp;
		otp_inverse = otp + 64;
		cur = 0;

		for (jx = 0; jx < 64; jx++, otp_origin++, otp_inverse++) {
			if (((*otp_origin) ^ (*otp_inverse)) == 0xFF)
				continue;
			cur ++;
		}

		if (cur < min) {
			min = cur;
			index = ix;
			ptr = otp;
			if (!cur)
				break;
		}
	}

	printk("RR select parameter %d from %d, error %d\n",
		index, ix, min);
	return ptr;
}
/*****************************************************************************/

static int hinfc610_hynix_bg_cdie_get_rr_param(void *host)
{
	char *otp;
	struct hinfc_host *hinfc_host = (struct hinfc_host *)host;

	hinfc_host->enable_ecc_randomizer(hinfc_host, DISABLE, DISABLE);
	/* step1: reset the chip */
	hinfc_host->send_cmd_reset(hinfc_host, hinfc_host->chipselect);

	/* step2: cmd: 0x36, address: 0xAE, data: 0x00 */
	hinfc_write(hinfc_host, 1, HINFC610_DATA_NUM);/* data length 1 */
	writel(0x00, hinfc_host->chip->IO_ADDR_R); /* data: 0x00 */
	hinfc_write(hinfc_host, 0xAE, HINFC610_ADDRL);/* address: 0xAE */
	hinfc_write(hinfc_host, 0x36, HINFC610_CMD);  /* cmd: 0x36 */
	/* according to hynix doc, no need to config HINFC610_OP_WAIT_READY_EN */
	hinfc_write(hinfc_host, HINFC610_WRITE_1CMD_1ADD_DATA(hinfc_host), HINFC610_OP);
	WAIT_CONTROLLER_FINISH(hinfc_host);

	/* step3: address: 0xB0, data: 0x4D */
	hinfc_write(hinfc_host, 1, HINFC610_DATA_NUM);/* data length 1 */
	writel(0x4D, hinfc_host->chip->IO_ADDR_R); /* data: 0x4d */
	hinfc_write(hinfc_host, 0xB0, HINFC610_ADDRL);/* address: 0xB0 */
	/* only address and data, without cmd */
	/* according to hynix doc, no need to config HINFC610_OP_WAIT_READY_EN */
	hinfc_write(hinfc_host, HINFC610_WRITE_0CMD_1ADD_DATA(hinfc_host), HINFC610_OP);
	WAIT_CONTROLLER_FINISH(hinfc_host);

	/* step4: cmd: 0x16, 0x17, 0x04, 0x19 */
	hinfc_write(hinfc_host, 0x17 << 8 | 0x16, HINFC610_CMD);
	/* according to hynix doc, no need to config HINFC610_OP_WAIT_READY_EN */
	hinfc_write(hinfc_host, HINFC610_WRITE_2CMD_0ADD_NODATA(hinfc_host), HINFC610_OP);
	WAIT_CONTROLLER_FINISH(hinfc_host);

	hinfc_write(hinfc_host, 0x19 << 8 | 0x04, HINFC610_CMD);
	/* according to hynix doc, no need to config HINFC610_OP_WAIT_READY_EN */
	hinfc_write(hinfc_host, HINFC610_WRITE_2CMD_0ADD_NODATA(hinfc_host), 
		    HINFC610_OP);
	WAIT_CONTROLLER_FINISH(hinfc_host);

	/* step5: cmd: 0x00 0x30, address: 0x02 00 00 00 */
	hinfc_write(hinfc_host, 0x2000000, HINFC610_ADDRL);
	hinfc_write(hinfc_host, 0x00, HINFC610_ADDRH);
	hinfc_write(hinfc_host, 0x30 << 8 | 0x00, HINFC610_CMD);
	hinfc_write(hinfc_host, 0x800, HINFC610_DATA_NUM);
	/* according to hynix doc, need to config HINFC610_OP_WAIT_READY_EN */
	hinfc_write(hinfc_host, HINFC610_READ_2CMD_5ADD(hinfc_host), 
		    HINFC610_OP);
	WAIT_CONTROLLER_FINISH(hinfc_host);

	/*step6 save otp read retry table to mem*/
	otp = hinfc610_hynix_bg_cdie_otp_check(hinfc_host->chip->IO_ADDR_R + 2);
	if (!otp) {
		printk(KERN_ERR "Read Retry select parameter failed, "
		       "this Nand Chip maybe invalidation.\n");
		return -1;
	}
	memcpy(hinfc_host->rr_data, otp, 64);
	hinfc_host->need_rr_data = 1;

	/* step7: reset the chip */
	hinfc_host->send_cmd_reset(hinfc_host, hinfc_host->chipselect);

	/* step8: cmd: 0x38 */
	hinfc_write(hinfc_host, 0x38, HINFC610_CMD);
	/* according to hynix doc, need to config HINFC610_OP_WAIT_READY_EN */
	hinfc_write(hinfc_host, 
		    HINFC610_WRITE_1CMD_0ADD_NODATA_WAIT_READY(hinfc_host), 
		    HINFC610_OP);
	WAIT_CONTROLLER_FINISH(hinfc_host);

	hinfc_host->enable_ecc_randomizer(hinfc_host, ENABLE, ENABLE);
	/* get hynix otp table finish */
	return 0;
}
/*****************************************************************************/
static char hinfc610_hynix_bg_cdie_rr_reg[8] = {
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7};

static int hinfc610_hynix_bg_cdie_set_rr_reg(struct hinfc_host *host,
					     char *val)
{
	int i;
	int regval;
	host->enable_ecc_randomizer(host, DISABLE, DISABLE);

	regval = hinfc_read(host, HINFC610_PWIDTH);
	hinfc_write(host, 0xFFF, HINFC610_PWIDTH);

	hinfc_write(host, 1, HINFC610_DATA_NUM);/* data length 1 */

	for (i = 0; i <= 8; i++) {
		switch (i) {
		case 0:
			writel(val[i], host->chip->IO_ADDR_R);
			hinfc_write(host,
				hinfc610_hynix_bg_cdie_rr_reg[i],
				HINFC610_ADDRL);
			hinfc_write(host,
				0x36, HINFC610_CMD);
			/*
			 * no need to config HINFC610_OP_WAIT_READY_EN, here not config this bit.
			 */
			hinfc_write(host,
				HINFC610_WRITE_1CMD_1ADD_DATA(host),
				HINFC610_OP);
			break;
		case 8:
			hinfc_write(host,
				0x16, HINFC610_CMD);
			/*
			 * according to hynix doc, only 1 cmd: 0x16. 
			 * And no need to config HINFC610_OP_WAIT_READY_EN, here not config this bit.
			 */
			hinfc_write(host,
				HINFC610_WRITE_1CMD_0ADD_NODATA(host),
				HINFC610_OP);
			break;
		default:
			writel(val[i], host->chip->IO_ADDR_R);
			hinfc_write(host,
				hinfc610_hynix_bg_cdie_rr_reg[i],
				HINFC610_ADDRL);
			/*
			 * no need to config HINFC610_OP_WAIT_READY_EN, here not config this bit.
			 */
			hinfc_write(host,
				HINFC610_WRITE_0CMD_1ADD_DATA(host),
				HINFC610_OP);
			break;
		}
		WAIT_CONTROLLER_FINISH(host);
	}
	host->enable_ecc_randomizer(host, ENABLE, ENABLE);
	hinfc_write(host, regval, HINFC610_PWIDTH);

	return 0;
}
/*****************************************************************************/
#if 0
static void hinfc610_hynix_bg_cdie_get_rr(struct hinfc_host *host)
{
	int index;

	host->enable_ecc_randomizer(host, DISABLE, DISABLE);
	hinfc_write(host, 1, HINFC610_DATA_NUM);

	for (index = 0; index < 8; index++) {
		memset(host->chip->IO_ADDR_R, 0xff, 32);
		hinfc_write(host,
			0x37, HINFC610_CMD);
		hinfc_write(host,
			hinfc610_hynix_bg_cdie_rr_reg[index],
			HINFC610_ADDRL);
		hinfc_write(host,
			HINFC610_READ_1CMD_1ADD_DATA(host),
			HINFC610_OP);

		WAIT_CONTROLLER_FINISH(host);
	
		printk("%02X ", readl(host->chip->IO_ADDR_R) & 0xff);
	}

	host->enable_ecc_randomizer(host, ENABLE, ENABLE);
}
#endif
/*****************************************************************************/

static int hinfc610_hynix_bg_cdie_set_rr_param(void *host,
					       int param)
{
	unsigned char *rr;
	struct hinfc_host *hinfc_host = (struct hinfc_host *)host;

	if (!(hinfc_host->rr_data[0] | hinfc_host->rr_data[1]
	    | hinfc_host->rr_data[2] | hinfc_host->rr_data[3]) || !param)
		return -1;

	rr = (unsigned char *)&hinfc_host->rr_data[((param & 0x07) << 3)];

	/* set the read retry regs to adjust reading level */
	return hinfc610_hynix_bg_cdie_set_rr_reg(hinfc_host, (char *)rr);
}
/*****************************************************************************/

static int hinfc610_hynix_bg_cdie_reset_rr_param(void *host)
{
	return hinfc610_hynix_bg_cdie_set_rr_param(host, 0);
}
/*****************************************************************************/

struct read_retry_t hinfc610_hynix_bg_cdie_read_retry = {
	.type = NAND_RR_HYNIX_BG_CDIE,
	.count = 8,
	.set_rr_param = hinfc610_hynix_bg_cdie_set_rr_param,
	.get_rr_param = hinfc610_hynix_bg_cdie_get_rr_param,
	.reset_rr_param = hinfc610_hynix_bg_cdie_reset_rr_param,
};
