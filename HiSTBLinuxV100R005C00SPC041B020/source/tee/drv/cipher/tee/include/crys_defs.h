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

 
 
#ifndef _1_CRYS_Defs_h_H
#define _1_CRYS_Defs_h_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */



#ifdef __cplusplus
extern "C"
{
#endif

  /*
   *  Object %name    : %
   *  State           :  %state%
   *  Creation date   :  Mon Jan 03 18:37:21 2005
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief A brief description of this module
   *
   *  \version crys_defs.h#1:incl:1
   *  \author ohads
   */
 

/*whether needed to export CRYS APIs for firmware testing*/
#ifdef CRYS_EXPORT_APIS_FOR_DLL
#define CEXPORT_C EXPORT_C
#define CIMPORT_C IMPORT_C
#else
#define CEXPORT_C
#define CIMPORT_C
#endif

/************************ Enums ********************************/

/* Defines the enum that is used for specifying whether or not to perform 
 * a decrypt operation when performing the AES operation mode on the Context
 */


/* Defines the enum that is used for specifying whether or not to perform 
 * a decrypt operation when performing the AES operation mode on the Context
 */
typedef enum
{
   AES_DECRYPT_CONTEXT = 0,
   AES_DONT_DECRYPT_CONTEXT = 1,
/*  AES_ENCRYPT_RELEASE_CONTEXT = 2*/
   
   CRYS_AES_CONTEXTS_flagLast = 0x7FFFFFFF,

}CRYS_AES_CONTEXTS_flag;
/************************ Enums ********************************/


/************************ Typedefs  ****************************/


/************************ Structs  ******************************/


/************************ Public Variables **********************/


/************************ Public Functions **********************/

#ifdef __cplusplus
}
#endif

#endif
