/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCU5hplaH9YrmSXgXDFMULL2AEucV8+0b91N2/j
oHtHIzSNb4kZY4lyOizLZeCm3IALqWX2LQkmLOlkesKau8v7KpK0sFuzMGUh5hxoLw6aE8Z6
aaGyeM/pxNbSwrMnBwEFO4Bp8RAsFK6qZIW8xtRFO7uLiuh67/kY8KZlisVoZQ==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/*
 * Copyright (C) 2008 Michael Niedermayer
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
#include "parser.h"

/*x00141957 20100628*/
#include "vp56.h"

static int parse(AVCodecParserContext *s,
                 AVCodecContext *avctx,
                 const uint8_t **poutbuf, int *poutbuf_size,
                 const uint8_t *buf, int buf_size)
{
    if (avctx->codec_id == CODEC_ID_THEORA)
    { s->pict_type = (buf[0] & 0x40) ? FF_P_TYPE : FF_I_TYPE; }
    else
    { s->pict_type = (buf[0] & 0x80) ? FF_P_TYPE : FF_I_TYPE; }

    *poutbuf = buf;
    *poutbuf_size = buf_size;
    return buf_size;
}

AVCodecParser vp3_parser =
{
    {
        CODEC_ID_THEORA, CODEC_ID_VP3,
        CODEC_ID_VP6,    CODEC_ID_VP6F, CODEC_ID_VP6A
    },
    0,
    NULL,
    parse,
};

int ff_vp6_find_frame_end(ParseContext *pc, const uint8_t *buf, int buf_size)
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

            if (state >> (32 - 24) == 0x01)
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

            if (state >> (32 - 24) == 0x01)
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

/*x00141957 20100629*/
/* 新增函数：解析一完整图像的vp6码流 */
int vp6_Frame_Parse(AVCodecContext *pstAVCodecContext, const uint8_t **pucOutBuf, unsigned int *puiOutBufSize, const uint8_t *pucInBuf, unsigned int uiInBufSize, int *piLength)
{
    int iRet = IMEDIA_RET_SUCCESS;


    VP56Context *pstVP6Context;
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

    pstVP6Context = (VP56Context *)pstAVCodecContext->priv_data;

    /*内存延迟分配 x00141957 20100916*/
    if (NULL == pstVP6Context->parse_context.buffer)
    {
        pstVP6Context->parse_context.buffer_size = pstAVCodecContext->usSourceWidth * pstAVCodecContext->usSourceHeight * 2;
        pstVP6Context->parse_context.buffer = av_malloc_hw(pstAVCodecContext, pstVP6Context->parse_context.buffer_size);
        pstVP6Context->parse_context.state = -1;
    }

    pstParseContext = &pstVP6Context->parse_context;

    iBytesConsumed = ff_vp6_find_frame_end(pstParseContext, pucInBuf, uiInBufSize);

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

    /*x00141957 20100917增加 根据找到的一帧大小 去掉私有头以及防错码*/
    {
        uint8_t *uiBuf = (uint8_t *)pucInBuf + 3;
        int bufsize = (int)uiInBufSize - 3;
        int i, j;

        for (i = 0; i < bufsize - 4; i++)
        {
            if ((uiBuf[i] == 0) && (uiBuf[i + 1] == 0) && (uiBuf[i + 2] == 3) && ((uiBuf[i + 3] == 3) || (uiBuf[i + 3] == 1)))
            {
                for (j = i + 2; j < (int)uiInBufSize - 2; j++)
                {
                    uiBuf[j] = uiBuf[j + 1];
                }

                bufsize -= 1;
                //i+=2;
            }
        }

        *pucOutBuf     = uiBuf;

        if (bufsize < 0)
        {
            bufsize = 0;
        }

        *puiOutBufSize = bufsize;
    }

    *piLength = iBytesConsumed;

    return iRet;
}