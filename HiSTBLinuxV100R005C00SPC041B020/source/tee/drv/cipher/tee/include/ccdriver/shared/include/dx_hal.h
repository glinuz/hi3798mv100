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


#ifndef __DX_HAL_H__
#define __DX_HAL_H__

//#include <stdint.h>
#include "dx_hal_plat.h"
#define INT32_MAX 2147483647

typedef enum {
	DX_HAL_OK = 0,
	DX_HAL_ENODEV,        /* Device not opened or does not exist */
	DX_HAL_EINTERNAL,     /* Internal driver error (check system log) */
	DX_HAL_MAPFAILED,
	DX_HAL_ENOTSUP,       /* Unsupported function/option */
	DX_HAL_ENOPERM,       /* Not enough permissions for request */
	DX_HAL_EINVAL,        /* Invalid parameters */
	DX_HAL_ENORSC,        /* No resources available (e.g., memory) */
	DX_HAL_RESERVE32B = 0x7FFFFFFFL
} DxHal_RetCode_t;

/*!
 * HAL layer entry point. 
 * Mappes CryptoCell 4.X regisers to the HOST virtual address space.
 */
int DX_HAL_Init(void);

/*!
 * Busy wait upon Interrupt Request Register (IRR) signals. 
 * This function notifys for any CC interrupt, it is the caller responsiblity
 * to verify and prompt the expected case interupt source.
 * 
 * @param[in] data 	- input data for future use 
 * \return uint32_t The IRR value.
 */
uint32_t DX_HAL_WaitInterrupt(uint32_t data);

/*!
 * Set HW cache parameters 
 * This function need to be changed according to customer's platform
 * 
 * \param void
 *
 * \return void
 */
void DX_HAL_InitHWCacheParams(void);


/*!
 * HAL exit point. 
 * Unmaps CryptoCell 4.X registers. 
 */
int DX_HAL_Terminate(void);


#endif

