/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCU5hplaH9YrmSXgXDFMULL2AEucV8+0b91N2/j
oHtHIzzOxE/8v8EkuPs+GDG9l7tRcP2+z+h8rrs/SrWHAn+++F+uuzPNfn2R/6kDpbbkFJHa
UPacv65LiL0YwqR/Ack/csHpmt53EelFhadbwKvATGKrwgokDW1h2KwR50HtZg==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/*
 * VC-1 and WMV3 parser
 * Copyright (c) 2006-2007 Konstantin Shishkov
 * Partly based on vc9.c (c) 2005 Anonymous, Alex Beregszaszi, Michael Niedermayer
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
 * @file libavcodec/vc1_parser.c
 * VC-1 and WMV3 parser
 */

#include "parser.h"
#include "vc1.h"

/**
 * finds the end of the current frame in the bitstream.
 * @return the position of the first byte of the next frame, or -1
 */
static int vc1_find_frame_end(ParseContext *pc, const uint8_t *buf,
                              int buf_size)
{
    int pic_found, i;
    uint32_t state;

    pic_found = pc->frame_start_found;
    state = pc->state;

    i = 0;

    if (!pic_found)
    {
        for (i = 0; i < buf_size; i++)
        {
            state = (state << 8) | buf[i];

            if (state == VC1_CODE_FRAME || state == VC1_CODE_FIELD)
            {
                i++;
                pic_found = 1;
                break;
            }
        }
    }

    if (pic_found)
    {
        /* EOF considered as end of frame */
        if (buf_size == 0)
        { return 0; }

        for (; i < buf_size; i++)
        {
            state = (state << 8) | buf[i];

            if (IS_MARKER(state) && state != VC1_CODE_FIELD && state != VC1_CODE_SLICE)
            {
                pc->frame_start_found = 0;
                pc->state = -1;
                return i - 3;
            }
        }
    }

    pc->frame_start_found = pic_found;
    pc->state = state;
    return END_NOT_FOUND;
}

static int vc1_parse(AVCodecParserContext *s,
                     AVCodecContext *avctx,
                     const uint8_t **poutbuf, int *poutbuf_size,
                     const uint8_t *buf, int buf_size)
{
    ParseContext *pc = s->priv_data;
    int next;

    if (s->flags & PARSER_FLAG_COMPLETE_FRAMES)
    {
        next = buf_size;
    }
    else
    {
        next = vc1_find_frame_end(pc, buf, buf_size);

        if (ff_combine_frame(pc, next, &buf, &buf_size) < 0)
        {
            *poutbuf = NULL;
            *poutbuf_size = 0;
            return buf_size;
        }
    }

    *poutbuf = buf;
    *poutbuf_size = buf_size;
    return next;
}

/* 2010/05/31 10:00:00 liuxw+00139685 */
/* 新增函数：寻找完整的一幅图像(一场或是一帧) */
int VC1_Frame_Parse(AVCodecContext *pstAVCodecContext, const uint8_t **pucOutBuf, unsigned int *puiOutBufSize, const uint8_t *pucInBuf, unsigned int uiInBufSize, int *piLength)
{
    int iRet = IMEDIA_RET_SUCCESS;

    VC1Context *pstVC1Context;
    ParseContext *pstParseContext;
    int iBytesConsumed = 0;
    int iReturn = 0;

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

    pstVC1Context  = (VC1Context *)pstAVCodecContext->priv_data;
    pstParseContext = &pstVC1Context->s.parse_context;


    iBytesConsumed = vc1_find_frame_end(pstParseContext, pucInBuf, uiInBufSize);

    iReturn = ff_combine_frame(pstParseContext, iBytesConsumed, &pucInBuf, &uiInBufSize);

    if (0 > iReturn)
    {
        /* start and end of the frame is not fount! */
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


static int vc1_split(AVCodecContext *avctx,
                     const uint8_t *buf, int buf_size)
{
    int i;
    uint32_t state = -1;
    int charged = 0;

    for (i = 0; i < buf_size; i++)
    {
        state = (state << 8) | buf[i];

        if (IS_MARKER(state))
        {
            if (state == VC1_CODE_SEQHDR || state == VC1_CODE_ENTRYPOINT)
            {
                charged = 1;
            }
            else if (charged)
            {
                return i - 3;
            }
        }
    }

    return 0;
}

AVCodecParser vc1_parser =
{
    { CODEC_ID_VC1 },
    sizeof(ParseContext1),
    NULL,
    vc1_parse,
    ff_parse1_close,
    vc1_split,
};
