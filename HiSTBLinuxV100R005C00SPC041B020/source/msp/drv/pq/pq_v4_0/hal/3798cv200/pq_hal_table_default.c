/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_table_default.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2015/10/30
  Description   :

******************************************************************************/

#include "hi_drv_pq.h"
#include "pq_hal_table_default.h"

static PQ_PHY_REG_S sg_stPhyRegDefault[PHY_REG_MAX] =
{
    /*Addr         Lsb       Msb  SourceMode  OutputMode   Module     Value        Description*/
    /***********************************************ACM_CTRL***********************************************/
    /*ACM_CTRL*/
    {0x61f0,    31,      31,        0,      0,      HI_PQ_MODULE_COLOR,       0},     //acm_en
    {0x61f0,    30,      30,        0,      0,      HI_PQ_MODULE_COLOR,       0},     //acm_dbg_en
    {0x61f0,    29,      29,        0,      0,      HI_PQ_MODULE_COLOR,       1},     //acm_stretch
    {0x61f0,    28,      28,        0,      0,      HI_PQ_MODULE_COLOR,       1},     //acm_cliprange
    {0x61f0,    27,      27,        0,      0,      HI_PQ_MODULE_COLOR,       0},     //acm_cliporwrap
    {0x61f0,    10,      21,        0,      0,      HI_PQ_MODULE_COLOR,       0},     //acm_dbg_pos
    {0x61f0,     9,       9,        0,      0,      HI_PQ_MODULE_COLOR,       0},     //acm_dbg_mode
    {0x61f0,     0,       8,        0,      0,      HI_PQ_MODULE_COLOR,       0},     //acm_cbcrthr
    /*ACM_GAIN*/
    {0x61f4,    20,      29,        0,      0,      HI_PQ_MODULE_COLOR,      80},     //acm_gain_luma
    {0x61f4,    10,      19,        0,      0,      HI_PQ_MODULE_COLOR,      80},     //acm_gain_hue
    {0x61f4,     0,       9,        0,      0,      HI_PQ_MODULE_COLOR,      80},     //acm_gain_sat

    /***********************************************SHARPEN***********************************************/
    /*SD source*/
    /*SPCTRL*/
    {0x6200,    17,      17,        1,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //ensctrl
    {0x6200,     4,      16,        1,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //demo_pos
    {0x6200,     2,       3,        1,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //demo_mode
    {0x6200,     1,       1,        1,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //sharpen_en
    {0x6200,     0,       0,        1,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //debug_en
    /*SPENABLE*/
    {0x6204,     4,       4,        1,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //hfreq_en_d
    {0x6204,     3,       3,        1,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //hfreq_en_v
    {0x6204,     2,       2,        1,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //hfreq_en_h
    {0x6204,     1,       1,        1,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //detec_en
    {0x6204,     0,       0,        1,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //peak_en
    /*SPWINSIZEHVD*/
    {0x6208,     6,       8,        1,      0,      HI_PQ_MODULE_SHARPNESS,   2},     //winsize_d
    {0x6208,     3,       5,        1,      0,      HI_PQ_MODULE_SHARPNESS,   2},     //winsize_v
    {0x6208,     0,       2,        1,      0,      HI_PQ_MODULE_SHARPNESS,   2},     //winsize_h
    /*SPTMPH1*/
    {0x620c,    16,      24,        1,      0,      HI_PQ_MODULE_SHARPNESS,  78},     //tmp2_h1
    {0x620c,     8,      15,        1,      0,      HI_PQ_MODULE_SHARPNESS,  61},     //tmp1_h1
    {0x620c,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS,  28},     //tmp0_h1
    /*SPTMPH2*/
    {0x6210,    16,      24,        1,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //tmp2_h2
    {0x6210,     8,      15,        1,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //tmp1_h2
    {0x6210,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS, -64},     //tmp0_h2
    /*SPTMPV1*/
    {0x6214,    16,      24,        1,      0,      HI_PQ_MODULE_SHARPNESS,  78},     //tmp2_v1
    {0x6214,     8,      15,        1,      0,      HI_PQ_MODULE_SHARPNESS,  61},     //tmp1_v1
    {0x6214,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS,  28},     //tmp0_v1
    /*SPTMPV2*/
    {0x6218,    16,      24,        1,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //tmp2_v2
    {0x6218,     8,      15,        1,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //tmp1_v2
    {0x6218,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS, -64},     //tmp0_v2
    /*SPGAIN*/
    {0x621c,    20,      27,        1,      0,      HI_PQ_MODULE_SHARPNESS,   8},     //dir_gain
    {0x621c,    10,      19,        1,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //edge_gain
    {0x621c,     0,       9,        1,      0,      HI_PQ_MODULE_SHARPNESS, 256},     //peak_gain
    {0x6220,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS,  20},     //peak_coring
    /*SPPEAKSHOOTCTL*/
    {0x6224,    16,      23,        1,      0,      HI_PQ_MODULE_SHARPNESS,  96},     //peak_mixratio
    {0x6224,     8,      15,        1,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //peak_under_thr
    {0x6224,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //peak_over_thr
    /*SPGAINPOS*/
    {0x6228,    18,      26,        1,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_pos_d
    {0x6228,     9,      17,        1,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_pos_v
    {0x6228,     0,       8,        1,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_pos_h
    /*SPGAINNEG*/
    {0x622c,    18,      26,        1,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_neg_d
    {0x622c,     9,      17,        1,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_neg_v
    {0x622c,     0,       8,        1,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_neg_h
    /*SPOVERTHR*/
    {0x6230,    16,      23,        1,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //over_thr_d
    {0x6230,     8,      15,        1,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //over_thr_v
    {0x6230,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //over_thr_h
    /*SPUNDERTHR*/
    {0x6234,    16,      23,        1,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //under_thr_d
    {0x6234,     8,      15,        1,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //under_thr_v
    {0x6234,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //under_thr_h
    /*SPMIXRATIO*/
    {0x6238,    16,      23,        1,      0,      HI_PQ_MODULE_SHARPNESS,  96},     //mixratio_d
    {0x6238,     8,      15,        1,      0,      HI_PQ_MODULE_SHARPNESS,  96},     //mixratio_v
    {0x6238,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS,  96},     //mixratio_h
    /*SPCORINGZERO*/
    {0x623c,     6,      23,        1,      0,      HI_PQ_MODULE_SHARPNESS,   2},     //coringzero_d
    {0x623c,     8,      15,        1,      0,      HI_PQ_MODULE_SHARPNESS,   3},     //coringzero_v
    {0x623c,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS,   3},     //coringzero_h
    /*SPCORINGRATIO*/
    {0x6240,    10,      14,        1,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //coringratio_d
    {0x6240,     5,       9,        1,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //coringratio_v
    {0x6240,     0,       4,        1,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //coringratio_h
    /*SPCORINGTHR*/
    {0x6244,    16,      23,        1,      0,      HI_PQ_MODULE_SHARPNESS,   7},     //coringthr_d
    {0x6244,     8,      15,        1,      0,      HI_PQ_MODULE_SHARPNESS,   7},     //coringthr_v
    {0x6244,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS,   7},     //coringthr_h
    /*SPLMTRATIO*/
    {0x6248,     6,       8,        1,      0,      HI_PQ_MODULE_SHARPNESS,   6},     //limit_ratio_d
    {0x6248,     3,       5,        1,      0,      HI_PQ_MODULE_SHARPNESS,   6},     //limit_ratio_v
    {0x6248,     0,       2,        1,      0,      HI_PQ_MODULE_SHARPNESS,   6},     //limit_ratio_h
    /*SPLMTPOS0*/
    {0x624c,    16,      23,        1,      0,      HI_PQ_MODULE_SHARPNESS,  64},     //limitpos0_d
    {0x624c,     8,      15,        1,      0,      HI_PQ_MODULE_SHARPNESS,  64},     //limitpos0_v
    {0x624c,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS,  64},     //limitpos0_h
    /*SPLMTPOS1*/
    {0x6250,    16,      23,        1,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //limitpos1_d
    {0x6250,     8,      15,        1,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //limitpos1_v
    {0x6250,     0,       7,        1,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //limitpos1_h
    /*SPBOUNDPOS*/
    {0x6254,    16,      23,        1,        0,    HI_PQ_MODULE_SHARPNESS,  112},    //boundpos_d
    {0x6254,     8,      15,        1,        0,    HI_PQ_MODULE_SHARPNESS,  112},    //boundpos_v
    {0x6254,     0,       7,        1,        0,    HI_PQ_MODULE_SHARPNESS,  112},    //boundpos_h
    /*SPLMTNEG0*/
    {0x6258,    18,      26,        1,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //limitneg0_d
    {0x6258,     9,      17,        1,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //limitneg0_v
    {0x6258,     0,       8,        1,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //limitneg0_h
    /*SPLMTNEG1*/
    {0x625c,    18,      26,        1,        0,    HI_PQ_MODULE_SHARPNESS, -128},    //limitneg1_d
    {0x625c,     9,      17,        1,        0,    HI_PQ_MODULE_SHARPNESS, -128},    //limitneg1_v
    {0x625c,     0,       8,        1,        0,    HI_PQ_MODULE_SHARPNESS, -128},    //limitneg1_h
    /*SPBOUNDNEG*/
    {0x6260,    18,      26,        1,        0,    HI_PQ_MODULE_SHARPNESS, -112},    //boundneg_d
    {0x6260,     9,      17,        1,        0,    HI_PQ_MODULE_SHARPNESS, -112},    //boundneg_v
    {0x6260,     0,       8,        1,        0,    HI_PQ_MODULE_SHARPNESS, -112},    //boundneg_h
    /*SPEDGESHOOTCTL*/
    {0x6264,    16,      23,        1,        0,    HI_PQ_MODULE_SHARPNESS,  128},    //edge_mixratio
    {0x6264,     8,      15,        1,        0,    HI_PQ_MODULE_SHARPNESS,   10},    //edge_under_thr
    {0x6264,     0,       7,        1,        0,    HI_PQ_MODULE_SHARPNESS,   10},    //edge_over_thr
    /*SPEDGETMPH*/
    {0x6268,    16,      24,        1,        0,    HI_PQ_MODULE_SHARPNESS,  128},    //tmp2_sx
    {0x6268,     8,      15,        1,        0,    HI_PQ_MODULE_SHARPNESS,    0},    //tmp1_sx
    {0x6268,     0,       7,        1,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //tmp0_sx
    /*SPEDGETMPV*/
    {0x626c,    16,      24,        1,        0,    HI_PQ_MODULE_SHARPNESS,  128},    //tmp2_sy
    {0x626c,     8,      15,        1,        0,    HI_PQ_MODULE_SHARPNESS,    0},    //tmp1_sy
    {0x626c,     0,       7,        1,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //tmp0_sy
    /*SPEDGEEI*/
    {0x6270,    16,      23,        1,        0,    HI_PQ_MODULE_SHARPNESS,    4},    //edge_eiratio
    {0x6270,     8,      15,        1,        0,    HI_PQ_MODULE_SHARPNESS,  100},    //edge_ei1
    {0x6270,     0,       7,        1,        0,    HI_PQ_MODULE_SHARPNESS,   68},    //edge_ei0
    /*SPGRADMIN*/
    {0x6274,    20,      26,        1,        0,    HI_PQ_MODULE_SHARPNESS,    4},    //gradminslop
    {0x6274,    10,      19,        1,        0,    HI_PQ_MODULE_SHARPNESS,   40},    //gradminthrh
    {0x6274,     0,       9,        1,        0,    HI_PQ_MODULE_SHARPNESS,    8},    //gradminthrl
    /*SPGRADCTR*/
    {0x6278,    10,      25,        1,        0,    HI_PQ_MODULE_SHARPNESS, 1600},    //chessboardthr
    {0x6278,     0,       9,        1,        0,    HI_PQ_MODULE_SHARPNESS,   16},    //gradminthr
    /*SPSHOOTRATIO*/
    {0x627c,    21,      27,        1,        0,    HI_PQ_MODULE_SHARPNESS,  127},    //oshootratiot
    {0x627c,    14,      20,        1,        0,    HI_PQ_MODULE_SHARPNESS,  127},    //oshootratioe
    {0x627c,     7,      13,        1,        0,    HI_PQ_MODULE_SHARPNESS,  16},     //ushootratiot
    {0x627c,     0,       6,        1,        0,    HI_PQ_MODULE_SHARPNESS,  16},     //ushootratioe
    /*SPGRADCONTRAST*/
    {0x6280,     8,      15,        1,        0,    HI_PQ_MODULE_SHARPNESS,  64},     //gradcontrastthr
    {0x6280,     0,      7,         1,        0,    HI_PQ_MODULE_SHARPNESS,   4},     //gradcontrastslop

    /*HD source*/
    /*SPCTRL*/
    {0x6200,    17,      17,        2,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //ensctrl
    {0x6200,     4,      16,        2,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //demo_pos
    {0x6200,     2,       3,        2,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //demo_mode
    {0x6200,     1,       1,        2,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //sharpen_en
    {0x6200,     0,       0,        2,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //debug_en
    /*SPENABLE*/
    {0x6204,     4,       4,        2,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //hfreq_en_d
    {0x6204,     3,       3,        2,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //hfreq_en_v
    {0x6204,     2,       2,        2,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //hfreq_en_h
    {0x6204,     1,       1,        2,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //detec_en
    {0x6204,     0,       0,        2,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //peak_en
    /*SPWINSIZEHVD*/
    {0x6208,     6,       8,        2,      0,      HI_PQ_MODULE_SHARPNESS,   2},     //winsize_d
    {0x6208,     3,       5,        2,      0,      HI_PQ_MODULE_SHARPNESS,   2},     //winsize_v
    {0x6208,     0,       2,        2,      0,      HI_PQ_MODULE_SHARPNESS,   2},     //winsize_h
    /*SPTMPH1*/
    {0x620c,    16,      24,        2,      0,      HI_PQ_MODULE_SHARPNESS,  78},     //tmp2_h1
    {0x620c,     8,      15,        2,      0,      HI_PQ_MODULE_SHARPNESS,  61},     //tmp1_h1
    {0x620c,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS,  28},     //tmp0_h1
    /*SPTMPH2*/
    {0x6210,    16,      24,        2,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //tmp2_h2
    {0x6210,     8,      15,        2,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //tmp1_h2
    {0x6210,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS, -64},     //tmp0_h2
    /*SPTMPV1*/
    {0x6214,    16,      24,        2,      0,      HI_PQ_MODULE_SHARPNESS,  78},     //tmp2_v1
    {0x6214,     8,      15,        2,      0,      HI_PQ_MODULE_SHARPNESS,  61},     //tmp1_v1
    {0x6214,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS,  28},     //tmp0_v1
    /*SPTMPV2*/
    {0x6218,    16,      24,        2,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //tmp2_v2
    {0x6218,     8,      15,        2,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //tmp1_v2
    {0x6218,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS, -64},     //tmp0_v2
    /*SPGAIN*/
    {0x621c,    20,      27,        2,      0,      HI_PQ_MODULE_SHARPNESS,   8},     //dir_gain
    {0x621c,    10,      19,        2,      0,      HI_PQ_MODULE_SHARPNESS,  40},     //edge_gain
    {0x621c,     0,       9,        2,      0,      HI_PQ_MODULE_SHARPNESS,  80},     //peak_gain
    {0x6220,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS,  20},     //peak_coring
    /*SPPEAKSHOOTCTL*/
    {0x6224,    16,      23,        2,      0,      HI_PQ_MODULE_SHARPNESS,  96},     //peak_mixratio
    {0x6224,     8,      15,        2,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //peak_under_thr
    {0x6224,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //peak_over_thr
    /*SPGAINPOS*/
    {0x6228,    18,      26,        2,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_pos_d
    {0x6228,     9,      17,        2,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_pos_v
    {0x6228,     0,       8,        2,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_pos_h
    /*SPGAINNEG*/
    {0x622c,    18,      26,        2,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_neg_d
    {0x622c,     9,      17,        2,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_neg_v
    {0x622c,     0,       8,        2,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_neg_h
    /*SPOVERTHR*/
    {0x6230,    16,      23,        2,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //over_thr_d
    {0x6230,     8,      15,        2,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //over_thr_v
    {0x6230,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //over_thr_h
    /*SPUNDERTHR*/
    {0x6234,    16,      23,        2,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //under_thr_d
    {0x6234,     8,      15,        2,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //under_thr_v
    {0x6234,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //under_thr_h
    /*SPMIXRATIO*/
    {0x6238,    16,      23,        2,      0,      HI_PQ_MODULE_SHARPNESS,  96},     //mixratio_d
    {0x6238,     8,      15,        2,      0,      HI_PQ_MODULE_SHARPNESS,  96},     //mixratio_v
    {0x6238,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS,  96},     //mixratio_h
    /*SPCORINGZERO*/
    {0x623c,     6,      23,        2,      0,      HI_PQ_MODULE_SHARPNESS,   2},     //coringzero_d
    {0x623c,     8,      15,        2,      0,      HI_PQ_MODULE_SHARPNESS,   3},     //coringzero_v
    {0x623c,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS,   3},     //coringzero_h
    /*SPCORINGRATIO*/
    {0x6240,    10,      14,        2,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //coringratio_d
    {0x6240,     5,       9,        2,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //coringratio_v
    {0x6240,     0,       4,        2,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //coringratio_h
    /*SPCORINGTHR*/
    {0x6244,    16,      23,        2,      0,      HI_PQ_MODULE_SHARPNESS,   7},     //coringthr_d
    {0x6244,     8,      15,        2,      0,      HI_PQ_MODULE_SHARPNESS,   7},     //coringthr_v
    {0x6244,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS,   7},     //coringthr_h
    /*SPLMTRATIO*/
    {0x6248,     6,       8,        2,      0,      HI_PQ_MODULE_SHARPNESS,   6},     //limit_ratio_d
    {0x6248,     3,       5,        2,      0,      HI_PQ_MODULE_SHARPNESS,   6},     //limit_ratio_v
    {0x6248,     0,       2,        2,      0,      HI_PQ_MODULE_SHARPNESS,   6},     //limit_ratio_h
    /*SPLMTPOS0*/
    {0x624c,    16,      23,        2,      0,      HI_PQ_MODULE_SHARPNESS,  64},     //limitpos0_d
    {0x624c,     8,      15,        2,      0,      HI_PQ_MODULE_SHARPNESS,  64},     //limitpos0_v
    {0x624c,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS,  64},     //limitpos0_h
    /*SPLMTPOS1*/
    {0x6250,    16,      23,        2,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //limitpos1_d
    {0x6250,     8,      15,        2,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //limitpos1_v
    {0x6250,     0,       7,        2,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //limitpos1_h
    /*SPBOUNDPOS*/
    {0x6254,    16,      23,        2,        0,    HI_PQ_MODULE_SHARPNESS,  112},    //boundpos_d
    {0x6254,     8,      15,        2,        0,    HI_PQ_MODULE_SHARPNESS,  112},    //boundpos_v
    {0x6254,     0,       7,        2,        0,    HI_PQ_MODULE_SHARPNESS,  112},    //boundpos_h
    /*SPLMTNEG0*/
    {0x6258,    18,      26,        2,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //limitneg0_d
    {0x6258,     9,      17,        2,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //limitneg0_v
    {0x6258,     0,       8,        2,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //limitneg0_h
    /*SPLMTNEG1*/
    {0x625c,    18,      26,        2,        0,    HI_PQ_MODULE_SHARPNESS, -128},    //limitneg1_d
    {0x625c,     9,      17,        2,        0,    HI_PQ_MODULE_SHARPNESS, -128},    //limitneg1_v
    {0x625c,     0,       8,        2,        0,    HI_PQ_MODULE_SHARPNESS, -128},    //limitneg1_h
    /*SPBOUNDNEG*/
    {0x6260,    18,      26,        2,        0,    HI_PQ_MODULE_SHARPNESS, -112},    //boundneg_d
    {0x6260,     9,      17,        2,        0,    HI_PQ_MODULE_SHARPNESS, -112},    //boundneg_v
    {0x6260,     0,       8,        2,        0,    HI_PQ_MODULE_SHARPNESS, -112},    //boundneg_h
    /*SPEDGESHOOTCTL*/
    {0x6264,    16,      23,        2,        0,    HI_PQ_MODULE_SHARPNESS,  128},    //edge_mixratio
    {0x6264,     8,      15,        2,        0,    HI_PQ_MODULE_SHARPNESS,   10},    //edge_under_thr
    {0x6264,     0,       7,        2,        0,    HI_PQ_MODULE_SHARPNESS,   10},    //edge_over_thr
    /*SPEDGETMPH*/
    {0x6268,    16,      24,        2,        0,    HI_PQ_MODULE_SHARPNESS,  128},    //tmp2_sx
    {0x6268,     8,      15,        2,        0,    HI_PQ_MODULE_SHARPNESS,    0},    //tmp1_sx
    {0x6268,     0,       7,        2,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //tmp0_sx
    /*SPEDGETMPV*/
    {0x626c,    16,      24,        2,        0,    HI_PQ_MODULE_SHARPNESS,  128},    //tmp2_sy
    {0x626c,     8,      15,        2,        0,    HI_PQ_MODULE_SHARPNESS,    0},    //tmp1_sy
    {0x626c,     0,       7,        2,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //tmp0_sy
    /*SPEDGEEI*/
    {0x6270,    16,      23,        2,        0,    HI_PQ_MODULE_SHARPNESS,    4},    //edge_eiratio
    {0x6270,     8,      15,        2,        0,    HI_PQ_MODULE_SHARPNESS,  100},    //edge_ei1
    {0x6270,     0,       7,        2,        0,    HI_PQ_MODULE_SHARPNESS,   68},    //edge_ei0
    /*SPGRADMIN*/
    {0x6274,    20,      26,        2,        0,    HI_PQ_MODULE_SHARPNESS,    4},    //gradminslop
    {0x6274,    10,      19,        2,        0,    HI_PQ_MODULE_SHARPNESS,   40},    //gradminthrh
    {0x6274,     0,       9,        2,        0,    HI_PQ_MODULE_SHARPNESS,    8},    //gradminthrl
    /*SPGRADCTR*/
    {0x6278,    10,      25,        2,        0,    HI_PQ_MODULE_SHARPNESS, 1600},    //chessboardthr
    {0x6278,     0,       9,        2,        0,    HI_PQ_MODULE_SHARPNESS,   16},    //gradminthr
    /*SPSHOOTRATIO*/
    {0x627c,    21,      27,        2,        0,    HI_PQ_MODULE_SHARPNESS,  127},    //oshootratiot
    {0x627c,    14,      20,        2,        0,    HI_PQ_MODULE_SHARPNESS,  127},    //oshootratioe
    {0x627c,     7,      13,        2,        0,    HI_PQ_MODULE_SHARPNESS,  16},     //ushootratiot
    {0x627c,     0,       6,        2,        0,    HI_PQ_MODULE_SHARPNESS,  16},     //ushootratioe
    /*SPGRADCONTRAST*/
    {0x6280,     8,      15,        2,        0,    HI_PQ_MODULE_SHARPNESS,  64},     //gradcontrastthr
    {0x6280,     0,      7,         2,        0,    HI_PQ_MODULE_SHARPNESS,   4},     //gradcontrastslop

    /*UHD source*/
    /*SPCTRL*/
    {0x6200,    17,      17,        3,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //ensctrl
    {0x6200,     4,      16,        3,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //demo_pos
    {0x6200,     2,       3,        3,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //demo_mode
    {0x6200,     1,       1,        3,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //sharpen_en
    {0x6200,     0,       0,        3,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //debug_en
    /*SPENABLE*/
    {0x6204,     4,       4,        3,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //hfreq_en_d
    {0x6204,     3,       3,        3,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //hfreq_en_v
    {0x6204,     2,       2,        3,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //hfreq_en_h
    {0x6204,     1,       1,        3,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //detec_en
    {0x6204,     0,       0,        3,      0,      HI_PQ_MODULE_SHARPNESS,   1},     //peak_en
    /*SPWINSIZEHVD*/
    {0x6208,     6,       8,        3,      0,      HI_PQ_MODULE_SHARPNESS,   2},     //winsize_d
    {0x6208,     3,       5,        3,      0,      HI_PQ_MODULE_SHARPNESS,   2},     //winsize_v
    {0x6208,     0,       2,        3,      0,      HI_PQ_MODULE_SHARPNESS,   2},     //winsize_h
    /*SPTMPH1*/
    {0x620c,    16,      24,        3,      0,      HI_PQ_MODULE_SHARPNESS,  78},     //tmp2_h1
    {0x620c,     8,      15,        3,      0,      HI_PQ_MODULE_SHARPNESS,  61},     //tmp1_h1
    {0x620c,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS,  28},     //tmp0_h1
    /*SPTMPH2*/
    {0x6210,    16,      24,        3,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //tmp2_h2
    {0x6210,     8,      15,        3,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //tmp1_h2
    {0x6210,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS, -64},     //tmp0_h2
    /*SPTMPV1*/
    {0x6214,    16,      24,        3,      0,      HI_PQ_MODULE_SHARPNESS,  78},     //tmp2_v1
    {0x6214,     8,      15,        3,      0,      HI_PQ_MODULE_SHARPNESS,  61},     //tmp1_v1
    {0x6214,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS,  28},     //tmp0_v1
    /*SPTMPV2*/
    {0x6218,    16,      24,        3,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //tmp2_v2
    {0x6218,     8,      15,        3,      0,      HI_PQ_MODULE_SHARPNESS,   0},     //tmp1_v2
    {0x6218,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS, -64},     //tmp0_v2
    /*SPGAIN*/
    {0x621c,    20,      27,        3,      0,      HI_PQ_MODULE_SHARPNESS,   8},     //dir_gain
    {0x621c,    10,      19,        3,      0,      HI_PQ_MODULE_SHARPNESS,  40},     //edge_gain
    {0x621c,     0,       9,        3,      0,      HI_PQ_MODULE_SHARPNESS,  80},     //peak_gain
    {0x6220,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS,  20},     //peak_coring
    /*SPPEAKSHOOTCTL*/
    {0x6224,    16,      23,        3,      0,      HI_PQ_MODULE_SHARPNESS,  96},     //peak_mixratio
    {0x6224,     8,      15,        3,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //peak_under_thr
    {0x6224,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //peak_over_thr
    /*SPGAINPOS*/
    {0x6228,    18,      26,        3,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_pos_d
    {0x6228,     9,      17,        3,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_pos_v
    {0x6228,     0,       8,        3,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_pos_h
    /*SPGAINNEG*/
    {0x622c,    18,      26,        3,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_neg_d
    {0x622c,     9,      17,        3,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_neg_v
    {0x622c,     0,       8,        3,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //gain_neg_h
    /*SPOVERTHR*/
    {0x6230,    16,      23,        3,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //over_thr_d
    {0x6230,     8,      15,        3,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //over_thr_v
    {0x6230,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //over_thr_h
    /*SPUNDERTHR*/
    {0x6234,    16,      23,        3,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //under_thr_d
    {0x6234,     8,      15,        3,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //under_thr_v
    {0x6234,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //under_thr_h
    /*SPMIXRATIO*/
    {0x6238,    16,      23,        3,      0,      HI_PQ_MODULE_SHARPNESS,  96},     //mixratio_d
    {0x6238,     8,      15,        3,      0,      HI_PQ_MODULE_SHARPNESS,  96},     //mixratio_v
    {0x6238,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS,  96},     //mixratio_h
    /*SPCORINGZERO*/
    {0x623c,     6,      23,        3,      0,      HI_PQ_MODULE_SHARPNESS,   2},     //coringzero_d
    {0x623c,     8,      15,        3,      0,      HI_PQ_MODULE_SHARPNESS,   3},     //coringzero_v
    {0x623c,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS,   3},     //coringzero_h
    /*SPCORINGRATIO*/
    {0x6240,    10,      14,        3,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //coringratio_d
    {0x6240,     5,       9,        3,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //coringratio_v
    {0x6240,     0,       4,        3,      0,      HI_PQ_MODULE_SHARPNESS,  15},     //coringratio_h
    /*SPCORINGTHR*/
    {0x6244,    16,      23,        3,      0,      HI_PQ_MODULE_SHARPNESS,   7},     //coringthr_d
    {0x6244,     8,      15,        3,      0,      HI_PQ_MODULE_SHARPNESS,   7},     //coringthr_v
    {0x6244,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS,   7},     //coringthr_h
    /*SPLMTRATIO*/
    {0x6248,     6,       8,        3,      0,      HI_PQ_MODULE_SHARPNESS,   6},     //limit_ratio_d
    {0x6248,     3,       5,        3,      0,      HI_PQ_MODULE_SHARPNESS,   6},     //limit_ratio_v
    {0x6248,     0,       2,        3,      0,      HI_PQ_MODULE_SHARPNESS,   6},     //limit_ratio_h
    /*SPLMTPOS0*/
    {0x624c,    16,      23,        3,      0,      HI_PQ_MODULE_SHARPNESS,  64},     //limitpos0_d
    {0x624c,     8,      15,        3,      0,      HI_PQ_MODULE_SHARPNESS,  64},     //limitpos0_v
    {0x624c,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS,  64},     //limitpos0_h
    /*SPLMTPOS1*/
    {0x6250,    16,      23,        3,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //limitpos1_d
    {0x6250,     8,      15,        3,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //limitpos1_v
    {0x6250,     0,       7,        3,      0,      HI_PQ_MODULE_SHARPNESS, 128},     //limitpos1_h
    /*SPBOUNDPOS*/
    {0x6254,    16,      23,        3,        0,    HI_PQ_MODULE_SHARPNESS,  112},    //boundpos_d
    {0x6254,     8,      15,        3,        0,    HI_PQ_MODULE_SHARPNESS,  112},    //boundpos_v
    {0x6254,     0,       7,        3,        0,    HI_PQ_MODULE_SHARPNESS,  112},    //boundpos_h
    /*SPLMTNEG0*/
    {0x6258,    18,      26,        3,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //limitneg0_d
    {0x6258,     9,      17,        3,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //limitneg0_v
    {0x6258,     0,       8,        3,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //limitneg0_h
    /*SPLMTNEG1*/
    {0x625c,    18,      26,        3,        0,    HI_PQ_MODULE_SHARPNESS, -128},    //limitneg1_d
    {0x625c,     9,      17,        3,        0,    HI_PQ_MODULE_SHARPNESS, -128},    //limitneg1_v
    {0x625c,     0,       8,        3,        0,    HI_PQ_MODULE_SHARPNESS, -128},    //limitneg1_h
    /*SPBOUNDNEG*/
    {0x6260,    18,      26,        3,        0,    HI_PQ_MODULE_SHARPNESS, -112},    //boundneg_d
    {0x6260,     9,      17,        3,        0,    HI_PQ_MODULE_SHARPNESS, -112},    //boundneg_v
    {0x6260,     0,       8,        3,        0,    HI_PQ_MODULE_SHARPNESS, -112},    //boundneg_h
    /*SPEDGESHOOTCTL*/
    {0x6264,    16,      23,        3,        0,    HI_PQ_MODULE_SHARPNESS,  128},    //edge_mixratio
    {0x6264,     8,      15,        3,        0,    HI_PQ_MODULE_SHARPNESS,   10},    //edge_under_thr
    {0x6264,     0,       7,        3,        0,    HI_PQ_MODULE_SHARPNESS,   10},    //edge_over_thr
    /*SPEDGETMPH*/
    {0x6268,    16,      24,        3,        0,    HI_PQ_MODULE_SHARPNESS,  128},    //tmp2_sx
    {0x6268,     8,      15,        3,        0,    HI_PQ_MODULE_SHARPNESS,    0},    //tmp1_sx
    {0x6268,     0,       7,        3,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //tmp0_sx
    /*SPEDGETMPV*/
    {0x626c,    16,      24,        3,        0,    HI_PQ_MODULE_SHARPNESS,  128},    //tmp2_sy
    {0x626c,     8,      15,        3,        0,    HI_PQ_MODULE_SHARPNESS,    0},    //tmp1_sy
    {0x626c,     0,       7,        3,        0,    HI_PQ_MODULE_SHARPNESS,  -64},    //tmp0_sy
    /*SPEDGEEI*/
    {0x6270,    16,      23,        3,        0,    HI_PQ_MODULE_SHARPNESS,    4},    //edge_eiratio
    {0x6270,     8,      15,        3,        0,    HI_PQ_MODULE_SHARPNESS,  100},    //edge_ei1
    {0x6270,     0,       7,        3,        0,    HI_PQ_MODULE_SHARPNESS,   68},    //edge_ei0
    /*SPGRADMIN*/
    {0x6274,    20,      26,        3,        0,    HI_PQ_MODULE_SHARPNESS,    4},    //gradminslop
    {0x6274,    10,      19,        3,        0,    HI_PQ_MODULE_SHARPNESS,   40},    //gradminthrh
    {0x6274,     0,       9,        3,        0,    HI_PQ_MODULE_SHARPNESS,    8},    //gradminthrl
    /*SPGRADCTR*/
    {0x6278,    10,      25,        3,        0,    HI_PQ_MODULE_SHARPNESS, 1600},    //chessboardthr
    {0x6278,     0,       9,        3,        0,    HI_PQ_MODULE_SHARPNESS,   16},    //gradminthr
    /*SPSHOOTRATIO*/
    {0x627c,    21,      27,        3,        0,    HI_PQ_MODULE_SHARPNESS,  127},    //oshootratiot
    {0x627c,    14,      20,        3,        0,    HI_PQ_MODULE_SHARPNESS,  127},    //oshootratioe
    {0x627c,     7,      13,        3,        0,    HI_PQ_MODULE_SHARPNESS,  16},     //ushootratiot
    {0x627c,     0,       6,        3,        0,    HI_PQ_MODULE_SHARPNESS,  16},     //ushootratioe
    /*SPGRADCONTRAST*/
    {0x6280,     8,      15,        3,        0,    HI_PQ_MODULE_SHARPNESS,  64},     //gradcontrastthr
    {0x6280,     0,      7,         3,        0,    HI_PQ_MODULE_SHARPNESS,   4},     //gradcontrastslop

    /***********************************************DCI_CTRL***********************************************/
    /*DCI_CTRL_SET*/
    {0x6100,        31,     31,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_en
    {0x6100,        30,     30,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_dbg_en
    {0x6100,        29,     29,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_dbg_mode
    {0x6100,        27,     27,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_man_adj0
    {0x6100,        26,     26,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_man_adj1
    {0x6100,        25,     25,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_man_adj2
    {0x6100,        24,     24,     0,      0,      HI_PQ_MODULE_DCI,       1},     //dci_cbcrcmp_en
    {0x6100,        23,     23,     0,      0,      HI_PQ_MODULE_DCI,       1},     //dci_cbcrsta_en
    {0x6100,        22,     22,     0,      0,      HI_PQ_MODULE_DCI,       1},     //dci_in_range
    {0x6100,        21,     21,     0,      0,      HI_PQ_MODULE_DCI,       1},     //dci_out_range
    {0x6100,        19,     20,     0,      0,      HI_PQ_MODULE_DCI,       1},     //dci_shift_ctrl
    {0x6100,        18,     18,     0,      0,      HI_PQ_MODULE_DCI,       1},     //dci_histlpf_en
    {0x6100,        17,     17,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_swcfg_en
    {0x6100,        14,     15,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_stat_ctrl
    {0x6100,        2,      2,      0,      0,      HI_PQ_MODULE_DCI,       1},     //dci_hist_wgt_en
    /*DCI_CTRL_SET_ReadOnly*/
    {0x6100,        28,     28,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_scene_flg
    /*DCI_HV_POS_ReadOnly*/
    /*DCI_HIS_BLD*/
    {0x610c,        24,     31,     0,      0,      HI_PQ_MODULE_DCI,       64},    //dci_cbcrsta_y
    {0x610c,        16,     23,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_cbcrsta_cb
    {0x610c,        8,      15,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_cbcrsta_cr
    {0x610c,        0,      6,      0,      0,      HI_PQ_MODULE_DCI,       64},    //bs_limit_thr
    /*DCI_HIS_OFT*/
    {0x6110,        23,     31,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_cbcrsta_oft
    {0x6110,        5,      16,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_dbg_pos
    {0x6110,        0,      4,      0,      0,      HI_PQ_MODULE_DCI,       0},     //bs_limit_ablend
    /*DCI_HIS_COR*/
    {0x6114,        24,     31,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_histcor_thr0
    {0x6114,        16,     23,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_histcor_thr1
    {0x6114,        8,      15,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_histcor_thr2
    /*DCI_MER_BLD*/
    {0x6118,        26,     31,     0,      0,      HI_PQ_MODULE_DCI,       8},     //dci_metrc_abld0
    {0x6118,        20,     25,     0,      0,      HI_PQ_MODULE_DCI,       8},     //dci_metrc_abld1
    {0x6118,        14,     19,     0,      0,      HI_PQ_MODULE_DCI,       8},     //dci_metrc_abld2
    {0x6118,        8,      13,     0,      0,      HI_PQ_MODULE_DCI,       8},     //dci_hist_abld
    {0x6118,        2,      7,      0,      0,      HI_PQ_MODULE_DCI,       18},    //dci_org_abld
    /*DCI_ADJ_WGT*/
    {0x611c,        24,     31,     0,      0,      HI_PQ_MODULE_DCI,       50},    //dci_man_adjwgt0
    {0x611c,        16,     23,     0,      0,      HI_PQ_MODULE_DCI,       60},    //dci_man_adjwgt1
    {0x611c,        8,      15,     0,      0,      HI_PQ_MODULE_DCI,       70},    //dci_man_adjwgt2
    /*DCI_CLIP*/
    {0x6120,        24,     31,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_wgt_cliplow0
    {0x6120,        16,     23,     0,      0,      HI_PQ_MODULE_DCI,       255},   //dci_wgt_cliphigh0
    /*DCI_PARARD*/
    {0x6198,        4,      4,      0,      0,      HI_PQ_MODULE_DCI,       1},     //dcivs_rd_en
    {0x6198,        3,      3,      0,      0,      HI_PQ_MODULE_DCI,       1},     //dcibs_rd_en
    {0x6198,        2,      2,      0,      0,      HI_PQ_MODULE_DCI,       1},     //dcilut_rd_en
    {0x6198,        1,      1,      0,      0,      HI_PQ_MODULE_DCI,       1},     //dcidiv_rd_en
    {0x6198,        0,      0,      0,      0,      HI_PQ_MODULE_DCI,       1},     //dcihbw_rd_en
    /*DCI_PARA_DATA*/
    /*DCI_SPLIT*/
    {0x61a0,        24,     31,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_split_point
    {0x61a0,        20,     21,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_split_type
    {0x61a0,        18,     19,     0,      0,      HI_PQ_MODULE_DCI,       1},     //dci_split_hist_mode
    {0x61a0,        11,     17,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_man_alg_blend
    {0x61a0,        10,     10,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_man_alg_blend_en
    {0x61a0,        2,      9,      0,      0,      HI_PQ_MODULE_DCI,       200},   //dci_flat_thrsh
    {0x61a0,        0,      1,      0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_flat_mode
    /*DCI_HIST_CLIP*/
    {0x61a4,        27,     31,     0,      0,      HI_PQ_MODULE_DCI,       2},     //dci_hist_map_blend
    {0x61a4,        17,     26,     0,      0,      HI_PQ_MODULE_DCI,      20},     //dci_clip_limit_r
    {0x61a4,        7,      16,     0,      0,      HI_PQ_MODULE_DCI,      20},     //dci_clip_limit_l
    /*DCI_BLACK0_STRETCH*/
    {0x61a8,        24,     31,     0,      0,      HI_PQ_MODULE_DCI,       3},     //dci_bs_min_black_thr
    {0x61a8,        18,     22,     0,      0,      HI_PQ_MODULE_DCI,       1},     //dci_bs_min_black_blend
    {0x61a8,        10,     17,     0,      0,      HI_PQ_MODULE_DCI,       160},   //dci_bs_max_level
    {0x61a8,        1,      9,      0,      0,      HI_PQ_MODULE_DCI,       320},   //dci_bs_blending_level
    {0x61a8,        0,      0,      0,      0,      HI_PQ_MODULE_DCI,       1},     //dci_bs_en
    /*DCI_BLACK1_STRETCH*/
    {0x61ac,        16,     31,     0,      0,      HI_PQ_MODULE_DCI,       102},   //dci_bs_deta_inv
    {0x61ac,        6,      11,     0,      0,      HI_PQ_MODULE_DCI,       25},    //dci_bs_black_wgt_thr
    {0x61ac,        0,      5,      0,      0,      HI_PQ_MODULE_DCI,       2},     //dci_bs_l_offset
    /*DCI_OUT_REGION*/
    {0x61b0,        20,     31,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_out_hstart
    {0x61b0,        8,      19,     0,      0,      HI_PQ_MODULE_DCI,       1920},  //dci_out_hend
    {0x61b4,        20,     31,     0,      0,      HI_PQ_MODULE_DCI,       0},     //dci_out_vstart
    {0x61b4,        8,      19,     0,      0,      HI_PQ_MODULE_DCI,       1080},  //dci_out_vend
    /*DCI_TOTAL_PIX*/
    /*DCI_STATICS_DATA*/

    /****************************************DESHOOT****************************************/
    /*DS_CTRL*/
    {0x2500,     2,       2,        0,        0,      HI_PQ_MODULE_DS,        1},         //ds_en
    {0x2760,    19,      25,        0,        0,      HI_PQ_MODULE_DS,        8},         //ds_flatthrscl
    {0x2760,    12,      18,        0,        0,      HI_PQ_MODULE_DS,        6},         //ds_cordirthr
    {0x2760,     1,      11,        0,        0,      HI_PQ_MODULE_DS,        3},         //ds_blendgain
    /*DS_THR1*/
    {0x2764,    21,      27,        0,        0,      HI_PQ_MODULE_DS,        8},         //ds_patcond3thr3
    {0x2764,    14,      20,        0,        0,      HI_PQ_MODULE_DS,       16},         //ds_patcond5thr5
    {0x2764,     7,      13,        0,        0,      HI_PQ_MODULE_DS,       11},         //ds_patcond6scl61
    {0x2764,     0,       6,        0,        0,      HI_PQ_MODULE_DS,        4},         //ds_patcond6scl62
    /*DS_THR2*/
    {0x2768,    21,      27,        0,        0,      HI_PQ_MODULE_DS,        3},         //ds_patcond6scl63
    {0x2768,    14,      20,        0,        0,      HI_PQ_MODULE_DS,       11},         //ds_patcond6scl64
    {0x2768,     7,      13,        0,        0,      HI_PQ_MODULE_DS,        5},         //ds_patcond6scl65
    {0x2768,     0,       6,        0,        0,      HI_PQ_MODULE_DS,        7},         //ds_patcond6scl66
    /*DS_THR3*/
    {0x276c,    16,      23,        0,        0,      HI_PQ_MODULE_DS,        1},         //ds_wincond1thr1
    {0x276c,     8,      15,        0,        0,      HI_PQ_MODULE_DS,        5},         //ds_wincond1val1
    {0x276c,     0,       7,        0,        0,      HI_PQ_MODULE_DS,        6},         //ds_wincond2thr1
    /*DS_THR4*/
    {0x2770,    16,      23,        0,        0,      HI_PQ_MODULE_DS,        5},         //ds_wincond2val1
    {0x2770,     8,      15,        0,        0,      HI_PQ_MODULE_DS,       80},         //ds_wincond2thr2
    {0x2770,     0,       7,        0,        0,      HI_PQ_MODULE_DS,       45},         //ds_wincond2val2
    /*DS_THR5*/
    {0x2774,    16,      23,        0,        0,      HI_PQ_MODULE_DS,       12},         //ds_wincond3thr1
    {0x2774,     8,      15,        0,        0,      HI_PQ_MODULE_DS,        5},         //ds_wincond3val1
    {0x2774,     0,       7,        0,        0,      HI_PQ_MODULE_DS,       10},         //ds_wincond3val2
    /*DS_THR6*/
    {0x2778,    17,      24,        0,        0,      HI_PQ_MODULE_DS,       90},         //ds_wincond3thr2
    {0x2778,     9,      16,        0,        0,      HI_PQ_MODULE_DS,       45},         //ds_wincond3thr3
    {0x2778,     0,       8,        0,        0,      HI_PQ_MODULE_DS,      256},         //ds_wincond3thr4
    /*DS_THR7*/
    {0x277c,    16,      23,        0,        0,      HI_PQ_MODULE_DS,        8},         //ds_wincond3scl1
    {0x277c,     8,      15,        0,        0,      HI_PQ_MODULE_DS,       38},         //ds_wincond4thr1
    {0x277c,     0,       7,        0,        0,      HI_PQ_MODULE_DS,        5},         //ds_wincond4thr2
    /*DS_THR8*/
    {0x2780,    16,      23,        0,        0,      HI_PQ_MODULE_DS,       10},         //ds_wincond4val1
    {0x2780,     8,      15,        0,        0,      HI_PQ_MODULE_DS,       45},         //ds_wincond4val2
    {0x2780,     0,       7,        0,        0,      HI_PQ_MODULE_DS,        5},         //ds_wincond4val3
    /*DS_THR9*/
    {0x2784,    17,      24,        0,        0,      HI_PQ_MODULE_DS,       45},         //ds_wincond3thr2
    {0x2784,     9,      16,        0,        0,      HI_PQ_MODULE_DS,       45},         //ds_wincond3thr3
    {0x2784,     0,       8,        0,        0,      HI_PQ_MODULE_DS,        8},         //ds_wincond3thr4
    /*DS_THR10*/
    {0x2788,    16,      23,        0,        0,      HI_PQ_MODULE_DS,       20},         //ds_wincond5thr1
    {0x2788,     8,      15,        0,        0,      HI_PQ_MODULE_DS,        5},         //ds_wincond5val1
    {0x2788,     0,       7,        0,        0,      HI_PQ_MODULE_DS,        5},         //ds_wincond5val2
    /*DS_THR11*/
    {0x278c,    18,      25,        0,        0,      HI_PQ_MODULE_DS,       80},         //ds_wincond5val3
    {0x278c,     9,      17,        0,        0,      HI_PQ_MODULE_DS,        8},         //ds_wincond5scl1
    {0x278c,     0,       8,        0,        0,      HI_PQ_MODULE_DS,        8},         //ds_wincond5scl2
    /*DS_THR12*/
    {0x2790,    18,      26,        0,        0,      HI_PQ_MODULE_DS,       16},         //ds_picresscl0
    {0x2790,     9,      17,        0,        0,      HI_PQ_MODULE_DS,        9},         //ds_picresscl1
    {0x2790,     0,       8,        0,        0,      HI_PQ_MODULE_DS,       10},         //ds_picresscl2
    /*DS_THR13*/
    {0x2794,    14,      26,        0,        0,      HI_PQ_MODULE_DS,     2500},         //ds_winactthr
    {0x2794,     0,      13,        0,        0,      HI_PQ_MODULE_DS,      720},         //ds_picresofst
    /*DS_THR14*/
    {0x2798,    21,      27,        0,        0,      HI_PQ_MODULE_DS,       11},         //ds_patcond6norm61
    {0x2798,    14,      20,        0,        0,      HI_PQ_MODULE_DS,        0},         //ds_patcond6norm62
    {0x2798,     7,      13,        0,        0,      HI_PQ_MODULE_DS,        0},         //ds_patcond6norm63
    {0x2798,     0,       6,        0,        0,      HI_PQ_MODULE_DS,        0},         //ds_patcond6normofst
    /*DS_THR15*/
    {0x279c,    14,      20,        0,        0,      HI_PQ_MODULE_DS,        0},         //ds_patcond6norm5
    {0x279c,     7,      13,        0,        0,      HI_PQ_MODULE_DS,        0},         //ds_patcond6norm6
    {0x279c,     0,       6,        0,        0,      HI_PQ_MODULE_DS,       32},         //ds_patcond6clipscl6
    /*DS_THR16*/
    {0x27a0,     0,       0,        0,        0,      HI_PQ_MODULE_DS,        1},         //ds_resadpten
    /*DS_THR17*/
    {0x27a4,     9,      17,        0,        0,      HI_PQ_MODULE_DS,       38},         //ds_shtctrstscl1
    {0x27a4,     0,       8,        0,        0,      HI_PQ_MODULE_DS,       22},         //ds_shtctrstscl2
    /*DS_THR20*/
    {0x27b8,    21,      28,        0,        0,      HI_PQ_MODULE_DS,       35},         //ds_patcond6scl7
    {0x27b8,     0,       0,        0,        0,      HI_PQ_MODULE_DS,        0},         //ds_overshootflag

    /************************************* ZME DS *****************************************/
    /* SHOOTCTRL_HL */
    {0x08f8,    17,      17,        0,        0,   HI_PQ_MODULE_ARTDS,        1},         //hl_shootctrl_en
    {0x08f8,    16,      16,        0,        0,   HI_PQ_MODULE_ARTDS,        0},         //hl_shootctrl_mode
    {0x08f8,    15,      15,        0,        0,   HI_PQ_MODULE_ARTDS,        1},         //hl_flatdect_mode
    {0x08f8,    14,      14,        0,        0,   HI_PQ_MODULE_ARTDS,        1},         //hl_coringadj_en
    {0x08f8,     8,      13,        0,        0,   HI_PQ_MODULE_ARTDS,       32},         //hl_gain
    {0x08f8,     0,       7,        0,        0,   HI_PQ_MODULE_ARTDS,       32},         //hl_coring
    /* SHOOTCTRL_VL */
    {0x0900,    17,      17,        0,        0,   HI_PQ_MODULE_ARTDS,        1},         //vl_shootctrl_en
    {0x0900,    16,      16,        0,        0,   HI_PQ_MODULE_ARTDS,        0},         //vl_shootctrl_mode
    {0x0900,    15,      15,        0,        0,   HI_PQ_MODULE_ARTDS,        1},         //vl_flatdect_mode
    {0x0900,    14,      14,        0,        0,   HI_PQ_MODULE_ARTDS,        1},         //vl_coringadj_en
    {0x0900,     8,      13,        0,        0,   HI_PQ_MODULE_ARTDS,       32},         //vl_gain
    {0x0900,     0,       7,        0,        0,   HI_PQ_MODULE_ARTDS,       32},         //vl_coring

    /************************************* ZME2 DS *****************************************/
    /* SHOOTCTRL_HL */
    {0xbaf8,    17,      17,        0,        0,   HI_PQ_MODULE_ARTDS,        1},         //hl_shootctrl_en
    {0xbaf8,    16,      16,        0,        0,   HI_PQ_MODULE_ARTDS,        0},         //hl_shootctrl_mode
    {0xbaf8,    15,      15,        0,        0,   HI_PQ_MODULE_ARTDS,        1},         //hl_flatdect_mode
    {0xbaf8,    14,      14,        0,        0,   HI_PQ_MODULE_ARTDS,        1},         //hl_coringadj_en
    {0xbaf8,     8,      13,        0,        0,   HI_PQ_MODULE_ARTDS,       32},         //hl_gain
    {0xbaf8,     0,       7,        0,        0,   HI_PQ_MODULE_ARTDS,       32},         //hl_coring
    /* SHOOTCTRL_VL */
    {0xbb00,    17,      17,        0,        0,   HI_PQ_MODULE_ARTDS,        1},         //vl_shootctrl_en
    {0xbb00,    16,      16,        0,        0,   HI_PQ_MODULE_ARTDS,        0},         //vl_shootctrl_mode
    {0xbb00,    15,      15,        0,        0,   HI_PQ_MODULE_ARTDS,        1},         //vl_flatdect_mode
    {0xbb00,    14,      14,        0,        0,   HI_PQ_MODULE_ARTDS,        1},         //vl_coringadj_en
    {0xbb00,     8,      13,        0,        0,   HI_PQ_MODULE_ARTDS,       32},         //vl_gain
    {0xbb00,     0,       7,        0,        0,   HI_PQ_MODULE_ARTDS,       32},         //vl_coring


    /****************************************DERING****************************************/
    /*DR_CTRL*/
    {0x2500,     3,       3,        0,        0,      HI_PQ_MODULE_DR,        1},         //dr_en
    {0x2800,     7,       7,        0,        0,      HI_PQ_MODULE_DR,        0},         //dr_ennosmoothwavepro
    {0x2800,     6,       6,        0,        0,      HI_PQ_MODULE_DR,        0},         //dr_en3x3filter
    {0x2800,     5,       5,        0,        0,      HI_PQ_MODULE_DR,        0},         //dr_enfarthestsmooth
    {0x2800,     4,       4,        0,        0,      HI_PQ_MODULE_DR,        1},         //dr_enlimitpro
    {0x2800,     3,       3,        0,        0,      HI_PQ_MODULE_DR,        1},         //dr_enunderpro
    {0x2800,     2,       2,        0,        0,      HI_PQ_MODULE_DR,        0},         //dr_encondarkpro
    {0x2800,     1,       1,        0,        0,      HI_PQ_MODULE_DR,        0},         //dr_enconpro
    /*DR_THR1*/
    {0x2804,    17,      26,        0,        0,      HI_PQ_MODULE_DR,      400},         //dr_gradmaxthd
    {0x2804,     7,      16,        0,        0,      HI_PQ_MODULE_DR,      200},         //dr_shootmaxthd
    {0x2804,     0,       6,        0,        0,      HI_PQ_MODULE_DR,        6},         //dr_gradratio
    /*DR_THR2*/
    {0x2808,    19,      22,        0,        0,      HI_PQ_MODULE_DR,        4},         //dr_underratio
    {0x2808,    11,      18,        0,        0,      HI_PQ_MODULE_DR,       24},         //dr_srhtrsngradalpha
    {0x2808,     8,      10,        0,        0,      HI_PQ_MODULE_DR,        2},         //dr_srhtrsngradbeta
    {0x2808,     0,       7,        0,        0,      HI_PQ_MODULE_DR,      120},         //dr_highfrequent
    /*DR_THR3*/
    {0x280c,    20,      29,        0,        0,      HI_PQ_MODULE_DR,      196},         //dr_ringx0
    {0x280c,    10,      19,        0,        0,      HI_PQ_MODULE_DR,      260},         //dr_ringx1
    {0x280c,     0,       9,        0,        0,      HI_PQ_MODULE_DR,      520},         //dr_ringx2
    /*DR_THR4*/
    {0x2810,    20,      29,        0,        0,      HI_PQ_MODULE_DR,      560},         //dr_ringx3
    {0x2810,    10,      19,        0,        0,      HI_PQ_MODULE_DR,       10},         //dr_ringy0
    {0x2810,     0,       9,        0,        0,      HI_PQ_MODULE_DR,       32},         //dr_ringy1
    /*DR_THR5*/
    {0x2814,    18,      27,        0,        0,      HI_PQ_MODULE_DR,       32},         //dr_ringy2
    {0x2814,     8,      17,        0,        0,      HI_PQ_MODULE_DR,       60},         //dr_ringy3
    {0x2814,     4,       7,        0,        0,      HI_PQ_MODULE_DR,        1},         //dr_shttornum0
    {0x2814,     0,       3,        0,        0,      HI_PQ_MODULE_DR,        1},         //dr_shttornum1
    /*DR_THR6*/
    {0x2818,    12,      27,        0,        0,      HI_PQ_MODULE_DR,       20},         //dr_ringk0
    {0x2818,     8,      11,        0,        0,      HI_PQ_MODULE_DR,        1},         //dr_shttornum2
    {0x2818,     4,       7,        0,        0,      HI_PQ_MODULE_DR,        2},         //dr_shttornum3
    {0x2818,     0,       3,        0,        0,      HI_PQ_MODULE_DR,        1},         //dr_restornum
    /*DR_THR7*/
    {0x281c,    10,      25,        0,        0,      HI_PQ_MODULE_DR,        2},         //dr_ringk1
    {0x281c,     0,       9,        0,        0,      HI_PQ_MODULE_DR,      240},         //dr_ringmax
    /*DR_THR8*/
    {0x2820,    10,      25,        0,        0,      HI_PQ_MODULE_DR,       20},         //dr_ringk2
    {0x2820,     0,       9,        0,        0,      HI_PQ_MODULE_DR,        0},         //dr_ringmin
    /*DR_THR9*/
    {0x2824,    18,      27,        0,        0,      HI_PQ_MODULE_DR,       12},         //dr_estresalterthd
    {0x2824,     8,      17,        0,        0,      HI_PQ_MODULE_DR,       40},         //dr_estresaltertmp
    {0x2824,     0,       7,        0,        0,      HI_PQ_MODULE_DR,       32},         //dr_estproringthd
    /*DR_THR10*/
    {0x2828,    16,      23,        0,        0,      HI_PQ_MODULE_DR,       12},         //dr_shtsmooththd0
    {0x2828,     8,      15,        0,        0,      HI_PQ_MODULE_DR,       20},         //dr_shtsmooththd1
    {0x2828,     0,       7,        0,        0,      HI_PQ_MODULE_DR,       48},         //dr_shtsmooththd2
    /*DR_THR11*/
    {0x282c,    16,      23,        0,        0,      HI_PQ_MODULE_DR,       12},         //dr_shtsmoothvalue0
    {0x282c,     8,      15,        0,        0,      HI_PQ_MODULE_DR,       20},         //dr_shtsmoothvalue1
    {0x282c,     0,       7,        0,        0,      HI_PQ_MODULE_DR,       48},         //dr_shtsmoothvalue2
    /*DR_THR12*/
    {0x2830,    17,      26,        0,        0,      HI_PQ_MODULE_DR,      272},         //dr_estprogradthd1
    {0x2830,     7,      16,        0,        0,      HI_PQ_MODULE_DR,      240},         //dr_estprogradthd0
    {0x2830,     0,       6,        0,        0,      HI_PQ_MODULE_DR,       56},         //dr_estprogradcoef
    /*DR_THR13*/
    {0x2834,    20,      27,        0,        0,      HI_PQ_MODULE_DR,       80},         //dr_estcomthd
    {0x2834,    14,      19,        0,        0,      HI_PQ_MODULE_DR,       12},         //dr_estcomratio1
    {0x2834,     8,      13,        0,        0,      HI_PQ_MODULE_DR,       32},         //dr_estcomratio2
    {0x2834,     0,       7,        0,        0,      HI_PQ_MODULE_DR,        8},         //dr_estcomtmp
    /*DR_THR14*/
    {0x2838,    16,      26,        0,        0,      HI_PQ_MODULE_DR,        0},         //dr_estpixeldiff
    {0x2838,     8,      15,        0,        0,      HI_PQ_MODULE_DR,      120},         //dr_limitwavescope0
    {0x2838,     0,       7,        0,        0,      HI_PQ_MODULE_DR,       80},         //dr_limitwavescope1
    /*DR_THR15*/
    {0x283c,    26,      29,        0,        0,      HI_PQ_MODULE_DR,        4},         //dr_limitwavenum0
    {0x283c,    22,      25,        0,        0,      HI_PQ_MODULE_DR,        1},         //dr_limitwavenum1
    {0x283c,    18,      21,        0,        0,      HI_PQ_MODULE_DR,        1},         //dr_limitwavenum2
    {0x283c,     8,      17,        0,        0,      HI_PQ_MODULE_DR,      440},         //dr_limitgrad
    {0x283c,     0,       7,        0,        0,      HI_PQ_MODULE_DR,      120},         //dr_limitscope
    /*DR_THR16*/
    {0x2840,    14,      23,        0,        0,      HI_PQ_MODULE_DR,      120},         //dr_calcsum5error
    {0x2840,     7,      13,        0,        0,      HI_PQ_MODULE_DR,       62},         //dr_largewaveblend
    {0x2840,     0,       6,        0,        0,      HI_PQ_MODULE_DR,       32},         //dr_largewavesumblend
    /*DR_THR17*/
    {0x2844,    22,      29,        0,        0,      HI_PQ_MODULE_DR,       12},         //dr_pixelrange
    {0x2844,    14,      21,        0,        0,      HI_PQ_MODULE_DR,      132},         //dr_calcwaverange
    {0x2844,     4,      13,        0,        0,      HI_PQ_MODULE_DR,      320},         //dr_largewaveminthd
    {0x2844,     0,       3,        0,        0,      HI_PQ_MODULE_DR,        2},         //dr_wavepronumlim
    /*DR_THR18*/
    {0x2848,    16,      23,        0,        0,      HI_PQ_MODULE_DR,        0},         //dr_smoothblendx0
    {0x2848,     8,      15,        0,        0,      HI_PQ_MODULE_DR,       12},         //dr_smoothblendx1
    {0x2848,     0,       7,        0,        0,      HI_PQ_MODULE_DR,       20},         //dr_smoothblendx2
    /*DR_THR19*/
    {0x284c,    16,      23,        0,        0,      HI_PQ_MODULE_DR,       48},         //dr_smoothblendx3
    {0x284c,     8,      15,        0,        0,      HI_PQ_MODULE_DR,      128},         //dr_smoothblendy0
    {0x284c,     0,       7,        0,        0,      HI_PQ_MODULE_DR,      100},         //dr_smoothblendy1
    /*DR_THR20*/
    {0x2850,    16,      23,        0,        0,      HI_PQ_MODULE_DR,       90},         //dr_smoothblendy2
    {0x2850,     8,      15,        0,        0,      HI_PQ_MODULE_DR,       80},         //dr_smoothblendy3
    {0x2850,     0,       7,        0,        0,      HI_PQ_MODULE_DR,      128},         //dr_smoothblendmax
    /*DR_THR21*/
    {0x2854,     8,      23,        0,        0,      HI_PQ_MODULE_DR,      -85},         //dr_smoothblendk0
    {0x2854,     0,       7,        0,        0,      HI_PQ_MODULE_DR,        0},         //dr_smoothblendmin
    /*DR_THR22*/
    {0x2858,     7,      22,        0,        0,      HI_PQ_MODULE_DR,        0},         //dr_smoothblendk1
    {0x2858,     0,       6,        0,        0,      HI_PQ_MODULE_DR,       62},         //dr_noflagareablend
    /*DR_THR23*/
    {0x285c,    10,      25,        0,        0,      HI_PQ_MODULE_DR,        0},         //dr_smoothblendk2
    {0x285c,     0,       9,        0,        0,      HI_PQ_MODULE_DR,      200},         //dr_pixellargeerror
    /*DR_THR24*/
    {0x2860,    20,      27,        0,        0,      HI_PQ_MODULE_DR,       20},         //dr_conpixelerror
    {0x2860,    12,      19,        0,        0,      HI_PQ_MODULE_DR,       40},         //dr_con5pixelerror
    {0x2860,     0,      11,        0,        0,      HI_PQ_MODULE_DR,      400},         //dr_conallpixelerror
    /*DR_THR25*/
    {0x2864,    19,      25,        0,        0,      HI_PQ_MODULE_DR,        1},         //dr_returnblend
    {0x2864,    12,      18,        0,        0,      HI_PQ_MODULE_DR,       32},         //dr_consumblend
    {0x2864,     8,      11,        0,        0,      HI_PQ_MODULE_DR,        3},         //dr_limitpixelnum
    {0x2864,     4,       7,        0,        0,      HI_PQ_MODULE_DR,        2},         //dr_limitsmoothpronum
    {0x2864,     0,       3,        0,        0,      HI_PQ_MODULE_DR,        2},         //dr_limitwavepronum
    /*DR_THR26*/
    {0x2868,    24,      30,        0,        0,      HI_PQ_MODULE_DR,        1},         //dr_crossblend
    {0x2868,    17,      23,        0,        0,      HI_PQ_MODULE_DR,        1},         //dr_shootblend
    {0x2868,     9,      16,        0,        0,      HI_PQ_MODULE_DR,       20},         //dr_borderprcwidth
    {0x2868,     0,       8,        0,        0,      HI_PQ_MODULE_DR,      248},         //dr_resblend

    /*************************************TNR****************************************/
    /*TNR_MODE*/
    {0x3800,    31,      31,        0,        0,      HI_PQ_MODULE_TNR,      0},         //market_en
    {0x3800,    30,      30,        0,        0,      HI_PQ_MODULE_TNR,      0},         //market_mode
    {0x3800,    28,      28,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tnr_y_bypass
    {0x3800,    27,      27,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tnr_c_bypass
    {0x3800,    26,      26,        0,        0,      HI_PQ_MODULE_TNR,      1},         //hist_motion_en
    {0x3800,    21,      21,        0,        0,      HI_PQ_MODULE_TNR,      1},       // 1  //tfm_lpf_en
    {0x3800,    12,      13,        0,        0,      HI_PQ_MODULE_TNR,      2},         //tm_cmt_mode (fixed)
    {0x3800,    10,      11,        0,        0,      HI_PQ_MODULE_TNR,      2},         //tm_ymt_mode (fixed)
    {0x3800,    8,       9,         0,        0,      HI_PQ_MODULE_TNR,      3},         //tm_cmt_mean_mode
    {0x3800,    6,       7,         0,        0,      HI_PQ_MODULE_TNR,      3},         //tm_ymt_mean_mode
    {0x3800,    0,       1,         0,        0,      HI_PQ_MODULE_TNR,      2},         //tm_lpf_mode
    /*TNR_YMOTION_MAP*/
    {0x3804,    31,      31,        0,        0,      HI_PQ_MODULE_TNR,      1},         //tm_ymt_meanratio_en
    {0x3804,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_ymt_mapping_min
    {0x3804,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,    255},         //tm_ymt_mapping_max
    {0x3808,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      5},         //tm_ymt_mapping_r0
    {0x3808,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     56},         //tm_ymt_mapping_r1
    {0x3808,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,     96},         //tm_ymt_mapping_r2
    {0x380c,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,    144},         //tm_ymt_mapping_r3
    {0x380c,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,    255},         //tm_ymt_mapping_r4
    {0x3810,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_ymt_mapping_t1
    {0x3810,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tm_ymt_mapping_t2
    {0x3810,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,     64},         //tm_ymt_mapping_t3
    {0x3814,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,    128},         //tm_ymt_mapping_t4
    {0x3814,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,    255},         //tm_ymt_mapping_t5
    {0x3814,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,    255},         //tm_ymt_mapping_t6
    {0x3818,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     26},         //tm_ymt_mapping_k1
    {0x3818,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     20},         //tm_ymt_mapping_k2
    {0x381c,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     12},         //tm_ymt_mapping_k3
    {0x381c,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     14},         //tm_ymt_mapping_k4
    /*TNR_CMOTION_MAP*/
    {0x3820,    31,      31,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_cmt_meanratio_en
    {0x3820,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_cmt_mapping_min
    {0x3820,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,    255},         //tm_cmt_mapping_max
    {0x3824,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      5},         //tm_cmt_mapping_r0
    {0x3824,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     56},         //tm_cmt_mapping_r1
    {0x3824,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,     96},         //tm_cmt_mapping_r2
    {0x3828,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,    144},         //tm_cmt_mapping_r3
    {0x3828,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,    255},         //tm_cmt_mapping_r4
    {0x382c,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_cmt_mapping_t1
    {0x382c,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tm_cmt_mapping_t2
    {0x382c,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,     64},         //tm_cmt_mapping_t3
    {0x3830,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,    128},         //tm_cmt_mapping_t4
    {0x3830,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,    255},         //tm_cmt_mapping_t5
    {0x3830,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,    255},         //tm_cmt_mapping_t6
    {0x3834,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     26},         //tm_cmt_mapping_k1
    {0x3834,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     20},         //tm_cmt_mapping_k2
    {0x3838,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     12},         //tm_cmt_mapping_k3
    {0x3838,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     14},         //tm_cmt_mapping_k4
    /*TNR_YMOTION_MEANRATIO*/
    {0x383c,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_ymt_meanratio_min
    {0x383c,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,   1023},         //tm_ymt_meanratio_max
    {0x3840,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,    120},         //tm_ymt_meanratio_r0
    {0x3840,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tm_ymt_meanratio_r1
    {0x3840,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tm_ymt_meanratio_r2
    {0x3844,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     32},         //tm_ymt_meanratio_r3
    {0x3844,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tm_ymt_meanratio_r4
    {0x3848,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     24},         //tm_ymt_meanratio_t1
    {0x3848,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     36},         //tm_ymt_meanratio_t2
    {0x3848,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,    256},         //tm_ymt_meanratio_t3
    {0x384c,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,   1023},         //tm_ymt_meanratio_t4
    {0x384c,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,   1023},         //tm_ymt_meanratio_t5
    {0x384c,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,   1023},         //tm_ymt_meanratio_t6
    {0x3850,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,   -117},         //tm_ymt_meanratio_k1
    {0x3850,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_ymt_meanratio_k2
    {0x3854,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_ymt_meanratio_k3
    {0x3854,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_ymt_meanratio_k4
    /*TNR_CMOTION_MEANRATIO*/
    {0x3858,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_cmt_meanratio_min
    {0x3858,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,   1023},         //tm_cmt_meanratio_max
    {0x385c,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     32},         //tm_cmt_meanratio_r0
    {0x385c,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tm_cmt_meanratio_r1
    {0x385c,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tm_cmt_meanratio_r2
    {0x3860,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     32},         //tm_cmt_meanratio_r3
    {0x3860,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tm_cmt_meanratio_r4
    {0x3864,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      1},         //tm_cmt_meanratio_t1
    {0x3864,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,    100},         //tm_cmt_meanratio_t2
    {0x3864,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,    200},         //tm_cmt_meanratio_t3
    {0x3868,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,    300},         //tm_cmt_meanratio_t4
    {0x3868,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,    400},         //tm_cmt_meanratio_t5
    {0x3868,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,   1023},         //tm_cmt_meanratio_t6
    {0x386c,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_cmt_meanratio_k1
    {0x386c,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_cmt_meanratio_k2
    {0x3870,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_cmt_meanratio_k3
    {0x3870,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_cmt_meanratio_k4
    /*TNR_TM_YCMTCOEF*/
    {0x3874,    0,       5,         0,        0,      HI_PQ_MODULE_TNR,      2},         //tm_ycmtcoef0
    {0x3874,    6,       11,        0,        0,      HI_PQ_MODULE_TNR,      6},         //tm_ycmtcoef1
    {0x3874,    12,      17,        0,        0,      HI_PQ_MODULE_TNR,     16},         //tm_ycmtcoef2
    {0x3874,    18,      23,        0,        0,      HI_PQ_MODULE_TNR,      6},         //tm_ycmtcoef3
    {0x3874,    24,      29,        0,        0,      HI_PQ_MODULE_TNR,      2},         //tm_ycmtcoef4
    /*TNR_TM_YCLPFCOEF*/
    /*对Motion进行图像处理时的滤波模式及其滤波参数, 系数之和必须为32 */
    {0x3878,    0,       5,         0,        0,      HI_PQ_MODULE_TNR,      2},         //tm_yclpfcoef0
    {0x3878,    6,       11,        0,        0,      HI_PQ_MODULE_TNR,      6},         //tm_yclpfcoef1
    {0x3878,    12,      17,        0,        0,      HI_PQ_MODULE_TNR,     16},         //tm_yclpfcoef2
    {0x3878,    18,      23,        0,        0,      HI_PQ_MODULE_TNR,      6},         //tm_yclpfcoef3
    {0x3878,    24,      29,        0,        0,      HI_PQ_MODULE_TNR,      2},         //tm_yclpfcoef4
    /*TNR_TFM_CMOTION_CK*/
    {0x3974,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_cmotion_core
    {0x3974,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tfm_cmotion_k
    /*TNR_TFM_CEDGE_CK*/
    {0x3978,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_cedge_core (fixed)
    {0x3978,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_cedge_k (fixed)
    {0x3978,    20,      20,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_2dupdate (fixed)
    {0x3978,    21,      21,        0,        0,      HI_PQ_MODULE_TNR,      1},         //tfm_cbcrupdate
    /*TNR_TFM_Y_MAP*/
    {0x397c,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_y_mapping_min
    {0x397c,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tfm_y_mapping_max
    {0x3980,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     10},         //tfm_y_mapping_r0
    {0x3980,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      5},         //tfm_y_mapping_r1
    {0x3980,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,     20},         //tfm_y_mapping_r2
    {0x3984,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     27},         //tfm_y_mapping_r3
    {0x3984,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tfm_y_mapping_r4
    {0x3988,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_y_mapping_t1
    {0x3988,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      4},         //tfm_y_mapping_t2
    {0x3988,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tfm_y_mapping_t3
    {0x398c,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     46},         //tfm_y_mapping_t4
    {0x398c,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     58},         //tfm_y_mapping_t5
    {0x398c,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,     63},         //tfm_y_mapping_t6
    {0x3990,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,    -20},         //tfm_y_mapping_k1
    {0x3990,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      9},         //tfm_y_mapping_k2
    {0x3994,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      8},         //tfm_y_mapping_k3
    {0x3994,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      7},         //tfm_y_mapping_k4
    /*TNR_TFM_C_MAP*/
    {0x3998,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_c_mapping_min
    {0x3998,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tfm_c_mapping_max
    {0x399c,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     10},         //tfm_c_mapping_r0
    {0x399c,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      5},         //tfm_c_mapping_r1
    {0x399c,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,     20},         //tfm_c_mapping_r2
    {0x39a0,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     27},         //tfm_c_mapping_r3
    {0x39a0,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tfm_c_mapping_r4
    {0x39a4,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_c_mapping_t1
    {0x39a4,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      4},         //tfm_c_mapping_t2
    {0x39a4,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tfm_c_mapping_t3
    {0x39a8,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     46},         //tfm_c_mapping_t4
    {0x39a8,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     58},         //tfm_c_mapping_t5
    {0x39a8,    20,      29,        0,        0,      HI_PQ_MODULE_TNR,     63},         //tfm_c_mapping_t6
    {0x39ac,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,    -70},         //tfm_c_mapping_k1
    {0x39ac,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      9},         //tfm_c_mapping_k2
    {0x39b0,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      8},         //tfm_c_mapping_k3
    {0x39b0,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      7},         //tfm_c_mapping_k4
    /*TNR_TFM_HISTMOTION_BLEND*/
    {0x39b4,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,     32},         //tfm_hism_term1
    {0x39b4,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,   1023},         //tfm_hism_term2
    {0x39b4,    20,      25,        0,        0,      HI_PQ_MODULE_TNR,     28},         //tfm_hism_alpha1
    {0x39b4,    26,      31,        0,        0,      HI_PQ_MODULE_TNR,     26},         //tfm_hism_alpha2
    /*TNR_TFM_PARA*/
    {0x39b8,    0,       0,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_filter_mode
    {0x39b8,    1,       2,         0,        0,      HI_PQ_MODULE_TNR,      1},         //tfm_calc_mode  (fixed)
    {0x39b8,    3,       3,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_scenechange_en  (fixed)
    {0x39b8,    4,       4,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_scenechangeformotion_en  (fixed)
    {0x39b8,    5,       5,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_globalmotion_en (fixed)
    {0x39b8,    6,       6,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_cbcrupdate_mode
    {0x39b8,    7,       7,         0,        0,      HI_PQ_MODULE_TNR,      1},        // 1 //tfm_1dlpf_mode
    {0x39b8,    8,       8,         0,        0,      HI_PQ_MODULE_TNR,      0},         //ymotion_mode
    {0x39b8,    9,       9,         0,        0,      HI_PQ_MODULE_TNR,      1},         //cmotion_mode
    {0x39b8,    10,      17,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_globalmotion  (fixed)
    //{0x39b8,    29,      29,        0,        0,      HI_PQ_MODULE_TNR,      0},         //dither_round
    //{0x39b8,    30,      30,        0,        0,      HI_PQ_MODULE_TNR,      1},         //dither_en_c
    //{0x39b8,    31,      31,        0,        0,      HI_PQ_MODULE_TNR,      1},         //dither_en
    /*TNR_TFM_YMOTION_CK*/
    {0x39bc,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_ymotion_core
    {0x39bc,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tfm_ymotion_k
    /*TNR_TFM_YEG_CK*/
    {0x39c0,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_yedge_core (fixed)
    {0x39c0,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tfm_yedge_k (fixed)
    /*TNR_TM_YMTMEAN_CK*/
    {0x39c4,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_ymtmean_core
    {0x39c4,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     16},         //tm_ymtmean_k
    /*TNR_TYMYCK*/
    {0x39c8,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tymy_core (fixed)
    {0x39c8,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tymy_k (fixed)
    /*TNR_TYMEYCK*/
    {0x39cc,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tymey_core (fixed)
    {0x39cc,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tymey_k (fixed)
    /*TNR_TCMYCK*/
    {0x39d0,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tcmy_core (fixed)
    {0x39d0,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     32},         //tcmy_k (fixed)
    /*TNR_TCMEYCK*/
    {0x39d4,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tcmey_core (fixed)
    {0x39d4,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,      0},         //tcmey_k (fixed)
    /*TNR_TM_CMT_CK*/
    {0x39d8,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_cmt_core
    {0x39d8,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     25},         //tm_cmt_k
    /*TNR_TM_YMT_CK*/
    {0x39dc,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_ymt_core
    {0x39dc,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     28},         //tm_ymt_k
    /*TNR_TM_CMTMEAN_CK*/
    {0x39e0,    0,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //tm_cmtmean_core
    {0x39e0,    10,      19,        0,        0,      HI_PQ_MODULE_TNR,     16},         //tm_cmtmean_k
    /*TNR_SNR_CTRL*/
    {0x39e4,    1,       1,         0,        0,      HI_PQ_MODULE_TNR,      1},         //ynr_1d_en
    {0x39e4,    2,       2,         0,        0,      HI_PQ_MODULE_TNR,      1},         //cnr_1d_en
    {0x39e4,    3,       10,        0,        0,      HI_PQ_MODULE_TNR,      0},         //sencechange  (fixed)
    {0x39e4,    19,      19,        0,        0,      HI_PQ_MODULE_TNR,      0},         //ctrandnoise_en
    {0x39e4,    20,      22,        0,        0,      HI_PQ_MODULE_TNR,      2},       // 2  //ctrandnoise_bit
    {0x39e4,    23,      23,        0,        0,      HI_PQ_MODULE_TNR,      0},         //ytrandnoise_en
    {0x39e4,    24,      26,        0,        0,      HI_PQ_MODULE_TNR,      2},         //ytrandnoise_bit
    /*TNR_BLEND_CTRL*/
    {0x39e8,    2,       9,         0,        0,      HI_PQ_MODULE_TNR,      0},         //s_change (fixed)
    {0x39e8,    10,      10,        0,        0,      HI_PQ_MODULE_TNR,      1},         //cmotion_strength_en
    {0x39e8,    11,      11,        0,        0,      HI_PQ_MODULE_TNR,      1},         //ymotion_strength_en
    /*TNR_Y_FMOTION_MAP*/
    {0x3e40,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,      7},         //data0
    {0x3e40,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,      7},         //data1
    {0x3e40,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,      7},         //data2
    {0x3e40,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,      6},         //data3
    {0x3e44,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,      6},         //data4
    {0x3e44,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,      7},         //data5
    {0x3e44,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,      7},         //data6
    {0x3e44,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,      8},         //data7
    {0x3e48,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,      9},         //data8
    {0x3e48,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,     10},         //data9
    {0x3e48,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,     11},         //data10
    {0x3e48,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,     12},         //data11
    {0x3e4c,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,     13},         //data12
    {0x3e4c,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,     15},         //data13
    {0x3e4c,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,     16},         //data14
    {0x3e4c,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,     17},         //data15
    {0x3e50,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,     18},         //data16
    {0x3e50,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,     19},         //data17
    {0x3e50,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,     20},         //data18
    {0x3e50,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,     21},         //data19
    {0x3e54,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,     22},         //data20
    {0x3e54,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,     23},         //data21
    {0x3e54,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,     24},         //data22
    {0x3e54,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,     25},         //data23
    {0x3e58,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,     26},         //data24
    {0x3e58,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,     27},         //data25
    {0x3e58,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,     28},         //data26
    {0x3e58,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,     29},         //data27
    {0x3e5c,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,     30},         //data28
    {0x3e5c,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,     31},         //data29
    {0x3e5c,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,     31},         //data30
    {0x3e5c,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,     31},         //data31
    /*TNR_C_FMOTION_MAP*/
    {0x3e60,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,      7},         //data0
    {0x3e60,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,      7},         //data1
    {0x3e60,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,      7},         //data2
    {0x3e60,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,      6},         //data3
    {0x3e64,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,      6},         //data4
    {0x3e64,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,      7},         //data5
    {0x3e64,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,      7},         //data6
    {0x3e64,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,      8},         //data7
    {0x3e68,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,      9},         //data8
    {0x3e68,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,     10},         //data9
    {0x3e68,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,     11},         //data10
    {0x3e68,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,     12},         //data11
    {0x3e6c,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,     13},         //data12
    {0x3e6c,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,     15},         //data13
    {0x3e6c,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,     16},         //data14
    {0x3e6c,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,     17},         //data15
    {0x3e70,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,     18},         //data16
    {0x3e70,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,     19},         //data17
    {0x3e70,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,     20},         //data18
    {0x3e70,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,     21},         //data19
    {0x3e74,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,     22},         //data20
    {0x3e74,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,     23},         //data21
    {0x3e74,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,     24},         //data22
    {0x3e74,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,     25},         //data23
    {0x3e78,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,     26},         //data24
    {0x3e78,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,     27},         //data25
    {0x3e78,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,     28},         //data26
    {0x3e78,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,     29},         //data27
    {0x3e7c,    0,       7,         0,        0,      HI_PQ_MODULE_TNR,     30},         //data28
    {0x3e7c,    8,       15,        0,        0,      HI_PQ_MODULE_TNR,     31},         //data29
    {0x3e7c,    16,      23,        0,        0,      HI_PQ_MODULE_TNR,     31},         //data30
    {0x3e7c,    24,      31,        0,        0,      HI_PQ_MODULE_TNR,     31},         //data31

    /************************************SNR*****************************************/
    /*SD source*/
    /*SNR_ENABLE*/
    {0x3000,        23,     27,     1,      0,      HI_PQ_MODULE_SNR,       16},    //snrstrength
    {0x3000,        22,     22,     1,      0,      HI_PQ_MODULE_SNR,       0},     //snrfiltermode
    {0x3000,        21,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratioen
    {0x3000,        19,     20,     1,      0,      HI_PQ_MODULE_SNR,       0},     //edgelpfmode
    {0x3000,        14,     14,     1,      0,      HI_PQ_MODULE_SNR,       0},     //diredgesmoothen
    {0x3000,        12,     13,     1,      0,      HI_PQ_MODULE_SNR,       1},     //debugmode
    {0x3000,        11,     11,     1,      0,      HI_PQ_MODULE_SNR,       0},     //debugmodeen
    {0x3000,        10,     10,     1,      0,      HI_PQ_MODULE_SNR,       0},     //demomodelr
    {0x3000,        9,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //demomodeen
    {0x3000,        7,      7,      1,      0,      HI_PQ_MODULE_SNR,       0},     //blendingmode
    {0x3000,        6,      6,      1,      0,      HI_PQ_MODULE_SNR,       1},     //edgeonlyen
    {0x3000,        5,      5,      1,      0,      HI_PQ_MODULE_SNR,       0},     //motionedgeen
    {0x3000,        4,      4,      1,      0,      HI_PQ_MODULE_SNR,       0},     //colorweighten
    {0x3000,        3,      3,      1,      0,      HI_PQ_MODULE_SNR,       0},     //edgeprelpfen
    {0x3000,        2,      2,      1,      0,      HI_PQ_MODULE_SNR,       1},     //chromasnren
    {0x3000,        1,      1,      1,      0,      HI_PQ_MODULE_SNR,       1},     //lumasnren
    /*SNR_EDGE_RATIO*/
    {0x3004,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       8},     //meanedgeratiox0
    {0x3004,        10,     19,     1,      0,      HI_PQ_MODULE_SNR,       40},    //meanedgeratiox1
    {0x3004,        20,     29,     1,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratiox2
    {0x3008,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       128},   //meanedgeratiox3
    {0x3008,        10,     19,     1,      0,      HI_PQ_MODULE_SNR,       255},   //meanedgeratiox4
    {0x3008,        20,     29,     1,      0,      HI_PQ_MODULE_SNR,       1023},  //meanedgeratiox5
    {0x300c,        0,      6,      1,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy0
    {0x300c,        7,      13,     1,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy1
    {0x300c,        14,     20,     1,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy2
    {0x3010,        0,      6,      1,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy3
    {0x3010,        7,      13,     1,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy4
    {0x3014,        0,      7,      1,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiok0
    {0x3014,        8,      15,     1,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiok1
    {0x3018,        0,      7,      1,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiok2
    {0x3018,        8,      15,     1,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiok3
    {0x301c,        7,      13,     1,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiomin
    {0x301c,        0,      6,      1,      0,      HI_PQ_MODULE_SNR,       127},   //meanedgeratiomax
    /*SNR_EDGE_STR*/
    {0x3020,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       7},     //edgestrmappingx0
    {0x3020,        10,     19,     1,      0,      HI_PQ_MODULE_SNR,       15},    //edgestrmappingx1
    {0x3020,        20,     29,     1,      0,      HI_PQ_MODULE_SNR,       31},    //edgestrmappingx2
    {0x3024,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       63},    //edgestrmappingx3
    {0x3024,        10,     19,     1,      0,      HI_PQ_MODULE_SNR,       127},   //edgestrmappingx4
    {0x3024,        20,     29,     1,      0,      HI_PQ_MODULE_SNR,       255},   //edgestrmappingx5
    {0x3028,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       511},   //edgestrmappingx6
    {0x3028,        10,     19,     1,      0,      HI_PQ_MODULE_SNR,       1023},  //edgestrmappingx7
    {0x302c,        0,      5,      1,      0,      HI_PQ_MODULE_SNR,       0},     //edgestrmappingy0
    {0x302c,        6,      11,     1,      0,      HI_PQ_MODULE_SNR,       4},     //edgestrmappingy1
    {0x302c,        12,     17,     1,      0,      HI_PQ_MODULE_SNR,       10},    //edgestrmappingy2
    {0x302c,        18,     23,     1,      0,      HI_PQ_MODULE_SNR,       18},    //edgestrmappingy3
    {0x3030,        0,      5,      1,      0,      HI_PQ_MODULE_SNR,       30},    //edgestrmappingy4
    {0x3030,        6,      11,     1,      0,      HI_PQ_MODULE_SNR,       46},    //edgestrmappingy5
    {0x3030,        12,     17,     1,      0,      HI_PQ_MODULE_SNR,       63},    //edgestrmappingy6
    {0x3034,        0,      7,      1,      0,      HI_PQ_MODULE_SNR,       128},   //edgestrmappingk0
    {0x3034,        8,      15,     1,      0,      HI_PQ_MODULE_SNR,       96},    //edgestrmappingk1
    {0x3034,        16,     23,     1,      0,      HI_PQ_MODULE_SNR,       64},    //edgestrmappingk2
    {0x3038,        0,      7,      1,      0,      HI_PQ_MODULE_SNR,       48},    //edgestrmappingk3
    {0x3038,        8,      15,     1,      0,      HI_PQ_MODULE_SNR,       32},    //edgestrmappingk4
    {0x3038,        16,     23,     1,      0,      HI_PQ_MODULE_SNR,       17},    //edgestrmappingk5
    {0x303c,        6,      11,     1,      0,      HI_PQ_MODULE_SNR,       63},    //edgestrmappingmax
    {0x303c,        0,      5,      1,      0,      HI_PQ_MODULE_SNR,       0},     //edgestrmappingmin
    /*SNR_FYMAX_VALTH*/
    {0x3040,        0,      4,      1,      0,      HI_PQ_MODULE_SNR,       4},     //sfymaxedgevalthd1
    {0x3040,        5,      9,      1,      0,      HI_PQ_MODULE_SNR,       7},     //sfymaxedgevalthd2
    {0x3040,        10,     14,     1,      0,      HI_PQ_MODULE_SNR,       11},    //sfymaxedgevalthd3
    {0x3044,        0,      3,      1,      0,      HI_PQ_MODULE_SNR,       0},     //sfymaxedgeidxthd1
    {0x3044,        4,      7,      1,      0,      HI_PQ_MODULE_SNR,       0},     //sfymaxedgeidxthd2
    {0x3044,        8,      11,     1,      0,      HI_PQ_MODULE_SNR,       0},     //sfymaxedgeidxthd3
    {0x3048,        0,      4,      1,      0,      HI_PQ_MODULE_SNR,       2},     //sfymeanedgevalthd1
    {0x3048,        5,      9,      1,      0,      HI_PQ_MODULE_SNR,       4},     //sfymeanedgevalthd2
    {0x3048,        10,     14,     1,      0,      HI_PQ_MODULE_SNR,       6},     //sfymeanedgevalthd3
    {0x304c,        0,      4,      1,      0,      HI_PQ_MODULE_SNR,       27},    //wfymaxedgevalthd1
    {0x304c,        5,      9,      1,      0,      HI_PQ_MODULE_SNR,       24},    //wfymaxedgevalthd2
    {0x304c,        10,     14,     1,      0,      HI_PQ_MODULE_SNR,       21},    //wfymaxedgevalthd3
    {0x3050,        0,      3,      1,      0,      HI_PQ_MODULE_SNR,       1},     //wfymaxedgeidxthd1
    {0x3050,        4,      7,      1,      0,      HI_PQ_MODULE_SNR,       2},     //wfymaxedgeidxthd2
    {0x3050,        8,      11,     1,      0,      HI_PQ_MODULE_SNR,       4},     //wfymaxedgeidxthd3
    {0x3054,        0,      4,      1,      0,      HI_PQ_MODULE_SNR,       24},    //wfymeanedgevalthd1
    {0x3054,        5,      9,      1,      0,      HI_PQ_MODULE_SNR,       21},    //wfymeanedgevalthd2
    {0x3054,        10,     14,     1,      0,      HI_PQ_MODULE_SNR,       18},    //wfymeanedgevalthd3
    /*SNR_FCMAX_VALTH*/
    {0x3058,        0,      4,      1,      0,      HI_PQ_MODULE_SNR,       0},     //sfcmaxedgevalthd1
    {0x3058,        5,      9,      1,      0,      HI_PQ_MODULE_SNR,       1},     //sfcmaxedgevalthd2
    {0x305c,        0,      3,      1,      0,      HI_PQ_MODULE_SNR,      15},     //sfcmaxedgeidxthd1
    {0x305c,        4,      7,      1,      0,      HI_PQ_MODULE_SNR,      15},     //sfcmaxedgeidxthd2
    {0x3060,        0,      4,      1,      0,      HI_PQ_MODULE_SNR,       0},     //sfcmeanedgevalthd1
    {0x3060,        5,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //sfcmeanedgevalthd2
    {0x3064,        0,      4,      1,      0,      HI_PQ_MODULE_SNR,       7},    //wfcmaxedgevalthd1
    {0x3064,        5,      9,      1,      0,      HI_PQ_MODULE_SNR,       4},     //wfcmaxedgevalthd2
    {0x3068,        0,      3,      1,      0,      HI_PQ_MODULE_SNR,       0},     //wfcmaxedgeidxthd1
    {0x3068,        4,      7,      1,      0,      HI_PQ_MODULE_SNR,       0},     //wfcmaxedgeidxthd2
    {0x306c,        0,      4,      1,      0,      HI_PQ_MODULE_SNR,       2},    //wfcmeanedgevalthd1
    {0x306c,        5,      9,      1,      0,      HI_PQ_MODULE_SNR,       1},     //wfcmeanedgevalthd2
    /*SNR_FILTER_TYPE*/
    {0x3070,        3,      5,      1,      0,      HI_PQ_MODULE_SNR,       0},     //cdefaultfiltertype
    {0x3070,        0,      2,      1,      0,      HI_PQ_MODULE_SNR,       3},     //ydefaultfiltertype
    /*SNR_NOISE_CORE*/
    {0x3074,        5,      10,     1,      0,      HI_PQ_MODULE_SNR,       32},    //nonoisegain
    {0x3074,        0,      4,      1,      0,      HI_PQ_MODULE_SNR,       0},     //noisecore
    {0x3078,        1,      5,      1,      0,      HI_PQ_MODULE_SNR,       3},     //randyinnoiselevel
    {0x3078,        0,      0,      1,      0,      HI_PQ_MODULE_SNR,       1},     //addrandyinnoiseen
    /*SNR_ABS_SUM*/
    {0x307c,        16,     27,     1,      0,      HI_PQ_MODULE_SNR,       0},     //diffabssumofst
    {0x307c,        4,      15,     1,      0,      HI_PQ_MODULE_SNR,       0},     //sclabssum
    {0x307c,        0,      3,      1,      0,      HI_PQ_MODULE_SNR,       7},     //edgeindicatorsel
    {0x3080,        16,     31,     1,      0,      HI_PQ_MODULE_SNR,       0},     //winactdirdiffthr
    {0x3080,        0,      15,     1,      0,      HI_PQ_MODULE_SNR,       0},     //winactthr
    /*SNR_DIR_THR*/
    {0x3084,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr0
    {0x3084,        10,     19,     1,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr1
    {0x3088,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr2
    {0x3088,        10,     19,     1,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr3
    {0x308c,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr4
    {0x308c,        10,     19,     1,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr5
    {0x3090,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr6
    {0x3090,        10,     19,     1,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr7
    /*SNR_THR_DIR*/
    {0x3094,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir0
    {0x3094,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir0
    {0x3098,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir0
    {0x3098,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir0
    {0x309c,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir1
    {0x309c,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir1
    {0x30a0,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir1
    {0x30a0,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir1
    {0x30a4,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir2
    {0x30a4,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir2
    {0x30a8,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir2
    {0x30a8,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir2
    {0x30ac,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir3
    {0x30ac,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir3
    {0x30b0,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir3
    {0x30b0,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir3
    {0x30b4,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir4
    {0x30b4,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir4
    {0x30b8,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir4
    {0x30b8,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir4
    {0x30bc,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir5
    {0x30bc,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir5
    {0x30c0,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir5
    {0x30c0,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir5
    {0x30c4,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir6
    {0x30c4,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir6
    {0x30c8,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir6
    {0x30c8,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir6
    {0x30cc,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir7
    {0x30cc,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir7
    {0x30d0,        10,     21,     1,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir7
    {0x30d0,        0,      9,      1,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir7

    /*HD source*/
    /*SNR_ENABLE*/
    {0x3000,        23,     27,     2,      0,      HI_PQ_MODULE_SNR,       16},    //snrstrength
    {0x3000,        22,     22,     2,      0,      HI_PQ_MODULE_SNR,       0},     //snrfiltermode
    {0x3000,        21,     21,     2,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratioen
    {0x3000,        19,     20,     2,      0,      HI_PQ_MODULE_SNR,       0},     //edgelpfmode
    {0x3000,        14,     14,     2,      0,      HI_PQ_MODULE_SNR,       1},     //diredgesmoothen
    {0x3000,        12,     13,     2,      0,      HI_PQ_MODULE_SNR,       1},     //debugmode
    {0x3000,        11,     11,     2,      0,      HI_PQ_MODULE_SNR,       0},     //debugmodeen
    {0x3000,        10,     10,     2,      0,      HI_PQ_MODULE_SNR,       0},     //demomodelr
    {0x3000,        9,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //demomodeen
    {0x3000,        7,      7,      2,      0,      HI_PQ_MODULE_SNR,       0},     //blendingmode
    {0x3000,        6,      6,      2,      0,      HI_PQ_MODULE_SNR,       1},     //edgeonlyen
    {0x3000,        5,      5,      2,      0,      HI_PQ_MODULE_SNR,       0},     //motionedgeen
    {0x3000,        4,      4,      2,      0,      HI_PQ_MODULE_SNR,       0},     //colorweighten
    {0x3000,        3,      3,      2,      0,      HI_PQ_MODULE_SNR,       0},     //edgeprelpfen
    {0x3000,        2,      2,      2,      0,      HI_PQ_MODULE_SNR,       1},     //chromasnren
    {0x3000,        1,      1,      2,      0,      HI_PQ_MODULE_SNR,       1},     //lumasnren
    /*SNR_EDGE_RATIO*/
    {0x3004,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       8},     //meanedgeratiox0
    {0x3004,        10,     19,     2,      0,      HI_PQ_MODULE_SNR,       40},    //meanedgeratiox1
    {0x3004,        20,     29,     2,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratiox2
    {0x3008,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       128},   //meanedgeratiox3
    {0x3008,        10,     19,     2,      0,      HI_PQ_MODULE_SNR,       255},   //meanedgeratiox4
    {0x3008,        20,     29,     2,      0,      HI_PQ_MODULE_SNR,       1023},  //meanedgeratiox5
    {0x300c,        0,      6,      2,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy0
    {0x300c,        7,      13,     2,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy1
    {0x300c,        14,     20,     2,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy2
    {0x3010,        0,      6,      2,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy3
    {0x3010,        7,      13,     2,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy4
    {0x3014,        0,      7,      2,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiok0
    {0x3014,        8,      15,     2,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiok1
    {0x3018,        0,      7,      2,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiok2
    {0x3018,        8,      15,     2,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiok3
    {0x301c,        7,      13,     2,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiomin
    {0x301c,        0,      6,      2,      0,      HI_PQ_MODULE_SNR,       127},   //meanedgeratiomax
    /*SNR_EDGE_STR*/
    {0x3020,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       7},     //edgestrmappingx0
    {0x3020,        10,     19,     2,      0,      HI_PQ_MODULE_SNR,       15},    //edgestrmappingx1
    {0x3020,        20,     29,     2,      0,      HI_PQ_MODULE_SNR,       31},    //edgestrmappingx2
    {0x3024,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       63},    //edgestrmappingx3
    {0x3024,        10,     19,     2,      0,      HI_PQ_MODULE_SNR,       127},   //edgestrmappingx4
    {0x3024,        20,     29,     2,      0,      HI_PQ_MODULE_SNR,       255},   //edgestrmappingx5
    {0x3028,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       511},   //edgestrmappingx6
    {0x3028,        10,     19,     2,      0,      HI_PQ_MODULE_SNR,       1023},  //edgestrmappingx7
    {0x302c,        0,      5,      2,      0,      HI_PQ_MODULE_SNR,       0},     //edgestrmappingy0
    {0x302c,        6,      11,     2,      0,      HI_PQ_MODULE_SNR,       4},     //edgestrmappingy1
    {0x302c,        12,     17,     2,      0,      HI_PQ_MODULE_SNR,       10},    //edgestrmappingy2
    {0x302c,        18,     23,     2,      0,      HI_PQ_MODULE_SNR,       18},    //edgestrmappingy3
    {0x3030,        0,      5,      2,      0,      HI_PQ_MODULE_SNR,       30},    //edgestrmappingy4
    {0x3030,        6,      11,     2,      0,      HI_PQ_MODULE_SNR,       46},    //edgestrmappingy5
    {0x3030,        12,     17,     2,      0,      HI_PQ_MODULE_SNR,       63},    //edgestrmappingy6
    {0x3034,        0,      7,      2,      0,      HI_PQ_MODULE_SNR,       128},   //edgestrmappingk0
    {0x3034,        8,      15,     2,      0,      HI_PQ_MODULE_SNR,       96},    //edgestrmappingk1
    {0x3034,        16,     23,     2,      0,      HI_PQ_MODULE_SNR,       64},    //edgestrmappingk2
    {0x3038,        0,      7,      2,      0,      HI_PQ_MODULE_SNR,       48},    //edgestrmappingk3
    {0x3038,        8,      15,     2,      0,      HI_PQ_MODULE_SNR,       32},    //edgestrmappingk4
    {0x3038,        16,     23,     2,      0,      HI_PQ_MODULE_SNR,       17},    //edgestrmappingk5
    {0x303c,        6,      11,     2,      0,      HI_PQ_MODULE_SNR,       63},    //edgestrmappingmax
    {0x303c,        0,      5,      2,      0,      HI_PQ_MODULE_SNR,       0},     //edgestrmappingmin
    /*SNR_FYMAX_VALTH*/
    {0x3040,        0,      4,      2,      0,      HI_PQ_MODULE_SNR,       4},     //sfymaxedgevalthd1
    {0x3040,        5,      9,      2,      0,      HI_PQ_MODULE_SNR,       7},     //sfymaxedgevalthd2
    {0x3040,        10,     14,     2,      0,      HI_PQ_MODULE_SNR,       11},    //sfymaxedgevalthd3
    {0x3044,        0,      3,      2,      0,      HI_PQ_MODULE_SNR,       0},     //sfymaxedgeidxthd1
    {0x3044,        4,      7,      2,      0,      HI_PQ_MODULE_SNR,       0},     //sfymaxedgeidxthd2
    {0x3044,        8,      11,     2,      0,      HI_PQ_MODULE_SNR,       0},     //sfymaxedgeidxthd3
    {0x3048,        0,      4,      2,      0,      HI_PQ_MODULE_SNR,       2},     //sfymeanedgevalthd1
    {0x3048,        5,      9,      2,      0,      HI_PQ_MODULE_SNR,       4},     //sfymeanedgevalthd2
    {0x3048,        10,     14,     2,      0,      HI_PQ_MODULE_SNR,       6},     //sfymeanedgevalthd3
    {0x304c,        0,      4,      2,      0,      HI_PQ_MODULE_SNR,       27},    //wfymaxedgevalthd1
    {0x304c,        5,      9,      2,      0,      HI_PQ_MODULE_SNR,       24},    //wfymaxedgevalthd2
    {0x304c,        10,     14,     2,      0,      HI_PQ_MODULE_SNR,       21},    //wfymaxedgevalthd3
    {0x3050,        0,      3,      2,      0,      HI_PQ_MODULE_SNR,       1},     //wfymaxedgeidxthd1
    {0x3050,        4,      7,      2,      0,      HI_PQ_MODULE_SNR,       2},     //wfymaxedgeidxthd2
    {0x3050,        8,      11,     2,      0,      HI_PQ_MODULE_SNR,       4},     //wfymaxedgeidxthd3
    {0x3054,        0,      4,      2,      0,      HI_PQ_MODULE_SNR,       24},    //wfymeanedgevalthd1
    {0x3054,        5,      9,      2,      0,      HI_PQ_MODULE_SNR,       21},    //wfymeanedgevalthd2
    {0x3054,        10,     14,     2,      0,      HI_PQ_MODULE_SNR,       18},    //wfymeanedgevalthd3
    /*SNR_FCMAX_VALTH*/
    {0x3058,        0,      4,      2,      0,      HI_PQ_MODULE_SNR,       0},     //sfcmaxedgevalthd1
    {0x3058,        5,      9,      2,      0,      HI_PQ_MODULE_SNR,       1},     //sfcmaxedgevalthd2
    {0x305c,        0,      3,      2,      0,      HI_PQ_MODULE_SNR,      15},     //sfcmaxedgeidxthd1
    {0x305c,        4,      7,      2,      0,      HI_PQ_MODULE_SNR,      15},     //sfcmaxedgeidxthd2
    {0x3060,        0,      4,      2,      0,      HI_PQ_MODULE_SNR,       0},     //sfcmeanedgevalthd1
    {0x3060,        5,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //sfcmeanedgevalthd2
    {0x3064,        0,      4,      2,      0,      HI_PQ_MODULE_SNR,       7},    //wfcmaxedgevalthd1
    {0x3064,        5,      9,      2,      0,      HI_PQ_MODULE_SNR,       4},     //wfcmaxedgevalthd2
    {0x3068,        0,      3,      2,      0,      HI_PQ_MODULE_SNR,       0},     //wfcmaxedgeidxthd1
    {0x3068,        4,      7,      2,      0,      HI_PQ_MODULE_SNR,       0},     //wfcmaxedgeidxthd2
    {0x306c,        0,      4,      2,      0,      HI_PQ_MODULE_SNR,       2},    //wfcmeanedgevalthd1
    {0x306c,        5,      9,      2,      0,      HI_PQ_MODULE_SNR,       1},     //wfcmeanedgevalthd2
    /*SNR_FILTER_TYPE*/
    {0x3070,        3,      5,      2,      0,      HI_PQ_MODULE_SNR,       0},     //cdefaultfiltertype
    {0x3070,        0,      2,      2,      0,      HI_PQ_MODULE_SNR,       3},     //ydefaultfiltertype
    /*SNR_NOISE_CORE*/
    {0x3074,        5,      10,     2,      0,      HI_PQ_MODULE_SNR,       32},    //nonoisegain
    {0x3074,        0,      4,      2,      0,      HI_PQ_MODULE_SNR,       0},     //noisecore
    {0x3078,        1,      5,      2,      0,      HI_PQ_MODULE_SNR,       3},     //randyinnoiselevel
    {0x3078,        0,      0,      2,      0,      HI_PQ_MODULE_SNR,       1},     //addrandyinnoiseen
    /*SNR_ABS_SUM*/
    {0x307c,        16,     27,     2,      0,      HI_PQ_MODULE_SNR,       0},     //diffabssumofst
    {0x307c,        4,      15,     2,      0,      HI_PQ_MODULE_SNR,       0},     //sclabssum
    {0x307c,        0,      3,      2,      0,      HI_PQ_MODULE_SNR,       7},     //edgeindicatorsel
    {0x3080,        16,     31,     2,      0,      HI_PQ_MODULE_SNR,       0},     //winactdirdiffthr
    {0x3080,        0,      15,     2,      0,      HI_PQ_MODULE_SNR,       0},     //winactthr
    /*SNR_DIR_THR*/
    {0x3084,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr0
    {0x3084,        10,     19,     2,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr1
    {0x3088,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr2
    {0x3088,        10,     19,     2,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr3
    {0x308c,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr4
    {0x308c,        10,     19,     2,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr5
    {0x3090,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr6
    {0x3090,        10,     19,     2,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr7
    /*SNR_THR_DIR*/
    {0x3094,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir0
    {0x3094,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir0
    {0x3098,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,     500},     //winmeanhordiffthrdir0
    {0x3098,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir0
    {0x309c,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir1
    {0x309c,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir1
    {0x30a0,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,     500},     //winmeanhordiffthrdir1
    {0x30a0,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir1
    {0x30a4,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir2
    {0x30a4,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir2
    {0x30a8,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,     500},     //winmeanhordiffthrdir2
    {0x30a8,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir2
    {0x30ac,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir3
    {0x30ac,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir3
    {0x30b0,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,     500},     //winmeanhordiffthrdir3
    {0x30b0,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir3
    {0x30b4,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir4
    {0x30b4,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir4
    {0x30b8,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,     500},     //winmeanhordiffthrdir4
    {0x30b8,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir4
    {0x30bc,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir5
    {0x30bc,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir5
    {0x30c0,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,     500},     //winmeanhordiffthrdir5
    {0x30c0,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir5
    {0x30c4,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir6
    {0x30c4,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir6
    {0x30c8,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,     500},     //winmeanhordiffthrdir6
    {0x30c8,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir6
    {0x30cc,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir7
    {0x30cc,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir7
    {0x30d0,        10,     21,     2,      0,      HI_PQ_MODULE_SNR,     500},     //winmeanhordiffthrdir7
    {0x30d0,        0,      9,      2,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir7

    /*UHD source*/
    /*SNR_ENABLE*/
    {0x3000,        23,     27,     3,      0,      HI_PQ_MODULE_SNR,       17},    //snrstrength
    {0x3000,        22,     22,     3,      0,      HI_PQ_MODULE_SNR,       0},     //snrfiltermode
    {0x3000,        21,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratioen
    {0x3000,        19,     20,     3,      0,      HI_PQ_MODULE_SNR,       0},     //edgelpfmode
    {0x3000,        14,     14,     3,      0,      HI_PQ_MODULE_SNR,       1},     //diredgesmoothen
    {0x3000,        12,     13,     3,      0,      HI_PQ_MODULE_SNR,       1},     //debugmode
    {0x3000,        11,     11,     3,      0,      HI_PQ_MODULE_SNR,       0},     //debugmodeen
    {0x3000,        10,     10,     3,      0,      HI_PQ_MODULE_SNR,       0},     //demomodelr
    {0x3000,        9,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //demomodeen
    {0x3000,        7,      7,      3,      0,      HI_PQ_MODULE_SNR,       0},     //blendingmode
    {0x3000,        6,      6,      3,      0,      HI_PQ_MODULE_SNR,       1},     //edgeonlyen
    {0x3000,        5,      5,      3,      0,      HI_PQ_MODULE_SNR,       0},     //motionedgeen
    {0x3000,        4,      4,      3,      0,      HI_PQ_MODULE_SNR,       0},     //colorweighten
    {0x3000,        3,      3,      3,      0,      HI_PQ_MODULE_SNR,       1},     //edgeprelpfen
    {0x3000,        2,      2,      3,      0,      HI_PQ_MODULE_SNR,       1},     //chromasnren
    {0x3000,        1,      1,      3,      0,      HI_PQ_MODULE_SNR,       1},     //lumasnren
    /*SNR_EDGE_RATIO*/
    {0x3004,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       8},     //meanedgeratiox0
    {0x3004,        10,     19,     3,      0,      HI_PQ_MODULE_SNR,       40},    //meanedgeratiox1
    {0x3004,        20,     29,     3,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratiox2
    {0x3008,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       128},   //meanedgeratiox3
    {0x3008,        10,     19,     3,      0,      HI_PQ_MODULE_SNR,       255},   //meanedgeratiox4
    {0x3008,        20,     29,     3,      0,      HI_PQ_MODULE_SNR,       1023},  //meanedgeratiox5
    {0x300c,        0,      6,      3,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy0
    {0x300c,        7,      13,     3,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy1
    {0x300c,        14,     20,     3,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy2
    {0x3010,        0,      6,      3,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy3
    {0x3010,        7,      13,     3,      0,      HI_PQ_MODULE_SNR,       64},    //meanedgeratioy4
    {0x3014,        0,      7,      3,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiok0
    {0x3014,        8,      15,     3,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiok1
    {0x3018,        0,      7,      3,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiok2
    {0x3018,        8,      15,     3,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiok3
    {0x301c,        7,      13,     3,      0,      HI_PQ_MODULE_SNR,       0},     //meanedgeratiomin
    {0x301c,        0,      6,      3,      0,      HI_PQ_MODULE_SNR,       127},   //meanedgeratiomax
    /*SNR_EDGE_STR*/
    {0x3020,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       7},     //edgestrmappingx0
    {0x3020,        10,     19,     3,      0,      HI_PQ_MODULE_SNR,       15},    //edgestrmappingx1
    {0x3020,        20,     29,     3,      0,      HI_PQ_MODULE_SNR,       31},    //edgestrmappingx2
    {0x3024,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       63},    //edgestrmappingx3
    {0x3024,        10,     19,     3,      0,      HI_PQ_MODULE_SNR,       127},   //edgestrmappingx4
    {0x3024,        20,     29,     3,      0,      HI_PQ_MODULE_SNR,       255},   //edgestrmappingx5
    {0x3028,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       511},   //edgestrmappingx6
    {0x3028,        10,     19,     3,      0,      HI_PQ_MODULE_SNR,       1023},  //edgestrmappingx7
    {0x302c,        0,      5,      3,      0,      HI_PQ_MODULE_SNR,       0},     //edgestrmappingy0
    {0x302c,        6,      11,     3,      0,      HI_PQ_MODULE_SNR,       4},     //edgestrmappingy1
    {0x302c,        12,     17,     3,      0,      HI_PQ_MODULE_SNR,       10},    //edgestrmappingy2
    {0x302c,        18,     23,     3,      0,      HI_PQ_MODULE_SNR,       18},    //edgestrmappingy3
    {0x3030,        0,      5,      3,      0,      HI_PQ_MODULE_SNR,       30},    //edgestrmappingy4
    {0x3030,        6,      11,     3,      0,      HI_PQ_MODULE_SNR,       46},    //edgestrmappingy5
    {0x3030,        12,     17,     3,      0,      HI_PQ_MODULE_SNR,       63},    //edgestrmappingy6
    {0x3034,        0,      7,      3,      0,      HI_PQ_MODULE_SNR,       128},   //edgestrmappingk0
    {0x3034,        8,      15,     3,      0,      HI_PQ_MODULE_SNR,       96},    //edgestrmappingk1
    {0x3034,        16,     23,     3,      0,      HI_PQ_MODULE_SNR,       64},    //edgestrmappingk2
    {0x3038,        0,      7,      3,      0,      HI_PQ_MODULE_SNR,       48},    //edgestrmappingk3
    {0x3038,        8,      15,     3,      0,      HI_PQ_MODULE_SNR,       32},    //edgestrmappingk4
    {0x3038,        16,     23,     3,      0,      HI_PQ_MODULE_SNR,       17},    //edgestrmappingk5
    {0x303c,        6,      11,     3,      0,      HI_PQ_MODULE_SNR,       63},    //edgestrmappingmax
    {0x303c,        0,      5,      3,      0,      HI_PQ_MODULE_SNR,       0},     //edgestrmappingmin
    /*SNR_FYMAX_VALTH*/
    {0x3040,        0,      4,      3,      0,      HI_PQ_MODULE_SNR,       4},     //sfymaxedgevalthd1
    {0x3040,        5,      9,      3,      0,      HI_PQ_MODULE_SNR,       7},     //sfymaxedgevalthd2
    {0x3040,        10,     14,     3,      0,      HI_PQ_MODULE_SNR,       11},    //sfymaxedgevalthd3
    {0x3044,        0,      3,      3,      0,      HI_PQ_MODULE_SNR,       0},     //sfymaxedgeidxthd1
    {0x3044,        4,      7,      3,      0,      HI_PQ_MODULE_SNR,       0},     //sfymaxedgeidxthd2
    {0x3044,        8,      11,     3,      0,      HI_PQ_MODULE_SNR,       0},     //sfymaxedgeidxthd3
    {0x3048,        0,      4,      3,      0,      HI_PQ_MODULE_SNR,       2},     //sfymeanedgevalthd1
    {0x3048,        5,      9,      3,      0,      HI_PQ_MODULE_SNR,       4},     //sfymeanedgevalthd2
    {0x3048,        10,     14,     3,      0,      HI_PQ_MODULE_SNR,       6},     //sfymeanedgevalthd3
    {0x304c,        0,      4,      3,      0,      HI_PQ_MODULE_SNR,       27},    //wfymaxedgevalthd1
    {0x304c,        5,      9,      3,      0,      HI_PQ_MODULE_SNR,       24},    //wfymaxedgevalthd2
    {0x304c,        10,     14,     3,      0,      HI_PQ_MODULE_SNR,       21},    //wfymaxedgevalthd3
    {0x3050,        0,      3,      3,      0,      HI_PQ_MODULE_SNR,       1},     //wfymaxedgeidxthd1
    {0x3050,        4,      7,      3,      0,      HI_PQ_MODULE_SNR,       2},     //wfymaxedgeidxthd2
    {0x3050,        8,      11,     3,      0,      HI_PQ_MODULE_SNR,       4},     //wfymaxedgeidxthd3
    {0x3054,        0,      4,      3,      0,      HI_PQ_MODULE_SNR,       24},    //wfymeanedgevalthd1
    {0x3054,        5,      9,      3,      0,      HI_PQ_MODULE_SNR,       21},    //wfymeanedgevalthd2
    {0x3054,        10,     14,     3,      0,      HI_PQ_MODULE_SNR,       18},    //wfymeanedgevalthd3
    /*SNR_FCMAX_VALTH*/
    {0x3058,        0,      4,      3,      0,      HI_PQ_MODULE_SNR,       0},     //sfcmaxedgevalthd1
    {0x3058,        5,      9,      3,      0,      HI_PQ_MODULE_SNR,       2},     //sfcmaxedgevalthd2
    {0x305c,        0,      3,      3,      0,      HI_PQ_MODULE_SNR,       0},     //sfcmaxedgeidxthd1
    {0x305c,        4,      7,      3,      0,      HI_PQ_MODULE_SNR,       0},     //sfcmaxedgeidxthd2
    {0x3060,        0,      4,      3,      0,      HI_PQ_MODULE_SNR,       0},     //sfcmeanedgevalthd1
    {0x3060,        5,      9,      3,      0,      HI_PQ_MODULE_SNR,       1},     //sfcmeanedgevalthd2
    {0x3064,        0,      4,      3,      0,      HI_PQ_MODULE_SNR,       15},    //wfcmaxedgevalthd1
    {0x3064,        5,      9,      3,      0,      HI_PQ_MODULE_SNR,       9},     //wfcmaxedgevalthd2
    {0x3068,        0,      3,      3,      0,      HI_PQ_MODULE_SNR,       2},     //wfcmaxedgeidxthd1
    {0x3068,        4,      7,      3,      0,      HI_PQ_MODULE_SNR,       4},     //wfcmaxedgeidxthd2
    {0x306c,        0,      4,      3,      0,      HI_PQ_MODULE_SNR,       12},    //wfcmeanedgevalthd1
    {0x306c,        5,      9,      3,      0,      HI_PQ_MODULE_SNR,       6},     //wfcmeanedgevalthd2
    /*SNR_FILTER_TYPE*/
    {0x3070,        3,      5,      3,      0,      HI_PQ_MODULE_SNR,       3},     //cdefaultfiltertype
    {0x3070,        0,      2,      3,      0,      HI_PQ_MODULE_SNR,       3},     //ydefaultfiltertype
    /*SNR_NOISE_CORE*/
    {0x3074,        5,      10,     3,      0,      HI_PQ_MODULE_SNR,       32},    //nonoisegain
    {0x3074,        0,      4,      3,      0,      HI_PQ_MODULE_SNR,       4},     //noisecore
    {0x3078,        1,      5,      3,      0,      HI_PQ_MODULE_SNR,       7},     //randyinnoiselevel
    {0x3078,        0,      0,      3,      0,      HI_PQ_MODULE_SNR,       1},     //addrandyinnoiseen
    /*SNR_ABS_SUM*/
    {0x307c,        16,     27,     3,      0,      HI_PQ_MODULE_SNR,       0},     //diffabssumofst
    {0x307c,        4,      15,     3,      0,      HI_PQ_MODULE_SNR,       0},     //sclabssum
    {0x307c,        0,      3,      3,      0,      HI_PQ_MODULE_SNR,       3},     //edgeindicatorsel
    {0x3080,        16,     31,     3,      0,      HI_PQ_MODULE_SNR,       0},     //winactdirdiffthr
    {0x3080,        0,      15,     3,      0,      HI_PQ_MODULE_SNR,       0},     //winactthr
    /*SNR_DIR_THR*/
    {0x3084,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr0
    {0x3084,        10,     19,     3,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr1
    {0x3088,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr2
    {0x3088,        10,     19,     3,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr3
    {0x308c,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr4
    {0x308c,        10,     19,     3,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr5
    {0x3090,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr6
    {0x3090,        10,     19,     3,      0,      HI_PQ_MODULE_SNR,       0},     //pixdirstrthr7
    /*SNR_THR_DIR*/
    {0x3094,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir0
    {0x3094,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir0
    {0x3098,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir0
    {0x3098,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir0
    {0x309c,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir1
    {0x309c,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir1
    {0x30a0,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir1
    {0x30a0,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir1
    {0x30a4,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir2
    {0x30a4,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir2
    {0x30a8,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir2
    {0x30a8,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir2
    {0x30ac,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir3
    {0x30ac,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir3
    {0x30b0,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir3
    {0x30b0,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir3
    {0x30b4,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir4
    {0x30b4,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir4
    {0x30b8,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir4
    {0x30b8,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir4
    {0x30bc,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir5
    {0x30bc,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir5
    {0x30c0,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir5
    {0x30c0,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir5
    {0x30c4,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir6
    {0x30c4,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir6
    {0x30c8,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir6
    {0x30c8,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir6
    {0x30cc,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //mediff3thrdir7
    {0x30cc,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntmintenthrdir7
    {0x30d0,        10,     21,     3,      0,      HI_PQ_MODULE_SNR,       0},     //winmeanhordiffthrdir7
    {0x30d0,        0,      9,      3,      0,      HI_PQ_MODULE_SNR,       0},     //mediffcntminnorthrdir7

    /***********************************************DEI***********************************************/
    /*VPSS_DIECTRL*/
    {0x1000,        22,     22,     0,      0,      HI_PQ_MODULE_DEI,       0},     //mc_only
    {0x1000,        21,     21,     0,      0,      HI_PQ_MODULE_DEI,       0},     //ma_only
    {0x1000,        29,     29,     0,      0,      HI_PQ_MODULE_DEI,       0},     //die_out_sel_l
    {0x1000,        28,     28,     0,      0,      HI_PQ_MODULE_DEI,       0},     //die_out_sel_c
    {0x1000,        16,     16,     0,      0,      HI_PQ_MODULE_DEI,       0},     //stinfo_stop
    /*VPSS_DIECTRL_readonly*/
    {0x1000,        26,     27,     0,      0,      HI_PQ_MODULE_DEI,       1},     //die_l_mode
    {0x1000,        24,     25,     0,      0,      HI_PQ_MODULE_DEI,       1},     //die_c_mode
    {0x1000,        17,     17,     0,      0,      HI_PQ_MODULE_DEI,       0},     //die_rst
    /*VPSS_DIELMA2*/
    {0x1004,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       8},     //chroma_mf_offset
    {0x1004,        7,      7,      0,      0,      HI_PQ_MODULE_DEI,       1},     //rec_mode_en
    {0x1004,        6,      6,      0,      0,      HI_PQ_MODULE_DEI,       0},     //chroma_mf_max
    {0x1004,        5,      5,      0,      0,      HI_PQ_MODULE_DEI,       0},     //luma_mf_max
    {0x1004,        4,      4,      0,      0,      HI_PQ_MODULE_DEI,       1},     //motion_iir_en
    {0x1004,        2,      2,      0,      0,      HI_PQ_MODULE_DEI,       0},     //luma_scesdf_max
    {0x1004,        1,      1,      0,      0,      HI_PQ_MODULE_DEI,       1},     //frame_motion_smooth_en
    {0x1004,        0,      0,      0,      0,      HI_PQ_MODULE_DEI,       0},     //recmode_frmfld_blend_mode
    /*VPSS_DIEINTEN*/
    {0x1008,        16,     31,     0,      0,      HI_PQ_MODULE_DEI,       -320},  //ver_min_inten
    {0x1008,        8,      11,     0,      0,      HI_PQ_MODULE_DEI,       2},     //dir_inten_ver
    /*VPSS_DIESCALE*/
    {0x100c,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       2},     //range_scale
    /*DIECHECK*/
    {0x1010,        16,     19,     0,      0,      HI_PQ_MODULE_DEI,       8},     //ck1_gain
    {0x1010,        8,      11,     0,      0,      HI_PQ_MODULE_DEI,       2},     //ck1_range_gain
    {0x1010,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       30},    //ck1_max_range
    {0x1014,        16,     19,     0,      0,      HI_PQ_MODULE_DEI,       8},     //ck2_gain
    {0x1014,        8,      11,     0,      0,      HI_PQ_MODULE_DEI,       2},     //ck2_range_gain
    {0x1014,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       30},    //ck2_max_range
    /*DIEDIR*/
    {0x1024,        16,     21,     0,      0,      HI_PQ_MODULE_DEI,       3},     //dir14_mult
    {0x1024,        8,      13,     0,      0,      HI_PQ_MODULE_DEI,       5},     //dir13_mult
    {0x1024,        0,      5,      0,      0,      HI_PQ_MODULE_DEI,       5},     //dir12_mult
    {0x1020,        24,     29,     0,      0,      HI_PQ_MODULE_DEI,       6},     //dir11_mult
    {0x1020,        16,     21,     0,      0,      HI_PQ_MODULE_DEI,       7},     //dir10_mult
    {0x1020,        8,      13,     0,      0,      HI_PQ_MODULE_DEI,       8},     //dir9_mult
    {0x1020,        0,      5,      0,      0,      HI_PQ_MODULE_DEI,       9},     //dir8_mult
    {0x101c,        24,     29,     0,      0,      HI_PQ_MODULE_DEI,       11},    //dir7_mult
    {0x101c,        16,     21,     0,      0,      HI_PQ_MODULE_DEI,       12},    //dir6_mult
    {0x101c,        8,      13,     0,      0,      HI_PQ_MODULE_DEI,       15},    //dir5_mult
    {0x101c,        0,      5,      0,      0,      HI_PQ_MODULE_DEI,       18},    //dir4_mult
    {0x1018,        24,     29,     0,      0,      HI_PQ_MODULE_DEI,       27},    //dir3_mult
    {0x1018,        16,     21,     0,      0,      HI_PQ_MODULE_DEI,       32},    //dir2_mult
    {0x1018,        8,      13,     0,      0,      HI_PQ_MODULE_DEI,       24},    //dir1_mult
    {0x1018,        0,      5,      0,      0,      HI_PQ_MODULE_DEI,       40},    //dir0_mult
    /*VPSS_DIEINTPSCL0*/
    {0x1030,        24,     27,     0,      0,      HI_PQ_MODULE_DEI,       8},     //intp_scale_ratio_15
    {0x1030,        20,     23,     0,      0,      HI_PQ_MODULE_DEI,       8},     //intp_scale_ratio_14
    {0x1030,        16,     19,     0,      0,      HI_PQ_MODULE_DEI,       8},     //intp_scale_ratio_13
    {0x1030,        12,     15,     0,      0,      HI_PQ_MODULE_DEI,       8},     //intp_scale_ratio_12
    {0x1030,        8,      11,     0,      0,      HI_PQ_MODULE_DEI,       8},     //intp_scale_ratio_11
    {0x1030,        4,      7,      0,      0,      HI_PQ_MODULE_DEI,       8},     //intp_scale_ratio_10
    {0x1030,        0,      3,      0,      0,      HI_PQ_MODULE_DEI,       8},     //intp_scale_ratio_9
    {0x102c,        28,     31,     0,      0,      HI_PQ_MODULE_DEI,       8},     //intp_scale_ratio_8
    {0x102c,        24,     27,     0,      0,      HI_PQ_MODULE_DEI,       7},     //intp_scale_ratio_7
    {0x102c,        20,     23,     0,      0,      HI_PQ_MODULE_DEI,       7},     //intp_scale_ratio_6
    {0x102c,        16,     19,     0,      0,      HI_PQ_MODULE_DEI,       6},     //intp_scale_ratio_5
    {0x102c,        12,     15,     0,      0,      HI_PQ_MODULE_DEI,       6},     //intp_scale_ratio_4
    {0x102c,        8,      11,     0,      0,      HI_PQ_MODULE_DEI,       5},     //intp_scale_ratio_3
    {0x102c,        4,      7,      0,      0,      HI_PQ_MODULE_DEI,       5},     //intp_scale_ratio_2
    {0x102c,        0,      3,      0,      0,      HI_PQ_MODULE_DEI,       6},     //intp_scale_ratio_1
    /*VPSS_DIEDIRTHD*/
    {0x1034,        16,     31,     0,      0,      HI_PQ_MODULE_DEI,       5000},  //strength_thd
    {0x1034,        13,     13,     0,      0,      HI_PQ_MODULE_DEI,       0},     //hor_edge_en
    {0x1034,        12,     12,     0,      0,      HI_PQ_MODULE_DEI,       0},     //edge_mode
    {0x1034,        8,      11,     0,      0,      HI_PQ_MODULE_DEI,       4},     //dir_thd
    {0x1034,        0,      6,      0,      0,      HI_PQ_MODULE_DEI,       32},    //bc_gain
    /*VPSS_DIEJITMTN*/
    {0x1038,        12,     19,     0,      0,      HI_PQ_MODULE_DEI,       0},     //fld_motion_coring
    {0x1038,        4,      11,     0,      0,      HI_PQ_MODULE_DEI,       0},     //jitter_coring
    {0x1038,        0,      3,      0,      0,      HI_PQ_MODULE_DEI,       0},     //jitter_gain
    /*VPSS_DIEFLDMTN*/
    {0x103c,        29,     29,     0,      0,      HI_PQ_MODULE_DEI,       0},     //long_motion_shf
    {0x103c,        28,     28,     0,      0,      HI_PQ_MODULE_DEI,       1},     //fld_motion_wnd_mode
    {0x103c,        24,     27,     0,      0,      HI_PQ_MODULE_DEI,       8},     //fld_motion_gain
    {0x103c,        16,     21,     0,      0,      HI_PQ_MODULE_DEI,       -2},    //fld_motion_curve_slope
    {0x103c,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       255},   //fld_motion_thd_high
    {0x103c,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       0},     //fld_motion_thd_low
    /*VPSS_DIEMTNDIFFTHD*/
    {0x1044,        24,     30,     0,      0,      HI_PQ_MODULE_DEI,       64},    //max_motion_iir_ratio
    {0x1044,        16,     22,     0,      0,      HI_PQ_MODULE_DEI,       32},    //min_motion_iir_ratio
    {0x1044,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       255},   //motion_diff_thd_5
    {0x1044,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       255},   //motion_diff_thd_4
    {0x1040,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       255},   //motion_diff_thd_3
    {0x1040,        16,     23,     0,      0,      HI_PQ_MODULE_DEI,       208},   //motion_diff_thd_2
    {0x1040,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       144},   //motion_diff_thd_1
    {0x1040,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       16},    //motion_diff_thd_0
    /*VPSS_DIEMTNIIRSLP*/
    {0x1048,        18,     23,     0,      0,      HI_PQ_MODULE_DEI,       0},     //motion_iir_curve_slope_3
    {0x1048,        12,     17,     0,      0,      HI_PQ_MODULE_DEI,       0},     //motion_iir_curve_slope_2
    {0x1048,        6,      11,     0,      0,      HI_PQ_MODULE_DEI,       2},     //motion_iir_curve_slope_1
    {0x1048,        0,      5,      0,      0,      HI_PQ_MODULE_DEI,       1},     //motion_iir_curve_slope_0
    /*VPSS_DIEMTNIIRRAT*/
    {0x104c,        24,     30,     0,      0,      HI_PQ_MODULE_DEI,       64},    //motion_iir_curve_ratio_4
    {0x104c,        16,     22,     0,      0,      HI_PQ_MODULE_DEI,       64},    //motion_iir_curve_ratio_3
    {0x104c,        8,      14,     0,      0,      HI_PQ_MODULE_DEI,       64},    //motion_iir_curve_ratio_2
    {0x104c,        0,      6,      0,      0,      HI_PQ_MODULE_DEI,       48},    //motion_iir_curve_ratio_1
    {0x1048,        24,     30,     0,      0,      HI_PQ_MODULE_DEI,       32},    //motion_iir_curve_ratio_0
    /*VPSS_DIEHISMODE*/
    {0x1050,        21,     21,     0,      0,      HI_PQ_MODULE_DEI,       0},     //his_motion_info_write_mode
    {0x1050,        20,     20,     0,      0,      HI_PQ_MODULE_DEI,       0},     //his_motion_write_mode
    {0x1050,        19,     19,     0,      0,      HI_PQ_MODULE_DEI,       1},     //his_motion_using_mode
    {0x1050,        18,     18,     0,      0,      HI_PQ_MODULE_DEI,       1},     //his_motion_en
    {0x1050,        17,     17,     0,      0,      HI_PQ_MODULE_DEI,       0},     //pre_info_en
    {0x1050,        16,     16,     0,      0,      HI_PQ_MODULE_DEI,       1},     //ppre_info_en
    {0x1050,        12,     13,     0,      0,      HI_PQ_MODULE_DEI,       0},     //rec_mode_frm_motion_step_1
    {0x1050,        8,      9,      0,      0,      HI_PQ_MODULE_DEI,       0},     //rec_mode_frm_motion_step_0
    {0x1050,        4,      6,      0,      0,      HI_PQ_MODULE_DEI,       2},     //rec_mode_fld_motion_step_1
    {0x1050,        0,      2,      0,      0,      HI_PQ_MODULE_DEI,       2},     //rec_mode_fld_motion_step_0
    /*VPSS_DIEMORFLT*/
    {0x1054,        26,     26,     0,      0,      HI_PQ_MODULE_DEI,       0},     //med_blend_en
    {0x1054,        25,     25,     0,      0,      HI_PQ_MODULE_DEI,       0},     //reserved_1
    {0x1054,        24,     24,     0,      0,      HI_PQ_MODULE_DEI,       1},     //mor_flt_en
    {0x1054,        10,     23,     0,      0,      HI_PQ_MODULE_DEI,       0},     //reserved_2
    {0x1054,        8,      9,      0,      0,      HI_PQ_MODULE_DEI,       0},     //mor_flt_size
    {0x1054,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       0},     //mor_flt_thd
    /*VPSS_DIECOMBCHK*/
    {0x105c,        16,     16,     0,      0,      HI_PQ_MODULE_DEI,       0},     //comb_chk_en
    {0x105c,        8,      12,     0,      0,      HI_PQ_MODULE_DEI,       30},    //comb_chk_md_thd
    {0x105c,        0,      6,      0,      0,      HI_PQ_MODULE_DEI,       64},    //comb_chk_edge_thd
    {0x1058,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       160},   //comb_chk_upper_limit
    {0x1058,        16,     23,     0,      0,      HI_PQ_MODULE_DEI,       10},    //comb_chk_lower_limit
    {0x1058,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       15},    //comb_chk_min_vthd
    {0x1058,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       255},   //comb_chk_min_hthd
    /*VPSS_DIEFRMMTNSMTHTHD*/
    {0x1064,        24,     30,     0,      0,      HI_PQ_MODULE_DEI,       64},    //frame_motion_smooth_ratio_max
    {0x1064,        16,     22,     0,      0,      HI_PQ_MODULE_DEI,       0},     //frame_motion_smooth_ratio_min
    {0x1064,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       255},   //frame_motion_smooth_thd5
    {0x1064,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       255},   //frame_motion_smooth_thd4
    {0x1060,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       255},   //frame_motion_smooth_thd3
    {0x1060,        16,     23,     0,      0,      HI_PQ_MODULE_DEI,       255},   //frame_motion_smooth_thd2
    {0x1060,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       72},    //frame_motion_smooth_thd1
    {0x1060,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       8},     //frame_motion_smooth_thd0
    /*VPSS_DIEFRMMTNSMTHSLP*/
    {0x1068,        18,     23,     0,      0,      HI_PQ_MODULE_DEI,       0},     //frame_motion_smooth_slope3
    {0x1068,        12,     17,     0,      0,      HI_PQ_MODULE_DEI,       0},     //frame_motion_smooth_slope2
    {0x1068,        6,      11,     0,      0,      HI_PQ_MODULE_DEI,       0},     //frame_motion_smooth_slope1
    {0x1068,        0,      5,      0,      0,      HI_PQ_MODULE_DEI,       8},     //frame_motion_smooth_slope0
    /*DIEFRMMTNSMTHRAT*/
    {0x106c,        24,     30,     0,      0,      HI_PQ_MODULE_DEI,       64},    //frame_motion_smooth_ratio4
    {0x106c,        16,     22,     0,      0,      HI_PQ_MODULE_DEI,       64},    //frame_motion_smooth_ratio3
    {0x106c,        8,      14,     0,      0,      HI_PQ_MODULE_DEI,       64},    //frame_motion_smooth_ratio2
    {0x106c,        0,      6,      0,      0,      HI_PQ_MODULE_DEI,       64},    //frame_motion_smooth_ratio1
    {0x1068,        24,     30,     0,      0,      HI_PQ_MODULE_DEI,       0},     //frame_motion_smooth_ratio0
    /*VPSS_DIEFRMFLDBLENDTHD*/
    {0x1074,        24,     30,     0,      0,      HI_PQ_MODULE_DEI,       64},    //frame_field_blend_ratio_max
    {0x1074,        16,     22,     0,      0,      HI_PQ_MODULE_DEI,       0},     //frame_field_blend_ratio_min
    {0x1074,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       255},   //frame_field_blend_thd5
    {0x1074,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       255},   //frame_field_blend_thd4
    {0x1070,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       255},   //frame_field_blend_thd3
    {0x1070,        16,     23,     0,      0,      HI_PQ_MODULE_DEI,       255},   //frame_field_blend_thd2
    {0x1070,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       72},    //frame_field_blend_thd1
    {0x1070,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       8},     //frame_field_blend_thd0
    /*VPSS_DIEFRMFLDBLENDSLP*/
    {0x1078,        18,     23,     0,      0,      HI_PQ_MODULE_DEI,       0},     //frame_field_blend_slope3
    {0x1078,        12,     17,     0,      0,      HI_PQ_MODULE_DEI,       0},     //frame_field_blend_slope2
    {0x1078,        6,      11,     0,      0,      HI_PQ_MODULE_DEI,       0},     //frame_field_blend_slope1
    {0x1078,        0,      5,      0,      0,      HI_PQ_MODULE_DEI,       8},     //frame_field_blend_slope0
    /*DIEFRMFLDBLENDRAT*/
    {0x107c,        24,     30,     0,      0,      HI_PQ_MODULE_DEI,       64},    //frame_field_blend_ratio4
    {0x107c,        16,     22,     0,      0,      HI_PQ_MODULE_DEI,       64},    //frame_field_blend_ratio3
    {0x107c,        8,      14,     0,      0,      HI_PQ_MODULE_DEI,       64},    //frame_field_blend_ratio2
    {0x107c,        0,      6,      0,      0,      HI_PQ_MODULE_DEI,       64},    //frame_field_blend_ratio1
    {0x1078,        24,     30,     0,      0,      HI_PQ_MODULE_DEI,       0},     //frame_field_blend_ratio0
    /*VPSS_DIEMTNADJ*/
    {0x1080,        16,     23,     0,      0,      HI_PQ_MODULE_DEI,       128},   //motion_adjust_gain_chr
    {0x1080,        8,      13,     0,      0,      HI_PQ_MODULE_DEI,       0},     //motion_adjust_coring
    {0x1080,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       128},   //motion_adjust_gain
    /*VPSS_DIEEDGENORM*/
    {0x1098,        12,     23,     0,      0,      HI_PQ_MODULE_DEI,       134},   //edge_norm_11
    {0x1098,        0,      11,     0,      0,      HI_PQ_MODULE_DEI,       135},   //edge_norm_10
    {0x1094,        12,     23,     0,      0,      HI_PQ_MODULE_DEI,       143},   //edge_norm_9
    {0x1094,        0,      11,     0,      0,      HI_PQ_MODULE_DEI,       142},   //edge_norm_8
    {0x1090,        12,     23,     0,      0,      HI_PQ_MODULE_DEI,       132},   //edge_norm_7
    {0x1090,        0,      11,     0,      0,      HI_PQ_MODULE_DEI,       140},   //edge_norm_6
    {0x108c,        12,     23,     0,      0,      HI_PQ_MODULE_DEI,       132},   //edge_norm_5
    {0x108c,        0,      11,     0,      0,      HI_PQ_MODULE_DEI,       134},   //edge_norm_4
    {0x1088,        12,     23,     0,      0,      HI_PQ_MODULE_DEI,       115},   //edge_norm_3
    {0x1088,        0,      11,     0,      0,      HI_PQ_MODULE_DEI,       136},   //edge_norm_2
    {0x1084,        12,     23,     0,      0,      HI_PQ_MODULE_DEI,       227},   //edge_norm_1
    {0x1084,        0,      11,     0,      0,      HI_PQ_MODULE_DEI,       0},     //edge_norm_0
    {0x1094,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       32},    //inter_diff_thd0
    {0x1098,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       64},    //inter_diff_thd1
    {0x109c,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       255},   //inter_diff_thd2
    {0x109c,        12,     23,     0,      0,      HI_PQ_MODULE_DEI,       32},    //edge_scale
    {0x109c,        0,      11,     0,      0,      HI_PQ_MODULE_DEI,       16},    //edge_coring
    /*VPSS_DIEMCSTRENGTH*/
    {0x10a4,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       16},    //mc_strength_maxg
    {0x10a4,        16,     23,     0,      0,      HI_PQ_MODULE_DEI,       16},    //mc_strength_ming
    {0x1090,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       64},    //mc_strength_g3
    {0x10a4,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       64},    //mc_strength_g2
    {0x10a4,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       16},    //mc_strength_g1
    {0x10a0,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       16},    //mc_strength_g0
    {0x108c,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       0},     //mc_strength_k3
    {0x10a0,        16,     23,     0,      0,      HI_PQ_MODULE_DEI,       0},     //mc_strength_k2
    {0x10a0,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       6},     //mc_strength_k1
    {0x10a0,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       0},     //mc_strength_k0
    /*VPSS_DIEFUSION*/
    {0x10a8,        24,     30,     0,      0,      HI_PQ_MODULE_DEI,       64},    //k_c_mcbld
    {0x10a8,        16,     22,     0,      0,      HI_PQ_MODULE_DEI,       8},     //k_c_mcw
    {0x10a8,        8,      14,     0,      0,      HI_PQ_MODULE_DEI,       64},    //k_y_mcbld
    {0x10a8,        0,      6,      0,      0,      HI_PQ_MODULE_DEI,       16},    //k_y_mcw
    {0x10ac,        16,     27,     0,      0,      HI_PQ_MODULE_DEI,       1023},  //g0_mcw_adj
    {0x10ac,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       64},    //k0_mcw_adj
    {0x10ac,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       64},    //x0_mcw_adj
    {0x10b0,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       128},   //k1_mcw_adj
    {0x10b0,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       0},     //k1_mcbld
    {0x10b0,        16,     23,     0,      0,      HI_PQ_MODULE_DEI,       0},     //k0_mcbld
    {0x10b0,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       0},     //x0_mcbld
    {0x10b4,        0,      11,     0,      0,      HI_PQ_MODULE_DEI,       0},     //g0_mcbld
    {0x10b4,        20,     20,     0,      0,      HI_PQ_MODULE_DEI,       1},     //mc_lai_bldmode
    {0x10b4,        12,     16,     0,      0,      HI_PQ_MODULE_DEI,       0},     //k_curw_mcbld
    /*VPSS_DIEMAGBMTHD*/
    {0x10b8,        16,     25,     0,      0,      HI_PQ_MODULE_DEI,       16},    //ma_gbm_thd0
    {0x10b8,        0,      9,      0,      0,      HI_PQ_MODULE_DEI,       48},    //ma_gbm_thd1
    {0x10bc,        16,     25,     0,      0,      HI_PQ_MODULE_DEI,       80},    //ma_gbm_thd2
    {0x10bc,        0,      9,      0,      0,      HI_PQ_MODULE_DEI,       112},   //ma_gbm_thd3
    /*VPSS_DIEMCGBMCOEF*/
    {0x10c0,        28,     28,     0,      0,      HI_PQ_MODULE_DEI,       1},     //mtfilten_gmd
    {0x10c0,        20,     25,     0,      0,      HI_PQ_MODULE_DEI,       28},    //mtth3_gmd
    {0x10c0,        12,     17,     0,      0,      HI_PQ_MODULE_DEI,       20},    //mtth2_gmd
    {0x10c0,        4,      8,      0,      0,      HI_PQ_MODULE_DEI,       12},    //mtth1_gmd
    {0x10c0,        0,      3,      0,      0,      HI_PQ_MODULE_DEI,       4},     //mtth0_gmd
    {0x10c4,        12,     19,     0,      0,      HI_PQ_MODULE_DEI,       96},    //k_mag_gmd
    {0x10c4,        4,      10,     0,      0,      HI_PQ_MODULE_DEI,       22},    //k_difh_gmd
    {0x10c4,        0,      3,      0,      0,      HI_PQ_MODULE_DEI,       4},     //k_maxmag_gmd
    /*VPSS_MCDI_RGDIFY*/
    {0x10fc,        24,     27,     0,      0,      HI_PQ_MODULE_DEI,       3},     //k_rgdifycore
    {0x10fc,        14,     23,     0,      0,      HI_PQ_MODULE_DEI,       1023},  //g_rgdifycore
    {0x10fc,        10,     13,     0,      0,      HI_PQ_MODULE_DEI,       7},     //core_rgdify
    {0x10fc,        0,      9,      0,      0,      HI_PQ_MODULE_DEI,       511},   //lmt_rgdify
    /*VPSS_MCDI_RGSAD*/
    {0x1100,        23,     25,     0,      0,      HI_PQ_MODULE_DEI,       1},     //coef_sadlpf
    {0x1100,        15,     21,     0,      0,      HI_PQ_MODULE_DEI,       32},    //kmv_rgsad
    {0x1100,        9,      14,     0,      0,      HI_PQ_MODULE_DEI,       63},    //k_tpdif_rgsad
    {0x1100,        0,      8,      0,      0,      HI_PQ_MODULE_DEI,       255},   //g_tpdif_rgsad
    /*VPSS_MCDI_RGMV*/
    {0x1104,        19,     26,     0,      0,      HI_PQ_MODULE_DEI,       48},    //thmag_rgmv
    {0x1104,        10,     18,     0,      0,      HI_PQ_MODULE_DEI,       128},   //th_saddif_rgmv
    {0x1104,        0,      9,      0,      0,      HI_PQ_MODULE_DEI,       256},   //th_0mvsad_rgmv
    /*VPSS_MCDI_RGMAG0*/
    {0x1108,        10,     13,     0,      0,      HI_PQ_MODULE_DEI,       3},     //core_mag_rg
    {0x1108,        0,      9,      0,      0,      HI_PQ_MODULE_DEI,       255},   //lmt_mag_rg
    /*VPSS_MCDI_RGMAG1*/
    {0x110c,        21,     25,     0,      0,      HI_PQ_MODULE_DEI,       2},     //core_mv_rgmvls
    {0x110c,        16,     20,     0,      0,      HI_PQ_MODULE_DEI,       16},    //k_mv_rgmvls
    {0x110c,        9,      15,     0,      0,      HI_PQ_MODULE_DEI,       -64},   //core_mag_rgmvls
    {0x110c,        5,      8,      0,      0,      HI_PQ_MODULE_DEI,       12},    //k_mag_rgmvls
    {0x110c,        1,      4,      0,      0,      HI_PQ_MODULE_DEI,       8},     //th_mvadj_rgmvls
    {0x110c,        0,      0,      0,      0,      HI_PQ_MODULE_DEI,       1},     //en_mvadj_rgmvls
    /*VPSS_MCDI_RGLS*/
    {0x1110,        15,     18,     0,      0,      HI_PQ_MODULE_DEI,       8},     //k_sad_rgls
    {0x1110,        9,      14,     0,      0,      HI_PQ_MODULE_DEI,       40},    //th_mag_rgls
    {0x1110,        4,      8,      0,      0,      HI_PQ_MODULE_DEI,       8},     //th_sad_rgls
    {0x1110,        0,      3,      0,      0,      HI_PQ_MODULE_DEI,       8},     //k_sadcore_rgmv
    /*VPSS_MCDI_SEL*/
    {0x1114,        8,      8,      0,      0,      HI_PQ_MODULE_DEI,       0},     //force_mven
    {0x1114,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       0},     //force_mvx
    /*VPSS_MCDI_DLT0*/
    {0x1118,        16,     18,     0,      0,      HI_PQ_MODULE_DEI,       6},     //th_blkmvx_mvdlt
    {0x1118,        12,     15,     0,      0,      HI_PQ_MODULE_DEI,       4},     //th_rgmvx_mvdlt
    {0x1118,        8,      11,     0,      0,      HI_PQ_MODULE_DEI,       8},     //th_ls_mvdlt
    {0x1118,        4,      7,      0,      0,      HI_PQ_MODULE_DEI,       1},     //th_vblkdist_mvdlt
    {0x1118,        0,      3,      0,      0,      HI_PQ_MODULE_DEI,       4},     //th_hblkdist_mvdlt
    /*VPSS_MCDI_DLT1*/
    {0x111c,        23,     25,     0,      0,      HI_PQ_MODULE_DEI,       2},     //k_sadcore_mvdlt
    {0x111c,        18,     22,     0,      0,      HI_PQ_MODULE_DEI,       12},    //th_mag_mvdlt
    {0x111c,        12,     17,     0,      0,      HI_PQ_MODULE_DEI,       16},    //g_mag_mvdlt
    {0x111c,        5,      11,     0,      0,      HI_PQ_MODULE_DEI,       96},    //thl_sad_mvdlt
    {0x111c,        0,      4,      0,      0,      HI_PQ_MODULE_DEI,       16},    //thh_sad_mvdlt
    /*VPSS_MCDI_RGMCW0*/
    {0x1120,        20,     24,     0,      0,      HI_PQ_MODULE_DEI,       20},    //k_rglsw
    {0x1120,        14,     19,     0,      0,      HI_PQ_MODULE_DEI,       32},    //k_simimvw
    {0x1120,        8,      13,     0,      0,      HI_PQ_MODULE_DEI,       15},    //gh_core_simimv
    {0x1120,        5,      7,      0,      0,      HI_PQ_MODULE_DEI,       0},     //gl_core_simimv
    {0x1120,        0,      4,      0,      0,      HI_PQ_MODULE_DEI,       8},     //k_core_simimv
    /*VPSS_MCDI_RGMCW1*/
    {0x1124,        23,     27,     0,      0,      HI_PQ_MODULE_DEI,       16},    //k_core_vsaddif
    {0x1124,        19,     22,     0,      0,      HI_PQ_MODULE_DEI,       8},     //k_rgsadadj_mcw
    {0x1124,        10,     18,     0,      0,      HI_PQ_MODULE_DEI,       64},    //core_rgsadadj_mcw
    {0x1124,        4,      9,      0,      0,      HI_PQ_MODULE_DEI,       16},    //k_mvy_mcw
    {0x1124,        1,      3,      0,      0,      HI_PQ_MODULE_DEI,       3},     //core_mvy_mcw
    {0x1124,        0,      0,      0,      0,      HI_PQ_MODULE_DEI,       1},     //rgtb_en_mcw
    /*VPSS_MCDI_RGMCW2*/
    {0x1128,        19,     26,     0,      0,      HI_PQ_MODULE_DEI,       24},    //core_rgmag_mcw
    {0x1128,        18,     18,     0,      0,      HI_PQ_MODULE_DEI,       0},     //mode_rgysad_mcw
    {0x1128,        12,     17,     0,      0,      HI_PQ_MODULE_DEI,       8},     //k_vsaddifw
    {0x1128,        5,      11,     0,      0,      HI_PQ_MODULE_DEI,       64},    //gh_core_vsad_dif
    {0x1128,        0,      4,      0,      0,      HI_PQ_MODULE_DEI,       8},     //gl_core_vsaddif
    /*VPSS_MCDI_RGMCW3*/
    {0x112c,        18,     25,     0,      0,      HI_PQ_MODULE_DEI,       64},    //g0_rgmag_mcw
    {0x112c,        9,      17,     0,      0,      HI_PQ_MODULE_DEI,       256},   //k0_rgmag_mcw
    {0x112c,        0,      8,      0,      0,      HI_PQ_MODULE_DEI,       64},    //x0_rgmag_mcw
    /*VPSS_MCDI_RGMCW4*/
    {0x1130,        17,     26,     0,      0,      HI_PQ_MODULE_DEI,       512},   //x0_rgsad_mcw
    {0x1130,        9,      16,     0,      0,      HI_PQ_MODULE_DEI,       96},    //core_rgsad_mcw
    {0x1130,        0,      8,      0,      0,      HI_PQ_MODULE_DEI,       320},   //k1_rgmag_mcw
    /*VPSS_MCDI_RGMCW5*/
    {0x1134,        17,     25,     0,      0,      HI_PQ_MODULE_DEI,       128},   //k1_rgsad_mcw
    {0x1134,        9,      16,     0,      0,      HI_PQ_MODULE_DEI,       255},   //g0_rgsad_mcw
    {0x1134,        0,      8,      0,      0,      HI_PQ_MODULE_DEI,       160},   //k0_rgsad_mcw
    /*VPSS_MCDI_RGMCW6*/
    {0x1138,        24,     29,     0,      0,      HI_PQ_MODULE_DEI,       24},    //k_rgsad_mcw
    {0x1138,        16,     23,     0,      0,      HI_PQ_MODULE_DEI,       122},   //x_rgsad_mcw
    {0x1138,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       64},    //k0_smrg_mcw
    {0x1138,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       16},    //x0_smrg_mcw
    /*VPSS_MCDI_TPMCW0*/
    {0x113c,        23,     29,     0,      0,      HI_PQ_MODULE_DEI,       32},    //k1_tpmvdist_mcw
    {0x113c,        15,     22,     0,      0,      HI_PQ_MODULE_DEI,       0},     //g0_tpmvdist_mcw
    {0x113c,        8,      14,     0,      0,      HI_PQ_MODULE_DEI,       64},    //k0_tpmvdist_mcw
    {0x113c,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       255},   //x0_tpmvdist_mcw
    /*VPSS_MCDI_TPMCW1*/
    {0x1140,        11,     13,     0,      0,      HI_PQ_MODULE_DEI,       2},     //k_core_tpmvdist_mcw
    {0x1140,        8,      10,     0,      0,      HI_PQ_MODULE_DEI,       2},     //b_core_tpmvdist_mcw
    {0x1140,        4,      7,      0,      0,      HI_PQ_MODULE_DEI,       4},     //k_avgmvdist_mcw
    {0x1140,        0,      3,      0,      0,      HI_PQ_MODULE_DEI,       4},     //k_minmvdist_mcw
    /*VPSS_MCDI_WNDMCW0*/
    {0x1144,        27,     30,     0,      0,      HI_PQ_MODULE_DEI,       15},    //k_tbdif_mcw
    {0x1144,        23,     26,     0,      0,      HI_PQ_MODULE_DEI,       8},     //k0_max_mag_mcw
    {0x1144,        19,     22,     0,      0,      HI_PQ_MODULE_DEI,       8},     //k1_max_mag_mcw
    {0x1144,        15,     18,     0,      0,      HI_PQ_MODULE_DEI,       8},     //k_max_dif_mcw
    {0x1144,        11,     14,     0,      0,      HI_PQ_MODULE_DEI,       8},     //k_max_core_mcw
    {0x1144,        5,      10,     0,      0,      HI_PQ_MODULE_DEI,       32},    //k_difvcore_mcw
    {0x1144,        0,      4,      0,      0,      HI_PQ_MODULE_DEI,       18},    //k_difhcore_mcw
    /*VPSS_MCDI_WNDMCW1*/
    {0x1148,        21,     24,     0,      0,      HI_PQ_MODULE_DEI,       6},     //k1_mag_wnd_mcw
    {0x1148,        15,     20,     0,      0,      HI_PQ_MODULE_DEI,       24},    //g0_mag_wnd_mcw
    {0x1148,        11,     14,     0,      0,      HI_PQ_MODULE_DEI,       6},     //k0_mag_wnd_mcw
    {0x1148,        4,      10,     0,      0,      HI_PQ_MODULE_DEI,       32},    //x0_mag_wnd_mcw
    {0x1148,        0,      3,      0,      0,      HI_PQ_MODULE_DEI,       0},     //k_tbmag_mcw
    /*VPSS_MCDI_WNDMCW2*/
    {0x114c,        21,     28,     0,      0,      HI_PQ_MODULE_DEI,       16},    //g0_sad_wnd_mcw
    {0x114c,        16,     20,     0,      0,      HI_PQ_MODULE_DEI,       16},    //k0_sad_wnd_mcw
    {0x114c,        9,      15,     0,      0,      HI_PQ_MODULE_DEI,       8},     //x0_sad_wnd_mcw
    {0x114c,        0,      8,      0,      0,      HI_PQ_MODULE_DEI,       288},   //g1_mag_wnd_mcw
    /*VPSS_MCDI_WNDMCW3*/
    {0x1150,        5,      13,     0,      0,      HI_PQ_MODULE_DEI,       288},   //g1_sad_wnd_mcw
    {0x1150,        0,      4,      0,      0,      HI_PQ_MODULE_DEI,       16},    //k1_sad_wnd_mcw
    /*VPSS_MCDI_VERTWEIGHT0*/
    {0x1154,        24,     26,     0,      0,      HI_PQ_MODULE_DEI,       0},     //b_hvdif_dw
    {0x1154,        20,     22,     0,      0,      HI_PQ_MODULE_DEI,       0},     //b_bhvdif_dw
    {0x1154,        12,     18,     0,      0,      HI_PQ_MODULE_DEI,       64},    //k_bhvdif_dw
    {0x1154,        8,      11,     0,      0,      HI_PQ_MODULE_DEI,       5},     //core_bhvdif_dw
    {0x1154,        4,      7,      0,      0,      HI_PQ_MODULE_DEI,       15},    //gain_lpf_dw
    {0x1154,        0,      3,      0,      0,      HI_PQ_MODULE_DEI,       12},    //k_max_hvdif_dw
    /*VPSS_MCDI_VERTWEIGHT1*/
    {0x1158,        20,     25,     0,      0,      HI_PQ_MODULE_DEI,       56},    //b_mv_dw
    {0x1158,        16,     19,     0,      0,      HI_PQ_MODULE_DEI,       -2},    //core_mv_dw
    {0x1158,        8,      12,     0,      0,      HI_PQ_MODULE_DEI,       20},    //k_difv_dw
    {0x1158,        0,      4,      0,      0,      HI_PQ_MODULE_DEI,       16},    //core_hvdif_dw
    /*VPSS_MCDI_VERTWEIGHT2*/
    {0x115c,        25,     30,     0,      0,      HI_PQ_MODULE_DEI,       8},     //k1_hvdif_dw
    {0x115c,        16,     24,     0,      0,      HI_PQ_MODULE_DEI,       128},   //g0_hvdif_dw
    {0x115c,        9,      14,     0,      0,      HI_PQ_MODULE_DEI,       8},     //k0_hvdif_dw
    {0x115c,        0,      8,      0,      0,      HI_PQ_MODULE_DEI,       256},   //x0_hvdif_dw
    /*VPSS_MCDI_VERTWEIGHT3*/
    {0x1160,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       64},    //k1_mv_dw
    {0x1160,        16,     21,     0,      0,      HI_PQ_MODULE_DEI,       32},    //g0_mv_dw
    {0x1160,        8,      13,     0,      0,      HI_PQ_MODULE_DEI,       16},    //k0_mv_dw
    {0x1160,        0,      4,      0,      0,      HI_PQ_MODULE_DEI,       8},     //x0_mv_dw
    /*VPSS_MCDI_VERTWEIGHT4*/
    {0x1164,        24,     29,     0,      0,      HI_PQ_MODULE_DEI,       32},    //k1_mt_dw
    {0x1164,        16,     23,     0,      0,      HI_PQ_MODULE_DEI,       64},    //g0_mt_dw
    {0x1164,        8,      13,     0,      0,      HI_PQ_MODULE_DEI,       32},    //k0_mt_dw
    {0x1164,        0,      6,      0,      0,      HI_PQ_MODULE_DEI,       32},    //x0_mt_dw
    /*VPSS_MCDI_VERTWEIGHT5*/
    {0x1168,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       0},     //b_mt_dw
    {0x1168,        12,     16,     0,      0,      HI_PQ_MODULE_DEI,       20},    //k1_mv_mt
    {0x1168,        8,      9,      0,      0,      HI_PQ_MODULE_DEI,       1},     //x0_mv_mt
    {0x1168,        0,      4,      0,      0,      HI_PQ_MODULE_DEI,       31},    //g0_mv_mt
    /*VPSS_MCDI_MC0*/
    {0x116c,        28,     28,     0,      0,      HI_PQ_MODULE_DEI,       1},     //mclpf_mode
    {0x116c,        20,     26,     0,      0,      HI_PQ_MODULE_DEI,       8},     //k_pxlmag_mcw
    {0x116c,        16,     18,     0,      0,      HI_PQ_MODULE_DEI,       2},     //x_pxlmag_mcw
    {0x116c,        15,     15,     0,      0,      HI_PQ_MODULE_DEI,       0},     //rs_pxlmag_mcw
    {0x116c,        10,     14,     0,      0,      HI_PQ_MODULE_DEI,       16},    //gain_mclpfh
    {0x116c,        5,      9,      0,      0,      HI_PQ_MODULE_DEI,       16},    //gain_dn_mclpfv
    {0x116c,        0,      4,      0,      0,      HI_PQ_MODULE_DEI,       16},    //gain_up_mclpfv
    /*VPSS_MCDI_MC1*/
    {0x1170,        0,      5,      0,      0,      HI_PQ_MODULE_DEI,       0},     //g_pxlmag_mcw
    /*VPSS_MCDI_MC2*/
    {0x1174,        5,      11,     0,      0,      HI_PQ_MODULE_DEI,       15},    //k_c_vertw
    {0x1174,        0,      4,      0,      0,      HI_PQ_MODULE_DEI,       8},     //k_y_vertw
    /*VPSS_MCDI_MC3*/
    {0x1178,        25,     29,     0,      0,      HI_PQ_MODULE_DEI,       0},     //k_fstmt_mc
    {0x1178,        20,     24,     0,      0,      HI_PQ_MODULE_DEI,       0},     //x_fstmt_mc
    {0x1178,        14,     19,     0,      0,      HI_PQ_MODULE_DEI,       16},    //k1_mv_mc
    {0x1178,        11,     13,     0,      0,      HI_PQ_MODULE_DEI,       2},     //x0_mv_mc
    {0x1178,        8,      10,     0,      0,      HI_PQ_MODULE_DEI,       4},     //bdv_mcpos
    {0x1178,        5,      7,      0,      0,      HI_PQ_MODULE_DEI,       4},     //bdh_mcpos
    {0x1178,        0,      4,      0,      0,      HI_PQ_MODULE_DEI,       8},     //k_delta
    /*VPSS_MCDI_MC4*/
    {0x117c,        26,     30,     0,      0,      HI_PQ_MODULE_DEI,       48},    //k_hfcore_mc
    {0x117c,        21,     25,     0,      0,      HI_PQ_MODULE_DEI,       8},     //x_hfcore_mc
    {0x117c,        15,     20,     0,      0,      HI_PQ_MODULE_DEI,       0},     //g_slmt_mc
    {0x117c,        10,     14,     0,      0,      HI_PQ_MODULE_DEI,       0},     //k_slmt_mc
    {0x117c,        5,      9,      0,      0,      HI_PQ_MODULE_DEI,       0},     //x_slmt_mc
    {0x117c,        0,      4,      0,      0,      HI_PQ_MODULE_DEI,       0},     //g_fstmt_mc
    /*VPSS_MCDI_MC5*/
    {0x1180,        18,     29,     0,      0,      HI_PQ_MODULE_DEI,       0},     //r0_mc
    {0x1180,        6,      17,     0,      0,      HI_PQ_MODULE_DEI,       0},     //c0_mc
    {0x1180,        0,      5,      0,      0,      HI_PQ_MODULE_DEI,       0},     //g_hfcore_mc
    /*VPSS_MCDI_MC6*/
    {0x1184,        24,     29,     0,      0,      HI_PQ_MODULE_DEI,       32},    //mcmvrange
    {0x1184,        12,     23,     0,      0,      HI_PQ_MODULE_DEI,       4095},  //r1_mc
    {0x1184,        0,      11,     0,      0,      HI_PQ_MODULE_DEI,       4095},  //c1_mc
    /*VPSS_MCDI_MC7*/
    {0x1188,        6,      12,     0,      0,      HI_PQ_MODULE_DEI,       48},    //k_frcount_mc
    {0x1188,        1,      5,      0,      0,      HI_PQ_MODULE_DEI,       8},     //x_frcount_mc
    {0x1188,        0,      0,      0,      0,      HI_PQ_MODULE_DEI,       0},     //scenechange_mc
    /*VPSS_MCDI_LCDINEW0*/
    {0x118c,        24,     31,     0,      0,      HI_PQ_MODULE_DEI,       0},     //mcendc
    {0x118c,        16,     23,     0,      0,      HI_PQ_MODULE_DEI,       0},     //mcendr
    {0x118c,        8,      15,     0,      0,      HI_PQ_MODULE_DEI,       0},     //mcstartc
    {0x118c,        0,      7,      0,      0,      HI_PQ_MODULE_DEI,       0},     //mcstartr
    /*VPSS_MCDI_LCDINEW1*/
    {0x1190,        18,     23,     0,      0,      HI_PQ_MODULE_DEI,       2},     //movegain
    {0x1190,        12,     17,     0,      0,      HI_PQ_MODULE_DEI,       4},     //movecorig
    {0x1190,        6,      11,     0,      0,      HI_PQ_MODULE_DEI,       2},     //movethdl
    {0x1190,        0,      5,      0,      0,      HI_PQ_MODULE_DEI,       12},    //movethdh
    /*VPSS_MCDI_NUMT*/
    {0x1194,        15,     15,     0,      0,      HI_PQ_MODULE_DEI,       0},     //mc_numt_blden
    {0x1194,        7,      14,     0,      0,      HI_PQ_MODULE_DEI,       32},    //numt_gain
    {0x1194,        1,      6,      0,      0,      HI_PQ_MODULE_DEI,       0},     //numt_coring
    {0x1194,        0,      0,      0,      0,      HI_PQ_MODULE_DEI,       0},     //numt_lpf_en
    /*VPSS_MCDI_DEMO*/
    {0x1198,        5,      16,     0,      0,      HI_PQ_MODULE_DEI,       0},     //demo_border
    {0x1198,        3,      4,      0,      0,      HI_PQ_MODULE_DEI,       2},     //demo_mode_r
    {0x1198,        1,      2,      0,      0,      HI_PQ_MODULE_DEI,       2},     //demo_mode_l
    {0x1198,        0,      0,      0,      0,      HI_PQ_MODULE_DEI,       0},     //demo_en

    /***********************************************FMD***********************************************/
    /*FMD_CTRL*/
    {0x1000,        20,     20,     0,      0,      HI_PQ_MODULE_FMD,       0},     //edge_smooth_en
    {0x1000,        8,      15,     0,      0,      HI_PQ_MODULE_FMD,       32},    //edge_smooth_ratio
    {0x10d0,        0,      2,      0,      0,      HI_PQ_MODULE_FMD,       6},     //bitsmov2r
    /*FMD_CTRL_ReadOnly*/
    {0x1000,        19,     19,     0,      0,      HI_PQ_MODULE_FMD,       0},     //mchdir_l
    {0x1000,        18,     18,     0,      0,      HI_PQ_MODULE_FMD,       0},     //mchdir_c
    /*VPSS_PDPCCMOV*/
    {0x10cc,        16,     23,     0,      0,      HI_PQ_MODULE_FMD,       20},    //mov_coring_norm
    {0x10cc,        8,      15,     0,      0,      HI_PQ_MODULE_FMD,       50},    //mov_coring_tkr
    {0x10cc,        0,      7,      0,      0,      HI_PQ_MODULE_FMD,       20},    //mov_coring_blk
    /*VPSS_PDPHISTTHD*/
    {0x10c8,        24,     31,     0,      0,      HI_PQ_MODULE_FMD,       32},    //hist_thd3
    {0x10c8,        16,     23,     0,      0,      HI_PQ_MODULE_FMD,       32},    //hist_thd2
    {0x10c8,        8,      15,     0,      0,      HI_PQ_MODULE_FMD,       16},    //hist_thd1
    {0x10c8,        0,      7,      0,      0,      HI_PQ_MODULE_FMD,       8},     //hist_thd0
    /*VPSS_PDCTRL*/
    {0x10d0,        19,     19,     0,      0,      HI_PQ_MODULE_FMD,       0},     //lasi_mode
    {0x10d4,        20,     27,     0,      0,      HI_PQ_MODULE_FMD,       30},    //diff_movblk_thd
    /*VPSS_PDUMTHD*/
    {0x10d8,        16,     23,     0,      0,      HI_PQ_MODULE_FMD,       64},    //um_thd2
    {0x10d8,        8,      15,     0,      0,      HI_PQ_MODULE_FMD,       32},    //um_thd1
    {0x10d8,        0,      7,      0,      0,      HI_PQ_MODULE_FMD,       8},     //um_thd0
    /*VPSS_PDPCCCORING*/
    {0x10dc,        16,     23,     0,      0,      HI_PQ_MODULE_FMD,       16},    //coring_blk
    {0x10dc,        8,      15,     0,      0,      HI_PQ_MODULE_FMD,       16},    //coring_norm
    {0x10dc,        0,      7,      0,      0,      HI_PQ_MODULE_FMD,       90},    //coring_tkr
    /*VPSS_PDPCCHVTHD*/
    {0x10e0,        0,      7,      0,      0,      HI_PQ_MODULE_FMD,       16},    //pcc_hthd
    {0x10e4,        24,     31,     0,      0,      HI_PQ_MODULE_FMD,       128},   //pcc_vthd3
    {0x10e4,        16,     23,     0,      0,      HI_PQ_MODULE_FMD,       64},    //pcc_vthd2
    {0x10e4,        8,      15,     0,      0,      HI_PQ_MODULE_FMD,       32},    //pcc_vthd1
    {0x10e4,        0,      7,      0,      0,      HI_PQ_MODULE_FMD,       16},    //pcc_vthd0
    /*VPSS_PDITDIFFVTHD*/
    {0x10e8,        24,     31,     0,      0,      HI_PQ_MODULE_FMD,       32},    //itdiff_vthd3
    {0x10e8,        16,     23,     0,      0,      HI_PQ_MODULE_FMD,       16},    //itdiff_vthd2
    {0x10e8,        8,      15,     0,      0,      HI_PQ_MODULE_FMD,       8},     //itdiff_vthd1
    {0x10e8,        0,      7,      0,      0,      HI_PQ_MODULE_FMD,       4},     //itdiff_vthd0
    /*VPSS_PDLASITHD*/
    {0x10ec,        16,     23,     0,      0,      HI_PQ_MODULE_FMD,       32},    //lasi_mov_thd
    {0x10ec,        8,      15,     0,      0,      HI_PQ_MODULE_FMD,       15},    //lasi_edge_thd
    {0x10ec,        0,      7,      0,      0,      HI_PQ_MODULE_FMD,       20},    //lasi_coring_thd
    /*PDLASIMODE0COEF*/
    {0x10f0,        8,      15,     0,      0,      HI_PQ_MODULE_FMD,       0},     //lasi_txt_coring
    {0x10f0,        0,      7,      0,      0,      HI_PQ_MODULE_FMD,      32},     //lasi_txt_alpha
    {0x10f4,        24,     31,     0,      0,      HI_PQ_MODULE_FMD,       0},     //lasi_txt_thd3
    {0x10f4,        16,     23,     0,      0,      HI_PQ_MODULE_FMD,       0},     //lasi_txt_thd2
    {0x10f4,        8,      15,     0,      0,      HI_PQ_MODULE_FMD,       0},     //lasi_txt_thd1
    {0x10f4,        0,      7,      0,      0,      HI_PQ_MODULE_FMD,       0},     //lasi_txt_thd0
    /*PDLASIMODE0COEF*/
    {0x10f8,        12,     23,     0,      0,      HI_PQ_MODULE_FMD,       0},     //region1_y_stt
    {0x10f8,        0,      11,     0,      0,      HI_PQ_MODULE_FMD,       0},     //region1_y_end

    /***********************************************DB***********************************************/
    /*DB_CTRL*/
    {0x0000,        11,     11,     0,      0,      HI_PQ_MODULE_DB,        1},     //dbm_en
    {0x2500,        0,      0,      0,      0,      HI_PQ_MODULE_DB,        1},     //db_en
    {0x2500,        9,      9,      0,      0,      HI_PQ_MODULE_DB,        0},     //dbm_demo_en
    {0x2500,        10,     10,     0,      0,      HI_PQ_MODULE_DB,        1},     //dbm_demo_mode
    {0x2500,        15,     15,     0,      0,      HI_PQ_MODULE_DB,        1},     //det_vy_en
    {0x2500,        14,     14,     0,      0,      HI_PQ_MODULE_DB,        1},     //det_hc_en
    {0x2500,        13,     13,     0,      0,      HI_PQ_MODULE_DB,        1},     //det_hy_en
    {0x2500,        5,      5,      0,      0,      HI_PQ_MODULE_DB,        1},     //db_luma_hor_en
    {0x2500,        4,      4,      0,      0,      HI_PQ_MODULE_DB,        1},     //db_luma_ver_en
    {0x2500,        19,     24,     0,      0,      HI_PQ_MODULE_DB,        4},     //dbm_out_sel_gain
    {0x2500,        16,     18,     0,      0,      HI_PQ_MODULE_DB,        5},     //dbm_out_mode
    {0x2500,        25,     27,     0,      0,      HI_PQ_MODULE_DB,        1},     //dbm_protocol
    /*VPSS_DB_LUT34*/
    {0x258c,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        8},     //db_lum_h_str_fade_lut1_p1
    {0x258c,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        16},    //db_lum_h_str_fade_lut1_p0
    /*VPSS_DB_LUT35*/
    {0x2590,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        12},    //db_lum_h_str_fade_lut2_p3
    {0x2590,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        12},    //db_lum_h_str_fade_lut2_p2
    {0x2590,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        16},    //db_lum_h_str_fade_lut2_p1
    {0x2590,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        16},    //db_lum_h_str_fade_lut2_p0
    /*VPSS_DB_LUT36~37*/
    {0x2598,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        2},     //db_lum_h_str_fade_lut3_p7
    {0x2598,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        4},     //db_lum_h_str_fade_lut3_p6
    {0x2594,        25,     29,     0,      0,      HI_PQ_MODULE_DB,        6},     //db_lum_h_str_fade_lut3_p5
    {0x2594,        20,     24,     0,      0,      HI_PQ_MODULE_DB,        8},     //db_lum_h_str_fade_lut3_p4
    {0x2594,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        10},    //db_lum_h_str_fade_lut3_p3
    {0x2594,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        12},    //db_lum_h_str_fade_lut3_p2
    {0x2594,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        14},    //db_lum_h_str_fade_lut3_p1
    {0x2594,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        16},    //db_lum_h_str_fade_lut3_p0
    /*VPSS_DB_LUT38~39*/
    {0x25a0,        25,     29,     0,      0,      HI_PQ_MODULE_DB,        1},     //db_lum_h_str_fade_lut4_p11
    {0x25a0,        20,     24,     0,      0,      HI_PQ_MODULE_DB,        3},     //db_lum_h_str_fade_lut4_p10
    {0x25a0,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        5},     //db_lum_h_str_fade_lut4_p9
    {0x25a0,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        7},     //db_lum_h_str_fade_lut4_p8
    {0x25a0,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        9},     //db_lum_h_str_fade_lut4_p7
    {0x25a0,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        10},    //db_lum_h_str_fade_lut4_p6
    {0x259c,        25,     29,     0,      0,      HI_PQ_MODULE_DB,        11},    //db_lum_h_str_fade_lut4_p5
    {0x259c,        20,     24,     0,      0,      HI_PQ_MODULE_DB,        12},    //db_lum_h_str_fade_lut4_p4
    {0x259c,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        13},    //db_lum_h_str_fade_lut4_p3
    {0x259c,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        14},    //db_lum_h_str_fade_lut4_p2
    {0x259c,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        15},    //db_lum_h_str_fade_lut4_p1
    {0x259c,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        16},    //db_lum_h_str_fade_lut4_p0
    /*VPSS_DB_LUT40*/
    {0x25a4,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        9},     //db_lum_v_str_fade_lut_p1
    {0x25a4,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        10},    //db_lum_v_str_fade_lut_p0
    /*VPSS_DB_LUT44~48*/
    {0x25c4,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        0},     //db_sobel_str_lut_p25
    {0x25c4,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        0},     //db_sobel_str_lut_p24
    {0x25c0,        25,     29,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_sobel_str_lut_p23
    {0x25c0,        20,     24,     0,      0,      HI_PQ_MODULE_DB,        1},     //db_sobel_str_lut_p22
    {0x25c0,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        2},     //db_sobel_str_lut_p21
    {0x25c0,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        2},     //db_sobel_str_lut_p20
    {0x25c0,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        2},     //db_sobel_str_lut_p19
    {0x25c0,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        2},     //db_sobel_str_lut_p18
    {0x25bc,        25,     29,     0,      0,      HI_PQ_MODULE_DB,        2},     //db_sobel_str_lut_p17
    {0x25bc,        20,     24,     0,      0,      HI_PQ_MODULE_DB,        2},     //db_sobel_str_lut_p16
    {0x25bc,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        4},     //db_sobel_str_lut_p15
    {0x25bc,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        4},     //db_sobel_str_lut_p14
    {0x25bc,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        4},     //db_sobel_str_lut_p13
    {0x25bc,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        5},     //db_sobel_str_lut_p12
    {0x25b8,        25,     29,     0,      0,      HI_PQ_MODULE_DB,        5},     //db_sobel_str_lut_p11
    {0x25b8,        20,     24,     0,      0,      HI_PQ_MODULE_DB,        6},     //db_sobel_str_lut_p10
    {0x25b8,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        7},     //db_sobel_str_lut_p9
    {0x25b8,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        9},     //db_sobel_str_lut_p8
    {0x25b8,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        10},    //db_sobel_str_lut_p7
    {0x25b8,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        11},    //db_sobel_str_lut_p6
    {0x25b4,        25,     29,     0,      0,      HI_PQ_MODULE_DB,        12},    //db_sobel_str_lut_p5
    {0x25b4,        20,     24,     0,      0,      HI_PQ_MODULE_DB,        12},    //db_sobel_str_lut_p4
    {0x25b4,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        13},    //db_sobel_str_lut_p3
    {0x25b4,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        14},    //db_sobel_str_lut_p2
    {0x25b4,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        15},    //db_sobel_str_lut_p1
    {0x25b4,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        16},    //db_sobel_str_lut_p0
    /*VPSS_DB_LUT49~51*/
    {0x25d0,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_dir_str_lut_p15
    {0x25d0,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_dir_str_lut_p14
    {0x25d0,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        0},     //db_dir_str_lut_p13
    {0x25d0,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        0},     //db_dir_str_lut_p12
    {0x25cc,        25,     29,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_dir_str_lut_p11
    {0x25cc,        20,     24,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_dir_str_lut_p10
    {0x25cc,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        2},     //db_dir_str_lut_p9
    {0x25cc,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        2},     //db_dir_str_lut_p8
    {0x25cc,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        12},    //db_dir_str_lut_p7
    {0x25cc,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        16},    //db_dir_str_lut_p6
    {0x25c8,        25,     29,     0,      0,      HI_PQ_MODULE_DB,        16},    //db_dir_str_lut_p5
    {0x25c8,        20,     24,     0,      0,      HI_PQ_MODULE_DB,        16},    //db_dir_str_lut_p4
    {0x25c8,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        16},    //db_dir_str_lut_p3
    {0x25c8,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        16},    //db_dir_str_lut_p2
    {0x25c8,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        16},    //db_dir_str_lut_p1
    {0x25c8,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        16},    //db_dir_str_lut_p0
    /*VPSS_DB_LUT52~53*/
    {0x25d8,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_dir_str_gain_lut_p9
    {0x25d8,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_dir_str_gain_lut_p8
    {0x25d8,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        2},     //db_dir_str_gain_lut_p7
    {0x25d8,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        4},     //db_dir_str_gain_lut_p6
    {0x25d4,        25,     29,     0,      0,      HI_PQ_MODULE_DB,        6},     //db_dir_str_gain_lut_p5
    {0x25d4,        20,     24,     0,      0,      HI_PQ_MODULE_DB,        8},     //db_dir_str_gain_lut_p4
    {0x25d4,        15,     19,     0,      0,      HI_PQ_MODULE_DB,        10},    //db_dir_str_gain_lut_p3
    {0x25d4,        10,     14,     0,      0,      HI_PQ_MODULE_DB,        12},    //db_dir_str_gain_lut_p2
    {0x25d4,        5,      9,      0,      0,      HI_PQ_MODULE_DB,        14},    //db_dir_str_gain_lut_p1
    {0x25d4,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        16},    //db_dir_str_gain_lut_p0
    /*VPSS_DB_LUT54~55*/
    {0x25e0,        24,     31,     0,      0,      HI_PQ_MODULE_DB,        120},   //db_diff_map_lut_p7
    {0x25e0,        16,     23,     0,      0,      HI_PQ_MODULE_DB,        80},    //db_diff_map_lut_p6
    {0x25e0,        8,      15,     0,      0,      HI_PQ_MODULE_DB,        40},    //db_diff_map_lut_p5
    {0x25e0,        0,      7,      0,      0,      HI_PQ_MODULE_DB,        36},    //db_diff_map_lut_p4
    {0x25dc,        24,     31,     0,      0,      HI_PQ_MODULE_DB,        28},    //db_diff_map_lut_p3
    {0x25dc,        16,     23,     0,      0,      HI_PQ_MODULE_DB,        24},    //db_diff_map_lut_p2
    {0x25dc,        8,      15,     0,      0,      HI_PQ_MODULE_DB,        12},    //db_diff_map_lut_p1
    {0x25dc,        0,      7,      0,      0,      HI_PQ_MODULE_DB,        8},     //db_diff_map_lut_p0
    /*VPSS_DB_LUT56~57*/
    {0x25e8,        20,     29,     0,      0,      HI_PQ_MODULE_DB,        99},    //db_diff_weight_lut_p5
    {0x25e8,        10,     19,     0,      0,      HI_PQ_MODULE_DB,        224},   //db_diff_weight_lut_p4
    {0x25e8,        0,      9,      0,      0,      HI_PQ_MODULE_DB,        112},   //db_diff_weight_lut_p3
    {0x25e4,        20,     29,     0,      0,      HI_PQ_MODULE_DB,        224},   //db_diff_weight_lut_p2
    {0x25e4,        10,     19,     0,      0,      HI_PQ_MODULE_DB,        416},   //db_diff_weight_lut_p1
    {0x25e4,        0,      9,      0,      0,      HI_PQ_MODULE_DB,        480},   //db_diff_weight_lut_p0
    /*VPSS_DB_LUT59~60*/
    {0x25f4,        6,      11,     0,      0,      HI_PQ_MODULE_DB,        60},    //db_diff_str_lut_p6
    {0x25f4,        0,      5,      0,      0,      HI_PQ_MODULE_DB,        44},    //db_diff_str_lut_p5
    {0x25f0,        24,     29,     0,      0,      HI_PQ_MODULE_DB,        40},    //db_diff_str_lut_p4
    {0x25f0,        18,     23,     0,      0,      HI_PQ_MODULE_DB,        32},    //db_diff_str_lut_p3
    {0x25f0,        12,     17,     0,      0,      HI_PQ_MODULE_DB,        28},    //db_diff_str_lut_p2
    {0x25f0,        6,      11,     0,      0,      HI_PQ_MODULE_DB,        8},     //db_diff_str_lut_p1
    {0x25f0,        0,      5,      0,      0,      HI_PQ_MODULE_DB,        0},     //db_diff_str_lut_p0
    /*VPSS_DB_CTRL1*/
    {0x25f8,        14,     14,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_streth_en
    {0x25f8,        13,     13,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_blk_pos_mode
    {0x25f8,        12,     12,     0,      0,      HI_PQ_MODULE_DB,        1},     //db_blk_mode
    {0x25f8,        6,      6,      0,      0,      HI_PQ_MODULE_DB,        0},     //db_vdh_trans_info_en
    {0x25f8,        5,      5,      0,      0,      HI_PQ_MODULE_DB,        1},     //db_det_en
    /*VPSS_DB_THR0~1*/
    {0x25fc,        19,     23,     0,      0,      HI_PQ_MODULE_DB,        8},     //db_ctrst_thresh
    {0x25fc,        14,     18,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_grad_sub_ratio
    {0x25fc,        12,     13,     0,      0,      HI_PQ_MODULE_DB,        3},     //db_dir_smooth_mode
    {0x25fc,        6,      11,     0,      0,      HI_PQ_MODULE_DB,        16},    //db_lum_v_border_str_th
    {0x25fc,        0,      5,      0,      0,      HI_PQ_MODULE_DB,        32},    //db_lum_h_border_str_th
    {0x2600,        24,     28,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_detect_ratio_v
    {0x2600,        19,     23,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_detect_ratio_h
    {0x2600,        14,     18,     0,      0,      HI_PQ_MODULE_DB,        10},    //db_text_str_gain
    {0x2620,        0,      3,      0,      0,      HI_PQ_MODULE_DB,        8},     //db_global_db_strength_lum
    {0x2628,        12,     15,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_hor_gain2
    {0x2628,        8,      11,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_hor_gain1
    {0x2628,        0,      7,      0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_hor_var_core
    /*VPSS_DB_OST0~1*/
    {0x2604,        16,     31,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_stt_offset_y
    {0x2604,        0,      15,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_stt_offset_x
    {0x2608,        16,     31,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_end_offset_y
    {0x2608,        0,      15,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_end_offset_x
    /*VPSS_DB_LUT61~62*/
    {0x2614,        28,     31,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_hor_delta_lut_p15
    {0x2614,        24,     27,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_hor_delta_lut_p14
    {0x2614,        20,     23,     0,      0,      HI_PQ_MODULE_DB,        2},     //db_lum_hor_delta_lut_p13
    {0x2614,        16,     19,     0,      0,      HI_PQ_MODULE_DB,        4},     //db_lum_hor_delta_lut_p12
    {0x2614,        12,     15,     0,      0,      HI_PQ_MODULE_DB,        6},     //db_lum_hor_delta_lut_p11
    {0x2614,        8,      11,     0,      0,      HI_PQ_MODULE_DB,        8},     //db_lum_hor_delta_lut_p10
    {0x2614,        4,      7,      0,      0,      HI_PQ_MODULE_DB,        8},     //db_lum_hor_delta_lut_p9
    {0x2614,        0,      3,      0,      0,      HI_PQ_MODULE_DB,        8},     //db_lum_hor_delta_lut_p8
    {0x2610,        28,     31,     0,      0,      HI_PQ_MODULE_DB,        8},     //db_lum_hor_delta_lut_p7
    {0x2610,        24,     27,     0,      0,      HI_PQ_MODULE_DB,        8},     //db_lum_hor_delta_lut_p6
    {0x2610,        20,     23,     0,      0,      HI_PQ_MODULE_DB,        8},     //db_lum_hor_delta_lut_p5
    {0x2610,        16,     19,     0,      0,      HI_PQ_MODULE_DB,        8},     //db_lum_hor_delta_lut_p4
    {0x2610,        12,     15,     0,      0,      HI_PQ_MODULE_DB,        8},     //db_lum_hor_delta_lut_p3
    {0x2610,        8,      11,     0,      0,      HI_PQ_MODULE_DB,        8},     //db_lum_hor_delta_lut_p2
    {0x2610,        4,      7,      0,      0,      HI_PQ_MODULE_DB,        8},     //db_lum_hor_delta_lut_p1
    {0x2610,        0,      3,      0,      0,      HI_PQ_MODULE_DB,        4},     //db_lum_hor_delta_lut_p0
    /*VPSS_DB_LUT63~64*/
    {0x261c,        28,     31,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p15
    {0x261c,        24,     27,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p14
    {0x261c,        20,     23,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p13
    {0x261c,        16,     19,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p12
    {0x261c,        12,     15,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p11
    {0x261c,        8,      11,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p10
    {0x261c,        4,      7,      0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p9
    {0x261c,        0,      3,      0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p8
    {0x2618,        28,     31,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p7
    {0x2618,        24,     27,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p6
    {0x2618,        20,     23,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p5
    {0x2618,        16,     19,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p4
    {0x2618,        12,     15,     0,      0,      HI_PQ_MODULE_DB,        2},     //db_lum_ver_delta_lut_p3
    {0x2618,        8,      11,     0,      0,      HI_PQ_MODULE_DB,        4},     //db_lum_ver_delta_lut_p2
    {0x2618,        4,      7,      0,      0,      HI_PQ_MODULE_DB,        6},     //db_lum_ver_delta_lut_p1
    {0x2618,        0,      3,      0,      0,      HI_PQ_MODULE_DB,        0},     //db_lum_ver_delta_lut_p0
    /*VPSS_DB_INFO*/
    {0x2624,        16,     31,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_blk_height
    {0x2624,        0,      15,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_blk_width
    /*VPSS_DB_THD*/
    {0x2640,        16,     21,     0,      0,      HI_PQ_MODULE_DB,        32},    //dbd_vy_thd_edge
    {0x2640,        8,      13,     0,      0,      HI_PQ_MODULE_DB,        32},    //dbd_hc_thd_edge
    {0x2640,        0,      5,      0,      0,      HI_PQ_MODULE_DB,        32},    //dbd_hy_thd_edge
    {0x2644,        16,     21,     0,      0,      HI_PQ_MODULE_DB,        16},    //dbd_vy_thd_txt
    {0x2644,        8,      13,     0,      0,      HI_PQ_MODULE_DB,        16},    //dbd_hc_thd_txt
    {0x2644,        0,      5,      0,      0,      HI_PQ_MODULE_DB,        16},    //dbd_hy_thd_txt
    /*DBD_LUTWGT*/
    {0x264c,        28,     31,     0,      0,      HI_PQ_MODULE_DB,        15},    //dbd_det_lut_wgt15
    {0x264c,        24,     27,     0,      0,      HI_PQ_MODULE_DB,        15},    //dbd_det_lut_wgt14
    {0x264c,        20,     23,     0,      0,      HI_PQ_MODULE_DB,        15},    //dbd_det_lut_wgt13
    {0x264c,        16,     19,     0,      0,      HI_PQ_MODULE_DB,        14},    //dbd_det_lut_wgt12
    {0x264c,        12,     15,     0,      0,      HI_PQ_MODULE_DB,        14},    //dbd_det_lut_wgt11
    {0x264c,        8,      11,     0,      0,      HI_PQ_MODULE_DB,        13},    //dbd_det_lut_wgt10
    {0x264c,        4,      7,      0,      0,      HI_PQ_MODULE_DB,        13},    //dbd_det_lut_wgt9
    {0x264c,        0,      3,      0,      0,      HI_PQ_MODULE_DB,        12},    //dbd_det_lut_wgt8
    {0x2648,        28,     31,     0,      0,      HI_PQ_MODULE_DB,        12},    //dbd_det_lut_wgt7
    {0x2648,        24,     27,     0,      0,      HI_PQ_MODULE_DB,        10},    //dbd_det_lut_wgt6
    {0x2648,        20,     23,     0,      0,      HI_PQ_MODULE_DB,        7},     //dbd_det_lut_wgt5
    {0x2648,        16,     19,     0,      0,      HI_PQ_MODULE_DB,        5},     //dbd_det_lut_wgt4
    {0x2648,        12,     15,     0,      0,      HI_PQ_MODULE_DB,        2},     //dbd_det_lut_wgt3
    {0x2648,        8,      11,     0,      0,      HI_PQ_MODULE_DB,        1},     //dbd_det_lut_wgt2
    {0x2648,        4,      7,      0,      0,      HI_PQ_MODULE_DB,        0},     //dbd_det_lut_wgt1
    {0x2648,        0,      3,      0,      0,      HI_PQ_MODULE_DB,        0},     //dbd_det_lut_wgt0
    /*DBD_TSTBLKNUM*/
    {0x2654,        20,     29,     0,      0,      HI_PQ_MODULE_DB,        60},    //dbd_vy_tst_blk_num
    {0x2654,        10,     19,     0,      0,      HI_PQ_MODULE_DB,        45},    //dbd_hc_tst_blk_num
    {0x2654,        0,      9,      0,      0,      HI_PQ_MODULE_DB,        90},    //dbd_hy_tst_blk_num
    /*DBD_RTNBDTXT*/
    {0x2650,        16,     20,     0,      0,      HI_PQ_MODULE_DB,        16},    //dbd_vy_rtn_bd_txt
    {0x2650,        8,      12,     0,      0,      HI_PQ_MODULE_DB,        16},    //dbd_hc_rtn_bd_txt
    {0x2650,        0,      4,      0,      0,      HI_PQ_MODULE_DB,        20},    //dbd_hy_rtn_bd_txt
    /*VPSS_DBD_HYRDX8BINLUT0~3*/
    {0x2664,        0,      10,     0,      0,      HI_PQ_MODULE_DB,        208},   //dbd_hy_rdx8bin_lut6
    {0x2660,        12,     22,     0,      0,      HI_PQ_MODULE_DB,        176},   //dbd_hy_rdx8bin_lut5
    {0x2660,        0,      10,     0,      0,      HI_PQ_MODULE_DB,        144},   //dbd_hy_rdx8bin_lut4
    {0x265c,        12,     22,     0,      0,      HI_PQ_MODULE_DB,        112},   //dbd_hy_rdx8bin_lut3
    {0x265c,        0,      10,     0,      0,      HI_PQ_MODULE_DB,        80},    //dbd_hy_rdx8bin_lut2
    {0x2658,        12,     22,     0,      0,      HI_PQ_MODULE_DB,        48},    //dbd_hy_rdx8bin_lut1
    {0x2658,        0,      10,     0,      0,      HI_PQ_MODULE_DB,        16},    //dbd_hy_rdx8bin_lut0
    /*VPSS_DBD_HCRDX8BINLUT0~3*/
    {0x2674,        0,      10,     0,      0,      HI_PQ_MODULE_DB,        208},   //dbd_hc_rdx8bin_lut6
    {0x2670,        12,     22,     0,      0,      HI_PQ_MODULE_DB,        176},   //dbd_hc_rdx8bin_lut5
    {0x2670,        0,      10,     0,      0,      HI_PQ_MODULE_DB,        144},   //dbd_hc_rdx8bin_lut4
    {0x266c,        12,     22,     0,      0,      HI_PQ_MODULE_DB,        112},   //dbd_hc_rdx8bin_lut3
    {0x266c,        0,      10,     0,      0,      HI_PQ_MODULE_DB,        80},    //dbd_hc_rdx8bin_lut2
    {0x2668,        12,     22,     0,      0,      HI_PQ_MODULE_DB,        48},    //dbd_hc_rdx8bin_lut1
    {0x2668,        0,      10,     0,      0,      HI_PQ_MODULE_DB,        16},    //dbd_hc_rdx8bin_lut0
    /*VPSS_DBD_VYRDX8BINLUT0~3*/
    {0x2684,        0,      10,     0,      0,      HI_PQ_MODULE_DB,        208},   //dbd_vy_rdx8bin_lut6
    {0x2680,        12,     22,     0,      0,      HI_PQ_MODULE_DB,        176},   //dbd_vy_rdx8bin_lut5
    {0x2680,        0,      10,     0,      0,      HI_PQ_MODULE_DB,        144},   //dbd_vy_rdx8bin_lut4
    {0x267c,        12,     22,     0,      0,      HI_PQ_MODULE_DB,        112},   //dbd_vy_rdx8bin_lut3
    {0x267c,        0,      10,     0,      0,      HI_PQ_MODULE_DB,        80},    //dbd_vy_rdx8bin_lut2
    {0x2678,        12,     22,     0,      0,      HI_PQ_MODULE_DB,        48},    //dbd_vy_rdx8bin_lut1
    {0x2678,        0,      10,     0,      0,      HI_PQ_MODULE_DB,        16},    //dbd_vy_rdx8bin_lut0
    /*VPSS_DBD_BLKSIZE*/
    {0x2688,        24,     30,     0,      0,      HI_PQ_MODULE_DB,        16},    //vy_max_blk_size
    {0x2688,        16,     22,     0,      0,      HI_PQ_MODULE_DB,        16},    //hc_max_blk_size
    {0x2688,        8,      14,     0,      0,      HI_PQ_MODULE_DB,        16},    //hy_max_blk_size
    {0x2688,        0,      5,      0,      0,      HI_PQ_MODULE_DB,        6},     //min_blk_size
    /*VPSS_DB_DEMO*/
    {0x260c,        0,      15,     0,      0,      HI_PQ_MODULE_DB,        0},     //db_demo_pos_x

    /***********************************************DM***********************************************/
    /*SD source*/
    /*DM_CTRL*/
    {0x2500,        1,      1,      1,      0,      HI_PQ_MODULE_DM,        1},     //dm_en
    {0x2700,        16,     17,     1,      0,      HI_PQ_MODULE_DM,        2},     //dm_opp_ang_ctrst_div
    {0x2700,        8,      15,     1,      0,      HI_PQ_MODULE_DM,        20},    //dm_opp_ang_ctrst_t
    {0x2700,        6,      7,      1,      0,      HI_PQ_MODULE_DM,        2},     //dm_ctrst_thresh
    {0x2700,        0,      4,      1,      0,      HI_PQ_MODULE_DM,        24},    //dm_grad_sub_ratio
    /*VPSS_DM_EDGE*/
    {0x2704,        26,     26,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_str_idx_flag
    {0x2704,        22,     25,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_global_str
    {0x2704,        18,     20,     1,      0,      HI_PQ_MODULE_DM,        4},     //dm_sw_thr_gain
    {0x2704,        14,     16,     1,      0,      HI_PQ_MODULE_DM,        5},     //dm_edge_gain2
    {0x2704,        10,     12,     1,      0,      HI_PQ_MODULE_DM,        4},     //dm_edge_gain1
    {0x2704,        6,      9,      1,      0,      HI_PQ_MODULE_DM,        0},     //dm_asymtrc_gain
    {0x2704,        0,      5,      1,      0,      HI_PQ_MODULE_DM,        0},     //dm_edge_thr
    /*VPSS_DM_LUT0~1*/
    {0x270c,        0,      6,      1,      0,      HI_PQ_MODULE_DM,        64},    //dm_sw_wht_lut_p4
    {0x2708,        24,     30,     1,      0,      HI_PQ_MODULE_DM,        32},    //dm_sw_wht_lut_p3
    {0x2708,        16,     22,     1,      0,      HI_PQ_MODULE_DM,        16},    //dm_sw_wht_lut_p2
    {0x2708,        8,      14,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_sw_wht_lut_p1
    {0x2708,        0,      6,      1,      0,      HI_PQ_MODULE_DM,        4},     //dm_sw_wht_lut_p0
    {0x270c,        16,     25,     1,      0,      HI_PQ_MODULE_DM,        72},    //dm_limit_t_10
    {0x270c,        8,      15,     1,      0,      HI_PQ_MODULE_DM,        18},    //dm_limit_t
    /*DM_LUT2*/
    {0x2710,        28,     31,     1,      0,      HI_PQ_MODULE_DM,        5},     //dm_dir_str_gain_lut_p7
    {0x2710,        24,     27,     1,      0,      HI_PQ_MODULE_DM,        6},     //dm_dir_str_gain_lut_p6
    {0x2710,        20,     23,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p5
    {0x2710,        16,     19,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p4
    {0x2710,        12,     15,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p3
    {0x2710,        8,      11,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p2
    {0x2710,        4,      7,      1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p1
    {0x2710,        0,      3,      1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p0
    /*DM_LUT3~4*/
    {0x2718,        28,     31,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p15
    {0x2718,        24,     27,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p14
    {0x2718,        20,     23,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p13
    {0x2718,        16,     19,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p12
    {0x2718,        12,     15,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p11
    {0x2718,        8,      11,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p10
    {0x2718,        4,      7,      1,      0,      HI_PQ_MODULE_DM,        1},     //dm_dir_str_lut_p9
    {0x2718,        0,      3,      1,      0,      HI_PQ_MODULE_DM,        1},     //dm_dir_str_lut_p8
    {0x2714,        28,     31,     1,      0,      HI_PQ_MODULE_DM,        6},     //dm_dir_str_lut_p7
    {0x2714,        24,     27,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p6
    {0x2714,        20,     23,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p5
    {0x2714,        16,     19,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p4
    {0x2714,        12,     15,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p3
    {0x2714,        8,      11,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p2
    {0x2714,        4,      7,      1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p1
    {0x2714,        0,      3,      1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p0
    /*VPSS_DM_DIRC*/
    {0x271c,        31,     31,     1,      0,      HI_PQ_MODULE_DM,        1},     //dm_mmf_limit_en
    {0x271c,        30,     30,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_mmf_en
    {0x271c,        26,     29,     1,      0,      HI_PQ_MODULE_DM,        9},     //dm_sim_dir_pt_count_t
    {0x271c,        18,     25,     1,      0,      HI_PQ_MODULE_DM,        18},    //dm_mndir_opp_ctrst_t
    {0x271c,        8,      17,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_ctrst_t_10
    {0x271c,        0,      7,      1,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_ctrst_t
    /*VPSS_DM_DIRO*/
    {0x2720,        22,     31,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_csw_trsnt_st_10
    {0x2720,        18,     21,     1,      0,      HI_PQ_MODULE_DM,        4},     //dm_dir_stren_shift_fac
    {0x2720,        8,      17,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_opp_ctrst_t_10
    {0x2720,        0,      7,      1,      0,      HI_PQ_MODULE_DM,        2},     //dm_dir_opp_ctrst_t
    /*VPSS_DM_LSW*/
    {0x2724,        27,     29,     1,      0,      HI_PQ_MODULE_DM,        4},     //dm_lsw_ratio
    {0x2724,        26,     26,     1,      0,      HI_PQ_MODULE_DM,        1},     //dm_sdbw_mode
    {0x2724,        18,     25,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_csw_trsnt_st
    {0x2724,        8,      17,     1,      0,      HI_PQ_MODULE_DM,        120},   //dm_lw_ctrst_t_10
    {0x2724,        0,      7,      1,      0,      HI_PQ_MODULE_DM,        30},    //dm_lw_ctrst_t
    /*VPSS_DM_MMFLR*/
    {0x2728,        20,     27,     1,      0,      HI_PQ_MODULE_DM,        40},    //dm_csw_trsnt_lt
    {0x2728,        9,      19,     1,      0,      HI_PQ_MODULE_DM,        100},   //dm_mmf_lr_10     (signed)
    {0x2728,        0,      8,      1,      0,      HI_PQ_MODULE_DM,        25},    //dm_mmf_lr        (signed)
    /*VPSS_DM_MMFSR*/
    {0x272c,        20,     29,     1,      0,      HI_PQ_MODULE_DM,        160},   //dm_csw_trsnt_lt_10
    {0x272c,        9,      19,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_mmf_sr_10      (signed)
    {0x272c,        0,      8,      1,      0,      HI_PQ_MODULE_DM,        0},     //dm_mmf_sr         (signed)
    /*VPSS_DM_DIRLR*/
    {0x2730,        9,      19,     1,      0,      HI_PQ_MODULE_DM,        80},    //dm_dir_mmf_lr_10  (signed)
    {0x2730,        0,      8,      1,      0,      HI_PQ_MODULE_DM,        20},    //dm_dir_mmf_lr     (signed)
    /*VPSS_DM_DIRSR*/
    {0x2734,        9,      19,     1,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_mmf_sr_10  (signed)
    {0x2734,        0,      8,      1,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_mmf_sr     (signed)
    /*VPSS_DM_THR*/
    {0x2738,        12,     15,     1,      0,      HI_PQ_MODULE_DM,        6},     //dm_dir_blend_str
    {0x2738,        8,      11,     1,      0,      HI_PQ_MODULE_DM,        7},     //dm_lim_res_blend_str2
    {0x2738,        4,      7,      1,      0,      HI_PQ_MODULE_DM,        7},     //dm_lim_res_blend_str1
    {0x2738,        0,      2,      1,      0,      HI_PQ_MODULE_DM,        5},     //dm_limit_lsw_ratio
    {0x273c,        16,     23,     1,      0,      HI_PQ_MODULE_DM,        6},     //dm_lim_trans_wid
    {0x273c,        8,      15,     1,      0,      HI_PQ_MODULE_DM,        2},     //dm_min_lim_val_trans
    {0x273c,        0,      7,      1,      0,      HI_PQ_MODULE_DM,        2},     //dm_min_lim_trans_wid
    {0x2740,        20,     29,     1,      0,      HI_PQ_MODULE_DM,        24},    //dm_lim_trans_wid_10
    {0x2740,        10,     19,     1,      0,      HI_PQ_MODULE_DM,        8},     //dm_min_lim_val_trans_10
    {0x2740,        0,      9,      1,      0,      HI_PQ_MODULE_DM,        8},     //dm_min_lim_trans_wid_10

    /*HD source*/
    /*DM_CTRL*/
    {0x2500,        1,      1,      2,      0,      HI_PQ_MODULE_DM,        1},     //dm_en
    {0x2700,        16,     17,     2,      0,      HI_PQ_MODULE_DM,        2},     //dm_opp_ang_ctrst_div
    {0x2700,        8,      15,     2,      0,      HI_PQ_MODULE_DM,        20},    //dm_opp_ang_ctrst_t
    {0x2700,        6,      7,      2,      0,      HI_PQ_MODULE_DM,        2},     //dm_ctrst_thresh
    {0x2700,        0,      4,      2,      0,      HI_PQ_MODULE_DM,        24},    //dm_grad_sub_ratio
    /*VPSS_DM_EDGE*/
    {0x2704,        26,     26,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_str_idx_flag
    {0x2704,        22,     25,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_global_str
    {0x2704,        18,     20,     2,      0,      HI_PQ_MODULE_DM,        4},     //dm_sw_thr_gain
    {0x2704,        14,     16,     2,      0,      HI_PQ_MODULE_DM,        5},     //dm_edge_gain2
    {0x2704,        10,     12,     2,      0,      HI_PQ_MODULE_DM,        4},     //dm_edge_gain1
    {0x2704,        6,      9,      2,      0,      HI_PQ_MODULE_DM,        0},     //dm_asymtrc_gain
    {0x2704,        0,      5,      2,      0,      HI_PQ_MODULE_DM,        0},     //dm_edge_thr
    /*VPSS_DM_LUT0~1*/
    {0x270c,        0,      6,      2,      0,      HI_PQ_MODULE_DM,        64},    //dm_sw_wht_lut_p4
    {0x2708,        24,     30,     2,      0,      HI_PQ_MODULE_DM,        32},    //dm_sw_wht_lut_p3
    {0x2708,        16,     22,     2,      0,      HI_PQ_MODULE_DM,        16},    //dm_sw_wht_lut_p2
    {0x2708,        8,      14,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_sw_wht_lut_p1
    {0x2708,        0,      6,      2,      0,      HI_PQ_MODULE_DM,        4},     //dm_sw_wht_lut_p0
    {0x270c,        16,     25,     2,      0,      HI_PQ_MODULE_DM,        72},    //dm_limit_t_10
    {0x270c,        8,      15,     2,      0,      HI_PQ_MODULE_DM,        18},    //dm_limit_t
    /*DM_LUT2*/
    {0x2710,        28,     31,     2,      0,      HI_PQ_MODULE_DM,        5},     //dm_dir_str_gain_lut_p7
    {0x2710,        24,     27,     2,      0,      HI_PQ_MODULE_DM,        6},     //dm_dir_str_gain_lut_p6
    {0x2710,        20,     23,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p5
    {0x2710,        16,     19,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p4
    {0x2710,        12,     15,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p3
    {0x2710,        8,      11,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p2
    {0x2710,        4,      7,      2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p1
    {0x2710,        0,      3,      2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p0
    /*DM_LUT3~4*/
    {0x2718,        28,     31,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p15
    {0x2718,        24,     27,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p14
    {0x2718,        20,     23,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p13
    {0x2718,        16,     19,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p12
    {0x2718,        12,     15,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p11
    {0x2718,        8,      11,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p10
    {0x2718,        4,      7,      2,      0,      HI_PQ_MODULE_DM,        1},     //dm_dir_str_lut_p9
    {0x2718,        0,      3,      2,      0,      HI_PQ_MODULE_DM,        1},     //dm_dir_str_lut_p8
    {0x2714,        28,     31,     2,      0,      HI_PQ_MODULE_DM,        6},     //dm_dir_str_lut_p7
    {0x2714,        24,     27,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p6
    {0x2714,        20,     23,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p5
    {0x2714,        16,     19,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p4
    {0x2714,        12,     15,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p3
    {0x2714,        8,      11,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p2
    {0x2714,        4,      7,      2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p1
    {0x2714,        0,      3,      2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p0
    /*VPSS_DM_DIRC*/
    {0x271c,        31,     31,     2,      0,      HI_PQ_MODULE_DM,        1},     //dm_mmf_limit_en
    {0x271c,        30,     30,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_mmf_en
    {0x271c,        26,     29,     2,      0,      HI_PQ_MODULE_DM,        9},     //dm_sim_dir_pt_count_t
    {0x271c,        18,     25,     2,      0,      HI_PQ_MODULE_DM,        18},    //dm_mndir_opp_ctrst_t
    {0x271c,        8,      17,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_ctrst_t_10
    {0x271c,        0,      7,      2,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_ctrst_t
    /*VPSS_DM_DIRO*/
    {0x2720,        22,     31,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_csw_trsnt_st_10
    {0x2720,        18,     21,     2,      0,      HI_PQ_MODULE_DM,        4},     //dm_dir_stren_shift_fac
    {0x2720,        8,      17,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_opp_ctrst_t_10
    {0x2720,        0,      7,      2,      0,      HI_PQ_MODULE_DM,        2},     //dm_dir_opp_ctrst_t
    /*VPSS_DM_LSW*/
    {0x2724,        27,     29,     2,      0,      HI_PQ_MODULE_DM,        4},     //dm_lsw_ratio
    {0x2724,        26,     26,     2,      0,      HI_PQ_MODULE_DM,        1},     //dm_sdbw_mode
    {0x2724,        18,     25,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_csw_trsnt_st
    {0x2724,        8,      17,     2,      0,      HI_PQ_MODULE_DM,        120},   //dm_lw_ctrst_t_10
    {0x2724,        0,      7,      2,      0,      HI_PQ_MODULE_DM,        30},    //dm_lw_ctrst_t
    /*VPSS_DM_MMFLR*/
    {0x2728,        20,     27,     2,      0,      HI_PQ_MODULE_DM,        40},    //dm_csw_trsnt_lt
    {0x2728,        9,      19,     2,      0,      HI_PQ_MODULE_DM,        100},   //dm_mmf_lr_10     (signed)
    {0x2728,        0,      8,      2,      0,      HI_PQ_MODULE_DM,        25},    //dm_mmf_lr        (signed)
    /*VPSS_DM_MMFSR*/
    {0x272c,        20,     29,     2,      0,      HI_PQ_MODULE_DM,        160},   //dm_csw_trsnt_lt_10
    {0x272c,        9,      19,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_mmf_sr_10      (signed)
    {0x272c,        0,      8,      2,      0,      HI_PQ_MODULE_DM,        0},     //dm_mmf_sr         (signed)
    /*VPSS_DM_DIRLR*/
    {0x2730,        9,      19,     2,      0,      HI_PQ_MODULE_DM,        80},    //dm_dir_mmf_lr_10  (signed)
    {0x2730,        0,      8,      2,      0,      HI_PQ_MODULE_DM,        20},    //dm_dir_mmf_lr     (signed)
    /*VPSS_DM_DIRSR*/
    {0x2734,        9,      19,     2,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_mmf_sr_10  (signed)
    {0x2734,        0,      8,      2,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_mmf_sr     (signed)
    /*VPSS_DM_THR*/
    {0x2738,        12,     15,     2,      0,      HI_PQ_MODULE_DM,        6},     //dm_dir_blend_str
    {0x2738,        8,      11,     2,      0,      HI_PQ_MODULE_DM,        7},     //dm_lim_res_blend_str2
    {0x2738,        4,      7,      2,      0,      HI_PQ_MODULE_DM,        7},     //dm_lim_res_blend_str1
    {0x2738,        0,      2,      2,      0,      HI_PQ_MODULE_DM,        5},     //dm_limit_lsw_ratio
    {0x273c,        16,     23,     2,      0,      HI_PQ_MODULE_DM,        6},     //dm_lim_trans_wid
    {0x273c,        8,      15,     2,      0,      HI_PQ_MODULE_DM,        2},     //dm_min_lim_val_trans
    {0x273c,        0,      7,      2,      0,      HI_PQ_MODULE_DM,        2},     //dm_min_lim_trans_wid
    {0x2740,        20,     29,     2,      0,      HI_PQ_MODULE_DM,        24},    //dm_lim_trans_wid_10
    {0x2740,        10,     19,     2,      0,      HI_PQ_MODULE_DM,        8},     //dm_min_lim_val_trans_10
    {0x2740,        0,      9,      2,      0,      HI_PQ_MODULE_DM,        8},     //dm_min_lim_trans_wid_10

    /*UHD source*/
    /*DM_CTRL*/
    {0x2500,        1,      1,      3,      0,      HI_PQ_MODULE_DM,        1},     //dm_en
    {0x2700,        16,     17,     3,      0,      HI_PQ_MODULE_DM,        2},     //dm_opp_ang_ctrst_div
    {0x2700,        8,      15,     3,      0,      HI_PQ_MODULE_DM,        20},    //dm_opp_ang_ctrst_t
    {0x2700,        6,      7,      3,      0,      HI_PQ_MODULE_DM,        2},     //dm_ctrst_thresh
    {0x2700,        0,      4,      3,      0,      HI_PQ_MODULE_DM,        24},    //dm_grad_sub_ratio
    /*VPSS_DM_EDGE*/
    {0x2704,        26,     26,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_str_idx_flag
    {0x2704,        22,     25,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_global_str
    {0x2704,        18,     20,     3,      0,      HI_PQ_MODULE_DM,        4},     //dm_sw_thr_gain
    {0x2704,        14,     16,     3,      0,      HI_PQ_MODULE_DM,        5},     //dm_edge_gain2
    {0x2704,        10,     12,     3,      0,      HI_PQ_MODULE_DM,        4},     //dm_edge_gain1
    {0x2704,        6,      9,      3,      0,      HI_PQ_MODULE_DM,        0},     //dm_asymtrc_gain
    {0x2704,        0,      5,      3,      0,      HI_PQ_MODULE_DM,        0},     //dm_edge_thr
    /*VPSS_DM_LUT0~1*/
    {0x270c,        0,      6,      3,      0,      HI_PQ_MODULE_DM,        64},    //dm_sw_wht_lut_p4
    {0x2708,        24,     30,     3,      0,      HI_PQ_MODULE_DM,        32},    //dm_sw_wht_lut_p3
    {0x2708,        16,     22,     3,      0,      HI_PQ_MODULE_DM,        16},    //dm_sw_wht_lut_p2
    {0x2708,        8,      14,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_sw_wht_lut_p1
    {0x2708,        0,      6,      3,      0,      HI_PQ_MODULE_DM,        4},     //dm_sw_wht_lut_p0
    {0x270c,        16,     25,     3,      0,      HI_PQ_MODULE_DM,        72},    //dm_limit_t_10
    {0x270c,        8,      15,     3,      0,      HI_PQ_MODULE_DM,        18},    //dm_limit_t
    /*DM_LUT2*/
    {0x2710,        28,     31,     3,      0,      HI_PQ_MODULE_DM,        5},     //dm_dir_str_gain_lut_p7
    {0x2710,        24,     27,     3,      0,      HI_PQ_MODULE_DM,        6},     //dm_dir_str_gain_lut_p6
    {0x2710,        20,     23,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p5
    {0x2710,        16,     19,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p4
    {0x2710,        12,     15,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p3
    {0x2710,        8,      11,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p2
    {0x2710,        4,      7,      3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p1
    {0x2710,        0,      3,      3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_gain_lut_p0
    /*DM_LUT3~4*/
    {0x2718,        28,     31,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p15
    {0x2718,        24,     27,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p14
    {0x2718,        20,     23,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p13
    {0x2718,        16,     19,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p12
    {0x2718,        12,     15,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p11
    {0x2718,        8,      11,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_str_lut_p10
    {0x2718,        4,      7,      3,      0,      HI_PQ_MODULE_DM,        1},     //dm_dir_str_lut_p9
    {0x2718,        0,      3,      3,      0,      HI_PQ_MODULE_DM,        1},     //dm_dir_str_lut_p8
    {0x2714,        28,     31,     3,      0,      HI_PQ_MODULE_DM,        6},     //dm_dir_str_lut_p7
    {0x2714,        24,     27,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p6
    {0x2714,        20,     23,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p5
    {0x2714,        16,     19,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p4
    {0x2714,        12,     15,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p3
    {0x2714,        8,      11,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p2
    {0x2714,        4,      7,      3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p1
    {0x2714,        0,      3,      3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_str_lut_p0
    /*VPSS_DM_DIRC*/
    {0x271c,        31,     31,     3,      0,      HI_PQ_MODULE_DM,        1},     //dm_mmf_limit_en
    {0x271c,        30,     30,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_mmf_en
    {0x271c,        26,     29,     3,      0,      HI_PQ_MODULE_DM,        9},     //dm_sim_dir_pt_count_t
    {0x271c,        18,     25,     3,      0,      HI_PQ_MODULE_DM,        18},    //dm_mndir_opp_ctrst_t
    {0x271c,        8,      17,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_ctrst_t_10
    {0x271c,        0,      7,      3,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_ctrst_t
    /*VPSS_DM_DIRO*/
    {0x2720,        22,     31,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_csw_trsnt_st_10
    {0x2720,        18,     21,     3,      0,      HI_PQ_MODULE_DM,        4},     //dm_dir_stren_shift_fac
    {0x2720,        8,      17,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_dir_opp_ctrst_t_10
    {0x2720,        0,      7,      3,      0,      HI_PQ_MODULE_DM,        2},     //dm_dir_opp_ctrst_t
    /*VPSS_DM_LSW*/
    {0x2724,        27,     29,     3,      0,      HI_PQ_MODULE_DM,        4},     //dm_lsw_ratio
    {0x2724,        26,     26,     3,      0,      HI_PQ_MODULE_DM,        1},     //dm_sdbw_mode
    {0x2724,        18,     25,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_csw_trsnt_st
    {0x2724,        8,      17,     3,      0,      HI_PQ_MODULE_DM,        120},   //dm_lw_ctrst_t_10
    {0x2724,        0,      7,      3,      0,      HI_PQ_MODULE_DM,        30},    //dm_lw_ctrst_t
    /*VPSS_DM_MMFLR*/
    {0x2728,        20,     27,     3,      0,      HI_PQ_MODULE_DM,        40},    //dm_csw_trsnt_lt
    {0x2728,        9,      19,     3,      0,      HI_PQ_MODULE_DM,        100},   //dm_mmf_lr_10     (signed)
    {0x2728,        0,      8,      3,      0,      HI_PQ_MODULE_DM,        25},    //dm_mmf_lr        (signed)
    /*VPSS_DM_MMFSR*/
    {0x272c,        20,     29,     3,      0,      HI_PQ_MODULE_DM,        160},   //dm_csw_trsnt_lt_10
    {0x272c,        9,      19,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_mmf_sr_10      (signed)
    {0x272c,        0,      8,      3,      0,      HI_PQ_MODULE_DM,        0},     //dm_mmf_sr         (signed)
    /*VPSS_DM_DIRLR*/
    {0x2730,        9,      19,     3,      0,      HI_PQ_MODULE_DM,        80},    //dm_dir_mmf_lr_10  (signed)
    {0x2730,        0,      8,      3,      0,      HI_PQ_MODULE_DM,        20},    //dm_dir_mmf_lr     (signed)
    /*VPSS_DM_DIRSR*/
    {0x2734,        9,      19,     3,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_mmf_sr_10  (signed)
    {0x2734,        0,      8,      3,      0,      HI_PQ_MODULE_DM,        0},     //dm_dir_mmf_sr     (signed)
    /*VPSS_DM_THR*/
    {0x2738,        12,     15,     3,      0,      HI_PQ_MODULE_DM,        6},     //dm_dir_blend_str
    {0x2738,        8,      11,     3,      0,      HI_PQ_MODULE_DM,        7},     //dm_lim_res_blend_str2
    {0x2738,        4,      7,      3,      0,      HI_PQ_MODULE_DM,        7},     //dm_lim_res_blend_str1
    {0x2738,        0,      2,      3,      0,      HI_PQ_MODULE_DM,        5},     //dm_limit_lsw_ratio
    {0x273c,        16,     23,     3,      0,      HI_PQ_MODULE_DM,        6},     //dm_lim_trans_wid
    {0x273c,        8,      15,     3,      0,      HI_PQ_MODULE_DM,        2},     //dm_min_lim_val_trans
    {0x273c,        0,      7,      3,      0,      HI_PQ_MODULE_DM,        2},     //dm_min_lim_trans_wid
    {0x2740,        20,     29,     3,      0,      HI_PQ_MODULE_DM,        24},    //dm_lim_trans_wid_10
    {0x2740,        10,     19,     3,      0,      HI_PQ_MODULE_DM,        8},     //dm_min_lim_val_trans_10
    {0x2740,        0,      9,      3,      0,      HI_PQ_MODULE_DM,        8},     //dm_min_lim_trans_wid_10
};

static PQ_PHY_REG_S sg_stPQSoftReg[SOFT_REG_MAX] =
{
    /***************************************************************************
      * Special Register
     ***************************************************************************/

    /*Addr                                   Lsb       Msb    SourceMode  OutputMode   Module     Value        Description*/
    /***********************************************FMD_CTRL***********************************************/
    /*IP_DETECT*/
    {PQ_SOFT_IP_DETECT,	        0,		 1, 	   0,		 0, 	HI_PQ_MODULE_FMD, 	    0},		  //ip_detect_choice
    {PQ_SOFT_FRMRATETHR_L,	    0,		15, 	   0,		 0, 	HI_PQ_MODULE_FMD,   23800},		  //g_u32FrmRateThr_L
    {PQ_SOFT_FRMRATETHR_H,	    0,		15, 	   0,		 0, 	HI_PQ_MODULE_FMD,   24200},		  //g_u32FrmRateThr_H

    /*HDR_SATU*/
    {PQ_SOFT_HDR_SATU,	        0,		 0, 	   0,		 0, 	HI_PQ_MODULE_HDR,      50},		  //hdr_saturation

    /*COLOR_TEMP*/
    {PQ_SOFT_COLOR_TEMP_R,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_CSC, 	   50},		  //colortemp red
    {PQ_SOFT_COLOR_TEMP_G,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_CSC,      50},		  //colortemp green
    {PQ_SOFT_COLOR_TEMP_B,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_CSC,      50},		  //colortemp blue

    /*MODULE_ON_OR_OFF*/
    {PQ_SOFT_MODULE_FMD_SD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_FMD, 	    1},		  //fmd sd on/off
    {PQ_SOFT_MODULE_FMD_HD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_FMD,       1},		  //fmd hd on/off
    {PQ_SOFT_MODULE_FMD_UHD,    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_FMD,       1},		  //fmd uhd on/off
    {PQ_SOFT_MODULE_TNR_SD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_TNR, 	    1},		  //tnr sd on/off
    {PQ_SOFT_MODULE_TNR_HD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_TNR,       1},		  //tnr hd on/off
    {PQ_SOFT_MODULE_TNR_UHD,    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_TNR,       0},		  //tnr uhd on/off
    {PQ_SOFT_MODULE_SNR_SD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_SNR, 	    1},		  //snr sd on/off
    {PQ_SOFT_MODULE_SNR_HD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_SNR,       1},		  //snr hd on/off
    {PQ_SOFT_MODULE_SNR_UHD,    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_SNR,       1},		  //snr uhd on/off
    {PQ_SOFT_MODULE_DB_SD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DB, 	    1},		  //db sd on/off
    {PQ_SOFT_MODULE_DB_HD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DB,        1},		  //db hd on/off
    {PQ_SOFT_MODULE_DB_UHD,     0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DB,        0},		  //db uhd on/off
    {PQ_SOFT_MODULE_DR_SD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DR, 	    1},		  //dr sd on/off
    {PQ_SOFT_MODULE_DR_HD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DR,        1},		  //dr hd on/off
    {PQ_SOFT_MODULE_DR_UHD,     0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DR,        1},		  //dr uhd on/off
    {PQ_SOFT_MODULE_DM_SD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DM, 	    1},		  //dm sd on/off
    {PQ_SOFT_MODULE_DM_HD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DM,        1},		  //dm hd on/off
    {PQ_SOFT_MODULE_DM_UHD,     0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DM,        0},		  //dm uhd on/off
    {PQ_SOFT_MODULE_SHARPN_SD,	0,		 0, 	   0,		 0, 	HI_PQ_MODULE_SHARPNESS, 1},		  //sharp sd on/off
    {PQ_SOFT_MODULE_SHARPN_HD,  0,		 0, 	   0,		 0, 	HI_PQ_MODULE_SHARPNESS, 1},		  //sharphd on/off
    {PQ_SOFT_MODULE_SHARPN_UHD, 0,		 0, 	   0,		 0, 	HI_PQ_MODULE_SHARPNESS, 0},		  //sharp uhd on/off
    {PQ_SOFT_MODULE_DCI_SD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DCI, 	    1},		  //dci sd on/off
    {PQ_SOFT_MODULE_DCI_HD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DCI,       1},		  //dci hd on/off
    {PQ_SOFT_MODULE_DCI_UHD,    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DCI,       0},		  //dci uhd on/off
    {PQ_SOFT_MODULE_ACM_SD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_COLOR, 	1},		  //acm sd on/off
    {PQ_SOFT_MODULE_ACM_HD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_COLOR,     1},		  //acm hd on/off
    {PQ_SOFT_MODULE_ACM_UHD,    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_COLOR,     1},		  //acm uhd on/off
    {PQ_SOFT_MODULE_CSC_SD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_CSC, 	    1},		  //csc sd on/off
    {PQ_SOFT_MODULE_CSC_HD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_CSC,       1},		  //csc hd on/off
    {PQ_SOFT_MODULE_CSC_UHD,    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_CSC,       1},		  //csc uhd on/off
    {PQ_SOFT_MODULE_DS_SD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DS, 	    1},		  //fmd sd on/off
    {PQ_SOFT_MODULE_DS_HD,	    0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DS,        1},		  //fmd hd on/off
    {PQ_SOFT_MODULE_DS_UHD,     0,		 0, 	   0,		 0, 	HI_PQ_MODULE_DS,        1},		  //fmd uhd on/off
};


PQ_PHY_REG_S* PQ_TABLE_GetPhyRegDefault(HI_VOID)
{
    return sg_stPhyRegDefault;
}

PQ_PHY_REG_S* PQ_TABLE_GetSoftRegDefault(HI_VOID)
{
    return sg_stPQSoftReg;
}

