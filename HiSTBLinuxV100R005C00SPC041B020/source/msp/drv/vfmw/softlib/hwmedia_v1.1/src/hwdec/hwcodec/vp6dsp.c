/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpBWPlpb76M4lvoojttTw05AfDyj0e7jwqOFHFuh
IwLDbLE1jjAnMC5tOUI11f8Ry1PQCkFIMyHyaAm8R5pNY6n7m2LOuKZZjNXJyp/+53J7x7EU
SGCIxLPIKJxXiYU5GoywlBEyaoxYJu6l59le+ugZA+y32Hd5J2soQ/Ras1/KFA==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/**
 * @file libavcodec/vp6dsp.c
 * VP6 DSP-oriented functions
 *
 * Copyright (C) 2006  Aurelien Jacobs <aurel@gnuage.org>
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

#include "common.h"
#include "dsputil.h"


void ff_vp6_filter_diag4_c(uint8_t *dst, uint8_t *src, int stride,
                           const int16_t *h_weights, const int16_t *v_weights)
{
    int x, y;
    int tmp[8 * 11];
    int *t = tmp;

    src -= stride;

    for (y = 0; y < 11; y++)
    {
        for (x = 0; x < 8; x++)
        {
            t[x] = av_clip_uint8((  src[x - 1] * h_weights[0]
                                    + src[x  ] * h_weights[1]
                                    + src[x + 1] * h_weights[2]
                                    + src[x + 2] * h_weights[3] + 64) >> 7);
        }

        src += stride;
        t += 8;
    }

    t = tmp + 8;

    for (y = 0; y < 8; y++)
    {
        for (x = 0; x < 8; x++)
        {
            dst[x] = av_clip_uint8((  t[x - 8 ] * v_weights[0]
                                      + t[x   ] * v_weights[1]
                                      + t[x + 8 ] * v_weights[2]
                                      + t[x + 16] * v_weights[3] + 64) >> 7);
        }

        dst += stride;
        t += 8;
    }
}

/*vp6 4³éÍ·²åÖµÂË²¨º¯Êý x00141957 2010 1127*/
void vp6_filter_hv4(uint8_t *dst, uint8_t *src, int stride,						   int delta, const int16_t *weights)
{
    int x, y;

    for (y = 0; y < 8; y++)
    {
        for (x = 0; x < 8; x++)
        {
            dst[x] = av_clip_uint8((  src[x - delta  ] * weights[0]			+ src[x        ] * weights[1]			+ src[x + delta  ] * weights[2]			+ src[x + 2 * delta] * weights[3] + 64) >> 7);
        }

        src += stride;
        dst += stride;
    }
}


int32_t vp6_adjust( int32_t FiltVal, uint32_t  FLimit)
{
    int32_t Clamp;
    int32_t FiltSign;
    int32_t NewSign;

    Clamp = 2 * FLimit;

    // Next 3 lines are fast way to find abs...
    FiltSign = (FiltVal >> 31);         // Sign extension makes FiltSign all 0's or all 1's
    FiltVal ^= FiltSign;                // FiltVal is then 1's complement of value if -ve
    FiltVal -= FiltSign;                // Filtval = abs Filtval

    FiltVal *= (FiltVal < Clamp);       // clamp filter value to 2 times limit

    FiltVal -= FLimit;                  // subtract limit value

    // Next 3 lines are fast way to find abs...
    NewSign = (FiltVal >> 31);          // Sign extension makes NewSign all 0's or all 1's
    FiltVal ^= NewSign;                 // FiltVal is then 1's complement of value if -ve
    FiltVal -= NewSign;                 // FiltVal = abs FiltVal

    FiltVal = FLimit - FiltVal;         // flimit - abs (filtVal - flimit)

    FiltVal += FiltSign;                // convert back to signed value
    FiltVal ^= FiltSign;

    return FiltVal;
}

void vp56_edge_filter_c(uint8_t *yuv,
                        int pix_inc, int line_inc, int t)
{
    int pix2_inc = 2 * pix_inc;
    int i, v;

    for (i = 0; i < 12; i++)
    {
        v = (yuv[-pix2_inc] + 3 * (yuv[0] - yuv[-pix_inc]) - yuv[pix_inc] + 4) >> 3;
        v = vp6_adjust(v, t);
        yuv[-pix_inc] = av_clip_uint8(yuv[-pix_inc] + v);
        yuv[0] = av_clip_uint8(yuv[0] - v);
        yuv += line_inc;
    }
}

void vp6_filter_diag2_h_c(uint8_t *dst/*align 8*/, uint8_t *src/*align 1*/, int stride, uint16_t *filter)
{
    const int A = filter[0];
    const int E = filter[1];
    int i;
    int step = 1;

    for (i = 0; i < 8; i++)
    {
        dst[0] = ((A * src[0] + E * src[step + 0]) + 4 ) >> 3;
        dst[1] = ((A * src[1] + E * src[step + 1]) + 4) >> 3;
        dst[2] = ((A * src[2] + E * src[step + 2]) + 4) >> 3;
        dst[3] = ((A * src[3] + E * src[step + 3]) + 4) >> 3;
        dst[4] = ((A * src[4] + E * src[step + 4]) + 4 ) >> 3;
        dst[5] = ((A * src[5] + E * src[step + 5]) + 4) >> 3;
        dst[6] = ((A * src[6] + E * src[step + 6]) + 4) >> 3;
        dst[7] = ((A * src[7] + E * src[step + 7]) + 4) >> 3;
        dst += stride;
        src += stride;
    }
}

void vp6_filter_diag2_v_c(uint8_t *dst/*align 8*/, uint8_t *src/*align 1*/, int stride, uint16_t *filter)
{
    const int A = filter[0];
    const int E = filter[1];
    int i;
    int step = stride;

    for (i = 0; i < 8; i++)
    {
        dst[0] = ((A * src[0] + E * src[step + 0]) + 4 ) >> 3;
        dst[1] = ((A * src[1] + E * src[step + 1]) + 4) >> 3;
        dst[2] = ((A * src[2] + E * src[step + 2]) + 4) >> 3;
        dst[3] = ((A * src[3] + E * src[step + 3]) + 4) >> 3;
        dst[4] = ((A * src[4] + E * src[step + 4]) + 4) >> 3;
        dst[5] = ((A * src[5] + E * src[step + 5]) + 4) >> 3;
        dst[6] = ((A * src[6] + E * src[step + 6]) + 4) >> 3;
        dst[7] = ((A * src[7] + E * src[step + 7]) + 4) >> 3;
        dst += stride;
        src += stride;
    }
}

void vp6_filter_diag2_c(uint8_t *dst, uint8_t *src, int stride, uint16_t *h_weight, uint16_t *v_weight)
{
    int32_t temp[8 * 9];
    int A = h_weight[0];
    int E = h_weight[1];
    int i;
    int step = 1;
    int32_t *FData = temp;

    for (i = 0; i < 9; i++)
    {
        FData[0] = ((A * src[0] + E * src[step + 0]) + 4) >> 3;
        FData[1] = ((A * src[1] + E * src[step + 1]) + 4) >> 3;
        FData[2] = ((A * src[2] + E * src[step + 2]) + 4) >> 3;
        FData[3] = ((A * src[3] + E * src[step + 3]) + 4) >> 3;
        FData[4] = ((A * src[4] + E * src[step + 4]) + 4) >> 3;
        FData[5] = ((A * src[5] + E * src[step + 5]) + 4) >> 3;
        FData[6] = ((A * src[6] + E * src[step + 6]) + 4) >> 3;
        FData[7] = ((A * src[7] + E * src[step + 7]) + 4) >> 3;
        FData += 8;
        src += stride;
    }

    A = v_weight[0];
    E = v_weight[1];

    step = 8;
    FData = temp;

    for (i = 0; i < 8; i++)
    {
        dst[0] = ((A * FData[0] + E * FData[step + 0]) + 4) >> 3;
        dst[1] = ((A * FData[1] + E * FData[step + 1]) + 4) >> 3;
        dst[2] = ((A * FData[2] + E * FData[step + 2]) + 4) >> 3;
        dst[3] = ((A * FData[3] + E * FData[step + 3]) + 4) >> 3;
        dst[4] = ((A * FData[4] + E * FData[step + 4]) + 4) >> 3;
        dst[5] = ((A * FData[5] + E * FData[step + 5]) + 4) >> 3;
        dst[6] = ((A * FData[6] + E * FData[step + 6]) + 4) >> 3;
        dst[7] = ((A * FData[7] + E * FData[step + 7]) + 4) >> 3;
        dst += stride;
        FData += 8;
    }

}