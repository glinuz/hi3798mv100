
/*
 * MPEG1/2 common code
 * Copyright (c) 2007 Aurelien Jacobs <aurel@gnuage.org>
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

#ifndef AVCODEC_MPEG12_H
#define AVCODEC_MPEG12_H

#include "mpegvideo.h"

/*从imedia_common.h中移过来 与mpeg2特定相关宏 xiongfei20100304*/
/*与mpeg2相关 数据结构  xiongfei2010211*/
/* Start codes. */
#define SEQ_END_CODE            0x000001b7
#define SEQ_START_CODE          0x000001b3
#define GOP_START_CODE          0x000001b8
#define PICTURE_START_CODE      0x00000100
#define SLICE_MIN_START_CODE    0x00000101
#define SLICE_MAX_START_CODE    0x000001af
#define EXT_START_CODE          0x000001b5
#define USER_START_CODE         0x000001b2

/*将Mpeg1Context从mpeg12.c中移到此处 xiongfei20100221*/
typedef struct Mpeg1Context
{
    MpegEncContext mpeg_enc_ctx;
    int mpeg_enc_ctx_allocated; /* true if decoding context allocated */
    int repeat_field; /* true if we must repeat the field */
    AVPanScan pan_scan; /** some temporary storage for the panscan */
    int slice_count;
    int swap_uv;//indicate VCR2
    int save_aspect_info;
    int save_width, save_height;
    AVRational frame_rate_ext;       ///< MPEG-2 specific framerate modificator

} Mpeg1Context;

#define DC_VLC_BITS 9
#define TEX_VLC_BITS 9

static VLC dc_lum_vlc;
static VLC dc_chroma_vlc;

extern uint8_t ff_mpeg12_static_rl_table_store[2][2][2 * MAX_RUN + MAX_LEVEL + 3];

void ff_mpeg12_common_init(MpegEncContext *s);
void ff_mpeg12_init_vlcs(void);

/* 2010/03/11 14:05:00 liuxw+00139685 */
/* 增加函数的声明 */
int mpeg1_decode_sequence(AVCodecContext *avctx, const uint8_t *buf, int buf_size);
void mpeg_decode_extension(AVCodecContext *avctx, const uint8_t *buf, int buf_size);
void mpeg_decode_user_data(AVCodecContext *avctx, const uint8_t *buf, int buf_size);

/*x00141957 20100726添加函数用来计算像素宽高比*/
void mpeg12_get_sar(MpegEncContext *s2, uint16_t *sample_height, uint16_t *sample_width);

static inline int decode_dc(GetBitContext *gb, int component)
{
    int code, diff;

    if (component == 0)
    {
        code = get_vlc2(gb, dc_lum_vlc.table, DC_VLC_BITS, 2);
    }
    else
    {
        code = get_vlc2(gb, dc_chroma_vlc.table, DC_VLC_BITS, 2);
    }

    if (code < 0)
    {
        //      av_log(NULL, AV_LOG_ERROR, "invalid dc code at\n");
        av_log(NULL, AV_LOG_WARNING, "invalid dc code at\n");
        return 0xffff;
    }

    if (code == 0)
    {
        diff = 0;
    }
    else
    {
        diff = get_xbits(gb, code);
    }

    return diff;
}

extern /*inline*/ int ff_mpeg1_decode_block_intra(MpegEncContext *s, DCTELEM *block, int n);

#endif /* AVCODEC_MPEG12_H */
