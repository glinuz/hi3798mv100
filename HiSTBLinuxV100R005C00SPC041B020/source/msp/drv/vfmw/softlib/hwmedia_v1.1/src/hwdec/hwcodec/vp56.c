/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpBWPlpb76M4lvoojttTw05AfDyj0e7jwqOFHFuh
IwLDbLVzhJCP0udAXIhWfcIbPeLaSvpF9ESPvJFQH9XMXjq0PN74x+UyFFxgA78JFHAWCIBH
xMw/jEFG/RjQWr86QsH53dEg2HedR6B9eIyoy9NA+tKRS4EWbo+J7gTwZOCrdg==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/**
 * @file libavcodec/vp56.c
 * VP5 and VP6 compatible video decoder (common features)
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

#include "avcodec.h"
#include "bytestream.h"

#include "vp56.h"
#include "vp56data.h"
#include "vp6data.h"

uint16_t BilinearFilters[8][2] =
{
    {  0,  0 },
    {  7,  1 },
    {  6,  2 },
    {  5,  3 },
    {  4,  4 },
    {  3,  5 },
    {  2,  6 },
    {  1,  7 }
};



int ff_fill_linesize(AVPicture *picture, int pix_fmt, int width);

void vp56_init_dequant(VP56Context *s, int quantizer)
{
    s->quantizer = quantizer;
    s->dequant_dc = vp56_dc_dequant[quantizer] << 2;
    s->dequant_ac = vp56_ac_dequant[quantizer] << 2;
}

static int vp56_get_vectors_predictors(VP56Context *s, int row, int col,
                                       VP56Frame ref_frame)
{
    int nb_pred = 0;
    VP56mv vect[2] = {{0, 0}, {0, 0}};
    int pos, offset;
    VP56mv mvp;

    for (pos = 0; pos < 12; pos++)
    {
        mvp.x = col + vp56_candidate_predictor_pos[pos][0];
        mvp.y = row + vp56_candidate_predictor_pos[pos][1];

        if (mvp.x < 0 || mvp.x >= s->mb_width ||
            mvp.y < 0 || mvp.y >= s->mb_height)
        { continue; }

        offset = mvp.x + s->mb_width * mvp.y;

        if (vp56_reference_frame[s->macroblocks[offset].type] != ref_frame)
        { continue; }

        if ((s->macroblocks[offset].mv.x == vect[0].x &&
             s->macroblocks[offset].mv.y == vect[0].y) ||
            (s->macroblocks[offset].mv.x == 0 &&
             s->macroblocks[offset].mv.y == 0))
        { continue; }

        vect[nb_pred++] = s->macroblocks[offset].mv;

        if (nb_pred > 1)
        {
            nb_pred = -1;
            break;
        }

        s->vector_candidate_pos = pos;
    }

    s->vector_candidate[0] = vect[0];
    s->vector_candidate[1] = vect[1];

    return nb_pred + 1;
}

static void vp56_parse_mb_type_models(VP56Context *s)
{
    VP56RangeCoder *c = &s->c;
    VP56Model *model = s->modelp;
    int i, ctx, type;

    for (ctx = 0; ctx < 3; ctx++)
    {
        if (vp56_rac_get_prob(c, 174))
        {
            int idx = vp56_rac_gets(c, 4);
            memcpy(model->mb_types_stats[ctx],
                   vp56_pre_def_mb_type_stats[idx][ctx],
                   sizeof(model->mb_types_stats[ctx]));
        }

        if (vp56_rac_get_prob(c, 254))
        {
            for (type = 0; type < 10; type++)
            {
                for (i = 0; i < 2; i++)
                {
                    if (vp56_rac_get_prob(c, 205))
                    {
                        int delta, sign = vp56_rac_get(c);

                        delta = vp56_rac_get_tree(c, vp56_pmbtm_tree,
                                                  vp56_mb_type_model_model);

                        if (!delta)
                        { delta = 4 * vp56_rac_gets(c, 7); }

                        model->mb_types_stats[ctx][type][i] += (delta ^ -sign) + sign;
                    }
                }
            }
        }
    }

    /* compute MB type probability tables based on previous MB type */
    for (ctx = 0; ctx < 3; ctx++)
    {
        int p[10];

        for (type = 0; type < 10; type++)
        { p[type] = 100 * model->mb_types_stats[ctx][type][1]; }

        for (type = 0; type < 10; type++)
        {
            int p02, p34, p0234, p17, p56, p89, p5689, p156789;

            /* conservative MB type probability */
            model->mb_type[ctx][type][0] = 255 - (255 * model->mb_types_stats[ctx][type][0]) / (1 + model->mb_types_stats[ctx][type][0] + model->mb_types_stats[ctx][type][1]);

            p[type] = 0;    /* same MB type => weight is null */

            /* binary tree parsing probabilities */
            p02 = p[0] + p[2];
            p34 = p[3] + p[4];
            p0234 = p02 + p34;
            p17 = p[1] + p[7];
            p56 = p[5] + p[6];
            p89 = p[8] + p[9];
            p5689 = p56 + p89;
            p156789 = p17 + p5689;

            model->mb_type[ctx][type][1] = 1 + 255 * p0234 / (1 + p0234 + p156789);
            model->mb_type[ctx][type][2] = 1 + 255 * p02  / (1 + p0234);
            model->mb_type[ctx][type][3] = 1 + 255 * p17  / (1 + p156789);
            model->mb_type[ctx][type][4] = 1 + 255 * p[0] / (1 + p02);
            model->mb_type[ctx][type][5] = 1 + 255 * p[3] / (1 + p34);
            model->mb_type[ctx][type][6] = 1 + 255 * p[1] / (1 + p17);
            model->mb_type[ctx][type][7] = 1 + 255 * p56  / (1 + p5689);
            model->mb_type[ctx][type][8] = 1 + 255 * p[5] / (1 + p56);
            model->mb_type[ctx][type][9] = 1 + 255 * p[8] / (1 + p89);

            /* restore initial value */
            p[type] = 100 * model->mb_types_stats[ctx][type][1];
        }
    }
}

static VP56mb vp56_parse_mb_type(VP56Context *s,
                                 VP56mb prev_type, int ctx)
{
    uint8_t *mb_type_model = s->modelp->mb_type[ctx][prev_type];
    VP56RangeCoder *c = &s->c;

    if (vp56_rac_get_prob(c, mb_type_model[0]))
    { return prev_type; }
    else
    { return vp56_rac_get_tree(c, vp56_pmbt_tree, mb_type_model); }
}

static void vp56_decode_4mv(VP56Context *s, int row, int col)
{
    VP56mv mv = {0, 0};
    int type[4];
    int b;

    /* parse each block type */
    for (b = 0; b < 4; b++)
    {
        type[b] = vp56_rac_gets(&s->c, 2);

        if (type[b])
        { type[b]++; }  /* only returns 0, 2, 3 or 4 (all INTER_PF) */
    }

    /* get vectors */
    for (b = 0; b < 4; b++)
    {
        switch (type[b])
        {
            case VP56_MB_INTER_NOVEC_PF:
                /*s->mv[b] = (VP56mv) {0,0};*/
                /*x00141957 20100608 修改c99语法*/
                s->mv[b].x = 0;
                s->mv[b].y = 0;
                break;

            case VP56_MB_INTER_DELTA_PF:
                s->parse_vector_adjustment(s, &s->mv[b]);
                break;

            case VP56_MB_INTER_V1_PF:
                s->mv[b] = s->vector_candidate[0];
                break;

            case VP56_MB_INTER_V2_PF:
                s->mv[b] = s->vector_candidate[1];
                break;
        }

        mv.x += s->mv[b].x;
        mv.y += s->mv[b].y;
    }

    /*x00141957 2010 1120修改*/
    mv.x = (mv.x + 1 + (mv.x >= 0));
    mv.y = (mv.y + 1 + (mv.y >= 0));

    /* this is the one selected for the whole MB for prediction */
    s->macroblocks[row * s->mb_width + col].mv = s->mv[3];

    /* chroma vectors are average luma vectors */
    if (s->avctx->codec->id == CODEC_ID_VP5)
    {
        s->mv[4].x = s->mv[5].x = RSHIFT(mv.x, 2);
        s->mv[4].y = s->mv[5].y = RSHIFT(mv.y, 2);
    }
    else
    {
        /*s->mv[4] = s->mv[5] = (VP56mv) {mv.x/4, mv.y/4};*/
        /*x00141957 20100608 修改c99语法*/
        s->mv[4].x = s->mv[5].x = mv.x >> 2;
        s->mv[4].y = s->mv[5].y = mv.y >> 2;
    }
}

static VP56mb vp56_decode_mv(VP56Context *s, int row, int col)
{
    VP56mv *mv, vect = {0, 0};
    int ctx, b;

    ctx = vp56_get_vectors_predictors(s, row, col, VP56_FRAME_PREVIOUS);
    //解析宏块类型XiongFei20091207
    s->mb_type = vp56_parse_mb_type(s, s->mb_type, ctx);
    s->macroblocks[row * s->mb_width + col].type = s->mb_type;

    switch (s->mb_type)
    {
        case VP56_MB_INTER_V1_PF:
            mv = &s->vector_candidate[0];
            break;

        case VP56_MB_INTER_V2_PF:
            mv = &s->vector_candidate[1];
            break;

        case VP56_MB_INTER_V1_GF:
            vp56_get_vectors_predictors(s, row, col, VP56_FRAME_GOLDEN);
            mv = &s->vector_candidate[0];
            break;

        case VP56_MB_INTER_V2_GF:
            vp56_get_vectors_predictors(s, row, col, VP56_FRAME_GOLDEN);
            mv = &s->vector_candidate[1];
            break;

        case VP56_MB_INTER_DELTA_PF:
            s->parse_vector_adjustment(s, &vect);
            mv = &vect;
            break;

        case VP56_MB_INTER_DELTA_GF:
            vp56_get_vectors_predictors(s, row, col, VP56_FRAME_GOLDEN);
            s->parse_vector_adjustment(s, &vect);
            mv = &vect;
            break;

        case VP56_MB_INTER_4V:
            vp56_decode_4mv(s, row, col);
            return s->mb_type;

        default:
            mv = &vect;
            break;
    }

    s->macroblocks[row * s->mb_width + col].mv = *mv;

    /* same vector for all blocks */
    for (b = 0; b < 6; b++)
    { s->mv[b] = *mv; }

    return s->mb_type;
}

static void vp56_add_predictors_dc(VP56Context *s, VP56Frame ref_frame)
{
    int idx = s->scantable.permutated[0];
    int b;

    for (b = 0; b < 6; b++)
    {
        VP56RefDc *ab = &s->above_blocks[s->above_block_idx[b]];
        VP56RefDc *lb = &s->left_block[vp56_b6to4[b]];
        int count = 0;
        int dc = 0;
        int i;

        if (ref_frame == lb->ref_frame)
        {
            dc += lb->dc_coeff;
            count++;
        }

        if (ref_frame == ab->ref_frame)
        {
            dc += ab->dc_coeff;
            count++;
        }

        if (s->avctx->codec->id == CODEC_ID_VP5)
            for (i = 0; i < 2; i++)
                if (count < 2 && ref_frame == ab[-1 + 2 * i].ref_frame)
                {
                    dc += ab[-1 + 2 * i].dc_coeff;
                    count++;
                }

        if (count == 0)
        { dc = s->prev_dc[vp56_b2p[b]][ref_frame]; }
        else if (count == 2)
        { dc /= 2; }

        s->block_coeff[b][idx] += dc;
        s->prev_dc[vp56_b2p[b]][ref_frame] = s->block_coeff[b][idx];
        ab->dc_coeff = s->block_coeff[b][idx];
        ab->ref_frame = ref_frame;
        lb->dc_coeff = s->block_coeff[b][idx];
        lb->ref_frame = ref_frame;
        s->block_coeff[b][idx] *= s->dequant_dc;
    }
}

static int vp6_block_variance(uint8_t *src, int stride)
{
    int sum = 0, square_sum = 0;
    int y, x;

    for (y = 0; y < 8; y += 2)
    {
        for (x = 0; x < 8; x += 2)
        {
            sum += src[x];
            square_sum += src[x] * src[x];
        }

        src += 2 * stride;
    }

    return (16 * square_sum - sum * sum) >> 8;
}

static void vp6_filter_diag2(VP56Context *s, uint8_t *dst, uint8_t *src,
                             int stride, int h_weight, int v_weight)
{
    uint8_t *tmp = s->edge_emu_buffer + 16;
    s->dsp.put_h264_chroma_pixels_tab[0](tmp, src, stride, 9, h_weight, 0);
    s->dsp.put_h264_chroma_pixels_tab[0](dst, tmp, stride, 8, 0, v_weight);
}

static void vp56_mc(VP56Context *s, int b, int plane, uint8_t *src,
                    int stride, int x, int y)
{
    uint8_t *dst = s->framep[VP56_FRAME_CURRENT]->data[plane] + s->block_offset[b];
    uint8_t *src_block;
    int src_offset;
    int overlap_offset = 0;
    int mask = s->vp56_coord_div[b] - 1;
    int deblock_filtering = s->deblock_filtering;
    int dx;
    int dy;
    int copy_flag = 0;

    if (s->avctx->skip_loop_filter >= AVDISCARD_ALL ||
        (s->avctx->skip_loop_filter >= AVDISCARD_NONKEY
         && !s->framep[VP56_FRAME_CURRENT]->key_frame))
    { deblock_filtering = 0; }

    dx = s->mv[b].x / s->vp56_coord_div[b];
    dy = s->mv[b].y / s->vp56_coord_div[b];

    if (b >= 4)
    {
        x /= 2;
        y /= 2;
    }

    x += dx - 2;
    y += dy - 2;

    if ((deblock_filtering) && (dy & 7 || dx & 7))
    {
        copy_flag = 1;
    }

    /*此处对mv所指向参考区范围作了限制,限回有效区域x00141957 20100903*/
    if (x < 0 || x + 12 >= s->plane_width[plane] ||
        y < 0 || y + 12 >= s->plane_height[plane])
    {
        ff_emulated_edge_mc(s->edge_emu_buffer,
                            src + s->block_offset[b] + (dy - 2)*stride + (dx - 2),
                            stride, 12, 12, x, y,
                            s->plane_width[plane],
                            s->plane_height[plane]);
        src_block = s->edge_emu_buffer;
        src_offset = 2 + 2 * stride;
    }
    else if (copy_flag)
    {
        /* only need a 12x12 block, but there is no such dsp function, */
        /* so copy a 16x12 block */
        s->dsp.put_pixels_tab[0][0](s->edge_emu_buffer,
                                    src + s->block_offset[b] + (dy - 2)*stride + (dx - 2),
                                    stride, 12);
        src_block = s->edge_emu_buffer;
        src_offset = 2 + 2 * stride;

    }
    else
    {
        src_block = src;
        src_offset = s->block_offset[b] + dy * stride + dx;
    }

    if (copy_flag)
    {
        int t = vp56_filter_threshold[s->quantizer];
        dx = dx & 7;
        dy = dy & 7;

        if (dx)
        {
            s->dsp.vp56_edge_filter(src_block +         10 - dx ,      1, stride, t);
        }

        if (dy)
        {
            s->dsp.vp56_edge_filter(src_block + stride * (10 - dy), stride,      1, t);
        }

    }


    if (s->mv[b].x & mask)
    { overlap_offset += (s->mv[b].x > 0) ? 1 : -1; }

    if (s->mv[b].y & mask)
    { overlap_offset += (s->mv[b].y > 0) ? stride : -stride; }

    if (overlap_offset)
    {
        int filter4 = 0;
        int x8 = s->mv[b].x & mask;
        int y8 = s->mv[b].y & mask;

        if (b < 4)
        {
            x8 *= 2;
            y8 *= 2;
            filter4 = s->filter_mode;

            if (filter4 == 2)
            {
                if (s->max_vector_length &&
                    (FFABS(s->mv[b].x) > s->max_vector_length ||
                     FFABS(s->mv[b].y) > s->max_vector_length))
                {
                    filter4 = 0;
                }
                else if (s->sample_variance_threshold
                         && (vp6_block_variance(src_block + src_offset, stride)
                             < s->sample_variance_threshold))
                {
                    filter4 = 0;
                }
            }
        }

        if ((y8 && overlap_offset * s->flip < 0) || (!y8 && src_offset > src_offset + overlap_offset))
        {
            src_offset = src_offset + overlap_offset;
        }

        if (filter4)
        {
            if (!y8)                        /* left or right combine */
            {
                s->dsp.vp6_filter_diag4_h(dst, src_block + src_offset, stride, 1,
                                          vp6_block_copy_filter[s->filter_selection][x8]);
            }
            else if (!x8)                   /* above or below combine */
            {
                s->dsp.vp6_filter_diag4_v(dst, src_block + src_offset, stride, stride,
                                          vp6_block_copy_filter[s->filter_selection][y8]);

            }
            else
            {
                s->dsp.vp6_filter_diag4(dst, src_block + src_offset + ((s->mv[b].x ^ s->mv[b].y) >> 31), stride,
                                        vp6_block_copy_filter[s->filter_selection][x8],
                                        vp6_block_copy_filter[s->filter_selection][y8]);
            }
        }
        else
        {
            if (!x8 || !y8)
            {
                if (!y8)
                {
                    s->dsp.vp6_filter_diag2_h(dst, src_block + src_offset, stride, BilinearFilters[x8]);
                }
                else
                {
                    s->dsp.vp6_filter_diag2_v(dst, src_block + src_offset, stride, BilinearFilters[y8]);
                }
            }
            else
            {

                s->dsp.vp6_filter_diag2(dst, src_block + src_offset + ((s->mv[b].x ^ s->mv[b].y) >> 31), stride, BilinearFilters[x8], BilinearFilters[y8]);

            }
        }

        /*做idct x00141957 20101120*/
        if ( s->eob_pos[b] <= 1 )
        {
            s->dsp.idct_slow1_add(dst,
                                  s->stride[plane], s->block_coeff[b]);
        }
        else if ( s->eob_pos[b] <= 3)
        {
            s->dsp.idct_slow3_add(dst,
                                  s->stride[plane], s->block_coeff[b]);
        }
        else if ( s->eob_pos[b] <= 10 )
        {
            s->dsp.idct_slow10_add(dst,
                                   s->stride[plane], s->block_coeff[b]);
        }
        else
        {
            s->dsp.idct_slow_add(dst,
                                 s->stride[plane], s->block_coeff[b]);
        }
    }
    else
    {
        s->dsp.put_pixels_tab[1][0](dst, src_block + src_offset, stride, 8);

        if ( s->eob_pos[b] <= 1 )
        {
            s->dsp.idct_slow1_add(dst,
                                  s->stride[plane], s->block_coeff[b]);
        }
        else if ( s->eob_pos[b] <= 3)
        {
            s->dsp.idct_slow3_add(dst,
                                  s->stride[plane], s->block_coeff[b]);
        }
        else if ( s->eob_pos[b] <= 10 )
        {
            s->dsp.idct_slow10_add(dst,
                                   s->stride[plane], s->block_coeff[b]);
        }
        else
        {
            s->dsp.idct_slow_add(dst,
                                 s->stride[plane], s->block_coeff[b]);
        }
    }

}

static void vp56_decode_mb(VP56Context *s, int row, int col, int is_alpha)
{
    AVFrame *frame_current, *frame_ref;
    VP56mb mb_type;
    VP56Frame ref_frame;
    int b, ab, b_max, plane, off;

    if (s->framep[VP56_FRAME_CURRENT]->key_frame)
    { mb_type = VP56_MB_INTRA; }
    else
    { mb_type = vp56_decode_mv(s, row, col); }

    ref_frame = vp56_reference_frame[mb_type];

    s->dsp.clear_blocks(*s->block_coeff);

    s->parse_coeff(s);

    vp56_add_predictors_dc(s, ref_frame);

    frame_current = s->framep[VP56_FRAME_CURRENT];
    frame_ref = s->framep[ref_frame];

    ab = 0;
    b_max = 6;

    switch (mb_type)
    {
        case VP56_MB_INTRA:
            for (b = 0; b < 6; b++)
            {
                plane = vp56_b2p[b];

                if ( s->eob_pos[b] <= 1 )
                {
                    s->dsp.idct_slow1_put(frame_current->data[plane] + s->block_offset[b],
                                          s->stride[plane], s->block_coeff[b]);
                }
                else if ( s->eob_pos[b] <= 3)
                {
                    s->dsp.idct_slow3_put(frame_current->data[plane] + s->block_offset[b],
                                          s->stride[plane], s->block_coeff[b]);
                }

                else if ( s->eob_pos[b] <= 10 )
                {
                    s->dsp.idct_slow10_put(frame_current->data[plane] + s->block_offset[b],
                                           s->stride[plane], s->block_coeff[b]);
                }
                else
                {
                    s->dsp.idct_slow_put(frame_current->data[plane] + s->block_offset[b],
                                         s->stride[plane], s->block_coeff[b]);
                }
            }

            break;

        case VP56_MB_INTER_NOVEC_PF:
        case VP56_MB_INTER_NOVEC_GF:
            for (b = 0; b < 6; b++)
            {
                plane = vp56_b2p[b + ab];
                off = s->block_offset[b];
                s->dsp.put_pixels_tab[1][0](frame_current->data[plane] + off, frame_ref->data[plane] + off, s->stride[plane], 8);

                if ( s->eob_pos[b] <= 1 )
                {
                    s->dsp.idct_slow1_add(frame_current->data[plane] + off,
                                          s->stride[plane], s->block_coeff[b]);
                }
                else if ( s->eob_pos[b] <= 3)
                {
                    s->dsp.idct_slow3_add(frame_current->data[plane] + off,
                                          s->stride[plane], s->block_coeff[b]);
                }
                else if ( s->eob_pos[b] <= 10 )
                {
                    s->dsp.idct_slow10_add(frame_current->data[plane] + off,
                                           s->stride[plane], s->block_coeff[b]);
                }
                else
                {
                    s->dsp.idct_slow_add(frame_current->data[plane] + off,
                                         s->stride[plane], s->block_coeff[b]);
                }

            }

            break;

        case VP56_MB_INTER_DELTA_PF:
        case VP56_MB_INTER_V1_PF:
        case VP56_MB_INTER_V2_PF:
        case VP56_MB_INTER_DELTA_GF:
        case VP56_MB_INTER_4V:
        case VP56_MB_INTER_V1_GF:
        case VP56_MB_INTER_V2_GF:
            for (b = 0; b < 6; b++)
            {
                int x_off = b == 1 || b == 3 ? 8 : 0;
                int y_off = b == 2 || b == 3 ? 8 : 0;
                plane = vp56_b2p[b + ab];
                off = s->block_offset[b];
                vp56_mc(s, b, plane, frame_ref->data[plane], s->stride[plane],
                        16 * col + x_off, 16 * row + y_off);
            }

            break;
    }
}

static int vp56_size_changed(AVCodecContext *avctx)
{
    VP56Context *s = avctx->priv_data;
    int stride = s->framep[VP56_FRAME_CURRENT]->linesize[0];
    int i;

    s->plane_width[0]  = s->plane_width[3]  = avctx->coded_width;
    s->plane_width[1]  = s->plane_width[2]  = avctx->coded_width / 2;
    s->plane_height[0] = s->plane_height[3] = avctx->coded_height;
    s->plane_height[1] = s->plane_height[2] = avctx->coded_height / 2;

    for (i = 0; i < 4; i++)
    { s->stride[i] = s->flip * s->framep[VP56_FRAME_CURRENT]->linesize[i]; }

    s->mb_width  = (avctx->coded_width + 15) / 16;
    s->mb_height = (avctx->coded_height + 15) / 16;

    if (s->mb_width > 1000 || s->mb_height > 1000)
    {
        av_log(avctx, AV_LOG_ERROR, "picture too big\n");
        return -1;
    }

    /*x00141957 20100701 采用av_malloc_hw分配内存 并且不支持size变化*/
    /* s->above_blocks = av_realloc(s->above_blocks,
                                   (4*s->mb_width+6) * sizeof(*s->above_blocks));
    s->macroblocks = av_realloc(s->macroblocks,
                                 s->mb_width*s->mb_height*sizeof(*s->macroblocks));
     */
    if ((s->above_blocks) || (s->macroblocks))
    {
        av_log(avctx, AV_LOG_ERROR, "picture size changed!\n");
        return -1;
    }

    s->above_blocks = av_malloc_hw(avctx, (4 * s->mb_width + 6) * sizeof(*s->above_blocks));
    s->macroblocks = av_malloc_hw(avctx, s->mb_width * s->mb_height * sizeof(*s->macroblocks));

    //	av_free(s->edge_emu_buffer_alloc);
    av_free_hw(s->edge_emu_buffer_alloc);

    /*将av_malloc改为av_malloc_hw x00141957 20100701*/
    s->edge_emu_buffer_alloc = av_malloc_hw(avctx, 16 * stride);

    s->edge_emu_buffer = s->edge_emu_buffer_alloc;

    if (s->flip < 0)
    { s->edge_emu_buffer += 15 * stride; }

    return 0;
}

/*x00141957 20100902修改 增加函数用以计算耗费字节数*/
static int inline vp56_count_bytes(const uint8_t *uiEndbuf, const uint8_t *uiStartbuf)
{
    return ((int )uiEndbuf - (int )uiStartbuf);
}

/*x00141957 20100914 新增函数 用来获得一帧指针*/
static int vp6_get_frame(VP56Context *s, AVFrame **p)
{
    unsigned int i;

    for (i = 0; i < s->avctx->uiBufNum; i++)
    {
        if ((0 == s->frames[i].reference) && (0 == s->frames[i].isbusy))
        {
            break;
        }
    }

    if (i >= s->avctx->uiBufNum)
    {
        av_log(s->avctx, AV_LOG_ERROR, "buf size not enough!\n");
        /*x00141957 20100712*/
        IMEDIA_SET_ERR_SLICE(s->avctx->iErrorCode, IMEDIA_ERR_PIC_OTHER);
        return -1;
    }
    else
    {
        *p = &s->frames[i];
    }

    /*根据实际解码宽高计算linesize*/
    {
        int w, h;
        int h_chroma_shift, v_chroma_shift;
        //int size[4] = {0};
        AVPicture picture;
        int stride_align[4];
        AVCodecContext *avctx = s->avctx;
        int ret;
        w = avctx->width;
        h = avctx->height;

        avcodec_get_chroma_sub_sample(avctx->pix_fmt, &h_chroma_shift, &v_chroma_shift);

        avcodec_align_dimensions(avctx, &w, &h);

        if (!(avctx->flags & CODEC_FLAG_EMU_EDGE))
        {
            w += EDGE_WIDTH * 2;
            h += EDGE_WIDTH * 2;
        }

        ret = ff_fill_linesize(&picture, avctx->pix_fmt, w);

        for (i = 0; i < 4; i++)
        {
#if HAVE_MMX

            if (avctx->codec_id == CODEC_ID_SVQ1)
            { stride_align[i] = 16; }
            else
#endif
                stride_align[i] = STRIDE_ALIGN;

#undef  ALIGN
#define ALIGN(x, a) (((x)+(a)-1)&~((a)-1))
            picture.linesize[i] = ALIGN(picture.linesize[i], stride_align[i]);
        }

        for (i = 0; i < 4; i++)
        {
            (*p)->linesize[i] = picture.linesize[i];
        }

        /*x00141957 201007914 改写*/
        {
            int i;
            int stride = picture.linesize[0];
            s->plane_width[0]  = s->plane_width[3]  = s->avctx->coded_width;
            s->plane_width[1]  = s->plane_width[2]  = s->avctx->coded_width / 2;
            s->plane_height[0] = s->plane_height[3] = s->avctx->coded_height;
            s->plane_height[1] = s->plane_height[2] = s->avctx->coded_height / 2;

            for (i = 0; i < 4; i++)
            { s->stride[i] = s->flip * picture.linesize[i]; }

            s->mb_width  = (s->avctx->coded_width + 15) / 16;
            s->mb_height = (s->avctx->coded_height + 15) / 16;
            /*x00141957 2010 1020修改*/
            /*if (s->flip < 0)
            	s->edge_emu_buffer += 15 * stride;*/
        }
    }
    return 0;
}

//VP5,6解码帧的主函数 XiongFei20091209
int vp56_decode_frame(AVCodecContext *avctx, void *data, int *data_size,
                      const uint8_t *buf, int buf_size)
{
    VP56Context *s = avctx->priv_data;
    AVFrame *p = NULL;

    int is_alpha, av_uninit(alpha_offset);

    /*x00141957 20100901 此处增加指针记录码流起始地址*/
    const uint8_t *uiStartBuf;
    int key_frame;

    avctx->iChromaFormat = 1;

    /*x00141957 2011 0331 buf的首地址是cropsize*/
    avctx->extradata[0] = buf;
    buf ++;
    buf_size --;

    /*x00141957 20100726*/
    avctx->sample_aspect_ratio.den = 1;
    avctx->sample_aspect_ratio.num = 1;

    /* x00141957 20100630 添加最后一帧标志位 */
    if (buf_size == 0)
    {
        AVFrame *pict = data;
        pict->ucLastFrame = 1;
        return AVCODEC_RET_DECODE_LAST_FRAME;
    }

    if (s->has_alpha)
    {
        if (buf_size < 3)
        {
            AVFrame *pict = data;
            pict->ucLastFrame = 1;
            return AVCODEC_RET_DECODE_LAST_FRAME;
        }

        /*x00141957 20100628*/
        alpha_offset = bytestream_get_be24(&buf);
        buf_size -= 3;
        /*目前不支持alpha通道 x00141957 20100902*/
        //av_log(avctx, AV_LOG_ERROR, "don't support alpha channel!\n");
        //IMEDIA_SET_ERR_SLICE(avctx->iErrorCode, IMEDIA_ERR_PIC_OTHER);
    }

    /*记录此帧数据起始地址 x00141957 20100901*/
    uiStartBuf = buf;

    /*不解码alpha通道 x00141957 20110331*/
    is_alpha = 0;
    //   for (is_alpha=0; is_alpha < 1+s->has_alpha; is_alpha++)
    {
        int mb_row, mb_col, mb_row_flip, mb_offset = 0;
        int block, y, uv, stride_y, stride_uv;
        int golden_frame = 0;
        int res;

        s->modelp = &s->models[is_alpha];

        //解析码流头 XiongFei20091208
        /*res = s->parse_header(s, buf, remaining_buf_size, &golden_frame);*/
        /*如果图像宽高变了，则修正为以前的宽高 x00141957 20100903*/
        res = s->parse_header(s, buf, buf_size, &golden_frame, &key_frame);

        /*如果图像宽高非法，则返回，重新寻找头 x00141957 20100913*/
        if (-1 == res)
        {
            return AVCODEC_RET_DECODE_FAILURE;
        }

        /*如果图像宽高大于通道规格，则无法继续解码 x00141957 20110328*/
        if (-2 == res)
        {
            return AVCODEC_RET_DECODE_ABORT;
        }


        /*x00141957 20100906修改 误码测试，当前一关键帧帧头有错误导致未分配数据时，如下修改：不管是否有错误都分配内存*/
        if (!is_alpha)
        {
            if (vp6_get_frame(s, &p) < 0 )
            {
                av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
                /*x00141957 20100712*/
                IMEDIA_SET_ERR_SLICE(avctx->iErrorCode, IMEDIA_ERR_SLICE_OTHER);
                return AVCODEC_RET_DECODE_BLOCK;
            }

            p->reference = 1;
            p->isbusy = 1;
            s->framep[VP56_FRAME_CURRENT] = p;
            s->framep[VP56_FRAME_CURRENT]->key_frame = key_frame;
        }

        if (!res)
        {
            av_log(avctx, AV_LOG_ERROR, "parse_header() failed\n");
            /*x00141957 20100712*/
            IMEDIA_SET_ERR_SLICE(avctx->iErrorCode, IMEDIA_ERR_PIC_OTHER);
            goto DECODE_END;
        }

        /*x00141957 20100901 此处增加代码，检查码流解析是否越界*/
        if (vp56_count_bytes(s->c.buffer, uiStartBuf) > buf_size)
        {
            av_log(avctx, AV_LOG_ERROR, "vp56 parse_header  Crossing the border!\n");
            /*x00141957 20100901*/
            IMEDIA_SET_ERR_SLICE(avctx->iErrorCode, IMEDIA_ERR_SLICE_OTHER);
            goto DECODE_END;
        }

        //当前帧为关键帧，也就是I帧 XiongFei20091209
        if (p->key_frame)
        {
            p->pict_type = FF_I_TYPE;
            s->default_models_init(s);

            for (block = 0; block < s->mb_height * s->mb_width; block++)
            { s->macroblocks[block].type = VP56_MB_INTRA; }
        }
        else
        {
            p->pict_type = FF_P_TYPE;
            vp56_parse_mb_type_models(s);
            s->parse_vector_models(s);
            s->mb_type = VP56_MB_INTER_NOVEC_PF;
        }

        /*x00141957 20100901 此处增加代码，检查码流解析是否越界*/
        if (vp56_count_bytes(s->c.buffer, uiStartBuf) > buf_size)
        {
            av_log(avctx, AV_LOG_ERROR, "vp56 decode MB  Crossing the border!\n");
            /*x00141957 20100901*/
            IMEDIA_SET_ERR_SLICE(avctx->iErrorCode, IMEDIA_ERR_SLICE_OTHER);
            goto DECODE_END;
        }

        if (s->parse_coeff_models(s) < 0)
        {
            av_log(avctx, AV_LOG_ERROR, "parse_coeff_models error!\n");
            /*x00141957 20100927*/
            IMEDIA_SET_ERR_SLICE(avctx->iErrorCode, IMEDIA_ERR_SLICE_OTHER);
            goto DECODE_END;
        }

        /*x00141957 20100901 此处增加代码，检查码流解析是否越界*/
        if (vp56_count_bytes(s->c.buffer, uiStartBuf) > buf_size)
        {
            av_log(avctx, AV_LOG_ERROR, "vp56 parse_coeff_models  Crossing the border!\n");
            /*x00141957 20100901*/
            IMEDIA_SET_ERR_SLICE(avctx->iErrorCode, IMEDIA_ERR_SLICE_OTHER);
            goto DECODE_END;

        }

        //更新dc预测相关信息 XiongFei20091208
        memset(s->prev_dc, 0, sizeof(s->prev_dc));
        s->prev_dc[1][VP56_FRAME_CURRENT] = 128;
        s->prev_dc[2][VP56_FRAME_CURRENT] = 128;

        for (block = 0; block < 4 * s->mb_width + 6; block++)
        {
            s->above_blocks[block].ref_frame = VP56_FRAME_NONE;
            s->above_blocks[block].dc_coeff = 0;
            s->above_blocks[block].not_null_dc = 0;
        }

        s->above_blocks[2 * s->mb_width + 2].ref_frame = VP56_FRAME_CURRENT;
        s->above_blocks[3 * s->mb_width + 4].ref_frame = VP56_FRAME_CURRENT;

        stride_y  = p->linesize[0];
        stride_uv = p->linesize[1];

        if (s->flip < 0)
        { mb_offset = 7; }

        /* main macroblocks loop */
        //从以下开始解码每一个宏块 XiongFei20091208
        for (mb_row = 0; mb_row < s->mb_height; mb_row++)
        {
            if (s->flip < 0)
            { mb_row_flip = s->mb_height - mb_row - 1; }
            else
            { mb_row_flip = mb_row; }

            for (block = 0; block < 4; block++)
            {
                s->left_block[block].ref_frame = VP56_FRAME_NONE;
                s->left_block[block].dc_coeff = 0;
                s->left_block[block].not_null_dc = 0;
            }

            memset(s->coeff_ctx, 0, sizeof(s->coeff_ctx));
            memset(s->coeff_ctx_last, 24, sizeof(s->coeff_ctx_last));

            s->above_block_idx[0] = 1;
            s->above_block_idx[1] = 2;
            s->above_block_idx[2] = 1;
            s->above_block_idx[3] = 2;
            s->above_block_idx[4] = 2 * s->mb_width + 2 + 1;
            s->above_block_idx[5] = 3 * s->mb_width + 4 + 1;

            s->block_offset[s->frbi] = (mb_row_flip * 16 + mb_offset) * stride_y;
            s->block_offset[s->srbi] = s->block_offset[s->frbi] + 8 * stride_y;
            s->block_offset[1] = s->block_offset[0] + 8;
            s->block_offset[3] = s->block_offset[2] + 8;
            s->block_offset[4] = (mb_row_flip * 8 + mb_offset) * stride_uv;
            s->block_offset[5] = s->block_offset[4];

            for (mb_col = 0; mb_col < s->mb_width; mb_col++)
            {

                vp56_decode_mb(s, mb_row, mb_col, is_alpha);

                /*x00141957 20100901 此处增加代码，检查码流解析是否越界*/
                if (vp56_count_bytes(s->c.buffer, uiStartBuf) > buf_size)
                {
                    av_log(avctx, AV_LOG_ERROR, "vp56 decode MB  Crossing the border!\n");
                    /*x00141957 20100901*/
                    IMEDIA_SET_ERR_SLICE(avctx->iErrorCode, IMEDIA_ERR_SLICE_OTHER);
                    goto DECODE_END;
                }

                for (y = 0; y < 4; y++)
                {
                    s->above_block_idx[y] += 2;
                    s->block_offset[y] += 16;
                }

                for (uv = 4; uv < 6; uv++)
                {
                    s->above_block_idx[uv] += 1;
                    s->block_offset[uv] += 8;
                }
            }
        }

    DECODE_END:

        if (p->key_frame || golden_frame)
        {
            if (s->framep[VP56_FRAME_GOLDEN]->data[0] &&
                s->framep[VP56_FRAME_GOLDEN] != s->framep[VP56_FRAME_GOLDEN2])
            {
                //avctx->release_buffer(avctx, s->framep[VP56_FRAME_GOLDEN]);
                /*x00141957 20100914 将前一帧的参考标志位置0*/
                s->framep[VP56_FRAME_GOLDEN]->reference = 0;
            }

            s->framep[VP56_FRAME_GOLDEN] = p;
        }

        /*不解码alpha通道 x00141957 20110331*/
        /*
                if (s->has_alpha) {
                    FFSWAP(AVFrame *, s->framep[VP56_FRAME_GOLDEN],
                                      s->framep[VP56_FRAME_GOLDEN2]);
                    buf += alpha_offset;
                    buf_size -= alpha_offset;
                }
        */
    }

    if (s->framep[VP56_FRAME_PREVIOUS] == s->framep[VP56_FRAME_GOLDEN] || s->framep[VP56_FRAME_PREVIOUS] == s->framep[VP56_FRAME_GOLDEN2])
    {
        if (s->framep[VP56_FRAME_UNUSED] != s->framep[VP56_FRAME_GOLDEN] &&
            s->framep[VP56_FRAME_UNUSED] != s->framep[VP56_FRAME_GOLDEN2])
            FFSWAP(AVFrame *, s->framep[VP56_FRAME_PREVIOUS],
                   s->framep[VP56_FRAME_UNUSED]);
        else
            FFSWAP(AVFrame *, s->framep[VP56_FRAME_PREVIOUS],
                   s->framep[VP56_FRAME_UNUSED2]);
    }
    else if (s->framep[VP56_FRAME_PREVIOUS]->data[0])
    {
        //avctx->release_buffer(avctx, s->framep[VP56_FRAME_PREVIOUS]);
        /*x00141957 20100914 将前一帧的参考标志位置0*/
        s->framep[VP56_FRAME_PREVIOUS]->reference = 0;
    }

    FFSWAP(AVFrame *, s->framep[VP56_FRAME_CURRENT], s->framep[VP56_FRAME_PREVIOUS]);

    *(AVFrame *)data = *p;
    *data_size = sizeof(AVFrame);
    avctx->frame_number++;
    return buf_size;
}

av_cold int vp56_init(AVCodecContext *avctx, int flip, int has_alpha)
{
    VP56Context *s = avctx->priv_data;
    unsigned int i;

    s->avctx = avctx;
    /*x00141957 2011 0331*/
    // avctx->pix_fmt = has_alpha ? PIX_FMT_YUVA420P : PIX_FMT_YUV420P;
    avctx->pix_fmt = PIX_FMT_YUV420P;

    avctx->idct_algo = FF_IDCT_VP3;

    /*x00141957 20100914 get buffer*/
    for (i = 0; i < avctx->uiBufNum ; i++)
    {
        AVFrame *p = &s->frames[i];

        if (avctx->get_buffer(avctx, p) < 0)
        {
            av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
            IMEDIA_SET_ERR_SLICE(avctx->iErrorCode, IMEDIA_ERR_SLICE_OTHER);
            /*释放所有已分配buff，并返回 x00141957 20100914*/
            vp56_free(avctx);
            return AVCODEC_RET_DECODE_ABORT;
        }

        p->data[3] = (uint8_t *)p;
    }

    dsputil_init(&s->dsp, avctx);
    ff_init_scantable(s->dsp.idct_permutation, &s->scantable, ff_zigzag_direct);

    //     for (i=0; i<4; i++)
    //         s->framep[i] = &s->frames[i];
    //     s->framep[VP56_FRAME_UNUSED] = s->framep[VP56_FRAME_GOLDEN];
    //     s->framep[VP56_FRAME_UNUSED2] = s->framep[VP56_FRAME_GOLDEN2];
    for (i = 0; i < 3; i++)
    { s->framep[i] = &s->frames[i]; }

    // 	s->framep[VP56_FRAME_UNUSED] = s->framep[VP56_FRAME_GOLDEN];
    // 	s->framep[VP56_FRAME_UNUSED2] = s->framep[VP56_FRAME_GOLDEN2];
    /*x00141957 20100915*/
    //s->edge_emu_buffer_alloc = NULL;
    //s->above_blocks = NULL;
    //s->macroblocks = NULL;
    s->quantizer = -1;
    /*x00141957 2010 1201*/
    // s->deblock_filtering = 1;

    s->filter = NULL;

    s->has_alpha = has_alpha;

    if (flip)
    {
        s->flip = -1;
        s->frbi = 2;
        s->srbi = 0;
    }
    else
    {
        s->flip = 1;
        s->frbi = 0;
        s->srbi = 2;
    }

    return 0;
}

av_cold int vp56_free(AVCodecContext *avctx)
{
    int pt, ct, cg;
    VP56Context *s = avctx->priv_data;

    /*x00141957 20100630添加*/
    av_freep(&s->parse_context.buffer);
    s->parse_context.buffer_size = 0;
    /*结束新增 x00141957 20100630*/

    /*x00141957 20100701修改 将av_free 改为av_freep*/
    av_freep(&s->above_blocks);
    av_freep(&s->macroblocks);
    av_freep(&s->edge_emu_buffer_alloc);

    /*释放vlc表内存 x00141957 20100928*/
    for (pt = 0; pt < 2; pt++)
    {
        free_vlc(&s->dccv_vlc[pt]);
        free_vlc(&s->runv_vlc[pt]);

        for (ct = 0; ct < 3; ct++)
            for (cg = 0; cg < 6; cg++)
            { free_vlc(&s->ract_vlc[pt][ct][cg]); }
    }

    if (s->framep[VP56_FRAME_GOLDEN]->data[0])
    { avctx->release_buffer(avctx, s->framep[VP56_FRAME_GOLDEN]); }

    //     if (s->framep[VP56_FRAME_GOLDEN2]->data[0])
    //         avctx->release_buffer(avctx, s->framep[VP56_FRAME_GOLDEN2]);
    if (s->framep[VP56_FRAME_PREVIOUS]->data[0])
    { avctx->release_buffer(avctx, s->framep[VP56_FRAME_PREVIOUS]); }

    /*问题单号：AZ1D02229
    修改人：熊飞 +00141957
    时间：2010/7/21
    问题描述： 多次creat process delete导致死机
    问题修改： 增加yuvbuffer的释放*/
    avcodec_default_free_buffers(avctx);
    return 0;
}
