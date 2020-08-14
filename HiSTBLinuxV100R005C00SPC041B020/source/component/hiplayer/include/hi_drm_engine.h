/*
 * drm_engine.h
 *
 *  Created on: Aug 7, 2012
 *      Author: zhangyi
 */

#ifndef DRM_ENGINE_H_
#define DRM_ENGINE_H_

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

typedef const struct hiDRM_ENGINE_S * const * HI_DRM_ENGINE_PPTR;
typedef struct hiDRM_ENGINE_S {
    //////////////////////////////////
    // Implementation of IDrmEngine //
    //////////////////////////////////
    /**
     * Initialize plug-in
     *
     * \param[in] uniqueId Unique identifier for a session
     * \return HI_S32
     *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
     */
    HI_S32 (*pfnInitialize)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId);
    /**
     * Register a callback to be invoked when the caller required to
     * receive necessary information
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] infoListener Listener
     * \return HI_S32
     *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
     */
    HI_S32 (*pfnSetOnInfoListener)(HI_DRM_ENGINE_PPTR ppSelf,
            HI_S32 uniqueId, const struct OnInfoListener infoListener);
    /**
     * Terminate the plug-in
     * and release resource bound to plug-in
     * e.g.) release native resource
     *
     * \param[in] uniqueId Unique identifier for a session
     * \return HI_S32
     *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
     */
    HI_S32 (*pfnTerminate)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId);
    /**
     * Get constraint information associated with input content
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] path Path of the protected content
     * \param[in] action Actions defined such as,
     *     Action::DEFAULT, Action::PLAY, etc
     * \return DrmConstraints
     *     key-value pairs of constraint are embedded in it
     * \note
     *     In case of error, return NULL
     */
    HI_DRM_CONSTRAINTS_PPTR (*pfnGetConstraints)(HI_DRM_ENGINE_PPTR ppSelf,
            HI_S32 uniqueId, const HI_CHAR* path, HI_S32 action);
    /**
     * Get metadata information associated with input content
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] path Path of the protected content
     * \return DrmMetadata
     *         key-value pairs of metadata
     * \note
     *      In case of error, return NULL
     */
    HI_DRM_METADATA_PPTR (*pfnGetMetadata)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, const HI_CHAR* path);
    /**
     * Get whether the given content can be handled by this plugin or not
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] path Path the protected object
     * \return HI_BOOL
     *     true if this plugin can handle , false in case of not able to handle
     */
    HI_BOOL (*pfnCanHandle)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, const HI_CHAR* path);
    /**
     * Executes given drm information based on its type
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] drmInfo Information needs to be processed
     * \return DrmInfoStatus
     *     instance as a result of processing given input
     */
    HI_DRM_INFO_STATUS_PTR (*pfnProcessDrmInfo)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, HI_DRM_INFO_PPTR drmInfo);
    /**
     * Retrieves necessary information for registration, unregistration or rights
     * acquisition information.
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] drmInfoRequest Request information to retrieve drmInfo
     * \return DrmInfo
     *     instance as a result of processing given input
     */
    HI_DRM_INFO_PPTR (*pfnAcquireDrmInfo)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, HI_DRM_INFO_REQUEST_PPTR drmInfoRequest);
    /**
     * Save DRM rights to specified rights path
     * and make association with content path
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] drmRights DrmRights to be saved
     * \param[in] rightsPath File path where rights to be saved
     * \param[in] contentPath File path where content was saved
     * \return HI_S32
     *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
     */
    HI_S32 (*pfnSaveRights)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, HI_DRM_RIGHTS_PPTR drmRights,
            const HI_CHAR* rightsPath, const HI_CHAR* contentPath);
    /**
     * Retrieves the mime type embedded inside the original content
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] path Path of the protected content
     * \return HI_CHAR*
     *     Returns mime-type of the original content, such as "video/mpeg"
     */
    HI_CHAR* (*pfnGetOriginalMimeType)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, const HI_CHAR* path);
    /**
     * Retrieves the type of the protected object (content, rights, etc..)
     * using specified path or mimetype. At least one parameter should be non null
     * to retrieve DRM object type
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] path Path of the content or null.
     * \param[in] mimeType Mime type of the content or null.
     * \return type of the DRM content,
     *     such as DrmObjectType::CONTENT, DrmObjectType::RIGHTS_OBJECT
     */
    HI_S32 (*pfnGetDrmObjectType)(HI_DRM_ENGINE_PPTR ppSelf,
            HI_S32 uniqueId, const HI_CHAR* path, const HI_CHAR* mimeType);
    /**
     * Check whether the given content has valid rights or not
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] path Path of the protected content
     * \param[in] action Action to perform (Action::DEFAULT, Action::PLAY, etc)
     * \return the status of the rights for the protected content,
     *     such as RightsStatus::RIGHTS_VALID, RightsStatus::RIGHTS_EXPIRED, etc.
     */
    HI_S32 (*pfnCheckRightsStatus)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, const HI_CHAR* path, HI_S32 action);
    /**
     * Consumes the rights for a content.
     * If the reserve parameter is true the rights is reserved until the same
     * application calls this api again with the reserve parameter set to false.
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] decryptHandle Handle for the decryption session
     * \param[in] action Action to perform. (Action::DEFAULT, Action::PLAY, etc)
     * \param[in] reserve True if the rights should be reserved.
     * \return HI_S32
     *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
     */
    HI_S32 (*pfnConsumeRights)(HI_DRM_ENGINE_PPTR ppSelf,
            HI_S32 uniqueId, HI_DECRYPT_HANDLE_PTR decryptHandle, HI_S32 action, HI_BOOL reserve);
    /**
     * Informs the DRM Engine about the playback actions performed on the DRM files.
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] decryptHandle Handle for the decryption session
     * \param[in] playbackStatus Playback action (Playback::START, Playback::STOP, Playback::PAUSE)
     * \param[in] position Position in the file (in milliseconds) where the start occurs.
     *     Only valid together with Playback::START.
     * \return HI_S32
     *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
     */
    HI_S32 (*pfnSetPlaybackStatus)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, HI_DECRYPT_HANDLE_PTR decryptHandle,
            HI_S32 playbackStatus, HI_S64 position);
    /**
     * Validates whether an action on the DRM content is allowed or not.
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] path Path of the protected content
     * \param[in] action Action to validate (Action::PLAY, Action::TRANSFER, etc)
     * \param[in] description Detailed description of the action
     * \return true if the action is allowed.
     */
    HI_BOOL (*pfnValidateAction)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, const HI_CHAR* path,
            HI_S32 action, const HI_ACTION_DESCRIPTION_S* description);
    /**
     * Removes the rights associated with the given protected content
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] path Path of the protected content
     * \return HI_S32
     *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
     */
    HI_S32 (*pfnRemoveRights)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, const HI_CHAR* path);
    /**
     * Removes all the rights information of each plug-in associated with
     * DRM framework. Will be used in master reset
     *
     * \param[in] uniqueId Unique identifier for a session
     * \return HI_S32
     *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
     */
    HI_S32 (*pfnRemoveAllRights)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId);
    /**
     * This API is for Forward Lock based DRM scheme.
     * Each time the application tries to download a new DRM file
     * which needs to be converted, then the application has to
     * begin with calling this API.
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] convertId Handle for the convert session
     * \return HI_S32
     *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
     */
    HI_S32 (*pfnOpenConvertSession)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, HI_S32 convertId);
    /**
     * Accepts and converts the input data which is part of DRM file.
     * The resultant converted data and the status is returned in the DrmConvertedInfo
     * object. This method will be called each time there are new block
     * of data received by the application.
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] convertId Handle for the convert session
     * \param[in] inputData Input Data which need to be converted
     * \return Return object contains the status of the data conversion,
     *     the output converted data and offset. In this case the
     *     application will ignore the offset information.
     */
    HI_DRM_CONVERTED_STATUS_PTR (*pfnConvertData)(HI_DRM_ENGINE_PPTR ppSelf,
            HI_S32 uniqueId, HI_S32 convertId, const HI_DRM_BUFFER_S inputData);
    /**
     * Informs the Drm Agent when there is no more data which need to be converted
     * or when an error occurs. Upon successful conversion of the complete data,
     * the agent will inform that where the header and body signature
     * should be added. This signature appending is needed to integrity
     * protect the converted file.
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] convertId Handle for the convert session
     * \return Return object contains the status of the data conversion,
     *     the header and body signature data. It also informs
     *     the application on which offset these signature data
     *     should be appended.
     */
    HI_DRM_CONVERTED_STATUS_PTR (*pfnCloseConvertSession)(HI_DRM_ENGINE_PPTR ppSelf,  HI_S32 uniqueId, HI_S32 convertId);
    /**
     * Returns the information about the Drm Engine capabilities which includes
     * supported MimeTypes and file suffixes.
     *
     * \param[in] uniqueId Unique identifier for a session
     * \return DrmSupportInfo
     *     instance which holds the capabilities of a plug-in
     */
    HI_DRM_SUPPORT_INFO_PPTR (*pfnGetSupportInfo)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId);
    /**
     * Open the decrypt session to decrypt the given protected content
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] decryptHandle Handle for the current decryption session
     * \param[in] fd File descriptor of the protected content to be decrypted
     * \param[in] offset Start position of the content
     * \param[in] length The length of the protected content
     * \param[in] mime Mime type of the protected content if it is
     *     not NULL or empty
     * \return
     *     DRM_ERROR_CANNOT_HANDLE for failure and DRM_NO_ERROR for success
     */
    HI_S32 (*pfnOpenDecryptSession)(HI_DRM_ENGINE_PPTR ppSelf,
        HI_S32 uniqueId, HI_DECRYPT_HANDLE_PTR decryptHandle,
        HI_S32 fd, HI_S64 offset, HI_S64 length, const HI_CHAR* mime);
    /**
     * Open the decrypt session to decrypt the given protected content
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] decryptHandle Handle for the current decryption session
     * \param[in] uri Path of the protected content to be decrypted
     * \param[in] mime Mime type of the protected content if it is
     *     not NULL or empty
     * \return
     *     DRM_ERROR_CANNOT_HANDLE for failure and DRM_NO_ERROR for success
     */
    HI_S32 (*pfnOpenDecryptSession2)(HI_DRM_ENGINE_PPTR ppSelf,
        HI_S32 uniqueId, HI_DECRYPT_HANDLE_PTR decryptHandle,
        const HI_CHAR* uri, const HI_CHAR* mime);
    /**
     * Close the decrypt session for the given handle
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] decryptHandle Handle for the decryption session
     * \return HI_S32
     *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
     */
    HI_S32 (*pfnCloseDecryptSession)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, HI_DECRYPT_HANDLE_PTR decryptHandle);
    /**
     * Initialize decryption for the given unit of the protected content
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] decryptHandle Handle for the decryption session
     * \param[in] decryptUnitId ID which specifies decryption unit, such as track ID
     * \param[in] headerInfo Information for initializing decryption of this decrypUnit
     * \return HI_S32
     *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
     */
    HI_S32 (*pfnInitializeDecryptUnit)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, HI_DECRYPT_HANDLE_PTR decryptHandle,
            HI_S32 decryptUnitId, const HI_DRM_BUFFER_S headerInfo);
    /**
     * Decrypt the protected content buffers for the given unit
     * This method will be called any number of times, based on number of
     * encrypted streams received from application.
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] decryptHandle Handle for the decryption session
     * \param[in] decryptUnitId ID which specifies decryption unit, such as track ID
     * \param[in] encBuffer Encrypted data block
     * \param[out] decBuffer Decrypted data block
     * \param[in] IV Optional buffer
     * \return HI_S32
     *     Returns the error code for this API
     *     DRM_NO_ERROR for success, and one of DRM_ERROR_UNKNOWN, DRM_ERROR_LICENSE_EXPIRED
     *     DRM_ERROR_SESSION_NOT_OPENED, DRM_ERROR_DECRYPT_UNIT_NOT_INITIALIZED,
     *     DRM_ERROR_DECRYPT for failure.
     */
    HI_S32 (*pfnDecrypt)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, HI_DECRYPT_HANDLE_PTR decryptHandle, HI_S32 decryptUnitId,
            const HI_DRM_BUFFER_S encBuffer, HI_DRM_BUFFER_S** decBuffer, HI_DRM_DECRYPT_EXTRADATA_S* extraData);
    /**
     * Finalize decryption for the given unit of the protected content
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] decryptHandle Handle for the decryption session
     * \param[in] decryptUnitId ID which specifies decryption unit, such as track ID
     * \return HI_S32
     *     Returns DRM_NO_ERROR for success, DRM_ERROR_UNKNOWN for failure
     */
    HI_S32 (*pfnFinalizeDecryptUnit)(HI_DRM_ENGINE_PPTR ppSelf,
            HI_S32 uniqueId, HI_DECRYPT_HANDLE_PTR decryptHandle, HI_S32 decryptUnitId);
    /**
     * Reads the specified number of bytes from an open DRM file.
     *
     * \param[in] uniqueId Unique identifier for a session
     * \param[in] decryptHandle Handle for the decryption session
     * \param[out] buffer Reference to the buffer that should receive the read data.
     * \param[in] numBytes Number of bytes to read.
     * \param[in] offset Offset with which to update the file position.
     *
     * \return Number of bytes read. Returns -1 for Failure.
     */
    ssize_t (*pfnPread)(HI_DRM_ENGINE_PPTR ppSelf, HI_S32 uniqueId, HI_DECRYPT_HANDLE_PTR decryptHandle,
            void* buffer, ssize_t numBytes, HI_S64 offset);
} HI_DRM_ENGINE_S;

/**
 * \breif Create a DRM engine object.
 *
 * \param[in] pppSelf Handle of DRM Client object
 *
 * \return Returns HI_SUCCESS for success, HI_FAILURE for failure
 */
HI_S32 HI_DRM_ENGINE_Create(HI_DRM_ENGINE_PPTR* pppSelf);

/**
 * \breif Destroy the DRM engine object.
 *
 * \param[in] pppSelf Handle of DRM engine object
 *
 * \return None
 */
HI_VOID HI_DRM_ENGINE_Destroy(HI_DRM_ENGINE_PPTR* pppSelf);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* DRM_ENGINE_H_ */
