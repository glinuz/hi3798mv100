/*******************************************************************  
* (c) Copyright 2011-2012 Discretix Technologies Ltd.              *
* This software is protected by copyright, international           *
* treaties and patents.                                            *
* Use of this Software as part of or with the Discretix CryptoCell *
* or Packet Engine products is governed by the products'           *
* commercial end user license agreement ("EULA").                  *
* It is possible that copies of this Software might be distributed *
* under some type of GNU General Public License ("GPL").           *
* Notwithstanding any such distribution under the terms of GPL,    *
* GPL does not govern the use of this Software as part of or with  *
* the Discretix CryptoCell or Packet Engine products, for which a  *
* EULA is required.                                                *
* If the product's EULA allows any copy or reproduction of this    *
* Software, then such copy or reproduction must include this       *
* Copyright Notice as well as any other notices provided           *
* thereunder.                                                      *
********************************************************************/

 
 #ifndef CRYS_COMMON_ERROR_H
#define CRYS_COMMON_ERROR_H

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
   *  Object % CRYS_AES_error.h    : %
   *  State           :  %state%
   *  Creation date   :  Sun Nov 21 11:07:08 2004
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This module containes the definitions of the CRYS AES errors.
   *
   *  \version CRYS_AES_error.h#1:incl:1
   *  \author adams
   */




/************************ Defines ******************************/

/* CRYS COMMON module errors. Base address - 0x00F00D00 */

#define CRYS_COMMON_INIT_HW_SEM_CREATION_FAILURE    (CRYS_COMMON_MODULE_ERROR_BASE + 0x0UL)
#define CRYS_COMMON_GLOBAL_DATA_INPUT_SIZE_ERROR    (CRYS_COMMON_MODULE_ERROR_BASE + 0x1UL)
#define CRYS_COMMON_GLOBAL_DATA_INPUT_ID_ERROR      (CRYS_COMMON_MODULE_ERROR_BASE + 0x2UL)
#define CRYS_COMMON_GLOBAL_DATA_ZERO_ERROR          (CRYS_COMMON_MODULE_ERROR_BASE + 0x3UL)
#define CRYS_COMMON_DATA_IN_POINTER_INVALID_ERROR   (CRYS_COMMON_MODULE_ERROR_BASE + 0x4UL) 	
#define CRYS_COMMON_DATA_SIZE_ILLEGAL		    (CRYS_COMMON_MODULE_ERROR_BASE + 0x5UL)
#define CRYS_COMMON_DATA_OUT_DATA_IN_OVERLAP_ERROR  (CRYS_COMMON_MODULE_ERROR_BASE + 0x6UL)
#define CRYS_COMMON_DATA_OUT_POINTER_INVALID_ERROR  (CRYS_COMMON_MODULE_ERROR_BASE + 0x7UL)
#define CRYS_SELF_TEST_FAIL_ERROR 		    (CRYS_COMMON_MODULE_ERROR_BASE + 0x8UL)
#define CRYS_COMMON_OUTPUT_BUFF_SIZE_ILLEGAL	    (CRYS_COMMON_MODULE_ERROR_BASE + 0x9UL)

#define CRYS_COMMON_TST_UTIL_CHUNK_SIZE_SMALL_ERROR (CRYS_COMMON_MODULE_ERROR_BASE + 0x10UL)
#define CRYS_COMMON_ERROR_IN_SAVING_LLI_DATA_ERROR  (CRYS_COMMON_MODULE_ERROR_BASE + 0x11UL)


#define CRYS_COMMON_TST_UTIL_LLI_ENTRY_SIZE_TOO_SMALL_ERROR   (CRYS_COMMON_MODULE_ERROR_BASE + 0x12UL)
#define CRYS_COMMON_TST_CSI_DATA_SIZE_EXCEED_ERROR            (CRYS_COMMON_MODULE_ERROR_BASE + 0x13UL)
#define CRYS_COMMON_TST_CSI_MODULE_ID_OUT_OF_RANGE            (CRYS_COMMON_MODULE_ERROR_BASE + 0x14UL)
#define CRYS_COMMON_TST_CSI_MEMORY_MAPPING_ERROR              (CRYS_COMMON_MODULE_ERROR_BASE + 0x15UL)

#define CRYS_COMMON_TERM_HW_SEM_DELETE_FAILURE                (CRYS_COMMON_MODULE_ERROR_BASE + 0x16UL)

#define CRYS_COMMON_TST_UTIL_NOT_INTEGER_CHAR_ERROR           (CRYS_COMMON_MODULE_ERROR_BASE + 0x17UL)
#define CRYS_COMMON_TST_UTIL_BUFFER_IS_SMALL_ERROR            (CRYS_COMMON_MODULE_ERROR_BASE + 0x18UL)
#define CRYS_COMMON_POINTER_NOT_ALIGNED_ERROR                 (CRYS_COMMON_MODULE_ERROR_BASE + 0x19UL)


/************************ Enums ********************************/


/************************ Typedefs  ****************************/


/************************ Structs  ******************************/


/************************ Public Variables **********************/


/************************ Public Functions **********************/

#ifdef __cplusplus
}
#endif

#endif


