/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpBWPlpb76M4lvoojttTw05AfDyj0e7jwqOFHFuh
IwLDbK4Mfid/d1pwq8wOLrWrV+QNMi9G7tTdX4V1e+ivRK0WxyytmcVBj/POUC8ehwNhZzsJ
E3Nqn46y2Buy0jFgFGXHm90+CdSfSKbbQ2d12B+3BDVnXBk/J5Y/LsLEf0HWpw==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/**
 * @file libavcodec/vp6.c
 * VP6 compatible video decoder
 *
 * Copyright (C) 2006  Aurelien Jacobs <aurel@gnuage.org>
 *
 * The VP6F decoder accepts an optional 1 byte extradata. It is composed of:
 *  - upper 4bits: difference between encoded width and visible width
 *  - lower 4bits: difference between encoded height and visible height
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

#include <stdlib.h>

/*x00141957 20100719*/
#include "mpegvideo.h"

#include "avcodec.h"
#include "dsputil.h"
#include "Bitstream.h"
#include "huffman.h"

#include "vp56.h"
#include "vp56data.h"
#include "vp6data.h"


// VP6 hufman table AC bands
#define VP6_AC_BANDS			6

// Tokens								Value		Extra Bits (range + sign)
#define ZERO_TOKEN              0		//0			Extra Bits 0+0
#define ONE_TOKEN               1		//1			Extra Bits 0+1       
#define TWO_TOKEN               2		//2			Extra Bits 0+1 
#define THREE_TOKEN             3		//3			Extra Bits 0+1
#define FOUR_TOKEN              4		//4			Extra Bits 0+1
#define DCT_VAL_CATEGORY1		5		//5-6		Extra Bits 1+1
#define DCT_VAL_CATEGORY2		6		//7-10		Extra Bits 2+1
#define DCT_VAL_CATEGORY3		7		//11-26		Extra Bits 4+1
#define DCT_VAL_CATEGORY4		8		//11-26		Extra Bits 5+1
#define DCT_VAL_CATEGORY5		9		//27-58		Extra Bits 5+1
#define DCT_VAL_CATEGORY6		10		//59+		Extra Bits 11+1	
#define DCT_EOB_TOKEN           11		//EOB		Extra Bits 0+0
#define MAX_ENTROPY_TOKENS      (DCT_EOB_TOKEN + 1)
#define ILLEGAL_TOKEN			255

#define DC_TOKEN_CONTEXTS		3 // 00, 0!0, !0!0
#define CONTEXT_NODES			(MAX_ENTROPY_TOKENS-7)

#define PREC_CASES				3
#define ZERO_RUN_PROB_CASES     14

#define DC_PROBABILITY_UPDATE_THRESH	100

#define ZERO_CONTEXT_NODE		0
#define EOB_CONTEXT_NODE		1
#define ONE_CONTEXT_NODE		2
#define LOW_VAL_CONTEXT_NODE	3
#define TWO_CONTEXT_NODE		4
#define THREE_CONTEXT_NODE		5
#define HIGH_LOW_CONTEXT_NODE	6
#define CAT_ONE_CONTEXT_NODE	7
#define CAT_THREEFOUR_CONTEXT_NODE	8
#define CAT_THREE_CONTEXT_NODE	9
#define CAT_FIVE_CONTEXT_NODE	10

/*x00141957 2010 1115*/
typedef struct
{
    uint16_t  MinVal;
    short   Length;
    uint8_t   Probs[11];
} TOKENEXTRABITS;

static const UINT32 VP6_HuffTokenMinVal[MAX_ENTROPY_TOKENS] = { 0, 1, 2, 3, 4, 5, 7, 11, 19, 35, 67, 0};
static const TOKENEXTRABITS VP6_TokenExtraBits2[MAX_ENTROPY_TOKENS] =
{
    {  0, -1, { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   } },  //ZERO_TOKEN
    {  1, 0, { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   } },   //ONE_TOKEN
    {  2, 0, { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   } },   //TWO_TOKEN
    {  3, 0, { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   } },   //THREE_TOKEN
    {  4, 0, { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   } },   //FOUR_TOKEN
    {  5, 0, { 159, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0   } },  //DCT_VAL_CATEGORY1
    {  7, 1, { 145, 165, 0,  0,  0,  0,  0,  0,  0,  0,  0   } }, //DCT_VAL_CATEGORY2
    { 11, 2, { 140, 148, 173, 0,  0,  0,  0,  0,  0,  0,  0   } }, //DCT_VAL_CATEGORY3
    { 19, 3, { 135, 140, 155, 176, 0,  0,  0,  0,  0,  0,  0   } }, //DCT_VAL_CATEGORY4
    { 35, 4, { 130, 134, 141, 157, 180, 0,  0,  0,  0,  0,  0   } }, //DCT_VAL_CATEGORY5
    { 67, 10, { 129, 130, 133, 140, 153, 177, 196, 230, 243, 254, 254 } }, //DCT_VAL_CATEGORY6
    {  0, -1, { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   } },  // EOB TOKEN
};

/****************************************************************************
 *
 *
 ****************************************************************************/
//    register int count = _mm_cvtsi64_si32(m64_brCount);
#define NDECODEBOOL_AND_BRANCH_IF_ONE(probability, branch) \
    { \
        unsigned int split; \
        split = 1 +  (((range-1) * probability) >> 8); \
        if ( value >= split<<8 ) \
        { \
            value -= (split<<8); \
            range = range - split; \
            while(range < 0x80 ) \
            { \
                range += range; \
                value += value; \
                if ( !--count ) \
                { \
                    count  = 8; \
                    value |= *brBuffer; \
                    brBuffer++; \
                } \
            } \
            goto branch; \
        } \
        range = split; \
        while(range < 0x80 ) \
        { \
            range += range; \
            value += value; \
            if ( !--count ) \
            { \
                count  = 8; \
                value |= *brBuffer; \
                brBuffer++; \
            } \
        } \
    }
/****************************************************************************
 *
 *
 ****************************************************************************/
#define NDECODEBOOL_AND_BRANCH_IF_ZERO(probability, branch) \
    { \
        unsigned int split; \
        split = 1 +  (((range-1) * probability) >> 8); \
        if ( value < split<<8 ) \
        { \
            range = split; \
            while(range < 0x80 ) \
            { \
                range += range; \
                value += value; \
                if ( !--count ) \
                { \
                    count  = 8; \
                    value |= *brBuffer; \
                    brBuffer++; \
                } \
            } \
            goto branch; \
        } \
        value -= (split<<8); \
        range = range - split; \
        while(range < 0x80 ) \
        { \
            range += range; \
            value += value; \
            if ( !--count ) \
            { \
                count  = 8; \
                value |= *brBuffer; \
                brBuffer++; \
            } \
        } \
    }

#define APPLYSIGN(dest, destvalue) \
    { \
        unsigned int split; \
        split    = (range + 1) >> 1; \
        if ( value >= split<<8 ) \
        { \
            value = value - (split<<8); \
            value += value; \
            range = range - split; \
            range += range; \
            if( !--count ) \
            { \
                count  = 8; \
                value |= *brBuffer; \
                brBuffer++; \
            } \
            dest = -destvalue; \
        } \
        else \
        { \
            range = split; \
            range += range; \
            value += value; \
            if( !--count ) \
            { \
                count  = 8; \
                value |= *brBuffer; \
                brBuffer++; \
            } \
            dest = destvalue; \
        } \
    }

static void vp6_parse_coeff(VP56Context *s);
static void vp6_parse_coeff_huffman(VP56Context *s);

static int vp6_parse_header(VP56Context *s, const uint8_t *buf, int buf_size,
                            int *golden_frame, int *key_frame)
{
    VP56RangeCoder *c = &s->c;
    int parse_filter_info = 0;
    int coeff_offset = 0;
    int vrt_shift = 0;
    int sub_version;
    int rows, cols;
    int res = 1;
    int separated_coeff;
    /*x00141957 20100908 增加变量记录crop大小*/
    /*问题单号：AZ1D02316
    修改人：熊飞 +00141957
    时间：2010/09/09
    问题描述： 解码1080p图像与ffmpeg不一致
    问题修改： 将flv中额外信息带入码流中*/
    /*cropsize改放在s->avctx->extradata[0]中 x00141957 2011 0331*/
    //uint8_t cropsize = buf[0];

    //buf++;
    //buf_size-=1;

    separated_coeff = buf[0] & 1;
    *key_frame = !(buf[0] & 0x80);
    vp56_init_dequant(s, (buf[0] >> 1) & 0x3F);

    if (*key_frame)
    {
        sub_version = buf[1] >> 3;
        s->sub_version = sub_version;

        if ((sub_version != 6) && (sub_version != 7) && (sub_version != 8))
        {
            /*x00141957 20100712*/
            av_log(s->avctx, AV_LOG_WARNING, "sub_version (%d )error !\n", sub_version);
            IMEDIA_SET_ERR_PIC(s->avctx->iErrorCode, IMEDIA_ERR_PIC_ID);
            //return 0;
        }

        /*vp Profile的值必定为0,3 x00141957 20100901*/
        s->filter_header = buf[1] & 0x06;

        if ((s->filter_header != 0) && (s->filter_header != 6))
        {
            av_log(s->avctx, AV_LOG_WARNING, "vpprofile(%d )error!\n", s->filter_header);
            IMEDIA_SET_ERR_PIC(s->avctx->iErrorCode, IMEDIA_ERR_PIC_ID);
        }

        /*此为保留位，必定为1 x00141957 20100901*/
        if (buf[1] & 1)
        {
            av_log(s->avctx, AV_LOG_WARNING, "interlacing not supported\n");
            /*x00141957 20100712*/
            IMEDIA_SET_ERR_PIC(s->avctx->iErrorCode, IMEDIA_ERR_PIC_ID);
            //return 0;
        }

        /*如果buff2offset存在，则还需要读取16个比特 x00141957 20100901*/
        if (separated_coeff || !s->filter_header)
        {
            coeff_offset = AV_RB16(buf + 2) - 2;
            buf += 2;
            buf_size -= 2;
        }

        rows = buf[2];  /* number of stored macroblock rows */
        cols = buf[3];  /* number of stored macroblock cols */
        /* buf[4] is number of displayed macroblock rows */
        /* buf[5] is number of displayed macroblock cols */

#if 0

        if (!s->macroblocks || /* first frame */
            16 * cols != s->avctx->coded_width ||
            16 * rows != s->avctx->coded_height)
        {
            avcodec_set_dimensions(s->avctx, 16 * cols, 16 * rows);

            if (s->avctx->extradata_num == 1)
            {
                s->avctx->width  -= ((unsigned int)s->avctx->extradata[0]) >> 4;
                s->avctx->height -= ( (unsigned int)s->avctx->extradata[0]) & 0x0F;
            }

            res = 2;
        }

#endif

        if (0 == s->avctx->frame_number /* first frame */)
        {
            /*判断图像宽高是否合法 x00141957 20100913*/
            {
                int actualwidth, actualheight;
                actualwidth = 16 * cols;
                actualheight = 16 * rows;
                /*
                actualwidth  -= ((unsigned int)cropsize) >> 4;
                actualheight -=((unsigned int)cropsize) & 0x0F;
                */
                actualwidth  -= ((unsigned int)s->avctx->extradata[0][0]) >> 4;
                actualheight -= ((unsigned int)s->avctx->extradata[0][0]) & 0x0F;

                if (actualwidth > (int)s->avctx->usSourceWidth || actualheight > (int)s->avctx->usSourceHeight )
                {
                    /*x00141957 2011 0328 修改，当实际宽高大于通道宽高时，返回-2，表示无法解码*/
                    av_log(s->avctx, AV_LOG_WARNING, "pic 's actual width and actual height illegal!\n");
                    IMEDIA_SET_ERR_SEQ(s->avctx->iErrorCode, IMEDIA_ERR_SEQ_SIZE);
                    return -2;
                }

                if (actualwidth <= 0 || actualheight <= 0)
                {
                    av_log(s->avctx, AV_LOG_WARNING, "pic 's actual width and actual height illegal!\n");
                    IMEDIA_SET_ERR_SEQ(s->avctx->iErrorCode, IMEDIA_ERR_SEQ_SIZE);
                    return -1;
                }
            }

            avcodec_set_dimensions(s->avctx, 16 * cols, 16 * rows);
            /*
            s->avctx->width  -= ((unsigned int)cropsize) >> 4;
            s->avctx->height -=((unsigned int)cropsize) & 0x0F;
            */
            /*cropsize放在s->avctx->extradata[0]中 x00141957 2011 0331*/
            s->avctx->width  -= ((unsigned int)s->avctx->extradata[0][0]) >> 4;
            s->avctx->height -= ((unsigned int)s->avctx->extradata[0][0]) & 0x0F;
            res = 2;
            /*记录图像实际宽高 x00141957 20100629*/
            s->avctx->usActualWidth = s->avctx->width;
            s->avctx->usActualHeight = s->avctx->height;
        }
        else if (16 * cols != s->avctx->coded_width ||
                 16 * rows != s->avctx->coded_height) /*x00141957 20100905修改*/
        {
            av_log(s->avctx, AV_LOG_WARNING, " unsupport size [%d x %d] changed to [%d x %d]!\n", s->avctx->coded_width, s->avctx->coded_height, 16 * cols, 16 * rows);
            /*x00141957 20100905*/
            IMEDIA_SET_ERR_PIC(s->avctx->iErrorCode, IMEDIA_ERR_PIC_ID);
        }


        /*初始化bool解码结构体 x00141957 20100901*/
        vp56_init_range_decoder(c, buf + 6, buf_size - 6);

        /*scaling mode x00141957 20100901*/
        vp56_rac_gets(c, 2);

        parse_filter_info = s->filter_header;

        if (sub_version < 8)
        { vrt_shift = 5; }

        s->sub_version = sub_version;

        /*x00141957 20100715 增加keyframe计数*/
        s->avctx->uiDecIFrames++;
    }
    else
    {

        if (separated_coeff || !s->filter_header)
        {
            coeff_offset = AV_RB16(buf + 1) - 2;
            buf += 2;
            buf_size -= 2;
        }

        vp56_init_range_decoder(c, buf + 1, buf_size - 1);

        *golden_frame = vp56_rac_get(c);

        if (s->filter_header)
        {
            s->deblock_filtering = vp56_rac_get(c);

            if (s->deblock_filtering)
            { vp56_rac_get(c); }

            if (s->sub_version > 7)
            { parse_filter_info = vp56_rac_get(c); }
        }

        /*x00141957 20100715 增加p frame计数*/
        s->avctx->uiDecPFrames++;

        /*增加代码判断，由于可能出现第一帧为P帧的情况 x00141957 20110725*/
        if (s->avctx->usActualWidth > (int)s->avctx->usSourceWidth || s->avctx->usActualHeight > (int)s->avctx->usSourceHeight )
        {
            /*x00141957 2011 0328 修改，当实际宽高大于通道宽高时，返回-2，表示无法解码*/
            av_log(s->avctx, AV_LOG_WARNING, "pic 's actual width and actual height illegal!\n");
            IMEDIA_SET_ERR_SEQ(s->avctx->iErrorCode, IMEDIA_ERR_SEQ_SIZE);
            return -2;
        }

        if (s->avctx->usActualWidth <= 0 || s->avctx->usActualHeight <= 0)
        {
            av_log(s->avctx, AV_LOG_WARNING, "pic 's actual width and actual height illegal!\n");
            IMEDIA_SET_ERR_SEQ(s->avctx->iErrorCode, IMEDIA_ERR_SEQ_SIZE);
            return -1;
        }
    }

    if (parse_filter_info)
    {
        if (vp56_rac_get(c))
        {
            s->filter_mode = 2;
            s->sample_variance_threshold = vp56_rac_gets(c, 5) << vrt_shift;
            s->max_vector_length = 2 << vp56_rac_gets(c, 3);
        }
        else if (vp56_rac_get(c))
        {
            s->filter_mode = 1;
        }
        else
        {
            s->filter_mode = 0;
        }

        if (s->sub_version > 7)
        { s->filter_selection = vp56_rac_gets(c, 4); }
        else
        { s->filter_selection = 16; }
    }

    s->use_huffman = vp56_rac_get(c);

    s->parse_coeff = vp6_parse_coeff;

    if (coeff_offset)
    {
        buf      += coeff_offset;
        buf_size -= coeff_offset;

        if (buf_size < 0)
        {
            /*x00141957 20100712*/
            av_log(s->avctx, AV_LOG_WARNING, "buf_size < 0 \n");
            /*x00141957 20100712*/
            IMEDIA_SET_ERR_PIC(s->avctx->iErrorCode, IMEDIA_ERR_PIC_ID);
            return 0;
        }

        if (s->use_huffman)
        {
            s->parse_coeff = vp6_parse_coeff_huffman;
            init_get_bits(&s->gb, buf, buf_size << 3);
        }
        else
        {
            vp56_init_range_decoder(&s->cc, buf, buf_size);
            s->ccp = &s->cc;
        }
    }
    else
    {
        s->ccp = &s->c;
    }

    return res;
}

static void vp6_coeff_order_table_init(VP56Context *s)
{
    int i, pos, idx = 1;

    int MaxOffset, j;

    s->modelp->coeff_index_to_pos[0] = 0;

    for (i = 0; i < 16; i++)
        for (pos = 1; pos < 64; pos++)
            if (s->modelp->coeff_reorder[pos] == i)
            { s->modelp->coeff_index_to_pos[idx++] = pos; }


    for ( i = 0; i < 64; i++ )
    {
        MaxOffset = 0;

        for ( j = 0; j <= i; j++ )
        {
            if ( s->modelp->coeff_index_to_pos[j] > MaxOffset )
            { MaxOffset = s->modelp->coeff_index_to_pos[j]; }
        }

        s->EobOffsetTable[i] = MaxOffset;

        if (s->sub_version > 6)
        { s->EobOffsetTable[i] = MaxOffset + 1; }

    }

}

static void vp6_default_models_init(VP56Context *s)
{
    VP56Model *model = s->modelp;

    model->vector_dct[0] = 0xA2;
    model->vector_dct[1] = 0xA4;
    model->vector_sig[0] = 0x80;
    model->vector_sig[1] = 0x80;

    memcpy(model->mb_types_stats, vp56_def_mb_types_stats, sizeof(model->mb_types_stats));
    memcpy(model->vector_fdv, vp6_def_fdv_vector_model, sizeof(model->vector_fdv));
    memcpy(model->vector_pdv, vp6_def_pdv_vector_model, sizeof(model->vector_pdv));
    memcpy(model->coeff_runv, vp6_def_runv_coeff_model, sizeof(model->coeff_runv));
    memcpy(model->coeff_reorder, vp6_def_coeff_reorder, sizeof(model->coeff_reorder));

    vp6_coeff_order_table_init(s);
}

static void vp6_parse_vector_models(VP56Context *s)
{
    VP56RangeCoder *c = &s->c;
    VP56Model *model = s->modelp;
    int comp, node;

    for (comp = 0; comp < 2; comp++)
    {
        if (vp56_rac_get_prob(c, vp6_sig_dct_pct[comp][0]))
        { model->vector_dct[comp] = vp56_rac_gets_nn(c, 7); }

        if (vp56_rac_get_prob(c, vp6_sig_dct_pct[comp][1]))
        { model->vector_sig[comp] = vp56_rac_gets_nn(c, 7); }
    }

    for (comp = 0; comp < 2; comp++)
        for (node = 0; node < 7; node++)
            if (vp56_rac_get_prob(c, vp6_pdv_pct[comp][node]))
            { model->vector_pdv[comp][node] = vp56_rac_gets_nn(c, 7); }

    for (comp = 0; comp < 2; comp++)
        for (node = 0; node < 8; node++)
            if (vp56_rac_get_prob(c, vp6_fdv_pct[comp][node]))
            { model->vector_fdv[comp][node] = vp56_rac_gets_nn(c, 7); }
}

/* nodes must ascend by count, but with descending symbol order */
static int vp6_huff_cmp(const void *va, const void *vb)
{
    const Node *a = va, *b = vb;
    return (a->count - b->count) * 16 + (b->sym - a->sym);
}

static int vp6_build_huff_tree(VP56Context *s, uint8_t coeff_model[],
                               const uint8_t *map, unsigned size, VLC *vlc)
{
    //Node nodes[2*size], *tmp = &nodes[size];
    /*x00141957 20100608修改*/
    //	Node nodes[2*size], *tmp = &nodes[size];
    Node *tmp;
#if __STDC_VERSION__ >= 199901L
    Node nodes[2 * size];
#else
    Node *nodes = _alloca(sizeof(Node) * 2 * size); //add by x00141957 20100608
#endif

    int a, b;
    /*x00141957 20100729 消除warning*/
    unsigned int i;

    /*x00141957 20100927*/
    int ret;
    /*x00141957 20100608*/
    tmp = &nodes[size];

    /* first compute probabilities from model */
    tmp[0].count = 256;

    for (i = 0; i < size - 1; i++)
    {
        a = tmp[i].count *        coeff_model[i]  >> 8;
        b = tmp[i].count * (255 - coeff_model[i]) >> 8;
        nodes[map[2 * i  ]].count = a + !a;
        nodes[map[2 * i + 1]].count = b + !b;
    }

    /* then build the huffman tree accodring to probabilities */
    ret = ff_huff_build_tree(s->avctx, vlc, size, nodes, vp6_huff_cmp,
                             FF_HUFFMAN_FLAG_HNODE_FIRST);

    return ret;
}

static int vp6_parse_coeff_models(VP56Context *s)
{
    VP56RangeCoder *c = &s->c;
    VP56Model *model = s->modelp;
    int def_prob[11];
    int node, cg, ctx, pos;
    int ct;    /* code type */
    int pt;    /* plane type (0 for Y, 1 for U or V) */

    memset(def_prob, 0x80, sizeof(def_prob));

    for (pt = 0; pt < 2; pt++)
        for (node = 0; node < 11; node++)
            if (vp56_rac_get_prob(c, vp6_dccv_pct[pt][node]))
            {
                def_prob[node] = vp56_rac_gets_nn(c, 7);
                model->coeff_dccv[pt][node] = def_prob[node];
            }
            else if (s->framep[VP56_FRAME_CURRENT]->key_frame)
            {
                model->coeff_dccv[pt][node] = def_prob[node];
            }

    if (vp56_rac_get(c))
    {
        for (pos = 1; pos < 64; pos++)
            if (vp56_rac_get_prob(c, vp6_coeff_reorder_pct[pos]))
            { model->coeff_reorder[pos] = vp56_rac_gets(c, 4); }

        vp6_coeff_order_table_init(s);
    }

    for (cg = 0; cg < 2; cg++)
        for (node = 0; node < 14; node++)
            if (vp56_rac_get_prob(c, vp6_runv_pct[cg][node]))
            { model->coeff_runv[cg][node] = vp56_rac_gets_nn(c, 7); }

    for (ct = 0; ct < 3; ct++)
    {
        for (pt = 0; pt < 2; pt++)
        {
            for (cg = 0; cg < 6; cg++)
            {
                for (node = 0; node < 11; node++)
                {
                    if (vp56_rac_get_prob(c, vp6_ract_pct[ct][pt][cg][node]))
                    {
                        def_prob[node] = vp56_rac_gets_nn(c, 7);
                        model->coeff_ract[pt][ct][cg][node] = def_prob[node];
                    }
                    else if (s->framep[VP56_FRAME_CURRENT]->key_frame)
                    {
                        model->coeff_ract[pt][ct][cg][node] = def_prob[node];
                    }
                }
            }
        }
    }

    if (s->use_huffman)
    {
        for (pt = 0; pt < 2; pt++)
        {
            s->dccv_vlc[pt].avctx = s->avctx;
            s->runv_vlc[pt].avctx = s->avctx;

            if (vp6_build_huff_tree(s, model->coeff_dccv[pt],
                                    vp6_huff_coeff_map, 12, &s->dccv_vlc[pt]) < 0)
            { return -1; }

            if (vp6_build_huff_tree(s, model->coeff_runv[pt],
                                    vp6_huff_run_map, 9, &s->runv_vlc[pt]) < 0)
            { return -1; }

            for (ct = 0; ct < 3; ct++)
                for (cg = 0; cg < 6; cg++)
                {
                    s->ract_vlc[pt][ct][cg].avctx = s->avctx;

                    if (vp6_build_huff_tree(s, model->coeff_ract[pt][ct][cg],
                                            vp6_huff_coeff_map, 12,
                                            &s->ract_vlc[pt][ct][cg]))
                    { return -1; }
                }
        }

        memset(s->nb_null, 0, sizeof(s->nb_null));
    }
    else
    {
        /* coeff_dcct is a linear combination of coeff_dccv */
        for (pt = 0; pt < 2; pt++)
            for (ctx = 0; ctx < 3; ctx++)
                for (node = 0; node < 5; node++)
                { model->coeff_dcct[pt][ctx][node] = av_clip(((model->coeff_dccv[pt][node] * vp6_dccv_lc[ctx][node][0] + 128) >> 8) + vp6_dccv_lc[ctx][node][1], 1, 255); }
    }

    return 0;
}

static void vp6_parse_vector_adjustment(VP56Context *s, VP56mv *vect)
{
    VP56RangeCoder *c = &s->c;
    VP56Model *model = s->modelp;
    int comp;

    /**vect = (VP56mv) {0,0};*/
    /*x00141957 20100608*/
    vect->x = 0;
    vect->y = 0;

    if (s->vector_candidate_pos < 2)
    { *vect = s->vector_candidate[0]; }

    for (comp = 0; comp < 2; comp++)
    {
        int i, delta = 0;

        if (vp56_rac_get_prob(c, model->vector_dct[comp]))
        {
            static const uint8_t prob_order[] = {0, 1, 2, 7, 6, 5, 4};

            for (i = 0; i < sizeof(prob_order); i++)
            {
                int j = prob_order[i];
                delta |= vp56_rac_get_prob(c, model->vector_fdv[comp][j]) << j;
            }

            if (delta & 0xF0)
            { delta |= vp56_rac_get_prob(c, model->vector_fdv[comp][3]) << 3; }
            else
            { delta |= 8; }
        }
        else
        {
            delta = vp56_rac_get_tree(c, vp56_pva_tree,
                                      model->vector_pdv[comp]);
        }

        if (delta && vp56_rac_get_prob(c, model->vector_sig[comp]))
        { delta = -delta; }

        if (!comp)
        { vect->x += delta; }
        else
        { vect->y += delta; }
    }
}

/**
 * Read number of consecutive blocks with null DC or AC.
 * This value is < 74.
 */
static unsigned vp6_get_nb_null(VP56Context *s)
{
    unsigned val = get_bits(&s->gb, 2);

    if (val == 2)
    { val += get_bits(&s->gb, 2); }
    else if (val == 3)
    {
        val = get_bits1(&s->gb) << 2;
        val = 6 + val + get_bits(&s->gb, 2 + val);
    }

    return val;
}

#if 0
static void vp6_parse_coeff_huffman(VP56Context *s)
{
    VP56Model *model = s->modelp;
    uint8_t *permute = s->scantable.permutated;
    VLC *vlc_coeff;
    int coeff, sign, coeff_idx;
    int b, cg, idx;
    int pt = 0;    /* plane type (0 for Y, 1 for U or V) */

    for (b = 0; b < 6; b++)
    {
        int ct = 0;    /* code type */

        if (b > 3) { pt = 1; }

        vlc_coeff = &s->dccv_vlc[pt];

        for (coeff_idx = 0; coeff_idx < 64; )
        {
            int run = 1;

            if (coeff_idx < 2 && s->nb_null[coeff_idx][pt])
            {
                s->nb_null[coeff_idx][pt]--;

                if (coeff_idx)
                { break; }
            }
            else
            {
                coeff = get_vlc2(&s->gb, vlc_coeff->table, VP6VLCBITS/*9*/, 3);

                if (coeff == 0)
                {
                    if (coeff_idx)
                    {
                        int pt = (coeff_idx >= 6);
                        run += get_vlc2(&s->gb, s->runv_vlc[pt].table, VP6VLCBITS/*9*/, 3);

                        if (run >= 9)
                        { run += get_bits(&s->gb, 6); }
                    }
                    else
                    { s->nb_null[0][pt] = vp6_get_nb_null(s); }

                    ct = 0;
                }
                else if (coeff == 11)      /* end of block */
                {
                    if (coeff_idx == 1)    /* first AC coeff ? */
                    { s->nb_null[1][pt] = vp6_get_nb_null(s); }

                    break;
                }
                else
                {
                    int coeff2 = vp56_coeff_bias[coeff];

                    if (coeff > 4)
                    { coeff2 += get_bits(&s->gb, coeff <= 9 ? coeff - 4 : 11); }

                    ct = 1 + (coeff2 > 1);
                    sign = get_bits1(&s->gb);
                    coeff2 = (coeff2 ^ -sign) + sign;

                    if (coeff_idx)
                    { coeff2 *= s->dequant_ac; }

                    idx = model->coeff_index_to_pos[coeff_idx];
                    s->block_coeff[b][permute[idx]] = coeff2;
                }
            }

            coeff_idx += run;
            cg = FFMIN(vp6_coeff_groups[coeff_idx], 3);
            vlc_coeff = &s->ract_vlc[pt][ct][cg];
        }
    }
}
#else
static void vp6_parse_coeff_huffman(VP56Context *s)
{
    VP56Model *model = s->modelp;
    uint8_t *permute = s->scantable.permutated;
    VLC *vlc_coeff;
    int coeff, sign, coeff_idx;
    int b, cg, idx;
    int pt = 0;    /* plane type (0 for Y, 1 for U or V) */
    int SignBit;
    int16_t *CoeffData;

    /*x00141957 2010 1128 6个block*/
    for (b = 0; b < 6; b++)
    {
        int ct = 0;    /* code type */
        int run = 0;

        if (b > 3)
        { pt = 1; }

        coeff_idx = 0;
        vlc_coeff = &s->dccv_vlc[pt];

        /*开始解码DC系数 x00141957 2010 1128*/

        if (s->nb_null[coeff_idx][pt])
        {
            s->nb_null[coeff_idx][pt]--;
        }
        else
        {
            coeff = get_vlc2(&s->gb, vlc_coeff->table, VP6VLCBITS/*9*/, 3);

            if (coeff == DCT_EOB_TOKEN)
            { goto Finished; }

            if (coeff == ZERO_TOKEN)
            {
                // Read zero run-length
                {
                    // Run of zeros at DC is coded as a tree
                    UINT32 val = 1 + get_bits(&s->gb, 2);

                    if ( val == 3 )
                    { val += get_bits(&s->gb, 2); }
                    else if ( val == 4 )
                    {
                        if ( get_bits1(&s->gb) )
                        { val = 11 + get_bits(&s->gb, 6); }
                        else
                        { val = 7 + get_bits(&s->gb, 2); }
                    }

                    s->nb_null[coeff_idx][pt] = val - 1;
                }
                ct = 0;
            }
            else
            {
                int value;

                value = VP6_HuffTokenMinVal[coeff];

                if (coeff <= FOUR_TOKEN)
                {
                    SignBit = get_bits1(&s->gb);
                }
                else if (coeff <= DCT_VAL_CATEGORY5)
                {
                    value   += get_bits(&s->gb, (coeff - 4));
                    SignBit = get_bits1(&s->gb);
                }
                else
                {
                    value   += get_bits(&s->gb, 11);
                    SignBit = get_bits1(&s->gb);
                }

                ct = (value > 1) ? 2 : 1;
                s->block_coeff[b][0] = (int16_t)((value ^ -SignBit) + SignBit);
            }
        }

        /*开始解码AC系数 x00141957 2010 1128*/

        coeff_idx ++;
        cg = FFMIN(vp6_coeff_groups[coeff_idx], 3);

        if ( s->nb_null[coeff_idx][pt] > 0 )
        {
            // First AC in scan order -- run of EOBs in progress
            --s->nb_null[coeff_idx][pt];
            goto Finished;
        }

        do
        {
            vlc_coeff = &s->ract_vlc[pt][ct][cg]; //VP6_CoeffToHuffBand[EncodedCoeffs];
            run = 0;
            coeff = get_vlc2(&s->gb, vlc_coeff->table, VP6VLCBITS/*9*/, 3);

            if (coeff == ZERO_TOKEN)
            {
                {
                    int pt = (coeff_idx >= 6);
                    run += get_vlc2(&s->gb, s->runv_vlc[pt].table, VP6VLCBITS/*9*/, 3);

                    if (run >= 8)
                    { run += get_bits(&s->gb, 6); }

                    coeff_idx += run;
                }
                coeff_idx ++;
                ct = 0;
                cg = FFMIN(vp6_coeff_groups[coeff_idx], 3);
                continue;
            }

            if (coeff == DCT_EOB_TOKEN)
            {
                if ( coeff_idx == 1 )
                {
                    // Read run of EOB at first AC position
                    s->nb_null[1][pt] = vp6_get_nb_null(s);
                }

                goto Finished;

            }

            {
                int value;

                value = VP6_HuffTokenMinVal[coeff];

                if (coeff > FOUR_TOKEN)
                { value += get_bits(&s->gb, coeff <= DCT_VAL_CATEGORY5 ? coeff - FOUR_TOKEN : 11); }

                ct = (value > 1) ? 2 : 1;
                SignBit = get_bits1(&s->gb);

                value = (value ^ -SignBit) + SignBit;
                value *= s->dequant_ac;;
                idx = model->coeff_index_to_pos[coeff_idx];
                s->block_coeff[b][permute[idx]] = (int16_t)(value);

                coeff_idx++;
                cg = FFMIN(vp6_coeff_groups[coeff_idx], 3);
            }
        }
        while ( coeff_idx < 64 );

        coeff_idx--;
    Finished:
        s->eob_pos[b] = s->EobOffsetTable[coeff_idx];
    }
}
#endif
#if 0
static void vp6_parse_coeff(VP56Context *s)
{
    VP56RangeCoder *c = s->ccp;
    VP56Model *model = s->modelp;
    uint8_t *permute = s->scantable.permutated;
    uint8_t *model1, *model2, *model3;
    int coeff, sign, coeff_idx;
    int b, i, cg, idx, ctx;
    int pt = 0;    /* plane type (0 for Y, 1 for U or V) */

    for (b = 0; b < 6; b++)
    {
        int ct = 1;    /* code type */
        int run = 1;

        if (b > 3) { pt = 1; }

        ctx = s->left_block[vp56_b6to4[b]].not_null_dc
              + s->above_blocks[s->above_block_idx[b]].not_null_dc;
        model1 = model->coeff_dccv[pt];
        model2 = model->coeff_dcct[pt][ctx];

        for (coeff_idx = 0; coeff_idx < 64; )
        {
            if ((coeff_idx > 1 && ct == 0) || vp56_rac_get_prob(c, model2[0]))
            {
                /* parse a coeff */
                if (vp56_rac_get_prob(c, model2[2]))
                {
                    if (vp56_rac_get_prob(c, model2[3]))
                    {
                        idx = vp56_rac_get_tree(c, vp56_pc_tree, model1);
                        coeff = vp56_coeff_bias[idx + 5];

                        for (i = vp56_coeff_bit_length[idx]; i >= 0; i--)
                        { coeff += vp56_rac_get_prob(c, vp56_coeff_parse_table[idx][i]) << i; }
                    }
                    else
                    {
                        if (vp56_rac_get_prob(c, model2[4]))
                        { coeff = 3 + vp56_rac_get_prob(c, model1[5]); }
                        else
                        { coeff = 2; }
                    }

                    ct = 2;
                }
                else
                {
                    ct = 1;
                    coeff = 1;
                }

                sign = vp56_rac_get(c);
                coeff = (coeff ^ -sign) + sign;

                if (coeff_idx)
                { coeff *= s->dequant_ac; }

                idx = model->coeff_index_to_pos[coeff_idx];
                s->block_coeff[b][permute[idx]] = coeff;
                run = 1;
            }
            else
            {
                /* parse a run */
                ct = 0;

                if (coeff_idx > 0)
                {
                    if (!vp56_rac_get_prob(c, model2[1]))
                    { break; }

                    model3 = model->coeff_runv[coeff_idx >= 6];
                    run = vp56_rac_get_tree(c, vp6_pcr_tree, model3);

                    if (!run)
                        for (run = 9, i = 0; i < 6; i++)
                        { run += vp56_rac_get_prob(c, model3[i + 8]) << i; }
                }
            }

            cg = vp6_coeff_groups[coeff_idx += run];
            model1 = model2 = model->coeff_ract[pt][ct][cg];
        }

        s->left_block[vp56_b6to4[b]].not_null_dc =
            s->above_blocks[s->above_block_idx[b]].not_null_dc = !!s->block_coeff[b][0];
    }
}

#else
static void vp6_parse_coeff(VP56Context *s)
{
    VP56RangeCoder *c = s->ccp;
    VP56Model *model = s->modelp;
    uint8_t *permute = s->scantable.permutated;
    uint8_t *model1, *model2, *model3;

    int b, cg, idx, ctx;
    int pt = 0;    /* plane type (0 for Y, 1 for U or V) */
    int index;
    int token;
    int run;
    int ct;

    int count = c->bits;
    unsigned int range = c->high;
    unsigned int value = c->code_word;

    uint8_t *brBuffer = c->buffer;

    ct = 0;
    b = 0;

    do
    {
        run = 0;
        idx = 0;
        index = 0;

        if (b > 3)
        {
            pt = 1;
        }

        ctx = s->left_block[vp56_b6to4[b]].not_null_dc  + s->above_blocks[s->above_block_idx[b]].not_null_dc;
        model1 = model->coeff_dccv[pt];
        model2 = model->coeff_dcct[pt][ctx];
        // Decode the dc token -- first test to see if it is zero
        NDECODEBOOL_AND_BRANCH_IF_ONE(model2[ZERO_CONTEXT_NODE], DC_NON_ZERO_);
        idx ++;
        ct = 0;
        /*DC系数为零 开始解码AC系数 x00141957 20101115*/
        goto AC_DO_WHILE;

    DC_NON_ZERO_:

        // Was the value a 1
        ct = 1;
        NDECODEBOOL_AND_BRANCH_IF_ZERO(model2[ONE_CONTEXT_NODE], ONE_CONTEXT_NODE_0_);
        ct = 2;

        // Value token > 1
        NDECODEBOOL_AND_BRANCH_IF_ZERO(model2[LOW_VAL_CONTEXT_NODE], LOW_VAL_CONTEXT_NODE_0_);

        // High value (value category) token
        NDECODEBOOL_AND_BRANCH_IF_ZERO(model1[HIGH_LOW_CONTEXT_NODE], HIGH_LOW_CONTEXT_NODE_0_);

        // Cat3,Cat4 or Cat5
        NDECODEBOOL_AND_BRANCH_IF_ZERO(model1[CAT_THREEFOUR_CONTEXT_NODE], CAT_THREEFOUR_CONTEXT_NODE_0_);

        token = DCT_VAL_CATEGORY5;

        NDECODEBOOL_AND_BRANCH_IF_ZERO(model1[CAT_FIVE_CONTEXT_NODE], DC_EXTRA_BITS_);

        token += 1;

        goto DC_EXTRA_BITS_;

    CAT_THREEFOUR_CONTEXT_NODE_0_:
        token = DCT_VAL_CATEGORY3;

        NDECODEBOOL_AND_BRANCH_IF_ZERO(model1[CAT_THREE_CONTEXT_NODE], DC_EXTRA_BITS_);

        token += 1;

        goto DC_EXTRA_BITS_;

    HIGH_LOW_CONTEXT_NODE_0_:
        // Either Cat1 or Cat2
        token = DCT_VAL_CATEGORY1;

        NDECODEBOOL_AND_BRANCH_IF_ZERO(model1[CAT_ONE_CONTEXT_NODE], DC_EXTRA_BITS_);

        token += 1;

    DC_EXTRA_BITS_:
        {
            INT32       tValue;
            INT32		BitsCount;

            unsigned int split;

            tValue = VP6_TokenExtraBits2[token].MinVal;

            // Read the extra bits
            BitsCount = VP6_TokenExtraBits2[token].Length;

            do
            {
                split = 1 +  (((range - 1) * VP6_TokenExtraBits2[token].Probs[BitsCount] ) >> 8);

                if ( value >= split << 8 )
                {
                    value -= (split << 8);
                    split = range - split;

                    tValue += (1 << BitsCount);

                }

                while (split < 0x80 )
                {
                    split <<= 1;
                    value <<= 1;

                    if ( !--count)
                    {
                        count  = 8;
                        value |= *brBuffer++;
                    }
                }

                range = split;
            }
            while (--BitsCount >= 0);


            // apply the sign to the value
            APPLYSIGN(s->block_coeff[b][0], tValue);

            idx ++;
            /*DC系数解码完成 x00141957 2010 11.16*/
            goto AC_DO_WHILE;
        }

    LOW_VAL_CONTEXT_NODE_0_:
        // Low value token
        NDECODEBOOL_AND_BRANCH_IF_ZERO(model2[TWO_CONTEXT_NODE], TWO_CONTEXT_NODE_0_);

        // Either a 3 or a 4
        token = THREE_TOKEN;

        NDECODEBOOL_AND_BRANCH_IF_ZERO(model1[THREE_CONTEXT_NODE], THREE_CONTEXT_NODE_0_);

        token += 1;

    THREE_CONTEXT_NODE_0_:
        // apply the sign to the value
        APPLYSIGN(s->block_coeff[b][0], token);
        idx ++;

        goto AC_DO_WHILE;

    TWO_CONTEXT_NODE_0_:
        // Is it a  2
        // apply the sign to the value
        APPLYSIGN(s->block_coeff[b][0], TWO_TOKEN);
        idx ++;

        goto AC_DO_WHILE;

    ONE_CONTEXT_NODE_0_:

        // apply the sign to the value
        APPLYSIGN(s->block_coeff[b][0], 1);
        idx ++;

    AC_DO_WHILE:
        /*采用AC解码prob表 x00141957 2010 11.16*/
        cg = vp6_coeff_groups[idx];
        model2 = model->coeff_ract[pt][ct][cg];
        ct = 0;
        //判断是否无值 x00141957 2010 1116
        NDECODEBOOL_AND_BRANCH_IF_ONE(model2[ZERO_CONTEXT_NODE], NON_ZERO_RUN_);

        //ZERO_RUN_:
        // Is the token a Zero or EOB
        NDECODEBOOL_AND_BRANCH_IF_ZERO(model2[EOB_CONTEXT_NODE], BLOCK_FINISHED_1);

        /*将model3指针指向zerorunprobs x00141957 2010 1115*/
        model3 = model->coeff_runv[idx >= 6];
        /*此时开始解码0run x00141957 2010 1115*/
        NDECODEBOOL_AND_BRANCH_IF_ONE(model3[0], ZERO_RUN_5_8);

        //ZERO_RUN_1_4:
        NDECODEBOOL_AND_BRANCH_IF_ONE(model3[1], ZERO_RUN_1_4_a);
        idx ++;

        NDECODEBOOL_AND_BRANCH_IF_ZERO(model3[2], ZERO_RUN_1_4_done);
        idx ++;

    ZERO_RUN_1_4_done:
        cg = vp6_coeff_groups[idx];
        model2 = model->coeff_ract[pt][ct][cg];

        if ( idx < 64)
        { goto NON_ZERO_RUN_; }

        goto BLOCK_FINISHED;

    ZERO_RUN_1_4_a:
        idx += 3;

        NDECODEBOOL_AND_BRANCH_IF_ZERO(model3[3], ZERO_RUN_1_4_a_done);

        idx += 1;

    ZERO_RUN_1_4_a_done:
        cg = vp6_coeff_groups[idx];
        model2 = model->coeff_ract[pt][ct][cg];

        if ( idx < 64)
        { goto NON_ZERO_RUN_; }

        goto BLOCK_FINISHED;

    ZERO_RUN_5_8:
        // Run length 5-8
        NDECODEBOOL_AND_BRANCH_IF_ONE(model3[4], ZERO_RUN_gt_8);

        NDECODEBOOL_AND_BRANCH_IF_ONE(model3[5], ZERO_RUN_5_8_a);

        idx += 5;

        NDECODEBOOL_AND_BRANCH_IF_ZERO(model3[6], ZERO_RUN_5_8_done);

        idx += 1;

    ZERO_RUN_5_8_done:
        cg = vp6_coeff_groups[idx];
        model2 = model->coeff_ract[pt][ct][cg];

        if ( idx < 64)
        { goto NON_ZERO_RUN_; }

        goto BLOCK_FINISHED;

    ZERO_RUN_5_8_a:
        idx += 7;

        NDECODEBOOL_AND_BRANCH_IF_ZERO(model3[7], ZERO_RUN_5_8_a_done);

        idx += 1;

    ZERO_RUN_5_8_a_done:
        cg = vp6_coeff_groups[idx];
        model2 = model->coeff_ract[pt][ct][cg];

        if ( idx < 64)
        { goto NON_ZERO_RUN_; }

        goto BLOCK_FINISHED;

    ZERO_RUN_gt_8:
        // Run length > 8
        {
            unsigned int decodeCount;
            unsigned int split;

            decodeCount = 0;

            do
            {
                // perform the actual decoding
                split = 1 +  (((range - 1) * model3[8 + decodeCount]) >> 8);

                if ( value >= split << 8 )
                {
                    value -= (split << 8);
                    split = range - split;

                    idx += (1 << decodeCount);

                }

                while (split < 0x80 )
                {
                    split <<= 1;
                    value <<= 1;

                    if ( !--count )
                    {
                        count = 8;
                        value |= *brBuffer++;
                    }
                }

                range = split;
            }
            while (++decodeCount < 6);

            idx += 9;

        }

        cg = vp6_coeff_groups[idx];
        model2 = model->coeff_ract[pt][ct][cg];

        if ( idx >= 64)
        { goto BLOCK_FINISHED; }


    NON_ZERO_RUN_:
        // The token codes a non zero value
        ct = 1;
        NDECODEBOOL_AND_BRANCH_IF_ZERO(model2[ONE_CONTEXT_NODE], AC_ONE_CONTEXT_0_);
        ct = 2;
        // Value token > 1
        NDECODEBOOL_AND_BRANCH_IF_ZERO(model2[LOW_VAL_CONTEXT_NODE], AC_LOW_VAL_CONTEXT_0_);

        // High value (value category) token
        NDECODEBOOL_AND_BRANCH_IF_ZERO(model2[HIGH_LOW_CONTEXT_NODE], AC_HIGH_LOW_CONTEXT_0_);

        // Cat3,Cat4
        NDECODEBOOL_AND_BRANCH_IF_ZERO(model2[CAT_THREEFOUR_CONTEXT_NODE], AC_CAT_THREEFOUR_CONTEXT_0_);

        token = DCT_VAL_CATEGORY5;

        // Cat5,Cat6
        NDECODEBOOL_AND_BRANCH_IF_ZERO(model2[CAT_FIVE_CONTEXT_NODE], AC_EXTRA_BITS_);

        //It is Cat6
        token += 1;

        goto AC_EXTRA_BITS_;

    AC_CAT_THREEFOUR_CONTEXT_0_:
        token = DCT_VAL_CATEGORY3;

        NDECODEBOOL_AND_BRANCH_IF_ZERO(model2[CAT_THREE_CONTEXT_NODE], AC_EXTRA_BITS_);

        //It is Cat4
        token += 1;

        goto AC_EXTRA_BITS_;

    AC_HIGH_LOW_CONTEXT_0_:
        // Either Cat1 or Cat2
        token = DCT_VAL_CATEGORY1;

        NDECODEBOOL_AND_BRANCH_IF_ZERO(model2[CAT_ONE_CONTEXT_NODE], AC_EXTRA_BITS_);

        //It is Cat2
        token += 1;

    AC_EXTRA_BITS_:
        {
            INT32 BitsCount;
            INT32 tValue;

            unsigned int split;

            tValue = VP6_TokenExtraBits2[token].MinVal;

            // Read the extra bits
            BitsCount = VP6_TokenExtraBits2[token].Length;

            do
            {
                //tValue += (NDECODEBOOL(VP6_TokenExtraBits2[token].Probs[BitsCount]) << BitsCount);
                split = 1 +  (((range - 1) * VP6_TokenExtraBits2[token].Probs[BitsCount] ) >> 8);

                if ( value >= split << 8 )
                {
                    value -= (split << 8);
                    split = range - split;

                    tValue += (1 << BitsCount);

                }

                while (split < 0x80 )
                {
                    split <<= 1;
                    value <<= 1;

                    if ( !--count)
                    {
                        count = 8;
                        value |= *brBuffer++;
                    }
                }

                range = split;
            }
            while (--BitsCount >= 0);

            tValue *= s->dequant_ac;
            // apply the sign to the value
            index = model->coeff_index_to_pos[idx];
            APPLYSIGN(s->block_coeff[b][permute[index]], tValue);
            idx ++;
        }

        if ( idx < 64)
        { goto AC_DO_WHILE; }

        goto BLOCK_FINISHED;


    AC_LOW_VAL_CONTEXT_0_:
        // Low value token
        NDECODEBOOL_AND_BRANCH_IF_ZERO(model2[TWO_CONTEXT_NODE], AC_TWO_CONTEXT_0_);

        // Either a 3 or a 4
        token = THREE_TOKEN + 1;

        NDECODEBOOL_AND_BRANCH_IF_ONE(model2[THREE_CONTEXT_NODE], AC_THREE_CONTEXT_1_);

        //It is a 3
        token = token - 1;

    AC_THREE_CONTEXT_1_:
        // apply the sign to the value
        token = token * s->dequant_ac;
        // apply the sign to the value
        index = model->coeff_index_to_pos[idx];
        APPLYSIGN(s->block_coeff[b][permute[index]], token);
        idx ++;

        if ( idx < 64)
        { goto AC_DO_WHILE; }

        goto BLOCK_FINISHED;


    AC_TWO_CONTEXT_0_:

        index = model->coeff_index_to_pos[idx];
        APPLYSIGN(s->block_coeff[b][permute[index]], TWO_TOKEN * s->dequant_ac);
        idx++;

        if ( idx < 64)
        { goto AC_DO_WHILE; }

        goto BLOCK_FINISHED;

    AC_ONE_CONTEXT_0_:
        index = model->coeff_index_to_pos[idx];
        APPLYSIGN(s->block_coeff[b][permute[index]], 1 * s->dequant_ac);

        idx++;


        if ( idx < 64)
        { goto AC_DO_WHILE; }

    BLOCK_FINISHED:
        idx--;

    BLOCK_FINISHED_1:
        s->eob_pos[b] = s->EobOffsetTable[idx];
        s->left_block[vp56_b6to4[b]].not_null_dc =   s->above_blocks[s->above_block_idx[b]].not_null_dc = !!s->block_coeff[b][0];

    }
    while (++b < 6);

    c->buffer = brBuffer;
    c->bits = count;
    c->code_word = value;
    c->high = range;
}

#endif

/*
static int vp6_adjust(int v, int t)
{
    int V = v, s = v >> 31;
    V ^= s;
    V -= s;
    if (V-t-1 >= (unsigned)(t-1))
        return v;
    V = 2*t - V;
    V += s;
    V ^= s;
    return V;
}
*/
#if 0
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

static void vp6_filter_hv4(uint8_t *dst, uint8_t *src, int stride,
                           int delta, const int16_t *weights)
{
    int x, y;

    for (y = 0; y < 8; y++)
    {
        for (x = 0; x < 8; x++)
        {
            dst[x] = av_clip_uint8((  src[x - delta  ] * weights[0]
                                      + src[x        ] * weights[1]
                                      + src[x + delta  ] * weights[2]
                                      + src[x + 2 * delta] * weights[3] + 64) >> 7);
        }

        src += stride;
        dst += stride;
    }
}

static void vp6_filter_diag2(VP56Context *s, uint8_t *dst, uint8_t *src,
                             int stride, int h_weight, int v_weight)
{
    uint8_t *tmp = s->edge_emu_buffer + 16;
    s->dsp.put_h264_chroma_pixels_tab[0](tmp, src, stride, 9, h_weight, 0);
    s->dsp.put_h264_chroma_pixels_tab[0](dst, tmp, stride, 8, 0, v_weight);
}

static void vp6_filter(VP56Context *s, uint8_t *dst, uint8_t *src,
                       int offset1, int offset2, int stride,
                       VP56mv mv, int mask, int select, int luma)
{
    int filter4 = 0;
    int x8 = mv.x & mask;
    int y8 = mv.y & mask;

    if (luma)
    {
        x8 *= 2;
        y8 *= 2;
        filter4 = s->filter_mode;

        if (filter4 == 2)
        {
            if (s->max_vector_length &&
                (FFABS(mv.x) > s->max_vector_length ||
                 FFABS(mv.y) > s->max_vector_length))
            {
                filter4 = 0;
            }
            else if (s->sample_variance_threshold
                     && (vp6_block_variance(src + offset1, stride)
                         < s->sample_variance_threshold))
            {
                filter4 = 0;
            }
        }
    }

    if ((y8 && (offset2 - offset1)*s->flip < 0) || (!y8 && offset1 > offset2))
    {
        offset1 = offset2;
    }

    if (filter4)
    {
#ifndef ARM_ASM_OPT

        if (!y8)                        /* left or right combine */
        {
            vp6_filter_hv4(dst, src + offset1, stride, 1,
                           vp6_block_copy_filter[select][x8]);
        }
        else if (!x8)                   /* above or below combine */
        {
            vp6_filter_hv4(dst, src + offset1, stride, stride,
                           vp6_block_copy_filter[select][y8]);
#else

        if (!y8)                        /* left or right combine */
        {
            vp6_filter_hv4asm_h(dst, src + offset1, stride, 1,
                                vp6_block_copy_filter[select][x8]);
        }
        else if (!x8)                   /* above or below combine */
        {
            vp6_filter_hv4asm_v(dst, src + offset1, stride, stride,
                                vp6_block_copy_filter[select][y8]);
#endif

        }
        else
        {
#ifndef ARM_ASM_OPT
            s->dsp.vp6_filter_diag4(dst, src + offset1 + ((mv.x ^ mv.y) >> 31), stride,
                                    vp6_block_copy_filter[select][x8],
                                    vp6_block_copy_filter[select][y8]);
#else
            vp6_filter_hv4asm_armv6(dst, src + offset1 + ((mv.x ^ mv.y) >> 31), stride,
                                    vp6_block_copy_filter[select][x8],
                                    vp6_block_copy_filter[select][y8]);
#endif
        }
    }
    else
    {
        if (!x8 || !y8)
        {
            s->dsp.put_h264_chroma_pixels_tab[0](dst, src + offset1, stride, 8, x8, y8);
        }
        else
        {
            vp6_filter_diag2(s, dst, src + offset1 + ((mv.x ^ mv.y) >> 31), stride, x8, y8);
        }
    }
}

#endif
static av_cold int vp6_decode_init(AVCodecContext *avctx)
{
    int stride;
    int i;
    int ret;
    VP56Context *s = avctx->priv_data;

    /*x00141957 20100914*/
    //memset(s,0,sizeof(VP56Context));

    avctx->width = avctx->usSourceWidth;
    avctx->height = avctx->usSourceHeight;

    ret = vp56_init(avctx, avctx->codec->id == CODEC_ID_VP6,
                    avctx->codec->id == CODEC_ID_VP6A);

    /*x00141957 20100914,如果初始化所有buffer失败，则返回*/
    if (ret < 0)
    {
        av_log(avctx, AV_LOG_ERROR, "get yuv buffer malloc failed!\n");
        return -1;
    }

    s->vp56_coord_div = vp6_coord_div;
    s->parse_vector_adjustment = vp6_parse_vector_adjustment;
    /*x00141957 2010 1127*/
    // s->adjust = vp6_adjust;
    //s->filter = vp6_filter;
    s->default_models_init = vp6_default_models_init;
    s->parse_vector_models = vp6_parse_vector_models;
    s->parse_coeff_models = vp6_parse_coeff_models;
    s->parse_header = vp6_parse_header;

    /*x00141957 20100630*/
    /* 新增代码: 给parse.buffer分配内存 改到parse frame中*/
    // 	if(NULL == s->parse_context.buffer)
    // 	{
    // 		s->parse_context.buffer_size = avctx->usSourceWidth*avctx->usSourceHeight*2;
    // 		s->parse_context.buffer = av_malloc_hw(s->parse_context.buffer_size);
    //
    /*增加codec的内存分配，由配置的静态参数中的最大宽高决定*/
    stride = s->framep[VP56_FRAME_CURRENT]->linesize[0];

    /*初始化coded_width为静态配置宽高 x00141957 20100914*/
    avctx->coded_width = avctx->usSourceWidth;
    avctx->coded_height = avctx->usSourceHeight;

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

    if (NULL == s->above_blocks)
    {
        s->above_blocks = av_malloc_hw(avctx, (4 * s->mb_width + 6) * sizeof(*s->above_blocks));
    }

    if (NULL == s->macroblocks)
    {
        s->macroblocks = av_malloc_hw(avctx, s->mb_width * s->mb_height * sizeof(*s->macroblocks));
    }

    if (NULL == s->edge_emu_buffer_alloc)
    {
        /*将av_malloc改为av_malloc_hw x00141957 20100701*/
        s->edge_emu_buffer_alloc = av_malloc_hw(avctx, 16 * stride);
    }

    //av_free(s->edge_emu_buffer_alloc);

    /*x00141957 20100914 判断上述内存分配是否成功 如果没有则释放所有已分配内存，并且返回-1*/
    if ((NULL == s->above_blocks) || (NULL == s->macroblocks) || (NULL == s->edge_emu_buffer_alloc))
    {
        av_log(avctx, AV_LOG_ERROR, "decode buffer malloc failed!\n");
        vp56_free(avctx);
        return -1;
    }

    s->edge_emu_buffer = s->edge_emu_buffer_alloc;

    if (s->flip < 0)
    { s->edge_emu_buffer += 15 * stride; }

    return 0;
}

#if 1
#if 0
/* 清空YUV buffer队列和状态信息 x00141957 20100702新增函数*/
static void ff_vp6_flush(AVCodecContext *avctx)
{
    int i;
    VP56Context *s = avctx->priv_data;

    if (s == NULL)
    { return; }

    /* 清空YUV buffer队列 */
    for (i = 0; i < MAX_PICTURE_COUNT; i++)
    {
        if (s->picture[i].data[0] && (   s->picture[i].type == FF_BUFFER_TYPE_INTERNAL
                                         || s->picture[i].type == FF_BUFFER_TYPE_USER))
        { avctx->release_buffer(avctx, (AVFrame *)&s->picture[i]); }
    }

    s->current_picture_ptr = s->last_picture_ptr = s->next_picture_ptr = NULL;

    /* 清空状态信息 */
    s->mb_x = s->mb_y = 0;

    s->parse_context.state = -1;
    s->parse_context.frame_start_found = 0;
    s->parse_context.overread = 0;
    s->parse_context.overread_index = 0;
    s->parse_context.index = 0;
    s->parse_context.last_index = 0;
    s->bitstream_buffer_size = 0;
    s->pp_time = 0;
}
#endif
/* 新增函数：复位VP6解码器 xiongfei20100702*/
/*x00141957 20101230 因为内核态编译栈空间过大，所以改写此函数，不再对vp56context结构体清零*/
static av_cold int vp6_reset(VP56Context *h)
{
    int pt, ct, cg;
    int i = 0;
    int iRet = 0;

    AVCodecContext *avctx	= h->avctx;

    /*保存前次分配的内存 x00141957 20100915*/
    VP56RefDc *above_blocks = h->above_blocks;
    VP56Macroblock *macroblocks = h->macroblocks;
    uint8_t *edge_emu_buffer_alloc = h->edge_emu_buffer_alloc;
    uint8_t *buffer = h->parse_context.buffer ;
    int buffer_size = h->parse_context.buffer_size;


    /* 清空VP56Context结构体变量 xionigfei20100702*/
    memset(h, 0, sizeof(VP56Context));

    /*保存前次分配的内存 x00141957 20100915*/
    h->above_blocks = above_blocks;
    h->macroblocks = macroblocks;
    h->edge_emu_buffer_alloc = edge_emu_buffer_alloc;
    h->parse_context.buffer = buffer;
    h->parse_context.buffer_size = buffer_size;
    h->parse_context.state = -1;
    /* 初始化AVCodecContext结构体变量 xiongfei20100702*/
    iRet = vp6_decode_init(avctx);

    if (0 != iRet)
    {
        av_log(avctx, AV_LOG_ERROR, "ctx_dec[%p] mpeg2_ctx[%p] failed! return code: %d !\n", avctx, h, iRet);
    }

    return iRet;
}

/*增加decode_reset函数，用于control中的reset xiongfei20100702*/
static av_cold int decode_reset(AVCodecContext *avctx)
{
    int iRet = 0;
    VP56Context *h = avctx->priv_data;

    /* 清空buffer x00141957 20100702*/
    //ff_mpeg_flush(avctx);

    /*问题单号：AZ1D02230
    修改人：熊飞 +00141957
    时间：2010/7/21
    问题描述： 多次creat process reset导致死机
    问题修改： 增加vp56_free释放内存*/
    //vp56_free(avctx);

    /* 复位及初始化AVCodecContext结构体变量 xiongfei20100702*/
    iRet = avcodec_reset(avctx);

    if (0 != iRet)
    {
        av_log(avctx, AV_LOG_ERROR, "ctx_dec[%p] avcodec_reset() failed! return code: %d !\n", avctx, iRet);
        return iRet;
    }

    /* 复位及初始化VP56Context结构体变量 xiongfei20100702*/
    iRet = vp6_reset(h);

    if (0 != iRet)
    {
        av_log(avctx, AV_LOG_ERROR, "ctx_dec[%p] VP6_ctx[%p] failed! return code: %d !\n", avctx, h, iRet);
        return iRet;
    }

    return iRet;
}
#endif

#undef  ALIGN
#define ALIGN(x, a) (((x)+(a)-1)&~((a)-1))

static void vp6_align_dimensions(int pix_format, int *width, int *height)
{
    int w_align = 1;
    int h_align = 1;

    switch (pix_format)
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
            break;

        case PIX_FMT_RGB555:
            break;

        case PIX_FMT_PAL8:
        case PIX_FMT_BGR8:
        case PIX_FMT_RGB8:
            break;

        case PIX_FMT_BGR24:
            break;

        default:
            w_align = 1;
            h_align = 1;
            break;
    }

    *width = ALIGN(*width , w_align);
    *height = ALIGN(*height, h_align);
}

int vp6_fill_pointer(AVPicture *picture, uint8_t *ptr, int pix_fmt,
                     int height)
{
    int size, h2, size2;

    size = picture->linesize[0] * height;

    switch (pix_fmt)
    {
        case PIX_FMT_YUV420P:
        case PIX_FMT_YUV422P:
        case PIX_FMT_YUV444P:
        case PIX_FMT_YUV410P:
        case PIX_FMT_YUV411P:
        case PIX_FMT_YUV440P:
        case PIX_FMT_YUVJ420P:
        case PIX_FMT_YUVJ422P:
        case PIX_FMT_YUVJ444P:
        case PIX_FMT_YUVJ440P:
            h2 = (height + (1 << 1) - 1) >> 1;
            size2 = picture->linesize[1] * h2;
            picture->data[0] = ptr;
            picture->data[1] = picture->data[0] + size;
            picture->data[2] = picture->data[1] + size2;
            picture->data[3] = NULL;
            return size + 2 * size2;

        default:
            picture->data[0] = NULL;
            picture->data[1] = NULL;
            picture->data[2] = NULL;
            picture->data[3] = NULL;
            return -1;
    }
}

int vp6_fill_linesize(AVPicture *picture, int pix_fmt, int width)
{
    int w2;

    memset(picture->linesize, 0, sizeof(picture->linesize));

    switch (pix_fmt)
    {
        case PIX_FMT_YUV420P:
        case PIX_FMT_YUV422P:
        case PIX_FMT_YUV444P:
        case PIX_FMT_YUV410P:
        case PIX_FMT_YUV411P:
        case PIX_FMT_YUV440P:
        case PIX_FMT_YUVJ420P:
        case PIX_FMT_YUVJ422P:
        case PIX_FMT_YUVJ444P:
        case PIX_FMT_YUVJ440P:
            w2 = (width + (1 << 1) - 1) >> 1;
            picture->linesize[0] = width;
            picture->linesize[1] = w2;
            picture->linesize[2] = w2;
            break;

        default:
            return -1;
    }

    return 0;
}

/*x00141957 20100926*/
/*查询算法所需要内存空间大小*/
int vp6_query_memsize(uint16_t width, uint16_t height, uint16_t buffernumber, int pix_format)
{
    int yuvbufsize;
    int decsize;
    int linesize;
    int w = (int)width;
    int h = (int)height;
    int VP56Contextsize;
    int internalbuffersize;
    int vlcsize;

    VP56Contextsize = sizeof(VP56Context) + 16;
    internalbuffersize = (INTERNAL_BUFFER_SIZE + 1) * sizeof(InternalBuffer) + 16;
    yuvbufsize = 0;
    /*计算单个yuvbuffer所需大小*/
    {
        int h_chroma_shift, v_chroma_shift;
        int size[4] = {0};
        int tmpsize;
        AVPicture picture;
        int stride_align[4];
        int i;
        int ret;

        avcodec_get_chroma_sub_sample(pix_format, &h_chroma_shift, &v_chroma_shift);

        vp6_align_dimensions(pix_format, &w, &h);

        w += EDGE_WIDTH * 2;
        h += EDGE_WIDTH * 2;

        ret = vp6_fill_linesize(&picture, pix_format, w);

        for (i = 0; i < 4; i++)
        {
            stride_align[i] = STRIDE_ALIGN;
            picture.linesize[i] = ALIGN(picture.linesize[i], stride_align[i]);
        }

        tmpsize = vp6_fill_pointer(&picture, NULL, pix_format, h);

        if (tmpsize < 0)
        { return -1; }

        for (i = 0; i < 3 && picture.data[i + 1]; i++)
        { size[i] = picture.data[i + 1] - picture.data[i]; }

        size[i] = tmpsize - (picture.data[i] - picture.data[0]);

        /*x00141957 20100914 对于size[0] 分配y+0.5*y大小空间*/
        /*x00141957 20110726 对于size[0] 增加分配y大小空间*/
        size[0] = (size[0] << 1) + (size[0] >> 1);

        for (i = 0; i < 4 && size[i]; i++)
        {
            yuvbufsize += size[i];
        }

        linesize = picture.linesize[0];
        yuvbufsize += 16;
    }
    yuvbufsize *= (int)buffernumber;

    /*计算解码所需buffer大小*/
    decsize = 0;
    {
        int stride;
        int mb_width, mb_height;
        int above_blocks_size, macroblocks_size, edge_emu_buffer_all0c_size;

        stride = linesize;

        mb_width  = ((int)width + 15) / 16;
        mb_height = ((int)height + 15) / 16;

        if (mb_width > 1000 || mb_height > 1000)
        {
            av_log(NULL, AV_LOG_ERROR, "picture too big\n");
            return -1;
        }

        above_blocks_size = (4 * mb_width + 6) * sizeof(VP56RefDc) + 16;
        macroblocks_size = mb_width * mb_height * sizeof(VP56Macroblock) + 16;
        edge_emu_buffer_all0c_size = 16 * stride + 16;

        decsize += VP56Contextsize;
        decsize += internalbuffersize;
        decsize += above_blocks_size;
        decsize += macroblocks_size;
        decsize += edge_emu_buffer_all0c_size;
    }
    vlcsize = 0;
    {
        vlcsize = 40 * (sizeof(VLC_TYPE) * 2 * (1 << 12) + 16);
    }
    return (yuvbufsize + decsize + vlcsize);
}
AVCodec vp6f_decoder =
{
    "vp6f",
    CODEC_TYPE_VIDEO,
    CODEC_ID_VP6F,
    sizeof(VP56Context),
    vp6_decode_init,
    NULL,
    vp56_free,
    vp56_decode_frame,
    CODEC_CAP_DRAW_HORIZ_BAND | CODEC_CAP_DR1 | CODEC_CAP_TRUNCATED | CODEC_CAP_DELAY,
    NULL,
    ff_mpeg_flush,
    NULL,
    NULL,
    NULL_IF_CONFIG_SMALL("On2 VP6 (Flash version)"),
    NULL,
    NULL,
    NULL,
    vp6_Frame_Parse,
    decode_reset,
    vp6_query_memsize
};

AVCodec vp6_decoder =
{
    "vp6",
    CODEC_TYPE_VIDEO,
    CODEC_ID_VP6,
    sizeof(VP56Context),
    vp6_decode_init,
    NULL,
    vp56_free,
    vp56_decode_frame,
    CODEC_CAP_DRAW_HORIZ_BAND | CODEC_CAP_DR1 | CODEC_CAP_TRUNCATED | CODEC_CAP_DELAY,
    NULL,
    ff_mpeg_flush,
    NULL,
    NULL,
    NULL_IF_CONFIG_SMALL("On2 VP6 (Flip version)"),
    NULL,
    NULL,
    NULL,
    vp6_Frame_Parse,
    decode_reset,
    vp6_query_memsize
};


AVCodec vp6a_decoder =
{
    "vp6a",
    CODEC_TYPE_VIDEO,
    CODEC_ID_VP6A,
    sizeof(VP56Context),
    vp6_decode_init,
    NULL,
    vp56_free,
    vp56_decode_frame,
    CODEC_CAP_DRAW_HORIZ_BAND | CODEC_CAP_DR1 | CODEC_CAP_TRUNCATED | CODEC_CAP_DELAY,
    NULL,
    ff_mpeg_flush,
    NULL,
    NULL,
    NULL_IF_CONFIG_SMALL("On2 VP6  (Flash version, with alpha channel)"),
    NULL,
    NULL,
    NULL,
    vp6_Frame_Parse,
    decode_reset,
    vp6_query_memsize
};

// AVCodec vp6_decoder = {
//     "vp6",
//     CODEC_TYPE_VIDEO,
//     CODEC_ID_VP6,
//     sizeof(VP56Context),
//     vp6_decode_init,
//     NULL,
//     vp56_free,
//     vp56_decode_frame,
//     CODEC_CAP_DR1,
//   //  .long_name = NULL_IF_CONFIG_SMALL("On2 VP6"),
// };
//
// /* flash version, not flipped upside-down */
// AVCodec vp6f_decoder = {
//     "vp6f",
//     CODEC_TYPE_VIDEO,
//     CODEC_ID_VP6F,
//     sizeof(VP56Context),
//     vp6_decode_init,
//     NULL,
//     vp56_free,
//     vp56_decode_frame,
//     CODEC_CAP_DR1,
//  //   .long_name = NULL_IF_CONFIG_SMALL("On2 VP6 (Flash version)"),
// };
//
// /* flash version, not flipped upside-down, with alpha channel */
// AVCodec vp6a_decoder = {
//     "vp6a",
//     CODEC_TYPE_VIDEO,
//     CODEC_ID_VP6A,
//     sizeof(VP56Context),
//     vp6_decode_init,
//     NULL,
//     vp56_free,
//     vp56_decode_frame,
//     CODEC_CAP_DR1,
//  //   .long_name = NULL_IF_CONFIG_SMALL("On2 VP6 (Flash version, with alpha channel)"),
// };
