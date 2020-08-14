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

 
 
#ifndef _15_CRYS_DH_KG_h_H
#define _15_CRYS_DH_KG_h_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */
#include "crys_dh.h"


#ifdef __cplusplus
extern "C"
{
#endif

  /**
   *  Object %name    : %crys_dh_kg.h
   *  State           :  %state%
   *  Creation date   :  16 Dec. 2007
   *  Last modified   :  %modify_time% 
   */
  /** @file
   *  \brief This module defines the API that supports Diffie-Hellman domain 
   *  parameters and key generation and checking according to ANSI X9.42.
   *
   *  \version crys_dh_kg.h #1:incl:
   *  \author R.Levin. 
   *
   */

/************************ Defines ******************************/

#define CRYS_DH_SEED_MIN_SIZE_IN_BYTES    CRYS_HASH_SHA1_DIGEST_SIZE_IN_BYTES  /*20 bytes */
#define CRYS_DH_SEED_MIN_SIZE_IN_BITS     (CRYS_DH_SEED_MIN_SIZE_IN_BYTES * 8) /* 160 bits */   

/************************ Enums ********************************/


/************************ Typedefs  ****************************/

/** temp buffers, used in different DH KG functions */

/* temp data buffer structure for domain parameters generation in DH */
typedef struct CRYS_DHKGData_t
{
	/* The aligned input and output temp buffers */
	uint32_t TempBuff1[CRYS_DH_MAX_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t TempBuff2[CRYS_DH_MAX_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t TempBuff3[CRYS_DH_MAX_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t TempBuff4[CRYS_DH_MAX_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t TempBuff5[CRYS_DH_MAX_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t TempBuff6[CRYS_DH_MAX_MOD_BUFFER_SIZE_IN_WORDS];

	CRYS_DH_Temp_t  ExpTemps;
}CRYS_DHKGData_t; 

typedef struct CRYS_DHKG_CheckTemp_t
{
	uint32_t         CheckTempBuff[3*CRYS_DH_MAX_MOD_BUFFER_SIZE_IN_WORDS];
	CRYS_DHKGData_t    DhKgBuff;
}CRYS_DHKG_CheckTemp_t; 


/************************ Structs  ******************************/

/************************ Public Variables **********************/

/************************ Public Functions **********************/


/******************************************************************************************/
/**
* @brief The function generates a DH (DLP) domain parameters in GF(P) (see X9.42-2001)
*
*
* @param [in]  modPSizeBits       - The  modulus (prime) P size in bits equal 256*n, where n >= 4. 
* @param [in]  orderQSizeBits     - The size of order Q of generator in bits (m). According to ANSI X9.43:
*                                  m must be multiple of 32 bits and m >= 160. According to ANSI X9.30-1: 
*                                  m = 160 bit. Because Q is prime factor of P-1, it must be <= (P-1)/2,  
*                                  therefore m <= modPSizeBits. 
* @param [in]  seedSizeBits       - The  seed size in bits. Requirements: 
*                                  seedSizeBits >= orderQSizeBits and seedSizeBits <= modPSizeBits ( the 
*                                  last is required by our implementation).
* @param [out] modP_ptr           - The prime modulus P of structure P = J*Q + 1, where Q is prime
*                                  and j is an integer. Size of the buffer for output generated value must 
*                                  be not less, than modulus size. 
* @param [out] orderQ_ptr         - The pointer to the order Q of generator. The size of the buffer for output 
*                                  generated value must be not less, than order size. 
* @param [out] generatorG_ptr     - The pointer to the generator of multiplicative subgroup in GF(P).
*                                  If the pointer == NULL, the function returns an error. Size of the buffer 
*	                            for output generated value must be not less, than modulus size. 
* @param [in/out]generGsizeBytes_ptr - The pointer to the one-word buffer, containing the generator size value (in bytes). 
*                                  The user must set the size of allocated buffer, and the function returns the 
*	                            actual size of the generator in bytes. 
* @param [out] factorJ_ptr        - The pointer to buffer for integer factor J. If the pointer == NULL, the function 
*                                  not puts this parameter out. In this case JsizeBytes_ptr must be set to NULL, 
*                                  otherwise the function returns an error. The size of the buffer must be not less,
*                                  than ( modPSizesBytes - orderQSizeBytes + 1 ). 
* @param [in/out] JsizeBytes_ptr  - The pointer to the size of integer factor J. If the pointer == NULL, 
*                                  the function not puts this parameter out. If output of the factor J is needed, the
*                                  user must set the J size value equal to the size of allocated buffer, and the 
*                                  function returns the actual size of J in bytes. 
* @param [in/out] seedS_ptr       - The random seed used for generation of primes. The size of the buffer for output 
*      			            generated value must be not less, than passed seed size (see above) and not less
*      			            20 bytes (160 bits). 
* @param [in] generateSeed        - The flag, defining whether the seed generated randomly by the function
*                                  (generateSeed = 1), or it is passed by the input (generateSeed = 0).                                 
* @param [out] pgenCounter_ptr    - The pointer to counter of tries to generate the primes. If the pointer == NULL, 
*                                  the function not puts this parameter out.
* @param [out] DHKGBuff_ptr       - The temp buffer for internal calculations. The buffer is defined as structure.
*
* @return CRYSError_t - On success CRYS_OK is returned, on failure - a predefined error code.
*
*     Note:  1. Input and Output vectors are in big endianness (high most bit is left most one).
*            2. For reliability of checking of input parameters, in case that the user don't wont output of
*               some parameters (generator or factorJ), he must set both - a pointer to appropriate buffer and a 
*               pointer to its size equaled to NULL for these parameters, otherwise the function returns an error.
*            2. In case of any error the function may clean the output buffers. 
*
*/
CIMPORT_C CRYSError_t CRYS_DH_CreateDomainParams( 
				uint32_t          modPsizeBits,             /*in */
				uint32_t          orderQsizeBits,           /*in */
				uint32_t          seedSizeBits,             /*in */
				uint8_t          *modP_ptr,                 /*out*/  
				uint8_t          *orderQ_ptr,               /*out*/
				uint8_t          *generatorG_ptr,           /*out*/
				uint32_t         *generGsizeBytes_ptr,      /*in/out*/
				uint8_t          *factorJ_ptr,              /*out*/
				uint32_t         *JsizeBytes_ptr,           /*in/out*/ 
				uint8_t          *seedS_ptr,                /*in/out*/
				int8_t            generateSeed,             /*in*/ 
				uint32_t         *pgenCounter_ptr,          /*out*/
				CRYS_DHKGData_t    *DHKGbuff_ptr              /*in */ );
                          
                                                   
/******************************************************************************************/
/** 
* @brief The function checks the obtained DH domain parameters according X9.42-2001.
*
*        There may be 3 case of checking:
*        1. Checking of primes only ( modulus P and order Q according to passed seed S and pgenCounter).
*           In this case all pointers and sizes of said parameters must be passed (not NULL), but generator 
*           G pointer and it size must be both set to NULL.
*        2. Checking of generator G only in assuming that primes parameters P, Q are valid. In ths case 
*           the user must to pass the P,Q,G pointers and sizes. The seed S pointer and size must be both
*           set to NULL, otherwise the function returns an error.
*        3. Checking all domain parameters. In this case all input parameters must be passed to the function.
*
*        If any of checked domain parameters is not compliant to X9.42-2001 standard and our implementation
*        limitation, the function returns an error according to CRYS_DH_error.h file. 
*
*        NOTE:  Detailed requirements to all used parameters are described above in CRYS_DH_CreateDomainParams 
*               functions API.
*
* @param[out] modP_ptr           - The prime modulus P. Must be of structure P = j*Q + 1, 
*                                  where Q is prime and j is an integer.
* @param[in]  modPSizeBits       - The  modulus (prime) P size in bits equal 256*n, where n >= 4.  
* @param[out] orderQ_ptr         - The pointer to the order Q of generator. 
* @param[in]  orderQSizeBytes    - The size of order of generator in bytes. According to ANSI X9.43:
*                                  m must be multiple of 32 bits and m >= 160. According to ANSI X9.30-1: 
*                                  m = 160 bit. Because Q is prime factor of P-1, it must be <= (P-1)/2.  
*                                  In our implementation required, that orderQSize <= modPSizeBytes/2. 
* @param[in]  generatorG_ptr     - The pointer to the generator of multiplicative subgroup in GF(P).
* @param[in]  generatorSizeBytes - The size of generator in bytes (must be set if generator will be checked). 
* @param[in]  seedS_ptr          - The random seed used for generation of primes (must be set if 
*                                  primes will be checked).
* @param[in]  seedSizeBits       - The seed size in bits. If the seed is used,
*       			   then its size must be:
*       			   seedSizeBits >= orderQSizeBits and
*       			   seedSizeBits <= modPSizeBits ( the last is
*       			   required by our implementation).
* @param[in]  pgenCounter        - The counter of tries to generate the primes (must be set if primes 
*                                  will be checked).
* @param[in] TempBuff_ptr        - The temp buffer of defined structure.
*
* @return CRYSError_t - On success CRYS_OK is returned, on failure or if one or more domain  
*                       parameters are invalid the function returns a predefined error code.
*
*     Note:  Input vectors are in big endianness.
*   
*/
CIMPORT_C CRYSError_t CRYS_DH_CheckDomainParams( 
				uint8_t               *modP_ptr,           /*in */  
				uint32_t               modPsizeBytes,      /*in */
				uint8_t               *orderQ_ptr,         /*in */
				uint32_t               orderQsizeBytes,    /*in */
				uint8_t               *generatorG_ptr,     /*in */
				uint32_t               generatorSizeBytes, /*in */
				uint8_t               *seedS_ptr,          /*in */
				uint32_t               seedSizeBits,       /*in */
				uint32_t               pgenCounter,        /*in */
				CRYS_DHKG_CheckTemp_t   *checkTempBuff_ptr   /*in */ );
                          
                          
#ifdef __cplusplus
}
#endif

#endif
