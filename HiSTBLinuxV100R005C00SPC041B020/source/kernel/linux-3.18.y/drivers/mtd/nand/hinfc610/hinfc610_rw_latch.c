/******************************************************************************
 *    Copyright (c) 2009-2014 by  Hisilicon Tech. Co., Ltd.
 *    All rights reserved.
 * ***
 *    Create By Czyong
******************************************************************************/
#include <linux/string.h>

#include "hinfc610_rw_latch.h"
/*****************************************************************************/

//extern struct hinfc_rw_latch *hinfc_rw_latch_table;

struct hinfc_rw_latch *get_hinfc_rw_latch(unsigned char id[8])
{
	struct hinfc_rw_latch *rw_latch_table;

	for (rw_latch_table = hinfc_rw_latch_table; 
	     rw_latch_table && rw_latch_table->length; rw_latch_table++) {
		if (!memcmp(id, rw_latch_table->id, rw_latch_table->length))
			break;
	}

	return rw_latch_table;
}
