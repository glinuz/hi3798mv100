/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hi_drv_descrambler.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2013/04/16
 Description   :
******************************************************************************/

#ifndef __HI_DRV_DESCRAMBLER_H__
#define __HI_DRV_DESCRAMBLER_H__

#include "hi_type.h"

#include "hi_unf_descrambler.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32  HI_DRV_DMX_CreateDescrambler(HI_U32 DmxId, HI_UNF_DMX_DESCRAMBLER_ATTR_S *DesrAttr, HI_HANDLE *KeyHandle, HI_VOID *file);
HI_S32  HI_DRV_DMX_DestroyDescrambler(HI_HANDLE KeyHandle);
HI_S32  HI_DRV_DMX_GetDescramblerAttr(HI_HANDLE KeyHandle, HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstDecsramblerAttr);
HI_S32  HI_DRV_DMX_SetDescramblerAttr(HI_HANDLE KeyHandle, HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstDescramblerAttr);
HI_S32  HI_DRV_DMX_SetDescramblerEvenKey(HI_HANDLE KeyHandle, HI_U8 *Key);
HI_S32  HI_DRV_DMX_SetDescramblerOddKey(HI_HANDLE KeyHandle, HI_U8 *Key);
HI_S32  HI_DRV_DMX_SetDescramblerEvenIVKey(HI_HANDLE KeyHandle, HI_U8 *Key);
HI_S32  HI_DRV_DMX_SetDescramblerOddIVKey(HI_HANDLE KeyHandle, HI_U8 *Key);
HI_S32  HI_DRV_DMX_AttachDescrambler(HI_HANDLE KeyHandle, HI_HANDLE ChanHandle);
HI_S32  HI_DRV_DMX_DetachDescrambler(HI_HANDLE KeyHandle, HI_HANDLE ChanHandle);
HI_S32  HI_DRV_DMX_GetDescramblerKeyHandle(HI_HANDLE ChanHandle, HI_HANDLE *KeyHandle);
HI_S32  HI_DRV_DMX_GetFreeDescramblerKeyCount(HI_U32 DmxId, HI_U32 *FreeCount);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __HI_DRV_DESCRAMBLER_H__ */

