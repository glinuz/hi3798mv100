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



#ifndef _DX_PAL_SEM_INT__H
#define _DX_PAL_SEM_INT__H



#ifdef __cplusplus
extern "C"
{
#endif
#include "dx_pal_types.h"
/** 
* @brief File Description:
*        This file contains functions for resource management (semaphor operations). 
*        The functions implementations are generally just wrappers to different operating system calls.
*        None of the described functions will check the input parameters so the behavior 
*        of the APIs in illegal parameters case is dependent on the operating system behavior.
*
*/

typedef int DX_PAL_SEM;



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
DxError_t _DX_PAL_SemCreate( DX_PAL_SEM *aSemId, uint32_t aInitialVal );

#define _DX_PAL_SemCreate(aSemId,aInitialVal) DX_SUCCESS
/** 
 * @brief This function purpose is to delete a semaphore
 *         
 *
 * @param[in] aSemId - Semaphore handle 
 *
 * @return The return values will be according to operating system return values.
 */
DxError_t _DX_PAL_SemDelete( DX_PAL_SEM *aSemId );

#define _DX_PAL_SemDelete( aSemId ) DX_SUCCESS
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
DxError_t _DX_PAL_SemWait(DX_PAL_SEM aSemId, uint32_t aTimeOut);

#define _DX_PAL_SemWait(aSemId, aTimeOut) DX_SUCCESS
/** 
 * @brief This function purpose is to signal the semaphore.
 *         
 *
 * @param[in] aSemId - Semaphore handle 
 *
 * @return The return values will be according to operating system return values.
 */
DxError_t _DX_PAL_SemGive(DX_PAL_SEM aSemId);

#define _DX_PAL_SemGive(aSemId) DX_SUCCESS



    
#ifdef __cplusplus
}
#endif

#endif


