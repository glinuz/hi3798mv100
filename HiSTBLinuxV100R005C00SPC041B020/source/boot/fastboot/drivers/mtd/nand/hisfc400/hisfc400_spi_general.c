/*****************************************************************************
 *    Copyright (c) 2014 by Hisilicon
 *    All rights reserved.
 *****************************************************************************/

#include "hisfc400.h"

/*****************************************************************************/
/*
  Send set/get features command to SPI Nand
*/
void spi_feature_op(struct hisfc_host *host, unsigned char op,
		    unsigned char addr, unsigned int *value)
{
	unsigned int regval = 0;

	hisfc_write(host, HISFC400_INTCLR, HISFC400_INTCLR_ALL);

	hisfc_write(host, HISFC400_OPCODE,
		    op ? SPI_CMD_SET_FEATURE : SPI_CMD_GET_FEATURES);
	hisfc_write(host, HISFC400_OP_ADDR, addr);

	hisfc_write(host, HISFC400_OP_CFG, HISFC400_OP_CFG_DIR_TRANS_ENABLE);

	hisfc_write(host, HISFC400_DATA_NUM, FEATURES_DATE_NUM);

	regval = (HISFC400_OP_SEL_CS(host->chipselect)
		| HISFC400_OP_ADDR_NUM(FEATURES_OP_ADDR_NUM)
		| HISFC400_OP_OPCODE_EN
		| HISFC400_OP_ADDR_EN
		| HISFC400_OP_START);

	if (SET_FEATURE == op) {
		regval |= HISFC400_OP_DATE_WRITE_EN;
		writeb(*value, host->chip->IO_ADDR_W);

		hisfc_write(host, HISFC400_OP, regval);

		HISFC400_CMD_WAIT_CPU_FINISH(host);

	} else if (GET_FEATURE == op) {
		regval |= HISFC400_OP_STATUS_READ_EN;

		hisfc_write(host, HISFC400_OP, regval);

		HISFC400_CMD_WAIT_CPU_FINISH(host);

		*value = hisfc_read(host, HISFC400_STATUS);
	}
}
/*
 * RETURN: 
 * 1 :  timeout
 * 0 :  op done, no errors.
 * -1:  op done, but errors occur.
 */

/*****************************************************************************/
/*
  Read status[C0H]:[0]bit OIP, judge whether the device is busy or not
*/
int spi_general_wait_ready(struct hisfc_operation *spi)
{
	unsigned int regval = 0;
	unsigned int timeout = 0;
	struct hisfc_host *host = (struct hisfc_host *)spi->host;

	do {
		spi_feature_op(host, GET_FEATURE, STATUS_REGISTER, &regval);
		if (!(regval & STATUS_OIP_MASK))
			return 0;

		udelay(1);
	} while (timeout++ < (40 << 20));

	printk("Wait spi nand chip ready timeout!\n");

	return 1;
}

/*****************************************************************************/
/*
  Send write enable command to SPI Nand, status[C0H]:[2]bit WEL must be set 1
*/
int spi_general_write_enable(struct hisfc_operation *spi)
{
	unsigned int regval;
	struct hisfc_host *host = (struct hisfc_host *)spi->host;

	hisfc_write(host, HISFC400_INTCLR, HISFC400_INTCLR_ALL);

	/* disable WP Pin */
	regval = hisfc_read(host, HISFC400_GLOBAL_CFG);
	regval &= ~HISFC400_GLOBAL_CFG_WP_ENABLE;
	hisfc_write(host, HISFC400_GLOBAL_CFG, regval);

	hisfc_write(host, HISFC400_OPCODE, SPI_CMD_WREN);

	hisfc_write(host, HISFC400_OP_CFG, HISFC400_OP_CFG_DIR_TRANS_ENABLE);

	hisfc_write(host, HISFC400_OP,
			(HISFC400_OP_SEL_CS(host->chipselect)
			| HISFC400_OP_OPCODE_EN
			| HISFC400_OP_START));

	HISFC400_CMD_WAIT_CPU_FINISH(host);

	spi_feature_op(host, GET_FEATURE, STATUS_REGISTER, &regval);

	if (!(regval & STATUS_WEL_MASK)) {
		printk("Config WEL failed, Status Register: 0x%x.\n", regval);
		return -1;
	}

	return 0;
}

/*****************************************************************************/
/*
  judge whether SPI Nand support QUAD read/write or not
*/
int hisfc400_iftype_quad_support(struct hisfc_operation *spi)
{
	if (spi->write->iftype == HISFC400_REG_IFCYCLE_QUAD 
	    || spi->read->iftype == HISFC400_REG_IFCYCLE_QUAD
	    || spi->read->iftype == HISFC400_REG_IFCYCLE_QUAD_ADDR)
		return 1;

	return 0;
}
/*****************************************************************************/
