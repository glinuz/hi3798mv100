/**
 \file
 \brief File demutiplexer (DEMUX), protocol structure. CNcomment:�ļ���������Э��ṹ��CNend
 \author HiSilicon Technologies Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author
 \date 2010-02-10
 */

#ifndef __HI_SVR_FORMAT_H__
#define __HI_SVR_FORMAT_H__

#include "hi_type.h"
#include "hi_svr_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/
/** \addtogroup       Hiplayer */
/** @{ */  /** <!-- [Hiplayer] */

/** Reach the end of a file. CNcomment:�����ļ�����λ�� CNend*/
#define HI_FORMAT_ERRO_ENDOFFILE            (HI_FAILURE - 1)

/** Return the file size(byte). CNcomment:�����ļ�ʵ�ʴ�С����λbyte CNend*/
#define HI_FORMAT_SEEK_FILESIZE             (65536)

/** @} */  /*! <!-- Macro Definition end */


/*************************** Structure Definition ****************************/
/** \addtogroup      Hiplayer  */
/** @{ */  /** <!-- [Hiplayer] */

/** Maximum number of characters in a uniform resource locator (URL) */
/** CNcomment: ���·������ (�ַ�����)*/
#define HI_FORMAT_MAX_URL_LEN               (2048)
/** Maximum number of characters  in a file name  */
/** CNcomment:����ļ����Ƴ��� (�ַ�����)*/
#define HI_FORMAT_MAX_FILE_NAME_LEN         (512)
/** Maximum number of characters in the information such as the file title and author */
/** CNcomment:�ļ����⡢���ߵ�ռ�õ�����ַ��� */
#define HI_FORMAT_TITLE_MAX_LEN             (512)
/** Maximum number of characters in a language name*/
/** CNcomment:�������ƺ��е�����ַ����� */
#define HI_FORMAT_LANG_LEN                  (4)
/** Maximum number of characters  in a subtitle title */
/** CNcomment:��Ļ�������󳤶�(�ַ�����) */
#define HI_FORMAT_SUB_TITLE_LEN             (32)
/** Maximum number of supported languages */
/** CNcomment:֧�ֵ��������������� */
#define HI_FORMAT_MAX_LANG_NUM              (32)
/** No presentation time stamp (PTS) */
/** CNcomment:û��pts */
#define HI_FORMAT_NO_PTS                    (-1)
/** The value of the position is invalid */
/** CNcomment:û��pos */
#define HI_FORMAT_NO_POS                    (-1)
/** The value of the index is invalid*/
/** CNcomment:û��index*/
#define HI_FORMAT_NO_INDEX                  (-1)
/** Invalid stream ID */
/** CNcomment:�Ƿ�streamid */
#define HI_FORMAT_INVALID_STREAM_ID         (-1)
/** Maximum bytes of palette, rgb+a */
/** CNcomment:��ɫ��ռ�õ�����ֽ�����rgb+a */
#define HI_FORMAT_PALETTE_LEN               (4 * 256)
/** Maximum planar surfaces for saving the data of the thumbnail */
/** CNcomment:�������ͼͼ��������������ƽ���� */
#define HI_FORMAT_THUMBNAIL_PLANAR          (4)
/** Maximum number of characters in a protocol name */
/** CNcomment: Э��������󳤶� (�ַ�����)*/
#define HI_FORMAT_PROTOCAL_NAME_LEN         (32)
/** Maximum number of characters in a subtitle type */
/** CNcomment: ��Ļ���͵���󳤶�(�ַ�����)*/
#define HI_FORMAT_SUB_TYPE_LEN              (32)
/** Maximum number of supported languages in a subtitle file */
/** CNcomment: һ����Ļ�ļ���������Ը��� */
#define HI_FORMAT_MAX_LANGUAGE_NUM          (4)
/** IV property len is hls segment */
/** CNcomment: hls�з�Ƭ����IV����󳤶� */
#define HI_FORMAT_AES_IV_LEN                (16)
/** the mask of mpeg dash descriptor's scheme */
/** CNcomment: mpeg dash descriptor �� scheme ��maskֵ */
#define HI_FORMAT_DESCRIPTOR_SCHEME_MASK (0xFF000000)
/** the mask of mpeg dash descriptor's value */
/** CNcomment: mpeg dash descriptor ��value ��maskֵ */
#define HI_FORMAT_DESCRIPTOR_VALUE_MASK (0x00FFFFFF)
/** number of service descriptor in the information such as service name, service provider */
/** CNcomment: ��Ŀ�������ƻ�����ṩ�������ַ��� */
#define HI_FORMAT_SERVICE_DESCRIPT_LEN      (64)
/** the length of mimetype info */
/** CNcomment: mimetype ��Ϣ���ݳ��� */
#define HI_FORMAT_MIMETYPE_LEN (256)
/** the max number of DRM header info*/
/** CNcomment: DRM��Ϣͷ�����ֵ */
#define HI_FORMAT_MAX_DRM_NB (32)
/** the max number of chapters*/
/** CNcomment: ֧�ֵ�����½���Ŀ */
#define HI_FORMAT_MAX_CHAPTERS_NUM (128)


/** LOG TAG */
/** CNcomment: ��־���TAG*/
#define HI_FORMAT_TAG_SWITCH_PG      "SWITCH-PG "
#define HI_FORMAT_TAG_SEEK           "SEEK2 "

/*************************** Structure Definition ****************************/
/** Version number definition */
/** CNcomment:�汾�Ŷ��� */
typedef struct hiFORMAT_LIB_VERSION_S
{
    HI_U8 u8VersionMajor;    /**< Major version accessor element */
    HI_U8 u8VersionMinor;    /**< Minor version accessor element */
    HI_U8 u8Revision;        /**< Revision version accessor element */
    HI_U8 u8Step;            /**< Step version accessor element */
} HI_FORMAT_LIB_VERSION_S;

/** log level */
/** CNcomment:��־�ȼ� */
typedef enum hiFORMAT_LOG_LEVEL_E
{
    HI_FORMAT_LOG_QUITE = 0,    /**< no log output */
    HI_FORMAT_LOG_FATAL,        /**< something went wrong and recovery is not possible. */
    HI_FORMAT_LOG_ERROR,        /**< something went wrong and cannot losslessly be recovered. */
    HI_FORMAT_LOG_WARNING,      /**< something does not look correct. This may or may not lead to problems. */
    HI_FORMAT_LOG_INFO,         /**< level is info */
    HI_FORMAT_LOG_DEBUG,        /**< level is debug */
    HI_FORMAT_LOG_VERBOSE,      /**< level is verbose */
} HI_FORMAT_LOG_LEVEL_E;

/** hls start mode */
/** CNcomment:hls ����ģʽ */
typedef enum hiFORMAT_HLS_START_MODE_E
{
    HI_FORMAT_HLS_MODE_NORMAL,  /**< manual mode *//**< CNcomment:��ͨģʽ,������Ƭ��ʼ�� */
    HI_FORMAT_HLS_MODE_FAST,    /**< auto mode,switch by the bandwidth *//**< CNcomment:��������,����С��Ƭ��ʼ�� */
    HI_FORMAT_HLS_MODE_BUTT
} HI_FORMAT_HLS_START_MODE_E;

/** File opening mode */
/** CNcomment:�ļ��򿪷�ʽ */
typedef enum hiFORMAT_URL_OPENFLAG_E
{
    HI_FORMAT_URL_OPEN_RDONLY = 0x0,   /**< Read-only mode *//**< CNcomment:ֻ����ʽ */
    HI_FORMAT_URL_OPEN_RDWR,           /**< Read/Write mode *//**< CNcomment:��/д��ʽ */
    HI_FORMAT_URL_OPEN_BUTT
} HI_FORMAT_URL_OPENFLAG_E;

/** File seeking flag */
/** CNcomment:�ļ�SEEK��� */
typedef enum hiFORMAT_SEEK_FLAG_E
{
    HI_FORMAT_AVSEEK_FLAG_NORMAL = 0x0,     /**< Seek the key frame that is closest to the current frame in a sequence from the current frame to end of the file. *//**< CNcomment:�ӵ�ǰ֡���ļ�βseek���ҵ��뵱ǰ֡����Ĺؼ�֡ */
    HI_FORMAT_AVSEEK_FLAG_BACKWARD,         /**< Seek the key frame that is closest to the current frame in a sequence from the current frame to the file header. *//**< CNcomment:�ӵ�ǰ֡���ļ�ͷseek���ҵ��뵱ǰ֡����Ĺؼ�֡ */
    HI_FORMAT_AVSEEK_FLAG_BYTE,             /**< Seek the key frame based on bytes. *//**< CNcomment:���ֽ�seek */
    HI_FORMAT_AVSEEK_FLAG_ANY,              /**< Seek the key frame that is closest to the current frame regardless of the sequence from the current frame to the file header or from the current frame to the end of the file. *//**< CNcomment:���������ļ�ͷ�����ļ�β���ҵ��뵱ǰ֡����Ĺؼ�֡ */
    HI_FORMAT_AVSEEK_FLAG_BUTT
} HI_FORMAT_SEEK_FLAG_E;

/** Type of the stream data that is parsed by the file DEMUX */
/** CNcomment:�����ͣ��ļ��������������������������� */
typedef enum hiFORMAT_DATA_TYPE_E
{
    HI_FORMAT_DATA_NULL = 0x0,
    HI_FORMAT_DATA_AUD,          /**< Audio stream *//**< CNcomment:��Ƶ�� */
    HI_FORMAT_DATA_VID,          /**< Video stream *//**< CNcomment:��Ƶ�� */
    HI_FORMAT_DATA_SUB,          /**< Subtitle stream *//**< CNcomment:��Ļ�� */
    HI_FORMAT_DATA_RAW,          /**< Byte stream *//**< CNcomment:�ֽ��� */
    HI_FORMAT_DATA_BUTT
} HI_FORMAT_DATA_TYPE_E;

/** mpeg dash Role descriptor scheme , take the most significant 8 bits of 32bits, refer to HI_FORMAT_DESCRIPTOR_SCHEME_MASK*/
/** CNcomment: mpeg dash Role descriptor ����ֵ,ռ��32λ�еĸ�8λ����ο�HI_FORMAT_DESCRIPTOR_SCHEME_MASK*/
typedef enum hiFORMAT_ROLE_SCHEME_E
{
    HI_FORMAT_ROLE_SCHEME_NONE = 0,
    HI_FORMAT_ROLE_SCHEME_URN_MPEG_DASH_2011 = (1 << 24), /**<  urn:mpeg:dash:role:2011*/
} HI_FORMAT_ROLE_SCHEME_E;

/** mpeg dash Role descriptor value ,take the least 24bits of 32bits, refer to HI_FORMAT_DESCRIPTOR_VALUE_MASK*/
/** CNcomment: mpeg dash Role descriptor ����ֵ��ռ��32λ�еĵ�24λ����ο�HI_FORMAT_DESCRIPTOR_VALUE_MASK*/
typedef enum hiFORMAT_ROLE_VALUE_E
{
    HI_FORMAT_ROLE_VALUE_NONE                 = 0,
    HI_FORMAT_ROLE_VALUE_MAIN                 = (1 << 0),
    HI_FORMAT_ROLE_VALUE_ALTERNATE         = (1 << 1),
    HI_FORMAT_ROLE_VALUE_SUPPLEMENTARY = (1 << 2),
    HI_FORMAT_ROLE_VALUE_COMMENTARY     = (1 << 3),
    HI_FORMAT_ROLE_VALUE_DUB                   = (1 << 4),
    HI_FORMAT_ROLE_VALUE_CAPTION            = (1 << 5),
    HI_FORMAT_ROLE_VALUE_SUBTITLE           = (1 << 6),
    HI_FORMAT_ROLE_VALUE_OTHER               = (1 << 23),
} HI_FORMAT_ROLE_VALUE_E;

/** mpeg dash Accessibility descriptor scheme, take the most significant 8 bits of 32bits, refer to HI_FORMAT_DESCRIPTOR_SCHEME_MASK*/
/** CNcomment: mpeg dash Accessibility descriptor scheme, ռ��32λ�еĸ�8λ����ο�HI_FORMAT_DESCRIPTOR_SCHEME_MASK*/
typedef enum hiFORMAT_ACCESSIBILITY_SCHEME_E
{
    HI_FORMAT_ACCESSIBILITY_SCHEME_NONE = 0,
    HI_FORMAT_ACCESSIBILITY_SCHEME_TVA_AUDIO_PURPOSE_CS_2007 = (1 << 24), /**<  urn:tva:metadata:cs:AudioPurposeCS:2007*/
} HI_FORMAT_ACCESSIBILITY_SCHEME_E;


/** Source type of the file to be played */
/** CNcomment:�ļ�Դ���� */
typedef enum hiFORMAT_SOURCE_TYPE_E
{
    HI_FORMAT_SOURCE_LOCAL = 0x0,   /**< Local file *//**< CNcomment:��ͨ�ļ� */
    HI_FORMAT_SOURCE_NET_VOD,       /**< Net VOD file *//**< CNcomment:����㲥 */
    HI_FORMAT_SOURCE_NET_LIVE,      /**< Net Live stream *//**< CNcomment:�������ļ� */
    HI_FORMAT_SOURCE_BUTT
} HI_FORMAT_SOURCE_TYPE_E;

/** Stream type of the file to be played */
/** CNcomment:�ļ������ͣ����ŵ��ļ����� */
typedef enum hiFORMAT_STREAM_TYPE_E
{
    HI_FORMAT_STREAM_ES = 0x0,    /**< Element stream (ES) file *//**< CNcomment:es���ļ� */
    HI_FORMAT_STREAM_TS,          /**< TS file *//**< CNcomment:ts���ļ� */
    HI_FORMAT_STREAM_BUTT
} HI_FORMAT_STREAM_TYPE_E;

/** Seek mode */
/** CNcomment:seek ���� */
typedef enum hiFORMAT_SEEK_MODE_E
{
    HI_FORMAT_SEEK_MODE_PTS = 0x0,     /**< seek by pts, default value *//**< CNcomment:Ĭ�ϰ�PTS��seek*/
    HI_FORMAT_SEEK_MODE_POS,           /**< seek by pos only if network stream and without index *//**< CNcomment:ֻ�е����粥���������ļ�ʱ�Ű�POS��seek */
    HI_FORMAT_SEEK_MODE_BUTT
} HI_FORMAT_SEEK_MODE_E;

/** HLS segment encryption type */
/** CNcomment:HLS ��Ƭ�������� */
typedef enum hiFORMAT_HLS_KEY_TYPE_E
{
    KEY_NONE,
    KEY_AES_128,
} HI_FORMAT_HLS_KEY_TYPE_E;

/** Playlist Type */
/** CNcomment:�����б����� */
typedef enum hiFORMAT_PLAYLIST_TYPE_E
{
    HI_FORMAT_PLAYLIST_TYPE_M3U8 = 0x0,     /**<M3U8 type *//**< CNcomment:m3u8���� */
    HI_FORMAT_PLAYLIST_TYPE_M3U9,           /**< M3u9 type *//**< CNcomment:m3u9���� */
    HI_FORMAT_PLAYLIST_TYPE_NO_PLAYLIST     /**< NoPlayList type *//**< CNcomment:�ǲ����б����� */
} HI_FORMAT_PLAYLIST_TYPE_E;

/** Stream information of the hls file */
/** CNcomment:hls�ļ���stream����Ϣ */
typedef struct hiFORMAT_HLS_STREAM_INFO_S
{
    HI_S32     stream_nb;                   /**< Input parameter, index of the stream to be obtained *//**< CNcomment:Ҫ��ȡ�����������ţ�������� */
    HI_S32     hls_segment_nb;              /**< Output parameter, total segments of this stream *//**< CNcomment:��·�����ܹ��ж��ٸ���Ƭ��������� */
    HI_S64     bandwidth;                   /**< Output parameter, minimum bandwidth of the stream to be obtained*//**<  CNcomment:��ȡ������Ҫ�����ʹ����������������λbits/s */
    HI_CHAR    url[HI_FORMAT_MAX_URL_LEN];  /**< Output parameter, url of the index file of the stream to be obtained *//**< CNcomment:��ȡ�������������ļ���url��������� */
} HI_FORMAT_HLS_STREAM_INFO_S;

/** The information of the currently played segment */
/** CNcomment:hls�ļ���ǰ���ŵķ�Ƭ����Ϣ */
typedef struct hiFORMAT_HLS_SEGMENT_INFO_S
{
    HI_S32    stream_num;                    /**< output parameter, which stream of the currently played segment *//**< CNcomment:��ǰ���ŵķ�Ƭ������һ·stream��������� */
    HI_S32    total_time;                    /**< output parameter, the duration of the currently played segment *//**< CNcomment:��ǰ���ŵķ�Ƭ��ʱ����������� */
    HI_S32    seq_num;                       /**< output parameter, the serial number of the current segment *//**< CNcomment:��ǰ��Ƭ�����кţ�������� */
    HI_S64    bandwidth;                     /**< output parameter, the bandwidth demands of the current segment *//**< CNcomment:��ǰ��ƬҪ��Ĵ����������������λbits/s */
    HI_CHAR   url[HI_FORMAT_MAX_URL_LEN];    /**< output parameter, the url of the current segment *//**< CNcomment:��ǰ��Ƭ��url��������� */
} HI_FORMAT_HLS_SEGMENT_INFO_S;

/** The information of the playlist segment detail info */
/** CNcomment:playlist�ļ���Ƭ����ϸ��Ϣ */
typedef struct hiFORMAT_PLAYLIST_SEGMENT_DETAIL_INFO_S
{
    HI_S32                      start_time;                 /**< output parameter, the offset time of the segment in all segments *//**< CNcomment:��ǰ��Ƭ�Ŀ�ʼʱ�������з�Ƭ�е�ƫ��ʱ��,������� */
    HI_S32                      total_time;                 /**< output parameter, the duration of the currently played segment *//**< CNcomment:��ǰ���ŵķ�Ƭ��ʱ����������� */
    HI_CHAR                     url[HI_FORMAT_MAX_URL_LEN]; /**< output parameter, the url of the current segment *//**< CNcomment:��ǰ��Ƭ��url��������� */
    HI_CHAR                     key[HI_FORMAT_MAX_URL_LEN]; /**< output parameter, the decryption key of current segment *//**< CNcomment:��ǰ��Ƭ�Ľ���KEY,������� */
    HI_FORMAT_HLS_KEY_TYPE_E    key_type;                   /**< output parameter, the encryption type of current segment *//**< CNcomment:��ǰ��Ƭ�ļ������ͣ�������� */
    HI_U8                       iv[HI_FORMAT_AES_IV_LEN];   /**< output parameter, the iv info of encryption of current segment *//**< CNcomment:��ǰ��Ƭ��IV�������� */
} HI_FORMAT_PLAYLIST_SEGMENT_DETAIL_INFO_S;

/** The information of the playlist stream detail info */
/** CNcomment:playlist����ϸ��Ϣ */
typedef struct hiFORMAT_PLAYLIST_STREAM_DETAIL_S
{
    HI_S32  bandwidth;                                                /**< output parameter, the bandwidth demands of the current stream *//**< CNcomment:��ǰ��ѡ��Ҫ��Ĵ����������������λbits/s */
    HI_CHAR url[HI_FORMAT_MAX_URL_LEN];                               /**< output parameter, the url of the current stream *//**< CNcomment:��ǰ��ѡ����url��������� */
    HI_FORMAT_PLAYLIST_TYPE_E  playlist_type;                         /**< output parameter, playlist type *//**< CNcomment:�����б�����*/
    struct tagSEGMENT_LIST_S
    {
        HI_S32                                      index;            /**< output parameter, the stream index *//**< CNcomment:��ǰ��ѡ����������,������� */
        HI_S32                                      finished;         /**< output parameter, if is live stream,0 is live *//**< CNcomment:��ǰ��ѡ���Ƿ���ֱ����,0��ֱ���� */
        HI_S32                                      target_duration;  /**< output parameter, the max duration of all segments *//**< CNcomment:��Ƭ�����ʱ�� */
        HI_S32                                      segment_nb;       /**< Output parameter, total segments of this stream *//**< CNcomment:��·�����ܹ��ж��ٸ���Ƭ��������� */
        HI_FORMAT_PLAYLIST_SEGMENT_DETAIL_INFO_S    **segments;       /**< Output parameter, segments info *//**< CNcomment:��Ƭ��Ϣ��������� */
    } SEGMENT_LIST_S;
} HI_FORMAT_PLAYLIST_STREAM_DETAIL_S;

/**The highest resolution scanning media restrictions*/
/**CNcomment:�ֱ���*/
typedef struct hiFORMAT_RESOLUTION_RATIOS
{
    HI_S32      s32Width;   /**< when use for thumbnail restrcit, zero means no limit, not less than zero *//**< CNcomment:��������ý��ɨ��ʱ, 0��ʾ������, ����С��0*/
    HI_S32      s32Height;  /**< when use for thumbnail restrcit, zero means no limit, not less than zero *//**< CNcomment:��������ý��ɨ��ʱ, 0��ʾ������, ����С��0*/
}HI_FORMAT_RESOLUTION_RATIOS;

/**The information of a chapter*/
/**CNcomment:�½���Ϣ*/
typedef struct hiFORMAT_CHAPTER_ATOM_S
{
    HI_S64      s64UID;         /**< unique id of the chapter *//**< CNcomment:��ʾ�½ڵ�ΨһID */
    HI_S32      s32TimeStart;   /**< timestamp of the start of chapter, unit: ms*//**< CNcomment:�½ڿ�ʼʱ��,��λms */
    HI_S32      s32TimeEnd;     /**< timestamp of the end of chapter, unit: ms*//**< CNcomment:�½ڽ���ʱ��,��λms */
    HI_CHAR*    pszTitle;       /**< the title of the chapter *//**< CNcomment:�½ڱ���*/
    HI_CHAR*    pszLanguage;    /**< the language corresponding to the title *//**< CNcomment: �½ڱ������������*/
}HI_FORMAT_CHAPTER_ATOM_S;

typedef struct hiFORMAT_CHAPTER_INFO_S
{
    HI_S32                      s32ChapterNum;                          /**< the total number of chapter*//**< CNcomment:�½�����Ŀ */
    HI_FORMAT_CHAPTER_ATOM_S    stChapter[HI_FORMAT_MAX_CHAPTERS_NUM];  /**< chapter atom*//**< CNcomment:���½ھ�����Ϣ*/
}HI_FORMAT_CHAPTER_INFO_S;


/** Command of the invoking */
/** CNcomment:invoke�������� */
typedef enum hiFORMAT_INVOKE_ID_E
{
    HI_FORMAT_INVOKE_PRE_CLOSE_FILE = 0x0,          /**< the command of the file to be closed, no parameter*//**< CNcomment:Ԥ�ر��ļ����޲��� */
    HI_FORMAT_INVOKE_SET_SOURCE_CODETYPE = 3,       /**< the command of setting the encoding type of the external subtitle. The parameter type is ::HI_CHARSET_CODETYPE_E* *//**< CNcomment:���������Ļ�ַ��������ͣ���������::HI_CHARSET_CODETYPE_E* */
    HI_FORMAT_INVOKE_SET_DEST_CODETYPE,             /**< the command of setting the encoding type of the external subtitle to be transferred to, the parameter is ::HI_CHARSET_CODETYPE_E* *//**< CNcomment:������ĻҪת�ɵı������ͣ���ת�ɵ�Ŀ������,��������::HI_CHARSET_CODETYPE_E* */
    HI_FORMAT_INVOKE_SET_CHARSETMGR_FUNC,           /**< the command of setting the function of the charset manager, the parameter is ::HI_CHARSET_FUN_S* *//**< CNcomment:�����ַ������������ӿ�.��������::HI_CHARSET_FUN_S *charsetmgr */
    HI_FORMAT_INVOKE_GET_METADATA,                  /**< the command of getting the metadata of the media file, the parameter is ::HI_SVR_PLAYER_METADATA_S* *//**< CNcomment:��ȡý���ļ�Ԫ���ݣ���������::HI_SVR_PLAYER_METADATA_S* */
    HI_FORMAT_INVOKE_GET_THUMBNAIL,                 /**< the command of getting the thumbnail, the parameter is ::HI_FORMAT_THUMBNAIL_PARAM_S* *//**< CNcomment:��ȡ����ͼ������Ϊ::HI_FORMAT_THUMBNAIL_PARAM_S* */
    HI_FORMAT_INVOKE_SET_COOKIE =9,                 /**< the command of setting the cookie. The parameter is the value of the buffer addr. Type of the parameter is::HI_CHAR* *//**< CNcomment:����Cookie��Ϣ������Ϊ�洢Cookie��Ϣ��buffer��ַ������Ϊ::HI_CHAR* */
    HI_FORMAT_INVOKE_GET_BANDWIDTH,                 /**< the command of getting the bandwidth, the parameter is ::HI_S64* *//**< CNcomment:���粥�ŵ�ʱ���ȡ��ǰ�����������λΪbps����������::HI_S64* */
    HI_FORMAT_INVOKE_GET_HLS_STREAM_NUM,            /**< the command of getting the numbers of streams in the hls file, the parameter is ::HI_S32* *//**< CNcomment:��ȡhls�ļ����ܹ��м�·��������������::HI_S32* */
    HI_FORMAT_INVOKE_GET_HLS_STREAM_INFO,           /**< the command of getting the information of the selected segment, the parameter is ::HI_FORMAT_HLS_STREAM_INFO_S* *//**< CNcomment:��ȡhls�ļ���ָ����ĳһ·����Ϣ����������::HI_FORMAT_HLS_STREAM_INFO_S* */
    HI_FORMAT_INVOKE_GET_HLS_SEGMENT_INFO,          /**< the command of getting the information of the currently played segment, the parameter is ::HI_FORMAT_HLS_SEGMENT_INFO_S* *//**< CNcomment:��ȡ���ڲ��ŵķ�Ƭ����Ϣ����������::HI_FORMAT_HLS_SEGMENT_INFO_S* */
    HI_FORMAT_INVOKE_GET_PLAYLIST_STREAM_DETAIL_INFO,/**< the command of getting the detail information of playlist file , the parameter is ::HI_FORMAT_PLAYLIST_STREAM_DETAIL_S* *//**< CNcomment:��ȡ�����б���ϸ��Ϣ::HI_FORMAT_PLAYLIST_STREAM_DETAIL_S* */
    HI_FORMAT_INVOKE_SET_HLS_STREAM,                /**< the command of selecting a specified stream, the parameter is ::HI_S32, not support *//**< CNcomment:ָ��hls�ļ���ĳһ·���Ų�������::HI_S32,�ݲ�֧�� */
    HI_FORMAT_INVOKE_SET_HLS_START_MODE,            /**< the command of setting hls switch mode,the parameter is ::HI_FORMAT_HLS_START_MODE_E *//**< CNcomment:hls ����ģʽ����,����ģʽ,����С��Ƭ��ʼ��::HI_FORMAT_HLS_START_MODE_E */
    HI_FORMAT_INVOKE_SET_BUFFER_CONFIG,             /**< the command of setting the configuration information of the buffer, the buffer is used only for network playing.The parameter is ::HI_FORMAT_BUFFER_CONFIG_S* *//**< CNcomment:���û���������Ϣ,�û�����������粥��. ����Ϊ::HI_FORMAT_BUFFER_CONFIG_S* */
    HI_FORMAT_INVOKE_GET_BUFFER_CONFIG,             /**< the command of obtaining the configuration information of the buffer, the buffer is used only for network playing.The parameter is ::HI_FORMAT_BUFFER_CONFIG_S* *//**< CNcomment:����type::HI_FORMAT_BUFFER_CONFIG_TYPE_E��ȡ����������Ϣ, �û�����������粥��. ����Ϊ::HI_FORMAT_BUFFER_CONFIG_S* */
    HI_FORMAT_INVOKE_GET_BUFFER_STATUS,             /**< the command of reading buffer status, the parameter is ::HI_FORMAT_BUFFER_STATUS_S* *//**< CNcomment:Read Buffer status ����Ϊ::HI_FORMAT_BUFFER_STATUS_S* */
    HI_FORMAT_INVOKE_SET_BUFFER_MAX_SIZE = 22,      /**< the command of setting the maximum value of the hiplayer's buffer(decoder's buffer size is not included),the buffer is used only for network playing. The parameter is ::HI_S64*. The maximum value of the buffer is 10 MB by default and cannot be less than 5 MB.*//**< CNcomment:����hiplayer��������ֵ(bytes),������decoder�����С,�û�����������粥��.����Ϊ::HI_S64*,Ĭ��ֵΪ10MB,����С�ڻ������ֵ5MB */
    HI_FORMAT_INVOKE_GET_BUFFER_MAX_SIZE,           /**< the command of obtaining the maximum value of the hiplayer's buffer(decoder's buffer size is not included),the buffer is used only for network playing. The parameter is ::HI_S64*. The maximum value of the buffer is 10 MB by default. *//**< CNcomment:��ȡhiplayer��������ֵ(bytes),������decoder�����С,�û�����������粥��.����Ϊ::HI_S64*,Ĭ��ֵΪ10MB */
    HI_FORMAT_INVOKE_SET_HEADERS = 28,              /**< the command of setting the http headers. The parameter is the value of the buffer addr. Type of the parameter is::HI_CHAR* *//**<CNcomment:����http headers��Ϣ������Ϊ�洢headers��Ϣ��buffer��ַ������Ϊ::HI_CHAR* */
    HI_FORMAT_INVOKE_SET_USERAGENT,                 /**< the command of setting User Agent info,the parameter is ::HI_CHAR * *//**< CNcomment:http �����û������User Agent��Ϣ*/
    HI_FORMAT_INVOKE_SET_REFERER,                   /**< the command of setting http referer info,the parameter is ::HI_CHAR * *//**< CNcomment:http �����û������Referer��Ϣ*/
    HI_FORMAT_INVOKE_SET_NOT_SUPPORT_BYTERANGE,     /**< the command of close byterange support,the parameter is ::HI_S32.The parameter is 1: close http byterange option, 0: open http byterange option,use 0 as the default value. * *//**< CNcomment:http �ر�httpЭ���byterange ֧��.����ȡֵ��Χ1:�ر�http byterange����,0:��http byterange����,Ĭ��ֵΪ0*/
    HI_FORMAT_INVOKE_SET_LOG_LEVEL,                 /**< the command of setting log level,the parameter is::HI_FORMAT_LOG_LEVEL_E *//**< CNcomment:������־�ȼ�������::HI_FORMAT_LOG_LEVEL_E */
    HI_FORMAT_INVOKE_SET_BUFFER_UNDERRUN = 35,      /**< the command of setting buffer underrun behavior, the parameter is ::HI_S32*, HI_TRUE:pause playing while buffer under run, HI_FALSE:default, do nothing*/ /**< CNcomment:���粥��ʱ����bufferǷ����Ϊ����������Ϊ::HI_S32*,HI_TRUE��Ƿ��ʱ��ͣ���ţ�HI_FALSE��Ĭ����Ϊ��Ƿ��ʱ�����κζ��� */
    HI_FORMAT_INVOKE_SET_HLS_LIVE_START_NUM,        /**< the command of setting hls live stream first play segment index ,the parameter is HI_S32��0:use default(the 3rd from last), >=1:begin from the segment specified by this number*//**< CNcomment:hls ֱ������ʼ���ŵķ�Ƭindex(1��ʾ��һ��) ::HI_S32 0:Ĭ�ϵ�����3����ʼ���ţ�>=1:��ָ������ſ�ʼ���� */
    HI_FORMAT_INVOKE_GET_VIDEO_RATING = 38,         /**< the command of getting video rating information,the parameter is::HI_CHAR**, need free by user  *//**< CNcomment:��ȡ��Ƶrating��Ϣ������::HI_CHAR**, ��Ҫ�û��ͷ� */
    HI_FORMAT_INVOKE_GET_AUDIO_RATING,              /**< the command of getting audio rating information,the parameter is::HI_CHAR**, need free by user *//**< CNcomment:��ȡ��Ƶrating��Ϣ������::HI_CHAR**����Ҫ�û��ͷ� */
    HI_FORMAT_INVOKE_GET_SUBTITLE_RATING,           /**< the command of getting subtitle rating information,the parameter is::HI_CHAR**, need free by user *//**< CNcomment:��ȡ��Ļrating��Ϣ������::HI_CHAR**�� ��Ҫ�û��ͷ� */
    HI_FORMAT_INVOKE_SET_BAND_COLLECT_FREQ_MS,      /**< the command of set band collect frequence.ms,the parameter is HI_S32 */ /**< CNcomment:��������ͳ�ƵĲ���ʱ������λms,��������Ϊ HI_S32 */
    HI_FORMAT_INVOKE_GET_SEEKABLE,                  /**< the command is used to getting if the demux support seek when playing current URL, the parameter is ::HI_BOOL* *//**< CNcomment:���ڻ�ȡ�ڲ��ŵ�ǰURLʱdemux�Ƿ�֧��seek����������::HI_BOOL* */
    HI_FORMAT_INVOKE_GET_MAX_PLAYSPEED,             /**< the command of getting the max play speed of hiplayer, this max speed may be changed during playing , the type of parameter is HI_S32 * *//**< CNcomment:��ȡHiPlayer��󲥷��ٶ�, ����󲥷��ٶ��ڲ��Ź������п��ܱ仯������������HI_S32 * */
    HI_FORMAT_INVOKE_SET_DOLBYRANGEINFO,            /**< the command of setting dolby rangeinfo *//**< CNcomment:���öű�rangeinfo */
    HI_FORMAT_INVOKE_GET_DOLBYINFO,                 /**< the command of getting dolby format info *//**< CNcomment:��ȡ�űȸ�ʽ��Ϣ */
    HI_FORMAT_INVOKE_SET_DOLBYDRCMODE,              /**< the command of setting dolby drcmode *//**< CNcomment:���öű�drcmode */
    HI_FORMAT_INVOKE_SET_EXTERNAL_AUDIOTRACK = 49,  /**< the command of add external audio track,maybe audio track is changed,so must reset audio tarck */ /**< CNcomment:������Ƶtrack�����ĳЩʱ����Ƶtrack���ܷ����仯�����Ա����������� */
    HI_FORMAT_INVOKE_SET_SEEK_MODE,                 /**< the command of setting seek mode, the parameter is ::HI_FORMAT_SEEK_MODE_E */ /**< CNcomment:����seekģʽ����������Ϊ::HI_FORMAT_SEEK_MODE_E */
    HI_FORMAT_INVOKE_GET_SWITCH_PG_TIME = 55,       /**< the command of getting proc switch-program time info, the parameter is HI_SVR_PLAYER_PROC_SWITCHPG_S */
    HI_FORMAT_INVOKE_SET_TPLAY_MODE,                /**< the command of setting tplay mode, the parameter is ::HI_SVR_PLAYER_TPLAY_MODE_E*/ /**< CNcomment:����TPlayģʽ������::HI_SVR_PLAYER_TPLAY_MODE_E*/
    HI_FORMAT_INVOKE_SET_LOW_DELAY = 60,            /**< the command of setting low delay, the parameter is ::HI_S32 */ /**< CNcomment:����low delayģʽ����������Ϊ::HI_S32 */
    HI_FORMAT_INVOKE_SET_FREE_RUN,                  /**< the command of setting play free run behavior, the parameter is ::HI_S32*, HI_TRUE:: play video only, as fast as possible; HI_FALSE: resume to normal play*//**< CNcomment:����free run����ģʽ����������Ϊ::HI_S32,  HI_TRUE:���������Ƶ֡; HI_FALSE: �ָ���������*/
    HI_FORMAT_INVOKE_SET_HTTP_DOWNLOAD_SIZE_ONCE,   /**< the command of setting download size each http connection in KB, the parameter is ::HI_S32 */ /**< CNcomment:����ÿ��http�������ص����ݴ�С����λKB����������Ϊ::HI_S32 */
    HI_FORMAT_INVOKE_GET_SUBTITLE_FRAME,            /**< the command of get subtitle frame, the parameter is ::HI_FORMAT_FRAME_S*/ /**< CNcomment:��ȡ��Ļ֡���ݣ�����::HI_FORMAT_FRAME_S*/
    HI_FORMAT_INVOKE_FREE_SUBTITLE_FRAME,           /**< the command of free subtitle frame, the parameter is ::HI_FORMAT_FRAME_S*/ /**< CNcomment:�ͷ���Ļ֡���ݣ�����::HI_FORMAT_FRAME_S*/
    HI_FORMAT_INVOKE_SET_HTTP_SERVICE,              /**< the command of setting http service handle, the parameter is ::IMediaHTTPService*/ /**< CNcomment:����httpservice,����Android5.x�汾������::IMediaHTTPService*/
    HI_FORMAT_INVOKE_SET_CACHE_TIME,                /**< the command of setting cache buffer time,the parameter is::HI_S32 *//**< CNcomment:���û�������ʱ�䣬����::HI_S32 */
    HI_FORMAT_INVOKE_GET_SS_MEDIA_INFO,             /**< the command of getting the media information of the smoothstreaming file ::HI_SVR_SMOOTHSTREAMING_MEDIA_INFO_S *//**< CNcomment:��ȡsmoothstreaming�ļ���Ϣ����������::HI_SVR_SMOOTHSTREAMING_MEDIA_INFO_S* */
    HI_FORMAT_INVOKE_SET_THUMBNAIL_RESTRICT,        /**< set the highest resolution for scanning media restrictions, the parameter is ::HI_FORMAT_THUMBNAIL_RESTRICT  *//**< CNcomment:��������ý��ɨ�����߷ֱ���, ����:HI_FORMAT_THUMBNAIL_RESTRICT*/
    HI_FORMAT_INVOKE_SET_SURFACE_TYPE = 70,         /**< the command of setting surface type, the parameter is ::HI_S32 */ /**< CNcomment:����surface ���ͣ���������Ϊ::HI_S32 */
    HI_FORMAT_INVOKE_SET_HDR_MODE,                  /**< the command of setting HDR mode,1: HDR mode; 0: SDR mode, the parameter is ::HI_S32 */ /**< CNcomment:����HDR��ʾģʽ��1: HDRģʽ; 0: SDR ģʽ����������Ϊ::HI_S32 */
    HI_FORMAT_INVOKE_GET_CHAPTERS,                  /**< the command of getting chapter information, the paramter is::HI_FORMAT_CHAPTER_INFO_S *//**<CNcomment:��ȡ�½���Ϣ,��������::HI_FORMAT_CHAPTER_INFO_S*/
    /* if hisilicon invoke command, please defined before USER */
    HI_FORMAT_INVOKE_PROTOCOL_USER=100,             /**< the command of setting data to user protocol *//**< CNcomment:�����û�Э������ */
} HI_FORMAT_INVOKE_ID_E;

/** The format of the thumbnail */
/** CNcomment:����ͼ��ͼ���ʽ */
typedef enum hiFORMAT_THUMB_PIXEL_FORMAT_E
{
    HI_FORMAT_PIX_FMT_YUV420P = 0x0,       /**< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples) */
    HI_FORMAT_PIX_FMT_YUYV422,             /**< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr */
    HI_FORMAT_PIX_FMT_RGB24,               /**< packed RGB 8:8:8, 24bpp, RGBRGB... */
    HI_FORMAT_PIX_FMT_BGR24,               /**< packed RGB 8:8:8, 24bpp, BGRBGR... */
    HI_FORMAT_PIX_FMT_YUV422P,             /**< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples) */
    HI_FORMAT_PIX_FMT_YUV444P,             /**< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples) */
    HI_FORMAT_PIX_FMT_YUV410P,             /**< planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples) */
    HI_FORMAT_PIX_FMT_YUV411P,             /**< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples) */

    HI_FORMAT_PIX_FMT_RGB565BE = 0x2B,     /**< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), big-endian */
    HI_FORMAT_PIX_FMT_RGB565LE,            /**< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), little-endian */
    HI_FORMAT_PIX_FMT_RGB555BE,            /**< packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), big-endian, most significant bit to 0 */
    HI_FORMAT_PIX_FMT_RGB555LE,            /**< packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), little-endian, most significant bit to 0 */
} HI_FORMAT_THUMB_PIXEL_FORMAT_E;

/*===============================Buffer NetWork================================================*/

/** Type of the buffer configuration */
/** CNcomment:�������õ����� */
typedef enum hiFORMAT_BUFFER_CONFIG_TYPE_E
{
    HI_FORMAT_BUFFER_CONFIG_NONE = 0x0, /**< No Config */
    HI_FORMAT_BUFFER_CONFIG_TIME,       /**< Configure Buffer at Time */
    HI_FORMAT_BUFFER_CONFIG_SIZE,       /**< Configure Buffer at size */
    HI_FORMAT_BUFFER_CONFIG_BULT
} HI_FORMAT_BUFFER_CONFIG_TYPE_E;

/** Configuration information of the hiplayer's buffer. You are advised to consider the current buffer size when configuring the buffer.
If the buffer size is too great, the system memory may be used up and cause oom-killer problem. The total buffer size in bytes(s64Total) must
be less than the value setted by HI_FORMAT_INVOKE_SET_BUFFER_MAX_SIZE,when the config type(eType) is
HI_FORMAT_BUFFER_CONFIG_TIME,this is limited automatically in the hiplayer.*/
/** CNcomment:����������Ϣ:
    ����������Ϣ�����鿼�ǵ�ǰ�ڴ�ռ䣬�����ڴ����ϵͳ�ڴ�ľ���
    �����ϵͳoom-killer�����⡣�����ܴ�С(s64Total)����С��ͨ��HI_FORMAT_INVOKE_SET_BUFFER_MAX_SIZE
    �����õĻ����ֵ��С�������ɲ���ʱ������(HI_FORMAT_BUFFER_CONFIG_TIME)���û���ʱ,hiplayer�ڲ���
    �Զ����ƻ����С���ֽ���������ͨ��HI_FORMAT_INVOKE_SET_BUFFER_MAX_SIZE�����õ�ֵ��
    eType:          HI_FORMAT_BUFFER_CONFIG_SIZE
    s64TimeOut:     2*60*1000   ms
    s64ToTal:       5*1024*1024 bytes
    s64EventStart:  512*1024    bytes
    s64EventEnough: 4*1024*1024 bytes
 */
typedef struct hiFORMAT_BUFFER_CONFIG_S
{
    HI_FORMAT_BUFFER_CONFIG_TYPE_E eType; /**< Config Type ::HI_FORMAT_BUFFER_CONFIG_TYPE_E */
    HI_S64 s64TimeOut;                    /**< network link time out(ms),default:120000ms (2 minute) */
    HI_S64 s64Total;                      /**< length of the hiplayer's Buffer size(bytes) or time(ms).*/
    HI_S64 s64EventStart;                 /**< The First Event start at size(bytes) or time(ms), it is less than s64EventEnough */
    HI_S64 s64EventEnough;                /**< The Enough Event at size(bytes) or time(ms), it is less than s64Total */
} HI_FORMAT_BUFFER_CONFIG_S;

/** Buffer status query */
/** CNcomment:����״̬��ѯ */
typedef struct hiFORMAT_BUFFER_STATUS_S
{
    HI_S64 s64BufferSize;    /**< The data size(bytes) of the buffer, data in hiplayer's buffer and data in decoder's buffer are all included. */
    HI_S64 s64Duration;      /**<The relative time that the data can be played to, data in hiplayer's buffer and data in decoder's buffer are all included. */
} HI_FORMAT_BUFFER_STATUS_S;

/** Type of network errors */
/** CNcomment:������������ */
typedef enum hiFORMAT_MSG_NETWORK_E
{
    HI_FORMAT_MSG_NETWORK_ERROR_UNKNOW = 0,     /**< unknown error *//**< CNcomment:δ֪���� */
    HI_FORMAT_MSG_NETWORK_ERROR_CONNECT_FAILED, /**< connection error *//**< CNcomment:����ʧ�� */
    HI_FORMAT_MSG_NETWORK_ERROR_TIMEOUT,        /**< operation timeout *//**< CNcomment:��ȡ��ʱ */
    HI_FORMAT_MSG_NETWORK_ERROR_DISCONNECT,     /**< net work disconnect *//**< CNcomment:���� */
    HI_FORMAT_MSG_NETWORK_ERROR_NOT_FOUND,      /**< file not found *//**< CNcomment:��Դ������ */
    HI_FORMAT_MSG_NETWORK_NORMAL,               /**< status of network is normal *//**< CNcomment:����״̬���� */
    HI_FORMAT_MSG_NETWORK_ERROR_BUTT,
} HI_FORMAT_MSG_NETWORK_E;

/** Network status*/
/** CNcomment:����״̬��Ϣ */
typedef struct hiFORMAT_NET_STATUS_S
{
    HI_CHAR szProtocals[HI_FORMAT_PROTOCAL_NAME_LEN];/**< network protocol, for example http *//**< CNcomment:����Э�飬��http */
    HI_S32  s32ErrorCode;                           /**< error code of the network protocol, for example 404 *//**< CNcomment:����Э������룬��404 */
    HI_FORMAT_MSG_NETWORK_E eType;                  /**< type of network errors *//**< CNcomment:����������::HI_FORMAT_MSG_NETWORK_ERROR_E */
} HI_FORMAT_NET_STATUS_S;

/*===============================Buffer NetWork End=============================================*/

/** Audio stream information */
/** CNcomment:��Ƶ����Ϣ */
typedef struct hiFORMAT_AUD_INFO_S
{
    HI_S32 s32StreamIndex;                   /**< Stream index. The invalid value is ::HI_FORMAT_INVALID_STREAM_ID. *//**< CNcomment:���������Ƿ�ֵΪ::HI_FORMAT_INVALID_STREAM_ID */
    HI_U32 u32Format;                        /**< Audio encoding format. For details about the value definition, see ::HI_FORMAT_AUDIO_TYPE_E. *//**< CNcomment:��Ƶ�����ʽ��ֵ����ο�::HI_FORMAT_AUDIO_TYPE_E */
    HI_U32 u32Profile;                       /**< Audio encoding version, such as 0x160(WMAV1) and 0x161 (WMAV2). It is valid only for WMA encoding. *//**< CNcomment:��Ƶ����汾������wma���룬0x160(WMAV1), 0x161 (WMAV2) */
    HI_U32 u32SampleRate;                    /**< 8000,11025,441000,... */
    HI_U16 u16BitPerSample;                  /**< Number of bits occupied by each audio sampling point such as 8 bits or 16 bits. *//**< CNcomment:��Ƶÿ����������ռ�ı�������8bit,16bit */
    HI_U16 u16Channels;                      /**< Number of channels, 1 or 2. *//**< CNcomment:������, 1 or 2 */
    HI_S32 s32SubStreamID;                   /**< Sub audio stream ID *//**< CNcomment:����Ƶ��ID,���ΪTrueHD��Ƶ������ܺ���һ·ac3���� ����ǰ��������Ϊ�����������뿪�������ô��ֶ�����¼��ϵ */
    HI_U32 u32BlockAlign;                    /**< Number of bytes contained in a packet *//**< CNcomment:packet�������ֽ��� */
    HI_U32 u32Bitrate;                       /**< Audio bit rate, in the unit of bit/s. *//**< CNcomment:��Ƶ���ʣ�bit/s */
    HI_BOOL bBigEndian;                      /**< Big endian or little endian. It is valid only for the PCM format *//**< CNcomment:��С�ˣ���pcm��ʽ��Ч */
    HI_CHAR aszLanguage[HI_FORMAT_LANG_LEN]; /**< Audio stream language *//**< CNcomment:��Ƶ������ */
    HI_U32 u32ExtradataSize;                 /**< Length of the extended data *//**< CNcomment:��չ���ݳ��� */
    HI_U8  *pu8Extradata;                    /**< Extended data *//**< CNcomment:��չ���� */
    HI_VOID  *pCodecContext;                 /**< Audio decode context *//**< CNcomment:��Ƶ���������� */
    HI_U32 u32Role;                          /**< Role descriptor value of mpeg dash. the most 8 bits is scheme value(refer to HI_FORMAT_ROLE_SCHEME_E), the left 24 bits is descriptor value, the descriptor value may be the bitwise '|' result of value define in HI_FORMAT_ROLE_VALUE_E*/
                                             /**< CNcomment:mpeg dash �� Role descriptor ��ֵ,��8λ����scheme(��ο�HI_FORMAT_ROLE_SCHEME_E),��24λ����descriptor ֵ,descriptorֵ��HI_FORMAT_ROLE_VALUE_E�ж��������1������ֵ��λ����'|' �Ľ��*/
    HI_U32 u32Accessibility;                 /**<  Accessbilitydescriptor value of mpeg dash. the most 8 bits is scheme value(refer to HI_FORMAT_ROLE_SCHEME_E), the left 24 bits is descriptor value, the descriptor value type is number*/
                                             /**< CNcomment:mpeg dash �� Accessibility descriptor ��ֵ,��8λ����scheme(��ο�hiFORMAT_ACCESSIBILITY_SCHEME_E),��24λ����descriptor ֵ,descriptorֵ��������������*/
    HI_S64 s64Duration;                      /**< Duration of audio stream, in the unit of ms. *//**< CNcomment:��Ƶ��ʱ������λms */
} HI_FORMAT_AUD_INFO_S;

/** Dolby format external information */
/** CNcomment:�űȸ�ʽ����ϸ��Ϣ */
typedef struct hiFORMAT_AUD_DOLBY_INFO_S
{
    HI_U32 u32DecoderType;      /**< Audio Decoder type *//**< CNcomment:������ID */
    HI_S32 s32StreamType;       /**< dolby type 0:dolby 1:dolby plus *//**< CNcomment:�űȸ�ʽ���Ͳμ�DOLBYPLUS_STREAM_INFO_S */
    HI_S32 s32ACMode;           /**< dolby ac mode *//**< CNcomment:�μ�DOLBYPLUS_STREAM_INFO_S */
} HI_FORMAT_AUD_DOLBY_INFO_S;

/** Type of the hdr stream */
/** CNcomment:hdr��Ƶ������ */
typedef enum hiFORMAT_HDR_VID_TYPE_E
{
    HI_FORMAT_HDR_VID_NONE = 0x0,  /**< none *//**< CNcomment:��HDR��Ƶ�� */
    HI_FORMAT_HDR_VID_SINGLE_SL,   /**< single *//**< CNcomment:���㵥��HDR��Ƶ�� */
    HI_FORMAT_HDR_VID_SINGLE_DL,   /**< single *//**< CNcomment:˫�㵥��HDR��Ƶ�� */
    HI_FORMAT_HDR_VID_DUAL_BL,     /**< dual bl *//**< CNcomment:˫��˫��HDR��Ƶ����base�� */
    HI_FORMAT_HDR_VID_DUAL_ELRPU,  /**< dual el rpu *//**< CNcomment: ˫��˫��HDR��Ƶ����enhancement��*/
    HI_FORMAT_HDR_VID_BUTT,
} HI_FORMAT_HDR_VID_TYPE_E;

/** Info of the hdr stream */
/** CNcomment:��Ƶ��HDR��Ϣ */
typedef struct hiFORMAT_HDR_VID_INFO_S
{
    HI_FORMAT_HDR_VID_TYPE_E eHdrVidType;  /**< tpye of hdr video stream. *//**< CNcomment:hdr��Ƶ������ */
    HI_S32 s32VdepTrackId;                 /**< trackid of hdr dual track video stream dependency. *//**< CNcomment:hdr˫����Ƶ����������Ƶ��ID */
    HI_U32 u32ElFormat;                    /**< Video encoding format. For details about the value definition, see ::HI_FORMAT_VIDEO_TYPE_E. *//**< CNcomment:��Ƶ�����ʽ��ֵ����ο�::HI_FORMAT_VIDEO_TYPE_E */
    HI_U32 u32ElWidth;                     /**< Width, in the unit of pixel. *//**< CNcomment:���ȣ���λ���� */
    HI_U32 u32ElHeight;                    /**< Height, in the unit of pixel. *//**< CNcomment:�߶ȣ���λ���� */
    HI_S32 s32Compatible;                  /**< backward compatibility,0:non-backward compatible; 1:SDR backward compatible; 2: Ultra HD Blu-rayTM HDR compatible*//**< CNcomment:�������� */
} HI_FORMAT_HDR_VID_INFO_S;

/** Video stream information */
/** CNcomment:��Ƶ����Ϣ */
typedef struct hiFORMAT_VID_INFO_S
{
    HI_S32 s32StreamIndex;                  /**< Stream index. The invalid value is ::HI_FORMAT_INVALID_STREAM_ID. *//**< CNcomment:���������Ƿ�ֵΪ::HI_FORMAT_INVALID_STREAM_ID */
    HI_U32 u32Format;                       /**< Video encoding format. For details about the value definition, see ::HI_FORMAT_VIDEO_TYPE_E. *//**< CNcomment:��Ƶ�����ʽ��ֵ����ο�::HI_FORMAT_VIDEO_TYPE_E */
    HI_U16 u16RefFrameNum;                  /**< Number of reference frames. *//**< CNcomment:�ο�֡��Ŀ */
    HI_U16 u16Profile;                      /**< Profile level. *//**< CNcomment:Profile���� */
    HI_U16 u16Width;                        /**< Width, in the unit of pixel. *//**< CNcomment:���ȣ���λ���� */
    HI_U16 u16Height;                       /**< Height, in the unit of pixel. *//**< CNcomment:�߶ȣ���λ���� */
    HI_U16 u16FpsInteger;                   /**< Integer part of the frame rate *//**< CNcomment:֡�ʣ��������� */
    HI_U16 u16FpsDecimal;                   /**< Decimal part of the frame rate *//**< CNcomment:֡�ʣ�С������ */
    HI_U32 u32Bitrate;                      /**< Video bit rate, in the unit of bit/s. *//**< CNcomment:��Ƶ���ʣ�bit/s */
    HI_U32 u32CodecVersion;                 /**< Version of codec. *//**< CNcomment:Codec �汾 */
    HI_U32 u32Rotate;                       /**< Video rotation angle, value is 90/180/270, default value is 0 *//**< CNcomment:��Ƶ��ת�Ƕ�,90/180/270,Ĭ��ֵ0 */
    HI_U32 u32Reversed;
    HI_BOOL bEnableTVP;
    HI_U32 u32ExtradataSize;                /**< Length of the extended data *//**< CNcomment:��չ���ݳ��� */
    HI_U8  *pu8Extradata;                   /**< Extended data *//**< CNcomment:��չ���� */
    HI_VOID  *pCodecContext;                /**< video decode context *//**< CNcomment:��Ƶ���������� */
    HI_U32 u32Role;                         /**< Role descriptor value of mpeg dash. the most 8 bits is scheme value(refer to HI_FORMAT_ROLE_SCHEME_E), the left 24 bits is descriptor value, the descriptor value may be the bitwise '|' result of value define in HI_FORMAT_ROLE_VALUE_E*/
                                            /**< CNcomment:mpeg dash �� Role descriptor ��ֵ,��8λ����scheme(��ο�HI_FORMAT_ROLE_SCHEME_E),��24λ����descriptor ֵ,descriptorֵ��HI_FORMAT_ROLE_VALUE_E�ж��������1������ֵ��λ����'|' �Ľ��*/
    HI_U32 u32Accessibility;                /**<  Accessbilitydescriptor value of mpeg dash. the most 8 bits is scheme value(refer to HI_FORMAT_ROLE_SCHEME_E), the left 24 bits is descriptor value, the descriptor value type is number*/
                                            /**< CNcomment:mpeg dash �� Accessibility descriptor ��ֵ,��8λ����scheme(��ο�hiFORMAT_ACCESSIBILITY_SCHEME_E),��24λ����descriptor ֵ,descriptorֵ��������������*/
    HI_S64 s64Duration;                     /**< Duration of video stream, in the unit of ms. *//**< CNcomment:��Ƶ��ʱ������λms */
    HI_FORMAT_HDR_VID_INFO_S stHdrVidInfo;  /**< info of hdr video stream. *//**< CNcomment:hdr��Ƶ����Ϣ */
} HI_FORMAT_VID_INFO_S;

/** Subtitle information */
/** CNcomment:��Ļ��Ϣ */
typedef struct hiFORMAT_SUB_INFO_S
{
    HI_S32  s32StreamIndex;                            /**< Stream index. The invalid value is ::HI_FORMAT_INVALID_STREAM_ID. *//**< CNcomment:���������Ƿ�ֵΪ::HI_FORMAT_INVALID_STREAM_ID */
    HI_U32  u32Format;                                 /**< Subtitle format, For details about the value definition, see::HI_FORMAT_SUBTITLE_TYPE_E *//**< CNcomment:��Ļ��ʽ��ȡֵ::HI_FORMAT_SUBTITLE_TYPE_E */
    HI_U32  u32CharSet;                                /**< Encoding type of the subtitle, the value range is as follows:
                                                            1. The default value is 0.
                                                            2. The value of the u32CharSet is the identified byte encoding value if the IdentStream byte encoding function (for details about the definition, see hi_charset_common.h) is set.
                                                            3. If the ConvStream function (for details about the definition, see hi_charset_common.h) is set and the invoke interface is called to set the encoding type to be converted by implementing HI_FORMAT_INVOKE_SET_SOURCE_CODETYPE, the value of the u32CharSet is the configured encoding type */
                                                       /**< CNcomment:��Ļ�������ͣ�ȡֵ��Χ����:
                                                            1. Ĭ��ֵΪ0
                                                            2. ���������IdentStream�ַ�����ʶ����
                                                               (�ο�hi_charset_common.h)���������ֵΪʶ������ַ�����ֵ
                                                            3. ���������ConvStreamת�뺯�� (�ο�hi_charset_common.h)��
                                                              ���ҵ���Invoke�ӿ�ִ��HI_FORMAT_INVOKE_SUB_SET_CODETYPE��
                                                              ��Ҫת���ɵı������ͣ���ֵΪ���õı�������*/
    HI_BOOL bExtSub;                                  /**< Whether subtitles are external subtitles. When bExtSub is HI_TRUE, the subtitles are external. When bExtSub is HI_FALSE, the subtitles are internal. *//**< CNcomment:�Ƿ�Ϊ�����Ļ, HI_TRUEΪ�����Ļ��HI_FALSEΪ������Ļ */
    HI_U32  u32StreamNum;                             /**< contains stream number *//**< CNcomment:��������Ļ����Ŀ */
    HI_CHAR paszLanguage[HI_FORMAT_MAX_LANGUAGE_NUM][HI_FORMAT_LANG_LEN]; /**< Subtitle language *//**< CNcomment:��Ļ���� */
    HI_U16  u16OriginalFrameWidth;                     /**< Width of the original image *//**< CNcomment:ԭʼͼ����� */
    HI_U16  u16OriginalFrameHeight;                    /**< Height of the original image *//**< CNcomment:ԭʼͼ��߶� */
    HI_U32  u32ExtradataSize;                          /**< Length of the extended data *//**< CNcomment:��չ���ݳ��� */
    HI_U8   *pu8Extradata;                             /**< Extended data *//**< CNcomment:��չ���� */
    HI_VOID  *pCodecContext;                           /**< Audio decode context *//**< CNcomment:��Ļ���������� */
    HI_U32 u32Role;                                    /**< Role descriptor value of mpeg dash. the most 8 bits is scheme value(refer to HI_FORMAT_ROLE_SCHEME_E), the left 24 bits is descriptor value, the descriptor value may be the bitwise '|' result of value define in HI_FORMAT_ROLE_VALUE_E*/
                                                       /**< CNcomment:mpeg dash �� Role descriptor ��ֵ,��8λ����scheme(��ο�HI_FORMAT_ROLE_SCHEME_E),��24λ����descriptor ֵ,descriptorֵ��HI_FORMAT_ROLE_VALUE_E�ж��������1������ֵ��λ����'|' �Ľ��*/
    HI_U32 u32Accessibility;                           /**<  Accessibility descriptor value of mpeg dash. the most 8 bits is scheme value(refer to HI_FORMAT_ROLE_SCHEME_E), the left 24 bits is descriptor value, value type is number*/
                                                       /**< CNcomment:mpeg dash �� Accessibility descriptor ��ֵ,��8λ����scheme(��ο�hiFORMAT_ACCESSIBILITY_SCHEME_E),��24λ����descriptor ֵ,descriptorֵ��������������*/
} HI_FORMAT_SUB_INFO_S;


/** Program information. If a file contains internal subtitles and external subtitles, add the external subtitles to the end of the internal subtitles.*/
/** CNcomment:��Ŀ��Ϣ������ļ��ȴ�������ĻҲ��������Ļ����������Ļ��Ϣ׷����������Ļ��*/
typedef struct hiFORMAT_PROGRAM_INFO_S
{
    HI_U32 u32VidStreamNum;                /**< Number of video streams *//**< CNcomment:��Ƶ������ */
    HI_FORMAT_VID_INFO_S *pastVidStream;   /**< Video stream information *//**< CNcomment:��Ƶ����Ϣ */
    HI_U32 u32AudStreamNum;                /**< Number of audio streams *//**< CNcomment:��Ƶ������ */
    HI_FORMAT_AUD_INFO_S *pastAudStream;   /**< Audio stream information *//**< CNcomment:��Ƶ����Ϣ */
    HI_U32 u32SubStreamNum;                /**< Number of subtitles *//**< CNcomment:��Ļ���� */
    HI_FORMAT_SUB_INFO_S *pastSubStream;   /**< Subtitle information *//**< CNcomment:��Ļ��Ϣ */
    HI_CHAR aszServiceName[HI_FORMAT_SERVICE_DESCRIPT_LEN];       /**< Program service name info *//**< CNcomment:��Ŀ���� */
    HI_CHAR aszServiceProvider[HI_FORMAT_SERVICE_DESCRIPT_LEN];   /**<  Program service provider info *//**< CNcomment:��Ŀ�ṩ�� */
    HI_S64  s64ProgramDuration;
    HI_S64  s64ProgramStartTime;
} HI_FORMAT_PROGRAM_INFO_S;

/** Media file information */
/** CNcomment:ý���ļ���Ϣ */
typedef struct hiFORMAT_FILE_INFO_S
{
    HI_FORMAT_SOURCE_TYPE_E eSourceType;    /**< File source type *//**< CNcomment:�ļ�Դ���� */
    HI_FORMAT_STREAM_TYPE_E eStreamType;    /**< File stream type *//**< CNcomment:�ļ������� */
    HI_S64  s64FileSize;                    /**< File size, in the unit of byte. *//**< CNcomment:�ļ���С����λ�ֽ� */
    HI_S64  s64StartTime;                   /**< Start time of playing a file, in the unit is ms. *//**< CNcomment:�ļ�������ʼʱ�䣬��λms */
    HI_S64  s64Duration;                    /**< Total duration of a file, in the unit of ms. *//**< CNcomment:�ļ���ʱ������λms */
    HI_U32  u32Bitrate;                     /**< File bit rate, in the unit of bit/s. *//**< CNcomment:�ļ����ʣ�bit/s */
    HI_CHAR aszFileFormat[HI_FORMAT_TITLE_MAX_LEN];   /**< File demuxer info .Not used now*//**< CNcomment:�ļ���ʽ�����������ڽ�������Ŀǰû��ʹ�� */
    HI_U32  u32ProgramNum;                  /**< Actual number of programs *//**< CNcomment:ʵ�ʽ�Ŀ���� */
    HI_FORMAT_PROGRAM_INFO_S *pastProgramInfo; /**< Program information *//**< CNcomment:��Ŀ��Ϣ */
    HI_BOOL bIsDivx;                        /**< If the stream is DIVX restricted stream,HI_TRUE yes,HI_FALSE no *//**< CNcomment:�Ƿ�DIVX����������HI_TRUE �ǣ�HI_FALSE ���� */
    HI_BOOL bIsDrmFile;
} HI_FORMAT_FILE_INFO_S;

/** Frame data */
/** CNcomment:֡���� */
typedef struct hiFORMAT_FRAME_S
{
    HI_FORMAT_DATA_TYPE_E eType;          /**< Data type *//**< CNcomment:�������� */
    HI_S32  s32StreamIndex;               /**< Stream index *//**< CNcomment:������ */
    HI_BOOL bKeyFrame;                    /**< Whether the frame is a key frame *//**< CNcomment:�Ƿ�Ϊ�ؼ�֡ */
    HI_U32  u32Len;                       /**< Length of frame data, in the unit of byte. This parameter is an input or output parameter in RAW mode. It indicates the number of bytes to be read if it is an input parameter, and indicates the number of
                                                                  bytes that are actually read if it is an output parameter. */
                                          /**< CNcomment:֡���ݳ��ȣ���λ�ֽڣ�����raw��ʽ���ò���Ϊ����/���������
                                                                 ����ΪҪ��ȡ���ֽ��������Ϊʵ�ʶ�ȡ���ֽ��� */
    HI_U8   *pu8Addr;                     /**< Address of the frame data *//**< CNcomment:֡���ݵ�ַ */
    HI_U8   *pu8FrameHeader;              /**< Frame data header address *//**< CNcomment:֡����ǰ��ͷ���ݵ�ַ���������Ҫ��������ΪNULL */
    HI_U32  u32FrameHeaderLen;            /**< Frame data header data length  *//**< CNcomment:֡����ǰ��ͷ���ݳ��� */
    HI_S64  s64Pts;                       /**< Presentation time stamp (PTS), IN the unit of ms. If there is no PTS, it is set to ::HI_FORMAT_NO_PTS. *//**< CNcomment:ʱ�������λms�����û��ʱ���������Ϊ::HI_FORMAT_NO_PTS */
    HI_S64  s64Pos;                       /**< position of this frame in the media file.*//**< CNcomment:��֡������ý���ļ��е��ֽ�ƫ��λ�� */
    HI_U32  u32Duration;                  /**< Display duration, in the unit of ms. It is set to 0 if there is no duration. *//**< CNcomment:��ʾʱ������λms��û��duration������Ϊ0 */
    HI_U32  u32UserData;                  /**< Private data. The DEMUX can transparently transmit private data by using this parameter. *//**< CNcomment:˽�����ݣ�����������ͨ���ò����ش�˽������ */
    HI_U32  u32Format;                   /**< Video/Audio/Subtitle format, For details about the value definition, see::HI_FORMAT_VIDEO_TYPE_E/HI_FORMAT_AUDIO_TYPE_E/HI_FORMAT_SUBTITLE_TYPE_E *//**< CNcomment:��Ƶ/��Ƶ/��Ļ/��Ļ��ʽ��ȡֵ::HI_FORMAT_VIDEO_TYPE_E/HI_FORMAT_AUDIO_TYPE_E/HI_FORMAT_SUBTITLE_TYPE_E */

    HI_U8   *pu8IVBuffer;                 /**< DRM IV buffer *//**< CNcomment: DRM �������� */
    HI_U32  u32IVSize;                    /**< DRM IV buffer *//**< CNcomment: DRM ����������С */
    HI_U8   *pu8KeyID;                    /**< DRM key ID buffer *//**< CNcomment: DRM UUID���� */
    HI_U32  u32KeyIDSize;                 /**< DRM key ID buffer size*//**< CNcomment: DRM UUID�����С */
    HI_S32  s32Cryptomode;
    HI_S32  s32Encryptsize;
    HI_S32  *ps32Encryptsize;
} HI_FORMAT_FRAME_S;
/** Parameters of the thumbnail */
/** CNcomment:����ͼ���� */
typedef struct hiFORMAT_THUMBNAIL_PARAM_S
{
    HI_FORMAT_THUMB_PIXEL_FORMAT_E thumbPixelFormat;    /**< It is an input parameter. It specifies the picture format of the obtained thumbnail. *//**< CNcomment:�������������ָ����ȡ��������ͼ��ͼ���ʽ */
    HI_S64 thumbnailTimeMs;          /**< It is an input parameter. It specifies the time of the obtained thumbnail. *//**< CNcomment:������������ڻ�ȡָ��ʱ�������ͼ */
    HI_S32 thumbnailSize;                               /**< It is an input parameter. It specifies the larger value between the width (tw) and height (th) of the obtained thumbnail.
                                                             Note: The value of thumbnailSize cannot be 0 in rule 1 and rule 2.
                                                             1. If the source video width (w) is greater than height (h), the width (tw) of the obtained thumbnail is thumbnailSize,
                                                               and the height (th) of the obtained thumbnail is (tw x w)/h.
                                                             2. If the source video height (h) is greater than the source picture width (w), the height (th) of the obtained thumbnail is thumbnailSize,
                                                               and the width (tw) of the obtained thumbnail is (th x h)/w.
                                                             3. If thumbnailSize is 0, the width (tw) of the obtained thumbnail is the source video width (w),
                                                               and the height (th) of the obtained thumbnail is the source video height (h).*/
                                                        /**< CNcomment:�������������ָ����ȡ��������ͼ�Ŀ�(tw)�͸�(th)�еĽϴ�ֵ��
                                                             ˵��(����1��2�е�thumbnailSize��ȡֵ��Ϊ0)��
                                                             1.���ԭʼ��Ƶ�Ŀ�(w)���ڸ�(h)�����ȡ��������ͼ�Ŀ�(tw)ΪthumbnailSize,
                                                               ��ȡ��������ͼ�ĸ�(th)Ϊ(tw*w)/h��
                                                             2.���ԭʼ��Ƶ�ĸ�(h)���ڿ�(w)�����ȡ��������ͼ�ĸ�(th)ΪthumbnailSize,
                                                               ��ȡ��������ͼ�Ŀ�(tw)Ϊ(th*h)/w��
                                                             3.�����thumbnailSize��Ϊ0�����ȡ��������ͼ�Ŀ�(tw)Ϊԭʼ��Ƶ�Ŀ�(w)��
                                                               ��ȡ��������ͼ�ĸ�(th)Ϊԭʼ��Ƶ�ĸ�(h). */

    HI_S32 width;                                       /**< It is an output parameter. The width of the obtained thumbnail is returned.*//**< CNcomment:������������ػ�ȡ��������ͼ�Ŀ��� */
    HI_S32 height;                                      /**< It is an output parameter. The height of the obtained thumbnail is returned.*//**< CNcomment:������������ػ�ȡ��������ͼ�ĸ߶� */
    HI_S32 lineSize[HI_FORMAT_THUMBNAIL_PLANAR];        /**< It is an output parameter. The width (in byte) of the block for storing the thumbnail data is returned.
                                                             Note:
                                                             1. For the current picture formats, a maximum of four blocks (HI_FORMAT_THUMBNAIL_PLANAR) are required for storing the picture data.
                                                               lineSize[0] to lineSize[HI_FORMAT_THUMBNAIL_PLANAR-1] indicate the
                                                               block widths (in byte) of block 0 to block (HI_FORMAT_THUMBNAIL_PLANAR-1) respectively.
                                                             2. For the RGB picture, only one block (block 0) is used. In this case, pay attention only to lineSize[0].
                                                             3. For the YUV picture, if the Y, U, and V components are stored in blocks, they are separately stored in a block.
                                                               lineSize[0] to lineSize[2] indicate the width of the blocks that store the Y, U, and V components respectively. */
                                                        /**< CNcomment:����������������ڴ������ͼͼ�����ݵĸ���ƽ��һ����ռ���ֽ�����
                                                             ˵��:
                                                             1.����Ŀǰ���е�ͼ���ʽ�������ҪHI_FORMAT_THUMBNAIL_PLANAR(��4)��ƽ����ͼ�����ݣ�
                                                               lineSize[0]~lineSize[HI_FORMAT_THUMBNAIL_PLANAR-1]�ֱ��ʾ��0~(HI_FORMAT_THUMBNAIL_PLANAR-1)
                                                               ��ƽ��һ����ռ���ֽ�����
                                                             2.����RGB���ͼ���ʽ��ֻʹ����1��ƽ�棬����0��ƽ�棬���ֻ���עlineSize[0]���ɣ�
                                                             3.����YUV���ͼ���ʽ���������ƽ�淽ʽ�洢����Y����������һ��ƽ��洢��U������V����Ҳ����
                                                               ��һ��ƽ��洢����lineSize[0]~lineSize[2]�ֱ��ʾY,U��V����ƽ��һ����ռ���ֽ���. */
    HI_U8  *frameData[HI_FORMAT_THUMBNAIL_PLANAR];      /**< It is an output parameter. The storage address of the thumbnail data in each block is returned.
                                                             Note:
                                                             1. For the current picture formats, a maximum of four blocks (HI_FORMAT_THUMBNAIL_PLANAR) are required for storing the picture data.
                                                               frameData[0] to frameData[HI_FORMAT_THUMBNAIL_PLANAR-1] indicate the storage addresses of the thumbnail data
                                                               in block 0 to block (HI_FORMAT_THUMBNAIL_PLANAR-1) respectively.
                                                             2. For the RGB picture, only one block (block 0) is used. In this case, pay attention only to frameData[0].
                                                             3. For the YUV picture, if the Y, U, and V components are stored in blocks, they are separately stored in a block.
                                                               frameData[0] to frameData[2] indicate the storage addresses of the thumbnail data in the blocks that store the Y, U, and V components respectively.
                                                             4. The storage space for frameData is allocated by the bottom software. After data in the storage space is used up by a program,
                                                               the storage space pointed by frameData must be released by calling the corresponding free interface. If the address of a storage space block is NULL,
                                                               the address is not allocated. You need to check whether the address is NULL before calling the free interface. */
                                                        /**< CNcomment:�����������������ͼͼ�������ڸ���ƽ��Ĵ洢��ַ;
                                                             ˵��:
                                                             1.����Ŀǰ���е�ͼ���ʽ�������ҪHI_FORMAT_THUMBNAIL_PLANAR(��4)��ƽ����ͼ�����ݣ�
                                                               frameData[0]~frameData[HI_FORMAT_THUMBNAIL_PLANAR-1]�ֱ��ʾ����ͼͼ��������
                                                               ��0~(HI_FORMAT_THUMBNAIL_PLANAR-1)��ƽ���ϵĴ洢��ַ��
                                                             2.����RGB���ͼ���ʽ��ֻʹ����1��ƽ�棬����0��ƽ�棬���ֻ���עframeData[0]���ɣ�
                                                             3.����YUV���ͼ���ʽ���������ƽ�淽ʽ�洢����Y����������һ��ƽ��洢��U������V����Ҳ����
                                                               ��һ��ƽ��洢����frameData[0]~frameData[2]�ֱ��ʾ����ͼͼ��������Y,U��V����ƽ���ϵĴ洢��ַ;
                                                             4.frameData�Ĵ洢�ռ����ɵײ����ģ��û�������ʹ����frameData����Ҫʹ��free�ͷ�frameDataָ���
                                                               �洢�ռ䣻frameData��û��ʹ�õĲ���ΪNULL�����freeǰ������ж��Ƿ�ΪNULL. */
} HI_FORMAT_THUMBNAIL_PARAM_S;

/** Protocol (such as file protocol and HTTP) API */
/** CNcomment:Э�飨�磺file��http��API */
typedef struct hiFORMAT_PROTOCOL_FUN_S
{
    /**
    \brief Check whether the entered protocol is supported. CNcomment:���������Э���Ƿ�֧��CNend
    \attention \n
    None
    \param[in] pszUrl file URL, such as http://. CNcomment:�ļ�url����http:// CNend

    \retval ::HI_SUCCESS The protocol is supported. CNcomment:֧�ָ�Э��CNend
    \retval ::HI_FAILURE The protocol is not supported. CNcomment:��֧�ָ�Э��CNend

    \see \n
    None
    */
    HI_S32 (*url_find)(const HI_CHAR *pszUrl);

    /**
    \brief Open a file. CNcomment:���ļ�CNend
    \attention \n
    None
    \param [in]  pszUrl URL address. CNcomment:url��ַCNend
    \param [in]  eFlag open mode. CNcomment:�򿪷�ʽCNend
    \param [out] pUrlHandle protocol handle. CNcomment:Э����CNend

    \retval ::HI_SUCCESS The file is successfully opened and the handle is valid. CNcomment:�ļ��򿪳ɹ��������ЧCNend
    \retval ::HI_FAILURE The file fails to be opened. CNcomment:�ļ���ʧ��CNend

    \see \n
    None
    */
    HI_S32 (*url_open)(const HI_CHAR *pszUrl, HI_FORMAT_URL_OPENFLAG_E eFlag, HI_VOID **pUrlHandle);

    /**
    \brief Read data of the specified size. CNcomment:��ȡָ����С����CNend
    \attention \n
    None
    \param [in] urlHandle file handle. CNcomment:�ļ����CNend
    \param [in] s32Size size of the read data, in the unit of byte. CNcomment:��ȡ�����ݴ�С���ֽ�Ϊ��λCNend
    \param [out] pu8Buf data storage buffer. CNcomment:���ݴ洢����CNend

    \retval ::Number greater than 0: The returned value indicates the number of read bytes, if less than s32Size, indicate the file ends.
                CNcomment:>0 ���ص�ֵ�Ƕ�ȡ���ֽ��������ʵ�ʶ�ȡ���ֽ���С��s32Size�������ļ�����CNend
    \retval ::HI_FAILURE The file ends. CNcomment:�ļ�����CNend

    \see \n
    None
    */
    HI_S32 (*url_read)(HI_VOID *urlHandle, HI_U8 *pu8Buf, HI_S32 s32Size);

    /**
    \brief Seek to a specified location. HI_FAILURE is returned if the file does not support seek operations. CNcomment:seek��ָ��λ�ã����ڲ�֧��seek���ļ�������HI_FAILURE��CNend
    \attention \n
    None
    \param [in] urlHandle file handle. CNcomment:�ļ����CNend
    \param [in] s64Offsets number of sought bytes. CNcomment:seek�ֽ���CNend
    \param [in] s32Whence seeking flag. SEEK_CUR indicates seeking after s64Offsets is deviated from the current location,
                SEEK_SET indicates seeking from the start location, SEEK_END indicates seeking from the end location and HI_FORMAT_SEEK_FILESIZE return the file size.
                CNcomment:seek��ʾ��SEEK_CUR�ӵ�ǰλ��ƫ��s64Offsets,SEEK_SET����ʼλ��ƫ��s64Offsets, SEEK_END���ļ�βƫ��s64Offsets,
                HI_FORMAT_SEEK_FILESIZE�����ļ���С��CNend
    \retval ::Value other than HI_FAILURE: It indicates the number of start offset bytes in the file . CNcomment:��HI_FAILURE �ļ���ʼƫ���ֽ���CNend
    \retval ::HI_FAILURE The seeking fails. CNcomment:seekʧ��CNend

    \see \n
    None
    */
    HI_S64 (*url_seek)(HI_VOID *urlHandle, HI_S64 s64Offsets, HI_S32 s32Whence);

    /**
    \brief Destroy the file handle. CNcomment:�����ļ����CNend
    \attention \n
    None
    \param [in] urlHandle file handle. CNcomment:�ļ����CNend

    \retval ::HI_SUCCESS The file handle is destroyed. CNcomment:�ļ�������ٳɹ�CNend
    \retval ::HI_FAILURE The parameter is invalid. CNcomment:�����Ƿ�CNend

    \see \n
    None
    */
    HI_S32 (*url_close)(HI_VOID *urlHandle);

    /**
    \brief Seek a specified stream.  CNcomment:seekָ������CNend
    \attention \n
    None
    \param [in] urlHandle file handle. CNcomment:�ļ����CNend
    \param [in] s32StreamIndex stream index. CNcomment:������CNend
    \param [in] s64Timestamp PTS, in the unit of ms. CNcomment:ʱ�������λms CNend
    \param [in] eFlags seeking flag. CNcomment:seek���CNend

    \retval ::HI_SUCCESS The seeking is successful. CNcomment:seek�ɹ�CNend
    \retval ::HI_FAILURE The seeking fails. CNcomment:seekʧ��CNend

    \see \n
    None
    */
    HI_S32 (*url_read_seek)(HI_VOID *urlHandle, HI_S32 s32StreamIndex, HI_S64 s64Timestamp, HI_FORMAT_SEEK_FLAG_E eFlags);
    /**
    \brief invoke operation. CNcomment:invoke����CNend
    \attention \n
    None
    \param [in] urlHandle file handle. CNcomment:�ļ����CNend
    \param [in] u32InvokeId The value of the command is the type of HI_FORMAT_INVOKE_ID_E. The type of HI_FORMAT_INVOKE_ID_E only supports HI_FORMAT_INVOKE_PRE_CLOSE_FILE and HI_FORMAT_INVOKE_USER_OPE.
                CNcomment:ֵΪ::HI_FORMAT_INVOKE_ID_E����, ��֧��HI_FORMAT_INVOKE_PRE_CLOSE_FILE/HI_FORMAT_INVOKE_USER_OPE CNend
    \param [in] pArg the parameter corresponding to the invoke operation. CNcomment:invoke�������Ĳ���CNend

    \retval ::HI_SUCCESS The operation is successful. CNcomment:�ɹ�CNend
    \retval ::HI_FAILURE The operation fails. CNcomment:ʧ��CNend

    \see \n
    None
    */
    HI_S32 (*url_invoke)(HI_VOID *urlHandle, HI_U32 u32InvokeId, HI_VOID *pArg);
} HI_FORMAT_PROTOCOL_FUN_S;

/** File DEMUX application programming interface (API) */
/** CNcomment:�ļ�������API */
typedef struct hiFORMAT_DEMUXER_FUN_S
{
    /**
    \brief Check whether the entered file format is supported. The value 0 indicates that the file format is supported and other values indicate that the file format is not supported.
           CNcomment:����������ļ���ʽ�Ƿ�֧�֣�����0֧�֣�������֧��CNend
    \attention \n
    None
    \param [in] pszFileName file name, that is, path+file name. CNcomment:�ļ����ƣ�·��+�ļ���CNend
    \param [in] pstProtocol interface of the protocol. The URL can be open and the file data can be read by using this interface. CNcomment:Э������ӿڣ�ͨ���ýӿڿ��Դ�url����ȡ�ļ�����CNend

    \retval ::HI_SUCCESS The file format is supported. CNcomment:֧�ָ��ļ���ʽCNend
    \retval ::HI_FAILURE The file format is not supported. CNcomment:��֧�ָ��ļ���ʽCNend

    \see \n
    None
    */
    HI_S32 (*fmt_find)(const HI_CHAR *pszFileName, const HI_FORMAT_PROTOCOL_FUN_S *pstProtocol);

    /**
    \brief Open a specified file. CNcomment:��ָ���ļ�CNend
    \attention \n
    None
    \param [in] pszFileName file path. CNcomment:�ļ�·��CNend
    \param [in] pstProtocol protocol operation interface. You can specify the protocol operation interface by using this parameter if no protocol operation interface is configured for a registered dynamic link library (DLL).
                CNcomment:Э������ӿڣ�ע��Ķ�̬�����û��Э������ӿڿ���ͨ���÷�ʽָ��CNend
    \param [out] pFmtHandle handle of the output file DEMUX. CNcomment:����ļ����������CNend

    \retval ::HI_SUCCESS The file is opened successfully and the DEMUX handle is valid. CNcomment:�ļ��򿪳ɹ��������������ЧCNend
    \retval ::HI_FAILURE The file fails to be opened. CNcomment:�ļ���ʧ��CNend

    \see \n
    None
    */
    HI_S32 (*fmt_open)(const HI_CHAR *pszFileName, const HI_FORMAT_PROTOCOL_FUN_S *pstProtocol, HI_VOID **pFmtHandle);

    /**
    \brief Look up the streams in the file. CNcomment:�����ļ��е�����ϢCNend
    \attention \n
    the function must return success if do nothing.  its calling process will be:
    1. Open a file by calling the fmt_open function.
    2. Look up the streams information by calling the fmt_find_stream.
    3. The file can be opened successfully only when the functions return HI_SUCCESS.
    CNcomment:�ýӿ������ʵ�֣��������ر���ΪHI_SUCCESS��HiPlayer������������:
    1. �ȵ���fmt_open������һ���ļ����
    2. �ٵ���fmt_find_stream�����ļ�����Ϣ
    3. ֻ��fmt_open,fmt_find_stream����������HI_SUCCESS������Ϊ�ļ��򿪳ɹ�CNend

    \param [in] fmtHandle file handle returned by the functions. CNcomment:fmt_open�������ص��ļ����CNend
    \param [in] pArg expand parameter. CNcomment:��չ����CNend

    \retval ::HI_SUCCESS get the valid stream information successfully. CNcomment:�ļ�������Ч������Ƶ����Ļ����ϢCNend
    \retval ::HI_FAILURE no stream in the file. The file cannot be played. CNcomment:�ļ���û���κ�����Ϣ�����ļ����ɲ���CNend

    \see \n
    None
    */
    HI_S32 (*fmt_find_stream)(HI_VOID *fmtHandle, HI_VOID *pArg);

    /**
    \brief Obtain the file information. CNcomment:��ȡ�ļ���ϢCNend
    \attention \n
    None
    \param [in] fmtHandle file handle. CNcomment:�ļ����CNend
    \param [out] ppstFmtInfo file information. CNcomment:�ļ���ϢCNend

    \retval ::HI_SUCCESS The operation is successful. CNcomment:�����ɹ�CNend
    \retval ::HI_FAILURE The parameters are invalid. CNcomment:�����Ƿ�CNend

    \see \n
    None
    */
    HI_S32 (*fmt_getinfo)(HI_VOID *fmtHandle, HI_FORMAT_FILE_INFO_S **ppstFmtInfo);

    /**
    \brief Read one frame data. The read data must be released by calling the fmt_free interface. CNcomment:��ȡһ֡���ݣ���ȡ���ݱ������fmt_free�ͷ�CNend
    \attention \n
    If the read frame data has no PTS, set the PTS to HI_FORMAT_NO_PTS.  CNcomment:��ȡ��֡�������û��ʱ�������ʱ���ֵ����ΪHI_FORMAT_NO_PTS CNend
    \param [in] fmtHandle file DEMUX handle. CNcomment:�ļ����������CNend
    \param [in/out] pstFmtFrame media data. CNcomment:ý��������ϢCNend

    \retval ::HI_SUCCESS The operation is successful. CNcomment:�����ɹ�CNend
    \retval ::HI_FAILURE The parameters are invalid. CNcomment:�����Ƿ�CNend
    \retval ::HI_FORMAT_ERRO_ENDOFFILE The file ends. CNcomment:�ļ�����CNend

    \see \n
    None
    */
    HI_S32 (*fmt_read)(HI_VOID *fmtHandle, HI_FORMAT_FRAME_S *pstFmtFrame);

    /**
    \brief Release frame data. CNcomment:�ͷ�֡����CNend
    \attention \n
    None
    \param [in] fmtHandle file DEMUX handle. CNcomment:�ļ����������CNend
    \param [in] pstFmtFrame media data read by the fmt_read interface. CNcomment:fmt_read�ӿڶ�ȡ��ý��������ϢCNend

    \retval ::HI_SUCCESS The operation is successful. CNcomment:�����ɹ�CNend
    \retval ::HI_FAILURE The parameters are invalid. CNcomment:�����Ƿ�CNend

    \see \n
    None
    */
    HI_S32 (*fmt_free)(HI_VOID *fmtHandle, HI_FORMAT_FRAME_S *pstFmtFrame);

    /**
    \brief Invoke the interface with the DEMUX and protocol. CNcomment:���������Э�齻�������ӿ�CNend
    \attention \n
    None
    \param [in] fmtHandle handle of the DEMUX. CNcomment:�ļ����������CNend
    \param [in] u32InvokeId command of invoking. The value is HI_FORMAT_INVOKE_ID_E. CNcomment:�������ֵΪHI_FORMAT_INVOKE_ID_E CNend
    \param [in/out] pArg the parameter corresponding to the command. CNcomment:�����Ӧ�Ĳ���CNend

    \retval ::HI_SUCCESS The invoke operation is successful. CNcomment:�����ɹ�CNend
    \retval ::HI_FAILURE The parameter is invalid. CNcomment:�����Ƿ�CNend

    \see \n
    None
    */
    HI_S32 (*fmt_invoke)(HI_VOID *fmtHandle, HI_U32 u32InvokeId, HI_VOID *pArg);

    /**
    \brief Seek frame data in a file based on the entered PTS. Seek frame data in a sequence from the current location to the file header if s64pts is set to 0.
           CNcomment:seek��ָ��pts��s64ptsΪ0��ʾseek���ļ�ͷCNend
    \attention \n
    None
    \param [in] fmtHandle file DEMUX handle. CNcomment:�ļ����������CNend
    \param [in] s32StreamIndex stream index. CNcomment:������CNend
    \param [in] s64Pts PTS, in the unit of ms. CNcomment:ʱ�������λms CNend
    \param [in] eFlag seeking flag. CNcomment:seek���CNend

    \retval ::HI_SUCCESS The operation is successful. CNcomment:�����ɹ�CNend
    \retval ::HI_FAILURE The seeking operation is not supported. CNcomment:��֧��seek����CNend
    \retval ::HI_FORMAT_ERRO_ENDOFFILE end of file. CNcomment:�ļ�����CNend

    \see \n
    None
    */
    HI_S32 (*fmt_seek_pts)(HI_VOID *fmtHandle, HI_S32 s32StreamIndex, HI_S64 s64Pts, HI_FORMAT_SEEK_FLAG_E eFlag);

    /**
    \brief Seek frame data in a file by deviating s64Offsets from the current location if s32Whence is set to SEEK_CUR, by deviating s64Offsets from the file header if
s32Whence is set to SEEK_SET, and by deviating s64Offsets from the end of the file if s32Whence is set to SEEK_END. Jumping can be implemented by using this interface for pure audio or video files.
CNcomment:seek��ָ��λ�ֽ�����λ�ã������������ļ�������ͨ���ýӿ�ʵ����ԾCNend
    \attention \n
    None
    \param [in] fmtHandle file DEMUX handle. CNcomment:�ļ����������CNend
    \param [in] s64Offsets number of sought bytes. CNcomment:seek�ֽ���CNend
    \param [in] s32Whence seek flag, such as SEEK_CUR. CNcomment:seek��ʾ����SEEK_CUR CNend

    \retval ::Value other than HI_FAILURE: It indicates the number of bytes deviated by the file pointer after seeking. CNcomment:��HI_FAILURE seek���ļ�ָ��ƫ���ֽ���CNend
    \retval ::HI_FAILURE The seeking operation is not supported. CNcomment:��֧��seek����CNend
    \retval ::HI_FORMAT_ERRO_ENDOFFILE end of file. CNcomment:�ļ�����CNend

    \see \n
    None
    */
    HI_S64 (*fmt_seek_pos)(HI_VOID *fmtHandle, HI_S64 s64Offsets, HI_S32 s32Whence);

    /**
    \brief Disable the file DEMUX. CNcomment:�ر��ļ�������CNend
    \attention \n
    None
    \param [in] fmtHandle file DEMUX handle. CNcomment:�ļ����������CNend

    \retval ::HI_SUCCESS The operation is successful. CNcomment:�����ɹ�CNend
    \retval ::HI_FAILURE The parameter is invalid. CNcomment:�����Ƿ�CNend

    \see \n
    None
    */
    HI_S32 (*fmt_close)(HI_VOID *fmtHandle);
} HI_FORMAT_DEMUXER_FUN_S;

/** The file parsing component, protocol component parameters, file DEMUX, and protocol components must implement functions defined in the structure.
  * Attributes such as the DEMUX name, supported file format, and version number must be specified.
  * The dlsym system function can be used to
  * Export the global symbol of the structure defined in the file DEMUX.
  * The attribute value of the symbol cannot be changed externally.
  * Example: The following variables are defined in the DEMUX:
  * HI_FORMAT_S g_stFormat_entry = {
  *     .pszDllName           = (const HI_PCHAR )"libavformat.so",
  *     .pszFormatName        = "asf,mp4,avi,smi,srt",
  *     .pszProtocolName      = "http,udp,file",
  *     .pszFmtDesc           = (const HI_PCHAR)"hisilicon avformat",
  *     .u32Merit             = 0;
  *     .stDEMUXFun.fmt_open             = ... ,
  *     .stDEMUXFun.fmt_getinfo          = ... ,
  *     .stDEMUXFun.fmt_read             = ... ,
  *     .stDEMUXFun.fmt_free             = ... ,
  *     .stDEMUXFun.fmt_close            = ... ,
  *     .stDEMUXFun.fmt_seek_pts         = ... ,
  *     .stDEMUXFun.fmt_seek_pos         = ... ,
  *
  *     .stProtocolFun.url_open            = ... ,
  *     .stProtocolFun.url_read            = ... ,
  *     .stProtocolFun.url_seek            = ... ,
  *     .stProtocolFun.url_close           = ... ,
  *     .stProtocolFun.url_read_seek       = ... ,
  * };
  *
  * Application (APP) loading mode:
  *
  * HI_FORMAT_S *pstEntry = NULL;
  * pstEntry = dlsym(pDllModule, "g_stFormat_entry");
  *
  * Calling mode:
  * pstEntry->fmt_open();
  *
  */

/** CNcomment:�ļ����������Э������������ļ���������Э���������ʵ��
  * �ṹ���ж���ĺ�������ָ�����������ƣ�
  * ֧�ֵ��ļ���ʽ���汾�ŵ����ԣ�ͨ��dlsym
  * ϵͳ�����ܵ����ļ��������ж���ĸý�
  * ����ȫ�ַ��ţ��ⲿ�����޸ĸ÷��ŵ�����ֵ��
  * ����ʾ��: �������ж������±�����
  * HI_FORMAT_S g_stFormat_entry = {
  *     .pszDllName           = (const HI_PCHAR )"libavformat.so",
  *     .pszFormatName        = "asf,mp4,avi,smi,srt",
  *     .pszProtocolName      = "http,udp,file",
  *     .pszFmtDesc           = (const HI_PCHAR)"hisilicon avformat",
  *     .u32Merit             = 0;
  *     .stDemuxerFun.fmt_open             = ... ,
  *     .stDemuxerFun.fmt_getinfo          = ... ,
  *     .stDemuxerFun.fmt_read             = ... ,
  *     .stDemuxerFun.fmt_free             = ... ,
  *     .stDemuxerFun.fmt_close            = ... ,
  *     .stDemuxerFun.fmt_seek_pts         = ... ,
  *     .stDemuxerFun.fmt_seek_pos         = ... ,
  *
  *     .stProtocolFun.url_open            = ... ,
  *     .stProtocolFun.url_read            = ... ,
  *     .stProtocolFun.url_seek            = ... ,
  *     .stProtocolFun.url_close           = ... ,
  *     .stProtocolFun.url_read_seek       = ... ,
  * };
  *
  * APP���ط�ʽ����:
  *
  * HI_FORMAT_S *pstEntry = NULL;
  * pstEntry = dlsym(pDllModule, "g_stFormat_entry");
  *
  * ���÷�ʽ:
  * pstEntry->fmt_open();
  *
  */

/** File DEMUX attributes */
/** CNcomment:�ļ����������� */
typedef struct hiFORMAT_S
{
    /**< DEMUX name */
    /**< CNcomment:���������� */
    const HI_CHAR *pszDllName;

    /**< File formats supported by the DEMUX. The file formats are separated by commas (,). */
    /**< CNcomment:������֧�ֵ��ļ���ʽ�������ļ���ʽ��","Ϊ��� */
    const HI_CHAR *pszFormatName;

    /**< Supported protocols. The protocols are separated by commas (,). */
    /**< CNcomment:֧�ֵ�Э�飬����Э����","Ϊ��� */
    const HI_CHAR *pszProtocolName;

    /**< DEMUX version */
    /**< CNcomment:�������汾�� */
    const HI_FORMAT_LIB_VERSION_S stLibVersion;

    /**< DEMUX description */
    /**< CNcomment:���������� */
    const HI_CHAR *pszFmtDesc;

    /**< Priority. The value ranges from 0x0 to 0xFFFFFFFF. 0xFFFFFFFF indicates the highest priority.*/
    /**< CNcomment:���ȼ�����0x0 - 0xFFFFFFFF��0xFFFFFFFF���ȼ���� */
    HI_U32 u32Merit;

   /**< Access interface of the file DEMUX. The implementation of the file DEMUX must comply with the following rules:
          1. For operations, such as opening, reading, and closing a file (fopen or fclose on local files).
             the access interface of the file DEMUX can be specified externally and can be adapted by using the following URL operation interface. */
    /**< CNcomment:�ļ����������ʽӿڣ��ļ�������ʵ�ֱ�����ѭ���¹���:
         1�������ļ��򿪡���ȡ���رյȲ���������ڱ����ļ���fopen/fclose�ȣ�
            �������ⲿָ�����ҿ������·���url�����ӿ����� */

    HI_FORMAT_DEMUXER_FUN_S stDemuxerFun;

    /**< Protocol access interface. The interface complies with the following rules:
         1. This parameter must be set to NULL if the file DEMUX does not implement the protocol access interface.
         2. This parameter can be specified externally. That is, the file DEMUX does not need to implement this interface.
            An interface specified externally is used for file operations during parsing. */
    /**< CNcomment:Э����ʽӿڣ��ýӿ���ѭ���¹���:
         1������ļ�������û��ʵ�ָýӿڣ����뽫�ò�������ΪNULL;
         2���ò����������ⲿָ������: �ļ����������Բ�ʵ�ָýӿڣ�
            �ڽ������̶��ļ��Ĳ���ʹ���ⲿָ���ӿ�; */

    HI_FORMAT_PROTOCOL_FUN_S stProtocolFun;

    struct hiFORMAT_S *next;

    /**< DLL handle of the DEMUX */
    /**< CNcomment:��������̬���� */
    HI_VOID  *pDllModule;

    /**< Private data */
    /**< CNcomment:˽������ */
    HI_U32 u32PrivateData;
} HI_FORMAT_S;
/** @} */  /** <!-- ==== Structure Definition end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_SVR_FORMAT_H__ */