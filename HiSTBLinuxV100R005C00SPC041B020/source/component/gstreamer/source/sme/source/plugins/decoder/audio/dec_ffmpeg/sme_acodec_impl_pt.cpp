/**************************************************************************//**

  Copyright (C), 2001-2016, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_acodec_impl_pt.cpp
 * @brief   sme_acodec_impl_pt.cpp实现
 * @author  yaojian(y00342710)
 * @date    2016/2/14
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2016/2/14
 * Author : yaojian(y00342710)
 * Desc   : Created file
 *
******************************************************************************/


#ifdef __cplusplus
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif
#include <stdint.h>
#endif

#include "osal_type.h"
#include "sme_macro.h"
#include "sme_media_type.h"
#include "sme_log_adpt.h"
#include "sme_adec_inf.h"
#include "osal_mem.h"
#include "osal_str.h"
#include "gstsmeaudiodec.h"

UTILS_EXTERN_C_BEGIN
#include "avcodec.h"
UTILS_EXTERN_C_END
;

/*
#define sw16(x) \
       ((uint16_t)(\
                 (((uint16_t)(x)&(uint16_t)0x00ffU)<< 8) |\
                 (((uint16_t)(x)&(uint16_t)0xff00U)>> 8)))
*/


#define sw32(x) \
       ((uint32_t)(\
                 (((uint32_t)(x)&(uint32_t)0x000000ffU)     << 24) |\
                 ((((uint32_t)(x)&(uint32_t)0x0000ff00U)>>8)  << 16) |\
                 ((((uint32_t)(x)&(uint32_t)0x00ff0000U)>>16) << 8 ) |\
                 (((uint32_t)(x)&(uint32_t)0xff000000U)>>24      )))

/*

#define sw64(x) \
       ((uint64_t)(\
                 (((uint64_t)(x)&(uint64_t)0x00000000000000ffLLU)     << 56) |\
                 (((uint64_t)(x)&(uint64_t)0x000000000000ff00LLU)>>8  << 48) |\
                 (((uint64_t)(x)&(uint64_t)0x0000000000ff0000LLU)>>16 << 40) |\
                 (((uint64_t)(x)&(uint64_t)0x00000000ff000000LLU)>>24 << 32) |\
                 (((uint64_t)(x)&(uint64_t)0x000000ff00000000LLU)>>32 << 24) |\
                 (((uint64_t)(x)&(uint64_t)0x0000ff0000000000LLU)>>40 << 16) |\
                 (((uint64_t)(x)&(uint64_t)0x00ff000000000000LLU)>>48 << 8) |\
                 (((uint64_t)(x)&(uint64_t)0xff00000000000000LLU)>>56      )))
*/

#define MAX_BUF_LEN (128*1024)
#define MAX_DURATION_START (30*1000000LL)   // 100ms
#define MAX_DURATION (30*1000000LL)

typedef struct _tagpstPTAudioDec
{
    ST_SME_AUDIOINFO stAInfo;
    uint8_t buf[MAX_BUF_LEN];
    int buf_len;
    uint64_t pts;
    uint64_t dur;
    uint64_t dur_max;
}ST_PT_AUDIO_DEC, *PST_PT_AUDIO_DEC;

typedef struct _ddp_pt_frame_header_t
{
    uint16_t sync_word;
    uint16_t version;
    uint32_t size;
    uint64_t pts;
} ddp_pt_frame_header_t, *pst_ddp_pt_frame_header_t;

static E_SME_AUDIO_RAW_FMT g_aemSmeAdecFmt_AC3[2] = {
#ifdef __DDP_PT__
    E_SME_AUDIO_RAW_FMT_PT_AC3,
#else
    E_SME_AUDIO_RAW_FMT_PCM_S16BIT,
#endif
    E_SME_AUDIO_RAW_FMT_NONE};
static E_SME_AUDIO_RAW_FMT g_aemSmeAdecFmt_E_AC3[2] = {
#ifdef __DDP_PT__
    E_SME_AUDIO_RAW_FMT_PT_E_AC3,
#else
    E_SME_AUDIO_RAW_FMT_PCM_S16BIT,
#endif
    E_SME_AUDIO_RAW_FMT_NONE};


static ST_SME_LOG_CB g_sstThisLogCtx = {NULL,NULL};
static ST_SME_LOG_CB *pstThisLogCtx = &g_sstThisLogCtx;


static inline E_SME_MEDIA_CODEC_ID Sme_PTCodecIdToSmeCodecId(IN AVCodecID eCodecId)
{
    return (E_SME_MEDIA_CODEC_ID)eCodecId;
}


static const V_CHAR* Sme_GetCodecNameOfPT(IN CSMECODECFMT pvCtx)
{
    const AVCodec *pstCodec = (const AVCodec *)pvCtx;
    UTILS_MLOGE_RET_VAL_IF(NULL == pstCodec, NULL, ("SMECODECFMT is NULL!"));

    return pstCodec->name;
}

static E_SME_MEDIA_CODEC_ID Sme_GetCodecIdOfPT(IN CSMECODECFMT pvCtx)
{
    const AVCodec *pstCodec = (const AVCodec *)pvCtx;
    //E_SME_MEDIA_CODEC_ID eCodeId = E_SME_MEDIA_CODEC_ID_BUTTON;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstCodec, E_SME_MEDIA_CODEC_ID_BUTTON, ("SMECODECFMT is NULL!"));

    E_SME_MEDIA_CODEC_ID eCodeId = Sme_PTCodecIdToSmeCodecId(pstCodec->id);

    return eCodeId;
}

static const E_SME_AUDIO_RAW_FMT* Sme_GetSampleFmtsOfPT(IN CSMECODECFMT pvCtx)
{
    const AVCodec *pstCodec = (const AVCodec *)pvCtx;
    E_SME_AUDIO_RAW_FMT* peFormats = NULL;
    //E_SME_MEDIA_CODEC_ID eCodeId = E_SME_MEDIA_CODEC_ID_BUTTON;


    UTILS_MLOGE_RET_VAL_IF(NULL == pstCodec, NULL, ("SMECODECFMT is NULL!"));

    E_SME_MEDIA_CODEC_ID eCodeId = Sme_PTCodecIdToSmeCodecId(pstCodec->id);
    switch (eCodeId) {
        case E_SME_MEDIA_CODEC_ID_AC3:
            peFormats = (E_SME_AUDIO_RAW_FMT*)g_aemSmeAdecFmt_AC3;
            break;
        case E_SME_MEDIA_CODEC_ID_EAC3:
            peFormats = (E_SME_AUDIO_RAW_FMT*)g_aemSmeAdecFmt_E_AC3;
            break;
        default:
        break;
    }

    return (const E_SME_AUDIO_RAW_FMT*)peFormats;
}

static CSMECODECFMT Sme_GetNextCodecOfPT(IN CSMECODECFMT pvCtx, IN const ST_SME_LOG_CB * pstLogCtx)
{
    AVCodec *pstCodec = av_codec_next((AVCodec *)(const_cast<SMECODECFMT>(pvCtx)));

    if(NULL != pstLogCtx)
    {
        g_sstThisLogCtx.pfCb = pstLogCtx->pfCb;
        g_sstThisLogCtx.pvCtx = pstLogCtx->pvCtx;
    }


    while(NULL != pstCodec)
    {
        E_SME_MEDIA_CODEC_ID eCodecId = Sme_PTCodecIdToSmeCodecId(pstCodec->id);

        if(((eCodecId != E_SME_MEDIA_CODEC_ID_AC3)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_EAC3)&&
            (eCodecId != E_SME_MEDIA_CODEC_ID_TRUEHD)))
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

static SMECODECHDL Sme_CreateAudioDecOfPT(IN const ST_SME_LOG_CB * pstLogCtx)
{
    //PST_PT_AUDIO_DEC pstPTAudioDec = NULL;
    E_SME_MEM_SECURE_RET eMemSecRet;

    SME_LOGD_ADPT("creat audiodec");

    if(NULL != pstLogCtx)
    {
        g_sstThisLogCtx.pfCb = pstLogCtx->pfCb;
        g_sstThisLogCtx.pvCtx = pstLogCtx->pvCtx;
    }
    //lint +rw(malloc)
    ST_PT_AUDIO_DEC * pstPTAudioDec = (ST_PT_AUDIO_DEC *)malloc(sizeof(ST_PT_AUDIO_DEC));
    //lint -e774 //布尔变量总是估值为[True/False]
    //lint -e424 //Inappropriate deallocation (free) for 'constant' data
    UTILS_MLOGE_RET_VAL_IF(NULL == pstPTAudioDec, NULL, ("malloc sme audio decoder failed!"));
    eMemSecRet = VOS_Memset_S(pstPTAudioDec,sizeof(ST_PT_AUDIO_DEC),0,sizeof(ST_PT_AUDIO_DEC));
    if(eMemSecRet != E_SME_MEM_OK) {
        SME_LOGE_ADPT("VOS_Memset_S failed:%d", eMemSecRet);
        UTILS_MSAFEFREE(pstPTAudioDec);
        (void)pstPTAudioDec;
        return NULL;
    }
    pstPTAudioDec->dur_max = MAX_DURATION_START;
    //lint +e774
    //lint +e424

    SME_LOGD_ADPT("creat audiodec finish");

    return (SMECODECHDL)pstPTAudioDec;
}

static V_VOID Sme_DestroyAudioDecOfPT(INOUT SMECODECHDL hdlDec)
{
    PST_PT_AUDIO_DEC pstPTAudioDec = (PST_PT_AUDIO_DEC)hdlDec;

    //SME_LOGD_ADPT("destory audiodec");
    //UTILS_MLOGE_RET_IF((NULL == pstPTAudioDec), ("hdlDec is NULL!"));

    UTILS_MSAFEFREE(pstPTAudioDec);

    SME_LOGD_ADPT("destory audiodec finish");
    (void)pstPTAudioDec;
    return;
}
static E_SME_ADEC_RET  Sme_OpenAudioDecOfPT(INOUT SMECODECHDL hdlDec)
{
    (void)hdlDec;
    return E_SME_ADEC_RET_SUCCESS;
}
static V_VOID Sme_CloseAudioDecOfPT(INOUT SMECODECHDL hdlDec)
{
    (void)hdlDec;
    return;
}

static E_SME_ADEC_RET Sme_SetAudioInfoOfPT(
    IN SMECODECHDL hdlDec,
    IN const ST_SME_AUDIOINFO* pstaudioinfo)
{
    PST_PT_AUDIO_DEC pstPTAudioDec  = (PST_PT_AUDIO_DEC)(hdlDec);

    UTILS_MLOGE_RET_VAL_IF(NULL == pstPTAudioDec,E_SME_ADEC_RET_INVALID_ARG, ("hdlDec is NULL!"));
    UTILS_MLOGE_RET_VAL_IF(NULL == pstaudioinfo,E_SME_ADEC_RET_INVALID_ARG,("pstaudioinfo is NULL!"));

    pstPTAudioDec->stAInfo = *pstaudioinfo;
    pstPTAudioDec->stAInfo.u32Version = 1;

    return E_SME_ADEC_RET_SUCCESS;
}



static E_SME_ADEC_RET Sme_GetAudioInfoOfPT(IN SMECODECHDL hdlDec, INOUT ST_SME_AUDIOINFO* pstAudioInfo)
{
    PST_PT_AUDIO_DEC pstPTAudioDec = (PST_PT_AUDIO_DEC)hdlDec;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstPTAudioDec,E_SME_ADEC_RET_INVALID_ARG, ("hdlDec is NULL!"));
    UTILS_MLOGE_RET_VAL_IF(NULL == pstAudioInfo,E_SME_ADEC_RET_INVALID_ARG, ("pstAudioInfo is NULL!"));

    pstPTAudioDec->stAInfo.u32Version = 1;
    *pstAudioInfo = pstPTAudioDec->stAInfo;

    return E_SME_ADEC_RET_SUCCESS;
}


static E_SME_ADEC_RET Sme_DecAudioFrameOfPT(IN SMECODECHDL hdlDec,
                        IN const ST_SME_MEDIA_FRAME* pstInBuffer,
                        INOUT ST_SME_ARAW_FRAMEBUFFER* pstOutBuffer,
                        INOUT V_INT32* decodedLen,
                        INOUT V_INT32* gotData,const void * dec_info)
{
    E_SME_ADEC_RET eRet = E_SME_ADEC_RET_NOT_COMPLETE;

    const GstSmeTSInfo * dec_info_gst = (const GstSmeTSInfo *)dec_info;
    uint8_t * buf = NULL;
    int buf_len = 0;
    //uint64_t pts = 0;
    //uint64_t dur = 0;
    uint8_t overflow_flag = 0;
    E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;

    UTILS_MLOGE_RET_VAL_IF(NULL == hdlDec,
                           E_SME_ADEC_RET_INVALID_ARG, ("hdlDec is NULL!"));
    PST_PT_AUDIO_DEC pstPTAudioDec = (PST_PT_AUDIO_DEC)hdlDec;
    UTILS_MLOGE_RET_VAL_IF(NULL == pstInBuffer,
                           E_SME_ADEC_RET_INVALID_ARG,
                           ("pstInBuffer is NULL!"));
    UTILS_MLOGE_RET_VAL_IF(NULL == pstOutBuffer,
                           E_SME_ADEC_RET_INVALID_ARG,
                           ("pstOutBuffer is NULL!"));

    *gotData = 0;
    *decodedLen = (V_INT32)pstInBuffer->u32DataLen;


    // 1. check if dur >= MAX_DURATION
    if (pstPTAudioDec->buf_len <= 0 && dec_info_gst->duration >= pstPTAudioDec->dur_max) {
        buf = const_cast<uint8_t *>(pstInBuffer->pu8DataBuf);
        buf_len = (int)pstInBuffer->u32DataLen;
        //pts = dec_info_gst->timestamp;
        //dur = dec_info_gst->duration;
        *gotData = 1;
    } else {
        if (pstPTAudioDec->buf_len + (gint)pstInBuffer->u32DataLen <= MAX_BUF_LEN) {
            if (pstPTAudioDec->buf_len <= 0) {  // save pts
                pstPTAudioDec->pts = dec_info_gst->timestamp;
            }
            pstPTAudioDec->dur += dec_info_gst->duration;
            eMemSecRet = VOS_Memcpy_S(pstPTAudioDec->buf+pstPTAudioDec->buf_len,(V_SIZE)(MAX_BUF_LEN-pstPTAudioDec->buf_len),
                pstInBuffer->pu8DataBuf, (V_SIZE)pstInBuffer->u32DataLen);
            if(eMemSecRet != E_SME_MEM_OK) {
                SME_LOGE_ADPT("VOS_Memcpy_S failed:%d", eMemSecRet);
            } else {
                pstPTAudioDec->buf_len += (int)pstInBuffer->u32DataLen;
                if (pstPTAudioDec->dur >= pstPTAudioDec->dur_max) {
                    *gotData = 1;
                }
            }
        } else {
            // why this happend?
            *gotData = 1;
            overflow_flag = 1;
        }
        if (*gotData) {
            buf = pstPTAudioDec->buf;
            buf_len = pstPTAudioDec->buf_len;
            //pts = pstPTAudioDec->pts;
            //dur = pstPTAudioDec->dur;
        }
    }

    if (*gotData) {
        pstPTAudioDec->dur_max = MAX_DURATION;
        eRet = E_SME_ADEC_RET_SUCCESS;
        pstOutBuffer->pstARawFrame =
            (PST_SME_ARAW_FRAME) pstOutBuffer->stAllocator.pfnLockBuf(
                pstOutBuffer->stAllocator.pvAllocCtx,
                (guint)((guint)buf_len + sizeof(ddp_pt_frame_header_t) + sizeof(ST_SME_ARAW_FRAME)),
                -1);
        UTILS_MLOGE_RET_VAL_IF(NULL == pstOutBuffer->pstARawFrame,
                E_SME_ADEC_RET_NOMEM,
                ("audiodec outbuffer memory malloc failed!"));
            // header
        pst_ddp_pt_frame_header_t frame_header = (pst_ddp_pt_frame_header_t)(void*)pstOutBuffer->pstARawFrame->pu8DataBuf;
        frame_header->sync_word = 0x5555;
        //uint16_t version = 0x0001;
        //frame_header->version = sw16(version);
        frame_header->version = 0x0100;
        frame_header->size = sw32((guint)buf_len);
        //frame_header->pts = sw64(pts);
        frame_header->pts = 0;
        // copy raw data
        eMemSecRet = VOS_Memcpy_S(pstOutBuffer->pstARawFrame->pu8DataBuf+sizeof(ddp_pt_frame_header_t),(guint)buf_len,
                buf, (guint)buf_len);
        if(eMemSecRet != E_SME_MEM_OK) {
            SME_LOGE_ADPT("VOS_Memcpy_S failed:%d", eMemSecRet);
            *gotData = 0;
            eRet = E_SME_ADEC_RET_NOT_COMPLETE;
        }
        pstOutBuffer->pstARawFrame->u32DataLen = (V_UINT32)buf_len+sizeof(ddp_pt_frame_header_t);
        //pstOutBuffer->pstARawFrame->eFmt = E_SME_AUDIO_RAW_FMT_PT;//(E_SME_AUDIO_RAW_FMT)pstDecodedFrame->format;
        pstOutBuffer->pstARawFrame->eFmt = pstPTAudioDec->stAInfo.eSampleFmt;
        eMemSecRet = VOS_Memcpy_S(&pstOutBuffer->pstARawFrame->stInfo, sizeof(ST_SME_FRAME_INFO), &pstInBuffer->stInfo,
            sizeof(ST_SME_FRAME_INFO));
        if(eMemSecRet != E_SME_MEM_OK) {
            SME_LOGE_ADPT("VOS_Memcpy_S failed:%d", eMemSecRet);
            *gotData = 0;
            eRet = E_SME_ADEC_RET_NOT_COMPLETE;
        }
        pstOutBuffer->stAllocator.pfnUnLockBuf(pstOutBuffer->stAllocator.pvAllocCtx,
                                                    pstOutBuffer->pstARawFrame);
        //pstOutBuffer->pstARawFrame->stInfo.i64Pts = dec_info_gst->timestamp;
        //pstOutBuffer->pstARawFrame->stInfo.i64Dts = dec_info_gst->timestamp;
        //pstOutBuffer->pstARawFrame->stInfo.u32Duration = dec_info_gst->duration;
#if 0
    static int count = 0;
    SME_LOGD_ADPT("decode audio pt %d/%lld: len %d, in pts %lld/%lld, pts %lld, dts %lld, dur %u.",count++,pts/dur, buf_len,
        pts,dur,
        pstOutBuffer->pstARawFrame->stInfo.i64Pts, pstOutBuffer->pstARawFrame->stInfo.i64Dts,pstOutBuffer->pstARawFrame->stInfo.u32Duration);
#endif
        // clear buf
        pstPTAudioDec->buf_len = 0;
        pstPTAudioDec->pts = 0;
        pstPTAudioDec->dur = 0;
    } else {
        //SME_LOGD_ADPT("decode audio got no data! buffer_len %d",pstPTAudioDec->buf_len);
    }

    if (overflow_flag) {
        // need copy input buffer to buffer
        if (pstPTAudioDec->buf_len + (gint)pstInBuffer->u32DataLen <= MAX_BUF_LEN) {
            if (pstPTAudioDec->buf_len <= 0) {  // save pts
                pstPTAudioDec->pts = dec_info_gst->timestamp;
            }
            pstPTAudioDec->dur += dec_info_gst->duration;
            eMemSecRet = VOS_Memcpy_S(pstPTAudioDec->buf+pstPTAudioDec->buf_len,(V_SIZE)(MAX_BUF_LEN-pstPTAudioDec->buf_len),
                pstInBuffer->pu8DataBuf, (V_SIZE)pstInBuffer->u32DataLen);
            if(eMemSecRet != E_SME_MEM_OK) {
                SME_LOGE_ADPT("VOS_Memcpy_S failed:%d", eMemSecRet);
                *gotData = 0;
                eRet = E_SME_ADEC_RET_NOT_COMPLETE;
            }
            pstPTAudioDec->buf_len += (gint)pstInBuffer->u32DataLen;
        }
    }



    return eRet;

}


static E_SME_ADEC_RET  Sme_FlushAudioDecoderOfPT(IN SMECODECHDL hdlDec)
{
    PST_PT_AUDIO_DEC pstPTAudioDec = (PST_PT_AUDIO_DEC)hdlDec;
    pstPTAudioDec->buf_len = 0;
    pstPTAudioDec->pts = 0;
    pstPTAudioDec->dur = 0;
    pstPTAudioDec->dur_max = MAX_DURATION_START;

    return E_SME_ADEC_RET_SUCCESS;
}

static ST_SME_AUDIO_DEC_INF g_sstSmeGetAudioDecInf = {
    /* stBase */
    {
        Sme_GetCodecNameOfPT,
        NULL,
        Sme_GetCodecIdOfPT,
        Sme_GetSampleFmtsOfPT,
        Sme_GetNextCodecOfPT
    },
    Sme_CreateAudioDecOfPT,    /* SME_CreateAudioDec */
    Sme_DestroyAudioDecOfPT,    /* SME_DestroyAudioDec */
    Sme_OpenAudioDecOfPT,    /* SME_OpenAudioDec */
    Sme_CloseAudioDecOfPT,    /* SME_CloseAudioDec */
    Sme_SetAudioInfoOfPT,    /* SME_SetAudioInfo */
    Sme_GetAudioInfoOfPT,    /* SME_GetAudioInfo */
    Sme_DecAudioFrameOfPT,    /* SME_DecAudioFrame */
    Sme_FlushAudioDecoderOfPT,    /* SME_FlushAudioDec */
};

const ST_SME_AUDIO_DEC_INF *SME_GetAudioDecInfOfPT()
{
    PST_SME_AUDIO_DEC_INF pRet = &g_sstSmeGetAudioDecInf;
    SME_LOGI_ADPT("SME Get audiodec g_sstSmeGetAudioDecInf,pRet=%p.", pRet);
    return pRet;
}
