/**
 \file
 \brief common module
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author
 \date 2011-10-9
 */

#ifndef __HI_SVR_CHARSET_NORM_H__
#define __HI_SVR_CHARSET_NORM_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


/***************************** Structs Definition ******************************/
/** \addtogroup     Hiplayer */
/** @{ */  /** <!--[Hiplayer] */

/** Codec type of charset */
/**
CNcomment:字符集编码类型,
添加字符集，遵循以下约定:
1、添加字符集后，需要在字符集编码类型后添加上编码类型
2、添加的编码类型编号是连续
3、添加的编码类型基于前面的编码类型编号+1
*/
typedef enum hiCHARSET_CODETYPE_E {
    HI_CHARSET_CODETYPE_NONE = 0,        /**< Default value *//**< CNcomment:默认值，不设置 */
    HI_CHARSET_CODETYPE_BIG5 = 1,        /**< big5 *//**< CNcomment:繁体中文 */
    HI_CHARSET_CODETYPE_UTF8 = 2,        /**< uft8 *//**< UTF8 */
    HI_CHARSET_CODETYPE_ISO8859_1 = 3,   /**< iso8850_1 *//**< CNcomment:西欧语系 ，拉丁文1 */
    HI_CHARSET_CODETYPE_ISO8859_2 = 4,   /**< iso8850_2 *//**< CNcomment:中欧和东欧语系，拉丁文2 */
    HI_CHARSET_CODETYPE_ISO8859_3 = 5,   /**< iso8850_3 *//**< CNcomment:欧洲东南部语系，拉丁文3 */
    HI_CHARSET_CODETYPE_ISO8859_4 = 6,   /**< iso8850_4 *//**< CNcomment:斯堪的那维亚和波罗的海语系，拉丁文4 */
    HI_CHARSET_CODETYPE_ISO8859_5 = 7,   /**< iso8850_5 *//**< CNcomment:拉丁文和斯拉夫文 */
    HI_CHARSET_CODETYPE_ISO8859_6 = 8,   /**< iso8850_6 *//**< CNcomment:拉丁文和阿拉伯文 */
    HI_CHARSET_CODETYPE_ISO8859_7 = 9,   /**< iso8850_7 *//**< CNcomment:拉丁文和希腊文 */
    HI_CHARSET_CODETYPE_ISO8859_8 =10,   /**< iso8850_8 *//**< CNcomment:拉丁文和希伯来文 */
    HI_CHARSET_CODETYPE_ISO8859_9 =11,   /**< iso8850_9 *//**< CNcomment:为土耳其文修正的拉丁文，拉丁文5 */
    HI_CHARSET_CODETYPE_ISO8859_10=12,   /**< iso8850_10 *//**< CNcomment:拉普人、北欧和爱斯基摩人的文字，拉丁文6 */
    HI_CHARSET_CODETYPE_ISO8859_11=13,   /**< iso8850_11 *//**< CNcomment:拉丁文与泰文 */
    HI_CHARSET_CODETYPE_ISO8859_13=14,   /**< iso8850_13 *//**< CNcomment:波罗的海周边语系，拉丁文7 */
    HI_CHARSET_CODETYPE_ISO8859_14=15,   /**< iso8850_14 *//**< CNcomment:凯尔特文，拉丁文8 */
    HI_CHARSET_CODETYPE_ISO8859_15=16,   /**< iso8850_15 *//**< CNcomment:改进的拉丁文1，拉丁文9 */
    HI_CHARSET_CODETYPE_ISO8859_16=17,   /**< iso8850_16 *//**< CNcomment:罗马尼亚文，拉丁文10 */
    HI_CHARSET_CODETYPE_UNICODE_16LE=18, /**< unicode_16 little-endian *//**< CNcomment:16位小端的UNICODE */
    HI_CHARSET_CODETYPE_UNICODE_16BE=19, /**< unicode_16 big-endian *//**< CNcomment:16位大端的UNICODE */
    HI_CHARSET_CODETYPE_GBK=20,          /**< gbk *//**< CNcomment:简体中文GBK */
    HI_CHARSET_CODETYPE_CP1250 = 21,     /**< cp1250 *//**< CNcomment:中欧 */
    HI_CHARSET_CODETYPE_CP1251 = 22,     /**< cp1251 *//**< CNcomment:西里尔文 */
    HI_CHARSET_CODETYPE_CP1252 = 23,     /**< cp1252 *//**< CNcomment:拉丁文I */
    HI_CHARSET_CODETYPE_CP1253 = 24,     /**< cp1253 *//**< CNcomment:希腊文 */
    HI_CHARSET_CODETYPE_CP1254 = 25,     /**< cp1254 *//**< CNcomment:土耳其文 */
    HI_CHARSET_CODETYPE_CP1255 = 26,     /**< cp1255 *//**< CNcomment:希伯来文 */
    HI_CHARSET_CODETYPE_CP1256 = 27,     /**< cp1256 *//**< CNcomment:阿拉伯文 */
    HI_CHARSET_CODETYPE_CP1257 = 28,     /**< cp1257 *//**< CNcomment:波罗的海文 */
    HI_CHARSET_CODETYPE_CP1258 = 29,     /**< cp1258 *//**< CNcomment:越南 */
    HI_CHARSET_CODETYPE_CP874  = 30,     /**< cp874 *//**< CNcomment:泰文 */
    HI_CHARSET_CODETYPE_UNICODE_32LE=31, /**< unicode_32 little-endian *//**< CNcomment:32位小端的UNICODE */
    HI_CHARSET_CODETYPE_UNICODE_32BE=32, /**< unicode_32 big-endian *//**< CNcomment:32位大端的UNICODE */
    HI_CHARSET_CODETYPE_CP949  = 33,     /**< cp874 *//**< CNcomment:韩文，CP949字符集 */
    HI_CHARSET_CODETYPE_BULT,
} HI_CHARSET_CODETYPE_E;

/** Verion of charset library */
/** CNcomment:版本号定义 */
typedef struct hiCHARSET_LIB_VERSION_S
{
    HI_U8 u8VersionMajor;    /**< Major version accessor element */
    HI_U8 u8VersionMinor;    /**< Minor version accessor element */
    HI_U8 u8Revision;        /**< Revision version accessor element */
    HI_U8 u8Step;            /**< Step version accessor element */
} HI_CHARSET_LIB_VERSION_S;

/** Stream detector API  */
/** CNcomment:码流探测器API */
typedef struct hiCHARSET_DETECTOR_FUN_S
{
    /**
    \brief Create detector according to user data. CNcomment:根据私有数据，创建探测器CNend
    \attention \n
    none
    \param [out] pDetectHandle  Handle of detector
    \param [in] u32UserData  User data

    \retval ::HI_SUCCESS Create successful.
    \retval ::HI_FAILURE Create fail.

    \see \n
    none
    */
    HI_S32 (*detect_create)(HI_VOID **pDetectHandle, HI_U32 u32UserData);

    /**
    \brief According to the data stream, detecting a coding type.CNcomment:根据数据流，探测编码类型CNend
    \attention \n
    none
    \param [in]  handle Handle of detector
    \param [in]  u8Input Input data
    \param [in]  u32Inlen Length of input data
    \param [in]  s32UserData User data
    \param [out] s32CodeType Code type of input data

    \retval ::HI_SUCCESS  Detect successful.
    \retval ::HI_FAILURE  Detect fail, value of s32CodeType is invalid.

    \see \n
    none
    */
    HI_S32 (*identstream)(HI_VOID *handle, const HI_U8 *u8Input, const HI_S32 s32Inlen, HI_S32 s32UserData, HI_S32 *s32CodeType);

    /**
    \brief Set user command and user data.CNcomment:异步设置用户命令和数据CNend
    \attention \n
    none
    \param [in] handle Handle of detector
    \param [in] u32Cmd  User command
    \param [in] pArg  User data

    \retval ::HI_SUCCESS Set successful.
    \retval ::HI_FAILURE Parameter invalid.

    \see \n
    none
    */
    HI_S32 (*detect_setDataAsync)(HI_VOID *handle, HI_U32 u32Cmd, HI_VOID *pArg);

    /**
    \brief Destroy detector. CNcomment:关闭探测器CNend
    \attention \n
    none
    \param [in] pDetectHandle Handle of detector

    \retval ::HI_SUCCESS Destroy successful.
    \retval ::HI_FAILURE Handle invalid.

    \see \n
    none
    */
    HI_S32 (*detect_close)(HI_VOID *pDetectHandle);
} HI_CHARSET_DETECTOR_FUN_S;

/** Stream transcoder API  */
/** CNcomment:转码器API */
typedef struct hiCHARSET_CONVERTER_FUN_S
{
    /**
    \brief Find a transcoder, according to the source data encoding type and target data encoding type.
    CNcomment:根据转码源编码类型和目的编码类型，查找识别器CNend
    \attention \n
    none
    \param [in] s32SrcType  Source data encoding type
    \param [in] s32DstType  Target data encoding type

    \retval ::HI_SUCCESS Find successfule.
    \retval ::HI_FAILURE Not found.

    \see \n
    none
    */
    HI_S32 (*conv_find)(HI_S32 s32SrcType, HI_S32 s32DstType);

    /**
    \brief Create a transcoder.CNcomment:创建转码器CNend
    \attention \n
    none
    \param [out] pConvHandle  Handle of the transcoder
    \param [in] u32UserData  User data

    \retval ::HI_SUCCESS Create successful.
    \retval ::HI_FAILURE Create fail.

    \see \n
    none
    */
    HI_S32 (*conv_create)(HI_VOID **pConvHandle, HI_U32 u32UserData);

    /**
    \brief Transcode data stream.CNcomment:根据数据流进行转码CNend
    \attention \n
    none
    \param [in]  pConvHandle Handle of transcoder
    \param [in]  u8Input Input data
    \param [in]  u32Inlen Length of input data
    \param [in] s32SrcType  Source data encoding type
    \param [in] s32DstType  Target data encoding type
    \param [out] u8Output Transcoded data
    \param [out] u32Outlen Length of transcoded data
    \param [out] s32Usedlen Used length

    \retval ::HI_SUCCESS  Transcode successful.
    \retval ::HI_FAILURE  Transcode fail.

    \see \n
    none
    */
    HI_S32 (*convstream)(HI_VOID *pConvHandle, const HI_U8 *u8Input, const HI_S32 s32Inlen, HI_U8 *u8Output, HI_S32 *s32Outlen, HI_S32 s32SrcType, HI_S32 s32DstType, HI_S32 *s32Usedlen);

    /**
    \brief Set user command and user data.CNcomment:异步设置用户命令和数据CNend
    \attention \n
    none
    \param [in] handle Handle of transcoder
    \param [in] u32Cmd  User command
    \param [in] pArg  User data

    \retval ::HI_SUCCESS Set successful.
    \retval ::HI_FAILURE Parameter invalid.

    \see \n
    none
    */
    HI_S32 (*conv_setDataAsync)(HI_VOID *handle,HI_U32 u32Cmd, HI_VOID *pArg);

    /**
    \brief Destroy transcoder.CNcomment:关闭转码器CNend
    \attention \n
    none
    \param [in] pConvHandle Handle of transcoder

    \retval ::HI_SUCCESS Destroy successfule.
    \retval ::HI_FAILURE Invalid handle.

    \see \n
    none
    */
    HI_S32 (*conv_close)(HI_VOID *pConvHandle);
} HI_CHARSET_CONVERTER_FUN_S;

/*******************************************************
HI_CHARSET_S g_stCharset_entry =
{
    .pszDllName  = (const HI_PCHAR )"libhicharset.so",
    .stLibVersion = {1, 0, 0, 0},
    .pszCharSetDesc = (const HI_PCHAR )"hicharset detector and converter",
    .u32Priority   = 0,

    .stDetectorFun.detect_create = ,
    .stDetectorFun.identstream   = ,
    .stDetectorFun.detect_setDataAsync = ,
    .stDetectorFun.detect_close  = ,

    .stConverterFun.conv_find   = ,
    .stConverterFun.conv_create = ,
    .stConverterFun.convstream  = ,
    .stConverterFun.conv_setDataAsync = ,
    .stConverterFun.conv_close = ,

    .next           = NULL,
    .pDllModule     = NULL,
    .u32PrivateData = 0,
};
********************************************************/

typedef struct hiCHARSET_S
{
    /**< Charset dll library name *//**< CNcomment:字符集名称 */
    const HI_CHAR *pszDllName;

    /**< Version of charset library *//**< CNcomment:字符集版本号 */
    const HI_CHARSET_LIB_VERSION_S stLibVersion;

    /**< Description of the charset library. *//**< CNcomment:字符集描述 */
    const HI_CHAR *pszCharSetDesc;

    /**< Priority of the charset library *//**< CNcomment:优先级，从0x0 - 0xFFFFFFFF，0xFFFFFFFF优先级最高，该属性无效 */
    HI_U32 u32Priority;

    /**< Interface of the detector */
    /**< CNcomment:字符集探测器访问接口，探测器实现必须遵循如下规则:
         1、对于探测器，探测精度也高，优先级越高，如果不确认编码类型，则失败返回
         2、如果字符集没有实现探测器功能，必须将该参数设置为NULL;
         */

    HI_CHARSET_DETECTOR_FUN_S stDetectorFun;

    /**< Interface of the detector */
    /**< CNcomment:转码器接口，该接口遵循以下规则:
         1、如果字符集没有实现该接口，必须将该参数设置为NULL;
         2、该参数可以在外部指定，即: 字符集可以不实现该接口，
         */

    HI_CHARSET_CONVERTER_FUN_S stConverterFun;

    struct hiCHARSET_S *next;

    /**< Handle of the dll library */
    /**< CNcomment:动态库句柄 */
    HI_VOID  *pDllModule;

    /**< User data */
    /**< CNcomment:私有数据 */
    HI_U32 u32PrivateData;
} HI_CHARSET_S;

/** Get the minimum of the character encoding value. */
/** CNcomment:获取字符编码最小值 */
extern HI_S32 HI_CHARSET_GetMinCodetype();

/** Get max of the character encoding value . */
/** CNcomment:获取字符编码最大值 */
extern HI_S32 HI_CHARSET_GetMaxCodetype();

/** @} */  /** <!-- ==== Structure Definition end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_SVR_CHARSET_NORM_H__ */
