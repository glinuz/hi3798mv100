/******************************************************************************
Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : module_mem_utils.h
Version       : V1.0 Initial Draft
Author        : g00182102
Created       : 2012/6/19
Last Modified :
Description   : The module memory utils, likely factory for creating link.
Function List : None.
History       :
******************************************************************************/

#ifndef __MEM_UTILS_H__
#define __MEM_UTILS_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "hi_type.h"

typedef enum {
    MEM_POOL_TYPE_MODULE,        /* module manager request memory pool */
    MEM_POOL_TYPE_MODULE_MEMORY, /* module memory request memory pool  */
    MEM_POOL_TYPE_USR_MEMORY,    /* memory wrap request memory pool     */
    MEM_POOL_TYPE_MMZ,
    MEM_POOL_TYPE_LOG,

    MEM_POOL_TYPE_BUTT
}UTILS_MEM_POOL_TYPE_E;


typedef struct tagMemUtils{
    UTILS_MEM_POOL_TYPE_E enType;

    HI_U32                u32ItemCount;
    HI_U32                u32HasCount;

    HI_VOID*              pMemBaseAddr;
}MEM_UTILS_S;

/**
@brief Initialize this utils module, which will alloc u32Count stMem ITEMS.
@attention Before calling other interfaces of this module, calling this interface.
@param[in] u32Count the request ITEMS number.
@param[out] None
@retval ::utils handle Success
@retval ::0 Failure
@see \n
N/A
*/
HI_HANDLE Mem_Utils_Init(HI_U32 u32Count, MEM_UTILS_S stMem);

/**
@brief Terminate this module.
@attention N/A
@param[in] hUtils The handle returned by Init.
@param[out] None
@retval ::None.
@see \n
N/A
*/
HI_VOID   Mem_Utils_DeInit(HI_HANDLE hUtils);


/**
@brief Request one position by hUtils from the memory.
@attention N/A
@param[in] hUtils The handle returned by Init.
@param[out] None
@retval ::None.
@see \n
N/A
*/
HI_VOID* Mem_Utils_MALLOC(HI_HANDLE hUtils);

/**
@brief Release one position by hUtils to the memory.
@attention N/A
@param[in] hUtils The handle returned by Init.
@param[out] None
@retval ::None.
@see \n
N/A
*/
HI_VOID  Mem_Utils_FREE(HI_HANDLE hUtils, HI_VOID* pAddr);


/**
@brief Get valid item number in the memory pool by hUtils.
@attention N/A
@param[in] hUtils The handle returned by Init.
@param[out] None
@retval ::Total valid item number success.
@retval ::0 failure.
@see \n
N/A
*/
HI_U32   Mem_Utils_GetItemNo(HI_HANDLE hUtils);

#ifdef __cplusplus
}
#endif

#endif

