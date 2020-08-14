				/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_player_type.h
 * @brief   sme player type定义
 * @date    2014/3/25
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/3/25
 * Desc   : Created file
 *
******************************************************************************/

#ifndef __SME_PLAYER_TYPE_H__
#define __SME_PLAYER_TYPE_H__

#include "osal_type.h"

typedef enum __tagSmePlayerNotify
{
    E_SME_PLAYER_NOTIFY_ERROR = 11000,
    E_SME_PLAYER_NOTIFY_PREPARED,
    E_SME_PLAYER_NOTIFY_PLAYBACK_COMPLETED,
    E_SME_PLAYER_NOTIFY_SEEK_COMPLETED,
    E_SME_PLAYER_NOTIFY_VIDEO_SIZE_CHANGED,
    E_SME_PLAYER_NOTIFY_INFO,
    E_SME_PLAYER_NOTIFY_BUFFER_UPDATE,
    E_SME_PLAYER_NOTIFY_FRAGMENT_UPDATE,
    E_SME_PLAYER_NOTIFY_PLAYER_STATE_CHANGED,
    E_SME_PLAYER_NOTIFY_HTTP_ERR,
    E_SME_PLAYER_NOTIFY_BUTT,
}E_SME_PLAYER_NOTIFY;

typedef enum __tagSmePlayerError
{
    E_SME_PLAYER_ERROR_NONE = ICS_SUCCESS,
    E_SME_PLAYER_ERROR_UNKOWN = 10000,
    E_SME_PLAYER_ERROR_TIME_OUT,
    E_SME_PLAYER_ERROR_UNSUPPORT_FORMAT,
    E_SME_PLAYER_ERROR_UNSUPPORT_AUDIO_CODEC,
    E_SME_PLAYER_ERROR_UNSUPPORT_VIDEO_CODEC,
    E_SME_PLAYER_ERROR_UNSUPPORT_DISPLAY_MODE,
    E_SME_PLAYER_ERROR_UNSUPPORT_DISPLAY_ANGLE,
    E_SME_PLAYER_ERROR_UNSUPPORT_PLAY_MODE,
    E_SME_PLAYER_ERROR_UNSUPPORT_OP,
    E_SME_PLAYER_ERROR_STATE,
    E_SME_PLAYER_ERROR_NOMEM,
    E_SME_PLAYER_ERROR_INVALID_ARGS,
    E_SME_PLAYER_ERROR_ASYNC,
    E_SME_PLAYER_ERROR_UNSUPPORT_SEEK,
    E_SME_PLAYER_ERROR_UNSUPPORT_TEXT_CODEC,
    E_SME_PLAYER_ERROR_SUBTILE_NOT_FOUND,
    E_SME_PLAYER_ERROR_SUBTILE_ACCESS,
    E_SME_PLAYER_ERROR_FITIN_PARAMETER,
    E_SME_PLAYER_ERROR_INVAILD_RECT,
    E_SME_PLAYER_ERROR_UNSUPPORT_LOOP_PLAYBACK,
    E_SME_PLAYER_ERROR_BUTT,
}E_SME_PLAYER_ERROR;

typedef enum __tagSmePlayerInfo
{
    E_SME_PLAYER_INFO_BUFFERING_START = 10100,
    E_SME_PLAYER_INFO_BUFFERING_STOP,
    E_SME_PLAYER_INFO_BUFFERING_OFFSET,         //extra为相对当前播放进度的事件偏移，单位ms
    E_SME_PLAYER_INFO_BUTT,
}E_SME_PLAYER_INFO;

typedef enum __tagSmePlayerProp
{
    E_SME_PLAYER_PROP_LOG = 10600,
    E_SME_PLAYER_PROP_BUTT,
}E_SME_PLAYER_PROP;

typedef enum __tagSmePlayerLogLevel
{
    E_SME_PLAYER_LOG_LEVEL_NONE = 0,
    E_SME_PLAYER_LOG_LEVEL_ERROR,
    E_SME_PLAYER_LOG_LEVEL_WARNING,
    E_SME_PLAYER_LOG_LEVEL_INFO,
    E_SME_PLAYER_LOG_LEVEL_DEBUG,
    E_SME_PLAYER_LOG_LEVEL_VERBOSE,
    E_SME_PLAYER_LOG_LEVEL_BUTT,
}E_SME_PLAYER_LOG_LEVEL;

typedef enum __tagSmePlayerDisPlayMode
{
    E_SME_PLAYER_DIS_MODE_FULL = 0,
    E_SME_PLAYER_DIS_MODE_FITIN,
    E_SME_PLAYER_DIS_MODE_FITOUT,
    E_SME_PLAYER_DIS_MODE_FITCENTER,
    E_SME_PLAYER_DIS_MODE_DEFAULT,
    E_SME_PLAYER_DIS_MODE_NONE,
    E_SME_PLAYER_DIS_MODE_BUTT,
}E_SME_PLAYER_DISPLAY_MODE;

typedef enum __tagSmePlayerDisPlayAngle
{
    E_SME_PLAYER_DIS_ANGLE_0 = 0,
    E_SME_PLAYER_DIS_ANGLE_90,
    E_SME_PLAYER_DIS_ANGLE_180,
    E_SME_PLAYER_DIS_ANGLE_270,
    E_SME_PLAYER_DIS_ANGLE_HFLIP,
    E_SME_PLAYER_DIS_ANGLE_VFLIP,
    E_SME_PLAYER_DIS_ANGLE_BUTT,
}E_SME_PLAYER_DISPLAY_ANGLE;

typedef enum __tagSmePlayerVolume
{
    E_SME_PLAYER_VOLUME_MIN = 0,
    E_SME_PLAYER_VOLUME_MAX = 10,
}E_SME_PLAYER_VOLUME;

typedef enum __tagSmePlayerMode
{
    SME_PLAYER_MODE_NORMAL = 0,
    SME_PLAYER_MODE_BUTT,
}E_SME_PLAYER_MODE;

typedef enum _tagSmePlayerState
{
    E_SME_PLAYER_STATE_PLAYING=0,
    E_SME_PLAYER_STATE_PAUSE,
    E_SME_PLAYER_STATE_BUFFERING,
    E_SME_PLAYER_STATE_STOP,
    E_SME_PLAYER_STATE_ERROR,
}E_SME_PLAYER_STATE;

//for subtitle
typedef enum __tagSmeSubType
{
    E_SME_SUBTITLE_TEXT = 0,        //Text subtitle *//**<CNcomment:string字幕
    E_SME_SUBTITLE_BUTT
}E_SME_SUBTITLE_TYPE_E;

typedef enum __tagSmeSubTextStyle
{
    E_SME_SUBTITLE_TEXT_STYLE_NORMAL = 0, //正常
    E_SME_SUBTITLE_TEXT_STYLE_BOLD,       //<b>String</b>|<B>String</B>
    E_SME_SUBTITLE_TEXT_STYLE_ITALIC,     //<i>String</i>|<I>String</I>
    E_SME_SUBTITLE_TEXT_STYLE_UNDERLINE,  //<u>String</u>|<U>String</U>
    E_SME_SUBTITLE_TEXT_STYLE_BUTT
}E_SME_SUBTITLE_TEXT_STYLE;

#define M_SME_SUB_COLOR_DFT (0xFFFFFFFF) //ARGB,wite
typedef struct __tagSubInfoText
{
    V_UINT8*  pu8SubData;             //字符内容buffer
    V_UINT32  u32SubDataLen;          //字符内容长度
    V_INT64   i64Pts;                 //字符pts，单位ms
    V_UINT32  u32Duration;            //字符时长，单位ms
    V_UINT64  u64Id;                  //字幕id，唯一。
    V_UINT32  u32Color;               //ARGB.默认白色，FFFFFFFF//<font color=#FFFFFF>String</font>
    E_SME_SUBTITLE_TEXT_STYLE eStyle; //text 风格
    V_INT32   i32PosX;                //位置X {\pos(x,y)} -1,为无效
    V_INT32   i32PosY;                //位置Y
    V_INT32   i32W;                   //位置宽
    V_INT32   i32H;                   //位置高
}ST_SUB_INFO_TEXT;

typedef struct __tagSubInfo{
    E_SME_SUBTITLE_TYPE_E eSubType;   //字幕类型，目前只有TEXT，后续有BITMAP等
    union
    {
        ST_SUB_INFO_TEXT stText;      //Text subtitle 信息
    }U_SUB_INFO;
}ST_SUB_INFO;

#define URI_MAX_LENGTH 512
//for fragment info
typedef struct  __tag_fragment_info
{
    V_INT8    i8ManifestUri[URI_MAX_LENGTH];
    V_INT8    i8FragmentUri[URI_MAX_LENGTH];
    V_UINT64  u64FragmentSize;
    V_UINT64  u64DownloadStartTime;
    V_UINT64  u64DownloadEndTime;
    V_UINT64  u64DownloadTotalTime;
}ST_FRAGMENT_INFO;

//返回值：成功：ICS_SUCCESS;失败：ICS_FAIL.
//pvCtx：上下文参数，透传ST_SubCallBack:: pvCtx
//stSubInfo：显示字幕数据结构，具体见ST_SUB_INFO
typedef  V_UINT32 (*PFN_SubDisPlayCb)(IN V_VOID* pvCtx,  IN ST_SUB_INFO stSubInfo);

//返回值：成功：ICS_SUCCESS;失败：ICS_FAIL.
//pvCtx：上下文参数，透传ST_SubCallBack:: pvCtx
//u64SubInfoId：要清除的字幕的编号，见ST_SUB_INFO。
typedef  V_UINT32 (*PFN_SubClearCb)(IN V_VOID* pvCtx,  IN V_UINT64 u64SubInfoId);

typedef struct __tag_subcallback{
    PFN_SubDisPlayCb  pfnSubDisplayCb;  //字幕显示回调
    PFN_SubClearCb    pfnSubClearCb;    //字幕清除回调
    V_VOID*           pvCtx;            //上述两个回调的上下文参数。
}ST_SubCallback;


typedef struct __tagSingleQueueStatus
{
    V_UINT32 u32BufferSize;
    V_UINT64 u64BufferPlayingTime;
    V_UINT64 u64LastBufferPTS;
}ST_SINGLE_QUEUE_STATUS;


typedef struct __tagBufferStatus
{
    V_UINT32 u32MaxBufferTime;
    V_UINT32 u32LowPercent;
    V_UINT32 u32HighPercent;
    ST_SINGLE_QUEUE_STATUS stVBufferStatus;
    ST_SINGLE_QUEUE_STATUS stABufferStatus;
}ST_BUFFER_STATUS;

typedef struct _tagSmeCurrentBitrate
{
     V_UINT32            audio;
     V_UINT32            video;
}ST_CURRENT_BITRATE;

typedef struct __tagDispRatio
{
    /* 应用设置的宽高比 */
    V_UINT32 u32DispRatioW;
    V_UINT32 u32DispRatioH;
}ST_DISP_RATIO;

typedef enum __tagSmePlayerSwitchChannelMode
{
    E_SME_PLAYER_SWITCH_CHANNEL_MODE_BLACK = 0, //black
    E_SME_PLAYER_SWITCH_CHANNEL_MODE_LAST,//static
    E_SME_PLAYER_SWITCH_CHANNEL_MODE_BUTT,
} E_SME_PLAYER_SWITCH_CHANNEL_MODE;

typedef  V_VOID(*PFN_StreamPlayModeCb)(V_VOID* pvCtx,
    IN V_UINT64 u64BitrateList[], IN V_UINT32 u32BitrateNum,
    OUT V_BOOL* bIsAdaptive, OUT V_UINT64* u64ConnectSpeed);

typedef  V_VOID(*PFN_BitrateChangedCb)(V_VOID* pvCtx,
                                       IN V_UINT64 u64VBitrate,
                                       IN V_UINT64 u64ABitrate);

#define M_SME_MIME_LEN_MAX                      64
typedef struct __tagVideoTrackInfo {
    V_UINT32 u32TrackId;
    V_CHAR   cVideoType[M_SME_MIME_LEN_MAX];
    V_UINT32 u32Bps;  // bits per second, for video
    V_UINT32 u32Bps_media;  // bits per second, for media
    V_UINT32 u32Width;
    V_UINT32 u32Height;
    V_UCHAR  ucFps;
    V_BOOL   bIsSupported;
} ST_VIDEO_TRACK_INFO;

#define AUDIO_TRACK_ARRAY_MAX_LEN    16
typedef struct __tagSingleAudioTrackInfo {
    V_UINT32 u32TrackId;
    V_CHAR   cAudioType[AUDIO_TRACK_ARRAY_MAX_LEN];
    V_UINT32 u32Bitrate;
    V_UINT32 u32SampleRate;
    V_UINT32 u32ChannelNum;
    V_CHAR   cLang[AUDIO_TRACK_ARRAY_MAX_LEN];
} ST_SINGLE_AUDIO_TRACK_INFO;

#define MAX_AUDIO_TRACK    32
typedef struct __tagAudioTrackInfo {
    V_UINT32 u32TrackNum;
    V_UINT32 u32CurrentTrackId;
    ST_SINGLE_AUDIO_TRACK_INFO stSingleAudioTrackInfo[MAX_AUDIO_TRACK];

    V_UINT32 u32UnSupportedTrackNum;
    /* info of unsupported audio track */
    ST_SINGLE_AUDIO_TRACK_INFO stUnSprtSingleAudioTrackInfo[MAX_AUDIO_TRACK];
} ST_AUDIO_TRACK_INFO;

#endif//__SME_PLAYER_TYPE_H__
