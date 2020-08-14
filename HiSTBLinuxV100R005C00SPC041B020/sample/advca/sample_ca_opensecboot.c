/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_opensecboot.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hi_type.h"
#include "hi_unf_advca.h"
#include "sample_ca_common.h"

HI_S32 main(HI_S32 argc,HI_CHAR**argv)
{
    HI_S32 Ret;
    HI_UNF_ADVCA_FLASH_TYPE_E enFlashType;

    if (argc < 2)
    {
        printf("usage:opensecboot flashtype[spi|nand|emmc|sd|spi_nand]\n");
        return HI_FAILURE;
    }

    if(0 == strcmp(argv[1],"spi"))
    {
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_SPI;
    }
    else if(0 == strcmp(argv[1],"nand"))
    {
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_NAND;
    }
    else if(0 == strcmp(argv[1],"emmc"))
    {
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_EMMC;
    }
    else if(0 == strcmp(argv[1],"spi_nand"))
    {
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_SPI_NAND;
    }
    else if(0 == strcmp(argv[1],"sd"))
    {
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_SD;
    }
    else
    {
        printf("usage:opensecboot flashtype[spi|nand|emmc|sd|spi_nand]\n");
        return HI_FAILURE;
    }


    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_ADVCA_EnableSecBoot(enFlashType);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Enable Secure Boot failed\n");
        (HI_VOID)HI_UNF_ADVCA_DeInit();   
        return HI_FAILURE;
    }

    (HI_VOID)HI_UNF_ADVCA_DeInit();   

    return Ret;
}


