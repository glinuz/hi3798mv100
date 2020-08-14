/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_setmarketid.c
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
#include "hi_type.h"
#include "hi_unf_advca.h"
#include "sample_ca_common.h"

/***************************** Macro Definition ******************************/



/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/
HI_S32 main(HI_S32 argc,HI_CHAR** argv)
{
    HI_S32 Ret;
    HI_U8 au8MarketId[4] = {0x12,0x34,0x56,0x78}; 

    Ret = HI_UNF_ADVCA_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
    }   

    Ret = HI_UNF_ADVCA_GetMarketId(au8MarketId);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_GetMarketId failed\n");
        return HI_FAILURE;
    }
    printf("market id: 0x%02x 0x%02x 0x%02x 0x%02x\n", au8MarketId[0], au8MarketId[1], au8MarketId[2], au8MarketId[3]);

    HI_DEBUG_ADVCA("HI_UNF_ADVCA_GetMarketId success\n");
    (HI_VOID)HI_UNF_ADVCA_DeInit();

    return Ret;
}





