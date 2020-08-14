/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_disp_tran.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/15
  Description   :
  History       :
  1.Date        : 2013/1/31
    Author      :
    Modification: Created file

*******************************************************************************/

#include "memory.h"
#include "mpi_disp_tran.h"
#include "hi_error_mpi.h"



#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

HI_S32 Transfer_DispType(HI_UNF_DISP_HDR_TYPE_E* pU, HI_DRV_DISP_OUT_TYPE_E* pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        switch (*pU)
        {
            case HI_UNF_DISP_HDR_TYPE_NONE:
            {
                *pM = HI_DRV_DISP_TYPE_NORMAL;
                break;
            }
    
            case HI_UNF_DISP_HDR_TYPE_DOLBY:
            {
                *pM = HI_DRV_DISP_TYPE_DOLBY;
                break;
            }
    
            case HI_UNF_DISP_HDR_TYPE_HDR10:
            {
                *pM = HI_DRV_DISP_TYPE_HDR10;
                break;
            }
            default:
                *pM = HI_DRV_DISP_TYPE_NORMAL;
        }
    }
    else
    {
        switch (*pM)
        {
            case HI_DRV_DISP_TYPE_NORMAL:
            {
                *pU = HI_UNF_DISP_HDR_TYPE_NONE;
                break;
            }
    
            case HI_DRV_DISP_TYPE_DOLBY:
            {
                *pU = HI_UNF_DISP_HDR_TYPE_DOLBY;
                break;
            }
    
            case HI_DRV_DISP_TYPE_HDR10:
            {
                *pU = HI_UNF_DISP_HDR_TYPE_HDR10;
                break;
            }
    
            default:
                *pU = HI_UNF_DISP_HDR_TYPE_NONE;
        }
    }
    return HI_SUCCESS;
}

HI_S32 Transfer_DispID(HI_UNF_DISP_E *pU, HI_DRV_DISPLAY_E *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        *pM = (HI_DRV_DISPLAY_E)(*pU);
        return HI_SUCCESS;
    }
    else
    {
        *pU = (HI_UNF_DISP_E)(*pM);
        return HI_SUCCESS;
    }
}

HI_S32 Transfer_VbiCfg(HI_UNF_DISP_VBI_CFG_S *pU, HI_DRV_DISP_VBI_CFG_S *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        pM->eType = (HI_DRV_DISP_VBI_TYPE_E)pU->enType;
        pM->u32InBufferSize = pU->u32InBufferSize;
        pM->u32WorkBufferSize = pU->u32WorkBufferSize;

        return HI_SUCCESS;
    }
    else
    {
        pU->enType = (HI_UNF_DISP_VBI_TYPE_E)pM->eType;
        pU->u32InBufferSize = pM->u32InBufferSize;
        pU->u32WorkBufferSize = pM->u32WorkBufferSize;
        return HI_SUCCESS;
    }
}

HI_S32 Transfer_VbiData(HI_UNF_DISP_VBI_DATA_S *pU, HI_DRV_DISP_VBI_DATA_S *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        pM->eType = (HI_DRV_DISP_VBI_TYPE_E)pU->enType;
        pM->pu8DataAddr = pU->pu8DataAddr;
        pM->u32DataLen = pU->u32DataLen;

        return HI_SUCCESS;
    }
    else
    {
        pU->enType = (HI_UNF_DISP_VBI_TYPE_E)pM->eType;
        pU->pu8DataAddr = pM->pu8DataAddr;
        pU->u32DataLen = pM->u32DataLen;
        return HI_SUCCESS;
    }
}



HI_S32 Transfer_DispOffset(HI_UNF_DISP_OFFSET_S *pU, HI_DRV_DISP_OFFSET_S *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        *pM = *((HI_DRV_DISP_OFFSET_S* )pU);
        return HI_SUCCESS;
    }
    else
    {
        *pU = *((HI_UNF_DISP_OFFSET_S *)pM);
        return HI_SUCCESS;
    }
}



HI_S32 Transfer_Disp3DMode(HI_UNF_DISP_3D_E *pU, HI_DRV_DISP_STEREO_MODE_E *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        *pM = (HI_DRV_DISP_STEREO_MODE_E)(*pU);
        return HI_SUCCESS;
    }
    else
    {
        *pU = (HI_UNF_DISP_3D_E)(*pM);
        return HI_SUCCESS;
    }
}

HI_S32 Transfer_LayerID(HI_UNF_DISP_LAYER_E *pU, HI_DRV_DISP_LAYER_E *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        if (*pU < HI_UNF_DISP_LAYER_BUTT)
        {
            if  (*pU == HI_UNF_DISP_LAYER_VIDEO)
                *pM  = HI_DRV_DISP_LAYER_VIDEO;
            else
                *pM  = HI_DRV_DISP_LAYER_GFX;

            return HI_SUCCESS;
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else
    {
        if(*pM < HI_DRV_DISP_LAYER_BUTT)
        {
             if  (*pM == HI_DRV_DISP_LAYER_VIDEO)
                *pU  = HI_UNF_DISP_LAYER_VIDEO;
            else
                *pU  = HI_UNF_DISP_LAYER_GFX;
            return HI_SUCCESS;
        }
        else
        {
            return HI_FAILURE;
        }
    }

}

HI_DRV_DISP_FMT_E Transfer_TVHDFmtU2V(HI_UNF_ENC_FMT_E U)
{
    if (U <= HI_UNF_ENC_FMT_480P_60)
    {
        HI_U32 t;

        t = (HI_U32)U;
        t = t - (HI_U32)HI_UNF_ENC_FMT_1080P_60;
        t = t + (HI_U32)HI_DRV_DISP_FMT_1080P_60;
        return (HI_DRV_DISP_FMT_E)t;
    }
    else
    {
        return HI_DRV_DISP_FMT_1080i_50;
    }
}

HI_UNF_ENC_FMT_E Transfer_TVHDFmtV2U(HI_DRV_DISP_FMT_E V)
{
    if (V <= HI_DRV_DISP_FMT_480P_60)
    {
        HI_U32 t;

        t = (HI_U32)V;
        t = t - (HI_U32)HI_DRV_DISP_FMT_1080P_60;
        t = t + (HI_U32)HI_UNF_ENC_FMT_1080P_60;
        return (HI_UNF_ENC_FMT_E)t;
    }
    else
    {
        return HI_UNF_ENC_FMT_1080i_50;
    }
}

HI_DRV_DISP_FMT_E Transfer_TVSDFmtU2V(HI_UNF_ENC_FMT_E U)
{
    switch (U)
    {
        case HI_UNF_ENC_FMT_PAL:
            return HI_DRV_DISP_FMT_PAL;
        case HI_UNF_ENC_FMT_PAL_N:
            return HI_DRV_DISP_FMT_PAL_N;
        case HI_UNF_ENC_FMT_PAL_Nc:
            return HI_DRV_DISP_FMT_PAL_Nc;
        case HI_UNF_ENC_FMT_NTSC:
            return HI_DRV_DISP_FMT_NTSC;
        case HI_UNF_ENC_FMT_NTSC_J:
            return HI_DRV_DISP_FMT_NTSC_J;
        case HI_UNF_ENC_FMT_NTSC_PAL_M:
            return HI_DRV_DISP_FMT_PAL_M;
        case HI_UNF_ENC_FMT_SECAM_SIN:
            return HI_DRV_DISP_FMT_SECAM_SIN;
        case HI_UNF_ENC_FMT_SECAM_COS:
            return HI_DRV_DISP_FMT_SECAM_COS;
        default:
            return HI_DRV_DISP_FMT_PAL;
    }
}

HI_UNF_ENC_FMT_E Transfer_TVSDFmtV2U(HI_DRV_DISP_FMT_E V)
{
    switch (V)
    {
        case HI_DRV_DISP_FMT_PAL:
        case HI_DRV_DISP_FMT_PAL_B:
        case HI_DRV_DISP_FMT_PAL_B1:
        case HI_DRV_DISP_FMT_PAL_D:
        case HI_DRV_DISP_FMT_PAL_D1:
        case HI_DRV_DISP_FMT_PAL_G:
        case HI_DRV_DISP_FMT_PAL_H:
        case HI_DRV_DISP_FMT_PAL_K:
        case HI_DRV_DISP_FMT_PAL_I:
        case HI_DRV_DISP_FMT_1440x576i_50:
            return HI_UNF_ENC_FMT_PAL;
        case HI_DRV_DISP_FMT_PAL_N:
            return HI_UNF_ENC_FMT_PAL_N;
        case HI_DRV_DISP_FMT_PAL_Nc:
            return HI_UNF_ENC_FMT_PAL_Nc;

        case HI_DRV_DISP_FMT_NTSC:
        case HI_DRV_DISP_FMT_PAL_60:
        case HI_DRV_DISP_FMT_NTSC_443:
        case HI_DRV_DISP_FMT_1440x480i_60:
            return HI_UNF_ENC_FMT_NTSC;
        case HI_DRV_DISP_FMT_NTSC_J:
            return HI_UNF_ENC_FMT_NTSC_J;
        case HI_DRV_DISP_FMT_PAL_M:
            return HI_UNF_ENC_FMT_NTSC_PAL_M;

        case HI_DRV_DISP_FMT_SECAM_SIN:
        case HI_DRV_DISP_FMT_SECAM_L:
        case HI_DRV_DISP_FMT_SECAM_B:
        case HI_DRV_DISP_FMT_SECAM_G:
        case HI_DRV_DISP_FMT_SECAM_D:
        case HI_DRV_DISP_FMT_SECAM_K:
        case HI_DRV_DISP_FMT_SECAM_H:
            return HI_UNF_ENC_FMT_SECAM_SIN;
        case HI_DRV_DISP_FMT_SECAM_COS:
            return HI_UNF_ENC_FMT_SECAM_COS;
        default:
            return HI_UNF_ENC_FMT_PAL;
    }
}

HI_DRV_DISP_FMT_E Transfer_3DFmtU2V(HI_UNF_ENC_FMT_E U)
{
    if (U <= HI_UNF_ENC_FMT_720P_50_FRAME_PACKING)
    {
        HI_U32 t;

        t = (HI_U32)U;
        t = t - (HI_U32)HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING;
        t = t + HI_DRV_DISP_FMT_1080P_24_FP;
        return (HI_DRV_DISP_FMT_E)t;
    }
    else
    {
        return HI_DRV_DISP_FMT_1080P_24_FP;
    }
}

HI_UNF_ENC_FMT_E Transfer_3DFmtV2U(HI_DRV_DISP_FMT_E V)
{
    if (V <= HI_DRV_DISP_FMT_720P_50_FP)
    {
        HI_U32 t;

        t = (HI_U32)V;
        t = t - (HI_U32)HI_DRV_DISP_FMT_1080P_24_FP;
        t = t + (HI_U32)HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING;
        return (HI_UNF_ENC_FMT_E)t;
    }
    else
    {
        return HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING;
    }
}

HI_UNF_ENC_FMT_E Transfer_4KFmtV2U(HI_DRV_DISP_FMT_E V)
{
    if (V <= HI_DRV_DISP_FMT_4096X2160_60)
    {
        HI_U32 t;

        t = (HI_U32)V;
        t = t - (HI_U32)HI_DRV_DISP_FMT_3840X2160_24;
        t = t + (HI_U32)HI_UNF_ENC_FMT_3840X2160_24;
        return (HI_UNF_ENC_FMT_E)t;
    }
    else
    {
        return HI_UNF_ENC_FMT_3840X2160_24;
    }
}

HI_DRV_DISP_FMT_E Transfer_4KFmtU2V(HI_UNF_ENC_FMT_E U)
{
    if (U <= HI_UNF_ENC_FMT_4096X2160_60)
    {
        HI_U32 t;

        t = (HI_U32)U;
        t = t - (HI_U32)HI_UNF_ENC_FMT_3840X2160_24;
        t = t + (HI_U32)HI_DRV_DISP_FMT_3840X2160_24;
        return (HI_DRV_DISP_FMT_E)t;
    }
    else
    {
        return HI_DRV_DISP_FMT_3840X2160_24;
    }
}


HI_DRV_DISP_FMT_E Transfer_DVIFmtU2V(HI_UNF_ENC_FMT_E U)
{
    if (U <= HI_UNF_ENC_FMT_VESA_2560X1600_60_RB)
    {
        HI_U32 t;

        t = (HI_U32)U;
        t = t - (HI_U32)HI_UNF_ENC_FMT_861D_640X480_60;
        t = t + (HI_U32)HI_DRV_DISP_FMT_861D_640X480_60;
        return (HI_DRV_DISP_FMT_E)t;
    }
    else
    {
        return HI_DRV_DISP_FMT_861D_640X480_60;
    }
}

HI_UNF_ENC_FMT_E Transfer_DVIFmtV2U(HI_DRV_DISP_FMT_E V)
{
    if (V <= HI_DRV_DISP_FMT_VESA_2560X1600_60_RB)
    {
        HI_U32 t;

        t = (HI_U32)V;
        t = t - (HI_U32)HI_DRV_DISP_FMT_861D_640X480_60;
        t = t + (HI_U32)HI_UNF_ENC_FMT_861D_640X480_60;

        return (HI_UNF_ENC_FMT_E)t;
    }
    else
    {
        return HI_UNF_ENC_FMT_861D_640X480_60;
    }
}


HI_DRV_DISP_FMT_E Transfer_DecimalFmtU2V(HI_UNF_ENC_FMT_E U)
{
    if (U <= HI_UNF_ENC_FMT_1080i_59_94)
    {
        HI_U32 t;

        t = (HI_U32)U;
        t = t - (HI_U32)HI_UNF_ENC_FMT_3840X2160_23_976;
        t = t + (HI_U32)HI_DRV_DISP_FMT_3840X2160_23_976;
        return (HI_DRV_DISP_FMT_E)t;
    }
    else
    {
        return HI_DRV_DISP_FMT_3840X2160_23_976;
    }
}

HI_UNF_ENC_FMT_E Transfer_DecimalFmtV2U(HI_DRV_DISP_FMT_E V)
{
    if (V <= HI_DRV_DISP_FMT_1080i_59_94)
    {
        HI_U32 t;

        t = (HI_U32)V;
        t = t - (HI_U32)HI_DRV_DISP_FMT_3840X2160_23_976;
        t = t + (HI_U32)HI_UNF_ENC_FMT_3840X2160_23_976;

        return (HI_UNF_ENC_FMT_E)t;
    }
    else
    {
        return HI_UNF_ENC_FMT_3840X2160_23_976;
    }
}

HI_S32 Transfer_EncFmt(HI_UNF_ENC_FMT_E *pU, HI_DRV_DISP_FMT_E *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        if (*pU <= HI_UNF_ENC_FMT_480P_60)
        {
            *pM = Transfer_TVHDFmtU2V(*pU);
            return HI_SUCCESS;
        }
        else if (*pU <= HI_UNF_ENC_FMT_SECAM_COS)
        {
            *pM = Transfer_TVSDFmtU2V(*pU);
            return HI_SUCCESS;
        }
        else if (*pU <= HI_UNF_ENC_FMT_720P_50_FRAME_PACKING)
        {
            *pM = Transfer_3DFmtU2V(*pU);
            return HI_SUCCESS;
        }
        else if (*pU <= HI_UNF_ENC_FMT_VESA_2560X1600_60_RB)
        {
            *pM = Transfer_DVIFmtU2V(*pU);
            return HI_SUCCESS;
        }
        else if (*pU <= HI_UNF_ENC_FMT_4096X2160_24)
        {
            *pM = Transfer_4KFmtU2V(*pU);
            return HI_SUCCESS;
        }
        else if (*pU <= HI_UNF_ENC_FMT_1080i_59_94)
        {
            *pM = Transfer_DecimalFmtU2V(*pU);
            return HI_SUCCESS;
        }

        else if (*pU == HI_UNF_ENC_FMT_BUTT)
        {
            return HI_ERR_DISP_INVALID_PARA;
        }
        else
        {
            return HI_FAILURE;
        }

    }
    else
    {
        if (*pM <= HI_DRV_DISP_FMT_480P_60)
        {
            *pU = Transfer_TVHDFmtV2U(*pM);
            return HI_SUCCESS;
        }
        else if (*pM <= HI_DRV_DISP_FMT_1440x480i_60)
        {
            *pU = Transfer_TVSDFmtV2U(*pM);
            return HI_SUCCESS;
        }
        else if (*pM <= HI_DRV_DISP_FMT_720P_50_FP)
        {
            *pU = Transfer_3DFmtV2U(*pM);
            return HI_SUCCESS;
        }
        else if (*pM <= HI_DRV_DISP_FMT_VESA_2560X1600_60_RB)
        {
            *pU = Transfer_DVIFmtV2U(*pM);
            return HI_SUCCESS;
        }
        else if (*pM <= HI_DRV_DISP_FMT_4096X2160_24)
        {
            *pU = Transfer_4KFmtV2U(*pM);
            return HI_SUCCESS;
        }
        else if (*pM <= HI_DRV_DISP_FMT_1080i_59_94)
        {
            *pU = Transfer_DecimalFmtV2U(*pM);
            return HI_SUCCESS;
        }
        else if (*pM == HI_DRV_DISP_FMT_CUSTOM)
        {
            *pU = HI_UNF_ENC_FMT_BUTT;
            return HI_SUCCESS;
        }
        else
        {
            return HI_FAILURE;
        }
    }

//    return HI_SUCCESS;
}

HI_S32 Transfer_AspectRatio(HI_UNF_DISP_ASPECT_RATIO_S *pU, HI_U32 *pH, HI_U32 *pV, HI_BOOL bu2m)
{
    if (bu2m)
    {
        switch(pU->enDispAspectRatio)
        {
            case HI_UNF_DISP_ASPECT_RATIO_AUTO:
                *pH = 0;
                *pV = 0;
            break;
            case HI_UNF_DISP_ASPECT_RATIO_4TO3:
                *pH = 4;
                *pV = 3;
            break;
            case HI_UNF_DISP_ASPECT_RATIO_16TO9:
                *pH = 16;
                *pV = 9;
            break;
            case HI_UNF_DISP_ASPECT_RATIO_221TO1:
                *pH = 221;
                *pV = 100;
            break;
            case HI_UNF_DISP_ASPECT_RATIO_USER:
                *pH = pU->u32UserAspectWidth;
                *pV = pU->u32UserAspectHeight;
            break;
            default:
                *pH = 0;
                *pV = 0;
            break;
        }
    }
    else
    {
        if ( !(*pH) || !(*pV) )
        {
            pU->enDispAspectRatio = HI_UNF_DISP_ASPECT_RATIO_AUTO;
        }
        else if ( (*pH == 4) && (*pV == 3) )
        {
            pU->enDispAspectRatio = HI_UNF_DISP_ASPECT_RATIO_4TO3;
        }
        else if ( (*pH == 16) && (*pV == 9) )
        {
            pU->enDispAspectRatio = HI_UNF_DISP_ASPECT_RATIO_16TO9;
        }
        else if ( (*pH == 221) && (*pV == 100) )
        {
            pU->enDispAspectRatio = HI_UNF_DISP_ASPECT_RATIO_221TO1;
        }
        else
        {
            pU->enDispAspectRatio = HI_UNF_DISP_ASPECT_RATIO_USER;
            pU->u32UserAspectWidth  = *pH;
            pU->u32UserAspectHeight = *pV;
        }
    }

    return HI_SUCCESS;
}

HI_S32 Transfer_Timing(HI_UNF_DISP_TIMING_S *pU, HI_DRV_DISP_TIMING_S *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        pM->u32VFB = pU->VFB;
        pM->u32VBB = pU->VBB;
        pM->u32VACT = pU->VACT;

        pM->u32HFB = pU->HFB;
        pM->u32HBB = pU->HBB;
        pM->u32HACT = pU->HACT;

        pM->u32VPW = pU->VPW;
        pM->u32HPW = pU->HPW;
        pM->bIDV = pU->IDV;
        pM->bIHS = pU->IHS;
        pM->bIVS = pU->IVS;

        pM->bClkReversal = pU->ClockReversal;
        pM->u32DataWidth = (HI_DRV_DISP_INTF_DATA_WIDTH_E)pU->DataWidth;

        if (pU->ItfFormat == HI_UNF_DISP_INTF_DATA_FMT_YUV422)
            pM->eDataFmt = HI_DRV_DISP_INTF_DATA_FMT_YUV422;
        else
            pM->eDataFmt = HI_DRV_DISP_INTF_DATA_FMT_RGB444;

        pM->bDitherEnable = pU->DitherEnable;
        pM->bInterlace= pU->bInterlace;
        pM->u32PixFreq = pU->PixFreq;
        pM->u32VertFreq = pU->VertFreq;
        pM->u32AspectRatioW = pU->AspectRatioW;
        pM->u32AspectRatioH = pU->AspectRatioH;

        pM->u32bUseGamma = pU->bUseGamma;
        pM->u32Reserve0 = pU->Reserve0;
        pM->u32Reserve1 = pU->Reserve1;
    }
    else
    {
        pU->VFB= pM->u32VFB;
        pU->VBB = pM->u32VBB;
        pU->VACT= pM->u32VACT;

        pU->HFB = pM->u32HFB  ;
        pU->HBB = pM->u32HBB  ;
        pU->HACT = pM->u32HACT ;

        pU->VPW = pM->u32VPW;
        pU->HPW = pM->u32HPW ;
        pU->IDV = pM->bIDV ;
        pU->IHS = pM->bIHS ;
        pU->IVS = pM->bIVS ;

        pU->ClockReversal = pM->bClkReversal ;
        pU->DataWidth = (HI_UNF_DISP_INTF_DATA_WIDTH_E)pM->u32DataWidth;

        if (HI_DRV_DISP_INTF_DATA_FMT_YUV422 == pM->eDataFmt  )
            pU->ItfFormat = HI_UNF_DISP_INTF_DATA_FMT_YUV422 ;
        else
            pU->ItfFormat = HI_UNF_DISP_INTF_DATA_FMT_RGB444 ;

        pU->DitherEnable = pM->bDitherEnable ;
        pU->bInterlace = pM->bInterlace ;
        pU->PixFreq = pM->u32PixFreq  ;
        pU->VertFreq = pM->u32VertFreq ;
        pU->AspectRatioW = pM->u32AspectRatioW;
        pU->AspectRatioH = pM->u32AspectRatioH  ;

        pU->bUseGamma = pM->u32bUseGamma ;
         pU->Reserve0 = pM->u32Reserve0 ;
        pU->Reserve1 = pM->u32Reserve1  ;
    }

    return HI_SUCCESS;
}

HI_S32 Transfer_BGColor(HI_UNF_DISP_BG_COLOR_S *pU, HI_DRV_DISP_COLOR_S *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        pM->u8Blue  = pU->u8Blue;
        pM->u8Green = pU->u8Green;
        pM->u8Red   = pU->u8Red;
    }
    else
    {
        pU->u8Blue  = pM->u8Blue;
        pU->u8Green = pM->u8Green;
        pU->u8Red   = pM->u8Red;
    }

    return HI_SUCCESS;
}


HI_S32 Transfer_FrameRate(HI_U32 *pM, HI_UNF_VCODEC_FRMRATE_S *pU, HI_BOOL bu2m)
{
    if (bu2m)
    {
        *pM = pU->u32fpsInteger * 1000 + pU->u32fpsDecimal;
    }
    else
    {
        pU->u32fpsInteger = *pM /1000;
        pU->u32fpsDecimal = *pM - ((*pM / 1000) * 1000);
    }

    return HI_SUCCESS;
}

HI_S32 Transfer_VideoFormat(HI_UNF_VIDEO_FORMAT_E  *pU, HI_DRV_PIX_FORMAT_E *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
       switch (*pU)
        {
            case HI_UNF_FORMAT_YUV_SEMIPLANAR_422:
            *pM = HI_DRV_PIX_FMT_NV61_2X1;
            break;
            case HI_UNF_FORMAT_YUV_SEMIPLANAR_420:
            *pM = HI_DRV_PIX_FMT_NV21;
            break;
            case HI_UNF_FORMAT_YUV_SEMIPLANAR_400:
            *pM = HI_DRV_PIX_FMT_NV80;
            break;

            case HI_UNF_FORMAT_YUV_SEMIPLANAR_411:
            *pM = HI_DRV_PIX_FMT_NV12_411;
            break;
            case HI_UNF_FORMAT_YUV_SEMIPLANAR_422_1X2:
            *pM = HI_DRV_PIX_FMT_NV61;
            break;
            case HI_UNF_FORMAT_YUV_SEMIPLANAR_444:
            *pM = HI_DRV_PIX_FMT_NV42;
            break;
            case HI_UNF_FORMAT_YUV_PACKAGE_UYVY:
            *pM = HI_DRV_PIX_FMT_UYVY;
            break;
            case HI_UNF_FORMAT_YUV_PACKAGE_YUYV:
            *pM = HI_DRV_PIX_FMT_YUYV;
            break;

            case HI_UNF_FORMAT_YUV_SEMIPLANAR_420_UV:
            *pM = HI_DRV_PIX_FMT_NV12;
            break;

            case HI_UNF_FORMAT_YUV_PACKAGE_YVYU:
            *pM = HI_DRV_PIX_FMT_YVYU;
            break;
            case HI_UNF_FORMAT_YUV_PLANAR_400:
            *pM = HI_DRV_PIX_FMT_YUV400;
            break;
            case HI_UNF_FORMAT_YUV_PLANAR_411:
            *pM = HI_DRV_PIX_FMT_YUV411;
            break;
            case HI_UNF_FORMAT_YUV_PLANAR_420:
            *pM = HI_DRV_PIX_FMT_YUV420p;
            break;

            case HI_UNF_FORMAT_YUV_PLANAR_422_1X2:
            *pM = HI_DRV_PIX_FMT_YUV422_1X2;
            break;
            case HI_UNF_FORMAT_YUV_PLANAR_422_2X1:
            *pM = HI_DRV_PIX_FMT_YUV422_2X1;
            break;
            case HI_UNF_FORMAT_YUV_PLANAR_444:
            *pM = HI_DRV_PIX_FMT_YUV_444;
            break;
            case HI_UNF_FORMAT_YUV_PLANAR_410:
                *pM = HI_DRV_PIX_FMT_YUV410p;
            break;
            case HI_UNF_FORMAT_YUV_TILE_420:
                *pM = HI_DRV_PIX_FMT_NV21_TILE;
            break;
            case HI_UNF_FORMAT_YUV_TILE_420_UV:
                *pM = HI_DRV_PIX_FMT_NV12_TILE;
            break;
            default:
            return HI_FAILURE;
        }
    }
    else
    {
        switch (*pM)
        {
            case HI_DRV_PIX_FMT_NV61_2X1:
            *pU = HI_UNF_FORMAT_YUV_SEMIPLANAR_422;
            break;
            case HI_DRV_PIX_FMT_NV21:
            *pU = HI_UNF_FORMAT_YUV_SEMIPLANAR_420;
            break;
            case HI_DRV_PIX_FMT_NV80:
            *pU = HI_UNF_FORMAT_YUV_SEMIPLANAR_400;
            break;
            case HI_DRV_PIX_FMT_NV12_411:
            *pU = HI_UNF_FORMAT_YUV_SEMIPLANAR_411;
            break;
            case HI_DRV_PIX_FMT_NV61:
            *pU = HI_UNF_FORMAT_YUV_SEMIPLANAR_422_1X2;
            break;
            case HI_DRV_PIX_FMT_NV42:
            *pU = HI_UNF_FORMAT_YUV_SEMIPLANAR_444;
            break;
            case HI_DRV_PIX_FMT_UYVY:
            *pU = HI_UNF_FORMAT_YUV_PACKAGE_UYVY;
            break;
            case HI_DRV_PIX_FMT_YUYV:
            *pU = HI_UNF_FORMAT_YUV_PACKAGE_YUYV;
            break;
            case HI_DRV_PIX_FMT_YVYU:
            *pU = HI_UNF_FORMAT_YUV_PACKAGE_YVYU;
            break;

            case HI_DRV_PIX_FMT_YUV400:
            *pU = HI_UNF_FORMAT_YUV_PLANAR_400;
            break;
            case HI_DRV_PIX_FMT_YUV411:
            *pU = HI_UNF_FORMAT_YUV_PLANAR_411;
            break;
            case HI_DRV_PIX_FMT_YUV420p:
            *pU = HI_UNF_FORMAT_YUV_PLANAR_420;
            break;
            case HI_DRV_PIX_FMT_YUV422_1X2:
            *pU = HI_UNF_FORMAT_YUV_PLANAR_422_1X2;
            break;
            case HI_DRV_PIX_FMT_YUV422_2X1:
            *pU = HI_UNF_FORMAT_YUV_PLANAR_422_2X1;
            break;
            case HI_DRV_PIX_FMT_YUV_444:
            *pU = HI_UNF_FORMAT_YUV_PLANAR_444;
            break;
            case HI_DRV_PIX_FMT_YUV410p:
            *pU = HI_UNF_FORMAT_YUV_PLANAR_410;
            break;

            case HI_DRV_PIX_FMT_NV12:
            *pU = HI_UNF_FORMAT_YUV_SEMIPLANAR_420_UV;
            break;
            case HI_DRV_PIX_FMT_NV21_TILE:
                *pU = HI_UNF_FORMAT_YUV_TILE_420;
            break;
            case HI_DRV_PIX_FMT_NV12_TILE:
                *pU = HI_UNF_FORMAT_YUV_TILE_420_UV;
            break;
            default:
            return HI_FAILURE;

        }
    }

    return HI_SUCCESS;
}

static HI_DRV_ASP_RAT_MODE_E s_DrvACModeTab[HI_UNF_VO_ASPECT_CVRS_BUTT]=
{
    HI_DRV_ASP_RAT_MODE_FULL,
    HI_DRV_ASP_RAT_MODE_LETTERBOX,
    HI_DRV_ASP_RAT_MODE_PANANDSCAN,
    HI_DRV_ASP_RAT_MODE_COMBINED,
    HI_DRV_ASP_RAT_MODE_FULL_H,
    HI_DRV_ASP_RAT_MODE_FULL_V,
};
static HI_UNF_VO_ASPECT_CVRS_E s_UnfACModeTab[HI_DRV_ASP_RAT_MODE_BUTT]=
{
    HI_UNF_VO_ASPECT_CVRS_IGNORE,
    HI_UNF_VO_ASPECT_CVRS_LETTERBOX,
    HI_UNF_VO_ASPECT_CVRS_PAN_SCAN,
    HI_UNF_VO_ASPECT_CVRS_COMBINED,
    HI_UNF_VO_ASPECT_CVRS_HORIZONTAL_FULL,
    HI_UNF_VO_ASPECT_CVRS_VERTICAL_FULL,
    HI_UNF_VO_ASPECT_CVRS_BUTT,
};

HI_S32 Transfe_ARConvert(HI_UNF_VO_ASPECT_CVRS_E  *pU, HI_DRV_ASP_RAT_MODE_E *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        if (*pU < HI_UNF_VO_ASPECT_CVRS_BUTT)
        {
            *pM = s_DrvACModeTab[*pU];
            return HI_SUCCESS;
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else
    {
        if (*pM < HI_DRV_ASP_RAT_MODE_BUTT)
        {
            *pU = s_UnfACModeTab[*pM];
            return HI_SUCCESS;
        }
        else
        {
            return HI_FAILURE;
        }
    }

    //return HI_FAILURE;
}

#if 0
HI_S32 Transfe_FrostMode(HI_UNF_WINDOW_FROST_E *pU, HI_DRV_WINDOW_FROST_E *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        *pM = (HI_DRV_WINDOW_FROST_E) *pU;
    }
    else
    {
        *pU = (HI_UNF_WINDOW_FROST_E)*pM;
    }

    return HI_SUCCESS;
}
#endif


HI_S32 Transfe_ZOrder(HI_LAYER_ZORDER_E *pU, HI_DRV_DISP_ZORDER_E *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        *pM = (HI_DRV_DISP_ZORDER_E) *pU;
    }
    else
    {
        *pU = (HI_LAYER_ZORDER_E)*pM;
    }

    return HI_SUCCESS;
}
HI_S32 Transfe_SwitchMode(HI_UNF_WINDOW_FREEZE_MODE_E *pU, HI_DRV_WIN_SWITCH_E *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        *pM = (HI_DRV_WIN_SWITCH_E)(*pU);
    }
    else
    {
        *pU = (HI_UNF_WINDOW_FREEZE_MODE_E)(*pM);
    }

    return HI_SUCCESS;
}

HI_S32 Transfe_Rotate(HI_UNF_VO_ROTATION_E *pU, HI_DRV_ROT_ANGLE_E *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        if (*pU < HI_UNF_VO_ROTATION_BUTT)
        {
            *pM = (HI_DRV_ROT_ANGLE_E)*pU;
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else
    {
        if (*pM < HI_DRV_ROT_ANGLE_BUTT)
        {
            *pU = (HI_UNF_VO_ROTATION_E)*pM;
        }
        else
        {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}


static HI_DRV_FRAME_TYPE_E s_DrvFrameTypeTab[HI_UNF_FRAME_PACKING_TYPE_BUTT] =
{HI_DRV_FT_NOT_STEREO, HI_DRV_FT_SBS, HI_DRV_FT_TAB, HI_DRV_FT_FPK};

static HI_UNF_VIDEO_FRAME_PACKING_TYPE_E s_UnfFrameTypeTab[HI_DRV_FT_BUTT] =
{
HI_UNF_FRAME_PACKING_TYPE_NONE,
HI_UNF_FRAME_PACKING_TYPE_SIDE_BY_SIDE,
HI_UNF_FRAME_PACKING_TYPE_TOP_AND_BOTTOM,
HI_UNF_FRAME_PACKING_TYPE_TIME_INTERLACED,
};

static HI_DRV_FIELD_MODE_E s_DrvFMTab[HI_UNF_VIDEO_FIELD_BUTT] =
{HI_DRV_FIELD_ALL, HI_DRV_FIELD_TOP, HI_DRV_FIELD_BOTTOM};

static HI_UNF_VIDEO_FIELD_MODE_E s_UnfFMTab[HI_DRV_FIELD_BUTT] =
{HI_UNF_VIDEO_FIELD_TOP,HI_UNF_VIDEO_FIELD_BOTTOM, HI_UNF_VIDEO_FIELD_ALL};

#define TRANSFER_A2B(a, b)   a = b
#define TRANSFER_A2B_R(a, b) b = a



HI_S32 Transfer_Frame(HI_UNF_VIDEO_FRAME_INFO_S  *pU, HI_DRV_VIDEO_FRAME_S *pM, HI_BOOL bu2m)
{
    HI_UNF_VIDEO_FIELD_MODE_E  enFieldtmp = HI_UNF_VIDEO_FIELD_BUTT;
    HI_DRV_FIELD_MODE_E        enDrvField = HI_DRV_FIELD_BUTT;
    HI_DRV_VIDEO_TB_ADJUST_E   enDrvTbAdjust = HI_DRV_VIDEO_TB_BUTT;
    
    if (bu2m)
    {
        HI_DRV_VIDEO_PRIVATE_S *pstPriv;
        pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pM->u32Priv;
            
        if ((pU->enVideoFormat > HI_UNF_FORMAT_YUV_BUTT)
            || (pU->enFieldMode > enFieldtmp)
            || (pU->enFramePackingType > HI_UNF_FRAME_PACKING_TYPE_BUTT))
        {
            return HI_ERR_VO_INVALID_PARA;
        }

        memset(pM, 0, sizeof(HI_DRV_VIDEO_FRAME_S));
        if (pU->enFramePackingType != HI_UNF_FRAME_PACKING_TYPE_BUTT)
        {
            pM->eFrmType = s_DrvFrameTypeTab[pU->enFramePackingType];
        }
        else
        {
            pM->eFrmType = HI_DRV_FT_BUTT;
        }

        Transfer_VideoFormat(&pU->enVideoFormat, &pM->ePixFormat, HI_TRUE);

        TRANSFER_A2B(pM->bProgressive, pU->bProgressive);
        TRANSFER_A2B(pM->bTopFieldFirst, pU->bTopFieldFirst);
        TRANSFER_A2B(pM->u32Width    , pU->u32Width);
        TRANSFER_A2B(pM->u32Height   , pU->u32Height);

        //TRANSFER_A2B(pM->stDispRect.s32X      , pU->u32DisplayCenterX - (pU->u32DisplayWidth>>1) );
        pM->stDispRect.s32X = (HI_S32)(pU->u32DisplayCenterX - (pU->u32DisplayWidth>>1));
        //TRANSFER_A2B(pM->stDispRect.s32Y      , pU->u32DisplayCenterY - (pU->u32DisplayHeight>>1));
        pM->stDispRect.s32Y = (HI_S32)(pU->u32DisplayCenterY - (pU->u32DisplayHeight>>1));
        TRANSFER_A2B(pM->stDispRect.s32Width  , pU->u32DisplayWidth);
        TRANSFER_A2B(pM->stDispRect.s32Height , pU->u32DisplayHeight);
        TRANSFER_A2B(pM->u32AspectWidth , (HI_U8)pU->u32AspectWidth);
        TRANSFER_A2B(pM->u32AspectHeight, (HI_U8)pU->u32AspectHeight);

        // TODO:
        Transfer_FrameRate(&pM->u32FrameRate , &pU->stFrameRate, HI_TRUE);
        TRANSFER_A2B(pM->u32FrameIndex, pU->u32FrameIndex);


        TRANSFER_A2B(pM->u32SrcPts , pU->u32SrcPts);
        TRANSFER_A2B(pM->u32Pts    , pU->u32Pts);

        if (pU->enFieldMode != HI_UNF_VIDEO_FIELD_BUTT)
        {
            pM->enFieldMode = s_DrvFMTab[pU->enFieldMode];
        }
        else
        {
            pM->enFieldMode = HI_DRV_FIELD_BUTT;
        }
        TRANSFER_A2B(pM->u32ErrorLevel, pU->u32ErrorLevel);


        TRANSFER_A2B(pM->stBufAddr[0].u32PhyAddr_Y  , pU->stVideoFrameAddr[0].u32YAddr    );
        TRANSFER_A2B(pM->stBufAddr[0].u32Stride_Y   , pU->stVideoFrameAddr[0].u32YStride  );
        TRANSFER_A2B(pM->stBufAddr[0].u32PhyAddr_C  , pU->stVideoFrameAddr[0].u32CAddr    );
        TRANSFER_A2B(pM->stBufAddr[0].u32Stride_C   , pU->stVideoFrameAddr[0].u32CStride  );
        TRANSFER_A2B(pM->stBufAddr[0].u32PhyAddr_Cr , pU->stVideoFrameAddr[0].u32CrAddr   );
        TRANSFER_A2B(pM->stBufAddr[0].u32Stride_Cr  , pU->stVideoFrameAddr[0].u32CrStride );
        TRANSFER_A2B(pM->stBufAddr[1].u32PhyAddr_Y  , pU->stVideoFrameAddr[1].u32YAddr    );
        TRANSFER_A2B(pM->stBufAddr[1].u32Stride_Y   , pU->stVideoFrameAddr[1].u32YStride  );
        TRANSFER_A2B(pM->stBufAddr[1].u32PhyAddr_C  , pU->stVideoFrameAddr[1].u32CAddr    );
        TRANSFER_A2B(pM->stBufAddr[1].u32Stride_C   , pU->stVideoFrameAddr[1].u32CStride  );
        TRANSFER_A2B(pM->stBufAddr[1].u32PhyAddr_Cr , pU->stVideoFrameAddr[1].u32CrAddr   );
        TRANSFER_A2B(pM->stBufAddr[1].u32Stride_Cr  , pU->stVideoFrameAddr[1].u32CrStride );

        TRANSFER_A2B(pM->u32Circumrotate, pU->u32Circumrotate);
        TRANSFER_A2B(pM->bToFlip_V, pU->bVerticalMirror);
        TRANSFER_A2B(pM->bToFlip_H, pU->bHorizontalMirror);

        //memcpy(pM->u32Priv, pU->u32Private, sizeof(HI_U32) * 64);
        if (pU->u32Private[0] == HI_TRUE)
        {
            pM->bSecure = HI_TRUE;
        }
        else
        {
            pM->bSecure = HI_FALSE;
        }
        
        if (pU->u32Width >720 &&  pU->u32Height > 576)
        {
            pstPriv->eColorSpace = HI_DRV_CS_BT709_YUV_LIMITED;
        }
        else
        {
            pstPriv->eColorSpace = HI_DRV_CS_BT601_YUV_LIMITED;
        }

        return HI_SUCCESS;
    }
    else
    {
        if ((pM->ePixFormat > HI_DRV_PIX_BUTT)
            || (pM->enFieldMode > enDrvField)
            || (pM->eFrmType > HI_DRV_FT_BUTT)
            || (pM->enTBAdjust > enDrvTbAdjust))
        {
            return HI_ERR_VO_INVALID_PARA;
        }

        memset(pU, 0, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));

        if (pM->eFrmType != HI_DRV_FT_BUTT)
        {
            pU->enFramePackingType = s_UnfFrameTypeTab[pM->eFrmType];
        }
        else
        {
            pU->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_BUTT;
        }

        Transfer_VideoFormat(&pU->enVideoFormat, &pM->ePixFormat, HI_FALSE);

        TRANSFER_A2B_R(pM->bProgressive, pU->bProgressive);
        TRANSFER_A2B_R(pM->bTopFieldFirst, pU->bTopFieldFirst);
        TRANSFER_A2B_R(pM->u32Width    , pU->u32Width);
        TRANSFER_A2B_R(pM->u32Height   , pU->u32Height);

        //TRANSFER_A2B_R(pM->stDispRect.s32X + (pM->stDispRect.s32Width>>1)  , pU->u32DisplayCenterX);
        pU->u32DisplayCenterX = (HI_U32)(pM->stDispRect.s32X + (pM->stDispRect.s32Width>>1));
        //TRANSFER_A2B_R(pM->stDispRect.s32Y + (pM->stDispRect.s32Height>>1) , pU->u32DisplayCenterY);
        pU->u32DisplayCenterY = (HI_U32)(pM->stDispRect.s32Y + (pM->stDispRect.s32Height>>1) );
        TRANSFER_A2B_R(pM->stDispRect.s32Width  , pU->u32DisplayWidth);

        TRANSFER_A2B_R(pM->stDispRect.s32Height , pU->u32DisplayHeight);
        TRANSFER_A2B_R((HI_U32)pM->u32AspectWidth, pU->u32AspectWidth);
        TRANSFER_A2B_R((HI_U32)pM->u32AspectHeight, pU->u32AspectHeight);

        Transfer_FrameRate(&pM->u32FrameRate , &pU->stFrameRate, HI_FALSE);

        TRANSFER_A2B_R(pM->u32FrameIndex, pU->u32FrameIndex);
        TRANSFER_A2B_R(pM->u32SrcPts , pU->u32SrcPts);
        TRANSFER_A2B_R(pM->u32Pts    , pU->u32Pts);

        if (pM->enFieldMode != HI_DRV_FIELD_BUTT)
        {
            pU->enFieldMode = s_UnfFMTab[pM->enFieldMode];
        }
        else
        {
            pU->enFieldMode = HI_UNF_VIDEO_FIELD_BUTT;
        }
        TRANSFER_A2B_R(pM->u32ErrorLevel, pU->u32ErrorLevel);



        TRANSFER_A2B_R(pM->stBufAddr[0].u32PhyAddr_Y  , pU->stVideoFrameAddr[0].u32YAddr    );
        TRANSFER_A2B_R(pM->stBufAddr[0].u32Stride_Y   , pU->stVideoFrameAddr[0].u32YStride  );
        TRANSFER_A2B_R(pM->stBufAddr[0].u32PhyAddr_C  , pU->stVideoFrameAddr[0].u32CAddr    );
        TRANSFER_A2B_R(pM->stBufAddr[0].u32Stride_C   , pU->stVideoFrameAddr[0].u32CStride  );

        TRANSFER_A2B_R(pM->stBufAddr[0].u32PhyAddr_Cr , pU->stVideoFrameAddr[0].u32CrAddr   );
        TRANSFER_A2B_R(pM->stBufAddr[0].u32Stride_Cr  , pU->stVideoFrameAddr[0].u32CrStride );
        TRANSFER_A2B_R(pM->stBufAddr[1].u32PhyAddr_Y  , pU->stVideoFrameAddr[1].u32YAddr    );
        TRANSFER_A2B_R(pM->stBufAddr[1].u32Stride_Y   , pU->stVideoFrameAddr[1].u32YStride  );

        TRANSFER_A2B_R(pM->stBufAddr[1].u32PhyAddr_C  , pU->stVideoFrameAddr[1].u32CAddr    );
        TRANSFER_A2B_R(pM->stBufAddr[1].u32Stride_C   , pU->stVideoFrameAddr[1].u32CStride  );
        TRANSFER_A2B_R(pM->stBufAddr[1].u32PhyAddr_Cr , pU->stVideoFrameAddr[1].u32CrAddr   );
        TRANSFER_A2B_R(pM->stBufAddr[1].u32Stride_Cr  , pU->stVideoFrameAddr[1].u32CrStride );


        TRANSFER_A2B_R(pM->u32Circumrotate, pU->u32Circumrotate);
        TRANSFER_A2B_R(pM->bToFlip_V, pU->bVerticalMirror);
        TRANSFER_A2B_R(pM->bToFlip_H, pU->bHorizontalMirror);

        //memcpy(pU->u32Private, pM->u32Priv, sizeof(HI_U32) * 64);
        pU->u32Private[0] = pM->bSecure;

        return HI_SUCCESS;
    }
}

HI_S32 Transfer_BufferPool(HI_UNF_BUFFER_ATTR_S *pU, HI_DRV_VIDEO_BUFFER_POOL_S*pM, HI_BOOL bu2m)
{
#if 0
    if (bu2m)
    {

    }
    else
    {

    }
#endif

    return HI_SUCCESS;
}

HI_S32 Transfer_CastCfg(HI_UNF_DISP_CAST_ATTR_S  *pU, HI_DRV_DISP_CAST_CFG_S *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        HI_S32 i;
        memset(pM, 0, sizeof(HI_DRV_DISP_CAST_CFG_S));

        Transfer_VideoFormat(&pU->enFormat, &pM->eFormat, HI_TRUE);

        pM->u32Width   = pU->u32Width;
        pM->u32Height  = pU->u32Height;

        pM->u32BufNumber = pU->u32BufNum;
        pM->bUserAlloc   = pU->bUserAlloc;
        pM->bLowDelay    = pU->bLowDelay;
        pM->u32BufSize   = pU->u32BufSize;
        pM->u32BufStride = pU->u32BufStride;
        
        pM->bCrop = pU->bCrop;
        pM->stCropRect.u32TopOffset = pU->stCropRect.u32TopOffset;
        pM->stCropRect.u32BottomOffset = pU->stCropRect.u32BottomOffset;
        pM->stCropRect.u32LeftOffset  = pU->stCropRect.u32LeftOffset;
        pM->stCropRect.u32RightOffset = pU->stCropRect.u32RightOffset;

        for(i=0; i<HI_DISP_CAST_BUFFER_MAX_NUMBER; i++)
        {
            pM->u32BufPhyAddr[i] = pU->u32BufPhyAddr[i];
        }
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}


HI_S32 Transfer_CgmsCfg(const HI_UNF_DISP_CGMS_CFG_S  *pU, HI_DRV_DISP_CGMSA_CFG_S *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
		pM->bEnable =  pU->bEnable;
		pM->enType  =  (HI_DRV_DISP_CGMSA_TYPE_E)pU->enType;
		pM->enMode  =  (HI_DRV_DISP_CGMSA_MODE_E)pU->enMode;
		return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}


HI_U8 Transfer_GetVdacIdFromPinIDForMPW(HI_U8 PinId)
{
    switch (PinId)
    {
        case 0:
            return (HI_U8)0;
        case 1:
            return (HI_U8)1;
        case 2:
            return (HI_U8)2;
        case 3:
            return (HI_U8)3;
        default:
            return (HI_U8)0xff;
    }
}

HI_S32 Transfer_Intf(HI_UNF_DISP_INTF_S *pU, HI_DRV_DISP_INTF_S *pM, HI_BOOL bu2m)
{
    if (bu2m)
    {
        /* set invalid value */
        pM->u8VDAC_Y_G  = HI_DISP_VDAC_INVALID_ID;
        pM->u8VDAC_Pb_B = HI_DISP_VDAC_INVALID_ID;
        pM->u8VDAC_Pr_R = HI_DISP_VDAC_INVALID_ID;

        switch (pU->enIntfType)
        {
            case HI_UNF_DISP_INTF_TYPE_HDMI:
                pM->eID = (HI_DRV_DISP_INTF_ID_E)((HI_U32)HI_DRV_DISP_INTF_HDMI0 + ((HI_U32)pU->unIntf.enHdmi - (HI_U32)HI_UNF_HDMI_ID_0));
                if ((pM->eID > HI_DRV_DISP_INTF_HDMI2) || (pM->eID < HI_DRV_DISP_INTF_HDMI0))
                {
                    return HI_FAILURE;
                }
                break;
            case HI_UNF_DISP_INTF_TYPE_YPBPR:
                pM->eID = HI_DRV_DISP_INTF_YPBPR0;
                pM->u8VDAC_Y_G  = Transfer_GetVdacIdFromPinIDForMPW(pU->unIntf.stYPbPr.u8DacY);
                pM->u8VDAC_Pb_B = Transfer_GetVdacIdFromPinIDForMPW(pU->unIntf.stYPbPr.u8DacPb);
                pM->u8VDAC_Pr_R = Transfer_GetVdacIdFromPinIDForMPW(pU->unIntf.stYPbPr.u8DacPr);
                break;
            case HI_UNF_DISP_INTF_TYPE_SVIDEO:
                pM->eID = HI_DRV_DISP_INTF_SVIDEO0;
                pM->u8VDAC_Y_G  = Transfer_GetVdacIdFromPinIDForMPW(pU->unIntf.stSVideo.u8DacY);
                pM->u8VDAC_Pb_B = Transfer_GetVdacIdFromPinIDForMPW(pU->unIntf.stSVideo.u8DacC);
                break;
            case HI_UNF_DISP_INTF_TYPE_CVBS:
                pM->eID = HI_DRV_DISP_INTF_CVBS0;
                pM->u8VDAC_Y_G  = Transfer_GetVdacIdFromPinIDForMPW(pU->unIntf.stCVBS.u8Dac);
                break;
            case HI_UNF_DISP_INTF_TYPE_RGB:
                pM->eID = HI_DRV_DISP_INTF_RGB0;
                pM->u8VDAC_Y_G  = Transfer_GetVdacIdFromPinIDForMPW(pU->unIntf.stRGB.u8DacG);
                pM->u8VDAC_Pb_B = Transfer_GetVdacIdFromPinIDForMPW(pU->unIntf.stRGB.u8DacB);
                pM->u8VDAC_Pr_R = Transfer_GetVdacIdFromPinIDForMPW(pU->unIntf.stRGB.u8DacR);
                pM->bDacSync = pU->unIntf.stRGB.bDacSync;
                break;
            case HI_UNF_DISP_INTF_TYPE_VGA:
                pM->eID = HI_DRV_DISP_INTF_VGA0;
                pM->u8VDAC_Y_G  = Transfer_GetVdacIdFromPinIDForMPW(pU->unIntf.stRGB.u8DacG);
                pM->u8VDAC_Pb_B = Transfer_GetVdacIdFromPinIDForMPW(pU->unIntf.stRGB.u8DacB);
                pM->u8VDAC_Pr_R = Transfer_GetVdacIdFromPinIDForMPW(pU->unIntf.stRGB.u8DacR);
                pM->bDacSync = pU->unIntf.stRGB.bDacSync;
                break;
            case HI_UNF_DISP_INTF_TYPE_LCD:
                pM->eID = HI_DRV_DISP_INTF_LCD0 + (pU->unIntf.enLcd - HI_UNF_DISP_LCD_0);
                if (pM->eID > HI_DRV_DISP_INTF_LCD2)
                {
                    return HI_FAILURE;
                }
                break;
            case HI_UNF_DISP_INTF_TYPE_BT1120:
                pM->eID = HI_DRV_DISP_INTF_BT1120_0 + (pU->unIntf.enBT1120 - HI_UNF_DISP_BT1120_0);
                if (pM->eID > HI_DRV_DISP_INTF_BT1120_2)
                {
                    return HI_FAILURE;
                }
                break;
/*
            case HI_UNF_DISP_INTF_TYPE_LCD:
                pM->eID =  + (pU->unIntf.enLCD - );
                if (pM->eID > )
                {
                    return HI_FAILURE;
                }
                break;
            case HI_UNF_DISP_INTF_TYPE_BT1120:
                pM->eID =  + (pU->unIntf.enHDMI - );
                if (pM->eID > )
                {
                    return HI_FAILURE;
                }
                break;
            case HI_UNF_DISP_INTF_TYPE_BT656:
                pM->eID =  + (pU->unIntf.enHDMI - );
                if (pM->eID > )
                {
                    return HI_FAILURE;
                }
                break;
            case HI_UNF_DISP_INTF_TYPE_RGB:
                pM->eID =  + (pU->unIntf.enHDMI - );
                if (pM->eID > )
                {
                    return HI_FAILURE;
                }
                break;
*/
            default:
                return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

