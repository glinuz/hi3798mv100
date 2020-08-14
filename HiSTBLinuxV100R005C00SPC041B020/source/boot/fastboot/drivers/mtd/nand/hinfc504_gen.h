/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-08-15
 *
******************************************************************************/
#ifndef HINFC504_GENH
#define HINFC504_GENH
/******************************************************************************/

#include <hinfc_gen.h>

enum hinfc504_ecc_reg {
	hinfc504_ecc_none    = 0x00,
	hinfc504_ecc_1bit    = 0x01,
	hinfc504_ecc_4bit    = 0x02,
	hinfc504_ecc_4bytes  = 0x02,
	hinfc504_ecc_8bytes  = 0x03,
	hinfc504_ecc_24bit1k = 0x04,
	hinfc504_ecc_40bit1k = 0x05,
};

enum hinfc504_page_reg {
	hinfc504_pagesize_512   = 0x00,
	hinfc504_pagesize_2K    = 0x01,
	hinfc504_pagesize_4K    = 0x02,
	hinfc504_pagesize_8K    = 0x03,
	hinfc504_pagesize_16K   = 0x04,
};

enum hinfc504_page_reg hinfc504_page_type2reg(int type);

int hinfc504_page_reg2type(enum hinfc504_page_reg reg);

enum hinfc504_ecc_reg hinfc504_ecc_type2reg(int type);

int hinfc504_ecc_reg2type(enum hinfc504_ecc_reg reg);

/******************************************************************************/
#endif /* HINFC504_GENH */
