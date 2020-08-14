/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_pvr_index.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/16
  Description   : INDEX module
  History       :
  1.Date        : 2008/04/16
    Author      : q46153
    Modification: Created file

******************************************************************************/
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/times.h>
#include <assert.h>
#include "hi_module.h"
#include "hi_mpi_mem.h"

#include "pvr_debug.h"
#include "hi_pvr_index.h"
#include "hi_pvr_index_inter.h"
#include "hi_pvr_rec_ctrl.h"
#include "hi_pvr_play_ctrl.h"
#include "hi_pvr_fifo.h"


//#include "hi_pvr_stub.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C"
{
#endif
#endif /* End of #ifdef __cplusplus */

#define PVR_INDEX_NUM (PVR_REC_MAX_CHN_NUM + PVR_PLAY_MAX_CHN_NUM)

/***************************************************************************/
/**
    only replay, use the first 2,
    only record, use the last 3,
    on timeshift, player use the index struct of record.
    only play, the index channel number is equal the play channel.
    but, the index channel number is not equal the record channel, it should be equal with the index parser channel number
*/
static PVR_INDEX_S g_stPVRIndex[PVR_INDEX_NUM];
static HI_U32 g_u32RecChnStat[PVR_REC_MAX_CHN_NUM] = {PVR_INDEX_REC_CHN_UNUSED};
static HI_U32 g_u32PvrIndexInit = 0;         
/***************************************************************************/ 

static HI_U32 PVRIndexGetRecTimeStamp(PVR_INDEX_HANDLE handle, const HI_MPI_DMX_REC_INDEX_S *pstRecIdx);
static HI_BOOL PVRIndexCheckEndOffsetCoverStartOffset(HI_U64 u64EndOffset,
                                                      HI_U64 u64EndNextOffset,
                                                      HI_U64 u64Offset,
                                                      HI_U64 u64CycSize);
static HI_BOOL PVRIndexIsFileRecording(const HI_CHAR *pIdxFileName, PVR_INDEX_HANDLE *handle);
static HI_BOOL PVRIndexIsFilePlaying(const HI_CHAR *pIdxFileName, PVR_INDEX_HANDLE *handle);
static HI_BOOL PVRIndexIsFrameValid(HI_U32 u32FrmPos, PVR_CYC_MGR_S *pstCycMgr);
STATIC INLINE HI_U32 PVRIndexCalcNewPos(PVR_INDEX_HANDLE handle, HI_U32 u32FrmPos, HI_S32 s32Offset);
STATIC INLINE HI_VOID PVRIndexCycMoveReadFrame(PVR_INDEX_HANDLE handle, HI_S32 s32Offset);
static HI_S32 PVRIndexWriteIndexCycMgr(PVR_INDEX_HANDLE hIndex, PVR_CYC_MGR_S *pstCycMgr);
STATIC INLINE HI_S32 PVRIndexGetHeaderInfo(HI_S32 s32Fd, PVR_IDX_HEADER_INFO_S* pstHeadInfo);
static HI_S32 PVRIndexReadHeaderInfo(PVR_INDEX_HANDLE handle);
static HI_S32 PVRIndexWriteIndexInfo(PVR_INDEX_HANDLE hIndex, PVR_REC_INDEX_INFO_S *pstIdxInfo);
STATIC INLINE HI_VOID PVRIndexSetDftAttr(PVR_INDEX_HANDLE handle);
static HI_S32 PVRIndexCheckFileCorrect(PVR_INDEX_HANDLE handle, HI_U64 u64FileSize);
#ifndef HI_PVR_EXTRA_BUF_SUPPORT
static HI_S32 PVRCacheWriteIdx(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S* pstEntry, 
                                    HI_S32 s32Num, HI_U32 u32Offset,HI_BOOL* bWriteFlag);
#endif
static HI_S32 PVRIndexGetFBwardIPBFrameNum(PVR_INDEX_HANDLE handle,
                                           PVR_INDEX_DIRECTION_E enDirection, 
                                           HI_U32 u32FrameType, 
                                           HI_U32 u32CurFrameNum, 
                                           HI_U32 *pu32NextFrameNum);
static HI_VOID PVRIndexCheckReadFrmValidWhenTimeshift(PVR_INDEX_HANDLE hIndex);
static HI_BOOL PVRIndexCheckRewind(PVR_INDEX_HANDLE hIndex,
                                   PVR_CYC_MGR_S *pstCycMgr,
                                   PVR_INDEX_ENTRY_NEW_S *pstIdxEntry);
static HI_VOID PVRIndexAddToRefList(PVR_REC_REF_LIST_S *pstRefList, HI_S32 s32CurPoc, HI_U32 u32FrmNum);
static HI_S32 PVRIndexPocToFrmNum(PVR_REC_REF_LIST_S *pstRefList, HI_S32 s32Poc, HI_U32 *pu32FrmNum);
static INLINE HI_VOID PVRIndexAssignIndexEntry(PVR_INDEX_HANDLE hIndex,
                                               const HI_MPI_DMX_REC_INDEX_S *pstDmxIndexInfo,
                                               PVR_INDEX_ENTRY_NEW_S *pstIdxEntry);
static HI_VOID PVRIndexSubGopInfo(PVR_INDEX_HANDLE hIndex,
                                     PVR_REC_INDEX_INFO_S *pstRecIdxInfo, 
                                     HI_U32 u32PreCurFrm);
static HI_VOID PVRIndexAddGopInfo(PVR_INDEX_HANDLE hIndex,
                                     PVR_REC_INDEX_INFO_S *pstRecIdxInfo,
                                     PVR_INDEX_ENTRY_NEW_S *pstIdxEntry);
static INLINE HI_VOID PVRIndexUpdateTotalFrmNum(PVR_REC_INDEX_INFO_S *pstRecIdxInfo,
                                                PVR_CYC_MGR_S *pstCycMgr);
static HI_VOID PVRIndexPushStartFrame(PVR_INDEX_HANDLE hIndex, 
                                      PVR_CYC_MGR_S *pstCycMgr,
                                      PVR_REC_INDEX_INFO_S *pstIdxInfo,
                                      HI_U64 u64NextEndOffset);
static HI_VOID PVRIndexUpdateCycMgrAndRecInfoWhenRewind(PVR_INDEX_HANDLE hIndex, 
                                                        PVR_CYC_MGR_S *pstCycMgr, 
                                                        PVR_REC_INDEX_INFO_S * pstIdxInfo, 
                                                        PVR_INDEX_ENTRY_NEW_S *pstEndEntry);
static HI_VOID PVRIndexUpdateCycMgrAndRecInfoWhenNotRewind(PVR_INDEX_HANDLE hIndex, 
                                                           PVR_CYC_MGR_S *pstCycMgr, 
                                                           PVR_REC_INDEX_INFO_S * pstIdxInfo,
                                                           PVR_INDEX_ENTRY_NEW_S *pstEndEntry);
static HI_VOID PVRIndexUpdateCycMgrAndRecInfoWhenDisableRewind(PVR_INDEX_HANDLE hIndex, 
                                                               PVR_CYC_MGR_S *pstCycMgr, 
                                                               PVR_REC_INDEX_INFO_S * pstIdxInfo, 
                                                               PVR_INDEX_ENTRY_NEW_S *pstEndEntry);
static HI_VOID PVRIndexGetRealOffset(PVR_INDEX_HANDLE hIndex,
                                     PVR_CYC_MGR_S *pstCycMgr,
                                     PVR_INDEX_ENTRY_NEW_S *pstIdxEntry);
static HI_VOID PVRIndexProcRewind(PVR_INDEX_HANDLE hIndex,
                                       PVR_CYC_MGR_S *pstCycMgr,
                                       PVR_REC_INDEX_INFO_S *pstRecInfo,
                                       PVR_INDEX_ENTRY_NEW_S *pstIdxEntry);
static HI_VOID PVRIndexProcNotRewind(PVR_INDEX_HANDLE hIndex,
                                       PVR_CYC_MGR_S *pstCycMgr,
                                       PVR_REC_INDEX_INFO_S *pstRecInfo,
                                       PVR_INDEX_ENTRY_NEW_S *pstIdxEntry);
static HI_VOID PVRIndexProcDisableRewind(PVR_INDEX_HANDLE hIndex,
                                       PVR_CYC_MGR_S *pstCycMgr,
                                       PVR_REC_INDEX_INFO_S *pstRecInfo,
                                       PVR_INDEX_ENTRY_NEW_S *pstIdxEntry);

STATIC HI_S32 PVRIndexGetNextEntry(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry);
STATIC INLINE HI_S32 PVRIndexGetNextIEntry(const PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry);
STATIC INLINE HI_S32 PVRIndexGetPreEntry(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry);
STATIC INLINE HI_S32 PVRIndexGetPreIEntry(const PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry);

STATIC HI_U32 PVRIndexSeachByTime(PVR_INDEX_HANDLE handle, HI_U32 timeWant,
                                  HI_U32 start, HI_U32 end,  PVR_FILE seekFd);
STATIC INLINE HI_U32 PVRIndexFindFrameByTime(PVR_INDEX_HANDLE handle, HI_U32 u32FindTime);
STATIC INLINE HI_U32 PVRIndexCalcNewPosForward(HI_U32 u32EndFrame, HI_U32 u32LastFrame, HI_U32 u32StartFrame, HI_U32 u32FrmPos, HI_U32 u32NewPos);
STATIC INLINE HI_U32 PVRIndexCalcNewPosBackward(HI_U32 u32EndFrame, HI_U32 u32LastFrame, HI_U32 u32StartFrame, HI_U32 u32FrmPos, HI_U32 u32NewPos);

#if 0  /*obsolete APIs*/
STATIC INLINE HI_S32 PVRIndexGetCurrentEntry(const PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry);
#endif

#ifdef HI_PVR_EXTRA_BUF_SUPPORT
static HI_BOOL PVRIndexWriteWhetherCoverRead(HI_U32 u32Write, HI_U32 u32Num, HIPVR_IDX_BUF_S *pstReadCache);
static HI_S32 PVRIndexWriteIndexEntry(PVR_INDEX_HANDLE hIndex,
                                      HI_U32 u32WriteNum,
                                      HI_U32 u32CacheOffset,
                                      HI_U32 u32FileOffset);
static HI_VOID PVRIndexCopyToCache(PVR_INDEX_HANDLE hIndex, HIPVR_IDX_BUF_S *pstIdxBuf, PVR_INDEX_ENTRY_NEW_S *pstIdxEntry);
#endif
/***************************************************************************/ 

static HI_U32 PVRIndexGetRecTimeStamp(PVR_INDEX_HANDLE handle, const HI_MPI_DMX_REC_INDEX_S *pstRecIdx)
{
    HI_U32 u32DeltaTime = 0;
    HI_U32 u32DeltaFrameNum = 0;
    HI_U32 u32DeltaTimeStamp = 0;
    PVR_REC_TIMESTAMP_S *pstTimeStamp = &handle->stTimeStamp;

    pstTimeStamp->u32FrmCnt++;

    //if pts is invalide, only add the deltatime
    if (pstRecIdx->u32PtsMs == PVR_INDEX_INVALID_PTSMS)
    {
        pstTimeStamp->u32CurFrameTimeMs += pstTimeStamp->u32DeltaTimeStamp;
        return pstTimeStamp->u32CurFrameTimeMs;
    }

    /*in first Gop (if less than 20 frames),we using average average delta timestamp of the first 20 frames to adjust the frameTimeMs*/
    /*get the first frame PTS ,as the calc base value to get the first average delta timestamp*/
    if (((pstRecIdx->enFrameType == HI_UNF_FRAME_TYPE_IDR) || (pstRecIdx->enFrameType == HI_UNF_FRAME_TYPE_I)) && 
        (pstTimeStamp->stLastIFrmPtsInfo.u32PtsMs == PVR_INDEX_INVALID_PTSMS))
    {
        pstTimeStamp->stLastIFrmPtsInfo.u32PtsMs = pstRecIdx->u32PtsMs;
        pstTimeStamp->stLastIFrmPtsInfo.u32FrmCnt = pstTimeStamp->u32FrmCnt;
    }

    /*After we get I frame ,then ,we only using I fram to calc the average delta timestamp*/
    if (((pstRecIdx->enFrameType == HI_UNF_FRAME_TYPE_IDR) || (pstRecIdx->enFrameType == HI_UNF_FRAME_TYPE_I))&& 
        (pstTimeStamp->u32FrmCnt >= PVR_INDEX_MAX_TIMESTAMP_BASE))
    {
        pstTimeStamp->stCurIFrmPtsInfo.u32PtsMs = pstRecIdx->u32PtsMs;
        pstTimeStamp->stCurIFrmPtsInfo.u32FrmCnt = pstTimeStamp->u32FrmCnt;
        if (pstTimeStamp->stCurIFrmPtsInfo.u32PtsMs != pstTimeStamp->stLastIFrmPtsInfo.u32PtsMs)
        {
            pstTimeStamp->bShouldUpdateTimeStamp = HI_TRUE;
        }
    }

    if ((pstTimeStamp->stCurIFrmPtsInfo.u32PtsMs == PVR_INDEX_INVALID_PTSMS) &&
        (pstRecIdx->u32PtsMs != pstTimeStamp->stLastIFrmPtsInfo.u32PtsMs) &&
        (abs(pstRecIdx->u32PtsMs - pstTimeStamp->stLastIFrmPtsInfo.u32PtsMs) < pstTimeStamp->u32MinPTSGapWhenBegin))
    {
        pstTimeStamp->u32MinPTSGapWhenBegin = abs(pstRecIdx->u32PtsMs - pstTimeStamp->stLastIFrmPtsInfo.u32PtsMs);
        pstTimeStamp->u32DeltaTimeStamp = pstTimeStamp->u32MinPTSGapWhenBegin;
    }

    if (pstTimeStamp->bShouldUpdateTimeStamp)
    {
        pstTimeStamp->bShouldUpdateTimeStamp = HI_FALSE;
        //if the pts is rewind , only update stLastIFrmPtsInfo, don't change the deltatime
        if (pstTimeStamp->stCurIFrmPtsInfo.u32PtsMs < pstTimeStamp->stLastIFrmPtsInfo.u32PtsMs)
        {
            pstTimeStamp->stLastIFrmPtsInfo.u32PtsMs = pstRecIdx->u32PtsMs;
            pstTimeStamp->stLastIFrmPtsInfo.u32FrmCnt = pstTimeStamp->u32FrmCnt;
            pstTimeStamp->stCurIFrmPtsInfo.u32PtsMs = PVR_INDEX_INVALID_PTSMS;
        }
        else
        {
            u32DeltaTime = pstTimeStamp->stCurIFrmPtsInfo.u32PtsMs - pstTimeStamp->stLastIFrmPtsInfo.u32PtsMs;
            u32DeltaFrameNum = pstTimeStamp->stCurIFrmPtsInfo.u32FrmCnt - pstTimeStamp->stLastIFrmPtsInfo.u32FrmCnt;
            if ((u32DeltaTime !=0) && (u32DeltaFrameNum >= 1))
            {
                u32DeltaTimeStamp = u32DeltaTime/u32DeltaFrameNum;
                /*liuxingqiang said ,he has meet the frame time gop rang of 15~60 Ms,some net stream is 15fps*/
                if ((u32DeltaTimeStamp < PVR_INDEX_MAX_TIMESTAMP_GOP) && 
                    (u32DeltaTimeStamp > PVR_INDEX_MIN_TIMESTAMP_GOP))
                {
                    pstTimeStamp->u32DeltaTimeStamp = u32DeltaTimeStamp;
                }
                else
                {
                    HI_INFO_PVR("The deltatime %d is beyond the regular gap! CurPts/LastPts/Frmcnt %d/%d/%d\n",
                                u32DeltaTime, pstTimeStamp->stCurIFrmPtsInfo.u32PtsMs,
                                pstTimeStamp->stLastIFrmPtsInfo.u32PtsMs, u32DeltaFrameNum);
                }
            }
            pstTimeStamp->stLastIFrmPtsInfo.u32PtsMs = pstTimeStamp->stCurIFrmPtsInfo.u32PtsMs;
            pstTimeStamp->stLastIFrmPtsInfo.u32FrmCnt = pstTimeStamp->stCurIFrmPtsInfo.u32FrmCnt;
        }
    }

    if (pstTimeStamp->u32FrmCnt <= 1)
    {
        pstTimeStamp->u32CurFrameTimeMs = 0;
    }
    else
    {
        if (handle->u32PauseFrameCnt != 0)
        {
            pstTimeStamp->u32CurFrameTimeMs = pstTimeStamp->u32CurFrameTimeMs + (handle->u32PauseFrameCnt + 1)*pstTimeStamp->u32DeltaTimeStamp;
            handle->u32PauseFrameCnt = 0;
        }
        else
        {
            pstTimeStamp->u32CurFrameTimeMs += pstTimeStamp->u32DeltaTimeStamp;
        }
    }

    return pstTimeStamp->u32CurFrameTimeMs;
}

#if 0
STATIC HI_U32 PVRIndexGetCurTimeMs(HI_VOID)
{
    HI_U32    Ticks;
    struct tms buf;
    Ticks = (HI_U32)times(&buf);

    return Ticks * 10;
}
#endif

static HI_BOOL PVRIndexCheckEndOffsetCoverStartOffset(HI_U64 u64EndOffset,
                                                      HI_U64 u64EndNextOffset,
                                                      HI_U64 u64Offset,
                                                      HI_U64 u64CycSize)
{
    if (((u64EndOffset < u64Offset) && (u64EndNextOffset >= u64Offset)) ||
         ((u64EndNextOffset > u64CycSize) && (u64EndNextOffset % u64CycSize >= u64Offset)))
    {
        return HI_TRUE;   
    }
    else
        return HI_FALSE;
}

/*****************************************************************************
 Prototype       : PVRIndexIsFileRecording
 Description     : check whether the file is recording or not.
 Input           : pIdxFileName--- to check the file
 Output          : handle------- PVR index handle
 Return Value    : On successfully, return HI_TRUE. otherwise return HI_FALSE;
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2010/06/18
    Author       : j40671
    Modification : Created function

*****************************************************************************/
/**
@brief Check the index file whether is the recording one or not. If HI_TRUE, param handle will save the recording index handle.

@param[in] pIdxFileName : the name of index file
@param[out] handle      : if return HI_TRUE, this param save the recording index handle.

@return HI_BOOL

@retval HI_FALSE The index file does NOT record, yet
@retval HI_TRUE The index file is recording

@author j40671
@date 2010/06/18
@sa PVRIndexIsFilePlaying.
*/
static HI_BOOL PVRIndexIsFileRecording(const HI_CHAR *pIdxFileName, PVR_INDEX_HANDLE *handle)
{
    HI_U32 i;

    if ((NULL == handle) || (NULL == pIdxFileName))
    {
        HI_PRINT("\n<%s %d>: Input pointer parameter is NULL!\n", __FUNCTION__, __LINE__);
        return HI_FALSE;   
    }

    for (i = 0; i < PVR_INDEX_NUM; i++)
    {
        if (g_stPVRIndex[i].bIsRec)
        {
            if  (!strncmp(g_stPVRIndex[i].szIdxFileName, pIdxFileName,strlen(pIdxFileName)))
            {
                *handle = &g_stPVRIndex[i];

                return HI_TRUE;
            }
        }
    }

    return HI_FALSE;
}

/*****************************************************************************
 Prototype       : PVRIndexIsFilePlaying
 Description     : get index handle by file name
 Input           : pFileName  **
 Output          : handle     **
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2010/06/18
    Author       : j40671
    Modification : Created function

*****************************************************************************/
/**
@brief Check the index file whether is the playing one or not. If HI_TRUE, param handle will save the playing index handle.

@param[in] pIdxFileName : the name of index file
@param[out] handle      : if return HI_TRUE, this param save the playing index handle.

@return HI_BOOL

@retval HI_FALSE The index file does NOT play, yet
@retval HI_TRUE The index file is playing

@author j40671
@date 2010/06/18
@sa PVRIndexIsFileRecording.
*/

static HI_BOOL PVRIndexIsFilePlaying(const HI_CHAR *pIdxFileName, PVR_INDEX_HANDLE *handle)
{
    HI_U32 i;
    
    if ((NULL == handle) || (NULL == pIdxFileName))
    {
        HI_PRINT("\n<%s %d>: Input pointer parameter is NULL!\n", __FUNCTION__, __LINE__);
        return HI_FALSE;   
    }

    for (i = 0; i < PVR_INDEX_NUM; i++)
    {
        if (g_stPVRIndex[i].bIsPlay)
        {
            if  (!strncmp(g_stPVRIndex[i].szIdxFileName, pIdxFileName,strlen(pIdxFileName)))
            {
                *handle = &g_stPVRIndex[i];

                return HI_TRUE;
            }
        }
    }

    return HI_FALSE;
}


static HI_BOOL PVRIndexIsFrameValid(HI_U32 u32FrmPos, PVR_CYC_MGR_S *pstCycMgr)
{
    HI_U32 u32StartFrame = 0;
    HI_U32 u32EndFrame = 0;
    HI_U32 u32LastFrame = 0;

    u32StartFrame = pstCycMgr->u32StartFrame;
    u32EndFrame   = (pstCycMgr->u32EndFrame == -1)?0:pstCycMgr->u32EndFrame;
    u32LastFrame  = (pstCycMgr->u32LastFrame == -1)?0:pstCycMgr->u32LastFrame;

    /*
        1. Not rewind case
        =================
        |//////////////////// |
        |//////////////////// |
        =================
        /\                                  /\
         |                                   |
         StartFrame                    EndFrame

         2. Rewind case
         =================
         |////|xxxxx |/////////|
         |////|xxxxx |/////////|
         =================
                /\         /\               /\
                 |          |                |
          EndFrame  StartFrame  LastFrame
        */
    /* Not rewind case */
    if (u32StartFrame < u32EndFrame)
    {
        if ((u32FrmPos >= u32StartFrame) && (u32FrmPos <= u32EndFrame))
        {
            return HI_TRUE;
        }
    }
    else
    {
        /* rewind case */
        if (((u32FrmPos >= u32StartFrame) && (u32FrmPos <= u32LastFrame))
            || (u32FrmPos <= u32EndFrame))
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

//#define DBG_FRAME_POS
STATIC INLINE HI_U32 PVRIndexCalcNewPosForward(HI_U32 u32EndFrame, HI_U32 u32LastFrame, HI_U32 u32StartFrame, HI_U32 u32FrmPos, HI_U32 u32NewPos)
{
    HI_U32 u32Boundary = u32EndFrame; /* not rewind, the boundary should be the u32EndFrame */

    if( (u32StartFrame >= u32EndFrame) && (u32FrmPos >= u32StartFrame) && (u32FrmPos <= u32LastFrame) )
    {
        if(u32NewPos > u32LastFrame)/* rewind, the frame is like X2 */
        {
            HI_WARN_PVR("Frame:%u rewind, LastFrame:%u\n",u32FrmPos, u32LastFrame);

            /* in this case, frame rewind to the u32EndFrame direction, so the right boundary should be the u32EndFrame */
            if (u32NewPos == u32LastFrame + 1)
                u32NewPos = 0;
            else
                u32NewPos -= u32LastFrame;
            HI_WARN_PVR("So subtract LastFrame, new frame:%u\n",u32NewPos);
        }
        else
        {
            /* in this case, frame is not real rewind, so the right boundary should be the u32LastFrame */
            u32Boundary = u32LastFrame;
        }
    }

    /* check the right boundary, make sure the value valid */
    u32NewPos = (u32NewPos > u32Boundary) ? u32Boundary : u32NewPos;
#ifdef DBG_FRAME_POS
    if (u32NewPos == 0)
    {
        HI_ERR_PVR("Seek forward, frame position:%u, u32Boundary = %d u32EndFrame = %d u32LastFrame = %d\n",u32NewPos,u32Boundary, u32EndFrame,u32LastFrame);
    }
#endif
    return u32NewPos;
}
STATIC INLINE HI_U32 PVRIndexCalcNewPosBackward(HI_U32 u32EndFrame, HI_U32 u32LastFrame, HI_U32 u32StartFrame, HI_U32 u32FrmPos, HI_U32 u32NewPos)
{
    HI_U32 u32Boundary = u32StartFrame; /* not rewind, the boundary should be the u32StartFrame */

    if( (u32StartFrame >= u32EndFrame) && (u32FrmPos <= u32EndFrame))
    {
        /* rewind*/
        if((HI_S32)u32NewPos < 0)
        {
            HI_WARN_PVR("Frame:%u rewind, LastFrame:%u, line:%d\n",u32FrmPos, u32LastFrame, __LINE__);

            /* frame rewind to the range u32StartFrame to u32LastFrame, like Frame X1 */
            if ((HI_S32)u32NewPos == -1)
                u32NewPos = u32LastFrame;
            else
                u32NewPos = u32LastFrame + u32NewPos;
        }
        else
        {
            /* In this case, the left boundary should be zero, NOT u32StartFrame! */
            u32Boundary = 0;
            HI_WARN_PVR("Seek backward, ref frame:%u, u32EndFrame=%d u32LastFrame=%d u32NewPos=%d\n",u32FrmPos,u32Boundary, u32EndFrame,u32LastFrame,u32NewPos);
        }
    }

    /* check the left boundary, make sure the value valid.*/
    u32NewPos = ((HI_S32)u32NewPos < (HI_S32)u32Boundary) ? u32Boundary : u32NewPos;
#ifdef DBG_FRAME_POS
    if (u32NewPos == 0)
    {
        HI_ERR_PVR("Seek backward, frame position:%u, u32FrmPos = %d u32Boundary = %d u32EndFrame = %d u32LastFrame = %d\n",u32NewPos,u32FrmPos,u32Boundary, u32EndFrame,u32LastFrame);
    }
#endif
    return u32NewPos;
}

STATIC INLINE HI_U32 PVRIndexCalcNewPos(PVR_INDEX_HANDLE handle, HI_U32 u32FrmPos, HI_S32 s32Offset)
{
    HI_U32 u32NewPos = 0;
    HI_U32 u32StartFrame = 0;
    HI_U32 u32EndFrame = 0;
    HI_U32 u32LastFrame = 0;
    HI_U32 u32DiffToStart = 0;
    HI_U32 u32DiffToEnd = 0;

    u32StartFrame = handle->stCycMgr.u32StartFrame;
    u32EndFrame   = (handle->stCycMgr.u32EndFrame == -1)?0:handle->stCycMgr.u32EndFrame;
    u32LastFrame  = (handle->stCycMgr.u32LastFrame == -1)?0:handle->stCycMgr.u32LastFrame;

#ifdef DBG_FRAME_POS
    HI_ERR_PVR("\033[1;45;37m!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Info start !!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    HI_ERR_PVR("u32StartFrame = %u, u32EndFrame = %u, u32LastFrame = %u\n",u32StartFrame, u32EndFrame, u32LastFrame);
    HI_ERR_PVR("u32FrmPos = %u, s32Offset = %d\n",u32FrmPos, s32Offset);
    HI_ERR_PVR("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Info end!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\033[0m\n");
#else
    HI_INFO_PVR("\033[1;45;37m #######%s########\n", __func__);
    HI_INFO_PVR("u32StartFrame = %u, u32EndFrame = %u, u32LastFrame = %u\n",u32StartFrame, u32EndFrame, u32LastFrame);
    HI_INFO_PVR("u32FrmPos = %u, s32Offset = %d\n",u32FrmPos, s32Offset);
    HI_INFO_PVR("##############################################\n\033[0m\n");
#endif

    /* check whether the frame position is valid or not. invalid, set it to start or end */
    if (!PVRIndexIsFrameValid(u32FrmPos, &handle->stCycMgr))
    {
#ifdef DBG_FRAME_POS
        HI_ERR_PVR("\033[1;45;37m!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Info start !!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        HI_ERR_PVR("u32StartFrame = %u, u32EndFrame = %u, u32LastFrame = %u\n",u32StartFrame, u32EndFrame, u32LastFrame);
        HI_ERR_PVR("u32FrmPos = %u, s32Offset = %d\n",u32FrmPos, s32Offset);
        HI_ERR_PVR("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Info end!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\033[0m\n");
#endif
        u32NewPos = u32LastFrame;
        if ( (u32FrmPos > u32EndFrame) && (u32FrmPos < u32StartFrame) )
        {
            u32DiffToStart = u32StartFrame - u32FrmPos;
            u32DiffToEnd = u32FrmPos - u32EndFrame;

            u32NewPos = (u32DiffToStart > u32DiffToEnd ) ? u32EndFrame : u32StartFrame;

            HI_WARN_PVR("Frame %u value invalid, set it to Frame %u, offset to end:%u, offset to start:%u\n",u32FrmPos,u32NewPos,u32DiffToEnd,u32DiffToStart);
        }

        HI_WARN_PVR("\033[1;37;30mNow u32NewPos is %u in line:%d\n\033[0m", u32NewPos, __LINE__);

        return u32NewPos;
    }

    u32NewPos = u32FrmPos + (HI_U32)s32Offset;

    if(s32Offset >= 0)/* away from the u32FrmPos forward direction toward to u32EndFrame */
    {
        /*
            1. Not rewind (X stands for the sample point)
            --------------------------------------------------
            |                    /     \                     |
            |(offset direction)  ---|--- (offset direction)  |
            |                    \     /                     |
            ------------------------X-------------------------
            /\                      /\                      /\
            ||                      ||                      ||
            u32StartFrame        u32FrmPos     u32LastFrame(u32EndFrame)

            *****************************************************************

            2. Rewind (X1 and X2 stand for the sample point)
            -------------------------------------------
            |              |\    /\  |   u32FrmPos    |
            |              | \  /  \ |     ||         |
            |              |  \/    \|     \/         |
            ---X1--------------------------X2----------
            /\             /\        /\               /\
            ||             ||        ||               ||
            u32FrmPos  u32EndFrame u32StartFrame u32LastFrame

            In the above graphic, the "\    /\" stands for invalid frame position bound.
                                        \  /  \
                                         \/    \

            Notes:(offset forward direction)
            (1) Not rewind, the right boundary should always be u32LastFrame(u32EndFrame).
            (2) Rewind, sample point range from 0 to u32EndFrame, which is similar with Not rewind, the right boudary is u32EndFrame.
            (3) Rewind, sample point range from u32StartFrame to u32LastFrame.The right boundary as following:
                (a). Right boundary -- u32LastFrame, u32NewPos not over u32LastFrame.
                (b). Right boundary -- u32EndFrame, u32NewPos over u32LastFrame, which rewind to u32EndFrame, so it.

        */
        u32NewPos = PVRIndexCalcNewPosForward(u32EndFrame,u32LastFrame,u32StartFrame,u32FrmPos,u32NewPos);
    }
    else/* away from the u32FrmPos backward direction toward to u32StartFrame */
    {
        /*
           Notes:(offset backward direction)
            (1) Not rewind, the left boundary should always be u32StartFrame(that is ZERO).
            (2) Rewind, sample point range from u32StartFrame to u32LastFrame, which is similar with Not rewind,
                the left boudary is u32StartFrame(NOT ZERO).
            (3) Rewind, sample point range from 0 to u32EndFrame.The left boundary as following:
                (a). left boundary -- ZERO, u32NewPos more than ZERO.
                (b). left boundary -- u32StartFrame, u32NewPos less than ZERO, which rewind to u32StartFrame, so it.

        */
        u32NewPos = PVRIndexCalcNewPosBackward(u32EndFrame,u32LastFrame,u32StartFrame,u32FrmPos,u32NewPos);
    }

#ifdef DBG_FRAME_POS
    HI_ERR_PVR("\033[1;42;34m!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! POSITION IS NOT EQUAL Info start !!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    HI_ERR_PVR("u32StartFrame = %u, u32EndFrame = %u, u32LastFrame = %u\n",u32StartFrame, u32EndFrame, u32LastFrame);
    HI_ERR_PVR("new position = %u u32FrmPos = %u, s32Offset = %d\n",u32NewPos, u32FrmPos, s32Offset);
    HI_ERR_PVR("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! POSITION IS NOT EQUAL Info end!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\033[0m\n");
#endif

    HI_INFO_PVR("\033[1;37;30m%s now u32NewPos is %u in line:%d\n\033[0m", __func__, u32NewPos, __LINE__);

    return u32NewPos;
}

/* move s32Offset direction forward or backward, over the end or start, set it to the end or start */
STATIC INLINE HI_VOID PVRIndexCycMoveReadFrame(PVR_INDEX_HANDLE handle, HI_S32 s32Offset)
{
    /* in regardless of rewind, calculate the new position for read by the following interface. */
    handle->u32ReadFrame = PVRIndexCalcNewPos(handle, handle->u32ReadFrame, s32Offset);
}


static HI_S32 PVRIndexWriteIndexCycMgr(PVR_INDEX_HANDLE hIndex, PVR_CYC_MGR_S *pstCycMgr)
{
    PVR_CYC_HEADER_INFO_S stHeaderCycInfo = {0};
    
    HI_ASSERT_RET((PVR_INDEX_HANDLE)NULL != hIndex);
    HI_ASSERT_RET((PVR_CYC_MGR_S *)NULL != pstCycMgr);

    stHeaderCycInfo.u32StartFrame = pstCycMgr->u32StartFrame;
    stHeaderCycInfo.u32IsRewind   = pstCycMgr->bIsRewind;
    stHeaderCycInfo.u32LastFrame  = pstCycMgr->u32LastFrame == -1?0:pstCycMgr->u32LastFrame;
    stHeaderCycInfo.u32EndFrame   = pstCycMgr->u32EndFrame == -1?0:pstCycMgr->u32EndFrame;
    
    if (0 > PVR_WRITE((HI_VOID *)&stHeaderCycInfo, (size_t)sizeof(PVR_CYC_HEADER_INFO_S),
                             hIndex->s32HeaderFd, (off_t)PVR_CYC_INFO_OFFSET))
    {
        HI_ERR_PVR("Write cyc info err\n");
        return HI_FAILURE;
    }
    /* after rewind case, update u64ValidSize in PVR_IDX_HEADER_INFO_S */
    if (0 > PVR_WRITE((void *)&(pstCycMgr->u64MaxCycSize), sizeof(HI_U64),
               hIndex->s32HeaderFd, (off_t)(PVR_CYC_INFO_OFFSET + sizeof(PVR_CYC_HEADER_INFO_S))))
    {
        HI_ERR_PVR("Write maxsize err\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/* get the header struct info from index file */
STATIC INLINE HI_S32 PVRIndexGetHeaderInfo(HI_S32 s32Fd, PVR_IDX_HEADER_INFO_S* pstHeadInfo)
{
    HI_S32 s32ReadRet = sizeof(PVR_IDX_HEADER_INFO_S);

    s32ReadRet = PVR_READ(pstHeadInfo, sizeof(PVR_IDX_HEADER_INFO_S), s32Fd, 0);
    if (s32ReadRet != (HI_S32)sizeof(PVR_IDX_HEADER_INFO_S))
    {
        HI_ERR_PVR("read Header info err, ret:%d, fd:%d, size:%d\n", s32ReadRet, s32Fd, s32ReadRet);
        memset(pstHeadInfo, 0, sizeof(PVR_IDX_HEADER_INFO_S));
        return HI_FAILURE;
    }

    if (PVR_INDEX_HEADER_CODE != pstHeadInfo->u32StartCode)
    {
        HI_ERR_PVR("Header info StartCode:0x%x, No head at this file, still play.\n", pstHeadInfo->u32StartCode);
        memset(pstHeadInfo, 0, sizeof(PVR_IDX_HEADER_INFO_S));
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* read the header info, and allowed without header info */
static HI_S32 PVRIndexReadHeaderInfo(PVR_INDEX_HANDLE handle)
{
    PVR_IDX_HEADER_INFO_S stIdxHeaderInfo;
    HI_S32 s32Ret = HI_SUCCESS;

    memset(&stIdxHeaderInfo, 0, sizeof(PVR_IDX_HEADER_INFO_S));

    /* means not found the header info in this index file */
    if (HI_SUCCESS != PVRIndexGetHeaderInfo(handle->s32HeaderFd, &stIdxHeaderInfo))
    {
        return HI_FAILURE;
    }
    else
    {
        handle->u32IdxStartOffsetLen =  stIdxHeaderInfo.u32HeaderLen;
        handle->stCycMgr.bIsRewind = (HI_BOOL)(stIdxHeaderInfo.stCycInfo.u32IsRewind);
        handle->stCycMgr.u32StartFrame = stIdxHeaderInfo.stCycInfo.u32StartFrame;
        handle->stCycMgr.u32EndFrame = stIdxHeaderInfo.stCycInfo.u32EndFrame;
        handle->stCycMgr.u32LastFrame = stIdxHeaderInfo.stCycInfo.u32LastFrame;
        handle->stCycMgr.u64MaxCycSize = stIdxHeaderInfo.u64ValidSize;
        HI_WARN_PVR("(start, end, last)=(%u, %u, %u)\n", handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame, handle->stCycMgr.u32LastFrame);
#ifdef HI_PVR_EXTRA_BUF_SUPPORT
        memcpy(&handle->stFileCycInfo, &stIdxHeaderInfo.stCycInfo, sizeof(PVR_CYC_HEADER_INFO_S));
#endif
        PVR_INDEX_LOCK(&(handle->stMutex));
        handle->u32ReadFrame = handle->stCycMgr.u32StartFrame;
        PVR_INDEX_UNLOCK(&(handle->stMutex));

        if (handle->stCycMgr.u32LastFrame == 0)
        {
            HI_ERR_PVR("Index last frame is 0!\n");
            return HI_FAILURE;
        }
        PVRINDEX_GET_ENTRY_BYNUM(handle, &handle->stStartFrame, handle->stCycMgr.u32StartFrame, s32Ret);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PVR("Get start frame(%u) failed, ret = 0x%08x!\n", handle->stCycMgr.u32StartFrame, s32Ret);
        }
        PVRINDEX_GET_ENTRY_BYNUM(handle, &handle->stLastFrame, handle->stCycMgr.u32LastFrame, s32Ret);
        if (HI_SUCCESS != s32Ret)
        {
            handle->stCycMgr.u32LastFrame = (0 == handle->stCycMgr.u32LastFrame) ? 0 : handle->stCycMgr.u32LastFrame - 1;
            HI_ERR_PVR("Get last frame(%u) failed, ret = 0x%08x!\n", handle->stCycMgr.u32LastFrame, s32Ret);
        }
        PVRINDEX_GET_ENTRY_BYNUM(handle, &handle->stEndFrame, handle->stCycMgr.u32EndFrame, s32Ret);
        if (HI_SUCCESS != s32Ret)
        {
            handle->stCycMgr.u32EndFrame = (0 == handle->stCycMgr.u32EndFrame) ? 0 : handle->stCycMgr.u32EndFrame - 1;
            HI_ERR_PVR("Get end frame(%u) failed, ret = 0x%08x!\n", handle->stCycMgr.u32EndFrame, s32Ret);
        }
    }

    return HI_SUCCESS;
}


static HI_S32 PVRIndexWriteIndexInfo(PVR_INDEX_HANDLE hIndex, PVR_REC_INDEX_INFO_S *pstIdxInfo)
{
    HI_S32 s32WriteRet;
    PVR_IDX_HEADER_INFO_S stIdxHeaderInfo = {0};

    if (HI_SUCCESS != PVRIndexGetHeaderInfo(hIndex->s32HeaderFd, &stIdxHeaderInfo))
    {
        HI_ERR_PVR("Can't get index header info.\n");
        return HI_FAILURE;
    }
    
    s32WriteRet = PVR_WRITE(pstIdxInfo, 
                            sizeof(PVR_REC_INDEX_INFO_S), 
                            hIndex->s32HeaderFd, 
                            PVR_GET_IDX_INFO_OFFSET(stIdxHeaderInfo));

    if (s32WriteRet != sizeof(PVR_REC_INDEX_INFO_S))
    {
        HI_ERR_PVR("Write index info fail ret=0x%x\n", s32WriteRet);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/* initialize */
STATIC INLINE HI_VOID PVRIndexSetDftAttr(PVR_INDEX_HANDLE handle)
{
    HI_INFO_PVR("index set default attr.\n");

    handle->u64GlobalOffset = 0;
    handle->u64OverflowOffset = 0;
    handle->u64FileSizeGlobal = 0;
    handle->u32PauseFrame  = 0;
    handle->u64PauseOffset = PVR_INDEX_PAUSE_INVALID_OFFSET;
    PVR_INDEX_LOCK(&(handle->stMutex));
    handle->u32ReadFrame  = 0;
    PVR_INDEX_UNLOCK(&(handle->stMutex));
    handle->u32WriteFrame = 0;
    handle->u16RecLastIframe = PVR_INDEX_INVALID_I_FRAME_OFFSET;
    handle->u32RecLastValidPtsMs = PVR_INDEX_INVALID_PTSMS;
    handle->u32RecPicParser = 0xffffffff;
    handle->u32RecFirstFrmTimeMs = 0;
    handle->u32EntryVersion = 0;
    handle->s32WriteFd = PVR_FILE_INVALID_FILE;
    handle->s32ReadFd = PVR_FILE_INVALID_FILE;
    handle->s32SeekFd = PVR_FILE_INVALID_FILE;
    handle->s32HeaderFd = PVR_FILE_INVALID_FILE;
    handle->u32IdxStartOffsetLen = 0;
    memset(&handle->stCurPlayFrame, 0x00, sizeof(handle->stCurPlayFrame));
    memset(&handle->stCurRecFrame, 0x00, sizeof(handle->stCurRecFrame));
    memset(&handle->stIndexFileAttr, 0x00, sizeof(handle->stIndexFileAttr) );
    memset(&handle->stCycMgr, 0x00, sizeof(handle->stCycMgr));
    memset(handle->szIdxFileName, 0x00, PVR_MAX_FILENAME_LEN);
#ifdef HI_PVR_EXTRA_BUF_SUPPORT
    handle->u32FrameIndex = 0;
    memset(&handle->stFileCycInfo, 0x00, sizeof(handle->stFileCycInfo));
    handle->pExtraCacheAddr = HI_NULL;
#endif
    return;
}

static HI_S32 PVRIndexCheckFileCorrect(PVR_INDEX_HANDLE handle, HI_U64 u64FileSize)
{
    HI_U32 u32IndexEntryNum;
    HI_S32 s32Ret;
    HI_S32 s32TmpOffset;
    HI_U64 u64IndexFileSize;
    HI_U64 u64GlobalOffset = 0;
    HI_U32 u32FrameSize = 0;
    PVR_INDEX_ENTRY_NEW_S stEndEntry;
    memset(&stEndEntry, 0x00, sizeof(stEndEntry));

    s32TmpOffset = (HI_S32)pvr_lseek(handle->s32HeaderFd, 0, SEEK_CUR);
    if (s32TmpOffset < 0)
    {
        HI_ERR_PVR("Index file can't seek to 0\n");
        return HI_FAILURE;
    }

    u64IndexFileSize = (HI_U64)pvr_lseek(handle->s32HeaderFd, 0, SEEK_END);
    (HI_VOID)pvr_lseek(handle->s32HeaderFd, s32TmpOffset, SEEK_SET);

    u32IndexEntryNum = (HI_U32)((u64IndexFileSize - (HI_U64)handle->u32IdxStartOffsetLen)/(HI_U64)handle->u32EntryLength);
    if ((HI_FALSE == handle->stCycMgr.bIsRewind) || 
        (0 == handle->stCycMgr.u64MaxCycSize))      //not rewind
    {
        if ((handle->stCycMgr.u32EndFrame > (u32IndexEntryNum - 1)) || 
            (handle->stCycMgr.u32LastFrame > (u32IndexEntryNum - 1)))
        {
            HI_WARN_PVR("HeadInfo's CycInfo.EndFrame(%u) or LastFrame(%u) > indexEntryNum(%u).\n", 
                         handle->stCycMgr.u32EndFrame, handle->stCycMgr.u32LastFrame, (u32IndexEntryNum - 1));
            handle->stCycMgr.u32EndFrame = u32IndexEntryNum - 1;
            handle->stCycMgr.u32LastFrame = u32IndexEntryNum - 1;
        }
        PVRINDEX_GET_ENTRY_BYNUM(handle, &stEndEntry, handle->stCycMgr.u32EndFrame, s32Ret);
        if (s32Ret == HI_SUCCESS)
        {
            u64GlobalOffset = PVRINDEX_GET_GLOBALOFFSET(handle->u32EntryVersion, &stEndEntry);
            u32FrameSize = PVRINDEX_GET_FRAMESIZE(handle->u32EntryVersion, &stEndEntry);
            if (u64GlobalOffset + u32FrameSize > u64FileSize)
            {
                HI_U32 u32CurEndNum = handle->stCycMgr.u32EndFrame;
                while (u64GlobalOffset + u32FrameSize > u64FileSize)
                {
                    if (u32CurEndNum == 0)
                    {
                        HI_ERR_PVR("Find end frm to start!\n");
                        return HI_FAILURE;
                    }
                    u32CurEndNum--;
                    PVRINDEX_GET_ENTRY_BYNUM(handle, &stEndEntry, u32CurEndNum, s32Ret);
                    if (HI_SUCCESS != s32Ret)
                    {
                        HI_ERR_PVR("Get frm %d failed, ret = 0x%08x\n", u32CurEndNum, s32Ret);
                        return HI_FAILURE;
                    }
                    u64GlobalOffset = PVRINDEX_GET_GLOBALOFFSET(handle->u32EntryVersion, &stEndEntry);
                    u32FrameSize = PVRINDEX_GET_FRAMESIZE(handle->u32EntryVersion, &stEndEntry);
                }
                handle->stCycMgr.u32EndFrame = u32CurEndNum;
                handle->stCycMgr.u32LastFrame = u32CurEndNum;
            }
            return HI_SUCCESS;
        }
        else
        {
            HI_ERR_PVR("Get endfrm %d failed! ret %x\n", handle->stCycMgr.u32EndFrame, s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}
#ifndef HI_PVR_EXTRA_BUF_SUPPORT
static HI_S32 PVRCacheWriteIdx(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S* pstEntry, 
                                    HI_S32 s32Num, HI_U32 u32Offset,HI_BOOL* bWriteFlag)
{
    if (0 == handle->u32EntryVersion)
    {
#if 0  /*obsolete APIs*/        
        return PVRIndex_CacheWriteIdx0(handle, pstEntry, s32Num, u32Offset, bDirectFlag, bWriteFlag);
#endif
        HI_ERR_PVR("not support to record using old index version\n");
        return HI_FAILURE;
    }
    else
    {
        return PVRIndex_CacheWriteIdx1(handle, pstEntry, s32Num, u32Offset, bWriteFlag);
    }
}
#endif
static HI_S32 PVRIndexGetFBwardIPBFrameNum(PVR_INDEX_HANDLE handle,
                                           PVR_INDEX_DIRECTION_E enDirection, 
                                           HI_U32 u32FrameType, 
                                           HI_U32 u32CurFrameNum, 
                                           HI_U32 *pu32NextFrameNum)
{
    HI_S32 s32NextFrameNum = (HI_S32)u32CurFrameNum;
    HI_S32 ret = HI_SUCCESS;
    PVR_INDEX_ENTRY_NEW_S stIdxEntry;
    PVR_CYC_MGR_S *pstCycMgr = HI_NULL;
    HI_U16 u16FrameTypeAndGop = 0;
    
    HI_ASSERT_RET(handle != NULL);
    HI_ASSERT_RET(pu32NextFrameNum != NULL);

    memset(&stIdxEntry, 0x00, sizeof(stIdxEntry));
    if ((PVR_INDEX_FRAME_I > u32FrameType) || (PVR_INDEX_FRAME_IDR < u32FrameType))
    {
        HI_ERR_PVR("Frame type %d id invalide!", u32FrameType);
        return PVR_INDEX_ERR_INVALID;
    }

    pstCycMgr = &handle->stCycMgr;

    if (HI_TRUE != PVRIndexIsFrameValid(u32CurFrameNum, pstCycMgr))
    {
        HI_ERR_PVR("CurFrm %d is invalid start/end/last %d/%d/%d.\n", 
                    u32CurFrameNum, pstCycMgr->u32StartFrame, pstCycMgr->u32EndFrame, pstCycMgr->u32LastFrame);
        return PVR_INDEX_ERR_INVALID;
    }

    while (1)
    {
        if (PVR_INDEX_DIRECTION_BACKWARD == enDirection)
            s32NextFrameNum--;
        else
            s32NextFrameNum++;

        if (pstCycMgr->u32EndFrame <= pstCycMgr->u32StartFrame)
        {
            if (PVR_INDEX_DIRECTION_BACKWARD == enDirection)
                s32NextFrameNum = (0 > s32NextFrameNum) ? (HI_S32)pstCycMgr->u32LastFrame : s32NextFrameNum;
            else
                s32NextFrameNum = (pstCycMgr->u32LastFrame < (HI_U32)s32NextFrameNum) ? 0 : s32NextFrameNum;
        }

        if (HI_TRUE != PVRIndexIsFrameValid(s32NextFrameNum, pstCycMgr))
        {
            HI_WARN_PVR("next frame number %d is invalid. start=%d end=%d last=%d\n",
                        s32NextFrameNum,
                        pstCycMgr->u32StartFrame,
                        pstCycMgr->u32EndFrame,
                        pstCycMgr->u32LastFrame);
            if (PVR_INDEX_DIRECTION_BACKWARD == enDirection)
                return HI_ERR_PVR_FILE_TILL_START;
            else
                return HI_ERR_PVR_FILE_TILL_END;
        }
        PVRINDEX_GET_ENTRY_BYNUM(handle, &stIdxEntry, s32NextFrameNum, ret);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_PVR("Get frame %d failed! ret=%#x. s/e/l %d/%d/%d\n",
                        s32NextFrameNum, pstCycMgr->u32StartFrame,
                        pstCycMgr->u32EndFrame, pstCycMgr->u32LastFrame, ret);
            return ret;
        }
        u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion,&stIdxEntry);
        if (PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion,u16FrameTypeAndGop, u32FrameType))
        {
            break;
        }
        if ((PVR_INDEX_FRAME_I == u32FrameType) 
            && PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR))
        {
            break;
        }
    }
    
    *pu32NextFrameNum = (HI_U32)s32NextFrameNum;
    return HI_SUCCESS;
}


static HI_VOID PVRIndexCheckReadFrmValidWhenTimeshift(PVR_INDEX_HANDLE hIndex)
{
    if (hIndex->bIsPlay == HI_FALSE)
        return ;

    if (hIndex->bRecReachPlay == HI_TRUE)
    {
        return ;
    }

    if (PVRIndexIsFrameValid(hIndex->u32ReadFrame, &hIndex->stCycMgr) == 0)
    {
        HI_WARN_PVR("Rec cover play!!!! S/E/L/R:%d/%d/%d/%d\n",
                     hIndex->stCycMgr.u32StartFrame, hIndex->stCycMgr.u32EndFrame,
                     hIndex->stCycMgr.u32LastFrame,hIndex->u32ReadFrame);
        hIndex->bRecReachPlay = HI_TRUE;
    }
}


static HI_BOOL PVRIndexCheckRewind(PVR_INDEX_HANDLE hIndex,
                                   PVR_CYC_MGR_S *pstCycMgr,
                                   PVR_INDEX_ENTRY_NEW_S *pstIdxEntry)
{
    HI_U64 u64Offset = 0;
    if (PVR_INDEX_REWIND_BY_TIME == pstCycMgr->enRewindType)
    {
        /*如果当前是时间回绕并且index落后ts，回绕处理实际发生在
          index的globaloffset超过TimeRewindMaxSize时，所以返回false*/
        if (hIndex->bTimeRewinding == HI_TRUE)
        {
            return HI_FALSE;
        }
        
//        if((pstIdxEntry->u32DisplayTimeMs - (pstCycMgr->u32MaxCycTimeInMs * pstCycMgr->u32CycTimes)) > pstCycMgr->u32MaxCycTimeInMs)
        if((hIndex->u32TotalRecTimeMs - (pstCycMgr->u32MaxCycTimeInMs * pstCycMgr->u32CycTimes)) > pstCycMgr->u32MaxCycTimeInMs)
        {
            return HI_TRUE;
        }
    }
    else
    {
        u64Offset = PVRINDEX_GET_OFFSET(hIndex->u32EntryVersion, pstIdxEntry);
        if (u64Offset > pstCycMgr->u64MaxCycSize)
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}


static HI_VOID PVRIndexAddToRefList(PVR_REC_REF_LIST_S *pstRefList, HI_S32 s32CurPoc, HI_U32 u32FrmNum)
{
    PVR_REC_REF_LIST_NODE_S *pstListNode = HI_NULL;

    if (pstRefList == NULL)
        return ;

    //CNComment:参考列表已经到达最大值了，则清掉开始的参考节点
    if (pstRefList->u32Write == PVR_INDEX_REF_LIST_NODE_NUM)
    {
        pstRefList->u32Write = 0;
    }

    pstListNode = pstRefList->pstRefHead + pstRefList->u32Write;
    pstListNode->u32FrmNum = u32FrmNum;
    pstListNode->s32Poc    = s32CurPoc;
    pstRefList->u32Write++;

//    pvrdprint("~~~~~~write %d frmnum %d poc %d\n", pstRefList->u32Write,pstListNode->u32FrmNum,pstListNode->s32Poc);

}

//CNComment:解码序只会前向参考
static HI_S32 PVRIndexPocToFrmNum(PVR_REC_REF_LIST_S *pstRefList, HI_S32 s32Poc, HI_U32 *pu32FrmNum)
{
    HI_U32 i;
    PVR_REC_REF_LIST_NODE_S *pstNode = HI_NULL;

    if (pstRefList == NULL)
        return HI_FAILURE;

    i = (pstRefList->u32Write + PVR_INDEX_REF_LIST_NODE_NUM - 1) % PVR_INDEX_REF_LIST_NODE_NUM;
    pstNode = pstRefList->pstRefHead + i;
    while (pstNode->s32Poc != s32Poc)
    {
        i = (i == 0)?(PVR_INDEX_REF_LIST_NODE_NUM - 1):(i - 1);
        //CNComment:找完整个list仍未找到
        if (i == pstRefList->u32Write)
        {
            HI_ERR_PVR("Can't find frm num\n");
            return HI_FAILURE;
        }
        pstNode = pstRefList->pstRefHead + i;
    }

    *pu32FrmNum = pstNode->u32FrmNum;

//    pvrdprint(" ==== num/poc %d/%d i %d\n", pstNode->u32FrmNum, pstNode->s32Poc, i);

    return HI_SUCCESS;
}

static INLINE HI_VOID PVRIndexAssignIndexEntry(PVR_INDEX_HANDLE hIndex,
                                               const HI_MPI_DMX_REC_INDEX_S *pstDmxIndexInfo,
                                               PVR_INDEX_ENTRY_NEW_S *pstIdxEntry)
{
    HI_U32 i;
    HI_U32 u32CurFrmTimeMs = 0;
    HI_U32 u32TimeNow = 0;
    HI_U32 u32DisplayTimeMs = 0;
    HI_U64 u64GlobalOffset = 0;
    HI_U64 u64Offset = 0;
    HI_U16 u16FrameTypeAndGop = 0;
    HI_U32 u32Version = hIndex->u32EntryVersion;
    HI_U32 u32RefNum = 0;

    hIndex->u64GlobalOffset = pstDmxIndexInfo->u64GlobalOffset - hIndex->u64DeltaGlobalOffset;
    hIndex->u32DmxClkTimeMs = pstDmxIndexInfo->u32DataTimeMs;

    if (PVR_INDEX_INVALID_I_FRAME_OFFSET != hIndex->u16RecLastIframe)
    {
        hIndex->u16RecLastIframe++;
    }
    else
    {
        hIndex->u16RecLastIframe = 0;
    }

    if ((HI_UNF_FRAME_TYPE_I == pstDmxIndexInfo->enFrameType) || 
        (HI_UNF_FRAME_TYPE_IDR == pstDmxIndexInfo->enFrameType))
    {
        hIndex->u16RecLastIframe = 0;
    }

    if ((PVR_INDEX_INVALID_PTSMS != pstDmxIndexInfo->u32PtsMs) && (0 != pstDmxIndexInfo->u32PtsMs))
    {
        hIndex->u32RecLastValidPtsMs = pstDmxIndexInfo->u32PtsMs;
    }

    if ((0 == hIndex->stCycMgr.u32CycTimes) && (-1 == hIndex->stCycMgr.u32LastFrame))
    {
        hIndex->u32RecFirstFrmTimeMs = hIndex->u32DmxClkTimeMs;
        u32CurFrmTimeMs = 0;
        PVRINDEX_SET_DISPTIME(u32Version,pstIdxEntry,0);
    }
    else
    {
        u32TimeNow = hIndex->u32DmxClkTimeMs;
        if (u32TimeNow >= hIndex->u32TotalRecTimeMs)
        {
            u32CurFrmTimeMs = u32TimeNow - hIndex->u32RecFirstFrmTimeMs;
        }
        else
        {
            HI_WARN_PVR("The time rewinded firstTimeMs(%u)\n", hIndex->u32RecFirstFrmTimeMs);
            u32DisplayTimeMs = PVRINDEX_GET_DISPTIME(u32Version, &(hIndex->stCurRecFrame));
            if ((hIndex->u32FRollTime*PVR_INDEX_SCD_WRAP_MS + u32TimeNow) < u32DisplayTimeMs)
            {
                hIndex->u32FRollTime++;
            }
            u32CurFrmTimeMs = hIndex->u32FRollTime*PVR_INDEX_SCD_WRAP_MS - hIndex->u32RecFirstFrmTimeMs + u32TimeNow;
            HI_WARN_PVR("Cur indexEntry.u32DisplayTimeMs = %u set the reference time is %u, displayTimeMs increase = %u\n",
                        u32DisplayTimeMs, u32TimeNow,u32CurFrmTimeMs);
        }
    }

    if ((hIndex->bNoIDRFrm == HI_TRUE) && (pstDmxIndexInfo->enFrameType == HI_UNF_FRAME_TYPE_IDR))
        hIndex->bNoIDRFrm = HI_FALSE;

    if (pstDmxIndexInfo->enFrameType == HI_UNF_FRAME_TYPE_B)
        hIndex->stRecIdxInfo.stIdxInfo.u32BFrmCnt++;

    u16FrameTypeAndGop = ((pstDmxIndexInfo->enFrameType) & 0x7) << 13 | (hIndex->u16RecLastIframe & 0x1fff);
    u64GlobalOffset    = pstDmxIndexInfo->u64GlobalOffset - hIndex->u64DeltaGlobalOffset + hIndex->u64OverflowOffset;
    u64Offset          = pstDmxIndexInfo->u64GlobalOffset - hIndex->u64DeltaGlobalOffset + hIndex->u64OverflowOffset;
    PVRINDEX_SET_FRAMETYPEANDGOP(u32Version, pstIdxEntry, u16FrameTypeAndGop);
    PVRINDEX_SET_GLOBALOFFSET(u32Version, pstIdxEntry, u64GlobalOffset);
    PVRINDEX_SET_OFFSET(u32Version, pstIdxEntry, u64Offset);
    PVRINDEX_SET_FRAMESIZE(u32Version, pstIdxEntry, pstDmxIndexInfo->u32FrameSize);
    PVRINDEX_SET_PTS(u32Version, pstIdxEntry, hIndex->u32RecLastValidPtsMs);
    PVRINDEX_SET_INDEXTYPE(u32Version, pstIdxEntry, (HI_U16)hIndex->enIndexType);
    PVRINDEX_SET_LASTFRAMEOFTT(u32Version, pstIdxEntry, 0);
    PVRINDEX_SET_CYCTIMES(u32Version, pstIdxEntry, hIndex->stCycMgr.u32CycTimes);    
    PVRINDEX_SET_CURPOC(u32Version, pstIdxEntry, (HI_S32)(pstDmxIndexInfo->s16CurPoc));
    PVRINDEX_SET_REFCNT(u32Version, pstIdxEntry, (HI_U8)(pstDmxIndexInfo->u16RefPocCnt));

    /* In case the recording data lose, compensate the current time */
    if ((u32CurFrmTimeMs - hIndex->u32LastDispTime) >= 1000)
    {
        hIndex->u32DeltaDispTimeMs += (u32CurFrmTimeMs - hIndex->u32LastDispTime);
    }

    hIndex->u32TotalRecTimeMs = u32CurFrmTimeMs - hIndex->u32DeltaDispTimeMs;
#if 0
//    if (!hIndex->stTimeStamp.bNeedAdjustByPts)
    {
        u32DisplayTimeMs = u32CurFrmTimeMs - hIndex->u32DeltaDispTimeMs;
        PVRINDEX_SET_DISPTIME(u32Version,pstIdxEntry,u32DisplayTimeMs);
    }
//    else
#else
    {
        u32DisplayTimeMs = PVRIndexGetRecTimeStamp(hIndex, pstDmxIndexInfo);
        PVRINDEX_SET_DISPTIME(u32Version,pstIdxEntry,u32DisplayTimeMs);
    }
#endif
    hIndex->u32LastDispTime = u32CurFrmTimeMs;

    hIndex->u32LastDispTime = u32CurFrmTimeMs;
    for (i = 0; i < pstDmxIndexInfo->u16RefPocCnt; i++)
    {
        if (16 <= i)
        {
            HI_ERR_PVR("the reference frame num(%u) is more than 16\n", pstDmxIndexInfo->u16RefPocCnt);
            break;
        }
        PVRIndexPocToFrmNum(&hIndex->stRefList, pstDmxIndexInfo->as16RefPoc[i], &u32RefNum);
        PVRINDEX_SET_REFINFO(u32Version,pstIdxEntry,u32RefNum,i);
    }
}


/*必须要求u32PreCurFrm为I帧，由外部保证有效性 */
static HI_VOID PVRIndexSubGopInfo(PVR_INDEX_HANDLE hIndex,
                                     PVR_REC_INDEX_INFO_S *pstRecIdxInfo, 
                                     HI_U32 u32PreCurFrm)
{
    HI_U32 u32GopSizeSeg;
    PVR_INDEX_ENTRY_NEW_S stPreEntryTmp;
    HI_U16 u16FrameTypeAndGop = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    memset(&stPreEntryTmp, 0x00, sizeof(stPreEntryTmp));
    pstRecIdxInfo->stIdxInfo.u32GopTotalNum--;
    PVRINDEX_GET_ENTRY_BYNUM(hIndex, &stPreEntryTmp, u32PreCurFrm, s32Ret);
    if (HI_SUCCESS == s32Ret)
    {
        u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(hIndex->u32EntryVersion, &stPreEntryTmp);
        u32GopSizeSeg = ((u16FrameTypeAndGop & 0x1fff) + 1)/10;
        u32GopSizeSeg = (u32GopSizeSeg > 12) ? 12 : u32GopSizeSeg;
        pstRecIdxInfo->stIdxInfo.u32GopSizeInfo[u32GopSizeSeg]--;
    }

    return;
}

static HI_VOID PVRIndexAddGopInfo(PVR_INDEX_HANDLE hIndex,
                                     PVR_REC_INDEX_INFO_S *pstRecIdxInfo,
                                     PVR_INDEX_ENTRY_NEW_S *pstIdxEntry)
{
    HI_U32 u32GopSizeSeg;

    pstRecIdxInfo->stIdxInfo.u32GopTotalNum++;

    if (pstRecIdxInfo->u32LastGopSize > pstRecIdxInfo->stIdxInfo.u32MaxGopSize)
    {
        pstRecIdxInfo->stIdxInfo.u32MaxGopSize = pstRecIdxInfo->u32LastGopSize;
    }

    if (0 != pstRecIdxInfo->u32LastGopSize)
    {
        u32GopSizeSeg = (pstRecIdxInfo->u32LastGopSize/10);
        u32GopSizeSeg = (u32GopSizeSeg > 12) ? 12 : u32GopSizeSeg;
        pstRecIdxInfo->stIdxInfo.u32GopSizeInfo[u32GopSizeSeg]++;
    }

    return;
}


static INLINE HI_VOID PVRIndexUpdateTotalFrmNum(PVR_REC_INDEX_INFO_S *pstRecIdxInfo,
                                                PVR_CYC_MGR_S *pstCycMgr)
{
    pstRecIdxInfo->u32MagicWord = PVR_REC_INDEX_MAGIC_WORD;

    if ((0 != pstCycMgr->u32CycTimes) && (0 != pstCycMgr->u32LastFrame))
    {
        pstRecIdxInfo->stIdxInfo.u32FrameTotalNum = pstCycMgr->u32LastFrame -
                                                    (pstCycMgr->u32StartFrame + pstCycMgr->u32LastFrame - pstCycMgr->u32EndFrame)
                                                    %pstCycMgr->u32LastFrame;
    }
    else
    {
        pstRecIdxInfo->stIdxInfo.u32FrameTotalNum = pstCycMgr->u32LastFrame + 1;
    }
}


static HI_VOID PVRIndexPushStartFrame(PVR_INDEX_HANDLE hIndex, 
                                      PVR_CYC_MGR_S *pstCycMgr,
                                      PVR_REC_INDEX_INFO_S *pstIdxInfo,
                                      HI_U64 u64NextEndOffset)
{
    HI_U32 u32NextIFrm;
    HI_S32 s32Ret;
    PVR_INDEX_ENTRY_NEW_S stEntryTmp;
    
    memset(&stEntryTmp, 0x00, sizeof(stEntryTmp));
    
    s32Ret = PVRIndexGetFBwardIPBFrameNum(hIndex, PVR_INDEX_DIRECTION_FORWARD, PVR_INDEX_FRAME_I, 
                                          pstCycMgr->u32StartFrame, &u32NextIFrm);
    if (HI_SUCCESS == s32Ret)
    {
        PVRIndexSubGopInfo(hIndex, pstIdxInfo, PVR_IDX_PREV_POS_IN_CYC(u32NextIFrm,(pstCycMgr->u32LastFrame + 1)));
        pstCycMgr->u32StartFrame = u32NextIFrm;
        PVRINDEX_GET_ENTRY_BYNUM(hIndex , &stEntryTmp, pstCycMgr->u32StartFrame, s32Ret);
        if (s32Ret == HI_SUCCESS)
        {
            pstCycMgr->u64StartFrameOffset = PVRINDEX_GET_OFFSET(hIndex->u32EntryVersion, &stEntryTmp);
            memcpy(&hIndex->stStartFrame, &stEntryTmp, sizeof(PVR_INDEX_ENTRY_NEW_S));
        }
        else
        {
            HI_ERR_PVR("!!!Get start idxentry failed ret %x, s/e/l %d/%d/%d\n",
                       s32Ret, pstCycMgr->u32StartFrame, pstCycMgr->u32EndFrame, pstCycMgr->u32LastFrame);
            pstCycMgr->u64StartFrameOffset = u64NextEndOffset;
        }
    }
    else
    {
        HI_ERR_PVR("!!!!Get next I frm failed ret %x, s/e/l %d/%d/%d\n",
                   s32Ret, pstCycMgr->u32StartFrame, pstCycMgr->u32EndFrame, pstCycMgr->u32LastFrame);
        pstCycMgr->u32StartFrame += 1;
        pstCycMgr->u64StartFrameOffset = u64NextEndOffset;
    }
}

static HI_VOID PVRIndexUpdateCycMgrAndRecInfoWhenRewind(PVR_INDEX_HANDLE hIndex, 
                                                        PVR_CYC_MGR_S *pstCycMgr, 
                                                        PVR_REC_INDEX_INFO_S * pstIdxInfo, 
                                                        PVR_INDEX_ENTRY_NEW_S *pstEndEntry)
{
    HI_U64 u64NextEndOffset;
    HI_U64 u64Offset = 0;
    HI_U32 u32FrameSize = 0;
    HI_U16 u16FrameTypeAndGop = 0;
    
    u64Offset = PVRINDEX_GET_OFFSET(hIndex->u32EntryVersion, pstEndEntry);
    u32FrameSize = PVRINDEX_GET_FRAMESIZE(hIndex->u32EntryVersion, pstEndEntry);
    u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(hIndex->u32EntryVersion, pstEndEntry); 

    u64NextEndOffset = u64Offset + u32FrameSize;

    pstCycMgr->u32CycTimes++;
    pstCycMgr->u32LastFrame = pstCycMgr->u32EndFrame;
    memcpy(&hIndex->stEndFrame, pstEndEntry, sizeof(PVR_INDEX_ENTRY_NEW_S));
    memcpy(&hIndex->stLastFrame, pstEndEntry, sizeof(PVR_INDEX_ENTRY_NEW_S));

    if (pstCycMgr->u32CycTimes == 1)
    {
        PVRIndexPushStartFrame(hIndex, pstCycMgr, pstIdxInfo, u64NextEndOffset);
    }
    else
    {
        /*if the bitrate becomes smaller,  the last may be smaller than start when rewind, so we must make sure the start is valide.
         |---------------------------------------------------|---------------------|----------|
                                                      end & cur last            start       pre last
        */
        /*CNComments:码流码率变小，可能导致回绕时start无效的情况，需要进行处理*/
        if (pstCycMgr->u32StartFrame >= pstCycMgr->u32LastFrame)
        {
            pstCycMgr->u32StartFrame = 0;
            PVRIndexPushStartFrame(hIndex, pstCycMgr, pstIdxInfo, u64NextEndOffset);
        }

        //we reserve 2 frames for this case:
        //read is equal to start, and we are pushing start at the monent. It will cause  the function of checking read invalide fail
        if ((pstCycMgr->u32StartFrame < 2) ||
            (pstCycMgr->u32StartFrame == PVR_IDX_NEXT_N_POS_IN_CYC(pstCycMgr->u32EndFrame, 2, (pstCycMgr->u32LastFrame + 1))) ||
            (PVRIndexCheckEndOffsetCoverStartOffset(u64Offset, u64NextEndOffset, pstCycMgr->u64StartFrameOffset, pstCycMgr->u64MaxCycSize)))
        {
            do
            {
                PVRIndexPushStartFrame(hIndex, pstCycMgr, pstIdxInfo, u64NextEndOffset);
            }while(PVRIndexCheckEndOffsetCoverStartOffset(u64Offset,u64NextEndOffset, 
                                                          pstCycMgr->u64StartFrameOffset, pstCycMgr->u64MaxCycSize));
        }
    }

    pstCycMgr->u32EndFrame = 0;
    PVRIndexCheckReadFrmValidWhenTimeshift(hIndex);

    /**CNComment:如果是IDR帧或者码流中没有IDR帧只有I帧，才更新gopinfo*/
    if (PVRINDEX_IS_CERTAINTYPE(hIndex->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR) 
        || (hIndex->bNoIDRFrm && PVRINDEX_IS_CERTAINTYPE(hIndex->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_I)))
    {
        PVRIndexAddGopInfo(hIndex, pstIdxInfo, pstEndEntry);
    }

    pstIdxInfo->u32LastGopSize = (u16FrameTypeAndGop & 0x1fff) + 1;

    PVRIndexUpdateTotalFrmNum(pstIdxInfo, pstCycMgr);

    HI_INFO_PVR("Chn %d update cycmgr cyctimes %3d start %8d end %8d last %8d\n",
                hIndex->u32RecPicParser,
                pstCycMgr->u32CycTimes, 
                pstCycMgr->u32StartFrame, 
                pstCycMgr->u32EndFrame,
                pstCycMgr->u32LastFrame);
}


static HI_VOID PVRIndexUpdateCycMgrAndRecInfoWhenNotRewind(PVR_INDEX_HANDLE hIndex, 
                                                           PVR_CYC_MGR_S *pstCycMgr, 
                                                           PVR_REC_INDEX_INFO_S * pstIdxInfo,
                                                           PVR_INDEX_ENTRY_NEW_S *pstEndEntry)
{
    HI_U64 u64NextEndOffset;
    HI_U64 u64Offset = 0;
    HI_U32 u32FrameSize = 0;
    HI_U16 u16FrameTypeAndGop = 0;
    
    u64Offset = PVRINDEX_GET_OFFSET(hIndex->u32EntryVersion, pstEndEntry);
    u32FrameSize = PVRINDEX_GET_FRAMESIZE(hIndex->u32EntryVersion, pstEndEntry);
    u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(hIndex->u32EntryVersion, pstEndEntry);    
    
    u64NextEndOffset = u64Offset + u32FrameSize;
    memcpy(&hIndex->stEndFrame, pstEndEntry, sizeof(PVR_INDEX_ENTRY_NEW_S));

    //非回绕时，start  不动，只移动end  和last
    if (pstCycMgr->u32CycTimes == 0)
    {
        if (pstCycMgr->u32EndFrame == -1)
        {
            pstCycMgr->u32EndFrame = 0;
            pstCycMgr->u32LastFrame = 0;
            memcpy(&hIndex->stStartFrame, pstEndEntry, sizeof(PVR_INDEX_ENTRY_NEW_S));
        }
        else
        {
            pstCycMgr->u32EndFrame++;
            pstCycMgr->u32LastFrame++;
        }
        memcpy(&hIndex->stLastFrame, pstEndEntry, sizeof(PVR_INDEX_ENTRY_NEW_S));
    }
    else
    {
        //两种情况会移动start   
        //1. end  指向的offset  会覆盖prestart  指向的offset
        //2. end  赶上prestart
        if ((pstCycMgr->u32StartFrame == PVR_IDX_NEXT_N_POS_IN_CYC(pstCycMgr->u32EndFrame,2,(pstCycMgr->u32LastFrame + 1))) ||
            (PVRIndexCheckEndOffsetCoverStartOffset(u64Offset, u64NextEndOffset, 
                                                    pstCycMgr->u64StartFrameOffset, pstCycMgr->u64MaxCycSize)))
        {
            do
            {
                PVRIndexPushStartFrame(hIndex, pstCycMgr, pstIdxInfo, u64NextEndOffset);
            }while(PVRIndexCheckEndOffsetCoverStartOffset(u64Offset,u64NextEndOffset, 
                                                          pstCycMgr->u64StartFrameOffset, pstCycMgr->u64MaxCycSize));
        }

        pstCycMgr->u32EndFrame++;
        if (pstCycMgr->u32LastFrame < pstCycMgr->u32EndFrame)
        {
            pstCycMgr->u32LastFrame++;
            memcpy(&hIndex->stLastFrame, pstEndEntry, sizeof(PVR_INDEX_ENTRY_NEW_S));
        }
    }

    PVRIndexCheckReadFrmValidWhenTimeshift(hIndex);

    if (PVRINDEX_IS_CERTAINTYPE(hIndex->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_I) 
        || PVRINDEX_IS_CERTAINTYPE(hIndex->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR))
    {
        PVRIndexAddGopInfo(hIndex, pstIdxInfo, pstEndEntry);
    }

    pstIdxInfo->u32LastGopSize = (u16FrameTypeAndGop & 0x1fff) + 1;

    PVRIndexUpdateTotalFrmNum(pstIdxInfo, pstCycMgr);

    HI_INFO_PVR("Chn %d update cycmgr cyctimes %3d start %8d end %8d last %8d\n",
                hIndex->u32RecPicParser,
                pstCycMgr->u32CycTimes, 
                pstCycMgr->u32StartFrame, 
                pstCycMgr->u32EndFrame,
                pstCycMgr->u32LastFrame);
}


static HI_VOID PVRIndexUpdateCycMgrAndRecInfoWhenDisableRewind(PVR_INDEX_HANDLE hIndex, 
                                                               PVR_CYC_MGR_S *pstCycMgr, 
                                                               PVR_REC_INDEX_INFO_S * pstIdxInfo, 
                                                               PVR_INDEX_ENTRY_NEW_S *pstEndEntry)
{
    HI_U16 u16FrameTypeAndGop = 0;
    if (pstCycMgr->u32EndFrame == -1)
    {
        pstCycMgr->u32EndFrame = 0;
        pstCycMgr->u32LastFrame = 0;
    }
    else
    {
        pstCycMgr->u32EndFrame++;
        pstCycMgr->u32LastFrame++;
    }
    memcpy(&hIndex->stEndFrame, pstEndEntry, sizeof(PVR_INDEX_ENTRY_NEW_S));
    memcpy(&hIndex->stLastFrame, pstEndEntry, sizeof(PVR_INDEX_ENTRY_NEW_S));
    u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(hIndex->u32EntryVersion, pstEndEntry);
    if (PVRINDEX_IS_CERTAINTYPE(hIndex->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_I) 
        || PVRINDEX_IS_CERTAINTYPE(hIndex->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR))
    {
        PVRIndexAddGopInfo(hIndex, pstIdxInfo, pstEndEntry);
    }
    
    pstIdxInfo->u32LastGopSize = (u16FrameTypeAndGop & 0x1fff) + 1;

    PVRIndexUpdateTotalFrmNum(pstIdxInfo, pstCycMgr);

    HI_INFO_PVR("Chn %d Update cycmgr cyctimes %3d start %8d end %8d last %8d\n",
                hIndex->u32RecPicParser,
                pstCycMgr->u32CycTimes, 
                pstCycMgr->u32StartFrame, 
                pstCycMgr->u32EndFrame,
                pstCycMgr->u32LastFrame);
}


static HI_VOID PVRIndexGetRealOffset(PVR_INDEX_HANDLE hIndex,
                                     PVR_CYC_MGR_S *pstCycMgr,
                                     PVR_INDEX_ENTRY_NEW_S *pstIdxEntry)
{
    HI_U64 u64GlobalOffset = 0;
    HI_U64 u64Offset = 0;
    HI_U32 u32FrameSize = 0;
    HI_U32 u32DisplayTimeMs = 0;
    
    u64GlobalOffset = PVRINDEX_GET_GLOBALOFFSET(hIndex->u32EntryVersion, pstIdxEntry);
    //u64Offset = PVRINDEX_GET_OFFSET(hIndex->u32EntryVersion, pstIdxEntry);
    u32FrameSize = PVRINDEX_GET_FRAMESIZE(hIndex->u32EntryVersion, pstIdxEntry);
    u32DisplayTimeMs = PVRINDEX_GET_DISPTIME(hIndex->u32EntryVersion, pstIdxEntry);
    
    if(pstCycMgr->enRewindType == PVR_INDEX_REWIND_BY_TIME)
    {
        /*when the flag bTimeRewinding is true, it means that ts lead index. 
          The maxcycsize and timerewindmaxsize are equal to the size of ts.
          so, we process the rewind case only the global offset of index must equal or lead the ts
         */
        /*CNComments: bTimeRewinding表明当前是发生时间回绕时index落后，maxsize和TimeRewindMaxSize
                                     已经为ts文件的大小，回绕点后的index的globaloffset必须实际达到
                                     TimeRewindMaxSize时才进行回绕处理
             */
        if (hIndex->bTimeRewinding == HI_TRUE)
        {
            /*(TimeRewindMaxSize - MaxCycSize)---- the value is the last TimeRewindMaxSize*/
            u64Offset = u64GlobalOffset - (hIndex->u64TimeRewindMaxSize - pstCycMgr->u64MaxCycSize);
        }
        else
        {
            /*when index lead ts, the timerewindsize which is size-aligned may be bigger than the next frame globle offset, 
                        so , we will make sure that the next frame offset is valid

             |-------------------------------------|--------------|-----------|
                                             lastframe        curframe     maxcycsize
                    */
            /*CNComments: 时间回绕大小做了字节对齐处理，有可能超过上一帧的大小，
                                            因此，需要判断下一帧的文件偏移值是否正确有效
                    */
            if (hIndex->u64TimeRewindMaxSize > u64GlobalOffset)
            {
                u64Offset = u64GlobalOffset - (hIndex->u64TimeRewindMaxSize - pstCycMgr->u64MaxCycSize);
            }
            else
            {
                u64Offset = u64GlobalOffset - hIndex->u64TimeRewindMaxSize;
            }
            
        }

        /*In the following case, update MaxCycSize to avoid timeshift playing to read wrong position.
                Max1=Latest time rewind file size;  Max2=Current time rewind file size;
                
                |-----------------------------------------------------|-------|
                0                                                                                      Max1      Max2
            */
        if ((0 < pstCycMgr->u32CycTimes) && 
             (pstCycMgr->u64MaxCycSize <= (u64Offset + u32FrameSize)) &&
             ((u32DisplayTimeMs - (pstCycMgr->u32MaxCycTimeInMs * pstCycMgr->u32CycTimes)) <= pstCycMgr->u32MaxCycTimeInMs))
        {
            pstCycMgr->u64MaxCycSize = (u64Offset + u32FrameSize);
        }
    }
    else
    {
        u64Offset = u64GlobalOffset - pstCycMgr->u64MaxCycSize * pstCycMgr->u32CycTimes;
    }
    PVRINDEX_SET_OFFSET(hIndex->u32EntryVersion, pstIdxEntry, u64Offset);
}

static HI_VOID PVRIndexProcRewind(PVR_INDEX_HANDLE hIndex,
                                       PVR_CYC_MGR_S *pstCycMgr,
                                       PVR_REC_INDEX_INFO_S *pstRecInfo,
                                       PVR_INDEX_ENTRY_NEW_S *pstIdxEntry)
{
    HI_U64 u64GlobalOffset = 0;
    HI_U64 u64Offset = 0;

    u64GlobalOffset = PVRINDEX_GET_GLOBALOFFSET(hIndex->u32EntryVersion, pstIdxEntry);
    u64Offset = PVRINDEX_GET_OFFSET(hIndex->u32EntryVersion, pstIdxEntry);
    if (pstCycMgr->enRewindType == PVR_INDEX_REWIND_BY_TIME)
    {
        /*两种回绕处理机制
                    1.index超前ts，回绕大小为index的GlobalOffset，直接进行回绕处理，
                      时移场景下由播放控制ts数据与index同步
                    2.index落后ts，回绕大小为当前文件大小，回绕处理推迟到index的
                      GlobalOffset超过回绕大小的时候
                   */
        if (u64GlobalOffset < hIndex->u64FileSizeGlobal)   //ts lead to index
        {
            pstCycMgr->u64MaxCycSize = PVR_IDX_ALIGN_DATA((hIndex->u64FileSizeGlobal - hIndex->u64TimeRewindMaxSize));
            hIndex->bTimeRewinding = HI_TRUE;
            PVRIndexUpdateCycMgrAndRecInfoWhenNotRewind(hIndex, pstCycMgr, pstRecInfo, pstIdxEntry);
        }
        else    //index lead to ts
        {
            pstCycMgr->u64MaxCycSize = PVR_IDX_ALIGN_DATA((u64GlobalOffset - hIndex->u64TimeRewindMaxSize));
            PVRIndexUpdateCycMgrAndRecInfoWhenRewind(hIndex, pstCycMgr, pstRecInfo, pstIdxEntry);
            hIndex->bRewindFlagForIndex = HI_TRUE;

            //说明当前cache  中有回绕点，需要savefile  时进行处理
            hIndex->stIdxWriteCache.u32RewindCacheWrite = hIndex->stIdxWriteCache.u32Write;
            PVRINDEX_SET_CYCTIMES(hIndex->u32EntryVersion, pstIdxEntry, hIndex->stCycMgr.u32CycTimes);

            //index超前是会对MaxCycSize进行修正，可能会超过u64Offset
            if(u64Offset >= pstCycMgr->u64MaxCycSize)
            {
                u64Offset -= pstCycMgr->u64MaxCycSize;
                PVRINDEX_SET_OFFSET(hIndex->u32EntryVersion, pstIdxEntry, u64Offset);
            }
        }
        
        hIndex->u64TimeRewindMaxSize += pstCycMgr->u64MaxCycSize;
        hIndex->bTimeRewindFlagForFile  = HI_TRUE;
    }
    else
    {
        u64Offset -= hIndex->stCycMgr.u64MaxCycSize;
        PVRINDEX_SET_OFFSET(hIndex->u32EntryVersion, pstIdxEntry, u64Offset);
        PVRIndexUpdateCycMgrAndRecInfoWhenRewind(hIndex, pstCycMgr, &hIndex->stRecIdxInfo, pstIdxEntry);
        hIndex->bRewindFlagForIndex = HI_TRUE;
        PVRINDEX_SET_CYCTIMES(hIndex->u32EntryVersion, pstIdxEntry, pstCycMgr->u32CycTimes);

        hIndex->stIdxWriteCache.u32RewindCacheWrite = hIndex->stIdxWriteCache.u32Write;
    }
}


static HI_VOID PVRIndexProcNotRewind(PVR_INDEX_HANDLE hIndex,
                                       PVR_CYC_MGR_S *pstCycMgr,
                                       PVR_REC_INDEX_INFO_S *pstRecInfo,
                                       PVR_INDEX_ENTRY_NEW_S *pstIdxEntry)
{
    HI_U64 u64GlobalOffset = 0;
    HI_U64 u64Offset = 0;

    u64GlobalOffset = PVRINDEX_GET_GLOBALOFFSET(hIndex->u32EntryVersion, pstIdxEntry);
    
//处理时间回绕下index落后的情况
    if((pstCycMgr->enRewindType == PVR_INDEX_REWIND_BY_TIME) &&
       (hIndex->bTimeRewinding == HI_TRUE))
    {
        if (u64GlobalOffset <= hIndex->u64TimeRewindMaxSize)
        {
            PVRIndexUpdateCycMgrAndRecInfoWhenNotRewind(hIndex, pstCycMgr, pstRecInfo, pstIdxEntry);
        }
        else
        {
            HI_INFO_PVR("Proc rewind when timerewind and ts lead idx!!!! idx globaloffset %llx timerewindmaxsize %llx\n",
                         u64GlobalOffset, hIndex->u64TimeRewindMaxSize);
            PVRIndexUpdateCycMgrAndRecInfoWhenRewind(hIndex, pstCycMgr, pstRecInfo, pstIdxEntry);
            hIndex->bRewindFlagForIndex = HI_TRUE;
            hIndex->bTimeRewinding   = HI_FALSE;
            PVRINDEX_SET_CYCTIMES(hIndex->u32EntryVersion, pstIdxEntry, hIndex->stCycMgr.u32CycTimes);
            u64Offset   = u64GlobalOffset - hIndex->u64TimeRewindMaxSize;
            PVRINDEX_SET_GLOBALOFFSET(hIndex->u32EntryVersion, pstIdxEntry, u64Offset);
            hIndex->stIdxWriteCache.u32RewindCacheWrite = hIndex->stIdxWriteCache.u32Write;
        }
    }
    //其他情况，正常更新
    else
    {
        PVRIndexUpdateCycMgrAndRecInfoWhenNotRewind(hIndex, pstCycMgr, pstRecInfo, pstIdxEntry);
    }
}


static HI_VOID PVRIndexProcDisableRewind(PVR_INDEX_HANDLE hIndex,
                                       PVR_CYC_MGR_S *pstCycMgr,
                                       PVR_REC_INDEX_INFO_S *pstRecInfo,
                                       PVR_INDEX_ENTRY_NEW_S *pstIdxEntry)
{
    HI_U32 u32DisplayTimeMs = 0;
    HI_U32 u32FrameSize = 0;
    HI_U64 u64GlobalOffset = 0;

    
    
    if ((pstCycMgr->u32MaxCycTimeInMs == 0) && (pstCycMgr->u64MaxCycSize == 0))
    {
        PVRIndexUpdateCycMgrAndRecInfoWhenDisableRewind(hIndex, pstCycMgr, pstRecInfo, pstIdxEntry);
        return ;
    }
    
    if (PVR_INDEX_REWIND_BY_TIME == pstCycMgr->enRewindType)
    {    
        u32DisplayTimeMs = PVRINDEX_GET_DISPTIME(hIndex->u32EntryVersion, pstIdxEntry);
        if(u32DisplayTimeMs > pstCycMgr->u32MaxCycTimeInMs)
        {
            u32FrameSize = PVRINDEX_GET_FRAMESIZE(hIndex->u32EntryVersion, &(hIndex->stCurRecFrame));
            u64GlobalOffset = PVRINDEX_GET_GLOBALOFFSET(hIndex->u32EntryVersion, &(hIndex->stCurRecFrame));
            pstCycMgr->u64MaxCycSize = PVR_IDX_ALIGN_DATA(u64GlobalOffset + u32FrameSize);
            hIndex->bRecReachFix = HI_TRUE;
            return ;
        }
    }
    else
    {
        u32FrameSize = PVRINDEX_GET_FRAMESIZE(hIndex->u32EntryVersion, pstIdxEntry);
        u64GlobalOffset = PVRINDEX_GET_GLOBALOFFSET(hIndex->u32EntryVersion, pstIdxEntry);
        /*we must guarantee that the frame is smaller than fix size*/
        /*CNComments:必须保证录制文件大小不大于指定大小*/
        if (u64GlobalOffset + u32FrameSize > pstCycMgr->u64MaxCycSize)
        {
            hIndex->bRecReachFix = HI_TRUE;
            return ;
        }
    }

    PVRIndexUpdateCycMgrAndRecInfoWhenDisableRewind(hIndex, pstCycMgr, pstRecInfo, pstIdxEntry);

    return ;
}

/*****************************************************************************
 Prototype       : PVR_IndexGetNextEntry
 Description     : to find next frame,
 first, check whether the read pointer is beyond the mark(by the result of read interface, if failure, the device maybe be unplugged)
 sedond, read frame
 last, move the read pointer to the next frame. again, check the valid, because, the end maybe not the last frame of index file
 so, this interface can be used for checking whether it reach to the end of the index file.
 Input           : handle  **
                   pEntry  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/17
    Author       : q46153
    Modification : Created function

*****************************************************************************/
STATIC HI_S32 PVRIndexGetNextEntry(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry)
{
    ssize_t readNum;
    HI_U32 u32OldFrame;
    HI_U64 u64Offset __attribute__((unused))	= 0;
    HI_U32 u32PtsMs __attribute__((unused))	= 0;
    HI_U32 u32DisplayTimeMs __attribute__((unused))	= 0;

    HI_ASSERT_RET(NULL != handle);
    HI_ASSERT_RET(NULL != pEntry);

    /*
    HI_INFO_PVR("S:%d, E:%d, L:%d, R:%d\n", handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame,
               handle->stCycMgr.u32LastFrame, handle->u32ReadFrame);
    */
    //readNum = PVR_READALL(pEntry, sizeof(PVR_INDEX_ENTRY_S), handle->s32ReadFd, (handle->u32ReadFrame * sizeof(PVR_INDEX_ENTRY_S)));
    PVR_READ_INDEX(readNum,pEntry, 1, handle->s32ReadFd,
                             ((HI_U64)handle->u32ReadFrame * handle->u32EntryLength), handle);

    if (readNum != (ssize_t)handle->u32EntryLength)
    {
        if (-1 == readNum)
        {
            HI_WARN_PVR("read index error: ");
            return HI_ERR_PVR_FILE_CANT_READ;
        }
        else
        {
            HI_INFO_PVR("read to end, cur and next is same: S:%d, E:%d, L:%d, C:%d\n", handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame,
                   handle->stCycMgr.u32LastFrame, handle->u32ReadFrame);
            return HI_ERR_PVR_FILE_TILL_END;
        }
    }

    u32OldFrame = handle->u32ReadFrame;
    PVRIndexCycMoveReadFrame(handle, 1);
    if (u32OldFrame == handle->u32ReadFrame)
    {
        HI_WARN_PVR("read to end, S:%d, E:%d, L:%d, C:%d, O:%d\n", handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame,
                   handle->stCycMgr.u32LastFrame, handle->u32ReadFrame, u32OldFrame);
        return HI_ERR_PVR_FILE_TILL_END;
    }
    u64Offset = PVRINDEX_GET_OFFSET(handle->u32EntryVersion, pEntry);
    u32PtsMs = PVRINDEX_GET_PTS(handle->u32EntryVersion, pEntry);
    u32DisplayTimeMs = PVRINDEX_GET_PTS(handle->u32EntryVersion, pEntry);
    HI_INFO_PVR("after get: Read frame:%u, Type:%u, offset:%llu, PTS:%u, Time:%u \n", handle->u32ReadFrame,
                PVR_INDEX_GET_FRAMETYPE(handle->u32EntryVersion, PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, pEntry)), 
                u64Offset, u32PtsMs, u32DisplayTimeMs);
    return HI_SUCCESS;
}

#if 0

/*****************************************************************************
 Prototype       : PVR_IndexGetPlayNextEntry
 Description     : to find next frame of current play frame,
 Input           : handle  **
                   pEntry  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 
    Author       : 
    Modification : Created function

*****************************************************************************/
STATIC HI_S32 PVRIndexGetPlayNextEntry(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry)
{
    ssize_t readNum = 0;
    HI_U64 u64Offset = 0;
    HI_U32 u32PtsMs = 0;
    HI_U32 u32DisplayTimeMs = 0;

    HI_ASSERT_RET(NULL != handle);
    HI_ASSERT_RET(NULL != pEntry);

    PVR_READ_INDEX(readNum, pEntry, 1, handle->s32ReadFd,
                   (handle->u32ReadFrame * handle->u32EntryLength), handle);

    if (readNum != (ssize_t)handle->u32EntryLength)
    {
        if (-1 == readNum)
        {
            HI_WARN_PVR("read index error: ");
            return HI_ERR_PVR_FILE_CANT_READ;
        }
        else
        {
            HI_INFO_PVR("read to end, cur and next is same: S:%d, E:%d, L:%d, C:%d\n", handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame,
                   handle->stCycMgr.u32LastFrame, handle->u32ReadFrame);
            return HI_ERR_PVR_FILE_TILL_END;
        }
    }
    u64Offset = PVRINDEX_GET_OFFSET(handle->u32EntryVersion, pEntry);
    u32PtsMs = PVRINDEX_GET_PTS(handle->u32EntryVersion, pEntry);
    u32DisplayTimeMs = PVRINDEX_GET_PTS(handle->u32EntryVersion, pEntry);
    HI_INFO_PVR("after get: Read frame:%u, Type:%u, offset:%llu, PTS:%u, Time:%u \n", handle->u32ReadFrame,
                PVR_INDEX_GET_FRAMETYPE(handle->u32EntryVersion, PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, pEntry)), 
                u64Offset, u32PtsMs, u32DisplayTimeMs);
    return HI_SUCCESS;
}
#endif


/*****************************************************************************
 Prototype       : PVRIndexGetNextIEntry
 Description     : to find next I frame
 Input           : handle  **
                   pEntry  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/17
    Author       : q46153
    Modification : Created function

*****************************************************************************/
STATIC INLINE HI_S32 PVRIndexGetNextIEntry(const PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry)
{
    HI_S32 ret;
    HI_U16 u16FrameTypeAndGop = 0;
    //HI_BOOL bIDRType = HI_FALSE;
    
    HI_ASSERT_RET(NULL != handle);
    HI_ASSERT_RET(NULL != pEntry);
/*    
    if (HI_UNF_VCODEC_TYPE_HEVC == (HI_UNF_VCODEC_TYPE_E)(PVR_Index_GetVtype(handle) - 100))
    {
        bIDRType = HI_TRUE;
    }
*/    
    while (1)
    {
        ret = PVRIndexGetNextEntry(handle, pEntry);
        if (ret != HI_SUCCESS)
        {
            return ret;
        }
        u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion,pEntry);
        /* I frame, and not found the frame upflow flag*/
        if (PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_I) 
            || PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR))
        {
            break;
        }
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : PVR_IndexGetPreEntry
 Description     : to find previous frame
 Input           : handle  **
                   pEntry  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/17
    Author       : q46153
    Modification : Created function

*****************************************************************************/
STATIC INLINE HI_S32 PVRIndexGetPreEntry(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry)
{
    ssize_t readNum = 0;
    HI_U32 u32OldFrame = 0;
    HI_U64 u64Offset __attribute__((unused)) = 0;
    HI_U32 u32PtsMs __attribute__((unused))	 = 0;

    HI_ASSERT_RET(NULL != handle);
    HI_ASSERT_RET(NULL != pEntry);

    HI_INFO_PVR("S:%d, E:%d, L:%d, R:%d\n", handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame,
               handle->stCycMgr.u32LastFrame, handle->u32ReadFrame);

    u32OldFrame = handle->u32ReadFrame;
    PVRIndexCycMoveReadFrame(handle, (HI_S32)(-1));

    if (u32OldFrame == handle->u32ReadFrame)
    {
        HI_WARN_PVR("read to start, cur and pre is same: S:%d, E:%d, L:%d, C:%d, O:%d\n", handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame,
                   handle->stCycMgr.u32LastFrame, handle->u32ReadFrame, u32OldFrame);
        return HI_ERR_PVR_FILE_TILL_START;
    }

    PVR_READ_INDEX(readNum, pEntry, 1, handle->s32ReadFd,
                             (handle->u32ReadFrame * handle->u32EntryLength), handle);
    if (readNum != (ssize_t)handle->u32EntryLength)
    {
        if (-1 == readNum)
        {
            HI_WARN_PVR("read index error: ");
            return HI_ERR_PVR_FILE_CANT_READ;
        }
        else
        {
            HI_WARN_PVR("read to start,  S:%d, E:%d, L:%d, C:%d\n", handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame,
                   handle->stCycMgr.u32LastFrame, handle->u32ReadFrame);
            return HI_ERR_PVR_FILE_TILL_START;
        }
    }
    u64Offset = PVRINDEX_GET_OFFSET(handle->u32EntryVersion, pEntry);
    u32PtsMs = PVRINDEX_GET_PTS(handle->u32EntryVersion, pEntry);
    HI_INFO_PVR("after get: R:%u, Type:%u, offset:%llu, PTS:%u \n", handle->u32ReadFrame,
                PVR_INDEX_GET_FRAMETYPE(handle->u32EntryVersion, PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, pEntry)), u64Offset, u32PtsMs);

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : PVR_IndexGetPreIEntry
 Description     : to find the previous I frame
 Input           : handle  **
                   pEntry  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/17
    Author       : q46153
    Modification : Created function

*****************************************************************************/
STATIC INLINE HI_S32 PVRIndexGetPreIEntry(const PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry)
{
    HI_S32 ret;
    HI_U16 u16FrameTypeAndGop = 0;
    //HI_BOOL bIDRType = HI_FALSE;
    HI_ASSERT_RET(NULL != handle);
    HI_ASSERT_RET(NULL != pEntry);
  /*  
    if (HI_UNF_VCODEC_TYPE_HEVC == (HI_UNF_VCODEC_TYPE_E)(PVR_Index_GetVtype(handle) - 100))
    {
        bIDRType = HI_TRUE;
    }
*/
    while (1)
    {
        ret = PVRIndexGetPreEntry(handle, pEntry);
        if (ret != HI_SUCCESS)
        {
            return ret;
        }
        u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, pEntry);
        /* I frame, and not found the frame upflow flag*/
        if (PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_I) 
            || PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR))
        {
            break;
        }
    }


    return HI_SUCCESS;
}

STATIC HI_U32 PVRIndexSeachByTime(PVR_INDEX_HANDLE handle, HI_U32 timeWant,
                                  HI_U32 start, HI_U32 end,  PVR_FILE seekFd)
{
    ssize_t l_readNum;
    HI_U32 target;
    PVR_INDEX_ENTRY_NEW_S entry;
    HI_U32 nextStart = 0;
    HI_U32 nextEnd = 0;
    HI_U32 u32DisplayTimeMs = 0;

    memset(&entry, 0x00, sizeof(entry));   
    target = (start + end)/2;

    if (target == start || target == end)
    {
        HI_WARN_PVR("PVRIndexSeachByTime end, ret:%d\n", target);
        return target;
    }

    /* get target's time */
    PVR_READ_INDEX(l_readNum, &entry, 1, seekFd,
                               ((HI_U64)target*handle->u32EntryLength), handle);
    if (l_readNum != (ssize_t)handle->u32EntryLength)
    {
        HI_ERR_PVR("read err,  want:%u, get:%u, off:%u\n", (handle->u32EntryLength), (l_readNum), target*handle->u32EntryLength);
        if (-1 == l_readNum)
        {
            HI_WARN_PVR("read index error: ");
            return 0;
        }
        else if (0 == l_readNum) /* if meet error at the end of file, return the last frame AI7D03033 */
        {
            HI_U32 u32LastPos;
            u32LastPos = (HI_U32)PVR_SEEK(seekFd, (off_t)(0 - (HI_S32)handle->u32EntryLength), SEEK_END);
            if ((HI_S32)u32LastPos >= 0)
            {
                if (handle->u32EntryLength != PVR_READ(&entry, handle->u32EntryLength, seekFd, u32LastPos))
                {
                    HI_ERR_PVR("read error\n");
                }
                return (u32LastPos / handle->u32EntryLength);
            }
            else
            {
                HI_WARN_PVR("can't get the last frame\n");
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    u32DisplayTimeMs = PVRINDEX_GET_DISPTIME(handle->u32EntryVersion, &entry);
    HI_INFO_PVR("^^^^ search time:want=%d, target=%d ^^^^\n", timeWant, u32DisplayTimeMs);
    if (u32DisplayTimeMs <= timeWant)
    {
        nextStart = target;
        nextEnd = end;
    }
    else
    {
        nextStart = start;
        nextEnd = target;
    }

    return PVRIndexSeachByTime(handle, timeWant, nextStart, nextEnd, seekFd);
}


/*****************************************************************************
 Prototype       : PVRIndexFindFrameByTime
 Description     : find a frame match the time
 Input           : handle           **
                   offsetFromStart  **
 Output          : None
 Return Value    : the frame ID from start of file.
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/6/30
    Author       : fd
    Modification : Created function

*****************************************************************************/
STATIC INLINE HI_U32 PVRIndexFindFrameByTime(PVR_INDEX_HANDLE handle, HI_U32 u32FindTime)
{
    HI_U32 u32FrameId;
    HI_U32 u32StartFrame;
    HI_U32 u32EndFrame;
    PVR_INDEX_ENTRY_NEW_S lastEntry;
    HI_U32 u32Offset;
    HI_S32 s32Read;
    HI_U32 u32LastFrame;
    HI_U32 u32DisplayTimeMs = 0;

    memset(&lastEntry, 0x00, sizeof(lastEntry));
    u32EndFrame   = (handle->stCycMgr.u32EndFrame == -1)?0:handle->stCycMgr.u32EndFrame;
    u32StartFrame = handle->stCycMgr.u32StartFrame;
    u32LastFrame  = (handle->stCycMgr.u32LastFrame == -1)?0:handle->stCycMgr.u32LastFrame;

    memset(&lastEntry, 0, handle->u32EntryLength);   
    /* Not rewind, find it directly*/
    if (u32EndFrame > u32StartFrame)
    {
        u32FrameId = PVRIndexSeachByTime(handle, u32FindTime, u32StartFrame,
                                         u32EndFrame,  handle->s32SeekFd);
    }
    else /* Rewind, find it for two part*/
    {
        u32Offset = (HI_U32)(handle->u32EntryLength * u32LastFrame);
        PVR_READ_INDEX(s32Read, &lastEntry, 1, handle->s32SeekFd, u32Offset, handle);
        if (s32Read != (HI_S32)handle->u32EntryLength)
        {
            HI_ERR_PVR("HI_PVR_GetFileAttrByFileName-read idx failed\n");
            return (handle->stCycMgr.u32StartFrame);
        }
        u32DisplayTimeMs = PVRINDEX_GET_DISPTIME(handle->u32EntryVersion, &lastEntry);
        HI_INFO_PVR("last entry PTS=%d ms\n", u32DisplayTimeMs);

        if (u32FindTime <= u32DisplayTimeMs)
        {
            HI_WARN_PVR("u32FindTime:%u, u32DisplayTimeMs:%u, find it in the last section\n", u32FindTime,u32DisplayTimeMs);
            u32FrameId = PVRIndexSeachByTime(handle, u32FindTime, u32StartFrame,
                                             u32LastFrame,  handle->s32SeekFd);
        }
        else /* find it in the start part */
        {
            HI_WARN_PVR("u32FindTime:%u, u32DisplayTimeMs:%u, find it in the first section\n", u32FindTime,u32DisplayTimeMs);
            if (0 == handle->stCycMgr.u32EndFrame)
            {
                return handle->stCycMgr.u32EndFrame;
            }
            else
            {
                u32FrameId = PVRIndexSeachByTime(handle, u32FindTime, 0,
                                                 u32EndFrame,  handle->s32SeekFd);
            }
            
        }
    }

    return u32FrameId;
}
#if 0  /*obsolete APIs*/
/*****************************************************************************
 Prototype       : PVR_IndexGetCurrentEntry
 Description     : to get current frame information
                    if the read pointer reach to the end, it will return failure, because of it reach to the end of the file .
 Input           : handle  **
                   pEntry  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/17
    Author       : q46153
    Modification : Created function

*****************************************************************************/
STATIC INLINE HI_S32 PVRIndexGetCurrentEntry(const PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry)
{
    ssize_t readNum = 0;
    HI_U64 u64Offset __attribute__((unused))	= 0;
    HI_U32 u32PtsMs __attribute__((unused))	= 0;
    

    HI_ASSERT_RET(handle != NULL);
    HI_ASSERT_RET(pEntry != NULL);

    HI_INFO_PVR("S:%d, E:%d, L:%d, C:%d\n", handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame,
               handle->stCycMgr.u32LastFrame, handle->u32ReadFrame);

    if (handle->u32ReadFrame == handle->stCycMgr.u32EndFrame)
    {
        HI_WARN_PVR("read to end, S:%d, E:%d, L:%d, C:%d\n", handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame,
               handle->stCycMgr.u32LastFrame, handle->u32ReadFrame);
        return HI_ERR_PVR_FILE_TILL_END;
    }

    PVR_READ_INDEX(readNum, pEntry, 1, handle->s32ReadFd, ((HI_U64)handle->u32ReadFrame * handle->u32EntryLength), handle);
    if (readNum != (ssize_t)handle->u32EntryLength)
    {
        /* PVR play to the end of file, no way for PVR_EVENT_PLAY_EOF, AI7D02611 */
        if (-1 == readNum)
        {
            HI_WARN_PVR("read failed in PVRIndexGetCurrentEntry");
            return HI_ERR_PVR_FILE_CANT_READ;
        }
        else
        {
            HI_WARN_PVR("read to end, S:%d, E:%d, L:%d, C:%d\n", handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame,
                   handle->stCycMgr.u32LastFrame, handle->u32ReadFrame);
            return HI_ERR_PVR_FILE_TILL_END;
        }
    }
    u64Offset = PVRINDEX_GET_OFFSET(handle->u32EntryVersion, pEntry);
    u32PtsMs = PVRINDEX_GET_PTS(handle->u32EntryVersion, pEntry);
    HI_INFO_PVR("frame cur <Read frame:%u, Type:%u, offset:%llu, PTS:%u> \n", handle->u32ReadFrame,
                PVR_INDEX_GET_FRAMETYPE(handle->u32EntryVersion, PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, pEntry)), u64Offset, u32PtsMs);

    return HI_SUCCESS;
}
#endif
#ifdef HI_PVR_EXTRA_BUF_SUPPORT
//跟writeindex配套，不考虑回绕的情况
static HI_BOOL PVRIndexWriteWhetherCoverRead(HI_U32 u32Write, HI_U32 u32Num, HIPVR_IDX_BUF_S *pstReadCache)
{
    if (u32Write >= pstReadCache->u32StartNum && u32Write < pstReadCache->u32StartNum + pstReadCache->u32UsedNum)
    {
        return HI_TRUE;
    }

    if (u32Write < pstReadCache->u32StartNum && u32Write + u32Num >= pstReadCache->u32StartNum)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

static HI_S32 PVRIndexWriteIndexEntry(PVR_INDEX_HANDLE hIndex,
                                      HI_U32 u32WriteNum,
                                      HI_U32 u32CacheOffset,
                                      HI_U32 u32FileOffset)
{
    HI_U32 u32SaveSize;

    //如果写入的index  会覆盖readcache  ，则刷新readcache
    PVR_IDX_CACHE_LOCK(&hIndex->stIdxReadCache.stCacheMutex);
    if (PVRIndexWriteWhetherCoverRead(u32FileOffset,u32WriteNum,&hIndex->stIdxReadCache))
    {
        HI_INFO_PVR("Write frame will cover read frame. read=%d write=%d\n",hIndex->stIdxReadCache.u32Read, u32WriteNum);
        hIndex->stIdxReadCache.bDirty = HI_TRUE;
    }
    PVR_IDX_CACHE_UNLOCK(&hIndex->stIdxReadCache.stCacheMutex);

    PVR_IDX_CACHE_LOCK(&(hIndex->stIdxWriteCache.stCacheMutex));
    if (u32CacheOffset + u32WriteNum >= PVR_INDEX_WRITE_CACHE_NUM)
    {
        PVR_WRITE_INDEX(u32SaveSize, 
                        (PVR_INDEX_WRITE_CACHE_NUM - u32CacheOffset) * hIndex->u32EntryLength,
                        hIndex->stIdxWriteCache.pu8Addr + u32CacheOffset * hIndex->u32EntryLength,
                        hIndex->s32WriteFd, 
                        u32FileOffset * hIndex->u32EntryLength,
                        hIndex);
        
        PVR_WRITE_INDEX(u32SaveSize, 
                        (u32CacheOffset + u32WriteNum - PVR_INDEX_WRITE_CACHE_NUM) * hIndex->u32EntryLength,
                        hIndex->stIdxWriteCache.pu8Addr,
                        hIndex->s32WriteFd, 
                        (u32FileOffset + PVR_INDEX_WRITE_CACHE_NUM - u32CacheOffset) * hIndex->u32EntryLength,
                        hIndex);
    }
    else
    {
        PVR_WRITE_INDEX(u32SaveSize, 
                        u32WriteNum * hIndex->u32EntryLength,
                        hIndex->stIdxWriteCache.pu8Addr + u32CacheOffset * hIndex->u32EntryLength,
                        hIndex->s32WriteFd, 
                        u32FileOffset * hIndex->u32EntryLength,
                        hIndex);
    }
    PVR_IDX_CACHE_UNLOCK(&(hIndex->stIdxWriteCache.stCacheMutex));

    HI_INFO_PVR("Write %d idx Cacheoffset %10d  Fileoffset %10d\n", u32WriteNum, u32CacheOffset,u32FileOffset);

    return HI_SUCCESS;
}

static HI_VOID PVRIndexCopyToCache(PVR_INDEX_HANDLE hIndex, HIPVR_IDX_BUF_S *pstIdxBuf, PVR_INDEX_ENTRY_NEW_S *pstIdxEntry)
{
    HI_U32 u32EntryLength = hIndex->u32EntryLength;
    memcpy(pstIdxBuf->pu8Addr + pstIdxBuf->u32Write*u32EntryLength, pstIdxEntry, u32EntryLength);
    pstIdxBuf->u32Write = PVR_IDX_NEXT_POS_IN_CYC(pstIdxBuf->u32Write, PVR_INDEX_WRITE_CACHE_NUM);
    pstIdxBuf->u32UsedNum++;
}

HI_S32 PVR_Index_SaveToFile(PVR_INDEX_HANDLE hIndex, HI_BOOL bDirectWrite)
{
    HI_S32 s32Ret;
    HI_U32 u32Read, u32Write, u32RewindInCache, u32IdxCount = 0;
    HI_BOOL bRewindInCache = HI_FALSE;
    PVR_CYC_MGR_S        stCycMgr = {0};
    PVR_REC_INDEX_INFO_S stIdxInfo = {0};
    HIPVR_IDX_BUF_S *pstIdxBuf = HI_NULL;

    pstIdxBuf = &hIndex->stIdxWriteCache;

/** we use u32UsedNum to judge whether the writecache is full or empty */
/**  CNcomment:统一用 usednum 判断 writecache 是否空或者满*/
    if (pstIdxBuf->u32UsedNum == 0)
    {
        HI_INFO_PVR("There is no data in write cache!\n");
        return HI_SUCCESS;
    }

//用单独的变量存储，保证下面处理过程中不会因为writecache  的
//写指针更新而出问题
    PVR_IDX_CACHE_LOCK(&hIndex->stIdxWriteCache.stCacheMutex);
    u32Read  = pstIdxBuf->u32Read;
    u32Write = pstIdxBuf->u32Write;
    PVR_IDX_CACHE_UNLOCK(&hIndex->stIdxWriteCache.stCacheMutex);

    PVR_INDEX_LOCK(&(hIndex->stMutex));
    bRewindInCache = hIndex->bRewindFlagForIndex;
    u32RewindInCache = pstIdxBuf->u32RewindCacheWrite;
    memcpy(&stCycMgr, &hIndex->stCycMgr, sizeof(PVR_CYC_MGR_S));
    memcpy(&stIdxInfo, &hIndex->stRecIdxInfo, sizeof(PVR_REC_INDEX_INFO_S));
    PVR_INDEX_UNLOCK(&(hIndex->stMutex));

    u32IdxCount = pstIdxBuf->u32UsedNum;

    //要写入的cache  中有回绕点，则分段写入index  文件
    if(bRewindInCache)
    {
        HI_U32 u32IdxCount1,u32IdxCount2;
        u32IdxCount1 = PVR_IDX_DISTANCE_IN_CYC(u32RewindInCache, u32Read, PVR_INDEX_WRITE_CACHE_NUM);
        u32IdxCount2 = PVR_IDX_DISTANCE_IN_CYC(u32Write, u32RewindInCache, PVR_INDEX_WRITE_CACHE_NUM);
        s32Ret = PVRIndexWriteIndexEntry(hIndex, u32IdxCount1, u32Read, hIndex->u32WriteFrame);
        s32Ret |= PVRIndexWriteIndexEntry(hIndex, u32IdxCount2, u32RewindInCache, 0);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_PVR("Write idx failed!! ret %x\n", s32Ret);
            return s32Ret;
        }

        (HI_VOID)PVRIndexWriteIndexCycMgr(hIndex, &stCycMgr);
        (HI_VOID)PVRIndexWriteIndexInfo(hIndex, &stIdxInfo);

        hIndex->u32WriteFrame = u32IdxCount2;
        //fileidxinfo主要给readcache使用，表明写入文件的帧号
        PVR_IDX_CACHE_LOCK(&(pstIdxBuf->stCacheMutex));
        pstIdxBuf->u32RewindCacheWrite = 0;
        pstIdxBuf->u32Read = PVR_IDX_NEXT_N_POS_IN_CYC(u32Read, u32IdxCount, PVR_INDEX_WRITE_CACHE_NUM);
        pstIdxBuf->u32UsedNum -= u32IdxCount;

        HI_ASSERT(pstIdxBuf->u32UsedNum <= PVR_INDEX_WRITE_CACHE_NUM);

        PVR_IDX_CACHE_UNLOCK(&(pstIdxBuf->stCacheMutex));

        PVR_INDEX_LOCK(&(hIndex->stMutex));
        hIndex->stFileCycInfo.u32EndFrame   = stCycMgr.u32EndFrame;
        hIndex->stFileCycInfo.u32LastFrame  = stCycMgr.u32LastFrame;
        hIndex->stFileCycInfo.u32StartFrame = stCycMgr.u32StartFrame;
        hIndex->stFileCycInfo.u32IsRewind   = stCycMgr.bIsRewind;
        hIndex->bRewindFlagForIndex = HI_FALSE;
        PVR_INDEX_UNLOCK(&(hIndex->stMutex));

    }
    //达到写文件门限或者直接写，则进行写文件操作
    else if (u32IdxCount > PVR_INDEX_WRITE_FILE_LIMIT || bDirectWrite)
    {
        s32Ret = PVRIndexWriteIndexEntry(hIndex,u32IdxCount,u32Read,hIndex->u32WriteFrame);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_PVR("Write index error %x\n", s32Ret);
            return s32Ret;
        }

        hIndex->u32WriteFrame += u32IdxCount;
        (HI_VOID)PVRIndexWriteIndexCycMgr(hIndex, &stCycMgr);
        (HI_VOID)PVRIndexWriteIndexInfo(hIndex, &stIdxInfo);

        PVR_IDX_CACHE_LOCK(&(pstIdxBuf->stCacheMutex));

        pstIdxBuf->u32Read = PVR_IDX_NEXT_N_POS_IN_CYC(u32Read, u32IdxCount, PVR_INDEX_WRITE_CACHE_NUM);
        pstIdxBuf->u32UsedNum -= u32IdxCount;

        HI_ASSERT(pstIdxBuf->u32UsedNum <= PVR_INDEX_WRITE_CACHE_NUM);
        
        PVR_IDX_CACHE_UNLOCK(&(pstIdxBuf->stCacheMutex));

        PVR_INDEX_LOCK(&(hIndex->stMutex));
        hIndex->stFileCycInfo.u32EndFrame   = stCycMgr.u32EndFrame;
        hIndex->stFileCycInfo.u32LastFrame  = stCycMgr.u32LastFrame;
        hIndex->stFileCycInfo.u32StartFrame = stCycMgr.u32StartFrame;
        hIndex->stFileCycInfo.u32IsRewind   = stCycMgr.bIsRewind;
        PVR_INDEX_UNLOCK(&(hIndex->stMutex));
    }

    return HI_SUCCESS;
}

HI_S32 PVR_Index_SaveToCache(PVR_INDEX_HANDLE hIndex, const HI_MPI_DMX_REC_INDEX_S *pstDmxIndexInfo)
{
    PVR_INDEX_ENTRY_NEW_S   stIndexEntry;
    PVR_CYC_MGR_S       *pstCycMgr = HI_NULL;
    PVR_REC_INDEX_INFO_S *pstRecInfo = HI_NULL;
    HIPVR_IDX_BUF_S     *pstIdxBuf = HI_NULL;

    memset(&stIndexEntry, 0x00, sizeof(stIndexEntry));
    pstCycMgr = &(hIndex->stCycMgr);
    pstRecInfo = &(hIndex->stRecIdxInfo);
    pstIdxBuf = &(hIndex->stIdxWriteCache);

    /*if index has reach fix point, there is nothing to do, just return.*/
    if (hIndex->bRecReachFix)
        return HI_SUCCESS;

    if ((HI_UNF_FRAME_TYPE_I != pstDmxIndexInfo->enFrameType) &&
        (HI_UNF_FRAME_TYPE_P != pstDmxIndexInfo->enFrameType) &&
        (HI_UNF_FRAME_TYPE_B != pstDmxIndexInfo->enFrameType) &&
        (HI_UNF_FRAME_TYPE_IDR != pstDmxIndexInfo->enFrameType))
    {
        HI_WARN_PVR("Frame type %d is invalid\n", pstDmxIndexInfo->enFrameType);
        return HI_SUCCESS;
    }

    if (pstIdxBuf->u32UsedNum == PVR_INDEX_WRITE_CACHE_NUM)
    {
        return HI_FAILURE;
    }

    PVR_INDEX_LOCK(&(hIndex->stMutex));
    PVRIndexAssignIndexEntry(hIndex, pstDmxIndexInfo, &stIndexEntry);

    if (hIndex->stCycMgr.enRewindType == PVR_INDEX_REWIND_BY_BOTH)
        PVR_IDX_CHECK_REWINDTYPE_BYBOTH(hIndex, (&stIndexEntry), pstCycMgr);

    PVRIndexGetRealOffset(hIndex, pstCycMgr, &stIndexEntry);


    if (PVR_IDX_ENABLE_REWIND(hIndex))      //支持回绕
    {
        if (PVRIndexCheckRewind(hIndex, pstCycMgr, &stIndexEntry))      //如果发生回绕
        {
            PVRIndexProcRewind(hIndex, pstCycMgr, pstRecInfo, &stIndexEntry);
        }
        else
        {
            PVRIndexProcNotRewind(hIndex, pstCycMgr, pstRecInfo, &stIndexEntry);
        }
    }
    else
    {
        PVRIndexProcDisableRewind(hIndex, pstCycMgr, pstRecInfo, &stIndexEntry);
        if (hIndex->bRecReachFix)
        {
            PVR_INDEX_UNLOCK(&(hIndex->stMutex));
            return HI_SUCCESS;
        }
    }
    PVRIndexAddToRefList(&hIndex->stRefList, pstDmxIndexInfo->s16CurPoc, hIndex->u32FrameIndex);
    hIndex->u32FrameIndex++;
    PVR_IDX_CACHE_LOCK(&(pstIdxBuf->stCacheMutex));
    PVRIndexCopyToCache(hIndex, pstIdxBuf, &stIndexEntry);
    PVR_IDX_CACHE_UNLOCK(&(pstIdxBuf->stCacheMutex));

    memcpy(&(hIndex->stCurRecFrame), &stIndexEntry, sizeof(stIndexEntry));

    PVR_INDEX_UNLOCK(&(hIndex->stMutex));
    
    HI_INFO_PVR("Chn %d saveidx! bufwrite/goffset/off/frmsize/disime/frmtype %d/%lld/%lld/%d/%d/%d\n",
                hIndex->u32RecPicParser, pstIdxBuf->u32Write, 
                PVRINDEX_GET_GLOBALOFFSET(hIndex->u32EntryVersion, &stIndexEntry), 
                PVRINDEX_GET_OFFSET(hIndex->u32EntryVersion, &stIndexEntry),
                PVRINDEX_GET_DISPTIME(hIndex->u32EntryVersion, &stIndexEntry),
                PVRINDEX_GET_FRAMESIZE(hIndex->u32EntryVersion, &stIndexEntry), 
                ((pstDmxIndexInfo->enFrameType)&0x3));

    return HI_SUCCESS;
}

/*
Flush the index in the write cache to the disk
*/
HI_S32 PVR_Index_FlushToFile(PVR_INDEX_HANDLE hIndex)
{
    HI_U32 u32IdxTotalNum = 0;
    HI_S32 s32Ret;
    PVR_CYC_MGR_S   *pstCycMgr = HI_NULL;
    HIPVR_IDX_BUF_S *pstIdxBuf = HI_NULL;

    pstCycMgr = &(hIndex->stCycMgr);
    pstIdxBuf = &(hIndex->stIdxWriteCache);
    u32IdxTotalNum = pstIdxBuf->u32UsedNum;

    if (hIndex->bRewindFlagForIndex)
    {
        HI_U32 u32BeforRewindIdxNum;
        u32BeforRewindIdxNum = PVR_IDX_DISTANCE_IN_CYC(pstIdxBuf->u32RewindCacheWrite, 
                                        pstIdxBuf->u32Read, PVR_INDEX_WRITE_CACHE_NUM);
        if (u32IdxTotalNum < u32BeforRewindIdxNum)
        {
            HI_ERR_PVR("Flush idx when rewind errer!!! beforerewind idxnum %d, totalnum %d\n",
                        u32BeforRewindIdxNum, u32IdxTotalNum);
            return HI_FAILURE;
        }

        s32Ret = PVRIndexWriteIndexEntry(hIndex, u32BeforRewindIdxNum, pstIdxBuf->u32Read, hIndex->u32WriteFrame);
        s32Ret |= PVRIndexWriteIndexEntry(hIndex, u32IdxTotalNum - u32BeforRewindIdxNum, pstIdxBuf->u32RewindCacheWrite, 0);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_PVR("Write idx failed!! ret %x\n", s32Ret);
            return s32Ret;
        }

        PVRIndexWriteIndexCycMgr(hIndex, pstCycMgr);
        PVRIndexWriteIndexInfo(hIndex, &hIndex->stRecIdxInfo);                

        hIndex->u32WriteFrame = u32IdxTotalNum - u32BeforRewindIdxNum;
        hIndex->bRewindFlagForIndex = HI_FALSE;
        pstIdxBuf->u32RewindCacheWrite = 0;
    }
    else
    {
        s32Ret = PVRIndexWriteIndexEntry(hIndex, u32IdxTotalNum, pstIdxBuf->u32Read, hIndex->u32WriteFrame);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_PVR("Write index error %x\n", s32Ret);
            return s32Ret;
        }

        (HI_VOID)PVRIndexWriteIndexCycMgr(hIndex, pstCycMgr);
        (HI_VOID)PVRIndexWriteIndexInfo(hIndex, &hIndex->stRecIdxInfo);
        hIndex->u32WriteFrame += u32IdxTotalNum;
    }

    if (hIndex->s32WriteFd && (hIndex->s32WriteFd != PVR_FILE_INVALID_FILE))
    {
        PVR_FSYNC(hIndex->s32WriteFd);
    }

    if (hIndex->s32HeaderFd && (hIndex->s32HeaderFd != PVR_FILE_INVALID_FILE))
    {
        PVR_FSYNC(hIndex->s32HeaderFd);
    }

    hIndex->stFileCycInfo.u32EndFrame   = pstCycMgr->u32EndFrame;
    hIndex->stFileCycInfo.u32LastFrame  = pstCycMgr->u32LastFrame;
    hIndex->stFileCycInfo.u32StartFrame = pstCycMgr->u32StartFrame;
    hIndex->stFileCycInfo.u32IsRewind   = pstCycMgr->bIsRewind;
    pstIdxBuf->u32Read = PVR_IDX_NEXT_N_POS_IN_CYC(pstIdxBuf->u32Read, u32IdxTotalNum, PVR_INDEX_WRITE_CACHE_NUM);
    pstIdxBuf->u32UsedNum = 0;

    return HI_SUCCESS;
}

#else
HI_S32 PVR_Index_SaveFramePosition(PVR_INDEX_HANDLE hIndex, HI_MPI_DMX_REC_INDEX_S *pstDmxIndexInfo)
{
    HI_S32              Ret;
    HI_BOOL             bWriteFile = HI_FALSE;
    HI_BOOL             bRewind = HI_FALSE;
    PVR_INDEX_ENTRY_NEW_S   stIdxEntry;
    PVR_CYC_MGR_S       *pstCycMgr = HI_NULL;
    PVR_REC_INDEX_INFO_S *pstRecInfo = HI_NULL;

    pstCycMgr = &(hIndex->stCycMgr);
    pstRecInfo = &(hIndex->stRecIdxInfo);

    memset(&stIdxEntry, 0x00, sizeof(stIdxEntry));
    if ((HI_UNF_FRAME_TYPE_IDR != pstDmxIndexInfo->enFrameType) &&
        (HI_UNF_FRAME_TYPE_I != pstDmxIndexInfo->enFrameType) &&
        (HI_UNF_FRAME_TYPE_B != pstDmxIndexInfo->enFrameType) &&
        (HI_UNF_FRAME_TYPE_P != pstDmxIndexInfo->enFrameType))
    {
        HI_WARN_PVR("Don't support frame type %d\n", pstDmxIndexInfo->enFrameType);
        return HI_SUCCESS;
    }

    PVR_INDEX_LOCK(&(hIndex->stMutex));
    
    PVRIndexAssignIndexEntry(hIndex, pstDmxIndexInfo, &stIdxEntry);
    
    if (PVR_INDEX_REWIND_BY_BOTH == pstCycMgr->enRewindType)
        PVR_IDX_CHECK_REWINDTYPE_BYBOTH(hIndex, (&stIdxEntry), pstCycMgr);

    PVRIndexGetRealOffset(hIndex, pstCycMgr, &stIdxEntry);

    if (PVR_IDX_ENABLE_REWIND(hIndex))
    {
        bRewind = PVRIndexCheckRewind(hIndex, pstCycMgr, &stIdxEntry);
        if (bRewind)
        {
            PVRIndexProcRewind(hIndex, pstCycMgr, pstRecInfo, &stIdxEntry);
        }
        else
        {
            PVRIndexProcNotRewind(hIndex, pstCycMgr, pstRecInfo, &stIdxEntry);
        }
    }
    else
    {
        PVRIndexProcDisableRewind(hIndex, pstCycMgr, pstRecInfo, &stIdxEntry);
        if (hIndex->bRecReachFix)
        {
            PVR_INDEX_UNLOCK(&(hIndex->stMutex));
            return HI_SUCCESS;
        }
    }

    if (hIndex->bRewindFlagForIndex)
    {
        hIndex->u32WriteFrame = 0;
        hIndex->bRewindFlagForIndex = HI_FALSE;
    }

    PVRIndexAddToRefList(&hIndex->stRefList, pstDmxIndexInfo->s16CurPoc, hIndex->u32WriteFrame);
    memcpy(&hIndex->stCurRecFrame, &stIdxEntry, sizeof(stIdxEntry));

    Ret = PVRCacheWriteIdx(hIndex, &stIdxEntry, 1,
                           hIndex->u32WriteFrame*hIndex->u32EntryLength, &bWriteFile);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_PVR("write idx cache failed  ret %x\n", Ret);
        PVR_INDEX_UNLOCK(&(hIndex->stMutex));
        return Ret;
    }
    hIndex->u32WriteFrame++;
    if (bWriteFile)
    {
        (HI_VOID)PVRIndexWriteIndexCycMgr(hIndex, pstCycMgr);
        (HI_VOID)PVRIndexWriteIndexInfo(hIndex, pstRecInfo);
    }
    
    HI_INFO_PVR("chn %d S:%u, E:%u, L:%u, W:%u\n",
                hIndex->u32RecPicParser,
                pstCycMgr->u32StartFrame,
                pstCycMgr->u32EndFrame,
                pstCycMgr->u32LastFrame,
                hIndex->u32WriteFrame);

    PVR_INDEX_UNLOCK(&(hIndex->stMutex));

    return HI_SUCCESS;
}
#endif
/*check if the offset in cache:1 in cache,0 not in cache*/
HI_S32 PVR_Index_IfOffsetInWriteCache(PVR_INDEX_HANDLE  handle,HI_U32 u32Offset,HI_U32 u32Size)
{
    if (handle->stIdxWriteCache.u32BufferLen == 0)
    {
        return 0;
    }
    PVR_IDX_CACHE_LOCK(&(handle->stIdxWriteCache.stCacheMutex));
    if (handle->stIdxWriteCache.u32BufferLen != 0 && handle->stIdxWriteCache.u32UsedSize)
    {
        if ((u32Offset >= handle->stIdxWriteCache.u32StartOffset && 
            u32Offset <= (handle->stIdxWriteCache.u32StartOffset + handle->stIdxWriteCache.u32UsedSize)) ||
            (u32Offset < handle->stIdxWriteCache.u32StartOffset && 
            ((u32Offset+u32Size) > handle->stIdxWriteCache.u32StartOffset)))
        {
            PVR_IDX_CACHE_UNLOCK(&(handle->stIdxWriteCache.stCacheMutex));
            return 1;
        }
    }
    PVR_IDX_CACHE_UNLOCK(&(handle->stIdxWriteCache.stCacheMutex));
    return 0;
}

HI_S32 PVR_Index_FlushIdxWriteCache(PVR_INDEX_HANDLE    handle)
{
    HI_U32 u32SaveSz;
    if (handle->stIdxWriteCache.u32BufferLen == 0)
    {
        return HI_SUCCESS;
    }
    PVR_IDX_CACHE_LOCK(&(handle->stIdxWriteCache.stCacheMutex));
    if (handle->stIdxWriteCache.u32UsedSize)
    {
        //lint -e774
        PVR_WRITE_INDEX(u32SaveSz, handle->stIdxWriteCache.u32UsedSize,handle->stIdxWriteCache.pu8Addr, 
                            handle->s32WriteFd,handle->stIdxWriteCache.u32StartOffset, handle);
        handle->stIdxWriteCache.u32UsedSize = 0;
        handle->stIdxWriteCache.u32StartOffset = 0;
        memset(handle->stIdxWriteCache.pu8Addr, 0x5a, handle->stIdxWriteCache.u32BufferLen);
        //lint +e774
    }
    PVR_IDX_CACHE_UNLOCK(&(handle->stIdxWriteCache.stCacheMutex));
    return HI_SUCCESS;
}

HI_S32 PVR_Index_Init(HI_VOID)
{
    HI_U32 i;

    if (0 == g_u32PvrIndexInit)
    {
        for (i = 0; i < PVR_INDEX_NUM; i++)
        {
            memset(&g_stPVRIndex[i], 0, sizeof(PVR_INDEX_S));
        }
        g_u32PvrIndexInit++;
    }

    return HI_SUCCESS;
}

HI_BOOL PVR_Index_CheckPlayIndexValid(PVR_INDEX_HANDLE handle)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32StartTimeMs = 0;
    HI_U32 u32EndTimeMs = 0;
    PVR_INDEX_ENTRY_NEW_S stEntry;

    PVR_INDEX_LOCK(&handle->stMutex);
    if ((-1 == handle->stCycMgr.u32EndFrame) || (-1 == handle->stCycMgr.u32LastFrame))
    {
        PVR_INDEX_UNLOCK(&handle->stMutex);
        HI_ERR_PVR("invalid ts files, no valid frame(E/L = (%d, %d)\n", 
            handle->stCycMgr.u32EndFrame, handle->stCycMgr.u32LastFrame);
        return HI_FALSE;
    }

    memset(&stEntry, 0x00, sizeof(stEntry));
    PVRINDEX_GET_ENTRY_BYNUM(handle,&stEntry,handle->stCycMgr.u32EndFrame,ret);
    if (HI_SUCCESS == ret)
    {
        u32EndTimeMs = PVRINDEX_GET_DISPTIME(handle->u32EntryVersion, &stEntry);
    }

    memset(&stEntry, 0x00, sizeof(stEntry));
    PVRINDEX_GET_ENTRY_BYNUM(handle,&stEntry,handle->stCycMgr.u32StartFrame,ret);
    if (HI_SUCCESS == ret)
    {
        u32StartTimeMs = PVRINDEX_GET_DISPTIME(handle->u32EntryVersion, &stEntry);
    }
    PVR_INDEX_UNLOCK(&handle->stMutex);
    if ((u32EndTimeMs <= u32StartTimeMs) || (1500 >= (u32EndTimeMs - u32StartTimeMs)))
    {
        HI_ERR_PVR("Invalid time or less than 1500ms:(u32StartTimeMs, u32EndTimeMs) = (%u, %u)\n", 
            u32StartTimeMs, u32EndTimeMs);
        return HI_FALSE;
    }
    return HI_TRUE;
}

/*****************************************************************************
 Prototype       : PVR_Index_CreatPlay
 Description     : init index module, get index handle
 Input           : pfileName  **
                    enIndexType: need this just only on recording
 Output          : None
 Return Value    : HI_NULL_PTR:failure, maybe operate file fail, included the wrong index file format
                   handle: success, retruen the handle of index
 Global Variable
    Read Only    :
    Read & Write : pIsNoIdx :index file exist or not
  History
  1.Date         : 2008/4/16
    Author       : q46153
    Modification : Created function
  2.Date         : 2010/06/19
    Author       : Jiang Lei
    Modification : modify for HD
*****************************************************************************/
PVR_INDEX_HANDLE PVR_Index_CreatPlay(HI_U32 chnID,
                                     const HI_UNF_PVR_PLAY_ATTR_S *pstPlayAttr,
                                     HI_BOOL *pIsNoIdx)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U64 u64TsFileSize;
    HI_CHAR szIndexName[PVR_MAX_FILENAME_LEN];
    PVR_INDEX_HANDLE handle;
    PVR_INDEX_ENTRY_NEW_S stStartEntry;
    HI_S32 s32CacheLen = 0;

    memset(&stStartEntry, 0x00, sizeof(stStartEntry));
    
    if (!pstPlayAttr)
    {
        return HI_NULL_PTR;
    }

    //snprintf(szIndexName,sizeof(szIndexName), "%s.%s", pstPlayAttr->szFileName, "idx");
    PVR_Index_GetIdxFileName(szIndexName, (HI_CHAR *)pstPlayAttr->szFileName);
    /* on recording file, play it, and return the recorded index handle */
    if (PVRIndexIsFileRecording(szIndexName, &handle))
    {
        handle->bIsPlay = HI_TRUE;
        PVR_LOCK(&(handle->stMutex));
        handle->bRecReachPlay = HI_FALSE;
        PVR_UNLOCK(&(handle->stMutex));
        return handle;
    }

    handle = &g_stPVRIndex[chnID];
    HI_ASSERT(HI_FALSE == handle->bIsPlay);
    
    if (-1 == pthread_mutex_init(&(handle->stMutex), NULL))
    {
        HI_ERR_PVR("init mutex lock for PVR index failed \n");
        handle->bIsPlay = HI_FALSE;
        return HI_NULL_PTR;
    }

    PVRIndexSetDftAttr(handle);
    memset(handle->szIdxFileName, 0, sizeof(handle->szIdxFileName));
    strncpy(handle->szIdxFileName, szIndexName, strlen(szIndexName));
    
    handle->bIsPlay = HI_TRUE;
    handle->bIsRec = HI_FALSE;

    /* check whether index file exist or not, if not, just  track it and not to open */
    if (PVR_CHECK_FILE_EXIST(szIndexName))
    {
        *pIsNoIdx = HI_FALSE;
        handle->s32ReadFd = PVR_OPEN(szIndexName, PVR_FOPEN_MODE_INDEX_READ);
        if (PVR_FILE_INVALID_FILE == handle->s32ReadFd)
        {
            HI_ERR_PVR("PVR open Index File for read failed !\n");
            goto ErrorExit0;
        }

        handle->s32SeekFd = PVR_OPEN(szIndexName, PVR_FOPEN_MODE_INDEX_READ);
        if (PVR_FILE_INVALID_FILE == handle->s32SeekFd)
        {
            HI_ERR_PVR("PVR open Index File for seek failed !\n");
            PVR_CLOSE(handle->s32ReadFd);
            goto ErrorExit0;
        }

        handle->s32HeaderFd = PVR_OPEN(szIndexName, PVR_FOPEN_MODE_INDEX_BOTH);
        if (PVR_FILE_INVALID_FILE == handle->s32HeaderFd)
        {
            HI_ERR_PVR("PVR open Index File for Header failed !\n");
            PVR_CLOSE(handle->s32SeekFd);
            PVR_CLOSE(handle->s32ReadFd);
            goto ErrorExit0;
        }
        PVR_Index_GetVersion(handle);
        PVR_Index_GetEntryLength(handle);
        memset(&handle->stIdxReadCache,0,sizeof(HIPVR_IDX_BUF_S));
#ifndef HI_PVR_EXTRA_BUF_SUPPORT
        s32CacheLen = PVR_DFT_IDX_READCACHE_SIZE;
#else
        s32CacheLen = handle->u32EntryLength * PVR_INDEX_READ_CACHE_NUM;
#endif
        handle->stIdxReadCache.pu8Addr = HI_MALLOC(HI_ID_PVR, s32CacheLen);
        if (handle->stIdxReadCache.pu8Addr)
        {
#ifndef HI_PVR_EXTRA_BUF_SUPPORT        
            handle->stIdxReadCache.u32BufferLen = PVR_DFT_IDX_READCACHE_SIZE;
#else
            handle->stIdxReadCache.u32BufferLen = 0;
#endif
            handle->stIdxReadCache.u32UsedSize = 0;
            handle->stIdxReadCache.u32StartOffset = 0;
            handle->stIdxReadCache.u32UsedNum = 0;
            handle->stIdxReadCache.u32StartNum = 0;
        }
        else
        {
            HI_ERR_PVR("HI_MALLOC read cache buffer failed!\n");
            handle->stIdxReadCache.u32BufferLen = 0;
        }
        if(-1 == pthread_mutex_init(&(handle->stIdxReadCache.stCacheMutex), NULL))
        {
            HI_ERR_PVR("init mutex lock for PVR index failed,check it\n");
            goto ErrorExit1;
        }
        
        ret = PVRIndexReadHeaderInfo(handle);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_PVR("PVR read Index File Header failed !\n");
            PVR_CLOSE(handle->s32ReadFd);
            PVR_CLOSE(handle->s32SeekFd);
            PVR_CLOSE(handle->s32HeaderFd);
            goto ErrorExit2;
        }

        u64TsFileSize = PVR_FILE_GetFileSize64(pstPlayAttr->szFileName);
        if (u64TsFileSize == 0)
        {
            HI_ERR_PVR("TS file size is 0!\n");
            PVR_CLOSE(handle->s32ReadFd);
            PVR_CLOSE(handle->s32SeekFd);
            PVR_CLOSE(handle->s32HeaderFd);
            goto ErrorExit2;
        }

        ret = PVRIndexCheckFileCorrect(handle, u64TsFileSize);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_PVR("Index not match ts file!\n");
            PVR_CLOSE(handle->s32ReadFd);
            PVR_CLOSE(handle->s32SeekFd);
            PVR_CLOSE(handle->s32HeaderFd);
            goto ErrorExit2;
        }
        
        PVRINDEX_GET_ENTRY_BYNUM(handle, &stStartEntry, handle->stCycMgr.u32StartFrame, ret);
        handle->enIndexType = (HI_UNF_PVR_REC_INDEX_TYPE_E)PVRINDEX_GET_INDEXTYPE(handle->u32EntryVersion, &stStartEntry);

        /*alone play, not open write handle */
        handle->s32WriteFd = PVR_FILE_INVALID_FILE;
    }
    else  /* NO index file, no need to process! */
    {
        HI_ERR_PVR("No index file for '%s' found!.\n", pstPlayAttr->szFileName);
        goto ErrorExit0;
    }
    return handle;
ErrorExit2:
    (HI_VOID)pthread_mutex_destroy(&(handle->stIdxReadCache.stCacheMutex));
ErrorExit1:
    if (handle->stIdxReadCache.pu8Addr)
    {
        HI_FREE(HI_ID_PVR, handle->stIdxReadCache.pu8Addr);
    }
    memset(&handle->stIdxReadCache,0,sizeof(HIPVR_IDX_BUF_S));
ErrorExit0:    
    handle->bIsPlay = HI_FALSE;
    (HI_VOID)pthread_mutex_destroy(&(handle->stMutex));
    return HI_NULL_PTR;
}


/*****************************************************************************
 Prototype       : PVR_Index_CreatRec
 Description     : create pvr rec channel
 Input           : pfileName  **
                    enIndexType: need this just only on recording
 Output          : None
 Return Value    : HI_NULL_PTR: failure, maybe operate file failure or the file playing, which the need to record.
                   handle: success, retruen the handle of index
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/16
    Author       : q46153
    Modification : Created function
  2.Date         : 2010/06/19
    Author       : Jiang Lei
    Modification : modify for HD
*****************************************************************************/
PVR_INDEX_HANDLE PVR_Index_CreatRec(HI_U32 chnID, HI_UNF_PVR_REC_ATTR_S *pstRecAttr)
{
    PVR_INDEX_HANDLE handle = HI_NULL;
    HI_CHAR szIndexName[PVR_MAX_FILENAME_LEN] = {0};
    HI_U32 i = 0;

    if (!pstRecAttr)
    {
        return HI_NULL_PTR;
    }

    PVR_Index_GetIdxFileName(szIndexName,  pstRecAttr->szFileName);
    //snprintf(szIndexName,sizeof(szIndexName), "%s.ts.%s", pstRecAttr->szFileName, "idx");

    /** if the file has been playing, return HI_NULL_PTR */
    if (PVRIndexIsFilePlaying(szIndexName, &handle))
    {
        HI_ERR_PVR("the file %s is playing, please stop it before recordring the same file.\n", pstRecAttr->szFileName);
        return HI_NULL_PTR;
    }

    for (i = 0; i < sizeof(g_u32RecChnStat)/sizeof(HI_U32); i++)
    {
        if (PVR_INDEX_REC_CHN_UNUSED == g_u32RecChnStat[i])
        {
            g_u32RecChnStat[i] = PVR_INDEX_REC_CHN_USED;
            break;
        }
    }
    if (i >= PVR_REC_MAX_CHN_NUM)
    {
        HI_ERR_PVR("Rec channel %d is out of range.\n", i);
        return HI_NULL_PTR;
    }

    handle = &g_stPVRIndex[i + PVR_PLAY_MAX_CHN_NUM];

    HI_ASSERT(HI_FALSE == handle->bIsRec);
    if (-1 == pthread_mutex_init(&(handle->stMutex), NULL))
    {
        HI_ERR_PVR("init mutex lock for PVR index failed \n");
        g_u32RecChnStat[i] = PVR_INDEX_REC_CHN_UNUSED;
        return HI_NULL_PTR;
    }
    
    PVRIndexSetDftAttr(handle);
    handle->u32EntryVersion = 1;
    PVR_Index_GetEntryLength(handle);
    memset(handle->szIdxFileName, 0, sizeof(handle->szIdxFileName));
    strncpy(handle->szIdxFileName, szIndexName, strlen(szIndexName));

    PVR_IDX_CHECK_CYC_SIZE(pstRecAttr);

    handle->bIsPlay = HI_FALSE;
    handle->bIsRec = HI_TRUE;
    handle->u32RecPicParser = i;
    handle->enIndexType = pstRecAttr->enIndexType;
    handle->u32DavBufSize = pstRecAttr->u32DavBufSize;
    handle->stCycMgr.bIsRewind = pstRecAttr->bRewind;
    handle->stCycMgr.u64MaxCycSize = (pstRecAttr->u64MaxFileSize / PVR_FIFO_WRITE_BLOCK_SIZE) * PVR_FIFO_WRITE_BLOCK_SIZE;
    handle->stCycMgr.u32MaxCycTimeInMs = pstRecAttr->u64MaxTimeInMs ;
    handle->stCycMgr.u32EndFrame = -1;
    handle->stCycMgr.u32LastFrame = -1;
#ifdef HI_PVR_EXTRA_BUF_SUPPORT
    handle->stFileCycInfo.u32EndFrame = -1;
    handle->stFileCycInfo.u32LastFrame = -1;
#endif

    handle->stCycMgr.enRewindType = PVR_INDEX_REWIND_BY_BOTH;
    
    if ((handle->stCycMgr.u64MaxCycSize == 0) && (handle->stCycMgr.u32MaxCycTimeInMs > 0))
    {
        handle->stCycMgr.enRewindType = PVR_INDEX_REWIND_BY_TIME;
    }
    if ((handle->stCycMgr.u64MaxCycSize > 0) && (handle->stCycMgr.u32MaxCycTimeInMs == 0))
    {
        handle->stCycMgr.enRewindType = PVR_INDEX_REWIND_BY_SIZE;
    }

    handle->s32WriteFd = PVR_OPEN(szIndexName, PVR_FOPEN_MODE_INDEX_WRITE);
    if (PVR_FILE_INVALID_FILE == handle->s32WriteFd)
    {
        HI_ERR_PVR("PVR open Index File for write failed !\n");
        goto ErrorExit;
    }

    handle->s32SeekFd = PVR_OPEN(szIndexName, PVR_FOPEN_MODE_INDEX_READ);
    if (PVR_FILE_INVALID_FILE == handle->s32SeekFd)
    {
        HI_ERR_PVR("PVR open Index File for seek failed !\n");
        PVR_CLOSE(handle->s32WriteFd);
        goto ErrorExit;
    }

    handle->s32HeaderFd = PVR_OPEN(szIndexName, PVR_FOPEN_MODE_INDEX_BOTH);
    if (PVR_FILE_INVALID_FILE == handle->s32HeaderFd)
    {
        HI_ERR_PVR("PVR open Index File for Idx Header failed !\n");
        PVR_CLOSE(handle->s32WriteFd);
        PVR_CLOSE(handle->s32SeekFd);
        goto ErrorExit;
    }

    handle->s32ReadFd = PVR_OPEN(szIndexName, PVR_FOPEN_MODE_INDEX_READ);
    if (PVR_FILE_INVALID_FILE == handle->s32ReadFd)
    {
        PVR_CLOSE(handle->s32WriteFd);
        PVR_CLOSE(handle->s32SeekFd);
        PVR_CLOSE(handle->s32HeaderFd);
        (HI_VOID)PVR_REMOVE(szIndexName);
        HI_ERR_PVR("PVR open Index File for read failed !\n");
        goto ErrorExit;
    }
#ifdef HI_PVR_EXTRA_BUF_SUPPORT
    handle->stIdxWriteCache.pu8Addr = HI_MALLOC(HI_ID_PVR, PVR_INDEX_WRITE_CACHE_NUM * handle->u32EntryLength);
    if (handle->stIdxWriteCache.pu8Addr)
    {
        handle->stIdxWriteCache.u32BufferLen = 0;
        handle->stIdxWriteCache.u32Read      = 0;
        handle->stIdxWriteCache.u32Write     = 0;
        handle->stIdxWriteCache.u32UsedNum   = 0;
        handle->stIdxWriteCache.u32StartNum  = 0;
    }
    else
    {
        handle->stIdxWriteCache.u32BufferLen = 0;
        HI_ERR_PVR("HI_MALLOC write cache buffer failed!\n");
    }

    if (-1 == pthread_mutex_init(&(handle->stIdxWriteCache.stCacheMutex), NULL))
    {
        HI_ERR_PVR("init mutex lock for PVR index failed,check it\n");
        if (handle->stIdxWriteCache.pu8Addr != HI_NULL)
        {
            HI_FREE(HI_ID_PVR,handle->stIdxWriteCache.pu8Addr);
        }

        PVR_CLOSE(handle->s32WriteFd);
        PVR_CLOSE(handle->s32SeekFd);
        PVR_CLOSE(handle->s32HeaderFd);
        PVR_CLOSE(handle->s32ReadFd);
        (HI_VOID)PVR_REMOVE(szIndexName);
        goto ErrorExit;
    }

    memset(&handle->stIdxReadCache, 0, sizeof(HIPVR_IDX_BUF_S));
    handle->stIdxReadCache.pu8Addr = HI_MALLOC(HI_ID_PVR, PVR_INDEX_READ_CACHE_NUM * handle->u32EntryLength);
    if (handle->stIdxReadCache.pu8Addr)
    {
        handle->stIdxReadCache.u32BufferLen = 0;
        handle->stIdxReadCache.u32UsedNum = 0;
        handle->stIdxReadCache.u32StartNum = 0;
        handle->stIdxReadCache.bDirty = HI_FALSE;
    }
    else
    {
        HI_ERR_PVR("HI_MALLOC read cache buffer failed!\n");
        handle->stIdxReadCache.u32BufferLen = 0;
    }
    
    if (-1 == pthread_mutex_init(&(handle->stIdxReadCache.stCacheMutex), NULL))
    {
        HI_ERR_PVR("init mutex lock for PVR index failed,check it\n");
        if (handle->stIdxWriteCache.pu8Addr != HI_NULL)
        {
            HI_FREE(HI_ID_PVR, handle->stIdxWriteCache.pu8Addr);
        }
        if (handle->stIdxReadCache.pu8Addr != HI_NULL)
        {
            HI_FREE(HI_ID_PVR, handle->stIdxReadCache.pu8Addr);
        }

        PVR_CLOSE(handle->s32WriteFd);
        PVR_CLOSE(handle->s32SeekFd);
        PVR_CLOSE(handle->s32HeaderFd);
        PVR_CLOSE(handle->s32ReadFd);
        (HI_VOID)PVR_REMOVE(szIndexName);
        (HI_VOID)pthread_mutex_destroy(&(handle->stIdxWriteCache.stCacheMutex));
        goto ErrorExit;
    }
#else
    handle->stIdxWriteCache.pu8Addr = HI_MALLOC(HI_ID_PVR, PVR_DFT_IDX_WRITECACHE_SIZE);
    if (handle->stIdxWriteCache.pu8Addr)
    {
        handle->stIdxWriteCache.u32BufferLen = PVR_DFT_IDX_WRITECACHE_SIZE;
        handle->stIdxWriteCache.u32UsedSize = 0;
        handle->stIdxWriteCache.u32StartOffset = 0;
        if (-1 == pthread_mutex_init(&(handle->stIdxWriteCache.stCacheMutex), NULL))
        {
            HI_ERR_PVR("init mutex lock for PVR index failed,check it\n");
            if (handle->stIdxWriteCache.pu8Addr != HI_NULL)
            {
                HI_FREE(HI_ID_PVR, handle->stIdxWriteCache.pu8Addr);
            }
            PVR_CLOSE(handle->s32WriteFd);
            PVR_CLOSE(handle->s32SeekFd);
            PVR_CLOSE(handle->s32HeaderFd);
            PVR_CLOSE(handle->s32ReadFd);
            (HI_VOID)PVR_REMOVE(szIndexName);
            goto ErrorExit;
        }
    }
    else
    {
        handle->stIdxWriteCache.u32BufferLen = 0;
        HI_ERR_PVR("HI_MALLOC write cache buffer failed!\n");
    }

    memset(&handle->stIdxReadCache, 0, sizeof(HIPVR_IDX_BUF_S));
    handle->stIdxReadCache.pu8Addr = HI_MALLOC(HI_ID_PVR, PVR_DFT_IDX_READCACHE_SIZE);
    if (handle->stIdxReadCache.pu8Addr)
    {
        handle->stIdxReadCache.u32BufferLen = PVR_DFT_IDX_READCACHE_SIZE;
        handle->stIdxReadCache.u32UsedSize = 0;
        handle->stIdxReadCache.u32StartOffset = 0;
        if (-1 == pthread_mutex_init(&(handle->stIdxReadCache.stCacheMutex), NULL))
        {
            HI_ERR_PVR("init mutex lock for PVR index failed,check it\n");
            if (handle->stIdxWriteCache.pu8Addr != HI_NULL)
            {
                HI_FREE(HI_ID_PVR, handle->stIdxWriteCache.pu8Addr);
            }
            if (handle->stIdxReadCache.pu8Addr != HI_NULL)
            {
                HI_FREE(HI_ID_PVR, handle->stIdxReadCache.pu8Addr);
            }
            PVR_CLOSE(handle->s32WriteFd);
            PVR_CLOSE(handle->s32SeekFd);
            PVR_CLOSE(handle->s32HeaderFd);
            PVR_CLOSE(handle->s32ReadFd);
            (HI_VOID)PVR_REMOVE(szIndexName);
            (HI_VOID)pthread_mutex_destroy(&(handle->stIdxWriteCache.stCacheMutex));
            goto ErrorExit;
        }
    }
    else
    {
        HI_ERR_PVR("HI_MALLOC read cache buffer failed!\n");
        handle->stIdxReadCache.u32BufferLen = 0;
    }
#endif
        
    handle->u32FRollTime = 0;
    handle->u32DeltaDispTimeMs = 0;
    handle->u32LastDispTime = 0;
    handle->u32TimeShiftTillEndTimeMs = 0;
    handle->u32TimeShiftTillEndCnt = 0;
    handle->u32TotalRecTimeMs = 0;
    handle->bRecReachFix = HI_FALSE;
    handle->u32PauseFrameCnt = 0;
    handle->bTimeRewindFlagForFile = HI_FALSE;
    handle->bTimeRewinding = HI_FALSE;
    handle->bRewindFlagForIndex = HI_FALSE;
    handle->u64TimeRewindMaxSize = 0;
    handle->u64DeltaGlobalOffset = 0;
    memset(&(handle->stRecIdxInfo), 0, sizeof(PVR_REC_INDEX_INFO_S));

    memset(&(handle->stTimeStamp), 0, sizeof(PVR_REC_TIMESTAMP_S));
    handle->stTimeStamp.u32DeltaTimeStamp     = PVR_INDEX_DEFAULT_FRAME_TIMESTAMP;
    handle->stTimeStamp.u32MinPTSGapWhenBegin = PVR_INDEX_DEFAULT_FRAME_TIMESTAMP;
    handle->stTimeStamp.stLastIFrmPtsInfo.u32PtsMs = PVR_INDEX_INVALID_PTSMS;
    handle->stTimeStamp.stCurIFrmPtsInfo.u32PtsMs  = PVR_INDEX_INVALID_PTSMS;

    UNUSED(chnID);

    handle->stRefList.pstRefHead = HI_MALLOC(HI_ID_PVR, PVR_INDEX_REF_LIST_NODE_NUM * sizeof(PVR_REC_REF_LIST_NODE_S));
    if (handle->stRefList.pstRefHead == HI_NULL)
    {
        HI_ERR_PVR("malloc failed\n");
    }
    else
    {
        memset((HI_VOID *)handle->stRefList.pstRefHead, 0, PVR_INDEX_REF_LIST_NODE_NUM * sizeof(PVR_REC_REF_LIST_NODE_S));
    }
    handle->stRefList.u32Write = 0;

    return handle;

ErrorExit:
    g_u32RecChnStat[i] = PVR_INDEX_REC_CHN_UNUSED;
    (HI_VOID)pthread_mutex_destroy(&(handle->stMutex));
    return HI_NULL_PTR;
}


/*****************************************************************************
 Prototype       : PVR_Index_Destroy
 Description     : de-init index module, release index handle and relative resource
                 may stop either record or play at any time, which are independent
 Input           : handle  **
                   u32PlayOrRec  need to stop record or play
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/16
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 PVR_Index_Destroy(PVR_INDEX_HANDLE handle, HI_U32 u32PlayOrRec)
{
    PVR_CHECK_POINTER(handle);

    if (PVR_INDEX_PLAY == u32PlayOrRec)
    {
        handle->bIsPlay = HI_FALSE;
    }
    else
    {
        handle->bIsRec = HI_FALSE;
    }

    if (handle->bIsPlay || handle->bIsRec)
    {
        HI_WARN_PVR("also play or rec was using this index.\n");
        return HI_SUCCESS;
    }

#ifndef HI_PVR_EXTRA_BUF_SUPPORT
    
    if (PVR_INDEX_PLAY != u32PlayOrRec)
    {
        (HI_VOID)PVR_Index_FlushIdxWriteCache(handle);
        (HI_VOID)PVRIndexWriteIndexCycMgr(handle, &handle->stCycMgr);
    }

    if (handle->s32HeaderFd && (handle->s32HeaderFd != PVR_FILE_INVALID_FILE))
    {
        PVR_FSYNC(handle->s32HeaderFd);
    }

    if (handle->s32WriteFd && (handle->s32WriteFd != PVR_FILE_INVALID_FILE))
    {
        PVR_FSYNC(handle->s32WriteFd);
    }
#endif
    (HI_VOID)pthread_mutex_destroy(&(handle->stMutex));

    /* close index file */
    if (handle->s32ReadFd && (handle->s32ReadFd != PVR_FILE_INVALID_FILE))
    {
        PVR_CLOSE(handle->s32ReadFd);
        handle->s32ReadFd = PVR_FILE_INVALID_FILE;
    }

    if (handle->s32WriteFd && (handle->s32WriteFd != PVR_FILE_INVALID_FILE))
    {
        PVR_CLOSE(handle->s32WriteFd);
        handle->s32WriteFd = PVR_FILE_INVALID_FILE;
    }

    if (handle->s32SeekFd && (handle->s32SeekFd != PVR_FILE_INVALID_FILE))
    {
        PVR_CLOSE(handle->s32SeekFd);
        handle->s32SeekFd = PVR_FILE_INVALID_FILE;
    }

    if (handle->s32HeaderFd && (handle->s32HeaderFd != PVR_FILE_INVALID_FILE))
    {
        PVR_CLOSE(handle->s32HeaderFd);
        handle->s32HeaderFd = PVR_FILE_INVALID_FILE;
    }

    /* release index handle */
    if ((handle->u32RecPicParser != 0xffffffff) && (PVR_REC_MAX_CHN_NUM > handle->u32RecPicParser))
    {
        g_u32RecChnStat[handle->u32RecPicParser] = PVR_INDEX_REC_CHN_UNUSED;
        handle->u32RecPicParser = 0xffffffff;
    }

#ifndef HI_PVR_EXTRA_BUF_SUPPORT    
    if (handle->stIdxWriteCache.pu8Addr)
    {
        HI_FREE(HI_ID_PVR, handle->stIdxWriteCache.pu8Addr);
        handle->stIdxWriteCache.pu8Addr = HI_NULL;
        handle->stIdxWriteCache.u32StartOffset = 0;
        handle->stIdxWriteCache.u32UsedSize = 0;
    }
    (HI_VOID)pthread_mutex_destroy(&(handle->stIdxWriteCache.stCacheMutex));

    if (handle->stIdxReadCache.pu8Addr)
    {
        HI_FREE(HI_ID_PVR, handle->stIdxReadCache.pu8Addr);
        handle->stIdxReadCache.pu8Addr = HI_NULL;
        handle->stIdxReadCache.u32StartOffset = 0;
        handle->stIdxReadCache.u32UsedSize = 0;
    }
#else
    if (handle->stIdxWriteCache.pu8Addr)
    {
        HI_FREE(HI_ID_PVR, handle->stIdxWriteCache.pu8Addr);
        handle->stIdxWriteCache.pu8Addr = HI_NULL;
        handle->stIdxWriteCache.u32StartNum = 0;
        handle->stIdxWriteCache.u32UsedNum = 0;
    }
    (HI_VOID)pthread_mutex_destroy(&(handle->stIdxWriteCache.stCacheMutex));

    if (handle->stIdxReadCache.pu8Addr)
    {
        HI_FREE(HI_ID_PVR, handle->stIdxReadCache.pu8Addr);
        handle->stIdxReadCache.pu8Addr = HI_NULL;
        handle->stIdxReadCache.u32StartNum = 0;
        handle->stIdxReadCache.u32UsedNum = 0;
    }
#endif
    if (handle->stRefList.pstRefHead)
    {
        HI_FREE(HI_ID_PVR, handle->stRefList.pstRefHead);
        handle->stRefList.pstRefHead = HI_NULL;
        handle->stRefList.u32Write = 0;
    }

    (HI_VOID)pthread_mutex_destroy(&(handle->stIdxReadCache.stCacheMutex));

    return HI_SUCCESS;
}



/*
  |---n * sizeof(PVR_INDEX_ENTRY_S)---|
  +---------+-------------------------+-------...
  | CycInfo | UsrDataInfo             | Idx data
  +---------+-------------------------+---------...
*/
HI_S32 PVR_Index_PrepareHeaderInfo(PVR_INDEX_HANDLE handle, HI_U32 u32UsrDataLen, HI_U32 u32Vtype)
{
    PVR_IDX_HEADER_INFO_S stIdxHeaderInfo;
    HI_U32 u32HeadInfoSize = 0;
    HI_S32 s32WriteRet;
    HI_U8* pTmpBuff = NULL;

    memset(&stIdxHeaderInfo, 0x0, sizeof(PVR_IDX_HEADER_INFO_S));

    stIdxHeaderInfo.u32CADataInfoLen = PVR_MAX_CADATA_LEN;
    HI_INFO_PVR("\t\thandle->u32EntryLength = %u\n", handle->u32EntryLength);
    /* compute HeaderSize, which should be the times of sizeof(PVR_INDEX_ENTRY_S)*/
    u32HeadInfoSize = (sizeof(PVR_IDX_HEADER_INFO_S) + stIdxHeaderInfo.u32CADataInfoLen + u32UsrDataLen + handle->u32EntryLength)
                      / handle->u32EntryLength * handle->u32EntryLength;
    HI_INFO_PVR("\t\tu32HeadInfoSize = %u\n", u32HeadInfoSize);
    stIdxHeaderInfo.u32HeaderLen = u32HeadInfoSize;
    stIdxHeaderInfo.u32StartCode = PVR_INDEX_HEADER_CODE;
    stIdxHeaderInfo.u32UsrDataInfoLen = u32UsrDataLen;

    stIdxHeaderInfo.u64ValidSize = handle->stCycMgr.u64MaxCycSize;
    stIdxHeaderInfo.stCycInfo.u32StartFrame = handle->stCycMgr.u32StartFrame;
    stIdxHeaderInfo.stCycInfo.u32EndFrame   = handle->stCycMgr.u32EndFrame;
    stIdxHeaderInfo.stCycInfo.u32LastFrame  = handle->stCycMgr.u32LastFrame;
    stIdxHeaderInfo.stCycInfo.u32IsRewind   = handle->stCycMgr.bIsRewind;
    /* use u32Reserved low 16bits to store HI_UNF_VCODEC_TYPE_E */
    stIdxHeaderInfo.u32Reserved = 0xFFFF & (100 + u32Vtype); 
    stIdxHeaderInfo.u32Reserved |= 0x30000 & (1 << 16);//0 -- old, 1 -- new
    if ((HI_S32)stIdxHeaderInfo.u32UsrDataInfoLen < 0)
    {
        HI_ERR_PVR("calc usr data len:%d err\n", stIdxHeaderInfo.u32UsrDataInfoLen);
        return HI_FAILURE;
    }

    pTmpBuff = HI_MALLOC(HI_ID_PVR, u32HeadInfoSize);
    if (NULL == pTmpBuff)
    {
        HI_ERR_PVR("no mem, want=%u\n", u32HeadInfoSize);
        return HI_FAILURE;
    }
    memset(pTmpBuff, 0x0, u32HeadInfoSize);
    memcpy(pTmpBuff, &stIdxHeaderInfo, sizeof(PVR_IDX_HEADER_INFO_S));

    s32WriteRet = (HI_S32)u32HeadInfoSize;
    if (s32WriteRet != PVR_WRITE((HI_VOID*)pTmpBuff, (size_t)u32HeadInfoSize, handle->s32HeaderFd, 0))
    {
        HI_ERR_PVR("write header info err, fd:%d, size:%d\n", handle->s32HeaderFd, u32HeadInfoSize);
        HI_FREE(HI_ID_PVR, pTmpBuff);
        return HI_FAILURE;
    }

    HI_INFO_PVR("write header info ok(%uByte writen), UDLen:%u, MaxSize:%llu\n", 
                 s32WriteRet, stIdxHeaderInfo.u32UsrDataInfoLen, stIdxHeaderInfo.u64ValidSize);
    PVR_FSYNC(handle->s32HeaderFd);

    handle->u32IdxStartOffsetLen = u32HeadInfoSize;

    HI_FREE(HI_ID_PVR, pTmpBuff);

    return HI_SUCCESS;
}

/* reset the player attribute, called when start play*/
HI_VOID PVR_Index_ResetPlayAttr(PVR_INDEX_HANDLE handle)
{
    PVR_INDEX_LOCK(&handle->stMutex);
    handle->u32ReadFrame = handle->stCycMgr.u32StartFrame;
    handle->bRecReachPlay = HI_FALSE;
    memset(&handle->stCurPlayFrame, 0x00, sizeof(handle->stCurPlayFrame));
    PVR_INDEX_UNLOCK(&handle->stMutex);
}

/* reset the player attribute, called when start record */
HI_VOID PVR_Index_ResetRecAttr(PVR_INDEX_HANDLE handle)
{
    handle->u64GlobalOffset = 0;
    handle->u64OverflowOffset = 0;
    handle->u32PauseFrame  = 0;
    handle->u64PauseOffset = PVR_INDEX_PAUSE_INVALID_OFFSET;
    handle->u32WriteFrame = 0;
    handle->u16RecLastIframe = PVR_INDEX_INVALID_I_FRAME_OFFSET;
    handle->u32RecLastValidPtsMs = PVR_INDEX_INVALID_PTSMS;
    handle->u32RecFirstFrmTimeMs = 0;
    handle->stCycMgr.u32StartFrame = 0;
    handle->stCycMgr.u32EndFrame = -1;
    handle->stCycMgr.u32LastFrame = -1;
    handle->stCycMgr.u32CycTimes = 0;
    handle->stCycMgr.u32StartFrame = 0;
    memset(&handle->stCurRecFrame, 0x00, sizeof(handle->stCurRecFrame));
    return;
}

/* set current frame size is zero, prevent from repeatly sending the last frame when switch play mode */
HI_S32 PVR_Index_ChangePlayMode(PVR_INDEX_HANDLE handle)
{
    PVRINDEX_SET_FRAMESIZE(handle->u32EntryVersion, &(handle->stCurPlayFrame), 0);
    return HI_SUCCESS;
}


HI_S32 PVR_Index_GetUsrDataInfo(HI_S32 s32Fd, HI_U8* pBuff, HI_U32 u32BuffSize)
{
    PVR_IDX_HEADER_INFO_S stIdxHeaderInfo = {0};
    HI_S32 s32ReadRet;
    HI_U32 u32ReadLen;

    if (HI_SUCCESS != PVRIndexGetHeaderInfo(s32Fd, &stIdxHeaderInfo))
    {
        HI_ERR_PVR("No userDataInfo in this file.\n");
        return HI_ERR_PVR_FILE_CANT_READ;
    }

    u32ReadLen = (stIdxHeaderInfo.u32UsrDataInfoLen > u32BuffSize) ? (u32BuffSize) : (stIdxHeaderInfo.u32UsrDataInfoLen);

    s32ReadRet = PVR_READ(pBuff, u32ReadLen, s32Fd, PVR_GET_USR_DATA_OFFSET(stIdxHeaderInfo));
    if (s32ReadRet != (HI_S32)u32ReadLen)
    {
        HI_ERR_PVR("read usr data info err, read ret:0x%x\n", s32ReadRet);
        return HI_FAILURE;
    }
    return s32ReadRet;
}

HI_S32 PVR_Index_SetUsrDataInfo(HI_S32 s32Fd, HI_U8* pBuff, HI_U32 u32UsrDataLen)
{
    PVR_IDX_HEADER_INFO_S stIdxHeaderInfo = {0};
    HI_S32 s32WriteRet = 0;

    if (HI_SUCCESS != PVRIndexGetHeaderInfo(s32Fd, &stIdxHeaderInfo))
    {
        return HI_ERR_PVR_FILE_CANT_READ;
    }
    if (stIdxHeaderInfo.u32UsrDataInfoLen < u32UsrDataLen)
    {
        HI_ERR_PVR("usr data len is no enough:%d\n", stIdxHeaderInfo.u32UsrDataInfoLen);
        return HI_FAILURE;
    }

    s32WriteRet = (HI_S32)PVR_WRITE((HI_VOID*)pBuff, (size_t)u32UsrDataLen, s32Fd, (off_t)PVR_GET_USR_DATA_OFFSET(stIdxHeaderInfo));
    if (s32WriteRet != (HI_S32)u32UsrDataLen)
    {
        HI_ERR_PVR("read usr data info err:0x%x\n", s32WriteRet);
        return HI_FAILURE;
    }

    return s32WriteRet;
}

HI_S32 PVR_Index_GetCADataInfo(HI_S32 s32Fd, HI_U8* pBuff, HI_U32 u32BuffSize)
{
    PVR_IDX_HEADER_INFO_S stIdxHeaderInfo = {0};
    HI_S32 s32ReadRet;
    HI_U32 u32ReadLen;

    if (HI_SUCCESS != PVRIndexGetHeaderInfo(s32Fd, &stIdxHeaderInfo))
    {
        HI_ERR_PVR("No CADataInfo in this file.\n");
        return HI_ERR_PVR_FILE_CANT_READ;
    }

    u32ReadLen = (stIdxHeaderInfo.u32CADataInfoLen > u32BuffSize) ? (u32BuffSize) : (stIdxHeaderInfo.u32CADataInfoLen);

    s32ReadRet = PVR_READ(pBuff, u32ReadLen, s32Fd, PVR_GET_CA_DATA_OFFSET());
    if (s32ReadRet != u32ReadLen)
    {
        HI_ERR_PVR("read usr CA info err, read ret:0x%x\n", s32ReadRet);
        return HI_FAILURE;
    }

    return s32ReadRet;
}


HI_S32 PVR_Index_SetCADataInfo(HI_S32 s32Fd, HI_U8* pBuff, HI_U32 u32CADataLen)
{
    PVR_IDX_HEADER_INFO_S stIdxHeaderInfo;
    HI_S32 s32WriteRet = 0;
    
    memset(&stIdxHeaderInfo, 0x00, sizeof(stIdxHeaderInfo));
    if (HI_SUCCESS != PVRIndexGetHeaderInfo(s32Fd, &stIdxHeaderInfo))
    {
        return HI_ERR_PVR_FILE_CANT_READ;
    }

    if (stIdxHeaderInfo.u32CADataInfoLen < u32CADataLen)
    {
        HI_ERR_PVR("CA data len is no enough:%d\n", stIdxHeaderInfo.u32CADataInfoLen);
        return HI_FAILURE;
    }

    s32WriteRet = PVR_WRITE(pBuff, u32CADataLen, s32Fd, PVR_GET_CA_DATA_OFFSET());
    if (s32WriteRet != u32CADataLen)
    {
        HI_ERR_PVR("read CA data info err:0x%x\n", s32WriteRet);
        return HI_FAILURE;
    }

    return s32WriteRet;
}

HI_VOID PVR_Index_GetIdxInfo(PVR_INDEX_HANDLE handle)
{
    HI_S32 i = 0;
    HI_U32 u32StartFrm = 0;
    HI_U32 u32EndFrm = 0;
    HI_U32 u32LastFrm = 0;
    HI_U32 u32FindStart = 0;
    PVR_INDEX_ENTRY_NEW_S stEntryTmp;
    HI_U32 u32CurGopSize = 0;
    HI_U32 u32GopSizeSeg = 0;
    HI_U16 u16FrameTypeAndGop = 0;

    memset(&stEntryTmp, 0x00, sizeof(stEntryTmp));
    u32StartFrm = handle->stCycMgr.u32StartFrame;
    u32EndFrm = (handle->stCycMgr.u32EndFrame == -1)?0:handle->stCycMgr.u32EndFrame;
    u32LastFrm = (handle->stCycMgr.u32LastFrame == -1)?0:handle->stCycMgr.u32LastFrame;

    if (u32StartFrm >= u32EndFrm)
    {
        handle->stRecIdxInfo.stIdxInfo.u32FrameTotalNum = u32LastFrm - u32StartFrm + u32EndFrm;
    }
    else
    {
        handle->stRecIdxInfo.stIdxInfo.u32FrameTotalNum = u32LastFrm + 1;
    }

    for (i = (HI_S32)u32EndFrm; i >= (HI_S32)u32FindStart; i--)
    {
        if (HI_SUCCESS == PVR_Index_GetFrameByNum(handle, &stEntryTmp, i))
        {
            u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, &stEntryTmp);
            if (0 != (u16FrameTypeAndGop & 0x1fff))
            {
                u32CurGopSize = (u16FrameTypeAndGop & 0x1fff) + 1;

                handle->stRecIdxInfo.stIdxInfo.u32GopTotalNum++;
                
                if(0 != handle->stRecIdxInfo.stIdxInfo.u32GopTotalNum)
                {
                    u32GopSizeSeg = (u32CurGopSize/10);
                    u32GopSizeSeg = (u32GopSizeSeg > 12) ? 12 : u32GopSizeSeg;
                    handle->stRecIdxInfo.stIdxInfo.u32GopSizeInfo[u32GopSizeSeg]++;
                }

                if (handle->stRecIdxInfo.stIdxInfo.u32MaxGopSize < u32CurGopSize)
                {
                    handle->stRecIdxInfo.stIdxInfo.u32MaxGopSize = u32CurGopSize;
                }

                i -= (HI_S32)(u16FrameTypeAndGop & 0x1fff);
            }
        }

        if ((u32StartFrm >= u32EndFrm)&&(i <= 0))
        {
            u32FindStart = u32StartFrm;
            i = (HI_S32)u32LastFrm;
            u32EndFrm = u32LastFrm;
            continue;
        }
    }

    if (HI_SUCCESS == PVR_Index_GetFrameByNum(handle, &stEntryTmp, u32FindStart))
    {
        u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, &stEntryTmp);
        if (0 != (u16FrameTypeAndGop & 0x1fff))
        {
            handle->stRecIdxInfo.stIdxInfo.u32GopTotalNum--;
            u32GopSizeSeg = (u32CurGopSize/10);
            u32GopSizeSeg = (u32GopSizeSeg > 12) ? 12 : u32GopSizeSeg;
            handle->stRecIdxInfo.stIdxInfo.u32GopSizeInfo[u32GopSizeSeg]--;
        }
    }
}

HI_VOID PVR_Index_GetRecIdxInfo(PVR_INDEX_HANDLE handle)
{
    PVR_IDX_HEADER_INFO_S stIdxHeaderInfo = {0};
    HI_S32 s32ReadRet = 0;

    if (HI_SUCCESS != PVRIndexGetHeaderInfo(handle->s32HeaderFd, &stIdxHeaderInfo))
    {
        HI_ERR_PVR("Can't get index header info.\n");
        return;
    }

    s32ReadRet = PVR_READ(&(handle->stRecIdxInfo), 
                            sizeof(PVR_REC_INDEX_INFO_S), 
                            handle->s32HeaderFd, 
                            PVR_GET_IDX_INFO_OFFSET(stIdxHeaderInfo));

    if (s32ReadRet != sizeof(PVR_REC_INDEX_INFO_S))
    {
        HI_ERR_PVR("Write index info fail ret=0x%x\n", s32ReadRet);
        return;
    }
}


HI_BOOL PVR_Index_CheckSetRecReachPlay(PVR_INDEX_HANDLE handle, HI_U32 u32MinDistance)
{
    HI_U32 u32ReadFrame = 0;
    HI_U32 u32StartFrame = 0;
    HI_U32 u32EndFrame = 0;
    HI_U32 u32LastFrame = 0;

    PVR_INDEX_LOCK(&handle->stMutex);

    if (handle->bRecReachPlay == HI_TRUE)
    {
        PVR_INDEX_UNLOCK(&handle->stMutex);
        return HI_TRUE;
    }

    u32StartFrame = handle->stCycMgr.u32StartFrame;
    u32EndFrame   = (handle->stCycMgr.u32EndFrame == -1)?0:handle->stCycMgr.u32EndFrame;
    u32ReadFrame  = handle->u32ReadFrame;
    u32LastFrame  = (handle->stCycMgr.u32LastFrame == -1)?0:handle->stCycMgr.u32LastFrame;

    if (u32StartFrame < u32EndFrame) /* NOT cycled, 0--S--R--E--L  */
    {

        if (u32StartFrame + u32MinDistance >= u32ReadFrame)
        {
            HI_WARN_PVR("Rec almost over Play: S/R/E/L: %u,%u,%u,%u.\n",
                         u32StartFrame, u32ReadFrame,u32EndFrame, u32LastFrame);
            handle->bRecReachPlay = HI_TRUE;
            PVR_INDEX_UNLOCK(&handle->stMutex);
            return HI_TRUE;
        }
        else
        {
            PVR_INDEX_UNLOCK(&handle->stMutex);
            return HI_FALSE;
        }
    }
    else  /* Cycled */
    {
        if (u32ReadFrame > u32StartFrame) /* 0----E----S----R--L */
        {
            if (u32ReadFrame - u32StartFrame > u32MinDistance)
            {
                PVR_INDEX_UNLOCK(&handle->stMutex);
                return HI_FALSE;
            }
            else
            {
                HI_WARN_PVR("Rec almost over Play: E/S/R/L: %u,%u,%u,%u.\n",
                             u32EndFrame, u32StartFrame, u32ReadFrame, u32LastFrame);
                handle->bRecReachPlay = HI_TRUE;
                PVR_INDEX_UNLOCK(&handle->stMutex);
                return HI_TRUE;
            }
        }
        else /* 0--R--E----S--L */
        {
            HI_U32 u32StartToLast;

            u32StartToLast = u32LastFrame - u32StartFrame;

            if (u32StartToLast + u32ReadFrame > u32MinDistance)
            {
                PVR_INDEX_UNLOCK(&handle->stMutex);
                return HI_FALSE;
            }
            else
            {
                HI_ERR_PVR("Rec almost over Play: R/E/S/L: %u/%u/%u/%u.\n",
                            u32ReadFrame, u32EndFrame, u32StartFrame, u32LastFrame);

                handle->bRecReachPlay = HI_TRUE;
                PVR_INDEX_UNLOCK(&handle->stMutex);
                return HI_TRUE;
            }
        }
    }
}


/*****************************************************************************
 Prototype       : PVR_Index_SeekByFrame2I
 Description     : seek by frame
 Input           : handle  **
                   offset  ** number of frame
                   whence  ** from start frame
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/28
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 PVR_Index_SeekByFrame2I(PVR_INDEX_HANDLE handle, HI_S32 offset, HI_S32 whence)
{
    HI_S32 ret = HI_SUCCESS;

    HI_INFO_PVR("whence:%s, offset:%d\n", WHENCE_STRING(whence), offset);
    if ((SEEK_SET != whence) || (0 > offset))
    {
        return HI_ERR_PVR_INVALID_PARA;
    }
    PVR_INDEX_LOCK(&(handle->stMutex));
    handle->u32ReadFrame = handle->stCycMgr.u32StartFrame;
    PVRIndexCycMoveReadFrame(handle, offset);
    ret = PVRIndexGetNextIEntry(handle, &(handle->stCurPlayFrame));
    if (HI_SUCCESS != ret)
    {
        ret = PVRIndexGetPreIEntry(handle, &(handle->stCurPlayFrame));
        if (HI_SUCCESS != ret)
        {
            HI_ERR_PVR("get next I entry error, file end.\n");
        }
    }
    else
    {
        PVRIndexCycMoveReadFrame(handle, -1);
    }

    HI_WARN_PVR("Ret:%#x. Cur frame Type:%lu, PTS:%u, at last seekto:%u\n", ret, 
               PVR_INDEX_GET_FRAMETYPE(handle->u32EntryVersion, PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, &(handle->stCurPlayFrame))),
               PVRINDEX_GET_PTS(handle->u32EntryVersion, &(handle->stCurPlayFrame)),
               handle->u32ReadFrame);

    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("can not find I frame@both, Err:%#x\n", ret);
        PVR_INDEX_UNLOCK(&(handle->stMutex));
        return ret;
    }

    handle->bRecReachPlay = HI_FALSE;

    PVR_INDEX_UNLOCK(&(handle->stMutex));
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : PVR_Index_SeekToTime
 Description     : seek the read pointer of index to I frame closed to the time value
 Input           : handle         **
 Output          : None
 Return Value    :
  History
  1.Date         : 2010/06/02
    Author       : j40671
    Modification : Created function
*****************************************************************************/
HI_S32 PVR_Index_SeekToTime(PVR_INDEX_HANDLE handle, HI_U32 u32TimeMs)
{
    HI_U32 frameToSeek;
    HI_U32 offsetEndFrame = 0;
    HI_U32 offsetStartFrame = 0;

    /* frame position to seek by time*/
    frameToSeek = PVRIndexFindFrameByTime(handle, u32TimeMs);

    HI_WARN_PVR("seek to time:%d, frame pos:%u\n", u32TimeMs, frameToSeek);

    if (PVR_IDX_ENABLE_REWIND(handle)) /* rewind */
    {
        if (HI_FALSE == PVRIndexIsFrameValid(frameToSeek, &handle->stCycMgr))
        {
            /* frame position is invalid, so check which close to the frame, and then set it */
            offsetEndFrame = frameToSeek - handle->stCycMgr.u32EndFrame;
            offsetStartFrame = handle->stCycMgr.u32StartFrame - frameToSeek;

            HI_WARN_PVR("frame position(%u) to seek is invalid\n", frameToSeek);
            HI_WARN_PVR("Now startFrame is %u, endFrame is %u, lastFrame is %u\n", handle->stCycMgr.u32StartFrame,
                handle->stCycMgr.u32EndFrame, handle->stCycMgr.u32LastFrame);

            if(offsetStartFrame > offsetEndFrame)
            {
                frameToSeek = handle->stCycMgr.u32EndFrame;
            }
            else
            {
                frameToSeek = handle->stCycMgr.u32StartFrame+10;
            }
        }

        /* frameToSeek should be the offset from u32StartFrame*/
        if (frameToSeek >= handle->stCycMgr.u32StartFrame)
        {
            frameToSeek -= handle->stCycMgr.u32StartFrame;
        }
        else
        {
            frameToSeek = handle->stCycMgr.u32LastFrame - handle->stCycMgr.u32StartFrame + frameToSeek;
        }
    }

    HI_WARN_PVR("seek frame position is %u to PVR_Index_SeekByFrame2I. S:%u, E:%u, L:%u\n", frameToSeek, handle->stCycMgr.u32StartFrame,
                handle->stCycMgr.u32EndFrame, handle->stCycMgr.u32LastFrame);

    return PVR_Index_SeekByFrame2I(handle, (HI_S32)frameToSeek, SEEK_SET);

}

/*****************************************************************************
 Prototype       : PVR_Index_SeekByTime
 Description     : By current time,  the start and end time will offset some time, in millisecond.
 Input           : handle         **
 Output          : None
 Return Value    :
  History
  1.Date         : 2010/06/02
    Author       : j40671
    Modification : Created function
*****************************************************************************/
HI_S32 PVR_Index_SeekByTime(PVR_INDEX_HANDLE handle, HI_S64 s64Offset, HI_S32 whence, HI_U32 u32CurPlayTime)
{
    HI_S32 ret;
    PVR_INDEX_ENTRY_NEW_S stFrameTmp;
    HI_U32 u32CurFrmTime;
    HI_U32 u32StartFrmTime;
    HI_U32 u32EndFrmTime;
    HI_U32 u32SeekToTime = 0;
    HI_U32 u32StartFrmPos;
    HI_U32 u32EndFrmPos;

    HI_INFO_PVR("seek pos(%lld) whence:%s.\n", s64Offset,  WHENCE_STRING(whence));

    memset(&stFrameTmp, 0x00, sizeof(stFrameTmp));    

    u32StartFrmPos = handle->stCycMgr.u32StartFrame;
    if (handle->stCycMgr.u32EndFrame > 0)
    {
        u32EndFrmPos = handle->stCycMgr.u32EndFrame;
    }
    else
    {
        u32EndFrmPos = handle->stCycMgr.u32LastFrame;
    }

    PVRINDEX_GET_ENTRY_BYNUM(handle, &stFrameTmp, u32StartFrmPos, ret);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("Can't get StartFrame:%d\n", u32StartFrmPos);
        return ret;
    }
    u32StartFrmTime = PVRINDEX_GET_DISPTIME(handle->u32EntryVersion, &stFrameTmp);

    PVRINDEX_GET_ENTRY_BYNUM(handle, &stFrameTmp, u32EndFrmPos, ret);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("Can't get EndFrame:%d\n", u32EndFrmPos);
        return ret;
    }
    u32EndFrmTime = PVRINDEX_GET_DISPTIME(handle->u32EntryVersion, &stFrameTmp);

    u32CurFrmTime = u32CurPlayTime;

    HI_WARN_PVR("frame info start %d:%d, end %d:%d, cur :%d\n",
                 u32StartFrmPos,u32StartFrmTime, u32EndFrmPos,u32EndFrmTime, u32CurFrmTime);

    if (u32CurFrmTime < u32StartFrmTime)
    {
        u32CurFrmTime = u32StartFrmTime;
    }
    if (u32CurFrmTime > u32EndFrmTime)
    {
        u32CurFrmTime = u32EndFrmTime;
    }

    switch (whence)
    {
    case SEEK_SET:
        u32SeekToTime = (HI_S32)s64Offset;
        break;
    case SEEK_CUR:
        u32SeekToTime = u32CurFrmTime + (HI_S32)s64Offset;
        break;
    case SEEK_END:
        u32SeekToTime = u32EndFrmTime + (HI_S32)s64Offset;
        break;
    default:
        return HI_ERR_PVR_INVALID_PARA;
    }

    if ((HI_S32)u32SeekToTime > (HI_S32)u32EndFrmTime) /* over the end, set it the end */
    {
        u32SeekToTime = u32EndFrmTime;
    }
    else if ((HI_S32)u32SeekToTime < (HI_S32)u32StartFrmTime) /* less the start, set it the start */
    {
        u32SeekToTime = u32StartFrmTime;
    }

    HI_WARN_PVR("seek to time: %u.  whence:%s, offset:%lld, start:%d, end:%d, cur:%d\n",
                 u32SeekToTime, WHENCE_STRING(whence), s64Offset, u32StartFrmTime, u32EndFrmTime, u32CurFrmTime);

    return PVR_Index_SeekToTime(handle, u32SeekToTime);
}

/*****************************************************************************
 Prototype       : PVR_Index_SeekToStart
 Description     : move the read pointer of  index to the start frame. if recording, move it direction backward more 20 frames.
 Input           : handle         **
                   pFrame         **
                   pDisplayTimes  **
 Output          : None
 Return Value    :
  History
  1.Date         : 2010/09/21
    Author       : j40671
    Modification : Created function

*****************************************************************************/
HI_S32 PVR_Index_SeekToStart(PVR_INDEX_HANDLE handle)
{
    HI_U32 u32BeforeSeek __attribute__((unused)) ;

    HI_ASSERT_RET(handle != NULL);

    PVR_INDEX_LOCK(&(handle->stMutex));
    handle->u32ReadFrame = handle->stCycMgr.u32StartFrame;
    handle->bRecReachPlay = HI_FALSE;
    u32BeforeSeek = handle->u32ReadFrame;

    if ((handle->bIsRec) && (handle->stCycMgr.u32StartFrame >= handle->stCycMgr.u32EndFrame))
    {
        PVRIndexCycMoveReadFrame(handle, PVR_TPLAY_MIN_DISTANCE);
    }

    HI_WARN_PVR("Seek to start, %u --> %u s/e/l %d/%d/%d\n", 
                 u32BeforeSeek, handle->u32ReadFrame, handle->stCycMgr.u32StartFrame, 
                 handle->stCycMgr.u32EndFrame, handle->stCycMgr.u32LastFrame);

    PVR_INDEX_UNLOCK(&(handle->stMutex));

    return HI_SUCCESS;
}



/*****************************************************************************
 Prototype       : PVR_Index_SeekToPauseOrStart
 Description     : on starting to play, seek the read pointer of index to the marked pause frame or start frame.
                        if exist paused frame, seek it to that.
                        if rewritten the pause frame, seek it to the start frame
 Input           : handle         **
                   pFrame         **
                   pDisplayTimes  **
 Output          : None
 Return Value    :
  History
  1.Date         : 2010/06/02
    Author       : j40671
    Modification : Created function

*****************************************************************************/
HI_S32 PVR_Index_SeekToPauseOrStart(PVR_INDEX_HANDLE handle)
{
    HI_S32 ret = HI_SUCCESS;
    PVR_INDEX_ENTRY_NEW_S stStartEntry;
    HI_U32 u32StartFrameNum;
    HI_U32 u32SeekToPos;

    HI_ASSERT_RET(handle != NULL);

    memset(&stStartEntry, 0x00, sizeof(stStartEntry));

    PVR_INDEX_LOCK(&(handle->stMutex));

    /* have been paused, play from pause postion, if the positiong haven't been rewritten by rewind file*/
    if (handle->u64PauseOffset != PVR_INDEX_PAUSE_INVALID_OFFSET)
    {
        HI_ASSERT(handle->u64PauseOffset <= handle->u64GlobalOffset);

        /* straight recording, the start should move direction to the backward, hold some frame(about 20 frames), which prevent  from catching up the live */
        u32StartFrameNum = PVRIndexCalcNewPos(handle, handle->stCycMgr.u32StartFrame, 20);

        PVRINDEX_GET_ENTRY_BYNUM(handle, &stStartEntry, u32StartFrameNum, ret);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_PVR("Can't get StartFrame:%d\n", handle->stCycMgr.u32StartFrame);
            PVR_INDEX_UNLOCK(&(handle->stMutex));
            return ret;
        }
        
        /* check whether pause postion is rewritten or not, if the offset of start frame more than pause frame, it implies rewrite the pause frame */
        if (PVRINDEX_GET_GLOBALOFFSET(handle->u32EntryVersion, &stStartEntry) > handle->u64PauseOffset)
        {
            /* on rewriting, seek it to the start frame */
            u32SeekToPos = u32StartFrameNum;
            HI_INFO_PVR("Pause frame was covered, so seek to start frame:%u\n", u32SeekToPos);
        }
        else
        {
            /* case, not rewrite pause frame, seek to pause frame */
            u32SeekToPos = handle->u32PauseFrame;
            HI_INFO_PVR("Seek to pause frame:%u\n", u32SeekToPos);
        }

        handle->u32ReadFrame = u32SeekToPos;
        handle->u64PauseOffset = PVR_INDEX_PAUSE_INVALID_OFFSET;
        handle->u32PauseFrame = 0;
    }

    PVR_INDEX_UNLOCK(&(handle->stMutex));
    return HI_SUCCESS;
}

HI_S32 PVR_Index_GetRecStatus(PVR_INDEX_HANDLE handle, HI_UNF_PVR_REC_STATUS_S *pstRecStatus)
{
    HI_U32 u32StartFrm = 0;
    HI_U32 u32EndFrm = 0;
    HI_U32 u32LastFrm = 0;
    HI_U32 u32StartDisplayTimeMs = 0;
    HI_U32 u32EndDisplayTimeMs = 0;
    
    HI_ASSERT_RET(handle != NULL);
    HI_ASSERT_RET(pstRecStatus != NULL);

     if ((-1 == handle->stCycMgr.u32EndFrame) || 
         (-1 == handle->stCycMgr.u32LastFrame))
    {
        pstRecStatus->u32CurWriteFrame = 0;
        pstRecStatus->u32CurTimeInMs = 0;
        pstRecStatus->u32StartTimeInMs = 0;
        pstRecStatus->u32EndTimeInMs = 0;
        pstRecStatus->u64CurWritePos = 0;

        return HI_SUCCESS;
    }

    u32StartFrm = handle->stCycMgr.u32StartFrame;
    u32LastFrm  = handle->stCycMgr.u32LastFrame;
    u32EndFrm   = handle->stCycMgr.u32EndFrame;

    if (u32StartFrm >= u32EndFrm)
    {
        pstRecStatus->u32CurWriteFrame = u32StartFrm - u32EndFrm;
    }
    else
    {
        pstRecStatus->u32CurWriteFrame = u32LastFrm - u32EndFrm + u32StartFrm;
    }
    u32StartDisplayTimeMs = PVRINDEX_GET_DISPTIME(handle->u32EntryVersion, &(handle->stStartFrame));
    u32EndDisplayTimeMs = PVRINDEX_GET_DISPTIME(handle->u32EntryVersion, &(handle->stEndFrame));
    if (handle->stCycMgr.u32CycTimes > 0)
    {
        if (handle->stCycMgr.u32MaxCycTimeInMs != 0)
            pstRecStatus->u32CurTimeInMs = handle->stCycMgr.u32MaxCycTimeInMs;
        else
            pstRecStatus->u32CurTimeInMs = u32EndDisplayTimeMs - u32StartDisplayTimeMs;
    }
    else
    {
        pstRecStatus->u32CurTimeInMs = handle->u32TotalRecTimeMs;
    }
    pstRecStatus->u32EndTimeInMs = u32EndDisplayTimeMs;
    pstRecStatus->u32StartTimeInMs = u32StartDisplayTimeMs;
    
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : PVR_Index_GetNextFrame
 Description     : get next frame for decode
 Input           : handle         **
                   pFrame         **
                   pDisplayTimes  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/28
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 PVR_Index_GetNextFrame(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pstFrame)
{
    HI_S32 ret;

    HI_ASSERT_RET(handle != NULL);
    HI_ASSERT_RET(pstFrame != NULL);

    PVR_INDEX_LOCK(&(handle->stMutex));

    ret = PVRIndexGetNextEntry(handle, pstFrame);
    if (HI_SUCCESS != ret)
    {
        HI_INFO_PVR("get next entry error, file end.\n");
        PVR_INDEX_UNLOCK(&(handle->stMutex));
        return ret;
    }

    PVR_INDEX_UNLOCK(&(handle->stMutex));
    return HI_SUCCESS;
}

HI_S32 PVR_Index_GetPreFrame(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pFrame)
{
    HI_S32 ret;

    HI_ASSERT_RET(handle != NULL);
    HI_ASSERT_RET(pFrame != NULL);

    PVR_INDEX_LOCK(&(handle->stMutex));

    ret = PVRIndexGetPreEntry(handle, pFrame);
    if (HI_SUCCESS != ret)
    {
        HI_INFO_PVR("get pre entry error, file end.\n");
        PVR_INDEX_UNLOCK(&(handle->stMutex));
        return ret;
    }

    PVR_INDEX_UNLOCK(&(handle->stMutex));
    return HI_SUCCESS;
}




/*****************************************************************************
 Prototype       : PVR_Index_GetFrameByNum
 Description     : get the index by the num. but not move the read pointer
 Input           : handle         **
                   pFrame         **
                   num  **
 Output          : None
 Return Value    :
  History
  1.Date         : 2010/06/02
    Author       : j40671
    Modification : Created function
*****************************************************************************/

HI_S32 PVR_Index_GetFrameByNum(const PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry, HI_U32 num)
{
    HI_S32 ret;
    HI_ASSERT_RET(NULL != handle);
    HI_ASSERT_RET(NULL != pEntry);

    PVR_INDEX_LOCK(&(handle->stMutex));
    PVRINDEX_GET_ENTRY_BYNUM(handle,pEntry,num, ret);

    PVR_INDEX_UNLOCK(&(handle->stMutex));
    return ret;
}

HI_S32 PVR_Index_QueryFrameByTime(const PVR_INDEX_HANDLE handle, HI_U32 u32SearchTime, PVR_INDEX_ENTRY_NEW_S *pstEntry, HI_U32 *pu32Pos)
{
    HI_U32 u32PtsPos;
    HI_S32 ret;

    HI_ASSERT_RET(NULL != handle);
    HI_ASSERT_RET(NULL != pstEntry);

    PVR_INDEX_LOCK(&(handle->stMutex));

    u32PtsPos = PVRIndexFindFrameByTime(handle, u32SearchTime);
    *pu32Pos =  u32PtsPos;
    HI_WARN_PVR("search Time:%u, Pos:%u\n", u32SearchTime, u32PtsPos);

    PVRINDEX_GET_ENTRY_BYNUM(handle, pstEntry, u32PtsPos, ret);
    if (ret != HI_SUCCESS)
    {
        if (HI_ERR_PVR_FILE_TILL_END == ret)
        {
            PVRINDEX_GET_ENTRY_BYNUM(handle, pstEntry, --u32PtsPos, ret);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_PVR("Can't get Frame:%d\n", u32PtsPos);
                PVR_INDEX_UNLOCK(&(handle->stMutex));
                return HI_ERR_PVR_FILE_TILL_END;
            }
        }
    }
    HI_WARN_PVR("Pos:%u, time:%u\n", u32PtsPos, PVRINDEX_GET_DISPTIME(handle->u32EntryVersion, pstEntry));

    PVR_INDEX_UNLOCK(&(handle->stMutex));

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : PVR_Index_MarkPausePos
 Description     : mark a flag for timeshift, where flag current record position. if start timeshift, play it from this position
 Input           : handle         **
 Output          : None
 Return Value    :
  History
  1.Date         : 2010/06/02
    Author       : j40671
    Modification : Created function
*****************************************************************************/
HI_S32 PVR_Index_MarkPausePos(PVR_INDEX_HANDLE handle)
{
    /* save the frame number of current recording frame*/

#ifdef HI_PVR_EXTRA_BUF_SUPPORT
    handle->u32PauseFrame = handle->stCycMgr.u32EndFrame;
#else
    handle->u32PauseFrame = handle->u32WriteFrame;
#endif

    /* save the absolute offset for the current frame, used for checking whether the pause position rewrote or not by rewind, on playing */
    handle->u64PauseOffset = handle->u64GlobalOffset;

    HI_WARN_PVR("<<==PVR_Index_MarkPausePos: frame=%d, global offset=%lld.\n",
                handle->u32PauseFrame, handle->u64PauseOffset);
    return HI_SUCCESS;

}

HI_VOID PVR_Index_GetIdxFileName(HI_CHAR* pIdxFileName, HI_CHAR* pSrcFileName)
{
    HI_CHAR* pSearch = NULL;
    HI_CHAR* pAppend = NULL;

    pSearch = strstr(pSrcFileName, ".idx");
    while(NULL != pSearch)
    {
        if (NULL != pSearch)
        {
            pAppend = pSearch;
            pSearch = strstr(pSearch + 1, ".idx");
        }
    }

    /* make sure it end with the .idx */
    if (NULL != pAppend && *(pAppend + 4) == 0)
    {
        strncpy(pIdxFileName, pSrcFileName,strlen(pSrcFileName)+1);
    }
    else
    {
        snprintf(pIdxFileName, PVR_MAX_FILENAME_LEN,"%s.idx", pSrcFileName);
    }
    return;
}


/*****************************************************************************
 Prototype       : PVR_Index_PlayGetFileAttrByFileName
 Description     : get the current state of the record file by file name. and can't get it without including index file.
 Input           : handle         **
 Output          : None
 Return Value    :
  History
  1.Date         : 2010/06/02
    Author       : j40671
    Modification : Created function
*****************************************************************************/
HI_S32 PVR_Index_PlayGetFileAttrByFileName(const HI_CHAR *pFileName, PVR_INDEX_HANDLE handle, HI_UNF_PVR_FILE_ATTR_S *pAttr)
{
    HI_CHAR           szIndexName[PVR_MAX_FILENAME_LEN];
    HI_S32            fdIdx;
    HI_U32            u32EntryVersion;
    
    PVR_INDEX_ENTRY_NEW_S stStartEntry;
    PVR_INDEX_ENTRY_NEW_S stEndEntry;
    PVR_IDX_HEADER_INFO_S stIdxHeaderInfo;

    memset(szIndexName, 0x00, PVR_MAX_FILENAME_LEN * sizeof(HI_CHAR));
    memset(&stStartEntry, 0x00, sizeof(stStartEntry));
    memset(&stEndEntry, 0x00, sizeof(stEndEntry));
    memset(&stIdxHeaderInfo, 0x00, sizeof(stIdxHeaderInfo));
        
    if ((!pFileName) || (!pAttr))
    {
        return HI_ERR_PVR_NUL_PTR;
    }

    memset(pAttr, 0x0, sizeof(HI_UNF_PVR_FILE_ATTR_S));

    if (HI_NULL != handle)
    {
        u32EntryVersion = handle->u32EntryVersion;
        if (handle->stCycMgr.u32LastFrame == -1)
        {
            pAttr->enIdxType = HI_UNF_PVR_REC_INDEX_TYPE_NONE;
            pAttr->u32StartTimeInMs = 0;
            pAttr->u32EndTimeInMs = 0;
            pAttr->u32FrameNum = 0;
            pAttr->u64ValidSizeInByte = 0;
            return HI_SUCCESS;
        }

        PVR_INDEX_LOCK(&(handle->stMutex));
        if (handle->stCycMgr.u32EndFrame > handle->stCycMgr.u32StartFrame)
        {
            pAttr->u32FrameNum = handle->stCycMgr.u32EndFrame
                                 - handle->stCycMgr.u32StartFrame;
        }
        else
        {
            pAttr->u32FrameNum = handle->stCycMgr.u32LastFrame
                                 - handle->stCycMgr.u32StartFrame
                                 + handle->stCycMgr.u32EndFrame;
        }
        pAttr->u32StartTimeInMs = PVRINDEX_GET_DISPTIME(u32EntryVersion, &(handle->stStartFrame));
        pAttr->u32EndTimeInMs   = PVRINDEX_GET_DISPTIME(u32EntryVersion, &(handle->stEndFrame));

        if (handle->stCycMgr.u32CycTimes > 0)
        {
            pAttr->u64ValidSizeInByte = handle->stCycMgr.u64MaxCycSize;
        }
        else
        {
            pAttr->u64ValidSizeInByte = PVRINDEX_GET_OFFSET(u32EntryVersion, &(handle->stEndFrame))
                                        + PVRINDEX_GET_FRAMESIZE(u32EntryVersion, &(handle->stEndFrame));
        }
        pAttr->enIdxType = (HI_UNF_PVR_REC_INDEX_TYPE_E)PVRINDEX_GET_INDEXTYPE(u32EntryVersion, &(handle->stStartFrame));
        PVR_INDEX_UNLOCK(&(handle->stMutex));
    }
    else
    {
        HI_S32 s32Ret = HI_SUCCESS;
        PVR_Index_GetIdxFileName(szIndexName, (HI_CHAR*)pFileName);
        fdIdx = PVR_OPEN(szIndexName, PVR_FOPEN_MODE_DATA_READ);
        if (fdIdx < 0)
        {
            HI_ERR_PVR("can not open index file:%s\n", szIndexName);
            return HI_ERR_PVR_FILE_CANT_OPEN;
        }

        if (HI_SUCCESS != PVRIndexGetHeaderInfo(fdIdx, &stIdxHeaderInfo))
        {
            HI_ERR_PVR("No Header Info in index File:%s\n", szIndexName);
            PVR_CLOSE(fdIdx);
            return HI_ERR_PVR_INDEX_FORMAT_ERR;
        }

        if ((-1 == stIdxHeaderInfo.stCycInfo.u32EndFrame) || 
            (-1 == stIdxHeaderInfo.stCycInfo.u32LastFrame))
        {
            HI_WARN_PVR("No frame in index File:%s\n", szIndexName);
            pAttr->u32FrameNum = 0;
            pAttr->u32StartTimeInMs = 0;
            pAttr->u32EndTimeInMs = 0;
            pAttr->u64ValidSizeInByte = 0;
            pAttr->enIdxType = HI_UNF_PVR_REC_INDEX_TYPE_NONE;

            PVR_CLOSE(fdIdx);
            return HI_SUCCESS;
        }
        HI_INFO_PVR("(End, Start, Last) = (%u, %u, %u)\n", 
            stIdxHeaderInfo.stCycInfo.u32EndFrame, 
            stIdxHeaderInfo.stCycInfo.u32StartFrame, 
            stIdxHeaderInfo.stCycInfo.u32LastFrame);
        u32EntryVersion = (stIdxHeaderInfo.u32Reserved & 0x30000) >> 16;
        if (0 == u32EntryVersion)
        {
            s32Ret = PVRIndex_GetFrameInfoByHeaderInfo0(&stIdxHeaderInfo, &stStartEntry, &stEndEntry, fdIdx);
        }
        else
        {
            s32Ret = PVRIndex_GetFrameInfoByHeaderInfo1(&stIdxHeaderInfo, &stStartEntry, &stEndEntry, fdIdx);
        }
        if (HI_SUCCESS != s32Ret)
        {
            PVR_CLOSE(fdIdx);
            return s32Ret;
        }
        if (stIdxHeaderInfo.stCycInfo.u32EndFrame > stIdxHeaderInfo.stCycInfo.u32StartFrame)
        {
            pAttr->u32FrameNum = stIdxHeaderInfo.stCycInfo.u32EndFrame
                                 - stIdxHeaderInfo.stCycInfo.u32StartFrame;
        }
        else
        {
            pAttr->u32FrameNum = stIdxHeaderInfo.stCycInfo.u32LastFrame
                                 - stIdxHeaderInfo.stCycInfo.u32StartFrame
                                 + stIdxHeaderInfo.stCycInfo.u32EndFrame;
        }
        pAttr->u32StartTimeInMs = PVRINDEX_GET_DISPTIME(u32EntryVersion, &stStartEntry);
        pAttr->u32EndTimeInMs = PVRINDEX_GET_DISPTIME(u32EntryVersion, &stEndEntry);
        if (stIdxHeaderInfo.stCycInfo.u32IsRewind)
        {
            pAttr->u64ValidSizeInByte = stIdxHeaderInfo.u64ValidSize;
        }
        else
        {
            pAttr->u64ValidSizeInByte = PVRINDEX_GET_OFFSET(u32EntryVersion, &stEndEntry) 
                                    + PVRINDEX_GET_FRAMESIZE(u32EntryVersion, &stEndEntry);
        }
        pAttr->enIdxType = (HI_UNF_PVR_REC_INDEX_TYPE_E)PVRINDEX_GET_INDEXTYPE(u32EntryVersion, &stStartEntry);

        PVR_CLOSE(fdIdx);
    }

    return HI_SUCCESS;
}




/*get vedio type from index header info   */
HI_S32 PVR_Index_GetVtype(PVR_INDEX_HANDLE handle)
{
    PVR_IDX_HEADER_INFO_S stIdxHeaderInfo;

    memset(&stIdxHeaderInfo, 0, sizeof(PVR_IDX_HEADER_INFO_S));

    /* means not found the header info in this index file */
    if (HI_SUCCESS != PVRIndexGetHeaderInfo(handle->s32HeaderFd, &stIdxHeaderInfo))
    {
        return HI_FAILURE;
    }
    else
        return (stIdxHeaderInfo.u32Reserved & 0xFFFF);
}
HI_S32 PVR_Index_GetVersion(PVR_INDEX_HANDLE handle)
{
    PVR_IDX_HEADER_INFO_S stIdxHeaderInfo;
    memset(&stIdxHeaderInfo, 0, sizeof(PVR_IDX_HEADER_INFO_S));

    /* means not found the header info in this index file */
    if (HI_SUCCESS != PVRIndexGetHeaderInfo(handle->s32HeaderFd, &stIdxHeaderInfo))
    {
        HI_ERR_PVR("Failed to get version of entry!\n");
        return HI_FAILURE;
    }
    else
    {
        HI_INFO_PVR("\t\tstIdxHeaderInfo.u32Reserved = %#x\n", stIdxHeaderInfo.u32Reserved);
        handle->u32EntryVersion = (stIdxHeaderInfo.u32Reserved & 0x30000) >> 16;
        return HI_SUCCESS;
    }
}



HI_VOID PVR_Index_GetFrameRate(PVR_INDEX_HANDLE handle, HI_U32 *pu32FrmRate)
{
    HI_U32 u32TotalFrames = 0;
    HI_U32 u32TotalTimeMs = 0;
    PVR_CYC_MGR_S *pstCycMgr = &handle->stCycMgr;

    PVR_INDEX_LOCK(&handle->stMutex);
    
    u32TotalTimeMs = PVRINDEX_GET_DISPTIME(handle->u32EntryVersion, &handle->stEndFrame)
                    - PVRINDEX_GET_DISPTIME(handle->u32EntryVersion, &handle->stStartFrame);
    
    u32TotalTimeMs = (1000 > u32TotalTimeMs) ? 1000 : u32TotalTimeMs;
    u32TotalFrames = PVR_Index_CalcFrameDistance(pstCycMgr->u32EndFrame, pstCycMgr->u32StartFrame, PVR_INDEX_DIRECTION_FORWARD, pstCycMgr);
    PVR_INDEX_UNLOCK(&handle->stMutex);
    HI_WARN_PVR("(E, S, V, E_t, S_t) = (%u, %u, %u, %u, %u)\n", 
        pstCycMgr->u32EndFrame, pstCycMgr->u32StartFrame, handle->u32EntryVersion,
        handle->stEndFrame.stEntry2.u32DisplayTimeMs, handle->stStartFrame.stEntry2.u32DisplayTimeMs);
    *pu32FrmRate = u32TotalFrames * 10/(u32TotalTimeMs/1000);
    
    if ((*pu32FrmRate) % 10 >= 5)
    {
        *pu32FrmRate = *pu32FrmRate/10 + 1;
    }
    else
    {
        *pu32FrmRate = *pu32FrmRate/10 ;
    }
    
    return ;
}


HI_S32 PVR_Index_GetFBwardIPBFrameNum(PVR_INDEX_HANDLE handle, PVR_INDEX_DIRECTION_E enDirection, HI_U32 u32FrameType, HI_U32 u32CurFrameNum, HI_U32 *pu32NextFrameNum)
{
    return PVRIndexGetFBwardIPBFrameNum(handle, enDirection, u32FrameType, u32CurFrameNum, pu32NextFrameNum);
}
HI_S32 PVR_Index_PushStartWhenTsLead(PVR_INDEX_HANDLE handle, HI_U64 u64CurFileSize)
{
    HI_U32 u32CurFrmNum = 0;
    HI_U32 u32NextFrmNum = 0;
    HI_S32 s32Ret = HI_FAILURE;
    PVR_CYC_MGR_S *pstCycMgr = HI_NULL;
    HI_U64 u64TsLeadLen = 0;
    HI_U64 u64InvalideLen = 0;
    HI_U64 u64StartGlobalOffset = 0;
    HI_U64 u64LastOffset = 0;
    HI_U64 u64NextGlobalOffset = 0;
    HI_U64 u64TempOffset = 0;
    HI_U32 u32LastFrameSize = 0;
    HI_U32 u32TempFrameSize = 0;
    PVR_INDEX_ENTRY_NEW_S stStartEntry;
    PVR_INDEX_ENTRY_NEW_S stLastEntry;
    PVR_INDEX_ENTRY_NEW_S stNextEntry;
    HI_U32 u32Version = 0;
    
    if (handle == HI_NULL)
    {
        HI_ERR_PVR("Invalide param\n");
        return HI_FAILURE;
    }
    
    memset(&stStartEntry, 0x00, sizeof(stStartEntry));
    memset(&stLastEntry, 0x00, sizeof(stLastEntry));
    memset(&stNextEntry, 0x00, sizeof(stNextEntry));
    u32Version = handle->u32EntryVersion;
    pstCycMgr = &handle->stCycMgr;
    u32CurFrmNum = pstCycMgr->u32StartFrame;
    u32NextFrmNum = pstCycMgr->u32StartFrame;
    
    if (handle->stCycMgr.bIsRewind == HI_FALSE)
        return HI_SUCCESS;

    
    if (u64CurFileSize <= PVRINDEX_GET_GLOBALOFFSET(u32Version, &handle->stCurRecFrame))
        return HI_SUCCESS;

    u64TsLeadLen = u64CurFileSize - PVRINDEX_GET_GLOBALOFFSET(u32Version, &handle->stCurRecFrame);

    PVR_INDEX_LOCK(&handle->stMutex);

    if (pstCycMgr->u32CycTimes < 1)
    {
        PVR_INDEX_UNLOCK(&handle->stMutex);
        return HI_SUCCESS;
    }
    
    u64TempOffset = PVRINDEX_GET_OFFSET(u32Version, &(handle->stCurRecFrame));
    u32TempFrameSize = PVRINDEX_GET_FRAMESIZE(u32Version, &(handle->stCurRecFrame));
    if (pstCycMgr->u32StartFrame > pstCycMgr->u32EndFrame)
    {
        u64InvalideLen = pstCycMgr->u64StartFrameOffset - u64TempOffset + u32TempFrameSize;
    }
    else
    {
        PVRINDEX_GET_ENTRY_BYNUM(handle, &stLastEntry, pstCycMgr->u32LastFrame, s32Ret);
        if (s32Ret == HI_SUCCESS)
        {
            u64LastOffset = PVRINDEX_GET_OFFSET(u32Version, &stLastEntry);
            u32LastFrameSize = PVRINDEX_GET_FRAMESIZE(u32Version, &stLastEntry);
            
            u64InvalideLen = (u64LastOffset + u32LastFrameSize - u64TempOffset - u32TempFrameSize) + pstCycMgr->u64StartFrameOffset;
        }
        else
        {
            u64InvalideLen = pstCycMgr->u64StartFrameOffset;
        }
    }

    if (u64InvalideLen >= u64TsLeadLen)
    {
        PVR_INDEX_UNLOCK(&handle->stMutex);
        return HI_SUCCESS;
    }

    PVRINDEX_GET_ENTRY_BYNUM(handle, &stStartEntry, u32CurFrmNum, s32Ret);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("Get start frame %d failed, ret %x\n", u32CurFrmNum, s32Ret);
        PVR_INDEX_UNLOCK(&handle->stMutex);
        return s32Ret;
    }
    u64StartGlobalOffset = PVRINDEX_GET_GLOBALOFFSET(u32Version, &stStartEntry);
    do
    {
        u32CurFrmNum = u32NextFrmNum;
        s32Ret = PVRIndexGetFBwardIPBFrameNum(handle, PVR_INDEX_DIRECTION_FORWARD, PVR_INDEX_FRAME_I,
                                              u32CurFrmNum, &u32NextFrmNum);
        if (s32Ret == HI_SUCCESS)
        {
            
            PVRINDEX_GET_ENTRY_BYNUM(handle, &stNextEntry, u32NextFrmNum, s32Ret);
            u64NextGlobalOffset = PVRINDEX_GET_GLOBALOFFSET(u32Version, &stNextEntry);
            if (s32Ret == HI_SUCCESS)
            {
                if (u64NextGlobalOffset <= u64StartGlobalOffset)
                {
                    HI_FATAL_PVR("Next %d goffset %llx small than start %d goffset %llx\n",
                                 u32NextFrmNum, u64NextGlobalOffset, pstCycMgr->u32StartFrame, u64StartGlobalOffset);
                    PVR_INDEX_UNLOCK(&handle->stMutex);
                    return HI_FAILURE;
                }
            }
            else
            {
                HI_ERR_PVR("Get %d frame failed, ret %x\n", u32NextFrmNum, s32Ret);
                PVR_INDEX_UNLOCK(&handle->stMutex);
                return s32Ret;
            }
        }
        else
        {
            HI_ERR_PVR("Get %d next I frame failed, ret %x!\n", u32CurFrmNum, s32Ret);
            PVR_INDEX_UNLOCK(&handle->stMutex);
            return s32Ret;
        }
    }while((u64InvalideLen + u64NextGlobalOffset - u64StartGlobalOffset) < u64TsLeadLen);

    HI_INFO_PVR("Push start %u to %u, InvalideLen %llx pushlen %llx tsleadlen %llx\n",
                pstCycMgr->u32StartFrame, u32NextFrmNum, u64InvalideLen,
                u64NextGlobalOffset - u64StartGlobalOffset, u64TsLeadLen);
    pstCycMgr->u32StartFrame = u32NextFrmNum;
    pstCycMgr->u64StartFrameOffset = PVRINDEX_GET_OFFSET(u32Version, &stNextEntry);

    (HI_VOID)PVRIndexCheckReadFrmValidWhenTimeshift(handle);
    PVR_INDEX_UNLOCK(&handle->stMutex);

    return HI_SUCCESS;
}    

HI_VOID PVR_Index_ProcOverFlow(PVR_INDEX_HANDLE handle)
{
    HI_U32 u32EntryVersion = handle->u32EntryVersion;
    handle->u64OverflowOffset = PVRINDEX_GET_GLOBALOFFSET(u32EntryVersion,&handle->stCurRecFrame)
                                + PVRINDEX_GET_FRAMESIZE(u32EntryVersion,&handle->stCurRecFrame);
    return;
}


/*
按照  Forward 或者 BackWard 方向，从当前帧号 ，移动 Distance 之后得到的帧号，
注意: 正向移动位置大于 u32LastFrame 或者 u32EndFrame， 则等于 u32LastFrame 或者 u32EndFrame；
反向移动小于 StartFrame ，则等于 StartFrame。
*/
HI_S32 PVR_Index_CycMoveFrmNum(PVR_INDEX_HANDLE handle, HI_BOOL bForward,HI_S32 s32CurFrmNum,HI_U32 Distance, HI_S32 *ps32NextFrameNum)
{
    HI_S32  s32Range = 0;
    HI_S32  s32NextFrameNum = 0;
    HI_BOOL bRewind = HI_FALSE;
    PVR_CYC_MGR_S* pstCycMgr = HI_NULL;

    HI_ASSERT_RET(handle != NULL);
    HI_ASSERT_RET(ps32NextFrameNum != NULL);

    pstCycMgr = &handle->stCycMgr;
    s32Range  = (bForward)?Distance:(0 - (HI_S32)Distance);
    bRewind   = (pstCycMgr->u32StartFrame > pstCycMgr->u32EndFrame)?HI_TRUE:HI_FALSE;

    if (HI_TRUE != PVRIndexIsFrameValid((HI_U32)s32CurFrmNum, pstCycMgr))
    {
        HI_ERR_PVR("current frame number %d is invalid. start=%d end=%d\n",
                    s32CurFrmNum, handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame);
        return PVR_INDEX_ERR_INVALID;
    }

    /*通常只会在一个GOP范围移动帧号，调用此函数的时候如果 移动范围太大，可能是异常，因此本函数只支持 512 范围内移动*/
    if (Distance > 512)
    {
        HI_ERR_PVR("move range > 512, This function not support,D:%d,R:%d\n", Distance, s32Range);
    }
    
    /*通常返回值就等于这句代码，下面的 if-else 都是在处理异常*/
    s32NextFrameNum = s32CurFrmNum + s32Range;

    if (bRewind)
    {
        if (bForward)   //FF 回绕
        {
            //1.  0---c---e---n---s---l     当前帧在前段并且快进到尾部
            if ((s32CurFrmNum <= pstCycMgr->u32EndFrame) && 
                (s32NextFrameNum > pstCycMgr->u32EndFrame))
            {
                s32NextFrameNum = pstCycMgr->u32EndFrame;
                *ps32NextFrameNum = s32NextFrameNum;
                return HI_ERR_PVR_FILE_TILL_END;
            }
            //1.  0---n---e---s---c---l     当前帧在后段且下一帧回绕且快进未到尾部
            //2.  0---e---n---s---c---l     当前帧在后段且下一帧回绕且快进到尾部
            else if ((s32CurFrmNum >= pstCycMgr->u32StartFrame) && 
                     (s32NextFrameNum > pstCycMgr->u32LastFrame))
            {
                
                s32NextFrameNum = s32NextFrameNum - pstCycMgr->u32LastFrame - 1;
                if (s32NextFrameNum > pstCycMgr->u32EndFrame)
                {
                    s32NextFrameNum = pstCycMgr->u32EndFrame;
                    *ps32NextFrameNum = s32NextFrameNum;
                    return HI_ERR_PVR_FILE_TILL_END;
                }
            }
        }
        else    //FB 回绕
        {
            //1.  0---c---e---s---n---l     当前帧在前段且下一帧回绕且快退未到尾部
            //2.  0---c---e---n---s---l     当前帧在前段且下一帧回绕且快退到尾部
            if ((s32CurFrmNum <= pstCycMgr->u32EndFrame) && 
                (s32NextFrameNum < 0))
            {
                s32NextFrameNum = s32NextFrameNum + pstCycMgr->u32LastFrame + 1;
                if (s32NextFrameNum < (HI_S32)pstCycMgr->u32StartFrame)
                {
                    s32NextFrameNum = pstCycMgr->u32StartFrame;
                    *ps32NextFrameNum = s32NextFrameNum;
                    return HI_ERR_PVR_FILE_TILL_START;
                }
            }
            //1.  0---e---n---s---c---l     当前帧在后段且快退到尾部
            else if ((s32CurFrmNum >= pstCycMgr->u32StartFrame) && 
                     (s32NextFrameNum < (HI_S32)pstCycMgr->u32StartFrame))
            {
                s32NextFrameNum = pstCycMgr->u32StartFrame;
                *ps32NextFrameNum = s32NextFrameNum;
                return HI_ERR_PVR_FILE_TILL_START;
            }
        }
    }
    else
    {
        //1.  0---s---c---e---n---l     快进到尾部
        //2.  0---s---c---n---e---l     快进未到尾部
        if (bForward)   //FF非回绕
        {
            if (s32NextFrameNum > (HI_S32)pstCycMgr->u32EndFrame)
            {
                s32NextFrameNum = pstCycMgr->u32EndFrame;
                *ps32NextFrameNum = s32NextFrameNum;
                return HI_ERR_PVR_FILE_TILL_END;
            }
        }
        //1.  0---s---n---c---e---l     快退未到头部
        //2.  0---n---s---c---e---l     快退到头部
        else    //FB非回绕
        {
            if (s32NextFrameNum < (HI_S32)pstCycMgr->u32StartFrame)
            {
                s32NextFrameNum = pstCycMgr->u32StartFrame;
                *ps32NextFrameNum = s32NextFrameNum;
                return HI_ERR_PVR_FILE_TILL_START;
            }
        }
    }

    if (s32NextFrameNum < 0)
    {
        HI_ERR_PVR("Move Frm error, F:%d,c:%d,D:%d,N:%d,S:%d,L:%d,E:%d\n",
                    bForward, s32CurFrmNum, Distance, s32NextFrameNum,
                    pstCycMgr->u32StartFrame, pstCycMgr->u32LastFrame, pstCycMgr->u32EndFrame);
        return PVR_INDEX_ERR_INVALID;
    }

    *ps32NextFrameNum = s32NextFrameNum;

    HI_INFO_PVR("Move , F:%d,c:%d,D:%d,N:%d,S:%d,L:%d,E:%d\n",
                bForward, s32CurFrmNum, Distance, s32NextFrameNum,
                pstCycMgr->u32StartFrame, pstCycMgr->u32LastFrame, pstCycMgr->u32EndFrame);

    return HI_SUCCESS;
}

HI_S32 PVR_Index_GetPreIFrame(PVR_INDEX_HANDLE handle, HI_U32 u32CurFrameNum, HI_U32 *pu32PreIFrameNum, PVR_INDEX_ENTRY_NEW_S *pstPreIFrame)
{
    HI_U32 u32PreFrmNum = 0;
    HI_U32 u32PreIOffset = 0;
    HI_S32 ret = 0;
    PVR_INDEX_ENTRY_NEW_S stIndexEntry;
    HI_U16 u16FrameTypeAndGop = 0;
    
    HI_ASSERT_RET(handle != NULL);
    HI_ASSERT_RET(pu32PreIFrameNum != NULL);

    memset(&stIndexEntry, 0x00, sizeof(stIndexEntry));
    if (HI_TRUE != PVRIndexIsFrameValid(u32CurFrameNum, &handle->stCycMgr))
    {
        HI_ERR_PVR("current frame number %d is invalid. start=%d end=%d\n",
                    u32CurFrameNum, handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame);
        return PVR_INDEX_ERR_INVALID;
    }

    ret = PVR_Index_CycMoveFrmNum(handle, HI_FALSE, (HI_S32)u32CurFrameNum, 1, (HI_S32 *)&u32PreFrmNum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("move failed, ret = %#x\n", ret);
        return ret;
    }

    ret = PVR_Index_GetFrameByNum(handle, &stIndexEntry, u32PreFrmNum);
    if (HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    u32PreIOffset = PVRINDEX_GET_PREIOFFSET(handle->u32EntryVersion, &stIndexEntry);
    ret = PVR_Index_CycMoveFrmNum(handle, HI_FALSE, (HI_S32)u32PreFrmNum, u32PreIOffset, (HI_S32*)pu32PreIFrameNum);
    if ((ret == PVR_INDEX_ERR_INVALID) || (HI_ERR_PVR_FILE_TILL_START == ret))
    {
        HI_ERR_PVR("search pre I frame till start ,can not get, ret = %#x\n", ret);
        return ret;
    }

    /*check it is I or IDR*/
    (HI_VOID)PVR_Index_GetFrameByNum(handle, &stIndexEntry, *pu32PreIFrameNum);
    u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, &stIndexEntry);
    if (!(PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_I) 
          || PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR)))
    {
        HI_WARN_PVR(" c:%d, PI:%d, but not I\n", u32CurFrameNum, *pu32PreIFrameNum);
        return HI_FAILURE;
    }

    memcpy(pstPreIFrame, &stIndexEntry, sizeof(PVR_INDEX_ENTRY_NEW_S));

    HI_INFO_PVR("GetPI: C:%d,PI:%d\n",u32CurFrameNum,*pu32PreIFrameNum);

    return ret;
}

HI_S32 PVR_Index_GetNextIFrame(PVR_INDEX_HANDLE handle, HI_U32 u32CurFrameNum, HI_U32 *pu32NextIFrameNum, PVR_INDEX_ENTRY_NEW_S* pstNextIFrame)
{
    HI_U32 u32Step = 0;
    HI_U32 u32Count = 0;
    HI_U32 u32PreIOffset = 0;
    HI_U32 u32PreIFrameNum = 0;
    HI_S32 s32NextIFrameNum = 0;
    HI_S32 ret=0;
    HI_S32 s32TmpStartFrm = 0;/*每一次搜寻帧的开始帧号*/
    HI_S32 s32TmpEndFrameNum = 0;/*每一次搜寻帧的结果帧号*/
    HI_U16 u16FrameTypeAndGop = 0;
    PVR_INDEX_ENTRY_NEW_S stIndexEntry;
    PVR_INDEX_ENTRY_NEW_S stPreIFrame;
    PVR_INDEX_INFO_S  *pstIdxInfo = &handle->stRecIdxInfo.stIdxInfo;

    HI_ASSERT_RET(handle != NULL);
    HI_ASSERT_RET(pu32NextIFrameNum != NULL);

    memset(&stIndexEntry, 0x00, sizeof(stIndexEntry));
    memset(&stPreIFrame, 0x00, sizeof(stPreIFrame));
    if (HI_TRUE != PVRIndexIsFrameValid(u32CurFrameNum, &handle->stCycMgr))
    {
        HI_ERR_PVR("current frame number %d is invalid. start=%d end=%d\n",
                     u32CurFrameNum, handle->stCycMgr.u32StartFrame, handle->stCycMgr.u32EndFrame);
        return PVR_INDEX_ERR_INVALID;
    }

    (HI_VOID)PVR_Index_GetFrameByNum(handle, &stIndexEntry, u32CurFrameNum);
    u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, &stIndexEntry);
    if (PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_I) 
        || PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR))
    {
        ret = PVRIndexGetFBwardIPBFrameNum(handle, PVR_INDEX_DIRECTION_FORWARD, 
                                           PVR_INDEX_FRAME_I, u32CurFrameNum, pu32NextIFrameNum);
        if (HI_SUCCESS == ret)
        {
            ret  = PVR_Index_GetFrameByNum(handle, &stIndexEntry, *pu32NextIFrameNum);
            if (HI_SUCCESS == ret)
            {
                memcpy(pstNextIFrame, &stIndexEntry, sizeof(PVR_INDEX_ENTRY_NEW_S));
                HI_INFO_PVR("GetNextI finish: Cur:%d,NextI:%d,ret:0x%x\n",
                             u32CurFrameNum, *pu32NextIFrameNum, ret);
                return HI_SUCCESS;
            }
            HI_ERR_PVR("Get frame by num failed! ret %x\n", ret);
        }
        return ret;
    }

    ret = PVR_Index_GetPreIFrame(handle, u32CurFrameNum, &u32PreIFrameNum, &stPreIFrame);
    if (HI_SUCCESS != ret)
    {
        ret = PVRIndexGetFBwardIPBFrameNum(handle, PVR_INDEX_DIRECTION_FORWARD, PVR_INDEX_FRAME_I, u32CurFrameNum, pu32NextIFrameNum);
        if (HI_SUCCESS == ret)
        {
            ret = PVR_Index_GetFrameByNum(handle, &stIndexEntry, *pu32NextIFrameNum);
            if (HI_SUCCESS == ret)
            {
                memcpy(pstNextIFrame,&stIndexEntry,sizeof(PVR_INDEX_ENTRY_NEW_S));
                HI_INFO_PVR("GetNextI finish: Cur:%d,NextI:%d,ret:0x%x\n", u32CurFrameNum, *pu32NextIFrameNum, ret);
                return HI_SUCCESS;
            }
            HI_ERR_PVR("Get frame by num failed! ret %x\n", ret);
        }
        return ret;
    }

    /*1. 计算步进值*/
    if (((HI_S32)pstIdxInfo->u32GopTotalNum > 0) && ((HI_S32)pstIdxInfo->u32FrameTotalNum > 0))
    {
        u32Step = pstIdxInfo->u32FrameTotalNum / pstIdxInfo->u32GopTotalNum;
    }
    else
    {
        HI_WARN_PVR("Recorded Gop Num:%d, Frm Num:%d may be error\n",
                     pstIdxInfo->u32GopTotalNum, pstIdxInfo->u32FrameTotalNum);
        u32Step = 50;
    }

    /*2. 找到某一个 TmpFrameNum ， 是 u32CurFrameNum 后面某一个GOP中的一帧，
    这个 do-while 通常循环一两次既可找到这样一个帧，因此虽然代码显得复杂，但是 读文件较少，IO操作较少*/ 
    s32TmpStartFrm = u32CurFrameNum;
    do
    {
        u32Count++;
        ret = PVR_Index_CycMoveFrmNum(handle, HI_TRUE, s32TmpStartFrm, u32Step, &s32TmpEndFrameNum); 
        ret |= PVR_Index_GetFrameByNum(handle, &stIndexEntry, s32TmpEndFrameNum);
        u32PreIOffset = PVRINDEX_GET_PREIOFFSET(handle->u32EntryVersion, &stIndexEntry);
        ret |= PVR_Index_CycMoveFrmNum(handle, HI_FALSE, s32TmpEndFrameNum, u32PreIOffset, &s32NextIFrameNum); 

        HI_INFO_PVR("Serach a Next GopFrm: S:%d,E:%d,Step:%d,NextI:%d,ret:0x%x\n",
                     s32TmpStartFrm, s32TmpEndFrameNum, u32Step, s32NextIFrameNum, ret);

        s32TmpStartFrm = s32TmpEndFrameNum;
        if ((PVR_INDEX_ERR_INVALID == ret) || (HI_ERR_PVR_FILE_TILL_END == ret))
        {
            HI_ERR_PVR("serach next I frame till end ,can not get,error, ret = %#x\n", ret);
            return ret;
        }
        if (u32Count > 256)
        {
            HI_ERR_PVR("search frame of next gop more than 256 times, something must wrong!Cur:%d", u32CurFrameNum);
            u32Count = 0;
            break;
        }
        if (u32PreIOffset == 0)
        {
            u32Count = 0;
            break;
        }
    } while (u32PreIFrameNum == (HI_U32)s32NextIFrameNum);
    HI_INFO_PVR("(u32Count,ret) = (%u, %#x)!\n", u32Count,ret); 
    /*3. 如果经过以上 do-while 找到的 TmpFrameNum 是这种情况 :
    ........u32CurFrameNum......I.......I.........I....       ........TmpFrameNum....I，
    需要继续反向寻找一个 与 u32CurFrameNum 相邻的 GOP 的I帧*/
    s32TmpStartFrm = s32TmpEndFrameNum;
    u32Count = 0;

    while (u32PreIFrameNum != (HI_U32)s32NextIFrameNum)
    {
        u32Count++;
        s32TmpEndFrameNum = s32NextIFrameNum;
        ret = PVR_Index_GetPreIFrame(handle, s32TmpStartFrm, (HI_U32*)&s32NextIFrameNum, &stIndexEntry); 
        ret |= PVR_Index_CycMoveFrmNum(handle, HI_FALSE, s32NextIFrameNum, 1, &s32TmpStartFrm); 
        if (HI_SUCCESS != ret)
        {
            u32Count = 0;
            break;
        }
        if (u32Count > 256)
        {
            HI_ERR_PVR("search next I frame more than 256 times, something must wrong!Cur:%d",u32CurFrameNum);
            u32Count = 0;
            break;
        }
        
    }
    HI_INFO_PVR("(u32Count,ret) = (%u, %#x)!\n", u32Count,ret); 
    s32NextIFrameNum = s32TmpEndFrameNum;

    /*4. check it is I*/
    (HI_VOID)PVR_Index_GetFrameByNum(handle, &stIndexEntry, (HI_U32)s32NextIFrameNum);
    u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, &stIndexEntry);
    if (PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_I) 
        || PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion,u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR))
    {
        *pu32NextIFrameNum = s32NextIFrameNum;
         memcpy(pstNextIFrame, &stIndexEntry, sizeof(PVR_INDEX_ENTRY_NEW_S));
    }
    else
    {
        HI_ERR_PVR("Get Next I Frame failed, Cur:%d\n", u32CurFrameNum);
        return PVR_INDEX_ERR_INVALID;
    }

    HI_INFO_PVR("GetNextI finish: Cur:%d,NextI:%d,ret:0x%x\n", u32CurFrameNum, *pu32NextIFrameNum, ret);
    return HI_SUCCESS;
}

/* get frame number, frame attribute, GOP attribute from one frame plus N forward*/
HI_S32 PVR_Index_GetForwardGOPAttr(PVR_INDEX_HANDLE piIndexHandle, 
                                   HI_PVR_FETCH_RESULT_S *pstFetchRes, 
                                   HI_U32 u32StartFrameNum, 
                                   HI_U32 u32FrameNum)
{
    HI_U32 i = 0, u32ReadFrameEnd = 0, u32FrmNum = 0;
    HI_S32 ret = 0;
    HI_U16 u16FrameTypeAndGop = 0;
    PVR_INDEX_ENTRY_NEW_S *pstFrame = HI_NULL; 

    HI_ASSERT_RET(NULL != piIndexHandle);
    HI_ASSERT_RET(NULL != pstFetchRes);

    if (HI_TRUE != PVRIndexIsFrameValid(u32StartFrameNum, &piIndexHandle->stCycMgr))
    {
        HI_ERR_PVR("Input frame number %d is invalid. S/E/L %d/%d/%d\n", 
                    u32StartFrameNum, 
                    piIndexHandle->stCycMgr.u32StartFrame,
                    piIndexHandle->stCycMgr.u32EndFrame,
                    piIndexHandle->stCycMgr.u32LastFrame);
        return HI_FAILURE;
    }

    memset(pstFetchRes, 0, sizeof(HI_PVR_FETCH_RESULT_S));
    u32ReadFrameEnd = u32StartFrameNum + u32FrameNum;

    for (i = u32StartFrameNum; i < u32ReadFrameEnd; i++)
    { 
        pstFrame = &pstFetchRes->stFrame[u32FrmNum].stIndexEntry;
        if (i > piIndexHandle->stCycMgr.u32LastFrame)
        {
            if (piIndexHandle->stCycMgr.u32EndFrame <= piIndexHandle->stCycMgr.u32StartFrame)
            {
                i = 0;
                u32ReadFrameEnd = u32FrameNum - (piIndexHandle->stCycMgr.u32LastFrame - u32StartFrameNum + 1) - 1;
                u32ReadFrameEnd = (u32ReadFrameEnd >= piIndexHandle->stCycMgr.u32EndFrame) ? piIndexHandle->stCycMgr.u32EndFrame : u32ReadFrameEnd;
            }
            else
                break;
        }
        
        if (HI_TRUE != PVRIndexIsFrameValid(i, &piIndexHandle->stCycMgr))
        {
            return HI_SUCCESS;
        }

        ret = PVR_Index_GetFrameByNum(piIndexHandle, pstFrame, i);
        if(HI_SUCCESS != ret)
        {
             if(ret == HI_ERR_PVR_FILE_TILL_END)
             {
                return HI_SUCCESS;
             }
             else
             {
                HI_WARN_PVR("get the %d entry fail.\n", i);
                return HI_FAILURE;
             }
        }

        pstFetchRes->stFrame[u32FrmNum].u32FrameNum = i;
        
        pstFetchRes->stFrame[u32FrmNum].u32FrameType = 
            PVR_INDEX_GET_FRAMETYPE(piIndexHandle->u32EntryVersion, PVRINDEX_GET_FRAMETYPEANDGOP(piIndexHandle->u32EntryVersion, pstFrame));

        u32FrmNum++;
        pstFetchRes->u32TotalFrameNum = u32FrmNum;
        u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(piIndexHandle->u32EntryVersion, pstFrame);
        if (PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_I) 
            || PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR))
        {
            pstFetchRes->u32IFrameNum++;
            pstFetchRes->u32GopNum++;
        }
        else if (PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_B))
        {
            pstFetchRes->u32BFrameNum++;
        }
        else if (PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_P))
        {
            pstFetchRes->u32PFrameNum++;
        }
    }

    return HI_SUCCESS;
}

/* get frame number, frame attribute, GOP attribute from one frame plus N backward*/
HI_S32 PVR_Index_GetBackwardGOPAttr(PVR_INDEX_HANDLE piIndexHandle, 
                                    HI_PVR_FETCH_RESULT_S *pstFetchRes, 
                                    HI_U32 u32StartFrameNum, 
                                    HI_U32 u32FrameCount)
{
    HI_U32 i, u32GopFlag = 0, u32FrmCount = 0, u32FrmNum = u32StartFrameNum;
    HI_S32 ret = HI_SUCCESS;
    PVR_INDEX_ENTRY_NEW_S       *pstEntry = HI_NULL;
    PVR_CYC_MGR_S           *pstCycMgr = HI_NULL;
    HI_PVR_FETCH_GOP_ATTR_S *pstFetchGopAttr = HI_NULL;
    HI_PVR_FETCH_FRAME_S    *pGopReverseFrm, *pTmpFetchFrame = HI_NULL;
    HI_U16 u16FrameTypeAndGop = 0;

    HI_ASSERT_RET(NULL != piIndexHandle);
    HI_ASSERT_RET(NULL != pstFetchRes);
    pstCycMgr = &(piIndexHandle->stCycMgr);

    if (HI_TRUE != PVRIndexIsFrameValid(u32StartFrameNum, pstCycMgr))
    {
        HI_ERR_PVR("Input frame number %d is invalid. S/E/L %d/%d/%d\n", 
                    u32StartFrameNum, 
                    piIndexHandle->stCycMgr.u32StartFrame,
                    piIndexHandle->stCycMgr.u32EndFrame,
                    piIndexHandle->stCycMgr.u32LastFrame);
        return HI_FAILURE;
    }

    pGopReverseFrm = (HI_PVR_FETCH_FRAME_S *)HI_MALLOC(HI_ID_PVR, sizeof(HI_PVR_FETCH_FRAME_S) * MAX_FRAME_NUM_ONCE_FETCH);
    HI_ASSERT_RET(NULL != pGopReverseFrm);

    memset(pGopReverseFrm, 0, sizeof(HI_PVR_FETCH_FRAME_S) * MAX_FRAME_NUM_ONCE_FETCH);
    memset(pstFetchRes, 0, sizeof(HI_PVR_FETCH_RESULT_S));
    pTmpFetchFrame = pGopReverseFrm;
    pstFetchGopAttr = &(pstFetchRes->stGop[0]);

    for (i = u32FrameCount; i > 0; i--)
    {
        pstEntry = &(pTmpFetchFrame->stIndexEntry);

        if (HI_TRUE != PVRIndexIsFrameValid(u32FrmNum, pstCycMgr))
        {
            ret = HI_FAILURE;
            HI_ERR_PVR("Readframe %d is invalide\n", u32FrmNum);
            break;
        }
        PVRINDEX_GET_ENTRY_BYNUM(piIndexHandle, pstEntry, u32FrmNum, ret);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_PVR("Get the %d entry fail! ret %x\n", u32FrmNum, ret);
            break;
        }
        u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(piIndexHandle->u32EntryVersion, pstEntry);
        pTmpFetchFrame->u32FrameNum = u32FrmNum;
        pTmpFetchFrame->u32FrameType = PVR_INDEX_GET_FRAMETYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop);

        u32FrmCount++;
        pstFetchGopAttr->u32TotalFrameNum = u32FrmCount;   //add frm num of this GOP
        pstFetchRes->u32TotalFrameNum++;                   //add frm num of fetch result

        if (PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_I) 
            || PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR))
        {
            u32GopFlag = 1;
            pstFetchRes->u32IFrameNum++;
        }
        else if (PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_B))
        {
            pstFetchRes->u32BFrameNum++;
            pstFetchGopAttr->u32BFrameNum++;
        }
        else if (PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_P))
        {
            pstFetchRes->u32PFrameNum++;
            pstFetchGopAttr->u32PFrameNum++;
        }

        if (1 == u32GopFlag)
        {
            HI_S32 j = (HI_S32)pstFetchGopAttr->u32TotalFrameNum - 1;
            for (; j >= 0 ; j--)
            {
                memcpy(&pstFetchRes->stFrame[pstFetchRes->u32TotalFrameNum - j - 1], pGopReverseFrm + j, sizeof(HI_PVR_FETCH_FRAME_S));
            }
           
            pstFetchRes->u32GopNum++;
            if (pstFetchRes->u32GopNum >= MAX_GOP_NUM_ONCE_FETCH)
            {
                HI_WARN_PVR("Gop num %d is too large\n", pstFetchRes->u32GopNum);
                break;
            }
            pstFetchGopAttr++;
            u32FrmCount = 0;
            u32GopFlag = 0;
            memset(pGopReverseFrm, 0, sizeof(HI_PVR_FETCH_FRAME_S) * MAX_FRAME_NUM_ONCE_FETCH);
            pTmpFetchFrame = pGopReverseFrm;
        }
        else
        {
            pTmpFetchFrame++;
        }
        u32FrmNum = PVR_IDX_PREV_POS_IN_CYC(u32FrmNum, (pstCycMgr->u32LastFrame + 1));
    }

    HI_FREE(HI_ID_PVR, pGopReverseFrm);
    pGopReverseFrm = HI_NULL;
    pTmpFetchFrame = HI_NULL;
    return ret;
}


/* get current GOP's attribute, include total frame number, numbers of B/P frame, from one I frame */
HI_S32 PVR_Index_GetCurGOPAttr(PVR_INDEX_HANDLE piIndexHandle, 
                               HI_PVR_FETCH_RESULT_S *pstFetchResult, 
                               HI_U32 u32StartIFrameNum)
{
    HI_U32 u32FrameNumTmp = u32StartIFrameNum, u32GopFlag = 0;
    HI_S32 s32Ret = 0;
    HI_U32 i = 0;
    HI_U16 u16FrameTypeAndGop = 0;
    PVR_INDEX_ENTRY_NEW_S* pstEntry = HI_NULL;
    
    HI_ASSERT_RET(NULL != piIndexHandle);
    HI_ASSERT_RET(NULL != pstFetchResult);

    pstEntry = &pstFetchResult->stFrame[0].stIndexEntry;
    memset(pstFetchResult, 0, sizeof(HI_PVR_FETCH_RESULT_S));

    if (HI_TRUE != PVRIndexIsFrameValid(u32StartIFrameNum, &piIndexHandle->stCycMgr))
    {
        HI_ERR_PVR("input frame number is invalid.\n");
        return HI_FAILURE;
    }

    if(HI_SUCCESS != PVR_Index_GetFrameByNum(piIndexHandle, pstEntry, u32StartIFrameNum))
    {
         HI_WARN_PVR("get the %d entry fail.\n", u32StartIFrameNum);
         return HI_FAILURE;
    }
    u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(piIndexHandle->u32EntryVersion, pstEntry);
    if (!(PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_I) 
        || PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR)))
    {
        HI_WARN_PVR("Start frame is not a I frame.\n");
        return HI_FAILURE;
    }

    while(1)
    {
        pstEntry = &pstFetchResult->stFrame[i].stIndexEntry;

        if (piIndexHandle->stCycMgr.u32EndFrame <= piIndexHandle->stCycMgr.u32StartFrame)
        {
            if (u32FrameNumTmp > piIndexHandle->stCycMgr.u32LastFrame)
                u32FrameNumTmp = 0;

            if ((u32FrameNumTmp > piIndexHandle->stCycMgr.u32EndFrame) && 
                (u32FrameNumTmp < piIndexHandle->stCycMgr.u32StartFrame))
            {
                HI_ERR_PVR("Curnum %d invalide, can't find next I frame.\n", u32FrameNumTmp);
                return HI_FAILURE;
            }
        }
        else
        {
            if (u32FrameNumTmp > piIndexHandle->stCycMgr.u32LastFrame)
            {
                HI_ERR_PVR("Curnum %d invalide, can't find next I frame.\n", u32FrameNumTmp);
                return HI_FAILURE;
            }
        }

        s32Ret = PVR_Index_GetFrameByNum(piIndexHandle, pstEntry, u32FrameNumTmp);
        if (HI_SUCCESS != s32Ret)
        {   
             if (HI_ERR_PVR_FILE_TILL_END == s32Ret)
             {
                HI_INFO_PVR("Reach the end of index, return success.\n");
                return HI_SUCCESS;
             }
             else
             {
                HI_ERR_PVR("Get the %d entry fail.\n", u32FrameNumTmp);
                return HI_FAILURE;
             }
        }
        u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(piIndexHandle->u32EntryVersion, pstEntry);
        if (PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_I) 
            || PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR))
        {
            u32GopFlag++;
            if (u32GopFlag > 1)
                break;
        }

        if (piIndexHandle->stCycMgr.u32EndFrame == u32FrameNumTmp)
        {
            break;
        }

        if (PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_P))
        {
            pstFetchResult->u32PFrameNum++;
        }

        if (PVRINDEX_IS_CERTAINTYPE(piIndexHandle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_B))
        {
            pstFetchResult->u32BFrameNum++;
        }

        pstFetchResult->stFrame[i].u32FrameNum = u32FrameNumTmp;
        pstFetchResult->stFrame[i].u32FrameType = 
            PVR_INDEX_GET_FRAMETYPE(piIndexHandle->u32EntryVersion, PVRINDEX_GET_FRAMETYPEANDGOP(piIndexHandle->u32EntryVersion, pstEntry));

        i++;
        u32FrameNumTmp++;
        pstFetchResult->u32TotalFrameNum = i;
    }

    return HI_SUCCESS;
}


/* the following function for get pointer of some filed of structure PVR_INDEX_HANDLE,
for hi_pvr_smooth_ctrl.c will use these pointer.
Attention: if the contents of these structure changed, need re-compile hi_pvr_smooth_ctrl.c*/
PVR_CYC_MGR_S* PVR_Index_GetCycMgr(PVR_INDEX_HANDLE IndexHandle)
{
    if ((PVR_INDEX_HANDLE)NULL == IndexHandle)
    {
        HI_ERR_PVR("NULL pointer error\n");
        return NULL;
    }
    return (PVR_CYC_MGR_S*)&IndexHandle->stCycMgr;    
}

PVR_INDEX_INFO_S* PVR_Index_GetPVRIdxInfo(PVR_INDEX_HANDLE IndexHandle)
{
    if ((PVR_INDEX_HANDLE)NULL == IndexHandle)
    {
        HI_ERR_PVR("NULL pointer error\n");
        return NULL;
    }
    return (PVR_INDEX_INFO_S*)&IndexHandle->stRecIdxInfo.stIdxInfo;    
}

HI_U32 PVR_Index_GetCurReadFrameNum(PVR_INDEX_HANDLE IndexHandle)
{
    if ((PVR_INDEX_HANDLE)NULL == IndexHandle)
    {
        HI_ERR_PVR("NULL pointer error\n");
        return HI_FAILURE;
    }
    return IndexHandle->u32ReadFrame;   
}

HI_S32 PVR_Index_SetCurReadFrameNum(PVR_INDEX_HANDLE handle, HI_U32 u32ReadFrame)
{
    if ((PVR_INDEX_HANDLE)NULL == handle)
    {
        HI_ERR_PVR("NULL pointer error\n");
        return HI_FAILURE;
    }

    PVR_INDEX_LOCK(&handle->stMutex);
    handle->u32ReadFrame = u32ReadFrame;
    if (handle->bIsRec)
    {
        if (handle->bRecReachPlay && PVRIndexIsFrameValid(u32ReadFrame, &handle->stCycMgr))
        {
            handle->bRecReachPlay = HI_FALSE;
        }
    }
    PVR_INDEX_UNLOCK(&handle->stMutex);
    return HI_SUCCESS;   
}

HI_BOOL PVR_Index_CheckIndexFrmValid(HI_U32 u32IndexFrm, PVR_CYC_MGR_S *pstIndexCycMgr)
{
    return PVRIndexIsFrameValid(u32IndexFrm, pstIndexCycMgr);
}



//1--forward direction   0--backward direction
HI_U32 PVR_Index_CalcFrameDistance(HI_U32 u32Dst, HI_U32 u32Src, PVR_INDEX_DIRECTION_E enDirection, PVR_CYC_MGR_S *pstCycMgr)
{
    if ((pstCycMgr->u32LastFrame == 0) || (-1 == pstCycMgr->u32LastFrame))
    {
        return 0;
    }
    if (0 == pstCycMgr->u32LastFrame + 1)
    {
        HI_ERR_PVR("pstCycMgr->u32LastFrame = 0x%08x\n", pstCycMgr->u32LastFrame);
    }
    if (enDirection == PVR_INDEX_DIRECTION_FORWARD)
    {
        
        return (u32Dst + pstCycMgr->u32LastFrame + 1 - u32Src)%(pstCycMgr->u32LastFrame + 1);
    }
    else
    {
        return (u32Src + pstCycMgr->u32LastFrame + 1 - u32Dst)%(pstCycMgr->u32LastFrame + 1);
    }
}

HI_U32 PVR_Index_GetNextFrameNum(HI_U32 u32FrmNum, PVR_CYC_MGR_S *pstCycMgr)
{
    return PVR_IDX_NEXT_POS_IN_CYC(u32FrmNum, (pstCycMgr->u32LastFrame + 1));
}

HI_U32 PVR_Index_GetPrevFrameNum(HI_U32 u32FrmNum, PVR_CYC_MGR_S *pstCycMgr)
{
    return PVR_IDX_PREV_POS_IN_CYC(u32FrmNum, (pstCycMgr->u32LastFrame + 1));
}
HI_VOID PVR_Index_GetIndexInfo(PVR_INDEX_HANDLE handle, const PVR_INDEX_ENTRY_NEW_S *pstEntry, PVR_INDEX_ENTRY_ELEMENT_E enType, HI_VOID *pArgs)
{
    HI_U32 u32EntryVersion = 0;
    HI_U32 i = 0;
    HI_U8  *pu8Args = (HI_U8 *)pArgs;
    HI_U16 *pu16Args = (HI_U16 *)pArgs;
    HI_U32 *pu32Args = (HI_U32 *)pArgs;
    HI_S32 *ps32Args = (HI_S32 *)pArgs;
    HI_U64 *pu64Args = (HI_U64 *)pArgs;
    
    if ((HI_NULL == pArgs) || (HI_NULL == pstEntry) || (HI_NULL == handle))
        return;
    
    u32EntryVersion = handle->u32EntryVersion;
    switch(enType)
    {
        case PVR_INDEX_FRAME_TYPE:
            *pu32Args = PVR_INDEX_GET_FRAMETYPE(u32EntryVersion, PVRINDEX_GET_FRAMETYPEANDGOP(u32EntryVersion, pstEntry));
            break;
        case PVR_INDEX_UP_FLOW_FLAG:
            *pu16Args = PVRINDEX_GET_UPOVERFLOW(u32EntryVersion, pstEntry);
            break;
        case PVR_INDEX_CYCTIMES:
            *ps32Args = PVRINDEX_GET_CYCTIMES(u32EntryVersion, pstEntry);
            break;
        case PVR_INDEX_GLOBAL_OFFSET:
            *pu64Args = PVRINDEX_GET_GLOBALOFFSET(u32EntryVersion, pstEntry);
            break;
        case PVR_INDEX_DISP_TIME:
            *pu32Args = PVRINDEX_GET_DISPTIME(u32EntryVersion, pstEntry);
            break;
        case PVR_INDEX_FRAME_SIZE:
            *pu32Args = PVRINDEX_GET_FRAMESIZE(u32EntryVersion, pstEntry);
            break;
        case PVR_INDEX_OFFSET:
            *pu64Args = PVRINDEX_GET_OFFSET(u32EntryVersion, pstEntry);
            break;
        case PVR_INDEX_PTS:
            *pu32Args = PVRINDEX_GET_PTS(u32EntryVersion, pstEntry);
            break;
        case PVR_INDEX_INDEX_TYPE:
            *pu16Args = PVRINDEX_GET_INDEXTYPE(u32EntryVersion, pstEntry);
            break;
        case PVR_INDEX_LAST_FRAME:
            break;
        case PVR_INDEX_REF_INFO:
            if (0 != u32EntryVersion)
            {
                if (0 < pstEntry->stEntry2.u8RefCnt)
                {
                    for (i = 0; i < pstEntry->stEntry2.u8RefCnt; i++)
                    {
                        pu32Args[i] = pstEntry->stEntry2.au32RefNum[i];
                    }
                }
            }
            else
            {
                HI_INFO_PVR("No RefInfo to need copying!\n");
                memset(pArgs, 0x00, 16*sizeof(HI_U32));
            }
            break;
        case PVR_INDEX_REF_CNT:
            if (0 != u32EntryVersion)
            {
                *pu8Args = pstEntry->stEntry2.u8RefCnt;
            }
            else
            {
                HI_INFO_PVR("No u8RefCnt to need copying!\n");
                *pu8Args = 0;
            }
            break;
        default:
            break;
    }
}

HI_VOID PVR_Index_SetIndexInfo(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pstEntry, PVR_INDEX_ENTRY_ELEMENT_E enType, HI_VOID *pArgs)
{
    if ((HI_NULL == pArgs) || (HI_NULL == pstEntry) || (HI_NULL == handle))
        return;
    HI_U32 u32EntryVersion = handle->u32EntryVersion;
    
    switch(enType)
    {
        case PVR_INDEX_FRAME_TYPE:
            break;
        case PVR_INDEX_UP_FLOW_FLAG:
            PVRINDEX_SET_UPOVERFLOW(u32EntryVersion, pstEntry, *(HI_U16 *)pArgs);
            break;
        case PVR_INDEX_CYCTIMES:
            PVRINDEX_SET_CYCTIMES(u32EntryVersion, pstEntry, *(HI_S32 *)pArgs);
            break;
        case PVR_INDEX_GLOBAL_OFFSET:
            PVRINDEX_SET_GLOBALOFFSET(u32EntryVersion, pstEntry, *(HI_U64 *)pArgs);
            break;
        case PVR_INDEX_DISP_TIME:
            PVRINDEX_SET_DISPTIME(u32EntryVersion, pstEntry, *(HI_U32 *)pArgs);
            break;
        case PVR_INDEX_FRAME_SIZE:
            PVRINDEX_SET_FRAMESIZE(u32EntryVersion, pstEntry, *(HI_U32 *)pArgs);
            break;
        case PVR_INDEX_OFFSET:
            PVRINDEX_SET_OFFSET(u32EntryVersion, pstEntry, *(HI_U64 *)pArgs);
            break;
        case PVR_INDEX_PTS:
            PVRINDEX_SET_PTS(u32EntryVersion, pstEntry, *(HI_U32 *)pArgs);
            break;
        case PVR_INDEX_INDEX_TYPE:
            PVRINDEX_SET_INDEXTYPE(u32EntryVersion, pstEntry, *(HI_U16 *)pArgs);
            break;
        case PVR_INDEX_LAST_FRAME:
            
            break;
        default:
            break;
    }
}

/*
0 -- quiet, 1 -- error, 2 -- warn, 3 -- info
*/

HI_VOID PVR_Index_PrintEntryInfo(PVR_INDEX_HANDLE handle, const PVR_INDEX_ENTRY_NEW_S *pstEntry, HI_S32 s32Level, const HI_CHAR *pFile, const HI_CHAR *pFunc, HI_S32 s32Line)
{
#if (1 == HI_PROC_SUPPORT)
    HI_U64 u64GlobalOffset __attribute__((unused))   = 0;
    HI_U32 u32DisplayTimeMs __attribute__((unused))   = 0;
    HI_U32 u32FrameSize __attribute__((unused))   = 0;
    HI_U64 u64Offset __attribute__((unused))   = 0;
    HI_U32 u32PtsMs __attribute__((unused))   = 0;
    HI_U16 u16FrameType __attribute__((unused))   = 0;
    HI_U16 u16FrameTypeAndGop __attribute__((unused))   = 0;
    
    HI_U32 u32EntryVersion = handle->u32EntryVersion;
    
    u64GlobalOffset = PVRINDEX_GET_GLOBALOFFSET(u32EntryVersion, pstEntry);
    u32DisplayTimeMs = PVRINDEX_GET_DISPTIME(u32EntryVersion, pstEntry);
    u32FrameSize = PVRINDEX_GET_FRAMESIZE(u32EntryVersion, pstEntry);
    u64Offset = PVRINDEX_GET_OFFSET(u32EntryVersion, pstEntry);
    u32PtsMs = PVRINDEX_GET_PTS(u32EntryVersion, pstEntry);
    u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, pstEntry);
    u16FrameType = PVR_INDEX_GET_FRAMETYPE(u32EntryVersion, u16FrameTypeAndGop);  
    
    switch(s32Level)
    {
        case 3:
            HI_INFO_PVR("\n[%s_%s_%d](u64GlobalOffset, u64Offset, u32FrameSize, u32PtsMs, u32DisplayTimeMs, u16FrameType) = (%llu, %llu, %u, %u, %u, %d)!\n",
                pFile, pFunc, s32Line, u64GlobalOffset, u64Offset, u32FrameSize, u32PtsMs, u32DisplayTimeMs, u16FrameType);
            break;
        case 2:
            HI_WARN_PVR("\n[%s_%s_%d](u64GlobalOffset, u64Offset, u32FrameSize, u32PtsMs, u32DisplayTimeMs, u16FrameType) = (%llu, %llu, %u, %u, %u, %d)!\n",
                pFile, pFunc, s32Line, u64GlobalOffset, u64Offset, u32FrameSize, u32PtsMs, u32DisplayTimeMs, u16FrameType);
            break;
        case 1:
            HI_ERR_PVR("\n[%s_%s_%d](u64GlobalOffset, u64Offset, u32FrameSize, u32PtsMs, u32DisplayTimeMs, u16FrameType) = (%llu, %llu, %u, %u, %u, %d)!\n",
                pFile, pFunc, s32Line, u64GlobalOffset, u64Offset, u32FrameSize, u32PtsMs, u32DisplayTimeMs, u16FrameType);
            break;
        case 0:
        default:
            break;
    }
#endif
    return;
}

/*CNcomment: 旧的录制文件，才使用旧的entry结构体*/
HI_S32 PVR_Index_GetEntryLength(PVR_INDEX_HANDLE handle)
{
    if (HI_NULL == handle)
        return HI_FAILURE;
    if (0 == handle->u32EntryVersion)
    {
        handle->u32EntryLength = sizeof(PVR_INDEX_ENTRY_S);
    }
    else
    {
        handle->u32EntryLength = sizeof(PVR_INDEX_ENTRY_NEW_S);
    }
    HI_INFO_PVR("\t\t(u32EntryVersion, u32EntryLength) = (%u, %u)!\n", handle->u32EntryVersion, handle->u32EntryLength);
    return HI_SUCCESS;
}

HI_U32 PVR_INDEX_get_preIoffset(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pstEntry)
{
    return (HI_U32)PVRINDEX_GET_PREIOFFSET(handle->u32EntryVersion, pstEntry);
}
HI_BOOL PVR_INDEX_is_Iframe(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pstEntry)
{
    HI_U16 u16FrameTypeAndGop = 0;
    u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, pstEntry);
    return PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_I);
}

HI_BOOL PVR_INDEX_is_Bframe(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pstEntry)
{
    HI_U16 u16FrameTypeAndGop = 0;
    u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, pstEntry);
    return PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_B);
}


HI_BOOL PVR_INDEX_is_Pframe(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pstEntry)
{
    HI_U16 u16FrameTypeAndGop = 0;
    u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, pstEntry);
    return PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_P);
}

HI_BOOL PVR_INDEX_is_IDRframe(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pstEntry)
{
    HI_U16 u16FrameTypeAndGop = 0;
    u16FrameTypeAndGop = PVRINDEX_GET_FRAMETYPEANDGOP(handle->u32EntryVersion, pstEntry);
    return PVRINDEX_IS_CERTAINTYPE(handle->u32EntryVersion, u16FrameTypeAndGop, PVR_INDEX_FRAME_IDR);
}

#if 0  /*obsolete APIs*/
HI_VOID PVR_Index_CycMoveReadFrm(PVR_INDEX_HANDLE handle, HI_S32 s32Offset)
{
    PVR_INDEX_LOCK(&handle->stMutex);
    PVRIndexCycMoveReadFrame(handle, s32Offset);
    PVR_INDEX_UNLOCK(&handle->stMutex);
}
HI_S32 PVR_Index_GetFrmNumByEntry(PVR_INDEX_HANDLE pstIndexHandle, PVR_INDEX_ENTRY_NEW_S stIndexEntry, HI_S32 *ps32FrmNum)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32StartFrmNum = 0;
    HI_U32 u32EndFrmNum = 0;
    HI_U32 u32LastFrmNum = 0;
    HI_U32 u32MidFrmNum = 0;
    HI_U32 u32Version = pstIndexHandle->u32EntryVersion;

    PVR_INDEX_ENTRY_NEW_S stStartIndexEntry;
    PVR_INDEX_ENTRY_NEW_S stEndIndexEntry;
    PVR_INDEX_ENTRY_NEW_S stMidIndexEntry;
    PVR_INDEX_ENTRY_NEW_S stZeroIndexEntry;
    PVR_INDEX_ENTRY_NEW_S stLastIndexEntry;
    HI_U64 u64StartGlobalOffset = 0;
    HI_U64 u64LastGlobalOffset = 0;
    HI_U64 u64EndGlobalOffset = 0;
    HI_U64 u64ZeroGlobalOffset = 0;
    HI_U64 u64CurGlobalOffset = 0;
    HI_U64 u64MidGlobalOffset = 0;

    memset(&stStartIndexEntry, 0x00, sizeof(stStartIndexEntry));
    memset(&stEndIndexEntry, 0x00, sizeof(stEndIndexEntry));
    memset(&stMidIndexEntry, 0x00, sizeof(stMidIndexEntry));
    memset(&stZeroIndexEntry, 0x00, sizeof(stZeroIndexEntry));
    memset(&stLastIndexEntry, 0x00, sizeof(stLastIndexEntry));

    u32StartFrmNum = pstIndexHandle->stCycMgr.u32StartFrame;
    u32EndFrmNum = (pstIndexHandle->stCycMgr.u32EndFrame == -1)?0:pstIndexHandle->stCycMgr.u32EndFrame;
    u32LastFrmNum = (pstIndexHandle->stCycMgr.u32LastFrame == -1)?0:pstIndexHandle->stCycMgr.u32LastFrame;

    if(HI_SUCCESS != PVR_Index_GetFrameByNum(pstIndexHandle, &stStartIndexEntry, u32StartFrmNum))
    {
        HI_ERR_PVR("get start index %d entry fail\n", u32StartFrmNum);
        return HI_FAILURE;
    }

    if(HI_SUCCESS != PVR_Index_GetFrameByNum(pstIndexHandle, &stZeroIndexEntry, 0))
    {
        HI_ERR_PVR("get index 0 entry fail\n", 0);
        return HI_FAILURE;
    }

    s32Ret = PVR_Index_GetFrameByNum(pstIndexHandle, &stLastIndexEntry, u32LastFrmNum);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("get last index %d entry fail. ret %x\n", u32LastFrmNum, s32Ret);
        return HI_FAILURE;
    }

    s32Ret = PVR_Index_GetFrameByNum(pstIndexHandle, &stEndIndexEntry, u32EndFrmNum);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("get the %d entry fail. ret %x\n", u32EndFrmNum, s32Ret);
        return HI_FAILURE;
    }

    if (u32EndFrmNum <= u32StartFrmNum)
    {
        u64CurGlobalOffset = PVRINDEX_GET_GLOBALOFFSET(u32Version, &stIndexEntry);
        u64StartGlobalOffset = PVRINDEX_GET_GLOBALOFFSET(u32Version, &stStartIndexEntry);
        u64LastGlobalOffset = PVRINDEX_GET_GLOBALOFFSET(u32Version, &stLastIndexEntry);
        u64EndGlobalOffset = PVRINDEX_GET_GLOBALOFFSET(u32Version, &stEndIndexEntry);
        u64ZeroGlobalOffset = PVRINDEX_GET_GLOBALOFFSET(u32Version, &stZeroIndexEntry);
        if((u64CurGlobalOffset >= u64StartGlobalOffset) && (u64CurGlobalOffset <= u64LastGlobalOffset))
        {
            u32MidFrmNum = (u32LastFrmNum - u32StartFrmNum)/2;
            u32EndFrmNum = u32LastFrmNum;

            if(HI_SUCCESS != PVR_Index_GetFrameByNum(pstIndexHandle, &stEndIndexEntry, u32EndFrmNum))
            {
                HI_ERR_PVR("get the %d entry fail.\n", u32EndFrmNum);
                return HI_FAILURE;
            }
        }
        else if((u64CurGlobalOffset <= u64EndGlobalOffset) && (u64CurGlobalOffset >= u64ZeroGlobalOffset))
        {
            u32MidFrmNum = u32EndFrmNum/2;
            u32StartFrmNum = 0;

            if(HI_SUCCESS != PVR_Index_GetFrameByNum(pstIndexHandle, &stStartIndexEntry, u32StartFrmNum))
            {
                HI_ERR_PVR("get the %d entry fail.\n", u32StartFrmNum);
                return HI_FAILURE;
            }
        }
        else
        {
            HI_ERR_PVR("invalid entry offset=%#llx zero(0)ffset=%#llx start(%d)offset=%#llx end(%d)offset=%#llx last(%d)offset=%#llx.\n",
                u64CurGlobalOffset,u64ZeroGlobalOffset, u32StartFrmNum, u64StartGlobalOffset,
                u32EndFrmNum, u64EndGlobalOffset, u32LastFrmNum, u64LastGlobalOffset);
            return HI_FAILURE;
        }
    }
    else
    {
        u32MidFrmNum = u32EndFrmNum/2;
    }

    if(HI_SUCCESS != PVR_Index_GetFrameByNum(pstIndexHandle, &stMidIndexEntry, u32MidFrmNum))
    {
        HI_ERR_PVR("get the %d entry fail.\n", u32MidFrmNum);
        return HI_FAILURE;
    }

    while((HI_S32)u32StartFrmNum <= (HI_S32)u32EndFrmNum)
    {
        u32MidFrmNum = u32StartFrmNum + (u32EndFrmNum - u32StartFrmNum)/2;

        if(HI_SUCCESS != PVR_Index_GetFrameByNum(pstIndexHandle, &stMidIndexEntry, u32MidFrmNum))
        {
            HI_ERR_PVR("get the %d entry fail.\n", u32MidFrmNum);
            return HI_FAILURE;
        }
        u64MidGlobalOffset = PVRINDEX_GET_GLOBALOFFSET(u32Version, &stMidIndexEntry);
        if(u64MidGlobalOffset > u64CurGlobalOffset)
        {
            u32EndFrmNum = u32MidFrmNum - 1;
        }
        else if(u64MidGlobalOffset < u64CurGlobalOffset)
        {
            u32StartFrmNum = u32MidFrmNum + 1;
        }
        else
        {
            if (HI_TRUE == PVRIndexIsFrameValid(u32MidFrmNum, &pstIndexHandle->stCycMgr))
            {
                *ps32FrmNum = (HI_S32)u32MidFrmNum;
                return HI_SUCCESS;
            }
            else
            {
                HI_ERR_PVR("find invalid frame number %d(start=%d end=%d) from entry offset %#llx.\n", 
                    u32MidFrmNum, u32StartFrmNum, u32EndFrmNum, u64CurGlobalOffset);
                return HI_FAILURE;
            }
        }
    }
    HI_ERR_PVR("can not find frame number from entry offset %#llx start=%d end=%d mid=%d. \n", 
               u64CurGlobalOffset, u32StartFrmNum, u32EndFrmNum, u32MidFrmNum);
    return HI_FAILURE;
}
/*reserved function for smooth play */
HI_S32 PVR_Index_GetMaxBitrate(PVR_INDEX_HANDLE piIndexHandle)
{
    return 0x100;
}
/*get stream bit rate  */
HI_S32 PVR_Index_GetStreamBitRate(PVR_INDEX_HANDLE piIndexHandle,
                                  HI_U32 *pBitRate,
                                  HI_U32 u32StartFrameNum, 
                                  HI_U32 u32EndFrameNum)
{
    HI_U32 i = 0;
    HI_U32 u32StartTime = 0;
    HI_U32 u32EndTime = 0;
    HI_U64 u64TotalBytes = 0;
    HI_U32 u32EntryVersion = piIndexHandle->u32EntryVersion;
    PVR_INDEX_ENTRY_NEW_S  stTmpEntry;
    
    if (HI_TRUE != PVRIndexIsFrameValid(u32StartFrameNum, &piIndexHandle->stCycMgr))
    {
        HI_ERR_PVR("input start frame number is invalid.\n");
        return HI_FAILURE;
    }

    if (HI_TRUE != PVRIndexIsFrameValid(u32EndFrameNum, &piIndexHandle->stCycMgr))
    {
        HI_ERR_PVR("input end frame number is invalid.\n");
        return HI_FAILURE;
    }

    if(HI_SUCCESS != PVR_Index_GetFrameByNum(piIndexHandle, &stTmpEntry, u32StartFrameNum))
    {
         HI_INFO_PVR("get the %d entry fail.\n", i);
         return HI_FAILURE;
    }
    u32StartTime = PVRINDEX_GET_DISPTIME(u32EntryVersion, &stTmpEntry);

    if(HI_SUCCESS != PVR_Index_GetFrameByNum(piIndexHandle, &stTmpEntry, u32EndFrameNum))
    {
         HI_INFO_PVR("get the %d entry fail.\n", i);
         return HI_FAILURE;
    }
    u32EndTime = PVRINDEX_GET_DISPTIME(u32EntryVersion, &stTmpEntry);

    if((u32StartTime == u32EndTime) || (u32StartTime >= u32EndTime))
    {
        HI_INFO_PVR("invalid pts, can not get bitrate.\n");
        return HI_FAILURE;
    }

    /* TODO: use global offset to calc total bytes */
    if (u32EndFrameNum < u32StartFrameNum)
    {
        for (i = u32StartFrameNum; i < piIndexHandle->stCycMgr.u32LastFrame; i++)
        {
            if(HI_SUCCESS != PVR_Index_GetFrameByNum(piIndexHandle, &stTmpEntry, i))
            {
                 HI_INFO_PVR("get the %d entry fail.\n", i);
                 return HI_FAILURE;
            }
            
            u64TotalBytes += PVRINDEX_GET_FRAMESIZE(u32EntryVersion, &stTmpEntry);
        }

        for (i = 0; i < u32EndFrameNum; i++)
        {
            if(HI_SUCCESS != PVR_Index_GetFrameByNum(piIndexHandle, &stTmpEntry, i))
            {
                 HI_INFO_PVR("get the %d entry fail.\n", i);
                 return HI_FAILURE;
            }
            
            u64TotalBytes += PVRINDEX_GET_FRAMESIZE(u32EntryVersion, &stTmpEntry);
        }
    }
    else
    {
        for (i = u32StartFrameNum; i < u32EndFrameNum; i++)
        {
            if(HI_SUCCESS != PVR_Index_GetFrameByNum(piIndexHandle, &stTmpEntry, i))
            {
                 HI_INFO_PVR("get the %d entry fail.\n", i);
                 return HI_FAILURE;
            }
            
            u64TotalBytes += PVRINDEX_GET_FRAMESIZE(u32EntryVersion, &stTmpEntry);
        }
    }

    *pBitRate = (u64TotalBytes*8)/((u32EndTime - u32StartTime)/1000);

    return HI_SUCCESS;
}
/*****************************************************************************
 Prototype       : PVR_Index_GetCurrentFrame
 Description     : get the frame pointed to the current read pointer. not move the read pointer
 Input           : handle         **
                   pFrame         **
                   pDisplayTimes  **
 Output          : None
 Return Value    :
  History
  1.Date         : 2010/06/02
    Author       : j40671
    Modification : Created function
*****************************************************************************/
HI_S32 PVR_Index_GetCurrentFrame(const PVR_INDEX_HANDLE handle,  PVR_INDEX_ENTRY_NEW_S *pEntry)
{
    HI_S32 ret;
    HI_ASSERT_RET(NULL != handle);
    HI_ASSERT_RET(NULL != pEntry);

    PVR_INDEX_LOCK(&(handle->stMutex));

    ret = PVRIndexGetCurrentEntry(handle, pEntry);

    PVR_INDEX_UNLOCK(&(handle->stMutex));
    return ret;
}
/*****************************************************************************
 Prototype       : PVR_Index_SeekToEnd
 Description     : move the read pointer of index to the end frame
 Input           : handle         **
                   pFrame         **
                   pDisplayTimes  **
 Output          : None
 Return Value    :
  History
  1.Date         : 2010/09/21
    Author       : j40671
    Modification : Created function

*****************************************************************************/
HI_S32 PVR_Index_SeekToEnd(PVR_INDEX_HANDLE handle)
{
    HI_ASSERT_RET(handle != NULL);

    PVR_INDEX_LOCK(&(handle->stMutex));
    HI_WARN_PVR("seek to end\n");

    handle->u32ReadFrame = handle->stCycMgr.u32EndFrame;
    handle->bRecReachPlay = HI_FALSE;
    PVR_INDEX_UNLOCK(&(handle->stMutex));

    return HI_SUCCESS;
}
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

