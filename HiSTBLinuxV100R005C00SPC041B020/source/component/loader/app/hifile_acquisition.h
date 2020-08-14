/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : hifile_acquisition.h
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

#ifndef __HIFILE_ACQUISITION_H__
#define __HIFILE_ACQUISITION_H__

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
/** \addtogroup      HIFILE_ACQUISITION */
/** @{ */  /** <!-- [HIFILE_ACQUISITION] */

/****** Macro switch definition ***********/

/****** Macro constants definition ********/

/****** Macro Functions definition ********/

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      HIFILE_ACQUISITION */
/** @{ */  /** <!-- [HIFILE_ACQUISITION] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      HIFILE_ACQUISITION */
/** @{ */  /** <!-- [HIFILE_ACQUISITION] */

HI_S32 HIFILE_ACQ_Init
(
    HI_LOADER_PARAM_S*      pstLoaderParam,
    PROT_DATA_CALLBACK      pfnDataCallback,
    PROT_PROCESS_CALLBACK   pfnProcessCallback
);

HI_S32 HIFILE_ACQ_DeInit(HI_VOID);

HI_S32 HIFILE_ACQ_GetUpgradeInfo(HIFILE_DOCUMENT_S* pstUpgradeInfo);

HI_S32 HIFILE_ACQ_GetUpgradeData
(
    HI_U8*                  pu8MemBuff,
    HI_U32                  u32MemSize,
    HIFILE_DOCUMENT_S*      pstUpgradeInfo
);

/** @} */  /** <!-- ==== API Declaration End ==== */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__HIFILE_ACQUISITION_H__*/
