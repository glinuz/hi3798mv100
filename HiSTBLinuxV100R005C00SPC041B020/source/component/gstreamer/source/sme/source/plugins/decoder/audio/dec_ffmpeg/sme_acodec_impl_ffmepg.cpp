/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_acodec_impl_ffmepg.cpp
 * @brief   sme_acodec_impl_ffmepg.cpp实现
 * @author
 * @date    2014/4/14
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/4/14
 * Author :
 * Desc   : Created file
 *
******************************************************************************/

/*begin : 解决cpp用ffmepg时INT64_C编译错误*/
#ifdef __cplusplus
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif
#include <stdint.h>
#endif
/*end : 解决cpp用ffmepg时INT64_C编译错误*/
#include "osal_type.h"
#include "sme_macro.h"
#include "sme_media_type.h"
#include "sme_log_adpt.h"
#include "sme_adec_inf.h"
#include "osal_mem.h"
#include "osal_str.h"

UTILS_EXTERN_C_BEGIN
#include "avcodec.h"
#include "libswresample/swresample.h"
#include "libswresample/swresample_internal.h"
UTILS_EXTERN_C_END
;
//lint -e585 (可变参数...报错，为gstreamer中的日志宏，无法修改。)
//lint -e655 //位操作使用（兼容的）枚举类型
//lint -e717 //do{}while(0)
//lint -e826 //不等价指针转换，这里为安全转换
//lint -e838 //增加指针容量（Context）
//lint -e801 //goto, 这里可以使用goto,gstreamer里面也是这么使用的。
//lint -e774 //new 判断为空报，始终不为空,gstreamer 宏无法修改。
//lint -e1784 //SME_GetAudioDecInf(void)' previously declared as "C"
//lint -e605 //增加指针容量（Context)
//lint -e818 //指针参数'Symbol' (Location)可被声明为const的指针
//lint -efunc(1773,Sme_DecAudioFrameOfFFMpeg)
//lint -e501 //预期有符号的类型。
//lint -e648 //位操作：计算常数时溢出

//#define SME_FF_ADEC_DEBUG

/* BEGIN: Modified for DTS2014091806146/DTS2014091806052 by liurongliang(l00180035), 2014/9/25 */
typedef struct _tag_stLastAudioInfo
{
    enum AVSampleFormat eSampleFmt;
    V_INT32 i32SampleRate;
    V_INT32 i32Channels;
    V_UINT64 u64ChLayOut;
}ST_LAST_AUDIO_INFO;

typedef struct _tagpstFFMpegAudioDec
{
    AVCodecContext *pstContext;
    SwrContext* pstSwrCtx;
    ST_LAST_AUDIO_INFO stLastAInfo;
}ST_FFMPEG_AUDIO_DEC, *PST_FFMPEG_AUDIO_DEC;
/* END:   Modified for DTS2014091806146/DTS2014091806052 by liurongliang(l00180035), 2014/9/25 */

static E_SME_AUDIO_RAW_FMT g_aemSmeAdecFmt[2] = {E_SME_AUDIO_RAW_FMT_PCM_S16BIT,
    E_SME_AUDIO_RAW_FMT_NONE};

static ST_SME_LOG_CB g_sstThisLogCtx = {NULL,NULL};
static ST_SME_LOG_CB *pstThisLogCtx = &g_sstThisLogCtx;

static SMECODECHDL Sme_CreateAudioDecOfFFMpeg(IN const ST_SME_LOG_CB * pstLogCtx);

static inline AVCodecID Sme_SmeCodecIdToFFMpegCodecId(IN E_SME_MEDIA_CODEC_ID eCodecId)
{
    return (AVCodecID)eCodecId;
}

static inline E_SME_MEDIA_CODEC_ID Sme_FFMpegCodecIdToSmeCodecId(IN AVCodecID eCodecId)
{
    return (E_SME_MEDIA_CODEC_ID)eCodecId;
}

static inline AVSampleFormat Sme_SmeSampleFmtToFFMpegSampleFmt(IN E_SME_AUDIO_RAW_FMT eSamplefmt)
{
    return (AVSampleFormat)eSamplefmt;
}

static inline E_SME_MEDIA_TYPE Sme_FFMpegMediaTypeToSmeMediaType(IN AVMediaType eMediaType)
{
    return (E_SME_MEDIA_TYPE)eMediaType;
}

static const V_CHAR* Sme_GetCodecNameOfFFMpeg(IN CSMECODECFMT pvCtx)
{
    const AVCodec *pstCodec = (const AVCodec *)pvCtx;
    UTILS_MLOGE_RET_VAL_IF(NULL == pstCodec, NULL, ("SMECODECFMT is NULL!"));

    return pstCodec->name;
}

static E_SME_MEDIA_CODEC_ID Sme_GetCodecIdOfFFMpeg(IN CSMECODECFMT pvCtx)
{
    const AVCodec *pstCodec = (const AVCodec *)pvCtx;
    E_SME_MEDIA_CODEC_ID eCodeId;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstCodec, E_SME_MEDIA_CODEC_ID_BUTTON, ("SMECODECFMT is NULL!"));

    eCodeId = Sme_FFMpegCodecIdToSmeCodecId(pstCodec->id);

    return eCodeId;
}

static const E_SME_AUDIO_RAW_FMT* Sme_GetSampleFmtsOfFFMpeg(IN CSMECODECFMT pvCtx)
{
    const AVCodec *pstCodec = (const AVCodec *)pvCtx;
    E_SME_AUDIO_RAW_FMT* peFormats = NULL;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstCodec, NULL, ("SMECODECFMT is NULL!"));

    /* BEGIN: Modified for DTS2015102704109 by liurongliang(l00180035), 2015/10/28 */
    //decode will convert other fmt to S16
    peFormats = (E_SME_AUDIO_RAW_FMT*)g_aemSmeAdecFmt;
    /* END:   Modified for by liurongliang(l00180035), 2015/10/28 */

    return (const E_SME_AUDIO_RAW_FMT*)peFormats;
}

static CSMECODECFMT Sme_GetNextCodecOfFFMpeg(IN CSMECODECFMT pvCtx, IN const ST_SME_LOG_CB * pstLogCtx)
{
    AVCodec *pstCodec = av_codec_next((AVCodec *)(const_cast<SMECODECFMT>(pvCtx)));

    if(NULL != pstLogCtx)
    {
        g_sstThisLogCtx.pfCb = pstLogCtx->pfCb;
        g_sstThisLogCtx.pvCtx = pstLogCtx->pvCtx;
    }


    while(NULL != pstCodec)
    {
        E_SME_MEDIA_CODEC_ID eCodecId = Sme_FFMpegCodecIdToSmeCodecId(pstCodec->id);

        if(((eCodecId != E_SME_MEDIA_CODEC_ID_AMR_NB)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_AMR_WB)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_MP2)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_MP3)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_AAC)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_VORBIS)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_PCM_S16LE)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_PCM_S16BE)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_PCM_MULAW)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_PCM_ALAW)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_PCM_S32LE)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_PCM_S24LE)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_PCM_S24BE)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_PCM_U8)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_PCM_DVD)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_PCM_F32BE)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_PCM_F32LE)&&
#ifdef __ENABLE_ACODEC_PCM_BLURAY__
            (eCodecId != E_SME_MEDIA_CODEC_ID_PCM_BLURAY)&&
#endif
#ifdef __DDP_PT__
#else
            //(eCodecId != E_SME_MEDIA_CODEC_ID_AC3)&&
            //(eCodecId != E_SME_MEDIA_CODEC_ID_EAC3)&&
            //(eCodecId != E_SME_MEDIA_CODEC_ID_GSM)&&
            //(eCodecId != E_SME_MEDIA_CODEC_ID_GSM_MS)&&
            //(eCodecId != E_SME_MEDIA_CODEC_ID_DTS)
#endif
            (eCodecId != E_SME_MEDIA_CODEC_ID_FLAC)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_WMAV1)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_WMAV2)&&
#ifdef __ENABLE_ACODEC_WMA_PRO__
            (eCodecId != E_SME_MEDIA_CODEC_ID_WMAVpro)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_WMAVLOSSLESS)&&
#endif
            (eCodecId != E_SME_MEDIA_CODEC_ID_ADPCM_IMA_WAV)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_ADPCM_MS)))
        {
            pstCodec = av_codec_next(pstCodec);
        }
        else
        {
            break;
        }
    }
    return (SMECODECFMT)pstCodec;
}

static SMECODECHDL Sme_CreateAudioDecOfFFMpeg(IN const ST_SME_LOG_CB * pstLogCtx)
{
    PST_FFMPEG_AUDIO_DEC pstFFMpegAudioDec = NULL;
    E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;

    SME_LOGI_ADPT("creat audiodec");

    if(NULL != pstLogCtx)
    {
        g_sstThisLogCtx.pfCb = pstLogCtx->pfCb;
        g_sstThisLogCtx.pvCtx = pstLogCtx->pvCtx;
    }

    //lint +rw(malloc)
    pstFFMpegAudioDec = (PST_FFMPEG_AUDIO_DEC)malloc(sizeof(ST_FFMPEG_AUDIO_DEC));
    UTILS_MLOGE_RET_VAL_IF(NULL == pstFFMpegAudioDec, NULL, ("malloc sme audio decoder failed!"));
    eMemSecRet = VOS_Memset_S(pstFFMpegAudioDec, sizeof(ST_FFMPEG_AUDIO_DEC), 0,
        sizeof(ST_FFMPEG_AUDIO_DEC));
    if(eMemSecRet != E_SME_MEM_OK)
    {
        SME_LOGE_ADPT("VOS_Memset_S failed:%d", eMemSecRet);
    }

    pstFFMpegAudioDec->pstContext = avcodec_alloc_context3(NULL);
    if(NULL == pstFFMpegAudioDec->pstContext)
    {
        SME_LOGE_ADPT("malloc sme audio decoder context failed!");
        //lint -e424
        //数据的不恰当重新分配
        UTILS_MSAFEFREE(pstFFMpegAudioDec);
        //lint +e424
    }
    else
    {
        pstFFMpegAudioDec->pstContext->extradata = NULL;
        pstFFMpegAudioDec->pstContext->extradata_size = 0;
    }

    SME_LOGI_ADPT("creat audiodec finish");

    return (SMECODECHDL)pstFFMpegAudioDec;
}

static V_VOID Sme_DestroyAudioDecOfFFMpeg(INOUT SMECODECHDL hdlDec)
{
    PST_FFMPEG_AUDIO_DEC pstFFMpegAudioDec = (PST_FFMPEG_AUDIO_DEC)hdlDec;

    SME_LOGI_ADPT("destory audiodec");
    UTILS_MLOGE_RET_IF((NULL == pstFFMpegAudioDec), ("hdlDec is NULL!"));

    /** BEGIN: Modified by liurongliang(l00180035), 2014/7/5 */
    /*adjust for ffmpeg-release-2.2*/
    if(NULL != pstFFMpegAudioDec->pstSwrCtx)
    {
        swr_free(&pstFFMpegAudioDec->pstSwrCtx);
    }

    if(NULL != pstFFMpegAudioDec->pstContext)
    {
        if (pstFFMpegAudioDec->pstContext->extradata)
        {
            av_free (pstFFMpegAudioDec->pstContext->extradata);
            pstFFMpegAudioDec->pstContext->extradata = NULL;
            pstFFMpegAudioDec->pstContext->extradata_size = 0;
        }
        av_free(pstFFMpegAudioDec->pstContext);
        pstFFMpegAudioDec->pstContext = NULL;
    }
    /** END:   Modified by liurongliang(l00180035), 2014/7/5 */

    UTILS_MSAFEFREE(pstFFMpegAudioDec);

    SME_LOGI_ADPT("destory audiodec finish");
    (void)pstFFMpegAudioDec;
    return;
}
static E_SME_ADEC_RET  Sme_OpenAudioDecOfFFMpeg(INOUT SMECODECHDL hdlDec)
{
    AVCodec *pstCodec = NULL;
    E_SME_ADEC_RET eRet = E_SME_ADEC_RET_SUCCESS;
    PST_FFMPEG_AUDIO_DEC pstFFMpegAudioDec = (PST_FFMPEG_AUDIO_DEC)hdlDec;
    AVCodecID eCodecId;
    E_SME_MEM_SECURE_RET eMemSecRet;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstFFMpegAudioDec, E_SME_ADEC_RET_INVALID_ARG, ("hdlDec is NULL!"));
    UTILS_MLOGE_RET_VAL_IF(NULL == pstFFMpegAudioDec->pstContext, E_SME_ADEC_RET_INVALID_ARG,("hdlDec conetext is NULL!"));

    eCodecId = pstFFMpegAudioDec->pstContext->codec_id;

    /* BEGIN: Added for DTS2014091806146/DTS2014091806052 by liurongliang(l00180035), 2014/9/25 */
    eMemSecRet = VOS_Memset_S(&pstFFMpegAudioDec->stLastAInfo, sizeof(pstFFMpegAudioDec->stLastAInfo),
        0, sizeof(pstFFMpegAudioDec->stLastAInfo));
    if(eMemSecRet != E_SME_MEM_OK)
    {
        SME_LOGE_ADPT("VOS_Memset_S failed:%d", eMemSecRet);
    }
    /* END:   Added for DTS2014091806146/DTS2014091806052 by liurongliang(l00180035), 2014/9/25 */

    SME_LOGI_ADPT("find %d avcodec!",eCodecId);

    pstCodec = avcodec_find_decoder(eCodecId);
    UTILS_MLOGE_RET_VAL_IF(NULL == pstCodec, E_SME_ADEC_RET_UNSUPPORT, ("codec not found!"));

    SME_LOGI_ADPT("codec_type=%d,type=%d",pstFFMpegAudioDec->pstContext->codec_type,pstCodec->type);

    if(avcodec_open2(pstFFMpegAudioDec->pstContext, pstCodec, NULL)< 0)
    {
        eRet = E_SME_ADEC_RET_UNSUPPORT;
        SME_LOGE_ADPT("open %d avcodec failed!",(AVCodecID)pstFFMpegAudioDec->pstContext->codec_id);
    }

    return eRet;

}
static V_VOID Sme_CloseAudioDecOfFFMpeg(INOUT SMECODECHDL hdlDec)
{
    PST_FFMPEG_AUDIO_DEC pstFFMpegAudioDec = (PST_FFMPEG_AUDIO_DEC)hdlDec;

    SME_LOGI_ADPT("Close audiodec");
    UTILS_MLOGE_RET_IF(NULL == pstFFMpegAudioDec, ("hdlDec is NULL!"));
    UTILS_MLOGE_RET_IF(NULL == pstFFMpegAudioDec->pstContext,("hdlDec conetext is NULL!"));

    /** BEGIN: Modified by liurongliang(l00180035), 2014/7/5 */
    /*adjust for ffmpeg-release-2.2*/
    if(NULL != pstFFMpegAudioDec->pstSwrCtx)
    {
        swr_free(&pstFFMpegAudioDec->pstSwrCtx);
    }

    if (pstFFMpegAudioDec->pstContext->extradata)
    {
        av_free (pstFFMpegAudioDec->pstContext->extradata);
        pstFFMpegAudioDec->pstContext->extradata = NULL;
        pstFFMpegAudioDec->pstContext->extradata_size = 0;
    }
    /** BEGIN: Modified for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */
    avcodec_close(pstFFMpegAudioDec->pstContext);
    /** END:   Modified for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */

    /** END:   Modified by liurongliang(l00180035), 2014/7/5 */

    SME_LOGI_ADPT("Close audiodec finish");

    return;
}

static E_SME_ADEC_RET Sme_SetAudioInfoOfFFMpeg(
    IN SMECODECHDL hdlDec,
    IN const ST_SME_AUDIOINFO* pstaudioinfo)
{
    PST_FFMPEG_AUDIO_DEC pstFFMpegAudioDec  = (PST_FFMPEG_AUDIO_DEC)(hdlDec);
    AVSampleFormat eSampleFmt = AV_SAMPLE_FMT_NONE;
    AVCodecID  eCodecId = CODEC_ID_NONE;
    E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstFFMpegAudioDec,
                           E_SME_ADEC_RET_INVALID_ARG, ("hdlDec is NULL!"));
    UTILS_MLOGE_RET_VAL_IF(NULL == pstaudioinfo,
                           E_SME_ADEC_RET_INVALID_ARG,
                           ("pstaudioinfo is NULL!"));
    UTILS_MLOGE_RET_VAL_IF(NULL == pstFFMpegAudioDec->pstContext,
                           E_SME_ADEC_RET_INVALID_ARG,
                           ("hdlDec conetext is NULL!"));

    if(0 != pstaudioinfo->stBase.stCfgData.u32CfgDataSize &&
            NULL != pstaudioinfo->stBase.stCfgData.pu8CfgData)
    {
        /** get_bits() in ffmpeg would read at lest 4 bytes once a time
            and this would make it access 3 bytes over the space boundary*/
        //lint +rw(av_malloc)
        pstFFMpegAudioDec->pstContext->extradata =
            (uint8_t *)av_malloc((unsigned int)(pstaudioinfo->stBase.stCfgData.u32CfgDataSize + 3));
        UTILS_MLOGE_RET_VAL_IF(NULL == pstFFMpegAudioDec->pstContext->extradata,
                               E_SME_ADEC_RET_NOMEM,
                               ("malloc ConfigData failed!"));
        eMemSecRet = VOS_Memcpy_S (pstFFMpegAudioDec->pstContext->extradata,
                            (V_UINT32)(pstaudioinfo->stBase.stCfgData.u32CfgDataSize + 3),
                            pstaudioinfo->stBase.stCfgData.pu8CfgData,
                            (V_UINT32)pstaudioinfo->stBase.stCfgData.u32CfgDataSize);
        if(eMemSecRet != E_SME_MEM_OK)
        {
            SME_LOGE_ADPT("VOS_Memcpy_S failed:%d", eMemSecRet);
        }
        pstFFMpegAudioDec->pstContext->extradata_size =
            (int)pstaudioinfo->stBase.stCfgData.u32CfgDataSize;
    }
    else
    {
        pstFFMpegAudioDec->pstContext->extradata = NULL;
        pstFFMpegAudioDec->pstContext->extradata_size = 0;
    }

    eCodecId = Sme_SmeCodecIdToFFMpegCodecId(pstaudioinfo->stBase.eCodecId);
    pstFFMpegAudioDec->pstContext->codec_id = eCodecId;
    pstFFMpegAudioDec->pstContext->channels = (int)pstaudioinfo->u32Channels;
    pstFFMpegAudioDec->pstContext->sample_rate =
        (int)pstaudioinfo->u32SampleRate;
    eSampleFmt = Sme_SmeSampleFmtToFFMpegSampleFmt(pstaudioinfo->eSampleFmt);
    pstFFMpegAudioDec->pstContext->sample_fmt = eSampleFmt;
    pstFFMpegAudioDec->pstContext->codec_type = AVMEDIA_TYPE_AUDIO;
    pstFFMpegAudioDec->pstContext->block_align = (V_INT32)pstaudioinfo->u32BlockAlign;
    pstFFMpegAudioDec->pstContext->bit_rate = (V_INT32)pstaudioinfo->stBase.u32Bps;
    pstFFMpegAudioDec->pstContext->bits_per_coded_sample = (V_INT32)pstaudioinfo->u32BitsPerCodecSample;
    return E_SME_ADEC_RET_SUCCESS;
}



//设置configdata或者解码输出第一帧数据后可获取真正音频信息。
static E_SME_ADEC_RET Sme_GetAudioInfoOfFFMpeg(IN SMECODECHDL hdlDec, INOUT ST_SME_AUDIOINFO* pstAudioInfo)
{
    E_SME_ADEC_RET eRet = E_SME_ADEC_RET_SUCCESS;
    PST_FFMPEG_AUDIO_DEC pstFFMpegAudioDec = (PST_FFMPEG_AUDIO_DEC)hdlDec;
    //E_SME_AUDIO_RAW_FMT eSampleFmt = E_SME_AUDIO_RAW_FMT_PCM_BUTTON;
    E_SME_MEDIA_CODEC_ID eCodecId;
    E_SME_MEDIA_TYPE eMediaType;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstFFMpegAudioDec,E_SME_ADEC_RET_INVALID_ARG, ("hdlDec is NULL!"));
    UTILS_MLOGE_RET_VAL_IF(NULL == pstAudioInfo,E_SME_ADEC_RET_INVALID_ARG, ("pstAudioInfo is NULL!"));
    UTILS_MLOGE_RET_VAL_IF(NULL == pstFFMpegAudioDec->pstContext, E_SME_ADEC_RET_INVALID_ARG, ("hdlDec conetext is NULL!"));
    eCodecId = Sme_FFMpegCodecIdToSmeCodecId(pstFFMpegAudioDec->pstContext->codec_id);
    eMediaType = Sme_FFMpegMediaTypeToSmeMediaType(pstFFMpegAudioDec->pstContext->codec_type);

    pstAudioInfo->stBase.eMediaType = eMediaType;
    pstAudioInfo->stBase.eCodecId = eCodecId;
    pstAudioInfo->stBase.u32Bps = (V_UINT32)pstFFMpegAudioDec->pstContext->bit_rate;
    pstAudioInfo->u32Version = 1;
    pstAudioInfo->u32SampleRate = (V_UINT32)pstFFMpegAudioDec->pstContext->sample_rate;
    pstAudioInfo->eSampleFmt = E_SME_AUDIO_RAW_FMT_PCM_S16BIT;///eSampleFmt;
    /**< FIXME: Output channle number is fixed to 2 */
    pstAudioInfo->u32Channels = 2;//(V_UINT32)pstFFMpegAudioDec->pstContext->channels;

    SME_LOGI_ADPT("samplerate=%d,fmt=%d,channels=%d,orgfmt=%d",
        pstAudioInfo->u32SampleRate, pstAudioInfo->eSampleFmt,
        pstAudioInfo->u32Channels,
        pstFFMpegAudioDec->pstContext->sample_fmt);

    return eRet;
}

/* BEGIN: Added for DTS2014091806146/DTS2014091806052 by liurongliang(l00180035), 2014/9/25 */
static V_BOOL IsAInfoChanged(ST_FFMPEG_AUDIO_DEC* pstFFMpegAudioDec, AVFrame *pstDecodedFrame)
{
    V_BOOL bRet = ICS_FALSE;

    if((pstFFMpegAudioDec->stLastAInfo.eSampleFmt != pstFFMpegAudioDec->pstContext->sample_fmt)
        || (pstFFMpegAudioDec->stLastAInfo.i32Channels != pstDecodedFrame->channels)
        || (pstFFMpegAudioDec->stLastAInfo.u64ChLayOut != (V_UINT64)((V_UINT32)pstDecodedFrame->channel_layout))
        || (pstFFMpegAudioDec->stLastAInfo.i32SampleRate != pstDecodedFrame->sample_rate))
    {

        SME_LOGI_ADPT("AInfo Change::Cur: sample_rate = %d, fmt = %#x, ch=%d, ch_layout = %lld;"
            "Last: sample_rate = %d, fmt = %#x, ch=%d, ch_layout = %lld",
            pstDecodedFrame->sample_rate,
            pstFFMpegAudioDec->pstContext->sample_fmt,
            pstDecodedFrame->channels,
            pstDecodedFrame->channel_layout,
            pstFFMpegAudioDec->stLastAInfo.i32SampleRate,
            pstFFMpegAudioDec->stLastAInfo.eSampleFmt,
            pstFFMpegAudioDec->stLastAInfo.i32Channels,
            pstFFMpegAudioDec->stLastAInfo.u64ChLayOut);

        pstFFMpegAudioDec->stLastAInfo.i32SampleRate    = pstDecodedFrame->sample_rate;
        pstFFMpegAudioDec->stLastAInfo.eSampleFmt       = pstFFMpegAudioDec->pstContext->sample_fmt;
        pstFFMpegAudioDec->stLastAInfo.i32Channels      = pstDecodedFrame->channels;
        pstFFMpegAudioDec->stLastAInfo.u64ChLayOut      = (V_UINT64)((V_UINT32)pstDecodedFrame->channel_layout);
        bRet = ICS_TRUE;
    }

    return bRet;
}
/* END:   Added for DTS2014091806146/DTS2014091806052 by liurongliang(l00180035), 2014/9/25 */

static E_SME_ADEC_RET Sme_DecAudioFrameOfFFMpeg(IN SMECODECHDL hdlDec,
                        IN const ST_SME_MEDIA_FRAME* pstInBuffer,
                        INOUT ST_SME_ARAW_FRAMEBUFFER* pstOutBuffer,
                        INOUT V_INT32* decodedLen,
                        INOUT V_INT32* gotData,const void * dec_info)
{
    E_SME_ADEC_RET eRet = E_SME_ADEC_RET_SUCCESS;

    AVPacket stAvpkt;
    AVFrame *pstDecodedFrame = NULL;
    V_INT32 i32GotFrame = 0;
    V_INT32 i32Len;
    V_UINT32 u32DataSize = 0;
    V_UINT32 u32OutDataLen = 0;
    V_UINT8* pu8OutData[ SWR_CH_MAX ];
    V_INT32 i32OutCount = 0;
    V_UINT8* pu8InData[ SWR_CH_MAX ];
    V_INT32 i32InCount = 0;
    V_INT64 i64DefaultLayout[] = {
        0, AV_CH_LAYOUT_MONO, AV_CH_LAYOUT_STEREO, AV_CH_LAYOUT_SURROUND,
        AV_CH_LAYOUT_2_2, AV_CH_LAYOUT_5POINT0, AV_CH_LAYOUT_5POINT1,
        AV_CH_LAYOUT_6POINT1, AV_CH_LAYOUT_7POINT1
    };
    E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;

    UTILS_MLOGE_RET_VAL_IF(NULL == hdlDec,
                           E_SME_ADEC_RET_INVALID_ARG, ("hdlDec is NULL!"));
    PST_FFMPEG_AUDIO_DEC pstFFMpegAudioDec = (PST_FFMPEG_AUDIO_DEC)hdlDec;
    UTILS_MLOGE_RET_VAL_IF(NULL == pstFFMpegAudioDec->pstContext,
                           E_SME_ADEC_RET_INVALID_ARG,
                           ("hdlDec context is NULL!"));
    UTILS_MLOGE_RET_VAL_IF(NULL == pstInBuffer,
                           E_SME_ADEC_RET_INVALID_ARG,
                           ("pstInBuffer is NULL!"));
    UTILS_MLOGE_RET_VAL_IF(NULL == pstOutBuffer,
                           E_SME_ADEC_RET_INVALID_ARG,
                           ("pstOutBuffer is NULL!"));

    pstOutBuffer->pstARawFrame = NULL;
    *gotData = 0;
    (void)dec_info;

    do
    {
        pstDecodedFrame = avcodec_alloc_frame();
        UTILS_MLOGE_BRK_VAL_IF(NULL == pstDecodedFrame,
                               eRet, E_SME_ADEC_RET_NOMEM,
                               ("malloc audiodec Frame failed!"));
        av_init_packet(&stAvpkt);
        stAvpkt.data = (uint8_t *)(pstInBuffer->pu8DataBuf);
        stAvpkt.size = (int)pstInBuffer->u32DataLen;

        i32Len = (V_INT32)avcodec_decode_audio4(pstFFMpegAudioDec->pstContext,
            pstDecodedFrame, &i32GotFrame, &stAvpkt);
        *decodedLen = i32Len;
        //针对码流中有无效数据做异常处理
        if((V_INT32)(AVERROR_INVALIDDATA) == i32Len)
        {
            i32GotFrame = 0;
        }

        UTILS_MLOGE_BRK_VAL_IF(i32Len < 0, eRet,
                               E_SME_ADEC_RET_FATAL,
                               ("Error while decoding!"));
        UTILS_MLOGW_BRK_VAL_IF(i32GotFrame <= 0, eRet,
                               E_SME_ADEC_RET_NOT_COMPLETE,
                               ("decode not completed"));

        u32DataSize =
            (V_UINT32)av_samples_get_buffer_size(
                NULL,
                pstFFMpegAudioDec->pstContext->channels,
                pstDecodedFrame->nb_samples,
                pstFFMpegAudioDec->pstContext->sample_fmt,
                (int)1);

#if defined SME_FF_ADEC_DEBUG
        SME_LOGI_ADPT("input data size %d, "
                      "samplerate=%d, fmt=%d, channels=%d,"
                      "output data size=%d, nb_smples=%d, "
                      "output data [%p:%p:%p],line[%d:%d:%d]",
                      pstInBuffer->u32DataLen,
                      pstFFMpegAudioDec->pstContext->sample_rate,
                      pstFFMpegAudioDec->pstContext->sample_fmt,
                      pstFFMpegAudioDec->pstContext->channels,
                      u32DataSize,
                      pstDecodedFrame->nb_samples,
                      pstDecodedFrame->data[0],
                      pstDecodedFrame->data[1],
                      pstDecodedFrame->data[2],
                      pstDecodedFrame->linesize[0],
                      pstDecodedFrame->linesize[1],
                      pstDecodedFrame->linesize[2]);
#endif

        /**< If the output PCM fomrat is fixed, we just need to consider sample
         *   number when calculate the output buffer size.
         */
        /*
        if((AV_SAMPLE_FMT_FLT == pstFFMpegAudioDec->pstContext->sample_fmt)
            || (AV_SAMPLE_FMT_FLTP == pstFFMpegAudioDec->pstContext->sample_fmt)
            || (AV_SAMPLE_FMT_S32 == pstFFMpegAudioDec->pstContext->sample_fmt)
            || (AV_SAMPLE_FMT_S32P == pstFFMpegAudioDec->pstContext->sample_fmt))
        {
            u32OutDataLen = u32DataSize >> 1;
        }
        else if((AV_SAMPLE_FMT_U8 == pstFFMpegAudioDec->pstContext->sample_fmt)
            || (AV_SAMPLE_FMT_U8P == pstFFMpegAudioDec->pstContext->sample_fmt))
        {
            u32OutDataLen = u32DataSize << 1;
        }
        else
        {
            u32OutDataLen = u32DataSize;
        }
        */
        i32OutCount = pstDecodedFrame->nb_samples;
        i32InCount = pstDecodedFrame->nb_samples;
        /**< FIXME: Always output 16 bits, stereo PCM format. */
        u32OutDataLen = (V_UINT32)i32OutCount << 2;
        pstOutBuffer->pstARawFrame =
            (PST_SME_ARAW_FRAME) pstOutBuffer->stAllocator.pfnLockBuf(
                pstOutBuffer->stAllocator.pvAllocCtx,
                u32OutDataLen + sizeof(ST_SME_ARAW_FRAME),
                -1);

        UTILS_MLOGE_BRK_VAL_IF(NULL == pstOutBuffer->pstARawFrame,
                               eRet, E_SME_ADEC_RET_NOMEM,
                               ("audiodec outbuffer memory malloc failed!"));

        /**< FIXME: wma decoder doesn't provide channel layout information,
         *   derive it from channels by outselves.
         *   Need to find a better way.
         */
        if (pstDecodedFrame->channel_layout == 0 ||
            pstFFMpegAudioDec->pstContext->channel_layout == 0)
        {
            if ((V_UINT32)pstDecodedFrame->channels <
                sizeof(i64DefaultLayout)/sizeof(V_INT64))
            {
                pstFFMpegAudioDec->pstContext->channel_layout =
                    pstDecodedFrame->channel_layout =
                    (V_UINT64)i64DefaultLayout[pstDecodedFrame->channels];
            }
            else
            {
                SME_LOGE_ADPT("Can't get a valid channel layout");
            }
        }

#if defined SME_FF_ADEC_DEBUG
        SME_LOGI_ADPT("Sme_DecAudioFrameOfFFMpeg 1,fmt=%#x, "
                      "ch=%d, ch_layout=%lld ctx=%lld, ch=%d",
                      pstFFMpegAudioDec->pstContext->sample_fmt,
                      pstDecodedFrame->channels,
                      pstDecodedFrame->channel_layout,
                      pstFFMpegAudioDec->pstContext->channel_layout,
                      pstFFMpegAudioDec->pstContext->channels);
#endif

        if(AV_SAMPLE_FMT_S16 == pstFFMpegAudioDec->pstContext->sample_fmt &&
           AV_CH_LAYOUT_STEREO == pstFFMpegAudioDec->pstContext->channel_layout)
        {
            eMemSecRet = VOS_Memcpy_S(pstOutBuffer->pstARawFrame->pu8DataBuf,
                u32OutDataLen, pstDecodedFrame->data[0], u32DataSize);
            if(eMemSecRet != E_SME_MEM_OK)
            {
                SME_LOGE_ADPT("VOS_Memcpy_S failed:%d", eMemSecRet);
            }
            pstOutBuffer->pstARawFrame->u32DataLen = u32DataSize;
        }
        else
        {
            /* BEGIN: Added for DTS2014091806146/DTS2014091806052 by liurongliang(l00180035), 2014/9/25 */
            if(IsAInfoChanged(pstFFMpegAudioDec, pstDecodedFrame))
            {
                if(NULL != pstFFMpegAudioDec->pstSwrCtx)
                {
                    swr_free(&pstFFMpegAudioDec->pstSwrCtx);
                }
            }
            /* END:   Added for DTS2014091806146/DTS2014091806052 by liurongliang(l00180035), 2014/9/25 */

            if(NULL == pstFFMpegAudioDec->pstSwrCtx)
            {
                int64_t i64ChLayOut = (V_INT64)
                    pstFFMpegAudioDec->pstContext->channel_layout;

                pstFFMpegAudioDec->pstSwrCtx = swr_alloc_set_opts(
                    pstFFMpegAudioDec->pstSwrCtx,
                    (V_INT64)AV_CH_LAYOUT_STEREO, /*i64ChLayOut,*/
                    AV_SAMPLE_FMT_S16,//out
                    pstFFMpegAudioDec->pstContext->sample_rate,
                    i64ChLayOut, pstFFMpegAudioDec->pstContext->sample_fmt,//in
                    pstFFMpegAudioDec->pstContext->sample_rate,
                    0, NULL);

                SME_LOGI_ADPT("create swr: %p, "
                              "in: layout %lld, sample fmt %d, sample rate %d, "
                              "out: layout %d, sample fmt %d, sample rate %d",
                              pstFFMpegAudioDec->pstSwrCtx,
                              i64ChLayOut,
                              pstFFMpegAudioDec->pstContext->sample_fmt,
                              pstFFMpegAudioDec->pstContext->sample_rate,
                              AV_CH_LAYOUT_STEREO,
                              pstFFMpegAudioDec->pstContext->sample_fmt,
                              AV_SAMPLE_FMT_S16);
            }

            if(NULL != pstFFMpegAudioDec->pstSwrCtx)
            {
                if(!swr_is_initialized(pstFFMpegAudioDec->pstSwrCtx))
                {
                    if (0 != swr_init(pstFFMpegAudioDec->pstSwrCtx))
                    {
                       SME_LOGE_ADPT("swr_init failed");
                    }
                }

                if(swr_is_initialized(pstFFMpegAudioDec->pstSwrCtx))
                {
                    V_INT32 i32Min = M_SME_MIN(SWR_CH_MAX,
                        (sizeof(pstDecodedFrame->data)
                        / sizeof(pstDecodedFrame->data[0])));

                    eMemSecRet = VOS_Memset_S(
                        pu8OutData,
                        sizeof(pu8OutData) / sizeof(pu8OutData[0]),
                        0,
                        sizeof(pu8OutData) / sizeof(pu8OutData[0]));
                    if(eMemSecRet != E_SME_MEM_OK)
                    {
                        SME_LOGE_ADPT("VOS_Memset_S failed:%d", eMemSecRet);
                    }
                    eMemSecRet = VOS_Memset_S(
                        pu8InData,
                        sizeof(pu8InData) / sizeof(pu8InData[0]),
                        0,
                        sizeof(pu8InData) / sizeof(pu8InData[0]));
                    if(eMemSecRet != E_SME_MEM_OK)
                    {
                        SME_LOGE_ADPT("VOS_Memset_S failed:%d", eMemSecRet);
                    }
                    pu8OutData[0] = pstOutBuffer->pstARawFrame->pu8DataBuf;
                    for(V_INT32 k = 0; k < i32Min; k++)
                    {
                        pu8InData[k] = (V_UINT8*)pstDecodedFrame->data[k];
                    }

                    V_INT32 i32Err = swr_convert(pstFFMpegAudioDec->pstSwrCtx,
                        (uint8_t**)pu8OutData, i32OutCount,
                        (const uint8_t**)pu8InData, i32InCount);
                    if(i32Err > 0)
                    {
                        pstOutBuffer->pstARawFrame->u32DataLen =
                            (V_UINT32)i32OutCount << 2;
                    }
                    else
                    {
                        SME_LOGE_ADPT("Fail to convert audio sample, return %d",
                                      i32Err);
                        pstOutBuffer->pstARawFrame->u32DataLen = 0;
                    }
                }
                else
                {
                    SME_LOGE_ADPT("SWR is not initialized");
                }
            }
        }
        pstOutBuffer->pstARawFrame->eFmt = E_SME_AUDIO_RAW_FMT_PCM_S16BIT;//(E_SME_AUDIO_RAW_FMT)pstDecodedFrame->format;
        eMemSecRet = VOS_Memcpy_S(&pstOutBuffer->pstARawFrame->stInfo,
            sizeof(ST_SME_FRAME_INFO),
            &pstInBuffer->stInfo,
            sizeof(ST_SME_FRAME_INFO));
        if(eMemSecRet != E_SME_MEM_OK)
        {
            SME_LOGE_ADPT("VOS_Memcpy_S failed:%d", eMemSecRet);
        }
        pstOutBuffer->stAllocator.pfnUnLockBuf(pstOutBuffer->stAllocator.pvAllocCtx,
                                                    pstOutBuffer->pstARawFrame);
    }while(ICS_FALSE);

    /** BEGIN: Modified by liurongliang(l00180035), 2014/7/5 */
    /*adjust for ffmpeg-release-2.2*/
    avcodec_free_frame(&pstDecodedFrame);
    /** END:   Modified by liurongliang(l00180035), 2014/7/5 */
    *gotData = i32GotFrame;
    return eRet;

}
static E_SME_ADEC_RET  Sme_FlushAudioDecoderOfFFMpeg(IN SMECODECHDL hdlDec)
{
    SME_LOGI_ADPT("Sme_FlushAudioDecoderOfFFMpeg in");

    UTILS_MLOGE_RET_VAL_IF(NULL == hdlDec, E_SME_ADEC_RET_INVALID_ARG, ("hdlDec is NULL!"));
    E_SME_ADEC_RET eRet = E_SME_ADEC_RET_SUCCESS;
    PST_FFMPEG_AUDIO_DEC pstFFMpegAudioDec = (PST_FFMPEG_AUDIO_DEC)hdlDec;
    UTILS_MLOGE_RET_VAL_IF(NULL == pstFFMpegAudioDec->pstContext,E_SME_ADEC_RET_INVALID_ARG, ("hdlDec context is NULL!"));

    SME_LOGI_ADPT("Sme_FlushAudioDecoderOfFFMpeg 0  type=%d, %d,layout=%lld,ch=%d",
        //HAVE_THREADS ,
        pstFFMpegAudioDec->pstContext->active_thread_type ,
        FF_THREAD_FRAME,pstFFMpegAudioDec->pstContext->channel_layout,
            pstFFMpegAudioDec->pstContext->channels);

    avcodec_flush_buffers(pstFFMpegAudioDec->pstContext);
        SME_LOGI_ADPT("Sme_FlushAudioDecoderOfFFMpeg 1 type=%d, %d,layout=%lld,ch=%d",
        //HAVE_THREADS ,
        pstFFMpegAudioDec->pstContext->active_thread_type ,
        FF_THREAD_FRAME,pstFFMpegAudioDec->pstContext->channel_layout,
            pstFFMpegAudioDec->pstContext->channels);
    SME_LOGI_ADPT("Sme_FlushAudioDecoderOfFFMpeg out");

    return eRet;
}

static ST_SME_AUDIO_DEC_INF g_sstSmeGetAudioDecInf = {
    /* stBase */
    {
        Sme_GetCodecNameOfFFMpeg,
        NULL,
        Sme_GetCodecIdOfFFMpeg,
        Sme_GetSampleFmtsOfFFMpeg,
        Sme_GetNextCodecOfFFMpeg
    },
    Sme_CreateAudioDecOfFFMpeg,    /* SME_CreateAudioDec */
    Sme_DestroyAudioDecOfFFMpeg,    /* SME_DestroyAudioDec */
    Sme_OpenAudioDecOfFFMpeg,    /* SME_OpenAudioDec */
    Sme_CloseAudioDecOfFFMpeg,    /* SME_CloseAudioDec */
    Sme_SetAudioInfoOfFFMpeg,    /* SME_SetAudioInfo */
    Sme_GetAudioInfoOfFFMpeg,    /* SME_GetAudioInfo */
    Sme_DecAudioFrameOfFFMpeg,    /* SME_DecAudioFrame */
    Sme_FlushAudioDecoderOfFFMpeg,    /* SME_FlushAudioDec */
};

const ST_SME_AUDIO_DEC_INF *SME_GetAudioDecInfOfFFMpeg()
{
    PST_SME_AUDIO_DEC_INF pRet = NULL;

    SME_LOGI_ADPT("SME Get audiodec Inf in.");
    avcodec_register_all();
    pRet = &g_sstSmeGetAudioDecInf;
    SME_LOGI_ADPT("SME Get audiodec out,pRet=%p.", pRet);
    return pRet;
}
