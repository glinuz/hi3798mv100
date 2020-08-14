
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_hal.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_DISP_HAL_H__
#define __DRV_DISP_HAL_H__

#include "hi_drv_disp.h"
#include "drv_disp_version.h"
#include "hal_specific_config.h"
//#include "vdp_define.h"
//#include "vdp_fpga_define.h"
#include "hal_specific_rwzb.h"
//#include "KdmTypeFxp.h"
#include "drv_disp_hdr.h"
#include "vdp_drv_hihdr.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define DISP_VDAC_MAX_NUMBER 6
#define DISP_VENC_SIGNAL_MAX_NUMBER 6
#define VDAC_STATE_INT 0xf000

#define DISP_VTTHD_VIDEO_OFFSET 0
#define DISP_VTTHD_GFX_OFFSET 80
#define DISP_VTTHD_DISP0_TO_DISP1 32

#define DISP_VTTHD_POINT_1 1
#define DISP_VTTHD_POINT_2 2
#define DISP_VTTHD_POINT_3 3
#define DISP_VTTHD_POINT_4 4

/*talk with jijiagang, all the chip behind cv200,will use the same reg.*/
#define DISP_USED_COMMON_REGISTER      0xf8000094
#define DISP_FASTBOOTUP_FLAG       0xabcd0000

//vdac
#define DAC_C_CTRL  0x0
#define DAC_R_CTRL  0x1
#define DAC_G_CTRL  0x2
#define DAC_B_CTRL  0x3
typedef enum tagDISP_INTF_TYPE_E
{
    DISP_INTF_TYPE_VDAC  = 0,

    DISP_INTF_TYPE_BT656,
    DISP_INTF_TYPE_BT1120,
    DISP_INTF_TYPE_LCD,
    DISP_INTF_TYPE_HDMI,
    DISP_INTF_TYPE_HVENC,
    DISP_INTF_TYPE_SVENC,
    DISP_INTF_TYPE_MIRACAST,
    DISP_INTF_TYPE_BUTT
}DISP_INTF_TYPE_E;

typedef enum tagDISP_WBC_E
{
    DISP_WBC_00 = 0,
    DISP_WBC_Video = 1,
    DISP_WBC_BUTT
}DISP_WBC_E;

typedef enum tagDISP_PLL_SOURCE_E
{
    DISP_CLOCK_SOURCE_SD0 = 0,
    DISP_CLOCK_SOURCE_HD0  ,
    DISP_CLOCK_SOURCE_HD1,
    DISP_CLOCK_SOURCE_BUTT
}DISP_PLL_SOURCE_E;
typedef enum tagDISP_CHANNEL_E
{
    DISP_CHANNEL_DHD0 = 0,
    DISP_CHANNEL_DHD1  ,
    DISP_CHANNEL_BUTT
}DISP_CHANNEL_E;

typedef enum tagDISP_HDMI_DATA_E
{
    DISP_HDMI_DATA_YUV = 0,
    DISP_HDMI_DATA_RGB  ,
    DISP_HDMI_DATA_BUTT
}DISP_HDMI_DATA_E;
#define HDMI_MODE_YUV  0
#define HDMI_MODE_RGB  1
typedef struct tagDISP_INTF_S
{
    HI_BOOL bOpen;

    HI_BOOL bLinkVenc;
    DISP_VENC_E eVencId;

    HI_DRV_DISP_INTF_S stIf;

    // signal index is VDACID
    //HI_DRV_DISP_VDAC_SIGNAL_E eSignal[DISP_VENC_SIGNAL_MAX_NUMBER];
    DISP_CHANNEL_E enDispChan;
    DISP_PLL_SOURCE_E enDispPll;
    DISP_HDMI_DATA_E enHDMIDataType;
}DISP_INTF_S;

typedef struct tagDISP_HAL_ENCFMT_PARAM_S
{
    // display into
    HI_DRV_DISP_FMT_E eFmt;
    HI_DRV_DISP_STEREO_E eDispMode;
    HI_BOOL bInterlace;

    HI_RECT_S stOrgRect;
    HI_RECT_S stRefRect;
    HI_DRV_ASPECT_RATIO_S stAR;

    HI_U32    u32RefreshRate;  /* in 1/100 Hz */

    HI_DRV_COLOR_SPACE_E enColorSpace;

}DISP_HAL_ENCFMT_PARAM_S;

typedef struct tagDISP_FMT_CFG_S
{
    VDP_DISP_SYNCINFO_S stTiming;
    DISP_PLL_SOURCE_E enPllIndex;
    HI_U32 u32Pll[2];
    DISP_HAL_ENCFMT_PARAM_S stInfo;
}DISP_FMT_CFG_S;


typedef struct tagDISP_VTTHD_CFG_S
{
    HI_U32 uVt1thdFieldMode;  // 0, frame; 1, field;
    HI_U32 uVt1thd;
    HI_U32 uVt2thdFieldMode;  // 0, frame; 1, field;
    HI_U32 uVt2thd;
}DISP_VTTHD_CFG_S;


typedef struct tagDISP_CAPA_S
{
    HI_BOOL bSupport;
    HI_BOOL bWbc;
    HI_BOOL bHD;
}DISP_CAPA_S;


typedef struct tagDISP_INTF_SETTING_S
{
    HI_DRV_DISP_FMT_E eFmt;

    // signal index is VDACID
    HI_DRV_DISP_VDAC_SIGNAL_E eSignal[DISP_VENC_SIGNAL_MAX_NUMBER];

}DISP_INTF_SETTING_S;


#if 0
typedef struct tagDISP_VENC_SETTING_S
{
    HI_DRV_DISP_FMT_E eFmt;

    // signal index is VDACID
    HI_DRV_DISP_VDAC_SIGNAL_E eSignal[DISP_VENC_SIGNAL_MAX_NUMBER];

}DISP_VENC_SETTING_S;

typedef struct tagDISP_HDMI_SETTING_S
{
    HI_DRV_COLOR_SPACE_E eColorSpace;
    HI_U32 u32DataWidth;  /* 8/10/12 bits */

}DISP_HDMI_SETTING_S;


typedef struct tagDISP_VDAC_SETTING_S
{
    HI_DRV_DISP_VDAC_SIGNAL_E eSignal;

}DISP_VDAC_SETTING_S;
#endif


typedef struct tagDISP_LOCAL_INTF_S
{
    HI_BOOL bSupport;
    HI_BOOL bIdle;
    HI_DRV_DISPLAY_E enChan;
}DISP_LOCAL_INTF_S;

typedef struct tagDISP_LOCAL_VENC_S
{
    HI_BOOL bSupport;
    HI_BOOL bIdle;
}DISP_LOCAL_VENC_S;

typedef struct tagDISP_LOCAL_VDAC_S
{
    HI_BOOL bSupport;
    HI_BOOL bIdle;
}DISP_LOCAL_VDAC_S;

typedef struct tagDISP_LOCAL_WBC_S
{
    HI_BOOL bSupport;
    HI_BOOL bIdle;
    HI_U32 u32RefCnt;
}DISP_LOCAL_WBC_S;

typedef struct tagDISP_HAL_COLOR_S
{
    HI_DRV_COLOR_SPACE_E enInputCS;
    HI_DRV_COLOR_SPACE_E enOutputCS;

    HI_U32 u32Bright;      //bright adjust value,range[0,100],default setting 50;
    HI_U32 u32Contrst;     //contrast adjust value,range[0,100],default setting 50;
    HI_U32 u32Hue;         //hue adjust value,range[0,100],default setting 50;
    HI_U32 u32Satur;       //saturation adjust value,range[0,100],default setting 50;
    HI_U32 u32Kr;          //red component gain adjust value for color temperature adjust,range[0,100],default setting 50;
    HI_U32 u32Kg;          //green component gain adjust value for color temperature adjust,range[0,100],default setting 50;
    HI_U32 u32Kb;          //blue component gain adjust value for color temperature adjust,range[0,100],default setting 50;

    HI_BOOL bGammaEnable;
    HI_BOOL bUseCustGammaTable;
}DISP_HAL_COLOR_S;

typedef struct tagDISP_CH_CFG_S
{
    HI_BOOL bLinkVenc;
    DISP_VENC_E enVenc;
    HI_U32 RESERVE;
}DISP_CH_CFG_S;

/**************************************PLL PARAMETRE BEGIN*********************************************/
#define FBDIV_I_MAX       2400
#define FBDIV_I_MIN       16

/*    pll_refdiv [17:12]  max=64*/
#define REFDIV_I_MAX       (1<<6)
#define REFDIV_I_MIN       1

/*KHz*/
/*800M~24000M*/
#define FOUTVCO_MAX       2400000
#define FOUTVCO_MIN       800000

#define PIX_DIFF_MAX       (3)

typedef struct tag_PLL_PARA_S
{
    HI_U32 u32FREF;
    HI_U32 u32REFDIV;
    HI_U32 u32FBDIV;
    HI_U32 u32FRAC;
    HI_U32 u32PSTDIV1;
    HI_U32 u32PSTDIV2;
    HI_U32 u32FOUTVCO;
    HI_U32 u32FOUTPOSTDIV;
    HI_U32 u32FOUT1PH0;
    HI_U32 u32REG1;
    HI_U32 u32REG2;

    /*set pix clk*/
    HI_U32 New_FOUT1PH0;
} PLL_PARA_S;

typedef struct tag_TEMP_PARA_S
{
    HI_U32 u32TmpFOUTVCO;
    HI_U32 u32TmpN;
    HI_S32 u32TmpM;
    HI_S32 u32TmpFBDIV;
} TEMP_PARA_S;

typedef struct tag_REG_PARA_S
{
    HI_S32 u32CalcPixFreq;
    HI_U32 u32ClkPara0;
    HI_U32 u32ClkPara1;
} REG_PARA_S;

#if 0
#ifndef __DISP_PLATFORM_BOOT__

/************HDR hal start****************/
//HAL layer adapter structure.
typedef enum tagDRV_DISP_HDR_CFG_MODE_E
{
    DRV_DOVI_BC_IN_DOVI_YUV_OUT  = 0x0 ,
    DRV_DOVI_NBC_IN_DOVI_YUV_OUT       ,

    DRV_DOVI_BC_IN_HDR10_OUT           ,
    DRV_DOVI_NBC_IN_HDR10_OUT          ,
    DRV_HDR10_IN_HDR10_OUT             ,

    //for STB hdmi output, 444
    //VDP_HDR_SDR_IN_SDR_OUT       , //don't need this mode.SDR will go through normal traditional path
    DRV_DOVI_BC_IN_SDR_OUT             ,
    DRV_DOVI_NBC_IN_SDR_OUT            ,
    DRV_HDR10_IN_SDR_OUT               ,

    DRV_DOVI_BC_IN_DOVI_IPT_OUT        ,
    DRV_DOVI_NBC_IN_DOVI_IPT_OUT       ,

    //for certificate: 422 uyvy output
//    VDP_HDR_SDR_IN_SDR_OUT_CERT            ,
//    VDP_HDR_DOVI_BC_IN_SDR_OUT_CERT        ,
//    VDP_HDR_DOVI_NBC_IN_SDR_OUT_CERT       ,
//    VDP_HDR_HDR10_IN_SDR_OUT_CERT          ,
//
//    //for FPGA test
//    VDP_HDR_DOVI_BC_IN_HDR10_OUT_FPGA      ,
//    VDP_HDR_DOVI_NBC_IN_HDR10_OUT_FPGA     ,
//    VDP_HDR_HDR10_IN_HDR10_OUT_FPGA        ,

    DRV_DISP_HDR_CFG_MODE_BUTT

}DRV_DISP_HDR_CFG_MODE_E;

typedef struct tagDISP_RECT_S
{
    HI_U32  u32X;
    HI_U32  u32Y;

    HI_U32  u32W;
    HI_U32  u32H;
}DISP_RECT_S;

typedef struct tagDRV_ADDR_S
{
    HI_U32 u32LumAddr;
    HI_U32 u32ChmAddr;

    HI_U32 u32LumStr;
    HI_U32 u32ChmStr;
}DRV_ADDR_S;

typedef struct hiDRV_DISP_HDR_CFG_S
{
    DRV_DISP_HDR_CFG_MODE_E  enHdrMode;
    HI_DRV_PIXEL_BITWIDTH_E  enDataWidth;  //0:8bit; 1:10bit
    DISP_RECT_S              stVidReso;     //vid input reso
    DISP_RECT_S              stBlReso;      //vid zme output reso
    DISP_RECT_S              stElReso;      //el input reso
    DISP_RECT_S              stGfxReso;     //gfx input reso

    DRV_ADDR_S               stBLAddr;
    DRV_ADDR_S               stELAddr;
    DRV_ADDR_S               stHeadAddr;    //dcmp head data addr

    HI_BOOL bDcmpEn;
    HI_BOOL bMuteEn;
    HI_BOOL bGfxEn;
    //HI_U32 u32CoefAddr[5];

} HI_DRV_DISP_HDR_CFG_S;
/*********************HDR hal end**********************************/
/**************************************PLL PARAMETRE END**********************************************/
#endif
#endif

typedef struct tagDISP_INTF_OPERATION_S
{
    /* usual */
    /* reset vdp */
    HI_S32 (*PF_ResetHardware)(HI_VOID);
    HI_S32 (*PF_CloseClkResetModule)(HI_VOID);
    HI_S32 (*PF_OpenClkStartModule)(HI_VOID);
    /* Display config */
    HI_BOOL (* PF_TestChnSupport)(HI_DRV_DISPLAY_E eChn);
    HI_BOOL (* PF_TestChnSupportHD)(HI_DRV_DISPLAY_E eChn);
    HI_BOOL (* PF_TestIntfSupport)(HI_DRV_DISPLAY_E eChn, HI_DRV_DISP_INTF_ID_E eIntf);
    HI_BOOL (* PF_TestChnSupportCast)(HI_DRV_DISPLAY_E eChn);
    HI_BOOL (* PF_TestChnEncFmt)(HI_DRV_DISPLAY_E eChn, HI_DRV_DISP_FMT_E eFmt);
    HI_BOOL (* PF_TestChnAttach)(HI_DRV_DISPLAY_E enM, HI_DRV_DISPLAY_E enS);

    HI_S32 (* PF_SetChnFmt)(HI_DRV_DISPLAY_E eChn, HI_DRV_DISP_FMT_E eFmt, HI_DRV_DISP_STEREO_E enStereo);
    HI_S32 (* PF_GetFmtTiming)(HI_DRV_DISP_FMT_E eFmt, HI_DRV_DISP_TIMING_S *pstTiming);
    HI_S32 (* PF_SetChnTiming)(HI_DRV_DISPLAY_E eChn, HI_DRV_DISP_TIMING_S *pstTiming);

    HI_S32 (* PF_SetHDMI420)(HI_DRV_DISPLAY_E eChn,HI_BOOL bEneble);

    HI_S32 (* PF_SetChnPixFmt)(HI_DRV_DISPLAY_E eChn, HI_DRV_PIX_FORMAT_E ePix);
    HI_S32 (* PF_SetChnBgColor)(HI_DRV_DISPLAY_E eChn, HI_DRV_COLOR_SPACE_E enCS, HI_DRV_DISP_COLOR_S *pstBGC);
    HI_S32 (* PF_SetChnColor)(HI_DRV_DISPLAY_E eChn, DISP_HAL_COLOR_S *pstColor);

    HI_S32 (* PF_SetChnEnable)(HI_DRV_DISPLAY_E eChn, HI_BOOL bEnalbe,HI_DRV_DISP_STEREO_MODE_E enStereo, HI_DRV_DISP_FMT_E enEncFmt,HI_BOOL bIsogenyMode);
    HI_S32 (* PF_GetChnEnable)(HI_DRV_DISPLAY_E eChn, HI_BOOL *pbEnalbe);
    HI_S32 (* PF_InitDacDetect)(HI_DRV_DISP_INTF_S *pstIf);
    HI_S32 (* PF_SetMSChnEnable)(HI_DRV_DISPLAY_E eChnM, HI_DRV_DISPLAY_E eChnS, HI_U32 u32DelayMs, HI_BOOL bEnalbe);
    HI_VOID (* PF_DispMask)(HI_DRV_DISPLAY_E eChn);
    HI_VOID (* PF_DispUnMask)(HI_DRV_DISPLAY_E eChn);
#if ((!defined(__DISP_PLATFORM_BOOT__)) && defined(DAC_TYPE_SYNOPSYS))
    HI_S32 (* PF_UpdateGamma)(GAMMA_CS_E  enGammaCsMode, PQ_GAMMA_RGB_MODE_S* pstPqGammaModeData);
    HI_S32 (* PF_SetGammaCtrl)(HI_DRV_DISPLAY_E eDisp,GAMMA_CS_E enGammaCsMode,GAMMA_MODE_E enGammaMode,HI_BOOL bEnable);
    HI_S32 (* PF_SetGammaRWZBCtrl)(HI_DRV_DISPLAY_E eDisp,HI_U32 ParaIndex);
#endif
    /* interrup */
    HI_S32 (* PF_SetIntEnable)(HI_U32 u32Int, HI_BOOL bEnable);
    HI_S32 (* PF_SetSMMUIntEnable)(HI_BOOL bEnable);
    HI_VOID (*PF_ConfigSmmu)(HI_VOID);
    HI_S32 (* PF_SetIntDisable)(HI_U32 u32Int);

    HI_S32 (* PF_GetIntSetting)(HI_U32 *pu32IntSetting);

    HI_S32 (* PF_GetMaskedIntState)(HI_U32 *pu32State);
    HI_S32 (* PF_GetMaskedIntState1)(HI_U32 *pu32State);
    HI_S32 (* PF_GetUnmaskedIntState)(HI_U32 *pu32State);
    HI_S32 (* PF_CleanIntState)(HI_U32 u32State);
    HI_S32 (* PF_CleanIntState1)(HI_U32 u32State);
    HI_U32 (* FP_GetChnIntState)(HI_DRV_DISPLAY_E enDisp, HI_U32 u32IntState);
    HI_U32 (* FP_GetChnBottomFlag)(HI_DRV_DISPLAY_E enDisp,  HI_BOOL *pbBtm, HI_U32 *pu32Vcnt);
    HI_S32 (*PF_GetAccurateLinePosition)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E eFmt);
    HI_S32 (*PF_GetCurrentVbiTimeInfor) (HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E eFmt, HI_U32 *pu32CircleTime, HI_U32 *pu32LeftTime);
    HI_S32 (*PF_Calculate_TriggerTime)(HI_U32 u32DispM_Intr_Line, HI_U32 u32DispM_Line_ReadFromSd, HI_U32 u32DispS_Intr_Line, HI_DRV_DISP_FMT_E eFmtM, HI_DRV_DISP_FMT_E eFmtS);

    /* intf manage */
    HI_S32 (* PF_AcquireIntf2)(HI_DRV_DISPLAY_E enDisp, DISP_INTF_S *pstIf);
    HI_S32 (* PF_ReleaseIntf2)(HI_DRV_DISPLAY_E enDisp, DISP_INTF_S *pstIf);
    HI_S32 (* PF_ResetIntfFmt2)(HI_DRV_DISPLAY_E enDisp, DISP_INTF_S *pstIntf, HI_DRV_DISP_FMT_E eFmt,HI_DRV_DISP_TIMING_S *pstCustomTimg);
    HI_S32 (* PF_SetIntfEnable2)(HI_DRV_DISPLAY_E enDisp, DISP_INTF_S *pstIntf, HI_BOOL bEnable);

    /* WBC manager */
    HI_S32 (* PF_AcquireWbcByChn)(HI_DRV_DISPLAY_E eChn, DISP_WBC_E *peWbc);
    HI_S32 (* PF_AcquireWbc)(DISP_WBC_E eWbc);
    HI_S32 (* PF_ReleaseWbc)(DISP_WBC_E eWbc);

    /* WBC */
    HI_S32 (*PF_SetWbcIORect)(DISP_WBC_E eWbc, HI_DISP_DISPLAY_INFO_S *pstDispInfo, HI_RECT_S *in, HI_RECT_S *out);
    HI_S32 (*PF_SetWbc3DInfo)(DISP_WBC_E eWbc, HI_DISP_DISPLAY_INFO_S *pstDispInfo, HI_RECT_S *in);
    HI_S32 (*PF_SetWbcColorSpace)(DISP_WBC_E eWbc, HI_DRV_COLOR_SPACE_E eSrcCS, HI_DRV_COLOR_SPACE_E eDstCS);
    HI_S32 (*PF_SetWbcPixFmt)(DISP_WBC_E eWbc, HI_DRV_PIX_FORMAT_E eFmt);
    HI_S32 (*PF_SetWbcAddr)(DISP_WBC_E eWbc, HI_DRV_VID_FRAME_ADDR_S *pstAddr);
    HI_S32 (*PF_SetWbcEnable)(DISP_WBC_E eWbc, HI_BOOL bEnable);
    HI_S32 (*PF_SetWbcLowDlyEnable)(DISP_WBC_E eWbc, HI_BOOL bEnable);
    HI_S32 (*PF_SetWbcPartfnsLineNum)(DISP_WBC_E eWbc, HI_U32 u32LineNum);
    HI_S32 (*PF_SetWbcLineNumInterval)(DISP_WBC_E eWbc, HI_U32 u32Interval);
    HI_S32 (*PF_SetWbcLineAddr)(DISP_WBC_E eWbc, HI_U32 u32Addr);
    HI_S32 (*PF_SetWbcSmmuBypass)(DISP_WBC_E eWbc, HI_BOOL bBypass);
    HI_S32 (*PF_UpdateWbc)(DISP_WBC_E eWbc);
    HI_S32 (*PF_DACIsr)(HI_U8 u8DAC);
    HI_S32 (*PF_SetDACDetEn)(HI_BOOL bDACDetEn);
    HI_S32 (*PF_GetDACAttr)(HI_DRV_VDAC_ATTR_S *pDACAttr);
    /*Zorder*/
    HI_S32 (*PF_CBM_MovTOP)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer);
    HI_S32 (*PF_VDP_RegSave)(HI_VOID);
    HI_S32 (*PF_VDP_RegReStore)(HI_VOID);

    /* date */
    HI_S32 (*PF_DATE_SetCoef)(HI_DRV_DISPLAY_E enDisp, HI_S32 u32Index);
    HI_S32 (*PF_DATE_SetIRE)(HI_DRV_DISPLAY_E enDisp, HI_U32 u32IRE);
    HI_S32 (*PF_GetIsogenyMode)(ISOGENY_MODE_E *peIsogeny_mode);

    /* O5 enable/disable DAC*/
    HI_S32 (*PF_SetAllDACEn)(HI_BOOL bDACEn);
    HI_S32 (*PF_GetIsoWbcPartFinishPoint) (HI_DRV_DISPLAY_E enDisp,
                                           HI_DRV_DISP_FMT_E eFmt,
                                           HI_U32  u32Percent,
                                           HI_U32 *pu32PartTime,
                                           HI_U32 *pu32FinishTime);

#ifndef __DISP_PLATFORM_BOOT__
/*****************HDR hal*****************************/
    HI_S32 (*PF_SetHdrMode)(HI_DRV_DISP_DOLBY_HDR_CFG_S *pstHdrCfg,rpu_ext_config_fixpt_main_t *pstCompCfg, DmKsFxp_t *pstDmKs);
    HI_S32 (*PF_SetHdrHdmiMetadata)(HI_U8 *pu8HeadData, HI_U32 u32MdLen, HI_U8 *pu8MdBuf);
    HI_VOID (*PF_CleanHdr)(HI_BOOL bclear);

    /* add for Hisi-self Hdr path */
    HI_S32 (*PF_SetHisiHdrMode)(HI_DRV_DISP_HISI_HDR_CFG_S *pstHisiHdrCfg,HI_PQ_HDR_CFG *pstPqHdrCfg);
    HI_VOID (*PF_SetHisiHdrMute)(HI_BOOL bEnbale);
    HI_VOID (*PF_CleanHisiHdr)(HI_BOOL bclear);

    /* gfx operations functions */
    HI_VOID (*PF_MaskAndBackupGfx)(HI_U32 u32GfxLayerId,HI_U32 *pu32Alpha,HI_U32 *pu32MuteState);
    HI_VOID (*PF_UnmaskGfx)(HI_U32 u32GfxLayerId,HI_U32 u32Alpha,HI_U32 u32MuteState);
    HI_VOID (*PF_SetGfxMixvBypassState)(HI_U32 u32GfxLayerId,HI_BOOL bEnable);
    HI_VOID (*PF_GetGfxState)(HI_BOOL *pbNeedProcessGfx);
    HI_S32  (*PF_CheckRegConfig)(HI_CHAR *pcheckItem);
    HI_S32  (*PF_GetPixelRepeatTimesAccordingFmt)(HI_DRV_DISPLAY_E enDisp,
                                                  HI_DRV_DISP_FMT_E eFmt,
                                                  HI_U32 *pu32PixelRepeatTimes);
#endif
}DISP_INTF_OPERATION_S;


HI_S32 DISP_HAL_Init(HI_U32 u32Base);
HI_S32 DISP_HAL_DeInit(HI_VOID);

HI_S32 DISP_HAL_GetVersion(HI_DRV_DISP_VERSION_S *pstVersion);
HI_S32 DISP_HAL_GetOperation(DISP_INTF_OPERATION_S *pstFunction);
DISP_INTF_OPERATION_S *DISP_HAL_GetOperationPtr(HI_VOID);



HI_S32 DISP_HAL_GetEncFmtPara(HI_DRV_DISP_FMT_E eFmt, DISP_HAL_ENCFMT_PARAM_S *pstFmtPara);
HI_S32 DISP_HAL_GetFmtAspectRatio(HI_DRV_DISP_FMT_E eFmt, HI_U32 *pH, HI_U32 *pV);
HI_S32 DISP_HAL_GetFmtColorSpace(HI_DRV_DISP_FMT_E eFmt, HI_DRV_COLOR_SPACE_E  *penColorSpace);

HI_U32 FP_GetChnBottomFlag(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbBtm, HI_U32 *pu32Vcnt);


extern HI_U32 Disp_SetFastbootupFlag(HI_U32 u32Value);

HI_VOID DISP_Hal_Isr_DebugSmmuProcess(HI_VOID);
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_DISP_CMP_H__  */










