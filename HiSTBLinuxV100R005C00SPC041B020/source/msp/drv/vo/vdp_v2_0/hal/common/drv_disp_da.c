
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_da.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_disp_com.h"
#include "drv_disp_da.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

//static DISP_ALG_VERSION_S s_stAlgCscVersion = {0};
HI_BOOL g_bDAInitFlag = HI_FALSE;
DISP_DA_FUNCTION_S g_stDAFuntion;
#ifndef __DISP_PLATFORM_BOOT__
ALG_GMM_MEM_S g_stAlgGammaMMZ;

HI_S32 DISP_GAMMA_Init(void)
{
    HI_S32 s32Ret;
    ALG_BUFFER_S* pstMBufRGB, *pstMBufYUV;


    //apply memory for gmm coefficient, and get the address.
    pstMBufRGB = &(g_stAlgGammaMMZ.stMBufRGB);
    s32Ret = HI_DRV_MMZ_AllocAndMap("DISP_Gamma_RGB", HI_NULL, (39 * 32) * 4, 0, (MMZ_BUFFER_S*)pstMBufRGB); //22*3*16  four table
    if (s32Ret != HI_SUCCESS)
    {
        DISP_FATAL("Get gmm_buf_RGB failed.\n");
        return HI_FAILURE;
    }

    pstMBufYUV = &(g_stAlgGammaMMZ.stMBufYUV);
    s32Ret = HI_DRV_MMZ_AllocAndMap("DISP_Gamma_YUV", HI_NULL, (39 * 32) * 4, 0, (MMZ_BUFFER_S*)pstMBufYUV);
    if (s32Ret != HI_SUCCESS)
    {
        DISP_FATAL("Get gmm_buf_YUV failed.\n");
        HI_DRV_MMZ_UnmapAndRelease((MMZ_BUFFER_S*)(&g_stAlgGammaMMZ.stMBufRGB));
        return HI_FAILURE;
    }

    s32Ret = ALG_GmmInit(&g_stAlgGammaMMZ);

    if (s32Ret != HI_SUCCESS)
    {
        HI_DRV_MMZ_UnmapAndRelease((MMZ_BUFFER_S*)(&g_stAlgGammaMMZ.stMBufRGB));
        HI_DRV_MMZ_UnmapAndRelease((MMZ_BUFFER_S*)(&g_stAlgGammaMMZ.stMBufYUV));
        DISP_FATAL("Init Gamma failed.\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 DISP_GAMMA_DeInit(void)
{
    HI_DRV_MMZ_UnmapAndRelease((MMZ_BUFFER_S*)(&g_stAlgGammaMMZ.stMBufRGB));
    HI_DRV_MMZ_UnmapAndRelease((MMZ_BUFFER_S*)(&g_stAlgGammaMMZ.stMBufYUV));

    return HI_SUCCESS;
}
HI_S32 UpdateGamma(const HI_S16 *ps16GmmCoef,  ALG_GMM_CS_MODE_E enCSMode)
{
    HI_S32 s32Ret;

    s32Ret = UpdateGmmCoef(&g_stAlgGammaMMZ,ps16GmmCoef,enCSMode);

    return s32Ret;
}


HI_S32 SetGammaCtrl(ALG_GMM_DRV_PARA_S *pstGmmDrvPara, ALG_GMM_RTL_PARA_S* pstGmmRtlPara)
{
    HI_S32 s32Ret;
    s32Ret =  ALG_GmmThdSet(&g_stAlgGammaMMZ,pstGmmDrvPara, pstGmmRtlPara);

    return s32Ret;
}
#endif
HI_S32 DISP_DA_Init(HI_DRV_DISP_VERSION_S* pstVersion)
{
#ifndef __DISP_PLATFORM_BOOT__
    HI_S32 nRet;
#endif
    if (!pstVersion)
    {
        DISP_ERROR("FUNC(%s) Error! Invalid input parameters!\n", __FUNCTION__);
        return HI_FAILURE;
    }

    if (g_bDAInitFlag)
    {
        DISP_INFO("FUNC(%s) inited!\n", __FUNCTION__);
        return HI_SUCCESS;
    }

    DISP_MEMSET(&g_stDAFuntion, 0, sizeof(g_stDAFuntion));
#ifndef __DISP_PLATFORM_BOOT__
    DISP_MEMSET(&g_stAlgGammaMMZ, 0, sizeof(g_stAlgGammaMMZ));
#endif
    if (   (pstVersion->u32VersionPartH == DISP_CV200_ES_VERSION_H)
        && (pstVersion->u32VersionPartL == DISP_CV200_ES_VERSION_L)
        )
    {
        g_stDAFuntion.PFCscRgb2Yuv   = DISP_ALG_CscRgb2Yuv;
#ifndef __DISP_PLATFORM_BOOT__
        g_stDAFuntion.PFUpdateGammaCoef = UpdateGamma;
        g_stDAFuntion.PFSetGammaCtrl = SetGammaCtrl;
#endif
    }
    else
    {
        DISP_ERROR("FUNC(%s) Error! Invalid display version!", __FUNCTION__);
        return HI_FAILURE;
    }
#ifndef __DISP_PLATFORM_BOOT__
    nRet = DISP_GAMMA_Init();

    if (nRet)
    {
        return HI_FAILURE;
    }
#endif
    g_bDAInitFlag = HI_TRUE;

    return HI_SUCCESS;
}


HI_S32 DISP_DA_DeInit(HI_VOID)
{
    if (g_bDAInitFlag)
    {
        DISP_MEMSET(&g_stDAFuntion, 0, sizeof(g_stDAFuntion));

        g_bDAInitFlag = HI_FALSE;
#ifndef __DISP_PLATFORM_BOOT__
        (void)DISP_GAMMA_DeInit();
#endif
    }


    return HI_SUCCESS;
}

DISP_DA_FUNCTION_S * DISP_DA_GetFunction(HI_VOID)
{
    if (g_bDAInitFlag)
    {
        return &g_stDAFuntion;
    }
    else
    {
        DISP_FATAL("UA is not inited! DISP_UA_GetFunction return NULL!\n");
        return HI_NULL;
    }
}










#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */









