/******************************************************************************

          Copyright (C), 2001-2015, Hisilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : dcas_cipher.h
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2016-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2016-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

#ifndef __DCAS_CIPHER_H__
#define __DCAS_CIPHER_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup     DCAS_CIPHER */
/** @{ */  /** <!-- [DCAS_CIPHER] */

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup     DCAS_CIPHER */
/** @{ */  /** <!-- [DCAS_CIPHER] */

/****** Enumeration definition ************/

typedef enum tagSSA_HASH_TYPE_E
{
    SSA_HASH_TYPE_1 = 0,
    SSA_HASH_TYPE_2,
    SSA_HASH_TYPE_BUTT

} SSA_HASH_TYPE_E;


/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/*nand and SPI max flash block size*/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup     DCAS_CIPHER */
/** @{ */  /** <!-- [DCAS_CIPHER] */

HI_S32 SSA_CIPHER_Init(HI_VOID);

HI_S32 SSA_CIPHER_DeInit(HI_VOID);

HI_S32 SSA_CIPHER_HashVerify(SSA_HASH_TYPE_E enHashType, HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8Value);

HI_S32 SSA_CIPHER_RSAVerify(HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8Signature, HI_U32 u32SignatureSize);

HI_S32 SSA_CIPHER_SetRSAPublicKey(HI_U8* pu8RsaData, HI_U32 u32Length);

HI_S32 SSA_CIPHER_GetRandomNum(HI_U32* pu32Random);

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__DCAS_CIPHER_H__*/

