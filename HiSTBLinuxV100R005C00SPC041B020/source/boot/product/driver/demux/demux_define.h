
#ifndef __DEMUX_DEFINE_H__
#define __DEMUX_DEFINE_H__

#include "hi_type.h"

#include "hi_unf_demux.h"
#include "demux_config.h"

typedef enum
{
    DMX_CHAN_STATUS_CLOSE = 0,
    DMX_CHAN_STATUS_OPEN
} DMX_CHAN_STATUS_E;

typedef struct
{
    HI_U32  PortId;
} DMX_Info_S;

typedef struct
{
    HI_UNF_DMX_PORT_TYPE_E  PortType;
    HI_U32 SyncLockTh;
    HI_U32 SyncLostTh;
    HI_U32 TunerInClk;
    HI_U32 BitSelector;
}DMX_TunerPortInfo_S;

typedef struct
{
    HI_U32      FqAddr;
    HI_U32      FqBufSize;
    HI_U32      FqBlockSize;
    HI_U32      FqDescAddr;
    HI_U32      FqDescCount;
} DMX_FqInfo_S;

typedef struct
{
    HI_VOID    *MemoryAddr;
    HI_U32      OqDescAddr;
    HI_U32      OqDescCount;
    HI_U32      OqCurrBlock;
    HI_U32      OqCurrOffset;
} DMX_OqInfo_S;

typedef struct
{
    HI_U32                      DmxId;
    HI_U32                      ChanPid;
    HI_UNF_DMX_CHAN_CRC_MODE_E  CrcMode;
    DMX_CHAN_STATUS_E           ChanStatus;
    HI_U32                      FilterCount;
    HI_U32                      FQId;
    HI_U32                      OQId;
} DMX_ChanInfo_S;

typedef struct
{
    HI_U32      DmxId;
    HI_U32      ChanId;
    HI_U32      Depth;
    HI_U8       Match[DMX_FILTER_MAX_DEPTH];
    HI_U8       Mask[DMX_FILTER_MAX_DEPTH];
    HI_U8       Negate[DMX_FILTER_MAX_DEPTH];
} DMX_FilterInfo_S;

typedef struct
{
    DMX_Info_S          DmxInfo[HI_DEMUX_COUNT];
    DMX_TunerPortInfo_S DmxTunerPortInfo[HI_TUNER_PORT_COUNT];
    DMX_FqInfo_S        DmxFqInfo;
    DMX_OqInfo_S        DmxOqInfo[HI_OQ_COUNT];
    DMX_ChanInfo_S      DmxChanInfo[HI_CHANNEL_COUNT];
    DMX_FilterInfo_S    DmxFilterInfo[HI_FILTER_COUNT];
} DMX_DeviceManager_S;

#endif

