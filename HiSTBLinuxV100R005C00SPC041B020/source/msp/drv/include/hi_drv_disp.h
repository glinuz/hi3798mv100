/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_disp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/15
  Description   :
  History       :
  1.Date        : 2009/12/15
    Author      : w58735
    Modification: Created file

******************************************************************************/
#ifndef __HI_DRV_DISP_H__
#define __HI_DRV_DISP_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hi_debug.h"
#include "hi_drv_video.h"
#include "drv_pq_define.h"
//#include "drv_pq_ext.h"
#include "hi_drv_edid.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif
#ifndef HI_ADVCA_FUNCTION_RELEASE
#define HI_FATAL_DISP(fmt...) \
            HI_FATAL_PRINT(HI_ID_DISP, fmt)

#define HI_ERR_DISP(fmt...) \
            HI_ERR_PRINT(HI_ID_DISP, fmt)

#define HI_WARN_DISP(fmt...) \
            HI_WARN_PRINT(HI_ID_DISP, fmt)

#define HI_INFO_DISP(fmt...) \
            HI_INFO_PRINT(HI_ID_DISP, fmt)
#else

#define HI_FATAL_DISP(fmt...)
#define HI_ERR_DISP(fmt...)
#define HI_WARN_DISP(fmt...)
#define HI_INFO_DISP(fmt...)
#endif




/* display ID */
typedef enum hiDRV_DISPLAY_E
{
    HI_DRV_DISPLAY_0 = 0,
    HI_DRV_DISPLAY_1,
    HI_DRV_DISPLAY_2,
    HI_DRV_DISPLAY_BUTT
}HI_DRV_DISPLAY_E;

typedef enum hiDRV_DISP_LAYER_E
{
    HI_DRV_DISP_LAYER_NONE = 0,
    HI_DRV_DISP_LAYER_VIDEO,
    HI_DRV_DISP_LAYER_GFX,
    HI_DRV_DISP_LAYER_BUTT
}HI_DRV_DISP_LAYER_E;


typedef enum hiDRV_DISP_STEREO_MODE_E
{
    HI_DRV_DISP_STEREO_NONE = 0, /* 2D Mode */
    HI_DRV_DISP_STEREO_FRAME_PACKING,
    HI_DRV_DISP_STEREO_SBS_HALF,
    HI_DRV_DISP_STEREO_TAB,
    HI_DRV_DISP_STEREO_FIELD_ALT,
    HI_DRV_DISP_STEREO_LINE_ALT,
    HI_DRV_DISP_STEREO_SBS_FULL,
    HI_DRV_DISP_STEREO_L_DEPTH,
    HI_DRV_DISP_STEREO_L_DEPTH_G_DEPTH,
    HI_DRV_DISP_STEREO_MODE_BUTT
}HI_DRV_DISP_STEREO_MODE_E;

typedef enum hiDRV_DISP_ZORDER_E
{
    HI_DRV_DISP_ZORDER_MOVETOP = 0,
    HI_DRV_DISP_ZORDER_MOVEUP,
    HI_DRV_DISP_ZORDER_MOVEBOTTOM,
    HI_DRV_DISP_ZORDER_MOVEDOWN,
    HI_DRV_DISP_ZORDER_BUTT
}HI_DRV_DISP_ZORDER_E;


typedef enum hiDRV_DISP_VDAC_SIGNAL_E
{
    HI_DRV_DISP_VDAC_NONE = 0,
    HI_DRV_DISP_VDAC_CVBS,
    HI_DRV_DISP_VDAC_Y,
    HI_DRV_DISP_VDAC_PB,
    HI_DRV_DISP_VDAC_PR,
    HI_DRV_DISP_VDAC_SV_Y,
    HI_DRV_DISP_VDAC_SV_C,
    HI_DRV_DISP_VDAC_R,
    HI_DRV_DISP_VDAC_G,
    HI_DRV_DISP_VDAC_B,
    HI_DRV_DISP_VDAC_G_NOSYNC,
    HI_DRV_DISP_VDAC_SIGNAL_BUTT
}HI_DRV_DISP_VDAC_SIGNAL_E;

typedef enum hiDRV_DISP_INTF_ID_E
{
    HI_DRV_DISP_INTF_YPBPR0 = 0,
    HI_DRV_DISP_INTF_RGB0,
    HI_DRV_DISP_INTF_SVIDEO0,
    HI_DRV_DISP_INTF_CVBS0,
    HI_DRV_DISP_INTF_VGA0,

    HI_DRV_DISP_INTF_HDMI0,
    HI_DRV_DISP_INTF_HDMI1,
    HI_DRV_DISP_INTF_HDMI2,

    HI_DRV_DISP_INTF_BT656_0,
    HI_DRV_DISP_INTF_BT656_1,
    HI_DRV_DISP_INTF_BT656_2,

    HI_DRV_DISP_INTF_BT1120_0,
    HI_DRV_DISP_INTF_BT1120_1,
    HI_DRV_DISP_INTF_BT1120_2,

    HI_DRV_DISP_INTF_LCD0,
    HI_DRV_DISP_INTF_LCD1,
    HI_DRV_DISP_INTF_LCD2,

    HI_DRV_DISP_INTF_ID_MAX
}HI_DRV_DISP_INTF_ID_E;


#define HI_DISP_VDAC_MAX_NUMBER 4
#define HI_DISP_VDAC_INVALID_ID 0xff
typedef struct hiDRV_DISP_INTF_S
{
    HI_DRV_DISP_INTF_ID_E eID;
    HI_U8 u8VDAC_Y_G;
    HI_U8 u8VDAC_Pb_B;
    HI_U8 u8VDAC_Pr_R;
    HI_BOOL bDacSync;
}HI_DRV_DISP_INTF_S;

typedef enum hiDRV_DISP_INTF_DATA_FMT
{
    HI_DRV_DISP_INTF_DATA_FMT_YUV422 = 0,
    HI_DRV_DISP_INTF_DATA_FMT_RGB565,
    HI_DRV_DISP_INTF_DATA_FMT_RGB444,
    HI_DRV_DISP_INTF_DATA_FMT_RGB666,
    HI_DRV_DISP_INTF_DATA_FMT_RGB888,
    HI_DRV_DISP_INTF_DATA_FMT_BUTT
}HI_DRV_DISP_INTF_DATA_FMT_E;

typedef enum hiDRV_DISP_INTF_DATA_WIDTH_E
{
    HI_DRV_DISP_INTF_DATA_WIDTH8 = 0,       /**<8 bits*//**<CNcomment:8位*/
    HI_DRV_DISP_INTF_DATA_WIDTH16,          /**<16 bits*//**<CNcomment:16位*/
    HI_DRV_DISP_INTF_DATA_WIDTH24,          /**<24 bits*//**<CNcomment:24位*/
    HI_DRV_DISP_INTF_DATA_WIDTH_BUTT
}HI_DRV_DISP_INTF_DATA_WIDTH_E;


typedef struct hiDRV_DISP_TIMING_S
{
    HI_U32  u32VFB;
    HI_U32  u32VBB;
    HI_U32  u32VACT;
    HI_U32  u32HFB;
    HI_U32  u32HBB;
    HI_U32  u32HACT;
    HI_U32  u32VPW;
    HI_U32  u32HPW;
    HI_BOOL bIDV;
    HI_BOOL bIHS;
    HI_BOOL bIVS;
    HI_BOOL bClkReversal;
    HI_DRV_DISP_INTF_DATA_WIDTH_E  u32DataWidth;
    HI_DRV_DISP_INTF_DATA_FMT_E eDataFmt;

    HI_BOOL bDitherEnable;
    HI_U32  u32ClkPara0;
    HI_U32  u32ClkPara1;

    HI_BOOL bInterlace;
    HI_U32  u32PixFreq;
    HI_U32  u32VertFreq;
    HI_U32  u32AspectRatioW;
    HI_U32  u32AspectRatioH;

    HI_BOOL u32bUseGamma;

    HI_U32  u32Reserve0;
    HI_U32  u32Reserve1;
}HI_DRV_DISP_TIMING_S;

typedef struct hiDRV_DISP_COLOR_S
{
    HI_U8 u8Red;
    HI_U8 u8Green;
    HI_U8 u8Blue;
}HI_DRV_DISP_COLOR_S;

typedef struct hiDISP_CROP_S
{
    HI_U32 u32LeftOffset;
    HI_U32 u32TopOffset;
    HI_U32 u32RightOffset;
    HI_U32 u32BottomOffset;
}HI_DRV_DISP_CROP_S;

typedef struct hiDRV_DISP_OFFSET_S
{
    HI_U32 u32Left;    /*left offset */
    HI_U32 u32Top;     /*top offset */
    HI_U32 u32Right;   /*right offset */
    HI_U32 u32Bottom;  /*bottom offset */
}HI_DRV_DISP_OFFSET_S;

typedef struct  hiDRV_DISP_ISOGENY_ATTR_S
{
    HI_DRV_DISPLAY_E          enDisp;
    HI_DRV_DISP_FMT_E         enFormat;
}HI_DRV_DISP_ISOGENY_ATTR_S ;

typedef enum hiDRV_DISP_OUT_TYPE_E
{
    HI_DRV_DISP_TYPE_NORMAL = 0,    /* 普通信号显示 BT601 BT709*/
    HI_DRV_DISP_TYPE_SDR_CERT,      /* SDR输出认证时需要*/    
    HI_DRV_DISP_TYPE_DOLBY,         /* DolbyVision信号显示 */
    HI_DRV_DISP_TYPE_HDR10,         /* 标准HDR信号显示 */
    HI_DRV_DISP_TYPE_HDR10_CERT,
    HI_DRV_DISP_TYPE_HLG_ARIB,
    HI_DRV_DISP_TYPE_HLG_BBC,
    HI_DRV_DISP_TYPE_TECHNICOLOR,
    
    HI_DRV_DISP_TYPE_BUTT
}HI_DRV_DISP_OUT_TYPE_E;

typedef struct hiDRV_DISP_HDR_ATTR_S
{
    HI_DRV_DISP_OUT_TYPE_E             enDispType;         /* 显示类型 */
    union
    {
        HI_HDMI_DOLBY_SINK_CAPS_S      stDolbySinkAttr;    /* Dolby电视属性，当 enDispType = HI_DRV_DISP_TYPE_DOLBY 时有效 */
        HI_HDMI_HDR_SINK_CAPS_S        stHDR10SinkAttr;    /* 标准HDR电视属性，当 enDispType = HI_DRV_DISP_TYPE_HDR10 时有效 */
    } unSinkAttr;
} HI_DRV_DISP_HDR_ATTR_S;

typedef enum hiDRV_DISP_PROCESS_MODE_E
{
    HI_DRV_DISP_PROCESS_MODE_SDR = 0x0,      /* DISP go through normal SDR path */
    HI_DRV_DISP_PROCESS_MODE_HDR,            /* DISP go through HDR path(when source is DolbyVision or HDR10) */

    HI_DRV_DISP_MODE_BUTT
}HI_DRV_DISP_PROCESS_MODE_E;


typedef struct hiDISP_DISPLAY_INFO_S
{
    HI_BOOL bIsMaster;
    HI_BOOL bIsSlave;
    HI_DRV_DISPLAY_E enAttachedDisp;

    HI_DRV_DISP_STEREO_E eDispMode;
    HI_BOOL bRightEyeFirst;
    HI_BOOL bInterlace;
    HI_BOOL bIsBottomField;
    HI_U32 u32Vline;

    /*just a back of display setting, for virt screen and  offset set.*/
	HI_DRV_DISP_FMT_E eFmt;
    HI_RECT_S stVirtaulScreen;
    HI_DRV_DISP_OFFSET_S stOffsetInfo;
    HI_RECT_S stFmtResolution;
    HI_RECT_S stPixelFmtResolution;

    HI_DRV_ASPECT_RATIO_S stAR;
    HI_U32 u32RefreshRate;
    HI_DRV_COLOR_SPACE_E eColorSpace;

    HI_U32 u32Bright;
    HI_U32 u32Hue;
    HI_U32 u32Satur;
    HI_U32 u32Contrst;

    HI_U32 u32Kr;
    HI_U32 u32Kg;
    HI_U32 u32Kb;
    HI_DRV_DISP_HDR_ATTR_S    stDispHDRAttr;  /*  HDR显示相关属性 */
    HI_DRV_DISP_PROCESS_MODE_E  enDispProcessMode;  /* DISP HDR Path state,info for GFX
                                                 * when DISP is on HDR path,GFX & PQ
                                                 * need NOT to config CSC!
                                                 */
    HI_BOOL bBT2020Enable;
}HI_DISP_DISPLAY_INFO_S;

/*************** IP protect ***************/
//Macrovision
typedef enum hiDRV_DISP_MACROVISION_E
{
    HI_DRV_DISP_MACROVISION_TYPE0,
    HI_DRV_DISP_MACROVISION_TYPE1,
    HI_DRV_DISP_MACROVISION_TYPE2,
    HI_DRV_DISP_MACROVISION_TYPE3,
    HI_DRV_DISP_MACROVISION_CUSTOMER,
    HI_DRV_DISP_MACROVISION_BUTT
}HI_DRV_DISP_MACROVISION_E;


//CGMS-A
typedef enum hiDRV_DISP_CGMSA_TYPE_E
{
    HI_DRV_DISP_CGMSA_A = 0x00,
    HI_DRV_DISP_CGMSA_B,
    HI_DRV_DISP_CGMSA_TYPE_BUTT
}HI_DRV_DISP_CGMSA_TYPE_E;

typedef enum hiDRV_DISP_CGMSA_MODE_E
{
   HI_DRV_DISP_CGMSA_COPY_FREELY  = 0,
   HI_DRV_DISP_CGMSA_COPY_NO_MORE = 0x01,
   HI_DRV_DISP_CGMSA_COPY_ONCE    = 0x02,
   HI_DRV_DISP_CGMSA_COPY_NEVER   = 0x03,

   HI_DRV_DISP_CGMSA_MODE_BUTT
}HI_DRV_DISP_CGMSA_MODE_E;

typedef struct hiDRV_DISP_CGMSA_CFG_S
{
    HI_BOOL           bEnable;
    HI_DRV_DISP_CGMSA_TYPE_E  enType;
    HI_DRV_DISP_CGMSA_MODE_E  enMode;
}HI_DRV_DISP_CGMSA_CFG_S;


/*************** about VBI ***************/
typedef enum hiDRV_DISP_VBI_TYPE_E
{
    HI_DRV_DISP_VBI_TTX=0,
    HI_DRV_DISP_VBI_CC,
    HI_DRV_DISP_VBI_VCHIP,
    HI_DRV_DISP_VBI_WSS,
    HI_DRV_DISP_VBI_TYPE_BUTT
}HI_DRV_DISP_VBI_TYPE_E;

typedef struct hiDRV_DISP_VBI_CFG_S
{
    HI_DRV_DISP_VBI_TYPE_E eType;
    HI_U32  u32InBufferSize;
    HI_U32  u32WorkBufferSize;
}HI_DRV_DISP_VBI_CFG_S;

typedef struct hiDRV_DISP_TTX_DATA_S
{
    HI_U8   *pu8DataAddr;
    HI_U32   u32DataLen;
} HI_DRV_DISP_TTX_DATA_S;

typedef struct hiDRV_DISP_VBI_DATA_S
{
    HI_DRV_DISP_VBI_TYPE_E eType;
    HI_U8  *pu8DataAddr;
    HI_U32  u32DataLen;
}HI_DRV_DISP_VBI_DATA_S;

typedef struct hiDRV_DISP_COMPAT_VBI_DATA_S
{
    HI_DRV_DISP_VBI_TYPE_E eType;
    HI_U32  u32DataAddr;
    HI_U32  u32DataLen;
}HI_DRV_DISP_COMPAT_VBI_DATA_S;

typedef struct hiDRV_DISP_WSS_DATA_S
{
    HI_BOOL bEnable;
    HI_U16  u16Data;
}HI_DRV_DISP_WSS_DATA_S;

typedef struct hiDRV_DISP_HDMI_S
{
    HI_BOOL bHsyncNegative;
    HI_BOOL bVsyncNegative;
    HI_BOOL bDvNegative;
    HI_S32 s32SyncType;

    HI_S32 s32CompNumber;  //0,10bit; 1, 20bit; 2, 30bit
    HI_S32 s32DataFmt;     //0:YCbCr;1: RGB
}HI_DRV_DISP_HDMI_S;

typedef struct hiDRV_DISP_COLOR_SETTING_S
{
    HI_DRV_COLOR_SPACE_E enInCS;
    HI_DRV_COLOR_SPACE_E enOutCS;

    HI_U32 u32Bright;      //bright adjust value,range[0,100],default setting 50;
    HI_U32 u32Hue;         //hue adjust value,range[0,100],default setting 50;
    HI_U32 u32Satur;       //saturation adjust value,range[0,100],default setting 50;
    HI_U32 u32Contrst;     //contrast adjust value,range[0,100],default setting 50;

    HI_U32 u32Kr;
    HI_U32 u32Kg;
    HI_U32 u32Kb;
    HI_BOOL bGammaEnable;
    HI_BOOL bUseCustGammaTable;

    HI_VOID *pReserve;    /* must be 0 */
    HI_U32   u32Reserve;  /* must be 0 */
}HI_DRV_DISP_COLOR_SETTING_S;

typedef struct hiDRV_DISP_INIT_PARAM_S
{
    HI_U32                u32Version;
    //HI_BOOL               bSelfStart;
    HI_BOOL               bIsMaster;
    HI_BOOL               bIsSlave;
    HI_DRV_DISPLAY_E      enAttachedDisp;
    HI_DRV_DISP_FMT_E     enFormat;
    HI_U32                u32Brightness;
    HI_U32                u32Contrast;
    HI_U32                u32Saturation;
    HI_U32                u32HuePlus;
    HI_BOOL               bGammaEnable;
    HI_U32                u32VirtScreenWidth;
    HI_U32                u32VirtScreenHeight;
    HI_DRV_DISP_OFFSET_S  stOffsetInfo;
    HI_DRV_DISP_COLOR_S   stBgColor;
    HI_BOOL               bCustomRatio;
    HI_U32                u32CustomRatioWidth;
    HI_U32                u32CustomRatioHeight;
    HI_DRV_DISP_INTF_S    stIntf[HI_DRV_DISP_INTF_ID_MAX];
    HI_DRV_DISP_TIMING_S  stDispTiming;

    HI_U32  u32Reseve;
    HI_VOID *pRevData;
}HI_DRV_DISP_INIT_PARAM_S;

typedef struct hiDRV_DISP_SETTING_S
{
    HI_U32  u32BootVersion;
    HI_BOOL bGetPDMParam;
    //HI_BOOL bSelfStart;
    HI_BOOL bIsMaster;
    HI_BOOL bIsSlave;
    HI_DRV_DISPLAY_E enAttachedDisp;

    /* output format */
    HI_DRV_DISP_STEREO_E eDispMode;
    HI_DRV_DISP_FMT_E enFormat;
    HI_DRV_DISP_TIMING_S stCustomTimg;

    /* about color */
    HI_DRV_DISP_COLOR_SETTING_S stColor;

    /* background color */
    HI_DRV_DISP_COLOR_S stBgColor;

    /*just for screen ajust.*/
    HI_RECT_S stVirtaulScreen;
    HI_DRV_DISP_OFFSET_S stOffsetInfo;

    /* interface setting */
    HI_U32 u32IntfNumber;
    HI_DRV_DISP_INTF_S stIntf[HI_DRV_DISP_INTF_ID_MAX];

    HI_U32 u32LayerNumber;
    HI_DRV_DISP_LAYER_E enLayer[HI_DRV_DISP_LAYER_BUTT]; /* Z-order is from bottom to top */

    HI_BOOL bCustomRatio;
    HI_U32 u32CustomRatioWidth;
    HI_U32 u32CustomRatioHeight;

    HI_U32  u32Reseve;
    HI_VOID *pRevData;
}HI_DRV_DISP_SETTING_S;

typedef struct tagHI_DRV_VDAC_STATE_S
{
    HI_BOOL bDACPlugIn;
    HI_BOOL bDACDetectEn;
}HI_DRV_VDAC_STATE_S;

typedef struct tagHI_DRV_VDAC_ATTR_S
{
    HI_BOOL bDACDetEn;
    HI_DRV_VDAC_STATE_S stDACState[4];
}HI_DRV_VDAC_ATTR_S;

#define DEF_HI_DRV_DISP_MIRROR_BUFFER_MAX_NUMBER 16

typedef struct hiDRV_DISP_CAST_CFG_S
{
    /* frame config */
    HI_DRV_PIX_FORMAT_E eFormat; /* Support ... */
    HI_U32 u32Width;
    HI_U32 u32Height;

    /* buffer config */
    HI_U32  u32BufNumber; /* not more than HI_DISP_MIRROR_BUFFER_MAX_NUMBER */

    HI_BOOL bUserAlloc;  /* TRUE: user alloc buffers; FALSE: enDisp alloc buffers */
    HI_BOOL bLowDelay;

    HI_U32 u32BufSize;    /* every buffer size in Byte */
    HI_U32 u32BufStride;  /* only for 'bUserAlloc = TRUE' */

    HI_BOOL               bCrop;
    HI_DRV_CROP_RECT_S    stCropRect;
    HI_U32 u32BufPhyAddr[DEF_HI_DRV_DISP_MIRROR_BUFFER_MAX_NUMBER]; /* only for 'bUserAlloc = TRUE' */
} HI_DRV_DISP_CAST_CFG_S;

#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400) || defined(CHIP_TYPE_hi3719cv100) || defined(CHIP_TYPE_hi3718cv100) || defined(CHIP_TYPE_hi3719mv100_a)

/*************** about alg ***************/
typedef enum hiDRV_DISP_ALG_TYPE_E
{
    HI_DRV_DISP_ALG_DEI = 0,
    HI_DRV_DISP_ALG_ACC,
    HI_DRV_DISP_ALG_ACM,
    HI_DRV_DISP_ALG_SHARPNESS,
    HI_DRV_DISP_ALG_DNR,
    HI_DRV_DISP_ALG_TYPE_BUTT
}HI_DRV_DISP_ALG_TYPE_E;

typedef struct hiDRV_DISP_ALG_CFG_S
{
    HI_DRV_DISP_ALG_TYPE_E eType;
    HI_BOOL bEnable;

    HI_U32   u32Reserved;
    HI_VOID *pPrivate;
}HI_DRV_DISP_ALG_CFG_S;

#endif

#define DEF_HI_DRV_DISP_MAX_LAYER_NUMBER 8

typedef struct hiDRV_DISP_VERSION_S
{
    HI_U32 u32VersionPartL;
    HI_U32 u32VersionPartH;
}HI_DRV_DISP_VERSION_S;

#define DEF_DISP_CALLBACK_MAX_EVENT  8

typedef enum hiDRV_DISP_CALLBACK_TYPE_E
{
    /* 中断发起位置，以行有效区为参考，0为第一行，100为最后一行结束 */
    HI_DRV_DISP_C_TYPE_NONE = 0,
    //HI_DRV_DISP_C_SHOW_MODE,
    HI_DRV_DISP_C_INTPOS_0_PERCENT,
    HI_DRV_DISP_C_INTPOS_90_PERCENT,
    HI_DRV_DISP_C_INTPOS_100_PERCENT,
    HI_DRV_DISP_C_DHD0_WBC,
    HI_DRV_DISP_C_VID_WBC,
    HI_DRV_DISP_C_GFX_WBC,
    HI_DRV_DISP_C_REG_UP,
    HI_DRV_DISP_C_SMMU,
    
    HI_DRV_DISP_C_TYPE_BUTT
}HI_DRV_DISP_CALLBACK_TYPE_E;

typedef enum hiDRV_DISP_CALLBACK_EVENT_E
{
    HI_DRV_DISP_C_EVET_NONE = 0,
    HI_DRV_DISP_C_PREPARE_CLOSE,
    //HI_DRV_DISP_C_CLOSE            = 0x2,
    //HI_DRV_DISP_CALLBACK_PREPARE_TO_OPEN,
    HI_DRV_DISP_C_OPEN,

    HI_DRV_DISP_C_PREPARE_TO_PEND  = 0x10,
    //HI_DRV_DISP_C_PEND             = 0x20,
    //HI_DRV_DISP_C_PREPARE_TO_RESUME,
    HI_DRV_DISP_C_RESUME,

    HI_DRV_DISP_C_DISPLAY_SETTING_CHANGE = 0x20,

    HI_DRV_DISP_C_VT_INT = 0x100,
    HI_DRV_DISP_C_EVENT_BUTT
}HI_DRV_DISP_CALLBACK_EVENT_E;

typedef enum hiDRV_DISP_FIELD_FLAG_E
{
    HI_DRV_DISP_FIELD_PROGRESSIVE = 0,
    HI_DRV_DISP_FIELD_TOP,
    HI_DRV_DISP_FIELD_BOTTOM,
    HI_DRV_DISP_FIELD_FLAG_BUTT
}HI_DRV_DISP_FIELD_FLAG_E;

typedef struct hiDRV_DISP_CALLBACK_INFO_S
{
    HI_DRV_DISP_CALLBACK_EVENT_E eEventType;
    HI_DRV_DISP_FIELD_FLAG_E eField;

    HI_DISP_DISPLAY_INFO_S stDispInfo;
}HI_DRV_DISP_CALLBACK_INFO_S;

typedef struct hiDRV_DISP_Cast_Attr_S
{
    HI_S32  s32Width;
    HI_S32  s32Height;
}HI_DRV_DISP_Cast_Attr_S;


typedef enum hiDRV_DISP_CALLBACK_PRIORTY_E
{
    HI_DRV_DISP_CALLBACK_PRIORTY_HIGH = 0,
    HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE,
    HI_DRV_DISP_CALLBACK_PRIORTY_LOW,
    HI_DRV_DISP_CALLBACK_PRIORTY_BUTT
}HI_DRV_DISP_CALLBACK_PRIORTY_E;

typedef struct hiDRV_DISP_CALLBACK_S
{
    HI_VOID (* pfDISP_Callback)(HI_VOID *hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo);
    HI_DRV_DISP_CALLBACK_PRIORTY_E  eCallBackPrior;
    HI_VOID *hDst;
}HI_DRV_DISP_CALLBACK_S;

typedef struct hiDRV_DISP_VideoLogoInfo
{
	HI_BOOL bVideoLogoMode;
    HI_DRV_PIX_FORMAT_E ePixelFmt;
	
    HI_U32 u32LogoWidth;
    HI_U32 u32LogoHeight;
    
    HI_U32 u32LogoYStride;
    HI_U32 u32LogoCbStride;
    HI_U32 u32LogoCrStride;
    
    HI_U32 u32LogoYAddr;
    HI_U32 u32LogoCbAddr;
    HI_U32 u32LogoCrAddr;
	
}HI_DRV_DISP_VideoLogoInfo;


HI_S32 HI_DRV_DISP_Init(HI_VOID);
HI_S32 HI_DRV_DISP_DeInit(HI_VOID);
HI_S32 HI_DRV_DISP_Attach(HI_DRV_DISPLAY_E enDstDisp, HI_DRV_DISPLAY_E enSlave);
HI_S32 HI_DRV_DISP_Detach(HI_DRV_DISPLAY_E enDstDisp, HI_DRV_DISPLAY_E enSlave);
HI_S32 HI_DRV_DISP_SetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E enEncodingFormat);
HI_S32 HI_DRV_DISP_GetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E *penFormat);
HI_S32 HI_DRV_DISP_SetCustomTiming(HI_DRV_DISPLAY_E enDisp,  HI_DRV_DISP_TIMING_S *pstTiming);
HI_S32 HI_DRV_DISP_GetCustomTiming(HI_DRV_DISPLAY_E enDisp,  HI_DRV_DISP_TIMING_S *pstTiming);
HI_S32 HI_DRV_DISP_AddIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf);
HI_S32 HI_DRV_DISP_DelIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf);

HI_S32 HI_DRV_DISP_Open(HI_DRV_DISPLAY_E enDisp);

HI_S32 HI_DRV_DISP_Close(HI_DRV_DISPLAY_E enDisp);
HI_S32 HI_DRV_DISP_SetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable);
HI_S32 HI_DRV_DISP_GetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbEnable);
HI_S32 HI_DRV_DISP_SetRightEyeFirst(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable);

HI_S32 HI_DRV_DISP_SetBgColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBgColor);
HI_S32 HI_DRV_DISP_GetBgColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBgColor);
#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400) || defined(CHIP_TYPE_hi3719cv100) || defined(CHIP_TYPE_hi3718cv100) || defined(CHIP_TYPE_hi3719mv100_a)
HI_S32 HI_DRV_DISP_SetColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS);
HI_S32 HI_DRV_DISP_GetColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS);
#endif

HI_S32 HI_DRV_DISP_SetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Ratio_h, HI_U32 u32Ratio_v);
HI_S32 HI_DRV_DISP_GetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Ratio_h, HI_U32 *pu32Ratio_v);

HI_S32 HI_DRV_DISP_SetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_DRV_DISP_ZORDER_E enZFlag);
HI_S32 HI_DRV_DISP_GetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_U32 *pu32Zorder);

HI_S32 HI_DRV_DISP_CreateCast (HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CAST_CFG_S * pstCfg, HI_HANDLE *phCast);
HI_S32 HI_DRV_DISP_DestroyCast(HI_HANDLE hCast);

HI_S32 HI_DRV_DISP_SetCastEnable(HI_HANDLE hCast, HI_BOOL bEnable);
HI_S32 HI_DRV_DISP_GetCastEnable(HI_HANDLE hCast, HI_BOOL *pbEnable);

HI_S32 HI_DRV_DISP_AcquireCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame);
HI_S32 HI_DRV_DISP_ReleaseCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame);
HI_S32 HI_DRV_DISP_ExternlAttach(HI_HANDLE hCast, HI_HANDLE hSink);
HI_S32 HI_DRV_DISP_ExternlDetach(HI_HANDLE hCast, HI_HANDLE hSink);

HI_S32 HI_DRV_DISP_GetInitFlag(HI_BOOL *pbInited);
HI_S32 HI_DRV_DISP_GetVersion(HI_DRV_DISP_VERSION_S *pstVersion);
HI_BOOL HI_DRV_DISP_IsOpened(HI_DRV_DISPLAY_E enDisp);
HI_S32 HI_DRV_DISP_GetSlave(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penSlave);
HI_S32 HI_DRV_DISP_GetMaster(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penMaster);
HI_S32 HI_DRV_DISP_GetDisplayInfo(HI_DRV_DISPLAY_E enDisp, HI_DISP_DISPLAY_INFO_S *pstInfo);
HI_S32 HI_DRV_DISP_Process(HI_U32 cmd, HI_VOID *arg);
HI_S32 HI_DRV_DISP_RegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                            HI_DRV_DISP_CALLBACK_S *pstCallback);
HI_S32 HI_DRV_DISP_UnRegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                              HI_DRV_DISP_CALLBACK_S *pstCallback);

HI_S32 HI_DRV_DISP_FreeLogoMem(HI_VOID);
HI_S32 HI_DRV_DISP_GetLogoInfor(HI_DRV_DISP_VideoLogoInfo *pstLogoInfor);

#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400) || defined(CHIP_TYPE_hi3719cv100) || defined(CHIP_TYPE_hi3718cv100) || defined(CHIP_TYPE_hi3719mv100_a)
HI_S32 HI_DRV_DISP_UpdatePqData(HI_U32 u32UpdateType,PQ_PARAM_S * pstPqParam);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

