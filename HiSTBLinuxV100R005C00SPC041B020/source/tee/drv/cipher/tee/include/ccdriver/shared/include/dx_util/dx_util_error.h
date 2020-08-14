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

#ifndef  _DX_UTIL_ERROR_H
#define  _DX_UTIL_ERROR_H


/***********************/
/* Util return codes   */
/***********************/

#define DX_UTIL_OK		0x00UL

#define DX_UTIL_MODULE_ERROR_BASE 0x80000000

/* The CMAC derive module errors base address - 0x80000000 */
#define DX_UTIL_INVALID_KEY_TYPE             		(DX_UTIL_MODULE_ERROR_BASE + 0x00UL)
#define DX_UTIL_DATA_IN_POINTER_INVALID_ERROR		(DX_UTIL_MODULE_ERROR_BASE + 0x01UL)
#define DX_UTIL_DATA_IN_SIZE_INVALID_ERROR		(DX_UTIL_MODULE_ERROR_BASE + 0x02UL)
#define DX_UTIL_DATA_OUT_POINTER_INVALID_ERROR		(DX_UTIL_MODULE_ERROR_BASE + 0x03UL)
#define DX_UTIL_DATA_OUT_SIZE_INVALID_ERROR		(DX_UTIL_MODULE_ERROR_BASE + 0x04UL)
#define DX_UTIL_FATAL_ERROR				(DX_UTIL_MODULE_ERROR_BASE + 0x05UL)
#define DX_UTIL_ILLEGAL_PARAMS_ERROR			(DX_UTIL_MODULE_ERROR_BASE + 0x06UL)
#define DX_UTIL_BAD_ADDR_ERROR				(DX_UTIL_MODULE_ERROR_BASE + 0x07UL)
#define DX_UTIL_EK_DOMAIN_INVALID_ERROR			(DX_UTIL_MODULE_ERROR_BASE + 0x08UL)
#define DX_UTIL_KDR_INVALID_ERROR             		(DX_UTIL_MODULE_ERROR_BASE + 0x09UL)
#define DX_UTIL_LCS_INVALID_ERROR             		(DX_UTIL_MODULE_ERROR_BASE + 0x0AUL)

#endif /*_DX_UTIL_ERROR_H*/
