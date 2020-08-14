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

 
 
#ifndef CRYS_KDF_h_H
#define CRYS_KDF_h_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */
#include "crys_hash.h"

#ifdef __cplusplus
extern "C"
{
#endif


  /*
   *  Object %name    : %crys_kdf.h State:
   *  %state% Creation date   :
   *  13 May 2007. Last modified   :
   *  %modify_time%
   */
  /** @file
   *  \brief This module defines the API that supports Key derivation function in modes
   *  as defined in PKCS#3, ,ANSI X9.42-2001, ANSI X9.63-1999, OMA-TS-DRM-DRM-V2_0-20050712-C.
   *
   *  \version crys_kdf.h#1:incl:15
   *  \author R.Levin 
   */

#include "crys_hash.h"


/************************ Defines ******************************/

/* Shared secret value max size in bytes: must be defined according to      *   
*  DH max. modulus size definition!     				    */
#define  CRYS_KDF_MAX_SIZE_OF_SHARED_SECRET_VALUE  1024/*bytes*/ 

/* Count and max. sizeof OtherInfo entries (pointers to data buffers) */
#define  CRYS_KDF_COUNT_OF_OTHER_INFO_ENTRIES   5
#define  CRYS_KDF_MAX_SIZE_OF_OTHER_INFO_ENTRY  64 /*bytes*/
/* Max size of keying data in bytes to generate */
#define  CRYS_KDF_MAX_SIZE_OF_KEYING_DATA  2048 

/************************ Enums ********************************/

typedef enum
{
	CRYS_KDF_HASH_SHA1_mode      = 0,
	CRYS_KDF_HASH_SHA224_mode	= 1,
	CRYS_KDF_HASH_SHA256_mode	= 2,
	CRYS_KDF_HASH_SHA384_mode	= 3,
	CRYS_KDF_HASH_SHA512_mode	= 4,
	
	CRYS_KDF_HASH_NumOfModes,
	
	CRYS_KDF_HASH_OpModeLast    = 0x7FFFFFFF,

}CRYS_KDF_HASH_OpMode_t;   

typedef enum
{
	CRYS_KDF_ASN1_DerivMode    = 0,
	CRYS_KDF_ConcatDerivMode   = 1,
	CRYS_KDF_X963_DerivMode    = CRYS_KDF_ConcatDerivMode,
	CRYS_KDF_OMADRM_DerivMode  = 2,
	CRYS_KDF_ISO18033_KDF1_DerivMode = 3,
	CRYS_KDF_ISO18033_KDF2_DerivMode = 4,
	
	CRYS_KDF_DerivFunc_NumOfModes = 5,
	
	CRYS_KDF_DerivFuncModeLast= 0x7FFFFFFF,
	
}CRYS_KDF_DerivFuncMode_t;   

/************************ Typedefs  ****************************/

/* Structure, containing the optional data for KDF, 
   if any data is not needed, then the pointer value and 
   the size must be set to NULL */ 
typedef struct
{
	/* a unique object identifier (OID), indicating algorithm(s) 
	for which the keying data will be used*/
	uint8_t    AlgorithmID[CRYS_KDF_MAX_SIZE_OF_OTHER_INFO_ENTRY]; 
	uint32_t   SizeOfAlgorithmID;
	/* Public information contributed by the initiator */
	uint8_t    PartyUInfo[CRYS_KDF_MAX_SIZE_OF_OTHER_INFO_ENTRY]; 
	uint32_t   SizeOfPartyUInfo;
	/* Public information contributed by the responder */   
	uint8_t    PartyVInfo[CRYS_KDF_MAX_SIZE_OF_OTHER_INFO_ENTRY]; 
	uint32_t   SizeOfPartyVInfo;
	/* Mutually-known private information, e.g. shared information 
	communicated throgh a separate channel */
	uint8_t    SuppPrivInfo[CRYS_KDF_MAX_SIZE_OF_OTHER_INFO_ENTRY]; 
	uint32_t   SizeOfSuppPrivInfo;
	/* Mutually-known public information, */
	uint8_t    SuppPubInfo[CRYS_KDF_MAX_SIZE_OF_OTHER_INFO_ENTRY]; 
	uint32_t   SizeOfSuppPubInfo;

}CRYS_KDF_OtherInfo_t;

/************************ Structs  ******************************/

/************************ Public Variables **********************/

/************************ Public Functions **********************/

/****************************************************************/


/*********************************************************************************************************/
/**
 * @brief CRYS_KDF_KeyDerivFunc performs key derivation according to one of some modes defined in standards: 
		ANSI X9.42-2001, ANSI X9.63, OMA_TS_DRM_DRM_V2_0-20050712-C, ISO/IEC 18033-2.
	
	The present implementation of the function allows the following operation modes:
	- CRYS_KDF_ASN1_DerivMode - mode based on  ASN.1 DER encoding;
	- CRYS_KDF_ConcatDerivMode - mode based on concatenation;
	- CRYS_KDF_X963_DerivMode = CRYS_KDF_ConcatDerivMode;
	- CRYS_KDF_OMA_DRM_DerivMode - specific mode for OMA DRM;
	- CRYS_KDF_ISO18033_KDF1_DerivMode - specific mode according to 
		ECIES-KEM algorithm (ISO/IEC 18033-2).

	The purpose of this function is to derive a keying data from the shared secret value and some 
	other optional shared information (SharedInfo).
		
	For calling the API on some specific modes may be used the following macros:
	- CRYS_KDF_ASN1_KeyDerivFunc ;			
	- CRYS_KDF_ConcatKeyDerivFunc ;
	- CRYS_KDF_OMADRM_DerivFunc.
 			
  \note The length in Bytes of the hash result buffer is denoted by "hashlen".
  \note All buffers arguments are represented in Big-Endian format.
 
  @param[in] ZZSecret_ptr - A pointer to shared secret value octet string. 
  @param[in] ZZSecretSize - The size of the shared secret value in bytes. The maximal
		size is defined as: CRYS_KDF_MAX_SIZE_OF_SHARED_SECRET_VALUE.
  @param[in] OtherInfo - The  pointer to structure , containing the data, 
		shared by two entities of agreement and the data sizes. 
		This argument is optional for some modes (if it is not needed - set NULL).
		On KDF OMA_DRM and two ISO/IEC 18033-2 modes - set NULL. 
        	On KDF ASN1 mode the OtherInfo and its AlgorithmID entry are mandatory.
  @param[in] KDFhashMode - The KDF identifier of hash function to be used. 
		The hash function output must be at least 160 bits.
  @param[in] derivation_mode - Specifies one of above described derivation modes.
  @param[out] KeyingData_ptr - A pointer to the buffer for derived keying data.
  @param[in] KeyingDataSizeBytes - The size in bytes of the keying data to be derived. 
		The maximal size is defined as: CRYS_KDF_MAX_SIZE_OF_KEYING_DATA.
 
  @return CRYSError_t - On success the value CRYS_OK is returned, 
			and on failure an ERROR as defined in CRYS_KDF_error.h:
			CRYS_KDF_INVALID_ARGUMENT_POINTER_ERROR
			CRYS_KDF_INVALID_KEY_DERIVATION_MODE_ERROR
			CRYS_KDF_INVALID_SHARED_SECRET_VALUE_SIZE_ERROR
			CRYS_KDF_INVALID_SIZE_OF_DATA_TO_HASHING_ERROR
			CRYS_KDF_INVALID_ARGUMENT_HASH_MODE_ERROR
			CRYS_KDF_INVALID_OTHER_INFO_SIZE_ERROR
			CRYS_KDF_INVALID_KEYING_DATA_SIZE_ERROR
*/
CIMPORT_C CRYSError_t  CRYS_KDF_KeyDerivFunc( 
					uint8_t                *ZZSecret_ptr,
					uint32_t                ZZSecretSize,
					CRYS_KDF_OtherInfo_t     *OtherInfo_ptr,
					CRYS_KDF_HASH_OpMode_t    KDFhashMode,
					CRYS_KDF_DerivFuncMode_t  derivation_mode,
					uint8_t                *KeyingData_ptr,
					uint32_t                KeyingDataSizeBytes );
				                    
/* alias for backward compatibility */
#define _DX_KDF_KeyDerivFunc   CRYS_KDF_KeyDerivFunc

/*********************************************************************************************************/
/**
 * @brief CRYS_KDF_ASN1_KeyDerivFunc performs key derivation according to ASN1 DER encoding method defined 
 		  in standard  ANSI X9.42-2001, 7.2.1.

		  This is the macro for calling the CRYS_KeyDerivationFunc on said mode (arguments and errors description see in 
		  function definitions).
*/				                    
#define CRYS_KDF_ASN1_KeyDerivFunc(ZZSecret_ptr,ZZSecretSize,OtherInfo_ptr,KDFhashMode,KeyingData_ptr,KeyLenInBytes)\
		CRYS_KDF_KeyDerivFunc((ZZSecret_ptr),(ZZSecretSize),(OtherInfo_ptr),(KDFhashMode),CRYS_KDF_ASN1_DerivMode,(KeyingData_ptr),(KeyLenInBytes))
				                    

/*********************************************************************************************************/
/**
 * @brief CRYS_KDF_ConcatKeyDerivFunc performs key derivation according to concatenation mode defined 
 		  in standard  ANSI X9.42-2001, 7.2.2.

		  This is the macro for calling the CRYS_KeyDerivationFunc on said mode (arguments and errors description see in 
		  function definitions).
*/	
#define CRYS_KDF_ConcatKeyDerivFunc(ZZSecret_ptr,ZZSecretSize,OtherInfo_ptr,KDFhashMode,KeyingData_ptr,KeyLenInBytes)\
		CRYS_KDF_KeyDerivFunc((ZZSecret_ptr),(ZZSecretSize),(OtherInfo_ptr),(KDFhashMode),CRYS_KDF_ConcatDerivMode,(KeyingData_ptr),(KeyLenInBytes))


/*********************************************************************************************************/
/**
 * @brief CRYS_KDF_OMADRM_KeyDerivFunc performs key derivation according to concatenation mode defined 
	in standard  ANSI X9.42-2001, 7.2.2 and specific requirements of OMA_TS_DRM_DRM_V2_0-20050712-C.
	
	The specific requirements have respect to following arguments of called _DX_KeyDerivationFunc function:
	- OtherInfo_ptr = NULL,
	- KDFhashMode = CRYS_KDF_HASH_SHA1_mode;
	- derivation_mode = CRYS_KDF_ConcatDerivMode.
	
	This is the macro for calling the CRYS_KeyDerivationFunc on said mode (arguments and errors description see in 
	function definitions).
*/	
#define CRYS_KDF_OMADRM_KeyDerivFunc(ZZSecret_ptr,ZZSecretSize,KeyingData_ptr,KeyLenInBytes)\
		CRYS_KDF_KeyDerivFunc((ZZSecret_ptr),(ZZSecretSize),DX_NULL,CRYS_KDF_HASH_SHA1_mode,CRYS_KDF_ConcatDerivMode,(KeyingData_ptr),(KeyLenInBytes))



#ifdef __cplusplus
}
#endif

#endif

