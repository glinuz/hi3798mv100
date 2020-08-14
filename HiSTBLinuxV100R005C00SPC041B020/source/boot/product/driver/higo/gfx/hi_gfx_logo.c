/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_gfx_logo.c
Version             : Initial Draft
Author              :
Created             : 2016/05/07
Description         :
                       CNcomment: 开机logo图层操作实现源文件 CNend\n
Function List   :
History         :
Date                        Author                  Modification
2016/05/07                  y00181162               Created file
******************************************************************************/


/************************** add include here**************************************/
#include "hi_go_logo.h"
#include "hi_go_gfxlayer.h"
#include "drv_disp_ext.h"

/************************** Macro Definition     *********************************/


/************************** Structure Definition *********************************/

/************************** Global Variable declaration **************************/


/************************** Api declaration **************************************/
static HI_S32 HI_GO_DecImg              (HI_GO_LOGO_INFO  *pLogoInfo,   HI_HANDLE *LogoDecSurface);
static HI_VOID HI_GO_IsSupport4KLOGO    (HI_GO_LOGO_INFO  *pLogoInfo,   HI_HANDLE LogoDecSurface);
static HI_S32 HI_GO_DisplayHDLogo       (HI_GO_LOGO_INFO  *pHdLogoInfo, HI_HANDLE LogoDecSurface);
static HI_S32 HI_GO_DisplaySDLogo       (HI_GO_LOGO_INFO  *pSdLogoInfo, HI_HANDLE LogoDecSurface);
static HI_S32 HI_GO_DisplayGfxLogo      (HI_GO_LOGO_INFO  *pLogoInfo,   HI_HANDLE LogoDecSurface);

/************************** API realization **************************************/


/***************************************************************************************
* func          : HI_GO_DisplayLogo
* description   : CNcomment: 显示开机Logo CNend\n
* param[in]     : LogoInfo : Logo信息
* retval        : HI_SUCCESS
* retval        : HI_FAILURE
* others:       : NA
***************************************************************************************/
HI_S32  HI_GO_DisplayLogo(HI_GO_LOGO_INFO *pLogoInfo)
{
     HI_S32 Ret = HI_SUCCESS;
     HI_HANDLE LocalLogoDecSurface = HI_INVALID_HANDLE;

     Ret = HI_GO_DecImg(pLogoInfo,&LocalLogoDecSurface);
     if (HI_SUCCESS != Ret)
     {
         HI_GFX_Log();
         return HI_FAILURE;
     }

     HI_GO_IsSupport4KLOGO(pLogoInfo,LocalLogoDecSurface);

     Ret = HI_GO_DisplayGfxLogo(pLogoInfo,LocalLogoDecSurface);
     if (HI_SUCCESS != Ret)
     {
         HI_GFX_Log();
         goto ERR_EXIT_ONE;
     }

     if (HI_INVALID_HANDLE != LocalLogoDecSurface)
     {
        HI_GO_FreeDecSurface(LocalLogoDecSurface);
     }

     return HI_SUCCESS;

ERR_EXIT_ONE:

     if (HI_INVALID_HANDLE != LocalLogoDecSurface)
     {
        HI_GO_FreeDecSurface(LocalLogoDecSurface);
     }

     return HI_FAILURE;
}


static HI_S32 HI_GO_DecImg(HI_GO_LOGO_INFO  *pLogoInfo, HI_HANDLE *LogoDecSurface)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_CHAR* pSrcBuf     = (HI_CHAR*)(pLogoInfo->LogoDataBuf);
    HI_HANDLE hDecHandle = HI_INVALID_HANDLE;

    Ret = HI_GO_CreateDecode(&hDecHandle,pSrcBuf,pLogoInfo->LogoDataLen);
    if (HI_SUCCESS != Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    Ret = HI_GO_StartDecode(hDecHandle, LogoDecSurface);
    if (HI_SUCCESS != Ret)
    {
        HI_GFX_Log();
        (HI_VOID)HI_GO_DestroyDecode(hDecHandle);
        return HI_FAILURE;
    }

    Ret |= HI_GO_DestroyDecode(hDecHandle);
    if (HI_SUCCESS != Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_VOID HI_GO_IsSupport4KLOGO(HI_GO_LOGO_INFO  *pLogoInfo, HI_HANDLE LogoDecSurface)
{
     HI_DISP_DISPLAY_INFO_S stInfo = {0};
     HI_GO_SURFACE_S *pstSrcSurface = (HI_GO_SURFACE_S *)LogoDecSurface;

     HI_DISP_GetDisplayInfo(HI_DRV_DISPLAY_1, &stInfo);

     if (   (CONFIG_GFX_DISP_REALLY4K_VIRTUAL_WIDTH == pstSrcSurface->u32Width[0] || CONFIG_GFX_DISP_4K_VIRTUAL_WIDTH == pstSrcSurface->u32Width[0])
         && (CONFIG_GFX_DISP_REALLY4K_VIRTUAL_WIDTH == stInfo.stFmtResolution.s32Width || CONFIG_GFX_DISP_4K_VIRTUAL_WIDTH == stInfo.stFmtResolution.s32Width)
         && (CONFIG_GFX_DISP_4K_VIRTUAL_HEIGHT == pstSrcSurface->u32Height[0])
         && (CONFIG_GFX_DISP_4K_VIRTUAL_HEIGHT == stInfo.stFmtResolution.s32Height))
     {
          pLogoInfo->Support4KLogo = HI_TRUE;
     }
     else
     {
          pLogoInfo->Support4KLogo = HI_FALSE;
     }
#if 0
     HI_GFX_Print("\n===========================================================\n");
     HI_GFX_Print("========%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
     HI_GFX_Print("the 4k logo support %d\n",pLogoInfo->Support4KLogo);
     HI_GFX_Print("img size [%d %d], fmt size [%d %d]\n",pstSrcSurface->u32Width[0],pstSrcSurface->u32Height[0],stInfo.stFmtResolution.s32Width,stInfo.stFmtResolution.s32Height);
     HI_GFX_Print("===========================================================\n");
#endif
     return;
}

static HI_S32 HI_GO_DisplayGfxLogo(HI_GO_LOGO_INFO  *pLogoInfo, HI_HANDLE LogoDecSurface)
{
    HI_S32 Ret = HI_SUCCESS;

    Ret = HI_GO_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    Ret = HI_GO_DisplayHDLogo(pLogoInfo,LogoDecSurface);
    if (HI_SUCCESS != Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    if (HI_TRUE != pLogoInfo->DispToSD)
    {
        return HI_SUCCESS;
    }

    Ret = HI_GO_DisplaySDLogo(pLogoInfo,LogoDecSurface);
    if (HI_SUCCESS != Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


static HI_S32 HI_GO_DisplayHDLogo(HI_GO_LOGO_INFO *pHdLogoInfo,HI_HANDLE LogoDecSurface)
{
     HI_S32 Ret = HI_SUCCESS;
     HIGO_BLTOPT_S  stBltOpt;
     HI_HANDLE LayerHDSurface = HI_INVALID_HANDLE;
     HI_HANDLE LayerSDSurface = HI_INVALID_HANDLE;
     HI_GO_RECT_S stHDRect = {0};
     HI_GO_RECT_S stSDRect = {0};
     HI_GO_SURFACE_S *pstHDSurface = NULL;
     HI_GO_SURFACE_S *pstSDSurface = NULL;

     Ret = HI_GO_CreateLayerSurface(HI_GO_LAYER_HD_0,pHdLogoInfo->Support4KLogo,pHdLogoInfo->DispColorFmt[CONFIG_GFX_DISP_LOGO_HD],&LayerHDSurface);
     if (HI_SUCCESS != Ret)
     {
         HI_GFX_Log();
         return HI_FAILURE;
     }

     if (HI_TRUE == pHdLogoInfo->Support4KLogo)
     {
         Ret = HI_GO_CreateLayerSurface(HI_GO_LAYER_SD_0,pHdLogoInfo->Support4KLogo,HIGO_PF_AYCbCr8888,&LayerSDSurface);
     }
     if (HI_SUCCESS != Ret)
     {
         HI_GFX_Log();
         goto ERR_EXIT_ONE;
     }

     Ret = HI_GO_FillRect(LayerHDSurface, NULL, 0x0, HIGO_COMPOPT_NONE);
     if (HI_SUCCESS != Ret)
     {
         HI_GFX_Log();
         goto ERR_EXIT_ONE;
     }

     if (HI_TRUE == pHdLogoInfo->Support4KLogo)
     {
        Ret = HI_GO_FillRect(LayerSDSurface, NULL, 0x0, HIGO_COMPOPT_NONE);
     }
     if (HI_SUCCESS != Ret)
     {
         HI_GFX_Log();
         goto ERR_EXIT_ONE;
     }

     memset(&stBltOpt, 0, sizeof(HIGO_BLTOPT_S));
     stBltOpt.EnableScale = HI_TRUE;

     pstHDSurface = (HI_GO_SURFACE_S*)LayerHDSurface;
     stHDRect.x = pstHDSurface->stRect.x;
     stHDRect.y = pstHDSurface->stRect.y;
     stHDRect.w = pstHDSurface->stRect.w;
     stHDRect.h = pstHDSurface->stRect.h;
     Ret = HI_GO_Blit(LogoDecSurface, NULL, LayerHDSurface, &stHDRect, &stBltOpt);
     if (HI_SUCCESS != Ret)
     {
         HI_GFX_Log();
         goto ERR_EXIT_ONE;
     }

     if (HI_TRUE == pHdLogoInfo->Support4KLogo)
     {
        pstSDSurface = (HI_GO_SURFACE_S*)LayerSDSurface;
        stSDRect.x = pstSDSurface->stRect.x;
        stSDRect.y = pstSDSurface->stRect.y;
        stSDRect.w = pstSDSurface->stRect.w;
        stSDRect.h = pstSDSurface->stRect.h;
        Ret = HI_GO_Blit(LogoDecSurface, NULL, LayerSDSurface, &stSDRect, &stBltOpt);
     }
     if (HI_SUCCESS != Ret)
     {
         HI_GFX_Log();
         goto ERR_EXIT_ONE;
     }

     Ret = HI_GO_RefreshLayer(LayerHDSurface,pHdLogoInfo->Support4KLogo);
     if (HI_SUCCESS != Ret)
     {
         HI_GFX_Log();
         goto ERR_EXIT_ONE;
     }

     return HI_SUCCESS;

ERR_EXIT_ONE:
     if (HI_INVALID_HANDLE != LayerHDSurface)
     {
        HI_GO_DestroyLayer(LayerHDSurface);
     }

     if (HI_INVALID_HANDLE != LayerSDSurface)
     {
        HI_GO_DestroyLayer(LayerSDSurface);
     }

     HI_GFX_Log();

     return HI_FAILURE;
}



static HI_S32 HI_GO_DisplaySDLogo(HI_GO_LOGO_INFO  *pSdLogoInfo,HI_HANDLE LogoDecSurface)
{
     HI_S32 Ret = HI_SUCCESS;
     HIGO_BLTOPT_S  stBltOpt;
     HI_HANDLE  LayerSDSurface = HI_INVALID_HANDLE;
     HI_GO_LAYER_ID_E  DispLogoToSD = HI_GO_LAYER_SD_0;

     Ret = HI_GO_CreateLayerSurface(DispLogoToSD,pSdLogoInfo->Support4KLogo,pSdLogoInfo->DispColorFmt[CONFIG_GFX_DISP_LOGO_SD],&LayerSDSurface);
     if (HI_SUCCESS != Ret)
     {
         HI_GFX_Log();
         return HI_FAILURE;
     }

     Ret = HI_GO_FillRect(LayerSDSurface, NULL, 0xffaabbcc, HIGO_COMPOPT_NONE);
     if (HI_SUCCESS != Ret)
     {
         goto ERR_EXIT_ONE;
     }

     memset(&stBltOpt, 0, sizeof(HIGO_BLTOPT_S));
     stBltOpt.EnableScale = HI_TRUE;
     Ret = HI_GO_Blit(LogoDecSurface, NULL, LayerSDSurface, NULL, &stBltOpt);
     if (HI_SUCCESS != Ret)
     {
         HI_GFX_Log();
         goto ERR_EXIT_ONE;
     }

     Ret = HI_GO_RefreshLayer(LayerSDSurface,pSdLogoInfo->Support4KLogo);
     if (HI_SUCCESS != Ret)
     {
         HI_GFX_Log();
         goto ERR_EXIT_ONE;
     }

     return HI_SUCCESS;

ERR_EXIT_ONE:

     if (HI_INVALID_HANDLE != LayerSDSurface)
     {
        HI_GO_DestroyLayer(LayerSDSurface);
     }

     return HI_FAILURE;
}
