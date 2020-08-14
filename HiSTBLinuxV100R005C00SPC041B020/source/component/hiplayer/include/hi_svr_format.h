/**
 \file
 \brief File demutiplexer (DEMUX), protocol structure. CNcomment:文件解析器，协议结构体CNend
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

/** Reach the end of a file. CNcomment:到达文件结束位置 CNend*/
#define HI_FORMAT_ERRO_ENDOFFILE            (HI_FAILURE - 1)

/** Return the file size(byte). CNcomment:返回文件实际大小，单位byte CNend*/
#define HI_FORMAT_SEEK_FILESIZE             (65536)

/** @} */  /*! <!-- Macro Definition end */


/*************************** Structure Definition ****************************/
/** \addtogroup      Hiplayer  */
/** @{ */  /** <!-- [Hiplayer] */

/** Maximum number of characters in a uniform resource locator (URL) */
/** CNcomment: 最大路径长度 (字符个数)*/
#define HI_FORMAT_MAX_URL_LEN               (2048)
/** Maximum number of characters  in a file name  */
/** CNcomment:最大文件名称长度 (字符个数)*/
#define HI_FORMAT_MAX_FILE_NAME_LEN         (512)
/** Maximum number of characters in the information such as the file title and author */
/** CNcomment:文件标题、作者等占用的最大字符数 */
#define HI_FORMAT_TITLE_MAX_LEN             (512)
/** Maximum number of characters in a language name*/
/** CNcomment:语言名称含有的最大字符个数 */
#define HI_FORMAT_LANG_LEN                  (4)
/** Maximum number of characters  in a subtitle title */
/** CNcomment:字幕标题的最大长度(字符个数) */
#define HI_FORMAT_SUB_TITLE_LEN             (32)
/** Maximum number of supported languages */
/** CNcomment:支持的最多语言种类个数 */
#define HI_FORMAT_MAX_LANG_NUM              (32)
/** No presentation time stamp (PTS) */
/** CNcomment:没有pts */
#define HI_FORMAT_NO_PTS                    (-1)
/** The value of the position is invalid */
/** CNcomment:没有pos */
#define HI_FORMAT_NO_POS                    (-1)
/** The value of the index is invalid*/
/** CNcomment:没有index*/
#define HI_FORMAT_NO_INDEX                  (-1)
/** Invalid stream ID */
/** CNcomment:非法streamid */
#define HI_FORMAT_INVALID_STREAM_ID         (-1)
/** Maximum bytes of palette, rgb+a */
/** CNcomment:调色板占用的最大字节数，rgb+a */
#define HI_FORMAT_PALETTE_LEN               (4 * 256)
/** Maximum planar surfaces for saving the data of the thumbnail */
/** CNcomment:存放缩略图图像数据所需的最大平面数 */
#define HI_FORMAT_THUMBNAIL_PLANAR          (4)
/** Maximum number of characters in a protocol name */
/** CNcomment: 协议名字最大长度 (字符个数)*/
#define HI_FORMAT_PROTOCAL_NAME_LEN         (32)
/** Maximum number of characters in a subtitle type */
/** CNcomment: 字幕类型的最大长度(字符个数)*/
#define HI_FORMAT_SUB_TYPE_LEN              (32)
/** Maximum number of supported languages in a subtitle file */
/** CNcomment: 一个字幕文件中最多语言个数 */
#define HI_FORMAT_MAX_LANGUAGE_NUM          (4)
/** IV property len is hls segment */
/** CNcomment: hls中分片属性IV的最大长度 */
#define HI_FORMAT_AES_IV_LEN                (16)
/** the mask of mpeg dash descriptor's scheme */
/** CNcomment: mpeg dash descriptor 的 scheme 的mask值 */
#define HI_FORMAT_DESCRIPTOR_SCHEME_MASK (0xFF000000)
/** the mask of mpeg dash descriptor's value */
/** CNcomment: mpeg dash descriptor 的value 的mask值 */
#define HI_FORMAT_DESCRIPTOR_VALUE_MASK (0x00FFFFFF)
/** number of service descriptor in the information such as service name, service provider */
/** CNcomment: 节目服务名称或服务提供的描述字符数 */
#define HI_FORMAT_SERVICE_DESCRIPT_LEN      (64)
/** the length of mimetype info */
/** CNcomment: mimetype 信息数据长度 */
#define HI_FORMAT_MIMETYPE_LEN (256)
/** the max number of DRM header info*/
/** CNcomment: DRM消息头的最大值 */
#define HI_FORMAT_MAX_DRM_NB (32)
/** the max number of chapters*/
/** CNcomment: 支持的最大章节数目 */
#define HI_FORMAT_MAX_CHAPTERS_NUM (128)


/** LOG TAG */
/** CNcomment: 日志输出TAG*/
#define HI_FORMAT_TAG_SWITCH_PG      "SWITCH-PG "
#define HI_FORMAT_TAG_SEEK           "SEEK2 "

/*************************** Structure Definition ****************************/
/** Version number definition */
/** CNcomment:版本号定义 */
typedef struct hiFORMAT_LIB_VERSION_S
{
    HI_U8 u8VersionMajor;    /**< Major version accessor element */
    HI_U8 u8VersionMinor;    /**< Minor version accessor element */
    HI_U8 u8Revision;        /**< Revision version accessor element */
    HI_U8 u8Step;            /**< Step version accessor element */
} HI_FORMAT_LIB_VERSION_S;

/** log level */
/** CNcomment:日志等级 */
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
/** CNcomment:hls 启动模式 */
typedef enum hiFORMAT_HLS_START_MODE_E
{
    HI_FORMAT_HLS_MODE_NORMAL,  /**< manual mode *//**< CNcomment:普通模式,从最大分片开始播 */
    HI_FORMAT_HLS_MODE_FAST,    /**< auto mode,switch by the bandwidth *//**< CNcomment:快速启动,从最小分片开始播 */
    HI_FORMAT_HLS_MODE_BUTT
} HI_FORMAT_HLS_START_MODE_E;

/** File opening mode */
/** CNcomment:文件打开方式 */
typedef enum hiFORMAT_URL_OPENFLAG_E
{
    HI_FORMAT_URL_OPEN_RDONLY = 0x0,   /**< Read-only mode *//**< CNcomment:只读方式 */
    HI_FORMAT_URL_OPEN_RDWR,           /**< Read/Write mode *//**< CNcomment:读/写方式 */
    HI_FORMAT_URL_OPEN_BUTT
} HI_FORMAT_URL_OPENFLAG_E;

/** File seeking flag */
/** CNcomment:文件SEEK标记 */
typedef enum hiFORMAT_SEEK_FLAG_E
{
    HI_FORMAT_AVSEEK_FLAG_NORMAL = 0x0,     /**< Seek the key frame that is closest to the current frame in a sequence from the current frame to end of the file. *//**< CNcomment:从当前帧向文件尾seek，找到离当前帧最近的关键帧 */
    HI_FORMAT_AVSEEK_FLAG_BACKWARD,         /**< Seek the key frame that is closest to the current frame in a sequence from the current frame to the file header. *//**< CNcomment:从当前帧向文件头seek，找到离当前帧最近的关键帧 */
    HI_FORMAT_AVSEEK_FLAG_BYTE,             /**< Seek the key frame based on bytes. *//**< CNcomment:按字节seek */
    HI_FORMAT_AVSEEK_FLAG_ANY,              /**< Seek the key frame that is closest to the current frame regardless of the sequence from the current frame to the file header or from the current frame to the end of the file. *//**< CNcomment:不管是向文件头还是文件尾，找到离当前帧最近的关键帧 */
    HI_FORMAT_AVSEEK_FLAG_BUTT
} HI_FORMAT_SEEK_FLAG_E;

/** Type of the stream data that is parsed by the file DEMUX */
/** CNcomment:流类型，文件解析器解析出来的流数据类型 */
typedef enum hiFORMAT_DATA_TYPE_E
{
    HI_FORMAT_DATA_NULL = 0x0,
    HI_FORMAT_DATA_AUD,          /**< Audio stream *//**< CNcomment:音频流 */
    HI_FORMAT_DATA_VID,          /**< Video stream *//**< CNcomment:视频流 */
    HI_FORMAT_DATA_SUB,          /**< Subtitle stream *//**< CNcomment:字幕流 */
    HI_FORMAT_DATA_RAW,          /**< Byte stream *//**< CNcomment:字节流 */
    HI_FORMAT_DATA_BUTT
} HI_FORMAT_DATA_TYPE_E;

/** mpeg dash Role descriptor scheme , take the most significant 8 bits of 32bits, refer to HI_FORMAT_DESCRIPTOR_SCHEME_MASK*/
/** CNcomment: mpeg dash Role descriptor 属性值,占用32位中的高8位，请参考HI_FORMAT_DESCRIPTOR_SCHEME_MASK*/
typedef enum hiFORMAT_ROLE_SCHEME_E
{
    HI_FORMAT_ROLE_SCHEME_NONE = 0,
    HI_FORMAT_ROLE_SCHEME_URN_MPEG_DASH_2011 = (1 << 24), /**<  urn:mpeg:dash:role:2011*/
} HI_FORMAT_ROLE_SCHEME_E;

/** mpeg dash Role descriptor value ,take the least 24bits of 32bits, refer to HI_FORMAT_DESCRIPTOR_VALUE_MASK*/
/** CNcomment: mpeg dash Role descriptor 属性值，占用32位中的低24位，请参考HI_FORMAT_DESCRIPTOR_VALUE_MASK*/
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
/** CNcomment: mpeg dash Accessibility descriptor scheme, 占用32位中的高8位，请参考HI_FORMAT_DESCRIPTOR_SCHEME_MASK*/
typedef enum hiFORMAT_ACCESSIBILITY_SCHEME_E
{
    HI_FORMAT_ACCESSIBILITY_SCHEME_NONE = 0,
    HI_FORMAT_ACCESSIBILITY_SCHEME_TVA_AUDIO_PURPOSE_CS_2007 = (1 << 24), /**<  urn:tva:metadata:cs:AudioPurposeCS:2007*/
} HI_FORMAT_ACCESSIBILITY_SCHEME_E;


/** Source type of the file to be played */
/** CNcomment:文件源类型 */
typedef enum hiFORMAT_SOURCE_TYPE_E
{
    HI_FORMAT_SOURCE_LOCAL = 0x0,   /**< Local file *//**< CNcomment:普通文件 */
    HI_FORMAT_SOURCE_NET_VOD,       /**< Net VOD file *//**< CNcomment:网络点播 */
    HI_FORMAT_SOURCE_NET_LIVE,      /**< Net Live stream *//**< CNcomment:网络流文件 */
    HI_FORMAT_SOURCE_BUTT
} HI_FORMAT_SOURCE_TYPE_E;

/** Stream type of the file to be played */
/** CNcomment:文件流类型，播放的文件类型 */
typedef enum hiFORMAT_STREAM_TYPE_E
{
    HI_FORMAT_STREAM_ES = 0x0,    /**< Element stream (ES) file *//**< CNcomment:es流文件 */
    HI_FORMAT_STREAM_TS,          /**< TS file *//**< CNcomment:ts流文件 */
    HI_FORMAT_STREAM_BUTT
} HI_FORMAT_STREAM_TYPE_E;

/** Seek mode */
/** CNcomment:seek 类型 */
typedef enum hiFORMAT_SEEK_MODE_E
{
    HI_FORMAT_SEEK_MODE_PTS = 0x0,     /**< seek by pts, default value *//**< CNcomment:默认按PTS做seek*/
    HI_FORMAT_SEEK_MODE_POS,           /**< seek by pos only if network stream and without index *//**< CNcomment:只有当网络播放无索引文件时才按POS做seek */
    HI_FORMAT_SEEK_MODE_BUTT
} HI_FORMAT_SEEK_MODE_E;

/** HLS segment encryption type */
/** CNcomment:HLS 分片加密类型 */
typedef enum hiFORMAT_HLS_KEY_TYPE_E
{
    KEY_NONE,
    KEY_AES_128,
} HI_FORMAT_HLS_KEY_TYPE_E;

/** Playlist Type */
/** CNcomment:播放列表类型 */
typedef enum hiFORMAT_PLAYLIST_TYPE_E
{
    HI_FORMAT_PLAYLIST_TYPE_M3U8 = 0x0,     /**<M3U8 type *//**< CNcomment:m3u8类型 */
    HI_FORMAT_PLAYLIST_TYPE_M3U9,           /**< M3u9 type *//**< CNcomment:m3u9类型 */
    HI_FORMAT_PLAYLIST_TYPE_NO_PLAYLIST     /**< NoPlayList type *//**< CNcomment:非播放列表类型 */
} HI_FORMAT_PLAYLIST_TYPE_E;

/** Stream information of the hls file */
/** CNcomment:hls文件的stream的信息 */
typedef struct hiFORMAT_HLS_STREAM_INFO_S
{
    HI_S32     stream_nb;                   /**< Input parameter, index of the stream to be obtained *//**< CNcomment:要获取的流的索引号，输入参数 */
    HI_S32     hls_segment_nb;              /**< Output parameter, total segments of this stream *//**< CNcomment:该路流中总共有多少个分片，输出参数 */
    HI_S64     bandwidth;                   /**< Output parameter, minimum bandwidth of the stream to be obtained*//**<  CNcomment:获取到的流要求的最低带宽，输出参数，单位bits/s */
    HI_CHAR    url[HI_FORMAT_MAX_URL_LEN];  /**< Output parameter, url of the index file of the stream to be obtained *//**< CNcomment:获取到的流的索引文件的url，输出参数 */
} HI_FORMAT_HLS_STREAM_INFO_S;

/** The information of the currently played segment */
/** CNcomment:hls文件当前播放的分片的信息 */
typedef struct hiFORMAT_HLS_SEGMENT_INFO_S
{
    HI_S32    stream_num;                    /**< output parameter, which stream of the currently played segment *//**< CNcomment:当前播放的分片属于哪一路stream，输出参数 */
    HI_S32    total_time;                    /**< output parameter, the duration of the currently played segment *//**< CNcomment:当前播放的分片的时长，输出参数 */
    HI_S32    seq_num;                       /**< output parameter, the serial number of the current segment *//**< CNcomment:当前分片的序列号，输出参数 */
    HI_S64    bandwidth;                     /**< output parameter, the bandwidth demands of the current segment *//**< CNcomment:当前分片要求的带宽，输出参数，单位bits/s */
    HI_CHAR   url[HI_FORMAT_MAX_URL_LEN];    /**< output parameter, the url of the current segment *//**< CNcomment:当前分片的url，输出参数 */
} HI_FORMAT_HLS_SEGMENT_INFO_S;

/** The information of the playlist segment detail info */
/** CNcomment:playlist文件分片的详细信息 */
typedef struct hiFORMAT_PLAYLIST_SEGMENT_DETAIL_INFO_S
{
    HI_S32                      start_time;                 /**< output parameter, the offset time of the segment in all segments *//**< CNcomment:当前分片的开始时间在所有分片中的偏移时间,输出参数 */
    HI_S32                      total_time;                 /**< output parameter, the duration of the currently played segment *//**< CNcomment:当前播放的分片的时长，输出参数 */
    HI_CHAR                     url[HI_FORMAT_MAX_URL_LEN]; /**< output parameter, the url of the current segment *//**< CNcomment:当前分片的url，输出参数 */
    HI_CHAR                     key[HI_FORMAT_MAX_URL_LEN]; /**< output parameter, the decryption key of current segment *//**< CNcomment:当前分片的解密KEY,输出参数 */
    HI_FORMAT_HLS_KEY_TYPE_E    key_type;                   /**< output parameter, the encryption type of current segment *//**< CNcomment:当前分片的加密类型，输出参数 */
    HI_U8                       iv[HI_FORMAT_AES_IV_LEN];   /**< output parameter, the iv info of encryption of current segment *//**< CNcomment:当前分片的IV加密属性 */
} HI_FORMAT_PLAYLIST_SEGMENT_DETAIL_INFO_S;

/** The information of the playlist stream detail info */
/** CNcomment:playlist的详细信息 */
typedef struct hiFORMAT_PLAYLIST_STREAM_DETAIL_S
{
    HI_S32  bandwidth;                                                /**< output parameter, the bandwidth demands of the current stream *//**< CNcomment:当前备选流要求的带宽，输出参数，单位bits/s */
    HI_CHAR url[HI_FORMAT_MAX_URL_LEN];                               /**< output parameter, the url of the current stream *//**< CNcomment:当前备选流的url，输出参数 */
    HI_FORMAT_PLAYLIST_TYPE_E  playlist_type;                         /**< output parameter, playlist type *//**< CNcomment:播放列表类型*/
    struct tagSEGMENT_LIST_S
    {
        HI_S32                                      index;            /**< output parameter, the stream index *//**< CNcomment:当前备选流的索引号,输出参数 */
        HI_S32                                      finished;         /**< output parameter, if is live stream,0 is live *//**< CNcomment:当前备选流是否是直播流,0是直播流 */
        HI_S32                                      target_duration;  /**< output parameter, the max duration of all segments *//**< CNcomment:分片的最大时长 */
        HI_S32                                      segment_nb;       /**< Output parameter, total segments of this stream *//**< CNcomment:该路流中总共有多少个分片，输出参数 */
        HI_FORMAT_PLAYLIST_SEGMENT_DETAIL_INFO_S    **segments;       /**< Output parameter, segments info *//**< CNcomment:分片信息，输出参数 */
    } SEGMENT_LIST_S;
} HI_FORMAT_PLAYLIST_STREAM_DETAIL_S;

/**The highest resolution scanning media restrictions*/
/**CNcomment:分辨率*/
typedef struct hiFORMAT_RESOLUTION_RATIOS
{
    HI_S32      s32Width;   /**< when use for thumbnail restrcit, zero means no limit, not less than zero *//**< CNcomment:用于限制媒体扫描时, 0表示不限制, 不能小于0*/
    HI_S32      s32Height;  /**< when use for thumbnail restrcit, zero means no limit, not less than zero *//**< CNcomment:用于限制媒体扫描时, 0表示不限制, 不能小于0*/
}HI_FORMAT_RESOLUTION_RATIOS;

/**The information of a chapter*/
/**CNcomment:章节信息*/
typedef struct hiFORMAT_CHAPTER_ATOM_S
{
    HI_S64      s64UID;         /**< unique id of the chapter *//**< CNcomment:标示章节的唯一ID */
    HI_S32      s32TimeStart;   /**< timestamp of the start of chapter, unit: ms*//**< CNcomment:章节开始时间,单位ms */
    HI_S32      s32TimeEnd;     /**< timestamp of the end of chapter, unit: ms*//**< CNcomment:章节结束时间,单位ms */
    HI_CHAR*    pszTitle;       /**< the title of the chapter *//**< CNcomment:章节标题*/
    HI_CHAR*    pszLanguage;    /**< the language corresponding to the title *//**< CNcomment: 章节标题的语言类型*/
}HI_FORMAT_CHAPTER_ATOM_S;

typedef struct hiFORMAT_CHAPTER_INFO_S
{
    HI_S32                      s32ChapterNum;                          /**< the total number of chapter*//**< CNcomment:章节总数目 */
    HI_FORMAT_CHAPTER_ATOM_S    stChapter[HI_FORMAT_MAX_CHAPTERS_NUM];  /**< chapter atom*//**< CNcomment:各章节具体信息*/
}HI_FORMAT_CHAPTER_INFO_S;


/** Command of the invoking */
/** CNcomment:invoke操作命令 */
typedef enum hiFORMAT_INVOKE_ID_E
{
    HI_FORMAT_INVOKE_PRE_CLOSE_FILE = 0x0,          /**< the command of the file to be closed, no parameter*//**< CNcomment:预关闭文件，无参数 */
    HI_FORMAT_INVOKE_SET_SOURCE_CODETYPE = 3,       /**< the command of setting the encoding type of the external subtitle. The parameter type is ::HI_CHARSET_CODETYPE_E* *//**< CNcomment:设置外挂字幕字符编码类型，参数类型::HI_CHARSET_CODETYPE_E* */
    HI_FORMAT_INVOKE_SET_DEST_CODETYPE,             /**< the command of setting the encoding type of the external subtitle to be transferred to, the parameter is ::HI_CHARSET_CODETYPE_E* *//**< CNcomment:设置字幕要转成的编码类型，即转成的目标类型,参数类型::HI_CHARSET_CODETYPE_E* */
    HI_FORMAT_INVOKE_SET_CHARSETMGR_FUNC,           /**< the command of setting the function of the charset manager, the parameter is ::HI_CHARSET_FUN_S* *//**< CNcomment:传递字符集管理函数接口.参数类型::HI_CHARSET_FUN_S *charsetmgr */
    HI_FORMAT_INVOKE_GET_METADATA,                  /**< the command of getting the metadata of the media file, the parameter is ::HI_SVR_PLAYER_METADATA_S* *//**< CNcomment:获取媒体文件元数据，参数类型::HI_SVR_PLAYER_METADATA_S* */
    HI_FORMAT_INVOKE_GET_THUMBNAIL,                 /**< the command of getting the thumbnail, the parameter is ::HI_FORMAT_THUMBNAIL_PARAM_S* *//**< CNcomment:获取缩略图，参数为::HI_FORMAT_THUMBNAIL_PARAM_S* */
    HI_FORMAT_INVOKE_SET_COOKIE =9,                 /**< the command of setting the cookie. The parameter is the value of the buffer addr. Type of the parameter is::HI_CHAR* *//**< CNcomment:设置Cookie信息，参数为存储Cookie信息的buffer地址，类型为::HI_CHAR* */
    HI_FORMAT_INVOKE_GET_BANDWIDTH,                 /**< the command of getting the bandwidth, the parameter is ::HI_S64* *//**< CNcomment:网络播放的时候获取当前网络带宽，单位为bps，参数类型::HI_S64* */
    HI_FORMAT_INVOKE_GET_HLS_STREAM_NUM,            /**< the command of getting the numbers of streams in the hls file, the parameter is ::HI_S32* *//**< CNcomment:获取hls文件中总共有几路码流，参数类型::HI_S32* */
    HI_FORMAT_INVOKE_GET_HLS_STREAM_INFO,           /**< the command of getting the information of the selected segment, the parameter is ::HI_FORMAT_HLS_STREAM_INFO_S* *//**< CNcomment:获取hls文件中指定的某一路的信息，参数类型::HI_FORMAT_HLS_STREAM_INFO_S* */
    HI_FORMAT_INVOKE_GET_HLS_SEGMENT_INFO,          /**< the command of getting the information of the currently played segment, the parameter is ::HI_FORMAT_HLS_SEGMENT_INFO_S* *//**< CNcomment:获取正在播放的分片的信息，参数类型::HI_FORMAT_HLS_SEGMENT_INFO_S* */
    HI_FORMAT_INVOKE_GET_PLAYLIST_STREAM_DETAIL_INFO,/**< the command of getting the detail information of playlist file , the parameter is ::HI_FORMAT_PLAYLIST_STREAM_DETAIL_S* *//**< CNcomment:获取播放列表详细信息::HI_FORMAT_PLAYLIST_STREAM_DETAIL_S* */
    HI_FORMAT_INVOKE_SET_HLS_STREAM,                /**< the command of selecting a specified stream, the parameter is ::HI_S32, not support *//**< CNcomment:指定hls文件的某一路播放参数类型::HI_S32,暂不支持 */
    HI_FORMAT_INVOKE_SET_HLS_START_MODE,            /**< the command of setting hls switch mode,the parameter is ::HI_FORMAT_HLS_START_MODE_E *//**< CNcomment:hls 启动模式设置,快速模式,从最小分片开始播::HI_FORMAT_HLS_START_MODE_E */
    HI_FORMAT_INVOKE_SET_BUFFER_CONFIG,             /**< the command of setting the configuration information of the buffer, the buffer is used only for network playing.The parameter is ::HI_FORMAT_BUFFER_CONFIG_S* *//**< CNcomment:设置缓冲配置信息,该缓冲仅用于网络播放. 参数为::HI_FORMAT_BUFFER_CONFIG_S* */
    HI_FORMAT_INVOKE_GET_BUFFER_CONFIG,             /**< the command of obtaining the configuration information of the buffer, the buffer is used only for network playing.The parameter is ::HI_FORMAT_BUFFER_CONFIG_S* *//**< CNcomment:根据type::HI_FORMAT_BUFFER_CONFIG_TYPE_E获取缓冲配置信息, 该缓冲仅用于网络播放. 参数为::HI_FORMAT_BUFFER_CONFIG_S* */
    HI_FORMAT_INVOKE_GET_BUFFER_STATUS,             /**< the command of reading buffer status, the parameter is ::HI_FORMAT_BUFFER_STATUS_S* *//**< CNcomment:Read Buffer status 参数为::HI_FORMAT_BUFFER_STATUS_S* */
    HI_FORMAT_INVOKE_SET_BUFFER_MAX_SIZE = 22,      /**< the command of setting the maximum value of the hiplayer's buffer(decoder's buffer size is not included),the buffer is used only for network playing. The parameter is ::HI_S64*. The maximum value of the buffer is 10 MB by default and cannot be less than 5 MB.*//**< CNcomment:设置hiplayer缓冲最大峰值(bytes),不包括decoder缓冲大小,该缓冲仅用于网络播放.参数为::HI_S64*,默认值为10MB,不能小于缓冲最大值5MB */
    HI_FORMAT_INVOKE_GET_BUFFER_MAX_SIZE,           /**< the command of obtaining the maximum value of the hiplayer's buffer(decoder's buffer size is not included),the buffer is used only for network playing. The parameter is ::HI_S64*. The maximum value of the buffer is 10 MB by default. *//**< CNcomment:获取hiplayer缓冲最大峰值(bytes),不包括decoder缓冲大小,该缓冲仅用于网络播放.参数为::HI_S64*,默认值为10MB */
    HI_FORMAT_INVOKE_SET_HEADERS = 28,              /**< the command of setting the http headers. The parameter is the value of the buffer addr. Type of the parameter is::HI_CHAR* *//**<CNcomment:设置http headers信息，参数为存储headers信息的buffer地址，类型为::HI_CHAR* */
    HI_FORMAT_INVOKE_SET_USERAGENT,                 /**< the command of setting User Agent info,the parameter is ::HI_CHAR * *//**< CNcomment:http 设置用户定义的User Agent信息*/
    HI_FORMAT_INVOKE_SET_REFERER,                   /**< the command of setting http referer info,the parameter is ::HI_CHAR * *//**< CNcomment:http 设置用户定义的Referer信息*/
    HI_FORMAT_INVOKE_SET_NOT_SUPPORT_BYTERANGE,     /**< the command of close byterange support,the parameter is ::HI_S32.The parameter is 1: close http byterange option, 0: open http byterange option,use 0 as the default value. * *//**< CNcomment:http 关闭http协议的byterange 支持.参数取值范围1:关闭http byterange配置,0:打开http byterange配置,默认值为0*/
    HI_FORMAT_INVOKE_SET_LOG_LEVEL,                 /**< the command of setting log level,the parameter is::HI_FORMAT_LOG_LEVEL_E *//**< CNcomment:设置日志等级，参数::HI_FORMAT_LOG_LEVEL_E */
    HI_FORMAT_INVOKE_SET_BUFFER_UNDERRUN = 35,      /**< the command of setting buffer underrun behavior, the parameter is ::HI_S32*, HI_TRUE:pause playing while buffer under run, HI_FALSE:default, do nothing*/ /**< CNcomment:网络播放时设置buffer欠载行为，参数类型为::HI_S32*,HI_TRUE：欠载时暂停播放，HI_FALSE：默认行为，欠载时不做任何动作 */
    HI_FORMAT_INVOKE_SET_HLS_LIVE_START_NUM,        /**< the command of setting hls live stream first play segment index ,the parameter is HI_S32，0:use default(the 3rd from last), >=1:begin from the segment specified by this number*//**< CNcomment:hls 直播流开始播放的分片index(1表示第一个) ::HI_S32 0:默认倒数第3个开始播放，>=1:从指定的序号开始播放 */
    HI_FORMAT_INVOKE_GET_VIDEO_RATING = 38,         /**< the command of getting video rating information,the parameter is::HI_CHAR**, need free by user  *//**< CNcomment:获取视频rating信息，参数::HI_CHAR**, 需要用户释放 */
    HI_FORMAT_INVOKE_GET_AUDIO_RATING,              /**< the command of getting audio rating information,the parameter is::HI_CHAR**, need free by user *//**< CNcomment:获取音频rating信息，参数::HI_CHAR**，需要用户释放 */
    HI_FORMAT_INVOKE_GET_SUBTITLE_RATING,           /**< the command of getting subtitle rating information,the parameter is::HI_CHAR**, need free by user *//**< CNcomment:获取字幕rating信息，参数::HI_CHAR**， 需要用户释放 */
    HI_FORMAT_INVOKE_SET_BAND_COLLECT_FREQ_MS,      /**< the command of set band collect frequence.ms,the parameter is HI_S32 */ /**< CNcomment:设置网速统计的采样时间间隔单位ms,参数类型为 HI_S32 */
    HI_FORMAT_INVOKE_GET_SEEKABLE,                  /**< the command is used to getting if the demux support seek when playing current URL, the parameter is ::HI_BOOL* *//**< CNcomment:用于获取在播放当前URL时demux是否支持seek，参数类型::HI_BOOL* */
    HI_FORMAT_INVOKE_GET_MAX_PLAYSPEED,             /**< the command of getting the max play speed of hiplayer, this max speed may be changed during playing , the type of parameter is HI_S32 * *//**< CNcomment:获取HiPlayer最大播放速度, 该最大播放速度在播放过程中有可能变化，参数类型是HI_S32 * */
    HI_FORMAT_INVOKE_SET_DOLBYRANGEINFO,            /**< the command of setting dolby rangeinfo *//**< CNcomment:设置杜比rangeinfo */
    HI_FORMAT_INVOKE_GET_DOLBYINFO,                 /**< the command of getting dolby format info *//**< CNcomment:获取杜比格式信息 */
    HI_FORMAT_INVOKE_SET_DOLBYDRCMODE,              /**< the command of setting dolby drcmode *//**< CNcomment:设置杜比drcmode */
    HI_FORMAT_INVOKE_SET_EXTERNAL_AUDIOTRACK = 49,  /**< the command of add external audio track,maybe audio track is changed,so must reset audio tarck */ /**< CNcomment:设置音频track的命令，某些时候音频track可能发生变化，所以必须重新设置 */
    HI_FORMAT_INVOKE_SET_SEEK_MODE,                 /**< the command of setting seek mode, the parameter is ::HI_FORMAT_SEEK_MODE_E */ /**< CNcomment:设置seek模式，参数类型为::HI_FORMAT_SEEK_MODE_E */
    HI_FORMAT_INVOKE_GET_SWITCH_PG_TIME = 55,       /**< the command of getting proc switch-program time info, the parameter is HI_SVR_PLAYER_PROC_SWITCHPG_S */
    HI_FORMAT_INVOKE_SET_TPLAY_MODE,                /**< the command of setting tplay mode, the parameter is ::HI_SVR_PLAYER_TPLAY_MODE_E*/ /**< CNcomment:设置TPlay模式，参数::HI_SVR_PLAYER_TPLAY_MODE_E*/
    HI_FORMAT_INVOKE_SET_LOW_DELAY = 60,            /**< the command of setting low delay, the parameter is ::HI_S32 */ /**< CNcomment:设置low delay模式，参数类型为::HI_S32 */
    HI_FORMAT_INVOKE_SET_FREE_RUN,                  /**< the command of setting play free run behavior, the parameter is ::HI_S32*, HI_TRUE:: play video only, as fast as possible; HI_FALSE: resume to normal play*//**< CNcomment:设置free run开关模式，参数类型为::HI_S32,  HI_TRUE:快速输出视频帧; HI_FALSE: 恢复正常播放*/
    HI_FORMAT_INVOKE_SET_HTTP_DOWNLOAD_SIZE_ONCE,   /**< the command of setting download size each http connection in KB, the parameter is ::HI_S32 */ /**< CNcomment:设置每次http连接下载的数据大小，单位KB，参数类型为::HI_S32 */
    HI_FORMAT_INVOKE_GET_SUBTITLE_FRAME,            /**< the command of get subtitle frame, the parameter is ::HI_FORMAT_FRAME_S*/ /**< CNcomment:获取字幕帧数据，参数::HI_FORMAT_FRAME_S*/
    HI_FORMAT_INVOKE_FREE_SUBTITLE_FRAME,           /**< the command of free subtitle frame, the parameter is ::HI_FORMAT_FRAME_S*/ /**< CNcomment:释放字幕帧数据，参数::HI_FORMAT_FRAME_S*/
    HI_FORMAT_INVOKE_SET_HTTP_SERVICE,              /**< the command of setting http service handle, the parameter is ::IMediaHTTPService*/ /**< CNcomment:设置httpservice,用于Android5.x版本，参数::IMediaHTTPService*/
    HI_FORMAT_INVOKE_SET_CACHE_TIME,                /**< the command of setting cache buffer time,the parameter is::HI_S32 *//**< CNcomment:设置缓冲数据时间，参数::HI_S32 */
    HI_FORMAT_INVOKE_GET_SS_MEDIA_INFO,             /**< the command of getting the media information of the smoothstreaming file ::HI_SVR_SMOOTHSTREAMING_MEDIA_INFO_S *//**< CNcomment:获取smoothstreaming文件信息，参数类型::HI_SVR_SMOOTHSTREAMING_MEDIA_INFO_S* */
    HI_FORMAT_INVOKE_SET_THUMBNAIL_RESTRICT,        /**< set the highest resolution for scanning media restrictions, the parameter is ::HI_FORMAT_THUMBNAIL_RESTRICT  *//**< CNcomment:设置限制媒体扫描的最高分辨率, 参数:HI_FORMAT_THUMBNAIL_RESTRICT*/
    HI_FORMAT_INVOKE_SET_SURFACE_TYPE = 70,         /**< the command of setting surface type, the parameter is ::HI_S32 */ /**< CNcomment:设置surface 类型，参数类型为::HI_S32 */
    HI_FORMAT_INVOKE_SET_HDR_MODE,                  /**< the command of setting HDR mode,1: HDR mode; 0: SDR mode, the parameter is ::HI_S32 */ /**< CNcomment:设置HDR显示模式，1: HDR模式; 0: SDR 模式，参数类型为::HI_S32 */
    HI_FORMAT_INVOKE_GET_CHAPTERS,                  /**< the command of getting chapter information, the paramter is::HI_FORMAT_CHAPTER_INFO_S *//**<CNcomment:获取章节信息,参数类型::HI_FORMAT_CHAPTER_INFO_S*/
    /* if hisilicon invoke command, please defined before USER */
    HI_FORMAT_INVOKE_PROTOCOL_USER=100,             /**< the command of setting data to user protocol *//**< CNcomment:设置用户协议数据 */
} HI_FORMAT_INVOKE_ID_E;

/** The format of the thumbnail */
/** CNcomment:缩略图的图像格式 */
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
/** CNcomment:缓冲配置的类型 */
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
/** CNcomment:缓冲配置信息:
    缓冲配置信息，建议考虑当前内存空间，分配内存过大，系统内存耗尽，
    会造成系统oom-killer的问题。缓冲总大小(s64Total)必须小于通过HI_FORMAT_INVOKE_SET_BUFFER_MAX_SIZE
    所设置的缓冲峰值大小，当按可播放时长类型(HI_FORMAT_BUFFER_CONFIG_TIME)配置缓冲时,hiplayer内部会
    自动限制缓冲大小的字节数不超过通过HI_FORMAT_INVOKE_SET_BUFFER_MAX_SIZE所设置的值。
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
/** CNcomment:缓冲状态查询 */
typedef struct hiFORMAT_BUFFER_STATUS_S
{
    HI_S64 s64BufferSize;    /**< The data size(bytes) of the buffer, data in hiplayer's buffer and data in decoder's buffer are all included. */
    HI_S64 s64Duration;      /**<The relative time that the data can be played to, data in hiplayer's buffer and data in decoder's buffer are all included. */
} HI_FORMAT_BUFFER_STATUS_S;

/** Type of network errors */
/** CNcomment:网络错误类别定义 */
typedef enum hiFORMAT_MSG_NETWORK_E
{
    HI_FORMAT_MSG_NETWORK_ERROR_UNKNOW = 0,     /**< unknown error *//**< CNcomment:未知错误 */
    HI_FORMAT_MSG_NETWORK_ERROR_CONNECT_FAILED, /**< connection error *//**< CNcomment:连接失败 */
    HI_FORMAT_MSG_NETWORK_ERROR_TIMEOUT,        /**< operation timeout *//**< CNcomment:读取超时 */
    HI_FORMAT_MSG_NETWORK_ERROR_DISCONNECT,     /**< net work disconnect *//**< CNcomment:断网 */
    HI_FORMAT_MSG_NETWORK_ERROR_NOT_FOUND,      /**< file not found *//**< CNcomment:资源不可用 */
    HI_FORMAT_MSG_NETWORK_NORMAL,               /**< status of network is normal *//**< CNcomment:网络状态正常 */
    HI_FORMAT_MSG_NETWORK_ERROR_BUTT,
} HI_FORMAT_MSG_NETWORK_E;

/** Network status*/
/** CNcomment:网络状态信息 */
typedef struct hiFORMAT_NET_STATUS_S
{
    HI_CHAR szProtocals[HI_FORMAT_PROTOCAL_NAME_LEN];/**< network protocol, for example http *//**< CNcomment:网络协议，如http */
    HI_S32  s32ErrorCode;                           /**< error code of the network protocol, for example 404 *//**< CNcomment:网络协议错误码，如404 */
    HI_FORMAT_MSG_NETWORK_E eType;                  /**< type of network errors *//**< CNcomment:网络错误类别::HI_FORMAT_MSG_NETWORK_ERROR_E */
} HI_FORMAT_NET_STATUS_S;

/*===============================Buffer NetWork End=============================================*/

/** Audio stream information */
/** CNcomment:音频流信息 */
typedef struct hiFORMAT_AUD_INFO_S
{
    HI_S32 s32StreamIndex;                   /**< Stream index. The invalid value is ::HI_FORMAT_INVALID_STREAM_ID. *//**< CNcomment:流索引，非法值为::HI_FORMAT_INVALID_STREAM_ID */
    HI_U32 u32Format;                        /**< Audio encoding format. For details about the value definition, see ::HI_FORMAT_AUDIO_TYPE_E. *//**< CNcomment:音频编码格式，值定义参考::HI_FORMAT_AUDIO_TYPE_E */
    HI_U32 u32Profile;                       /**< Audio encoding version, such as 0x160(WMAV1) and 0x161 (WMAV2). It is valid only for WMA encoding. *//**< CNcomment:音频编码版本，仅对wma编码，0x160(WMAV1), 0x161 (WMAV2) */
    HI_U32 u32SampleRate;                    /**< 8000,11025,441000,... */
    HI_U16 u16BitPerSample;                  /**< Number of bits occupied by each audio sampling point such as 8 bits or 16 bits. *//**< CNcomment:音频每个采样点所占的比特数，8bit,16bit */
    HI_U16 u16Channels;                      /**< Number of channels, 1 or 2. *//**< CNcomment:声道数, 1 or 2 */
    HI_S32 s32SubStreamID;                   /**< Sub audio stream ID *//**< CNcomment:子音频流ID,如果为TrueHD音频，则可能含有一路ac3子流 ，当前处理策略为将两条流分离开来，并用此字段来记录关系 */
    HI_U32 u32BlockAlign;                    /**< Number of bytes contained in a packet *//**< CNcomment:packet包含的字节数 */
    HI_U32 u32Bitrate;                       /**< Audio bit rate, in the unit of bit/s. *//**< CNcomment:音频码率，bit/s */
    HI_BOOL bBigEndian;                      /**< Big endian or little endian. It is valid only for the PCM format *//**< CNcomment:大小端，仅pcm格式有效 */
    HI_CHAR aszLanguage[HI_FORMAT_LANG_LEN]; /**< Audio stream language *//**< CNcomment:音频流语言 */
    HI_U32 u32ExtradataSize;                 /**< Length of the extended data *//**< CNcomment:扩展数据长度 */
    HI_U8  *pu8Extradata;                    /**< Extended data *//**< CNcomment:扩展数据 */
    HI_VOID  *pCodecContext;                 /**< Audio decode context *//**< CNcomment:音频解码上下文 */
    HI_U32 u32Role;                          /**< Role descriptor value of mpeg dash. the most 8 bits is scheme value(refer to HI_FORMAT_ROLE_SCHEME_E), the left 24 bits is descriptor value, the descriptor value may be the bitwise '|' result of value define in HI_FORMAT_ROLE_VALUE_E*/
                                             /**< CNcomment:mpeg dash 的 Role descriptor 的值,高8位代表scheme(请参考HI_FORMAT_ROLE_SCHEME_E),低24位代表descriptor 值,descriptor值是HI_FORMAT_ROLE_VALUE_E中定义的任意1个或多个值的位运算'|' 的结果*/
    HI_U32 u32Accessibility;                 /**<  Accessbilitydescriptor value of mpeg dash. the most 8 bits is scheme value(refer to HI_FORMAT_ROLE_SCHEME_E), the left 24 bits is descriptor value, the descriptor value type is number*/
                                             /**< CNcomment:mpeg dash 的 Accessibility descriptor 的值,高8位代表scheme(请参考hiFORMAT_ACCESSIBILITY_SCHEME_E),低24位代表descriptor 值,descriptor值类型是数字类型*/
    HI_S64 s64Duration;                      /**< Duration of audio stream, in the unit of ms. *//**< CNcomment:音频流时长，单位ms */
} HI_FORMAT_AUD_INFO_S;

/** Dolby format external information */
/** CNcomment:杜比格式的详细信息 */
typedef struct hiFORMAT_AUD_DOLBY_INFO_S
{
    HI_U32 u32DecoderType;      /**< Audio Decoder type *//**< CNcomment:解码器ID */
    HI_S32 s32StreamType;       /**< dolby type 0:dolby 1:dolby plus *//**< CNcomment:杜比格式类型参见DOLBYPLUS_STREAM_INFO_S */
    HI_S32 s32ACMode;           /**< dolby ac mode *//**< CNcomment:参见DOLBYPLUS_STREAM_INFO_S */
} HI_FORMAT_AUD_DOLBY_INFO_S;

/** Type of the hdr stream */
/** CNcomment:hdr视频流类型 */
typedef enum hiFORMAT_HDR_VID_TYPE_E
{
    HI_FORMAT_HDR_VID_NONE = 0x0,  /**< none *//**< CNcomment:非HDR视频流 */
    HI_FORMAT_HDR_VID_SINGLE_SL,   /**< single *//**< CNcomment:单层单流HDR视频流 */
    HI_FORMAT_HDR_VID_SINGLE_DL,   /**< single *//**< CNcomment:双层单流HDR视频流 */
    HI_FORMAT_HDR_VID_DUAL_BL,     /**< dual bl *//**< CNcomment:双层双流HDR视频流的base层 */
    HI_FORMAT_HDR_VID_DUAL_ELRPU,  /**< dual el rpu *//**< CNcomment: 双层双流HDR视频流的enhancement层*/
    HI_FORMAT_HDR_VID_BUTT,
} HI_FORMAT_HDR_VID_TYPE_E;

/** Info of the hdr stream */
/** CNcomment:视频流HDR信息 */
typedef struct hiFORMAT_HDR_VID_INFO_S
{
    HI_FORMAT_HDR_VID_TYPE_E eHdrVidType;  /**< tpye of hdr video stream. *//**< CNcomment:hdr视频流类型 */
    HI_S32 s32VdepTrackId;                 /**< trackid of hdr dual track video stream dependency. *//**< CNcomment:hdr双流视频流依赖的视频流ID */
    HI_U32 u32ElFormat;                    /**< Video encoding format. For details about the value definition, see ::HI_FORMAT_VIDEO_TYPE_E. *//**< CNcomment:视频编码格式，值定义参考::HI_FORMAT_VIDEO_TYPE_E */
    HI_U32 u32ElWidth;                     /**< Width, in the unit of pixel. *//**< CNcomment:宽度，单位像素 */
    HI_U32 u32ElHeight;                    /**< Height, in the unit of pixel. *//**< CNcomment:高度，单位像素 */
    HI_S32 s32Compatible;                  /**< backward compatibility,0:non-backward compatible; 1:SDR backward compatible; 2: Ultra HD Blu-rayTM HDR compatible*//**< CNcomment:兼容能力 */
} HI_FORMAT_HDR_VID_INFO_S;

/** Video stream information */
/** CNcomment:视频流信息 */
typedef struct hiFORMAT_VID_INFO_S
{
    HI_S32 s32StreamIndex;                  /**< Stream index. The invalid value is ::HI_FORMAT_INVALID_STREAM_ID. *//**< CNcomment:流索引，非法值为::HI_FORMAT_INVALID_STREAM_ID */
    HI_U32 u32Format;                       /**< Video encoding format. For details about the value definition, see ::HI_FORMAT_VIDEO_TYPE_E. *//**< CNcomment:视频编码格式，值定义参考::HI_FORMAT_VIDEO_TYPE_E */
    HI_U16 u16RefFrameNum;                  /**< Number of reference frames. *//**< CNcomment:参考帧数目 */
    HI_U16 u16Profile;                      /**< Profile level. *//**< CNcomment:Profile级别 */
    HI_U16 u16Width;                        /**< Width, in the unit of pixel. *//**< CNcomment:宽度，单位像素 */
    HI_U16 u16Height;                       /**< Height, in the unit of pixel. *//**< CNcomment:高度，单位像素 */
    HI_U16 u16FpsInteger;                   /**< Integer part of the frame rate *//**< CNcomment:帧率，整数部分 */
    HI_U16 u16FpsDecimal;                   /**< Decimal part of the frame rate *//**< CNcomment:帧率，小数部分 */
    HI_U32 u32Bitrate;                      /**< Video bit rate, in the unit of bit/s. *//**< CNcomment:视频码率，bit/s */
    HI_U32 u32CodecVersion;                 /**< Version of codec. *//**< CNcomment:Codec 版本 */
    HI_U32 u32Rotate;                       /**< Video rotation angle, value is 90/180/270, default value is 0 *//**< CNcomment:视频旋转角度,90/180/270,默认值0 */
    HI_U32 u32Reversed;
    HI_BOOL bEnableTVP;
    HI_U32 u32ExtradataSize;                /**< Length of the extended data *//**< CNcomment:扩展数据长度 */
    HI_U8  *pu8Extradata;                   /**< Extended data *//**< CNcomment:扩展数据 */
    HI_VOID  *pCodecContext;                /**< video decode context *//**< CNcomment:视频解码上下文 */
    HI_U32 u32Role;                         /**< Role descriptor value of mpeg dash. the most 8 bits is scheme value(refer to HI_FORMAT_ROLE_SCHEME_E), the left 24 bits is descriptor value, the descriptor value may be the bitwise '|' result of value define in HI_FORMAT_ROLE_VALUE_E*/
                                            /**< CNcomment:mpeg dash 的 Role descriptor 的值,高8位代表scheme(请参考HI_FORMAT_ROLE_SCHEME_E),低24位代表descriptor 值,descriptor值是HI_FORMAT_ROLE_VALUE_E中定义的任意1个或多个值的位运算'|' 的结果*/
    HI_U32 u32Accessibility;                /**<  Accessbilitydescriptor value of mpeg dash. the most 8 bits is scheme value(refer to HI_FORMAT_ROLE_SCHEME_E), the left 24 bits is descriptor value, the descriptor value type is number*/
                                            /**< CNcomment:mpeg dash 的 Accessibility descriptor 的值,高8位代表scheme(请参考hiFORMAT_ACCESSIBILITY_SCHEME_E),低24位代表descriptor 值,descriptor值类型是数字类型*/
    HI_S64 s64Duration;                     /**< Duration of video stream, in the unit of ms. *//**< CNcomment:视频流时长，单位ms */
    HI_FORMAT_HDR_VID_INFO_S stHdrVidInfo;  /**< info of hdr video stream. *//**< CNcomment:hdr视频流信息 */
} HI_FORMAT_VID_INFO_S;

/** Subtitle information */
/** CNcomment:字幕信息 */
typedef struct hiFORMAT_SUB_INFO_S
{
    HI_S32  s32StreamIndex;                            /**< Stream index. The invalid value is ::HI_FORMAT_INVALID_STREAM_ID. *//**< CNcomment:流索引，非法值为::HI_FORMAT_INVALID_STREAM_ID */
    HI_U32  u32Format;                                 /**< Subtitle format, For details about the value definition, see::HI_FORMAT_SUBTITLE_TYPE_E *//**< CNcomment:字幕格式，取值::HI_FORMAT_SUBTITLE_TYPE_E */
    HI_U32  u32CharSet;                                /**< Encoding type of the subtitle, the value range is as follows:
                                                            1. The default value is 0.
                                                            2. The value of the u32CharSet is the identified byte encoding value if the IdentStream byte encoding function (for details about the definition, see hi_charset_common.h) is set.
                                                            3. If the ConvStream function (for details about the definition, see hi_charset_common.h) is set and the invoke interface is called to set the encoding type to be converted by implementing HI_FORMAT_INVOKE_SET_SOURCE_CODETYPE, the value of the u32CharSet is the configured encoding type */
                                                       /**< CNcomment:字幕编码类型，取值范围如下:
                                                            1. 默认值为0
                                                            2. 如果设置了IdentStream字符编码识别函数
                                                               (参考hi_charset_common.h)，则该属性值为识别出的字符编码值
                                                            3. 如果设置了ConvStream转码函数 (参考hi_charset_common.h)，
                                                              并且调用Invoke接口执行HI_FORMAT_INVOKE_SUB_SET_CODETYPE设
                                                              置要转换成的编码类型，该值为设置的编码类型*/
    HI_BOOL bExtSub;                                  /**< Whether subtitles are external subtitles. When bExtSub is HI_TRUE, the subtitles are external. When bExtSub is HI_FALSE, the subtitles are internal. *//**< CNcomment:是否为外挂字幕, HI_TRUE为外挂字幕，HI_FALSE为内置字幕 */
    HI_U32  u32StreamNum;                             /**< contains stream number *//**< CNcomment:包含的字幕流数目 */
    HI_CHAR paszLanguage[HI_FORMAT_MAX_LANGUAGE_NUM][HI_FORMAT_LANG_LEN]; /**< Subtitle language *//**< CNcomment:字幕语言 */
    HI_U16  u16OriginalFrameWidth;                     /**< Width of the original image *//**< CNcomment:原始图像宽度 */
    HI_U16  u16OriginalFrameHeight;                    /**< Height of the original image *//**< CNcomment:原始图像高度 */
    HI_U32  u32ExtradataSize;                          /**< Length of the extended data *//**< CNcomment:扩展数据长度 */
    HI_U8   *pu8Extradata;                             /**< Extended data *//**< CNcomment:扩展数据 */
    HI_VOID  *pCodecContext;                           /**< Audio decode context *//**< CNcomment:字幕解码上下文 */
    HI_U32 u32Role;                                    /**< Role descriptor value of mpeg dash. the most 8 bits is scheme value(refer to HI_FORMAT_ROLE_SCHEME_E), the left 24 bits is descriptor value, the descriptor value may be the bitwise '|' result of value define in HI_FORMAT_ROLE_VALUE_E*/
                                                       /**< CNcomment:mpeg dash 的 Role descriptor 的值,高8位代表scheme(请参考HI_FORMAT_ROLE_SCHEME_E),低24位代表descriptor 值,descriptor值是HI_FORMAT_ROLE_VALUE_E中定义的任意1个或多个值的位运算'|' 的结果*/
    HI_U32 u32Accessibility;                           /**<  Accessibility descriptor value of mpeg dash. the most 8 bits is scheme value(refer to HI_FORMAT_ROLE_SCHEME_E), the left 24 bits is descriptor value, value type is number*/
                                                       /**< CNcomment:mpeg dash 的 Accessibility descriptor 的值,高8位代表scheme(请参考hiFORMAT_ACCESSIBILITY_SCHEME_E),低24位代表descriptor 值,descriptor值类型是数字类型*/
} HI_FORMAT_SUB_INFO_S;


/** Program information. If a file contains internal subtitles and external subtitles, add the external subtitles to the end of the internal subtitles.*/
/** CNcomment:节目信息，如果文件既带内置字幕也带外置字幕，则外置字幕信息追加在内置字幕后*/
typedef struct hiFORMAT_PROGRAM_INFO_S
{
    HI_U32 u32VidStreamNum;                /**< Number of video streams *//**< CNcomment:视频流个数 */
    HI_FORMAT_VID_INFO_S *pastVidStream;   /**< Video stream information *//**< CNcomment:视频流信息 */
    HI_U32 u32AudStreamNum;                /**< Number of audio streams *//**< CNcomment:音频流个数 */
    HI_FORMAT_AUD_INFO_S *pastAudStream;   /**< Audio stream information *//**< CNcomment:音频流信息 */
    HI_U32 u32SubStreamNum;                /**< Number of subtitles *//**< CNcomment:字幕个数 */
    HI_FORMAT_SUB_INFO_S *pastSubStream;   /**< Subtitle information *//**< CNcomment:字幕信息 */
    HI_CHAR aszServiceName[HI_FORMAT_SERVICE_DESCRIPT_LEN];       /**< Program service name info *//**< CNcomment:节目名称 */
    HI_CHAR aszServiceProvider[HI_FORMAT_SERVICE_DESCRIPT_LEN];   /**<  Program service provider info *//**< CNcomment:节目提供商 */
    HI_S64  s64ProgramDuration;
    HI_S64  s64ProgramStartTime;
} HI_FORMAT_PROGRAM_INFO_S;

/** Media file information */
/** CNcomment:媒体文件信息 */
typedef struct hiFORMAT_FILE_INFO_S
{
    HI_FORMAT_SOURCE_TYPE_E eSourceType;    /**< File source type *//**< CNcomment:文件源类型 */
    HI_FORMAT_STREAM_TYPE_E eStreamType;    /**< File stream type *//**< CNcomment:文件流类型 */
    HI_S64  s64FileSize;                    /**< File size, in the unit of byte. *//**< CNcomment:文件大小，单位字节 */
    HI_S64  s64StartTime;                   /**< Start time of playing a file, in the unit is ms. *//**< CNcomment:文件播放起始时间，单位ms */
    HI_S64  s64Duration;                    /**< Total duration of a file, in the unit of ms. *//**< CNcomment:文件总时长，单位ms */
    HI_U32  u32Bitrate;                     /**< File bit rate, in the unit of bit/s. *//**< CNcomment:文件码率，bit/s */
    HI_CHAR aszFileFormat[HI_FORMAT_TITLE_MAX_LEN];   /**< File demuxer info .Not used now*//**< CNcomment:文件格式描述，来自于解析器。目前没有使用 */
    HI_U32  u32ProgramNum;                  /**< Actual number of programs *//**< CNcomment:实际节目个数 */
    HI_FORMAT_PROGRAM_INFO_S *pastProgramInfo; /**< Program information *//**< CNcomment:节目信息 */
    HI_BOOL bIsDivx;                        /**< If the stream is DIVX restricted stream,HI_TRUE yes,HI_FALSE no *//**< CNcomment:是否DIVX限制码流，HI_TRUE 是，HI_FALSE 不是 */
    HI_BOOL bIsDrmFile;
} HI_FORMAT_FILE_INFO_S;

/** Frame data */
/** CNcomment:帧数据 */
typedef struct hiFORMAT_FRAME_S
{
    HI_FORMAT_DATA_TYPE_E eType;          /**< Data type *//**< CNcomment:数据类型 */
    HI_S32  s32StreamIndex;               /**< Stream index *//**< CNcomment:流索引 */
    HI_BOOL bKeyFrame;                    /**< Whether the frame is a key frame *//**< CNcomment:是否为关键帧 */
    HI_U32  u32Len;                       /**< Length of frame data, in the unit of byte. This parameter is an input or output parameter in RAW mode. It indicates the number of bytes to be read if it is an input parameter, and indicates the number of
                                                                  bytes that are actually read if it is an output parameter. */
                                          /**< CNcomment:帧数据长度，单位字节，对于raw方式，该参数为输入/输出参数，
                                                                 输入为要读取的字节数，输出为实际读取的字节数 */
    HI_U8   *pu8Addr;                     /**< Address of the frame data *//**< CNcomment:帧数据地址 */
    HI_U8   *pu8FrameHeader;              /**< Frame data header address *//**< CNcomment:帧数据前的头数据地址，如果不需要，则设置为NULL */
    HI_U32  u32FrameHeaderLen;            /**< Frame data header data length  *//**< CNcomment:帧数据前的头数据长度 */
    HI_S64  s64Pts;                       /**< Presentation time stamp (PTS), IN the unit of ms. If there is no PTS, it is set to ::HI_FORMAT_NO_PTS. *//**< CNcomment:时间戳，单位ms，如果没有时间戳，设置为::HI_FORMAT_NO_PTS */
    HI_S64  s64Pos;                       /**< position of this frame in the media file.*//**< CNcomment:该帧数据在媒体文件中的字节偏移位置 */
    HI_U32  u32Duration;                  /**< Display duration, in the unit of ms. It is set to 0 if there is no duration. *//**< CNcomment:显示时长，单位ms，没有duration，设置为0 */
    HI_U32  u32UserData;                  /**< Private data. The DEMUX can transparently transmit private data by using this parameter. *//**< CNcomment:私有数据，解析器可以通过该参数回带私有数据 */
    HI_U32  u32Format;                   /**< Video/Audio/Subtitle format, For details about the value definition, see::HI_FORMAT_VIDEO_TYPE_E/HI_FORMAT_AUDIO_TYPE_E/HI_FORMAT_SUBTITLE_TYPE_E *//**< CNcomment:视频/音频/字幕/字幕格式，取值::HI_FORMAT_VIDEO_TYPE_E/HI_FORMAT_AUDIO_TYPE_E/HI_FORMAT_SUBTITLE_TYPE_E */

    HI_U8   *pu8IVBuffer;                 /**< DRM IV buffer *//**< CNcomment: DRM 解密向量 */
    HI_U32  u32IVSize;                    /**< DRM IV buffer *//**< CNcomment: DRM 解密向量大小 */
    HI_U8   *pu8KeyID;                    /**< DRM key ID buffer *//**< CNcomment: DRM UUID缓存 */
    HI_U32  u32KeyIDSize;                 /**< DRM key ID buffer size*//**< CNcomment: DRM UUID缓存大小 */
    HI_S32  s32Cryptomode;
    HI_S32  s32Encryptsize;
    HI_S32  *ps32Encryptsize;
} HI_FORMAT_FRAME_S;
/** Parameters of the thumbnail */
/** CNcomment:缩略图参数 */
typedef struct hiFORMAT_THUMBNAIL_PARAM_S
{
    HI_FORMAT_THUMB_PIXEL_FORMAT_E thumbPixelFormat;    /**< It is an input parameter. It specifies the picture format of the obtained thumbnail. *//**< CNcomment:输入参数：用于指定获取到的缩略图的图像格式 */
    HI_S64 thumbnailTimeMs;          /**< It is an input parameter. It specifies the time of the obtained thumbnail. *//**< CNcomment:输入参数：用于获取指定时间的缩略图 */
    HI_S32 thumbnailSize;                               /**< It is an input parameter. It specifies the larger value between the width (tw) and height (th) of the obtained thumbnail.
                                                             Note: The value of thumbnailSize cannot be 0 in rule 1 and rule 2.
                                                             1. If the source video width (w) is greater than height (h), the width (tw) of the obtained thumbnail is thumbnailSize,
                                                               and the height (th) of the obtained thumbnail is (tw x w)/h.
                                                             2. If the source video height (h) is greater than the source picture width (w), the height (th) of the obtained thumbnail is thumbnailSize,
                                                               and the width (tw) of the obtained thumbnail is (th x h)/w.
                                                             3. If thumbnailSize is 0, the width (tw) of the obtained thumbnail is the source video width (w),
                                                               and the height (th) of the obtained thumbnail is the source video height (h).*/
                                                        /**< CNcomment:输入参数：用于指定获取到的缩略图的宽(tw)和高(th)中的较大值；
                                                             说明(规则1和2中的thumbnailSize的取值不为0)：
                                                             1.如果原始视频的宽(w)大于高(h)，则获取到的缩略图的宽(tw)为thumbnailSize,
                                                               获取到的缩略图的高(th)为(tw*w)/h；
                                                             2.如果原始视频的高(h)大于宽(w)，则获取到的缩略图的高(th)为thumbnailSize,
                                                               获取到的缩略图的宽(tw)为(th*h)/w；
                                                             3.如果将thumbnailSize置为0，则获取到的缩略图的宽(tw)为原始视频的宽(w)，
                                                               获取到的缩略图的高(th)为原始视频的高(h). */

    HI_S32 width;                                       /**< It is an output parameter. The width of the obtained thumbnail is returned.*//**< CNcomment:输出参数：返回获取到的缩略图的宽度 */
    HI_S32 height;                                      /**< It is an output parameter. The height of the obtained thumbnail is returned.*//**< CNcomment:输出参数：返回获取到的缩略图的高度 */
    HI_S32 lineSize[HI_FORMAT_THUMBNAIL_PLANAR];        /**< It is an output parameter. The width (in byte) of the block for storing the thumbnail data is returned.
                                                             Note:
                                                             1. For the current picture formats, a maximum of four blocks (HI_FORMAT_THUMBNAIL_PLANAR) are required for storing the picture data.
                                                               lineSize[0] to lineSize[HI_FORMAT_THUMBNAIL_PLANAR-1] indicate the
                                                               block widths (in byte) of block 0 to block (HI_FORMAT_THUMBNAIL_PLANAR-1) respectively.
                                                             2. For the RGB picture, only one block (block 0) is used. In this case, pay attention only to lineSize[0].
                                                             3. For the YUV picture, if the Y, U, and V components are stored in blocks, they are separately stored in a block.
                                                               lineSize[0] to lineSize[2] indicate the width of the blocks that store the Y, U, and V components respectively. */
                                                        /**< CNcomment:输出参数：返回用于存放缩略图图像数据的各个平面一行所占的字节数；
                                                             说明:
                                                             1.对于目前已有的图像格式，最多需要HI_FORMAT_THUMBNAIL_PLANAR(即4)个平面存放图像数据，
                                                               lineSize[0]~lineSize[HI_FORMAT_THUMBNAIL_PLANAR-1]分别表示第0~(HI_FORMAT_THUMBNAIL_PLANAR-1)
                                                               个平面一行所占的字节数；
                                                             2.对于RGB类的图像格式，只使用了1个平面，即第0个平面，因此只需关注lineSize[0]即可；
                                                             3.对于YUV类的图像格式，如果采用平面方式存储，即Y分量单独用一个平面存储，U分量和V分量也单独
                                                               用一个平面存储，则lineSize[0]~lineSize[2]分别表示Y,U和V三个平面一行所占的字节数. */
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
                                                        /**< CNcomment:输出参数：返回缩略图图像数据在各个平面的存储地址;
                                                             说明:
                                                             1.对于目前已有的图像格式，最多需要HI_FORMAT_THUMBNAIL_PLANAR(即4)个平面存放图像数据，
                                                               frameData[0]~frameData[HI_FORMAT_THUMBNAIL_PLANAR-1]分别表示缩略图图像数据在
                                                               第0~(HI_FORMAT_THUMBNAIL_PLANAR-1)个平面上的存储地址；
                                                             2.对于RGB类的图像格式，只使用了1个平面，即第0个平面，因此只需关注frameData[0]即可；
                                                             3.对于YUV类的图像格式，如果采用平面方式存储，即Y分量单独用一个平面存储，U分量和V分量也单独
                                                               用一个平面存储，则frameData[0]~frameData[2]分别表示缩略图图像数据在Y,U和V三个平面上的存储地址;
                                                             4.frameData的存储空间是由底层分配的，用户程序在使用完frameData后，需要使用free释放frameData指向的
                                                               存储空间；frameData中没有使用的部分为NULL，因此free前请务必判断是否为NULL. */
} HI_FORMAT_THUMBNAIL_PARAM_S;

/** Protocol (such as file protocol and HTTP) API */
/** CNcomment:协议（如：file，http）API */
typedef struct hiFORMAT_PROTOCOL_FUN_S
{
    /**
    \brief Check whether the entered protocol is supported. CNcomment:查找输入的协议是否支持CNend
    \attention \n
    None
    \param[in] pszUrl file URL, such as http://. CNcomment:文件url，如http:// CNend

    \retval ::HI_SUCCESS The protocol is supported. CNcomment:支持该协议CNend
    \retval ::HI_FAILURE The protocol is not supported. CNcomment:不支持该协议CNend

    \see \n
    None
    */
    HI_S32 (*url_find)(const HI_CHAR *pszUrl);

    /**
    \brief Open a file. CNcomment:打开文件CNend
    \attention \n
    None
    \param [in]  pszUrl URL address. CNcomment:url地址CNend
    \param [in]  eFlag open mode. CNcomment:打开方式CNend
    \param [out] pUrlHandle protocol handle. CNcomment:协议句柄CNend

    \retval ::HI_SUCCESS The file is successfully opened and the handle is valid. CNcomment:文件打开成功，句柄有效CNend
    \retval ::HI_FAILURE The file fails to be opened. CNcomment:文件打开失败CNend

    \see \n
    None
    */
    HI_S32 (*url_open)(const HI_CHAR *pszUrl, HI_FORMAT_URL_OPENFLAG_E eFlag, HI_VOID **pUrlHandle);

    /**
    \brief Read data of the specified size. CNcomment:读取指定大小数据CNend
    \attention \n
    None
    \param [in] urlHandle file handle. CNcomment:文件句柄CNend
    \param [in] s32Size size of the read data, in the unit of byte. CNcomment:读取的数据大小，字节为单位CNend
    \param [out] pu8Buf data storage buffer. CNcomment:数据存储缓冲CNend

    \retval ::Number greater than 0: The returned value indicates the number of read bytes, if less than s32Size, indicate the file ends.
                CNcomment:>0 返回的值是读取的字节数，如果实际读取的字节数小于s32Size，表明文件结束CNend
    \retval ::HI_FAILURE The file ends. CNcomment:文件结束CNend

    \see \n
    None
    */
    HI_S32 (*url_read)(HI_VOID *urlHandle, HI_U8 *pu8Buf, HI_S32 s32Size);

    /**
    \brief Seek to a specified location. HI_FAILURE is returned if the file does not support seek operations. CNcomment:seek到指定位置，对于不支持seek的文件，返回HI_FAILURE，CNend
    \attention \n
    None
    \param [in] urlHandle file handle. CNcomment:文件句柄CNend
    \param [in] s64Offsets number of sought bytes. CNcomment:seek字节数CNend
    \param [in] s32Whence seeking flag. SEEK_CUR indicates seeking after s64Offsets is deviated from the current location,
                SEEK_SET indicates seeking from the start location, SEEK_END indicates seeking from the end location and HI_FORMAT_SEEK_FILESIZE return the file size.
                CNcomment:seek标示，SEEK_CUR从当前位置偏移s64Offsets,SEEK_SET从起始位置偏移s64Offsets, SEEK_END从文件尾偏移s64Offsets,
                HI_FORMAT_SEEK_FILESIZE返回文件大小。CNend
    \retval ::Value other than HI_FAILURE: It indicates the number of start offset bytes in the file . CNcomment:非HI_FAILURE 文件起始偏移字节数CNend
    \retval ::HI_FAILURE The seeking fails. CNcomment:seek失败CNend

    \see \n
    None
    */
    HI_S64 (*url_seek)(HI_VOID *urlHandle, HI_S64 s64Offsets, HI_S32 s32Whence);

    /**
    \brief Destroy the file handle. CNcomment:销毁文件句柄CNend
    \attention \n
    None
    \param [in] urlHandle file handle. CNcomment:文件句柄CNend

    \retval ::HI_SUCCESS The file handle is destroyed. CNcomment:文件句柄销毁成功CNend
    \retval ::HI_FAILURE The parameter is invalid. CNcomment:参数非法CNend

    \see \n
    None
    */
    HI_S32 (*url_close)(HI_VOID *urlHandle);

    /**
    \brief Seek a specified stream.  CNcomment:seek指定的流CNend
    \attention \n
    None
    \param [in] urlHandle file handle. CNcomment:文件句柄CNend
    \param [in] s32StreamIndex stream index. CNcomment:流索引CNend
    \param [in] s64Timestamp PTS, in the unit of ms. CNcomment:时间戳，单位ms CNend
    \param [in] eFlags seeking flag. CNcomment:seek标记CNend

    \retval ::HI_SUCCESS The seeking is successful. CNcomment:seek成功CNend
    \retval ::HI_FAILURE The seeking fails. CNcomment:seek失败CNend

    \see \n
    None
    */
    HI_S32 (*url_read_seek)(HI_VOID *urlHandle, HI_S32 s32StreamIndex, HI_S64 s64Timestamp, HI_FORMAT_SEEK_FLAG_E eFlags);
    /**
    \brief invoke operation. CNcomment:invoke操作CNend
    \attention \n
    None
    \param [in] urlHandle file handle. CNcomment:文件句柄CNend
    \param [in] u32InvokeId The value of the command is the type of HI_FORMAT_INVOKE_ID_E. The type of HI_FORMAT_INVOKE_ID_E only supports HI_FORMAT_INVOKE_PRE_CLOSE_FILE and HI_FORMAT_INVOKE_USER_OPE.
                CNcomment:值为::HI_FORMAT_INVOKE_ID_E类型, 仅支持HI_FORMAT_INVOKE_PRE_CLOSE_FILE/HI_FORMAT_INVOKE_USER_OPE CNend
    \param [in] pArg the parameter corresponding to the invoke operation. CNcomment:invoke操作带的参数CNend

    \retval ::HI_SUCCESS The operation is successful. CNcomment:成功CNend
    \retval ::HI_FAILURE The operation fails. CNcomment:失败CNend

    \see \n
    None
    */
    HI_S32 (*url_invoke)(HI_VOID *urlHandle, HI_U32 u32InvokeId, HI_VOID *pArg);
} HI_FORMAT_PROTOCOL_FUN_S;

/** File DEMUX application programming interface (API) */
/** CNcomment:文件解析器API */
typedef struct hiFORMAT_DEMUXER_FUN_S
{
    /**
    \brief Check whether the entered file format is supported. The value 0 indicates that the file format is supported and other values indicate that the file format is not supported.
           CNcomment:查找输入的文件格式是否支持，返回0支持，其他不支持CNend
    \attention \n
    None
    \param [in] pszFileName file name, that is, path+file name. CNcomment:文件名称，路径+文件名CNend
    \param [in] pstProtocol interface of the protocol. The URL can be open and the file data can be read by using this interface. CNcomment:协议操作接口，通过该接口可以打开url，读取文件数据CNend

    \retval ::HI_SUCCESS The file format is supported. CNcomment:支持该文件格式CNend
    \retval ::HI_FAILURE The file format is not supported. CNcomment:不支持该文件格式CNend

    \see \n
    None
    */
    HI_S32 (*fmt_find)(const HI_CHAR *pszFileName, const HI_FORMAT_PROTOCOL_FUN_S *pstProtocol);

    /**
    \brief Open a specified file. CNcomment:打开指定文件CNend
    \attention \n
    None
    \param [in] pszFileName file path. CNcomment:文件路径CNend
    \param [in] pstProtocol protocol operation interface. You can specify the protocol operation interface by using this parameter if no protocol operation interface is configured for a registered dynamic link library (DLL).
                CNcomment:协议操作接口，注册的动态库如果没有协议操作接口可以通过该方式指定CNend
    \param [out] pFmtHandle handle of the output file DEMUX. CNcomment:输出文件解析器句柄CNend

    \retval ::HI_SUCCESS The file is opened successfully and the DEMUX handle is valid. CNcomment:文件打开成功，解析器句柄有效CNend
    \retval ::HI_FAILURE The file fails to be opened. CNcomment:文件打开失败CNend

    \see \n
    None
    */
    HI_S32 (*fmt_open)(const HI_CHAR *pszFileName, const HI_FORMAT_PROTOCOL_FUN_S *pstProtocol, HI_VOID **pFmtHandle);

    /**
    \brief Look up the streams in the file. CNcomment:查找文件中的流信息CNend
    \attention \n
    the function must return success if do nothing.  its calling process will be:
    1. Open a file by calling the fmt_open function.
    2. Look up the streams information by calling the fmt_find_stream.
    3. The file can be opened successfully only when the functions return HI_SUCCESS.
    CNcomment:该接口如果不实现，函数返回必须为HI_SUCCESS，HiPlayer调用流程如下:
    1. 先调用fmt_open函数打开一个文件句柄
    2. 再调用fmt_find_stream查找文件流信息
    3. 只有fmt_open,fmt_find_stream函数都返回HI_SUCCESS，才认为文件打开成功CNend

    \param [in] fmtHandle file handle returned by the functions. CNcomment:fmt_open函数返回的文件句柄CNend
    \param [in] pArg expand parameter. CNcomment:扩展参数CNend

    \retval ::HI_SUCCESS get the valid stream information successfully. CNcomment:文件中有有效的音视频或字幕流信息CNend
    \retval ::HI_FAILURE no stream in the file. The file cannot be played. CNcomment:文件中没有任何流信息，该文件不可播放CNend

    \see \n
    None
    */
    HI_S32 (*fmt_find_stream)(HI_VOID *fmtHandle, HI_VOID *pArg);

    /**
    \brief Obtain the file information. CNcomment:获取文件信息CNend
    \attention \n
    None
    \param [in] fmtHandle file handle. CNcomment:文件句柄CNend
    \param [out] ppstFmtInfo file information. CNcomment:文件信息CNend

    \retval ::HI_SUCCESS The operation is successful. CNcomment:操作成功CNend
    \retval ::HI_FAILURE The parameters are invalid. CNcomment:参数非法CNend

    \see \n
    None
    */
    HI_S32 (*fmt_getinfo)(HI_VOID *fmtHandle, HI_FORMAT_FILE_INFO_S **ppstFmtInfo);

    /**
    \brief Read one frame data. The read data must be released by calling the fmt_free interface. CNcomment:读取一帧数据，读取数据必须调用fmt_free释放CNend
    \attention \n
    If the read frame data has no PTS, set the PTS to HI_FORMAT_NO_PTS.  CNcomment:读取的帧数据如果没有时间戳，将时间戳值设置为HI_FORMAT_NO_PTS CNend
    \param [in] fmtHandle file DEMUX handle. CNcomment:文件解析器句柄CNend
    \param [in/out] pstFmtFrame media data. CNcomment:媒体数据信息CNend

    \retval ::HI_SUCCESS The operation is successful. CNcomment:操作成功CNend
    \retval ::HI_FAILURE The parameters are invalid. CNcomment:参数非法CNend
    \retval ::HI_FORMAT_ERRO_ENDOFFILE The file ends. CNcomment:文件结束CNend

    \see \n
    None
    */
    HI_S32 (*fmt_read)(HI_VOID *fmtHandle, HI_FORMAT_FRAME_S *pstFmtFrame);

    /**
    \brief Release frame data. CNcomment:释放帧数据CNend
    \attention \n
    None
    \param [in] fmtHandle file DEMUX handle. CNcomment:文件解析器句柄CNend
    \param [in] pstFmtFrame media data read by the fmt_read interface. CNcomment:fmt_read接口读取的媒体数据信息CNend

    \retval ::HI_SUCCESS The operation is successful. CNcomment:操作成功CNend
    \retval ::HI_FAILURE The parameters are invalid. CNcomment:参数非法CNend

    \see \n
    None
    */
    HI_S32 (*fmt_free)(HI_VOID *fmtHandle, HI_FORMAT_FRAME_S *pstFmtFrame);

    /**
    \brief Invoke the interface with the DEMUX and protocol. CNcomment:与解析器、协议交互操作接口CNend
    \attention \n
    None
    \param [in] fmtHandle handle of the DEMUX. CNcomment:文件解析器句柄CNend
    \param [in] u32InvokeId command of invoking. The value is HI_FORMAT_INVOKE_ID_E. CNcomment:交互命令，值为HI_FORMAT_INVOKE_ID_E CNend
    \param [in/out] pArg the parameter corresponding to the command. CNcomment:命令对应的参数CNend

    \retval ::HI_SUCCESS The invoke operation is successful. CNcomment:操作成功CNend
    \retval ::HI_FAILURE The parameter is invalid. CNcomment:参数非法CNend

    \see \n
    None
    */
    HI_S32 (*fmt_invoke)(HI_VOID *fmtHandle, HI_U32 u32InvokeId, HI_VOID *pArg);

    /**
    \brief Seek frame data in a file based on the entered PTS. Seek frame data in a sequence from the current location to the file header if s64pts is set to 0.
           CNcomment:seek到指定pts，s64pts为0表示seek到文件头CNend
    \attention \n
    None
    \param [in] fmtHandle file DEMUX handle. CNcomment:文件解析器句柄CNend
    \param [in] s32StreamIndex stream index. CNcomment:流索引CNend
    \param [in] s64Pts PTS, in the unit of ms. CNcomment:时间戳，单位ms CNend
    \param [in] eFlag seeking flag. CNcomment:seek标记CNend

    \retval ::HI_SUCCESS The operation is successful. CNcomment:操作成功CNend
    \retval ::HI_FAILURE The seeking operation is not supported. CNcomment:不支持seek操作CNend
    \retval ::HI_FORMAT_ERRO_ENDOFFILE end of file. CNcomment:文件结束CNend

    \see \n
    None
    */
    HI_S32 (*fmt_seek_pts)(HI_VOID *fmtHandle, HI_S32 s32StreamIndex, HI_S64 s64Pts, HI_FORMAT_SEEK_FLAG_E eFlag);

    /**
    \brief Seek frame data in a file by deviating s64Offsets from the current location if s32Whence is set to SEEK_CUR, by deviating s64Offsets from the file header if
s32Whence is set to SEEK_SET, and by deviating s64Offsets from the end of the file if s32Whence is set to SEEK_END. Jumping can be implemented by using this interface for pure audio or video files.
CNcomment:seek到指定位字节数的位置，对于裸码流文件，可以通过该接口实现跳跃CNend
    \attention \n
    None
    \param [in] fmtHandle file DEMUX handle. CNcomment:文件解析器句柄CNend
    \param [in] s64Offsets number of sought bytes. CNcomment:seek字节数CNend
    \param [in] s32Whence seek flag, such as SEEK_CUR. CNcomment:seek标示，如SEEK_CUR CNend

    \retval ::Value other than HI_FAILURE: It indicates the number of bytes deviated by the file pointer after seeking. CNcomment:非HI_FAILURE seek后，文件指针偏移字节数CNend
    \retval ::HI_FAILURE The seeking operation is not supported. CNcomment:不支持seek操作CNend
    \retval ::HI_FORMAT_ERRO_ENDOFFILE end of file. CNcomment:文件结束CNend

    \see \n
    None
    */
    HI_S64 (*fmt_seek_pos)(HI_VOID *fmtHandle, HI_S64 s64Offsets, HI_S32 s32Whence);

    /**
    \brief Disable the file DEMUX. CNcomment:关闭文件解析器CNend
    \attention \n
    None
    \param [in] fmtHandle file DEMUX handle. CNcomment:文件解析器句柄CNend

    \retval ::HI_SUCCESS The operation is successful. CNcomment:操作成功CNend
    \retval ::HI_FAILURE The parameter is invalid. CNcomment:参数非法CNend

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

/** CNcomment:文件解析组件、协议组件参数，文件解析器、协议组件必须实现
  * 结构体中定义的函数，并指定解析器名称，
  * 支持的文件格式，版本号等属性，通过dlsym
  * 系统函数能导出文件解析器中定义的该结
  * 构体全局符号，外部不能修改该符号的属性值，
  * 如下示例: 解析器中定义如下变量，
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
  * APP加载方式如下:
  *
  * HI_FORMAT_S *pstEntry = NULL;
  * pstEntry = dlsym(pDllModule, "g_stFormat_entry");
  *
  * 调用方式:
  * pstEntry->fmt_open();
  *
  */

/** File DEMUX attributes */
/** CNcomment:文件解析器属性 */
typedef struct hiFORMAT_S
{
    /**< DEMUX name */
    /**< CNcomment:解析器名称 */
    const HI_CHAR *pszDllName;

    /**< File formats supported by the DEMUX. The file formats are separated by commas (,). */
    /**< CNcomment:解析器支持的文件格式，多种文件格式以","为间隔 */
    const HI_CHAR *pszFormatName;

    /**< Supported protocols. The protocols are separated by commas (,). */
    /**< CNcomment:支持的协议，多种协议以","为间隔 */
    const HI_CHAR *pszProtocolName;

    /**< DEMUX version */
    /**< CNcomment:解析器版本号 */
    const HI_FORMAT_LIB_VERSION_S stLibVersion;

    /**< DEMUX description */
    /**< CNcomment:解析器描述 */
    const HI_CHAR *pszFmtDesc;

    /**< Priority. The value ranges from 0x0 to 0xFFFFFFFF. 0xFFFFFFFF indicates the highest priority.*/
    /**< CNcomment:优先级，从0x0 - 0xFFFFFFFF，0xFFFFFFFF优先级最高 */
    HI_U32 u32Merit;

   /**< Access interface of the file DEMUX. The implementation of the file DEMUX must comply with the following rules:
          1. For operations, such as opening, reading, and closing a file (fopen or fclose on local files).
             the access interface of the file DEMUX can be specified externally and can be adapted by using the following URL operation interface. */
    /**< CNcomment:文件解析器访问接口，文件解析器实现必须遵循如下规则:
         1、对于文件打开、读取、关闭等操作，如对于本地文件的fopen/fclose等，
            可以由外部指定，且可以由下方的url操作接口适配 */

    HI_FORMAT_DEMUXER_FUN_S stDemuxerFun;

    /**< Protocol access interface. The interface complies with the following rules:
         1. This parameter must be set to NULL if the file DEMUX does not implement the protocol access interface.
         2. This parameter can be specified externally. That is, the file DEMUX does not need to implement this interface.
            An interface specified externally is used for file operations during parsing. */
    /**< CNcomment:协议访问接口，该接口遵循以下规则:
         1、如果文件解析器没有实现该接口，必须将该参数设置为NULL;
         2、该参数可以在外部指定，即: 文件解析器可以不实现该接口，
            在解析过程对文件的操作使用外部指定接口; */

    HI_FORMAT_PROTOCOL_FUN_S stProtocolFun;

    struct hiFORMAT_S *next;

    /**< DLL handle of the DEMUX */
    /**< CNcomment:解析器动态库句柄 */
    HI_VOID  *pDllModule;

    /**< Private data */
    /**< CNcomment:私有数据 */
    HI_U32 u32PrivateData;
} HI_FORMAT_S;
/** @} */  /** <!-- ==== Structure Definition end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_SVR_FORMAT_H__ */
