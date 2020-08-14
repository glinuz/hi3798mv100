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

#ifndef  _DX_UTIL_STIMER_H
#define  _DX_UTIL_STIMER_H

#include "dx_util_error.h"
#include "dx_pal_types_plat.h"


#define NSEC_SEC 1000000000
#define CONVERT_CLK_TO_NSEC(clks,hz) ((NSEC_SEC/hz)*(clks))

/******************************************************************************
*                        	DEFINITIONS
******************************************************************************/


typedef struct {
	uint32_t lower_bit_reg;
	uint32_t upper_bit_reg;
}DX_UTIL_Cntr_t;

typedef struct {
	uint64_t lr_cntr_value_back;
	uint64_t hr_cntr_value_back;
	uint64_t lr_cntr_value_forward;
	uint64_t hr_cntr_value_forward;
}DX_UTIL_TimeStamp_t;

/*!  
 * This function is used to read secure timer low resolution registers 
 * and secure timer high reolution registers 
 * 
 * @param[in/out] stimer 	- secure timer structure
 *  
 *  
 */
void DX_UTIL_GetTimeStamp(DX_UTIL_TimeStamp_t *time_stamp);

/*!  
 * This function computes duration between to time stamps 
 * 
 * @param[in] time_stamp1 	- first secure timer structure 
 *  
 *  @param[in] time_stamp2 	- second secure timer structure 
 *   
 * @return  - duration between two time stamps of secure timer in nsec
 *  
 *  
 */
int64_t DX_UTIL_CmpTimeStamp(DX_UTIL_TimeStamp_t *time_stamp1,
			      DX_UTIL_TimeStamp_t *time_stamp2);

#endif /*_DX_UTIL_STIMER_H*/
