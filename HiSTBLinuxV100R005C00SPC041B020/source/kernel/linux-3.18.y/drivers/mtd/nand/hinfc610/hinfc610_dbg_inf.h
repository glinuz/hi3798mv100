/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-07-04
 *
******************************************************************************/
#ifndef HINFC610_DBG_INFH
#define HINFC610_DBG_INFH
/******************************************************************************/

int hinfc610_dbg_init(struct hinfc_host *host);

void hinfc610_dbg_write(struct hinfc_host *host);

void hinfc610_dbg_erase(struct hinfc_host *host);

void hinfc610_dbg_read(struct hinfc_host *host);

void hinfc610_dbg_read_retry(struct hinfc_host *host, int index);

/******************************************************************************/
#endif /* HINFC610_DBG_INFH */
