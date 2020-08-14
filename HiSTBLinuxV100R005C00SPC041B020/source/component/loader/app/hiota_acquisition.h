/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : hiota_acquisition.h
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

#ifndef __HIOTA_ACQUISITION_H__
#define __HIOTA_ACQUISITION_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "protocol_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup      HIOTA_ACQUISITION */
/** @{ */  /** <!-- [HIOTA_ACQUISITION] */

/****** Macro switch definition ***********/

/****** Macro constants definition ********/

/****** Macro Functions definition ********/

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      HIOTA_ACQUISITION */
/** @{ */  /** <!-- [HIOTA_ACQUISITION] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/
/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      HIOTA_ACQUISITION */
/** @{ */  /** <!-- [HIOTA_ACQUISITION] */

HI_S32 HIOTA_ACQ_Init
(
    HI_LOADER_PARAM_S*      pstLoaderParam,
    PROT_DATA_CALLBACK      pfnDataCallback,
    PROT_PROCESS_CALLBACK   pfnProcessCallback
);

HI_S32 HIOTA_ACQ_DeInit(HI_VOID);

HI_S32 HIOTA_ACQ_GetUpgradeInfo(HIOTA_UPGRADE_INFO_S* pstUpgradeInfo);

HI_S32 HIOTA_ACQ_GetUpgradeData
(
    HI_U8*                  pu8MemBuff,
    HI_U32                  u32MemSize,
    HIOTA_UPGRADE_INFO_S*   pstUpgradeInfo
);

/** @} */  /** <!-- ==== API Declaration End ==== */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__HIOTA_ACQUISITION_H__*/
