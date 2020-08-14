/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-08-15
 *
******************************************************************************/
#ifndef HINFC610_GENH
#define HINFC610_GENH
/******************************************************************************/

#include <hinfc_gen.h>
#include "hinfc610.h"

enum hinfc610_ecc_reg {
	hinfc610_ecc_none   = 0x00,
	hinfc610_ecc_8bit   = 0x01,
	hinfc610_ecc_13bit  = 0x02,
	hinfc610_ecc_18bit  = 0x03,
	hinfc610_ecc_24bit  = 0x04,
	hinfc610_ecc_27bit  = 0x05,
	hinfc610_ecc_32bit  = 0x06,
	hinfc610_ecc_41bit  = 0x07,
	hinfc610_ecc_48bit  = 0x08,
	hinfc610_ecc_60bit  = 0x09,
	hinfc610_ecc_72bit  = 0x0a,
	hinfc610_ecc_80bit  = 0x0b,
};

enum hinfc610_page_reg {
	hinfc610_pagesize_2K    = 0x01,
	hinfc610_pagesize_4K    = 0x02,
	hinfc610_pagesize_8K    = 0x03,
	hinfc610_pagesize_16K   = 0x04,
	hinfc610_pagesize_32K   = 0x05,
};

int hinfc610_get_pagesize(struct hinfc_host *host);

void hinfc610_set_pagesize(struct hinfc_host *host, int pagesize);

int hinfc610_get_ecctype(struct hinfc_host *host);

void hinfc610_set_ecctype(struct hinfc_host *host, int ecctype);

/******************************************************************************/
#endif /* HINFC610_GENH */
