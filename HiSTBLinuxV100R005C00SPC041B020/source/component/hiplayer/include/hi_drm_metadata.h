/*
 * hi_drm_metadata.h
 *
 *  Created on: Aug 1, 2012
 *      Author: zhangyi
 */

#ifndef HI_DRM_METADATA_H_
#define HI_DRM_METADATA_H_

#include <hi_drm_common.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * \breif This is an utility class which contains the constraints information.
 *
 * As a result of DrmManagerClient::getMetadata(const String8*)
 * an instance of DrmMetadata would be returned.
 */
typedef const struct hiDRM_METADATA_S * const * HI_DRM_METADATA_PPTR;

/**
 * \breif Get <key, value> pair counts of this instance
 *
 * \param[in]
 * \return Returns counts of pairs
 */
HI_S32 HI_DRM_METADATA_GetCount(HI_DRM_METADATA_PPTR ppSelf);

/**
 * \breif Put <key, value> pair to this instance
 *
 * \param[in] key Key to add
 * \param[in] value Value to add
 * \return Returns HI_SUCCESS
 */
HI_S32 HI_DRM_METADATA_Put(HI_DRM_METADATA_PPTR ppSelf, const HI_CHAR* key, const HI_CHAR* value);

/**
 * \breif Get the value by key
 *
 * \param[in] key Key to search
 * \return Returns value pointer,
 * \note
 *      Incase of none, return NULL
 */
const HI_CHAR* HI_DRM_METADATA_Get(HI_DRM_METADATA_PPTR ppSelf, const HI_CHAR* key);

/**
 * \breif Get the value by key
 *
 * \param[in] key Key to search
 * \return Returns value pointer,
 * \note
 *      Incase of none, return NULL
 */
const HI_CHAR* HI_DRM_METADATA_GetAsByteArray(HI_DRM_METADATA_PPTR ppSelf, const HI_CHAR* key);

/**
 * \breif Get the iterator of key
 *
 * \param[in]
 * \return Returns iterator object,
 */
HI_ITERATOR_S HI_DRM_METADATA_KeyIterator(HI_DRM_METADATA_PPTR ppSelf);

/**
 * \breif Get the iterator of value
 *
 * \param[in]
 * \return Returns iterator object,
 */
HI_ITERATOR_S HI_DRM_METADATA_Iterator(HI_DRM_METADATA_PPTR ppSelf);

/**
 * \breif Create a DRM metadata object.
 *
 * \param[in] pppSelf Handle of DRM metadata object
 *
 * \return Returns HI_SUCCESS for success, HI_FAILURE for failure
 */
HI_S32 HI_DRM_METADATA_Create(HI_DRM_METADATA_PPTR* pppSelf);

/**
 * \breif Destroy the DRM metadata object.
 *
 * \param[in] pppSelf Handle of DRM metadata object
 *
 */
HI_VOID HI_DRM_METADATA_Destroy(HI_DRM_METADATA_PPTR* pppSelf);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* HI_DRM_METADATA_H_ */
