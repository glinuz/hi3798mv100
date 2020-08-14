/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_enable_trustzone.c
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

extern HI_S32 HI_UNF_ADVCA_SetTZEnable(HI_BOOL bValue);
extern HI_S32 HI_UNF_ADVCA_GetTZEnStatus(HI_BOOL *pbEn);

int main(void)
{
    HI_S32 ret = HI_SUCCESS;
    HI_BOOL bTZEnStatus = HI_FALSE;

	printf("Attention: This sample will enable the trustzone otp area too!!!\n");

    ret = HI_UNF_ADVCA_Init();
    if(HI_SUCCESS != ret)
    {
        printf("ADVCA Init failed!\n");
        return HI_FAILURE;
    }

    ret = HI_UNF_ADVCA_GetTZEnStatus(&bTZEnStatus);
    if(HI_SUCCESS != ret)
    {
        printf("Get trustzone status failed, ret = 0x%x\n", ret);
        HI_UNF_ADVCA_DeInit();
        return HI_FAILURE;
    }

	if(HI_TRUE == bTZEnStatus)
	{
    	printf("Trustzone is already enable, do not set again!\n");
    	HI_UNF_ADVCA_DeInit();
    	return HI_SUCCESS;
	}
	else
	{
	    ret = HI_UNF_ADVCA_SetTZEnable(HI_TRUE);
	    if(HI_SUCCESS != ret)
	    {
	    	printf("Enable trustzone failed! ret = 0x%x\n", ret);
	    	HI_UNF_ADVCA_DeInit();
	    	return HI_FAILURE;
	    }
	    else 
	    {
	    	printf("Enable trustzone success!\n");
	    	HI_UNF_ADVCA_DeInit();
	    	return HI_SUCCESS;
	    }
	}

	return HI_SUCCESS;
}

