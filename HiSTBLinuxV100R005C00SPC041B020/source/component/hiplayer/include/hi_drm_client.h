/*
 * hi_drm_manager_client.h
 *
 *  Created on: Jul 31, 2012
 *      Author: zhangyi
 */

#ifndef HI_DRM_CLIENT_H_
#define HI_DRM_CLIENT_H_

#include <hi_drm_common.h>
#include <hi_drm_rights.h>
#include <hi_drm_constraints.h>
#include <hi_drm_metadata.h>
#include <hi_drm_info.h>
#include <hi_drm_support_info.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * \breif The Native application will instantiate this class and access DRM Framework
 * services through this class.
 *
 */

typedef const struct hiDRM_CLIENT_S* const * HI_DRM_CLIENT_PPTR;

/**
 * \breif Install new DRM Engine Plug-in at the runtime
 *
 * \param[in] drmEngine Shared Object(so) File in which DRM Engine defined
 * \return status_t
 *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_InstallDrmEngine(HI_DRM_CLIENT_PPTR ppSelf, const HI_CHAR* drmEngineFile);

/**
 * \breif Retrieves all DrmSupportInfo instance that native DRM framework can handle.
 * This interface is meant to be used by JNI layer
 *
 * \param[out] length Number of elements in drmSupportInfoArray
 * \param[out] drmSupportInfoArray Array contains all DrmSupportInfo
 *     that native DRM framework can handle
 * \return HI_S32
 *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_GetAllSupportInfo(HI_DRM_CLIENT_PPTR ppSelf,
        HI_S32* length, HI_DRM_SUPPORT_INFO_PPTR** drmSupportInfoArray);

/**
 * \breif Open the decrypt session to decrypt the given protected content
 *
 * \param[in] fd File descriptor of the protected content to be decrypted
 * \param[in] offset Start position of the content
 * \param[in] length The length of the protected content
 * \param[in] mime Mime type of the protected content if it is not NULL or empty
 * \return
 *     Handle for the decryption session
 */
HI_DECRYPT_HANDLE_PTR HI_DRM_OpenDecryptSession(HI_DRM_CLIENT_PPTR ppSelf,
        HI_S32 fd, HI_S64 offset, HI_S64 length, const HI_CHAR* mime);

/**
 * \breif Open the decrypt session to decrypt the given protected content
 *
 * \param[in] uri Path of the protected content to be decrypted
 * \param[in] mime Mime type of the protected content if it is not NULL or empty
 * \return
 *     Handle for the decryption session
 */
HI_DECRYPT_HANDLE_PTR HI_DRM_OpenDecryptSession2(HI_DRM_CLIENT_PPTR ppSelf,
        const HI_CHAR* uri, const HI_CHAR* mime);

/**
 * \breif Close the decrypt session for the given handle
 *
 * \param[in] decryptHandle Handle for the decryption session
 * \return HI_S32
 *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_CloseDecryptSession(HI_DRM_CLIENT_PPTR ppSelf,
        HI_DECRYPT_HANDLE_PTR ecryptHandle);

/**
 * \breif Consumes the rights for a content.
 * If the reserve parameter is true the rights is reserved until the same
 * application calls this api again with the reserve parameter set to false.
 *
 * \param[in] decryptHandle Handle for the decryption session
 * \param[in] action Action to perform. (Action::DEFAULT, Action::PLAY, etc)
 * \param[in] reserve True if the rights should be reserved.
 * \return HI_S32
 *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure.
 *     In case license has been expired, DRM_ERROR_LICENSE_EXPIRED will be returned.
 */
HI_S32 HI_DRM_ConsumeRights(HI_DRM_CLIENT_PPTR ppSelf,
        HI_DECRYPT_HANDLE_PTR ecryptHandle, HI_S32 action, HI_BOOL reserve);

/**
 * \breif Informs the DRM engine about the playback actions performed on the DRM files.
 *
 * \param[in] decryptHandle Handle for the decryption session
 * \param[in] playbackStatus Playback action (Playback::START, Playback::STOP, Playback::PAUSE)
 * \param[in] position Position in the file (in milliseconds) where the start occurs.
 *                     Only valid together with Playback::START.
 * \return HI_S32
 *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_SetPlaybackStatus(HI_DRM_CLIENT_PPTR ppSelf,
        HI_DECRYPT_HANDLE_PTR ecryptHandle, HI_S32 playbackStatus, HI_S64 position);

/**
 * \breif Initialize decryption for the given unit of the protected content
 *
 * \param[in] decryptHandle Handle for the decryption session
 * \param[in] decryptUnitId ID which specifies decryption unit, such as track ID
 * \param[in] headerInfo Information for initializing decryption of this decrypUnit
 * \return HI_S32
 *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_InitializeDecryptUnit(HI_DRM_CLIENT_PPTR ppSelf,
        HI_DECRYPT_HANDLE_PTR ecryptHandle, HI_S32 decryptUnitId, const HI_DRM_BUFFER_S headerInfo);

/**
 * \breif Decrypt the protected content buffers for the given unit
 * This method will be called any number of times, based on number of
 * encrypted streams received from application.
 *
 * \param[in] decryptHandle Handle for the decryption session
 * \param[in] decryptUnitId ID which specifies decryption unit, such as track ID
 * \param[in] encBuffer Encrypted data block
 * \param[out] decBuffer Decrypted data block
 * \param[in] extraData Optional buffer
 * \return HI_S32
 *     Returns the error code for this API
 *     DRM_NO_ERROR for success, and one of DRM_ERROR_UNKNOWN, DRM_ERROR_LICENSE_EXPIRED
 *     DRM_ERROR_SESSION_NOT_OPENED, DRM_ERROR_DECRYPT_UNIT_NOT_INITIALIZED,
 *     DRM_ERROR_DECRYPT for failure.
 */
HI_S32 HI_DRM_Decrypt(HI_DRM_CLIENT_PPTR ppSelf,
        HI_DECRYPT_HANDLE_PTR ecryptHandle, HI_S32 decryptUnitId,
        const HI_DRM_BUFFER_S encBuffer, HI_DRM_BUFFER_S** decBuffer, HI_DRM_DECRYPT_EXTRADATA_S* extraData);

/**
 * \breif Finalize decryption for the given unit of the protected content
 *
 * \param[in] decryptHandle Handle for the decryption session
 * \param[in] decryptUnitId ID which specifies decryption unit, such as track ID
 * \return HI_S32
 *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_FinalizeDecryptUnit(HI_DRM_CLIENT_PPTR ppSelf,
        HI_DECRYPT_HANDLE_PTR ecryptHandle, HI_S32 decryptUnitId);
/**
 * \breif Reads the specified number of bytes from an open DRM file.
 *
 * \param[in] decryptHandle Handle for the decryption session
 * \param[out] buffer Reference to the buffer that should receive the read data.
 * \param[in] numBytes Number of bytes to read.
 * \param[in] offset Offset with which to update the file position.
 *
 * \return Number of bytes read. Returns -1 for Failure.
 */
ssize_t HI_DRM_Pread(HI_DRM_CLIENT_PPTR ppSelf,
        HI_DECRYPT_HANDLE_PTR ecryptHandle,
        HI_VOID* buffer, ssize_t numBytes, HI_S64 offset);

/**
 * \breif Validates whether an action on the DRM content is allowed or not.
 *
 * \param[in] path Path of the protected content
 * \param[in] action Action to validate. (Action::DEFAULT, Action::PLAY, etc)
 * \param[in] description Detailed description of the action
 * \return true if the action is allowed.
 */
HI_BOOL HI_DRM_ValidateAction(HI_DRM_CLIENT_PPTR ppSelf,
        const HI_CHAR* path, HI_S32 action, const HI_ACTION_DESCRIPTION_S* description);

/**
 * \breif Register a callback to be invoked when the caller required to
 * receive necessary information
 *
 * \param[in] infoListener Listener
 * \return HI_S32
 *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_SetOnInfoListener(HI_DRM_CLIENT_PPTR ppSelf,
        const struct OnInfoListener infoListener);

/**
 * \breif Get constraint information associated with input content
 *
 * \param[in] path Path of the protected content
 * \param[in] action Actions defined such as,
 *             Action::DEFAULT, Action::PLAY, etc
 * \return DrmConstraints
 *     key-value pairs of constraint are embedded in it
 * \note
 *     In case of error, return NULL
 */
HI_DRM_CONSTRAINTS_PPTR HI_DRM_GetConstraints(HI_DRM_CLIENT_PPTR ppSelf,
        const HI_CHAR* path, const HI_S32 action);

/**
 * \breif Get metadata information associated with input content
 *
 * \param[in] path Path of the protected content
 * \return DrmMetadata
 *         key-value pairs of metadata
 * \note
 *     In case of error, return NULL
 */
HI_DRM_METADATA_PPTR HI_DRM_GetMetadata(HI_DRM_CLIENT_PPTR ppSelf,
        const HI_CHAR* path);

/**
 * \breif Check whether the given mimetype or path can be handled
 *
 * \param[in] path Path of the content needs to be handled
 * \param[in] mimetype Mimetype of the content needs to be handled
 * \return
 *     True if DrmManager can handle given path or mime type.
 */
HI_BOOL HI_DRM_CanHandle(HI_DRM_CLIENT_PPTR ppSelf,
        const HI_CHAR* path, const HI_CHAR* mimeType);

/**
 * \breif Executes given drm information based on its type
 *
 * \param[in] drmInfo Information needs to be processed
 * \return DrmInfoStatus
 *     instance as a result of processing given input
 */
HI_DRM_INFO_STATUS_PTR HI_DRM_ProcessDrmInfo(HI_DRM_CLIENT_PPTR ppSelf,
        HI_DRM_INFO_PPTR drmInfo);

/**
 * \breif Retrieves necessary information for registration, unregistration or rights
 * acquisition information.
 *
 * \param[in] drmInfoRequest Request information to retrieve drmInfo
 * \return DrmInfo
 *     instance as a result of processing given input
 */
HI_DRM_INFO_PPTR HI_DRM_AcquireDrmInfo(HI_DRM_CLIENT_PPTR ppSelf,
        HI_DRM_INFO_REQUEST_PPTR drmInfoRequest);

/**
 * \breif Save DRM rights to specified rights path
 * and make association with content path
 *
 * \param[in] drmRights DrmRights to be saved
 * \param[in] rightsPath File path where rights to be saved
 * \param[in] contentPath File path where content was saved
 * \return HI_S32
 *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_SaveRights(HI_DRM_CLIENT_PPTR ppSelf,
        HI_DRM_RIGHTS_PPTR drmRights, const HI_CHAR* rightsPath, const HI_CHAR* contentPath);

/**
 * \breif Retrieves the mime type embedded inside the original content
 *
 * \param[in] path the path of the protected content
 * \return String8
 *     Returns mime-type of the original content, such as "video/mpeg"
 */
const HI_CHAR* HI_DRM_GetOriginalMimeType(HI_DRM_CLIENT_PPTR ppSelf,
        const HI_CHAR* path);

/**
 * \breif Retrieves the type of the protected object (content, rights, etc..)
 * by using specified path or mimetype. At least one parameter should be non null
 * to retrieve DRM object type
 *
 * \param[in] path Path of the content or null.
 * \param[in] mimeType Mime type of the content or null.
 * \return type of the DRM content,
 *     such as DrmObjectType::CONTENT, DrmObjectType::RIGHTS_OBJECT
 */
HI_S32 HI_DRM_GetDrmObjectType(HI_DRM_CLIENT_PPTR ppSelf,
        const HI_CHAR* path, const HI_CHAR* mimeType);

/**
 * \breif Check whether the given content has valid rights or not
 *
 * \param[in] path Path of the protected content
 * \param[in] action Action to perform
 * \return the status of the rights for the protected content,
 *     such as RightsStatus::RIGHTS_VALID, RightsStatus::RIGHTS_EXPIRED, etc.
 */
HI_S32 HI_DRM_CheckRightsStatus(HI_DRM_CLIENT_PPTR ppSelf,
        const HI_CHAR* path, HI_S32 action);

/**
 * \breif Removes the rights associated with the given protected content
 *
 * \param[in] path Path of the protected content
 * \return HI_S32
 *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_RemoveRights(HI_DRM_CLIENT_PPTR ppSelf,
        const HI_CHAR* path);

/**
 * \breif Removes all the rights information of each plug-in associated with
 * DRM framework. Will be used in master reset
 *
 * \return HI_S32
 *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
 */
HI_S32 HI_DRM_RemoveAllRights(HI_DRM_CLIENT_PPTR ppSelf);

/**
 * \breif This API is for Forward Lock DRM.
 * Each time the application tries to download a new DRM file
 * which needs to be converted, then the application has to
 * begin with calling this API.
 *
 * \param[in] convertId Handle for the convert session
 * \param[in] mimeType Description/MIME type of the input data packet
 * \return Return handle for the convert session
 */
HI_S32 HI_DRM_OpenConvertSession(HI_DRM_CLIENT_PPTR ppSelf,
        const HI_CHAR* mimeType);

/**
 * \breif Passes the input data which need to be converted. The resultant
 * converted data and the status is returned in the DrmConvertedInfo
 * object. This method will be called each time there are new block
 * of data received by the application.
 *
 * \param[in] convertId Handle for the convert session
 * \param[in] inputData Input Data which need to be converted
 * \return Return object contains the status of the data conversion,
 *     the output converted data and offset. In this case the
 *     application will ignore the offset information.
 */
HI_DRM_CONVERTED_STATUS_PTR HI_DRM_ConvertData(HI_DRM_CLIENT_PPTR ppSelf,
        HI_S32 convertId, const HI_DRM_BUFFER_S inputData);

/**
 * \breif When there is no more data which need to be converted or when an
 * error occurs that time the application has to inform the Drm agent
 * via this API. Upon successful conversion of the complete data,
 * the agent will inform that where the header and body signature
 * should be added. This signature appending is needed to integrity
 * protect the converted file.
 *
 * \param[in] convertId Handle for the convert session
 * \return Return object contains the status of the data conversion,
 *     the header and body signature data. It also informs
 *     the application on which offset these signature data
 *     should be appended.
 */
HI_DRM_CONVERTED_STATUS_PTR HI_DRM_CloseConvertSession(HI_DRM_CLIENT_PPTR ppSelf,
        HI_S32 convertId);

/**
 * \breif Create a DRM client object.
 *
 * \param[in] pppSelf Handle of DRM Client object
 *
 * \return Returns HI_SUCCESS for success, HI_FAILURE for failure
 */
HI_S32 HI_DRM_CreateClient(HI_DRM_CLIENT_PPTR* pppSelf);

/**
 * \breif Destroy the DRM client object.
 *
 * \param[in] pppSelf Handle of DRM Client object
 *
 */
HI_VOID HI_DRM_DestroyClient(HI_DRM_CLIENT_PPTR* pppSelf);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* HI_DRM_CLIENT_H_ */
