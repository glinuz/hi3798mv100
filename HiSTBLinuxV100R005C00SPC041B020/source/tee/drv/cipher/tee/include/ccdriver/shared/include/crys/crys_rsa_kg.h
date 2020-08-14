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

 
 
#ifndef CRYS_RSA_KG_H
#define CRYS_RSA_KG_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */

#include "crys_rsa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /*
   *  Object %name    : %
   *  State           :  %state%
   *  Creation date   :  Sun Mar 06 15:55:45 2005
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This module defines the API for key-pair generation functions
   *
   *  \version CRYS_RSA_KG.h#1:incl:1
   *  \author adams
   */


/************************ Defines ******************************/

/* Max allowed size and values of public exponent for key generation in CRYS */
#define CRYS_RSA_KG_PUB_EXP_MAX_SIZE_BITS    17
#define CRYS_RSA_KG_PUB_EXP_ALLOW_VAL_1      0x000003
#define CRYS_RSA_KG_PUB_EXP_ALLOW_VAL_2      0x000011
#define CRYS_RSA_KG_PUB_EXP_ALLOW_VAL_3      0x010001




/***********************************************************************************************/

/**
   @brief CRYS_RSA_KG_GenerateKeyPair generates a Pair of public and private keys on non CRT mode.
 
   @param[in] pubExp_ptr - The pointer to the public exponent (public key)
   @param[in] pubExpSizeInBytes - The public exponent size in bytes.
   @param[in] keySize  - The size of the key, in bits. Supported sizes are:
                            - for PKI without PKA HW: all 256 bit multiples between 512 - 2048;
                            - for PKI with PKA: HW all 32 bit multiples between 512 - 2112;
   @param[out] userPrivKey_ptr - A pointer to the private key structure. 
                           This structure is used as input to the CRYS_RSA_PRIM_Decrypt API.
   @param[out] userPubKey_ptr - A pointer to the public key structure. 
                           This structure is used as input to the CRYS_RSA_PRIM_Encrypt API.
   @param[in] keyGenData_ptr - a pointer to a structure required for the KeyGen operation.                       

   @return CRYSError_t - CRYS_OK,
                         CRYS_RSA_INVALID_EXPONENT_POINTER_ERROR,
                         CRYS_RSA_INVALID_PRIV_KEY_STRUCT_POINTER_ERROR,
                         CRYS_RSA_INVALID_PUB_KEY_STRUCT_POINTER_ERROR,
                         CRYS_RSA_KEY_GEN_DATA_STRUCT_POINTER_INVALID,
                         CRYS_RSA_INVALID_MODULUS_SIZE,
                         CRYS_RSA_INVALID_EXPONENT_SIZE
*/
CIMPORT_C CRYSError_t CRYS_RSA_KG_GenerateKeyPair(
	                                    uint8_t             *pubExp_ptr,
                                        uint16_t             pubExpSizeInBytes, 
                                        uint32_t             keySize,
                                        CRYS_RSAUserPrivKey_t *userPrivKey_ptr,
                                        CRYS_RSAUserPubKey_t  *userPubKey_ptr,
                                        CRYS_RSAKGData_t      *keyGenData_ptr );


/***********************************************************************************************/
/**
   @brief CRYS_RSA_KG_GenerateKeyPairCRT generates a Pair of public and private keys on CRT mode.

   @param[in] pubExp_ptr - The pointer to the public exponent (public key)
   @param[in] pubExpSizeInBytes - The public exponent size in bits. 
   @param[in] keySize  - The size of the key, in bits. Supported sizes are:
                            - for PKI without PKA HW: all 256 bit multiples between 512 - 2048;
                            - for PKI with PKA: HW all 32 bit multiples between 512 - 2112;
   @param[out] userPrivKey_ptr - A pointer to the private key structure. 
                           This structure is used as input to the CRYS_RSA_PRIM_Decrypt API.
   @param[out] userPubKey_ptr - A pointer to the public key structure. 
                           This structure is used as input to the CRYS_RSA_PRIM_Encryped API.
   @param[in] keyGenData_ptr - a pointer to a structure required for the KeyGen operation.                       

   @return CRYSError_t - CRYS_OK,
                         CRYS_RSA_INVALID_EXPONENT_POINTER_ERROR,
                         CRYS_RSA_INVALID_PRIV_KEY_STRUCT_POINTER_ERROR,
                         CRYS_RSA_INVALID_PUB_KEY_STRUCT_POINTER_ERROR,
                         CRYS_RSA_KEY_GEN_DATA_STRUCT_POINTER_INVALID,
                         CRYS_RSA_INVALID_MODULUS_SIZE,
                         CRYS_RSA_INVALID_EXPONENT_SIZE
*/

CIMPORT_C CRYSError_t CRYS_RSA_KG_GenerateKeyPairCRT(
                                           uint8_t             *pubExp_ptr,
                                           uint16_t             pubExpSizeInBytes, 
                                           uint32_t             keySize,
                                           CRYS_RSAUserPrivKey_t *userPrivKey_ptr,
                                           CRYS_RSAUserPubKey_t  *userPubKey_ptr,
                                           CRYS_RSAKGData_t      *keyGenData_ptr );
                                           
                                           
                                           
                                           


#ifdef __cplusplus
}
#endif

#endif


