/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
* 
******************************************************************************
File Name           : boot_adp.c
Version             : Initial Draft
Author              : 
Created             : 2015/07/02
Description         : display初始化
Function List       : 
History             :
Date                       Author                   Modification
2015/07/02                                          modify by y00181162        
******************************************************************************/

/*********************************add include here******************************/
#include <uboot.h>
#include "hi_common.h"
#include "hi_unf_disp.h"
#include "hi_boot_logo.h"
#include "hi_boot_adp.h"
#include "hi_unf_hdmi.h"

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/*****************************************************************************
* func			   HI_ADP_DispInit
* description	:  display初始化
* retval		:  NA
* others:		:  NA
*****************************************************************************/
HI_S32 HI_ADP_DispInit(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_UNF_DISP_Init();
    if(HI_SUCCESS != s32Ret){
        HI_ERR_MCE("HI_UNF_DISP_Init err! s32Ret = %x\n", s32Ret);
        goto ERR0;
    }
  
    s32Ret = HI_UNF_DISP_Open(HI_UNF_DISPLAY0);
    if(HI_SUCCESS != s32Ret){
        HI_ERR_MCE("HI_UNF_DISP_Open err! Ret = %x\n", s32Ret);
        goto ERR1; 
    }  
    
    s32Ret = HI_UNF_DISP_Open(HI_UNF_DISPLAY1);
    if(HI_SUCCESS != s32Ret){
        HI_ERR_MCE("HI_UNF_DISP_Open err! Ret = %x\n", s32Ret);
        goto ERR2; 
    }
    
    return s32Ret;

ERR2:
    HI_UNF_DISP_Close(HI_UNF_DISPLAY0);
ERR1:
    HI_UNF_DISP_DeInit();
ERR0:
    return s32Ret;
}
