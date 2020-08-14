/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : drv_descrambler_func.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2013/04/16
 Description   :
******************************************************************************/

#ifndef __DRV_DESCRAMBLER_FUNC_H__
#define __DRV_DESCRAMBLER_FUNC_H__

#include "hi_unf_descrambler.h"

#include "drv_demux_define.h"

#ifdef __cplusplus
extern "C" {
#endif

HI_VOID DescramblerReset(Dmx_Set_S * DmxSet, HI_U32 KeyId, DMX_KeyInfo_S *KeyInfo);
HI_VOID DescInitHardFlag(Dmx_Set_S * DmxSet);
HI_VOID DmxDescramblerResume(Dmx_Set_S *DmxSet);

HI_S32 DmxGetChnInstance(Dmx_Set_S *DmxSet, HI_U32 ChnId);
HI_VOID DmxPutChnInstance(Dmx_Set_S *DmxSet, HI_U32 ChnId);
HI_S32 DmxGetKeyInstance(Dmx_Set_S *DmxSet, HI_U32 KeyId);
HI_VOID DmxPutKeyInstance(Dmx_Set_S *DmxSet, HI_U32 KeyId);
HI_S32  DMX_OsiDescramblerCreate(HI_U32 DmxId, HI_U32 *KeyId, const HI_UNF_DMX_DESCRAMBLER_ATTR_S *DescAttr);
HI_S32  DMX_OsiDescramblerDestroy(HI_U32 DmxId, HI_U32 KeyId);
HI_S32  DMX_OsiDescramblerGetAttr(HI_U32 DmxId, HI_U32 KeyId, HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstDesramblerAttr);
HI_S32  DMX_OsiDescramblerSetAttr(HI_U32 DmxId, HI_U32 KeyId, HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstDesramblerAttr);
HI_S32  DMX_OsiDescramblerSetKey(HI_U32 DmxId, HI_U32 KeyId, HI_U32 KeyType, HI_U8 *Key);
HI_S32  DMX_OsiDescramblerSetIVKey(HI_U32 DmxId, HI_U32 KeyId, HI_U32 KeyType, HI_U8 *Key);
HI_S32  DMX_OsiDescramblerAttach(HI_U32 DmxId, HI_U32 KeyId, HI_U32 ChanId);
HI_S32  __DMX_OsiDescramblerDetach(HI_U32 DmxId, HI_U32 KeyId, HI_U32 ChanId);
HI_S32  DMX_OsiDescramblerDetach(HI_U32 DmxId, HI_U32 KeyId, HI_U32 ChanId);
HI_S32  DMX_OsiDescramblerGetFreeKeyNum(HI_U32 DmxId, HI_U32 *FreeCount);
HI_S32  DMX_OsiDescramblerGetKeyId(HI_U32 DmxId, HI_U32 ChanId, HI_U32 *KeyId);

#ifdef __cplusplus
}
#endif

#endif  // __DRV_DESCRAMBLER_FUNC_H__

