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

 
 
#ifndef CRYS_COMBINED_ERROR_H
#define CRYS_COMBINED_ERROR_H

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
   *  Object % crys_combined_error.h    : %
   *  State           :  %state%
   *  Creation date   :  Sun Nov 21 11:07:08 2004
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This module containes the definitions of the CRYS
   *         combined mode errors.
   *
   *  \version crys_combined_error.h#1:incl:1
   *  \author stasn
   */

/************************ Defines ******************************/
/*Combined mode module on the CRYS layer base address - 0x00F01900*/
/* The CRYS COMBINED module errors */
#define CRYS_COMBINED_INVALID_NODES_CONFIG_POINTER_ERROR   		(CRYS_COMBINED_MODULE_ERROR_BASE + 0x0UL)
#define CRYS_COMBINED_DATA_IN_SIZE_ILLEGAL			   	(CRYS_COMBINED_MODULE_ERROR_BASE + 0x1UL)
#define CRYS_COMBINED_DATA_IN_POINTER_INVALID_ERROR         		(CRYS_COMBINED_MODULE_ERROR_BASE + 0x2UL)
#define CRYS_COMBINED_DATA_SIZE_ILLEGAL                     		(CRYS_COMBINED_MODULE_ERROR_BASE + 0x3UL)
#define CRYS_COMBINED_DATA_OUT_POINTER_INVALID_ERROR   			(CRYS_COMBINED_MODULE_ERROR_BASE + 0x4UL)
#define CRYS_COMBINED_DATA_AUTH_POINTER_INVALID_ERROR 			(CRYS_COMBINED_MODULE_ERROR_BASE + 0x5UL)
#define CRYS_COMBINED_ILLEGAL_OPERATION_MODE_ERROR			(CRYS_COMBINED_MODULE_ERROR_BASE + 0x6UL)
#define CRYS_COMBINED_UNSUPPORTED_SUB_OPERATION_ALG_ERROR		(CRYS_COMBINED_MODULE_ERROR_BASE + 0x7UL)
#define CRYS_COMBINED_UNSUPPORTED_SUB_OPERATION_MODE_ERROR		(CRYS_COMBINED_MODULE_ERROR_BASE + 0x8UL)
#define CRYS_COMBINED_DATA_AUTH_BUFFER_SIZE_INVALID_ERROR		(CRYS_COMBINED_MODULE_ERROR_BASE + 0x9UL)
#define CRYS_COMBINED_CIPHER_OFFSET_ILLEGAL		      		(CRYS_COMBINED_MODULE_ERROR_BASE + 0xAUL)
#define CRYS_COMBINED_DATA_OUT_DATA_IN_OVERLAP_ERROR         		(CRYS_COMBINED_MODULE_ERROR_BASE + 0xBUL)
#define CRYS_COMBINED_INVALID_USER_SUB_CONTEXT_POINTER_ERROR		(CRYS_COMBINED_MODULE_ERROR_BASE + 0xCUL)
#define CRYS_COMBINED_ILLEGAL_PARAMS_ERROR		   		(CRYS_COMBINED_MODULE_ERROR_BASE + 0xDUL)
#define CRYS_COMBINED_HASH_DIGEST_SIZE_ERROR		   		(CRYS_COMBINED_MODULE_ERROR_BASE + 0xEUL)

#define CRYS_COMBINED_IS_NOT_SUPPORTED                      		(CRYS_COMBINED_MODULE_ERROR_BASE + 0xFUL)


/************************ Enums ********************************/

/************************ Typedefs  ****************************/

/************************ Structs  *****************************/

/************************ Public Variables *********************/

/************************ Public Functions *********************/

#ifdef __cplusplus
}
#endif

#endif


