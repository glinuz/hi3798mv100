/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_setVersionID.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2015-06-09
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
    HI_U8 au8VersionId[4] = {0}; 

    Ret = HI_UNF_ADVCA_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_ADVCA_GetVersionId(au8VersionId);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_GetVersionId failed\n");
        return HI_FAILURE;
    }
    HI_DEBUG_ADVCA("HI_UNF_ADVCA_GetVersionId success\n");
    printf("version id: 0x%02x 0x%02x 0x%02x 0x%02x\n", au8VersionId[0],au8VersionId[1],au8VersionId[2],au8VersionId[3]);

    (HI_VOID)HI_UNF_ADVCA_DeInit();

    return Ret;
}





