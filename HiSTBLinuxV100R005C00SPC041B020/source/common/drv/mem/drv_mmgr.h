/******************************************************************************
Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : kmodule_mem_mgr.h
Version       : V1.0 Initial Draft
Author        : g00182102
Created       : 2012/6/19
Last Modified :
Description   : In kernel mode, memory pool manager initialize interfaces for inner modules.
Function List : None.
History       :
******************************************************************************/
/** @addtogroup iKMEM */
/** @{ */
#ifndef __KMODULE_MEM_MGR_H__
#define __KMODULE_MEM_MGR_H__

#ifdef __cplusplus
extern "C"
{
#endif

HI_S32 KModule_MemMgr_Init(HI_U32 u32ModuleCount, HI_U32 u32ModuleMemCount);
HI_VOID KModule_MemMgr_Exit(HI_VOID);

#ifdef __cplusplus
}
#endif

#endif
/** @} */
