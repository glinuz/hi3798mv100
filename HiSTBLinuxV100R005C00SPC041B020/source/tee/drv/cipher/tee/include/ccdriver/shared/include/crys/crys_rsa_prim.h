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

 
#ifndef CRYS_RSA_PRIM_H
#define CRYS_RSA_PRIM_H

#include "crys_rsa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 *  Object name     :  CRYS_RSA_PRIM.h
 */
  
/** @file
 * \brief This module defines the API that supports PKCS#1 v2.1 primitive functions
 *
 * \version 
 * \author ronys
 */


/************************ Defines ******************************/

/** 
@brief - A definition describing the low level Engine type (software, hardware, etc.)
*/

#define CRYS_RSA_PRIM_PKI_ENGINE_TYPE  LLF_PKI_ENGINE_TYPE

/**********************************************************************************/ 
/**
@brief
CRYS_RSA_PRIM_Encrypt implements the RSAEP algorithm as defined in PKCS#1 v2.1 6.1.1

  @param[in] UserPubKey_ptr - Pointer to the public key data structure.
  @param[in] PrimeData_ptr - A pointer to a structure containing temp buffers.
  @param[in] Data_ptr - Pointer to the input data to be encrypted.
  @param[in] DataSize - The size, in bytes, of the input data. 
			\Note: DataSize <= modulus size is supported, but recommended
			that size is equal to modulus size. If smaller, the data will be 
			zero-padded on left side up to the modulus size and therefore, 
			after further decrypt operation its result will contain 
			padding zeros also.
  @param[out] Output_ptr - Pointer to the encrypted data. The size of output data 
			is always equal to size modulus size. The output buffer
                        must be at least of modulus size in bytes.

  @return CRYSError_t - CRYS_OK,
                        CRYS_RSA_INVALID_PUB_KEY_STRUCT_POINTER_ERROR,
                        CRYS_RSA_PUB_KEY_VALIDATION_TAG_ERROR,
                        CRYS_RSA_PRIM_DATA_STRUCT_POINTER_INVALID,
                        CRYS_RSA_DATA_POINTER_INVALID_ERROR,
                        CRYS_RSA_INVALID_OUTPUT_POINTER_ERROR,
                        CRYS_RSA_INVALID_MESSAGE_BUFFER_SIZE,
                        CRYS_RSA_INVALID_MESSAGE_DATA_SIZE,
                        CRYS_RSA_INVALID_MESSAGE_VAL
 */
CIMPORT_C CRYSError_t CRYS_RSA_PRIM_Encrypt(CRYS_RSAUserPubKey_t *UserPubKey_ptr,
                                  CRYS_RSAPrimeData_t  *PrimeData_ptr,
                                  uint8_t              *Data_ptr,
                                  uint16_t              DataSize,
                                  uint8_t              *Output_ptr);


/**********************************************************************************/ 
/**
@brief
CRYS_RSA_PRIM_Decrypt implements the RSADP algorithm as defined in PKCS#1 v2.1 6.1.2

  @param[in] PrivKey_ptr - Pointer to the private key data
                           structure. \note The RSA key parameters and hence the algorithm (CRT
                           or not) are determined by this structure. Using CRYS_Build_PrivKey or
                           CRYS_Build_PrivKeyCRT determines which algorithm will be used.
                           
  @param[in] PrimeData_ptr - A pointer to a structure containing internal
        		     buffers required for the RSA operation.
  @param[in] Data_ptr - Pointer to the data to be decrypted. 
  @param[in] DataSize - The size, in bytes, of the input data. To decrypt
			previously encrypted data its size must be equal to
			modulus size.
        		\Note: DataSize <= modulus size is supported, but
        		it is recommended that the size is equal to modulus size. 
			If smaller, the data will be zero-padded on left side 
			up to the modulus size and therefore, after further decrypt 
			operation its result will contain padding zeros also.
  @param[out] Output_ptr - Pointer to the decrypted data. The size of output data 
			is always equal to size modulus size. The output buffer
                        must be at least of modulus size in bytes.

  @return CRYSError_t - CRYS_OK,
                        CRYS_RSA_INVALID_PRIV_KEY_STRUCT_POINTER_ERROR,
                        CRYS_RSA_PRIM_DATA_STRUCT_POINTER_INVALID,
                        CRYS_RSA_PRIV_KEY_VALIDATION_TAG_ERROR,
                        CRYS_RSA_DATA_POINTER_INVALID_ERROR,
                        CRYS_RSA_INVALID_OUTPUT_POINTER_ERROR,
                        CRYS_RSA_INVALID_MESSAGE_DATA_SIZE,
                        CRYS_RSA_INVALID_MESSAGE_VAL
 */
CIMPORT_C CRYSError_t CRYS_RSA_PRIM_Decrypt(CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,
                                  CRYS_RSAPrimeData_t   *PrimeData_ptr,
                                  uint8_t     *Data_ptr,
                                  uint16_t     DataSize,
                                  uint8_t     *Output_ptr);


/**
   \brief CRYS_RSA_PRIM_Sign implements the RSASP1 algorithm as defined in PKCS#1 v2.1 6.2.1

 \def CRYS_RSA_PRIM_Sign The signature primitive is identical to the decryption algorithm

*/
#define CRYS_RSA_PRIM_Sign CRYS_RSA_PRIM_Decrypt

/**
   \brief CRYS_RSA_PRIM_Verify implements the RSAVP1 algorithm as defined in PKCS#1 v2.1 6.2.2

 \def CRYS_RSA_PRIM_Verify The verification primitive is identical to the encryption algorithm

*/
#define CRYS_RSA_PRIM_Verify CRYS_RSA_PRIM_Encrypt

#ifdef __cplusplus
}
#endif

#endif
