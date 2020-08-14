/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name             :   hi_mpi_vdec.h
  Version               :   Initial Draft
  Author                :   Hisilicon multimedia software group
  Created               :   2006/06/12
  Last Modified         :
  Description           :
  Function List         :
  History               :
  1.Date                :   2006/06/12
    Author              :
Modification            :   Created file
******************************************************************************/

/******************************* Include Files *******************************/

#ifndef  __HI_MPI_VDEC_H__
#define  __HI_MPI_VDEC_H__

#include "hi_unf_avplay.h"
#include "hi_drv_video.h"
#include "hi_drv_vpss.h"
#include "hi_video_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/****************************** Macro Definition *****************************/

#ifndef HI_VDEC_REG_CODEC_SUPPORT
#define HI_VDEC_REG_CODEC_SUPPORT (0)
#endif

#ifndef HI_VDEC_MJPEG_SUPPORT
#define HI_VDEC_MJPEG_SUPPORT (0)
#endif

#ifndef HI_VDEC_USERDATA_CC_SUPPORT
#define HI_VDEC_USERDATA_CC_SUPPORT (0)
#endif

#ifndef HI_VDEC_DFS_SUPPORT
#define HI_VDEC_DFS_SUPPORT (0)
#endif
#define MAX_VDEC_EXT_BUF_NUM 16
/*************************** Structure Definition ****************************/

typedef struct hiVDEC_ES_BUF_S
{
    HI_U8*  pu8Addr;      /*Virtual address of the buffer user.*/
    HI_U32  u32PhyAddr;   /*Physical address of the buffer */
    HI_U32  u32BufSize;   /*Buffer size, in the unit of byte.*/
    HI_U64  u64Pts;       /*PTS of the data filled in a buffer.*/
    HI_BOOL bEndOfFrame;  /* End of Frame flag */
    HI_BOOL bDiscontinuous;/* Stream continue or not */
}VDEC_ES_BUF_S, *PTR_VDEC_ES_BUF_S;
typedef struct hiVDEC_ES_BUF_PRI_S
{
    HI_U64  u64Addr;      /*Virtual address of the buffer user.*/
    HI_U32  u32PhyAddr;   /*Physical address of the buffer */
    HI_U32  u32BufSize;   /*Buffer size, in the unit of byte.*/
    HI_U64  u64Pts;       /*PTS of the data filled in a buffer.*/
    HI_BOOL bEndOfFrame;  /* End of Frame flag */
    HI_BOOL bDiscontinuous;/* Stream continue or not */
}VDEC_ES_BUF_PRI_S, *PTR_VDEC_ES_BUF_PRI_S;

typedef struct hiVDEC_USER_FRAME_BUF_S
{
    HI_U8*  pu8Addr;      /*Virtual address of the buffer user.*/
    HI_U32  u32PhyAddr;   /*Physical address of the buffer */
    HI_U32  u32BufSize;   /*Buffer size, in the unit of byte.*/
}VDEC_USER_FRAME_BUF_S;

typedef struct tagVDEC_PROBE_STREAM_INFO_S{
    HI_BOOL  bProbeCodecTypeChangeFlag;
    HI_UNF_VCODEC_TYPE_E enCodecType;
}VDEC_PROBE_STREAM_INFO_S;

typedef struct hiVDEC_STATUSINFO_S
{
    HI_U32  u32BufferSize;      /* Total buffer size, in the unit of byte.*/
    HI_U32  u32BufferAvailable; /* Available buffer, in the unit of byte.*/
    HI_U32  u32BufferUsed;      /* Used buffer, in the unit of byte.*/
    HI_U32  u32VfmwFrmNum;
    HI_U32  u32VfmwStrmSize;
    HI_U32  u32VfmwStrmNum;     /* The un-decoded stream seg num produced by SCD */
    HI_U32  u32VfmwTotalDispFrmNum; /* total display num ( plus extra_disp ) */
    HI_U32  u32FieldFlag;       /* 0:frame 1:field */
    HI_UNF_VCODEC_FRMRATE_S stVfmwFrameRate;/* vfmw frame rate */
    HI_U32  u32StrmInBps;
    HI_U32  u32TotalDecFrmNum;
    HI_U32  u32TotalErrFrmNum;
    HI_U32  u32TotalErrStrmNum;
    HI_U32  u32FrameBufNum;     /* frame num in buffer to display */
    HI_BOOL bEndOfStream;       /* There's no enough stream in buffer to decode a frame */
    HI_BOOL bAllPortCompleteFrm;
}VDEC_STATUSINFO_S;

#if 1
typedef struct hiVDEC_FRMSTATUSINFO_S
{
    HI_U32  u32DecodedFrmNum;  /* decoded, but not ready to display.*/
    HI_U32  u32StrmSize;       /* the un-decoded stream seg produced by SCD */
    HI_U32  u32StrmInBps;      /*average Bps */
    HI_U32  u32OutBufFrmNum;   /*decoded, and ready to display*/
}VDEC_FRMSTATUSINFO_S;
typedef struct tagVDEC_FRMSTATUSINFOWITHPORT
{
    HI_HANDLE hPort;
    VDEC_FRMSTATUSINFO_S stVdecFrmStatus;
}VDEC_FRMSTATUSINFOWITHPORT_S;
#endif

typedef struct hiVDEC_EVENT_S
{
    HI_BOOL  bNewFrame;
    HI_BOOL  bNewSeq;
    HI_BOOL  bNewUserData;
    HI_BOOL  bErrorFrame;   /*The decoder find the start code of END OF FRAME, but has not received any slice data, so the decoder can not decode and output one frame*/

    HI_BOOL  bFirstValidPts;
    HI_U32   u32FirstValidPts;
    HI_BOOL  bSecondValidPts;
    HI_U32   u32SecondValidPts;

    HI_BOOL  bNormChange;
    HI_UNF_NORMCHANGE_PARAM_S stNormChangeParam;

    HI_BOOL  bFramePackingChange;
    HI_UNF_VIDEO_FRAME_PACKING_TYPE_E enFramePackingType;

    HI_BOOL  bIFrameErr;
    HI_BOOL  bUnSupportStream;
	HI_BOOL  bDecErr;
    HI_U32   u32ErrRatio;
	VDEC_PROBE_STREAM_INFO_S stProbeStreamInfo;
} VDEC_EVENT_S;

typedef enum hiVDEC_DISCARD_MODE_E
{
    VDEC_DISCARD_STOP,      /* Don't discard frame, or stop when current status is discarding */
    VDEC_DISCARD_START,     /* Start to discard frame until mode switch to VDEC_DISCARD_STOP */
    VDEC_DISCARD_BY_NUM,    /* Discard appointed number frames */
    VDEC_DISCARD_BUTT
}VDEC_DISCARD_MODE_E;

typedef enum hiVDEC_PORT_ABILITY_E
{
    VDEC_PORT_HD,
    VDEC_PORT_SD,
    VDEC_PORT_STR,/*virtual window*/
    VDEC_PORT_BUTT
}VDEC_PORT_ABILITY_E;
typedef struct hiVDEC_PORT_CFG_S
{
    HI_HANDLE *phPort;
    VDEC_PORT_ABILITY_E ePortAbility;
}VDEC_PORT_CFG_S;

typedef struct hiVDEC_DISCARD_FRAME_S
{
    VDEC_DISCARD_MODE_E enMode; /* Discard mode */
    HI_U32              u32Num; /* Discard number, usable when mode is VDEC_DISCARD_BY_NUM */
}VDEC_DISCARD_FRAME_S;

//add by l00225186
typedef int (*PFN_VDEC_Chan_VOAcqFrame)(HI_HANDLE, HI_DRV_VIDEO_FRAME_S*);
typedef int (*PFN_VDEC_Chan_VORlsFrame)(HI_HANDLE, HI_DRV_VIDEO_FRAME_S*);
typedef int (*PFN_VDEC_Chan_VOChangeWinInfo)(HI_HANDLE,HI_DRV_WIN_PRIV_INFO_S*);
//add by l00225186
typedef struct tagVDEC_PORT_PARAM_S
{
   /*提供给VO,收帧，释放帧，处理窗口信息改变的函数*/
  PFN_VDEC_Chan_VOAcqFrame pfVOAcqFrame;
  PFN_VDEC_Chan_VORlsFrame pfVORlsFrame;
  PFN_VDEC_Chan_VOChangeWinInfo pfVOSendWinInfo;
}VDEC_PORT_PARAM_S;
typedef struct tagVDEC_PORT_PARAM_WITHPORT_S
{
  HI_HANDLE hPort;
  VDEC_PORT_PARAM_S stVdecPortParam;
}VDEC_PORT_PARAM_WITHPORT_S;
typedef enum hiVDEC_PORT_TYPE_E
{
    VDEC_PORT_TYPE_MASTER,
    VDEC_PORT_TYPE_SLAVE,
    VDEC_PORT_TYPE_VIRTUAL,
    VDEC_PORT_TYPE_BUTT
}VDEC_PORT_TYPE_E;

typedef struct tagVDEC_PORT_TYPE_WITHPORT_S
{
  HI_HANDLE hPort;
  VDEC_PORT_TYPE_E enPortType;
}VDEC_PORT_TYPE_WITHPORT_S;

typedef struct tagVDEC_PORT_ATTR_WITHHANDLE_S
{
    HI_HANDLE                   hPort;
    HI_DRV_VPSS_PORT_CFG_S      stPortCfg;
}VDEC_PORT_ATTR_WITHHANDLE_S;

typedef struct hiUNF_VDEC_BUFF_ATTR_S
{
    HI_U8* pu8UsrVirAddr[MAX_VDEC_EXT_BUF_NUM];  /**<User virtual address *//**<CNcomment: 用户态虚拟地址*/
    HI_U32 u32PhyAddr[MAX_VDEC_EXT_BUF_NUM];     /**<Physical address *//**<CNcomment: 物理地址*/
    HI_U8* pu8UsrVirAddr_meta[MAX_VDEC_EXT_BUF_NUM];  /**<User virtual address *//**<CNcomment: 用户态虚拟地址*/
    HI_U32 u32PhyAddr_meta[MAX_VDEC_EXT_BUF_NUM];     /**<Physical address *//**<CNcomment: 物理地址*/

    HI_S32 s32FrameBufFd[MAX_VDEC_EXT_BUF_NUM];
    HI_S32 s32MetadataBufFd[MAX_VDEC_EXT_BUF_NUM];
    HI_U32 u32PrivOffset_meta;

    HI_U32 u32BufNum;                          /**<Buffer number *//**<CNcomment: 缓冲区个数*/
	HI_U32 u32BufSize;
	HI_U32 u32MetaBufSize;
    HI_U32 u32Stride;                          /**<Stride of external frame buffer *//**<CNcomment:外部帧存的stride*/
	HI_U32 bTvp;
	HI_BOOL bVideoBypass;
} VDEC_BUFFER_ATTR_S;
typedef struct hiUNF_VDEC_BUFF_ATTR_PRI_S
{
    HI_U64 u64UsrVirAddr[MAX_VDEC_EXT_BUF_NUM];  /**<User virtual address *//**<CNcomment: 用户态虚拟地址*/
    HI_U32 u32PhyAddr[MAX_VDEC_EXT_BUF_NUM];     /**<Physical address *//**<CNcomment: 物理地址*/
    HI_U64 u64UsrVirAddr_meta[MAX_VDEC_EXT_BUF_NUM];  /**<User virtual address *//**<CNcomment: 用户态虚拟地址*/
    HI_U32 u32PhyAddr_meta[MAX_VDEC_EXT_BUF_NUM];     /**<Physical address *//**<CNcomment: 物理地址*/

    HI_S32 s32FrameBufFd[MAX_VDEC_EXT_BUF_NUM];
    HI_S32 s32MetadataBufFd[MAX_VDEC_EXT_BUF_NUM];
    HI_U32 u32PrivOffset_meta;

    HI_U32 u32BufNum;                          /**<Buffer number *//**<CNcomment: 缓冲区个数*/
	HI_U32 u32BufSize;
	HI_U32 u32MetaBufSize;
    HI_U32 u32Stride;                          /**<Stride of external frame buffer *//**<CNcomment:外部帧存的stride*/
	HI_U32 bTvp;
	HI_BOOL bVideoBypass;
} VDEC_BUFFER_ATTR_PRI_S;

typedef enum hiVDEC_FRAMEBUFFER_MODE_E{
	VDEC_BUF_VPSS_ALLOC_MANAGE = 0,/**vpss buffer should alloc and manage by vpss*//**<CNcomment:vpss的buffer的分配和管理有vpss模块自己管理*/
    VDEC_BUF_USER_ALLOC_MANAGE,    /**vpss buffer should alloc and manage by user*//**<CNcomment:vpss的buffer分配由vdec的上层用户来分配，管理有vdec负责*/
    VDEC_BUF_TYPE_BUTT
}VDEC_FRAMEBUFFER_MODE_E;
typedef enum hiVDEC_FRAMEBUFFER_STATE_E{
	VDEC_BUF_STATE_EMPTY = 0,/**the frame buffer have no data*//**<CNcomment:要查询的buffer里面没有数据*/
    VDEC_BUF_STATE_FULL,     /**the frame buffer have data*//**<CNcomment:要查询的buffer里面已经被填充了有效数据*/
    VDEC_BUF_STATE_IN_USE,   /**the frame buffer is in using*//**<CNcomment:要查询的buffer正在被填充数据的过程中*/
    VDEC_BUF_STATE_BUTT
}VDEC_FRAMEBUFFER_STATE_E;
typedef struct hi_VDEC_BUFF_INFO_S
{
    HI_U32 u32PhyAddr;
	VDEC_FRAMEBUFFER_STATE_E enBufState;
} VDEC_BUFFER_INFO_S;

typedef enum hiVDEC_EXTBUFFER_STATE_E
{
    VDEC_EXTBUFFER_STATE_START,     /* control VPSS start use the extern buffer*/
    VDEC_EXTBUFFER_STATE_STOP,      /* control VPSS stop use the extern buffer*/
    VDEC_EXTBUFFER_STATE_BUTT
}VDEC_EXTBUFFER_STATE_E;

typedef struct tagVDEC_RESOLUTION_ATTR_S
{
    HI_S32                   s32Width;
	HI_S32                   s32Height;
}VDEC_RESOLUTION_ATTR_S;

typedef enum hiVDEC_PRE_CAP
{
    VDEC_PRE_CAP_1080P,
    VDEC_PRE_CAP_MVC,
    VDEC_PRE_CAP_2160P,
    VDEC_PRE_CAP_4K,
    VDEC_PRE_CAP_BUTT
}VDEC_PRE_CAP_E;

#ifdef HI_HDR_SUPPORT
typedef enum hiVDEC_HDR_TYPE_E
{
    HI_VDEC_HDR_CHAN_TYPE_BL = 0,    /**<CNcomment: VDEC BL解码通道.*/
    HI_VDEC_HDR_CHAN_TYPE_EL,        /**<CNcomment: VDEC EL解码通道.*/
    HI_VDEC_HDR_CHAN_TYPE_BUTT
} VDEC_HDR_CHAN_TYPE_E;

typedef struct hiVDEC_HDR_ATTR_S
{
    VDEC_HDR_CHAN_TYPE_E       enVdecHDRType;    /**<CNcomment: VDEC解码通道类型.*/
    HI_UNF_AVPLAY_HDR_ATTR_S   AvplayHDRAttr;    /**<CNcomment: HDR配置属性.*/
} VDEC_HDR_ATTR_S;
#endif
typedef struct hiVDEC_RECVFRM_OPT_S
{
    HI_U32                          TimeOut;
    HI_DRV_VIDEO_FRAME_PACKAGE_S   *pVidFrmPakage;
} VDEC_RECVFRM_OPT_S;
/****************************** API Declaration ******************************/

HI_S32 HI_MPI_VDEC_Init(HI_VOID);
HI_S32 HI_MPI_VDEC_DeInit(HI_VOID);
HI_S32 HI_MPI_VDEC_RegisterVcodecLib(const HI_CHAR *pszCodecDllName);
HI_S32 HI_MPI_VDEC_RegisterVcodecFunc(const HI_CODEC_S *pszCodec);
HI_S32 HI_MPI_VDEC_AllocChan(HI_HANDLE *phHandle, const HI_UNF_AVPLAY_OPEN_OPT_S *pstMaxCapbility);
HI_S32 HI_MPI_VDEC_FreeChan(HI_HANDLE hVdec);
HI_S32 HI_MPI_VDEC_SetChanAttr(HI_HANDLE hVdec, const HI_UNF_VCODEC_ATTR_S *pstAttr);
HI_S32 HI_MPI_VDEC_GetChanAttr(HI_HANDLE hVdec, HI_UNF_VCODEC_ATTR_S *pstAttr);
#ifndef HI_TEE_SUPPORT
HI_S32 HI_MPI_VDEC_ChanBufferInit(HI_HANDLE hVdec, HI_U32 u32BufSize, HI_HANDLE hDmxVidChn);
#else
HI_S32 HI_MPI_VDEC_ChanBufferInit(HI_HANDLE hVdec, HI_HANDLE hDmxVidChn, VDEC_BUFFER_ATTR_S *pstBufAttr);
#endif
HI_S32 HI_MPI_VDEC_ChanBufferDeInit(HI_HANDLE hVdec);
HI_S32 HI_MPI_VDEC_ResetChan(HI_HANDLE hVdec);
HI_S32 HI_MPI_VDEC_ChanStart(HI_HANDLE hVdec);
HI_S32 HI_MPI_VDEC_ChanStop(HI_HANDLE hVdec);
HI_S32 HI_MPI_VDEC_ChanGetBuffer(HI_HANDLE hVdec, HI_U32 u32RequestSize, VDEC_ES_BUF_S *pstBuf);
HI_S32 HI_MPI_VDEC_ChanPutBuffer(HI_HANDLE hVdec, const VDEC_ES_BUF_S *pstBuf);
HI_S32 HI_MPI_VDEC_ChanRecvFrm(HI_HANDLE hVdec, HI_DRV_VIDEO_FRAME_S* pstFrameInfo);
HI_S32 HI_MPI_VDEC_ChanRlsFrm(HI_HANDLE hVdec, HI_DRV_VIDEO_FRAME_S* pstFrameInfo);
HI_S32 HI_MPI_VDEC_ChanIFrameDecode(HI_HANDLE hVdec, HI_UNF_AVPLAY_I_FRAME_S *pstIFrameStream,HI_BOOL bCapture);
HI_S32 HI_MPI_VDEC_ChanIFrameRelease(HI_HANDLE hVdec, HI_DRV_VIDEO_FRAME_S *pstVoFrameInfo);
HI_S32 HI_MPI_VDEC_GetChanStatusInfo(HI_HANDLE hVdec, VDEC_STATUSINFO_S *pstStatusInfo);
HI_S32 HI_MPI_VDEC_GetChanStreamInfo(HI_HANDLE hVdec, HI_UNF_VCODEC_STREAMINFO_S *pstStreamInfo);
HI_S32 HI_MPI_VDEC_CheckNewEvent(HI_HANDLE hVdec, VDEC_EVENT_S *pstNewEvent);
HI_S32 HI_MPI_VDEC_ReadNewFrame(HI_HANDLE hVdec, HI_DRV_VIDEO_FRAME_S *pstNewFrame);
HI_S32 HI_MPI_VDEC_ChanRecvUsrData(HI_HANDLE hVdec, HI_UNF_VIDEO_USERDATA_S *pstUsrData);
HI_S32 HI_MPI_VDEC_SetChanFrmRate(HI_HANDLE hVdec, HI_UNF_AVPLAY_FRMRATE_PARAM_S *pstFrmRate);
HI_S32 HI_MPI_VDEC_GetChanFrmRate(HI_HANDLE hVdec, HI_UNF_AVPLAY_FRMRATE_PARAM_S *pstFrmRate);
HI_S32 HI_MPI_VDEC_SetEosFlag(HI_HANDLE hVdec);
HI_S32 HI_MPI_VDEC_DiscardFrame(HI_HANDLE hVdec, VDEC_DISCARD_FRAME_S* pstParam);
HI_S32 HI_MPI_VDEC_CreatePort(HI_HANDLE hVdec, HI_HANDLE *phPort, VDEC_PORT_ABILITY_E ePortAbility);
HI_S32 HI_MPI_VDEC_EnablePort(HI_HANDLE hVdec,HI_HANDLE hPort);
HI_S32 HI_MPI_VDEC_DisablePort(HI_HANDLE hVdec,HI_HANDLE hPort);
HI_S32 HI_MPI_VDEC_SetPortType(HI_HANDLE hVdec, HI_HANDLE hPort, VDEC_PORT_TYPE_E enPortType);

HI_S32 HI_MPI_VDEC_GetPortAttr(HI_HANDLE hVdec, HI_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstPortCfg);

HI_S32 HI_MPI_VDEC_SetPortAttr(HI_HANDLE hVdec, HI_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstPortCfg);

HI_S32 HI_MPI_VDEC_CancleMainPort(HI_HANDLE hVdec,HI_HANDLE hPort);
HI_S32 HI_MPI_VDEC_DestroyPort(HI_HANDLE hVdec,HI_HANDLE hPort);
HI_S32 HI_MPI_VDEC_ReceiveFrame(HI_HANDLE hVdec, HI_DRV_VIDEO_FRAME_PACKAGE_S *pFrmPack, HI_U32 TimeOut);
HI_S32 HI_MPI_VDEC_GetChanFrmStatusInfo(HI_HANDLE hVdec, HI_HANDLE  hPort,VDEC_FRMSTATUSINFO_S *pstVdecFrmStatus);
HI_S32 HI_MPI_VDEC_SetChanFrmPackType(HI_HANDLE hVdec, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E *pFrmPackingType);
HI_S32 HI_MPI_VDEC_GetChanFrmPackType(HI_HANDLE hVdec, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E *pFrmPackingType);
HI_S32 HI_MPI_VDEC_AcqUserData(HI_HANDLE hVdec,
                HI_UNF_VIDEO_USERDATA_S* pstUserData, HI_UNF_VIDEO_USERDATA_TYPE_E* penType);
HI_S32 HI_MPI_VDEC_RlsUserData(HI_HANDLE hVdec, HI_UNF_VIDEO_USERDATA_S* pstUserData);
HI_S32 HI_MPI_VDEC_Invoke(HI_HANDLE hVdec, HI_CODEC_VIDEO_CMD_S* pstParam);
HI_S32 HI_MPI_VDEC_GetChanOpenParam(HI_HANDLE hVdec, HI_UNF_AVPLAY_OPEN_OPT_S *pstOpenPara);

HI_S32 HI_MPI_VDEC_ReleaseFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pVideoFrame);
HI_S32 HI_MPI_VDEC_SetLowDelay(HI_HANDLE hVdec, HI_UNF_AVPLAY_LOW_DELAY_ATTR_S *pstAttr);
HI_S32 HI_MPI_VDEC_ChanDropStream(HI_HANDLE hVdec, HI_U32 *pSeekPts, HI_U32 u32Gap);
HI_S32 HI_MPI_VDEC_SetExternBuffer(HI_HANDLE hVdec, VDEC_BUFFER_ATTR_S* pstBufAttr);
HI_S32 HI_MPI_VDEC_SetChanBufferMode(HI_HANDLE hVdec,VDEC_FRAMEBUFFER_MODE_E enFrameBufferType);
HI_S32 HI_MPI_VDEC_CheckAndDeleteExtBuffer(HI_HANDLE hVdec,HI_U32 u32PhyAddr,VDEC_FRAMEBUFFER_STATE_E* penBufState);
HI_S32 HI_MPI_VDEC_SetExternBufferState(HI_HANDLE hVdec, VDEC_EXTBUFFER_STATE_E enExtBufferState);
HI_S32 HI_MPI_VDEC_SetResolution(HI_HANDLE hVdec,VDEC_RESOLUTION_ATTR_S* pstResolution);
HI_S32 HI_MPI_VDEC_GetPreCap(VDEC_PRE_CAP_E* peCapType);
HI_S32 HI_MPI_VDEC_GetLeftStreamFrm(HI_HANDLE hVdec, HI_U32* pLeftStreamFrm);
#ifdef HI_TEE_SUPPORT
HI_S32 HI_MPI_VDEC_SetTVP(HI_HANDLE hVdec, HI_UNF_AVPLAY_TVP_ATTR_S *pstAttr);
#endif

#ifdef HI_HDR_SUPPORT
HI_S32 HI_MPI_VDEC_SetHDRAttr(HI_HANDLE hVdec, VDEC_HDR_ATTR_S *VdecHDRAttr);
#endif

HI_S32 HI_MPI_VDEC_GlobalRelFrm(HI_DRV_VIDEO_FRAME_S *pVideoFrame);
HI_S32 HI_MPI_VDEC_GetVideoBypass(HI_HANDLE hVdec,HI_BOOL* pbVideoBypass);

HI_S32 HI_MPI_VDEC_SetChnTunnelMode(HI_HANDLE hVdec, HI_U32 Enable);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif//__HI_MPI_VDEC_H__

