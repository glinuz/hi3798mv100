/******************************************************************************
                              Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : download_ota.h
Version       : Initial Draft
Author        : Hisilicon multimedia Hardware group l50549
Created       : 2007/08/20
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#ifndef __DOWNLOAD_OTA_H__
#define __DOWNLOAD_OTA_H__

#include "loader_dbinfo.h"
#include "hi_unf_demux.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_1*/
/** @{ */  /** <!?a[ota download data structure] */

#define HI_SET_MASK(stFilterAttr, u32Index, u8Match)        \
    {                                                       \
        stFilterAttr.au8Match[u32Index] = (u8Match);        \
        stFilterAttr.au8Mask[u32Index]  = 0x00;             \
        stFilterAttr.au8Negate[u32Index] = 0x00;            \
        if (stFilterAttr.u32FilterDepth < ((u32Index) + 1)) \
        {                                                   \
            stFilterAttr.u32FilterDepth = (u32Index) + 1;   \
        }                                                   \
    }

/**Definition of button types for infrared response*/

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      H_1_1 */
/** @{ */  /** <! [ota download API] */

HI_S32 DOWNLOAD_OTA_Init(HI_LOADER_PARAM_S* pstLoaderParam);

/**
 \brief deinitialize OTA download mode.
 \attention \n
 \retval ::HI_SUCCESS : get data success
 \retval ::HI_FAILURE : failed to get data
 */
HI_S32 DOWNLOAD_OTA_DeInit(HI_VOID);

/**
 \brief Get upgrade data through OTA, usually section data.
 \attention \n
 \param[in] pu8Buffer: data buffer, allocate by caller
 \param[in] u64BufferSize: the max expected size, it must be samller than the buffer length
 \param[out] pu64DataLen: the really length of data
 \retval ::HI_SUCCESS : get data success
 \retval ::HI_FAILURE : failed to get data
 */
HI_S32 DOWNLOAD_OTA_GetData(HI_VOID* pbuffer, HI_U32 u32BufferSize, HI_U32* pu32DataLen);

/**
 \brief Set fillter attributes to acquire data through OTA.
 \attention \n
 \param[in] pstFilterAttr: filter attributs
 \retval ::HI_SUCCESS : set fillter success
 \retval ::HI_FAILURE : set fillter failed
 */
HI_S32 DOWNLOAD_OTA_SetFilter(HI_UNF_DMX_FILTER_ATTR_S* pstFilterAttr);

HI_S32 DOWNLOAD_OTA_GetPid(HI_LOADER_PARAM_OTA_S* pstUpgradeParam, HI_U16* pu16Pid);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__DOWNLOAD_OTA_H__*/
