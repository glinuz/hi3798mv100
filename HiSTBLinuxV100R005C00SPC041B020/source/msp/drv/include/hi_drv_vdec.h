/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_vdec.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/05/17
  Description   :
  History       :
  1.Date        : 2006/05/17
    Author      : g45345
    Modification: Created file

******************************************************************************/
#ifndef __HI_DRV_VDEC_H__
#define __HI_DRV_VDEC_H__

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_mpi_vdec.h"
#include "hi_debug.h"
//add by l00225186
#include "hi_drv_video.h"
#include "drv_omxvdec_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_MALLOC_VDEC(size)    HI_MALLOC(HI_ID_VDEC, size)
#define HI_FREE_VDEC(addr)      HI_FREE(HI_ID_VDEC, addr)

#define VDEC_LOCK(Mutex)        (HI_VOID)pthread_mutex_lock(&Mutex)
#define VDEC_UNLOCK(Mutex)      (HI_VOID)pthread_mutex_unlock(&Mutex)

#define HI_FATAL_VDEC(fmt...)   HI_FATAL_PRINT(HI_ID_VDEC, fmt)
#define HI_ERR_VDEC(fmt...)     HI_ERR_PRINT(HI_ID_VDEC, fmt)
#define HI_WARN_VDEC(fmt...)    HI_WARN_PRINT(HI_ID_VDEC, fmt)
#define HI_INFO_VDEC(fmt...)    HI_INFO_PRINT(HI_ID_VDEC, fmt)

//#ifndef HI_VDEC_MAX_INSTANCE_NEW
#define HI_VDEC_MAX_INSTANCE_NEW (16)
//#endif

/** max user data length*/
#define MAX_USER_DATA_LEN       256UL

typedef enum tagVDEC_RESET_TYPE_E
{
    VDEC_RESET_TYPE_ALL = 0,
    VDEC_RESET_TYPE_IDLE,
    VDEC_RESET_TYPE_INVALID
}HI_DRV_VDEC_RESET_TYPE_E;

typedef struct
{
    HI_U32 u32StartVirAddr;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32Size;
} VDEC_MMZ_BUFFER_S;

typedef struct hiVDEC_BTL_S
{
    HI_DRV_VIDEO_FRAME_S *pstInFrame;
    HI_DRV_VIDEO_FRAME_S *pstOutFrame;
    HI_U32 u32PhyAddr;
    HI_U32 u32Size;
    HI_U32 u32TimeOutMs;
}HI_DRV_VDEC_BTL_S;

typedef struct hiVDEC_USR_FRAME_S
{
    HI_BOOL             bFrameValid;
    HI_BOOL             bEndOfStream;
    HI_UNF_VIDEO_FORMAT_E enFormat;           /* Color format */
    HI_U32              u32Pts;
    HI_S32              s32YWidth;
    HI_S32              s32YHeight;
    HI_S32              s32LineNumPhyAddr;
    HI_U64              u64LineNumVirAddr;
    HI_S32              s32LumaPhyAddr;
    HI_S32              s32LumaStride;
    HI_S32              s32CbPhyAddr;    /* ChromePhyAddr if bSemiPlanar is true;CbPhyAddr if bSemiPlanar is false */
    HI_S32              s32CrPhyAddr;    /* invalid if bSemiPlanar is true; CrPhyAddr if bSemiPlanar is false */
    HI_S32              s32ChromStride;
    HI_S32              s32ChromCrStride;
	HI_S32              s32FrameID;
}HI_DRV_VDEC_USR_FRAME_S;

typedef struct hiVDEC_FRAME_BUF_S
{
    HI_U32 u32PhyAddr;
    HI_U32 u32LineNumPhyAddr;
    HI_U64 u64LineNumVirAddr;
    HI_U32 u32Size;
}HI_DRV_VDEC_FRAME_BUF_S;

typedef struct hiVDEC_STREAM_BUF_S
{
    HI_U32              u32Size;    /* Buffer size[in] */
    HI_HANDLE           hHandle;    /* Stream buffer handle [out] */
    HI_U32              u32PhyAddr; /* Buffer phy address [out] */
	#ifdef HI_TEE_SUPPORT
    HI_BOOL             bTvp;      /*Trust Video Path Flag[in]*/
	#endif
    HI_HANDLE           hVdec;      /* Vdec handle [in] */
}HI_DRV_VDEC_STREAM_BUF_S;

/** Stream buffer status */
typedef struct
{
    HI_U32  u32Size;            /**< Total buffer size, in the unit of byte.*/
    HI_U32  u32Available;       /**< Available buffer, in the unit of byte.*/
    HI_U32  u32Used;            /**< Used buffer, in the unit of byte.*/
    HI_U32  u32DataNum;         /**< For stream mode, it is undecoded packet number.
                                     For frame mode, it is undecoded frame number, support BUFMNG_NOT_END_FRAME_BIT.*/
}HI_DRV_VDEC_STREAMBUF_STATUS_S;

/** Frame buffer status */
typedef struct
{
    HI_U32  u32TotalDecFrameNum;
    HI_U32  u32FrameBufNum;     /**< Frame num in buffer to display */
    HI_BOOL bAllPortCompleteFrm;
}HI_DRV_VDEC_FRAMEBUF_STATUS_S;

typedef struct hiVDEC_USERDATABUF_S
{
    HI_U32  u32Size;            /* Buffer size[in] */
    HI_U32  u32PhyAddr;         /* Buffer phy address [out] */
}HI_DRV_VDEC_USERDATABUF_S;

typedef enum
{
    VIDDEC_EVT_SPEED_MISMATCH,         /*!< decoder speed does not match requested speed                  */
    VIDDEC_EVT_BUFFER_ALMOST_FULL,     /*!< decoder buffer is almost full (pacing for inject operations)  */
    VIDDEC_EVT_BUFFER_ALMOST_EMPTY,    /*!< decoder buffer is almost empty (pacing for inject operations) */
    VIDDEC_EVT_UNSUPPORTED_STREAM_TYPE,/*!< stream type is unsupported                                    */
    VIDDEC_EVT_1ST_IFRAME_DECODED,     /*!< the first IFRAME has been decoded                             */
    VIDDEC_EVT_STREAM_INFO_CHANGE,     /*!< Change in the stream characteristics                          */
    VIDDEC_EVT_CC_DATA,                /*!< Closed caption data event                                     */
    VIDDEC_EVT_DATA_ERROR,             /*!< stream data error                                             */
    VIDDEC_EVT_ENDDEF                  /* enum terminator */
}VIDDEC_EVNT_TYPE_E;

/*l00284814 64bit ioctl使用的结构体不能包含指针*/
/**Defines the structure of the user data in the video information.*/
/**CNcomment: 定义视频信息中的用户数据结构*/
typedef struct hiVDEC_VIDEO_USERDATA_S
{
    HI_UNF_VIDEO_BROADCAST_PROFILE_E   enBroadcastProfile;   /**<Broadcasting profile of the user data*/ /**<CNcomment: 用户数据的广播profile*/
    HI_UNF_VIDEO_USER_DATA_POSITION_E  enPositionInStream;   /**<Position of the user data in video streams*/ /**<CNcomment: 用户数据在视频流中的位置*/
    HI_U32                             u32Pts;               /**<PTS corresponding to the user data*/ /**<CNcomment: 用户数据对应的时间戳*/
    HI_U32                             u32SeqCnt;            /**<Sequence ID of the user data*/ /**<CNcomment: 用户数据的前一序列数*/
    HI_U32                             u32SeqFrameCnt;       /**<Frame number of the user data*/ /**<CNcomment: 用户数据的前一帧数*/
    HI_U64                             pu8Buffer;           /**<Initial address of the user data memory, output parameter*/ /**<CNcomment: 用户数据内存区的初始地址,输出参数*/
    HI_U32                             u32Length;            /**<User data size (a multiple of 1 byte)*/ /**<CNcomment: 用户数据的大小，以1byte为单位*/
    HI_BOOL                            bBufferOverflow;      /**<Indicate that whether the user data size exceeds the maximum size defined by MAX_USER_DATA_LEN.*/ /**<CNcomment: 标志用户数据的长度是否超过了MAX_USER_DATA_LEN定义的最大值*/
    HI_BOOL                            bTopFieldFirst;       /**<Top field first flag*/ /**<CNcomment: 顶场优先标志*/
}VDEC_VIDEO_USERDATA_S;



typedef  HI_S32 ( *EventCallBack)(HI_HANDLE vHandle,HI_VOID *pEvent, VIDDEC_EVNT_TYPE_E eType);
typedef  HI_S32 ( *GetDmxHdlCallBack)(HI_HANDLE dmxID,HI_U32 chanType, HI_HANDLE *pDmxHdl);

// for 64bit @f00241306
/**Defines the attributes of a video decoder.*/
/**CNcomment: 定义视频解码器属性结构*/
typedef struct tagVDEC_VCODEC_ATTR_S
{
    HI_UNF_VCODEC_TYPE_E        enType;          /**<Video encoding type*/ /**<CNcomment: 视频编码类型*/
    HI_UNF_VCODEC_EXTATTR_U     unExtAttr;       /**<Extra attributes related to the video encoding type*/ /**<CNcomment: 视频编码类型相关的额外属性*/
    HI_UNF_VCODEC_MODE_E        enMode;          /**<Mode of a video decoder*/ /**<CNcomment: 视频解码器模式模式*/
    HI_U32                      u32ErrCover;     /**<Error concealment threshold of the output frames of a video decoder. The value 0 indicates that no frames are output if an error occurs; the value 100 indicates that all frames are output no matter whether errors occur.*/
    /**<CNcomment: 视频解码器的输出帧错误隐藏门限，0:出现错误即不输出；100:不管错误比例全部输出*/
    HI_U32                      u32Priority;     /**<Priority of a video decoder. The value range is [1, HI_UNF_VCODEC_MAX_PRIORITY]. The value 0 is a reserved value. If you set the value to 0, no error message is displayed, but the value 1 is used automatically. The smaller the value, the lower the priority.*/
    /**<CNcomment: 视频解码器优先级, 取值范围: 大于等于1，小于等于HI_UNF_VCODEC_MAX_PRIORITY, 0是保留值，配置为0不会报错，但是会自动取值为1,数值越小优先级越低 */
    HI_BOOL                     bOrderOutput;    /**<Whether the videos are output by the decoding sequence. You are advised to set this parameter to HI_TRUE in VP mode, and HI_FALSE in other modes.*/
    /**<CNcomment: 是否按解码序输出，VP模式下推荐配置为HI_TRUE, 一般模式下配置为HI_FALSE */    
    HI_S32                      s32CtrlOptions;  /**<The value is HI_UNF_VCODEC_CTRL_OPTION_E, or the 'or' value of several enum type*/
    /**<CNcomment: 取值为HI_UNF_VCODEC_CTRL_OPTION_E，或者几个枚举的'或'  */
    HI_U64                   CodecContext;   /**<Private codec context */
}VDEC_VCODEC_ATTR_S;


// for 64bit @f00241306

/**Defines the information about an I frame.*//**CNcomment:定义I帧数据信息类型 */
typedef struct hiHI_DRV_VDEC_I_FRAME_64BIT_S
{
    HI_U64     Addr;             /* *<User-state virtual address of a frame*/ /**<CNcomment:帧数据用户态虚拟地址 */
    HI_U32     u32BufSize;       /* *<Frame size, in byte*/                   /**<CNcomment:帧数据大小，单位字节 */
    HI_UNF_VCODEC_TYPE_E enType; /* Protocol type of a data segment*/         /**<CNcomment:该片数据的协议类型 */
} HI_DRV_VDEC_I_FRAME_64BIT_S;

HI_S32 HI_DRV_VDEC_Init(HI_VOID);
HI_VOID HI_DRV_VDEC_DeInit(HI_VOID);
HI_S32 HI_DRV_VDEC_Open(HI_VOID);
HI_S32 HI_DRV_VDEC_Close(HI_VOID);
HI_S32 HI_DRV_VDEC_AllocChan(HI_HANDLE *phHandle, HI_UNF_AVPLAY_OPEN_OPT_S *pstCapParam);
HI_S32 HI_DRV_VDEC_FreeChan(HI_HANDLE hHandle);
HI_S32 HI_DRV_VDEC_SetChanAttr(HI_HANDLE hHandle, HI_UNF_VCODEC_ATTR_S *pstCfgParam);
#ifdef HI_HDR_SUPPORT
HI_S32 HI_DRV_VDEC_SetChanHDRAttr(HI_HANDLE hHandle, VDEC_HDR_ATTR_S *vdec_hdr_attr);
#endif
HI_S32 HI_DRV_VDEC_GetChanAttr(HI_HANDLE hHandle, HI_UNF_VCODEC_ATTR_S *pstCfgParam);
HI_S32 HI_DRV_VDEC_ChanBufferInit(HI_HANDLE hHandle, HI_U32 u32BufSize, HI_HANDLE hDmxVidChn);
HI_S32 HI_DRV_VDEC_ChanBufferDeInit(HI_HANDLE hHandle);
HI_S32 HI_DRV_VDEC_ResetChan(HI_HANDLE hHandle);
HI_S32 HI_DRV_VDEC_ChanStart(HI_HANDLE hHandle);
HI_S32 HI_DRV_VDEC_ChanStop(HI_HANDLE hHandle);
HI_S32 HI_DRV_VDEC_GetChanStatusInfo(HI_HANDLE hHandle, VDEC_STATUSINFO_S* pstStatus);
HI_S32 HI_DRV_VDEC_GetChanStreamInfo(HI_HANDLE hHandle, HI_UNF_VCODEC_STREAMINFO_S *pstStreamInfo);
HI_S32 HI_DRV_VDEC_CheckNewEvent(HI_HANDLE hHandle, VDEC_EVENT_S *pstEvent);
HI_S32 HI_DRV_VDEC_GetUsrData(HI_HANDLE hHandle, HI_UNF_VIDEO_USERDATA_S *pstUsrData);
HI_S32 HI_DRV_VDEC_SetTrickMode(HI_HANDLE hHandle, HI_UNF_AVPLAY_TPLAY_OPT_S* pstOpt);
HI_S32 HI_DRV_VDEC_SetCtrlInfo(HI_HANDLE hHandle, HI_UNF_AVPLAY_CONTROL_INFO_S* pstCtrlInfo);
HI_S32 HI_DRV_VDEC_DecodeIFrame(HI_HANDLE hHandle, HI_UNF_AVPLAY_I_FRAME_S *pstStreamInfo,HI_BOOL bCapture, HI_BOOL bUserSpace);

HI_S32 HI_DRV_VDEC_ReleaseIFrame(HI_HANDLE hHandle, HI_DRV_VIDEO_FRAME_S *pstFrameInfo);

HI_S32 HI_DRV_VDEC_SetEosFlag(HI_HANDLE hHandle);
HI_S32 HI_DRV_VDEC_DiscardFrm(HI_HANDLE hHandle, VDEC_DISCARD_FRAME_S* pstParam);
HI_S32 HI_DRV_VDEC_GetFrmBuf(HI_HANDLE hHandle, HI_DRV_VDEC_FRAME_BUF_S* pstFrm);
HI_S32 HI_DRV_VDEC_PutFrmBuf(HI_HANDLE hHandle, HI_DRV_VDEC_USR_FRAME_S* pstFrm);
HI_S32 HI_DRV_VDEC_CreateStrmBuf(HI_HANDLE hVdec, HI_DRV_VDEC_STREAM_BUF_S *pstBuf);
HI_S32 HI_DRV_VDEC_RecvFrmBuf(HI_HANDLE s32Handle, HI_DRV_VIDEO_FRAME_S * pstFrameInfo);
HI_S32 HI_DRV_VDEC_RlsFrmBuf(HI_HANDLE s32Handle, HI_DRV_VIDEO_FRAME_S * pstFrameInfo);
HI_S32 HI_DRV_VDEC_RlsFrmBufWithoutHandle(HI_DRV_VIDEO_FRAME_S * pstFrameInfo);
HI_S32 HI_DRV_VDEC_BlockToLine(HI_S32 s32Handle, HI_DRV_VDEC_BTL_S * pstBTLInfo);
HI_S32 HI_DRV_VDEC_GetEsBuf(HI_HANDLE s32Handle,  VDEC_ES_BUF_S * pstBuf);
HI_S32 HI_DRV_VDEC_PutEsBuf(HI_HANDLE s32Handle,  VDEC_ES_BUF_S * pstBuf);
HI_S32 HI_DRV_VDEC_SetPortType(HI_HANDLE hVpss,HI_HANDLE hPort, VDEC_PORT_TYPE_E enPortType );
HI_S32 HI_DRV_VDEC_EnablePort(HI_HANDLE hVdec,HI_HANDLE hPort);
HI_S32 HI_DRV_VDEC_CreatePort(HI_HANDLE hVdec,HI_HANDLE* phPort, VDEC_PORT_ABILITY_E ePortAbility);
HI_S32 HI_DRV_VDEC_DestroyPort(HI_HANDLE hVdec,HI_HANDLE hPort);
HI_S32 HI_DRV_VDEC_GetPortParam(HI_HANDLE hVdec,HI_HANDLE hPort,VDEC_PORT_PARAM_S* pstPortParam);
HI_S32 HI_DRV_VDEC_SetChanFrmRate(HI_HANDLE hHandle, HI_UNF_AVPLAY_FRMRATE_PARAM_S *pstFrmRate);
HI_S32 HI_DRV_VDEC_GetChanFrmRate(HI_HANDLE hHandle, HI_UNF_AVPLAY_FRMRATE_PARAM_S *pstFrmRate);
HI_S32 HI_DRV_VDEC_Chan_RecvVpssFrmBuf(HI_HANDLE hVdec, HI_DRV_VIDEO_FRAME_PACKAGE_S* pstFrm);
HI_S32 HI_DRV_VDEC_SetProgressive(HI_HANDLE hHandle, HI_BOOL pProgressive);
HI_S32 HI_DRV_VDEC_SetFrmPackingType(HI_HANDLE hVdec,HI_UNF_VIDEO_FRAME_PACKING_TYPE_E eFramePackType);
HI_S32 HI_DRV_VDEC_GetFrmPackingType(HI_HANDLE hVdec,HI_UNF_VIDEO_FRAME_PACKING_TYPE_E *penFramePackType);
HI_S32 HI_DRV_VDEC_GetVideoFrameInfo(HI_HANDLE hVdec, HI_UNF_AVPLAY_VIDEO_FRAME_INFO_S *pstVideoFrameInfo);
HI_VOID HI_DRV_VDEC_GetVcmpFlag(HI_BOOL *pbVcmpFlag);
HI_S32 HI_DRV_VDEC_GetPreAllocVideoBuffer(VDEC_MMZ_BUFFER_S *pstMMZBuf);
HI_S32 HI_VDEC_Chan_VOAcqFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);
HI_S32 HI_VDEC_Chan_VORlsFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);
HI_S32 HI_VDEC_Chan_VOChangeWinInfo(HI_HANDLE hPort,HI_DRV_WIN_PRIV_INFO_S* pstWinInfo);
HI_VOID HI_DRV_VDEC_SetOmxCallBacks(OMX_EXPORT_FUNC_S* OmxCallbacks);
HI_VOID HI_DRV_VDEC_ReportEsRels(HI_HANDLE hHandle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __HI_DRV_VDEC_H__ */
