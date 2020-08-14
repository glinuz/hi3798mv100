/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hal_aiao_func.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/09/22
Last Modified :
Description   : aiao
Function List :
History       :
* main\1    2012-09-22   z40717     init.
******************************************************************************/
#ifndef __DRV_AO_AEF_H__
#define __DRV_AO_AEF_H__

#include "hi_drv_ao.h"
#include "hal_aoe_func.h"
#include "hal_aoe.h"
#include "drv_ao_private.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

/***************************** Struct Definition ******************************/
/*audio effect proc attr*/
typedef struct
{
    MMZ_BUFFER_S        stProcMMz;
    AO_AEF_PROC_ITEM_S  *pstProcItem;
} SND_AEF_PROC_ATTR_S;

/******************************AEF process FUNC*************************************/

HI_S32 AEF_AttachSnd(SND_CARD_STATE_S *pCard, HI_U32 u32AefId, AO_AEF_ATTR_S* pstAefAttr, HI_U32* pu32AefProcAddr);
HI_S32 AEF_DetachSnd(SND_CARD_STATE_S *pCard, HI_U32 u32AefId, AO_AEF_ATTR_S* pstAefAttr);
HI_S32 AEF_GetSetting(SND_CARD_STATE_S *pCard, SND_CARD_SETTINGS_S* pstSndSettings);
HI_S32 AEF_RestoreSetting(SND_CARD_STATE_S *pCard, SND_CARD_SETTINGS_S* pstSndSettings);

#if (1 == HI_PROC_SUPPORT)
HI_S32 AEF_ReadProc(struct seq_file* p, SND_CARD_STATE_S *pCard);
HI_S32 AEF_WriteProc(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf);
#endif

HI_S32 AEF_GetBufAttr(SND_CARD_STATE_S* pCard, AO_AEF_BUF_ATTR_S* pstAefBuf);
HI_S32 AEF_GetDebugAddr(SND_CARD_STATE_S* pCard, AEF_DEBUG_ATTR_S *pstDebugAd);
HI_VOID AEF_DestroyDebugAddr(SND_CARD_STATE_S* pCard);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  // __DRV_AO_AEF_H__