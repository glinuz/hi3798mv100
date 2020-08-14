/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_getchipid.c
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


HI_S32 main(HI_S32 argc,HI_CHAR** argv)
{
    HI_S32 Ret;
    HI_U32 ChipID;

    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_ADVCA_GetChipId(&ChipID);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("get chip id failed\n");
        return HI_FAILURE;
    }
    printf("chip id:%#x\n",ChipID);
    (HI_VOID)HI_UNF_ADVCA_DeInit();   

    return Ret;
}


