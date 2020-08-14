/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_ctrl.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/
#ifndef __HDMI_HAL_CTRL_H__
#define __HDMI_HAL_CTRL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "hi_type.h"

#ifndef HDMI_BUILD_IN_BOOT
#include "hdmi_hal_machine.h"
#endif

#include "drv_hdmi_common.h"
//nclude "drv_hdmi_edid.h"

#define HDMI_INFOFRMAE_MAX_SIZE         31


typedef enum
{
    HDMI_DBG_TIMMING_480P           = 0,
    HDMI_DBG_TIMMING_576P           = 1,
    HDMI_DBG_TIMMING_720P_50        = 2,
    HDMI_DBG_TIMMING_720P_60        = 3,
    HDMI_DBG_TIMMING_1080P_50       = 4,
    HDMI_DBG_TIMMING_1080P_60       = 5,
    HDMI_DBG_TIMMING_2160P_24       = 6,
    HDMI_DBG_TIMMING_2160P_25       = 7,
    HDMI_DBG_TIMMING_2160P_30       = 8,
    HDMI_DBG_TIMMING_2160P_24_SMPTE = 9,

    HDMI_DBG_TIMMING_1366X768_60    = 10,
    HDMI_DBG_TIMMING_800X600        = 11,
    HDMI_DBG_TIMMING_1680X1050      = 12,
    HDMI_DBG_TIMMING_1920X1200      = 13,
    HDMI_DBG_TIMMING_1600X1200_60   = 14,
    HDMI_DBG_TIMMING_2160P_50       = 15,
    HDMI_DBG_TIMMING_2160P_60       = 16,
    HDMI_DBG_TIMMING_1280X16_50     = 17,
    HDMI_DBG_TIMMING_240X16_60      = 18,
    HDMI_DBG_TIMMING_BUTT,    
}HDMI_DBG_TIMMING_E;

typedef enum
{
    HDMI_DBG_PIX_FMT_444,
    HDMI_DBG_PIX_FMT_YCBCR422,
    HDMI_DBG_PIX_FMT_YCBCR420,    
    HDMI_DBG_PIX_FMT_BUTT,
}HDMI_DBG_PIX_FMT_E;

typedef struct
{
    HI_BOOL             bPatternEnable;
    HDMI_DBG_PIX_FMT_E  enPixFormat;
    HI_BOOL             bSolidPattern;
    HI_BOOL             bColorBar;
    HI_BOOL             bSquarePattern;
    HI_BOOL             bYMask;
    HI_BOOL             bCbMask;
    HI_BOOL             bCrMask;
    HI_BOOL             bYCbCrSpace;
    //HI_BOOL             bMixColor;

    HI_BOOL             bTimmingEnable;
    HI_BOOL             bSelfDefineTimming;
    HI_BOOL             bHsyncPolarlity;
    HI_BOOL             bVsyncPolarlity;
    HDMI_DBG_TIMMING_E  enTimming;
}HDMI_DBG_VIDEO_S;


typedef enum
{
    HDMI_COLORMETRY_BT601,
    HDMI_COLORMETRY_BT709,
    HDMI_COLORMETRY_BT2020,
    HDMI_COLORMETRY_BT2020_CONST,
    HDMI_COLORMETRY_BUTT,
}HDMI_COLORMETRY_E;


typedef enum
{
	HDMI_VEDIO_DITHER_12_10	    = 0x00,
	HDMI_VEDIO_DITHER_12_8	    = 0x01,
	HDMI_VEDIO_DITHER_10_8	    = 0x02,
	HDMI_VEDIO_DITHER_DISALBE	= 0x03,
} HDMI_VEDIO_DITHER_E;



typedef struct 
{
    HI_BOOL     bHPolInver;
    HI_BOOL     bVPolInver;
    HI_BOOL     bDeInver;
    
}HDMI_SYNC_POL_S;

typedef struct 
{
    //HI_U32                  u32InPixelClk;      /* in KHz */
    HI_U32                  u32OutTmdsClk;      /* in KHz */
    HDMI_COLORSPACE_E       enInColorSpace;   
    HDMI_COLORSPACE_E       enOutColorSpace; 
    HDMI_COLORMETRY_E       enInColormetry;
    HDMI_COLORMETRY_E       enOutColormetry;
    HDMI_DEEP_COLOR_E       enInDeepColor;
    HDMI_DEEP_COLOR_E       enOutDeepColor;  
    HDMI_SYNC_POL_S         stOutHvSyncPol;
    HDMI_QUANTIZATION_RANGE enInQuantization;
    HDMI_QUANTIZATION_RANGE enOutQuantization;
    
}HDMI_VIDEO_PATH_S;

typedef struct 
{
    /*VDP hw timming*/
    HI_BOOL                 bSyncSwEnable;      
    HI_BOOL                 bVsyncPolarity;     /* when bSyncSwEnable==0,indicates hw;or ,indicates sw */
    HI_BOOL                 bHsyncPolarity;     /* when bSyncSwEnable==0,indicates hw;or ,indicates sw */
    HI_BOOL                 bProgressive;
    HI_U32                  u32HsyncTotal;
    HI_U32                  u32HactiveCnt;
    HI_U32                  u32VsyncTotal;
    HI_U32                  u32VactiveCnt;

}CTRL_TIMMING_DETECT_S;

typedef struct 
{
    //HI_U32                  u32InPixelClk;      /* in KHz */
    HI_U32                  u32OutTmdsClk;      /* in KHz */
    HDMI_COLORSPACE_E       enInColorSpace;   
    HDMI_COLORSPACE_E       enOutColorSpace; 
    HDMI_COLORMETRY_E       enInColormetry;
    HDMI_COLORMETRY_E       enOutColormetry;
    HDMI_DEEP_COLOR_E       enInDeepColor;
    HDMI_DEEP_COLOR_E       enOutDeepColor;  
    HDMI_SYNC_POL_S         stOutHvSyncPol;
    HDMI_QUANTIZATION_RANGE enInQuantization;
    HDMI_QUANTIZATION_RANGE enOutQuantization;

    HI_BOOL                 bCscEnable;
    HI_BOOL                 bY422Enalbe;
    HI_BOOL                 bY420Enalbe;
    HI_BOOL                 bDwsmHoriEnalbe;
    HI_BOOL                 bDwsmVertEnalbe;
    HDMI_VEDIO_DITHER_E     enDitherMode;

    CTRL_TIMMING_DETECT_S   stTimming;
    
}CTRL_VIDEO_STATUS_S;



typedef struct 
{
    //HI_BOOL                     bEnableAudio;
    //HI_BOOL                     bAudioMute;
    HDMI_AUDIO_INTERFACE_E      enSoundIntf;
    HDMI_SAMPLE_RATE_E          enSampleRate;
    HDMI_AUDIO_CH_E             enChanneNum;
    HDMI_AUDIO_BIT_DEPTH_E      enSampleBit;
    HI_U32                      u32PixelClk;        /* it is used to config N value */
    //HDMI_AUDIO_FORMAT_CODE_E enAudioCode;
    //HI_BOOL                               bDownSample;
}HDMI_AUDIO_PATH_S;

typedef struct 
{
    HI_BOOL                     bEnableAudio;
    HI_BOOL                     bAudioMute;
    HDMI_AUDIO_INTERFACE_E      enSoundIntf;
    HDMI_SAMPLE_RATE_E          enSampleRate;
    HDMI_AUDIO_CH_E             enChanneNum;
    HDMI_AUDIO_BIT_DEPTH_E      enSampleBit;
    HI_U32                      u32PixelClk;        /* it is used to config N value */
    //HDMI_AUDIO_FORMAT_CODE_E enAudioCode;
    //HI_BOOL                               bDownSample;

    HI_U32                      u32RefNvalue;
    HI_U32                      u32RegNvalue;
    HI_U32                      u32RefCtsvalue;
    HI_U32                      u32RegCtsvalue;
}CTRL_AUDIO_STATUS_S;


typedef struct
{
    HI_BOOL     bHpdOn;
    HI_BOOL     bRsenOn;
}HDMI_HPD_RSEN_S;


typedef struct
{
    HI_BOOL             bEnable;                //extern thread cfg
    HDMI_TIMER_TYPE_E   enTimerType;            //extern thread cfg
    HI_U64              u64TimeLength;          //extern thread cfg,in ms
    HI_U64              u64StartTime;           //extern thread cfg,in ms
}CTRL_HDR_TIMER_S;


typedef struct
{
    HI_BOOL                 bInit;
    HI_U32                  u32MachId;
    HDMI_TMDS_MODE_E        enTmdsMode;
    HI_BOOL                 bAvmute;
    HI_BOOL                 bAudioMute;
    HI_BOOL                 bVideoMute;
    HI_BOOL                 bIfEnable[HDMI_INFOFRAME_TYPE_BUTT-HDMI_INFOFRAME_TYPE_VENDOR];
    HI_BOOL                 bIfData[HDMI_INFOFRAME_TYPE_BUTT-HDMI_INFOFRAME_TYPE_VENDOR][HDMI_INFOFRMAE_MAX_SIZE];                 
    HDMI_AUDIO_PATH_S       stAudioPath;
    HDMI_VIDEO_PATH_S       stVideoPath;
    CTRL_HDR_TIMER_S        astHdrTimer[HDMI_TIMER_TYPE_BUTT];

    HI_VOID                 *pvEventData;
    HMDI_CALLBACK           pfnEventCallBack; 

#ifndef HDMI_BUILD_IN_BOOT
    PFN_MACH_CALLBACK       *pfnHpdCallBack;
#endif

}HDMI_CTRL_INFO_S ;


typedef struct
{
    HI_BOOL                 bInit;
    HDMI_TMDS_MODE_E        enTmdsMode;
    HI_BOOL                 bAvmute;
    HI_BOOL                 bAudioMute;
    HI_BOOL                 bVideoMute;
    HI_BOOL                 bIfEnable[HDMI_INFOFRAME_TYPE_BUTT-HDMI_INFOFRAME_TYPE_VENDOR];
    HI_BOOL                 bIfData[HDMI_INFOFRAME_TYPE_BUTT-HDMI_INFOFRAME_TYPE_VENDOR][HDMI_INFOFRMAE_MAX_SIZE];                 
    HDMI_AUDIO_PATH_S       stAudioPath;
    HDMI_VIDEO_PATH_S       stVideoPath;

    HDMI_HPD_RSEN_S         stHpdRsen;
    //HDMI_EDID_RAW_S         stEdidRaw;
}HDMI_CTRL_STATUS_S;


HI_S32 HAL_HDMI_CtrlInit(HDMI_DEVICE_ID_E enHdmi,HDMI_HAL_INIT_S *pstHalInit);
HI_S32 HAL_HDMI_CtrlDeinit(HDMI_DEVICE_ID_E enHdmi);
HI_S32 HAL_HDMI_CtrlReset(HDMI_DEVICE_ID_E enHdmi);
//HI_S32 HAL_HDMI_CtrlStatusGet(HDMI_CTRL_INFO_S  *pstCtrlInfo,HDMI_CTRL_STATUS_S *pstCtrlStatus);

HI_S32 HAL_HDMI_CtrlAvmuteSet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL bEnable);
HI_S32 HAL_HDMI_CtrlAvmuteGet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL *pbEnable);

HI_S32 HAL_HDMI_CtrlAudioMuteSet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL bEnable);
HI_S32 HAL_HDMI_CtrlAudioMuteGet(HDMI_DEVICE_ID_E  enHdmi,HI_BOOL *pbEnable);
HI_S32 HAL_HDMI_CtrlAudioPathEnableSet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL bEnable);
HI_S32 HAL_HDMI_CtrlAudioPathEnableGet(HDMI_DEVICE_ID_E  enHdmi,HI_BOOL *pbEnable);
HI_S32 HAL_HDMI_CtrlAudioPathSet(HDMI_DEVICE_ID_E  enHdmi,HDMI_AUDIO_PATH_S *pstAudioPath);
HI_S32 HAL_HDMI_CtrlAudioPathGet(HDMI_DEVICE_ID_E  enHdmi,CTRL_AUDIO_STATUS_S *pstAudioPath);

HI_S32 HAL_HDMI_CtrlVideoMuteSet(HDMI_DEVICE_ID_E  enHdmi,HI_BOOL bEnable);
HI_S32 HAL_HDMI_CtrlVideoMuteGet(HDMI_DEVICE_ID_E  enHdmi,HI_BOOL *pbEnable);
HI_S32 HAL_HDMI_CtrlVideoPathSet(HDMI_DEVICE_ID_E  enHdmi,HDMI_VIDEO_PATH_S *pstVideoPath);
HI_S32 HAL_HDMI_CtrlVideoPathGet(HDMI_DEVICE_ID_E  enHdmi,CTRL_VIDEO_STATUS_S *pstVideoStat);

HI_S32 HAL_HDMI_CtrlTmdsModeSet(HDMI_DEVICE_ID_E  enHdmi,HDMI_TMDS_MODE_E enTmdsMode);
HI_S32 HAL_HDMI_CtrlTmdsModeGet(HDMI_DEVICE_ID_E  enHdmi,HDMI_TMDS_MODE_E *penTmdsMode);

HI_S32 HAL_HDMI_CtrlInfoFrameEnSet(HDMI_DEVICE_ID_E  enHdmi, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_BOOL bEnable);
HI_S32 HAL_HDMI_CtrlInfoFrameEnGet(HDMI_DEVICE_ID_E  enHdmi, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_BOOL *pbEnable);
HI_S32 HAL_HDMI_CtrlInfoFrameDataSet(HDMI_DEVICE_ID_E  enHdmi, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_U8 *pu8IfData);
HI_S32 HAL_HDMI_CtrlInfoFrameDataGet(HDMI_DEVICE_ID_E  enHdmi, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_U8 *pu8IfData);

HI_S32 HAL_HDMI_CtrlHpdRsenGet(HDMI_DEVICE_ID_E  enHdmi,HDMI_HPD_RSEN_S *pstHpdRsen);

HI_S32 HAL_HDMI_CtrlHdrTimerSet(HDMI_DEVICE_ID_E  enHdmi,HDMI_TIMER_CONFIG_S *stHdrTimerCfg);

HI_S32 HAL_HDMI_CtrlVideoDbgSet(HDMI_DEVICE_ID_E  enHdmi,HDMI_DBG_VIDEO_S *pstDbgVideo);
HI_S32 HAL_HDMI_CtrlVideoDbgGet(HDMI_DEVICE_ID_E  enHdmi,HDMI_DBG_VIDEO_S *pstDbgVideo);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __HDMI_HAL_CTRL_H__ */   
