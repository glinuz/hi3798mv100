/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-07-05
 *
******************************************************************************/
#include "spi_nand_chip.h"

#ifndef HISFC400_ECC_INFOH
#define HISFC400_ECC_INFOH

struct nand_ctrl_info_t *hisfc400_get_best_ecc(struct spi_nand_chip_info_t *nand_info);
struct nand_ctrl_info_t *hisfc400_force_ecc(struct spi_nand_chip_info_t *nand_info, 
					    int pagetype, int ecctype, 
					    char *cfgmsg, int allow_pagediv);


#endif /* HISFC400_ECC_INFOH */
