/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_wrap_hisi_hal_vo.h
 * @brief   android hisi37xx vdec&vdisppipe
 * @author  lidanjing(l00346954)
 * @date    2015/12/4
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2015/12/4
 * Author : lidanjing(l00346954)
 * Desc   : Created file
 *
******************************************************************************/
#ifndef __SME_WRAP_HISI_HAL_VO_H__
#define __SME_WRAP_HISI_HAL_VO_H__

#include "osal_thread.h"
#include "hi_common.h"
#include "tvos_hal_vout.h"


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus


typedef V_VOID* SME_HAL_VDISP_PIPE_HDL;

#define HI_MMZ_ADDR_MAP_NUM_MAX 12


typedef struct __tagSmeHalHiVdispPipe
{
  VOUT_DEVICE_S * pstVoutDev;
  HANDLE phVout;
  HANDLE phVoutWindow;

  ST_VOS_THD_MUTEX    stMutex;
  V_UINT32            u32CurIdx;
  V_UINT64            u64FrameIdx;
  V_UINT64            u64DeqFailNum;

}ST_SME_HAL_HI_VDISP_PIPE;

typedef struct __tagVideoFrame
{
    V_UINT32 u32Width;
    V_UINT32 u32Height;
    V_VOID* pvAddr[4]; //y,u,v;r,g,b,a?, if no, set NULL
    V_UINT32 u32Len;
    V_UINT32 u32Stride[4];//reserve
    V_UINT32 u32Fmt;   //reserve;
    V_UINT32 u32Duration;//for FRC,unit: ms
}ST_SME_VIDEO_FRAME;

typedef enum
{
    E_SME_HIVO_ERR_NONE = 0,
    E_SME_HIVO_ERR_BUF_FULL, //queu again,buffer full
    E_SME_HIVO_ERR_INVALID_ARGS,
    E_SME_HIVO_ERR_FATAL, //fatal err;
}E_SME_HIVO_ERR;

typedef enum
{
    E_SME_HIVO_SWITCH_TYPE_BLACK = 0,
    E_SME_HIVO_SWITCH_TYPE_LAST,
    E_SME_HIVO_SWITCH_TYPE_BUTT,
}E_SME_HIVO_SWITCH_TYPE;

typedef enum smeMOD_ID_E
{
    SME_ID_STB         = 0,

    /**< common. */ /**< CNcomment: ?|¡§??¡ìa3?¡ìa??¨¬?¡ì??????¡ì???¡ìoy?Y3?¡§o?¡ì?¨¦? */
    SME_ID_SYS         = 1,
    SME_ID_MODULE      ,
    SME_ID_LOG         ,
    SME_ID_PROC        ,
    SME_ID_MEM         ,
    SME_ID_STAT        ,
    SME_ID_PDM         ,
    SME_ID_MEMDEV      ,
    SME_ID_CUSTOM      ,
    SME_ID_COMMON_BUTT ,

    SME_ID_DEMUX       = SME_ID_COMMON_BUTT + 1,
    SME_ID_DEMUX_BUTT  ,

    /**< audio. */ /**< CNcomment: ?¡ì????|¡§?2???¨¨?3?¡§o?¡ì?¨¦??? */
    SME_ID_ADEC        = SME_ID_DEMUX_BUTT + 1,
    SME_ID_AO          ,
    SME_ID_SIO_AI      ,
    SME_ID_SIO_AO      ,
    SME_ID_SIO         ,
    SME_ID_AI          ,
    SME_ID_AENC        ,
    SME_ID_SRC         ,
    SME_ID_AIAO        ,
    SME_ID_AFLT        ,
    SME_ID_ADSP        ,
    SME_ID_AMP         ,
    SME_ID_SIF         ,
    SME_ID_AUDIO_BUTT  ,



    /**< video. */ /**< CNcomment: ?¡ìo?¡ì??|¡§?2???¨¨?3?¡§o?¡ì?¨¦??? */
    SME_ID_VFMW        = SME_ID_AUDIO_BUTT + 1,
    SME_ID_SVDEC       ,
    SME_ID_DISP        ,
    SME_ID_HDMI        ,
    SME_ID_VO          ,
    SME_ID_VPSS        ,
    SME_ID_VDEC        ,
    SME_ID_VI          ,
    SME_ID_VENC        ,
    SME_ID_PQ          ,
    SME_ID_EDID        ,
    SME_ID_MEMC        ,
    SME_ID_VICAP       ,
    SME_ID_PANEL       ,
    SME_ID_FDMNG       ,
    SME_ID_VIDEO_BUTT  ,

    /**< graphics. */ /**< CNcomment: ?¡ìa?D?2???¨¨?3?¡§o?¡ì?¨¦??? */
    SME_ID_TDE         = SME_ID_VIDEO_BUTT + 1,
    SME_ID_JPGDEC      ,
    SME_ID_JPGENC      ,
    SME_ID_FB          ,
    SME_ID_PNG         ,
    SME_ID_HIGO        ,
    SME_ID_GRAPHICS_BUTT,

    /**< player. */ /**< CNcomment: 2?¨º?¨¨??¨¨???¡ì?¨¨1?2???¨¨?3?¡§o?¡ì?¨¦??? */
    SME_ID_PVR         = SME_ID_GRAPHICS_BUTT + 1,
    SME_ID_AVPLAY      ,
    SME_ID_SYNC        ,
    SME_ID_VSYNC       ,
    SME_ID_ASYNC       ,
    SME_ID_FASTPLAY    ,
    SME_ID_PLAYER_BUTT ,

    /**< ecs. */ /**< CNcomment: ?¡ìaa?¡ì|?¡ì?¡ì2???¨¨?3?¡§o?¡ì?¨¦??? */
    SME_ID_FLASH       = SME_ID_PLAYER_BUTT + 1,
    SME_ID_IR          ,
    SME_ID_RTC         ,
    SME_ID_I2C         ,
    SME_ID_SCI         ,
    SME_ID_ETH         ,
    SME_ID_USB_PROTECT ,
    SME_ID_WDG         ,   /* watch dog used 'W' */
    SME_ID_GPIO        ,
    SME_ID_GPIO_I2C    ,
    SME_ID_DMAC        ,
    SME_ID_PMOC        ,
    SME_ID_TUNER       ,
    SME_ID_KEYLED      ,
    SME_ID_E2PROM      ,
    SME_ID_CIPHER      ,
    SME_ID_OTP         ,
    SME_ID_CA          ,
    SME_ID_PM          ,
    SME_ID_CI          ,
    SME_ID_CIMAXPLUS   ,
    SME_ID_TVP5150     ,
    SME_ID_SIL9293     ,
    SME_ID_PWM         ,
    SME_ID_LSADC       ,
    SME_ID_ECS_BUTT    ,

    /**< ATV */ /**<  CNcomment: ATV?????2???¨¨?3?¡§o?¡ì?¨¦???*/
    SME_ID_VFE         = SME_ID_ECS_BUTT + 1,
    SME_ID_TVD         ,
    SME_ID_HDDEC       ,
    SME_ID_HDMIRX      ,
    SME_ID_VBI         ,
    SME_ID_ATV_BUTT    ,

    /**< voip, bluetooth,alsa. */ /**<  CNcomment: VOIP??¡§|?¡ì?¨¨???¡ì?¨¨2???¨¨?3?¡§o?¡ì?¨¦???*/
    SME_ID_VOIP_HME    = SME_ID_ATV_BUTT + 1,
    SME_ID_NDPT        ,
    SME_ID_AUDADP      ,
    SME_ID_BT          ,
    SME_ID_ALSA        ,
    SME_ID_3G          ,
    SME_ID_KARAOKE     ,
    SME_ID_VOIP_BUTT   ,

    /**< vp. */ /**<  CNcomment: VP3?¡§o?¡ì?¨¦???*/
    SME_ID_VP          = SME_ID_VOIP_BUTT+1,
    SME_ID_HDCP        ,
    SME_ID_VP_BUTT     ,

    /**< subtitle. */ /**<  CNcomment: ?¡§¡é???3?¡§o?¡ì?¨¦???*/
    SME_ID_SUBT        = SME_ID_VP_BUTT + 1,
    SME_ID_TTX         ,
    SME_ID_CC          ,
    SME_ID_SUBTITLE_BUTT,

    /**< loader. */ /**< CNcomment: loader */
    SME_ID_LOADER      = SME_ID_SUBTITLE_BUTT + 1,
    SME_ID_LOADER_BUTT ,

    /**< user definition. */ /**< CNcomment: ?a????¡ì2?¡§¡è?¡§o?¡ì?¨¦?|¡§???¡§¡é????¨¬?¡ì????? */
    SME_ID_USR_START   = SME_ID_LOADER_BUTT + 1,
    SME_ID_USER              ,
    SME_ID_USR_END     = 0xFE,    /*Caution: to avoid overflow*/

    SME_ID_BUTT        = 0xFF
} SME_MOD_ID_E;


typedef struct __tagHalDispRatio
{
    V_UINT32 u32DispRatioW;
    V_UINT32 u32DispRatioH;
}ST_DISP_HAL_RATIO;


typedef struct __tagOutRect
{
    V_UINT32 u32OutX;
    V_UINT32 u32OutY;
    V_UINT32 u32OutWidth;
    V_UINT32 u32OutHeight;
}ST_OUT_RECT;

typedef struct __tagDispScreen
{
    V_UINT32 u32DispWidth;
    V_UINT32 u32DispHeight;
}ST_DISP_SCREEN;

SME_HAL_VDISP_PIPE_HDL SME_Hal_VDisp_Create();
V_VOID SME_Hal_VDisp_Destory(IN SME_HAL_VDISP_PIPE_HDL hdlPipe);
V_UINT32 SME_Hal_VDisp_Init(IN SME_HAL_VDISP_PIPE_HDL hdlPipe);
V_UINT32 SME_Hal_VDisp_DeInit(IN SME_HAL_VDISP_PIPE_HDL hdlPipe);
V_UINT32 SME_Hal_VDisp_Updata_OutRect(IN SME_HAL_VDISP_PIPE_HDL hdlPipe, const HI_RECT_S * pstRect);
E_SME_HIVO_ERR SME_Hal_VDisp_QueueFrame(IN SME_HAL_VDISP_PIPE_HDL hdlPipe, IN const ST_SME_VIDEO_FRAME* pstVideoFrame);
E_SME_HIVO_ERR SME_Hal_VDisp_DeQueueFrame(IN SME_HAL_VDISP_PIPE_HDL hdlPipe, OUT ST_SME_VIDEO_FRAME* pstVideoFrame);
V_UINT32 SME_Hal_VDisp_Reset(IN SME_HAL_VDISP_PIPE_HDL hdlPipe, IN E_SME_HIVO_SWITCH_TYPE eSwitchMode);
E_SME_HIVO_ERR SME_Hal_VDisp_GetRenderDelay(IN SME_HAL_VDISP_PIPE_HDL hldPipe, OUT V_UINT64 *delay );

V_VOID SME_Get_Virtual_Screen(IN SME_HAL_VDISP_PIPE_HDL hdlPipe, OUT ST_DISP_SCREEN * pstVirScreen);
V_VOID SME_Disp_Full(IN const ST_OUT_RECT * pstDispRect, OUT ST_OUT_RECT * pstOutRect);
V_VOID SME_Disp_Fitin(SME_HAL_VDISP_PIPE_HDL hdlPipe, const ST_DISP_HAL_RATIO* p_disp_radio, const ST_OUT_RECT* p_win_rect, ST_OUT_RECT* p_disp_win);
V_UINT32 SME_Calcu_Win_Rect(IN const ST_DISP_HAL_RATIO stUI, IN ST_DISP_SCREEN stScreen, const ST_OUT_RECT* pstDispRect, ST_OUT_RECT* pOutRect);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif//__SME_VDEC_VDISP_PIPE_H__
