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

 
#ifndef CRYS_SELF_TEST_ERROR_H
#define CRYS_SELF_TEST_ERROR_H
  
/*
   *  Object % crys_self_test_error.h    : %
   *  State           :  %state%
   *  Creation date   :  07/03/ 2012 Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This module containes the definitions of the crys "self test"
   *         errors.
   *
   *  \version crys_self_test_error.h#1:incl:1
   *  \author G.Koltun
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

/****************************************************************************************
* CRYS SELF TEST MODULE ERRORS 							*                     
***************************************************************************************/
#define CRYS_SELF_TEST_MSG_GENERAL_RPC_A_ERROR    (CRYS_SELF_TEST_MODULE_ERROR_BASE + 0UL)
#define CRYS_SELF_TEST_MSG_GENERAL_RPC_B_ERROR    (CRYS_SELF_TEST_MODULE_ERROR_BASE + 1UL)
#define CRYS_SELF_TEST_MSG_GENERAL_RPC_C_ERROR    (CRYS_SELF_TEST_MODULE_ERROR_BASE + 2UL)



















/************************ Enums ********************************/

/************************ Typedefs  ****************************/

/************************ Structs  ******************************/

/************************ Public Variables **********************/

/************************ Public Functions **********************/

#ifdef __cplusplus
}
#endif

#endif


