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



#ifndef CRYS_RND_H
#define CRYS_RND_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */
#include "crys_error.h"


#ifdef __cplusplus
extern "C"
{
#endif

/*
*  Object CRYS_RND.h
*  State           :  %state%
*  Creation date   :  Thu Dec 16 17:11:32 2004
*  Last modified   :  %modify_time% 18 Mar. 2010
*/
/** @file
*  \brief This file contains the CRYS APIs used for random number generation.
*
*  \version CRYS_RND.h#1:incl:15
*  \author adams, R.Levin 
* 
*/ 

/************************ Defines ******************************/

	/*  Maximal reseed counter - indicates maximal number of 
	requests allowed between reseeds; according NIST 800-90 
	it is (2^48 - 1), but n our implementation allowed the 
	following restricted value */ 
#define CRYS_RND_MAX_RESEED_COUNTER 	(0xFFFFFFFF - 0xF)

	/* maximal requested size counter (12 bits active) - maximal count 
	of generated random 128 bit blocks allowed per one request of 
	Generate function according NIST 800-90 it is (2^12 - 1) = 0x3FFFF */
#define CRYS_RND_REQUESTED_SIZE_COUNTER  0x3FFFF

	/* AES output block size in words */
#define CRYS_RND_AES_BLOCK_SIZE_IN_WORDS  CRYS_AES_BLOCK_SIZE_IN_WORDS


/* RND seed and additional input sizes */
#define CRYS_RND_SEED_MAX_SIZE_WORDS                  12
#ifndef CRYS_RND_ADDITINAL_INPUT_MAX_SIZE_WORDS
#define CRYS_RND_ADDITINAL_INPUT_MAX_SIZE_WORDS	CRYS_RND_SEED_MAX_SIZE_WORDS
#endif

/* Max size of generated random vector in bits according uint16_t type of   * 
*  input size parameter in CRYS_RND_Generate function   		      */
#define CRYS_RND_MAX_GEN_VECTOR_SIZE_BYTES    0xFFFF

/* allowed sizes of AES Key, in words */
#define CRYS_RND_AES_KEY_128_SIZE_WORDS  4
#define CRYS_RND_AES_KEY_192_SIZE_WORDS  6
#define CRYS_RND_AES_KEY_256_SIZE_WORDS  8

/**   Definitions of temp buffer for RND_DMA version of CRYS_RND   */
/*******************************************************************/ 
#ifdef CRYS_NO_RND_DMA_SUPPORT
	/**   Definitions of temp buffer for non DMA version of CRYS_RND   */

	#define CRYS_RND_WORK_BUFFER_SIZE_WORDS   514
	typedef struct
	{
	   /* include the specific fields that are used by the low level */
	   uint32_t crysRndEntrIntBuff[CRYS_RND_WORK_BUFFER_SIZE_WORDS];
	}CRYS_RND_EntropyEstimatData_t;

	/* Defines for compatibility with other versions */
	#define CRYS_RND_WorkBuff_t   CRYS_RND_EntropyEstimatData_t
	#define crysRndWorkBuff  crysRndEntrIntBuff

#else
	/**   Definitions of temp buffer for DMA version of CRYS_RND   */

	#define CRYS_RND_WORK_BUFFER_SIZE_WORDS 1528 /*1024*/ /*894*/
	typedef struct
	{
		/* include the specific fields that are used by the low level */
		uint32_t crysRndWorkBuff[CRYS_RND_WORK_BUFFER_SIZE_WORDS];
	}CRYS_RND_WorkBuff_t;
	
	/* Defines for compatibility with other versions */
	#define CRYS_RND_EntropyEstimatData_t  CRYS_RND_WorkBuff_t 
	#define crysRndEntrIntBuff  crysRndWorkBuff
#endif

/* RND source buffer inner (entrpopy) offset       */
#define CRYS_RND_TRNG_SRC_INNER_OFFSET_WORDS    2  


/* Max size for one RNG operation */
#define CRYS_RND_MAX_SIZE_OF_OUTPUT_BYTES	3*1024


/************************ Enumerators  ****************************/ 

/* Definition of Fast or Slow mode of random generator (TRNG)*/
typedef  enum
{
	CRYS_RND_Fast  = 0,
	CRYS_RND_Slow  = 1,
	CRYS_RND_ModeLast = 0x7FFFFFFF,
} CRYS_RND_mode_t;



/************************ Structs  *****************************/


/*****************************************************************************/
/**********************        Public Functions      *************************/
/*****************************************************************************/



/** 
 * -----------------------------------------------------------------------------
 *        @brief The function performs instantiation of RNG and creates new
 *        internal State (including Seed) of RNG.
 *
 *        It implements the CTR_DRBG_Instantiate function of 9.1 [1]. 
 *  	  This function must be called at least once per system reset (boot) and
 *        required before any random generation can be produced. 
 * 
 * @param[in/out] rndWorkBuff_ptr - The temp buffer for specific operations 
 *                               on entropy estimator.
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */
CIMPORT_C CRYSError_t CRYS_RND_Instantiation(CRYS_RND_WorkBuff_t  *rndWorkBuff_ptr/*in/out*/);


/** -----------------------------------------------------------------------------
 * @brief The CRYS_RND_UnInstantiation cleans the unused RNG State for security goals.
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */
CIMPORT_C CRYSError_t CRYS_RND_UnInstantiation(void);


/** ------------------------------------------------------------/
 * @brief The function performs reseeding of RNG Seed, and performs: 
 * 	     1. Mixing of additional entropy into the working state.
 *		 2. Mixing additional input provided by the user called additional input buffer.        
 *
 *        The function implements the CTR_DRBG_Reseeding function of 10.2.1.4.2 NIST SP [SP800-90]. 
 *  	  This function must be called if reseed counter > reseed interval,
 *        in our implementation it is 2^32-1. 
 * 
 * @param[in/out] rndWorkBuff_ptr - The temp buffer for specific operations 
 *                               on entropy estimator.
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */
CIMPORT_C CRYSError_t CRYS_RND_Reseeding(CRYS_RND_WorkBuff_t  *rndWorkBuff_ptr/*in/out*/);



/****************************************************************************************/
/**
 * @brief The function performs NIST 800-90, 10.2.1.5.2 algorithm of Generate function.
 * 
 * NOTE: The function should change the data in given output buffer also if an error occurs.
 *        
 * @param[in] outSizeBytes - The required size of random data in bytes.
 * @param[out] out_ptr - The pointer to output buffer. 
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */
CIMPORT_C CRYSError_t CRYS_RND_GenerateVector(
				uint16_t   outSizeBytes, /*in*/
				uint8_t   *out_ptr);     /*out*/


/**********************************************************************************************************/
/** 
 * @brief The CRYS_RND_GenerateVectorInRange function generates a random vector Rand in range:
 *            1 < RandVect < MaxVect,   using the FIPS-PUB 186-2 standard appendix 3 :
 *        The function performs the following:
 *        1.  Check input parameters.
 *        2.  If maxVect != 0 (maxVect is provided), then calculate required size of random 
 *            equaled to actual bit size of MaxVector, else set it = rndSizeInBits.
 *        3.  Calls the CRYS_RND_GenerateVector() function for generating random vector 
 *            RndVect of required size.
 *        4.  If maxVect is provided, then:
 *             4.1. Sets all high bits of RndVect, greatest than MSBit of MaxVector, to 0.
 *             4.2. If size of random vector > 16 bytes, then: 
 *                     4.2.1. Compares high 16 bytes of randVect to maxVect and low limit 
 *                     4.2.2. If condition 1 < randVect16 < maxVect16 is not satisfied, 
 *                            then generate new high 16 bytes rndVect16 and go to step 4.2.1.
 *             4.3. Compares full value of RndVect with MaxVector and with 1 . If condition  
 *                 1 < RandVect < MaxVector is not satisfied, then go to step 3, else go to 6.
 *        5. Else if maxVect is not provided, then set MSBit of rndVect to 1.
 *        6. Output the result and Exit. 
 *
 *         Note: Random and Max vectors are given as sequence of bytes, where LSB is most left byte
 *               and MSB = most right one. 
 *
 * @rndSizeInBits[in]   - If maxVect_ptr is not given, then rndSizeInBits defining the exact size (in bits) 
 *                        of generated random vector. If maxVect is given, then it defines the 
 *	                  size (rounded up to bytes) of the maxVect_ptr buffer. 
 *                        
 * @maxVect_ptr[in]     - The pointer to vector defining a high limit
 *                        of random vector. 
 * @rndVect_ptr[in,out] - The output buffer for the random vector. 
 *	
 * @return CRYSError_t  - On success CRYS_OK is returned, on failure - a value,
 *      		  defined in crys_rnd_error.h.
 */
CIMPORT_C CRYSError_t CRYS_RND_GenerateVectorInRange(
					uint32_t  rndSizeInBits, 
					uint8_t  *maxVect_ptr, 
					uint8_t  *rndVect_ptr );
											         
											         
/*************************************************************************************/
/**
 * @brief The CRYS_RND_AddAdditionalInput is used for:
 *		  Add additional input by the user.
 *		  This data is used to input additional data for random generation
 *	      reseeding operation or Instantiation.
 *		  The size must be 16 or 32 bytes.
 *
 * @AdditonalInput_ptr[in] - The AdditionalInput buffer.
 * 
 * AdditonalInputSize[in] - The size of the AdditionalInput buffer, can be 16 or 32 bytes.
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */
CIMPORT_C CRYSError_t CRYS_RND_AddAdditionalInput( uint8_t *AdditonalInput_ptr,
						uint16_t AdditonalInputSize);
													
/**
 * @brief The CRYS_RND_EnterKatMode function sets KAT mode bit into StateFlag
 *        of global CRYS_RND_WorkingState structure.
 * 
 *   The user must call this function before calling functions performing KAT tests.  
 * 
 * @param entrData_ptr  - entropy data,
 * @param entrSize      - entropy size in bytes,
 * @param nonce_ptr     - nonce,
 * @param nonceSize     - nonce size in bytes,
 * @param workBuff_ptr  - RND working buffer, must be the same buffer, 
 *      		  which should be passed into Instantiation/Reseeding functions.
 *  
 *    Note: Total size of entropy and nonce must be not great than:
 *    	    CRYS_RND_MAX_KAT_ENTROPY_AND_NONCE_SIZE, defined 
 *  
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */
CIMPORT_C CRYSError_t CRYS_RND_EnterKatMode(
			uint8_t            *entrData_ptr,
			uint32_t            entrSize,
			uint8_t            *nonce_ptr,
			uint32_t            nonceSize,
			CRYS_RND_WorkBuff_t  *workBuff_ptr/*out*/);

/**********************************************************************************************************/
/**
 * @brief The CRYS_RND_DisableKatMode function disables KAT mode bit into StateFlag
 *        of global CRYS_RND_WorkingState structure.
 * 
 *   The user must call this function after KAT tests before actual using RND module
 *   (Instantiation etc.).  
 * 
 * @return - no return value.
 */
CIMPORT_C void CRYS_RND_DisableKatMode( void );
								

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CRYS_RND_H */

