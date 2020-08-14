/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-07-05
 *
******************************************************************************/

#ifndef HINFC610_ECC_INFOH
#define HINFC610_ECC_INFOH

/*
 * oob area for user, this MACRO used by cache.
 * include all mark in oob area.
 */
#define HINFC610_OOB_OFFSET       0
#define HINFC610_BB_OFFSET        0
#define HINFC610_EPM_OFFSET       28
#define HINFC610_OOB_SIZE         32

int hinfc610_ecc_info_init(struct hinfc_host *host);

struct nand_ctrl_info_t *hinfc610_get_best_ecc(struct mtd_info *mtd);

struct nand_ctrl_info_t *hinfc610_force_ecc(struct mtd_info *mtd, int pagesize,
					     int ecctype, char *cfgmsg,
					     int allow_pagediv);

#endif /* HINFC610_ECC_INFOH */
