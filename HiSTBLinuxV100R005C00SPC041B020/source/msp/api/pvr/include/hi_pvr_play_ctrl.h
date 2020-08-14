/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_pvr_play_ctrl.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/14
  Description   :
  History       :
  1.Date        : 2008/04/14
    Author      : q46153
    Modification: Created file

******************************************************************************/

#ifndef __HI_PVR_PLAY_CTRL_H__
#define __HI_PVR_PLAY_CTRL_H__

#include "hi_type.h"
#include "hi_pvr_fifo.h"
#include "hi_pvr_index.h"
#include "hi_drv_pvr.h"
#include "hi_pvr_rec_ctrl.h"
#ifdef HI_TEE_PVR_SUPPORT
#include "hi_pvr_tee.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


#define PVR_PLAY_DMX_GET_BUF_TIME_OUT 5000  /* ms */


/*whether move the short pakcet less than 188 byte to the end of TS or not.
    0:not move, and after that fill it with 0xff
    1:move it, and before that fill it with 0xff 
*/
#define PVR_TS_MOVE_TO_END   1


#define PVR_PLAY_DO_NOT_MARK_DISPLAY 0xffU

#define PVR_PLAY_STEP_WATI_TIME   1000UL  /* ms */

#define PVR_PLAY_MAX_FRAME_SIZE  (1024*1024*10)   /* the size of max frame */

#define PVR_PLAY_PICTURE_HEADER_LEN  4          /* the length of picture header ID, in byte */


#define WHENCE_STRING(whence)   ((0 == (whence)) ? "SEEK_SET" : ((1 == (whence)) ? "SEEK_CUR" : "SEEK_END"))

#define PVR_TIME_CTRL_INTERVAL 1000 

#define PVR_DEFAULT_FRAME_BUFF_NUM  6
#define PVR_VO_FRMBUFF_NUM_OF_DISABLE_DEI  3
#define PVR_VO_FRMBUFF_NUM_OF_ENABLE_DEI   7

#define PVR_ENABLE_DISP_OPTIMIZE  1
#define PVR_DISABLE_DISP_OPTIMIZE 0

/* check channel validity                                                   */
#define PVR_PLAY_CHECK_CHN(u32Chn)\
    do {\
        if (u32Chn >= PVR_PLAY_MAX_CHN_NUM )\
        {\
            HI_ERR_PVR("play chn(%u) id invalid!\n", u32Chn);\
            return HI_ERR_PVR_INVALID_CHNID;\
        }\
        if(g_stPvrPlayChns[u32Chn].enState == HI_UNF_PVR_PLAY_STATE_INVALID)\
        {\
            HI_ERR_PVR("play chn(%u) state(%d) invalid!\n", u32Chn, g_stPvrPlayChns[u32Chn].enState);\
            return HI_ERR_PVR_CHN_NOT_INIT;\
        }\
        if( g_stPlayInit.bInit == HI_FALSE)\
        {\
            HI_ERR_PVR("pvr is not init!\n");\
            return HI_ERR_PVR_NOT_INIT;\
        }\
    }while(0)

/* check play module initialized                                            */
#define PVR_PLAY_CHECK_INIT(pCommAttr)\
    do {\
        if (HI_FALSE == (pCommAttr)->bInit)\
        {\
            HI_ERR_PVR("play not inti yet!\n");\
            return HI_ERR_PVR_NOT_INIT;\
        }\
    }while(0)

#define PVR_PLAY_CHECK_CHN_INIT(enState)\
            do\
            {\
                if (HI_UNF_PVR_PLAY_STATE_INVALID ==  enState )\
                {\
                    return HI_ERR_PVR_CHN_NOT_INIT;\
                }\
            } while (0)

#define PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr)\
                    do\
                    {\
                        if (HI_UNF_PVR_PLAY_STATE_INVALID ==  pChnAttr->enState )\
                        {\
                            PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));\
                            return HI_ERR_PVR_CHN_NOT_INIT;\
                        }\
                    } while (0)

/* PVR ts file read.
return pointer offset forward on success.
otherwise, return the file header.*/
#define  PVR_PLAY_READ_FILE(pu8Addr, offset, size, pChnAttr) \
            do \
            {\
                ssize_t  n;\
                if ((n = PVR_PREAD64(pu8Addr, (size), \
                            pChnAttr->s32DataFile, (offset))) == -1)\
                {\
                    if (NULL != &errno)\
                    {\
                        if (EINTR == errno)\
                        {\
                            continue;\
                        }\
                        else if (errno)\
                        { \
                            return HI_ERR_PVR_FILE_CANT_READ;\
                        }\
                        else\
                        {\
                            HI_ERR_PVR("read err1,  want:%u, off:%llu \n", (size), offset);\
                            return HI_ERR_PVR_FILE_TILL_END;\
                        }\
                    }\
                }\
                if ((0 == n) && (0 != (size)))\
                {\
                    HI_WARN_PVR("read 0,  want:%u, off:%llu \n", (size), offset);\
                    return HI_ERR_PVR_FILE_TILL_END;\
                }\
           }while(0)

#define PVR_GET_STATE_BY_SPEED(state, speed) \
do {\
    switch (speed)\
    {\
        case HI_UNF_PVR_PLAY_SPEED_NORMAL           :\
            state = HI_UNF_PVR_PLAY_STATE_PLAY;\
            break;\
        case HI_UNF_PVR_PLAY_SPEED_2X_FAST_FORWARD  :\
        case HI_UNF_PVR_PLAY_SPEED_4X_FAST_FORWARD  :\
        case HI_UNF_PVR_PLAY_SPEED_8X_FAST_FORWARD  :\
        case HI_UNF_PVR_PLAY_SPEED_16X_FAST_FORWARD :\
        case HI_UNF_PVR_PLAY_SPEED_32X_FAST_FORWARD :\
        case HI_UNF_PVR_PLAY_SPEED_64X_FAST_FORWARD :\
            state = HI_UNF_PVR_PLAY_STATE_FF;\
            break;\
        case HI_UNF_PVR_PLAY_SPEED_1X_FAST_BACKWARD :\
        case HI_UNF_PVR_PLAY_SPEED_2X_FAST_BACKWARD :\
        case HI_UNF_PVR_PLAY_SPEED_4X_FAST_BACKWARD :\
        case HI_UNF_PVR_PLAY_SPEED_8X_FAST_BACKWARD :\
        case HI_UNF_PVR_PLAY_SPEED_16X_FAST_BACKWARD:\
        case HI_UNF_PVR_PLAY_SPEED_32X_FAST_BACKWARD:\
        case HI_UNF_PVR_PLAY_SPEED_64X_FAST_BACKWARD:\
            state = HI_UNF_PVR_PLAY_STATE_FB;\
            break;\
        case HI_UNF_PVR_PLAY_SPEED_2X_SLOW_FORWARD  :\
        case HI_UNF_PVR_PLAY_SPEED_4X_SLOW_FORWARD  :\
        case HI_UNF_PVR_PLAY_SPEED_8X_SLOW_FORWARD  :\
        case HI_UNF_PVR_PLAY_SPEED_16X_SLOW_FORWARD :\
        case HI_UNF_PVR_PLAY_SPEED_32X_SLOW_FORWARD :\
        case HI_UNF_PVR_PLAY_SPEED_64X_SLOW_FORWARD :\
            state = HI_UNF_PVR_PLAY_STATE_SF;\
            break;\
        case HI_UNF_PVR_PLAY_SPEED_2X_SLOW_BACKWARD :\
        case HI_UNF_PVR_PLAY_SPEED_4X_SLOW_BACKWARD :\
        case HI_UNF_PVR_PLAY_SPEED_8X_SLOW_BACKWARD :\
        case HI_UNF_PVR_PLAY_SPEED_16X_SLOW_BACKWARD:\
        case HI_UNF_PVR_PLAY_SPEED_32X_SLOW_BACKWARD:\
        case HI_UNF_PVR_PLAY_SPEED_64X_SLOW_BACKWARD:\
            state = HI_UNF_PVR_PLAY_STATE_SB;\
            break;\
        default:\
            state = HI_UNF_PVR_PLAY_STATE_INVALID;\
            break;\
    }\
}while(0)

/* common information for play module                                      */
typedef struct hiPVR_PLAY_COMM_S
{
    HI_BOOL bInit;
    HI_S32  s32Reserved;
} PVR_PLAY_COMM_S;

/* attributes of play channel                                               */
typedef struct hiPVR_PLAY_CHN_S
{
    HI_U32           u32chnID;
    HI_U32           u32CurPlayTimeMs;
    HI_U32           u32GopNumOfStart;
    HI_U32           u32UseDecodeAbility;    /* decode ability consumed by play chn */
    HI_U64           u64CurReadPos;          /* current data file read position */  
    HI_U64           u64TsFileSize;          /* the size of ts file, to control the end for playing without index file */
    PVR_FILE64       s32DataFile;             /* descriptor of play file */
    HI_U32           u32LastDispTimeMs;
    HI_U32           u32CurDispTimeMs;
    
    HI_BOOL          bTimeShiftStartFlg;
    HI_BOOL          bRecordedVideoExist;        /*use for judgement of call avplay invoke with cmd = HI_UNF_AVPLAY_INVOKE_VCODEC and some other vedio corresponede cmd*/
    HI_BOOL          bNotAvailableTsBuff;
    HI_BOOL          bIndexBeyondTs;
    HI_BOOL          bPlayMainThreadStop;
    HI_BOOL          bEndOfFile;             /* playing to EOF */
    HI_BOOL          bEofEvent;
    HI_BOOL          bStartOfFile;           /* backward to start of file */
    HI_BOOL          bFlushingFlag;          /* the state of flushing but not eof*/
    HI_BOOL          bPlayingTsNoIdx;

    HI_HANDLE        hAvplay;                 /* avplay handle */
    HI_HANDLE        hTsBuffer;               /* TS buffer handle */
    HI_HANDLE        hCipher;                 /* cipher handle */
    PVR_INDEX_HANDLE IndexHandle;             /* index handle */

    pthread_t        stPlayMainThread;        /* play thread id   */
    pthread_mutex_t  stPlayChnMutex;
    
    HI_UNF_SYNC_REF_E       enLastSyncState;
    HI_UNF_PVR_PLAY_STATE_E enState;                 /* play state */
    HI_UNF_PVR_PLAY_SPEED_E enSpeed;
    HI_PVR_SMOOTH_PARA_S stFBAttr;
    HI_UNF_PVR_PLAY_ATTR_S        stUserCfg;               /* play attributes for user configure */
    HI_UNF_PVR_PLAY_STATUS_S      stLastStatus;     /* the last play status, when failure to get current play status, return this */

#ifdef HI_PVR_EXTRA_BUF_SUPPORT
    PVR_REC_TS_DATA_QUEUE_S *pstRecDataQueue;
#endif
#ifdef  HI_TEE_PVR_SUPPORT
#if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    HI_BOOL bSecTsBuffer;
    PVR_REE_BUFFER_INFO_S stReeBuf;
#endif    
#endif

    HI_U64                 u64SendFrameNum;
    ExtraCallBack          readCallBack;
    PVR_EXTEND_CALLBACK_S  stPlayCallback;
} PVR_PLAY_CHN_S;


HI_S32 HI_PVR_PlayRegisterReadCallBack(HI_U32 u32Chn, ExtraCallBack readCallBack);

HI_S32 HI_PVR_PlayUnRegisterReadCallBack(HI_U32 u32Chn);

HI_S32 HI_PVR_PlayRegisterPlayCallBack(HI_U32 u32Chn, ExtendCallBack_New playCallBack, HI_VOID *pUserData);

HI_S32 HI_PVR_PlayUnRegisterPlayCallBack(HI_U32 u32Chn, ExtendCallBack_New pfnPlayCallback);

HI_BOOL PVRPlayCheckInitStat(HI_VOID);

void PVRPlaySyncTrickPlayTime(PVR_PLAY_CHN_S *pChnAttr);
#ifdef HI_PVR_EXTRA_BUF_SUPPORT
#ifndef PVR_ACQUIREDATAINDX
HI_BOOL PVRPlayCheckSlowPauseBack(const HI_CHAR *pFileName);
#endif
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifdef __HI_PVR_PLAY_CTRL_H__ */

