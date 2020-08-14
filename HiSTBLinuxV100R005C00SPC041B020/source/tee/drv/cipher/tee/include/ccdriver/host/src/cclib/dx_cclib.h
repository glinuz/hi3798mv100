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


#ifndef __DX_CCLIB_H__
#define __DX_CCLIB_H__

#include "dx_pal_types.h"

typedef enum {
	DX_CCLIB_RET_OK = 0,
	DX_CCLIB_RET_ENODEV,        /* Device not opened or does not exist */
	DX_CCLIB_RET_EINTERNAL,     /* Internal driver error (check system log) */
	DX_CCLIB_RET_ENOTSUP,       /* Unsupported function/option */
	DX_CCLIB_RET_ENOPERM,       /* Not enough permissions for request */
	DX_CCLIB_RET_EINVAL,        /* Invalid parameters */
	DX_CCLIB_RET_HW_Q_INIT,
	DX_CCLIB_RET_COMPLETION,
	DX_CCLIB_RET_HAL,
	DX_CCLIB_RET_ASYM_ERR,
	DX_CCLIB_RET_RND_INST_ERR,
	DX_CCLIB_RET_EINVAL_HW_VERSION,    /* Invalid HW version */
	DX_CCLIB_RET_EINVAL_HW_SIGNATURE,  /* Invalid HW signature */
	DX_CCLIB_RET_PAL,
	DX_CCLIB_RESERVE32B = 0x7FFFFFFFL
} DX_CclibRetCode_t;

#define DXDI_DEV_PRIO0 "/dev/dx_sep_q0"
#define DXDI_DEV_PRIO1 "/dev/dx_sep_q1"

#define DX_VERSION_PRODUCT_BIT_SHIFT 	0x18UL
#define DX_VERSION_PRODUCT_BIT_SIZE 	0x8UL

typedef uint32_t DX_CclibDevPriority;

/*!
 * Init OS driver context
 * This function must be invoked before any other CC library API function
 * but for the functions which deal with library initialization properties:
 * DX_CclibSetPriority, DX_CclibGetPriority, DX_CclibGetMaxPriority
 * 
 * \return DxCclib_RetCode
 */
DX_CclibRetCode_t DX_CclibInit(void);

/*!
 * Finalize OS driver context - release associated resources
 */
void DX_CclibFini(void);

/*!
 * Set requested CryptoCell priority queue 
 * This function must be invoked before DX_CclibInit
 * 
 * \param priority Requested priority queue
 *
 * \return DxCclib_RetCode
 */
DX_CclibRetCode_t DX_CclibSetPriority(DX_CclibDevPriority priority);

/*!
 * Get selected priority 
 * (as set by DX_CclibSetPriority or default if it was not invoked)
 *
 * 
 * \return DX_CclibDevPriority
 */
DX_CclibDevPriority DX_CclibGetPriority(void);

/*!
 * Get the highest priority supported 
 *
 * \return DX_CclibDevPriority
 */
DX_CclibDevPriority DX_CclibGetMaxPriority(void);



#endif /*__DX_CCLIB_H__*/

