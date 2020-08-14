/******************************************************************************
Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : module_mem_mgr.h
Version       : V1.0 Initial Draft
Author        : g00182102
Created       : 2012/6/19
Last Modified :
Description   : The module memory manager
Function List : None.
History       :
******************************************************************************/
/** @addtogroup iMEM */
/** @{ */

#ifndef __HI_MODULE_MEM_MGR_H__
#define __HI_MODULE_MEM_MGR_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "hi_type.h"

#include "drv_module_ioctl.h"

// !! Bytes:20
typedef struct tagModuleMemInfo
{
    HI_U32 u32ModuleID;

    HI_U32 u32SizeUsrMem;
    HI_U32 u32SizeKernelMem;
    HI_U32 u32SizeMMZ;

    HI_U32 u32MaxMemSize;
}MODULE_MEM_INFO_S;

// !! bytes: 24
typedef struct tagModuleMem
{
    MODULE_MEM_INFO_S stModuleMem;
    struct tagModuleMem* pNext;
}MODULE_MEM_S;

// !! bytes:28
typedef struct tagMemMGR
{
    HI_U32       u32Idle;
    
    MODULE_MEM_S stModuleMemItem;    
}MODULE_MEM_POOL_S;

/**
@brief Initialize this module memory manager moudle, which will alloc u32ModuleCount
       items for holding the registered modules 
@attention Before calling other interfaces of this module, calling this interface.
@param[in] u32ModuleCount module total number.
@param[out] None
@retval ::HI_SUCCESS Success
@retval ::HI_FAILURE Failure
@see \n
N/A
*/
HI_S32 Module_Mem_Adp_Init(HI_U32 u32ModuleCount);

/**
@brief Terminate this module 
@attention N/A
@param[in] None.
@param[out] None
@retval ::HI_SUCCESS Success
@retval ::HI_FAILURE Failure
@see \n
N/A
*/
HI_S32 Module_Mem_Adp_DeInit(HI_VOID);

/**
@brief Add module info into the link table, mainly the module ID etc. 
@attention N/A
@param[in] pstModule module attirbutes info.
@param[out] None
@retval ::HI_SUCCESS Success
@retval ::HI_FAILURE Failure
@see \n
N/A
*/
HI_S32 Module_Mem_AddModuleInfo(MODULE_INFO_S* pstModule);

/**
@brief Delete module from into the link table. 
@attention N/A
@param[in] pstModule module attirbutes info.
@param[out] None
@retval ::HI_SUCCESS Success
@retval ::HI_FAILURE Failure
@see \n
N/A
*/
HI_S32 Module_Mem_DelModuleInfo(MODULE_INFO_S* pstModule);


#ifdef __cplusplus
}
#endif
#endif
/** @} */
