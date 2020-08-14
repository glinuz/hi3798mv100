/******************************************************************************
Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_drv_module.h
Version       : V1.0 Initial Draft
Author        : g00182102
Created       : 2012/6/19
Last Modified :
Description   : The module manager.
Function List : None.
History       :
******************************************************************************/

#ifndef __HI_DRV_MODULE_H__
#define __HI_DRV_MODULE_H__

#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define HI_KMODULE_MAX_COUNT      (256)
#define HI_KMODULE_MEM_MAX_COUNT  (256*256)

#define HI_FATAL_MODULE(fmt...)    HI_FATAL_PRINT(HI_ID_MODULE, fmt)
#define HI_ERR_MODULE(fmt...)      HI_ERR_PRINT(HI_ID_MODULE, fmt)
#define HI_WARN_MODULE(fmt...)     HI_WARN_PRINT(HI_ID_MODULE, fmt)
#define HI_INFO_MODULE(fmt...)     HI_INFO_PRINT(HI_ID_MODULE, fmt)

HI_S32  HI_DRV_MMNGR_Init(HI_U32 u32ModuleCount, HI_U32 u32ModuleMemCount);
HI_VOID HI_DRV_MMNGR_Exit(HI_VOID);

HI_S32 HI_DRV_MODULE_AllocId(HI_U8* pu8ModuleName, HI_U32 *pu32ModuleID, HI_S32 *ps32Status);
HI_S32 HI_DRV_MODULE_Register(HI_U32 u32ModuleID, const HI_U8* pu8ModuleName, HI_VOID* pFunc);
HI_S32 HI_DRV_MODULE_UnRegister(HI_U32 u32ModuleID);

#ifdef CMN_MMGR_SUPPORT
HI_U8* HI_DRV_MODULE_GetNameByID(HI_U32 u32ModuleID);
HI_U32 HI_DRV_MODULE_GetIDByName(HI_U8* pu8Name);
#endif

HI_S32 HI_DRV_MODULE_GetFunction(HI_U32 u32ModuleID, HI_VOID** ppFunc);

HI_S32  MMNGR_DRV_ModInit(HI_U32 u32ModuleCount, HI_U32 u32ModuleMemCount);
HI_VOID MMNGR_DRV_ModExit(HI_VOID);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_DRV_MODULE_H__ */

