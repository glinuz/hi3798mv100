/*
 * hi_drm_support_info.h
 *
 *  Created on: Aug 2, 2012
 *      Author: zhangyi
 */

#ifndef HI_DRM_SUPPORT_INFO_H_
#define HI_DRM_SUPPORT_INFO_H_

#include <hi_drm_common.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * \breif This is an utility class which wraps the capability of each plug-in,
 * such as mimetype's and file suffixes it could handle.
 *
 * Plug-in developer could return the capability of the plugin by passing
 * DrmSupportInfo instance.
 *
 */
typedef const struct hiDRM_SUPPORT_INFO_S * const * HI_DRM_SUPPORT_INFO_PPTR;

/**
 * \breif Returns FileSuffixIterator object to walk through file suffix values
 * associated with this instance
 *
 * \return FileSuffixIterator object
 */
HI_ITERATOR_S HI_DRM_SUPPORT_INFO_GetFileSuffixIterator(HI_DRM_SUPPORT_INFO_PPTR ppSelf);

/**
 * \breif Returns MimeTypeIterator object to walk through mimetype values
 * associated with this instance
 *
 * \return MimeTypeIterator object
 */
HI_ITERATOR_S HI_DRM_SUPPORT_INFO_GetMimeTypeIterator(HI_DRM_SUPPORT_INFO_PPTR ppSelf);

/**
 * \breif Returns the number of mimetypes supported.
 *
 * \return Number of mimetypes supported
 */
HI_S32 HI_DRM_SUPPORT_INFO_GetMimeTypeCount(HI_DRM_SUPPORT_INFO_PPTR ppSelf);

/**
 * \breif Returns the number of file types supported.
 *
 * \return Number of file types supported
 */
HI_S32 HI_DRM_SUPPORT_INFO_GetFileSuffixCount(HI_DRM_SUPPORT_INFO_PPTR ppSelf);

/**
 * \breif Adds the mimetype to the list of supported mimetypes
 *
 * \param[in] mimeType mimetype to be added
 * \return Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_SUPPORT_INFO_AddMimeType(HI_DRM_SUPPORT_INFO_PPTR ppSelf, const HI_CHAR* mimeType);

/**
 * \breif Adds the filesuffix to the list of supported file types
 *
 * \param[in] filesuffix file suffix to be added
 * \return Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_SUPPORT_INFO_AddFileSuffix(HI_DRM_SUPPORT_INFO_PPTR ppSelf, const HI_CHAR* fileSuffix);

/**
 * \breif Set the unique description about the plugin
 *
 * \param[in] description Unique description
 * \return Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_SUPPORT_INFO_SetDescription(HI_DRM_SUPPORT_INFO_PPTR ppSelf, const HI_CHAR* description);

/**
 * \breif Returns the unique description associated with the plugin
 *
 * \return Unique description
 */
const HI_CHAR* HI_DRM_SUPPORT_INFO_GetDescription(HI_DRM_SUPPORT_INFO_PPTR ppSelf);

/**
 * \breif Returns whether given mimetype is supported or not
 *
 * \param[in] mimeType MIME type
 * \return
 *        true - if mime-type is supported
 *        false - if mime-type is not supported
 */
HI_BOOL HI_DRM_SUPPORT_INFO_IsSupportedMimeType(HI_DRM_SUPPORT_INFO_PPTR ppSelf, const HI_CHAR* mimeType);

/**
 * \breif Returns whether given file type is supported or not
 *
 * \param[in] fileType File type
 * \return
 *     true if file type is supported
 *     false if file type is not supported
 */
HI_BOOL HI_DRM_SUPPORT_INFO_IsSupportedFileSuffix(HI_DRM_SUPPORT_INFO_PPTR ppSelf, const HI_CHAR* fileType);

/**
 * \breif Create a DRM support info object.
 *
 * \param[in] pppSelf Handle of DRM support info object
 *
 * \return Returns HI_SUCCESS for success, HI_FAILURE for failure
 */
HI_S32 HI_DRM_SUPPORT_INFO_Create(HI_DRM_SUPPORT_INFO_PPTR* pppSelf);

/**
 * \breif Destroy the DRM client object.
 *
 * \param[in] pppSelf Handle of DRM support info object
 *
 */
HI_VOID HI_DRM_SUPPORT_INFO_Destroy(HI_DRM_SUPPORT_INFO_PPTR* pppSelf);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* HI_DRM_SUPPORT_INFO_H_ */
