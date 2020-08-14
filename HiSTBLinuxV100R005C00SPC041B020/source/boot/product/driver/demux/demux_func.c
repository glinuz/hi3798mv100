
#include <uboot.h>

#include "hi_type.h"

#include "hi_error_mpi.h"

#include "demux_func.h"
#include "demux_define.h"
#include "demux_hal.h"

#ifdef  HI_ADVCA_FUNCTION_RELEASE
#define HI_FATAL_DEMUX(fmt...)         
#define HI_ERR_DEMUX(fmt...)           
#define HI_WARN_DEMUX(fmt...)         
#define HI_INFO_DEMUX(fmt...)         
#else
#define HI_FATAL_DEMUX(fmt...)          printf(fmt)
#define HI_ERR_DEMUX(fmt...)            printf(fmt)
#define HI_WARN_DEMUX(fmt...)           printf(fmt)
#define HI_INFO_DEMUX(fmt...)           printf(fmt)
#endif

#define DMX_MEM_ALIGN_SIZE              0x1000

#define DMX_COMMON_FQ_ID                0
#define DMX_COMMON_FQ_BUFFER_SIZE       (1 * 1024 * 1024)
#define DMX_COMMON_FQ_BLOCK_SIZE        (4 * 1024)

#define DMX_OQ_ALMOST_OVERFLOW_COUNT    0x4
#define DMX_OQ_OUTINT_CNT               0x8

#define DMX_FQ_DESC_SIZE                8
#define DMX_OQ_DESC_SIZE                16

#define DMX_MAX_OQ_DEPTH                0x3FF

#define DMXINC(a, size)         \
    do                          \
    {                           \
        if ((++a) >= (size))    \
        {                       \
            (a) = 0;            \
        }                       \
    } while (0)

#define DMX_INVALID_PORT_ID             0xFFFFFFFF
#define DMX_INVALID_CHAN_ID             0xFFFFFFFF
#define DMX_INVALID_FILTER_ID           0xFFFFFFFF
#define DMX_INVALID_PID                 0x1FFF

#define SECTION_LENGTH(a, b)            (((((a) & 0xf) << 8) | (b)) + 3)

#define MIN_SECTION_LENGTH_WITH_CRC     7

#define DMX_MAX_FILTER_NUM_PER_CHANNEL  32

#define DMX_MAX_FLUSH_WAIT              0x1000

#define CHECK_DMX_INIT()                                    \
    do                                                      \
    {                                                       \
        if (HI_NULL == DmxDevMgr)                           \
        {                                                   \
            return HI_ERR_DMX_NOT_INIT;                     \
        }                                                   \
    } while (0)

#define CHECK_POINTER(ptr)                                  \
    do                                                      \
    {                                                       \
        if (!(ptr))                                         \
        {                                                   \
            return HI_ERR_DMX_NULL_PTR;                     \
        }                                                   \
    } while (0)

#define CHECK_DMXID(DmxId)                                  \
    do                                                      \
    {                                                       \
        if ((DmxId) >= HI_DEMUX_COUNT)                      \
        {                                                   \
            return HI_ERR_DMX_INVALID_PARA;                 \
        }                                                   \
    } while (0)

#define CHECK_TUNER_PORTID(PortId)                          \
    do                                                      \
    {                                                       \
        if ((PortId) >= HI_TUNER_PORT_COUNT)                \
        {                                                   \
            return HI_ERR_DMX_INVALID_PARA;                 \
        }                                                   \
    } while (0)

#define DMX_CHECK_CHANID(ChanId)                            \
    do                                                      \
    {                                                       \
        if ((ChanId) >= HI_CHANNEL_COUNT)                   \
        {                                                   \
            return HI_ERR_DMX_INVALID_PARA;                 \
        }                                                   \
    } while (0)

#define DMX_CHECK_FILTERID(FilterId)                        \
    do                                                      \
    {                                                       \
        if ((FilterId) >= HI_FILTER_COUNT)                  \
        {                                                   \
            return HI_ERR_DMX_INVALID_PARA;                 \
        }                                                   \
    } while (0)

#define DMX_CHECK_PID(pid)                                  \
    do                                                      \
    {                                                       \
        if ((pid) > DMX_INVALID_PID)                        \
        {                                                   \
            return HI_ERR_DMX_INVALID_PARA;                 \
        }                                                   \
    } while (0)

typedef struct
{
    HI_U32  StartAddr;
    HI_U32  BufSize     : 16;   // [15:0]
    HI_U32  Reserved0   : 16;   // [31:16]
} FQ_DescInfo_S;

typedef struct
{
    HI_U32  StartAddr;
    HI_U32  BlockSize   : 16;   // [15:0]
    HI_U32  CaCtrl      : 16;   // [31:16]
    HI_U32  DataSize    : 16;   // [15:0]
    HI_U32  PvrCtrl     : 8;    // [23:16]
    HI_U32  Reserved    : 8;    // [31:24]
    HI_U32  CaReserved;
} OQ_DescInfo_S;

static DMX_DeviceManager_S *DmxDevMgr = HI_NULL;

static HI_VOID DmxFqConfig(HI_U32 FQId)
{
    DMX_FqInfo_S   *FqInfo      = &DmxDevMgr->DmxFqInfo;
    FQ_DescInfo_S  *DescInfo    = (FQ_DescInfo_S*)FqInfo->FqDescAddr;
    HI_U32          PhyAddr     = FqInfo->FqAddr;

    while (PhyAddr < (FqInfo->FqAddr + FqInfo->FqBufSize))
    {
        DescInfo->StartAddr = PhyAddr;
        DescInfo->BufSize   = FqInfo->FqBlockSize;
        DescInfo->Reserved0 = 0;

        ++DescInfo;

        PhyAddr += FqInfo->FqBlockSize;
    }

    DmxHalFQSetStartAddr(FQId, FqInfo->FqDescAddr);
    DmxHalFQSetDepth(FQId, FqInfo->FqDescCount);
    DmxHalFQSetWrite(FQId, FqInfo->FqDescCount - 1);
    DmxHalFQClrFreeCount(FQId);
    DmxHalFQSetRead(FQId, 0);
    DmxHalFQSetAlmostOverflow(FQId, 0);
    DmxHalFQClrUsedCount(FQId);

    DmxHalFQEnableRecive(FQId);
}

static HI_VOID DmxOqConfig(HI_U32 OQId, HI_U32 FQId, HI_U32 Depth)
{
    DMX_OqInfo_S *OqInfo  = &DmxDevMgr->DmxOqInfo[OQId];

    memset((HI_VOID*)OqInfo->OqDescAddr, 0, Depth * DMX_OQ_DESC_SIZE);

    OqInfo->OqDescCount     = Depth;
    OqInfo->OqCurrBlock     = 0;
    OqInfo->OqCurrOffset    = 0;

    DmxHalOQSetStartAddr(OQId, OqInfo->OqDescAddr);

    DmxHalOQSetFQId(OQId, FQId);
    DmxHalOQSetAlmostOverflow(OQId, DMX_OQ_ALMOST_OVERFLOW_COUNT);
    DmxHalOQSetDepth(OQId, OqInfo->OqDescCount);
    DmxHalOQSetIntCfg(OQId, DMX_OQ_OUTINT_CNT);

    DmxHalOQSetWrite(OQId, 0);
    DmxHalOQSetIntCount(OQId, 0);
    DmxHalOQSetRead(OQId, 0);

    DmxHalOQSetBBSAddr(OQId, 0);

    DmxHalOQSetBQUse(OQId, 0);
    DmxHalOQSetBQVal(OQId, 0);
    DmxHalOQSetBBSize(OQId, 0);

    DmxHalOQSetBBWaddr(OQId, 0);
    DmxHalOQSetEopAddr(OQId, 0);

    DmxHalOQSetPvrCtrl(OQId, 0);
    DmxHalOQSetWResByte(OQId, 0);

    DmxHalOQSetEopResByte(OQId, 0);

    DmxHalOQResetCounter(OQId);
}

static HI_U32 DmxOQGetEopAddr(HI_U32 OQId)
{
    HI_U32  EopAddr = 0;
    HI_U32  PvrCtrl;

    PvrCtrl = DmxHalOQGetPvrCtrl(OQId);
    if (PvrCtrl & 0x80)
    {
        EopAddr = DmxHalOQGetEopAddr(OQId);
    }

    return EopAddr;
}

static HI_VOID DmxOQRelease(HI_U32 OQId, HI_U32 FQId, HI_BOOL ReleaseBbsAddr)
{
    DMX_OqInfo_S   *OqInfo  = &DmxDevMgr->DmxOqInfo[OQId];
    DMX_FqInfo_S   *FqInfo  = &DmxDevMgr->DmxFqInfo;
    OQ_DescInfo_S  *OqDescInfo;
    FQ_DescInfo_S  *FqDescInfo;
    HI_U32          OqRead;
    HI_U32          OqWrite;
    HI_U32          FqWrite;

    OqRead  = DmxHalOQGetRead(OQId);
    OqWrite = DmxHalOQGetWrite(OQId);

    FqWrite = DmxHalFQGetWrite(FQId);

    while (OqRead != OqWrite)
    {
        OqDescInfo  = (OQ_DescInfo_S*)(OqInfo->OqDescAddr + OqRead * DMX_OQ_DESC_SIZE);
        FqDescInfo  = (FQ_DescInfo_S*)(FqInfo->FqDescAddr + FqWrite * DMX_FQ_DESC_SIZE);

        if ((OqRead == OqInfo->OqCurrBlock) && !ReleaseBbsAddr)
        {
            if (OqInfo->OqCurrOffset < OqDescInfo->DataSize)
            {
                break;
            }

            DMXINC(OqInfo->OqCurrBlock, OqInfo->OqDescCount);

            OqInfo->OqCurrOffset = 0;
        }

        FqDescInfo->StartAddr   = OqDescInfo->StartAddr;
        FqDescInfo->BufSize     = OqDescInfo->BlockSize;

        DMXINC(OqRead, OqInfo->OqDescCount);
        DMXINC(FqWrite, FqInfo->FqDescCount);

        DmxHalOQSetRead(OQId, OqRead);
        DmxHalFQSetWrite(FQId, FqWrite);
    }

    if (ReleaseBbsAddr)
    {
        HI_U32  PvrCtrl = DmxHalOQGetPvrCtrl(OQId);

        if (PvrCtrl & 0x80)
        {
            FqDescInfo = (FQ_DescInfo_S*)(FqInfo->FqDescAddr + FqWrite * DMX_FQ_DESC_SIZE);

            FqDescInfo->StartAddr   = DmxHalOQGetBBSAddr(OQId);
            FqDescInfo->BufSize     = DmxHalOQGetBBSize(OQId);

            DMXINC(FqWrite, FqInfo->FqDescCount);

            DmxHalFQSetWrite(FQId, FqWrite);
        }
    }
}

static HI_VOID DmxChannelFlush(HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo = &DmxDevMgr->DmxChanInfo[ChanId];
    HI_U32          i;

    DmxHalChannelStartFlush(ChanId);
    DmxHalOQStartFlush(ChanInfo->OQId);

    for (i = 0; i < DMX_MAX_FLUSH_WAIT; i++)
    {
        if (DmxHalChannelIsFlushDone())
        {
            break;
        }
    }

    if (DMX_MAX_FLUSH_WAIT == i)
    {
        HI_WARN_DEMUX("Channel %u flush failed\n", ChanId);

        return;
    }

    for (i = 0; i < DMX_MAX_FLUSH_WAIT; i++)
    {
        if (DmxHalOQIsFlushDone())
        {
            break;
        }
    }

    if (DMX_MAX_FLUSH_WAIT == i)
    {
        HI_WARN_DEMUX("OQ %u flush failed\n", ChanInfo->OQId);

        return;
    }

    return;
}

static HI_U32 FilterGetValidDepth(const HI_UNF_DMX_FILTER_ATTR_S *FilterAttr)
{
    HI_U32 Depth = FilterAttr->u32FilterDepth;

    while (Depth)
    {
        if (0xff == FilterAttr->au8Mask[Depth - 1])
        {
            --Depth;
        }
        else
        {
            break;
        }
    }

    return Depth;
}

static HI_BOOL DmxOQBlockSectionCheck(HI_U8 *buf, HI_U32 size)
{
    HI_U8  *ptr = buf;
    HI_U32  len = 0;
    HI_U32  sect_len;

    while (len < size)
    {
        sect_len = SECTION_LENGTH(ptr[1], ptr[2]);

        ptr += sect_len;
        len += sect_len;
    }

    return (len == size) ? HI_TRUE : HI_FALSE;
}

HI_S32 DmxInit(HI_VOID)
{
    if (!DmxDevMgr)
    {
        HI_U32          DescDepth;
        HI_U32          DescSize;
        DMX_FqInfo_S   *FqInfo;
        HI_U32          Size;
        HI_U32          i;

        DmxHalConfigHardware();

        udelay(10);

        if (HI_SUCCESS != DmxHalGetInitStatus())
        {
            HI_FATAL_DEMUX("init failed\n");

            return HI_ERR_DMX_NOT_INIT;
        }

        Size        = sizeof(DMX_DeviceManager_S);
        DescDepth   = DMX_COMMON_FQ_BUFFER_SIZE / DMX_COMMON_FQ_BLOCK_SIZE + 1;
        DescSize    = DescDepth * DMX_FQ_DESC_SIZE;

        DmxDevMgr = (DMX_DeviceManager_S*)malloc(Size + DMX_MEM_ALIGN_SIZE + DMX_COMMON_FQ_BUFFER_SIZE + DescSize);
        if (!DmxDevMgr)
        {
            HI_FATAL_DEMUX("malloc failed\n");

            return HI_ERR_DMX_ALLOC_MEM_FAILED;
        }

        memset(DmxDevMgr, 0, Size);

        FqInfo = &DmxDevMgr->DmxFqInfo;

        FqInfo->FqAddr      = ((HI_U32)DmxDevMgr) + Size;
        FqInfo->FqAddr      = (FqInfo->FqAddr + DMX_MEM_ALIGN_SIZE) / DMX_MEM_ALIGN_SIZE * DMX_MEM_ALIGN_SIZE;
        FqInfo->FqBufSize   = DMX_COMMON_FQ_BUFFER_SIZE;
        FqInfo->FqBlockSize = DMX_COMMON_FQ_BLOCK_SIZE;
        FqInfo->FqDescAddr  = FqInfo->FqAddr + FqInfo->FqBufSize;
        FqInfo->FqDescCount = DescDepth;

        DmxFqConfig(DMX_COMMON_FQ_ID);

        for (i = 0; i < HI_DEMUX_COUNT; i++)
        {
            DmxDevMgr->DmxInfo[i].PortId = DMX_INVALID_PORT_ID;
        }

        for (i = 0; i < HI_TUNER_PORT_COUNT; i++)
        {
            DmxDevMgr->DmxTunerPortInfo[i].PortType             = HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188;
            DmxDevMgr->DmxTunerPortInfo[i].SyncLockTh         = 5;
            DmxDevMgr->DmxTunerPortInfo[i].SyncLostTh         = 1;
            DmxDevMgr->DmxTunerPortInfo[i].TunerInClk         = 0;
            DmxDevMgr->DmxTunerPortInfo[i].BitSelector  = 0;
    
            DmxHalDvbPortSetAttr(i, DmxDevMgr->DmxTunerPortInfo[i].PortType, DmxDevMgr->DmxTunerPortInfo[i].SyncLockTh, 
                    DmxDevMgr->DmxTunerPortInfo[i].SyncLostTh, DmxDevMgr->DmxTunerPortInfo[i].TunerInClk , DmxDevMgr->DmxTunerPortInfo[i].BitSelector );

            DmxHalDvbPortSetTsCountCtrl(i, TS_COUNT_CRTL_START);
            DmxHalDvbPortSetErrTsCountCtrl(i, TS_COUNT_CRTL_START);
        }

        for (i = 0; i < HI_CHANNEL_COUNT; i++)
        {
            DMX_ChanInfo_S *ChanInfo = &DmxDevMgr->DmxChanInfo[i];

            ChanInfo->DmxId         = DMX_INVALID_DMX_ID;
            ChanInfo->ChanPid       = DMX_INVALID_PID;
            ChanInfo->ChanStatus    = DMX_CHAN_STATUS_CLOSE;
            ChanInfo->FilterCount   = 0;

            DmxHalChannelSetPid(i, ChanInfo->ChanPid);
        }

        for (i = 0; i < HI_FILTER_COUNT; i++)
        {
            DMX_FilterInfo_S *FilterInfo = &DmxDevMgr->DmxFilterInfo[i];

            FilterInfo->DmxId   = DMX_INVALID_DMX_ID;
            FilterInfo->ChanId  = DMX_INVALID_CHAN_ID;
        }
    }

    return HI_SUCCESS;
}

HI_VOID DmxDeInit(HI_VOID)
{
    if (DmxDevMgr)
    {
        HI_U32 i;

        for (i = 0; i < HI_TUNER_PORT_COUNT; i++)
        {
            DmxHalDvbPortSetTsCountCtrl(i, TS_COUNT_CRTL_STOP);
            DmxHalDvbPortSetTsCountCtrl(i, TS_COUNT_CRTL_RESET);

            DmxHalDvbPortSetErrTsCountCtrl(i, TS_COUNT_CRTL_STOP);
            DmxHalDvbPortSetErrTsCountCtrl(i, TS_COUNT_CRTL_RESET);
        }

        free(DmxDevMgr);

        DmxDevMgr = HI_NULL;
    }
}

HI_S32 DmxAttachTunerPort(const HI_U32 DmxId, const HI_U32 PortId)
{
    DMX_Info_S *DmxInfo;

    CHECK_DMX_INIT();
    CHECK_DMXID(DmxId);
    CHECK_TUNER_PORTID(PortId);

    DmxInfo = &DmxDevMgr->DmxInfo[DmxId];

    DmxInfo->PortId = PortId;

    DmxHalDemuxSetPortId(DmxId, DMX_PORT_MODE_TUNER, PortId);

    return HI_SUCCESS;
}

HI_S32 DmxDetachPort(const HI_U32 DmxId)
{
    DMX_Info_S *DmxInfo;

    CHECK_DMX_INIT();
    CHECK_DMXID(DmxId);

    DmxInfo = &DmxDevMgr->DmxInfo[DmxId];

    DmxInfo->PortId = DMX_INVALID_PORT_ID;

    DmxHalDemuxSetPortId(DmxId, DMX_PORT_MODE_BUTT, 0);

    return HI_SUCCESS;
}

HI_S32 DmxGetTSPortAttr(HI_U32 PortId, HI_UNF_DMX_PORT_ATTR_S *pstAttr)
{
    DMX_TunerPortInfo_S *TunerPortInfo;
    
    CHECK_DMX_INIT();
    CHECK_TUNER_PORTID(PortId);
    CHECK_POINTER(pstAttr);

    TunerPortInfo = &DmxDevMgr->DmxTunerPortInfo[PortId];
    
    pstAttr->enPortMod             = HI_UNF_DMX_PORT_MODE_EXTERNAL;
    pstAttr->enPortType            = TunerPortInfo->PortType;
    pstAttr->u32SyncLostTh         = TunerPortInfo->SyncLostTh;
    pstAttr->u32SyncLockTh         = TunerPortInfo->SyncLockTh;
    pstAttr->u32TunerInClk         = TunerPortInfo->TunerInClk;
    pstAttr->u32SerialBitSelector  = TunerPortInfo->BitSelector;
    pstAttr->u32TunerErrMod        = 0;
    pstAttr->u32UserDefLen1        = 0;
    pstAttr->u32UserDefLen2        = 0;

    return HI_SUCCESS;
}

HI_S32 DmxSetTSPortAttr(HI_U32 PortId, HI_UNF_DMX_PORT_ATTR_S *pstAttr)
{
    DMX_TunerPortInfo_S *TunerPortInfo;
    
    CHECK_DMX_INIT();
    CHECK_TUNER_PORTID(PortId);
    CHECK_POINTER(pstAttr);

    switch (pstAttr->enPortType)
    {
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_BURST :
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_VALID :
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188 :
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_204 :
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188_204 :
        case HI_UNF_DMX_PORT_TYPE_SERIAL :
            break;

        case HI_UNF_DMX_PORT_TYPE_SERIAL2BIT :
        case HI_UNF_DMX_PORT_TYPE_SERIAL_NOSYNC :
        case HI_UNF_DMX_PORT_TYPE_SERIAL2BIT_NOSYNC :
            break;

        case HI_UNF_DMX_PORT_TYPE_USER_DEFINED :
        case HI_UNF_DMX_PORT_TYPE_AUTO :
            return HI_ERR_DMX_NOT_SUPPORT;

        default :
            HI_ERR_DEMUX("Port %u set invalid port type %d\n", PortId, pstAttr->enPortType);

            return HI_ERR_DMX_INVALID_PARA;
    }

    if (pstAttr->u32TunerInClk > 1)
    {
        HI_ERR_DEMUX("Port %u set invalid tunner in clock %d\n", PortId, pstAttr->u32TunerInClk);

        return HI_ERR_DMX_INVALID_PARA;
    }

    if (pstAttr->u32SerialBitSelector > 1)
    {
        HI_ERR_DEMUX("Port %u set invalid Serial Bit Selector %d\n", PortId, pstAttr->u32SerialBitSelector);

        return HI_ERR_DMX_INVALID_PARA;
    }

    TunerPortInfo = &DmxDevMgr->DmxTunerPortInfo[PortId];

    TunerPortInfo->PortType = pstAttr->enPortType;
    TunerPortInfo->SyncLockTh = pstAttr->u32SyncLockTh;
    TunerPortInfo->SyncLostTh = pstAttr->u32SyncLostTh;
    TunerPortInfo->TunerInClk = pstAttr->u32TunerInClk;
    TunerPortInfo->BitSelector = pstAttr->u32SerialBitSelector;
    
    DmxHalDvbPortSetAttr(PortId, TunerPortInfo->PortType, TunerPortInfo->SyncLockTh,
            TunerPortInfo->SyncLostTh, TunerPortInfo->TunerInClk, TunerPortInfo->BitSelector);

    return HI_SUCCESS;
}

HI_S32 DmxChannelCreate(const HI_U32 DmxId, const HI_UNF_DMX_CHAN_ATTR_S *ChanAttr, HI_U32 *ChanId)
{
    DMX_ChanInfo_S *ChanInfo    = HI_NULL;
    DMX_OqInfo_S   *OqInfo      = HI_NULL;
    HI_U32          i;

    CHECK_DMX_INIT();
    CHECK_DMXID(DmxId);
    CHECK_POINTER(ChanAttr);
    CHECK_POINTER(ChanId);

    if (   (HI_UNF_DMX_CHAN_TYPE_SEC != ChanAttr->enChannelType)
        || (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY != ChanAttr->enOutputMode) )
    {
        return HI_ERR_DMX_NOT_SUPPORT;
    }

    switch (ChanAttr->enCRCMode)
    {
        case HI_UNF_DMX_CHAN_CRC_MODE_FORBID :
        case HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD :
        case HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_SEND :
        case HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD :
        case HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_SEND :
            break;

        default :
            return HI_ERR_DMX_INVALID_PARA;
    }

    for (i = 0; i < HI_CHANNEL_COUNT; i++)
    {
        ChanInfo = &DmxDevMgr->DmxChanInfo[i];
        if (DMX_INVALID_DMX_ID == ChanInfo->DmxId)
        {
            break;
        }
    }

    if (HI_CHANNEL_COUNT == i)
    {
        return HI_ERR_DMX_NOFREE_CHAN;
    }

    OqInfo = &DmxDevMgr->DmxOqInfo[i];

    OqInfo->MemoryAddr = (HI_VOID*)malloc(DMX_MEM_ALIGN_SIZE + DMX_MAX_OQ_DEPTH * DMX_OQ_DESC_SIZE);
    if (!OqInfo->MemoryAddr)
    {
        HI_ERR_DEMUX("malloc failed\n");

        return HI_ERR_DMX_ALLOC_MEM_FAILED;
    }

    OqInfo->OqDescAddr = ((HI_U32)OqInfo->MemoryAddr + DMX_MEM_ALIGN_SIZE) / DMX_MEM_ALIGN_SIZE * DMX_MEM_ALIGN_SIZE;

    DmxOqConfig(i, DMX_COMMON_FQ_ID, DMX_MAX_OQ_DEPTH);

    DmxHalChannelSetPlayBufId(i, i);

    DmxHalChannelSetDataType(i, ChanAttr->enChannelType);
    DmxHalChannelSetAttr(i, ChanAttr->enChannelType);
    DmxHalChannelSetPusiCtrl(i, HI_FALSE);
    DmxHalChannelSetCRCMode(i, ChanAttr->enCRCMode);
    DmxHalChannelSetAFMode(i);
    DmxHalChannelSetCCDiscon(i, HI_FALSE);
    DmxHalChannelSetCCRepeatCtrl(i, HI_TRUE);

    DmxHalChannelSetPid(i, DMX_INVALID_PID);

    ChanInfo->DmxId         = DmxId;
    ChanInfo->ChanPid       = DMX_INVALID_PID;
    ChanInfo->CrcMode       = ChanAttr->enCRCMode;
    ChanInfo->ChanStatus    = DMX_CHAN_STATUS_CLOSE;
    ChanInfo->FilterCount   = 0;
    ChanInfo->FQId          = DMX_COMMON_FQ_ID;
    ChanInfo->OQId          = i;

    *ChanId = i;

    return HI_SUCCESS;
}

HI_S32 DmxChannelDestroy(const HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo;
    DMX_OqInfo_S   *OqInfo;

    CHECK_DMX_INIT();
    DMX_CHECK_CHANID(ChanId);

    ChanInfo    = &DmxDevMgr->DmxChanInfo[ChanId];
    OqInfo      = &DmxDevMgr->DmxOqInfo[ChanId];

    if (DMX_INVALID_DMX_ID == ChanInfo->DmxId)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (DMX_CHAN_STATUS_CLOSE != ChanInfo->ChanStatus)
    {
        DmxChannelClose(ChanId);
    }

    if (ChanInfo->FilterCount)
    {
        HI_U32 i;

        for (i = 0; i < HI_FILTER_COUNT; i++)
        {
            DMX_FilterInfo_S *FilterInfo = &DmxDevMgr->DmxFilterInfo[i];

            if (ChanId == FilterInfo->ChanId)
            {
                FilterInfo->ChanId = DMX_INVALID_CHAN_ID;

                DmxHalFilterDetach(i, ChanId);
            }
        }

        ChanInfo->FilterCount = 0;
    }

    if (OqInfo->OqDescAddr)
    {
        free(OqInfo->MemoryAddr);

        OqInfo->MemoryAddr  = HI_NULL;
        OqInfo->OqDescAddr  = 0;
    }

    ChanInfo->DmxId     = DMX_INVALID_DMX_ID;
    ChanInfo->ChanPid   = DMX_INVALID_PID;

    return HI_SUCCESS;
}

HI_S32 DmxChannelSetPid(const HI_U32 ChanId, const HI_U32 Pid)
{
    DMX_ChanInfo_S *ChanInfo;
    HI_U32          i;

    CHECK_DMX_INIT();
    DMX_CHECK_CHANID(ChanId);
    DMX_CHECK_PID(Pid);

    ChanInfo = &DmxDevMgr->DmxChanInfo[ChanId];

    if (DMX_INVALID_DMX_ID == ChanInfo->DmxId)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (DMX_CHAN_STATUS_CLOSE != ChanInfo->ChanStatus)
    {
        return HI_ERR_DMX_OPENING_CHAN;
    }

    for (i = 0; i < HI_CHANNEL_COUNT; i++)
    {
        if ((ChanInfo->DmxId == DmxDevMgr->DmxChanInfo[i].DmxId) && (Pid == DmxDevMgr->DmxChanInfo[i].ChanPid))
        {
            ChanInfo->ChanPid = DMX_INVALID_PID;

            DmxHalChannelSetPid(i, ChanInfo->ChanPid);

            break;
        }
    }

    ChanInfo->ChanPid = Pid;
    DmxHalChannelSetPid(ChanId, Pid);

    return HI_SUCCESS;
}

HI_S32 DmxChannelOpen(const HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo;

    CHECK_DMX_INIT();
    DMX_CHECK_CHANID(ChanId);

    ChanInfo = &DmxDevMgr->DmxChanInfo[ChanId];
    if (DMX_INVALID_DMX_ID == ChanInfo->DmxId)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (DMX_CHAN_STATUS_CLOSE == ChanInfo->ChanStatus)
    {
        DmxHalChannelResetCounter(ChanId);
        DmxHalChannelSetPlayDmxId(ChanId, ChanInfo->DmxId);
        DmxHalChannelSetFilterMode(ChanId, HI_TRUE);

        DmxHalOQEnableRecive(ChanInfo->OQId, HI_TRUE);

        ChanInfo->ChanStatus = DMX_CHAN_STATUS_OPEN;
    }

    return HI_SUCCESS;
}

HI_S32 DmxChannelClose(const HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo;

    CHECK_DMX_INIT();
    DMX_CHECK_CHANID(ChanId);

    ChanInfo = &DmxDevMgr->DmxChanInfo[ChanId];
    if (DMX_INVALID_DMX_ID == ChanInfo->DmxId)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    DmxHalOQEnableRecive(ChanInfo->OQId, HI_FALSE);
    DmxHalChannelSetPlayDmxId(ChanId, DMX_INVALID_DMX_ID);

    DmxChannelFlush(ChanId);

    DmxOQRelease(ChanInfo->OQId, ChanInfo->FQId, HI_TRUE);

    DmxOqConfig(ChanId, DMX_COMMON_FQ_ID, DMX_MAX_OQ_DEPTH);

    ChanInfo->ChanStatus = DMX_CHAN_STATUS_CLOSE;

    return HI_SUCCESS;
}

HI_S32 DmxFilterCreate(const HI_U32 DmxId, const HI_UNF_DMX_FILTER_ATTR_S *FilterAttr, HI_U32 *FilterId)
{
    HI_S32  ret = HI_ERR_DMX_NOFREE_FILTER;
    HI_U32  i;

    CHECK_DMX_INIT();
    CHECK_DMXID(DmxId);
    CHECK_POINTER(FilterAttr);
    CHECK_POINTER(FilterId);

    if (FilterAttr->u32FilterDepth > DMX_FILTER_MAX_DEPTH)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    for (i = 0; i < FilterAttr->u32FilterDepth; i++)
    {
        if (FilterAttr->au8Negate[i] > 1)
        {
            return HI_ERR_DMX_INVALID_PARA;
        }
    }

    for (i = 0; i < HI_FILTER_COUNT; i++)
    {
        DMX_FilterInfo_S *FilterInfo = &DmxDevMgr->DmxFilterInfo[i];

        if (DMX_INVALID_DMX_ID == FilterInfo->DmxId)
        {
            HI_U32 j;

            FilterInfo->DmxId   = DmxId;
            FilterInfo->ChanId  = DMX_INVALID_CHAN_ID;
            FilterInfo->Depth   = FilterGetValidDepth(FilterAttr);

            for (j = 0; j < DMX_FILTER_MAX_DEPTH; j++)
            {
                if (j < FilterAttr->u32FilterDepth)
                {
                    HI_U32 Negate = FilterAttr->au8Negate[j];

                    if (0xff == FilterAttr->au8Mask[j])
                    {
                        Negate = 0;
                    }

                    FilterInfo->Match[j]    = FilterAttr->au8Match[j];
                    FilterInfo->Mask[j]     = FilterAttr->au8Mask[j];
                    FilterInfo->Negate[j]   = Negate;

                    DmxHalFilterSet(i, j, FilterInfo->Match[j], Negate, FilterInfo->Mask[j]);
                }
                else
                {
                    DmxHalFilterSet(i, j, 0, 0, 0xff);
                }
            }

            *FilterId = i;

            ret = HI_SUCCESS;

            break;
        }
    }

    return ret;
}

HI_S32 DmxFilterDestroy(const HI_U32 FilterId)
{
    HI_S32              ret = HI_ERR_DMX_INVALID_PARA;
    DMX_FilterInfo_S   *FilterInfo;

    CHECK_DMX_INIT();
    DMX_CHECK_FILTERID(FilterId);

    FilterInfo = &DmxDevMgr->DmxFilterInfo[FilterId];

    if (DMX_INVALID_DMX_ID != FilterInfo->DmxId)
    {
        if (FilterInfo->ChanId < HI_CHANNEL_COUNT)
        {
            DMX_ChanInfo_S *ChanInfo = &DmxDevMgr->DmxChanInfo[FilterInfo->ChanId];

            --ChanInfo->FilterCount;

            DmxHalFilterDetach(FilterId, FilterInfo->ChanId);
        }

        FilterInfo->DmxId   = DMX_INVALID_DMX_ID;
        FilterInfo->ChanId  = DMX_INVALID_CHAN_ID;

        ret = HI_SUCCESS;
    }

    return ret;
}

HI_S32 DmxFilterAttach(const HI_U32 FilterId, const HI_U32 ChanId)
{
    DMX_FilterInfo_S   *FilterInfo;
    DMX_ChanInfo_S     *ChanInfo;

    CHECK_DMX_INIT();
    DMX_CHECK_FILTERID(FilterId);
    DMX_CHECK_CHANID(ChanId);

    FilterInfo  = &DmxDevMgr->DmxFilterInfo[FilterId];
    ChanInfo    = &DmxDevMgr->DmxChanInfo[ChanId];

    if ((DMX_INVALID_DMX_ID == ChanInfo->DmxId) || (DMX_INVALID_DMX_ID == FilterInfo->DmxId))
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (FilterInfo->ChanId < HI_CHANNEL_COUNT)
    {
        return HI_ERR_DMX_ATTACHED_FILTER;
    }

    if (ChanInfo->FilterCount >= DMX_MAX_FILTER_NUM_PER_CHANNEL)
    {
        return HI_ERR_DMX_NOT_SUPPORT;
    }

    FilterInfo->ChanId = ChanId;

    ++ChanInfo->FilterCount;

    DmxHalFilterAttach(FilterId, ChanId);

    return HI_SUCCESS;
}

HI_S32 DmxFilterDetach(const HI_U32 FilterId, const HI_U32 ChanId)
{
    DMX_FilterInfo_S   *FilterInfo;
    DMX_ChanInfo_S     *ChanInfo;

    CHECK_DMX_INIT();
    DMX_CHECK_FILTERID(FilterId);
    DMX_CHECK_CHANID(ChanId);

    FilterInfo  = &DmxDevMgr->DmxFilterInfo[FilterId];
    ChanInfo    = &DmxDevMgr->DmxChanInfo[ChanId];

    if (DMX_INVALID_DMX_ID == FilterInfo->DmxId)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (DMX_INVALID_DMX_ID == ChanInfo->DmxId)
    {
        return HI_ERR_DMX_NOATTACH_FILTER;
    }

    if (FilterInfo->ChanId != ChanId)
    {
        return HI_ERR_DMX_UNMATCH_FILTER;
    }

    DmxHalFilterDetach(FilterId, ChanId);

    FilterInfo->ChanId = DMX_INVALID_CHAN_ID;

    --ChanInfo->FilterCount;

    return HI_SUCCESS;
}

HI_S32 DmxAcquireBuf(const HI_U32 ChanId, const HI_U32 AcquireNum, HI_U32 *AcquiredNum, HI_UNF_DMX_DATA_S *DmxData)
{
    DMX_ChanInfo_S *ChanInfo;
    DMX_OqInfo_S   *OqInfo;
    HI_U32          SectCount   = 0;
    HI_U32          OqWrite;
    HI_U32          EopAddr;

    CHECK_DMX_INIT();
    DMX_CHECK_CHANID(ChanId);
    CHECK_POINTER(AcquiredNum);
    CHECK_POINTER(DmxData);

    *AcquiredNum = 0;

    if (0 == AcquireNum)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    ChanInfo = &DmxDevMgr->DmxChanInfo[ChanId];
    if (DMX_INVALID_DMX_ID == ChanInfo->DmxId)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (DMX_CHAN_STATUS_CLOSE == ChanInfo->ChanStatus)
    {
        return HI_ERR_DMX_NOT_OPEN_CHAN;
    }

    OqInfo = &DmxDevMgr->DmxOqInfo[ChanInfo->OQId];

    OqWrite = DmxHalOQGetWrite(ChanInfo->OQId);

    do
    {
        HI_U8  *buf = HI_NULL;
        HI_U32  len = 0;
        HI_U32  sect_len;

        if (OqInfo->OqCurrBlock == OqWrite)
        {
            EopAddr = DmxOQGetEopAddr(ChanInfo->OQId);
            if ((0 == EopAddr) || (OqInfo->OqCurrOffset == EopAddr))
            {
                break;
            }

            buf = (HI_U8*)(DmxHalOQGetBBSAddr(ChanInfo->OQId) + OqInfo->OqCurrOffset);
            len = EopAddr - OqInfo->OqCurrOffset;

            OqWrite = DmxHalOQGetWrite(ChanInfo->OQId);
            if (OqInfo->OqCurrBlock != OqWrite)
            {
                continue;
            }
        }
        else
        {
            OQ_DescInfo_S *OqDescInfo = (OQ_DescInfo_S*)(OqInfo->OqDescAddr + OqInfo->OqCurrBlock * DMX_OQ_DESC_SIZE);

            buf = (HI_U8*)(OqDescInfo->StartAddr + OqInfo->OqCurrOffset);

            if (OqDescInfo->DataSize == OqInfo->OqCurrOffset)
            {
                DMXINC(OqInfo->OqCurrBlock, OqInfo->OqDescCount);

                OqInfo->OqCurrOffset = 0;

                continue;
            }

            len = OqDescInfo->DataSize - OqInfo->OqCurrOffset;
        }

        if (!DmxOQBlockSectionCheck(buf, len))
        {
            HI_ERR_DEMUX("data error of block, buf=0x%p, len=0x%x\n", buf, len);

            if (OqInfo->OqCurrBlock != OqWrite)
            {
                DMXINC(OqInfo->OqCurrBlock, OqInfo->OqDescCount);

                OqInfo->OqCurrOffset = 0;

                continue;
            }
            else
            {
                break;
            }
        }

        do
        {
            sect_len = SECTION_LENGTH(buf[1], buf[2]);

            len -= sect_len;

            OqInfo->OqCurrOffset += sect_len;

            if (sect_len < MIN_SECTION_LENGTH_WITH_CRC)
            {
                if (   (HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD == ChanInfo->CrcMode)
                    || ((HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD == ChanInfo->CrcMode) && (buf[1] & 0x80)) )
                {
                    buf += sect_len;

                    continue;
                }
            }

            DmxData[SectCount].pu8Data      = buf;
            DmxData[SectCount].u32Size      = sect_len;
            DmxData[SectCount].enDataType   = HI_UNF_DMX_DATA_TYPE_WHOLE;

            buf += sect_len;

            if (++SectCount == AcquireNum)
            {
                break;
            }
        } while (len > 0);
    } while (SectCount < AcquireNum);

    *AcquiredNum = SectCount;

    return SectCount ? HI_SUCCESS : HI_ERR_DMX_NOAVAILABLE_DATA;
}

HI_S32 DmxReleaseBuf(const HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo;

    CHECK_DMX_INIT();
    DMX_CHECK_CHANID(ChanId);

    ChanInfo = &DmxDevMgr->DmxChanInfo[ChanId];
    if (DMX_INVALID_DMX_ID == ChanInfo->DmxId)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (DMX_CHAN_STATUS_CLOSE == ChanInfo->ChanStatus)
    {
        return HI_ERR_DMX_NOT_OPEN_CHAN;
    }

    DmxOQRelease(ChanInfo->OQId, ChanInfo->FQId, HI_FALSE);

    return HI_SUCCESS;
}

