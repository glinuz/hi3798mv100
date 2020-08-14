/******************************************************************************

          Copyright (C), 2001-2015, Hisilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : dcas_verify.h
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

#ifndef __DCAS_VERIFY_H__
#define __DCAS_VERIFY_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_type.h"
#include "dcas_flash.h"
#include "dcas_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup     DCAS_VERIFY */
/** @{ */  /** <!-- [DCAS_VERIFY] */

#define SSA_CHECK_TAILSECTION(pu8Section)   (0 == memcmp(pu8Section, SSA_DCAS_STR_MAGICNUMBER, SSA_DCAS_LEN_MAGICNUMBER))

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup     DCAS_VERIFY */
/** @{ */  /** <!-- [DCAS_VERIFY] */

/****** Enumeration definition ************/

/****** Structure definition **************/

typedef struct tagSSA_SECTIONTAIL_S
{
    HI_U8   au8MagicNumber[SSA_DCAS_LEN_MAGICNUMBER];               /**< Magic Number: "Hisilicon_ADVCA_ImgHead_MagicNum" */
    HI_U8   au8Version[SSA_DCAS_LEN_VERSION];                       /**< version: "V000 0003" */
    HI_U32  u32CreateDay;                                           /**< yyyymmdd */
    HI_U32  u32CreateTime;                                          /**< hhmmss */
    HI_U32  u32HeadLength;                                          /**< The following data size */
    HI_U32  u32SignedDataLength;                                    /**< signed data length */
    HI_U32  u32IsYaffsImage;                                        /**< Yaffsr image need to specail read-out, No use */
    HI_U32  u32IsConfigNandBlock;                                   /**< Yaffsr image need to specail read-out, No use */
    HI_U32  u32NandBlockType;                                       /**< Yaffsr image need to specail read-out, No use */
    HI_U32  u32IsNeedEncrypt;                                       /**< if "1", code need to be encrypted. */
    HI_U32  u32IsEncrypted;                                         /**< if "1", code has encrypted. */
    HI_U32  u32HashType;                                            /**< if "0", au8Sha save sha1 of code, if "1",  au8Sha save sha256 of code */
    HI_U8   au8Sha[SSA_DCAS_LEN_SHA_VALUE];                         /**< SHA value */
    HI_U32  u32SignatureLen;                                        /**< Actural Signature length */
    HI_U8   au8Signature[SSA_DCAS_LEN_SIGNATURE];                   /**< Max length:0x100 */
    HI_U8   au8OrignalImagePath[SSA_DCAS_LEN_ORIGNAL_IMAGE_PATH];   /**< Max length: */
    HI_U8   au8RSAPrivateKeyPath[SSA_DCAS_LEN_RSA_PRIVATE_KEY_PATH];/**< Max length:0x100 */
    HI_U32  u32CurrentsectionID;                                    /**< Begin with 0 */
    HI_U32  u32SectionSize;                                         /**< Section size */
    HI_U32  u32Totalsection;                                        /**< Total Section Count >= 1 */
    HI_U32  u32CRC32;                                               /**< CRC32 value */
} SSA_SECTIONTAIL_S;

/****** Union definition ******************/

/****** Global variable declaration *******/

/*nand and SPI max flash block size*/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup     DCAS_VERIFY */
/** @{ */  /** <!-- [DCAS_VERIFY] */

HI_S32 SSA_ConfigExternKey(SSA_FLASH_OPENPARAM_S* pstOpenParam, HI_U8* pu8Buffer, HI_U32 u32Size);

HI_S32 SSA_VerifySectionTail(HI_U8* pu8PrevTail, HI_U8* pu8CurrTail);

HI_S32 SSA_VerifySectionData(HI_U8* pu8Data, HI_U8* pu8Tail);

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__DCAS_VERIFY_H__*/

