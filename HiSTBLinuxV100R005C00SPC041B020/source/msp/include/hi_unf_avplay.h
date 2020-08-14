/******************************************************************************

  Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_avplay.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/21
  Description   :
  History       :
  1.Date        : 2009/12/21
    Author      : w58735
    Modification: Created file

*******************************************************************************/
/**
 * \file
 * \brief Describes the information about the audio/video player (AVPLAY) module.
          CNcomment:提供AVPLAY的相关信息 CNend
 */
#ifndef __HI_UNF_AVPLAY_H__
#define __HI_UNF_AVPLAY_H__

#include "hi_unf_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/*********************************error  macro******************************************/
/*************************** Structure Definition ****************************/
/** \addtogroup      AVPLAY */
/** @{ */  /** <!-- [AVPLAY] */

/**Highest priority of the video decoder*/
/**CNcomment:视频解码器的最大优先级 */
#define HI_UNF_VCODEC_MAX_PRIORITY  16


/**Defines the buffer ID required in an AVPLAY.*/
/**CNcomment:定义AV播放器中需要的BufferID枚举类型 */
typedef enum hiUNF_AVPLAY_BUFID_E
{
    HI_UNF_AVPLAY_BUF_ID_ES_VID  = 0,       /**<ID of the buffer for storing the video elementary streams (ESs) played by an AVPLAY*//**<CNcomment: AV播放器ES视频Buffer ID. */
    HI_UNF_AVPLAY_BUF_ID_ES_AUD,            /**<ID of the buffer for storing the audio ESs played by an AVPLAY*//**<CNcommentAV播放器ES音频Buffer ID.*/
    HI_UNF_AVPLAY_BUF_ID_ES_AUD_AD,         /**<ID of the buffer for storing the AD audio ESs played by an AVPLAY*//**<CNcommentAV播放器AD ES音频Buffer ID.*/
    HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_BL,     /**<ID of the buffer for storing the HDR BL video elementary streams (ESs) played by an AVPLAY*//**<CNcomment: AV播放器HDR BL帧Buffer ID. */
    HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_EL,     /**<ID of the buffer for storing the HDR EL video elementary streams (ESs) played by an AVPLAY*//**<CNcomment: AV播放器HDR EL帧Buffer ID. */
    HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_DL,     /**<ID of the buffer for storing the HDR BL+EL video elementary streams (ESs) played by an AVPLAY*//**<CNcomment: AV播放器HDR BL+EL帧Buffer ID. */
    HI_UNF_AVPLAY_BUF_ID_BUTT
} HI_UNF_AVPLAY_BUFID_E;

/**Defines the type of a media channel.*/
/**CNcomment:定义媒体通道类型. */
typedef enum hiUNF_AVPLAY_MEDIA_CHAN
{
    HI_UNF_AVPLAY_MEDIA_CHAN_AUD  = 0x01,  /**<Audio channel*//**<CNcomment: 音频通道 */
    HI_UNF_AVPLAY_MEDIA_CHAN_VID  = 0x02,  /**<Video channel*//**<CNcomment: 视频通道 */

    HI_UNF_AVPLAY_MEDIA_CHAN_BUTT = 0x8
} HI_UNF_AVPLAY_MEDIA_CHAN_E;

/**Defines the type of an input stream interface.*/
/**CNcomment:定义数据输入流接口类型 */
typedef enum hiUNF_AVPLAY_STREAM_TYPE_E
{
    HI_UNF_AVPLAY_STREAM_TYPE_TS = 0,   /**<Transport stream (TS)*//**<CNcomment:TS码流 */
    HI_UNF_AVPLAY_STREAM_TYPE_ES,       /**<ES stream*//**<CNcomment:ES码流 */

    HI_UNF_AVPLAY_STREAM_TYPE_BUTT
} HI_UNF_AVPLAY_STREAM_TYPE_E;

/**Defines the stream attributes.*/
/**CNcomment:定义码流属性的结构体 */
typedef struct hiUNF_AVPLAY_STREAM_ATTR_S
{
    HI_UNF_AVPLAY_STREAM_TYPE_E enStreamType;   /**<Stream type*//**<CNcomment:码流类型 */

    HI_U32                      u32VidBufSize;  /**<Video buffer size*//**<CNcomment: 视频缓冲大小 */
    HI_U32                      u32AudBufSize;  /**<Audio buffer size*//**<CNcomment: 音频缓冲大小 */

} HI_UNF_AVPLAY_STREAM_ATTR_S;

/**Supported synchronization control mode*/
/**CNcomment:支持的同步控制模式 */
typedef enum hiUNF_SYNC_REF_E
{
    HI_UNF_SYNC_REF_NONE = 0, /**<Free playing without synchronization*//**<CNcomment: 自由播放 */
    HI_UNF_SYNC_REF_AUDIO,    /**<Audio-based synchronization*//**<CNcomment: 以音频为准 */
    HI_UNF_SYNC_REF_VIDEO,    /**<Video-based synchronization*//**<CNcomment: 以视频为准 */
    HI_UNF_SYNC_REF_PCR,      /**<Program Clock Reference (PCR)-based synchronization*//**<CNcomment: 以PCR（Program Clock Reference）为准 */
    HI_UNF_SYNC_REF_SCR,      /**<System Clock Reference (SCR)-based synchronization*//**<CNcomment: 以SCR (System Clock Reference) 为准 */

    HI_UNF_AVPLAY_SYNC_REF_BUTT
} HI_UNF_SYNC_REF_E;

/**Defines the status of a buffer.*/
/**CNcomment:定义使用的缓冲区状态枚举类型 */
typedef enum hiUNF_AVPLAY_BUF_STATE_E
{
    HI_UNF_AVPLAY_BUF_STATE_EMPTY = 0,   /**<The buffer is idle.*//**<CNcomment: 缓冲区空闲 */
    HI_UNF_AVPLAY_BUF_STATE_LOW,         /**<The buffer usage is too low.*//**<CNcomment: 缓冲区占用率过低 */
    HI_UNF_AVPLAY_BUF_STATE_NORMAL,      /**<The buffer works normally.*//**<CNcomment: 缓冲区使用正常 */
    HI_UNF_AVPLAY_BUF_STATE_HIGH,        /**<The buffer usage is too high.*//**<CNcomment: 缓冲区占用率过高 */
    HI_UNF_AVPLAY_BUF_STATE_FULL,        /**<The buffer is full.*//**<CNcomment: 缓冲区已满 */

    HI_UNF_AVPLAY_BUF_STATE_BUTT
}HI_UNF_AVPLAY_BUF_STATE_E;

/**Defines the type of pts channel.*/
/** CNcomment:定义PTS通道类型 */
typedef enum hiUNF_SYNC_PTS_CHAN_E
{
    HI_UNF_SYNC_PTS_CHAN_VID,   /**<Video pts channel.*//**<CNcomment:视频PTS通道 */
    HI_UNF_SYNC_PTS_CHAN_AUD,   /**<Audio pts channel.*//**<CNcomment:音频PTS通道 */
    HI_UNF_SYNC_PTS_CHAN_PCR,   /**<Pcr channel.*//**<CNcomment:PCR通道 */

    HI_UNF_SYNC_PTS_CHAN_BUTT
}HI_UNF_SYNC_PTS_CHAN_E;

/**Defines the parameters of pts jump.*/
/** CNcomment:定义PTS跳变参数的结构体 */
typedef struct hiUNF_SYNC_PTSJUMP_PARAM_S
{
    HI_UNF_SYNC_PTS_CHAN_E  enPtsChan;  /**<Pts channel.*//**<CNcomment:PTS通道 */
    HI_BOOL                 bLoopback;  /**<Loopback or not.*//**<CNcomment:是否环回 */
    HI_U32                  u32FirstPts;/**<The first pts.*//**<CNcomment:第一个PTS */
    HI_U32                  u32FirstValidPts;/**<The first valid pts.*//**<CNcomment:第一个有效PTS */
    HI_U32                  u32CurSrcPts;   /**<The current src pts.*//**<CNcomment:当前原始PTS */
    HI_U32                  u32CurPts;      /**<The current pts.*//**<CNcomment:当前PTS */
    HI_U32                  u32LastSrcPts;  /**<The last src pts.*//**<CNcomment:上一个原始PTS */
    HI_U32                  u32LastPts;     /**<The last pts.*//**<CNcomment:上一个PTS */
}HI_UNF_SYNC_PTSJUMP_PARAM_S;

/**Defines the parameters of synchronization status change*/
/** CNcomment:定义同步状态变更参数的结构体 */
typedef struct hiUNF_SYNC_STAT_PARAM_S
{
     HI_S32          s32VidAudDiff;     /**<The diffrence between video and audio frames*//**<CNcomment: 音视频差值 */
     HI_S32          s32VidPcrDiff;     /**<The diffrence between video frame and pcr*//**<CNcomment: 视频PCR差值 */
     HI_S32          s32AudPcrDiff;     /**<The diffrence between audio frame and pcr*//**<CNcomment: 音频PCR差值 */
     HI_U32          u32VidLocalTime;   /**<Local video synchronization reference time*//**<CNcomment: 视频本地时间 */
     HI_U32          u32AudLocalTime;   /**<Local audio synchronization reference time*//**<CNcomment: 音频本地时间 */
     HI_U32          u32PcrLocalTime;   /**<Local pcr synchronization reference time*//**<CNcomment: PCR本地时间 */
}HI_UNF_SYNC_STAT_PARAM_S;

/*Type of the event callback function*/
/**CNcomment: 事件回调函数类型 */
typedef enum hiUNF_AVPLAY_EVENT_E
{
    HI_UNF_AVPLAY_EVENT_EOS,                   /**<The end of stream (EOS) operation is performed, NULL*//**<CNcomment: EOS执行结束, NULL.*/
    HI_UNF_AVPLAY_EVENT_STOP,                  /**<The stop operation is performed, NULL*//**<CNcomment: STOP执行结束, NULL.*/
    HI_UNF_AVPLAY_EVENT_RNG_BUF_STATE,         /**<Status change of the media buffer queue, HI_UNF_AVPLAY_BUF_STATE_E*//**<CNcomment: 媒体缓存队列状态变化, HI_UNF_AVPLAY_BUF_STATE_E.*/
    HI_UNF_AVPLAY_EVENT_NORM_SWITCH,           /**<Standard switch, HI_UNF_NORMCHANGE_PARAM_S*//**<CNcomment: 制式切换, HI_UNF_NORMCHANGE_PARAM_S .*/
    HI_UNF_AVPLAY_EVENT_FRAMEPACKING_CHANGE,   /**<3D Frame packing change, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E*//**<CNcomment: 3D帧类型变化, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E .*/
    HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME,         /**<New video frame, HI_UNF_VO_FRAMEINFO_S*//**<CNcomment: 新视频帧, HI_UNF_VO_FRAMEINFO_S .*/
    HI_UNF_AVPLAY_EVENT_NEW_AUD_FRAME,         /**<New audio frame, HI_UNF_AO_FRAMEINFO_S*//**<CNcomment: 新音频帧, HI_UNF_AO_FRAMEINFO_S .*/
    HI_UNF_AVPLAY_EVENT_NEW_USER_DATA,         /**<New video user data, HI_UNF_VIDEO_USERDATA_S*//**<CNcomment: 新视频用户数据, HI_UNF_VIDEO_USERDATA_S .*/
    HI_UNF_AVPLAY_EVENT_GET_AUD_ES,            /**<New audio ES data, HI_UNF_ES_BUF_S*//**<CNcomment: 新音频ES数据, HI_UNF_ES_BUF_S .*/
    HI_UNF_AVPLAY_EVENT_IFRAME_ERR,            /**<I frame decode error*//**<CNcomment: 解码I帧错误 .*/
    HI_UNF_AVPLAY_EVENT_SYNC_PTS_JUMP,         /**<Pts Jump, HI_UNF_SYNC_PTSJUMP_PARAM_S * *//**<CNcomment: PTS跳变, HI_UNF_SYNC_PTSJUMP_PARAM_S * .*/
    HI_UNF_AVPLAY_EVENT_SYNC_STAT_CHANGE,      /**<Synchronization status change, HI_UNF_SYNC_STAT_PARAM_S * *//**<CNcomment: 同步状态变更, HI_UNF_SYNC_STAT_PARAM_S * .*/
    HI_UNF_AVPLAY_EVENT_VID_BUF_STATE,         /**<Status change of the media buffer queue, HI_UNF_AVPLAY_EVENT_VID_BUF_STATE*//**<CNcomment: 视频缓存队列状态变化, HI_UNF_AVPLAY_EVENT_VID_BUF_STATE */
    HI_UNF_AVPLAY_EVENT_AUD_BUF_STATE,         /**<Status change of the media buffer queue, HI_UNF_AVPLAY_EVENT_AUD_BUF_STATE*//**<CNcomment: 音频缓存队列状态变化, HI_UNF_AVPLAY_EVENT_AUD_BUF_STATE */
    HI_UNF_AVPLAY_EVENT_VID_UNSUPPORT,         /**<The video stream is unsupport*//**<CNcomment: 视频码流不支持*/
    HI_UNF_AVPLAY_EVENT_VID_ERR_RATIO,         /**<frame error ratio *//**<CNcomment: 图像帧出错比例*/
    HI_UNF_AVPLAY_EVENT_AUD_INFO_CHANGE,       /**<audio info change, HI_UNF_ACODEC_STREAMINFO_S *//**<CNcomment: 音频信息变化，HI_UNF_ACODEC_STREAMINFO_S*/
    HI_UNF_AVPLAY_EVENT_AUD_UNSUPPORT,         /**<unsupported audio *//**<CNcomment: 不支持的音频*/
    HI_UNF_AVPLAY_EVENT_AUD_FRAME_ERR,         /**<audio frame error *//**<CNcomment: 音频帧出错*/
    HI_UNF_AVPLAY_EVENT_VID_ERR_TYPE,          /**<the setting vid codec type is error, HI_UNF_VCODEC_TYPE_E*//**<CNcomment: 设置的视频协议类型错误, HI_UNF_VCODEC_TYPE_E*/
    HI_UNF_AVPLAY_EVENT_FIRST_FRAME_DISPLAYED, /**<the first deocded frame have been displayed out by the tv.*//**<CNcomment: 第一帧已被显示*/
    HI_UNF_AVPLAY_EVENT_VID_ERR_FRAME,         /**<vid frame decode error.*/ /**<CNcomment: 解码视频帧错误*/
    HI_UNF_AVPLAY_EVENT_STATUS_REPORT,         /**<avplay status info, HI_UNF_AVPLAY_STATUS_INFO_S*/ /**<CNcomment: 播放器状态信息*/    
    HI_UNF_AVPLAY_EVENT_VID_FRAME_INFO,        /**<Video frame status info, HI_UNF_VIDEO_FRM_STATUS_INFO_S*/ /**<CNcomment: 视频帧状态信息*/
    HI_UNF_AVPLAY_EVENT_BUTT
} HI_UNF_AVPLAY_EVENT_E;

/**Mode of processing the buffer overflow*/
/**CNcomment: 缓冲溢出处理类型  */
typedef enum hiUNF_AVPLAY_OVERFLOW_E
{
    HI_UNF_AVPLAY_OVERFLOW_RESET,              /**<Reset during overflow*//**<CNcomment: 溢出时进行复位  */
    HI_UNF_AVPLAY_OVERFLOW_DISCARD,            /**<Discard during overflow*//**<CNcomment: 溢出时进行丢弃  */
    HI_UNF_AVPLAY_OVERFLOW_BUTT
} HI_UNF_AVPLAY_OVERFLOW_E;

/**Defines the type of the event callback function.*/
/**CNcomment: 定义事件回调函数枚举类型 */
typedef HI_S32 (*HI_UNF_AVPLAY_EVENT_CB_FN)(HI_HANDLE hAvplay, HI_UNF_AVPLAY_EVENT_E enEvent, HI_U32 u32Para);

/**Defines the type of the event callback function.*/
/**CNcomment: 定义事件回调函数枚举类型 */
typedef HI_S32 (*HI_UNF_AVPLAY_EVENT_CB_FN64)(HI_HANDLE hAvplay, HI_UNF_AVPLAY_EVENT_E enEvent, HI_VOID *pPara);

/**Defines the attribute of audio security.*/
/**CNcomment: 定义音频安全属性结构体*/
typedef struct hiUNF_AVPLAY_AUD_SECURITY_ATTR_S
{
    HI_BOOL    bEnable; /**<Audio dmx channel is security mode or not*//**<CNcomment: 音频DMX通道是否为安全模式*/
}HI_UNF_AVPLAY_AUD_SECURITY_ATTR_S;

/**Defines the attribute ID of an AVPLAY.*/
/**CNcomment: 定义AV播放器属性ID枚举类型 */
typedef enum hiUNF_AVPLAY_ATTR_ID_E
{
    HI_UNF_AVPLAY_ATTR_ID_STREAM_MODE = 0,  /**<Playing mode, HI_UNF_AVPLAY_ATTR_S*//**<CNcomment: 播放模式, HI_UNF_AVPLAY_ATTR_S.*/

    HI_UNF_AVPLAY_ATTR_ID_ADEC,             /**<Audio attribute, HI_UNF_ACODEC_ATTR_S*//**<CNcomment: 音频属性, HI_UNF_ACODEC_ATTR_S.*/
    HI_UNF_AVPLAY_ATTR_ID_VDEC,             /**<Video attribute, HI_UNF_VCODEC_ATTR_S*//**<CNcomment: 视频属性, HI_UNF_VCODEC_ATTR_S.*/

    HI_UNF_AVPLAY_ATTR_ID_AUD_PID,          /**<Audio packet identifier (PID), HI_U32*//**<CNcomment: 音频PID, HI_U32.*/
    HI_UNF_AVPLAY_ATTR_ID_VID_PID,          /**<Video PID, HI_U32*//**<CNcomment: 视频PID, HI_U32.*/
    HI_UNF_AVPLAY_ATTR_ID_PCR_PID,          /**<PCR PID, HI_U32*//**<CNcomment: PCR PID, HI_U32.*/

    HI_UNF_AVPLAY_ATTR_ID_SYNC,             /**<Synchronization attribute, HI_UNF_SYNC_ATTR_S*//**<CNcomment: 同步属性, HI_UNF_SYNC_ATTR_S.*/
    HI_UNF_AVPLAY_ATTR_ID_AFD,              /**<Whether to enable the active format descriptor (AFD), HI_BOOL* *//**<CNcomment: AFD 是否开启， HI_BOOL * .*/
    HI_UNF_AVPLAY_ATTR_ID_OVERFLOW,         /**<Overflow processing type, HI_UNF_AVPLAY_OVERFLOW_E* *//**<CNcomment: 溢出处理类型 , HI_UNF_AVPLAY_OVERFLOW_E * .*/

    HI_UNF_AVPLAY_ATTR_ID_MULTIAUD,         /**<Multiple audio attribute,  HI_UNF_AVPLAY_MULTIAUD_ATTR_S **//**<CNcomment: 多音轨属性, HI_UNF_AVPLAY_MULTIAUD_ATTR_S * .*/
    HI_UNF_AVPLAY_ATTR_ID_FRMRATE_PARAM,    /**<Frame Rate Parameter, HI_UNF_AVPLAY_FRMRATE_PARAM_S * *//**<CNcomment:帧率参数,HI_UNF_AVPLAY_FRMRATE_PARAM_S * .*/
    HI_UNF_AVPLAY_ATTR_ID_FRMPACK_TYPE,     /**<3D Frame Packing Type, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E * *//**<CNcomment:3D帧的打包类型,HI_UNF_VIDEO_FRAME_PACKING_TYPE_E * .*/
    HI_UNF_AVPLAY_ATTR_ID_LOW_DELAY,        /**<Low Delay Attr, HI_UNF_AVPLAY_LOW_DELAY_ATTR_S * *//**<CNcomment: 低延时属性 , HI_UNF_AVPLAY_LOW_DELAY_ATTR_S * .*/
    HI_UNF_AVPLAY_ATTR_ID_TVP,              /**<Trusted Video Path Attr, HI_UNF_AVPLAY_TVP_ATTR_S * *//**<CNcomment: 安全视频通路属性 , HI_UNF_AVPLAY_TRUST_VIDEO_PATH_ATTR_S * .*/
    HI_UNF_AVPLAY_ATTR_ID_AD,               /**<MS12 Attr, HI_UNF_AVPLAY_AD_ATTR_S * *//**<CNcomment: MS12通路属性 , HI_UNF_AVPLAY_AD_ATTR_S * .*/
    HI_UNF_AVPLAY_ATTR_ID_HDR,              /**<HDR Attr, HI_UNF_AVPLAY_HDR_ATTR_S * *//**<CNcomment: HDR通路属性 , HI_UNF_AVPLAY_HDR_ATTR_S * .*/    
    HI_UNF_AVPLAY_ATTR_ID_AUD_SECURITY,     /**<Dmx audio channel security mode, HI_UNF_AVPLAY_AUD_SECURITY_ATTR_S * *//**<CNcomment: 音频安全属性 , HI_UNF_AVPLAY_AUD_SECURITY_ATTR_S * .*/
    HI_UNF_AVPLAY_ATTR_ID_BUTT
} HI_UNF_AVPLAY_ATTR_ID_E;

/**Defines the stream type of HDR.*/
/**CNcomment: 定义DOLBY HDR码流类型 */
typedef enum hiUNF_AVPLAY_HDR_STREAM_TYPE_E
{
    HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_SINGLE_VES = 0,    /**<Dolby Vision Dual layer Single VES Format*/ /**<CNcomment: 单流双层格式.*/
    HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_DUAL_VES,          /**<Dolby Vision Dual Layer Dual VES Format*/ /**<CNcomment: 双流双层格式.*/
    HI_UNF_AVPLAY_HDR_STREAM_TYPE_SL_VES,               /**<Dolby Vision Single Layer VES Format*/ /**<CNcomment: 单流单层格式.*/
    HI_UNF_AVPLAY_HDR_STREAM_TYPE_BUTT
} HI_UNF_AVPLAY_HDR_STREAM_TYPE_E;

/**Defines the attribute of low delay.*/
/**CNcomment: 定义低延时属性结构体*/
typedef struct hiUNF_AVPLAY_LOW_DELAY_ATTR_S
{
    HI_BOOL               bEnable;  /**<Is low delay enable or not*//**<CNcomment: 低延时是否使能*/
}HI_UNF_AVPLAY_LOW_DELAY_ATTR_S;

/**Defines the attribute of trust video path.*/
/**CNcomment: 定义安全视频通路属性结构体*/
typedef struct hiUNF_AVPLAY_TVP_ATTR_S
{
    HI_BOOL               bEnable;  /**<Is trusted video path enable or not*//**<CNcomment: 安全视频通路是否使能*/
}HI_UNF_AVPLAY_TVP_ATTR_S;

/**Defines the audio/video synchronization .*/
/**CNcomment: 定义音视频同步调整区间 */
typedef struct hiUNF_SYNC_REGION_S
{
    HI_S32                s32VidPlusTime;        /**<Plus time range during video synchronization*//**<CNcomment: 视频同步超前的时间范围 */
    HI_S32                s32VidNegativeTime;    /**<Negative time range during video synchronization*//**<CNcomment: 视频同步落后的时间范围 */
    HI_BOOL               bSmoothPlay;           /**<Slow playing enable*//**<CNcomment: 慢放使能 */
} HI_UNF_SYNC_REGION_S;

/**Defines the audio/video synchronization attributes.*/
/**CNcomment: 定义音视频同步属性 */
typedef struct hiUNF_SYNC_ATTR_S
{
    HI_UNF_SYNC_REF_E     enSyncRef;             /**<Audio-based synchronization, PCR-based synchronization, and free playing without synchronization*//**<CNcomment: 音频为准，PCR为准，自由播放等 */
    HI_UNF_SYNC_REGION_S  stSyncStartRegion;     /**<Synchronization start region*//**<CNcomment: 同步起调区间 */
    HI_UNF_SYNC_REGION_S  stSyncNovelRegion;     /**<Synchronization exception region*//**<CNcomment: 同步异常区间 */
    HI_S32                s32VidPtsAdjust;       /**<Video presentation time stamp (PTS) adjustment*//**<CNcomment: 视频PTS调整 */
    HI_S32                s32AudPtsAdjust;       /**<Audio PTS adjustment*//**<CNcomment: 音频PTS调整 */

    HI_U32                u32PreSyncTimeoutMs;   /**<Pre-synchronization timeout, in ms*//**<CNcomment: 预同步的超时时间，单位为毫秒 */
    HI_BOOL               bQuickOutput;          /**<Fast output enable*//**<CNcomment: 快速输出使能 */
} HI_UNF_SYNC_ATTR_S;

/**Defines the playing attributes of an AVPLAY.*/
/**CNcomment: 定义AV播放属性 */
typedef struct hiUNF_AVPLAY_ATTR_S
{
    HI_U32                       u32DemuxId;   /**<ID of the DEMUX used by an AVPLAY*//**<CNcomment: AVPLAY所使用的DEMUX ID 仅当码流类型为TS时有效 */
    HI_UNF_AVPLAY_STREAM_ATTR_S  stStreamAttr; /**<Stream attributes*//**<CNcomment: 码流属性 */
} HI_UNF_AVPLAY_ATTR_S;

/**Defines the synchronization status when an AVPLAY is running.*/
/**CNcomment: 定义播放器运行状态信息中同步状态信息类型 */
typedef struct hiUNF_SYNC_STATUS_S
{
    HI_U32 u32FirstAudPts;    /**<PTS of the first audio frame*//**<CNcomment: 第一个音频帧 PTS .*/
    HI_U32 u32FirstVidPts;    /**<PTS of the first video frame*//**<CNcomment: 第一个视频帧 PTS .*/
    HI_U32 u32LastAudPts;     /**<PTS of the last audio frame*//**<CNcomment: 最近播放的一个音频帧 PTS .*/
    HI_U32 u32LastVidPts;     /**<PTS of the last video frame*//**<CNcomment: 最近播放的一个视频帧 PTS .*/
    HI_S32 s32DiffAvPlayTime; /**<Playing time difference between audio and video frames*//**<CNcomment: 音视频 播放时差 .*/
    HI_U32 u32PlayTime;       /**<Playing time*//**<CNcomment: 当前已播放时间 .*/
    HI_U32 u32LocalTime;      /**<Local synchronization reference time*//**<CNcomment: 本地同步参考时间 .*/
} HI_UNF_SYNC_STATUS_S;

/**Defines the status of a media buffer.*/
/**CNcomment:定义媒体缓冲区的状态信息 */
typedef struct hiUNF_AVPLAY_BUF_STATUS_S
{
    HI_U32 u32BufId;                /**<Media buffer ID*//**<CNcomment: 媒体缓冲 标识 */
    HI_U32 u32BufSize;              /**<Media buffer size*//**<CNcomment: 媒体缓冲大小 */
    HI_U32 u32BufRptr;              /*Read pointer of the media buffer. This pointer is valid when TSs are being played.*//**<CNcomment: 媒体缓冲读指针,Ts播放时有效 */
    HI_U32 u32BufWptr;              /*Write pointer of the media buffer. This pointer is valid when TSs are being played.*//**<CNcomment: 媒体缓冲写指针,Ts播放时有效 */
    HI_U32 u32UsedSize;             /**<Used size of the media buffer*//**<CNcomment: 媒体缓冲已使用大小 */
    HI_U32 u32CurrentAvailableSize; /**<Current available size of the media buffer for HI_UNF_AVPLAY_GetBuf()*//**<CNcomment: 媒体缓冲可使用大小 */
    HI_U32 u32FrameBufTime;         /**<Frame buffer time*//**<CNcomment: 帧缓冲时间 */
    HI_U32 u32FrameBufNum;          /**<The number of frames in frame buffer*//**<CNcomment: 帧缓冲数目 仅VIDEO有效 */
    HI_BOOL bEndOfStream;           /**<Flag to indicate end of stream*//**<CNcomment: 缓冲中码流解码完毕标识 仅VIDEO有效 */
} HI_UNF_AVPLAY_BUF_STATUS_S;

/**Defines the playing status of an AVPLAY.*/
/**CNcomment:定义AV的播放状态 */
typedef enum hiUNF_AVPLAY_STATUS_E
{
    HI_UNF_AVPLAY_STATUS_STOP = 0,  /**<Stop*/      /**<CNcomment: 停止 */
    HI_UNF_AVPLAY_STATUS_PREPLAY,   /**<Buffer*/    /**<CNcomment: 缓冲 */
    HI_UNF_AVPLAY_STATUS_PLAY,      /**<Play*/      /**<CNcomment: 播放 */
    HI_UNF_AVPLAY_STATUS_TPLAY,     /**<Trick play, such as fast forward and rewind*/   /**<CNcomment: TPlay, 快进快退 */
    HI_UNF_AVPLAY_STATUS_PAUSE,     /**<Pause*/     /**<CNcomment: 暂停 */
    HI_UNF_AVPLAY_STATUS_EOS,       /**<EOS*/       /**<CNcomment: 码流播放结束 */
    HI_UNF_AVPLAY_STATUS_SEEK ,     /**<Seek play*/ /**<CNcomment: 定位播放 */

    HI_UNF_AVPLAY_STATUS_BUTT
} HI_UNF_AVPLAY_STATUS_E;

/**Defines the output value after the playing status of an AVPLAY is queried.*/
/**CNcomment:定义AV播放状态查询输出值类型 */
typedef struct hiUNF_AVPLAY_STATUS_INFO_S
{
    HI_UNF_SYNC_STATUS_S       stSyncStatus;      /**<Synchronization status*//**<CNcomment: 同步状态 */
    HI_UNF_AVPLAY_STATUS_E     enRunStatus;       /**<Running status*//**< CNcomment:运行状态 */
    HI_U32                     u32VidFrameCount;  /**<Count of played video frames*//**<CNcomment: 视频已播放帧数 */
    HI_U32                     u32AuddFrameCount; /**<Count of played audio frames*//**<CNcomment: 音频已播放帧数 */
    HI_UNF_AVPLAY_BUF_STATUS_S stBufStatus[HI_UNF_AVPLAY_BUF_ID_BUTT]; /**<Status of the media buffer*//**<CNcomment:媒体缓冲状态 */
    HI_U32                     u32VidErrorFrameCount; /**<Number of error frames during video decoding*/ /**<CNcomment: 视频解码错误帧数 */
} HI_UNF_AVPLAY_STATUS_INFO_S;

/**Defines the information about the playing program streams to be queried.*/
/**CNcomment: 定义播放节目流查询信息类型 */
typedef struct hiUNF_AVPLAY_STREAM_INFO_S
{
    HI_UNF_VCODEC_STREAMINFO_S stVidStreamInfo; /**<Video stream information*//**<CNcomment:视频流信息 */
    HI_UNF_ACODEC_STREAMINFO_S stAudStreamInfo; /**<Audio stream information*//**<CNcomment:音频流信息 */
} HI_UNF_AVPLAY_STREAM_INFO_S;

/**Defines the information about an I frame.*//**CNcomment:定义I帧数据信息类型 */
typedef struct hiUNF_AVPLAY_I_FRAME_S
{
    HI_U8                *pu8Addr;    /**<User-state virtual address of a frame*//**<CNcomment:帧数据用户态虚拟地址 */
    HI_U32               u32BufSize; /**<Frame size, in byte*//**<CNcomment:帧数据大小，单位字节 */
    HI_UNF_VCODEC_TYPE_E enType;     /*Protocol type of a data segment*//**<CNcomment:该片数据的协议类型 */
} HI_UNF_AVPLAY_I_FRAME_S;

/**Defines the decoder type. The occupied memory varies according to decoders.*/
/**CNcomment:定义解码器类型 不同类型的解码器占用内存不同 */
typedef enum hiHI_UNF_VCODEC_DEC_TYPE_E
{
    HI_UNF_VCODEC_DEC_TYPE_NORMAL,            /**<Normal type.*//**<CNcomment:普通类型 */

    /**<I frame decoding type. If an AVPLAY is used to decode I frames only (HI_UNF_AVPLAY_DecodeIFrame), you can select this type to reduce the memory usage.*/
    /**<CNcomment:I帧解码类型 如果avplay仅用于I帧解码(HI_UNF_AVPLAY_DecodeIFrame) 设置为此类型可以节省内存 */
    HI_UNF_VCODEC_DEC_TYPE_ISINGLE,

    HI_UNF_VCODEC_DEC_TYPE_BUTT
}HI_UNF_VCODEC_DEC_TYPE_E;

/**Defines the level of the protocol supported by the decoder. This value affects the number of frame buffers allocated by the normal decoder.
The greater the value, the more the required frame buffers.*/
/**CNcomment:定义解码器支持的协议级别 影响NORMAL类型解码器分配的帧存个数 值越大需要的帧存数目越多 */
typedef enum hiHI_UNF_VCODEC_PRTCL_LEVEL_E
{
    HI_UNF_VCODEC_PRTCL_LEVEL_MPEG = 0,     /**<Protocols excluding the H.264 protocol*//**<CNcomment:除h264外的其他协议 */
    HI_UNF_VCODEC_PRTCL_LEVEL_H264 = 1,     /**<H.264 protocol and other protocols*//**<CNcomment:h264协议 */
    HI_UNF_VCODEC_PRTCL_LEVEL_MVC,
    HI_UNF_VCODEC_PRTCL_LEVEL_BUTT
}HI_UNF_VCODEC_PRTCL_LEVEL_E;

/**Defines the attributes when an AVPLAY enables the video decoder. The settings affect the memory occupied by the video decoder and decoding performance.*/
/**CNcomment:定义AV播放器打开视频解码器时属性设置结构体 影响视频解码器占用内存大小及解码能力 */
typedef struct hiHI_UNF_AVPLAY_OPEN_OPT_S
{
    HI_UNF_VCODEC_DEC_TYPE_E    enDecType;       /**<Decoder type.*//**<CNcomment:解码器类型*/
    HI_UNF_VCODEC_CAP_LEVEL_E   enCapLevel;      /**<Maximum resolution supported by the decoder. This value affects the size of each frame buffer.*//**<CNcomment:解码器支持的最大分辨率 影响每个帧存的大小 */
    HI_UNF_VCODEC_PRTCL_LEVEL_E enProtocolLevel; /**<Supported protocol level. This value affects the number of frame buffers.*//**<CNcomment:支持的协议级别 影响帧存数目 */
}HI_UNF_AVPLAY_OPEN_OPT_S;

/**Defines the attribute of MS12.*/
/**CNcomment: 定义音频MS12属性结构体*/
typedef struct hiUNF_AVPLAY_AD_ATTR_S
{
    HI_BOOL             bAdEnble;       /**whether enable ad service (on/off)*/ /**<CNcomment: 是否使能AD.*/
    HI_BOOL             bAdToSomePort;  /**whether enable ad only to some port,such as speaker/headphone etc*/ /**<CNcomment: 是否使能所有输出都混音，默认为false.*/
    HI_U32              u32AdPid;       /**AD program id*/ /**<CNcomment: AD流PID.*/
    HI_S16              s16AdBalance;   /**AD Balance*/ /**<CNcomment: AD音量.*/
} HI_UNF_AVPLAY_AD_ATTR_S;

/**Defines the attribute of HDR.*/
/**CNcomment: 定义HDR属性结构体*/
typedef struct hiUNF_AVPLAY_HDR_ATTR_S
{
    HI_BOOL                           bEnable;          /**<Is Dolby HDR enable or not*/ /**<CNcomment: Dolby HDR是否使能.*/
    HI_UNF_AVPLAY_HDR_STREAM_TYPE_E   enHDRStreamType;  /**<Dolby Vision VES Format*/ /**<CNcomment: Dolby HDR流类型.*/
    HI_UNF_VCODEC_TYPE_E              enElType;         /**<Video encoding type*/ /**<CNcomment: EL视频编码类型*/
    HI_UNF_AVPLAY_OPEN_OPT_S          stElCodecOpt;     /**<Video decoding option*/ /**<CNcomment: 视频解码能力级设置*/
    HI_BOOL                           bCompatible;      /**<Dolby frame is compatible or not.*/ /**<CNcomment: Dolby帧是否兼容.*/
} HI_UNF_AVPLAY_HDR_ATTR_S;

/**Defines the video display mode after an AVPLAY is stopped.*/
/**CNcomment:定义AV播放器停止时视频显示模式 */
typedef enum hiUNF_AVPLAY_STOP_MODE_E
{
    HI_UNF_AVPLAY_STOP_MODE_STILL = 0,  /**<The last frame is still after an AVPLAY is stopped.*//**<CNcomment:stop后保留最后一帧 */
    HI_UNF_AVPLAY_STOP_MODE_BLACK = 1,  /**<The blank screen is displayed after an AVPLAY is stopped.*//**<CNcomment:stop后黑屏 */
    HI_UNF_AVPLAY_STOP_MODE_BUTT
} HI_UNF_AVPLAY_STOP_MODE_E;


/**Defines the attributes when an AVPLAY is prestarted.*/
/**CNcomment:定义AV播放器预启动时属性设置结构体 */
typedef struct hiAVPLAY_PRESTART_OPT_S
{
    HI_U32       u32Reserved;
} HI_UNF_AVPLAY_PRESTART_OPT_S;

/**Defines the attributes when an AVPLAY is started.*/
/**CNcomment:定义AV播放器启动时属性设置结构体 */
typedef struct hiAVPLAY_START_OPT_S
{
    HI_U32       u32Reserved;
} HI_UNF_AVPLAY_START_OPT_S;

/**Defines the attributes when an AVPLAY is prestoped.*/
/**CNcomment:定义AV播放器预停止时属性设置结构体 */
typedef struct hiAVPLAY_PRESTOP_OPT_S
{
    HI_U32       u32Reserved;
} HI_UNF_AVPLAY_PRESTOP_OPT_S;


/**Defines the attributes when an AVPLAY is stopped.*/
/**CNcomment:定义AV播放器停止时属性设置结构体 */
typedef struct hiAVPLAY_STOP_OPT_S
{
    /*
         s32Timeout: end of stream timeout
         s32Timeout = 0   Wait until streams are played in non-block mode, that is, the interface is returned immediately. CNcomment:非阻塞等待码流播放结束，立即返回 CNend
         s32Timeout > 0   Block timeout, in ms, CNcomment:阻塞超时时间，单位为毫秒 CNend
         s32Timeout = -1  Infinite wait,CNcomment:无限等待 CNend
     */
    HI_U32                    u32TimeoutMs;    /**<Timeout*//**<CNcomment:超时值 */
    HI_UNF_AVPLAY_STOP_MODE_E enMode;          /**<Video display mode*//**<CNcomment:视频显示模式 */
} HI_UNF_AVPLAY_STOP_OPT_S;

/*Defines the attributes when an AVPLAY is paused.*/
/**CNcomment:定义AV播放器暂停时属性设置结构体 */
typedef struct hiAVPLAY_PAUSE_OPT_S
{
    HI_U32       u32Reserved;
} HI_UNF_AVPLAY_PAUSE_OPT_S;

/**Defines the direction of tplay*/
/**CNcomment:定义AV播放器TPLAY的方向 */
typedef enum hiUNF_AVPLAY_TPLAY_DIRECT_E
{
    HI_UNF_AVPLAY_TPLAY_DIRECT_FORWARD,     /**<Tplay forward*//**<CNcomment: 向前TPLAY. */
    HI_UNF_AVPLAY_TPLAY_DIRECT_BACKWARD,    /**<Tplay backward*//**<CNcomment: 向后TPLAY. */
    HI_UNF_AVPLAY_TPLAY_DIRECT_BUTT

} HI_UNF_AVPLAY_TPLAY_DIRECT_E;

/**Defines the attributes when the playing mode of an AVPLAY is tplay.*/
/**CNcomment:定义AV播放器TPALY时属性设置结构体 */
typedef struct hiAVPLAY_TPLAY_OPT_S
{
    HI_UNF_AVPLAY_TPLAY_DIRECT_E    enTplayDirect;      /**<Tplay direction*//**<CNcomment: TPLAY方向 */
    HI_U32                          u32SpeedInteger;    /**<Integral part of tplay speed*//**<CNcomment: TPLAY倍数的整数部分 */
    HI_U32                          u32SpeedDecimal;    /**<Fractional part (calculated to three decimal places) of tplay speed*//**<CNcomment: TPLAY倍数的小数部分，保留3位小数 */
} HI_UNF_AVPLAY_TPLAY_OPT_S;

/**Defines the attributes when an AVPLAY is resumed.*/
/**CNcomment:定义AV播放器恢复时属性设置结构体 */
typedef struct hiAVPLAY_RESUME_OPT_S
{
    HI_U32       u32Reserved;
} HI_UNF_AVPLAY_RESUME_OPT_S;

/**Defines the attributes when an AVPLAY is reset.*/
/**CNcomment:定义AV播放器复位时属性设置结构体 */
typedef struct hiAVPLAY_RESET_OPT_S
{
    HI_U32        u32SeekPtsMs;   /**<clear these datas which pts is smaller than u32SeekPtsMs in buffer *//**<CNcomment:清空buffer内u32SeekPtsMs之前的数据 */
} HI_UNF_AVPLAY_RESET_OPT_S;

/**Defines the attributes when an AVPLAY is step.*/
/**CNcomment:定义AV播放器步进时属性设置结构体 */
typedef struct hiUNF_AVPLAY_STEP_OPT_S
{
    HI_U32       u32Reserved;
}HI_UNF_AVPLAY_STEP_OPT_S;

/**Defines the attributes when an AVPLAY is in Flushing Stream Status.*/
/**CNcomment:定义AV播放器Flush Stream时属性设置结构体 */
typedef struct hiUNF_AVPLAY_FLUSH_STREAM_OPT_S
{
    HI_U32          u32Reserved;
}HI_UNF_AVPLAY_FLUSH_STREAM_OPT_S;

/**Defines the attributes of multiple audio.*/
/**CNcomment:定义多音轨属性结构体 */
typedef struct hiUNF_AVPLAY_MULTIAUD_ATTR_S
{
    HI_U32                  u32PidNum;      /**<the number of Audio PID *//**< CNcomment:音频PID个数 */
    HI_U32                  *pu32AudPid;    /**<Pointer to the array of audio PID*//**< CNcomment:指向PID数组的指针 */
    HI_UNF_ACODEC_ATTR_S    *pstAcodecAttr; /**<Pointer to the array of audio attribute*//**< CNcomment:指向音频属性数组的指针 */
}HI_UNF_AVPLAY_MULTIAUD_ATTR_S;

/**Defines the source of frame rate.*/
/**CNcomment: 定义帧率来源类型的枚举 */
typedef enum hiUNF_AVPLAY_FRMRATE_TYPE_E
{
    HI_UNF_AVPLAY_FRMRATE_TYPE_PTS,         /**<Use the frame rate calculates from PTS*//**<CNcomment: 采用PTS计算帧率 */
    HI_UNF_AVPLAY_FRMRATE_TYPE_STREAM,      /**<Use the frame rate comes from stream*//**<CNcomment: 采用码流信息中的帧率 */
    HI_UNF_AVPLAY_FRMRATE_TYPE_USER,        /**<Use the frame rate set by user*//**<CNcomment: 采用用户设置的帧率 */
    HI_UNF_AVPLAY_FRMRATE_TYPE_USER_PTS,    /**<Use the frame rate set by user until the 2nd I frame comes, then use the frame rate calculates from PTS*//**<CNcomment: 第二个I帧来之前采用用户设置的帧率，之后根据PTS计算帧率 */
    HI_UNF_AVPLAY_FRMRATE_TYPE_BUTT
}HI_UNF_AVPLAY_FRMRATE_TYPE_E;

/**Defines the parameter of frame rate.*/
/**CNcomment: 定义帧率属性参数的结构体 */
typedef struct hiUNF_AVPLAY_FRMRATE_PARAM_S
{
    HI_UNF_AVPLAY_FRMRATE_TYPE_E    enFrmRateType;  /**<The source of frame rate*//**<CNcomment: 帧率来源类型 */
    HI_UNF_VCODEC_FRMRATE_S         stSetFrmRate;   /**<Setting frame rate*//**<CNcomment: 设置的帧率 */
}HI_UNF_AVPLAY_FRMRATE_PARAM_S;

/**Defines commond to get vdec information, the parameter is HI_UNF_AVPLAY_VDEC_INFO_S.*/
/**CNcomment: 获取解码器信息命令，参数对应类型为HI_UNF_AVPLAY_VDEC_INFO_S */
#define HI_UNF_AVPLAY_GET_VDEC_INFO_CMD         0x20
/**Defines commond to set TPLAY parameter, the parameter is HI_UNF_AVPLAY_TPLAY_OPT_S.*/
/**CNcomment: 设置TPLAY参数命令，参数对应类型为HI_UNF_AVPLAY_TPLAY_OPT_S*/
#define HI_UNF_AVPLAY_SET_TPLAY_PARA_CMD        0x21
/**Defines commond to set special control information of stream, the parameter is HI_UNF_AVPLAY_CONTROL_INFO_S*/
/**CNcomment: 用来设置一些码流的特殊控制信息，参数对应类型为HI_UNF_AVPLAY_CONTROL_INFO_S*/
#define HI_UNF_AVPLAY_SET_CTRL_INFO_CMD         0x22

/**Defines commond to set video sample type, HI_BOOL *, HI_TRUE: force Progressive, HI_FALSE: auto recognise Progressive or Interlance */
/**CNcomment: 设置视频逐行信息, HI_TRUE: 强制逐行, HI_FALSE: 自动识别逐隔行*/
#define HI_UNF_AVPLAY_SET_PROGRESSIVE_CMD       0x23

/**Defines commond to set video color space, the parameter is HI_UNF_COLOR_SPACE_E*/
/**CNcomment: 设置视频色彩空间, 参数对应类型为HI_UNF_COLOR_SPACE_E*/
#define HI_UNF_AVPLAY_SET_COLORSPACE_CMD        0x24

/**Defines commond to set dpb full control, HI_BOOL* ,HI_TRUE:force delete min poc frame when dpb is full,HI_FALSE:return error when dpb is full*/
/**CNcomment:设置dpb满的时候的处理策略, 参数对应类型为HI_BOOL*/
#define HI_UNF_AVPLAY_SET_DPBFULL_CTRL_CMD      0x25

/***fix  videophone scene by AVPLAY ,  format MJPEG**/
#define HI_UNF_AVPLAY_SET_SCENE_MODE_CMD        0x26

/**Defines the mode of videophone.*/
/**CNcomment: 定义Videophone 场景类型的枚举 */
typedef enum hiUNF_AVPLAY_SCENE_MODE_E
{
   HI_UNF_AVPLAY_SCENE_MODE_NORMAL,     /**<Normal scene of non-voip*//**<CNcomment: 非可视电视场景 */
   HI_UNF_AVPLAY_SCENE_MODE_VIDEOPHONE_PREVIEW,   /**<Voip scene of the local end*//**<CNcomment: 可视电话近端场景 */
   HI_UNF_AVPLAY_SCENE_MODE_VIDEOPHONE_REMOTE,   /**<Voip scene of the remote end*//**<CNcomment: 可视电话远端场景 */
   HI_UNF_AVPLAY_SCENE_MODE_BUTT,
} HI_UNF_AVPLAY_SCENE_MODE_E;


/**Defines the type of AVPLAY invoke.*/
/**CNcomment: 定义AVPLAY Invoke调用类型的枚举 */
typedef enum hiUNF_AVPLAY_INVOKE_E
{
    HI_UNF_AVPLAY_INVOKE_ACODEC  = 0,   /**<Invoke commond to control audio codec*//**<CNcomment: 控制音频解码器的Invoke调用 */
    HI_UNF_AVPLAY_INVOKE_VCODEC,        /**<Invoke commond to control video codec, HI_CODEC_VIDEO_CMD_S*//**<CNcomment: 控制视频解码器的Invoke调用 */
    HI_UNF_AVPLAY_INVOKE_GET_PRIV_PLAYINFO, /**<Invoke commond to get private play infomation,the parameter is HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S*//**<CNcomment: 获取私有播放信息的Invoke调用， 参数为HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S * */
    HI_UNF_AVPLAY_INVOKE_SET_DISP_OPTIMIZE_FLAG, /**Defines commond to set Display Optimize Flag, The Parameter is HI_U32, 1: Enable, 0: Disable */
    HI_UNF_AVPLAY_INVOKE_GET_GLOBAL_PLAYINFO,   /**<Get global play information of avplay*//**<CNcomment: 获取AVPLAY全局播放信息 */
    HI_UNF_AVPLAY_INVOKE_SET_SYNC_MODE,  /**<Invoke commond to set sync mode, HI_U32, 0 normal sync, 1 use sync replace frc*//**<CNcomment: 设置同步模式，HI_U32，0: 正常模式, 1: 使用同步替代帧率转换*/
    HI_UNF_AVPLAY_INVOKE_BUTT
} HI_UNF_AVPLAY_INVOKE_E;

/**Defines the decoding information of video codec.*/
/**CNcomment: 定义VDEC解码信息的结构体 */
typedef struct hiUNF_AVPLAY_VDEC_INFO_S
{
    HI_U32                  u32DispFrmBufNum;   /**<the number of display frame*//**<CNcomment: 显示帧存个数 */
    HI_U32                  u32FieldFlag;       /**<The encoding mode of image, 0 frame mode, 1 filed mode*//**<CNcomment: 图像编码方式, 0 帧模式，1 场模式 */
    HI_UNF_VCODEC_FRMRATE_S stDecFrmRate;      /**<decoding frame rate*//**<CNcomment: 解码帧率 */
    HI_U32                  u32UndecFrmNum;     /**<the number of undecoded frame*//**<CNcomment: 未解码帧个数 */
}HI_UNF_AVPLAY_VDEC_INFO_S;

/**Defines the private status information.*/
/**CNcomment: 定义AVPLAY私有状态信息 */
typedef struct hiUNF_AVPLAY_PRIVATE_STATUS_INFO_S
{
    HI_U32 u32LastPts;   /**<PTS of the last audio or video frame*/ /**<CNcomment: 最近播放的一个音频帧 PTS或视频PTS*/
    HI_U32 u32LastPlayTime; /**< PlayTime of the last audio or video frame */ /**<CNcomment: 最近播放的一个音频帧 PlayTime或视PlayTime  */
    HI_U32 u32DispOptimizeFlag; /**<Display Optimize Flag,1: Enable, 0: Disable*//**<CNcomment: 显示优化标志*/
} HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S;

/**Defines the special control information of stream.*/
/**CNcomment: 定义特殊控制信息 */
typedef struct hiUNF_AVPLAY_CONTROL_INFO_S
{
    HI_U32 u32IDRFlag;               /**<IDR frame Flag, 1 means IDR(instantaneous decoding refresh) frame.*/ /**<CNcomment: 是否是IDR(此帧前后无参考关系)帧，1表示是*/
    HI_U32 u32BFrmRefFlag;           /**<Whether B frame is refer frame, 1 means B frame is refer frame.*/ /**<CNcomment: B帧是否是参考帧，1表示是*/
    HI_U32 u32ContinuousFlag;        /**<Whether send frame is continusous. 1 means continusous*/ /**<CNcomment: 帧是否连续，1表示连续*/
    HI_U32 u32BackwardOptimizeFlag;  /**<The Backward Optimize Flag*//**<CNcomment: 快退优化使能标志.*/
    HI_U32 u32DispOptimizeFlag;      /**<Display Optimize Flag,1: Enable, 0: Disable*//**<CNcomment: 显示优化标志*/
}HI_UNF_AVPLAY_CONTROL_INFO_S;

/**Defines the parameter when the stream is send by HI_UNF_AVPLAY_PutBufEx.*/
/**CNcomment: 定义按PutBufEx模式送码流的参数结构体 */
typedef struct hiUNF_AVPLAY_PUTBUFEX_OPT_S
{
    HI_BOOL bEndOfFrm;      /**<whether this package of stream is the end of one frame*//**<CNcomment: 该包码流是否为一帧的最后一包 */
    HI_BOOL bContinue;      /**<whether this package of stream is contnued with the last package*//**<CNcomment: 该包码流是否与之前连续 */
}HI_UNF_AVPLAY_PUTBUFEX_OPT_S;

typedef struct hiUNF_AVPLAY_GLOBAL_PLAY_INFO_S
{
    HI_U32 u32ContentCount;
} HI_UNF_AVPLAY_GLOBAL_PLAY_INFO_S;

typedef struct hiUNF_AVPLAY_VIDEO_FRAME_INFO_S
{
    HI_U32                              u32Width;           /**<Width of the source picture*/ /**<CNcomment: 原始图像宽*/
    HI_U32                              u32Height;          /**<Height of the source picture*/ /**<CNcomment: 原始图像高*/
    HI_U32                              u32AspectWidth;     /**<aspect ratio: width*/ /**<CNcomment:宽高比之宽值 */
    HI_U32                              u32AspectHeight;    /**<aspect ratio: height*/ /**<CNcomment:宽高比之高值 */
    HI_U32                              u32fpsInteger;     /**<Integral part of the frame rate (in frame/s)*/ /**<CNcomment: 码流的帧率的整数部分, fps */
    HI_U32                              u32fpsDecimal;     /**<Fractional part (calculated to three decimal places) of the frame rate (in frame/s)*/
    HI_BOOL                             bProgressive;       /**<Sampling type (progressive or interlaced)*/ /**<CNcomment: 采样方式(逐行/隔行) */
    HI_UNF_VIDEO_FRAME_PACKING_TYPE_E   enFramePackingType; /**<3D frame packing type*/
} HI_UNF_AVPLAY_VIDEO_FRAME_INFO_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      AVPLAY */
/** @{ */  /** <!-- [AVPLAY] */

/**
\brief Initializes the AVPLAY module.CNcomment:初始化AVPLAY模块 CNend
\attention \n
Before calling ::HI_UNF_AVPLAY_Create to create an AVPLAY, you must call this application programming interface (API).
CNcomment 在调用AVPLAY模块其他接口前，要求首先调用本接口 CNend
\param  N/A
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NOT_EXIST There is no AVPLAY. CNcomment:AVPLAY设备不存在 CNend
\retval ::HI_ERR_AVPLAY_NOT_DEV_FILE  The file is not an AVPLAY file. CNcomment:AVPLAY非设备 CNend
\retval ::HI_ERR_AVPLAY_DEV_OPEN_ERR  An AVPLAY fails to be started. CNcomment:AVPLAY打开失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Init(HI_VOID);

/**
\brief Deinitializes the AVPLAY module. CNcomment:去初始化AVPLAY模块 CNend
\attention \n
Please call this API function, before call anyother API of AVPLAY module.
CNcomment: 在调用::HI_UNF_AVPLAY_Destroy接口销毁所有的播放器后，调用本接口 CNend
\param N/A
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT  The operation is invalid.  CNcomment:操作非法 CNend
\retval ::HI_ERR_AVPLAY_DEV_CLOSE_ERR  An AVPLAY fails to be stopped. CNcomment:AVPLAY关闭失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_DeInit(HI_VOID);

/**
\brief Get video frame info. CNcomment:获取视频帧信息 CNend
\attention \n
\param[out] pstVideoFrameInfo  Pointer to video frame info. For details, see the description of ::HI_UNF_AVPLAY_VIDEO_FRAME_INFO_S. CNcomment:指针类型，视频帧信息，请参见::HI_UNF_AVPLAY_VIDEO_FRAME_INFO_S. CNend
\param[in] hAvplay    AVPLAY handle . CNcomment:AVPLAY句柄 . CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR  The pointer is null.  CNcomment:指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_FAILURE  Failure. CNcomment:失败. CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetVideoFrameInfo(HI_HANDLE hAvplay, HI_UNF_AVPLAY_VIDEO_FRAME_INFO_S *pstVideoFrameInfo);

/**
\brief Obtains the default configuration of an AVPLAY. CNcomment:获取缺省的AV播放配置 CNend
\attention \n
When calling this API to set the enCfg parameter, you must enter correct mode of the player to be created.\n
It is recommended that you call this API to obtain the default AVPLAY attributes before creating an AVPLAY. This avoids creation failures due to incomplete or incorrect parameters.
CNcomment:调用本接口输入enCfg参数时，请正确输入想要创建播放器模式\n
创建AV播放器前建议调用本接口，获取到AV播放器默认属性，避免创建AV播放器时由于参数不全或参数错误导致播放器创建不成功现象 CNend
\param[out] pstAvAttr  Pointer to AVPLAY attributes. For details, see the description of ::HI_UNF_AVPLAY_ATTR_S. CNcomment: 指针类型，AV播放属性，请参见::HI_UNF_AVPLAY_ATTR_S. CNend
\param[in] enCfg       AVPLAY type. For details, see the description of ::HI_UNF_AVPLAY_STREAM_TYPE_E. CNcomment: AV播放的类型，请参见::HI_UNF_AVPLAY_STREAM_TYPE_E. CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetDefaultConfig(HI_UNF_AVPLAY_ATTR_S *pstAvAttr, HI_UNF_AVPLAY_STREAM_TYPE_E enCfg);

/**
\brief Registers a dynamic audio decoding library. CNcomment:注册音频动态解码库 CNend
\attention \n
\param[in] pFileName Name of the file in the audio decoding library CNcomment:音频解码库文件名 CNend
\retval ::HI_SUCCESS  Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR  The input pointer is null. CNcomment:输入指针为空 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_RegisterAcodecLib(const HI_CHAR *pFileName);

/**
\brief Registers a dynamic video decoding library. CNcomment:注册视频动态解码库 CNend
\attention \n
\param[in] pFileName Name of the file in the video decoding library CNcomment:视频解码库文件名 CNend
\retval ::HI_SUCCESS  Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR  The input pointer is null. CNcomment:输入指针为空 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_RegisterVcodecLib(const HI_CHAR *pFileName);

/**
\brief Searches for registered dynamic audio decoding libraries based on the audio format.
CNcomment:根据音频格式, 查找注册音频动态解码库 CNend
\attention \n
\param[in] enFormat Audio format CNcomment:音频格式 CNend
\param[out] penDstCodecID If an audio decoding library is found, its codec ID is returned.
CNcomment:成功则返回音频解码库CodecID CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_FoundSupportDeoder(const HA_FORMAT_E enFormat,HI_U32 * penDstCodecID);


/**
\brief Sets private commands for a dynamic audio decoding library. These commands are used to call ha_codec.
CNcomment:设置私有命令给音频动态解码库, 调用ha_codec 方法  CNend
HI_HA_ERRORTYPE_E (*DecSetConfig)(HI_VOID * hDecoder, HI_VOID * pstConfigStructure);
\attention \n
\param[in] enDstCodecID  The audio Codec ID  CNcomment:音频解码库ID CNend
\param[in] pPara  Attribute structure CNcomment:属性结构 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR  The input pointer is null. CNcomment:输入指针为空 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_ConfigAcodec(const HI_U32 enDstCodecID, HI_VOID *pPara);

/**
\brief Creates an AVPLAY. CNcomment:创建AV播放器 CNend
\attention \n
Note the following point when setting the input parameter pstAttr: The stream source can be HI_UNF_AVPLAY_STREAM_TYPE_ES (ESs input from the memory) or HI_UNF_AVPLAY_STREAM_TYPE_TS (ESs input from the memory and TSs input from the Tuner).
CNcomment:输入属性参数pstAttr中有几点需要注意：码流源支持HI_UNF_AVPLAY_STREAM_TYPE_ES（内存输入ES流）、HI_UNF_AVPLAY_STREAM_TYPE_TS（内存输入TS流或TUNER输入TS 流）CNend
\param[in]  pstAvAttr   Pointer to AVPLAY attributes. For details, see the description of ::HI_UNF_AVPLAY_ATTR_S. CNcomment:指针类型，AV播放属性，请参见::HI_UNF_AVPLAY_ATTR_S. CNend
\param[out] phAvplay    Pointer to the handle of a created AVPLAY.CNcomment:指针类型，创建的AV播放句柄 CNend
\retval ::HI_SUCCESS  Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_CREATE_ERR       The AVPLAY fails to be created. CNcomment:AVPLAY创建失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Create(const HI_UNF_AVPLAY_ATTR_S *pstAvAttr, HI_HANDLE *phAvplay);

/**
\brief Destroys an AVPLAY. CNcomment:销毁AV播放器 CNend
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle CNcomment:AV播放句柄 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Destroy(HI_HANDLE hAvplay);

/**
\brief Sets the attributes of an AVPLAY. CNcomment:设置AV播放属性 CNend
\attention \n
You can set the audio/video PID, audio/video decoding type, and synchronization mode by calling this API.\n
Different attribute IDs correspond to different data types. For details, see the Note part of HI_UNF_AVPLAY_ATTR_ID_E. The attribute types of the pPara and enAttrID parameters must be the same.\n
Before setting HI_UNF_AVPLAY_ATTR_ID_ADEC (audio decoding attribute) and HI_UNF_AVPLAY_ATTR_ID_VDEC (video decoding attribute),\n
you must disable the audio channel or video channel. The new attributes take effect when you enable the audio channel or video channel again.
CNcomment:调用本接口可实现设置音视频PID、设置音视频解码类型、设置同步方式等功能\n
不同的属性ID对应的结构体请参见结构体HI_UNF_AVPLAY_ATTR_ID_E的[注意], pPara参数要与enAttrID对应的属性结构体类型保持一致\n
当需要设置HI_UNF_AVPLAY_ATTR_ID_ADEC(音频解码属性),HI_UNF_AVPLAY_ATTR_ID_VDEC(视频解码属性)时\n
需要先关闭音频或视频通道，再设置新属性，然后再重新打开音频或视频通道新属性才可以生效。 CNend
\param[in] hAvplay         AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enAttrID        Attribute ID CNcomment:属性ID CNend
\param[in] pPara  Data type corresponding to an attribute ID CNcomment:属性ID对应结构 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_SetAttr(HI_HANDLE hAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara);

/**
\brief Obtains the attributes of an AVPLAY. CNcomment:获取AV播放属性 CNend
\attention \n
N/A
\param[in] hAvplay          AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enAttrID         Attribute ID CNcomment:属性ID CNend
\param[in] pPara   Data type corresponding to an attribute ID, CNcomment:属性ID对应结构 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetAttr(HI_HANDLE hAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara);

/**
\brief Decodes I frames. CNcomment:解码I帧码流 CNend
\attention \n
If pstCapPicture is null, the decoded I frames are displayed in the window,and do not need release memory; \n
if pstCapPicture is not null, the information about I frames is reported, and need invoke ::HI_UNF_AVPLAY_ReleaseIFrame to release memory.
You must stop the video decoder before calling this API. If I frames are being processed, do not enable the video decoder.
CNcomment:当pstCapPicture为空指针时，解完的I帧将在window上显示，不需释放内存，如果非空，则不会显示而是将I帧信息上报,同时I帧处理完毕后，需要调用HI_UNF_AVPLAY_ReleaseIFrame来释放I帧
调用该接口前，需停止视频解码器；I帧处理没有完成时，也不能启动视频解码器 CNend
\param[in] hAvplay         AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] pstFrame        Pointer to frame attributes CNcomment:指针类型，帧数据属性 CNend
\param[in] pstCapPicture   Pointer to the frame information CNcomment:指针类型，帧信息 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_DecodeIFrame(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_I_FRAME_S *pstFrame, HI_UNF_VIDEO_FRAME_INFO_S *pstCapPicture);


/**
\brief Release memory of I frame. CNcomment:释放I帧帧存 CNend
\attention \n
If you call HI_UNF_AVPLAY_DecodeIFrame with non-null pstCapPicture, you need call this API to release the memory.
CNcomment:如果调用HI_UNF_AVPLAY_DecodeIFrame时pstCapPicture不为空，需要调用此接口来释放内存 CNend
\param[in] hAvplay         AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] pstCapPicture   Pointer to the frame information CNcomment:指针类型，释放的帧信息 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_ReleaseIFrame(HI_HANDLE hAvplay, HI_UNF_VIDEO_FRAME_INFO_S *pstCapPicture);


/**
\brief Sets the mode of a video decoder. CNcomment:设置视频解码器的模式 CNend
\attention \n
This API is used in trick play mode. Before switching the mode to the trick play mode, you must enable a decoder to decode only I frames by calling this API.\n
Before switching the mode to the normal mode, you also need to set the mode of a decoder to normal by calling this API.
CNcomment:本接口主要应用在快进播放的场景，当切换到快进播放前，可以先调用本接口将解码器设置为只解I帧，\n
当切换回正常播放前，先调用本接口将解码器设置为NORMAL。 CNend
\param[in] hAvplay            AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enDecodeMode       Decoding mode CNcomment:解码模式 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_SetDecodeMode(HI_HANDLE hAvplay, HI_UNF_VCODEC_MODE_E enDecodeMode);

/**
\brief Registers an event on 32bit system. CNcomment:注册事件 CNend
\attention \n
N/A
\param[in] hAvplay     AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enEvent     Event type enumeration CNcomment:枚举类型，表示事件类型 CNend
\param[in] pfnEventCB  Pointer to the callback function corresponding to the registered event. CNcomment:回调函数指针，指向与注册事件对应的回调函数 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT     The AVPLAY is not initialized.  CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_RegisterEvent(HI_HANDLE      hAvplay,
                                   HI_UNF_AVPLAY_EVENT_E     enEvent,
                                   HI_UNF_AVPLAY_EVENT_CB_FN pfnEventCB);
/**
\brief Registers an event on 64bit system. CNcomment:64位系统注册事件 CNend
\attention \n
N/A
\param[in] hAvplay     AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enEvent     Event type enumeration CNcomment:枚举类型，表示事件类型 CNend
\param[in] pfnEventCB  Pointer to the callback function corresponding to the registered event. CNcomment:回调函数指针，指向与注册事件对应的回调函数 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT     The AVPLAY is not initialized.  CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_RegisterEvent64(HI_HANDLE      hAvplay,
                                   HI_UNF_AVPLAY_EVENT_E     enEvent,
                                   HI_UNF_AVPLAY_EVENT_CB_FN64 pfnEventCB);

/**
\brief Deregisters an event. CNcomment:取消注册事件 CNend
\attention \n
N/A
\param[in] hAvplay   AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enEvent   Event type enumeration CNcomment:枚举类型，表示事件类型 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_UnRegisterEvent(HI_HANDLE hAvplay, HI_UNF_AVPLAY_EVENT_E enEvent);

/**
\brief Enables an AVPLAY channel. CNcomment:打开AV播放器通道 CNend
\attention \n
You can enable an audio channel and a video channel for each AVPLAY. If you only need to play videos or audios, you can enable the corresponding channel to save resources.
CNcomment:每个AV播放器仅支持打开音视频通道各1个。如果只播放音频或视频，只需要打开相应通道，以节省资源。 CNend
\param[in] hAvplay  AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enChn    Separate audio channel or video channel. For details, see the description of ::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNcomment:单独的音视频通道，请参见::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNend
\param[in] pPara    Pointer type. For details, see the description of ::HI_UNF_AVPLAY_OPEN_OPT_S. CNcomment:指针类型，请参见::HI_UNF_AVPLAY_OPEN_OPT_S. CNend
    If enChn is set to HI_UNF_AVPLAY_MEDIA_CHAN_VID, this API is used to specify the maximum decoding performance of the video decoder.
    If enChn is set to NULL, the maximum performance H264+HI_UNF_VCODEC_CAP_LEVEL_FULLHD is used by default.
    The higher the configured decoding performance, the larger the required MMZ. It is recommended that you configure the performance as required.
    CNcomment:enChn为HI_UNF_AVPLAY_MEDIA_CHAN_VID时用来指定视频解码器的最大解码能力。
    如果设为NULL，将默认为最大能力: H264+HI_UNF_VCODEC_CAP_LEVEL_FULLHD。
    配置支持的能力越大，需要的MMZ物理内存也就越大，建议按需配置即可。 CNend
\retval ::HI_SUCCESS Success             CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_ChnOpen(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_VOID *pPara);

/**
\brief Disables an AVPLAY channel. CNcomment:关闭AV播放器通道 CNend
\attention \n
N/A
\param[in] hAvplay   AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enChn     Separate audio channel or video channel. For details, see the description of ::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNcomment:单独的音视频通道，请参见::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_ChnClose(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn);

/**
\brief Start an AVPLAY to prepare play mode, which just demux ts stream. CNcomment:启动播放器进入仅仅做解复用TS的PREPLAY状态 CNend
\attention \n
After enabling channels and setting their attributes, you can call this API to start an AVPLAY to enable it to work in prepare play mode. The audios and videos can be prepared play separately or simultaneously.
CNcomment:当完成通道打开和属性设置后，调用本接口启动预播放，进入PREPLAY状态。支持分别和同时启动音视频预播放。 CNend
\param[in] hAvplay         AVPLAY handle  CNcomment:AV播放句柄 CNend
\param[in] enChn           Separate audio channel or video channel. For details, see the description of ::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNcomment:单独的音视频通道，请参见::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNend
\param[in] pstPreStartOpt     Pointer used for expansion. You can set it to NULL. CNcomment:指针类型，待扩展使用，设为NULL即可. CNend
\retval ::HI_SUCCESS  Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_PreStart(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_UNF_AVPLAY_PRESTART_OPT_S *pstPreStartOpt);

/**
\brief Starts an AVPLAY. The AVPLAY is in play mode. CNcomment:启动播放器，进入PLAY状态 CNend
\attention \n
After enabling channels and setting their attributes, you can call this API to start an AVPLAY to enable it to work in play mode. The audios and videos can be played separately or simultaneously.
CNcomment:当完成通道打开和属性设置后，调用本接口启动播放，进入PLAY状态。支持分别和同时启动音视频播放。 CNend
\param[in] hAvplay         AVPLAY handle  CNcomment:AV播放句柄 CNend
\param[in] enChn           Separate audio channel or video channel. For details, see the description of ::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNcomment:单独的音视频通道，请参见::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNend
\param[in] pstStartOpt     Pointer used for expansion. You can set it to NULL. CNcomment:指针类型，待扩展使用，设为NULL即可. CNend
\retval ::HI_SUCCESS  Success CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Start(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_UNF_AVPLAY_START_OPT_S *pstStartOpt);

/**
\brief Stop an AVPLAY to prepare stop mode,the interface is reserved for future use. CNcomment:停止AV播放使其进入PRESTOP状态,该接口保留备用 CNend
\param[in] hAvplay  AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enChn    Separate audio channel or video channel. For details, see the description of ::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNcomment:单独的音视频通道，请参见::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNend
\param[in] pstPreStopOpt   Pointer to stop mode. For details, see the description of ::HI_UNF_AVPLAY_PRESTOP_OPT_S. CNcomment:指针类型，清屏模式，请参见::HI_UNF_AVPLAY_STOP_OPT_S. CNend
\retval ::HI_ERR_AVPLAY_NOT_SUPPORT not support for the moment  CNcomment:暂不支持 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_PreStop(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_UNF_AVPLAY_PRESTOP_OPT_S *pstPreStopOpt);

/**
\brief Stops an AVPLAY. Then the AVPLAY is in stop mode. CNcomment:停止AV播放，进入STOP状态 CNend
\attention \n
If you call this API, all selected channels stop playing. The playing audios and videos can be stopped separately or simultaneously.\n
The parameter pstStopOpt->enMode takes effect only when a video channel is selected.\n
If pstStopOpt->enMode is set to NULL or HI_UNF_AVPLAY_STOP_MODE_STILL, the last frame is kept still.\n
If pstStopOpt->enMode is set to HI_UNF_AVPLAY_STOP_MODE_BLACK, the last frame is cleared, and the blank screen appears.\n
When pstStopOpt->u32TimeoutMs is 0, the AVPLAY stops and this API is returned.\n
When pstStopOpt->u32TimeoutMs is greater than 0, this API is blocked until the data in the buffer is used up.\n
When pstStopOpt->u32TimeoutMs is greater than -1, this API is blocked until the data in the buffer is used up.\n
To stop the audio or video separately when both the audio and video are enabled, you must set pstStopOpt->u32TimeoutMs to 0.
CNcomment:调用本接口将停止所选通道的播放，支持分别和同时停止音视频播放。\n
当所选通道中包含视频通道时，参数pstStopOpt->enMode才有意义。\n
当pstStopOpt->enMode为空或者为HI_UNF_AVPLAY_STOP_MODE_STILL时，保留最后一帧视频图像。\n
当pstStopOpt->enMode为HI_UNF_AVPLAY_STOP_MODE_BLACK时，清除视频最后一帧，视频输出为黑屏。\n
当pstStopOpt->u32TimeoutMs为0时将离开停止播放并返回。\n
当pstStopOpt->u32TimeoutMs>0时将阻塞相应时间，直到缓冲中的数据播完。\n
当pstStopOpt->u32TimeoutMs=-1时将一直阻塞到缓冲中的数据播完。\n
当音视频都处于开启状态时,要单独停止音频和视频，必须设置pstStopOpt->u32TimeoutMs为0. CNend
\param[in] hAvplay  AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enChn    Separate audio channel or video channel. For details, see the description of ::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNcomment:单独的音视频通道，请参见::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNend
\param[in] pstStopOpt   Pointer to the clear screen mode. For details, see the description of ::HI_UNF_AVPLAY_STOP_OPT_S. CNcomment:指针类型，清屏模式，请参见::HI_UNF_AVPLAY_STOP_OPT_S. CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Stop(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_UNF_AVPLAY_STOP_OPT_S *pstStopOpt);

/**
\brief Pauses an AVPLAY. Then the AVPLAY is in pause mode. CNcomment:暂停AV播放，进入PAUSE状态 CNend
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle CNcomment: AV播放句柄 CNend
\param[in] pstPauseOpt  Pointer used for expansion. You can set it to NULL. CNcomment:指针类型，待扩展使用，设为为NULL即可 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Pause(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_PAUSE_OPT_S *pstPauseOpt);

/**
\brief Plays videos or audios in trick play mode. Then the AVPLAY is in TPLAY mode. CNcomment:倍速播放，进入TPLAY状态 CNend
\attention \n
pstTplayOpt->u32SpeedInteger is the integer part of speed, the range is 0-64.
pstTplayOpt->u32SpeedDecimal is the decimal part of speed, the range is 0-999.
CNcomment: pstTplayOpt->u32SpeedInteger为速度的整数部分，取值范围为0-64. CNend
CNcomment: pstTplayOpt->u32SpeedDecimal为速度的小数部分，保留3位小数，取值范围为0-999. CNend
\param[in] hAvplay  AVPLAY handle CNcomment: AV播放句柄 CNend
\param[in] pstTplayOpt   The pointer of Tplay parameter,For details, see the description of ::HI_UNF_AVPLAY_TPLAY_OPT_S . CNcomment:指针类型，TPLAY参数指针,请参见HI_UNF_AVPLAY_TPLAY_OPT_S结构体定义 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Tplay(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_TPLAY_OPT_S *pstTplayOpt);

/**
\brief Resumes an AVPLAY. Then the AVPLAY is in play mode. CNcomment:恢复AV播放，进入PLAY状态 CNend
\attention \n
By calling this API, you can resume an AVPLAY from the trick play mode or pause mode rather than the stop mode.
CNcomment:本接口用来将倍速或暂停状态恢复为播放状态，但无法将停止状态恢复为播放状态。 CNend
\param[in] hAvplay  AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] pstResumeOpt  Pointer used for expansion. You can set it to NULL. CNcomment:指针类型，待扩展使用，暂置为空即可 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Resume(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_RESUME_OPT_S *pstResumeOpt);

/**
\brief Resets an AVPLAY. In this case, the play mode is not changed. CNcomment:复位AV播放，不改变状态 CNend
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] pstResetOpt   Pointer used for expansion. You can set it to NULL. CNcomment:指针类型，待扩展使用，设为为NULL即可 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Reset(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_RESET_OPT_S *pstResetOpt);

/**
\brief Step play. CNcomment:步进播放 CNend
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] pstStepOpt   Pointer used for expansion. You can set it to NULL. CNcomment:指针类型，待扩展使用，设为为NULL即可 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Step(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_STEP_OPT_S *pstStepOpt);

/**
\brief Applies for a buffer for storing the streams played by an AVPLAY. CNcomment:申请AV播放的码流缓冲 CNend
\attention \n
This API is used only when you want to play the ESs obtained from Internet or local ESs.\n
The pstData parameter is used to return the start address and size of the buffer applied for.\n
If u32TimeOutMs is set to 0, it indicates that the waiting time is 0; if u32TimeOutMs is set to 0XFFFFFFFF, it indicates that the API waits for an infinite time; if u32TimeOutMs is set to other values, it indicates that the waiting time is u32TimeOutMs ms.\n
If no buffer can be applied for during the block period, an error code indicating full buffer is returned.\n
If u32TimeOutMs is set to 0, and no buffer can be applied for, it indicates that the audio and video buffers are full. In this case, you need to call the usleep(N*1000) function to release the CPU.
Therefore, other threads can be scheduled.
CNcomment:当播放网络或本地ES流时才需要使用本接口。\n
参数pstData用来返回成功申请到的Buffer的首地址以及大小。\n
u32TimeOutMs设置为0表示不等待，设置为0xffffffff表示一直等待，设置为其他值表示等待u32TimeOutMs毫秒。\n
若超过阻塞时间，还无法申请到Buffer，则返回buffer满错误码\n
u32TimeOutMs配置为0时，如果申请不到Buffer，说明此时音视频的Buffer已满，需要通过usleep(N*1000)释放cpu
以使其它线程能够得到调度。 CNend
\param[in] hAvplay  AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enBufId     Buffer queue ID CNcomment:缓冲队列ID CNend
\param[in] u32ReqLen   Size of the buffer applied for CNcomment:申请缓存的大小 CNend
\param[out] pstData    Pointer to the returned buffer CNcomment:返回缓存指针 CNend
\param[in] u32TimeOutMs      Wait timeout, in ms CNcomment:等待超时时间，单位ms. CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:操作非法 CNend
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetBuf(HI_HANDLE  hAvplay,
                            HI_UNF_AVPLAY_BUFID_E enBufId,
                            HI_U32                u32ReqLen,
                            HI_UNF_STREAM_BUF_S  *pstData,
                            HI_U32                u32TimeOutMs);

/**
\brief Updates the write pointer after data is successfully copied. CNcomment:拷贝数据成功后，更新写指针 CNend
\attention \n
After transmitting streams to the buffer applied for, you can call this API to update the write pointer of the audio and video buffers.\n
If the transmitted streams do not contain PTS, u32ptsMs must be set to -1.
CNcomment:在向申请到的缓冲区内送码流完毕后，调用本接口更新音视频缓冲区的写指针。\n
如果本次送入的码流没有对应的PTS，u32ptsMs必须为-1。 CNend
\param[in] hAvplay  AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enBufId     Buffer queue ID CNcomment:缓冲队列ID. CNend
\param[in] u32ValidDataLen     Number of bytes that are written to the buffer CNcomment:实际写入缓冲区的字节数 CNend
\param[in] u32PtsMs            PTS, in ms  CNcomment:时间戳,以毫秒为单位 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_PutBuf(HI_HANDLE hAvplay, HI_UNF_AVPLAY_BUFID_E enBufId,
                         HI_U32 u32ValidDataLen, HI_U32 u32PtsMs);


/**
\brief Updates the write pointer after data is successfully copied. CNcomment:拷贝数据成功后，更新写指针 CNend
\attention \n
After transmitting streams to the buffer applied for, you can call this API to update the write pointer of the audio and video buffers.\n
If the transmitted streams do not contain PTS, u32ptsMs must be set to -1.
CNcomment:在向申请到的缓冲区内送码流完毕后，调用本接口更新音视频缓冲区的写指针, 本接口可由于一帧码流分多包送入。\n
如果本次送入的码流没有对应的PTS，u32ptsMs必须为-1。 CNend
\param[in] hAvplay  AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] enBufId     Buffer queue ID CNcomment:缓冲队列ID CNend
\param[in] u32ValidDataLen     Number of bytes that are written to the buffer CNcomment:实际写入缓冲区的字节数 CNend
\param[in] u32PtsMs            PTS, in ms  CNcomment:时间戳,以毫秒为单位 CNend
\param[in] pPutOpt   the extern parameter of PutBufEx, see the description of ::HI_UNF_AVPLAY_PUTBUFEX_OPT_S.CNcomment:PutBufEx的额外参数，请参见::HI_UNF_AVPLAY_PUTBUFEX_OPT_S. CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_PutBufEx(HI_HANDLE hAvplay, HI_UNF_AVPLAY_BUFID_E enBufId,
                                       HI_U32 u32ValidDataLen, HI_U32 u32Pts, HI_UNF_AVPLAY_PUTBUFEX_OPT_S *pPutOpt);

/**
\brief Obtains the handle of the DMX audio channel used by an AVPLAY in TS mode. CNcomment:TS模式时获取AV播放器使用的DMX音频通道的Handle CNend
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] phDmxAudChn    Pointer to the handle of a DMX audio channel CNcomment:DMX音频通道Handle指针 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetDmxAudChnHandle(HI_HANDLE hAvplay, HI_HANDLE *phDmxAudChn);

/**
\brief Obtains the handle of the DMX video channel used by an AVPLAY in TS mode. CNcomment:TS模式时获取AV播放器使用的DMX视频通道的Handle. CNend
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle CNcomment:AV播放句柄 CNend
\param[in] phDmxVidChn    Pointer to the handle of a DMX video channel CNcomment:DMX视频通道Handle指针 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetDmxVidChnHandle(HI_HANDLE hAvplay, HI_HANDLE *phDmxVidChn);


/**
\brief Obtains the status information about an AVPLAY. CNcomment:获取AV播放状态信息 CNend
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle  CNcomment:AV播放句柄 CNend
\param[out] pstStatusInfo  Pointer to the status of an AVPLAY. For details, see the description of HI_UNF_AVPLAY_STATUS_INFO_S. CNcomment:指针类型，AV播放状态信息，请参见HI_UNF_AVPLAY_STATUS_INFO_S. CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetStatusInfo(HI_HANDLE          hAvplay,
                                 HI_UNF_AVPLAY_STATUS_INFO_S *pstStatusInfo);


/**
\brief Obtains the information about audio and video streams. CNcomment:获取音视频码流信息 CNend
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle  CNcomment:AV播放句柄 CNend
\param[in] pstStreamInfo     Pointer to the information about audio and video streams. For details, see the description of HI_UNF_AVPLAY_STREAM_INFO_S. CNcomment:指针类型，音视频码流信息，请参见HI_UNF_AVPLAY_STREAM_INFO_S CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetStreamInfo(HI_HANDLE          hAvplay,
                                   HI_UNF_AVPLAY_STREAM_INFO_S *pstStreamInfo);

/**
\brief Obtains the information audio spectrums. CNcomment:获取音频能量信息 CNend
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle  CNcomment:AV播放句柄 CNend
\param[in] pSpectrum      Pointer to the array of audio spectrums. CNcomment:指针类型，音频能量信息数组指针 CNend
\param[in] u32BandNum     Length of an audio spectrum array CNcomment:音频能量信息数组长度 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetAudioSpectrum(HI_HANDLE hAvplay, HI_U16 *pSpectrum, HI_U32 u32BandNum);

/**
\brief Queries whether the AVPLAY buffer is empty. CNcomment:查询AVPLAY buffer是否已经为空 CNend
\attention \n
N/A
\param[in] hAvplay        AVPLAY handle CNcomment:AV播放句柄 CNend
\param[out] pbIsEmpty      Pointer type. This pointer indicates whether the AVPLAY buffer is empty (the playing ends). CNcomment:指针类型，指示buffer是否已经为空(播放完成) CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_IsBuffEmpty(HI_HANDLE hAvplay, HI_BOOL * pbIsEmpty);

/**
\brief Switch the demux audio channel CNcomment:切换音频DEMUX句柄 CNend
\attention \n
N/A
\param[in] hAvplay       AVPLAY handle CNcomment: AV播放句柄 CNend
\param[in] hNewDmxAud     New demux audio handle CNcomment:新DMX句柄 CNend
\param[out] phOldDmxAud     Old  demux audio handle CNcomment:旧DMX句柄指针 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_SwitchDmxAudChn(HI_HANDLE hAvplay, HI_HANDLE hNewDmxAud, HI_HANDLE *phOldDmxAud);

/**
\brief Notify an AVPLAY the stream is end CNcomment:通知AVPLAY码流已经送完 CNend
\attention \n
Call this interface to notice AVPLAY when the last package of stream has been sent,
then check whether the last frame has been output by eos event or by invoking ::HI_UNF_AVPLAY_IsBuffEmpty,
this interface is only apply to ES mode.
CNcomment: 当用户送完最后一包码流时，调用该接口通知AVPLAY，之后可以通过检测EOS事件或者调用::HI_UNF_AVPLAY_IsBuffEmpty判断最后一帧是否输出
目前该接口仅适用于ES模式 CNend
\param[in] hAvplay       AVPLAY handle CNcomment: AV播放句柄 CNend
\param[in] pstFlushOpt   Pointer used for expansion. You can set it to NULL.CNcomment:指针类型，待扩展使用，设为为NULL即可 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_FlushStream(HI_HANDLE hAvplay, HI_UNF_AVPLAY_FLUSH_STREAM_OPT_S *pstFlushOpt);

/**
\brief AVPLAY private command invoking. CNcomment: AVPLAY私有命令调用 CNend
\attention \n
\param[in] hAvplay       AVPLAY handle CNcomment: AV播放句柄 CNend
\param[in] enInvokeType  Type of private command invoking.CNcomment:私有命令调用类型 CNend
\param[in] pPara         Pointer to the parameter of invoking. CNcomment:指针类型，指向Invoke调用的参数 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Invoke(HI_HANDLE hAvplay, HI_UNF_AVPLAY_INVOKE_E enInvokeType, HI_VOID *pPara);

/**
\brief Accquire user dada. CNcomment: 获取用户数据 CNend
\attention \n
Only support Closed Caption Data.
CNcomment: 仅支持CC数据 CNend
\param[in] hAvplay       AVPLAY handle CNcomment: AV播放句柄 CNend
\param[out] pstUserData  user data.CNcomment:用户数据 CNend
\param[out] penType      user data type. CNcomment:用户数据类型 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_AcqUserData(HI_HANDLE hAvplay, HI_UNF_VIDEO_USERDATA_S *pstUserData, HI_UNF_VIDEO_USERDATA_TYPE_E *penType);

/**
\brief Accquire user dada. CNcomment: 释放用户数据 CNend
\attention \n
Only support Closed Caption Data.
CNcomment: 仅支持CC数据 CNend
\param[in] hAvplay       AVPLAY handle CNcomment: AV播放句柄 CNend
\param[in] pstUserData  user data.CNcomment:用户数据 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_RlsUserData(HI_HANDLE hAvplay, HI_UNF_VIDEO_USERDATA_S* pstUserData);

/**
\brief Obtains the left video frame tahr are not decoded CNcomment: 获取视频流缓冲区中未被解码的帧数目 CNend
\attention \n
Only support AVPLAY buffer by frame mode.
CNcomment: 仅支持按帧送数据流方式 CNend
\param[in] hAvplay       AVPLAY handle CNcomment: AV播放句柄 CNend
\param[in] pLeftStreamFrm  This pointer indicates the AVPLAY buffer left video frame that are not decoded.CNcomment:视频流缓存区中没有被解码的帧数目 CNend
\retval ::HI_SUCCESS Success  CNcomment:成功 CNend
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAY未初始化 CNend
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetLeftStreamFrm(HI_HANDLE hAvplay, HI_U32 *pLeftStreamFrm);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif
