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

 
 #ifndef CRYS_RSA_SCHEMES_H
#define CRYS_RSA_SCHEMES_H


#include "crys_error.h"
#include "crys_rsa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 *  Object name     :  CRYS_RSA_SCHEMES.h
 */
  
/** @file
 * \brief This module defines the API that supports PKCS#1 v1.5 and PKCS#1 v2.1 schemes
 *
 * \version 
 * \author ronys ohads
 */
 
/**********************************************************************************************************/
/**
   @brief
   RSA_SCHEMES_Encrypt implements the RSAES-OAEP algorithm as defined
   in PKCS#1 v2.1 8.1 and in PKCS#1 v1.5 8.1

	This function combines the RSA encryption primitive and the
	EME-OAEP encoding method, to provide an RSA-based encryption
	method that is semantically secure against adaptive
	chosen-ciphertext attacks. For more details, please refere to
	the PKCS#1 standard.

	The actual macro that will be used by the user is:
	CRYS_RSA_OAEP_Encrypt 	  - for v2.1
	CRYS_RSA_PKCS1v15_Encrypt - for v1.5

   @param[in] UserPubKey_ptr - A pointer to the public key data structure of the User.
   @param[in] PrimeData_ptr - A pointer to a CRYS_RSAPrimeData_t 
   				that is used for the Encryption operation
   @param[in] hashFunc - The hash function to be used. 
                         The hash functions supported: SHA1, SHA-256/284/512, 
                         MD5 (MD5 - allowed only for PKCS#1 v1.5).
   @param[in] L - The label input pointer. Relevant for PKCS#1 Ver2.1 only, may be NULL also.
                  For PKCS#1 Ver1.5 it is an empty string (NULL). 
   @param[in] Llen - The label length. Relevant for PKCS#1 Ver2.1 only (see notes above).                 
   @param[in] MGF - the mask generation function. PKCS#1 v2.1
                    defines MGF1, so the currently allowed value is CRYS_PKCS1_MGF1.
   @param[in] Data_ptr - Pointer to the data to encrypt.
   @param[in] DataSize - The size, in bytes, of the data to encrypt. 
			 \Note: The data size must be:
			    1. for PKCS #1 v.2.1  DataSize <= PrivKey_ptr->N.len - 2*HashLen - 2.
			    2. for PKCS #1 v.1.5  DataSize <= PrivKey_ptr->N.len - 11.
   @param[out] Output_ptr - Pointer to the encrypted data. The size of the data is always 
			    equal to the RSA key (modulus) size, in bytes. Therefore the size 
		            of allocated buffer must be at least of this size.

   @return CRYSError_t - CRYS_OK, CRYS_BAD_PARAM, CRYS_OUT_OF_RANGE
*/
CIMPORT_C CRYSError_t _DX_RSA_SCHEMES_Encrypt(
			CRYS_RSAUserPubKey_t  *UserPubKey_ptr,
			CRYS_RSAPrimeData_t   *PrimeData_ptr,
			CRYS_RSA_HASH_OpMode_t hashFunc,
			uint8_t  *L,
			uint16_t  Llen,
			CRYS_PKCS1_MGF_t MGF,
			uint8_t   *DataIn_ptr,
			uint16_t   DataInSize,
			uint8_t   *Output_ptr,
			CRYS_PKCS1_version  PKCS1_ver);

/**
   @brief
   CRYS_RSA_OAEP_PSS21_Encrypt implements the RSAES-OAEP algorithm 
   as defined in PKCS#1 v2.1 8.1
   
   \note It is not recommended to use hash MD5 in OAEP PKCS1 ver 2.1, therfore 
   	it is not supported and the hash function argument has been eliminated
   
   This function combines the RSA encryption primitive and the
   EME-OAEP encoding method, to provide an RSA-based encryption
   method that is semantically secure against adaptive
   chosen-ciphertext attacks. For additional details, please refer to
   the PKCS#1 standard.
   \note The \em L input is defined by the standard as the empty
   string, therefore the caller need not provide it.
*/
#define CRYS_RSA_OAEP_Encrypt(UserPubKey_ptr,PrimeData_ptr,HashMode,L,Llen,MGF,Data_ptr,DataSize,Output_ptr)\
		_DX_RSA_SCHEMES_Encrypt(UserPubKey_ptr,PrimeData_ptr,HashMode/*CRYS_RSA_HASH_SHA1_mode*/,L,Llen,MGF,Data_ptr,DataSize,Output_ptr,CRYS_PKCS1_VER21)

/**
   @brief
   CRYS_RSA_PKCS1v15_Encrypt implements the RSAES-PKCS1v15 algorithm 
   as defined in PKCS#1 v2.1 8.2
*/
#define CRYS_RSA_PKCS1v15_Encrypt(UserPubKey_ptr,PrimeData_ptr,DataIn_ptr,DataInSize,Output_ptr)\
		_DX_RSA_SCHEMES_Encrypt(UserPubKey_ptr,PrimeData_ptr,CRYS_RSA_HASH_NO_HASH_mode,DX_NULL,0,CRYS_PKCS1_NO_MGF,DataIn_ptr,DataInSize,Output_ptr,CRYS_PKCS1_VER15)
		

/**********************************************************************************************************/
/**
   @brief
   RSA_SCHEMES_Decrypt implements the RSAES-OAEP algorithm as defined
   in PKCS#1 v2.1 8.1 and in PKCS#1 v1.5

	   This function combines the RSA decryption primitive and the
	   EME-OAEP decoding method, to provide an RSA-based decryption
	   method that is semantically secure against adaptive
	   chosen-ciphertext attacks. For more details, please refer to
	   the PKCS#1 standard.

   @param[in] UserPrivKey_ptr - Pointer to the private key data structure. 
                   \Note: The representation (pair or quintuple) 
                    and hence the algorithm (CRT or not) is determined
                    by the Private Key data structure. Using CRYS_Build_PrivKey
                    or CRYS_Build_PrivKeyCRT determines which algorithm will be used.
                            
   @param[in] PrimeData_ptr - Pointer to a CRYS_RSAPrimeData_t which is used for the 
   							  Encryption operation
   							                            
   @param[in] hashFunc - The hash function to be used. 
                         The hash functions supported: SHA1, SHA-256/284/512, 
                         MD5 (MD5 - allowed only for PKCS#1 v1.5).
                         
   @param[in] L - The label input pointer. Relevant for PKCS#1 Ver2.1 only, may be NULL also.
                  For PKCS#1 Ver1.5 it is an empty string (NULL). 
   @param[in] Llen - The label length. Relevant for PKCS#1 Ver2.1 only (see notes above).                 
   @param[in] MGF - The mask generation function. PKCS#1 v2.1 defines MGF1,
                    so the only value allowed here is CRYS_PKCS1_MGF1.
   @param[in] Data_ptr - Pointer to the data to decrypt.
   @param[in] DataSize - The size, in bytes, of the data to decrypt. 
                        \Note: The size must be = the size of the modulus.

   @param[out] Output_ptr - Pointer to the decrypted data, the size of the buffer in bytes 
		must be not less than the actual size of Encrypted message, if it is known,
		else the output buffer size must be :
		1. for PKCS #1 v.2.1  *OutputSize_ptr >= PrivKey_ptr->N.len - 2*HashLen - 2.
		2. for PKCS #1 v.1.5  *OutputSize_ptr >= PrivKey_ptr->N.len - 11.
   @param[in/out] OutputSize_ptr - The size of the user passed Output_ptr buffer in bytes [in] and 
		actual size of decrypted message [out].
		The minimal input size value of *OutputSize_ptr is described above. 
		This value is updated with the actual number of bytes that 
		are loaded to Output_ptr buffer byDecrypt function.

   @return CRYSError_t - CRYS_OK or appropriate Error message defined in the RSA module.
*/
CIMPORT_C CRYSError_t _DX_RSA_SCHEMES_Decrypt(
	                            CRYS_RSAUserPrivKey_t  *UserPrivKey_ptr,
                                    CRYS_RSAPrimeData_t    *PrimeData_ptr,
                                    CRYS_RSA_HASH_OpMode_t  hashFunc,
                                    uint8_t              *L,
                                    uint16_t              Llen,
                                    CRYS_PKCS1_MGF_t        MGF,
                                    uint8_t              *DataIn_ptr,
                                    uint16_t              DataInSize,
                                    uint8_t              *Output_ptr,
                                    uint16_t             *OutputSize_ptr,
                                    CRYS_PKCS1_version      PKCS1_ver);

/**********************************************************************************************************/
/**
   @brief
   CRYS_RSA_OAEP_Decrypt implements the RSAES-OAEP algorithm 
   as defined in PKCS#1 v2.1 8.1
   
   Note: It is not recommended to use hash MD5 in OAEP PKCS1 ver 2.1, therefore 
   		it is not supported and the hash function argument is eliminated
   		
   This function combines the RSA decryption primitive and the
   EME-OAEP encoding method, to provide an RSA-based decryption
   method that is semantically secure against adaptive
   chosen-ciphertext attacks. For more details, please refer to
   the PKCS#1 standard.
   \note The \em L input is defined by the standard as the empty
   string, therefore the caller need not provide it.   
   
*/
#define CRYS_RSA_OAEP_Decrypt(UserPrivKey_ptr,PrimeData_ptr,HashMode,L,Llen,MGF,Data_ptr,DataSize,Output_ptr,OutputSize_ptr)\
		_DX_RSA_SCHEMES_Decrypt(UserPrivKey_ptr,PrimeData_ptr,HashMode/*CRYS_RSA_HASH_SHA1_mode*/,L,Llen,MGF,Data_ptr,DataSize,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)
	
		
/**
   @brief
   CRYS_RSA_PKCS1v15_Decrypt implements the RSAES-PKCS1v15 algorithm as defined
   in PKCS#1 v2.1 8.2
*/
#define CRYS_RSA_PKCS1v15_Decrypt(UserPrivKey_ptr,PrimeData_ptr,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr)\
		_DX_RSA_SCHEMES_Decrypt(UserPrivKey_ptr,PrimeData_ptr,CRYS_RSA_HASH_NO_HASH_mode,DX_NULL,0,CRYS_PKCS1_NO_MGF,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER15)
		
					      
/**********************************************************************************************************/
/**
   \brief RSA_SignInit initializes the Signing multi-call algorithm
          as defined in PKCS#1 2.1 standard, including v1.5.

	The actual macro that will be used by the user is:
	CRYS_RSA_PSS_SignInit		 - for v2.1
	CRYS_RSA_PKCS1v15_SignInit	 - for v1.5

	NOTE: 1. In PSS_Sign v2.1 MD5 is not supported, since it is not recommended 
		 by the PKCS#1 v2.1.	
	      2. According to thesaid standard, implementation of the function 
		 for version v1.5 is based on DER encoding of the algorithm info. 
 
   This function does not do cryptographic processing. Rather, it
   prepares a context that is used by the Update and Finish functions.
   
   @param[in,out] UserContext_ptr - A pointer to a Context. The value
                                returned here must be passed to
                                the Update and Finish functions.
   @param[in] UserPrivKey_ptr - A pointer to the private key data
                            structure. \note The representation
                            (pair or quintuple) and hence the
                            algorithm (CRT or not) is determined
                            by the Private Key data structure.
                            Using of the CRYS_Build_PrivKey
                            or CRYS_Build_PrivKeyCRT determines
                            which algorithm will be used.
   @param[in] rsaHashMode - The enumerator value, defining the hash function to be used:
			 SHA-1SHA224/256/384/512, MD5 (MD5 allowed only in v1.5).
			 The hash functions recommended by PKCS#1 v2.1 are: 
                         256/384/512. Also allowed "After" HASH modes for said functions. 
   @param[in] MGF - The mask generation function. PKCS#1 v2.1
                    defines MGF1, so the only value allowed here is CRYS_PKCS1_MGF1.
   @param[in] SaltLen - The Length of the Salt buffer (relevant for PKCS#1 Ver 2.1 only).
   			Typical lengths are 0 and hLen (20 for SHA1).
   			The maximum length allowed is NSize - hLen - 2.                    
   @param[in] PKCS1_ver - Ver 1.5 or 2.1, according to the functionality required.
                       
   @return CRYSError_t - CRYS_OK, CRYS_BAD_PARAM
*/
CIMPORT_C CRYSError_t _DX_RSA_SignInit(CRYS_RSAPrivUserContext_t *UserContext_ptr,
                             CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,
			     CRYS_RSA_HASH_OpMode_t rsaHashMode,
                             CRYS_PKCS1_MGF_t MGF,
                             uint16_t SaltLen,				  
                             CRYS_PKCS1_version PKCS1_ver);


/**
   \brief CRYS_RSA_PSS_SignInit initializes the RSASSA-PSS
   multi-call version.
*/			
#define CRYS_RSA_PSS_SignInit(UserContext_ptr,UserPrivKey_ptr,hashFunc,MGF,SaltLen)\
			_DX_RSA_SignInit(UserContext_ptr,UserPrivKey_ptr,hashFunc,MGF,SaltLen,CRYS_PKCS1_VER21)				  

/**
   \brief CRYS_RSA_PKCS1v15_SignInit initializes the PKCS#1 v1.5
   multi-call version.
*/
#define CRYS_RSA_PKCS1v15_SignInit(UserContext_ptr,UserPrivKey_ptr,hashFunc)\
			_DX_RSA_SignInit(UserContext_ptr,UserPrivKey_ptr,hashFunc,CRYS_PKCS1_NO_MGF,0,CRYS_PKCS1_VER15)
			

/**********************************************************************************************************/
/**
   \brief CRYS_RSA_PSS_SignUpdate processes the data to be signed
   in a given context.

   \note RSA_PSS_SignUpdate can be called multiple times
   with data 
   
	The actual macro that will be used by the users is (one of):
	CRYS_RSA_PSS_SignUpdate
	CRYS_RSA_PKCS1v15_SignUpdate

   \note CRYS_RSA_PSS_SignUpdate can be called multiple times
   with data 

   @param[in] UserContext_ptr - A pointer to a valid context,
                            as returned by CRYS_RSA_PSS_SignInit.
   @param[in] DataIn_ptr - A pointer to the data to sign.
   @param[in] DataInSize - The size, in bytes, of the data to sign.

   @return CRYSError_t - CRYS_OK, CRYS_BAD_PARAM
*/

CIMPORT_C CRYSError_t _DX_RSA_SignUpdate(CRYS_RSAPrivUserContext_t *UserContext_ptr,
                               uint8_t     *DataIn_ptr,
                               uint32_t     DataInSize);


/**
   \brief CRYS_RSA_PSS_SignUpdate processes the data to be signed
   in a given context, according to PKCS1 v2.1
*/
#define CRYS_RSA_PSS_SignUpdate(UserContext_ptr,Data_ptr,DataSize)\
			_DX_RSA_SignUpdate(UserContext_ptr,Data_ptr,DataSize)


/**
   \brief CRYS_RSA_PKCS1v15_SignUpdate processes the data to be signed
   in a given context, according to PKCS1 v1.5
*/
#define CRYS_RSA_PKCS1v15_SignUpdate(UserContext_ptr,Data_ptr,DataSize)\
			_DX_RSA_SignUpdate(UserContext_ptr,Data_ptr,DataSize)		

			
/**********************************************************************************************************/
/**
   \brief _DX_RSA_SignFinish calculates the signature on the
   data passed to one or more calls to CRYS_RSA_PSS_SignUpdate,
   and releases the context.

	The actual macro that will be used by the user is:
	CRYS_RSA_PSS_SignFinish		- for v2.1
	CRYS_RSA_PKCS1v15_SignFinish	- for v1.5
	
   @param[in,out] UserContext_ptr - A pointer to the Context
                                initialized by the SignInit function 
                                and used by the SignUpdate function
   @param[out] Output_ptr - A pointer to the signature. 
                            The buffer must be at least PrivKey_ptr->N.len bytes long 
                            (that is, the size of the modulus, in bytes).
   @param[in,out] OutputSize_ptr - A pointer to the Signature Size value - the input value
                                   is the signature buffer size allocated, the output value is 
                                   the signature size used.
                                   The buffer must be at least PrivKey_ptr->N.len bytes long 
                                   (that is, the size of the modulus, in bytes).

   @return CRYSError_t - CRYS_OK,
                         CRYS_RSA_INVALID_USER_CONTEXT_POINTER_ERROR,
                         CRYS_RSA_USER_CONTEXT_VALIDATION_TAG_ERROR,
                         CRYS_RSA_INVALID_OUTPUT_POINTER_ERROR,
                         CRYS_RSA_INVALID_SIGNATURE_BUFFER_SIZE,
                         CRYS_RSA_HASH_ILLEGAL_OPERATION_MODE_ERROR,
                         CRYS_RSA_PKCS1_VER_ARG_ERROR
*/

CIMPORT_C CRYSError_t _DX_RSA_SignFinish(CRYS_RSAPrivUserContext_t *UserContext_ptr,
                               uint8_t     *Output_ptr,
                               uint16_t    *OutputSize_ptr);			

/**
   \brief CRYS_RSA_PSS_SignFinish calculates the signature on the
   data passed to one or more calls to CRYS_RSA_PSS_SignUpdate,
   and releases the context.
*/
#define CRYS_RSA_PSS_SignFinish(UserContext_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_SignFinish((UserContext_ptr),(Output_ptr),(OutputSize_ptr))

/**
   \brief CRYS_RSA_PKCS1v15_SignFinish calculates the signature on the
   data passed to one or more calls to CRYS_RSA_PKCS1v15_SignUpdate,
   and releases the context.
*/			
#define CRYS_RSA_PKCS1v15_SignFinish(UserContext_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_SignFinish((UserContext_ptr),(Output_ptr),(OutputSize_ptr))
		
			
/**********************************************************************************************************/
/**
   @brief
   \brief RSA_Sign implements the RSASSA-PKCS1v15 algorithm
    in a single function as defined in PKCS#1 v2.1 standard, including v1.5.
   
	The user can call the function by appropriate macro according to choosen 
	(and allowed) HASH algorithm SHA1, SHA224... (see macros below).

	NOTE: 1. In PSS_Sign v2.1 MD5 is not supported, since it is not recommended 
		 by the PKCS#1 v2.1.	
	      2. According to thesaid standard, implementation of the function 
		 for version v1.5 is based on DER encoding of the algorithm info. 
   
   @param[in] UserContext_ptr - A pointer to a Context. For the use of the 
   								function as a space to work on.
   @param[in] UserPrivKey_ptr - A pointer to the private key data
                            structure of the user. \note The representation
                            (pair or quintuple) and hence the
                            algorithm (CRT or not) is determined
                            by the Private Key data
                            structure - using CRYS_Build_PrivKey
                            or CRYS_Build_PrivKeyCRT determines
                            which algorithm will be used.
   @param[in] hashFunc - The hash functions supported: SHA1, SHA-256/224/264/512, MD5 
                         (MD5 - allowed only for PKCS#1 v1.5).
			 Also allowed "After" HASH modes for said functions. 
   @param[in] MGF - The mask generation function (enum). Only for PKCS#1 v2.1
                    defines MGF1, so the only value allowed for v2.1 
                    is CRYS_PKCS1_MGF1. 
   @param[in] SaltLen - The Length of the Salt buffer (relevant for PKCS#1 Ver 2.1 only)
   			Typical lengths are 0 and hLen (20 for SHA1)
   			The maximum length allowed is NSize - hLen - 2.                                                                                       
   @param[in] DataIn_ptr - A pointer to the data to sign.
   @param[in] DataInSize - The size, in bytes, of the data to sign.
   @param[out] Output_ptr - A pointer to the signature. 
                            The buffer must be at least PrivKey_ptr->N.len bytes long 
                            (that is, the size of the modulus in bytes).
   @param[in,out] OutputSize_ptr - A pointer to the Signature Size value - the input value
                            is the signature buffer size allocated, the output value is 
                            the signature size actually used.
                            The buffer must be at least PrivKey_ptr->N.len bytes long 
                            (that is, the size of the modulus in bytes).
   @param[in] PKCS1_ver - Ver 1.5 or 2.1, according to the functionality required
      
   @return CRYSError_t - CRYS_OK,
                         CRYS_RSA_INVALID_USER_CONTEXT_POINTER_ERROR,
                         CRYS_RSA_INVALID_PRIV_KEY_STRUCT_POINTER_ERROR,
                         CRYS_RSA_PRIV_KEY_VALIDATION_TAG_ERROR,
                         CRYS_RSA_HASH_ILLEGAL_OPERATION_MODE_ERROR,
                         CRYS_RSA_MGF_ILLEGAL_ARG_ERROR,
                         CRYS_RSA_PKCS1_VER_ARG_ERROR,
                         CRYS_RSA_INVALID_MESSAGE_DATA_SIZE,
                         CRYS_RSA_INVALID_OUTPUT_POINTER_ERROR,
                         CRYS_RSA_INVALID_SIGNATURE_BUFFER_SIZE
*/
CIMPORT_C CRYSError_t _DX_RSA_Sign(CRYS_RSAPrivUserContext_t *UserContext_ptr,
				   CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,
				   CRYS_RSA_HASH_OpMode_t rsaHashMode,
				   CRYS_PKCS1_MGF_t MGF,
				   uint16_t     SaltLen,                                                                                          
				   uint8_t     *DataIn_ptr,
				   uint32_t     DataInSize,
				   uint8_t     *Output_ptr,
				   uint16_t    *OutputSize_ptr,
				   CRYS_PKCS1_version PKCS1_ver);


/**
   @brief
   CRYS_RSA_PKCS1v15_Sign implements the RSASSA-PKCS1v15 algorithm 
   as defined in PKCS#1 v1.5

   This function combines the RSASP1 signature primitive and the
   EMSA-PKCS1v15 encoding method, to provide an RSA-based signature scheme. 
   For more details, please refer to the PKCS#1 standard.
  
*/	
	
#define CRYS_RSA_PKCS1v15_Sign(UserContext_ptr,UserPrivKey_ptr,hashFunc,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),(hashFunc),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),(DataInSize),(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)
		

/**
   @brief
   CRYS_RSA_PKCS1v15_SHA1_Sign implements the RSASSA-PKCS1v15 algorithm 
   as defined in PKCS#1 v1.5, but without performing a HASH function - 
   it assumes that the data in has already been hashed using SHA-1
   
   Note: The data_in size is already known after the Hash
      
*/
#define CRYS_RSA_PKCS1v15_SHA1_Sign(UserContext_ptr,UserPrivKey_ptr,DataIn_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),(CRYS_RSA_After_SHA1_mode),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),CRYS_HASH_SHA1_DIGEST_SIZE_IN_BYTES,(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)		

/**
   @brief
   CRYS_RSA_PKCS1v15_MD5_Sign implements the RSASSA-PKCS1v15 algorithm 
   as defined in PKCS#1 v1.5, but without performing a HASH function - 
   it assumes that the data in has already been hashed using MD5
   
   Note: The data_in size is already known after the Hash
*/

#define CRYS_RSA_PKCS1v15_MD5_Sign(UserContext_ptr,UserPrivKey_ptr,DataIn_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),CRYS_RSA_After_MD5_mode,CRYS_PKCS1_NO_MGF,0,(DataIn_ptr),CRYS_HASH_MD5_DIGEST_SIZE_IN_BYTES,(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)


/**
   @brief
   CRYS_RSA_PKCS1v15_SHA224_Sign implements the RSASSA-PKCS1v15 algorithm 
   as defined in PKCS#1 v1.5, but without performing a HASH function - 
   it assumes that the data in has already been hashed using SHA-224
   
   Note: The data_in size is already known after the Hash
      
*/
#define CRYS_RSA_PKCS1v15_SHA224_Sign(UserContext_ptr,UserPrivKey_ptr,DataIn_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),(CRYS_RSA_After_SHA224_mode),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),CRYS_HASH_SHA224_DIGEST_SIZE_IN_BYTES,(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)		




/**
   @brief
   CRYS_RSA_PKCS1v15_SHA256_Sign implements the RSASSA-PKCS1v15 algorithm 
   as defined in PKCS#1 v1.5, but without performing a HASH function - 
   it assumes that the data in has already been hashed using SHA-256
   
   Note: The data_in size is already known after the Hash
      
*/
#define CRYS_RSA_PKCS1v15_SHA256_Sign(UserContext_ptr,UserPrivKey_ptr,DataIn_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),(CRYS_RSA_After_SHA256_mode),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),CRYS_HASH_SHA256_DIGEST_SIZE_IN_BYTES,(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)		

/**
   @brief
   CRYS_RSA_PKCS1v15_SHA1_Sign implements the RSASSA-PKCS1v15 algorithm 
   as defined in PKCS#1 v1.5, but without performing a HASH function - 
   it assumes that the data in has already been hashed using SHA-384
   
   Note: The data_in size is already known after the Hash
      
*/
#define CRYS_RSA_PKCS1v15_SHA384_Sign(UserContext_ptr,UserPrivKey_ptr,DataIn_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),(CRYS_RSA_After_SHA384_mode),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),CRYS_HASH_SHA384_DIGEST_SIZE_IN_BYTES,(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)		


/**
   @brief
   CRYS_RSA_PKCS1v15_SHA512_Sign implements the RSASSA-PKCS1v15 algorithm 
   as defined in PKCS#1 v1.5, but without performing a HASH function - 
   it assumes that the data in has already been hashed using SHA-512
   
   Note: The data_in size is already known after the Hash
      
*/
#define CRYS_RSA_PKCS1v15_SHA512_Sign(UserContext_ptr,UserPrivKey_ptr,DataIn_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),(CRYS_RSA_After_SHA512_mode),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),CRYS_HASH_SHA512_DIGEST_SIZE_IN_BYTES,(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)		



/**
   \brief CRYS_RSA_PSS_Sign implements the RSASSA-PSS algorithm
   as defined in PKCS#1 v2.1 9.1, in a single function call.

    Note: According to the PKCS#1 ver2.1 it is not recommended to use MD5 Hash, 
		therefore it is not supported
   
	The actual macro that will be used by the user is:
	CRYS_RSA_PSS_Sign
*/	

#define CRYS_RSA_PSS_Sign(UserContext_ptr,UserPrivKey_ptr,hashFunc,MGF,SaltLen,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign(UserContext_ptr,UserPrivKey_ptr,hashFunc,MGF,SaltLen,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)	


/**
   \brief CRYS_RSA_PSS_SHA1_Sign implements the RSASSA-PSS algorithm
   as defined in PKCS#1 v2.1 9.1 in a single function call, but without 
   performing a HASH function - it assumes that the data in
   has already been hashed using SHA-1

   Note: The data_in size is already known after the Hash
   
	The actual macro that will be used by the users is:
	CRYS_RSA_PSS_SHA1_Sign
*/	

#define CRYS_RSA_PSS_SHA1_Sign(UserContext_ptr,UserPrivKey_ptr,MGF,SaltLen,DataIn_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign(UserContext_ptr,UserPrivKey_ptr,CRYS_RSA_After_SHA1_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA1_DIGEST_SIZE_IN_BYTES,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)	


/**
   \brief CRYS_RSA_PSS_SHA224_Sign implements the RSASSA-PSS algorithm
   as defined in PKCS#1 v2.1 9.1 in a single function call, but without 
   performing a HASH function - it assumes that the data in
   has already been hashed using SHA-224

   Note: The data_in size is already known after the Hash
   
	The actual macro that will be used by the users is:
	CRYS_RSA_PSS_SHA224_Sign
*/	

#define CRYS_RSA_PSS_SHA224_Sign(UserContext_ptr,UserPrivKey_ptr,MGF,SaltLen,DataIn_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign(UserContext_ptr,UserPrivKey_ptr,CRYS_RSA_After_SHA224_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA224_DIGEST_SIZE_IN_BYTES,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)	


/**
   \brief CRYS_RSA_PSS_SHA256_Sign implements the RSASSA-PSS algorithm
   as defined in PKCS#1 v2.1 9.1 in a single function call, but without 
   performing a HASH function - it assumes that the data in
   has already been hashed using SHA-256

   Note: The data_in size is already known after the Hash
   
	The actual macro that will be used by the users is:
	CRYS_RSA_PSS_SHA256_Sign
*/	

#define CRYS_RSA_PSS_SHA256_Sign(UserContext_ptr,UserPrivKey_ptr,MGF,SaltLen,DataIn_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign(UserContext_ptr,UserPrivKey_ptr,CRYS_RSA_After_SHA256_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA256_DIGEST_SIZE_IN_BYTES,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)	



/**
   \brief CRYS_RSA_PSS_SHA384_Sign implements the RSASSA-PSS algorithm
   as defined in PKCS#1 v2.1 9.1 in a single function call, but without 
   performing a HASH function - it assumes that the data in
   has already been hashed using SHA-384

   Note: The data_in size is already known after the Hash
   
	The actual macro that will be used by the users is:
	CRYS_RSA_PSS_SHA384_Sign
*/	

#define CRYS_RSA_PSS_SHA384_Sign(UserContext_ptr,UserPrivKey_ptr,MGF,SaltLen,DataIn_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign(UserContext_ptr,UserPrivKey_ptr,CRYS_RSA_After_SHA384_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA384_DIGEST_SIZE_IN_BYTES,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)	


/**
   \brief CRYS_RSA_PSS_SHA512_Sign implements the RSASSA-PSS algorithm
   as defined in PKCS#1 v2.1 9.1 in a single function call, but without 
   performing a HASH function - it assumes that the data in
   has already been hashed using SHA-512

   Note: The data_in size is already known after the Hash
   
	The actual macro that will be used by the users is:
	CRYS_RSA_PSS_SHA512_Sign
*/	

#define CRYS_RSA_PSS_SHA512_Sign(UserContext_ptr,UserPrivKey_ptr,MGF,SaltLen,DataIn_ptr,Output_ptr,OutputSize_ptr)\
		_DX_RSA_Sign(UserContext_ptr,UserPrivKey_ptr,CRYS_RSA_After_SHA512_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA512_DIGEST_SIZE_IN_BYTES,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)	


/**********************************************************************************************************/
/**
   \brief  RSA_VerifyInit initializes the Verify
   multi-call algorithm as defined in PKCS#1 v1.5 and 2.1

	The actual macro that will be used by the user is:
	CRYS_RSA_PKCS1v15_VerifyInit - for v1.5
	CRYS_RSA_PSS_VerifyInit	  - for v2.1

	NOTE: 1. In PSS_Sign v2.1 MD5 is not supported, since it is not recommended 
		 by the PKCS#1 v2.1.	
	      2. According to thesaid standard, implementation of the function 
		 for version v1.5 is based on DER encoding of the algorithm info. 

	
   @param[in] UserContext_ptr - A pointer to the public Context
			 structure of the User.
   @param[in] UserPubKey_ptr - A pointer to the public key data
			   structure.	                           
   @param[in] rsaHashMode - The hash function to be used. Currently
			 avaliable HASH functions: SHA1/SHA-256/384/512/MD5
			 (MD5 - allowed only for PKCS#1 v1.5).
			 Also allowed "After" HASH modes for said functions. 
   @param[in] MGF - The mask generation function, relevant only for PKCS#1 v2.1.
		    The currently allowed value for v2.1 is CRYS_PKCS1_MGF1.
   @param[in] SaltLen - The Length of the Salt buffer. Relevant for PKCS#1 Ver 2.1 only.
			Typical lengths are 0 and hashLen (20 for SHA1).
			The maximum length allowed is NSize - hLen - 2.
			If the salt length is not available in this process, the user  
			can use the define: CRYS_RSA_VERIFY_SALT_LENGTH_UNKNOWN.
			Security Note: it is recommended not to use this flag and provide 
	                the Salt length on each verify
   @param[in] PKCS1_ver - Ver 1.5 or 2.1, according to the functionality required.
   
   @return CRYSError_t - CRYS_OK, CRYS_INVALID_SIGNATURE,
			 CRYS_BAD_PARAM, CRYS_OUT_OF_RANGE 
*/
CIMPORT_C CRYSError_t _DX_RSA_VerifyInit(CRYS_RSAPubUserContext_t *UserContext_ptr,
                               CRYS_RSAUserPubKey_t *UserPubKey_ptr,
                               CRYS_RSA_HASH_OpMode_t rsaHashMode,
                               CRYS_PKCS1_MGF_t MGF,
                               uint16_t SaltLen,
                               CRYS_PKCS1_version PKCS1_ver);                     
  				  

/**
   \brief CRYS_RSA_PKCS1v15_VerifyInit implements the RSASSA-PKCS1v15 algorithm
   as defined in PKCS#1 v2.1 8.2.1
*/				     
#define CRYS_RSA_PKCS1v15_VerifyInit(UserContext_ptr,UserPubKey_ptr,rsaHashMode)\
		_DX_RSA_VerifyInit(UserContext_ptr,UserPubKey_ptr,rsaHashMode,CRYS_PKCS1_NO_MGF,0,CRYS_PKCS1_VER15)

/*
   \brief CRYS_RSA_PSS_VerifyInit implements the RSASSA-PSS algorithm
   as defined in PKCS#1 v2.1 
   
   Note: According to PKCS#1 ver2.1 MD5 hash is not recommended, 
		therefore it is not supported
*/   
#define CRYS_RSA_PSS_VerifyInit(UserContext_ptr, UserPubKey_ptr, rsaHashMode, MGF,SaltLen)\
		_DX_RSA_VerifyInit(UserContext_ptr, UserPubKey_ptr, rsaHashMode, MGF, SaltLen, CRYS_PKCS1_VER21)
		

/**********************************************************************************************************/
/**
   \brief RSA_VerifyUpdate processes the data to be verified
   in a given context, according to PKCS1 v1.5 and 2.1

   \note RSA_VerifyUpdate can be called multiple times with data
   
	The actual macro that will be used by the user is:
	CRYS_RSA_PKCS1v15_VerifyUpdate		- for v1.5
	CRYS_RSA_PSS_VerifyUpdate		- for v2.1
	
   @param[in] UserContext_ptr - A pointer to the public Context
	                           structure of the User.
   @param[in] DataIn_ptr - A pointer to the data whose signature is
                         to be verified.
   @param[in] DataInSize - The size, in bytes, of the data whose
                         signature is to be verified.

   @return CRYSError_t - CRYS_OK, CRYS_INVALID_SIGNATURE,
                         CRYS_BAD_PARAM, CRYS_OUT_OF_RANGE 
*/

CIMPORT_C CRYSError_t _DX_RSA_VerifyUpdate(CRYS_RSAPubUserContext_t *UserContext_ptr,
                                 uint8_t     *DataIn_ptr,
                                 uint32_t     DataInSize);
		
		
#define CRYS_RSA_PKCS1v15_VerifyUpdate(UserContext_ptr,DataIn_ptr,DataInSize)\
		_DX_RSA_VerifyUpdate(UserContext_ptr,DataIn_ptr,DataInSize)		


#define CRYS_RSA_PSS_VerifyUpdate(UserContext_ptr,DataIn_ptr,DataInSize)\
		_DX_RSA_VerifyUpdate(UserContext_ptr,DataIn_ptr,DataInSize)		


/**********************************************************************************************************/
/**

   \brief RSA_VerifyFinish implements the Finish Phase of the Verify algorithm
   as defined in PKCS#1 v2.1 or PKCS#1 v1.5

	The actual macro that will be used by the user is:
	CRYS_RSA_PKCS1v15_VerifyFinish - for v1.5
	CRYS_RSA_PSS_VerifyFinish	   - for v2.1
	
   @param[in] UserContext_ptr - A pointer to the public Context
	                           structure of the User.
   @param[in] Sig_ptr - A pointer to the signature to be verified. 
                        The length of the signature is PubKey_ptr->N.len bytes 
                        (that is, the size of the modulus, in bytes).

   @return CRYSError_t - CRYS_OK, CRYS_INVALID_SIGNATURE,
                         CRYS_BAD_PARAM, CRYS_OUT_OF_RANGE 
*/

CIMPORT_C CRYSError_t _DX_RSA_VerifyFinish(CRYS_RSAPubUserContext_t *UserContext_ptr,
					uint8_t *Sig_ptr);

								  
/**
   \brief CRYS_RSA_PKCS1v15_VerifyFinish Verifies the signature on the
   data passed to one or more calls to CRYS_RSA_PKCS1v15_VerifyUpdate,
   and releases the context.
*/				     		
#define CRYS_RSA_PKCS1v15_VerifyFinish(UserContext_ptr,Sig_ptr)\
		_DX_RSA_VerifyFinish(UserContext_ptr,Sig_ptr)                     


/**
   \brief CRYS_RSA_PSS_VerifyFinish Verifies the signature on the
   data passed to one or more calls to CRYS_RSA_PSS_VerifyUpdate,
   and releases the context.
*/
#define CRYS_RSA_PSS_VerifyFinish(UserContext_ptr,Sig_ptr)\
		_DX_RSA_VerifyFinish(UserContext_ptr,Sig_ptr)                                  		      	      


/**********************************************************************************************************/
/**
   \brief RSA_Verify implements the RSASSA-PKCS1v15 algorithm
    in a single function as defined in PKCS#1 v2.1 (including v1.5).
   
	The user can call the function by appropriate macro according to choosen 
	(and allowed) HASH algorithm SHA1, SHA224... (see macros below).

	NOTE: 1. In PSS_Verify v2.1 MD5 is not supported, since it is not recommended 
		 by the PKCS#1 ver2.1.	
	      2. According to the said standard, implementation of the function 
		 for version v1.5 is based on DER encoding of hash algorithm ID.

	
   @param[in] UserContext_ptr - A pointer to the public Context, 
   				for the use of the function as a space to work on
   @param[in] UserPubKey_ptr - A pointer to the public key data
                           structure of the user.
   @param[in] rsaHashMode - The hash function to be used. Currently
		      avaliable HASH functions: SHA1/SHA-224/256/384/512, MD5
		      (MD5 - allowed only for PKCS#1 v1.5).
		      Also allowed "After" HASH modes for said functions. 
   @param[in] MGF - The mask generation function. only for PKCS#1 v2.1
                    defines MGF1, so the only value allowed for v2.1 
                    is CRYS_PKCS1_MGF1. 
   @param[in] SaltLen - The Length of the Salt buffer. relevant for PKCS#1 Ver 2.1 Only
			Typical lengths are 0 and hLen (20 for SHA1)
			The maximum length allowed is NSize - hLen - 2                                             
   @param[in] DataIn_ptr - A pointer to the data whose signature is
                         to be verified.
   @param[in] DataInSize - The size, in bytes, of the data whose
                         signature is to be verified.
   @param[in] Sig_ptr - A pointer to the signature to be verified. 
                        The length of the signature is PubKey_ptr->N.len bytes 
                        (that is, the size of the modulus, in bytes).
   @param[in] PKCS1_ver - Ver 1.5 or 2.1, according to the functionality required

   @return CRYSError_t - CRYS_OK, CRYS_INVALID_SIGNATURE,
                         CRYS_BAD_PARAM, CRYS_OUT_OF_RANGE 
*/
				     
CIMPORT_C CRYSError_t _DX_RSA_Verify(CRYS_RSAPubUserContext_t *UserContext_ptr,
                           CRYS_RSAUserPubKey_t *UserPubKey_ptr,
                           CRYS_RSA_HASH_OpMode_t hashFunc,
                           CRYS_PKCS1_MGF_t MGF,
                           uint16_t SaltLen,
                           uint8_t     *DataIn_ptr,
                           uint32_t     DataInSize,
                           uint8_t     *Sig_ptr,
                           CRYS_PKCS1_version PKCS1_ver);

#define CRYS_RSA_PKCS1v15_Verify(UserContext_ptr,UserPubKey_ptr,hashFunc,DataIn_ptr,DataInSize,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,hashFunc,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,DataInSize,Sig_ptr,CRYS_PKCS1_VER15)


/**
   \brief CRYS_RSA_PKCS1v15_MD5_Verify implements the RSASSA-PKCS1v15 Verify algorithm
   as defined in PKCS#1 v1.5, but without operating the HASH function - 
   it assumes the DataIn_ptr data has already been hashed using MD5
   
   \note The data_in size is already known after the Hash
   
	The actual macro that will be used by the user is:
	CRYS_RSA_PKCS1v15_MD5_Verify   
*/

#define CRYS_RSA_PKCS1v15_MD5_Verify(UserContext_ptr,UserPubKey_ptr,DataIn_ptr,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_MD5_mode,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,CRYS_HASH_MD5_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER15)


/**
   \brief CRYS_RSA_PKCS1v15_SHA1_Verify implements the RSASSA-PKCS1v15 Verify algorithm
   as defined in PKCS#1 v1.5, but without operating the HASH function - 
   it assumes that the DataIn_ptr data has already been hashed using SHA1
   
   Note: The data_in size is already known after the Hash
      
	The actual macro that will be used by the users is:
	CRYS_RSA_PKCS1v15_SHA1_Verify   
*/
#define CRYS_RSA_PKCS1v15_SHA1_Verify(UserContext_ptr,UserPubKey_ptr,DataIn_ptr,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA1_mode,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,CRYS_HASH_SHA1_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER15)


/**
   \brief CRYS_RSA_PKCS1v15_SHA224_Verify implements the RSASSA-PKCS1v15 Verify algorithm
   as defined in PKCS#1 v1.5, but without operating the HASH function - 
   it assumes that the DataIn_ptr data has already been hashed using SHA224
   
   Note: The data_in size is already known after the Hash
      
	The actual macro that will be used by the users is:
	CRYS_RSA_PKCS1v15_SHA224_Verify   
*/
#define CRYS_RSA_PKCS1v15_SHA224_Verify(UserContext_ptr,UserPubKey_ptr,DataIn_ptr,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA224_mode,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,CRYS_HASH_SHA224_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER15)



/**
   \brief CRYS_RSA_PKCS1v15_SHA256_Verify implements the RSASSA-PKCS1v15 Verify algorithm
   as defined in PKCS#1 v1.5, but without operating the HASH function - 
   it assumes that the DataIn_ptr data has already been hashed using SHA256
   
   Note: The data_in size is already known after the Hash
      
	The actual macro that will be used by the users is:
	CRYS_RSA_PKCS1v15_SHA256_Verify   
*/
#define CRYS_RSA_PKCS1v15_SHA256_Verify(UserContext_ptr,UserPubKey_ptr,DataIn_ptr,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA256_mode,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,CRYS_HASH_SHA256_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER15)


/**
   \brief CRYS_RSA_PKCS1v15_SHA384_Verify implements the RSASSA-PKCS1v15 Verify algorithm
   as defined in PKCS#1 v1.5, but without operating the HASH function - 
   it assumes that the DataIn_ptr data has already been hashed using SHA384
   
   Note: The data_in size is already known after the Hash
      
	The actual macro that will be used by the users is:
	CRYS_RSA_PKCS1v15_SHA384_Verify   
*/
#define CRYS_RSA_PKCS1v15_SHA384_Verify(UserContext_ptr,UserPubKey_ptr,DataIn_ptr,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA384_mode,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,CRYS_HASH_SHA384_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER15)



/**
   \brief CRYS_RSA_PKCS1v15_SHA512_Verify implements the RSASSA-PKCS1v15 Verify algorithm
   as defined in PKCS#1 v1.5, but without operating the HASH function - 
   it assumes that the DataIn_ptr data has already been hashed using SHA512
   
   Note: The data_in size is already known after the Hash
      
	The actual macro that will be used by the users is:
	CRYS_RSA_PKCS1v15_SHA512_Verify   
*/
#define CRYS_RSA_PKCS1v15_SHA512_Verify(UserContext_ptr,UserPubKey_ptr,DataIn_ptr,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA512_mode,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,CRYS_HASH_SHA512_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER15)


/**
   \brief CRYS_RSA_PKCS1v15_SHA1_Verify implements the RSASSA-PKCS1v15 Verify algorithm
   as defined in PKCS#1 v1.5, but without operating the HASH function - 
   it assumes that the DataIn_ptr has already been hashed using SHA1
   
   Note: The data_in size is already known after Hash
   Note: According to the PKCS#1 ver2.1 it is not recommended to use MD5 Hash, 
		therefore it is not supported
	      
	The actual macro that will be used by the user is:
	CRYS_RSA_PKCS1v15_SHA1_Verify   
*/
		
#define CRYS_RSA_PSS_Verify(UserContext_ptr,UserPubKey_ptr,hashFunc,MGF,SaltLen,DataIn_ptr,DataInSize,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,hashFunc,MGF,SaltLen,DataIn_ptr,DataInSize,Sig_ptr,CRYS_PKCS1_VER21)
			   
			      
/**
   \brief CRYS_RSA_PSS_SHA1_Verify implements the PKCS1v21 Verify algorithm
   as defined in PKCS#1 v2.1, but without operating the HASH function - 
   it assumes the DataIn_ptr has already been hashed using SHA1
      
   \note The data_in size is already known after the Hash
      
	The actual macro that will be used by the user is:
	CRYS_RSA_PKCS1v15_SHA1_Verify   
*/

#define CRYS_RSA_PSS_SHA1_Verify(UserContext_ptr,UserPubKey_ptr,MGF,SaltLen,DataIn_ptr,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA1_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA1_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER21)			      				      	      


/**
   \brief CRYS_RSA_PSS_SHA224_Verify implements the PKCS1v21 Verify algorithm
   as defined in PKCS#1 v2.1, but without operating the HASH function - 
   it assumes the DataIn_ptr has already been hashed using SHA224
      
   \note The data_in size is already known after the Hash
      
	The actual macro that will be used by the user is:
	CRYS_RSA_PKCS1v15_SHA224_Verify   
*/

#define CRYS_RSA_PSS_SHA224_Verify(UserContext_ptr,UserPubKey_ptr,MGF,SaltLen,DataIn_ptr,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA224_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA224_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER21)

/**
   \brief CRYS_RSA_PSS_SHA256_Verify implements the PKCS1v21 Verify algorithm
   as defined in PKCS#1 v2.1, but without operating the HASH function - 
   it assumes the DataIn_ptr has already been hashed using SHA256
      
   \note The data_in size is already known after the Hash
      
	The actual macro that will be used by the user is:
	CRYS_RSA_PKCS1v15_SHA256_Verify   
*/

#define CRYS_RSA_PSS_SHA256_Verify(UserContext_ptr,UserPubKey_ptr,MGF,SaltLen,DataIn_ptr,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA256_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA256_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER21)			      


/**
   \brief CRYS_RSA_PSS_SHA384_Verify implements the PKCS1v21 Verify algorithm
   as defined in PKCS#1 v2.1, but without operating the HASH function - 
   it assumes the DataIn_ptr has already been hashed using SHA384
      
   \note The data_in size is already known after the Hash
      
	The actual macro that will be used by the user is:
	CRYS_RSA_PKCS1v15_SHA384_Verify   
*/

#define CRYS_RSA_PSS_SHA384_Verify(UserContext_ptr,UserPubKey_ptr,MGF,SaltLen,DataIn_ptr,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA384_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA384_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER21)


/**
   \brief CRYS_RSA_PSS_SHA512_Verify implements the PKCS1v21 Verify algorithm
   as defined in PKCS#1 v2.1, but without operating the HASH function - 
   it assumes the DataIn_ptr has already been hashed using SHA512
      
   \note The data_in size is already known after the Hash
      
	The actual macro that will be used by the user is:
	CRYS_RSA_PKCS1v15_SHA512_Verify   
*/

#define CRYS_RSA_PSS_SHA512_Verify(UserContext_ptr,UserPubKey_ptr,MGF,SaltLen,DataIn_ptr,Sig_ptr)\
		_DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA512_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA512_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER21)

/**********************************************************************************************************/


#ifdef __cplusplus
}
#endif			

#endif
