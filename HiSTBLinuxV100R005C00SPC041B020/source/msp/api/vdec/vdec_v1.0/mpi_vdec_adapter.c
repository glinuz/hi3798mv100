/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_vdec_adapter.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/07/31
  Description   : Implement for vdec driver
  History       :
  1.Date        : 2012/07/31
    Author      : l00185424
    Modification: Created file

*******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <pthread.h>

/* Unf headers */
#include "hi_video_codec.h"

/* Mpi headers */
#include "hi_error_mpi.h"
#include "hi_mpi_mem.h"
#include "mpi_vdec_adapter.h"

/* Drv headers */
#include "hi_drv_struct.h"
#include "vfmw.h"
#include "hi_drv_vdec.h"
#include "hi_drv_video.h"
#include "list.h"
#include "drv_vdec_ioctl.h"
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

/****************************** Macro Definition *****************************/

#define VFMW_INST_HANDLE(hInst) (hInst & 0xFF)

#define VFMW_FIND_INST(hInst, pVFMWInst) \
    { \
        struct list_head* pos; \
        struct list_head* n; \
        VFMW_INST_S* pstTmp; \
        if (!list_empty(&s_stVdecAdpParam.stInstHead)) \
        { \
            list_for_each_safe(pos, n, &s_stVdecAdpParam.stInstHead) \
            { \
                pstTmp = list_entry(pos, VFMW_INST_S, stInstNode); \
                if (VFMW_INST_HANDLE(hInst) == pstTmp->hInst) \
                { \
                    pVFMWInst = pstTmp; \
                    break; \
                } \
            } \
        } \
    }

/* Find an instance pointer from list by handle */
#define STRMBUF_FIND_INST(hBuf, pBufInst) \
    { \
        struct list_head* pos; \
        struct list_head* n; \
        STREAM_BUF_INST_S* pstTmp; \
        VDEC_LOCK(s_stStrmBufParam.stMutex); \
        if (!list_empty(&s_stStrmBufParam.stBufHead)) \
        { \
            list_for_each_safe(pos, n, &s_stStrmBufParam.stBufHead) \
            { \
                pstTmp = list_entry(pos, STREAM_BUF_INST_S, stBufNode); \
                if (hBuf == pstTmp->hBuf) \
                { \
                    pBufInst = pstTmp; \
                    break; \
                } \
            } \
        } \
        VDEC_UNLOCK(s_stStrmBufParam.stMutex); \
    }

/* Find an instance pointer from list by handle */
#define FRMBUF_FIND_INST(hBuf, pBufInst) \
    { \
        struct list_head* pos; \
        struct list_head* n; \
        FRAME_BUF_INST_S* pstTmp; \
        VDEC_LOCK(s_stFrmBufParam.stMutex); \
        if (!list_empty(&s_stFrmBufParam.stBufHead)) \
        { \
            list_for_each_safe(pos, n, &s_stFrmBufParam.stBufHead) \
            { \
                pstTmp = list_entry(pos, FRAME_BUF_INST_S, stBufNode); \
                if (hBuf == pstTmp->hBuf) \
                { \
                    pBufInst = pstTmp; \
                    break; \
                } \
            } \
        } \
        VDEC_UNLOCK(s_stFrmBufParam.stMutex); \
    }

/************************ Static Structure Definition ************************/

/* Describe a VFMW instance */
typedef struct tagVFMW_INST_S
{
    HI_HANDLE        hInst;                         /* Instance handle */
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    HI_U8*           pu8UserDataVirAddr;            /* User data MMZ user space address  */
#endif
    HI_U8            au8UsrData[MAX_USER_DATA_LEN]; /* Userdata buffer */
    struct list_head stInstNode;                    /* Instance list node */
} VFMW_INST_S;

/* Global parameters for VFMW */
typedef struct tagVFMW_GLOBAL_S
{
    const HI_CHAR*      pszDevPath;     /* Device name */
    HI_S32              s32DevFd;       /* Device file handle */
    HI_U32              u32InitCount;   /* Init counter */
    HI_CODEC_CAP_S      stCap;          /* VFMW capability */
    pthread_mutex_t     stMutex;        /* Mutex */
    struct list_head    stInstHead;     /* Instance list head */
} VDEC_ADP_GLOBAL_S;

/* Describe a stream buffer instance */
typedef struct tagSTREAM_BUF_INST_S
{
    HI_HANDLE     hBuf;                 /* BM buffer handle */
#ifdef HI_TEE_SUPPORT
    HI_BOOL       bTvp;                 /* Secure mem flag */
#endif
    HI_U32        u32Size;              /* Size */
    HI_U8*        pu8MMZVirAddr;        /* MMZ virtual address */
    HI_BOOL       bGetPutFlag;          /* Gut/Put flag */
    VDEC_ES_BUF_S stLastGet;            /* Last get buffer */
#if (HI_VDEC_REG_CODEC_SUPPORT == 1) || (HI_VDEC_MJPEG_SUPPORT == 1)
    HI_BOOL       bRecvRlsFlag;         /* Receive/Release flag */
    VDEC_ES_BUF_S stLastRecv;           /* Last Receive buffer */
#endif
    struct list_head stBufNode;         /* Instance list node */
} STREAM_BUF_INST_S;

/* Global parameters for stream buffer */
typedef struct tagSTREAM_BUF_GLOBAL_S
{
    pthread_mutex_t     stMutex;        /* Mutex */
    struct list_head    stBufHead;      /* Instance list head */
} STREAM_BUF_GLOBAL_S;

/* Describe a frame buffer instance */
typedef struct tagFRAME_BUF_INST_S
{
    HI_HANDLE     hBuf;                 /* Frame buffer handle */
    HI_BOOL       bGetPutFlag;          /* Gut/Put flag */
    HI_DRV_VDEC_FRAME_BUF_S stLastGet;         /* Last get buffer */
    struct list_head stBufNode;         /* Instance list node */
} FRAME_BUF_INST_S;

/* Global parameters for stream buffer */
typedef struct tagFRAME_BUF_GLOBAL_S
{
    pthread_mutex_t     stMutex;        /* Mutex */
    struct list_head    stBufHead;      /* Instance list head */
} FRAME_BUF_GLOBAL_S;


/***************************** Global Definition *****************************/


/***************************** Static Definition *****************************/

static HI_S32 VFMW_GetCap(HI_CODEC_CAP_S *pstCodecCap);
static HI_S32 VFMW_Create(HI_HANDLE* phInst, const HI_CODEC_OPENPARAM_S * pstParam);
static HI_S32 VFMW_Destroy(HI_HANDLE hInst);
static HI_S32 VFMW_Start(HI_HANDLE hInst);
static HI_S32 VFMW_Stop(HI_HANDLE hInst);
static HI_S32 VFMW_Reset(HI_HANDLE hInst);
static HI_S32 VFMW_SetAttr(HI_HANDLE hInst, const HI_CODEC_ATTR_S * pstAttr);
static HI_S32 VFMW_GetAttr(HI_HANDLE hInst, HI_CODEC_ATTR_S* pstAttr);
static HI_S32 VFMW_GetStreamInfo(HI_HANDLE hInst, HI_CODEC_STREAMINFO_S *pstStreamInfo);
static HI_S32 VFMW_Control(HI_HANDLE hInst, HI_U32 u32CMD, HI_VOID * pParam);

static VDEC_ADP_GLOBAL_S s_stVdecAdpParam =
{
    .pszDevPath   = "/dev/" UMAP_DEVNAME_VDEC,
    .s32DevFd     = -1,
    .u32InitCount = 0,
    .stCap        = {HI_CODEC_CAP_DRIVENSELF | HI_CODEC_CAP_OUTPUT2SELFADDR, HI_NULL},
    .stMutex      = PTHREAD_MUTEX_INITIALIZER,
    .stInstHead   = {&s_stVdecAdpParam.stInstHead, &s_stVdecAdpParam.stInstHead}
};

static HI_CODEC_S s_stCodec =
{
    .pszName		= "VFMW",
    .unVersion		= {.stVersion = {1, 0, 0, 0}},
    .pszDescription = "Hisilicon hardware codec",

    .GetCap			= VFMW_GetCap,
    .Create			= VFMW_Create,
    .Destroy		= VFMW_Destroy,
    .Start			= VFMW_Start,
    .Stop			= VFMW_Stop,
    .Reset			= VFMW_Reset,
    .SetAttr		= VFMW_SetAttr,
    .GetAttr		= VFMW_GetAttr,
    .DecodeFrame	= HI_NULL,
    .EncodeFrame	= HI_NULL,
    .GetStreamInfo	= VFMW_GetStreamInfo,
    .Control		= VFMW_Control,
};

static STREAM_BUF_GLOBAL_S s_stStrmBufParam =
{
    .stMutex   = PTHREAD_MUTEX_INITIALIZER,
    .stBufHead = {&s_stStrmBufParam.stBufHead,	&s_stStrmBufParam.stBufHead}
};

static FRAME_BUF_GLOBAL_S s_stFrmBufParam =
{
    .stMutex   = PTHREAD_MUTEX_INITIALIZER,
    .stBufHead = {&s_stFrmBufParam.stBufHead,	&s_stFrmBufParam.stBufHead}
};

/*********************************** Code ************************************/

/* Open VDEC device, initialize all the channel config info and the ES buffer */
HI_S32 VDEC_OpenDevFile(HI_VOID)
{
    VDEC_LOCK(s_stVdecAdpParam.stMutex);

    if (0 == s_stVdecAdpParam.u32InitCount)
    {
        if (-1 == s_stVdecAdpParam.s32DevFd)
        {
            /* Open dev */
            s_stVdecAdpParam.s32DevFd = open(s_stVdecAdpParam.pszDevPath, O_RDWR | O_NONBLOCK, 0);
            if (-1 == s_stVdecAdpParam.s32DevFd)
            {
                VDEC_UNLOCK(s_stVdecAdpParam.stMutex);
                HI_FATAL_VDEC("Open video device err!\n");
                return HI_ERR_VDEC_NOT_OPEN;
            }
        }
    }

    s_stVdecAdpParam.u32InitCount++;
    VDEC_UNLOCK(s_stVdecAdpParam.stMutex);
    return HI_SUCCESS;
}

HI_S32 VDEC_CloseDevFile(HI_VOID)
{
    VDEC_LOCK(s_stVdecAdpParam.stMutex);

    if (1 == s_stVdecAdpParam.u32InitCount)
    {
        /* Free VFMW capability structure */
        if (s_stVdecAdpParam.stCap.pstSupport)
        {
            HI_FREE_VDEC(s_stVdecAdpParam.stCap.pstSupport);
            s_stVdecAdpParam.stCap.pstSupport = HI_NULL;
        }

        /* Close dev */
        if (-1 != s_stVdecAdpParam.s32DevFd)
        {
            close(s_stVdecAdpParam.s32DevFd);
            s_stVdecAdpParam.s32DevFd = -1;
        }
    }

    if (s_stVdecAdpParam.u32InitCount > 0)
    {
        s_stVdecAdpParam.u32InitCount--;
    }

    VDEC_UNLOCK(s_stVdecAdpParam.stMutex);
    return HI_SUCCESS;
}

HI_CODEC_ID_E VDEC_UNF2CodecId(HI_UNF_VCODEC_TYPE_E enType)
{
    switch (enType)
    {
    case HI_UNF_VCODEC_TYPE_MPEG2:
        return HI_CODEC_ID_VIDEO_MPEG2;
    case HI_UNF_VCODEC_TYPE_MPEG4:
        return HI_CODEC_ID_VIDEO_MPEG4;
    case HI_UNF_VCODEC_TYPE_AVS:
        return HI_CODEC_ID_VIDEO_AVS;
    case HI_UNF_VCODEC_TYPE_H263:
        return HI_CODEC_ID_VIDEO_H263;
    case HI_UNF_VCODEC_TYPE_H264:
        return HI_CODEC_ID_VIDEO_H264;
    case HI_UNF_VCODEC_TYPE_REAL8:
        return HI_CODEC_ID_VIDEO_REAL8;
    case HI_UNF_VCODEC_TYPE_REAL9:
        return HI_CODEC_ID_VIDEO_REAL9;
    case HI_UNF_VCODEC_TYPE_VC1:
        return HI_CODEC_ID_VIDEO_VC1;
    case HI_UNF_VCODEC_TYPE_VP6:
        return HI_CODEC_ID_VIDEO_VP6;
    case HI_UNF_VCODEC_TYPE_VP6F:
        return HI_CODEC_ID_VIDEO_VP6F;
    case HI_UNF_VCODEC_TYPE_VP6A:
        return HI_CODEC_ID_VIDEO_VP6A;
    case HI_UNF_VCODEC_TYPE_VP8:
        return HI_CODEC_ID_VIDEO_VP8;
    case HI_UNF_VCODEC_TYPE_MJPEG:
        return HI_CODEC_ID_VIDEO_MJPEG;
    case HI_UNF_VCODEC_TYPE_SORENSON:
        return HI_CODEC_ID_VIDEO_SORENSON;
    case HI_UNF_VCODEC_TYPE_DIVX3:
        return HI_CODEC_ID_VIDEO_DIVX3;
    case HI_UNF_VCODEC_TYPE_RAW:
        return HI_CODEC_ID_VIDEO_RAW;
    case HI_UNF_VCODEC_TYPE_JPEG:
        return HI_CODEC_ID_VIDEO_JPEG;
    case HI_UNF_VCODEC_TYPE_MSMPEG4V1:
        return HI_CODEC_ID_VIDEO_MSMPEG4V1;
    case HI_UNF_VCODEC_TYPE_MSMPEG4V2:
        return HI_CODEC_ID_VIDEO_MSMPEG4V2;
    case HI_UNF_VCODEC_TYPE_MSVIDEO1:
        return HI_CODEC_ID_VIDEO_MSVIDEO1;
    case HI_UNF_VCODEC_TYPE_WMV1:
        return HI_CODEC_ID_VIDEO_WMV1;
    case HI_UNF_VCODEC_TYPE_WMV2:
        return HI_CODEC_ID_VIDEO_WMV2;
    case HI_UNF_VCODEC_TYPE_RV10:
        return HI_CODEC_ID_VIDEO_RV10;
    case HI_UNF_VCODEC_TYPE_RV20:
        return HI_CODEC_ID_VIDEO_RV20;
    case HI_UNF_VCODEC_TYPE_SVQ1:
        return HI_CODEC_ID_VIDEO_SVQ1;
    case HI_UNF_VCODEC_TYPE_SVQ3:
        return HI_CODEC_ID_VIDEO_SVQ3;
    case HI_UNF_VCODEC_TYPE_H261:
        return HI_CODEC_ID_VIDEO_H261;
    case HI_UNF_VCODEC_TYPE_VP3:
        return HI_CODEC_ID_VIDEO_VP3;
    case HI_UNF_VCODEC_TYPE_VP5:
        return HI_CODEC_ID_VIDEO_VP5;
    case HI_UNF_VCODEC_TYPE_CINEPAK:
        return HI_CODEC_ID_VIDEO_CINEPAK;
    case HI_UNF_VCODEC_TYPE_INDEO2:
        return HI_CODEC_ID_VIDEO_INDEO2;
    case HI_UNF_VCODEC_TYPE_INDEO3:
        return HI_CODEC_ID_VIDEO_INDEO3;
    case HI_UNF_VCODEC_TYPE_INDEO4:
        return HI_CODEC_ID_VIDEO_INDEO4;
    case HI_UNF_VCODEC_TYPE_INDEO5:
        return HI_CODEC_ID_VIDEO_INDEO5;
    case HI_UNF_VCODEC_TYPE_MJPEGB:
        return HI_CODEC_ID_VIDEO_MJPEGB;
    case HI_UNF_VCODEC_TYPE_DV:
		return HI_CODEC_ID_VIDEO_DV;
	case HI_UNF_VCODEC_TYPE_HEVC:
		return HI_CODEC_ID_VIDEO_HEVC;
    default:
        return HI_CODEC_ID_NONE;
    }
}

HI_UNF_VCODEC_TYPE_E VDEC_CodecId2UNF(HI_CODEC_ID_E enCodecId)
{
    switch (enCodecId)
    {
    case HI_CODEC_ID_VIDEO_MPEG2:
        return HI_UNF_VCODEC_TYPE_MPEG2;
    case HI_CODEC_ID_VIDEO_MPEG4:
        return HI_UNF_VCODEC_TYPE_MPEG4;
    case HI_CODEC_ID_VIDEO_AVS:
        return HI_UNF_VCODEC_TYPE_AVS;
    case HI_CODEC_ID_VIDEO_H263:
        return HI_UNF_VCODEC_TYPE_H263;
    case HI_CODEC_ID_VIDEO_H264:
        return HI_UNF_VCODEC_TYPE_H264;
    case HI_CODEC_ID_VIDEO_HEVC:
        return HI_UNF_VCODEC_TYPE_HEVC;
    case HI_CODEC_ID_VIDEO_REAL8:
        return HI_UNF_VCODEC_TYPE_REAL8;
    case HI_CODEC_ID_VIDEO_REAL9:
        return HI_UNF_VCODEC_TYPE_REAL9;
    case HI_CODEC_ID_VIDEO_VC1:
        return HI_UNF_VCODEC_TYPE_VC1;
    case HI_CODEC_ID_VIDEO_VP6:
        return HI_UNF_VCODEC_TYPE_VP6;
    case HI_CODEC_ID_VIDEO_VP6F:
        return HI_UNF_VCODEC_TYPE_VP6F;
    case HI_CODEC_ID_VIDEO_VP6A:
        return HI_UNF_VCODEC_TYPE_VP6A;
    case HI_CODEC_ID_VIDEO_VP8:
        return HI_UNF_VCODEC_TYPE_VP8;
    case HI_CODEC_ID_VIDEO_MJPEG:
        return HI_UNF_VCODEC_TYPE_MJPEG;
    case HI_CODEC_ID_VIDEO_SORENSON:
        return HI_UNF_VCODEC_TYPE_SORENSON;
    case HI_CODEC_ID_VIDEO_DIVX3:
        return HI_UNF_VCODEC_TYPE_DIVX3;
    case HI_CODEC_ID_VIDEO_RAW:
        return HI_UNF_VCODEC_TYPE_RAW;
    case HI_CODEC_ID_VIDEO_JPEG:
        return HI_UNF_VCODEC_TYPE_JPEG;
    case HI_CODEC_ID_VIDEO_MSMPEG4V1:
        return HI_UNF_VCODEC_TYPE_MSMPEG4V1;
    case HI_CODEC_ID_VIDEO_MSMPEG4V2:
        return HI_UNF_VCODEC_TYPE_MSMPEG4V2;
    case HI_CODEC_ID_VIDEO_MSVIDEO1:
        return HI_UNF_VCODEC_TYPE_MSVIDEO1;
    case HI_CODEC_ID_VIDEO_WMV1:
        return HI_UNF_VCODEC_TYPE_WMV1;
    case HI_CODEC_ID_VIDEO_WMV2:
        return HI_UNF_VCODEC_TYPE_WMV2;
    case HI_CODEC_ID_VIDEO_RV10:
        return HI_UNF_VCODEC_TYPE_RV10;
    case HI_CODEC_ID_VIDEO_RV20:
        return HI_UNF_VCODEC_TYPE_RV20;
    case HI_CODEC_ID_VIDEO_SVQ1:
        return HI_UNF_VCODEC_TYPE_SVQ1;
    case HI_CODEC_ID_VIDEO_SVQ3:
        return HI_UNF_VCODEC_TYPE_SVQ3;
    case HI_CODEC_ID_VIDEO_H261:
        return HI_UNF_VCODEC_TYPE_H261;
    case HI_CODEC_ID_VIDEO_VP3:
        return HI_UNF_VCODEC_TYPE_VP3;
    case HI_CODEC_ID_VIDEO_VP5:
        return HI_UNF_VCODEC_TYPE_VP5;
    case HI_CODEC_ID_VIDEO_CINEPAK:
        return HI_UNF_VCODEC_TYPE_CINEPAK;
    case HI_CODEC_ID_VIDEO_INDEO2:
        return HI_UNF_VCODEC_TYPE_INDEO2;
    case HI_CODEC_ID_VIDEO_INDEO3:
        return HI_UNF_VCODEC_TYPE_INDEO3;
    case HI_CODEC_ID_VIDEO_INDEO4:
        return HI_UNF_VCODEC_TYPE_INDEO4;
    case HI_CODEC_ID_VIDEO_INDEO5:
        return HI_UNF_VCODEC_TYPE_INDEO5;
    case HI_CODEC_ID_VIDEO_MJPEGB:
        return HI_UNF_VCODEC_TYPE_MJPEGB;
	case HI_CODEC_ID_VIDEO_DV:
		return HI_UNF_VCODEC_TYPE_DV;
    default:
        return HI_UNF_VCODEC_TYPE_BUTT;
    }
}

static HI_CODEC_ID_E VDEC_VFMW_STD2CodecId(VID_STD_E enVidStd)
{
    switch (enVidStd)
    {
    case VFMW_H264:
        return HI_CODEC_ID_VIDEO_H264;
    case VFMW_HEVC:
        return HI_CODEC_ID_VIDEO_HEVC;
    case VFMW_VC1:
        return HI_CODEC_ID_VIDEO_VC1;
    case VFMW_MPEG4:
        return HI_CODEC_ID_VIDEO_MPEG4;
    case VFMW_MPEG2:
        return HI_CODEC_ID_VIDEO_MPEG2;
    case VFMW_H263:
        return HI_CODEC_ID_VIDEO_H263;
    case VFMW_DIVX3:
        return HI_CODEC_ID_VIDEO_DIVX3;
    case VFMW_AVS:
        return HI_CODEC_ID_VIDEO_AVS;
    case VFMW_JPEG:
        return HI_CODEC_ID_VIDEO_JPEG;
    case VFMW_REAL8:
        return HI_CODEC_ID_VIDEO_REAL8;
    case VFMW_REAL9:
        return HI_CODEC_ID_VIDEO_REAL9;
    case VFMW_VP6:
        return HI_CODEC_ID_VIDEO_VP6;
    case VFMW_VP6F:
        return HI_CODEC_ID_VIDEO_VP6F;
    case VFMW_VP6A:
        return HI_CODEC_ID_VIDEO_VP6A;
    case VFMW_VP8:
        return HI_CODEC_ID_VIDEO_VP8;
    case VFMW_SORENSON:
        return HI_CODEC_ID_VIDEO_SORENSON;
    case VFMW_RAW:
        return HI_CODEC_ID_VIDEO_RAW;
    default:
        return HI_CODEC_ID_NONE;
    }
}

HI_UNF_ENC_FMT_E VDEC_DisplayFmt2UNF(HI_CODEC_ENC_FMT_E enDisplayNorm)
{
    switch(enDisplayNorm)
    {
    case HI_CODEC_ENC_FMT_1080P_60:
        return HI_UNF_ENC_FMT_1080P_60;
    case HI_CODEC_ENC_FMT_1080P_50:
        return HI_UNF_ENC_FMT_1080P_50;
    case HI_CODEC_ENC_FMT_1080P_30:
        return HI_UNF_ENC_FMT_1080P_30;
    case HI_CODEC_ENC_FMT_1080P_25:
        return HI_UNF_ENC_FMT_1080P_25;
    case HI_CODEC_ENC_FMT_1080P_24:
        return HI_UNF_ENC_FMT_1080P_24;

    case HI_CODEC_ENC_FMT_1080i_60:
        return HI_UNF_ENC_FMT_1080i_60;
   case HI_CODEC_ENC_FMT_1080i_50:
        return HI_UNF_ENC_FMT_1080i_50;
    case HI_CODEC_ENC_FMT_720P_60:
        return HI_UNF_ENC_FMT_720P_60;
    case HI_CODEC_ENC_FMT_720P_50:
        return HI_UNF_ENC_FMT_720P_50;

    case HI_CODEC_ENC_FMT_576P_50:
        return HI_UNF_ENC_FMT_576P_50;
    case HI_CODEC_ENC_FMT_480P_60:
        return HI_UNF_ENC_FMT_480P_60;

    case HI_CODEC_ENC_FMT_PAL:
        return HI_UNF_ENC_FMT_PAL;
    case HI_CODEC_ENC_FMT_PAL_N:
        return HI_UNF_ENC_FMT_PAL_N;
    case HI_CODEC_ENC_FMT_PAL_Nc:
        return HI_UNF_ENC_FMT_PAL_Nc;

    case HI_CODEC_ENC_FMT_NTSC:
        return HI_UNF_ENC_FMT_NTSC;
    case HI_CODEC_ENC_FMT_NTSC_J:
        return HI_UNF_ENC_FMT_NTSC_J;
    case HI_CODEC_ENC_FMT_NTSC_PAL_M:
        return HI_UNF_ENC_FMT_NTSC_PAL_M;

    case HI_CODEC_ENC_FMT_SECAM_SIN:
        return HI_UNF_ENC_FMT_SECAM_SIN;
    case HI_CODEC_ENC_FMT_SECAM_COS:
        return HI_UNF_ENC_FMT_SECAM_COS;
        
    case HI_CODEC_ENC_FMT_1080P_24_FRAME_PACKING:
        return HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING;
    case HI_CODEC_ENC_FMT_720P_60_FRAME_PACKING:
        return HI_UNF_ENC_FMT_720P_60_FRAME_PACKING;
    case HI_CODEC_ENC_FMT_720P_50_FRAME_PACKING:
        return HI_UNF_ENC_FMT_720P_50_FRAME_PACKING;
    
    case HI_CODEC_ENC_FMT_861D_640X480_60:
        return HI_UNF_ENC_FMT_861D_640X480_60;
        
    case HI_CODEC_ENC_FMT_VESA_800X600_60:
        return HI_UNF_ENC_FMT_VESA_800X600_60;
    case HI_CODEC_ENC_FMT_VESA_1024X768_60:
        return HI_UNF_ENC_FMT_VESA_1024X768_60;
    case HI_CODEC_ENC_FMT_VESA_1280X720_60:
        return HI_UNF_ENC_FMT_VESA_1280X720_60;
    case HI_CODEC_ENC_FMT_VESA_1280X800_60:
        return HI_UNF_ENC_FMT_VESA_1280X800_60;
    case HI_CODEC_ENC_FMT_VESA_1280X1024_60:
        return HI_UNF_ENC_FMT_VESA_1280X1024_60;
    case HI_CODEC_ENC_FMT_VESA_1360X768_60:
        return HI_UNF_ENC_FMT_VESA_1360X768_60;
    case HI_CODEC_ENC_FMT_VESA_1366X768_60:
        return HI_UNF_ENC_FMT_VESA_1366X768_60;
    case HI_CODEC_ENC_FMT_VESA_1400X1050_60:
        return HI_UNF_ENC_FMT_VESA_1400X1050_60;
    case HI_CODEC_ENC_FMT_VESA_1440X900_60:
        return HI_UNF_ENC_FMT_VESA_1440X900_60;
    case HI_CODEC_ENC_FMT_VESA_1440X900_60_RB:
        return HI_UNF_ENC_FMT_VESA_1440X900_60_RB;
    case HI_CODEC_ENC_FMT_VESA_1600X900_60_RB:
        return HI_UNF_ENC_FMT_VESA_1600X900_60_RB;
    case HI_CODEC_ENC_FMT_VESA_1600X1200_60:
        return HI_UNF_ENC_FMT_VESA_1600X1200_60;
    case HI_CODEC_ENC_FMT_VESA_1680X1050_60:
        return HI_UNF_ENC_FMT_VESA_1680X1050_60;
    case HI_CODEC_ENC_FMT_VESA_1920X1080_60:
        return HI_UNF_ENC_FMT_VESA_1920X1080_60;
    case HI_CODEC_ENC_FMT_VESA_1920X1200_60:
        return HI_UNF_ENC_FMT_VESA_1920X1200_60;
    case HI_CODEC_ENC_FMT_VESA_2048X1152_60:
        return HI_UNF_ENC_FMT_VESA_2048X1152_60;
     default:
        HI_INFO_VDEC("VDEC Unknow Dispfrm\n");
        return HI_UNF_ENC_FMT_BUTT;
    }
}
HI_CODEC_ENC_FMT_E VDEC_UNFDisplayFmt2CODEC(HI_UNF_ENC_FMT_E enDisplayNorm)
{
    switch(enDisplayNorm)
    {
     case HI_UNF_ENC_FMT_1080P_60:
        return HI_CODEC_ENC_FMT_1080P_60;
        
    case HI_UNF_ENC_FMT_1080P_50:
        return HI_CODEC_ENC_FMT_1080P_50;

    case HI_UNF_ENC_FMT_1080P_30:
        return HI_CODEC_ENC_FMT_1080P_30;
        
    case HI_UNF_ENC_FMT_1080P_25:
        return HI_CODEC_ENC_FMT_1080P_25;

    case HI_UNF_ENC_FMT_1080P_24:
        return HI_CODEC_ENC_FMT_1080P_24;

    case HI_UNF_ENC_FMT_1080i_60:
        return HI_CODEC_ENC_FMT_1080i_60;

    case HI_UNF_ENC_FMT_1080i_50:
           return HI_CODEC_ENC_FMT_1080i_50;
               
    case HI_UNF_ENC_FMT_720P_60:
        return HI_CODEC_ENC_FMT_720P_60;
    
    case HI_UNF_ENC_FMT_720P_50:
        return HI_CODEC_ENC_FMT_720P_50;

    case HI_UNF_ENC_FMT_576P_50:
        return HI_CODEC_ENC_FMT_576P_50;
        
    case HI_UNF_ENC_FMT_480P_60:
        return HI_CODEC_ENC_FMT_480P_60;

    case HI_UNF_ENC_FMT_PAL:
        return HI_CODEC_ENC_FMT_PAL;

    case HI_UNF_ENC_FMT_PAL_N:
        return HI_CODEC_ENC_FMT_PAL_N;

    case HI_UNF_ENC_FMT_PAL_Nc:
        return HI_CODEC_ENC_FMT_PAL_Nc;

    case HI_UNF_ENC_FMT_NTSC:
        return HI_CODEC_ENC_FMT_NTSC;

    case HI_UNF_ENC_FMT_NTSC_J:
        return HI_CODEC_ENC_FMT_NTSC_J;

    case HI_UNF_ENC_FMT_NTSC_PAL_M:
        return HI_CODEC_ENC_FMT_NTSC_PAL_M;


    case HI_UNF_ENC_FMT_SECAM_SIN:
        return HI_CODEC_ENC_FMT_SECAM_SIN;

    case HI_UNF_ENC_FMT_SECAM_COS:
        return HI_CODEC_ENC_FMT_SECAM_COS;

    
    case HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING:
        return HI_CODEC_ENC_FMT_1080P_24_FRAME_PACKING;

    case HI_UNF_ENC_FMT_720P_60_FRAME_PACKING:
        return HI_CODEC_ENC_FMT_720P_60_FRAME_PACKING;

    case HI_UNF_ENC_FMT_720P_50_FRAME_PACKING:
        return HI_CODEC_ENC_FMT_720P_50_FRAME_PACKING;

    case HI_UNF_ENC_FMT_861D_640X480_60:
        return HI_CODEC_ENC_FMT_861D_640X480_60;

    
    case HI_UNF_ENC_FMT_VESA_800X600_60:
        return HI_CODEC_ENC_FMT_VESA_800X600_60;

    case HI_UNF_ENC_FMT_VESA_1024X768_60:
        return HI_CODEC_ENC_FMT_VESA_1024X768_60;

    case HI_UNF_ENC_FMT_VESA_1280X720_60:
        return HI_CODEC_ENC_FMT_VESA_1280X720_60;

    case HI_UNF_ENC_FMT_VESA_1280X800_60:
        return HI_CODEC_ENC_FMT_VESA_1280X800_60;

    case HI_UNF_ENC_FMT_VESA_1280X1024_60:
        return HI_CODEC_ENC_FMT_VESA_1280X1024_60;

    case HI_UNF_ENC_FMT_VESA_1360X768_60:
        return HI_CODEC_ENC_FMT_VESA_1360X768_60;

    case HI_UNF_ENC_FMT_VESA_1366X768_60:
        return HI_CODEC_ENC_FMT_VESA_1366X768_60;

    case HI_UNF_ENC_FMT_VESA_1400X1050_60:
        return HI_CODEC_ENC_FMT_VESA_1400X1050_60;

    case HI_UNF_ENC_FMT_VESA_1440X900_60:
        return HI_CODEC_ENC_FMT_VESA_1440X900_60;

    case HI_UNF_ENC_FMT_VESA_1440X900_60_RB:
        return HI_CODEC_ENC_FMT_VESA_1440X900_60_RB;

    case HI_UNF_ENC_FMT_VESA_1600X900_60_RB:
        return HI_CODEC_ENC_FMT_VESA_1600X900_60_RB;

    case HI_UNF_ENC_FMT_VESA_1600X1200_60:
        return HI_CODEC_ENC_FMT_VESA_1600X1200_60;

    case HI_UNF_ENC_FMT_VESA_1680X1050_60:
        return HI_CODEC_ENC_FMT_VESA_1680X1050_60;

    case HI_UNF_ENC_FMT_VESA_1920X1080_60:
        return HI_CODEC_ENC_FMT_VESA_1920X1080_60;

    case HI_UNF_ENC_FMT_VESA_1920X1200_60:
        return HI_CODEC_ENC_FMT_VESA_1920X1200_60;

    case HI_UNF_ENC_FMT_VESA_2048X1152_60:
        return HI_CODEC_ENC_FMT_VESA_2048X1152_60;

     default:
        HI_INFO_VDEC("VDEC Unknow UNF Dispfrm\n");
        return HI_CODEC_ENC_FMT_BUTT;
    }
}
static HI_S32 VDEC_VFMW_CheckAttr(const HI_UNF_VCODEC_ATTR_S *pstAttr)
{
    if (HI_NULL == pstAttr)
    {
        HI_ERR_VDEC("Bad priv attr!\n");
        return HI_FAILURE;
    }

    if (pstAttr->enType >= HI_UNF_VCODEC_TYPE_BUTT)
    {
        HI_ERR_VDEC("Unsupport protocol: %d!\n", pstAttr->enType);
        return HI_FAILURE;
    }

    if (pstAttr->enMode >= HI_UNF_VCODEC_MODE_BUTT)
    {
        HI_ERR_VDEC("Unsupport mode: %d!\n", pstAttr->enMode);
        return HI_FAILURE;
    }

    if (pstAttr->u32ErrCover > 100)
    {
        HI_ERR_VDEC("Unsupport err_cover: %d!\n", pstAttr->u32ErrCover);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 VFMW_GetCap(HI_CODEC_CAP_S *pstCodecCap)
{
    HI_S32 s32Ret;
    HI_U32 i;
    HI_U32 u32Num = 0;
    VDEC_CAP_S stCap;
    HI_CODEC_SUPPORT_S* pstSupport;
    HI_CODEC_SUPPORT_S* pstTmp;

    VDEC_LOCK(s_stVdecAdpParam.stMutex);

    /* If get capability the first time */
    if (HI_NULL == s_stVdecAdpParam.stCap.pstSupport)
    {
        /* Get VFMW capability */
        s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_GETCAP, &stCap);

        if (HI_SUCCESS != s32Ret)
        {
            VDEC_UNLOCK(s_stVdecAdpParam.stMutex);
            return HI_FAILURE;
        }

        /* Calc standard number */
        while (VFMW_END_RESERVED != stCap.SupportedStd[u32Num++])
        {
            ;
        }

        /* Allocate memory for HI_CODEC_SUPPORT_S */
        pstSupport =  (HI_CODEC_SUPPORT_S*)HI_MALLOC_VDEC(sizeof(HI_CODEC_SUPPORT_S) * u32Num);
        if (HI_NULL == pstSupport)
        {
            VDEC_UNLOCK(s_stVdecAdpParam.stMutex);
            return HI_FAILURE;
        }
        pstTmp = pstSupport;
        s_stVdecAdpParam.stCap.pstSupport = pstSupport;

        /* Setup stCap.SupportedStd to s_stVdecAdpParam.stCap.pstSupport */
        for (i = 0; i < u32Num; i++)
        {
            pstSupport = pstTmp;
            pstSupport->u32Type = HI_CODEC_TYPE_DEC;
            pstSupport->enID = VDEC_VFMW_STD2CodecId(stCap.SupportedStd[i]);				
            pstSupport->pstNext = pstSupport + 1;
            pstTmp = pstSupport + 1;
        }

        pstSupport->pstNext = HI_NULL;
    }

    *pstCodecCap = s_stVdecAdpParam.stCap;
    VDEC_UNLOCK(s_stVdecAdpParam.stMutex);
    return HI_SUCCESS;
}

/* Create instance */
static HI_S32 VFMW_Create(HI_HANDLE* phInst, const HI_CODEC_OPENPARAM_S * pstParam)
{
    HI_S32 s32Ret;
    HI_UNF_AVPLAY_OPEN_OPT_S*   pstOpenParam;
    VDEC_CMD_ALLOC_S            stParam;
    VFMW_INST_S*                pstVFMWInst;
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    VDEC_CMD_USERDATABUF_S      stUsrData = {0};
    VDEC_CMD_BUF_USERADDR_OLD_S     stUserAddr = {0};
#endif

    if (HI_NULL == phInst)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = *phInst;
    
    /* If HI_NULL == pstParam, use default parameter */
    if (HI_NULL == pstParam)
    {
        stParam.stOpenOpt.enDecType  = HI_UNF_VCODEC_DEC_TYPE_NORMAL;
        stParam.stOpenOpt.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
        stParam.stOpenOpt.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_H264;
    }
    /* Else, use pstParam */
    else
    {
        /* Check parameter */
        pstOpenParam = (HI_UNF_AVPLAY_OPEN_OPT_S*)pstParam->unParam.stVdec.pPlatformPriv;
        if (HI_NULL == pstOpenParam)
        {
            HI_ERR_VDEC("Bad open param!\n");
            return HI_ERR_CODEC_INVALIDPARAM;
        }

        if (pstOpenParam->enDecType >= HI_UNF_VCODEC_DEC_TYPE_BUTT)
        {
            HI_ERR_VDEC("Bad enDecType:%d!\n", pstOpenParam->enDecType);
            return HI_ERR_CODEC_INVALIDPARAM;
        }

        if (pstOpenParam->enCapLevel >= HI_UNF_VCODEC_CAP_LEVEL_BUTT)
        {
            HI_ERR_VDEC("Bad enH264CapLevel:%d!\n", pstOpenParam->enCapLevel);
            return HI_ERR_CODEC_INVALIDPARAM;
        }

        if (pstOpenParam->enProtocolLevel >= HI_UNF_VCODEC_PRTCL_LEVEL_BUTT)
        {
            HI_ERR_VDEC("Bad enProtocolLevel:%d!\n", pstOpenParam->enProtocolLevel);
            return HI_ERR_CODEC_INVALIDPARAM;
        }

        stParam.stOpenOpt.enDecType  = pstOpenParam->enDecType;
        stParam.stOpenOpt.enCapLevel = pstOpenParam->enCapLevel;
        stParam.stOpenOpt.enProtocolLevel = pstOpenParam->enProtocolLevel;
    }
    
    if(NULL != pstParam)
    {
        #if (1 == HI_VDEC_DFS_SUPPORT)  
        if((HI_CODEC_ID_VIDEO_H263 != pstParam->enID) && \
           (HI_CODEC_ID_VIDEO_MJPEG != pstParam->enID) && \
           (HI_CODEC_ID_VIDEO_SORENSON != pstParam->enID))//l00273086
        {
            stParam.u32DFSEnable = 1;
        }
        else
        {
            stParam.u32DFSEnable = 0;
        }    
    #else  
        stParam.u32DFSEnable = 0;        
    #endif

    }
    

    /* Ioctl UMAPC_VDEC_CHAN_ALLOC */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_ALLOC, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan alloc err:%x!\n", s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    /* Output new handle */
    memcpy(phInst, &stParam, sizeof(HI_HANDLE));
    if (HI_INVALID_HANDLE == *phInst)
    {
        HI_ERR_VDEC("hInst err!\n");
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    VDEC_LOCK(s_stVdecAdpParam.stMutex);
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)    
    stUsrData.hHandle = VFMW_INST_HANDLE(*phInst);
    stUsrData.stBuf.u32Size = HI_VDEC_USERDATA_CC_BUFSIZE;
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_USERDATAINITBUF, &stUsrData);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Init user data buf fail!\n");
        goto err;
    }
        
    /* Map MMZ */
    stUserAddr.u32UserAddr = (HI_U32)HI_MEM_Map(stUsrData.stBuf.u32PhyAddr, stUsrData.stBuf.u32Size);
    if (HI_NULL == stUserAddr.u32UserAddr)
    {
        HI_ERR_VDEC("HI_MMZ_Map fail.\n");
        goto err;
    }
        
    /* Told the user virtual to kernel */
    stUserAddr.hHandle = stUsrData.hHandle;
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_USERDATASETBUFADDR_OLD, &stUserAddr);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VDEC("Set userdata addr fail.\n");
        goto err;
    }
    
    HI_INFO_VDEC("USERDATA MMZ %#x map to %#x, %dB\n", 
        stUsrData.stBuf.u32PhyAddr, stUserAddr.u32UserAddr, stUsrData.stBuf.u32Size);
#endif
    
    pstVFMWInst = (VFMW_INST_S*)HI_MALLOC_VDEC(sizeof(VFMW_INST_S));
    if (HI_NULL == pstVFMWInst)
    {
        HI_ERR_VDEC("No memory!\n");
        goto err;
    }
    pstVFMWInst->hInst = VFMW_INST_HANDLE(*phInst);
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    pstVFMWInst->pu8UserDataVirAddr = (HI_U8*)stUserAddr.u32UserAddr;
#endif

    list_add_tail(&pstVFMWInst->stInstNode, &s_stVdecAdpParam.stInstHead);
    VDEC_UNLOCK(s_stVdecAdpParam.stMutex);

    HI_INFO_VDEC("Alloc chan hInst=%d\n", *phInst);
    return HI_SUCCESS;

err:
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    if (0 != stUserAddr.u32UserAddr)
    {
        (HI_VOID)HI_MEM_Unmap((HI_VOID*)stUserAddr.u32UserAddr);
    }
#endif
    
    (HI_VOID)ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_FREE, &stParam);
    *phInst = HI_INVALID_HANDLE;
    return HI_ERR_CODEC_NOENOUGHRES;
}

/* Destroy instance */
static HI_S32 VFMW_Destroy(HI_HANDLE hInst)
{
    HI_S32 s32Ret;
    HI_HANDLE hHandle = VFMW_INST_HANDLE(hInst);
    VFMW_INST_S* pstVFMWInst = HI_NULL;

    /* Ioctl UMAPC_VDEC_CHAN_FREE */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_FREE, &hHandle);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d destroy err:%x!\n", hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    VDEC_LOCK(s_stVdecAdpParam.stMutex);

    /* Find instance from list */
    VFMW_FIND_INST(hInst, pstVFMWInst);

    /* If find, delete it from list and free resource */
    if (pstVFMWInst)
    {
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
        if (HI_NULL != pstVFMWInst->pu8UserDataVirAddr)
        {
            (HI_VOID)HI_MEM_Unmap(pstVFMWInst->pu8UserDataVirAddr);
        }
#endif
        
        list_del(&pstVFMWInst->stInstNode);
        HI_FREE_VDEC(pstVFMWInst);
    }

    VDEC_UNLOCK(s_stVdecAdpParam.stMutex);

    HI_INFO_VDEC("Chan %d destroy.\n", hHandle);
    return HI_SUCCESS;
}

/* Start instance */
static HI_S32 VFMW_Start(HI_HANDLE hInst)
{
    HI_S32 s32Ret;
    HI_HANDLE hHandle = VFMW_INST_HANDLE(hInst);

    /* Ioctl UMAPC_VDEC_CHAN_START */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_START, &hHandle);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d start err:%x!\n", hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d start.\n", hHandle);
    return HI_SUCCESS;
}

/* Stop instance */
static HI_S32 VFMW_Stop(HI_HANDLE hInst)
{
    HI_S32 s32Ret;
    HI_HANDLE hHandle = VFMW_INST_HANDLE(hInst);

    /* Ioctl UMAPC_VDEC_CHAN_STOP */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_STOP, &hHandle);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d stop err:%x!\n", hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d stop.\n", hHandle);
    return HI_SUCCESS;
}

/* Reset instance */
static HI_S32 VFMW_Reset(HI_HANDLE hInst)
{
    HI_S32 s32Ret;
    VDEC_CMD_RESET_S stParam;

    /* Set parameter */
    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.enType = VDEC_RESET_TYPE_ALL;

    /* Ioctl UMAPC_VDEC_CHAN_RESET */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_RESET, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d reset err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d reset.\n", stParam.hHandle);
    return HI_SUCCESS;
}

/* Set attribute */
static HI_S32 VFMW_SetAttr(HI_HANDLE hInst, const HI_CODEC_ATTR_S * pstAttr)
{
    HI_S32 s32Ret;
    VDEC_CMD_ATTR_OLD_S stParam;
    HI_UNF_VCODEC_ATTR_S* pstPrivAttr;

    /* Check parameter */
    if (HI_NULL == pstAttr)
    {
        HI_ERR_VDEC("Bad attr!\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    pstPrivAttr = (HI_UNF_VCODEC_ATTR_S*)pstAttr->unAttr.stVdec.pPlatformPriv;
    if (HI_SUCCESS != VDEC_VFMW_CheckAttr(pstPrivAttr))
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* Copy parameter */
    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.stAttr = *pstPrivAttr;

    /* priority will never be 0, but do not return error */
    if (0 == stParam.stAttr.u32Priority)
    {
        stParam.stAttr.u32Priority = 1;
    }

    /* Ioctl UMAPC_VDEC_CHAN_SETATTR */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETATTR_OLD, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d SetAttr err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d SetAttr.\n", stParam.hHandle);
    return HI_SUCCESS;
}

/* Get attribute */
static HI_S32 VFMW_GetAttr(HI_HANDLE hInst, HI_CODEC_ATTR_S* pstAttr)
{
    HI_S32 s32Ret;
    HI_UNF_VCODEC_ATTR_S* pstPrivAttr;
    VDEC_CMD_ATTR_OLD_S stParam;

    /* Check parameter */
    if (HI_NULL == pstAttr)
    {
        HI_ERR_VDEC("Bad attr!\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = VFMW_INST_HANDLE(hInst);

    /* Ioctl UMAPC_VDEC_CHAN_GETATTR */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_GETATTR_OLD, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d GetAttr err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    pstPrivAttr = (HI_UNF_VCODEC_ATTR_S*)pstAttr->unAttr.stVdec.pPlatformPriv;
    *pstPrivAttr = stParam.stAttr;
    pstPrivAttr->pCodecContext = HI_NULL;
    pstAttr->enID = VDEC_UNF2CodecId(stParam.stAttr.enType);
    HI_INFO_VDEC("Chan %d GetAttr.\n", stParam.hHandle);
    return HI_SUCCESS;
}

/* Get stream info */
static HI_S32 VFMW_GetStreamInfo(HI_HANDLE hInst, HI_CODEC_STREAMINFO_S *pstStreamInfo)
{
    HI_S32 s32Ret;
    VDEC_CMD_STREAM_INFO_S stParam;

    if (HI_NULL == pstStreamInfo)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* Ioctl UMAPC_VDEC_CHAN_STREAMINFO */
    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_STREAMINFO, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d GetStreamInfo err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    pstStreamInfo->stVideo.enCodecID = VDEC_UNF2CodecId(stParam.stInfo.enVCodecType);
    pstStreamInfo->stVideo.enSubStandard = (HI_CODEC_VIDEO_SUB_STANDARD_E)stParam.stInfo.enSubStandard;
    pstStreamInfo->stVideo.u32SubVersion = stParam.stInfo.u32SubVersion;
    pstStreamInfo->stVideo.u32Profile = stParam.stInfo.u32Profile;
    pstStreamInfo->stVideo.u32Level = stParam.stInfo.u32Level;
    pstStreamInfo->stVideo.enDisplayNorm = VDEC_UNFDisplayFmt2CODEC(stParam.stInfo.enDisplayNorm);
    pstStreamInfo->stVideo.bProgressive = stParam.stInfo.bProgressive;
    pstStreamInfo->stVideo.u32AspectWidth = stParam.stInfo.u32AspectWidth;
    pstStreamInfo->stVideo.u32AspectHeight = stParam.stInfo.u32AspectHeight;
    pstStreamInfo->stVideo.u32bps = stParam.stInfo.u32bps;
    pstStreamInfo->stVideo.u32FrameRateInt = stParam.stInfo.u32fpsInteger;
    pstStreamInfo->stVideo.u32FrameRateDec = stParam.stInfo.u32fpsDecimal;
    pstStreamInfo->stVideo.u32Width  = stParam.stInfo.u32Width;
    pstStreamInfo->stVideo.u32Height = stParam.stInfo.u32Height;
    pstStreamInfo->stVideo.u32DisplayWidth   = stParam.stInfo.u32DisplayWidth;
    pstStreamInfo->stVideo.u32DisplayHeight  = stParam.stInfo.u32DisplayHeight;
    pstStreamInfo->stVideo.u32DisplayCenterX = stParam.stInfo.u32DisplayCenterX;
    pstStreamInfo->stVideo.u32DisplayCenterY = stParam.stInfo.u32DisplayCenterY;
    HI_INFO_VDEC("Chan %d GetStreamInfo OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_CheckEvt(HI_HANDLE hInst, VDEC_EVENT_S *pstNewEvent)
{
    HI_S32 s32Ret;
    VDEC_CMD_EVENT_S stParam;

    if (HI_NULL == pstNewEvent)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* Ioctl UMAPC_VDEC_CHAN_CHECKEVT */
    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_CHECKEVT, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d CheckEvt err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    *pstNewEvent = stParam.stEvent;
    HI_INFO_VDEC("Chan %d CheckEvt OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_ReadNewFrame(HI_HANDLE hInst, HI_DRV_VIDEO_FRAME_S *pstNewFrame)
{
    HI_S32 s32Ret;
    VDEC_CMD_FRAME_S stParam;

    if (HI_NULL == pstNewFrame)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* Ioctl UMAPC_VDEC_CHAN_EVNET_NEWFRAME */
    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_EVNET_NEWFRAME, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
    //    HI_ERR_VDEC("Chan %d ReadNewFrame err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    *pstNewFrame = stParam.stFrame;
    HI_INFO_VDEC("Chan %d ReadNewFrame OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_RecvUsrData(HI_HANDLE hInst, HI_UNF_VIDEO_USERDATA_S *pstUsrData)
{
    HI_S32 s32Ret;
    VFMW_INST_S* pstVFMWInst = HI_NULL;
    VDEC_CMD_USERDATA_OLD_S stParam;

    if (HI_NULL == pstUsrData)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    VDEC_LOCK(s_stVdecAdpParam.stMutex);
    VFMW_FIND_INST(hInst, pstVFMWInst);
    VDEC_UNLOCK(s_stVdecAdpParam.stMutex);
    if (pstVFMWInst)
    {
        stParam.hHandle = VFMW_INST_HANDLE(hInst);
        stParam.stUserData.pu8Buffer = pstVFMWInst->au8UsrData;

        /* Ioctl UMAPC_VDEC_CHAN_USRDATA */
        s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_USRDATA_OLD, &stParam);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_VDEC("Chan %d RecvUsrData err:%x!\n", stParam.hHandle, s32Ret);
            return HI_ERR_CODEC_INVALIDPARAM;
        }

        *pstUsrData = stParam.stUserData;
        pstUsrData->pu8Buffer = pstVFMWInst->au8UsrData;
        HI_INFO_VDEC("Chan %d RlsFrm OK\n", stParam.hHandle);
        return HI_SUCCESS;
    }

    return HI_ERR_CODEC_INVALIDPARAM;
}

static HI_S32 VFMW_SetFrmRate(HI_HANDLE hInst, const HI_UNF_AVPLAY_FRMRATE_PARAM_S *pstFrmRate)
{
    HI_S32 s32Ret;
    VDEC_CMD_FRAME_RATE_S stParam;

    if (HI_NULL == pstFrmRate)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    if ((pstFrmRate->enFrmRateType >= HI_UNF_AVPLAY_FRMRATE_TYPE_BUTT) || 
        (pstFrmRate->stSetFrmRate.u32fpsInteger > 60) ||
        (pstFrmRate->stSetFrmRate.u32fpsDecimal >= 1000))
    {
        HI_ERR_VDEC("FPS too large.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.stFrameRate = *pstFrmRate;

    /* Ioctl UMAPC_VDEC_CHAN_SETFRMRATE */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETFRMRATE, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d SetFrmRate err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d SetFrmRate OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_GetFrmRate(HI_HANDLE hInst, HI_UNF_AVPLAY_FRMRATE_PARAM_S *pstFrmRate)
{
    HI_S32 s32Ret;
    VDEC_CMD_FRAME_RATE_S stParam;

    if (HI_NULL == pstFrmRate)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* Ioctl UMAPC_VDEC_CHAN_GETFRMRATE */
    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_GETFRMRATE, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d GetFrmRate err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    *pstFrmRate = stParam.stFrameRate;
    HI_INFO_VDEC("Chan %d GetFrmRate OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_GetStatusInfo(HI_HANDLE hInst, VDEC_STATUSINFO_S* pstStatusInfo)
{
    HI_S32 s32Ret;
    VDEC_CMD_STATUS_S stParam;

    if (HI_NULL == pstStatusInfo)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* Ioctl UMAPC_VDEC_CHAN_STATUSINFO */
    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_STATUSINFO, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d GetStatusInfo err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    *pstStatusInfo = stParam.stStatus;
    HI_INFO_VDEC("Chan %d GetStatusInfo OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_SetEosFlag(HI_HANDLE hInst)
{
    HI_S32 s32Ret;
    HI_U32 u32Arg;

    u32Arg = VFMW_INST_HANDLE(hInst);

    /* Ioctl UMAPC_VDEC_CHAN_SETEOSFLAG */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETEOSFLAG, &u32Arg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d SetEosFlag err:%x!\n", u32Arg, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d SetEosFlag OK\n", u32Arg);
    return HI_SUCCESS;
}

static HI_S32 VFMW_IFrameDecode(HI_HANDLE hInst, const VFMW_IFRAME_PARAM_S* pstParam)
{
    HI_S32 s32Ret;
    VDEC_CMD_IFRAME_DEC_OLD_S stParam;

    if ((HI_NULL == pstParam)
       || (HI_NULL == pstParam->pstIFrameStream->pu8Addr))
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* Check type */
    if ((pstParam->pstIFrameStream->enType >= HI_UNF_VCODEC_TYPE_BUTT))
    {
        HI_ERR_VDEC("Unsupport protocol %d!\n", pstParam->pstIFrameStream->enType);
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.stIFrame = *(pstParam->pstIFrameStream);
    stParam.bCapture = pstParam->bCapture;

    /* Ioctl UMAPC_VDEC_CHAN_IFRMDECODE */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_IFRMDECODE_OLD, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d IFrameDecode err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d IFrameDecode OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_IFrameRelease(HI_HANDLE hInst, const HI_DRV_VIDEO_FRAME_S* pstParam)
{
    HI_S32 s32Ret;
    VDEC_CMD_IFRAME_RLS_S stParam;

    if (HI_NULL == pstParam)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }    

    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.stVoFrameInfo = *pstParam;

    /* Ioctl UMAPC_VDEC_CHAN_IFRMRELEASE */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_IFRMRELEASE, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d IFrameRelease err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d IFrameRelease OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_RecvFrm(HI_HANDLE hInst, HI_DRV_VIDEO_FRAME_S* pstFrameInfo)
{
    HI_S32 s32Ret;
    VDEC_CMD_VO_FRAME_S stParam;

    if (HI_NULL == pstFrameInfo)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = VFMW_INST_HANDLE(hInst);

    /* Ioctl UMAPC_VDEC_CHAN_RCVFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_RCVFRM, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d RecvFrm err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    *pstFrameInfo = stParam.stFrame;
    HI_INFO_VDEC("Chan %d RecvFrm OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_RlsFrm(HI_HANDLE hInst, const HI_DRV_VIDEO_FRAME_S* pstFrameInfo)
{
    HI_S32 s32Ret;
    VDEC_CMD_VO_FRAME_S stParam;

    if (HI_NULL == pstFrameInfo)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.stFrame = *pstFrameInfo;

    /* Ioctl UMAPC_VDEC_CHAN_RLSFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_RLSFRM, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d RlsFrm err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d RlsFrm OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_AttachBuf(HI_HANDLE hInst, const VFMW_STREAMBUF_S* pstParam)
{
    HI_S32 s32Ret;
    VDEC_CMD_ATTACH_BUF_S stParam;

    if (HI_NULL == pstParam)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    /* The parameters are: Instance handle\buffer size\demux handle\stream buffer handle */
    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.u32BufSize = pstParam->u32BufSize;
    stParam.hDmxVidChn = pstParam->hDmxVidChn;
    stParam.hStrmBuf = pstParam->hStrmBuf;

    /* Ioctl UMAPC_VDEC_CHAN_ATTACHBUF */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_ATTACHBUF, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d AttachStreamBuf err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d AttachStreamBuf OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_DetachBuf(HI_HANDLE hInst)
{
    HI_S32 s32Ret;
    HI_U32 u32Arg;

    u32Arg = VFMW_INST_HANDLE(hInst);

    /* Ioctl UMAPC_VDEC_CHAN_DETACHBUF */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_DETACHBUF, &u32Arg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d DetachStreamBuf err:%x!\n", u32Arg, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d DetachStreamBuf OK\n", u32Arg);
    return HI_SUCCESS;
}

static HI_S32 VFMW_DiscardFrame(HI_HANDLE hInst, const VDEC_DISCARD_FRAME_S* pstParam)
{
    HI_S32 s32Ret;
    VDEC_CMD_DISCARD_FRAME_S stParam;

    if ((HI_NULL == pstParam) || (pstParam->enMode >= VDEC_DISCARD_BUTT))
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.stDiscardOpt = *pstParam;

    /* Ioctl UMAPC_VDEC_CHAN_DISCARDFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_DISCARDFRM, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d DiscardFrame err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d DiscardFrame OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
static HI_S32 VFMW_AcqUserData(HI_HANDLE hInst, VFMW_USERDATA_S* pstParam)
{
    HI_S32 s32Ret;
    VDEC_CMD_USERDATA_ACQMODE_OLD_S stParam;

    if (HI_NULL == pstParam)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = VFMW_INST_HANDLE(hInst);

    /* Ioctl UMAPC_VDEC_CHAN_ACQUSERDATA */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_ACQUSERDATA_OLD, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_VDEC("Chan %d AcqUserData err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    *(pstParam->pstData) = stParam.stUserData;
    *(pstParam->penType) = stParam.enType;
    HI_INFO_VDEC("Chan %d AcqUserData OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_RlsUserData(HI_HANDLE hInst, const HI_UNF_VIDEO_USERDATA_S* pstParam)
{
    HI_S32 s32Ret;
    VDEC_CMD_USERDATA_OLD_S stParam;

    if (HI_NULL == pstParam)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.stUserData = *pstParam;

    /* Ioctl UMAPC_VDEC_CHAN_RLSUSERDATA */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_RLSUSERDATA_OLD, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d RlsUserData err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d RlsUserData OK\n", stParam.hHandle);
    return HI_SUCCESS;
}
#endif

static HI_S32 VFMW_DropStream(HI_HANDLE hInst,VFMW_SEEKPTS_PARAM_S* pstVfmwSeekPts)
{
    HI_S32 s32Ret;
	VDEC_CMD_SEEK_PTS_S stParam;
	if (HI_NULL == pstVfmwSeekPts)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }
	stParam.hHandle = VFMW_INST_HANDLE(hInst);
	stParam.u32Gap = pstVfmwSeekPts->u32Gap;
	stParam.u32SeekPts = *(pstVfmwSeekPts->pu32SeekPts);
    /* Ioctl UMAPC_VDEC_CHAN_SEEKPTS */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SEEKPTS, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d SeekPts err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d SeekPts OK\n", stParam.hHandle);
    return HI_SUCCESS;	
}

static HI_S32 VFMW_GetInfo(HI_HANDLE hInst, HI_UNF_AVPLAY_VDEC_INFO_S* pstParam)
{
    HI_S32 s32Ret;
    VDEC_STATUSINFO_S stStat;
    
    if (HI_NULL == pstParam)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    s32Ret = VFMW_GetStatusInfo(hInst, &stStat);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    pstParam->u32DispFrmBufNum = stStat.u32VfmwTotalDispFrmNum;
    pstParam->u32FieldFlag = stStat.u32FieldFlag;
    pstParam->stDecFrmRate = stStat.stVfmwFrameRate;
    pstParam->u32UndecFrmNum = 0;

    HI_INFO_VDEC("Chan %d GetInfo OK\n", hInst);
    return HI_SUCCESS;
}

static HI_S32 VFMW_SetTPlayOpt(HI_HANDLE hInst, HI_UNF_AVPLAY_TPLAY_OPT_S* pstParam)
{
    HI_S32 s32Ret;
    VDEC_CMD_TRICKMODE_OPT_S stParam;

    if ((HI_NULL == pstParam) || (pstParam->enTplayDirect >= HI_UNF_AVPLAY_TPLAY_DIRECT_BUTT))
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.stTPlayOpt = *pstParam;

    /* Ioctl UMAPC_VDEC_CHAN_SETTRICKMODE */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETTRICKMODE, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d SetTPlayOpt err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d SetTPlayOpt OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

static HI_S32 VFMW_SetCtrlInfo(HI_HANDLE hInst, HI_UNF_AVPLAY_CONTROL_INFO_S* pstParam)
{
    HI_S32 s32Ret;
    VDEC_CMD_SET_CTRL_INFO_S stParam;

    if (HI_NULL == pstParam)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.stCtrlInfo = *pstParam;

    /* Ioctl UMAPC_VDEC_CHAN_SETCTRLINFO */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETCTRLINFO, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d SetCtrlInfo err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }

    HI_INFO_VDEC("Chan %d SetCtrlInfo OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

 HI_S32 VFMW_SetProgressive(HI_HANDLE hInst, HI_BOOL* pParam)
{
    HI_S32 s32Ret;
	VDEC_CMD_SET_PROGRESSIVE_S stParam;
    if (HI_NULL == pParam)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = VFMW_INST_HANDLE(hInst);
	stParam.bProgressive = *pParam;
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_PROGRSSIVE, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d SetProgressive err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }	
    HI_INFO_VDEC("Chan %d SetProgressive OK\n", stParam.hHandle);
    return HI_SUCCESS;
	
}

 HI_S32 VDEC_SetDPBFullCtrl(HI_HANDLE hInst, HI_BOOL* pParam)
{
    HI_S32 s32Ret;
	VDEC_CMD_SET_DPBFULL_CTRL_S stParam;
    if (HI_NULL == pParam)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }

    stParam.hHandle = VFMW_INST_HANDLE(hInst);
	stParam.bDPBFullCtrl = *pParam;
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_DPBFULL, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d UMAPC_VDEC_CHAN_DPBFULL err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }	
    return HI_SUCCESS;
	
}

HI_S32 VDEC_SetLowDelay(HI_HANDLE hInst, HI_UNF_AVPLAY_LOW_DELAY_ATTR_S* pParam)
{
    HI_S32 s32Ret;
    VDEC_CMD_SET_LOWDELAY_S stParam;
    if (HI_NULL == pParam)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }
    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.bLowdelay= pParam->bEnable;
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_LOWDELAY, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d SetLowdelay err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }	
    HI_INFO_VDEC("Chan %d SetLowDelay OK\n", stParam.hHandle);
    return HI_SUCCESS;
    
}
HI_S32 VDEC_SetColorSpace(HI_HANDLE hInst,HI_UNF_COLOR_SPACE_E*pParam)
{
    HI_S32 s32Ret;
    VDEC_CMD_SET_COLORSPACE_S stParam;
    if (HI_NULL == pParam)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }
    stParam.hHandle = VFMW_INST_HANDLE(hInst);
    stParam.u32ColorSpace = *pParam;
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETCOLORSPACE, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d SetColorSpace err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }
    HI_INFO_VDEC("Chan %d SetColorSpace OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

#ifdef HI_TEE_SUPPORT
HI_S32 VDEC_SetTVP(HI_HANDLE hInst, HI_UNF_AVPLAY_TVP_ATTR_S* pParam)
{
    HI_S32 s32Ret;
    VDEC_CMD_SET_TVP_S stParam;
    if (HI_NULL == pParam)
    {
        return HI_ERR_CODEC_INVALIDPARAM;
    }
    stParam.hHandle = VFMW_INST_HANDLE(hInst);
	stParam.bTVP    = pParam->bEnable;
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_TVP, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d SetTvp err:%x!\n", stParam.hHandle, s32Ret);
        return HI_ERR_CODEC_OPERATEFAIL;
    }	
    HI_INFO_VDEC("Chan %d SetTvp OK\n", stParam.hHandle);
    return HI_SUCCESS;
}
#endif

static HI_S32 VFMW_Control(HI_HANDLE hInst, HI_U32 u32CMD, HI_VOID * pParam)
{
    switch (u32CMD)
    {
    case VFMW_CMD_CHECKEVT:
        return VFMW_CheckEvt(hInst, (VDEC_EVENT_S *)pParam);
    case VFMW_CMD_READNEWFRAME:
        return VFMW_ReadNewFrame(hInst, (HI_DRV_VIDEO_FRAME_S*)pParam);
    case VFMW_CMD_READUSRDATA:
        return VFMW_RecvUsrData(hInst, (HI_UNF_VIDEO_USERDATA_S*)pParam);
    case VFMW_CMD_SETFRAMERATE:
        return VFMW_SetFrmRate(hInst, (HI_UNF_AVPLAY_FRMRATE_PARAM_S*)pParam);
    case VFMW_CMD_GETFRAMERATE:
        return VFMW_GetFrmRate(hInst, (HI_UNF_AVPLAY_FRMRATE_PARAM_S*)pParam);
    case VFMW_CMD_GETSTATUSINFO:
        return VFMW_GetStatusInfo(hInst, (VDEC_STATUSINFO_S*)pParam);
    case VFMW_CMD_SETEOSFLAG:
        return VFMW_SetEosFlag(hInst);
    case VFMW_CMD_IFRAMEDECODE:
        return VFMW_IFrameDecode(hInst, (VFMW_IFRAME_PARAM_S*)pParam);
    case VFMW_CMD_IFRAMERELEASE:
        return VFMW_IFrameRelease(hInst, (HI_DRV_VIDEO_FRAME_S*)pParam);
    case VFMW_CMD_RECEIVEFRAME:
        return VFMW_RecvFrm(hInst, (HI_DRV_VIDEO_FRAME_S*)pParam);
    case VFMW_CMD_RELEASEFRAME:
        return VFMW_RlsFrm(hInst, (HI_DRV_VIDEO_FRAME_S*)pParam);
    case VFMW_CMD_ATTACHBUF:
        return VFMW_AttachBuf(hInst, (VFMW_STREAMBUF_S*)pParam);
    case VFMW_CMD_DETACHBUF:
        return VFMW_DetachBuf(hInst);
    case VFMW_CMD_DISCARDFRAME:
        return VFMW_DiscardFrame(hInst, (VDEC_DISCARD_FRAME_S*)pParam);
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    case VFMW_CMD_ACQUSERDATA:
        return VFMW_AcqUserData(hInst, (VFMW_USERDATA_S*)pParam);
    case VFMW_CMD_RLSUSERDATA:
        return VFMW_RlsUserData(hInst, (HI_UNF_VIDEO_USERDATA_S*)pParam);
#endif
    case VFMW_CMD_DROPSTREAM:
		return VFMW_DropStream(hInst,(VFMW_SEEKPTS_PARAM_S*) pParam);
    case VFMW_CMD_GETINFO:
        return VFMW_GetInfo(hInst, (HI_UNF_AVPLAY_VDEC_INFO_S*)pParam);
    case VFMW_CMD_SETTPLAYOPT:
        return VFMW_SetTPlayOpt(hInst, (HI_UNF_AVPLAY_TPLAY_OPT_S*)pParam);
    case VFMW_CMD_SETCTRLINFO:
        return VFMW_SetCtrlInfo(hInst, (HI_UNF_AVPLAY_CONTROL_INFO_S*)pParam);
	case VFMW_CMD_SET_PROGRESSIVE:
		return VFMW_SetProgressive(hInst, (HI_BOOL*)pParam);
    case VFMW_CMD_SETLOWDELAY:
        return VDEC_SetLowDelay(hInst, (HI_UNF_AVPLAY_LOW_DELAY_ATTR_S*)pParam);
    case VFMW_CMD_SET_DPBFULL_CTRL:
		return VDEC_SetDPBFullCtrl(hInst,(HI_BOOL*)pParam);
     case VFMW_CMD_SET_COLORSPACE:
            return VDEC_SetColorSpace(hInst,(HI_UNF_COLOR_SPACE_E*)pParam);
    default:
        return HI_ERR_CODEC_UNSUPPORT;
    }
}
//add by l00225186
HI_S32 VPSS_RecvFrm(HI_HANDLE hVpss, HI_DRV_VIDEO_FRAME_PACKAGE_S* pstFrameInfo)
{
	HI_S32 s32Ret;
    VDEC_CMD_VPSS_FRAME_S stParam = {0};

    if (HI_NULL == pstFrameInfo)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    stParam.hHandle = hVpss;//VPSS_INST_HANDLE(hInst);

    /* Ioctl UMAPC_VDEC_CHAN_RCVFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_RCVVPSSFRM, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d VPSS_RecvFrm err:%x!\n", stParam.hHandle, s32Ret);
        // return HI_ERR_CODEC_OPERATEFAIL;
        return HI_FAILURE;
    }

    /* BEGIN: Deleted by z00111416, 2013/7/19 入参使用局部变量赋值!!!!!*/
    //*pstFrameInfo = stParam.stFrame;
    /* END:   Deleted by z00111416, 2013/7/19 */
    memcpy(pstFrameInfo, &(stParam.stFrame), sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));
    HI_INFO_VDEC("Chan %d RecvVPSSFrm OK\n", stParam.hHandle);
    return s32Ret;
}

HI_S32 VPSS_ReleaseFrm(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pVideoFrame)
{
    HI_S32 s32Ret;
    VDEC_CMD_VPSS_FRAME_S stParam = {0};

    if (HI_NULL == pVideoFrame)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }
    stParam.hHandle = hPort;
    //stParam.pVideoFrame = pVideoFrame;
    memcpy(&(stParam.VideoFrame),pVideoFrame,sizeof(HI_DRV_VIDEO_FRAME_S));
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_RLSPORTFRM, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("release port %d frame err:%x!\n", stParam.hHandle, s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;	
}

//add by l00225186
HI_S32 VPSS_CreateVpss(HI_HANDLE hVdec,HI_HANDLE* phVpss)
{
   	HI_S32 s32Ret;
	VDEC_CMD_VPSS_FRAME_S stParam;
	if (HI_NULL == phVpss)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }
	stParam.hHandle = hVdec;
    /* Ioctl UMAPC_VDEC_CHAN_RCVFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_CREATEVPSS, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VPSS Create err:%d!\n", hVdec);
        return HI_FAILURE;
    }
    memcpy(phVpss, &stParam, sizeof(HI_HANDLE));
	return s32Ret; 
}
//add by l00225186
HI_S32 VPSS_DestoryVpss(HI_HANDLE hVdec,HI_HANDLE* phVpss)
{
   	HI_S32 s32Ret;
	VDEC_CMD_VPSS_FRAME_S stParam;
	if (HI_NULL == phVpss)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }
	stParam.hHandle = hVdec;
    /* Ioctl UMAPC_VDEC_CHAN_RCVFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_DESTORYVPSS, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VPSS Destory err:%d!\n", hVdec);
        return HI_FAILURE;
    }
	return s32Ret; 
}
//add by l00225186
HI_S32 VPSS_CreatePort(HI_HANDLE hVpss, VDEC_PORT_CFG_S* psVdecPortCfg)
{
	HI_S32 s32Ret;
	VDEC_CMD_VPSS_FRAME_S stParam;
	if (HI_NULL == psVdecPortCfg)
    {
        HI_ERR_VDEC("Bad param.\n");
        //return HI_ERR_CODEC_INVALIDPARAM;
        return HI_ERR_VDEC_INVALID_PARA;
    }
	stParam.hHandle = hVpss;
	stParam.ePortAbility = psVdecPortCfg->ePortAbility;
    /* Ioctl UMAPC_VDEC_CHAN_RCVFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_CREATEPORT, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VPSS_CreatePort err:%x!\n", hVpss, s32Ret);
        return HI_FAILURE;
    }
	*(psVdecPortCfg->phPort) = stParam.hPort;
	return s32Ret;
}
//add by l00225186
HI_S32 VPSS_DestoryPort(HI_HANDLE hVpss, HI_HANDLE* phPort)
{
    HI_S32 s32Ret;
	VDEC_CMD_VPSS_FRAME_S stParam;
	if (HI_NULL == phPort)
    {
        HI_ERR_VDEC("Bad param.\n");
        //return HI_ERR_CODEC_INVALIDPARAM;
        return HI_ERR_VDEC_INVALID_PARA;
    }
	//stParam.hHandle = VPSS_INST_HANDLE(hVdec);
	stParam.hHandle = hVpss;
	stParam.hPort  = *phPort;
    /* Ioctl UMAPC_VDEC_CHAN_RCVFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_DESTROYPORT, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VPSS_DestoryPort err:%x!\n", hVpss, s32Ret);
        // return HI_ERR_CODEC_OPERATEFAIL;
        return HI_FAILURE;
    }
	return s32Ret;
}
//add by l00225186
HI_S32 VPSS_EnablePort(HI_HANDLE hVpss, HI_HANDLE* phPort)
{
    HI_S32 s32Ret;
	VDEC_CMD_VPSS_FRAME_S stParam;
	if (HI_NULL == phPort)
    {
        HI_ERR_VDEC("Bad param.\n");
        //return HI_ERR_CODEC_INVALIDPARAM;
        return HI_ERR_VDEC_INVALID_PARA;
    }
	//stParam.hHandle = VPSS_INST_HANDLE(hVdec);
	stParam.hHandle = hVpss;
	stParam.hPort  = *phPort;
    /* Ioctl UMAPC_VDEC_CHAN_RCVFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_ENABLEPORT, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VPSS_EnablePort err:%x!\n", hVpss, s32Ret);
        // return HI_ERR_CODEC_OPERATEFAIL;
        return HI_FAILURE;
    }
	return s32Ret;
}
//add by l00225186
HI_S32 VPSS_DisablePort(HI_HANDLE hVpss, HI_HANDLE* phPort)
{
    HI_S32 s32Ret;
	VDEC_CMD_VPSS_FRAME_S stParam;
	if (HI_NULL == phPort)
    {
        HI_ERR_VDEC("Bad param.\n");
        //return HI_ERR_CODEC_INVALIDPARAM;
        return HI_ERR_VDEC_INVALID_PARA;
    }
	stParam.hHandle = hVpss;
	stParam.hPort  = *phPort;
    /* Ioctl UMAPC_VDEC_CHAN_RCVFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_DISABLEPORT, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VPSS_DisablePort err:%x!\n", hVpss, s32Ret);
        // return HI_ERR_CODEC_OPERATEFAIL;
        return HI_FAILURE;
    }
	return s32Ret;
}
//add by l00225186
HI_S32 VPSS_SetPortType(HI_HANDLE hVpss, VDEC_PORT_TYPE_WITHPORT_S* pstPortTypeWithPortHandle)
{
    HI_S32 s32Ret;
	VDEC_CMD_VPSS_FRAME_S stParam;
	if (HI_NULL == pstPortTypeWithPortHandle)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }
	stParam.hHandle = hVpss;
	stParam.hPort  = pstPortTypeWithPortHandle->hPort;
	stParam.enPortType = pstPortTypeWithPortHandle->enPortType;
    /* Ioctl UMAPC_VDEC_CHAN_RCVFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETPORTTYPE, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VPSS_SetPortType err:%x!\n", hVpss, s32Ret);
        return HI_FAILURE;
    }
	return s32Ret;
}
//add by l00225186
HI_S32 VPSS_CancleMainPort(HI_HANDLE hVpss, HI_HANDLE* phPort)
{
    HI_S32 s32Ret;
	VDEC_CMD_VPSS_FRAME_S stParam;
	if (HI_NULL == phPort)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }
	stParam.hHandle = hVpss;
	stParam.hPort  = *phPort;
    /* Ioctl UMAPC_VDEC_CHAN_RCVFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_CANCLEMAINPORT, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VPSS_EnablePort err:%x!\n", hVpss, s32Ret);
        return HI_FAILURE;
    }
	return s32Ret;
}
HI_S32 VPSS_SetChanFrmPackType(HI_HANDLE hVpss,HI_UNF_VIDEO_FRAME_PACKING_TYPE_E* pParam)
{
    HI_S32 s32Ret;
	VDEC_CMD_VPSS_FRAME_S stParam;
	if (HI_NULL == pParam)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }
	stParam.hHandle = hVpss;
	stParam.eFramePackType  = *pParam;
    /* Ioctl UMAPC_VDEC_CHAN_RCVFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETFRMPACKTYPE, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VPSS_SetChanFrmPackType err:%x!\n", hVpss, s32Ret);
        return HI_FAILURE;
    }
	return s32Ret;
}
HI_S32 VPSS_GetChanFrmPackType(HI_HANDLE hVpss,HI_UNF_VIDEO_FRAME_PACKING_TYPE_E* pParam)
{
    HI_S32 s32Ret;
	VDEC_CMD_VPSS_FRAME_S stParam;
	if (HI_NULL == pParam)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }
	stParam.hHandle = hVpss;
    /* Ioctl UMAPC_VDEC_CHAN_RCVFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_GETFRMPACKTYPE, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VPSS_GetChanFrmPackType err:%x!\n", hVpss, s32Ret);
        return HI_FAILURE;
    }
	*pParam = stParam.eFramePackType;
	return s32Ret;
}
HI_S32 VPSS_SendEos(HI_HANDLE hVdec)
{
	HI_S32 s32Ret;
	s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SENDEOS, &hVdec);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VPSS_SendEos err:%x!\n", hVdec, s32Ret);
        return HI_FAILURE;
    }
	return s32Ret;
}

HI_S32 VPSS_GetPortState(HI_HANDLE hVdec,HI_BOOL* bAllPortComplete)
{
	HI_S32 s32Ret;
	VDEC_CMD_VPSS_FRAME_S stParam;
	stParam.hHandle = hVdec;
	
	s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_GETPORTSTATE, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VPSS_GetPortState err:%x!\n", hVdec, s32Ret);
        return HI_FAILURE;
    }
	*bAllPortComplete = stParam.bAllPortComplete;
	return s32Ret;
}
HI_S32 VPSS_ResetVpss(HI_HANDLE hVpss)
{
    HI_S32 s32Ret;
	s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_RESETVPSS, &hVpss);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VPSS_ResetVpss err:%x!\n", hVpss, s32Ret);
        return HI_FAILURE;
    }
	return s32Ret;
}
HI_S32 VPSS_GetStatusInfo(HI_HANDLE hVdec,VDEC_FRMSTATUSINFOWITHPORT_S* pstVdecFrmStatusInfo)
{
	HI_S32 s32Ret;
	VDEC_CMD_VPSS_FRAME_S stParam;
	stParam.hHandle = hVdec;
	stParam.hPort = pstVdecFrmStatusInfo->hPort;
	s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_GETFRMSTATUSINFO, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VPSS_GetStatusInfo err:%x!\n", hVdec, s32Ret);
        return HI_FAILURE;
    }

    memcpy(&pstVdecFrmStatusInfo->stVdecFrmStatus,&stParam.stVdecFrmStatusInfo,sizeof(VDEC_FRMSTATUSINFO_S));

    return s32Ret;
}

HI_S32 VPSS_GetPortAttr(HI_HANDLE hVdec, VDEC_PORT_ATTR_WITHHANDLE_S *pstAttrWithHandle)
{
    HI_S32                  Ret;
    VDEC_CMD_VPSS_FRAME_S   stParam;

    memset(&stParam, 0x0, sizeof(VDEC_CMD_VPSS_FRAME_S));

    stParam.hHandle = hVdec;
    stParam.hPort = pstAttrWithHandle->hPort;

    Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_GETPORTATTR, &stParam);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_VDEC("Chan %d Get Port Attr ERR, Ret=%#x\n", hVdec, Ret);
        return Ret;
    }

    pstAttrWithHandle->stPortCfg = stParam.stPortCfg;

    return HI_SUCCESS;
}

HI_S32 VPSS_SetPortAttr(HI_HANDLE hVdec, VDEC_PORT_ATTR_WITHHANDLE_S *pstAttrWithHandle)
{
    HI_S32                  Ret;
    VDEC_CMD_VPSS_FRAME_S   stParam;

    memset(&stParam, 0x0, sizeof(VDEC_CMD_VPSS_FRAME_S));

    stParam.hHandle = hVdec;
    stParam.hPort = pstAttrWithHandle->hPort;
    stParam.stPortCfg = pstAttrWithHandle->stPortCfg;

    Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETPORTATTR, &stParam);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_VDEC("Chan %d Set Port Attr ERR, Ret=%#x\n", hVdec, Ret);
        return Ret;
    }

    return Ret;
}

HI_S32 VPSS_SetExtBuffer(HI_HANDLE handle,VDEC_BUFFER_ATTR_S *pstBufferAttr)
{
    HI_S32                  Ret;
    VDEC_CMD_VPSS_FRAME_S   stParam;
    stParam.hHandle = handle;
    memcpy(&stParam.stBufferAttr,pstBufferAttr,sizeof(VDEC_BUFFER_ATTR_S));
    Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETEXTBUFFER, &stParam);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_VDEC("Chan %d UMAPC_VDEC_CHAN_SETEXTBUFFER ERR, Ret=%#x\n", handle, Ret);
        return Ret;
    }
    return Ret;
}
HI_S32 VPSS_SetBufferMode(HI_HANDLE handle,VDEC_FRAMEBUFFER_MODE_E *penFrameBufferMode)
{
    HI_S32                  Ret;
    VDEC_CMD_SET_BUFFERMODE_S   stParam;
    stParam.hHandle = handle;
    stParam.enFrameBufferMode = *penFrameBufferMode;
    Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETBUFFERMODE, &stParam);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_VDEC("Chan %d UMAPC_VDEC_CHAN_SETBUFFERMODE ERR, Ret=%#x\n", handle, Ret);
        return Ret;
    }
	return HI_SUCCESS;
}
HI_S32 VPSS_CheckAndDelBuffer(HI_HANDLE handle,VDEC_BUFFER_INFO_S *pstBufInfo)
{
    HI_S32                  Ret;
    VDEC_CMD_CHECKANDDELBUFFER_S   stParam;
    stParam.hHandle = handle;
    //memcpy(&(stParam.stBufInfo),pstBufInfo,sizeof(VDEC_BUFFER_INFO_S));
    stParam.stBufInfo.u32PhyAddr = pstBufInfo->u32PhyAddr;
    stParam.stBufInfo.enBufState = pstBufInfo->enBufState;
    Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_CHECKANDDELBUFFER, &stParam);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_VDEC("Chan %d UMAPC_VDEC_CHAN_CHECKANDDELBUFFER ERR, Ret=%#x\n", handle, Ret);
        return Ret;
    }
	pstBufInfo->enBufState = stParam.stBufInfo.enBufState;
	return HI_SUCCESS;
}

HI_S32 VPSS_SetExtBufferState(HI_HANDLE handle,VDEC_EXTBUFFER_STATE_E *pEnExtBufferState)
{
    HI_S32 Ret;
    VDEC_CMD_SETEXTBUFFERTATE_S stParam;
    stParam.hHandle = handle;
	stParam.enExtBufferState = *pEnExtBufferState;
    Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETEXTBUFFERSTATE, &stParam);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_VDEC("Chan %d UMAPC_VDEC_CHAN_SETEXTBUFFERSTATE ERR, Ret=%#x\n", handle, Ret);
        return Ret;
    }
	return HI_SUCCESS;

}

HI_S32 VPSS_SetResolution(HI_HANDLE handle,VDEC_RESOLUTION_ATTR_S* pstResolution)
{
    HI_S32 Ret;
    VDEC_CMD_SETRESOLUTION_S stParam;
    stParam.hHandle = handle;
    memcpy(&(stParam.stResolution),pstResolution,sizeof(VDEC_RESOLUTION_ATTR_S));
    Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETRESOLUTION, &stParam);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_VDEC("Chan %d UMAPC_VDEC_CHAN_SETRESOLUTION ERR, Ret=%#x\n", handle, Ret);
        return Ret;
    }
	return HI_SUCCESS;  
}
//add by l00225186
HI_S32 VPSS_Control(HI_HANDLE handle, HI_U32 u32CMD, HI_VOID * pParam)
{
	switch (u32CMD)
    {
        case VPSS_CMD_CREATEVPSS:
            /*handle is hvdec*/
            return VPSS_CreateVpss(handle,(HI_HANDLE *)pParam);
		case VPSS_CMD_DESTORYVPSS:
			return VPSS_DestoryVpss(handle,(HI_HANDLE *)pParam);
    	case VPSS_CMD_RECEIVEFRAME:
    		return VPSS_RecvFrm(handle, (HI_DRV_VIDEO_FRAME_PACKAGE_S*)pParam);
    	case VPSS_CMD_CREATEPORT:
    		return VPSS_CreatePort(handle,(VDEC_PORT_CFG_S *)pParam);
    	case VPSS_CMD_DESTORYPORT:
    		return VPSS_DestoryPort(handle,(HI_HANDLE *)pParam);
		case VPSS_CMD_ENABLEPORT:
			return VPSS_EnablePort(handle,(HI_HANDLE *)pParam);
		case VPSS_CMD_DISABLEPORT:
			return VPSS_DisablePort(handle,(HI_HANDLE *)pParam);
		case VPSS_CMD_SETPORTTYPE:
			return VPSS_SetPortType(handle,(VDEC_PORT_TYPE_WITHPORT_S *)pParam);
		case VPSS_CMD_CANCLEMAINPORT:
			return VPSS_CancleMainPort(handle,(HI_HANDLE *)pParam);
		case VPSS_CMD_SETCHAN_FRMPACKTYPE:
			return VPSS_SetChanFrmPackType(handle,(HI_UNF_VIDEO_FRAME_PACKING_TYPE_E*)pParam);
	    case VPSS_CMD_GETCHAN_FRMPACKTYPE:
			return VPSS_GetChanFrmPackType(handle,(HI_UNF_VIDEO_FRAME_PACKING_TYPE_E*)pParam);
		case VPSS_CMD_RESETVPSS:
			return VPSS_ResetVpss(handle);
		case VPSS_CMD_GETSTATUSINFO:
			/*handle is hvdec*/
			return VPSS_GetStatusInfo(handle,(VDEC_FRMSTATUSINFOWITHPORT_S *)pParam);
		case VPSS_CMD_SENDEOS:
			return VPSS_SendEos(handle);
		case VPSS_CMD_GETPORTSTATE:
			return VPSS_GetPortState(handle,(HI_BOOL*)pParam);
		case VPSS_CMD_GETPORTATTR:
		    return VPSS_GetPortAttr(handle, (VDEC_PORT_ATTR_WITHHANDLE_S *)pParam);
		case VPSS_CMD_SETPORTATTR:
		    return VPSS_SetPortAttr(handle, (VDEC_PORT_ATTR_WITHHANDLE_S *)pParam);
		case VPSS_CMD_SETEXTBUFFER:
			return VPSS_SetExtBuffer(handle,(VDEC_BUFFER_ATTR_S *)pParam);
		case VPSS_CMD_SETBUFFERMODE:
			return VPSS_SetBufferMode(handle,(VDEC_FRAMEBUFFER_MODE_E *)pParam);
		case VPSS_CMD_CHECKANDDELBUFFER:
			return VPSS_CheckAndDelBuffer(handle,(VDEC_BUFFER_INFO_S *)pParam);
		case VPSS_CMD_SETEXTBUFFERSTATE:
			return VPSS_SetExtBufferState(handle,(VDEC_EXTBUFFER_STATE_E*)pParam);
	    case VPSS_CMD_SETRESOLUTION:
			return VPSS_SetResolution(handle,(VDEC_RESOLUTION_ATTR_S*)pParam);			
        default:
            return HI_ERR_CODEC_UNSUPPORT;    
	}
}
HI_CODEC_S* VDEC_VFMW_Codec(HI_VOID)
{
    return &s_stCodec;
}

HI_S32 VDEC_AllocHandle(HI_HANDLE *phHandle)
{
    HI_S32 s32Ret;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (HI_NULL == phHandle)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_ALLOCHANDLE, &hHandle);
    if (HI_SUCCESS == s32Ret)
    {
        *phHandle = hHandle;
    }
    return s32Ret;
}

HI_S32 VDEC_FreeHandle(HI_HANDLE hHandle)
{
    return ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_FREEHANDLE, &hHandle);
}

#ifndef HI_TEE_SUPPORT
HI_S32 VDEC_CreateStreamBuf(HI_HANDLE* phBuf, HI_U32 u32BufSize)
#else
HI_S32 VDEC_CreateStreamBuf(HI_HANDLE* phBuf, VDEC_BUFFER_ATTR_S *pstBufAttr)
#endif
{
    HI_S32 s32Ret;
    VDEC_CMD_CREATEBUF_S stBuf;
    VDEC_CMD_BUF_USERADDR_OLD_S stUserAddr;
    STREAM_BUF_INST_S* pstBufInst = HI_NULL;
#ifndef HI_TEE_SUPPORT
    if ((HI_NULL == phBuf) || (0 == u32BufSize))
#else
    if ((HI_NULL == phBuf) || (0 == pstBufAttr->u32BufSize))
#endif
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Alloc instance memory */
    pstBufInst = (STREAM_BUF_INST_S*)HI_MALLOC_VDEC(sizeof(STREAM_BUF_INST_S));
    if (HI_NULL == pstBufInst)
    {
        return HI_ERR_VDEC_MALLOC_FAILED;
    }
    memset(pstBufInst, 0, sizeof(STREAM_BUF_INST_S));
#ifdef HI_TEE_SUPPORT
    memset(&stUserAddr, 0, sizeof(VDEC_CMD_BUF_USERADDR_OLD_S));
    pstBufInst->u32Size = pstBufAttr->u32BufSize;
#else
    pstBufInst->u32Size = u32BufSize;
#endif
    pstBufInst->bGetPutFlag = HI_FALSE;
#if (HI_VDEC_REG_CODEC_SUPPORT == 1) || (HI_VDEC_MJPEG_SUPPORT == 1)
    pstBufInst->bRecvRlsFlag = HI_FALSE;
#endif

#ifndef HI_TEE_SUPPORT
    /* Create buffer manager */
    stBuf.u32Size = u32BufSize;
#else
    pstBufInst->bTvp = pstBufAttr->bTvp;

    /* Create buffer manager */
    stBuf.u32Size = pstBufAttr->u32BufSize;
    stBuf.bTvp = pstBufAttr->bTvp;
#endif

    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CREATE_ESBUF, &stBuf);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VDEC("UMAPC_VDEC_CREATE_ESBUF fail.\n");
        HI_FREE_VDEC(pstBufInst);
        return s32Ret;
    }

    /* Map MMZ */
#ifndef HI_TEE_SUPPORT
    stUserAddr.u32UserAddr = (HI_U32)HI_MEM_Map(stBuf.u32PhyAddr, u32BufSize);
#else
    if(HI_TRUE == pstBufAttr->bTvp)
    {
        stUserAddr.u32UserAddr = stBuf.u32PhyAddr; /*前提是安全内存物理地址和虚拟地址一致，否则有极大风险*/
    }
    else
    {
        stUserAddr.u32UserAddr = (HI_U32)HI_MEM_Map(stBuf.u32PhyAddr, pstBufAttr->u32BufSize);
    }        
#endif
    if (HI_NULL == stUserAddr.u32UserAddr)
    {
        HI_ERR_VDEC("HI_MMZ_Map fail.\n");
        (HI_VOID)ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_DESTROY_ESBUF, &stBuf.hHandle);
        HI_FREE_VDEC(pstBufInst);
        return s32Ret;
    }
    pstBufInst->pu8MMZVirAddr = (HI_U8*)stUserAddr.u32UserAddr;

    /* Told the user virtual to kernel */
    stUserAddr.hHandle = stBuf.hHandle;
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_SETUSERADDR_OLD, &stUserAddr);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VDEC("UMAPC_VDEC_SETUSERADDR fail.\n");
	#ifndef HI_TEE_SUPPORT
        (HI_VOID)HI_MEM_Unmap(pstBufInst->pu8MMZVirAddr);
	#else
        if(HI_TRUE == pstBufAttr->bTvp)
        {
        }
        else
        {
            (HI_VOID)HI_MEM_Unmap(pstBufInst->pu8MMZVirAddr);
        } 
	#endif 
        (HI_VOID)ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_DESTROY_ESBUF, &stBuf.hHandle);
        HI_FREE_VDEC(pstBufInst);
        return s32Ret;
    }

    /* Save buffer manager handle */
    pstBufInst->hBuf = *phBuf = stBuf.hHandle;

    /* Add instance to list */
    VDEC_LOCK(s_stStrmBufParam.stMutex);
    list_add_tail(&pstBufInst->stBufNode, &s_stStrmBufParam.stBufHead);
    VDEC_UNLOCK(s_stStrmBufParam.stMutex);

    return s32Ret;
}

HI_S32 VDEC_DestroyStreamBuf(HI_HANDLE hBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;
    STREAM_BUF_INST_S* pstBufInst = HI_NULL;
	
    STRMBUF_FIND_INST(hBuf, pstBufInst);
    if (HI_NULL == pstBufInst)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Free buffer memory */
    if (HI_NULL != pstBufInst->pu8MMZVirAddr)
    {
	#ifndef HI_TEE_SUPPORT
        s32Ret = HI_MEM_Unmap(pstBufInst->pu8MMZVirAddr);
	#else
	    if (HI_TRUE == pstBufInst->bTvp)
	    {
	    }
	    else
	    {
	    	s32Ret = HI_MEM_Unmap(pstBufInst->pu8MMZVirAddr);
	    }
	#endif
    }
    s32Ret |= ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_DESTROY_ESBUF, &hBuf);
    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_VDEC("Free memory err.\n");
    }

    /* Delete node and free memory */
	VDEC_LOCK(s_stStrmBufParam.stMutex);
    list_del(&pstBufInst->stBufNode);
    HI_FREE_VDEC(pstBufInst);
    VDEC_UNLOCK(s_stStrmBufParam.stMutex);

    return HI_SUCCESS;
}

HI_S32 VDEC_GetStreamBuf(HI_HANDLE hBuf, HI_U32 u32RequestSize, VDEC_ES_BUF_S *pstBuf)
{
    HI_S32 s32Ret;
    STREAM_BUF_INST_S* pstBufInst = HI_NULL;
    VDEC_CMD_BUF_S stBufParam = {0};

    if (HI_NULL == pstBuf)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    STRMBUF_FIND_INST(hBuf, pstBufInst);
    if (HI_NULL == pstBufInst)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* If get but not put, return last address */
    if (pstBufInst->bGetPutFlag)
    {
        memcpy(pstBuf, &(pstBufInst->stLastGet), sizeof(VDEC_ES_BUF_S));
        return HI_SUCCESS;
    }

    /* Get */
    stBufParam.hHandle = hBuf;
    stBufParam.stBuf.u32BufSize = u32RequestSize;
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_GETBUF, &stBufParam);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    /* Save */
	pstBuf->pu8Addr 		= (HI_U8*)(HI_SIZE_T)stBufParam.stBuf.u64Addr;
	pstBuf->u32PhyAddr		= stBufParam.stBuf.u32PhyAddr;
	pstBuf->u32BufSize		= stBufParam.stBuf.u32BufSize;
	pstBuf->u64Pts			= stBufParam.stBuf.u64Pts;
	pstBuf->bEndOfFrame 	= stBufParam.stBuf.bEndOfFrame;
	pstBuf->bDiscontinuous	= stBufParam.stBuf.bDiscontinuous;

    pstBufInst->stLastGet   = *pstBuf;
    pstBufInst->bGetPutFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 VDEC_PutStreamBuf(HI_HANDLE hBuf, const VDEC_ES_BUF_S *pstBuf)
{
    HI_S32 s32Ret;
    STREAM_BUF_INST_S* pstBufInst = HI_NULL;
    VDEC_CMD_BUF_S stBufParam;

    if ((HI_NULL == pstBuf) || (HI_NULL == ((HI_VOID *)pstBuf->pu8Addr)))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    STRMBUF_FIND_INST(hBuf, pstBufInst);
    if (HI_NULL == pstBufInst)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Support put continuously */
    if (!pstBufInst->bGetPutFlag)
    {
        return HI_SUCCESS;
    }

    /* Put */
    stBufParam.hHandle = hBuf;
    stBufParam.stBuf.u64Addr        = (HI_U64)(HI_SIZE_T)pstBuf->pu8Addr;
	stBufParam.stBuf.u32PhyAddr     = pstBuf->u32PhyAddr;
	stBufParam.stBuf.u32BufSize     = pstBuf->u32BufSize;
	stBufParam.stBuf.u64Pts         = pstBuf->u64Pts;
	stBufParam.stBuf.bEndOfFrame    = pstBuf->bEndOfFrame;
	stBufParam.stBuf.bDiscontinuous = pstBuf->bDiscontinuous;
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_PUTBUF, &stBufParam);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VDEC("Put err\n");
        return HI_FAILURE;
    }

    /* Set flag */
    pstBufInst->bGetPutFlag = HI_FALSE;
    return HI_SUCCESS;
}

HI_S32 VDEC_ResetStreamBuf(HI_HANDLE hBuf)
{
    STREAM_BUF_INST_S* pstBufInst = HI_NULL;

    STRMBUF_FIND_INST(hBuf, pstBufInst);
    if (HI_NULL == pstBufInst)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    pstBufInst->bGetPutFlag = HI_FALSE;
    memset(&(pstBufInst->stLastGet), 0, sizeof(pstBufInst->stLastGet));
    
#if (HI_VDEC_REG_CODEC_SUPPORT == 1) || (HI_VDEC_MJPEG_SUPPORT == 1)
    pstBufInst->bRecvRlsFlag = HI_FALSE;
    memset(&(pstBufInst->stLastRecv), 0, sizeof(pstBufInst->stLastRecv));
#endif

    return ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_RESET_ESBUF, &hBuf);
}

HI_S32 VDEC_GetStreamBufStatus(HI_HANDLE hBuf, HI_DRV_VDEC_STREAMBUF_STATUS_S* pstStatus)
{
    HI_S32 s32Ret;
    VDEC_CMD_BUF_STATUS_S stParam;

    stParam.hHandle = hBuf;
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_GET_ESBUF_STATUS, &stParam);
    if (HI_SUCCESS == s32Ret)
    {
        *pstStatus = stParam.stStatus;
    }
    return s32Ret;
}

#if (HI_VDEC_REG_CODEC_SUPPORT == 1) || (HI_VDEC_MJPEG_SUPPORT == 1)
HI_S32 VDEC_RecvStream(HI_HANDLE hBuf, VDEC_ES_BUF_S *pstBuf)
{
    HI_S32 s32Ret;
    STREAM_BUF_INST_S* pstBufInst = HI_NULL;
    VDEC_CMD_BUF_S stParam;

    if (HI_NULL == pstBuf)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    STRMBUF_FIND_INST(hBuf, pstBufInst);
    if (HI_NULL == pstBufInst)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* If request but not release, return last address */
    if (HI_TRUE == pstBufInst->bRecvRlsFlag)
    {
        memcpy((HI_VOID *)pstBuf, (const HI_VOID *)&pstBufInst->stLastRecv, sizeof(VDEC_ES_BUF_S));
        return HI_SUCCESS;
    }

    /* Request */
    stParam.hHandle = hBuf;
    memset((HI_VOID *)&(stParam.stBuf), 0, sizeof(VDEC_ES_BUF_S));
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_RCVBUF, &stParam);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    /* Save */
	pstBuf->pu8Addr         = (HI_U8*)(HI_SIZE_T)stParam.stBuf.u64Addr;
	pstBuf->u32PhyAddr      = stParam.stBuf.u32PhyAddr;
	pstBuf->u32BufSize      = stParam.stBuf.u32BufSize;
	pstBuf->u64Pts          = stParam.stBuf.u64Pts;
	pstBuf->bEndOfFrame     = stParam.stBuf.bEndOfFrame;
	pstBuf->bDiscontinuous  = stParam.stBuf.bDiscontinuous;
    pstBufInst->stLastRecv   = *pstBuf;
    pstBufInst->bRecvRlsFlag = HI_TRUE;
    return HI_SUCCESS;
}

HI_S32 VDEC_RlsStream(HI_HANDLE hBuf, const VDEC_ES_BUF_S *pstBuf)
{
    HI_S32 s32Ret;
    STREAM_BUF_INST_S* pstBufInst = HI_NULL;
    VDEC_CMD_BUF_S stParam;

    if ((HI_NULL == pstBuf) || (HI_NULL == ((HI_VOID *)pstBuf->pu8Addr)))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    STRMBUF_FIND_INST(hBuf, pstBufInst);
    if (HI_NULL == pstBufInst)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Support release continuously */
    if (!pstBufInst->bRecvRlsFlag)
    {
        return HI_SUCCESS;
    }

    /* Support release 0 */
    if (0 == pstBuf->u32BufSize)
    {
        return HI_SUCCESS;
    }

    /* Release */
    stParam.hHandle = hBuf;
    stParam.stBuf.u64Addr        = (HI_U64)(HI_SIZE_T)pstBuf->pu8Addr;
	stParam.stBuf.u32PhyAddr     = pstBuf->u32PhyAddr;
	stParam.stBuf.u32BufSize     = pstBuf->u32BufSize;
	stParam.stBuf.u64Pts         = pstBuf->u64Pts;
	stParam.stBuf.bEndOfFrame    = pstBuf->bEndOfFrame;
	stParam.stBuf.bDiscontinuous = pstBuf->bDiscontinuous;
    
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_RLSBUF, &stParam);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VDEC("Rls err\n");
        return HI_FAILURE;
    }

    /* Set flag */
    pstBufInst->bRecvRlsFlag = HI_FALSE;
    return HI_SUCCESS;
}

HI_S32 VDEC_CreateFrameBuf(HI_HANDLE *phBuf)
{
    HI_S32 s32Ret;
    FRAME_BUF_INST_S* pstBufInst = HI_NULL;
    VDEC_CMD_ALLOC_S stParam;
    VDEC_CMD_ATTR_OLD_S stFrmBufAttr = {0};

    if (HI_NULL == phBuf)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Alloc instance memory */
    pstBufInst = (FRAME_BUF_INST_S*)HI_MALLOC_VDEC(sizeof(FRAME_BUF_INST_S));
    if (HI_NULL == pstBufInst)
    {
        return HI_ERR_VDEC_MALLOC_FAILED;
    }
	else
	{
        memset(pstBufInst, 0, sizeof(FRAME_BUF_INST_S));
	}

    stParam.hHandle = *phBuf;
    stParam.stOpenOpt.enDecType  = HI_UNF_VCODEC_DEC_TYPE_NORMAL;
    stParam.stOpenOpt.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
    stParam.stOpenOpt.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_H264;
    stParam.u32DFSEnable = 0;//l00273086

    /* Ioctl UMAPC_VDEC_CHAN_ALLOC */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_ALLOC, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan alloc err:%x!\n", s32Ret);
        HI_FREE_VDEC(pstBufInst);
        return HI_ERR_VDEC_CREATECH_FAILED;
    }

    /* Output handle */
    memcpy(&stFrmBufAttr.hHandle, &stParam, sizeof(HI_HANDLE));
    if (HI_INVALID_HANDLE == stFrmBufAttr.hHandle)
    {
        HI_ERR_VDEC("hBuf err!\n");
	    HI_FREE_VDEC(pstBufInst);
        return HI_ERR_VDEC_CREATECH_FAILED;
    }

    /* Config type MJPEG, vfmw channel will be a frame buffer */
    stFrmBufAttr.stAttr.enType = HI_UNF_VCODEC_TYPE_MJPEG;
    stFrmBufAttr.stAttr.enMode = HI_UNF_VCODEC_MODE_NORMAL;
    stFrmBufAttr.stAttr.u32ErrCover = 100;
    stFrmBufAttr.stAttr.u32Priority = 15;
    stFrmBufAttr.stAttr.bOrderOutput = HI_TRUE;
    stFrmBufAttr.stAttr.s32CtrlOptions = 0;
    stFrmBufAttr.stAttr.pCodecContext = HI_NULL;

    /* Ioctl UMAPC_VDEC_CHAN_SETATTR */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_SETATTR_OLD, &stFrmBufAttr);
    s32Ret |= ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_START, &stFrmBufAttr.hHandle);
    if (HI_SUCCESS != s32Ret)
    {
        (HI_VOID)ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_FREE, &stFrmBufAttr.hHandle);
        HI_FREE_VDEC(pstBufInst);
        HI_ERR_VDEC("Chan %d SetAttr err:%x!\n", stFrmBufAttr.hHandle, s32Ret);
        return HI_ERR_VDEC_SETATTR_FAILED;
    }

    /* Save handle */
    pstBufInst->hBuf = *phBuf = stFrmBufAttr.hHandle;

    /* Add instance to list */
    VDEC_LOCK(s_stFrmBufParam.stMutex);
    list_add_tail(&pstBufInst->stBufNode, &s_stFrmBufParam.stBufHead);
    VDEC_UNLOCK(s_stFrmBufParam.stMutex);
    
    return HI_SUCCESS;
}

HI_S32 VDEC_DestroyFrameBuf(HI_HANDLE hBuf)
{
    HI_S32 s32Ret;
    FRAME_BUF_INST_S* pstBufInst = HI_NULL;
    FRMBUF_FIND_INST(hBuf, pstBufInst);
    if (HI_NULL == pstBufInst)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_STOP, &hBuf);
    s32Ret |= ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_FREE, &hBuf);
    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_VDEC("Free channel err.\n");
    }

    /* Delete node and free memory */
	VDEC_LOCK(s_stFrmBufParam.stMutex);
    list_del(&pstBufInst->stBufNode);
    HI_FREE_VDEC(pstBufInst);
    VDEC_UNLOCK(s_stFrmBufParam.stMutex);
    
    return HI_SUCCESS;
}

HI_S32 VDEC_GetFrameBuf(HI_HANDLE hBuf, HI_DRV_VDEC_FRAME_BUF_S* pstBuf)
{
    HI_S32 s32Ret;
    FRAME_BUF_INST_S* pstBufInst = HI_NULL;
    VDEC_CMD_GET_FRAME_S stParam;

    if (HI_NULL == pstBuf)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    FRMBUF_FIND_INST(hBuf, pstBufInst);
    if (HI_NULL == pstBufInst)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* If get but not put, return last address */
    if (pstBufInst->bGetPutFlag)
    {
        memcpy(pstBuf, &(pstBufInst->stLastGet), sizeof(HI_DRV_VDEC_FRAME_BUF_S));
        return HI_SUCCESS;
    }

    stParam.hHandle = hBuf;
    
    /* Ioctl UMAPC_VDEC_CHAN_GETFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_GETFRM, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        /*HI_WARN_VDEC("Frame buffer %d GET err:%x!\n", stParam.hHandle, s32Ret);*/
        return HI_FAILURE;
    }

    /* Save */
    pstBufInst->stLastGet   = *pstBuf = stParam.stFrame;
    pstBufInst->bGetPutFlag = HI_TRUE;

    HI_INFO_VDEC("Frame buffer %d GET OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

HI_S32 VDEC_PutFrameBuf(HI_HANDLE hBuf, const HI_DRV_VDEC_USR_FRAME_S* pstBuf)
{
    HI_S32 s32Ret;
    FRAME_BUF_INST_S* pstBufInst = HI_NULL;
    VDEC_CMD_PUT_FRAME_S stParam;

    if (HI_NULL == pstBuf)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    FRMBUF_FIND_INST(hBuf, pstBufInst);
    if (HI_NULL == pstBufInst)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Support put continuously */
    if (!pstBufInst->bGetPutFlag)
    {
        return HI_SUCCESS;
    }

    stParam.hHandle = hBuf;
    stParam.stFrame = *pstBuf;
    
    /* Ioctl UMAPC_VDEC_CHAN_PUTFRM */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_PUTFRM, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Frame buffer %d PUT err:%x!\n", stParam.hHandle, s32Ret);
        return HI_FAILURE;
    }

    /* Save */
    pstBufInst->bGetPutFlag = HI_FALSE;

    HI_INFO_VDEC("Frame buffer %d PUT OK\n", stParam.hHandle);
    return HI_SUCCESS;
}

HI_S32 VDEC_ResetFrameBuf(HI_HANDLE hBuf)
{
    HI_S32 s32Ret;
    VDEC_CMD_RESET_S stParam;

    /* Set parameter */
    stParam.hHandle = hBuf;
    stParam.enType = VDEC_RESET_TYPE_ALL;

    /* Ioctl UMAPC_VDEC_CHAN_RESET */
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_STOP, &hBuf);
    s32Ret |= ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_RESET, &stParam);
    s32Ret |= ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_START, &hBuf);
    return s32Ret;
}

HI_S32 VDEC_GetFrameBufStatus(HI_HANDLE hBuf, HI_DRV_VDEC_FRAMEBUF_STATUS_S* pstStatus)
{
    HI_S32 s32Ret;
    VDEC_CMD_STATUS_S stParam;

    if (HI_NULL == pstStatus)
    {
        HI_ERR_VDEC("Bad param.\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Ioctl UMAPC_VDEC_CHAN_STATUSINFO */
    stParam.hHandle = hBuf;
    s32Ret = ioctl(s_stVdecAdpParam.s32DevFd, UMAPC_VDEC_CHAN_STATUSINFO, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
//        HI_ERR_VDEC("Chan %d GetStatusInfo err:%x!\n", stParam.hHandle, s32Ret);
        return HI_FAILURE;
    }

    pstStatus->u32TotalDecFrameNum = stParam.stStatus.u32TotalDecFrmNum;
    pstStatus->u32FrameBufNum = stParam.stStatus.u32FrameBufNum;
	pstStatus->bAllPortCompleteFrm = stParam.stStatus.bAllPortCompleteFrm;
    return HI_SUCCESS;
}

HI_S32 VDEC_GetNewFrm(HI_HANDLE hBuf, HI_DRV_VIDEO_FRAME_S* pstFrm)
{
    if (HI_SUCCESS != VFMW_ReadNewFrame(hBuf, pstFrm))
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 VDEC_SetFrmRate(HI_HANDLE hBuf, const HI_UNF_AVPLAY_FRMRATE_PARAM_S* pstFrmRate)
{
    if (HI_SUCCESS != VFMW_SetFrmRate(hBuf, pstFrmRate))
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 VDEC_GetFrmRate(HI_HANDLE hBuf, HI_UNF_AVPLAY_FRMRATE_PARAM_S* pstFrmRate)
{
    if (HI_SUCCESS != VFMW_GetFrmRate(hBuf, pstFrmRate))
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}
#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */
