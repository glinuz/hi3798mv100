/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_pvr_rec_ctrl.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/10
  Description   :
  History       :
  1.Date        : 2008/04/10
    Author      : q46153
    Modification: Created file

******************************************************************************/

#ifndef __HI_PVR_REC_CTRL_H__
#define __HI_PVR_REC_CTRL_H__

#include "hi_pvr_priv.h"
#include "hi_pvr_index.h"
#include "hi_pvr_fifo.h"
#include "hi_drv_pvr.h"
#include "hi_mpi_demux.h"
#ifdef HI_TEE_PVR_SUPPORT
#include "hi_pvr_tee.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


#define PVR_REC_MIN_DMXID (0)
#define PVR_REC_MAX_DMXID (6)

#define PVR_REC_DMX_GET_SC_TIME_OUT     1000  /* ms */
#define PVR_REC_DMX_GET_STREAM_TIME_OUT 1500  /* ms */

#define PVR_REC_APPEND_LEN  (PVR_TS_LEN * 2)

#define CHECK_REC_CHNID(u32ChnID)\
    do\
    {\
        if ((u32ChnID < PVR_REC_START_NUM) || (u32ChnID >= (PVR_REC_MAX_CHN_NUM + PVR_REC_START_NUM)))\
        {\
           HI_ERR_PVR("Rec chn(%u) id invalid!\n", u32ChnID);\
           return HI_ERR_PVR_INVALID_CHNID;\
        }\
        if(g_stPvrRecChns[u32ChnID - PVR_REC_START_NUM].enState == HI_UNF_PVR_REC_STATE_INVALID)\
        {\
            HI_ERR_PVR("Rec chn(%u) state(%d) invalid!\n", u32ChnID, g_stPvrRecChns[u32ChnID - PVR_REC_START_NUM].enState);\
            return HI_ERR_PVR_CHN_NOT_INIT;\
        }\
        if( g_stRecInit.bInit == HI_FALSE)\
        {\
            HI_ERR_PVR("pvr rec is not init!\n");\
            return HI_ERR_PVR_NOT_INIT;\
        }\
    } while (0)

#define CHECK_REC_INIT(pCommAttr)\
    do\
    {\
        if ((pCommAttr)->bInit != HI_TRUE)\
        {\
            HI_ERR_PVR("Record Module is not Initialized!\n");\
            return HI_ERR_PVR_NOT_INIT;\
        }\
    } while (0)

#define CHECK_REC_DEMUX_ID(DemuxID)\
    do\
    {\
        if (DemuxID > PVR_REC_MAX_DMXID || DemuxID < PVR_REC_MIN_DMXID)\
        {\
            return HI_ERR_PVR_REC_INVALID_DMXID;\
        }\
    } while (0)

#define CHECK_REC_CHN_INIT(enState)\
    do\
    {\
        if (HI_UNF_PVR_REC_STATE_INVALID ==  enState )\
        {\
            return HI_ERR_PVR_CHN_NOT_INIT;\
        }\
    } while (0)

#define CHECK_REC_CHN_INIT_UNLOCK(pRecChnAttr)\
            do\
            {\
                if (HI_UNF_PVR_REC_STATE_INVALID ==  pRecChnAttr->enState )\
                {\
                    PVR_UNLOCK(&(pRecChnAttr->stMutex));\
                    return HI_ERR_PVR_CHN_NOT_INIT;\
                }\
            } while (0)


/* common information for record module                                     */
typedef struct hiPVR_REC_COMM_S
{
    HI_BOOL             bInit ;                             /* module init flag */
    HI_S32              s32Reserved;                        /* reserved */
} PVR_REC_COMM_S;

typedef struct hiPVR_SCD_INFO_S
{
    HI_U32 ScdCnt;
    DMX_IDX_DATA_S ScdBuffer[20];
}PVR_SCD_INFO_S;

typedef struct hiPVR_EXTEND_CALLBACK_S
{
    ExtendCallBack_New pfnCallback;
    HI_VOID *pUserData;
}PVR_EXTEND_CALLBACK_S;

#ifdef HI_PVR_EXTRA_BUF_SUPPORT

#define PVR_REC_TS_QUEUE_EMPTY 0
#define PVR_REC_TS_QUEUE_FULL  1

#define PVR_REC_TS_QUEUE_BLOCK (0x2F000)/* write 188K one time */

#define PvrRecTsQueueIsEmpty(q) (((q)->head == (q)->tail) && ((q)->flag == PVR_REC_TS_QUEUE_EMPTY))
#define PvrRecTsQueueIsFull(q) (((q)->head == (q)->tail) && ((q)->flag == PVR_REC_TS_QUEUE_FULL))  


typedef struct hiPVR_REC_TS_DATA_QUEUE
{  
   HI_U8 *pu8Head; 
   HI_U8 *pu8Tail;   
   HI_U32 u32Flag;   
   HI_U32 u32Size; 
   HI_U8 *pu8RecData;
   HI_U64 u64HeadGolbalOffset;
   HI_U64 u64TailGolbalOffset;
}PVR_REC_TS_DATA_QUEUE_S;  

#endif

#ifdef HI_TEE_PVR_SUPPORT
typedef struct hiPVR_REC_SEC_OS_STATE_S
{
    HI_U32 u32TeeBufAddr;
    HI_U32 u32TeeBufLen;
    HI_U32 u32ReeBufAddr;
    HI_U32 u32ReeBufLen;
    HI_U32 u32TeeReadPos;
    HI_U32 u32TeeWritePos;
    HI_U64 u64DataGlobalOffset;
    HI_U64 u64BaseGlobalOffset;
    HI_U64 u64NextOldOffset;
    HI_U64 u64NextAdjustOffset;
    HI_S32 s32InsertTsPacketNum;
    HI_BOOL bUsedNext;
}PVR_REC_SEC_OS_STATE_S;
#endif


/* attributes of record channel                                             */
typedef struct hiPVR_REC_CHN_S
{
    HI_U32                  u32ChnID;
    HI_HANDLE               hCipher;                   /* cipher handle */
    PVR_INDEX_HANDLE        IndexHandle;               /* index handle */

    HI_UNF_PVR_REC_ATTR_S   stUserCfg;                 /* record attributes for user configure */

    volatile HI_U64         u64CurFileSize;            /* current size of record file, included rewind */
    volatile HI_U64         u64CurWritePos;            /*current write position*/
    HI_UNF_PVR_REC_STATE_E  enState;                   /* record state */
    PVR_FILE64              dataFile;                  /* descriptor of record file */

    pthread_t               RecordCacheThread;         /* record thread pids */
    pthread_t               RecordFileThread;

    HI_BOOL                 bSavingData;               /* whether saving data or not */
    HI_S32                  s32OverFixTimes;
    HI_BOOL                 bEventFlg;                 /* Event flag */

    pthread_mutex_t         stMutex;

    struct timespec         tv_start;
    struct timespec         tv_stop;
        
    ExtraCallBack           writeCallBack;
    HI_U32                  u32RecStartTimeMs;
    HI_HANDLE               DemuxRecHandle;
    PVR_EVENT_HISTORY_S     stEventHistory;
#ifdef HI_PVR_EXTRA_BUF_SUPPORT
    PVR_REC_TS_DATA_QUEUE_S stRecTsQueue;
    #ifndef PVR_ACQUIREDATAINDX
    HI_BOOL                 bUseLastDmxIdx;
    HI_MPI_DMX_REC_INDEX_S  stDmxIdxInfo;
    #endif
#endif
    
    PVR_EXTEND_CALLBACK_S  stRecCallback;
#ifdef HI_TEE_PVR_SUPPORT
    PVR_REE_BUFFER_INFO_S stReeBuff;
    PVR_REE_BUFFER_INFO_S stTeeState;
    HI_U32 u32TeeChnId;
#endif
} PVR_REC_CHN_S;

extern HI_BOOL PVRRecCheckFileRecording(const HI_CHAR *pFileName);

extern HI_BOOL PVRRecCheckChnRecording(HI_U32 u32ChnID);

extern HI_S32 PVRRecMarkPausePos(HI_U32 u32ChnID);

extern HI_BOOL PVRRecCheckInitStat(void);

HI_S32 HI_PVR_RecRegisterWriteCallBack(HI_U32 u32ChnID, ExtraCallBack writeCallBack);

HI_S32 HI_PVR_RecUnRegisterWriteCallBack(HI_U32 u32ChnID);

HI_S32 HI_PVR_RecRegisterRecordCallBack(HI_U32 u32ChnID, ExtendCallBack_New RecCallBack, HI_VOID *pUserData);

HI_S32 HI_PVR_RecUnRegisterRecCallBack(HI_U32 u32ChnID, ExtendCallBack_New pRecCallback);

PVR_REC_CHN_S* PVRRecGetChnAttrByName(const HI_CHAR *pFileName);
#ifdef HI_PVR_EXTRA_BUF_SUPPORT
PVR_REC_CHN_S *PVRRecGetChnByIdxHandle(PVR_INDEX_HANDLE hIndex);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifdef __HI_PVR_H__ */

