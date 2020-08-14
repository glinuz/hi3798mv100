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

#ifndef  _DX_UTIL_H
#define  _DX_UTIL_H

#include "dx_util_error.h"
#include "dx_pal_types_plat.h"

/******************************************************************************
*                        	DEFINITIONS
******************************************************************************/

/*****************************************/
/* CMAC derive key definitions*/
/*****************************************/
#define DX_UTIL_CMAC_DERV_MIN_DATA_IN_SIZE	0x8UL    
#define DX_UTIL_CMAC_DERV_MAX_DATA_IN_SIZE	0x400UL   
#define DX_UTIL_AES_CMAC_RESULT_SIZE_IN_BYTES	0x10UL    
#define DX_UTIL_AES_CMAC_RESULT_SIZE_IN_WORDS	(DX_UTIL_AES_CMAC_RESULT_SIZE_IN_BYTES/4)    

typedef uint32_t DxUTILError_t;
/* Defines the CMAC result buffer  - 16 bytes array */
typedef uint8_t DX_UTIL_AES_CmacResult_t[DX_UTIL_AES_CMAC_RESULT_SIZE_IN_BYTES];

typedef enum  {
	DX_UTIL_SESSION_KEY = 0,
	DX_UTIL_KDR_KEY = 1,
	DX_UTIL_END_OF_KEYS = 0x7FFFFFFF
}DX_UTIL_KeyType_t;


/*!  
 * This function is used to generate bytes stream for key derivation purposes. 
 * The function gets an input data and can use use one of the following keys: KDR, Session. 
 * 
 * @param[in] aesKeyType 	- 1 (KDR); 0 (Session)
 * @param[in] pDataIn 		- a pointer to input buffer 
 * @param[in] dataInSize 	- size of data in bytes(min = DX_UTIL_CMAC_DERV_MIN_DATA_IN_SIZE, 
 * 							max = DX_UTIL_CMAC_DERV_MAX_DATA_IN_SIZE)
 * @param[out] pCmacResult 	- a pointer to output buffer 16 bytes array
 *  
 * @return DX_UTIL_OK on success, otherwise failure 
 *  
 */
DxUTILError_t DX_UTIL_CmacDeriveKey(DX_UTIL_KeyType_t		aesKeyType, 
			            uint8_t  			*pDataIn,
			            uint32_t  		dataInSize,
			            DX_UTIL_AES_CmacResult_t  	pCmacResult);

/*****************************************/
/* Endorsement key derivation definitions*/
/*****************************************/
typedef enum {
	DX_UTIL_EK_DomainID_secp256k1 = 1,
	DX_UTIL_EK_DomainID_secp256r1 = 2,
	DX_UTIL_EK_DomainID_Max,
	DX_UTIL_EK_DomainID_Last      = 0x7FFFFFFF,
}DX_UTIL_EK_DomainID_t;


#define DX_UTIL_EK_MODUL_MAX_LENGTH 32  // 256 bit modulus size

typedef  uint8_t  DX_UTIL_EK_Point_t[DX_UTIL_EK_MODUL_MAX_LENGTH];

typedef DX_UTIL_EK_Point_t  DX_UTIL_EK_Privkey_t;

typedef  struct  
{
    DX_UTIL_EK_Point_t		PublKeyX; 
    DX_UTIL_EK_Point_t		PublKeyY;
} DX_UTIL_EK_Pubkey_t;


/*!  
 * Derive an ECC256 key-pair from the device root key (KDR) 
 * 
 * @param[in] domainID 		- 1 (DX_UTIL_EK_DomainID_secp256k1); 2 (DX_UTIL_EK_DomainID_secp256r1)
 * @param[out] pPrivKey_ptr 	- a pointer to derived ECC256 private key, 
 * @param[out] pPublKey_ptr 	- a pointer to derived ECC256 public key
 *  
 * @return DX_UTIL_OK on success, otherwise failure 
 *  
 */
DxUTILError_t DX_UTIL_DeriveEndorsementKey(DX_UTIL_EK_DomainID_t  domainID,	   
					   DX_UTIL_EK_Privkey_t   *pPrivKey_ptr,  
					   DX_UTIL_EK_Pubkey_t    *pPublKey_ptr);    



/*****************************************/
/*   SESSION key settings definitions    */
/*****************************************/

/*!
 * @brief This function derives the session key based on random data & KDR 
 * 	  The output is written to the session key registers. 
 *
 * @return DX_UTIL_OK on success, otherwise failure 
 */ 
DxUTILError_t DX_UTIL_SetSessionKey(void);



#endif /*_DX_UTIL_H*/
