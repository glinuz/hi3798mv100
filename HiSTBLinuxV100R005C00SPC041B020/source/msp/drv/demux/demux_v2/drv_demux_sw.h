
#ifndef __DRV_DEMUX_SW_H__
#define __DRV_DEMUX_SW_H__

#include "drv_demux_define.h"

#ifdef DMX_USE_ECM

#define DMX_SW_CHNBUF_MINLEN    (32 * 1024)
#define DMX_SW_CHNBUF_MAXLEN    (2 * 1024 * 1024)
#define DMX_SW_INVALID_CC       (0xff)
#define DMX_SW_OVERFL_PERCENT   (5) //if channel buffer < 5%,do not read data

HI_S32 HI_DMX_SwInit(HI_VOID);
HI_S32 HI_DMX_SwNewChannel(HI_U32 u32ChannelId);
HI_S32 HI_DMX_SwDestoryChannel(HI_U32 u32ChannelId);
HI_S32 HI_DMX_SwOpenChannel(HI_U32 u32ChannelId);
HI_S32 HI_DMX_SwCloseChannel(HI_U32 u32ChannelId);
HI_S32 HI_DMX_SwReadDataRequest(HI_U32         u32ChId,
                                HI_U32         u32AcqNum,
                                HI_U32 *       pu32AcqedNum,
                                DMX_UserMsg_S* psMsgList,
                                HI_U32         u32TimeOutMs);
HI_S32 HI_DMX_SwPeekDataRequest(HI_U32 u32ChId, HI_U32 u32PeekLen, DMX_UserMsg_S* psMsgList);
HI_S32 HI_DMX_SwReleaseReadData(HI_U32 u32ChId, HI_U32 u32RelNum, DMX_UserMsg_S* psMsgList);
HI_S32 HI_DMX_SwGetChannelDataStatus(HI_U32 u32ChId);
HI_S32 HI_DMX_SwGetChannelSwBufAddr(HI_U32 u32ChId, MMZ_BUFFER_S* pstSwBuf);

#endif

#endif

