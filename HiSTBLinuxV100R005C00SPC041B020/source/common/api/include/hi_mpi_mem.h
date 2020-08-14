/******************************************************************************
Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_mpi_mem.h
Version       : V1.0 Initial Draft
Author        : g00182102
Created       : 2012/6/19
Last Modified :
Description   : The system heap memory allocate interfaces.
Function List : None.
History       :
******************************************************************************/
#ifndef __HI_MPI_MEM_H__
#define __HI_MPI_MEM_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "hi_type.h"

/** @addtogroup H_MPI_MEM */
/** @{ */

/**
@brief malloc the pointed size from system heap. 
@attention None
@param[in] u32ModuleID The module ID, who need to request memory.
@param[in] u32Size The size of requesting.
@param[out] None
@retval ::Valid memory address Success
@retval ::NULL Failure
@see \n
N/A
*/
HI_VOID* HI_MALLOC(HI_U32 u32ModuleID, HI_U32 u32Size);


/**
@brief Free the requsted memory by hi_malloc. 
@attention when stopping to use the memory, calling this interface.
@param[in] u32ModuleID The module ID, who need to free memory.
@param[in] pMemAddr The memory address to free
@param[out] None
@retval ::None
@see \n
N/A
*/
HI_VOID HI_FREE(HI_U32 u32ModuleID, HI_VOID* pMemAddr);

/**
@brief Calloc memory, with u32MemBlock blocks and u32Size size per. 
@attention None
@param[in] u32ModuleID The module id, who need to calloc memory.
@param[in] u32MemBlock The requesting block number.
@param[in] u32Size The requesting size per block.
@param[out] None
@retval ::Valid memory address Success
@retval ::NULL Failure
@see \n
N/A
*/
HI_VOID* HI_CALLOC(HI_U32 u32ModuleID, HI_U32 u32MemBlock, HI_U32 u32Size);

HI_VOID* HI_MMAP(HI_U32 phy_addr, HI_U32 size);
HI_S32   HI_MUNMAP(HI_VOID * addr_mapped);

/** @} */

#ifdef __cplusplus
}
#endif

#endif



