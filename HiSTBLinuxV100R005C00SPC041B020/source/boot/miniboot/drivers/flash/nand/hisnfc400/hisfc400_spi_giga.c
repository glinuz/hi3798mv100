/*****************************************************************************
 *    Copyright (c) 2014 by Hisilicon
 *    All rights reserved.
 *****************************************************************************/

#include "hisfc400.h"

/*****************************************************************************/
/*
  Send set features command to SPI Nand, feature[B0H]:[0]bit QE would be set
*/
int spi_giga_qaud_enable(struct hisfc_operation *spi)
{
	unsigned int regval;
	unsigned int host_qe_support = 0;
	struct hisfc_host *host = (struct hisfc_host *)spi->host;
	extern int hisfc400_iftype_quad_support(struct hisfc_operation *spi); 

	host_qe_support = hisfc400_iftype_quad_support(spi);
	spi_feature_op(host, GET_FEATURE, FEATURE_REGISTER, &regval);
	if ((regval & FEATURE_QUAD_ENABLE) && (!host_qe_support)) {
		regval &= ~FEATURE_QUAD_ENABLE;
		spi_feature_op(host, SET_FEATURE, FEATURE_REGISTER, &regval);

	} else if (!(regval & FEATURE_QUAD_ENABLE) && host_qe_support) {
		regval |= FEATURE_QUAD_ENABLE;
		spi_feature_op(host, SET_FEATURE, FEATURE_REGISTER, &regval);

	} else
		return 0;

	return 0;
}
