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



#ifndef _DX_PAL_MUTEX_H
#define _DX_PAL_MUTEX_H
#include "dx_pal_mutex_plat.h"


#ifdef __cplusplus
extern "C"
{
#endif

/** 
* @brief File Description:
*        This file contains functions for resource management (mutex operations). 
*        The functions implementations are generally just wrappers to different operating system calls.
*        None of the described functions will check the input parameters so the behavior 
*        of the APIs in illegal parameters case is dependent on the operating system behavior.
*
*/




/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/
		
/** 
 * @brief This function purpose is to create a mutex.
 *         
 *
 * @param[out] pMutexId - Pointer to created mutex handle
 *
 * @return returns 0 on success, otherwise indicates failure
 */		
DxError_t DX_PAL_MutexCreate(DX_PAL_MUTEX *pMutexId);


/** 
 * @brief This function purpose is to destroy a mutex
 *         
 *
 * @param[in] pMutexId - pointer to Mutex handle 
 *
 * @return returns 0 on success, otherwise indicates failure
 */
DxError_t DX_PAL_MutexDestroy(DX_PAL_MUTEX *pMutexId);


/** 
 * @brief This function purpose is to Wait for Mutex with aTimeOut. aTimeOut is 
 *        specified in milliseconds. (DX_INFINITE is blocking)
 *         
 *
 * @param[in] pMutexId - pointer to Mutex handle 
 * @param[in] aTimeOut - timeout in mSec, or DX_INFINITE
 *
 * @return returns 0 on success, otherwise indicates failure
 */
DxError_t DX_PAL_MutexLock (DX_PAL_MUTEX *pMutexId, uint32_t aTimeOut);



/** 
 * @brief This function purpose is to release the mutex.
 *         
 *
 * @param[in] pMutexId - pointer to Mutex handle 
 *
 * @return returns 0 on success, otherwise indicates failure
 */
DxError_t DX_PAL_MutexUnlock (DX_PAL_MUTEX *pMutexId);




    
#ifdef __cplusplus
}
#endif

#endif


