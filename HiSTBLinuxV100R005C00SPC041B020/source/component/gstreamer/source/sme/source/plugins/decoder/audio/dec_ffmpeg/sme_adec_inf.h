/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_adec_inf.h
 * @brief   audio dec interface
 * @author  liurongliang(l00180035)
 * @date    2014/3/26
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/3/26
 * Author : liurongliang(l00180035)
 * Desc   : Created file
 *
******************************************************************************/

#ifndef __SME_ADEC_INF_H__
#define __SME_ADEC_INF_H__

#include "osal_type.h"
#include "sme_media_type.h"
#include "sme_codec.h"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

typedef enum _tagAudioDecRet
{
    E_SME_ADEC_RET_SUCCESS = ICS_SUCCESS,
    E_SME_ADEC_RET_NOT_COMPLETE,//解码没有输出
    E_SME_ADEC_RET_UNSUPPORT,//不支持
    E_SME_ADEC_RET_INVALID_ARG,//非法参数
    E_SME_ADEC_RET_NOMEM,//分配内存错误
    E_SME_ADEC_RET_FATAL,
}E_SME_ADEC_RET;

typedef enum _tagAudioLibType
{
    E_SME_ADEC_LIB_BASE = 0,
    E_SME_ADEC_LIB_FFMPEG = E_SME_ADEC_LIB_BASE,
#ifdef __LINUX__
    E_SME_ADEC_LIB_HISI,
#endif
#ifdef __DDP_PT__
    E_SME_ADEC_LIB_PT,
#endif
    E_SME_ADEC_LIB_MAX,
} E_SME_ADEC_LIB_TYPE;

typedef struct _tagSmeAudioDecInf
{
    ST_SME_CODEC_BASE_INF stBase;
    SMECODECHDL (*SME_CreateAudioDec)(IN const ST_SME_LOG_CB * pstLogCtx);
    V_VOID (*SME_DestroyAudioDec)(INOUT SMECODECHDL hdlDec);
    E_SME_ADEC_RET (*SME_OpenAudioDec)(INOUT SMECODECHDL hdlDec);
    V_VOID (*SME_CloseAudioDec)(INOUT SMECODECHDL hdlDec);
    E_SME_ADEC_RET (*SME_SetAudioInfo)(IN SMECODECHDL hdlDec, IN const ST_SME_AUDIOINFO* pstaudioinfo);

    //设置configdata或者解码输出第一帧数据后可获取真正音频信息。
    E_SME_ADEC_RET (*SME_GetAudioInfo)(IN SMECODECHDL hdlDec, INOUT ST_SME_AUDIOINFO* pstAudioInfo);
    E_SME_ADEC_RET (*SME_DecAudioFrame)(IN SMECODECHDL hdlDec,
                            IN const ST_SME_MEDIA_FRAME* pstInBuffer,
                            INOUT ST_SME_ARAW_FRAMEBUFFER* pstOutBuffer,
                            INOUT V_INT32* decodedLen,
                            INOUT V_INT32* gotData,
                            IN const void * dec_info);
    E_SME_ADEC_RET (*SME_FlushAudioDec)(IN SMECODECHDL hdlDec);
}ST_SME_AUDIO_DEC_INF, *PST_SME_AUDIO_DEC_INF;

//单例
const ST_SME_AUDIO_DEC_INF *SME_GetAudioDecInfOfFFMpeg();
const ST_SME_AUDIO_DEC_INF *SME_GetAudioDecInfOfHisi();
#ifdef __DDP_PT__
const ST_SME_AUDIO_DEC_INF *SME_GetAudioDecInfOfPT();
#endif

#ifdef __cplusplus
}
#endif //__cplusplus

#endif//__SME_ADEC_INF_H__
