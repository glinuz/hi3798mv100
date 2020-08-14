/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : download_fs.h
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

#ifndef __DOWNLOAD_FS_H__
#define __DOWNLOAD_FS_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_type.h"
#include "loader_dbinfo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup      DOWNLOAD_FS */
/** @{ */  /** <!-- [DOWNLOAD_FS] */

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      DOWNLOAD_FS */
/** @{ */  /** <!-- [DOWNLOAD_FS] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/
/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      DOWNLOAD_FS */
/** @{ */  /** <!-- [DOWNLOAD_FS] */


/**
 \brief initialize filesystem download mode.
 \attention \n
 \param[in] pstLoaderParam:
 \retval ::HI_SUCCESS Success
 \retval ::ERROR CODE Failure
 */

HI_S32 DOWNLOAD_FS_Init(HI_LOADER_PARAM_S* pstLoaderParam);

/**
 \brief deinitialize filesystem download mode.
 \attention \n
 \retval ::HI_SUCCESS Success
 \retval ::ERROR CODE Failure
 */
HI_S32 DOWNLOAD_FS_DeInit(HI_VOID);

/**
 \brief Get upgrade data from filesystem.
 \attention \n
 \param[in] pbuffer: Data buffer, allocate by caller
 \param[in] u32BufferSize: Data buffer's size
 \param[out] pu32DataLen: the really length of data
 \retval ::HI_SUCCESS Success
 \retval ::ERROR CODE Failure
 */
HI_S32 DOWNLOAD_FS_Getdata(HI_VOID* pbuffer, HI_U32 u32BufferSize, HI_U32* pu32DataLen);

/**
 \brief Get size of the upgrade image.
 \attention \n
 \param[out] pu32FileSize: Size of the upgrade image
 \retval ::HI_SUCCESS Success
 \retval ::ERROR CODE Failure
 */
HI_S32 DOWNLOAD_FS_GetFileSize(HI_U32* pu32FileSize);

HI_S32 DOWNLOAD_FS_PartitionReset(HI_VOID);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__DOWNLOAD_FS_H__*/
