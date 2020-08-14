/******************************************************************************

          Copyright (C), 2001-2015, Hisilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : dcas_image.h
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

#ifndef __DCAS_IMAGE_H__
#define __DCAS_IMAGE_H__

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

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup     DCAS_IMAGE */
/** @{ */  /** <!-- [DCAS_IMAGE] */

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup     DCAS_IMAGE */
/** @{ */  /** <!-- [DCAS_IMAGE] */

/****** Enumeration definition ************/

typedef enum tagSSA_VERIFYMODE_E
{
    SSA_VERIFYMODE_COMPLETE = 0,
    SSA_VERIFYMODE_SAMPLING,
    SSA_VERIFYMODE_BUTT

} SSA_VERIFYMODE_E;

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup     DCAS_IMAGE */
/** @{ */  /** <!-- [DCAS_IMAGE] */

HI_S32 SSA_IMG_Verify
(
    SSA_FLASH_OPENPARAM_S*  pstDataParam,
    SSA_FLASH_OPENPARAM_S*  pstSignParam,
    HI_U8*                  pu8DataBuffer,
    HI_U32                  u32DataBufferSize,
    HI_U8*                  pu8SignBuffer,
    HI_U32                  u32SignBufferSize,
    SSA_VERIFYMODE_E        enVerifyMode
);

HI_S32 SSA_IMG_Read
(
    SSA_FLASH_OPENPARAM_S*  pstDataParam,
    SSA_FLASH_OPENPARAM_S*  pstSignParam,
    HI_U8*                  pu8DataBuffer,
    HI_U32                  u32DataBufferSize,
    HI_U8*                  pu8SignBuffer,
    HI_U32                  u32SignBufferSize
);


HI_BOOL SSA_IMG_Clone
(
    SSA_FLASH_OPENPARAM_S*  pstParamDst,
    SSA_FLASH_OPENPARAM_S*  pstParamSrc,
    HI_U8*                  pu8DataBuffer,
    HI_U32                  u32DataBufferSize
);

HI_BOOL SSA_IMG_Compare(HI_U8* pu8SignBuffer1, HI_U8* pu8SignBuffer2);

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__DCAS_IMAGE_H__*/

