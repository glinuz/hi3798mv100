/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : loader_verify.h
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

#ifndef __LOADER_VERIFY_H__
#define __LOADER_VERIFY_H__

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
/** \addtogroup     LOADER_VERIFY */
/** @{ */  /** <!-- [LOADER_VERIFY] */

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup     LOADER_VERIFY */
/** @{ */  /** <!-- [LOADER_VERIFY] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup     LOADER_VERIFY */
/** @{ */  /** <!-- [LOADER_VERIFY] */

#define LOADER_GetImageDataByName(pu8PartName, pu8Buffer, u32BufferSize, pu32DataOffset, pu32DataSize) \
    LOADER_GetImageData(pu8PartName, 0, 0, pu8Buffer, u32BufferSize, pu32DataOffset, pu32DataSize)

#define LOADER_GetImageDataByAddr(u32Address, u32Length, pu8Buffer, u32BufferSize, pu32DataOffset, pu32DataSize) \
    LOADER_GetImageData(HI_NULL_PTR, u32Address, u32Length, pu8Buffer, u32BufferSize, pu32DataOffset, pu32DataSize)

HI_S32 LOADER_GetImageData
(
    HI_CHAR*    pcPartName,
    HI_U32      u32Address,
    HI_U32      u32Length,
    HI_U8*      pu8Buffer,
    HI_U32      u32BufferSize,
    HI_U32*     pu32DataOffset,
    HI_U32*     pu32DataSize
);

HI_BOOL LOADER_CpyImageData
(
    HI_CHAR*    pcPartNameDst,
    HI_CHAR*    pcPartNameSrc,
    HI_U8*      pu8Buffer,
    HI_U32      u32BufferSize
);

HI_BOOL LOADER_CmpImageData
(
    HI_U8*      pu8ImageData1,
    HI_U8*      pu8ImageData2,
    HI_U32      u32DataOffset,
    HI_U32      u32DataSize
);

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif

