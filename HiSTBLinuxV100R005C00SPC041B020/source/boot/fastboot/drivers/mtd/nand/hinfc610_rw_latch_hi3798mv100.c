/******************************************************************************
 *    Copyright (c) 2009-2014 by  Hisilicon Tech. Co., Ltd.
 *    All rights reserved.
 * ***
 *    Create By Czyong
******************************************************************************/

#include "hinfc610_rw_latch.h"
/*****************************************************************************/

struct hinfc_rw_latch hinfc610_rw_latch_hi3798mv100[] = {
	{        /* MLC 40bit/1k */
		.name      = "MT29F32G08CBADA",
		.id        = {0x2C, 0x44, 0x44, 0x4B, 0xA9, 0x00, 0x00, 0x00},
		.length    = 8,
		/* datasheet says tRC=tWC=16ns(min);
		 * nand interface clk:200MHz, 
		 * pwidth regsiter: 0x117,
		 * tRC=tWC=5ns * (2+3) = 25ns.
		 */
		.rw_latch  = 0x127,
	}, {
		.name   = "general",
		.id     = {0x00},
		.length = 0,
		.rw_latch = 0x375,
	}
};

struct hinfc_rw_latch *hinfc_rw_latch_table = hinfc610_rw_latch_hi3798mv100;

