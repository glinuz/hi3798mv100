/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ssd_router.h
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

#ifndef __SSD_ROUTER_H__
#define __SSD_ROUTER_H__

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
/** \addtogroup      ADVCA_SSD */
/** @{ */  /** <!-- [ADVCA_SSD] */

/****** Macro switch definition ***********/


/****** Macro constants definition ********/

/****** Macro Functions definition ********/

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      ADVCA_SSD */
/** @{ */  /** <!-- [ADVCA_SSD] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      ADVCA_SSD */
/** @{ */  /** <!-- [ADVCA_SSD] */

HI_S32 ADVCA_SSD_Init(HI_VOID);

HI_S32 ADVCA_SSD_DeInit(HI_VOID);

HI_S32 ADVCA_SSD_WholeImageVerify
(
    HI_U8*      pu8ImageData,
    HI_U32      u32ImageSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize
);

HI_S32 ADVCA_SSD_PartitionImageVerify
(
    HI_U8*      pu8PartitionName,
    HI_U64      u64FlashStartAddr,
    HI_U8*      pu8ImageData,
    HI_U32      u32ImageSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize,
    HI_U32*     pu32ImageVersion,
    HI_BOOL*    pbNeedUpgrade
);


/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__SSD_ROUTER_H__*/
