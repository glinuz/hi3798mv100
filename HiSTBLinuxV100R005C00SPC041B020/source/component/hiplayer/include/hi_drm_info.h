/*
 * hi_drm_info.h
 *
 *  Created on: Aug 1, 2012
 *      Author: zhangyi
 */

#ifndef HI_DRM_INFO_H_
#define HI_DRM_INFO_H_

#include <hi_drm_common.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
/**
 * \breif Key to pass the unique id for the account or the user
 */
extern const HI_CHAR* ACCOUNT_ID;
/**
 * \breif Key to pass the subscription id
 */
extern const HI_CHAR* SUBSCRIPTION_ID;
//------------------------------------------------------------------------------
// Changes in following constants should be in sync with DrmInfoRequest.java
typedef enum {
    HI_DRM_REGISTRATION_INFO = 1,
    HI_DRM_UNREGISTRATION_INFO = 2,
    HI_DRM_RIGHTS_ACQUISITION_INFO = 3,
    HI_DRM_RIGHTS_ACQUISITION_PROGRESS_INFO = 4,
    /**
     * Playready extend commands
     */
    HI_DRM_PLAYREADY_INITIATOR_INFO   = 100,
    HI_DRM_PLAYREADY_ENVELOP_INFO     = 102,
    HI_DRM_PLAYERADY_EXTERN_CERT      = 103,
} HI_DRM_INFO_TYPE_E;

typedef enum {
//! HI_DRM_EVENT_ALREADY_REGISTERED_BY_ANOTHER_ACCOUNT, when registration has been
//! already done by another account ID.
    HI_DRM_EVENT_ALREADY_REGISTERED_BY_ANOTHER_ACCOUNT = 1,
//! HI_DRM_EVENT_REMOVE_RIGHTS, when the rights needs to be removed completely.
    HI_DRM_EVENT_REMOVE_RIGHTS = 2,
//! TYPE_RIGHTS_INSTALLED, when the rights are downloaded and installed ok.
    HI_DRM_EVENT_RIGHTS_INSTALLED = 3,
//! TYPE_WAIT_FOR_RIGHTS, rights object is on it's way to phone,
//! wait before calling checkRights again
    HI_DRM_EVENT_WAIT_FOR_RIGHTS = 4,
//! TYPE_ACCOUNT_ALREADY_REGISTERED, when registration has been
//! already done for the given account.
    HI_DRM_EVENT_ACCOUNT_ALREADY_REGISTERED = 5,
//! HI_DRM_EVENT_RIGHTS_REMOVED, when the rights has been removed.
    HI_DRM_EVENT_RIGHTS_REMOVED = 6,

//! HI_DRM_EVENT_RIGHTS_NOT_INSTALLED, when something went wrong installing the rights
    HI_DRM_EVENT_RIGHTS_NOT_INSTALLED = 2001,
//! HI_DRM_EVENT_RIGHTS_RENEWAL_NOT_ALLOWED, when the server rejects renewal of rights
    HI_DRM_EVENT_RIGHTS_RENEWAL_NOT_ALLOWED = 2002,
//! HI_DRM_EVENT_NOT_SUPPORTED, when answer from server can not be handled by the native agent
    HI_DRM_EVENT_NOT_SUPPORTED = 2003,
//! HI_DRM_EVENT_OUT_OF_MEMORY, when memory allocation fail during renewal.
//! Can in the future perhaps be used to trigger garbage collector
    HI_DRM_EVENT_OUT_OF_MEMORY = 2004,
//! TYPE_NO_INTERNET_CONNECTION, when the Internet connection is missing and no attempt
//! can be made to renew rights
    HI_DRM_EVENT_NO_INTERNET_CONNECTION = 2005,
//! HI_DRM_EVENT_PROCESS_DRM_INFO_FAILED, when failed to process DrmInfo.
    HI_DRM_EVENT_PROCESS_DRM_INFO_FAILED = 2006,
//! HI_DRM_EVENT_REMOVE_ALL_RIGHTS_FAILED, when failed to remove all the rights objects
//! associated with all DRM schemes.
    HI_DRM_EVENT_REMOVE_ALL_RIGHTS_FAILED = 2007,
//! HI_DRM_EVENT_ACQUIRE_DRM_INFO_FAILED, when failed to acquire DrmInfo.
    HI_DRM_EVENT_ACQUIRE_DRM_INFO_FAILED = 2008,
    /*Extend event for playrady*/
//! HI_DRM_EVENT_ACQUIRE_PLAYREADY_PARAM, when external buffer no more useful
    HI_DRM_EVENT_PLAYREADY_OPENED = 3001,

} HI_DRM_INFO_EVENT_E;
//------------------------------------------------------------------------------
typedef const struct hiDRM_INFO_REQUEST_S * const * HI_DRM_INFO_REQUEST_PPTR;
typedef const struct hiDRM_INFO_S * const * HI_DRM_INFO_PPTR;

/**
 * \breif This is an utility class in which necessary information required to transact
 * between device and online DRM server is described. DRM Framework achieves
 * server registration, license acquisition and any other server related transaction
 * by passing an instance of this class to DrmManagerClient::processDrmInfo(const DrmInfo*).
 *
 * The Caller can retrieve the DrmInfo instance by using
 * DrmManagerClient::acquireDrmInfo(const DrmInfoRequest*) by passing DrmInfoRequest instance.
 *
 */

/**
 * \breif Returns information type associated with this instance
 *
 * \return Information type
 */
HI_S32 HI_DRM_INFO_GetInfoType(HI_DRM_INFO_PPTR ppSelf);

/**
 * \breif Returns MIME type associated with this instance
 *
 * \return MIME type
 */
const HI_CHAR* HI_DRM_INFO_GetMimeType(HI_DRM_INFO_PPTR ppSelf);

/**
 * \breif Returns the trigger data associated with this instance
 *
 * \return Trigger data
 */
HI_DRM_BUFFER_S HI_DRM_INFO_GetData(HI_DRM_INFO_PPTR ppSelf);

/**
 * \breif Returns the number of attributes contained in this instance
 *
 * \return Number of attributes
 */
HI_S32 HI_DRM_INFO_GetCount(HI_DRM_INFO_PPTR ppSelf);

/**
 * \breif Adds optional information as <key, value> pair to this instance
 *
 * \param[in] key Key to add
 * \param[in] value Value to add
 * \return Returns the error code
 */
HI_S32 HI_DRM_INFO_Put(HI_DRM_INFO_PPTR ppSelf, const HI_CHAR* key, const HI_CHAR* value);

/**
 * \breif Retrieves the value of given key
 *
 * \param key Key whose value to be retrieved
 * \return The value
 */
const HI_CHAR* HI_DRM_INFO_Get(HI_DRM_INFO_PPTR ppSelf, const HI_CHAR* key);

/**
 * \breif Returns KeyIterator object to walk through the keys associated with this instance
 *
 * \return KeyIterator object
 */
HI_ITERATOR_S HI_DRM_INFO_KeyIterator(HI_DRM_INFO_PPTR ppSelf);

/**
 * \breif Returns Iterator object to walk through the values associated with this instance
 *
 * \return Iterator object
 */
HI_ITERATOR_S HI_DRM_INFO_Iterator(HI_DRM_INFO_PPTR ppSelf);

/**
 * \breif Returns index of the given key
 *
 * \return index
 */
HI_S32 HI_DRM_INFO_IndexOfKey(HI_DRM_INFO_PPTR ppSelf, const HI_CHAR* key);

//------------------------------------------------------------------------------
/**
 * \breif This is an utility class used to pass required parameters to get
 * the necessary information to communicate with online DRM server
 *
 * An instance of this class is passed to
 * DrmManagerClient::acquireDrmInfo(const DrmInfoRequest*) to get the
 * instance of DrmInfo.
 *
 */

/**
 * \breif Returns information type associated with this instance
 *
 * \return Information type
 */
HI_S32 HI_DRM_INFO_REQUEST_GetInfoType(HI_DRM_INFO_REQUEST_PPTR ppSelf);

/**
 * \breif Returns MIME type associated with this instance
 *
 * \return MIME type
 */
const HI_CHAR* HI_DRM_INFO_REQUEST_GetMimeType(HI_DRM_INFO_REQUEST_PPTR ppSelf);

/**
 * \breif Returns the number of entries in DrmRequestInfoMap
 *
 * \return Number of entries
 */
HI_S32 HI_DRM_INFO_REQUEST_GetCount(HI_DRM_INFO_REQUEST_PPTR ppSelf);

/**
 * \breif Adds optional information as <key, value> pair to this instance
 *
 * \param[in] key Key to add
 * \param[in] value Value to add
 * \return Returns the error code
 */
HI_S32 HI_DRM_INFO_REQUEST_Put(HI_DRM_INFO_REQUEST_PPTR ppSelf, const HI_CHAR* key, const HI_CHAR* value);

/**
 * \breif Retrieves the value of given key
 *
 * \param key Key whose value to be retrieved
 * \return The value
 */
const HI_CHAR* HI_DRM_INFO_REQUEST_Get(HI_DRM_INFO_REQUEST_PPTR ppSelf, const HI_CHAR* key);

/**
 * \breif Returns KeyIterator object to walk through the keys associated with this instance
 *
 * \return KeyIterator object
 */
HI_ITERATOR_S HI_DRM_INFO_REQUEST_KeyIterator(HI_DRM_INFO_REQUEST_PPTR ppSelf);

/**
 * \breif Returns Iterator object to walk through the values associated with this instance
 *
 * \return Iterator object
 */
HI_ITERATOR_S HI_DRM_INFO_REQUEST_Iterator(HI_DRM_INFO_REQUEST_PPTR ppSelf);


//------------------------------------------------------------------------------
//HI_DRM_INFO_EVENT_PPTR
//------------------------------------------------------------------------------
/**
 * \breif This is an entity class which would be passed to caller in
 * DrmManagerClient::OnInfoListener::onInfo(const DrmInfoEvent&).
 */

typedef const struct hiDRM_INFO_EVENT_S * const * HI_DRM_INFO_EVENT_PPTR;

/**
 * \breif Returns the Unique Id associated with this instance
 *
 * \return Unique Id
 */
HI_S32 HI_DRM_INFO_EVENT_GetUniqueId(HI_DRM_INFO_EVENT_PPTR ppSelf);

/**
 * \breif Returns the Type of information associated with this object
 *
 * \return Type of information
 */
HI_S32 HI_DRM_INFO_EVENT_GetType(HI_DRM_INFO_EVENT_PPTR ppSelf);

/**
 * \breif Returns the message description associated with this object
 *
 * \return Message description
 */
const HI_CHAR* HI_DRM_INFO_EVENT_GetMessage(HI_DRM_INFO_EVENT_PPTR ppSelf);

/**
 * \breif This is an utility class which wraps the result of communication between device
 * and online DRM server.
 *
 * As a result of DrmManagerClient::processDrmInfo(const DrmInfo*) an instance of
 * DrmInfoStatus would be returned. This class holds DrmBuffer which could be
 * used to instantiate DrmRights in license acquisition.
 *
 * \Notes If the info status object is created by framework, user should destroy
 *        it by calling HI_DRM_INFO_STATUS_Destroy() method.
 */
typedef struct hiDRM_INFO_STATUS_S {
    HI_S32 statusCode;
    HI_S32 infoType;
    HI_DRM_BUFFER_S drmBuffer;
    HI_CHAR* mimeType;
} HI_DRM_INFO_STATUS_S;
typedef HI_DRM_INFO_STATUS_S* HI_DRM_INFO_STATUS_PTR;

/**
 * \breif Constructor for DrmInfo
 *
 * \param[in] infoType Type of information
 * \param[in] drmBuffer Trigger data
 * \param[in] mimeType MIME type
 * \return Returns HI_SUCCESS for success, HI_FAILURE for failure
 */
HI_S32 HI_DRM_INFO_Create(HI_DRM_INFO_PPTR* pppSelf, HI_S32 infoType,
    const HI_DRM_BUFFER_S drmBuffer, const HI_CHAR* mimeType);

/**
 * \breif Destroy the DRM info object.
 *
 * \param[in] pppSelf Handle of DRM info object
 *
 */
HI_VOID HI_DRM_INFO_Destroy(HI_DRM_INFO_PPTR* pppSelf);

/**
 * \breif Constructor for DrmInfoEvent
 *
 * \param[in] uniqueId Unique session identifier
 * \param[in] infoType Type of information
 * \param[in] message Message description
 * \return Returns HI_SUCCESS for success, HI_FAILURE for failure
 */
HI_S32 HI_DRM_INFO_EVENT_Create(HI_DRM_INFO_EVENT_PPTR* pppSelf, HI_S32 infoType, const HI_CHAR* message);

/**
 * \breif Destroy the DRM info event object.
 *
 * \param[in] pppSelf Handle of DRM info enent object
 *
 */
HI_VOID HI_DRM_INFO_EVENT_Destroy(HI_DRM_INFO_EVENT_PPTR* pppSelf);

/**
 * \breif Constructor for DrmInfoRequest
 *
 * \param[in] infoType Type of information
 * \param[in] mimeType MIME type
 * \return Returns HI_SUCCESS for success, HI_FAILURE for failure
 */
HI_S32 HI_DRM_INFO_REQUEST_Create(HI_DRM_INFO_REQUEST_PPTR* pppSelf, HI_S32 infoType, const HI_CHAR* mimeType);

/**
 * \breif Destroy the DRM info request object.
 *
 * \param[in] pppSelf Handle of DRM info request object
 *
 */
HI_VOID HI_DRM_INFO_REQUEST_Destroy(HI_DRM_INFO_REQUEST_PPTR* pppSelf);

/**
 * \breif Destroy the DRM info status object
 * \param[in] infoStatus Pointer of DRM info status object
 */
HI_VOID HI_DRM_INFO_STATUS_Destroy(HI_DRM_INFO_STATUS_PTR infoStatus);

struct OnInfoListener{
    HI_VOID (*onInfo)(struct OnInfoListener* me, HI_DRM_INFO_EVENT_PPTR event);
    const HI_VOID* opaque;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* HI_DRM_INFO_H_ */
