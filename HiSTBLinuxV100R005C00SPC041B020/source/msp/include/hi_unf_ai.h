/******************************************************************************
  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
  File Name     : hi_unf_ai.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/4/22
  Last Modified :
  Description   : header file for audio and video output control
  Function List :
  History       :
  1.Date        :
  Author        : zgjie
  Modification  : Created file
******************************************************************************/
/**
 * \file
 * \brief Describes the information about the audio input (AI) module. CNcomment:提供AI的相关信息 CNend
 */

#ifndef  __HI_UNF_AI_H__
#define  __HI_UNF_AI_H__

#include "hi_unf_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/********************************Macro Definition********************************/
/** \addtogroup      AI */
/** @{ */  /** <!-- AI */


/** @} */  /** <!-- ==== Macro Definition end ==== */


/*************************** Structure Definition ****************************/
/** \addtogroup      AI */
/** @{ */  /** <!--  【AI】 */




/**Audio inputport defination */
/**CNcomment:音频输入端口定义*/
typedef enum hiUNF_AI_E
{
    HI_UNF_AI_I2S0 = 0,

    HI_UNF_AI_I2S1,

    HI_UNF_AI_ADC0 = 0x10,

    HI_UNF_AI_ADC1,

    HI_UNF_AI_ADC2,

    HI_UNF_AI_ADC3,

    HI_UNF_AI_ADC4,

    HI_UNF_AI_SIF0 = 0x20,

    HI_UNF_AI_HDMI0 = 0x30,

    HI_UNF_AI_HDMI1,

    HI_UNF_AI_HDMI2,

    HI_UNF_AI_HDMI3,

    HI_UNF_AI_BUTT = 0xff,
} HI_UNF_AI_E;

/**Defines internal Audio ADC inputport attribute */
/**CNcomment:定义内置音频ADC输入端口属性*/
typedef struct hiUNF_AI_ADC_ATTR_S
{
    HI_BOOL   bByPass;
} HI_UNF_AI_ADC_ATTR_S;


/**Defines  Audio I2S inputport attribute */
/**CNcomment:定义音频I2S输入端口属性*/
typedef struct hiUNF_AI_I2S_ATTR_S
{
    HI_UNF_I2S_ATTR_S  stAttr;  /**<I2S Attribute*/ /**<CNcomment:I2S属性*/
}  HI_UNF_AI_I2S_ATTR_S;

/**Defines the HDMI RX Audio data format .*/
typedef enum hiHI_UNF_AI_HDMI_FORMAT_E
{
    HI_UNF_AI_HDMI_FORMAT_LPCM = 0,          /* LPCM 2/8 channels,  Audio Sample Packet layout0 or layout1 */ 
    HI_UNF_AI_HDMI_FORMAT_LBR = 1,           /* IEC-61937 DD/DDP/DTS, Audio Sample Packet layout0 */ 
    HI_UNF_AI_HDMI_FORMAT_HBR = 8,           /* IEC-61937 DTSHD/TrueHD, High-Bitrate (HBR) Audio Stream Packet*/ 
    HI_UNF_AI_HDMI_FORMAT_BUTT
} HI_UNF_AI_HDMI_FORMAT_E;


/**Defines  HDMI Audio inputport attribute */
/**CNcomment:定义HDMI音频输入端口属性*/
typedef struct hiUNF_AI_HDMI_ATTR_S
{
    HI_UNF_I2S_CHNUM_E    enChannel;    /**<Channel number*/ /**<CNcomment:通道数*/
    HI_UNF_I2S_BITDEPTH_E enBitDepth;   /**<Bit Depth*/ /**<CNcomment:位宽*/
    HI_UNF_SAMPLE_RATE_E  enSampleRate; /**<Sample Rate*/ /**<CNcomment:采样率*/
    HI_UNF_AI_HDMI_FORMAT_E enHdmiAudioDataFormat;  /**<HDMI audio data format*/ /**<CNcomment:HDMI音频数据格式*/
} HI_UNF_AI_HDMI_ATTR_S;

/**Defines internal SIF(Audio Demodulator) inputport attribute */
/**CNcomment:定义SIF输入端口属性*/
typedef struct hiUNF_AI_SIF_ATTR_S
{
    HI_VOID* pPara;
} HI_UNF_AI_SIF_ATTR_S;

/**Defines  Audio inputport attribute */
/**CNcomment:定义音频输入端口属性*/
typedef struct hiHI_UNF_AI_ATTR_S
{
    HI_UNF_SAMPLE_RATE_E    enSampleRate;          /**<samplerate, default 48000Hz*/ /**<CNcomment:设备采样频率，默认 48000Hz*/
    HI_U32                  u32PcmFrameMaxNum;     /**<Max frame of the PCM data at cast buffer, default 16*/ /**<CNcomment: 最大可缓存帧数，默认 16*/
    HI_U32                  u32PcmSamplesPerFrame; /**<Number of sample of the PCM data, default 960*/ /**<CNcomment: PCM数据采样点数量，默认 960*/
    union
    {
        HI_UNF_AI_ADC_ATTR_S  stAdcAttr;    /**<ADC Attribute*/ /**<CNcomment:ADC属性*/
        HI_UNF_AI_I2S_ATTR_S  stI2sAttr;    /**<I2S Attribute*/ /**<CNcomment:I2S属性*/
        HI_UNF_AI_HDMI_ATTR_S stHDMIAttr;   /**<HDMI Attribute*/ /**<CNcomment:HDMI属性*/
    } unAttr;
} HI_UNF_AI_ATTR_S;

/**Defines AI Delay attribute */
/**CNcomment:定义AI延迟属性*/
typedef struct hiHI_UNF_AI_DELAY_S
{
    HI_U32                  u32DelayMs;    /**<buffer delay compensation(ms), Min Value is 20, Max Value depends on u32PcmFrameMaxNum, u32PcmSamplesPerFrame and enSampleRate in HI_UNF_AI_ATTR_S.
                                                    MaxValue = u32PcmSamplesPerFrame * u32PcmFrameMaxNum * 1000 / enSampleRate. 
                                                    MaxValue is equal to 320ms according to default HI_UNF_AI_ATTR_S*/ 
                                                   /**<CNcomment:缓存中数据量延迟补偿控制(ms), 最小值为20ms，最大值依赖HI_UNF_AI_ATTR_S中的u32PcmFrameMaxNum，u32PcmSamplesPerFrame以及enSampleRate
                                                    计算公式:MaxValue = u32PcmSamplesPerFrame * u32PcmFrameMaxNum * 1000 / enSampleRate，
                                                    若根据默认HI_UNF_AI_ATTR_S，则最大值为320ms */
    HI_BOOL                 bDelayMsAutoHold;  /**<if hold buffer delay compensation time automatically*/ /**<CNcomment:是否由AI_AO通路自动保持缓存数据量稳定在u32CompensationMs*/
} HI_UNF_AI_DELAY_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */


/******************************* API declaration *****************************/
/** \addtogroup      AI */
/** @{ */  /** <!--  【AI】 */

/**
\brief Initializes an AI device. CNcomment:初始化音频输入设备 CNend
\attention \n
Before calling the AI module, you must call this application programming interface (API). CNcomment:调用AI模块要求首先调用该接口 CNend
\param N/A
\retval ::HI_SUCCESS Success CNcomment:成功 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AI_Init(HI_VOID);

/**
\brief Deinitializes an AI device. CNcomment:去初始化音频输入设备 CNend
\attention \n
\param N/A
\retval ::HI_SUCCESS Success CNcomment:成功  CNend
\see \n
N/A
*/
HI_S32 HI_UNF_AI_DeInit(HI_VOID);

/**
 \brief Obtains the default attributes of a AI port. CNcomment: 获取AI端口默认属性 CNend
 \attention \n
 \param[in] pstAttr Pointer to AI attributes CNcomment: AI属性指针 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_AI_NULL_PTR   The pointer is null. CNcomment: 空指针 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_AI_GetDefaultAttr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S *pstAttr);

/**
 \brief Enables a AI port to create a AI handle. CNcomment: 打开AI Port，创建AI句柄 CNend
 \attention \n
Before calling the AI, you must call this API.\n
CNcomment: 调用AI 模块需要首先调用该接口 CNend
 \param[in] pstAttr CNcomment: Pointer to AI attributes AI属性指针 CNend
 \param[in] phandle CNcomment: hAi Pointer to the AI handle AI句柄指针 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_AI_NULL_PTR   The pointer is null. CNcomment: 空指针 CNend
 \retval ::HI_ERR_AI_INVALID_PARA   The attribute parameters are incorrect. CNcomment: 属性参数错误 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_AI_Create(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S *pstAttr, HI_HANDLE *phAI);

/**
 \brief Disable a AI port to destroy the handle. CNcomment: 关闭AI Port，销毁句柄 CNend
 \attention \n
 \param[in] hAi AI handle CNcomment: AI句柄 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_AI_INVALID_PARA  The handle is incorrect. CNcomment: 句柄错误 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_AI_Destroy(HI_HANDLE hAI);

/**
 \brief Set the attributes of a AI port. CNcomment: 设置AI Port属性 CNend
 \attention \n
 \param[in] hAi AI handle CNcomment: AI句柄 CNend
 \param[in] pstAttr  Pointer to AI attributes CNcomment: AI属性指针 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_AI_NULL_PTR   The pointer is null. CNcomment: 空指针 CNend
 \retval ::HI_ERR_AI_INVALID_ID   The handle is invalid. CNcomment: 非法句柄 CNend
 \retval ::HI_ERR_AI_INVALID_PARA   The attribute parameters are incorrect. CNcomment: 属性参数错误 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
 HI_S32 HI_UNF_AI_SetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S *pstAttr);


/**
 \brief Obtains the attributes of a AI port. CNcomment: 获取AI Port属性 CNend
 \attention \n
 \param[in] hAi AI handle CNcomment: AI句柄 CNend
 \param[out] pstAttr Pointer to AI attributes CNcomment: AI属性指针 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_AI_NULL_PTR   The pointer is null. CNcomment: 空指针 CNend
 \retval ::HI_ERR_AI_INVALID_ID   The handle is invalid. CNcomment: 非法句柄 CNend
 \retval ::HI_ERR_AI_INVALID_PARA   The handle is incorrect. CNcomment: 句柄错误 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_AI_GetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S *pstAttr);


/**
\brief enable AI port. CNcomment: 使能AI通道 CNend
\attention \n
\param[in] AI handle CNcomment: AI句柄 CNend
\param[in] bEnable      enable  .CNcomment:使能控制 CNend
\retval ::HI_SUCCESS CNcomment: success.成功 CNend
\retval ::HI_ERR_AI_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AI_INVALID_ID     The handle is invalid. CNcomment: 非法句柄 CNend
\retval ::HI_ERR_AI_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_AI_SetEnable(HI_HANDLE hAI, HI_BOOL bEnable);

/**
\brief Obtains enable/disable status of AI port. CNcomment: 获取AI通道时能状态 CNend
\attention \n
\param[in] AI handle CNcomment: AI句柄 CNend
\param[out] pbEnable enable/disable status AI port. CNcomment:AI通道时能状态 CNend
\retval ::HI_SUCCESS CNcomment: success.成功 CNend
\retval ::HI_ERR_AI_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AI_INVALID_ID     The handle is invalid. CNcomment: 非法句柄 CNend
\retval ::HI_ERR_AI_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_AI_GetEnable(HI_HANDLE hAI, HI_BOOL *pbEnable);

/**
\brief set AI delay compensation. CNcomment: 设置AI缓存延迟补偿 CNend
\attention \n
\param[in] AI handle CNcomment: AI句柄 CNend
\param[in] pstDelay  delay compensation .CNcomment:延迟补偿参数 CNend
\retval ::HI_SUCCESS CNcomment: success.成功 CNend
\retval ::HI_ERR_AI_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AI_INVALID_ID     The handle is invalid. CNcomment: 非法句柄 CNend
\retval ::HI_ERR_AI_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_AI_SetDelay(HI_HANDLE hAI, const HI_UNF_AI_DELAY_S *pstDelay);

/**
\brief Obtains AI delay compensation. CNcomment: 获取AI缓存延迟补偿 CNend
\attention \n
\param[in] AI handle CNcomment: AI句柄 CNend
\param[out] pstDelay delay compensation. CNcomment:延迟补偿参数 CNend
\retval ::HI_SUCCESS CNcomment: success.成功 CNend
\retval ::HI_ERR_AI_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AI_INVALID_ID     The handle is invalid. CNcomment: 非法句柄 CNend
\retval ::HI_ERR_AI_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_AI_GetDelay(HI_HANDLE hAI, HI_UNF_AI_DELAY_S *pstDelay);

/**
\brief get frame buffer from AI. CNcomment: 获取声音帧存 CNend
\attention \n
Cast pcm data format  s32BitPerSample(16), u32Channels(2),bInterleaved(HI_TRUE), u32SampleRate(same as AI).
\param[in] AI handle CNcomment: AI句柄 CNend
\param[in] u32TimeoutMs     acquire timeout.CNcomment:获取超时 CNend
\param[out] pstFrame        frame info.CNcomment:帧信息 CNend
\retval ::HI_SUCCESS CNcomment: success.成功 CNend
\retval ::HI_ERR_AI_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AI_INVALID_ID     The handle is invalid. CNcomment: 非法句柄 CNend
\retval ::HI_ERR_AI_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_AI_AcquireFrame(HI_HANDLE hAI, HI_UNF_AO_FRAMEINFO_S *pstFrame, HI_U32 u32TimeoutMs);

/**
\brief Releases the frame buffer for AI . CNcomment: 释放声音帧存 CNend
\attention \n
\param[in] AI handle CNcomment: AI句柄 CNend
\param[in] u32TimeoutMs     acquire timeout.CNcomment:释放超时 CNend
\param[out] pstFrame        frame info.CNcomment:帧信息 CNend
\retval ::HI_SUCCESS CNcomment: success.成功 CNend
\retval ::HI_ERR_AI_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
\retval ::HI_ERR_AI_INVALID_ID     The handle is invalid. CNcomment: 非法句柄 CNend
\retval ::HI_ERR_AI_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_AI_ReleaseFrame(HI_HANDLE hAI, HI_UNF_AO_FRAMEINFO_S *pstFrame);


/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  /*__HI_UNF_AI_H__*/

