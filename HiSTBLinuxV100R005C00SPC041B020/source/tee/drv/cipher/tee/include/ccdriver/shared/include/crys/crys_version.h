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

 
 
#ifndef CRYS_version_H
#define CRYS_version_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */
#include "dx_pal_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

  /*
   *  Object %name    : %
   *  State           :  %state%
   *  Creation date   :  Tue Mar 22 09:35:02 2005
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief Get CRYS version information, including underlying engines.
   *
   *  \version CRYS_version.h#1:incl:1
   *  \author adams
   */

/************************ Defines ******************************/

/* @brief the version structure definition */
typedef struct
{
   char compName[4];
   char type;
   uint32_t major;
   uint32_t minor;
   uint32_t sub;
   uint32_t internal;   

}CRYS_ComponentVersion_t;

typedef struct
{
   CRYS_ComponentVersion_t CRYS_Version;
   CRYS_ComponentVersion_t LLF_AES_Version;
   CRYS_ComponentVersion_t LLF_HASH_Version;
   CRYS_ComponentVersion_t LLF_DES_Version;
   CRYS_ComponentVersion_t LLF_PKI_Version;
   CRYS_ComponentVersion_t LLF_RND_Version;
   CRYS_ComponentVersion_t LLF_ECPKI_Version;
   CRYS_ComponentVersion_t LLF_RC4_Version;
   
}CRYS_Version_t;

/************************ Enums ********************************/


/************************ Typedefs  ****************************/


/************************ Structs  ******************************/


/************************ Public Variables **********************/


/************************ Public Functions **********************/

/**
 * @brief This function returns the CRYS version.
 *
 * The version contains the following fields:
 *
 * component string - a string describing the nature of the release.
 * release type: 'D' - development, 'A' - alpha (passed to QA) , 
 *                'R' - release, after QA testing.
 *
 * major, minor, sub, internal - the release digits.
 * 
 * each component: CRYS, LLF machines has this structure. 
 *
 * @param[in] version_ptr - a pointer to the version structure.
 *
 */

 void  CRYS_GetVersion(CRYS_Version_t *version_ptr);

#ifdef __cplusplus
}
#endif

#endif


