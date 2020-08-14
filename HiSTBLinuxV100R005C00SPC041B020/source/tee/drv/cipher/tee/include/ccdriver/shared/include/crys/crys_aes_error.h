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

 
 
#ifndef CRYS_AES_ERROR_H
#define CRYS_AES_ERROR_H

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
   *  Object % crys_aes_error.h    : %
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

/* The CRYS AES module errors base address - 0x00F00000 */
#define CRYS_AES_INVALID_USER_CONTEXT_POINTER_ERROR     	(CRYS_AES_MODULE_ERROR_BASE + 0x00UL)
#define CRYS_AES_INVALID_IV_OR_TWEAK_PTR_ERROR          	(CRYS_AES_MODULE_ERROR_BASE + 0x01UL)
#define CRYS_AES_ILLEGAL_OPERATION_MODE_ERROR           	(CRYS_AES_MODULE_ERROR_BASE + 0x02UL)
#define CRYS_AES_ILLEGAL_KEY_SIZE_ERROR                 	(CRYS_AES_MODULE_ERROR_BASE + 0x03UL)
#define CRYS_AES_INVALID_KEY_POINTER_ERROR              	(CRYS_AES_MODULE_ERROR_BASE + 0x04UL)
#define CRYS_AES_INVALID_ENCRYPT_MODE_ERROR             	(CRYS_AES_MODULE_ERROR_BASE + 0x06UL)
#define CRYS_AES_USER_CONTEXT_CORRUPTED_ERROR           	(CRYS_AES_MODULE_ERROR_BASE + 0x07UL) 
#define CRYS_AES_DATA_IN_POINTER_INVALID_ERROR          	(CRYS_AES_MODULE_ERROR_BASE + 0x08UL)
#define CRYS_AES_DATA_OUT_POINTER_INVALID_ERROR         	(CRYS_AES_MODULE_ERROR_BASE + 0x09UL)
#define CRYS_AES_DATA_IN_SIZE_ILLEGAL                   	(CRYS_AES_MODULE_ERROR_BASE + 0x0AUL)
#define CRYS_AES_DATA_OUT_DATA_IN_OVERLAP_ERROR         	(CRYS_AES_MODULE_ERROR_BASE + 0x0BUL)
#define CRYS_AES_DECRYPT_MODE_NOT_ALLOWED_ON_CTR_MODE   	(CRYS_AES_MODULE_ERROR_BASE + 0x0CUL)
#define CRYS_AES_XTS_IS_NOT_SUPPORTED_ON_HW_ERROR       	(CRYS_AES_MODULE_ERROR_BASE + 0x0DUL)
#define CRYS_AES_DATA_OUT_SIZE_POINTER_INVALID_ERROR    	(CRYS_AES_MODULE_ERROR_BASE + 0x10UL)
#define CRYS_AES_DATA_OUT_SIZE_INVALID_ERROR            	(CRYS_AES_MODULE_ERROR_BASE + 0x11UL)
#define CRYS_AES_DECRYPTION_NOT_ALLOWED_ON_THIS_MODE    	(CRYS_AES_MODULE_ERROR_BASE + 0x12UL)
#define CRYS_AES_NOT_ALLOWED_KEY_TYPE_ON_THIS_MODE      	(CRYS_AES_MODULE_ERROR_BASE + 0x13UL)
#define CRYS_AES_ILLEGAL_SECRET_KEY_MODE                	(CRYS_AES_MODULE_ERROR_BASE + 0x14UL)
#define CRYS_AES_ADDITIONAL_BLOCK_NOT_PERMITTED_ERROR   	(CRYS_AES_MODULE_ERROR_BASE + 0x15UL)

/* The CRYS_AES_Wrap and CRYS_AES_Unrap errors */
#define CRYS_AES_WRAP_ILLEGAL_DATA_PTR_ERROR            	(CRYS_AES_MODULE_ERROR_BASE + 0x30UL)
#define CRYS_AES_WRAP_DATA_LENGTH_ERROR                 	(CRYS_AES_MODULE_ERROR_BASE + 0x31UL)
#define CRYS_AES_WRAP_ILLEGAL_KEY_PTR_ERROR             	(CRYS_AES_MODULE_ERROR_BASE + 0x32UL)
#define CRYS_AES_WRAP_KEY_LENGTH_ERROR                  	(CRYS_AES_MODULE_ERROR_BASE + 0x33UL)
#define CRYS_AES_WRAP_ILLEGAL_WRAP_DATA_PTR_ERROR       	(CRYS_AES_MODULE_ERROR_BASE + 0x34UL)
#define CRYS_AES_WRAP_ILLEGAL_WRAP_DATA_LEN_PTR_ERROR   	(CRYS_AES_MODULE_ERROR_BASE + 0x35UL)
#define CRYS_AES_WRAP_ILLEGAL_WRAP_DATA_LENGTH_ERROR    	(CRYS_AES_MODULE_ERROR_BASE + 0x36UL)
#define CRYS_AES_WRAP_DATA_OUT_DATA_IN_OVERLAP_ERROR    	(CRYS_AES_MODULE_ERROR_BASE + 0x37UL)
#define CRYS_AES_WRAP_IS_SECRET_KEY_FLAG_ILLEGAL_ERROR  	(CRYS_AES_MODULE_ERROR_BASE + 0x38UL)

#define CRYS_AES_UNWRAP_ILLEGAL_WRAP_DATA_PTR_ERROR     	(CRYS_AES_MODULE_ERROR_BASE + 0x40UL)
#define CRYS_AES_UNWRAP_WRAP_DATA_LENGTH_ERROR          	(CRYS_AES_MODULE_ERROR_BASE + 0x41UL)
#define CRYS_AES_UNWRAP_ILLEGAL_KEY_PTR_ERROR           	(CRYS_AES_MODULE_ERROR_BASE + 0x42UL)
#define CRYS_AES_UNWRAP_KEY_LEN_ERROR                   	(CRYS_AES_MODULE_ERROR_BASE + 0x43UL)
#define CRYS_AES_UNWRAP_ILLEGAL_DATA_PTR_ERROR          	(CRYS_AES_MODULE_ERROR_BASE + 0x44UL)
#define CRYS_AES_UNWRAP_ILLEGAL_DATA_LEN_PTR_ERROR      	(CRYS_AES_MODULE_ERROR_BASE + 0x45UL)
#define CRYS_AES_UNWRAP_ILLEGAL_DATA_LENGTH_ERROR       	(CRYS_AES_MODULE_ERROR_BASE + 0x46UL)
#define CRYS_AES_UNWRAP_FUNCTION_FAILED_ERROR           	(CRYS_AES_MODULE_ERROR_BASE + 0x47UL)
#define CRYS_AES_UNWRAP_DATA_OUT_DATA_IN_OVERLAP_ERROR  	(CRYS_AES_MODULE_ERROR_BASE + 0x48UL)
#define CRYS_AES_UNWRAP_IS_SECRET_KEY_FLAG_ILLEGAL_ERROR 	(CRYS_AES_MODULE_ERROR_BASE + 0x49UL)

#define CRYS_AES_XTS_IS_NOT_SUPPORTED_ERROR			(CRYS_AES_MODULE_ERROR_BASE + 0x50UL)
#define CRYS_AES_ILLEGAL_PARAMS_ERROR    			(CRYS_AES_MODULE_ERROR_BASE + 0x60UL)
#define CRYS_AES_CTR_ILLEGAL_BLOCK_OFFSET_ERROR    		(CRYS_AES_MODULE_ERROR_BASE + 0x70UL)
#define CRYS_AES_CTR_ILLEGAL_COUNTER_ERROR    			(CRYS_AES_MODULE_ERROR_BASE + 0x71UL)
#define CRYS_AES_IS_NOT_SUPPORTED                       	(CRYS_AES_MODULE_ERROR_BASE + 0xFFUL)

/************************ Enums ********************************/

/************************ Typedefs  ****************************/

/************************ Structs  *****************************/

/************************ Public Variables *********************/

/************************ Public Functions *********************/

#ifdef __cplusplus
}
#endif

#endif


