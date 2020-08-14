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

 
 #ifndef CRYS_ECPKI_ECDSA_H
#define CRYS_ECPKI_ECDSA_H
/*
 *  Object name     :  CRYS__ECPKI_ECDSA.h
   *  State           :  %state%
   *  Creation date   :  02.02.2006
   *  Last modified   :  %modify_time%
   */
/** @file
 * \brief Defines the APIs that support the ECDSA functions. 
 *
 * \version CRYS__ECPKI_ECDSA.h#1:hinc:1
 * \author R.Levin
 */

#include "crys_error.h"

#include "crys_ecpki_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

 
/**************************************************************************
 *	              CRYS_ECDSA_Sign_Init function
 **************************************************************************/
/**
   \brief    
   The CRYS_ECDSA_Sign_Init functions user shall call first to perform the 
   EC DSA Signing operation.

   The function performs the following steps:
   -# Validates all the inputs of the function. If one of the received 
      parameters is not valid, the function returns an error. 
   -# Decrypts the received context to the working context after capturing 
      the working context by calling the CRYS_CCM_GetContext() function.
   -# Initializes the working context and other variables and structures. 
   -# Calls the CRYS_HASH_Init() function.
   -# Calls the CRYS_CCM_EncryptAndReleaseContext() function to encrypt 
      the information in the working context, store it in the user's 
      received context, and then release the working context.
   -# Exits the handler with the OK code.

   This function does not do ECDSA cryptographic processing. Rather, it
   prepares a context that is used by the Update() and Finish() functions.
   
   NOTE: Using of HASH functions with HASH size great, than EC modulus size, is not recommended!
   
   
   @param[in,out] SignUserContext_ptr A pointer to the user buffer for signing data.
   @param[in] 	  SignerPrivKey_ptr   A pointer to the private key that will be used to 
                                      sign the data.
   @param[in]     HashMode            Defines the hash mode used for DSA.

   @return <b>CRYSError_t</b>: <br> 
			 CRYS_OK<br>
			 CRYS_ECDSA_SIGN_INVALID_USER_CONTEXT_PTR_ERROR
			 CRYS_ECDSA_SIGN_INVALID_USER_PRIV_KEY_PTR_ERROR
			 CRYS_ECDSA_SIGN_USER_PRIV_KEY_VALIDATION_TAG_ERROR
			 CRYS_ECDSA_SIGN_INVALID_DOMAIN_ID_ERROR
			 CRYS_ECDSA_SIGN_ILLEGAL_HASH_OP_MODE_ERROR		 
*/
CIMPORT_C CRYSError_t CRYS_ECDSA_SignInit( 
	                             CRYS_ECDSA_SignUserContext_t  *SignUserContext_ptr, /*in/out*/
								 CRYS_ECPKI_UserPrivKey_t      *SignerPrivKey_ptr,   /*in*/
								 CRYS_ECPKI_HASH_OpMode_t       HashMode             /*in*/ );




/**************************************************************************
 *	              CRYS_ECDSA_Sign_Update function
 **************************************************************************/
/**
   @brief  Performs a hash  operation on data allocated by the user before finally signing it.
  
 In case the user divides signing data by block, the user must call the Update() function 
 continuously for some period, until processing of the entire data block is complete.
 
 NOTE: Using of HASH functions with HASH size great, than EC modulus size, is not recommended!

   @param[in,out] SignUserContext_ptr A pointer to the user buffer for signing the database.
   @param[in] 	  MessageDataIn_ptr   Message data for calculating Hash.   	
   @param[in]     DataInSize          The size of the message data block, in bytes.
   
   @return <b>CRYSError_t</b>: <br> 
			 CRYS_OK<br>
           		 CRYS_ECDSA_SIGN_INVALID_USER_CONTEXT_PTR_ERROR
                         CRYS_ECDSA_SIGN_USER_CONTEXT_VALIDATION_TAG_ERROR
			 CRYS_ECDSA_SIGN_INVALID_MESSAGE_DATA_IN_PTR_ERROR 
			 CRYS_ECDSA_SIGN_INVALID_MESSAGE_DATA_IN_SIZE_ERROR
			 CRYS_ECDSA_SIGN_ILLEGAL_HASH_OP_MODE_ERROR
		 										 
 */
CIMPORT_C CRYSError_t CRYS_ECDSA_SignUpdate( CRYS_ECDSA_SignUserContext_t  *SignUserContext_ptr,  /*in/out*/
								             uint8_t                     *MessageDataIn_ptr,    /* in */ 
							  	             uint32_t                      DataInSize            /* in */ );


/**************************************************************************
 *	              _DX_ECDSA_Sign_Finish function
 **************************************************************************/
/**
   @brief  Performs initialization of variables and structures, calls the hash function  
           for the last block of data (if necessary) and then calculates digital 
		   signature according to the EC DSA algorithm.
   	   
   	   NOTE: Using of HASH functions with HASH size great, than EC modulus size, 
	         is not recommended!

   @param[in] SignUserContext_ptr      A pointer to the user buffer for signing database.
   @param[in] SignatureOut_ptr         A pointer to a buffer for output of signature.
   @param[in,out] SignatureOutSize_ptr A pointer to the size of a user passed buffer for signature (in)
                                       and size of actual signature (out). The size of buffer 
									   must be not less than 2*OrderSizeInBytes.
   @param[out] IsEphemerKeyInternal    A parameter defining whether the ephemeral key 
                                       is internal or external (1 or 0).
   @param[out] EphemerKeyData_ptr      A pointer to external ephemeral key data. The buffer must
                                       to contain the following data placed continuously:
									    - ephemeral private key  - 4*(ModSizeInWords + 1 ) bytes,
										- ephemeral public key X - 4*ModSizeInWords bytes,
										- ephemeral public key Y - 4*ModSizeInWords bytes.
   @return <b>CRYSError_t</b>: <br> 
			 CRYS_OK<br>
                         CRYS_ECDSA_SIGN_INVALID_USER_CONTEXT_PTR_ERROR <br>
			 CRYS_ECDSA_SIGN_USER_CONTEXT_VALIDATION_TAG_ERROR <br>
			 CRYS_ECDSA_SIGN_INVALID_SIGNATURE_OUT_PTR_ERROR <br>
			 CRYS_ECDSA_SIGN_ILLEGAL_HASH_OP_MODE_ERROR <br>
			 CRYS_ECDSA_SIGN_INVALID_SIGNATURE_OUT_SIZE_PTR_ERROR <br>
			 CRYS_ECDSA_SIGN_INVALID_SIGNATURE_OUT_SIZE_ERROR <br>
			 CRYS_ECDSA_SIGN_INVALID_DOMAIN_ID_ERROR <br>
			 CRYS_ECDSA_SIGN_INVALID_IS_EPHEMER_KEY_INTERNAL_ERROR <br>
			 CRYS_ECDSA_SIGN_INVALID_EPHEMERAL_KEY_PTR_ERROR <br>
**/
CIMPORT_C  CRYSError_t _DX_ECDSA_SignFinish( 
	                        CRYS_ECDSA_SignUserContext_t   *SignUserContext_ptr, /*in*/ 
							uint8_t                      *SignatureOut_ptr,    /*out*/
							uint32_t                     *SignatureOutSize_ptr,/*in/out*/
                            int8_t                       IsEphemerKeyInternal, /*in*/
                            uint32_t                     *EphemerKeyData_ptr   /*in*/ );

/**************************************************************************
 *	              CRYS_ECDSA_Sign_Finish function
 **************************************************************************/
/**
   @brief  The macro definition for calling the
           _DX_ECDSA_SignFinish function with internal generation of ephemeral keys.
           
           NOTE: Using of HASH functions with HASH size great, than EC modulus size, is not recommended!

	The macro calls the function with the following arguments as constant: 
	IsEphemerKeyInternal = 1 and EphemerKeyData_ptr = DX_NULL.
*/
#define CRYS_ECDSA_SignFinish(SignUserContext_ptr, SignatureOut_ptr, SignatureOutSize_ptr) \
	_DX_ECDSA_SignFinish(SignUserContext_ptr, SignatureOut_ptr, SignatureOutSize_ptr, 1, DX_NULL)


/**************************************************************************
 *	              CRYS_ECDSA_Sign - integrated function
 **************************************************************************/
/**
   @brief  Performs all of the ECDSA signing operations simultaneously.
           This function simply calls the Init, Update and Finish functions continuously.  
   	       This function's prototype is similar to the prototypes of the called functions 
   	       and includes all of their input and output arguments.
   
   NOTE: Using of HASH functions with HASH size great, than EC modulus size, is not recommended!

   @param[in,out] SignUserContext_ptr - A pointer to the user buffer for signing database.
   @param[in]     SignerPrivKey_ptr   - A pointer to a user private key structure.    				
   @param[in]     HashMode            - The enumerator variable defines hash function to be used.                         
   @param[in] 	  MessageDataIn_ptr   - A message data for calculation of hash.   			
   @param[in]     MessageSizeInBytes  - A size of block of message data in bytes. 
   @param[in]     SignatureOut_ptr    - A pointer to a buffer for output of signature.                         
   @param[in,out] SignatureOutSize_ptr- A pointer to the size of user passed buffer for signature (in)
                                        and size of actual signature (out). The size of buffer 
									    must be not less than 2*OrderSizeInBytes.
   @return <b>CRYSError_t</b>: <br> 
			 CRYS_OK<br>
			 CRYS_ECDSA_SIGN_INVALID_USER_CONTEXT_PTR_ERROR<br>
			 CRYS_ECDSA_SIGN_USER_CONTEXT_VALIDATION_TAG_ERROR<br>
 		     CRYS_ECDSA_SIGN_INVALID_USER_PRIV_KEY_PTR_ERROR<br>
			 CRYS_ECDSA_SIGN_USER_PRIV_KEY_VALIDATION_TAG_ERROR<br>
			 CRYS_ECDSA_SIGN_ILLEGAL_HASH_OP_MODE_ERROR <br>                         
			 CRYS_ECDSA_SIGN_INVALID_MESSAGE_DATA_IN_PTR_ERROR<br> 
			 CRYS_ECDSA_SIGN_INVALID_MESSAGE_DATA_IN_SIZE_ERROR<br>
			 CRYS_ECDSA_SIGN_INVALID_SIGNATURE_OUT_PTR_ERROR<br>
			 CRYS_ECDSA_SIGN_INVALID_SIGNATURE_OUT_SIZE_PTR_ERROR<br>
             CRYS_ECDSA_SIGN_INVALID_SIGNATURE_OUT_SIZE_ERROR<br>			 
			 CRYS_ECDSA_SIGN_INVALID_DOMAIN_ID_ERROR <br>			 
**/
CIMPORT_C CRYSError_t CRYS_ECDSA_Sign(
					CRYS_ECDSA_SignUserContext_t  *SignUserContext_ptr,     /*in/out*/
					CRYS_ECPKI_UserPrivKey_t      *SignerPrivKey_ptr,       /*in*/
					CRYS_ECPKI_HASH_OpMode_t       HashMode,                /*in*/
					uint8_t                     *MessageDataIn_ptr,       /*in*/ 
					uint32_t                     MessageSizeInBytes,      /*in*/
					uint8_t                     *SignatureOut_ptr,        /*out*/ 
					uint32_t                    *SignatureOutSize_ptr     /*in*/); 



/**************************************************************************
 *	              CRYS_ECDSA_VerifyInit  function
 **************************************************************************/
/**
   @brief  Prepares a context that is used by the Update and Finish functions
           but does not perform elliptic curve cryptographic processing

		    The function:
			- Receives and decrypts user data (working context). 
			- Checks input parameters of  ECDSA Signing primitive.
			- Calls hash init function.
			- Initializes variables and structures for calling next functions.
			- Encrypts and releases working context.
			
			NOTE: Using of HASH functions with HASH size great, than EC modulus size, 
			is not recommended!

   @param[in,out] VerifyUserContext_ptr - A pointer to the user buffer for verifying database.
   @param[in] SignerPublKey_ptr - A pointer to a Signer public key structure.
   @param[in] HashMode - The enumerator variable defines the hash function to be used.
   
   @return <b>CRYSError_t</b>: <br> 
			 CRYS_OK<br>
                         CRYS_ECDSA_VERIFY_INVALID_USER_CONTEXT_PTR_ERROR <br>
			 CRYS_ECDSA_VERIFY_INVALID_SIGNER_PUBL_KEY_PTR_ERROR <br>
                         CRYS_ECDSA_VERIFY_SIGNER_PUBL_KEY_VALIDATION_TAG_ERROR <br>
                         CRYS_ECDSA_VERIFY_INVALID_DOMAIN_ID_ERROR <br>
			 CRYS_ECDSA_VERIFY_ILLEGAL_HASH_OP_MODE_ERROR <br>
**/
CIMPORT_C CRYSError_t CRYS_ECDSA_VerifyInit( CRYS_ECDSA_VerifyUserContext_t  *VerifyUserContext_ptr, /*in/out*/
								   CRYS_ECPKI_UserPublKey_t 	   *SignerPublKey_ptr,     /*in*/
								   CRYS_ECPKI_HASH_OpMode_t         HashMode               /*in*/ );

/**************************************************************************
 *	              CRYS_ECDSA_VerifyUpdate function
 **************************************************************************/
/**
   @brief  Performs a hash  operation on data allocated by the user 
           before finally signing it.
  
   	   In case user divides signing data by block, he must call the Update function 
   	   continuously a number of times until processing of the entire data block is complete.
   	   
       NOTE: Using of HASH functions with HASH size great, than EC modulus size, 
             is not recommended!

   @param[in,out] VerifyUserContext_ptr A pointer to the user buffer for signing database.
   @param[in] 	  MessageDataIn_ptr     Message data for calculation of hash.
   @param[in]     DataInSize            The size of the message data block, in bytes.
   
   @return <b>CRYSError_t</b>: <br> 
			 CRYS_OK<br>
                         CRYS_ECDSA_VERIFY_INVALID_USER_CONTEXT_PTR_ERROR <br>
			 CRYS_ECDSA_VERIFY_USER_CONTEXT_VALIDATION_TAG_ERROR <br>
			 CRYS_ECDSA_VERIFY_INVALID_MESSAGE_DATA_IN_PTR_ERROR <br>
			 CRYS_ECDSA_VERIFY_INVALID_MESSAGE_DATA_IN_SIZE_ERROR <br>
			 CRYS_ECDSA_VERIFY_ILLEGAL_HASH_OP_MODE_ERROR <br>
 **/
CIMPORT_C CRYSError_t CRYS_ECDSA_VerifyUpdate( CRYS_ECDSA_VerifyUserContext_t *VerifyUserContext_ptr, /*in/out*/
								     uint8_t                      *MessageDataIn_ptr,     /* in */ 
							  	     uint32_t                      DataInSize             /* in */ );


/**************************************************************************
 *	              CRYS_ECDSA_VerifyFinish function
 **************************************************************************/
/**
   @brief  Performs initialization of variables and structures, 
           calls the hash function for the last block of data (if necessary),  
   		   than calls LLF_ECDSA_VerifyCalcCall function for verifying signature  
		   according to EC DSA algorithm.

       NOTE: Using of HASH functions with HASH size great, than EC modulus size, 
			 is not recommended!	   

   @param[in] VerifyUserContext_ptr - A pointer to the user buffer for verifying the database.
   @param[in] SignatureIn_ptr       - A pointer to a buffer for the signature to be compared
   @param[in] SignatureSizeBytes    - The size of a user passed signature (must be 2*OrderSizeInBytes).

   @return <b>CRYSError_t</b>: <br>
              CRYS_OK <br>
			  CRYS_ECDSA_VERIFY_INVALID_USER_CONTEXT_PTR_ERROR <br>
			  CRYS_ECDSA_VERIFY_USER_CONTEXT_VALIDATION_TAG_ERROR <br>
			  CRYS_ECDSA_VERIFY_INVALID_SIGNATURE_IN_PTR_ERROR <br>
			  CRYS_ECDSA_VERIFY_ILLEGAL_HASH_OP_MODE_ERROR <br>
			  CRYS_ECDSA_VERIFY_INVALID_SIGNATURE_SIZE_ERROR <br>
			  CRYS_ECDSA_VERIFY_INVALID_DOMAIN_ID_ERROR <br>
			  CRYS_ECDSA_VERIFY_INCONSISTENT_VERIFY_ERROR <br>
**/
CIMPORT_C CRYSError_t CRYS_ECDSA_VerifyFinish( 
	                         CRYS_ECDSA_VerifyUserContext_t *VerifyUserContext_ptr, /*in*/ 
							 uint8_t                      *SignatureIn_ptr,       /*in*/
							 uint32_t                      SignatureSizeBytes    /*in*/ );


/**************************************************************************
 *	              CRYS_ECDSA_Verify integrated function
 **************************************************************************/
/**
   @brief  Performs all ECDSA verifying operations simultaneously. 
		   
	   This function simply calls the Init, Update and Finish functions continuously.  
   	   This function's prototype is similar to the prototypes of the called functions 
   	   and includes all of their input and output arguments.
   	   
       NOTE: Using of HASH functions with HASH size great, than EC modulus size, 
			 is not recommended!
   	   

   @param[in]  VerifyUserContext_ptr - A pointer to the user buffer for signing the database.
   @param[in]  UserPublKey_ptr       - A pointer to a user public key structure.
   @param[in]  HashMode              - The enumerator variable defines the hash function to be used.
   @param[in]  MessageDataIn_ptr     - Message data for calculating hash.   			
   @param[in]  MessageSizeInBytes    - Size of block of message data in bytes.       
   @param[in]  SignatureIn_ptr       - A pointer to a buffer for output of signature. 
   @param[in]  SignatureSizeBytes    - Size of signature, in bytes (must be 2*OrderSizeInBytes).
                                       
   @return <b>CRYSError_t</b>: <br> 
              CRYS_OK <br>
              CRYS_ECDSA_VERIFY_INVALID_USER_CONTEXT_PTR_ERROR <br> 
              CRYS_ECDSA_VERIFY_USER_CONTEXT_VALIDATION_TAG_ERROR <br>
              CRYS_ECDSA_VERIFY_INVALID_DOMAIN_ID_ERROR <br>
              CRYS_ECDSA_VERIFY_INVALID_SIGNER_PUBL_KEY_PTR_ERROR <br>
              CRYS_ECDSA_VERIFY_SIGNER_PUBL_KEY_VALIDATION_TAG_ERROR <br>
              CRYS_ECDSA_VERIFY_ILLEGAL_HASH_OP_MODE_ERROR <br>                
              CRYS_ECDSA_VERIFY_INVALID_MESSAGE_DATA_IN_PTR_ERROR <br>
              CRYS_ECDSA_VERIFY_INVALID_MESSAGE_DATA_IN_SIZE_ERROR <br>						                        
              CRYS_ECDSA_VERIFY_INVALID_SIGNATURE_IN_PTR_ERROR <br>						 
              CRYS_ECDSA_VERIFY_INVALID_SIGNATURE_SIZE_ERROR <br>
			  CRYS_ECDSA_VERIFY_INCONSISTENT_VERIFY_ERROR <br>
**/
CIMPORT_C CRYSError_t CRYS_ECDSA_Verify ( CRYS_ECDSA_VerifyUserContext_t *VerifyUserContext_ptr, /*in/out*/
									   	  CRYS_ECPKI_UserPublKey_t       *UserPublKey_ptr,        /*in*/
									      CRYS_ECPKI_HASH_OpMode_t        HashMode,               /*in*/
										  uint8_t                      *SignatureIn_ptr,        /*in*/
										  uint32_t                      SignatureSizeBytes,     /*in*/
										  uint8_t                      *MessageDataIn_ptr,      /*in*/ 
										  uint32_t                      MessageSizeInBytes      /*in*/ );


/**********************************************************************************************************/


#ifdef __cplusplus
}
#endif			

#endif
