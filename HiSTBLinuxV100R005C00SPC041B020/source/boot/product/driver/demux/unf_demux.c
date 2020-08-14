
#include "hi_type.h"

#include "hi_unf_demux.h"
#include "hi_error_mpi.h"
#include "demux_func.h"
#include "demux_config.h"

HI_S32 HI_UNF_DMX_Init(HI_VOID)
{
    return DmxInit();
}

HI_S32 HI_UNF_DMX_DeInit(HI_VOID)
{
    DmxDeInit();

    return HI_SUCCESS;
}

#define GetRawPortId(enPortId) ({ \
    HI_U32 RawPortId; \
    switch (enPortId) \
    { \
        case HI_UNF_DMX_PORT_IF_0 :\
        case HI_UNF_DMX_PORT_IF_1 :\
        case HI_UNF_DMX_PORT_IF_2 :\
        case HI_UNF_DMX_PORT_IF_3 :\
        case HI_UNF_DMX_PORT_IF_4 :\
        case HI_UNF_DMX_PORT_IF_5 :\
        case HI_UNF_DMX_PORT_IF_6 :\
        case HI_UNF_DMX_PORT_IF_7 :\
            RawPortId = (HI_U32)enPortId - HI_UNF_DMX_PORT_IF_0;\
            RawPortId = RawPortId >= HI_DMX_IFPORT_CNT ? -1 : RawPortId; \
            break;\
        case HI_UNF_DMX_PORT_TSI_0 :\
        case HI_UNF_DMX_PORT_TSI_1 :\
        case HI_UNF_DMX_PORT_TSI_2 :\
        case HI_UNF_DMX_PORT_TSI_3 :\
        case HI_UNF_DMX_PORT_TSI_4 :\
        case HI_UNF_DMX_PORT_TSI_5 :\
        case HI_UNF_DMX_PORT_TSI_6 :\
        case HI_UNF_DMX_PORT_TSI_7 : \
            RawPortId = (HI_U32)enPortId - HI_UNF_DMX_PORT_TSI_0;\
            RawPortId = RawPortId >= HI_DMX_TSIPORT_CNT ? -1 : RawPortId + HI_DMX_IFPORT_CNT; \
            break;\
        default: \
              return HI_ERR_DMX_INVALID_PARA; \
    } \
    RawPortId; \
})

HI_S32 HI_UNF_DMX_AttachTSPort(HI_U32 u32DmxId, HI_UNF_DMX_PORT_E enPortId)
{
    HI_U32 PortId;

    PortId = GetRawPortId(enPortId);

    return DmxAttachTunerPort(u32DmxId, PortId);
}

HI_S32 HI_UNF_DMX_DetachTSPort(HI_U32 u32DmxId)
{
    return DmxDetachPort(u32DmxId);
}

HI_S32 HI_UNF_DMX_GetTSPortAttr(HI_UNF_DMX_PORT_E enPortId, HI_UNF_DMX_PORT_ATTR_S *pstAttr)
{
    HI_U32 PortId;

    PortId = GetRawPortId(enPortId);

    return DmxGetTSPortAttr(PortId, pstAttr);
}

HI_S32 HI_UNF_DMX_SetTSPortAttr(HI_UNF_DMX_PORT_E enPortId, const HI_UNF_DMX_PORT_ATTR_S *pstAttr)
{
    HI_U32 PortId;

    PortId = GetRawPortId(enPortId);

    return DmxSetTSPortAttr(PortId, pstAttr);
}

HI_S32 HI_UNF_DMX_CreateChannel(HI_U32 u32DmxId, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr, HI_HANDLE *phChannel)
{
    return DmxChannelCreate(u32DmxId, pstChAttr, (HI_U32*)phChannel);
}

HI_S32 HI_UNF_DMX_DestroyChannel(HI_HANDLE hChannel)
{
    return DmxChannelDestroy((HI_U32)hChannel);
}

HI_S32 HI_UNF_DMX_SetChannelPID(HI_HANDLE hChannel, HI_U32 u32Pid)
{
    return DmxChannelSetPid((HI_U32)hChannel, u32Pid);
}

HI_S32 HI_UNF_DMX_OpenChannel(HI_HANDLE hChannel)
{
    return DmxChannelOpen((HI_U32)hChannel);
}

HI_S32 HI_UNF_DMX_CloseChannel(HI_HANDLE hChannel)
{
    return DmxChannelClose((HI_U32)hChannel);
}

HI_S32 HI_UNF_DMX_CreateFilter(HI_U32 u32DmxId, const HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr, HI_HANDLE *phFilter)
{
    return DmxFilterCreate(u32DmxId, pstFilterAttr, (HI_U32*)phFilter);
}

HI_S32 HI_UNF_DMX_DestroyFilter(HI_HANDLE hFilter)
{
    return DmxFilterDestroy((HI_U32)hFilter);
}

HI_S32 HI_UNF_DMX_AttachFilter(HI_HANDLE hFilter, HI_HANDLE hChannel)
{
    return DmxFilterAttach((HI_U32)hFilter, (HI_U32)hChannel);
}

HI_S32 HI_UNF_DMX_DetachFilter(HI_HANDLE hFilter, HI_HANDLE hChannel)
{
    return DmxFilterDetach((HI_U32)hFilter, (HI_U32)hChannel);
}

HI_S32 HI_UNF_DMX_AcquireBuf(
        HI_HANDLE           hChannel,
        HI_U32              u32AcquireNum,
        HI_U32             *pu32AcquiredNum,
        HI_UNF_DMX_DATA_S  *pstBuf,
        HI_U32              u32TimeOutMs
    )
{
    return DmxAcquireBuf((HI_U32)hChannel, u32AcquireNum, pu32AcquiredNum, pstBuf);
}

HI_S32 HI_UNF_DMX_ReleaseBuf(HI_HANDLE hChannel, HI_U32 u32ReleaseNum, HI_UNF_DMX_DATA_S *pstBuf)
{
    return DmxReleaseBuf((HI_U32)hChannel);
}

