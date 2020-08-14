/*
 * hi_drm_common.h
 *
 *  Created on: Jul 31, 2012
 *      Author: zhangyi
 */

#ifndef HI_DRM_COMMON_H_
#define HI_DRM_COMMON_H_

#include <sys/types.h>
#include <hi_type.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

//------------------------------------------------------------------------------
/**
 * \breif Macros defined by Playready
 */

/**
 * \breif Define the mimetype of playraedy
 * {HI_KEY_PDY_MimeType, "application/vnd.ms-playready"}
 */
extern const HI_CHAR* HI_KEY_PDY_MimeType;
extern const HI_CHAR* HI_KEY_WMDRM_MimeType;
/**
 * \breif key strings used by playready open
 */
extern const HI_CHAR* HI_KEY_PDY_DrmPath;
extern const HI_CHAR* HI_KEY_PDY_LocalStoreFile;
extern const HI_CHAR* HI_KEY_PDY_OriginalFile;
extern const HI_CHAR* HI_KEY_PDY_SercureClockServerIP;
extern const HI_CHAR* HI_KEY_PDY_SecureServerPort;
extern const HI_CHAR* HI_KEY_PDY_ServerIP;
extern const HI_CHAR* HI_KEY_PDY_ServerPort;
extern const HI_CHAR* HI_KEY_PDY_ResponseFile;
extern const HI_CHAR* HI_KEY_PDY_SecretData;
extern const HI_CHAR* HI_KEY_PDY_ProtectionType;
extern const HI_CHAR* HI_KEY_PDY_KeyID;
extern const HI_CHAR* HI_KEY_PDY_LicenseURL;
extern const HI_CHAR* HI_KEY_PDY_EncryVer2Data;
extern const HI_CHAR* HI_KEY_PDY_EncryVersion;
extern const HI_CHAR* HI_KEY_PDY_EncryType;
extern const HI_CHAR* HI_KEY_PDY_InitiatorFile;
extern const HI_CHAR* HI_KEY_PDY_DataSize;
extern const HI_CHAR* HI_KEY_PDY_KeyFilePath;
extern const HI_CHAR* HI_KEY_PDY_EnvelopFile;
extern const HI_CHAR* HI_KEY_PDY_EnvelopOutputFile;
extern const HI_CHAR* HI_VAL_PDY_EncryVersion1;
extern const HI_CHAR* HI_VAL_PDY_EncryVersion2;
extern const HI_CHAR* HI_VAL_PDY_EncryTypeWMDRM;
extern const HI_CHAR* HI_VAL_PDY_EncryTypePlayReady;

#define SVR_DRM_TYPE_WIDEVINE   "video/wvm"
#define SVR_DRM_TYPE_PLAYREADY  "video/playready"

/*PlayReady Certification Buffer define*/
typedef struct tagHI_PLAYREADY_DEVCERT_BUF_S HI_PLAYREADY_DEVCERT_BUF_S;
struct tagHI_PLAYREADY_DEVCERT_BUF_S
{
    HI_U8*      pu8Pri;
    HI_S32      s32PriLen;
    HI_U8*      pu8Zgpriv;
    HI_S32      s32ZgprivLen;
    HI_U8*      pu8Bgroupcert;
    HI_S32      s32BgroupcertLen;
    HI_U8*      pu8Devcerttemplate;
    HI_S32      s32DevcerttemplateLen;
    HI_VOID     (*pfnDestructor)(HI_PLAYREADY_DEVCERT_BUF_S* thiz);
};
/*End of playready defines*/
//------------------------------------------------------------------------------


enum {
    HI_DRM_STATUS_OK = 1,
    HI_DRM_STATUS_ERROR = 2,
};
/**
 * \breif Error code for DRM Frameowrk
 */
typedef enum hiDRM_ERROR_E{
    // The following constant values should be in sync with
    // media/stagefright/MediaErrors.h
    HI_DRM_ERROR_BASE = -2000,

    HI_DRM_ERROR_UNKNOWN                       = HI_DRM_ERROR_BASE,
    HI_DRM_ERROR_NO_LICENSE                    = HI_DRM_ERROR_BASE - 1,
    HI_DRM_ERROR_LICENSE_EXPIRED               = HI_DRM_ERROR_BASE - 2,
    HI_DRM_ERROR_SESSION_NOT_OPENED            = HI_DRM_ERROR_BASE - 3,
    HI_DRM_ERROR_DECRYPT_UNIT_NOT_INITIALIZED  = HI_DRM_ERROR_BASE - 4,
    HI_DRM_ERROR_DECRYPT                       = HI_DRM_ERROR_BASE - 5,
    HI_DRM_ERROR_CANNOT_HANDLE                 = HI_DRM_ERROR_BASE - 6,
    HI_DRM_ERROR_TAMPER_DETECTED               = HI_DRM_ERROR_BASE - 7,
    HI_DRM_ERROR_NO_PERMISSION                 = HI_DRM_ERROR_BASE - 8,
    HI_DRM_ERROR_NOT_SUPPORT                   = HI_DRM_ERROR_BASE - 9,
    HI_DRM_NO_ERROR                            = HI_SUCCESS
} HI_DRM_ERROR_E;

/**
 * \breif copy control settings used in DecryptHandle::copyControlVector
 */
typedef enum hiDRM_COPY_CONTROL_E {
    HI_DRM_COPY_CONTROL_BASE = 1000,
    // the key used to set the value for HDCP
    // if the associated value is 1, then HDCP is required
    // otherwise, HDCP is not required
    HI_DRM_COPY_CONTROL_HDCP = HI_DRM_COPY_CONTROL_BASE
} HI_DRM_COPY_CONTROL_E;

/**
 * \breif Defines DRM Buffer
 */
typedef struct hiDRM_BUFFER_S {
    HI_CHAR* data;
    HI_U32   length;
} HI_DRM_BUFFER_S;

/** Type of the stream data that is parsed by the file DEMUX */
/** CNcomment:流类型，文件解析器解析出来的流数据类型 */
typedef enum hiDRM_FORMAT_DATA_TYPE_E
{
    HI_DRM_FORMAT_DATA_DEFAULT,
    HI_DRM_FORMAT_DATA_AUD,          /**< Audio stream *//**< CNcomment:音频流 */
    HI_DRM_FORMAT_DATA_VID,          /**< Video stream *//**< CNcomment:视频流 */
    HI_DRM_FORMAT_DATA_BUTT
} HI_DRM_FORMAT_DATA_TYPE_E;

typedef struct hiDRM_DECRYPT_EXTRADATA_S {
    HI_DRM_BUFFER_S* IV;
    HI_DRM_BUFFER_S* key;
    HI_U32   u32length;
    HI_S32   s32CryptoMode;
    HI_S32   *pEncryptUnitInfo;
    HI_S32   s32EncryptSize;
    HI_DRM_FORMAT_DATA_TYPE_E emStreamType;
} HI_DRM_DECRYPT_EXTRADATA_S;


#define DECLARE_DRM_BUFFER(name, _data, _len) \
    HI_DRM_BUFFER_S name = {\
        .data = (HI_CHAR*)_data,\
        .length = (HI_U32)_len,\
}

/**
 * \breif Defines detailed description of the action
 */
typedef struct hiACTION_DESCRIPTION_S {
    HI_S32 outputType;   /* BLUETOOTH , HDMI*/
    HI_S32 configuration; /* RESOLUTION_720_480 , RECORDABLE etc.*/
} HI_ACTION_DESCRIPTION_S;

/**
 * \breif Defines constants related to DRM types
 */
typedef enum hiDRM_OBJECT_TYPE_E {
    /**
     * Field specifies the unknown type
     */
    HI_DRM_OBJECT_TYPE_UNKNOWN = 0x00,
    /**
     * Field specifies the protected content type
     */
    HI_DRM_OBJECT_TYPE_CONTENT = 0x01,
    /**
     * Field specifies the rights information
     */
    HI_DRM_OBJECT_TYPE_RIGHTS = 0x02,
    /**
     * Field specifies the trigger information
     */
    HI_DRM_OBJECT_TYPE_TRIGGER = 0x03,
} HI_DRM_OBJECT_TYPE_E;

/**
 * \breif Defines constants related to play back
 */
typedef enum hiPLAY_BACK_E {
    /**
     * Constant field signifies playback start
     */
    HI_PLAY_BACK_START = 0x00,
    /**
     * Constant field signifies playback stop
     */
    HI_PLAY_BACK_STOP = 0x01,
    /**
     * Constant field signifies playback paused
     */
    HI_PLAY_BACK_PAUSE = 0x02,
    /**
     * Constant field signifies playback resumed
     */
    HI_PLAY_BACK_RESUME = 0x03,
} HI_PLAY_BACK_E;

/**
 * \breif Defines actions that can be performed on protected content
 */
typedef enum hiACTION_E {
    /**
     * Constant field signifies that the default action
     */
    HI_ACTION_DEFAULT = 0x00,
    /**
     * Constant field signifies that the content can be played
     */
    HI_ACTION_PLAY = 0x01,
    /**
     * Constant field signifies that the content can be set as ring tone
     */
    HI_ACTION_RINGTONE = 0x02,
    /**
     * Constant field signifies that the content can be transfered
     */
    HI_ACTION_TRANSFER = 0x03,
    /**
     * Constant field signifies that the content can be set as output
     */
    HI_ACTION_OUTPUT = 0x04,
    /**
     * Constant field signifies that preview is allowed
     */
    HI_ACTION_PREVIEW = 0x05,
    /**
     * Constant field signifies that the content can be executed
     */
    HI_ACTION_EXECUTE = 0x06,
    /**
     * Constant field signifies that the content can displayed
     */
    HI_ACTION_DISPLAY = 0x07,
} HI_ACTION_E;


/**
 * \breif Defines constants related to status of the rights
 */
typedef enum hiRIGHTS_STATUS_E {
    /**
     * Constant field signifies that the rights are valid
     */
    HI_DRM_RIGHTS_VALID = 0x00,
    /**
     * Constant field signifies that the rights are invalid
     */
    HI_DRM_RIGHTS_INVALID = 0x01,
    /**
     * Constant field signifies that the rights are expired for the content
     */
    HI_DRM_RIGHTS_EXPIRED = 0x02,
    /**
     * Constant field signifies that the rights are not acquired for the content
     */
    HI_DRM_RIGHTS_NOT_ACQUIRED = 0x03,
} HI_DRM_RIGHTS_STATUS_E;

/**
 * \breif Defines API set for decryption
 */
typedef enum hiDECRYPT_API_TYPE_E {
    /**
     * Decrypt API set for non encrypted content
     */
    HI_DECRYPT_API_NON_ENCRYPTED = 0x00,
    /**
     * Decrypt API set for ES based DRM
     */
    HI_DECRYPT_API_ELEMENTARY_STREAM_BASED = 0x01,
    /**
     * POSIX based Decrypt API set for container based DRM
     */
    HI_DECRYPT_API_CONTAINER_BASED = 0x02,
    /**
     * Decrypt API for Widevine streams
     */
    HI_DECRYPT_API_WV_BASED = 0x3,
} HI_DECRYPT_API_TYPE_E;

/**
 * \breif Defines decryption information
 */
typedef struct hiDECRYPT_INFO_S {
    /**
     * size of memory to be allocated to get the decrypted content.
     */
    HI_S32 decryptBufferLength;
    /**
     * reserved for future purpose
     */
} HI_DECRYPT_INFO_S;

/**
 * \breif Defines Iterator type
 */
typedef struct hiITERATOR_S HI_ITERATOR_S;
struct hiITERATOR_S {
    HI_BOOL         (*pfnHasNext)(HI_ITERATOR_S* self);
    const HI_CHAR*  (*pfnNext)(HI_ITERATOR_S* self);
    HI_S32          index;
    HI_VOID*        opaque;
};


static inline HI_BOOL HI_ITERATOR_HasNext(HI_ITERATOR_S* self)
{
    return self->pfnHasNext(self);
}

static inline const HI_CHAR*  HI_ITERATOR_Next(HI_ITERATOR_S* self)
{
    return self->pfnNext(self);
}

/**
 * \breif Defines decryption handle
 */
typedef struct hiDECRYPT_HANDLE_S * HI_DECRYPT_HANDLE_PTR;
typedef struct hiDECRYPT_HANDLE_S {
    /**
     * Decryption session Handle
     */
    HI_S32 decryptId;
    /**
     * Mimetype of the content to be used to select the media extractor
     * For e.g., "video/mpeg" or "audio/mp3"
     */
    HI_CHAR* mimeType;
    /**
     * Defines which decryption pattern should be used to decrypt the given content
     * DrmFramework provides two different set of decryption APIs.
     *   1. Decrypt APIs for elementary stream based DRM
     *      (file format is not encrypted but ES is encrypted)
     *         e.g., Marlin DRM (MP4 file format), WM-DRM (asf file format)
     *
     *         DecryptApiType::ELEMENTARY_STREAM_BASED
     *             Decryption API set for ES based DRM
     *                 initializeDecryptUnit(), decrypt(), and finalizeDecryptUnit()
     *   2. Decrypt APIs for container based DRM (file format itself is encrypted)
     *         e.g., OMA DRM (dcf file format)
     *
     *         DecryptApiType::CONTAINER_BASED
     *             POSIX based Decryption API set for container based DRM
     *                 pread()
     */
    HI_S32 decryptApiType;
    /**
     * Defines the status of the rights like
     *     RIGHTS_VALID, RIGHTS_INVALID, RIGHTS_EXPIRED or RIGHTS_NOT_ACQUIRED
     */
    HI_S32 status;
    /**
     * Information required to decrypt content
     * e.g. size of memory to be allocated to get the decrypted content.
     */
    HI_DECRYPT_INFO_S* decryptInfo;
    /**
     * Defines a vector for the copy control settings sent from the DRM plugin
     * to the player
     */
    HI_ITERATOR_S copyControlVector;

    /**
     * Defines a vector for any extra data the DRM plugin wants to send
     * to the native code
     */
    HI_ITERATOR_S extendedData;
} HI_DECRYPT_HANDLE_S;
//------------------------------------------------------------------------------
/**
 * \breif This is an utility class which wraps the status of the conversion, the converted
 * data/checksum data and the offset. Offset is going to be used in the case of close
 * session where the agent will inform where the header and body signature should be added
 *
 * As a result of DrmManagerClient::convertData(HI_S32, const DrmBuffer*) and
 * DrmManagerClient::closeConvertSession(HI_S32) an instance of DrmConvertedStatus
 * would be returned.
 *
 */
// Should be in sync with DrmConvertedStatus.java
enum {
    HI_DRM_CONVERTED_STATUS_OK = 1,
    HI_DRM_CONVERTED_STATUS_INPUTDATA_ERROR = 2,
    HI_DRM_CONVERTED_TATUS_ERROR = 3,
};
typedef struct hiDRM_CONVERTED_STATUS_S {
    HI_S32 statusCode;
    const HI_DRM_BUFFER_S convertedData;
    HI_S32 offset;
} HI_DRM_CONVERTED_STATUS_S;
typedef HI_DRM_CONVERTED_STATUS_S* HI_DRM_CONVERTED_STATUS_PTR;
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* HI_DRM_COMMON_H_ */
