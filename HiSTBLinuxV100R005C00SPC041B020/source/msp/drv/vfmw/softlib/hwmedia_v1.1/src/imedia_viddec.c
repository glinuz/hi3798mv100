/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCO4ir2/uQ7BFDYEYESXdzFFpPTRP1CkEuTQUs7
G00Y4fnzdvrSRb0c1p3m7lnA5w5NwipD5z3ahoIiaTI2O9FFPKW0FS5yheX/ruh2DsUQhouE
p8/aZmlhIZIRQm4B1S9uBGlfpahesWX5aHB037OB9NvI+OxxEBYUGD39uOkUAg==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/************************************************************************
* Copyright(C), 2008-2013, Huawei Tech. Co., Ltd.
* All rights reserved.
*
* $Id: hwdec.c,v 1.138 2010/01/12 09:39:22 songxiaogang Exp $
*
* Description：
*
* Reference Doc：
*
* Modification：//见文件尾
*
*************************************************************************/

//#include <stdlib.h>
#include <stdint.h>

#include "iMedia_error.h"
#include "iMedia_viddec.h"
#include "iMedia_util.h"
#include "probe_header.h"

#include "avcodec.h"
#include "log.h"
#include "mem.h"

typedef struct STRU_DEC_CONTEXT
{
    /* preventing error code: will be set to 0x55AA55AA */
    INT32 iCrcMask;

    /* codec handle */
    AVCodecContext *pstAVCodecContext;

    /* storing the pointer of the memory that for YUV422toYUV420 convert */
    AVFrame stAVPicture;

    /* the flag of if print the decoder overhead infomation */
    INT32  iFlags;
    /* total time(unit:us) that consumed currently */
    UINT64 ullTotalSpendTime;
    /* previous total time(unit:us) that store the time before ullTotalSpendTime more than 1s */
    UINT64 ullPrevSpendTime;

    /* 用于统计实时性能 */
    UINT32 uiPrevOutputFrames;

    /* 用于统计输出YUV ID */
    UINT32 uiPrevDisplayID;
    /* 2010/05/25 16:00:00 liuxw+00139685 [AZ1D02103] */
    /* 将算法通道状态定义从avcodeccontext的结构体中移到本结构体中，在复位的时候并不对其进行清零 */
    /**< 算法通道状态 */
    ENUM_IVIDEO_CODEC_STATUS eCodecStatus;

} STRU_DEC_CONTEXT;

#define MAX_INPUT_BS_SIZE 0x4000000
#define MAX_REF_FRAME_NUM 16
#define MIN_REF_FRAME_MUM 0

#ifdef __KERNEL__
#define THREAD_SAFE 0
#else
#define THREAD_SAFE 1
#endif

#if THREAD_SAFE
#include "hw_mutex.h"

hw_mutex_t g_mutex_avcodec = {0};
INT32    g_iChanNum = 0;
#endif

extern AVCodec h264_decoder;
extern AVCodec mpeg2video_decoder;
extern AVCodec h263_decoder;
extern AVCodec mpeg4_decoder;
extern AVCodec vc1_decoder;
extern AVCodec wmv3_decoder;
/*x00141957 20100701*/
extern AVCodec vp6_decoder;
extern AVCodec vp6f_decoder;
/*x00141957 2011 0331增加*/
extern AVCodec vp6a_decoder;

/*guoshan+00101841 20100830*/
extern AVCodec flv_decoder;


#define DEBUG_CONTENT_PART_NUM 65
INT32 aiDebugCnt[DEBUG_CONTENT_PART_NUM] =
{
    DEBUG_NOTHING, FF_DEBUG_PICT_INFO, FF_DEBUG_MB_TYPE | FF_DEBUG_QP | FF_DEBUG_MV | FF_DEBUG_SKIP | FF_DEBUG_VIS_QP | FF_DEBUG_VIS_QP | FF_DEBUG_VIS_MB_TYPE,
    0, FF_DEBUG_DCT_COEFF, 0, 0, 0, FF_DEBUG_STARTCODE, 0, 0, 0, 0, 0, 0, 0, FF_DEBUG_ER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, FF_DEBUG_MMCO, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FF_DEBUG_BUFFERS
};

#define MAX_PIC_TYPE 8
INT32 aiPicType[MAX_PIC_TYPE] = {IVIDEO_FRAME_UNKNOWN, IVIDEO_FRAME_IDR, IVIDEO_FRAME_P, IVIDEO_FRAME_B, IVIDEO_FRAME_P, IVIDEO_FRAME_SI, IVIDEO_FRAME_SP, IVIDEO_FRAME_B};

/* change color format from codec to interface layer */
int aiColorFmt[] = {IVIDEO_CSP_MONOCHROME, IVIDEO_CSP_YUV420, IVIDEO_CSP_YUV422, IVIDEO_CSP_YUV444};

#if 0
/* convert YUV422 to YUV420 */
static INT32 YUV422toYUV420(AVPicture *pstSrcAVFrame, AVPicture *pstDstAVFrame, UINT16 usWidth, UINT16 usHeight);
#endif

/* get the status of the decoder */
static INT32 GetDecStatus(IMEDIA_CODEC_CTX ctx, void *wparam);

/* get the information of the bitstream decoded currently */
static INT32 GetStreamInfo(IMEDIA_CODEC_CTX ctx, void *wparam);

/* get the bitstream information */
static INT32 GetDecParams(IMEDIA_CODEC_CTX ctx, void *wparam);

/* flush the reference buffer and delay buffer */
static INT32 DecFlush(IMEDIA_CODEC_CTX ctx);

/* reset the xxx_decoder: include flush buffer and reset the status information */
static INT32 DecReset(IMEDIA_CODEC_CTX ctx);

/* set the debug content */
static INT32 SetDebugCnt(IMEDIA_CODEC_CTX ctx, void *piDebug);

/* release the yuv buf after display */
static INT32 ReleasePicture(IMEDIA_CODEC_CTX ctx, void *pstPicture);

static void *iMedia_GetAlignMem(void *p, unsigned int size);

static void iMedia_Release(void *pPtr);

/* System message print call back function */
#ifdef __GNUC__
void Decoder_msg_callback(const char *file, int line, const char *funcname, void *ptr, int level, const char *format, va_list ap)
#else
void Decoder_msg_callback(void *ptr, int level, const char *format, va_list ap)
#endif
{
    static char *pLastPtr = NULL;
    static int   print_prefix_flag = 1;
    char msg[512] = { 0 };
    //va_list ap;

    if (NULL != ptr && print_prefix_flag)
    {
        AVClass *avc = *((AVClass **)ptr);

        /* print the codec name and it's address */
        snprintf(msg, 512, "[%s @ %p]", avc->item_name(ptr), ptr);
    }

    //va_start(ap, format);
    vsnprintf(msg + strlen(msg), 512 - strlen(msg), format, ap);
    //va_end(ap);

    print_prefix_flag = (pLastPtr != ptr || msg[strlen(msg) - 1] == '\n');
    pLastPtr = ptr;

#if defined(__GNUC__)
    IMedia_msg_format(file, line, funcname, level, msg);
#else
    IMedia_msg_format(level, msg);
#endif
}


/*========================================================================================
 * 函数说明：从码流中解析元数据类型。
 * 输入参数：
         eCodecType 算法类型
         pstInArgs  输入参数信息
 * 输出参数：
         pstOutArgs 输出参数信息
 * 返 回 值：0-成功，其他值见返回值定义。
=========================================================================================*/
/*x00141957 20101230 内核态编译不需要此函数*/
#ifndef LINUX_KERNEL_BUILD
EXPORT_API INT32 IMedia_Viddec_ProbeHeader(ENUM_IVIDEO_CODEC_TYPE eCodecType, STRU_IMEDIA_SINGLE_BUFFER *pstInArgs, STRU_IVIDDEC_META_DATA_OUT_ARGS *pstOutArgs)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    /* check the validation of the in params */
    if (NULL == pstInArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input params is NULL!\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pstOutArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "output params is NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pstInArgs->pucBuf || 0 == pstInArgs->uiSize)
    {
        /*guoshan+00101841 20100715 16:05:00 问题单号[AZ1D02218]*/
        /*日志信息中NILL拼写错误，更正为NULL*/
        //       IMEDIA_LOG(IMEDIA_ERROR, "input bitsteam is NILL or len is zero!\n");
        IMEDIA_LOG(IMEDIA_ERROR, "NULL == pstInArgs->pucBuf || 0 == pstInArgs->uiSize\n");
        return IMEDIA_RET_PARAM_IN_VALUE_NULL;
    }

    /*lint -e685*/
    if (META_DATA_NOT_FOUND > pstOutArgs->eStatus || META_DATA_FOUND_ONLY_PPS < pstOutArgs->eStatus)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "probe status %d is invalid!\n", pstOutArgs->eStatus);
        return IMEDIA_RET_PARAM_OUT_VALUE_INVALID;
    }

    /*lint +e685*/

    /* 2010/05/11 10:25:00 songxg+00133955 */
    /* 接口调用前同样需要统一malloc、free、log等函数 */
    /* config the malloc function which will be used in application layer and interface layer (just config once) */
    AV_MALLOC_FUNCTION(iMedia_GetAlignMem);

    /* config the free function which will be used in application layer and interface layer (just config once) */
    AV_FREE_FUNCTION(iMedia_Release);

    /* config the log printf function which will be used in codec layer(just config once) */
    av_log_set_callback(Decoder_msg_callback);

    g_stGlobalInfo.bMallocFxnUsed = TRUE;

    /* 2010/07/14 14:30:00 liuxw+00139685 [AZ1D02219] */
    /* 将profile和level的无效值修改为-1 */
    pstOutArgs->stStreamInfo.eProfile        = IVIDEO_PROFILE_UNKNOWN;
    pstOutArgs->stStreamInfo.eLevel          = IVIDEO_LEVEL_UNKNOWN;
    /* 2010/08/2 17:00:00 liuxw+00139685 [AZ1D02237] */
    /* 给输出参数中的码流信息结构成员初始化为无效值 */
    pstOutArgs->stStreamInfo.eCodecType      = IVIDEO_CODEC_UNKNOWN;
    pstOutArgs->stStreamInfo.eContentType    = IVIDEO_CONTENT_UNKNOW;
    pstOutArgs->stStreamInfo.eColorSpaceType = IVIDEO_CSP_UNKNOWN;


    /* call the corresponding ProbeHeader(): find the header and parse it */
    switch (eCodecType)
    {
#if 0

        case IVIDEO_CODEC_H264:
        {
            iRet = H264_ProbeHeader(pstInArgs->pucBuf, pstInArgs->uiSize, &(pstOutArgs->uiBytesConsumed), (INT32 *)(&pstOutArgs->eStatus), &pstOutArgs->stMetaData, &pstOutArgs->stStreamInfo);
        }
        break;

        case IVIDEO_CODEC_MPEG2:
        {
            iRet = MPEG2_ProbeHeader(pstInArgs->pucBuf, pstInArgs->uiSize, &(pstOutArgs->uiBytesConsumed), (INT32 *)(&pstOutArgs->eStatus), &pstOutArgs->stMetaData, &pstOutArgs->stStreamInfo);
        }
        break;

        case IVIDEO_CODEC_MPEG4:
        {
            iRet = MPEG4_ProbeHeader(pstInArgs->pucBuf, pstInArgs->uiSize, &(pstOutArgs->uiBytesConsumed), (INT32 *)(&pstOutArgs->eStatus), &pstOutArgs->stMetaData, &pstOutArgs->stStreamInfo);
        }
        break;

        case IVIDEO_CODEC_WMV3:
        {
            iRet = WMV3_ProbeHeader(pstInArgs->pucBuf, pstInArgs->uiSize, &(pstOutArgs->uiBytesConsumed), (INT32 *)(&pstOutArgs->eStatus), &pstOutArgs->stMetaData, &pstOutArgs->stStreamInfo);
        }
        break;

        case IVIDEO_CODEC_VC1:
        {
            iRet = VC1_ProbeHeader(pstInArgs->pucBuf, pstInArgs->uiSize, &(pstOutArgs->uiBytesConsumed), (INT32 *)(&pstOutArgs->eStatus), &pstOutArgs->stMetaData, &pstOutArgs->stStreamInfo);
        }
        break;
#endif

        /*x00141957 20100721*/
        case IVIDEO_CODEC_VP6F:
        case IVIDEO_CODEC_VP6:
        {
            iRet = VP6_ProbeHeader(pstInArgs->pucBuf, pstInArgs->uiSize, &(pstOutArgs->uiBytesConsumed), (INT32 *)(&pstOutArgs->eStatus), &pstOutArgs->stMetaData, &pstOutArgs->stStreamInfo);
        }
        break;

        /*x00141957 2011 0331 增加vp6a*/
        case IVIDEO_CODEC_VP6A:
        {
            iRet = VP6A_ProbeHeader(pstInArgs->pucBuf, pstInArgs->uiSize, &(pstOutArgs->uiBytesConsumed), (INT32 *)(&pstOutArgs->eStatus), &pstOutArgs->stMetaData, &pstOutArgs->stStreamInfo);
        }
        break;

        case IVIDEO_CODEC_H263:
        {
            iRet = H263_ProbeHeader(pstInArgs->pucBuf, pstInArgs->uiSize, &(pstOutArgs->uiBytesConsumed), (INT32 *)(&pstOutArgs->eStatus), &pstOutArgs->stMetaData, &pstOutArgs->stStreamInfo);
        }
        break;

        /*guoshan+00101841 20100830 增加Sorenson Spark*/
        case IVIDEO_CODEC_SORENSON:
        {
            iRet = Sorenson_Spark_ProbeHeader(pstInArgs->pucBuf, pstInArgs->uiSize, &(pstOutArgs->uiBytesConsumed), (INT32 *)(&pstOutArgs->eStatus), &pstOutArgs->stMetaData, &pstOutArgs->stStreamInfo);
        }
        break;

        default:
        {
            IMEDIA_LOG(IMEDIA_ERROR, "codec type %d is invalid\n", eCodecType);
            return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
        }

        //break;
    }

    /* 2010/08/2 17:00:00 liuxw+00139685 [AZ1D02237] */
    /* 给输出参数中的码流信息结构成员赋值 */
    pstOutArgs->stStreamInfo.eCodecType = eCodecType;

    return iRet;
}
#endif

/*========================================================================================
* 函数说明：寻问解码算法所需的内存大小。
* 输入参数：
      eCodecType    算法类型，具体见ENUM_IVIDEO_CODEC_TYPE定义
      pstMemInfo    图像的最大宽度、 图像的最高高度、最大图像个数以及色度空间等信息
* 输出参数：
      piSize        解码器所需的总的内存大小
* 返 回 值：0-成功，其他值见返回值定义。
=========================================================================================*/
EXPORT_API INT32 IMedia_Viddec_QueryMemSize(ENUM_IVIDEO_CODEC_TYPE eCodecType, STRU_IVIDDEC_MEMORY_INFO *pstMemInfo , INT32 *piSize)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    /* check params validation */
    if (NULL == pstMemInfo)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "Input pstMemInfo pointer is NULL!\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == piSize)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "Output Params iSize pointer is NULL!\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (IVIDEO_MAX_WIDTH < pstMemInfo->usWidth || IVIDEO_MIN_WIDTH > pstMemInfo->usWidth)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "Width[%d] of pstMemInfo is out of range[%d,%d]!\n", pstMemInfo->usWidth, IVIDEO_MIN_WIDTH, IVIDEO_MAX_WIDTH);
        return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
    }

    if (IVIDEO_MAX_HEIGHT < pstMemInfo->usHeight || IVIDEO_MIN_HEIGHT > pstMemInfo->usHeight)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "Height[%d] of pstMemInfo is out of range[%d,%d]!\n", pstMemInfo->usHeight, IVIDEO_MIN_HEIGHT, IVIDEO_MAX_HEIGHT);
        return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
    }

    if (INTERNAL_BUFFER_SIZE < pstMemInfo->usBufNum || INTERNAL_BUFFER_MIN_SIZE > pstMemInfo->usBufNum)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "FrameBufNum[%d] of pstMemInfo is exceed MaxFrameBufNum[%d,%d]!\n", pstMemInfo->usBufNum, INTERNAL_BUFFER_MIN_SIZE, INTERNAL_BUFFER_SIZE);
        return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
    }

    if (IVIDEO_CSP_IYUV != pstMemInfo->eColorFormat && IVIDEO_CSP_YV12 != pstMemInfo->eColorFormat)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Not supported ColorFormat[%d] and modify it to IVIDEO_CSP_IYUV!\n", pstMemInfo->eColorFormat);
        pstMemInfo->eColorFormat = IVIDEO_CSP_IYUV;
        //      return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
    }

    *piSize = 0;

    switch (eCodecType)
    {
        case IVIDEO_CODEC_H263:
            *piSize	+= h263_decoder.query_memsize(pstMemInfo->usWidth, pstMemInfo->usHeight, pstMemInfo->usBufNum, (INT32)(pstMemInfo->eColorFormat));
            break;

        case IVIDEO_CODEC_SORENSON:
            *piSize	+= flv_decoder.query_memsize(pstMemInfo->usWidth, pstMemInfo->usHeight, pstMemInfo->usBufNum, (INT32)(pstMemInfo->eColorFormat));
            break;
#if 0

        case IVIDEO_CODEC_VP6:
            *piSize	+= vp6_decoder.query_memsize(pstMemInfo->usWidth, pstMemInfo->usHeight, pstMemInfo->usBufNum, (INT32)(pstMemInfo->eColorFormat));
            break;

        case IVIDEO_CODEC_VP6F:
            *piSize	+= vp6f_decoder.query_memsize(pstMemInfo->usWidth, pstMemInfo->usHeight, pstMemInfo->usBufNum, (INT32)(pstMemInfo->eColorFormat));
            break;

        case IVIDEO_CODEC_VP6A:
            *piSize	+= vp6f_decoder.query_memsize(pstMemInfo->usWidth, pstMemInfo->usHeight, pstMemInfo->usBufNum, (INT32)(pstMemInfo->eColorFormat));
            break;
#endif

        default:
            IMEDIA_LOG(IMEDIA_ERROR, "Not supported eCodecType[%d]\n", eCodecType);
            return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
    }

    if (0 >= *piSize)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "IMedia_Viddec_QueryMemSize failed! return code:%d!\n", *piSize);
        return IMEDIA_RET_PARAM_OTHER;
    }

    *piSize += sizeof(STRU_DEC_CONTEXT) + sizeof(AVCodecContext);  // add the size of dec handle

    return iRet;
}



/*========================================================================================
 * 函数说明：创建算法通道。
 * 输入参数：
         eCodecType 算法类型
         params     指向存储图像的一些基本信息的结构体（包括图像的宽高等）
 * 输出参数：
         pctx       算法通道句柄
 * 返 回 值：0-成功，其他值见返回值定义。
=========================================================================================*/
EXPORT_API INT32 IMedia_Viddec_Create(ENUM_IVIDEO_CODEC_TYPE eCodecType, STRU_IVIDDEC_PARAMS *pstParams, IMEDIA_CODEC_CTX *pctx)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT *pstDecContext     = NULL;
    AVCodecContext   *pstAVCodecContext = NULL;
    AVCodec          *pstAVCodec = NULL;
    UINT32  		 offset = 0;

    if (NULL == pstParams)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "Input static params pointer is NULL!\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "Decoder handle pointer is NULL!\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pstParams->pucBuf)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "pstParams->pucBuf is NULL!\n");
        return IMEDIA_RET_PARAM_IN_VALUE_NULL;
    }

    if (0 >= pstParams->iBufLength)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "pstParams->iBufLength[%d] is invalid!\n", pstParams->iBufLength);
        return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
    }

    *pctx = NULL;

    g_stGlobalInfo.bMallocFxnUsed = TRUE;

    /* Allocate memory for decoder handle */
    //  pstDecContext = IMedia_Malloc(sizeof(STRU_DEC_CONTEXT));
    //if(NULL == pstDecContext)
    //{
    //    IMEDIA_LOG(IMEDIA_ERROR,"Allocate memory for stDecContext failed!\n");
    //    return IMEDIA_RET_MEM_MALLOC_FAIL;
    //}
    if ((INT32)(sizeof(STRU_DEC_CONTEXT) + offset) <= pstParams->iBufLength)
    {
        pstDecContext = (STRU_DEC_CONTEXT *)((UINT8 *)pstParams->pucBuf + offset);
        offset       += sizeof(STRU_DEC_CONTEXT);
    }
    else
    {
        IMEDIA_LOG(IMEDIA_ERROR, "Mem left Bufsize[%d] is not enough for STRU_DEC_CONTEXT len[%d]!\n", pstParams->iBufLength - offset, sizeof(STRU_DEC_CONTEXT));
        return IMEDIA_RET_MEM_MALLOC_FAIL;
    }

    IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p] Dec handle create successful!\n", pstDecContext);

    /* config the malloc function which will be used in application layer and interface layer (just config once) */
    //  AV_MALLOC_FUNCTION(IMedia_AlignMalloc);
    AV_MALLOC_FUNCTION(iMedia_GetAlignMem);

    /* config the free function which will be used in application layer and interface layer (just config once) */
    //  AV_FREE_FUNCTION(IMedia_AlignFree);
    AV_FREE_FUNCTION(iMedia_Release);

    /* config the log printf function which will be used in codec layer(just config once) */
    av_log_set_callback(Decoder_msg_callback);

    /* init some tables */
    avcodec_init();

    /* allocate memory for codec handle and init it with default value */
    //    pstAVCodecContext = avcodec_alloc_context();
    //    if(NULL == pstAVCodecContext)
    //    {
    //        IMEDIA_LOG(IMEDIA_ERROR,"Allocate memory for stAVCodecContext failed!\n");
    ////      return IMEDIA_RET_MEM_MALLOC_FAIL;
    //        iRet = IMEDIA_RET_MEM_MALLOC_FAIL;
    //        goto CREAT_END;
    //    }
    if ((INT32)(sizeof(AVCodecContext) + offset) <= pstParams->iBufLength)
    {
        pstAVCodecContext = (AVCodecContext *)((UINT8 *)pstParams->pucBuf + offset);
        offset            += sizeof(AVCodecContext);
    }
    else
    {
        IMEDIA_LOG(IMEDIA_ERROR, "Mem left Bufsize[%d] is not enough for AVCodecContext len[%d]!\n", pstParams->iBufLength - offset, sizeof(AVCodecContext));
        return IMEDIA_RET_MEM_MALLOC_FAIL;
    }

    avcodec_get_context_defaults2(pstAVCodecContext, CODEC_TYPE_UNKNOWN);

    IMEDIA_LOG(IMEDIA_INFO, "codec_ctx[%p] AVCodec create successful!\n", pstAVCodecContext);

    memset(pstDecContext, 0, sizeof(STRU_DEC_CONTEXT));
    pstDecContext->pstAVCodecContext = pstAVCodecContext;
    pstDecContext->iFlags = pstParams->iFlags;

    /* init the decoder status */
    pstDecContext->eCodecStatus = IVIDEO_CODEC_INVALID;

    /* check params validation */
    if (IVIDEO_MAX_WIDTH < pstParams->usMaxWidth || IVIDEO_MIN_WIDTH > pstParams->usMaxWidth)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "Width[%d] of static params is out of range[%d,%d]!\n", pstParams->usMaxWidth, IVIDEO_MIN_WIDTH, IVIDEO_MAX_WIDTH);
        iRet = IMEDIA_RET_PARAM_IN_VALUE_INVALID;
        goto CREAT_END;
    }

    if (IVIDEO_MAX_HEIGHT < pstParams->usMaxHeight || IVIDEO_MIN_HEIGHT > pstParams->usMaxHeight)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "Height[%d] of static params is out of range[%d,%d]!\n", pstParams->usMaxHeight, IVIDEO_MIN_HEIGHT, IVIDEO_MAX_HEIGHT);
        iRet = IMEDIA_RET_PARAM_IN_VALUE_INVALID;
        goto CREAT_END;
    }

    //if(/*MIN_REF_FRAME_MUM > pstParams->usMaxRefFrame ||*/ MAX_REF_FRAME_NUM < pstParams->usMaxRefFrame)
    //{
    //    IMEDIA_LOG(IMEDIA_ERROR,"RefFrmNum[%d] of static params is out of range[%d,%d]!\n", pstParams->usMaxRefFrame,MIN_REF_FRAME_MUM,MAX_REF_FRAME_NUM);
    //    iRet = IMEDIA_RET_PARAM_IN_VALUE_INVALID;
    //    goto CREAT_END;
    //}

    if (INTERNAL_BUFFER_SIZE < pstParams->usMaxFrameBufNum || INTERNAL_BUFFER_MIN_SIZE > pstParams->usMaxFrameBufNum)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "FrameBufNum[%d] of static params is exceed MaxFrameBufNum[%d,%d]!\n", pstParams->usMaxFrameBufNum, INTERNAL_BUFFER_MIN_SIZE, INTERNAL_BUFFER_SIZE);
        iRet = IMEDIA_RET_PARAM_IN_VALUE_INVALID;
        goto CREAT_END;
    }

    /* init dec handle with params */
    pstAVCodecContext->uiProfileIdc    = pstParams->eProfile;
    pstAVCodecContext->uiLevelIdc      = pstParams->eLevel;
    pstAVCodecContext->usSourceWidth   = pstParams->usMaxWidth;
    pstAVCodecContext->usSourceHeight  = pstParams->usMaxHeight;
    pstAVCodecContext->uiRefFrame      = pstParams->usMaxRefFrame;
    pstAVCodecContext->eColorSpaceType = pstParams->eColorSpaceType;
    pstAVCodecContext->bForceOutYUV420 = pstParams->bForceOutYUV420Flag;
    pstAVCodecContext->uiBufNum        = pstParams->usMaxFrameBufNum;
    pstAVCodecContext->pucBase         = pstParams->pucBuf;
    pstAVCodecContext->iBufLength      = pstParams->iBufLength;
    pstAVCodecContext->uiCurPos        = offset;

    /* init dec handle with bitstream information */
    pstAVCodecContext->eCodecType = eCodecType;

    /* init dec handle with system features and function */
    /* 2010/04/23 9:30:00 liuxw+00139685 */
    /* 将cpu_mask的值进行一个取反操作：由于在后面具体使用时，会再进行一个取反操作，所以反反则为正 */
    //  pstAVCodecContext->dsp_mask = pstSysInfo->stCpuCaps.uiCpuFlag;
    //    pstAVCodecContext->dsp_mask = (pstSysInfo->stCpuCaps.uiCpuFlag & 0x80000000) ? 0x80000000 : ~(pstSysInfo->stCpuCaps.uiCpuFlag | 0xffff0000);

    /* 2010/04/29 18:05:00 songxiaogang+00133955 */
    /* 修改汇编优化传入接口 */
    pstAVCodecContext->dsp_mask =  (pstParams->iFlags & IMEDIA_FLAG_DISABLE_ASM) ? 0xffff : 0x80000000;

    pstAVCodecContext->arm_disable_flag = (pstParams->iFlags & IMEDIA_FLAG_DISABLE_ARM);

    /* set pstAVCodec with the corresponding xxx_decoder: include all the function such as init, decode, close... */
    switch (eCodecType)
    {
#if 0

        case IVIDEO_CODEC_H264:
        {
            pstAVCodec = &h264_decoder;

            if (IVIDEO_CSP_YUV420 != pstParams->eColorSpaceType)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "ColorSpaceType[%d] of static params is not supported for H264 currently!\n", pstParams->eColorSpaceType);
                iRet = IMEDIA_RET_PARAM_IN_VALUE_INVALID;
                goto CREAT_END;
            }
        }
        break;

        case IVIDEO_CODEC_MPEG2:
        {
            pstAVCodec = &mpeg2video_decoder;

            if (IVIDEO_CSP_YUV420 != pstParams->eColorSpaceType && IVIDEO_CSP_YUV422 != pstParams->eColorSpaceType)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "ColorSpaceType[%d] of static params is not supported for MPEG2 currently!\n", pstParams->eColorSpaceType);
                iRet = IMEDIA_RET_PARAM_IN_VALUE_INVALID;
                goto CREAT_END;
            }

            pstAVCodecContext->iSourceChromaFormat = (IVIDEO_CSP_YUV420 == pstParams->eColorSpaceType) ? 1 /* YUV420 */ : 2 /* YUV422 */;
        }
        break;
#endif

        case IVIDEO_CODEC_H263:
        {
            pstAVCodec = &h263_decoder;

            if (IVIDEO_CSP_YUV420 != pstParams->eColorSpaceType)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "ColorSpaceType[%d] of static params is not supported for H263 currently!\n", pstParams->eColorSpaceType);
                iRet = IMEDIA_RET_PARAM_IN_VALUE_INVALID;
                goto CREAT_END;
            }
        }
        break;
#if 0

        case IVIDEO_CODEC_MPEG4:
        {
            pstAVCodec = &mpeg4_decoder;

            if (IVIDEO_CSP_YUV420 != pstParams->eColorSpaceType)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "ColorSpaceType[%d] of static params is not supported for MPEG4 currently!\n", pstParams->eColorSpaceType);
                iRet = IMEDIA_RET_PARAM_IN_VALUE_INVALID;
                goto CREAT_END;
            }
        }
        break;

        case IVIDEO_CODEC_WMV3:
        {
            pstAVCodec = &wmv3_decoder;

            if (IVIDEO_CSP_YUV420 != pstParams->eColorSpaceType)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "ColorSpaceType[%d] of static params is not supported for WMV3 currently!\n", pstParams->eColorSpaceType);
                iRet = IMEDIA_RET_PARAM_IN_VALUE_INVALID;
                goto CREAT_END;
            }
        }
        break;

        case IVIDEO_CODEC_VC1:
        {
            pstAVCodec = &vc1_decoder;

            if (IVIDEO_CSP_YUV420 != pstParams->eColorSpaceType)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "ColorSpaceType[%d] of static params is not supported for VC1 currently!\n", pstParams->eColorSpaceType);
                iRet = IMEDIA_RET_PARAM_IN_VALUE_INVALID;
                goto CREAT_END;
            }
        }
        break;

        case IVIDEO_CODEC_VP6F:
        {
            pstAVCodec = &vp6f_decoder;
        }
        break;

        case IVIDEO_CODEC_VP6:
        {
            pstAVCodec = &vp6_decoder;
        }
        break;

        /*x00141957 2011 0331 增加vp6a codec*/
        case IVIDEO_CODEC_VP6A:
        {
            pstAVCodec = &vp6a_decoder;
        }
        break;
#endif

        case IVIDEO_CODEC_SORENSON:
        {
            pstAVCodec = &flv_decoder;
        }
        break;

        default:
        {
            IMEDIA_LOG(IMEDIA_ERROR, "codec type: %d is invalid!\n", eCodecType);
            iRet = IMEDIA_RET_PARAM_IN_VALUE_INVALID;
            goto CREAT_END;
        }

        //break;
    }

#if THREAD_SAFE

    if (0 == g_iChanNum)
    {
        hw_mutex_init(&g_mutex_avcodec);
    }

    hw_mutex_lock(&g_mutex_avcodec);
    g_iChanNum++;
#endif

    /* allocate memory for the xxx_decoder handle and init it */
    iRet = avcodec_open(pstAVCodecContext, pstAVCodec);

    /* 2010/04/24 17:25:00 songxiaogang+00133955 */
    /* 需保证在异常返回前解锁 */
#if THREAD_SAFE
    hw_mutex_unlock(&g_mutex_avcodec);
#endif

    if (iRet < 0)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "avcodec_open() failed!\n");
        iRet = IMEDIA_RET_OBJ_CREATE_FAIL;
        goto CREAT_END;
    }

    IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p] %s decoder handle[%p] create successful!\n", pstDecContext, pstAVCodecContext->codec->name , pstAVCodecContext->priv_data);

    /* if decoder handle creat successful , then set the preventing code */
    pstDecContext->iCrcMask = CRC_MASK;
    *pctx = pstDecContext;

    /* set the decoder status */
    pstDecContext->eCodecStatus = IVIDEO_CODEC_RUNNING;

#if 0

    /* if color type is YUV422, then allocate the memory for the converted YUV420 */
    if (IVIDEO_CSP_YUV420 != pstAVCodecContext->eColorSpaceType && 1 == pstParams->bForceOutYUV420Flag)
    {
        /* allocate memory for the Y */
        pstDecContext->stAVPicture.data[0]     = IMedia_AlignMalloc(IVIDEO_Y_SIZE(pstAVCodecContext->usSourceWidth, pstAVCodecContext->usSourceHeight));
        pstDecContext->stAVPicture.linesize[0] = IVIDEO_WIDTH_STRIDE(pstAVCodecContext->usSourceWidth);

        /* allocate memory for the U */
        pstDecContext->stAVPicture.data[1]     = IMedia_AlignMalloc(IVIDEO_UV_SIZE(pstAVCodecContext->usSourceWidth, pstAVCodecContext->usSourceHeight));
        pstDecContext->stAVPicture.linesize[1] = IVIDEO_WIDTH_STRIDE(pstAVCodecContext->usSourceWidth) >> 1;

        /* allocate memory for the V */
        pstDecContext->stAVPicture.data[2]     = IMedia_AlignMalloc(IVIDEO_UV_SIZE(pstAVCodecContext->usSourceWidth, pstAVCodecContext->usSourceHeight));
        pstDecContext->stAVPicture.linesize[2] = IVIDEO_WIDTH_STRIDE(pstAVCodecContext->usSourceWidth) >> 1;

        if (NULL == pstDecContext->stAVPicture.data[0] || NULL == pstDecContext->stAVPicture.data[1] || NULL == pstDecContext->stAVPicture.data[2])
        {
            IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Allocate memory for the converted YUV420 failed!\n", pstDecContext);
            iRet = IMEDIA_RET_MEM_MALLOC_FAIL;
            goto CREAT_END;
        }

        IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p] malloc for converted YUV420(Y:%p U:%p V:%p)!\n", pstDecContext,
                   pstDecContext->stAVPicture.data[0], pstDecContext->stAVPicture.data[1], pstDecContext->stAVPicture.data[2]);
    }

#endif

    return IMEDIA_RET_SUCCESS;

CREAT_END:
#if 0

    /* free the memory allocated for converted YUV420(Y) */
    if (NULL != pstDecContext->stAVPicture.data[0])
    {
        IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p] Free mem for converted YUV420(Y:%p)!\n", pstDecContext, pstDecContext->stAVPicture.data[0]);
        IMedia_AlignFree(pstDecContext->stAVPicture.data[0]);
        pstDecContext->stAVPicture.data[0] = NULL;
    }

    /* free the memory allocated for converted YUV420(U) */
    if (NULL != pstDecContext->stAVPicture.data[1])
    {
        IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p] Free mem for converted YUV420(U:%p)!\n", pstDecContext, pstDecContext->stAVPicture.data[1]);
        IMedia_AlignFree(pstDecContext->stAVPicture.data[1]);
        pstDecContext->stAVPicture.data[1] = NULL;
    }

    /* free the memory allocated for converted YUV420(V) */
    if (NULL != pstDecContext->stAVPicture.data[2])
    {
        IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p] Free mem for converted YUV420(V:%p)!\n", pstDecContext, pstDecContext->stAVPicture.data[2]);
        IMedia_AlignFree(pstDecContext->stAVPicture.data[2]);
        pstDecContext->stAVPicture.data[2] = NULL;
    }

#endif

    if (NULL != pstAVCodecContext)
    {
        /* free the xxx_decoder handle and buffer allocate for it */
        IMEDIA_LOG(IMEDIA_INFO, "codec_ctx[%p] Close the decoder!\n", pstAVCodecContext);
        avcodec_close(pstAVCodecContext);

        /* free codec Handle */
        IMEDIA_LOG(IMEDIA_INFO, "codec_ctx[%p] Free the decoder handle!\n", pstAVCodecContext);
        avcodec_free_context(pstAVCodecContext);
        pstAVCodecContext = NULL;
    }

    /* free decoder handle */
    if (NULL != pstDecContext)
    {
        IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p] Free the decoder handle!\n", pstDecContext);
        pstDecContext->eCodecStatus = IVIDEO_CODEC_INVALID;
        pstDecContext->iCrcMask = 0;
        iMedia_Release(pstDecContext);
        pstDecContext = NULL;
    }

    return iRet;
}
/*========================================================================================
* 函数说明：算法通道控制函数。
* 输入参数：
ctx       算法通道句柄
cmd:      控制参数的命令码
IMEDIA_GET_VERSION    返回版本信息                   wparam(STRU_IMEDIA_VERSION*)         lparam(NULL)
IMEDIA_GET_STATUS    返回算法通道状态信息           wparam(STRU_IVIDDEC_STATUS*)            lparam(NULL)
IMEDIA_GET_STREAM_INFO 返回算法通道当前码流信息    wparam(STRU_IVIDDEC_STREAM_INFO*)    lparam(NULL)
IMEDIA_GET_PARAMS    设置算法通道的参数信息         wparam(STRU_IVIDDEC_PARAMS*)         lparam(NULL)
IMEDIA_RESET        复位算法通道                   wparam(NULL)                         lparam(NULL)
wparam    每种命令对应各自类型，可作为输入或输出
lparam    每种命令对应各自类型，可作为输入或输出
* 输出参数：
wparam    每种命令对应各自类型，可作为输入或输出
lparam    每种命令对应各自类型，可作为输入或输出
* 返 回 值：0-成功，其他值见返回值定义。
=========================================================================================*/
EXPORT_API INT32 IMedia_Viddec_Control(IMEDIA_CODEC_CTX ctx, ENUM_IMEDIA_CMD cmd, void *wparam, void *lparam)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    switch (cmd)
    {
            /* get the decoder version */
        case IMEDIA_GET_VERSION:
        {
            STRU_IMEDIA_VERSION *pstVersion = (STRU_IMEDIA_VERSION *)wparam;

            if (NULL == pstVersion)
            {
                return IMEDIA_RET_PARAM_NULL;
            }

            /* 2010/05/11 14:00:00 liuxw+00139685 [AZ1D02063] */
            /* 将iMedia MS-ENC V100R001P301修改为iMedia MS-DEC V100R001P301 */
            strncpy(pstVersion->cVersionChar, "iMedia MS-DEC V100R001P301", IMEDIA_VERSION_LENGTH);
            strncpy(pstVersion->cReleaseTime, __TIME__ " " __DATE__, IMEDIA_TIME_LENGTH);
#if defined(_MSC_VER)
            pstVersion->uiCompileVersion = IMEDIA_FOURCC('V', 'S', '9', '0');
#else
            pstVersion->uiCompileVersion = IMEDIA_FOURCC('G', 'C', 'C', '4');
#endif
        }
        break;

        /* get the status of the decoder */
        case IMEDIA_GET_STATUS:
        {
            return GetDecStatus(ctx, wparam);
        }

        //break;

        /* get the bitstream infomation */
        case IMEDIA_GET_STREAM_INFO:
        {
            return GetStreamInfo(ctx, wparam);
        }

        //break;

        /* get the static params */
        case IMEDIA_GET_PARAMS:
        {
            return GetDecParams(ctx, wparam);
        }

        //break;

        /* flush the reference buffer and delay buffer */
        case IMEDIA_FLUSH:
        {
            return DecFlush(ctx);
        }

        //break;

        /* reset the whole decoder: include flush all the buffer and status information */
        case IMEDIA_RESET:
        {
            return DecReset(ctx);
        }

        //break;

        /* set the debug flag for decode modules */
        case IMEDIA_SET_DEBUG_FLAG:
        {
            return SetDebugCnt(ctx, wparam);
        }

        //break;
        /* set the debug flag for decode modules */
        case IMEDIA_PICTURE_RELEASE:
        {
            return ReleasePicture(ctx, wparam);
        }

        default:
        {
            IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CMD[%d]  is invalid!\n", ctx, cmd);
            return IMEDIA_RET_CMD_INVALID;
        }

        //break;
    }

    return iRet;
}
/*========================================================================================
 * 函数说明：处理函数。
 * 输入参数：ctx     解码器句柄
          inArgs  输入参数 - 输入码流的地址以及长度等
 * 输出参数：outArgs 输出参数 - 解码输出图像的相关信息
 * 返 回 值：0-成功，其他值见返回值定义。
=========================================================================================*/
#if 0
EXPORT_API INT32 IMedia_Viddec_Process(IMEDIA_CODEC_CTX ctx, STRU_IVIDDEC_IN_ARGS *pstInArgs, STRU_IVIDDEC_OUT_ARGS *pstOutArgs)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT *pstDecContext = NULL;
    AVCodecContext *pstAVCodecContext = NULL;
    AVFrame stAVFrame;
    UINT32 uiOutputSize = 0;
    UINT32 uiOutputBufSize = 0 , uiConsumedBytes = 0;
    const UINT8 *pucOutputBuf = NULL;
    INT32 i = 0, iLength = 0 , iReturn = 0;
    UINT8 *pucBuf = NULL;
    UINT32 uiNumBytes = 0;
    UINT64 ullCurrentSysTime = 0;
    AVFrame *pstTmpPic = NULL;

    /* check the validation of the params */
    if (NULL == ctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    pstDecContext = (STRU_DEC_CONTEXT *)ctx;

    if (CRC_MASK != pstDecContext->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CRC mask[0x%x != 0x%x]\n", pstDecContext, pstDecContext->iCrcMask, CRC_MASK);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    pstAVCodecContext = pstDecContext->pstAVCodecContext;

    if (NULL == pstInArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Input args pointer is NULL!\n", pstDecContext);
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pstOutArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Output args pointer is NULL!\n", pstDecContext);
        return IMEDIA_RET_PARAM_NULL;
    }

    /* 2010/04/24 17:05:00 songxiaogang+00133955 */
    /* 保证输出参数的值始终有效 */
    memset(pstOutArgs, 0, sizeof(STRU_IVIDDEC_OUT_ARGS));
    pstOutArgs->eFrameType = IVIDEO_FRAME_UNKNOWN;
    pstOutArgs->stPicture.eContentType = IVIDEO_CONTENT_UNKNOW;

    /* 2010/07/26 11:50:00 guoshan+00101841 */
    /* 消除pc-lint中的warning: Warning 685 Relational operator '<' always evaluates to 'false' */
    /*lint -e685*/
    if (BITSTREAM_SEGMENT_BEGIN > pstInArgs->eBitstreamStatus || BITSTREAM_OTHER < pstInArgs->eBitstreamStatus)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Bitstream status is invalid!\n", pstDecContext);
        return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
    }

    /*lint +e685*/

    /* if this is the first time come in , then set the bitstream of header to Decoder Handle: when decode frame , it first decode the header */
    if (BITSTREAM_SEGMENT_BEGIN == pstInArgs->eBitstreamStatus || !pstAVCodecContext->extradata_num)
    {
        if (pstInArgs->pstMetaData)
        {
            if (MAX_META_DATA_COUNT < pstInArgs->pstMetaData->iNumMetaData)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Input args Meta number[%d] is out of range[0,%d]!\n", pstDecContext, pstInArgs->pstMetaData->iNumMetaData, MAX_META_DATA_COUNT);
                return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
            }

            for (i = 0; i < pstInArgs->pstMetaData->iNumMetaData; i++)
            {
                pstAVCodecContext->extradata[i]      = pstInArgs->pstMetaData->apucMetaData[i];
                pstAVCodecContext->extradata_size[i] = pstInArgs->pstMetaData->ausMetaDataLen[i];
            }

            pstAVCodecContext->extradata_num  = pstInArgs->pstMetaData->iNumMetaData;
        }
    }

    pucBuf      = pstInArgs->pucBuf;
    uiNumBytes  = pstInArgs->uiNumBytes;
    //  pstAVCodecContext->iErrorCode = 0;    /* init it every time */

    if (pstDecContext->iFlags & IMEDIA_FLAG_CALC_FPS)
    {
        ullCurrentSysTime = IMedia_OS_Milliseconds();
    }

    /* check the input bitstream size */
    if (MAX_INPUT_BS_SIZE < pstInArgs->uiNumBytes)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "dec_ctx[%p] Input bitstream size[%u] is exceed the Max[%u]!\n", pstDecContext, pstInArgs->uiNumBytes, MAX_INPUT_BS_SIZE);
        pstOutArgs->uiBytesConsumed = pstInArgs->uiNumBytes;
        return iRet;
    }

    /* if find a whole frame , then decode it and go out of the loop; if reach the buffer end ,also go out of the loop */
    while (uiConsumedBytes < pstInArgs->uiNumBytes  || (BITSTREAM_SEGMENT_END == pstInArgs->eBitstreamStatus && uiConsumedBytes == pstInArgs->uiNumBytes))
    {

        pstAVCodecContext->iErrorCode = 0;    /* init it every time */

        /*  parse one whole picture(frame of field) */
        pucOutputBuf = NULL;
        uiOutputBufSize = 0;
        iLength = 0;
        iRet = pstAVCodecContext->codec->Frame_Parse(pstAVCodecContext, &pucOutputBuf, &uiOutputBufSize, pucBuf, uiNumBytes, &iLength);

        /* update the size and pointer */
        uiConsumedBytes += iLength;
        pucBuf          += iLength;
        uiNumBytes      -= iLength;

        /* 2010/04/24 17:25:00 songxiaogang+00133955 */
        /* 及时更新已分析完的字节数，便于应用层往后进行处理 */
        pstOutArgs->uiBytesConsumed = uiConsumedBytes;

        if (IMEDIA_RET_SUCCESS != iRet)
        {
            IMEDIA_LOG(IMEDIA_ERROR, "codec_ctx[%p] Frame_Parse failed! Return Code: %d\n", pstAVCodecContext, iRet);
            return iRet;
        }

        /* if reach the end of bitstream and not find one whole picture , then go out of the loop */
        if (NULL == pucOutputBuf || 0 == uiOutputBufSize)
        {
            /* if reach the end of bitstream file, then not go out of the loop and keep on outputing the YUV stored in delay buffer */
            /* 2010/06/29 19:30:00 liuxw+00139685 */
            /* 增加一个判断条件 */
            if (BITSTREAM_SEGMENT_END != pstInArgs->eBitstreamStatus && iLength)
            {
                break;
            }
            else if (0 == uiNumBytes && iLength)
            {
                continue;
            }
        }

        /* decode one picture(frame or field) */
        if (IVIDEO_CODEC_RUNNING == pstDecContext->eCodecStatus)
        {
            iReturn = pstAVCodecContext->codec->decode(pstAVCodecContext, &stAVFrame, (int *)&uiOutputSize, pucOutputBuf, (int)uiOutputBufSize);

            /* if there are some problem that lead to return , then return errors code */
            if (0 > iReturn)
            {
                pstOutArgs->iErrorCode = pstAVCodecContext->iErrorCode;
                pstDecContext->eCodecStatus = IVIDEO_CODEC_STOPPED;
                return IMEDIA_RET_DECODE_INTERRUPT;
            }

            if (pstAVCodecContext->iMallocFailed)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "Malloc memory for codec[%s] failed!\n", pstAVCodecContext->codec->name);
                pstDecContext->eCodecStatus = IVIDEO_CODEC_STOPPED;
                return IMEDIA_RET_DECODE_INTERRUPT;
            }

            /*guoshan+00101841 20100730 问题单：AZ1D02234 */
            /*日志打印信息多余0x,去掉*/
            //             IMEDIA_LOG(IMEDIA_DEBUG,"codec_ctx[0x%p] buf[0x%p] input size[%d]  consumed bytes[%d] left[%d] pic[0x%p] decode_return[%d]\n",
            //                 pstAVCodecContext,pucBuf-iLength, uiNumBytes+iLength, iLength,uiNumBytes, pucOutputBuf, iReturn);
            IMEDIA_LOG(IMEDIA_DEBUG, "codec_ctx[%p] buf[%p] input size[%d]  consumed bytes[%d] left[%d] pic[%p] decode_return[%d]\n",
                       pstAVCodecContext, pucBuf - iLength, uiNumBytes + iLength, iLength, uiNumBytes, pucOutputBuf, iReturn);

            /* if there is not YUV left in delay buf , then set the last frame flag and go out of the loop */
            if (0 == iReturn)
            {
                pstOutArgs->bLastFrameFlag = 1;
                break;
            }

            /* if decoded a whole frame , then go out of the loop */
            if (uiOutputSize > 0)
            {
                break;
            }
        }
        else
        {
            pstOutArgs->bLastFrameFlag = 1;
            break;
        }
    }

    /* update color format */
    pstAVCodecContext->eColorFormatType = aiColorFmt[pstAVCodecContext->iChromaFormat];

    /* if decoded whole frame , then set the OutArgs with the information decoded */
    if (uiOutputSize > 0)
    {
        /* if the color space is YUV422 , then we must turn YUV422 to YUV420 */
        switch (pstAVCodecContext->eColorFormatType)
        {
            case IVIDEO_CSP_YUV420:
            {
                pstTmpPic = &stAVFrame;
            }
            break;

            case IVIDEO_CSP_UYVY:
            case IVIDEO_CSP_YUY2:
            {
                /* convert YUV422 to YUV420 */
                if (1 == pstAVCodecContext->bForceOutYUV420)
                {
                    iRet = YUV422toYUV420((AVPicture *)(&stAVFrame), (AVPicture *)(&(pstDecContext->stAVPicture)),
                                          IVIDEO_WIDTH_STRIDE(pstAVCodecContext->usActualWidth), IVIDEO_HEIGHT_STRIDE(pstAVCodecContext->usActualHeight));

                    if (IMEDIA_RET_SUCCESS != iRet)
                    {
                        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] YUV422 converts to YUV420 failed! Return COde: %d\n", pstDecContext, iRet);
                        return IMEDIA_RET_OTHER;
                    }

                    pstTmpPic = &(pstDecContext->stAVPicture);
                }
                else
                {
                    pstTmpPic = &stAVFrame;
                }

            }
            break;

            default:
            {
                IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Not supported color format type[%d]!\n", pstDecContext, pstAVCodecContext->eColorFormatType);
                return IMEDIA_RET_OTHER;
            }
        }

        /* check the picture type is valid */
        if (0 > stAVFrame.pict_type || MAX_PIC_TYPE < stAVFrame.pict_type)
        {
            IMEDIA_LOG(IMEDIA_WARNING, "dec_ctx[%p] Not supported frame type[%d]!\n", pstAVCodecContext, stAVFrame.pict_type);
            stAVFrame.pict_type = IVIDEO_FRAME_UNKNOWN;
        }

        /* set the picture type */
        pstOutArgs->eFrameType = (ENUM_IVIDEO_FRAME_TYPE)aiPicType[stAVFrame.pict_type];

        if (IVIDEO_FRAME_B == pstOutArgs->eFrameType && 0 != stAVFrame.reference)
        {
            pstOutArgs->eFrameType = IVIDEO_FRAME_BREF;
        }

        pstAVCodecContext->uiDiscardFrames += (pstAVCodecContext->uiFldCnt + 1) / 2;
        pstDecContext->uiPrevDisplayID += pstAVCodecContext->uiDiscardFrames;
        pstAVCodecContext->uiDiscardFrames = 0;
        pstAVCodecContext->uiFldCnt = 0;

        /* fill the OutArgs with the infomation that xxx_decoder output */
        pstOutArgs->bLastFrameFlag          = stAVFrame.ucLastFrame;
        pstOutArgs->uiDisplayID             = pstDecContext->uiPrevDisplayID++;
        pstOutArgs->stPicture.eContentType  = (ENUM_IVIDEO_CONTENT_TYPE)stAVFrame.interlaced_frame;
        pstOutArgs->stPicture.usWidth       = pstAVCodecContext->usActualWidth;
        pstOutArgs->stPicture.usHeight      = pstAVCodecContext->usActualHeight;
        pstOutArgs->stPicture.usWidthPitch  = (UINT16)pstTmpPic->linesize[0];
        pstOutArgs->stPicture.apucBuf[0]    = pstTmpPic->data[0];;
        pstOutArgs->stPicture.apucBuf[1]    = pstTmpPic->data[1];
        pstOutArgs->stPicture.apucBuf[2]    = pstTmpPic->data[2];

        /* 2010/07/23 15:30:00 liuxw+00139685 */
        /* 对aspect ratio进行赋值 */
        pstOutArgs->stPicture.stAspectRatio.usSarWidth  = pstAVCodecContext->sample_aspect_ratio.num;
        pstOutArgs->stPicture.stAspectRatio.usSarHeight = pstAVCodecContext->sample_aspect_ratio.den;

        pstAVCodecContext->uiDisplayedFrames++;

        /* Update the bitstream infomation dynamicly */
        pstAVCodecContext->eContentType = pstOutArgs->stPicture.eContentType;

        pstOutArgs->iErrorCode = pstTmpPic->iErrorCode;
        pstTmpPic->iErrorCode = 0;
    }

    if (pstDecContext->iFlags & IMEDIA_FLAG_CALC_FPS)
    {
        UINT32 uiSpendMS = 0;
        UINT32 uiDecodedFrames = pstAVCodecContext->uiDecIFrames + pstAVCodecContext->uiDecPFrames + pstAVCodecContext->uiDecBFrames;

        ullCurrentSysTime = IMedia_OS_Milliseconds() - ullCurrentSysTime;
        pstDecContext->ullTotalSpendTime += ullCurrentSysTime;

        /* print the decoder overhead every 1 second */
        uiSpendMS = (UINT32)(pstDecContext->ullTotalSpendTime - pstDecContext->ullPrevSpendTime);

        if (1000 < uiSpendMS)
        {
            IMEDIA_LOG(IMEDIA_DEBUG, "dec_ctx[%p] fps: %.2f [process frames %u -> %u spend %u ms]\n",
                       pstDecContext, (FLOAT32)((uiDecodedFrames - pstDecContext->uiPrevOutputFrames) * 1000) / uiSpendMS,
                       pstDecContext->uiPrevOutputFrames, uiDecodedFrames, uiSpendMS);
            pstDecContext->ullPrevSpendTime = pstDecContext->ullTotalSpendTime;
            pstDecContext->uiPrevOutputFrames = uiDecodedFrames;
        }
    }

    return iRet;
}
#endif
/*========================================================================================
* 函数说明：删除算法通道。
* 输入参数：
ctx       算法通道句柄
* 输出参数：无
* 返 回 值：0-成功，其他值见返回值定义。
=========================================================================================*/
EXPORT_API INT32 IMedia_Viddec_Delete(IMEDIA_CODEC_CTX ctx)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT *pstDecContext = NULL;
    AVCodecContext *pstAVCodecContext = NULL;

    /* check the validation of the params */
    if (NULL == ctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    pstDecContext = (STRU_DEC_CONTEXT *)ctx;

    if (CRC_MASK != pstDecContext->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CRC mask[0x%x != 0x%x]\n", pstDecContext, pstDecContext->iCrcMask, CRC_MASK);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    pstAVCodecContext = pstDecContext->pstAVCodecContext;
#if 0

    /* free the memory allocated for converted YUV420(Y) */
    if (NULL != pstDecContext->stAVPicture.data[0])
    {
        IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p] Free mem for converted YUV420(Y:%p)!\n", pstDecContext, pstDecContext->stAVPicture.data[0]);
        IMedia_AlignFree(pstDecContext->stAVPicture.data[0]);
        pstDecContext->stAVPicture.data[0] = NULL;
    }

    /* free the memory allocated for converted YUV420(U) */
    if (NULL != pstDecContext->stAVPicture.data[1])
    {
        IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p] Free mem for converted YUV420(U:%p)!\n", pstDecContext, pstDecContext->stAVPicture.data[1]);
        IMedia_AlignFree(pstDecContext->stAVPicture.data[1]);
        pstDecContext->stAVPicture.data[1] = NULL;
    }

    /* free the memory allocated for converted YUV420(V) */
    if (NULL != pstDecContext->stAVPicture.data[2])
    {
        IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p] Free mem for converted YUV420(V:%p)!\n", pstDecContext, pstDecContext->stAVPicture.data[2]);
        IMedia_AlignFree(pstDecContext->stAVPicture.data[2]);
        pstDecContext->stAVPicture.data[2] = NULL;
    }

#endif

#if THREAD_SAFE
    hw_mutex_lock(&g_mutex_avcodec);
#endif
    /* close the xxx_decoder and free the memory has allocated for it */
    iRet = avcodec_close(pstAVCodecContext);

#if THREAD_SAFE
    /*guoshan+0010841 20100730 问题单:[AZ1D02235]*/
    /*消除频繁创建、删除通道过程中线程锁出现临界资源冲突的问题*/
    //    g_iChanNum--;
    hw_mutex_unlock(&g_mutex_avcodec);

    if (0 == g_iChanNum)
    {
        hw_mutex_destroy(&g_mutex_avcodec);
    }

#endif

    if (iRet < 0)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p]: AVCodec[%p] close failed\n", pstDecContext, pstAVCodecContext);
        iRet =  IMEDIA_RET_OTHER;
    }
    else
    {
        IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p]: AVCodec[%p] close successful\n", pstDecContext, pstAVCodecContext);
    }

    /* free the codec handle */
    IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p]: free AVCodec[%p]\n", pstDecContext, pstAVCodecContext);
    avcodec_free_context(pstAVCodecContext);
    pstAVCodecContext = NULL;

    /* free the decoder handle */
    IMEDIA_LOG(IMEDIA_INFO, "dec_ctx[%p] free handle!\n", pstDecContext);
    pstDecContext->iCrcMask = 0;
    pstDecContext->eCodecStatus = IVIDEO_CODEC_INVALID;
    //  IMedia_Free(pstDecContext);
    iMedia_Release(pstDecContext);
    pstDecContext = NULL;

    return iRet;
}

/*****************************************************************************************
* 函数说明：码流帧解析接口，从输入码流中解析出一帧数据，注意输出的帧缓存有可能是输入码流地址，也有可能是内部缓存地址（当输入码流不足一帧时内部会拼接）。
*           该接口在海思ARM软解码项目中仅提供给应用层做调试
* 输入参数：
*     ctx        算法通道句柄
*     pstInArgs  输入参数信息（输入码流的地址、长度及时间戳等）
* 输出参数：
*     pstOutArgs 输出参数信息（帧缓存的地址和长度）
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Viddec_FrameParse(IMEDIA_CODEC_CTX ctx, STRU_IVIDDEC_STREAM *pstInArgs, STRU_IVIDDEC_FRAME_PARSE_OUT_ARGS *pstOutArgs)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT *pstDecContext = NULL;
    AVCodecContext *pstAVCodecContext = NULL;

    /* check the validation of the params */
    if (NULL == ctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    pstDecContext = (STRU_DEC_CONTEXT *)ctx;

    if (CRC_MASK != pstDecContext->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CRC mask[0x%x != 0x%x]\n", pstDecContext, pstDecContext->iCrcMask, CRC_MASK);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    pstAVCodecContext = pstDecContext->pstAVCodecContext;

    if (NULL == pstInArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Input args pointer is NULL!\n", pstDecContext);
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pstOutArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Output args pointer is NULL!\n", pstDecContext);
        return IMEDIA_RET_PARAM_NULL;
    }

    /* 保证输出参数的值始终有效 */
    memset(pstOutArgs, 0, sizeof(STRU_IVIDDEC_FRAME_PARSE_OUT_ARGS));

    /* check the input bitstream size */
    if (MAX_INPUT_BS_SIZE < pstInArgs->uiNumBytes)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "dec_ctx[%p] Input bitstream size[%u] is exceed the Max[%u]!\n", pstDecContext, pstInArgs->uiNumBytes, MAX_INPUT_BS_SIZE);
        pstOutArgs->uiBytesConsumed = pstInArgs->uiNumBytes;
        return iRet;
    }

    iRet = pstAVCodecContext->codec->Frame_Parse(pstAVCodecContext, (const uint8_t **) & (pstOutArgs->stFrame.pucBuf), &(pstOutArgs->stFrame.uiNumBytes), (const uint8_t *)pstInArgs->pucBuf, pstInArgs->uiNumBytes, (int *)&pstOutArgs->uiBytesConsumed);

    if (IMEDIA_RET_SUCCESS != iRet)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "codec_ctx[%p] Frame_Parse failed! Return Code: %d\n", pstAVCodecContext, iRet);
        return iRet;
    }

    pstOutArgs->stFrame.iPTS = pstInArgs->iPTS;

    return iRet;
}


/*****************************************************************************************
* 函数说明：码流帧解码接口，每次只解码一帧，延迟输出的帧需重复调用该接口（输入码流地址赋值为NULL），直到输出参数中指示为最后一帧时。
* 输入参数：
*     ctx        算法通道句柄
*     pstInArgs  输入参数信息（输入码流帧的地址、长度及时间戳等）
* 输出参数：
*     pstOutArgs 输出参数信息（解码输出图像的相关信息）
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Viddec_FrameDecode(IMEDIA_CODEC_CTX ctx, STRU_IVIDDEC_STREAM *pstInArgs, STRU_IVIDDEC_OUT_ARGS *pstOutArgs)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT *pstDecContext = NULL;
    AVCodecContext *pstAVCodecContext = NULL;
    AVFrame stAVFrame;
    AVFrame *pstTmpPic = NULL;
    UINT32 uiOutputSize = 0;
    INT32 iReturn = 0;
    UINT64 ullCurrentSysTime = 0;

    /* check the validation of the params */
    if (NULL == ctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    pstDecContext = (STRU_DEC_CONTEXT *)ctx;

    if (CRC_MASK != pstDecContext->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CRC mask[0x%x != 0x%x]\n", pstDecContext, pstDecContext->iCrcMask, CRC_MASK);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    pstAVCodecContext = pstDecContext->pstAVCodecContext;

    if (NULL == pstInArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Input args pointer is NULL!\n", pstDecContext);
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pstOutArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Output args pointer is NULL!\n", pstDecContext);
        return IMEDIA_RET_PARAM_NULL;
    }

    /* 2010/04/24 17:05:00 songxiaogang+00133955 */
    /* 保证输出参数的值始终有效 */
    memset(pstOutArgs, 0, sizeof(STRU_IVIDDEC_OUT_ARGS));
    pstOutArgs->eFrameType = IVIDEO_FRAME_UNKNOWN;
    pstOutArgs->stPicture.eContentType = IVIDEO_CONTENT_UNKNOW;

    pstAVCodecContext->iPts = pstInArgs->iPTS;

    if (pstDecContext->iFlags & IMEDIA_FLAG_CALC_FPS)
    {
        ullCurrentSysTime = IMedia_OS_Milliseconds();
    }

    /* decode one picture(frame or field) */
    if (IVIDEO_CODEC_RUNNING == pstDecContext->eCodecStatus)
    {
        iReturn = pstAVCodecContext->codec->decode(pstAVCodecContext, &stAVFrame, (int *)(&uiOutputSize), pstInArgs->pucBuf, (int)pstInArgs->uiNumBytes);

        switch (iReturn)
        {
            case AVCODEC_RET_DECODE_LAST_FRAME:
                pstOutArgs->bLastFrameFlag = 1;
                break;

            case AVCODEC_RET_DECODE_ABORT:
                pstOutArgs->iErrorCode = pstAVCodecContext->iErrorCode;
                pstDecContext->eCodecStatus = IVIDEO_CODEC_STOPPED;
                return IMEDIA_RET_DECODE_INTERRUPT;

            case AVCODEC_RET_DECODE_BLOCK:
                pstOutArgs->uiBytesConsumed = 0;
                return IMEDIA_RET_SUCCESS;

            case AVCODEC_RET_DECODE_FAILURE:
            default:
                pstOutArgs->uiBytesConsumed = (UINT32)iReturn;
                break;
        }
    }
    else
    {
        pstOutArgs->bLastFrameFlag = 1;
    }

    /* update color format */
    pstAVCodecContext->eColorFormatType = aiColorFmt[pstAVCodecContext->iChromaFormat];


    /* if decoded whole frame , then set the OutArgs with the information decoded */
    if (0 < uiOutputSize)
    {
#if 0

        /* if the color space is YUV422 , then we must turn YUV422 to YUV420 */
        switch (pstAVCodecContext->eColorFormatType)
        {
            case IVIDEO_CSP_YUV420:
            {
                pstTmpPic = &stAVFrame;
            }
            break;

            case IVIDEO_CSP_UYVY:
            case IVIDEO_CSP_YUY2:
            {
                /* convert YUV422 to YUV420 */
                if (1 == pstAVCodecContext->bForceOutYUV420)
                {
                    iRet = YUV422toYUV420((AVPicture *)(&stAVFrame), (AVPicture *)(&(pstDecContext->stAVPicture)),
                                          IVIDEO_WIDTH_STRIDE(pstAVCodecContext->usActualWidth), IVIDEO_HEIGHT_STRIDE(pstAVCodecContext->usActualHeight));

                    if (IMEDIA_RET_SUCCESS != iRet)
                    {
                        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] YUV422 converts to YUV420 failed! Return COde: %d\n", pstDecContext, iRet);
                        return IMEDIA_RET_OTHER;
                    }

                    pstTmpPic = &(pstDecContext->stAVPicture);
                }
                else
                {
                    pstTmpPic = &stAVFrame;
                }
            }
            break;

            default:
            {
                IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Not supported color format type[%d]!\n", pstDecContext, pstAVCodecContext->eColorFormatType);
                return IMEDIA_RET_OTHER;
            }
        }

#endif
        pstTmpPic = &stAVFrame;

        /* check the picture type is valid */
        if (0 > stAVFrame.pict_type || MAX_PIC_TYPE < stAVFrame.pict_type)
        {
            IMEDIA_LOG(IMEDIA_WARNING, "dec_ctx[%p] Not supported frame type[%d]!\n", pstAVCodecContext, stAVFrame.pict_type);
            stAVFrame.pict_type = IVIDEO_FRAME_UNKNOWN;
        }

        /* set the picture type */
        pstOutArgs->eFrameType = (ENUM_IVIDEO_FRAME_TYPE)aiPicType[stAVFrame.pict_type];

        if (IVIDEO_FRAME_B == pstOutArgs->eFrameType && 0 != stAVFrame.reference)
        {
            pstOutArgs->eFrameType = IVIDEO_FRAME_BREF;
        }

        pstAVCodecContext->uiDiscardFrames += (pstAVCodecContext->uiFldCnt + 1) / 2;
        pstDecContext->uiPrevDisplayID += pstAVCodecContext->uiDiscardFrames;
        pstAVCodecContext->uiDiscardFrames = 0;
        pstAVCodecContext->uiFldCnt = 0;

        /* fill the OutArgs with the infomation that xxx_decoder output */
        pstOutArgs->bLastFrameFlag          = stAVFrame.ucLastFrame;
        pstOutArgs->uiDisplayID             = pstDecContext->uiPrevDisplayID++;
        pstOutArgs->stPicture.eContentType  = (ENUM_IVIDEO_CONTENT_TYPE)stAVFrame.interlaced_frame;
        pstOutArgs->stPicture.usWidth       = pstAVCodecContext->usActualWidth;
        pstOutArgs->stPicture.usHeight      = pstAVCodecContext->usActualHeight;
        pstOutArgs->stPicture.usWidthPitch  = (UINT16)pstTmpPic->linesize[0];
        pstOutArgs->stPicture.apucBuf[0]    = pstTmpPic->data[0];
        pstOutArgs->stPicture.apucBuf[1]    = pstTmpPic->data[1];
        pstOutArgs->stPicture.apucBuf[2]    = pstTmpPic->data[2];
        pstOutArgs->stPicture.apucBuf[3]    = pstTmpPic->data[3];

        /* 2010/07/23 15:30:00 liuxw+00139685 */
        /* 对aspect ratio进行赋值 */
        pstOutArgs->stPicture.stAspectRatio.usSarWidth  = pstAVCodecContext->sample_aspect_ratio.num;
        pstOutArgs->stPicture.stAspectRatio.usSarHeight = pstAVCodecContext->sample_aspect_ratio.den;

        pstAVCodecContext->uiDisplayedFrames++;

        /* Update the bitstream infomation dynamicly */
        pstAVCodecContext->eContentType = pstOutArgs->stPicture.eContentType;

        pstOutArgs->iErrorCode = pstTmpPic->iErrorCode;
        pstTmpPic->iErrorCode = 0;

        pstOutArgs->iPTS = pstTmpPic->iPts;
    }

    pstOutArgs->uiBytesConsumed += pstInArgs->uiNumBytes;

    if (pstDecContext->iFlags & IMEDIA_FLAG_CALC_FPS)
    {
        UINT32 uiSpendMS = 0;
        UINT32 uiDecodedFrames = pstAVCodecContext->uiDecIFrames + pstAVCodecContext->uiDecPFrames + pstAVCodecContext->uiDecBFrames;

        ullCurrentSysTime = IMedia_OS_Milliseconds() - ullCurrentSysTime;
        pstDecContext->ullTotalSpendTime += ullCurrentSysTime;

        /* print the decoder overhead every 1 second */
        uiSpendMS = (UINT32)(pstDecContext->ullTotalSpendTime - pstDecContext->ullPrevSpendTime);

        if (1000 < uiSpendMS)
        {
            /*      IMEDIA_LOG(IMEDIA_DEBUG, "dec_ctx[%p] fps: %u [process frames %u -> %u spend %u ms]\n",
                      pstDecContext, ((uiDecodedFrames - pstDecContext->uiPrevOutputFrames) * 1000) / uiSpendMS,
                      pstDecContext->uiPrevOutputFrames, uiDecodedFrames, uiSpendMS); */
            pstDecContext->ullPrevSpendTime = pstDecContext->ullTotalSpendTime;
            pstDecContext->uiPrevOutputFrames = uiDecodedFrames;
        }

    }

    return iRet;
}

/*========================================================================================
 * 函数说明：处理函数。
 * 输入参数：ctx     解码器句柄
          inArgs  输入参数 - 输入码流的地址以及长度等
 * 输出参数：outArgs 输出参数 - 解码输出图像的相关信息
 * 返 回 值：0-成功，其他值见返回值定义。
=========================================================================================*/
EXPORT_API INT32 IMedia_Viddec_Process(IMEDIA_CODEC_CTX ctx, STRU_IVIDDEC_IN_ARGS *pstInArgs, STRU_IVIDDEC_OUT_ARGS *pstOutArgs)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT *pstDecContext = NULL;
    AVCodecContext *pstAVCodecContext = NULL;
    UINT32 uiConsumedBytes = 0;

    STRU_IVIDDEC_STREAM stInArgs;
    STRU_IVIDDEC_FRAME_PARSE_OUT_ARGS stOutArgs;

    /* check the validation of the params */
    if (NULL == ctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    pstDecContext = (STRU_DEC_CONTEXT *)ctx;

    if (CRC_MASK != pstDecContext->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CRC mask[0x%x != 0x%x]\n", pstDecContext, pstDecContext->iCrcMask, CRC_MASK);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    pstAVCodecContext = pstDecContext->pstAVCodecContext;

    if (NULL == pstInArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Input args pointer is NULL!\n", pstDecContext);
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pstOutArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Output args pointer is NULL!\n", pstDecContext);
        return IMEDIA_RET_PARAM_NULL;
    }

    if (BITSTREAM_SEGMENT_BEGIN > pstInArgs->eBitstreamStatus || BITSTREAM_OTHER < pstInArgs->eBitstreamStatus)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Bitstream status is invalid!\n", pstDecContext);
        return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
    }

    /* if this is the first time come in , then set the bitstream of header to Decoder Handle: when decode frame , it first decode the header */
    if (BITSTREAM_SEGMENT_BEGIN == pstInArgs->eBitstreamStatus || !pstAVCodecContext->extradata_num)
    {
        if (pstInArgs->pstMetaData)
        {
            int i;

            if (MAX_META_DATA_COUNT < pstInArgs->pstMetaData->iNumMetaData)
            {
                IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] Input args Meta number[%d] is out of range[0,%d]!\n", pstDecContext, pstInArgs->pstMetaData->iNumMetaData, MAX_META_DATA_COUNT);
                return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
            }

            for (i = 0; i < pstInArgs->pstMetaData->iNumMetaData; i++)
            {
                pstAVCodecContext->extradata[i]      = pstInArgs->pstMetaData->apucMetaData[i];
                pstAVCodecContext->extradata_size[i] = pstInArgs->pstMetaData->ausMetaDataLen[i];
            }

            pstAVCodecContext->extradata_num  = pstInArgs->pstMetaData->iNumMetaData;
        }
    }

    /* 将输入参数赋给临时结构体变量，用于帧边界函数的输入参数 */
    stInArgs.iPTS = pstInArgs->iPTS;
    stInArgs.pucBuf = pstInArgs->pucBuf;
    stInArgs.uiNumBytes = pstInArgs->uiNumBytes;

    /* check the input bitstream size */
    if (MAX_INPUT_BS_SIZE < pstInArgs->uiNumBytes)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "dec_ctx[%p] Input bitstream size[%u] is exceed the Max[%u]!\n", pstDecContext, pstInArgs->uiNumBytes, MAX_INPUT_BS_SIZE);
        pstOutArgs->uiBytesConsumed = pstInArgs->uiNumBytes;
        return iRet;
    }

    /* if find a whole frame , then decode it and go out of the loop; if reach the buffer end ,also go out of the loop */
    while (uiConsumedBytes < pstInArgs->uiNumBytes  || (BITSTREAM_SEGMENT_END == pstInArgs->eBitstreamStatus && uiConsumedBytes == pstInArgs->uiNumBytes))
    {
        pstAVCodecContext->iErrorCode = 0;    /* init it every time */

        /*  parse one whole picture(frame of field) */
        memset(&stOutArgs, 0, sizeof(stOutArgs));
        //      iRet = pstAVCodecContext->codec->Frame_Parse(pstAVCodecContext, &pucOutputBuf, &uiOutputBufSize, pucBuf, uiNumBytes, &iLength);
        iRet = IMedia_Viddec_FrameParse(ctx, &stInArgs, &stOutArgs);

        /* update the size and pointer */
        stInArgs.iPTS       = -1;
        stInArgs.pucBuf     += stOutArgs.uiBytesConsumed;
        stInArgs.uiNumBytes -= stOutArgs.uiBytesConsumed;
        uiConsumedBytes     += stOutArgs.uiBytesConsumed;

        if (IMEDIA_RET_SUCCESS != iRet)
        {
            /* 若解析错误，也需更新用掉的字节数 */
            pstOutArgs->uiBytesConsumed = uiConsumedBytes;
            IMEDIA_LOG(IMEDIA_ERROR, "codec_ctx[%p] Frame_Parse failed! Return Code: %d\n", pstAVCodecContext, iRet);
            return iRet;
        }

        /* if reach the end of bitstream and not find one whole picture , then go out of the loop */
        if (NULL == stOutArgs.stFrame.pucBuf || 0 == stOutArgs.stFrame.uiNumBytes)
        {
            /* if reach the end of bitstream file, then not go out of the loop and keep on outputing the YUV stored in delay buffer */
            if (BITSTREAM_SEGMENT_END != pstInArgs->eBitstreamStatus && stOutArgs.uiBytesConsumed)
            {
                break;
            }
            else if (0 == stOutArgs.stFrame.uiNumBytes && stOutArgs.uiBytesConsumed)
            {
                continue;
            }
        }

        //      iReturn = pstAVCodecContext->codec->decode(pstAVCodecContext, &stAVFrame, (int *)&uiOutputSize, pucOutputBuf, (int)uiOutputBufSize);
        iRet = IMedia_Viddec_FrameDecode(ctx, &stOutArgs.stFrame, pstOutArgs);

        /* 更新消耗的字节数 */
        pstOutArgs->uiBytesConsumed = uiConsumedBytes;

        /* if there are some problem that lead to return , then return errors code */
        if (IMEDIA_RET_SUCCESS != iRet)
        {
            IMEDIA_LOG(IMEDIA_ERROR, "codec_ctx[%p] occurs seriously error：%d\n", pstAVCodecContext, iRet);
            return iRet;
        }

        /* if decoded a whole frame , then go out of the loop */
        if (NULL != pstOutArgs->stPicture.apucBuf[0] || pstOutArgs->bLastFrameFlag)
        {
            break;
        }
    }

    return iRet;
}

#if 0
/* convert YUV422 to YUV420
    usWidth:  aligned 16
    usHeight: aligned 2 */
static INT32 YUV422toYUV420(AVPicture *pstSrcAVFrame, AVPicture *pstDstAVFrame, UINT16 usWidth, UINT16 usHeight)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    INT32 i;
    UINT8 *pucSrcYPtr;
    UINT8 *pucSrcUPtr;
    UINT8 *pucSrcVPtr;
    UINT8 *pucDstYPtr;
    UINT8 *pucDstUPtr;
    UINT8 *pucDstVPtr;

    if (NULL == pstSrcAVFrame || NULL == pstDstAVFrame)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "The param(SRC:%p DST:%p) is NULL!\n", pstSrcAVFrame, pstDstAVFrame);
        return IMEDIA_RET_PARAM_NULL;
    }

    pucSrcYPtr = pstSrcAVFrame->data[0];
    pucSrcUPtr = pstSrcAVFrame->data[1];
    pucSrcVPtr = pstSrcAVFrame->data[2];
    pucDstYPtr = pstDstAVFrame->data[0];
    pucDstUPtr = pstDstAVFrame->data[1];
    pucDstVPtr = pstDstAVFrame->data[2];

    if (NULL == pucSrcYPtr || NULL == pucSrcUPtr || NULL == pucSrcVPtr)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "The pointer(Y:%p U:%p V:%p) of input param(%p) is NULL!\n", pucSrcYPtr, pucSrcUPtr, pucSrcVPtr, pstSrcAVFrame);
        return IMEDIA_RET_PARAM_IN_VALUE_NULL;
    }

    if (NULL == pucDstYPtr || NULL == pucDstUPtr || NULL == pucDstVPtr)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "The pointer(Y:%p U:%p V:%p) of out param(%p) is NULL!\n", pucDstYPtr, pucDstUPtr, pucDstVPtr, pstDstAVFrame);
        return IMEDIA_RET_PARAM_OUT_VALUE_NULL;
    }

    /* copy Y from src to dst directly */
    for (i = 0; i < usHeight; i++)
    {
        memcpy(pucDstYPtr, pucSrcYPtr, usWidth);
        pucSrcYPtr += pstSrcAVFrame->linesize[0];
        pucDstYPtr += pstDstAVFrame->linesize[0];
    }

    /* just copy U/V of the even line from src to dst */
    for (i = 0; i < (usHeight >> 1); i++)
    {
        memcpy(pucDstUPtr, pucSrcUPtr, (usWidth) >> 1);
        memcpy(pucDstVPtr, pucSrcVPtr, (usWidth) >> 1);
        pucSrcUPtr += pstSrcAVFrame->linesize[1] * 2;
        pucSrcVPtr += pstSrcAVFrame->linesize[2] * 2;
        pucDstUPtr += pstDstAVFrame->linesize[1];
        pucDstVPtr += pstDstAVFrame->linesize[2];
    }

    return iRet;
}
#endif

/* get the status of the decoder */
INT32 GetDecStatus(IMEDIA_CODEC_CTX ctx, void *wparam)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT    *pstDecContext = (STRU_DEC_CONTEXT *)ctx;
    AVCodecContext      *pstAVCodecContext = NULL;
    STRU_IVIDDEC_STATUS *pstVidDecStatus = (STRU_IVIDDEC_STATUS *)wparam;

    /* check the validation of the params */
    if (NULL == pstDecContext)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    /* check the preventing code */
    if (CRC_MASK != pstDecContext->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CRC mask[0x%x != 0x%x]\n", pstDecContext, pstDecContext->iCrcMask, CRC_MASK);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    pstAVCodecContext = pstDecContext->pstAVCodecContext;

    if (NULL == pstVidDecStatus)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] The pointer of storing decoder status is NULL!\n", pstDecContext);
        return IMEDIA_RET_PARAM_NULL;
    }

    memset(pstVidDecStatus, 0, sizeof(STRU_IVIDDEC_STATUS));

    pstVidDecStatus->eCodecStatus         = (ENUM_IVIDEO_CODEC_STATUS)pstDecContext->eCodecStatus;
    pstVidDecStatus->iErrorCode           = pstAVCodecContext->iErrorCode;
    pstVidDecStatus->uiTotalErrors        = pstAVCodecContext->iTotalError;
    pstVidDecStatus->uiDecIFrames         = pstAVCodecContext->uiDecIFrames;
    pstVidDecStatus->uiDecPFrames         = pstAVCodecContext->uiDecPFrames;
    pstVidDecStatus->uiDecBFrames         = pstAVCodecContext->uiDecBFrames;
    pstVidDecStatus->uiDisplayedFrames    = pstAVCodecContext->uiDisplayedFrames;
    pstVidDecStatus->uiFreeBufNum         = pstAVCodecContext->uiFreeBufNum;
    pstVidDecStatus->uiInUsedBufNum       = pstAVCodecContext->uiInUsedBufNum;
#if 0

    if (pstDecContext->ullTotalSpendTime > 0)
    {
        UINT32 uiDecodedFrames = pstAVCodecContext->uiDecIFrames + pstAVCodecContext->uiDecPFrames + pstAVCodecContext->uiDecBFrames;
        // 2010/07/21 songxg 由于VP6在开启汇编后浮点转换存在问题
        pstVidDecStatus->uiAvgFps = (UINT32)(((UINT64)uiDecodedFrames * 1000 * 1000) / (pstDecContext->ullTotalSpendTime));
        //		pstVidDecStatus->uiH263LFFps = (UINT32)(((UINT64)uiDecodedFrames * 1000 * 1000) / (pstAVCodecContext->loop_filter_time));
    }

#endif
    return iRet;
}

/* get the infomation of the bitstream decoded currently */
INT32 GetStreamInfo(IMEDIA_CODEC_CTX ctx, void *wparam)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT *pstDecContext = NULL;
    AVCodecContext *pstAVCodecContext = NULL;
    STRU_IVIDEO_STREAM_INFO *pstVideoStreamInfo = NULL;

    /* check the validation of the params */
    if (NULL == ctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    pstDecContext = (STRU_DEC_CONTEXT *)ctx;

    /* check the preventing code */
    if (CRC_MASK != pstDecContext->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CRC mask[0x%x != 0x%x]\n", pstDecContext, pstDecContext->iCrcMask, CRC_MASK);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    pstAVCodecContext = pstDecContext->pstAVCodecContext;

    if (NULL == wparam)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] The pointer of storing stream infomation is NULL!\n", pstDecContext);
        return IMEDIA_RET_PARAM_NULL;
    }

    pstVideoStreamInfo = (STRU_IVIDEO_STREAM_INFO *)wparam;
    pstVideoStreamInfo->eCodecType        = (ENUM_IVIDEO_CODEC_TYPE)pstAVCodecContext->eCodecType;
    pstVideoStreamInfo->eContentType      = (ENUM_IVIDEO_CONTENT_TYPE)pstAVCodecContext->eContentType;
    pstVideoStreamInfo->uiBitrate         = 0; /* pstAVCodecContext->uiBitrate; */
    pstVideoStreamInfo->uiFrameRate       = 0; /* pstAVCodecContext->uiFrameRate; */
    pstVideoStreamInfo->uiVbvBuf          = 0; /* pstAVCodecContext->uiVbvBufSize; */
    pstVideoStreamInfo->eColorSpaceType   = (ENUM_IVIDEO_COLOR_SPACE_TYPE)pstAVCodecContext->eColorFormatType;
    pstVideoStreamInfo->eProfile          = (ENUM_IVIDEO_CODEC_PROFILE)pstAVCodecContext->iActualProfile;
    pstVideoStreamInfo->eLevel            = (ENUM_IVIDEO_CODEC_LEVEL)pstAVCodecContext->iActualLevel;
    pstVideoStreamInfo->uiRefFrameNum     = pstAVCodecContext->iActualRefNum;
    pstVideoStreamInfo->usWidth           = pstAVCodecContext->usActualWidth;
    pstVideoStreamInfo->usHeight          = pstAVCodecContext->usActualHeight;
    pstVideoStreamInfo->stAspectRatio.usSarWidth  =  pstAVCodecContext->sample_aspect_ratio.num;
    pstVideoStreamInfo->stAspectRatio.usSarHeight =  pstAVCodecContext->sample_aspect_ratio.den;

    return iRet;
}

/* get the bitstream infomation */
INT32 GetDecParams(IMEDIA_CODEC_CTX ctx, void *wparam)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT *pstDecContext = NULL;
    AVCodecContext *pstAVCodecContext = NULL;
    STRU_IVIDDEC_PARAMS *pstVidDecParams = NULL;

    /* check the validation of the params */
    if (NULL == ctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    pstDecContext = (STRU_DEC_CONTEXT *)ctx;

    /* check the preventing code */
    if (CRC_MASK != pstDecContext->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CRC mask[0x%x != 0x%x]\n", pstDecContext, pstDecContext->iCrcMask, CRC_MASK);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    pstAVCodecContext = pstDecContext->pstAVCodecContext;

    if (NULL == wparam)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] The pointer of storing static params is NULL!\n", pstDecContext);
        return IMEDIA_RET_PARAM_NULL;
    }

    pstVidDecParams = (STRU_IVIDDEC_PARAMS *)wparam;
    pstVidDecParams->usMaxWidth             = pstAVCodecContext->usSourceWidth;
    pstVidDecParams->usMaxHeight            = pstAVCodecContext->usSourceHeight;
    pstVidDecParams->usMaxRefFrame          = pstAVCodecContext->uiRefFrame;
    pstVidDecParams->eProfile            = (ENUM_IVIDEO_CODEC_PROFILE)pstAVCodecContext->uiProfileIdc;
    pstVidDecParams->eLevel              = (ENUM_IVIDEO_CODEC_LEVEL)pstAVCodecContext->uiLevelIdc;
    pstVidDecParams->eColorSpaceType     = (ENUM_IVIDEO_COLOR_SPACE_TYPE)pstAVCodecContext->eColorSpaceType;
    pstVidDecParams->bForceOutYUV420Flag = pstAVCodecContext->bForceOutYUV420;
    pstVidDecParams->iFlags              = pstDecContext->iFlags;
    pstVidDecParams->pucBuf              = pstAVCodecContext->pucBase;
    pstVidDecParams->iBufLength          = pstAVCodecContext->iBufLength;

    //pstVidDecParams->eFrameSkipMode      = (ENUM_IVIDDEC_FRAME_SKIP_MODE)pstAVCodecContext->eFrameSkipMode;

    return iRet;
}

/* flush the reference buffer and delay buffer */
INT32 DecFlush(IMEDIA_CODEC_CTX ctx)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT *pstDecContext = NULL;
    AVCodecContext *pstAVCodecContext = NULL;

    /* check the validation of the params */
    if (NULL == ctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    pstDecContext = (STRU_DEC_CONTEXT *)ctx;

    /* check the preventing code */
    if (CRC_MASK != pstDecContext->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CRC mask[0x%x != 0x%x]\n", pstDecContext, pstDecContext->iCrcMask, CRC_MASK);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    pstAVCodecContext = pstDecContext->pstAVCodecContext;

    pstAVCodecContext->codec->flush(pstAVCodecContext);

    return iRet;
}

/* reset the xxx_decoder: include flush buffer and reset the status infomation */
INT32 DecReset(IMEDIA_CODEC_CTX ctx)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT *pstDecContext = NULL;
    AVCodecContext *pstAVCodecContext = NULL;

    /* check the validation of the params */
    if (NULL == ctx)
    {
        //IMEDIA_LOG(IMEDIA_WARNING,"dec_ctx is NULL!\n");  //y00226912
        return IMEDIA_RET_PARAM_INVALID;
    }

    pstDecContext = (STRU_DEC_CONTEXT *)ctx;

    /* check the preventing code */
    if (CRC_MASK != pstDecContext->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CRC mask[0x%x != 0x%x]\n", pstDecContext, pstDecContext->iCrcMask, CRC_MASK);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    pstAVCodecContext = pstDecContext->pstAVCodecContext;

    iRet = pstAVCodecContext->codec->reset(pstAVCodecContext);

    if (iRet != IMEDIA_RET_SUCCESS)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] %s Decoder reset Failed!\n", pstDecContext, pstAVCodecContext->codec->name);
        return iRet;
    }

    pstDecContext->ullTotalSpendTime = 0;
    pstDecContext->ullPrevSpendTime = 0;
    pstDecContext->uiPrevOutputFrames = 0;

    pstDecContext->uiPrevDisplayID = 0;

    return iRet;
}

/* set the debug content */
INT32 SetDebugCnt(IMEDIA_CODEC_CTX ctx, void *piDebug)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT *pstDecContext = NULL;
    AVCodecContext *pstAVCodecContext = NULL;
    INT32 iDebug;

    /* check the validation of the params */
    if (NULL == ctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    pstDecContext = (STRU_DEC_CONTEXT *)ctx;

    /* check the preventing code */
    if (CRC_MASK != pstDecContext->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CRC mask[0x%x != 0x%x]\n", pstDecContext, pstDecContext->iCrcMask, CRC_MASK);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    /* check the validation of the params */
    if (NULL == piDebug)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "Input debug flag pointer is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    iDebug = *((INT32 *)piDebug);

    pstAVCodecContext = pstDecContext->pstAVCodecContext;

    pstAVCodecContext->debug |= aiDebugCnt[DEBUG_PIC_INFO & iDebug];
    pstAVCodecContext->debug |= aiDebugCnt[DEBUG_MB_INFO & iDebug];
    pstAVCodecContext->debug |= aiDebugCnt[DEBUG_DCT_COF & iDebug];
    pstAVCodecContext->debug |= aiDebugCnt[DEBUG_START_CODE & iDebug];
    pstAVCodecContext->debug |= aiDebugCnt[DEBUG_ERROR & iDebug];
    pstAVCodecContext->debug |= aiDebugCnt[DEBUG_REF_MANAGE & iDebug];
    pstAVCodecContext->debug |= aiDebugCnt[DEBUG_BUF_MANAGE & iDebug];

    pstAVCodecContext->debug_mv = (DEBUG_MB_INFO & iDebug) ? (FF_DEBUG_VIS_MV_P_FOR | FF_DEBUG_VIS_MV_B_FOR | FF_DEBUG_VIS_MV_B_BACK) : 0;

    return iRet;
}


INT32 ReleasePicture(IMEDIA_CODEC_CTX ctx, void *pstPicture)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;

    STRU_DEC_CONTEXT *pstDecContext = NULL;
    //AVCodecContext *pstAVCodecContext = NULL;
    AVFrame *pstFrm = NULL;

    /* check the validation of the params */
    if (NULL == ctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    pstDecContext = (STRU_DEC_CONTEXT *)ctx;

    /* check the preventing code */
    if (CRC_MASK != pstDecContext->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "dec_ctx[%p] CRC mask[0x%x != 0x%x]\n", pstDecContext, pstDecContext->iCrcMask, CRC_MASK);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    /* check the validation of the params */
    if (NULL == pstPicture)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "Input pic pointer is NULL!\n");
        return IMEDIA_RET_PARAM_INVALID;
    }

    pstFrm = (AVFrame *)(((STRU_IVIDEO_PICTURE *)pstPicture)->apucBuf[3]);
    pstFrm->isbusy = 0;

    return iRet;
}

static void *iMedia_GetAlignMem(void *p , unsigned int size)
{
    void *ptr = NULL;
    UINT32 diff = 0;
    AVCodecContext *pstAVCtx = (AVCodecContext *)p;

    diff = ((-(long)(pstAVCtx->pucBase + pstAVCtx->uiCurPos) - 1) & 15) + 1;

    if (diff != 16)
    {
        pstAVCtx->uiCurPos += diff;
        //		ptr = pstAVCtx->pucBase + pstAVCtx->uiCurPos+diff;
    }

    if ((INT32)(pstAVCtx->uiCurPos + size) > pstAVCtx->iBufLength)
    {
        //		pstAVCtx->uiCurPos -= diff;
        return NULL;
    }

    ptr = pstAVCtx->pucBase + pstAVCtx->uiCurPos;
    pstAVCtx->uiCurPos += size;

    return ptr;
}

static void iMedia_Release(void *pPtr)
{
    pPtr = NULL;
}
