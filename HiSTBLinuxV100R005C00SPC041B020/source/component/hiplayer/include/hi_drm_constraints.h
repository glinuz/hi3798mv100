/*
 * hi_drm_constraints.h
 *
 *  Created on: Jul 31, 2012
 *      Author: zhangyi
 */

#ifndef HI_DRM_CONSTRAINTS_H_
#define HI_DRM_CONSTRAINTS_H_

#include <hi_drm_common.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * \breif The following variables are replica of android.drm.DrmStore.ConstraintsColumns
 * Any changes should also be incorporated with Java Layer as well
 */

/**
 * \breif The max repeat count
 */
#define HI_DRM_MAX_REPEAT_COUNT "max_repeat_count"
/**
 * \breif The remaining repeat count
 */
#define HI_DRM_REMAINING_REPEAT_COUNT "remaining_repeat_count"

/**
 * \breif The time before which the protected file can not be played/viewed
 */
#define HI_DRM_LICENSE_START_TIME "license_start_time"

/**
 * \breif The time after which the protected file can not be played/viewed
 */
#define HI_DRM_LICENSE_EXPIRY_TIME "license_expiry_time"

/**
 * \breif The available time for license
 */
#define HI_DRM_LICENSE_AVAILABLE_TIME "license_available_time"

/**
 * \breif The data stream for extended metadata
 */
#define HI_DRM_EXTENDED_METADATA "extended_metadata"

typedef const struct hiDRM_CONSTRAINTS_S * const * HI_DRM_CONSTRAINTS_PPTR;

/**
 * \breif Returns the number of constraints contained in this instance
 *
 * \return Number of constraints
 */
HI_S32 HI_DRM_CONSTRAINTS_GetCount(HI_DRM_CONSTRAINTS_PPTR ppSelf);

/**
 * \breif Adds constraint information as <key, value> pair to this instance
 *
 * \param[in] key Key to add
 * \param[in] value Value to add
 * \return Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_CONSTRAINTS_Put(HI_DRM_CONSTRAINTS_PPTR ppSelf, const HI_CHAR* key, const HI_CHAR* value);

/**
 * \breif Retrieves the value of given key
 *
 * \param key Key whose value to be retrieved
 * \return The value
 */
const HI_CHAR* HI_DRM_CONSTRAINTS_Get(HI_DRM_CONSTRAINTS_PPTR ppSelf, const HI_CHAR* key);

/**
 * \breif Retrieves the value as byte array of given key
 * \param key Key whose value to be retrieved as byte array
 * \return The byte array value
 */
const HI_CHAR* HI_DRM_CONSTRAINTS_GetAsByteArray(HI_DRM_CONSTRAINTS_PPTR ppSelf, const HI_CHAR* key);

/**
 * \breif Returns KeyIterator object to walk through the keys associated with this instance
 *
 * \return KeyIterator object
 */
HI_ITERATOR_S HI_DRM_CONSTRAINTS_KeyIterator(HI_DRM_CONSTRAINTS_PPTR ppSelf);

/**
 * \breif Returns Iterator object to walk through the values associated with this instance
 *
 * \return Iterator object
 */
HI_ITERATOR_S HI_DRM_CONSTRAINTS_Iterator(HI_DRM_CONSTRAINTS_PPTR ppSelf);

/**
 * \breif Create a DRM Constraints object.
 *
 * \param[in] pppSelf Handle of DRM Constraints object
 *
 * \return Returns HI_SUCCESS for success, HI_FAILURE for failure
 */
HI_S32 HI_DRM_CONSTRAINTS_Create(HI_DRM_CONSTRAINTS_PPTR* pppSelf);

/**
 * \breif Destroy the DRM Constraints object.
 *
 * \param[in] pppSelf Handle of DRM Constraints object
 *
 * \return None
 */
HI_VOID HI_DRM_CONSTRAINTS_Destroy(HI_DRM_CONSTRAINTS_PPTR* pppSelf);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* HI_DRM_CONSTRAINTS_H_ */
