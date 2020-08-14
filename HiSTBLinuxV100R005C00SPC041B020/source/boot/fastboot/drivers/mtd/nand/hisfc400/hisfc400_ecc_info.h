/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-07-05
 *
******************************************************************************/

#ifndef HISFC400_ECC_INFOH
#define HISFC400_ECC_INFOH

struct nand_ctrl_info_t *hisfc400_get_best_ecc(struct mtd_info *mtd);

struct nand_ctrl_info_t *hisfc400_force_ecc(struct mtd_info *mtd, int pagetype,
					     int ecctype, char *cfgmsg,
					     int allow_pagediv);

#endif /* HISFC400_ECC_INFOH */
