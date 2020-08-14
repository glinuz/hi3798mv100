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

#ifndef  _DX_UTIL_RPMB_H
#define  _DX_UTIL_RPMB_H

#include "dx_util_error.h"

/******************************************************************************
*                        	DEFINITIONS
******************************************************************************/

/*******************************************/
/*   RPMB shared secret key definitions    */
/*******************************************/

#define DX_UTIL_RPMB_DATA_FRAME_SIZE_IN_BYTES	 284
#define DX_UTIL_MIN_RPMB_DATA_BUFFERS			 1
#define DX_UTIL_MAX_RPMB_DATA_BUFFERS			 65535
#define DX_UTIL_HMAC_SHA256_DIGEST_SIZE_IN_WORDS 8

typedef uint8_t 	DxUtilRpmbKey_t[DX_UTIL_AES_CMAC_RESULT_SIZE_IN_BYTES*2];
typedef uint8_t 	DxUtilRpmbDataBuffer_t[DX_UTIL_RPMB_DATA_FRAME_SIZE_IN_BYTES];
typedef uint32_t 	DxUtilHmacResult_t[DX_UTIL_HMAC_SHA256_DIGEST_SIZE_IN_WORDS];


/**
 * @brief This function computes and outputs the device RPMB Key based
 *    on fixed data & KDR.
 * 
 * @param[out] pCmacResult 	- a pointer to output buffer 32 bytes array
 *
 * @return DX_UTIL_OK on success, otherwise failure 
 */ 
DxUTILError_t DX_UTIL_DeriveRPMBKey(DxUtilRpmbKey_t pRpmbKey);


/**
 * @brief This function calculates an HMAC-SHA256 authentication code of the
 *    callers' data buffers using RPMB key.
 *    The function receives a list of data frames, each 284 bytes long, as described in [JESD84].
 * 
 * @param[in] pListOfDataFrames - a pointer to the caller's list, consist of data frames addresses.
 * @param[in] listSize 			- number of data frames (each 284 bytes).
 * 						  		  valid from 1 single data buffer, up to 65,535 buffers.
 * @param[out] pHmacResult 		- a pointer to HMAC result buffer (8words for HMAC-SHA256).
 *
 * @return DX_UTIL_OK on success, otherwise failure 
 */ 
DxUTILError_t DX_UTIL_SignRPMBFrames(unsigned long *pListOfDataFrames,
									uint32_t       		    listSize,
									DxUtilHmacResult_t 		pHmacResult);


#endif /*_DX_UTIL_H*/
