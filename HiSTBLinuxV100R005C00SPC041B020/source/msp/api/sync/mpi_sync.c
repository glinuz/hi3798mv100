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

#include "hi_mpi_sync.h"
#include "hi_module.h"
#include "hi_mpi_mem.h"
#include "hi_drv_struct.h"
#include "drv_sync_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


//static HI_U32 u32SynMutexCount = 0;
void SYNC_Mutex_Lock(pthread_mutex_t *ss)
{
    //u32SynMutexCount ++;
    //HI_INFO_SYNC("lock u32SynMutexCount:%d\n", u32SynMutexCount);
    (HI_VOID)pthread_mutex_lock(ss);
}

void SYNC_Mutex_UnLock(pthread_mutex_t *ss)
{
    //u32SynMutexCount --;
    //HI_INFO_SYNC("unlock u32SynMutexCount:%d\n", u32SynMutexCount);
    (HI_VOID)pthread_mutex_unlock(ss);
}

static HI_S32            g_SyncDevFd    = -1;
static const HI_CHAR     g_SyncDevName[] ="/dev/"UMAP_DEVNAME_SYNC;
static pthread_mutex_t   g_SyncMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_SYNC_LOCK()       (void)pthread_mutex_lock(&g_SyncMutex);
#define HI_SYNC_UNLOCK()     (void)pthread_mutex_unlock(&g_SyncMutex);

#define CHECK_SYNC_INIT()\
do{\
    HI_SYNC_LOCK();\
    if (g_SyncDevFd < 0)\
    {\
        HI_ERR_SYNC("SYNC is not init.\n");\
        HI_SYNC_UNLOCK();\
        return HI_ERR_SYNC_DEV_NO_INIT;\
    }\
    HI_SYNC_UNLOCK();\
}while(0)

HI_S32 SYNC_CheckHandle(HI_HANDLE hSync, SYNC_USR_ADDR_S  *pSyncUsrAddr)
{
    pSyncUsrAddr->SyncId = hSync & 0xff;

    return ioctl(g_SyncDevFd, CMD_SYNC_CHECK_ID, pSyncUsrAddr);
}

HI_S32 SYNC_GetNum(HI_U32 *pSyncyNum)
{
    return ioctl(g_SyncDevFd, CMD_SYNC_CHECK_NUM, pSyncyNum);
}

HI_VOID SYNC_ResetStatInfo(SYNC_S *pSync, SYNC_CHAN_E enChn)
{
    if (enChn == SYNC_CHAN_VID)
    {
        pSync->VidFirstCome = HI_FALSE;
        pSync->VidFirstSysTime = HI_INVALID_TIME;
        pSync->VidFirstPts = HI_INVALID_PTS;
        pSync->VidLastPts = HI_INVALID_PTS;
        pSync->VidPreSyncTargetInit = HI_FALSE;
        pSync->VidPreSyncTargetTime = HI_INVALID_TIME;
        pSync->VidLocalTimeFlag = HI_FALSE;
        pSync->VidLastSysTime = HI_INVALID_TIME;
        pSync->VidLastLocalTime = HI_INVALID_TIME;
        pSync->VidPauseLocalTime = HI_INVALID_TIME;
        pSync->VidPtsSeriesCnt = 0;

        pSync->CrtBufStatus.VidBufState = SYNC_BUF_STATE_NORMAL;
        pSync->CrtBufStatus.VidBufPercent = 0;
        pSync->CrtBufStatus.bOverflowDiscFrm = HI_FALSE;
        pSync->VidSyndAdjust = HI_FALSE;
        pSync->VidDisPlayCnt = 0;
        pSync->VidDiscardCnt = 0;
        pSync->VidRepPlayCnt = 0;
        pSync->VidRepeatCnt = 0;
        pSync->VidFirstPlay = HI_FALSE;
        pSync->VidFirstPlayTime = HI_INVALID_TIME;
        pSync->VidInfo.SrcPts = HI_INVALID_PTS;
        pSync->VidInfo.Pts = HI_INVALID_PTS;
        pSync->VidInfo.FrameTime = 40;
        pSync->VidInfo.DelayTime = 20;
        pSync->VidInfo.DispTime = 1;

        pSync->VidFirstValidCome = HI_FALSE;
        pSync->VidFirstValidPts = HI_INVALID_PTS;
        pSync->VidLastSrcPts = HI_INVALID_PTS;
        pSync->VidPtsLoopBack = HI_FALSE;

        pSync->VidFirstDecPts = HI_INVALID_PTS;
        pSync->VidSecondDecPts = HI_INVALID_PTS;
    }

    if (enChn == SYNC_CHAN_AUD)
    {
        pSync->AudFirstCome = HI_FALSE;
        pSync->AudFirstSysTime = HI_INVALID_TIME;
        pSync->AudFirstPts = HI_INVALID_PTS;
        pSync->AudLastPts = HI_INVALID_PTS;
        pSync->AudLastBufTime = 0;
        pSync->AudPreSyncTargetInit = HI_FALSE;
        pSync->AudPreSyncTargetTime = HI_INVALID_TIME;
        pSync->AudLocalTimeFlag = HI_FALSE;
        pSync->AudLastSysTime = HI_INVALID_TIME;
        pSync->AudLastLocalTime = HI_INVALID_TIME;
        pSync->AudPauseLocalTime = HI_INVALID_TIME;
        pSync->AudPtsSeriesCnt = 0;

        pSync->CrtBufStatus.AudBufState = SYNC_BUF_STATE_NORMAL;
        pSync->CrtBufStatus.AudBufPercent = 0;

        pSync->AudReSync = HI_TRUE;
        pSync->AudReBufFund = HI_TRUE;
        pSync->AudPlayCnt = 0;
        pSync->AudDiscardCnt = 0;
        pSync->AudRepeatCnt = 0;
        pSync->AudInfo.SrcPts = HI_INVALID_PTS;
        pSync->AudInfo.Pts = HI_INVALID_PTS;
        pSync->AudInfo.FrameTime = 24;
        pSync->AudInfo.BufTime = 0;
        pSync->AudInfo.FrameNum = 0;
        pSync->AudFirstPlay = HI_FALSE;
        pSync->AudFirstPlayTime = HI_INVALID_TIME;

        pSync->AudFirstValidCome = HI_FALSE;
        pSync->AudFirstValidPts = HI_INVALID_PTS;
        pSync->AudLastSrcPts = HI_INVALID_PTS;
        pSync->AudPtsLoopBack = HI_FALSE;
    }

    if ((!pSync->VidEnable)
      &&(!pSync->AudEnable)
       )
    {
        pSync->CrtStatus = SYNC_STATUS_STOP;
        pSync->PreSyncStartSysTime = HI_INVALID_TIME;
        pSync->PreSyncEndSysTime = HI_INVALID_TIME;
        pSync->PreSyncFinish = HI_FALSE;
        pSync->BufFundEndSysTime = HI_INVALID_TIME;
        pSync->BufFundFinish = HI_FALSE;
        pSync->PreSyncTarget = SYNC_CHAN_VID;
        pSync->PreSyncTargetTime = HI_INVALID_TIME;
        pSync->PreSyncTargetInit = HI_FALSE;
        pSync->ExtPreSyncTagetTime = HI_INVALID_TIME;
        pSync->UseExtPreSyncTaget = HI_FALSE;

        pSync->PcrSyncInfo.PcrFirstCome = HI_FALSE;
        pSync->PcrSyncInfo.PcrAdjustDeltaOK = HI_TRUE;
        pSync->PcrSyncInfo.PcrFirstSysTime = HI_INVALID_TIME;
        pSync->PcrSyncInfo.PcrFirst = HI_INVALID_PTS;
        pSync->PcrSyncInfo.PcrLast = HI_INVALID_PTS;
        pSync->PcrSyncInfo.PcrLastSysTime = HI_INVALID_TIME;
        pSync->PcrSyncInfo.PcrLastLocalTime = HI_INVALID_TIME;
        pSync->PcrSyncInfo.PcrPauseLocalTime = HI_INVALID_TIME;
        pSync->PcrSyncInfo.PcrLocalTimeFlag = HI_FALSE;
        pSync->PcrSyncInfo.PcrSeriesCnt = 0;
        pSync->PcrSyncInfo.PcrSyncStartSysTime = HI_INVALID_TIME;
        pSync->PcrSyncInfo.PcrDelta = 0;
        pSync->PcrSyncInfo.enPcrAdjust = SYNC_SCR_ADJUST_BUTT;
        pSync->PcrSyncInfo.PcrLoopBack = HI_FALSE;

        pSync->ScrInitFlag = HI_FALSE;
        pSync->ScrFirstLocalTime = HI_INVALID_TIME;
        pSync->ScrFirstSysTime = HI_INVALID_TIME;
        pSync->ScrLastLocalTime = HI_INVALID_TIME;
        pSync->ScrLastSysTime = HI_INVALID_TIME;

        pSync->AudReSync = HI_FALSE;
        pSync->AudReBufFund = HI_FALSE;

        pSync->SyncEvent.bAudPtsJump = HI_FALSE;
        pSync->SyncEvent.bVidPtsJump = HI_FALSE;
        pSync->SyncEvent.bStatChange = HI_FALSE;
        pSync->LoopBackTime = HI_INVALID_TIME;
        pSync->LoopBackFlag = HI_FALSE;
    }

    return;
}


HI_S32 HI_MPI_SYNC_Init(HI_VOID)
{
    HI_S32 s32val;

    HI_SYNC_LOCK();

    /* already opened in this process */
    if (g_SyncDevFd > 0)
    {
        HI_SYNC_UNLOCK();
        return HI_SUCCESS;
    }

    g_SyncDevFd = open(g_SyncDevName, O_RDWR|O_NONBLOCK, 0);

    if (g_SyncDevFd < 0)
    {
        HI_FATAL_SYNC("open SYNC err.\n");
        HI_SYNC_UNLOCK();
        return HI_ERR_SYNC_DEV_OPEN_ERR;
    }

    s32val=fcntl(g_SyncDevFd,F_GETFD);
    if (s32val > 0)
    {
        s32val|=FD_CLOEXEC;
        (HI_VOID)fcntl(g_SyncDevFd,F_SETFD,s32val);
    }

    HI_SYNC_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_DeInit(HI_VOID)
{
    HI_S32  Ret;
    HI_U32  SyncNum = 0;

    HI_SYNC_LOCK();

    if (g_SyncDevFd < 0)
    {
        HI_SYNC_UNLOCK();
        return HI_SUCCESS;
    }

    SYNC_GetNum(&SyncNum);

    if (SyncNum)
    {
        HI_FATAL_SYNC("there are %d SYNC not been destroied.\n", SyncNum);
        HI_SYNC_UNLOCK();
        return HI_ERR_SYNC_INVALID_OPT;
    }

    Ret = close(g_SyncDevFd);

    if(HI_SUCCESS != Ret)
    {
        HI_FATAL_SYNC("DeInit AVPLAY err.\n");
        HI_SYNC_UNLOCK();
        return HI_ERR_SYNC_DEV_CLOSE_ERR;
    }

    g_SyncDevFd = -1;

    HI_SYNC_UNLOCK();

    return HI_SUCCESS;
}

HI_VOID HI_MPI_SYNC_GetDefaultAttr(HI_UNF_SYNC_ATTR_S *pstSyncAttr)
{
    pstSyncAttr->enSyncRef = HI_UNF_SYNC_REF_AUDIO;

    pstSyncAttr->stSyncStartRegion.s32VidPlusTime = 100;
    pstSyncAttr->stSyncStartRegion.s32VidNegativeTime = -100;
    pstSyncAttr->stSyncStartRegion.bSmoothPlay = HI_TRUE;

    pstSyncAttr->stSyncNovelRegion.s32VidPlusTime = 3000;
    pstSyncAttr->stSyncNovelRegion.s32VidNegativeTime = -3000;
    pstSyncAttr->stSyncNovelRegion.bSmoothPlay = HI_FALSE;

    pstSyncAttr->s32VidPtsAdjust = 0;
    pstSyncAttr->s32AudPtsAdjust = 0;
    pstSyncAttr->u32PreSyncTimeoutMs = 0;
    pstSyncAttr->bQuickOutput = HI_TRUE;

    return;
}

HI_S32 HI_MPI_SYNC_Create(HI_UNF_SYNC_ATTR_S *pstSyncAttr, HI_HANDLE *phSync)
{
    SYNC_S               *pSync = HI_NULL;
    SYNC_CREATE_S        SyncCreate;
    SYNC_USR_ADDR_S      SyncUsrAddr = {0};
    HI_S32                 Ret = 0;

    if (!phSync)
    {
        HI_ERR_SYNC("para phSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    if (!pstSyncAttr)
    {
        HI_ERR_SYNC("para pstSyncAttr is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    /* create SYNC */
    Ret = ioctl(g_SyncDevFd, CMD_SYNC_CREATE, &SyncCreate);
    if (Ret != HI_SUCCESS)
    {
        goto RET;
    }

    /* map the memories allocated in kernel space to user space */
    pSync = (SYNC_S *)(HI_MMAP(SyncCreate.SyncPhyAddr, 0x1000));
    if (!pSync)
    {
         Ret = HI_ERR_AVPLAY_CREATE_ERR;
         goto SYNC_DESTROY;
    }

    SyncUsrAddr.SyncId = SyncCreate.SyncId;
    SyncUsrAddr.SyncUsrAddr = (HI_U8 *)pSync;

    Ret = ioctl(g_SyncDevFd, CMD_SYNC_SET_USRADDR, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
         Ret = HI_ERR_SYNC_CREATE_ERR;
         goto SYNC_UNMAP;
    }

    pSync->pSyncMutex = (pthread_mutex_t *)HI_MALLOC(HI_ID_SYNC, sizeof(pthread_mutex_t));
    if (pSync->pSyncMutex == HI_NULL)
    {
         Ret = HI_ERR_SYNC_CREATE_ERR;
         goto SYNC_UNMAP;
    }

    (HI_VOID)pthread_mutex_init(pSync->pSyncMutex, NULL);

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    pSync->SyncAttr = *pstSyncAttr;

    pSync->VidEnable = HI_FALSE;
    pSync->AudEnable = HI_FALSE;
    pSync->CrtStatus = SYNC_STATUS_STOP;

    pSync->AudDDPMode = HI_FALSE;

    pSync->bUseStopRegion = HI_TRUE;

    if (0 == SyncCreate.SyncId)
    {
        pSync->bPrint = HI_TRUE;
    }
    else
    {
        pSync->bPrint = HI_FALSE;
    }

    SYNC_ResetStatInfo(pSync, SYNC_CHAN_VID);
    SYNC_ResetStatInfo(pSync, SYNC_CHAN_AUD);

    *phSync = (HI_ID_SYNC << 16) | SyncCreate.SyncId;

    SYNC_Mutex_UnLock(pSync->pSyncMutex);

    return     HI_SUCCESS;

SYNC_UNMAP:
    (HI_VOID)HI_MUNMAP((HI_VOID *)SyncUsrAddr.SyncUsrAddr);

SYNC_DESTROY:
    (HI_VOID)ioctl(g_SyncDevFd, CMD_SYNC_DESTROY, &(SyncCreate.SyncId));

RET:
    return Ret;
}

HI_S32 HI_MPI_SYNC_Destroy(HI_HANDLE hSync)
{
    SYNC_S            *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr = {0};
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    (HI_VOID)pthread_mutex_destroy(pSync->pSyncMutex);

    HI_FREE(HI_ID_SYNC, (HI_VOID*)(pSync->pSyncMutex));

    Ret = ioctl(g_SyncDevFd, CMD_SYNC_DESTROY, &SyncUsrAddr.SyncId);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    (HI_VOID)HI_MUNMAP((HI_VOID *)SyncUsrAddr.SyncUsrAddr);

    return HI_SUCCESS ;
}

HI_S32 HI_MPI_SYNC_SetAttr(HI_HANDLE hSync, HI_UNF_SYNC_ATTR_S *pstSyncAttr)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    if (!pstSyncAttr)
    {
        HI_ERR_SYNC("para pstSyncAttr is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    if (pstSyncAttr->enSyncRef >= HI_UNF_AVPLAY_SYNC_REF_BUTT)
    {
        HI_ERR_SYNC("para pstSyncAttr->enSyncRef is invalid.\n");
        return HI_ERR_SYNC_INVALID_PARA;
    }

    if (HI_UNF_SYNC_REF_VIDEO == pstSyncAttr->enSyncRef)
    {
        HI_ERR_SYNC("para pstSyncAttr->enSyncRef is no-support.\n");
        return HI_ERR_SYNC_INVALID_PARA;
    }

    if (HI_UNF_SYNC_REF_NONE != pstSyncAttr->enSyncRef)
    {
        if (pstSyncAttr->stSyncNovelRegion.s32VidPlusTime < 0)
        {
            HI_ERR_SYNC("para pstSyncAttr->stSyncNovelRegion.s32VidPlusTime is invalid.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if (pstSyncAttr->stSyncNovelRegion.s32VidNegativeTime > 0)
        {
            HI_ERR_SYNC("para pstSyncAttr->stSyncNovelRegion.s32VidNegativeTime is invalid.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if (pstSyncAttr->stSyncNovelRegion.s32VidPlusTime > VID_LEAD_DISCARD_THRESHOLD)
        {
            HI_ERR_SYNC("para pstSyncAttr->stSyncNovelRegion.s32VidPlusTime is invalid.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if (pstSyncAttr->stSyncNovelRegion.s32VidNegativeTime < (-VID_LAG_DISCARD_THRESHOLD))
        {
            HI_ERR_SYNC("para pstSyncAttr->stSyncNovelRegion.s32VidNegativeTime is invalid.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if ((pstSyncAttr->stSyncNovelRegion.bSmoothPlay!= HI_TRUE)
          &&(pstSyncAttr->stSyncNovelRegion.bSmoothPlay!= HI_FALSE)
           )
        {
            HI_ERR_SYNC("para pstSyncAttr->stSyncNovelRegion.bSmoothPlay is invalid.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if (pstSyncAttr->stSyncStartRegion.s32VidPlusTime < 0)
        {
            HI_ERR_SYNC("para pstSyncAttr->stSyncStartRegion.s32VidPlusTime is invalid.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if (pstSyncAttr->stSyncStartRegion.s32VidNegativeTime > 0)
        {
            HI_ERR_SYNC("para pstSyncAttr->stSyncStartRegion.s32VidNegativeTime is invalid.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if (pstSyncAttr->stSyncStartRegion.s32VidPlusTime > VID_LEAD_DISCARD_THRESHOLD)
        {
            HI_ERR_SYNC("para pstSyncAttr->stSyncStartRegion.s32VidPlusTime is invalid.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if (pstSyncAttr->stSyncStartRegion.s32VidNegativeTime < (-VID_LAG_DISCARD_THRESHOLD))
        {
            HI_ERR_SYNC("para pstSyncAttr->stSyncStartRegion.s32VidNegativeTime is invalid.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if ((pstSyncAttr->stSyncStartRegion.bSmoothPlay!= HI_TRUE)
          &&(pstSyncAttr->stSyncStartRegion.bSmoothPlay!= HI_FALSE)
           )
        {
            HI_ERR_SYNC("para pstSyncAttr->stSyncStartRegion.bSmoothPlay is invalid.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if (pstSyncAttr->stSyncNovelRegion.s32VidPlusTime <= pstSyncAttr->stSyncStartRegion.s32VidPlusTime)
        {
            HI_ERR_SYNC("para stSyncNovelRegion.s32VidPlusTime <= stSyncStartRegion.s32VidPlusTime.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if (pstSyncAttr->stSyncNovelRegion.s32VidNegativeTime >=  pstSyncAttr->stSyncStartRegion.s32VidNegativeTime)
        {
            HI_ERR_SYNC("para stSyncNovelRegion.s32VidNegativeTime >= stSyncStartRegion.s32VidNegativeTime.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if ((pstSyncAttr->bQuickOutput != HI_TRUE)
          &&(pstSyncAttr->bQuickOutput != HI_FALSE)
           )
        {
            HI_ERR_SYNC("para pstSyncAttr->bQuickOutput is invalid.\n");
            return HI_ERR_SYNC_INVALID_PARA;
        }

        if ((pstSyncAttr->u32PreSyncTimeoutMs)
          &&(pstSyncAttr->u32PreSyncTimeoutMs < PRE_SYNC_MIN_TIME)
           )
        {
            HI_WARN_SYNC("para pstSyncAttr->u32PreSyncTimeoutMs is invalid, modify it to 300.\n");
            pstSyncAttr->u32PreSyncTimeoutMs = PRE_SYNC_MIN_TIME;
        }
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    memcpy(&pSync->SyncAttr, pstSyncAttr, sizeof(HI_UNF_SYNC_ATTR_S));

    SYNC_Mutex_UnLock(pSync->pSyncMutex);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_GetAttr(HI_HANDLE hSync, HI_UNF_SYNC_ATTR_S *pstSyncAttr)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    if (!pstSyncAttr)
    {
        HI_ERR_SYNC("para pstSyncAttr is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    memcpy(pstSyncAttr, &pSync->SyncAttr, sizeof(HI_UNF_SYNC_ATTR_S));

    SYNC_Mutex_UnLock(pSync->pSyncMutex);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_Start(HI_HANDLE hSync, SYNC_CHAN_E enChn)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    HI_U32             SyncId;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    if (enChn >= SYNC_CHAN_BUTT)
    {
        HI_ERR_SYNC("para enChn is invalid.\n");
        return HI_ERR_SYNC_INVALID_PARA;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    SyncId = hSync & 0xff;

    if (enChn == SYNC_CHAN_VID)
    {
        pSync->VidEnable = HI_TRUE;

        Ret = ioctl(g_SyncDevFd, CMD_SYNC_START_SYNC, &SyncId);
        if (Ret != HI_SUCCESS)
        {
            SYNC_Mutex_UnLock(pSync->pSyncMutex);
            return Ret;
        }
    }

    if (enChn == SYNC_CHAN_AUD)
    {
        pSync->AudEnable = HI_TRUE;

        Ret = ioctl(g_SyncDevFd, CMD_SYNC_START_SYNC, &SyncId);
        if (Ret != HI_SUCCESS)
        {
            SYNC_Mutex_UnLock(pSync->pSyncMutex);
            return Ret;
        }
    }

    SYNC_Mutex_UnLock(pSync->pSyncMutex);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_Stop(HI_HANDLE hSync, SYNC_CHAN_E enChn)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    if (enChn == SYNC_CHAN_VID)
    {
        pSync->VidEnable = HI_FALSE;
        SYNC_ResetStatInfo(pSync, SYNC_CHAN_VID);
    }

    if (enChn == SYNC_CHAN_AUD)
    {
        pSync->AudEnable = HI_FALSE;
        SYNC_ResetStatInfo(pSync, SYNC_CHAN_AUD);
    }

    SYNC_Mutex_UnLock(pSync->pSyncMutex);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_Play(HI_HANDLE hSync)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    pSync->CrtStatus = SYNC_STATUS_PLAY;

    SYNC_Mutex_UnLock(pSync->pSyncMutex);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_Seek(HI_HANDLE hSync, HI_U32 u32SeekPts)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    pSync->ExtPreSyncTagetTime = u32SeekPts;
    pSync->UseExtPreSyncTaget = HI_TRUE;

    SYNC_Mutex_UnLock(pSync->pSyncMutex);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_Pause(HI_HANDLE hSync)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    HI_U32             SyncId;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    SyncId = hSync & 0xff;

    pSync->CrtStatus = SYNC_STATUS_PAUSE;

    Ret = ioctl(g_SyncDevFd, CMD_SYNC_PAUSE_SYNC, &SyncId);
    if (Ret != HI_SUCCESS)
    {
        SYNC_Mutex_UnLock(pSync->pSyncMutex);
        return Ret;
    }

    SYNC_Mutex_UnLock(pSync->pSyncMutex);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_Tplay(HI_HANDLE hSync )
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    pSync->CrtStatus = SYNC_STATUS_TPLAY;

    SYNC_Mutex_UnLock(pSync->pSyncMutex);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_Resume(HI_HANDLE hSync)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    HI_U32             SyncId;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    SyncId = hSync & 0xff;

    Ret = ioctl(g_SyncDevFd, CMD_SYNC_RESUME_SYNC, &SyncId);
    if (Ret != HI_SUCCESS)
    {
        SYNC_Mutex_UnLock(pSync->pSyncMutex);
        return Ret;
    }

    pSync->CrtStatus = SYNC_STATUS_PLAY;

    SYNC_Mutex_UnLock(pSync->pSyncMutex);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_SetBufState(HI_HANDLE hSync, SYNC_BUF_STATUS_S stBufStatus)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    if (stBufStatus.VidBufState >= SYNC_BUF_STATE_BUTT)
    {
        HI_ERR_SYNC("VidBufState is invalid.\n");
        return HI_ERR_SYNC_INVALID_PARA;
    }

    if (stBufStatus.AudBufState >= SYNC_BUF_STATE_BUTT)
    {
        HI_ERR_SYNC("AudBufState is invalid.\n");
        return HI_ERR_SYNC_INVALID_PARA;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    memcpy(&(pSync->CrtBufStatus), &stBufStatus, sizeof(SYNC_BUF_STATUS_S));

    SYNC_Mutex_UnLock(pSync->pSyncMutex);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_AudJudge(HI_HANDLE hSync, SYNC_AUD_INFO_S *pAudInfo, SYNC_AUD_OPT_S *pAudOpt)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    SYNC_AUD_JUDGE_S   SyncAudJudge;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    if (!pAudInfo)
    {
        HI_ERR_SYNC("para pAudInfo is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    if (!pAudOpt)
    {
        HI_ERR_SYNC("para pAudOpt is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    SyncAudJudge.hSync = hSync;
    SyncAudJudge.AudInfo = *pAudInfo;

    Ret = ioctl(g_SyncDevFd, CMD_SYNC_AUD_JUDGE, &SyncAudJudge);
    if (Ret != HI_SUCCESS)
    {
        SYNC_Mutex_UnLock(pSync->pSyncMutex);
        return Ret;
    }

    *pAudOpt = SyncAudJudge.AudOpt;

    SYNC_Mutex_UnLock(pSync->pSyncMutex);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_VidJudge(HI_HANDLE hSync, SYNC_VID_INFO_S *pVidInfo, SYNC_VID_OPT_S *pVidOpt)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    SYNC_VID_JUDGE_S   SyncVidJudge;
    HI_S32             Ret;

    if (HI_INVALID_HANDLE == hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    if (!pVidInfo)
    {
        HI_ERR_SYNC("para pAudInfo is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    if (!pVidOpt)
    {
        HI_ERR_SYNC("para pAudOpt is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    SyncVidJudge.hSync = hSync;
    SyncVidJudge.VidInfo = *pVidInfo;

    Ret = ioctl(g_SyncDevFd, CMD_SYNC_VID_JUDGE, &SyncVidJudge);
    if (HI_SUCCESS != Ret)
    {
        SYNC_Mutex_UnLock(pSync->pSyncMutex);
        return Ret;
    }

    *pVidOpt = SyncVidJudge.VidOpt;

    SYNC_Mutex_UnLock(pSync->pSyncMutex);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_GetStatus(HI_HANDLE hSync, HI_UNF_SYNC_STATUS_S *pSyncStatus)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    SYNC_GET_TIME_S    SyncTime;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    if (!pSyncStatus)
    {
        HI_ERR_SYNC("para pSyncStatus is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    SYNC_Mutex_Lock(pSync->pSyncMutex);

    SyncTime.SyncId = hSync & 0xff;

    Ret = ioctl(g_SyncDevFd, CMD_SYNC_GET_TIME, &SyncTime);
    if (Ret != HI_SUCCESS)
    {
        SYNC_Mutex_UnLock(pSync->pSyncMutex);
        return Ret;
    }

    pSyncStatus->u32FirstAudPts = pSync->AudFirstPts;
    pSyncStatus->u32FirstVidPts = pSync->VidFirstPts;
    pSyncStatus->u32LastAudPts = pSync->AudLastPts;
    pSyncStatus->u32LastVidPts = pSync->VidLastPts;
    pSyncStatus->s32DiffAvPlayTime = pSync->VidAudDiff;
    pSyncStatus->u32PlayTime = SyncTime.PlayTime;
    pSyncStatus->u32LocalTime = SyncTime.LocalTime;

    SYNC_Mutex_UnLock(pSync->pSyncMutex);
    return HI_SUCCESS;
}


HI_S32 HI_MPI_SYNC_SetDDPTestMode(HI_HANDLE hSync, HI_BOOL bEnable)
{
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;
    HI_S32             Ret;

    if (!hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    (HI_VOID)SYNC_Mutex_Lock(pSync->pSyncMutex);

    pSync->AudDDPMode = bEnable;

    (HI_VOID)SYNC_Mutex_UnLock(pSync->pSyncMutex);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_CheckNewEvent(HI_HANDLE hSync, SYNC_EVENT_S *pstEvent)
{
    HI_S32             Ret;
    SYNC_S             *pSync = HI_NULL;
    SYNC_USR_ADDR_S    SyncUsrAddr;

    if (HI_INVALID_HANDLE == hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    if (HI_NULL == pstEvent)
    {
        HI_ERR_SYNC("para pstEvent is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    (HI_VOID)SYNC_Mutex_Lock(pSync->pSyncMutex);

    memcpy(pstEvent, &(pSync->SyncEvent), sizeof(SYNC_EVENT_S));

    if (pSync->SyncEvent.bVidPtsJump)
    {
        pSync->SyncEvent.bVidPtsJump = HI_FALSE;
    }

    if (pSync->SyncEvent.bAudPtsJump)
    {
        pSync->SyncEvent.bAudPtsJump = HI_FALSE;
    }

    if (pSync->SyncEvent.bStatChange)
    {
        pSync->SyncEvent.bStatChange = HI_FALSE;
    }

    (HI_VOID)SYNC_Mutex_UnLock(pSync->pSyncMutex);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYNC_SetExtInfo(HI_HANDLE hSync, SYNC_EXT_INFO_E enExtInfo, HI_VOID *pData)
{
    HI_S32              Ret;

    SYNC_S              *pSync = HI_NULL;
    SYNC_USR_ADDR_S     SyncUsrAddr;

    if (HI_INVALID_HANDLE == hSync)
    {
        HI_ERR_SYNC("para hSync is null.\n");
        return HI_ERR_SYNC_NULL_PTR;
    }

    CHECK_SYNC_INIT();

    Ret = SYNC_CheckHandle(hSync, &SyncUsrAddr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    pSync = (SYNC_S *)SyncUsrAddr.SyncUsrAddr;

    (HI_VOID)SYNC_Mutex_Lock(pSync->pSyncMutex);

    switch(enExtInfo)
    {
    case SYNC_EXT_INFO_FIRST_PTS:
        pSync->VidFirstDecPts = ((SYNC_DEC_PTS_S *)pData)->VidDecPts;
        break;
    case SYNC_EXT_INFO_SECOND_PTS:
        pSync->VidSecondDecPts = ((SYNC_DEC_PTS_S *)pData)->VidDecPts;
        break;
    case SYNC_EXT_INFO_STOP_REGION:
        pSync->bUseStopRegion = ((SYNC_STOP_REGION_S *)pData)->bUseStopRegion;
        break;
    default:
        break;
    }

    (HI_VOID)SYNC_Mutex_UnLock(pSync->pSyncMutex);

    return HI_SUCCESS;

}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
