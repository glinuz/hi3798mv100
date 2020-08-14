
#ifndef __DEMUX_HAL_H__
#define __DEMUX_HAL_H__

#include "hi_type.h"

#include "hi_unf_demux.h"

#define DMX_INVALID_DMX_ID      0xFFFFFFFF

#define TS_COUNT_CRTL_RESET     (0)
#define TS_COUNT_CRTL_START     (1)
#define TS_COUNT_CRTL_STOP      (2)

typedef enum
{
    DMX_PORT_MODE_TUNER = 0,
    DMX_PORT_MODE_RAM   = 1,
    DMX_PORT_MODE_BUTT
} DMX_PORT_MODE_E;

HI_VOID DmxHalConfigHardware(HI_VOID);
HI_S32  DmxHalGetInitStatus(HI_VOID);

HI_VOID DmxHalDvbPortSetAttr(HI_U32 PortId,  HI_UNF_DMX_PORT_TYPE_E  PortType,
        HI_U32                  SyncOn,
        HI_U32                  SyncOff,
        HI_U32                  TunerInClk,
        HI_U32                  BitSelector);
HI_VOID DmxHalDvbPortSetTsCountCtrl(HI_U32 PortId, HI_U32 option);
HI_VOID DmxHalDvbPortSetErrTsCountCtrl(HI_U32 PortId, HI_U32 option);

HI_VOID DmxHalDemuxSetPortId(HI_U32 DmxId, DMX_PORT_MODE_E PortMode, HI_U32 PortId);
HI_VOID DmxHalChannelSetPlayDmxId(HI_U32 ChanId, HI_U32 DmxId);
HI_VOID DmxHalChannelSetPlayBufId(HI_U32 ChanId, HI_U32 OQId);
HI_VOID DmxHalChannelSetDataType(HI_U32 ChanId, HI_UNF_DMX_CHAN_TYPE_E ChanType);
HI_VOID DmxHalChannelSetAttr(HI_U32 ChanId, HI_UNF_DMX_CHAN_TYPE_E ChanType);
HI_VOID DmxHalChannelSetPusiCtrl(HI_U32 ChanId, HI_BOOL PusiCtrl);
HI_VOID DmxHalChannelSetCRCMode(HI_U32 ChanId, HI_UNF_DMX_CHAN_CRC_MODE_E CrcMode);
HI_VOID DmxHalChannelSetAFMode(HI_U32 ChanId);
HI_VOID DmxHalChannelSetCCDiscon(HI_U32 ChanId, HI_BOOL DiscardFlag);
HI_VOID DmxHalChannelSetCCRepeatCtrl(HI_U32 ChanId, HI_U32 CCRepeatCtrl);

HI_VOID DmxHalChannelSetPid(HI_U32 ChanId, HI_U32 pid);
HI_VOID DmxHalChannelSetFilterMode(HI_U32 ChanId, HI_BOOL Enable);
HI_VOID DmxHalChannelResetCounter(HI_U32 ChanId);
HI_VOID DmxHalChannelStartFlush(HI_U32 ChanId);
HI_BOOL DmxHalChannelIsFlushDone(HI_VOID);

//-------------------------- FQ --------------------------
HI_VOID DmxHalFQClrUsedCount(HI_U32 FQId);
HI_VOID DmxHalFQSetAlmostOverflow(HI_U32 FQId, HI_U32 AlmostOverflow);
HI_VOID DmxHalFQSetRead(HI_U32 FQId, HI_U32 Read);
HI_VOID DmxHalFQClrFreeCount(HI_U32 FQId);
HI_U32  DmxHalFQGetWrite(HI_U32 FQId);
HI_VOID DmxHalFQSetWrite(HI_U32 FQId, HI_U32 Write);
HI_VOID DmxHalFQSetDepth(HI_U32 FQId, HI_U32 Depth);
HI_VOID DmxHalFQSetStartAddr(HI_U32 FQId, HI_U32 StartAddr);
HI_VOID DmxHalFQEnableRecive(HI_U32 FQId);

//-------------------------- OQ --------------------------
HI_VOID DmxHalOQSetEopResByte(HI_U32 OQId, HI_U32 Byte);
HI_VOID DmxHalOQSetWResByte(HI_U32 OQId, HI_U32 Byte);
HI_U32  DmxHalOQGetPvrCtrl(HI_U32 OQId);
HI_VOID DmxHalOQSetPvrCtrl(HI_U32 OQId, HI_U32 Val);
HI_U32  DmxHalOQGetEopAddr(HI_U32 OQId);
HI_VOID DmxHalOQSetEopAddr(HI_U32 OQId, HI_U32 addr);
HI_VOID DmxHalOQSetBBWaddr(HI_U32 OQId, HI_U32 addr);
HI_U32  DmxHalOQGetBBSize(HI_U32 OQId);
HI_VOID DmxHalOQSetBBSize(HI_U32 OQId, HI_U32 Size);
HI_VOID DmxHalOQSetBQVal(HI_U32 OQId, HI_U32 Val);
HI_VOID DmxHalOQSetBQUse(HI_U32 OQId, HI_U32 Use);
HI_U32  DmxHalOQGetBBSAddr(HI_U32 OQId);
HI_VOID DmxHalOQSetBBSAddr(HI_U32 OQId, HI_U32 addr);
HI_U32  DmxHalOQGetRead(HI_U32 OQId);
HI_VOID DmxHalOQSetRead(HI_U32 OQId, HI_U32 Read);
HI_VOID DmxHalOQSetIntCount(HI_U32 OQId, HI_U32 Count);
HI_U32  DmxHalOQGetWrite(HI_U32 OQId);
HI_VOID DmxHalOQSetWrite(HI_U32 OQId, HI_U32 Write);
HI_VOID DmxHalOQSetIntCfg(HI_U32 OQId, HI_U32 Count);
HI_VOID DmxHalOQSetDepth(HI_U32 OQId, HI_U32 Depth);
HI_VOID DmxHalOQSetAlmostOverflow(HI_U32 OQId, HI_U32 AlmostOverflow);
HI_VOID DmxHalOQSetFQId(HI_U32 OQId, HI_U32 FQId);
HI_VOID DmxHalOQSetStartAddr(HI_U32 OQId, HI_U32 StartAddr);

HI_VOID DmxHalOQResetCounter(HI_U32 OQId);
HI_VOID DmxHalOQEnableRecive(HI_U32 OQId, HI_BOOL Enable);
HI_VOID DmxHalOQStartFlush(HI_U32 OQId);
HI_BOOL DmxHalOQIsFlushDone(HI_VOID);

//-------------------------- Filter --------------------------
HI_VOID DmxHalFilterSet(HI_U32 FilterId, HI_U32 Depth, HI_U8 Content, HI_U32 Reverse, HI_U8 Mask);
HI_VOID DmxHalFilterAttach(HI_U32 FilterId, HI_U32 ChanId);
HI_VOID DmxHalFilterDetach(HI_U32 FilterId, HI_U32 ChanId);

#endif

