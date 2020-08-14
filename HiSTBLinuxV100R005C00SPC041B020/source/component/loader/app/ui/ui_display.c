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

#include "hi_unf_disp.h"
#include "com_debug.h"
#include "com_osal.h"
#include "ui_display.h"

#ifdef HI_LOADER_APPLOADER
#include "ui_hdmi.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


static HI_UNF_ENC_FMT_E displayGetDispFormat(HI_UNF_ENC_FMT_E enHdFmt)
{
    if ((HI_UNF_ENC_FMT_1080P_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_1080i_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_720P_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_480P_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_NTSC == enHdFmt)
        || (HI_UNF_ENC_FMT_861D_640X480_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_800X600_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1024X768_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1280X720_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1280X800_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1280X1024_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1360X768_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1366X768_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1400X1050_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1440X900_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1440X900_60_RB == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1600X900_60_RB == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1600X1200_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1680X1050_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1920X1080_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_1920X1200_60 == enHdFmt)
        || (HI_UNF_ENC_FMT_VESA_2048X1152_60 == enHdFmt))
    {
        return HI_UNF_ENC_FMT_NTSC;
    }

    return HI_UNF_ENC_FMT_PAL;
}

HI_S32 UI_DisplayInit(HI_UNF_PDM_DISP_PARAM_S* pstDispParam)
{
    HI_UNF_ENC_FMT_E enHDFormat = HI_UNF_ENC_FMT_BUTT;
    HI_UNF_ENC_FMT_E enSDFormat = HI_UNF_ENC_FMT_BUTT;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstDispParam);

    s32Ret = HI_UNF_DISP_Init();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_DISP_Init, s32Ret);
        return COM_ERR_DEVICE;
    }

    /* Get hd and sd format*/
    enHDFormat = (HI_UNF_ENC_FMT_BUTT <= pstDispParam->enFormat) ? HI_UNF_ENC_FMT_1080i_50 : pstDispParam->enFormat;
    enSDFormat = displayGetDispFormat(enHDFormat);

    /* Set hd display param */
    COM_CHECK(HI_UNF_DISP_SetFormat(HI_UNF_DISPLAY1,     enHDFormat));
#if defined(HI_DAC_YPBPR_Y) && defined(HI_DAC_YPBPR_PB) && defined(HI_DAC_YPBPR_PR)
    COM_CHECK(HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY1,  &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR]), 1));
#else
    COM_CHECK(HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY1,  &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_HDMI]), 1));
#endif
    COM_CHECK(HI_UNF_DISP_SetBgColor(HI_UNF_DISPLAY1,  &(pstDispParam->stBgColor)));
    COM_CHECK(HI_UNF_DISP_SetBrightness(HI_UNF_DISPLAY1, pstDispParam->u32Brightness));
    COM_CHECK(HI_UNF_DISP_SetContrast(HI_UNF_DISPLAY1,   pstDispParam->u32Contrast));
    COM_CHECK(HI_UNF_DISP_SetSaturation(HI_UNF_DISPLAY1, pstDispParam->u32Saturation));
    COM_CHECK(HI_UNF_DISP_SetHuePlus(HI_UNF_DISPLAY1,    pstDispParam->u32HuePlus));

    /* Set sd display param */
    COM_CHECK(HI_UNF_DISP_SetFormat(HI_UNF_DISPLAY0,     enSDFormat));
#ifdef HI_DAC_CVBS
    COM_CHECK(HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY0,  &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_CVBS]), 1));
#else
    COM_CHECK(HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY1,  &(pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_HDMI]), 1));
#endif
    COM_CHECK(HI_UNF_DISP_SetBgColor(HI_UNF_DISPLAY0,  &(pstDispParam->stBgColor)));
    COM_CHECK(HI_UNF_DISP_SetBrightness(HI_UNF_DISPLAY0, pstDispParam->u32Brightness));
    COM_CHECK(HI_UNF_DISP_SetContrast(HI_UNF_DISPLAY0,   pstDispParam->u32Contrast));
    COM_CHECK(HI_UNF_DISP_SetSaturation(HI_UNF_DISPLAY0, pstDispParam->u32Saturation));
    COM_CHECK(HI_UNF_DISP_SetHuePlus(HI_UNF_DISPLAY0,    pstDispParam->u32HuePlus));

    s32Ret = HI_UNF_DISP_Attach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_DISP_Attach, s32Ret);
        HI_UNF_DISP_DeInit();
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_DISP_Open(HI_UNF_DISPLAY1);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_DISP_Open, s32Ret);

        HI_UNF_DISP_DeInit();
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_DISP_Open(HI_UNF_DISPLAY0);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_DISP_Open, s32Ret);

        HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return COM_ERR_DEVICE;
    }

#ifdef HI_LOADER_APPLOADER
    s32Ret = UI_HDMI_Init(HI_UNF_HDMI_ID_0, enHDFormat);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_HDMI_DeInit, s32Ret);

        HI_UNF_DISP_Close(HI_UNF_DISPLAY0);
        HI_UNF_DISP_Close(HI_UNF_DISPLAY1);

        HI_UNF_DISP_DeInit();

        return COM_ERR_DEVICE;
    }

#endif
    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_DisplayDeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

#ifdef HI_LOADER_APPLOADER
    COM_CHECK(UI_HDMI_DeInit(HI_UNF_HDMI_ID_0));
#endif
    COM_CHECK(HI_UNF_DISP_Close(HI_UNF_DISPLAY0));
    COM_CHECK(HI_UNF_DISP_Close(HI_UNF_DISPLAY1));

    COM_CHECK(HI_UNF_DISP_DeInit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


