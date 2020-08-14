/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ui_display.c
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "com_debug.h"
#include "ui_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define UI_D_DISPLAY_TIMING_VFB             (27)
#define UI_D_DISPLAY_TIMING_VBB             (23)
#define UI_D_DISPLAY_TIMING_VACT            (768)
#define UI_D_DISPLAY_TIMING_HFB             (210)
#define UI_D_DISPLAY_TIMING_HBB             (46)
#define UI_D_DISPLAY_TIMING_HACT            (1366)
#define UI_D_DISPLAY_TIMING_VPW             (4)
#define UI_D_DISPLAY_TIMING_HPW             (24)
#define UI_D_DISPLAY_TIMING_IDV             (HI_FALSE)
#define UI_D_DISPLAY_TIMING_IHS             (HI_FALSE)
#define UI_D_DISPLAY_TIMING_IVS             (HI_FALSE)
#define UI_D_DISPLAY_TIMING_CLOCK_REVERSAL  (HI_FALSE)
#define UI_D_DISPLAY_TIMING_DATA_WIDTH      (HI_UNF_DISP_INTF_DATA_WIDTH24)
#define UI_D_DISPLAY_TIMING_INTF_DATA_FMT   (HI_UNF_DISP_INTF_DATA_FMT_RGB888)
#define UI_D_DISPLAY_TIMING_DITHER_ENABLE   (HI_FALSE)
#define UI_D_DISPLAY_TIMING_CLOCK_PARAM0    (0x912ccccc)
#define UI_D_DISPLAY_TIMING_CLOCK_PARAM1    (0x006d8157)

#define UI_D_DISPLAY_PARAM_SCREEN_WIDTH     (1280)
#define UI_D_DISPLAY_PARAM_SCREEN_HEIGHT    (720)

#define UI_D_DISPLAY_ENC_FMT                (HI_UNF_ENC_FMT_720P_50)
#define UI_D_DISPLAY_PIX_FMT                (HIGO_PF_8888)
#define UI_D_DISPLAY_HUEPLUS                (50)
#define UI_D_DISPLAY_SATURATION             (50)
#define UI_D_DISPLAY_CONTRAST               (50)
#define UI_D_DISPLAY_BRIGHTNESS             (50)

#define UI_D_DISPLAY_BGCOLOR_R              (0x00)
#define UI_D_DISPLAY_BGCOLOR_G              (0x00)
#define UI_D_DISPLAY_BGCOLOR_B              (0x00)


static HI_VOID driverDispGetDefaultParam(HI_UNF_PDM_DISP_PARAM_S* pstDispParam)
{
    HI_S32 i = 0;

    pstDispParam->enFormat              = UI_D_DISPLAY_ENC_FMT;
    pstDispParam->enPixelFormat         = UI_D_DISPLAY_PIX_FMT;
    pstDispParam->u32HuePlus            = UI_D_DISPLAY_HUEPLUS;
    pstDispParam->u32Saturation         = UI_D_DISPLAY_SATURATION;
    pstDispParam->u32Contrast           = UI_D_DISPLAY_CONTRAST;
    pstDispParam->u32Brightness         = UI_D_DISPLAY_BRIGHTNESS;

    pstDispParam->stBgColor.u8Red       = UI_D_DISPLAY_BGCOLOR_R;
    pstDispParam->stBgColor.u8Green     = UI_D_DISPLAY_BGCOLOR_G;
    pstDispParam->stBgColor.u8Blue      = UI_D_DISPLAY_BGCOLOR_B;

    pstDispParam->u32VirtScreenWidth    = UI_D_DISPLAY_PARAM_SCREEN_WIDTH;
    pstDispParam->u32VirtScreenHeight   = UI_D_DISPLAY_PARAM_SCREEN_HEIGHT;

    for (i = 0; i < HI_UNF_DISP_INTF_TYPE_BUTT; i++)
    {
        pstDispParam->stIntf[i].enIntfType = HI_UNF_DISP_INTF_TYPE_BUTT;
    }

    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_HDMI].enIntfType = HI_UNF_DISP_INTF_TYPE_HDMI;
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_HDMI].unIntf.enHdmi = HI_UNF_HDMI_ID_0;

#if defined(HI_DAC_YPBPR_Y) && defined(HI_DAC_YPBPR_PB) && defined(HI_DAC_YPBPR_PR)
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].enIntfType = HI_UNF_DISP_INTF_TYPE_YPBPR;
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].unIntf.stYPbPr.u8DacY  = HI_DAC_YPBPR_Y;
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].unIntf.stYPbPr.u8DacPb = HI_DAC_YPBPR_PB;
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].unIntf.stYPbPr.u8DacPr = HI_DAC_YPBPR_PR;
#endif

#ifdef HI_DAC_CVBS
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_CVBS].enIntfType = HI_UNF_DISP_INTF_TYPE_CVBS;
    pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_CVBS].unIntf.stCVBS.u8Dac = HI_DAC_CVBS;
#endif

    pstDispParam->stDispTiming.VFB           = UI_D_DISPLAY_TIMING_VFB;
    pstDispParam->stDispTiming.VBB           = UI_D_DISPLAY_TIMING_VBB;
    pstDispParam->stDispTiming.VACT          = UI_D_DISPLAY_TIMING_VACT;
    pstDispParam->stDispTiming.HFB           = UI_D_DISPLAY_TIMING_HFB;
    pstDispParam->stDispTiming.HBB           = UI_D_DISPLAY_TIMING_HBB;
    pstDispParam->stDispTiming.HACT          = UI_D_DISPLAY_TIMING_HACT;
    pstDispParam->stDispTiming.VPW           = UI_D_DISPLAY_TIMING_VPW;
    pstDispParam->stDispTiming.HPW           = UI_D_DISPLAY_TIMING_HPW;
    pstDispParam->stDispTiming.IDV           = UI_D_DISPLAY_TIMING_IDV;
    pstDispParam->stDispTiming.IHS           = UI_D_DISPLAY_TIMING_IHS;
    pstDispParam->stDispTiming.IVS           = UI_D_DISPLAY_TIMING_IVS;
    pstDispParam->stDispTiming.ClockReversal = UI_D_DISPLAY_TIMING_CLOCK_REVERSAL;
    pstDispParam->stDispTiming.DataWidth     = UI_D_DISPLAY_TIMING_DATA_WIDTH;
    pstDispParam->stDispTiming.ItfFormat     = UI_D_DISPLAY_TIMING_INTF_DATA_FMT;
    pstDispParam->stDispTiming.DitherEnable  = UI_D_DISPLAY_TIMING_DITHER_ENABLE;
    pstDispParam->stDispTiming.ClkPara0      = UI_D_DISPLAY_TIMING_CLOCK_PARAM0;
    pstDispParam->stDispTiming.ClkPara1      = UI_D_DISPLAY_TIMING_CLOCK_PARAM1;

    return;
}

HI_S32 UI_GetDisplayParam(HI_UNF_PDM_DISP_PARAM_S* pstDispParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    COM_CHECK_PARAM(HI_NULL_PTR == pstDispParam);

    s32Ret = HI_UNF_PDM_GetBaseParam(HI_UNF_PDM_BASEPARAM_DISP1, pstDispParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_PDM_GetBaseParam, s32Ret);
        driverDispGetDefaultParam(pstDispParam);
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


