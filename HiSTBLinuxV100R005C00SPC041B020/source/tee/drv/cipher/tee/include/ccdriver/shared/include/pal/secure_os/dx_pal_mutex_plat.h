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



#ifndef _DX_PAL_MUTEX_INT__H
#define _DX_PAL_MUTEX_INT__H



#ifdef __cplusplus
extern "C"
{
#endif
/** 
* @brief File Description:
*        This file contains functions for resource management (semaphor operations). 
*        The functions implementations are generally just wrappers to different operating system calls.
*        None of the described functions will check the input parameters so the behavior 
*        of the APIs in illegal parameters case is dependent on the operating system behavior.
*
*/

typedef uint32_t DX_PAL_MUTEX;



/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/
		
    
#ifdef __cplusplus
}
#endif

#endif


