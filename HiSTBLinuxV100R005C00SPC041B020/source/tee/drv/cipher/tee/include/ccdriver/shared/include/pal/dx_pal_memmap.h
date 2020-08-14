/***************************************************************
*  Copyright 2014 (c) Discretix Technologies Ltd.              *
*  This software is protected by copyright, international      *
*  treaties and various patents. Any copy, reproduction or     *
*  otherwise use of this software must be authorized in a      *
*  license agreement and include this Copyright Notice and any *
*  other notices specified in the license agreement.           *
*  Any redistribution in binary form must be authorized in the *
*  license agreement and include this Copyright Notice and     *
*  any other notices specified in the license agreement and/or *
*  in materials provided with the binary distribution.         *
****************************************************************/



#ifndef _DX_PAL_MEMMAP_H
#define _DX_PAL_MEMMAP_H


#ifdef __cplusplus
extern "C"
{
#endif


#include "dx_pal_types.h"
/** 
* @brief File Description:
*        This file conatins functions for memory mapping 
*        None of the described functions will check the input parameters so the behavior 
*        of the APIs in illegal parameters case is dependent on the operating system behavior.
*
*/


/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/

/** 
 * @brief This function purpose is to return the base virtual address that maps the 
 *        base physical address 
 *
 * @param[in] physicalAddress - Starts physical address of the I/O range to be mapped.
 * @param[in] mapSize - Number of bytes that were mapped
 * @param[out] ppVirtBuffAddr - Pointer to the base virtual address to which the physical pages were mapped
 *
 * @return Returns a non-zero value in case of failure
 */ 	
uint32_t DX_PAL_MemMap(uint32_t physicalAddress,	
	               uint32_t mapSize,	
		       uint32_t **ppVirtBuffAddr);
		                      
		                      
/** 
 * @brief This function purpose is to Unmaps a specified address range previously mapped 
 *        by DX_PAL_MemMap         
 *         
 *
 * @param[in] pVirtBuffAddr - Pointer to the base virtual address to which the physical 
 *            pages were mapped
 * @param[in] mapSize - Number of bytes that were mapped 
 *
 * @return Returns a non-zero value in case of failure
 */ 	
uint32_t DX_PAL_MemUnMap(uint32_t *pVirtBuffAddr,	
	                  uint32_t mapSize);
    
#ifdef __cplusplus
}
#endif

#endif


