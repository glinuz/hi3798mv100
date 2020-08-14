/*******************************************************************************
*             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hal_demux.h
* Description: Register define for HiPVRV200
*
* History:
* Version   Date              Author    DefectNum       Description
* main\1    2009-09-29    y00106256     NULL            create this file.
*******************************************************************************/
#ifndef __HAL_DEMUX_H__
#define __HAL_DEMUX_H__

#include "drv_demux_define.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    DMX_CHAN_DATA_TYPE_SEC = 0,
    DMX_CHAN_DATA_TYPE_PES = 1,
} DMX_CHAN_DATA_TYPE_E;

typedef enum
{
    DMX_REC_TYPE_NONE       = 0,
    DMX_REC_TYPE_DESCRAM_TS = 1,
    DMX_REC_TYPE_PES        = 2,
    DMX_REC_TYPE_SCRAM_TS   = 3,
    DMX_REC_TYPE_ALL_TS     = 4,
    DMX_REC_TYPE_UNDEF
} DMX_REC_TYPE_E;

HI_VOID DmxHalDvbPortSetAttr(Dmx_Set_S *DmxSet,
        HI_U32                  RawPortId,
        HI_UNF_DMX_PORT_TYPE_E  PortType,
        HI_U32                  SyncOn,
        HI_U32                  SyncOff,
        HI_U32                  TunerInClk,
        HI_U32                  BitSelector
    );

HI_U32  DmxHalGetDmxClk(Dmx_Set_S *DmxSet);
HI_VOID DmxHalTSOPortSetAttr(Dmx_Set_S *DmxSet, HI_U32 PortId,HI_UNF_DMX_TSO_PORT_ATTR_S *PortAttr);

HI_S32  DmxHalDvbPortGetShareClk(HI_U32 PortID, HI_UNF_DMX_PORT_E *pSerialPortShareClk);
HI_VOID DmxHalDvbPortSetShareClk(HI_U32 PortID, HI_UNF_DMX_PORT_E SerialPortShareClk);
HI_VOID DmxHalDvbPortSetClkInPol(HI_U32 RawPortId, HI_BOOL bClkInPol);
HI_VOID DmxHalDvbPortSetTsCountCtrl(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_U32 option);
HI_U32  DmxHalDvbPortGetTsPackCount(Dmx_Set_S *DmxSet, HI_U32 RawPortId);
HI_VOID DmxHalDvbPortSetErrTsCountCtrl(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_U32 option);
HI_U32  DmxHalDvbPortGetErrTsPackCount(Dmx_Set_S *DmxSet, HI_U32 RawPortId);

HI_VOID DmxHalIPPortSetAttr(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_UNF_DMX_PORT_TYPE_E PortType, HI_U32 SyncOn, HI_U32 SyncOff);
HI_VOID DmxHalIPPortSetSyncLen(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_U32 SyncLen1, HI_U32 SyncLen2);

#ifdef DMX_RAM_PORT_AUTO_SCAN_SUPPORT
HI_VOID DmxHalIPPortSetAutoScanRegion(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_U32 len, HI_U32 step);
#endif

HI_VOID DmxHalIPPortSetTsCountCtrl(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_BOOL enable);
HI_U32  DmxHalIPPortGetTsPackCount(Dmx_Set_S *DmxSet, HI_U32 PortId);
HI_VOID __DmxHalIPPortStartStream(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_BOOL Enable);
HI_VOID DmxHalIPPortStartStream(Dmx_Set_S *DmxSet, const HI_U32 PortId, const HI_BOOL Enable);
HI_VOID __DmxHalIPPortDescSet(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_U32 StartAddr, HI_U32 Depth);
HI_VOID DmxHalIPPortDescSet(Dmx_Set_S *DmxSet, HI_U32 PortId, HI_U32 StartAddr, HI_U32 Depth);
HI_VOID DmxHalIPPortDescAdd(Dmx_Set_S *DmxSet, const HI_U32 PortId, const HI_U32 DescNum);
HI_VOID __DmxHalIPPortRateSet(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_U32 Rate);
HI_VOID DmxHalIPPortRateSet(Dmx_Set_S *DmxSet, HI_U32 PortId, HI_U32 Rate);

HI_VOID DmxHalGetChannelTSCount(HI_U32 ChanId, HI_U32 *ChanTsCount, HI_U32* OQTsCount);
HI_VOID DmxHalSetChannelDataType(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_CHAN_DATA_TYPE_E DataType);
HI_VOID DmxHalSetChannelAFMode(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_Ch_AFMode_E eAfMode);
HI_VOID DmxHalSetChannelCRCMode(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_UNF_DMX_CHAN_CRC_MODE_E CrcMode);
HI_VOID DmxHalSetChannelCCDiscon(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 DiscardFlag);
HI_VOID DmxHalSetChannelPusiCtrl(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 PusiCtrl);
HI_VOID DmxHalSetChannelCCRepeatCtrl(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 CCRepeatCtrl);
HI_VOID DmxHalSetChannelTsPostMode(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 TsPost);
HI_VOID DmxHalSetChannelTsPostThresh(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 Threshold);
HI_VOID DmxHalSetChannelAttr(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_Ch_ATTR_E echattr);
HI_VOID DmxHalSetChannelFltMode(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_BOOL bEnable);
HI_VOID DmxHalGetChannelPlayDmxid(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 *dmxid);
HI_VOID DmxHalSetChannelPlayDmxid(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 dmxid);
HI_VOID DmxHalSetChannelRecDmxid(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 dmxid);
HI_VOID DmxHalSetChannelPid(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 pid);
#ifdef DMX_DUP_PID_CHANNEL_SUPPORT
HI_VOID DmxHalEnablePidCopy(Dmx_Set_S *DmxSet);
#else
static inline HI_VOID DmxHalEnablePidCopy(Dmx_Set_S *DmxSet){};
#endif
HI_VOID DmxHalSetChannelRecBufId(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 bufid);
HI_VOID DmxHalSetShareRecChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_BOOL bEnable);
HI_VOID DmxHalSetChannelPlayBufId(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 bufid);
HI_VOID __DmxHalSetDemuxPortId(Dmx_Set_S *DmxSet, HI_U32 RawDmxId, DMX_PORT_MODE_E PortMode, HI_U32 PortId);
HI_VOID DmxHalSetDemuxPortId(Dmx_Set_S *DmxSet, HI_U32 DmxId, DMX_PORT_MODE_E PortMode, HI_U32 PortId);
HI_VOID DmxHalSetDataFakeMod(Dmx_Set_S *DmxSet, HI_BOOL bFakeEn);
HI_VOID DmxHalSetRecType(Dmx_Set_S *DmxSet, HI_U32 DmxId, DMX_REC_TYPE_E RecType);
HI_VOID DmxHalFlushChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_FLUSH_TYPE_E FlushType);
HI_BOOL DmxHalIsFlushChannelDone(Dmx_Set_S *DmxSet);

#ifndef DMX_SCD_VERSION_2
static inline HI_VOID DmxHalSetRecBuf(Dmx_Set_S *DmxSet, HI_U32 RecId) {};
static inline HI_VOID DmxHalSetDmxRecFlag(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 RecId) { };
static inline HI_VOID DmxHalClearDmxRecFlag(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 RecId) { };
#else
HI_VOID DmxHalSetRecBuf(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 OqId);
HI_VOID DmxHalSetDmxRecFlag(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 RecId);
HI_VOID DmxHalClearDmxRecFlag(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 RecId);
#endif
HI_VOID DmxHalSetDmxRecBufId(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 OqId);
HI_VOID DmxHalSetSpsRefRecCh(Dmx_Set_S *DmxSet, HI_U32 RawDmxId, HI_U32 ChanId);
HI_VOID DmxHalSetSpsPauseType(Dmx_Set_S *DmxSet, HI_U32 u32DmxId, HI_U32 type);
HI_VOID DmxHalSetFilter(Dmx_Set_S *DmxSet, HI_U32 FilterId, HI_U32 Depth, HI_U8 Content, HI_BOOL bReverse, HI_U8 Mask);
HI_VOID DmxHalAttachFilter(Dmx_Set_S *DmxSet, HI_U32 FilterId, HI_U32 ChanId);
HI_VOID DmxHalDetachFilter(Dmx_Set_S *DmxSet, HI_U32 FilterId, HI_U32 ChanId);

HI_VOID DmxHalClearOq(Dmx_Set_S *DmxSet, HI_U32 OqId, DMX_OQ_CLEAR_TYPE_E ClearType);
HI_BOOL DmxHalIsClearOqDone(Dmx_Set_S *DmxSet);

HI_VOID DmxHalEnableAllPVRInt(Dmx_Set_S *DmxSet);
HI_VOID DmxHalDisableAllPVRInt(Dmx_Set_S *DmxSet);

HI_VOID DmxHalIPPortAutoClearBP(Dmx_Set_S * DmxSet);
HI_BOOL DmxHalGetIPBPStatus(Dmx_Set_S *DmxSet, HI_U32 PortId);
HI_VOID DmxHalClrIPBPStatus(Dmx_Set_S *DmxSet, HI_U32 PortId);
HI_VOID DmxHalClrIPFqBPStatus(Dmx_Set_S *DmxSet, HI_U32 PortId, HI_U32 FQId);

HI_U32  DmxHalIPPortGetOutIntStatus(Dmx_Set_S *DmxSet, HI_U32 RawPortId);
HI_VOID DmxHalIPPortClearOutIntStatus(Dmx_Set_S *DmxSet, HI_U32 RawPortId);
HI_VOID __DmxHalIPPortSetOutInt(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_BOOL Enable);
HI_VOID DmxHalIPPortSetOutInt(Dmx_Set_S *DmxSet, const HI_U32 PortId, const HI_BOOL Enable);
HI_U32  DmxHalIPPortDescGetRead(Dmx_Set_S *DmxSet, HI_U32 RawPortId);
HI_VOID DmxHalIPPortEnableInt(Dmx_Set_S *DmxSet, HI_U32 RawPortId);
HI_VOID DmxHalIPPortDisableInt(Dmx_Set_S *DmxSet, HI_U32 RawPortId);


HI_U32  DmxHalOQGetAllEopIntStatus(Dmx_Set_S *DmxSet);
HI_VOID DmxHalEnableAllChEopInt(Dmx_Set_S *DmxSet);
HI_VOID DmxHalEnableAllChEnqueInt(Dmx_Set_S *DmxSet);
/* buffer overflow int status of play channel */
HI_VOID DmxHalEnableD2BFOvflInt(Dmx_Set_S *DmxSet);
HI_VOID DmxHalDisableD2BFOvflInt(Dmx_Set_S *DmxSet);
HI_U32  DmxHalGetD2BFOvflIntStatus(Dmx_Set_S *DmxSet);
HI_VOID DmxHalClsD2BFOvflIntStatus(Dmx_Set_S *DmxSet);
/* buffer overflow int status of record channel */
HI_VOID DmxHalEnableD2BROvflInt(Dmx_Set_S *DmxSet);
HI_VOID DmxHalDisableD2BROvflInt(Dmx_Set_S *DmxSet);
HI_U32  DmxHalGetD2BROvflIntStatus(Dmx_Set_S *DmxSet);
HI_VOID DmxHalClsD2BROvflIntStatus(Dmx_Set_S *DmxSet);

HI_VOID DmxHalEnableFQOutqueInt(HI_VOID);
HI_VOID DmxHalDisableFQOutqueInt(HI_VOID);
HI_VOID DmxHalSetFlushMaxWaitTime(Dmx_Set_S *DmxSet, HI_U32 u32MaxTime);
HI_S32  DmxHalSetScdFilter(Dmx_Set_S *DmxSet, HI_U32 u32FltId, HI_U8 u8Content);
HI_S32  DmxHalSetScdRangeFilter(Dmx_Set_S *DmxSet, HI_U32 u32FltId, HI_U8 u8High, HI_U8 u8Low);
HI_VOID DmxHalSetScdNewRangeFilter(Dmx_Set_S *DmxSet, HI_U32 FilterId, HI_U8 High, HI_U8 Low, HI_U8 Mask, HI_BOOL Negate);

HI_VOID DmxHalEnScdFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U32 FilterId);
HI_VOID DmxHalClrScdFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId);
HI_VOID DmxHalEnScdRangeFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U32 FilterId);
HI_VOID DmxHalClrScdRangeFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId);
HI_VOID DmxHalEnScdNewRangeFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U32 FilterId);
HI_VOID DmxHalClrScdNewRangeFilter(Dmx_Set_S *DmxSet, HI_U32 ScdId);

HI_VOID DmxHalEnablePesSCD(Dmx_Set_S *DmxSet, HI_U32 ScdId);
HI_VOID DmxHalDisablePesSCD(Dmx_Set_S *DmxSet, HI_U32 ScdId);
HI_VOID DmxHalEnableEsSCD(Dmx_Set_S *DmxSet, HI_U32 ScdId);
HI_VOID DmxHalDisableEsSCD(Dmx_Set_S *DmxSet, HI_U32 ScdId);
HI_VOID DmxHalEnableMp4SCD(Dmx_Set_S *DmxSet, HI_U32 ScdId);
HI_VOID DmxHalDisableMp4SCD(Dmx_Set_S *DmxSet, HI_U32 ScdId);
HI_VOID DmxHalSetSCDAttachChannel(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U32 ChanId);
HI_VOID DmxHalSetSCDBufferId(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 OqId);

HI_VOID DmxHalSetFlushIPPort(Dmx_Set_S *DmxSet, HI_U32 PortId);

HI_VOID DmxHalGetChannelTSScrambleFlag(Dmx_Set_S *DmxSet, HI_U32 u32Chid, HI_BOOL  *pEnable);
HI_VOID DmxHalGetChannelPesScrambleFlag(Dmx_Set_S *DmxSet, HI_U32 u32Chid, HI_BOOL  *pEnable);

HI_VOID DmxHalGetChannelDescStatus(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_BOOL *DoScram, HI_U32 *KeyId);

HI_VOID DmxHalSetPcrDmxId(Dmx_Set_S *DmxSet, const HI_U32 PcrId, const HI_U32 DmxId);
HI_VOID DmxHalSetPcrPid(Dmx_Set_S *DmxSet, const HI_U32 PcrId, const HI_U32 PcrPid);
HI_VOID DmxHalGetPcrValue(Dmx_Set_S *DmxSet, const HI_U32 PcrId, HI_U64 *PcrVal);
HI_VOID DmxHalGetScrValue(Dmx_Set_S *DmxSet, const HI_U32 PcrId, HI_U64 *ScrVal);

/*---------------------------test IP-----------------------------*/

HI_U32  DmxHalGetTotalTeiIntStatus(Dmx_Set_S *DmxSet);
HI_U32  DmxHalGetTotalPcrIntStatus(Dmx_Set_S *DmxSet);
HI_U32  DmxHalGetTotalDiscIntStatus(Dmx_Set_S *DmxSet);
HI_U32  DmxHalGetTotalCrcIntStatus(Dmx_Set_S *DmxSet);
HI_U32  DmxHalGetTotalPenLenIntStatus(Dmx_Set_S *DmxSet);
HI_U32  DmxHalGetPcrIntStatus(Dmx_Set_S *DmxSet);
HI_VOID DmxHalClrPcrIntStatus(Dmx_Set_S *DmxSet, const HI_U32 PcrId);
HI_VOID DmxHalSetPcrIntEnable(Dmx_Set_S *DmxSet, const HI_U32 PcrId, const HI_BOOL Enable);
HI_VOID DmxHalGetTeiIntInfo(Dmx_Set_S *DmxSet, HI_U32* pu32DmxId, HI_U32* pu32ChanId);
HI_VOID DmxHalClrTeiIntStatus(Dmx_Set_S *DmxSet);
HI_U32  DmxHalGetDiscIntStatus(Dmx_Set_S *DmxSet, HI_U32 RegionNum);
HI_VOID DmxHalClearDiscIntStatus(Dmx_Set_S *DmxSet, HI_U32 ChanId);
HI_U32  DmxHalGetCrcIntStatus(Dmx_Set_S *DmxSet, HI_U32 RegionNum);
HI_VOID DmxHalClearCrcIntStatus(Dmx_Set_S *DmxSet, HI_U32 ChanId);
HI_U32  DmxHalGetPesLenIntStatus(Dmx_Set_S *DmxSet, HI_U32 RegionNum);
HI_VOID DmxHalClearPesLenIntStatus(Dmx_Set_S *DmxSet, HI_U32 ChanId);
HI_VOID DmxHalFlushScdChn(Dmx_Set_S *DmxSet, HI_U32 ScdId);
HI_VOID DmxHalFlushScdBuf(Dmx_Set_S *DmxSet, HI_U32 RecId);
HI_VOID DmxHalClearRecTsCnt(Dmx_Set_S *DmxSet, HI_U32 RecId);
HI_VOID DmxHalGetRecTsCnt(Dmx_Set_S *DmxSet, HI_U32 ScdId, HI_U64 *TsCnt);
HI_VOID DmxHalGetCurrentSCR(Dmx_Set_S *DmxSet, HI_U32 *ScrClk);
HI_VOID DmxHalConfigHardware(HI_VOID);
HI_VOID DmxHalDeConfigHardware(HI_VOID);
HI_VOID DmxHalIPPortSetIntCnt(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_U32 DescNum);
HI_VOID DmxHalGetTSOClkCfg(HI_U32 PortId,HI_BOOL *ClkReverse,HI_U32 *enClk,HI_U32 *ClkDiv);
HI_VOID DmxHalCfgTSOClk(HI_U32 PortId,HI_BOOL ClkReverse,HI_U32 enClk,HI_U32 ClkDiv);

#ifdef DMX_SUPPORT_DMX_CLK_DYNAMIC_TUNE
HI_VOID DmxHalDynamicTuneDmxClk(HI_U32 Reduce);
#else
static inline HI_VOID DmxHalDynamicTuneDmxClk(HI_U32 Reduce) { }
#endif

HI_VOID DmxHalEnableOQOutDInt(HI_U32 u32OQId);
HI_VOID DmxHalDisableOQOutDInt(HI_U32 u32OQId);
HI_BOOL DmxHalGetOQEopIntStatus(Dmx_Set_S *DmxSet, HI_U32 u32OQId);
HI_VOID DmxHalClearOQEopIntStatus(Dmx_Set_S *DmxSet, HI_U32 u32OQId);
HI_VOID DmxHalEnableOQEopInt(Dmx_Set_S *DmxSet, HI_U32 u32OQId);
HI_VOID DmxHalDisableOQEopInt(Dmx_Set_S *DmxSet, HI_U32 u32OQId);

HI_VOID DmxHalFQEnableAllOverflowInt(Dmx_Set_S *DmxSet);
HI_U32  DmxHalFQGetAllOverflowIntStatus(Dmx_Set_S *DmxSet);
HI_U32  DmxHalFQGetOverflowIntStatus(Dmx_Set_S *DmxSet, HI_U32 offset);
HI_U32  DmxHalFQGetOverflowIntType(Dmx_Set_S *DmxSet, HI_U32 offset);
HI_VOID DmxHalFQClearOverflowInt(Dmx_Set_S *DmxSet, HI_U32 FqId);
HI_VOID DmxHalFQSetOverflowInt(Dmx_Set_S *DmxSet, HI_U32 FqId, HI_BOOL Enable);
HI_BOOL DmxHalFQIsEnableOverflowInt(Dmx_Set_S *DmxSet, HI_U32 FqId);
HI_VOID DmxHalIPPortGetDescInfo(Dmx_Set_S *DmxSet, HI_U32 RawPortId, DMX_Proc_RamPort_DescInfo_S *DescInfo);
HI_VOID DmxHalIPPortGetBPStatus(Dmx_Set_S *DmxSet, HI_U32 RawPortId, DMX_Proc_RamPort_BPStatus_S *BPStatus);
HI_VOID DMXHalGetChannelDataFlow(HI_U32 ChannelId, ChannelDataFlow_info_t *ChannelDF);


HI_U32  DmxHalOQGetAllOverflowIntStatus(Dmx_Set_S *DmxSet);
HI_BOOL DmxHalOQGetOverflowIntStatus(Dmx_Set_S *DmxSet, HI_U32 OQId);
HI_VOID DmxHalOQClearOverflowInt(Dmx_Set_S *DmxSet, HI_U32 OQId);
HI_VOID DmxHalOQEnableOverflowInt(HI_U32 OQId);
HI_VOID DmxHalOQDisableOverflowInt(HI_U32 OQId);

HI_U32  DmxHalOQGetAllOutputIntStatus(Dmx_Set_S *DmxSet);
HI_U32  DmxHalOQGetOutputIntStatus(Dmx_Set_S *DmxSet, HI_U32 OqRegionId);
HI_VOID DmxHalOQEnableOutputInt(Dmx_Set_S *DmxSet, HI_U32 OQId, HI_BOOL Enable);

HI_VOID DxmHalSetOQRegMask(Dmx_Set_S *DmxSet, HI_U32 u32MaskValue);
HI_U32  DxmHalGetOQRegMask(Dmx_Set_S *DmxSet);
HI_VOID DmxHalEnableFQOvflErrInt(HI_VOID);
HI_VOID DmxHalDisableFQOvflErrInt(HI_VOID);
HI_VOID DmxHalEnableOQOvflErrInt(HI_VOID);
HI_VOID DmxHalDisableOQOvflErrInt(HI_VOID);
HI_VOID DmxHalEnableFQCfgErrInt(HI_VOID);
HI_VOID DmxHalDisableFQCfgErrInt(HI_VOID);
HI_VOID DmxHalEnableFQDescErrInt(HI_VOID);
HI_VOID DmxHalDisableFQDescErrInt(HI_VOID);
HI_VOID DmxHalEnableAllDavInt(Dmx_Set_S *DmxSet);
HI_VOID DmxHalEnableOQRecive(Dmx_Set_S *DmxSet, HI_U32 u32OQId);
HI_VOID DmxHalDisableOQRecive(Dmx_Set_S *DmxSet, HI_U32 u32OQId);
HI_BOOL DmxHalGetOQEnableStatus(Dmx_Set_S *DmxSet, HI_U32 u32OQId);
HI_VOID DmxHalFQEnableRecive(Dmx_Set_S *DmxSet, HI_U32 FQId, HI_BOOL Enable);
HI_S32  DmxHalGetInitStatus(Dmx_Set_S * DmxSet);
HI_VOID DmxHalSetFQWORDx(Dmx_Set_S *DmxSet, HI_U32 u32FQId, HI_U32 u32Offset, HI_U32 u32Data);
HI_VOID DmxHalGetFQWORDx(Dmx_Set_S *DmxSet, HI_U32 u32FQId, HI_U32 u32Offset, HI_U32 *pu32Data);
HI_VOID DmxHalSetFQWritePtr(Dmx_Set_S *DmxSet, HI_U32 u32FQId, HI_U32 u32WritePtr);
HI_VOID DmxHalGetFQReadWritePtr(Dmx_Set_S *DmxSet, HI_U32 FQId, HI_U32 *Read, HI_U32 *Write);
HI_VOID DmxHalSetOQWORDx(Dmx_Set_S *DmxSet, HI_U32 u32OQId, HI_U32 u32Offset, HI_U32 u32Data);
HI_VOID DmxHalGetOQWORDx(Dmx_Set_S *DmxSet, HI_U32 u32OQId, HI_U32 u32Offset, HI_U32 *pu32Data);
HI_VOID DmxHalSetOQReadPtr(Dmx_Set_S *DmxSet, HI_U32 u32OQId, HI_U32 u32ReadPtr);

#ifdef DMX_MMU_SUPPORT
HI_VOID DmxHalEnableGblMmu(Dmx_Mmu_S *DmxMmu);
HI_VOID DmxHalEnableMmu(Dmx_Mmu_S *DmxMmu);

HI_VOID DmxHalEnMmuInt(Dmx_Mmu_S *DmxMmu);
HI_VOID DmxHalDisMmuInt(Dmx_Mmu_S *DmxMmu);
HI_U32 DmxHalGetMmuIntStatus(Dmx_Mmu_S *DmxMmu);
HI_VOID DmxHalClrMmuIntStatus(Dmx_Mmu_S *DmxMmu);

HI_VOID DmxHalGetMmuErrAddr(Dmx_Mmu_S *DmxMmu, HI_U32 *ErrAddr1, HI_U32 *ErrAddr2);
HI_VOID DmxHalGetMmuErrInfo(Dmx_Mmu_S *DmxMmu, HI_U32 *ErrInfo);

HI_VOID DmxHalEnableOQMmu(Dmx_Set_S *DmxSet, HI_U32 OQId);
HI_VOID DmxHalDisableOQMmu(Dmx_Set_S *DmxSet, HI_U32 OQId);
HI_VOID __DmxHalEnableRamMmu(Dmx_Set_S *DmxSet, HI_U32 RawPortId);
HI_VOID DmxHalEnableRamMmu(Dmx_Set_S *DmxSet, HI_U32 PortId);
HI_VOID __DmxHalDisableRamMmu(Dmx_Set_S *DmxSet, HI_U32 RawPortId);
HI_VOID DmxHalDisableRamMmu(Dmx_Set_S *DmxSet, HI_U32 PortId);
#else
static inline HI_VOID DmxHalEnableGblMmu(Dmx_Mmu_S *DmxMmu){};
static inline HI_VOID DmxHalEnableMmu(Dmx_Mmu_S *DmxMmu){};

static inline HI_VOID DmxHalEnMmuInt(Dmx_Mmu_S *DmxMmu){};
static inline HI_VOID DmxHalDisMmuInt(Dmx_Mmu_S *DmxMmu){};
static inline HI_U32 DmxHalGetMmuIntStatus(Dmx_Mmu_S *DmxMmu){return 0;};
static inline HI_VOID DmxHalClrMmuIntStatus(Dmx_Mmu_S *DmxMmu){};

static inline HI_VOID DmxHalGetMmuErrAddr(Dmx_Mmu_S *DmxMmu, HI_U32 *ErrAddr1, HI_U32 *ErrAddr2){};
static inline HI_VOID DmxHalGetMmuErrInfo(Dmx_Mmu_S *DmxMmu, HI_U32 *ErrInfo){};

static inline HI_VOID DmxHalEnableOQMmu(Dmx_Set_S *DmxSet, HI_U32 OQId) {};
static inline HI_VOID DmxHalDisableOQMmu(Dmx_Set_S *DmxSet, HI_U32 OQId){};
static inline HI_VOID __DmxHalEnableRamMmu(Dmx_Set_S *DmxSet, HI_U32 RawPortId){};
static inline HI_VOID DmxHalEnableRamMmu(Dmx_Set_S *DmxSet, HI_U32 PortId){};
static inline HI_VOID __DmxHalDisableRamMmu(Dmx_Set_S *DmxSet, HI_U32 RawPortId){};
static inline HI_VOID DmxHalDisableRamMmu(Dmx_Set_S *DmxSet, HI_U32 PortId){};
#endif

HI_VOID DmxHalAttachIPBPFQ(Dmx_Set_S *DmxSet, HI_U32 PortId, HI_U32 FQId);
HI_VOID DmxHalDetachIPBPFQ(Dmx_Set_S *DmxSet, HI_U32 PortId, HI_U32 FQId);

HI_VOID DmxHalSetRecTsCounter(Dmx_Set_S *DmxSet, HI_U32 u32DmxId, HI_U32 u32OqId);
HI_VOID DmxHalSetRecTsCntReplace(Dmx_Set_S *DmxSet, HI_U32 u32DmxId);
HI_U32  DmxHalGetDavDropTsFlag(Dmx_Set_S *DmxSet);
HI_VOID DmxHalClrDavDropTsFlag(Dmx_Set_S *DmxSet, HI_U32 RecId);
HI_U32  DmxHalGetSwitchDropTsFlag(Dmx_Set_S *DmxSet);
HI_VOID DmxHalClrSwitchDropTsFlag(Dmx_Set_S *DmxSet, HI_U32 DmxId);
HI_U32  DmxHalGetOqCounter(Dmx_Set_S *DmxSet, HI_U32 OQId);
HI_VOID DmxHalResetOqCounter(Dmx_Set_S *DmxSet, HI_U32 u32OqId);
HI_U32  DmxHalGetChannelCounter(Dmx_Set_S *DmxSet, HI_U32 u32ChId);
HI_VOID DmxHalResetChannelCounter(Dmx_Set_S *DmxSet, HI_U32 u32ChId);

HI_VOID DmxHalFilterSetSecStuffCtrl(Dmx_Set_S * DmxSet, HI_BOOL Enable);
HI_VOID DmxHalSetTei(Dmx_Set_S *DmxSet, HI_U32 u32DemuxID,HI_BOOL bCheckTei);

#ifdef DMX_FILTER_DEPTH_SUPPORT
HI_VOID DmxHalFilterEnableDepth(Dmx_Set_S * DmxSet);
HI_VOID DmxHalFilterSetDepth(Dmx_Set_S *DmxSet, HI_U32 FilterId, HI_U32 Depth);
#endif

#ifdef DMX_REC_EXCLUDE_PID_SUPPORT
HI_VOID DmxHalEnableAllRecExcludePid(Dmx_Set_S *DmxSet, HI_U32 DmxID);
HI_VOID DmxHalDisableAllRecExcludePid(Dmx_Set_S *DmxSet, HI_U32 DmxID);
HI_VOID DmxHalGetAllRecExcludePid(Dmx_Set_S *DmxSet, HI_U32 RecCfgID, HI_U32* DmxID, HI_U32* PID);
HI_VOID DmxHalSetAllRecExcludePid(Dmx_Set_S *DmxSet, HI_U32 RecCfgID, HI_U32 RawDmxID, HI_U32 PID);
#endif


HI_VOID DmxHalConfigRecTsTimeStamp(Dmx_Set_S *DmxSet, HI_U32 DmxId, DMX_REC_TIMESTAMP_MODE_E enRecTimeStamp);

#if defined(DMX_TAGPORT_CNT) && (DMX_TAGPORT_CNT > 0)
HI_S32 __DmxHalSetTagDealCtl(HI_BOOL bEnable, HI_UNF_DMX_PORT_E enPortId, HI_U32 u32SyncMode, HI_U32 u32TagLen);
HI_S32 DmxHalSetTagDealCtl(HI_BOOL bEnable, HI_UNF_DMX_PORT_E enPortId, HI_U32 u32SyncMode, HI_U32 u32TagLen);
HI_VOID DmxHalSetTagDealAttr(HI_U32 u32TagPortId, HI_U32 u32Low, HI_U32 u32Mid, HI_U32 u32High);
#endif

#ifdef DMX_SUPPORT_RAM_CLK_AUTO_CTL
HI_VOID DmxHalEnableRamClkAutoCtl(Dmx_Set_S * DmxSet);
#else
inline static HI_VOID DmxHalEnableRamClkAutoCtl(Dmx_Set_S * DmxSet) { };
#endif

#ifdef RMX_SUPPORT
HI_VOID DmxHalSetRemuxPort(Dmx_Set_S * DmxSet, HI_U32 ChanId, HI_U32 RmxPortId, DMX_PORT_MODE_E PortMode, HI_U32 PortId);
HI_VOID DmxHalUnsetRemuxPort(Dmx_Set_S * DmxSet, HI_U32 ChanId);
HI_VOID DmxHalSetRemuxPortAllPass(Dmx_Set_S * DmxSet, HI_U32 RmxPortId);
HI_VOID DmxHalUnSetRemuxPortAllPass(Dmx_Set_S * DmxSet, HI_U32 RmxPortId);
HI_VOID DmxHalEnDetectRemuxPort(Dmx_Set_S * DmxSet, HI_U32 RmxPortId);
HI_VOID DmxHalDisDetectRemuxPort(Dmx_Set_S * DmxSet, HI_U32 RmxPortId);
HI_U32 DmxHalGetRemuxPortOverflowCount(Dmx_Set_S * DmxSet, HI_U32 RmxPortId);
HI_VOID DmxHalSetRemuxPidTab(Dmx_Set_S * DmxSet, HI_U32 PumpId, HI_U32 RmxPortId, HI_U32 Pid);
HI_VOID DmxHalSetRemuxRemapPidTab(Dmx_Set_S * DmxSet, HI_U32 PumpId, HI_U32 RmxPortId, HI_U32 Pid, HI_U32 NewPid);
HI_VOID DmxHalUnSetRemuxPidTab(Dmx_Set_S * DmxSet, HI_U32 PumpId);
#endif

#ifdef __cplusplus
}
#endif

#endif

