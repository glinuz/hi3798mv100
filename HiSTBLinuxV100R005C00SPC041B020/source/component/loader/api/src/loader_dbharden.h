/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : loader_dbharden.h
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

#ifndef __LOADER_DBHARDEN_H__
#define __LOADER_DBHARDEN_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_type.h"
#include "loader_cipher.h"
#include "loader_dbinfo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/*************************** Macro Definition *********************************/
/** \addtogroup      LOADER_DBHARDEN */
/** @{ */  /** <!-- [LOADER_DBHARDEN] */

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      LOADER_DBHARDEN */
/** @{ */  /** <!-- [LOADER_DBHARDEN] */

/****** Enumeration definition ************/

#ifdef HI_ADVCA_SUPPORT
#define LDDB_D_HARDEN_BYTEWIDTH     LDDB_D_CIPHER_BYTEWIDTH
#else
#define LDDB_D_HARDEN_BYTEWIDTH     (1)
#endif


/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      LOADER_DBHARDEN */
/** @{ */  /** <!-- [LOADER_DBHARDEN] */
HI_S32 LDDB_HARDEN_Init(HI_VOID);

HI_S32 LDDB_HARDEN_DeInit(HI_VOID);

HI_S32 LDDB_HARDEN_DataVerify(LDDB_HARDEN_TYPE_E enVerifyType, HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue);

HI_S32 LDDB_HARDEN_DataSignature(LDDB_HARDEN_TYPE_E enVerifyType, HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue);

HI_S32 LDDB_HARDEN_DataEncrypt(LDDB_HARDEN_TYPE_E enVerifyType, HI_U8* pu8Data, HI_U32 u32Size);

HI_S32 LDDB_HARDEN_DataDecrypt(LDDB_HARDEN_TYPE_E enVerifyType, HI_U8* pu8Data, HI_U32 u32Size);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__LOADER_DBHARDEN_H__*/

