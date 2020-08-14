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



#ifndef _DX_PAL_PLAT_H
#define _DX_PAL_PLAT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif


 
/** 
 * @brief   Initializes contiguous memory pool required for DX_PAL_AllocateDmaBuff() and DX_PAL_FreeDmaBuff(). Our 
 *           implementation is to mmap 0x10000000 and call to bpool(), for use of bget() in DX_PAL_AllocateDmaBuff(),
 *           and brel() in DX_PAL_FreeDmaBuff(). 
 *         
 * @param[in] buffSize - buffer size in Bytes
 * @param[in] physBuffAddr - physical start address of the memory to map
 *
 * @return Virtual start address of contiguous memory
 */ 	
extern uint32_t DX_PAL_DmaInit(uint32_t  buffSize,
			       uint32_t  physBuffAddr);

/** 
 * @brief   free system resources created in PD_PAL_DmaInit() 
 *         
 * @param[in] buffSize - buffer size in Bytes 
 *  
 * @return void
 */ 	
extern void DX_PAL_DmaTerminate(void);
#ifdef __cplusplus
}
#endif

#endif


