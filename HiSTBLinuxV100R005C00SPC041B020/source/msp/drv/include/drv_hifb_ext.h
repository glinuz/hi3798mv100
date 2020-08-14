/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_ext.h
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef __DRV_HIFB_EXT_H__
#define __DRV_HIFB_EXT_H__


/*********************************add include here******************************/
#include "hi_type.h"
#include "drv_pq_define.h"


/*****************************************************************************/


/*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */


/***************************** Macro Definition ******************************/
typedef HI_VOID (*FN_HIFB_SetLogoLayerEnable)(HI_BOOL);
typedef HI_S32  (*FN_HIFB_UpdatePqData)(HI_U32 u32UpdateType,PQ_PARAM_S * pstPqParam);



#define hifb_init_module_k       DRV_HIFB_ModInit_K
#define hifb_cleanup_module_k    DRV_HIFB_ModExit_K

/*************************** Structure Definition ****************************/
typedef struct
{
    FN_HIFB_SetLogoLayerEnable   pfnHifbSetLogoLayerEnable;
    FN_HIFB_UpdatePqData         pfnHifbUpdatePqData; 
} HIFB_EXPORT_FUNC_S;

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/
HI_S32  HIFB_DRV_ModInit(HI_VOID);
HI_VOID HIFB_DRV_ModExit(HI_VOID);

HI_S32  DRV_HIFB_ModInit_K(HI_VOID);
HI_VOID DRV_HIFB_ModExit_K(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
