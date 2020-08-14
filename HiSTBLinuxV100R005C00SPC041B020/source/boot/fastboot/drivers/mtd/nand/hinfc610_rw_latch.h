/******************************************************************************
 *    Copyright (c) 2009-2014 by  Hisilicon Tech. Co., Ltd.
 *    All rights reserved.
 * ***
 *    Create By Czyong
******************************************************************************/
#ifndef HINFC_RW_LATCH_H
#define HINFC_RW_LATCH_H

struct hinfc_rw_latch {
	char *name;
	unsigned char id[8];
	int length;
	unsigned int rw_latch;
};

struct hinfc_rw_latch *get_hinfc_rw_latch(unsigned char id[8]);

extern struct hinfc_rw_latch *hinfc_rw_latch_table;

#endif /* HINFC_RW_LATCH_H */
