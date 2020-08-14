/**************************************************************************//**

  Copyright (C), 2001-2014, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file	-sme_demux_impl_ffmpeg.cpp
 * @brief	-sme demux impl based on ffmpeg
 * @author	-
 * @date	-2014/3/31
 * @version -V1.00
 *
 * History:
 *
 * Number : V1.00
 * Date   : 2014/3/31
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

#define MOD_NAME "SME_DEMUX_FFMPEG"
#include "osal_type.h"
#include "sme_macro.h"
#include "osal_mem.h"
#include "osal_str.h"
#include "osal_thread.h"
#include "sme_codec_cfgdata_proc.h"
#include "sme_demux_inf.h"

UTILS_EXTERN_C_BEGIN

#include "avformat.h"
#include "avcodec.h"
#include "url.h"
#include "libavutil/error.h"
#include "libavutil/mathematics.h"

UTILS_EXTERN_C_END
;

//android4.4 pclint
//typedef size_t V_SIZE;

//lint -e774
//lint -e717 //do{}while(0)
//lint -e737 //int到uint的转换
//lint -e838 //前一次赋值未使用(通常是在变量定义时赋空值，无需修改)
//lint -efunc(435,FlushAVIOFormat) //ffmpeg定义的宏
//lint -e438 //前一次赋值未使用
//lint -esym(40,va_list) //系统变量不识别，头文件已包含
//lint -esym(601,vl) //va_list系统变量类型不识别，头文件已包含
//lint -e826 //不等价指针转换，这里为安全转换
//lint -e650 //常量超出==符号的操作范围,ffmpeg定义错误码的方式
//lint -e669 //memory copy溢出问题，这里为安全拷贝
//lint -e1784
//lint -esym(754,_tagSmeDemuxHdl::acMime) //没使用
//lint -e501 //ffmpeg符号中宏定义AVERROR_EOF
//lint -esym(1055,strlen) //strlen系统函数
//lint -esym(40,NULL) //NULL误报
//lint -esym(613,pstVideoInfo,pstAudioInfo,pstMediaInfo) //指针已经判断过
//lint -e10

#define SME_DEMUX_DEFAULT_DURATION_V    33//ms
#define SME_DEMUX_DEFAULT_DURATION_A    21//ms
#define SME_DEMUX_DEFAULT_DURATION_MAX  330//ms
//#define SME_DEMUX_DEFAULT_DURATION_MIN  5 //ms
#define SME_DEMUX_DEFAULT_WIDTH         3840
#define SME_DEMUX_DEFAULT_HEIGHT        2160
#define SME_DEMUX_DEFAULT_A_SR          44100 //Sample Rate
#define SME_DEMUX_DEFAULT_Ch            2     //channel
#define SME_DEMUX_V_MAX_PROBESIZE       (384*1000)//384KB
#define SME_DEMUX_A_MAX_PROBESIZE       (128*1000)//128KB
#define SME_DEMUX_MIN_PROBESIZE         (0)//0MB
#define SME_DEMUX_PROBESIZE_LEFT        (1024*1000)//1MB
//#define SME_DEMUX_SEEK_BACK_STEP        (5000000LL)//mS
#define GST_ES_MAX_FRAME_PTS_DELTA       10000//ms
#define SME_DEMUX_PACKET_QUEUE_MAX_LEN  100

#define SME_DEMUX_PREV_DTS_NUM          3
#define SME_DEMUX_DTS_HOP_NUM           3
#define SME_DEMUX_DTS_HOP_THRESH_HOLD   1.5
#define SME_DEMUX_DTS_LINEUP_MAX        1.05
#define SME_DEMUX_DTS_LINEUP_MIN        0.95

typedef struct _tagSmeStreamQueue
{
    AVPacketList* pstPktl;//frame list
    AVPacketList* pstPktlEnd;//the last;
    V_INT32 i32Len;//pkt len
    V_INT32 i32ValidNum;//pktlist 头部连续几个 有效timestamp的帧。
    V_INT32 i32LastAvgDuration;//time base;

    V_INT64 PrevValidDts[SME_DEMUX_PREV_DTS_NUM];
    int ValidDtsNum;
    int PrevValidDuration;
    AVPacket *DtsHopArray[SME_DEMUX_DTS_HOP_NUM];
    int DtsHopNum;

}ST_SME_STREAM_Q;

typedef struct _tagSmeUrlOptions
{
    V_CHAR *pszUserAgent;
    V_CHAR *pszReferer;
    V_CHAR *pszCookies;
    V_CHAR *pszHeaders;
}ST_SME_URL_OPTIONS;

typedef struct _tagSmeDemuxHdl
{
    V_CHAR *pszFileName;
    V_CHAR acMime[64];

    ST_SME_STREAMINFO stStreams;

    AVFormatContext *pstAVFmtCtx;
    SMEDEMUXFMT pvDemuxFmt;

    //private ffmpeg IO protocol to read data through gstreamer
    URLProtocol stURLProt;

    V_BOOL bOpened;
    V_BOOL bStopReadFf;
    V_INT64 i64BasePts[M_SME_STREAM_NUM_MAX];
    V_INT64 i64BaseDts[M_SME_STREAM_NUM_MAX];
    V_INT64 i64BasePtsMin;
    V_INT64 i64BaseDtsMin;
    V_INT64 i64LastTSInBase[M_SME_STREAM_NUM_MAX];
    V_INT64 i64LastDurationInBase[M_SME_STREAM_NUM_MAX];
    V_BOOL  bFirstAfterSeek[M_SME_STREAM_NUM_MAX];
    V_INT64 i64DeltaPts[M_SME_STREAM_NUM_MAX];
    V_INT64 i64DeltaDts[M_SME_STREAM_NUM_MAX];
    V_INT64 i64MaxPts[M_SME_STREAM_NUM_MAX];
    ST_SME_IOINTERRUPTCB stIOInterruptCb;
    V_BOOL  bNeedAddVideoFrameHeader;
    E_SME_DEMUX_RET eReadErr;//av read error;
    E_SME_DEMUX_RET eLastReadRes;//last SME Read return;
    ST_SME_STREAM_Q stStreamQ[M_SME_STREAM_NUM_MAX];
    V_UINT64 u64VideoPkts;
    V_UINT64 u64VideoDtsOutofOrderChangeCounts;
    V_UINT64 u64AudioDtsOutofOrderChangeCounts;
    ST_SME_URL_OPTIONS stUrlOptions;
}ST_SME_DEMUX_HDL, *PST_SME_DEMUX_HDL;


#ifdef SME_UT
#include "sme_demux_impl_ffmpeg_ut.h"
#define static
#define inline
#endif

static E_SME_DEMUX_RET ParseCfgData(PST_SME_DEMUX_HDL pstThis,
                                    const AVCodecContext* pstCodeCtx,
                                    ST_SME_CFG_DATA& stCfgData);


V_BOOL NeedAACAdtsHeader(const V_UINT8 *pBuffer, V_UINT32 uiCodecId)
{
    V_BOOL bNeedAdtsHeader = false;

    if ((uiCodecId == AV_CODEC_ID_AAC) &&
        (pBuffer[0] != 0xff || (pBuffer[1] & 0xf0) != 0xf0))
    {
        bNeedAdtsHeader = true;
    }

#if 0
    ICS_LOGI("codecId 0x%x, pbuffer[0] = 0x%x, "
                  "pbuffer[1] = 0x%x, bNeed = %d",
                  uiCodecId, pBuffer[0], pBuffer[1], bNeedAdtsHeader);
#endif

    return bNeedAdtsHeader;
}

static V_VOID FreeUrlOptions(ST_SME_URL_OPTIONS *pstUrlOptions)
{
    if (!pstUrlOptions)
        return;

    UTILS_MSAFEFREE(pstUrlOptions->pszUserAgent);
    UTILS_MSAFEFREE(pstUrlOptions->pszReferer);
    UTILS_MSAFEFREE(pstUrlOptions->pszCookies);
    UTILS_MSAFEFREE(pstUrlOptions->pszHeaders);
}

V_BOOL WriteAACAdtsHeader(V_UINT8 *pAdtsHeader,
                          V_UINT32 ui32Id,
                          V_UINT32 ui32Profile,
                          V_UINT32 ui32SampleRate,
                          V_UINT32 ui32ChannelConfig,
                          V_UINT32 ui32FrameLen,
                          V_UINT8 *pExtraData,
                          V_INT32 i32ExtraDataSize)
{
    V_UINT32 ui32SampRateIdx = 0;
    V_UINT32 ui32FsIdx = 0, ui32ObjType = 0, ui32ChanConf = 0;

    /* parse audio specific configure and get related information */
    if (pExtraData != NULL && i32ExtraDataSize > 1)
    {
        V_UINT8 *pBuf = pExtraData;
        ui32ObjType = (pBuf[0] & 0xf8) >> 3;
        if (ui32ObjType == 0x1f)
        {
            ICS_LOGE("Unsupported, escaped object type");
        }
        if (ui32ObjType < 4)
        {
            ui32Profile = ui32ObjType - 1;
        }

        ui32FsIdx = ((pBuf[0] & 0x7) << 1) | ((pBuf[1] & 0x80) >> 7);
        if (ui32FsIdx == 0xf)
        {
            ICS_LOGE("Unsupported, escaped sample rate index");
        }

        if (ui32FsIdx != 0xf)
        {
            ui32SampRateIdx = ui32FsIdx;
        }
        ui32ChanConf = (pBuf[1] & 0x78) >> 3;
        if (ui32ChanConf <= 8)
        {
            ui32ChannelConfig = ui32ChanConf;
        }
    }

#if 0
    ICS_LOGE("ui32Id 0x%x, ui32Profile = 0x%x, "
                  "ui32SampRateIdx = 0x%x, ui32ChannelConfig = 0x%x, "
                  "ui32FrameLen = 0x%x, object type 0x%x",
                  ui32Id, ui32Profile, ui32SampRateIdx,
                  ui32ChannelConfig, ui32FrameLen, ui32ObjType);
#endif

    if (ui32SampRateIdx == 0)
    {
        /* get sample rate index */
        switch (ui32SampleRate)
        {
            case 96000:
                ui32SampRateIdx = 0;
                break;
            case 882000:
                ui32SampRateIdx = 1;
                break;
            case 64000:
                ui32SampRateIdx = 2;
                break;
            case 48000:
                ui32SampRateIdx = 3;
                break;
            case 44100:
                ui32SampRateIdx = 4;
                break;
            case 32000:
                ui32SampRateIdx = 5;
                break;
            case 24000:
                ui32SampRateIdx = 6;
                break;
            case 22050:
                ui32SampRateIdx = 7;
                break;
            case 16000:
                ui32SampRateIdx = 8;
                break;
            case 12000:
                ui32SampRateIdx = 9;
                break;
            case 11025:
                ui32SampRateIdx = 10;
                break;
            case 8000:
                ui32SampRateIdx = 11;
                break;
            default:
                ui32SampRateIdx = 15;
                break;
        }
    }

    /* Sync point over a full byte */
    pAdtsHeader[0] = 0xFF;
    pAdtsHeader[1] = 0xF0;
    /* ID */
    pAdtsHeader[1] |= (ui32Id & 0x1) << 3;
    /* layer and protection bit, fix it to 00b and 1b */
    pAdtsHeader[1] |= 0x01;
    /* profile over first 2 bits */
    pAdtsHeader[2] = (ui32Profile & 0x3) << 6;
    /* rate index over next 4 bits */
    pAdtsHeader[2] |= (ui32SampRateIdx & 0xf) << 2;
    /* channels over last 2 bits */
    pAdtsHeader[2] |= (ui32ChannelConfig & 0x4) >> 2;
    /* channels continued over next 2 bits + 4 bits at zero */
    pAdtsHeader[3] = (ui32ChannelConfig & 0x3) << 6;
    /* frame size over last 2 bits */
    pAdtsHeader[3] |= (ui32FrameLen & 0x1800) >> 11;
    /* frame size continued over full byte */
    pAdtsHeader[4] = (ui32FrameLen & 0x7F8) >> 3;
    /* frame size continued first 3 bits */
    pAdtsHeader[5] = (ui32FrameLen & 0x7) << 5;
    /* buffer fullness (0x7FF for VBR) over 5 last bits */
    pAdtsHeader[5] |= 0x1F;
    /* buffer fullness (0x7FF for VBR) continued over 6 first bits + 2 zeros for
     * number of raw data blocks */
    pAdtsHeader[6] = 0xFC;

//    pAdtsHeader[6] |= 0x1;

    return true;
}

static ST_VOS_THD_MUTEX g_stffmpegMutex  = VOS_PTHREAD_MUTEX_INITIALIZER;
static URLProtocol* g_pstURLProtocol = NULL;
static PST_SME_MEDIA_FRAME AllocBufferAndCopyPacket(
    ST_SME_DEMUX_HDL *pstThis,
    const ST_SME_FRAMEBUFFER *pstBuffer,
    AVPacket *pstAVPkt,
    V_VOID *pvAllocCtx)
{
    E_SME_DEMUX_RET eSmeRet = E_SME_DEMUX_RET_SUCCESS;
    PST_SME_MEDIA_FRAME pstFrameTmp = NULL;
    AVCodecContext *pstCodeCtx = NULL;
    AVStream *pAvStream = NULL;
    V_BOOL bNeedAdtsHeader;
    V_INT32 i32ExtHdSize = 0;
    V_UINT32 i32CodecDataSize = 0;
    ST_SME_MEDIAINFO* pstMediaInfo = pstThis->stStreams.pstMedia[pstAVPkt->stream_index];

    pAvStream = pstThis->pstAVFmtCtx->streams[pstAVPkt->stream_index];
    pstCodeCtx = pAvStream->codec;

    bNeedAdtsHeader = NeedAACAdtsHeader(pstAVPkt->data, pstCodeCtx->codec_id);

    if (bNeedAdtsHeader)
    {
        i32ExtHdSize = 7;
    }
    else if((NULL != pstMediaInfo)
        && (E_SME_MEDIA_CODEC_ID_VC1 == pstMediaInfo->eCodecId)
        && (pstThis->bNeedAddVideoFrameHeader))
    {
        i32ExtHdSize = 4;
    }

    /** BEGIN: Added for DTS2015082904200  by baihaili(b00311320), 2015/8/29 */
    if ((NULL != pstMediaInfo) && (pstAVPkt->flags & AV_PKT_FLAG_CODEC_CHANGED))
    {
        eSmeRet = ParseCfgData((PST_SME_DEMUX_HDL)pstThis, pstCodeCtx, pstMediaInfo->stCfgData);
        UTILS_M_RET_VAL_IF(eSmeRet != E_SME_DEMUX_RET_SUCCESS,NULL);
        i32CodecDataSize = pstMediaInfo->stCfgData.u32CfgDataSize;
        ICS_LOGW("AVPkt codec data changed,codec size:%d",i32CodecDataSize);
        pstAVPkt->flags &= ~AV_PKT_FLAG_CODEC_CHANGED;
    }
    /* END:   Added by baihaili(b00311320), 2015/8/29 */

    //获取空Buffer
    /** get_bits() in ffmpeg would read at lest 4 bytes once a time
    and this would make it access 3 bytes over the space boundary*/
    pstFrameTmp = (PST_SME_MEDIA_FRAME)(pstBuffer->stAllocator.pfnLockBuf(
            pvAllocCtx,
            (V_UINT32)(sizeof(ST_SME_MEDIA_FRAME) +
                       pstAVPkt->size + i32ExtHdSize + i32CodecDataSize + 3),
            pstAVPkt->stream_index));

    if (pstFrameTmp == NULL)
    {
        return NULL;
    }

    //拷贝帧数据
    pstFrameTmp->eType =
        pstThis->stStreams.pstMedia[pstAVPkt->stream_index]->eMediaType;
    pstFrameTmp->i32StreamIdx = pstAVPkt->stream_index;
    pstFrameTmp->u32DataLen = (V_UINT32)pstAVPkt->size + i32ExtHdSize + i32CodecDataSize;

    if (E_SME_MEM_OK != VOS_Memcpy_S(
        pstFrameTmp->pu8DataBuf + i32ExtHdSize + i32CodecDataSize,
        (V_SIZE)pstAVPkt->size,
        pstAVPkt->data, (V_SIZE)pstAVPkt->size))
    {
        pstBuffer->stAllocator.pfnUnLockBuf(pvAllocCtx, pstFrameTmp);
        ICS_LOGE("SECURE: VOS_Memcpy_S return failed !!!");
        return NULL;
    }

    /** BEGIN: Added for DTS2015082904200  by baihaili(b00311320), 2015/8/29 */
    //Copy codec data if needed
    if ((NULL != pstMediaInfo) && i32CodecDataSize)
    {
        if (E_SME_MEM_OK != VOS_Memcpy_S(pstFrameTmp->pu8DataBuf,
                        (V_SIZE)i32CodecDataSize, pstMediaInfo->stCfgData.pu8CfgData, (V_SIZE)i32CodecDataSize))
        {
            pstBuffer->stAllocator.pfnUnLockBuf(pvAllocCtx, pstFrameTmp);
            ICS_LOGE("SECURE: VOS_Memcpy_S return failed !!!");
            return NULL;
        }
    }
    /* END:   Added by baihaili(b00311320), 2015/8/29 */

    if (bNeedAdtsHeader)
    {
        WriteAACAdtsHeader(pstFrameTmp->pu8DataBuf + i32CodecDataSize,
                           0, /* mpeg2 */
                           1, /* LC profile */
                           (unsigned int)pstCodeCtx->sample_rate,
                           (unsigned int)pstCodeCtx->channels,
                           pstFrameTmp->u32DataLen,
                           pstCodeCtx->extradata,
                           pstCodeCtx->extradata_size);
    }
    else if((NULL != pstMediaInfo)
        && (E_SME_MEDIA_CODEC_ID_VC1 == pstMediaInfo->eCodecId)
        && (pstThis->bNeedAddVideoFrameHeader))
    {
        pstFrameTmp->pu8DataBuf[i32CodecDataSize] = 0;
        pstFrameTmp->pu8DataBuf[1 + i32CodecDataSize] = 0;
        pstFrameTmp->pu8DataBuf[2 + i32CodecDataSize] = 1;
        pstFrameTmp->pu8DataBuf[3 + i32CodecDataSize] = 13;
    }

    /** BEGIN: Added for DTS2015082904200  by baihaili(b00311320), 2015/8/29 */
    if((NULL != pstMediaInfo)
       && (E_SME_MEDIA_CODEC_ID_H264 == pstMediaInfo->eCodecId)
       && (pstThis->bNeedAddVideoFrameHeader))
    {
        //ICS_LOGW("SME_AddNalHeaderToAvcData");
        SME_AddNalHeaderToAvcData(pstFrameTmp->pu8DataBuf + i32CodecDataSize,
                                  pstFrameTmp->u32DataLen - i32CodecDataSize);
    }
    else if((NULL != pstMediaInfo)
            && (E_SME_MEDIA_CODEC_ID_HEVC == pstMediaInfo->eCodecId)
            && pstThis->bNeedAddVideoFrameHeader)
    {
        //ICS_LOGW("SME_AddNalHeaderToHevcData");
        SME_AddNalHeaderToHevcData(pstFrameTmp->pu8DataBuf + i32CodecDataSize,
                                   pstFrameTmp->u32DataLen - i32CodecDataSize);
    }
    /* END:   Added by baihaili(b00311320), 2015/8/29 */
    return pstFrameTmp;
}

static V_BOOL CmpURLProtocol(const URLProtocol* pstURLProt1,
    const URLProtocol* pstURLProt2)
{
    return ((pstURLProt1->name == pstURLProt2->name)
        && (pstURLProt1->url_open == pstURLProt2->url_open)
        && (pstURLProt1->url_close == pstURLProt2->url_close)
        && (pstURLProt1->url_read == pstURLProt2->url_read)
        && (pstURLProt1->url_write == pstURLProt2->url_write)
        && (pstURLProt1->url_seek == pstURLProt2->url_seek));
}

static URLProtocol* CloneProtIfNotExist(const URLProtocol* pstURLProt)
{
    URLProtocol**       ppCur           = NULL;
    URLProtocol*        pstRet          = NULL;

    VOS_ThdMutexLock(&g_stffmpegMutex);
    ppCur = &g_pstURLProtocol;

    do
    {
        while(NULL != *ppCur)
        {
            UTILS_M_BRK_IF(CmpURLProtocol((const URLProtocol*)(*ppCur), pstURLProt));
            ppCur = &((*ppCur)->next);
        }

        UTILS_MLOGW_BRK_IF(NULL != *ppCur, ("already has this proto=%s",
            pstURLProt->name));

        ICS_LOGI("CloneProtIfNotExist a new proto = %s", pstURLProt->name);
        //need add this proto;
        pstRet = new URLProtocol;//进程消失时消失该内存。
        UTILS_MLOGE_BRK_IF(NULL == pstRet, ("new failed"));
        *pstRet = *pstURLProt;
        pstRet->next = NULL;
        *ppCur = pstRet;
    } while (ICS_FALSE);

    VOS_ThdMutexUnLock(&g_stffmpegMutex);

    return pstRet;
}

static V_VOID FreePacketBuffer(AVPacketList **pkt_buf, AVPacketList **pkt_buf_end)
{
    while (*pkt_buf) {
        AVPacketList *pktl = *pkt_buf;
        *pkt_buf = pktl->next;
        av_free_packet(&pktl->pkt);
        av_freep(&pktl);
    }
    *pkt_buf_end = NULL;
}


static void
sme_ffmpeg_log_callback (V_VOID *ptr, V_INT32 level, const V_CHAR *fmt, va_list vl)
{
    char acBuf[2048] = {0,};

    //ICS_LOGW("sme_ffmpeg_log_callback");

    (void)ptr;

    if (-1 == VOS_Vsnprintf_S(acBuf, sizeof(acBuf), sizeof(acBuf) - 1, fmt, vl))
    {
        ICS_LOGE("VOS_Vsnprintf_S return failed");
    }

    switch (level) {
    case AV_LOG_QUIET:
        ICS_LOGE("%s", acBuf);
        break;
    case AV_LOG_WARNING:
        ICS_LOGW("%s", acBuf);
        break;
    case AV_LOG_ERROR:
        ICS_LOGE("%s", acBuf);
        break;
    case AV_LOG_INFO:
        ICS_LOGI("%s", acBuf);
        break;
    case AV_LOG_DEBUG:
        ICS_LOGD("%s", acBuf);
        break;
    default:
        ICS_LOGE("%s", acBuf);
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////////
static inline V_BOOL IsPacketDtsValid(const AVPacket* pstPkt)
{
    return !!((AV_NOPTS_VALUE != pstPkt->dts) || (AV_NOPTS_VALUE != pstPkt->pts));
}

static inline V_INT64 GetPacketTimeStamp(const AVPacket* pstPkt)
{
    return (AV_NOPTS_VALUE != pstPkt->dts) ?  pstPkt->dts : pstPkt->pts;
}

//init DTS_Hop data
static void inline CleanDtsHop(ST_SME_STREAM_Q& stQ)
{
    V_INT32 i;

    for(i = 0; i < SME_DEMUX_PREV_DTS_NUM; i++)
        stQ.PrevValidDts[i] = -1;
    stQ.ValidDtsNum = 0;
    stQ.PrevValidDuration = 0;

    for(i = 0; i < SME_DEMUX_DTS_HOP_NUM; i++)
        stQ.DtsHopArray[i] = 0;
    stQ.DtsHopNum = 0;

    return;
}

static V_VOID FlushStreamQueue(ST_SME_STREAM_Q& stQ)
{
    stQ.i32ValidNum = 0;
    stQ.i32Len = 0;
    FreePacketBuffer(&stQ.pstPktl, &stQ.pstPktlEnd);

    //init DTS_Hop data
    CleanDtsHop(stQ);

    return;
}

static V_VOID InitStreamQueue(ST_SME_STREAM_Q& stQ)
{
    stQ.i32LastAvgDuration = 3003;
    stQ.i32ValidNum = 0;
    stQ.i32Len = 0;
    stQ.pstPktl = NULL;
    stQ.pstPktlEnd = NULL;

    //init DTS_Hop data
    CleanDtsHop(stQ);

    return;
}

static V_VOID DeInitStreamQueue(ST_SME_STREAM_Q& stQ)
{
    stQ.i32LastAvgDuration = 3003;
    stQ.i32Len = 0;
    FlushStreamQueue(stQ);

    //init DTS_Hop data
    CleanDtsHop(stQ);

    return;
}


/*
1.if pkt is null, we use st->last_avg_duration to compute all dts & duration.
2.if pkt is not null, we poll all queue && use dts between two dts to compute average
  duration. and than the front pkt->dts + i32LastAvgDuration.... until the back pkt.
  '|' is valid timestamp frame.
*/

static V_VOID RevisePktlistDts(ST_SME_STREAM_Q& stQ, V_BOOL bForeReviseAll)
{
    //AVPacketList **pstktl = &stQ.pstPktl;
    //AVPacketList **pstktlEnd = &stQ.pstPktlEnd;

    if(stQ.i32ValidNum == stQ.i32Len)
    {
        //no need to revise.
        return;
    }

    AVPacketList *BgValidPktl = NULL;
    AVPacketList *EdValidPktl = NULL;
    AVPacketList *pstPktl;
    V_BOOL bNeedCheckFirstPkt = ICS_TRUE;
    AVPacketList* pstPktlLast = NULL;
    V_INT32 i32Num = 0;

    stQ.i32ValidNum = 0;
    pstPktl = stQ.pstPktl;
    while (pstPktl)
    {
        if(AV_NOPTS_VALUE != pstPktl->pkt.pts
            || AV_NOPTS_VALUE != pstPktl->pkt.dts)
        {
            stQ.i32ValidNum++;
            if(!BgValidPktl)
            {
                BgValidPktl = pstPktl;
            }
            else if(!EdValidPktl)
            {
                EdValidPktl = pstPktl;
            }

            if(bNeedCheckFirstPkt && BgValidPktl && pstPktl != stQ.pstPktl)
            {
                //the first one is invalid, we should use BgValidPktl-st->last_avg_duration
                //----|
                AVPacketList *pstPktlCur = stQ.pstPktl;
                while(pstPktlCur)
                {
                    if(pstPktlCur == BgValidPktl)
                    {
                        break;
                    }

                    pstPktlCur->pkt.duration = stQ.i32LastAvgDuration;
                    pstPktlCur->pkt.dts = GetPacketTimeStamp(&BgValidPktl->pkt)
                        - (int64_t)i32Num * (int64_t)(stQ.i32LastAvgDuration);
                    i32Num--;
                    stQ.i32ValidNum++;
                    pstPktlCur = pstPktlCur->next;
                }

                if(i32Num)
                {
                    ICS_LOGW("i32Num = %d, it should be 0, why not?", i32Num);
                    i32Num = 0;
                }
            }

            bNeedCheckFirstPkt = ICS_FALSE;

            if(BgValidPktl && EdValidPktl)
            {
                // we should compute frame between these two.
                V_INT64 i64EndTs = GetPacketTimeStamp(&EdValidPktl->pkt);
                V_INT64 i64BeginTs = GetPacketTimeStamp(&BgValidPktl->pkt);

                if(i32Num > 1 && EdValidPktl != BgValidPktl->next)
                {
                    V_INT64 i64LastTs = i64BeginTs;
                    AVPacketList *pstPktlCur = BgValidPktl->next;
                    stQ.i32LastAvgDuration = (V_INT32)((i64EndTs - i64BeginTs) / i32Num);
                    while(pstPktlCur && pstPktlCur != EdValidPktl)
                    {
                        pstPktlCur->pkt.duration = stQ.i32LastAvgDuration;
                        pstPktlCur->pkt.dts = i64LastTs + pstPktlCur->pkt.duration;
                        i64LastTs = pstPktlCur->pkt.dts;
                        stQ.i32ValidNum++;
                        pstPktlCur = pstPktlCur->next;
                    }
                }
                BgValidPktl = EdValidPktl;
                EdValidPktl = NULL;
                i32Num = 0;
            }
        }

        i32Num++;
        pstPktlLast = pstPktl;
        pstPktl = pstPktl->next;
    }

    if(bForeReviseAll && pstPktlLast != BgValidPktl && BgValidPktl && !EdValidPktl){
      //|-----
        //BgValidPktl+stQ.i32LastAvgDuration
        V_INT64 i64LastTs = GetPacketTimeStamp(&BgValidPktl->pkt);
        AVPacketList *pstPktlCur = BgValidPktl->next;
        while(pstPktlCur)
        {
            pstPktlCur->pkt.duration = stQ.i32LastAvgDuration;
            pstPktlCur->pkt.dts = i64LastTs + pstPktlCur->pkt.duration;
            i64LastTs = pstPktlCur->pkt.dts;
            pstPktlCur = pstPktlCur->next;
            stQ.i32ValidNum++;
        }
        i32Num = 0;
    }

    return;
};

static V_BOOL ReadPktFromPktlist(ST_SME_STREAM_Q& stQ, AVPacket* pstPkt, V_BOOL bReadAll)
{
    V_BOOL bRet = ICS_TRUE;

    if((((stQ.i32ValidNum < 2 && 0 == stQ.DtsHopNum) || (stQ.i32ValidNum < 3 && stQ.DtsHopNum > 0))
        && !bReadAll) || (stQ.i32ValidNum < 1))
    {
        if (stQ.i32Len < SME_DEMUX_PACKET_QUEUE_MAX_LEN/2)
        {
            bRet = ICS_FALSE;
        }
    }

    if(bRet)
    {
        AVPacketList* pstCur = stQ.pstPktl;
        stQ.i32ValidNum--;
        stQ.i32Len--;
        *pstPkt = pstCur->pkt;
        if(!stQ.i32Len)
        {
            stQ.pstPktl = NULL;
            stQ.pstPktlEnd = NULL;
        }
        else
        {
            stQ.pstPktl = pstCur->next;
        }
        av_free(pstCur);
    }

    return bRet;
}

static V_VOID FlushStreamQueueArray(ST_SME_STREAM_Q stQ[], V_INT32 i32Num)
{
    for(V_INT32 i = 0; i < i32Num; i++)
    {
        FlushStreamQueue(stQ[i]);
    }

    return;
}

static V_VOID InitStreamQueueArray(ST_SME_STREAM_Q stQ[], V_INT32 i32Num)
{
    for(V_INT32 i = 0; i < i32Num; i++)
    {
       InitStreamQueue(stQ[i]);
    }

    return;
}

static V_VOID DeInitStreamQueueArray(ST_SME_STREAM_Q stQ[], V_INT32 i32Num)
{
    for(V_INT32 i = 0; i < i32Num; i++)
    {
        DeInitStreamQueue(stQ[i]);
    }

    return;
}

static V_VOID SME_ClearPktlist(ST_SME_DEMUX_HDL* pstThis, V_INT32 i32StreamIdx)
{
    ICS_LOGI("flush SME_ClearPktlist begin..., i32StreamIdx=%d", i32StreamIdx);

    if(i32StreamIdx < 0)
    {
        FlushStreamQueueArray(pstThis->stStreamQ,
            sizeof(pstThis->stStreamQ)/sizeof(pstThis->stStreamQ[0]));
    }
    else
    {
        FlushStreamQueue(pstThis->stStreamQ[i32StreamIdx]);
    }

    ICS_LOGI("flush SME_ClearPktlist end, i32StreamIdx=%d", i32StreamIdx);

    return;
}

static AVPacket *add_to_pktbuf(AVPacketList **packet_buffer, const AVPacket *pkt,
    AVPacketList **plast_pktl)
{
    AVPacketList *pktl = (AVPacketList *)av_mallocz(sizeof(AVPacketList));

    if (!pktl)
    {
        return NULL;
    }

    if (*packet_buffer)
        (*plast_pktl)->next = pktl;
    else
        *packet_buffer = pktl;

    /* Add the packet in the buffered packet list. */
    *plast_pktl = pktl;
    pktl->pkt   = *pkt;

    return &pktl->pkt;
}

/////////////////////////////////////////////////////////////////////////////////////////////////



// return value:
//   1, PrevValidDts and PrevValidDuration is available to check DTS hop
//   0, not available.

static V_BOOL inline IsOkPrevValidDts(const ST_SME_STREAM_Q *pStreamQueue)
{
    if ((pStreamQueue->ValidDtsNum == SME_DEMUX_PREV_DTS_NUM)
        && (pStreamQueue->PrevValidDuration > 0))
    {
        // PrevValidDts and PrevValidDuration is available

        //ICS_LOGI(
        //    "[PrevValidDts_yes]: PrevValidDts=[%lld, %lld, %lld] "
        //    "PrevValidDuration=%d ValidDtsNum=3",
        //    pStreamQueue->PrevValidDts[0],
        //    pStreamQueue->PrevValidDts[1],
        //    pStreamQueue->PrevValidDts[2],
        //    pStreamQueue->PrevValidDuration);

        return ICS_TRUE;
    }

    //ICS_LOGI(
    //    "[PrevValidDts_no]: PrevValidDts=[%lld, %lld, %lld]"
    //    "PrevValidDuration=%d ValidDtsNum=%d",
    //    pStreamQueue->PrevValidDts[0],
    //    pStreamQueue->PrevValidDts[1],
    //    pStreamQueue->PrevValidDts[2],
    //    pStreamQueue->PrevValidDuration,
    //    pStreamQueue->ValidDtsNum);

    return ICS_FALSE;
}


// Function:
//   Get valid continuous DTS and record it.
//   Not cover the case when DTS decrease.
static void inline RecordPrevValidDts(ST_SME_STREAM_Q *pStreamQueue, const AVPacket* pstAVPkt)
{
    //ICS_LOGI(
    //    "[RecordValidDts_in]: index=%d, PrevValidDts=[%lld, %lld, %lld], in_DTS=%lld "
    //    "PrevValidDuration=%d ValidDtsNum=%d",
    //    pstAVPkt->stream_index,
    //    pStreamQueue->PrevValidDts[0],
    //    pStreamQueue->PrevValidDts[1],
    //    pStreamQueue->PrevValidDts[2],
    //    pstAVPkt->dts,
    //    pStreamQueue->PrevValidDuration,
    //    pStreamQueue->ValidDtsNum);


    switch(pStreamQueue->ValidDtsNum)
    {
        case 0:
            if (pstAVPkt->dts >= 0)
            {
                pStreamQueue->PrevValidDts[0] = pstAVPkt->dts;
                pStreamQueue->ValidDtsNum = 1;

                if (pstAVPkt->duration > 0) {
                    pStreamQueue->PrevValidDuration = pstAVPkt->duration;
                }
            }

            break;

        case 1:
            if (pstAVPkt->dts >= 0
                && (pstAVPkt->dts > pStreamQueue->PrevValidDts[0]))
            {
                pStreamQueue->PrevValidDts[1] = pstAVPkt->dts;
                pStreamQueue->PrevValidDuration = (int)(pstAVPkt->dts -
                    pStreamQueue->PrevValidDts[0]);

                pStreamQueue->ValidDtsNum ++;
            } else {
                pStreamQueue->ValidDtsNum = 0;
                pStreamQueue->PrevValidDuration = 0;
            }

            break;

        case 2:
            if (pstAVPkt->dts >= 0
                && (pstAVPkt->dts > pStreamQueue->PrevValidDts[1]))
            {
                int dts_increase =
                    (int)(pstAVPkt->dts - pStreamQueue->PrevValidDts[1]);

                if ((dts_increase <  pStreamQueue->PrevValidDuration * SME_DEMUX_DTS_LINEUP_MAX)
                    && (dts_increase >  pStreamQueue->PrevValidDuration * SME_DEMUX_DTS_LINEUP_MIN))
                {
                    pStreamQueue->PrevValidDts[2] = pstAVPkt->dts;
                    pStreamQueue->ValidDtsNum ++;

                    pStreamQueue->PrevValidDuration =
                        (dts_increase + pStreamQueue->PrevValidDuration) / 2;
                } else {
                    pStreamQueue->ValidDtsNum = 0;
                    pStreamQueue->PrevValidDuration = 0;
                }

            } else {
                pStreamQueue->ValidDtsNum = 0;
                pStreamQueue->PrevValidDuration = 0;
            }

            break;

        case 3:
            if (pstAVPkt->dts >= 0
                && (pstAVPkt->dts > pStreamQueue->PrevValidDts[2]))
            {
                int dts_increase =
                    (int)(pstAVPkt->dts - pStreamQueue->PrevValidDts[2]);

                if ((dts_increase <  pStreamQueue->PrevValidDuration * SME_DEMUX_DTS_LINEUP_MAX)
                    && (dts_increase >  pStreamQueue->PrevValidDuration * SME_DEMUX_DTS_LINEUP_MIN))
                {
                    pStreamQueue->PrevValidDuration =
                        (int)((pstAVPkt->dts - pStreamQueue->PrevValidDts[0])/3);

                    pStreamQueue->PrevValidDts[0] = pStreamQueue->PrevValidDts[1];
                    pStreamQueue->PrevValidDts[1] = pStreamQueue->PrevValidDts[2];
                    pStreamQueue->PrevValidDts[2] = pstAVPkt->dts;
                } else {
                    pStreamQueue->ValidDtsNum = 0;
                    pStreamQueue->PrevValidDuration = 0;
                }
            } else {
                pStreamQueue->ValidDtsNum = 0;
                pStreamQueue->PrevValidDuration = 0;
            }

            break;

        default:
            ICS_LOGE("[RecordValidDts], check, should_not_go_here.");

            // CleanDtsHop and rebuild PrevValidDTS and PrevValidDuration
            pStreamQueue->DtsHopNum = 0;
            pStreamQueue->ValidDtsNum = 0;
            pStreamQueue->PrevValidDuration = 0;

            break;

    }

    //ICS_LOGI(
    //    "[RecordValidDts_out]: index=%d, PrevValidDts=[%lld, %lld, %lld], in_DTS=%lld "
    //    "PrevValidDuration=%d ValidDtsNum=%d",
    //    pstAVPkt->stream_index,
    //    pStreamQueue->PrevValidDts[0],
    //    pStreamQueue->PrevValidDts[1],
    //    pStreamQueue->PrevValidDts[2],
    //    pstAVPkt->dts,
    //    pStreamQueue->PrevValidDuration,
    //    pStreamQueue->ValidDtsNum);

    return;
}

//
// Only when IsOkPrevValidDts return 1, this function could be called.
//
// return value:
//   1,  found dts hop
//   0,  dts is continuous,
//   -1, could not check if it is continuous.
//     if DTS is not fetched, return 0.
//
static V_INT32 CheckDtsHop(ST_SME_STREAM_Q *pStreamQueue, AVPacket* pstAVPkt)
{
    V_INT32 cur_duration = 0;
    V_INT32 ret = 0;

    if (AV_NOPTS_VALUE == pstAVPkt->dts || pstAVPkt->dts <= pStreamQueue->PrevValidDts[2])
    {
        // when dts decrease, do not handle it now;

        // CleanDtsHop and rebuild PrevValidDTS and PrevValidDuration
        pStreamQueue->DtsHopNum = 0;
        pStreamQueue->ValidDtsNum = 0;
        pStreamQueue->PrevValidDuration = 0;

        return -1;
    }

    switch(pStreamQueue->DtsHopNum)
    {
        case 0:
            cur_duration = (int)(pstAVPkt->dts - pStreamQueue->PrevValidDts[2]);

            // check DTS hop
            if (cur_duration > pStreamQueue->PrevValidDuration * SME_DEMUX_DTS_HOP_THRESH_HOLD)
            {
                // hold it in the queue to watch
                pStreamQueue->DtsHopArray[pStreamQueue->DtsHopNum] = pstAVPkt;
                pStreamQueue->DtsHopNum ++;

                ICS_LOGI(
                    "raw_DTS_hop: index=%d, dts=%lld, duration=%d, pos=%lld, "
                    "PrevValidDts[2]=%lld, PrevValidDuration=%d, DtsHopArray[%d]=%p, DtsHopNum=%d",
                    pstAVPkt->stream_index,
                    pstAVPkt->dts, cur_duration, pstAVPkt->pos,
                    pStreamQueue->PrevValidDts[2], pStreamQueue->PrevValidDuration,
                    pStreamQueue->DtsHopNum - 1, pstAVPkt, pStreamQueue->DtsHopNum);

                ret = 1;
            }

            break;

         case 1:
            cur_duration =
                (int)(pstAVPkt->dts - pStreamQueue->PrevValidDts[2]) / 2;

            // check if DTS is linely
            if ((cur_duration >=  pStreamQueue->PrevValidDuration * SME_DEMUX_DTS_LINEUP_MAX)
                || (cur_duration <=  pStreamQueue->PrevValidDuration * SME_DEMUX_DTS_LINEUP_MIN))
            {
                // hold it in the queue to watch
                pStreamQueue->DtsHopArray[pStreamQueue->DtsHopNum] = pstAVPkt;
                pStreamQueue->DtsHopNum ++;

                ICS_LOGI(
                    "raw_DTS_hop: index=%d, dts=%lld, duration=%d, pos=%lld, "
                    "PrevValidDts[2]=%lld, PrevValidDuration=%d, DtsHopArray[%d]=%p, DtsHopNum=%d",
                    pstAVPkt->stream_index,
                    pstAVPkt->dts, cur_duration, pstAVPkt->pos,
                    pStreamQueue->PrevValidDts[2], pStreamQueue->PrevValidDuration,
                    pStreamQueue->DtsHopNum - 1, pstAVPkt, pStreamQueue->DtsHopNum);

                ret = 1;

            }

            break;

        case 2:
            cur_duration =
                (int)(pstAVPkt->dts - pStreamQueue->PrevValidDts[2]) / 3;

            // check if DTS is linely
            if ((cur_duration >=  pStreamQueue->PrevValidDuration * SME_DEMUX_DTS_LINEUP_MAX)
                || (cur_duration <=  pStreamQueue->PrevValidDuration * SME_DEMUX_DTS_LINEUP_MIN))
            {
                ICS_LOGI("raw_DTS_hop: DtsHopNum=%d, handle it as new "
                    "DTS continuous stream, dts=%lld",
                    pStreamQueue->DtsHopNum + 1, pstAVPkt->dts);

                // CleanDtsHop and rebuild PrevValidDTS and PrevValidDuration
                pStreamQueue->DtsHopNum = 0;
                pStreamQueue->ValidDtsNum = 0;
                pStreamQueue->PrevValidDuration = 0;

                ret = 0;
            }

            break;

        default:
            ICS_LOGE("[CheckDtsHop], check, should_not_go_here.");

            // CleanDtsHop and rebuild PrevValidDTS and PrevValidDuration
            pStreamQueue->DtsHopNum = 0;
            pStreamQueue->ValidDtsNum = 0;
            pStreamQueue->PrevValidDuration = 0;

            ret = 0;
            break;
    }

    return ret;
}

static void AdjustDtsHop(ST_SME_STREAM_Q *pStreamQueue)
{
    V_INT64 old_dts = -1;
    AVPacket * pAVPacket = 0;
    V_INT32 i = 0;

    if (pStreamQueue->ValidDtsNum != SME_DEMUX_PREV_DTS_NUM) {
        //should not go here
        ICS_LOGW("AdjustDtsHop: ValidDtsNum=%d", pStreamQueue->ValidDtsNum);
        return;
    }

    for (i = 0; i < pStreamQueue->DtsHopNum; i ++) {
        pAVPacket = pStreamQueue->DtsHopArray[i];

        old_dts = pAVPacket->dts;
        pAVPacket->dts = (pStreamQueue->PrevValidDts[2])
            + ((int64_t)(pStreamQueue->PrevValidDuration) * ((int64_t)i + 1));

        ICS_LOGI(
            "Adjust_DtsHop_Done: index=%d, old_dts=%lld, revised to %lld, pos=%lld "
            "DtsHopArray[%d]=%p",
            pAVPacket->stream_index, old_dts, pAVPacket->dts, pAVPacket->pos, i, pAVPacket);

        pStreamQueue->DtsHopArray[i] = 0;
        //pStreamQueue->i32ValidNum ++;
    }

    // clean hop record
    pStreamQueue->DtsHopNum = 0;

    // will rebuild PrevValidDTS and PrevValidDuration
    pStreamQueue->ValidDtsNum = 0;
    pStreamQueue->PrevValidDuration = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

static V_VOID SME_AddPktToPktlist(ST_SME_DEMUX_HDL* pstThis,
    AVPacket* pstAVPkt, const enum AVMediaType mediaType)
{
    V_INT32 is_DTS_hop = 0;
    AVPacket * pAVPacketInQueue =
        add_to_pktbuf(&pstThis->stStreamQ[pstAVPkt->stream_index].pstPktl, pstAVPkt,
                      &pstThis->stStreamQ[pstAVPkt->stream_index].pstPktlEnd);

    if(!pAVPacketInQueue)
    {
        ICS_LOGE("add to buff failed");
        av_free_packet(pstAVPkt);

        return;
    }

    if (AVMEDIA_TYPE_VIDEO == mediaType && IsOkPrevValidDts(&pstThis->stStreamQ[pstAVPkt->stream_index])) {
        is_DTS_hop = CheckDtsHop(&pstThis->stStreamQ[pstAVPkt->stream_index],
                                pAVPacketInQueue);
    }

    if (AVMEDIA_TYPE_VIDEO == mediaType && 0 == is_DTS_hop
        && pstThis->stStreamQ[pstAVPkt->stream_index].DtsHopNum > 0)
    {
        //if current DTS does not hop, but previous DTS is hopping, it is time
        //to adjust.
        AdjustDtsHop(&pstThis->stStreamQ[pstAVPkt->stream_index]);
    }

    if(0 == is_DTS_hop && IsPacketDtsValid(pstAVPkt)
        && (pstThis->stStreamQ[pstAVPkt->stream_index].i32Len
          == pstThis->stStreamQ[pstAVPkt->stream_index].i32ValidNum))
    {
        pstThis->stStreamQ[pstAVPkt->stream_index].i32ValidNum++;
    }

    pstThis->stStreamQ[pstAVPkt->stream_index].i32Len++;

    if (AVMEDIA_TYPE_VIDEO == mediaType && 0 == is_DTS_hop)
        RecordPrevValidDts(&pstThis->stStreamQ[pstAVPkt->stream_index], pstAVPkt);

    //if exceed more than 100, we dorp the HEAD
    if(pstThis->stStreamQ[pstAVPkt->stream_index].i32Len > SME_DEMUX_PACKET_QUEUE_MAX_LEN)
    {
        pstThis->stStreamQ[pstAVPkt->stream_index].i32Len--;
        AVPacketList* pstHead = pstThis->stStreamQ[pstAVPkt->stream_index].pstPktl;
        if(pstHead)
        {
            pstThis->stStreamQ[pstAVPkt->stream_index].pstPktl = pstHead->next;

            if(IsPacketDtsValid(&pstHead->pkt))
            {
                pstThis->stStreamQ[pstAVPkt->stream_index].i32ValidNum--;
            }

            av_free_packet(&pstHead->pkt);
            av_free(pstHead);
            ICS_LOGW("quque:idx=%d exceed 100,we drop the head", pstAVPkt->stream_index);
        }

    }

    return;
}

static V_VOID SME_RevisePktlistDts(ST_SME_DEMUX_HDL* pstThis, V_INT32 i32StreamIdx,
    V_BOOL bForeReviseAll)
{
    if(i32StreamIdx < 0)
    {
        V_INT32 i32StreamQNum = sizeof(pstThis->stStreamQ)/sizeof(pstThis->stStreamQ[0]);

        //all stream
        for(V_INT32 i = 0; i < i32StreamQNum; i++)
        {
            RevisePktlistDts(pstThis->stStreamQ[i], bForeReviseAll);
        }
    }
    else
    {
        RevisePktlistDts(pstThis->stStreamQ[i32StreamIdx], bForeReviseAll);
    }

    return;
}

static  V_BOOL SME_ReadFrameFromePktlist(ST_SME_DEMUX_HDL* pstThis, AVPacket* pstPkt,
    V_INT32 i32StreamIdx)
{
    V_BOOL bRet = ICS_TRUE;
    V_BOOL bReadAll = E_SME_DEMUX_RET_SUCCESS != pstThis->eReadErr ? ICS_TRUE : ICS_FALSE;

    if(i32StreamIdx < 0)
    {
        //poll all stQ.
        V_INT32 i32StreamQNum = sizeof(pstThis->stStreamQ)/sizeof(pstThis->stStreamQ[0]);

        //all stream
        for(V_INT32 i = 0; i < i32StreamQNum; i++)
        {
            bRet = ReadPktFromPktlist(pstThis->stStreamQ[i], pstPkt, bReadAll);
            if(bRet)
            {
                break;
            }
        }
    }
    else
    {
        bRet = ReadPktFromPktlist(pstThis->stStreamQ[i32StreamIdx], pstPkt, bReadAll);
    }

    return bRet;
}

///////////////////////////////////////////////////////////////////////////////////
static V_VOID FFCodecidToMime(const AVCodecContext *codecCtx,
	V_CHAR *pszMime, V_UINT32 len)
{
    E_VOS_STR_ERR ret = E_VOS_STR_ENONE;

    switch(codecCtx->codec_id)
    {
        case CODEC_ID_MPEG1VIDEO:
        case CODEC_ID_MPEG2VIDEO:
            ret = VOS_Strncpy_S(
                pszMime,
                (V_SIZE)len,
                "video/mpeg",
                (V_SIZE)strlen("video/mpeg"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case CODEC_ID_H263:
            ret = VOS_Strncpy_S(
                pszMime,
                (V_SIZE)len,
                "video/x-h263",
                (V_SIZE)strlen("video/x-h263"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case CODEC_ID_MJPEG:
        case CODEC_ID_LJPEG:
            ret = VOS_Strncpy_S(
                pszMime,
                (V_SIZE)len,
                "image/jpeg",
                (V_SIZE)strlen("image/jpeg"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }
            break;
        case CODEC_ID_MPEG4:
            switch (codecCtx->codec_tag) {
                case M_SME_MAKE_FOURCC ('D', 'I', 'V', 'X'):
                    ret = VOS_Strncpy_S(
                        pszMime,
                        (V_SIZE)len,
                        "video/x-divx",
                        (V_SIZE)strlen("video/x-divx"));

                    if (E_VOS_STR_ENONE != ret)
                    {
                        ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
                    }

                    break;
                case M_SME_MAKE_FOURCC ('m', 'p', '4', 'v'):
                default:
                    ret = VOS_Strncpy_S(
                        pszMime,
                        (V_SIZE)len,
                        "video/mpeg",
                        (V_SIZE)strlen("video/mpeg"));

                    if (E_VOS_STR_ENONE != ret)
                    {
                        ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
                    }

                    break;
            }
            break;
        case CODEC_ID_H264:
            ret = VOS_Strncpy_S(
                pszMime,
                (V_SIZE)len,
                "video/x-h264",
                (V_SIZE)strlen("video/x-h264"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case CODEC_ID_AMR_NB:
            ret = VOS_Strncpy_S(
                pszMime,
                (V_SIZE)len,
                "audio/AMR-NB",
                (V_SIZE)strlen("audio/AMR-NB"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case CODEC_ID_AMR_WB:
            ret = VOS_Strncpy_S(
                pszMime,
                (V_SIZE)len,
                "audio/AMR-WB",
                (V_SIZE)strlen("audio/AMR-WB"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case CODEC_ID_MP2:
        case CODEC_ID_MP3:
        case CODEC_ID_AAC:
            ret = VOS_Strncpy_S(
                pszMime,
                (V_SIZE)len,
                "audio/mpeg",
                (V_SIZE)strlen("audio/mpeg"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case CODEC_ID_VORBIS:
            ret = VOS_Strncpy_S(
                pszMime,
                (V_SIZE)len,
                "audio/x-vorbis",
                (V_SIZE)strlen("audio/x-vorbis"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case CODEC_ID_AC3:
            ret = VOS_Strncpy_S(
                pszMime,
                (V_SIZE)len,
                "audio/ac3",
                (V_SIZE)strlen("audio/ac3"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case AV_CODEC_ID_PCM_S16LE:
        case AV_CODEC_ID_PCM_S16BE:
        case AV_CODEC_ID_PCM_U16LE:
        case AV_CODEC_ID_PCM_U16BE:
        case AV_CODEC_ID_PCM_S8:
        case AV_CODEC_ID_PCM_U8:
        case AV_CODEC_ID_PCM_MULAW:
        case AV_CODEC_ID_PCM_ALAW:
        case AV_CODEC_ID_PCM_S32LE:
            //	case AV_CODEC_ID_PCM_S32BE:
            //	case AV_CODEC_ID_PCM_U32LE:
            //  case AV_CODEC_ID_PCM_U32BE:
        case AV_CODEC_ID_PCM_S24LE:
            //  case AV_CODEC_ID_PCM_S24BE:
            //	case AV_CODEC_ID_PCM_U24LE:
            //	case AV_CODEC_ID_PCM_U24BE:
            ret = VOS_Strncpy_S(
                pszMime,
                (V_SIZE)len,
                "audio/x-pcm",
                (V_SIZE)strlen("audio/x-pcm"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case AV_CODEC_ID_ADPCM_IMA_WAV:
            ret = VOS_Strncpy_S(
                pszMime,
                (V_SIZE)len,
                "audio/x-adpcm_ima_wav",
                (V_SIZE)strlen("audio/x-adpcm_ima_wav"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case AV_CODEC_ID_ADPCM_MS:
            ret = VOS_Strncpy_S(
                    pszMime,
                    (V_SIZE)len,
                    "audio/x-adpcm_ms",
                    (V_SIZE)strlen("audio/x-adpcm_ms"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case AV_CODEC_ID_GSM:
            ret = VOS_Strncpy_S(
                    pszMime,
                    (V_SIZE)len,
                    "audio/x-gsm",
                    (V_SIZE)strlen("audio/x-gsm"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case AV_CODEC_ID_GSM_MS:
            ret = VOS_Strncpy_S(
                    pszMime,
                    (V_SIZE)len,
                    "audio/x-gsm_ms",
                    (V_SIZE)strlen("audio/x-gsm_ms"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        case AV_CODEC_ID_DTS:
            ret = VOS_Strncpy_S(
                    pszMime,
                    (V_SIZE)len,
                    "audio/x-dts",
                    (V_SIZE)strlen("audio/x-dts"));

            if (E_VOS_STR_ENONE != ret)
            {
                ICS_LOGE("VOS_Strncpy_S ret=%d", ret);
            }

            break;
        default:
            break;
    }

    return;
}

static E_SME_MEDIA_CODEC_ID FFCodecidToSmeCodecid(AVCodecID eFFCodecId)
{
    return (E_SME_MEDIA_CODEC_ID)eFFCodecId;
}

static E_SME_AUDIO_RAW_FMT FFSampleFmtToSmeAudFmt(AVSampleFormat eFFSampleFmt)
{
    return (E_SME_AUDIO_RAW_FMT)eFFSampleFmt;
}

static V_VOID FreeStreams(PST_SME_STREAMINFO pstreams)
{
	V_UINT32 u32Index;

	UTILS_MLOGE_RET_IF((!pstreams || (pstreams->u32NumStream == 0)), ("no streams."));

	for(u32Index = 0; u32Index < pstreams->u32NumStream; u32Index++)
	{
	    /** BEGIN: Added for DTS2014061909146 by liurongliang(l00180035), 2014/6/28 */
        /*mem leak*/
	    if(NULL != pstreams->pstMedia[u32Index])
        {
            UTILS_MSAFEFREE(pstreams->pstMedia[u32Index]->stCfgData.pu8CfgData);
            pstreams->pstMedia[u32Index]->stCfgData.u32CfgDataSize = 0;
        }
	    /** END:   Added for DTS2014061909146 by liurongliang(l00180035), 2014/6/28 */
        UTILS_MSAFEFREE(pstreams->pstMedia[u32Index]);
	}

    pstreams->u32NumStream = 0;

    return;
}

static E_SME_DEMUX_RET ParseCfgDataOfHEVC(PST_SME_DEMUX_HDL pstThis,
    const AVCodecContext* pstCodeCtx, ST_SME_CFG_DATA& stCfgData)
{
    E_SME_DEMUX_RET     eSmeRet         = E_SME_DEMUX_RET_SUCCESS;
    V_UINT32            u32ExtraLen     = 0;
    ST_SME_CFG_DATA     stCfgDataTmp ;
    ST_HEVCNALCFG_LIST  stHevcCfgSet;
    V_BOOL              bRes            = ICS_FALSE;

    ICS_LOGD("ParseCfgDataOfHEVC, pstCodeCtx->codec_id=%d", pstCodeCtx->codec_id);
    do
    {
        if (E_SME_MEM_OK != VOS_Memset_S(&stHevcCfgSet, sizeof(stHevcCfgSet), 0, sizeof(stHevcCfgSet)))
        {
            eSmeRet = E_SME_DEMUX_RET_NOMEM;

            ICS_LOGE("VOS_Memset_S return failed !!!");
            break;
        }

        stCfgDataTmp.pu8CfgData = pstCodeCtx->extradata;
        stCfgDataTmp.u32CfgDataSize = (V_UINT32)(pstCodeCtx->extradata_size);
        bRes = SME_HevcRawCfgDataToNal(stCfgDataTmp, &stHevcCfgSet);
        if(!bRes)
        {
            ICS_LOGW("SME_HevcRawCfgDataToNal failed,try use raw");
            u32ExtraLen =(V_UINT32)(pstCodeCtx->extradata_size);
            UTILS_MLOGE_BRK_VAL_IF(0 == u32ExtraLen, eSmeRet, E_SME_DEMUX_RET_INVALID_ARG,
                ("extralen = 0"));
            V_UINT8* pu8CfgData = (V_UINT8*)malloc(u32ExtraLen);
            UTILS_MLOGE_BRK_VAL_IF(!pu8CfgData, eSmeRet, E_SME_DEMUX_RET_NOMEM, ("no memory."));

            if (E_SME_MEM_OK != VOS_Memset_S(pu8CfgData, u32ExtraLen, 0, u32ExtraLen))
            {
                eSmeRet = E_SME_DEMUX_RET_NOMEM;
                free(pu8CfgData);

                ICS_LOGE("VOS_Memset_S return failed !!!");
                break;
            }

            if (E_SME_MEM_OK != VOS_Memcpy_S(pu8CfgData, u32ExtraLen, pstCodeCtx->extradata, (V_SIZE)pstCodeCtx->extradata_size))
            {
                eSmeRet = E_SME_DEMUX_RET_NOMEM;
                free(pu8CfgData);

                ICS_LOGE("VOS_Memcpy_S return failed !!!");
                break;
            }

            stCfgData.u32CfgDataSize = (V_UINT32)pstCodeCtx->extradata_size;
            stCfgData.pu8CfgData = pu8CfgData;
            if((pstCodeCtx->extradata_size >= 3)
                && (!pstCodeCtx->extradata[0] && !pstCodeCtx->extradata[1]
                    && pstCodeCtx->extradata[2] <= 1))
            {
                pstThis->bNeedAddVideoFrameHeader = ICS_FALSE;
            }
            else
            {
                pstThis->bNeedAddVideoFrameHeader = ICS_TRUE;
            }
            break;
        }
        ICS_LOGW("SME_HevcRawCfgDataToNal cnt=%d", stHevcCfgSet.u32Count);

        /* BEGIN: Added  for DTS2015071608573 by liurongliang(l00180035), 2015/7/17 */
        pstThis->bNeedAddVideoFrameHeader = ICS_TRUE;
        UTILS_MLOGW_BRK_VAL_IF(0 == stHevcCfgSet.u32Count, eSmeRet, E_SME_DEMUX_RET_SUCCESS,
            ("no cfg data"));
        /* END:   Added  for DTS2015071608573 by liurongliang(l00180035), 2015/7/17 */
        //for fortify
        V_UINT64 u64ExtraLenTmp = (V_UINT64)u32ExtraLen;
        for(V_UINT32 j = 0; j < stHevcCfgSet.u32Count; j++)
        {
           //u32ExtraLen += stHevcCfgSet.pstItem[j]->u32Len;
           u64ExtraLenTmp += (V_UINT64)stHevcCfgSet.pstItem[j]->u32Len;
        }
        u32ExtraLen = (V_UINT32)u64ExtraLenTmp;
        UTILS_MLOGE_BRK_VAL_IF((M_SME_UINT32_MAX < u64ExtraLenTmp) || (0 == u32ExtraLen),
            eSmeRet, E_SME_DEMUX_RET_INVALID_ARG, ("extradata len overflow"));

        V_UINT8* pu8CfgData = (V_UINT8*)malloc(u32ExtraLen);
        UTILS_MLOGE_BRK_VAL_IF(!pu8CfgData, eSmeRet, E_SME_DEMUX_RET_NOMEM, ("no memory."));

        if (E_SME_MEM_OK != VOS_Memset_S(pu8CfgData, u32ExtraLen, 0, u32ExtraLen))
        {
            eSmeRet = E_SME_DEMUX_RET_NOMEM;
            free(pu8CfgData);
            ICS_LOGE("VOS_Memset_S return failed !!!");
            break;
        }

        u32ExtraLen = 0;
        for(V_UINT32 j = 0; j < stHevcCfgSet.u32Count; j++)
        {
            if (E_SME_MEM_OK != VOS_Memcpy_S(pu8CfgData + u32ExtraLen,
                (V_SIZE)stHevcCfgSet.pstItem[j]->u32Len,
                stHevcCfgSet.pstItem[j]->au8Cfgs,
                (V_SIZE)stHevcCfgSet.pstItem[j]->u32Len))
            {
                eSmeRet = E_SME_DEMUX_RET_NOMEM;
                ICS_LOGE("VOS_Memcpy_S return failed !!!");
            }

            u32ExtraLen += stHevcCfgSet.pstItem[j]->u32Len;
        }

        stCfgData.u32CfgDataSize = u32ExtraLen;
        /** BEGIN: Added for DTS2015082904200  by baihaili(b00311320), 2015/8/29 */
        if (stCfgData.pu8CfgData)
            free(stCfgData.pu8CfgData);
        /* END:   Added by baihaili(b00311320), 2015/8/29 */
        stCfgData.pu8CfgData = pu8CfgData;
    } while ( ICS_FALSE );

    for(V_UINT32 j = 0; j < stHevcCfgSet.u32Count ; j++)
    {
       UTILS_MSAFEFREE(stHevcCfgSet.pstItem[j]);
    }

    return eSmeRet;
}

static E_SME_DEMUX_RET ParseCfgDataOfAVC(PST_SME_DEMUX_HDL pstThis,
    const AVCodecContext* pstCodeCtx, ST_SME_CFG_DATA& stCfgData)
{
    E_SME_DEMUX_RET     eSmeRet         = E_SME_DEMUX_RET_SUCCESS;
    V_UINT32            u32ExtraLen     = 0;
    ST_SME_CFG_DATA     stCfgDataTmp ;
    ST_AVCNALCFG_LIST   stAvcCfgSet;
    V_BOOL              bRes            = ICS_FALSE;

    ICS_LOGI("ParseCfgDataOfAVC, pstCodeCtx->codec_id=%d", pstCodeCtx->codec_id);
    do
    {
        if (E_SME_MEM_OK != VOS_Memset_S(&stAvcCfgSet, sizeof(stAvcCfgSet), 0, sizeof(stAvcCfgSet)))
        {
            eSmeRet = E_SME_DEMUX_RET_NOMEM;

            ICS_LOGE("VOS_Memset_S return failed !!!");
            break;
        }

        stCfgDataTmp.pu8CfgData = pstCodeCtx->extradata;
        stCfgDataTmp.u32CfgDataSize = (V_UINT32)pstCodeCtx->extradata_size;
        bRes = SME_AvcRawCfgDataToNal(stCfgDataTmp, &stAvcCfgSet, NULL);
        if(!bRes)
        {
            ICS_LOGW("SME_AvcRawCfgDataToNal failed,try use raw");
            if((pstCodeCtx->extradata_size >= 3)
                && (!pstCodeCtx->extradata[0] && !pstCodeCtx->extradata[1]
                    && pstCodeCtx->extradata[2] <= 1))
            {
                pstThis->bNeedAddVideoFrameHeader = ICS_FALSE;
            }
            else
            {
                pstThis->bNeedAddVideoFrameHeader = ICS_TRUE;
            }
            u32ExtraLen = (V_UINT32)pstCodeCtx->extradata_size;
            //for fortify.
            UTILS_MLOGE_BRK_VAL_IF(0 == u32ExtraLen, eSmeRet, E_SME_DEMUX_RET_INVALID_ARG,
                ("extra len zero !"));
            V_UINT8* pu8CfgData = (V_UINT8*)malloc(u32ExtraLen);
            UTILS_MLOGE_BRK_VAL_IF(!pu8CfgData, eSmeRet, E_SME_DEMUX_RET_NOMEM, ("no memory."));

            if (E_SME_MEM_OK != VOS_Memset_S(pu8CfgData, u32ExtraLen, 0, u32ExtraLen))
            {
                eSmeRet = E_SME_DEMUX_RET_NOMEM;
                free(pu8CfgData);
                ICS_LOGE("VOS_Memset_S return failed !!!");
                break;
            }

            if (E_SME_MEM_OK != VOS_Memcpy_S(pu8CfgData,
                (V_SIZE)pstCodeCtx->extradata_size,
                pstCodeCtx->extradata,
                (V_SIZE)pstCodeCtx->extradata_size))
            {
                eSmeRet = E_SME_DEMUX_RET_NOMEM;
                free(pu8CfgData);
                ICS_LOGE("VOS_Memcpy_S return failed !!!");
                break;
            }

            stCfgData.u32CfgDataSize = (V_UINT32)pstCodeCtx->extradata_size;

            /** BEGIN: Added for DTS2015082904200  by baihaili(b00311320), 2015/8/29 */
            if (stCfgData.pu8CfgData)
               free(stCfgData.pu8CfgData);
            /* END:   Added by baihaili(b00311320), 2015/8/29 */
            stCfgData.pu8CfgData = pu8CfgData;
            break;
        }

        ICS_LOGW("SME_AvcRawCfgDataToNal cnt=%d", stAvcCfgSet.u32Count);
        /* BEGIN: Added for DTS2015071608573 by liurongliang(l00180035), 2015/7/17 */
        pstThis->bNeedAddVideoFrameHeader = ICS_TRUE;
        UTILS_MLOGW_BRK_VAL_IF(0 == stAvcCfgSet.u32Count, eSmeRet, E_SME_DEMUX_RET_SUCCESS,
            ("no cfg data"));
        /* END:   Added for DTS2015071608573 by liurongliang(l00180035), 2015/7/17 */

        //for fortify
        V_UINT64 u64ExtraLenTmp = (V_UINT64)u32ExtraLen;
        for(V_UINT32 j = 0; j < stAvcCfgSet.u32Count; j++)
        {
           //u32ExtraLen += stAvcCfgSet.pstItem[j]->u32Len;
           u64ExtraLenTmp += (V_UINT64)(stAvcCfgSet.pstItem[j]->u32Len & 0xFFFF);
        }
        u32ExtraLen = (V_UINT32)u64ExtraLenTmp;
        UTILS_MLOGE_BRK_VAL_IF((M_SME_UINT32_MAX < u64ExtraLenTmp || 0 == u32ExtraLen),
            eSmeRet, E_SME_DEMUX_RET_INVALID_ARG, ("extradata len overflow!"));

        V_UINT8* pu8CfgData = (V_UINT8*)malloc(u32ExtraLen);
        UTILS_MLOGE_BRK_VAL_IF(!pu8CfgData, eSmeRet, E_SME_DEMUX_RET_NOMEM, ("no memory."));

        if (E_SME_MEM_OK != VOS_Memset_S(pu8CfgData, u32ExtraLen, 0, u32ExtraLen))
        {
            eSmeRet = E_SME_DEMUX_RET_NOMEM;
            free(pu8CfgData);
            ICS_LOGE("VOS_Memset_S return failed !!!");
            break;
        }

        u32ExtraLen = 0;
        for(V_UINT32 j = 0; j < stAvcCfgSet.u32Count; j++)
        {
            if (E_SME_MEM_OK != VOS_Memcpy_S(
                pu8CfgData + u32ExtraLen,
                (V_SIZE)stAvcCfgSet.pstItem[j]->u32Len,
                stAvcCfgSet.pstItem[j]->au8SpsOrPps,
                (V_SIZE)stAvcCfgSet.pstItem[j]->u32Len))
            {
                eSmeRet = E_SME_DEMUX_RET_NOMEM;
                ICS_LOGE("VOS_Memcpy_S return failed !!!");
                break;
            }

           u32ExtraLen += stAvcCfgSet.pstItem[j]->u32Len;
        }

        stCfgData.u32CfgDataSize = u32ExtraLen;
        /** BEGIN: Added for DTS2015082904200  by baihaili(b00311320), 2015/8/29 */
        if (stCfgData.pu8CfgData)
            free(stCfgData.pu8CfgData);
        /* END:   Added by baihaili(b00311320), 2015/8/29 */
        stCfgData.pu8CfgData = pu8CfgData;
    } while ( ICS_FALSE );

    for(V_UINT32 j = 0; j < stAvcCfgSet.u32Count; j++)
    {
       UTILS_MSAFEFREE(stAvcCfgSet.pstItem[j]);
    }

    return eSmeRet;
}

static E_SME_DEMUX_RET ParseCfgDataOfVC1AP(PST_SME_DEMUX_HDL pstThis,
    const AVCodecContext* pstCodeCtx, ST_SME_CFG_DATA& stCfgData)
{
    V_UINT8*        pu8CfgData      = NULL;
    V_UINT32        u32Len          = 0;

    ICS_LOGD("ParseCfgDataOfVC1AP, pstCodeCtx->codec_id=%d", pstCodeCtx->codec_id);

    if(pstCodeCtx->extradata_size <= 1)
    {
        u32Len = (V_UINT32)pstCodeCtx->extradata_size;
    }
    else
    {
        u32Len = (V_UINT32)pstCodeCtx->extradata_size - 1;
    }

    UTILS_MLOGE_RET_VAL_IF(0 == u32Len, E_SME_DEMUX_RET_FATAL, ("extra len = 0"));
    pu8CfgData = (V_UINT8*)malloc(u32Len);
    UTILS_MLOGE_RET_VAL_IF(!pu8CfgData, E_SME_DEMUX_RET_FATAL, ("no memory."));

    if (E_SME_MEM_OK != VOS_Memset_S(pu8CfgData, u32Len, 0, u32Len))
    {
        ICS_LOGE("VOS_Memset_S return failed !!!");
        free(pu8CfgData);

        return E_SME_DEMUX_RET_NOMEM;
    }

    if (E_SME_MEM_OK != VOS_Memcpy_S(
        pu8CfgData,
        (V_SIZE)u32Len,
        pstCodeCtx->extradata + 1,
        (V_SIZE)pstCodeCtx->extradata_size - 1))
    {
        ICS_LOGE("VOS_Memcpy_S return failed !!!");
        free(pu8CfgData);

        return E_SME_DEMUX_RET_NOMEM;
    }
    stCfgData.u32CfgDataSize = u32Len;

    /** BEGIN: Added for DTS2015082904200  by baihaili(b00311320), 2015/8/29 */
    if (stCfgData.pu8CfgData)
        free(stCfgData.pu8CfgData);
    /* END:   Added by baihaili(b00311320), 2015/8/29 */
    stCfgData.pu8CfgData = pu8CfgData;
    pstThis->bNeedAddVideoFrameHeader = ICS_TRUE;

    return E_SME_DEMUX_RET_SUCCESS;

}

static E_SME_DEMUX_RET ParseCfgDataOfVP6A(PST_SME_DEMUX_HDL pstThis,
    const AVCodecContext* pstCodeCtx, ST_SME_CFG_DATA& stCfgData)
{
    V_UINT8*        pu8CfgData      = NULL;
    V_UINT32        u32Len          = 0;

    ICS_LOGI("ParseCfgDataOfVP6A, pstCodeCtx->codec_id=%d,len=%d",
        pstCodeCtx->codec_id, pstCodeCtx->extradata_size);

    if(pstCodeCtx->extradata_size <= 1)
    {
        stCfgData.u32CfgDataSize = 0;
        stCfgData.pu8CfgData = NULL;
        return E_SME_DEMUX_RET_SUCCESS;
    }
    u32Len = (V_UINT32)pstCodeCtx->extradata_size - 1;
    UTILS_MLOGE_RET_VAL_IF(0 == u32Len, E_SME_DEMUX_RET_FATAL, ("extra len = 0"));
    pu8CfgData = (V_UINT8*)malloc(u32Len);
    UTILS_MLOGE_RET_VAL_IF(!pu8CfgData, E_SME_DEMUX_RET_FATAL, ("no memory."));

    if (E_SME_MEM_OK != VOS_Memset_S(pu8CfgData, u32Len, 0, u32Len))
    {
        ICS_LOGE("VOS_Memset_S return failed !!!");
        free(pu8CfgData);

        return E_SME_DEMUX_RET_NOMEM;
    }

    if (E_SME_MEM_OK != VOS_Memcpy_S(
        pu8CfgData,
        u32Len,
        pstCodeCtx->extradata + 1,
        (V_SIZE)pstCodeCtx->extradata_size - 1))
    {
        ICS_LOGE("VOS_Memcpy_S return failed !!!");
        free(pu8CfgData);

        return E_SME_DEMUX_RET_NOMEM;
    }

    stCfgData.u32CfgDataSize = u32Len;
    /** BEGIN: Added for DTS2015082904200  by baihaili(b00311320), 2015/8/29 */
    if (stCfgData.pu8CfgData)
        free(stCfgData.pu8CfgData);
    /* END:   Added by baihaili(b00311320), 2015/8/29 */
    stCfgData.pu8CfgData = pu8CfgData;
    pstThis->bNeedAddVideoFrameHeader = ICS_TRUE;

    return E_SME_DEMUX_RET_SUCCESS;

}

static E_SME_DEMUX_RET ParseCfgDataOfGeneric(
    const AVCodecContext* pstCodeCtx, ST_SME_CFG_DATA& stCfgData)
{
    V_UINT8*        pu8CfgData      = NULL;

    ICS_LOGI(
        "ParseCfgDataOfGeneric, pstCodeCtx->codec_id=%d, extradata_size=%d",
        pstCodeCtx->codec_id, pstCodeCtx->extradata_size);

    if(pstCodeCtx->extradata_size <= 0)
    {
        ICS_LOGE("extradata_size :%d error", pstCodeCtx->extradata_size);
        return E_SME_DEMUX_RET_INVALID_ARG;
    }

    pu8CfgData = (V_UINT8*)malloc(pstCodeCtx->extradata_size);
    UTILS_MLOGE_RET_VAL_IF(!pu8CfgData, E_SME_DEMUX_RET_FATAL, ("no memory."));

    if (E_SME_MEM_OK != VOS_Memset_S(pu8CfgData, (V_SIZE)pstCodeCtx->extradata_size, 0, (V_SIZE)pstCodeCtx->extradata_size))
    {
        ICS_LOGE("VOS_Memset_S return failed !!!");
        free(pu8CfgData);

        return E_SME_DEMUX_RET_NOMEM;
    }

    if (E_SME_MEM_OK != VOS_Memcpy_S(
        pu8CfgData,
        (V_SIZE)pstCodeCtx->extradata_size,
        pstCodeCtx->extradata,
        (V_SIZE)pstCodeCtx->extradata_size))
    {
        ICS_LOGE("VOS_Memcpy_S return failed !!!");
        free(pu8CfgData);

        return E_SME_DEMUX_RET_NOMEM;
    }

    stCfgData.u32CfgDataSize = (V_UINT32)pstCodeCtx->extradata_size;
    /** BEGIN: Added for DTS2015082904200  by baihaili(b00311320), 2015/8/29 */
    if (stCfgData.pu8CfgData)
        free(stCfgData.pu8CfgData);
    /* END:   Added by baihaili(b00311320), 2015/8/29 */
    stCfgData.pu8CfgData = pu8CfgData;

    return E_SME_DEMUX_RET_SUCCESS;
}

static E_SME_DEMUX_RET ParseCfgData(PST_SME_DEMUX_HDL pstThis, const AVCodecContext* pstCodeCtx,
    ST_SME_CFG_DATA& stCfgData)
{
    E_SME_DEMUX_RET eSmeRet         = E_SME_DEMUX_RET_SUCCESS;

    UTILS_MLOGW_RET_VAL_IF(pstCodeCtx->extradata_size <= 0 || NULL == pstCodeCtx->extradata,
        E_SME_DEMUX_RET_SUCCESS, ("codec:%#x, no extradata", pstCodeCtx->codec_id));

    if(AV_CODEC_ID_H264 == pstCodeCtx->codec_id)
    {
        eSmeRet = ParseCfgDataOfAVC(pstThis, pstCodeCtx, stCfgData);
    }
    else if(AV_CODEC_ID_HEVC == pstCodeCtx->codec_id)
    {
        eSmeRet = ParseCfgDataOfHEVC(pstThis, pstCodeCtx, stCfgData);
    }
    else if ((AV_CODEC_ID_VC1 == pstCodeCtx->codec_id)
        && (E_SME_VC1_PROFILE_ADVANCED == pstCodeCtx->profile))
    {
        eSmeRet = ParseCfgDataOfVC1AP(pstThis, pstCodeCtx, stCfgData);
    }
    else if (AV_CODEC_ID_VP6A == pstCodeCtx->codec_id)
    {
        eSmeRet = ParseCfgDataOfVP6A(pstThis, pstCodeCtx, stCfgData);
    }
    else
    {
        eSmeRet = ParseCfgDataOfGeneric(pstCodeCtx, stCfgData);
    }

    return eSmeRet;
};

static E_SME_DEMUX_RET GetStream(PST_SME_DEMUX_HDL pstThis, const AVStream *avstream)
{
    E_SME_DEMUX_RET eSmeRet = E_SME_DEMUX_RET_SUCCESS;
    PST_SME_MEDIAINFO pstMediaInfo = NULL;
    AVCodecContext *pstCodeCtx;
    ST_SME_MEDIAINFO stMediaInfoTmp;

    ICS_LOGD("GetStream in");

	do
	{
        pstCodeCtx = avstream->codec;

        UTILS_MLOGE_BRK_VAL_IF(!pstCodeCtx, eSmeRet, E_SME_DEMUX_RET_INVALID_ARG,
            ("code context is null."))

		if(pstCodeCtx->codec_type != AVMEDIA_TYPE_AUDIO
			&& pstCodeCtx->codec_type != AVMEDIA_TYPE_VIDEO)
		{
            ICS_LOGW("unsupport codec type, pstCodeCtx->codec_type=%d, pstCodeCtx->codec_id=%d",
                pstCodeCtx->codec_type, pstCodeCtx->codec_id);
            eSmeRet = E_SME_DEMUX_RET_UNSUPPORT;
            break;
		}

        /* BEGIN: Added for DTS2015071410459 by liurongliang(l00180035), 2015/7/16 */
        UTILS_MLOGE_BRK_VAL_IF(M_SME_STREAM_NUM_MAX <= avstream->index, eSmeRet,
            E_SME_DEMUX_RET_UNSUPPORT,
            ("index=%d >= max=%d", avstream->index, M_SME_STREAM_NUM_MAX))
        /* END:   Added for DTS2015071410459 by liurongliang(l00180035), 2015/7/16 */

        //保存音视频公共属性

        if (E_SME_MEM_OK != VOS_Memset_S(&stMediaInfoTmp, sizeof(stMediaInfoTmp), 0, sizeof(stMediaInfoTmp)))
        {
            ICS_LOGE("VOS_Memset_S return failed !!!");
            eSmeRet = E_SME_DEMUX_RET_NOMEM;
            break;
        }

        if(avstream->duration <= 0)
        {
            stMediaInfoTmp.i32Duration = (pstThis->pstAVFmtCtx->duration < 0)?
                -1 : (V_INT32)(pstThis->pstAVFmtCtx->duration / M_SME_MS_TO_US);
        }
        else
        {
            stMediaInfoTmp.i32Duration = (V_INT32)(avstream->duration
                * M_SME_SEC_TO_MS * avstream->time_base.num
                / avstream->time_base.den);
        }
        ICS_LOGI("CODEC_info: codec_type=%d, codec_id=%d, stream_duration=%lld,"
            " media_dur=%d ms, stream_index=%d, field_order=%d",
            pstCodeCtx->codec_type, pstCodeCtx->codec_id,
            avstream->duration,stMediaInfoTmp.i32Duration, avstream->index,
            pstCodeCtx->field_order);

        stMediaInfoTmp.i32StreamIdx = avstream->index;
        stMediaInfoTmp.u32Bps = (V_UINT32)pstCodeCtx->bit_rate;
        stMediaInfoTmp.eCodecId = FFCodecidToSmeCodecid(pstCodeCtx->codec_id);
        FFCodecidToMime((const AVCodecContext *)pstCodeCtx, stMediaInfoTmp.acMime, (sizeof(stMediaInfoTmp.acMime) - 1));
        stMediaInfoTmp.u32CodecForcc = pstCodeCtx->codec_tag;

        stMediaInfoTmp.i64FirstPacketPos = avstream->first_packet_pos;

        if(pstCodeCtx->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            //create video stream, memory will be free in FreeStreams().
            PST_SME_VIDEOINFO pstVideoInfo;
            pstVideoInfo = (PST_SME_VIDEOINFO)malloc(sizeof(ST_SME_VIDEOINFO));
            UTILS_MLOGE_BRK_VAL_IF(pstVideoInfo == NULL, eSmeRet, E_SME_DEMUX_RET_NOMEM,
            	("no memory"));
            stMediaInfoTmp.eMediaType = E_SME_MEDIA_TYPE_VIDEO;

            pstVideoInfo->stBase = stMediaInfoTmp;
            if(0 != pstCodeCtx->ticks_per_frame)
            {
                pstVideoInfo->i32FpsNum = pstCodeCtx->time_base.den / pstCodeCtx->ticks_per_frame;
            }
            pstVideoInfo->i32FpsDen = pstCodeCtx->time_base.num;

            ICS_LOGI("demux width=%d, height=%d", pstCodeCtx->width, pstCodeCtx->height);

            if(pstCodeCtx->width <= 0)
            {
                pstCodeCtx->width = SME_DEMUX_DEFAULT_WIDTH;
                ICS_LOGI("demux width use default(%d)", SME_DEMUX_DEFAULT_WIDTH);
            }
            if(pstCodeCtx->height <= 0)
            {
                pstCodeCtx->height = SME_DEMUX_DEFAULT_HEIGHT;
                ICS_LOGI("demux height use default(%d).", SME_DEMUX_DEFAULT_HEIGHT);
            }
            /* BEGIN: Added for DTS2015042111511 by liurongliang(l00180035), 2015/4/27 */
            else if(pstCodeCtx->field_order > AV_FIELD_PROGRESSIVE)
            {
                pstCodeCtx->height *= 2;
                ICS_LOGI("video interlaced demux height*2(%d).", pstCodeCtx->height);
            }
            /* END:   Added for DTS2015042111511 by liurongliang(l00180035), 2015/4/27 */

            pstVideoInfo->stVideoRes.i32Width = pstCodeCtx->width;
            pstVideoInfo->stVideoRes.i32Height = pstCodeCtx->height;

            if (avstream->metadata) {
                AVDictionaryEntry *rot = av_dict_get(avstream->metadata, "rotate", NULL, 0);
                pstVideoInfo->rotate = V_UINT16((rot && rot->value) ? atoi(rot->value) : 0);
                ICS_LOGI("GetStream:codec_id=%d, rotate=%d",
                    pstCodeCtx->codec_id, pstVideoInfo->rotate);
            }

            pstMediaInfo = (PST_SME_MEDIAINFO)pstVideoInfo;

            if(AV_CODEC_ID_VC1 == pstCodeCtx->codec_id)
            {
                ICS_LOGI("GetStream:codec_id:%#x, profile=%d",
                    pstCodeCtx->codec_id, pstCodeCtx->profile);
                pstVideoInfo->stBase.i32Profile = pstCodeCtx->profile;
            }
            ICS_LOGI("GetStream:AVMEDIA_TYPE_VIDEO:i32FpsNum(%d), i32FpsDen(%d)",
                pstVideoInfo->i32FpsNum, pstVideoInfo->i32FpsDen);
        }
        else
        {
            //create audio stream
            PST_SME_AUDIOINFO pstAudioInfo;
            pstAudioInfo = (PST_SME_AUDIOINFO)malloc(sizeof(ST_SME_AUDIOINFO));
            UTILS_MLOGE_BRK_VAL_IF(pstAudioInfo == NULL, eSmeRet, E_SME_DEMUX_RET_NOMEM,
                ("no memory"));
            stMediaInfoTmp.eMediaType = E_SME_MEDIA_TYPE_AUDIO;

            pstAudioInfo->stBase = stMediaInfoTmp;
            pstAudioInfo->eSampleFmt = FFSampleFmtToSmeAudFmt(pstCodeCtx->sample_fmt);
            if(pstCodeCtx->channels <=0)
            {
                pstAudioInfo->u32Channels = SME_DEMUX_DEFAULT_Ch;
            }
            else
            {
                pstAudioInfo->u32Channels = (V_UINT32)pstCodeCtx->channels;
            }

            if(pstCodeCtx->sample_rate <=0)
            {
                pstAudioInfo->u32SampleRate = SME_DEMUX_DEFAULT_A_SR;
            }
            else
            {
                pstAudioInfo->u32SampleRate = (V_UINT32)pstCodeCtx->sample_rate;
            }

            if (pstCodeCtx->block_align <= 0)
            {
                pstAudioInfo->u32BlockAlign = 0;
            }
            else
            {
                pstAudioInfo->u32BlockAlign = (V_UINT32)pstCodeCtx->block_align;
            }

            if (avstream->metadata)
            {
                AVDictionaryEntry *lang = av_dict_get(avstream->metadata,
                                                      "language", NULL, 0);
                V_UINT32 str_len;

                E_SME_MEM_SECURE_RET ret = VOS_Memset_S(pstAudioInfo->cLang, sizeof(pstAudioInfo->cLang),
                           0, sizeof(pstAudioInfo->cLang));

                UTILS_MLOGE_BRK_VAL_IF(E_SME_MEM_OK != ret, eSmeRet, E_SME_DEMUX_RET_NOMEM,
                        ("VOS_Memset_S return failed."));

                if (lang && lang->value)
                {
                    str_len = (V_UINT32)strlen(lang->value);

                    if (0 < str_len)
                    {
                        ret = VOS_Memcpy_S(pstAudioInfo->cLang,
                            UTILS_MMIN_VAL_TWO(str_len, M_SME_LANG_MAX_LEN - 1),
                            lang->value,
                            UTILS_MMIN_VAL_TWO(str_len, M_SME_LANG_MAX_LEN - 1));

                        UTILS_MLOGE_BRK_VAL_IF(E_SME_MEM_OK != ret, eSmeRet, E_SME_DEMUX_RET_NOMEM,
                            ("VOS_Memcpy_S return failed."));

                        pstAudioInfo->cLang[M_SME_LANG_MAX_LEN - 1] = 0;
                    }
                    else
                    {
                        ICS_LOGI("lang->value exist, but str_len=0");
                    }
                }
            }

            if (pstCodeCtx->bits_per_coded_sample <= 0)
            {
                pstAudioInfo->u32BitsPerCodecSample = 0;
            }
            else
            {
                pstAudioInfo->u32BitsPerCodecSample =
                    (V_UINT32)pstCodeCtx->bits_per_coded_sample;
            }

            pstMediaInfo = (PST_SME_MEDIAINFO)pstAudioInfo;
            ICS_LOGI("GetStream:AVMEDIA_TYPE_Audio:eSampleFmt(%d), "
                          "u32Channels(%d), u32SampleRate(%d), lang(%s)",
                          pstAudioInfo->eSampleFmt, pstAudioInfo->u32Channels,
                          pstAudioInfo->u32SampleRate, pstAudioInfo->cLang);
        }

        /* BEGIN: Modified by liurongliang(l00180035), 2015/3/5 */
        /*parse cfg data here*/
        eSmeRet = ParseCfgData(pstThis, pstCodeCtx, pstMediaInfo->stCfgData);
        UTILS_MLOGE_BRK_IF(eSmeRet != E_SME_DEMUX_RET_SUCCESS, ("ParseCfgData failed"));
        /* END:   Modified by liurongliang(l00180035), 2015/3/5 */

        pstThis->stStreams.pstMedia[avstream->index] = pstMediaInfo;
	}while(ICS_FALSE);

	if(eSmeRet != E_SME_DEMUX_RET_SUCCESS)
	{
        ICS_LOGE("error happened(eSmeRet:%d)", eSmeRet);
        UTILS_MSAFEFREE(pstMediaInfo);
	}

    ICS_LOGD("GetStream out");

    return eSmeRet;
}

static SMEDEMUXHDL
SME_CreateDemuxOfFFMpeg(
    IN const ST_SME_IOINTERRUPTCB* pstIOInterruptCb)
{
    SMEDEMUXHDL pvRet = NULL;

    ICS_LOGD("SME_CreateDemuxOfFFMpeg in");

    do
    {
        PST_SME_DEMUX_HDL pstThis = (PST_SME_DEMUX_HDL)malloc(sizeof(ST_SME_DEMUX_HDL));
        UTILS_MLOGE_BRK_VAL_IF(!pstThis, pvRet, NULL, ("no memory"));

        if (E_SME_MEM_OK != VOS_Memset_S(pstThis, sizeof(ST_SME_DEMUX_HDL), 0, sizeof(ST_SME_DEMUX_HDL)))
        {
            free(pstThis);
            ICS_LOGE("VOS_Memset_S return failed !!!");
            break;
        }

        if(NULL != pstIOInterruptCb){
            pstThis->stIOInterruptCb = *pstIOInterruptCb;
        }

        pvRet = (SMEDEMUXHDL)pstThis;
    }while(ICS_FALSE);

    ICS_LOGD("SME_CreateDemuxOfFFMpeg out");

    return pvRet;
}

static V_VOID
SME_DestroyDemuxOfFFMpeg(INOUT SMEDEMUXHDL phdl)
{
    PST_SME_DEMUX_HDL pstThis = NULL;

    ICS_LOGD("SME_DestroyDemuxOfFFMpeg in");

    UTILS_MLOGE_RET_IF(!phdl, ("handle is null."));

    pstThis = (PST_SME_DEMUX_HDL)phdl;

    /*Forbid use ICS_LOGD after pstThis is freed
      due to ICS_LOGD would access it*/
    ICS_LOGD("SME_DestroyDemuxOfFFMpeg out");
    free(pstThis);
    return;
}

static V_VOID
SME_OrderStream(IN SMEDEMUXHDL pvDemuxHdl, IN const AVFormatContext *pstContext)
{
    PST_SME_DEMUX_HDL pstThis = NULL;
    V_INT64 *position = NULL;
    V_UINT32 *index_array = NULL;
    V_UINT32 i = 0, j = 0;
    V_INT64 min_pos = 0;
    V_INT64 temp_pos = 0;
    V_UINT32 temp_idx = 0;
    V_UINT32 min_index = 0, cur_index = 0;

    pstThis = (PST_SME_DEMUX_HDL)pvDemuxHdl;

    UTILS_M_RET_IF(0 == pstContext->nb_streams);

    position = (V_INT64 *)malloc(sizeof(V_INT64)*pstContext->nb_streams);
    if (NULL == position) {
        ICS_LOGE("malloc error for position");
        return;
    }

    if (E_SME_MEM_OK != VOS_Memset_S(position,
        sizeof(V_INT64)*pstContext->nb_streams, 0, sizeof(V_INT64)*pstContext->nb_streams))
    {
        ICS_LOGE("VOS_Memset_S return failed !!!");
        free(position);
        return;
    }

    index_array = (V_UINT32 *)malloc(sizeof(V_INT32)*pstContext->nb_streams);
    if (NULL == index_array) {
        free(position);
        ICS_LOGE("malloc error for index_array");
        return;
    }

    if (E_SME_MEM_OK != VOS_Memset_S(index_array,
        sizeof(V_INT32)*pstContext->nb_streams, 0, sizeof(V_INT32)*pstContext->nb_streams))
    {
        free(position);
        free(index_array);
        ICS_LOGE("VOS_Memset_S return failed !!!");
        return;
    }

    for (i = 0; i < pstContext->nb_streams; i++)
    {
        position[i] = pstContext->streams[i]->first_packet_pos;
        index_array[i] = i;
    }

    for (i = 0; i < pstContext->nb_streams; i++)
    {
        min_pos = position[i];
        min_index = index_array[i];
        cur_index = i;

        for (j = i; j < pstContext->nb_streams; j++)
        {
            if (min_pos > position[j])
            {
                cur_index = j;
                min_pos = position[j];
                min_index = index_array[j];
            }
        }
        if (cur_index != i)
        {
            temp_pos = position[i];
            position[i] = position[cur_index];
            position[cur_index] = temp_pos;

            temp_idx = index_array[i];
            index_array[i] = index_array[cur_index];
            index_array[cur_index] = temp_idx;
        }
        pstThis->stStreams.u32IdxOrderInPos[i] = min_index;
    }

    for (i = 0; i < pstContext->nb_streams; i++)
    {
        ICS_LOGW("====index:%d,pos:%lld", pstThis->stStreams.u32IdxOrderInPos[i],
            pstContext->streams[pstThis->stStreams.u32IdxOrderInPos[i]]->first_packet_pos);

    }

    for (i = 0; i < pstContext->nb_streams && i < M_SME_STREAM_NUM_MAX; i++)
    {
        if(NULL != pstThis->stStreams.pstMedia[i])
        {
            ICS_LOGW("====mediatype:%d, id:%#x, index=%d,duration=%d,streamidentifier:%d====",
                pstThis->stStreams.pstMedia[i]->eMediaType,
                pstThis->stStreams.pstMedia[i]->eCodecId,
                pstThis->stStreams.pstMedia[i]->i32StreamIdx,
                pstThis->stStreams.pstMedia[i]->i32Duration,
                pstContext->streams[i]->stream_identifier);
        }
    }

    free(position);
    free(index_array);

    return;
}

/* support two scenarios:
 * 1. for local vod play,
 *    and http/hls/dash(protocol implemented in GStreamer plugin):
 *    input parameter pvDemuxFmt is not NULL.
 * 2. for http/hls/rtsp/etc(protocol implemented in ffmpeg):
 *    input parameter pvDemuxFmt is NULL.
 */
static E_SME_DEMUX_RET
SME_OpenDemuxOfFFMpeg(IN SMEDEMUXHDL pvDemuxHdl,
                        IN const V_CHAR* pszUrl,
                        IN const ST_SME_IOPROTO* pstIOProto,
                        IN SMEDEMUXFMT pvDemuxFmt)
{
    V_INT32 i32Ret = -1;
    E_SME_DEMUX_RET eSmeRet = E_SME_DEMUX_RET_SUCCESS;
    PST_SME_DEMUX_HDL pstThis = NULL;
    AVFormatContext *pstContext = NULL;
    V_UINT32 u32ValidStreams = 0;
    E_VOS_STR_ERR ret = E_VOS_STR_ENONE;

    ICS_LOGI("SME_OpenDemuxOfFFMpeg in");

    do
    {
        //入参检查
        UTILS_MLOGE_BRK_VAL_IF((!pvDemuxHdl || !pszUrl), eSmeRet,
            E_SME_DEMUX_RET_INVALID_ARG, ("null param."));

        pstThis = (PST_SME_DEMUX_HDL)pvDemuxHdl;

        UTILS_MLOGE_BRK_VAL_IF(pstThis->bOpened, eSmeRet, E_SME_DEMUX_RET_SUCCESS,
            ("already opened."));

        /** BEGIN: Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
        pstContext = avformat_alloc_context();
        UTILS_MLOGE_BRK_VAL_IF(NULL == pstContext, eSmeRet, E_SME_DEMUX_RET_NOMEM,
            ("alloc context failed"));

        pstContext->interrupt_callback.callback = pstThis->stIOInterruptCb.callback;
        pstContext->interrupt_callback.opaque = pstThis->stIOInterruptCb.opaque;
        /** END:   Added DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */

        //register ffmpeg IO protocol to read data through gstreamer
        if(NULL != pstIOProto)
        {
            if (E_SME_MEM_OK != VOS_Memset_S(&pstThis->stURLProt,
                sizeof(pstThis->stURLProt), 0, sizeof(pstThis->stURLProt)))
            {
                ICS_LOGE("VOS_Memset_S return failed !!!");
                eSmeRet = E_SME_DEMUX_RET_NOMEM;
                break;
            }

            pstThis->stURLProt.name         = pstIOProto->pstName;
            pstThis->stURLProt.url_open     =
                (int(*)( URLContext *h, const char *url, int flags))
                pstIOProto->SME_OpenIOProto;
            pstThis->stURLProt.url_read     =
                (int(*)( URLContext *h, unsigned char *buf, int size))
                pstIOProto->SME_Read;
            pstThis->stURLProt.url_write    =
                (int(*)(URLContext *h, const unsigned char *buf, int size))
                pstIOProto->SME_Write;
            pstThis->stURLProt.url_seek     =
                (int64_t(*)( URLContext *h, int64_t pos, int whence))
                pstIOProto->SME_Seek;
            pstThis->stURLProt.url_close    =
                (int(*)(URLContext *h))pstIOProto->SME_Close;

            //find if in g_pstURLProtocol;
            URLProtocol* pstNew = CloneProtIfNotExist((const URLProtocol*)
                &pstThis->stURLProt);
            if(NULL != pstNew)
            {
                VOS_ThdMutexLock(&g_stffmpegMutex);
                i32Ret = ffurl_register_protocol(pstNew);
                VOS_ThdMutexUnLock(&g_stffmpegMutex);

                if (0 == i32Ret) {
                    ICS_LOGI("ffurl_register_protocol() succeed");
                }

                UTILS_MLOGE_BRK_VAL_IF(i32Ret != 0, eSmeRet, E_SME_DEMUX_RET_FATAL,
                    ("ffurl_register_protocol() fail"));
            }
        }

        AVInputFormat *pstAVFmt = (AVInputFormat *)pvDemuxFmt;
        if (pstAVFmt)
        {
            /* For local vod play, and http/hls/dash(protocol implemented in GStreamer plugin):
             *   input parameter pvDemuxFmt is not NULL.
             */
            ICS_LOGI("DEMUX: gst_protocol, AVInputFormat name:%s, long_name:%s",
                pstAVFmt->name, pstAVFmt->long_name);

            i32Ret = avformat_open_input(&pstContext, pszUrl, pstAVFmt, NULL);
        } else {
            /* for http/hls/rtsp/etc(protocol implemented in ffmpeg):
             *   input parameter pvDemuxFmt is NULL.
             */
            AVDictionary *pstOpts = NULL;

            if (pstThis->stUrlOptions.pszUserAgent)
                av_dict_set(&pstOpts, "user-agent", pstThis->stUrlOptions.pszUserAgent, 0);

            if (pstThis->stUrlOptions.pszReferer)
                av_dict_set(&pstOpts, "referer", pstThis->stUrlOptions.pszReferer, 0);

            if (pstThis->stUrlOptions.pszCookies)
                av_dict_set(&pstOpts, "cookies", pstThis->stUrlOptions.pszCookies, 0);

            if (pstThis->stUrlOptions.pszHeaders)
                av_dict_set(&pstOpts, "headers", pstThis->stUrlOptions.pszHeaders, 0);

            i32Ret = avformat_open_input(&pstContext, pszUrl, pstAVFmt, &pstOpts);

            av_dict_free(&pstOpts);
        }

        UTILS_MLOGE_BRK_VAL_IF(i32Ret < 0, eSmeRet, E_SME_DEMUX_RET_OPEN_FAIL,
            ("ffmpeg open input file fail"));

        int64_t i64FileSize = avio_size(pstContext->pb);
        i64FileSize = i64FileSize > 0 ? i64FileSize : 0;
        ICS_LOGI("av_open_input_file success, file_size=%lld", i64FileSize);

        //next, to get media stream(codec, etc) information

        pstContext->max_analyze_duration2 = 120*AV_TIME_BASE;

        if (!pstAVFmt) {
            /* for http/hls/rtsp/etc(protocol implemented in ffmpeg): */
            pstAVFmt = pstContext->iformat;

            UTILS_MLOGE_BRK_VAL_IF(!pstAVFmt, eSmeRet, E_SME_DEMUX_RET_UNSUPPORT,
                ("DEMUX: ff_protocol, AVInputFormat not found"));

            ICS_LOGI("DEMUX: ff_protocol, AVInputFormat name:%s, long_name:%s",
                pstAVFmt->name, pstAVFmt->long_name);
        }

        if(pstAVFmt->name && (!strcmp(pstAVFmt->name, "mpegts")
            || (!strcmp(pstAVFmt->name, "matroska,webm"))
            || (!strcmp(pstAVFmt->name, "flv"))
            || (!strcmp(pstAVFmt->name, "avi"))))
        {
            //pstContext->probesize = SME_DEMUX_V_MAX_PROBESIZE*1000;
            pstContext->probesize = SME_DEMUX_V_MAX_PROBESIZE*100;
        }
        else if(pstAVFmt->name && ((!strcmp(pstAVFmt->name, "mpeg"))
            || (!strcmp(pstAVFmt->name, "mpegvideo"))))
        {
            pstContext->probesize = SME_DEMUX_V_MAX_PROBESIZE * 2;
        }
        else if(pstAVFmt->name && (!strcmp(pstAVFmt->name, "aac")
            || !strcmp(pstAVFmt->name, "amr")
            || !strcmp(pstAVFmt->name, "ogg")
            || !strcmp(pstAVFmt->name, "wav")  //support audio codec:dts in wav
            || !strcmp(pstAVFmt->name, "mp3")))
        {
            pstContext->probesize = SME_DEMUX_A_MAX_PROBESIZE;
        }
        else
        {
            pstContext->probesize = SME_DEMUX_MIN_PROBESIZE;//not probe
        }

        /*
         * probe_size should not exceeds file size.
         */
        if (i64FileSize > 0 &&
           ((int64_t)pstContext->probesize + SME_DEMUX_PROBESIZE_LEFT) >  i64FileSize)
        {
            if (i64FileSize >= SME_DEMUX_PROBESIZE_LEFT )
            {
                pstContext->probesize = ((unsigned int)i64FileSize - SME_DEMUX_PROBESIZE_LEFT);
            }
            else
            {
                pstContext->probesize = ((unsigned int)i64FileSize * 2) / 3;
            }
        }

        ICS_LOGI("[streamInfo]file_size=%lld, probe_size=%u",
            i64FileSize, pstContext->probesize);

        /* BEGIN: Added by liurongliang(l00180035), 2014/11/28 */
        //for accelerate avformat_find_stream_info when avc/hevc
        pstContext->flags |= AVFMT_FLAG_VIDEO_HARD_DEC;
        /* END:   Added by liurongliang(l00180035), 2014/11/28 */

        i32Ret = avformat_find_stream_info(pstContext, NULL);
        if (i32Ret == AVERROR_EOF)
        {
            ICS_LOGI("avformat_find_stream_info reach EOF, file_size=%lld, probe_size=%u",
            i64FileSize, pstContext->probesize);
        }
        else
        {
            UTILS_MLOGE_BRK_VAL_IF(i32Ret < 0, eSmeRet, E_SME_DEMUX_RET_NO_DATA,
                ("ffmpeg find stream info, no info."));
        }

        ICS_LOGI("DEMUX: av_find_stream_info success");

        UTILS_MLOGE_BRK_VAL_IF(pstContext->nb_streams == 0, eSmeRet, E_SME_DEMUX_RET_NO_DATA,
            ("stream num is zero."));


        ICS_LOGI("DEMUX: [streamInfo]nb_streams:%u, duration:%lld, bit_rate:%u",
            pstContext->nb_streams, pstContext->duration, pstContext->bit_rate);

        pstThis->bNeedAddVideoFrameHeader = ICS_FALSE;
        pstThis->pstAVFmtCtx = pstContext;
        //保存每条媒体流属性为SME格式
        for(V_UINT32 i = 0; i < pstContext->nb_streams; i++)
        {
            eSmeRet = GetStream(pstThis, (const AVStream *)pstContext->streams[i]);
            UTILS_MLOGW_CTN_IF(eSmeRet != E_SME_DEMUX_RET_SUCCESS, ("get stream fail."));
            u32ValidStreams++;
        }

        //所有流都获取失败则Open接口失败
        UTILS_MLOGE_BRK_VAL_IF(u32ValidStreams == 0, eSmeRet, E_SME_DEMUX_RET_NO_DATA,
            ("all streams retrive fail."));

        SME_OrderStream(pstThis, pstContext);

        //保存媒体流公共属性
        pstThis->stStreams.i32Duration = (pstContext->duration < 0) ? -1 : (V_INT32)(pstContext->duration/ M_SME_MS_TO_US);
        pstThis->stStreams.i64FileSize = i64FileSize;
        pstThis->stStreams.u32BitRate = (V_UINT32)pstContext->bit_rate;
        pstThis->stStreams.u32NumStream = pstContext->nb_streams;
        ICS_LOGI("[streamInfo]ValidStreams:%u", u32ValidStreams);

        //保存其他信息
        pstThis->pszFileName = (V_CHAR *)malloc(strlen(pszUrl) + 1);
        UTILS_M_BRK_VAL_IF(NULL == pstThis->pszFileName, eSmeRet, E_SME_DEMUX_RET_NOMEM);

        if (E_SME_MEM_OK != VOS_Memset_S(pstThis->pszFileName,
            (V_SIZE)strlen(pszUrl) + 1, 0, (V_SIZE)strlen(pszUrl) + 1))
        {
            ICS_LOGE("VOS_Memset_S return failed !!!");
            eSmeRet = E_SME_DEMUX_RET_NOMEM;
            break;
        }

        ret = VOS_Strncpy_S(
            pstThis->pszFileName,
            (V_SIZE)strlen(pszUrl) + 1,
            pszUrl,
            (V_SIZE)strlen(pszUrl));
        if (E_VOS_STR_ENONE != ret)
        {
            ICS_LOGE("VOS_Strncpy_S  return failed, ret=%d", ret);
            eSmeRet = E_SME_DEMUX_RET_NOMEM;
            break;
        }

        pstThis->pvDemuxFmt = pvDemuxFmt;
        for(int i = 0; i < M_SME_STREAM_NUM_MAX; i++)
        {
            pstThis->i64BasePts[i]              = -1;
            pstThis->i64BaseDts[i]              = -1;
            pstThis->i64BasePtsMin              = M_SME_INT64_MAX;
            pstThis->i64BaseDtsMin              = M_SME_INT64_MAX;
            pstThis->i64LastTSInBase[i]         = -1;
            pstThis->bFirstAfterSeek[i]         = ICS_FALSE;
            pstThis->i64DeltaPts[i]             = 0;
            pstThis->i64DeltaDts[i]             = 0;
            pstThis->i64MaxPts[i]               = -1;

            if(NULL != pstThis->stStreams.pstMedia[i])
            {
                if(E_SME_MEDIA_TYPE_VIDEO
                    == pstThis->stStreams.pstMedia[i]->eMediaType)
                {
                    pstThis->i64LastDurationInBase[i]
                        = M_SME_TS_SCALETOBASE(SME_DEMUX_DEFAULT_DURATION_V,
                        1,
                        pstContext->streams[i]->time_base.num,
                        pstContext->streams[i]->time_base.den);
                }
                else
                {
                    pstThis->i64LastDurationInBase[i]
                        = M_SME_TS_SCALETOBASE(SME_DEMUX_DEFAULT_DURATION_A,
                        1,
                        pstContext->streams[i]->time_base.num,
                        pstContext->streams[i]->time_base.den);
                }
            }
            else
            {
                pstThis->i64LastDurationInBase[i] = 0;
            }
        }

        pstThis->u64VideoPkts = 0;
        pstThis->u64VideoDtsOutofOrderChangeCounts = 0;
        pstThis->u64AudioDtsOutofOrderChangeCounts = 0;

        pstThis->bOpened = ICS_TRUE;
        pstThis->bStopReadFf = ICS_FALSE;
        pstThis->eReadErr = E_SME_DEMUX_RET_SUCCESS;
        pstThis->eLastReadRes = E_SME_DEMUX_RET_SUCCESS;
        InitStreamQueueArray(pstThis->stStreamQ,
            sizeof(pstThis->stStreamQ) / sizeof(pstThis->stStreamQ[0]));
        eSmeRet = E_SME_DEMUX_RET_SUCCESS;
    }while(ICS_FALSE);

    if(E_SME_DEMUX_RET_SUCCESS != eSmeRet)
    {
        ICS_LOGE("open fail(errno:%d), free resouce.", eSmeRet);
        if(NULL != pstContext)
        {
            avformat_close_input(&pstContext);
        }

        if(NULL != pstThis)
        {
            pstThis->eReadErr = E_SME_DEMUX_RET_SUCCESS;
            pstThis->eLastReadRes = E_SME_DEMUX_RET_SUCCESS;
            UTILS_MSAFEFREE(pstThis->pszFileName);
            pstThis->pstAVFmtCtx    = NULL;
            pstThis->pvDemuxFmt     = NULL;

            if (E_SME_MEM_OK != VOS_Memset_S(&pstThis->stURLProt,
                sizeof(pstThis->stURLProt), 0, sizeof(pstThis->stURLProt)))
            {
                ICS_LOGE("VOS_Memset_S return failed !!!");
                eSmeRet = E_SME_DEMUX_RET_NOMEM;
            }
        }
    }

    ICS_LOGD("SME_OpenDemuxOfFFMpeg out");

    return eSmeRet;
}

static V_VOID
SME_CloseDemuxOfFFMpeg(INOUT SMEDEMUXHDL pvDemuxHdl)
{
    PST_SME_DEMUX_HDL pstThis = NULL;

    ICS_LOGI("SME_CloseDemuxOfFFMpeg in");

    UTILS_MLOGE_RET_IF(!pvDemuxHdl, ("Handle is null."));
    pstThis = (PST_SME_DEMUX_HDL)pvDemuxHdl;

    UTILS_MLOGW_RET_IF(!pstThis->bOpened, ("not open yet."));

    avformat_close_input (&pstThis->pstAVFmtCtx);

    FreeStreams(&(pstThis->stStreams));
    UTILS_MSAFEFREE(pstThis->pszFileName);
    FreeUrlOptions(&(pstThis->stUrlOptions));

    if (E_SME_MEM_OK != VOS_Memset_S(&pstThis->stURLProt,
        sizeof(pstThis->stURLProt), 0, sizeof(pstThis->stURLProt)))
    {
        ICS_LOGE("VOS_Memset_S return failed !!!");
    }

    DeInitStreamQueueArray(pstThis->stStreamQ,
        sizeof(pstThis->stStreamQ) / sizeof(pstThis->stStreamQ[0]));
    pstThis->bOpened = ICS_FALSE;
    pstThis->bStopReadFf = ICS_TRUE;

    ICS_LOGI("SME_CloseDemuxOfFFMpeg out");

    return;
}

/*add begin, for read frame eos*/
static inline V_BOOL IsReadFrameEof(V_INT32 i32Ret, AVIOContext *pb)
{
    if(i32Ret >= 0)
    {
        return ICS_FALSE;
    }

    if(AVERROR_EOF == i32Ret)
    {
        ICS_LOGE("AVERROR_EOF");
        return ICS_TRUE;
    }

    if(pb)
    {
        if(avio_feof(pb) && (!pb->error))
        {
            ICS_LOGE("url_feof");
            return ICS_TRUE;
        }
    }

    return ICS_FALSE;
}
/*end*/
#if 0
static E_SME_DEMUX_RET
SME_ReadFrameOfFFMpeg(IN SMEDEMUXHDL pvDemuxHdl,
                        IN V_INT32 i32StreamIdx,
                        INOUT ST_SME_FRAMEBUFFER* pstBuffer)
{
    V_INT32 i32Ret = -1;
	E_SME_DEMUX_RET eSmeRet = E_SME_DEMUX_RET_SUCCESS;
    PST_SME_DEMUX_HDL pstThis = NULL;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    PST_SME_MEDIA_FRAME pstFrameTmp = NULL;
    V_VOID *pvAllocCtx = NULL;
    AVPacket stAVPkt;
    AVPacket* pstAVPkt = NULL;
    AVRational stAVTimeBase;

	do
	{
	    //参数检查
        UTILS_MLOGE_BRK_VAL_IF(!pvDemuxHdl || !pstBuffer, eSmeRet,
            E_SME_DEMUX_RET_INVALID_ARG, ("invalid param."));

        pstThis = (PST_SME_DEMUX_HDL)pvDemuxHdl;
        pstThisLogCtx = &pstThis->stLogCtx;

        //必须指定内存分配器
        UTILS_MLOGE_BRK_VAL_IF(!(pstBuffer->stAllocator.pfnLockBuf)
            || !(pstBuffer->stAllocator.pfnUnLockBuf), eSmeRet,
            E_SME_DEMUX_RET_INVALID_ARG, ("alloc buff func is null."));

        //ffmpeg不支持按stream_id读取帧
        UTILS_MLOGE_BRK_VAL_IF(i32StreamIdx != -1, eSmeRet,
            E_SME_DEMUX_RET_INVALID_ARG, ("not support read frame by stream_id!"));

        //demuxer状态检查
        UTILS_MLOGE_BRK_VAL_IF(!pstThis->bOpened, eSmeRet, E_SME_DEMUX_RET_NOT_OPENNED,
            ("cannot readframe, please open demuxer first."));

        //读帧
        av_init_packet(&stAVPkt);
        pstAVPkt = &stAVPkt;
        i32Ret = av_read_frame (pstThis->pstAVFmtCtx, &stAVPkt);

        /*add begin, for read frame eos*/
        UTILS_MLOGW_BRK_VAL_IF(IsReadFrameEof(i32Ret, pstThis->pstAVFmtCtx->pb),
                eSmeRet, E_SME_DEMUX_RET_EOS, ("ffmpeg read frame eos."));
        /*end*/

        UTILS_MLOGE_BRK_VAL_IF(i32Ret < 0, eSmeRet,
            E_SME_DEMUX_RET_NO_DATA, ("ffmpeg read frame fail."));

        /** BEGIN: Modified for DTS2014080802671 by mamingqiang(m00204629), 2014/08/08 */
        /*add begin, for stream index > max*/
        UTILS_MLOGW_BRK_VAL_IF(pstAVPkt->stream_index >= M_SME_STREAM_NUM_MAX, eSmeRet,
            E_SME_DEMUX_RET_NOMEM, ("stream_index > max."));
        /*end*/
        /** END: Modified for DTS2014080802671 by mamingqiang(m00204629), 2014/08/08 */

        //获取空Buffer
        pvAllocCtx = pstBuffer->stAllocator.pvAllocCtx;
        pstFrameTmp = (PST_SME_MEDIA_FRAME)(pstBuffer->stAllocator.pfnLockBuf(
            pvAllocCtx, (V_UINT32)(sizeof(ST_SME_MEDIA_FRAME) + pstAVPkt->size),
            pstAVPkt->stream_index));
        UTILS_MLOGE_BRK_VAL_IF(!pstFrameTmp, eSmeRet, E_SME_DEMUX_RET_NOMEM,
            ("alloc frame buffer fail."));

        //拷贝帧数据
        pstFrameTmp->eType
            = pstThis->stStreams.pstMedia[pstAVPkt->stream_index]->eMediaType;
        pstFrameTmp->i32StreamIdx = pstAVPkt->stream_index;
        pstFrameTmp->u32DataLen = (V_UINT32)pstAVPkt->size;

        if (E_SME_MEM_OK != VOS_Memcpy_S(
            pstFrameTmp->pu8DataBuf,
            (V_SIZE)pstAVPkt->size,
            pstAVPkt->data,
            (V_SIZE)pstAVPkt->size))
        {
            ICS_LOGE("VOS_Memcpy_S return failed !!!");
            eSmeRet = E_SME_DEMUX_RET_NOMEM;
            break;
        }

        //保存帧类型信息
        V_UINT32 tempFlags = (V_UINT32)(pstAVPkt->flags);
        if((tempFlags & AV_PKT_FLAG_KEY) == AV_PKT_FLAG_KEY)
        {
            pstFrameTmp->stInfo.eFrameType = E_SME_MEDIA_FRAME_TYPE_IDR;
        }
        else
        {
            pstFrameTmp->stInfo.eFrameType = E_SME_MEDIA_FRAME_TYPE_OTHER;
        }
        //保存其他帧信息
        stAVTimeBase
            = pstThis->pstAVFmtCtx->streams[pstAVPkt->stream_index]->time_base;
        pstFrameTmp->stInfo.i32TsBaseNum = stAVTimeBase.num;
        pstFrameTmp->stInfo.i32TsBaseDen = stAVTimeBase.den;

        ICS_LOGI("read_frame_from_ffmpeg:idx=%d,BaseNum=%d,den=%d,pts=%lld,dts=%lld,"
            "pkt_duration=%d,iskey=%d",
            pstAVPkt->stream_index,
            pstFrameTmp->stInfo.i32TsBaseNum, pstFrameTmp->stInfo.i32TsBaseDen,
            pstAVPkt->pts, pstAVPkt->dts, pstAVPkt->duration,
            pstAVPkt->flags & AV_PKT_FLAG_KEY);

        if(pstAVPkt->pts >= 0)
        {
            pstFrameTmp->stInfo.i64Pts = (pstAVPkt->pts * (V_INT64)M_SME_SEC_TO_MS);
        }
        else if(pstAVPkt->dts >= 0)
        {
            pstFrameTmp->stInfo.i64Pts = (pstAVPkt->dts * (V_INT64)M_SME_SEC_TO_MS);
        }
        else
        {
            pstFrameTmp->stInfo.i64Pts = -1;//
        }

        if(pstAVPkt->dts >= 0)
        {
            pstFrameTmp->stInfo.i64Dts = (pstAVPkt->dts * (V_INT64)M_SME_SEC_TO_MS);
        }
        else
        {
            pstFrameTmp->stInfo.i64Dts = -1;
        }

        if(pstThis->i64BasePts[pstAVPkt->stream_index] < 0)
        {
            pstThis->i64BasePts[pstAVPkt->stream_index]
                = pstFrameTmp->stInfo.i64Pts;
            if(pstThis->i64BasePtsMin > pstFrameTmp->stInfo.i64Pts)
            {
                pstThis->i64BasePtsMin = pstFrameTmp->stInfo.i64Pts;
            }
            else
            {
                pstThis->i64BasePts[pstAVPkt->stream_index]
                    = pstThis->i64BasePtsMin;
            }
            ICS_LOGD("pstThis->i64BasePts[%d]=%lld, pstThis->i64BasePtsMin=%lld",
                pstAVPkt->stream_index, pstThis->i64BasePts[pstAVPkt->stream_index],
                pstThis->i64BasePtsMin);
        }
        pstFrameTmp->stInfo.i64Pts -= pstThis->i64BasePts[pstAVPkt->stream_index];

        if(pstThis->i64BaseDts[pstAVPkt->stream_index] < 0)
        {
            pstThis->i64BaseDts[pstAVPkt->stream_index]
                = pstFrameTmp->stInfo.i64Dts;
            if(pstThis->i64BaseDtsMin > pstFrameTmp->stInfo.i64Dts)
            {
                pstThis->i64BaseDtsMin = pstFrameTmp->stInfo.i64Dts;
            }
            else
            {
                pstThis->i64BaseDts[pstAVPkt->stream_index]
                    = pstThis->i64BaseDtsMin;
            }
            ICS_LOGD("pstThis->i64BaseDts[%d]=%lld, pstThis->i64BaseDtsMin=%lld",
                pstAVPkt->stream_index, pstThis->i64BaseDts[pstAVPkt->stream_index],
                pstThis->i64BaseDtsMin);
        }
        pstFrameTmp->stInfo.i64Dts -= pstThis->i64BaseDts[pstAVPkt->stream_index];
        ICS_LOGW("1 read index=%d,i64Pts=%lld,i64Dts=%lld,i64LastTSInBase=%lld,i64LastDurationInBase=%lld",
            pstAVPkt->stream_index,pstFrameTmp->stInfo.i64Pts,pstFrameTmp->stInfo.i64Dts,
            pstThis->i64LastTSInBase[pstAVPkt->stream_index],
            pstThis->i64LastDurationInBase[pstAVPkt->stream_index]);

        /** BEGIN: Added for DTS2014081400222 by liurongliang(l00180035), 2014/8/14 */
        if((pstFrameTmp->stInfo.i64Pts < 0)
            && (pstThis->i64LastTSInBase[pstAVPkt->stream_index] >= 0))
        {
            pstFrameTmp->stInfo.i64Pts
                = pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                + pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
        }

        if((pstFrameTmp->stInfo.i64Dts < 0)
            && (pstThis->i64LastTSInBase[pstAVPkt->stream_index] >= 0))
        {
            pstFrameTmp->stInfo.i64Dts
                = pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                + pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
        }
        //else
        //{
        //    ICS_LOGW("read index=%d,num = %d,den=%d,pts=%lld,dts=%lld, "
        //        "pstAVPkt.duration=%d",
        //        pstAVPkt->stream_index,pstFrameTmp->stInfo.i32TsBaseNum,
        //        pstFrameTmp->stInfo.i32TsBaseDen,pstAVPkt->pts,
        //        pstAVPkt->dts, pstAVPkt->duration);
        //}
        /** END:   Added for DTS2014081400222 by liurongliang(l00180035), 2014/8/14 */
        ICS_LOGW("2 read index=%d,i64Pts=%lld,i64Pts=%lld,i64LastTSInBase=%lld,i64LastDurationInBase=%lld",
            pstAVPkt->stream_index,pstFrameTmp->stInfo.i64Pts,pstFrameTmp->stInfo.i64Dts,
            pstThis->i64LastTSInBase[pstAVPkt->stream_index],
            pstThis->i64LastDurationInBase[pstAVPkt->stream_index]);

        if((pstAVPkt->duration * M_SME_SEC_TO_MS) <= pstFrameTmp->stInfo.i32TsBaseDen)
        {
            if(pstThis->i64LastTSInBase[pstAVPkt->stream_index] < 0)
            {
                pstFrameTmp->stInfo.u32Duration =
                    (V_UINT32)pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
            }
            else
            {
                if((pstFrameTmp->stInfo.i64Dts > 0)
                    && (pstFrameTmp->stInfo.i64Dts
                    > pstThis->i64LastTSInBase[pstAVPkt->stream_index]) )
                {
                    pstFrameTmp->stInfo.u32Duration
                        = (V_UINT32)(pstFrameTmp->stInfo.i64Dts
                        - pstThis->i64LastTSInBase[pstAVPkt->stream_index]);
                }
                else if(pstFrameTmp->stInfo.i64Pts
                    > pstThis->i64LastTSInBase[pstAVPkt->stream_index])
                {
                    pstFrameTmp->stInfo.u32Duration
                        = (V_UINT32)(pstFrameTmp->stInfo.i64Pts
                        - pstThis->i64LastTSInBase[pstAVPkt->stream_index]);
                }
                else
                {
                    pstFrameTmp->stInfo.u32Duration =
                        (V_UINT32)pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
                }
            }

            if(M_SME_TS_RESCALE((V_INT32)pstFrameTmp->stInfo.u32Duration,
                1, pstFrameTmp->stInfo.i32TsBaseNum,
                pstFrameTmp->stInfo.i32TsBaseDen) > SME_DEMUX_DEFAULT_DURATION_MAX)
            {
                 pstFrameTmp->stInfo.u32Duration =
                    pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
            }
        }
        else
        {
            pstFrameTmp->stInfo.u32Duration
                = (V_UINT32)(pstAVPkt->duration * M_SME_SEC_TO_MS);
        }
        ICS_LOGW("3 read index=%d,i64Pts=%lld,i64Dts=%lld,i64LastTSInBase=%lld,i64LastDurationInBase=%lld",
            pstAVPkt->stream_index,pstFrameTmp->stInfo.i64Pts,pstFrameTmp->stInfo.i64Dts,
            pstThis->i64LastTSInBase[pstAVPkt->stream_index],
            pstThis->i64LastDurationInBase[pstAVPkt->stream_index]);

        /** BEGIN:   Added for DTS2014081504842 by liurongliang(l00180035), 2014/8/19 */
        V_INT64 i64Delta = M_SME_TS_SCALETOBASE(
            GST_ES_MAX_FRAME_PTS_DELTA, 1,
            pstFrameTmp->stInfo.i32TsBaseNum, pstFrameTmp->stInfo.i32TsBaseDen);

        if(!pstThis->bFirstAfterSeek[pstAVPkt->stream_index])
        {
            //pts
            if((pstThis->i64LastTSInBase[pstAVPkt->stream_index] >= 0)
                && ((pstThis->i64LastTSInBase[pstAVPkt->stream_index] + i64Delta
                <= pstFrameTmp->stInfo.i64Pts)
                || (pstFrameTmp->stInfo.i64Pts + i64Delta
                <= pstThis->i64LastTSInBase[pstAVPkt->stream_index])))
            {
                pstFrameTmp->stInfo.i64Pts
                    = pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                    + pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
                pstFrameTmp->stInfo.u32Duration
                    = pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
            }

            //dts
            if((pstThis->i64LastTSInBase[pstAVPkt->stream_index] >= 0)
                && ((pstThis->i64LastTSInBase[pstAVPkt->stream_index] + i64Delta
                <= pstFrameTmp->stInfo.i64Dts)
                || (pstFrameTmp->stInfo.i64Dts + i64Delta
                <= pstThis->i64LastTSInBase[pstAVPkt->stream_index])))
            {
                pstFrameTmp->stInfo.i64Dts
                    = pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                    + pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
            }
        }
        else
        {
            pstThis->bFirstAfterSeek[pstAVPkt->stream_index] = ICS_FALSE;
        }
        /** END:   Added for DTS2014081504842 by liurongliang(l00180035), 2014/8/19 */
        ICS_LOGW("4 read index=%d,i64Pts=%lld,i64Dts=%lld,i64LastTSInBase=%lld,i64LastDurationInBase=%lld",
            pstAVPkt->stream_index,pstFrameTmp->stInfo.i64Pts,pstFrameTmp->stInfo.i64Dts,
            pstThis->i64LastTSInBase[pstAVPkt->stream_index],
            pstThis->i64LastDurationInBase[pstAVPkt->stream_index]);


        if(pstFrameTmp->stInfo.i64Dts >= 0)
        {
            pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                = pstFrameTmp->stInfo.i64Dts;
        }
        else
        {
            pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                = pstFrameTmp->stInfo.i64Pts;
        }

        pstThis->i64LastDurationInBase[pstAVPkt->stream_index]
            = (V_INT64)pstFrameTmp->stInfo.u32Duration;
        ICS_LOGW("5 read index=%d,i64Pts=%lld,i64Dts=%lld,i64LastTSInBase=%lld,i64LastDurationInBase=%lld",
            pstAVPkt->stream_index,pstFrameTmp->stInfo.i64Pts,pstFrameTmp->stInfo.i64Dts,
            pstThis->i64LastTSInBase[pstAVPkt->stream_index],
            pstThis->i64LastDurationInBase[pstAVPkt->stream_index]);

        //保存帧
        pstBuffer->pstMediaFrame = pstFrameTmp;

        //释放buffer
        pstBuffer->stAllocator.pfnUnLockBuf(pvAllocCtx, pstFrameTmp);
	}while(ICS_FALSE);

    av_free_packet (pstAVPkt);

	return eSmeRet;
}
#else

#define MAX_VIDEO_DURATION 100             //unit: ms
static E_SME_DEMUX_RET
SME_ReadFrameOfFFMpeg(IN SMEDEMUXHDL pvDemuxHdl,
                        IN V_INT32 i32StreamIdx,
                        INOUT ST_SME_FRAMEBUFFER* pstBuffer)
{
    V_INT32 i32Ret = -1;
    E_SME_DEMUX_RET eSmeRet = E_SME_DEMUX_RET_SUCCESS;
    PST_SME_DEMUX_HDL pstThis = NULL;
    PST_SME_MEDIA_FRAME pstFrameTmp = NULL;
    AVPacket stAVPkt;
    AVPacket* pstAVPkt = NULL;
    AVRational stAVTimeBase;
    AVStream *pAvStream = NULL;

    do
    {
        //参数检查
        UTILS_MLOGE_BRK_VAL_IF(!pvDemuxHdl || !pstBuffer, eSmeRet,
            E_SME_DEMUX_RET_INVALID_ARG, ("invalid param."));

        pstThis = (PST_SME_DEMUX_HDL)pvDemuxHdl;

        //必须指定内存分配器
        UTILS_MLOGE_BRK_VAL_IF(!(pstBuffer->stAllocator.pfnLockBuf)
            || !(pstBuffer->stAllocator.pfnUnLockBuf), eSmeRet,
            E_SME_DEMUX_RET_INVALID_ARG, ("alloc buff func is null."));

        //ffmpeg不支持按stream_id读取帧
        UTILS_MLOGE_BRK_VAL_IF(i32StreamIdx != -1, eSmeRet,
            E_SME_DEMUX_RET_INVALID_ARG, ("not support read frame by stream_id!"));

        //demuxer status check
        UTILS_MLOGE_BRK_VAL_IF(!pstThis->bOpened, eSmeRet, E_SME_DEMUX_RET_NOT_OPENNED,
            ("cannot readframe, please open demuxer first."));

        //demuxer I/O status check
        UTILS_MLOGE_BRK_VAL_IF(pstThis->bStopReadFf, eSmeRet, E_SME_DEMUX_RET_STOP_READ_DATA,
            ("stop read frame, bStopReadFf=1"));

        /*1.先从packetlist里面，读取帧。
          2.如果有帧那么返回;
          3.如果没有，并且av_read_frame错误码为NONE，那么从a，v_read_frame中读取，否则返回错误。
          4.如果成功那么加入对应的packetlist，校准改list中的dts及duration。
          5.如果不成功，则记录av_read_frame错误码。强制校准所有packetlist。
          6.seek时清除packetlist。清除av_read_frame错误码。
        */
        if(E_SME_DEMUX_RET_SUCCESS != pstThis->eLastReadRes)
        {
            //if last return an error, and the app call again, we clear the err.
            pstThis->eReadErr = E_SME_DEMUX_RET_SUCCESS;//av read error;
            ICS_LOGW("lastErr=%d, read again,clear avread err", pstThis->eLastReadRes);
        }

        V_INT32 i32StreamIndex = -1;
        V_INT32 i32RdStep = 0;//0:read from pkt list;1:av_read_frame;2:done
        V_BOOL  bExitRead = ICS_FALSE;
        while(!bExitRead && !pstThis->bStopReadFf)
        {
            switch (i32RdStep)
            {
                case 0:
                {
                    V_BOOL bRes = ICS_TRUE;

                    if (E_SME_MEM_OK != VOS_Memset_S(&stAVPkt,
                        sizeof(stAVPkt), 0, sizeof(stAVPkt)))
                    {
                        ICS_LOGE("VOS_Memset_S return failed !!!");
                    }

                    av_init_packet(&stAVPkt);
                    pstAVPkt = &stAVPkt;
                    bRes = SME_ReadFrameFromePktlist(pstThis, pstAVPkt, i32StreamIndex);
                    if(bRes)
                    {
                        eSmeRet = E_SME_DEMUX_RET_SUCCESS;
                        i32RdStep = 2;
                    }
                    else if(!bRes && ICS_SUCCESS != pstThis->eReadErr)
                    {
                        eSmeRet = pstThis->eReadErr;
                        i32RdStep = 2;
                    }
                    else/* if(!bRes)*/
                    {
                        i32RdStep = 1;
                    }
                    break;
                }
                case 1:
                {
                    //here we need read by av_read_frame
                    if (E_SME_MEM_OK != VOS_Memset_S(&stAVPkt,
                        sizeof(stAVPkt), 0, sizeof(stAVPkt)))
                    {
                        ICS_LOGE("VOS_Memset_S return failed !!!");
                    }

                    av_init_packet(&stAVPkt);
                    pstAVPkt = &stAVPkt;
                    i32Ret = av_read_frame (pstThis->pstAVFmtCtx, &stAVPkt);
                    if(IsReadFrameEof(i32Ret, pstThis->pstAVFmtCtx->pb))
                    {
                        pstThis->eReadErr = E_SME_DEMUX_RET_EOS;
                        //force to revise all stream.
                        SME_RevisePktlistDts(pstThis, -1, ICS_TRUE);
                        i32RdStep = 0;
                        ICS_LOGW("av_read_frame return eos!");
                        break;
                    }

                    // BEGIN: Added for SME-254  2016/8/30
                    // From:   ./ffmpeg/doc/errno.txt:
                    // EAGAIN  POSIX,  Resource temporarily unavailable
                    if(i32Ret == AVERROR(EAGAIN))
                    {
                        // continue to read
                        ICS_LOGI("av_read_frame return nodata, continue to read, i32Ret=%d", i32Ret);
                        break;
                    }
                    // END: Added for SME-254  2016/8/30

                    if(i32Ret < 0)
                    {
                        pstThis->eReadErr = E_SME_DEMUX_RET_NO_DATA;

                        //force to revise all stream.
                        SME_RevisePktlistDts(pstThis, -1, ICS_TRUE);
                        i32RdStep = 0;
                        ICS_LOGW("av_read_frame return nodata! i32Ret=%d", i32Ret);
                        break;
                    }

                    if (pstAVPkt->size <= 0 || NULL == pstAVPkt->data) {
                        ICS_LOGI("ReadFromFFmpeg: no Pkt data for stream_idx=%d, size=%d, pos=%lld",
                            pstAVPkt->stream_index, pstAVPkt->size, pstAVPkt->pos);

                        av_free_packet (pstAVPkt);

                        // continue to read
                        break;
                    }

                    //if (pstAVPkt->stream_index == 0)
                    ICS_LOGI("ReadFromFFmpeg index=%d, pts=%lld,dts=%lld, "
                        "duration=%d,size=%d,iskey=%d, pos=%lld",
                        pstAVPkt->stream_index,
                        pstAVPkt->pts, pstAVPkt->dts, pstAVPkt->duration,
                        pstAVPkt->size, pstAVPkt->flags & AV_PKT_FLAG_KEY,
                        pstAVPkt->pos);


                    if(pstAVPkt->stream_index >= M_SME_STREAM_NUM_MAX)
                    {
                        //pstThis->eSmeRet = E_SME_DEMUX_RET_NOMEM;
                        //av_free_packet (pstAVPkt);
                        //continue read.
                        break;
                    }

                    /** BEGIN: Added for DTS2015082904200  by baihaili(b00311320), 2015/8/29 */
                    /**Check whether codec data is changed*/
                    pAvStream = pstThis->pstAVFmtCtx->streams[pstAVPkt->stream_index];
                    if (pAvStream->codec->extradata_changed_flag
                        && (pstAVPkt->flags & AV_PKT_FLAG_KEY)) {
                        ICS_LOGW("Codecdata updated!");
                          pstAVPkt->flags |= AV_PKT_FLAG_CODEC_CHANGED;
                        pAvStream->codec->extradata_changed_flag = 0;
                    }
                    /* END:   Added by baihaili(b00311320), 2015/8/29 */
                    SME_AddPktToPktlist(pstThis, pstAVPkt, pAvStream->codec->codec_type);
                    SME_RevisePktlistDts(pstThis,
                        pstAVPkt->stream_index, ICS_FALSE);
                    if(pstThis->stStreamQ[pstAVPkt->stream_index].i32ValidNum <= 1
                        && (pstThis->stStreamQ[pstAVPkt->stream_index].i32Len <= SME_DEMUX_PACKET_QUEUE_MAX_LEN/2))
                    {
                        //no frame output,continue; read.
                        break;
                    }
                    i32RdStep = 0;
                    i32StreamIndex = pstAVPkt->stream_index;
                    break;
                }
                case 2:
                {
                    bExitRead = ICS_TRUE;
                    break;
                }
                default:
                    ICS_LOGE("i32RdStep=%d, not by here why?", i32RdStep);
                break;
            }
        }

        if (pstThis->bStopReadFf) {
            eSmeRet = E_SME_DEMUX_RET_STOP_READ_DATA;
            ICS_LOGW("stop read data, bStopReadFf=1");
        }

        pstThis->eLastReadRes = eSmeRet;
        UTILS_MLOGW_BRK_IF((E_SME_DEMUX_RET_SUCCESS != eSmeRet) || (NULL == pstAVPkt),
            ("read frame error"));

        V_VOID *pvAllocCtx = pstBuffer->stAllocator.pvAllocCtx;

        pstFrameTmp = AllocBufferAndCopyPacket(pstThis,
                                               pstBuffer,
                                               pstAVPkt,
                                               pvAllocCtx);

        if(!pstFrameTmp)
        {
            eSmeRet = E_SME_DEMUX_RET_UN_SUPPORT_STREAM;
            ICS_LOGI("alloc frame buffer fail for stream_idx=%d, size=%d",
                pstAVPkt->stream_index, pstAVPkt->size);
            break;
        }

        //保存帧类型信息
        V_UINT32 tempFlags = (V_UINT32)(pstAVPkt->flags);
        if((tempFlags & AV_PKT_FLAG_KEY) == AV_PKT_FLAG_KEY)
        {
            pstFrameTmp->stInfo.eFrameType = E_SME_MEDIA_FRAME_TYPE_IDR;
        }
        else
        {
            pstFrameTmp->stInfo.eFrameType = E_SME_MEDIA_FRAME_TYPE_OTHER;
        }

        pstFrameTmp->stInfo.bIsDiscont = ICS_FALSE;

        //保存其他帧信息
        stAVTimeBase
            = pstThis->pstAVFmtCtx->streams[pstAVPkt->stream_index]->time_base;
        pstFrameTmp->stInfo.i32TsBaseNum = stAVTimeBase.num;
        pstFrameTmp->stInfo.i32TsBaseDen = stAVTimeBase.den;

        /*
        ICS_LOGI("ReadFrame index=%d,num = %d,den=%d,pts=%lld,dts=%lld, "
          "pstAVPkt.duration=%d,size=%d,iskey=%d, pos=%lld",
          pstAVPkt->stream_index,pstFrameTmp->stInfo.i32TsBaseNum,
          pstFrameTmp->stInfo.i32TsBaseDen,pstAVPkt->pts,
          pstAVPkt->dts, pstAVPkt->duration,
          pstAVPkt->size,pstAVPkt->flags & AV_PKT_FLAG_KEY,
          pstAVPkt->pos);
        */

        if(pstAVPkt->pts >= 0)
        {
            pstFrameTmp->stInfo.i64Pts = (pstAVPkt->pts * (V_INT64)M_SME_SEC_TO_MS);
        }
        else if(pstAVPkt->dts >= 0)
        {
            pstFrameTmp->stInfo.i64Pts = (pstAVPkt->dts * (V_INT64)M_SME_SEC_TO_MS);
        }
        else
        {
            pstFrameTmp->stInfo.i64Pts = -1;//
        }

        if(pstAVPkt->dts >= 0)
        {
            pstFrameTmp->stInfo.i64Dts = (pstAVPkt->dts * (V_INT64)M_SME_SEC_TO_MS);
        }
        else
        {
            pstFrameTmp->stInfo.i64Dts = -1;
        }

        // reset delta pts/dts and max pts after seek
        if (pstThis->bFirstAfterSeek[pstAVPkt->stream_index])
        {
            pstThis->i64DeltaPts[pstAVPkt->stream_index] = 0;
            pstThis->i64DeltaDts[pstAVPkt->stream_index] = 0;
            pstThis->i64MaxPts[pstAVPkt->stream_index] = -1;

            pstThis->u64VideoPkts = 0;
            pstThis->u64VideoDtsOutofOrderChangeCounts = 0;
            pstThis->u64AudioDtsOutofOrderChangeCounts = 0;
        }

        //判断跳变
        V_INT64 i64MaxDelta = M_SME_TS_SCALETOBASE(
            GST_ES_MAX_FRAME_PTS_DELTA, 1, pstFrameTmp->stInfo.i32TsBaseNum,
            pstFrameTmp->stInfo.i32TsBaseDen);
        if(!pstThis->bFirstAfterSeek[pstAVPkt->stream_index] && (pstThis->stStreams.i32Duration <= 0))
        {
            //pts
            if((pstThis->i64MaxPts[pstAVPkt->stream_index] >= 0)
                && ((pstThis->i64MaxPts[pstAVPkt->stream_index] + i64MaxDelta + pstThis->i64BasePts[pstAVPkt->stream_index]
                <= pstFrameTmp->stInfo.i64Pts + pstThis->i64DeltaPts[pstAVPkt->stream_index])
                || (pstFrameTmp->stInfo.i64Pts + i64MaxDelta + pstThis->i64DeltaPts[pstAVPkt->stream_index]
                <= pstThis->i64MaxPts[pstAVPkt->stream_index] + pstThis->i64BasePts[pstAVPkt->stream_index])))
            {
                pstThis->i64BasePts[pstAVPkt->stream_index] = pstFrameTmp->stInfo.i64Pts;
                pstThis->i64DeltaPts[pstAVPkt->stream_index]
                    = pstThis->i64MaxPts[pstAVPkt->stream_index]
                    + pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
                ICS_LOGI("pts_hop, i64MaxPts[%d]=%lld, i64MaxDelta=%lld, "
                    "i64BasePts=%lld, i64DeltaPts=%lld, lastduration=%lld",
                    pstAVPkt->stream_index, pstThis->i64MaxPts[pstAVPkt->stream_index],
                    i64MaxDelta, pstThis->i64BasePts[pstAVPkt->stream_index],
                    pstThis->i64DeltaPts[pstAVPkt->stream_index],
                    pstThis->i64LastDurationInBase[pstAVPkt->stream_index]);
            }

            //dts
            if((pstThis->i64LastTSInBase[pstAVPkt->stream_index] >= 0)
                && ((pstThis->i64LastTSInBase[pstAVPkt->stream_index] + i64MaxDelta + pstThis->i64BaseDts[pstAVPkt->stream_index]
                <= pstFrameTmp->stInfo.i64Dts + pstThis->i64DeltaDts[pstAVPkt->stream_index])
                || (pstFrameTmp->stInfo.i64Dts + i64MaxDelta + pstThis->i64DeltaDts[pstAVPkt->stream_index]
                <= pstThis->i64LastTSInBase[pstAVPkt->stream_index] + pstThis->i64BaseDts[pstAVPkt->stream_index])))
            {
                pstThis->i64BaseDts[pstAVPkt->stream_index] = pstFrameTmp->stInfo.i64Dts;
                pstThis->i64DeltaDts[pstAVPkt->stream_index]
                    = pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                    + pstThis->i64LastDurationInBase[pstAVPkt->stream_index];

                ICS_LOGI("dts_hop, i64LastTSInBase[%d]=%lld, i64MaxDelta=%lld, "
                    "i64BaseDts=%lld, i64DeltaDts=%lld, lastduration=%lld",
                    pstAVPkt->stream_index, pstThis->i64LastTSInBase[pstAVPkt->stream_index],
                    i64MaxDelta, pstThis->i64BaseDts[pstAVPkt->stream_index],
                    pstThis->i64DeltaDts[pstAVPkt->stream_index],
                    pstThis->i64LastDurationInBase[pstAVPkt->stream_index]);
            }
        }

        if(pstThis->i64BasePts[pstAVPkt->stream_index] < 0)
        {
            pstThis->i64BasePts[pstAVPkt->stream_index]
                = pstFrameTmp->stInfo.i64Pts;
            if(pstThis->i64BasePtsMin > pstFrameTmp->stInfo.i64Pts)
            {
                pstThis->i64BasePtsMin = pstFrameTmp->stInfo.i64Pts;
            }
            else
            {
                pstThis->i64BasePts[pstAVPkt->stream_index]
                    = pstThis->i64BasePtsMin;
            }
            ICS_LOGI("pstThis->i64BasePts[%d]=%lld, "
            "pstThis->i64BasePtsMin=%lld, pts=%lld, DeltaPts=%lld",
                pstAVPkt->stream_index,
                pstThis->i64BasePts[pstAVPkt->stream_index],
                pstThis->i64BasePtsMin,
                pstFrameTmp->stInfo.i64Pts,
                pstThis->i64DeltaPts[pstAVPkt->stream_index]);
        }
        pstFrameTmp->stInfo.i64Pts = (pstFrameTmp->stInfo.i64Pts -
            pstThis->i64BasePts[pstAVPkt->stream_index])
            + pstThis->i64DeltaPts[pstAVPkt->stream_index];

        if(pstThis->i64BaseDts[pstAVPkt->stream_index] < 0)
        {
            pstThis->i64BaseDts[pstAVPkt->stream_index]
                = pstFrameTmp->stInfo.i64Dts;
            if(pstThis->i64BaseDtsMin > pstFrameTmp->stInfo.i64Dts)
            {
                pstThis->i64BaseDtsMin = pstFrameTmp->stInfo.i64Dts;
            }
            else
            {
                pstThis->i64BaseDts[pstAVPkt->stream_index]
                    = pstThis->i64BaseDtsMin;
            }

            ICS_LOGI("pstThis->i64BaseDts[%d]=%lld, pstThis->i64BaseDtsMin=%lld",
                pstAVPkt->stream_index, pstThis->i64BaseDts[pstAVPkt->stream_index],
                pstThis->i64BaseDtsMin);
        }

        /*
        V_INT64 adjusted_dts = (pstFrameTmp->stInfo.i64Dts
            - pstThis->i64BaseDts[pstAVPkt->stream_index])
            + pstThis->i64DeltaDts[pstAVPkt->stream_index];

        ICS_LOGI(
            "DTS: index=%d, raw_dts=%lld, dts=%lld, duration=%d, base_dts=%lld, delta_dts=%lld, adjusted_dts=%lld",
            pstAVPkt->stream_index,
            pstAVPkt->dts,
            pstFrameTmp->stInfo.i64Dts,
            pstFrameTmp->stInfo.u32Duration,
            pstThis->i64BaseDts[pstAVPkt->stream_index],
            pstThis->i64DeltaDts[pstAVPkt->stream_index],
            adjusted_dts);
        */

        pstFrameTmp->stInfo.i64Dts = (pstFrameTmp->stInfo.i64Dts
            - pstThis->i64BaseDts[pstAVPkt->stream_index])
            + pstThis->i64DeltaDts[pstAVPkt->stream_index];

        /*
        ICS_LOGI(
            "Read_PTS_DTS: read index=%d, i64Pts=%lld, i64Dts=%lld, "
            "i64LastTSInBase=%lld, i64LastDurationInBase=%lld, i64DeltaDts=%lld",
            pstAVPkt->stream_index,
            pstFrameTmp->stInfo.i64Pts,
            pstFrameTmp->stInfo.i64Dts,
            pstThis->i64LastTSInBase[pstAVPkt->stream_index],
            pstThis->i64LastDurationInBase[pstAVPkt->stream_index],
            pstThis->i64DeltaDts[pstAVPkt->stream_index]);
        */

        /** BEGIN: Added for DTS2014081400222 by liurongliang(l00180035), 2014/8/14 */
        if((pstFrameTmp->stInfo.i64Pts < 0)
            && (pstThis->i64LastTSInBase[pstAVPkt->stream_index] >= 0))
        {
            pstFrameTmp->stInfo.i64Pts
                = pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                + pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
        }

        if((pstFrameTmp->stInfo.i64Dts < 0)
            && (pstThis->i64LastTSInBase[pstAVPkt->stream_index] >= 0))
        {
            pstFrameTmp->stInfo.i64Dts
                = pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                + pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
        }
        //else
        //{
        //    ICS_LOGW("read index=%d,num = %d,den=%d,pts=%lld,dts=%lld, "
        //        "pstAVPkt.duration=%d",
        //        pstAVPkt->stream_index,pstFrameTmp->stInfo.i32TsBaseNum,
        //        pstFrameTmp->stInfo.i32TsBaseDen,pstAVPkt->pts,
        //        pstAVPkt->dts, pstAVPkt->duration);
        //}
        /** END:   Added for DTS2014081400222 by liurongliang(l00180035), 2014/8/14 */
        //ICS_LOGW("2 read index=%d,i64Pts=%lld,i64Pts=%lld,i64LastTSInBase=%lld,i64LastDurationInBase=%lld",
        //    pstAVPkt->stream_index,pstFrameTmp->stInfo.i64Pts,pstFrameTmp->stInfo.i64Dts,
        //    pstThis->i64LastTSInBase[pstAVPkt->stream_index],
        //    pstThis->i64LastDurationInBase[pstAVPkt->stream_index]);

        if((pstAVPkt->duration * M_SME_SEC_TO_MS * pstFrameTmp->stInfo.i32TsBaseNum)
            <= pstFrameTmp->stInfo.i32TsBaseDen)
        {
            if(pstThis->i64LastTSInBase[pstAVPkt->stream_index] < 0)
            {
                pstFrameTmp->stInfo.u32Duration =
                    (V_UINT32)pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
            }
            else
            {
                /* BEGIN: Added for DTS2015102708095 by liguohuai(l00321198),2015/11/06th */
                if(E_SME_MEDIA_TYPE_VIDEO == pstFrameTmp->eType)
                {
                    V_UINT32 duration = 0;

                    if ((pstFrameTmp->stInfo.i64Dts > 0)
                        && (pstFrameTmp->stInfo.i64Dts > pstThis->i64LastTSInBase[pstAVPkt->stream_index])
                        && pstFrameTmp->stInfo.i32TsBaseDen > 0)
                    {

                        duration = (V_UINT32)(pstFrameTmp->stInfo.i64Dts
                                   - pstThis->i64LastTSInBase[pstAVPkt->stream_index]);

                        if ((duration * (V_UINT32)pstFrameTmp->stInfo.i32TsBaseNum)/(V_UINT32)pstFrameTmp->stInfo.i32TsBaseDen
                            < MAX_VIDEO_DURATION)
                        {
                            pstFrameTmp->stInfo.u32Duration = duration;

                            //ICS_LOGI("SETUP_duration=%d with dts=%lld, last_ts=%lld",
                            //    pstFrameTmp->stInfo.u32Duration,
                            //    pstFrameTmp->stInfo.i64Dts,
                            //    pstThis->i64LastTSInBase[pstAVPkt->stream_index]);
                        }
                        else
                        {
                            ICS_LOGI(
                                "duration=%d ms got from dts increase is invalid, raw_duration=%d, dts=%lld, last_ts=%lld",
                                (duration * (V_UINT32)pstFrameTmp->stInfo.i32TsBaseNum)/(V_UINT32)pstFrameTmp->stInfo.i32TsBaseDen,
                                pstFrameTmp->stInfo.u32Duration,
                                pstFrameTmp->stInfo.i64Dts,
                                pstThis->i64LastTSInBase[pstAVPkt->stream_index]);

                            duration = 0;
                        }

                    }

                    if (0 == duration)
                    {
                        pstFrameTmp->stInfo.u32Duration =
                            (V_UINT32)pstThis->i64LastDurationInBase[pstAVPkt->stream_index];

                        //ICS_LOGI(
                        //    "SETUP_duration=%d with last_ts=%lld, pts=%lld, dts=%lld",
                        //    pstFrameTmp->stInfo.u32Duration,
                        //    pstThis->i64LastTSInBase[pstAVPkt->stream_index],
                        //    pstFrameTmp->stInfo.i64Pts,
                        //    pstFrameTmp->stInfo.i64Dts);
                    }
                }
                /* END: Added for DTS2015102708095 by liguohuai(l00321198),2015/11/06th */
                else if(E_SME_MEDIA_TYPE_AUDIO == pstFrameTmp->eType)
                {
                    if((pstFrameTmp->stInfo.i64Dts > 0)
                        && (pstFrameTmp->stInfo.i64Dts
                        > pstThis->i64LastTSInBase[pstAVPkt->stream_index]))
                    {
                        pstFrameTmp->stInfo.u32Duration
                            = (V_UINT32)(pstFrameTmp->stInfo.i64Dts
                            - pstThis->i64LastTSInBase[pstAVPkt->stream_index]);

                        //ICS_LOGI("SETUP_duration=%d with dts=%lld, last_ts=%lld",
                        //    pstFrameTmp->stInfo.u32Duration,
                        //    pstFrameTmp->stInfo.i64Dts,
                        //    pstThis->i64LastTSInBase[pstAVPkt->stream_index]);
                    }
                    else if(pstFrameTmp->stInfo.i64Pts
                        > pstThis->i64LastTSInBase[pstAVPkt->stream_index])
                    {
                        pstFrameTmp->stInfo.u32Duration
                            = (V_UINT32)(pstFrameTmp->stInfo.i64Pts
                            - pstThis->i64LastTSInBase[pstAVPkt->stream_index]);

                        //ICS_LOGI("SETUP_duration=%d with pts=%lld, last_ts=%lld",
                        //    pstFrameTmp->stInfo.u32Duration,
                        //    pstFrameTmp->stInfo.i64Pts,
                        //    pstThis->i64LastTSInBase[pstAVPkt->stream_index]);
                    }
                    else
                    {
                        pstFrameTmp->stInfo.u32Duration =
                            (V_UINT32)pstThis->i64LastDurationInBase[pstAVPkt->stream_index];

                        //ICS_LOGI(
                        //    "SETUP_duration=%d with last_ts=%lld, pts=%lld, dts=%lld",
                        //    pstFrameTmp->stInfo.u32Duration,
                        //    pstThis->i64LastTSInBase[pstAVPkt->stream_index],
                        //    pstFrameTmp->stInfo.i64Pts,
                        //    pstFrameTmp->stInfo.i64Dts);
                    }
                }
            }

            if(M_SME_TS_RESCALE((V_INT32)pstFrameTmp->stInfo.u32Duration,
                1, pstFrameTmp->stInfo.i32TsBaseNum,
                pstFrameTmp->stInfo.i32TsBaseDen) > SME_DEMUX_DEFAULT_DURATION_MAX)
            {
                 pstFrameTmp->stInfo.u32Duration =
                    (V_UINT32)pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
            }
        }
        else
        {
            pstFrameTmp->stInfo.u32Duration
                = (V_UINT32)(pstAVPkt->duration * M_SME_SEC_TO_MS);
        }
        //ICS_LOGW("3 read index=%d,i64Pts=%lld,i64Dts=%lld,i64LastTSInBase=%lld,i64LastDurationInBase=%lld",
        //    pstAVPkt->stream_index,pstFrameTmp->stInfo.i64Pts,pstFrameTmp->stInfo.i64Dts,
        //    pstThis->i64LastTSInBase[pstAVPkt->stream_index],
        //    pstThis->i64LastDurationInBase[pstAVPkt->stream_index]);

        /** BEGIN:   Added for DTS2014081504842 by liurongliang(l00180035), 2014/8/19 */
        V_INT64 i64Delta = M_SME_TS_SCALETOBASE(
            GST_ES_MAX_FRAME_PTS_DELTA, 1,
            pstFrameTmp->stInfo.i32TsBaseNum, pstFrameTmp->stInfo.i32TsBaseDen);

        V_BOOL dts_err = ICS_FALSE;

        if (!pstThis->bFirstAfterSeek[pstAVPkt->stream_index])
        {
            //pts
            if ((pstThis->i64LastTSInBase[pstAVPkt->stream_index] >= 0)
                 && ((pstThis->i64LastTSInBase[pstAVPkt->stream_index] + i64Delta
                    <= pstFrameTmp->stInfo.i64Pts)
                 || (pstFrameTmp->stInfo.i64Pts + i64Delta
                    <= pstThis->i64LastTSInBase[pstAVPkt->stream_index])))
            {
                pstFrameTmp->stInfo.bIsDiscont = ICS_TRUE;

                ICS_LOGI(
                    "PTS_IsDiscont=1, read index=%d, i64Pts=%lld, i64Dts=%lld, "
                    "i64LastTSInBase=%lld, i64Delta=%lld",
                    pstAVPkt->stream_index,
                    pstFrameTmp->stInfo.i64Pts,
                    pstFrameTmp->stInfo.i64Dts,
                    pstThis->i64LastTSInBase[pstAVPkt->stream_index],
                    i64Delta);
            }

            //dts
            if((pstThis->i64LastTSInBase[pstAVPkt->stream_index] >= 0)
                && ((pstThis->i64LastTSInBase[pstAVPkt->stream_index] + i64Delta
                    <= pstFrameTmp->stInfo.i64Dts)
                    || (pstFrameTmp->stInfo.i64Dts + i64Delta
                    <= pstThis->i64LastTSInBase[pstAVPkt->stream_index])))
            {
                dts_err = ICS_TRUE;
                if (llabs(pstFrameTmp->stInfo.i64Pts - pstFrameTmp->stInfo.i64Dts) <= i64Delta)
                {
                    pstFrameTmp->stInfo.bIsDiscont = ICS_TRUE;
                    ICS_LOGI(
                        "DTS_IsDiscont=1, read index=%d, i64Pts=%lld, i64Dts=%lld, "
                        "i64LastTSInBase=%lld, i64Delta=%lld",
                        pstAVPkt->stream_index,
                        pstFrameTmp->stInfo.i64Pts,
                        pstFrameTmp->stInfo.i64Dts,
                        pstThis->i64LastTSInBase[pstAVPkt->stream_index],
                        i64Delta);
                } else
                {
                    ICS_LOGI(
                        "DTS_ERRO, read index=%d, i64Pts=%lld, i64Dts=%lld, "
                        "i64LastTSInBase=%lld, i64Delta=%lld, fixedDTS=%lld",
                        pstAVPkt->stream_index,
                        pstFrameTmp->stInfo.i64Pts,
                        pstFrameTmp->stInfo.i64Dts,
                        pstThis->i64LastTSInBase[pstAVPkt->stream_index],
                        i64Delta,
                        pstThis->i64LastTSInBase[pstAVPkt->stream_index]+pstThis->i64LastDurationInBase[pstAVPkt->stream_index]);
                    // FIX DTS,DTS2016042306599
                    pstFrameTmp->stInfo.i64Dts =
                        pstThis->i64LastTSInBase[pstAVPkt->stream_index]+pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
                }
            }
            else if((pstThis->i64LastTSInBase[pstAVPkt->stream_index] >= 0)
                && pstFrameTmp->stInfo.i64Dts < pstThis->i64LastTSInBase[pstAVPkt->stream_index])
            {
                //dts is < last, we must revise.

                ICS_LOGI(
                    "DTS_mis_order: stream_index=%d, dts=%lld < last dts %lld, "
                    "duration=%d, last_duration=%lld, pos=%lld, size=%d",
                    pstAVPkt->stream_index,
                    pstFrameTmp->stInfo.i64Dts,
                    pstThis->i64LastTSInBase[pstAVPkt->stream_index],
                    pstFrameTmp->stInfo.u32Duration,
                    pstThis->i64LastDurationInBase[pstAVPkt->stream_index],
                    pstAVPkt->pos, pstAVPkt->size);

                if (E_SME_MEDIA_TYPE_VIDEO == pstFrameTmp->eType)
                {
                    pstFrameTmp->stInfo.i64Dts
                    = pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                    + pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
                    pstFrameTmp->stInfo.i64Pts
                    = pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                    + pstThis->i64LastDurationInBase[pstAVPkt->stream_index];

                    pstThis->u64VideoDtsOutofOrderChangeCounts ++;
                }

                if (E_SME_MEDIA_TYPE_AUDIO == pstFrameTmp->eType)
                {
                    if ((pstThis->u64VideoDtsOutofOrderChangeCounts > 0 && pstThis->u64VideoPkts > 0)
                        || (pstThis->u64VideoPkts == 0))
                    {
                        pstFrameTmp->stInfo.i64Dts
                            = pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                            + pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
                        pstFrameTmp->stInfo.i64Pts
                            = pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                            + pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
                    }

                    pstThis->u64AudioDtsOutofOrderChangeCounts ++;
                }

                //pstFrameTmp->stInfo.u32Duration
                //    = pstThis->i64LastDurationInBase[pstAVPkt->stream_index];
            }
        }
        else
        {
            pstThis->bFirstAfterSeek[pstAVPkt->stream_index] = ICS_FALSE;
        }
        /** END:   Added for DTS2014081504842 by liurongliang(l00180035), 2014/8/19 */
        //ICS_LOGW("4 read index=%d,i64Pts=%lld,i64Dts=%lld,i64LastTSInBase=%lld,i64LastDurationInBase=%lld",
        //    pstAVPkt->stream_index,pstFrameTmp->stInfo.i64Pts,pstFrameTmp->stInfo.i64Dts,
        //    pstThis->i64LastTSInBase[pstAVPkt->stream_index],
        //    pstThis->i64LastDurationInBase[pstAVPkt->stream_index]);


        if(pstFrameTmp->stInfo.i64Dts >= 0 && !dts_err)
        {
            pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                = pstFrameTmp->stInfo.i64Dts;
        }
        else
        {
            pstThis->i64LastTSInBase[pstAVPkt->stream_index]
                = pstFrameTmp->stInfo.i64Pts;
        }

        pstThis->i64MaxPts[pstAVPkt->stream_index]
            = (pstThis->i64MaxPts[pstAVPkt->stream_index] > pstFrameTmp->stInfo.i64Pts)
            ? pstThis->i64MaxPts[pstAVPkt->stream_index] : pstFrameTmp->stInfo.i64Pts;
        pstThis->i64LastDurationInBase[pstAVPkt->stream_index]
            = (V_INT64)pstFrameTmp->stInfo.u32Duration;

        if (E_SME_MEDIA_TYPE_VIDEO == pstFrameTmp->eType)
        {
            pstThis->u64VideoPkts ++;
        }

        /*
        ICS_LOGI(
            "ReadFrameDone, type=%d index=%d,num = %d,den=%d, pts=%lld, dts=%lld=lastTS,"
            "pstAVPkt.duration=%d,size=%d,frametype=%d(2 is key), pos=%lld",
            pstFrameTmp->eType,pstFrameTmp->i32StreamIdx, pstFrameTmp->stInfo.i32TsBaseNum,
            pstFrameTmp->stInfo.i32TsBaseDen,pstFrameTmp->stInfo.i64Pts,
            pstFrameTmp->stInfo.i64Dts, pstFrameTmp->stInfo.u32Duration,
            pstFrameTmp->u32DataLen,pstFrameTmp->stInfo.eFrameType,
            pstAVPkt->pos);
        */

        //save frame.
        pstBuffer->pstMediaFrame = pstFrameTmp;

        //release buffer
        pstBuffer->stAllocator.pfnUnLockBuf(pvAllocCtx, pstFrameTmp);
	}while(ICS_FALSE);

    av_free_packet (pstAVPkt);

	return eSmeRet;
}
#endif

//
// return value:
//   0:  is not seekable
//   1:  is seekable
//   -1: parameter is invalid or error.
static V_INT32
SME_IsSeekableOfFFMpeg(IN SMEDEMUXHDL pvDemuxHdl,
                       IN V_INT32 i32StreamIdx)
{
    V_INT32 i32SearchIdx = 0;
    if (!pvDemuxHdl)
    {
        ICS_LOGW("pvDemuxHdl=%p, parameter invalid", pvDemuxHdl);
        return -1;
    }

    PST_SME_DEMUX_HDL  pstThis = (PST_SME_DEMUX_HDL) pvDemuxHdl;
    if (!pstThis->pstAVFmtCtx)
    {
        ICS_LOGW("pstAVFmtCtx could not found");
        return -1;
    }

    AVInputFormat *pstAVFmt = pstThis->pstAVFmtCtx->iformat;

    //if it is not avi format, handle it as seekable.
    if(pstAVFmt && strcmp(pstAVFmt->name, "avi"))
    {
        return 1;
    }

    if(i32StreamIdx >= 0)
    {
        i32SearchIdx = i32StreamIdx;
    }
    else
    {
        i32SearchIdx = av_find_default_stream_index (pstThis->pstAVFmtCtx);
        ICS_LOGI("av_find_default_stream_index idx = %d ", i32SearchIdx);

        if(i32SearchIdx >= (V_INT32)(pstThis->pstAVFmtCtx->nb_streams)
            || i32SearchIdx < 0)
        {
            ICS_LOGW("av_find_default_stream_index idx = %d is invalid", i32SearchIdx);
            return -1;
        }

    }

    return av_is_seekable(pstThis->pstAVFmtCtx, i32SearchIdx);
}

/** BEGIN:   Modified for DTS2014071000465 by liurongliang(l00180035), 2014/7/10 */
static V_INT32
SME_SeekToOfFFMpeg(IN SMEDEMUXHDL pvDemuxHdl,
                        IN V_INT32 i32SeekToms,
                        IN V_INT32 i32StreamIdx,
                        IN V_INT32 i32Flags)
{
    V_INT32             i32Ret          = -1;
    PST_SME_DEMUX_HDL   pstThis         = NULL;
    V_INT64             i64SeekTg       = 0;
    V_INT64             i64FFSeekTg     = 0;
    V_INT32             i32KeyFrameIdx  = -1;
    V_INT32             i32SearchIdx    = 0;
    V_INT32             i32BaseTime     = 0;
    V_INT32             i32BaseTimePts  = INT_MAX;
    V_INT32             i32BaseTimeDts  = INT_MAX;

    ICS_LOGI("SME_SeekToOfFFMpeg in, pvDemuxHdl=%p,seekto=%dms,Idx=%d,flags=%#x",
        pvDemuxHdl, i32SeekToms, i32StreamIdx, i32Flags);

    do
    {
        UTILS_MLOGE_BRK_VAL_IF(!pvDemuxHdl, i32Ret, E_SME_DEMUX_RET_INVALID_ARG,
            ("demuxer handle is null."));

        pstThis = (PST_SME_DEMUX_HDL) pvDemuxHdl;

        if(i32StreamIdx >= 0)
        {
            i32SearchIdx = i32StreamIdx;
        }
        else
        {
            i32SearchIdx = av_find_default_stream_index (pstThis->pstAVFmtCtx);
            ICS_LOGE("av_find_default_stream_index idx = %d ", i32SearchIdx);
            UTILS_MLOGE_BRK_VAL_IF(
                i32SearchIdx >= (V_INT32)(pstThis->pstAVFmtCtx->nb_streams) || i32SearchIdx < 0,
                i32Ret, -1,
                ("i32SearchIdx = %d beyond!", i32SearchIdx));
        }

        AVStream* pstStream = pstThis->pstAVFmtCtx->streams[i32SearchIdx];
        UTILS_MLOGE_BRK_VAL_IF(NULL == pstStream, i32Ret, -1,
            ("i32SearchIdx = %d stream is null", i32SearchIdx));

        /**< corret timestamp with base PTS or DTS */
        if(pstThis->i64BasePts[i32SearchIdx] > 0)
        {
            i32BaseTimePts = (V_INT32)av_rescale(
                pstThis->i64BasePts[i32SearchIdx],
                (V_INT64)pstStream->time_base.num,
                (V_INT64)pstStream->time_base.den);
        }
        if (pstThis->i64BaseDts[i32SearchIdx] > 0)
        {
            i32BaseTimeDts = (V_INT32)av_rescale(
                pstThis->i64BaseDts[i32SearchIdx],
                (V_INT64)pstStream->time_base.num,
                (V_INT64)pstStream->time_base.den);
        }

        i32BaseTime = M_SME_MIN(i32BaseTimePts, i32BaseTimeDts);  //unit: ms
        i32BaseTime = i32BaseTime == INT_MAX ? 0 : i32BaseTime;

        //unit: ms
        i64SeekTg = (V_INT64)i32SeekToms + (V_INT64)i32BaseTime;

        ICS_LOGI("streamidx=%d, basePts=%lld, baseDts=%lld,"
                      " baseresacle=%d ms, actual seek to %lld ms",
                      i32SearchIdx,
                      pstThis->i64BasePts[i32SearchIdx],
                      pstThis->i64BaseDts[i32SearchIdx],
                      i32BaseTime,
                      i64SeekTg);

        i64FFSeekTg = av_rescale((int64_t)i64SeekTg, (int64_t)pstStream->time_base.den,
            1000 * (int64_t)pstStream->time_base.num);

        UTILS_MLOGE_BRK_IF(i64FFSeekTg < 0, ("i64FFSeekTg=%lld < 0", i64FFSeekTg));

        if (i32Flags & AVSEEK_FLAG_BACKWARD) {
            ICS_LOGI("AVSEEK_FLAG_BACKWARD");
        } else {
            ICS_LOGI("not AVSEEK_FLAG_BACKWARD");
        }

        i32Ret = av_seek_frame (pstThis->pstAVFmtCtx, i32SearchIdx,
            (int64_t)i64FFSeekTg, i32Flags);

        UTILS_MLOGE_BRK_IF(i32Ret < 0, ("av_seek_frame fail."));

        pstThis->eReadErr = E_SME_DEMUX_RET_SUCCESS;
        pstThis->eLastReadRes = E_SME_DEMUX_RET_SUCCESS;
        SME_ClearPktlist(pstThis, -1);

        i32KeyFrameIdx = av_index_search_timestamp (pstStream, (int64_t)i64FFSeekTg,
            AVSEEK_FLAG_BACKWARD);

        if ((i32KeyFrameIdx >= 0)
            && (pstStream->index_entries[i32KeyFrameIdx].flags & AVINDEX_KEYFRAME))
        {
            AVRational stBq = {1, M_SME_SEC_TO_MS};
            i32Ret = (V_INT32)av_rescale_q(
                (int64_t)pstStream->index_entries[i32KeyFrameIdx].timestamp,
                pstStream->time_base, stBq);

            i32Ret -= i32BaseTime;

            /* BEGIN: Added for DTS2015032502016 by liurongliang(l00180035), 2015/3/26 */
            /*ffmpeg will usr dts for timestamp sometimes, and maybe < 0,
              we just revise it
            */
            if(i32Ret < 0)
            {
                ICS_LOGW("i32Ret < 0 ! revise it.");
                i32Ret = 0;
            }
            /* END:   Added for DTS2015032502016 by liurongliang(l00180035), 2015/3/26 */

            ICS_LOGI("pts = %lld ,num=%d, den=%d, seek_to=%d ms,flags=%#x, min_distance=%d",
                pstStream->index_entries[i32KeyFrameIdx].timestamp,
                pstStream->time_base.num,
                pstStream->time_base.den,
                i32Ret,
                pstStream->index_entries[i32KeyFrameIdx].flags,
                pstStream->index_entries[i32KeyFrameIdx].min_distance);
        }
        else
        {
            ICS_LOGW("av_index_search_timestamp: not found i32KeyFrameIdx = %d ", i32KeyFrameIdx);
        }

        for(V_INT32 i = 0; (i32Ret >= 0) && (i < M_SME_STREAM_NUM_MAX); i++)
        {
            pstThis->i64LastTSInBase[i] = -1;
            pstThis->bFirstAfterSeek[i] = ICS_TRUE;
        }
    }while(ICS_FALSE);

    ICS_LOGI("SME_SeekToOfFFMpeg out,ret=%d", i32Ret);

    return i32Ret;
}
/** END:   Modified for DTS2014071000465 by liurongliang(l00180035), 2014/7/10 */

//clean the data in ffmpeg and ffmpeg adapter
static void
SME_FlushFFMpeg(IN SMEDEMUXHDL pvDemuxHdl)
{
    PST_SME_DEMUX_HDL   pstThis = (PST_SME_DEMUX_HDL) pvDemuxHdl;

    ICS_LOGD("SME_Flush in, pvDemuxHdl=%p", pvDemuxHdl);

    UTILS_MLOGE_RET_IF(!pvDemuxHdl, ("demuxer handle is null"));

    /** Added on 2016/08/17, for ffmpeg2.8.x
     *  solve hls(implemented in GStreamer) seek issue:
     *  before seek, clean the data cached in ffmpeg.
     */
    avformat_flush(pstThis->pstAVFmtCtx);
    ICS_LOGI("SME_Flush avformat_flush done");

    for(V_INT32 i = 0; i < M_SME_STREAM_NUM_MAX; i++)
    {
        /** BEGIN: Added for DTS2015072305039 by baihaili(b00311320), 2015/7/25 */
        pstThis->i64LastTSInBase[i] = -1;
        /** END: Added for DTS2015072305039 by baihaili(b00311320), 2014/7/25 */
        pstThis->bFirstAfterSeek[i] = ICS_TRUE;
    }
    pstThis->eReadErr = E_SME_DEMUX_RET_SUCCESS;
    pstThis->eLastReadRes = E_SME_DEMUX_RET_SUCCESS;
    SME_ClearPktlist(pstThis, -1);

    return ;
}

static E_SME_DEMUX_RET
SME_GetStreamInfoOfFFMpeg(IN SMEDEMUXHDL pvDemuxHdl,
            INOUT ST_SME_STREAMINFO* pstStreamInfo)
{
    E_SME_DEMUX_RET eSmeRet = E_SME_DEMUX_RET_SUCCESS;
    PST_SME_DEMUX_HDL pstThis = NULL;
    V_UINT32 u32ValidStreams = 0;

    ICS_LOGI("SME_GetStreamInfoOfFFMpeg in");

    do
    {
        UTILS_MLOGE_BRK_VAL_IF(!pvDemuxHdl || !pstStreamInfo, eSmeRet,
            E_SME_DEMUX_RET_INVALID_ARG, ("invalid param."));

        pstThis = (PST_SME_DEMUX_HDL)pvDemuxHdl;

        pstStreamInfo->i32Duration = pstThis->stStreams.i32Duration;
        pstStreamInfo->i64FileSize = pstThis->stStreams.i64FileSize;
        pstStreamInfo->u32BitRate = pstThis->stStreams.u32BitRate;
        pstStreamInfo->u32NumStream = pstThis->stStreams.u32NumStream;

        if (E_SME_MEM_OK != VOS_Memcpy_S(
            pstStreamInfo->u32IdxOrderInPos,
            sizeof(V_UINT32)*M_SME_STREAM_NUM_MAX,
            pstThis->stStreams.u32IdxOrderInPos,
            sizeof(V_UINT32)*M_SME_STREAM_NUM_MAX))
        {
            ICS_LOGE("VOS_Memcpy_S return failed !!!");
            eSmeRet = E_SME_DEMUX_RET_NOMEM;
            break;
        }

        /*add begin, for stream index > max*/
        if(pstStreamInfo->u32NumStream > M_SME_STREAM_NUM_MAX)
        {
            ICS_LOGW("stream number is more than max num %d, use the first %d stream",
                    M_SME_STREAM_NUM_MAX, M_SME_STREAM_NUM_MAX);
            pstStreamInfo->u32NumStream = M_SME_STREAM_NUM_MAX;
        }
        /*end*/

        /* 获取媒体流信息 */
        /* 注意:此处会为每条流分配内存并返回给调用者，调用者使用完需释放 */
        for(V_UINT32 i = 0; i < pstStreamInfo->u32NumStream; i++)
        {
            PST_SME_MEDIAINFO pstMediaInfoTmp = NULL;
            V_UINT8 *pu8CfgDataTmp = NULL;
            V_UINT32 u32CfgDataSizeTmp = 0;
            V_UINT8 *pu8CfgDataNew = NULL;

            //存在无效stream的情况
            UTILS_CTN_IF(!pstThis->stStreams.pstMedia[i]);
            pu8CfgDataTmp = pstThis->stStreams.pstMedia[i]->stCfgData.pu8CfgData;
            u32CfgDataSizeTmp = pstThis->stStreams.pstMedia[i]->stCfgData.u32CfgDataSize;

            //初始化为NULL
            pstStreamInfo->pstMedia[i] = NULL;

            //拷贝流
            if(pstThis->stStreams.pstMedia[i]->eMediaType == E_SME_MEDIA_TYPE_VIDEO)
            {
                ICS_LOGD("media type:VIDEO");
                pstMediaInfoTmp = (PST_SME_MEDIAINFO)malloc(sizeof(ST_SME_VIDEOINFO));
                UTILS_MLOGW_CTN_IF(!pstMediaInfoTmp, ("no momery"));

                if (E_SME_MEM_OK != VOS_Memcpy_S(
                    pstMediaInfoTmp,
                    sizeof(ST_SME_VIDEOINFO),
                    pstThis->stStreams.pstMedia[i],
                    sizeof(ST_SME_VIDEOINFO)))
                {
                    ICS_LOGE("VOS_Memcpy_S return failed !!!");
                    eSmeRet = E_SME_DEMUX_RET_NOMEM;
                    free(pstMediaInfoTmp);

                    break;
                }
            }
            else if(pstThis->stStreams.pstMedia[i]->eMediaType == E_SME_MEDIA_TYPE_AUDIO)
            {
                ICS_LOGD("media type:AUDIO");
                pstMediaInfoTmp = (PST_SME_MEDIAINFO)malloc(sizeof(ST_SME_AUDIOINFO));
                UTILS_MLOGW_CTN_IF(!pstMediaInfoTmp, ("no momery"));

                if (E_SME_MEM_OK != VOS_Memcpy_S(
                    pstMediaInfoTmp,
                    sizeof(ST_SME_AUDIOINFO),
                    pstThis->stStreams.pstMedia[i],
                    sizeof(ST_SME_AUDIOINFO)))
                {
                    ICS_LOGE("VOS_Memcpy_S return failed !!!");
                    eSmeRet = E_SME_DEMUX_RET_NOMEM;
                    free(pstMediaInfoTmp);

                    break;
                }
            }
            else
            {
                ICS_LOGW("unknown media type, go next.");
                continue;
            }

            //拷贝config data
            if(u32CfgDataSizeTmp > 0)
            {
                ICS_LOGI("config data size:%u", u32CfgDataSizeTmp);

                pu8CfgDataNew = (V_UINT8 *)malloc(u32CfgDataSizeTmp);
                if(NULL == pu8CfgDataNew)
                {
                    ICS_LOGE("no memory.");
                    free(pstMediaInfoTmp);
                    continue;
                }

                if (E_SME_MEM_OK != VOS_Memcpy_S(
                    pu8CfgDataNew,
                    (V_SIZE)u32CfgDataSizeTmp,
                    pu8CfgDataTmp,
                    (V_SIZE)u32CfgDataSizeTmp))
                {
                    ICS_LOGE("VOS_Memcpy_S return failed !!!");
                    eSmeRet = E_SME_DEMUX_RET_NOMEM;
                    free(pstMediaInfoTmp);
                    free(pu8CfgDataNew);
                    break;
                }

                pstMediaInfoTmp->stCfgData.pu8CfgData = pu8CfgDataNew;
                pstMediaInfoTmp->stCfgData.u32CfgDataSize = u32CfgDataSizeTmp;
            }

            //保存此条媒体流
            pstStreamInfo->pstMedia[i] = pstMediaInfoTmp;
            u32ValidStreams++;
        }

        //如果没有获取到流，则接口返回失败
        UTILS_MLOGE_BRK_VAL_IF(0 == u32ValidStreams, eSmeRet, E_SME_DEMUX_RET_NO_DATA,
            ("no stream can retrive."));

    }while(ICS_FALSE);

    ICS_LOGI("SME_GetStreamInfoOfFFMpeg out");

    return eSmeRet;
}

static CSMEDEMUXFMT
SME_GetCurrentDemuxCtxOfFFMpeg(IN SMEDEMUXHDL pvDemuxHdl)
{
    SMEDEMUXFMT pvDemuxFmt = NULL;
    PST_SME_DEMUX_HDL pstThis = NULL;

    //ICS_LOGI("SME_GetCurrentDemuxCtxOfFFMpeg in");

    UTILS_MLOGE_RET_VAL_IF(!pvDemuxHdl, NULL, ("null handle."));

    pstThis = (PST_SME_DEMUX_HDL)pvDemuxHdl;

    pvDemuxFmt = pstThis->pvDemuxFmt;

    ICS_LOGD("SME_GetCurrentDemuxCtxOfFFMpeg ok");

    return pvDemuxFmt;
}

static V_INT32
SME_ProbeByDataOfFFMpeg(IN CSMEDEMUXFMT pvDemuxCtx,
    IN const V_UINT8* pu8Data, IN V_UINT32 u32Size)
{
    V_INT32 i32Ret = -1;
    AVProbeData stProbeData;
    AVInputFormat *pstAVInputFmt = NULL;

    //ICS_LOGI("SME_ProbeByDataOfFFMpeg in");

    UTILS_MLOGE_RET_VAL_IF(!pvDemuxCtx || !pu8Data || !u32Size, 0, ("invalid param."));

    pstAVInputFmt = (AVInputFormat *)(const_cast<SMEDEMUXFMT>(pvDemuxCtx));
    stProbeData.buf = const_cast<V_UINT8*>(pu8Data);
    stProbeData.buf_size = (V_INT32)u32Size;

    //make sure read_probe function is not null
    if (pstAVInputFmt->read_probe)
    {
       i32Ret = pstAVInputFmt->read_probe (&stProbeData);
    }

    UTILS_MLOGE_IF(i32Ret < 0, ("ffmpeg read probe fail."));

    ICS_LOGI("SME_ProbeByDataOfFFMpeg ok, probe_score=%d", i32Ret);

    return i32Ret;
}

static const V_CHAR*
SME_GetDemuxNameOfFFMpeg(IN CSMEDEMUXFMT pvDemuxFmt)
{
    AVInputFormat *pstAVInputFmt = NULL;

    //ICS_LOGI("SME_GetDemuxNameOfFFMpeg in");

    UTILS_MLOGE_RET_VAL_IF(!pvDemuxFmt, NULL, ("invalid param."));

    pstAVInputFmt = (AVInputFormat *)(const_cast<SMEDEMUXFMT>(pvDemuxFmt));

    ICS_LOGD("SME_GetDemuxNameOfFFMpeg ok(name:%s)", pstAVInputFmt->name);

    return pstAVInputFmt->name;
}

static const V_CHAR*
SME_GetDemuxDescriptionOfFFMpeg(IN CSMEDEMUXFMT pvDemuxFmt)
{
    AVInputFormat *pstAVInputFmt = NULL;

    //ICS_LOGI("SME_GetDemuxDescriptionOfFFMpeg in");

    UTILS_MLOGE_RET_VAL_IF(!pvDemuxFmt, NULL, ("invalid param."));

    pstAVInputFmt = (AVInputFormat *)(const_cast<SMEDEMUXFMT>(pvDemuxFmt));

    ICS_LOGD("SME_GetDemuxNameOfFFMpeg ok(name:%s)", pstAVInputFmt->long_name);

    return pstAVInputFmt->long_name;
}

static CSMEDEMUXFMT
SME_GetNextDemuxOfFFMpeg(IN CSMEDEMUXFMT pvDemuxFmt)
{
    AVInputFormat *pstAVInputFmt = NULL;
    SMEDEMUXFMT pvDemuxFmtTmp = NULL;

    //ICS_LOGD("SME_GetNextDemuxOfFFMpeg in");

    pstAVInputFmt = (AVInputFormat *)(const_cast<SMEDEMUXFMT>(pvDemuxFmt));

    pvDemuxFmtTmp = (SMEDEMUXFMT)av_iformat_next (pstAVInputFmt);

    ICS_LOGD("SME_GetNextDemuxOfFFMpeg ok");

    return (CSMEDEMUXFMT)pvDemuxFmtTmp;
}

static E_SME_DEMUX_RET
SME_InvokeDemuxOfFFMpeg(IN SMEDEMUXHDL pvDemuxHdl, IN E_SME_INVOKE_IDS eInvokeId,
    INOUT const V_CHAR* pszData)
{
    E_SME_DEMUX_RET eSmeRet = E_SME_DEMUX_RET_SUCCESS;
    PST_SME_DEMUX_HDL  pstThis = NULL;

    ICS_LOGI("FUNC:%s in", __LINE__, __FUNCTION__);
    if (!pvDemuxHdl)
    {
        ICS_LOGE("Demux hdl is invalid !!!", eInvokeId);
        return E_SME_DEMUX_RET_INVALID_ARG;
    }

    pstThis = (PST_SME_DEMUX_HDL)pvDemuxHdl;

    switch (eInvokeId)
    {
        case E_SME_INVOKE_SET_USERAGENT:
        {
            if (pszData)
            {
                pstThis->stUrlOptions.pszUserAgent = strdup(pszData);
            }
            else
            {
                eSmeRet = E_SME_DEMUX_RET_INVALID_ARG;
                ICS_LOGE("pu8Data is invalid !!!");
            }
            break;
        }

        case E_SME_INVOKE_SET_REFERER:
        {
            if (pszData)
            {
                pstThis->stUrlOptions.pszReferer = strdup(pszData);
            }
            else
            {
                eSmeRet = E_SME_DEMUX_RET_INVALID_ARG;
                ICS_LOGE("pu8Data is invalid !!!");
            }
            break;
        }

        case E_SME_INVOKE_SET_COOKIE:
        {
            if (pszData)
            {
                pstThis->stUrlOptions.pszCookies = strdup(pszData);
            }
            else
            {
                eSmeRet = E_SME_DEMUX_RET_INVALID_ARG;
                ICS_LOGE("pu8Data is invalid !!!");
            }
            break;
        }

        case E_SME_INVOKE_SET_HEADERS:
        {
            if (pszData)
            {
                pstThis->stUrlOptions.pszHeaders = strdup(pszData);
            }
            else
            {
                eSmeRet = E_SME_DEMUX_RET_INVALID_ARG;
                ICS_LOGE("pu8Data is invalid !!!");
            }
            break;
        }

        default:
        {
            ICS_LOGE("invoke id(%d) unsupport !!!", eInvokeId);
            eSmeRet = E_SME_DEMUX_RET_UNSUPPORT;
            break;
        }
    }

    ICS_LOGI("FUNC:%s out", __FUNCTION__);
    return eSmeRet;
}

static void
SME_StopReadFFMpeg(IN SMEDEMUXHDL pvDemuxHdl)
{
    PST_SME_DEMUX_HDL   pstThis = (PST_SME_DEMUX_HDL) pvDemuxHdl;

    ICS_LOGD("SME_StopReadFFMpeg in, pvDemuxHdl=%p", pvDemuxHdl);

    pstThis->bStopReadFf = ICS_TRUE;
    return;
}

static ST_SME_DEMUX_INF g_sstSmeDemuxInf = {
    /* SME_CreateDemux */           SME_CreateDemuxOfFFMpeg,
    /* SME_DestroyDemux */          SME_DestroyDemuxOfFFMpeg,
    /* SME_OpenDemux */             SME_OpenDemuxOfFFMpeg,
    /* SME_CloseDemux */            SME_CloseDemuxOfFFMpeg,
    /* SME_ReadFrame */             SME_ReadFrameOfFFMpeg,
    /* SME_SeekTo */                SME_SeekToOfFFMpeg,
    /* SME_Flush */                 SME_FlushFFMpeg,
    /* SME_GetStreamInfo */         SME_GetStreamInfoOfFFMpeg,
    /* SME_GetCurrentDemuxCtx */    SME_GetCurrentDemuxCtxOfFFMpeg,
    /* SME_ProbeByData */           SME_ProbeByDataOfFFMpeg,
    /* SME_GetDemuxName */          SME_GetDemuxNameOfFFMpeg,
    /* SME_GetDemuxDescription */   SME_GetDemuxDescriptionOfFFMpeg,
    /* SME_GetNextDemux */          SME_GetNextDemuxOfFFMpeg,
    /* SME_IsSeekable */            SME_IsSeekableOfFFMpeg,
    /* SME_InvokeDemux */           SME_InvokeDemuxOfFFMpeg,
    /* SME_StopRead */              SME_StopReadFFMpeg,
};

const ST_SME_DEMUX_INF* SME_GetDemuxInf()
{
    PST_SME_DEMUX_INF pstDemuxInf = NULL;

    VOS_ThdMutexLock(&g_stffmpegMutex);
    av_register_all ();
    av_log_set_callback (sme_ffmpeg_log_callback);
    VOS_ThdMutexUnLock(&g_stffmpegMutex);

    pstDemuxInf = &g_sstSmeDemuxInf;

    return pstDemuxInf;
}
