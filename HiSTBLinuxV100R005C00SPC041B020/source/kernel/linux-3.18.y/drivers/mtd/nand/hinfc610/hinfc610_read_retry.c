/******************************************************************************
 *    NAND Flash Controller V610 Device Driver
 *    Copyright (c) 2012-2013 by Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By Czyong.
 *
******************************************************************************/
#include "hinfc610_gen.h"
#include "hinfc610.h"
#include "hinfc610_read_retry.h"

extern struct read_retry_t hinfc610_hynix_bg_cdie_read_retry;
extern struct read_retry_t hinfc610_hynix_bg_bdie_read_retry;
extern struct read_retry_t hinfc610_hynix_cg_adie_read_retry;
extern struct read_retry_t hinfc610_micron_read_retry;
extern struct read_retry_t hinfc610_toshiba_24nm_read_retry;
extern struct read_retry_t hinfc610_toshiba_19nm_read_retry;
extern struct read_retry_t hinfc610_samsung_read_retry;

static struct read_retry_t *read_retry_table[] = {
	&hinfc610_hynix_bg_bdie_read_retry,
	&hinfc610_hynix_bg_cdie_read_retry,
	&hinfc610_hynix_cg_adie_read_retry,
	&hinfc610_micron_read_retry,
	&hinfc610_toshiba_24nm_read_retry,
	&hinfc610_toshiba_19nm_read_retry,
	&hinfc610_samsung_read_retry,
	NULL,
};

struct read_retry_t *hinfc610_find_read_retry(int type)
{
	struct read_retry_t **rr;

	for (rr = read_retry_table; rr; rr++) {
		if ((*rr)->type == type)
			return *rr;
	}

	return NULL;
}
