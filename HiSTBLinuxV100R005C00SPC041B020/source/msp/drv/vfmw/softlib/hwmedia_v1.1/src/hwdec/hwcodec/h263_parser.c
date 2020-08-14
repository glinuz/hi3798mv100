/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpACtEOGRYSklhuLfno8c3LrcT0rqTCexJu34jBP
BVpQe+t+WlNepI3OQFu51Rbiswg9i5PKw0N5JnJJdWOWWSudiEbPzOPMJcrR2GZaEOEw159W
FwnhaTyJKYcNuAlP8dEpGTceDo9Ith1NMFHaTytBjs6f2clnOcBLObf5tqsF+Q==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/*
 * H.263 parser
 * Copyright (c) 2002-2004 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file libavcodec/h263_parser.c
 * H.263 parser
 */

#include "iMedia_error.h"
#include "mpegvideo.h"
#include "parser.h"

int ff_h263_find_frame_end(ParseContext *pc, const uint8_t *buf, int buf_size)
{
    int vop_found, i;
    uint32_t state;

    vop_found = pc->frame_start_found;
    state = pc->state;

    i = 0;

    if (!vop_found)
    {
        for (i = 0; i < buf_size; i++)
        {
            state = (state << 8) | buf[i];

            if (state >> (32 - 22) == 0x20)
            {
                i++;
                vop_found = 1;
                break;
            }
        }
    }

    if (vop_found)
    {
        for (; i < buf_size; i++)
        {
            state = (state << 8) | buf[i];

            if (state >> (32 - 22) == 0x20)
            {
                pc->frame_start_found = 0;
                pc->state = -1;
                return i - 3;
            }
        }
    }

    pc->frame_start_found = vop_found;
    pc->state = state;

    return END_NOT_FOUND;
}

#if 0
static int h263_parse(AVCodecParserContext *s,
                      AVCodecContext *avctx,
                      const uint8_t **poutbuf, int *poutbuf_size,
                      const uint8_t *buf, int buf_size)
{
    ParseContext *pc = s->priv_data;
    int next;

    next = ff_h263_find_frame_end(pc, buf, buf_size);

    if (ff_combine_frame(pc, next, &buf, &buf_size) < 0)
    {
        *poutbuf = NULL;
        *poutbuf_size = 0;
        return buf_size;
    }

    *poutbuf = buf;
    *poutbuf_size = buf_size;
    return next;
}
#endif

/*guoshan + 00101841 2010811*/
/* 新增函数：find sorenson spark帧 */
int sorenson_spark_find_frame_end(ParseContext *pc, const uint8_t *buf, int buf_size)
{
    int vop_found, i;
    uint32_t state;

    vop_found = pc->frame_start_found;
    state = pc->state;

    i = 0;

    if (!vop_found)
    {
        for (i = 0; i < buf_size; i++)
        {
            state = (state << 8) | buf[i];

            if (state >> (32 - 17) == 0x01)
            {
                i++;
                vop_found = 1;
                break;
            }
        }
    }

    if (vop_found)
    {
        for (; i < buf_size; i++)
        {
            state = (state << 8) | buf[i];

            if (state >> (32 - 17) == 0x01)
            {
                pc->frame_start_found = 0;
                pc->state = -1;
                return i - 3;
            }
        }
    }

    pc->frame_start_found = vop_found;
    pc->state = state;

    return END_NOT_FOUND;
}



/*guoshan + 00101841 20100415*/
/* 新增函数：解析一帧完整的263码流 */
int H263_Frame_Parse(AVCodecContext *pstAVCodecContext, const uint8_t **pucOutBuf, unsigned int *puiOutBufSize, const uint8_t *pucInBuf, unsigned int uiInBufSize, int *piLength)
{
    int iRet = IMEDIA_RET_SUCCESS;

    MpegEncContext *pstH263Context;
    ParseContext *pstParseContext;
    int iBytesConsumed = 0;

    if (NULL == pstAVCodecContext)
    {
        av_log(pstAVCodecContext, AV_LOG_ERROR, "the decoder handle is NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (0 == uiInBufSize)
    {
        av_log(pstAVCodecContext, AV_LOG_INFO, "the length of input bitstream is zero!\n");
    }

    if (0 != uiInBufSize && NULL == pucInBuf)
    {
        av_log(pstAVCodecContext, AV_LOG_ERROR, "input bitstream buf is NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pucOutBuf || NULL == puiOutBufSize)
    {
        av_log(pstAVCodecContext, AV_LOG_ERROR, "output bitstream buf or length pointer is NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    pstH263Context  = (MpegEncContext *)pstAVCodecContext->priv_data;
    pstParseContext = &pstH263Context->parse_context;

    if (pstH263Context->parse_context.buffer == NULL)
    {
        pstH263Context->parse_context.buffer_size = pstAVCodecContext->usSourceWidth * pstAVCodecContext->usSourceHeight * 2;
        pstH263Context->parse_context.buffer      = av_malloc_hw(pstAVCodecContext, pstH263Context->parse_context.buffer_size);

        if (NULL == pstH263Context->parse_context.buffer)
        {
            av_log(pstAVCodecContext, AV_LOG_ERROR, "dec_ctx[%p] malloc buffer for pstH263Context->parse_context.buffer failed!\n", pstAVCodecContext);
            MPV_common_end(pstH263Context);
            return -1;
        }

        pstH263Context->parse_context.state = -1;
    }

    iBytesConsumed = ff_h263_find_frame_end(pstParseContext, pucInBuf, uiInBufSize);

    if (ff_combine_frame(pstParseContext, iBytesConsumed, &pucInBuf, &uiInBufSize) < 0)
    {
        /* start or end of the frame is not found! */
        *pucOutBuf = NULL;
        *puiOutBufSize = 0;
        *piLength = uiInBufSize;
        return iRet;
    }

    /* 如果当前解析帧在前一个buffer的边界，则当前buffer的解析用掉的字节数为0 */
    if (0 > iBytesConsumed)
    {
        iBytesConsumed = 0;
    }

    *pucOutBuf     = pucInBuf;
    *puiOutBufSize = uiInBufSize;
    *piLength       = iBytesConsumed;

    return iRet;
}


/*guoshan + 00101841 20100811*/
/* 新增函数：解析一帧完整的sorenson spark码流*/
int Sorenson_Spark_Frame_Parse(AVCodecContext *pstAVCodecContext, const uint8_t **pucOutBuf, unsigned int *puiOutBufSize, const uint8_t *pucInBuf, unsigned int uiInBufSize, int *piLength)
{
    int iRet = IMEDIA_RET_SUCCESS;

    MpegEncContext *pstSorensonContext;
    ParseContext *pstParseContext;
    int iBytesConsumed = 0;

    if (NULL == pstAVCodecContext)
    {
        av_log(pstAVCodecContext, AV_LOG_ERROR, "the decoder handle is NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (0 == uiInBufSize)
    {
        av_log(pstAVCodecContext, AV_LOG_INFO, "the length of input bitstream is zero!\n");
    }

    if (0 != uiInBufSize && NULL == pucInBuf)
    {
        av_log(pstAVCodecContext, AV_LOG_ERROR, "input bitstream buf is NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pucOutBuf || NULL == puiOutBufSize)
    {
        av_log(pstAVCodecContext, AV_LOG_ERROR, "output bitstream buf or length pointer is NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    pstSorensonContext  = (MpegEncContext *)pstAVCodecContext->priv_data;
    pstParseContext = &pstSorensonContext->parse_context;

    if (pstSorensonContext->parse_context.buffer == NULL)
    {
        pstSorensonContext->parse_context.buffer_size = pstAVCodecContext->usSourceWidth * pstAVCodecContext->usSourceHeight * 2;
        pstSorensonContext->parse_context.buffer      = av_malloc_hw(pstAVCodecContext, pstSorensonContext->parse_context.buffer_size);

        if (NULL == pstSorensonContext->parse_context.buffer)
        {
            av_log(pstAVCodecContext, AV_LOG_ERROR, "dec_ctx[%p] malloc buffer for pstSorensonContext->parse_context.buffer failed!\n", pstAVCodecContext);
            MPV_common_end(pstSorensonContext);
            return -1;
        }

        pstSorensonContext->parse_context.state = -1;
    }

    iBytesConsumed = sorenson_spark_find_frame_end(pstParseContext, pucInBuf, uiInBufSize);

    if (ff_combine_frame(pstParseContext, iBytesConsumed, &pucInBuf, &uiInBufSize) < 0)
    {
        /* start or end of the frame is not found! */
        *pucOutBuf = NULL;
        *puiOutBufSize = 0;
        *piLength = uiInBufSize;
        return iRet;
    }

    /* 如果当前解析帧在前一个buffer的边界，则当前buffer的解析用掉的字节数为0 */
    if (0 > iBytesConsumed)
    {
        iBytesConsumed = 0;
    }

    *pucOutBuf     = pucInBuf;
    *puiOutBufSize = uiInBufSize;
    *piLength       = iBytesConsumed;

    return iRet;
}

// AVCodecParser h263_parser = {
//     { CODEC_ID_H263 },
//     sizeof(ParseContext),
//     NULL,
//     h263_parse,
//     ff_parse_close,
// };
