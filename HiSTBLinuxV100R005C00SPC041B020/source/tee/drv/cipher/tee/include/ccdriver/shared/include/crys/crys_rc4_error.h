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

 
 
#ifndef CRYS_RC4_ERROR_H
#define CRYS_RC4_ERROR_H

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
   *  Creation date   :  Mon Mar 07 10:50:30 2005
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief A brief description of this module
   *
   *  \version CRYS_RC4_error.h#1:incl:1
   *  \author adams
   */




/************************ Defines ******************************/

/* The CRYS RC4 module errors */
#define CRYS_RC4_INVALID_USER_CONTEXT_POINTER_ERROR   (CRYS_RC4_MODULE_ERROR_BASE + 0x0UL)
#define CRYS_RC4_ILLEGAL_KEY_SIZE_ERROR               (CRYS_RC4_MODULE_ERROR_BASE + 0x1UL)
#define CRYS_RC4_INVALID_KEY_POINTER_ERROR            (CRYS_RC4_MODULE_ERROR_BASE + 0x2UL)
#define CRYS_RC4_USER_CONTEXT_CORRUPTED_ERROR         (CRYS_RC4_MODULE_ERROR_BASE + 0x3UL)
#define CRYS_RC4_DATA_IN_POINTER_INVALID_ERROR        (CRYS_RC4_MODULE_ERROR_BASE + 0x4UL)
#define CRYS_RC4_DATA_OUT_POINTER_INVALID_ERROR       (CRYS_RC4_MODULE_ERROR_BASE + 0x5UL)
#define CRYS_RC4_DATA_OUT_DATA_IN_OVERLAP_ERROR       (CRYS_RC4_MODULE_ERROR_BASE + 0x6UL)
#define CRYS_RC4_DATA_SIZE_ILLEGAL                    (CRYS_RC4_MODULE_ERROR_BASE + 0x7UL)

/* The CRYS_RC4_SEP additional errors*/
#define CRYS_RC4_DATA_IN_LLI_TAB_POINTER_INVALID_ERROR  (CRYS_RC4_MODULE_ERROR_BASE + 0x16UL)
#define CRYS_RC4_DATA_IN_LLI_TAB_SIZE_INVALID_ERROR     (CRYS_RC4_MODULE_ERROR_BASE + 0x17UL)
#define CRYS_RC4_DATA_OUT_LLI_TAB_POINTER_INVALID_ERROR (CRYS_RC4_MODULE_ERROR_BASE + 0x18UL)
#define CRYS_RC4_DATA_OUT_LLI_TAB_SIZE_INVALID_ERROR    (CRYS_RC4_MODULE_ERROR_BASE + 0x19UL)
#define CRYS_RC4_ILLEGAL_PARAMS_ERROR			(CRYS_RC4_MODULE_ERROR_BASE + 0x1AUL)
	
#define CRYS_RC4_IS_NOT_SUPPORTED                     (CRYS_RC4_MODULE_ERROR_BASE + 0xFFUL)


/************************ Enums ********************************/


/************************ Typedefs  ****************************/


/************************ Structs  ******************************/


/************************ Public Variables **********************/


/************************ Public Functions **********************/

#ifdef __cplusplus
}
#endif

#endif

