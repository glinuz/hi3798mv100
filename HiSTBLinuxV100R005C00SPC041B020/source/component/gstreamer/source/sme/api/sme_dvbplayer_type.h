/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

******************************************************************************/

#ifndef __SME_DVBPLAYER_TYPE_H__
#define __SME_DVBPLAYER_TYPE_H__

#define TVOS_MAX_KEY_LENGTH 16

typedef enum
{
  E_DVB_VID_CODEC_TYPE_UNKNOWN = 0,
  E_DVB_VID_CODEC_TYPE_MPEG1,
  E_DVB_VID_CODEC_TYPE_MPEG2,
  E_DVB_VID_CODEC_TYPE_MPEG4,
  E_DVB_VID_CODEC_TYPE_H264,
  E_DVB_VID_CODEC_TYPE_MVC,
  E_DVB_VID_CODEC_TYPE_H265,
  E_DVB_VID_CODEC_TYPE_AVS,
  E_DVB_VID_CODEC_TYPE_WMV,
  E_DVB_VID_CODEC_TYPE_AVS_PLUS,//AVS+
  E_DVB_VID_CODEC_TYPE_MAX,
}E_DVB_VID_CODEC_ID;

typedef enum
{
  E_DVB_AUD_CODEC_TYPE_UNKNOWN = 0,
  E_DVB_AUD_CODEC_TYPE_MP1,
  E_DVB_AUD_CODEC_TYPE_MP2,
  E_DVB_AUD_CODEC_TYPE_MP3,
  E_DVB_AUD_CODEC_TYPE_LPCM,
  E_DVB_AUD_CODEC_TYPE_AC3,
  E_DVB_AUD_CODEC_TYPE_EAC3,
  E_DVB_AUD_CODEC_TYPE_DTS,
  E_DVB_AUD_CODEC_TYPE_DRA,
  E_DVB_AUD_CODEC_TYPE_AAC,
  E_DVB_AUD_CODEC_TYPE_MAX,
}E_DVB_AUD_CODEC_ID;

typedef enum
{
  E_DVB_MEDIA_TYPE_AUDIO = 0,
  E_DVB_MEDIA_TYPE_VIDEO,
  E_DVB_MEDIA_TYPE_LAST,
}E_DVB_MEDIA_TYPE;

typedef struct
{
  unsigned int        vpid;                   // video pid
  unsigned int        apid;                   // audio pid
  unsigned int        pcrpid;                 // pcr pid
  E_DVB_VID_CODEC_ID  e_vtype;                // video codec type
  E_DVB_AUD_CODEC_ID  e_atype;                // audio codec type
  unsigned int        demux_id;               // HW demux ID
  unsigned int        audio_samplerate;       // audio sample rate (for PCM),reserved
  unsigned int        audio_channel;          // audio channel (for PCM),reserved
  unsigned int        audio_bits_per_sample;  // bits/sample (for PCM),reserved
  unsigned int        audio_is_ca;            //0 no ca, !0 ca.
  unsigned int        video_is_ca;             //0 no ca, !0 ca.
}ST_DVBProgMediaInfo;

typedef struct
{
  unsigned int        apid;                   // audio pid
  E_DVB_AUD_CODEC_ID  e_atype;                // audio codec type
  unsigned int        is_ca;                  //0 no ca, !0 ca.
}ST_DVBProgAudioInfo;


typedef struct
{
  //0 no CA, 1 clear CW, 2 TDES keyladder, 3 AES keyladder
  unsigned char video_ca_type;
  //0 for DMX_DESC_TYPE_CSA2, 1 for DMX_DESC_TYPE_CSA3,
  //please reference HAL DMX_DESC_TYPE_E in tvos_hal_demux.h
  unsigned char video_stream_descramble_type;
  //0 no CA, 1 clear CW, 2 TDES, 3 AES
  unsigned char audio_ca_type;
  //0 for DMX_DESC_TYPE_CSA2, 1 for DMX_DESC_TYPE_CSA3,
  //please reference HAL DMX_DESC_TYPE_E in tvos_hal_demux.h
  unsigned char audio_stream_descramble_type;
  //CA vendor ID
  unsigned int ca_vendor_id;
} ST_DVBProgCAInfo;

typedef struct
{
   unsigned char au8_iv[TVOS_MAX_KEY_LENGTH];
}ST_CAIVParam;

typedef struct
{
    unsigned char au8_ek1[TVOS_MAX_KEY_LENGTH];
    unsigned char au8_ek2[TVOS_MAX_KEY_LENGTH];
    //in case    auc_xxxx_ca_type==1(clear CW), this array saves cw instead of ecw
    unsigned char au8_ecw[TVOS_MAX_KEY_LENGTH];
}ST_CAKeyParam;

typedef struct
{
    /* 应用设置的宽高比 */
    V_UINT32 u32DispRatioW;
    V_UINT32 u32DispRatioH;
}ST_DispRatio;

typedef enum
{
    E_DVB_CA_SET_AUDIO_ODD_KEY,
    E_DVB_CA_SET_AUDIO_EVEN_KEY,
    E_DVB_CA_SET_VIDEO_ODD_KEY,
    E_DVB_CA_SET_VIDEO_EVEN_KEY,
    E_DVB_CA_SET_BUTT,
}E_DVB_CA_KEY_ID;

typedef enum
{
    E_DVB_CA_SET_AUDIO_ODD_KEY_IV,
    E_DVB_CA_SET_AUDIO_EVEN_KEY_IV,
    E_DVB_CA_SET_VIDEO_ODD_KEY_IV,
    E_DVB_CA_SET_VIDEO_EVEN_KEY_IV,
    E_DVB_CA_SET_IV_BUTT,
}E_DVB_CA_KEY_IV_ID;

typedef enum
{
    E_DVB_CC_MODE_BLACK = 0,
    E_DVB_CC_MODE_LAST,
}E_DVB_CC_MODE;


typedef enum
{
    E_DVB_TYPE_LIVE = 0,
    E_DVB_TYPE_VOD,
}E_DVB_TYPE;


typedef enum
{
    E_DVB_FREEZE_MODE_BLACK = 0,
    E_DVB_FREEZE_MODE_LAST,
}E_DVB_FREEZE_MODE;

typedef enum __tagSmeDVBPlayerDisPlayMode
{
    E_SME_DVBPLAYER_DIS_MODE_FULL = 0,
    E_SME_DVBPLAYER_DIS_MODE_FITIN,
    E_SME_DVBPLAYER_DIS_MODE_FITOUT,
    E_SME_DVBPLAYER_DIS_MODE_FITCENTER,
    E_SME_DVBPLAYER_DIS_MODE_BUTT,
}E_SME_DVBPLAYER_DISPLAY_MODE;

typedef enum __tagSmeDVBPlayerLogLevel
{
    E_SME_DVBPLAYER_LOG_LEVEL_NONE = 0,
    E_SME_DVBPLAYER_LOG_LEVEL_ERROR,
    E_SME_DVBPLAYER_LOG_LEVEL_WARNING,
    E_SME_DVBPLAYER_LOG_LEVEL_INFO,
    E_SME_DVBPLAYER_LOG_LEVEL_DEBUG,
    E_SME_DVBPLAYER_LOG_LEVEL_VERBOSE,
    E_SME_DVBPLAYER_LOG_LEVEL_BUTT,
}E_SME_DVBPLAYER_LOG_LEVEL;

typedef struct _tagSmeDVBDispRect
{
    V_INT32 i32StartX;
    V_INT32 i32StartY;
    V_INT32 i32Width;
    V_INT32 i32Height;
}ST_SME_DVBDISP_RECT, *PST_SME_DVBDISP_RECT;

typedef enum __tagSmeDVBPlayerNotify
{
    E_SME_DVBPLAYER_NOTIFY_ERROR = 11000,
    E_SME_DVBPLAYER_NOTIFY_PREPARED,
    E_SME_DVBPLAYER_NOTIFY_INFO,
    E_SME_DVBPLAYER_NOTIFY_PLAYER_STATE_CHANGED,
    E_SME_DVBPLAYER_NOTIFY_HAVE_NO_STREAM,
    E_SME_DVBPLAYER_NOTIFY_STREAM_RECOVER,
    E_SME_DVBPLAYER_NOTIFY_BUTT,
}E_SME_DVBPLAYER_NOTIFY;

typedef enum __tagSmeDVBPlayerError
{
    E_SME_DVBPLAYER_ERROR_NONE = ICS_SUCCESS,
    E_SME_DVBPLAYER_ERROR_UNKOWN = 10000,
    E_SME_DVBPLAYER_ERROR_UNSUPPORT_FORMAT,
    E_SME_DVBPLAYER_ERROR_UNSUPPORT_AUDIO_CODEC,
    E_SME_DVBPLAYER_ERROR_UNSUPPORT_VIDEO_CODEC,
    E_SME_DVBPLAYER_ERROR_UNSUPPORT_OP,
    E_SME_DVBPLAYER_ERROR_TIME_OUT,
    E_SME_DVBPLAYER_ERROR_STATE,
    E_SME_DVBPLAYER_ERROR_NOMEM,
    E_SME_DVBPLAYER_ERROR_INVALID_ARGS,
    E_SME_DVBPLAYER_ERROR_INVAILD_RECT,
    E_SME_DVBPLAYER_ERROR_BUTT,
}E_SME_DVBPLAYER_ERROR;

typedef enum _tagSmeDVBPlayerState
{
    E_SME_DVBPLAYER_STATE_PLAYING = 0,
    E_SME_DVBPLAYER_STATE_PAUSE,
    E_SME_DVBPLAYER_STATE_STOP,
    E_SME_DVBPLAYER_STATE_ERROR,
}E_SME_DVBPLAYER_STATE;

#endif//__SME_DVBPLAYER_TYPE_H__
