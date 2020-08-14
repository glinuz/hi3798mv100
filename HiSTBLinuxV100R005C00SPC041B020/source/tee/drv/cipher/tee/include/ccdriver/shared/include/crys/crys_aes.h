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

/** @file
 *  \brief This file contains all of the enums and definitions that are used for the 
 *         CRYS AES APIs, as well as the APIs themselves. 
 *
 */
#ifndef CRYS_AES_H
#define CRYS_AES_H

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

#if defined(DX_CC_SEP)
#define CRYS_AES_USER_CTX_SIZE_IN_WORDS 32
#elif defined(DX_CC_TEE)
/*Inorder to allow contiguous context the user context is doubled + 3 words for managment */
#define CRYS_AES_USER_CTX_SIZE_IN_WORDS 131
#else
#define CRYS_AES_USER_CTX_SIZE_IN_WORDS 312
#endif

/* The AES block size in words and in bytes */
#define CRYS_AES_BLOCK_SIZE_IN_WORDS 4
#define CRYS_AES_BLOCK_SIZE_IN_BYTES  (CRYS_AES_BLOCK_SIZE_IN_WORDS * sizeof(uint32_t))

/* The size of the IV or counter buffer */
#define CRYS_AES_IV_COUNTER_SIZE_IN_WORDS   CRYS_AES_BLOCK_SIZE_IN_WORDS
#define CRYS_AES_IV_COUNTER_SIZE_IN_BYTES  (CRYS_AES_IV_COUNTER_SIZE_IN_WORDS * sizeof(uint32_t))

/* The maximum size of the AES KEY in words and bytes */
#define CRYS_AES_KEY_MAX_SIZE_IN_WORDS 16
#define CRYS_AES_KEY_MAX_SIZE_IN_BYTES (CRYS_AES_KEY_MAX_SIZE_IN_WORDS * sizeof(uint32_t))

/* The AES_WRAP minimum data size in bytes  (one 64-bits block)  */
#define CRYS_AES_WRAP_DATA_MIN_SIZE_IN_BYTES   8

/* The AES_WRAP maximum data size in bytes:   */
#define CRYS_AES_WRAP_DATA_MAX_SIZE_IN_BYTES   512

/* The CRYS_AES_WRAP block size in bytes and in words */
#define CRYS_AES_WRAP_BLOCK_SIZE_IN_BYTES 8
#define CRYS_AES_WRAP_BLOCK_SIZE_IN_WORDS  (CRYS_AES_WRAP_BLOCK_SIZE_IN_BYTES / sizeof(uint32_t))

#define CRYS_AES_WRAP_STEPS   6 //according to rfc3394
/************************ Enums ********************************/

/* Enum defining the user's key size argument */
typedef enum {
	CRYS_AES_Key128BitSize   = 0,
	CRYS_AES_Key192BitSize   = 1,
	CRYS_AES_Key256BitSize   = 2,
	CRYS_AES_Key512BitSize   = 3,

	CRYS_AES_KeySizeNumOfOptions,

	CRYS_AES_KeySizeLast    = 0x7FFFFFFF,

}CRYS_AES_KeySize_t;  

/* Enum defining the Encrypt or Decrypt operation mode */
typedef enum {
	CRYS_AES_Encrypt = 0,
	CRYS_AES_Decrypt = 1,

	CRYS_AES_EncryptNumOfOptions,

	CRYS_AES_EncryptModeLast= 0x7FFFFFFF,

}CRYS_AES_EncryptMode_t;

/* Enum defining the AES operation mode */
typedef enum {
	CRYS_AES_ECB_mode          = 0,
	CRYS_AES_CBC_mode          = 1,
	CRYS_AES_MAC_mode          = 2,
	CRYS_AES_CTR_mode          = 3,
	CRYS_AES_XCBC_MAC_mode     = 4,
	CRYS_AES_CMAC_mode         = 5,
	CRYS_AES_XTS_mode          = 6,
	CRYS_AES_CBC_CTS_mode      = 7,
	CRYS_AES_OFB_mode          = 8,
	CRYS_AES_CCM_mode          = 9,

	CRYS_AES_NumOfModes,

	CRYS_AES_OperationModeLast= 0x7FFFFFFF,

}CRYS_AES_OperationMode_t;

/************************ Typedefs  ****************************/

/* Defines the IV counter buffer  - 16 bytes array */
typedef uint8_t CRYS_AES_IvCounter_t[CRYS_AES_IV_COUNTER_SIZE_IN_BYTES];

/* Define the XTS Tweak value type - 16 bytes array */
typedef  CRYS_AES_IvCounter_t  CRYS_AES_XTS_Tweak_t;

/* Defines the AES key buffer */
typedef uint8_t CRYS_AES_Key_t[CRYS_AES_KEY_MAX_SIZE_IN_BYTES];

/* Defines the AES MAC result maximum size buffer */
typedef uint8_t CRYS_AES_MAX_MAC_RESULT_t[CRYS_AES_IV_COUNTER_SIZE_IN_BYTES];

/************************ context Structs  ******************************/

/* The user's context prototype - the argument type that will be passed by the user 
   to the APIs called by him */
typedef struct CRYS_AESUserContext_t {
	/* Allocated buffer must be double the size of actual context 
	 * + 1 word for offset management */
	uint32_t buff[CRYS_AES_USER_CTX_SIZE_IN_WORDS]; 
}CRYS_AESUserContext_t;

/************************ Public Variables **********************/


/************************ Public Functions **********************/

/****************************************************************************************************/
/** 
 * @brief This function is used to initialize the AES machine or SW structures.
 *        To perform the AES operations this should be the first function called.
 *
 *
 * @param[in] ContextID_ptr - A pointer to the AES context buffer that is allocated by the user 
 *                            and is used for the AES machine operation.
 * @param[in] IVCounter_ptr - A buffer containing an initial value: IV, Counter or Tweak according 
 *                            to operation mode:
 *                            - on ECB, XCBC, CMAC mode this parameter is not used and may be NULL,
 *                            - on CBC and MAC modes it contains the IV value,
 *                            - on CTR and OFB modes it contains the init counter,
 *                            - on XTS mode it contains the initial tweak value - 128-bit consecutive number 
 *                              of data unit (in little endian).
 * @param[in] Key_ptr  -  A pointer to the user's key buffer.
 * @param[in] KeySize  -  An enum parameter, defines size of used key (128, 192, 256, 512 bits):
 *                        On XCBC mode allowed 128 bit size only, on XTS - 256 or 512 bit, on other modes <= 256 bit.
 * @param[in] EncryptDecryptFlag - A flag specifying whether the AES should perform an Encrypt operation (0) 
 *                                 or a Decrypt operation (1). In XCBC and CMAC modes it must be Encrypt.
 * @param[in] OperationMode - The operation mode: ECB, CBC, MAC, CTR, OFB, XCBC (PRF and 96), CMAC, XTS and CBC-CTS.
 *
 * @return CRYSError_t - On success the value CRYS_OK is returned, and on failure - a value from crys_aes_error.h
 */
CIMPORT_C CRYSError_t  CRYS_AES_Init(
				   CRYS_AESUserContext_t    *ContextID_ptr,
				   CRYS_AES_IvCounter_t     IVCounter_ptr,
				   CRYS_AES_Key_t           Key_ptr,
				   CRYS_AES_KeySize_t       KeySizeID,
				   CRYS_AES_EncryptMode_t   EncryptDecryptFlag,
				   CRYS_AES_OperationMode_t OperationMode);


/****************************************************************************************************/
/** 
 * @brief This function is used to operate a block of data on the SW or on AES machine.
 *        This function should be called after the appropriate CRYS AES init function 
 *        (according to used AES operation mode).
 *
 * @param[in] ContextID_ptr - A pointer to the AES context buffer allocated by the user that
 *                            is used for the AES machine operation. This should be the same context that was
 *                            used on the previous call of this session.
 *
 * @param[in] DataIn_ptr - A pointer to the buffer of the input data to the AES. The pointer does 
 *                         not need to be aligned.
 *
 * @param[in] DataInSize - A size of the input data must be multiple of 16 bytes, 
 *                         on all modes. Note last chunk (block) of data must be processed by 
 *                         CRYS_AES_Finish function but not by CRYS_AES_Block function;  
 *
 * @param[out] DataOut_ptr - A pointer to the buffer of the output data from the AES. The pointer  does not 
 *                             need to be aligned. In case of MAC modes it can be NULL 
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                       value MODULE_* CRYS_AES_error.h
 *
 */
CIMPORT_C CRYSError_t  CRYS_AES_Block( CRYS_AESUserContext_t   *ContextID_ptr,   
				       uint8_t               *DataIn_ptr,     
				       uint32_t               DataInSize,     
				       uint8_t               *DataOut_ptr ); 


/****************************************************************************************************/
/** 
 * @brief This function is used as finish operation on all AES modes.
 *
 *        The function must be called after AES_Block operations (or instead) for last chunck 
 *        of data with size > 0. 
 *
 *        The function performs all operations, including specific operations for last blocks of 
 *        data on some modes (XCBC, CMAC, MAC) and puts out the result. After all operations 
 *        the function cleans the secure sensitive data from context.  
 *        
 *
 * @param[in] ContextID_ptr - A pointer to the AES context buffer allocated by the user that
 *                            should be the same context that was used on the previous call
 *                            of this session.
 *
 * @param[in] DataIn_ptr    - A pointer to the buffer of the input data to the AES. The pointer does 
 *                            not need to be aligned. 
 *
 * @param[in] DataInSize    - A size of the input data must be:  DataInSize >= minimalSize, where: 
 *                            minimalSize =
 *                                  -  1 byte for CTR, OFB, XCBC, CMAC mode;
 *                                  - 16 bytes for ECB CBC MAC XTS modes. 
 *                                  - >16 bytes for CBC-CTS modes. 
 *
 * @param[out] DataOut_ptr  - A pointer to the output buffer. The pointer  does not need to be aligned. 
 *                            The size of the output buffer must be not less than:  
 *                                - 16 bytes for MAC, XCBC, CMAC modes;
 *                                - DataInSize for ECB,CBC,CTR,XTS,OFB modes. 
 *
 * @return CRYSError_t    - On success CRYS_OK is returned, on failure - a value defined in crys_aes_error.h.
 *
 */
CIMPORT_C CRYSError_t  CRYS_AES_Finish(
				      CRYS_AESUserContext_t   *ContextID_ptr,  
				      uint8_t               *DataIn_ptr,     
				      uint32_t               DataInSize,     
				      uint8_t               *DataOut_ptr );

/****************************************************************************************************/
/**
 * @brief This function is used to perform the AES operation in one integrated process.
 *
 *
 *        The input-output parameters of the function are the following:
 *
 * @param[in] IVCounter_ptr - A buffer containing an initial value: IV, Counter or Tweak according 
 *                            to operation mode:
 *                            - on ECB, XCBC, CMAC mode this parameter is not used and may be NULL,
 *                            - on CBC ,CBC-CTS and MAC modes it contains the IV value,
 *                            - on CTR and OFB modes it contains the init counter,
 *                            - on XTS mode it contains the initial tweak value - 128-bit consecutive number 
 *                              of data unit (in little endian).
 * @param[in] Key_ptr  -  A pointer to the user's key buffer (set to NULL for secret key).
 * @param[in] KeySize  -  An enum parameter, defines size of used key (128, 192, 256 bits).
 * @param[in] EncryptDecryptFlag - A flag specifying whether the AES should perform an Encrypt operation 
 *                                 or a Decrypt operation . In MAC modes it must be 0.
 * @param[in] OperationMode - The operation mode: ECB, CBC, MAC, CTR, XCBC (PRF and 96), CMAC, XTS, OFB, CBC-CTS.
 * @param[in] DataIn_ptr - A pointer to the buffer of the input data to the AES. The pointer does 
 *                         not need to be aligned.
 *
 * @param[in] DataInSize - The size of the input data, it must be:
 *                         - on ECB,CBC,MAC modes must be not 0 and must be a multiple of 16 bytes,
 *                         - on CTR, XCBC, CMAC, OFB and CBS-CTS modes must be not 0,
 *                         - on XTS mode must be or multiple of 16 bytes (not 0), or not less than 17 bytes.
 * @param[out] DataOut_ptr - A pointer to the buffer of the output data from the AES. The pointer  does not 
 *                             need to be aligned.
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a value defined in crys_aes_error.h
 *
 *     NOTES: 1. Temporarily it is not allowed, that both the Input and the Output simultaneously 
 *               were on CSI mode.
 *            2. Temporarily the CSI input or output are not allowed on XCBC, CMAC and XTS modes.    
 *
 */
CIMPORT_C CRYSError_t  CRYS_AES( CRYS_AES_IvCounter_t       IVCounter_ptr,       
				CRYS_AES_Key_t             Key_ptr,                                                             
				CRYS_AES_KeySize_t         KeySize,            
				CRYS_AES_EncryptMode_t     EncryptDecryptFlag, 
				CRYS_AES_OperationMode_t   OperationMode ,      
				uint8_t                  *DataIn_ptr,        
				uint32_t                 DataInSize,         
				uint8_t                  *DataOut_ptr );     


/**************************************************************************
 *	              CRYS_AES_Wrap function                                  *
 **************************************************************************/
/**
   @brief  The CRYS_AES_Wrap function implements the following algorithm 
	   (rfc3394, Sept. 2002):  
   
   Inputs:  Plaintext DataIn, n 64-bit values {P1, P2, ..., Pn}, 
	    KeyData, K (the KEK).                
   Outputs: Ciphertext, WrapDataOut (n+1) 64-bit values {C0, C1, ..., Cn}.
  

  @param[in]  DataIn_ptr - A pointer to plain text data to be wrapped
			    NOTE: Overlapping between the data input and data output buffer
				  is not allowed, except the inplace case that is legal . 			
   @param[in]  DataInLen  - Length of data in bytes. DataLen must be multiple of 
			    8 bytes and  must be in range [8, 512].
   @param[in]  KeyData	  - A pointer to  key data (key encryption key - KEK). 
   @param[in]  KeySize	  - Enumerator variable, defines length of key.
   @param[in]  Reserved   - Reserved param, should not be used.
   @param[out] WrapDataOut_ptr -	A pointer to buffer for output of wrapped data.
   @param[in/out] WrapDataLen_ptr - A pointer to a buffer for input of size of 
				    user passed buffer and for output actual 
				    size of unwrapped data in bytes. Buffer size must 
				    be not less than DataLen+CRYS_AES_WRAP_BLOCK_SIZE_IN_BYTES.                         
   @return CRYSError_t - CRYS_OK, or error message                         
			 CRYS_AES_WRAP_ILLEGAL_DATA_PTR_ERROR            
			 CRYS_AES_WRAP_DATA_LENGTH_ERROR                 
			 CRYS_AES_WRAP_ILLEGAL_KEY_PTR_ERROR             
			 CRYS_AES_WRAP_KEY_LENGTH_ERROR                  
			 CRYS_AES_WRAP_ILLEGAL_WRAP_DATA_PTR_ERROR      
			 CRYS_AES_WRAP_ILLEGAL_WRAP_DATA_LEN_PTR_ERROR  
			 CRYS_AES_WRAP_ILLEGAL_WRAP_DATA_LENGTH_ERROR
						 CRYS_AES_WRAP_DATA_OUT_DATA_IN_OVERLAP_ERROR 
						 CRYS_AES_WRAP_IS_SECRET_KEY_FLAG_ILLEGAL_ERROR

     NOTE:  On error exiting from function the output buffer may be zeroed by the function.
*/
CIMPORT_C  CRYSError_t CRYS_AES_Wrap (
				     uint8_t            *DataIn_ptr,	    /*in*/   
				     uint32_t            DataInLen,	    /*in*/
				     CRYS_AES_Key_t        KeyData,	    /*in*/
				     CRYS_AES_KeySize_t    KeySize,	    /*in*/
				     uint8_t            *WrapDataOut_ptr, /*out*/
				     uint32_t           *WrapDataLen_ptr  /*in/out*/ ); 



/**************************************************************************
 *	              CRYS_AES_Uwnrap function                                *
 **************************************************************************/
/**
   @brief  The CRYS_AES_Unwrap function performs inverse AES_Wrap transformation 
	   and implements the following algorithm (rfc3394, Sept. 2002):  

   Inputs:  Ciphertext, (n+1) 64-bit values {C0, C1, ..., Cn}, and
	    K  - key (the KEK).
  
   Outputs: Plaintext, n 64-bit values {P1, P2, ..., Pn}.
   
		       
   @param[in]  WrapDataIn_ptr - A pointer to wrapped data to be unwrapped 
				NOTE: Overlapping between the data input and data output buffer
				      is not allowed, except the inplace case that is legal . 			
   @param[in]  WrapDataInLen  - Length of wrapped data in bytes. DataLen must be multiple of 
				8 bytes and  must be in range [16, 512+8].
   @param[in]  KeyData        - A pointer to  key data (key encryption key - KEK). 
   @param[in]  KeySize        - Enumerator variable, defines length of key.
   @param[out] DataOut_ptr     - A pointer to buffer for output of unwrapped data.
   @param[in/out]  DataOutLen_ptr - A pointer to a buffer for input of size of user passed 
			      buffer and for output of actual size of unwrapped data in bytes.
			      DataOutLen must be multiple of 8 bytes and must be not less
			      than WrapDataInLen - CRYS_AES_WRAP_BLOCK_SIZE_IN_BYTES.
			   
   @return CRYSError_t - CRYS_OK, or error message 
			 CRYS_AES_UNWRAP_WRAP_DATA_LENGTH_ERROR          
			 CRYS_AES_UNWRAP_ILLEGAL_KEY_PTR_ERROR           
			 CRYS_AES_UNWRAP_KEY_LEN_ERROR                   
			 CRYS_AES_UNWRAP_ILLEGAL_DATA_PTR_ERROR          
			 CRYS_AES_UNWRAP_ILLEGAL_DATA_LEN_PTR_ERROR      
			 CRYS_AES_UNWRAP_ILLEGAL_DATA_LENGTH_ERROR       
			 CRYS_AES_UNWRAP_FUNCTION_FAILED_ERROR 
						 CRYS_AES_UNWRAP_DATA_OUT_DATA_IN_OVERLAP_ERROR 
						 CRYS_AES_UNWRAP_IS_SECRET_KEY_FLAG_ILLEGAL_ERROR

    NOTE:  On error exiting from function the output buffer may be zeroed by the function.
			 
*/                               
CIMPORT_C  CRYSError_t CRYS_AES_Unwrap(
				      uint8_t            *WrapDataIn_ptr, /*in*/   
				      uint32_t            WrapDataInLen,    /*in*/
				      CRYS_AES_Key_t        KeyData,	    /*in*/
				      CRYS_AES_KeySize_t    KeySize,	    /*in*/ 
				      uint8_t            *DataOut_ptr,    /*out*/
				      uint32_t           *DataOutLen_ptr     /*in/out*/ );


/**************************************************************************
 *	              CRYS_AES_SetIv function                                *
 **************************************************************************/
/**
   @brief  The CRYS_AES_SetIv function puts a new initial vector into
   an existing context.

   Inputs:  New IV vector
  
   Outputs: Result
   
   @param[in/out] ContextID_ptr - A pointer to the AES context buffer that is allocated by the user 
                                  and is used for the AES machine operation.
			   
   @return CRYSError_t - CRYS_OK, or error message 
			 CRYS_AES_INVALID_USER_CONTEXT_POINTER_ERROR          
			 
*/                               
CIMPORT_C  CRYSError_t CRYS_AES_SetIv(CRYS_AESUserContext_t   *ContextID_ptr,  
				                       uint8_t               *iv_ptr);



/**************************************************************************
 *	              CRYS_AES_GetIv function                                *
 **************************************************************************/
/**
   @brief  The CRYS_AES_GetIv function retrieves the initial vector from
   the context.

   Inputs:  IV vector buffer
  
   Outputs: Result
   
   @param[in/out] ContextID_ptr - A pointer to the AES context buffer that is allocated by the user 
                                  and is used for the AES machine operation.
			   
   @return CRYSError_t - CRYS_OK, or error message 
			 CRYS_AES_INVALID_USER_CONTEXT_POINTER_ERROR          
			 
*/                               
CIMPORT_C  CRYSError_t CRYS_AES_GetIv(CRYS_AESUserContext_t   *ContextID_ptr,  
				                       uint8_t               *iv_ptr);






#ifdef __cplusplus
}
#endif

#endif /* #ifndef CRYS_AES_H */ 





