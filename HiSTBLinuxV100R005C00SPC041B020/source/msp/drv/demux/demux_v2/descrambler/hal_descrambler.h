/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hal_descrambler.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2013/04/16
 Description   :
******************************************************************************/

#ifndef __HAL_DESCRAMBLER_H__
#define __HAL_DESCRAMBLER_H__

#include "drv_demux_define.h"
#include "drv_descrambler.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_VOID DmxHalSetChannelCWIndex(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 cwIndex);
HI_VOID DmxHalSetChannelDsc(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_BOOL Enable);

HI_VOID DmxHalSetEntropyReduction(Dmx_Set_S *DmxSet, HI_U32 KeyId, HI_UNF_DMX_CA_ENTROPY_E EntropyReduction);

HI_U32  DmxHalGetOptCAType(Dmx_Set_S * DmxSet);
HI_U32  DmxHalGetOptDescramblerType(HI_VOID);
HI_VOID DmxHalSetCAType(HI_U32 u32KeyId, HI_BOOL bAdvance);
HI_VOID DmxHalSetDescramblerType(HI_U32 KeyId, HI_BOOL bHigh);
HI_VOID DmxHalInitSpeCWOrder(Dmx_Set_S * DmxSet);
HI_VOID DmxHalInitTdesCWOrder(Dmx_Set_S * DmxSet);
HI_VOID DmxHalInitDesCWOrder(Dmx_Set_S * DmxSet);
HI_VOID DmxHalSetCWWord(Dmx_Set_S *DmxSet,
        HI_U32                          GroupId,
        HI_U32                          WordId,
        HI_U32                          Key,
        DMX_KEY_TYPE_E                  KeyType,
        DMX_CW_TYPE                     CWType,
        HI_UNF_DMX_DESCRAMBLER_TYPE_E   DescType
    );
HI_VOID DmxHalSetChanCwTabId(Dmx_Set_S * DmxSet, HI_U32 ChanId, HI_U32 TabId);
HI_VOID DmxHalSetDmxIvEnable(Dmx_Set_S * DmxSet, HI_U32 RawDmxId, HI_BOOL Enable);
HI_VOID DmxHalSetCSA3Reset(HI_BOOL Enable);

#if defined(DMX_SECURE_KEY_SUPPORT) && !defined(DMX_TEE_SUPPORT)
HI_VOID DmxHalSetSecureKey(Dmx_Set_S *DmxSet, HI_U32 KeyId);
#else
static inline HI_VOID DmxHalSetSecureKey(Dmx_Set_S *DmxSet, HI_U32 KeyId){};
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  // __HAL_DESCRAMBLER_H__

