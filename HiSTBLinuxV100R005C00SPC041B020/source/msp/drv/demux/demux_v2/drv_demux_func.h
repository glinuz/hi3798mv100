/*******************************************************************************
*             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: drv_demux_func.h
* Description: Register define for HiPVRV200
*
* History:
* Version   Date              Author    DefectNum       Description
* main\1    2009-09-29    y00106256     NULL            create this file.
*******************************************************************************/
#ifndef __DRV_DEMUX_FUNC_H__
#define __DRV_DEMUX_FUNC_H__

#include "hi_drv_dev.h"
#include "drv_demux_define.h"
#include "hi_unf_demux.h"
#include "hi_mpi_demux.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


HI_S32  DMX_OsiInit(HI_VOID);
HI_S32  DMX_OsiDeInit(HI_VOID);
HI_S32  DMX_OsiGetPoolBufAddr(DMX_MMZ_BUF_S *PoolBuf);
HI_S32  DMX_OsiGetCapability(HI_UNF_DMX_CAPABILITY_S *Cap);
HI_VOID DMX_OsiSetNoPusiEn(HI_BOOL bNoPusiEn);
HI_VOID DMX_OsiSetTei(HI_U32 u32DmxId,HI_BOOL bTei);
HI_S32   DMX_OsiGetResumeCount(HI_U32 *pCount);

/* Port */
HI_S32 DMX_OsiTSOPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_TSO_PORT_ATTR_S *PortAttr);
HI_S32 DMX_OsiTSOPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_TSO_PORT_ATTR_S *PortAttr);

HI_S32 DMX_OsiTunerPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_PORT_ATTR_S *PortAttr);
HI_S32 __DMX_OsiTunerPortSetAttr(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_UNF_DMX_PORT_ATTR_S *PortAttr);
HI_S32 DMX_OsiTunerPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_PORT_ATTR_S *PortAttr);

HI_S32 DMX_OsiRamPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_PORT_ATTR_S *PortAttr);
HI_S32 __DMX_OsiRamPortSetAttr(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_UNF_DMX_PORT_ATTR_S *PortAttr);
HI_S32 DMX_OsiRamPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_PORT_ATTR_S *PortAttr);

HI_S32 DMX_OsiTunerPortGetPacketNum(const HI_U32 PortId, HI_U32 *TsPackCnt, HI_U32 *ErrTsPackCnt);
HI_S32 DMX_OsiRamPortGetPacketNum(const HI_U32 PortId, HI_U32 *TsPackCnt);

HI_S32 DMX_OsiTsBufferCreate(const HI_U32 PortId, const HI_UNF_DMX_TSBUF_ATTR_S *pstTsBufAttr, DMX_MMZ_BUF_S *TsBuf);
HI_S32 DMX_OsiTsBufferDestroy(const HI_U32 PortId);
HI_S32 DMX_OsiTsBufferGet(const HI_U32 PortId, const HI_U32 ReqLen, DMX_DATA_BUF_S *Buf, HI_U32 Timeout);
#ifdef DMX_TS_BUF_VERSION_2
HI_S32 DMX_OsiTsBufferPush(const HI_U32 PortId, DMX_DATA_BUF_S *pstData);
HI_S32 DMX_OsiTsBufferRelease(const HI_U32 PortId, DMX_DATA_BUF_S *pstData);
#else
static inline HI_S32 DMX_OsiTsBufferPush(const HI_U32 PortId, DMX_DATA_BUF_S *pstData)
{
    return HI_ERR_DMX_NOT_SUPPORT;
}
static inline HI_S32 DMX_OsiTsBufferRelease(const HI_U32 PortId, DMX_DATA_BUF_S *pstData)
{
    return HI_ERR_DMX_NOT_SUPPORT;
}
#endif
HI_S32 DMX_OsiTsBufferPut(const HI_U32 PortId, const HI_U32 DataLen, const HI_U32 StartPos);
HI_S32 DMX_OsiTsBufferReset(const HI_U32 PortId);
HI_S32 DMX_OsiTsBufferGetStatus(const HI_U32 PortId, HI_UNF_DMX_TSBUF_STATUS_S *Status);

/*SubDev*/
HI_S32  DMX_OsiAttachPort(const HI_U32 DmxId, const DMX_PORT_MODE_E PortMode, const HI_U32 PortId);
HI_S32  DMX_OsiDetachPort(const HI_U32 DmxId);
HI_S32  DMX_OsiGetPortId(const HI_U32 DmxId, DMX_PORT_MODE_E *PortMode, HI_U32 *PortId);

/* Filter */
HI_S32 DmxGetFltInstance(Dmx_Set_S *DmxSet, HI_U32 FltId);
HI_VOID DmxPutFltInstance(Dmx_Set_S *DmxSet, HI_U32 FltId);
HI_S32  DMX_OsiNewFilter(const HI_U32 DmxId, HI_U32 *FilterId);
HI_S32  DMX_OsiDeleteFilter(HI_U32 DmxId, const HI_U32 FilterId);
HI_S32  DMX_OsiSetFilterAttr(HI_U32 DmxId, const HI_U32 FilterId, const HI_UNF_DMX_FILTER_ATTR_S *FilterAttr);
HI_S32  DMX_OsiGetFilterAttr(HI_U32 DmxId, const HI_U32 FilterId, HI_UNF_DMX_FILTER_ATTR_S *FilterAttr);
HI_S32  DMX_OsiAttachFilter(HI_U32 DmxId, const HI_U32 FilterId, const HI_U32 ChanId);
HI_S32  DMX_OsiDetachFilter(HI_U32 DmxId, const HI_U32 FilterId, const HI_U32 ChanId);
HI_S32  DMX_OsiGetFilterChannel(HI_U32 DmxId, const HI_U32 FilterId, HI_U32 *ChanId);
HI_S32  DMX_OsiGetFreeFilterNum(const HI_U32 DmxId, HI_U32 *FreeCount);

// Channel
HI_S32  DmxGetChnInstance(Dmx_Set_S *DmxSet, HI_U32 ChanId);
HI_VOID DmxPutChnInstance(Dmx_Set_S *DmxSet, HI_U32 ChnId);
HI_S32  DMX_OsiCreateChannel(HI_U32 DmxId, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr, DMX_MMZ_BUF_S *ChanBuf, HI_U32 *ChanId);
HI_S32  DMX_OsiOpenPlyChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_S32  DMX_OsiOpenRecChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_S32  DMX_OsiOpenChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_S32  DMX_OsiClosePlyChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_S32  DMX_OsiCloseRecChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_S32  DMX_OsiCloseChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_S32  DMX_OsiDestroyPlyChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_S32  DMX_OsiDestroyRecChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_S32  DMX_OsiDestroyChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_S32  DMX_OsiAddChannelFunc(HI_U32 DmxId, HI_U32 ChanId, HI_U32 Pid, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr, DMX_MMZ_BUF_S *ChanBuf);
HI_S32  DMX_OsiDelChannelFunc(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_CHAN_OUTPUT_MODE_E OutputMode);
HI_S32  DMX_OsiGetChannelAttr(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr);
HI_S32  DMX_OsiSetChannelAttr(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr);
HI_S32  DMX_OsiGetChannelPid(HI_U32 DmxId, HI_U32 ChanId, HI_U32 *Pid);
HI_S32  DMX_OsiSetChannelPid(HI_U32 DmxId, HI_U32 ChanId, HI_U32 Pid);
HI_S32  DMX_OsiGetChannelStatus(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_CHAN_STATUS_E *ChanStatus);
HI_S32  DMX_OsiGetFreeChannelNum(HI_U32 u32DmxId, HI_U32 *pu32FreeCount);
HI_S32  DMX_OsiGetChannelScrambleFlag(HI_U32 DmxId, HI_U32 u32ChannelId, HI_UNF_DMX_SCRAMBLED_FLAG_E *penScrambleFlag);
HI_S32  DMX_OsiSetChannelEosFlag(HI_U32 DmxId, HI_U32 ChanId);
HI_S32  DMX_OsiGetChnDataFlag(Dmx_Set_S *DmxSet, HI_U32 u32ChanId);
HI_S32  DMX_OsiResetChannel(Dmx_Set_S *DmxSet, HI_U32 u32ChId, DMX_FLUSH_TYPE_E eFlushType);
HI_S32  DMX_OsiGetChannelTsCnt(HI_U32 DmxId, HI_U32 u32ChannelId, HI_U32* pu32TsCnt);
HI_S32  DMX_OsiSetChannelCCRepeat(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_CHAN_CC_REPEAT_SET_S * pstChCCReaptSet);


/* get channel ID by pid, return HI_ERR_DMX_NOMATCH_CHN when failed to find it */
HI_S32  DMX_OsiGetChannelId(HI_U32 u32DmxId, HI_U32 u32Pid, HI_U32 *pu32ChannelId);
HI_S32  DMX_OsiSelectDataFlag(HI_HANDLE *WatchHandleSet, HI_U32 WatchCnt, HI_HANDLE *ValidHandleSet, HI_U32 *ValidCnt, HI_U32 u32TimeOutMs);
HI_S32 DMX_OsiGetDataFlag(HI_U32 *ValidHandleSet, HI_U32 *ValidCnt, HI_U32 u32TimeOutMs);
    
HI_S32  DMX_OsiReadDataRequset(HI_U32 DmxId, HI_U32 u32ChId, HI_U32 u32AcqNum,
                               HI_U32 *pu32AcqedNum, DMX_UserMsg_S* psMsgList, HI_U32 u32TimeOutMs);
HI_S32  DMX_OsiReleaseReadData(HI_U32 DmxId, HI_U32 u32ChId, HI_U32 u32RelNum, DMX_UserMsg_S* psMsgList);

HI_S32  DMX_OsiReadEsRequest(HI_U32 DmxId, HI_U32 ChanId, DMX_Stream_S *EsData);
HI_S32  DMX_OsiReleaseReadEs(HI_U32 DmxId, HI_U32 ChanId, DMX_Stream_S *EsData);

HI_S32  DMX_OsiGetChanBufStatus(HI_U32 DmxId, HI_U32 ChanId, HI_MPI_DMX_BUF_STATUS_S *BufStatus);

/* get whether all the channel(96 channenels) channel data exist or not, returned by three u32 flag
    per-bit present one channel, just only return common channel, except the audio-video channel
 */
HI_S32  DMX_OsiGetAllDataFlag(HI_U32 *pu32Flag, HI_U32 u32TimeOutMs);

/* PCR */
HI_S32 DmxGetPcrInstance(Dmx_Set_S *DmxSet, HI_U32 PcrId);
HI_VOID DmxPutPcrInstance(Dmx_Set_S *DmxSet, HI_U32 PcrId);
HI_S32  DMX_OsiPcrChannelCreate(const HI_U32 DmxId, HI_U32 *PcrId);
HI_S32  DMX_OsiPcrChannelDestroy(const HI_U32 DmxId, const HI_U32 PcrId);
HI_S32  DMX_OsiPcrChannelSetPid(const HI_U32 DmxId, const HI_U32 PcrId, const HI_U32 PcrPid);
HI_S32  DMX_OsiPcrChannelGetPid(const HI_U32 DmxId, const HI_U32 PcrId, HI_U32 *PcrPid);
HI_S32  DMX_OsiPcrChannelGetClock(const HI_U32 DmxId, const HI_U32 PcrId, HI_U64 *PcrValue, HI_U64 *ScrValue);
HI_S32  DMX_OsiPcrChannelAttachSync(const HI_U32 DmxId, const HI_U32 PcrId, const HI_U32 SyncHadle);
HI_S32  DMX_OsiPcrChannelDetachSync(const HI_U32 DmxId, const HI_U32 PcrId);

/* REC */
HI_S32 DmxGetRecInstance(Dmx_Set_S *DmxSet, HI_U32 RecId);
HI_VOID DmxPutRecInstance(Dmx_Set_S *DmxSet, HI_U32 RecId);
HI_S32 DMX_DRV_REC_CreateChannel(HI_UNF_DMX_REC_ATTR_S *RecAttr, DMX_REC_TIMESTAMP_MODE_E enRecTimeStamp,HI_U32 *RecId, DMX_MMZ_BUF_S *RecBuf);
HI_S32  DMX_DRV_REC_DestroyChannel(HI_U32 DmxId, HI_U32 RecId);
HI_S32  DMX_DRV_REC_AddRecPid(HI_U32 DmxId, HI_U32 RecId, HI_U32 Pid, HI_U32 *ChanId);
HI_S32  DMX_DRV_REC_DelRecPid(HI_U32 DmxId, HI_U32 RecId, HI_U32 ChanId);
HI_S32  DMX_DRV_REC_DelAllRecPid(HI_U32 DmxId, HI_U32 RecId);
#ifdef DMX_REC_EXCLUDE_PID_SUPPORT
HI_S32  DMX_DRV_REC_AddExcludeRecPid(HI_U32 DmxId, HI_U32 RecId, HI_U32 Pid);
HI_S32  DMX_DRV_REC_DelExcludeRecPid(HI_U32 DmxId, HI_U32 RecId, HI_U32 Pid);
HI_S32  DMX_DRV_REC_DelAllExcludeRecPid(HI_U32 DmxId, HI_U32 RecId);
#else
static inline HI_S32  DMX_DRV_REC_AddExcludeRecPid(HI_U32 DmxId, HI_U32 RecId, HI_U32 Pid){return HI_ERR_DMX_NOT_SUPPORT;};
static inline HI_S32  DMX_DRV_REC_DelExcludeRecPid(HI_U32 DmxId, HI_U32 RecId, HI_U32 Pid){return HI_ERR_DMX_NOT_SUPPORT;};
static inline HI_S32  DMX_DRV_REC_DelAllExcludeRecPid(HI_U32 DmxId, HI_U32 RecId){return HI_ERR_DMX_NOT_SUPPORT;};
#endif
HI_S32  DMX_DRV_REC_GetTsCnt(HI_U32 DmxId, HI_U32 RecId,HI_U32* TSCnt);
HI_S32  DMX_DRV_REC_StartRecChn(HI_U32 DmxId, HI_U32 RecId);
HI_S32  DMX_DRV_REC_StopRecChn(HI_U32 DmxId, HI_U32 RecId);
HI_S32  DMX_DRV_REC_AcquireRecData(HI_U32 DmxId, HI_U32 RecId, HI_U32 *PhyAddr, HI_U8 **KerAddr, HI_U32 *Len, HI_U32 Timeout);
HI_S32  DMX_DRV_REC_ReleaseRecData(HI_U32 DmxId, HI_U32 RecId, HI_U32 PhyAddr, HI_U32 Len);
HI_S32  DMX_DRV_REC_AcquireRecIndex(HI_U32 DmxId, HI_U32 RecId, HI_MPI_DMX_REC_INDEX_S *RecIndex, HI_U32 Timeout);
HI_S32  DMX_DRV_REC_GetRecBufferStatus(HI_U32 DmxId, HI_U32 RecId, HI_UNF_DMX_RECBUF_STATUS_S *BufStatus);

HI_S32  DMX_OsiPeekDataRequest(HI_U32  DmxId, HI_U32 u32ChId, HI_U32 u32PeekLen, DMX_UserMsg_S* psMsgList);

HI_S32 DMX_DRV_REC_AcquireRecDataIndex(HI_U32 DmxId, HI_U32 RecId, HI_MPI_DMX_REC_DATA_INDEX_S *pstRecDataIdx);
HI_S32 DMX_DRV_REC_ReleaseRecDataIndex(HI_U32 DmxId, HI_U32 RecId, HI_MPI_DMX_REC_DATA_INDEX_S *pstRecDataIdx);

#ifdef HI_DEMUX_PROC_SUPPORT

typedef struct
{
    HI_U32 ReleaseErrCount;
    HI_U32 ReleaseErrLastChan;
    HI_U32 ReleaseErrLastOQId;
    /*PES error*/
    HI_U32 PesErrCount;
    HI_U64 PesErrLastPCR;

    /*poolbuf err, may be caused by multi-process */
    HI_U32 ResetPoolbufCount;
} DMX_Proc_Err_Info_S;


typedef struct
{
    HI_S32              MaxRef;
    HI_S32              Ref;
    DMX_Proc_Err_Info_S ErrInfo;
    /*some other infor can be added ,such as ,clk ,version and so on*/
} DMX_Proc_Global_Info_S;


typedef struct
{
    HI_U32  PhyAddr;
    HI_U32  BufSize;
    HI_U32  UsedSize;
    HI_U32  Read;
    HI_U32  Write;
    HI_U32  GetCount;
    HI_U32  GetValidCount;
    HI_U32  PutCount;
} DMX_Proc_RamPort_BufInfo_S;

typedef struct
{
    HI_BOOL DoScram;
    HI_U32  KeyId;
}DMX_Proc_ChanDesc_S;

typedef struct
{
    HI_U32  DescDepth;
    HI_U32  DescRead;
    HI_U32  DescWrite;
    HI_U32  BlockSize;
    HI_U32  Overflow;
} DMX_Proc_ChanBuf_S;

typedef struct
{
    HI_UNF_DMX_REC_TYPE_E   RecType;
    HI_BOOL                 Descramed;
    HI_U32                  BlockCnt;
    HI_U32                  BlockSize;
    HI_U32                  BufRead;
    HI_U32                  BufWrite;
    HI_U32                  RecStatus;
    HI_U32                  Overflow;
} DMX_Proc_Rec_BufInfo_S;

typedef struct
{
    HI_UNF_DMX_REC_INDEX_TYPE_E IndexType;
    HI_U32                      FqId;
    HI_U32                      OqId;
    HI_U32                      ScdId;
    HI_U32                      IndexPid;
    HI_U32                      BlockCnt;
    HI_U32                      BlockSize;
    HI_U32                      BufRead;
    HI_U32                      BufWrite;
    HI_U32                  Overflow;
} DMX_Proc_RecScd_BufInfo_S;

HI_S32  DMX_OsiRamPortGetBufInfo(HI_U32 PortId, DMX_Proc_RamPort_BufInfo_S *BufInfo);
HI_S32  DMX_OsiRamPortGetDescInfo(HI_U32 PortId, DMX_Proc_RamPort_DescInfo_S *DescInfo);
HI_S32  DMX_OsiRamPortGetBPStatus(HI_U32 PortId, DMX_Proc_RamPort_BPStatus_S *BPStatus);

HI_S32  DMX_OsiGetChannelDescStatus(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_Proc_ChanDesc_S *DescInfo);

HI_S32  DMX_OsiGetChanBufProc(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_Proc_ChanBuf_S *BufInfo);

HI_S32  DMX_OsiGetDmxRecProc(Dmx_Set_S *DmxSet, HI_U32 RecId, DMX_Proc_Rec_BufInfo_S *RecBufInfo);
HI_S32  DMX_OsiGetDmxRecScdProc(Dmx_Set_S *DmxSet, HI_U32 RecId, DMX_Proc_RecScd_BufInfo_S *ScdBufInfo);

HI_S32  DMX_OsiGetDmxClkProc(HI_U32 *DmxClk);

HI_S32  DMX_OsiSaveDmxTs_Start(HI_U32 u32RecId, HI_U32 u32DmxId, HI_U32 u32RecDmxId);

#endif

HI_S32  DMX_OsiDeviceInit(HI_VOID);
HI_VOID DMX_OsiDeviceDeInit(HI_VOID);
HI_U32  DMX_OsiGetChType(HI_U32 DmxId, HI_U32 u32ChId);

HI_S32  DMX_OsiSuspend(PM_BASEDEV_S *himd, pm_message_t state);
HI_S32  DMX_OsiResume(PM_BASEDEV_S *himd);

#if defined(DMX_TAGPORT_CNT) && (DMX_TAGPORT_CNT > 0)
HI_S32  DMX_OsiTagPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_TAG_ATTR_S *pstAttr);
HI_S32  DMX_OsiTagPortSetAttr(const HI_U32 DmxId, const HI_UNF_DMX_TAG_ATTR_S *pstAttr);
HI_S32 DMX_OsiGetTagPortAttr(const HI_U32 TagPortId, HI_UNF_DMX_TAG_ATTR_S *pstAttr);
HI_VOID DMX_OsiResumeTag(HI_VOID);
#endif

#ifdef RMX_SUPPORT
HI_S32 RmxGetChnInstance(Dmx_Set_S *DmxSet, HI_U32 ChanId);
HI_VOID RmxPutChnInstance(Dmx_Set_S *DmxSet, HI_U32 ChanId);
HI_S32 RMX_OsiCreate(HI_MPI_RMX_ATTR_S *Attr, HI_U32 *ChanId);
HI_S32 RMX_OsiDestroy(HI_U32 RmxId, HI_U32 ChanId);
HI_S32 RMX_OsiGetAttr(HI_U32 RmxId, HI_U32 ChanId, HI_MPI_RMX_ATTR_S *Attr);
HI_S32 RMX_OsiSetAttr(HI_U32 RmxId, HI_U32 ChanId, HI_MPI_RMX_ATTR_S *Attr);
HI_S32 RMX_OsiStart(HI_U32 RmxId, HI_U32 ChanId);
HI_S32 __RMX_OsiStop(HI_U32 RmxId, HI_U32 ChanId);
HI_S32 RMX_OsiStop(HI_U32 RmxId, HI_U32 ChanId);
HI_S32 RmxGetPumpInstance(Dmx_Set_S *DmxSet, HI_U32 PumpId);
HI_VOID RmxPutPumpInstance(Dmx_Set_S *DmxSet, HI_U32 PumpId);
HI_S32 RMX_OsiAddPump(HI_U32 RmxId, HI_U32 ChanId, HI_MPI_RMX_PUMP_ATTR_S *Attr, HI_U32 *PumpId);
HI_S32 __RMX_OsiDelPump(HI_U32 RmxId, HI_U32 ChanId, HI_U32 PumpId);
HI_S32 RMX_OsiDelPump(HI_U32 RmxId, HI_U32 ChanId, HI_U32 PumpId);
HI_S32 RMX_OsiGetPumpDefaultAttr(HI_MPI_RMX_PUMP_ATTR_S *Attr);
HI_S32 RMX_OsiGetPumpAttr(HI_U32 RmxId, HI_U32 ChanId, HI_U32 PumpId, HI_MPI_RMX_PUMP_ATTR_S *Attr);
HI_S32 RMX_OsiSetPumpAttr(HI_U32 RmxId, HI_U32 ChanId, HI_U32 PumpId, HI_MPI_RMX_PUMP_ATTR_S *Attr);
HI_VOID RmxEnFqBPIp(Dmx_Set_S *DmxSet, HI_U32 RmxId, HI_U32 FqId);
HI_VOID RmxDisFqBPIp(Dmx_Set_S *DmxSet, HI_U32 RmxId, HI_U32 FqId);
HI_VOID RmxChkFqBpIp(Dmx_Set_S *DmxSet, HI_U32 RmxId, HI_U32 FqId);
HI_VOID RemuxOverflowMonitor(Dmx_Set_S *DmxSet);
#else
static inline HI_S32 RMX_OsiCreate(HI_MPI_RMX_ATTR_S *Attr, HI_U32 *ChanId){return HI_ERR_DMX_NOT_SUPPORT;}
static inline HI_S32 RMX_OsiDestroy(HI_U32 RmxId, HI_U32 ChanId){return HI_ERR_DMX_NOT_SUPPORT;}
static inline HI_S32 RMX_OsiGetAttr(HI_U32 RmxId, HI_U32 ChanId, HI_MPI_RMX_ATTR_S *Attr){return HI_ERR_DMX_NOT_SUPPORT;};
static inline HI_S32 RMX_OsiSetAttr(HI_U32 RmxId, HI_U32 ChanId, HI_MPI_RMX_ATTR_S *Attr){return HI_ERR_DMX_NOT_SUPPORT;}
static inline HI_S32 RMX_OsiStart(HI_U32 RmxId, HI_U32 ChanId){return HI_ERR_DMX_NOT_SUPPORT;}
static inline HI_S32 RMX_OsiStop(HI_U32 RmxId, HI_U32 ChanId){return HI_ERR_DMX_NOT_SUPPORT;}
static inline HI_S32 RMX_OsiAddPump(HI_U32 RmxId, HI_U32 ChanId, HI_MPI_RMX_PUMP_ATTR_S *Attr, HI_U32 *PumpId){return HI_ERR_DMX_NOT_SUPPORT;}
static inline HI_S32 RMX_OsiDelPump(HI_U32 RmxId, HI_U32 ChanId, HI_U32 PumpId){return HI_ERR_DMX_NOT_SUPPORT;}
static inline HI_S32 RMX_OsiGetPumpDefaultAttr(HI_MPI_RMX_PUMP_ATTR_S *Attr){return HI_ERR_DMX_NOT_SUPPORT;}
static inline HI_S32 RMX_OsiGetPumpAttr(HI_U32 RmxId, HI_U32 ChanId, HI_U32 PumpId, HI_MPI_RMX_PUMP_ATTR_S *Attr){return HI_ERR_DMX_NOT_SUPPORT;}
static inline HI_S32 RMX_OsiSetPumpAttr(HI_U32 RmxId, HI_U32 ChanId, HI_U32 PumpId, HI_MPI_RMX_PUMP_ATTR_S *Attr){return HI_ERR_DMX_NOT_SUPPORT;}
static inline HI_VOID RmxEnFqBPIp(Dmx_Set_S *DmxSet, HI_U32 RmxId, HI_U32 FqId){};
static inline HI_VOID RmxDisFqBPIp(Dmx_Set_S *DmxSet, HI_U32 RmxId, HI_U32 FqId){};
static inline HI_VOID RmxChkFqBpIp(Dmx_Set_S *DmxSet, HI_U32 RmxId, HI_U32 FqId){};
static inline HI_VOID RemuxOverflowMonitor(Dmx_Set_S *DmxSet){};
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  // __DRV_DEMUX_FUNC_H__

