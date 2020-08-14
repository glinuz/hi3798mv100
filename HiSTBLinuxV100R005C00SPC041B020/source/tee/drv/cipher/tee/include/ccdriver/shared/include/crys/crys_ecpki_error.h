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

 
 #ifndef CRYS_ECPKI_ERROR3_H
#define CRYS_ECPKI_ERROR3_H
  
/*
   *  Object % CRYS_ECPKI_error.h    : %
   *  State           :  %state%
   *  Creation date   :  05/12/ 2005
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This module containes the definitions of the CRYS ECPKI errors.
   *
   *  \version CRYS_ECPKI_error.h#1:incl:1
   *  \author R.Levin
  */
/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */
#include "crys_error.h"

#ifdef __cplusplus
extern "C"
{
#endif


/************************ Defines ******************************/

/* CRYS_ECPKI_MODULE_ERROR_BASE = 0x00F00800  */

/************************************************************************************************************
 * CRYS ECPKI KEY GENERATION MODULE ERRORS                                                                  *
 ************************************************************************************************************/
/* The CRYS ECPKI GEN KEY PAIR module errors */
#define CRYS_ECPKI_GEN_KEY_ILLEGAL_D0MAIN_ID_ERROR              (CRYS_ECPKI_MODULE_ERROR_BASE + 0x1UL)
#define CRYS_ECPKI_GEN_KEY_THIS_D0MAIN_IS_NOT_SUPPORTED_ERROR   (CRYS_ECPKI_MODULE_ERROR_BASE + 0x2UL)
#define CRYS_ECPKI_GEN_KEY_INVALID_PRIVATE_KEY_PTR_ERROR        (CRYS_ECPKI_MODULE_ERROR_BASE + 0x3UL)
#define CRYS_ECPKI_GEN_KEY_INVALID_PUBLIC_KEY_PTR_ERROR         (CRYS_ECPKI_MODULE_ERROR_BASE + 0x4UL)
#define CRYS_ECPKI_GEN_KEY_INVALID_TEMP_DATA_PTR_ERROR          (CRYS_ECPKI_MODULE_ERROR_BASE + 0x5UL)

/************************************************************************************************************
* The CRYS ECPKI BUILD KEYS MODULE ERRORS                                                                   *
*************************************************************************************************************/
#define CRYS_ECPKI_BUILD_KEY_ILLEGAL_DOMAIN_ID_ERROR            (CRYS_ECPKI_MODULE_ERROR_BASE + 0x08UL)
#define CRYS_ECPKI_BUILD_KEY_INVALID_PRIV_KEY_IN_PTR_ERROR      (CRYS_ECPKI_MODULE_ERROR_BASE + 0x09UL)
#define CRYS_ECPKI_BUILD_KEY_INVALID_USER_PRIV_KEY_PTR_ERROR    (CRYS_ECPKI_MODULE_ERROR_BASE + 0x0AUL)
#define CRYS_ECPKI_BUILD_KEY_INVALID_PRIV_KEY_SIZE_ERROR        (CRYS_ECPKI_MODULE_ERROR_BASE + 0x0BUL)
#define CRYS_ECPKI_BUILD_KEY_INVALID_PRIV_KEY_DATA_ERROR        (CRYS_ECPKI_MODULE_ERROR_BASE + 0x0CUL)
#define CRYS_ECPKI_BUILD_KEY_INVALID_PUBL_KEY_IN_PTR_ERROR      (CRYS_ECPKI_MODULE_ERROR_BASE + 0x0DUL)
#define CRYS_ECPKI_BUILD_KEY_INVALID_USER_PUBL_KEY_PTR_ERROR    (CRYS_ECPKI_MODULE_ERROR_BASE + 0x0EUL)
#define CRYS_ECPKI_BUILD_KEY_INVALID_PUBL_KEY_SIZE_ERROR        (CRYS_ECPKI_MODULE_ERROR_BASE + 0x0FUL)
#define CRYS_ECPKI_BUILD_KEY_INVALID_PUBL_KEY_DATA_ERROR        (CRYS_ECPKI_MODULE_ERROR_BASE + 0x10UL)
#define CRYS_ECPKI_BUILD_KEY_INVALID_COMPRESSION_MODE_ERROR     (CRYS_ECPKI_MODULE_ERROR_BASE + 0x11UL)
#define CRYS_ECPKI_BUILD_KEY_INVALID_CHECK_MODE_ERROR           (CRYS_ECPKI_MODULE_ERROR_BASE + 0x12UL)
#define CRYS_ECPKI_BUILD_KEY_INVALID_TEMP_BUFF_PTR_ERROR        (CRYS_ECPKI_MODULE_ERROR_BASE + 0x13UL)

/* The CRYS ECPKI EXPORT PUBLIC KEY MODULE ERRORS */
#define CRYS_ECPKI_EXPORT_PUBL_KEY_INVALID_USER_PUBL_KEY_PTR_ERROR      (CRYS_ECPKI_MODULE_ERROR_BASE + 0x14UL)
#define CRYS_ECPKI_EXPORT_PUBL_KEY_ILLEGAL_COMPRESSION_MODE_ERROR       (CRYS_ECPKI_MODULE_ERROR_BASE + 0x15UL)
#define CRYS_ECPKI_EXPORT_PUBL_KEY_INVALID_EXTERN_PUBL_KEY_PTR_ERROR    (CRYS_ECPKI_MODULE_ERROR_BASE + 0x16UL)
#define CRYS_ECPKI_EXPORT_PUBL_KEY_INVALID_PUBL_KEY_SIZE_PTR_ERROR      (CRYS_ECPKI_MODULE_ERROR_BASE + 0x17UL)
#define CRYS_ECPKI_EXPORT_PUBL_KEY_INVALID_PUBL_KEY_SIZE_ERROR          (CRYS_ECPKI_MODULE_ERROR_BASE + 0x18UL)
#define CRYS_ECPKI_EXPORT_PUBL_KEY_ILLEGAL_DOMAIN_ID_ERROR              (CRYS_ECPKI_MODULE_ERROR_BASE + 0x19UL)


/************************************************************************************************************
 * CRYS EC DIFFIE-HELLMAN MODULE ERRORS
*************************************************************************************************************/
/* The CRYS EC SVDP_DH Function errors */
#define CRYS_ECDH_SVDP_DH_INVALID_PARTNER_PUBL_KEY_PTR_ERROR	              (CRYS_ECPKI_MODULE_ERROR_BASE + 0x31UL)
#define CRYS_ECDH_SVDP_DH_PARTNER_PUBL_KEY_VALID_TAG_ERROR	              (CRYS_ECPKI_MODULE_ERROR_BASE + 0x32UL)
#define CRYS_ECDH_SVDP_DH_INVALID_USER_PRIV_KEY_PTR_ERROR		      (CRYS_ECPKI_MODULE_ERROR_BASE + 0x33UL)
#define CRYS_ECDH_SVDP_DH_USER_PRIV_KEY_VALID_TAG_ERROR		              (CRYS_ECPKI_MODULE_ERROR_BASE + 0x34UL)
#define CRYS_ECDH_SVDP_DH_INVALID_SHARED_SECRET_VALUE_PTR_ERROR	              (CRYS_ECPKI_MODULE_ERROR_BASE + 0x35UL)
#define CRYS_ECDH_SVDP_DH_INVALID_TEMP_DATA_PTR_ERROR                         (CRYS_ECPKI_MODULE_ERROR_BASE + 0x36UL)
#define CRYS_ECDH_SVDP_DH_INVALID_SHARED_SECRET_VALUE_SIZE_PTR_ERROR          (CRYS_ECPKI_MODULE_ERROR_BASE + 0x37UL)
#define CRYS_ECDH_SVDP_DH_INVALID_SHARED_SECRET_VALUE_SIZE_ERROR              (CRYS_ECPKI_MODULE_ERROR_BASE + 0x38UL)
#define CRYS_ECDH_SVDP_DH_ILLEGAL_DOMAIN_ID_ERROR                             (CRYS_ECPKI_MODULE_ERROR_BASE + 0x39UL)
#define CRYS_ECDH_SVDP_DH_NOT_CONCENT_PUBL_AND_PRIV_DOMAIN_ID_ERROR           (CRYS_ECPKI_MODULE_ERROR_BASE + 0x3AUL)


/************************************************************************************************************
 * CRYS ECDSA  MODULE ERRORS
 ************************************************************************************************************/
/* The CRYS ECDSA Signing  errors */
#define CRYS_ECDSA_SIGN_INVALID_DOMAIN_ID_ERROR			(CRYS_ECPKI_MODULE_ERROR_BASE + 0x50UL)
#define CRYS_ECDSA_SIGN_INVALID_USER_CONTEXT_PTR_ERROR 	        (CRYS_ECPKI_MODULE_ERROR_BASE + 0x51UL)
#define CRYS_ECDSA_SIGN_INVALID_USER_PRIV_KEY_PTR_ERROR         (CRYS_ECPKI_MODULE_ERROR_BASE + 0x52UL)
#define CRYS_ECDSA_SIGN_ILLEGAL_HASH_OP_MODE_ERROR              (CRYS_ECPKI_MODULE_ERROR_BASE + 0x53UL)
#define CRYS_ECDSA_SIGN_INVALID_MESSAGE_DATA_IN_PTR_ERROR       (CRYS_ECPKI_MODULE_ERROR_BASE + 0x54UL)
#define CRYS_ECDSA_SIGN_INVALID_MESSAGE_DATA_IN_SIZE_ERROR      (CRYS_ECPKI_MODULE_ERROR_BASE + 0x55UL)
#define CRYS_ECDSA_SIGN_USER_CONTEXT_VALIDATION_TAG_ERROR       (CRYS_ECPKI_MODULE_ERROR_BASE + 0x57UL)
#define CRYS_ECDSA_SIGN_USER_PRIV_KEY_VALIDATION_TAG_ERROR      (CRYS_ECPKI_MODULE_ERROR_BASE + 0x58UL)
#define CRYS_ECDSA_SIGN_INVALID_SIGNATURE_OUT_PTR_ERROR         (CRYS_ECPKI_MODULE_ERROR_BASE + 0x60UL)
#define CRYS_ECDSA_SIGN_INVALID_SIGNATURE_OUT_SIZE_PTR_ERROR    (CRYS_ECPKI_MODULE_ERROR_BASE + 0x61UL)
#define CRYS_ECDSA_SIGN_INVALID_SIGNATURE_OUT_SIZE_ERROR        (CRYS_ECPKI_MODULE_ERROR_BASE + 0x62UL)
#define CRYS_ECDSA_SIGN_INVALID_IS_EPHEMER_KEY_INTERNAL_ERROR   (CRYS_ECPKI_MODULE_ERROR_BASE + 0x63UL)
#define CRYS_ECDSA_SIGN_INVALID_EPHEMERAL_KEY_PTR_ERROR         (CRYS_ECPKI_MODULE_ERROR_BASE + 0x64UL)

/* The CRYS ECDSA Verifying  errors */
#define CRYS_ECDSA_VERIFY_INVALID_DOMAIN_ID_ERROR		(CRYS_ECPKI_MODULE_ERROR_BASE + 0x70UL)
#define CRYS_ECDSA_VERIFY_INVALID_USER_CONTEXT_PTR_ERROR	(CRYS_ECPKI_MODULE_ERROR_BASE + 0x71UL)
#define CRYS_ECDSA_VERIFY_INVALID_SIGNER_PUBL_KEY_PTR_ERROR     (CRYS_ECPKI_MODULE_ERROR_BASE + 0x72UL)
#define CRYS_ECDSA_VERIFY_ILLEGAL_HASH_OP_MODE_ERROR            (CRYS_ECPKI_MODULE_ERROR_BASE + 0x73UL)
#define CRYS_ECDSA_VERIFY_INVALID_SIGNATURE_IN_PTR_ERROR        (CRYS_ECPKI_MODULE_ERROR_BASE + 0x76UL)
#define CRYS_ECDSA_VERIFY_INVALID_SIGNATURE_SIZE_ERROR   	(CRYS_ECPKI_MODULE_ERROR_BASE + 0x77UL)
#define CRYS_ECDSA_VERIFY_INVALID_MESSAGE_DATA_IN_PTR_ERROR     (CRYS_ECPKI_MODULE_ERROR_BASE + 0x80UL)
#define CRYS_ECDSA_VERIFY_INVALID_MESSAGE_DATA_IN_SIZE_ERROR    (CRYS_ECPKI_MODULE_ERROR_BASE + 0x81UL)
#define CRYS_ECDSA_VERIFY_USER_CONTEXT_VALIDATION_TAG_ERROR     (CRYS_ECPKI_MODULE_ERROR_BASE + 0x82UL)
#define CRYS_ECDSA_VERIFY_SIGNER_PUBL_KEY_VALIDATION_TAG_ERROR  (CRYS_ECPKI_MODULE_ERROR_BASE + 0x83UL)	
#define CRYS_ECDSA_VERIFY_INCONSISTENT_VERIFY_ERROR             (CRYS_ECPKI_MODULE_ERROR_BASE + 0x84UL)	


/************************************************************************************************************
 * CRYS EC ELGAMAL  MODULE ERRORS
 ************************************************************************************************************/
/* The CRYS EC ELGAMAL Encrypt function errors */
#define CRYS_EC_ELGAMAL_ENCR_INVALID_DOMAIN_ID_ERROR		     (CRYS_ECPKI_MODULE_ERROR_BASE + 0xB1UL)
#define CRYS_EC_ELGAMAL_ENCR_INVALID_RECEIVER_PUBL_KEY_PTR_ERROR     (CRYS_ECPKI_MODULE_ERROR_BASE + 0xB2UL)
#define CRYS_EC_ELGAMAL_ENCR_INVALID_MESSAGE_DATA_IN_PTR_ERROR       (CRYS_ECPKI_MODULE_ERROR_BASE + 0xB3UL)
#define CRYS_EC_ELGAMAL_ENCR_INVALID_MESSAGE_DATA_IN_SIZE_ERROR      (CRYS_ECPKI_MODULE_ERROR_BASE + 0xB4UL)
#define CRYS_EC_ELGAMAL_ENCR_INVALID_ENCR_MESSAGE_OUT_PTR_ERROR      (CRYS_ECPKI_MODULE_ERROR_BASE + 0xB5UL)
#define CRYS_EC_ELGAMAL_ENCR_INVALID_ENCR_MESS_OUT_SIZE_PTR_ERROR    (CRYS_ECPKI_MODULE_ERROR_BASE + 0xB6UL)
#define CRYS_EC_ELGAMAL_ENCR_INVALID_ENCR_MESSAGE_OUT_SIZE_ERROR     (CRYS_ECPKI_MODULE_ERROR_BASE + 0xB7UL)
#define CRYS_EC_ELGAMAL_ENCR_INVALID_TEMP_DATA_BUFFER_PTR_ERROR      (CRYS_ECPKI_MODULE_ERROR_BASE + 0xB8UL)
#define CRYS_EC_ELGAMAL_RECEIVER_PUBL_KEY_VALIDATION_TAG_ERROR       (CRYS_ECPKI_MODULE_ERROR_BASE + 0xB9UL)
#define CRYS_EC_ELGAMAL_ENCR_INVALID_EPHEMERAL_KEY_DATA_ERROR        (CRYS_ECPKI_MODULE_ERROR_BASE + 0xBAUL)
#define CRYS_EC_ELGAMAL_ENCR_INVALID_EPHEMERAL_PRIV_KEY_SIZE_ERROR   (CRYS_ECPKI_MODULE_ERROR_BASE + 0xBBUL)

/* The CRYS EC ELGAMAL Decrypt function errors */
#define CRYS_EC_ELGAMAL_DECR_INVALID_DOMAIN_ID_ERROR		     (CRYS_ECPKI_MODULE_ERROR_BASE + 0xC1UL)
#define CRYS_EC_ELGAMAL_DECR_INVALID_RECEIVER_PRIV_KEY_PTR_ERROR     (CRYS_ECPKI_MODULE_ERROR_BASE + 0xC2UL)
#define CRYS_EC_ELGAMAL_DECR_INVALID_ENCR_MESSAGE_IN_PTR_ERROR       (CRYS_ECPKI_MODULE_ERROR_BASE + 0xC3UL)
#define CRYS_EC_ELGAMAL_DECR_INVALID_DECR_MESSAGE_OUT_PTR_ERROR      (CRYS_ECPKI_MODULE_ERROR_BASE + 0xC4UL)
#define CRYS_EC_ELGAMAL_DECR_INVALID_TEMP_DATA_BUFFER_PTR_ERROR      (CRYS_ECPKI_MODULE_ERROR_BASE + 0xC5UL)
#define CRYS_EC_ELGAMAL_RECEIVER_PRIV_KEY_VALIDATION_TAG_ERROR       (CRYS_ECPKI_MODULE_ERROR_BASE + 0xC6UL)
#define CRYS_EC_ELGAMAL_DECR_INVALID_ENCR_MESSAGE_IN_SIZE_ERROR      (CRYS_ECPKI_MODULE_ERROR_BASE + 0xC7UL)
#define CRYS_EC_ELGAMAL_DECR_INVALID_DECR_MESSAGE_OUT_SIZE_PTR_ERROR (CRYS_ECPKI_MODULE_ERROR_BASE + 0xC8UL)
#define CRYS_EC_ELGAMAL_DECR_INVALID_DECR_MESSAGE_OUT_SIZE_ERROR     (CRYS_ECPKI_MODULE_ERROR_BASE + 0xC9UL)
			 
	
#define CRYS_ECC_HOST_MSG_GENERAL_RPC_A_ERROR            	     (CRYS_ECPKI_MODULE_ERROR_BASE+0xD0UL)
#define CRYS_ECC_HOST_MSG_GENERAL_RPC_B_ERROR         		     (CRYS_ECPKI_MODULE_ERROR_BASE+0xD1UL)
#define CRYS_ECC_HOST_MSG_GENERAL_RPC_C_ERROR              	     (CRYS_ECPKI_MODULE_ERROR_BASE+0xD2UL)			 

#define CRYS_ECC_ILLEGAL_PARAMS_ACCORDING_TO_PRIV_ERROR      	     (CRYS_ECPKI_MODULE_ERROR_BASE + 0xD3UL)
/************************************************************************************************************
 *    CRYS ECPKI IS NOTUPPORTED ERROR                                                                       *
 ************************************************************************************************************/
#define CRYS_ECPKI_IS_NOT_SUPPORTED                         	     (CRYS_ECPKI_MODULE_ERROR_BASE + 0xFFUL)



/************************ Enums ********************************/

/************************ Typedefs  ****************************/

/************************ Structs  ******************************/

/************************ Public Variables **********************/

/************************ Public Functions **********************/

#ifdef __cplusplus
}
#endif

#endif


