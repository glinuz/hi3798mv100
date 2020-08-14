
#ifndef __DEMUX_FUNC_H__
#define __DEMUX_FUNC_H__

#include "hi_unf_demux.h"

HI_S32  DmxInit(HI_VOID);
HI_VOID DmxDeInit(HI_VOID);

HI_S32  DmxAttachTunerPort(const HI_U32 DmxId, const HI_U32 PortId);
HI_S32  DmxDetachPort(const HI_U32 DmxId);

HI_S32  DmxChannelCreate(const HI_U32 DmxId, const HI_UNF_DMX_CHAN_ATTR_S *ChanAttr, HI_U32 *ChanId);
HI_S32  DmxChannelDestroy(const HI_U32 ChanId);
HI_S32  DmxChannelSetPid(const HI_U32 ChanId, const HI_U32 Pid);
HI_S32  DmxChannelOpen(const HI_U32 ChanId);
HI_S32  DmxChannelClose(const HI_U32 ChanId);

HI_S32  DmxFilterCreate(const HI_U32 DmxId, const HI_UNF_DMX_FILTER_ATTR_S *FilterAttr, HI_U32 *FilterId);
HI_S32  DmxFilterDestroy(const HI_U32 FilterId);
HI_S32  DmxFilterAttach(const HI_U32 FilterId, const HI_U32 ChanId);
HI_S32  DmxFilterDetach(const HI_U32 FilterId, const HI_U32 ChanId);

HI_S32  DmxAcquireBuf(const HI_U32 ChanId, const HI_U32 AcquireNum, HI_U32 *AcquiredNum, HI_UNF_DMX_DATA_S *DmxData);
HI_S32  DmxReleaseBuf(const HI_U32 ChanId);

#endif

