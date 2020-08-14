/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_disp_adp2unf.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/09/20
  Description   :
  History       :
  1.Date        :
  Author        :
  Modification  : Created file

*******************************************************************************/
#ifndef __DISP_PLATFORM_BOOT__
#include "hi_unf_disp.h"
#include "hi_drv_video.h"
#include "hi_drv_disp.h"
#include "mpi_disp_tran.h"

#include "hi_drv_pdm.h"
#include "drv_pdm_ext.h"
#include "hi_drv_module.h"

#include "drv_display.h"
#include "drv_disp_com.h"

#else

#include "hi_unf_disp.h"
#include "drv_disp_com.h"
#include "drv_display.h"
#include "hi_drv_video.h"
#include "hi_drv_disp.h"
//#include "mpi_disp_tran.h"
#include "hi_drv_pdm.h"

#endif
#include "drv_disp_hal.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 DISP_PrintParam(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S* pP)
{
    HI_S32 i;

    DISP_PRINT("DISP%d PDM Param:\n", enDisp);

    DISP_PRINT("Fmt=%d, B=%d, C=%d, S=%d,H=%d,GM=%d\n",
               pP->enFormat,
               pP->u32Brightness,
               pP->u32Contrast,
               pP->u32Saturation,
               pP->u32HuePlus,
               pP->bGammaEnable);
#if 0
    DISP_PRINT("x=%d, y=%d, w=%d, h=%d\n",
               pP->u32ScreenXpos,
               pP->u32ScreenYpos,
               pP->u32ScreenWidth,
               pP->u32ScreenHeight);

    DISP_PRINT("BGC=%d, =%d, =%d, AR=%d,WvsH=%dvs%d\n",
               pP->stBgColor.u8Red,
               pP->stBgColor.u8Green,
               pP->stBgColor.u8Blue,
               pP->stAspectRatio.enDispAspectRatio,
               pP->stAspectRatio.u32UserAspectWidth,
               pP->stAspectRatio.u32UserAspectHeight);
#endif

    for (i = 0; i < HI_UNF_DISP_INTF_TYPE_BUTT; i++)
    {
        //DISP_PRINT("INTF %d, type=%d : ", i, pP->stIntf[i].enIntfType);
        if (pP->stIntf[i].enIntfType == HI_UNF_DISP_INTF_TYPE_HDMI)
        {
            DISP_PRINT("HDMI ID=%d\n", pP->stIntf[i].enIntfType);
        }
        else if (pP->stIntf[i].enIntfType == HI_UNF_DISP_INTF_TYPE_YPBPR)
        {
            DISP_PRINT("Y=%d, Pb=%d, Pr=%d\n",
                       pP->stIntf[i].unIntf.stYPbPr.u8DacY,
                       pP->stIntf[i].unIntf.stYPbPr.u8DacPb,
                       pP->stIntf[i].unIntf.stYPbPr.u8DacPr);
        }
        else if (pP->stIntf[i].enIntfType == HI_UNF_DISP_INTF_TYPE_CVBS)
        {
            DISP_PRINT("CVBS=%d\n",
                       pP->stIntf[i].unIntf.stCVBS.u8Dac);
        }
        else
        {
            //DISP_PRINT("NULL\n");
        }

    }
#if 0
    DISP_PRINT("TIMING===============================\n");
    DISP_PRINT(" VFB=%d,VBB=%d,VACT=%d\n HFB=%d,HBB=%d,HACT=%d\n VPW=%d,HPW=%d\n IDV=%d,IHS=%d,IVS=%d\n",
               pP->stDispTiming.VFB,
               pP->stDispTiming.VBB,
               pP->stDispTiming.VACT,
               pP->stDispTiming.HFB,
               pP->stDispTiming.HBB,
               pP->stDispTiming.HACT,
               pP->stDispTiming.VPW,
               pP->stDispTiming.HPW,
               pP->stDispTiming.IDV,
               pP->stDispTiming.IHS,
               pP->stDispTiming.IVS);
#endif

    return HI_SUCCESS;
}



HI_S32 DISP_CheckParam(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S* pP)
{
    HI_S32 i;

    //DISP_PrintParam(enDisp, pP);

    if (enDisp == HI_UNF_DISPLAY1)
    {
        if (pP->enFormat > HI_UNF_ENC_FMT_BUTT)
        {
            DISP_ERROR("invalid enformt!\n");
            return HI_FAILURE;
        }

        for (i = 0; i < HI_UNF_DISP_INTF_TYPE_BUTT; i++)
        {
            if (pP->stIntf[i].enIntfType != HI_UNF_DISP_INTF_TYPE_BUTT)
            {
                if (pP->stIntf[i].enIntfType == HI_UNF_DISP_INTF_TYPE_YPBPR)
                {
                    if (   ( pP->stIntf[i].unIntf.stYPbPr.u8DacY > 3)
                           || ( pP->stIntf[i].unIntf.stYPbPr.u8DacPb > 3)
                           || ( pP->stIntf[i].unIntf.stYPbPr.u8DacPr > 3)
                           || ( pP->stIntf[i].unIntf.stYPbPr.u8DacY == pP->stIntf[i].unIntf.stYPbPr.u8DacPb)
                           || ( pP->stIntf[i].unIntf.stYPbPr.u8DacY == pP->stIntf[i].unIntf.stYPbPr.u8DacPr)
                           || ( pP->stIntf[i].unIntf.stYPbPr.u8DacPb == pP->stIntf[i].unIntf.stYPbPr.u8DacPr)
                       )
                    {
                        DISP_ERROR("invalid vadc id!\n");
                        return HI_FAILURE;
                    }
                }
            }
        }

    }
    else
    {
        //HI_UNF_DISPLAY1
         if (pP->enFormat > HI_UNF_ENC_FMT_BUTT)
        {
            DISP_ERROR("invalid enformt!\n");
            return HI_FAILURE;
        }

        for (i = 0; i < HI_UNF_DISP_INTF_TYPE_BUTT; i++)
        {
            if (pP->stIntf[i].enIntfType != HI_UNF_DISP_INTF_TYPE_BUTT)
            {
                if (pP->stIntf[i].enIntfType == HI_UNF_DISP_INTF_TYPE_CVBS)
                {
                    if ( pP->stIntf[i].unIntf.stCVBS.u8Dac > 3)
                    {
                        DISP_ERROR("invalid vadc id!\n");
                        return HI_FAILURE;
                    }
                }
            }
        }

    }

    if (  (pP->u32Brightness > 100 )
          || (pP->u32Contrast > 100)
          || (pP->u32Saturation > 100)
          || (pP->u32HuePlus > 100) )
    {
        DISP_ERROR("invalid color param!\n");
        return HI_FAILURE;
    }

    /*
        pP->bGammaEnable;
        pP->u32ScreenXpos;
        pP->u32ScreenYpos;
        pP->u32ScreenWidth;
        pP->u32ScreenHeight;
        pP->stBgColor;
    */
    if (pP->stAspectRatio.enDispAspectRatio > HI_UNF_DISP_ASPECT_RATIO_USER)
    {
        DISP_ERROR("invalid aspect ratio param!\n");
        return HI_FAILURE;
    }

    if (pP->stAspectRatio.enDispAspectRatio == HI_UNF_DISP_ASPECT_RATIO_USER)
    {
        if (  (pP->stAspectRatio.u32UserAspectWidth > (pP->stAspectRatio.u32UserAspectHeight * 16))
              || (pP->stAspectRatio.u32UserAspectHeight > (pP->stAspectRatio.u32UserAspectWidth * 16)))
        {
            DISP_ERROR("invalid aspect ratio param!\n");
            return HI_FAILURE;
        }
    }

    //pP->stDispTiming;

    return HI_SUCCESS;
}


HI_S32 HI_PDM_GetDispParamTEST(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S* pP)
{
    HI_S32 i;

    if (enDisp == HI_UNF_DISPLAY1)
    {
        pP->enFormat = HI_UNF_ENC_FMT_1080i_50;
        pP->u32Brightness = 50;
        pP->u32Contrast   = 50;
        pP->u32Saturation = 50;
        pP->u32HuePlus    = 50;
        pP->bGammaEnable  = HI_FALSE;
        pP->u32VirtScreenWidth  = 0;
        pP->u32VirtScreenHeight = 0;
        pP->stBgColor.u8Red   = 0;
        pP->stBgColor.u8Green = 0;
        pP->stBgColor.u8Blue  = 0;
        pP->stAspectRatio.enDispAspectRatio = HI_UNF_DISP_ASPECT_RATIO_16TO9;

        pP->stIntf[0].enIntfType = HI_UNF_DISP_INTF_TYPE_YPBPR;
        pP->stIntf[0].unIntf.stYPbPr.u8DacY  = 0;
        pP->stIntf[0].unIntf.stYPbPr.u8DacPb = 1;
        pP->stIntf[0].unIntf.stYPbPr.u8DacPr = 3;

        pP->stIntf[1].enIntfType = HI_UNF_DISP_INTF_TYPE_HDMI;
        pP->stIntf[1].unIntf.enHdmi = HI_UNF_HDMI_ID_0;

        for (i = 1; i < HI_UNF_DISP_INTF_TYPE_BUTT; i++)
        {
            pP->stIntf[i].enIntfType = HI_UNF_DISP_INTF_TYPE_BUTT;
        }
        //pP->stDispTiming;
    }
    else
    {
        //HI_UNF_DISPLAY1
        pP->enFormat = HI_UNF_ENC_FMT_PAL;
        pP->u32Brightness = 50;
        pP->u32Contrast   = 50;
        pP->u32Saturation = 50;
        pP->u32HuePlus    = 50;
        pP->bGammaEnable  = HI_FALSE;
        pP->u32VirtScreenWidth  = 0;
        pP->u32VirtScreenHeight = 0;
        pP->stBgColor.u8Red   = 0;
        pP->stBgColor.u8Green = 0;
        pP->stBgColor.u8Blue  = 0;
        pP->stAspectRatio.enDispAspectRatio = HI_UNF_DISP_ASPECT_RATIO_4TO3;

        pP->stIntf[0].enIntfType = HI_UNF_DISP_INTF_TYPE_CVBS;
        pP->stIntf[0].unIntf.stCVBS.u8Dac = 2;

        for (i = 1; i < HI_UNF_DISP_INTF_TYPE_BUTT; i++)
        {
            pP->stIntf[i].enIntfType = HI_UNF_DISP_INTF_TYPE_BUTT;
        }
    }

    return HI_SUCCESS;
}

#ifdef __DISP_PLATFORM_BOOT__
HI_S32 HI_UNF_DISP_Init(HI_VOID)
{
    HI_S32 nRet;
    // disp init
    nRet = DISP_Init();

    return nRet;
}

HI_S32 HI_UNF_DISP_DeInit(HI_VOID)
{
    //HI_S32 nRet;

    // disp deinit
    DISP_DeInit();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_DISP_Open (HI_UNF_DISP_E enDisp)
{
    HI_S32 nRet;

    nRet = DISP_Open((HI_DRV_DISPLAY_E)(enDisp));

    return nRet;
}

HI_S32 HI_UNF_DISP_Close(HI_UNF_DISP_E enDisp)
{
    HI_S32 nRet;

    nRet =  DISP_Close((HI_DRV_DISPLAY_E)(enDisp));
    return nRet;
}
HI_S32 HI_DISP_GetDisplayInfo(HI_DRV_DISPLAY_E enDisp, HI_DISP_DISPLAY_INFO_S *pstInfo)
{
    HI_S32 Ret;


    Ret = DISP_GetDisplayInfo(enDisp, pstInfo);


    return Ret;
}

#endif

HI_DRV_DISP_FMT_E DISP_Disp0FmtRevise_Attach(HI_DRV_DISP_FMT_E U)
{
    if (U >= HI_DRV_DISP_FMT_PAL &&  U  <= HI_DRV_DISP_FMT_SECAM_H)
        return U;

    switch (U)
    {
        case HI_DRV_DISP_FMT_1080P_50:
        case HI_DRV_DISP_FMT_1080P_25:
        case HI_DRV_DISP_FMT_1080i_50:
        case HI_DRV_DISP_FMT_720P_50:
        case HI_DRV_DISP_FMT_576P_50:
        case HI_DRV_DISP_FMT_720P_50_FP:
        case HI_DRV_DISP_FMT_1440x576i_50:
		case HI_DRV_DISP_FMT_3840X2160_25:
		case HI_DRV_DISP_FMT_3840X2160_50:
		case HI_DRV_DISP_FMT_4096X2160_25:
		case HI_DRV_DISP_FMT_4096X2160_50:
            return HI_DRV_DISP_FMT_PAL;
        case HI_DRV_DISP_FMT_3840X2160_24:
		case HI_DRV_DISP_FMT_3840X2160_30:
		case HI_DRV_DISP_FMT_3840X2160_60:
		case HI_DRV_DISP_FMT_4096X2160_24:
		case HI_DRV_DISP_FMT_4096X2160_30:
		case HI_DRV_DISP_FMT_4096X2160_60:
        case HI_DRV_DISP_FMT_1080P_24:
        case HI_DRV_DISP_FMT_1080P_24_FP:
        case HI_DRV_DISP_FMT_1080P_60:
        case HI_DRV_DISP_FMT_1080P_30:
        case HI_DRV_DISP_FMT_1080i_60:
        case HI_DRV_DISP_FMT_720P_60:
        case HI_DRV_DISP_FMT_480P_60:
        case HI_DRV_DISP_FMT_1440x480i_60:
        case HI_DRV_DISP_FMT_720P_60_FP:
        case HI_DRV_DISP_FMT_3840X2160_23_976:
        case HI_DRV_DISP_FMT_3840X2160_29_97:
        case HI_DRV_DISP_FMT_720P_59_94:
        case HI_DRV_DISP_FMT_1080P_59_94:
        case HI_DRV_DISP_FMT_1080P_29_97:
        case HI_DRV_DISP_FMT_1080P_23_976:
        case HI_DRV_DISP_FMT_1080i_59_94:
            return HI_DRV_DISP_FMT_NTSC;
        default:
            return HI_DRV_DISP_FMT_NTSC;
    }
}

HI_DRV_DISP_FMT_E DISP_TVHDFmtU2V(HI_UNF_ENC_FMT_E U)
{
    if (U <= HI_UNF_ENC_FMT_480P_60)
    {
        return (HI_DRV_DISP_FMT_E)(HI_DRV_DISP_FMT_1080P_60 + (U - HI_UNF_ENC_FMT_1080P_60));
    }
    else
    {
        return HI_DRV_DISP_FMT_1080i_50;
    }
}



HI_DRV_DISP_FMT_E DISP_TVSDFmtU2V(HI_UNF_ENC_FMT_E U)
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

HI_DRV_DISP_FMT_E DISP_3DFmtU2V(HI_UNF_ENC_FMT_E U)
{
    if (U <= HI_UNF_ENC_FMT_720P_50_FRAME_PACKING)
    {
        return (HI_DRV_DISP_FMT_E)(HI_DRV_DISP_FMT_1080P_24_FP + (U - HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING));
    }
    else
    {
        return HI_DRV_DISP_FMT_1080P_24_FP;
    }
}

HI_DRV_DISP_FMT_E DISP_4KFmtU2V(HI_UNF_ENC_FMT_E U)
{
    if (U <= HI_UNF_ENC_FMT_4096X2160_60)
    {
        return (HI_DRV_DISP_FMT_E)(HI_DRV_DISP_FMT_3840X2160_24 + (U - HI_UNF_ENC_FMT_3840X2160_24));
    }
    else
    {
        return HI_DRV_DISP_FMT_3840X2160_24;
    }
}

HI_DRV_DISP_FMT_E DISP_DecimalFmtU2V(HI_UNF_ENC_FMT_E U)
{
    if (U <= HI_UNF_ENC_FMT_1080i_59_94)
    {
        return (HI_DRV_DISP_FMT_E)(HI_DRV_DISP_FMT_3840X2160_23_976 + (U - HI_UNF_ENC_FMT_3840X2160_23_976));
    }
    else
    {
        return HI_DRV_DISP_FMT_3840X2160_23_976;
    }
}


HI_DRV_DISP_FMT_E DISP_DVIFmtU2V(HI_UNF_ENC_FMT_E U)
{
    if (U <= HI_UNF_ENC_FMT_VESA_2560X1600_60_RB)
    {
        return (HI_DRV_DISP_FMT_E)(HI_DRV_DISP_FMT_861D_640X480_60 + (U - HI_UNF_ENC_FMT_861D_640X480_60));
    }
    else
    {
        return HI_DRV_DISP_FMT_861D_640X480_60;
    }
}

HI_DRV_DISP_FMT_E DISP_GetEncFmt(HI_UNF_ENC_FMT_E enUnFmt)
{
    if (enUnFmt <= HI_UNF_ENC_FMT_480P_60)
    {
        return DISP_TVHDFmtU2V(enUnFmt);
    }
    else if (enUnFmt <= HI_UNF_ENC_FMT_SECAM_COS)
    {
        return DISP_TVSDFmtU2V(enUnFmt);
    }
    else if (enUnFmt <= HI_UNF_ENC_FMT_720P_50_FRAME_PACKING)
    {
        return DISP_3DFmtU2V(enUnFmt);
    }
    else if (enUnFmt <= HI_UNF_ENC_FMT_VESA_2560X1600_60_RB)
    {
        return DISP_DVIFmtU2V(enUnFmt);
    }
    else if (enUnFmt <= HI_UNF_ENC_FMT_4096X2160_24)
    {
        return DISP_4KFmtU2V(enUnFmt);
    }
    else if (enUnFmt <= HI_UNF_ENC_FMT_1080i_59_94)
    {
        return DISP_DecimalFmtU2V(enUnFmt);
    }
    else if (enUnFmt == HI_UNF_ENC_FMT_BUTT)
    {
        return HI_DRV_DISP_FMT_CUSTOM;
    }
    else
    {
        return HI_DRV_DISP_FMT_PAL;
    }
}

HI_U8 DISP_GetVdacIdFromPinIDForMPW(HI_U8 PinId)
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


HI_S32 DISP_GetDrvIntf(HI_UNF_DISP_INTF_S* pU, HI_DRV_DISP_INTF_S* pM, HI_BOOL bu2m)
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
                pM->eID = HI_DRV_DISP_INTF_HDMI0 + (pU->unIntf.enHdmi - HI_UNF_HDMI_ID_0);
                if (pM->eID > HI_DRV_DISP_INTF_HDMI2)
                {
                    return HI_FAILURE;
                }
                break;
            case HI_UNF_DISP_INTF_TYPE_YPBPR:
                pM->eID = HI_DRV_DISP_INTF_YPBPR0;
                pM->u8VDAC_Y_G  = DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stYPbPr.u8DacY);
                pM->u8VDAC_Pb_B = DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stYPbPr.u8DacPb);
                pM->u8VDAC_Pr_R = DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stYPbPr.u8DacPr);
                break;
            case HI_UNF_DISP_INTF_TYPE_SVIDEO:
                pM->eID = HI_DRV_DISP_INTF_SVIDEO0;
                pM->u8VDAC_Y_G  = DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stSVideo.u8DacY);
                pM->u8VDAC_Pb_B = DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stSVideo.u8DacC);
                break;
            case HI_UNF_DISP_INTF_TYPE_CVBS:
                pM->eID = HI_DRV_DISP_INTF_CVBS0;
                pM->u8VDAC_Y_G  = DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stCVBS.u8Dac);
                break;
            case HI_UNF_DISP_INTF_TYPE_VGA:
                pM->eID = HI_DRV_DISP_INTF_VGA0;
                pM->u8VDAC_Y_G  = DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stVGA.u8DacG);
                pM->u8VDAC_Pb_B = DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stVGA.u8DacB);
                pM->u8VDAC_Pr_R = DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stVGA.u8DacR);
                break;
            case HI_UNF_DISP_INTF_TYPE_RGB:
                pM->eID = HI_DRV_DISP_INTF_RGB0;
                pM->u8VDAC_Y_G  = DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stRGB.u8DacG);
                pM->u8VDAC_Pb_B = DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stRGB.u8DacB);
                pM->u8VDAC_Pr_R = DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stRGB.u8DacR);
                pM->bDacSync= DISP_GetVdacIdFromPinIDForMPW(pU->unIntf.stRGB.bDacSync);
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

HI_S32 ParaTransfer_Timing(HI_UNF_DISP_TIMING_S *pU, HI_DRV_DISP_TIMING_S *pM, HI_BOOL bu2m)
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
        pM->u32ClkPara0 = pU->ClkPara0;
        pM->u32ClkPara1 = pU->ClkPara1;

        pM->bDitherEnable = pU->DitherEnable;
        pM->bInterlace = pU->bInterlace;

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
        pU->VFB = pM->u32VFB ;
        pU->VBB = pM->u32VBB ;
        pU->VACT = pM->u32VACT ;

        pU->HFB = pM->u32HFB  ;
        pU->HBB = pM->u32HBB ;
        pU->HACT = pM->u32HACT;

        pU->VPW = pM->u32VPW ;
        pU->HPW = pM->u32HPW;
        pU->IDV = pM->bIDV;
        pU->IHS = pM->bIHS ;
        pU->IVS = pM->bIVS ;

        pU->ClockReversal = pM->bClkReversal ;
        pU->DataWidth = (HI_UNF_DISP_INTF_DATA_WIDTH_E)pM->u32DataWidth ;
        pU->ClkPara0 = pM->u32ClkPara0  ;
        pU->ClkPara1 = pM->u32ClkPara1 ;


        pU->DitherEnable = pM->bDitherEnable ;
        pU->bInterlace = pM->bInterlace ;
        pU->PixFreq = pM->u32PixFreq  ;
        pU->VertFreq = pM->u32VertFreq ;
        pU->AspectRatioW = pM->u32AspectRatioW  ;
        pU->AspectRatioH = pM->u32AspectRatioH ;

        pU->bUseGamma = pM->u32bUseGamma  ;
        pU->Reserve0 = pM->u32Reserve0 ;
        pU->Reserve1 = pM->u32Reserve1 ;
    }

    return HI_SUCCESS;
}


#define DISP_VERSION_HI3716CV200_MPW 0x20130417ul

HI_S32 DispGetInitParam(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INIT_PARAM_S* pstSetting)
{
    HI_DISP_PARAM_S stDispParam0, stDispParam1, *pstDispParam;
    HI_DRV_DISP_FMT_E  enFormat = HI_DRV_DISP_FMT_BUTT;
    #ifndef __DISP_PLATFORM_BOOT__    
    PDM_EXPORT_FUNC_S* pst_PDMFunc;
    #endif	
    HI_S32 i, j, nRet;
		
	DISP_HAL_ENCFMT_PARAM_S  stHDFmtPara;
	DISP_HAL_ENCFMT_PARAM_S  stSDFmtPara;
    memset((void*)&stDispParam0, 0, sizeof(HI_DISP_PARAM_S));
    memset((void*)&stDispParam1, 0, sizeof(HI_DISP_PARAM_S));

    if (enDisp > HI_DRV_DISPLAY_1)
    {
        return HI_FAILURE;
    }
    else
    {
         #ifndef __DISP_PLATFORM_BOOT__
        // get PDM data
        nRet = HI_DRV_MODULE_GetFunction(HI_ID_PDM, (HI_VOID**)&pst_PDMFunc);
        if (nRet || !pst_PDMFunc->pfnPDM_GetDispParam)
        {
            DISP_ERROR("DISP_get PDM funt failed!");
            return HI_FAILURE;
        }

        nRet  = pst_PDMFunc->pfnPDM_GetDispParam(HI_UNF_DISPLAY0, &stDispParam0);
        nRet |= pst_PDMFunc->pfnPDM_GetDispParam(HI_UNF_DISPLAY1, &stDispParam1);
        if (nRet)
        {
            DISP_WARN("Got no disp params, may not burned!\n");
            return HI_FAILURE;
        }

        nRet  = DISP_CheckParam(HI_UNF_DISPLAY0, &stDispParam0);
        nRet |= DISP_CheckParam(HI_UNF_DISPLAY1, &stDispParam1);
        if (nRet)
        {
            DISP_ERROR("DISP0 Param invalid!\n");
            return HI_FAILURE;
        }

        #else
		
        nRet = HI_DRV_PDM_GetDispParam(HI_UNF_DISPLAY0, &stDispParam0);
        nRet |= HI_DRV_PDM_GetDispParam(HI_UNF_DISPLAY1, &stDispParam1);
        if (nRet)
        {
            DISP_ERROR("DISP get param failed!\n");
            return HI_FAILURE;
        }

        nRet  = DISP_CheckParam(HI_UNF_DISPLAY0, &stDispParam0);
        nRet  |= DISP_CheckParam(HI_UNF_DISPLAY1, &stDispParam1);
        if (nRet)
        {
            DISP_INFO("DISP0 Param invalid!\n");
            return HI_FAILURE;
        }

        #endif
    }

    if (HI_DRV_DISPLAY_0 == enDisp)
    {
        pstDispParam = &stDispParam0;
    }
    else
    {
        pstDispParam = &stDispParam1;
    }

    pstSetting->u32Version = DISP_VERSION_HI3716CV200_MPW;

    pstSetting->enFormat   = DISP_GetEncFmt(pstDispParam->enFormat);


    if (HI_UNF_ENC_FMT_BUTT == pstDispParam->enFormat)
        ParaTransfer_Timing(&pstDispParam->stDispTiming,&pstSetting->stDispTiming,HI_TRUE);
	
    if (HI_DRV_DISPLAY_1 == (HI_DRV_DISPLAY_E)stDispParam0.enSrcDisp)
    {
        /*attach  mode*/
        if (HI_DRV_DISPLAY_0 == enDisp)
        {
            enFormat = DISP_GetEncFmt(stDispParam1.enFormat);
#ifndef HI_DISP_DOUBLE_HD_SUPPORT
            pstSetting->enFormat = DISP_Disp0FmtRevise_Attach(enFormat);
#endif

            pstSetting->bIsMaster = HI_FALSE;
            pstSetting->bIsSlave  = HI_TRUE;
            pstSetting->enAttachedDisp = HI_DRV_DISPLAY_1;

            pstSetting->u32VirtScreenWidth  = stDispParam0.u32VirtScreenWidth;
            pstSetting->u32VirtScreenHeight = stDispParam0.u32VirtScreenHeight;
            pstSetting->stOffsetInfo        = *((HI_DRV_DISP_OFFSET_S*)&stDispParam0.stOffsetInfo);
			(HI_VOID)DISP_HAL_GetEncFmtPara(DISP_GetEncFmt(stDispParam1.enFormat), &stHDFmtPara);
			(HI_VOID)DISP_HAL_GetEncFmtPara(pstSetting->enFormat, &stSDFmtPara);
			DispHD2SDOffset((HI_DRV_DISP_OFFSET_S *) &stDispParam1.stOffsetInfo,&pstSetting->stOffsetInfo,stHDFmtPara.stRefRect,stSDFmtPara.stRefRect);
        }

        if (HI_DRV_DISPLAY_1 == enDisp)
        {
            pstSetting->bIsMaster = HI_TRUE;
            pstSetting->bIsSlave  = HI_FALSE;
            pstSetting->enAttachedDisp = HI_DRV_DISPLAY_0;
            pstSetting->u32VirtScreenWidth  = stDispParam1.u32VirtScreenWidth;
            pstSetting->u32VirtScreenHeight = stDispParam1.u32VirtScreenHeight;
            pstSetting->stOffsetInfo        = *((HI_DRV_DISP_OFFSET_S*)&stDispParam1.stOffsetInfo);
        }

        pstSetting->u32Brightness = stDispParam1.u32Brightness;
        pstSetting->u32Contrast   = stDispParam1.u32Contrast;
        pstSetting->u32Saturation = stDispParam1.u32Saturation;
        pstSetting->u32HuePlus    = stDispParam1.u32HuePlus;
        pstSetting->bGammaEnable  = stDispParam1.bGammaEnable;
        pstSetting->stBgColor.u8Red     = stDispParam1.stBgColor.u8Red;
        pstSetting->stBgColor.u8Green   = stDispParam1.stBgColor.u8Green;
        pstSetting->stBgColor.u8Blue    = stDispParam1.stBgColor.u8Blue;
    }
    else
    {
        //printk("pdm no attach  mode!\n ");
        pstSetting->bIsMaster = HI_FALSE;
        pstSetting->bIsSlave  = HI_FALSE;
        pstSetting->enAttachedDisp = HI_DRV_DISPLAY_BUTT;

        pstSetting->u32Brightness = pstDispParam->u32Brightness;
        pstSetting->u32Contrast   = pstDispParam->u32Contrast;
        pstSetting->u32Saturation = pstDispParam->u32Saturation;
        pstSetting->u32HuePlus    = pstDispParam->u32HuePlus;
        pstSetting->bGammaEnable  = pstDispParam->bGammaEnable;
        pstSetting->u32VirtScreenWidth  = pstDispParam->u32VirtScreenWidth;
        pstSetting->u32VirtScreenHeight = pstDispParam->u32VirtScreenHeight;
        pstSetting->stOffsetInfo        = *((HI_DRV_DISP_OFFSET_S*)&pstDispParam->stOffsetInfo);
        pstSetting->stBgColor.u8Red     = pstDispParam->stBgColor.u8Red;
        pstSetting->stBgColor.u8Green   = pstDispParam->stBgColor.u8Green;
        pstSetting->stBgColor.u8Blue    = pstDispParam->stBgColor.u8Blue;
    }

    switch (pstDispParam->stAspectRatio.enDispAspectRatio)
    {
        case HI_UNF_DISP_ASPECT_RATIO_4TO3:
            pstSetting->bCustomRatio = HI_TRUE;
            pstSetting->u32CustomRatioWidth  = 4;
            pstSetting->u32CustomRatioHeight = 3;
            break;
        case HI_UNF_DISP_ASPECT_RATIO_16TO9:
            pstSetting->bCustomRatio = HI_TRUE;
            pstSetting->u32CustomRatioWidth  = 16;
            pstSetting->u32CustomRatioHeight = 9;
            break;
        case HI_UNF_DISP_ASPECT_RATIO_221TO1:
            pstSetting->bCustomRatio = HI_TRUE;
            pstSetting->u32CustomRatioWidth  = 221;
            pstSetting->u32CustomRatioHeight = 100;
            break;
        case HI_UNF_DISP_ASPECT_RATIO_USER:
            pstSetting->bCustomRatio = HI_TRUE;
            pstSetting->u32CustomRatioWidth  = pstDispParam->stAspectRatio.u32UserAspectWidth;
            pstSetting->u32CustomRatioHeight = pstDispParam->stAspectRatio.u32UserAspectHeight;
            break;
        case HI_UNF_DISP_ASPECT_RATIO_AUTO:
        default:
            pstSetting->bCustomRatio = HI_FALSE;
            pstSetting->u32CustomRatioWidth  = 0;
            pstSetting->u32CustomRatioHeight = 0;
            break;
    }

    for (i = 0, j = 0; i < HI_UNF_DISP_INTF_TYPE_BUTT; i++)
    {
        //printk(" i=%d,  intf type=%d\n", i, pstDispParam->stIntf[i].enIntfType);
        if (pstDispParam->stIntf[i].enIntfType < HI_UNF_DISP_INTF_TYPE_BUTT)
        {
            DISP_GetDrvIntf(&(pstDispParam->stIntf[i]), &pstSetting->stIntf[j], HI_TRUE);
            j++;
            //printk("================= i=%d,j=%d\n", i, j);
        }
    }

    for (; j < HI_DRV_DISP_INTF_ID_MAX; j++)
    {
        pstSetting->stIntf[j].eID = HI_DRV_DISP_INTF_ID_MAX;
    }


    //pstSetting->stDispTiming;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
