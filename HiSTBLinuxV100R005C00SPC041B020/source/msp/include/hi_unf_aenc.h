/******************************************************************************
  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
  File Name     : hi_unf_aenc.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 
  Last Modified :
  Description   : header file for audio and video output control
  Function List :
  History       :
  1.Date        :
  Author        : 
  Modification  : Created file
******************************************************************************/

/** 
 * \file
 * \brief describle the information about AENC. CNcomment:提供AENC的相关信息 CNend
 */
 
#ifndef  __HI_UNF_AENC_H__
#define  __HI_UNF_AENC_H__

#include "hi_unf_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/******************************* API Declaration *****************************/
/** \addtogroup      AENC */
/** @{ */  /** <!-- [AENC] */

/**
\brief Initializes audio encoder. CNcomment:初始化音频编码器 CNend
\attention \n
Call the interface before calling HI_UNF_AENC_Open. CNcomment:HI_UNF_AENC_Open接口前调用 CNend
\param N/A
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_Init(HI_VOID);

/**
\brief Deinitializes audio encoder. CNcomment:去初始化音频编码器 CNend
\attention \n
N/A
\param N/A
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT  Invalid operation CNcomment:HI_ERR_AENC_CH_NOT_SUPPORT 无效的操作 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_DeInit(HI_VOID);

/**
\brief Create audio encoder. CNcomment:创建音频编码器 CNend
\attention \n
Before calling the interface, you must configure the encoder. Now AENC only supports 16-bit and 2 channels, and support 16K, 22K, 24K, 32K, 44K, or 48Ksample rate.   
CNcomment:调用该接口前，必须配置编码器，目前AENC只支持16位2声道，且采样率为：16K 22K 24K 32K 44K和48K CNend
\param[in] pstAencAttr               Attribute structure of the audio encoder. CNcomment:音频编码器属性结构 CNend
\param[out] phAenc                   Obtain the handle of the audio encoder. CNcomment:得到音频编码器句柄 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\retval ::HI_ERR_AENC_NULL_PTR        The pointer is null. CNcomment:指针为空 CNend
\retval ::HI_ERR_AENC_CREATECH_FAIL   The encoder fails to be created. CNcomment:创建编码器失败 CNend
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_Create(const HI_UNF_AENC_ATTR_S *pstAencAttr, HI_HANDLE *phAenc);

/**
\brief Destroy the audio encoder. CNcomment:销毁音频编码器 CNend
\attention \n
\param[in] phAenc the handle of the audio encoder CNcomment:音频编码器句柄 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT  The operation is invalid. CNcomment:无效的操作 CNend
\retval ::HI_ERR_AENC_INVALID_PARA    The parameter is invalid. CNcomment:无效的参数 CNend
\retval ::HI_ERR_AENC_CH_NOT_OPEN     The audio encoder is not opened. CNcomment:音频编码器没有打开 CNend
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_Destroy(HI_HANDLE hAenc);

/** 
\brief Copy data to the encoder for encoding. CNcomment:向编码器拷贝数据以供编码 CNend
\attention \n
\param[in] phAenc     The audio encoder handle CNcomment:音频编码器句柄 CNend
\param[in] pstAOFrame Send a frame of data to the encoder. CNcomment:向音频编码器送一帧数据 CNend
\retval ::HI_SUCCESS  Success CNcomment:成功 CNend
\retval ::HI_ERR_AENC_IN_BUF_FULL      The input buffer of the encoder is full. CNcomment:编码器输入缓冲满 CNend
\retval ::HI_ERR_AENC_INVALID_PARA     The parameter is invalid. CNcomment:无效的参数 CNend
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT   The operation is invalid. CNcomment:无效的操作 CNend
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_SendFrame(HI_HANDLE hAenc, const HI_UNF_AO_FRAMEINFO_S *pstAOFrame);

/**
\brief Obtain data from a output buffer of the encoder and write the data into the file. CNcomment:从编码器的一个输出缓冲中取出数据以供写入文件 CNend
\attention \n
\param[in] phAenc     The audio encoder handle CNcomment:音频编码器句柄 CNend
\param[in] pstStream  The data in a output buffer of the audio encoder CNcomment:音频编码器一个输出缓冲中的数据 CNend
\retval ::HI_SUCCESS  Success CNcomment:成功 CNend
\retval ::HI_ERR_AENC_INVALID_PARA    The parameter is invalid. CNcomment:无效的参数 CNend
\retval ::HI_ERR_AENC_OUT_BUF_EMPTY   The output buffer of the encoder is empty. CNcomment:编码器输出缓冲为空 CNend
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT  The operation is invalid. CNcomment:无效的操作 CNend
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_AcquireStream(HI_HANDLE hAenc, HI_UNF_ES_BUF_S *pstStream, HI_U32 u32TimeoutMs);

/**
\brief Destroy the data in a output buffer of the encoder. CNcomment:销毁编码器一个输出缓冲中的数据 CNend
\attention \n
Call the interface after calling HI_UNF_AENC_ReceiveStream. CNcomment:在HI_UNF_AENC_ReceiveStream之后调用 CNend
\param[in] phAenc     The handle of audio encoder CNcomment:音频编码器句柄 CNend
\param[in] u32TimeoutMs   allowable timeout  CNcomment:获取编码数据允许的超时 CNend
\param[out] pstStream  The data in a output buffer of the audio encoder CNcomment:音频编码器一个输出缓冲中的数据 CNend
\retval ::HI_SUCCESS  Success CNcomment:成功 CNend
\retval ::HI_ERR_AENC_INVALID_PARA   The parameter is invalid. CNcomment:无效的参数 CNend
\retval ::HI_ERR_AENC_OUT_BUF_BAD    The parameter is invalid. CNcomment:无效的参数 CNend
\retval ::HI_ERR_AENC_IN_BUF_UNEMPTY The input buffer is empty. CNcomment:输入缓冲非空 CNend
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT The operation is invalid. CNcomment:无效的操作 CNend
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_ReleaseStream(HI_HANDLE hAenc, const HI_UNF_ES_BUF_S *pstStream);


/**
\brief Register dynamic audio encoding libraries. CNcomment:注册音频动态编码库 CNend
\attention \n
\param[in] pEncoderDllName The file name of audio encoding  libraries CNcomment:pEncoderDllName 音频编码库文件名 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_FAILURE Failure CNcomment:失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AENC_RegisterEncoder(const HI_CHAR *pEncoderDllName);

/**
\brief Attach audio encoder with audio track. CNcomment:绑定音频编码器和音频Track 通路 CNend
\attention \n
Before obtaining the streams and encoding, you must call the interface. CNcomment:开始编码和获取码流之前需要首先调用该接口 CNend
\param[in] hSrc   The handle of audio track CNcomment:资源句柄 CNend
\param[in] hAenc     The handle of audio encoder CNcomment:音频编码器句柄 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AENC_INVALID_PARA	        The parameter is invalid. CNcomment:无效的参数 CNend
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT        The operation is invalid. CNcomment:无效的操作 CNend
\retval ::HI_ERR_AENC_NULL_PTR	            The pointer is null. CNcomment:指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT	        The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AENC_AttachInput(HI_HANDLE hAenc, HI_HANDLE hSrc);

/**
\brief Detach audio encoder with audio track. CNcomment:解除音频编码器和音频Track 通路的绑定 CNend
\attention \n
Before calling the interface, stop the encoding. CNcomment:调用该接口需要首先停止编码 CNend
\param[in] hAenc     The handle of audio encoder CNcomment:音频编码器句柄 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AENC_INVALID_PARA	        The parameter is invalid. CNcomment:无效的参数 CNend
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT        The operation is invalid. CNcomment:无效的操作 CNend
\retval ::HI_ERR_AENC_NULL_PTR	            The pointer is null. CNcomment:指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT	        The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AENC_DetachInput(HI_HANDLE hAenc);

/**
\brief Start to encode. CNcomment:开始编码 CNend
\attention \n
Before calling the interface, you need to initialize the encoder, create an encoding channel and attach the player.
CNcomment:调用该接口需要首先初始化编码器，创建编码通道，绑定播放器 CNend
\param[in] hAenc The handle of audio encoder CNcomment:音频编码器句柄 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AENC_INVALID_PARA	        The parameter is invalid. CNcomment:无效的参数 CNend
\retval ::HI_ERR_AENC_NULL_PTR	            The pointer is null. CNcomment:指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT	        The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AENC_Start(HI_HANDLE hAenc);


/**
\brief Stop to encode. CNcomment:停止编码 CNend
\attention \n
Before calling the interface, you need to initialize the encoder, create an encoding channel, attach the player, and start the encoding.
CNcomment:调用该接口需要首先初始化编码器，创建编码通道，绑定播放器，已经开始编码 CNend
\param[in] hAenc   The handle of audio encoder CNcomment:音频编码器句柄 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AENC_INVALID_PARA	        The parameter is invalid. CNcomment:无效的参数 CNend
\retval ::HI_ERR_AENC_NULL_PTR	            The pointer is null. CNcomment:指针为空 CNend
\retval ::HI_ERR_AVPLAY_INVALID_OPT	        The operation is invalid. CNcomment:操作非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AENC_Stop(HI_HANDLE hAenc);

/**
\brief Set the attribution of encode. CNcomment:设置编码属性 CNend
\attention \n
Before calling the interface, you need to stop the encoder.
CNcomment:调用该接口需要首先停止编码CNend
\param[in] hAenc   The handle of audio encoder CNcomment:音频编码器句柄 CNend
\param[in] pstAencAttr   The attribution of audio encoder CNcomment:音频编码属性 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AENC_INVALID_PARA        The parameter is invalid. CNcomment:无效的参数 CNend
\retval ::HI_ERR_AENC_NULL_PTR            The pointer is null. CNcomment:指针为空 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AENC_SetAttr(HI_HANDLE hAenc, const HI_UNF_AENC_ATTR_S *pstAencAttr);

/**
\brief Set the attribution of encode. CNcomment:获取编码属性 CNend
\attention \n
\param[in] hAenc   The handle of audio encoder CNcomment:音频编码器句柄 CNend
\param[out] pstAencAttr   The attribution of audio encoder CNcomment:音频编码属性 CNend
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\retval ::HI_ERR_AENC_INVALID_PARA        The parameter is invalid. CNcomment:无效的参数 CNend
\retval ::HI_ERR_AENC_NULL_PTR            The pointer is null. CNcomment:指针为空 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AENC_GetAttr(HI_HANDLE hAenc, HI_UNF_AENC_ATTR_S *pstAencAttr);


/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  /*__HI_UNF_AENC_H__*/

