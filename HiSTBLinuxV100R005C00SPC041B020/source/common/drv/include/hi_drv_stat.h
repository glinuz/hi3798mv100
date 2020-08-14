/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_stat.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/12/17
  Description   :
  History       :
  1.Date        : 2008/12/17
    Author      : z42136
    Modification: Created file

******************************************************************************/

#ifndef __HI_DRV_STAT_H__
#define __HI_DRV_STAT_H__

#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define HI_FATAL_STAT(fmt...) HI_FATAL_PRINT(HI_ID_STAT, fmt)
#define HI_ERR_STAT(fmt...) HI_ERR_PRINT(HI_ID_STAT, fmt)
#define HI_WARN_STAT(fmt...) HI_WARN_PRINT(HI_ID_STAT, fmt)
#define HI_INFO_STAT(fmt...) HI_INFO_PRINT(HI_ID_STAT, fmt)
#define HI_DBG_STAT(fmt...) HI_DBG_PRINT(HI_ID_STAT, fmt)

typedef enum
{
    STAT_ISR_AUDIO = 0x0,
    STAT_ISR_VIDEO,
    STAT_ISR_DEMUX,
    STAT_ISR_SYNC,
    STAT_ISR_VO,
    STAT_ISR_TDE,

    STAT_ISR_BUTT
}STAT_ISR_E;

typedef enum tagSTAT_EVENT
{
    STAT_EVENT_KEYIN,
    STAT_EVENT_KEYOUT,
    STAT_EVENT_ASTOP,
    STAT_EVENT_VSTOP,
    STAT_EVENT_CONNECT,
    STAT_EVENT_LOCKED,
    STAT_EVENT_ASTART,
    STAT_EVENT_VSTART,
    STAT_EVENT_CWSET,
    STAT_EVENT_STREAMIN,
    STAT_EVENT_ISTREAMGET,
    STAT_EVENT_IFRAMEOUT,
    STAT_EVENT_VPSSGETFRM,
    STAT_EVENT_VPSSOUTFRM,
    STAT_EVENT_AVPLAYGETFRM,
    STAT_EVENT_PRESYNC,
    STAT_EVENT_BUFREADY,
    STAT_EVENT_FRAMESYNCOK,
    STAT_EVENT_VOGETFRM,
    STAT_EVENT_IFRAMEINTER,
    STAT_EVENT_BUTT

}STAT_EVENT_E;


typedef HI_VOID (*STAT_Event_Fun)(STAT_EVENT_E, HI_U32);

HI_S32  HI_DRV_STAT_EventFunc_Register(HI_VOID* pFunc);
HI_VOID HI_DRV_STAT_EventFunc_UnRegister(HI_VOID);
HI_S32  HI_DRV_STAT_KInit(HI_VOID);
HI_VOID HI_DRV_STAT_KExit(HI_VOID);

HI_S32  HI_DRV_STAT_Init(HI_VOID);
HI_VOID HI_DRV_STAT_Exit(HI_VOID);

/*interrupt cost time in kernel-state*/
/*CNcomment:内核态中断耗时统计*/
#if defined(HI_STAT_ISR_SUPPORTED)
HI_VOID HI_DRV_STAT_IsrReset(HI_VOID);
HI_VOID HI_DRV_STAT_IsrEnable(HI_VOID);
HI_VOID HI_DRV_STAT_IsrDisable(HI_VOID);

HI_VOID HI_DRV_STAT_IsrBegin(STAT_ISR_E isr);
HI_VOID HI_DRV_STAT_IsrEnd(STAT_ISR_E isr);
#endif

HI_VOID HI_DRV_STAT_Event(STAT_EVENT_E enEvent, HI_U32 Value);
HI_U32  HI_DRV_STAT_GetTick(HI_VOID);

/* ---------------- low delay statistics structs and export functions ----------------------- */

/*
 * scenes type definition
 */
typedef enum {
    SCENES_VID_PLAY = 0,
    SCENES_VID_CAP,
    SCENES_VID_CAST,

    SCENES_LD_BUTT,
}HI_LD_Scenes_E;

/*
 * event type definition
 */
typedef enum {
    EVENT_VI_FRM_IN = 0,
    EVENT_VI_FRM_OUT,
    EVENT_VPSS_FRM_IN,
    EVENT_VPSS_FRM_OUT,
    EVENT_VDEC_FRM_IN,
    EVENT_VDEC_FRM_OUT,
    EVENT_VENC_FRM_IN,
    EVENT_VENC_FRM_OUT,
    EVENT_AVPLAY_FRM_IN,
    EVENT_AVPLAY_FRM_OUT,
    EVENT_VO_FRM_IN,
    EVENT_VO_FRM_OUT,
    EVENT_CAST_FRM_BEGIN,
    EVENT_CAST_FRM_OUT,

    EVENT_LD_BUTT,
}HI_LD_Event_ID_E;

/*
 * low delay event definition
 */
typedef struct
{
    HI_LD_Event_ID_E    evt_id;
    HI_HANDLE           handle;
    HI_U32              frame;
    HI_U32              time;
} HI_LD_Event_S;

#define MAX_EVENT_QUEUE_SIZE    (10 + 1)    /* ring queue need one more entry for diff empty and full status */
#define MAX_SCENES_TYPE_NR      (SCENES_LD_BUTT)
#define MAX_EVENT_TYPE_NR       (EVENT_LD_BUTT)

/*
 * define scenes what's the composition of which events
 */
static const HI_LD_Event_ID_E g_scenes_desc[MAX_SCENES_TYPE_NR][MAX_EVENT_TYPE_NR] __attribute__((unused)) =
{
    /* SCENES_VID_PLAY: VDEC_IN -> VDEC_OUT ->VPSS_IN -> VPSS_OUT -> AVPLAY_IN-> AVPLAY_OUT -> VO_IN->VO_OUT */
    {EVENT_VDEC_FRM_IN, EVENT_VDEC_FRM_OUT, EVENT_VPSS_FRM_IN, EVENT_VPSS_FRM_OUT, EVENT_AVPLAY_FRM_IN, EVENT_AVPLAY_FRM_OUT, EVENT_VO_FRM_IN, EVENT_VO_FRM_OUT, EVENT_LD_BUTT},
    /* SCENES_VID_CAP: VI_IN -> VPSS_IN -> VPSS_OUT -> VI_OUT -> VENC_IN -> VENC_OUT */
    {EVENT_VI_FRM_IN, EVENT_VPSS_FRM_IN, EVENT_VPSS_FRM_OUT, EVENT_VI_FRM_OUT, EVENT_VENC_FRM_IN,  EVENT_VENC_FRM_OUT, EVENT_LD_BUTT},
    /* SCENES_VID_CAST: DISP_OUT -> VENC_IN -> VENC_OUT */
    {EVENT_CAST_FRM_BEGIN,EVENT_CAST_FRM_OUT, EVENT_VENC_FRM_IN, EVENT_VENC_FRM_OUT, EVENT_LD_BUTT},
};

/*
 * define event name
 */
static const HI_CHAR * g_event_name[MAX_EVENT_TYPE_NR] __attribute__((unused)) =
{
    [EVENT_VI_FRM_IN]       = "VI_IN",
    [EVENT_VI_FRM_OUT]      = "VI_OUT",
    [EVENT_VPSS_FRM_IN]     = "VPSS_IN",
    [EVENT_VPSS_FRM_OUT]    = "VPSS_OUT",
    [EVENT_VDEC_FRM_IN]     = "VDEC_IN",
    [EVENT_VDEC_FRM_OUT]    = "VDEC_OUT",
    [EVENT_VENC_FRM_IN]     = "VENC_IN",
    [EVENT_VENC_FRM_OUT]    = "VENC_OUT",
    [EVENT_AVPLAY_FRM_IN]   = "AVPLAY_IN",
    [EVENT_AVPLAY_FRM_OUT]  = "AVPLAY_OUT",
    [EVENT_VO_FRM_IN]       = "VO_IN",
    [EVENT_VO_FRM_OUT]      = "VO_OUT",
    [EVENT_CAST_FRM_BEGIN]  = "CAST_FRAME_BEGIN",
    [EVENT_CAST_FRM_OUT]    = "CAST_OUT",
};

HI_S32  HI_DRV_LD_Start_Statistics(HI_LD_Scenes_E scenes_id, HI_HANDLE filter);
HI_VOID HI_DRV_LD_Stop_Statistics(HI_VOID);
HI_VOID HI_DRV_LD_Notify_Event(HI_LD_Event_S * evt);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_DRV_STAT_H__ */

