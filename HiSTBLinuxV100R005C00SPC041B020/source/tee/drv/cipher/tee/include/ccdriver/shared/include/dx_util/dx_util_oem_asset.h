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

#ifndef  _DX_UTIL_OEM_ASSET_H
#define  _DX_UTIL_OEM_ASSET_H



#include "dx_util.h"
/* Defines the OEM key buffer */
typedef DX_UTIL_AES_CmacResult_t DX_UTIL_OemKey_t;


/*!
 * @brief The function unpacks the asset packet and return the asster data required by OEM for TEE 
 *
 * @param[in] pOemKey   	- OEM key computed during boot using the GetProvisioningKey() ROM function
 * @param[in] assetId   	- an asset identifier
 * @param[in] pAssetPackage   	- a asset package byte-array formatted to unpack
 * @param[in] assetPackageLen   - a asset package length in bytes, must be multiple of 16 bytes 
 * @param[out] pAssetData   	- the decrypted contents of asset data
 * @param[in/out] pAssetDataLen - as input: the size of the allocated asset data buffer (max size is 512 bytes)
 * 				- as output: the actual size of the decrypted asset data buffer (max size is 512 bytes)
 * @param[out] pUserData   	- may be NULL, otherwise the output will hold the user Data within the asset Package in BE format
 *
 * @return DX_UTIL_OK on success, otherwise failure 
 */ 
DxUTILError_t DX_UTIL_OemAssetUnpack(DX_UTIL_OemKey_t      	pOemKey, 
				     uint32_t 	  	assetId, 
				     uint8_t     		*pAssetPackage, 
				     uint32_t  		assetPackageLen,
				     uint8_t     		*pAssetData, 
				     uint32_t  		*pAssetDataLen,
				     uint32_t     		*pUserData);


#endif /*_DX_UTIL_ASSET_H*/
