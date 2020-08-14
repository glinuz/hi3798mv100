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

 
 
#ifndef CRYS_HASH_ERROR_H
#define CRYS_HASH_ERROR_H

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
   *  Object % crys_hash_error.h    : %
   *  State           :  %state%
   *  Creation date   :  Sun Nov 21 11:07:08 2004
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This module containes the definitions of the CRYS AES errors.
   *
   *  \version crys_aes_error.h#1:incl:1
   *  \author adams
   */




/************************ Defines ******************************/
/*HASH module on the CRYS layer base address - 0x00F00200*/
/* The CRYS HASH module errors */
#define CRYS_HASH_INVALID_USER_CONTEXT_POINTER_ERROR    	(CRYS_HASH_MODULE_ERROR_BASE + 0x0UL)
#define CRYS_HASH_ILLEGAL_OPERATION_MODE_ERROR          	(CRYS_HASH_MODULE_ERROR_BASE + 0x1UL)
#define CRYS_HASH_USER_CONTEXT_CORRUPTED_ERROR          	(CRYS_HASH_MODULE_ERROR_BASE + 0x2UL) 
#define CRYS_HASH_DATA_IN_POINTER_INVALID_ERROR         	(CRYS_HASH_MODULE_ERROR_BASE + 0x3UL)
#define CRYS_HASH_DATA_SIZE_ILLEGAL                     	(CRYS_HASH_MODULE_ERROR_BASE + 0x4UL)
#define CRYS_HASH_INVALID_RESULT_BUFFER_POINTER_ERROR   	(CRYS_HASH_MODULE_ERROR_BASE + 0x5UL)
#define CRYS_HASH_DATA_SIZE_IS_ILLEGAL_FOR_CSI_ERROR    	(CRYS_HASH_MODULE_ERROR_BASE + 0x8UL)
#define CRYS_HASH_LAST_BLOCK_ALREADY_PROCESSED_ERROR		(CRYS_HASH_MODULE_ERROR_BASE + 0xCUL)
#define CRYS_HASH_ILLEGAL_PARAMS_ERROR 				(CRYS_HASH_MODULE_ERROR_BASE + 0xDUL)
	

#define CRYS_HASH_IS_NOT_SUPPORTED                      	(CRYS_HASH_MODULE_ERROR_BASE + 0xFUL)



/************************ Enums ********************************/


/************************ Typedefs  ****************************/


/************************ Structs  ******************************/


/************************ Public Variables **********************/


/************************ Public Functions **********************/

#ifdef __cplusplus
}
#endif

#endif


