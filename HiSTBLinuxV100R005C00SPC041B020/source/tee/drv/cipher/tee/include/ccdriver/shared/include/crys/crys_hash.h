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

/** \file 
 * \brief This file contains all of the enums and definitions 
 *        that are used for the CRYS HASH APIs, as well as the APIs themseleves
 *
 */

#ifndef CRYS_HASH_H
#define CRYS_HASH_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */


#include "dx_pal_types.h"
#include "crys_error.h"



#ifdef __cplusplus
extern "C"
{
#endif


/************************ Defines ******************************/

/** 
@brief - a definition describing the low level Engine type ( SW , Hardware , Etc )
*/
#if defined(DX_CC_SEP) || defined(DX_SEP_HOST_RPC_CONFIG)
#define CRYS_HASH_USER_CTX_SIZE_IN_WORDS 33
#elif defined(DX_CC_TEE)
/*Inorder to allow contiguous context the user context is doubled + 3 words for managment */
#define CRYS_HASH_USER_CTX_SIZE_IN_WORDS 133
#else
#define CRYS_HASH_USER_CTX_SIZE_IN_WORDS 256
#endif

/* The hash result in words 
#define CRYS_HASH_RESULT_SIZE_IN_WORDS 5*/
/* The maximum hash result is 512 bits for SHA512 */
#define CRYS_HASH_RESULT_SIZE_IN_WORDS	16

/* The MD5 digest result size in bytes */
#define CRYS_HASH_MD5_DIGEST_SIZE_IN_BYTES 16

/* The MD5 digest result size in words */
#define CRYS_HASH_MD5_DIGEST_SIZE_IN_WORDS 4

/* The SHA-1 digest result size in bytes */
#define CRYS_HASH_SHA1_DIGEST_SIZE_IN_BYTES 20

/* The SHA-1 digest result size in words */
#define CRYS_HASH_SHA1_DIGEST_SIZE_IN_WORDS 5

/* The SHA-256 digest result size in bytes */
#define CRYS_HASH_SHA224_DIGEST_SIZE_IN_WORDS 7

/* The SHA-256 digest result size in bytes */
#define CRYS_HASH_SHA256_DIGEST_SIZE_IN_WORDS 8

/* The SHA-384 digest result size in bytes */
#define CRYS_HASH_SHA384_DIGEST_SIZE_IN_WORDS 12

/* The SHA-512 digest result size in bytes */
#define CRYS_HASH_SHA512_DIGEST_SIZE_IN_WORDS 16

/* The SHA-256 digest result size in bytes */
#define CRYS_HASH_SHA224_DIGEST_SIZE_IN_BYTES 28

/* The SHA-256 digest result size in bytes */
#define CRYS_HASH_SHA256_DIGEST_SIZE_IN_BYTES 32

/* The SHA-384 digest result size in bytes */
#define CRYS_HASH_SHA384_DIGEST_SIZE_IN_BYTES 48

/* The SHA-512 digest result size in bytes */
#define CRYS_HASH_SHA512_DIGEST_SIZE_IN_BYTES 64 

/* The SHA1 hash block size in words */
#define CRYS_HASH_BLOCK_SIZE_IN_WORDS 16

/* The SHA1 hash block size in bytes */
#define CRYS_HASH_BLOCK_SIZE_IN_BYTES 64

/* The SHA2 hash block size in words */
#define CRYS_HASH_SHA512_BLOCK_SIZE_IN_WORDS	32

/* The SHA2 hash block size in bytes */
#define CRYS_HASH_SHA512_BLOCK_SIZE_IN_BYTES	128



/************************ Enums ********************************/

/* Defines the enum for the HASH operation mode */
typedef enum {
	CRYS_HASH_SHA1_mode          = 0,
	CRYS_HASH_SHA224_mode        = 1,
	CRYS_HASH_SHA256_mode        = 2,
	CRYS_HASH_SHA384_mode        = 3,
	CRYS_HASH_SHA512_mode        = 4,
	CRYS_HASH_MD5_mode           = 5,

	CRYS_HASH_NumOfModes,

	CRYS_HASH_OperationModeLast= 0x7FFFFFFF,

}CRYS_HASH_OperationMode_t;   

/************************ Typedefs  *****************************/

/* Defines the IV counter buffer */
typedef uint32_t CRYS_HASH_Result_t[CRYS_HASH_RESULT_SIZE_IN_WORDS];

/************************ Structs  ******************************/
/* The user's context prototype - the argument type that will be passed by the user 
   to the APIs called by him */
typedef struct CRYS_HASHUserContext_t {
	uint32_t buff[CRYS_HASH_USER_CTX_SIZE_IN_WORDS];
}CRYS_HASHUserContext_t;

/************************ Public Variables **********************/

/************************ Public Functions **********************/


/************************************************************************************************/
/**
 * This function initializes the HASH machine on the CRYS level.
 *
 * This function allocates and initializes the HASH Context.
 * The function receives as input a pointer to store the context handle to the HASH Context,
 * and it initializes the HASH Context with the cryptographic attributes that are needed for 
 * the HASH block operation (initializes H's value for the HASH algorithm).
 * 
 * @param[in] ContextID_ptr - A pointer to the HASH context buffer allocated by the user 
 *                       that is used for the HASH machine operation.
 *
 * @param[in] OperationMode - The operation mode: CRYS_HASH_SHA1_mode, CRYS_HASH_SHA224_mode, CRYS_HASH_SHA256_mode,
 *												  CRYS_HASH_SHA384_mode, CRYS_HASH_SHA512_mode, CRYS_HASH_MD5_mode
 *
 * @return CRYSError_t - On success the function returns the value CRYS_OK, 
 *				and on failure a non-ZERO error.
 *
 */
CIMPORT_C CRYSError_t CRYS_HASH_Init(CRYS_HASHUserContext_t     *ContextID_ptr,
				     CRYS_HASH_OperationMode_t  OperationMode);

/************************************************************************************************/
/**
 * This function process a block of data via the HASH Hardware/Software.
 *
 * The function receives as input an handle to the HASH Context that was previously 
 * initialized by a CRYS_HASH_Init function or by another call to a CRYS_HASH_Update function. \n
 * The function sets the last H's value that was stored in 
 * the CRYS HASH context, and then processes the data block. 
 * At the end of the process, the function stores in the HASH context the H's value, 
 * together with the cryptographic attributes that are needed for 
 * the HASH block operation (initialize H's value for the HASH algorithm).
 * This function is used in cases where the data is not all arranged in one 
 * continuous buffer. \n
 * 
 * @param[in] ContextID_ptr - a pointer to the HASH context buffer allocated by the user that
 *                       is used for the HASH machine operation.
 *
 * @param[in] DataIn_ptr - The pointer to the buffer of the input data to the HASH. The pointer does 
 *                         not need to be aligned. On CSI input mode the pointer must be equal to
 *                         value (0xFFFFFFFC | DataInAlignment). 
 * 
 * @param[in] DataInSize - The size of the data to be hashed in bytes. On CSI data transfer mode the size must  
 *                         multiple of HASH_BLOCK_SIZE for used HASH mode.
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* crys_hash_error.h
 *
 *
 */

CIMPORT_C CRYSError_t CRYS_HASH_Update(CRYS_HASHUserContext_t  *ContextID_ptr,
				       uint8_t                 *DataIn_ptr,
				       uint32_t                 DataInSize );


/************************************************************************************************/
/**
 * This function finalizes the hashing process of data block.
 * The function receives as input an handle to the HASH Context , that was initialized before
 * by an CRYS_HASH_Init function or by CRYS_HASH_Update function.
 * The function "adds" an header to the data block as the specific hash standard 
 * specifics, then it loads the engine and reads the final message digest.
 *
 *  @param[in] ContextID_ptr - a pointer to the HASH context buffer allocated by the user that
 *                       is used for the HASH machine operation.
 *
 *  @retval HashResultBuff a pointer to the target buffer where the 
 *                       HASE result stored in the context is loaded to.
 *
 *  @return CRYSError_t - On success the function returns the value CRYS_OK, 
 *				and on failure a non-ZERO error.
 */

CIMPORT_C CRYSError_t CRYS_HASH_Finish( CRYS_HASHUserContext_t  *ContextID_ptr ,
					CRYS_HASH_Result_t       HashResultBuff );


/************************************************************************************************/
/**
 * @brief This function is a utility function that frees the context if the operation has failed.
 *
 *        The function executes the following major steps:
 *
 *        1. Checks the validity of all of the inputs of the function. 
 *           If one of the received parameters is not valid it shall return an error.
 *           The main checks that are run over the received parameters:
 *           - verifying that the context pointer is not NULL (*ContextID_ptr). 
 *        2. Clears the user's context.
 *        3. Exits the handler with the OK code.
 *      
 *
 * @param[in] ContextID_ptr - A pointer to the HASH context buffer allocated by the user 
 *                       that is used for the HASH machine operation. This should be 
 *                       the same context that was used on the previous call of this session.
 *
 *
 * @return CRYSError_t - On success the value CRYS_OK is returned, 
 *                        and on failure a value from crys_error.h
 */


CIMPORT_C CRYSError_t  CRYS_HASH_Free(CRYS_HASHUserContext_t  *ContextID_ptr );


/************************************************************************************************/
/**
 * \brief This function provides a HASH function for processing one buffer of data.
 *
 * The function allocates an internal HASH Context, and initializes the 
 * HASH Context with the cryptographic attributes that are needed for 
 * the HASH block operation (initialize H's value for the HASH algorithm).
 * Next the function loads the engine with the initializing values, 
 * and then processes the data block, calculating the hash.
 * Finally, the function return the message digest of the data buffer .
 *
 * @param[in] OperationMode - The operation mode: 
 *      CRYS_HASH_SHA1_mode, CRYS_HASH_SHA224_mode, CRYS_HASH_SHA256_mode,
 *	CRYS_HASH_SHA384_mode, CRYS_HASH_SHA512_mode, CRYS_HASH_MD5_mode
 * 
 * @param[in] DataIn_ptr - The pointer to the buffer of the input data to the HASH. The pointer does 
 *                         not need to be aligned. On CSI input mode the pointer must be equal to
 *                         value (0xFFFFFFFC | DataInAlignment). 
 * 
 * @param[in] DataInSize - The size of the data to be hashed in bytes. On CSI data transfer mode the size must  
 *                         multiple of HASH_BLOCK_SIZE for used HASH mode.
 *
 * @retval HashResultBuff - A pointer to the target buffer where the 
 *                      HASE result stored in the context is loaded to.
 *
 * @return CRYSError_t -  On success the function returns the value CRYS_OK, 
 *				and on failuer a non-ZERO error.
 *
 */

CIMPORT_C CRYSError_t CRYS_HASH  ( CRYS_HASH_OperationMode_t  OperationMode,
				   uint8_t                   *DataIn_ptr,
				   uint32_t                   DataSize,
				   CRYS_HASH_Result_t         HashResultBuff );



#ifdef __cplusplus
}
#endif

#endif
