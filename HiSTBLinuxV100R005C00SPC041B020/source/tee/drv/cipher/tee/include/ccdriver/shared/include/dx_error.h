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

#ifndef __DX_ERROR_H__
#define __DX_ERROR_H__

#ifdef __KERNEL__
//#include <linux/types.h>
#define INT32_MAX 0x7FFFFFFFL
#else
//#include <stdint.h>
#endif
#define INT32_MAX 2147483647


typedef enum DxSymRetCode {
	DX_RET_OK = 0, /* No error */
	DX_RET_UNSUPP_ALG, /* Unsupported algorithm */
	DX_RET_UNSUPP_ALG_MODE, /* Unsupported algorithm mode */
	DX_RET_UNSUPP_OPERATION, /* Unsupported operation */
	DX_RET_INVARG, /* Invalid parameter */
	DX_RET_INVARG_QID, /* Invalid queue ID */
	DX_RET_INVARG_KEY_SIZE, /* Invalid key size */
	DX_RET_INVARG_CTX_IDX, /* Invalid context index */
	DX_RET_INVARG_CTX, /* Bad or corrupted context */
	DX_RET_INVARG_BAD_ADDR, /* Bad address */
	DX_RET_INVARG_INCONSIST_DMA_TYPE, /* DIN is inconsist with DOUT DMA type */
	DX_RET_PERM, /* Operation not permitted */
	DX_RET_NOEXEC, /* Execution format error */
	DX_RET_BUSY, /* Resource busy */
	DX_RET_NOMEM, /* Out of memory */
	DX_RET_OSFAULT, /* Internal OS error */
	SEPSYMCRYPTO_RET_RESERVE32 = INT32_MAX /* assure this enum is 32b */
}DxSymRetCode_t;


#endif /*__DX_ERROR_H__*/
