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


 
 
#ifndef _1_CRYS_DH_errors_h_H
#define _1_CRYS_DH_errors_h_H

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
   *  Object %name    : %
   *  State           :  %state%
   *  Creation date   :  Tue Feb 01 17:26:37 2005
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This file contains error codes definitions for CRYS DH module.
   *
   *  \version crys_dh_errors.h#1:incl:1
   *  \author ohads
   */

/************************ Defines ******************************/
/* DH module on the CRYS layer base address - 0x00F00500 */
/* The CRYS DH module errors */
#define CRYS_DH_INVALID_ARGUMENT_POINTER_ERROR			(CRYS_DH_MODULE_ERROR_BASE + 0x0UL)
#define CRYS_DH_INVALID_ARGUMENT_SIZE_ERROR			(CRYS_DH_MODULE_ERROR_BASE + 0x1UL)
#define CRYS_DH_INVALID_ARGUMENT_OPERATION_MODE_ERROR		(CRYS_DH_MODULE_ERROR_BASE + 0x2UL)
#define CRYS_DH_INVALID_ARGUMENT_HASH_MODE_ERROR		(CRYS_DH_MODULE_ERROR_BASE + 0x3UL)

/*the derived secret key size needed is wrong*/
#define CRYS_DH_SECRET_KEYING_DATA_SIZE_ILLEGAL_ERROR		(CRYS_DH_MODULE_ERROR_BASE + 0x4UL)
#define CRYS_DH_INVALID_L_ARGUMENT_ERROR			(CRYS_DH_MODULE_ERROR_BASE + 0x5UL)
#define CRYS_DH_ARGUMENT_PRIME_SMALLER_THAN_GENERATOR_ERROR	(CRYS_DH_MODULE_ERROR_BASE + 0x6UL)
#define CRYS_DH_ARGUMENT_GENERATOR_SMALLER_THAN_ZERO_ERROR    	(CRYS_DH_MODULE_ERROR_BASE + 0x7UL)
#define CRYS_DH_ARGUMENT_PRV_SIZE_ERROR				(CRYS_DH_MODULE_ERROR_BASE + 0x8UL)
#define CRYS_DH_ARGUMENT_BUFFER_SIZE_ERROR			(CRYS_DH_MODULE_ERROR_BASE + 0x9UL)
#define CRYS_DH_IS_NOT_SUPPORTED				(CRYS_DH_MODULE_ERROR_BASE + 0xFUL)

#define CRYS_DH_X942_HYBRID_SIZE1_BUFFER_ERROR			(CRYS_DH_MODULE_ERROR_BASE + 0x15UL)

/*The requested derived secret key size is invalid*/
#define CRYS_DH_SECRET_KEY_SIZE_NEEDED_ERROR			(CRYS_DH_MODULE_ERROR_BASE + 0x16UL)
#define CRYS_DH_SECRET_KEY_SIZE_OUTPUT_ERROR			(CRYS_DH_MODULE_ERROR_BASE + 0x17UL)
#define CRYS_DH_OTHERINFO_SIZE_ERROR                            (CRYS_DH_MODULE_ERROR_BASE + 0x18UL)

/* DH domain and key generation and checking errors */
#define CRYS_DH_INVALID_MODULUS_SIZE_ERROR			(CRYS_DH_MODULE_ERROR_BASE + 0x20UL)
#define CRYS_DH_INVALID_ORDER_SIZE_ERROR			(CRYS_DH_MODULE_ERROR_BASE + 0x21UL)
#define CRYS_DH_INVALID_SEED_SIZE_ERROR  			(CRYS_DH_MODULE_ERROR_BASE + 0x22UL)
#define CRYS_DH_INVALID_J_FACTOR_PTR_OR_SIZE_ERROR 		(CRYS_DH_MODULE_ERROR_BASE + 0x23UL)
#define CRYS_DH_INVALID_GENERATOR_PTR_OR_SIZE_ERROR 		(CRYS_DH_MODULE_ERROR_BASE + 0x24UL)

#define CRYS_DH_CHECK_DOMAIN_PRIMES_NOT_VALID_ERROR         	(CRYS_DH_MODULE_ERROR_BASE + 0x25UL)
#define CRYS_DH_CHECK_DOMAIN_GENERATOR_NOT_VALID_ERROR         	(CRYS_DH_MODULE_ERROR_BASE + 0x26UL)
#define CRYS_DH_INVALID_PUBLIC_KEY_SIZE_ERROR                 	(CRYS_DH_MODULE_ERROR_BASE + 0x27UL)
#define CRYS_DH_CHECK_PUB_KEY_NOT_VALID_ERROR                 	(CRYS_DH_MODULE_ERROR_BASE + 0x28UL)
#define CRYS_DH_CHECK_GENERATOR_SIZE_OR_PTR_NOT_VALID_ERROR    	(CRYS_DH_MODULE_ERROR_BASE + 0x29UL)
#define CRYS_DH_CHECK_SEED_SIZE_OR_PTR_NOT_VALID_ERROR          (CRYS_DH_MODULE_ERROR_BASE + 0x2AUL)
#define CRYS_DH_CHECK_GENERATOR_NOT_VALID_ERROR               	(CRYS_DH_MODULE_ERROR_BASE + 0x2BUL)
#define CRYS_DH_PRIME_P_GENERATION_FAILURE_ERROR               	(CRYS_DH_MODULE_ERROR_BASE + 0x2CUL)
#define CRYS_DH_INVALID_PUBLIC_KEY_ERROR                    	(CRYS_DH_MODULE_ERROR_BASE + 0x2DUL)
#define CRYS_DH_PASSED_INVALID_SEED_ERROR  	            	(CRYS_DH_MODULE_ERROR_BASE + 0x2EUL)

#define CRYS_DH_HOST_MSG_GENERAL_RPC_A_ERROR                    (CRYS_DH_MODULE_ERROR_BASE + 0x30UL)
#define CRYS_DH_HOST_MSG_GENERAL_RPC_B_ERROR                    (CRYS_DH_MODULE_ERROR_BASE + 0x31UL)
#define CRYS_DH_HOST_MSG_GENERAL_RPC_C_ERROR                    (CRYS_DH_MODULE_ERROR_BASE + 0x32UL)

      



/************************ Enums ********************************/


/************************ Typedefs  ****************************/


/************************ Structs  ******************************/


/************************ Public Variables **********************/


/************************ Public Functions **********************/




#ifdef __cplusplus
}
#endif

#endif

