/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hi_drv_demux.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2013/04/10
 Description   :
******************************************************************************/

#ifndef __HI_DRV_DEMUX_H__
#define __HI_DRV_DEMUX_H__

#include "hi_type.h"
#include "hi_unf_demux.h"
#include "hi_mpi_demux.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    DMX_PORT_MODE_TUNER = 0,
    DMX_PORT_MODE_RAM,
    DMX_PORT_MODE_TAG,
    DMX_PORT_MODE_RMX,
    DMX_PORT_MODE_BUTT
} DMX_PORT_MODE_E;

typedef struct
{
    HI_U8   *VirAddr;         /* Virtual address of a buffer. */
    HI_U32  PhyAddr;        /* Physical address of a buffer. */
    HI_U32  Size;               /* Buffer size, in the unit of byte. */
    DMX_BUF_FLAG_E Flag;
}DMX_MMZ_BUF_S;

typedef struct
{
    HI_U8    *BufKerAddr;
    HI_U32  BufPhyAddr;
    HI_U32  BufLen;
} DMX_DATA_BUF_S;

typedef struct
{
    HI_U8    *pu8BufVirAddr;
    HI_U32  u32BufPhyAddr;
    HI_U32  u32BufLen;
    HI_U32  u32PtsMs;
    HI_U32  u32Index;
    HI_U32  u32DispTime;//add for pvr
    HI_U32  u32DispEnableFlag;
    HI_U32  u32DispFrameDistance;
    HI_U32  u32DistanceBeforeFirstFrame;
    HI_U32  u32GopNum;
} DMX_Stream_S;

typedef struct hiDMX_UserMsg_S
{
    HI_U32                  u32BufStartAddr;
    HI_U32                  u32MsgLen;
    HI_UNF_DMX_DATA_TYPE_E  enDataType;  /**< the data packet type */
} DMX_UserMsg_S;

typedef struct hiDMX_BUF_STATUS_S
{
    HI_U32 u32BufSize;  /*buffer size */
    HI_U32 u32UsedSize; /* buffer used size */
    HI_U32 u32BufRptr;  /*buffer read pointer */
    HI_U32 u32BufWptr;/*buffer written pointer */
} HI_DRV_DMX_BUF_STATUS_S;

HI_S32  HI_DRV_DMX_BasicInit(HI_VOID);
HI_S32  HI_DRV_DMX_Init(HI_VOID);
HI_VOID HI_DRV_DMX_DeInit(HI_VOID);

HI_S32  HI_DRV_DMX_Open(HI_VOID *file);
HI_S32  HI_DRV_DMX_Close(HI_VOID *file);

HI_S32  HI_DRV_DMX_GetPoolBufAddr(DMX_MMZ_BUF_S *PoolBuf);
HI_S32  HI_DRV_DMX_GetCapability(HI_UNF_DMX_CAPABILITY_S *Cap);
HI_S32 HI_DRV_DMX_SetPusi(HI_BOOL bCheckPusi);
HI_S32 HI_DRV_DMX_SetTei(HI_UNF_DMX_TEI_SET_S *pstTei);
HI_S32 HI_DRV_DMX_TSIAttachTSO(HI_UNF_DMX_TSI_ATTACH_TSO_S *pstTSIAttachTSO);
HI_BOOL HI_DRV_DMX_IsTSIAttachTSO(HI_U32 PortId, HI_UNF_DMX_TSO_PORT_E* TSO);
HI_S32  HI_DRV_DMX_GetResumeCount(HI_U32 *pCount);

HI_S32  HI_DRV_DMX_TSOPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_TSO_PORT_ATTR_S *PortAttr);
HI_S32  HI_DRV_DMX_TSOPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_TSO_PORT_ATTR_S *PortAttr);

HI_S32  HI_DRV_DMX_TunerPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_PORT_ATTR_S *PortAttr);
HI_S32  HI_DRV_DMX_TunerPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_PORT_ATTR_S *PortAttr);

HI_S32  HI_DRV_DMX_RamPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_PORT_ATTR_S *PortAttr);
HI_S32  HI_DRV_DMX_RamPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_PORT_ATTR_S *PortAttr);

HI_S32  HI_DRV_DMX_GetTagAttr(const HI_U32 DmxId, HI_UNF_DMX_TAG_ATTR_S *pstAttr);
HI_S32  HI_DRV_DMX_SetTagAttr(const HI_U32 DmxId, const HI_UNF_DMX_TAG_ATTR_S *pstAttr);
HI_S32  HI_DRV_DMX_GetTagPortId(HI_U32 DmxId, HI_U32 *TagPortId);


HI_S32  HI_DRV_DMX_AttachTunerPort(HI_U32 DmxId, HI_U32 PortId);
HI_S32  HI_DRV_DMX_AttachRamPort(HI_U32 DmxId, HI_U32 PortId);
HI_S32  HI_DRV_DMX_AttachPort(HI_U32 DmxId, DMX_PORT_MODE_E PortMode, HI_U32 PortId);
HI_S32  HI_DRV_DMX_DetachPort(HI_U32 DmxId);

HI_S32  HI_DRV_DMX_GetPortId(HI_U32 DmxId, DMX_PORT_MODE_E *PortMode, HI_U32 *PortId);

HI_S32  HI_DRV_DMX_TunerPortGetPacketNum(const HI_U32 PortId, HI_U32 *TsPackCnt, HI_U32 *ErrTsPackCnt);
HI_S32  HI_DRV_DMX_RamPortGetPacketNum(const HI_U32 PortId, HI_U32 *TsPackCnt);

HI_S32  HI_DRV_DMX_CreateTSBuffer(const HI_U32 PortId, const HI_UNF_DMX_TSBUF_ATTR_S *pstTsBufAttr, DMX_MMZ_BUF_S *TsBuf, const HI_VOID *file);
HI_S32  HI_DRV_DMX_DestroyTSBuffer(const HI_U32 PortId);
HI_S32  HI_DRV_DMX_GetTSBuffer(const HI_U32 PortId, const HI_U32 ReqLen, DMX_DATA_BUF_S *Data, const HI_U32 TimeoutMs);
HI_S32  HI_DRV_DMX_PushTSBuffer(const HI_U32 PortId, DMX_DATA_BUF_S *pstData);
HI_S32  HI_DRV_DMX_ReleaseTSBuffer(const HI_U32 PortId, DMX_DATA_BUF_S *pstData);
HI_S32  HI_DRV_DMX_PutTSBuffer(const HI_U32 PortId, const HI_U32 DataLen, const HI_U32 StartPos);
HI_S32  HI_DRV_DMX_ResetTSBuffer(const HI_U32 PortId);
HI_S32  HI_DRV_DMX_GetTSBufferStatus(const HI_U32 PortId, HI_UNF_DMX_TSBUF_STATUS_S *Status);

HI_S32  HI_DRV_DMX_CreateChannel(HI_U32 u32DmxId, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr,
                           HI_HANDLE *phChannel, DMX_MMZ_BUF_S *pstChBuf, HI_VOID *file);
HI_S32  HI_DRV_DMX_CreateChannelWithPID(HI_U32 u32DmxId, HI_U32 Pid, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr,
                           HI_HANDLE *phChannel, DMX_MMZ_BUF_S *pstChBuf, HI_VOID *file);
HI_S32  HI_DRV_DMX_DestroyChannel(HI_HANDLE hChannel);
HI_S32  HI_DRV_DMX_GetChannelAttr(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr);
HI_S32  HI_DRV_DMX_SetChannelAttr(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr);
HI_S32  HI_DRV_DMX_SetChannelPID(HI_HANDLE hChannel, HI_U32 u32Pid);
HI_S32  HI_DRV_DMX_GetChannelPID(HI_HANDLE hChannel, HI_U32 *pu32Pid);
HI_S32  HI_DRV_DMX_OpenChannel(HI_HANDLE hChannel);
HI_S32  HI_DRV_DMX_CloseChannel(HI_HANDLE hChannel);
HI_S32  HI_DRV_DMX_GetChannelStatus(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_STATUS_S *pstStatus);
HI_S32  HI_DRV_DMX_GetChannelHandle(HI_U32 u32DmxId, HI_U32 u32Pid, HI_HANDLE *phChannel);
HI_S32  HI_DRV_DMX_GetFreeChannelCount(HI_U32 u32DmxId, HI_U32 *pu32FreeCount);
HI_S32  HI_DRV_DMX_GetScrambledFlag(HI_HANDLE hChannel, HI_UNF_DMX_SCRAMBLED_FLAG_E *penScrambleFlag);
HI_S32  HI_DRV_DMX_GetChannelTsCount(HI_HANDLE hChannel, HI_U32* pu32TsCnt);
HI_S32  HI_DRV_DMX_SetChannelEosFlag(HI_HANDLE hChannel);
HI_S32  HI_DRV_DMX_SetChannelCCRepeat(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_CC_REPEAT_SET_S *pstChCCReaptSet);


HI_S32  HI_DRV_DMX_CreateFilter(HI_U32 DmxId, HI_UNF_DMX_FILTER_ATTR_S *FilterAttr, HI_HANDLE *Filter, HI_VOID *file);
HI_S32  HI_DRV_DMX_DestroyFilter(HI_HANDLE Filter);
HI_S32  HI_DRV_DMX_DestroyAllFilter(HI_HANDLE Channel);
HI_S32  HI_DRV_DMX_SetFilterAttr(HI_HANDLE Filter, HI_UNF_DMX_FILTER_ATTR_S *FilterAttr);
HI_S32  HI_DRV_DMX_GetFilterAttr(HI_HANDLE Filter, HI_UNF_DMX_FILTER_ATTR_S *FilterAttr);
HI_S32  HI_DRV_DMX_AttachFilter(HI_HANDLE Filter, HI_HANDLE Channel);
HI_S32  HI_DRV_DMX_DetachFilter(HI_HANDLE Filter, HI_HANDLE Channel);
HI_S32  HI_DRV_DMX_GetFilterChannelHandle(HI_HANDLE Filter, HI_HANDLE *Channel);
HI_S32  HI_DRV_DMX_GetFreeFilterCount(HI_U32 DmxId, HI_U32 *FreeCount);
HI_S32  HI_DRV_DMX_GetDataHandle(HI_U32 *pu32ValidChannel, HI_U32 *pu32ValidNum, HI_U32 u32TimeOutMs);
HI_S32  HI_DRV_DMX_SelectDataHandle(HI_U32 *pu32WatchChannel, HI_U32 u32WatchNum, HI_U32 *pu32ValidChannel, HI_U32 *pu32ValidNum, HI_U32 u32TimeOutMs);
HI_S32  HI_DRV_DMX_AcquireBuf(HI_HANDLE hChannel, HI_U32 u32AcquireNum,
                           HI_U32 *pu32AcquiredNum, DMX_UserMsg_S *pstBuf,
                           HI_U32 u32TimeOutMs);
HI_S32  HI_DRV_DMX_ReleaseBuf(HI_HANDLE hChannel, HI_U32 u32ReleaseNum, DMX_UserMsg_S *pstBuf);
HI_S32  HI_DRV_DMX_PeekBuf(HI_HANDLE hChannel, HI_U32 u32PeekLen,DMX_UserMsg_S *pstBuf);

HI_S32  HI_DRV_DMX_CreatePcrChannel(const HI_U32 DmxId, HI_U32 *PcrHandle, const HI_VOID *file);
HI_S32  HI_DRV_DMX_DestroyPcrChannel(const HI_U32 PcrHandle);
HI_S32  HI_DRV_DMX_PcrPidSet(const HI_U32 PcrHandle, const HI_U32 PcrPid);
HI_S32  HI_DRV_DMX_PcrPidGet(const HI_U32 PcrHandle, HI_U32 *PcrPid);
HI_S32  HI_DRV_DMX_PcrScrGet(const HI_U32 PcrHandle, HI_U64 *PcrValue, HI_U64 *ScrValue);
HI_S32  HI_DRV_DMX_PcrSyncAttach(const HI_U32 PcrHandle, const HI_U32 SyncHandle);
HI_S32  HI_DRV_DMX_PcrSyncDetach(const HI_U32 PcrHandle);

HI_S32  HI_DRV_DMX_AcquireEs(HI_HANDLE hChannel, DMX_Stream_S *pEsBuf);
HI_S32  HI_DRV_DMX_ReleaseEs(HI_HANDLE hChannel, DMX_Stream_S *pEsBuf);
HI_S32  HI_DRV_DMX_GetPESBufferStatus(HI_HANDLE ChanHandle, HI_MPI_DMX_BUF_STATUS_S *BufStatus);

HI_S32  HI_DRV_DMX_CreateRecChn(
        HI_UNF_DMX_REC_ATTR_S  *RecAttr,
        HI_HANDLE              *RecHandle,
        DMX_MMZ_BUF_S    *RecBuf,
        HI_VOID                *file
    );
HI_S32  HI_DRV_DMX_DestroyRecChn(HI_HANDLE RecHandle);
HI_S32  HI_DRV_DMX_AddRecPid(HI_HANDLE RecHandle, HI_U32 Pid, HI_HANDLE *ChanHandle, HI_VOID *file);
HI_S32  HI_DRV_DMX_DelRecPid(HI_HANDLE RecHandle, HI_HANDLE ChanHandle);
HI_S32  HI_DRV_DMX_DelAllRecPid(HI_HANDLE RecHandle);
HI_S32  HI_DRV_DMX_GetRecTsCnt(HI_HANDLE RecHandle,HI_U32* TSCnt);
HI_S32  HI_DRV_DMX_AddExcludeRecPid(HI_HANDLE RecHandle, HI_U32 Pid);
HI_S32  HI_DRV_DMX_DelExcludeRecPid(HI_HANDLE RecHandle, HI_U32 Pid);
HI_S32  HI_DRV_DMX_DelAllExcludeRecPid(HI_HANDLE RecHandle);
HI_S32  HI_DRV_DMX_StartRecChn(HI_HANDLE RecHandle);
HI_S32  HI_DRV_DMX_StopRecChn(HI_HANDLE RecHandle);
HI_S32  HI_DRV_DMX_AcquireRecData(HI_HANDLE RecHandle, HI_MPI_DMX_REC_DATA_S *RecData, HI_U32 Timeout);
HI_S32  HI_DRV_DMX_ReleaseRecData(HI_HANDLE RecHandle, const HI_MPI_DMX_REC_DATA_S *RecData);
HI_S32  HI_DRV_DMX_AcquireRecIndex(HI_HANDLE RecHandle, HI_MPI_DMX_REC_INDEX_S *RecIndex, HI_U32 Timeout);
HI_S32  HI_DRV_DMX_GetRecBufferStatus(HI_HANDLE RecHandle, HI_UNF_DMX_RECBUF_STATUS_S *BufStatus);


HI_S32 HI_DRV_DMX_AcquireRecDataIndex(HI_HANDLE RecHandle, HI_MPI_DMX_REC_DATA_INDEX_S *RecDataIndex);
HI_S32 HI_DRV_DMX_ReleaseRecDataIndex(HI_HANDLE RecHandle, HI_MPI_DMX_REC_DATA_INDEX_S *RecDataIndex);

HI_S32 HI_DRV_RMX_Create(HI_MPI_RMX_ATTR_S *Attr, HI_HANDLE *Handle);
HI_S32 HI_DRV_RMX_Destroy(HI_HANDLE Handle);
HI_S32 HI_DRV_RMX_GetAttr(HI_HANDLE Handle, HI_MPI_RMX_ATTR_S *Attr);
HI_S32 HI_DRV_RMX_SetAttr(HI_HANDLE Handle, HI_MPI_RMX_ATTR_S *Attr);
HI_S32 HI_DRV_RMX_Start(HI_HANDLE Handle);
HI_S32 HI_DRV_RMX_Stop(HI_HANDLE Handle);
HI_S32 HI_DRV_RMX_AddPump(HI_HANDLE RmxHandle, HI_MPI_RMX_PUMP_ATTR_S *Attr, HI_HANDLE *PumpHandle);
HI_S32 HI_DRV_RMX_DelPump(HI_HANDLE PumpHandle);
HI_S32 HI_DRV_RMX_GetPumpDefaultAttr(HI_MPI_RMX_PUMP_ATTR_S *Attr);
HI_S32 HI_DRV_RMX_GetPumpAttr(HI_HANDLE PumpHandle, HI_MPI_RMX_PUMP_ATTR_S *Attr);
HI_S32 HI_DRV_RMX_SetPumpAttr(HI_HANDLE PumpHandle, HI_MPI_RMX_PUMP_ATTR_S *Attr);

#ifdef __cplusplus
}
#endif

#endif  // __HI_DRV_DEMUX_H__

