/*
 * hi_drm_rights.h
 *
 *  Created on: Aug 2, 2012
 *      Author: zhangyi
 */

#ifndef HI_DRM_RIGHTS_H_
#define HI_DRM_RIGHTS_H_

#include <hi_drm_common.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * \breif This is an utility class which wraps the license information which was
 * retrieved from the online DRM server.
 *
 * Caller can instantiate DrmRights by invoking DrmRights(const DrmBuffer&, String)
 * constructor by using the result of DrmManagerClient::ProcessDrmInfo(const DrmInfo*) API.
 * Caller can also instantiate DrmRights using the file path which contains rights information.
 *
 */
typedef const struct hiDRM_RIGHTS_S * const * HI_DRM_RIGHTS_PPTR;

/**
 * \breif Returns the rights data associated with this instance
 *
 * \return Rights data
 */
HI_DRM_BUFFER_S HI_DRM_RIGHTS_GetData(HI_DRM_RIGHTS_PPTR ppSelf);

/**
 * \breif Returns MIME type associated with this instance
 *
 * \return MIME type
 */
const HI_CHAR* HI_DRM_RIGHTS_GetMimeType(HI_DRM_RIGHTS_PPTR ppSelf);

/**
 * \breif Returns the account-id associated with this instance
 *
 * \return Account Id
 */
const HI_CHAR* HI_DRM_RIGHTS_GetAccountId(HI_DRM_RIGHTS_PPTR ppSelf);

/**
 * \breif Returns the subscription-id associated with this object
 *
 * \return Subscription Id
 */
const HI_CHAR* HI_DRM_RIGHTS_GetSubscriptionId(HI_DRM_RIGHTS_PPTR ppSelf);

/**
 * \breif Constructor for DrmRights
 *
 * \param[in] rightsFilePath Path of the file containing rights data
 * \param[in] mimeType MIME type
 * \param[in] accountId Account Id of the user
 * \param[in] subscriptionId Subscription Id of the user
 */
HI_S32 HI_DRM_RIGHTS_CreateFromFile(HI_DRM_RIGHTS_PPTR* pppSelf,
        const HI_CHAR* rightsFilePath, const HI_CHAR* mimeType,
        const HI_CHAR* accountId,
        const HI_CHAR* subscriptionId);

/**
 * \breif Constructor for DrmRights
 *
 * \param[in] rightsData Rights data
 * \param[in] mimeType MIME type
 * \param[in] accountId Account Id of the user
 * \param[in] subscriptionId Subscription Id of the user
 * \return Returns HI_SUCCESS for success, HI_FAILURE for failure
 */
HI_S32 HI_DRM_RIGHTS_Create(HI_DRM_RIGHTS_PPTR* pppSelf,
        const HI_DRM_BUFFER_S rightsData, const HI_CHAR* mimeType,
        const HI_CHAR* accountId,
        const HI_CHAR* subscriptionId);

/**
 * \breif Destroy the DRM rights object.
 *
 * \param[in] pppSelf Handle of DRM rights object
 *
 * \return None
 */
HI_VOID HI_DRM_RIGHTS_Destroy(HI_DRM_RIGHTS_PPTR* pppSelf);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* HI_DRM_RIGHTS_H_ */
