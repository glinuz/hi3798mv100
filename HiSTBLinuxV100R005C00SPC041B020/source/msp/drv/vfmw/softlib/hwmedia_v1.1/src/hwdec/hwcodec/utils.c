/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCU5hplaH9YrmSXgXDFMULL2AEucV8+0b91N2/j
oHtHI7tUQvZGiqtJ3392rbNSUW7/tHwJFGBapAaQVgf8h1iUeEKcU3LtQyffnnk5Bb3/6TT2
PZEmE9qxAH76A2lHqKTMLA90GlFBe9Z/aSMiU03HTSJ7KRSFgsO+tCUWCnMVXg==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/*
 * utils for libavcodec
 * Copyright (c) 2001 Fabrice Bellard
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
 * @file libavcodec/utils.c
 * utils.
 */

/* needed for mkstemp() */
#define _XOPEN_SOURCE 600

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <float.h>
#ifndef __KERNEL__
#include <stddef.h>
#include <limits.h>
#endif

#if !HAVE_MKSTEMP
#ifndef __KERNEL__
#include <fcntl.h>
#endif
#ifdef _MSC_VER
#include <io.h>
#endif
#endif

#include "avstring.h"
#include "integer.h"
//#include "crc.h"
#include "avcodec.h"
#include "dsputil.h"
#include "opt.h"
#include "imgconvert.h"
//#include "audioconvert.h"
#include "internal.h"

/* 2010/30/04 10:50:00 liuxw+00139685 */
/* 增加头文件，方便设置codec error type */
//#include "iMedia_error.h"

#define Max_STRNCMP_LEN 256

const uint8_t ff_reverse[256] =
{
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF,
};

static int volatile entangled_thread_counter = 0;

void *av_fast_realloc(void *ptr, unsigned int *size, unsigned int min_size)
{
    if (min_size < *size)
    { return ptr; }

    *size = FFMAX(17 * min_size / 16 + 32, min_size);

    ptr = av_realloc(ptr, *size);

    if (!ptr) //we could set this to the unmodified min_size but this is safer if the user lost the ptr and uses NULL now
    { *size = 0; }

    return ptr;
}

/* encoder management */
static AVCodec *first_avcodec = NULL;

AVCodec *av_codec_next(AVCodec *c)
{
    if (c) { return c->next; }
    else  { return first_avcodec; }
}

void avcodec_register(AVCodec *codec)
{
    AVCodec **p;
    avcodec_init();
    p = &first_avcodec;

    while (*p != NULL) { p = &(*p)->next; }

    *p = codec;
    codec->next = NULL;
}

#if LIBAVCODEC_VERSION_MAJOR < 53
void register_avcodec(AVCodec *codec)
{
    avcodec_register(codec);
}
#endif

void avcodec_set_dimensions(AVCodecContext *s, int width, int height)
{
    s->coded_width = width;
    s->coded_height = height;
    s->width = -((-width ) >> s->lowres);
    s->height = -((-height) >> s->lowres);
}

/*guoshan + 00101841 20100416*/
/*新增函数,设置chroma_format*/
void avcodec_set_chroma_format(AVCodecContext *s, int chroma_format)
{
    s->iChromaFormat = chroma_format;
}

//typedef struct InternalBuffer{
//    int last_pic_num;
//    uint8_t *base[4];
//    uint8_t *data[4];
//    int linesize[4];
//    int width, height;
//    enum PixelFormat pix_fmt;
//}InternalBuffer;

#undef  ALIGN
#define ALIGN(x, a) (((x)+(a)-1)&~((a)-1))

void avcodec_align_dimensions(AVCodecContext *s, int *width, int *height)
{
    int w_align = 1;
    int h_align = 1;

    switch (s->pix_fmt)
    {
        case PIX_FMT_YUV420P:
        case PIX_FMT_YUYV422:
        case PIX_FMT_UYVY422:
        case PIX_FMT_YUV422P:
        case PIX_FMT_YUV444P:
        case PIX_FMT_GRAY8:
        case PIX_FMT_GRAY16BE:
        case PIX_FMT_GRAY16LE:
        case PIX_FMT_YUVJ420P:
        case PIX_FMT_YUVJ422P:
        case PIX_FMT_YUVJ444P:
        case PIX_FMT_YUVA420P:
            w_align = 16; //FIXME check for non mpeg style codecs and use less alignment
            h_align = 16;
            break;

        case PIX_FMT_YUV411P:
        case PIX_FMT_UYYVYY411:
            w_align = 32;
            h_align = 8;
            break;

        case PIX_FMT_YUV410P:
            if (s->codec_id == CODEC_ID_SVQ1)
            {
                w_align = 64;
                h_align = 64;
            }

            break;

        case PIX_FMT_RGB555:
            if (s->codec_id == CODEC_ID_RPZA)
            {
                w_align = 4;
                h_align = 4;
            }

            break;

        case PIX_FMT_PAL8:
        case PIX_FMT_BGR8:
        case PIX_FMT_RGB8:
            if (s->codec_id == CODEC_ID_SMC)
            {
                w_align = 4;
                h_align = 4;
            }

            break;

        case PIX_FMT_BGR24:
            if ((s->codec_id == CODEC_ID_MSZH) || (s->codec_id == CODEC_ID_ZLIB))
            {
                w_align = 4;
                h_align = 4;
            }

            break;

        default:
            w_align = 1;
            h_align = 1;
            break;
    }

    *width = ALIGN(*width , w_align);
    *height = ALIGN(*height, h_align);

    if (s->codec_id == CODEC_ID_H264)
    { *height += 2; } // some of the optimized chroma MC reads one line too much
}

int avcodec_check_dimensions(void *av_log_ctx, unsigned int w, unsigned int h)
{
    if ((int)w > 0 && (int)h > 0 && (w + 128) * (uint64_t)(h + 128) < INT_MAX / 4)
    { return 0; }

    //  av_log(av_log_ctx, AV_LOG_ERROR, "picture size invalid (%ux%u)\n", w, h);
    av_log(av_log_ctx, AV_LOG_WARNING, "picture size invalid (%ux%u)\n", w, h);
    return -1;
}

int avcodec_default_get_buffer(AVCodecContext *s, AVFrame *pic)
{
    int i;
    /*x001491957 20100914*/
    int w = s->width;
    int h = s->height;
    InternalBuffer *buf;
    int *picture_number;

    if (pic->data[0] != NULL)
    {
        //      av_log(s, AV_LOG_ERROR, "pic->data[0]!=NULL in avcodec_default_get_buffer\n");
        av_log(s, AV_LOG_WARNING, "pic->data[0]!=NULL in avcodec_default_get_buffer\n");
        return -1;
    }

    if (s->internal_buffer_count >= INTERNAL_BUFFER_SIZE)
    {
        //      av_log(s, AV_LOG_ERROR, "internal_buffer_count overflow (missing release_buffer?)\n");
        av_log(s, AV_LOG_WARNING, "internal_buffer_count overflow (missing release_buffer?)\n");
        return -1;
    }

    if (avcodec_check_dimensions(s, w, h))
    { return -1; }

    if (s->internal_buffer == NULL)
    {
        //      s->internal_buffer= av_mallocz((INTERNAL_BUFFER_SIZE+1)*sizeof(InternalBuffer));
        s->internal_buffer = av_malloc_hw(s, (INTERNAL_BUFFER_SIZE + 1) * sizeof(InternalBuffer));

        /* 2010/06/10 18:30:00 liuxw+00139685 */
        /* 增加对内存分配失败的检测及返回 */
        if (NULL == s->internal_buffer)
        {
            av_log(s, AV_LOG_ERROR, "Malloc memory for internal buffer failed!\n");
            s->iMallocFailed = 1;
            return -1;
        }

        memset(s->internal_buffer, 0, (INTERNAL_BUFFER_SIZE + 1)*sizeof(InternalBuffer));
    }

#if 0
    s->internal_buffer = av_fast_realloc(
                             s->internal_buffer,
                             &s->internal_buffer_size,
                             sizeof(InternalBuffer) * FFMAX(99,  s->internal_buffer_count + 1) /*FIXME*/
                         );
#endif

    buf = &((InternalBuffer *)s->internal_buffer)[s->internal_buffer_count];
    picture_number = &(((InternalBuffer *)s->internal_buffer)[INTERNAL_BUFFER_SIZE]).last_pic_num; //FIXME ugly hack
    (*picture_number)++;

    if (buf->base[0] && (buf->width != w || buf->height != h || buf->pix_fmt != s->pix_fmt))
    {
        for (i = 0; i < 4; i++)
        {
            av_freep(&buf->base[i]);
            buf->data[i] = NULL;
        }
    }

    if (buf->base[0])
    {
        pic->age = *picture_number - buf->last_pic_num;
        buf->last_pic_num = *picture_number;
    }
    else
    {
        int h_chroma_shift, v_chroma_shift;
        int size[4] = {0};
        int tmpsize;
        AVPicture picture;
        int stride_align[4];

        avcodec_get_chroma_sub_sample(s->pix_fmt, &h_chroma_shift, &v_chroma_shift);

        avcodec_align_dimensions(s, &w, &h);

        if (!(s->flags & CODEC_FLAG_EMU_EDGE))
        {
            w += EDGE_WIDTH * 2;
            h += EDGE_WIDTH * 2;
        }

        ff_fill_linesize(&picture, s->pix_fmt, w);

        for (i = 0; i < 4; i++)
        {
            //STRIDE_ALIGN is 8 for SSE* but this does not work for SVQ1 chroma planes
            //we could change STRIDE_ALIGN to 16 for x86/sse but it would increase the
            //picture size unneccessarily in some cases. The solution here is not
            //pretty and better ideas are welcome!
#if HAVE_MMX
            if (s->codec_id == CODEC_ID_SVQ1)
            { stride_align[i] = 16; }
            else
#endif
                stride_align[i] = STRIDE_ALIGN;

            picture.linesize[i] = ALIGN(picture.linesize[i], stride_align[i]);
        }

        tmpsize = ff_fill_pointer(&picture, NULL, s->pix_fmt, h);

        if (tmpsize < 0)
        { return -1; }

        for (i = 0; i < 3 && picture.data[i + 1]; i++)
        { size[i] = picture.data[i + 1] - picture.data[i]; }

        size[i] = tmpsize - (picture.data[i] - picture.data[0]);

        buf->last_pic_num = -256 * 256 * 256 * 64;
        memset(buf->base, 0, sizeof(buf->base));
        memset(buf->data, 0, sizeof(buf->data));

        /*x00141957 20100914 对于size[0] 分配y+0.5*y大小空间*/
        /*x00141957 20110726 对于size[0] 增加分配y大小空间*/
        size[0] = (size[0] << 1) + (size[0] >> 1);

        for (i = 0; i < 4 && size[i]; i++)
        {
            const int h_shift = i == 0 ? 0 : h_chroma_shift;
            const int v_shift = i == 0 ? 0 : v_chroma_shift;

            buf->linesize[i] = picture.linesize[i];

            /* 2010/30/06 9:30:00 liuxw+00139685 */
            /* 将YUV buf的分配改为对齐式分配 */
            //          buf->base[i]= av_malloc(size[i]+16); //FIXME 16
            //			buf->base[i]= av_malloc_align(size[i], 16);
            buf->base[i] = av_malloc_hw(s, size[i]);

            if (buf->base[i] == NULL)
            {
                s->iMallocFailed = 1;
                av_log(s, AV_LOG_ERROR, "Malloc memory for yuv buffer failed!\n");
                return -1;
            }

            memset(buf->base[i], 128, size[i]);

            // no edge if EDEG EMU or not planar YUV
            if ((s->flags & CODEC_FLAG_EMU_EDGE) || !size[2])
            { buf->data[i] = buf->base[i]; }
            else
            { buf->data[i] = buf->base[i] + ALIGN((buf->linesize[i] * EDGE_WIDTH >> v_shift) + (EDGE_WIDTH >> h_shift), stride_align[i]); }
        }

        if (size[1] && !size[2])
        { ff_set_systematic_pal((uint32_t *)buf->data[1], s->pix_fmt); }

        buf->width  = s->width;
        buf->height = s->height;
        buf->pix_fmt = s->pix_fmt;
        pic->age = 256 * 256 * 256 * 64;
    }

    pic->type = FF_BUFFER_TYPE_INTERNAL;

    for (i = 0; i < 4; i++)
    {
        pic->base[i] = buf->base[i];
        pic->data[i] = buf->data[i];
        pic->linesize[i] = buf->linesize[i];
    }

    s->internal_buffer_count++;

    /* 2010/30/06 9:30:00 liuxw+00139685 */
    /* 统计使用YUV buffer的数量 */
    s->uiInUsedBufNum = s->internal_buffer_count; /*  */

    for (i = s->uiInUsedBufNum; i < INTERNAL_BUFFER_SIZE; i++)
    {
        if (NULL == ((InternalBuffer *)s->internal_buffer)[i].base[0])
        {
            break;
        }
    }

    /* 2010/30/06 9:30:00 liuxw+00139685 */
    /* 统计空闲YUV buffer的数量 */
    s->uiFreeBufNum = i - s->uiInUsedBufNum; /*  */

    pic->reordered_opaque = s->reordered_opaque;

    if (s->debug & FF_DEBUG_BUFFERS)
    { av_log(s, AV_LOG_DEBUG, "default_get_buffer called on pic %p, %d buffers used\n", pic, s->internal_buffer_count); }

    return 0;
}

void avcodec_default_release_buffer(AVCodecContext *s, AVFrame *pic)
{
    int i;
    InternalBuffer *buf, *last;

    assert(pic->type == FF_BUFFER_TYPE_INTERNAL);
    assert(s->internal_buffer_count);

    buf = NULL; /* avoids warning */

    for (i = 0; i < s->internal_buffer_count; i++) //just 3-5 checks so is not worth to optimize
    {
        buf = &((InternalBuffer *)s->internal_buffer)[i];

        if (buf->data[0] == pic->data[0])
        { break; }
    }

    assert(i < s->internal_buffer_count);
    s->internal_buffer_count--;
    last = &((InternalBuffer *)s->internal_buffer)[s->internal_buffer_count];

    FFSWAP(InternalBuffer, *buf, *last);

    for (i = 0; i < 3/* 4 */; i++)
    {
        pic->data[i] = NULL;
        //      pic->base[i]=NULL;
    }

    if (s->debug & FF_DEBUG_BUFFERS)
    { av_log(s, AV_LOG_DEBUG, "default_release_buffer called on pic %p, %d buffers used\n", pic, s->internal_buffer_count); }
}

int avcodec_default_reget_buffer(AVCodecContext *s, AVFrame *pic)
{
    AVFrame temp_pic;
    int i;

    /* If no picture return a new buffer */
    if (pic->data[0] == NULL)
    {
        /* We will copy from buffer, so must be readable */
        pic->buffer_hints |= FF_BUFFER_HINTS_READABLE;
        return s->get_buffer(s, pic);
    }

    /* If internal buffer type return the same buffer */
    if (pic->type == FF_BUFFER_TYPE_INTERNAL)
    { return 0; }

    /*
     * Not internal type and reget_buffer not overridden, emulate cr buffer
     */
    temp_pic = *pic;

    for (i = 0; i < 4; i++)
    { pic->data[i] = pic->base[i] = NULL; }

    pic->opaque = NULL;

    /* Allocate new frame */
    if (s->get_buffer(s, pic))
    { return -1; }

    /* Copy image data from old buffer to new buffer */
    av_picture_copy((AVPicture *)pic, (AVPicture *)&temp_pic, s->pix_fmt, s->width,
                    s->height);
    s->release_buffer(s, &temp_pic); // Release old frame
    return 0;
}

int avcodec_default_execute(AVCodecContext *c, int (*func)(AVCodecContext *c2, void *arg2), void *arg, int *ret, int count, int size)
{
    int i;

    for (i = 0; i < count; i++)
    {
        int r = func(c, (char *)arg + i * size);

        if (ret) { ret[i] = r; }
    }

    return 0;
}

enum PixelFormat avcodec_default_get_format(struct AVCodecContext *s, const enum PixelFormat *fmt)
{
    while (*fmt != PIX_FMT_NONE && ff_is_hwaccel_pix_fmt(*fmt))
    { ++fmt; }

    return fmt[0];
}

void avcodec_get_frame_defaults(AVFrame *pic)
{
    memset(pic, 0, sizeof(AVFrame));

    pic->pts = AV_NOPTS_VALUE;
    pic->key_frame = 1;
}
#if 0
AVFrame *avcodec_alloc_frame(void)
{
    AVFrame *pic = av_malloc_hw(sizeof(AVFrame));

    if (pic == NULL) { return NULL; }

    avcodec_get_frame_defaults(pic);

    return pic;
}
#endif
int attribute_align_arg avcodec_open(AVCodecContext *avctx, AVCodec *codec)
{
    int ret = -1;

    entangled_thread_counter++;

    if (entangled_thread_counter != 1)
    {
        av_log(avctx, AV_LOG_ERROR, "insufficient thread locking around avcodec_open/close()\n");
        goto end;
    }

    if (avctx->codec || !codec)
    {
        goto end;
    }

    if (codec->priv_data_size > 0)
    {
        //      avctx->priv_data = av_mallocz(codec->priv_data_size);
        avctx->priv_data = av_malloc_hw(avctx, codec->priv_data_size);

        if (!avctx->priv_data)
        {
            /* 2010/30/04 10:46:00 liuxw+00139685 */
            /* 增加日志及设置错误码 */
            av_log(avctx, AV_LOG_ERROR, "dec_ctx[%p] allocate %d byte memory for codec priv_data failed!\n", avctx, codec->priv_data_size);
            //          avctx->iErrorCode = IMEDIA_CODEC_ERR_MEM_MALLOC_FAIL;
            ret = AVERROR(ENOMEM);
            goto end;
        }

        memset(avctx->priv_data, 0, codec->priv_data_size);
    }
    else
    {
        avctx->priv_data = NULL;
    }

    if (avctx->coded_width && avctx->coded_height)
    { avcodec_set_dimensions(avctx, avctx->coded_width, avctx->coded_height); }
    else if (avctx->width && avctx->height)
    { avcodec_set_dimensions(avctx, avctx->width, avctx->height); }

    if ((avctx->coded_width || avctx->coded_height) && avcodec_check_dimensions(avctx, avctx->coded_width, avctx->coded_height))
    {
        av_freep(&avctx->priv_data);
        ret = AVERROR(EINVAL);
        goto end;
    }

    avctx->codec = codec;
    avctx->codec_id = codec->id;
    avctx->frame_number = 0;
    /* 2010/30/06 11:48:00 liuxw+00139685 */
    /* 初始化线程数目为1 */
    avctx->thread_count = 1;

    if (avctx->codec->init)
    {
        ret = avctx->codec->init(avctx);

        if (ret < 0)
        {
            av_freep(&avctx->priv_data);
            avctx->codec = NULL;
            goto end;
        }
    }

    ret = 0;
end:

    entangled_thread_counter--;
    return ret;
}

/* 新增函数(l00139685)：复位及初始化AVCodecContext结构体变量 */
int attribute_align_arg avcodec_reset(AVCodecContext *avctx)
{
    int iRet = 0;

    AVRational		av01		= {0, 1};

    /* 分配变量保存初始化参数 */
    AVCodec 		*codec		= avctx->codec;
    void			*priv_data	= avctx->priv_data;
    AVClass			*av_class	= (AVClass *)avctx->av_class;

    /* 分配变量保存静态参数 */
    unsigned short usSourceWidth	= avctx->usSourceWidth;
    unsigned short usSourceHeight	= avctx->usSourceHeight;
    unsigned int   uiRefFrame		= avctx->uiRefFrame;
    unsigned int   uiProfileIdc		= avctx->uiProfileIdc;
    unsigned int   uiLevelIdc		= avctx->uiLevelIdc;
    int			   eColorSpaceType	= avctx->eColorSpaceType;
    char		   bForceOutYUV420	= avctx->bForceOutYUV420;
    char		   bDeblockFilter	= avctx->bDeblockFilter;
    int			   eFrameSkipMode	= avctx->eFrameSkipMode;
    int			   iChromaFormat    = avctx->iSourceChromaFormat;
    unsigned int   uiBufNum         = avctx->uiBufNum;
    unsigned char  *pucBase         = avctx->pucBase;
    int			   iBufLength       = avctx->iBufLength;
    unsigned int   uiCurPos         = avctx->uiCurPos;

    /*guoshan+00101841 20100813 问题单：[AZ1D02261]*/
    /*reset之前保留dsp_mask的状态*/
    unsigned int dsp_mask          = avctx->dsp_mask;
    int			 eCodecType        = avctx->eCodecType;
    int          flags             = avctx->flags;
    int          flags2             = avctx->flags2;
    void           *internal_buffer            = avctx->internal_buffer;
    //	int            internal_buffer_count       = avctx->internal_buffer_count;

    /* 清空AVCodecContext结构体变量 */
    memset(avctx, 0, sizeof(AVCodecContext));

    /* 赋默认值(AVCodecContext结构体变量) */
    //	avctx->av_class				= &av_codec_context_class;
    avctx->codec_type   		= CODEC_TYPE_UNKNOWN;
    avctx->rc_eq				= "tex^qComp";
    avctx->time_base			= av01;
    avctx->get_buffer			= avcodec_default_get_buffer;
    avctx->release_buffer		= avcodec_default_release_buffer;
    avctx->get_format			= avcodec_default_get_format;
    avctx->execute				= avcodec_default_execute;
    avctx->sample_aspect_ratio	= av01;
    avctx->pix_fmt				= PIX_FMT_NONE;
    avctx->sample_fmt			= SAMPLE_FMT_S16;
    avctx->palctrl				= NULL;
    avctx->reget_buffer			= avcodec_default_reget_buffer;
    avctx->iChromaFormat		= INT_MAX;
    /* 2010/07/23 15:30:00 liuxw+00139685 */
    /* 将sample_aspect_ratio的num和den的初始值设为1 */
    avctx->sample_aspect_ratio.num = 1;
    avctx->sample_aspect_ratio.den = 1;
    /* 2010/07/14 14:30:00 liuxw+00139685 [AZ1D02219] */
    /* 将profile和level的无效值修改为-1 */
    avctx->iActualProfile = -1;
    avctx->iActualLevel   = -1;

    avctx->error_recognition = 1;
    avctx->error_concealment = 3;

    /* 恢复之前保存的变量 */
    avctx->codec		= codec;
    avctx->priv_data	= priv_data;
    avctx->av_class		= av_class;
    avctx->codec_id		= codec->id;
    avctx->frame_number = 0;
    avctx->thread_count = 1;

    /*guoshan+00101841 20100813 问题单：[AZ1D02261]*/
    /*reset之后恢复之前的dsp_mask状态*/
    avctx->dsp_mask                    = dsp_mask;
    avctx->eCodecType                  = eCodecType;
    avctx->internal_buffer             = internal_buffer;
    //	avctx->internal_buffer_count       = internal_buffer_count;
    avctx->flags					   = flags;
    avctx->flags2                      = flags2;
    avctx->pucBase                     = pucBase;
    avctx->iBufLength                  = iBufLength;
    avctx->uiCurPos                    = uiCurPos;

    /* 恢复静态参数（AVCodecContext结构体变量）*/
    avctx->usSourceWidth		= usSourceWidth;
    avctx->usSourceHeight		= usSourceHeight;
    avctx->uiRefFrame			= uiRefFrame;
    avctx->uiProfileIdc			= uiProfileIdc;
    avctx->uiLevelIdc			= uiLevelIdc;
    avctx->eColorSpaceType		= eColorSpaceType;
    avctx->bForceOutYUV420		= bForceOutYUV420;
    avctx->bDeblockFilter		= bDeblockFilter;
    avctx->eFrameSkipMode		= eFrameSkipMode;
    avctx->iSourceChromaFormat  = iChromaFormat;
    avctx->uiBufNum             = uiBufNum;
#if 0
    /* 初始化解码通道变量的某些状态或统计值 */
    avctx->pix_fmt			   = PIX_FMT_NONE;
    avctx->coded_width		   = 0;
    avctx->coded_height		   = 0;
    avctx->width			   = 0;
    avctx->height			   = 0;
    avctx->usActualWidth	   = 0;
    avctx->usActualHeight	   = 0;
    avctx->frame_number        = 0;
    avctx->avctx->thread_count = 1;
    avctx->slice_count		   = 0;
    avctx->slice_offset		   = 0;
    avctx->has_b_frames		   = 0;
    avctx->sample_aspect_ratio.num = 0;
    avctx->sample_aspect_ratio.den = 0;
    avctx->time_base.num		   = 0;
    avctx->time_base.den		   = 0;
    avctx->internal_buffer_count   = 0;
    avctx->profile				   = FF_PROFILE_UNKNOWN;
    avctx->level				   = FF_LEVEL_UNKNOWN;
    avctx->refs					   = 0;

    if (NULL != avctx->coded_frame)
    {
        memset(avctx->coded_frame，0, sizeof(AAVFrame));
    }

    /* 解码器状态信息复位 */
    avctx->eCodecStatus		 = 0;
    avctx->iErrorCode		 = 0;
    avctx->uiDecIFrames		 = 0;
    avctx->uiDecPFrames		 = 0;
    avctx->uiDecBFrames		 = 0;
    avctx->uiDisplayedFrames = 0;
    avctx->uiFreeBufNum		 = 0;
    avctx->uiInUsedBufNum	 = 0;

    /* 码流信息复位 */
    //	avctx->eCodecType		 = 0;
    avctx->eColorSpaceType	 = 0;
    avctx->eContentType		 = 0;
    avctx->uiBitRate		 = 0;
    avctx->uiFrameRate		 = 0;
    avctx->uiProfileIdc		 = 0;
    avctx->uiLevelIdc		 = 0;
    avctx->uiRefFrame		 = 0;
    avctx->uiVbvBufSize      = 0;

    /* 清空序列头和码流头指针 */
    for (i = 0; i < MAZX_EXTRADATA_NUM; i++)
    {
        avctx->extradata[i]		 = NULL;
        avctx->extradata_size[i] = 0;
    }

    avctx->extradata_num = 0;
#endif

    return iRet;
}


/*
int attribute_align_arg avcodec_encode_audio(AVCodecContext *avctx, uint8_t *buf, int buf_size,
                         const short *samples)
{
    if(buf_size < FF_MIN_BUFFER_SIZE && 0){
        av_log(avctx, AV_LOG_ERROR, "buffer smaller than minimum size\n");
        return -1;
    }
    if((avctx->codec->capabilities & CODEC_CAP_DELAY) || samples){
        int ret = avctx->codec->encode(avctx, buf, buf_size, samples);
        avctx->frame_number++;
        return ret;
    }else
        return 0;
}
*/
/*
int attribute_align_arg avcodec_encode_video(AVCodecContext *avctx, uint8_t *buf, int buf_size,
                         const AVFrame *pict)
{
    if(buf_size < FF_MIN_BUFFER_SIZE){
        av_log(avctx, AV_LOG_ERROR, "buffer smaller than minimum size\n");
        return -1;
    }
    if(avcodec_check_dimensions(avctx,avctx->width,avctx->height))
        return -1;
    if((avctx->codec->capabilities & CODEC_CAP_DELAY) || pict){
        int ret = avctx->codec->encode(avctx, buf, buf_size, pict);
        avctx->frame_number++;
        emms_c(); //needed to avoid an emms_c() call before every return;

        return ret;
    }else
        return 0;
} */

/*
int avcodec_encode_subtitle(AVCodecContext *avctx, uint8_t *buf, int buf_size,
                            const AVSubtitle *sub)
{
    int ret;
    if(sub->start_display_time) {
        av_log(avctx, AV_LOG_ERROR, "start_display_time must be 0.\n");
        return -1;
    }
    if(sub->num_rects == 0 || !sub->rects)
        return -1;
    ret = avctx->codec->encode(avctx, buf, buf_size, sub);
    avctx->frame_number++;
    return ret;
} */

int attribute_align_arg avcodec_decode_video(AVCodecContext *avctx, AVFrame *picture,
        int *got_picture_ptr,
        const uint8_t *buf, int buf_size)
{
    int ret;

    *got_picture_ptr = 0;

    if ((avctx->coded_width || avctx->coded_height) && avcodec_check_dimensions(avctx, avctx->coded_width, avctx->coded_height))
    { return -1; }

    if ((avctx->codec->capabilities & CODEC_CAP_DELAY) || buf_size)
    {
        ret = avctx->codec->decode(avctx, picture, got_picture_ptr,
                                   buf, buf_size);

        emms_c(); //needed to avoid an emms_c() call before every return;

        if (*got_picture_ptr)
        { avctx->frame_number++; }
    }
    else
    { ret = 0; }

    return ret;
}

int attribute_align_arg avcodec_decode_audio2(AVCodecContext *avctx, int16_t *samples,
        int *frame_size_ptr,
        const uint8_t *buf, int buf_size)
{
    int ret;

    if ((avctx->codec->capabilities & CODEC_CAP_DELAY) || buf_size)
    {
        //FIXME remove the check below _after_ ensuring that all audio check that the available space is enough
        if (*frame_size_ptr < AVCODEC_MAX_AUDIO_FRAME_SIZE)
        {
            av_log(avctx, AV_LOG_ERROR, "buffer smaller than AVCODEC_MAX_AUDIO_FRAME_SIZE\n");
            return -1;
        }

        if (*frame_size_ptr < (int)(FF_MIN_BUFFER_SIZE) ||
            *frame_size_ptr < avctx->channels * avctx->frame_size * (int)sizeof(int16_t))
        {
            av_log(avctx, AV_LOG_ERROR, "buffer %d too small\n", *frame_size_ptr);
            return -1;
        }

        ret = avctx->codec->decode(avctx, samples, frame_size_ptr,
                                   buf, buf_size);
        avctx->frame_number++;
    }
    else
    {
        ret = 0;
        *frame_size_ptr = 0;
    }

    return ret;
}

int avcodec_decode_subtitle(AVCodecContext *avctx, AVSubtitle *sub,
                            int *got_sub_ptr,
                            const uint8_t *buf, int buf_size)
{
    int ret;

    *got_sub_ptr = 0;
    ret = avctx->codec->decode(avctx, sub, got_sub_ptr,
                               buf, buf_size);

    if (*got_sub_ptr)
    { avctx->frame_number++; }

    return ret;
}

int avcodec_close(AVCodecContext *avctx)
{

    entangled_thread_counter++;

    if (entangled_thread_counter != 1)
    {
        av_log(avctx, AV_LOG_ERROR, "insufficient thread locking around avcodec_open/close()\n");
        entangled_thread_counter--;
        return -1;
    }

    if (HAVE_THREADS && avctx->thread_opaque)
    { avcodec_thread_free(avctx); }

    /* 2010/03/26 16:30:00 liuxw+00139685 */
    /* 多增加了一个判断条件avctx->codec */
    if (avctx->codec && avctx->codec->close)
    { avctx->codec->close(avctx); }

    avcodec_default_free_buffers(avctx);
    av_freep( &avctx->priv_data);
    avctx->codec = NULL;
    avctx->extradata_num = 0;
    entangled_thread_counter--;
    return 0;
}

AVCodec *avcodec_find_encoder(enum CodecID id)
{
    /*
        AVCodec *p;
        p = first_avcodec;
        while (p) {
            if (p->encode != NULL && p->id == id)
                return p;
            p = p->next;
        }*/

    return NULL;
}

AVCodec *avcodec_find_encoder_by_name(const char *name)
{

    return NULL;
}


AVCodec *avcodec_find_decoder(enum CodecID id)
{
    AVCodec *p;
    p = first_avcodec;

    while (p)
    {
        if (p->decode != NULL && p->id == id)
        { return p; }

        p = p->next;
    }

    return NULL;
}

AVCodec *avcodec_find_decoder_by_name(const char *name)
{
    AVCodec *p;

    if (!name)
    { return NULL; }

    p = first_avcodec;

    while (p)
    {
        if (p->decode != NULL && strncmp(name, p->name, Max_STRNCMP_LEN) == 0)
        { return p; }

        p = p->next;
    }

    return NULL;
}

#if 0
void avcodec_string(char *buf, int buf_size, AVCodecContext *enc, int encode)
{
    const char *codec_name;
    AVCodec *p;
    char buf1[32];
    int bitrate;
    AVRational display_aspect_ratio;
    display_aspect_ratio.den = 0;
    display_aspect_ratio.num = 0;

    if (encode)
    { p = avcodec_find_encoder(enc->codec_id); }
    else
    { p = avcodec_find_decoder(enc->codec_id); }

    if (p)
    {
        codec_name = p->name;
    }
    else if (enc->codec_id == CODEC_ID_MPEG2TS)
    {
        /* fake mpeg2 transport stream codec (currently not
           registered) */
        codec_name = "mpeg2ts";
    }
    else if (enc->codec_name[0] != '\0')
    {
        codec_name = enc->codec_name;
    }
    else
    {
        /* output avi tags */
        if (   isprint(enc->codec_tag & 0xFF) && isprint((enc->codec_tag >> 8) & 0xFF)
               && isprint((enc->codec_tag >> 16) & 0xFF) && isprint((enc->codec_tag >> 24) & 0xFF))
        {
            snprintf(buf1, 32, "%c%c%c%c / 0x%04X",
                     enc->codec_tag & 0xff,
                     (enc->codec_tag >> 8) & 0xff,
                     (enc->codec_tag >> 16) & 0xff,
                     (enc->codec_tag >> 24) & 0xff,
                     enc->codec_tag);
        }
        else
        {
            snprintf(buf1, 32, "0x%04x", enc->codec_tag);
        }

        codec_name = buf1;
    }

    switch (enc->codec_type)
    {
        case CODEC_TYPE_VIDEO:
            snprintf(buf, buf_size,
                     "Video: %s%s",
                     codec_name, enc->mb_decision ? " (hq)" : "");

            if (enc->pix_fmt != PIX_FMT_NONE)
            {
                snprintf(buf + strlen(buf), buf_size - strlen(buf),
                         ", %s",
                         avcodec_get_pix_fmt_name(enc->pix_fmt));
            }

            if (enc->width)
            {
                snprintf(buf + strlen(buf), buf_size - strlen(buf),
                         ", %dx%d",
                         enc->width, enc->height);

                if (enc->sample_aspect_ratio.num)
                {
                    /* av_reduce(&display_aspect_ratio.num, &display_aspect_ratio.den,
                               enc->width*enc->sample_aspect_ratio.num,
                               enc->height*enc->sample_aspect_ratio.den,
                               1024*1024)*/;
                    snprintf(buf + strlen(buf), buf_size - strlen(buf),
                             " [PAR %d:%d DAR %d:%d]",
                             enc->sample_aspect_ratio.num, enc->sample_aspect_ratio.den,
                             display_aspect_ratio.num, display_aspect_ratio.den);
                }

                if (av_log_get_level() >= AV_LOG_DEBUG)
                {
#if 0
                    int g = (int)av_gcd(enc->time_base.num, enc->time_base.den);
                    snprintf(buf + strlen(buf), buf_size - strlen(buf),
                             ", %d/%d",
                             enc->time_base.num / g, enc->time_base.den / g);
#endif
                    ;
                }
            }

            if (encode)
            {
                snprintf(buf + strlen(buf), buf_size - strlen(buf),
                         ", q=%d-%d", enc->qmin, enc->qmax);
            }

            bitrate = enc->bit_rate;
            break;

        case CODEC_TYPE_AUDIO:
            snprintf(buf, buf_size,
                     "Audio: %s",
                     codec_name);

            if (enc->sample_rate)
            {
                snprintf(buf + strlen(buf), buf_size - strlen(buf),
                         ", %d Hz", enc->sample_rate);
            }

            av_strlcat(buf, ", ", buf_size);

            //   avcodec_get_channel_layout_string(buf + strlen(buf), buf_size - strlen(buf), enc->channels, enc->channel_layout);
            if (enc->sample_fmt != SAMPLE_FMT_NONE)
            {
                /*      snprintf(buf + strlen(buf), buf_size - strlen(buf),
                               ", %s", avcodec_get_sample_fmt_name(enc->sample_fmt)); */  /* removed by lxw ; do not need audio */
            }

            /* for PCM codecs, compute bitrate directly */
            switch (enc->codec_id)
            {
                case CODEC_ID_PCM_F64BE:
                case CODEC_ID_PCM_F64LE:
                    bitrate = enc->sample_rate * enc->channels * 64;
                    break;

                case CODEC_ID_PCM_S32LE:
                case CODEC_ID_PCM_S32BE:
                case CODEC_ID_PCM_U32LE:
                case CODEC_ID_PCM_U32BE:
                case CODEC_ID_PCM_F32BE:
                case CODEC_ID_PCM_F32LE:
                    bitrate = enc->sample_rate * enc->channels * 32;
                    break;

                case CODEC_ID_PCM_S24LE:
                case CODEC_ID_PCM_S24BE:
                case CODEC_ID_PCM_U24LE:
                case CODEC_ID_PCM_U24BE:
                case CODEC_ID_PCM_S24DAUD:
                    bitrate = enc->sample_rate * enc->channels * 24;
                    break;

                case CODEC_ID_PCM_S16LE:
                case CODEC_ID_PCM_S16BE:
                case CODEC_ID_PCM_S16LE_PLANAR:
                case CODEC_ID_PCM_U16LE:
                case CODEC_ID_PCM_U16BE:
                    bitrate = enc->sample_rate * enc->channels * 16;
                    break;

                case CODEC_ID_PCM_S8:
                case CODEC_ID_PCM_U8:
                case CODEC_ID_PCM_ALAW:
                case CODEC_ID_PCM_MULAW:
                case CODEC_ID_PCM_ZORK:
                    bitrate = enc->sample_rate * enc->channels * 8;
                    break;

                default:
                    bitrate = enc->bit_rate;
                    break;
            }

            break;

        case CODEC_TYPE_DATA:
            snprintf(buf, buf_size, "Data: %s", codec_name);
            bitrate = enc->bit_rate;
            break;

        case CODEC_TYPE_SUBTITLE:
            snprintf(buf, buf_size, "Subtitle: %s", codec_name);
            bitrate = enc->bit_rate;
            break;

        case CODEC_TYPE_ATTACHMENT:
            snprintf(buf, buf_size, "Attachment: %s", codec_name);
            bitrate = enc->bit_rate;
            break;

        default:
            snprintf(buf, buf_size, "Invalid Codec type %d", enc->codec_type);
            return;
    }

    if (encode)
    {
        if (enc->flags & CODEC_FLAG_PASS1)
            snprintf(buf + strlen(buf), buf_size - strlen(buf),
                     ", pass 1");

        if (enc->flags & CODEC_FLAG_PASS2)
            snprintf(buf + strlen(buf), buf_size - strlen(buf),
                     ", pass 2");
    }

    if (bitrate != 0)
    {
        snprintf(buf + strlen(buf), buf_size - strlen(buf),
                 ", %d kb/s", bitrate / 1000);
    }
}
#endif

unsigned avcodec_version( void )
{
    return LIBAVCODEC_VERSION_INT;
}

void avcodec_init(void)
{
    static int initialized = 0;

    if (initialized != 0)
    { return; }

    initialized = 1;
#if __STDC_VERSION__ < 199901L
    /* init pixel format info array in win32 */
    avpicture_init_pixfmtinfo();  //add by lxw
#endif
    /* init some tables like square table,inv_zigzag_direct16 and crop tables */
    dsputil_static_init();
}

void avcodec_flush_buffers(AVCodecContext *avctx)
{
    if (avctx->codec->flush)
    { avctx->codec->flush(avctx); }
}

void avcodec_default_free_buffers(AVCodecContext *s)
{
    int i, j;

    if (s->internal_buffer == NULL) { return; }

    for (i = 0; i < (int)(s->uiBufNum); i++)
    {
        InternalBuffer *buf = &((InternalBuffer *)s->internal_buffer)[i];

        for (j = 0; j < 4; j++)
        {
            av_free_hw(buf->base[j]);
            buf->base[j] = NULL;
            buf->data[j] = NULL;
        }
    }

    av_freep(&s->internal_buffer);

    s->internal_buffer_count = 0;
}

char av_get_pict_type_char(int pict_type)
{
    switch (pict_type)
    {
        case FF_I_TYPE:
            return 'I';

        case FF_P_TYPE:
            return 'P';

        case FF_B_TYPE:
            return 'B';

        case FF_S_TYPE:
            return 'S';

        case FF_SI_TYPE:
            return 'i';

        case FF_SP_TYPE:
            return 'p';

        case FF_BI_TYPE:
            return 'b';

        default:
            return '?';
    }
}

int av_get_bits_per_sample(enum CodecID codec_id)
{
    switch (codec_id)
    {
        case CODEC_ID_ADPCM_SBPRO_2:
                return 2;

        case CODEC_ID_ADPCM_SBPRO_3:
            return 3;

        case CODEC_ID_ADPCM_SBPRO_4:
        case CODEC_ID_ADPCM_CT:
            return 4;

        case CODEC_ID_PCM_ALAW:
        case CODEC_ID_PCM_MULAW:
        case CODEC_ID_PCM_S8:
        case CODEC_ID_PCM_U8:
        case CODEC_ID_PCM_ZORK:
            return 8;

        case CODEC_ID_PCM_S16BE:
        case CODEC_ID_PCM_S16LE:
        case CODEC_ID_PCM_S16LE_PLANAR:
        case CODEC_ID_PCM_U16BE:
        case CODEC_ID_PCM_U16LE:
            return 16;

        case CODEC_ID_PCM_S24DAUD:
        case CODEC_ID_PCM_S24BE:
        case CODEC_ID_PCM_S24LE:
        case CODEC_ID_PCM_U24BE:
        case CODEC_ID_PCM_U24LE:
            return 24;

        case CODEC_ID_PCM_S32BE:
        case CODEC_ID_PCM_S32LE:
        case CODEC_ID_PCM_U32BE:
        case CODEC_ID_PCM_U32LE:
        case CODEC_ID_PCM_F32BE:
        case CODEC_ID_PCM_F32LE:
            return 32;

        case CODEC_ID_PCM_F64BE:
        case CODEC_ID_PCM_F64LE:
            return 64;

        default:
            return 0;
    }
}

int av_get_bits_per_sample_format(enum SampleFormat sample_fmt)
{
    switch (sample_fmt)
    {
        case SAMPLE_FMT_U8:
                return 8;

        case SAMPLE_FMT_S16:
            return 16;

        case SAMPLE_FMT_S32:
        case SAMPLE_FMT_FLT:
            return 32;

        case SAMPLE_FMT_DBL:
            return 64;

        default:
            return 0;
    }
}

#if !HAVE_THREADS
int avcodec_thread_init(AVCodecContext *s, int thread_count)
{
    return -1;
}
#endif

unsigned int av_xiphlacing(unsigned char *s, unsigned int v)
{
    unsigned int n = 0;

    while (v >= 0xff)
    {
        *s++ = 0xff;
        v -= 0xff;
        n++;
    }

    *s = v;
    n++;
    return n;
}

/* Wrapper to work around the lack of mkstemp() on mingw/cygin.
 * Also, tries to create file in /tmp first, if possible.
 * *prefix can be a character constant; *filename will be allocated internally.
 * Returns file descriptor of opened file (or -1 on error)
 * and opened file name in **filename. */
#if 0
#ifndef __KERNEL__
int av_tempfile(char *prefix, char **filename)
{
    int fd = -1;
#if !HAVE_MKSTEMP
    *filename = tempnam(".", prefix);
#else
    size_t len = strlen(prefix) + 12; /* room for "/tmp/" and "XXXXXX\0" */
    *filename = av_malloc_hw(len);
#endif

    /* -----common section-----*/
    if (*filename == NULL)
    {
        av_log(NULL, AV_LOG_ERROR, "ff_tempfile: Cannot allocate file name\n");
        return -1;
    }

#if !HAVE_MKSTEMP
    fd = open(*filename, O_RDWR | O_BINARY | O_CREAT, 0444);
#else
    snprintf(*filename, len, "/tmp/%sXXXXXX", prefix);
    fd = mkstemp(*filename);

    if (fd < 0)
    {
        snprintf(*filename, len, "./%sXXXXXX", prefix);
        fd = mkstemp(*filename);
    }

#endif

    /* -----common section-----*/
    if (fd < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "ff_tempfile: Cannot open temporary file %s\n", *filename);
        return -1;
    }

    return fd; /* success */
}
#endif
#endif

typedef struct
{
    const char *abbr;
    int width, height;
} VideoFrameSizeAbbr;

typedef struct
{
    const char *abbr;
    int rate_num, rate_den;
} VideoFrameRateAbbr;

static const VideoFrameSizeAbbr video_frame_size_abbrs[] =
{
    { "ntsc",      720, 480 },
    { "pal",       720, 576 },
    { "qntsc",     352, 240 }, /* VCD compliant NTSC */
    { "qpal",      352, 288 }, /* VCD compliant PAL */
    { "sntsc",     640, 480 }, /* square pixel NTSC */
    { "spal",      768, 576 }, /* square pixel PAL */
    { "film",      352, 240 },
    { "ntsc-film", 352, 240 },
    { "sqcif",     128,  96 },
    { "qcif",      176, 144 },
    { "cif",       352, 288 },
    { "4cif",      704, 576 },
    { "qqvga",     160, 120 },
    { "qvga",      320, 240 },
    { "vga",       640, 480 },
    { "svga",      800, 600 },
    { "xga",      1024, 768 },
    { "uxga",     1600, 1200 },
    { "qxga",     2048, 1536 },
    { "sxga",     1280, 1024 },
    { "qsxga",    2560, 2048 },
    { "hsxga",    5120, 4096 },
    { "wvga",      852, 480 },
    { "wxga",     1366, 768 },
    { "wsxga",    1600, 1024 },
    { "wuxga",    1920, 1200 },
    { "woxga",    2560, 1600 },
    { "wqsxga",   3200, 2048 },
    { "wquxga",   3840, 2400 },
    { "whsxga",   6400, 4096 },
    { "whuxga",   7680, 4800 },
    { "cga",       320, 200 },
    { "ega",       640, 350 },
    { "hd480",     852, 480 },
    { "hd720",    1280, 720 },
    { "hd1080",   1920, 1080 },
};

static const VideoFrameRateAbbr video_frame_rate_abbrs[] =
{
    { "ntsc",      30000, 1001 },
    { "pal",          25,    1 },
    { "qntsc",     30000, 1001 }, /* VCD compliant NTSC */
    { "qpal",         25,    1 }, /* VCD compliant PAL */
    { "sntsc",     30000, 1001 }, /* square pixel NTSC */
    { "spal",         25,    1 }, /* square pixel PAL */
    { "film",         24,    1 },
    { "ntsc-film", 24000, 1001 },
};

int av_parse_video_frame_size(int *width_ptr, int *height_ptr, const char *str)
{
    int i;
    int n = FF_ARRAY_ELEMS(video_frame_size_abbrs);
    char *p;
    int frame_width = 0, frame_height = 0;

    for (i = 0; i < n; i++)
    {
        if (!strncmp(video_frame_size_abbrs[i].abbr, str, Max_STRNCMP_LEN))
        {
            frame_width = video_frame_size_abbrs[i].width;
            frame_height = video_frame_size_abbrs[i].height;
            break;
        }
    }

    if (i == n)
    {
        p = (char *)str;
        frame_width = strtol(p, &p, 10);

        if (*p)
        { p++; }

        frame_height = strtol(p, &p, 10);
    }

    if (frame_width <= 0 || frame_height <= 0)
    { return -1; }

    *width_ptr = frame_width;
    *height_ptr = frame_height;
    return 0;
}

int av_parse_video_frame_rate(AVRational *frame_rate, const char *arg)
{
    int i;
    int n = FF_ARRAY_ELEMS(video_frame_rate_abbrs);
    char *cp;

    /* First, we check our abbreviation table */
    for (i = 0; i < n; ++i)
        if (!strncmp(video_frame_rate_abbrs[i].abbr, arg, Max_STRNCMP_LEN))
        {
            frame_rate->num = video_frame_rate_abbrs[i].rate_num;
            frame_rate->den = video_frame_rate_abbrs[i].rate_den;
            return 0;
        }

    /* Then, we try to parse it as fraction */
    cp = strchr(arg, '/');

    if (!cp)
    { cp = strchr(arg, ':'); }

    if (cp)
    {
        char *cpp;
        frame_rate->num = strtol(arg, &cpp, 10);

        if (cpp != arg || cpp == cp)
        { frame_rate->den = strtol(cp + 1, &cpp, 10); }
        else
        { frame_rate->num = 0; }
    }
    else
    {
        /* Finally we give up and parse it as double */
        AVRational time_base = {0, 1}; //av_d2q(strtod(arg, 0), 1001000);
        frame_rate->den = time_base.den;
        frame_rate->num = time_base.num;
    }

    if (!frame_rate->num || !frame_rate->den)
    { return -1; }
    else
    { return 0; }
}

void ff_log_ask_for_sample(void *avc, const char *msg)
{
    if (msg)
    { av_log(avc, AV_LOG_WARNING, "%s ", msg); }

    av_log(avc, AV_LOG_WARNING, "If you want to help, upload a sample "
           "of this file to ftp://upload.ffmpeg.org/MPlayer/incoming/ "
           "and contact the ffmpeg-devel mailing list.\n");
}

void ff_log_missing_feature(void *avc, const char *feature, int want_sample)
{
    av_log(avc, AV_LOG_WARNING, "%s not implemented. Update your FFmpeg "
           "version to the newest one from SVN. If the problem still "
           "occurs, it means that your file has a feature which has not "
           "been implemented.", feature);

    if (want_sample)
    { ff_log_ask_for_sample(avc, NULL); }
    else
    { av_log(avc, AV_LOG_WARNING, "\n"); }
}


static AVHWAccel *first_hwaccel = NULL;

void av_register_hwaccel(AVHWAccel *hwaccel)
{
    AVHWAccel **p = &first_hwaccel;

    while (*p)
    { p = &(*p)->next; }

    *p = hwaccel;
    hwaccel->next = NULL;
}

AVHWAccel *av_hwaccel_next(AVHWAccel *hwaccel)
{
    return hwaccel ? hwaccel->next : first_hwaccel;
}

AVHWAccel *ff_find_hwaccel(enum CodecID codec_id, enum PixelFormat pix_fmt)
{
    AVHWAccel *hwaccel = NULL;

    while ((hwaccel = av_hwaccel_next(hwaccel)))
    {
        if (   hwaccel->id      == codec_id
               && hwaccel->pix_fmt == pix_fmt)
        { return hwaccel; }
    }

    return NULL;
}

