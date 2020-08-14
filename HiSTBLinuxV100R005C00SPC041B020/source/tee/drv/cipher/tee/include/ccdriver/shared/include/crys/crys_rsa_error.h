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

 
 #ifndef CRYS_RSA_ERROR_H
#define CRYS_RSA_ERROR_H

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
   *  Object % crys_rsa_error.h    : %
   *  State           :  %state%
   *  Creation date   :  Sun Nov 21 11:07:08 2004
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This module containes the definitions of the CRYS RSA errors.
   *
   *  \version crys_rsa_error.h#1:incl:1
   *  \author adams
   */




/************************ Defines ******************************/

/* PKI RSA module on the CRYS layer base address - 0x00F00400 */

/* The CRYS RSA module errors */
#define CRYS_RSA_INVALID_MODULUS_SIZE                        (CRYS_RSA_MODULE_ERROR_BASE + 0x0UL)
#define CRYS_RSA_INVALID_MODULUS_POINTER_ERROR               (CRYS_RSA_MODULE_ERROR_BASE + 0x1UL)
#define CRYS_RSA_INVALID_EXPONENT_POINTER_ERROR              (CRYS_RSA_MODULE_ERROR_BASE + 0x2UL)
#define CRYS_RSA_INVALID_PUB_KEY_STRUCT_POINTER_ERROR        (CRYS_RSA_MODULE_ERROR_BASE + 0x3UL)
#define CRYS_RSA_INVALID_PRIV_KEY_STRUCT_POINTER_ERROR       (CRYS_RSA_MODULE_ERROR_BASE + 0x4UL)
#define CRYS_RSA_INVALID_EXPONENT_VAL                        (CRYS_RSA_MODULE_ERROR_BASE + 0x5UL)
#define CRYS_RSA_INVALID_EXPONENT_SIZE                       (CRYS_RSA_MODULE_ERROR_BASE + 0x6UL)
#define CRYS_RSA_INVALID_CRT_FIRST_FACTOR_POINTER_ERROR      (CRYS_RSA_MODULE_ERROR_BASE + 0x7UL)
#define CRYS_RSA_INVALID_CRT_SECOND_FACTOR_POINTER_ERROR     (CRYS_RSA_MODULE_ERROR_BASE + 0x8UL)
#define CRYS_RSA_INVALID_CRT_FIRST_FACTOR_EXP_PTR_ERROR      (CRYS_RSA_MODULE_ERROR_BASE + 0x9UL)
#define CRYS_RSA_INVALID_CRT_SECOND_FACTOR_EXP_PTR_ERROR     (CRYS_RSA_MODULE_ERROR_BASE + 0xAUL)
#define CRYS_RSA_INVALID_CRT_COEFFICIENT_PTR_ERROR           (CRYS_RSA_MODULE_ERROR_BASE + 0xBUL)
#define CRYS_RSA_INVALID_CRT_FIRST_FACTOR_SIZE               (CRYS_RSA_MODULE_ERROR_BASE + 0xCUL) 
#define CRYS_RSA_INVALID_CRT_SECOND_FACTOR_SIZE              (CRYS_RSA_MODULE_ERROR_BASE + 0xDUL) 
#define CRYS_RSA_INVALID_CRT_FIRST_AND_SECOND_FACTOR_SIZE    (CRYS_RSA_MODULE_ERROR_BASE + 0xEUL)
#define CRYS_RSA_INVALID_CRT_FIRST_FACTOR_EXPONENT_VAL       (CRYS_RSA_MODULE_ERROR_BASE + 0xFUL)
#define CRYS_RSA_INVALID_CRT_SECOND_FACTOR_EXPONENT_VAL      (CRYS_RSA_MODULE_ERROR_BASE + 0x10UL)
#define CRYS_RSA_INVALID_CRT_COEFF_VAL                       (CRYS_RSA_MODULE_ERROR_BASE + 0x11UL)
#define CRYS_RSA_DATA_POINTER_INVALID_ERROR                  (CRYS_RSA_MODULE_ERROR_BASE + 0x12UL)
#define CRYS_RSA_INVALID_MESSAGE_DATA_SIZE                   (CRYS_RSA_MODULE_ERROR_BASE + 0x13UL)
#define CRYS_RSA_INVALID_MESSAGE_VAL                         (CRYS_RSA_MODULE_ERROR_BASE + 0x14UL)

#define CRYS_RSA_MODULUS_EVEN_ERROR                          (CRYS_RSA_MODULE_ERROR_BASE + 0x15UL)
#define CRYS_RSA_INVALID_USER_CONTEXT_POINTER_ERROR          (CRYS_RSA_MODULE_ERROR_BASE + 0x16UL)
#define CRYS_RSA_HASH_ILLEGAL_OPERATION_MODE_ERROR           (CRYS_RSA_MODULE_ERROR_BASE + 0x17UL)
#define CRYS_RSA_MGF_ILLEGAL_ARG_ERROR                       (CRYS_RSA_MODULE_ERROR_BASE + 0x18UL)
#define CRYS_RSA_PKCS1_VER_ARG_ERROR                         (CRYS_RSA_MODULE_ERROR_BASE + 0x19UL)

#define CRYS_RSA_PRIV_KEY_VALIDATION_TAG_ERROR               (CRYS_RSA_MODULE_ERROR_BASE + 0x1AUL)
#define CRYS_RSA_PUB_KEY_VALIDATION_TAG_ERROR                (CRYS_RSA_MODULE_ERROR_BASE + 0x1BUL)
#define CRYS_RSA_USER_CONTEXT_VALIDATION_TAG_ERROR           (CRYS_RSA_MODULE_ERROR_BASE + 0x1CUL)
#define CRYS_RSA_INVALID_OUTPUT_POINTER_ERROR                (CRYS_RSA_MODULE_ERROR_BASE + 0x1DUL)
#define CRYS_RSA_INVALID_OUTPUT_SIZE_POINTER_ERROR           (CRYS_RSA_MODULE_ERROR_BASE + 0x1FUL)
#define CRYS_RSA_CONV_TO_CRT_INVALID_TEMP_BUFF_POINTER_ERROR (CRYS_RSA_MODULE_ERROR_BASE + 0x20UL)

#define CRYS_RSA_BASE_OAEP_ENCODE_PARAMETER_STRING_TOO_LONG  (CRYS_RSA_MODULE_ERROR_BASE + 0x22UL)
#define CRYS_RSA_BASE_OAEP_DECODE_PARAMETER_STRING_TOO_LONG  (CRYS_RSA_MODULE_ERROR_BASE + 0x23UL)
#define CRYS_RSA_BASE_OAEP_ENCODE_MESSAGE_TOO_LONG           (CRYS_RSA_MODULE_ERROR_BASE + 0x24UL)
#define CRYS_RSA_BASE_OAEP_DECODE_MESSAGE_TOO_LONG           (CRYS_RSA_MODULE_ERROR_BASE + 0x25UL)
#define CRYS_RSA_KEY_GEN_DATA_STRUCT_POINTER_INVALID         (CRYS_RSA_MODULE_ERROR_BASE + 0x26UL)
#define CRYS_RSA_PRIM_DATA_STRUCT_POINTER_INVALID            (CRYS_RSA_MODULE_ERROR_BASE + 0x27UL)
#define CRYS_RSA_INVALID_MESSAGE_BUFFER_SIZE                 (CRYS_RSA_MODULE_ERROR_BASE + 0x28UL)
#define CRYS_RSA_INVALID_SIGNATURE_BUFFER_SIZE               (CRYS_RSA_MODULE_ERROR_BASE + 0x29UL)
#define CRYS_RSA_INVALID_MOD_BUFFER_SIZE_POINTER             (CRYS_RSA_MODULE_ERROR_BASE + 0x2AUL)
#define CRYS_RSA_INVALID_EXP_BUFFER_SIZE_POINTER             (CRYS_RSA_MODULE_ERROR_BASE + 0x2BUL)
#define CRYS_RSA_INVALID_SIGNATURE_BUFFER_POINTER            (CRYS_RSA_MODULE_ERROR_BASE + 0x2CUL)
#define CRYS_RSA_WRONG_PRIVATE_KEY_TYPE                      (CRYS_RSA_MODULE_ERROR_BASE + 0x2DUL)

#define CRYS_RSA_INVALID_CRT_FIRST_FACTOR_SIZE_POINTER_ERROR (CRYS_RSA_MODULE_ERROR_BASE + 0x2EUL)
#define CRYS_RSA_INVALID_CRT_SECOND_FACTOR_SIZE_POINTER_ERROR (CRYS_RSA_MODULE_ERROR_BASE + 0x2FUL)
#define CRYS_RSA_INVALID_CRT_FIRST_FACTOR_EXP_SIZE_PTR_ERROR (CRYS_RSA_MODULE_ERROR_BASE + 0x30UL) 
#define CRYS_RSA_INVALID_CRT_SECOND_FACTOR_EXP_SIZE_PTR_ERROR (CRYS_RSA_MODULE_ERROR_BASE + 0x31UL) 
#define CRYS_RSA_INVALID_CRT_COEFFICIENT_SIZE_PTR_ERROR		 (CRYS_RSA_MODULE_ERROR_BASE + 0x32UL)

#define CRYS_RSA_INVALID_CRT_FIRST_FACTOR_SIZE_ERROR         (CRYS_RSA_MODULE_ERROR_BASE + 0x33UL)
#define CRYS_RSA_INVALID_CRT_SECOND_FACTOR_SIZE_ERROR        (CRYS_RSA_MODULE_ERROR_BASE + 0x34UL)
#define CRYS_RSA_INVALID_CRT_FIRST_FACTOR_EXP_SIZE_ERROR     (CRYS_RSA_MODULE_ERROR_BASE + 0x35UL) 
#define CRYS_RSA_INVALID_CRT_SECOND_FACTOR_EXP_SIZE_ERROR    (CRYS_RSA_MODULE_ERROR_BASE + 0x36UL) 
#define CRYS_RSA_INVALID_CRT_COEFFICIENT_SIZE_ERROR		     (CRYS_RSA_MODULE_ERROR_BASE + 0x37UL)
#define	CRYS_RSA_KEY_GEN_CONDITIONAL_TEST_FAIL_ERROR	     (CRYS_RSA_MODULE_ERROR_BASE + 0x38UL)

#define	CRYS_RSA_CAN_NOT_GENERATE_RAND_IN_RANGE     	     (CRYS_RSA_MODULE_ERROR_BASE + 0x39UL)
#define CRYS_RSA_INVALID_CRT_PARAMETR_SIZE_ERROR             (CRYS_RSA_MODULE_ERROR_BASE + 0x3AUL)

#define CRYS_RSA_INVALID_MODULUS_ERROR                       (CRYS_RSA_MODULE_ERROR_BASE + 0x40UL)
#define CRYS_RSA_INVALID_PTR_ERROR                           (CRYS_RSA_MODULE_ERROR_BASE + 0x41UL)
#define CRYS_RSA_INVALID_DECRYPRION_MODE_ERROR               (CRYS_RSA_MODULE_ERROR_BASE + 0x42UL)

#define CRYS_RSA_FIPS_MODE_FAIL_ERROR			     (CRYS_RSA_MODULE_ERROR_BASE + 0x43UL)
#define CRYS_RSA_FIPS_MODE_SF_NOT_PERFORMED_ERROR	     (CRYS_RSA_MODULE_ERROR_BASE + 0x44UL)
#define CRYS_RSA_INVALID_INPUT_BUFFER_SIZE_ERROR             (CRYS_RSA_MODULE_ERROR_BASE + 0x45UL)


/****************************************************************************************
 * PKCS#1 VERSION 1.5 ERRORS
 ****************************************************************************************/
#define CRYS_RSA_BER_ENCODING_OK                            CRYS_OK
#define CRYS_RSA_ERROR_BER_PARSING                         (CRYS_RSA_MODULE_ERROR_BASE+0x51UL)
#define CRYS_RSA_ENCODE_15_MSG_OUT_OF_RANGE                (CRYS_RSA_MODULE_ERROR_BASE+0x52UL)
#define CRYS_RSA_ENCODE_15_PS_TOO_SHORT                    (CRYS_RSA_MODULE_ERROR_BASE+0x53UL)
#define CRYS_RSA_PKCS1_15_BLOCK_TYPE_NOT_SUPPORTED         (CRYS_RSA_MODULE_ERROR_BASE+0x54UL)
#define CRYS_RSA_15_ERROR_IN_DECRYPTED_BLOCK_PARSING       (CRYS_RSA_MODULE_ERROR_BASE+0x55UL)
#define CRYS_RSA_ERROR_IN_RANDOM_OPERATION_FOR_ENCODE      (CRYS_RSA_MODULE_ERROR_BASE+0x56UL)
#define CRYS_RSA_ERROR_VER15_INCONSISTENT_VERIFY           (CRYS_RSA_MODULE_ERROR_BASE+0x57UL)
#define CRYS_RSA_INVALID_MESSAGE_DATA_SIZE_IN_NO_HASH_CASE (CRYS_RSA_MODULE_ERROR_BASE+0x58UL)
#define CRYS_RSA_INVALID_MESSAGE_DATA_SIZE_IN_SSL_CASE	   (CRYS_RSA_MODULE_ERROR_BASE+0x59UL)
#define CRYS_RSA_PKCS15_VERIFY_BER_ENCODING_HASH_TYPE	   (CRYS_RSA_MODULE_ERROR_BASE+0x60UL)  /*PKCS#1 Ver 1.5 verify hash input inconsistent with hash mode derived from signature*/
#define CRYS_RSA_GET_DER_HASH_MODE_ILLEGAL                 (CRYS_RSA_MODULE_ERROR_BASE+0x61UL)

/****************************************************************************************
 * PKCS#1 VERSION 2.1 ERRORS
 ****************************************************************************************/
#define CRYS_RSA_PSS_ENCODING_MODULUS_HASH_SALT_LENGTHS_ERROR  (CRYS_RSA_MODULE_ERROR_BASE+0x80UL)
#define CRYS_RSA_BASE_MGF_MASK_TOO_LONG                   (CRYS_RSA_MODULE_ERROR_BASE+0x81UL)
#define CRYS_RSA_ERROR_PSS_INCONSISTENT_VERIFY            (CRYS_RSA_MODULE_ERROR_BASE+0x82UL)
#define CRYS_RSA_OAEP_VER21_MESSAGE_TOO_LONG              (CRYS_RSA_MODULE_ERROR_BASE+0x83UL)
#define CRYS_RSA_ERROR_IN_DECRYPTED_BLOCK_PARSING         (CRYS_RSA_MODULE_ERROR_BASE+0x84UL)
#define CRYS_RSA_OAEP_DECODE_ERROR                        (CRYS_RSA_MODULE_ERROR_BASE+0x85UL)
#define CRYS_RSA_15_ERROR_IN_DECRYPTED_DATA_SIZE          (CRYS_RSA_MODULE_ERROR_BASE+0x86UL)
#define CRYS_RSA_15_ERROR_IN_DECRYPTED_DATA               (CRYS_RSA_MODULE_ERROR_BASE+0x87UL)
#define CRYS_RSA_OAEP_L_POINTER_ERROR                     (CRYS_RSA_MODULE_ERROR_BASE+0x88UL)
#define CRYS_RSA_DECRYPT_INVALID_OUTPUT_SIZE              (CRYS_RSA_MODULE_ERROR_BASE+0x89UL)
#define CRYS_RSA_DECRYPT_OUTPUT_SIZE_POINTER_ERROR        (CRYS_RSA_MODULE_ERROR_BASE+0x8AUL)

#define CRYS_RSA_HOST_MSG_GENERAL_RPC_A_ERROR               (CRYS_RSA_MODULE_ERROR_BASE+0x90UL)
#define CRYS_RSA_HOST_MSG_GENERAL_RPC_B_ERROR               (CRYS_RSA_MODULE_ERROR_BASE+0x91UL)
#define CRYS_RSA_HOST_MSG_GENERAL_RPC_C_ERROR               (CRYS_RSA_MODULE_ERROR_BASE+0x92UL)

#define CRYS_RSA_ILLEGAL_PARAMS_ACCORDING_TO_PRIV_ERROR   (CRYS_RSA_MODULE_ERROR_BASE + 0x93UL)
#define CRYS_RSA_IS_NOT_SUPPORTED                         (CRYS_RSA_MODULE_ERROR_BASE+0xFFUL)


/************************ Enums ********************************/


/************************ Typedefs  ****************************/


/************************ Structs  ******************************/


/************************ Public Variables **********************/


/************************ Public Functions **********************/

#ifdef __cplusplus
}
#endif

#endif


