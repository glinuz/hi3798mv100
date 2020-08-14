/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_logo.h
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef __DRV_HIFB_LOGO_H__
#define __DRV_HIFB_LOGO_H__


/*********************************add include here******************************/

#include "drv_hifb_config.h"
#include <linux/kthread.h>

/*****************************************************************************/


/*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */



#ifdef CFG_HIFB_LOGO_SUPPORT

/***************************** Macro Definition ******************************/
#define HIFB_BOOT_FMT_1555             5
#define HIFB_BOOT_FMT_8888             9

#define HIFB_LOGO_LAYER_HD0_ID          HIFB_LAYER_HD_0
#define HIFB_LOGO_LAYER_HD1_ID          HIFB_LAYER_HD_1

#define HIFB_LOGO_LAYER_WBC_HD0_ID      HIFB_LAYER_SD_0
#define HIFB_LOGO_LAYER_WBC_HD1_ID      HIFB_LAYER_SD_1

/*************************** Structure Definition ****************************/

typedef enum tagHIFB_LOGO_CHANNEL_E
{
    HIFB_LOGO_HD0     = 0,
    HIFB_LOGO_HD1     = 1,
    HIFB_LOGO_WBC_HD0 = 2,
    HIFB_LOGO_WBC_HD1 = 3,
    HIFB_LOGO_BUTT,
}HIFB_LOGO_LAYER_E;

typedef struct tagHIFB_LOGO_INFO_S
{
    HI_BOOL          bShow;
    HIFB_LAYER_ID_E  eLogoID;
    struct task_struct *pThreadTask;
    struct work_struct freeLogoMemWork;
}HIFB_GFX_LOGO_INFO_S;

typedef struct tagHIFB_LOGO_S
{
    HI_BOOL               Support4KLogo;
    HIFB_LOGO_DATA_S      stLogoDataInfo[HIFB_LOGO_BUTT];
    HIFB_GFX_LOGO_INFO_S  stGfxLogoInfo[HIFB_LOGO_BUTT];
}HIFB_LOGO_S;

/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/
HI_VOID DRV_HIFB_ResetLogoState    (HI_VOID);
HI_VOID DRV_HIFB_WhetherHasLogo    (HI_VOID);

HI_VOID DRV_HIFB_GetLogoData       (HI_U32 u32LayerId);
HI_VOID DRV_HIFB_GetBaseData       (HI_U32 u32LayerId);
HI_VOID DRV_HIFB_LogoToApp         (HI_U32 u32LayerId);
HI_VOID DRV_HIFB_CloseLogo         (HI_U32 u32LayerId);
HI_VOID DRV_HIFB_CloseLogoByMask   (HI_U32 u32LayerId);

#endif



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
