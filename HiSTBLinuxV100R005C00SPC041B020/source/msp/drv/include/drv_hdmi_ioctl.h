#ifndef __DRV_HDMI_IOCTL_H__
#define __DRV_HDMI_IOCTL_H__


#include "hi_module.h"
#include "hi_debug.h"
#if defined(HI_HDMI_SUPPORT_2_0)
#include "drv_hdmi_infoframe.h"
#include "drv_hdmi_edid.h"
#include "drv_hdmi_common.h"
#include "hi_drv_hdmi.h"
#else
#include "hi_unf_hdmi.h"
#include "hi_error_mpi.h"
#include "hi_drv_hdmi.h"
#endif

#ifdef __cplusplus
#if __cplusplus
	extern "C"{
#endif
#endif


#if defined(HI_HDMI_SUPPORT_2_0)
/*
** HDMI IOCTL Data Structure
*/

typedef struct
{
    HDMI_DEVICE_ID_E        enHdmiID;
    HDMI_HDR_ATTR_S         stHdrAttr; 
}DRV_HDMI_HDR_S;

typedef struct
{
    HDMI_DEVICE_ID_E        enHdmiID;
    HDMI_VIDEO_CAPABILITY_S stVideoCap;
}DRV_HDMI_VIDEO_CAPABILITY_S;

typedef struct
{
    HDMI_DEVICE_ID_E        enHdmiID;
    HDMI_VIDEO_ATTR_S       stVideoAttr;
}DRV_HDMI_DISP_ATTR_S;


typedef struct hiHDMI_OPEN_S
{
	HDMI_DEVICE_ID_E           enHdmiID;
    HDMI_DEFAULT_ACTION_E      enDefaultMode;
}HDMI_OPEN_S;

typedef struct
{
    HDMI_DEVICE_ID_E    enHdmiID;
    HDMI_EVENT_E        enEvent;    
}DRV_HDMI_EVENT_S;

typedef struct
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HDMI_AUDIO_CAPABILITY_S stAudioCap;    
}DRV_HDMI_AUDIO_CAPABILITY_S;

typedef struct
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HDMI_SINK_CAPABILITY_S stCap;    
}DRV_HDMI_SINK_CAPABILITY_S;

typedef struct
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HDMI_APP_ATTR_S        stAPPAttr;    
}DRV_HDMI_APP_ATTR_S;

typedef struct
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HDMI_VO_ATTR_S         stVOAttr;    
}DRV_HDMI_VO_ATTR_S;

typedef struct
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HDMI_INFOFRAME_ID_E    enInfoFrameId;
    HDMI_INFOFRAME_U       uInfoFrame;   
}DRV_HDMI_INFOFRAME_S;

typedef struct
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HI_BOOL                bAvMute;
}DRV_HDMI_AVMUTE_S;


typedef struct
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HDMI_DEEP_COLOR_E      enDeepColorMode;
}DRV_HDMI_DEEP_COLOR_S;


typedef struct
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HDMI_CEC_INFO_S        stCecInfo;
}DRV_HDMI_CEC_INFO_S;


#if 0
typedef struct 
{
    HI_U32                   u32VideoTiming;
    HI_U32                   u32Stereo;
}HDMI_VIDEOTIMING_S;

typedef struct 
{
    HI_U32                   u32VideoTiming;
}HDMI_PREVIDEOTIMING_S;

#endif


typedef struct 
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HDMI_CEC_CMD_S         CECCmd;
//    HI_U32                 u32Timeout;
}DRV_HDMI_CEC_CMD_S;

typedef struct 
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HDMI_EDID_RAWDATA_S    stEdidRaw;
}DRV_HDMI_EDID_RAWDATA_S;

typedef struct 
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HDMI_LOAD_KEY_S        stKey;
}DRV_HDMI_LOAD_KEY_S;

typedef struct 
{
    HDMI_DEVICE_ID_E            enHdmiID;
    HDMI_COMPAT_LOAD_KEY_S      stKey;
}DRV_HDMI_COMPAT_LOAD_KEY_S;

typedef struct 
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HDMI_AO_ATTR_S         stAOAttr;
}DRV_HDMI_AO_ATTR_S;

typedef struct 
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HDMI_STATUS_S          stHdmiStatus;
}DRV_HDMI_STATUS_S;


typedef struct 
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HI_BOOL                bEnable;
}DRV_HDMI_XVYCC_ENABLE_S;

typedef struct 
{
    HDMI_DEVICE_ID_E       enHdmiID;
    HI_U32                 u32Status;
}DRV_HDMI_PLAYSTATUS_S;


typedef struct 
{
	HDMI_DEVICE_ID_E       enHdmiID;
	HDMI_DELAY_S           stDelay;
}DRV_HDMI_DELAY_S;

#else
/*
** HDMI IOCTL Data Structure
*/
typedef struct hiHDMI_OPEN_S
{
	HI_UNF_HDMI_ID_E                  enHdmi;
    HI_UNF_HDMI_DEFAULT_ACTION_E      enDefaultMode;
    HI_U32         u32ProcID;
}HDMI_OPEN_S;

typedef struct hiHDMI_DEINIT_S
{
    HI_U32                          NoUsed;
}HDMI_DEINIT_S;

typedef struct hiHDMI_INIT_S
{
    HI_U32                          NoUsed;
}HDMI_INIT_S;

typedef struct hiHDMI_COLSE_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          NoUsed;
}HDMI_CLOSE_S;

typedef struct hiHDMI_START_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          NoUsed;
}HDMI_START_S;

typedef struct hiHDMI_STOP_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          NoUsed;
}HDMI_STOP_S;

typedef struct hiHDMI_POLL_EVENT_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_EVENT_TYPE_E        Event;
    HI_U32                          u32ProcID;
}HDMI_POLL_EVENT_S;

typedef struct hiHDMI_SINK_CAPABILITY_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_EDID_BASE_INFO_S         SinkCap;
}HDMI_SINK_CAPABILITY_S;

typedef struct hiHDMI_PORT_ATTR_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HDMI_APP_ATTR_S                 stHdmiAppAttr;
}HDMI_PORT_ATTR_S;

typedef struct hiHDMI_INFORFRAME_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_INFOFRAME_TYPE_E    enInfoFrameType;
    HI_UNF_HDMI_INFOFRAME_S         InfoFrame;
}HDMI_INFORFRAME_S;

typedef struct hiHDMI_AVMUTE_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          AVMuteEnable;
}HDMI_AVMUTE_S;

typedef struct hiHDMI_VIDEOTIMING_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          VideoTiming;
    HI_DRV_DISP_STEREO_E            enStereo;
}HDMI_VIDEOTIMING_S;

typedef struct hiHDMI_PREVIDEOTIMING_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          VideoTiming;
}HDMI_PREVIDEOTIMING_S;

typedef struct hiHDMI_DEEPCOLOR_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_DEEP_COLOR_E        enDeepColor;
}HDMI_DEEPCOLORC_S;

typedef struct hiHDMI_SET_XVYCC_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          xvYCCEnable;
}HDMI_SET_XVYCC_S;

typedef struct hiHDMI_CEC_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_CEC_CMD_S           CECCmd;
    HI_U32                          timeout;
}HDMI_CEC_S;


typedef struct
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_CEC_STATUS_S        stStatus;
}HDMI_CEC_STATUS;

typedef struct hiHDMI_EDID_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U8                           u8EdidValid;
    HI_U32                          u32Edidlength;
    HI_U8                           u8Edid[512];
}HDMI_EDID_S;

typedef struct hiHDMI_PLAYSTAUS_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          u32PlayStaus;
}HDMI_PLAYSTAUS_S;

typedef struct hiHDMI_CEC_ENABLE_S
{
    HI_U32                          NoUsed;
}HDMI_CEC_ENABlE_S;

typedef struct hiHDMI_CEC_DISABLE_S
{
    HI_U32                          NoUsed;
}HDMI_CEC_DISABLE_S;

typedef struct hiHDMI_REGCALLBACKFUNC_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
	HI_U32							u32CallBackAddr;
}HDMI_REGCALLBACKFUNC_S;

typedef struct hiHDMI_LOADKEY_S
{
	HI_UNF_HDMI_ID_E               enHdmi;
	HI_UNF_HDMI_LOAD_KEY_S         stLoadKey;
}HDMI_LOADKEY_S;

typedef struct hiHDMI_GETPROCID_S
{
	HI_UNF_HDMI_ID_E               enHdmi;
	HI_U32         u32ProcID;
}HDMI_GET_PROCID_S;

typedef struct hiHDMI_GETAOATTR_S
{
	HI_UNF_HDMI_ID_E               enHdmi;
	HDMI_AUDIO_ATTR_S              stAudAttr;
}HDMI_GETAOATTR_S;

typedef struct hiHDMI_STATUS_S
{
	HI_UNF_HDMI_ID_E               enHdmi;
	HI_UNF_HDMI_STATUS_S           stStatus;
}HDMI_STATUS_S;

typedef struct hiHDMI_SETFMT_DELAY_S
{
	HI_UNF_HDMI_ID_E               enHdmi;
	HI_UNF_HDMI_DELAY_S            stDelay;
}HDMI_DELAY_S;

#endif


enum hiIOCTL_HDMI_E
{
    IOCTL_HDMI_INIT = 0X01,
    IOCTL_HDMI_DEINIT,
    IOCTL_HDMI_OPEN,
    IOCTL_HDMI_CLOSE,
    IOCTL_HDMI_START,
    IOCTL_HDMI_STOP,
    IOCTL_HDMI_GET_AUDIO_CAPABILITY,
    IOCTL_HDMI_GET_SINK_CAPABILITY,
    IOCTL_HDMI_POLL_EVENT,
    IOCTL_HDMI_GET_ATTR,
    IOCTL_HDMI_SET_ATTR,
    IOCTL_HDMI_GET_INFOFRAME,
    IOCTL_HDMI_SET_INFOFRAME,
    IOCTL_HDMI_AVMUTE,
    IOCTL_HDMI_PRE_VIDEO_TIMING,
    IOCTL_HDMI_GET_VO_ATTR,
    IOCTL_HDMI_SET_VO_ATTR,
    IOCTL_HDMI_GET_DEEPCOLOR,
    IOCTL_HDMI_SET_DEEPCOLOR,
    IOCTL_HDMI_XVYCC,
    IOCTL_HDMI_SET_CEC,
    IOCTL_HDMI_GET_CEC,
    IOCTL_HDMI_GET_CECSTATUS,
    IOCTL_HDMI_UPDATE_EDID,
    IOCTL_HDMI_READ_EDID,
    IOCTL_HDMI_GET_HDMI_PLAYSTATUS,
    IOCTL_HDMI_CEC_ENABLE,
    IOCTL_HDMI_CEC_DISABLE,
    IOCTL_HDMI_REG_CALLBACK_FUNC,
    IOCTL_HDMI_LOAD_KEY,
    IOCTL_HDMI_GET_PROCID,
    IOCTL_HDMI_RELEASE_PROCID,
    IOCTL_HDMI_GET_AO_ATTR,
    IOCTL_HDMI_SET_AO_ATTR,
    IOCTL_HDMI_GET_STATUS,
    IOCTL_HDMI_GET_DELAY,
    IOCTL_HDMI_SET_DELAY,
    IOCTL_HDMI_GET_HDR_ATTR,
    IOCTL_HDMI_SET_HDR_ATTR,
    IOCTL_HDMI_GET_VIDEO_CAPABILITY,
    IOCTL_HDMI_SET_DISP_ATTR,
    IOCTL_HDMI_MAX,
};

/*
**IOCTRL Between User/Kernel Level
*/

#if defined(HI_HDMI_SUPPORT_2_0)
#define CMD_HDMI_INIT                _IO(HI_ID_HDMI, IOCTL_HDMI_INIT)
#define CMD_HDMI_DEINIT              _IO(HI_ID_HDMI, IOCTL_HDMI_DEINIT)
#define CMD_HDMI_OPEN                _IOWR(HI_ID_HDMI, IOCTL_HDMI_OPEN,  HDMI_OPEN_S)
#define CMD_HDMI_CLOSE               _IOWR(HI_ID_HDMI, IOCTL_HDMI_CLOSE, HDMI_DEVICE_ID_E)
#define CMD_HDMI_START               _IOWR(HI_ID_HDMI, IOCTL_HDMI_START, HDMI_DEVICE_ID_E)
#define CMD_HDMI_STOP                _IOWR(HI_ID_HDMI, IOCTL_HDMI_STOP,  HDMI_DEVICE_ID_E)
#define CMD_HDMI_GET_AUDIO_CAPABILITY _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_AUDIO_CAPABILITY, DRV_HDMI_AUDIO_CAPABILITY_S)
#define CMD_HDMI_GET_SINK_CAPABILITY _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_SINK_CAPABILITY, DRV_HDMI_SINK_CAPABILITY_S)
#define CMD_HDMI_POLL_EVENT          _IOWR(HI_ID_HDMI, IOCTL_HDMI_POLL_EVENT,     DRV_HDMI_EVENT_S)
#define CMD_HDMI_GET_ATTR            _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_ATTR,       DRV_HDMI_APP_ATTR_S)
#define CMD_HDMI_SET_ATTR            _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_ATTR,       DRV_HDMI_APP_ATTR_S)
#define CMD_HDMI_GET_INFOFRAME       _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_INFOFRAME,  DRV_HDMI_INFOFRAME_S)
#define CMD_HDMI_SET_INFOFRAME       _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_INFOFRAME,  DRV_HDMI_INFOFRAME_S)
#define CMD_HDMI_SET_AVMUTE          _IOWR(HI_ID_HDMI, IOCTL_HDMI_AVMUTE,         DRV_HDMI_AVMUTE_S)
#define CMD_HDMI_GET_VO_ATTR         _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_VO_ATTR,    DRV_HDMI_VO_ATTR_S)
#define CMD_HDMI_SET_VO_ATTR         _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_VO_ATTR,    DRV_HDMI_VO_ATTR_S)
#define CMD_HDMI_GET_DEEPCOLOR       _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_DEEPCOLOR,  DRV_HDMI_DEEP_COLOR_S)
#define CMD_HDMI_SET_DEEPCOLOR       _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_DEEPCOLOR,  DRV_HDMI_DEEP_COLOR_S)
#define CMD_HDMI_SET_XVYCC           _IOWR(HI_ID_HDMI, IOCTL_HDMI_XVYCC,          DRV_HDMI_XVYCC_ENABLE_S)
#define CMD_HDMI_GET_CEC             _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_CEC,        DRV_HDMI_CEC_CMD_S)
#define CMD_HDMI_SET_CEC             _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_CEC,        DRV_HDMI_CEC_CMD_S)
#define CMD_HDMI_GET_CECSTATUS       _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_CECSTATUS,  DRV_HDMI_CEC_INFO_S)
#define CMD_HDMI_UPDATE_EDID         _IOWR(HI_ID_HDMI, IOCTL_HDMI_UPDATE_EDID,      DRV_HDMI_EDID_RAWDATA_S) 
#define CMD_HDMI_GET_HDMI_PLAYSTATUS _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_HDMI_PLAYSTATUS,  DRV_HDMI_PLAYSTATUS_S) 
#define CMD_HDMI_CEC_ENABLE          _IOWR(HI_ID_HDMI, IOCTL_HDMI_CEC_ENABLE,      HDMI_DEVICE_ID_E) 
#define CMD_HDMI_CEC_DISABLE         _IOWR(HI_ID_HDMI, IOCTL_HDMI_CEC_DISABLE,     HDMI_DEVICE_ID_E) 
#define CMD_HDMI_LOAD_KEY            _IOWR(HI_ID_HDMI, IOCTL_HDMI_LOAD_KEY,        DRV_HDMI_LOAD_KEY_S)
#define CMD_HDMI_COMPAT_LOAD_KEY     _IOWR(HI_ID_HDMI, IOCTL_HDMI_LOAD_KEY,        DRV_HDMI_COMPAT_LOAD_KEY_S)

#define CMD_HDMI_GET_AO_ATTR         _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_AO_ATTR,  DRV_HDMI_AO_ATTR_S)
#define CMD_HDMI_SET_AO_ATTR         _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_AO_ATTR,  DRV_HDMI_AO_ATTR_S)
#define CMD_HDMI_GET_STATUS          _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_STATUS,   DRV_HDMI_STATUS_S)
#define CMD_HDMI_GET_DELAY           _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_DELAY,    DRV_HDMI_DELAY_S)
#define CMD_HDMI_SET_DELAY           _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_DELAY,    DRV_HDMI_DELAY_S)

#define CMD_HDMI_GET_HDR_ATTR        _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_HDR_ATTR, DRV_HDMI_HDR_S)
#define CMD_HDMI_SET_HDR_ATTR        _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_HDR_ATTR, DRV_HDMI_HDR_S)
#define CMD_HDMI_GET_VIDEO_CAPABILITY _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_VIDEO_CAPABILITY, DRV_HDMI_VIDEO_CAPABILITY_S)

#else
#define CMD_HDMI_INIT                _IOWR(HI_ID_HDMI, IOCTL_HDMI_INIT,           HDMI_INIT_S)
#define CMD_HDMI_DEINIT              _IOWR(HI_ID_HDMI, IOCTL_HDMI_DEINIT,         HDMI_DEINIT_S)
#define CMD_HDMI_OPEN                _IOWR(HI_ID_HDMI, IOCTL_HDMI_OPEN,           HDMI_OPEN_S)
#define CMD_HDMI_CLOSE               _IOWR(HI_ID_HDMI, IOCTL_HDMI_CLOSE,          HDMI_CLOSE_S)
#define CMD_HDMI_START               _IOWR(HI_ID_HDMI, IOCTL_HDMI_START,          HDMI_START_S)
#define CMD_HDMI_STOP                _IOWR(HI_ID_HDMI, IOCTL_HDMI_STOP,           HDMI_STOP_S)
#define CMD_HDMI_SINK_CAPABILITY     _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_SINK_CAPABILITY,HDMI_SINK_CAPABILITY_S)
#define CMD_HDMI_POLL_EVENT          _IOWR(HI_ID_HDMI, IOCTL_HDMI_POLL_EVENT,     HDMI_POLL_EVENT_S)
#define CMD_HDMI_GET_ATTR            _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_ATTR,       HDMI_PORT_ATTR_S)
#define CMD_HDMI_SET_ATTR            _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_ATTR,       HDMI_PORT_ATTR_S)
#define CMD_HDMI_GET_INFORFRAME      _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_INFOFRAME, HDMI_INFORFRAME_S)
#define CMD_HDMI_SET_INFORFRAME      _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_INFOFRAME, HDMI_INFORFRAME_S)
#define CMD_HDMI_SET_AVMUTE          _IOWR(HI_ID_HDMI, IOCTL_HDMI_AVMUTE,         HDMI_AVMUTE_S)
#define CMD_HDMI_VIDEO_TIMING        _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_VO_ATTR,   HDMI_VIDEOTIMING_S)
#define CMD_HDMI_GET_DEEPCOLOR       _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_DEEPCOLOR,  HDMI_DEEPCOLORC_S)
#define CMD_HDMI_SET_DEEPCOLOR       _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_DEEPCOLOR,  HDMI_DEEPCOLORC_S)
#define CMD_HDMI_SET_XVYCC           _IOWR(HI_ID_HDMI, IOCTL_HDMI_XVYCC,          HDMI_SET_XVYCC_S)
#define CMD_HDMI_GET_CEC             _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_CEC,        HDMI_CEC_S)
#define CMD_HDMI_SET_CEC             _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_CEC,        HDMI_CEC_S)
#define CMD_HDMI_CECSTATUS           _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_CECSTATUS,      HDMI_CEC_STATUS)
#define CMD_HDMI_PREVTIMING          _IOWR(HI_ID_HDMI, IOCTL_HDMI_PRE_VIDEO_TIMING,    HDMI_PREVIDEOTIMING_S)
#define CMD_HDMI_FORCE_GET_EDID      _IOWR(HI_ID_HDMI, IOCTL_HDMI_UPDATE_EDID,      HDMI_EDID_S) 
#define CMD_HDMI_READ_EDID           _IOWR(HI_ID_HDMI, IOCTL_HDMI_READ_EDID,        HDMI_EDID_S) 
#define CMD_HDMI_GET_HDMI_PLAYSTAUS  _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_HDMI_PLAYSTATUS,  HDMI_PLAYSTAUS_S) 
#define CMD_HDMI_CEC_ENABLE          _IOWR(HI_ID_HDMI, IOCTL_HDMI_CEC_ENABLE,     HDMI_CEC_ENABlE_S) 
#define CMD_HDMI_CEC_DISABLE         _IOWR(HI_ID_HDMI, IOCTL_HDMI_CEC_DISABLE,    HDMI_CEC_DISABLE_S) 
#define CMD_HDMI_REG_CALLBACK_FUNC   _IOWR(HI_ID_HDMI, IOCTL_HDMI_REG_CALLBACK_FUNC, HDMI_REGCALLBACKFUNC_S) 
#define CMD_HDMI_LOADKEY             _IOWR(HI_ID_HDMI, IOCTL_HDMI_LOAD_KEY, HDMI_LOADKEY_S)
#define CMD_HDMI_GET_PROCID          _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_PROCID, HDMI_GET_PROCID_S)
#define CMD_HDMI_RELEASE_PROCID      _IOWR(HI_ID_HDMI, IOCTL_HDMI_RELEASE_PROCID, HDMI_GET_PROCID_S)
#define CMD_HDMI_GET_AO_ATTR         _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_AO_ATTR, HDMI_GETAOATTR_S)
#define CMD_HDMI_AUDIO_CHANGE        _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_AO_ATTR, HDMI_GETAOATTR_S)
#define CMD_HDMI_GET_STATUS          _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_STATUS, HDMI_STATUS_S)
#define CMD_HDMI_GET_DELAY           _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_DELAY,HDMI_DELAY_S)
#define CMD_HDMI_SET_DELAY           _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_DELAY,HDMI_DELAY_S)
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

