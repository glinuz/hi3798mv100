/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_proc.h
Version         : Initial Draft
Author          :
Created         : 2016/08/27
Description     :
Function List   :


History         :
Date                  Author                Modification
2016/08/27            y00181162              Created file
******************************************************************************/
#ifndef __DRV_HIFB_PROC_H__
#define __DRV_HIFB_PROC_H__


/*********************************add include here******************************/

#include "drv_hifb_config.h"


/*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */


#ifdef CFG_HIFB_PROC_SUPPORT

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/
typedef struct tagDrvHiFBProcS
{
    HI_BOOL bCreateProc;
    HI_BOOL bHasDeCmpErr;
    HI_BOOL bDebugByMoreMessage;
    HI_BOOL b3DStatus;
    HI_BOOL bHwcRefreshInDeCmpStatus;  /** decompress status from hard composer refresh  */
    HI_U32  WbcLayerID;
    HI_U32  VdpIntMaskValue;
    HI_U32  HwcRefreshInDispFmt;      /** display pixel format from hard composer refresh */
    HI_U32  HwcRefreshInDispStride;   /** display stride from hard composer refresh       */
    HI_U32  HwcRefreshInDispAdress;   /** display address from hard composer refresh      */
    HI_U32  HwcRefreshUnSyncCnt;      /** account unsync by hwc refresh                   */
    HI_U32  DeCmpARDataStatus[2];
    HI_U32  DeCmpGBDataStatus[2];
    HI_U32  DeCmpARDataErrCnt;        /** account decompress AR data error times          */
    HI_U32  DeCmpGBDataErrCnt;        /** account decompress GB data error times          */
}DRV_HIFB_PROC_S;


/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/
HI_S32  DRV_HIFB_CreateProc   (HI_U32 LayerID);
HI_VOID DRV_HIFB_DestoryProc  (HI_U32 LayerID);


#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
