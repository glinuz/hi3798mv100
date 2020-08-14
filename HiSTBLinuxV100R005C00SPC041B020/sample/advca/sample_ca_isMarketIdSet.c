/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_ismarketidset.c
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
    HI_BOOL bIsMarketIdSet = HI_FALSE;

    Ret = HI_UNF_ADVCA_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
    }   

    Ret = HI_UNF_ADVCA_IsMarketIdSet(&bIsMarketIdSet);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("check is marketId set failed\n");
        return HI_FAILURE;
    }

    printf("bIsMarketIdSet:0x%x\n", bIsMarketIdSet);
    (HI_VOID)HI_UNF_ADVCA_DeInit();

    return Ret;
}



