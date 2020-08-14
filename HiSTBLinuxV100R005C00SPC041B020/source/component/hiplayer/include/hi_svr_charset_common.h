/**
 \file
 \brief common module
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author
 \date 2011-10-9
 */

#ifndef __HI_SVR_CHARSET_COMMON_H__
#define __HI_SVR_CHARSET_COMMON_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Structs Definition ******************************/
/** \addtogroup     Hiplayer */
/** @{ */  /** <!--[Hiplayer] */

/** Type of charset stream */
/** CNcomment:数据流类型 */
typedef enum hiCHARSET_STREAMTYPE_E {
    HI_CHARSET_STREAMTYPE_UNKNOW = 0,    /**< Unsupport type *//**< CNcomment:未知数据流 */
    HI_CHARSET_STREAMTYPE_BITS8  = 1,    /**< Single-byte data stream  *//**< CNcomment:单字节数据流 */
    HI_CHARSET_STREAMTYPE_BITS16LE=2,    /**< Double-byte little-endian data stream *//**< CNcomment:双字节小端数据流 */
    HI_CHARSET_STREAMTYPE_BITS16BE=3,    /**< Double-byte big-endian data stream*//**< CNcomment:双字节大端数据流 */
    HI_CHARSET_STREAMTYPE_BITS32LE=4,    /**< Four-byte little-endian data stream *//**< CNcomment:四字节小端数据流 */
    HI_CHARSET_STREAMTYPE_BITS32BE=5,    /**< Four-byte big-endian data stream *//**< CNcomment:四字节大端数据流 */
    HI_CHARSET_STREAMTYPE_BULT,
} HI_CHARSET_STREAMTYPE_E;

/** Type of user command */
/** CNcomment:用户调用异步设置命令类型 */
typedef enum hiCHARSET_CMD_E {
    HI_CHARSET_CMD_NONE = 0,   /**< Unknow command *//**< CNcomment:未知命令 */
    HI_CHARSET_CMD_EXIT,       /**< Quick exit command *//**< CNcomment:快速退出命令 */
    HI_CHARSET_CMD_USERDATA,   /**< Command of setting user data, param is::HI_U32 *//**< CNcomment:设置用户数据命令，参数::HI_U32 */
    HI_CHARSET_CMD_BULT,
}HI_CHARSET_CMD_E;

typedef struct hiCHARSET_FUN_S {
    /**
    \brief Create charset handle. CNcomment:创建字符集模块句柄CNend
    \attention \n
    none
    \param [in] u32Userdata User data
    \param [out] handle Handle of charset

    \retval ::HI_SUCCESS Create successful.
    \retval ::HI_FAILURE Create fail.

    \see \n
    none
    */
    HI_S32 (*Create)(HI_VOID **handle, HI_U32 u32Userdata);
    /**
    \brief Identify the type of data, create recognizer and identify data, then destroy the recognizer.
    CNcomment:识别数据流类型，创建识别器并且识别数据，完成后销毁识别器CNend
    \attention \n
    none
    \param [in] handle Handle of charset
    \param [in] input Input data
    \param [in] inLen  Size of data
    \param [in] s32UserData User data
    \param [out] codeType  Codec type of the data

    \retval ::HI_SUCCESS Identify the data success.
    \retval ::HI_FAILURE Parameter is invalid.

    \see \n
    none
    */
    HI_S32 (*IdentStream)(HI_VOID *handle, const HI_U8 *input, const HI_S32 inLen, HI_S32 s32UserData, HI_S32 *codeType);
    /**
    \brief Create a transcoder, transcode the input data.
    CNcomment:数据流转码，创建转码器，对数据进行转码，如果发现数据类型不对，销毁转码器，重新创建CNend
    \attention \n
    none
    \param [in] handle Handle of charset
    \param [in] DstType  Target transcoding type
    \param [in] input  Input data stream
    \param [in] inLen  The length of the input data stream
    \param [in] SrcType  The type of source coding
    \param [out] out  Transcoded data stream
    \param [out] outlen  The length of the transcoded data stream
    \param [out] Usedlen  The length of the stream of the data has been transcoded

    \retval ::HI_SUCCESS Transcode successful.
    \retval ::HI_FAILURE Parameter is invalid.

    \see \n
    none
    */
    HI_S32 (*ConvStream)(HI_VOID *handle, HI_U8 *out, HI_S32 *outlen,
        const HI_U8 *input, const HI_S32 inLen, HI_S32 SrcType, HI_S32 DstType, HI_S32* Usedlen);
    /**
    \brief Set user command or user data. CNcomment:异步设置用户命令和数据CNend
    \attention \n
    none
    \param [in] handle Handle of charset
    \param [in] u32Data  User data

    \retval ::HI_SUCCESS Set successful.
    \retval ::HI_FAILURE Parameter is invalid.

    \see \n
    none
    */
    HI_S32 (*setDataAsync)(HI_VOID *handle,HI_U32 u32Cmd, HI_VOID *pArg);

    /**
    \brief Destroy charset object.CNcomment:销毁字符集模块句柄CNend
    \attention \n
    none
    \param [in] handle Handle of charset

    \retval ::HI_SUCCESS Destroy successful.
    \retval ::HI_FAILURE Invalid handle.

    \see \n
    none
    */
    HI_S32 (*Destroy)(HI_VOID *handle);

    /**
    \brief Determine the type of byte stream. CNcomment:根据字符编码类型判断字节流类型CNend
    \attention \n
    none
    \param [in] codetype Character encoding
    \param [out] eStreamType Data stream type

    \retval ::HI_SUCCESS Operation success.
    \retval ::HI_FAILURE Parameter invalid.

    \see \n
    none
    */

    HI_S32 (*GetStreamType)(HI_S32 codetype, HI_CHARSET_STREAMTYPE_E* eStreamType);
} HI_CHARSET_FUN_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_SVR_CHARSET_COMMON_H__ */
