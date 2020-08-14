/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_osi.c
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/


/*********************************add include here******************************/
#ifndef HI_BUILD_IN_BOOT
#include <linux/string.h>
#include <linux/fb.h>
#else
#include "hifb_debug.h"
#endif
#include "drv_hifb_adp.h"
#include "hifb_drv.h"
#include "drv_hifb_config.h"

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/
static OPTM_GFX_OPS_S g_stGfxOps;

/******************************* API declaration *****************************/

/******************************* API release *********************************/

/***************************************************************************************
* func          : OPTM_GetGfxGpId
* description   : CNcomment: 根据图层ID获取GP ID CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
OPTM_GFX_GP_E OPTM_GetGfxGpId(HIFB_LAYER_ID_E enLayerId)
{
    if(enLayerId <= HIFB_LAYER_HD_3)
    {
        return OPTM_GFX_GP_0;
    }
    else if(enLayerId >= HIFB_LAYER_SD_0 && enLayerId <= HIFB_LAYER_SD_1)
    {
        return OPTM_GFX_GP_1;
    }

    return OPTM_GFX_GP_BUTT;
}

/***************************************************************************************
* func          : HIFB_DRV_SetLayerKeyMask
* description   : CNcomment: 设置图层color key CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 HIFB_DRV_SetLayerKeyMask(HIFB_LAYER_ID_E enLayerId, const HIFB_COLORKEYEX_S *pstColorkey)
{
   return g_stGfxOps.OPTM_GfxSetLayKeyMask(enLayerId, pstColorkey);
}
/***************************************************************************************
* func          : HIFB_DRV_EnableLayer
* description   : CNcomment: 图层使能 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 HIFB_DRV_EnableLayer(HIFB_LAYER_ID_E enLayerId, HI_BOOL bEnable)
{
    return g_stGfxOps.OPTM_GfxSetEnable(enLayerId, bEnable);;
}


/***************************************************************************************
* func            : HIFB_DRV_SetLayerAddr
* description    : CNcomment: 设置显示地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 HIFB_DRV_SetLayerAddr(HIFB_LAYER_ID_E enLayerId, HI_U32 u32Addr)
{
    return g_stGfxOps.OPTM_GfxSetLayerAddr(enLayerId, u32Addr);
}

/***************************************************************************************
* func            : HIFB_DRV_GetLayerAddr
* description    : CNcomment: 获取显示地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 HIFB_DRV_GetLayerAddr(HIFB_LAYER_ID_E enLayerId,HI_U32 *pu32Addr)
{
    return g_stGfxOps.OPTM_GfxGetLayerAddr(enLayerId, pu32Addr);
}

/***************************************************************************************
* func            : HIFB_DRV_SetLayerStride
* description    : CNcomment: 设置图层行间距 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 HIFB_DRV_SetLayerStride(HIFB_LAYER_ID_E enLayerId, HI_U32 u32Stride)
{
    return g_stGfxOps.OPTM_GfxSetLayerStride(enLayerId, u32Stride);
}

/***************************************************************************************
* func            : HIFB_DRV_SetLayerDataFmt
* description    : CNcomment: 设置图层像素格式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 HIFB_DRV_SetLayerDataFmt(HIFB_LAYER_ID_E enLayerId, HIFB_COLOR_FMT_E enDataFmt)
{
    if(enDataFmt >= HIFB_FMT_BUTT){
           return HI_FAILURE;
    }

    return g_stGfxOps.OPTM_GfxSetLayerDataFmt(enLayerId, enDataFmt);
}

HI_S32 HIFB_DRV_SetColorReg(HIFB_LAYER_ID_E enLayerId, HI_U32 u32OffSet, HI_U32 u32Color, HI_S32 UpFlag)
{
    if (u32OffSet > 255)
    {
        HIFB_ERROR("GFX color clut offset > 255.\n");
        return HI_FAILURE;
    }
    return g_stGfxOps.OPTM_GfxSetColorReg(enLayerId, u32OffSet, u32Color, UpFlag);
}

HI_S32 HIFB_DRV_WaitVBlank(HIFB_LAYER_ID_E enLayerId)
{
    return g_stGfxOps.OPTM_GfxWaitVBlank(enLayerId);
}

/***************************************************************************************
* func          : HIFB_DRV_SetLayerDeFlicker
* description   : CNcomment: 设置图层抗闪 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 HIFB_DRV_SetLayerDeFlicker(HIFB_LAYER_ID_E enLayerId, HIFB_DEFLICKER_S *pstDeFlicker)
{
    return g_stGfxOps.OPTM_GfxSetLayerDeFlicker(enLayerId, pstDeFlicker);
}

/***************************************************************************************
* func          : HIFB_DRV_SetLayerAlpha
* description   : CNcomment: 设置图层alpha的值 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 HIFB_DRV_SetLayerAlpha(HIFB_LAYER_ID_E enLayerId, HIFB_ALPHA_S *pstAlpha)
{
    return g_stGfxOps.OPTM_GfxSetLayerAlpha(enLayerId, pstAlpha);
}

/***************************************************************************************
* func          : HIFB_DRV_GetGPGalphaSum
* description   : CNcomment: 获取GP全局alpha值为0的像素点个数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 HIFB_DRV_GetGPGalphaSum(HIFB_LAYER_ID_E enLayerId, HI_U32 *pu32GalphaSum)
{
    return g_stGfxOps.OPTM_GfxGetLayerGalphaSum(enLayerId, pu32GalphaSum);
}


HI_S32 HIFB_DRV_GetLayerInRect(HIFB_LAYER_ID_E enLayerId, HIFB_RECT *pstInputRect)
{
     HI_S32 s32Ret;

     s32Ret = HI_SUCCESS;

     if (HI_NULL != pstInputRect)
     {
         s32Ret |=    g_stGfxOps.OPTM_GfxGetLayerRect(enLayerId, pstInputRect);
     }

     return s32Ret;
}

 /***************************************************************************************
 * func          : HIFB_DRV_SetLayerInRect
 * description     : CNcomment: 设置图层输入矩形 CNend\n
 * param[in]     : HI_VOID
 * retval         : NA
 * others:         : NA
 ***************************************************************************************/
 HI_S32 HIFB_DRV_SetLayerInRect(HIFB_LAYER_ID_E enLayerId, const HIFB_RECT *pstInputRect)
 {
     HI_S32 s32Ret;

     s32Ret = HI_SUCCESS;

     if (HI_NULL != pstInputRect)
     {
         s32Ret |=    g_stGfxOps.OPTM_GfxSetLayerRect(enLayerId, pstInputRect);
     }

     return s32Ret;
 }

 HI_S32 HIFB_DRV_SetLayerOutRect(HIFB_LAYER_ID_E enLayerId, const HIFB_RECT *pstOutputRect)
{
    HI_S32 s32Ret;

    s32Ret = HI_SUCCESS;

    if (HI_NULL != pstOutputRect)
    {
        s32Ret |= g_stGfxOps.OPTM_GfxSetGpRect(OPTM_GetGfxGpId(enLayerId), pstOutputRect);
    }

    return s32Ret;
}

HI_S32 HIFB_DRV_SetLayerScreenSize(HIFB_LAYER_ID_E enLayerId, HI_U32 u32Width, HI_U32 u32Height)
{

    return g_stGfxOps.OPTM_GfxSetGpInPutSize(OPTM_GetGfxGpId(enLayerId), u32Width, u32Height);
}

HI_S32 HIFB_DRV_GetLayerOutRect(HIFB_LAYER_ID_E enLayerId, HIFB_RECT *pstOutputRect)
{
    HI_S32 s32Ret;

    s32Ret = HI_SUCCESS;

    if (HI_NULL != pstOutputRect)
    {
        s32Ret |= g_stGfxOps.OPTM_GfxGetOutRect(enLayerId, pstOutputRect);
    }

    return s32Ret;
}

HI_S32 HIFB_DRV_GetDispSize(HIFB_LAYER_ID_E enLayerId, HIFB_RECT *pstOutputRect)
{
    HI_S32 s32Ret;

    s32Ret = HI_SUCCESS;

    if (HI_NULL != pstOutputRect)
    {
        s32Ret |= g_stGfxOps.OPTM_GfxGetDispFMTSize(OPTM_GetGfxGpId(enLayerId), pstOutputRect);
    }

    return s32Ret;
}


/***************************************************************************************
* func          : HIFB_DRV_SetTriDimMode
* description   : CNcomment: 设置3D模式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
HI_S32 HIFB_DRV_SetTriDimMode(HI_U32 u32LayerId, HIFB_STEREO_MODE_E enStereoMode, HIFB_STEREO_MODE_E enWbcSteroMode)
{
    if( HIFB_STEREO_MONO == enStereoMode)
    {
        g_stGfxOps.OPTM_GfxSetTriDimEnable(u32LayerId, HI_FALSE);
    }
    else
    {
        g_stGfxOps.OPTM_GfxSetTriDimEnable(u32LayerId, HI_TRUE);
    }
    return g_stGfxOps.OPTM_GfxSetTriDimMode(u32LayerId, enStereoMode, enWbcSteroMode);
}

/***************************************************************************************
* func            : HIFB_DRV_SetTriDimAddr
* description    : CNcomment: 设置3D显示地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 HIFB_DRV_SetTriDimAddr(HI_U32 u32LayerId, HI_U32 u32TriDimAddr)
{
    g_stGfxOps.OPTM_GfxSetTriDimAddr(u32LayerId, u32TriDimAddr);
    return HI_SUCCESS;
}
#endif


HI_S32 HIFB_DRV_ColorConvert(const struct fb_var_screeninfo *pstVar, HIFB_COLORKEYEX_S *pCkey)
{
    HI_U8 rOff, gOff, bOff;

    rOff = pstVar->red.length;
    gOff = pstVar->green.length;
    bOff = pstVar->blue.length;

    pCkey->u8RedMask = (0xff >> rOff);
    pCkey->u8GreenMask = (0xff >> gOff);
    pCkey->u8BlueMask  = (0xff >> bOff);

    return HI_SUCCESS;
}
//#endif
HI_S32 HIFB_DRV_UpdataLayerReg(HIFB_LAYER_ID_E enLayerId)
{
    return g_stGfxOps.OPTM_GfxUpLayerReg(enLayerId);
}


/***************************************************************************************
* func          : HIFB_DRV_SetLayerPreMult
* description   : CNcomment: 设置图层预乘 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 HIFB_DRV_SetLayerPreMult(HIFB_LAYER_ID_E enLayerId, HI_BOOL bEnable)
{
    return g_stGfxOps.OPTM_GfxSetLayerPreMult(enLayerId, bEnable);
}


HI_S32 HIFB_DRV_SetIntCallback(HIFB_CALLBACK_TPYE_E eCallbackType, IntCallBack pCallBack, HIFB_LAYER_ID_E enLayerId)
{
    return g_stGfxOps.OPTM_GfxSetCallback(enLayerId, pCallBack, eCallbackType);
}


/***************************************************************************************
* func          : HIFB_DRV_OpenLayer
* description   : CNcomment: 打开对应的图层 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 HIFB_DRV_OpenLayer(HIFB_LAYER_ID_E enLayerId)
{
    return g_stGfxOps.OPTM_GfxOpenLayer(enLayerId);
}

HI_VOID HIFB_DRV_GetOSDData(HIFB_LAYER_ID_E enLayerId, HIFB_OSD_DATA_S *pstLayerData)
{
    g_stGfxOps.OPTM_GfxGetOSDData(enLayerId, pstLayerData);
}

HI_VOID DRV_HIFB_OSI_GetLogoData(HIFB_LAYER_ID_E enLayerId, HIFB_LOGO_DATA_S *pstLogoData)
{
    g_stGfxOps.HIFB_ADP_GetLogoData(enLayerId, pstLogoData);
}


HI_S32 HIFB_DRV_CloseLayer(HIFB_LAYER_ID_E enLayerId)
{
    return g_stGfxOps.OPTM_GfxCloseLayer(enLayerId);
}


HI_S32 HIFB_DRV_GfxInit(HI_VOID)
{
    OPTM_GFX_GetOps(&g_stGfxOps);
    return g_stGfxOps.OPTM_GfxInit();
}


/***************************************************************************************
* func          : HIFB_DRV_GfxDeInit
* description   : CNcomment: 图形设备去初始化 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 HIFB_DRV_GfxDeInit(HI_VOID)
{
    return g_stGfxOps.OPTM_GfxDeInit();
}

HI_S32 HIFB_DRV_SetLayerPriority(HIFB_LAYER_ID_E enLayerId, HIFB_ZORDER_E enZOrder)
{
    g_stGfxOps.OPTM_GfxSetLayerPriority(enLayerId, enZOrder);
    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_GetLayerPriority(HIFB_LAYER_ID_E enLayerId, HI_U32 *pU32ZOrder)
{
    g_stGfxOps.OPTM_GfxGetLayerPriority(enLayerId, pU32ZOrder);
    return HI_SUCCESS;
}


HI_S32 HIFB_DRV_PauseCompression(HIFB_LAYER_ID_E enLayerId)
{
     return HI_SUCCESS;
}
HI_S32 HIFB_DRV_ResumeCompression(HIFB_LAYER_ID_E enLayerId)
{
     return HI_SUCCESS;
}

/***************************************************************************************
* func          : HIFB_DRV_GetGFXCap
* description   : CNcomment: 获取图层能力级 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID HIFB_DRV_GetGFXCap(const HIFB_CAPABILITY_S **pstCap)
{
    g_stGfxOps.OPTM_GFX_GetDevCap(pstCap);
}


HI_S32 HIFB_DRV_SetLayerMaskFlag(HIFB_LAYER_ID_E enLayerId, HI_BOOL bFlag)
{
    g_stGfxOps.OPTM_GFX_SetGPMask(OPTM_GetGfxGpId(enLayerId), bFlag);
    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_GetLayerMaskFlag(HIFB_LAYER_ID_E enLayerId)
{
    return g_stGfxOps.OPTM_GFX_GetGfxMask(OPTM_GetGfxGpId(enLayerId));
}

HI_S32 HIFB_DRV_ClearLogo(HIFB_LAYER_ID_E enLayerId)
{
    return g_stGfxOps.OPTM_GFX_ClearLogoOsd(enLayerId);
}

HI_S32 HIFB_DRV_SetStereoDepth(HIFB_LAYER_ID_E enLayerId, HI_S32 s32Depth)
{
    return g_stGfxOps.OPTM_GFX_SetStereoDepth(enLayerId, s32Depth);
}

HI_S32 HIFB_DRV_SetTCFlag(HI_BOOL bFlag)
{
    return g_stGfxOps.OPTM_GFX_SetTCFlag(bFlag);
}

HI_VOID HIFB_DRV_SetDeCmpSwitch(HIFB_LAYER_ID_E enLayerId, HI_BOOL bOpen)
{
#if defined(CFG_HI_FB_DECOMPRESS_SUPPORT) && !defined(HI_BUILD_IN_BOOT)
    if (bOpen)
    {
        g_stGfxOps.OPTM_GFX_DECMP_Open(enLayerId);
    }
    else
    {
        g_stGfxOps.OPTM_GFX_DECMP_Close(enLayerId);
    }
#endif
}

HI_VOID HIFB_DRV_SetDeCmpDdrInfo(HIFB_LAYER_ID_E enLayerId, HI_U32 ARHeadDdr,HI_U32 ARDataDdr,HI_U32 GBHeadDdr,HI_U32 GBDataDdr,HI_U32 DeCmpStride)
{
#if defined(CFG_HI_FB_DECOMPRESS_SUPPORT)
    g_stGfxOps.HIFB_ADP_SetDeCmpDdrInfo(enLayerId,ARHeadDdr,ARDataDdr,GBHeadDdr,GBDataDdr,DeCmpStride);
#else
    UNUSED(enLayerId);
    UNUSED(ARHeadDdr);
    UNUSED(ARDataDdr);
    UNUSED(GBHeadDdr);
    UNUSED(GBDataDdr);
    UNUSED(DeCmpStride);
#endif
     return;
}

HI_VOID HIFB_DRV_GetDeCmpStatus(HIFB_LAYER_ID_E enLayerId, DRV_HIFB_DECMPSTATUS_S *pstDeCmpStatus, HI_BOOL bDebug)
{
#if defined(CFG_HI_FB_DECOMPRESS_SUPPORT)
    g_stGfxOps.HIFB_ADP_GetDeCmpStatus(enLayerId, pstDeCmpStatus, bDebug);
#else
    UNUSED(enLayerId);
    UNUSED(pstDeCmpStatus);
    UNUSED(bDebug);
#endif
    return;
}

HI_VOID HIFB_DRV_SetVdpIntMask(HI_U32 IntMask)
{
#ifdef CFG_HI_FB_DECOMPRESS_SUPPORT
    g_stGfxOps.HIFB_ADP_SetVdpIntMask(IntMask);
#else
    UNUSED(IntMask);
#endif
    return;
}


HI_VOID HIFB_DRV_GetVdpIntMask(HI_U32 *pIntMask)
{
#ifdef CFG_HI_FB_DECOMPRESS_SUPPORT
    g_stGfxOps.HIFB_ADP_GetVdpIntMask(pIntMask);
#else
    UNUSED(pIntMask);
#endif
    return;
}


HI_VOID HIFB_DRV_GetResumeStatus(HIFB_LAYER_ID_E enLayerId, HI_BOOL *pbResume)
{
    g_stGfxOps.HIFB_ADP_GetResumeStatus(enLayerId, pbResume);
    return;
}

HI_S32 HIFB_DRV_SetCmpSwitch(HIFB_LAYER_ID_E enLayerId, HI_BOOL bOpen)
{
    if (bOpen)
    {
        return g_stGfxOps.OPTM_GFX_CMP_Open(enLayerId);
    }
    else
    {
        return g_stGfxOps.OPTM_GFX_CMP_Close(enLayerId);
    }
}

HI_S32 HIFB_DRV_GetCmpSwitch(HIFB_LAYER_ID_E enLayerId)
{
    return g_stGfxOps.OPTM_GFX_CMP_GetSwitch(enLayerId);
}

HI_S32 HIFB_DRV_SetCmpRect(HIFB_LAYER_ID_E enLayerId, HIFB_RECT *pstRect)
{
    return g_stGfxOps.OPTM_GFX_SetCmpRect(enLayerId, pstRect);
}

HI_S32 HIFB_DRV_SetCmpMode(HIFB_LAYER_ID_E enLayerId, HIFB_CMP_MODE_E enCMPMode)
{
    return g_stGfxOps.OPTM_GFX_SetCmpMode(enLayerId, enCMPMode);
}

HIFB_CMP_MODE_E HIFB_DRV_GetCmpMode(HIFB_LAYER_ID_E enLayerId)
{
    return g_stGfxOps.OPTM_GFX_GetCmpMode(enLayerId);
}

HI_S32 HIFB_DRV_SetCmpDDROpen(HIFB_LAYER_ID_E enLayerId, HI_BOOL bOpen)
{
    return g_stGfxOps.OPTM_GFX_SetCmpDDROpen(enLayerId, bOpen);
}
HI_S32 HIFB_DRV_SetGpDeflicker(HI_U32 u32DispChn, HI_BOOL bDeflicker)
{
    OPTM_GFX_GP_E enGpID;
    enGpID = u32DispChn ? OPTM_GFX_GP_0 : OPTM_GFX_GP_1;
    return g_stGfxOps.OPTM_GfxSetGpDeflicker(enGpID, bDeflicker);
}

HI_S32 HIFB_DRV_GetSlvLayerInfo(HIFB_SLVLAYER_DATA_S *pstLayerInfo)
{
    return g_stGfxOps.OPTM_GFX_GetSlvLayerInfo(pstLayerInfo);
}

HI_S32 HIFB_DRV_GetHaltDispStatus(HIFB_LAYER_ID_E enLayerId,HI_BOOL *pbDispInit)
{
    return g_stGfxOps.OPTM_GFX_GetHaltDispStatus(enLayerId, pbDispInit);
}


static HI_BOOL DRV_HIFB_GetIsGfxWorkAtWbc(HI_VOID)
{
    return g_stGfxOps.HIFB_ADP_GetIsGfxWorkAtWbc();
}

HI_VOID DRV_HIFB_GetAdpCallBackFunction(HIFB_DRV_OPS_S *AdpCallBackFunction)
{
    AdpCallBackFunction->HIFB_DRV_CloseLayer        = HIFB_DRV_CloseLayer;
    AdpCallBackFunction->HIFB_DRV_ColorConvert      = HIFB_DRV_ColorConvert;
    AdpCallBackFunction->HIFB_DRV_EnableLayer       = HIFB_DRV_EnableLayer;
    AdpCallBackFunction->HIFB_DRV_GetGFXCap         = HIFB_DRV_GetGFXCap;
    AdpCallBackFunction->HIFB_DRV_GetOSDData        = HIFB_DRV_GetOSDData;
    AdpCallBackFunction->DRV_HIFB_OSI_GetLogoData   = DRV_HIFB_OSI_GetLogoData;
    AdpCallBackFunction->HIFB_DRV_GetLayerPriority  = HIFB_DRV_GetLayerPriority;
    AdpCallBackFunction->HIFB_DRV_GfxDeInit         = HIFB_DRV_GfxDeInit;
    AdpCallBackFunction->HIFB_DRV_GfxInit           = HIFB_DRV_GfxInit;
    AdpCallBackFunction->HIFB_DRV_OpenLayer         = HIFB_DRV_OpenLayer;
    AdpCallBackFunction->HIFB_DRV_PauseCompression  = HIFB_DRV_PauseCompression;
    AdpCallBackFunction->HIFB_DRV_ResumeCompression = HIFB_DRV_ResumeCompression;
    AdpCallBackFunction->HIFB_DRV_SetColorReg       = HIFB_DRV_SetColorReg;
#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    AdpCallBackFunction->HIFB_DRV_SetTriDimMode     = HIFB_DRV_SetTriDimMode;
    AdpCallBackFunction->HIFB_DRV_SetTriDimAddr     = HIFB_DRV_SetTriDimAddr;
#endif
    AdpCallBackFunction->HIFB_DRV_SetLayerAddr      = HIFB_DRV_SetLayerAddr;
    AdpCallBackFunction->HIFB_DRV_GetLayerAddr      = HIFB_DRV_GetLayerAddr;
    AdpCallBackFunction->HIFB_DRV_SetLayerAlpha     = HIFB_DRV_SetLayerAlpha;
    AdpCallBackFunction->HIFB_DRV_GetGPGalphaSum    = HIFB_DRV_GetGPGalphaSum;
    AdpCallBackFunction->HIFB_DRV_SetLayerDataFmt   = HIFB_DRV_SetLayerDataFmt;
    AdpCallBackFunction->HIFB_DRV_SetLayerDeFlicker = HIFB_DRV_SetLayerDeFlicker;
    AdpCallBackFunction->HIFB_DRV_SetLayerPriority  = HIFB_DRV_SetLayerPriority;
    AdpCallBackFunction->HIFB_DRV_UpdataLayerReg    = HIFB_DRV_UpdataLayerReg;
    AdpCallBackFunction->HIFB_DRV_WaitVBlank        = HIFB_DRV_WaitVBlank;
    AdpCallBackFunction->HIFB_DRV_SetLayerKeyMask   = HIFB_DRV_SetLayerKeyMask;
    AdpCallBackFunction->HIFB_DRV_SetLayerPreMult   = HIFB_DRV_SetLayerPreMult;
    AdpCallBackFunction->HIFB_DRV_SetIntCallback    = HIFB_DRV_SetIntCallback;
    AdpCallBackFunction->HIFB_DRV_SetLayerStride    = HIFB_DRV_SetLayerStride;
    AdpCallBackFunction->HIFB_DRV_SetLayerInRect    = HIFB_DRV_SetLayerInRect;
    AdpCallBackFunction->HIFB_DRV_SetLayerOutRect   = HIFB_DRV_SetLayerOutRect;
    AdpCallBackFunction->HIFB_DRV_GetLayerOutRect   = HIFB_DRV_GetLayerOutRect;
    AdpCallBackFunction->HIFB_DRV_GetLayerInRect    = HIFB_DRV_GetLayerInRect;
    AdpCallBackFunction->HIFB_DRV_SetLayerScreenSize = HIFB_DRV_SetLayerScreenSize;
    AdpCallBackFunction->HIFB_DRV_SetLayerMaskFlag  = HIFB_DRV_SetLayerMaskFlag;
    AdpCallBackFunction->HIFB_DRV_GetLayerMaskFlag  = HIFB_DRV_GetLayerMaskFlag;
    AdpCallBackFunction->HIFB_DRV_GetDispSize       = HIFB_DRV_GetDispSize;
    AdpCallBackFunction->HIFB_DRV_ClearLogo         = HIFB_DRV_ClearLogo;
    AdpCallBackFunction->HIFB_DRV_SetStereoDepth    = HIFB_DRV_SetStereoDepth;
    AdpCallBackFunction->HIFB_DRV_SetTCFlag         = HIFB_DRV_SetTCFlag;
    AdpCallBackFunction->HIFB_DRV_SetDeCmpSwitch    = HIFB_DRV_SetDeCmpSwitch;
    AdpCallBackFunction->HIFB_DRV_SetDeCmpDdrInfo   = HIFB_DRV_SetDeCmpDdrInfo;
    AdpCallBackFunction->HIFB_DRV_GetDeCmpStatus    = HIFB_DRV_GetDeCmpStatus;
    AdpCallBackFunction->HIFB_DRV_SetVdpIntMask     = HIFB_DRV_SetVdpIntMask;
    AdpCallBackFunction->HIFB_DRV_GetVdpIntMask     = HIFB_DRV_GetVdpIntMask;
    AdpCallBackFunction->HIFB_DRV_GetResumeStatus   = HIFB_DRV_GetResumeStatus;
    AdpCallBackFunction->HIFB_DRV_SetCmpSwitch      = HIFB_DRV_SetCmpSwitch;
    AdpCallBackFunction->HIFB_DRV_GetCmpSwitch      = HIFB_DRV_GetCmpSwitch;
    AdpCallBackFunction->HIFB_DRV_SetCmpRect        = HIFB_DRV_SetCmpRect;
    AdpCallBackFunction->HIFB_DRV_SetCmpMode        = HIFB_DRV_SetCmpMode;
    AdpCallBackFunction->HIFB_DRV_GetCmpMode        = HIFB_DRV_GetCmpMode;
    AdpCallBackFunction->HIFB_DRV_SetCmpDDROpen     = HIFB_DRV_SetCmpDDROpen;
    AdpCallBackFunction->HIFB_DRV_SetGpDeflicker    = HIFB_DRV_SetGpDeflicker;
    AdpCallBackFunction->HIFB_DRV_GetSlvLayerInfo   = HIFB_DRV_GetSlvLayerInfo;
    AdpCallBackFunction->HIFB_DRV_GetHaltDispStatus = HIFB_DRV_GetHaltDispStatus;
    AdpCallBackFunction->DRV_HIFB_GetIsGfxWorkAtWbc = DRV_HIFB_GetIsGfxWorkAtWbc;
    return;
}
