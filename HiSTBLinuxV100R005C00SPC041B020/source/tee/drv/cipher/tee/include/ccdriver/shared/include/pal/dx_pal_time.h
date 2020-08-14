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



#ifndef _DX_PAL_TIME_H
#define _DX_PAL_TIME_H


#ifdef __cplusplus
extern "C"
{
#endif

/** 
* @brief File Description:
*        This file conatins functions for time handling. The functions implementations
*        are generally just wrappers to different operating system calls.
*/


/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/

/** 
 * @brief This function returns the number of seconds passed from 1/1/1970 00:00 UTC.
 *         
 *
 * @param[in] void
 *
 * @return Number of seconds passed from 1/1/1970 00:00 UTC
 */
int32_t DX_PAL_GetTime( void ); 
		                      

    
#ifdef __cplusplus
}
#endif

#endif


