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



#ifndef _DX_PAL_MEMALLOC_INT_H
#define _DX_PAL_MEMALLOC_INT_H


#ifdef __cplusplus
extern "C"
{
#endif

//modified by lixiaojie , 2012/11/1/17:47
//#include <stdlib.h>
/** 
* @brief File Description:
*        This file contains wrappers for memory operations APIs. 
*/


/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/

/** 
 * @brief A wrapper over malloc functionality. The function allocates a buffer according to given size
 *
 */
 #define _DX_PAL_MemMalloc    malloc


/** 
 * @brief A wrapper over realloc functionality. The function allocates and copy a buffer
 *        according to size
 *         
 */
#define _DX_PAL_MemRealloc    realloc


/** 
 * @brief A wrapper over free functionality/ The function will free allocated memory.
 *          
 */
#define _DX_PAL_MemFree       free
 

    
#ifdef __cplusplus
}
#endif

#endif


