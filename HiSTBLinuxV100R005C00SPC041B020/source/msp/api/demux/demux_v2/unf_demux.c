/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  hi_unf_demux.c
* Description:  Define functions used to test function of demux driver
*
* History:
* Version      Date         Author     DefectNum    Description
* main\1    2006-11-01      l55251        NULL      Create this file.
***********************************************************************************/
#include <stdio.h>

#include "hi_type.h"
#include "hi_debug.h"

#include "demux_debug.h"
#include "hi_mpi_demux.h"
#include "hi_unf_demux.h"

HI_S32 HI_UNF_DMX_Init(HI_VOID)
{
    return HI_MPI_DMX_Init();
}

HI_S32 HI_UNF_DMX_DeInit(HI_VOID)
{
    return HI_MPI_DMX_DeInit();
}

HI_S32 HI_UNF_DMX_GetCapability(HI_UNF_DMX_CAPABILITY_S *pstCap)
{
    return HI_MPI_DMX_GetCapability(pstCap);
}

HI_S32 HI_UNF_DMX_GetTSPortAttr(HI_UNF_DMX_PORT_E enPortId, HI_UNF_DMX_PORT_ATTR_S *pstAttr)
{
    return HI_MPI_DMX_GetTSPortAttr(enPortId, pstAttr);
}

HI_S32 HI_UNF_DMX_SetTSPortAttr(HI_UNF_DMX_PORT_E enPortId, const HI_UNF_DMX_PORT_ATTR_S *pstAttr)
{
    return HI_MPI_DMX_SetTSPortAttr(enPortId, pstAttr);
}

HI_S32 HI_UNF_DMX_GetTSOPortAttr(HI_UNF_DMX_TSO_PORT_E enPortId, HI_UNF_DMX_TSO_PORT_ATTR_S *pstAttr)
{
    return HI_MPI_DMX_GetTSOPortAttr(enPortId, pstAttr);
}

HI_S32 HI_UNF_DMX_SetTSOPortAttr(HI_UNF_DMX_TSO_PORT_E enPortId, HI_UNF_DMX_TSO_PORT_ATTR_S *pstAttr)
{
    return HI_MPI_DMX_SetTSOPortAttr(enPortId, pstAttr);
}

#if 0
HI_S32 HI_UNF_DMX_GetTagPortAttr(HI_UNF_DMX_PORT_E enPortId, HI_UNF_DMX_TAG_ATTR_S *pstAttr)
{
    return HI_MPI_DMX_GetTagPortAttr(enPortId, pstAttr);
}

HI_S32 HI_UNF_DMX_SetTagPortAttr(HI_UNF_DMX_PORT_E enPortId, const HI_UNF_DMX_TAG_ATTR_S *pstAttr)
{
    return HI_MPI_DMX_SetTagPortAttr(enPortId, pstAttr);
}
#endif

HI_S32 HI_UNF_DMX_AttachTSPort(HI_U32 u32DmxId, HI_UNF_DMX_PORT_E enPortId)
{
    return HI_MPI_DMX_AttachTSPort(u32DmxId, enPortId);
}

HI_S32 HI_UNF_DMX_DetachTSPort(HI_U32 u32DmxId)
{
    return HI_MPI_DMX_DetachTSPort(u32DmxId);
}

HI_S32 HI_UNF_DMX_GetTSPortId(HI_U32 u32DmxId, HI_UNF_DMX_PORT_E *penPortId)
{
    return HI_MPI_DMX_GetTSPortId(u32DmxId, penPortId);
}

HI_S32 HI_UNF_DMX_GetTSBufferDefaultAttr(HI_UNF_DMX_TSBUF_ATTR_S *pstTsBufAttr)
{
    return HI_MPI_DMX_GetTSBufferDefaultAttr(pstTsBufAttr);
}

HI_S32 HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_E enPortId, HI_U32 u32TsBufSize, HI_HANDLE *phTsBuffer)
{
    HI_UNF_DMX_TSBUF_ATTR_S Attrs = {
        .enSecureMode = HI_UNF_DMX_SECURE_MODE_NONE,
        .u32BufSize   = u32TsBufSize,
    };
    
    return HI_MPI_DMX_CreateTSBuffer(enPortId, &Attrs, phTsBuffer);
}

HI_S32 HI_UNF_DMX_CreateTSBufferExt(HI_UNF_DMX_PORT_E enPortId, const HI_UNF_DMX_TSBUF_ATTR_S *pstTsBufAttr, HI_HANDLE *phTsBuffer)
{
    return HI_MPI_DMX_CreateTSBuffer(enPortId, pstTsBufAttr, phTsBuffer);
}

HI_S32 HI_UNF_DMX_DestroyTSBuffer(HI_HANDLE hTsBuffer)
{
    return HI_MPI_DMX_DestroyTSBuffer(hTsBuffer);
}

HI_S32 HI_UNF_DMX_GetTSBuffer(HI_HANDLE hTsBuffer, HI_U32 u32ReqLen, HI_UNF_STREAM_BUF_S *pstData, HI_U32 u32TimeOutMs)
{
    HI_U32 u32PhyAddr;

    return HI_MPI_DMX_GetTSBuffer(hTsBuffer, u32ReqLen, pstData, &u32PhyAddr, u32TimeOutMs);
}

HI_S32 HI_UNF_DMX_GetTSBufferEx(HI_HANDLE hTsBuffer, HI_U32 u32ReqLen,
        HI_UNF_STREAM_BUF_S *pstData, HI_U32 *pu32PhyAddr, HI_U32 u32TimeOutMs)
{
    return HI_MPI_DMX_GetTSBuffer(hTsBuffer, u32ReqLen, pstData, pu32PhyAddr, u32TimeOutMs);
}

HI_S32 HI_UNF_DMX_PushTSBuffer(HI_HANDLE hTsBuffer, const HI_UNF_STREAM_BUF_S *pstData)
{
    return HI_MPI_DMX_PushTSBuffer(hTsBuffer, pstData);
}

HI_S32 HI_UNF_DMX_ReleaseTSBuffer(HI_HANDLE hTsBuffer, const HI_UNF_STREAM_BUF_S *pstData)
{
    return HI_MPI_DMX_ReleaseTSBuffer(hTsBuffer, pstData);
}

HI_S32 HI_UNF_DMX_PutTSBuffer(HI_HANDLE hTsBuffer, HI_U32 u32ValidDataLen)
{
    return HI_MPI_DMX_PutTSBuffer(hTsBuffer, u32ValidDataLen, 0);
}

HI_S32 HI_UNF_DMX_PutTSBufferEx(HI_HANDLE hTsBuffer, HI_U32 u32ValidDataLen, HI_U32 u32StartPos)
{
    return HI_MPI_DMX_PutTSBuffer(hTsBuffer, u32ValidDataLen, u32StartPos);
}

HI_S32 HI_UNF_DMX_ResetTSBuffer(HI_HANDLE hTsBuffer)
{
    return HI_MPI_DMX_ResetTSBuffer(hTsBuffer);
}

HI_S32 HI_UNF_DMX_GetTSBufferStatus(HI_HANDLE hTsBuffer, HI_UNF_DMX_TSBUF_STATUS_S *pStatus)
{
    return HI_MPI_DMX_GetTSBufferStatus(hTsBuffer, pStatus);
}

HI_S32 HI_UNF_DMX_GetTSBufferPortId(HI_HANDLE hTsBuffer, HI_UNF_DMX_PORT_E *penPortId)
{
    return HI_MPI_DMX_GetTSBufferPortId(hTsBuffer, penPortId);
}

HI_S32 HI_UNF_DMX_GetTSBufferHandle(HI_UNF_DMX_PORT_E enPortId, HI_HANDLE *phTsBuffer)
{
    return HI_MPI_DMX_GetTSBufferHandle(enPortId, phTsBuffer);
}

HI_S32 HI_UNF_DMX_GetChannelDefaultAttr(HI_UNF_DMX_CHAN_ATTR_S *pstChAttr)
{
    return HI_MPI_DMX_GetChannelDefaultAttr(pstChAttr);
}

HI_S32 HI_UNF_DMX_CreateChannel(HI_U32 u32DmxId, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr, HI_HANDLE *phChannel)
{
    return HI_MPI_DMX_CreateChannel(u32DmxId, pstChAttr, phChannel);
}

HI_S32 HI_UNF_DMX_CreateChannelWithPID(HI_U32 u32DmxId, HI_U32 u32Pid, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr, HI_HANDLE *phChannel)
{
    return HI_ERR_DMX_NOT_SUPPORT;
}

HI_S32 HI_UNF_DMX_DestroyChannel(HI_HANDLE hChannel)
{
    return HI_MPI_DMX_DestroyChannel(hChannel);
}

HI_S32 HI_UNF_DMX_GetChannelAttr(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr)
{
    return HI_MPI_DMX_GetChannelAttr(hChannel, pstChAttr);
}

HI_S32 HI_UNF_DMX_SetChannelAttr(HI_HANDLE hChannel, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr)
{
    return HI_MPI_DMX_SetChannelAttr(hChannel, pstChAttr);
}

HI_S32 HI_UNF_DMX_SetChannelPID(HI_HANDLE hChannel, HI_U32 u32Pid)
{
    return HI_MPI_DMX_SetChannelPID(hChannel, u32Pid);
}

HI_S32 HI_UNF_DMX_GetChannelPID(HI_HANDLE hChannel, HI_U32 *pu32Pid)
{
    return HI_MPI_DMX_GetChannelPID(hChannel, pu32Pid);
}

HI_S32 HI_UNF_DMX_OpenChannel(HI_HANDLE hChannel)
{
    return HI_MPI_DMX_OpenChannel(hChannel);
}

HI_S32 HI_UNF_DMX_CloseChannel(HI_HANDLE hChannel)
{
    return HI_MPI_DMX_CloseChannel(hChannel);
}

HI_S32 HI_UNF_DMX_GetChannelStatus(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_STATUS_S *pstStatus)
{
    return HI_MPI_DMX_GetChannelStatus(hChannel, pstStatus);
}

HI_S32 HI_UNF_DMX_GetChannelTsCount(HI_HANDLE hChannel, HI_U32 *pu32TsCount)
{
    return HI_MPI_DMX_GetChannelTsCount(hChannel,pu32TsCount);
}

HI_S32 HI_UNF_DMX_GetChannelHandle(HI_U32 u32DmxId, HI_U32 u32Pid, HI_HANDLE *phChannel)
{
    return HI_MPI_DMX_GetChannelHandle(u32DmxId, u32Pid, phChannel);
}

HI_S32 HI_UNF_DMX_GetFreeChannelCount(HI_U32 u32DmxId, HI_U32 *pu32FreeCount)
{
    return HI_MPI_DMX_GetFreeChannelCount(u32DmxId, pu32FreeCount);
}

HI_S32 HI_UNF_DMX_GetScrambledFlag(HI_HANDLE hChannel, HI_UNF_DMX_SCRAMBLED_FLAG_E *penScrambleFlag)
{
    return HI_MPI_DMX_GetScrambledFlag(hChannel, penScrambleFlag);
}

HI_S32 HI_UNF_DMX_CreateFilter(HI_U32 u32DmxId, const HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr, HI_HANDLE *phFilter)
{
    return HI_MPI_DMX_CreateFilter(u32DmxId, pstFilterAttr, phFilter);
}

HI_S32 HI_UNF_DMX_DestroyFilter(HI_HANDLE hFilter)
{
    return HI_MPI_DMX_DestroyFilter(hFilter);
}

HI_S32 HI_UNF_DMX_DeleteAllFilter(HI_HANDLE hChannel)
{
    return HI_MPI_DMX_DeleteAllFilter(hChannel);
}

HI_S32 HI_UNF_DMX_SetFilterAttr(HI_HANDLE hFilter, const HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr)
{
    return HI_MPI_DMX_SetFilterAttr(hFilter, pstFilterAttr);
}

HI_S32 HI_UNF_DMX_GetFilterAttr(HI_HANDLE hFilter, HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr)
{
    return HI_MPI_DMX_GetFilterAttr(hFilter, pstFilterAttr);
}

HI_S32 HI_UNF_DMX_AttachFilter(HI_HANDLE hFilter, HI_HANDLE hChannel)
{
    return HI_MPI_DMX_AttachFilter(hFilter, hChannel);
}

HI_S32 HI_UNF_DMX_DetachFilter(HI_HANDLE hFilter, HI_HANDLE hChannel)
{
    return HI_MPI_DMX_DetachFilter(hFilter, hChannel);
}

HI_S32 HI_UNF_DMX_GetFilterChannelHandle(HI_HANDLE hFilter, HI_HANDLE *phChannel)
{
    return HI_MPI_DMX_GetFilterChannelHandle(hFilter, phChannel);
}

HI_S32 HI_UNF_DMX_GetFreeFilterCount(HI_U32 u32DmxId, HI_U32 *pu32FreeCount)
{
    return HI_MPI_DMX_GetFreeFilterCount(u32DmxId, pu32FreeCount);
}

HI_S32 HI_UNF_DMX_GetDataHandle(HI_HANDLE *phChannel, HI_U32 *pu32ChNum, HI_U32 u32TimeOutMs)
{
    return HI_MPI_DMX_GetDataHandle(phChannel, pu32ChNum, u32TimeOutMs);
}

HI_S32 HI_UNF_DMX_SelectDataHandle(HI_HANDLE *phWatchChannel, HI_U32 u32WatchNum,
            HI_HANDLE *phDataChannel, HI_U32 *pu32ChNum, HI_U32 u32TimeOutMs)
{
    return HI_MPI_DMX_SelectDataHandle(phWatchChannel, u32WatchNum, phDataChannel, pu32ChNum, u32TimeOutMs);
}

HI_S32 HI_UNF_DMX_AcquireBuf(HI_HANDLE hChannel, HI_U32 u32AcquireNum,
            HI_U32 *pu32AcquiredNum, HI_UNF_DMX_DATA_S *pstBuf, HI_U32 u32TimeOutMs)
{
    return HI_MPI_DMX_AcquireBuf(hChannel, u32AcquireNum, pu32AcquiredNum, pstBuf, u32TimeOutMs);
}

HI_S32 HI_UNF_DMX_ReleaseBuf(HI_HANDLE hChannel, HI_U32 u32ReleaseNum, HI_UNF_DMX_DATA_S *pstBuf)
{
    return HI_MPI_DMX_ReleaseBuf(hChannel, u32ReleaseNum, pstBuf);
}

HI_S32 HI_UNF_DMX_AcquireEs(HI_HANDLE hChannel, HI_UNF_ES_BUF_S *pEsBuf)
{
    return HI_MPI_DMX_AcquireEs(hChannel, pEsBuf);
}

HI_S32 HI_UNF_DMX_ReleaseEs(HI_HANDLE hChannel, const HI_UNF_ES_BUF_S *pEsBuf)
{
    return HI_MPI_DMX_ReleaseEs(hChannel, pEsBuf);
}

HI_S32 HI_UNF_DMX_GetTSPortPacketNum(HI_UNF_DMX_PORT_E enPortId, HI_UNF_DMX_PORT_PACKETNUM_S *sPortStat)
{
    return HI_MPI_DMX_GetTSPortPacketNum(enPortId, sPortStat);
}

HI_S32 HI_UNF_DMX_CreateRecChn(HI_UNF_DMX_REC_ATTR_S *pstRecAttr, HI_HANDLE *phRecChn)
{
    return HI_MPI_DMX_CreateRecChn(pstRecAttr, phRecChn);
}

HI_S32 HI_UNF_DMX_DestroyRecChn(HI_HANDLE hRecChn)
{
    return HI_MPI_DMX_DestroyRecChn(hRecChn);
}

HI_S32 HI_UNF_DMX_AddRecPid(HI_HANDLE hRecChn, HI_U32 u32Pid, HI_HANDLE *phChannel)
{
    return HI_MPI_DMX_AddRecPid(hRecChn, u32Pid, phChannel);
}

HI_S32 HI_UNF_DMX_DelRecPid(HI_HANDLE hRecChn, HI_HANDLE hChannel)
{
    return HI_MPI_DMX_DelRecPid(hRecChn, hChannel);
}

HI_S32 HI_UNF_DMX_DelAllRecPid(HI_HANDLE hRecChn)
{
    return HI_MPI_DMX_DelAllRecPid(hRecChn);
}

HI_S32 HI_UNF_DMX_AddExcludeRecPid(HI_HANDLE hRecChn, HI_U32 u32Pid)
{
    return HI_MPI_DMX_AddExcludeRecPid(hRecChn, u32Pid);
}

HI_S32 HI_UNF_DMX_DelExcludeRecPid(HI_HANDLE hRecChn, HI_U32 u32Pid)
{
    return HI_MPI_DMX_DelExcludeRecPid(hRecChn, u32Pid);
}

HI_S32 HI_UNF_DMX_DelAllExcludeRecPid(HI_HANDLE hRecChn)
{
    return HI_MPI_DMX_DelAllExcludeRecPid(hRecChn);
}

HI_S32 HI_UNF_DMX_StartRecChn(HI_HANDLE hRecChn)
{
    return HI_MPI_DMX_StartRecChn(hRecChn);
}

HI_S32 HI_UNF_DMX_StopRecChn(HI_HANDLE hRecChn)
{
    return HI_MPI_DMX_StopRecChn(hRecChn);
}

HI_S32 HI_UNF_DMX_AcquireRecData(HI_HANDLE hRecChn, HI_UNF_DMX_REC_DATA_S *pstRecData, HI_U32 u32TimeoutMs)
{
    HI_S32 ret = HI_FAILURE;
    HI_MPI_DMX_REC_DATA_S RawData;

    if (!pstRecData)
    {
        HI_ERR_DEMUX("Null Pointer!\n"); 
        return HI_ERR_DMX_NULL_PTR; 
    }
    
    ret = HI_MPI_DMX_AcquireRecData(hRecChn, &RawData, u32TimeoutMs);
    if (HI_SUCCESS == ret)
    {
        pstRecData->pDataAddr      = RawData.pDataAddr;
        pstRecData->u32DataPhyAddr = RawData.u32DataPhyAddr;
        pstRecData->u32Len         = RawData.u32Len;
    }

    return ret;
}

HI_S32 HI_UNF_DMX_ReleaseRecData(HI_HANDLE hRecChn, const HI_UNF_DMX_REC_DATA_S *pstRecData)
{
    HI_MPI_DMX_REC_DATA_S RawData;

    if (!pstRecData)
    {
        HI_ERR_DEMUX("Null Pointer!\n"); 
        return HI_ERR_DMX_NULL_PTR; 
    }

    RawData.pDataAddr      = pstRecData->pDataAddr;
    RawData.u32DataPhyAddr = pstRecData->u32DataPhyAddr;
    RawData.u32Len         = pstRecData->u32Len;
    
    return HI_MPI_DMX_ReleaseRecData(hRecChn, &RawData);
}

HI_S32 HI_UNF_DMX_AcquireRecIndex(HI_HANDLE hRecChn, HI_UNF_DMX_REC_INDEX_S *pstRecIndex, HI_U32 u32TimeoutMs)
{
    HI_S32 ret = HI_FAILURE;
    HI_MPI_DMX_REC_INDEX_S RawIndex;

    if (!pstRecIndex)
    {
        HI_ERR_DEMUX("Null Pointer!\n"); 
        return HI_ERR_DMX_NULL_PTR; 
    }

    ret = HI_MPI_DMX_AcquireRecIndex(hRecChn, &RawIndex, u32TimeoutMs);
    if (HI_SUCCESS == ret)
    {
        pstRecIndex->enFrameType     = RawIndex.enFrameType;
        pstRecIndex->u32PtsMs        = RawIndex.u32PtsMs;
        pstRecIndex->u64GlobalOffset = RawIndex.u64GlobalOffset;
        pstRecIndex->u32FrameSize    = RawIndex.u32FrameSize;
        pstRecIndex->u32DataTimeMs   = RawIndex.u32DataTimeMs;
    }

    return ret;
}

HI_S32 HI_UNF_DMX_AcquireRecDataAndIndex(HI_HANDLE hRecChn, HI_UNF_DMX_REC_DATA_INDEX_S* pstRecDataIdx)
{
    HI_S32 ret = HI_FAILURE;
    HI_MPI_DMX_REC_DATA_INDEX_S RawDataIndex = {
        .u32IdxNum = 0,
        .u32RecDataCnt = 0,
    };

    if (!pstRecDataIdx)
    {
        HI_ERR_DEMUX("Null Pointer!\n"); 
        return HI_ERR_DMX_NULL_PTR; 
    }
    
    ret = HI_MPI_DMX_AcquireRecDataAndIndex(hRecChn, &RawDataIndex);
    if (HI_SUCCESS == ret)
    {
        HI_U32 idx;

        /* covert MPI to UNF structure. */
        pstRecDataIdx->u32RecDataCnt = RawDataIndex.u32RecDataCnt;
        for (idx = 0; idx < pstRecDataIdx->u32RecDataCnt; idx ++)
        {
            pstRecDataIdx->stRecData[idx].pDataAddr      = RawDataIndex.stRecData[idx].pDataAddr;
            pstRecDataIdx->stRecData[idx].u32DataPhyAddr = RawDataIndex.stRecData[idx].u32DataPhyAddr;
            pstRecDataIdx->stRecData[idx].u32Len         = RawDataIndex.stRecData[idx].u32Len;
        }

        pstRecDataIdx->u32IdxNum = RawDataIndex.u32IdxNum;
        for (idx = 0; idx < pstRecDataIdx->u32IdxNum; idx ++)
        {
            pstRecDataIdx->stIndex[idx].enFrameType      = RawDataIndex.stIndex[idx].enFrameType;
            pstRecDataIdx->stIndex[idx].u32PtsMs         = RawDataIndex.stIndex[idx].u32PtsMs;
            pstRecDataIdx->stIndex[idx].u64GlobalOffset  = RawDataIndex.stIndex[idx].u64GlobalOffset;
            pstRecDataIdx->stIndex[idx].u32FrameSize     = RawDataIndex.stIndex[idx].u32FrameSize;
            pstRecDataIdx->stIndex[idx].u32DataTimeMs    = RawDataIndex.stIndex[idx].u32DataTimeMs;
        }
    }

    return ret;
}

HI_S32 HI_UNF_DMX_ReleaseRecDataAndIndex(HI_HANDLE hRecChn, HI_UNF_DMX_REC_DATA_INDEX_S* pstRecDataIdx)
{
    HI_MPI_DMX_REC_DATA_INDEX_S RawDataIndex;
    HI_U32 idx;

    if (!pstRecDataIdx)
    {
        HI_ERR_DEMUX("Null Pointer!\n"); 
        return HI_ERR_DMX_NULL_PTR; 
    }

    /* covert UNF to MPI structure. */
    RawDataIndex.u32RecDataCnt = pstRecDataIdx->u32RecDataCnt;
    for (idx = 0; idx < RawDataIndex.u32RecDataCnt; idx ++)
    {
        RawDataIndex.stRecData[idx].pDataAddr      = pstRecDataIdx->stRecData[idx].pDataAddr;
        RawDataIndex.stRecData[idx].u32DataPhyAddr = pstRecDataIdx->stRecData[idx].u32DataPhyAddr;
        RawDataIndex.stRecData[idx].u32Len         = pstRecDataIdx->stRecData[idx].u32Len;
    }

    /* idx info unused when release. */
    RawDataIndex.u32IdxNum = 0; 
    
    return HI_MPI_DMX_ReleaseRecDataAndIndex(hRecChn, &RawDataIndex);
}

HI_S32 HI_UNF_DMX_GetRecBufferStatus(HI_HANDLE hRecChn, HI_UNF_DMX_RECBUF_STATUS_S *pstBufStatus)
{
    return HI_MPI_DMX_GetRecBufferStatus(hRecChn, pstBufStatus);
}

HI_S32 HI_UNF_DMX_RegisterCallback(HI_HANDLE hChannel, HI_UNF_DMX_CB_DESC_S *pstCbDesc)
{
    return HI_MPI_DMX_RegisterCallback(hChannel, pstCbDesc);
}

HI_S32 HI_UNF_DMX_UnRegisterCallback(HI_HANDLE hChannel)
{
    return HI_MPI_DMX_UnRegisterCallback(hChannel);
}

HI_S32 HI_UNF_DMX_Invoke(HI_UNF_DMX_INVOKE_TYPE_E enCmd, const HI_VOID *pCmdPara)
{
    return HI_MPI_DMX_Invoke(enCmd, pCmdPara);
}


