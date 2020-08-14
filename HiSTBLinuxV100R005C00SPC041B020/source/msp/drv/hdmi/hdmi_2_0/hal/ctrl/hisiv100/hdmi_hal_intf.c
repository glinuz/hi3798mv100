/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_intf.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/

#include "hdmi_platform.h"
#include "hdmi_hal_phy.h"
#include "hdmi_hal_intf.h"


#include "hdmi_hal.h"
#include "hdmi_reg_mcu.h"
#include "hdmi_product_define.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_intf.h"
#include "hdmi_hal_hdcp14.h"
#include "hdmi_hal_hdcp22.h"
#endif
#if defined(CONFIG_HDMI_STB_SDK)
#include "hi_drv_disp.h"
#endif

#ifdef HDMI_BUILD_IN_BOOT
#include "boot_hdmi_intf.h"

#ifndef HI_MINIBOOT_SUPPORT
#include <uboot.h>
#else
#include "stdio.h"
#endif

#endif


typedef HI_VOID* HDMI_HANDLE;

#if 0

typedef enum{
    HDMI_DEBUG_CMD_TEST,
    HDMI_DEBUG_CMD_COLOR_BAR,
    HDMI_DEBUG_CMD_SW_RESET,
    HDMI_DEBUG_CMD_RGB2YUV,
    HDMI_DEBUG_CMD_YUV2RGB,
    HDMI_DEBUG_CMD_DITHER,
    HDMI_DEBUG_CMD_BYPASS,
    HDMI_DEBUG_CMD_DDC_FREQ,
    HDMI_DEBUG_CMD_PHY_DEFAULT_GET,
    HDMI_DEBUG_CMD_PHY_PARA_SET,
    HDMI_DEBUG_CMD_DUMP,
}HDMI_HAL_DEBUG_CMD_E;

#endif





#define INTF_NULL_CHK(p) do{\
	if(HI_NULL == p)\
	{HDMI_ERR("%s is null pointer!return fail.\n",#p);\
	return HI_FAILURE;}\
}while(0)

#define INTF_NULL_CHK_NO_RET(p) do{\
    if(HI_NULL == p)\
    {HDMI_ERR("%s is null pointer!return fail.\n",#p);\
    return ;}\
}while(0)


#define INTF_INIT_CHK(pstHalCfg) do{\
    INTF_NULL_CHK(pstHalCfg);\
    if(HI_TRUE != pstHalCfg->bInit)\
    {HDMI_ERR("hal intf module didn't init!return fail.\n");\
    return HI_FAILURE;}\
}while(0)

#define INTF_INIT_CHK_NO_RET(pstHalCfg) do{\
    INTF_NULL_CHK_NO_RET(pstHalCfg);\
    if(HI_TRUE != pstHalCfg->bInit)\
    {HDMI_ERR("hal intf module didn't init!return fail.\n");\
    return ;}\
}while(0)



static HDMI_HAL_CFG_S s_stHalCfg[HDMI_DEVICE_ID_BUTT] ;



static HDMI_HAL_CFG_S *IntfHalInfoPtrGet(HDMI_DEVICE_ID_E enHdmi)
{
    if (enHdmi < HDMI_DEVICE_ID_BUTT)
    {
        return &s_stHalCfg[enHdmi];
    }
    return HI_NULL;
}

#ifndef HDMI_BUILD_IN_BOOT
static HI_VOID HalHdmiCBarEnable(HI_BOOL bEnable)
{
    HI_U32               *pu32RegAddr = HI_NULL;
    HI_U32               u32RegValue = 0;


    pu32RegAddr = (HI_U32 *)HDMI_IO_MAP(HDMI_COLOR_BAR_BASE, 4);
    u32RegValue = HdmiRegRead(pu32RegAddr);

    if (bEnable)
    {
        u32RegValue |= HDMI_COLOR_BAR_MASK;
        u32RegValue |=HDMI_COLOR_BAR_UPDATE_MASK;
    }
    else
    {
        u32RegValue &= ~HDMI_COLOR_BAR_MASK;
        u32RegValue |=HDMI_COLOR_BAR_UPDATE_MASK;
    }
    HdmiRegWrite(pu32RegAddr, u32RegValue);
    HDMI_IO_UNMAP(pu32RegAddr);

}

#endif

static HI_S32 IntfTxCapabilityInit(HDMI_DEVICE_ID_E enHdmiId)
{
    HDMI_HAL_CFG_S *pstHalCfg = IntfHalInfoPtrGet(enHdmiId);

    INTF_NULL_CHK(pstHalCfg);
    //INTF_INIT_CHK(pstHalCfg);

    /* Tx Capability */
    pstHalCfg->stTxCapability.bTxHdmi_14      = HI_TRUE;
    pstHalCfg->stTxCapability.bTxHdmi_20      = HI_TRUE;

    pstHalCfg->stTxCapability.bTxHdcp_14      = HI_TRUE;
    pstHalCfg->stTxCapability.bTxHdcp_22      = HI_TRUE;

    pstHalCfg->stTxCapability.bTxRGB444       = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR444     = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR422     = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR420     = HI_TRUE;

    pstHalCfg->stTxCapability.bTxDeepClr10Bit = HI_TRUE;
    pstHalCfg->stTxCapability.bTxDeepClr12Bit = HI_TRUE;
    pstHalCfg->stTxCapability.bTxDeepClr16Bit = HI_FALSE;

    pstHalCfg->stTxCapability.bTxRGB_YCbCR444 = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR444_422 = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR422_420 = HI_TRUE;

    pstHalCfg->stTxCapability.bTxYCBCR420_422 = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR422_444 = HI_TRUE;
    pstHalCfg->stTxCapability.bTxYCBCR444_RGB = HI_TRUE;

    pstHalCfg->stTxCapability.bTxScdc         = HI_TRUE;
    pstHalCfg->stTxCapability.u32TxMaxTmdsClk = 600; /*in MHz*/

    return HI_SUCCESS;
 }

HI_VOID HAL_HDMI_HardwareInit(struct hiHDMI_HAL_S *pstHdmiHal)
{
    DRV_HDMI_ProdCrgInit();
    return ;
}
//HI_S32*HAL_HDMI_HpdRsenGet)(struct hiHDMI_HAL_S *pstHdmiHal,HDMI_HPD_RSEN_S *pstHpdRsen);

HI_VOID HAL_HDMI_TmdsModeSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_TMDS_MODE_E enTmdsMode)
{
    HAL_HDMI_CtrlTmdsModeSet(pstHdmiHal->stHalCtx.u32HdmiID,enTmdsMode);
    return ;
}
HI_VOID HAL_HDMI_AvMuteSet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bAvMute)
{
    HAL_HDMI_CtrlAvmuteSet(pstHdmiHal->stHalCtx.u32HdmiID,bAvMute);

    return ;
}

HI_VOID HAL_HDMI_EmiSet(HDMI_HAL_S *pstHdmiHal, HDMI_EMI_CONFIG_S* pstEmiCfg)
{

	HDMI_WARN("no support emi intf now !\n");

 	return ;
}

HI_VOID HAL_HDMI_EmiStatusGet(HDMI_HAL_S *pstHdmiHal, HDMI_EMI_STATUS_S *pstEmiStatus)
{
    HDMI_PHY_INFO_S stPhyInfo = {0};

    HAL_HDMI_PhyInfoGet(&stPhyInfo);

    pstEmiStatus->bSWEmiEnable  = stPhyInfo.stSscCfg.bSscEnable;
    pstEmiStatus->bDebugEnable  = stPhyInfo.stSscCfg.bSscDebugEn;
    HAL_HDMI_PhySscGet(&pstEmiStatus->bHWEmiEnable);

	return ;

}

HI_VOID HAL_HDMI_CscParamSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_VIDEO_CONFIG_S* pstVideoCfg)
{
	HDMI_WARN("no support Csc Parma set now !\n");
    return ;
}


HI_VOID HAL_HDMI_InfoframeSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_U8 *pu8InBuffer)
{
    HAL_HDMI_CtrlInfoFrameDataSet(pstHdmiHal->stHalCtx.u32HdmiID,enInfoFrameId,pu8InBuffer);
    return ;
}



HI_VOID HAL_HDMI_InfoframeEnableSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_BOOL bEnable)
{
    HAL_HDMI_CtrlInfoFrameEnSet(pstHdmiHal->stHalCtx.u32HdmiID,enInfoFrameId,bEnable);
    return ;
}

HI_S32  HAL_HDMI_VideoPathSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_VIDEO_CONFIG_S* pstVideoCfg)
{
    HI_S32                  s32Ret = HI_SUCCESS;
    HDMI_PHY_TMDS_CFG_S     stPhyTmds = {0};
    HDMI_VIDEO_PATH_S       stVideoPath = {0};
    HDMI_HAL_CFG_S          *pstHalCfg = HI_NULL;


    INTF_NULL_CHK(pstHdmiHal);
    INTF_NULL_CHK(pstVideoCfg);
    pstHalCfg = IntfHalInfoPtrGet(pstHdmiHal->stHalCtx.u32HdmiID);
    INTF_NULL_CHK(pstHalCfg);
    pstHalCfg->u32TmdsClk = pstVideoCfg->u32TmdsClk;

    HDMI_MEMSET(&stPhyTmds, 0, sizeof(HDMI_PHY_TMDS_CFG_S));

    stVideoPath.enInColorSpace              = pstVideoCfg->enInColorSpace;
    stVideoPath.enOutColorSpace             = pstVideoCfg->enOutColorSpace;

    stVideoPath.enInDeepColor               = pstVideoCfg->enInBitDepth;
    stVideoPath.enOutDeepColor              = pstVideoCfg->enDeepColor;

    stVideoPath.stOutHvSyncPol.bDeInver     = pstVideoCfg->bDEPol;
    stVideoPath.stOutHvSyncPol.bHPolInver   = pstVideoCfg->bHSyncPol;
    stVideoPath.stOutHvSyncPol.bVPolInver   = pstVideoCfg->bVSyncPol;

    switch (pstVideoCfg->enConvStd)
    {
        case HDMI_CONV_STD_BT_601:
            stVideoPath.enOutColormetry = HDMI_COLORMETRY_BT601;
            break;
        case HDMI_CONV_STD_BT_709:
            stVideoPath.enOutColormetry = HDMI_COLORMETRY_BT709;
            break;
        case HDMI_CONV_STD_BT_2020_non_const_luminous:
            stVideoPath.enOutColormetry = HDMI_COLORMETRY_BT2020;
            break;
        case HDMI_CONV_STD_BT_2020_const_luminous:
            stVideoPath.enOutColormetry = HDMI_COLORMETRY_BT2020_CONST;
            break;

        default:
            stVideoPath.enOutColormetry = HDMI_COLORMETRY_BT709;
            break;

    }
    stVideoPath.enInColormetry = stVideoPath.enOutColormetry;


    stVideoPath.enInQuantization    = pstVideoCfg->enQuantization;//should set according to the sink;should sync 98cv200
    stVideoPath.enOutQuantization   = pstVideoCfg->enQuantization;
    stVideoPath.u32OutTmdsClk       = pstVideoCfg->u32TmdsClk;  //video path no use
    s32Ret |= HAL_HDMI_CtrlVideoPathSet(pstHdmiHal->stHalCtx.u32HdmiID,&stVideoPath);

    /* PHY config */
    stPhyTmds.bEmiEnable    = pstVideoCfg->bEmiEnable;
    stPhyTmds.enDeepColor   = pstVideoCfg->enDeepColor;
    stPhyTmds.u32PixelClk   = pstVideoCfg->u32PixelClk;
    stPhyTmds.u32TmdsClk    = pstVideoCfg->u32TmdsClk;
    s32Ret |= HAL_HDMI_PhyTmdsSet(&stPhyTmds);

    return s32Ret;
}

HI_VOID HAL_HDMI_PhyOutputEnableSet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable)
{
    HAL_HDMI_PhyOeSet(bEnable);
    return ;
}

HI_VOID HAL_HDMI_PhyPowerEnableSet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable)
{
    HI_BOOL bOldEable = HI_FALSE;

    HAL_HDMI_PhyPowerGet(&bOldEable);
    if(bEnable != bOldEable)
    {
        HAL_HDMI_PhyPowerSet(bEnable);
    }
    return ;
}

HI_VOID HAL_HDMI_TxCapabilityGet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_TX_CAPABILITY_S *pstTxCapability)
{
    HDMI_HAL_CFG_S *pstHalCfg = HI_NULL;

    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    INTF_NULL_CHK_NO_RET(pstTxCapability);
    pstHalCfg = IntfHalInfoPtrGet(pstHdmiHal->stHalCtx.u32HdmiID);
    INTF_NULL_CHK_NO_RET(pstHalCfg);
    INTF_INIT_CHK_NO_RET(pstHalCfg);
    HDMI_MEMCPY(pstTxCapability,&pstHalCfg->stTxCapability,sizeof(HDMI_TX_CAPABILITY_S));
    return ;
}

#ifndef HDMI_BUILD_IN_BOOT
#if defined(CONFIG_HDMI_STB_SDK)
static HI_DRV_DISP_FMT_E DISP_TVHDFmtU2V(HI_UNF_ENC_FMT_E U)
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

static HI_DRV_DISP_FMT_E DISP_TVSDFmtU2V(HI_UNF_ENC_FMT_E U)
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

static HI_DRV_DISP_FMT_E DISP_3DFmtU2V(HI_UNF_ENC_FMT_E U)
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

static HI_DRV_DISP_FMT_E DISP_4KFmtU2V(HI_UNF_ENC_FMT_E U)
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

static HI_DRV_DISP_FMT_E DISP_DecimalFmtU2V(HI_UNF_ENC_FMT_E U)
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


static HI_DRV_DISP_FMT_E DISP_DVIFmtU2V(HI_UNF_ENC_FMT_E U)
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

static HI_DRV_DISP_FMT_E DISP_GetEncFmt(HI_UNF_ENC_FMT_E enUnFmt)
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

#endif
#endif




HDMI_VIDEO_TIMING_E HAL_HDMI_DispFmt2HdmiTiming(HDMI_HAL_S *pstHdmiHal, HI_U32 u32DispFmt)
{
    HDMI_VIDEO_TIMING_E VideoTimingMode;
    switch (u32DispFmt)
    {
        case HI_DRV_DISP_FMT_1080P_60:
        case HI_DRV_DISP_FMT_1080P_59_94:
            VideoTimingMode = HDMI_VIDEO_TIMING_1920X1080P_60000;
            break;
        case HI_DRV_DISP_FMT_1080P_50:
            VideoTimingMode = HDMI_VIDEO_TIMING_1920X1080P_50000;
            break;
        case HI_DRV_DISP_FMT_1080P_30:
        case HI_DRV_DISP_FMT_1080P_29_97:
            VideoTimingMode = HDMI_VIDEO_TIMING_1920X1080P_30000;
            break;
        case HI_DRV_DISP_FMT_1080P_25:
            VideoTimingMode = HDMI_VIDEO_TIMING_1920X1080P_25000;
            break;
        case HI_DRV_DISP_FMT_1080P_24:
        case HI_DRV_DISP_FMT_1080P_23_976:
        case HI_DRV_DISP_FMT_1080P_24_FP:
            VideoTimingMode = HDMI_VIDEO_TIMING_1920X1080P_24000;
            break;
        case HI_DRV_DISP_FMT_1080i_60:
        case HI_DRV_DISP_FMT_1080i_59_94:
            VideoTimingMode = HDMI_VIDEO_TIMING_1920X1080I_60000;
            break;
        case HI_DRV_DISP_FMT_1080i_50:
            VideoTimingMode = HDMI_VIDEO_TIMING_1920X1080I_50000;
            break;
        case HI_DRV_DISP_FMT_720P_60:
        case HI_DRV_DISP_FMT_720P_59_94:
        case HI_DRV_DISP_FMT_720P_60_FP:
            VideoTimingMode = HDMI_VIDEO_TIMING_1280X720P_60000;
            break;
        case HI_DRV_DISP_FMT_720P_50:
        case HI_DRV_DISP_FMT_720P_50_FP:
            VideoTimingMode = HDMI_VIDEO_TIMING_1280X720P_50000;
            break;
        case HI_DRV_DISP_FMT_576P_50:
            VideoTimingMode = HDMI_VIDEO_TIMING_720X576P_50000;
            break;
        case HI_DRV_DISP_FMT_480P_60:
            VideoTimingMode = HDMI_VIDEO_TIMING_720X480P_60000;
            break;
        case HI_DRV_DISP_FMT_3840X2160_24:
        case HI_DRV_DISP_FMT_3840X2160_23_976:
            VideoTimingMode = HDMI_VIDEO_TIMING_3840X2160P_24000;
            break;
        case HI_DRV_DISP_FMT_3840X2160_25:
            VideoTimingMode = HDMI_VIDEO_TIMING_3840X2160P_25000;
            break;
        case HI_DRV_DISP_FMT_3840X2160_30:
        case HI_DRV_DISP_FMT_3840X2160_29_97:
            VideoTimingMode = HDMI_VIDEO_TIMING_3840X2160P_30000;
            break;
        case HI_DRV_DISP_FMT_4096X2160_24:
            VideoTimingMode = HDMI_VIDEO_TIMING_4096X2160P_24000;
            break;
        case HI_DRV_DISP_FMT_4096X2160_25:
            VideoTimingMode = HDMI_VIDEO_TIMING_4096X2160P_25000;
            break;
        case HI_DRV_DISP_FMT_4096X2160_30:
            VideoTimingMode = HDMI_VIDEO_TIMING_4096X2160P_30000;
            break;
        case HI_DRV_DISP_FMT_3840X2160_50:
            VideoTimingMode = HDMI_VIDEO_TIMING_3840X2160P_50000;
            break;
        case HI_DRV_DISP_FMT_3840X2160_60:
            VideoTimingMode = HDMI_VIDEO_TIMING_3840X2160P_60000;
            break;
        case HI_DRV_DISP_FMT_4096X2160_50:
            VideoTimingMode = HDMI_VIDEO_TIMING_4096X2160P_50000;
            break;
        case HI_DRV_DISP_FMT_4096X2160_60:
            VideoTimingMode = HDMI_VIDEO_TIMING_4096X2160P_60000;
            break;
        case HI_DRV_DISP_FMT_1440x576i_50:
        case HI_DRV_DISP_FMT_PAL:
        case HI_DRV_DISP_FMT_PAL_B:
        case HI_DRV_DISP_FMT_PAL_B1:
        case HI_DRV_DISP_FMT_PAL_D:
        case HI_DRV_DISP_FMT_PAL_D1:
        case HI_DRV_DISP_FMT_PAL_G:
        case HI_DRV_DISP_FMT_PAL_H:
        case HI_DRV_DISP_FMT_PAL_K:
        case HI_DRV_DISP_FMT_PAL_I:
        case HI_DRV_DISP_FMT_PAL_M:
        case HI_DRV_DISP_FMT_PAL_N:
        case HI_DRV_DISP_FMT_PAL_Nc:
        case HI_DRV_DISP_FMT_PAL_60:

        case HI_DRV_DISP_FMT_SECAM_SIN:
        case HI_DRV_DISP_FMT_SECAM_COS:
        case HI_DRV_DISP_FMT_SECAM_L:
        case HI_DRV_DISP_FMT_SECAM_B:
        case HI_DRV_DISP_FMT_SECAM_G:
        case HI_DRV_DISP_FMT_SECAM_D:
        case HI_DRV_DISP_FMT_SECAM_K:
        case HI_DRV_DISP_FMT_SECAM_H:
            VideoTimingMode = HDMI_VIDEO_TIMING_1440X576I_50000;
            break;
        case HI_DRV_DISP_FMT_1440x480i_60:
            VideoTimingMode = HDMI_VIDEO_TIMING_1440X480I_60000;
            break;
        case HI_DRV_DISP_FMT_NTSC:
        case HI_DRV_DISP_FMT_NTSC_J:
        case HI_DRV_DISP_FMT_NTSC_443:
            VideoTimingMode = HDMI_VIDEO_TIMING_1440X480I_60000;
            break;
        case HI_DRV_DISP_FMT_861D_640X480_60:
            VideoTimingMode = HDMI_VIDEO_TIMING_640X480P_60000;
            break;
        default:
            HDMI_INFO("Not CEA video timing:%d\n", u32DispFmt);
            //  vesa
            VideoTimingMode = HDMI_VIDEO_TIMING_UNKNOWN;
            break;
    }
    return VideoTimingMode;
}


#ifndef HDMI_BUILD_IN_BOOT
HI_S32 HAL_HDMI_DispFmtGet(HDMI_HAL_S *pstHdmiHal, HDMI_HAL_BASE_PARAM_S *pstBaseParam)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_DISP_PARAM_S   stDispParam;
    PDM_EXPORT_FUNC_S   *pstPdmFuncs  = HI_NULL;

    INTF_NULL_CHK(pstHdmiHal);
    INTF_NULL_CHK(pstBaseParam);

    pstBaseParam->u32DispFmt    = HI_UNF_ENC_FMT_720P_50;
    pstBaseParam->enDeepColor   = HDMI_DEEP_COLOR_24BIT;
    pstBaseParam->enColorSpace  = HDMI_COLORSPACE_YCbCr444;
    s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_PDM, (HI_VOID**)&pstPdmFuncs);
    if (s32Ret == HI_FAILURE || NULL == pstPdmFuncs
        || NULL == pstPdmFuncs->pfnPDM_GetDispParam)
    {
        HDMI_ERR("get pdm module function failed\r\n");
        return HI_FAILURE;
    }

    s32Ret = pstPdmFuncs->pfnPDM_GetDispParam(HI_DRV_DISPLAY_1, &stDispParam);
    if (s32Ret == HI_FAILURE)
    {
        HDMI_WARN("PDM_GetDispParam failed\r\n");
        return HI_FAILURE;
    }
    pstBaseParam->u32DispFmt    = DISP_GetEncFmt(stDispParam.enFormat);
    HI_DRV_HDMI_PixClkGet(stDispParam.enFormat , &pstBaseParam->u32PixClk);

    switch(stDispParam.enDeepColorMode)
    {
        case HI_UNF_HDMI_DEEP_COLOR_30BIT:
            pstBaseParam->enDeepColor = HDMI_DEEP_COLOR_30BIT;
            break;
        case HI_UNF_HDMI_DEEP_COLOR_36BIT:
            pstBaseParam->enDeepColor = HDMI_DEEP_COLOR_36BIT;
            break;

        case HI_UNF_HDMI_DEEP_COLOR_OFF:
        case HI_UNF_HDMI_DEEP_COLOR_24BIT :
        default:
            pstBaseParam->enDeepColor = HDMI_DEEP_COLOR_24BIT;
            break;
    }

    switch(stDispParam.enVidOutMode)
    {
        case HI_UNF_HDMI_VIDEO_MODE_RGB444:
            pstBaseParam->enColorSpace = HDMI_COLORSPACE_RGB;
            break;
        case HI_UNF_HDMI_VIDEO_MODE_YCBCR422:
            pstBaseParam->enColorSpace = HDMI_COLORSPACE_YCbCr422;
            break;
        case HI_UNF_HDMI_VIDEO_MODE_YCBCR444:
            pstBaseParam->enColorSpace = HDMI_COLORSPACE_YCbCr444;
            break;
    	case HI_UNF_HDMI_VIDEO_MODE_YCBCR420:
            pstBaseParam->enColorSpace = HDMI_COLORSPACE_YCbCr420;
            break;

    	default:
    	    HDMI_ERR("stDispParam.enVidOutMode=%d\n",stDispParam.enVidOutMode);
            pstBaseParam->enColorSpace = HDMI_COLORSPACE_RGB;
	        break;
    }

    return s32Ret;
}

#ifdef HDMI_HDR_SUPPORT
HI_VOID HAL_HDMI_HdrTimerSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_TIMER_CONFIG_S *pstTimerCfg)
{
    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    INTF_NULL_CHK_NO_RET(pstTimerCfg);

    HAL_HDMI_CtrlHdrTimerSet(pstHdmiHal->stHalCtx.u32HdmiID,pstTimerCfg);

    return ;
}
#endif


HI_VOID HAL_HDMI_SequencerHandlerProcess(struct hiHDMI_HAL_S *pstHdmiHal)
{
    HAL_HDMI_MachInvoke();
    return ;
}


HI_VOID HAL_HDMI_HardwareStatusGet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_HARDWARE_STATUS_S* pstHwStatus)
{
    HDMI_HPD_RSEN_S         stHpdRsen = {0};
    CTRL_AUDIO_STATUS_S     stAudioStat = {0};
    CTRL_VIDEO_STATUS_S     stVideoStat = {0};
    HDCP14_STAUTS_S         stHdcp14Stauts  ;
    HDCP14_RUN_STAT_S       stHdcp14RunStat ;
    HDCP22_RUN_STATUS_S     stHdcp22RunStat ;
    HDCP22_AUTH_STATUS_S    stHdcp22AuthStat;

    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    INTF_NULL_CHK_NO_RET(pstHwStatus);

    /* PHY */
    HAL_HDMI_PhyOeGet(&pstHwStatus->stPhyStatus.bPhyOe);
    HAL_HDMI_PhyPowerGet(&pstHwStatus->stPhyStatus.bPhyPowerOn);

    /* info frame */
    HAL_HDMI_CtrlInfoFrameEnGet(pstHdmiHal->stHalCtx.u32HdmiID,HDMI_INFOFRAME_TYPE_AVI,&pstHwStatus->stInfoFrameStatus.bAVIEnable);
    HAL_HDMI_CtrlInfoFrameEnGet(pstHdmiHal->stHalCtx.u32HdmiID,HDMI_INFOFRAME_TYPE_AUDIO,&pstHwStatus->stInfoFrameStatus.bAUDIOEnable);
    HAL_HDMI_CtrlInfoFrameEnGet(pstHdmiHal->stHalCtx.u32HdmiID,HDMI_INFOFRAME_TYPE_VENDOR,&pstHwStatus->stInfoFrameStatus.bVSIFEnable);
    HAL_HDMI_CtrlInfoFrameEnGet(pstHdmiHal->stHalCtx.u32HdmiID,HDMI_INFOFRAME_TYPE_GBD,&pstHwStatus->stInfoFrameStatus.bGBDEnable);
    HAL_HDMI_CtrlInfoFrameEnGet(pstHdmiHal->stHalCtx.u32HdmiID,HDMI_INFOFRAME_TYPE_DRM,&pstHwStatus->stInfoFrameStatus.bDRMEnable);

    HAL_HDMI_CtrlInfoFrameDataGet(pstHdmiHal->stHalCtx.u32HdmiID,HDMI_INFOFRAME_TYPE_AVI,pstHwStatus->stInfoFrameStatus.u8AVI);
    HAL_HDMI_CtrlInfoFrameDataGet(pstHdmiHal->stHalCtx.u32HdmiID,HDMI_INFOFRAME_TYPE_AUDIO,pstHwStatus->stInfoFrameStatus.u8AUDIO);
    HAL_HDMI_CtrlInfoFrameDataGet(pstHdmiHal->stHalCtx.u32HdmiID,HDMI_INFOFRAME_TYPE_VENDOR,pstHwStatus->stInfoFrameStatus.u8VSIF);
    HAL_HDMI_CtrlInfoFrameDataGet(pstHdmiHal->stHalCtx.u32HdmiID,HDMI_INFOFRAME_TYPE_GBD,pstHwStatus->stInfoFrameStatus.u8GDB);
    HAL_HDMI_CtrlInfoFrameDataGet(pstHdmiHal->stHalCtx.u32HdmiID,HDMI_INFOFRAME_TYPE_DRM,pstHwStatus->stInfoFrameStatus.u8DRM);

    /* HPD,avmute,TmdsMode */
    HAL_HDMI_CtrlHpdRsenGet(pstHdmiHal->stHalCtx.u32HdmiID, &stHpdRsen);
    pstHwStatus->stCommonStatus.bHotPlug = stHpdRsen.bHpdOn;
    pstHwStatus->stCommonStatus.bRsen    = stHpdRsen.bRsenOn;
    HAL_HDMI_CtrlAvmuteGet(pstHdmiHal->stHalCtx.u32HdmiID, &pstHwStatus->stCommonStatus.bAvMute);
    HAL_HDMI_CtrlTmdsModeGet(pstHdmiHal->stHalCtx.u32HdmiID, &pstHwStatus->stCommonStatus.enTmdsMode);

    /* audio */
    HAL_HDMI_CtrlAudioPathGet(pstHdmiHal->stHalCtx.u32HdmiID, &stAudioStat);
    pstHwStatus->stAudioStatus.bAudioMute       = stAudioStat.bAudioMute;
    pstHwStatus->stAudioStatus.bAudioEnable     = stAudioStat.bEnableAudio;
    pstHwStatus->stAudioStatus.enSampleFs       = stAudioStat.enSampleRate;
    pstHwStatus->stAudioStatus.enLayout         = stAudioStat.enChanneNum;
    pstHwStatus->stAudioStatus.enSoundIntf      = stAudioStat.enSoundIntf;
    pstHwStatus->stAudioStatus.enSampleDepth    = stAudioStat.enSampleBit;
    pstHwStatus->stAudioStatus.bDownSample      = HI_FALSE;//not support
    pstHwStatus->stAudioStatus.u32RefCtsvalue   = stAudioStat.u32RefCtsvalue;
    pstHwStatus->stAudioStatus.u32RegCtsvalue   = stAudioStat.u32RegCtsvalue;
    pstHwStatus->stAudioStatus.u32RefNvalue     = stAudioStat.u32RefNvalue;
    pstHwStatus->stAudioStatus.u32RegNvalue     = stAudioStat.u32RegNvalue;

    /* video */
    HAL_HDMI_CtrlVideoPathGet(pstHdmiHal->stHalCtx.u32HdmiID,&stVideoStat);
    HAL_HDMI_CtrlVideoMuteGet(pstHdmiHal->stHalCtx.u32HdmiID, &pstHwStatus->stVideoStatus.bVideoMute);
    pstHwStatus->stVideoStatus.bYCbCr2RGB         = stVideoStat.bCscEnable && (stVideoStat.enOutColorSpace == HDMI_COLORSPACE_RGB);
    pstHwStatus->stVideoStatus.bRGB2YCbCr         = stVideoStat.bCscEnable && (stVideoStat.enInColorSpace == HDMI_COLORSPACE_RGB);
    pstHwStatus->stVideoStatus.bYCbCr444_422      = stVideoStat.bY422Enalbe;
    pstHwStatus->stVideoStatus.bYCbCr422_420      = stVideoStat.bY420Enalbe;
    pstHwStatus->stVideoStatus.bYCbCr420_422      = HI_FALSE;//not support
    pstHwStatus->stVideoStatus.bYCbCr422_444      = HI_FALSE;//not support
    pstHwStatus->stVideoStatus.bIn420Ydemux       = stVideoStat.bDwsmVertEnalbe;
    pstHwStatus->stVideoStatus.bOut420Ydemux      = HI_FALSE;//not support
    pstHwStatus->stVideoStatus.bDither            = (stVideoStat.enDitherMode==HDMI_VEDIO_DITHER_DISALBE) ? HI_FALSE : HI_TRUE;
    pstHwStatus->stVideoStatus.bVSyncPol          = stVideoStat.stOutHvSyncPol.bHPolInver;
    pstHwStatus->stVideoStatus.bHSyncPol          = stVideoStat.stOutHvSyncPol.bVPolInver;
    pstHwStatus->stVideoStatus.bCSyncPol          = HI_FALSE ;//not support
    pstHwStatus->stVideoStatus.bDEPol             = stVideoStat.stOutHvSyncPol.bDeInver;
    pstHwStatus->stVideoStatus.bSwapHsCs          = HI_FALSE ;//not support
    pstHwStatus->stVideoStatus.enInColorSpace     = stVideoStat.enInColorSpace;
    pstHwStatus->stVideoStatus.enOutColorSpace    = stVideoStat.enOutColorSpace;
    pstHwStatus->stVideoStatus.enOutBitDepth      = (stVideoStat.enOutDeepColor == HDMI_DEEP_COLOR_OFF) ? HDMI_VIDEO_BITDEPTH_OFF : stVideoStat.enOutDeepColor;
    //pstHwStatusstVideoStatus.->enHvSyncPol;
    pstHwStatus->stVideoStatus.bSyncSwEnable    = stVideoStat.stTimming.bSyncSwEnable;
    pstHwStatus->stVideoStatus.bVsyncPolarity   = stVideoStat.stTimming.bVsyncPolarity;
    pstHwStatus->stVideoStatus.bHsyncPolarity   = stVideoStat.stTimming.bHsyncPolarity;
    pstHwStatus->stVideoStatus.bProgressive     = stVideoStat.stTimming.bProgressive;
    pstHwStatus->stVideoStatus.u32HsyncTotal    = stVideoStat.stTimming.u32HsyncTotal;
    pstHwStatus->stVideoStatus.u32HactiveCnt    = stVideoStat.stTimming.u32HactiveCnt;
    pstHwStatus->stVideoStatus.u32VsyncTotal    = stVideoStat.stTimming.u32VsyncTotal;
    pstHwStatus->stVideoStatus.u32VactiveCnt    = stVideoStat.stTimming.u32VactiveCnt;

    HAL_HDMI_Hdcp14RunStatusGet(pstHdmiHal->stHalCtx.u32HdmiID,&stHdcp14RunStat);
    HAL_HDMI_Hdcp14StatusGet(pstHdmiHal->stHalCtx.u32HdmiID,&stHdcp14Stauts);
    HAL_HDMI_Hdcp22RunStatusGet(pstHdmiHal->stHalCtx.u32HdmiID, &stHdcp22RunStat);
    HAL_HDMI_Hdcp22AuthStatusGet(pstHdmiHal->stHalCtx.u32HdmiID, &stHdcp22AuthStat);

    pstHwStatus->stHdcpStatus.bHdcp1_4Enable = stHdcp14RunStat.bEnable;
    pstHwStatus->stHdcpStatus.bHdcp2_2Enable = stHdcp22RunStat.bEnable;
    pstHwStatus->stHdcpStatus.hdcpStatus     = 0;
    if(stHdcp14RunStat.bEnable)
    {

        pstHwStatus->stHdcpStatus.u8BKSVs[0] = stHdcp14Stauts.stPartI.au8Bksv[0];
        pstHwStatus->stHdcpStatus.u8BKSVs[1] = stHdcp14Stauts.stPartI.au8Bksv[1];
        pstHwStatus->stHdcpStatus.u8BKSVs[2] = stHdcp14Stauts.stPartI.au8Bksv[2];
        pstHwStatus->stHdcpStatus.u8BKSVs[3] = stHdcp14Stauts.stPartI.au8Bksv[3];
        pstHwStatus->stHdcpStatus.u8BKSVs[4] = stHdcp14Stauts.stPartI.au8Bksv[4];

        pstHwStatus->stHdcpStatus.u8AKSVs[0] = stHdcp14Stauts.stPartI.au8Aksv[0];
        pstHwStatus->stHdcpStatus.u8AKSVs[1] = stHdcp14Stauts.stPartI.au8Aksv[1];
        pstHwStatus->stHdcpStatus.u8AKSVs[2] = stHdcp14Stauts.stPartI.au8Aksv[2];
        pstHwStatus->stHdcpStatus.u8AKSVs[3] = stHdcp14Stauts.stPartI.au8Aksv[3];
        pstHwStatus->stHdcpStatus.u8AKSVs[4] = stHdcp14Stauts.stPartI.au8Aksv[4];

        pstHwStatus->stHdcpStatus.bRepeaterOn = stHdcp14Stauts.stPartI.unBCaps.u8.bzRepeater;

    }

    if(stHdcp22RunStat.bEnable)
    {
        pstHwStatus->stHdcpStatus.u8BKSVs[0] = stHdcp22AuthStat.u8RecvId[0];
        pstHwStatus->stHdcpStatus.u8BKSVs[1] = stHdcp22AuthStat.u8RecvId[1];
        pstHwStatus->stHdcpStatus.u8BKSVs[2] = stHdcp22AuthStat.u8RecvId[2];
        pstHwStatus->stHdcpStatus.u8BKSVs[3] = stHdcp22AuthStat.u8RecvId[3];
        pstHwStatus->stHdcpStatus.u8BKSVs[4] = stHdcp22AuthStat.u8RecvId[4];

        pstHwStatus->stHdcpStatus.u8AKSVs[0] = 0;
        pstHwStatus->stHdcpStatus.u8AKSVs[1] = 0;
        pstHwStatus->stHdcpStatus.u8AKSVs[2] = 0;
        pstHwStatus->stHdcpStatus.u8AKSVs[3] = 0;
        pstHwStatus->stHdcpStatus.u8AKSVs[4] = 0;

        pstHwStatus->stHdcpStatus.bRepeaterOn = stHdcp22AuthStat.stRxCaps.bRepeater;
    }

    return ;
}

HI_VOID HAL_HDMI_HotPlugStatusGet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL* bHotPlug)
{
    HDMI_HPD_RSEN_S stHpdRsen;

    if (bHotPlug)
    {
        HAL_HDMI_CtrlHpdRsenGet(pstHdmiHal->stHalCtx.u32HdmiID,&stHpdRsen);
        *bHotPlug = stHpdRsen.bHpdOn;
    }
    else
    {
        HDMI_ERR("ctrl bHotPlug = null!\n");
    }

    return ;
}

HI_VOID HAL_HDMI_AudioMuteSet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bAudioMute)
{
    HAL_HDMI_CtrlAudioMuteSet(pstHdmiHal->stHalCtx.u32HdmiID,bAudioMute);
    return ;
}

HI_S32 HAL_HDMI_AudioPathSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_AUDIO_CONFIG_S* pstAudioCfg)
{
    HDMI_AUDIO_PATH_S stAudioPath = {0};

    //stAudioPath.bEnableAudio    = pstAudioCfg->bEnableAudio;
    //stAudioPath.bAudioMute      = !pstAudioCfg->bEnableAudio;
    stAudioPath.enSoundIntf     = pstAudioCfg->enSoundIntf;
    stAudioPath.enSampleRate    = pstAudioCfg->enSampleFs;
    stAudioPath.enChanneNum     = pstAudioCfg->enLayout ;
    stAudioPath.enSampleBit     = pstAudioCfg->enSampleDepth;
    stAudioPath.u32PixelClk     = pstAudioCfg->u32TmdsClk;
    HAL_HDMI_CtrlAudioPathSet(pstHdmiHal->stHalCtx.u32HdmiID,&stAudioPath);
    return HI_SUCCESS;
}


HI_VOID HAL_HDMI_AudioPathEnableSet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable)
{
    INTF_NULL_CHK_NO_RET(pstHdmiHal);

    HAL_HDMI_CtrlAudioPathEnableSet(pstHdmiHal->stHalCtx.u32HdmiID, bEnable);

    return ;
}


HI_U32 HAL_HDMI_EdidRawDataRead(struct hiHDMI_HAL_S *pstHdmiHal, HI_U32 u32Size, HI_U8 u8OutBuffer[])
{
    HI_U32 u32Ret = 0;

    u32Ret = HAL_HDMI_DdcEdidRawGet(pstHdmiHal->stHalCtx.u32HdmiID,u32Size,u8OutBuffer);

    return u32Ret;
}

HI_VOID HAL_HDMI_PhyOutputEnableGet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL* pbEnable)
{
    HAL_HDMI_PhyOeGet(pbEnable);
    return ;
}

HI_VOID HAL_HDMI_IrqEnableSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_INT_TYPE_E enIntType, HI_BOOL bEnable)
{
    return ;
}

HI_VOID HAL_HDMI_VideoMuteSet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bVMute)
{
    HAL_HDMI_CtrlVideoMuteSet(pstHdmiHal->stHalCtx.u32HdmiID,bVMute);
    return ;
}
HI_VOID HAL_HDMI_BlackDataSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_BLACK_FRAME_INFO_S *pstBlack)
{
    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    INTF_NULL_CHK_NO_RET(pstBlack);
    HAL_HDMI_CtrlVideoMuteSet(pstHdmiHal->stHalCtx.u32HdmiID,pstBlack->bBlacEnable);

    return ;
}

#if 0
HI_VOID HAL_HDMI_BlackDataSet(HDMI_HAL_S *pstHdmiHal, HDMI_BLACK_FRAME_INFO_S *pstBlackPram)
{

    SiiBlackPramInfo_t stBlackPram;
    stBlackPram.bBlacEnable = pstBlackPram->bBlacEnable;
    stBlackPram.inBitDepth = pstBlackPram->inBitDepth;
    stBlackPram.inColorSpace = pstBlackPram->inColorSpace;
    stBlackPram.inQuantization = pstBlackPram->inQuantization;

    SiiDrvTxVideoInputPramSet(pstHdmiHal->stHalCtx.hHdmiHw, &stBlackPram);
    SiiDrvTxVideoBlackDataSet(pstHdmiHal->stHalCtx.hHdmiHw, pstBlackPram->bBlacEnable);

    return;
}
#endif
HI_VOID HAL_HDMI_Debug(HDMI_HAL_S *pstHdmiHal, HDMI_HAL_DEBUG_CMD_E enDebugCmd,HI_VOID *pData)
{
    DEBUG_DATA_CEC_S    stCecData;

    if (!pData)
    {
        HDMI_ERR("null \n");
        return ;
    }

    switch(enDebugCmd)
    {
        case HDMI_DEBUG_CMD_COLOR_BAR:
            INTF_NULL_CHK_NO_RET(pData);
            HalHdmiCBarEnable(*(HI_BOOL *)pData);
            break;

#if 0
        case HDMI_DEBUG_CMD_SW_RESET:
            HalHdmiSwReset(pstHdmiHal,HI_TRUE);
            break;

        case HDMI_DEBUG_CMD_RGB2YUV:
            HAL_NULL_CHK_NO_RET(pData);
            enConvertion = *(HI_BOOL *)pData ?  SII_COLORSAPCE_RGB2YUV : SII_COLORSPACE_NO_CONVERT;
            SiiDrvTxYuvRgbConvertion(pstHdmiHal->stHalCtx.hHdmiHw, &enConvertion);
            break;

        case HDMI_DEBUG_CMD_YUV2RGB:
            HAL_NULL_CHK_NO_RET(pData);
            enConvertion = *(HI_BOOL *)pData ?  SII_COLORSAPCE_YUV2RGB : SII_COLORSPACE_NO_CONVERT;
            SiiDrvTxYuvRgbConvertion(pstHdmiHal->stHalCtx.hHdmiHw, &enConvertion);
            break;

        case HDMI_DEBUG_CMD_DITHER:

            break;

        case HDMI_DEBUG_CMD_BYPASS:
            HAL_NULL_CHK_NO_RET(pData);
            SiiDrvTxTansCodeBypassConfig(pstHdmiHal->stHalCtx.hHdmiHw,*(bool_t *)pData);
            break;

        case HDMI_DEBUG_CMD_DDC_FREQ:
            HAL_NULL_CHK_NO_RET(pData);
            u32DdcSpeed = *(uint32_t *)pData;
            SiiDrvTxDdcSpeedConfig(pstHdmiHal->stHalCtx.hHdmiHw, u32DdcSpeed);
            break;

        case HDMI_DEBUG_CMD_PHY_DEFAULT_GET:
            HAL_NULL_CHK_NO_RET(pData);
            HalHdmiPhyParamGet(pstHdmiHal->stHalCtx.stVideoCfg.u32PixelClk,&pstPhyParam);
            HDMI_MEMCPY(pData,pstPhyParam,sizeof(SiiDrvPhyPara_t));
            break;

        case HDMI_DEBUG_CMD_PHY_PARA_SET:
            HAL_NULL_CHK_NO_RET(pData);
            pstPhyParam = (SiiDrvPhyPara_t *)pData;
            SiiDrvTxPhyConfig(pstHdmiHal->stHalCtx.hHdmiHw,pstPhyParam);
            break;

        case HDMI_DEBUG_CMD_DUMP:
            HAL_NULL_CHK_NO_RET(pData);
            pstReg  = (HDMI_HAL_REG_S *)pData;
            pstReg->u32RegVaule = HdmiRegRead((HI_VOID *)HDMI_IO_MAP(pstReg->u32RegAddr));
            break;
#endif
//#if defined (HDMI_SUPPORT_LOGIC_HISIV100)
        case HDMI_DEBUG_CMD_PROC_MACH:
            HAL_HDMI_DebugMachPrint(pData);
            break;

        case HDMI_DEBUG_CMD_PROC_SCDC:
            HAL_HDMI_DebugScdcPrint(pData);
            break;

        case HDMI_DEBUG_CMD_PROC_HDCP14:
            HAL_HDMI_DebugHdcp14Print(pData);
            break;

        case HDMI_DEBUG_CMD_PROC_DDC:
            HAL_HDMI_DebugDdcPrint(pData);
            break;

        case HDMI_DEBUG_CMD_PROC_CECTX:
            stCecData.u32HdmiId = *(HI_U32 *)pData;
            stCecData.bCecTx    = HI_TRUE;
            HAL_HDMI_DebugCecPrint(&stCecData);
            break;

        case HDMI_DEBUG_CMD_PROC_CECRX:
            stCecData.u32HdmiId = *(HI_U32 *)pData;
            stCecData.bCecTx    = HI_FALSE;
            HAL_HDMI_DebugCecPrint(&stCecData);
            break;
        case HDMI_DEBUG_CMD_DBG_VIDEO_GET:
            HAL_HDMI_CtrlVideoDbgGet(pstHdmiHal->stHalCtx.u32HdmiID,(HDMI_DBG_VIDEO_S *)pData);
            break;
        case HDMI_DEBUG_CMD_DBG_VIDEO_SET:
            HAL_HDMI_CtrlVideoDbgSet(pstHdmiHal->stHalCtx.u32HdmiID,(HDMI_DBG_VIDEO_S *)pData);
            break;
        case HDMI_DEBUG_CMD_SSC:
            HAL_HDMI_PhySscSet((HDMI_PHY_SSC_CFG_S *)pData);
            break;

//#endif
        default:
            HDMI_ERR("enDebugCmd = %d\n",enDebugCmd);
            break;
    }

    return;
}

#endif

HI_U32  *HAL_HDMI_BaseAddrGet(struct hiHDMI_HAL_S *pstHdmiHal)
{
    HI_U32 *pu32BaseAddr = HI_NULL;
#ifndef HDMI_BUILD_IN_BOOT
    pu32BaseAddr = (uint32_t *)HDMI_IO_MAP(HDMI_CTRL_BASE_ADDR, 0x4000);
#else
    pu32BaseAddr = (HI_U32 *)HDMI_CTRL_BASE_ADDR;
#endif

    return pu32BaseAddr;
}


#ifdef HDMI_HDCP_SUPPORT
HI_VOID HAL_HDMI_HdcpEnableSet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable)
{
    HDCP22_ATTR_S   stHdcp22Attr = {0};
    HDCP14_ATTR_S   stHdcp14Attr = {0};
    HDMI_HAL_CFG_S  *pstHalCfg = HI_NULL;

    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    pstHalCfg = IntfHalInfoPtrGet(pstHdmiHal->stHalCtx.u32HdmiID);
    INTF_NULL_CHK_NO_RET(pstHalCfg);

    if (pstHalCfg->enHdcpMode == HDMI_HDCP_MODE_1_4)
    {
        HAL_HDMI_Hdcp22MachEnable(pstHdmiHal->stHalCtx.u32HdmiID,HI_FALSE);
        HAL_HDMI_Hdcp14AttrSet(pstHdmiHal->stHalCtx.u32HdmiID,&stHdcp14Attr);
        DRV_HDMI_CompatHdcpMsGet(pstHdmiHal->stHalCtx.u32HdmiID,&stHdcp14Attr.u64StartWaitMs);
        HAL_HDMI_Hdcp14AttrSet(pstHdmiHal->stHalCtx.u32HdmiID,&stHdcp14Attr);
        HAL_HDMI_Hdcp14MachEnable(pstHdmiHal->stHalCtx.u32HdmiID,bEnable);
    }
    else if (pstHalCfg->enHdcpMode == HDMI_HDCP_MODE_2_2)
    {
        HAL_HDMI_Hdcp14MachEnable(pstHdmiHal->stHalCtx.u32HdmiID,HI_FALSE);
        HAL_HDMI_Hdcp22AttrGet(pstHdmiHal->stHalCtx.u32HdmiID,&stHdcp22Attr);
        DRV_HDMI_CompatHdcpMsGet(pstHdmiHal->stHalCtx.u32HdmiID,&stHdcp22Attr.u64StartWaitMs);
        HAL_HDMI_Hdcp22AttrSet(pstHdmiHal->stHalCtx.u32HdmiID,&stHdcp22Attr);
        HAL_HDMI_Hdcp22MachEnable(pstHdmiHal->stHalCtx.u32HdmiID,bEnable);
    }
    else
    {
        HDMI_WARN("halintf can't support hdcp mode %u\n",pstHalCfg->enHdcpMode );
    }

    return ;
}

HI_S32 HAL_HDMI_HdcpModeSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_HDCP_MODE_E enHdcpMode)
{
    HDMI_HAL_CFG_S *pstHalCfg = HI_NULL;

    INTF_NULL_CHK(pstHdmiHal);
    pstHalCfg = IntfHalInfoPtrGet(pstHdmiHal->stHalCtx.u32HdmiID);
    INTF_INIT_CHK(pstHalCfg);

    pstHalCfg->enHdcpMode = enHdcpMode;
    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_HdcpMcuCodeLoad(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_HDCP_MODE_E enHdcpMode)
{
    return HAL_HDMI_Hdcp22McuCodeLoad(pstHdmiHal->stHalCtx.u32HdmiID);
}


HI_S32 HAL_HDMI_HdcpKsvListGet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_HDCP_KSVLIST_S* pstKsvList)
{
    HDMI_WARN("not support this intf!\n");
    return HI_SUCCESS;
}
HI_VOID HAL_HDMI_HdcpSinkCapabilityGet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_HDCP_CAPABILITY_S *pstHdcpSupport)
{
    HDMI_HAL_CFG_S *pstHalCfg = HI_NULL;

    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    INTF_NULL_CHK_NO_RET(pstHdcpSupport);

    pstHalCfg = IntfHalInfoPtrGet(pstHdmiHal->stHalCtx.u32HdmiID);
    INTF_INIT_CHK_NO_RET(pstHalCfg);

    HAL_HDMI_Hdcp22Support(pstHdmiHal->stHalCtx.u32HdmiID,&pstHdcpSupport->bHdcp22Support);
    HAL_HDMI_Hdcp14Support(pstHdmiHal->stHalCtx.u32HdmiID,&pstHdcpSupport->bHdcp14Support);

    return ;
}
#endif

#ifdef HDMI_CEC_SUPPORT
HI_S32 HAL_HDMI_CecEnableSet(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable)
{
    INTF_NULL_CHK(pstHdmiHal);

    return HAL_HDMI_CecEnable(pstHdmiHal->stHalCtx.u32HdmiID,bEnable);
}

HI_S32 HAL_HDMI_CecNetPing(struct hiHDMI_HAL_S *pstHdmiHal, HI_U8 *pu8PhyAddr)
{
    INTF_NULL_CHK(pstHdmiHal);

    return HAL_HDMI_CecNetGet(pstHdmiHal->stHalCtx.u32HdmiID,pu8PhyAddr);
}


HI_S32 HAL_HDMI_CecStatusGet(struct hiHDMI_HAL_S *pstHdmiHal,HDMI_CEC_INFO_S *pstCecInfo)
{
    HI_S32      s32Ret = HI_SUCCESS;

    INTF_NULL_CHK(pstHdmiHal);
    INTF_NULL_CHK(pstCecInfo);

    s32Ret = HAL_HDMI_CecInfoGet(pstHdmiHal->stHalCtx.u32HdmiID,pstCecInfo);

    return s32Ret;

}


HI_S32 HAL_HDMI_CecMsgSend(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_CEC_CMD_S* pstMsg)
{
    HI_U32      i = 0;
    CEC_MSG_S   stCecMsg = {0};

    stCecMsg.enDestAddr     = pstMsg->enDstAdd;
    stCecMsg.enSrcAddr      = pstMsg->enSrcAdd;
    stCecMsg.u8Opcode       = pstMsg->u8Opcode;
    stCecMsg.stOperand.enUIOpcode           = pstMsg->stOperand.enUIOpcode;
    stCecMsg.stOperand.stRawData.u8Length   = pstMsg->stOperand.stRawData.u8Length;
    for (i=0;i<sizeof(stCecMsg.stOperand.stRawData.u8Data);i++)
    {
        stCecMsg.stOperand.stRawData.u8Data[i] = pstMsg->stOperand.stRawData.u8Data[i];
    }

    return HAL_HDMI_CecMsgTx(pstHdmiHal->stHalCtx.u32HdmiID,&stCecMsg);
}

HI_S32 HAL_HDMI_CecMsgReceive(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_CEC_CMD_S* pstMsg)
{
    HI_U32      i = 0;
    HI_S32      s32Ret = HI_SUCCESS;
    CEC_MSG_S   stCecMsg = {0};
    s32Ret = HAL_HDMI_CecMsgRx(pstHdmiHal->stHalCtx.u32HdmiID,&stCecMsg);

    pstMsg->enDstAdd = stCecMsg.enDestAddr;
    pstMsg->enSrcAdd = stCecMsg.enSrcAddr ;
    pstMsg->u8Opcode = stCecMsg.u8Opcode ;
    pstMsg->stOperand.enUIOpcode = stCecMsg.stOperand.enUIOpcode ;
    pstMsg->stOperand.stRawData.u8Length = stCecMsg.stOperand.stRawData.u8Length ;
    for (i=0;i<sizeof(stCecMsg.stOperand.stRawData.u8Data);i++)
    {
        pstMsg->stOperand.stRawData.u8Data[i] = stCecMsg.stOperand.stRawData.u8Data[i] ;
    }
    return s32Ret ;
}

#endif

#ifdef HDMI_SCDC_SUPPORT

HI_VOID HAL_HDMI_ScdcStatusSet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_SCDC_STATUS_S* pstScdcStatus)
{
    SCDC_ATTR_S stScdcAttr = {0};

    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    INTF_NULL_CHK_NO_RET(pstScdcStatus);
    stScdcAttr.bSinkReadQuest       = HI_FALSE;
    stScdcAttr.bSinkScramble        = pstScdcStatus->bSinkScrambleOn;
    stScdcAttr.bSrcScramble         = pstScdcStatus->bSourceScrambleOn;
    stScdcAttr.bTmdsClkRatio40x     = pstScdcStatus->u8TmdsBitClkRatio==40 ? HI_TRUE : HI_FALSE;
    stScdcAttr.u32ScrambleInterval  = pstScdcStatus->u32ScrambleInterval;
    stScdcAttr.u32ScrambleTimeout   = pstScdcStatus->u32ScrambleTimeout;
    HAL_HDMI_ScdcAttrSet(pstHdmiHal->stHalCtx.u32HdmiID, &stScdcAttr);
    return ;
}

HI_VOID HAL_HDMI_ScdcStatusGet(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_SCDC_STATUS_S* pstScdcStatus)
{

    SCDC_ATTR_S stScdcAttr = {0};

    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    INTF_NULL_CHK_NO_RET(pstScdcStatus);
    HAL_HDMI_ScdcAttrGet(pstHdmiHal->stHalCtx.u32HdmiID, &stScdcAttr);
    pstScdcStatus->bSinkReadQuest       = HI_FALSE;
    pstScdcStatus->bSinkScrambleOn      = stScdcAttr.bSinkScramble;
    pstScdcStatus->bSourceScrambleOn    = stScdcAttr.bSrcScramble;
    pstScdcStatus->u8TmdsBitClkRatio    = stScdcAttr.bTmdsClkRatio40x ? 40 : 10;
    pstScdcStatus->u32ScrambleInterval  = stScdcAttr.u32ScrambleInterval;
    pstScdcStatus->u32ScrambleTimeout   = stScdcAttr.u32ScrambleTimeout;

    return ;
}

HI_VOID HAL_HDMI_ScdcConfig(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_SCDC_CONFIG_S* pstScdcConfig)
{
    return ;
}

#endif

/*******************************define_start**********************************************************
Function:
int HAL_HDMI_Init(HMDI_CALLBACK_FUNC pCallback, HDMI_HAL_S** pstHdmiHalCtrl)
* Description:  打开HDMI HAL模块
* Input:        pCallback:上层驱动事件处理回调
* Output:       **pstHdmiHalCtrl: hal层全局结构体指针
* Return:        成功 / 错误码
* Others:        无
************************************end****************************************************************/
HI_S32 HAL_HDMI_Open(HDMI_HAL_INIT_S *pstHalInit, HDMI_HAL_S** pstHalHandle)
{
    HDMI_HAL_S  *pstHdmiHal = HI_NULL;
    HDMI_HAL_CFG_S *pstHalCfg = HI_NULL;

    if (pstHalHandle==NULL)
    {
        HDMI_ERR("pstHalHandle==NULL!\n");
        return HI_FAILURE;
    }

    pstHdmiHal = (HDMI_HAL_S *)HDMI_KMALLOC(sizeof(HDMI_HAL_S));
    if (pstHdmiHal == HI_NULL)
    {
        HDMI_ERR("Alloc HDMI_HAL_S struct memory fail\n");
        return HI_FAILURE;
    }
    else
    {
        HDMI_MEMSET(pstHdmiHal,0,sizeof(HDMI_HAL_S));
    }

    if (pstHalInit)
    {
        pstHdmiHal->stHalCtx.pCallback  = pstHalInit->pfnEventCallBack;
        pstHdmiHal->stHalCtx.hHdmiDev   = pstHalInit->pvEventData;
        pstHdmiHal->stHalCtx.u32HdmiID  = pstHalInit->u32HdmiDevId;
    }
    else
    {
        HDMI_INFO("pstHalInit null!open int boot!\n");
    }

    pstHalCfg = IntfHalInfoPtrGet(pstHdmiHal->stHalCtx.u32HdmiID);

    IntfTxCapabilityInit(pstHdmiHal->stHalCtx.u32HdmiID);

    HAL_HDMI_PhyInit();
#ifndef HDMI_BUILD_IN_BOOT
    HAL_HDMI_MachInit();

#endif
    HAL_HDMI_CtrlInit(pstHdmiHal->stHalCtx.u32HdmiID,pstHalInit);
    HAL_HDMI_DdcInit(pstHdmiHal->stHalCtx.u32HdmiID);
#ifndef HDMI_BUILD_IN_BOOT
    HAL_HDMI_CecInit(pstHdmiHal->stHalCtx.u32HdmiID);
    HAL_HDMI_Hdcp14Init(pstHdmiHal->stHalCtx.u32HdmiID,pstHalInit);
    HAL_HDMI_Hdcp22Init(pstHdmiHal->stHalCtx.u32HdmiID, pstHalInit);

#endif
    HAL_HDMI_ScdcInit(pstHdmiHal->stHalCtx.u32HdmiID);

    pstHdmiHal->HAL_HDMI_HardwareInit            = HAL_HDMI_HardwareInit;
    pstHdmiHal->HAL_HDMI_AvMuteSet               = HAL_HDMI_AvMuteSet;
    pstHdmiHal->HAL_HDMI_TmdsModeSet             = HAL_HDMI_TmdsModeSet;
    pstHdmiHal->HAL_HDMI_InfoframeSet            = HAL_HDMI_InfoframeSet;
    pstHdmiHal->HAL_HDMI_InfoframeEnableSet      = HAL_HDMI_InfoframeEnableSet;
    pstHdmiHal->HAL_HDMI_VideoPathSet            = HAL_HDMI_VideoPathSet;
    pstHdmiHal->HAL_HDMI_PhyPowerEnableSet       = HAL_HDMI_PhyPowerEnableSet;
    pstHdmiHal->HAL_HDMI_PhyOutputEnableSet      = HAL_HDMI_PhyOutputEnableSet;
    pstHdmiHal->HAL_HDMI_TxCapabilityGet         = HAL_HDMI_TxCapabilityGet;
    pstHdmiHal->HAL_HDMI_EmiSet                  = HAL_HDMI_EmiSet;
    pstHdmiHal->HAL_HDMI_EmiStatusGet            = HAL_HDMI_EmiStatusGet;
    pstHdmiHal->HAL_HDMI_CscParamSet             = HAL_HDMI_CscParamSet;
#if defined(CONFIG_HDMI_STB_SDK)
    pstHdmiHal->HAL_HDMI_DispFmt2HdmiTiming      = HAL_HDMI_DispFmt2HdmiTiming;
#endif
#ifndef HDMI_BUILD_IN_BOOT
    pstHdmiHal->HAL_HDMI_HardwareStatusGet       = HAL_HDMI_HardwareStatusGet;
    pstHdmiHal->HAL_HDMI_SequencerHandlerProcess = HAL_HDMI_SequencerHandlerProcess;
    pstHdmiHal->HAL_HDMI_AudioMuteSet            = HAL_HDMI_AudioMuteSet;
    pstHdmiHal->HAL_HDMI_AudioPathSet            = HAL_HDMI_AudioPathSet;
    pstHdmiHal->HAL_HDMI_AudioPathEnableSet      = HAL_HDMI_AudioPathEnableSet;
    pstHdmiHal->HAL_HDMI_EdidRawDataRead         = HAL_HDMI_EdidRawDataRead;
    pstHdmiHal->HAL_HDMI_PhyOutputEnableGet      = HAL_HDMI_PhyOutputEnableGet;
    pstHdmiHal->HAL_HDMI_HotPlugStatusGet        = HAL_HDMI_HotPlugStatusGet;
    pstHdmiHal->HAL_HDMI_IrqEnableSet            = HAL_HDMI_IrqEnableSet;
    pstHdmiHal->HAL_HDMI_VideoMuteSet            = HAL_HDMI_VideoMuteSet;
    pstHdmiHal->HAL_HDMI_BlackDataSet            = HAL_HDMI_BlackDataSet;
    pstHdmiHal->HAL_HDMI_Debug                   = HAL_HDMI_Debug;
    pstHdmiHal->HAL_HDMI_BaseAddrGet             = HAL_HDMI_BaseAddrGet;
#if defined(CONFIG_HDMI_STB_SDK)
    pstHdmiHal->HAL_HDMI_DispFmtGet              = HAL_HDMI_DispFmtGet;
#endif

#ifdef HDMI_HDR_SUPPORT
    pstHdmiHal->HAL_HDMI_HdrTimerSet             = HAL_HDMI_HdrTimerSet;
#endif

#endif
#ifdef HDMI_HDCP_SUPPORT
    pstHdmiHal->HAL_HDMI_HdcpEnableSet           = HAL_HDMI_HdcpEnableSet;
    pstHdmiHal->HAL_HDMI_HdcpModeSet             = HAL_HDMI_HdcpModeSet;
    pstHdmiHal->HAL_HDMI_HdcpMcuCodeLoad         = HAL_HDMI_HdcpMcuCodeLoad;
    pstHdmiHal->HAL_HDMI_HdcpKsvListGet          = HAL_HDMI_HdcpKsvListGet;
    pstHdmiHal->HAL_HDMI_HdcpSinkCapabilityGet   = HAL_HDMI_HdcpSinkCapabilityGet;
#endif
#ifdef HDMI_CEC_SUPPORT
    pstHdmiHal->HAL_HDMI_CecEnableSet            = HAL_HDMI_CecEnableSet;
    pstHdmiHal->HAL_HDMI_CecNetPing              = HAL_HDMI_CecNetPing;
    pstHdmiHal->HAL_HDMI_CecStatusGet         	 = HAL_HDMI_CecStatusGet;
    pstHdmiHal->HAL_HDMI_CecMsgSend              = HAL_HDMI_CecMsgSend;
    pstHdmiHal->HAL_HDMI_CecMsgReceive           = HAL_HDMI_CecMsgReceive;
#endif
#ifdef HDMI_SCDC_SUPPORT
    pstHdmiHal->HAL_HDMI_ScdcConfig              = HAL_HDMI_ScdcConfig;
    pstHdmiHal->HAL_HDMI_ScdcStatusGet           = HAL_HDMI_ScdcStatusGet;
    pstHdmiHal->HAL_HDMI_ScdcStatusSet           = HAL_HDMI_ScdcStatusSet;
#endif

    *pstHalHandle = pstHdmiHal;
    pstHalCfg->bInit = HI_TRUE;


    return HI_SUCCESS;
}

/*******************************define_start**********************************************************
Function:
void HAL_HDMI_Deinit(HDMI_HAL_S* pstHdmiHal)
* Description:  释放HDMI HAL模块
* Input:        *pstHdmiHal:hal层全局结构体指针
* Output:       无
* Return:        无
* Others:        无
************************************end****************************************************************/
void HAL_HDMI_Close(HDMI_HAL_S* pstHdmiHal)
{
    HDMI_HAL_CFG_S *pstHalCfg = (HDMI_HAL_CFG_S *)HI_NULL;

    INTF_NULL_CHK_NO_RET(pstHdmiHal);
    pstHalCfg = IntfHalInfoPtrGet(pstHdmiHal->stHalCtx.u32HdmiID);
    INTF_NULL_CHK_NO_RET(pstHalCfg);

#ifndef HDMI_BUILD_IN_BOOT
    HAL_HDMI_CecDeInit(pstHdmiHal->stHalCtx.u32HdmiID);
    HAL_HDMI_Hdcp14DeInit(pstHdmiHal->stHalCtx.u32HdmiID);
    HAL_HDMI_Hdcp22DeInit(pstHdmiHal->stHalCtx.u32HdmiID);
#endif
    HAL_HDMI_ScdcDeinit(pstHdmiHal->stHalCtx.u32HdmiID);
    HAL_HDMI_DdcDeInit(pstHdmiHal->stHalCtx.u32HdmiID);
    HAL_HDMI_CtrlDeinit(pstHdmiHal->stHalCtx.u32HdmiID);
#ifndef HDMI_BUILD_IN_BOOT

    HAL_HDMI_MachDeInit();
#endif
    HAL_HDMI_PhyDeInit();
    REG_HDMI_CRG_reg_DeInit();

    HDMI_MEMSET(pstHdmiHal,0,sizeof(HDMI_HAL_S));
    HDMI_KFREE(pstHdmiHal);
    pstHdmiHal = HI_NULL;

    pstHalCfg->bInit = HI_FALSE;

    return ;
}



