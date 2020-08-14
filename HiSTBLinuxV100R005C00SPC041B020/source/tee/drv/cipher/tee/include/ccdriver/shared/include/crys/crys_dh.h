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

 
 
#ifndef _15_CRYS_DH_h_H
#define _15_CRYS_DH_h_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */
#include "crys_rsa_types.h"
#include "crys_kdf.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /*
   *  Object %name    : %crys_dh.h
   *  State           :  %state%
   *  Creation date   :  Wed Jan 26 16:54:02 2005
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This module defines the API that supports Diffie-Hellman key exchange 
   *  as defined in PKCS#3 and ANSI9.42 v1.4 (Revised from November 1,1993)
   *
   *  \version crys_dh.h#1:incl:15
   *  \author ohads
   */


/************************ Defines ******************************/

/** The following defines are for the readability of the user*/
#define CRYS_DHPubKey_t  CRYSRSAPubKey_t

#define CRYS_DH_MAX_VALID_KEY_SIZE_VALUE_IN_BITS  CRYS_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_BITS 
#define CRYS_DH_MIN_VALID_KEY_SIZE_VALUE_IN_BITS  1024 /*according to ANSI*/
 
#define CRYS_DH_MAX_MOD_SIZE_IN_BYTES   (CRYS_DH_MAX_VALID_KEY_SIZE_VALUE_IN_BITS / 8)
#define CRYS_DH_MAX_MOD_SIZE_IN_WORDS   (CRYS_DH_MAX_MOD_SIZE_IN_BYTES/sizeof(uint32_t))

#define CRYS_DH_MAX_MOD_BUFFER_SIZE_IN_WORDS (CRYS_DH_MAX_MOD_SIZE_IN_WORDS + 2)
#define CRYS_DH_DOMAIN_GENERATION_MAX_SIZE_BITS  CRYS_RSA_MAX_KEY_GENERATION_HW_SIZE_BITS

/*The following defines are for the readability of the user*/
#define CRYS_DHPrimeData_t   CRYS_RSAPrimeData_t
#define CRYS_DHUserPubKey_t  CRYS_RSAUserPubKey_t
#define CRYS_DH_OtherInfo_t  CRYS_KDF_OtherInfo_t
#define CRYS_DH_COUNT_OF_OTHER_INFO_ENTRIES  CRYS_KDF_COUNT_OF_OTHER_INFO_ENTRIES
#define CRYS_DH_MAX_SIZE_OF_OTHER_INFO_ENTRY  CRYS_KDF_MAX_SIZE_OF_OTHER_INFO_ENTRY /*bytes*/
/* MAX_SIZE_OF_KEYING_DATA definition for CC54 project. For CC52 projects it is
   limited by 2*CRYS_DH_MAX_MOD_SIZE_IN_BYTES */
#define CRYS_DH_MAX_SIZE_OF_KEYING_DATA  CRYS_KDF_MAX_SIZE_OF_KEYING_DATA /*bytes*/

/************************ Enums ********************************/

/* DH operations mode (used in some functions of DH) */
typedef enum
{
   CRYS_DH_PKCS3_mode  = 0,
   CRYS_DH_ANSI_X942_mode = 1,
   CRYS_DH_NumOfModes,

   CRYS_DH_OpModeLast    = 0x7FFFFFFF,

}CRYS_DH_OpMode_t;   

/* HASH operation modes, used in DH */
typedef enum
{
	CRYS_DH_HASH_SHA1_mode		= CRYS_HASH_SHA1_mode,
	CRYS_DH_HASH_SHA224_mode	= CRYS_HASH_SHA224_mode,
	CRYS_DH_HASH_SHA256_mode	= CRYS_HASH_SHA256_mode,
	CRYS_DH_HASH_SHA384_mode	= CRYS_HASH_SHA384_mode,
	CRYS_DH_HASH_SHA512_mode	= CRYS_HASH_SHA512_mode,
	CRYS_DH_HASH_MD5_mode	        = CRYS_HASH_MD5_mode, /* not used in DH */

	CRYS_DH_HASH_NumOfModes		= CRYS_HASH_MD5_mode,
	CRYS_DH_HASH_OperationModeLast  = 0x7FFFFFFF,

}CRYS_DH_HASH_OpMode_t;   

/* key derivation modes, used in DH */
typedef enum
{
	CRYS_DH_ASN1_Der_mode    = CRYS_KDF_ASN1_DerivMode,
	CRYS_DH_Concat_Der_mode  = CRYS_KDF_ConcatDerivMode,
	CRYS_DH_X963_DerMode    = CRYS_KDF_ConcatDerivMode,
	CRYS_DH_OMADRM_DerMode  = CRYS_KDF_OMADRM_DerivMode, /* not used in DH */

	CRYS_DH_der_NumOfModes  = CRYS_DH_OMADRM_DerMode,
	CRYS_DH_DerivationFunc_ModeLast= 0x7FFFFFFF,

}CRYS_DH_DerivationFunc_Mode;


/************************ Typedefs  *************************************/
/* temp buffer structure used for exponentiation in DH functions */
typedef struct
{
	CRYS_DHPrimeData_t PrimeData;
	CRYS_DHPubKey_t    PubKey; 
	uint32_t TempBuff[CRYS_DH_MAX_MOD_BUFFER_SIZE_IN_WORDS];
} CRYS_DH_ExpTemp_t;

/* temp buffer structure used for DH functions (non hybrid) */
typedef struct
{
	CRYS_DHPrimeData_t PrimeData;
	CRYS_DHUserPubKey_t    UserPubKey; 
	uint32_t TempBuff[CRYS_DH_MAX_MOD_BUFFER_SIZE_IN_WORDS];
} CRYS_DH_Temp_t;

/* temp buffer structure used for DH hybrid functions */
typedef struct
{
	CRYS_DHPrimeData_t PrimeData;
	CRYS_DHUserPubKey_t    UserPubKey; 
	uint32_t TempBuff[2*CRYS_DH_MAX_MOD_BUFFER_SIZE_IN_WORDS];
} CRYS_DH_HybrTemp_t;


/************************ Structs  **************************************/

/************************ Public Variables ******************************/

/************************ Public Functions ******************************/

/*******************************************************************************************/

/**
 * @brief _DX_DH_GeneratePubPrv has 2 functionalities:

 	   1. Randomly generates the Client private key Prv.
 	   2. Compute the Client public key which is 
 		  ClientPub = Generator ^ Prv mod Prime. 
 		
 	Note: 1. All buffers arguments are represented in Big-Endian, i.e. MS byte is most left one.
	      2. Assumed, that DH domain parameters (prime modulus P, generator G and order Q) are 
		     trusted or checked before calling this function. The function partly checks only 
			 pointers and sizes.
 	
  @param[in] Generator_ptr 	- Pointer to the Generator octet string.
  @param[in] GeneratorSize 	- Size of the Generator String (in bytes). The size must be not great than PrimeSize.
  @param[in] Prime_ptr 		- Pointer to the Prime octet string P.
  @param[in] PrimeSize 		- Size of the Prime string (in bytes). The size must be not great than 
                                  CRYS_DH_MAX_MOD_SIZE_IN_BYTES.
  @param[in] L 			- Exact length of Private key in bits (relevant only for PKCS#3), defined 
                                  by central authority. Must be not great, than Prime Size in bits.
				   - If L != 0, then L must met to requirements:  1 <= L and 2^(L-1) <= P,
				     Note: in our implementation required L >= 3.
				     in this case force the private key size to be 2^(L-1) <= Prv < 2^l.
				   - If L  = 0, then: 0 < Prv < P-1.
  @param[in] Q_ptr 		- Pointer to the order Q of generator relevant only for ANSI X9.42):
  					1 <= Prv <= q-1   or   1 < Prv < q-1.
  @param[in] QSize	 	- Size of the Q string in bytes. Relevant only for ANSI X9.42. 
    				 The size must be not great than PrimeSize.	  
  @param[in] DH_mode		- Enumerator, declaring whether the mode is PKCS#3 or ANSI X9.42.  
  @param[in] tmpPubKey_ptr	- A pointer to the public key structure. Used for the Exp operation.
  								  The structure doesn't need to be initialized.
  @param[in] tmpPrimeData_ptr 	- A pointer to a structure containing internal buffers, the structure
                                  doesn't need to be initialized.
  @param[out] ClientPrvKey_ptr  - Pointer to the buffer for output the Private key octet string (Prv). 
				  In PKCS#3 this buffer should be at least the following size:
					  - if L is provided the size of buffer: bufSizeBytes*8 >= L.
					  - if L is DX_NULL, then size of buffer >= PrimeSize.
				  In X9.42 size of the buffer must be >= QSize.	  							  	  
  @param[in/out] ClientPrvKeySize_ptr - The user should provide the size of the buffer indicated by ClientPrvKey_ptr.
                                        The function returns the actual size in bytes of the Private key.
  @param[out] ClientPub_ptr 	- A pointer to the Public key octet string.   							  
  @param[in/out] ClientPubSize_ptr - The user should provide the size of the buffer indicated by ClientPub_ptr. 
                                     This Buffer should be at least PrimeSize bytes.
                                     The function returns the actual size in bytes of the generated client public key.
 
  @return CRYSError_t - On success CRYS_OK is returned, on failure an ERROR as defined CRYS_DH_error.h:
			CRYS_DH_INVALID_ARGUMENT_POINTER_ERROR;      
			CRYS_DH_INVALID_ARGUMENT_SIZE_ERROR;
			CRYS_DH_INVALID_ARGUMENT_OPERATION_MODE_ERROR;   						
			CRYS_DH_ARGUMENT_GENERATOR_SMALLER_THAN_ZERO_ERROR;
			CRYS_DH_ARGUMENT_PRIME_SMALLER_THAN_GENERATOR_ERROR;						
			CRYS_DH_INVALID_L_ARGUMENT_ERROR;
			CRYS_DH_ARGUMENT_PRV_SIZE_ERROR;						
			CRYS_DH_INVALID_ARGUMENT_OPERATION_MODE_ERROR;
 */
CIMPORT_C CRYSError_t _DX_DH_GeneratePubPrv(
					uint8_t *Generator_ptr,              /*generator*/
					uint16_t GeneratorSize,
					uint8_t *Prime_ptr,                  /*modulus*/
					uint16_t PrimeSize,
					uint16_t L,         /*Exact length of Private key in bits*/           
					uint8_t *Q_ptr,                      /*order*/ 
					uint16_t QSize,
					CRYS_DH_OpMode_t DH_mode,
					CRYS_DHUserPubKey_t *tmpPubKey_ptr,    /*temp buff*/
					CRYS_DHPrimeData_t  *tmpPrimeData_ptr, /*temp buff*/
					uint8_t *ClientPrvKey_ptr,           /*out*/
					uint16_t *ClientPrvKeySize_ptr,      /*in/out*/
					uint8_t *ClientPub1_ptr,             /*out*/
					uint16_t *ClientPubSize_ptr);        /*in/out*/


/* macro for calling the GeneratePubPrv function on PKCS#3 mode:  Q is irrelevant */
/*--------------------------------------------------------------------------------*/ 
#define CRYS_DH_PKCS3_GeneratePubPrv(Generator_ptr,GeneratorSize,\
				Prime_ptr,PrimeSize,\
				L,\
				tmpPubKey_ptr,tmpPrimeData_ptr,\
				ClientPrvKey_ptr,ClientPrvKeySize_ptr,\
				ClientPub_ptr,ClientPubSize_ptr)\
		_DX_DH_GeneratePubPrv((Generator_ptr),(GeneratorSize),\
				(Prime_ptr),(PrimeSize),\
				(L),\
				(uint8_t *)DX_NULL,(uint16_t)0,\
				CRYS_DH_PKCS3_mode,\
				(tmpPubKey_ptr),(tmpPrimeData_ptr),\
				(ClientPrvKey_ptr),(ClientPrvKeySize_ptr),\
				(ClientPub_ptr),(ClientPubSize_ptr))

/* macro for calling the GeneratePubPrv function on ANSI X9.42 mode:  L is irrelevant */
#define CRYS_DH_ANSI_X942_GeneratePubPrv(Generator_ptr,GeneratorSize,Prime_ptr,PrimeSize,\
				Q_ptr,QSize,\
				tmpPubKey_ptr,tmpPrimeData_ptr,\
				ClientPrvKey_ptr,ClientPrvKeySize_ptr,\
				ClientPub_ptr,ClientPubSize_ptr)\
		_DX_DH_GeneratePubPrv((Generator_ptr),(GeneratorSize),\
				(Prime_ptr),(PrimeSize),\
				(uint16_t)0,\
				(Q_ptr),(QSize),\
				CRYS_DH_ANSI_X942_mode,\
				(tmpPubKey_ptr),(tmpPrimeData_ptr),\
				(ClientPrvKey_ptr),(ClientPrvKeySize_ptr),\
				(ClientPub_ptr),(ClientPubSize_ptr))


/*******************************************************************************************/
/**
 * @brief CRYS_DH_GetSecretKey computes the shared secret key in the following computation:
 	               SecretKey = ServerPubKey ^ ClientPrvKey mod Prime.

   Note:
	- All buffers arguments are represented in Big-Endian.
	- The actual size of private key in bits must be not less than 2 and not great, 
	than actual size of Prime (modulus) in bits (in our implementation); 
	
  @param[in] ClientPrvKey_ptr 	- Pointer to the private key octet string. 
  @param[in] ClientPrvKeySize  	- The Private key Size (in bytes).
  @param[in] ServerPubKey_ptr   - Pointer to the Server public key octet string  
  @param[in] ServerPubKeySize   - The Server Public key Size (in bytes)
  @param[in] Prime_ptr 		- Pointer to the Prime octet string.
  @param[in] PrimeSize 		- Size of the Prime string. 
  @param[in] tmprPubKey_ptr	- A pointer to the temp buffer (public key structure), used 
                                  for the Exponentiation function; the struct doesn't need 
                                  to be initialized.
  @param[in] tmpPrimeData_ptr 	- A pointer to temp buffer (structure containing internal buffers);
                                  the structure doesn't need to be initialized.    
  @param[out] SecretKey_ptr 	- Pointer to output buffer for the secret key octet string.
  				This buffer should be at least of PrimeSize bytes.  							  
  @param[in/out] SecretKeySize_ptr - The user should provide the size in bytes of the buffer indicated by SecretKey_ptr.
                                    The function will return the actual size in bytes of the output secret key.

  @return CRYSError_t - On success CRYS_OK is returned, on failure an ERROR as defined CRYS_DH_error.h:
					    CRYS_DH_INVALID_ARGUMENT_POINTER_ERROR;      
   					    CRYS_DH_INVALID_ARGUMENT_SIZE_ERROR;
					    CRYS_DH_SECRET_KEY_SIZE_OUTPUT_ERROR;
*/
 CIMPORT_C CRYSError_t CRYS_DH_GetSecretKey(
					uint8_t *ClientPrvKey_ptr,
					uint16_t ClientPrvKeySize,
					uint8_t *ServerPubKey_ptr,
					uint16_t ServerPubKeySize,				                    
					uint8_t *Prime_ptr,
					uint16_t PrimeSize,
					CRYS_DHUserPubKey_t *tmpPubKey_ptr,
					CRYS_DHPrimeData_t  *tmpPrimeData_ptr, 				                    
					uint8_t *SecretKey_ptr,
					uint16_t *SecretKeySize_ptr);


 /******************************************************************************************/
/**
 * @brief CRYS_DH_X942_GetSecretData computes the shared secret key as follows:
 		1. OPTIONAL - validate the correctness of the argument keys
 		2. SecretKey = ServerPubKey ^ ClientPrvKey mod Prime
 		3. Use of Derivation function to derive a data key from the secret key 
 
 	The actual APIs that will be used by the user are:
 	CRYS_DH_X942_GetSecret_ASN1
 	CRYS_DH_X942_GetSecret_Concat
 
  	Note: All buffers arguments are represented in Big-Endian format

  @param[in] ClientPrvKey_ptr 	- A pointer to the Private key octet string 
  @param[in] ClientPrvKeySize  	- The Private key size, in bytes
  @param[in] ServerPubKey_ptr   - A pointer to the Server public key octet string - 
  @param[in] ServerPubKeySize   - The Server Public key size, in bytes
  @param[in] Prime_ptr 		- A pointer to the Prime octet string
  @param[in] PrimeSize 		- The size of the Prime string
  @param[in] OtherInfo - The  pointer to structure , containing the data, 
		shared by two entities of agreement and the data sizes. 
		This argument is optional (if not needed - set it to NULL).
  @param[in] hashMode			- The hash function to be used
  @param[in] DerFunc_mode 	- The type of function to derive the secret key to the key data.
  				We use ASN.1-based function or Hash concatenation function mode.
  @param[in] tmpBuff_ptr	- A pointer to the DH temp buffer structure. Not initialized.
  @param[out] SecretKeyData_ptr - A pointer to the buffer for secret keying data.
  @param[in] SecretKeyDataSize - Specifies the derived Secret Key data size needed in bytes.  
				The size can't be larger than CRYS_DH_MAX_SIZE_OF_KEYING_DATA 
				(required by implementation). 

  @return CRYSError_t - On success CRYS_OK is returned, on failure an ERROR as defined CRYS_DH_error.h:
        		CRYS_DH_SECRET_KEYING_DATA_SIZE_ILLEGAL_ERROR;
 
*/
CIMPORT_C CRYSError_t CRYS_DH_X942_GetSecretData(
					uint8_t                  *ClientPrvKey_ptr,
					uint16_t                  ClientPrvKeySize,
					uint8_t                  *ServerPubKey_ptr,
					uint16_t                  ServerPubKeySize,				                    
					uint8_t                  *Prime_ptr,
					uint16_t                  PrimeSize,
					CRYS_DH_OtherInfo_t        *otherInfo_ptr,
					CRYS_DH_HASH_OpMode_t       hashMode,									
					CRYS_DH_DerivationFunc_Mode DerivFunc_mode,
					CRYS_DH_Temp_t             *tmpBuff_ptr,
					uint8_t                  *SecretKeyingData_ptr,
					uint16_t                  SecretKeyingDataSize );

/****************************************************************/
/**
   \@brief CRYS_DH_X942_GetSecretDataAsn1 implements the DH ANSI X9.42 standard. It derives a secret key 
   		  using the Derivation function based on ASN1.
          Note: deleting of private ephemeral key (if it used) is out of scope of this function and must be performed 
                by user after calling this function. 
*/
#define CRYS_DH_X942_GetSecretDataAsn1(ClientPrvKey_ptr,ClientPrvKeySize,ServerPubKey_ptr,ServerPubKeySize,Prime_ptr,PrimeSize,otherInfo_ptr,hashMode,tmpBuff_ptr,SecretKeyingData_ptr,SecretKeyingDataSize)\
	CRYS_DH_X942_GetSecretData((ClientPrvKey_ptr),(ClientPrvKeySize),(ServerPubKey_ptr),(ServerPubKeySize),(Prime_ptr),(PrimeSize),(otherInfo_ptr),(hashMode),(CRYS_DH_ASN1_Der_mode),(tmpBuff_ptr),(SecretKeyingData_ptr),(SecretKeyingDataSize))
/**
   \@brief  CRYS_DH_X942_GetSecretDataConcat implements the DH ANSI X9.42 standard. It derives a secret key 
   			using the Derivation function based on concatenation of SHA1 hash 
          Note: deleting of private ephemeral key (if it used) is out of scope of this function and must be performed 
                by user after calling this function. 
*/				                    
#define CRYS_DH_X942_GetSecretDataConcat(ClientPrvKey_ptr,ClientPrvKeySize,ServerPubKey_ptr,ServerPubKeySize,Prime_ptr,PrimeSize,otherInfo_ptr,hashMode,tmpBuff_ptr,SecretKeyingData_ptr,SecretKeyingDataSize)\
	CRYS_DH_X942_GetSecretData((ClientPrvKey_ptr),(ClientPrvKeySize),(ServerPubKey_ptr),(ServerPubKeySize),(Prime_ptr),(PrimeSize),(otherInfo_ptr),(hashMode),(CRYS_DH_Concat_Der_mode),(tmpBuff_ptr),(SecretKeyingData_ptr),(SecretKeyingDataSize))
				                    

/****************************************************************/
/**
 * @brief CRYS_DH_X942_HybridGetSecretData computes the shared secret key as follows:
 		1. OPTIONAL - validate the correctness of the argument keys
 		2. SecretKey1 = ServerPubKey1 ^ ClientPrvKey1 mod Prime
 		3. SecretKey2 = ServerPubKey2 ^ ClientPrvKey2 mod Prime
 		3. Use of Derivation function to derive a secret data key from the 2 secret keys 
 
 	The actual APIs that will be used by the user are:
 	CRYS_DH_X942_Hybrid_GetSecret_ASN1
 	CRYS_DH_X942_Hybrid_GetSecret_Concat
 
  	Note: All buffers arguments are represented in Big-Endian format.
 
  @param[in] ClientPrvKey_ptr1 	- A pointer to the First Private key octet string number 
  @param[in] ClientPrvKeySize1 	- The First Private key Size, in bytes
  @param[in] ClientPrvKey_ptr2 	- A pointer to the Second Private key octet string
  @param[in] ClientPrvKeySize2 	- The Second Private key Size, in bytes
  @param[in] ServerPubKey_ptr1  - A pointer to the First Server public key octet string
  @param[in] ServerPubKeySize1  - The First Server Public key Size, in bytes
  @param[in] ServerPubKey_ptr2  - A pointer to the Second Server public key octet string
  @param[in] ServerPubKeySize2  - The Second Server Public key Size, in bytes
  @param[in] Prime_ptr 		- A pointer to the Prime octet string
  @param[in] PrimeSize 		- The size of the Prime string
  @param[in] OtherInfo - The  pointer to structure , containing the data, 
		shared by two entities of agreement and the data sizes. 
		This argument is optional (if not needed - set it to NULL).
  @param[in] hashMode		- The hash function to be used.
  @param[in] DerFunc_mode 	- The type of function to derive the secret key to the key data.
                                  We use an ASN.1-based function or a Hash concatenation function mode.
  @param[in] tmpDhHybr_ptr      - A pointer to the DH hybrid temp byffer structure. Not initialized.
  @param[out] SecretKeyData_ptr - A pointer to the buffer for secret keying data.
  @param[in] SecretKeyDataSize - Specifies the derived Secret Key data size needed in bytes.  
				The size can't be 0 or larger than CRYS_DH_MAX_SIZE_OF_KEYING_DATA 
				(required by implementation). 

  @return CRYSError_t - On success the value CRYS_OK is returned, and on failure an ERROR as defined in CRYS_DH_error.h:
			CRYS_DH_SECRET_KEYING_DATA_SIZE_ILLEGAL_ERROR;
			CRYS_DH_X942_HYBRID_SIZE1_BUFFER_ERROR;
			CRYS_DH_INVALID_ARGUMENT_POINTER_ERROR;      
			CRYS_DH_INVALID_ARGUMENT_SIZE_ERROR;
			CRYS_DH_INVALID_ARGUMENT_OPERATION_MODE_ERROR;   						
			CRYS_DH_ARGUMENT_GENERATOR_SMALLER_THAN_ZERO_ERROR;
			CRYS_DH_ARGUMENT_PRIME_SMALLER_THAN_GENERATOR_ERROR;						
			CRYS_DH_INVALID_L_ARGUMENT_ERROR;
			CRYS_DH_ARGUMENT_PRV_SIZE_ERROR;						
*/
CIMPORT_C CRYSError_t CRYS_DH_X942_HybridGetSecretData(
				uint8_t            *ClientPrvKey_ptr1,
				uint16_t            ClientPrvKeySize1,
				uint8_t            *ClientPrvKey_ptr2,
				uint16_t            ClientPrvKeySize2,				                    
				uint8_t            *ServerPubKey_ptr1,
				uint16_t            ServerPubKeySize1,
				uint8_t            *ServerPubKey_ptr2,
				uint16_t            ServerPubKeySize2,
				uint8_t            *Prime_ptr,
				uint16_t            PrimeSize,
				CRYS_DH_OtherInfo_t  *otherInfo_ptr,
				CRYS_DH_HASH_OpMode_t hashMode,
				CRYS_DH_DerivationFunc_Mode DerivFunc_mode,
				CRYS_DH_HybrTemp_t   *tmpDhHybr_ptr,
				uint8_t            *SecretKeyingData_ptr,
				uint16_t            SecretKeyingDataSize);

				                    			  
/****************************************************************/
/**
\brief   The CRYS_DH_X942_HybridGetSecretDataAsn1 function implements the DH ANSI X9.42 
standard for deriving a hybrid secret key from 2 public-private pair of keys using 
the Derivation function based on ASN1.
*/
#define CRYS_DH_X942_HybridGetSecretDataAsn1(ClientPrvKey_ptr1,ClientPrvKeySize1,ClientPrvKey_ptr2,ClientPrvKeySize2,ServerPubKey_ptr1,ServerPubKeySize1,ServerPubKey_ptr2,ServerPubKeySize2,Prime_ptr,PrimeSize,otherInfo_ptr,hashFunc,tmpDhHybr_ptr,SecretKeyingData_ptr,SecretKeyingDataSize)\
	CRYS_DH_X942_HybridGetSecretData((ClientPrvKey_ptr1),(ClientPrvKeySize1),(ClientPrvKey_ptr2),(ClientPrvKeySize2),(ServerPubKey_ptr1),(ServerPubKeySize1),(ServerPubKey_ptr2),(ServerPubKeySize2),(Prime_ptr),(PrimeSize),(otherInfo_ptr),(hashFunc),CRYS_DH_ASN1_Der_mode,(tmpDhHybr_ptr),(SecretKeyingData_ptr),(SecretKeyingDataSize))

/**
   \brief CRYS_DH_X942_HybridGetSecretDataConcat implements the DH ANSI X9.42 standard deriving a hybrid secret key 
   			from 2 public-private pair of keys using the Derivation function based on concatenation using SHA-1 Hash
*/				                    			  
#define CRYS_DH_X942_HybridGetSecretDataConcat(ClientPrvKey_ptr1,ClientPrvKeySize1,ClientPrvKey_ptr2,ClientPrvKeySize2,ServerPubKey_ptr1,ServerPubKeySize1,ServerPubKey_ptr2,ServerPubKeySize2,Prime_ptr,PrimeSize,otherInfo_ptr,hashFunc,tmpDhHybr_ptr,SecretKeyingData_ptr,SecretKeyingDataSize)\
	CRYS_DH_X942_HybridGetSecretData((ClientPrvKey_ptr1),(ClientPrvKeySize1),(ClientPrvKey_ptr2),(ClientPrvKeySize2),(ServerPubKey_ptr1),(ServerPubKeySize1),(ServerPubKey_ptr2),(ServerPubKeySize2),(Prime_ptr),(PrimeSize),(otherInfo_ptr),(hashFunc),CRYS_DH_Concat_Der_mode,(tmpDhHybr_ptr),(SecretKeyingData_ptr),(SecretKeyingDataSize))
                             

/******************************************************************************************/
/** 
 * @brief The function checks the obtained DH public key according to its domain 
 *        parameters (see X9.42-2001)
 *
 * @param[out] modP_ptr           - The modulus (prime) P.
 * @param[in]  modPSizeBytes      - The modulus size in bytes.
 * @param[in]  orderQ_ptr         - The pointer to prime order Q of generator,
 * @param[in]  orderQSizeBytes    - The size of order of generator in bytes.
 * @param[in]  pubKey_ptr         - The pointer to public key to be validated . 
 * @param[in]  pubKeySizeBytes    - The public key size in bytes. 
 * @param[in]  tempBuff_ptr       - The temp buffer for internal calculations.
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure or if one or more domain  
 *                       parameters are invalid the function returns a predefined error code.
 *
 *              Assuming: The DH domain parameters are valid.
 */
CIMPORT_C CRYSError_t CRYS_DH_CheckPubKey( 
					uint8_t              *modP_ptr,             /*in */  
					uint32_t              modPsizeBytes,        /*in */
					uint8_t              *orderQ_ptr,           /*in */
					uint32_t              orderQsizeBytes,      /*in */
					uint8_t              *pubKey_ptr,           /*in */
					uint32_t              pubKeySizeBytes,      /*in */
					CRYS_DH_Temp_t         *tempBuff_ptr          /*in */);
                          

#ifdef __cplusplus
}
#endif

#endif
