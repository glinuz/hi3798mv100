/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpANSkN3SEmzpLd2djjy+EZczQecyEEWxqdmWeqB
7bvhSvuKnQWjH9ew8kzhF0WWC1beN99h7QGZTzQNK227X5mSZq1ZtpVi92QVIeO5CzCiARwS
Q9rUBL63aC5rp2jbhxGLw1zvM8SokH9VJpH0oZ5uBH+ECNMFdR0lPm4OTPFudA==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/*
 * MPEG4 Video frame extraction
 * Copyright (c) 2003 Fabrice Bellard
 * Copyright (c) 2003 Michael Niedermayer
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

#include "iMedia_error.h"

#include "parser.h"
#include "mpegvideo.h"
#include "mpeg4video_parser.h"


int ff_mpeg4_find_frame_end(ParseContext *pc, const uint8_t *buf, int buf_size)
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

            if (state == 0x1B6)
            {
                i++;
                vop_found = 1;
                break;
            }
        }
    }

    if (vop_found)
    {
        /* EOF considered as end of frame */
        if (buf_size == 0)
        { return 0; }

        for (; i < buf_size; i++)
        {
            state = (state << 8) | buf[i];

            if ((state & 0xFFFFFF00) == 0x100)
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
/* XXX: make it use less memory */
static int av_mpeg4_decode_header(AVCodecParserContext *s1,
                                  AVCodecContext *avctx,
                                  const uint8_t *buf, int buf_size)
{
    ParseContext1 *pc = s1->priv_data;
    MpegEncContext *s = pc->enc;
    GetBitContext gb1;
    GetBitContext *gb = &gb1;
    int ret;

    int ii = 0;

    s->avctx = avctx;
    s->current_picture_ptr = &s->current_picture;

    /*guoshan + 00101841 20100415*/
    /*这个变量被修改成数组，但对于H.263码流，最多只会使用其中的一个，即第0维的数组*/
    /*对于MPEG-4码流，最多只会使用其中的两个，即第0，1维的数组*/
    for (ii = 0; ii < avctx->extradata_num; ii++)
    {
        // if (avctx->extradata_size && pc->first_picture)
        if (avctx->extradata_size[ii] && pc->first_picture)
        {
            // init_get_bits(gb, avctx->extradata, avctx->extradata_size*8);
            init_get_bits(gb, avctx->extradata[ii], avctx->extradata_size[ii] * 8);
            ret = ff_mpeg4_decode_picture_header(s, gb);
        }
    }

    init_get_bits(gb, buf, 8 * buf_size);
    ret = ff_mpeg4_decode_picture_header(s, gb);

    if (s->width)
    {
        avcodec_set_dimensions(avctx, s->width, s->height);
    }

    s1->pict_type = s->pict_type;
    pc->first_picture = 0;
    return ret;
}

static av_cold int mpeg4video_parse_init(AVCodecParserContext *s)
{
    ParseContext1 *pc = s->priv_data;

    pc->enc = av_mallocz(sizeof(MpegEncContext));

    if (!pc->enc)
    { return -1; }

    pc->first_picture = 1;
    return 0;
}


static int mpeg4video_parse(AVCodecParserContext *s,
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
        next = ff_mpeg4_find_frame_end(pc, buf, buf_size);

        if (ff_combine_frame(pc, next, &buf, &buf_size) < 0)
        {
            *poutbuf = NULL;
            *poutbuf_size = 0;
            return buf_size;
        }
    }

    av_mpeg4_decode_header(s, avctx, buf, buf_size);

    *poutbuf = buf;
    *poutbuf_size = buf_size;
    return next;
}
#endif

/*guoshan + 00101841 20100415*/
/* 新增函数：解析一完整图像的mpeg4码流 */
int MPEG4_Frame_Parse(AVCodecContext *pstAVCodecContext, const uint8_t **pucOutBuf, unsigned int *puiOutBufSize, const uint8_t *pucInBuf, unsigned int uiInBufSize, int *piLength)
{
    int iRet = IMEDIA_RET_SUCCESS;

    MpegEncContext *pstMpeg4Context;
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

    pstMpeg4Context  = (MpegEncContext *)pstAVCodecContext->priv_data;
    pstParseContext = &pstMpeg4Context->parse_context;

    iBytesConsumed = ff_mpeg4_find_frame_end(pstParseContext, pucInBuf, uiInBufSize);

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


// AVCodecParser mpeg4video_parser = {
//     { CODEC_ID_MPEG4 },
//     sizeof(ParseContext1),
//     mpeg4video_parse_init,
//     mpeg4video_parse,
//     ff_parse1_close,
//     ff_mpeg4video_split,
// };
