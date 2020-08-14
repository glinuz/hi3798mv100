/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_getTrustzoneStatus.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2014-04-12
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hi_type.h"
#include "hi_common.h"
#include "hi_unf_advca.h"

extern HI_S32 HI_UNF_ADVCA_GetTZEnStatus(HI_BOOL *pbEn);

int main(void)
{
    HI_S32 ret = HI_SUCCESS;
    HI_BOOL bTZEnStatus = HI_FALSE;

    ret = HI_UNF_ADVCA_Init();
    if(HI_SUCCESS != ret)
    {
        printf("ADVCA Init failed!\n");
        return HI_FAILURE;
    }

    ret = HI_UNF_ADVCA_GetTZEnStatus(&bTZEnStatus);
    if(HI_SUCCESS != ret)
    {
        printf("Get advca tz status failed, ret = 0x%x\n", ret);
        HI_UNF_ADVCA_DeInit();
        return HI_FAILURE;
    }

    printf("Trustzone status: %s\n",(bTZEnStatus == HI_TRUE)?"enabled":"not enabled");

	(HI_VOID)HI_UNF_ADVCA_DeInit();

	return HI_SUCCESS;
}

