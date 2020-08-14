/******************************************************************************
Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : kmodule_mem.h
Version       : V1.0 Initial Draft
Author        : g00182102
Created       : 2012/6/19
Last Modified :
Description   : In kernel mode, memory pool initialize interfaces for inner modules.
Function List : None.
History       :
******************************************************************************/
/** @addtogroup iKMEM */
/** @{ */
#ifndef __KMODULE_MEM_H__
#define __KMODULE_MEM_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum tagKMemType
{
    KMEM_TYPE_MMZ,
    KMEM_TYPE_KMEM,
    KMEM_TYPE_VMEM,

    KMEM_TYPE_BUTT
}KMEM_TYPE_E;

typedef HI_S32 (* fnKMemStatCallback)(HI_U32 u32ModuleID, KMEM_TYPE_E enMemType, HI_S32 s32Size);


HI_S32 KMODULE_MEM_POOL_DelModule(HI_U32 u32ModuleID);
HI_S32 KMODULE_MEM_POOL_AddModule(HI_U32 u32ModuleID);


HI_S32 KModule_Mem_Init(HI_U32 u32CountMem, fnKMemStatCallback fnCallback);

HI_S32 KModule_Mem_DeInit(HI_VOID);


#ifdef __cplusplus
}
#endif

#endif
