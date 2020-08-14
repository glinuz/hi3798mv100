#ifndef _DRV_SYNC_IOCTL_H_
#define _DRV_SYNC_IOCTL_H_

#include "hi_type.h"
#include "hi_drv_sync.h"
#include "hi_error_mpi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define  SYNC_MAX_NUM                   16

#define  SYS_TIME_MAX                   0xFFFFFFFFU
#define  PCR_TIME_MAX                   95443718

#define  PTS_SERIES_COUNT               2
#define  PCR_MAX_DELTA                  100

#define  PCR_LEAD_ADJUST_THRESHOLD      200
#define  PCR_LAG_ADJUST_THRESHOLD       200

#define  PCR_LEAD_STOP_THRESHOLD        100
#define  PCR_LAG_STOP_THRESHOLD         100

#define  PCR_TIMEOUTMS                  50
#define  AUD_TIMEOUTMS                  200

#define  PRE_SYNC_MIN_TIME              300
#define  BUF_FUND_TIMEOUT               200

#define  VID_LEAD_DISCARD_THRESHOLD     10000
#define  VID_LAG_DISCARD_THRESHOLD      10000

#define  AUD_RESYNC_TIMEOUT             2000  /* Temporarily reduce the timeout value
                                                 of audio resynchronization to 1000ms. 10000 */
#define  AUD_RESYNC_ADJUST_THRESHOLD    10000 /* aud resync adjust threshold */

#define  PCR_DISCARD_THRESHOLD          2000
#define  PCR_ADJUST_THRESHOLD           500

#define  SCR_DISCARD_THRESHOLD          10000

#define VID_PTS_GAP                     300
#define AUD_PTS_GAP                     200

#define VID_SMOOTH_DISCARD_INTERVAL     2     /*discard one frame in every interval frame*/
#define VID_SMOOTH_REPEAT_INTERVAL      2     /*repeat  one frame in every interval frames, including repeated frame*/

#define SYNC_FRAME_VID_EMPTY_BUFNUM     4     /* vo bufnum waterline */
#define SYNC_ES_VID_EMPTY_PERCENT       2     /* vid es buf waterline */

#define SYNC_PTS_JUMP_FRM_NUM           2

#define PTS_LOOPBACK_TIMEOUT            2000

typedef enum hiSYNC_PCR_ADJUST_E
{
    SYNC_PCR_ADJUST_SCR,
    SYNC_AUD_ADJUST_SCR,

    SYNC_SCR_ADJUST_BUTT
}SYNC_PCR_ADJUST_E;

typedef struct hiSYNC_PCR_INFO_S
{
    HI_BOOL                    PcrFirstCome;         /* arrive flag of the first pcr */
    HI_BOOL                    PcrAudSyncOK;
    HI_BOOL                    PcrVidSyncOK;
    HI_BOOL                    PcrAdjustDeltaOK;
    HI_U32                     PcrFirstSysTime;      /* arrive time of the first pcr */
    HI_U32                     PcrFirst;             /* first pcr value */
    HI_U32                     PcrLast;              /* last  pcr value*/
    HI_U32                     PcrLastSysTime;       /* last system time of setting pcr localtime*/
    HI_U32                     PcrLastLocalTime;     /* last pcr localtime */
    HI_U32                     PcrPauseLocalTime;    /* pcr localtime when pause */
    HI_U32                     PcrSeriesCnt;         /* pcr successive count*/
    HI_U32                     PcrSyncStartSysTime;  /* start time of pcr synchronization,set when sync start */
    HI_BOOL                    PcrLocalTimeFlag;     /* valid flag of pcr localtime */
    HI_BOOL                    PcrAdjustDeltaFlag;
    SYNC_PCR_ADJUST_E          enPcrAdjust;          /*which way to adjust scr*/
    HI_S32                     PcrDelta;             /*adjust pcr value*/

    HI_S32                     AudPcrDiff;           /* difference value between audio localtime and pcr localtime */
    HI_S32                     VidPcrDiff;           /* difference value between video localtime and pcr localtime */
    HI_S32                     LastVidPcrDiff;
    HI_S32                     LastAudPcrDiff;

    HI_BOOL                    PcrLoopBack;
    HI_U32                     PcrGradient;         /*gradient of pcr and system time*//**<CNcomment: PCR和系统时间比例斜率(*100)*/

}SYNC_PCRINFO_S;

typedef enum hiSYNC_BUF_STATE_E
{
    SYNC_BUF_STATE_EMPTY = 0,   /**<The buffer is idle.*//**<CNcomment: 缓冲区空闲*/
    SYNC_BUF_STATE_LOW,         /**<The buffer usage is too low.*//**<CNcomment: 缓冲区占用率过低*/
    SYNC_BUF_STATE_NORMAL,      /**<The buffer works normally.*//**<CNcomment: 缓冲区使用正常*/
    SYNC_BUF_STATE_HIGH,        /**<The buffer usage is too high.*//**<CNcomment: 缓冲区占用率过高*/
    SYNC_BUF_STATE_FULL,        /**<The buffer is full.*//**<CNcomment: 缓冲区已满*/

    SYNC_BUF_STATE_BUTT
}SYNC_BUF_STATE_E;

typedef struct hiSYNC_BUF_STATUS_S
{
    HI_U32    VidBufPercent;
    HI_U32    AudBufPercent;
    SYNC_BUF_STATE_E VidBufState;
    SYNC_BUF_STATE_E AudBufState;
    HI_BOOL   bOverflowDiscFrm;

}SYNC_BUF_STATUS_S;

/* sync event struct */
typedef struct tagSYNC_EVENT_S
{
    HI_BOOL                         bVidPtsJump;
    HI_BOOL                         bAudPtsJump;

    HI_BOOL                         bStatChange;

    HI_UNF_SYNC_PTSJUMP_PARAM_S     VidPtsJumpParam;
    HI_UNF_SYNC_PTSJUMP_PARAM_S     AudPtsJumpParam;

    HI_UNF_SYNC_STAT_PARAM_S        StatParam;

}SYNC_EVENT_S;

typedef struct tagSYNC_DEC_PTS_S
{
    HI_U32      VidDecPts;       /*<video dec pts*/
}SYNC_DEC_PTS_S;

typedef struct tagSYNC_STOP_REGION_S
{
    HI_BOOL     bUseStopRegion;  /*<use stop region or not*/
}SYNC_STOP_REGION_S;

typedef enum tagSYNC_EXT_INFO_E
{
    SYNC_EXT_INFO_FIRST_PTS,     /*<first dec pts, SYNC_PTS_INFO_S*/
    SYNC_EXT_INFO_SECOND_PTS,    /*<second dec pts, SYNC_PTS_INFO_S*/

    SYNC_EXT_INFO_STOP_REGION,   /*<Is Sync stop region enable or not, SYNC_STOP_REGION_S*/

    SYNC_EXT_INFO_BUTT
}SYNC_EXT_INFO_E;

/* sync region status enum */
typedef enum tagSYNC_REGION_STAT_E
{
    SYNC_REGION_STAT_IN_STOP,
    SYNC_REGION_STAT_IN_START,
    SYNC_REGION_STAT_IN_NOVEL,
    SYNC_REGION_STAT_IN_DISCARD,
    SYNC_REGION_STAT_OUT_DISCARD,

    SYNC_REGION_STAT_BUTT
}SYNC_REGION_STAT_E;

typedef struct hiSYNC_S
{
    HI_UNF_SYNC_ATTR_S         SyncAttr;             /* sync attributes set by user*/

    HI_BOOL                    VidEnable;            /* video enable flag, set when start video*/
    HI_BOOL                    AudEnable;            /* audio enable flag, set when start audio*/
    HI_BOOL                    AudDDPMode;           /* for DDP test only */

    /* AV common information. reset only when both of av are stoped */
    SYNC_STATUS_E              CrtStatus;            /* current status */

    SYNC_BUF_STATUS_S          CrtBufStatus;         /* current buf status*/

    SYNC_EVENT_S               SyncEvent;
    HI_U32                     LoopBackTime;
    HI_BOOL                    LoopBackFlag;

    HI_U32                     VidFirstDecPts;
    HI_U32                     VidSecondDecPts;

    HI_U32                     PreSyncStartSysTime;  /* start time of presynchronization,set when presync start */
    HI_U32                     PreSyncEndSysTime;    /* end time of presynchronization,set when presync end */
    HI_BOOL                    PreSyncFinish;        /* finish flag of presynchronization */
    SYNC_CHAN_E                PreSyncTarget;        /* presynchronization target*/
    HI_U32                     PreSyncTargetTime;    /* target time of presynchronization */
    HI_BOOL                    PreSyncTargetInit;    /* initialize flag of presynchronization target */
    HI_BOOL                    BufFundFinish;        /* finish flag of audio and video data cumulation */
    HI_U32                     BufFundEndSysTime;    /* end time of audio and video data cumulation  */
    HI_U32                     ExtPreSyncTagetTime;
    HI_BOOL                    UseExtPreSyncTaget;

    SYNC_PCRINFO_S             PcrSyncInfo;          /*some information while sync reference set HI_UNF_SYNC_REF_PCR */

    /* video statistics, reset when stop*/
    HI_BOOL                    VidFirstCome;         /* arrive flag of the first video frame */
    HI_U32                     VidFirstSysTime;      /* arrive time of the first video frame */
    HI_U32                     VidFirstPts;          /* PTS of the first video frame */
    HI_BOOL                    VidFirstValidCome;
    HI_U32                     VidFirstValidPts;
    HI_U32                     VidLastPts;           /* PTS of the last video frame*/
    HI_U32                     VidLastSrcPts;
    HI_BOOL                    VidPreSyncTargetInit; /* initialize flag of video presync target */
    HI_U32                     VidPreSyncTargetTime; /* video presync target time */
    HI_BOOL                    VidLocalTimeFlag;     /* valid flag of video localtime */
    HI_U32                     VidLastSysTime;       /* last system time of setting video localtime*/
    HI_U32                     VidLastLocalTime;     /* last video localtime */
    HI_U32                     VidPauseLocalTime;    /* video localtime when pause */
    HI_U32                     VidPtsSeriesCnt;      /* video pts successive count */
    HI_BOOL                    VidSyndAdjust;        /* adjust flag */
    // TODO: x57522  the usage of the  following three members
    HI_U32                     VidDisPlayCnt;        /* video play count when discard*/
    HI_U32                     VidDiscardCnt;        /* video discard count */
    HI_U32                     VidRepPlayCnt;        /* video play count when repeat*/
    HI_U32                     VidRepeatCnt;         /* video repeat count */
    SYNC_VID_INFO_S            VidInfo;              /* video channel information,set by vo and used by sync */
    SYNC_VID_OPT_S             VidOpt;               /* video adjust mode,set by sync */
    HI_BOOL                    VidFirstPlay;         /* played flag of the first video frame */
    HI_U32                     VidFirstPlayTime;     /* time of playing the first video frame */
    HI_S32                     VidAudDiff;           /* difference value between video localtime and audio localtime */
    HI_S32                     LastVidAudDiff;
    HI_BOOL                    VidPtsLoopBack;

    /* audio statistics,reset when stop */
    HI_BOOL                    AudFirstCome;         /* arrive flag of the first audio frame */
    HI_U32                     AudFirstSysTime;      /* arrive time of the first audio frame */
    HI_U32                     AudFirstPts;          /* pts of the first audio frame */
    HI_BOOL                    AudFirstValidCome;
    HI_U32                     AudFirstValidPts;
    HI_U32                     AudLastPts;           /* pts of the last audio frame */
    HI_U32                     AudLastSrcPts;
    HI_U32                     AudLastBufTime;       /* buftime value when last audio pts arrived */
    HI_BOOL                    AudPreSyncTargetInit; /* initialize flag of audio presync target */
    HI_U32                     AudPreSyncTargetTime; /* audio presync target time */
    HI_BOOL                    AudLocalTimeFlag;     /* valid flag of audio localtime */
    HI_U32                     AudLastSysTime;       /* last system time of setting audio localtime */
    HI_U32                     AudLastLocalTime;     /* last audio localtime */
    HI_U32                     AudPauseLocalTime;    /* audio localtime when pause */
    HI_U32                     AudPtsSeriesCnt;      /* audio pts successive count*/
    HI_BOOL                    AudReSync;            /* audio resync flag */
    HI_BOOL                    AudReBufFund;         /* audio recumulate flag */
    HI_U32                     AudPlayCnt;           /* audio play count */
    HI_U32                     AudRepeatCnt;         /* audio repeat count */
    HI_U32                     AudDiscardCnt;        /* audio discard count */
    SYNC_AUD_INFO_S            AudInfo;              /* audio channel information,set by adec and used by sync */
    SYNC_AUD_OPT_S             AudOpt;               /* audio adjust mode,set by sync */
    HI_BOOL                    AudFirstPlay;         /* played flag of the first audio frame */
    HI_U32                     AudFirstPlayTime;     /* time of playing the first audio frame */
    HI_BOOL                    AudPtsLoopBack;

    HI_U32                     ScrFirstSysTime;      /* last scr system time */
    HI_U32                     ScrFirstLocalTime;    /* last scr local time */
    HI_U32                     ScrLastSysTime;       /* last scr system time */
    HI_U32                     ScrLastLocalTime;     /* last scr local time */
    HI_U32                     ScrPauseLocalTime;    /* scr localtime when pause */
    HI_S32                     AudScrDiff;           /* difference time between audio localtime and scr localtime */
    HI_S32                     VidScrDiff;           /* difference time between vidio localtime and scr localtime */
    HI_BOOL                    ScrInitFlag;          /* scr be inited or not */

    HI_BOOL                    bPrint;

    HI_BOOL                    bUseStopRegion;       /*use stop region or not*/

#ifndef __KERNEL__
    pthread_mutex_t           *pSyncMutex;           /* mutex used to protect sync interface */
#endif

}SYNC_S;

typedef struct hiSYNC_CREATE_S
{
    HI_U32     SyncId;
    HI_U32     SyncPhyAddr;
}SYNC_CREATE_S;

typedef struct hiSYNC_USR_ADDR_S
{
    HI_U32     SyncId;
    HI_U8     *SyncUsrAddr;    /* SYNC user space address */
}SYNC_USR_ADDR_S;

/* To maintain the same memory space for the 32 bit APP to 64 bit system */
typedef struct hiSYNC_COMPAT_USR_ADDR_S
{
    HI_U32     SyncId;
    HI_U32     SyncUsrAddr;    /* SYNC user space address */
}SYNC_COMPAT_USR_ADDR_S;

typedef struct hiSYNC_AUD_JUDGE_S
{
    HI_HANDLE         hSync;
    SYNC_AUD_INFO_S   AudInfo;
    SYNC_AUD_OPT_S    AudOpt;
}SYNC_AUD_JUDGE_S;

typedef struct hiSYNC_VID_JUDGE_S
{
    HI_HANDLE           hSync;
    SYNC_VID_INFO_S     VidInfo;
    SYNC_VID_OPT_S      VidOpt;
}SYNC_VID_JUDGE_S;

typedef struct hiSYNC_GET_TIME_S
{
    HI_U32             SyncId;
    HI_U32             LocalTime;
    HI_U32             PlayTime;
}SYNC_GET_TIME_S;

typedef struct hiSYNC_INFO_S
{
    SYNC_S     *pSync;           /* SYNC kernel space pointer */
    HI_VOID    *File;            /* SYNC process handle */
    HI_U32     SyncPhyAddr;      /* SYNC physical address*/
    HI_U8      *SyncUsrAddr;     /* SYNC usr space address*/
}SYNC_INFO_S;

typedef  HI_S32 ( *SyncManage)(HI_HANDLE *hSyncID);

typedef struct hiSYNC_GLOBAL_STATE_S
{
    SYNC_INFO_S   SyncInfo[SYNC_MAX_NUM];
    HI_U32        SyncCount;
    SyncManage    AddSyncIns;
    SyncManage    DelSyncIns;
}SYNC_GLOBAL_STATE_S;


typedef enum hiIOC_SYNC_E
{
    IOC_SYNC_GET_NUM = 0,

    IOC_SYNC_CREATE,
    IOC_SYNC_DESTROY,

    IOC_SYNC_CHECK_ID,
    IOC_SYNC_SET_USRADDR,
    IOC_SYNC_CHECK_NUM,

    IOC_SYNC_START_SYNC,
    IOC_SYNC_AUD_JUDGE,
    IOC_SYNC_VID_JUDGE,
    IOC_SYNC_PAUSE_SYNC,
    IOC_SYNC_RESUME_SYNC,
    IOC_SYNC_GET_TIME,

    IOC_SYNC_SET_BUTT
}IOC_SYNC_E;


#define CMD_SYNC_CREATE                _IOWR(HI_ID_SYNC, IOC_SYNC_CREATE, SYNC_CREATE_S)
#define CMD_SYNC_DESTROY               _IOW(HI_ID_SYNC, IOC_SYNC_DESTROY, HI_U32)

#define CMD_SYNC_CHECK_ID              _IOWR(HI_ID_SYNC, IOC_SYNC_CHECK_ID, SYNC_USR_ADDR_S)
#define CMD_SYNC_COMPAT_CHECK_ID       _IOWR(HI_ID_SYNC, IOC_SYNC_CHECK_ID, SYNC_COMPAT_USR_ADDR_S)
#define CMD_SYNC_SET_USRADDR           _IOW(HI_ID_SYNC, IOC_SYNC_SET_USRADDR, SYNC_USR_ADDR_S)
#define CMD_SYNC_COMPAT_SET_USRADDR    _IOW(HI_ID_SYNC, IOC_SYNC_SET_USRADDR, SYNC_COMPAT_USR_ADDR_S)
#define CMD_SYNC_CHECK_NUM             _IOWR(HI_ID_SYNC, IOC_SYNC_CHECK_NUM, HI_U32)

#define CMD_SYNC_START_SYNC            _IOW(HI_ID_SYNC, IOC_SYNC_START_SYNC, HI_U32)
#define CMD_SYNC_AUD_JUDGE             _IOWR(HI_ID_SYNC, IOC_SYNC_AUD_JUDGE, SYNC_AUD_JUDGE_S)
#define CMD_SYNC_VID_JUDGE             _IOWR(HI_ID_SYNC, IOC_SYNC_VID_JUDGE, SYNC_VID_JUDGE_S)
#define CMD_SYNC_PAUSE_SYNC            _IOW(HI_ID_SYNC, IOC_SYNC_PAUSE_SYNC, HI_U32)
#define CMD_SYNC_RESUME_SYNC           _IOW(HI_ID_SYNC, IOC_SYNC_RESUME_SYNC, HI_U32)
#define CMD_SYNC_GET_TIME              _IOWR(HI_ID_SYNC, IOC_SYNC_GET_TIME, SYNC_GET_TIME_S)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

