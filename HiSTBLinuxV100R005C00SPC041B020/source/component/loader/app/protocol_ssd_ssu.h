/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : protocol_ssd_ssu.h
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

#ifndef __PROTOCOL_SSD_SSU_H__
#define __PROTOCOL_SSD_SSU_H__

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
/** \addtogroup      PROTOCOL_SSD_SSU */
/** @{ */  /** <!-- [PROTOCOL_SSD_SSU] */

/****** Macro switch definition ***********/

/****** Macro constants definition ********/

/****** Macro Functions definition ********/

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      PROTOCOL_SSD_SSU */
/** @{ */  /** <!-- [PROTOCOL_SSD_SSU] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      PROTOCOL_SSD_SSU */
/** @{ */  /** <!-- [PROTOCOL_SSD_SSU] */

/**
 \brief initialize protocol.
 \attention \n
 \param[in] pstLoaderParam: Upgrade parameter
 \param[in] pfnDataCallback: Upgrade data callback func
 \param[in] pfnProgressCallback: Download Progress callback func
 \retval ::HI_SUCCESS : initialize success
 \retval ::Error Code : initialize failed
 */
HI_S32 PROT_SSD_SSU_Init
(
    HI_LOADER_PARAM_S*      pstLoaderParam,
    PROT_DATA_CALLBACK      pfnDataCallback,
    PROT_PROCESS_CALLBACK   pfnProgressCallback
);

/**
 \brief deinitialize protocol.
 \attention \n
 \retval ::HI_SUCCESS : deinitialize success
 \retval ::Error Code : deinitialize failed
 */
HI_S32 PROT_SSD_SSU_DeInit(HI_VOID);

/**
 \brief get the version infomation of the upgrade image.
 \attention \n
 \param[out] pstVersionInfo: the version information of the image
 \retval ::HI_SUCCESS : success
 \retval ::Error Code : failed
 */
HI_S32 PROT_SSD_SSU_GetVersionInfo(PROT_VERSION_INFO_S* pstVersionInfo);

/**
 \brief get the partition infomation of the upgrade image.
 \attention \n
 \param[in]  pstBuffer:   The buffer for storing partition information.
 \param[in]  u32BufNum:   The size of the buffer for storing partition information.
 \param[out] pu32PartNum: The Actual number of partitions
 \retval ::HI_SUCCESS : success
 \retval ::Error Code : failed
 */
HI_S32 PROT_SSD_SSU_GetPartitionInfo
(
    PROT_PARTITION_INFO_S*   pstBuffer,
    HI_U32                   u32BufNum,
    HI_U32*                  pu32PartNum
);

/**
 \brief receive upgrade data, and call datacallback to notice caller the data.
 \attention \n
 \retval ::HI_SUCCESS : success
 \retval ::Error Code : failed
 */
HI_S32 PROT_SSD_SSU_Process(HI_VOID);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__PROTOCOL_SSD_SSU_H__*/
