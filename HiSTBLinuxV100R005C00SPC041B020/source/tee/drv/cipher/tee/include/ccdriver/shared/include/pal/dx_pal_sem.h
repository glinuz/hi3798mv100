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



#ifndef _DX_PAL_SEM_H
#define _DX_PAL_SEM_H
#include "dx_pal_sem_plat.h"

#define DX_PAL_SEM_NO_WAIT          0
#define DX_PAL_SEM_FREE				1
#define DX_PAL_SEM_LOCKED			0


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




/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/
		
/** 
 * @brief This function purpose is to create a semaphore.
 *         
 *
 * @param[out] aSemId - Pointer to created semaphor handle
 * @param[in] aInitialVal - Initial semaphore value 
 *
 * @return The return values will be according to operating system return values.
 */		
//DxError_t DX_PAL_SemCreate( DX_PAL_SEM *aSemId, uint32_t aInitialVal );
#define DX_PAL_SemCreate _DX_PAL_SemCreate
/** 
 * @brief This function purpose is to delete a semaphore
 *         
 *
 * @param[in] aSemId - Semaphore handle 
 *
 * @return The return values will be according to operating system return values.
 */
//DxError_t DX_PAL_SemDelete( DX_PAL_SEM *aSemId );
#define DX_PAL_SemDelete _DX_PAL_SemDelete
/** 
 * @brief This function purpose is to Wait for semaphore with aTimeOut. aTimeOut is 
 *        specified in milliseconds.
 *         
 *
 * @param[in] aSemId - Semaphore handle
 * @param[in] aTimeOut - timeout in mSec, or DX_INFINITE
 *
 * @return The return values will be according to operating system return values.
 */
//DxError_t DX_PAL_SemWait(DX_PAL_SEM aSemId, uint32_t aTimeOut);
#define DX_PAL_SemWait _DX_PAL_SemWait
/** 
 * @brief This function purpose is to signal the semaphore.
 *         
 *
 * @param[in] aSemId - Semaphore handle 
 *
 * @return The return values will be according to operating system return values.
 */
//DxError_t DX_PAL_SemGive(DX_PAL_SEM aSemId);

#define DX_PAL_SemGive _DX_PAL_SemGive



    
#ifdef __cplusplus
}
#endif

#endif


