/***********************************************************
*                      Copyright    , 2009-2014, Hisilicon Tech. Co., Ltd.
*                                   ALL RIGHTS RESERVED
* FileName:  download_ip.h
* Description:
*
* History:
* Version   Date          Author         DefectNum      Description
* main\1    2009-11-5     y00107738      NULL
************************************************************/
#ifndef __UPGRD_IP_H__
#define __UPGRD_IP_H__

/**include header files*/
#include "hi_type.h"
#include "loader_dbinfo.h"

/** C++ support */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /**end of  __cplusplus */

/******************************* API declaration *****************************/
/** \addtogroup      H_1_4 */
/** @{ */  /** <!-- ip upgrade interface */

/**
\brief initialize IP download mode.
\attention \n
\param[in] pstLoaderParam:
\retval ::HI_SUCCESS : get data success
\retval ::HI_FAILURE : failed to get data
*/
// OTA
HI_S32 DOWNLOAD_IP_Init(HI_LOADER_PARAM_S* pstLoaderParam);

/**
\brief deinitialize USB download mode.
\attention \n
\retval ::HI_SUCCESS : get data success
\retval ::HI_FAILURE : failed to get data
*/
HI_S32 DOWNLOAD_IP_DeInit(HI_VOID);

/**
\brief Get upgrade data through USB.
\attention \n
\param[in] pbuffer: data buffer, allocate by caller
\param[in] size: the max expected size, it must be samller than the buffer length
\param[out] outlen: the really length of data
\retval ::HI_SUCCESS : get data success
\retval ::HI_FAILURE : failed to get data
*/
HI_S32 DOWNLOAD_IP_Getdata(HI_VOID* pbuffer, HI_U32 size, HI_U32* outlen);

HI_S32 DOWNLOAD_IP_GetFileSize(HI_U32* pu32FileSize);


/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  /* __cplusplus */
#endif /*__UPGRD_IP_H__*/




