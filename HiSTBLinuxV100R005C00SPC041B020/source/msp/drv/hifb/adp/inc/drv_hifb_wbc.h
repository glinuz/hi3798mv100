/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_wbc.h
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef __DRV_HIFB_WBC_H__
#define __DRV_HIFB_WBC_H__


/*********************************add include here******************************/
#include "drv_hifb_hal.h"
#include "drv_hifb_adp.h"
#include "optm_define.h"
#include "optm_alg_csc.h"
#include "optm_alg_gzme.h"
#include "optm_alg_gsharp.h"
#include "hi_drv_disp.h"
#include "hi_gfx_comm_k.h"


/*****************************************************************************/


/*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */


/***************************** Macro Definition ******************************/


/**
 **使用的回写buffer数，LOGO只需要一块就可以了，因为只回写一次
 **/
#ifndef HI_BUILD_IN_BOOT
    #define OPTM_WBCBUFFER_NUM               2
#else
    #define OPTM_WBCBUFFER_NUM               1
#endif

/*************************** Structure Definition ****************************/

/***************************  The enum of Jpeg image format  ******************/



typedef enum tagOPTM_WBC_MODE_E{
    OPTM_WBC_MODE_MONO      = 0x0,
    OPTM_WBC_MODE_LFET_EYE  = 0x2,
    OPTM_WBC_MODE_RIGHT_EYE = 0x3,
    OPTM_WBC_MODE_BUTT,
}OPTM_WBC_MODE_E;

typedef struct tagOPTM_GFX_WBC_S{
    HI_BOOL                IsOpen;
    HI_BOOL                bWorking;
    HI_BOOL                bStartWork;
    HI_BOOL                bEndWork;
    OPTM_VDP_LAYER_WBC_E   enWbcHalId;
    HI_S32                 s32BufferWidth;
    HI_S32                 s32BufferHeight;
    HI_U32                 u32BufferStride;
    HI_U32                 u32BufIndex;
    HI_S32                 s32WbcCnt;
    HI_U32                 u32WBCBuffer[OPTM_WBCBUFFER_NUM];
    HI_U32                 u32WriteBufAddr;
    HI_U32                 u32ReadBufAddr;
    GFX_MMZ_BUFFER_S       stFrameBuffer;
    HI_U32                 u32DataPoint;  /* 0, feeder; others, reserve */
    HIFB_COLOR_FMT_E       enDataFmt;
    HIFB_RECT              stInRect;
    HI_BOOL                bInProgressive;
    HIFB_RECT              stOutRect;
    HI_BOOL                bOutProgressive;
    HI_U32                 u32BtmOffset;
    HI_BOOL                bHdDispProgressive;
    OPTM_VDP_DITHER_E      enDitherMode;
    OPTM_VDP_WBC_OFMT_E    stWBCFmt;
    OPTM_VDP_DATA_RMODE_E  enReadMode;
    OPTM_VDP_DATA_RMODE_E  enOutMode;
    OPTM_WBC_MODE_E        enWbcMode;
    OPTM_VDP_INTMSK_E      enWbcInt;
}OPTM_GFX_WBC_S;


#ifdef HI_BUILD_IN_BOOT
typedef struct
{
    HI_U32 u32Stride;
    HI_U32 u32ReadBufAddr;
}HIFB_WBC_DATA_S;
#endif

/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/


/***************************************************************************************
* func          : HIFB_WBC2_SetRegUp
* description   : CNcomment:更新回写寄存器 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 HIFB_WBC2_SetRegUp(OPTM_VDP_LAYER_WBC_E enWbcID);



HI_VOID HIFB_WBC_DinitLayer (HIFB_LAYER_ID_E enLayerId);
HI_VOID HIFB_WBC_InitLayer  (HIFB_LAYER_ID_E enLayerId);

#ifndef HI_BUILD_IN_BOOT
HI_VOID HIFB_WBC_StartWorkQueue(struct work_struct *data);
#endif


/***************************************************************************************
* func          : HIFB_WBC2_SetTcFlag
* description   : CNcomment: 设置TC回写标记 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID HIFB_WBC2_SetTcFlag(HI_BOOL bFlag);

/***************************************************************************************
* func          : HIFB_WBC2_SetCropReso
* description   : CNcomment: 设置WBC_GP0裁剪分辨率，也就输入的起始和结束坐标 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID HIFB_WBC2_SetCropReso(OPTM_VDP_DISP_RECT_S stInputRect);

/***************************************************************************************
* func          : HIFB_WBC2_SetPreZmeEn
* description   : CNcomment: 设置ZME使能 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID HIFB_WBC2_SetPreZmeEn(OPTM_ALG_GZME_DRV_PARA_S *pstZmeDrvPara);



HI_VOID HIFB_WBC2_Init  (HI_VOID);
HI_VOID HIFB_WBC2_Dinit (HI_VOID);

/***************************************************************************************
* func          : HIFB_WBC2_SetEnable
* description   : CNcomment: 设置回写使能状态 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID HIFB_WBC2_SetEnable(HI_BOOL bEnable);

/***************************************************************************************
* func          : HIFB_WBC2_CfgSlvLayer
* description   : CNcomment: 配置标清图层 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 HIFB_WBC2_CfgSlvLayer(HIFB_LAYER_ID_E enLayerId,HI_RECT_S *pstRect);

#ifndef HI_BUILD_IN_BOOT
HI_S32 HIFB_WBC_FrameEndProcess(OPTM_GFX_GP_E enGpId);
HI_S32 HIFB_WBC_Process        (OPTM_GFX_GP_E enGpId);
HI_S32 HIFB_WBC_GetLayerInfo   (HIFB_SLVLAYER_DATA_S *pstLayerInfo);
#else
HI_VOID HIFB_WBC_BOOT_GetWbcInfo  (HIFB_WBC_DATA_S *pstWbcInfo);
HI_VOID HIFB_WBC_BOOT_ReFresh     (HI_BOOL Support4KLogo);
#endif

/***************************************************************************************
* func          : HIFB_WBC2_Recovery
* description   : CNcomment: 重新设置WBC和回写图层属性 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID HIFB_WBC2_Recovery(OPTM_GFX_GP_E enGPId);

/***************************************************************************************
* func          : HIFB_WBC2_Reset
* description   : CNcomment: 回写状态复位 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID HIFB_WBC2_Reset(HI_BOOL bWbcRegUp);

#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __HIFB_WBC_H__ */
