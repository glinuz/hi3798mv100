/***********************************************************************************
*              Copyright 2004 - 2015, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  tee_hal_demux.h
* Description:  Define demux hal interface.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20151012    NA                 NULL      Create this file.
***********************************************************************************/

#ifndef __TEE_HAL_DEMUX_H__
#define __TEE_HAL_DEMUX_H__

#include "tee_drv_demux_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 DmxHalGetInitStatus(Dmx_Set_S *DmxSet);

#ifdef DMX_MMU_BASE
HI_VOID DmxHalSetSecurePgTable(Dmx_Set_S *DmxSet);
#else
static inline HI_VOID DmxHalSetSecurePgTable(Dmx_Set_S *DmxSet){};
#endif

HI_VOID DmxHalSetChannelCWIndex(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 cwIndex);
HI_VOID DmxHalSetChanCwTabId(Dmx_Set_S * DmxSet, HI_U32 ChanId, HI_U32 TabId);
HI_VOID DmxHalSetChannelDsc(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_BOOL Enable);
HI_U32 DmxHalGetChannelPid(Dmx_Set_S *DmxSet, HI_U32 ChanId);
HI_VOID DmxHalSetEntropyReduction(Dmx_Set_S *DmxSet, HI_U32 KeyId, HI_TEE_DMX_CA_ENTROPY_E EntropyReduction);
HI_U32 DmxHalGetOptCAType(Dmx_Set_S * DmxSet);
HI_VOID DmxHalSetCWWord(Dmx_Set_S *DmxSet,
        HI_U32                          GroupId,
        HI_U32                          WordId,
        HI_U32                          Key,
        DMX_KEY_TYPE_E                  KeyType,
        DMX_CW_TYPE                     CWType,
        HI_TEE_DMX_DESCRAMBLER_TYPE_E   DescType
    );

#ifdef DMX_SECURE_CHANNEL_SUPPORT
HI_S32 DmxHalSetSecureOq(Dmx_Set_S * DmxSet, HI_U32 OqId);
HI_S32 DmxHalClrSecureOq(Dmx_Set_S * DmxSet, HI_U32 OqId);
#else
static inline HI_S32 DmxHalSetSecureOq(Dmx_Set_S * DmxSet, HI_U32 OqId)
{
    HI_ERR_DEMUX("DMX_SECURE_CHANNEL_SUPPORT not configured.\n");
    return HI_FAILURE;
}
static inline HI_S32 DmxHalClrSecureOq(Dmx_Set_S * DmxSet, HI_U32 OqId)
{
    HI_ERR_DEMUX("DMX_SECURE_CHANNEL_SUPPORT not configured.\n");
    return HI_FAILURE;
}
#endif

#ifdef DMX_SECURE_KEY_SUPPORT
HI_VOID DmxHalSetSecureKey(Dmx_Set_S *DmxSet, HI_U32 KeyId);
HI_VOID DmxHalLockSecureKey(Dmx_Set_S *DmxSet);
#else
static inline HI_VOID DmxHalSetSecureKey(Dmx_Set_S *DmxSet, HI_U32 KeyId){};
static inline HI_VOID DmxHalLockSecureKey(Dmx_Set_S *DmxSet){};
#endif

#ifdef DMX_SECURE_ENCRYPT_KEY_SUPPORT
HI_S32 DmxHalSetSecureEncryptKey(Dmx_Set_S *DmxSet, HI_U32 KeyId);
HI_S32 DmxHalClrSecureEncryptKey(Dmx_Set_S *DmxSet, HI_U32 KeyId);
HI_S32 DmxHalSetReplaceScFlag(Dmx_Set_S *DmxSet, HI_U32 DmxId);
HI_S32 DmxHalClrReplaceScFlag(Dmx_Set_S *DmxSet, HI_U32 DmxId);
HI_S32 DmxHalSetActiveEncryptKey(Dmx_Set_S *DmxSet, HI_U32 KeyId, HI_BOOL IsEvenKey);
#else
static inline HI_S32 DmxHalSetSecureEncryptKey(Dmx_Set_S *DmxSet, HI_U32 KeyId)
{
    HI_ERR_DEMUX("DMX_SECURE_ENCRYPT_KEY_SUPPORT not configured.\n");
    return HI_FAILURE;
}

static inline HI_S32 DmxHalClrSecureEncryptKey(Dmx_Set_S *DmxSet, HI_U32 KeyId)
{
    HI_ERR_DEMUX("DMX_SECURE_ENCRYPT_KEY_SUPPORT not configured.\n");
    return HI_FAILURE;
}

static inline HI_S32 DmxHalSetReplaceScFlag(Dmx_Set_S *DmxSet, HI_U32 DmxId)
{
    HI_ERR_DEMUX("DMX_SECURE_ENCRYPT_KEY_SUPPORT not configured.\n");
    return HI_FAILURE;
}

static inline HI_S32 DmxHalClrReplaceScFlag(Dmx_Set_S *DmxSet, HI_U32 DmxId)
{
    HI_ERR_DEMUX("DMX_SECURE_ENCRYPT_KEY_SUPPORT not configured.\n");
    return HI_FAILURE;
}

static inline HI_S32 DmxHalSetActiveEncryptKey(Dmx_Set_S *DmxSet, HI_U32 KeyId, HI_BOOL IsEvenKey)
{
    HI_ERR_DEMUX("DMX_SECURE_ENCRYPT_KEY_SUPPORT not configured.\n");
    return HI_FAILURE;
}
#endif

#ifdef DMX_SECURE_RAM_PORT_SUPPORT
HI_S32 DmxHalSetSecureRamPort(Dmx_Set_S * DmxSet, HI_U32 RamPortId);
HI_S32 DmxHalClrSecureRamPort(Dmx_Set_S * DmxSet, HI_U32 RamPortId);
#else
static inline HI_S32 DmxHalSetSecureRamPort(Dmx_Set_S * DmxSet, HI_U32 RamPortId)
{
    HI_ERR_DEMUX("DMX_SECURE_RAM_PORT_SUPPORT not configured.\n");
    return HI_FAILURE;
}
static inline HI_S32 DmxHalClrSecureRamPort(Dmx_Set_S * DmxSet, HI_U32 RamPortId)
{
    HI_ERR_DEMUX("DMX_SECURE_RAM_PORT_SUPPORT not configured.\n");
    return HI_FAILURE;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __TEE_HAL_DEMUX_H__ */