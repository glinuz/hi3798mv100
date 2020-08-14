/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : loader_cipher.h
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

#ifndef __LOADER_CIPHER_H__
#define __LOADER_CIPHER_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_type.h"

/*************************** Macro Definition *********************************/
/** \addtogroup     LOADER_CIPHER */
/** @{ */  /** <!-- [LOADER_CIPHER] */

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup     LOADER_CIPHER */
/** @{ */  /** <!-- [LOADER_CIPHER] */

/****** Enumeration definition ************/

#define LDDB_D_CIPHER_BYTEWIDTH    (16)

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup     LOADER_CIPHER */
/** @{ */  /** <!-- [LOADER_CIPHER] */
HI_S32 LDDB_CIPHER_Init(HI_VOID);

HI_S32 LDDB_CIPHER_DeInit(HI_VOID);

HI_S32 LDDB_CIPHER_BlockVerify(HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue);

HI_S32 LDDB_CIPHER_BlockSignature(HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue);

HI_S32 LDDB_CIPHER_BlockEncrypt(HI_U8* pu8BlockData, HI_U32 u32BlockSize);

HI_S32 LDDB_CIPHER_BlockDecrypt(HI_U8* pu8BlockData, HI_U32 u32BlockSize);


/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif

