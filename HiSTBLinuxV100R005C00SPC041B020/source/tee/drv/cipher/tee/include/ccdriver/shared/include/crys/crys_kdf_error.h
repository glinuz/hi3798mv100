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


 
 
#ifndef CRYS_KDF_error_h_H
#define CRYS_KDF_error_h_H

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
   *  Object %name    : %crys_kdf_errors.h
   *  State           :  %state%
   *  Creation date   :  14 May 2007
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief A brief description of this module
   *
   *  \version crys_kdf_errors.h#1:incl:1
   *  \author R.Levin
   */


/************************ Defines *******************************/

/* The CRYS KDF module errors / base address - 0x00F01100*/
#define CRYS_KDF_INVALID_ARGUMENT_POINTER_ERROR			(CRYS_KDF_MODULE_ERROR_BASE + 0x0UL)
#define CRYS_KDF_INVALID_ARGUMENT_SIZE_ERROR			(CRYS_KDF_MODULE_ERROR_BASE + 0x1UL)
#define CRYS_KDF_INVALID_ARGUMENT_OPERATION_MODE_ERROR		(CRYS_KDF_MODULE_ERROR_BASE + 0x2UL)
#define CRYS_KDF_INVALID_ARGUMENT_HASH_MODE_ERROR		(CRYS_KDF_MODULE_ERROR_BASE + 0x3UL)
#define CRYS_KDF_INVALID_KEY_DERIVATION_MODE_ERROR              (CRYS_KDF_MODULE_ERROR_BASE + 0x4UL)
#define CRYS_KDF_INVALID_SHARED_SECRET_VALUE_SIZE_ERROR         (CRYS_KDF_MODULE_ERROR_BASE + 0x5UL)
#define CRYS_KDF_INVALID_OTHER_INFO_SIZE_ERROR                  (CRYS_KDF_MODULE_ERROR_BASE + 0x6UL)
#define CRYS_KDF_INVALID_KEYING_DATA_SIZE_ERROR                 (CRYS_KDF_MODULE_ERROR_BASE + 0x7UL)
#define CRYS_KDF_INVALID_ALGORITHM_ID_POINTER_ERROR             (CRYS_KDF_MODULE_ERROR_BASE + 0x8UL)
#define CRYS_KDF_INVALID_ALGORITHM_ID_SIZE_ERROR                (CRYS_KDF_MODULE_ERROR_BASE + 0x9UL)

#define CRYS_KDF_HOST_MSG_GENERAL_RPC_A_ERROR                   (CRYS_KDF_MODULE_ERROR_BASE + 0xAUL)
#define CRYS_KDF_HOST_MSG_GENERAL_RPC_B_ERROR                   (CRYS_KDF_MODULE_ERROR_BASE + 0xBUL)
#define CRYS_KDF_HOST_MSG_GENERAL_RPC_C_ERROR                   (CRYS_KDF_MODULE_ERROR_BASE + 0xCUL)

#define CRYS_KDF_IS_NOT_SUPPORTED                               (CRYS_KDF_MODULE_ERROR_BASE + 0xFFUL)

/************************ Enums *********************************/

/************************ Typedefs  *****************************/

/************************ Structs  ******************************/

/************************ Public Variables **********************/

/************************ Public Functions **********************/




#ifdef __cplusplus
}
#endif

#endif




