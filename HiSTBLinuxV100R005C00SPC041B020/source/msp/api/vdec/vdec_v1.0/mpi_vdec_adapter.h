/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_vdec_adapter.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/07/31
  Description   : Definitions for vdec driver
  History       :
  1.Date        : 2012/07/31
    Author      : l00185424
    Modification: Created file

*******************************************************************************/

#ifndef __HI_MPI_VDEC_ADAPTER_H__
#define __HI_MPI_VDEC_ADAPTER_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/******************************* Include Files *******************************/

#include "hi_type.h"
#include "hi_unf_avplay.h"
#include "hi_video_codec.h"
#include "hi_mpi_vdec.h"
#include "hi_drv_vdec.h"

/****************************** Macro Definition *****************************/
//#define VPU_MEM_MAP

/*************************** Structure Definition ****************************/

/* VFMW private attribute */
typedef struct 
{
    HI_UNF_AVPLAY_OPEN_OPT_S        stOpenOpt;
}VFMW_PRIV_ATTR_S;

/* VFMW IFrame private parameter */
typedef struct 
{
    HI_UNF_AVPLAY_I_FRAME_S*        pstIFrameStream;
    HI_BOOL                         bCapture;
}VFMW_IFRAME_PARAM_S;

/* VFMW stream buffer */
typedef struct 
{
    HI_HANDLE                       hStrmBuf;       /* ES buffer handle, if uses ES buffer, must set this param */
    HI_HANDLE                       hDmxVidChn;     /* Demux handle, if play TS stream from demux, must set this param */
    HI_U32                          u32BufSize;
}VFMW_STREAMBUF_S;

/* VFMW User data */
typedef struct 
{
    HI_UNF_VIDEO_USERDATA_TYPE_E*   penType;        /* User data type */
    HI_UNF_VIDEO_USERDATA_S*        pstData;        /* User data info */
}VFMW_USERDATA_S;

/*VFMW SeekPts parameter*/
typedef struct
{
    HI_U32*                        pu32SeekPts;
	HI_U32                         u32Gap;
}VFMW_SEEKPTS_PARAM_S;

/* VFMW extended command */
typedef enum 
{
    VFMW_CMD_CHECKEVT = 0,      /**< The param is VDEC_EVENT_S* */
    VFMW_CMD_READNEWFRAME,      /**< The param is HI_UNF_VIDEO_FRAME_INFO_S* */
    VFMW_CMD_READUSRDATA,       /**< The param is HI_UNF_VIDEO_USERDATA_S* */
    VFMW_CMD_SETFRAMERATE,      /**< The param is HI_UNF_AVPLAY_FRMRATE_PARAM_S* */
    VFMW_CMD_GETFRAMERATE,      /**< The param is HI_UNF_AVPLAY_FRMRATE_PARAM_S* */
    VFMW_CMD_GETSTATUSINFO,     /**< The param is VDEC_STATUSINFO_S* */
    VFMW_CMD_SETEOSFLAG,        /**< No parameter */
    VFMW_CMD_IFRAMEDECODE,      /**< The param is VFMW_IFRAME_PARAM_S* */
    VFMW_CMD_IFRAMERELEASE,     /**< The param is HI_UNF_VIDEO_FRAME_INFO_S* */
    VFMW_CMD_RECEIVEFRAME,      /**< The param is HI_UNF_VIDEO_FRAME_INFO_S* */
    VFMW_CMD_RELEASEFRAME,      /**< The param is HI_UNF_VIDEO_FRAME_INFO_S* */
    VFMW_CMD_ATTACHBUF,         /**< The param is VFMW_STREAMBUF_S* */
    VFMW_CMD_DETACHBUF,         /**< No parameter */
    VFMW_CMD_DISCARDFRAME,      /**< The param is VDEC_DISCARD_FRAME_S* */
    VFMW_CMD_SETLOWDELAY,      /**< The param is HI_UNF_AVPLAY_LOW_DELAY_ATTR_S* */
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    VFMW_CMD_ACQUSERDATA,       /**< The param is VFMW_USERDATA_S* */
    VFMW_CMD_RLSUSERDATA,       /**< The param is HI_UNF_VIDEO_USERDATA_S* */
#endif
    VFMW_CMD_DROPSTREAM,
    /* Commond can be invoked by AVPLAY directly */
    VFMW_CMD_GETINFO = 0x20,    /* Get vdec info, the param is HI_UNF_AVPLAY_VDEC_INFO_S* */
    VFMW_CMD_SETTPLAYOPT = 0x21,/* Set T play, the param is HI_UNF_AVPLAY_TPLAY_OPT_S* */
    VFMW_CMD_SETCTRLINFO = 0x22,/* Set control info, the param is HI_UNF_AVPLAY_CONTROL_INFO_S* */
    /**Defines commond to set video sample type, HI_BOOL *, HI_TRUE: Progressive, HI_FALSE: Interlance */
    /**CNcomment: 设置视频逐行信息, HI_TRUE: 逐行, HI_FALSE: 隔行*/
    VFMW_CMD_SET_PROGRESSIVE = 0x23,
    VFMW_CMD_SET_COLORSPACE =  0x24,

    VFMW_CMD_SET_DPBFULL_CTRL = 0x25,
    
    VFMW_CMD_BUTT
}VFMW_CMD_E;

//add by l00225186
typedef enum
{
    VPSS_CMD_RECEIVEFRAME = 0,
    VPSS_CMD_CREATEVPSS,
    VPSS_CMD_DESTORYVPSS,
	VPSS_CMD_CREATEPORT,
	VPSS_CMD_DESTORYPORT,
	VPSS_CMD_ENABLEPORT,
	VPSS_CMD_DISABLEPORT,
	VPSS_CMD_RESETVPSS,
	VPSS_CMD_GETSTATUSINFO,
	VPSS_CMD_SETPORTTYPE,
	VPSS_CMD_CANCLEMAINPORT,
	VPSS_CMD_SETCHAN_FRMPACKTYPE,
	VPSS_CMD_GETCHAN_FRMPACKTYPE,
	VPSS_CMD_SENDEOS,
	VPSS_CMD_GETPORTSTATE,
	VPSS_CMD_GETPORTATTR,
	VPSS_CMD_SETPORTATTR,
	VPSS_CMD_SETEXTBUFFER,
	VPSS_CMD_SETBUFFERMODE,
	VPSS_CMD_CHECKANDDELBUFFER,
	VPSS_CMD_SETEXTBUFFERSTATE,
	VPSS_CMD_SETRESOLUTION
		
}VPSS_CMD_E;
/******************************* API declaration *****************************/

HI_CODEC_ID_E VDEC_UNF2CodecId(HI_UNF_VCODEC_TYPE_E enType);
HI_UNF_VCODEC_TYPE_E VDEC_CodecId2UNF(HI_CODEC_ID_E enCodecId);
HI_UNF_ENC_FMT_E VDEC_DisplayFmt2UNF(HI_CODEC_ENC_FMT_E enDisplayNorm);
HI_CODEC_ENC_FMT_E VDEC_UNFDisplayFmt2CODEC(HI_UNF_ENC_FMT_E enDisplayNorm);

HI_S32 VDEC_OpenDevFile(HI_VOID);
HI_S32 VDEC_CloseDevFile(HI_VOID);
HI_CODEC_S* VDEC_VFMW_Codec(HI_VOID);

HI_S32 VPSS_Control(HI_HANDLE handle, HI_U32 u32CMD, HI_VOID * pParam);
	
HI_S32 VDEC_AllocHandle(HI_HANDLE *phHandle);
HI_S32 VDEC_FreeHandle(HI_HANDLE hHandle);

#ifndef HI_TEE_SUPPORT
HI_S32 VDEC_CreateStreamBuf(HI_HANDLE* phBuf, HI_U32 u32BufSize);
#else
HI_S32 VDEC_CreateStreamBuf(HI_HANDLE* phBuf, VDEC_BUFFER_ATTR_S *pstBufAttr);
HI_S32 VDEC_SetTVP(HI_HANDLE hInst, HI_UNF_AVPLAY_TVP_ATTR_S* pParam);
#endif
HI_S32 VDEC_DestroyStreamBuf(HI_HANDLE hBuf);
HI_S32 VDEC_GetStreamBuf(HI_HANDLE hBuf, HI_U32 u32RequestSize, VDEC_ES_BUF_S *pstBuf);
HI_S32 VDEC_PutStreamBuf(HI_HANDLE hBuf, const VDEC_ES_BUF_S *pstBuf);
HI_S32 VDEC_ResetStreamBuf(HI_HANDLE hBuf);
HI_S32 VDEC_GetStreamBufStatus(HI_HANDLE hBuf, HI_DRV_VDEC_STREAMBUF_STATUS_S* pstStatus);
HI_S32 VPSS_ReleaseFrm(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pVideoFrame);
HI_S32 VDEC_SetLowDelay(HI_HANDLE hInst, HI_UNF_AVPLAY_LOW_DELAY_ATTR_S* pParam);
HI_S32 VFMW_SetProgressive(HI_HANDLE hInst, HI_BOOL* pParam);
#if (HI_VDEC_REG_CODEC_SUPPORT == 1) || (HI_VDEC_MJPEG_SUPPORT == 1)
HI_S32 VDEC_RecvStream(HI_HANDLE hBuf, VDEC_ES_BUF_S *pstBuf);
HI_S32 VDEC_RlsStream(HI_HANDLE hBuf, const VDEC_ES_BUF_S *pstBuf);
HI_S32 VDEC_CreateFrameBuf(HI_HANDLE *phBuf);
HI_S32 VDEC_DestroyFrameBuf(HI_HANDLE hBuf);
HI_S32 VDEC_GetFrameBuf(HI_HANDLE hBuf, HI_DRV_VDEC_FRAME_BUF_S* pstBuf);
HI_S32 VDEC_PutFrameBuf(HI_HANDLE hBuf, const HI_DRV_VDEC_USR_FRAME_S* pstBuf);
HI_S32 VDEC_RecvFrameBuf(HI_HANDLE hBuf, HI_DRV_VDEC_USR_FRAME_S* pstBuf);
HI_S32 VDEC_RlsFrameBuf(HI_HANDLE hBuf, const HI_DRV_VDEC_USR_FRAME_S* pstBuf);
HI_S32 VDEC_ResetFrameBuf(HI_HANDLE hBuf);
HI_S32 VDEC_GetFrameBufStatus(HI_HANDLE hBuf, HI_DRV_VDEC_FRAMEBUF_STATUS_S* pstStatus);
HI_S32 VDEC_GetNewFrm(HI_HANDLE hBuf, HI_DRV_VIDEO_FRAME_S* pstFrm);
HI_S32 VDEC_SetFrmRate(HI_HANDLE hBuf, const HI_UNF_AVPLAY_FRMRATE_PARAM_S* pstFrmRate);
HI_S32 VDEC_GetFrmRate(HI_HANDLE hBuf, HI_UNF_AVPLAY_FRMRATE_PARAM_S* pstFrmRate);
#endif

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_MPI_VDEC_ADAPTER_H__ */

