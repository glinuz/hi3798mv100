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



#ifndef _DX_PAL_MEM_INT_H
#define _DX_PAL_MEM_INT_H


#ifdef __cplusplus
extern "C"
{
#endif

//modified by lixiaojie, 2012/11/1/15:46,delete stdlib.h and string.h, on fpga, the standard head file doesn't need
//#include <stdlib.h>
//#include <string.h>
#include "memmove.h"/*2013.1.18 added*/
#include "mem.h"/*2013.1.18 added*/
/** 
* @brief File Description:
*        This file conatins the implementation for memory operations APIs. 
*        The functions implementations are generally just wrappers to different operating system calls.
*/


/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/

/** 
 * @brief A wrapper over memcmp functionality. The function compares two given buffers
 *        according to size.
 */
#define _DX_PAL_MemCmp        memcmp
		                      
/** 
 * @brief A wrapper over memmove functionality, the function copies from one 
 *        buffer to another according to given size
 *         
 */
#define _DX_PAL_MemCopy       memmove

#define	_DX_PAL_MemMove	      memmove
		                  
/** 
 * @brief A wrapper over memset functionality, the function sets a buffer with given value
 *        according to size
 *         
 */
#define _DX_PAL_MemSet(aTarget, aChar, aSize)        memset(aTarget, aChar, aSize)		   

/** 
 * @brief A wrapper over memset functionality, the function sets a buffer with zeroes
 *        according to size
 *         
 */
#define _DX_PAL_MemSetZero(aTarget, aSize)    _DX_PAL_MemSet(aTarget,0x00, aSize)
		                  	  		                  
		                                          
    
#ifdef __cplusplus
}
#endif

#endif


