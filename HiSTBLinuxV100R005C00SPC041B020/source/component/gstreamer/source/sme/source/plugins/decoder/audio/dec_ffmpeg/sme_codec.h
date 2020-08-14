/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_codec.h
 * @brief   sme codec公共结构
 * @author
 * @date    2014/3/27
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/3/27
 * Author :
 * Desc   : Created file
 *
******************************************************************************/


#ifndef __SME_CODEC_H__
#define __SME_CODEC_H__

#include "osal_type.h"
#include "sme_media_type.h"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

typedef V_VOID* SMECODECHDL;
typedef V_VOID* SMECODECFMT;
typedef const V_VOID* CSMECODECFMT;

typedef struct _tagSmeCodecBaseInf
{
    const V_CHAR*(*SME_GetCodecName)(IN CSMECODECFMT pvCtx);
    const V_CHAR*(*SME_GetCodecDescription)(IN CSMECODECFMT pvCtx);
    E_SME_MEDIA_CODEC_ID (*SME_GetCodecId)(IN CSMECODECFMT pvCtx);

    //TODO:视频解码没这个类似的接口，需要想办法做成类似
    const E_SME_AUDIO_RAW_FMT* (*SME_GetSampleFmts)(IN CSMECODECFMT pvCtx);

    //第一个时adecCtx输入NULL。
    CSMECODECFMT (*SME_GetNextCodec)(IN CSMECODECFMT pvCtx, IN const ST_SME_LOG_CB * pstLogCtx);
}ST_SME_CODEC_BASE_INF;

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__SME_CODEC_H__
