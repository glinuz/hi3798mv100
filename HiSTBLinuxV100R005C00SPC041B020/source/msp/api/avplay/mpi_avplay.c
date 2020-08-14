/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_avplay.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/21
  Description   :
  History       :
  1.Date        : 2009/12/21
    Author      : w58735
    Modification: Created file

 *******************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <signal.h>

#include "hi_common.h"
#include "hi_unf_pmoc.h"
#include "hi_mpi_stat.h"
#include "hi_mpi_adec.h"
#include "hi_mpi_vdec.h"
#include "hi_mpi_demux.h"
#include "hi_mpi_sync.h"
#include "hi_mpi_win.h"
#include "hi_mpi_ao.h"
#include "hi_mpi_avplay.h"
#include "hi_error_mpi.h"
#include "hi_mpi_mem.h"
#include "hi_module.h"
#include "hi_drv_struct.h"
#include "avplay_frc.h"
#include "frame_detect.h"
#include "hi_video_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define  HI_FATAL_AVPLAY(fmt...)     HI_FATAL_PRINT(HI_ID_AVPLAY, fmt)
#define  HI_ERR_AVPLAY(fmt...)       HI_ERR_PRINT(HI_ID_AVPLAY, fmt)
#define  HI_WARN_AVPLAY(fmt...)      HI_WARN_PRINT(HI_ID_AVPLAY, fmt)
#define  HI_INFO_AVPLAY(fmt...)      HI_INFO_PRINT(HI_ID_AVPLAY, fmt)

#define  AVPLAY_AUD_SPEED_ADJUST_SUPPORT

#define  GET_AVPLAY_HANDLE(id)   ((HI_ID_AVPLAY << 16) | (id))
#define  GET_AVPLAY_ID(handle)   ((handle) & 0xFF)
#define  GET_SYNC_ID(handle)     ((handle) & 0xFF)

#define  DEMUX_INVALID_PID       0x1FFF

#define  AVPLAY_MAX_NUM                 16
#define  AVPLAY_MAX_WIN                 6
#define  AVPLAY_MAX_DMX_AUD_CHAN_NUM    32
#define  AVPLAY_MAX_TRACK               6

#define  AVPLAY_MAX_PORT_NUM            3    //The max num of port
#define  AVPLAY_MAX_SLAVE_FRMCHAN       2    //The max num of slave port
#define  AVPLAY_MAX_VIR_FRMCHAN         2    //The max num of virtual port

#define  AVPLAY_DFT_VID_SIZE       (5*1024*1024)
#define  AVPLAY_MIN_VID_SIZE       (0)
#define  AVPLAY_MAX_VID_SIZE       (64*1024*1024)

#define  AVPLAY_TS_DFT_AUD_SIZE    (384*1024)
#define  AVPLAY_ES_DFT_AUD_SIZE    (256*1024)
#define  AVPLAY_MIN_AUD_SIZE       (1*1024)
#define  AVPLAY_MAX_AUD_SIZE       (4*1024*1024)

#define  AVPLAY_ADEC_FRAME_NUM     (8)

#define  AVPLAY_SYS_SLEEP_TIME     (10)

#define  APPLAY_EOS_BUF_MIN_LEN    (1024)
#define  AVPLAY_EOS_TIMEOUT        (1000)
#define  APPLAY_EOS_STREAM_THRESHOLD (2)


/* video buffer dither waterline */
/* CNcomment: 视频缓冲管理抖动水线的百分比，0-99 */
#define  AVPLAY_ES_VID_FULL_PERCENT    85
#define  AVPLAY_ES_VID_HIGH_PERCENT    70
#define  AVPLAY_ES_VID_LOW_PERCENT     30
#define  AVPLAY_ES_VID_EMPTY_PERCENT   10

/* audio buffer dither waterline */
/* CNcomment: 音频缓冲管理抖动水线的百分比，0-99 */
#define  AVPLAY_ES_AUD_FULL_PERCENT    98
#define  AVPLAY_ES_AUD_HIGH_PERCENT    85
#define  AVPLAY_ES_AUD_LOW_PERCENT     5
#define  AVPLAY_ES_AUD_EMPTY_PERCENT   2

/* audio buffer waterline on PreStart mode*/
#define  AVPLAY_AUD_PREBUF_THRESHOLD   40

/* video buffer waterline on PreStart mode*/
#define  AVPLAY_VID_PREBUF_THRESHOLD   50

/* max delay time of adec in buffer */
#define  AVPLAY_ADEC_MAX_DELAY         1200

#define  AVPLAY_THREAD_TIMEOUT         30

#define  AVPLAY_VDEC_SEEKPTS_THRESHOLD 5000

#define  AVPLAY_RECVFRAME_MAX_TIME     10

#define  AVPLAY_VIDTHREAD_SLEEP_TIME   1

#define  HI_AVPLAY_LOCK()        (HI_VOID)pthread_mutex_lock(&g_AvplayMutex)
#define  HI_AVPLAY_UNLOCK()      (HI_VOID)pthread_mutex_unlock(&g_AvplayMutex)

#define  AVPLAY_INST_LOCK(id)    (HI_VOID)pthread_mutex_lock(&g_Avplay[id].Mutex)
#define  AVPLAY_INST_UNLOCK(id)  (HI_VOID)pthread_mutex_unlock(&g_Avplay[id].Mutex)

#define  AVPLAY_INST_LOCK_CHECK(handle, id) \
    do \
    { \
        if (id >= AVPLAY_MAX_NUM) \
        { \
            HI_ERR_AVPLAY("avplay %u error\n", id); \
            return HI_ERR_AVPLAY_INVALID_PARA; \
        } \
        AVPLAY_INST_LOCK(id); \
        pAvplay = g_Avplay[id].Avplay; \
        if (HI_NULL == pAvplay) \
        { \
            AVPLAY_INST_UNLOCK(id); \
            HI_ERR_AVPLAY("avplay is null\n"); \
            return HI_ERR_AVPLAY_INVALID_PARA; \
        } \
        if (pAvplay->hAvplay != handle) \
        { \
            AVPLAY_INST_UNLOCK(id); \
            HI_ERR_AVPLAY("avplay handle 0x%x, 0x%x error\n", handle, pAvplay->hAvplay); \
            return HI_ERR_AVPLAY_INVALID_PARA; \
        } \
    } while (0)

#define  AVPLAY_HDR_CHN_CHECK(HDRChn) \
    do \
    {  \
        switch ((HI_U32)HDRChn) \
        {\
            case AVPLAY_HDR_CHAN_BL : \
            case AVPLAY_HDR_CHAN_EL : \
            case AVPLAY_HDR_CHAN_BL | AVPLAY_HDR_CHAN_EL : \
                break; \
            default :\
                HI_ERR_AVPLAY("para HDRChn 0x%x is invalid\n", HDRChn); \
                return HI_FAILURE; \
        } \
    } while (0)

#if    defined(CHIP_TYPE_hi3798cv200) \
    || defined(CHIP_TYPE_hi3798mv200) \
    || defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) \
    || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3798mv200_a)

    #define AVPLAY_FRAME_RATE_ADJUST_SUPPORT
#endif

#if    !defined(CHIP_TYPE_hi3716mv410) \
    && !defined(CHIP_TYPE_hi3716mv420) \
    && !defined(CHIP_TYPE_hi3798cv200) \
    && !defined(CHIP_TYPE_hi3798mv200) \
    && !defined(CHIP_TYPE_hi3798mv200_a)

    #define AVPLAY_HOMOLOGOUS_WINDOW_SUPPORT
#endif

typedef enum
{
    AVPLAY_PROC_ADEC_AO,
    AVPLAY_PROC_DMX_ADEC,
    AVPLAY_PROC_VDEC_VO,
    AVPLAY_PROC_VDEC_AD_ADEC,
    AVPLAY_PROC_BUTT
}AVPLAY_PROC_ID_E;

typedef struct
{
    HI_HANDLE   hWindow;
    HI_HANDLE   hPort;
}AVPLAY_VID_PORT_AND_WIN_S;

typedef struct
{
    HI_U32      SendNum;
    HI_U32      PlayNum;
    HI_U32      RepeatNum;
    HI_U32      DiscardNum;
}AVPLAY_VIDFRM_STAT_S;

typedef enum
{
    AVPLAY_HDR_SYNC_PROC_DISCARD,
    AVPLAY_HDR_SYNC_PROC_REPEAT,
    AVPLAY_HDR_SYNC_PROC_PLAY,
    AVPLAY_HDR_SYNC_PROC_CONTINUE,
    AVPLAY_HDR_SYNC_PROC_BUTT
}AVPLAY_HDR_SYNC_PROC_E;

typedef enum
{
    AVPLAY_HDR_CHAN_BL = 0x01,
    AVPLAY_HDR_CHAN_EL = 0x02,
    AVPLAY_HDR_CHAN_BUTT
}AVPLAY_HDR_CHAN_E;

typedef struct
{
    HI_U32                     AcquireAudEsNum;
    HI_U32                     AcquiredAudEsNum;
    HI_U32                     SendAudEsNum;
    HI_U32                     SendedAudEsNum;

    HI_U32                     AcquireAudFrameNum;
    HI_U32                     AcquiredAudFrameNum;
    HI_U32                     SendAudFrameNum;
    HI_U32                     SendedAudFrameNum;

    HI_U32                     AcquireVidFrameNum;
    HI_U32                     AcquiredVidFrameNum;
    AVPLAY_VIDFRM_STAT_S       MasterVidStat;
    AVPLAY_VIDFRM_STAT_S       SlaveVidStat[AVPLAY_MAX_SLAVE_FRMCHAN];
    AVPLAY_VIDFRM_STAT_S       VirVidStat[AVPLAY_MAX_VIR_FRMCHAN];

    HI_U32                     VidOverflowNum;
    HI_U32                     AudOverflowNum;

    HI_U32                     VidThreadScheTimeOutCnt;
    HI_U32                     AudThreadScheTimeOutCnt;

    HI_U32                     VidThreadExeTimeOutCnt;
    HI_U32                     AudThreadExeTimeOutCnt;
    HI_U32                     CpuFreqScheTimeCnt;

    HI_U32                     AcquireAudADEsNum;
    HI_U32                     AcquiredAudADEsNum;
    HI_U32                     SendAudADEsNum;
    HI_U32                     SendedAudADEsNum;

    HI_BOOL                    bAoBufEmpty;
    HI_BOOL                    bEndOfFrame;
    HI_U32                     UsedBufNum;

    HI_BOOL                    bEndOfStream;
    HI_BOOL                    bAllPortCompleteFrm;
    HI_U32                     FrameNumInBufQn;
}AVPLAY_DEBUG_INFO_S;

typedef enum
{
    THREAD_PRIO_REALTIME,    /*Realtime thread, only 1 permitted*/
    THREAD_PRIO_HIGH,
    THREAD_PRIO_MID,
    THREAD_PRIO_LOW,
    THREAD_PRIO_BUTT
} THREAD_PRIO_E;

typedef struct
{
    HI_UNF_AVPLAY_ATTR_S            AvplayAttr;
    HI_UNF_VCODEC_ATTR_S            VdecAttr;
    HI_UNF_AVPLAY_LOW_DELAY_ATTR_S  LowDelayAttr;

#ifdef HI_TEE_SUPPORT
    HI_UNF_AVPLAY_TVP_ATTR_S        TVPAttr;
#endif

    HI_U32                          AdecType;

    HI_HANDLE                       hAvplay;
    HI_HANDLE                       hVdec;
    HI_HANDLE                       hAdec;
    HI_HANDLE                       hSync;
    HI_HANDLE                       hDmxPcr;
    HI_HANDLE                       hDmxVid;
    HI_HANDLE                       hDmxAud[AVPLAY_MAX_DMX_AUD_CHAN_NUM];

    HI_U32                          DmxPcrPid;
    HI_U32                          DmxVidPid;
    HI_U32                          DmxAudPid[AVPLAY_MAX_DMX_AUD_CHAN_NUM];

    /*multi audio demux channel*/
    HI_U32                          CurDmxAudChn;
    HI_U32                          DmxAudChnNum;
    HI_UNF_ACODEC_ATTR_S            *pstAcodecAttr;

    HI_HANDLE                       hSharedOrgWin;  /*Original window of homologous*/

    /*multi video frame channel*/
    AVPLAY_VID_PORT_AND_WIN_S       MasterFrmChn;
    AVPLAY_VID_PORT_AND_WIN_S       SlaveFrmChn[AVPLAY_MAX_SLAVE_FRMCHAN];
    AVPLAY_VID_PORT_AND_WIN_S       VirFrmChn[AVPLAY_MAX_VIR_FRMCHAN];

    /*multi audio track channel*/
    HI_HANDLE                       hSyncTrack;
    HI_HANDLE                       hTrack[AVPLAY_MAX_TRACK];
    HI_U32                          TrackNum;

    /*frc parameters*/
    HI_BOOL                         bFrcEnable;
    AVPLAY_FRC_CFG_S                FrcParamCfg;        /* config frc param */ /*CNcomment: 配置的frc参数 */
    AVPLAY_ALG_FRC_S                FrcCalAlg;          /* frc used rate info */ /*CNcomment: frc正在使用的帧率信息 */
    AVPLAY_FRC_CTRL_S               FrcCtrlInfo;        /* frc control */ /*CNcomment: frc控制信息 */
    HI_U32                          FrcNeedPlayCnt;     /* this frame need to play time*/ /*CNcomment:该帧需要播几次 */
    HI_U32                          FrcCurPlayCnt;      /* this frame had played time*/   /*CNcomment:该帧实际播到第几次*/

    /*flush stream control*/
    HI_BOOL                         bSetEosFlag;
    HI_BOOL                         bSetAudEos;

    /*ddp test*/
    HI_BOOL                         AudDDPMode;
    HI_U32                          LastAudPts;

#ifndef __aarch64__
    HI_UNF_AVPLAY_EVENT_CB_FN       EvtCbFunc[HI_UNF_AVPLAY_EVENT_BUTT];
#endif

    HI_BOOL                         EvtCbFunc64Flag;
    HI_UNF_AVPLAY_EVENT_CB_FN64     EvtCbFunc64[HI_UNF_AVPLAY_EVENT_BUTT];

    /*play control parameters*/
    HI_BOOL                         bProcFrmToVirtualWin;                            /*whether this frame send virtual window*/
    HI_BOOL                         bSendedFrmToMasterWin;                           /*whether this frame has send to Master window*/
    HI_BOOL                         bSendedFrmToSlaveWin[AVPLAY_MAX_SLAVE_FRMCHAN];  /*whether this frame has send to Slave window*/
    HI_BOOL                         bSendedFrmToVirtualWin[AVPLAY_MAX_VIR_FRMCHAN];  /*whether this frame has send to virtual window*/
    HI_BOOL                         VidEnable;
    HI_BOOL                         AudEnable;
    HI_BOOL                         bVidPreEnable;
    HI_BOOL                         bAudPreEnable;
    HI_U32                          VidPreBufThreshold;
    HI_U32                          AudPreBufThreshold;
    HI_U32                          VidPreSysTime;
    HI_U32                          AudPreSysTime;
    HI_U32                          PreAudPts;
    HI_U32                          PreVidPts;
    HI_U32                          PreAudWaterLine;
    HI_U32                          PreVidWaterLine;
    HI_U32                          PreVidFirstWaterLine;
    HI_U32                          PreVidSecondWaterLine;
    HI_U32                          PreVidBufLen;

    HI_UNF_AVPLAY_STATUS_E          LstStatus;                   /* last avplay status */
    HI_UNF_AVPLAY_STATUS_E          CurStatus;                   /* current avplay status */
    HI_UNF_AVPLAY_OVERFLOW_E        OverflowProc;
    HI_BOOL                         AvplayProcAudContinue;          /*flag for aud thread continue*/
    HI_BOOL                         AvplayProcVidContinue;          /*flag for vid thread continue*/

    HI_BOOL                         AvplayProcDataFlag[AVPLAY_PROC_BUTT];

    HI_UNF_STREAM_BUF_S             AvplayAudEsBuf;      /*adec buffer in es mode*/
    HI_UNF_ES_BUF_S                 AvplayDmxEsBuf;      /*audio denux buffer in ts mode*/
    HI_UNF_AO_FRAMEINFO_S           AvplayAudFrm;        /*audio frames get form adec*/
    SYNC_AUD_INFO_S                 AudInfo;
    SYNC_AUD_OPT_S                  AudOpt;

    VDEC_ES_BUF_S                   AvplayVidEsBuf;      /*vdec buffer in es mode*/
    HI_DRV_VIDEO_FRAME_PACKAGE_S    CurFrmPack;
    HI_DRV_VIDEO_FRAME_PACKAGE_S    LstFrmPack;
    SYNC_VID_INFO_S                 VidInfo;
    SYNC_VID_OPT_S                  VidOpt;

    HI_DRV_VDEC_FRAME_S             stIFrame;

    HI_BOOL                         bStepMode;
    HI_BOOL                         bStepPlay;

    AVPLAY_DEBUG_INFO_S             DebugInfo;

    HI_U32                          PreAudEsBuf;         /*audio es buffer size when EOS happens*/
    HI_U32                          PreVidEsBuf;         /*video es buffer size when EOS happens*/
    HI_U32                          PreSystime;          /*system time when EOS happens*/
    HI_U32                          PreVidEsBufWPtr;     /*position of the video es buffer write pointer*/
    HI_U32                          PreAudEsBufWPtr;     /*position of the audio es buffer write pointer*/
    HI_U32                          PreTscnt;            /*ts count when EOS happens*/
    HI_BOOL                         CurBufferEmptyState; /*current buffer state is empty or not*/

    HI_UNF_AVPLAY_BUF_STATE_E       PreVidBufState;     /*the status of video es buffer when CheckBuf*/
    HI_UNF_AVPLAY_BUF_STATE_E       PreAudBufState;     /*the status of audio es buffer when CheckBuf*/
    HI_BOOL                         VidDiscard;

    HI_U32                          u32ResumeCount;     /*Resume times*/
    HI_BOOL                         bSetResumeCnt;      /*is set resume count*/

    HI_U32                          AdecDelayMs;            /*How many mseconds in ADEC buffer*/
    ADEC_SzNameINFO_S               AdecNameInfo;

    HI_U32                          u32DispOptimizeFlag;    /*this is for pvr smooth tplay*/

    HI_U32                          ThreadID;
    HI_BOOL                         AvplayThreadRun;

    pthread_t                       AvplayAudDataThdInst;   /* run handle of avplay thread */
    pthread_t                       AvplayVidDataThdInst;   /* run handle of avplay thread */
    pthread_t                       AvplayStatThdInst;      /* run handle of avplay thread */
    pthread_mutex_t                 AvplayAudThreadMutex;   /*mutex for data safety use*/
    pthread_mutex_t                 AvplayVidThreadMutex;   /*mutex for data safety use*/

    HI_U32                          AoUnloadTime;
    HI_U32                          WinUnloadTime;
    HI_U32                          AudThreadScheTimeOut;
    HI_U32                          VidThreadScheTimeOut;

    HI_PROC_ENTRY_S                 Proc;

#ifdef HI_HDR_SUPPORT
    HI_UNF_AVPLAY_OPEN_OPT_S        AvplayChnOpt;
    HI_BOOL                         HDRSyncTargetInit;
    HI_U32                          HDRSyncTargetTime;
    AVPLAY_HDR_CHAN_E               HDRSyncTarget;
    AVPLAY_HDR_SYNC_PROC_E          ElSyncProc;
    AVPLAY_HDR_SYNC_PROC_E          BlSyncProc;
    HI_BOOL                         bHDRPlay;
    HI_BOOL                         bDualChnPlay;
    HI_BOOL                         bBlFrmProcFlag;
    HI_BOOL                         bElFrmProcFlag;
    HI_U32                          CurBlPts;
    HI_U32                          CurElPts;
    HI_UNF_AVPLAY_HDR_ATTR_S        AvplayHDRAttr;
    HI_HANDLE                       hElVdec;
    HI_HANDLE                       hElPort;
    HI_HANDLE                       hBlPort;
    VDEC_HDR_ATTR_S                 VdecBlAtrr;
    VDEC_HDR_ATTR_S                 VdecElAtrr;
    HI_DRV_VIDEO_FRAME_PACKAGE_S    CurBlFrmPack;
    HI_DRV_VIDEO_FRAME_PACKAGE_S    LstBlFrmPack;
    HI_DRV_VIDEO_FRAME_PACKAGE_S    CurElFrmPack;
    HI_DRV_VIDEO_FRAME_PACKAGE_S    LstElFrmPack;
    VDEC_ES_BUF_S                   AvplayBlEsBuf;
    VDEC_ES_BUF_S                   AvplayElEsBuf;
#endif
    HI_UNF_AVPLAY_AD_ATTR_S         AdAttr;
    HI_HANDLE                       hDmxAdAud;
    HI_UNF_STREAM_BUF_S             AvplayAdAudEsBuf;
    HI_UNF_ES_BUF_S                 AvplayAdDmxEsBuf;

    HI_BOOL                         bBoostCpuFreq;

    HI_HANDLE                       hDFCtx;

    HI_U32                          AdecOutBufNum;

    HI_U32                          VidFirstFrameDispTime;
    HI_U32                          VidFirstQueueFrameTime;
    HI_BOOL                         bVidFirstQueueFrame;
    HI_BOOL                         bVidFirstDispFrame;

    HI_UNF_DMX_SECURE_MODE_E        DmxAudChnSecMode;
} AVPLAY_S;

typedef struct
{
    AVPLAY_S           *Avplay;
    pthread_mutex_t     Mutex;
} AVPLAY_INFO_S;

#ifndef HI_ADVCA_FUNCTION_RELEASE
static const HI_CHAR s_AVPLAYVersion[] __attribute__((used)) = USER_VERSION_STRING;
#endif

static AVPLAY_INFO_S    g_Avplay[AVPLAY_MAX_NUM] = {{HI_NULL, PTHREAD_MUTEX_INITIALIZER}};
static HI_U8            g_StartCode[] = {0, 0, 1};
static HI_U32           g_AvplayInit = 0;
static pthread_mutex_t  g_AvplayMutex = PTHREAD_MUTEX_INITIALIZER;

#ifndef HI_ADVCA_FUNCTION_RELEASE
static HI_CHAR *g_AvplayVidType[HI_UNF_VCODEC_TYPE_BUTT] =
{
    "MPEG2",   "MPEG4",      "AVS",        "H263",      "H264",      "REAL8",    "REAL9",   "VC1",
    "VP6",     "VP6F",       "VP6A",       "MJPEG",     "SORENSON",  "DIVX3",    "RAW",     "JPEG",
    "VP8",     "MSMPEG4V1",  "MSMPEG4V2",  "MSVIDEO1",  "WMV1",      "WMV2",     "RV10",    "RV20",
    "SVQ1",    "SVQ3",       "H261",       "VP3",       "VP5",       "CINEPAK",  "INDEO2",  "INDEO3",
    "INDEO4",  "INDEO5",     "MJPEGB",     "MVC",       "HEVC",      "DV",       "VP9"
};
#endif

static HI_VOID AVPLAY_DRV2UNF_VidFrm(HI_DRV_VIDEO_FRAME_S *pstDRVFrm, HI_UNF_VIDEO_FRAME_INFO_S *pstUNFFrm);

HI_S32 AVPLAY_CreatePort(AVPLAY_S *pAvplay, HI_HANDLE hWin, VDEC_PORT_ABILITY_E enAbility, HI_HANDLE *phPort);
HI_S32 AVPLAY_DestroyPort(AVPLAY_S *pAvplay, HI_HANDLE hWin, HI_HANDLE hPort);
HI_S32 AVPLAY_SetPortAttr(AVPLAY_S *pAvplay, HI_HANDLE hPort, VDEC_PORT_TYPE_E enType);

#ifdef HI_HDR_SUPPORT
static HI_S32 AVPLAY_GetBlFramePort(AVPLAY_S *pAvplay, HI_HANDLE hPort, HI_U32 *PortId);
static HI_S32 AVPLAY_GetElFramePort(AVPLAY_S *pAvplay, HI_HANDLE hPort, HI_U32 *PortId);
#endif

void AVPLAY_ThreadMutex_Lock(pthread_mutex_t *ss)
{
    (HI_VOID)pthread_mutex_lock(ss);
}

void AVPLAY_ThreadMutex_UnLock(pthread_mutex_t *ss)
{
    (HI_VOID)pthread_mutex_unlock(ss);
}

void AVPLAY_Mutex_Lock(pthread_mutex_t *ss)
{
    (HI_VOID)pthread_mutex_lock(ss);
}

void AVPLAY_Mutex_UnLock(pthread_mutex_t *ss)
{
    (HI_VOID)pthread_mutex_unlock(ss);
}

extern HI_S32 AVPLAY_ResetAudChn(AVPLAY_S *pAvplay);
extern HI_S32 AVPLAY_Reset(AVPLAY_S *pAvplay);

HI_U32 AVPLAY_GetSysTime(HI_VOID)
{
    HI_U32 time = 0;

    HI_SYS_GetTimeStampMs(&time);

    return time;
}

HI_U32 AVPLAY_GetVirtualWinChnNum(const AVPLAY_S *pAvplay)
{
    HI_U32  i;
    HI_U32  VirChnNum = 0;

    for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
    {
        if (HI_INVALID_HANDLE != pAvplay->VirFrmChn[i].hWindow)
        {
            VirChnNum++;
        }
    }

    return VirChnNum;
}

HI_U32 AVPLAY_GetSlaveWinChnNum(const AVPLAY_S *pAvplay)
{
    HI_U32  i;
    HI_U32  SlaveChnNum = 0;

    for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
    {
        if (HI_INVALID_HANDLE != pAvplay->SlaveFrmChn[i].hWindow)
        {
            SlaveChnNum++;
        }
    }

    return SlaveChnNum;
}

HI_S32 AVPLAY_GetStatusInfo(const AVPLAY_S *pAvplay, HI_UNF_AVPLAY_STATUS_INFO_S *pstStatusInfo)
{
    HI_S32                      Ret;
    ADEC_BUFSTATUS_S            AdecBufStatus = {0};
    VDEC_STATUSINFO_S           VdecBufStatus = {0};
    HI_MPI_DMX_BUF_STATUS_S     VidChnBuf = {0};
    HI_U32                      SndDelay = 0;
    HI_DRV_WIN_PLAY_INFO_S      WinPlayInfo = {0};
    HI_BOOL                     WinEnable;

    if (!pstStatusInfo)
    {
        HI_ERR_AVPLAY("para pstStatusInfo is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    pstStatusInfo->enRunStatus = pAvplay->CurStatus;

    if ((HI_INVALID_HANDLE != pAvplay->hAdec) && pAvplay->AudEnable)
    {
        memset(&AdecBufStatus, 0, sizeof(AdecBufStatus));

        (HI_VOID)HI_MPI_ADEC_GetInfo(pAvplay->hAdec, HI_MPI_ADEC_BUFFERSTATUS, &AdecBufStatus);
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32BufId = HI_UNF_AVPLAY_BUF_ID_ES_AUD;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32BufSize = AdecBufStatus.u32BufferSize;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32UsedSize = AdecBufStatus.u32BufferUsed;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32BufWptr = AdecBufStatus.u32BufWritePos;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32BufRptr = (HI_U32)AdecBufStatus.s32BufReadPos;
        pstStatusInfo->u32AuddFrameCount = AdecBufStatus.u32TotDecodeFrame;
    }
    else
    {
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32BufId = HI_UNF_AVPLAY_BUF_ID_ES_AUD;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32BufSize  = 0;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32UsedSize = 0;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32BufWptr  = 0;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32BufRptr  = 0;
        pstStatusInfo->u32AuddFrameCount = 0;
    }

    if (pAvplay->hSyncTrack != HI_INVALID_HANDLE)
    {
        Ret = HI_MPI_AO_Track_GetDelayMs(pAvplay->hSyncTrack, &SndDelay);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_HIAO_GetDelayMs failed 0x%x.\n", Ret);
            return Ret;
        }

        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32FrameBufTime = SndDelay;

    }
    else
    {
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32FrameBufTime = 0;
    }

    if ((HI_INVALID_HANDLE != pAvplay->hVdec) && pAvplay->VidEnable)
    {
        Ret = HI_MPI_VDEC_GetChanStatusInfo(pAvplay->hVdec, &VdecBufStatus);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_GetChanStatusInfo failed 0x%x\n", Ret);
            return Ret;
        }

        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
        #ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
            Ret = HI_MPI_DMX_GetPESBufferStatus(pAvplay->hDmxVid, &VidChnBuf);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_GetPESBufferStatus failed 0x%x\n", Ret);
                return Ret;
            }

            pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufSize = VidChnBuf.u32BufSize;
            pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32UsedSize = VidChnBuf.u32UsedSize;
            pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufWptr = VidChnBuf.u32BufWptr;
            pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufRptr = VidChnBuf.u32BufRptr;
        #else
            if(HI_INVALID_HANDLE != pAvplay->hDmxVid)
            {
                Ret = HI_MPI_DMX_GetPESBufferStatus(pAvplay->hDmxVid, &VidChnBuf);
                if (Ret != HI_SUCCESS)
                {
                    HI_ERR_AVPLAY("HI_MPI_DMX_GetPESBufferStatus failed 0x%x\n", Ret);
                    return Ret;
                }

                pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufSize = VidChnBuf.u32BufSize;
                pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32UsedSize = VidChnBuf.u32UsedSize;
                pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufWptr = VidChnBuf.u32BufWptr;
                pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufRptr = VidChnBuf.u32BufRptr;
            }
            else
            {
                pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufSize = 0;
                pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32UsedSize = 0;
                pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufWptr = 0;
                pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufRptr = 0;

            }
        #endif
        }
        else
        {
            pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufSize = VdecBufStatus.u32BufferSize;
            pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32UsedSize = VdecBufStatus.u32BufferUsed;
            pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32CurrentAvailableSize = VdecBufStatus.u32BufferAvailable;
            pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufWptr = 0;
            pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufRptr = 0;
        }

        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufId = HI_UNF_AVPLAY_BUF_ID_ES_VID;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32FrameBufNum = VdecBufStatus.u32FrameBufNum;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].bEndOfStream = VdecBufStatus.bEndOfStream;
        pstStatusInfo->u32VidFrameCount = VdecBufStatus.u32TotalDecFrmNum;
        pstStatusInfo->u32VidErrorFrameCount = VdecBufStatus.u32TotalErrFrmNum;
    }
    else
    {
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufSize = 0;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32UsedSize = 0;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32CurrentAvailableSize = 0;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufWptr = 0;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufRptr = 0;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufId = HI_UNF_AVPLAY_BUF_ID_ES_VID;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32FrameBufNum = 0;
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].bEndOfStream = HI_TRUE;
        pstStatusInfo->u32VidFrameCount = 0;
        pstStatusInfo->u32VidErrorFrameCount = 0;
    }

    if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
    {
        Ret = HI_MPI_WIN_GetEnable(pAvplay->MasterFrmChn.hWindow, &WinEnable);
        if ((Ret == HI_SUCCESS) && WinEnable)
        {
            Ret = HI_MPI_WIN_GetPlayInfo(pAvplay->MasterFrmChn.hWindow, &WinPlayInfo);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_WIN_GetPlayInfo failed 0x%x\n", Ret);
                return Ret;
            }
            else
            {
                pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32FrameBufTime = WinPlayInfo.u32DelayTime;
            }
        }
        else
        {
            pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32FrameBufTime = 0;
        }
    }
    else
    {
        pstStatusInfo->stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32FrameBufTime = 0;
    }

    if (HI_INVALID_HANDLE != pAvplay->hSync)
    {
        Ret = HI_MPI_SYNC_GetStatus(pAvplay->hSync, &pstStatusInfo->stSyncStatus);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_SYNC_GetStatus failed 0x%x\n", Ret);
            return Ret;
        }
    }
    else
    {
        pstStatusInfo->stSyncStatus.u32FirstAudPts    = HI_INVALID_PTS;
        pstStatusInfo->stSyncStatus.u32FirstVidPts    = HI_INVALID_PTS;
        pstStatusInfo->stSyncStatus.u32LastAudPts     = HI_INVALID_PTS;
        pstStatusInfo->stSyncStatus.u32LastVidPts     = HI_INVALID_PTS;
        pstStatusInfo->stSyncStatus.s32DiffAvPlayTime = 0;
        pstStatusInfo->stSyncStatus.u32PlayTime       = 0;
        pstStatusInfo->stSyncStatus.u32LocalTime      = HI_INVALID_TIME;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_MallocVidBuffer(AVPLAY_S *pAvplay, HI_HANDLE hVdec)
{
    HI_S32  Ret;
#ifdef HI_TEE_SUPPORT
    VDEC_BUFFER_ATTR_S stVdecBufAttr;
#endif

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
    #ifdef HI_TEE_SUPPORT
        /* set vdec to trust video path mode */
        Ret = HI_MPI_VDEC_SetTVP(hVdec, &(pAvplay->TVPAttr));
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_SetTVP ERR %#x\n", Ret);
            return Ret;
        }

        stVdecBufAttr.u32BufSize = 0;
        stVdecBufAttr.bTvp = pAvplay->TVPAttr.bEnable;
        Ret = HI_MPI_VDEC_ChanBufferInit(hVdec, pAvplay->hDmxVid, &stVdecBufAttr);
    #else
        Ret = HI_MPI_VDEC_ChanBufferInit(hVdec, 0, pAvplay->hDmxVid);
    #endif
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanBufferInit failed 0x%x\n", Ret);
            return Ret;
        }
    }
    else if (HI_UNF_AVPLAY_STREAM_TYPE_ES == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
    #ifdef HI_TEE_SUPPORT
        /* set vdec to trust video path mode */
        Ret = HI_MPI_VDEC_SetTVP(hVdec, &(pAvplay->TVPAttr));
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_SetTVP ERR, Ret=%#x\n", Ret);
            return Ret;
        }

        if (0 != pAvplay->AvplayAttr.stStreamAttr.u32VidBufSize)
        {
            stVdecBufAttr.u32BufSize = pAvplay->AvplayAttr.stStreamAttr.u32VidBufSize;
            stVdecBufAttr.bTvp = pAvplay->TVPAttr.bEnable;
            Ret = HI_MPI_VDEC_ChanBufferInit(hVdec, HI_INVALID_HANDLE, &stVdecBufAttr);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_VDEC_ChanBufferInit failed 0x%x\n", Ret);
                return Ret;
            }
        }
    #else
        if (0 != pAvplay->AvplayAttr.stStreamAttr.u32VidBufSize)
        {
            Ret = HI_MPI_VDEC_ChanBufferInit(hVdec, pAvplay->AvplayAttr.stStreamAttr.u32VidBufSize, HI_INVALID_HANDLE);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_VDEC_ChanBufferInit failed 0x%x\n", Ret);
                return Ret;
            }
        }
    #endif
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_FreeVidBuffer(AVPLAY_S *pAvplay, HI_HANDLE hVdec)
{
    HI_S32  Ret = HI_SUCCESS;

    if (0 != pAvplay->AvplayAttr.stStreamAttr.u32VidBufSize)
    {
        Ret = HI_MPI_VDEC_ChanBufferDeInit(hVdec);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanBufferDeInit failed 0x%x\n", Ret);
            return Ret;
        }
    }

    return Ret;
}

HI_VOID AVPLAY_Notify(const AVPLAY_S *pAvplay, HI_UNF_AVPLAY_EVENT_E EvtMsg, HI_VOID *pEvtPara)
{
    HI_S32 Ret;

    if (pAvplay->EvtCbFunc64Flag)
    {
        if (pAvplay->EvtCbFunc64[EvtMsg])
        {
            Ret = (pAvplay->EvtCbFunc64[EvtMsg])(pAvplay->hAvplay, EvtMsg, pEvtPara);
            if (HI_SUCCESS != Ret)
            {
                HI_WARN_AVPLAY("callback event %d error\n", EvtMsg);
            }
        }
    }
#ifndef __aarch64__
    else
    {
        HI_U32  Param = 0;

        switch (EvtMsg)
        {
            case HI_UNF_AVPLAY_EVENT_RNG_BUF_STATE :
            case HI_UNF_AVPLAY_EVENT_AUD_BUF_STATE :
            case HI_UNF_AVPLAY_EVENT_VID_BUF_STATE :
            case HI_UNF_AVPLAY_EVENT_FRAMEPACKING_CHANGE :
            case HI_UNF_AVPLAY_EVENT_IFRAME_ERR :
            case HI_UNF_AVPLAY_EVENT_VID_UNSUPPORT :
            case HI_UNF_AVPLAY_EVENT_VID_ERR_RATIO :
            case HI_UNF_AVPLAY_EVENT_VID_ERR_TYPE :
            case HI_UNF_AVPLAY_EVENT_AUD_UNSUPPORT :
            case HI_UNF_AVPLAY_EVENT_AUD_FRAME_ERR :
            case HI_UNF_AVPLAY_EVENT_STOP :
            case HI_UNF_AVPLAY_EVENT_VID_ERR_FRAME :
            case HI_UNF_AVPLAY_EVENT_STATUS_REPORT:
            {
                if (pEvtPara)
                {
                    Param = *((HI_U32*)pEvtPara);
                }
                break;
            }

            default :
                Param = (HI_U32)pEvtPara;
        }

        if (pAvplay->EvtCbFunc[EvtMsg])
        {
            Ret = (pAvplay->EvtCbFunc[EvtMsg])(pAvplay->hAvplay, EvtMsg, Param);
            if (HI_SUCCESS != Ret)
            {
                HI_WARN_AVPLAY("callback event %d error\n", EvtMsg);
            }
        }
    }
#endif

    return;
}

HI_BOOL AVPLAY_IsBufEmpty(AVPLAY_S *pAvplay)
{
    ADEC_BUFSTATUS_S            AdecBuf = {0};
    VDEC_STATUSINFO_S           VdecBuf = {0};
    HI_U32                      AudEsBuf = 0;
    HI_U32                      VidEsBuf = 0;
    HI_U32                      VidEsBufWptr = 0;
    HI_U32                      AudEsBufWptr = 0;

    HI_BOOL                     bEmpty = HI_TRUE;
    HI_U32                      Systime = 0;
    HI_U32                      u32TsCnt = 0;
    HI_DRV_WIN_PLAY_INFO_S      WinPlayInfo = {0};
    HI_MPI_DMX_BUF_STATUS_S     VidPesBuf = {0};
    HI_S32                      Ret;
    HI_BOOL                     bBufEmpty = HI_FALSE;
    HI_BOOL                     bBufStill = HI_FALSE;
    HI_UNF_AVPLAY_STREAM_TYPE_E enStreamType;

    WinPlayInfo.u32DelayTime = 0;

    enStreamType = pAvplay->AvplayAttr.stStreamAttr.enStreamType;

    if (!pAvplay->AudEnable && !pAvplay->VidEnable)
    {
        return HI_TRUE;
    }

    if (pAvplay->AudEnable)
    {
        Ret = HI_MPI_ADEC_GetInfo(pAvplay->hAdec, HI_MPI_ADEC_BUFFERSTATUS, &AdecBuf);
        if (HI_SUCCESS == Ret)
        {
            AudEsBuf = AdecBuf.u32BufferUsed;
            AudEsBufWptr = AdecBuf.u32BufWritePos;
        }

        if (HI_INVALID_HANDLE != pAvplay->hSyncTrack)
        {
            (HI_VOID)HI_MPI_AO_Track_IsBufEmpty(pAvplay->hSyncTrack, &bEmpty);
        }
    }

    if (pAvplay->VidEnable)
    {
        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == enStreamType)
        {
            (HI_VOID)HI_MPI_DMX_GetChannelTsCount(pAvplay->hDmxVid, &u32TsCnt);
            Ret = HI_MPI_DMX_GetPESBufferStatus(pAvplay->hDmxVid, &VidPesBuf);
            if (HI_SUCCESS == Ret)
            {
                VidEsBuf = VidPesBuf.u32UsedSize;
            }
        }
        else
        {
            Ret = HI_MPI_VDEC_GetChanStatusInfo(pAvplay->hVdec, &VdecBuf);
            if (HI_SUCCESS == Ret)
            {
                VidEsBuf = VdecBuf.u32BufferUsed;
            }
        }

        if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
        {
            (HI_VOID)HI_MPI_WIN_GetPlayInfo(pAvplay->MasterFrmChn.hWindow, &WinPlayInfo);
        }
    }

    if ((WinPlayInfo.u32FrameNumInBufQn != 0) || (bEmpty != HI_TRUE))
    {
        pAvplay->CurBufferEmptyState = HI_FALSE;
        return HI_FALSE;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == enStreamType)
    {
        bBufEmpty =    (AudEsBuf < APPLAY_EOS_BUF_MIN_LEN)
                    && (VidEsBuf < APPLAY_EOS_BUF_MIN_LEN)
                    && (u32TsCnt == pAvplay->PreTscnt)
                    && (AudEsBufWptr == pAvplay->PreAudEsBufWPtr);

        bBufStill =    (AudEsBuf == pAvplay->PreAudEsBuf)
                    && (VidEsBuf == pAvplay->PreVidEsBuf)
                    && (u32TsCnt == pAvplay->PreTscnt)
                    && (AudEsBufWptr == pAvplay->PreAudEsBufWPtr);
    }
    else
    {
        bBufEmpty =    (AudEsBuf < APPLAY_EOS_BUF_MIN_LEN)
                    && (VidEsBuf < APPLAY_EOS_BUF_MIN_LEN)
                    && (VidEsBufWptr == pAvplay->PreVidEsBufWPtr)
                    && (AudEsBufWptr == pAvplay->PreAudEsBufWPtr);

        bBufStill =    (AudEsBuf == pAvplay->PreAudEsBuf)
                    && (VidEsBuf == pAvplay->PreVidEsBuf)
                    && (VidEsBufWptr == pAvplay->PreVidEsBufWPtr)
                    && (AudEsBufWptr == pAvplay->PreAudEsBufWPtr);
    }

    if (bBufEmpty)
    {
        pAvplay->PreAudEsBuf = AudEsBuf;
        pAvplay->CurBufferEmptyState = HI_TRUE;
        pAvplay->PreSystime = 0;

        HI_UNF_AVPLAY_STREAM_TYPE_TS == enStreamType ?
            (pAvplay->PreTscnt = u32TsCnt) : (pAvplay->PreVidEsBuf = VidEsBuf);

        return HI_TRUE;
    }

    if (bBufStill)
    {
        Systime = AVPLAY_GetSysTime();

        if (   ((Systime > pAvplay->PreSystime) && ((Systime-pAvplay->PreSystime) > AVPLAY_EOS_TIMEOUT))
            || ((Systime < pAvplay->PreSystime) && (((SYS_TIME_MAX-pAvplay->PreSystime)+Systime) > AVPLAY_EOS_TIMEOUT)) )
        {
            pAvplay->PreAudEsBuf = AudEsBuf;
            pAvplay->PreVidEsBuf = VidEsBuf;
            pAvplay->CurBufferEmptyState = HI_TRUE;
            pAvplay->PreSystime = 0;

            return HI_TRUE;
        }
    }
    else
    {
        pAvplay->PreAudEsBuf = AudEsBuf;
        pAvplay->PreVidEsBuf = VidEsBuf;
        pAvplay->PreAudEsBufWPtr = AudEsBufWptr;
        pAvplay->PreSystime = AVPLAY_GetSysTime();

        HI_UNF_AVPLAY_STREAM_TYPE_TS == enStreamType ?
            (pAvplay->PreTscnt = u32TsCnt) : (pAvplay->PreVidEsBufWPtr = VidEsBufWptr);
    }

    pAvplay->CurBufferEmptyState = HI_FALSE;

    return HI_FALSE;
}

HI_UNF_AVPLAY_BUF_STATE_E AVPLAY_CaclBufState(const AVPLAY_S *pAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, HI_U32 UsedBufPercent)
{
    HI_UNF_AVPLAY_BUF_STATE_E          CurBufState = HI_UNF_AVPLAY_BUF_STATE_NORMAL;
    VDEC_FRMSTATUSINFO_S               VdecFrmBuf = {0};
    HI_U32                             u32FrmTime = 0;
    HI_DRV_WIN_PLAY_INFO_S             WinInfo;
    HI_S32                             Ret = HI_SUCCESS;
    HI_U32                             u32StrmTime = 0;

    memset(&WinInfo, 0x0, sizeof(HI_DRV_WIN_PLAY_INFO_S));

    if (HI_UNF_AVPLAY_MEDIA_CHAN_AUD == enChn)
    {
        if (UsedBufPercent >= AVPLAY_ES_AUD_FULL_PERCENT)
        {
            CurBufState = HI_UNF_AVPLAY_BUF_STATE_FULL;
        }
        else if ((UsedBufPercent >= AVPLAY_ES_AUD_HIGH_PERCENT) && (UsedBufPercent < AVPLAY_ES_AUD_FULL_PERCENT))
        {
            CurBufState = HI_UNF_AVPLAY_BUF_STATE_HIGH;
        }
        else if (UsedBufPercent < AVPLAY_ES_AUD_LOW_PERCENT)
        {
            u32FrmTime = pAvplay->AudInfo.FrameNum * pAvplay->AudInfo.FrameTime;

            if (UsedBufPercent < AVPLAY_ES_AUD_EMPTY_PERCENT)
            {
                if (pAvplay->AudInfo.BufTime + u32FrmTime <= 150)
                {
                    CurBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
                }
                else
                {
                    CurBufState = HI_UNF_AVPLAY_BUF_STATE_LOW;
                }
            }
            else
            {
                if (pAvplay->AudInfo.BufTime + u32FrmTime <= 150)
                {
                    CurBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
                }
                else if (pAvplay->AudInfo.BufTime + u32FrmTime <= 240)
                {
                    CurBufState = HI_UNF_AVPLAY_BUF_STATE_LOW;
                }
            }
        }
    }
    else
    {
        if (UsedBufPercent >= AVPLAY_ES_VID_FULL_PERCENT)
        {
            CurBufState = HI_UNF_AVPLAY_BUF_STATE_FULL;
        }
        else if ((UsedBufPercent >= AVPLAY_ES_VID_HIGH_PERCENT) && (UsedBufPercent < AVPLAY_ES_VID_FULL_PERCENT))
        {
            CurBufState = HI_UNF_AVPLAY_BUF_STATE_HIGH;
        }
        else if (UsedBufPercent < AVPLAY_ES_VID_LOW_PERCENT)
        {
            if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
            {
                return HI_UNF_AVPLAY_BUF_STATE_LOW;
            }

            Ret = HI_MPI_VDEC_GetChanFrmStatusInfo(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort, &VdecFrmBuf);
            Ret |= HI_MPI_WIN_GetPlayInfo(pAvplay->MasterFrmChn.hWindow, &WinInfo);
            if (Ret != HI_SUCCESS)
            {
                return HI_UNF_AVPLAY_BUF_STATE_LOW;
            }

            /*InBps is too small*/
            if(VdecFrmBuf.u32StrmInBps < 100)
            {
                u32StrmTime = 0;
            }
            else
            {
                u32StrmTime = (VdecFrmBuf.u32StrmSize * 1000)/(VdecFrmBuf.u32StrmInBps);
            }

            if (WinInfo.u32FrameNumInBufQn <= 1)
            {
                CurBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
            }
            else if (WinInfo.u32FrameNumInBufQn + VdecFrmBuf.u32DecodedFrmNum <= 5)
            {
                if (u32StrmTime <= 80)
                {
                    CurBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
                }
                else
                {
                    CurBufState = HI_UNF_AVPLAY_BUF_STATE_LOW;
                }
            }
            else if (WinInfo.u32FrameNumInBufQn + VdecFrmBuf.u32DecodedFrmNum <= 10)
            {
                if (u32StrmTime <= 40)
                {
                    CurBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
                }
                else if (u32StrmTime <= 80)
                {
                    CurBufState = HI_UNF_AVPLAY_BUF_STATE_LOW;
                }
                else
                {
                    CurBufState = HI_UNF_AVPLAY_BUF_STATE_NORMAL;
                }
            }
            else
            {
                CurBufState = HI_UNF_AVPLAY_BUF_STATE_NORMAL;
            }
        }
    }

    return CurBufState;
}

HI_VOID AVPLAY_SetAoTrackSpeed(AVPLAY_S *pAvplay)
{
#ifdef AVPLAY_AUD_SPEED_ADJUST_SUPPORT

    HI_S32                    Ret;
    HI_UNF_AUDIOTRACK_ATTR_S  TrackInfo = {0};

    memset(&TrackInfo, 0x0, sizeof(HI_UNF_AUDIOTRACK_ATTR_S));

    if (HI_INVALID_HANDLE != pAvplay->hSyncTrack)
    {
        return;
    }

    Ret = HI_MPI_AO_Track_GetAttr(pAvplay->hSyncTrack, &TrackInfo);
    if (   (HI_SUCCESS == Ret)
        && (HI_UNF_SND_TRACK_TYPE_MASTER == TrackInfo.enTrackType)
        && (HI_FALSE == pAvplay->AudDDPMode) )  /*do not use speed adjust when ddp test*/
    {

        if (SYNC_AUD_SPEED_ADJUST_NORMAL == pAvplay->AudOpt.SpeedAdjust)
        {
            (HI_VOID)HI_MPI_AO_Track_SetSpeedAdjust(pAvplay->hSyncTrack, 0, HI_MPI_AO_SND_SPEEDADJUST_MUTE);
            (HI_VOID)HI_MPI_AO_Track_SetSpeedAdjust(pAvplay->hSyncTrack, 0, HI_MPI_AO_SND_SPEEDADJUST_SRC);
        }
        else if (SYNC_AUD_SPEED_ADJUST_UP == pAvplay->AudOpt.SpeedAdjust)
        {
            (HI_VOID)HI_MPI_AO_Track_SetSpeedAdjust(pAvplay->hSyncTrack, 0, HI_MPI_AO_SND_SPEEDADJUST_SRC);
        }
        else if (SYNC_AUD_SPEED_ADJUST_DOWN == pAvplay->AudOpt.SpeedAdjust)
        {
            (HI_VOID)HI_MPI_AO_Track_SetSpeedAdjust(pAvplay->hSyncTrack, -10, HI_MPI_AO_SND_SPEEDADJUST_SRC);
        }
        else if (SYNC_AUD_SPEED_ADJUST_MUTE_REPEAT == pAvplay->AudOpt.SpeedAdjust)
        {
            (HI_VOID)HI_MPI_AO_Track_SetSpeedAdjust(pAvplay->hSyncTrack, -100, HI_MPI_AO_SND_SPEEDADJUST_MUTE);
        }
    }
#endif
    return;
}

HI_VOID AVPLAY_ProcAdecToAo(AVPLAY_S *pAvplay)
{
    HI_S32                    Ret = HI_SUCCESS;
    ADEC_EXTFRAMEINFO_S       AdecExtInfo = {0};
    HI_U32                    AoBufTime = 0;
    ADEC_STATUSINFO_S         AdecStatusinfo;
    HI_U32                    i;

    if (!pAvplay->AudEnable)
    {
        return;
    }

    if (HI_UNF_AVPLAY_STATUS_PAUSE == pAvplay->CurStatus)
    {
        return;
    }

    memset(&AdecStatusinfo, 0x0, sizeof(ADEC_STATUSINFO_S));


    if (!pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO])
    {
        pAvplay->DebugInfo.AcquireAudFrameNum++;

        Ret = HI_MPI_ADEC_ReceiveFrame(pAvplay->hAdec, &pAvplay->AvplayAudFrm, &AdecExtInfo);
        if (HI_SUCCESS == Ret)
        {
            pAvplay->AudInfo.SrcPts = AdecExtInfo.u32OrgPtsMs;
            pAvplay->AudInfo.Pts = pAvplay->AvplayAudFrm.u32PtsMs;
            pAvplay->AudInfo.FrameTime = AdecExtInfo.u32FrameDurationMs;

            pAvplay->DebugInfo.AcquiredAudFrameNum++;
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_TRUE;

            AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_NEW_AUD_FRAME, &pAvplay->AvplayAudFrm);
        }
        else
        {
        }
    }

    if (pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO])
    {
        if (HI_UNF_AVPLAY_STATUS_TPLAY == pAvplay->CurStatus)
        {
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;
            pAvplay->AvplayProcAudContinue = HI_TRUE;
            (HI_VOID)HI_MPI_ADEC_ReleaseFrame(pAvplay->hAdec, &pAvplay->AvplayAudFrm);
        }
        else
        {
            if (HI_INVALID_HANDLE != pAvplay->hSyncTrack)
            {
                (HI_VOID)HI_MPI_AO_Track_GetDelayMs(pAvplay->hSyncTrack, &AoBufTime);

                (HI_VOID)HI_MPI_ADEC_GetInfo(pAvplay->hAdec, HI_MPI_ADEC_STATUSINFO, &AdecStatusinfo);

                pAvplay->AudInfo.BufTime = AoBufTime;
                pAvplay->AudInfo.FrameNum = AdecStatusinfo.u32UsedBufNum;

                Ret = HI_MPI_SYNC_AudJudge(pAvplay->hSync, &pAvplay->AudInfo, &pAvplay->AudOpt);
                if (HI_SUCCESS == Ret)
                {
                    if (SYNC_PROC_DISCARD == pAvplay->AudOpt.SyncProc)
                    {
                        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;
                        pAvplay->AvplayProcAudContinue = HI_TRUE;
                        (HI_VOID)HI_MPI_ADEC_ReleaseFrame(pAvplay->hAdec, &pAvplay->AvplayAudFrm);
                        return;
                    }
                    else if (SYNC_PROC_REPEAT == pAvplay->AudOpt.SyncProc)
                    {
                        return;
                    }

                    (HI_VOID)AVPLAY_SetAoTrackSpeed(pAvplay);
                }
            }

            pAvplay->DebugInfo.SendAudFrameNum++;

            if (HI_INVALID_HANDLE != pAvplay->hSyncTrack)
            {
                /*send frame to main track*/
                Ret = HI_MPI_AO_Track_SendData(pAvplay->hSyncTrack, &pAvplay->AvplayAudFrm);
                if (HI_SUCCESS == Ret)
                {
                    /*send frame to other track*/
                    for(i=0; i<pAvplay->TrackNum; i++)
                    {
                        if (pAvplay->hSyncTrack != pAvplay->hTrack[i])
                        {
                            (HI_VOID)HI_MPI_AO_Track_SendData(pAvplay->hTrack[i], &pAvplay->AvplayAudFrm);
                        }
                    }
                }
            }
            else
            {
                for(i = 0; i < pAvplay->TrackNum; i++)
                {
                    Ret = HI_MPI_AO_Track_SendData(pAvplay->hTrack[i], &pAvplay->AvplayAudFrm);
                    if (HI_SUCCESS != Ret)
                    {
                        HI_WARN_AVPLAY("track num %d send data failed\n", i);
                    }
                }
            }

            if (HI_SUCCESS == Ret)
            {
                pAvplay->DebugInfo.SendedAudFrameNum++;
                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;
                pAvplay->AvplayProcAudContinue = HI_TRUE;

                (HI_VOID)HI_MPI_ADEC_ReleaseFrame(pAvplay->hAdec, &pAvplay->AvplayAudFrm);
            }
            else
            {
                if (   HI_ERR_AO_OUT_BUF_FULL != Ret
                    && HI_ERR_AO_SENDMUTE != Ret
                    && HI_ERR_AO_PAUSE_STATE != Ret) /* Error drop this frame */
                {
                    pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;
                    pAvplay->AvplayProcAudContinue = HI_TRUE;
                    (HI_VOID)HI_MPI_ADEC_ReleaseFrame(pAvplay->hAdec, &pAvplay->AvplayAudFrm);

                    HI_ERR_AVPLAY("Send AudFrame to AO failed:%#x, drop a frame.\n", Ret);
                }
            }
        }
    }

    return;
}

HI_S32 AVPLAY_GetWindowByPort(const AVPLAY_S *pAvplay, HI_HANDLE hPort, HI_HANDLE *phWindow)
{
    HI_U32          i;

    if (pAvplay->MasterFrmChn.hPort == hPort)
    {
        *phWindow = pAvplay->MasterFrmChn.hWindow;
        return HI_SUCCESS;
    }

    for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
    {
        if (pAvplay->SlaveFrmChn[i].hPort == hPort)
        {
            *phWindow = pAvplay->SlaveFrmChn[i].hWindow;
            return HI_SUCCESS;
        }
    }

    for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
    {
        if (pAvplay->VirFrmChn[i].hPort == hPort)
        {
            *phWindow = pAvplay->VirFrmChn[i].hWindow;
            return HI_SUCCESS;
        }
    }

    *phWindow = HI_INVALID_HANDLE;

    return HI_FAILURE;
}

HI_VOID AVPLAY_ProcFirstDispFrame(AVPLAY_S *pAvplay)
{
    HI_S32                  Ret;
    HI_DRV_WIN_PLAY_INFO_S  WinInfo;

    if (!pAvplay->bVidFirstQueueFrame)
    {
        pAvplay->VidFirstQueueFrameTime = AVPLAY_GetSysTime();

        Ret = HI_MPI_WIN_GetPlayInfo(pAvplay->MasterFrmChn.hWindow, &WinInfo);
        if (HI_SUCCESS == Ret)
        {
            if (0 != WinInfo.u32DispRate)
            {
                pAvplay->VidFirstFrameDispTime = 100000 / WinInfo.u32DispRate;
            }
        }

        pAvplay->bVidFirstQueueFrame = HI_TRUE;
        pAvplay->bVidFirstDispFrame  = HI_TRUE;
    }

    return;
}

HI_VOID AVPLAY_ProcFrmToVirWin(AVPLAY_S *pAvplay)
{
    HI_S32                              Ret;
    HI_U32                              i, j;
    HI_HANDLE                           hWindow = HI_INVALID_HANDLE;

    pAvplay->bProcFrmToVirtualWin = HI_FALSE;

    if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
    {
        for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
        {
            (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

            for (j = 0; j < AVPLAY_MAX_VIR_FRMCHAN; j++)
            {
                pAvplay->bSendedFrmToVirtualWin[j] = HI_TRUE;

                if (hWindow == pAvplay->VirFrmChn[j].hWindow)
                {
                    pAvplay->DebugInfo.VirVidStat[j].SendNum++;

                    Ret = HI_MPI_WIN_QueueFrame(hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                    if (HI_SUCCESS == Ret)
                    {
                        pAvplay->DebugInfo.VirVidStat[j].PlayNum++;
                    }
                    else
                    {
                        /* To ensure the real-time performance of the transcode,
                           it is required to discard the failed video frames.*/
                        Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport,
                            &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                        if (HI_SUCCESS != Ret)
                        {
                            HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
                        }

                        pAvplay->DebugInfo.VirVidStat[j].DiscardNum++;
                    }
                }
            }
        }
    }
    else
    {
        for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
        {
            (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

            for (j = 0; j < AVPLAY_MAX_VIR_FRMCHAN; j++)
            {
                if (hWindow == pAvplay->VirFrmChn[j].hWindow)
                {
                    pAvplay->DebugInfo.VirVidStat[j].SendNum++;
                    pAvplay->bSendedFrmToVirtualWin[j] = HI_TRUE;

                    Ret = HI_MPI_WIN_QueueFrame(hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                    if (HI_SUCCESS == Ret)
                    {
                        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
                        pAvplay->DebugInfo.VirVidStat[j].PlayNum++;
                    }
                    else if (HI_ERR_VO_BUFQUE_FULL != Ret)
                    {
                        Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport,
                            &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                        if (HI_SUCCESS != Ret)
                        {
                            HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
                        }

                        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
                        pAvplay->DebugInfo.VirVidStat[j].DiscardNum++;
                    }
                    else
                    {
                        pAvplay->bSendedFrmToVirtualWin[j] = HI_FALSE;
                        pAvplay->bProcFrmToVirtualWin      = HI_TRUE;
                    }
                }
            }
        }
    }

    return;
}

HI_VOID AVPLAY_ProcOriVidFrameRate(AVPLAY_S *pAvplay)
{
#ifdef AVPLAY_FRAME_RATE_ADJUST_SUPPORT
    HI_U32                i;
    HI_U32                j;
    HI_DRV_VIDEO_FRAME_S *pVidFrm = HI_NULL;
    HI_HANDLE             hWindow = HI_INVALID_HANDLE;

#ifdef HI_HDR_SUPPORT
    if (HI_TRUE == pAvplay->bHDRPlay)
    {
        HI_S32                Ret;
        HI_U32                BlPortId = -1;
        HI_U32                ElPortId = -1;
        HI_DRV_VIDEO_FRAME_S *pBlVidFrm = HI_NULL;
        HI_DRV_VIDEO_FRAME_S *pElVidFrm = HI_NULL;

        if (HI_TRUE == pAvplay->bDualChnPlay)
        {
            Ret = AVPLAY_GetBlFramePort(pAvplay, pAvplay->hBlPort, &BlPortId);
            if (HI_SUCCESS != Ret)
            {
                return;
            }

            Ret = AVPLAY_GetElFramePort(pAvplay, pAvplay->hElPort, &ElPortId);
            if (HI_SUCCESS != Ret)
            {
                return;
            }

            pVidFrm   = &pAvplay->CurFrmPack.stFrame[BlPortId].stFrameVideo;
            pBlVidFrm = &pAvplay->CurBlFrmPack.stFrame[BlPortId].stFrameVideo;
            pElVidFrm = &pAvplay->CurElFrmPack.stFrame[ElPortId].stFrameVideo;

            pVidFrm->u32OriFrameRate   = pVidFrm->u32FrameRate;
            pBlVidFrm->u32OriFrameRate = pBlVidFrm->u32FrameRate;
            pElVidFrm->u32OriFrameRate = pElVidFrm->u32FrameRate;
        }
        else
        {
            for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
            {
                if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
                {
                    break;
                }
            }

            if (i == pAvplay->CurFrmPack.u32FrmNum)
            {
                return;
            }

            pVidFrm = &pAvplay->CurFrmPack.stFrame[i].stFrameVideo;
            pVidFrm->u32OriFrameRate = pVidFrm->u32FrameRate;
        }
    }
    else
#endif
    {
        for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
        {
            (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

            if (hWindow == pAvplay->MasterFrmChn.hWindow)
            {
                pVidFrm = &pAvplay->CurFrmPack.stFrame[i].stFrameVideo;
                pVidFrm->u32OriFrameRate = pVidFrm->u32FrameRate;
                continue;
            }

            for (j = 0; j < AVPLAY_MAX_SLAVE_FRMCHAN; j++)
            {
                if (hWindow == pAvplay->SlaveFrmChn[j].hWindow)
                {
                    pVidFrm = &pAvplay->CurFrmPack.stFrame[i].stFrameVideo;
                    pVidFrm->u32OriFrameRate = pVidFrm->u32FrameRate;
                }
            }
        }
    }
#endif

    return;
}

HI_VOID AVPLAY_ProcVidFrameRate(AVPLAY_S *pAvplay, HI_DRV_VIDEO_FRAME_S *pVideoFrame)
{
   /*
    * Private video playback:
    * 1.Set the input frame rate is equal to the output frame rate;
    * 2.The display module needn't do FRC;
    */
#ifdef AVPLAY_FRAME_RATE_ADJUST_SUPPORT
    HI_S32                  Ret;
    HI_DRV_WIN_PLAY_INFO_S  WinInfo = {0};

    Ret = HI_MPI_WIN_GetPlayInfo(pAvplay->MasterFrmChn.hWindow, &WinInfo);
    if (HI_SUCCESS == Ret)
    {
        pVideoFrame->u32FrameRate = WinInfo.u32DispRate * 10;
    }
#endif

    return;
}

HI_VOID AVPLAY_ProcVidFrc(AVPLAY_S *pAvplay)
{
    HI_U32                              i;
    HI_S32                              Ret;
    HI_HANDLE                           hWindow = HI_INVALID_HANDLE;
    HI_DRV_WIN_PLAY_INFO_S              WinInfo;
    HI_DRV_VIDEO_PRIVATE_S             *pVideoPriv = HI_NULL;
    HI_UNF_AVPLAY_FRMRATE_PARAM_S       FrameRate = {0};

    memset(&WinInfo, 0x0, sizeof(HI_DRV_WIN_PLAY_INFO_S));

    pAvplay->FrcNeedPlayCnt = 1;
    pAvplay->FrcCurPlayCnt = 0;
    pAvplay->FrcCtrlInfo.s32FrmState = 0;

    /* do not do frc in low delay mode */
    if (pAvplay->LowDelayAttr.bEnable)
    {
        return;
    }

    /* find the master chan */
    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

        if (hWindow == pAvplay->MasterFrmChn.hWindow)
        {
            break;
        }
    }

    if (i == pAvplay->CurFrmPack.u32FrmNum)
    {
        return;
    }

    pVideoPriv = (HI_DRV_VIDEO_PRIVATE_S*)(pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32Priv);

    (HI_VOID)HI_MPI_WIN_GetPlayInfo(hWindow, &WinInfo);

    memset(&FrameRate, 0, sizeof(HI_UNF_AVPLAY_FRMRATE_PARAM_S));
    Ret = HI_MPI_VDEC_GetChanFrmRate(pAvplay->hVdec, &FrameRate);
    if (HI_SUCCESS != Ret)
    {
        return;
    }

    if (FrameRate.enFrmRateType == HI_UNF_AVPLAY_FRMRATE_TYPE_USER)
    {
        if ((HI_DRV_FIELD_TOP == pVideoPriv->eOriginField)
            || (HI_DRV_FIELD_BOTTOM == pVideoPriv->eOriginField))
        {
            pAvplay->FrcParamCfg.u32InRate = (FrameRate.stSetFrmRate.u32fpsInteger * 100
                                         + FrameRate.stSetFrmRate.u32fpsDecimal / 10) * 2;
        }
        else
        {
            pAvplay->FrcParamCfg.u32InRate = FrameRate.stSetFrmRate.u32fpsInteger * 100
                                         + FrameRate.stSetFrmRate.u32fpsDecimal / 10;
        }
    }
    else
    {
        pAvplay->FrcParamCfg.u32InRate = pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32FrameRate/10;
    }

    pAvplay->FrcParamCfg.u32OutRate = WinInfo.u32DispRate;

    if (HI_TRUE == pAvplay->bFrcEnable)
    {
        /*do frc for every new frame*/
        (HI_VOID)AVPLAY_FrcCalculate(&pAvplay->FrcCalAlg, &pAvplay->FrcParamCfg, &pAvplay->FrcCtrlInfo);

        /* sometimes(such as pvr smooth tplay), vdec set u32PlayTime, means this frame must repeat */
        pAvplay->FrcNeedPlayCnt = (1 + pAvplay->FrcCtrlInfo.s32FrmState) * (1 + pVideoPriv->u32PlayTime);
    }

    return;
}

HI_VOID AVPLAY_ProcVidSync(AVPLAY_S *pAvplay)
{
    HI_U32                              i;
    HI_HANDLE                           hWindow = HI_INVALID_HANDLE;
    HI_DRV_WIN_PLAY_INFO_S              WinInfo = {0};
    HI_DRV_VIDEO_PRIVATE_S             *pFrmPriv = HI_NULL;

    memset(&WinInfo, 0x0, sizeof(HI_DRV_WIN_PLAY_INFO_S));

    pAvplay->VidOpt.SyncProc = SYNC_PROC_PLAY;

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        pAvplay->CurFrmPack.stFrame[i].stFrameVideo.enTBAdjust = HI_DRV_VIDEO_TB_PLAY;
    }

    /* do not do sync in low delay mode */
    if (pAvplay->LowDelayAttr.bEnable)
    {
        return;
    }

    /* find the master chan */
    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

        if (hWindow == pAvplay->MasterFrmChn.hWindow)
        {
            break;
        }
    }

    if (i == pAvplay->CurFrmPack.u32FrmNum)
    {
        return;
    }

    pAvplay->VidInfo.SrcPts = pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32SrcPts;
    pAvplay->VidInfo.Pts = pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32Pts;

    if (0 != pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32OriFrameRate)
    {
        pAvplay->VidInfo.FrameTime = 1000000 / pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32OriFrameRate;
    }
    else
    {
        pAvplay->VidInfo.FrameTime = 40;
    }

    pFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)(pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32Priv);
    pFrmPriv->u32PlayTime = 1;

    /* obtain original stream info, judge whether Progressive*/
    if (HI_DRV_FIELD_ALL == pFrmPriv->eOriginField)
    {
        pAvplay->VidInfo.bProgressive = HI_TRUE;
    }
    else
    {
        pAvplay->VidInfo.bProgressive = HI_FALSE;
    }

    pAvplay->VidInfo.DispTime = pAvplay->FrcNeedPlayCnt;

    /* need to obtain real-time delaytime */
    (HI_VOID)HI_MPI_WIN_GetPlayInfo(hWindow, &WinInfo);
    pAvplay->VidInfo.DelayTime = WinInfo.u32DelayTime;
    pAvplay->VidInfo.DispRate = WinInfo.u32DispRate;

    (HI_VOID)HI_MPI_SYNC_VidJudge(pAvplay->hSync, &pAvplay->VidInfo, &pAvplay->VidOpt);

    for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        pAvplay->CurFrmPack.stFrame[i].stFrameVideo.enTBAdjust = pAvplay->VidOpt.enTBAdjust;

        if (HI_UNF_AVPLAY_STATUS_TPLAY == pAvplay->CurStatus)
        {
            pAvplay->CurFrmPack.stFrame[i].stFrameVideo.enTBAdjust = HI_DRV_VIDEO_TB_PLAY;
        }
    }

    return;
}

HI_VOID AVPLAY_ProcVidPlay(AVPLAY_S *pAvplay)
{
    HI_S32                              Ret;
    HI_U32                              i, j;
    HI_HANDLE                           hWindow = HI_INVALID_HANDLE;
    HI_LD_Event_S                       LdEvent;

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

        if (hWindow == pAvplay->MasterFrmChn.hWindow)
        {
            break;
        }
    }

    if (i == pAvplay->CurFrmPack.u32FrmNum)
    {
        return;
    }

    pAvplay->DebugInfo.MasterVidStat.SendNum++;
    pAvplay->bSendedFrmToMasterWin = HI_TRUE;

    AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);

    Ret = HI_MPI_WIN_QueueFrame(hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
    if (HI_SUCCESS == Ret)
    {
        /* record low delay event */
        LdEvent.evt_id = EVENT_AVPLAY_FRM_OUT;
        LdEvent.frame = pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32FrameIndex;
        LdEvent.handle = pAvplay->CurFrmPack.stFrame[i].stFrameVideo.hTunnelSrc;
        (HI_VOID)HI_SYS_GetTimeStampMs(&(LdEvent.time));

        (HI_VOID)HI_MPI_STAT_NotifyLowDelayEvent(&LdEvent);

        /* record program switch event */
        if (pAvplay->CurFrmPack.stFrame[i].stFrameVideo.bIsFirstIFrame)
        {
            HI_MPI_STAT_Event(STAT_EVENT_VOGETFRM, 0);
        }

        AVPLAY_ProcFirstDispFrame(pAvplay);

        HI_INFO_AVPLAY("Play: queue frame to master win success!\n");

        memcpy(&pAvplay->LstFrmPack, &pAvplay->CurFrmPack, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));
        pAvplay->FrcCurPlayCnt++;
        pAvplay->DebugInfo.MasterVidStat.PlayNum++;
    }
    else if (HI_ERR_VO_BUFQUE_FULL != Ret)
    {
        HI_ERR_AVPLAY("Play: queue frame to master win failed %#x!\n", Ret);

        if (0 == pAvplay->FrcCurPlayCnt)
        {
            Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport,
                &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("Play: avplay release frame failed %#x!\n", Ret);
            }
        }

        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
        pAvplay->DebugInfo.MasterVidStat.DiscardNum++;
    }
    else
    {
        /* master window is full, do not send to slave window */
        HI_INFO_AVPLAY("Play: queue frame to master win, master win full!\n");

        pAvplay->AvplayProcVidContinue = HI_FALSE;

        if (0 == pAvplay->FrcCurPlayCnt)
        {
            pAvplay->bSendedFrmToMasterWin = HI_FALSE;
        }
        return;
    }

    for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

        for (j = 0; j < AVPLAY_MAX_SLAVE_FRMCHAN; j++)
        {
            if (hWindow == pAvplay->SlaveFrmChn[j].hWindow)
            {
                pAvplay->DebugInfo.SlaveVidStat[j].SendNum++;
                pAvplay->bSendedFrmToSlaveWin[j] = HI_TRUE;

                AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);

                Ret = HI_MPI_WIN_QueueFrame(hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                if (HI_SUCCESS == Ret)
                {
                    pAvplay->DebugInfo.SlaveVidStat[j].PlayNum++;
                }
                else if (HI_ERR_VO_BUFQUE_FULL != Ret)
                {
                    HI_WARN_AVPLAY("Master queue ok, slave queue failed, Ret=%#x!\n", Ret);

                    /*FrcCurPlayCnt maybe has add to 1, because master window send success!*/
                    if (0 == pAvplay->FrcCurPlayCnt || 1 == pAvplay->FrcCurPlayCnt)
                    {
                        Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport,
                            &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                        if (HI_SUCCESS != Ret)
                        {
                            HI_ERR_AVPLAY("avplay release frame failed %#x!\n", Ret);
                        }
                    }

                    pAvplay->DebugInfo.SlaveVidStat[j].DiscardNum++;
                }
                else
                {
                    pAvplay->AvplayProcVidContinue = HI_FALSE;

                    if (0 == pAvplay->FrcCurPlayCnt || 1 == pAvplay->FrcCurPlayCnt)
                    {
                        pAvplay->bSendedFrmToSlaveWin[j] = HI_FALSE;

                    }
                }
            }
        }
    }

    return;
}

HI_VOID AVPLAY_ProcVidQuickOutput(AVPLAY_S *pAvplay)
{
    HI_S32                              Ret;
    HI_U32                              i, j;
    HI_HANDLE                           hWindow = HI_INVALID_HANDLE;
    HI_LD_Event_S                       LdEvent;

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

        if (hWindow == pAvplay->MasterFrmChn.hWindow)
        {
            break;
        }
    }

    if (i == pAvplay->CurFrmPack.u32FrmNum)
    {
        return;
    }

    pAvplay->DebugInfo.MasterVidStat.SendNum++;
    pAvplay->bSendedFrmToMasterWin = HI_TRUE;

    AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);

    Ret = HI_MPI_WIN_QueueFrame(hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
    if (HI_SUCCESS == Ret)
    {
        /* record low delay event */
        LdEvent.evt_id = EVENT_AVPLAY_FRM_OUT;
        LdEvent.frame = pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32FrameIndex;
        LdEvent.handle = pAvplay->CurFrmPack.stFrame[i].stFrameVideo.hTunnelSrc;
        (HI_VOID)HI_SYS_GetTimeStampMs(&(LdEvent.time));

        (HI_VOID)HI_MPI_STAT_NotifyLowDelayEvent(&LdEvent);

        /* record program switch event */
        if (pAvplay->CurFrmPack.stFrame[i].stFrameVideo.bIsFirstIFrame)
        {
            HI_MPI_STAT_Event(STAT_EVENT_VOGETFRM, 0);
        }

        AVPLAY_ProcFirstDispFrame(pAvplay);

        memcpy(&pAvplay->LstFrmPack, &pAvplay->CurFrmPack, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));
        pAvplay->DebugInfo.MasterVidStat.PlayNum++;
        pAvplay->FrcCurPlayCnt++;
    }
    else if (HI_ERR_VO_BUFQUE_FULL != Ret)
    {
        HI_ERR_AVPLAY("Queue frame to master win failed %#x!\n", Ret);

        if (0 == pAvplay->FrcCurPlayCnt)
        {
            Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport,
                &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("avplay release frame failed %#x!\n", Ret);
            }
        }
        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
        pAvplay->DebugInfo.MasterVidStat.DiscardNum++;
    }
    else
    {
        /* master window is full, do not send to slave window */
        pAvplay->AvplayProcVidContinue = HI_FALSE;

        if (0 == pAvplay->FrcCurPlayCnt)
        {
            pAvplay->bSendedFrmToMasterWin = HI_FALSE;
        }
        return;
    }

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

        for (j=0; j<AVPLAY_MAX_SLAVE_FRMCHAN; j++)
        {
            if (hWindow == pAvplay->SlaveFrmChn[j].hWindow)
            {
                pAvplay->DebugInfo.SlaveVidStat[j].SendNum++;
                pAvplay->bSendedFrmToSlaveWin[j] = HI_TRUE;

                AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);

                Ret = HI_MPI_WIN_QueueFrame(hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                if (HI_SUCCESS == Ret)
                {
                    pAvplay->DebugInfo.SlaveVidStat[j].PlayNum++;
                }
                else if (HI_ERR_VO_BUFQUE_FULL != Ret)
                {
                    HI_ERR_AVPLAY("Master queue ok, slave queue failed, Ret=%#x!\n", Ret);

                    if (0 == pAvplay->FrcCurPlayCnt)
                    {
                        Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport,
                            &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                        if (HI_SUCCESS != Ret)
                        {
                           HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
                        }
                    }
                    pAvplay->DebugInfo.SlaveVidStat[j].DiscardNum++;
                }
                else
                {
                    pAvplay->AvplayProcVidContinue = HI_FALSE;

                    if (0 == pAvplay->FrcCurPlayCnt || 1 == pAvplay->FrcCurPlayCnt)
                    {
                        pAvplay->bSendedFrmToSlaveWin[j] = HI_FALSE;

                    }
                }
            }
        }
    }

    return;
}

HI_VOID AVPLAY_ProcVidRepeat(AVPLAY_S *pAvplay)
{
    HI_S32                              Ret;
    HI_U32                              i, j;
    HI_HANDLE                           hWindow = HI_INVALID_HANDLE;

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

        if (hWindow == pAvplay->MasterFrmChn.hWindow)
        {
            break;
        }
    }

    if (i == pAvplay->CurFrmPack.u32FrmNum)
    {
        return;
    }

    if (0 == pAvplay->LstFrmPack.u32FrmNum)
    {
        return;
    }

    if (pAvplay->CurFrmPack.stFrame[i].hport != pAvplay->LstFrmPack.stFrame[i].hport)
    {
        return;
    }

    pAvplay->DebugInfo.MasterVidStat.SendNum++;

    AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->LstFrmPack.stFrame[i].stFrameVideo);

    Ret = HI_MPI_WIN_QueueFrame(hWindow, &pAvplay->LstFrmPack.stFrame[i].stFrameVideo);
    if (HI_SUCCESS == Ret)
    {
        pAvplay->DebugInfo.MasterVidStat.RepeatNum++;
        HI_INFO_AVPLAY("Repeat: Queue frame to master win success!\n");
    }
    else if (HI_ERR_VO_BUFQUE_FULL != Ret)
    {
        HI_INFO_AVPLAY("Repeat: Queue frame to master win falied 0x%x!\n", Ret);
        return;
    }
    else
    {
        pAvplay->AvplayProcVidContinue = HI_FALSE;
        return;
    }

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

        for (j=0; j<AVPLAY_MAX_SLAVE_FRMCHAN; j++)
        {
            if (hWindow == pAvplay->SlaveFrmChn[j].hWindow)
            {
                if (pAvplay->CurFrmPack.stFrame[i].hport != pAvplay->LstFrmPack.stFrame[i].hport)
                {
                    continue;
                }

                pAvplay->DebugInfo.SlaveVidStat[j].SendNum++;

                AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->LstFrmPack.stFrame[i].stFrameVideo);

                Ret = HI_MPI_WIN_QueueFrame(hWindow, &pAvplay->LstFrmPack.stFrame[i].stFrameVideo);
                if (HI_SUCCESS == Ret)
                {
                    pAvplay->DebugInfo.SlaveVidStat[j].RepeatNum++;
                }
                else if (HI_ERR_VO_BUFQUE_FULL != Ret)
                {
                    HI_INFO_AVPLAY("Sync repeat, queue last frame to slave win failed, Ret=%#x!\n", Ret);
                }
                else
                {
                    pAvplay->AvplayProcVidContinue = HI_FALSE;
                }
            }
        }
    }

    return;
}

HI_VOID AVPLAY_ProcVidDiscard(AVPLAY_S *pAvplay)
{
    HI_S32                              Ret;
    HI_U32                              i, j;
    HI_HANDLE                           hWindow = HI_INVALID_HANDLE;

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

        if (hWindow == pAvplay->MasterFrmChn.hWindow)
        {
            break;
        }
    }

    if (i == pAvplay->CurFrmPack.u32FrmNum)
    {
        return;
    }

    Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
    if (HI_SUCCESS == Ret)
    {
        HI_INFO_AVPLAY("Discard, avplay release frame success!\n");
        pAvplay->FrcCurPlayCnt = pAvplay->FrcNeedPlayCnt;
        pAvplay->DebugInfo.MasterVidStat.DiscardNum++;
    }
    else
    {
        HI_ERR_AVPLAY("Discard, avplay release frame fail 0x%x!\n", Ret);
        return;
    }

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

        for (j=0; j<AVPLAY_MAX_SLAVE_FRMCHAN; j++)
        {
            if (hWindow == pAvplay->SlaveFrmChn[j].hWindow)
            {
                Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("Discard, avplay release frame fail %#x!\n", Ret);
                }
                pAvplay->DebugInfo.SlaveVidStat[j].DiscardNum++;
            }
        }
    }
    return;
}

HI_S32 AVPLAY_WindowLoad(HI_HANDLE hWin, HI_BOOL WinLastState)
{
    HI_S32  Ret;

    if (!WinLastState)
    {
        return HI_SUCCESS;
    }

    Ret = HI_MPI_WIN_SetEnable(hWin, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_WIN_SetEnable 0x%x\n", Ret);
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_WindowUnload(HI_HANDLE hWin, HI_BOOL *pWinCurState)
{
    HI_S32  Ret;
    HI_BOOL WinRunState;

    Ret = HI_MPI_WIN_GetEnable(hWin, &WinRunState);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_WIN_GetEnable 0x%x\n", Ret);
        return Ret;
    }

    if (!WinRunState)
    {
        *pWinCurState = WinRunState;
        return HI_SUCCESS;
    }

    Ret = HI_MPI_WIN_SetEnable(hWin, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_WIN_SetEnable 0x%x\n", Ret);
        return Ret;
    }

    *pWinCurState = WinRunState;

    return HI_SUCCESS;
}

#ifdef HI_HDR_SUPPORT
HI_S32 AVPLAY_GetBlFramePort(AVPLAY_S *pAvplay, HI_HANDLE hPort, HI_U32 *PortId)
{
    HI_U32              i;

    for (i = 0; i < pAvplay->CurBlFrmPack.u32FrmNum; i++)
    {
        if (pAvplay->CurBlFrmPack.stFrame[i].hport == hPort)
        {
            break;
        }
    }

    if (i == pAvplay->CurBlFrmPack.u32FrmNum)
    {
        return HI_FAILURE;
    }

    *PortId = i;
    return HI_SUCCESS;
}

HI_S32 AVPLAY_GetElFramePort(AVPLAY_S *pAvplay, HI_HANDLE hPort, HI_U32 *PortId)
{
    HI_U32              i;

    for (i = 0; i < pAvplay->CurElFrmPack.u32FrmNum; i++)
    {
        if (pAvplay->CurElFrmPack.stFrame[i].hport == hPort)
        {
            break;
        }
    }

    if (i == pAvplay->CurElFrmPack.u32FrmNum)
    {
        return HI_FAILURE;
    }

    *PortId = i;
    return HI_SUCCESS;
}

HI_S32 AVPLAY_RelHDRChnFrame(AVPLAY_S *pAvplay)
{
    HI_S32                              Ret;
    HI_U32                              i;
    HI_U32                              BlPortId = -1;
    HI_U32                              ElPortId = -1;

    if (pAvplay->bDualChnPlay)
    {
        Ret = AVPLAY_GetBlFramePort(pAvplay, pAvplay->hBlPort, &BlPortId);
        if (HI_SUCCESS != Ret)
        {
            HI_WARN_AVPLAY("AVPLAY_GetBlFramePort failed\n");
        }

        Ret = AVPLAY_GetElFramePort(pAvplay, pAvplay->hElPort, &ElPortId);
        if (HI_SUCCESS != Ret)
        {
            HI_WARN_AVPLAY("AVPLAY_GetElFramePort failed\n");
        }

        if (HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
        {
            if (HI_FALSE == pAvplay->bSendedFrmToMasterWin)
            {
                if ( (HI_INVALID_HANDLE != pAvplay->hVdec) && (-1 != BlPortId) )
                {
                    Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->hBlPort, &pAvplay->CurBlFrmPack.stFrame[BlPortId].stFrameVideo);
                    if (HI_SUCCESS != Ret)
                    {
                        HI_ERR_AVPLAY("avplay release Bl frame failed\n", Ret);
                    }
                    memset(&pAvplay->LstBlFrmPack, 0, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));
                }

                if ( (HI_INVALID_HANDLE != pAvplay->hElVdec) && (-1 != ElPortId) )
                {
                    Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->hElPort, &pAvplay->CurElFrmPack.stFrame[ElPortId].stFrameVideo);
                    if (HI_SUCCESS != Ret)
                    {
                        HI_ERR_AVPLAY("avplay release El frame failed\n", Ret);
                    }
                    memset(&pAvplay->LstElFrmPack, 0, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));
                }

                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
            }
        }
        else
        {
            if ( (HI_TRUE == pAvplay->bBlFrmProcFlag) && (-1 != BlPortId) )
            {
                Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->hBlPort, &pAvplay->CurBlFrmPack.stFrame[BlPortId].stFrameVideo);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("avplay release Bl frame failed\n", Ret);
                }

                pAvplay->bBlFrmProcFlag = HI_FALSE;
                memset(&pAvplay->LstBlFrmPack, 0, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));
            }

            if ( (HI_TRUE == pAvplay->bElFrmProcFlag) && (-1 != ElPortId) )
            {
                Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->hElPort, &pAvplay->CurElFrmPack.stFrame[ElPortId].stFrameVideo);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("avplay release El frame failed\n", Ret);
                }

                pAvplay->bElFrmProcFlag = HI_FALSE;
                memset(&pAvplay->LstElFrmPack, 0, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));
            }
        }
    }
    else
    {
        if (pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] && !pAvplay->bSendedFrmToMasterWin)
        {
            for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
            {
                if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
                {
                    Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                    if (HI_SUCCESS != Ret)
                    {
                        HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
                    }

                    pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
                    memset(&pAvplay->CurFrmPack, 0, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));
                }
            }
        }
    }

    return HI_SUCCESS;
}

HI_VOID AVPLAY_ProcHDRPlay(AVPLAY_S *pAvplay)
{
    HI_S32             Ret;
    HI_U32             i;
    HI_U32             BlPortId;
    HI_U32             ElPortId;

    if (HI_TRUE == pAvplay->bDualChnPlay)
    {
        Ret = AVPLAY_GetBlFramePort(pAvplay, pAvplay->hBlPort, &BlPortId);
        Ret|= AVPLAY_GetElFramePort(pAvplay, pAvplay->hElPort, &ElPortId);
        if (HI_SUCCESS != Ret)
        {
            return;
        }

        pAvplay->DebugInfo.MasterVidStat.SendNum++;

        pAvplay->bSendedFrmToMasterWin = HI_TRUE;

        AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->CurBlFrmPack.stFrame[BlPortId].stFrameVideo);
        AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->CurElFrmPack.stFrame[ElPortId].stFrameVideo);

        Ret = HI_MPI_WIN_QueueDolbyFrame(
            pAvplay->MasterFrmChn.hWindow,
            &pAvplay->CurBlFrmPack.stFrame[BlPortId].stFrameVideo,
            &pAvplay->CurElFrmPack.stFrame[ElPortId].stFrameVideo);
        if (HI_SUCCESS == Ret)
        {
            if (pAvplay->CurBlFrmPack.stFrame[BlPortId].stFrameVideo.bIsFirstIFrame)
            {
                HI_MPI_STAT_Event(STAT_EVENT_VOGETFRM, 0);
            }

            AVPLAY_ProcFirstDispFrame(pAvplay);

            HI_INFO_AVPLAY("Play: queue frame to master win success!\n");

            memcpy(&pAvplay->LstBlFrmPack, &pAvplay->CurBlFrmPack, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));
            memcpy(&pAvplay->LstElFrmPack, &pAvplay->CurElFrmPack, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));

            pAvplay->FrcCurPlayCnt++;
            pAvplay->DebugInfo.MasterVidStat.PlayNum++;

        }
        else if (HI_ERR_VO_BUFQUE_FULL != Ret)
        {
            if (0 == pAvplay->FrcCurPlayCnt)
            {
                Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->hBlPort, &pAvplay->CurBlFrmPack.stFrame[BlPortId].stFrameVideo);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("avplay release Bl frame failed 0x%x\n", Ret);
                }

                Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->hElPort, &pAvplay->CurElFrmPack.stFrame[ElPortId].stFrameVideo);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("avplay release El frame failed 0x%x\n", Ret);
                }
            }

            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
            pAvplay->DebugInfo.MasterVidStat.DiscardNum++;
        }
        else
        {
            HI_INFO_AVPLAY("Play: queue frame to master win, master win full!\n");

            pAvplay->AvplayProcVidContinue = HI_FALSE;

            if (0 == pAvplay->FrcCurPlayCnt)
            {
                pAvplay->bSendedFrmToMasterWin = HI_FALSE;
            }
            return;
        }
    }
    else
    {
        for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
        {
            if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
            {
                break;
            }
        }

        if (i == pAvplay->CurFrmPack.u32FrmNum)
        {
            return;
        }

        pAvplay->DebugInfo.MasterVidStat.SendNum++;

        pAvplay->bSendedFrmToMasterWin = HI_TRUE;

        AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);

        Ret = HI_MPI_WIN_QueueDolbyFrame(
            pAvplay->MasterFrmChn.hWindow,
            &pAvplay->CurFrmPack.stFrame[i].stFrameVideo,
            HI_NULL);
        if (HI_SUCCESS == Ret)
        {
            if (pAvplay->CurFrmPack.stFrame[i].stFrameVideo.bIsFirstIFrame)
            {
                HI_MPI_STAT_Event(STAT_EVENT_VOGETFRM, 0);
            }

            HI_INFO_AVPLAY("Play: queue frame to master win success!\n");

            memcpy(&pAvplay->LstFrmPack, &pAvplay->CurFrmPack, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));

            pAvplay->FrcCurPlayCnt++;
            pAvplay->DebugInfo.MasterVidStat.PlayNum++;

        }
        else if (HI_ERR_VO_BUFQUE_FULL != Ret)
        {
            if (0 == pAvplay->FrcCurPlayCnt)
            {
                Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
                }
            }

            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
            pAvplay->DebugInfo.MasterVidStat.DiscardNum++;
        }
        else
        {
            HI_INFO_AVPLAY("Play: queue frame to master win, master win full!\n");

            pAvplay->AvplayProcVidContinue = HI_FALSE;

            if (0 == pAvplay->FrcCurPlayCnt)
            {
                pAvplay->bSendedFrmToMasterWin = HI_FALSE;
            }
            return;
        }
    }

    return;
}

HI_VOID AVPLAY_ProcHDRQuickOutput(AVPLAY_S *pAvplay)
{
    HI_S32             Ret;
    HI_U32             i;
    HI_U32             BlPortId;
    HI_U32             ElPortId;

    if (HI_TRUE == pAvplay->bDualChnPlay)
    {
        Ret = AVPLAY_GetBlFramePort(pAvplay, pAvplay->hBlPort, &BlPortId);
        Ret|= AVPLAY_GetElFramePort(pAvplay, pAvplay->hElPort, &ElPortId);
        if (HI_SUCCESS != Ret)
        {
            return;
        }

        pAvplay->DebugInfo.MasterVidStat.SendNum++;
        pAvplay->bSendedFrmToMasterWin = HI_TRUE;

        AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->CurBlFrmPack.stFrame[BlPortId].stFrameVideo);
        AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->CurElFrmPack.stFrame[ElPortId].stFrameVideo);

        Ret = HI_MPI_WIN_QueueDolbyFrame(
            pAvplay->MasterFrmChn.hWindow,
            &pAvplay->CurBlFrmPack.stFrame[BlPortId].stFrameVideo,
            &pAvplay->CurElFrmPack.stFrame[ElPortId].stFrameVideo);
        if (HI_SUCCESS == Ret)
        {
            /* record program switch event */
            if (pAvplay->CurFrmPack.stFrame[BlPortId].stFrameVideo.bIsFirstIFrame)
            {
                HI_MPI_STAT_Event(STAT_EVENT_VOGETFRM, 0);
            }

            AVPLAY_ProcFirstDispFrame(pAvplay);

            memcpy(&pAvplay->LstBlFrmPack, &pAvplay->CurBlFrmPack, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));
            memcpy(&pAvplay->LstElFrmPack, &pAvplay->CurElFrmPack, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));

            pAvplay->DebugInfo.MasterVidStat.PlayNum++;
            pAvplay->FrcCurPlayCnt++;
        }
        else if (HI_ERR_VO_BUFQUE_FULL != Ret)
        {
            HI_ERR_AVPLAY("Queue frame to master win failed, Ret=%#x!\n", Ret);

            if (0 == pAvplay->FrcCurPlayCnt)
            {
                Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->hBlPort, &pAvplay->CurBlFrmPack.stFrame[BlPortId].stFrameVideo);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("avplay release Bl frame failed 0x%x\n", Ret);
                }

                Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->hElPort, &pAvplay->CurElFrmPack.stFrame[ElPortId].stFrameVideo);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("avplay release El frame failed 0x%x\n", Ret);
                }
            }

            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
            pAvplay->DebugInfo.MasterVidStat.DiscardNum++;
        }
        else
        {
            pAvplay->AvplayProcVidContinue = HI_FALSE;

            if (0 == pAvplay->FrcCurPlayCnt)
            {
                pAvplay->bSendedFrmToMasterWin = HI_FALSE;
            }
            return;
        }
    }
    else
    {
        for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
        {
            if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
            {
                break;
            }
        }

        if (i == pAvplay->CurFrmPack.u32FrmNum)
        {
            return;
        }

        pAvplay->DebugInfo.MasterVidStat.SendNum++;
        pAvplay->bSendedFrmToMasterWin = HI_TRUE;

        AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);

        Ret = HI_MPI_WIN_QueueDolbyFrame(
            pAvplay->MasterFrmChn.hWindow,
            &pAvplay->CurFrmPack.stFrame[i].stFrameVideo,
            HI_NULL);
        if (HI_SUCCESS == Ret)
        {
            memcpy(&pAvplay->LstFrmPack, &pAvplay->CurFrmPack, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));

            pAvplay->DebugInfo.MasterVidStat.PlayNum++;
            pAvplay->FrcCurPlayCnt++;
        }
        else if (HI_ERR_VO_BUFQUE_FULL != Ret)
        {
            HI_ERR_AVPLAY("Queue frame to master win failed, Ret=%#x!\n", Ret);

            if (0 == pAvplay->FrcCurPlayCnt)
            {
                Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
                }
            }
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
            pAvplay->DebugInfo.MasterVidStat.DiscardNum++;
        }
        else
        {
            pAvplay->AvplayProcVidContinue = HI_FALSE;

            if (0 == pAvplay->FrcCurPlayCnt)
            {
                pAvplay->bSendedFrmToMasterWin = HI_FALSE;
            }
            return;
        }
    }

    return;
}

HI_VOID AVPLAY_ProcHDRRepeat(AVPLAY_S *pAvplay)
{
    HI_S32             Ret;
    HI_U32             i;

    if (HI_TRUE == pAvplay->bDualChnPlay)
    {
        HI_U32  BlPortId = 0;
        HI_U32  ElPortId = 0;

        Ret = AVPLAY_GetBlFramePort(pAvplay, pAvplay->hBlPort, &BlPortId);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("call AVPLAY_GetBlFramePort failed\n");
            return;
        }

        Ret = AVPLAY_GetElFramePort(pAvplay, pAvplay->hElPort, &ElPortId);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("call AVPLAY_GetElFramePort failed\n");
            return;
        }

        if ( (0 == pAvplay->LstBlFrmPack.u32FrmNum) || (0 == pAvplay->LstElFrmPack.u32FrmNum) )
        {
            return;
        }

        if (   (pAvplay->CurBlFrmPack.stFrame[BlPortId].hport != pAvplay->LstBlFrmPack.stFrame[BlPortId].hport)
            || (pAvplay->CurElFrmPack.stFrame[ElPortId].hport != pAvplay->LstElFrmPack.stFrame[ElPortId].hport) )
        {
            return;
        }

        pAvplay->DebugInfo.MasterVidStat.SendNum++;

        AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->LstBlFrmPack.stFrame[BlPortId].stFrameVideo);
        AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->LstElFrmPack.stFrame[ElPortId].stFrameVideo);

        Ret = HI_MPI_WIN_QueueDolbyFrame(
            pAvplay->MasterFrmChn.hWindow,
            &pAvplay->LstBlFrmPack.stFrame[BlPortId].stFrameVideo,
            &pAvplay->LstElFrmPack.stFrame[ElPortId].stFrameVideo
            );
        if (HI_SUCCESS == Ret)
        {
            pAvplay->DebugInfo.MasterVidStat.RepeatNum++;
            HI_INFO_AVPLAY("Repeat: Queue frame to master win success!\n");
        }
        else if (HI_ERR_VO_BUFQUE_FULL != Ret)
        {
            HI_INFO_AVPLAY("Repeat, queue last frame to master win failed %#x!\n", Ret);
        }
        else
        {
            pAvplay->AvplayProcVidContinue = HI_FALSE;
        }
    }
    else
    {
        if (0 == pAvplay->LstFrmPack.u32FrmNum)
        {
            return;
        }

        for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
        {
            if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
            {
                break;
            }
        }

        if (i == pAvplay->CurFrmPack.u32FrmNum)
        {
            return;
        }

        if (pAvplay->CurFrmPack.stFrame[i].hport != pAvplay->LstFrmPack.stFrame[i].hport)
        {
            return;
        }

        pAvplay->DebugInfo.MasterVidStat.SendNum++;

        AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->LstFrmPack.stFrame[i].stFrameVideo);

        Ret = HI_MPI_WIN_QueueDolbyFrame(
            pAvplay->MasterFrmChn.hWindow,
            &pAvplay->LstFrmPack.stFrame[i].stFrameVideo,
            HI_NULL
            );
        if (HI_SUCCESS == Ret)
        {
            pAvplay->DebugInfo.MasterVidStat.RepeatNum++;
            HI_INFO_AVPLAY("Repeat: Queue frame to master win success!\n");
        }
        else if (HI_ERR_VO_BUFQUE_FULL != Ret)
        {
            HI_INFO_AVPLAY("Repeat, queue last frame to master win failed %#x!\n", Ret);
        }
        else
        {
            pAvplay->AvplayProcVidContinue = HI_FALSE;
        }
    }

    return;
}

HI_VOID AVPLAY_ProcHDRDiscard(AVPLAY_S *pAvplay)
{
    HI_S32             Ret;
    HI_U32             i;
    HI_U32             BlPortId;
    HI_U32             ElPortId;

    if (HI_TRUE == pAvplay->bDualChnPlay)
    {
        Ret = AVPLAY_GetBlFramePort(pAvplay, pAvplay->hBlPort, &BlPortId);
        Ret|= AVPLAY_GetElFramePort(pAvplay, pAvplay->hElPort, &ElPortId);
        if (HI_SUCCESS != Ret)
        {
            return;
        }

        pAvplay->DebugInfo.MasterVidStat.SendNum++;

        Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurBlFrmPack.stFrame[BlPortId].hport,
            &pAvplay->CurBlFrmPack.stFrame[BlPortId].stFrameVideo);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
            return;
        }

        Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurElFrmPack.stFrame[ElPortId].hport,
            &pAvplay->CurElFrmPack.stFrame[ElPortId].stFrameVideo);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
            return;
        }

        HI_INFO_AVPLAY("Discard, avplay release frame success!\n");
        pAvplay->FrcCurPlayCnt = pAvplay->FrcNeedPlayCnt;
        pAvplay->DebugInfo.MasterVidStat.DiscardNum++;

    }
    else
    {
        for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
        {
            if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
            {
                break;
            }
        }

        if (i == pAvplay->CurFrmPack.u32FrmNum)
        {
            return;
        }

        Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport,
            &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
            return;
        }

        HI_INFO_AVPLAY("Discard, avplay release frame success!\n");
        pAvplay->FrcCurPlayCnt = pAvplay->FrcNeedPlayCnt;
        pAvplay->DebugInfo.MasterVidStat.DiscardNum++;
    }

    return;
}

HI_VOID AVPLAY_QueueHDRFrame(AVPLAY_S *pAvplay)
{
    if (   (0 == pAvplay->FrcNeedPlayCnt)
        || (pAvplay->FrcCurPlayCnt < pAvplay->FrcNeedPlayCnt) )
    {
        if (SYNC_PROC_PLAY == pAvplay->VidOpt.SyncProc)
        {
            AVPLAY_ProcHDRPlay(pAvplay);
        }
        else if (SYNC_PROC_REPEAT == pAvplay->VidOpt.SyncProc)
        {
            AVPLAY_ProcHDRRepeat(pAvplay);
        }
        else if (SYNC_PROC_DISCARD == pAvplay->VidOpt.SyncProc)
        {
            AVPLAY_ProcHDRDiscard(pAvplay);
        }
        else if (SYNC_PROC_QUICKOUTPUT == pAvplay->VidOpt.SyncProc)
        {
            AVPLAY_ProcHDRQuickOutput(pAvplay);
        }
    }

    if (pAvplay->FrcCurPlayCnt >= pAvplay->FrcNeedPlayCnt)
    {
        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
    }

    return;
}

HI_VOID AVPLAY_ProcElVidSync(AVPLAY_S *pAvplay)
{
    HI_S32                Ret;
    HI_U32                ElPortId;

    Ret = AVPLAY_GetElFramePort(pAvplay, pAvplay->hElPort, &ElPortId);
    if (HI_SUCCESS != Ret)
    {
        return;
    }

    pAvplay->CurElPts = pAvplay->CurElFrmPack.stFrame[ElPortId].stFrameVideo.u32Pts;

    HI_INFO_AVPLAY("[EL] ElPts:%d\n", pAvplay->CurElPts);

    if (HI_INVALID_PTS == pAvplay->CurElPts)
    {
        pAvplay->ElSyncProc = AVPLAY_HDR_SYNC_PROC_PLAY;
        return;
    }

    if (HI_FALSE == pAvplay->HDRSyncTargetInit)
    {
        HI_INFO_AVPLAY("[EL] HDRSyncTargetInit.\n");

        pAvplay->HDRSyncTargetTime = pAvplay->CurElPts;
        pAvplay->HDRSyncTargetInit = HI_TRUE;
        pAvplay->HDRSyncTarget = AVPLAY_HDR_CHAN_EL;
        pAvplay->ElSyncProc = AVPLAY_HDR_SYNC_PROC_REPEAT;

        return;
    }

    if (HI_INVALID_PTS == pAvplay->CurBlPts)
    {
        return;
    }

    if (AVPLAY_HDR_CHAN_BL == pAvplay->HDRSyncTarget)
    {
        if (pAvplay->CurElPts == pAvplay->HDRSyncTargetTime)
        {
            pAvplay->ElSyncProc = AVPLAY_HDR_SYNC_PROC_PLAY;
            pAvplay->BlSyncProc = AVPLAY_HDR_SYNC_PROC_PLAY;
        }
        else if (pAvplay->CurElPts < pAvplay->HDRSyncTargetTime)
        {
            pAvplay->ElSyncProc = AVPLAY_HDR_SYNC_PROC_DISCARD;
        }
        else if (pAvplay->CurElPts > pAvplay->HDRSyncTargetTime)
        {
            pAvplay->HDRSyncTarget = AVPLAY_HDR_CHAN_EL;
            pAvplay->HDRSyncTargetTime = pAvplay->CurElPts;
            pAvplay->ElSyncProc = AVPLAY_HDR_SYNC_PROC_REPEAT;
        }
    }
    else if (AVPLAY_HDR_CHAN_EL == pAvplay->HDRSyncTarget)
    {
        if (pAvplay->BlSyncProc == AVPLAY_HDR_SYNC_PROC_PLAY)
        {
            pAvplay->ElSyncProc = AVPLAY_HDR_SYNC_PROC_PLAY;
        }
        else
        {
            pAvplay->ElSyncProc = AVPLAY_HDR_SYNC_PROC_REPEAT;
        }
    }

    HI_INFO_AVPLAY("[EL] SyncProc:%d, HDRSyncTargetTime:%d, HDRSyncTarget:%d, BlPts:%d, ElPts:%d, SysTime:%u\n",
        pAvplay->ElSyncProc, pAvplay->HDRSyncTargetTime,pAvplay->HDRSyncTarget, pAvplay->CurBlPts, pAvplay->CurElPts, AVPLAY_GetSysTime());

    return;
}

HI_VOID AVPLAY_ProcBlVidSync(AVPLAY_S *pAvplay)
{
    HI_S32                Ret;
    HI_U32                BlPortId;

    Ret = AVPLAY_GetBlFramePort(pAvplay, pAvplay->hBlPort, &BlPortId);
    if (HI_SUCCESS != Ret)
    {
        return;
    }

    pAvplay->CurBlPts = pAvplay->CurBlFrmPack.stFrame[BlPortId].stFrameVideo.u32Pts;

    HI_INFO_AVPLAY("[BL] BlPts:%d\n", pAvplay->CurBlPts);

    if (HI_INVALID_PTS == pAvplay->CurBlPts)
    {
        pAvplay->BlSyncProc = AVPLAY_HDR_SYNC_PROC_PLAY;
        return;
    }

    if (HI_FALSE == pAvplay->HDRSyncTargetInit)
    {
        HI_INFO_AVPLAY("[BL] HDRSyncTargetInit.\n");

        pAvplay->HDRSyncTargetTime = pAvplay->CurBlPts;
        pAvplay->HDRSyncTargetInit = HI_TRUE;
        pAvplay->HDRSyncTarget = AVPLAY_HDR_CHAN_BL;
        pAvplay->BlSyncProc = AVPLAY_HDR_SYNC_PROC_REPEAT;

        return;
    }

    if (HI_INVALID_PTS == pAvplay->CurElPts)
    {
        return;
    }

    if (AVPLAY_HDR_CHAN_EL == pAvplay->HDRSyncTarget)
    {
        if (pAvplay->CurBlPts == pAvplay->HDRSyncTargetTime)
        {
            pAvplay->BlSyncProc = AVPLAY_HDR_SYNC_PROC_PLAY;
            pAvplay->ElSyncProc = AVPLAY_HDR_SYNC_PROC_PLAY;
        }
        else if (pAvplay->CurBlPts < pAvplay->HDRSyncTargetTime)
        {
            pAvplay->BlSyncProc = AVPLAY_HDR_SYNC_PROC_DISCARD;
        }
        else if (pAvplay->CurBlPts > pAvplay->HDRSyncTargetTime)
        {
            pAvplay->HDRSyncTarget = AVPLAY_HDR_CHAN_BL;
            pAvplay->HDRSyncTargetTime = pAvplay->CurBlPts;
            pAvplay->BlSyncProc = AVPLAY_HDR_SYNC_PROC_REPEAT;
        }
    }
    else if (AVPLAY_HDR_CHAN_BL == pAvplay->HDRSyncTarget)
    {
        if (pAvplay->ElSyncProc == AVPLAY_HDR_SYNC_PROC_PLAY)
        {
            pAvplay->BlSyncProc = AVPLAY_HDR_SYNC_PROC_PLAY;
        }
        else
        {
            pAvplay->BlSyncProc = AVPLAY_HDR_SYNC_PROC_REPEAT;
        }
    }

    HI_INFO_AVPLAY("[BL] SyncProc:%d, HDRSyncTargetTime:%d, HDRSyncTarget:%d, BlPts:%d, ElPts:%d, SysTime:%u\n",
        pAvplay->BlSyncProc, pAvplay->HDRSyncTargetTime,pAvplay->HDRSyncTarget, pAvplay->CurBlPts, pAvplay->CurElPts, AVPLAY_GetSysTime());

    return;
}

HI_VOID AVPLAY_ProcBlFrame(AVPLAY_S *pAvplay)
{
    HI_S32                Ret;
    HI_U32                PortId;

    if ((AVPLAY_HDR_SYNC_PROC_PLAY == pAvplay->BlSyncProc)
      &&(AVPLAY_HDR_SYNC_PROC_PLAY == pAvplay->ElSyncProc))
    {
        return;
    }

    if (!pAvplay->bBlFrmProcFlag)
    {
        Ret = HI_MPI_VDEC_ReceiveFrame(pAvplay->hVdec, &pAvplay->CurBlFrmPack, AVPLAY_RECVFRAME_MAX_TIME);
        if (HI_SUCCESS != Ret)
        {
            return;
        }
    }

    AVPLAY_ProcBlVidSync(pAvplay);

    if (AVPLAY_HDR_SYNC_PROC_PLAY == pAvplay->BlSyncProc)
    {
        pAvplay->bBlFrmProcFlag = HI_FALSE;
    }
    else if (AVPLAY_HDR_SYNC_PROC_REPEAT == pAvplay->BlSyncProc)
    {
        pAvplay->bBlFrmProcFlag = HI_TRUE;
    }
    else if (AVPLAY_HDR_SYNC_PROC_DISCARD == pAvplay->BlSyncProc)
    {
        Ret = AVPLAY_GetBlFramePort(pAvplay, pAvplay->hBlPort, &PortId);
        if (HI_SUCCESS == Ret)
        {
            Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->hBlPort, &pAvplay->CurBlFrmPack.stFrame[PortId].stFrameVideo);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
            }
        }

        pAvplay->bBlFrmProcFlag = HI_FALSE;
    }

    return;
}

HI_VOID AVPLAY_ProcElFrame(AVPLAY_S *pAvplay)
{
    HI_S32                Ret;
    HI_U32                PortId;

    if ((AVPLAY_HDR_SYNC_PROC_PLAY == pAvplay->BlSyncProc)
      &&(AVPLAY_HDR_SYNC_PROC_PLAY == pAvplay->ElSyncProc))
    {
        return;
    }

    if (!pAvplay->bElFrmProcFlag)
    {
        Ret = HI_MPI_VDEC_ReceiveFrame(pAvplay->hElVdec, &pAvplay->CurElFrmPack, AVPLAY_RECVFRAME_MAX_TIME);
        if (HI_SUCCESS != Ret)
        {
            return;
        }
    }

    AVPLAY_ProcElVidSync(pAvplay);

    if (AVPLAY_HDR_SYNC_PROC_PLAY == pAvplay->ElSyncProc)
    {
        pAvplay->bElFrmProcFlag = HI_FALSE;
    }
    else if (AVPLAY_HDR_SYNC_PROC_REPEAT == pAvplay->ElSyncProc)
    {
        pAvplay->bElFrmProcFlag = HI_TRUE;
    }
    else if (AVPLAY_HDR_SYNC_PROC_DISCARD == pAvplay->ElSyncProc)
    {
        Ret = AVPLAY_GetElFramePort(pAvplay, pAvplay->hElPort, &PortId);
        if (HI_SUCCESS == Ret)
        {
            Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->hElPort, &pAvplay->CurElFrmPack.stFrame[PortId].stFrameVideo);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("avplay release El frame failed 0x%x\n", Ret);
            }
        }

        pAvplay->bElFrmProcFlag = HI_FALSE;
    }

    return;
}

HI_S32 AVPLAY_FreeHDRChn(AVPLAY_S *pAvplay, AVPLAY_HDR_CHAN_E HDRChn)
{
    HI_S32                      Ret = HI_SUCCESS;

    AVPLAY_HDR_CHN_CHECK(HDRChn);

    if (HDRChn & ((HI_U32)AVPLAY_HDR_CHAN_BL))
    {
        Ret = HI_MPI_VDEC_ChanBufferDeInit(pAvplay->hVdec);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanBufferDeInit failed 0x%x\n", Ret);
            return Ret;
        }

        Ret = HI_MPI_VDEC_FreeChan(pAvplay->hVdec);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_FreeChan failed 0x%x\n", Ret);
            return Ret;
        }

        pAvplay->hVdec = HI_INVALID_HANDLE;
    }

    if (HDRChn & ((HI_U32)AVPLAY_HDR_CHAN_EL))
    {
        if (pAvplay->hElPort != HI_INVALID_HANDLE)
        {
            Ret = HI_MPI_VDEC_DestroyPort(pAvplay->hElVdec, pAvplay->hElPort);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("HI_MPI_VDEC_DestroyPort failed 0x%x\n", Ret);
                return Ret;
            }

            pAvplay->hElPort = HI_INVALID_HANDLE;
        }

        if (pAvplay->hElVdec != HI_INVALID_HANDLE)
        {
            Ret = HI_MPI_VDEC_ChanBufferDeInit(pAvplay->hElVdec);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("HI_MPI_VDEC_ChanBufferDeInit failed 0x%x\n", Ret);
                return Ret;
            }

            Ret = HI_MPI_VDEC_FreeChan(pAvplay->hElVdec);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("HI_MPI_VDEC_FreeChan failed 0x%x\n", Ret);
                return Ret;
            }

            pAvplay->hElVdec = HI_INVALID_HANDLE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_AllocHDRChn(AVPLAY_S *pAvplay, AVPLAY_HDR_CHAN_E HDRChn)
{
    HI_S32                  Ret = HI_SUCCESS;
    HI_HANDLE               VdecHandle;

    AVPLAY_HDR_CHN_CHECK(HDRChn);

    if (HDRChn & ((HI_U32)AVPLAY_HDR_CHAN_EL))
    {
        Ret = HI_MPI_VDEC_AllocChan(&VdecHandle, &pAvplay->AvplayHDRAttr.stElCodecOpt);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_AllocChan failed 0x%x\n", Ret);
            return Ret;
        }

        Ret = AVPLAY_MallocVidBuffer(pAvplay, VdecHandle);
        if (Ret != HI_SUCCESS)
        {
            (HI_VOID)HI_MPI_VDEC_FreeChan(VdecHandle);
            return Ret;
        }

        pAvplay->hElVdec = VdecHandle;
    }

    return Ret;
}

HI_S32 AVPLAY_StartHDRChn(const AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

    Ret = HI_MPI_VDEC_ChanStart(pAvplay->hVdec);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_ChanStart failed 0x%x\n", Ret);
        return Ret;
    }

    if (pAvplay->bDualChnPlay)
    {
        Ret = HI_MPI_VDEC_ChanStart(pAvplay->hElVdec);
        if (HI_SUCCESS != Ret)
        {
            (HI_VOID)HI_MPI_VDEC_ChanStop(pAvplay->hVdec);
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanStart failed 0x%x\n", Ret);
            return Ret;
        }
    }

    Ret = HI_MPI_SYNC_Start(pAvplay->hSync, SYNC_CHAN_VID);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_SYNC_Start failed 0x%x\n", Ret);
        goto ERR_VDEC_CHAN_STOP;
    }

    return HI_SUCCESS;

ERR_VDEC_CHAN_STOP:

    (HI_VOID)HI_MPI_VDEC_ChanStop(pAvplay->hVdec);

    if (pAvplay->bDualChnPlay)
    {
        (HI_VOID)HI_MPI_VDEC_ChanStop(pAvplay->hElVdec);
    }

    return HI_FAILURE;
}

HI_S32 AVPLAY_StopHDRChn(AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

    Ret = HI_MPI_VDEC_ChanStop(pAvplay->hVdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_ChanStop failed 0x%x\n", Ret);
        return Ret;
    }

    Ret = HI_MPI_VDEC_ResetChan(pAvplay->hVdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_ResetChan failed 0x%x\n", Ret);
        return Ret;
    }

    if (pAvplay->bDualChnPlay)
    {
        Ret = HI_MPI_VDEC_ChanStop(pAvplay->hElVdec);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanStop failed 0x%x\n", Ret);
            return Ret;
        }

        Ret = HI_MPI_VDEC_ResetChan(pAvplay->hElVdec);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ResetChan failed 0x%x\n", Ret);
            return Ret;
        }

        pAvplay->HDRSyncTargetTime = HI_INVALID_PTS;
        pAvplay->HDRSyncTargetInit = HI_FALSE;
        pAvplay->HDRSyncTarget     = AVPLAY_HDR_CHAN_BUTT;
        pAvplay->bBlFrmProcFlag    = HI_FALSE;
        pAvplay->bBlFrmProcFlag    = HI_FALSE;
        pAvplay->BlSyncProc        = AVPLAY_HDR_SYNC_PROC_CONTINUE;
        pAvplay->ElSyncProc        = AVPLAY_HDR_SYNC_PROC_CONTINUE;
        pAvplay->CurBlPts          = HI_INVALID_PTS;
        pAvplay->CurElPts          = HI_INVALID_PTS;
    }

    return Ret;
}

HI_S32 AVPLAY_SetHDRChnAttr(AVPLAY_S *pAvplay, AVPLAY_HDR_CHAN_E HDRChn)
{
    HI_S32                      Ret = HI_SUCCESS;
    HI_UNF_VCODEC_ATTR_S        ElVdecAttr;
    VDEC_HDR_ATTR_S             VdecHDRAttr;

    AVPLAY_HDR_CHN_CHECK(HDRChn);

    if (HDRChn & ((HI_U32)AVPLAY_HDR_CHAN_BL))
    {
        VdecHDRAttr.AvplayHDRAttr = pAvplay->AvplayHDRAttr;
        VdecHDRAttr.enVdecHDRType = HI_VDEC_HDR_CHAN_TYPE_BL;
        Ret = HI_MPI_VDEC_SetHDRAttr(pAvplay->hVdec, &VdecHDRAttr);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_SetHDRAttr fail 0x%x\n", Ret);
            return Ret;
        }
    }

    if (HDRChn & ((HI_U32)AVPLAY_HDR_CHAN_EL))
    {
        Ret = HI_MPI_VDEC_GetChanAttr(pAvplay->hElVdec, &ElVdecAttr);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_GetChanAttr fail 0x%x\n", Ret);
            return Ret;
        }

        ElVdecAttr.enType = pAvplay->AvplayHDRAttr.enElType;
        ElVdecAttr.enMode = HI_UNF_VCODEC_MODE_NORMAL;
        ElVdecAttr.u32Priority = 3;
        ElVdecAttr.u32ErrCover = 0;
        Ret = HI_MPI_VDEC_SetChanAttr(pAvplay->hElVdec, &ElVdecAttr);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_SetChanAttr fail 0x%x\n", Ret);
            return Ret;
        }

        VdecHDRAttr.AvplayHDRAttr = pAvplay->AvplayHDRAttr;
        VdecHDRAttr.enVdecHDRType = HI_VDEC_HDR_CHAN_TYPE_EL;
        Ret = HI_MPI_VDEC_SetHDRAttr(pAvplay->hElVdec, &VdecHDRAttr);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_SetHDRAttr fail 0x%x\n", Ret);
            return Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_SetHDRPortAttr(AVPLAY_S *pAvplay, VDEC_PORT_TYPE_E enType)
{
    HI_S32                      Ret;

    Ret = HI_MPI_VDEC_SetPortType(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort, enType);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_SetPortType %#x.\n", Ret);
        return Ret;
    }

    Ret = HI_MPI_VDEC_EnablePort(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_EnablePort %#x.\n", Ret);
        return Ret;
    }

    if (pAvplay->bDualChnPlay)
    {
        Ret = HI_MPI_VDEC_SetPortType(pAvplay->hElVdec, pAvplay->hElPort, enType);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_SetPortType %#x.\n", Ret);
            return Ret;
        }

        Ret = HI_MPI_VDEC_EnablePort(pAvplay->hElVdec, pAvplay->hElPort);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_EnablePort %#x.\n", Ret);
            return Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_CreateHDRPort(AVPLAY_S *pAvplay, VDEC_PORT_ABILITY_E enAbility, HI_HANDLE hWindow)
{
    HI_S32                  Ret;
    HI_BOOL                 WinCurState;
    HI_DRV_WIN_SRC_HANDLE_S WinSrcInfo = {0};

    memset(&WinSrcInfo, 0, sizeof(HI_DRV_WIN_SRC_HANDLE_S));

    Ret = AVPLAY_WindowUnload(hWindow, &WinCurState);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = HI_MPI_WIN_SetMode(hWindow, HI_DRV_WINDOW_MODE_DOLBY);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_WIN_SetMode failed 0x%x\n", Ret);
        return Ret;
    }

    Ret = HI_MPI_VDEC_CreatePort(pAvplay->hVdec, &pAvplay->MasterFrmChn.hPort, enAbility);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_CreatePort, Ret=0x%x\n", Ret);
        return Ret;
    }

    pAvplay->hBlPort = pAvplay->MasterFrmChn.hPort;

    if (pAvplay->bDualChnPlay)
    {
        Ret = HI_MPI_VDEC_CreatePort(pAvplay->hElVdec, &pAvplay->hElPort, enAbility);
        if (HI_SUCCESS != Ret)
        {
            (HI_VOID)HI_MPI_VDEC_DestroyPort(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort);
            HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_CreatePort.\n");
            return Ret;
        }
    }

    Ret = AVPLAY_SetHDRPortAttr(pAvplay, VDEC_PORT_TYPE_MASTER);
    if (HI_SUCCESS != Ret)
    {
        goto ERR_DESTROY_PORT;
    }

    WinSrcInfo.enSrcMode  = HI_ID_VDEC;
    WinSrcInfo.hSrc       = pAvplay->MasterFrmChn.hPort;
    WinSrcInfo.hSecondSrc = pAvplay->bDualChnPlay ? pAvplay->hElPort : HI_INVALID_HANDLE;
    Ret = HI_MPI_WIN_SetSource(hWindow, &WinSrcInfo);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_WIN_SetSource 0x%x\n", Ret);
        goto ERR_DESTROY_PORT;
    }

    Ret = AVPLAY_WindowLoad(hWindow, WinCurState);
    if (HI_SUCCESS != Ret)
    {
        goto ERR_DESTROY_PORT;
    }

    return HI_SUCCESS;

ERR_DESTROY_PORT:
    (HI_VOID)HI_MPI_VDEC_DestroyPort(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort);

    if (pAvplay->bDualChnPlay)
    {
        (HI_VOID)HI_MPI_VDEC_DestroyPort(pAvplay->hElVdec, pAvplay->hElPort);
    }

    return HI_FAILURE;
}

HI_S32 AVPLAY_DestroyHDRPort(AVPLAY_S *pAvplay, HI_HANDLE hWindow)
{
    HI_S32                   Ret;
    HI_BOOL                  WinCurState;
    HI_DRV_WIN_SRC_HANDLE_S  WinSrcInfo = {0};

    memset(&WinSrcInfo, 0, sizeof(HI_DRV_WIN_SRC_HANDLE_S));

    Ret = AVPLAY_WindowUnload(hWindow, &WinCurState);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    WinSrcInfo.enSrcMode  = HI_ID_BUTT;
    WinSrcInfo.hSrc       = HI_INVALID_HANDLE;
    WinSrcInfo.hSecondSrc = HI_INVALID_HANDLE;
    Ret = HI_MPI_WIN_SetSource(hWindow, &WinSrcInfo);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_WIN_SetSource 0x%x.\n", Ret);
        return Ret;
    }

    if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hPort)
    {
        Ret = HI_MPI_VDEC_DestroyPort(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_DestroyPort 0x%x.\n", Ret);
        }

        pAvplay->MasterFrmChn.hPort = HI_INVALID_HANDLE;
        pAvplay->hBlPort = HI_INVALID_HANDLE;
    }

    if (HI_INVALID_HANDLE != pAvplay->hElPort)
    {
        Ret = HI_MPI_VDEC_DestroyPort(pAvplay->hElVdec, pAvplay->hElPort);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_DestroyPort 0x%x.\n", Ret);
        }

        pAvplay->hElPort = HI_INVALID_HANDLE;
    }

    Ret = AVPLAY_WindowLoad(hWindow, WinCurState);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_CheckHDRInParam(HI_UNF_AVPLAY_HDR_ATTR_S *pstAttr)
{
    if ((HI_U32)pstAttr->enElType >= HI_UNF_VCODEC_TYPE_BUTT)
    {
        HI_ERR_AVPLAY("vcodec type %d is invalid.\n", pstAttr->enElType);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if ((HI_U32)pstAttr->stElCodecOpt.enDecType >= HI_UNF_VCODEC_DEC_TYPE_BUTT)
    {
        HI_ERR_AVPLAY("enDecType %d is invalid.\n", pstAttr->stElCodecOpt.enDecType);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if ((HI_U32)pstAttr->stElCodecOpt.enCapLevel >= HI_UNF_VCODEC_CAP_LEVEL_BUTT)
    {
        HI_ERR_AVPLAY("enCapLevel %d is invalid.\n", pstAttr->stElCodecOpt.enCapLevel);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if ((HI_U32)pstAttr->stElCodecOpt.enProtocolLevel >= HI_UNF_VCODEC_PRTCL_LEVEL_BUTT)
    {
        HI_ERR_AVPLAY("enProtocolLevel %d is invalid.\n", pstAttr->stElCodecOpt.enProtocolLevel);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_AddHDRChn(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_HDR_ATTR_S *pstAttr)
{
    HI_S32  Ret;
    HI_BOOL bAllocHDRChn = HI_FALSE;

    memcpy(&pAvplay->AvplayHDRAttr, pstAttr, sizeof(HI_UNF_AVPLAY_HDR_ATTR_S));

    if (   (pstAttr->enHDRStreamType == HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_SINGLE_VES)
        || (pstAttr->enHDRStreamType == HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_DUAL_VES) )
    {
        Ret = AVPLAY_CheckHDRInParam(pstAttr);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }

        if (pAvplay->hElVdec == HI_INVALID_HANDLE)
        {
            Ret = AVPLAY_AllocHDRChn(pAvplay, AVPLAY_HDR_CHAN_EL);
            if (HI_SUCCESS != Ret)
            {
                return Ret;
            }

            bAllocHDRChn = HI_TRUE;
        }

        Ret = AVPLAY_SetHDRChnAttr(pAvplay, AVPLAY_HDR_CHAN_BL | AVPLAY_HDR_CHAN_EL);
        if (HI_SUCCESS != Ret)
        {
            if (bAllocHDRChn)
            {
                (HI_VOID)AVPLAY_FreeHDRChn(pAvplay, AVPLAY_HDR_CHAN_EL);
            }

            return Ret;
        }

        pAvplay->bHDRPlay     = HI_TRUE;
        pAvplay->bDualChnPlay = HI_TRUE;
    }
    else if (pstAttr->enHDRStreamType == HI_UNF_AVPLAY_HDR_STREAM_TYPE_SL_VES)
    {
        if (pAvplay->hVdec == HI_INVALID_HANDLE)
        {
            Ret = AVPLAY_AllocHDRChn(pAvplay, AVPLAY_HDR_CHAN_BL);
            if (HI_SUCCESS != Ret)
            {
                return Ret;
            }

            bAllocHDRChn = HI_TRUE;
        }

        Ret = AVPLAY_SetHDRChnAttr(pAvplay, AVPLAY_HDR_CHAN_BL);
        if (HI_SUCCESS != Ret)
        {
            if (bAllocHDRChn)
            {
                (HI_VOID)AVPLAY_FreeHDRChn(pAvplay, AVPLAY_HDR_CHAN_BL);
            }

            return Ret;
        }

        pAvplay->bHDRPlay     = HI_TRUE;
        pAvplay->bDualChnPlay = HI_FALSE;
    }
    else
    {
        HI_ERR_AVPLAY("enHDRStreamType %d is invalid.\n", pstAttr->enHDRStreamType);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
    {
        Ret = AVPLAY_DestroyHDRPort(pAvplay, pAvplay->MasterFrmChn.hWindow);
        if (HI_SUCCESS != Ret)
        {
            return Ret;
        }

        Ret = AVPLAY_CreateHDRPort(pAvplay, VDEC_PORT_HD, pAvplay->MasterFrmChn.hWindow);
        if (HI_SUCCESS != Ret)
        {
            return Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_DelHDRChn(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_HDR_ATTR_S *pstAttr)
{
    HI_S32           Ret;
    VDEC_HDR_ATTR_S  VdecHDRAttr;

    switch (pAvplay->AvplayHDRAttr.enHDRStreamType)
    {
        case HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_DUAL_VES :
        case HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_SINGLE_VES :
        case HI_UNF_AVPLAY_HDR_STREAM_TYPE_SL_VES :
            break;

        default:
            HI_ERR_AVPLAY("enHDRStreamType %d is invalid.\n", pstAttr->enHDRStreamType);
            return HI_ERR_AVPLAY_INVALID_PARA;
    }

    Ret = AVPLAY_DestroyHDRPort(pAvplay, pAvplay->MasterFrmChn.hWindow);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    Ret = AVPLAY_CreatePort(pAvplay, pAvplay->MasterFrmChn.hWindow, VDEC_PORT_HD, &pAvplay->MasterFrmChn.hPort);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = AVPLAY_SetPortAttr(pAvplay, pAvplay->MasterFrmChn.hPort, VDEC_PORT_TYPE_MASTER);
    if (HI_SUCCESS != Ret)
    {
        goto ERR_DESTROY_PORT;
    }

    Ret = HI_MPI_WIN_SetMode(pAvplay->MasterFrmChn.hWindow, HI_DRV_WINDOW_MODE_NORMAL);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("HI_MPI_WIN_SetMode fail 0x%x\n", Ret);
        goto ERR_DESTROY_PORT;
    }

    VdecHDRAttr.AvplayHDRAttr.bEnable = HI_FALSE;
    VdecHDRAttr.enVdecHDRType = HI_VDEC_HDR_CHAN_TYPE_BUTT;
    Ret = HI_MPI_VDEC_SetHDRAttr(pAvplay->hVdec, &VdecHDRAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_SetHDRAttr fail 0x%x\n", Ret);
        goto ERR_DESTROY_PORT;
    }

    if (   (pAvplay->AvplayHDRAttr.enHDRStreamType == HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_DUAL_VES)
        || (pAvplay->AvplayHDRAttr.enHDRStreamType == HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_SINGLE_VES) )
    {
        Ret = AVPLAY_FreeHDRChn(pAvplay, AVPLAY_HDR_CHAN_EL);
        if (Ret != HI_SUCCESS)
        {
            goto ERR_DESTROY_PORT;
        }
    }

    pAvplay->bHDRPlay     = HI_FALSE;
    pAvplay->bDualChnPlay = HI_FALSE;
    pAvplay->hBlPort      = HI_INVALID_HANDLE;
    pAvplay->hElPort      = HI_INVALID_HANDLE;

    return HI_SUCCESS;

ERR_DESTROY_PORT:

    (HI_VOID)AVPLAY_DestroyPort(pAvplay, pAvplay->MasterFrmChn.hWindow, pAvplay->MasterFrmChn.hPort);

    return HI_FAILURE;
}

HI_S32 AVPLAY_SetHDR(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_HDR_ATTR_S *pstAttr)
{
    if (pAvplay->VidEnable == HI_TRUE)
    {
        HI_ERR_AVPLAY("vid chn is enable, can not set HDR Attr.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if ((pstAttr->bEnable == HI_FALSE) && (pAvplay->bHDRPlay == HI_FALSE))
    {
        HI_INFO_AVPLAY("vid chn is already normal\n");
        return HI_SUCCESS;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        HI_ERR_AVPLAY("Not support ts mode.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (pAvplay->hVdec == HI_INVALID_HANDLE)
    {
        HI_ERR_AVPLAY("Vdec Channel is not open.\n");
        return HI_FAILURE;
    }

    if (pstAttr->bEnable)
    {
        return AVPLAY_AddHDRChn(pAvplay, pstAttr);
    }
    else
    {
        return AVPLAY_DelHDRChn(pAvplay, pstAttr);
    }
}

HI_S32 AVPLAY_GetHDR(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_HDR_ATTR_S *pstAttr)
{
    memcpy(pstAttr, &pAvplay->AvplayHDRAttr, sizeof(HI_UNF_AVPLAY_HDR_ATTR_S));

    return HI_SUCCESS;
}
#endif

HI_S32 AVPLAY_ReceiveVidFrame(AVPLAY_S *pAvplay, HI_U32 TimeoutMs)
{
    HI_S32  Ret;

#ifdef HI_HDR_SUPPORT
    if (pAvplay->bHDRPlay && pAvplay->bDualChnPlay)
    {
        (HI_VOID)AVPLAY_ProcBlFrame(pAvplay);
        (HI_VOID)AVPLAY_ProcElFrame(pAvplay);

        if (   (AVPLAY_HDR_SYNC_PROC_PLAY == pAvplay->BlSyncProc)
            && (AVPLAY_HDR_SYNC_PROC_PLAY == pAvplay->ElSyncProc) )
        {
            pAvplay->bBlFrmProcFlag = HI_FALSE;
            pAvplay->bElFrmProcFlag = HI_FALSE;
            pAvplay->HDRSyncTargetInit = HI_FALSE;
            pAvplay->ElSyncProc = AVPLAY_HDR_SYNC_PROC_CONTINUE;
            pAvplay->BlSyncProc = AVPLAY_HDR_SYNC_PROC_CONTINUE;
        }
        else
        {
            pAvplay->AvplayProcVidContinue = HI_TRUE;
            return HI_FAILURE;
        }

        memcpy(&pAvplay->CurFrmPack, &pAvplay->CurBlFrmPack, sizeof(pAvplay->CurFrmPack));

        HI_INFO_AVPLAY("=====Receive HDR frame, Sys=%u, Id=%u, BlPts=%u, ElPts=%u, BlType:%d, ElType:%d =====\n",
            AVPLAY_GetSysTime(), pAvplay->CurFrmPack.stFrame[0].stFrameVideo.u32FrameIndex,
            pAvplay->CurFrmPack.stFrame[0].stFrameVideo.u32Pts, pAvplay->CurElFrmPack.stFrame[0].stFrameVideo.u32Pts,
            pAvplay->CurFrmPack.stFrame[0].stFrameVideo.enSrcFrameType, pAvplay->CurElFrmPack.stFrame[0].stFrameVideo.enSrcFrameType);
    }
    else
#endif
    {
        Ret = HI_MPI_VDEC_ReceiveFrame(pAvplay->hVdec, &pAvplay->CurFrmPack, AVPLAY_RECVFRAME_MAX_TIME);
        if (HI_SUCCESS != Ret)
        {
            pAvplay->AvplayProcVidContinue = HI_TRUE;
            return HI_FAILURE;
        }

        HI_INFO_AVPLAY("=====Receive a new frame, Sys=%u, Id=%u, Pts=%u, FrameType=%d =====\n",
            AVPLAY_GetSysTime(), pAvplay->CurFrmPack.stFrame[0].stFrameVideo.u32FrameIndex,
            pAvplay->CurFrmPack.stFrame[0].stFrameVideo.u32Pts,
            pAvplay->CurFrmPack.stFrame[0].stFrameVideo.enSrcFrameType);
    }

    return HI_SUCCESS;
}

HI_VOID AVPLAY_QueueVidFrame(AVPLAY_S *pAvplay)
{
    if (   (0 == pAvplay->FrcNeedPlayCnt)
        || (pAvplay->FrcCurPlayCnt < pAvplay->FrcNeedPlayCnt) )
    {
        if (SYNC_PROC_PLAY == pAvplay->VidOpt.SyncProc)
        {
            AVPLAY_ProcVidPlay(pAvplay);
        }
        else if (SYNC_PROC_REPEAT == pAvplay->VidOpt.SyncProc)
        {
            AVPLAY_ProcVidRepeat(pAvplay);
        }
        else if (SYNC_PROC_DISCARD == pAvplay->VidOpt.SyncProc)
        {
            AVPLAY_ProcVidDiscard(pAvplay);
        }
        else if (SYNC_PROC_QUICKOUTPUT == pAvplay->VidOpt.SyncProc)
        {
            AVPLAY_ProcVidQuickOutput(pAvplay);
        }
    }

    if (pAvplay->FrcCurPlayCnt >= pAvplay->FrcNeedPlayCnt)
    {
        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
    }

    return;
}


HI_VOID AVPLAY_ProcVdecToVo(AVPLAY_S *pAvplay)
{
    HI_U32                              i;
    HI_S32                              Ret;
    HI_LD_Event_S                       LdEvent;

    if (!pAvplay->VidEnable)
    {
        return;
    }

    if (HI_UNF_AVPLAY_STATUS_PAUSE == pAvplay->CurStatus)
    {
        return;
    }

    if (!pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
    {
        HI_DRV_VDEC_FRAME_S        *pVdecFrame = HI_NULL;
        HI_UNF_VIDEO_FRAME_INFO_S   VidFrm;

        pAvplay->DebugInfo.AcquireVidFrameNum++;

        Ret = AVPLAY_ReceiveVidFrame(pAvplay, AVPLAY_RECVFRAME_MAX_TIME);
        if (HI_SUCCESS != Ret)
        {
            return;
        }

        pVdecFrame = &pAvplay->CurFrmPack.stFrame[0];

        AVPLAY_DRV2UNF_VidFrm(&pVdecFrame->stFrameVideo, &VidFrm);

        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME, &VidFrm);

        /* record low delay event */
        LdEvent.evt_id = EVENT_AVPLAY_FRM_IN;
        LdEvent.frame  = pVdecFrame->stFrameVideo.u32FrameIndex;
        LdEvent.handle = pVdecFrame->stFrameVideo.hTunnelSrc;
        (HI_VOID)HI_SYS_GetTimeStampMs(&(LdEvent.time));

        (HI_VOID)HI_MPI_STAT_NotifyLowDelayEvent(&LdEvent);

        /* record program switch event */
        if (pVdecFrame->stFrameVideo.bIsFirstIFrame)
        {
            HI_MPI_STAT_Event(STAT_EVENT_AVPLAYGETFRM, 0);
        }

        for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
        {
            pAvplay->bSendedFrmToSlaveWin[i] = HI_FALSE;
        }

        for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
        {
            pAvplay->bSendedFrmToVirtualWin[i] = HI_FALSE;
        }

        pAvplay->bSendedFrmToMasterWin = HI_FALSE;

        pAvplay->bProcFrmToVirtualWin = HI_TRUE;

        pAvplay->AvplayProcVidContinue = HI_TRUE;

        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_TRUE;

        pAvplay->DebugInfo.AcquiredVidFrameNum++;

        AVPLAY_ProcOriVidFrameRate(pAvplay);

        AVPLAY_ProcVidFrc(pAvplay);
    }

#ifdef HI_HDR_SUPPORT
    if (pAvplay->bProcFrmToVirtualWin && !pAvplay->bHDRPlay)
    {
        AVPLAY_ProcFrmToVirWin(pAvplay);
    }
#else
    if (pAvplay->bProcFrmToVirtualWin)
    {
        AVPLAY_ProcFrmToVirWin(pAvplay);
    }
#endif

    if (pAvplay->bStepMode)
    {
        if (pAvplay->bStepPlay)
        {
            AVPLAY_ProcVidPlay(pAvplay);
            pAvplay->bStepPlay = HI_FALSE;
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
        }

        return;
    }

    if (0 == pAvplay->FrcCurPlayCnt)
    {
        AVPLAY_ProcVidSync(pAvplay);
    }

    HI_INFO_AVPLAY("sys:%u, frm:%d, need:%u, cur:%u, sync:%u, delay:%u\n",
        AVPLAY_GetSysTime(),pAvplay->CurFrmPack.stFrame[0].stFrameVideo.u32FrameIndex, pAvplay->FrcNeedPlayCnt,
        pAvplay->FrcCurPlayCnt, pAvplay->VidOpt.SyncProc, pAvplay->VidInfo.DelayTime);

#ifdef HI_HDR_SUPPORT
    if (pAvplay->bHDRPlay)
    {
        AVPLAY_QueueHDRFrame(pAvplay);
    }
    else
#endif
    {
        AVPLAY_QueueVidFrame(pAvplay);
    }

    return;
}

HI_VOID AVPLAY_AudDDPMode(AVPLAY_S *pAvplay)
{
    static HI_U32 s_u32LastPtsTime = 0;
    HI_U32 thisPts = pAvplay->AvplayDmxEsBuf.u32PtsMs;
    HI_U32 thisPtsTime = AVPLAY_GetSysTime();
    HI_S32 ptsDiff = 0;

    if (   (thisPts < pAvplay->LastAudPts)
        && (pAvplay->LastAudPts != HI_INVALID_PTS)
        && (thisPts != HI_INVALID_PTS)
        )
    {
        HI_ERR_AVPLAY("PTS:%u -> %u, PtsLess.\n ", pAvplay->LastAudPts, thisPts);
        (HI_VOID)AVPLAY_ResetAudChn(pAvplay);
        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;
        usleep(1200*1000);
        HI_ERR_AVPLAY("Rest OK.\n");
    }
    else
    {
        if ( thisPtsTime >  s_u32LastPtsTime)
        {
            ptsDiff = (HI_S32)(thisPtsTime - s_u32LastPtsTime);
        }
        else
        {
            ptsDiff = 0;
        }

        if ( ptsDiff > 1000 )
        {
            HI_ERR_AVPLAY("PtsTime:%u -> %u, Diff:%d.\n ", s_u32LastPtsTime, thisPtsTime, ptsDiff);
            (HI_VOID)AVPLAY_ResetAudChn(pAvplay);
            usleep(1200*1000);
            HI_ERR_AVPLAY("Rest OK.\n");
            s_u32LastPtsTime = HI_INVALID_PTS;
            pAvplay->LastAudPts = HI_INVALID_PTS;

        }
    }

    if (thisPts != HI_INVALID_PTS)
    {
        pAvplay->LastAudPts = thisPts;
        s_u32LastPtsTime = thisPtsTime;
    }

    return;
}

HI_VOID AVPLAY_ProcDmxToAdec(AVPLAY_S *pAvplay)
{
    HI_UNF_STREAM_BUF_S             AdecEsBuf = {0};
    HI_S32                          Ret;
    HI_U32                          i;
    HI_UNF_ES_BUF_S                 AudDmxEsBuf = {0};

    if (!pAvplay->AudEnable)
    {
        return;
    }

    Ret = HI_MPI_ADEC_GetDelayMs(pAvplay->hAdec, &pAvplay->AdecDelayMs);
    if (HI_SUCCESS == Ret && pAvplay->AdecDelayMs > AVPLAY_ADEC_MAX_DELAY)
    {
        return;
    }

    if (!pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC])
    {
        for (i = 0; i < pAvplay->DmxAudChnNum; i++)
        {
            if (i == pAvplay->CurDmxAudChn)
            {
                pAvplay->DebugInfo.AcquireAudEsNum++;
                Ret = HI_MPI_DMX_AcquireEs(pAvplay->hDmxAud[i], &(pAvplay->AvplayDmxEsBuf));
                if (HI_SUCCESS == Ret)
                {
                    pAvplay->DebugInfo.AcquiredAudEsNum++;
                    pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC] = HI_TRUE;
                }
                else
                {
                    /*if is eos and there is no data in demux channel, set eos to adec and ao*/
                    if (HI_ERR_DMX_EMPTY_BUFFER == Ret
                        && pAvplay->bSetEosFlag && !pAvplay->bSetAudEos)
                    {
                        Ret = HI_MPI_ADEC_SetEosFlag(pAvplay->hAdec);
                        if (HI_SUCCESS != Ret)
                        {
                            HI_ERR_AVPLAY("ERR: HI_MPI_ADEC_SetEosFlag, Ret = %#x! \n", Ret);
                            return;
                        }

                        if (HI_INVALID_HANDLE != pAvplay->hSyncTrack)
                        {
                            Ret = HI_MPI_AO_Track_SetEosFlag(pAvplay->hSyncTrack, HI_TRUE);
                            if (HI_SUCCESS != Ret)
                            {
                                HI_ERR_AVPLAY("ERR: HI_MPI_HIAO_SetEosFlag, Ret = %#x! \n", Ret);
                                return;
                            }
                        }

                        pAvplay->bSetAudEos = HI_TRUE;
                    }
                }
            }
            else
            {
                Ret = HI_MPI_DMX_AcquireEs(pAvplay->hDmxAud[i], &AudDmxEsBuf);
                if (HI_SUCCESS == Ret)
                {
                    (HI_VOID)HI_MPI_DMX_ReleaseEs(pAvplay->hDmxAud[i], &AudDmxEsBuf);
                }
            }
        }
    }

    if (pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC])
    {
        AdecEsBuf.pu8Data = pAvplay->AvplayDmxEsBuf.pu8Buf;
        AdecEsBuf.u32Size = pAvplay->AvplayDmxEsBuf.u32BufLen;

        pAvplay->DebugInfo.SendAudEsNum++;


        /* for DDP test only, when ts stream revers(this pts < last pts),
            reset audChn, and buffer 600ms audio stream  */
        if (pAvplay->AudDDPMode)
        {
            AVPLAY_AudDDPMode(pAvplay);
        }

        Ret = HI_MPI_ADEC_SendStream(pAvplay->hAdec, &AdecEsBuf, pAvplay->AvplayDmxEsBuf.u32PtsMs);
        if (HI_SUCCESS == Ret)
        {
            pAvplay->DebugInfo.SendedAudEsNum++;
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC] = HI_FALSE;
            pAvplay->AvplayProcAudContinue = HI_TRUE;

            AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_GET_AUD_ES, &pAvplay->AvplayDmxEsBuf);

            (HI_VOID)HI_MPI_DMX_ReleaseEs(pAvplay->hDmxAud[pAvplay->CurDmxAudChn], &pAvplay->AvplayDmxEsBuf);
        }
        else
        {
            if ((Ret != HI_ERR_ADEC_IN_BUF_FULL) && (Ret != HI_ERR_ADEC_IN_PTSBUF_FULL)) /* drop this pkg */
            {
                HI_ERR_AVPLAY("Send AudEs buf to ADEC fail:%#x, drop a pkg.\n", Ret);
                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC] = HI_FALSE;
                pAvplay->AvplayProcAudContinue = HI_TRUE;
                (HI_VOID)HI_MPI_DMX_ReleaseEs(pAvplay->hDmxAud[pAvplay->CurDmxAudChn], &pAvplay->AvplayDmxEsBuf);
            }
        }
    }

    return;
}

HI_VOID AVPLAY_ProcDmxToAdAdec(AVPLAY_S *pAvplay)
{
    HI_S32                          Ret;
    ADEC_ATTR_S                     AdecAttr = {0};
    HI_UNF_STREAM_BUF_S             AdecEsBuf = {0};

    /* AVPLAY_Start: pAvplay->AudEnable = HI_TRUE */
    if (HI_INVALID_HANDLE == pAvplay->hDmxAdAud)
    {
        return;
    }

    if (!pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_AD_ADEC])
    {
        pAvplay->DebugInfo.AcquireAudADEsNum++;

        Ret = HI_MPI_DMX_AcquireEs(pAvplay->hDmxAdAud, &(pAvplay->AvplayAdDmxEsBuf));
        if (HI_SUCCESS == Ret)
        {
            pAvplay->DebugInfo.AcquiredAudADEsNum++;
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_AD_ADEC] = HI_TRUE;
        }

        Ret = HI_MPI_ADEC_GetAllAttr(pAvplay->hAdec, &AdecAttr);
        if (HI_SUCCESS == Ret)
        {
            /*if not support dual decoder, release Ad ES stream*/
            if (AdecAttr.bDualDec == HI_FALSE)
            {
                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_AD_ADEC] = HI_FALSE;
                (HI_VOID)HI_MPI_DMX_ReleaseEs(pAvplay->hDmxAdAud, &pAvplay->AvplayAdDmxEsBuf);
                return;
            }
        }
    }

    if (pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_AD_ADEC])
    {
        pAvplay->DebugInfo.SendAudADEsNum++;

        AdecEsBuf.pu8Data = pAvplay->AvplayAdDmxEsBuf.pu8Buf;
        AdecEsBuf.u32Size = pAvplay->AvplayAdDmxEsBuf.u32BufLen;
        Ret = HI_MPI_ADEC_SendADStream(pAvplay->hAdec, &AdecEsBuf, pAvplay->AvplayAdDmxEsBuf.u32PtsMs);
        if (HI_SUCCESS == Ret)
        {
            pAvplay->DebugInfo.SendedAudADEsNum++;
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_AD_ADEC] = HI_FALSE;
            (HI_VOID)HI_MPI_DMX_ReleaseEs(pAvplay->hDmxAdAud, &pAvplay->AvplayAdDmxEsBuf);
        }
        else
        {
            if ((Ret != HI_ERR_ADEC_IN_BUF_FULL) && (Ret != HI_ERR_ADEC_IN_PTSBUF_FULL)) /* drop this pkg */
            {
                HI_ERR_AVPLAY("Send AD AudEs buf to ADEC fail:%#x, drop a pkg.\n", Ret);
                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_AD_ADEC] = HI_FALSE;
                (HI_VOID)HI_MPI_DMX_ReleaseEs(pAvplay->hDmxAdAud, &pAvplay->AvplayAdDmxEsBuf);
            }
        }
    }

    return;
}

HI_VOID AVPLAY_Eos(AVPLAY_S *pAvplay)
{
    pAvplay->PreAudEsBuf = 0;
    pAvplay->PreVidEsBuf = 0;
    pAvplay->PreSystime = 0;
    pAvplay->PreVidEsBufWPtr= 0;
    pAvplay->PreAudEsBufWPtr= 0;
    pAvplay->CurBufferEmptyState = HI_TRUE;
    pAvplay->LstStatus = pAvplay->CurStatus;
    pAvplay->CurStatus = HI_UNF_AVPLAY_STATUS_EOS;

    return;
}

HI_VOID AVPLAY_ProcEos(AVPLAY_S *pAvplay)
{
    HI_S32                  Ret;
    HI_BOOL                 bVidEos = HI_TRUE;
    HI_BOOL                 bAudEos = HI_TRUE;

    if (pAvplay->CurStatus == HI_UNF_AVPLAY_STATUS_EOS)
    {
        return;
    }

    if (pAvplay->AudEnable)
    {
        ADEC_BUFSTATUS_S    AdecBuf = {0};
        ADEC_STATUSINFO_S   AdecStatus = {0};
        HI_BOOL             bAoBufEmpty = HI_TRUE;

        bAudEos = HI_FALSE;

        if (HI_INVALID_HANDLE != pAvplay->hSyncTrack)
        {
            Ret = HI_MPI_AO_Track_IsBufEmpty(pAvplay->hSyncTrack, &bAoBufEmpty);
            if (HI_SUCCESS != Ret)
            {
                bAoBufEmpty = HI_FALSE;
            }
        }

        Ret = HI_MPI_ADEC_GetInfo(pAvplay->hAdec, HI_MPI_ADEC_BUFFERSTATUS, &AdecBuf);
        if (HI_SUCCESS == Ret)
        {
            Ret = HI_MPI_ADEC_GetInfo(pAvplay->hAdec, HI_MPI_ADEC_STATUSINFO, &AdecStatus);
            if (HI_SUCCESS == Ret)
            {
                if (bAoBufEmpty && AdecBuf.bEndOfFrame && (AdecStatus.u32UsedBufNum == 0))
                {
                    bAudEos = HI_TRUE;
                }

                pAvplay->DebugInfo.bAoBufEmpty = bAoBufEmpty;
                pAvplay->DebugInfo.bEndOfFrame = AdecBuf.bEndOfFrame;
                pAvplay->DebugInfo.UsedBufNum  = AdecStatus.u32UsedBufNum;
            }
        }
    }

    if (pAvplay->VidEnable)
    {
        VDEC_STATUSINFO_S VdecStatus;

        bVidEos = HI_FALSE;

        Ret = HI_MPI_VDEC_GetChanStatusInfo(pAvplay->hVdec, &VdecStatus);
        if (HI_SUCCESS == Ret)
        {
            pAvplay->DebugInfo.bEndOfStream        = VdecStatus.bEndOfStream;
            pAvplay->DebugInfo.bAllPortCompleteFrm = VdecStatus.bAllPortCompleteFrm;

            if (VdecStatus.bEndOfStream && VdecStatus.bAllPortCompleteFrm)
            {
                if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
                {
                    bVidEos = HI_TRUE;
                }
                else
                {
                    HI_DRV_WIN_PLAY_INFO_S  WinPlayInfo;

                    Ret = HI_MPI_WIN_GetPlayInfo(pAvplay->MasterFrmChn.hWindow, &WinPlayInfo);
                    if (HI_SUCCESS == Ret)
                    {
                        if (0 == WinPlayInfo.u32FrameNumInBufQn)
                        {
                            bVidEos = HI_TRUE;
                        }

                        pAvplay->DebugInfo.FrameNumInBufQn = WinPlayInfo.u32FrameNumInBufQn;
                    }
                }
            }
        }
    }

    if (bVidEos && bAudEos)
    {
        AVPLAY_Eos(pAvplay);

        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_EOS, HI_NULL);
    }

    return;
}

static HI_VOID AVPLAY_CalPreBufThreshold(AVPLAY_S *pAvplay)
{
    HI_U32                                          VidBufPercent = 0;
    HI_U32                                          AudBufPercent = 0;
    HI_MPI_DMX_BUF_STATUS_S                         VidChnBuf = {0};
    HI_MPI_DMX_BUF_STATUS_S                         AudChnBuf = {0};
    HI_U32                                          u32SysTime = 0;
    HI_S32                                          Ret;

    if (0 == pAvplay->AudPreBufThreshold)
    {
        HI_SYS_GetTimeStampMs(&u32SysTime);
        if (-1 == pAvplay->AudPreSysTime)
        {
            pAvplay->AudPreSysTime = u32SysTime;
        }
        else
        {
            Ret = HI_MPI_DMX_GetPESBufferStatus(pAvplay->hDmxAud[pAvplay->CurDmxAudChn], &AudChnBuf);
            if ( HI_SUCCESS == Ret )
            {
                if ( AudChnBuf.u32BufSize == 0 )
                {
                    AudBufPercent = 0;
                    HI_ERR_AVPLAY("AudChnBuf.u32BufSize == 0\n");
                }
                else
                {
                    AudBufPercent = AudChnBuf.u32UsedSize * 100 / AudChnBuf.u32BufSize;
                }
            }
            else
            {
                AudBufPercent = 0;
                HI_ERR_AVPLAY("HI_MPI_DMX_GetPESBufferStatus failed:%#x\n",Ret);
            }

            if (   ((u32SysTime - pAvplay->AudPreSysTime > 1000) && (AudBufPercent > 0))
                || (AudBufPercent >= pAvplay->PreAudWaterLine) )
            {
                pAvplay->AudPreBufThreshold = AudBufPercent;
                HI_INFO_AVPLAY("Audio Es buffer Threshold is :%d\n", pAvplay->AudPreBufThreshold);
            }
        }
    }

    if ( (0 == pAvplay->VidPreBufThreshold) && (0 != pAvplay->PreVidWaterLine) )
    {
        HI_SYS_GetTimeStampMs(&u32SysTime);
        if (-1 == pAvplay->VidPreSysTime)
        {
            pAvplay->VidPreSysTime = u32SysTime;
        }
        else
        {
            Ret = HI_MPI_DMX_GetPESBufferStatus(pAvplay->hDmxVid, &VidChnBuf);
            if ( HI_SUCCESS == Ret )
            {
                if ( VidChnBuf.u32BufSize == 0 )
                {
                    VidBufPercent = 0;
                    HI_ERR_AVPLAY("VidChnBuf.u32BufSize == 0\n");
                }
                else
                {
                    VidBufPercent = VidChnBuf.u32UsedSize * 100 / VidChnBuf.u32BufSize;
                }
            }
            else
            {
                VidBufPercent = 0;
                HI_ERR_AVPLAY("HI_MPI_DMX_GetPESBufferStatus failed:%#x\n",Ret);
            }

            if (   ((u32SysTime - pAvplay->VidPreSysTime > 1000) && (VidBufPercent > 0))
                || ((VidBufPercent >= pAvplay->PreVidWaterLine) && (0 != pAvplay->PreVidWaterLine)) )
            {
                pAvplay->VidPreBufThreshold = VidBufPercent;
                HI_INFO_AVPLAY("Video Es buffer Threshold is :%d\n", pAvplay->VidPreBufThreshold);
            }
        }
    }

    return;
}

HI_S32 AVPLAY_CalcVidPreStartWaterLine(AVPLAY_S *pAvplay)
{
    HI_S32                  Ret = HI_FAILURE;
    HI_MPI_DMX_BUF_STATUS_S VidChnBuf = {0};

    Ret = HI_MPI_DMX_GetPESBufferStatus(pAvplay->hDmxVid, &VidChnBuf);
    if ( HI_SUCCESS != Ret )
    {
        HI_ERR_AVPLAY("Call HI_MPI_DMX_GetPESBufferStatus failed 0x%x\n", Ret);
        return HI_FAILURE;
    }

    if ((0 == pAvplay->PreVidFirstWaterLine) && (0 != VidChnBuf.u32BufSize))
    {
        pAvplay->PreVidFirstWaterLine = pAvplay->PreVidBufLen * 100 / VidChnBuf.u32BufSize;
        if (pAvplay->PreVidFirstWaterLine < 1)
        {
            pAvplay->PreVidFirstWaterLine = 1;
        }
    }
    else if ((0 == pAvplay->PreVidSecondWaterLine) && (0 != VidChnBuf.u32BufSize))
    {
        pAvplay->PreVidSecondWaterLine = pAvplay->PreVidBufLen * 100 / VidChnBuf.u32BufSize;

        if (pAvplay->PreVidSecondWaterLine < 50)
        {
            pAvplay->PreVidWaterLine = (pAvplay->PreVidSecondWaterLine > pAvplay->PreVidFirstWaterLine)
                ? pAvplay->PreVidSecondWaterLine : pAvplay->PreVidFirstWaterLine;

            if (pAvplay->PreVidWaterLine <= 5 )
            {
                pAvplay->PreVidWaterLine= pAvplay->PreVidWaterLine * 3 / 2;
            }
            else if (pAvplay->PreVidWaterLine < 10)
            {
                pAvplay->PreVidWaterLine= 10;
            }
        }
        else
        {
            pAvplay->PreVidWaterLine = 50;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_FilterIFrame(AVPLAY_S *pAvplay, HI_U8 *Buf, HI_U32 Size)
{
    HI_S32            Ret;
    HI_U8            *Data = Buf;
    HI_U8            *pSliceStartPos = HI_NULL;
    HI_U8            *pSliceEndPos = HI_NULL;
    DF_CTX_S         *pCtx = HI_NULL;

    if (Size <= sizeof(g_StartCode))
    {
        return HI_FAILURE;
    }

    Ret = DetFrm_GetDFCtx(pAvplay->hDFCtx, &pCtx);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("call DetFrm_GetDFCtx failed 0x%x\n", Ret);
        return HI_FAILURE;
    }

    while (Data < Buf + Size - sizeof(g_StartCode))
    {
        if (!memcmp(Data, g_StartCode, sizeof(g_StartCode)))
        {
            if (!pSliceStartPos)
            {
                pSliceStartPos = Data;
                Data++;
                continue;
            }

            if (!pSliceEndPos)
            {
                pSliceEndPos = Data;
            }
        }

        if (pSliceStartPos && pSliceEndPos)
        {
            pCtx->pNalAddr = pSliceStartPos + sizeof(g_StartCode);
            pCtx->NalLen   = (HI_SIZE_T)(pSliceEndPos - pCtx->pNalAddr);

            if (HI_SUCCESS == DetFrm_Start(pAvplay->hDFCtx))
            {
                if (FRAME_TYPE_I == pCtx->s32NewFrmType)
                {
                    return HI_SUCCESS;
                }
            }

            pSliceStartPos = pSliceEndPos;
            pSliceEndPos = HI_NULL;
        }

        Data++;
    }

    /* Slice incomplete, only start code */
    if (pSliceStartPos)
    {
        pCtx->pNalAddr = pSliceStartPos + sizeof(g_StartCode);
        pCtx->NalLen   = (HI_SIZE_T)(Buf + Size - pCtx->pNalAddr);

        if (HI_SUCCESS == DetFrm_Start(pAvplay->hDFCtx))
        {
            if (FRAME_TYPE_I == pCtx->s32NewFrmType)
            {
                return HI_SUCCESS;
            }
        }
    }

    return HI_FAILURE;
}

HI_VOID AVPLAY_FindIFrameFromDmxEsBuf(AVPLAY_S *pAvplay)
{
    HI_S32          Ret = HI_FAILURE;
    HI_UNF_ES_BUF_S VidDmxEsBuf = {0};

    Ret = HI_MPI_DMX_AcquireEs(pAvplay->hDmxVid, &VidDmxEsBuf);
    if (HI_SUCCESS == Ret)
    {
        Ret = AVPLAY_FilterIFrame(pAvplay, VidDmxEsBuf.pu8Buf, VidDmxEsBuf.u32BufLen);
        if (HI_SUCCESS == Ret)
        {
            pAvplay->PreVidBufLen += VidDmxEsBuf.u32BufLen;

            Ret = AVPLAY_CalcVidPreStartWaterLine(pAvplay);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("Call AVPLAY_CalcVidPreStartWaterLine failed 0x%x\n", Ret);
            }

            pAvplay->PreVidBufLen = 0;
        }
        else
        {
            pAvplay->PreVidBufLen += VidDmxEsBuf.u32BufLen;
        }

        (HI_VOID)HI_MPI_DMX_ReleaseEs(pAvplay->hDmxVid, &VidDmxEsBuf);
    }
    return;
}

HI_VOID AVPLAY_CalcAudPreStartWaterLine(AVPLAY_S *pAvplay)
{
    if ( (-1 != pAvplay->PreAudPts) && (-1 != pAvplay->PreVidPts) )
    {
        if ((abs((HI_S32)(pAvplay->PreAudPts - pAvplay->PreVidPts))) > 10000)
        {
            return;
        }

        if (   (pAvplay->PreAudPts > pAvplay->PreVidPts)
            && (pAvplay->PreAudPts - pAvplay->PreVidPts > 200)
            && (pAvplay->PreAudWaterLine < 50) )
        {
            pAvplay->PreAudWaterLine    = pAvplay->PreAudWaterLine + 3;
            pAvplay->AudPreBufThreshold = pAvplay->PreAudWaterLine;
        }

        if (   (pAvplay->PreVidPts > pAvplay->PreAudPts)
            && (pAvplay->PreVidPts - pAvplay->PreAudPts > 200)
            && (pAvplay->PreAudWaterLine > 5) )
        {
            pAvplay->PreAudWaterLine    = pAvplay->PreAudWaterLine - 3;
            pAvplay->AudPreBufThreshold = pAvplay->PreAudWaterLine;
        }
    }

    return;
}

HI_VOID AVPLAY_ProcDmxBuf(AVPLAY_S *pAvplay)
{
    HI_S32 Ret;
    HI_MPI_DMX_BUF_STATUS_S            VidChnBuf = {0};
    HI_MPI_DMX_BUF_STATUS_S            AudChnBuf = {0};
    HI_U32                             VidBufPercent = 0;
    HI_U32                             AudBufPercent = 0;
    HI_UNF_ES_BUF_S                    AudDmxEsBuf;
    HI_UNF_ES_BUF_S                    VidDmxEsBuf;

    if (HI_UNF_AVPLAY_STATUS_PREPLAY != pAvplay->CurStatus)
    {
        return;
    }

    if ( !pAvplay->AudEnable && pAvplay->bAudPreEnable )
    {
        AVPLAY_CalPreBufThreshold(pAvplay);
        if (pAvplay->AudPreBufThreshold != 0)
        {
            Ret = HI_MPI_DMX_GetPESBufferStatus(pAvplay->hDmxAud[pAvplay->CurDmxAudChn], &AudChnBuf);
            if ( HI_SUCCESS != Ret )
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_GetPESBufferStatus failed:%#x\n",Ret);
            }

            if ( AudChnBuf.u32BufSize == 0 )
            {
                HI_ERR_AVPLAY("AudChnBuf.u32BufSize == 0\n");
            }
            else
            {
                AudBufPercent = AudChnBuf.u32UsedSize * 100 / AudChnBuf.u32BufSize;

                if ( AudBufPercent > pAvplay->AudPreBufThreshold )
                {
                    Ret = HI_MPI_DMX_AcquireEs(pAvplay->hDmxAud[pAvplay->CurDmxAudChn], &AudDmxEsBuf);
                    if ( HI_SUCCESS != Ret  )
                    {
                        HI_ERR_AVPLAY("HI_MPI_DMX_AcquireEs failed:%#x\n",Ret);
                    }
                    else
                    {
                        pAvplay->PreAudPts = AudDmxEsBuf.u32PtsMs;
                        (HI_VOID)HI_MPI_DMX_ReleaseEs(pAvplay->hDmxAud[pAvplay->CurDmxAudChn], &AudDmxEsBuf);
                        pAvplay->AvplayProcAudContinue = HI_TRUE;
                    }
                }
            }
        }
    }

    if ( !pAvplay->VidEnable && pAvplay->bVidPreEnable)
    {
        if (0 != pAvplay->PreVidWaterLine)
        {
            AVPLAY_CalPreBufThreshold(pAvplay);
            if (pAvplay->VidPreBufThreshold != 0)
            {
                Ret = HI_MPI_DMX_GetPESBufferStatus(pAvplay->hDmxVid, &VidChnBuf);
                if ( HI_SUCCESS != Ret )
                {
                    HI_ERR_AVPLAY("HI_MPI_DMX_GetPESBufferStatus failed:%#x\n",Ret);
                }

                if ( VidChnBuf.u32BufSize == 0 )
                {
                    HI_ERR_AVPLAY("VidChnBuf.u32BufSize == 0\n");
                }
                else
                {
                    VidBufPercent = VidChnBuf.u32UsedSize * 100 / VidChnBuf.u32BufSize;

                    if ( VidBufPercent > pAvplay->VidPreBufThreshold )
                    {
                        Ret = HI_MPI_DMX_AcquireEs(pAvplay->hDmxVid, &VidDmxEsBuf);
                        if ( HI_SUCCESS != Ret  )
                        {
                            HI_ERR_AVPLAY("HI_MPI_DMX_AcquireEs failed:%#x\n",Ret);
                        }
                        else
                        {
                            pAvplay->PreVidPts = VidDmxEsBuf.u32PtsMs;
                            (HI_VOID)HI_MPI_DMX_ReleaseEs(pAvplay->hDmxVid, &VidDmxEsBuf);
                        }
                    }
                }
            }
        }
        else
        {
            AVPLAY_FindIFrameFromDmxEsBuf(pAvplay);
        }
    }

    AVPLAY_CalcAudPreStartWaterLine(pAvplay);

    return;
}

HI_UNF_AVPLAY_BUF_STATE_E AVPLAY_CaclAudBufState(const AVPLAY_S *pAvplay, HI_U32 UsedBufPercent)
{
    HI_UNF_AVPLAY_BUF_STATE_E          CurBufState = HI_UNF_AVPLAY_BUF_STATE_NORMAL;
    HI_U32                             u32FrmTime = 0;

    if (UsedBufPercent >= AVPLAY_ES_AUD_FULL_PERCENT)
    {
        CurBufState = HI_UNF_AVPLAY_BUF_STATE_FULL;
    }
    else if ((UsedBufPercent >= AVPLAY_ES_AUD_HIGH_PERCENT)
             && (UsedBufPercent < AVPLAY_ES_AUD_FULL_PERCENT) )
    {
        CurBufState = HI_UNF_AVPLAY_BUF_STATE_HIGH;
    }
    else if (UsedBufPercent < AVPLAY_ES_AUD_LOW_PERCENT)
    {
        u32FrmTime = pAvplay->AudInfo.FrameNum * pAvplay->AudInfo.FrameTime;

        if (UsedBufPercent < AVPLAY_ES_AUD_EMPTY_PERCENT)
        {
            if (pAvplay->AudInfo.BufTime + u32FrmTime <= 150)
            {
                CurBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
            }
            else
            {
                CurBufState = HI_UNF_AVPLAY_BUF_STATE_LOW;
            }
        }
        else
        {
            if (pAvplay->AudInfo.BufTime + u32FrmTime <= 150)
            {
                CurBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
            }
            else if (pAvplay->AudInfo.BufTime + u32FrmTime <= 240)
            {
                CurBufState = HI_UNF_AVPLAY_BUF_STATE_LOW;
            }
        }
    }

    return CurBufState;
}

HI_UNF_AVPLAY_BUF_STATE_E AVPLAY_CaclVidBufState(const AVPLAY_S *pAvplay, HI_U32 UsedBufPercent)
{
    HI_S32                             Ret = HI_SUCCESS;
    HI_UNF_AVPLAY_BUF_STATE_E          CurBufState = HI_UNF_AVPLAY_BUF_STATE_NORMAL;
    VDEC_FRMSTATUSINFO_S               VdecFrmBuf = {0};
    HI_DRV_WIN_PLAY_INFO_S             WinInfo;
    HI_U32                             u32StrmTime = 0;

    memset(&WinInfo, 0x0, sizeof(HI_DRV_WIN_PLAY_INFO_S));

    if (UsedBufPercent >= AVPLAY_ES_VID_FULL_PERCENT)
    {
        CurBufState = HI_UNF_AVPLAY_BUF_STATE_FULL;
    }
    else if ((UsedBufPercent >= AVPLAY_ES_VID_HIGH_PERCENT) && (UsedBufPercent < AVPLAY_ES_VID_FULL_PERCENT))
    {
        CurBufState = HI_UNF_AVPLAY_BUF_STATE_HIGH;
    }
    else if (UsedBufPercent < AVPLAY_ES_VID_LOW_PERCENT)
    {
        if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
        {
            return HI_UNF_AVPLAY_BUF_STATE_LOW;
        }

        Ret = HI_MPI_VDEC_GetChanFrmStatusInfo(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort, &VdecFrmBuf);
        Ret |= HI_MPI_WIN_GetPlayInfo(pAvplay->MasterFrmChn.hWindow, &WinInfo);
        if (Ret != HI_SUCCESS)
        {
            return HI_UNF_AVPLAY_BUF_STATE_LOW;
        }

        /*InBps is too small*/
        if(VdecFrmBuf.u32StrmInBps < 100)
        {
            u32StrmTime = 0;
        }
        else
        {
            u32StrmTime = (VdecFrmBuf.u32StrmSize * 1000)/(VdecFrmBuf.u32StrmInBps);
        }

        if (WinInfo.u32FrameNumInBufQn <= 1)
        {
            CurBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
        }
        else if (WinInfo.u32FrameNumInBufQn + VdecFrmBuf.u32DecodedFrmNum <= 5)
        {
            if (u32StrmTime <= 80)
            {
                CurBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
            }
            else
            {
                CurBufState = HI_UNF_AVPLAY_BUF_STATE_LOW;
            }
        }
        else if (WinInfo.u32FrameNumInBufQn + VdecFrmBuf.u32DecodedFrmNum <= 10)
        {
            if (u32StrmTime <= 40)
            {
                CurBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
            }
            else if (u32StrmTime <= 80)
            {
                CurBufState = HI_UNF_AVPLAY_BUF_STATE_LOW;
            }
            else
            {
                CurBufState = HI_UNF_AVPLAY_BUF_STATE_NORMAL;
            }
        }
        else
        {
            CurBufState = HI_UNF_AVPLAY_BUF_STATE_NORMAL;
        }
    }

    return CurBufState;
}

HI_VOID AVPLAY_GetAudBufState(AVPLAY_S *pAvplay, HI_U32 *pAudBufPercent, HI_UNF_AVPLAY_BUF_STATE_E *pCurAudBufState)
{
    HI_S32                             Ret;
    ADEC_BUFSTATUS_S                   AdecBuf = {0};
    HI_MPI_DMX_BUF_STATUS_S            AudChnBuf = {0};
    HI_UNF_AVPLAY_BUF_STATE_E          CurAudBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
    HI_U32                             AudBufPercent = 0;

    Ret = HI_MPI_ADEC_GetInfo(pAvplay->hAdec, HI_MPI_ADEC_BUFFERSTATUS, &AdecBuf);
    if (HI_SUCCESS == Ret)
    {
        if(HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            Ret = HI_MPI_DMX_GetPESBufferStatus(pAvplay->hDmxAud[pAvplay->CurDmxAudChn], &AudChnBuf);
            if (HI_SUCCESS == Ret)
            {
                if ((AudChnBuf.u32BufSize + AdecBuf.u32BufferSize) > 0)
                {
                    AudBufPercent = (AudChnBuf.u32UsedSize + AdecBuf.u32BufferUsed) * 100 / (AudChnBuf.u32BufSize + AdecBuf.u32BufferSize);
                }
                else
                {
                    AudBufPercent = 0;
                }

                CurAudBufState = AVPLAY_CaclAudBufState(pAvplay, AudBufPercent);
            }
        }
        else
        {
            if (AdecBuf.u32BufferSize > 0)
            {
                AudBufPercent = AdecBuf.u32BufferUsed * 100 / AdecBuf.u32BufferSize;
            }
            else
            {
                AudBufPercent = 0;
            }

            CurAudBufState = AVPLAY_CaclBufState(pAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, AudBufPercent);
        }
    }

    if (CurAudBufState != pAvplay->PreAudBufState)
    {
        if (!pAvplay->VidEnable)
        {
            AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_RNG_BUF_STATE, &CurAudBufState);
        }

        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_AUD_BUF_STATE, &CurAudBufState);

        pAvplay->PreAudBufState = CurAudBufState;
    }

    *pAudBufPercent  = AudBufPercent;
    *pCurAudBufState = CurAudBufState;

    return;
}

HI_VOID AVPLAY_GetVidBufState(AVPLAY_S *pAvplay, HI_U32 *pVidBufPercent, HI_UNF_AVPLAY_BUF_STATE_E *pCurVidBufState)
{
    HI_S32                             Ret;
    VDEC_STATUSINFO_S                  VdecBuf = {0};
    HI_MPI_DMX_BUF_STATUS_S            VidChnBuf = {0};
    HI_UNF_AVPLAY_BUF_STATE_E          CurVidBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
    HI_U32                             VidBufPercent = 0;

    Ret = HI_MPI_VDEC_GetChanStatusInfo(pAvplay->hVdec, &VdecBuf);
    if(HI_SUCCESS == Ret)
    {
        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            Ret = HI_MPI_DMX_GetPESBufferStatus(pAvplay->hDmxVid, &VidChnBuf);
            if (HI_SUCCESS == Ret)
            {
                if (VidChnBuf.u32BufSize > 0)
                {
                    VidBufPercent = VidChnBuf.u32UsedSize * 100 / VidChnBuf.u32BufSize;
                }
                else
                {
                    VidBufPercent = 0;
                }

                CurVidBufState = AVPLAY_CaclVidBufState(pAvplay, VidBufPercent);
            }
        }
        else
        {
            if (VdecBuf.u32BufferSize > 0)
            {
                VidBufPercent = VdecBuf.u32BufferUsed * 100 / VdecBuf.u32BufferSize;
            }
            else
            {
                VidBufPercent = 0;
            }

            CurVidBufState = AVPLAY_CaclBufState(pAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, VidBufPercent);
        }
    }

    if (CurVidBufState != pAvplay->PreVidBufState)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_RNG_BUF_STATE, &CurVidBufState);

        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_VID_BUF_STATE, &CurVidBufState);

        pAvplay->PreVidBufState = CurVidBufState;
    }

    *pVidBufPercent  = VidBufPercent;
    *pCurVidBufState = CurVidBufState;

}

HI_VOID AVPLAY_ProcCheckBuf(AVPLAY_S *pAvplay)
{
    HI_S32                             Ret;

    HI_UNF_AVPLAY_BUF_STATE_E          CurVidBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
    HI_UNF_AVPLAY_BUF_STATE_E          CurAudBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
    HI_U32                             VidBufPercent = 0;
    HI_U32                             AudBufPercent = 0;

    HI_UNF_DMX_PORT_MODE_E             PortMode = HI_UNF_DMX_PORT_MODE_BUTT;

    HI_BOOL                            RealModeFlag = HI_FALSE;
    HI_BOOL                            ResetProc = HI_FALSE;

    SYNC_BUF_STATUS_S                  SyncBufStatus = {0};
    SYNC_BUF_STATE_E                   SyncAudBufState = SYNC_BUF_STATE_NORMAL;
    SYNC_BUF_STATE_E                   SyncVidBufState = SYNC_BUF_STATE_NORMAL;

    if (   (HI_UNF_AVPLAY_STATUS_STOP == pAvplay->CurStatus)
        || (HI_UNF_AVPLAY_STATUS_PAUSE == pAvplay->CurStatus))
    {
        return;
    }

    if (pAvplay->AudEnable)
    {
        AVPLAY_GetAudBufState(pAvplay, &AudBufPercent, &CurAudBufState);
        SyncAudBufState = (SYNC_BUF_STATE_E)CurAudBufState;
    }

    if (pAvplay->VidEnable)
    {
        AVPLAY_GetVidBufState(pAvplay, &VidBufPercent, &CurVidBufState);
        SyncVidBufState = (SYNC_BUF_STATE_E)CurVidBufState;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = HI_MPI_DMX_GetPortMode(pAvplay->AvplayAttr.u32DemuxId, &PortMode);
        if (HI_SUCCESS == Ret)
        {
            RealModeFlag = (HI_UNF_DMX_PORT_MODE_RAM == PortMode) ? HI_FALSE : HI_TRUE;
        }
    }

    /*  real mode */
    if (RealModeFlag)
    {
        if (HI_UNF_AVPLAY_BUF_STATE_FULL == CurAudBufState)
        {
            ResetProc = HI_TRUE;
            pAvplay->DebugInfo.AudOverflowNum++;
            HI_ERR_AVPLAY("Aud Dmx Buf overflow, reset.\n");
        }

        if (pAvplay->VidDiscard)
        {
            if (VidBufPercent <= 60)
            {
                pAvplay->VidDiscard = HI_FALSE;
            }
        }
        else
        {
            if (HI_UNF_AVPLAY_BUF_STATE_FULL == CurVidBufState)
            {
                if (HI_UNF_AVPLAY_OVERFLOW_RESET == pAvplay->OverflowProc)
                {
                    ResetProc = HI_TRUE;
                    pAvplay->DebugInfo.VidOverflowNum++;
                    HI_ERR_AVPLAY("Vid Dmx Buf overflow, reset.\n");
                }
                else
                {
                    pAvplay->VidDiscard = HI_TRUE;
                    pAvplay->DebugInfo.VidOverflowNum++;

                    (HI_VOID)AVPLAY_ResetAudChn(pAvplay);

                    HI_ERR_AVPLAY("Vid Dmx Buf overflow, discard.\n");
                }
            }
        }

        if (ResetProc)
        {
            (HI_VOID)AVPLAY_Reset(pAvplay);
            pAvplay->VidDiscard = HI_FALSE;
        }
        else
        {
            SyncBufStatus.AudBufPercent = AudBufPercent;
            SyncBufStatus.AudBufState = SyncAudBufState;
            SyncBufStatus.VidBufPercent = VidBufPercent;
            SyncBufStatus.VidBufState = SyncVidBufState;
            SyncBufStatus.bOverflowDiscFrm = pAvplay->VidDiscard;
            (HI_VOID)HI_MPI_SYNC_SetBufState(pAvplay->hSync,SyncBufStatus);
        }
    }
    else
    {
        if (SyncAudBufState == SYNC_BUF_STATE_LOW || SyncAudBufState == SYNC_BUF_STATE_EMPTY)
        {
            SyncBufStatus.AudBufState = SyncAudBufState;
        }
        else
        {
            SyncBufStatus.AudBufState = SYNC_BUF_STATE_NORMAL;
        }

        SyncBufStatus.AudBufPercent = AudBufPercent;

        if (SyncVidBufState == SYNC_BUF_STATE_LOW || SyncVidBufState == SYNC_BUF_STATE_EMPTY)
        {
            SyncBufStatus.VidBufState = SyncVidBufState;
        }
        else
        {
            SyncBufStatus.VidBufState = SYNC_BUF_STATE_NORMAL;
        }

        SyncBufStatus.VidBufPercent = VidBufPercent;

        SyncBufStatus.bOverflowDiscFrm = pAvplay->VidDiscard;

        (HI_VOID)HI_MPI_SYNC_SetBufState(pAvplay->hSync,SyncBufStatus);
    }

    return;
}

static HI_VOID AVPLAY_DRV2UNF_VidFrm(HI_DRV_VIDEO_FRAME_S *pstDRVFrm, HI_UNF_VIDEO_FRAME_INFO_S *pstUNFFrm)
{
    pstUNFFrm->u32FrameIndex = pstDRVFrm->u32FrameIndex;
    pstUNFFrm->stVideoFrameAddr[0].u32YAddr = pstDRVFrm->stBufAddr[0].u32PhyAddr_Y;
    pstUNFFrm->stVideoFrameAddr[0].u32CAddr = pstDRVFrm->stBufAddr[0].u32PhyAddr_C;
    pstUNFFrm->stVideoFrameAddr[0].u32CrAddr = pstDRVFrm->stBufAddr[0].u32PhyAddr_Cr;
    pstUNFFrm->stVideoFrameAddr[0].u32YStride = pstDRVFrm->stBufAddr[0].u32Stride_Y;
    pstUNFFrm->stVideoFrameAddr[0].u32CStride = pstDRVFrm->stBufAddr[0].u32Stride_C;
    pstUNFFrm->stVideoFrameAddr[0].u32CrStride = pstDRVFrm->stBufAddr[0].u32Stride_Cr;
    pstUNFFrm->stVideoFrameAddr[1].u32YAddr = pstDRVFrm->stBufAddr[1].u32PhyAddr_Y;
    pstUNFFrm->stVideoFrameAddr[1].u32CAddr = pstDRVFrm->stBufAddr[1].u32PhyAddr_C;
    pstUNFFrm->stVideoFrameAddr[1].u32CrAddr = pstDRVFrm->stBufAddr[1].u32PhyAddr_Cr;
    pstUNFFrm->stVideoFrameAddr[1].u32YStride = pstDRVFrm->stBufAddr[1].u32Stride_Y;
    pstUNFFrm->stVideoFrameAddr[1].u32CStride = pstDRVFrm->stBufAddr[1].u32Stride_C;
    pstUNFFrm->stVideoFrameAddr[1].u32CrStride = pstDRVFrm->stBufAddr[1].u32Stride_Cr;
    pstUNFFrm->u32Width = pstDRVFrm->u32Width;
    pstUNFFrm->u32Height = pstDRVFrm->u32Height;
    pstUNFFrm->u32SrcPts = pstDRVFrm->u32SrcPts;
    pstUNFFrm->u32Pts = pstDRVFrm->u32Pts;
    pstUNFFrm->u32AspectWidth = pstDRVFrm->u32AspectWidth;
    pstUNFFrm->u32AspectHeight = pstDRVFrm->u32AspectHeight;
    pstUNFFrm->stFrameRate.u32fpsInteger = pstDRVFrm->u32FrameRate/1000;
    pstUNFFrm->stFrameRate.u32fpsDecimal = pstDRVFrm->u32FrameRate % 1000;
    pstUNFFrm->bProgressive = pstDRVFrm->bProgressive;

    switch (pstDRVFrm->ePixFormat)
    {
        case HI_DRV_PIX_FMT_NV61_2X1:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_422;
            break;
        case HI_DRV_PIX_FMT_NV21:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_420;
            break;
        case HI_DRV_PIX_FMT_NV80:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_400;
            break;
        case HI_DRV_PIX_FMT_NV12_411:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_411;
            break;
        case HI_DRV_PIX_FMT_NV61:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_422_1X2;
            break;
        case HI_DRV_PIX_FMT_NV42:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_444;
            break;
        case HI_DRV_PIX_FMT_UYVY:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_UYVY;
            break;
        case HI_DRV_PIX_FMT_YUYV:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_YUYV;
            break;
        case HI_DRV_PIX_FMT_YVYU:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_YVYU;
            break;

        case HI_DRV_PIX_FMT_YUV400:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_400;
            break;
        case HI_DRV_PIX_FMT_YUV411:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_411;
            break;
        case HI_DRV_PIX_FMT_YUV420p:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_420;
            break;
        case HI_DRV_PIX_FMT_YUV422_1X2:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_422_1X2;
            break;
        case HI_DRV_PIX_FMT_YUV422_2X1:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_422_2X1;
            break;
        case HI_DRV_PIX_FMT_YUV_444:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_444;
            break;
        case HI_DRV_PIX_FMT_YUV410p:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_410;
            break;
        default:
            pstUNFFrm->enVideoFormat = HI_UNF_FORMAT_YUV_BUTT;
            break;
    }

    switch (pstDRVFrm->enFieldMode)
    {
        case HI_DRV_FIELD_TOP:
        {
            pstUNFFrm->enFieldMode = HI_UNF_VIDEO_FIELD_TOP;
            break;
        }
        case HI_DRV_FIELD_BOTTOM:
        {
            pstUNFFrm->enFieldMode = HI_UNF_VIDEO_FIELD_BOTTOM;
            break;
        }
        case HI_DRV_FIELD_ALL:
        {
            pstUNFFrm->enFieldMode = HI_UNF_VIDEO_FIELD_ALL;
            break;
        }
        default:
        {
            pstUNFFrm->enFieldMode = HI_UNF_VIDEO_FIELD_BUTT;
            break;
        }
    }

    pstUNFFrm->bTopFieldFirst = pstDRVFrm->bTopFieldFirst;

    switch (pstDRVFrm->eFrmType)
    {
        case HI_DRV_FT_NOT_STEREO:
        {
            pstUNFFrm->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_NONE;
            break;
        }
        case HI_DRV_FT_SBS:
        {
            pstUNFFrm->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_SIDE_BY_SIDE;
            break;
        }
        case HI_DRV_FT_TAB:
        {
            pstUNFFrm->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_TOP_AND_BOTTOM;
            break;
        }
        case HI_DRV_FT_FPK:
        {
            pstUNFFrm->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_TIME_INTERLACED;
            break;
        }
        default:
        {
            pstUNFFrm->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_BUTT;
            break;
        }
    }

    pstUNFFrm->u32Circumrotate = pstDRVFrm->u32Circumrotate;
    pstUNFFrm->bVerticalMirror = pstDRVFrm->bToFlip_V;
    pstUNFFrm->bHorizontalMirror = pstDRVFrm->bToFlip_H;
    pstUNFFrm->u32DisplayWidth = (HI_U32)pstDRVFrm->stDispRect.s32Width;
    pstUNFFrm->u32DisplayHeight = (HI_U32)pstDRVFrm->stDispRect.s32Height;
    pstUNFFrm->u32DisplayCenterX = (HI_U32)pstDRVFrm->stDispRect.s32X;
    pstUNFFrm->u32DisplayCenterY = (HI_U32)pstDRVFrm->stDispRect.s32Y;
    pstUNFFrm->u32ErrorLevel = pstDRVFrm->u32ErrorLevel;

    memset(pstUNFFrm->u32Private, 0, sizeof(pstUNFFrm->u32Private));

    return;
}

HI_VOID AVPLAY_ProcVidEvent(AVPLAY_S *pAvplay)
{
    VDEC_EVENT_S              VdecEvent = {0};
    HI_UNF_VIDEO_USERDATA_S   VdecUsrData;
    SYNC_DEC_PTS_S            SyncDecPts;
    HI_S32                    Ret;

    if (!pAvplay->VidEnable)
    {
        return;
    }

    if (pAvplay->bVidFirstDispFrame)
    {
        HI_U32 CurTime;

        CurTime = AVPLAY_GetSysTime();

        if (CurTime >= pAvplay->VidFirstQueueFrameTime + pAvplay->VidFirstFrameDispTime)
        {
            AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_FIRST_FRAME_DISPLAYED, HI_NULL);
            pAvplay->bVidFirstDispFrame = HI_FALSE;
        }
    }

    Ret = HI_MPI_VDEC_CheckNewEvent(pAvplay->hVdec, &VdecEvent);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_AVPLAY("HI_MPI_VDEC_CheckNewEvent failed 0x%x\n", Ret);
        return;
    }

    if (VdecEvent.bNewUserData)
    {
        Ret = HI_MPI_VDEC_ChanRecvUsrData(pAvplay->hVdec, &VdecUsrData);
        if (HI_SUCCESS == Ret)
        {
            AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_NEW_USER_DATA, &VdecUsrData);
        }
        else
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanRecvUsrData failed 0x%x\n", Ret);
        }
    }

    if (VdecEvent.bNormChange)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_NORM_SWITCH, &VdecEvent.stNormChangeParam);
    }

    if (VdecEvent.bFramePackingChange)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_FRAMEPACKING_CHANGE, &VdecEvent.enFramePackingType);
    }

    if (VdecEvent.bIFrameErr)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_IFRAME_ERR, HI_NULL);
    }

    if (VdecEvent.bUnSupportStream)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_VID_UNSUPPORT, HI_NULL);
    }

    if (0 != VdecEvent.u32ErrRatio)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_VID_ERR_RATIO, &VdecEvent.u32ErrRatio);
    }

    if (VdecEvent.stProbeStreamInfo.bProbeCodecTypeChangeFlag)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_VID_ERR_TYPE, &VdecEvent.stProbeStreamInfo.enCodecType);
    }

    if (VdecEvent.bFirstValidPts)
    {
        SyncDecPts.VidDecPts = VdecEvent.bFirstValidPts;
        (HI_VOID)HI_MPI_SYNC_SetExtInfo(pAvplay->hSync, SYNC_EXT_INFO_FIRST_PTS, (HI_VOID *)&SyncDecPts);
    }

    if (VdecEvent.bSecondValidPts)
    {
        SyncDecPts.VidDecPts = VdecEvent.u32SecondValidPts;
        (HI_VOID)HI_MPI_SYNC_SetExtInfo(pAvplay->hSync, SYNC_EXT_INFO_SECOND_PTS, (HI_VOID *)&SyncDecPts);
    }

    if (VdecEvent.bErrorFrame)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_VID_ERR_FRAME, HI_NULL);
    }

    return;
}

HI_VOID AVPLAY_ProcAudEvent(AVPLAY_S *pAvplay)
{
    HI_S32          Ret;
    ADEC_EVENT_S    AdecEvent;

    if (!pAvplay->AudEnable)
    {
        return;
    }

    Ret = HI_MPI_ADEC_CheckNewEvent(pAvplay->hAdec, &AdecEvent);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_AVPLAY("HI_MPI_ADEC_CheckNewEvent failed 0x%x\n", Ret);
        return;
    }

    if (AdecEvent.bFrameInfoChange)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_AUD_INFO_CHANGE, &AdecEvent.stStreamInfo);
    }

    if (AdecEvent.bUnSupportFormat)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_AUD_UNSUPPORT, HI_NULL);
    }

    if (AdecEvent.bStreamCorrupt)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_AUD_FRAME_ERR, HI_NULL);
    }

    return;
}

HI_VOID AVPLAY_ProcSyncEvent(AVPLAY_S *pAvplay)
{
    HI_S32              Ret;
    SYNC_EVENT_S        SyncEvent = {0};

    Ret = HI_MPI_SYNC_CheckNewEvent(pAvplay->hSync, &SyncEvent);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("HI_MPI_SYNC_CheckNewEvent failed 0x%x\n", Ret);
        return;
    }

    if (SyncEvent.bVidPtsJump)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_SYNC_PTS_JUMP, &SyncEvent.VidPtsJumpParam);
    }

    if (SyncEvent.bAudPtsJump)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_SYNC_PTS_JUMP, &SyncEvent.AudPtsJumpParam);
    }

    if (SyncEvent.bStatChange)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_SYNC_STAT_CHANGE, &SyncEvent.StatParam);
    }

    return;
}

HI_VOID AVPLAY_ProcStatusEvent(AVPLAY_S *pAvplay)
{
    HI_S32                      Ret;
    HI_UNF_AVPLAY_STATUS_INFO_S StatusInfo;

    if (pAvplay->EvtCbFunc64Flag)
    {
        if (!pAvplay->EvtCbFunc64[HI_UNF_AVPLAY_EVENT_STATUS_REPORT])
        {
            return;
        }
    }
#ifndef __aarch64__
    else
    {
        if (!pAvplay->EvtCbFunc[HI_UNF_AVPLAY_EVENT_STATUS_REPORT])
        {
            return;
        }
    }
#endif

    Ret = AVPLAY_GetStatusInfo(pAvplay, &StatusInfo);
    if (HI_SUCCESS == Ret)
    {
        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_STATUS_REPORT, &StatusInfo);
    }

    return;
}

HI_VOID AVPLAY_ProcCheckStandBy(AVPLAY_S *pAvplay)
{
    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        HI_U32 u32ResumeCount = 0;
        (HI_VOID)HI_MPI_DMX_GetResumeCount(&u32ResumeCount);

        if (HI_FALSE == pAvplay->bSetResumeCnt)
        {
            pAvplay->u32ResumeCount = u32ResumeCount;
            pAvplay->bSetResumeCnt  = HI_TRUE;
            return ;
        }

        /*ts mode, we need reset avplay when system standby*/
        if (pAvplay->u32ResumeCount != u32ResumeCount)
        {
            pAvplay->u32ResumeCount = u32ResumeCount;
            (HI_VOID)AVPLAY_Reset(pAvplay);
            HI_WARN_AVPLAY("System standby, now reset the AVPLAY!\n");
        }
    }

    return;
}

extern HI_S32 HI_MPI_PMOC_BoostCpuFreq(HI_VOID);

HI_VOID AVPLAY_ProcUnloadTime(AVPLAY_S *pAvplay)
{
    HI_S32              Ret                     = HI_SUCCESS;
    HI_U32              AoUnloadTime            = 0;
    HI_U32              WinUnloadTime           = 0;
    HI_U32              AudThreadScheTimeOutCnt = 0;
    HI_U32              VidThreadScheTimeOutCnt = 0;
    HI_BOOL             BoostCpuFreq            = HI_FALSE;

    if (!pAvplay->bBoostCpuFreq)
    {
        return;
    }

    // CPU schedule is disable when avplay on stop/pause status
    if ((HI_UNF_AVPLAY_STATUS_STOP == pAvplay->CurStatus) || (HI_UNF_AVPLAY_STATUS_PAUSE == pAvplay->CurStatus))
    {
        return;
    }

    // CPU schedule is disable when network disconnect
    if (   (pAvplay->PreVidBufState == HI_UNF_AVPLAY_BUF_STATE_EMPTY)
        && (pAvplay->PreAudBufState == HI_UNF_AVPLAY_BUF_STATE_EMPTY) )
    {
        return;
    }

    AudThreadScheTimeOutCnt = pAvplay->DebugInfo.AudThreadScheTimeOutCnt;
    VidThreadScheTimeOutCnt = pAvplay->DebugInfo.VidThreadScheTimeOutCnt;

    if (pAvplay->MasterFrmChn.hWindow != HI_INVALID_HANDLE)
    {
        Ret = HI_MPI_WIN_GetUnloadTimes(pAvplay->MasterFrmChn.hWindow, &WinUnloadTime);
        if (HI_SUCCESS == Ret)
        {
            if (   (pAvplay->WinUnloadTime != WinUnloadTime)
                || (pAvplay->VidThreadScheTimeOut != VidThreadScheTimeOutCnt) )
            {
                BoostCpuFreq = HI_TRUE;

                pAvplay->WinUnloadTime = WinUnloadTime;
                pAvplay->VidThreadScheTimeOut = VidThreadScheTimeOutCnt;
            }
        }
    }

    if (pAvplay->AudEnable)
    {
        Ret = HI_MPI_AO_SND_GetXrunCount(HI_UNF_SND_0, &AoUnloadTime);
        if (HI_SUCCESS == Ret)
        {
            if (   (pAvplay->AoUnloadTime != AoUnloadTime)
                || (pAvplay->AudThreadScheTimeOut != AudThreadScheTimeOutCnt) )
            {
                BoostCpuFreq = HI_TRUE;

                pAvplay->AoUnloadTime = AoUnloadTime;
                pAvplay->AudThreadScheTimeOut = AudThreadScheTimeOutCnt;
            }
        }
    }

    if (BoostCpuFreq)
    {
        Ret = HI_MPI_PMOC_BoostCpuFreq();
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_PMOC_BoostCpuFreq failed 0x%x\n", Ret);
            return;
        }

        pAvplay->DebugInfo.CpuFreqScheTimeCnt++;
     }

     return;
}

HI_VOID *AVPLAY_StatThread(HI_VOID *Arg)
{
    AVPLAY_S *pAvplay = (AVPLAY_S*)Arg;

    while (pAvplay->AvplayThreadRun)
    {
        if (pAvplay->bSetEosFlag)
        {
            AVPLAY_ProcEos(pAvplay);
        }

        AVPLAY_ProcVidEvent(pAvplay);

        AVPLAY_ProcAudEvent(pAvplay);

        AVPLAY_ProcSyncEvent(pAvplay);

        AVPLAY_ProcStatusEvent(pAvplay);

        AVPLAY_ProcUnloadTime(pAvplay);

        if (pAvplay->AvplayThreadRun)
        {
            AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);
            AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

            AVPLAY_ProcDmxBuf(pAvplay);

            AVPLAY_ProcCheckBuf(pAvplay);

            AVPLAY_ProcCheckStandBy(pAvplay);

            AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
            AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
        }

        if (pAvplay->AvplayThreadRun)
        {
            usleep(AVPLAY_SYS_SLEEP_TIME * 1000);
        }
    }

    return HI_NULL;
}

HI_VOID *AVPLAY_AudDataThread(HI_VOID *Arg)
{
    AVPLAY_S   *pAvplay     = (AVPLAY_S *)Arg;
    HI_U32      BeginTime   = 0;
    HI_U32      EndTime     = 0;

    pAvplay->ThreadID = syscall(__NR_gettid);

    while (pAvplay->AvplayThreadRun)
    {
        HI_SYS_GetTimeStampMs(&BeginTime);

        if ((EndTime != 0) && (BeginTime - EndTime > AVPLAY_THREAD_TIMEOUT))
        {
            pAvplay->DebugInfo.AudThreadScheTimeOutCnt++;
        }

        AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);

        pAvplay->AvplayProcAudContinue = HI_FALSE;

        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            AVPLAY_ProcDmxToAdec(pAvplay);
            AVPLAY_ProcDmxToAdAdec(pAvplay);
        }

        AVPLAY_ProcAdecToAo(pAvplay);

        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);

        HI_SYS_GetTimeStampMs(&EndTime);

        if (EndTime - BeginTime > AVPLAY_THREAD_TIMEOUT)
        {
            pAvplay->DebugInfo.AudThreadExeTimeOutCnt++;
        }

        if (pAvplay->AvplayProcAudContinue)
        {
            continue;
        }

        (HI_VOID)usleep(AVPLAY_SYS_SLEEP_TIME * 1000);
    }

    return HI_NULL;
}

HI_VOID *AVPLAY_VidDataThread(HI_VOID *Arg)
{
    AVPLAY_S   *pAvplay     = (AVPLAY_S *)Arg;
    HI_U32      BeginTime   = 0;
    HI_U32      EngTime     = 0;

    while (pAvplay->AvplayThreadRun)
    {
        HI_SYS_GetTimeStampMs(&BeginTime);

        if ((EngTime != 0) && (BeginTime - EngTime > AVPLAY_THREAD_TIMEOUT))
        {
            pAvplay->DebugInfo.VidThreadScheTimeOutCnt++;
        }

        AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

        AVPLAY_ProcVdecToVo(pAvplay);

        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);

        HI_SYS_GetTimeStampMs(&EngTime);

        if (EngTime - BeginTime > AVPLAY_THREAD_TIMEOUT)
        {
            pAvplay->DebugInfo.VidThreadExeTimeOutCnt++;
        }

        if (pAvplay->AvplayProcVidContinue)
        {
            (HI_VOID)usleep(AVPLAY_VIDTHREAD_SLEEP_TIME * 1000);
            continue;
        }

        (HI_VOID)usleep(AVPLAY_SYS_SLEEP_TIME * 1000);
    }

    return HI_NULL;
}

HI_VOID AVPLAY_ResetProcFlag(AVPLAY_S *pAvplay)
{
    HI_U32 i;

    pAvplay->AvplayProcAudContinue = HI_FALSE;
    pAvplay->AvplayProcVidContinue = HI_FALSE;

    for (i=0; i<AVPLAY_PROC_BUTT; i++)
    {
        pAvplay->AvplayProcDataFlag[i] = HI_FALSE;
    }

    for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
    {
        pAvplay->bSendedFrmToVirtualWin[i] = HI_FALSE;
    }

    for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
    {
        pAvplay->bSendedFrmToSlaveWin[i] = HI_FALSE;
    }

    pAvplay->VidFirstFrameDispTime  = 0xFFFFFFFF;
    pAvplay->VidFirstQueueFrameTime = 0xFFFFFFFF;
    pAvplay->bVidFirstQueueFrame    = HI_FALSE;
    pAvplay->bVidFirstDispFrame     = HI_FALSE;

    pAvplay->bSendedFrmToMasterWin  = HI_FALSE;
    pAvplay->bProcFrmToVirtualWin   = HI_FALSE;

    pAvplay->PreVidBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
    pAvplay->PreAudBufState = HI_UNF_AVPLAY_BUF_STATE_EMPTY;
    pAvplay->VidDiscard = HI_FALSE;

    pAvplay->bSetEosFlag = HI_FALSE;
    pAvplay->bSetAudEos = HI_FALSE;

    pAvplay->AdecDelayMs = 0;

    pAvplay->u32DispOptimizeFlag = 0;

    if (HI_TRUE == pAvplay->CurBufferEmptyState)
    {
       pAvplay->PreTscnt =0;
       pAvplay->PreAudEsBuf = 0;
       pAvplay->PreAudEsBufWPtr = 0;
       pAvplay->PreVidEsBuf = 0;
       pAvplay->PreVidEsBufWPtr = 0;
       pAvplay->CurBufferEmptyState = HI_FALSE;
    }
    else
    {
       pAvplay->PreTscnt = 0xFFFFFFFF;
       pAvplay->PreAudEsBuf = 0xFFFFFFFF;
       pAvplay->PreAudEsBufWPtr = 0xFFFFFFFF;
       pAvplay->PreVidEsBuf = 0xFFFFFFFF;
       pAvplay->PreVidEsBufWPtr = 0xFFFFFFFF;
    }

    memset(&pAvplay->DebugInfo, 0, sizeof(AVPLAY_DEBUG_INFO_S));

    pAvplay->DebugInfo.bAoBufEmpty = HI_TRUE;
    pAvplay->DebugInfo.bEndOfFrame = HI_TRUE;
    pAvplay->DebugInfo.UsedBufNum  = 0;

    pAvplay->DebugInfo.bEndOfStream = HI_TRUE;
    pAvplay->DebugInfo.bAllPortCompleteFrm = HI_TRUE;
    pAvplay->DebugInfo.FrameNumInBufQn = 0;

    memset(&pAvplay->LstFrmPack, 0, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));

    pAvplay->stIFrame.hport = HI_INVALID_HANDLE;
    memset(&pAvplay->stIFrame.stFrameVideo, 0x0, sizeof(HI_DRV_VIDEO_FRAME_S));

    return;
}

HI_S32 AVPLAY_CreateThread(AVPLAY_S *pAvplay)
{
    HI_S32                  Ret = 0;
    pthread_attr_t         *pThreadAttr = HI_NULL;

#if 0
    pthread_attr_t          ThreadAttr;
    struct sched_param      SchedParam;

    (HI_VOID)pthread_attr_init(&ThreadAttr);

    (HI_VOID)pthread_attr_setschedpolicy(&ThreadAttr, SCHED_FIFO);
    (HI_VOID)pthread_attr_getschedparam(&ThreadAttr, &SchedParam);
    SchedParam.sched_priority = 4;
    (HI_VOID)pthread_attr_setschedparam(&ThreadAttr, &SchedParam);

    pThreadAttr = &ThreadAttr;
#endif

    (HI_VOID)pthread_mutex_init(&pAvplay->AvplayAudThreadMutex, HI_NULL);

    (HI_VOID)pthread_mutex_init(&pAvplay->AvplayVidThreadMutex, HI_NULL);

    pAvplay->AvplayThreadRun = HI_TRUE;

    /* create avplay audio process thread */
    Ret = pthread_create(&pAvplay->AvplayAudDataThdInst, pThreadAttr, AVPLAY_AudDataThread, pAvplay);
    if (HI_SUCCESS != Ret)
    {
        goto MUTEX_DESTROY;
    }

    /* create avplay video process thread */
    Ret = pthread_create(&pAvplay->AvplayVidDataThdInst, pThreadAttr, AVPLAY_VidDataThread, pAvplay);
    if (HI_SUCCESS != Ret)
    {
        goto AUD_THREAD_DESTROY;
    }

    /* create avplay status check thread */
    Ret = pthread_create(&pAvplay->AvplayStatThdInst, pThreadAttr, AVPLAY_StatThread, pAvplay);
    if (HI_SUCCESS != Ret)
    {
        goto VID_THREAD_DESTROY;
    }

    return HI_SUCCESS;

VID_THREAD_DESTROY:
    pAvplay->AvplayThreadRun = HI_FALSE;
    (HI_VOID)pthread_join(pAvplay->AvplayVidDataThdInst, HI_NULL);

AUD_THREAD_DESTROY:
    pAvplay->AvplayThreadRun = HI_FALSE;
    (HI_VOID)pthread_join(pAvplay->AvplayAudDataThdInst, HI_NULL);

MUTEX_DESTROY:
    (HI_VOID)pthread_mutex_destroy(&pAvplay->AvplayAudThreadMutex);

    (HI_VOID)pthread_mutex_destroy(&pAvplay->AvplayVidThreadMutex);

    return HI_FAILURE;
}

HI_VOID AVPLAY_DestroyThread(AVPLAY_S *pAvplay)
{
    // stop thread
    pAvplay->AvplayThreadRun = HI_FALSE;

    (HI_VOID)pthread_join(pAvplay->AvplayAudDataThdInst, HI_NULL);

    (HI_VOID)pthread_join(pAvplay->AvplayVidDataThdInst, HI_NULL);

    (HI_VOID)pthread_join(pAvplay->AvplayStatThdInst, HI_NULL);

    (HI_VOID)pthread_mutex_destroy(&pAvplay->AvplayAudThreadMutex);

    (HI_VOID)pthread_mutex_destroy(&pAvplay->AvplayVidThreadMutex);
}

HI_S32 AVPLAY_MallocVdec(AVPLAY_S *pAvplay, const HI_VOID *pPara)
{
    HI_S32  Ret;

    Ret = HI_MPI_VDEC_AllocChan(&pAvplay->hVdec, (HI_UNF_AVPLAY_OPEN_OPT_S*)pPara);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_AllocChan failed 0x%x\n", Ret);
    }

    return Ret;
}

HI_S32 AVPLAY_FreeVdec(AVPLAY_S *pAvplay)
{
    HI_S32           Ret;

    Ret = HI_MPI_VDEC_FreeChan(pAvplay->hVdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_freeChan failed 0x%x\n", Ret);
        return Ret;
    }

    pAvplay->hVdec = HI_INVALID_HANDLE;

    return Ret;
}

HI_S32 AVPLAY_MallocAdec(AVPLAY_S *pAvplay)
{
    HI_S32           Ret;

    Ret = HI_MPI_ADEC_Open(&pAvplay->hAdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_ADEC_Open failed 0x%x\n", Ret);
    }

    return Ret;
}

HI_S32 AVPLAY_FreeAdec(AVPLAY_S *pAvplay)
{
    HI_S32           Ret;

    Ret = HI_MPI_ADEC_Close(pAvplay->hAdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_ADEC_Close failed 0x%x\n", Ret);
        return Ret;
    }

    pAvplay->hAdec = HI_INVALID_HANDLE;

    return Ret;
}

HI_S32 AVPLAY_MallocDmxChn(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_BUFID_E BufId)
{
    HI_S32                      Ret = 0;
    HI_UNF_DMX_CHAN_ATTR_S      DmxChnAttr;

    DmxChnAttr.enCRCMode    = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
    DmxChnAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;

    if (HI_UNF_AVPLAY_BUF_ID_ES_VID == BufId)
    {
        DmxChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_VID;
        DmxChnAttr.u32BufSize    = pAvplay->AvplayAttr.stStreamAttr.u32VidBufSize;
    #ifdef HI_TEE_SUPPORT
        DmxChnAttr.enSecureMode  = pAvplay->TVPAttr.bEnable ? HI_UNF_DMX_SECURE_MODE_TEE : HI_UNF_DMX_SECURE_MODE_NONE;
    #else
        DmxChnAttr.enSecureMode  = HI_UNF_DMX_SECURE_MODE_NONE;
    #endif

        Ret = HI_MPI_DMX_CreateChannel(pAvplay->AvplayAttr.u32DemuxId, &DmxChnAttr, &pAvplay->hDmxVid);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_DMX_CreateChannel failed 0x%x\n", Ret);
        }
    }
    else if (HI_UNF_AVPLAY_BUF_ID_ES_AUD == BufId)
    {
        DmxChnAttr.enChannelType    = HI_UNF_DMX_CHAN_TYPE_AUD;
        DmxChnAttr.u32BufSize       = pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize / 3;
        DmxChnAttr.enSecureMode     = pAvplay->DmxAudChnSecMode;

        Ret = HI_MPI_DMX_CreateChannel(pAvplay->AvplayAttr.u32DemuxId, &DmxChnAttr, &pAvplay->hDmxAud[0]);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_DMX_CreateChannel failed 0x%x\n", Ret);
            return HI_FAILURE;
        }

        pAvplay->DmxAudChnNum = 1;
    }

    return Ret;
}

HI_S32 AVPLAY_FreeDmxChn(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_BUFID_E BufId)
{
    HI_S32              Ret = 0;
    HI_U32              i;

    if ((HI_UNF_AVPLAY_BUF_ID_ES_VID == BufId) && (pAvplay->hDmxVid != HI_INVALID_HANDLE))
    {
        Ret = HI_MPI_DMX_DestroyChannel(pAvplay->hDmxVid);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_DMX_DestroyChannel failed 0x%x\n", Ret);
            return Ret;
        }

        pAvplay->hDmxVid = HI_INVALID_HANDLE;
    }
    else if (HI_UNF_AVPLAY_BUF_ID_ES_AUD == BufId)
    {
        for (i = 0; i < pAvplay->DmxAudChnNum; i++)
        {
            if (pAvplay->hDmxAud[i] != HI_INVALID_HANDLE)
            {
                Ret = HI_MPI_DMX_DestroyChannel(pAvplay->hDmxAud[i]);
                if (Ret != HI_SUCCESS)
                {
                    HI_ERR_AVPLAY("HI_MPI_DMX_DestroyChannel failed 0x%x\n", Ret);
                    return Ret;
                }

                pAvplay->hDmxAud[i] = HI_INVALID_HANDLE;
            }
        }

        if (HI_INVALID_HANDLE != pAvplay->hDmxAdAud)
        {
            (HI_VOID)HI_MPI_DMX_DestroyChannel(pAvplay->hDmxAdAud);
            pAvplay->hDmxAdAud = HI_INVALID_HANDLE;
        }

        pAvplay->DmxAudChnNum = 0;
        pAvplay->DmxAudChnSecMode = HI_UNF_DMX_SECURE_MODE_NONE;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_MallocVidChn(AVPLAY_S *pAvplay, const HI_VOID *pPara)
{
    HI_S32 Ret = HI_SUCCESS;

    Ret = AVPLAY_MallocVdec(pAvplay, pPara);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("Avplay malloc vdec failed.\n");
        return Ret;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
    #ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
        Ret = AVPLAY_MallocDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("Avplay malloc vid dmx chn failed.\n");
            (HI_VOID)AVPLAY_FreeVdec(pAvplay);
            return Ret;
        }

        Ret = AVPLAY_MallocVidBuffer(pAvplay, pAvplay->hVdec);
        if (Ret != HI_SUCCESS)
        {
            (HI_VOID)AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID);
            (HI_VOID)AVPLAY_FreeVdec(pAvplay);
            return Ret;
        }
    #endif
    }
    else if (HI_UNF_AVPLAY_STREAM_TYPE_ES == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = AVPLAY_MallocVidBuffer(pAvplay, pAvplay->hVdec);
        if (Ret != HI_SUCCESS)
        {
            (HI_VOID)AVPLAY_FreeVdec(pAvplay);
            return Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_FreeVidChn(AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

#ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
    Ret = AVPLAY_FreeVidBuffer(pAvplay, pAvplay->hVdec);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("Avplay free dmx vid chn failed.\n");
            return Ret;
        }
    }
#else
    if (HI_UNF_AVPLAY_STREAM_TYPE_ES == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = AVPLAY_FreeVidBuffer(pAvplay, pAvplay->hVdec);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
    }
#endif
    Ret = AVPLAY_FreeVdec(pAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("Avplay free vdec failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

#ifdef HI_AVPLAY_PLAY_AND_REC_SUPPORT
HI_S32 AVPLAY_MallocDmxChnWithPID(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_BUFID_E BufId, HI_U32 u32pid)
{
    HI_S32                      Ret = 0;
    HI_UNF_DMX_CHAN_ATTR_S      DmxChnAttr;

    DmxChnAttr.enCRCMode    = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
    DmxChnAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;

    if (HI_UNF_AVPLAY_BUF_ID_ES_VID == BufId)
    {
        DmxChnAttr.enChannelType    = HI_UNF_DMX_CHAN_TYPE_VID;
        DmxChnAttr.u32BufSize       = pAvplay->AvplayAttr.stStreamAttr.u32VidBufSize;
        DmxChnAttr.enSecureMode     = HI_UNF_DMX_SECURE_MODE_NONE;

        Ret = HI_MPI_DMX_CreateChannelWithPID(pAvplay->AvplayAttr.u32DemuxId, u32pid, &DmxChnAttr, &pAvplay->hDmxVid);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_DMX_CreateChannel failed 0x%x\n", Ret);
        }
    }
    else if (HI_UNF_AVPLAY_BUF_ID_ES_AUD == BufId)
    {
        DmxChnAttr.enChannelType    = HI_UNF_DMX_CHAN_TYPE_AUD;
        DmxChnAttr.u32BufSize       = pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize / 3;
        DmxChnAttr.enSecureMode     = HI_UNF_DMX_SECURE_MODE_NONE;

        Ret = HI_MPI_DMX_CreateChannelWithPID(pAvplay->AvplayAttr.u32DemuxId, u32pid, &DmxChnAttr, &pAvplay->hDmxAud[pAvplay->CurDmxAudChn]);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_DMX_CreateChannel failed.\n");
        }

        pAvplay->DmxAudChnNum = 1;
    }

    return Ret;
}
#endif

HI_S32 AVPLAY_MallocAudChn(AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

    Ret = AVPLAY_MallocAdec(pAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("Avplay malloc adec failed.\n");
        return Ret;
    }

#ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = AVPLAY_MallocDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("Avplay malloc aud dmx chn failed.\n");
            (HI_VOID)AVPLAY_FreeAdec(pAvplay);
            return Ret;
        }
    }
#endif

    return HI_SUCCESS;
}

HI_S32 AVPLAY_FreeAudChn(AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

#ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("Avplay free dmx aud chn failed.\n");
            return Ret;
        }
    }
#endif

    Ret = AVPLAY_FreeAdec(pAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("Avplay free vdec failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_ReallocVidChn(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_ATTR_S *pAvplayAttr)
{
    HI_S32  Ret;

    if (pAvplay->VidEnable)
    {
        HI_ERR_AVPLAY("vid chn is enable, can not set stream mode.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = AVPLAY_FreeVidBuffer(pAvplay, pAvplay->hVdec);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("Avplay free dmx vid chn failed.\n");
            return Ret;
        }
    }

    pAvplay->AvplayAttr.stStreamAttr.u32VidBufSize = pAvplayAttr->stStreamAttr.u32VidBufSize;
    pAvplay->AvplayAttr.stStreamAttr.enStreamType  = pAvplayAttr->stStreamAttr.enStreamType;

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
    #ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
        Ret = AVPLAY_MallocDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("Avplay malloc vid dmx chn failed.\n");
            return Ret;
        }

        Ret = AVPLAY_MallocVidBuffer(pAvplay, pAvplay->hVdec);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("Avplay malloc vid dmx chn failed.\n");
            (HI_VOID)AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID);
            return Ret;
        }
    #endif
    }
    else if (HI_UNF_AVPLAY_STREAM_TYPE_ES == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = AVPLAY_MallocVidBuffer(pAvplay, pAvplay->hVdec);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_ReallocAudChn(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_ATTR_S *pAvplayAttr)
{
    HI_S32  Ret;

    if (pAvplay->AudEnable)
    {
        HI_ERR_AVPLAY("aud chn is enable, can not set stream mode.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize = pAvplayAttr->stStreamAttr.u32AudBufSize;
    pAvplay->AvplayAttr.stStreamAttr.enStreamType  = pAvplayAttr->stStreamAttr.enStreamType;

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("Avplay free dmx aud chn failed.\n");
            return Ret;
        }

    #ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
        Ret = AVPLAY_MallocDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("Avplay malloc aud dmx chn failed.\n");
            return Ret;
        }
    #endif
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_SetStreamMode(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_ATTR_S *pAvplayAttr)
{
    HI_S32                      Ret;
    HI_U32                      CurVidBufSize = pAvplay->AvplayAttr.stStreamAttr.u32VidBufSize;
    HI_U32                      CurAudBufSize = pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize;
    HI_UNF_AVPLAY_STREAM_TYPE_E CurStreamType = pAvplay->AvplayAttr.stStreamAttr.enStreamType;

    switch (pAvplayAttr->stStreamAttr.enStreamType)
    {
        case HI_UNF_AVPLAY_STREAM_TYPE_TS:
        case HI_UNF_AVPLAY_STREAM_TYPE_ES:
            break;
        default:
            HI_ERR_AVPLAY("enStreamType 0x%x is invalid.\n", pAvplayAttr->stStreamAttr.enStreamType);
            return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (pAvplayAttr->stStreamAttr.u32VidBufSize > AVPLAY_MAX_VID_SIZE)
    {
        HI_ERR_AVPLAY("VidBufSize 0x%x is invalid.\n", pAvplayAttr->stStreamAttr.u32VidBufSize);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (   (pAvplayAttr->stStreamAttr.u32AudBufSize > AVPLAY_MAX_AUD_SIZE)
        || (pAvplayAttr->stStreamAttr.u32AudBufSize < AVPLAY_MIN_AUD_SIZE) )
    {
        HI_ERR_AVPLAY("AudBufSize 0x%x is invalid.\n", pAvplayAttr->stStreamAttr.u32AudBufSize);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (!memcmp(pAvplayAttr, &pAvplay->AvplayAttr, sizeof(HI_UNF_AVPLAY_ATTR_S)))
    {
        if (pAvplay->VidEnable || pAvplay->AudEnable)
        {
            return HI_FAILURE;
        }
        
        return HI_SUCCESS;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplayAttr->stStreamAttr.enStreamType)
    {
        if (pAvplayAttr->u32DemuxId != pAvplay->AvplayAttr.u32DemuxId)
        {
            pAvplay->AvplayAttr.u32DemuxId = pAvplayAttr->u32DemuxId;

            Ret = AVPLAY_ReallocVidChn(pAvplay, pAvplayAttr);
            if (HI_SUCCESS != Ret)
            {
                return Ret;
            }

            Ret = AVPLAY_ReallocAudChn(pAvplay, pAvplayAttr);
            if (HI_SUCCESS != Ret)
            {
                return Ret;
            }
        }
    }

    if (pAvplay->hVdec != HI_INVALID_HANDLE)
    {
        if (   (CurStreamType != pAvplayAttr->stStreamAttr.enStreamType)
            || (CurVidBufSize != pAvplayAttr->stStreamAttr.u32VidBufSize) )
        {
            Ret = AVPLAY_ReallocVidChn(pAvplay, pAvplayAttr);
            if (HI_SUCCESS != Ret)
            {
                return Ret;
            }
        }
    }
    else
    {
        pAvplay->AvplayAttr.stStreamAttr.u32VidBufSize = pAvplayAttr->stStreamAttr.u32VidBufSize;
        pAvplay->AvplayAttr.stStreamAttr.enStreamType  = pAvplayAttr->stStreamAttr.enStreamType;
    }

    if (pAvplay->hAdec != HI_INVALID_HANDLE)
    {
        if (   (CurStreamType != pAvplayAttr->stStreamAttr.enStreamType)
            || (CurAudBufSize != pAvplayAttr->stStreamAttr.u32AudBufSize) )
        {
            Ret = AVPLAY_ReallocAudChn(pAvplay, pAvplayAttr);
            if (HI_SUCCESS != Ret)
            {
                return Ret;
            }
        }
    }
    else
    {
        pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize = pAvplayAttr->stStreamAttr.u32AudBufSize;
        pAvplay->AvplayAttr.stStreamAttr.enStreamType  = pAvplayAttr->stStreamAttr.enStreamType;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_GetStreamMode(const AVPLAY_S *pAvplay, HI_UNF_AVPLAY_ATTR_S *pAvplayAttr)
{
    memcpy(pAvplayAttr, &pAvplay->AvplayAttr, sizeof(HI_UNF_AVPLAY_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 AVPLAY_SetAdecAttr(AVPLAY_S *pAvplay, const HI_UNF_ACODEC_ATTR_S *pAdecAttr)
{
    ADEC_ATTR_S  AdecAttr;
    HI_S32       Ret;

    if (pAvplay->AudEnable)
    {
        HI_ERR_AVPLAY("aud chn is running, can not set adec attr.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_INVALID_HANDLE == pAvplay->hAdec)
    {
        HI_ERR_AVPLAY("aud chn is close, can not set adec attr.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    AdecAttr.bEnable = HI_FALSE;
    AdecAttr.bEosState = HI_FALSE;
    AdecAttr.u32CodecID = (HI_U32)pAdecAttr->enType;
    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        AdecAttr.u32InBufSize = pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize * 2 / 3;
    }
    else
    {
        AdecAttr.u32InBufSize = pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize;
    }

    AdecAttr.u32OutBufNum = pAvplay->AdecOutBufNum;
    AdecAttr.sOpenPram = pAdecAttr->stDecodeParam;
    Ret = HI_MPI_ADEC_SetAllAttr(pAvplay->hAdec, &AdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_ADEC_SetAllAttr failed 0x%x\n", Ret);
        return Ret;
    }

    pAvplay->AdecType = (HI_U32)pAdecAttr->enType;

    return Ret;
}

HI_S32 AVPLAY_GetAdecAttr(const AVPLAY_S *pAvplay, HI_UNF_ACODEC_ATTR_S *pAdecAttr)
{
    ADEC_ATTR_S  AdecAttr;
    HI_S32       Ret;

    memset(&AdecAttr, 0x0, sizeof(ADEC_ATTR_S));

    if (HI_INVALID_HANDLE == pAvplay->hAdec)
    {
        HI_ERR_AVPLAY("aud chn is close, can not set adec attr.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_ADEC_GetAllAttr(pAvplay->hAdec, &AdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_ADEC_GetAllAttr failed 0x%x\n", Ret);
    }

    pAdecAttr->enType = (HA_CODEC_ID_E)AdecAttr.u32CodecID;
    pAdecAttr->stDecodeParam = AdecAttr.sOpenPram;

    return Ret;
}

HI_S32 AVPLAY_SetVdecAttr(AVPLAY_S *pAvplay, HI_UNF_VCODEC_ATTR_S *pVdecAttr)
{
    HI_UNF_VCODEC_ATTR_S  VdecAttr;
    HI_S32                Ret;

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_AVPLAY("vid chn is close, can not set vdec attr.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_VDEC_GetChanAttr(pAvplay->hVdec, &VdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_GetChanAttr failed.\n");
        return Ret;
    }

    if (pAvplay->VidEnable)
    {
        if (VdecAttr.enType != pVdecAttr->enType)
        {
            HI_ERR_AVPLAY("vid chn is running, can not set vdec type.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        if (HI_UNF_VCODEC_TYPE_VC1 == VdecAttr.enType
         && (VdecAttr.unExtAttr.stVC1Attr.bAdvancedProfile != pVdecAttr->unExtAttr.stVC1Attr.bAdvancedProfile
            || VdecAttr.unExtAttr.stVC1Attr.u32CodecVersion != pVdecAttr->unExtAttr.stVC1Attr.u32CodecVersion))
        {
            HI_ERR_AVPLAY("vid chn is running, can not set vdec type.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }
    }

    Ret = HI_MPI_VDEC_SetChanAttr(pAvplay->hVdec,pVdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("call HI_MPI_VDEC_SetChanAttr failed.\n");
    }

    memcpy(&pAvplay->VdecAttr, pVdecAttr, sizeof(HI_UNF_VCODEC_ATTR_S));

    return Ret;
}

HI_S32 AVPLAY_GetVdecAttr(const AVPLAY_S *pAvplay, HI_UNF_VCODEC_ATTR_S *pVdecAttr)
{
    HI_S32                Ret;

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_AVPLAY("vid chn is close, can not set vdec attr.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_VDEC_GetChanAttr(pAvplay->hVdec, pVdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("call HI_MPI_VDEC_GetChanAttr failed.\n");
    }

    return Ret;
}

HI_S32 AVPLAY_SetPid(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, const HI_U32 Pid)
{
    HI_U32       i;
#ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
    HI_S32       Ret;
#endif

    if (pAvplay->AvplayAttr.stStreamAttr.enStreamType != HI_UNF_AVPLAY_STREAM_TYPE_TS)
    {
        HI_ERR_AVPLAY("avplay is not ts mode.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_UNF_AVPLAY_ATTR_ID_AUD_PID == enAttrID)
    {
        if (HI_INVALID_HANDLE == pAvplay->hAdec)
        {
            HI_ERR_AVPLAY("aud chn is close, can not set aud pid.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

#ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
        if (pAvplay->DmxAudChnNum == 1)
#else
        if((pAvplay->DmxAudChnNum == 1)|| (pAvplay->DmxAudChnNum == 0))
#endif
        {
            if (pAvplay->AudEnable)
            {
                HI_ERR_AVPLAY("aud chn is running, can not set aud pid.\n");
                return HI_ERR_AVPLAY_INVALID_OPT;
            }

#ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
            Ret = HI_MPI_DMX_SetChannelPID(pAvplay->hDmxAud[0], Pid);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_SetChannelPID failed 0x%x\n", Ret);
                return Ret;
            }
#endif

            pAvplay->DmxAudPid[0] = Pid;

            pAvplay->CurDmxAudChn = 0;
        }
        /*multi audio*/
        else
        {
            AVPLAY_Mutex_Lock(&pAvplay->AvplayAudThreadMutex);

            for (i = 0; i < pAvplay->DmxAudChnNum; i++)
            {
                if (pAvplay->DmxAudPid[i] == Pid)
                {
                    break;
                }
            }

            if (i < pAvplay->DmxAudChnNum)
            {
                /* if the es buf has not been released */
                if (pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC])
                {
                    (HI_VOID)HI_MPI_DMX_ReleaseEs(pAvplay->hDmxAud[pAvplay->CurDmxAudChn], &pAvplay->AvplayDmxEsBuf);
                    pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC] = HI_FALSE;
                }

                pAvplay->CurDmxAudChn = i;
            }

            pAvplay->bSetAudEos = HI_FALSE;
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;

            (HI_VOID)HI_MPI_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_AUD);

            (HI_VOID)HI_MPI_ADEC_Stop(pAvplay->hAdec);

            for (i=0; i<pAvplay->TrackNum; i++)
            {
                if (HI_INVALID_HANDLE != pAvplay->hTrack[i])
                {
                    (HI_VOID)HI_MPI_AO_Track_Flush(pAvplay->hTrack[i]);
                }
            }

            if (HI_NULL != pAvplay->pstAcodecAttr)
            {
                (HI_VOID)AVPLAY_SetAdecAttr(pAvplay, (HI_UNF_ACODEC_ATTR_S *)(pAvplay->pstAcodecAttr + pAvplay->CurDmxAudChn));
            }

            (HI_VOID)HI_MPI_ADEC_Start(pAvplay->hAdec);

            (HI_VOID)HI_MPI_SYNC_Start(pAvplay->hSync, SYNC_CHAN_AUD);

            AVPLAY_Mutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        }
    }
    else if (HI_UNF_AVPLAY_ATTR_ID_VID_PID == enAttrID)
    {
        if (pAvplay->VidEnable)
        {
            HI_ERR_AVPLAY("vid chn is running, can not set vid pid.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        if (HI_INVALID_HANDLE == pAvplay->hVdec)
        {
            HI_ERR_AVPLAY("vid chn is close, can not set vid pid.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

#ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
        Ret = HI_MPI_DMX_SetChannelPID(pAvplay->hDmxVid, Pid);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_DMX_SetChannelPID failed 0x%x\n", Ret);
            return Ret;
        }
#endif

        pAvplay->DmxVidPid = Pid;
    }
    else
    {
        if (pAvplay->CurStatus != HI_UNF_AVPLAY_STATUS_STOP)
        {
            HI_ERR_AVPLAY("AVPLAY is not stopped, can not set pcr pid.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        pAvplay->DmxPcrPid = Pid;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_GetPid(const AVPLAY_S *pAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_U32 *pPid)
{
    if (pAvplay->AvplayAttr.stStreamAttr.enStreamType != HI_UNF_AVPLAY_STREAM_TYPE_TS)
    {
        HI_ERR_AVPLAY("avplay is not ts mode.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_UNF_AVPLAY_ATTR_ID_AUD_PID == enAttrID)
    {
        if (HI_INVALID_HANDLE == pAvplay->hAdec)
        {
            HI_ERR_AVPLAY("aud chn is close, can not get aud pid.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        *pPid = pAvplay->DmxAudPid[pAvplay->CurDmxAudChn];
    }
    else if (HI_UNF_AVPLAY_ATTR_ID_VID_PID == enAttrID)
    {
        if (HI_INVALID_HANDLE == pAvplay->hVdec)
        {
            HI_ERR_AVPLAY("vid chn is close, can not get vid pid.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        *pPid = pAvplay->DmxVidPid;
    }
    else
    {
        *pPid = pAvplay->DmxPcrPid;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_SetSyncAttr(AVPLAY_S *pAvplay, HI_UNF_SYNC_ATTR_S *pSyncAttr)
{
    HI_S32 Ret;

    Ret = HI_MPI_SYNC_SetAttr(pAvplay->hSync, pSyncAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_SYNC_SetAttr failed 0x%x\n", Ret);
    }

    return Ret;
}

HI_S32 AVPLAY_GetSyncAttr(AVPLAY_S *pAvplay, HI_UNF_SYNC_ATTR_S *pSyncAttr)
{
    HI_S32 Ret;

    Ret = HI_MPI_SYNC_GetAttr(pAvplay->hSync, pSyncAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_SYNC_GetAttr failed 0x%x\n", Ret);
    }

    return Ret;
}

HI_S32 AVPLAY_SetOverflowProc(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_OVERFLOW_E *pOverflowProc)
{
    if (*pOverflowProc >= HI_UNF_AVPLAY_OVERFLOW_BUTT)
    {
        HI_ERR_AVPLAY("para OverflowProc is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    pAvplay->OverflowProc = *pOverflowProc;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_GetOverflowProc(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_OVERFLOW_E *pOverflowProc)
{
    *pOverflowProc = pAvplay->OverflowProc;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_SetLowDelay(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_LOW_DELAY_ATTR_S *pstAttr)
{
    HI_S32                  Ret;
    HI_U32                  i;
    HI_UNF_SYNC_ATTR_S      SyncAttr = {0};
    HI_CODEC_VIDEO_CMD_S    VdecCmd;
    HI_BOOL                 bProgressive;
    HI_U32                  VirChnNum;
    HI_U32                  SlaveChnNum;

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_AVPLAY("vid chan is closed!\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (pAvplay->VidEnable)
    {
        HI_ERR_AVPLAY("vid chan is running!\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    VirChnNum = AVPLAY_GetVirtualWinChnNum(pAvplay);
    SlaveChnNum = AVPLAY_GetSlaveWinChnNum(pAvplay);

    if ((HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
        && (0 == SlaveChnNum) && (0 == VirChnNum) )
    {
        HI_ERR_AVPLAY("there is now window attached, can not set low delay!\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    /* set window to quickoutput mode */
    if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
    {
        Ret = HI_MPI_WIN_SetQuickOutput(pAvplay->MasterFrmChn.hWindow, pstAttr->bEnable);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_WIN_SetQuickOutput ERR, Ret=%#x\n", Ret);
            return Ret;
        }
    }

    for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
    {
        if (HI_INVALID_HANDLE != pAvplay->SlaveFrmChn[i].hWindow)
        {
            Ret = HI_MPI_WIN_SetQuickOutput(pAvplay->SlaveFrmChn[i].hWindow, pstAttr->bEnable);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("HI_MPI_WIN_SetQuickOutput ERR, Ret=%#x\n", Ret);
            }
        }
    }

    /* set vdec to lowdelay mode */
    Ret = HI_MPI_VDEC_SetLowDelay(pAvplay->hVdec, pstAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_SetLowDelay ERR, Ret=%#x\n", Ret);
        return Ret;
    }

    memset(&SyncAttr, 0, sizeof(HI_UNF_SYNC_ATTR_S));
    if (pstAttr->bEnable)
    {
        /* set sync to none */
        Ret = HI_MPI_SYNC_GetAttr(pAvplay->hSync, &SyncAttr);
        SyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
        Ret |= HI_MPI_SYNC_SetAttr(pAvplay->hSync, &SyncAttr);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_SYNC_SetAttr ERR, Ret=%#x\n", Ret);
            return Ret;
        }

        bProgressive = HI_TRUE;
        VdecCmd.u32CmdID = HI_UNF_AVPLAY_SET_PROGRESSIVE_CMD;
        VdecCmd.pPara = (HI_VOID *)&bProgressive;
        Ret = HI_MPI_VDEC_Invoke(pAvplay->hVdec, &VdecCmd);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_Invoke ERR, Ret=%#x\n", Ret);
            return Ret;
        }
    }
    else
    {
        /* set sync to audio */
        Ret = HI_MPI_SYNC_GetAttr(pAvplay->hSync, &SyncAttr);
        SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
        Ret |= HI_MPI_SYNC_SetAttr(pAvplay->hSync, &SyncAttr);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_SYNC_SetAttr ERR, Ret=%#x\n", Ret);
            return Ret;
        }

        bProgressive = HI_FALSE;
        VdecCmd.u32CmdID = HI_UNF_AVPLAY_SET_PROGRESSIVE_CMD;
        VdecCmd.pPara = (HI_VOID *)&bProgressive;
        Ret = HI_MPI_VDEC_Invoke(pAvplay->hVdec, &VdecCmd);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_Invoke ERR, Ret=%#x\n", Ret);
            return Ret;
        }
    }

    pAvplay->LowDelayAttr = *pstAttr;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_GetLowDelay(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_LOW_DELAY_ATTR_S *pstAttr)
{
    *pstAttr = pAvplay->LowDelayAttr;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_SetAdAttr(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_AD_ATTR_S *pstAttr)
{
    HI_S32                  Ret;
    HI_UNF_DMX_CHAN_ATTR_S  DmxChnAttr;

    if (HI_INVALID_HANDLE == pAvplay->hAdec)
    {
        HI_ERR_AVPLAY("aud chan is closed!\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (!pAvplay->AudEnable)
    {
        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            if ((pstAttr->u32AdPid != 0x1fff)
              &&(pstAttr->u32AdPid != pAvplay->AdAttr.u32AdPid))
            {
                if (HI_INVALID_HANDLE == pAvplay->hDmxAdAud)
                {
                    DmxChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_AUD;
                    DmxChnAttr.enCRCMode     = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
                    DmxChnAttr.enOutputMode  = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
                    DmxChnAttr.enSecureMode  = HI_UNF_DMX_SECURE_MODE_NONE;
                    DmxChnAttr.u32BufSize    = pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize / 3;
                    Ret = HI_MPI_DMX_CreateChannel(pAvplay->AvplayAttr.u32DemuxId, &DmxChnAttr,  &(pAvplay->hDmxAdAud));
                    if (HI_SUCCESS != Ret)
                    {
                        HI_ERR_AVPLAY("HI_MPI_DMX_CreateChannel Ret = 0x%x!\n", Ret);
                        return Ret;
                    }
                }

                if (HI_INVALID_HANDLE != pAvplay->hDmxAdAud)
                {
                    Ret = HI_MPI_DMX_SetChannelPID(pAvplay->hDmxAdAud, pstAttr->u32AdPid);
                    if (HI_SUCCESS != Ret)
                    {
                        (HI_VOID)HI_MPI_DMX_DestroyChannel(pAvplay->hDmxAdAud);
                        pAvplay->hDmxAdAud = HI_INVALID_HANDLE;
                        HI_ERR_AVPLAY("HI_MPI_DMX_SetChannelPID Ret = 0x%x!\n", Ret);
                        return Ret;
                    }
                }
            }
        }
    }
    else
    {
        if ((pAvplay->AdAttr.bAdEnble != pstAttr->bAdEnble)
          ||(pAvplay->AdAttr.bAdToSomePort != pstAttr->bAdToSomePort))
        {
            Ret = HI_MPI_ADEC_SetADMixAttr(pAvplay->hAdec, pstAttr->bAdEnble, pstAttr->bAdToSomePort);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("HI_MPI_ADEC_SetADMixAttr Ret = 0x%x!\n", Ret);
                return Ret;
            }
        }

        if (pAvplay->AdAttr.s16AdBalance != pstAttr->s16AdBalance)
        {
            Ret = HI_MPI_ADEC_SetADBalance(pAvplay->hAdec, pstAttr->s16AdBalance);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("HI_MPI_ADEC_SetADBalance Ret = 0x%x!\n", Ret);
                return Ret;
            }
        }

        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            if ((pstAttr->u32AdPid != 0x1fff)
              &&(pstAttr->u32AdPid != pAvplay->AdAttr.u32AdPid))
            {
                HI_ERR_AVPLAY("AD Dmx is open, can't set pid\n");
            }
        }
    }

    pAvplay->AdAttr = *pstAttr;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_GetAdAttr(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_AD_ATTR_S *pstAttr)
{
    *pstAttr = pAvplay->AdAttr;
    return HI_SUCCESS;
}


HI_S32 AVPLAY_RelSpecialFrame(AVPLAY_S *pAvplay, HI_HANDLE hWin)
{
    HI_U32                              i;
    HI_HANDLE                           hWindow = HI_INVALID_HANDLE;
    HI_S32                              Ret;

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);
        if (hWindow == hWin)
        {
            Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_RelAllVirChnFrame(AVPLAY_S *pAvplay)
{
    HI_S32      Ret;
    HI_U32      i, j;
    HI_HANDLE   hWindow = HI_INVALID_HANDLE;

    for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

        for (j = 0; j < AVPLAY_MAX_VIR_FRMCHAN; j++)
        {
            if ((hWindow == pAvplay->VirFrmChn[j].hWindow) && (HI_FALSE == pAvplay->bSendedFrmToVirtualWin[j]))
            {
                Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport,
                    &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
                }
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_RelAllChnFrame(AVPLAY_S *pAvplay)
{
    HI_S32      Ret;
    HI_U32      i, j;
    HI_HANDLE   hWindow = HI_INVALID_HANDLE;

    if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
    {
        AVPLAY_RelAllVirChnFrame(pAvplay);
    }
    else
    {
        for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
        {
            (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

             /* may be CurFrmPack has not master frame */
             if ((hWindow == pAvplay->MasterFrmChn.hWindow) && (HI_FALSE == pAvplay->bSendedFrmToMasterWin))
             {
                 Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport,
                    &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                 if (HI_SUCCESS != Ret)
                 {
                     HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
                 }
                 break;
             }
        }

        for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
        {
            (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, pAvplay->CurFrmPack.stFrame[i].hport, &hWindow);

            for (j = 0; j < AVPLAY_MAX_SLAVE_FRMCHAN; j++)
            {
                if ((hWindow == pAvplay->SlaveFrmChn[j].hWindow) && (HI_FALSE == pAvplay->bSendedFrmToSlaveWin[j]))
                {
                    Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->CurFrmPack.stFrame[i].hport,
                        &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                    if (HI_SUCCESS != Ret)
                    {
                        HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
                    }
                }
            }
        }

        memset(&pAvplay->LstFrmPack, 0, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));
    }

    return HI_SUCCESS;
}

HI_VOID AVPLAY_ProcSignal(HI_S32 SignalType)
{
    signal(SIGINT, SIG_IGN);

    HI_U32    Id;
    AVPLAY_S *pAvplay;

    for (Id = 0; Id < AVPLAY_MAX_NUM; Id++)
    {
        AVPLAY_INST_LOCK(Id);

        if (HI_NULL != g_Avplay[Id].Avplay)
        {
            pAvplay = g_Avplay[Id].Avplay;

            AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);
        #ifdef HI_HDR_SUPPORT
            if (HI_TRUE == pAvplay->bHDRPlay)
            {
                (HI_VOID)AVPLAY_RelHDRChnFrame(pAvplay);
            }
            else
        #endif
            {
                if (HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
                {
                    (HI_VOID)AVPLAY_RelAllChnFrame(pAvplay);
                    pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
                }
            }

            pAvplay->VidEnable = HI_FALSE;

            AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
        }

        AVPLAY_INST_UNLOCK(Id);
    }

    signal(SIGINT, SIG_DFL);

    exit(0);
}

HI_S32 AVPLAY_ResetWindow(const AVPLAY_S *pAvplay, HI_DRV_WIN_SWITCH_E SwitchType)
{
    HI_U32              i;
    HI_DRV_WIN_INFO_S   WinInfo = {0};

    memset(&WinInfo, 0, sizeof(HI_DRV_WIN_INFO_S));

    if (HI_INVALID_HANDLE != pAvplay->hSharedOrgWin)
    {
        (HI_VOID)HI_MPI_WIN_GetInfo(pAvplay->hSharedOrgWin, &WinInfo);

        (HI_VOID)HI_MPI_WIN_Reset(pAvplay->hSharedOrgWin, SwitchType);
    }
    else
    {
        WinInfo.hPrim = HI_INVALID_HANDLE;
        WinInfo.hSec = HI_INVALID_HANDLE;
    }

    if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
    {
        if (pAvplay->MasterFrmChn.hWindow != WinInfo.hPrim)
        {
            (HI_VOID)HI_MPI_WIN_Reset(pAvplay->MasterFrmChn.hWindow, SwitchType);
        }
    }

    for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
    {
        if (HI_INVALID_HANDLE != pAvplay->SlaveFrmChn[i].hWindow)
        {
            if (pAvplay->SlaveFrmChn[i].hWindow != WinInfo.hSec)
            {
                (HI_VOID)HI_MPI_WIN_Reset(pAvplay->SlaveFrmChn[i].hWindow, SwitchType);
            }
        }
    }

    for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
    {
        if (HI_INVALID_HANDLE != pAvplay->VirFrmChn[i].hWindow)
        {
            (HI_VOID)HI_MPI_WIN_Reset(pAvplay->VirFrmChn[i].hWindow, SwitchType);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 AVPLAY_StartVidChn(AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

#ifdef HI_HDR_SUPPORT
    if (pAvplay->bHDRPlay)
    {
        Ret = AVPLAY_StartHDRChn(pAvplay);
        if (HI_SUCCESS != Ret)
        {
            return Ret;
        }
    }
    else
#endif
    {
        Ret = HI_MPI_SYNC_Start(pAvplay->hSync, SYNC_CHAN_VID);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_SYNC_Start failed 0x%x\n", Ret);
            return Ret;
        }

        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            Ret = HI_MPI_DMX_OpenChannel(pAvplay->hDmxVid);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_OpenChannel failed 0x%x\n", Ret);
                (HI_VOID)HI_MPI_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_VID);
                return Ret;
            }
        }

        Ret = HI_MPI_VDEC_ChanStart(pAvplay->hVdec);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanStart failed 0x%x\n", Ret);

            if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
            {
                (HI_VOID)HI_MPI_DMX_CloseChannel(pAvplay->hDmxVid);
            }

            (HI_VOID)HI_MPI_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_VID);

            return Ret;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 AVPLAY_StopVidChn(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_STOP_MODE_E enMode)
{
    HI_S32                  Ret;
    HI_DRV_WIN_SWITCH_E     SwitchType = HI_DRV_WIN_SWITCH_BUTT;

#ifdef HI_HDR_SUPPORT
    if (pAvplay->bHDRPlay)
    {
        /* may be only stop vidchannel,avoid there is frame at avplay, when stop avplay, we drop this frame*/
        (HI_VOID)AVPLAY_RelHDRChnFrame(pAvplay);

        Ret = AVPLAY_StopHDRChn(pAvplay);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }

        SwitchType = (HI_UNF_AVPLAY_STOP_MODE_STILL == enMode) ? HI_DRV_WIN_SWITCH_LAST : HI_DRV_WIN_SWITCH_BLACK;

        (HI_VOID)AVPLAY_ResetWindow(pAvplay, SwitchType);

        Ret = HI_MPI_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_VID);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_SYNC_Stop failed 0x%x\n", Ret);
            return Ret;
        }
    }
    else
#endif
    {
        /* may be only stop vidchannel,avoid there is frame at avplay, when stop avplay, we drop this frame*/
        if (HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
        {
            /*Release vpss frame*/
            (HI_VOID)AVPLAY_RelAllChnFrame(pAvplay);
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
        }

        Ret = HI_MPI_VDEC_ChanStop(pAvplay->hVdec);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanStop failed 0x%x\n", Ret);
            return Ret;
        }

        Ret = HI_MPI_VDEC_ResetChan(pAvplay->hVdec);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ResetChan failed 0x%x\n", Ret);
            return Ret;
        }

    #ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            Ret = HI_MPI_DMX_CloseChannel(pAvplay->hDmxVid);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_CloseChannel failed 0x%x\n", Ret);
                return Ret;
            }
        }
    #endif

        SwitchType = (HI_UNF_AVPLAY_STOP_MODE_STILL == enMode) ? HI_DRV_WIN_SWITCH_LAST : HI_DRV_WIN_SWITCH_BLACK;

        (HI_VOID)AVPLAY_ResetWindow(pAvplay, SwitchType);

        Ret = HI_MPI_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_VID);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_SYNC_Stop failed 0x%x\n", Ret);
            return Ret;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 AVPLAY_StartAudChn(AVPLAY_S *pAvplay)
{
    HI_S32         Ret;
    HI_U32         i, j;

    Ret = HI_MPI_SYNC_Start(pAvplay->hSync, SYNC_CHAN_AUD);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_SYNC_Start Aud failed 0x%x\n", Ret);
        return Ret;
    }

    Ret = HI_MPI_ADEC_Start(pAvplay->hAdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_ADEC_Start failed 0x%x\n", Ret);
        (HI_VOID)HI_MPI_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_AUD);
        return Ret;
    }

    /* get the string of adec type */
    (HI_VOID)HI_MPI_ADEC_GetInfo(pAvplay->hAdec, HI_MPI_ADEC_HaSzNameInfo, &(pAvplay->AdecNameInfo));

    for (i = 0; i < pAvplay->TrackNum; i++)
    {
        if (HI_INVALID_HANDLE != pAvplay->hTrack[i])
        {
            Ret |= HI_MPI_AO_Track_Start(pAvplay->hTrack[i]);
            if(HI_SUCCESS != Ret)
            {
                break;
            }
        }
    }

    if (i < pAvplay->TrackNum)
    {
        for (j = 0; j < i; j++)
        {
            (HI_VOID)HI_MPI_AO_Track_Stop(pAvplay->hTrack[j]);
        }

        HI_ERR_AVPLAY("HI_MPI_AO_Track_Start failed.\n");

        (HI_VOID)HI_MPI_ADEC_Stop(pAvplay->hAdec);

        (HI_VOID)HI_MPI_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_AUD);
        return Ret;
    }

    if (HI_TRUE == pAvplay->AdAttr.bAdEnble)
    {
        //AD enable
        Ret = HI_MPI_ADEC_SetADMixAttr(pAvplay->hAdec, pAvplay->AdAttr.bAdEnble, pAvplay->AdAttr.bAdToSomePort);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_ADEC_SetADMixAttr failed 0x%x!\n", Ret);
            return Ret;
        }

        //AD balance
        Ret = HI_MPI_ADEC_SetADBalance(pAvplay->hAdec, pAvplay->AdAttr.s16AdBalance);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_ADEC_SetADBalance failed 0x%x!\n", Ret);
            return Ret;
        }
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        for (i = 0; i < pAvplay->DmxAudChnNum; i++)
        {
            Ret = HI_MPI_DMX_OpenChannel(pAvplay->hDmxAud[i]);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_OpenChannel failed 0x%x\n", Ret);
                break;
            }
        }

        if (i < pAvplay->DmxAudChnNum)
        {
            for (j = 0; j < i; j++)
            {
                (HI_VOID)HI_MPI_DMX_DestroyChannel(pAvplay->hDmxAud[j]);
            }

            /*Destroy AD Audio Demux too*/
            if (HI_INVALID_HANDLE != pAvplay->hDmxAdAud)
            {
                (HI_VOID)HI_MPI_DMX_DestroyChannel(pAvplay->hDmxAdAud);
                pAvplay->hDmxAdAud = HI_INVALID_HANDLE;
            }

            for (i = 0; i < pAvplay->TrackNum; i++)
            {
                if (HI_INVALID_HANDLE != pAvplay->hTrack[i])
                {
                    (HI_VOID)HI_MPI_AO_Track_Stop(pAvplay->hTrack[i]);
                }
            }

            (HI_VOID)HI_MPI_ADEC_Stop(pAvplay->hAdec);

            (HI_VOID)HI_MPI_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_AUD);
            return Ret;
        }

        /*Open AD Audio Demux*/
        if (HI_INVALID_HANDLE != pAvplay->hDmxAdAud)
        {
            Ret = HI_MPI_DMX_OpenChannel(pAvplay->hDmxAdAud);
            if(HI_SUCCESS != Ret)
            {
                (HI_VOID)HI_MPI_DMX_DestroyChannel(pAvplay->hDmxAdAud);
                pAvplay->hDmxAdAud = HI_INVALID_HANDLE;
                return Ret;
            }
        }
    }

    return HI_SUCCESS;
}

static HI_S32 AVPLAY_StopAudChn(AVPLAY_S *pAvplay)
{
    HI_S32         Ret;
    HI_U32         i;

    /* To avoid invalid data send to audio track*/
    if (pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO])
    {
        (HI_VOID)HI_MPI_ADEC_ReleaseFrame(pAvplay->hAdec, &pAvplay->AvplayAudFrm);
        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;
    }

    Ret = HI_MPI_ADEC_Stop(pAvplay->hAdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_ADEC_Stop failed 0x%x\n", Ret);
        return Ret;
    }

#ifndef HI_AVPLAY_PLAY_AND_REC_SUPPORT
    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        /* if the cur dmx es buf has not been released */
        if(pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC])
        {
            (HI_VOID)HI_MPI_DMX_ReleaseEs(pAvplay->hDmxAud[pAvplay->CurDmxAudChn], &pAvplay->AvplayDmxEsBuf);
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC] = HI_FALSE;
        }

        for (i = 0; i < pAvplay->DmxAudChnNum; i++)
        {
            Ret = HI_MPI_DMX_CloseChannel(pAvplay->hDmxAud[i]);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_CloseChannel failed 0x%x\n", Ret);
                return Ret;
            }
        }

        /*Close AD Audio Demux*/
        if (HI_INVALID_HANDLE != pAvplay->hDmxAdAud)
        {
            (HI_VOID)HI_MPI_DMX_CloseChannel(pAvplay->hDmxAdAud);
        }
    }
#endif

    for (i = 0; i < pAvplay->TrackNum; i++)
    {
        if (HI_INVALID_HANDLE != pAvplay->hTrack[i])
        {
            Ret = HI_MPI_AO_Track_Stop(pAvplay->hTrack[i]);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_AO_Track_Stop failed 0x%x\n", Ret);
                return Ret;
            }

           //(HI_VOID)HI_MPI_AO_Track_Flush(pAvplay->hTrack[i]);
        }
    }

    Ret = HI_MPI_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_AUD);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_SYNC_Stop failed 0x%x\n", Ret);
        return Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 AVPLAY_StopVideo(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_STOP_OPT_S *pStop)
{
    HI_S32  Ret;

    if (pAvplay->VidEnable)
    {
        Ret = AVPLAY_StopVidChn(pAvplay, pStop->enMode);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("stop vid chn failed.\n");
            return Ret;
        }

    #ifdef  HI_AVPLAY_PLAY_AND_REC_SUPPORT
        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            Ret = AVPLAY_FreeVidBuffer(pAvplay, pAvplay->hVdec);
            if (Ret != HI_SUCCESS)
            {
                return Ret;
            }

            Ret = HI_MPI_DMX_CloseChannel(pAvplay->hDmxVid);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_CloseChannel failed 0x%x\n",Ret);
                return Ret;
            }


            Ret = AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("Avplay free dmx vid chn failed.Ret = 0x%x\n",Ret);
                return Ret;
            }
        }
    #endif
        if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
        {
            /* resume the frc and window ratio */
            pAvplay->bFrcEnable = HI_TRUE;
            pAvplay->FrcParamCfg.u32PlayRate = AVPLAY_ALG_FRC_BASE_PLAY_RATIO;
        }

        pAvplay->VidEnable = HI_FALSE;
        pAvplay->bVidPreEnable = HI_FALSE;

        (HI_VOID)HI_MPI_STAT_Event(STAT_EVENT_VSTOP, 0);
    }
    else if (pAvplay->bVidPreEnable)
    {
    #ifdef HI_AVPLAY_PLAY_AND_REC_SUPPORT
        Ret = AVPLAY_FreeVidBuffer(pAvplay, pAvplay->hVdec);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
    #endif

        Ret = HI_MPI_DMX_CloseChannel(pAvplay->hDmxVid);
        if(HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_DMX_CloseChannel failed:%#x.\n",Ret);
            return Ret;
        }

    #ifdef HI_AVPLAY_PLAY_AND_REC_SUPPORT
        Ret = AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("Avplay free dmx vid chn failed.\n");
            return Ret;
        }
    #endif

        if (HI_INVALID_HANDLE != pAvplay->hDFCtx)
        {
            Ret = DetFrm_Stop(pAvplay->hDFCtx);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("Avplay stop DetFrm failed 0x%x\n", Ret);
                return Ret;
            }

            Ret = DetFrm_Destroy(pAvplay->hDFCtx);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("Avplay destroy DetFrm failed 0x%x\n", Ret);
                return Ret;
            }

            pAvplay->hDFCtx = HI_INVALID_HANDLE;
        }

        pAvplay->bVidPreEnable = HI_FALSE;
    }

    return HI_SUCCESS;
}

static HI_S32 AVPLAY_StopAudio(AVPLAY_S *pAvplay)
{
    HI_S32  Ret;
    HI_U32  i;

    if (pAvplay->AudEnable)
    {
        Ret = AVPLAY_StopAudChn(pAvplay);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("stop aud chn failed.\n");
            return Ret;
        }

    #ifdef  HI_AVPLAY_PLAY_AND_REC_SUPPORT
        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            Ret = HI_MPI_DMX_CloseChannel(pAvplay->hDmxAud[pAvplay->CurDmxAudChn]);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_CloseChannel failed 0x%x\n", Ret);
                return Ret;
            }

            Ret = AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("Avplay free dmx Aud chn failed.Ret=0x%x\n", Ret);
                return Ret;
            }
        }
    #endif
        pAvplay->AudEnable = HI_FALSE;
        pAvplay->bAudPreEnable = HI_FALSE;

        /* may be only stop audchannel,avoid there is frame at avplay, when stop avplay, we drop this frame*/
        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;

        (HI_VOID)HI_MPI_STAT_Event(STAT_EVENT_ASTOP, 0);
    }
    else if (pAvplay->bAudPreEnable)
    {
        for (i = 0; i < pAvplay->DmxAudChnNum; i++)
        {
            Ret = HI_MPI_DMX_CloseChannel(pAvplay->hDmxAud[i]);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_CloseChannel failed 0x%x\n", Ret);
                return Ret;
            }
        }

    #ifdef HI_AVPLAY_PLAY_AND_REC_SUPPORT
        Ret = AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("Avplay free dmx Aud chn failed.\n");
            return Ret;
        }
    #endif
        pAvplay->bAudPreEnable = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_PcrEnable(AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS != pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        return HI_SUCCESS;
    }

    if ((HI_INVALID_HANDLE == pAvplay->hDmxPcr) && (DEMUX_INVALID_PID != pAvplay->DmxPcrPid))
    {
        Ret = HI_MPI_DMX_CreatePcrChannel(pAvplay->AvplayAttr.u32DemuxId, &pAvplay->hDmxPcr);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("Avplay malloc pcr chn failed.\n");
            return Ret;
        }

        (HI_VOID)HI_MPI_DMX_PcrSyncAttach(pAvplay->hDmxPcr, pAvplay->hSync);

        Ret = HI_MPI_DMX_PcrPidSet(pAvplay->hDmxPcr, pAvplay->DmxPcrPid);
        if (Ret != HI_SUCCESS)
        {
            (HI_VOID)HI_MPI_DMX_DestroyPcrChannel(pAvplay->hDmxPcr);
            HI_ERR_AVPLAY("ERR: HI_MPI_DMX_PcrPidSet failed 0x%x\n", Ret);
            return Ret;
        }
    }

    return HI_SUCCESS;

}

HI_S32 AVPLAY_PcrDisable(AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS != pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        return HI_SUCCESS;
    }

    if (   (!pAvplay->VidEnable) && (!pAvplay->bVidPreEnable)
        && (!pAvplay->AudEnable) && (!pAvplay->bAudPreEnable) )
    {
        if (pAvplay->hDmxPcr != HI_INVALID_HANDLE)
        {
            Ret = HI_MPI_DMX_PcrPidSet(pAvplay->hDmxPcr, DEMUX_INVALID_PID);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_PcrPidSet failed 0x%x\n", Ret);
                return Ret;
            }

            (HI_VOID)HI_MPI_DMX_PcrSyncDetach(pAvplay->hDmxPcr);
            Ret = HI_MPI_DMX_DestroyPcrChannel(pAvplay->hDmxPcr);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("Avplay free pcr chn failed 0x%x\n", Ret);
                return Ret;
            }

            pAvplay->hDmxPcr = HI_INVALID_HANDLE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_StartVideo(AVPLAY_S *pAvplay)
{
    HI_S32   Ret;
    HI_U32   VirChnNum;
    HI_U32   SlaveChnNum;

    if (pAvplay->VidEnable)
    {
        return HI_SUCCESS;
    }

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_AVPLAY("vid chn is close, can not start.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    VirChnNum = AVPLAY_GetVirtualWinChnNum(pAvplay);
    SlaveChnNum = AVPLAY_GetSlaveWinChnNum(pAvplay);

    if (   (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
        && (0 == SlaveChnNum) && (0 == VirChnNum) )
    {
        HI_ERR_AVPLAY("window is not attached, can not start.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        if(!pAvplay->bVidPreEnable)
        {
        #ifdef HI_AVPLAY_PLAY_AND_REC_SUPPORT
            Ret = AVPLAY_MallocDmxChnWithPID(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID, pAvplay->DmxVidPid);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("Avplay malloc vid dmx chn failed.\n");
                return Ret;
            }

            Ret = AVPLAY_MallocVidBuffer(pAvplay, pAvplay->hVdec);
            if (Ret != HI_SUCCESS)
            {
                (HI_VOID)AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID);
                return Ret;
            }
        #endif
        }
        else
        {
            if (HI_INVALID_HANDLE != pAvplay->hDFCtx)
            {
                Ret = DetFrm_Stop(pAvplay->hDFCtx);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("DetFrm_Stop failed 0x%x\n", Ret);
                    return Ret;
                }

                Ret = DetFrm_Destroy(pAvplay->hDFCtx);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("DetFrm_Destroy failed 0x%x\n", Ret);
                    return Ret;
                }

                pAvplay->hDFCtx = HI_INVALID_HANDLE;
            }
        }
    }

    Ret = AVPLAY_StartVidChn(pAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("start vid chn failed.\n");
        goto ERR_FREE_VID_CHN;
    }

    Ret = HI_MPI_SYNC_Play(pAvplay->hSync);
    if (Ret != HI_SUCCESS)
    {
        (HI_VOID)AVPLAY_StopVidChn(pAvplay, HI_UNF_AVPLAY_STOP_MODE_BLACK);
        HI_ERR_AVPLAY("ERR: HI_MPI_SYNC_Play Vid failed 0x%x\n", Ret);
        goto ERR_FREE_VID_CHN;
    }

    pAvplay->VidEnable = HI_TRUE;
    pAvplay->bVidPreEnable = HI_FALSE;

    return HI_SUCCESS;

ERR_FREE_VID_CHN:

#ifdef HI_AVPLAY_PLAY_AND_REC_SUPPORT
    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        (HI_VOID)AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID);
        (HI_VOID)AVPLAY_FreeVidBuffer(pAvplay, pAvplay->hVdec);
    }
#endif
    return HI_FAILURE;
}

HI_S32 AVPLAY_StartAudio(AVPLAY_S *pAvplay)
{
    HI_S32      Ret;

    if (pAvplay->AudEnable)
    {
        return HI_SUCCESS;
    }

    if (HI_INVALID_HANDLE == pAvplay->hAdec)
    {
        HI_ERR_AVPLAY("aud chn is close, can not start.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (0 == pAvplay->TrackNum)
    {
        HI_ERR_AVPLAY("track is not attached, can not start.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

#ifdef HI_AVPLAY_PLAY_AND_REC_SUPPORT
    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        if(!pAvplay->bAudPreEnable)
        {
            Ret = AVPLAY_MallocDmxChnWithPID(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, pAvplay->DmxAudPid[pAvplay->CurDmxAudChn]);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("Avplay malloc aud dmx chn failed.\n");
                return Ret;
            }
        }
    }
#endif

    Ret = AVPLAY_StartAudChn(pAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("start aud chn failed.\n");
        goto ERR_FREE_AUD_CHN;
    }

    Ret = HI_MPI_SYNC_Play(pAvplay->hSync);
    if (Ret != HI_SUCCESS)
    {
        (HI_VOID)AVPLAY_StopAudChn(pAvplay);
        HI_ERR_AVPLAY("ERR: HI_MPI_SYNC_Play Aud failed 0x%x\n", Ret);
        goto ERR_FREE_AUD_CHN;
    }

    pAvplay->AudEnable = HI_TRUE;
    pAvplay->bAudPreEnable = HI_FALSE;

    return HI_SUCCESS;

ERR_FREE_AUD_CHN:

#ifdef HI_AVPLAY_PLAY_AND_REC_SUPPORT
    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        (HI_VOID)AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD);
    }
#endif

    return HI_FAILURE;
}

HI_VOID AVPLAY_PrePlay(AVPLAY_S *pAvplay)
{
    if (pAvplay->CurStatus != HI_UNF_AVPLAY_STATUS_PREPLAY)
    {
        pAvplay->LstStatus = pAvplay->CurStatus;
        pAvplay->CurStatus = HI_UNF_AVPLAY_STATUS_PREPLAY;
    }

    return;
}

HI_VOID AVPLAY_Play(AVPLAY_S *pAvplay)
{
    if (pAvplay->CurStatus != HI_UNF_AVPLAY_STATUS_PLAY)
    {
        pAvplay->LstStatus = pAvplay->CurStatus;
        pAvplay->CurStatus = HI_UNF_AVPLAY_STATUS_PLAY;
    }

    return;
}

HI_VOID AVPLAY_Stop(AVPLAY_S *pAvplay)
{
    if (pAvplay->CurStatus != HI_UNF_AVPLAY_STATUS_STOP)
    {
        pAvplay->LstStatus = pAvplay->CurStatus;
        pAvplay->CurStatus = HI_UNF_AVPLAY_STATUS_STOP;
    }

    AVPLAY_ResetProcFlag(pAvplay);
    return;
}

HI_VOID AVPLAY_Pause(AVPLAY_S *pAvplay)
{
    pAvplay->LstStatus = pAvplay->CurStatus;
    pAvplay->CurStatus = HI_UNF_AVPLAY_STATUS_PAUSE;

    return;
}

HI_VOID AVPLAY_Tplay(AVPLAY_S *pAvplay)
{
    pAvplay->LstStatus = pAvplay->CurStatus;
    pAvplay->CurStatus = HI_UNF_AVPLAY_STATUS_TPLAY;

    return;
}

HI_S32 AVPLAY_ResetAudChn(AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

    if (pAvplay->AudEnable)
    {
        Ret = AVPLAY_StopAudChn(pAvplay);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("stop aud chn failed.\n");
            return Ret;
        }
    }

    if (pAvplay->AudEnable)
    {
        Ret = AVPLAY_StartAudChn(pAvplay);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("start aud chn failed.\n");
            return Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_Seek(AVPLAY_S *pAvplay, HI_U32 u32SeekPts)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 i = 0;
    HI_UNF_SYNC_STATUS_S SyncStatus;
    HI_U32 u32FindObjectPts = u32SeekPts;  //u32FindObjectPts  as  input param and output param

    HI_INFO_AVPLAY("seekpts is %d\n", u32SeekPts);

    if (pAvplay->AudEnable)
    {
        Ret = HI_MPI_SYNC_GetStatus(pAvplay->hSync, &SyncStatus);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_SYNC_GetStatus failed 0x%x\n", Ret);
            return HI_FAILURE;
        }
        else
        {
            if (u32SeekPts < SyncStatus.u32LastAudPts)
            {
                HI_INFO_AVPLAY("find pts in ao buf ok quit\n");
                return HI_SUCCESS;
            }
        }
    }

    //1. pause vid, aud only 800ms, don't need pause
    Ret = HI_MPI_SYNC_Pause(pAvplay->hSync);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_SYNC_Pause failed 0x%x.\n", Ret);
        return HI_FAILURE;
    }

    //2. discard stream
    if (pAvplay->VidEnable)
    {
        Ret = HI_MPI_VDEC_ChanDropStream(pAvplay->hVdec, &u32FindObjectPts, AVPLAY_VDEC_SEEKPTS_THRESHOLD);
        if (Ret != HI_SUCCESS)
        {
            HI_INFO_AVPLAY("HI_MPI_VDEC_ChanDropStream NO FIND SEEKPTS.\n");
            HI_INFO_AVPLAY("return vid pts is %d\n", u32FindObjectPts);
            return HI_FAILURE;
        }
        else
        {
            HI_INFO_AVPLAY("call HI_MPI_VDEC_ChanDropStream FIND SEEKPTS OK.\n");
            HI_INFO_AVPLAY("return vid pts is %d\n", u32FindObjectPts);

            (HI_VOID)AVPLAY_ResetWindow(pAvplay, HI_DRV_WIN_SWITCH_LAST);

            HI_INFO_AVPLAY("reset window\n");

            if (HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
            {
                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
            }
        }
    }

    if (pAvplay->AudEnable)
    {
        u32SeekPts = (u32FindObjectPts > u32SeekPts) ? u32FindObjectPts : u32SeekPts;
        Ret = HI_MPI_ADEC_DropStream(pAvplay->hAdec, u32SeekPts);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("no find aud pts quit\n");
            return HI_FAILURE;
        }
        else
        {
            HI_INFO_AVPLAY("find aud pts ok\n");
            for (i=0; i<pAvplay->TrackNum; i++)
            {
                if (HI_INVALID_HANDLE != pAvplay->hTrack[i])
                {
                    (HI_VOID)HI_MPI_AO_Track_Flush(pAvplay->hTrack[i]);
                }
            }
            HI_INFO_AVPLAY("reset ao\n");

            HI_INFO_AVPLAY("set AVPLAY_PROC_ADEC_AO false\n");

            if (HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO])
            {
                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;
            }
        }
    }

    if ((pAvplay->AudEnable) && (pAvplay->VidEnable))
    {
        Ret  = HI_MPI_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_AUD);
        Ret |= HI_MPI_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_VID);
        Ret |= HI_MPI_SYNC_Seek(pAvplay->hSync, u32SeekPts);
        Ret |= HI_MPI_SYNC_Start(pAvplay->hSync, SYNC_CHAN_AUD);
        Ret |= HI_MPI_SYNC_Start(pAvplay->hSync, SYNC_CHAN_VID);
    }

    Ret |= HI_MPI_SYNC_Play(pAvplay->hSync);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("call HI_MPI_SYNC_Play failed, Ret=0x%x.\n", Ret);
    }

    return Ret;
}

HI_S32 AVPLAY_Reset(AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

    if (pAvplay->VidEnable)
    {
        Ret = AVPLAY_StopVidChn(pAvplay, HI_UNF_AVPLAY_STOP_MODE_STILL);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
    }

    if (pAvplay->AudEnable)
    {
        Ret = AVPLAY_StopAudChn(pAvplay);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        if (HI_INVALID_HANDLE != pAvplay->hDmxPcr)
        {
            Ret = HI_MPI_DMX_PcrPidSet(pAvplay->hDmxPcr, DEMUX_INVALID_PID);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_PcrPidSet failed 0x%x\n", Ret);
                return Ret;
            }
        }
    }

    if (pAvplay->VidEnable)
    {
        Ret = AVPLAY_StartVidChn(pAvplay);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
    }

    if (pAvplay->AudEnable)
    {
        Ret = AVPLAY_StartAudChn(pAvplay);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        if ((HI_INVALID_HANDLE != pAvplay->hDmxPcr) && (DEMUX_INVALID_PID != pAvplay->DmxPcrPid))
        {
            Ret = HI_MPI_DMX_PcrPidSet(pAvplay->hDmxPcr, pAvplay->DmxPcrPid);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_PcrPidSet failed 0x%x\n", Ret);
                return Ret;
            }
        }
    }

    if (HI_UNF_AVPLAY_STATUS_PLAY == pAvplay->CurStatus)
    {
        Ret = HI_MPI_SYNC_Play(pAvplay->hSync);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_SYNC_Play failed 0x%x\n", Ret);
        }
    }
    else if (HI_UNF_AVPLAY_STATUS_TPLAY == pAvplay->CurStatus)
    {
        Ret = HI_MPI_SYNC_Tplay(pAvplay->hSync);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_SYNC_Play failed 0x%x\n", Ret);
        }
    }
    else if (HI_UNF_AVPLAY_STATUS_EOS == pAvplay->CurStatus)
    {
        /*Current status change to Last status while call Reset after EOS, scene: free TS->scramble TS->free TS*/
        pAvplay->CurStatus = pAvplay->LstStatus;

        if (HI_UNF_AVPLAY_STATUS_TPLAY == pAvplay->LstStatus)
        {
            Ret = HI_MPI_SYNC_Tplay(pAvplay->hSync);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_SYNC_Play failed 0x%x\n", Ret);
            }
        }
        else
        {
            Ret = HI_MPI_SYNC_Play(pAvplay->hSync);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("HI_MPI_SYNC_Play failed 0x%x\n", Ret);
            }
        }
    }
    else
    {
        Ret = HI_MPI_SYNC_Pause(pAvplay->hSync);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_SYNC_Pause failed 0x%x\n", Ret);
        }
    }

    AVPLAY_ResetProcFlag(pAvplay);

    return HI_SUCCESS;
}

HI_S32 AVPLAY_SetMultiAud(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_MULTIAUD_ATTR_S *pAttr)
{
    HI_S32                      Ret;
    HI_UNF_DMX_CHAN_ATTR_S      DmxChnAttr;
    HI_U32                      i, j;

    if(HI_NULL == pAttr || HI_NULL == pAttr->pu32AudPid || HI_NULL == pAttr->pstAcodecAttr)
    {
        HI_ERR_AVPLAY("multi aud attr is null!\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if(pAttr->u32PidNum > AVPLAY_MAX_DMX_AUD_CHAN_NUM)
    {
        HI_ERR_AVPLAY("pidnum is too large\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (pAvplay->AudEnable)
    {
        HI_ERR_AVPLAY("aud chn is running, can not set aud pid.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_INVALID_HANDLE == pAvplay->hAdec)
    {
        HI_ERR_AVPLAY("aud chn is close, can not set aud pid.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    DmxChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_AUD;
    DmxChnAttr.enCRCMode     = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
    DmxChnAttr.enOutputMode  = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
    DmxChnAttr.enSecureMode  = pAvplay->DmxAudChnSecMode;
    DmxChnAttr.u32BufSize    = pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize / 3;

    /* destroy the old resource */
    for (i = 1; i < pAvplay->DmxAudChnNum; i++)
    {
        (HI_VOID)HI_MPI_DMX_DestroyChannel(pAvplay->hDmxAud[i]);
    }

    if (HI_NULL != pAvplay->pstAcodecAttr)
    {
        HI_FREE(HI_ID_AVPLAY, (HI_VOID*)(pAvplay->pstAcodecAttr));
        pAvplay->pstAcodecAttr = HI_NULL;
    }

    /* create new resource */
    for (i = 1; i < pAttr->u32PidNum; i++)
    {
        Ret = HI_MPI_DMX_CreateChannel(pAvplay->AvplayAttr.u32DemuxId, &DmxChnAttr, &(pAvplay->hDmxAud[i]));
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_DMX_CreateChannel failed 0x%x\n", Ret);
            break;
        }
    }

    if(i != pAttr->u32PidNum)
    {
        for(j = 1; j < i; j++)
        {
            (HI_VOID)HI_MPI_DMX_DestroyChannel(pAvplay->hDmxAud[j]);
        }

        return HI_FAILURE;
    }

    for(i = 0; i < pAttr->u32PidNum; i++)
    {
        Ret = HI_MPI_DMX_SetChannelPID(pAvplay->hDmxAud[i], *(pAttr->pu32AudPid + i));
        if(HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_DMX_SetChannelPID failed 0x%x\n", Ret);
            return Ret;
        }
        else
        {
            pAvplay->DmxAudPid[i] = *(pAttr->pu32AudPid + i);
        }
    }

    pAvplay->DmxAudChnNum = pAttr->u32PidNum;

    pAvplay->pstAcodecAttr = (HI_UNF_ACODEC_ATTR_S *)HI_MALLOC(HI_ID_AVPLAY, sizeof(HI_UNF_ACODEC_ATTR_S) * pAttr->u32PidNum);
    if (HI_NULL == pAvplay->pstAcodecAttr)
    {
        HI_ERR_AVPLAY("malloc pstAcodecAttr error.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    memcpy(pAvplay->pstAcodecAttr, pAttr->pstAcodecAttr, sizeof(HI_UNF_ACODEC_ATTR_S)*pAttr->u32PidNum);

    return HI_SUCCESS;
}

HI_S32 AVPLAY_GetMultiAud(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_MULTIAUD_ATTR_S *pAttr)
{
    if (!pAttr || !pAttr->pu32AudPid || !pAttr->pstAcodecAttr || (pAttr->u32PidNum <= 1))
    {
        HI_ERR_AVPLAY("invalid para\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    /* only get the real pid num */
    if (pAttr->u32PidNum > pAvplay->DmxAudChnNum)
    {
        pAttr->u32PidNum = pAvplay->DmxAudChnNum;
    }

    if (pAttr->u32PidNum <= AVPLAY_MAX_DMX_AUD_CHAN_NUM)
    {
        memcpy(pAttr->pu32AudPid, pAvplay->DmxAudPid, sizeof(HI_U32) * pAttr->u32PidNum);
    }

    if (HI_NULL != pAvplay->pstAcodecAttr)
    {
        memcpy(pAttr->pstAcodecAttr, pAvplay->pstAcodecAttr, sizeof(HI_UNF_ACODEC_ATTR_S) * pAttr->u32PidNum);
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_SetEosFlag(AVPLAY_S *pAvplay)
{
    HI_S32          Ret;
    HI_U32          i;

    if (!pAvplay->AudEnable && !pAvplay->VidEnable)
    {
        HI_ERR_AVPLAY("vid and aud both disable, can not set eos!\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (pAvplay->AudEnable)
    {
        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            for (i = 0; i < pAvplay->DmxAudChnNum; i++)
            {
                Ret = HI_MPI_DMX_SetChannelEosFlag(pAvplay->hDmxAud[i]);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("ERR: HI_MPI_DMX_SetChannelEosFlag, Chn:%d, Ret = %#x! \n", i, Ret);
                    return HI_ERR_AVPLAY_INVALID_OPT;
                }
            }
        }

        if (HI_UNF_AVPLAY_STREAM_TYPE_ES == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            Ret = HI_MPI_ADEC_SetEosFlag(pAvplay->hAdec);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("ERR: HI_MPI_ADEC_SetEosFlag, Ret = %#x! \n", Ret);
                return HI_ERR_AVPLAY_INVALID_OPT;
            }

            if (HI_INVALID_HANDLE != pAvplay->hSyncTrack)
            {
                Ret = HI_MPI_AO_Track_SetEosFlag(pAvplay->hSyncTrack, HI_TRUE);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("ERR: HI_MPI_HIAO_SetEosFlag, Ret = %#x! \n", Ret);
                    return HI_ERR_AVPLAY_INVALID_OPT;
                }
            }
        }
    }

    if (pAvplay->VidEnable)
    {
    #ifdef HI_HDR_SUPPORT
        if (pAvplay->bHDRPlay == HI_TRUE)
        {
            if (pAvplay->bDualChnPlay == HI_TRUE)
            {
                Ret = HI_MPI_VDEC_SetEosFlag(pAvplay->hVdec);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_SetEosFlag %#x! \n", Ret);
                    return HI_ERR_AVPLAY_INVALID_OPT;
                }

                Ret = HI_MPI_VDEC_SetEosFlag(pAvplay->hElVdec);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_SetEosFlag %#x! \n", Ret);
                    return HI_ERR_AVPLAY_INVALID_OPT;
                }
            }
            else
            {
                Ret = HI_MPI_VDEC_SetEosFlag(pAvplay->hVdec);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_SetEosFlag %#x! \n", Ret);
                    return HI_ERR_AVPLAY_INVALID_OPT;
                }
            }
        }
        else
    #endif
        {
            Ret = HI_MPI_VDEC_SetEosFlag(pAvplay->hVdec);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_SetEosFlag, Ret = %#x! \n", Ret);
                return HI_ERR_AVPLAY_INVALID_OPT;
            }
        }

        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            Ret = HI_MPI_DMX_SetChannelEosFlag(pAvplay->hDmxVid);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("ERR: HI_MPI_DMX_SetChannelEosFlag, Ret = %#x! \n", Ret);
                return HI_ERR_AVPLAY_INVALID_OPT;
            }
        }
    }

    pAvplay->bSetEosFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_SetPortAttr(AVPLAY_S *pAvplay, HI_HANDLE hPort, VDEC_PORT_TYPE_E enType)
{
    HI_S32                      Ret;

    Ret = HI_MPI_VDEC_SetPortType(pAvplay->hVdec, hPort, enType);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_SetPortType, Ret=%#x.\n", Ret);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_VDEC_EnablePort(pAvplay->hVdec, hPort);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_EnablePort, Ret=%#x.\n", Ret);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_CreatePort(AVPLAY_S *pAvplay, HI_HANDLE hWin, VDEC_PORT_ABILITY_E enAbility, HI_HANDLE *phPort)
{
    HI_S32                      Ret;
    HI_BOOL                     WinCurState;
    HI_DRV_WIN_SRC_HANDLE_S     WinSrcInfo = {0};

    Ret = AVPLAY_WindowUnload(hWin, &WinCurState);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    memset(&WinSrcInfo, 0x0, sizeof(HI_DRV_WIN_SRC_HANDLE_S));

    Ret = HI_MPI_VDEC_CreatePort(pAvplay->hVdec, phPort, enAbility);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_CreatePort.\n");

        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    WinSrcInfo.enSrcMode  = HI_ID_VDEC;
    WinSrcInfo.hSrc       = *phPort;
    WinSrcInfo.hSecondSrc = HI_INVALID_HANDLE;
    Ret = HI_MPI_WIN_SetSource(hWin, &WinSrcInfo);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_WIN_SetSource.\n");
        (HI_VOID)HI_MPI_VDEC_DestroyPort(pAvplay->hVdec, *phPort);
        *phPort = HI_INVALID_HANDLE;
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = AVPLAY_WindowLoad(hWin, WinCurState);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_DestroyPort(AVPLAY_S *pAvplay, HI_HANDLE hWin, HI_HANDLE hPort)
{
    HI_S32                      Ret;
    HI_BOOL                     WinCurState;
    HI_DRV_WIN_SRC_HANDLE_S     WinSrcInfo = {0};

    Ret = AVPLAY_WindowUnload(hWin, &WinCurState);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    memset(&WinSrcInfo, 0x0, sizeof(HI_DRV_WIN_SRC_HANDLE_S));

    WinSrcInfo.enSrcMode     = HI_ID_BUTT;
    WinSrcInfo.hSrc          = HI_INVALID_HANDLE;
    WinSrcInfo.hSecondSrc    = HI_INVALID_HANDLE;
    Ret = HI_MPI_WIN_SetSource(hWin, &WinSrcInfo);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_WIN_SetSource 0x%x\n", Ret);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_VDEC_DestroyPort(pAvplay->hVdec, hPort);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_DestroyPort 0x%x\n", Ret);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = AVPLAY_WindowLoad(hWin, WinCurState);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return Ret;
}

HI_S32 AVPLAY_LookUpMasterPort(AVPLAY_S *pAvplay)
{
    HI_S32            Ret;
    HI_U32            i;
    HI_DRV_WIN_INFO_S WinInfo;

    //look up another master window
    for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
    {
        if (HI_INVALID_HANDLE == pAvplay->SlaveFrmChn[i].hWindow)
        {
            continue;
        }

        Ret = HI_MPI_WIN_GetInfo(pAvplay->SlaveFrmChn[i].hWindow, &WinInfo);
        if ((HI_SUCCESS == Ret) && (HI_DRV_WIN_ACTIVE_SINGLE == WinInfo.eType))
        {
            pAvplay->MasterFrmChn.hPort   = pAvplay->SlaveFrmChn[i].hPort;
            pAvplay->MasterFrmChn.hWindow = pAvplay->SlaveFrmChn[i].hWindow;
            Ret = AVPLAY_SetPortAttr(pAvplay, pAvplay->MasterFrmChn.hPort, VDEC_PORT_TYPE_MASTER);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("ERR: set main port failed.\n");
                return Ret;
            }

            pAvplay->SlaveFrmChn[i].hWindow = HI_INVALID_HANDLE;
            pAvplay->SlaveFrmChn[i].hPort   = HI_INVALID_HANDLE;

            break;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_AddMasterPort(AVPLAY_S *pAvplay, HI_HANDLE hWin)
{
    HI_S32  Ret;

    if (pAvplay->MasterFrmChn.hWindow == hWin)
    {
        HI_ERR_AVPLAY("this window is alreay attached!\n");
        return HI_SUCCESS;
    }

    /* if attach homologous window, homologous window must be master window*/
    if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
    {
        HI_ERR_AVPLAY("avplay can only attach one master handle.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

#ifdef HI_HDR_SUPPORT
    if (pAvplay->bHDRPlay)
    {
        Ret = AVPLAY_CreateHDRPort(pAvplay, VDEC_PORT_HD, hWin);
        if (HI_SUCCESS != Ret)
        {
            return Ret;
        }

        pAvplay->MasterFrmChn.hWindow = hWin;

        return HI_SUCCESS;
    }
    else
#endif
    {
        Ret = AVPLAY_CreatePort(pAvplay, hWin, VDEC_PORT_HD, &(pAvplay->MasterFrmChn.hPort));
        if(HI_SUCCESS != Ret)
        {
            return Ret;
        }

        Ret = AVPLAY_SetPortAttr(pAvplay,pAvplay->MasterFrmChn.hPort, VDEC_PORT_TYPE_MASTER);
        if(HI_SUCCESS != Ret)
        {
            (HI_VOID)AVPLAY_DestroyPort(pAvplay, hWin, pAvplay->MasterFrmChn.hPort);
            pAvplay->MasterFrmChn.hPort = HI_INVALID_HANDLE;

            return Ret;
        }
    }

    pAvplay->MasterFrmChn.hWindow = hWin;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_DelMasterPort(AVPLAY_S *pAvplay, HI_HANDLE hWin)
{
    HI_S32            Ret;

    if (pAvplay->MasterFrmChn.hWindow != hWin)
    {
        HI_ERR_AVPLAY("ERR: this is not a attached window.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

#ifdef HI_HDR_SUPPORT
    if (pAvplay->bHDRPlay)
    {
        Ret = AVPLAY_DestroyHDRPort(pAvplay, pAvplay->MasterFrmChn.hWindow);
        if (HI_SUCCESS != Ret)
        {
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        pAvplay->MasterFrmChn.hWindow = HI_INVALID_HANDLE;
        pAvplay->MasterFrmChn.hPort = HI_INVALID_HANDLE;

        return HI_SUCCESS;
    }
    else
#endif
    {
        Ret = AVPLAY_DestroyPort(pAvplay, pAvplay->MasterFrmChn.hWindow, pAvplay->MasterFrmChn.hPort);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("ERR: AVPLAY_DestroyPort fail.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

    }

    pAvplay->MasterFrmChn.hWindow = HI_INVALID_HANDLE;
    pAvplay->MasterFrmChn.hPort = HI_INVALID_HANDLE;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_AddSlavePort(AVPLAY_S *pAvplay, HI_HANDLE hWin)
{
    HI_U32  i;
    HI_S32  Ret;
    HI_U32  Index = AVPLAY_MAX_SLAVE_FRMCHAN;

    for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
    {
        if (AVPLAY_MAX_SLAVE_FRMCHAN == Index)
        {
            if (HI_INVALID_HANDLE == pAvplay->SlaveFrmChn[i].hWindow)
            {
                Index = i;
            }
        }

        if (pAvplay->SlaveFrmChn[i].hWindow == hWin)
        {
            HI_ERR_AVPLAY("this window is already attached!\n");
            return HI_SUCCESS;
        }
    }

    if (Index >= AVPLAY_MAX_SLAVE_FRMCHAN)
    {
        HI_ERR_AVPLAY("avplay has attached max slave window.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = AVPLAY_CreatePort(pAvplay, hWin, VDEC_PORT_SD, &(pAvplay->SlaveFrmChn[Index].hPort));
    if(HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = AVPLAY_SetPortAttr(pAvplay,pAvplay->SlaveFrmChn[Index].hPort,VDEC_PORT_TYPE_SLAVE);
    if(HI_SUCCESS != Ret)
    {
        (HI_VOID)AVPLAY_DestroyPort(pAvplay, hWin, pAvplay->SlaveFrmChn[Index].hPort);
        pAvplay->SlaveFrmChn[Index].hPort = HI_INVALID_HANDLE;

        return Ret;
    }

    pAvplay->SlaveFrmChn[Index].hWindow = hWin;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_DelSlavePort(AVPLAY_S *pAvplay, HI_HANDLE hWin)
{
    HI_U32  i;
    HI_S32  Ret;

    for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
    {
        if (pAvplay->SlaveFrmChn[i].hWindow == hWin)
        {
            break;
        }
    }

    if (i >= AVPLAY_MAX_SLAVE_FRMCHAN)
    {
        HI_INFO_AVPLAY("this is not a attached slave window.\n");
        return HI_SUCCESS;
    }

    /*FATAL: after AVPLAY_DettachWinRelFrame, but AVPLAY_DestroyPort Failed*/
    Ret = AVPLAY_DestroyPort(pAvplay, hWin, pAvplay->SlaveFrmChn[i].hPort);
    if (HI_SUCCESS != Ret)
    {
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    pAvplay->SlaveFrmChn[i].hWindow = HI_INVALID_HANDLE;
    pAvplay->SlaveFrmChn[i].hPort = HI_INVALID_HANDLE;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_AddVirtualPort(AVPLAY_S *pAvplay, HI_HANDLE hWin)
{
    HI_U32  i;
    HI_S32  Ret;
    HI_U32  Index = AVPLAY_MAX_VIR_FRMCHAN;

    for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
    {
        if (AVPLAY_MAX_VIR_FRMCHAN == Index)
        {
            if (HI_INVALID_HANDLE == pAvplay->VirFrmChn[i].hWindow)
            {
                Index = i;
            }
        }

        if (pAvplay->VirFrmChn[i].hWindow == hWin)
        {
            HI_ERR_AVPLAY("this window is already attached!\n");
            return HI_SUCCESS;
        }
    }

    if (Index >= AVPLAY_MAX_VIR_FRMCHAN)
    {
        HI_ERR_AVPLAY("the avplay has attached max window!\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = AVPLAY_CreatePort(pAvplay, hWin, VDEC_PORT_STR, &pAvplay->VirFrmChn[Index].hPort);
    if (HI_SUCCESS != Ret)
    {
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = AVPLAY_SetPortAttr(pAvplay, pAvplay->VirFrmChn[Index].hPort, VDEC_PORT_TYPE_VIRTUAL);
    if (HI_SUCCESS != Ret)
    {
        (HI_VOID)AVPLAY_DestroyPort(pAvplay, hWin, pAvplay->VirFrmChn[Index].hPort);
        pAvplay->VirFrmChn[Index].hPort = HI_INVALID_HANDLE;

        return Ret;
    }

    pAvplay->VirFrmChn[Index].hWindow = hWin;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_DelVirtualPort(AVPLAY_S *pAvplay, HI_HANDLE hWin)
{
    HI_U32  i;
    HI_S32  Ret;

    for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
    {
        if (pAvplay->VirFrmChn[i].hWindow == hWin)
        {
            break;
        }
    }

    if (i >= AVPLAY_MAX_VIR_FRMCHAN)
    {
        HI_ERR_AVPLAY("ERR: this is not a attached master window.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = AVPLAY_DestroyPort(pAvplay, hWin, pAvplay->VirFrmChn[i].hPort);
    if (HI_SUCCESS != Ret)
    {
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    pAvplay->VirFrmChn[i].hWindow   = HI_INVALID_HANDLE;
    pAvplay->VirFrmChn[i].hPort     = HI_INVALID_HANDLE;

    return HI_SUCCESS;

}

HI_S32 AVPLAY_AttachWindow(AVPLAY_S *pAvplay, HI_HANDLE hWin)
{
    HI_S32             Ret;
    HI_DRV_WIN_INFO_S  stWinInfo;

#ifdef HI_HDR_SUPPORT
    if (pAvplay->bHDRPlay)
    {
        if (pAvplay->VidEnable)
        {
            HI_ERR_AVPLAY("Not support attach window as dynamic\n");
            return HI_ERR_AVPLAY_NOT_SUPPORT;
        }
    }
    else
#endif
    {
        if (pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
        {
            (HI_VOID)AVPLAY_RelAllChnFrame(pAvplay);
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
        }
    }

    Ret = HI_MPI_WIN_GetInfo(hWin, &stWinInfo);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_WIN_GetPrivnfo 0x%x.\n", Ret);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    switch (stWinInfo.eType)
    {
        /* homologous window*/ /* CNcomment: 同源窗口 */
        case HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE :
        {
            Ret = AVPLAY_AddMasterPort(pAvplay, stWinInfo.hPrim);
            if (HI_SUCCESS != Ret)
            {
                return HI_FAILURE;
            }

        #ifdef AVPLAY_HOMOLOGOUS_WINDOW_SUPPORT
            Ret = AVPLAY_AddSlavePort(pAvplay, stWinInfo.hSec);
            if (HI_SUCCESS != Ret)
            {
                (HI_VOID)AVPLAY_DelMasterPort(pAvplay, stWinInfo.hPrim);
                return HI_FAILURE;
            }
        #endif

            pAvplay->hSharedOrgWin = hWin;

            break;
        }

        /* analogous master window*/ /* CNcomment: 非同源 主窗口及从窗口 */
        case HI_DRV_WIN_ACTIVE_SINGLE :
        {
            if (hWin == pAvplay->MasterFrmChn.hWindow)
            {
                HI_ERR_AVPLAY("this window is alreay attached!\n");
                return HI_SUCCESS;
            }

            if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
            {
                Ret = AVPLAY_AddMasterPort(pAvplay, hWin);
                if (HI_SUCCESS != Ret)
                {
                    return HI_FAILURE;
                }
            }
            else
            {
                Ret = AVPLAY_AddSlavePort(pAvplay, hWin);
                if (HI_SUCCESS != Ret)
                {
                    return HI_FAILURE;
                }
            }

            break;
        }

        /* analogous virtual window*/ /* CNcomment 非同源 虚拟窗口*/
        case HI_DRV_WIN_VITUAL_SINGLE :
        {
            Ret = AVPLAY_AddVirtualPort(pAvplay, hWin);
            if (HI_SUCCESS != Ret)
            {
                return HI_FAILURE;
            }

            break;
        }

        default :
            HI_ERR_AVPLAY("this window type 0x%x is not support\n", stWinInfo.eType);
            return HI_ERR_AVPLAY_INVALID_PARA;
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_DetachWindow(AVPLAY_S *pAvplay, HI_HANDLE hWin)
{
    HI_S32             Ret;
    HI_DRV_WIN_INFO_S  WinInfo;

#ifdef HI_HDR_SUPPORT
    if (HI_TRUE == pAvplay->bHDRPlay)
    {
        if (pAvplay->VidEnable)
        {
            HI_ERR_AVPLAY("Not support detach window as dynamic\n");
            return HI_ERR_AVPLAY_NOT_SUPPORT;
        }
    }
    else
#endif
    {
        if (pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
        {
            (HI_VOID)AVPLAY_RelAllChnFrame(pAvplay);
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
        }
    }

    Ret = HI_MPI_WIN_GetInfo(hWin, &WinInfo);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_VO_GetWindowInfo.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    switch (WinInfo.eType)
    {
        /* homologous window*/ /* CNcomment: 同源窗口 */
        case HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE :
        {
            Ret = AVPLAY_DelMasterPort(pAvplay, WinInfo.hPrim);
            if (HI_SUCCESS != Ret)
            {
                return HI_FAILURE;
            }

        #ifdef AVPLAY_HOMOLOGOUS_WINDOW_SUPPORT
            Ret = AVPLAY_DelSlavePort(pAvplay, WinInfo.hSec);
            if (HI_SUCCESS != Ret)
            {
                return HI_FAILURE;
            }

            Ret = AVPLAY_LookUpMasterPort(pAvplay);
            if (HI_SUCCESS != Ret)
            {
                return HI_FAILURE;
            }
        #endif

            pAvplay->hSharedOrgWin = HI_INVALID_HANDLE;

            break;
        }

        /* analogous master window*/ /* CNcomment: 非同源 主窗口及从窗口 */
        case HI_DRV_WIN_ACTIVE_SINGLE :
        {
            if (pAvplay->MasterFrmChn.hWindow == hWin)
            {
                Ret = AVPLAY_DelMasterPort(pAvplay, hWin);
                if (HI_SUCCESS != Ret)
                {
                    return HI_FAILURE;
                }

                Ret = AVPLAY_LookUpMasterPort(pAvplay);
                if (HI_SUCCESS != Ret)
                {
                    return HI_FAILURE;
                }
            }
            else
            {
                Ret = AVPLAY_DelSlavePort(pAvplay, hWin);
                if (HI_SUCCESS != Ret)
                {
                    return HI_FAILURE;
                }
            }

            break;
        }

        /* analogous virtual window*/ /* CNcomment 非同源 虚拟窗口*/
        case HI_DRV_WIN_VITUAL_SINGLE :
        {
            Ret = AVPLAY_DelVirtualPort(pAvplay, hWin);
            if (HI_SUCCESS != Ret)
            {
                return HI_FAILURE;
            }

            break;
        }

        default :
            HI_ERR_AVPLAY("this window type 0x%x is not support\n", WinInfo.eType);
            return HI_ERR_AVPLAY_INVALID_PARA;
    }

    return HI_SUCCESS;
}

#ifdef HI_TEE_SUPPORT
HI_S32 AVPLAY_SetTVP(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_TVP_ATTR_S *pstAttr)
{
    HI_S32                  Ret;
    HI_HANDLE               hWindow;
    HI_UNF_VCODEC_ATTR_S    stVcodecAttr = {0};

    if (pAvplay->TVPAttr.bEnable == pstAttr->bEnable)
    {
        return HI_SUCCESS;
    }

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        pAvplay->TVPAttr = *pstAttr;
        return HI_SUCCESS;
    }

    if (pAvplay->VidEnable)
    {
        HI_ERR_AVPLAY("vid chn is enable, can not set trusted video path.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = AVPLAY_GetVdecAttr(pAvplay, &stVcodecAttr);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
    {
        Ret = AVPLAY_FreeVidChn(pAvplay);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }

        pAvplay->TVPAttr.bEnable = pstAttr->bEnable;

        Ret = AVPLAY_MallocVidChn(pAvplay, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }

        Ret = AVPLAY_SetVdecAttr(pAvplay, &stVcodecAttr);
        if (Ret != HI_SUCCESS)
        {
            (HI_VOID)AVPLAY_FreeVidChn(pAvplay);
            HI_ERR_AVPLAY("HI_MPI_WIN_SetEnable failed 0x%x\n", Ret);
            return Ret;
        }
    }
    else
    {
        hWindow = pAvplay->MasterFrmChn.hWindow;

        Ret = HI_MPI_WIN_SetEnable(hWindow, HI_FALSE);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }

        Ret = AVPLAY_DetachWindow(pAvplay, hWindow);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }

        Ret = AVPLAY_FreeVidChn(pAvplay);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }

        pAvplay->TVPAttr.bEnable = pstAttr->bEnable;

        Ret = AVPLAY_MallocVidChn(pAvplay, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }

        Ret = AVPLAY_SetVdecAttr(pAvplay, &stVcodecAttr);
        if (Ret != HI_SUCCESS)
        {
            (HI_VOID)AVPLAY_FreeVidChn(pAvplay);
            return Ret;
        }

        Ret = AVPLAY_AttachWindow(pAvplay, hWindow);
        if (Ret != HI_SUCCESS)
        {
            (HI_VOID)AVPLAY_FreeVidChn(pAvplay);
            return Ret;
        }

        Ret = HI_MPI_WIN_SetEnable(hWindow, HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            (HI_VOID)AVPLAY_DetachWindow(pAvplay, hWindow);
            (HI_VOID)AVPLAY_FreeVidChn(pAvplay);
            HI_ERR_AVPLAY("HI_MPI_WIN_SetEnable failed 0x%x\n", Ret);
            return Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_GetTVP(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_TVP_ATTR_S *pstAttr)
{
    *pstAttr = pAvplay->TVPAttr;

    return HI_SUCCESS;
}
#endif

#ifndef HI_ADVCA_FUNCTION_RELEASE
static HI_CHAR* AVPLAY_GetVidType(HI_UNF_VCODEC_TYPE_E enType)
{
    if ((HI_U32)enType >= HI_UNF_VCODEC_TYPE_BUTT)
    {
        return "UNKNOWN";
    }

    return g_AvplayVidType[enType];
}

static HI_VOID AVPLAY_BasicProcInfo(AVPLAY_S *pAvplay, HI_PROC_SHOW_BUFFER_S *ProcBuf)
{
    HI_CHAR            *StreamType;
    HI_CHAR            *CurStatus;
    HI_CHAR            *Overflow;
    HI_CHAR             szSyncID[16]  = {0};
    HI_CHAR             szDemuxID[16] = {0};

    snprintf(szSyncID, sizeof(szSyncID), "sync%02d", pAvplay->hSync & 0xff);

    if (HI_UNF_AVPLAY_STREAM_TYPE_ES == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        snprintf(szDemuxID, sizeof(szDemuxID), "INVALID");
    }
    else
    {
        snprintf(szDemuxID, sizeof(szDemuxID), "%d", pAvplay->AvplayAttr.u32DemuxId);
    }

    switch (pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        case HI_UNF_AVPLAY_STREAM_TYPE_TS :
            StreamType = "TS";
            break;
        case HI_UNF_AVPLAY_STREAM_TYPE_ES :
            StreamType = "ES";
            break;
        default :
            StreamType = "UNKNOWN";
            break;
    }

    switch (pAvplay->CurStatus)
    {
        case HI_UNF_AVPLAY_STATUS_STOP :
            CurStatus = "STOP";
            break;
        case HI_UNF_AVPLAY_STATUS_PREPLAY :
            CurStatus = "PREPLAY";
            break;
        case HI_UNF_AVPLAY_STATUS_PLAY :
            CurStatus = "PLAY";
            break;
        case HI_UNF_AVPLAY_STATUS_TPLAY :
            CurStatus = "TPLAY";
            break;
        case HI_UNF_AVPLAY_STATUS_PAUSE :
            CurStatus = "PAUSE";
            break;
        case HI_UNF_AVPLAY_STATUS_EOS :
            CurStatus = "EOS";
            break;
        case HI_UNF_AVPLAY_STATUS_SEEK :
            CurStatus = "SEEK";
            break;
        default:
            CurStatus = "UNKNOWN";
            break;
    }

    switch (pAvplay->OverflowProc)
    {
        case HI_UNF_AVPLAY_OVERFLOW_RESET :
            Overflow = "RESET";
            break;
        case HI_UNF_AVPLAY_OVERFLOW_DISCARD :
            Overflow = "DISCARD";
            break;
        default:
            Overflow = "UNKNOWN";
            break;
    }

    HI_PROC_Printf(ProcBuf, "----------------------Hisilicon AVPLAY%d Out Info-------------------\n",
        GET_SYNC_ID(pAvplay->hSync));

    HI_PROC_Printf(ProcBuf,
                    "Stream Type           :%-10s   |DmxId                 :%s\n"
                    "CurStatus             :%-10s   |OverflowProc          :%s\n"
                    "Sync ID               :%-10s   |ThreadID              :%d\n"
                    "AudThreadScheTimeOut  :%-10u   |AudThreadExeTimeOut   :%-10u\n"
                    "VidThreadScheTimeOut  :%-10u   |VidThreadExeTimeOut   :%-10u\n"
                    "CpuFreqScheTimeCnt    :%-10u\n",
                    StreamType, szDemuxID, CurStatus, Overflow,
                    szSyncID, pAvplay->ThreadID,
                    pAvplay->DebugInfo.AudThreadScheTimeOutCnt, pAvplay->DebugInfo.AudThreadExeTimeOutCnt,
                    pAvplay->DebugInfo.VidThreadScheTimeOutCnt, pAvplay->DebugInfo.VidThreadExeTimeOutCnt,
                    pAvplay->DebugInfo.CpuFreqScheTimeCnt
                    );

}

static HI_VOID AVPLAY_HDRProcInfo(AVPLAY_S *pAvplay, HI_PROC_SHOW_BUFFER_S *ProcBuf)
{
#ifdef HI_HDR_SUPPORT
    HI_CHAR            *VidElType;
    HI_CHAR            *VidBlType;
    HI_CHAR            *HDRSyncTarget;
    HI_CHAR            *HDRStreamType;

    if (!pAvplay->bHDRPlay)
    {
        return;
    }

    VidBlType = AVPLAY_GetVidType(pAvplay->VdecAttr.enType);
    VidElType = AVPLAY_GetVidType(pAvplay->AvplayHDRAttr.enElType);

    if (pAvplay->hElVdec == HI_INVALID_HANDLE)
    {
        VidElType = "NONE";
    }

    switch (pAvplay->HDRSyncTarget)
    {
        case AVPLAY_HDR_CHAN_BL :
            HDRSyncTarget = "BL";
            break;
        case AVPLAY_HDR_CHAN_EL :
            HDRSyncTarget = "EL";
            break;
        default :
            HDRSyncTarget = "UNKNOWN";
            break;
    }

    switch (pAvplay->AvplayHDRAttr.enHDRStreamType)
    {
        case HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_SINGLE_VES :
            HDRStreamType = "DlSingleVes";
            break;

        case HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_DUAL_VES :
            HDRStreamType = "DlDualVes";
            break;

        case HI_UNF_AVPLAY_HDR_STREAM_TYPE_SL_VES :
            HDRStreamType = "SingleVes";
            break;

        default :
            HDRStreamType = "UNKNOWN";
            break;
    }

    if (pAvplay->bHDRPlay)
    {
        HI_PROC_Printf(ProcBuf,
            "-------------------------------HDR INFO----------------------------\n"
            "HDREnable             :%-10s   |HDRStream             :%-10s\n"
            "BlType                :%-10s   |ElType                :%-10s\n"
            "HDRSyncTarget         :%-10s   |HDRSyncTargetTime     :%-10d\n"
            "BlPts                 :%-10d   |ElPts                 :%-10d\n"
            "BlSyncProc            :%-10d   |ElSyncProc            :%-10d\n"
            "PtsDiff               :%-10d\n",
            (pAvplay->bHDRPlay) ? "TRUE" : "FALSE",
            HDRStreamType,
            VidBlType, VidElType,
            HDRSyncTarget, pAvplay->HDRSyncTargetTime,
            pAvplay->CurBlPts, pAvplay->CurElPts,
            pAvplay->BlSyncProc, pAvplay->ElSyncProc,
            pAvplay->CurBlPts - pAvplay->CurElPts);
    }

#endif
    return;
}

static HI_VOID AVPLAY_VidProcInfo(AVPLAY_S *pAvplay, HI_PROC_SHOW_BUFFER_S *ProcBuf)
{
    HI_U32              i;
    HI_CHAR            *VidType;
    HI_CHAR            *DecMode;
    HI_CHAR             szFrcInRate[16]   = {0};
    HI_CHAR             szFrcOutRate[16]  = {0};
    HI_CHAR             szTplaySpeed[16]  = {0};

    snprintf(szFrcInRate, sizeof(szFrcInRate), "%d.%d",
        pAvplay->FrcParamCfg.u32InRate/100, pAvplay->FrcParamCfg.u32InRate%100);

    snprintf(szFrcOutRate, sizeof(szFrcOutRate), "%d.%d",
        pAvplay->FrcParamCfg.u32OutRate/100, pAvplay->FrcParamCfg.u32OutRate%100);

    snprintf(szTplaySpeed, sizeof(szTplaySpeed), "%d.%d",
        pAvplay->FrcParamCfg.u32PlayRate/256, pAvplay->FrcParamCfg.u32PlayRate % 256 * 100 / 256);

    switch (pAvplay->VdecAttr.enMode)
    {
        case HI_UNF_VCODEC_MODE_NORMAL :
            DecMode = "NORMAL";
            break;
        case HI_UNF_VCODEC_MODE_IP :
            DecMode = "IP";
            break;
        case HI_UNF_VCODEC_MODE_I :
            DecMode = "I";
            break;
        case HI_UNF_VCODEC_MODE_DROP_INVALID_B :
            DecMode = "DROP_INVALID_B";
            break;
        default :
            DecMode = "UNKNOWN";
            break;
    }

    VidType = AVPLAY_GetVidType(pAvplay->VdecAttr.enType);

    HI_PROC_Printf(ProcBuf,
                    "------------------------------VID CHANNEL--------------------------\n"
                    "Vid Enable            :%-10s   |Vdec Type             :%s\n"
                    "VidOverflowNum        :%-10d   |Vdec Mode             :%s\n"
                    "VidPid                :0x%-10x |FrcEnable             :%s\n"
                    "FrcInRate             :%-10s   |FrcOutRate            :%s\n"
                    "TplaySpeed            :%-10s   |LowDelayEnable        :%s\n"
                    "TvpEnable             :%-10s   |Vdec ID               :vdec%02d\n",
                    (pAvplay->VidEnable) ? "TRUE" : "FALSE",
                    VidType,
                    pAvplay->DebugInfo.VidOverflowNum,
                    DecMode,
                    pAvplay->DmxVidPid,
                    (pAvplay->bFrcEnable) ? "TRUE" : "FALSE",
                    szFrcInRate,
                    szFrcOutRate,
                    szTplaySpeed,
                    (pAvplay->LowDelayAttr.bEnable) ? "TRUE" : "FALSE",
                #ifdef HI_TEE_SUPPORT
                    (pAvplay->TVPAttr.bEnable) ? "TRUE" : "FALSE",
                #else
                    "FALSE",
                #endif
                    pAvplay->hVdec & 0xff
                    );

    //HDR Proc info
    AVPLAY_HDRProcInfo(pAvplay, ProcBuf);

    if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
    {
        HI_PROC_Printf(ProcBuf, "FrameChanID           :vpss_port%04x->win%04x(master)\n",
                        pAvplay->MasterFrmChn.hPort & 0xffff,
                        pAvplay->MasterFrmChn.hWindow & 0xffff);

    }

    for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
    {
        if (HI_INVALID_HANDLE != pAvplay->SlaveFrmChn[i].hWindow)
        {
            HI_PROC_Printf(ProcBuf, "FrameChanID           :vpss_port%04x->win%04x(slave%02d)\n",
                            pAvplay->SlaveFrmChn[i].hPort & 0xffff,
                            pAvplay->SlaveFrmChn[i].hWindow & 0xffff,
                            i);
        }
    }

    for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
    {
        if (HI_INVALID_HANDLE != pAvplay->VirFrmChn[i].hWindow)
        {
            HI_PROC_Printf(ProcBuf, "FrameChanID           :vpss_port%04x->win%04x(virtual%02d)\n",
                pAvplay->VirFrmChn[i].hPort & 0xffff,
                pAvplay->VirFrmChn[i].hWindow & 0xffff, i);
        }
    }

    HI_PROC_Printf(ProcBuf,
                    "AcquireFrame(Try/OK)  :%u/%u\n",
                    pAvplay->DebugInfo.AcquireVidFrameNum,
                    pAvplay->DebugInfo.AcquiredVidFrameNum
                    );

    if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
    {
        HI_PROC_Printf(ProcBuf,
                        "SendFrame(Try/OK)     :%u/%u(master)\n",
                        pAvplay->DebugInfo.MasterVidStat.SendNum,
                        pAvplay->DebugInfo.MasterVidStat.PlayNum +
                        pAvplay->DebugInfo.MasterVidStat.RepeatNum
                        );
    }

    for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
    {
        if (HI_INVALID_HANDLE != pAvplay->SlaveFrmChn[i].hWindow)
        {
            HI_PROC_Printf(ProcBuf,
                            "SendFrame(Try/OK)     :%u/%u(slave%02d)\n",
                            pAvplay->DebugInfo.SlaveVidStat[i].SendNum,
                            pAvplay->DebugInfo.SlaveVidStat[i].PlayNum +
                            pAvplay->DebugInfo.SlaveVidStat[i].RepeatNum,
                            i
                            );
        }
    }

    for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
    {
        if (HI_INVALID_HANDLE != pAvplay->VirFrmChn[i].hWindow)
        {
            HI_PROC_Printf(ProcBuf,
                            "SendFrame(Try/OK)     :%u/%u(virtual%02d)\n",
                            pAvplay->DebugInfo.VirVidStat[i].SendNum,
                            pAvplay->DebugInfo.VirVidStat[i].PlayNum +
                            pAvplay->DebugInfo.VirVidStat[i].RepeatNum,
                            i
                            );
        }
    }

    return;
}

static HI_VOID AVPLAY_FccProcInfo(AVPLAY_S *pAvplay, HI_PROC_SHOW_BUFFER_S *ProcBuf)
{
    if (!pAvplay->bVidPreEnable)
    {
        return;
    }

    HI_PROC_Printf(ProcBuf,
                    "--------------------------------FCC INFO---------------------------\n"
                    "VidPts                :%-10d   |AudPts                :%d\n"
                    "VidFirstWaterLine(%)  :%-10d   |AudWaterLine(%)       :%d\n"
                    "VidSecondWaterLine(%) :%-10d   |                         \n",
                    pAvplay->PreVidPts, pAvplay->PreAudPts,
                    pAvplay->PreVidFirstWaterLine, pAvplay->PreAudWaterLine,
                    pAvplay->PreVidSecondWaterLine
                    );

    return;
}

static HI_VOID AVPLAY_EosProcInfo(AVPLAY_S *pAvplay, HI_PROC_SHOW_BUFFER_S *ProcBuf)
{
    if (pAvplay->bSetEosFlag)
    {
        HI_PROC_Printf(ProcBuf,
            "--------------------------------EOS INFO---------------------------\n"
            "AoBufEmpty            :%-10s   |EndOfStream           :%-10s\n"
            "EndOfFrame            :%-10s   |AllPortCompleteFrm    :%-10s\n"
            "UsedBufNum            :%-10s   |FrameNumInBufQn       :%-10s\n",
            pAvplay->DebugInfo.bAoBufEmpty ? "*" : "-", pAvplay->DebugInfo.bEndOfStream ? "*" : "-",
            pAvplay->DebugInfo.bEndOfFrame ? "*" : "-", pAvplay->DebugInfo.bAllPortCompleteFrm ? "*" : "-",
            (pAvplay->DebugInfo.UsedBufNum == 0) ? "*" : "-",  (pAvplay->DebugInfo.FrameNumInBufQn == 0) ? "*" : "-"
            );
    }

    return;
}

static HI_VOID AVPLAY_AudProcInfo(AVPLAY_S *pAvplay, HI_PROC_SHOW_BUFFER_S *ProcBuf)
{
    HI_U32              i;
    HI_HA_DECODE_LIB_S *pHaLib = HI_NULL;

    HI_PROC_Printf(ProcBuf,
                  "------------------------------AUD CHANNEL--------------------------\n"
                  "Aud Enable            :%-10s   |Adec Type             :%s\n"
                  "AudOverflowNum        :%-10d   |AdecDelayMs           :%u\n"
                  "DmxAudChnNum          :%-10d   |DmxAudChnMode         :%s\n",
                  (pAvplay->AudEnable) ? "TRUE" : "FALSE",
                  pAvplay->AdecNameInfo.szHaCodecName,
                  pAvplay->DebugInfo.AudOverflowNum,
                  pAvplay->AdecDelayMs,
                  pAvplay->DmxAudChnNum, (pAvplay->DmxAudChnSecMode == HI_UNF_DMX_SECURE_MODE_TEE) ? "TEE" : "NORMAL"
                  );

    HI_PROC_Printf(ProcBuf, "DmxAudPid             :");

    for (i = 0; i < pAvplay->DmxAudChnNum; i++)
    {
        HI_PROC_Printf(ProcBuf, "%#x", pAvplay->DmxAudPid[i]);

        if ((pAvplay->DmxAudChnNum > 1) && (i == pAvplay->CurDmxAudChn))
        {
            HI_PROC_Printf(ProcBuf, "(play)");
        }

        if (i < pAvplay->DmxAudChnNum - 1)
        {
            HI_PROC_Printf(ProcBuf, ",");
        }
    }

    HI_PROC_Printf(ProcBuf, "\n");

    HI_PROC_Printf(ProcBuf, "Adec ID               :adec%02d\n", pAvplay->hAdec & 0xff);

    for (i = 0; i < pAvplay->TrackNum; i++)
    {
        HI_PROC_Printf(ProcBuf, "Track ID              :track%02d", pAvplay->hTrack[i] & 0xff);

        if (pAvplay->hSyncTrack == pAvplay->hTrack[i])
        {
            HI_PROC_Printf(ProcBuf, "(master)");
        }

        HI_PROC_Printf(ProcBuf, "\n");
    }

    HI_PROC_Printf(ProcBuf,
                    "AcquireStream(Try/OK)   :%u/%u\n"
                    "SendStream(Try/OK)      :%u/%u\n"
                    "AcquireFrame(Try/OK)    :%u/%u\n",
                    pAvplay->DebugInfo.AcquireAudEsNum,    pAvplay->DebugInfo.AcquiredAudEsNum,
                    pAvplay->DebugInfo.SendAudEsNum,       pAvplay->DebugInfo.SendedAudEsNum,
                    pAvplay->DebugInfo.AcquireAudFrameNum, pAvplay->DebugInfo.AcquiredAudFrameNum
                    );

    HI_PROC_Printf(ProcBuf,
                    "AcquireADStream(Try/OK) :%u/%u\n"
                    "SendADStream(Try/OK)    :%u/%u\n",
                    pAvplay->DebugInfo.AcquireAudADEsNum,  pAvplay->DebugInfo.AcquiredAudADEsNum,
                    pAvplay->DebugInfo.SendAudADEsNum,     pAvplay->DebugInfo.SendedAudADEsNum
                    );

    for (i = 0; i < pAvplay->TrackNum; i++)
    {
        HI_PROC_Printf(ProcBuf,
                        "SendFrame(Try/OK)     :%u/%u",
                        pAvplay->DebugInfo.SendAudFrameNum,
                        pAvplay->DebugInfo.SendedAudFrameNum
                        );

        if (pAvplay->hSyncTrack == pAvplay->hTrack[i])
        {
            HI_PROC_Printf(ProcBuf, "(master)");
        }

        HI_PROC_Printf(ProcBuf, "\n");
    }

    (HI_VOID)HI_MPI_ADEC_GetDecoderList((HI_HA_DECODE_LIB_S**)(&pHaLib));

    HI_PROC_Printf(ProcBuf, "--------------------------------AUD LIB----------------------------\n");
    while (pHaLib != HI_NULL)
    {
        HI_PROC_Printf(ProcBuf, "%s\n", pHaLib->szLibName);
        pHaLib = pHaLib->pstNext;
    }

    HI_PROC_Printf(ProcBuf, "\n");

    return;
}

static HI_S32 AVPLAY_ProcRead(HI_PROC_SHOW_BUFFER_S *ProcBuf, HI_VOID *Data)
{
    HI_SIZE_T           Id = (HI_SIZE_T)Data;
    AVPLAY_S           *pAvplay;

    AVPLAY_INST_LOCK(Id);

    pAvplay = g_Avplay[Id].Avplay;
    if (HI_NULL == pAvplay)
    {
        AVPLAY_INST_UNLOCK(Id);
        return HI_FAILURE;
    }

    AVPLAY_BasicProcInfo(pAvplay, ProcBuf);

    AVPLAY_VidProcInfo(pAvplay, ProcBuf);

    AVPLAY_FccProcInfo(pAvplay, ProcBuf);

    AVPLAY_EosProcInfo(pAvplay, ProcBuf);

    AVPLAY_AudProcInfo(pAvplay, ProcBuf);

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

static HI_S32 AVPLAY_ProcWrite(HI_PROC_SHOW_BUFFER_S *ProcBuf, HI_U32 Argc, HI_U8 *Argv[], HI_VOID *Data)
{
    HI_SIZE_T   Id      = (HI_SIZE_T)Data;
    AVPLAY_S   *pAvplay = HI_NULL;
    HI_CHAR    *String  = (HI_CHAR*)Argv[0];

    if ( (Argc < 1) || (0 == strncmp(String, "help", sizeof("help"))) )
    {
        HI_PROC_Printf(ProcBuf, "echo FrcEnable=true|false > /proc/msp/avplayxx, enable or disable frc\n");
        HI_PROC_Printf(ProcBuf, "echo BoostCpuFreq=true|false > /proc/msp/avplayxx, enable or disable Boost\n");
        return HI_FAILURE;
    }

    if (0 == strncmp(String, "FrcEnable", strlen("FrcEnable")))
    {
        AVPLAY_INST_LOCK(Id);

        pAvplay = g_Avplay[Id].Avplay;
        if (HI_NULL == pAvplay)
        {
            AVPLAY_INST_UNLOCK(Id);

            return -1;
        }

        String += sizeof("FrcEnable");

        if (0 == strncmp(String, "true", sizeof("true")))
        {
            pAvplay->bFrcEnable = HI_TRUE;
        }
        else if (0 == strncmp(String, "false", sizeof("false")))
        {
            pAvplay->bFrcEnable = HI_FALSE;
        }

        AVPLAY_INST_UNLOCK(Id);
    }
    else if (0 == strncmp(String, "BoostCpuFreq", strlen("BoostCpuFreq")))
    {
        AVPLAY_INST_LOCK(Id);

        pAvplay = g_Avplay[Id].Avplay;
        if (HI_NULL == pAvplay)
        {
            AVPLAY_INST_UNLOCK(Id);

            return -1;
        }

        String += sizeof("BoostCpuFreq");

        if (0 == strncmp(String, "true", sizeof("true")))
        {
            pAvplay->bBoostCpuFreq = HI_TRUE;
        }
        else if (0 == strncmp(String, "false", sizeof("false")))
        {
            pAvplay->bBoostCpuFreq = HI_FALSE;
        }

        AVPLAY_INST_UNLOCK(Id);
    }
    else
    {
        HI_PROC_Printf(ProcBuf, "Unknow Command!\n");
    }

    return HI_SUCCESS;
}
#endif

HI_S32 AVPLAY_GetDmxAudChnMode(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_AUD_SECURITY_ATTR_S *pAudSecAttr)
{
    pAudSecAttr->bEnable = (pAvplay->DmxAudChnSecMode == HI_UNF_DMX_SECURE_MODE_TEE) ? HI_TRUE : HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_SetDmxAudChnMode(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_AUD_SECURITY_ATTR_S *pAudSecAttr)
{
    HI_S32                    Ret;
    HI_U32                    i, j;
    HI_U32                    DmxAudChnNum;
    HI_UNF_DMX_CHAN_ATTR_S    DmxChnAttr;
    HI_UNF_DMX_SECURE_MODE_E  DmxChnSecMode;

    if (pAvplay->AudEnable)
    {
        HI_ERR_AVPLAY("aud chn is running, can not set dmx channel mode.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    DmxChnSecMode = pAudSecAttr->bEnable ? HI_UNF_DMX_SECURE_MODE_TEE : HI_UNF_DMX_SECURE_MODE_NONE;

    if (pAvplay->DmxAudChnSecMode == DmxChnSecMode)
    {
        HI_INFO_AVPLAY("aud chn has been set in the same mode\n");
        return HI_SUCCESS;
    }

    if (0 == pAvplay->DmxAudChnNum)
    {
        pAvplay->DmxAudChnSecMode = DmxChnSecMode;
    }
    else if (1 == pAvplay->DmxAudChnNum)
    {
        Ret = AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("AVPLAY_FreeDmxChn fail\n");
            return HI_FAILURE;
        }

        pAvplay->DmxAudChnSecMode = DmxChnSecMode;

        Ret = AVPLAY_MallocDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("AVPLAY_MallocDmxChn\n");
            return HI_FAILURE;
        }

        Ret = HI_MPI_DMX_SetChannelPID(pAvplay->hDmxAud[0], pAvplay->DmxAudPid[0]);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_DMX_SetChannelPID 0x%x\n", Ret);
            return HI_FAILURE;
        }
    }
    else if (1 < pAvplay->DmxAudChnNum)
    {
        DmxAudChnNum = pAvplay->DmxAudChnNum;

        Ret = AVPLAY_FreeDmxChn(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("AVPLAY_FreeDmxChn fail\n");
            return Ret;
        }

        pAvplay->DmxAudChnSecMode = DmxChnSecMode;

        DmxChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_AUD;
        DmxChnAttr.enCRCMode     = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
        DmxChnAttr.enOutputMode  = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
        DmxChnAttr.enSecureMode  = DmxChnSecMode;
        DmxChnAttr.u32BufSize    = pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize / 3;
        for (i = 0; i < DmxAudChnNum; i++)
        {
            Ret = HI_MPI_DMX_CreateChannel(pAvplay->AvplayAttr.u32DemuxId, &DmxChnAttr, &(pAvplay->hDmxAud[i]));
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("HI_MPI_DMX_CreateChannel fail 0x%x\n", Ret);
                break;
            }

            Ret = HI_MPI_DMX_SetChannelPID(pAvplay->hDmxAud[i], pAvplay->DmxAudPid[i]);
            if (HI_SUCCESS != Ret)
            {
                (HI_VOID)HI_MPI_DMX_DestroyChannel(pAvplay->hDmxAud[i]);
                HI_ERR_AVPLAY("HI_MPI_DMX_SetChannelPID fail 0x%x\n", Ret);
                break;
            }
        }

        if (i != DmxAudChnNum)
        {
            for (j = 0; j < i; j++)
            {
                (HI_VOID)HI_MPI_DMX_DestroyChannel(pAvplay->hDmxAud[j]);
            }

            return HI_FAILURE;
        }

        pAvplay->DmxAudChnNum = DmxAudChnNum;
    }

    return HI_SUCCESS;
}

HI_BOOL AVPLAY_IsStopStatus(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn)
{
    /*The relevant channel is already stopped*/
    if (   ((HI_UNF_AVPLAY_MEDIA_CHAN_AUD == enChn) && (!pAvplay->AudEnable) && (!pAvplay->bAudPreEnable))
        || ((HI_UNF_AVPLAY_MEDIA_CHAN_VID == enChn) && (!pAvplay->VidEnable) && (!pAvplay->bVidPreEnable)) )
    {
        return HI_TRUE;
    }

    if (   (((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_AUD | (HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID) == enChn)
        && (!pAvplay->AudEnable) && (!pAvplay->bAudPreEnable)
        && (!pAvplay->VidEnable) && (!pAvplay->bVidPreEnable))
    {
       return HI_TRUE;
    }

    return HI_FALSE;
}

HI_S32 AVPLAY_SetBlockStopMode(AVPLAY_S *pAvplay, HI_UNF_AVPLAY_STOP_OPT_S *pStop)
{
    HI_S32   Ret;
    HI_U32   StartSysTime;
    HI_U32   EndSysTime;
    HI_U32   DurationTime;
    HI_BOOL  Block;

    Block = (pStop->u32TimeoutMs != SYS_TIME_MAX) ? HI_FALSE : HI_TRUE;

    Ret = AVPLAY_SetEosFlag(pAvplay);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    StartSysTime = AVPLAY_GetSysTime();

    while (1)
    {
        if (HI_UNF_AVPLAY_STATUS_EOS == pAvplay->CurStatus)
        {
            break;
        }

        if (!Block)
        {
            EndSysTime = AVPLAY_GetSysTime();

            if (EndSysTime > StartSysTime)
            {
                DurationTime = EndSysTime - StartSysTime;
            }
            else
            {
                DurationTime = (0xFFFFFFFFU - StartSysTime) + 1 + EndSysTime;
            }

            if (DurationTime >= pStop->u32TimeoutMs)
            {
                HI_ERR_AVPLAY("eos proc timeout.\n");
                break;
            }
        }

        (HI_VOID)usleep(AVPLAY_SYS_SLEEP_TIME * 1000);
    }

    return HI_SUCCESS;
}

HI_S32 AVPLAY_GetVidBuf(AVPLAY_S             *pAvplay,
                        HI_UNF_AVPLAY_BUFID_E enBufId,
                        HI_U32                u32ReqLen,
                        HI_UNF_STREAM_BUF_S  *pstData)
{
    HI_S32      Ret;

    if (!pAvplay->VidEnable)
    {
        HI_WARN_AVPLAY("vid chn is stopped.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_UNF_AVPLAY_BUF_ID_ES_VID == enBufId)
    {
        Ret = HI_MPI_VDEC_ChanGetBuffer(pAvplay->hVdec, u32ReqLen, &pAvplay->AvplayVidEsBuf);
        if (Ret != HI_SUCCESS)
        {
            if (Ret != HI_ERR_VDEC_BUFFER_FULL)
            {
                HI_WARN_AVPLAY("WARN: call HI_MPI_VDEC_ChanGetBuffer failed 0x%x.\n", Ret);
            }

            return Ret;
        }

        pstData->pu8Data = pAvplay->AvplayVidEsBuf.pu8Addr;
        pstData->u32Size = pAvplay->AvplayVidEsBuf.u32BufSize;
    }

#ifdef HI_HDR_SUPPORT
    if (HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_BL == enBufId)
    {
        Ret = HI_MPI_VDEC_ChanGetBuffer(pAvplay->hVdec, u32ReqLen, &pAvplay->AvplayBlEsBuf);
        if (Ret != HI_SUCCESS)
        {
            if (Ret != HI_ERR_VDEC_BUFFER_FULL)
            {
                HI_WARN_AVPLAY("WARN: call HI_MPI_VDEC_ChanGetBuffer failed 0x%x.\n", Ret);
            }
            return Ret;
        }

        pstData->pu8Data = pAvplay->AvplayBlEsBuf.pu8Addr;
        pstData->u32Size = pAvplay->AvplayBlEsBuf.u32BufSize;
    }

    if (HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_EL == enBufId)
    {
        Ret = HI_MPI_VDEC_ChanGetBuffer(pAvplay->hElVdec, u32ReqLen, &pAvplay->AvplayElEsBuf);
        if (Ret != HI_SUCCESS)
        {
            if (Ret != HI_ERR_VDEC_BUFFER_FULL)
            {
                HI_WARN_AVPLAY("WARN: call HI_MPI_VDEC_ChanGetBuffer failed 0x%x.\n", Ret);
            }
            return Ret;
        }

        pstData->pu8Data = pAvplay->AvplayElEsBuf.pu8Addr;
        pstData->u32Size = pAvplay->AvplayElEsBuf.u32BufSize;
    }

    if (HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_DL == enBufId)
    {
        Ret = HI_MPI_VDEC_ChanGetBuffer(pAvplay->hVdec, u32ReqLen, &pAvplay->AvplayBlEsBuf);
        Ret|= HI_MPI_VDEC_ChanGetBuffer(pAvplay->hElVdec, u32ReqLen, &pAvplay->AvplayElEsBuf);
        if (Ret != HI_SUCCESS)
        {
            if (Ret != HI_ERR_VDEC_BUFFER_FULL)
            {
                HI_WARN_AVPLAY("WARN: call HI_MPI_VDEC_ChanGetBuffer failed 0x%x.\n", Ret);
            }
            return Ret;
        }

        pstData->pu8Data = pAvplay->AvplayBlEsBuf.pu8Addr;
        pstData->u32Size = pAvplay->AvplayBlEsBuf.u32BufSize;
    }
#endif

    return HI_SUCCESS;
}

HI_S32 AVPLAY_GetAudBuf(AVPLAY_S             *pAvplay,
                        HI_UNF_AVPLAY_BUFID_E enBufId,
                        HI_U32                u32ReqLen,
                        HI_UNF_STREAM_BUF_S  *pstData)
{
    HI_S32      Ret;

    if (HI_UNF_AVPLAY_BUF_ID_ES_AUD == enBufId)
    {
        if (!pAvplay->AudEnable)
        {
            HI_WARN_AVPLAY("aud chn is stopped.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        Ret = HI_MPI_ADEC_GetBuffer(pAvplay->hAdec, u32ReqLen, &pAvplay->AvplayAudEsBuf);
        if (Ret != HI_SUCCESS)
        {
            if ((Ret != HI_ERR_ADEC_IN_BUF_FULL) && (Ret != HI_ERR_ADEC_IN_PTSBUF_FULL) )
            {
                HI_ERR_AVPLAY("ERR: HI_MPI_ADEC_GetBuffer failed 0x%x.\n", Ret);
            }

            return Ret;
        }

        pstData->pu8Data = pAvplay->AvplayAudEsBuf.pu8Data;
        pstData->u32Size = pAvplay->AvplayAudEsBuf.u32Size;
    }

    if (HI_UNF_AVPLAY_BUF_ID_ES_AUD_AD == enBufId)
    {
        if (!pAvplay->AdAttr.bAdEnble)
        {
            HI_WARN_AVPLAY("aud chn not support AD.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        if (HI_INVALID_HANDLE == pAvplay->hAdec)
        {
            HI_WARN_AVPLAY("aud chn not open.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        Ret = HI_MPI_ADEC_GetADBuffer(pAvplay->hAdec, u32ReqLen, &pAvplay->AvplayAdAudEsBuf);
        if (Ret != HI_SUCCESS)
        {
            if ((Ret != HI_ERR_ADEC_IN_BUF_FULL) && (Ret != HI_ERR_ADEC_IN_PTSBUF_FULL) )
            {
                HI_ERR_AVPLAY("HI_MPI_ADEC_GetADBuffer failed 0x%x.\n", Ret);
            }

            return Ret;
        }

        pstData->pu8Data = pAvplay->AvplayAdAudEsBuf.pu8Data;
        pstData->u32Size = pAvplay->AvplayAdAudEsBuf.u32Size;
    }

    return HI_SUCCESS;
}


HI_S32 AVPLAY_PutVidBuf(AVPLAY_S                     *pAvplay,
                        HI_UNF_AVPLAY_BUFID_E         enBufId,
                        HI_U32                        u32ValidDataLen,
                        HI_U32                        u32Pts,
                        HI_UNF_AVPLAY_PUTBUFEX_OPT_S *pstExOpt)
{
    HI_S32      Ret;

    if (!pAvplay->VidEnable)
    {
        HI_ERR_AVPLAY("vid chn is stopped.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_UNF_AVPLAY_BUF_ID_ES_VID == enBufId)
    {
        pAvplay->AvplayVidEsBuf.u32BufSize     = u32ValidDataLen;
        pAvplay->AvplayVidEsBuf.u64Pts         = u32Pts;
        pAvplay->AvplayVidEsBuf.bEndOfFrame    = pstExOpt->bEndOfFrm;
        pAvplay->AvplayVidEsBuf.bDiscontinuous = pstExOpt->bContinue ? HI_FALSE : HI_TRUE;

        Ret = HI_MPI_VDEC_ChanPutBuffer(pAvplay->hVdec, &pAvplay->AvplayVidEsBuf);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanPutBuffer failed 0x%x\n", Ret);
            return Ret;
        }
    }

#ifdef HI_HDR_SUPPORT
    if (HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_BL == enBufId)
    {
        pAvplay->AvplayBlEsBuf.u32BufSize     = u32ValidDataLen;
        pAvplay->AvplayBlEsBuf.u64Pts         = u32Pts;
        pAvplay->AvplayBlEsBuf.bEndOfFrame    = pstExOpt->bEndOfFrm;
        pAvplay->AvplayBlEsBuf.bDiscontinuous = pstExOpt->bContinue ? HI_FALSE : HI_TRUE;

        Ret = HI_MPI_VDEC_ChanPutBuffer(pAvplay->hVdec, &pAvplay->AvplayBlEsBuf);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanPutBuffer failed 0x%x\n", Ret);
            return Ret;
        }
    }

    if (HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_EL == enBufId)
    {
        pAvplay->AvplayElEsBuf.u32BufSize     = u32ValidDataLen;
        pAvplay->AvplayElEsBuf.u64Pts         = u32Pts;
        pAvplay->AvplayElEsBuf.bEndOfFrame    = pstExOpt->bEndOfFrm;
        pAvplay->AvplayElEsBuf.bDiscontinuous = pstExOpt->bContinue ? HI_FALSE : HI_TRUE;

        Ret = HI_MPI_VDEC_ChanPutBuffer(pAvplay->hElVdec, &pAvplay->AvplayElEsBuf);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanPutBuffer failed 0x%x\n", Ret);
            return Ret;
        }
    }

    if (HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_DL == enBufId)
    {
        pAvplay->AvplayBlEsBuf.u32BufSize     = u32ValidDataLen;
        pAvplay->AvplayBlEsBuf.u64Pts         = u32Pts;
        pAvplay->AvplayBlEsBuf.bEndOfFrame    = pstExOpt->bEndOfFrm;
        pAvplay->AvplayBlEsBuf.bDiscontinuous = pstExOpt->bContinue ? HI_FALSE : HI_TRUE;

        pAvplay->AvplayElEsBuf.u32BufSize     = u32ValidDataLen;
        pAvplay->AvplayElEsBuf.u64Pts         = u32Pts;
        pAvplay->AvplayElEsBuf.bEndOfFrame    = pstExOpt->bEndOfFrm;
        pAvplay->AvplayElEsBuf.bDiscontinuous = pstExOpt->bContinue ? HI_FALSE : HI_TRUE;

        memcpy(pAvplay->AvplayElEsBuf.pu8Addr, pAvplay->AvplayBlEsBuf.pu8Addr, u32ValidDataLen);

        Ret = HI_MPI_VDEC_ChanPutBuffer(pAvplay->hVdec, &pAvplay->AvplayBlEsBuf);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanPutBuffer failed 0x%x\n", Ret);
            return Ret;
        }

        Ret = HI_MPI_VDEC_ChanPutBuffer(pAvplay->hElVdec, &pAvplay->AvplayElEsBuf);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_ChanPutBuffer failed 0x%x\n", Ret);
            return Ret;
        }
    }
#endif

    return HI_SUCCESS;
}


HI_S32 AVPLAY_PutAudBuf(AVPLAY_S                     *pAvplay,
                        HI_UNF_AVPLAY_BUFID_E         enBufId,
                        HI_U32                        u32ValidDataLen,
                        HI_U32                        u32Pts)
{
    HI_S32      Ret;

    if (HI_UNF_AVPLAY_BUF_ID_ES_AUD == enBufId)
    {
        if (!pAvplay->AudEnable)
        {
            HI_ERR_AVPLAY("aud chn is stopped.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        pAvplay->AvplayAudEsBuf.u32Size = u32ValidDataLen;
        Ret = HI_MPI_ADEC_PutBuffer(pAvplay->hAdec, &pAvplay->AvplayAudEsBuf, u32Pts);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_ADEC_PutBuffer failed 0x%x\n", Ret);
            return Ret;
        }
    }

    if (HI_UNF_AVPLAY_BUF_ID_ES_AUD_AD == enBufId)
    {
        if (!pAvplay->AdAttr.bAdEnble)
        {
            HI_WARN_AVPLAY("aud chn not support AD.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        if (HI_INVALID_HANDLE == pAvplay->hAdec)
        {
            HI_WARN_AVPLAY("aud chn not open.\n");
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        pAvplay->AvplayAdAudEsBuf.u32Size = u32ValidDataLen;
        Ret = HI_MPI_ADEC_PutADBuffer(pAvplay->hAdec, &pAvplay->AvplayAdAudEsBuf, u32Pts);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_ADEC_PutADBuffer failed 0x%x\n", Ret);
            return Ret;
        }
    }

    return HI_SUCCESS;
}


HI_S32 HI_MPI_AVPLAY_Init(HI_VOID)
{
    HI_S32 Ret;

    HI_AVPLAY_LOCK();

    if (0 == g_AvplayInit)
    {
        Ret = HI_MODULE_Register(HI_ID_AVPLAY, "HI_AVPLAY");
        if (HI_SUCCESS != Ret)
        {
            HI_AVPLAY_UNLOCK();

            HI_ERR_AVPLAY("HI_MODULE_Register failed 0x%x\n", Ret);

            return Ret;
        }

        Ret = HI_MPI_ADEC_Init(HI_NULL);
        if (HI_SUCCESS != Ret)
        {
            (HI_VOID)HI_MODULE_UnRegister(HI_ID_AVPLAY);
            HI_AVPLAY_UNLOCK();

            HI_ERR_AVPLAY("HI_MPI_ADEC_Init failed 0x%x\n", Ret);

            return Ret;
        }

        Ret = HI_MPI_VDEC_Init();
        if (HI_SUCCESS != Ret)
        {
            (HI_VOID)HI_MPI_ADEC_deInit();
            (HI_VOID)HI_MODULE_UnRegister(HI_ID_AVPLAY);
            HI_AVPLAY_UNLOCK();

            HI_ERR_AVPLAY("HI_MPI_VDEC_Init failed 0x%x\n", Ret);

            return Ret;
        }

        Ret = HI_MPI_SYNC_Init();
        if (HI_SUCCESS != Ret)
        {
            (HI_VOID)HI_MPI_VDEC_DeInit();
            (HI_VOID)HI_MPI_ADEC_deInit();
            (HI_VOID)HI_MODULE_UnRegister(HI_ID_AVPLAY);
            HI_AVPLAY_UNLOCK();

            HI_FATAL_AVPLAY("HI_MPI_SYNC_Init failed 0x%x\n", Ret);

            return Ret;
        }

        Ret = HI_UNF_PMOC_Init();
        if (HI_SUCCESS != Ret)
        {
            (HI_VOID)HI_MPI_SYNC_DeInit();
            (HI_VOID)HI_MPI_VDEC_DeInit();
            (HI_VOID)HI_MPI_ADEC_deInit();
            (HI_VOID)HI_MODULE_UnRegister(HI_ID_AVPLAY);
            HI_AVPLAY_UNLOCK();

            HI_FATAL_AVPLAY("HI_UNF_PMOC_Init failed 0x%x\n", Ret);

            return Ret;
        }

        g_AvplayInit = 1;
    }

    HI_AVPLAY_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_DeInit(HI_VOID)
{
    HI_S32  Ret;
    HI_U32  i;
    HI_U32  AvplayNum = 0;

    for (i = 0; i < AVPLAY_MAX_NUM; i++)
    {
        AVPLAY_INST_LOCK(i);
        if (g_Avplay[i].Avplay)
        {
            AvplayNum++;
        }
        AVPLAY_INST_UNLOCK(i);
    }

    if (AvplayNum)
    {
        HI_ERR_AVPLAY("there are %d AVPLAY not been destroied\n", AvplayNum);

        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    HI_AVPLAY_LOCK();

    g_AvplayInit = 0;

    Ret = HI_UNF_PMOC_DeInit();
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("HI_UNF_PMOC_DeInit failed 0x%x\n", Ret);
    }

    Ret = HI_MPI_SYNC_DeInit();
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("HI_MPI_SYNC_DeInit failed 0x%x\n", Ret);
    }

    Ret = HI_MPI_VDEC_DeInit();
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_DeInit failed 0x%x\n", Ret);
    }

    Ret = HI_MPI_ADEC_deInit();
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("HI_MPI_ADEC_deInit failed 0x%x\n", Ret);
    }

    Ret = HI_MODULE_UnRegister(HI_ID_AVPLAY);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("HI_MODULE_UnRegister failed 0x%x\n", Ret);
    }

    HI_AVPLAY_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_GetDefaultConfig(HI_UNF_AVPLAY_ATTR_S *pstAvAttr, HI_UNF_AVPLAY_STREAM_TYPE_E enCfg)
{
    HI_S32              ret;
    HI_SYS_VERSION_S    SysVersion;
    HI_CHIP_TYPE_E      ChipType    = HI_CHIP_TYPE_HI3716C;
    HI_CHIP_VERSION_E   ChipVersion = HI_CHIP_VERSION_V200;

    if (!pstAvAttr)
    {
        HI_ERR_AVPLAY("para pstAvAttr is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    if ((HI_UNF_AVPLAY_STREAM_TYPE_TS != enCfg) && (HI_UNF_AVPLAY_STREAM_TYPE_ES != enCfg))
    {
        HI_ERR_AVPLAY("para enCfg is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    pstAvAttr->u32DemuxId = 0;
    pstAvAttr->stStreamAttr.enStreamType = enCfg;

    memset(&SysVersion, 0, sizeof(HI_SYS_VERSION_S));
    ret = HI_SYS_GetVersion(&SysVersion);
    if (HI_SUCCESS == ret)
    {
        ChipType    = SysVersion.enChipTypeHardWare;
        ChipVersion = SysVersion.enChipVersion;
    }

    if (   ((HI_CHIP_TYPE_HI3798M == ChipType) && (HI_CHIP_VERSION_V100 == ChipVersion))
        || ((HI_CHIP_TYPE_HI3796M == ChipType) && (HI_CHIP_VERSION_V100 == ChipVersion))
        || ((HI_CHIP_TYPE_HI3798C == ChipType) && (HI_CHIP_VERSION_V200 == ChipVersion))
        || ((HI_CHIP_TYPE_HI3798M == ChipType) && (HI_CHIP_VERSION_V200 == ChipVersion)))
    {
        pstAvAttr->stStreamAttr.u32VidBufSize = 16 * 1024 * 1024;
        pstAvAttr->stStreamAttr.u32AudBufSize = 768 * 1024;
    }
    else
    {
        pstAvAttr->stStreamAttr.u32VidBufSize = 6 * 1024 * 1024;
        pstAvAttr->stStreamAttr.u32AudBufSize = 384 * 1024;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_Create(const HI_UNF_AVPLAY_ATTR_S *pstAvAttr, HI_HANDLE *phAvplay)
{
    HI_S32                  Ret;
    HI_SIZE_T               Id;
    AVPLAY_S               *pAvplay = HI_NULL;
    HI_UNF_SYNC_ATTR_S      SyncAttr;
#ifndef HI_ADVCA_FUNCTION_RELEASE
    HI_CHAR                 ProcName[32];
#endif
    HI_U32                  i;

    if (!pstAvAttr)
    {
        HI_ERR_AVPLAY("para pstAvAttr is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    if (!phAvplay)
    {
        HI_ERR_AVPLAY("para phAvplay is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    if (pstAvAttr->stStreamAttr.enStreamType >= HI_UNF_AVPLAY_STREAM_TYPE_BUTT)
    {
        HI_ERR_AVPLAY("para pstAvAttr->stStreamAttr.enStreamType is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (pstAvAttr->stStreamAttr.u32VidBufSize > AVPLAY_MAX_VID_SIZE)
    {
        HI_ERR_AVPLAY("para pstAvAttr->stStreamAttr.u32VidBufSize is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if ((pstAvAttr->stStreamAttr.u32AudBufSize > AVPLAY_MAX_AUD_SIZE)
      ||(pstAvAttr->stStreamAttr.u32AudBufSize < AVPLAY_MIN_AUD_SIZE)
       )
    {
        HI_ERR_AVPLAY("para pstAvAttr->stStreamAttr.u32AudBufSize is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    HI_AVPLAY_LOCK();

    if (0 == g_AvplayInit)
    {
        HI_AVPLAY_UNLOCK();
        HI_ERR_AVPLAY("AVPLAY is not init\n");
        return HI_ERR_AVPLAY_DEV_NO_INIT;
    }

    for (Id = 0; Id < AVPLAY_MAX_NUM; Id++)
    {
        AVPLAY_INST_LOCK(Id);
        if (HI_NULL == g_Avplay[Id].Avplay)
        {
            g_Avplay[Id].Avplay = (AVPLAY_S*)HI_MALLOC(HI_ID_AVPLAY, sizeof(AVPLAY_S));
            if (HI_NULL == g_Avplay[Id].Avplay)
            {
                AVPLAY_INST_UNLOCK(Id);
                HI_AVPLAY_UNLOCK();

                HI_ERR_AVPLAY("avplay malloc failed\n");

                return HI_ERR_AVPLAY_CREATE_ERR;
            }

            memset(g_Avplay[Id].Avplay, 0, sizeof(AVPLAY_S));

            AVPLAY_INST_UNLOCK(Id);

            break;
        }
        AVPLAY_INST_UNLOCK(Id);
    }

    HI_AVPLAY_UNLOCK();

    if (Id >= AVPLAY_MAX_NUM)
    {
        HI_ERR_AVPLAY("no free avplay\n");

        return HI_ERR_AVPLAY_CREATE_ERR;
    }

    AVPLAY_INST_LOCK(Id);

    pAvplay = g_Avplay[Id].Avplay;

    pAvplay->hAvplay = GET_AVPLAY_HANDLE(Id);

    memcpy(&pAvplay->AvplayAttr, pstAvAttr, sizeof(HI_UNF_AVPLAY_ATTR_S));

    pAvplay->VdecAttr.enType        = HI_UNF_VCODEC_TYPE_BUTT;
    pAvplay->VdecAttr.enMode        = HI_UNF_VCODEC_MODE_NORMAL;
    pAvplay->VdecAttr.u32ErrCover   = 0;
    pAvplay->VdecAttr.u32Priority   = 0;

    pAvplay->LowDelayAttr.bEnable   = HI_FALSE;

#ifdef HI_TEE_SUPPORT
    pAvplay->TVPAttr.bEnable = HI_FALSE;
#endif

    pAvplay->AdecType   = 0xffffffff;

    pAvplay->hVdec = HI_INVALID_HANDLE;
    pAvplay->hAdec = HI_INVALID_HANDLE;
    pAvplay->hSync = HI_INVALID_HANDLE;

    pAvplay->hDmxPcr    = HI_INVALID_HANDLE;
    pAvplay->DmxPcrPid  = DEMUX_INVALID_PID;

    pAvplay->hDmxVid    = HI_INVALID_HANDLE;
    pAvplay->DmxVidPid  = DEMUX_INVALID_PID;

    for (i = 0; i < AVPLAY_MAX_DMX_AUD_CHAN_NUM; i++)
    {
        pAvplay->hDmxAud[i]     = HI_INVALID_HANDLE;
        pAvplay->DmxAudPid[i]   = DEMUX_INVALID_PID;
    }

    pAvplay->bStepMode = HI_FALSE;
    pAvplay->bStepPlay = HI_FALSE;

    pAvplay->hSharedOrgWin = HI_INVALID_HANDLE;

    pAvplay->MasterFrmChn.hPort     = HI_INVALID_HANDLE;
    pAvplay->MasterFrmChn.hWindow   = HI_INVALID_HANDLE;

    for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
    {
        pAvplay->SlaveFrmChn[i].hPort   = HI_INVALID_HANDLE;
        pAvplay->SlaveFrmChn[i].hWindow = HI_INVALID_HANDLE;
    }

    for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
    {
        pAvplay->VirFrmChn[i].hPort     = HI_INVALID_HANDLE;
        pAvplay->VirFrmChn[i].hWindow   = HI_INVALID_HANDLE;
    }

#ifdef HI_HDR_SUPPORT
    pAvplay->bHDRPlay          = HI_FALSE;
    pAvplay->bDualChnPlay      = HI_FALSE;
    pAvplay->hBlPort           = HI_INVALID_HANDLE;
    pAvplay->hElPort           = HI_INVALID_HANDLE;
    pAvplay->hElVdec           = HI_INVALID_HANDLE;
    pAvplay->HDRSyncTargetTime = HI_INVALID_PTS;
    pAvplay->HDRSyncTargetInit = HI_FALSE;
    pAvplay->HDRSyncTarget     = AVPLAY_HDR_CHAN_BUTT;
    pAvplay->BlSyncProc        = AVPLAY_HDR_SYNC_PROC_CONTINUE;
    pAvplay->ElSyncProc        = AVPLAY_HDR_SYNC_PROC_CONTINUE;
#endif

    AVPLAY_FrcCreate(&pAvplay->FrcCalAlg, &pAvplay->FrcParamCfg, &pAvplay->FrcCtrlInfo);

    pAvplay->bFrcEnable = HI_TRUE;

    for (i = 0; i < AVPLAY_MAX_TRACK; i++)
    {
        pAvplay->hTrack[i] = HI_INVALID_HANDLE;
    }

    pAvplay->TrackNum   = 0;
    pAvplay->hSyncTrack = HI_INVALID_HANDLE;

    pAvplay->AudDDPMode = HI_FALSE; /* for DDP test only */
    pAvplay->LastAudPts = 0;        /* for DDP test only */

    pAvplay->VidEnable = HI_FALSE;
    pAvplay->AudEnable = HI_FALSE;

    pAvplay->bVidPreEnable = HI_FALSE;
    pAvplay->bAudPreEnable = HI_FALSE;
    pAvplay->PreVidWaterLine = 0;
    pAvplay->PreAudWaterLine = AVPLAY_AUD_PREBUF_THRESHOLD;
    pAvplay->PreVidFirstWaterLine = 0;
    pAvplay->PreVidSecondWaterLine = 0;
    pAvplay->PreAudPts = HI_INVALID_PTS;
    pAvplay->PreVidPts = HI_INVALID_PTS;
    pAvplay->hDFCtx = HI_INVALID_HANDLE;

    pAvplay->LstStatus = HI_UNF_AVPLAY_STATUS_STOP;
    pAvplay->CurStatus = HI_UNF_AVPLAY_STATUS_STOP;
    pAvplay->OverflowProc = HI_UNF_AVPLAY_OVERFLOW_RESET;

    pAvplay->AdAttr.u32AdPid = 0x1fff;
    pAvplay->AdAttr.bAdEnble = HI_FALSE;      //AD enable default param
    pAvplay->AdAttr.bAdToSomePort = HI_FALSE;
    pAvplay->AdAttr.s16AdBalance = 0;
    pAvplay->hDmxAdAud = HI_INVALID_HANDLE;

    pAvplay->CurBufferEmptyState = HI_FALSE;

    /*  initialize standby count */
    pAvplay->u32ResumeCount = 0;
    pAvplay->bSetResumeCnt = HI_FALSE;

    AVPLAY_ResetProcFlag(pAvplay);

    /*
    * These members should be 0 when create avplay called,
    * to avoid AVPLAY_IsBufEmpty calculate a wrong state.
    * eg. call AVPLAY_IsBufEmpty immediately after create avplay, wanted empty is true.
    * and call AVPLAY_IsBufEmpty immediately after start vid channel or aud channel, wanted empty is true.
    */
    pAvplay->PreTscnt =0;
    pAvplay->PreAudEsBuf = 0;
    pAvplay->PreAudEsBufWPtr = 0;
    pAvplay->PreVidEsBuf = 0;
    pAvplay->PreVidEsBufWPtr = 0;

    pAvplay->EvtCbFunc64Flag = HI_FALSE;

    pAvplay->AdecOutBufNum = AVPLAY_ADEC_FRAME_NUM;

    /* initialize events callback function*/
    for (i = 0; i < HI_UNF_AVPLAY_EVENT_BUTT; i++)
    {
    #ifndef __aarch64__
        pAvplay->EvtCbFunc[i] = HI_NULL;
    #endif
        pAvplay->EvtCbFunc64[i] = HI_NULL;
    }

    /* Initalize CPU Freq boost function*/
    pAvplay->bBoostCpuFreq = HI_FALSE;

    /* If the signal signum is CTRL+C, AVPLAY must release received vid frame*/
    signal(SIGINT, AVPLAY_ProcSignal);

    pAvplay->VidFirstFrameDispTime  = 0xFFFFFFFF;
    pAvplay->VidFirstQueueFrameTime = 0xFFFFFFFF;
    pAvplay->bVidFirstQueueFrame    = HI_FALSE;
    pAvplay->bVidFirstDispFrame     = HI_FALSE;

    pAvplay->DmxAudChnSecMode = HI_UNF_DMX_SECURE_MODE_NONE;

    HI_MPI_SYNC_GetDefaultAttr(&SyncAttr);

    Ret = HI_MPI_SYNC_Create(&SyncAttr, &pAvplay->hSync);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("AVPLAY create sync failed 0x%x\n", Ret);
        goto AVPLAY_DESTROY;
    }

    Ret = AVPLAY_CreateThread(pAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("AVPLAY create thread failed 0x%x\n", Ret);
        goto SYNC_DESTROY;
    }

#ifndef HI_ADVCA_FUNCTION_RELEASE
    snprintf(ProcName, sizeof(ProcName), "avplay%02d", GET_SYNC_ID(pAvplay->hSync));

    pAvplay->Proc.pszEntryName = ProcName;
    pAvplay->Proc.pszDirectory = "msp";
    pAvplay->Proc.pfnShowProc  = AVPLAY_ProcRead;
    pAvplay->Proc.pfnCmdProc   = AVPLAY_ProcWrite;
    pAvplay->Proc.pPrivData    = (HI_VOID*)Id;

    Ret = HI_PROC_AddEntry(HI_ID_AVPLAY, &pAvplay->Proc);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_PROC_AddEntry failed 0x%x\n", Ret);

        pAvplay->Proc.pfnShowProc   = HI_NULL;
        pAvplay->Proc.pfnCmdProc    = HI_NULL;
    }
#endif

    *phAvplay = pAvplay->hAvplay;

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;

SYNC_DESTROY:
    (HI_VOID)HI_MPI_SYNC_Destroy(pAvplay->hSync);

AVPLAY_DESTROY:
    HI_FREE(HI_ID_AVPLAY, g_Avplay[Id].Avplay);
    g_Avplay[Id].Avplay = HI_NULL;

    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_Destroy(HI_HANDLE hAvplay)
{
    HI_S32      Ret __attribute__((unused)) = HI_FAILURE;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;
    HI_U32      VirChnNum;
    HI_U32      SlaveChnNum;

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if ((pAvplay->hVdec != HI_INVALID_HANDLE) || (pAvplay->hAdec != HI_INVALID_HANDLE))
    {
        AVPLAY_INST_UNLOCK(Id);

        HI_ERR_AVPLAY("vid or aud chn is not closed\n");

        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    VirChnNum = AVPLAY_GetVirtualWinChnNum(pAvplay);
    SlaveChnNum = AVPLAY_GetSlaveWinChnNum(pAvplay);

    if ((HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow) || (0 != SlaveChnNum) || (0 != VirChnNum))
    {
        AVPLAY_INST_UNLOCK(Id);

        HI_ERR_AVPLAY("win is not detach\n");

        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (0 != pAvplay->TrackNum)
    {
        AVPLAY_INST_UNLOCK(Id);

        HI_ERR_AVPLAY("snd is not detach\n");

        return HI_ERR_AVPLAY_INVALID_OPT;
    }

#ifndef HI_ADVCA_FUNCTION_RELEASE
    if (pAvplay->Proc.pfnShowProc || pAvplay->Proc.pfnCmdProc)
    {
        HI_CHAR ProcName[32];

        snprintf(ProcName, sizeof(ProcName), "avplay%02d", GET_SYNC_ID(pAvplay->hSync));

        pAvplay->Proc.pszEntryName = ProcName;

        Ret = HI_PROC_RemoveEntry(HI_ID_AVPLAY, &pAvplay->Proc);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_PROC_RemoveEntry failed 0x%x\n", Ret);
        }
    }
#endif

    AVPLAY_DestroyThread(pAvplay);

    (HI_VOID)HI_MPI_SYNC_Destroy(pAvplay->hSync);

    AVPLAY_FrcDestroy(&pAvplay->FrcCalAlg);
    pAvplay->bFrcEnable = HI_FALSE;

    if (HI_NULL != pAvplay->pstAcodecAttr)
    {
        HI_FREE(HI_ID_AVPLAY, (HI_VOID*)(pAvplay->pstAcodecAttr));
        pAvplay->pstAcodecAttr = HI_NULL;
    }

    HI_FREE(HI_ID_AVPLAY, g_Avplay[Id].Avplay);
    g_Avplay[Id].Avplay = HI_NULL;

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_ChnOpen(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_VOID *pPara)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    switch ((HI_U32)enChn)
    {
        case HI_UNF_AVPLAY_MEDIA_CHAN_AUD :
        case HI_UNF_AVPLAY_MEDIA_CHAN_VID :
        case HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID :
            break;

        default :
            HI_ERR_AVPLAY("para enChn 0x%x is invalid\n", enChn);
            return HI_ERR_AVPLAY_INVALID_PARA;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID))
    {
        if (HI_INVALID_HANDLE == pAvplay->hVdec)
        {
            Ret = AVPLAY_MallocVidChn(pAvplay, pPara);
            if (Ret != HI_SUCCESS)
            {
                AVPLAY_INST_UNLOCK(Id);

                HI_ERR_AVPLAY("Avplay malloc vid chn failed 0x%x\n", Ret);

                return Ret;
            }
        }
    }

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_AUD))
    {
        if (HI_INVALID_HANDLE == pAvplay->hAdec)
        {
            Ret = AVPLAY_MallocAudChn(pAvplay);
            if (Ret != HI_SUCCESS)
            {
                if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID))
                {
                    (HI_VOID)AVPLAY_FreeVidChn(pAvplay);
                }

                AVPLAY_INST_UNLOCK(Id);

                HI_ERR_AVPLAY("Avplay malloc aud chn failed 0x%x\n", Ret);

                return Ret;
            }
        }
    }

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_ChnClose(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;
    HI_U32      VirChnNum;
    HI_U32      SlaveChnNum;

    switch ((HI_U32)enChn)
    {
        case HI_UNF_AVPLAY_MEDIA_CHAN_AUD :
        case HI_UNF_AVPLAY_MEDIA_CHAN_VID :
        case HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID :
            break;

        default :
            HI_ERR_AVPLAY("para enChn 0x%x is invalid\n", enChn);
            return HI_ERR_AVPLAY_INVALID_PARA;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID))
    {
        if (pAvplay->VidEnable)
        {
            AVPLAY_INST_UNLOCK(Id);

            HI_ERR_AVPLAY("vid chn is enable, can not colsed\n");

            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        VirChnNum = AVPLAY_GetVirtualWinChnNum(pAvplay);
        SlaveChnNum = AVPLAY_GetSlaveWinChnNum(pAvplay);

        if ((HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow) || (0 != SlaveChnNum) || (0 != VirChnNum))
        {
            AVPLAY_INST_UNLOCK(Id);

            HI_ERR_AVPLAY("window is attach to vdec, can not colsed\n");

            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        if (pAvplay->hVdec != HI_INVALID_HANDLE)
        {
#ifdef HI_HDR_SUPPORT
            if (pAvplay->bHDRPlay)
            {
                Ret = AVPLAY_FreeHDRChn(pAvplay, AVPLAY_HDR_CHAN_BL | AVPLAY_HDR_CHAN_EL);
                if (Ret != HI_SUCCESS)
                {
                    AVPLAY_INST_UNLOCK(Id);

                    HI_ERR_AVPLAY("Avplay free vid chn failed 0x%x\n", Ret);

                    return Ret;
                }
            }
            else
#endif
            {
                Ret = AVPLAY_FreeVidChn(pAvplay);
                if (Ret != HI_SUCCESS)
                {
                    AVPLAY_INST_UNLOCK(Id);

                    HI_ERR_AVPLAY("Avplay free vid chn failed 0x%x\n", Ret);

                    return Ret;
                }
            }
        }
    }

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_AUD))
    {
        if (pAvplay->AudEnable)
        {
            AVPLAY_INST_UNLOCK(Id);

            HI_ERR_AVPLAY("aud chn is enable, can not colsed\n");

            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        if (pAvplay->TrackNum)
        {
            AVPLAY_INST_UNLOCK(Id);

            HI_ERR_AVPLAY("track is attach to adec, can not colsed\n");

            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        if (pAvplay->hAdec != HI_INVALID_HANDLE)
        {
            Ret = AVPLAY_FreeAudChn(pAvplay);
            if (Ret != HI_SUCCESS)
            {
                AVPLAY_INST_UNLOCK(Id);

                HI_ERR_AVPLAY("Avplay free aud chn failed 0x%x\n", Ret);

                return Ret;
            }
        }
    }

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 AVPLAY_SetFrmPackingType(AVPLAY_S *pAvplay, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E *pFrmPackingType)
{
    HI_S32 Ret;

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_AVPLAY("vid chn is close, can not set frm packing type.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    /* input param check */
    if (*pFrmPackingType >= HI_UNF_FRAME_PACKING_TYPE_BUTT)
    {
        HI_ERR_AVPLAY("FrmPackingType is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    Ret = HI_MPI_VDEC_SetChanFrmPackType(pAvplay->hVdec, pFrmPackingType);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_SetChanFrmPackType failed 0x%x\n", Ret);
        return Ret;
    }
    return HI_SUCCESS;
}

HI_S32 AVPLAY_GetFrmPackingType(AVPLAY_S *pAvplay, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E *pFrmPackingType)
{
    HI_S32 Ret;

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_AVPLAY("vid chn is close, can not get frm packing type.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_VDEC_GetChanFrmPackType(pAvplay->hVdec, pFrmPackingType);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_GetChanFrmPackType failed 0x%x\n", Ret);
        return Ret;
    }
    return HI_SUCCESS;
}

HI_S32 AVPLAY_SetVdecFrmRateParam(AVPLAY_S *pAvplay,  HI_UNF_AVPLAY_FRMRATE_PARAM_S *pFrmRate)
{
    HI_S32 Ret;

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_AVPLAY("vid chn is close, can not set vdec frm rate.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    /* input param check */
    if (pFrmRate->enFrmRateType >= HI_UNF_AVPLAY_FRMRATE_TYPE_BUTT)
    {
        HI_ERR_AVPLAY("enFrmRateType is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    /* input param check */
    if ((HI_UNF_AVPLAY_FRMRATE_TYPE_USER == pFrmRate->enFrmRateType)
        || (HI_UNF_AVPLAY_FRMRATE_TYPE_USER_PTS == pFrmRate->enFrmRateType)
        )
    {
        if ((pFrmRate->stSetFrmRate.u32fpsInteger == 0)
            && (pFrmRate->stSetFrmRate.u32fpsDecimal == 0)
            )
        {
            HI_ERR_AVPLAY("stSetFrmRate is invalid.\n");
            return HI_ERR_AVPLAY_INVALID_PARA;
        }
    }

    Ret = HI_MPI_VDEC_SetChanFrmRate(pAvplay->hVdec, pFrmRate);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_SetChanFrmRate failed 0x%x\n", Ret);
        return Ret;
    }
    return HI_SUCCESS;
}

HI_S32 AVPLAY_GetVdecFrmRateParam(AVPLAY_S *pAvplay,  HI_UNF_AVPLAY_FRMRATE_PARAM_S *pFrmRate)
{
    HI_S32  Ret;

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_AVPLAY("vid chn is close, can not set vdec frm rate.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }
    Ret = HI_MPI_VDEC_GetChanFrmRate(pAvplay->hVdec, pFrmRate);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("call HI_MPI_VDEC_GetChanFrmRate failed.\n");
        return Ret;
    }
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_SetAttr(HI_HANDLE hAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!pPara)
    {
        HI_ERR_AVPLAY("para pPara is null\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    switch (enAttrID)
    {
        case HI_UNF_AVPLAY_ATTR_ID_STREAM_MODE:
            Ret = AVPLAY_SetStreamMode(pAvplay, (HI_UNF_AVPLAY_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_ADEC:
            Ret = AVPLAY_SetAdecAttr(pAvplay, (HI_UNF_ACODEC_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_VDEC:
            Ret = AVPLAY_SetVdecAttr(pAvplay, (HI_UNF_VCODEC_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_AUD_PID:
            Ret = AVPLAY_SetPid(pAvplay, enAttrID, *((HI_U32*)pPara));
            break;

        case HI_UNF_AVPLAY_ATTR_ID_VID_PID:
            Ret = AVPLAY_SetPid(pAvplay, enAttrID, *((HI_U32*)pPara));
            break;

        case HI_UNF_AVPLAY_ATTR_ID_PCR_PID:
            Ret = AVPLAY_SetPid(pAvplay, enAttrID, *((HI_U32*)pPara));
            break;

        case HI_UNF_AVPLAY_ATTR_ID_SYNC:
            Ret = AVPLAY_SetSyncAttr(pAvplay, (HI_UNF_SYNC_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_AFD:
            Ret = HI_ERR_AVPLAY_NOT_SUPPORT;
            break;

        case HI_UNF_AVPLAY_ATTR_ID_OVERFLOW:
            Ret = AVPLAY_SetOverflowProc(pAvplay, (HI_UNF_AVPLAY_OVERFLOW_E *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_MULTIAUD:
            Ret = AVPLAY_SetMultiAud(pAvplay, (HI_UNF_AVPLAY_MULTIAUD_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_FRMRATE_PARAM:
            Ret = AVPLAY_SetVdecFrmRateParam(pAvplay, (HI_UNF_AVPLAY_FRMRATE_PARAM_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_FRMPACK_TYPE:
            Ret = AVPLAY_SetFrmPackingType(pAvplay, (HI_UNF_VIDEO_FRAME_PACKING_TYPE_E *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_LOW_DELAY:
            Ret = AVPLAY_SetLowDelay(pAvplay, (HI_UNF_AVPLAY_LOW_DELAY_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_TVP:
        #ifdef HI_TEE_SUPPORT
            Ret = AVPLAY_SetTVP(pAvplay, (HI_UNF_AVPLAY_TVP_ATTR_S *)pPara);
        #else
            Ret = HI_ERR_AVPLAY_NOT_SUPPORT;
        #endif
            break;

        case HI_UNF_AVPLAY_ATTR_ID_AD:
            Ret = AVPLAY_SetAdAttr(pAvplay, (HI_UNF_AVPLAY_AD_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_HDR:
        #ifdef HI_HDR_SUPPORT
            Ret = AVPLAY_SetHDR(pAvplay, (HI_UNF_AVPLAY_HDR_ATTR_S *)pPara);
        #else
            Ret = HI_ERR_AVPLAY_NOT_SUPPORT;
        #endif
            break;

        case HI_UNF_AVPLAY_ATTR_ID_AUD_SECURITY:
            Ret = AVPLAY_SetDmxAudChnMode(pAvplay, (HI_UNF_AVPLAY_AUD_SECURITY_ATTR_S *)pPara);
            break;

        default:
            Ret = HI_ERR_AVPLAY_INVALID_PARA;
            HI_ERR_AVPLAY("para enAttrID 0x%x is invalid\n", enAttrID);
            break;
    }

    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_GetAttr(HI_HANDLE hAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!pPara)
    {
        HI_ERR_AVPLAY("para pPara is null\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    switch (enAttrID)
    {
        case HI_UNF_AVPLAY_ATTR_ID_STREAM_MODE:
            Ret = AVPLAY_GetStreamMode(pAvplay, (HI_UNF_AVPLAY_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_ADEC:
            Ret = AVPLAY_GetAdecAttr(pAvplay, (HI_UNF_ACODEC_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_VDEC:
            Ret = AVPLAY_GetVdecAttr(pAvplay, (HI_UNF_VCODEC_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_AUD_PID:
            Ret = AVPLAY_GetPid(pAvplay, enAttrID, (HI_U32 *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_VID_PID:
            Ret = AVPLAY_GetPid(pAvplay, enAttrID, (HI_U32 *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_PCR_PID:
            Ret = AVPLAY_GetPid(pAvplay, enAttrID, (HI_U32 *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_SYNC:
            Ret = AVPLAY_GetSyncAttr(pAvplay, (HI_UNF_SYNC_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_AFD:
            Ret = HI_ERR_AVPLAY_NOT_SUPPORT;
            break;

        case HI_UNF_AVPLAY_ATTR_ID_OVERFLOW:
            Ret = AVPLAY_GetOverflowProc(pAvplay, (HI_UNF_AVPLAY_OVERFLOW_E *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_MULTIAUD:
            Ret = AVPLAY_GetMultiAud(pAvplay, (HI_UNF_AVPLAY_MULTIAUD_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_FRMRATE_PARAM:
            Ret = AVPLAY_GetVdecFrmRateParam(pAvplay, (HI_UNF_AVPLAY_FRMRATE_PARAM_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_FRMPACK_TYPE:
            Ret = AVPLAY_GetFrmPackingType(pAvplay, (HI_UNF_VIDEO_FRAME_PACKING_TYPE_E *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_LOW_DELAY:
            Ret = AVPLAY_GetLowDelay(pAvplay, (HI_UNF_AVPLAY_LOW_DELAY_ATTR_S *)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_TVP:
        #ifdef HI_TEE_SUPPORT
            Ret = AVPLAY_GetTVP(pAvplay, (HI_UNF_AVPLAY_TVP_ATTR_S *)pPara);
        #else
            Ret = HI_ERR_AVPLAY_NOT_SUPPORT;
        #endif
            break;

        case HI_UNF_AVPLAY_ATTR_ID_AD:
            Ret = AVPLAY_GetAdAttr(pAvplay, (HI_UNF_AVPLAY_AD_ATTR_S*)pPara);
            break;

        case HI_UNF_AVPLAY_ATTR_ID_HDR:
        #ifdef HI_HDR_SUPPORT
            Ret = AVPLAY_GetHDR(pAvplay, (HI_UNF_AVPLAY_HDR_ATTR_S *)pPara);
        #else
            Ret = HI_ERR_AVPLAY_NOT_SUPPORT;
        #endif
            break;

        case HI_UNF_AVPLAY_ATTR_ID_AUD_SECURITY:
            Ret = AVPLAY_GetDmxAudChnMode(pAvplay, (HI_UNF_AVPLAY_AUD_SECURITY_ATTR_S*)pPara);
            break;

        default:
            Ret = HI_ERR_AVPLAY_INVALID_PARA;
            HI_ERR_AVPLAY("para enAttrID 0x%x is invalid\n", enAttrID);
            break;
    }

    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_DecodeIFrame(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_I_FRAME_S *pstIframe,
                                              HI_UNF_VIDEO_FRAME_INFO_S *pstCapPicture)
{
    HI_S32                          Ret;
    HI_U32                          Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S                       *pAvplay;
    HI_U32                          i, j;
    HI_HANDLE                       hWindow = HI_INVALID_HANDLE;
    HI_DRV_VIDEO_FRAME_PACKAGE_S    FrmPack = {0};
    HI_BOOL                         bCapture = HI_FALSE;
    HI_DRV_VPSS_PORT_CFG_S          VpssPortOldCfg;
    HI_DRV_VPSS_PORT_CFG_S          VpssPortNewCfg;

    if (HI_NULL == pstIframe)
    {
        HI_ERR_AVPLAY("para pstIframe is null\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    memset(&FrmPack, 0x0, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        AVPLAY_INST_UNLOCK(Id);

        HI_ERR_AVPLAY("hVdec is invalid\n");

        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_TRUE == pAvplay->VidEnable)
    {
        AVPLAY_INST_UNLOCK(Id);

        HI_ERR_AVPLAY("vid chn is opened\n");

        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (pAvplay->stIFrame.hport != HI_INVALID_HANDLE)
    {
        AVPLAY_INST_UNLOCK(Id);

        HI_ERR_AVPLAY("please release I frame first\n");

        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    /* if there is no window exist, we need create vpss source */
    if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
    {
        if (HI_NULL == pstCapPicture)
        {
            AVPLAY_INST_UNLOCK(Id);

            HI_ERR_AVPLAY("there is no window\n");

            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        Ret = HI_MPI_VDEC_CreatePort(pAvplay->hVdec, &pAvplay->MasterFrmChn.hPort, VDEC_PORT_HD);
        if (HI_SUCCESS != Ret)
        {
            AVPLAY_INST_UNLOCK(Id);

            HI_ERR_AVPLAY("HI_MPI_VDEC_CreatePort ERR, Ret=%#x\n", Ret);

            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        Ret = HI_MPI_VDEC_SetPortType(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort, VDEC_PORT_TYPE_MASTER);
        Ret |= HI_MPI_VDEC_EnablePort(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort);
        if (HI_SUCCESS != Ret)
        {
            (HI_VOID)HI_MPI_VDEC_DestroyPort(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort);

            AVPLAY_INST_UNLOCK(Id);

            HI_ERR_AVPLAY("HI_MPI_VDEC_EnablePort ERR, Ret=%#x\n", Ret);

            return HI_ERR_AVPLAY_INVALID_OPT;
        }
    }

    if (HI_NULL != pstCapPicture)
    {
        bCapture = HI_TRUE;
    }

    Ret = HI_MPI_VDEC_GetPortAttr(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort, &VpssPortOldCfg);
    if (HI_SUCCESS != Ret)
    {
        AVPLAY_INST_UNLOCK(Id);

        HI_ERR_AVPLAY("HI_MPI_VDEC_GetPortAttr ERR, Ret=%#x\n", Ret);

        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    VpssPortNewCfg = VpssPortOldCfg;
    // need capture frame, config vpss to do not zoom
    if (HI_TRUE == bCapture)
    {
        VpssPortNewCfg.s32OutputWidth = 0;
        VpssPortNewCfg.s32OutputHeight = 0;
        VpssPortNewCfg.eFormat = HI_DRV_PIX_FMT_NV21;
    }

    /*set vpss attr, do not do zoom*/
    Ret = HI_MPI_VDEC_SetPortAttr(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort, &VpssPortNewCfg);
    if (HI_SUCCESS != Ret)
    {
        AVPLAY_INST_UNLOCK(Id);

        HI_ERR_AVPLAY("HI_MPI_VDEC_SetPortAttr ERR, Ret=%#x\n", Ret);

        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_VDEC_ChanIFrameDecode(pAvplay->hVdec, (HI_UNF_AVPLAY_I_FRAME_S *)pstIframe, bCapture);
    if (Ret != HI_SUCCESS)
    {
        (HI_VOID)HI_MPI_VDEC_SetPortAttr(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort, &VpssPortOldCfg);

        AVPLAY_INST_UNLOCK(Id);

        HI_ERR_AVPLAY("HI_MPI_VDEC_ChanIFrameDecode failed 0x%x\n", Ret);

        return Ret;
    }

    /*wait for vpss process complete*/
    for (i = 0; i < 20; i++)
    {
        Ret = HI_MPI_VDEC_ReceiveFrame(pAvplay->hVdec, &FrmPack, AVPLAY_RECVFRAME_MAX_TIME);
        if (Ret == HI_SUCCESS)
        {
            break;
        }

        usleep(10 * 1000);
    }

    if (i >= 20)
    {
        AVPLAY_INST_UNLOCK(Id);

        HI_ERR_AVPLAY("HI_MPI_VDEC_ReceiveFrame failed 0x%x\n", Ret);

        return Ret;
    }

    /*resume vpss attr*/
    Ret = HI_MPI_VDEC_SetPortAttr(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort, &VpssPortOldCfg);
    if (HI_SUCCESS != Ret)
    {
        AVPLAY_INST_UNLOCK(Id);

        HI_ERR_AVPLAY("HI_MPI_VDEC_SetPortAttr failed 0x%x\n", Ret);

        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    /* display on vo */
    if (HI_FALSE == bCapture)
    {
        for (i = 0; i < FrmPack.u32FrmNum; i++)
        {
            (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, FrmPack.stFrame[i].hport, &hWindow);

            if (hWindow == pAvplay->MasterFrmChn.hWindow)
            {
                break;
            }
        }

        if (i == FrmPack.u32FrmNum)
        {
            AVPLAY_INST_UNLOCK(Id);

            HI_ERR_AVPLAY("I Frame Dec: No master window\n");

            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        Ret = HI_MPI_WIN_QueueFrame(hWindow, &FrmPack.stFrame[i].stFrameVideo);
        if (HI_SUCCESS != Ret)
        {
            AVPLAY_INST_UNLOCK(Id);

            HI_ERR_AVPLAY("I Frame Dec: Queue frame to master win err, Ret=%#x\n", Ret);

            return Ret;
        }

        for (i = 0; i < FrmPack.u32FrmNum; i++)
        {
            (HI_VOID)AVPLAY_GetWindowByPort(pAvplay, FrmPack.stFrame[i].hport, &hWindow);

            for (j = 0; j < AVPLAY_MAX_SLAVE_FRMCHAN; j++)
            {
                if (hWindow == pAvplay->SlaveFrmChn[j].hWindow)
                {
                    Ret = HI_MPI_WIN_QueueFrame(hWindow, &FrmPack.stFrame[i].stFrameVideo);
                    if (HI_SUCCESS != Ret)
                    {
                        AVPLAY_INST_UNLOCK(Id);

                        HI_ERR_AVPLAY("I Frame Dec: Queue frame to slave win err, Ret=%#x\n", Ret);

                        return Ret;
                    }
                }
            }
        }
    }
    else
    {
        /*use frame of port0, release others*/
        memcpy(&pAvplay->stIFrame, &FrmPack.stFrame[0], sizeof(HI_DRV_VDEC_FRAME_S));

        for (i = 1; i < FrmPack.u32FrmNum; i++)
        {
            Ret = HI_MPI_VDEC_ReleaseFrame(FrmPack.stFrame[i].hport, &FrmPack.stFrame[i].stFrameVideo);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
            }
        }

        AVPLAY_DRV2UNF_VidFrm(&(pAvplay->stIFrame.stFrameVideo), pstCapPicture);
    }

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_ReleaseIFrame(HI_HANDLE hAvplay, HI_UNF_VIDEO_FRAME_INFO_S *pstCapPicture)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (HI_NULL == pstCapPicture)
    {
        HI_ERR_AVPLAY("para pstCapPicture is null\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        AVPLAY_INST_UNLOCK(Id);

        HI_ERR_AVPLAY("hVdec is invalid\n");

        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    /* destroy vpss source */
    if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
    {
        Ret = HI_MPI_VDEC_DisablePort(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort);
        Ret |= HI_MPI_VDEC_DestroyPort(pAvplay->hVdec, pAvplay->MasterFrmChn.hPort);
        if (HI_SUCCESS != Ret)
        {
            AVPLAY_INST_UNLOCK(Id);
            HI_ERR_AVPLAY("HI_MPI_VDEC_DestroyPort ERR, Ret=%#x\n", Ret);
            return Ret;
        }

        pAvplay->MasterFrmChn.hPort = HI_INVALID_HANDLE;
    }
    else
    {
        if (pAvplay->stIFrame.hport != HI_INVALID_HANDLE)
        {
            Ret = HI_MPI_VDEC_ReleaseFrame(pAvplay->stIFrame.hport, &pAvplay->stIFrame.stFrameVideo);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("avplay release frame failed 0x%x\n", Ret);
            }

            memset(&pAvplay->stIFrame.stFrameVideo, 0x0, sizeof(HI_DRV_VIDEO_FRAME_S));
        }
    }

    pAvplay->stIFrame.hport = HI_INVALID_HANDLE;

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_SetDecodeMode(HI_HANDLE hAvplay, HI_UNF_VCODEC_MODE_E enDecodeMode)
{
    HI_S32                  Ret;
    HI_U32                  Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S               *pAvplay;
    HI_UNF_VCODEC_ATTR_S    VdecAttr;

    if (enDecodeMode >= HI_UNF_VCODEC_MODE_BUTT)
    {
        HI_ERR_AVPLAY("para enDecodeMode 0x%x is invalid\n", enDecodeMode);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        AVPLAY_INST_UNLOCK(Id);
        HI_ERR_AVPLAY("vid chn is close, can not set vdec attr\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_VDEC_GetChanAttr(pAvplay->hVdec, &VdecAttr);
    if (Ret != HI_SUCCESS)
    {
        AVPLAY_INST_UNLOCK(Id);
        HI_ERR_AVPLAY("HI_MPI_VDEC_GetChanAttr failed 0x%x\n", Ret);
        return Ret;
    }

    VdecAttr.enMode = enDecodeMode;

    Ret = HI_MPI_VDEC_SetChanAttr(pAvplay->hVdec, &VdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_SetChanAttr failed 0x%x\n", Ret);
    }

    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}

#ifndef __aarch64__
HI_S32 HI_MPI_AVPLAY_RegisterEvent(
        HI_HANDLE                   hAvplay,
        HI_UNF_AVPLAY_EVENT_E       enEvent,
        HI_UNF_AVPLAY_EVENT_CB_FN   pfnEventCB
    )
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (enEvent >= HI_UNF_AVPLAY_EVENT_BUTT)
    {
        HI_ERR_AVPLAY("para enEvent 0x%x is invalid\n", enEvent);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (!pfnEventCB)
    {
        HI_ERR_AVPLAY("para pfnEventCB is null\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (pAvplay->EvtCbFunc[enEvent])
    {
        AVPLAY_INST_UNLOCK(Id);
        HI_ERR_AVPLAY("this event %d has been registered.\n", enEvent);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    pAvplay->EvtCbFunc[enEvent] = pfnEventCB;
    pAvplay->EvtCbFunc64Flag = HI_FALSE;

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}
#endif

HI_S32 HI_MPI_AVPLAY_RegisterEvent64(
        HI_HANDLE                   hAvplay,
        HI_UNF_AVPLAY_EVENT_E       enEvent,
        HI_UNF_AVPLAY_EVENT_CB_FN64 pfnEventCB
    )
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (enEvent >= HI_UNF_AVPLAY_EVENT_BUTT)
    {
        HI_ERR_AVPLAY("para enEvent 0x%x is invalid\n", enEvent);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (!pfnEventCB)
    {
        HI_ERR_AVPLAY("para pfnEventCB is null\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (pAvplay->EvtCbFunc64[enEvent])
    {
        AVPLAY_INST_UNLOCK(Id);
        HI_ERR_AVPLAY("this event %d has been registered.\n", enEvent);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    pAvplay->EvtCbFunc64[enEvent] = pfnEventCB;
    pAvplay->EvtCbFunc64Flag = HI_TRUE;

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_UnRegisterEvent(HI_HANDLE hAvplay, HI_UNF_AVPLAY_EVENT_E enEvent)
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (enEvent >= HI_UNF_AVPLAY_EVENT_BUTT)
    {
        HI_ERR_AVPLAY("para enEvent 0x%x is invalid\n", enEvent);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

#ifndef __aarch64__
    pAvplay->EvtCbFunc[enEvent] = HI_NULL;
#endif

    pAvplay->EvtCbFunc64[enEvent] = HI_NULL;

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_RegisterAcodecLib(const HI_CHAR *pFileName)
{
    HI_S32    Ret;

    if (!pFileName)
    {
        HI_ERR_AVPLAY("para pFileName is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    Ret = HI_MPI_ADEC_RegisterDeoderLib(pFileName, strlen(pFileName));
    if (Ret != HI_SUCCESS)
    {
        HI_INFO_AVPLAY("HI_MPI_ADEC_RegisterDeoderLib failed 0x%x\n", Ret);
    }

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_RegisterVcodecLib(const HI_CHAR *pFileName)
{
    HI_S32    Ret;

    if (!pFileName)
    {
        HI_ERR_AVPLAY("para pFileName is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    Ret = HI_MPI_VDEC_RegisterVcodecLib(pFileName);
    if (Ret != HI_SUCCESS)
    {
        HI_INFO_AVPLAY("HI_MPI_VDEC_RegisterVcodecLib failed 0x%x\n", Ret);
    }

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_FoundSupportDeoder(const HA_FORMAT_E enFormat,HI_U32 * penDstCodecID)
{
    HI_S32    Ret;

    Ret = HI_MPI_ADEC_FoundSupportDeoder(enFormat,penDstCodecID);
    if (Ret != HI_SUCCESS)
    {
        HI_INFO_AVPLAY("HI_MPI_ADEC_FoundSupportDeoder failed 0x%x\n", Ret);
    }

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_ConfigAcodec( const HI_U32 enDstCodecID, HI_VOID *pstConfigStructure)
{
    HI_S32 Ret;

    Ret = HI_MPI_ADEC_SetConfigDeoder(enDstCodecID, pstConfigStructure);
    if (Ret != HI_SUCCESS)
    {
        HI_INFO_AVPLAY("HI_MPI_ADEC_SetConfigDeoder failed 0x%x\n", Ret);
    }

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_PreStart(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    HI_S32      VidStd = -1;
    AVPLAY_S   *pAvplay;

    switch ((HI_U32)enChn)
    {
        case HI_UNF_AVPLAY_MEDIA_CHAN_AUD :
        case HI_UNF_AVPLAY_MEDIA_CHAN_VID :
        case HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID :
            break;

        default :
            HI_ERR_AVPLAY("para enChn 0x%x is invalid\n", enChn);
            return HI_ERR_AVPLAY_INVALID_PARA;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS != pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        AVPLAY_INST_UNLOCK(Id);
        HI_ERR_AVPLAY("HI_MPI_AVPLAY_PreStart is not supported in es mode\n");
        return HI_ERR_AVPLAY_NOT_SUPPORT;
    }

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID))
    {
        AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

        if (!pAvplay->VidEnable && !pAvplay->bVidPreEnable)
        {
            if (HI_UNF_VCODEC_TYPE_H264 == pAvplay->VdecAttr.enType)
            {
                VidStd = STD_H264;
                pAvplay->PreVidWaterLine = 0;
            }
            else if(HI_UNF_VCODEC_TYPE_MPEG2 == pAvplay->VdecAttr.enType)
            {
                VidStd = STD_MPEG2;
                pAvplay->PreVidWaterLine = 0;
            }
            else if(HI_UNF_VCODEC_TYPE_MPEG4 == pAvplay->VdecAttr.enType)
            {
                VidStd = STD_MPEG4;
                pAvplay->PreVidWaterLine = 0;
            }
            else
            {
                pAvplay->PreVidWaterLine = AVPLAY_VID_PREBUF_THRESHOLD;
            }

            if (-1 != VidStd)
            {
                Ret = DetFrm_Create(&pAvplay->hDFCtx, VidStd);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("call DetFrm_Create failed 0x%x\n", Ret);
                    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
                    AVPLAY_INST_UNLOCK(Id);
                }
            }

        #ifdef HI_AVPLAY_PLAY_AND_REC_SUPPORT
            Ret = AVPLAY_MallocDmxChnWithPID(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID,pAvplay->DmxVidPid);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("Avplay malloc vid dmx chn failed.\n");
                AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
                AVPLAY_INST_UNLOCK(Id);
                return Ret;
            }

            Ret = AVPLAY_MallocVidBuffer(pAvplay, pAvplay->hVdec);
            if (Ret != HI_SUCCESS)
            {
                AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
                AVPLAY_INST_UNLOCK(Id);
                return Ret;
            }
        #endif

            Ret = HI_MPI_DMX_OpenChannel(pAvplay->hDmxVid);
            if (Ret != HI_SUCCESS)
            {
            #ifdef HI_AVPLAY_PLAY_AND_REC_SUPPORT
                (HI_VOID)AVPLAY_FreeVidBuffer(pAvplay, pAvplay->hVdec);
            #endif
                AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
                AVPLAY_INST_UNLOCK(Id);
                HI_ERR_AVPLAY("HI_MPI_DMX_OpenChannel 0x%x failed 0x%x\n", pAvplay->hDmxVid, Ret);
                return Ret;
            }

            pAvplay->VidPreBufThreshold = 0;
            pAvplay->VidPreSysTime = -1;
            pAvplay->bVidPreEnable = HI_TRUE;
            pAvplay->PreVidBufLen = 0;
            pAvplay->PreVidFirstWaterLine = 0;
            pAvplay->PreVidSecondWaterLine = 0;
            AVPLAY_PrePlay(pAvplay);
        }

        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
    }

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_AUD))
    {
        AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);

        if (!pAvplay->AudEnable && !pAvplay->bAudPreEnable)
        {
            HI_U32 i;

        #ifdef HI_AVPLAY_PLAY_AND_REC_SUPPORT
            if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
            {
                Ret = AVPLAY_MallocDmxChnWithPID(pAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, pAvplay->DmxAudPid[pAvplay->CurDmxAudChn]);
                if (Ret != HI_SUCCESS)
                {
                    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
                    AVPLAY_INST_UNLOCK(Id);
                    HI_ERR_AVPLAY("Avplay malloc aud dmx chn failed.\n");
                    return Ret;
                }
            }
        #endif

            for (i = 0; i < pAvplay->DmxAudChnNum; i++)
            {
                Ret = HI_MPI_DMX_OpenChannel(pAvplay->hDmxAud[i]);
                if (HI_SUCCESS != Ret)
                {
                    HI_ERR_AVPLAY("HI_MPI_DMX_OpenChannel 0x%x failed 0x%x\n", i, Ret);
                    break;
                }
            }

            if (i < pAvplay->DmxAudChnNum)
            {
                HI_U32 j;

                for (j = 0; j < i; j++)
                {
                    (HI_VOID)HI_MPI_DMX_CloseChannel(pAvplay->hDmxAud[j]);
                }
                AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
                AVPLAY_INST_UNLOCK(Id);
                return HI_FAILURE;
            }

            pAvplay->AudPreBufThreshold = 0;
            pAvplay->AudPreSysTime = -1;
            pAvplay->bAudPreEnable = HI_TRUE;
            AVPLAY_PrePlay(pAvplay);
        }

        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
    }

    Ret = AVPLAY_PcrEnable(pAvplay);
    if (HI_SUCCESS != Ret)
    {
        AVPLAY_INST_UNLOCK(Id);
        return Ret;
    }

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_Start(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    switch ((HI_U32)enChn)
    {
        case HI_UNF_AVPLAY_MEDIA_CHAN_AUD :
        case HI_UNF_AVPLAY_MEDIA_CHAN_VID :
        case HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID :
            break;

        default :
            HI_ERR_AVPLAY("para enChn 0x%x is invalid\n", enChn);
            return HI_ERR_AVPLAY_INVALID_PARA;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_UNF_AVPLAY_MEDIA_CHAN_VID == enChn)
    {
        AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

        Ret = AVPLAY_StartVideo(pAvplay);

        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);

        if (HI_SUCCESS != Ret)
        {
            AVPLAY_INST_UNLOCK(Id);
            return Ret;
        }

        if (!pAvplay->AudEnable)
        {
            AVPLAY_Play(pAvplay);
        }

        (HI_VOID)HI_MPI_STAT_Event(STAT_EVENT_VSTART, 0);
    }

    if (HI_UNF_AVPLAY_MEDIA_CHAN_AUD == enChn)
    {
        AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);

        Ret = AVPLAY_StartAudio(pAvplay);

        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);

        if (HI_SUCCESS != Ret)
        {
            AVPLAY_INST_UNLOCK(Id);
            return Ret;
        }

        if (!pAvplay->VidEnable)
        {
            AVPLAY_Play(pAvplay);
        }

        (HI_VOID)HI_MPI_STAT_Event(STAT_EVENT_ASTART, 0);
    }

    if ((HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID) == enChn)
    {
        AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

        Ret = AVPLAY_StartAudio(pAvplay);
        if (HI_SUCCESS != Ret)
        {
            goto ERR_UNLOCK;
        }

        Ret = AVPLAY_StartVideo(pAvplay);
        if (HI_SUCCESS != Ret)
        {
            (HI_VOID)AVPLAY_StopAudio(pAvplay);
            goto ERR_UNLOCK;
        }

ERR_UNLOCK:
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);

        if (HI_SUCCESS != Ret)
        {
            AVPLAY_INST_UNLOCK(Id);
            return Ret;
        }

        AVPLAY_Play(pAvplay);
        (HI_VOID)HI_MPI_STAT_Event(STAT_EVENT_VSTART, 0);
        (HI_VOID)HI_MPI_STAT_Event(STAT_EVENT_ASTART, 0);
    }

    Ret = AVPLAY_PcrEnable(pAvplay);
    if (HI_SUCCESS != Ret)
    {
        AVPLAY_INST_UNLOCK(Id);
        return Ret;
    }

    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_PreStop(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn,const HI_UNF_AVPLAY_PRESTOP_OPT_S *pPreStopOpt)
{
    HI_ERR_AVPLAY("HI_MPI_AVPLAY_PreStop is not supported\n");
    return HI_ERR_AVPLAY_NOT_SUPPORT;
}

HI_S32 HI_MPI_AVPLAY_Stop(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_UNF_AVPLAY_STOP_OPT_S *pStop)
{
    HI_S32                      Ret;
    HI_U32                      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S                   *pAvplay;
    HI_UNF_AVPLAY_STOP_OPT_S    StopOpt;

    switch ((HI_U32)enChn)
    {
        case HI_UNF_AVPLAY_MEDIA_CHAN_AUD :
        case HI_UNF_AVPLAY_MEDIA_CHAN_VID :
        case HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID :
            break;

        default :
            HI_ERR_AVPLAY("para enChn 0x%x is invalid\n", enChn);
            return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (pStop)
    {
        if (pStop->enMode >= HI_UNF_AVPLAY_STOP_MODE_BUTT)
        {
            HI_ERR_AVPLAY("para pStop->enMode is invalid.\n");
            return HI_ERR_AVPLAY_INVALID_PARA;
        }

        StopOpt.u32TimeoutMs = pStop->u32TimeoutMs;
        StopOpt.enMode = pStop->enMode;
    }
    else
    {
        StopOpt.u32TimeoutMs = 0;
        StopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (AVPLAY_IsStopStatus(pAvplay, enChn))
    {
        HI_INFO_AVPLAY("The chn is already stoped\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_SUCCESS;
    }

    if (StopOpt.u32TimeoutMs)
    {
        Ret = AVPLAY_SetBlockStopMode(pAvplay, &StopOpt);
        if (HI_SUCCESS != Ret)
        {
            AVPLAY_INST_UNLOCK(Id);
            return Ret;
        }
    }

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID))
    {
        AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

        Ret = AVPLAY_StopVideo(pAvplay, &StopOpt);

        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);

        if (HI_SUCCESS != Ret)
        {
            AVPLAY_INST_UNLOCK(Id);
            return Ret;
        }
    }

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_AUD))
    {
        AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);

        Ret = AVPLAY_StopAudio(pAvplay);

        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);

        if (HI_SUCCESS != Ret)
        {
            AVPLAY_INST_UNLOCK(Id);
            return Ret;
        }
    }

    Ret = AVPLAY_PcrDisable(pAvplay);
    if (HI_SUCCESS != Ret)
    {
        AVPLAY_INST_UNLOCK(Id);
        return Ret;
    }

    if (   (!pAvplay->VidEnable) && (!pAvplay->bVidPreEnable)
        && (!pAvplay->AudEnable) && (!pAvplay->bAudPreEnable) )
    {
        AVPLAY_Stop(pAvplay);

        AVPLAY_Notify(pAvplay, HI_UNF_AVPLAY_EVENT_STOP, HI_NULL);
    }

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_Pause(HI_HANDLE hAvplay)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;
    HI_U32      i;

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_UNF_AVPLAY_STATUS_PAUSE == pAvplay->CurStatus)
    {
        AVPLAY_INST_UNLOCK(Id);
        return HI_SUCCESS;
    }

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

    if ((!pAvplay->VidEnable) && (!pAvplay->AudEnable))
    {
        HI_ERR_AVPLAY("vid and aud chn is stopped.\n");
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_SYNC_Pause(pAvplay->hSync);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("HI_MPI_SYNC_Pause failed 0x%x.\n", Ret);
    }

    AVPLAY_Pause(pAvplay);

    if (pAvplay->VidEnable)
    {
        if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
        {
            (HI_VOID)HI_MPI_WIN_Pause(pAvplay->MasterFrmChn.hWindow, HI_TRUE);
        }

        for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
        {
            if (HI_INVALID_HANDLE != pAvplay->SlaveFrmChn[i].hWindow)
            {
                (HI_VOID)HI_MPI_WIN_Pause(pAvplay->SlaveFrmChn[i].hWindow, HI_TRUE);

            }
        }

        for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
        {
            if (HI_INVALID_HANDLE != pAvplay->VirFrmChn[i].hWindow)
            {
                (HI_VOID)HI_MPI_WIN_Pause(pAvplay->VirFrmChn[i].hWindow, HI_TRUE);
            }
        }
    }

    if (pAvplay->AudEnable)
    {
        for (i = 0; i < pAvplay->TrackNum; i++)
        {
            if (HI_INVALID_HANDLE != pAvplay->hTrack[i])
            {
                Ret |= HI_MPI_AO_Track_Pause(pAvplay->hTrack[i]);
            }
        }

        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_HIAO_SetPause failed 0x%x.\n", Ret);
        }
    }

    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_Tplay(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_TPLAY_OPT_S *pstTplayOpt)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;
    HI_U32      i;
    HI_U32      AvplayRatio;
    HI_BOOL     bSetEosFlag;

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

    if (!pAvplay->VidEnable && !pAvplay->AudEnable)
    {
        HI_ERR_AVPLAY("vid and aud chn is stopped.\n");
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
    {
        HI_ERR_AVPLAY("AVPLAY has not attach master window.\n");
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    /* disable frc if opt is null */
    if (HI_NULL == pstTplayOpt)
    {
        pAvplay->bFrcEnable = HI_FALSE;
        AvplayRatio = AVPLAY_ALG_FRC_BASE_PLAY_RATIO;
    }
    else
    {
        pAvplay->bFrcEnable = HI_TRUE;

        AvplayRatio = (pstTplayOpt->u32SpeedInteger*1000 + pstTplayOpt->u32SpeedDecimal) * AVPLAY_ALG_FRC_BASE_PLAY_RATIO / 1000;

        if ((pstTplayOpt->u32SpeedInteger > 64)
            || (pstTplayOpt->u32SpeedDecimal > 999)
            || (AvplayRatio > AVPLAY_ALG_FRC_MAX_PLAY_RATIO)
            || (AvplayRatio < AVPLAY_ALG_FRC_MIN_PLAY_RATIO)
            )
        {
            HI_ERR_AVPLAY("Set tplay speed invalid!\n");
            AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
            AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
            AVPLAY_INST_UNLOCK(Id);
            return HI_ERR_AVPLAY_INVALID_PARA;
        }
    }

    if (HI_UNF_AVPLAY_STATUS_TPLAY == pAvplay->CurStatus)
    {
        pAvplay->FrcParamCfg.u32PlayRate = AvplayRatio;

        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_SUCCESS;
    }

    if (((HI_UNF_AVPLAY_STATUS_PLAY == pAvplay->LstStatus) && (HI_UNF_AVPLAY_STATUS_PAUSE == pAvplay->CurStatus))
      ||(HI_UNF_AVPLAY_STATUS_PLAY == pAvplay->CurStatus)
       )
    {
        bSetEosFlag = pAvplay->bSetEosFlag;

        Ret = AVPLAY_Reset(pAvplay);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("avplay reset err, Ret=%#x.\n", Ret);
            AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
            AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
            AVPLAY_INST_UNLOCK(Id);
            return Ret;
        }

        if (bSetEosFlag)
        {
            Ret = AVPLAY_SetEosFlag(pAvplay);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AVPLAY("ERR: AVPLAY_SetEosFlag, Ret = %#x.\n", Ret);
                AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
                AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
                AVPLAY_INST_UNLOCK(Id);
                return Ret;
            }

            pAvplay->bSetEosFlag = HI_TRUE;
        }
    }

    if (HI_UNF_AVPLAY_STATUS_PAUSE == pAvplay->CurStatus)
    {
        if (pAvplay->VidEnable)
        {
            if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
            {
                (HI_VOID)HI_MPI_WIN_Pause(pAvplay->MasterFrmChn.hWindow, HI_FALSE);
            }

            for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
            {
                if (HI_INVALID_HANDLE != pAvplay->SlaveFrmChn[i].hWindow)
                {
                    (HI_VOID)HI_MPI_WIN_Pause(pAvplay->SlaveFrmChn[i].hWindow, HI_FALSE);

                }
            }

            for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
            {
                if (HI_INVALID_HANDLE != pAvplay->VirFrmChn[i].hWindow)
                {
                    (HI_VOID)HI_MPI_WIN_Pause(pAvplay->VirFrmChn[i].hWindow, HI_FALSE);
                }
            }
        }

        /* pause->tplay, resume hiao */
        if (pAvplay->AudEnable)
        {
            for (i = 0; i < pAvplay->TrackNum; i++)
            {
                if (HI_INVALID_HANDLE != pAvplay->hTrack[i])
                {
                    Ret = HI_MPI_AO_Track_Resume(pAvplay->hTrack[i]);
                    if (Ret != HI_SUCCESS)
                    {
                        HI_ERR_AVPLAY("HI_MPI_AO_Track_Resume failed 0x%x\n", Ret);
                    }
                }
            }
        }

        /* pause->tplay, resume sync */
        Ret = HI_MPI_SYNC_Resume(pAvplay->hSync);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_SYNC_Resume failed 0x%x\n", Ret);
        }
    }

    pAvplay->FrcParamCfg.u32PlayRate = AvplayRatio;

    (HI_VOID)HI_MPI_SYNC_Tplay(pAvplay->hSync);
    AVPLAY_Tplay(pAvplay);

    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_Resume(HI_HANDLE hAvplay)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;
    HI_U32      i;

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

    pAvplay->bStepMode = HI_FALSE;
    pAvplay->bStepPlay = HI_FALSE;

    if (HI_UNF_AVPLAY_STATUS_PLAY == pAvplay->CurStatus)
    {
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_SUCCESS;
    }

    if ((!pAvplay->VidEnable) && (!pAvplay->AudEnable))
    {
        HI_ERR_AVPLAY("vid and aud chn is stopped.\n");
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (((HI_UNF_AVPLAY_STATUS_TPLAY == pAvplay->LstStatus) && (HI_UNF_AVPLAY_STATUS_PAUSE == pAvplay->CurStatus))
      ||(HI_UNF_AVPLAY_STATUS_TPLAY == pAvplay->CurStatus)
       )
    {
        Ret = AVPLAY_Reset(pAvplay);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("AVPLAY_Reset, Ret=%#x.\n", Ret);
            AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
            AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
            AVPLAY_INST_UNLOCK(Id);
            return Ret;
        }

        (HI_VOID)HI_MPI_SYNC_Play(pAvplay->hSync);

       pAvplay->bFrcEnable = HI_TRUE;
       pAvplay->FrcParamCfg.u32PlayRate = AVPLAY_ALG_FRC_BASE_PLAY_RATIO;
    }

    /* resume hiao and sync if curstatus is pause */
    if (HI_UNF_AVPLAY_STATUS_PAUSE == pAvplay->CurStatus)
    {
        if (pAvplay->VidEnable)
        {
            if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
            {
                (HI_VOID)HI_MPI_WIN_Pause(pAvplay->MasterFrmChn.hWindow, HI_FALSE);
            }

            for (i = 0; i < AVPLAY_MAX_SLAVE_FRMCHAN; i++)
            {
                if (HI_INVALID_HANDLE != pAvplay->SlaveFrmChn[i].hWindow)
                {
                    (HI_VOID)HI_MPI_WIN_Pause(pAvplay->SlaveFrmChn[i].hWindow, HI_FALSE);
                }
            }

            for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
            {
                if (HI_INVALID_HANDLE != pAvplay->VirFrmChn[i].hWindow)
                {
                    (HI_VOID)HI_MPI_WIN_Pause(pAvplay->VirFrmChn[i].hWindow, HI_FALSE);
                }
            }
        }

        if (pAvplay->AudEnable)
        {
            for (i = 0; i < pAvplay->TrackNum; i++)
            {
                if (HI_INVALID_HANDLE != pAvplay->hTrack[i])
                {
                    Ret = HI_MPI_AO_Track_Resume(pAvplay->hTrack[i]);
                    if (Ret != HI_SUCCESS)
                    {
                        HI_ERR_AVPLAY("HI_MPI_AO_Track_Resume failed 0x%x\n", Ret);
                    }
                }
            }
        }

        Ret = HI_MPI_SYNC_Resume(pAvplay->hSync);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_SYNC_Resume failed 0x%x\n", Ret);
        }
    }

    AVPLAY_Play(pAvplay);

    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_Reset(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_RESET_OPT_S *pstResetOpt)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

    if ((HI_UNF_AVPLAY_STATUS_PLAY == pAvplay->CurStatus)
        && (HI_UNF_AVPLAY_STREAM_TYPE_ES == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        && (HI_NULL != pstResetOpt)
        && (HI_INVALID_PTS != pstResetOpt->u32SeekPtsMs)
    )
    {
        HI_INFO_AVPLAY("sdk buf seek enter\n");

        Ret = AVPLAY_Seek(pAvplay, pstResetOpt->u32SeekPtsMs);
        if (Ret != HI_SUCCESS)
        {
            HI_INFO_AVPLAY("not in sdk buf\n");

            Ret = AVPLAY_Reset(pAvplay);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("AVPLAY_Reset failed.\n");
                AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
                AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
                AVPLAY_INST_UNLOCK(Id);
                return Ret;
            }
        }

        HI_INFO_AVPLAY("sdk buf seek quit\n");
    }
    else
    {
        Ret = AVPLAY_Reset(pAvplay);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("AVPLAY_Reset failed.\n");
            AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
            AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
            AVPLAY_INST_UNLOCK(Id);
            return Ret;
        }
    }

    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_GetBuf(HI_HANDLE  hAvplay,
                            HI_UNF_AVPLAY_BUFID_E enBufId,
                            HI_U32                u32ReqLen,
                            HI_UNF_STREAM_BUF_S  *pstData,
                            HI_U32                u32TimeOutMs)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!pstData)
    {
        HI_ERR_AVPLAY("para pstData is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    if (u32TimeOutMs != 0)
    {
        HI_ERR_AVPLAY("enBufId=%d NOT support block mode, please set 'u32TimeOutMs' to 0.\n", enBufId);
        return HI_ERR_AVPLAY_NOT_SUPPORT;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        HI_ERR_AVPLAY("avplay is ts stream mode.\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    /*if user contine getting or putting buffer after EOS, Current status change to Last status*/
    if (HI_UNF_AVPLAY_STATUS_EOS == pAvplay->CurStatus)
    {
        HI_WARN_AVPLAY("avplay curstatus is eos.\n");
        pAvplay->CurStatus = pAvplay->LstStatus;
    }

    pAvplay->bSetEosFlag = HI_FALSE;

    switch (enBufId)
    {
        case HI_UNF_AVPLAY_BUF_ID_ES_VID :
        case HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_BL :
        case HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_EL :
        case HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_DL :
        {
            Ret = AVPLAY_GetVidBuf(pAvplay, enBufId, u32ReqLen, pstData);
            break;
        }

        case HI_UNF_AVPLAY_BUF_ID_ES_AUD :
        case HI_UNF_AVPLAY_BUF_ID_ES_AUD_AD :
        {
            Ret = AVPLAY_GetAudBuf(pAvplay, enBufId, u32ReqLen, pstData);
            break;
        }

        default :
            Ret = HI_ERR_AVPLAY_INVALID_PARA;
            HI_ERR_AVPLAY("para enBufId 0x%x is invalid.\n", enBufId);
            break;
    }

    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_PutBuf(HI_HANDLE                     hAvplay,
                            HI_UNF_AVPLAY_BUFID_E         enBufId,
                            HI_U32                        u32ValidDataLen,
                            HI_U32                        u32Pts,
                            HI_UNF_AVPLAY_PUTBUFEX_OPT_S *pstExOpt)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        HI_ERR_AVPLAY("avplay is ts stream mode.\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    /*if user contine getting or putting buffer after EOS, Current status change to Last status*/
    if (HI_UNF_AVPLAY_STATUS_EOS == pAvplay->CurStatus)
    {
        HI_WARN_AVPLAY("avplay curstatus is eos.\n");
        pAvplay->CurStatus = pAvplay->LstStatus;
    }

    pAvplay->bSetEosFlag = HI_FALSE;

    switch (enBufId)
    {
        case HI_UNF_AVPLAY_BUF_ID_ES_VID :
        case HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_BL :
        case HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_EL :
        case HI_UNF_AVPLAY_BUF_ID_ES_VID_HDR_DL :
        {
            Ret = AVPLAY_PutVidBuf(pAvplay, enBufId, u32ValidDataLen, u32Pts, pstExOpt);
            break;
        }

        case HI_UNF_AVPLAY_BUF_ID_ES_AUD :
        case HI_UNF_AVPLAY_BUF_ID_ES_AUD_AD :
        {
            Ret = AVPLAY_PutAudBuf(pAvplay, enBufId, u32ValidDataLen, u32Pts);
            break;
        }

        default :
            Ret = HI_ERR_AVPLAY_INVALID_PARA;
            HI_ERR_AVPLAY("para enBufId 0x%x is invalid.\n", enBufId);
            break;
    }

    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_GetSyncVdecHandle(HI_HANDLE hAvplay, HI_HANDLE *phVdec, HI_HANDLE *phSync)
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!phVdec)
    {
        HI_ERR_AVPLAY("para phVdec is invalid.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    if (!phSync)
    {
        HI_ERR_AVPLAY("para phSync is invalid.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_AVPLAY("Avplay have not vdec.\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    *phVdec = pAvplay->hVdec;
    *phSync = pAvplay->hSync;

    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_GetSndHandle(HI_HANDLE hAvplay, HI_HANDLE *phTrack)
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!phTrack)
    {
        HI_ERR_AVPLAY("para phTrack is invalid.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_INVALID_HANDLE == pAvplay->hSyncTrack)
    {
        HI_ERR_AVPLAY("Avplay have not main track.\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    *phTrack = pAvplay->hSyncTrack;

    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

// TODO: 确认该接口能否删除
HI_S32 HI_MPI_AVPLAY_GetWindowHandle(HI_HANDLE hAvplay, HI_HANDLE *phWindow)
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!phWindow)
    {
        HI_ERR_AVPLAY("para phWindow is invalid.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
    {
        HI_ERR_AVPLAY("AVPLAY has not attach master window.\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    *phWindow = pAvplay->MasterFrmChn.hWindow;

    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_AttachWindow(HI_HANDLE hAvplay, HI_HANDLE hWindow)
{
    HI_S32              Ret;
    HI_U32              Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S           *pAvplay;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_AVPLAY("para hWindow is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

    Ret = AVPLAY_AttachWindow(pAvplay, hWindow);

    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);

    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_DetachWindow(HI_HANDLE hAvplay, HI_HANDLE hWindow)
{
    HI_S32              Ret;
    HI_U32              Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S           *pAvplay;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_AVPLAY("para hWindow is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

    Ret = AVPLAY_DetachWindow(pAvplay, hWindow);

    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);

    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}


// TODO: 确认该功能是否还需要
HI_S32 HI_MPI_AVPLAY_SetWindowRepeat(HI_HANDLE hAvplay, HI_U32 u32Repeat)
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;
    HI_U32      AvplayRatio;

    if (0 == u32Repeat)
    {
        HI_ERR_AVPLAY("para u32Repeat is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

    if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
    {
        HI_ERR_AVPLAY("AVPLAY has not attach master window.\n");
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    AvplayRatio = 256/u32Repeat;

    if ((AvplayRatio > AVPLAY_ALG_FRC_MAX_PLAY_RATIO)
        || (AvplayRatio < AVPLAY_ALG_FRC_MIN_PLAY_RATIO))
    {
        HI_ERR_AVPLAY("Set repeat invalid!\n");
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    pAvplay->FrcParamCfg.u32PlayRate = AvplayRatio;

    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_AttachSnd(HI_HANDLE hAvplay, HI_HANDLE hTrack)
{
    HI_S32                      Ret;
    HI_U32                      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S                   *pAvplay;
    HI_S32                      i;
    HI_UNF_AUDIOTRACK_ATTR_S    TrackInfo = {0};

    if (HI_INVALID_HANDLE == hTrack)
    {
        HI_ERR_AVPLAY("para hTrack is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);

    for (i=0; i<AVPLAY_MAX_TRACK; i++)
    {
        if (pAvplay->hTrack[i] == hTrack)
        {
            AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
            AVPLAY_INST_UNLOCK(Id);
            return HI_SUCCESS;
        }
    }

    memset(&TrackInfo, 0x0, sizeof(HI_UNF_AUDIOTRACK_ATTR_S));
    Ret = HI_MPI_AO_Track_GetAttr(hTrack, &TrackInfo);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: HI_MPI_HIAO_GetTrackInfo.\n");
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_FAILURE;
    }

    for (i=0; i<AVPLAY_MAX_TRACK; i++)
    {
        if (HI_INVALID_HANDLE == pAvplay->hTrack[i])
        {
            break;
        }
    }

    if(AVPLAY_MAX_TRACK == i)
    {
        HI_ERR_AVPLAY("AVPLAY has attached max track.\n");
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_FAILURE;
    }

    pAvplay->hTrack[i] = hTrack;
    pAvplay->TrackNum++;

    if ((HI_UNF_SND_TRACK_TYPE_VIRTUAL != TrackInfo.enTrackType)
        && (HI_INVALID_HANDLE == pAvplay->hSyncTrack)
        )
    {
        pAvplay->hSyncTrack = hTrack;
    }

    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_DetachSnd(HI_HANDLE hAvplay, HI_HANDLE hTrack)
{
    HI_U32                      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S                   *pAvplay;
    HI_U32                      i, j;
    HI_UNF_AUDIOTRACK_ATTR_S    TrackInfo = {0};

    if (HI_INVALID_HANDLE == hTrack)
    {
        HI_ERR_AVPLAY("para hTrack is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    memset(&TrackInfo, 0x0, sizeof(HI_UNF_AUDIOTRACK_ATTR_S));

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);

    if ((0 == pAvplay->TrackNum) || (pAvplay->TrackNum >= AVPLAY_MAX_TRACK))
    {
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    for (i = 0; i < pAvplay->TrackNum; i++)
    {
        if (pAvplay->hTrack[i] == hTrack)
        {
            break;
        }
    }

    if (i == pAvplay->TrackNum)
    {
        HI_ERR_AVPLAY("this is not a attached track, can not detach.\n");
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    pAvplay->hTrack[i] = pAvplay->hTrack[pAvplay->TrackNum - 1];
    pAvplay->hTrack[pAvplay->TrackNum - 1] = HI_INVALID_HANDLE;
    pAvplay->TrackNum--;

    if (hTrack == pAvplay->hSyncTrack)
    {
        for (j = 0; j < pAvplay->TrackNum; j++)
        {
            (HI_VOID)HI_MPI_AO_Track_GetAttr(pAvplay->hTrack[j], &TrackInfo);

            if (HI_UNF_SND_TRACK_TYPE_VIRTUAL != TrackInfo.enTrackType)
            {
                pAvplay->hSyncTrack = pAvplay->hTrack[j];
                AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
                AVPLAY_INST_UNLOCK(Id);
                return HI_SUCCESS;
            }
        }

        if (j == pAvplay->TrackNum)
        {
            pAvplay->hSyncTrack= HI_INVALID_HANDLE;
        }
    }

    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_GetDmxAudChnHandle(HI_HANDLE hAvplay, HI_HANDLE *phDmxAudChn)
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!phDmxAudChn)
    {
        HI_ERR_AVPLAY("para phDmxAudChn is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (pAvplay->AvplayAttr.stStreamAttr.enStreamType != HI_UNF_AVPLAY_STREAM_TYPE_TS)
    {
        HI_ERR_AVPLAY("avplay is not ts stream mode.\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_INVALID_HANDLE == pAvplay->hAdec)
    {
        HI_ERR_AVPLAY("aud chn is close.\n");
        AVPLAY_INST_UNLOCK(Id);
         return HI_ERR_AVPLAY_INVALID_OPT;
    }

    *phDmxAudChn = pAvplay->hDmxAud[pAvplay->CurDmxAudChn];

    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_GetDmxVidChnHandle(HI_HANDLE hAvplay, HI_HANDLE *phDmxVidChn)
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!phDmxVidChn)
    {
        HI_ERR_AVPLAY("para phDmxVidChn is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (pAvplay->AvplayAttr.stStreamAttr.enStreamType != HI_UNF_AVPLAY_STREAM_TYPE_TS)
    {
        HI_ERR_AVPLAY("avplay is not ts stream mode.\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_AVPLAY("vid chn is close.\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    *phDmxVidChn = pAvplay->hDmxVid;

    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_GetStatusInfo(HI_HANDLE hAvplay, HI_UNF_AVPLAY_STATUS_INFO_S *pstStatusInfo)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!pstStatusInfo)
    {
        HI_ERR_AVPLAY("para pstStatusInfo is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    Ret = AVPLAY_GetStatusInfo(pAvplay, pstStatusInfo);

    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_GetStreamInfo(HI_HANDLE hAvplay, HI_UNF_AVPLAY_STREAM_INFO_S *pstStreamInfo)
{
    HI_S32              Ret;
    HI_U32              Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S           *pAvplay;
    ADEC_STREAMINFO_S   AdecStreaminfo = {0};
    VDEC_STATUSINFO_S   VdecBufStatus  = {0};

    if (!pstStreamInfo)
    {
        HI_ERR_AVPLAY("para pstStreamInfo is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (pAvplay->hAdec != HI_INVALID_HANDLE)
    {
        Ret = HI_MPI_ADEC_GetInfo(pAvplay->hAdec, HI_MPI_ADEC_STREAMINFO, &AdecStreaminfo);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_ADEC_GetInfo failed 0x%x\n", Ret);
        }
        else
        {
            pstStreamInfo->stAudStreamInfo.enACodecType = AdecStreaminfo.u32CodecID;
            pstStreamInfo->stAudStreamInfo.enSampleRate = AdecStreaminfo.enSampleRate;
            pstStreamInfo->stAudStreamInfo.enBitDepth = HI_UNF_BIT_DEPTH_16;
        }
    }

    if (pAvplay->hVdec != HI_INVALID_HANDLE)
    {
        Ret = HI_MPI_VDEC_GetChanStreamInfo(pAvplay->hVdec, &(pstStreamInfo->stVidStreamInfo));
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_GetChanStreamInfo failed 0x%x\n", Ret);
        }

        Ret = HI_MPI_VDEC_GetChanStatusInfo(pAvplay->hVdec, &VdecBufStatus);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("call HI_MPI_VDEC_GetChanStatusInfo failed 0x%x\n", Ret);
        }
        else
        {
            pstStreamInfo->stVidStreamInfo.u32bps = VdecBufStatus.u32StrmInBps;
        }
    }

    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_GetAudioSpectrum(HI_HANDLE hAvplay, HI_U16 *pSpectrum, HI_U32 u32BandNum)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!pSpectrum)
    {
        HI_ERR_AVPLAY("para pSpectrum is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (!pAvplay->AudEnable)
    {
        HI_ERR_AVPLAY("aud chn is stopped.\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_ADEC_GetAudSpectrum(pAvplay->hAdec,  pSpectrum , u32BandNum);
    if(HI_SUCCESS != Ret)
    {
        HI_WARN_AVPLAY("WARN: HI_MPI_ADEC_GetAudSpectrum.\n");
    }

    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}

/* add for user to get buffer state, user may want to check if buffer is empty,
    but NOT want to block the user's thread. then user can use this API to check the buffer state
    by q46153 */
HI_S32 HI_MPI_AVPLAY_IsBuffEmpty(HI_HANDLE hAvplay, HI_BOOL *pbIsEmpty)
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!pbIsEmpty)
    {
        HI_ERR_AVPLAY("para pbIsEmpty is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    *pbIsEmpty = HI_FALSE;

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (pAvplay->bSetEosFlag)
    {
        if (HI_UNF_AVPLAY_STATUS_EOS == pAvplay->CurStatus)
        {
            *pbIsEmpty = HI_TRUE;
            pAvplay->CurBufferEmptyState = HI_TRUE;
        }
        else
        {
            *pbIsEmpty = HI_FALSE;
            pAvplay->CurBufferEmptyState = HI_FALSE;
        }
    }
    else
    {
        *pbIsEmpty = AVPLAY_IsBufEmpty(pAvplay);
    }

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}


/* for DDP test only! call this before HI_UNF_AVPLAY_ChnOpen */
HI_S32 HI_MPI_AVPLAY_SetDDPTestMode(HI_HANDLE hAvplay, HI_BOOL bEnable)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    pAvplay->AudDDPMode = bEnable;
    pAvplay->LastAudPts = 0;

    Ret = HI_MPI_SYNC_SetDDPTestMode(pAvplay->hSync, pAvplay->AudDDPMode);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("Set SYNC DDPTestMode error %#x.\n", Ret);
    }

    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_SwitchDmxAudChn(HI_HANDLE hAvplay, HI_HANDLE hNewDmxAud, HI_HANDLE *phOldDmxAud)
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if ((HI_INVALID_HANDLE == hNewDmxAud) || !phOldDmxAud)
    {
        HI_ERR_AVPLAY("para is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    AVPLAY_Mutex_Lock(&pAvplay->AvplayAudThreadMutex);

    if (pAvplay->CurDmxAudChn >= AVPLAY_MAX_DMX_AUD_CHAN_NUM)
    {
        AVPLAY_Mutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    /* if the es buf has not been released */
    if(pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC])
    {
        (HI_VOID)HI_MPI_DMX_ReleaseEs(pAvplay->hDmxAud[pAvplay->CurDmxAudChn], &pAvplay->AvplayDmxEsBuf);
        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC] = HI_FALSE;
    }

    *phOldDmxAud = pAvplay->hDmxAud[pAvplay->CurDmxAudChn];
    pAvplay->hDmxAud[pAvplay->CurDmxAudChn] = hNewDmxAud;

    AVPLAY_Mutex_UnLock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

/* add for Flashplayer adjust pts */
HI_S32 HI_MPI_AVPLAY_PutAudPts(HI_HANDLE hAvplay, HI_U32 u32AudPts)
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!hAvplay)
    {
        HI_ERR_AVPLAY("para is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    pAvplay->AudInfo.SrcPts = u32AudPts;
    pAvplay->AudInfo.Pts = u32AudPts;

    pAvplay->AudInfo.BufTime = 0;
    pAvplay->AudInfo.FrameNum = 0;
    pAvplay->AudInfo.FrameTime = 5000;

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);
    HI_MPI_SYNC_AudJudge(pAvplay->hSync, &pAvplay->AudInfo, &pAvplay->AudOpt);
    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_FlushStream(HI_HANDLE hAvplay, HI_UNF_AVPLAY_FLUSH_STREAM_OPT_S *pstFlushOpt)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!hAvplay)
    {
        HI_ERR_AVPLAY("para is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_UNF_AVPLAY_STATUS_EOS == pAvplay->CurStatus)
    {
        HI_INFO_AVPLAY("current status is eos!\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_SUCCESS;
    }

    if (pAvplay->bSetEosFlag)
    {
        HI_INFO_AVPLAY("Eos Flag has been set!\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_SUCCESS;
    }

    AVPLAY_Mutex_Lock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_Mutex_Lock(&pAvplay->AvplayVidThreadMutex);

    Ret = AVPLAY_SetEosFlag(pAvplay);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("ERR: AVPLAY_SetEosFlag %#x\n", Ret);
        AVPLAY_Mutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_Mutex_UnLock(&pAvplay->AvplayVidThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return Ret;
    }

    AVPLAY_Mutex_UnLock(&pAvplay->AvplayAudThreadMutex);
    AVPLAY_Mutex_UnLock(&pAvplay->AvplayVidThreadMutex);
    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_Step(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_STEP_OPT_S *pstStepOpt)
{
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (!hAvplay)
    {
        HI_ERR_AVPLAY("para is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayAudThreadMutex);

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

    if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
    {
        HI_ERR_AVPLAY("AVPLAY has not attach master window.\n");
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);
        AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    pAvplay->bStepMode = HI_TRUE;
    pAvplay->bStepPlay = HI_TRUE;

    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayAudThreadMutex);

    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_Invoke(HI_HANDLE hAvplay, HI_UNF_AVPLAY_INVOKE_E enInvokeType, HI_VOID *pPara)
{
    HI_S32                                  Ret = HI_SUCCESS;
    HI_U32                                  Id  = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S                               *pAvplay;
    HI_DRV_VIDEO_FRAME_S                    stVidFrame = {0};
    HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S     stPlayInfo;
    HI_DRV_VIDEO_PRIVATE_S                  stVidPrivate;
    SYNC_STOP_REGION_S                      SyncStopRegion;

    if (enInvokeType >= HI_UNF_AVPLAY_INVOKE_BUTT)
    {
        HI_ERR_AVPLAY("para enInvokeType is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (!pPara)
    {
        HI_ERR_AVPLAY("para pPara is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    memset(&stVidFrame, 0x0, sizeof(HI_DRV_VIDEO_FRAME_S));
    memset(&stPlayInfo, 0x0, sizeof(HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S));
    memset(&stVidPrivate, 0x0, sizeof(HI_DRV_VIDEO_PRIVATE_S));

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_UNF_AVPLAY_INVOKE_VCODEC == enInvokeType)
    {
        if (HI_INVALID_HANDLE == pAvplay->hVdec)
        {
            HI_ERR_AVPLAY("vid chn is close, can not set vcodec cmd.\n");
            AVPLAY_INST_UNLOCK(Id);
            return HI_ERR_AVPLAY_INVALID_OPT;
        }
        
#if !defined(CHIP_TYPE_hi3798mv200) && !defined(CHIP_TYPE_hi3798mv200_a)
        if(((HI_CODEC_VIDEO_CMD_S*)pPara)->u32CmdID == HI_UNF_AVPLAY_SET_SCENE_MODE_CMD)
        {
            AVPLAY_INST_UNLOCK(Id);
            return HI_SUCCESS;	
        }
#endif

        Ret = HI_MPI_VDEC_Invoke(pAvplay->hVdec, pPara);
        if (Ret != HI_SUCCESS)
        {
            HI_WARN_AVPLAY("HI_MPI_VDEC_Invoke failed.\n");
        }
    }
    else if (HI_UNF_AVPLAY_INVOKE_ACODEC == enInvokeType)
    {
        if (HI_INVALID_HANDLE == pAvplay->hAdec)
        {
            HI_ERR_AVPLAY("aud chn is close, can not set acodec cmd.\n");
            AVPLAY_INST_UNLOCK(Id);
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        Ret = HI_MPI_ADEC_SetCodecCmd(pAvplay->hAdec, pPara);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("ADEC_SetCodecCmd failed.\n");
        }
    }
    else if (HI_UNF_AVPLAY_INVOKE_GET_PRIV_PLAYINFO == enInvokeType)
    {
        if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
        {
            HI_ERR_AVPLAY("AVPLAY has not attach master window.\n");
            AVPLAY_INST_UNLOCK(Id);
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        Ret = HI_MPI_WIN_GetLatestFrameInfo(pAvplay->MasterFrmChn.hWindow, &stVidFrame);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("HI_MPI_WIN_GetLatestFrameInfo failed.\n");
        }

        stPlayInfo.u32LastPts = stVidFrame.u32Pts;

        memcpy(&stVidPrivate, (HI_DRV_VIDEO_PRIVATE_S *)(stVidFrame.u32Priv), sizeof(HI_DRV_VIDEO_PRIVATE_S));

        stPlayInfo.u32LastPlayTime = stVidPrivate.u32PrivDispTime;

        stPlayInfo.u32DispOptimizeFlag = pAvplay->u32DispOptimizeFlag;

        memcpy((HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S *)pPara, &stPlayInfo, sizeof(HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S));
    }
    else if (HI_UNF_AVPLAY_INVOKE_SET_DISP_OPTIMIZE_FLAG == enInvokeType)
    {
        pAvplay->u32DispOptimizeFlag = *(HI_U32 *)pPara;
    }
    else if (HI_UNF_AVPLAY_INVOKE_SET_SYNC_MODE == enInvokeType)
    {
        if (*(HI_U32 *)pPara == 1)
        {
            SyncStopRegion.bUseStopRegion = HI_FALSE;
            (HI_VOID)HI_MPI_SYNC_SetExtInfo(pAvplay->hSync, SYNC_EXT_INFO_STOP_REGION, (HI_VOID *)&SyncStopRegion);
        }
    }

    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}


HI_S32 HI_MPI_AVPLAY_AcqUserData(HI_HANDLE hAvplay, HI_UNF_VIDEO_USERDATA_S *pstUserData, HI_UNF_VIDEO_USERDATA_TYPE_E *penType)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if ((HI_INVALID_HANDLE == hAvplay))
    {
        HI_ERR_AVPLAY("para is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (!pAvplay->VidEnable)
    {
        HI_ERR_AVPLAY("Vid chan is not start.\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_VDEC_AcqUserData(pAvplay->hVdec, pstUserData, penType);
    if (HI_SUCCESS != Ret)
    {
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_RlsUserData(HI_HANDLE hAvplay, HI_UNF_VIDEO_USERDATA_S* pstUserData)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if ((HI_INVALID_HANDLE == hAvplay))
    {
        HI_ERR_AVPLAY("para is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (!pAvplay->VidEnable)
    {
        HI_ERR_AVPLAY("Vid chan is not start.\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_VDEC_RlsUserData(pAvplay->hVdec, pstUserData);
    if (HI_SUCCESS != Ret)
    {
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_GetVidChnOpenParam(HI_HANDLE hAvplay, HI_UNF_AVPLAY_OPEN_OPT_S *pstOpenPara)
{
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    if (HI_NULL == pstOpenPara)
    {
        HI_ERR_AVPLAY("pstOpenPara is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_AVPLAY("Vid Chan is not open!\n");
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_MPI_VDEC_GetChanOpenParam(pAvplay->hVdec, pstOpenPara);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AVPLAY("HI_MPI_VDEC_GetChanOpenParam ERR %#x\n", Ret);
        AVPLAY_INST_UNLOCK(Id);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_UseExternalBuffer(HI_HANDLE hAvplay, HI_MPI_EXT_BUFFER_S* pstExtBuf)
{
    HI_S32                  Ret;
    HI_U32                  Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S               *pAvplay;
    VDEC_BUFFER_ATTR_S      stVdecAttr;
    HI_U32                  i;

    if (HI_NULL == pstExtBuf)
    {
        HI_ERR_AVPLAY("invalid external buffer\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

    stVdecAttr.bVideoBypass   = pstExtBuf->bVideoBypass;
    stVdecAttr.u32BufNum      = pstExtBuf->u32BufCnt;
    stVdecAttr.u32Stride      = pstExtBuf->u32Stride;
    stVdecAttr.u32BufSize     = pstExtBuf->u32FrameBufSize;
    stVdecAttr.u32MetaBufSize = pstExtBuf->u32MetadataBufSize;

    stVdecAttr.u32PrivOffset_meta = pstExtBuf->u32PrivMetadataOffset;

    for (i = 0; i < pstExtBuf->u32BufCnt; i++)
    {
        stVdecAttr.pu8UsrVirAddr[i]      = 0;
        stVdecAttr.u32PhyAddr[i]         = pstExtBuf->au32FrameBuffer[i];
        stVdecAttr.pu8UsrVirAddr_meta[i] = 0;
        stVdecAttr.u32PhyAddr_meta[i]    = pstExtBuf->au32MetadataBuf[i];
        stVdecAttr.s32FrameBufFd[i]      = pstExtBuf->as32FrameBufFd[i];
        stVdecAttr.s32MetadataBufFd[i]    = pstExtBuf->as32MetadataBufFd[i];
    }

    Ret = HI_MPI_VDEC_SetExternBufferState(pAvplay->hVdec, VDEC_EXTBUFFER_STATE_STOP);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("stop external buffer manager failed");
    }
    Ret = HI_MPI_VDEC_SetExternBuffer(pAvplay->hVdec, &stVdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("Set vdec external buffer failed");
        goto out;
    }

    Ret = HI_MPI_VDEC_SetExternBufferState(pAvplay->hVdec, VDEC_EXTBUFFER_STATE_START);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("stop external buffer manager failed");
    }
out:
    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
    AVPLAY_INST_UNLOCK(Id);
    return Ret;
}

HI_S32 HI_MPI_AVPLAY_DeleteExternalBuffer(HI_HANDLE hAvplay, HI_MPI_EXT_BUFFER_S* pstExtBuf)
{
    HI_S32                      Ret;
    HI_U32                      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S                   *pAvplay;
    HI_U32                      i;
    VDEC_FRAMEBUFFER_STATE_E    state = VDEC_BUF_STATE_BUTT;
    HI_S32                      WaitTime = 0;

    if (HI_NULL == pstExtBuf)
    {
        HI_ERR_AVPLAY("invalid external buffer\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    AVPLAY_ThreadMutex_Lock(&pAvplay->AvplayVidThreadMutex);

    Ret = HI_MPI_VDEC_SetExternBufferState(pAvplay->hVdec, VDEC_EXTBUFFER_STATE_STOP);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("stop external buffer manager failed");
    }
    /* may be only stop vidchannel,avoid there is frame at avplay, when stop avplay, we drop this frame*/
    if (HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
    {
        /*Release vpss frame*/
        (HI_VOID)AVPLAY_RelAllChnFrame(pAvplay);
        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
        HI_INFO_AVPLAY("release avplay frame success");
    }

    /* release frame in virtual window */
    for (i = 0; i < AVPLAY_MAX_VIR_FRMCHAN; i++)
    {
        if (HI_INVALID_HANDLE != pAvplay->VirFrmChn[i].hWindow)
        {
            Ret = HI_MPI_WIN_Reset(pAvplay->VirFrmChn[i].hWindow, HI_DRV_WIN_SWITCH_BLACK);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AVPLAY("reset window failed");
            }
        }
    }

    for (i = 0; i < pstExtBuf->u32BufCnt; i++)
    {
        WaitTime = 0;
        while (WaitTime < 50)
        {
            Ret = HI_MPI_VDEC_CheckAndDeleteExtBuffer(pAvplay->hVdec, pstExtBuf->au32FrameBuffer[i], &state);
            if (Ret != HI_SUCCESS || (state == VDEC_BUF_STATE_IN_USE))
            {
                HI_ERR_AVPLAY("delete buffer %#x from vdec failed, state:%d, Ret:%d\n", pstExtBuf->au32FrameBuffer[i], state, Ret);
                usleep(1000*10);
                WaitTime++;
                continue;
            }
            HI_INFO_AVPLAY("delete buffer %#x from vdec successed, state:%d, Ret:%d\n", pstExtBuf->au32FrameBuffer[i], state, Ret);
            break;
        }
    }
    Ret = HI_MPI_VDEC_SetExternBufferState(pAvplay->hVdec, VDEC_EXTBUFFER_STATE_START);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AVPLAY("start external buffer manager failed");
    }

    AVPLAY_ThreadMutex_UnLock(&pAvplay->AvplayVidThreadMutex);
    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_SetTunnelMode(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, HI_BOOL enable)
{
    HI_S32  Ret = HI_FAILURE;
    HI_U32  Id  = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    switch ((HI_U32)enChn)
    {
        case HI_UNF_AVPLAY_MEDIA_CHAN_AUD :
        case HI_UNF_AVPLAY_MEDIA_CHAN_VID :
        case HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID :
            break;

        default :
            HI_ERR_AVPLAY("para enChn 0x%x is invalid\n", enChn);
            AVPLAY_INST_UNLOCK(Id);
            return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID))
    {
        if (HI_INVALID_HANDLE == pAvplay->hVdec)
        {
            HI_ERR_AVPLAY("vid chn is close, can not start.\n");
            AVPLAY_INST_UNLOCK(Id);
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        Ret = HI_MPI_VDEC_SetChnTunnelMode(pAvplay->hVdec, (HI_U32)enable);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AVPLAY("HI_MPI_VDEC_SetChnTunnelMode fail 0x%x.\n", Ret);
            AVPLAY_INST_UNLOCK(Id);
            return HI_FAILURE;
        }
    }

    AVPLAY_INST_UNLOCK(Id);

    return Ret;
}

HI_S32 HI_MPI_AVPLAY_GetAdecOutFrameInfo(HI_HANDLE hAvplay, HI_U32 *u32OutFrames, HI_U32 *u32SampleRate)
{
    AVPLAY_S           *pAvplay;
    ADEC_BUFSTATUS_S    AdecBufStatus = {0};
    HI_U32              Id = GET_AVPLAY_ID(hAvplay);

    if(u32OutFrames == HI_NULL || u32SampleRate == HI_NULL)
    {
        HI_ERR_AVPLAY("some para is NULL\n");
        return HI_FAILURE;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (pAvplay->hAdec != HI_INVALID_HANDLE)
    {
        memset(&AdecBufStatus, 0, sizeof(AdecBufStatus));
        (HI_VOID)HI_MPI_ADEC_GetInfo(pAvplay->hAdec, HI_MPI_ADEC_BUFFERSTATUS, &AdecBufStatus);

        *u32SampleRate = 0;
        *u32OutFrames  = AdecBufStatus.u32TotDecodeFrame * AdecBufStatus.u32SamplesPerFrame;
    }
    else
    {
        *u32SampleRate = 0;
        *u32OutFrames  = 0;
    }

    AVPLAY_INST_UNLOCK(Id);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_SetAdecOutBufSize(HI_HANDLE hAvplay, HI_U32 u32OutBufFrameCnt)
{
    AVPLAY_S   *pAvplay;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    pAvplay->AdecOutBufNum = u32OutBufFrameCnt;

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_CalculateFRC(HI_HANDLE hAvplay, HI_UNF_VIDEO_FRAME_INFO_S* pstFrame,
        HI_U32 u32RefreshRate, HI_S32* ps32RepeatCnt)
{
    HI_S32                          Ret;
    HI_U32                          Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S                       *pAvplay;
    HI_UNF_AVPLAY_FRMRATE_PARAM_S   FrameRate = {0};

    if (HI_NULL == pstFrame || NULL == ps32RepeatCnt)
    {
        HI_ERR_AVPLAY("invalid parameter \n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    memset(&FrameRate, 0, sizeof(HI_UNF_AVPLAY_FRMRATE_PARAM_S));
    Ret = HI_MPI_VDEC_GetChanFrmRate(pAvplay->hVdec, &FrameRate);
    if (HI_SUCCESS != Ret)
    {
        AVPLAY_INST_UNLOCK(Id);
        HI_ERR_AVPLAY("ERR: HI_MPI_VDEC_GetChanFrmRate 0x%x\n", Ret);
        return HI_FAILURE;
    }

    if (FrameRate.enFrmRateType == HI_UNF_AVPLAY_FRMRATE_TYPE_USER)
    {
        if (   (HI_UNF_VIDEO_FIELD_TOP      == pstFrame->enFieldMode)
            || (HI_UNF_VIDEO_FIELD_BOTTOM   == pstFrame->enFieldMode) )
        {
            pAvplay->FrcParamCfg.u32InRate = (FrameRate.stSetFrmRate.u32fpsInteger * 100
                                         + FrameRate.stSetFrmRate.u32fpsDecimal / 10) * 2;
        }
        else
        {
            pAvplay->FrcParamCfg.u32InRate = FrameRate.stSetFrmRate.u32fpsInteger * 100
                                         + FrameRate.stSetFrmRate.u32fpsDecimal / 10;
        }
    }
    else
    {
        pAvplay->FrcParamCfg.u32InRate = pstFrame->stFrameRate.u32fpsInteger * 100
                                        + pstFrame->stFrameRate.u32fpsDecimal /10;
    }

    pAvplay->FrcParamCfg.u32OutRate = u32RefreshRate;

    /*do frc for every the frame*/
    (HI_VOID)AVPLAY_FrcCalculate(&pAvplay->FrcCalAlg, &pAvplay->FrcParamCfg, &pAvplay->FrcCtrlInfo);

    /* sometimes(such as pvr smooth tplay), vdec set u32PlayTime, means this frame must repeat */
    *ps32RepeatCnt = (1 + pAvplay->FrcCtrlInfo.s32FrmState);// * (1 + pstVideoPriv->u32PlayTime);

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AVPLAY_GetLeftStreamFrm(HI_HANDLE hAvplay, HI_U32 *pLeftStreamFrm)
{
#ifdef HI_NVR_SUPPORT
    HI_S32      Ret;
    HI_U32      Id = GET_AVPLAY_ID(hAvplay);
    AVPLAY_S   *pAvplay;

    AVPLAY_INST_LOCK_CHECK(hAvplay, Id);

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        AVPLAY_INST_UNLOCK(Id);
        HI_INFO_AVPLAY("vid chn is not create\n");
        return HI_FAILURE;
    }

    Ret = HI_MPI_VDEC_GetLeftStreamFrm(pAvplay->hVdec, pLeftStreamFrm);
    if (HI_SUCCESS != Ret)
    {
        AVPLAY_INST_UNLOCK(Id);
        HI_ERR_AVPLAY("HI_MPI_VDEC_GetLeftStreamFrm failed 0x%x\n", Ret);
        return Ret;
    }

    AVPLAY_INST_UNLOCK(Id);

    return HI_SUCCESS;
#else
    return HI_ERR_AVPLAY_NOT_SUPPORT;
#endif
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

