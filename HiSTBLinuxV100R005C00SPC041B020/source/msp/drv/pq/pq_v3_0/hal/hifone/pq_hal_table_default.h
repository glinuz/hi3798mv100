/*****************************************************************************
 *
 * Copyright (C) 2014-2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
 *
 * This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
 *  and may not be copied, reproduced, modified, disclosed to others, published or used, in
 * whole or in part, without the express prior written permission of Hisilicon.
 *
 *****************************************************************************

   File Name     : pq_hal_table_default.h
   Version       : Initial Draft
   Author        : p00203646
   Created       : 2014/04/01
   Author        : p00203646
 ******************************************************************************/

#ifndef __PQ_MNG_PQ_HIFONE_DEFAULT_H__
#define __PQ_MNG_PQ_HIFONE_DEFAULT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


static PQ_PHY_REG_S sg_stPhyReg[] =
{
    /*Addr         Lsb      Msb   SourceMode  OutputMode  Module     Value       Description*/


    /****************************************DEI****************************************/
    /*DEI_CTRL*/
    {0xf8cb1000,    29,      29,        0,        0,      0x1,        0},         //die_out_sel_l
    {0xf8cb1000,    28,      28,        0,        0,      0x1,        0},         //die_out_sel_c
    /*DIELMA2*/
    {0xf8cb1004,    7,       7,         0,        0,      0x1,        1},         //rec_mode_en
    {0xf8cb1004,    6,       6,         0,        0,      0x1,        0},         //chroma_mf_max
    {0xf8cb1004,    5,       5,         0,        0,      0x1,        0},         //luma_mf_max
    {0xf8cb1004,    4,       4,         0,        0,      0x1,        1},         //motion_iir_en
    {0xf8cb1004,    2,       2,         0,        0,      0x1,        0},         //luma_scesdf_max
    {0xf8cb1004,    1,       1,         0,        0,      0x1,        1},         //frame_motion_smooth_en
    {0xf8cb1004,    0,       0,         0,        0,      0x1,        0},         //recmode_frmfld_blend_mode
    /*DIECHECK*/
    {0xf8cb1010,    16,      19,        0,        0,      0x1,        8},         //ck1_gain
    {0xf8cb1010,    8,       11,        0,        0,      0x1,        2},         //ck1_range_gain
    {0xf8cb1010,    0,       7,         0,        0,      0x1,       30},         //ck1_max_range
    {0xf8cb1014,    16,      19,        0,        0,      0x1,        8},         //ck2_gain
    {0xf8cb1014,    8,       11,        0,        0,      0x1,        2},         //ck2_range_gain
    {0xf8cb1014,    0,       7,         0,        0,      0x1,       30},         //ck2_max_range
    /*DIEDIR*/
    {0xf8cb1024,    16,      21,        0,        0,      0x1,        3},         //dir14_mult
    {0xf8cb1024,    8,       13,        0,        0,      0x1,        5},         //dir13_mult
    {0xf8cb1024,    0,       5,         0,        0,      0x1,        5},         //dir12_mult
    {0xf8cb1020,    24,      29,        0,        0,      0x1,        6},         //dir11_mult
    {0xf8cb1020,    16,      21,        0,        0,      0x1,        7},         //dir10_mult
    {0xf8cb1020,    8,       13,        0,        0,      0x1,        8},         //dir9_mult
    {0xf8cb1020,    0,       5,         0,        0,      0x1,        9},         //dir8_mult
    {0xf8cb101c,    24,      29,        0,        0,      0x1,       11},         //dir7_mult
    {0xf8cb101c,    16,      21,        0,        0,      0x1,       12},         //dir6_mult
    {0xf8cb101c,    8,       13,        0,        0,      0x1,       15},         //dir5_mult
    {0xf8cb101c,    0,       5,         0,        0,      0x1,       18},         //dir4_mult
    {0xf8cb1018,    24,      29,        0,        0,      0x1,       27},         //dir3_mult
    {0xf8cb1018,    16,      21,        0,        0,      0x1,       32},         //dir2_mult
    {0xf8cb1018,    8,       13,        0,        0,      0x1,       24},         //dir1_mult
    {0xf8cb1018,    0,       5,         0,        0,      0x1,       40},         //dir0_mult
    /*INTP_SCALE_RATIO*/
    {0xf8cb1038,    24,      27,        0,        0,      0x1,        8},         //intp_scale_ratio_15
    {0xf8cb1038,    20,      23,        0,        0,      0x1,        8},         //intp_scale_ratio_14
    {0xf8cb1038,    16,      19,        0,        0,      0x1,        8},         //intp_scale_ratio_13
    {0xf8cb1038,    12,      15,        0,        0,      0x1,        8},         //intp_scale_ratio_12
    {0xf8cb1038,    8,       11,        0,        0,      0x1,        8},         //intp_scale_ratio_11
    {0xf8cb1038,    4,       7,         0,        0,      0x1,        8},         //intp_scale_ratio_10
    {0xf8cb1038,    0,       3,         0,        0,      0x1,        8},         //intp_scale_ratio_9
    {0xf8cb1034,    28,      31,        0,        0,      0x1,        8},         //intp_scale_ratio_8
    {0xf8cb1034,    24,      27,        0,        0,      0x1,        7},         //intp_scale_ratio_7
    {0xf8cb1034,    20,      23,        0,        0,      0x1,        7},         //intp_scale_ratio_6
    {0xf8cb1034,    16,      19,        0,        0,      0x1,        6},         //intp_scale_ratio_5
    {0xf8cb1034,    12,      15,        0,        0,      0x1,        6},         //intp_scale_ratio_4
    {0xf8cb1034,    8,       11,        0,        0,      0x1,        5},         //intp_scale_ratio_3
    {0xf8cb1034,    4,       7,         0,        0,      0x1,        5},         //intp_scale_ratio_2
    {0xf8cb1034,    0,       3,         0,        0,      0x1,        6},         //intp_scale_ratio_1
    /*DIEINTPCTRL*/
    {0xf8cb103c,    16,      31,        0,        0,      0x1,     5000},         //strength_thd
    {0xf8cb103c,    13,      13,        0,        0,      0x1,        0},         //hor_edge_en
    {0xf8cb103c,    12,      12,        0,        0,      0x1,        0},         //edge_mode
    {0xf8cb103c,    8,       11,        0,        0,      0x1,        4},         //dir_thd
    {0xf8cb103c,    0,        6,        0,        0,      0x1,       32},         //bc_gain
    {0xf8cb1008,    16,      31,        0,        0,      0x1,     -320},         //ver_min_inten
    {0xf8cb1008,    8,       11,        0,        0,      0x1,        2},         //dir_inten_ver
    {0xf8cb100c,    0,       7,         0,        0,      0x1,        2},         //range_scale
    /*DIEJITMTN*/
    {0xf8cb1040,    4,       11,        0,        0,      0x1,        0},         //jitter_coring
    {0xf8cb1040,    0,       3,         0,        0,      0x1,        0},         //jitter_gain
    /*DIEFLDMTN*/
    {0xf8cb1040,    12,      19,        0,        0,      0x1,        0},         //fld_motion_coring
    {0xf8cb1044,    24,      27,        0,        0,      0x1,        8},         //fld_motion_gain
    {0xf8cb1044,    16,      21,        0,        0,      0x1,       -2},         //fld_motion_curve_slope
    {0xf8cb1044,    8,       15,        0,        0,      0x1,      255},         //fld_motion_thd_high
    {0xf8cb1044,    0,       7,         0,        0,      0x1,        0},         //fld_motion_thd_low
    /*DIEMTNDIFFTHD*/
    {0xf8cb104c,    24,      30,        0,        0,      0x1,       64},         //max_motion_iir_ratio
    {0xf8cb104c,    16,      22,        0,        0,      0x1,       32},         //min_motion_iir_ratio
    {0xf8cb104c,    8,       15,        0,        0,      0x1,      255},         //motion_diff_thd_5
    {0xf8cb104c,    0,       7,         0,        0,      0x1,      255},         //motion_diff_thd_4
    {0xf8cb1048,    24,      31,        0,        0,      0x1,      255},         //motion_diff_thd_3
    {0xf8cb1048,    16,      23,        0,        0,      0x1,      208},         //motion_diff_thd_2
    {0xf8cb1048,    8,       15,        0,        0,      0x1,      144},         //motion_diff_thd_1
    {0xf8cb1048,    0,       7,         0,        0,      0x1,       16},         //motion_diff_thd_0
    /*DIEMTNIIRSLP*/
    {0xf8cb1050,    18,      23,        0,        0,      0x1,        0},         //motion_iir_curve_slope_3
    {0xf8cb1050,    12,      17,        0,        0,      0x1,        0},         //motion_iir_curve_slope_2
    {0xf8cb1050,    6,       11,        0,        0,      0x1,        2},         //motion_iir_curve_slope_1
    {0xf8cb1050,    0,       5,         0,        0,      0x1,        1},         //motion_iir_curve_slope_0
    /*DIEMTNIIRRAT*/
    {0xf8cb1054,    24,      30,        0,        0,      0x1,       64},         //motion_iir_curve_ratio_4
    {0xf8cb1054,    16,      22,        0,        0,      0x1,       64},         //motion_iir_curve_ratio_3
    {0xf8cb1054,    8,       14,        0,        0,      0x1,       64},         //motion_iir_curve_ratio_2
    {0xf8cb1054,    0,       6,         0,        0,      0x1,       48},         //motion_iir_curve_ratio_1
    {0xf8cb1050,    24,      30,        0,        0,      0x1,       32},         //motion_iir_curve_ratio_0
    /*DIEHISMODE*/
    {0xf8cb1058,    21,      21,        0,        0,      0x1,        0},         //his_motion_info_write_mode
    {0xf8cb1058,    20,      20,        0,        0,      0x1,        0},         //his_motion_write_mode
    {0xf8cb1058,    19,      19,        0,        0,      0x1,        1},         //his_motion_using_mode
    {0xf8cb1058,    18,      18,        0,        0,      0x1,        1},         //his_motion_en
    {0xf8cb1058,    17,      17,        0,        0,      0x1,        0},         //pre_info_en
    {0xf8cb1058,    16,      16,        0,        0,      0x1,        1},         //ppre_info_en
    {0xf8cb1058,    12,      13,        0,        0,      0x1,        0},         //rec_mode_frm_motion_step_1
    {0xf8cb1058,    8,       9,         0,        0,      0x1,        0},         //rec_mode_frm_motion_step_0
    {0xf8cb1058,    4,       6,         0,        0,      0x1,        1},         //rec_mode_fld_motion_step_1
    {0xf8cb1058,    0,       2,         0,        0,      0x1,        1},         //rec_mode_fld_motion_step_0
    /*DIEMORFLT*/
    {0xf8cb105c,    26,      26,        0,        0,      0x1,        0},         //med_blend_en
    {0xf8cb105c,    25,      25,        0,        0,      0x1,        0},         //reserved_1
    {0xf8cb105c,    24,      24,        0,        0,      0x1,        1},         //mor_flt_en
    {0xf8cb105c,    10,      23,        0,        0,      0x1,        0},         //reserved_2
    {0xf8cb105c,    8,       9,         0,        0,      0x1,        0},         //mor_flt_size
    {0xf8cb105c,    0,       7,         0,        0,      0x1,        0},         //mor_flt_thd
    /*DIECOMBCHK*/
    {0xf8cb1064,    16,      16,        0,        0,      0x1,        0},         //comb_chk_en
    {0xf8cb1064,    8,       12,        0,        0,      0x1,       30},         //comb_chk_md_thd
    {0xf8cb1064,    0,       6,         0,        0,      0x1,       64},         //comb_chk_edge_thd
    {0xf8cb1060,    24,      31,        0,        0,      0x1,      160},         //comb_chk_upper_limit
    {0xf8cb1060,    16,      23,        0,        0,      0x1,       10},         //comb_chk_lower_limit
    {0xf8cb1060,    8,       15,        0,        0,      0x1,       15},         //comb_chk_min_vthd
    {0xf8cb1060,    0,       7,         0,        0,      0x1,      255},         //comb_chk_min_hthd
    /*DIEFRMMTNSMTH*/
    {0xf8cb106c,    24,      30,        0,        0,      0x1,       64},         //frame_motion_smooth_ratio_max
    {0xf8cb106c,    16,      22,        0,        0,      0x1,        0},         //frame_motion_smooth_ratio_min
    {0xf8cb106c,    8,       15,        0,        0,      0x1,      255},         //frame_motion_smooth_thd5
    {0xf8cb106c,    0,       7,         0,        0,      0x1,      255},         //frame_motion_smooth_thd4
    {0xf8cb1068,    24,      31,        0,        0,      0x1,      255},         //frame_motion_smooth_thd3
    {0xf8cb1068,    16,      23,        0,        0,      0x1,      255},         //frame_motion_smooth_thd2
    {0xf8cb1068,    8,       15,        0,        0,      0x1,       72},         //frame_motion_smooth_thd1
    {0xf8cb1068,    0,       7,         0,        0,      0x1,        8},         //frame_motion_smooth_thd0
    /*DIEFRMMTNSMTHSLP*/
    {0xf8cb1070,    18,      23,        0,        0,      0x1,        0},         //frame_motion_smooth_slope3
    {0xf8cb1070,    12,      17,        0,        0,      0x1,        0},         //frame_motion_smooth_slope2
    {0xf8cb1070,    6,       11,        0,        0,      0x1,        0},         //frame_motion_smooth_slope1
    {0xf8cb1070,    0,       5,         0,        0,      0x1,        8},         //frame_motion_smooth_slope0
    /*DIEFRMMTNSMTHRAT*/
    {0xf8cb1074,    24,      30,        0,        0,      0x1,       64},         //frame_motion_smooth_ratio4
    {0xf8cb1074,    16,      22,        0,        0,      0x1,       64},         //frame_motion_smooth_ratio3
    {0xf8cb1074,    8,       14,        0,        0,      0x1,       64},         //frame_motion_smooth_ratio2
    {0xf8cb1074,    0,       6,         0,        0,      0x1,       64},         //frame_motion_smooth_ratio1
    {0xf8cb1070,    24,      30,        0,        0,      0x1,        0},         //frame_motion_smooth_ratio0
    /*DIEFRMFLDBLEND*/
    {0xf8cb107c,    24,      30,        0,        0,      0x1,       64},         //frame_field_blend_ratio_max
    {0xf8cb107c,    16,      22,        0,        0,      0x1,        0},         //frame_field_blend_ratio_min
    {0xf8cb107c,    8,       15,        0,        0,      0x1,      255},         //frame_field_blend_thd5
    {0xf8cb107c,    0,       7,         0,        0,      0x1,      255},         //frame_field_blend_thd4
    {0xf8cb1078,    24,      31,        0,        0,      0x1,      255},         //frame_field_blend_thd3
    {0xf8cb1078,    16,      23,        0,        0,      0x1,      255},         //frame_field_blend_thd2
    {0xf8cb1078,    8,       15,        0,        0,      0x1,       72},         //frame_field_blend_thd1
    {0xf8cb1078,    0,       7,         0,        0,      0x1,        8},         //frame_field_blend_thd0
    /*DIEFRMFLDBLENDSLP*/
    {0xf8cb1080,    18,      23,        0,        0,      0x1,        0},         //frame_field_blend_slope3
    {0xf8cb1080,    12,      17,        0,        0,      0x1,        0},         //frame_field_blend_slope2
    {0xf8cb1080,    6,       11,        0,        0,      0x1,        0},         //frame_field_blend_slope1
    {0xf8cb1080,    0,       5,         0,        0,      0x1,        8},         //frame_field_blend_slope0
    /*DIEFRMFLDBLENDRAT*/
    {0xf8cb1084,    24,      30,        0,        0,      0x1,       64},         //frame_field_blend_ratio4
    {0xf8cb1084,    16,      22,        0,        0,      0x1,       64},         //frame_field_blend_ratio3
    {0xf8cb1084,    8,       14,        0,        0,      0x1,       64},         //frame_field_blend_ratio2
    {0xf8cb1084,    0,       6,         0,        0,      0x1,       64},         //frame_field_blend_ratio1
    {0xf8cb1080,    24,      30,        0,        0,      0x1,        0},         //frame_field_blend_ratio0
    /*DIEMTNADJ*/
    {0xf8cb1088,    16,      23,        0,        0,      0x1,      128},         //motion_adjust_gain_chr
    {0xf8cb1088,    8,       13,        0,        0,      0x1,        0},         //motion_adjust_coring
    {0xf8cb1088,    0,       7,         0,        0,      0x1,      128},         //motion_adjust_gain
    /*DIEGLBMTNTHD*/
    {0xf8cb1090,   16,      25,         0,        0,      0x1,       16},         //motion_thd3
    {0xf8cb1090,    0,       9,         0,        0,      0x1,       16},         //motion_thd2
    {0xf8cb108c,   16,      25,         0,        0,      0x1,       16},         //motion_thd1
    {0xf8cb108c,    0,       9,         0,        0,      0x1,       16},         //motion_thd0
    /*DIEFRMFLDBLENDRAT*/
    {0xf8cb1094,    30,      30,        0,        0,      0x1,        1},         //chroma_ccr_en
    {0xf8cb1094,    20,      29,        0,        0,      0x1,        4},         //chroma_ma_offset
    {0xf8cb1094,    10,      19,        0,        0,      0x1,        8},         //no_ccr_detect_max
    {0xf8cb1094,     0,       9,        0,        0,      0x1,        4},         //no_ccr_detect_thd
    {0xf8cb1098,    23,      31,        0,        0,      0x1,        8},         //similar_max
    {0xf8cb1098,    14,      22,        0,        0,      0x1,        4},         //similar_thd
    {0xf8cb1098,    10,      13,        0,        0,      0x1,        6},         //no_ccr_detect_blend
    {0xf8cb1098,     0,       9,        0,        0,      0x1,      192},         //max_xchroma

    /****************************************FMD****************************************/
    /*FMD_CTRL*/
    {0xf8cb1150,    30,      30,        0,        0,      0x2,        0},         //edge_smooth_en
    {0xf8cb1150,    20,      27,        0,        0,      0x2,       32},         //edge_smooth_ratio
    {0xf8cb1150,     0,       2,        0,        0,      0x2,        6},         //bitsmov2r
    /*HISTBIN_THD*/
    {0xf8cb1144,    0,        7,        0,        0,      0x2,        8},         //hist_thd0
    {0xf8cb1144,    8,       15,        0,        0,      0x2,       16},         //hist_thd1
    {0xf8cb1144,    16,      23,        0,        0,      0x2,       32},         //hist_thd2
    {0xf8cb1144,    24,      31,        0,        0,      0x2,       32},         //hist_thd3
    {0xf8cb1158,    8,       11,        0,        0,      0x2,        0},         //mon_start_idx
    {0xf8cb1158,    0,        7,        0,        0,      0x2,        0},         //mon_tkr_thr
    {0xf8cb1154,    8,       15,        0,        0,      0x2,       30},         //diff_movblk_thd
    {0xf8cb1154,    0,       7,         0,        0,      0x2,        0},         //stillblk_thd
    /*PCC_THD*/
    {0xf8cb1148,    16,      23,        0,        0,      0x2,       20},         //mov_coring_norm
    {0xf8cb1148,    8,       15,        0,        0,      0x2,       50},         //mov_coring_tkr
    {0xf8cb1148,    0,        7,        0,        0,      0x2,       20},         //mov_coring_blk
    {0xf8cb1160,    16,      23,        0,        0,      0x2,       16},         //coring_blk
    {0xf8cb1160,    8,       15,        0,        0,      0x2,       16},         //coring_norm
    {0xf8cb1160,    0,        7,        0,        0,      0x2,       90},         //coring_tkr
    {0xf8cb1164,    0,        7,        0,        0,      0x2,       16},         //pcc_hthd
    {0xf8cb1168,    24,      31,        0,        0,      0x2,      128},         //pcc_vthd3
    {0xf8cb1168,    16,      23,        0,        0,      0x2,       64},         //pcc_vthd2
    {0xf8cb1168,    8,       15,        0,        0,      0x2,       32},         //pcc_vthd1
    {0xf8cb1168,    0,        7,        0,        0,      0x2,       16},         //pcc_vthd0
    /*ITDIFF_THD*/
    {0xf8cb116c,    24,      31,        0,        0,      0x2,       32},         //itdiff_vthd3
    {0xf8cb116c,    16,      23,        0,        0,      0x2,       16},         //itdiff_vthd2
    {0xf8cb116c,    8,       15,        0,        0,      0x2,        8},         //itdiff_vthd1
    {0xf8cb116c,    0,       7,         0,        0,      0x2,        4},         //itdiff_vthd0
    /*UM_THD*/
    {0xf8cb115c,    16,      23,        0,        0,      0x2,       64},         //um_thd2
    {0xf8cb115c,    8,       15,        0,        0,      0x2,       32},         //um_thd1
    {0xf8cb115c,    0,       7,         0,        0,      0x2,        8},         //um_thd0
    /*PDLASITHR*/
    {0xf8cb1150,    19,      19,        0,        0,      0x2,        0},         //lasi_mode
    {0xf8cb1170,     0,       7,        0,        0,      0x2,       20},         //lasi_coring_thd
    {0xf8cb1170,     8,      15,        0,        0,      0x2,       15},         //lasi_edge_thd
    {0xf8cb1170,    16,      23,        0,        0,      0x2,       32},         //lasi_mov_thd
    /*PDLASIMODE0COEF*/
    {0xf8cb1174,     8,      15,        0,        0,      0x2,        0},         //lasi_txt_coring
    {0xf8cb1174,     0,       7,        0,        0,      0x2,       32},         //lasi_txt_alpha
    {0xf8cb1178,    24,      31,        0,        0,      0x2,        0},         //lasi_txt_thd3
    {0xf8cb1178,    16,      23,        0,        0,      0x2,        0},         //lasi_txt_thd2
    {0xf8cb1178,     8,      15,        0,        0,      0x2,        0},         //lasi_txt_thd1
    {0xf8cb1178,     0,       7,        0,        0,      0x2,        0},         //lasi_txt_thd0
    /*PDLASIMODE0COEF*/
    {0xf8cb117c,    12,      23,        0,        0,      0x2,        0},         //region1_y_stt
    {0xf8cb117c,     0,      11,        0,        0,      0x2,        0},         //region1_y_end


    /***********************************HSHARPEN***************************************/
    /*SD Source-->HD Output*/
    /*VPSS_HSPCTRL*/
    {0xF8cb0000,     7,       7,        1,        2,      0x4,        0},         //hsp_en
    {0xf8cb2138,    31,      31,        1,        2,      0x4,        1},         //hsp_h0_en
    {0xf8cb2138,    30,      30,        1,        2,      0x4,        1},         //hsp_h1_en
    {0xf8cb2138,    29,      29,        1,        2,      0x4,        1},         //hsp_ltih_en
    {0xf8cb2138,    28,      28,        1,        2,      0x4,        1},         //hsp_ctih_en
    {0xf8cb2104,     0,       7,        1,        2,      0x4,        0},         //hsp_hf0_coring
    {0xf8cb2114,    16,      26,        1,        2,      0x4,       63},         //hsp_hf0_gainneg
    {0xf8cb2114,     0,      10,        1,        2,      0x4,       63},         //hsp_hf0_gainpos
    {0xf8cb2120,     0,       7,        1,        2,      0x4,        0},         //hsp_hf1_coring
    {0xf8cb2130,    16,      26,        1,        2,      0x4,       63},         //hsp_hf1_gainneg
    {0xf8cb2130,     0,      10,        1,        2,      0x4,       63},         //hsp_hf1_gainpos
    {0xf8cb2138,    16,      23,        1,        2,      0x4,      128},         //hsp_lti_ratio
    {0xf8cb2138,     8,      15,        1,        2,      0x4,       96},         //hsp_ldti_gain
    {0xf8cb2138,     0,       7,        1,        2,      0x4,       96},         //hsp_cdti_gain
    {0xf8cb213C,    20,      27,        1,        2,      0x4,      128},         //hsp_peak_ratio
    {0xf8cb213C,    19,      19,        1,        2,      0x4,        1},         //hsp_mk_mode right open mode
    {0xf8cb213C,     9,       9,        1,        2,      0x4,        0},         //hsp_mk_en
    /*HSHARPENBEFORESCAL*/
    {0xf8cb2100,    24,      31,        1,        2,      0x4,        0},         //hsp_hf0_tmp3
    {0xf8cb2100,    16,      23,        1,        2,      0x4,        0},         //hsp_hf0_tmp2
    {0xf8cb2100,     8,      15,        1,        2,      0x4,        0},         //hsp_hf0_tmp1
    {0xf8cb2100,     0,       7,        1,        2,      0x4,      -64},         //hsp_hf0_tmp0
    {0xf8cb2118,    31,      31,        1,        2,      0x4,        1},         //hsp_hf0_adpshoot_en
    {0xf8cb2118,    28,      30,        1,        2,      0x4,        2},         //hsp_hf0_winsize
    {0xf8cb2118,    16,      23,        1,        2,      0x4,        0},         //hsp_hf0_mixratio
    {0xf8cb2118,     8,      15,        1,        2,      0x4,      255},         //hsp_hf0_underth
    {0xf8cb2118,     0,       7,        1,        2,      0x4,      255},         //hsp_hf0_overth
    {0xf8cb211C,    24,      31,        1,        2,      0x4,        0},         //hsp_hf1_tmp3
    {0xf8cb211C,    16,      23,        1,        2,      0x4,        0},         //hsp_hf1_tmp2
    {0xf8cb211C,     8,      15,        1,        2,      0x4,      -64},         //hsp_hf1_tmp1
    {0xf8cb211C,     0,       7,        1,        2,      0x4,        0},         //hsp_hf1_tmp0
    {0xf8cb2134,    31,      31,        1,        2,      0x4,        1},         //hsp_hf1_adpshoot_en
    {0xf8cb2134,    28,      30,        1,        2,      0x4,        2},         //hsp_hf1_winsize
    {0xf8cb2134,    16,      23,        1,        2,      0x4,        0},         //hsp_hf1_mixratio
    {0xf8cb2134,     8,      15,        1,        2,      0x4,      255},         //hsp_hf1_underth
    {0xf8cb2134,     0,       7,        1,        2,      0x4,      255},         //hsp_hf1_overth
    {0xf8cb2138,    24,      26,        1,        2,      0x4,        4},         //hsp_hf_shootdiv
    {0xf8cb213C,    10,      18,        1,        2,      0x4,       50},         //hsp_glb_overth
    {0xf8cb213C,     0,       8,        1,        2,      0x4,      128},         //hsp_glb_underth

    /*SD Source-->UHD Output*/
    /*VPSS_HSPCTRL*/
    {0xF8cb0000,     7,       7,        1,        3,      0x4,        0},         //hsp_en
    {0xf8cb2138,    31,      31,        1,        3,      0x4,        1},         //hsp_h0_en
    {0xf8cb2138,    30,      30,        1,        3,      0x4,        1},         //hsp_h1_en
    {0xf8cb2138,    29,      29,        1,        3,      0x4,        1},         //hsp_ltih_en
    {0xf8cb2138,    28,      28,        1,        3,      0x4,        1},         //hsp_ctih_en
    {0xf8cb2104,     0,       7,        1,        3,      0x4,        0},         //hsp_hf0_coring
    {0xf8cb2114,    16,      26,        1,        3,      0x4,       63},         //hsp_hf0_gainneg
    {0xf8cb2114,     0,      10,        1,        3,      0x4,       63},         //hsp_hf0_gainpos
    {0xf8cb2120,     0,       7,        1,        3,      0x4,        0},         //hsp_hf1_coring
    {0xf8cb2130,    16,      26,        1,        3,      0x4,       63},         //hsp_hf1_gainneg
    {0xf8cb2130,     0,      10,        1,        3,      0x4,       63},         //hsp_hf1_gainpos
    {0xf8cb2138,    16,      23,        1,        3,      0x4,      128},         //hsp_lti_ratio
    {0xf8cb2138,     8,      15,        1,        3,      0x4,       96},         //hsp_ldti_gain
    {0xf8cb2138,     0,       7,        1,        3,      0x4,       96},         //hsp_cdti_gain
    {0xf8cb213C,    20,      27,        1,        3,      0x4,      128},         //hsp_peak_ratio
    {0xf8cb213C,    19,      19,        1,        3,      0x4,        1},         //hsp_mk_mode right open mode
    {0xf8cb213C,     9,       9,        1,        3,      0x4,        0},         //hsp_mk_en
    /*HSHARPENBEFORESCAL*/
    {0xf8cb2100,    24,      31,        1,        3,      0x4,        0},         //hsp_hf0_tmp3
    {0xf8cb2100,    16,      23,        1,        3,      0x4,        0},         //hsp_hf0_tmp2
    {0xf8cb2100,     8,      15,        1,        3,      0x4,        0},         //hsp_hf0_tmp1
    {0xf8cb2100,     0,       7,        1,        3,      0x4,      -64},         //hsp_hf0_tmp0
    {0xf8cb2118,    31,      31,        1,        3,      0x4,        1},         //hsp_hf0_adpshoot_en
    {0xf8cb2118,    28,      30,        1,        3,      0x4,        2},         //hsp_hf0_winsize
    {0xf8cb2118,    16,      23,        1,        3,      0x4,        0},         //hsp_hf0_mixratio
    {0xf8cb2118,     8,      15,        1,        3,      0x4,      255},         //hsp_hf0_underth
    {0xf8cb2118,     0,       7,        1,        3,      0x4,      255},         //hsp_hf0_overth
    {0xf8cb211C,    24,      31,        1,        3,      0x4,        0},         //hsp_hf1_tmp3
    {0xf8cb211C,    16,      23,        1,        3,      0x4,        0},         //hsp_hf1_tmp2
    {0xf8cb211C,     8,      15,        1,        3,      0x4,      -64},         //hsp_hf1_tmp1
    {0xf8cb211C,     0,       7,        1,        3,      0x4,        0},         //hsp_hf1_tmp0
    {0xf8cb2134,    31,      31,        1,        3,      0x4,        1},         //hsp_hf1_adpshoot_en
    {0xf8cb2134,    28,      30,        1,        3,      0x4,        2},         //hsp_hf1_winsize
    {0xf8cb2134,    16,      23,        1,        3,      0x4,        0},         //hsp_hf1_mixratio
    {0xf8cb2134,     8,      15,        1,        3,      0x4,      255},         //hsp_hf1_underth
    {0xf8cb2134,     0,       7,        1,        3,      0x4,      255},         //hsp_hf1_overth
    {0xf8cb2138,    24,      26,        1,        3,      0x4,        4},         //hsp_hf_shootdiv
    {0xf8cb213C,    10,      18,        1,        3,      0x4,       50},         //hsp_glb_overth
    {0xf8cb213C,     0,       8,        1,        3,      0x4,      128},         //hsp_glb_underth

    /*HD Source-->HD Output*/
    /*VPSS_HSPCTRL*/
    {0xF8cb0000,     7,       7,        2,        2,      0x4,        0},         //hsp_en
    {0xf8cb2138,    31,      31,        2,        2,      0x4,        1},         //hsp_h0_en
    {0xf8cb2138,    30,      30,        2,        2,      0x4,        1},         //hsp_h1_en
    {0xf8cb2138,    29,      29,        2,        2,      0x4,        1},         //hsp_ltih_en
    {0xf8cb2138,    28,      28,        2,        2,      0x4,        1},         //hsp_ctih_en
    {0xf8cb2104,     0,       7,        2,        2,      0x4,        0},         //hsp_hf0_coring
    {0xf8cb2114,    16,      26,        2,        2,      0x4,       63},         //hsp_hf0_gainneg
    {0xf8cb2114,     0,      10,        2,        2,      0x4,       63},         //hsp_hf0_gainpos
    {0xf8cb2120,     0,       7,        2,        2,      0x4,        0},         //hsp_hf1_coring
    {0xf8cb2130,    16,      26,        2,        2,      0x4,       63},         //hsp_hf1_gainneg
    {0xf8cb2130,     0,      10,        2,        2,      0x4,       63},         //hsp_hf1_gainpos
    {0xf8cb2138,    16,      23,        2,        2,      0x4,      128},         //hsp_lti_ratio
    {0xf8cb2138,     8,      15,        2,        2,      0x4,       96},         //hsp_ldti_gain
    {0xf8cb2138,     0,       7,        2,        2,      0x4,       96},         //hsp_cdti_gain
    {0xf8cb213C,    20,      27,        2,        2,      0x4,      128},         //hsp_peak_ratio
    {0xf8cb213C,    19,      19,        2,        2,      0x4,        1},         //hsp_mk_mode right open mode
    {0xf8cb213C,     9,       9,        2,        2,      0x4,        0},         //hsp_mk_en
    /*HSHARPENBEFORESCAL*/
    {0xf8cb2100,    24,      31,        2,        2,      0x4,        0},         //hsp_hf0_tmp3
    {0xf8cb2100,    16,      23,        2,        2,      0x4,        0},         //hsp_hf0_tmp2
    {0xf8cb2100,     8,      15,        2,        2,      0x4,        0},         //hsp_hf0_tmp1
    {0xf8cb2100,     0,       7,        2,        2,      0x4,      -64},         //hsp_hf0_tmp0
    {0xf8cb2118,    31,      31,        2,        2,      0x4,        1},         //hsp_hf0_adpshoot_en
    {0xf8cb2118,    28,      30,        2,        2,      0x4,        2},         //hsp_hf0_winsize
    {0xf8cb2118,    16,      23,        2,        2,      0x4,        0},         //hsp_hf0_mixratio
    {0xf8cb2118,     8,      15,        2,        2,      0x4,      255},         //hsp_hf0_underth
    {0xf8cb2118,     0,       7,        2,        2,      0x4,      255},         //hsp_hf0_overth
    {0xf8cb211C,    24,      31,        2,        2,      0x4,        0},         //hsp_hf1_tmp3
    {0xf8cb211C,    16,      23,        2,        2,      0x4,        0},         //hsp_hf1_tmp2
    {0xf8cb211C,     8,      15,        2,        2,      0x4,      -64},         //hsp_hf1_tmp1
    {0xf8cb211C,     0,       7,        2,        2,      0x4,        0},         //hsp_hf1_tmp0
    {0xf8cb2134,    31,      31,        2,        2,      0x4,        1},         //hsp_hf1_adpshoot_en
    {0xf8cb2134,    28,      30,        2,        2,      0x4,        2},         //hsp_hf1_winsize
    {0xf8cb2134,    16,      23,        2,        2,      0x4,        0},         //hsp_hf1_mixratio
    {0xf8cb2134,     8,      15,        2,        2,      0x4,      255},         //hsp_hf1_underth
    {0xf8cb2134,     0,       7,        2,        2,      0x4,      255},         //hsp_hf1_overth
    {0xf8cb2138,    24,      26,        2,        2,      0x4,        4},         //hsp_hf_shootdiv
    {0xf8cb213C,    10,      18,        2,        2,      0x4,       50},         //hsp_glb_overth
    {0xf8cb213C,     0,       8,        2,        2,      0x4,      128},         //hsp_glb_underth

    /*HD Source-->UHD Output*/
    /*VPSS_HSPCTRL*/
    {0xF8cb0000,     7,       7,        2,        3,      0x4,        0},         //hsp_en
    {0xf8cb2138,    31,      31,        2,        3,      0x4,        1},         //hsp_h0_en
    {0xf8cb2138,    30,      30,        2,        3,      0x4,        1},         //hsp_h1_en
    {0xf8cb2138,    29,      29,        2,        3,      0x4,        1},         //hsp_ltih_en
    {0xf8cb2138,    28,      28,        2,        3,      0x4,        1},         //hsp_ctih_en
    {0xf8cb2104,     0,       7,        2,        3,      0x4,        0},         //hsp_hf0_coring
    {0xf8cb2114,    16,      26,        2,        3,      0x4,       63},         //hsp_hf0_gainneg
    {0xf8cb2114,     0,      10,        2,        3,      0x4,       63},         //hsp_hf0_gainpos
    {0xf8cb2120,     0,       7,        2,        3,      0x4,        0},         //hsp_hf1_coring
    {0xf8cb2130,    16,      26,        2,        3,      0x4,       63},         //hsp_hf1_gainneg
    {0xf8cb2130,     0,      10,        2,        3,      0x4,       63},         //hsp_hf1_gainpos
    {0xf8cb2138,    16,      23,        2,        3,      0x4,      128},         //hsp_lti_ratio
    {0xf8cb2138,     8,      15,        2,        3,      0x4,       96},         //hsp_ldti_gain
    {0xf8cb2138,     0,       7,        2,        3,      0x4,       96},         //hsp_cdti_gain
    {0xf8cb213C,    20,      27,        2,        3,      0x4,      128},         //hsp_peak_ratio
    {0xf8cb213C,    19,      19,        2,        3,      0x4,        1},         //hsp_mk_mode right open mode
    {0xf8cb213C,     9,       9,        2,        3,      0x4,        0},         //hsp_mk_en
    /*HSHARPENBEFORESCAL*/
    {0xf8cb2100,    24,      31,        2,        3,      0x4,        0},         //hsp_hf0_tmp3
    {0xf8cb2100,    16,      23,        2,        3,      0x4,        0},         //hsp_hf0_tmp2
    {0xf8cb2100,     8,      15,        2,        3,      0x4,        0},         //hsp_hf0_tmp1
    {0xf8cb2100,     0,       7,        2,        3,      0x4,      -64},         //hsp_hf0_tmp0
    {0xf8cb2118,    31,      31,        2,        3,      0x4,        1},         //hsp_hf0_adpshoot_en
    {0xf8cb2118,    28,      30,        2,        3,      0x4,        2},         //hsp_hf0_winsize
    {0xf8cb2118,    16,      23,        2,        3,      0x4,        0},         //hsp_hf0_mixratio
    {0xf8cb2118,     8,      15,        2,        3,      0x4,      255},         //hsp_hf0_underth
    {0xf8cb2118,     0,       7,        2,        3,      0x4,      255},         //hsp_hf0_overth
    {0xf8cb211C,    24,      31,        2,        3,      0x4,        0},         //hsp_hf1_tmp3
    {0xf8cb211C,    16,      23,        2,        3,      0x4,        0},         //hsp_hf1_tmp2
    {0xf8cb211C,     8,      15,        2,        3,      0x4,      -64},         //hsp_hf1_tmp1
    {0xf8cb211C,     0,       7,        2,        3,      0x4,        0},         //hsp_hf1_tmp0
    {0xf8cb2134,    31,      31,        2,        3,      0x4,        1},         //hsp_hf1_adpshoot_en
    {0xf8cb2134,    28,      30,        2,        3,      0x4,        2},         //hsp_hf1_winsize
    {0xf8cb2134,    16,      23,        2,        3,      0x4,        0},         //hsp_hf1_mixratio
    {0xf8cb2134,     8,      15,        2,        3,      0x4,      255},         //hsp_hf1_underth
    {0xf8cb2134,     0,       7,        2,        3,      0x4,      255},         //hsp_hf1_overth
    {0xf8cb2138,    24,      26,        2,        3,      0x4,        4},         //hsp_hf_shootdiv
    {0xf8cb213C,    10,      18,        2,        3,      0x4,       50},         //hsp_glb_overth
    {0xf8cb213C,     0,       8,        2,        3,      0x4,      128},         //hsp_glb_underth

    /*UHD Source-->HD Output*/
    /*VPSS_HSPCTRL*/
    {0xF8cb0000,     7,       7,        3,        2,      0x4,        0},         //hsp_en
    {0xf8cb2138,    31,      31,        3,        2,      0x4,        1},         //hsp_h0_en
    {0xf8cb2138,    30,      30,        3,        2,      0x4,        1},         //hsp_h1_en
    {0xf8cb2138,    29,      29,        3,        2,      0x4,        1},         //hsp_ltih_en
    {0xf8cb2138,    28,      28,        3,        2,      0x4,        1},         //hsp_ctih_en
    {0xf8cb2104,     0,       7,        3,        2,      0x4,        0},         //hsp_hf0_coring
    {0xf8cb2114,    16,      26,        3,        2,      0x4,       63},         //hsp_hf0_gainneg
    {0xf8cb2114,     0,      10,        3,        2,      0x4,       63},         //hsp_hf0_gainpos
    {0xf8cb2120,     0,       7,        3,        2,      0x4,        0},         //hsp_hf1_coring
    {0xf8cb2130,    16,      26,        3,        2,      0x4,       63},         //hsp_hf1_gainneg
    {0xf8cb2130,     0,      10,        3,        2,      0x4,       63},         //hsp_hf1_gainpos
    {0xf8cb2138,    16,      23,        3,        2,      0x4,      128},         //hsp_lti_ratio
    {0xf8cb2138,     8,      15,        3,        2,      0x4,       96},         //hsp_ldti_gain
    {0xf8cb2138,     0,       7,        3,        2,      0x4,       96},         //hsp_cdti_gain
    {0xf8cb213C,    20,      27,        3,        2,      0x4,      128},         //hsp_peak_ratio
    {0xf8cb213C,    19,      19,        3,        2,      0x4,        1},         //hsp_mk_mode right open mode
    {0xf8cb213C,     9,       9,        3,        2,      0x4,        0},         //hsp_mk_en
    /*HSHARPENBEFORESCAL*/
    {0xf8cb2100,    24,      31,        3,        2,      0x4,        0},         //hsp_hf0_tmp3
    {0xf8cb2100,    16,      23,        3,        2,      0x4,        0},         //hsp_hf0_tmp2
    {0xf8cb2100,     8,      15,        3,        2,      0x4,        0},         //hsp_hf0_tmp1
    {0xf8cb2100,     0,       7,        3,        2,      0x4,      -64},         //hsp_hf0_tmp0
    {0xf8cb2118,    31,      31,        3,        2,      0x4,        1},         //hsp_hf0_adpshoot_en
    {0xf8cb2118,    28,      30,        3,        2,      0x4,        2},         //hsp_hf0_winsize
    {0xf8cb2118,    16,      23,        3,        2,      0x4,        0},         //hsp_hf0_mixratio
    {0xf8cb2118,     8,      15,        3,        2,      0x4,      255},         //hsp_hf0_underth
    {0xf8cb2118,     0,       7,        3,        2,      0x4,      255},         //hsp_hf0_overth
    {0xf8cb211C,    24,      31,        3,        2,      0x4,        0},         //hsp_hf1_tmp3
    {0xf8cb211C,    16,      23,        3,        2,      0x4,        0},         //hsp_hf1_tmp2
    {0xf8cb211C,     8,      15,        3,        2,      0x4,      -64},         //hsp_hf1_tmp1
    {0xf8cb211C,     0,       7,        3,        2,      0x4,        0},         //hsp_hf1_tmp0
    {0xf8cb2134,    31,      31,        3,        2,      0x4,        1},         //hsp_hf1_adpshoot_en
    {0xf8cb2134,    28,      30,        3,        2,      0x4,        2},         //hsp_hf1_winsize
    {0xf8cb2134,    16,      23,        3,        2,      0x4,        0},         //hsp_hf1_mixratio
    {0xf8cb2134,     8,      15,        3,        2,      0x4,      255},         //hsp_hf1_underth
    {0xf8cb2134,     0,       7,        3,        2,      0x4,      255},         //hsp_hf1_overth
    {0xf8cb2138,    24,      26,        3,        2,      0x4,        4},         //hsp_hf_shootdiv
    {0xf8cb213C,    10,      18,        3,        2,      0x4,       50},         //hsp_glb_overth
    {0xf8cb213C,     0,       8,        3,        2,      0x4,      128},         //hsp_glb_underth

    /*UHD Source-->UHD Output*/
    /*VPSS_HSPCTRL*/
    {0xF8cb0000,     7,       7,        3,        3,      0x4,        0},         //hsp_en
    {0xf8cb2138,    31,      31,        3,        3,      0x4,        1},         //hsp_h0_en
    {0xf8cb2138,    30,      30,        3,        3,      0x4,        1},         //hsp_h1_en
    {0xf8cb2138,    29,      29,        3,        3,      0x4,        1},         //hsp_ltih_en
    {0xf8cb2138,    28,      28,        3,        3,      0x4,        1},         //hsp_ctih_en
    {0xf8cb2104,     0,       7,        3,        3,      0x4,        0},         //hsp_hf0_coring
    {0xf8cb2114,    16,      26,        3,        3,      0x4,       63},         //hsp_hf0_gainneg
    {0xf8cb2114,     0,      10,        3,        3,      0x4,       63},         //hsp_hf0_gainpos
    {0xf8cb2120,     0,       7,        3,        3,      0x4,        0},         //hsp_hf1_coring
    {0xf8cb2130,    16,      26,        3,        3,      0x4,       63},         //hsp_hf1_gainneg
    {0xf8cb2130,     0,      10,        3,        3,      0x4,       63},         //hsp_hf1_gainpos
    {0xf8cb2138,    16,      23,        3,        3,      0x4,      128},         //hsp_lti_ratio
    {0xf8cb2138,     8,      15,        3,        3,      0x4,       96},         //hsp_ldti_gain
    {0xf8cb2138,     0,       7,        3,        3,      0x4,       96},         //hsp_cdti_gain
    {0xf8cb213C,    20,      27,        3,        3,      0x4,      128},         //hsp_peak_ratio
    {0xf8cb213C,    19,      19,        3,        3,      0x4,        1},         //hsp_mk_mode right open mode
    {0xf8cb213C,     9,       9,        3,        3,      0x4,        0},         //hsp_mk_en
    /*HSHARPENBEFORESCAL*/
    {0xf8cb2100,    24,      31,        3,        3,      0x4,        0},         //hsp_hf0_tmp3
    {0xf8cb2100,    16,      23,        3,        3,      0x4,        0},         //hsp_hf0_tmp2
    {0xf8cb2100,     8,      15,        3,        3,      0x4,        0},         //hsp_hf0_tmp1
    {0xf8cb2100,     0,       7,        3,        3,      0x4,      -64},         //hsp_hf0_tmp0
    {0xf8cb2118,    31,      31,        3,        3,      0x4,        1},         //hsp_hf0_adpshoot_en
    {0xf8cb2118,    28,      30,        3,        3,      0x4,        2},         //hsp_hf0_winsize
    {0xf8cb2118,    16,      23,        3,        3,      0x4,        0},         //hsp_hf0_mixratio
    {0xf8cb2118,     8,      15,        3,        3,      0x4,      255},         //hsp_hf0_underth
    {0xf8cb2118,     0,       7,        3,        3,      0x4,      255},         //hsp_hf0_overth
    {0xf8cb211C,    24,      31,        3,        3,      0x4,        0},         //hsp_hf1_tmp3
    {0xf8cb211C,    16,      23,        3,        3,      0x4,        0},         //hsp_hf1_tmp2
    {0xf8cb211C,     8,      15,        3,        3,      0x4,      -64},         //hsp_hf1_tmp1
    {0xf8cb211C,     0,       7,        3,        3,      0x4,        0},         //hsp_hf1_tmp0
    {0xf8cb2134,    31,      31,        3,        3,      0x4,        1},         //hsp_hf1_adpshoot_en
    {0xf8cb2134,    28,      30,        3,        3,      0x4,        2},         //hsp_hf1_winsize
    {0xf8cb2134,    16,      23,        3,        3,      0x4,        0},         //hsp_hf1_mixratio
    {0xf8cb2134,     8,      15,        3,        3,      0x4,      255},         //hsp_hf1_underth
    {0xf8cb2134,     0,       7,        3,        3,      0x4,      255},         //hsp_hf1_overth
    {0xf8cb2138,    24,      26,        3,        3,      0x4,        4},         //hsp_hf_shootdiv
    {0xf8cb213C,    10,      18,        3,        3,      0x4,       50},         //hsp_glb_overth
    {0xf8cb213C,     0,       8,        3,        3,      0x4,      128},         //hsp_glb_underth
    /*************************************HD_DB*****************************************/
    /*DB_CTRL*/
    {0xf8cb0000,    10,      10,        0,        0,    0x200,        1},         //db_en
    {0xf8cb2500,    10,      10,        0,        0,    0x200,        0},         //db_demo_en
    {0xf8cb2500,    11,      11,        0,        0,    0x200,        0},         //db_demo_mode
    {0xf8cb2500,    15,      15,        0,        0,    0x200,        1},         //det_vy_en
    {0xf8cb2500,    14,      14,        0,        0,    0x200,        1},         //det_hc_en
    {0xf8cb2500,    13,      13,        0,        0,    0x200,        1},         //det_hy_en
    {0xf8cb2500,    12,      12,        0,        0,    0x200,        1},         //det_size_en
    {0xf8cb2500,     7,       7,        0,        0,    0x200,        1},         //db_chr_hor_mid_grey_en
    {0xf8cb2500,     6,       6,        0,        0,    0x200,        1},         //db_chr_hor_en
    {0xf8cb2500,     5,       5,        0,        0,    0x200,        0},         //db_chr_ver_en
    {0xf8cb2500,     4,       4,        0,        0,    0x200,        1},         //db_cr_en
    {0xf8cb2500,     3,       3,        0,        0,    0x200,        1},         //db_cb_en
    {0xf8cb2500,     2,       2,        0,        0,    0x200,        1},         //db_lum_hor_en
    {0xf8cb2500,     1,       1,        0,        0,    0x200,        1},         //db_lum_ver_en
    {0xf8cb2500,     0,       0,        0,        0,    0x200,        1},         //db_lum_en
    /*DB_DIR*/
    {0xf8cb2504,     8,      12,        0,        0,    0x200,        2},         //ctrst_thresh
    {0xf8cb2504,     0,       4,        0,        0,    0x200,       14},         //grad_sub_ratio
    /*DB_BLK*/
    {0xf8cb2508,    24,      30,        0,        0,    0x200,        0},         //chr_v_blk_size
    {0xf8cb2508,    16,      22,        0,        0,    0x200,        8},         //chr_h_blk_size
    {0xf8cb2508,     8,      14,        0,        0,    0x200,        8},         //lum_v_blk_size
    {0xf8cb2508,     0,       6,        0,        0,    0x200,        8},         //lum_h_blk_size
    /*DB_RATIO*/
    {0xf8cb250c,    20,      21,        0,        0,    0x200,        2},         //lum_hor_txt_win_size
    {0xf8cb250c,    16,      19,        0,        0,    0x200,        8},         //glabal_db_strenth_chr
    {0xf8cb250c,    12,      15,        0,        0,    0x200,        8},         //glabal_db_strenth_lum
    {0xf8cb250c,    10,      11,        0,        0,    0x200,        1},         //chr_hor_filter_sel
    {0xf8cb250c,     8,       9,        0,        0,    0x200,        1},         //lum_hor_filter_sel
    {0xf8cb250c,     6,       7,        0,        0,    0x200,        0},         //chr_ver_scale_ratio
    {0xf8cb250c,     4,       5,        0,        0,    0x200,        1},         //chr_hor_scale_ratio
    {0xf8cb250c,     2,       3,        0,        0,    0x200,        3},         //lum_ver_scale_ratio
    {0xf8cb250c,     0,       1,        0,        0,    0x200,        3},         //lum_hor_scale_ratio
    /*DB_LHHF*/
    {0xf8cb2510,    28,      31,        0,        0,    0x200,        2},         //lum_hor_hf_var_gain2
    {0xf8cb2510,    24,      27,        0,        0,    0x200,        1},         //lum_hor_hf_var_gain1
    {0xf8cb2510,    21,      23,        0,        0,    0x200,        7},         //lum_hor_bord_adj_gain
    {0xf8cb2510,    16,      20,        0,        0,    0x200,        0},         //lum_hor_hf_var_core
    {0xf8cb2510,    12,      15,        0,        0,    0x200,        8},         //lum_hor_hf_diff_gain2
    {0xf8cb2510,     8,      11,        0,        0,    0x200,        0},         //lum_hor_hf_diff_gain1
    {0xf8cb2510,     0,       4,        0,        0,    0x200,        0},         //lum_hor_hf_diff_core
    /*DB_LVHF*/
    {0xf8cb2514,    24,      28,        0,        0,    0x200,       12},         //lum_ver_adj_gain
    {0xf8cb2514,    16,      20,        0,        0,    0x200,        4},         //lum_hor_adj_gain
    {0xf8cb2514,    12,      15,        0,        0,    0x200,        8},         //lum_ver_hf_var_gain2
    {0xf8cb2514,     8,      11,        0,        0,    0x200,        0},         //lum_ver_hf_var_gain1
    {0xf8cb2514,     5,       7,        0,        0,    0x200,        4},         //lum_ver_bord_adj_gain
    {0xf8cb2514,     0,       4,        0,        0,    0x200,        0},         //lum_ver_hf_var_core
    /*DB_CTRS*/
    {0xf8cb2518,    20,      23,        0,        0,    0x200,        3},         //max_chr_hor_db_dist
    {0xf8cb2518,    16,      19,        0,        0,    0x200,        4},         //max_lum_hor_db_dist
    {0xf8cb2518,    12,      15,        0,        0,    0x200,        8},         //ctrst_adj_gain2
    {0xf8cb2518,     8,      11,        0,        0,    0x200,        0},         //ctrst_adj_gain1
    {0xf8cb2518,     5,       6,        0,        0,    0x200,        2},         //dir_smooth_mode
    {0xf8cb2518,     0,       4,        0,        0,    0x200,        0},         //ctrst_adj_core
    /*DB_LHD_LUT*/
    {0xf8cb2520,    28,      31,        0,        0,    0x200,        0},         //lum_hor_delta_lut_p15
    {0xf8cb2520,    24,      27,        0,        0,    0x200,        0},         //lum_hor_delta_lut_p14
    {0xf8cb2520,    20,      23,        0,        0,    0x200,        2},         //lum_hor_delta_lut_p13
    {0xf8cb2520,    16,      19,        0,        0,    0x200,        4},         //lum_hor_delta_lut_p12
    {0xf8cb2520,    12,      15,        0,        0,    0x200,        6},         //lum_hor_delta_lut_p11
    {0xf8cb2520,     8,      11,        0,        0,    0x200,        8},         //lum_hor_delta_lut_p10
    {0xf8cb2520,     4,       7,        0,        0,    0x200,        8},         //lum_hor_delta_lut_p9
    {0xf8cb2520,     0,       3,        0,        0,    0x200,        8},         //lum_hor_delta_lut_p8
    {0xf8cb251c,    28,      31,        0,        0,    0x200,        8},         //lum_hor_delta_lut_p7
    {0xf8cb251c,    24,      27,        0,        0,    0x200,        8},         //lum_hor_delta_lut_p6
    {0xf8cb251c,    20,      23,        0,        0,    0x200,        8},         //lum_hor_delta_lut_p5
    {0xf8cb251c,    16,      19,        0,        0,    0x200,        8},         //lum_hor_delta_lut_p4
    {0xf8cb251c,    12,      15,        0,        0,    0x200,        8},         //lum_hor_delta_lut_p3
    {0xf8cb251c,     8,      11,        0,        0,    0x200,        8},         //lum_hor_delta_lut_p2
    {0xf8cb251c,     4,       7,        0,        0,    0x200,        8},         //lum_hor_delta_lut_p1
    {0xf8cb251c,     0,       3,        0,        0,    0x200,        4},         //lum_hor_delta_lut_p0
    /*DB_LVD_LUT*/
    {0xf8cb2528,    28,      31,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p15
    {0xf8cb2528,    24,      27,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p14
    {0xf8cb2528,    20,      23,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p13
    {0xf8cb2528,    16,      19,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p12
    {0xf8cb2528,    12,      15,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p11
    {0xf8cb2528,     8,      11,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p10
    {0xf8cb2528,     4,       7,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p9
    {0xf8cb2528,     0,       3,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p8
    {0xf8cb2524,    28,      31,        0,        0,    0x200,        2},         //lum_ver_delta_lut_p7
    {0xf8cb2524,    24,      27,        0,        0,    0x200,        4},         //lum_ver_delta_lut_p6
    {0xf8cb2524,    20,      23,        0,        0,    0x200,        6},         //lum_ver_delta_lut_p5
    {0xf8cb2524,    16,      19,        0,        0,    0x200,        8},         //lum_ver_delta_lut_p4
    {0xf8cb2524,    12,      15,        0,        0,    0x200,        8},         //lum_ver_delta_lut_p3
    {0xf8cb2524,     8,      11,        0,        0,    0x200,        8},         //lum_ver_delta_lut_p2
    {0xf8cb2524,     4,       7,        0,        0,    0x200,        8},         //lum_ver_delta_lut_p1
    {0xf8cb2524,     0,       3,        0,        0,    0x200,        6},         //lum_ver_delta_lut_p0
    /*DB_CHD_LUT*/
    {0xf8cb2530,    28,      31,        0,        0,    0x200,        0},         //chr_hor_delta_lut_p15
    {0xf8cb2530,    24,      27,        0,        0,    0x200,        0},         //chr_hor_delta_lut_p14
    {0xf8cb2530,    20,      23,        0,        0,    0x200,        0},         //chr_hor_delta_lut_p13
    {0xf8cb2530,    16,      19,        0,        0,    0x200,        0},         //chr_hor_delta_lut_p12
    {0xf8cb2530,    12,      15,        0,        0,    0x200,        0},         //chr_hor_delta_lut_p11
    {0xf8cb2530,     8,      11,        0,        0,    0x200,        0},         //chr_hor_delta_lut_p10
    {0xf8cb2530,     4,       7,        0,        0,    0x200,        0},         //chr_hor_delta_lut_p9
    {0xf8cb2530,     0,       3,        0,        0,    0x200,        0},         //chr_hor_delta_lut_p8
    {0xf8cb252c,    28,      31,        0,        0,    0x200,        0},         //chr_hor_delta_lut_p7
    {0xf8cb252c,    24,      27,        0,        0,    0x200,        0},         //chr_hor_delta_lut_p6
    {0xf8cb252c,    20,      23,        0,        0,    0x200,        0},         //chr_hor_delta_lut_p5
    {0xf8cb252c,    16,      19,        0,        0,    0x200,        0},         //chr_hor_delta_lut_p4
    {0xf8cb252c,    12,      15,        0,        0,    0x200,        4},         //chr_hor_delta_lut_p3
    {0xf8cb252c,     8,      11,        0,        0,    0x200,        8},         //chr_hor_delta_lut_p2
    {0xf8cb252c,     4,       7,        0,        0,    0x200,        8},         //chr_hor_delta_lut_p1
    {0xf8cb252c,     0,       3,        0,        0,    0x200,        8},         //chr_hor_delta_lut_p0
    /*DB_LHSF_LUT*/
    {0xf8cb2540,     3,       5,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p11
    {0xf8cb2540,     0,       2,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p10
    {0xf8cb2534,    27,      29,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p9
    {0xf8cb2534,    24,      26,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p8
    {0xf8cb2534,    21,      23,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p7
    {0xf8cb2534,    18,      20,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p6
    {0xf8cb2534,    15,      17,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p5
    {0xf8cb2534,    12,      14,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p4
    {0xf8cb2534,     9,      11,        0,        0,    0x200,        3},         //lum_h_str_fade_lut_p3
    {0xf8cb2534,     6,       8,        0,        0,    0x200,        3},         //lum_h_str_fade_lut_p2
    {0xf8cb2534,     3,       5,        0,        0,    0x200,        4},         //lum_h_str_fade_lut_p1
    {0xf8cb2534,     0,       2,        0,        0,    0x200,        4},         //lum_h_str_fade_lut_p0
    /*DB_LVSF_LUT*/
    {0xf8cb2540,     9,      11,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p11
    {0xf8cb2540,     6,       8,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p10
    {0xf8cb2538,    27,      29,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p9
    {0xf8cb2538,    24,      26,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p8
    {0xf8cb2538,    21,      23,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p7
    {0xf8cb2538,    18,      20,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p6
    {0xf8cb2538,    15,      17,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p5
    {0xf8cb2538,    12,      14,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p4
    {0xf8cb2538,     9,      11,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p3
    {0xf8cb2538,     6,       8,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p2
    {0xf8cb2538,     3,       5,        0,        0,    0x200,        3},         //lum_v_str_fade_lut_p1
    {0xf8cb2538,     0,       2,        0,        0,    0x200,        4},         //lum_v_str_fade_lut_p0
    /*DB_CHSF_LUT*/
    {0xf8cb2540,    15,      17,        0,        0,    0x200,        0},         //chr_h_str_fade_lut_p11
    {0xf8cb2540,    12,      14,        0,        0,    0x200,        0},         //chr_h_str_fade_lut_p10
    {0xf8cb253c,    27,      29,        0,        0,    0x200,        0},         //chr_h_str_fade_lut_p9
    {0xf8cb253c,    24,      26,        0,        0,    0x200,        0},         //chr_h_str_fade_lut_p8
    {0xf8cb253c,    21,      23,        0,        0,    0x200,        0},         //chr_h_str_fade_lut_p7
    {0xf8cb253c,    18,      20,        0,        0,    0x200,        0},         //chr_h_str_fade_lut_p6
    {0xf8cb253c,    15,      17,        0,        0,    0x200,        0},         //chr_h_str_fade_lut_p5
    {0xf8cb253c,    12,      14,        0,        0,    0x200,        0},         //chr_h_str_fade_lut_p4
    {0xf8cb253c,     9,      11,        0,        0,    0x200,        3},         //chr_h_str_fade_lut_p3
    {0xf8cb253c,     6,       8,        0,        0,    0x200,        3},         //chr_h_str_fade_lut_p2
    {0xf8cb253c,     3,       5,        0,        0,    0x200,        4},         //chr_h_str_fade_lut_p1
    {0xf8cb253c,     0,       2,        0,        0,    0x200,        4},         //chr_h_str_fade_lut_p0
    /*DB_DSG_LUT*/
    {0xf8cb2544,    28,      31,        0,        0,    0x200,        0},         //dir_str_gain_lut_p7
    {0xf8cb2544,    24,      27,        0,        0,    0x200,        0},         //dir_str_gain_lut_p6
    {0xf8cb2544,    20,      23,        0,        0,    0x200,        0},         //dir_str_gain_lut_p5
    {0xf8cb2544,    16,      19,        0,        0,    0x200,        0},         //dir_str_gain_lut_p4
    {0xf8cb2544,    12,      15,        0,        0,    0x200,        1},         //dir_str_gain_lut_p3
    {0xf8cb2544,     8,      11,        0,        0,    0x200,        3},         //dir_str_gain_lut_p2
    {0xf8cb2544,     4,       7,        0,        0,    0x200,        5},         //dir_str_gain_lut_p1
    {0xf8cb2544,     0,       3,        0,        0,    0x200,        8},         //dir_str_gain_lut_p0
    /*DBD_THD*/
    {0xf8cb2550,    16,      21,        0,        0,    0x200,       32},         //vy_thd_edge
    {0xf8cb2550,     8,      13,        0,        0,    0x200,       32},         //hc_thd_edge
    {0xf8cb2550,     0,       5,        0,        0,    0x200,       32},         //hy_thd_edge
    {0xf8cb2554,    16,      21,        0,        0,    0x200,       16},         //vy_thd_txt
    {0xf8cb2554,     8,      13,        0,        0,    0x200,       16},         //hc_thd_txt
    {0xf8cb2554,     0,       5,        0,        0,    0x200,       16},         //hy_thd_txt
    /*DBD_LUTWGT*/
    {0xf8cb255c,    28,      31,        0,        0,    0x200,       15},         //det_lut_wgt15
    {0xf8cb255c,    24,      27,        0,        0,    0x200,       15},         //det_lut_wgt14
    {0xf8cb255c,    20,      23,        0,        0,    0x200,       15},         //det_lut_wgt13
    {0xf8cb255c,    16,      19,        0,        0,    0x200,       14},         //det_lut_wgt12
    {0xf8cb255c,    12,      15,        0,        0,    0x200,       14},         //det_lut_wgt11
    {0xf8cb255c,     8,      11,        0,        0,    0x200,       13},         //det_lut_wgt10
    {0xf8cb255c,     4,       7,        0,        0,    0x200,       13},         //det_lut_wgt9
    {0xf8cb255c,     0,       3,        0,        0,    0x200,       12},         //det_lut_wgt8
    {0xf8cb2558,    28,      31,        0,        0,    0x200,       12},         //det_lut_wgt7
    {0xf8cb2558,    24,      27,        0,        0,    0x200,       10},         //det_lut_wgt6
    {0xf8cb2558,    20,      23,        0,        0,    0x200,        7},         //det_lut_wgt5
    {0xf8cb2558,    16,      19,        0,        0,    0x200,        5},         //det_lut_wgt4
    {0xf8cb2558,    12,      15,        0,        0,    0x200,        2},         //det_lut_wgt3
    {0xf8cb2558,     8,      11,        0,        0,    0x200,        1},         //det_lut_wgt2
    {0xf8cb2558,     4,       7,        0,        0,    0x200,        0},         //det_lut_wgt1
    {0xf8cb2558,     0,       3,        0,        0,    0x200,        0},         //det_lut_wgt0
    /*DBD_RTNBDTXT*/
    {0xf8cb2560,    16,      20,        0,        0,    0x200,       16},         //vy_rtn_bd_txt
    {0xf8cb2560,     8,      12,        0,        0,    0x200,       16},         //hc_rtn_bd_txt
    {0xf8cb2560,     0,       4,        0,        0,    0x200,       16},         //hy_rtn_bd_txt
    /*DBD_TSTBLKNUM*/
    {0xf8cb2564,    20,      28,        0,        0,    0x200,       60},         //vy_tst_blk_num
    {0xf8cb2564,    10,      18,        0,        0,    0x200,       45},         //hc_tst_blk_num
    {0xf8cb2564,     0,       8,        0,        0,    0x200,       90},         //hy_tst_blk_num
    /*DBD_HYRDX8BINLUT*/
    {0xf8cb2574,     0,      10,        0,        0,    0x200,      208},         //hy_rdx8bin_lut6
    {0xf8cb2570,    12,      22,        0,        0,    0x200,      176},         //hy_rdx8bin_lut5
    {0xf8cb2570,     0,      10,        0,        0,    0x200,      144},         //hy_rdx8bin_lut4
    {0xf8cb256c,    12,      22,        0,        0,    0x200,      112},         //hy_rdx8bin_lut3
    {0xf8cb256c,     0,      10,        0,        0,    0x200,       80},         //hy_rdx8bin_lut2
    {0xf8cb2568,    12,      22,        0,        0,    0x200,       48},         //hy_rdx8bin_lut1
    {0xf8cb2568,     0,      10,        0,        0,    0x200,       16},         //hy_rdx8bin_lut0
    /*DBD_HCRDX8BINLUT*/
    {0xf8cb2584,     0,      10,        0,        0,    0x200,      208},         //hc_rdx8bin_lut6
    {0xf8cb2580,    12,      22,        0,        0,    0x200,      176},         //hc_rdx8bin_lut5
    {0xf8cb2580,     0,      10,        0,        0,    0x200,      144},         //hc_rdx8bin_lut4
    {0xf8cb257c,    12,      22,        0,        0,    0x200,      112},         //hc_rdx8bin_lut3
    {0xf8cb257c,     0,      10,        0,        0,    0x200,       80},         //hc_rdx8bin_lut2
    {0xf8cb2578,    12,      22,        0,        0,    0x200,       48},         //hc_rdx8bin_lut1
    {0xf8cb2578,     0,      10,        0,        0,    0x200,       16},         //hc_rdx8bin_lut0
    /*DBD_VYRDX8BINLUT*/
    {0xf8cb2594,     0,      10,        0,        0,    0x200,      208},         //vy_rdx8bin_lut6
    {0xf8cb2590,    12,      22,        0,        0,    0x200,      176},         //vy_rdx8bin_lut5
    {0xf8cb2590,     0,      10,        0,        0,    0x200,      144},         //vy_rdx8bin_lut4
    {0xf8cb258c,    12,      22,        0,        0,    0x200,      112},         //vy_rdx8bin_lut3
    {0xf8cb258c,     0,      10,        0,        0,    0x200,       80},         //vy_rdx8bin_lut2
    {0xf8cb2588,    12,      22,        0,        0,    0x200,       48},         //vy_rdx8bin_lut1
    {0xf8cb2588,     0,      10,        0,        0,    0x200,       16},         //vy_rdx8bin_lut0
    /*DBD_BLKSIZE*/
    {0xf8cb2598,    24,      29,        0,        0,    0x200,       16},         //vy_max_blk_size
    {0xf8cb2598,    16,      21,        0,        0,    0x200,       16},         //hc_max_blk_size
    {0xf8cb2598,     8,      13,        0,        0,    0x200,       16},         //hy_max_blk_size
    {0xf8cb2598,     0,       5,        0,        0,    0x200,        6},         //min_blk_size

    /*************************************HD_DR*****************************************/
    /*DR_CTRL*/
    {0xf8cb0000,    11,      11,        0,        0,    0x400,        1},         //dr_en
    {0xf8cb2300,     8,       8,        0,        0,    0x400,        0},         //demo_en
    {0xf8cb2300,     9,       9,        0,        0,    0x400,        0},         //demo_mode
    {0xf8cb2300,     7,       7,        0,        0,    0x400,        1},         //clipmode
    {0xf8cb2300,     6,       6,        0,        0,    0x400,        1},         //enangadjstbylm
    {0xf8cb2300,     5,       5,        0,        0,    0x400,        1},         //enstradjstbylm
    {0xf8cb2300,     1,       4,        0,        0,    0x400,       15},         //enovt
    {0xf8cb2300,     0,       0,        0,        0,    0x400,        0},         //enep
    /*DR_THR*/
    {0xf8cb0000,    11,      11,        0,        0,    0x400,        1},         //regovtstrenth
    {0xf8cb2300,     8,       8,        0,        0,    0x400,        8},         //regdrstrenth
    {0xf8cb2300,     9,       9,        0,        0,    0x400,        6},         //regthrovshtminang
    {0xf8cb2300,     7,       7,        0,        0,    0x400,        7},         //regthrmdfangctrst
    /*DRLSWSTRLUT0*/
    {0xf8cb2308,    25,      29,        0,        0,    0x400,       12},         //lswstrlut50
    {0xf8cb2308,    20,      24,        0,        0,    0x400,       12},         //lswstrlut40
    {0xf8cb2308,    15,      19,        0,        0,    0x400,       10},         //lswstrlut30
    {0xf8cb2308,    10,      14,        0,        0,    0x400,        6},         //lswstrlut20
    {0xf8cb2308,     5,       9,        0,        0,    0x400,        2},         //lswstrlut10
    {0xf8cb2308,     0,       4,        0,        0,    0x400,        0},         //lswstrlut00
    /*DRLSWSTRLUT1*/
    {0xf8cb230c,    25,      29,        0,        0,    0x400,        6},         //lswstrlut51
    {0xf8cb230c,    20,      24,        0,        0,    0x400,        6},         //lswstrlut41
    {0xf8cb230c,    15,      19,        0,        0,    0x400,       12},         //lswstrlut31
    {0xf8cb230c,    10,      14,        0,        0,    0x400,       10},         //lswstrlut21
    {0xf8cb230c,     5,       9,        0,        0,    0x400,        4},         //lswstrlut11
    {0xf8cb230c,     0,       4,        0,        0,    0x400,        0},         //lswstrlut01
    /*DRLSWSTRLUT2*/
    {0xf8cb2310,    25,      29,        0,        0,    0x400,        6},         //lswstrlut52
    {0xf8cb2310,    20,      24,        0,        0,    0x400,        6},         //lswstrlut42
    {0xf8cb2310,    15,      19,        0,        0,    0x400,        8},         //lswstrlut32
    {0xf8cb2310,    10,      14,        0,        0,    0x400,        8},         //lswstrlut22
    {0xf8cb2310,     5,       9,        0,        0,    0x400,        4},         //lswstrlut12
    {0xf8cb2310,     0,       4,        0,        0,    0x400,        0},         //lswstrlut02
    /*DRLSWSTRLUT3*/
    {0xf8cb2314,    25,      29,        0,        0,    0x400,       10},         //lswstrlut53
    {0xf8cb2314,    20,      24,        0,        0,    0x400,       10},         //lswstrlut43
    {0xf8cb2314,    15,      19,        0,        0,    0x400,       12},         //lswstrlut33
    {0xf8cb2314,    10,      14,        0,        0,    0x400,        0},         //lswstrlut23
    {0xf8cb2314,     5,       9,        0,        0,    0x400,        0},         //lswstrlut13
    {0xf8cb2314,     0,       4,        0,        0,    0x400,        0},         //lswstrlut03
    /*DRLSWSTRLUT4*/
    {0xf8cb2318,    25,      29,        0,        0,    0x400,        8},         //lswstrlut54
    {0xf8cb2318,    20,      24,        0,        0,    0x400,        8},         //lswstrlut44
    {0xf8cb2318,    15,      19,        0,        0,    0x400,        8},         //lswstrlut34
    {0xf8cb2318,    10,      14,        0,        0,    0x400,        0},         //lswstrlut24
    {0xf8cb2318,     5,       9,        0,        0,    0x400,        0},         //lswstrlut14
    {0xf8cb2318,     0,       4,        0,        0,    0x400,        0},         //lswstrlut04
    /*DRLSWSTRLUT5*/
    {0xf8cb231c,    25,      29,        0,        0,    0x400,        4},         //lswstrlut55
    {0xf8cb231c,    20,      24,        0,        0,    0x400,        4},         //lswstrlut45
    {0xf8cb231c,    15,      19,        0,        0,    0x400,        0},         //lswstrlut35
    {0xf8cb231c,    10,      14,        0,        0,    0x400,        0},         //lswstrlut25
    {0xf8cb231c,     5,       9,        0,        0,    0x400,        0},         //lswstrlut15
    {0xf8cb231c,     0,       4,        0,        0,    0x400,        0},         //lswstrlut05
    /*DRLSWSTRLUT6*/
    {0xf8cb2320,    25,      29,        0,        0,    0x400,        0},         //lswstrlut56
    {0xf8cb2320,    20,      24,        0,        0,    0x400,        0},         //lswstrlut46
    {0xf8cb2320,    15,      19,        0,        0,    0x400,        0},         //lswstrlut36
    {0xf8cb2320,    10,      14,        0,        0,    0x400,        0},         //lswstrlut26
    {0xf8cb2320,     5,       9,        0,        0,    0x400,        0},         //lswstrlut16
    {0xf8cb2320,     0,       4,        0,        0,    0x400,        0},         //lswstrlut06
    /*DRLSWSTRLUT7*/
    {0xf8cb2324,    25,      29,        0,        0,    0x400,        0},         //lswstrlut57
    {0xf8cb2324,    20,      24,        0,        0,    0x400,        0},         //lswstrlut47
    {0xf8cb2324,    15,      19,        0,        0,    0x400,        0},         //lswstrlut37
    {0xf8cb2324,    10,      14,        0,        0,    0x400,        0},         //lswstrlut27
    {0xf8cb2324,     5,       9,        0,        0,    0x400,        0},         //lswstrlut17
    {0xf8cb2324,     0,       4,        0,        0,    0x400,        0},         //lswstrlut07
    /*DRLSWSTRLUT8*/
    {0xf8cb2328,    25,      29,        0,        0,    0x400,        0},         //lswstrlut58
    {0xf8cb2328,    20,      24,        0,        0,    0x400,        0},         //lswstrlut48
    {0xf8cb2328,    15,      19,        0,        0,    0x400,        0},         //lswstrlut38
    {0xf8cb2328,    10,      14,        0,        0,    0x400,        0},         //lswstrlut28
    {0xf8cb2328,     5,       9,        0,        0,    0x400,        0},         //lswstrlut18
    {0xf8cb2328,     0,       4,        0,        0,    0x400,        0},         //lswstrlut08
    /*DRLRWSTRLUT0*/
    {0xf8cb232c,    25,      29,        0,        0,    0x400,       12},         //lrwstrlut50
    {0xf8cb232c,    20,      24,        0,        0,    0x400,       12},         //lrwstrlut40
    {0xf8cb232c,    15,      19,        0,        0,    0x400,       10},         //lrwstrlut30
    {0xf8cb232c,    10,      14,        0,        0,    0x400,        3},         //lrwstrlut20
    {0xf8cb232c,     5,       9,        0,        0,    0x400,        1},         //lrwstrlut10
    {0xf8cb232c,     0,       4,        0,        0,    0x400,        1},         //lrwstrlut00
    /*DRLRWSTRLUT1*/
    {0xf8cb2330,    25,      29,        0,        0,    0x400,       16},         //lrwstrlut51
    {0xf8cb2330,    20,      24,        0,        0,    0x400,       16},         //lrwstrlut41
    {0xf8cb2330,    15,      19,        0,        0,    0x400,       16},         //lrwstrlut31
    {0xf8cb2330,    10,      14,        0,        0,    0x400,        6},         //lrwstrlut21
    {0xf8cb2330,     5,       9,        0,        0,    0x400,        2},         //lrwstrlut11
    {0xf8cb2330,     0,       4,        0,        0,    0x400,        0},         //lrwstrlut01
    /*DRLRWSTRLUT2*/
    {0xf8cb2334,    25,      29,        0,        0,    0x400,       16},         //lrwstrlut52
    {0xf8cb2334,    20,      24,        0,        0,    0x400,       16},         //lrwstrlut42
    {0xf8cb2334,    15,      19,        0,        0,    0x400,       16},         //lrwstrlut32
    {0xf8cb2334,    10,      14,        0,        0,    0x400,        6},         //lrwstrlut22
    {0xf8cb2334,     5,       9,        0,        0,    0x400,        2},         //lrwstrlut12
    {0xf8cb2334,     0,       4,        0,        0,    0x400,        0},         //lrwstrlut02
    /*DRLRWSTRLUT3*/
    {0xf8cb2338,    25,      29,        0,        0,    0x400,       12},         //lrwstrlut53
    {0xf8cb2338,    20,      24,        0,        0,    0x400,       12},         //lrwstrlut43
    {0xf8cb2338,    15,      19,        0,        0,    0x400,        8},         //lrwstrlut33
    {0xf8cb2338,    10,      14,        0,        0,    0x400,        6},         //lrwstrlut23
    {0xf8cb2338,     5,       9,        0,        0,    0x400,        2},         //lrwstrlut13
    {0xf8cb2338,     0,       4,        0,        0,    0x400,        0},         //lrwstrlut03
    /*DRLRWSTRLUT4*/
    {0xf8cb233c,    25,      29,        0,        0,    0x400,       10},         //lrwstrlut54
    {0xf8cb233c,    20,      24,        0,        0,    0x400,       10},         //lrwstrlut44
    {0xf8cb233c,    15,      19,        0,        0,    0x400,        4},         //lrwstrlut34
    {0xf8cb233c,    10,      14,        0,        0,    0x400,        2},         //lrwstrlut24
    {0xf8cb233c,     5,       9,        0,        0,    0x400,        0},         //lrwstrlut14
    {0xf8cb233c,     0,       4,        0,        0,    0x400,        0},         //lrwstrlut04
    /*DRLRWSTRLUT5*/
    {0xf8cb2340,    25,      29,        0,        0,    0x400,        6},         //lrwstrlut55
    {0xf8cb2340,    20,      24,        0,        0,    0x400,        6},         //lrwstrlut45
    {0xf8cb2340,    15,      19,        0,        0,    0x400,        2},         //lrwstrlut35
    {0xf8cb2340,    10,      14,        0,        0,    0x400,        0},         //lrwstrlut25
    {0xf8cb2340,     5,       9,        0,        0,    0x400,        0},         //lrwstrlut15
    {0xf8cb2340,     0,       4,        0,        0,    0x400,        0},         //lrwstrlut05
    /*DRLRWSTRLUT6*/
    {0xf8cb2344,    25,      29,        0,        0,    0x400,        2},         //lrwstrlut56
    {0xf8cb2344,    20,      24,        0,        0,    0x400,        2},         //lrwstrlut46
    {0xf8cb2344,    15,      19,        0,        0,    0x400,        0},         //lrwstrlut36
    {0xf8cb2344,    10,      14,        0,        0,    0x400,        0},         //lrwstrlut26
    {0xf8cb2344,     5,       9,        0,        0,    0x400,        0},         //lrwstrlut16
    {0xf8cb2344,     0,       4,        0,        0,    0x400,        0},         //lrwstrlut06
    /*DRLRWSTRLUT7*/
    {0xf8cb2348,    25,      29,        0,        0,    0x400,        0},         //lrwstrlut57
    {0xf8cb2348,    20,      24,        0,        0,    0x400,        0},         //lrwstrlut47
    {0xf8cb2348,    15,      19,        0,        0,    0x400,        0},         //lrwstrlut37
    {0xf8cb2348,    10,      14,        0,        0,    0x400,        0},         //lrwstrlut27
    {0xf8cb2348,     5,       9,        0,        0,    0x400,        0},         //lrwstrlut17
    {0xf8cb2348,     0,       4,        0,        0,    0x400,        0},         //lrwstrlut07
    /*DRLRWSTRLUT8*/
    {0xf8cb234c,    25,      29,        0,        0,    0x400,        0},         //lrwstrlut58
    {0xf8cb234c,    20,      24,        0,        0,    0x400,        0},         //lrwstrlut48
    {0xf8cb234c,    15,      19,        0,        0,    0x400,        0},         //lrwstrlut38
    {0xf8cb234c,    10,      14,        0,        0,    0x400,        0},         //lrwstrlut28
    {0xf8cb234c,     5,       9,        0,        0,    0x400,        0},         //lrwstrlut18
    {0xf8cb234c,     0,       4,        0,        0,    0x400,        0},         //lrwstrlut08
    /*DRLSWSTRADJUSTLUT*/
    {0xf8cb2358,     8,      11,        0,        0,    0x400,        8},         //dr_lswadjlut8
    {0xf8cb2358,     4,       7,        0,        0,    0x400,        8},         //dr_lswadjlut7
    {0xf8cb2358,     0,       3,        0,        0,    0x400,        8},         //dr_lswadjlut6
    {0xf8cb2354,     8,      11,        0,        0,    0x400,        8},         //dr_lswadjlut5
    {0xf8cb2354,     4,       7,        0,        0,    0x400,        6},         //dr_lswadjlut4
    {0xf8cb2354,     0,       3,        0,        0,    0x400,        4},         //dr_lswadjlut3
    {0xf8cb2350,     8,      11,        0,        0,    0x400,        4},         //dr_lswadjlut2
    {0xf8cb2350,     4,       7,        0,        0,    0x400,        4},         //dr_lswadjlut1
    {0xf8cb2350,     0,       3,        0,        0,    0x400,        4},         //dr_lswadjlut0
    /*DRLSWSTRADJUSTLUT*/
    {0xf8cb2364,     8,      11,        0,        0,    0x400,        4},         //dr_lrwadjlut8
    {0xf8cb2364,     4,       7,        0,        0,    0x400,        4},         //dr_lrwadjlut7
    {0xf8cb2364,     0,       3,        0,        0,    0x400,        4},         //dr_lrwadjlut6
    {0xf8cb2360,     8,      11,        0,        0,    0x400,        4},         //dr_lrwadjlut5
    {0xf8cb2360,     4,       7,        0,        0,    0x400,        4},         //dr_lrwadjlut4
    {0xf8cb2360,     0,       3,        0,        0,    0x400,        4},         //dr_lrwadjlut3
    {0xf8cb235c,     8,      11,        0,        0,    0x400,        4},         //dr_lrwadjlut2
    {0xf8cb235c,     4,       7,        0,        0,    0x400,        4},         //dr_lrwadjlut1
    {0xf8cb235c,     0,       3,        0,        0,    0x400,        4},         //dr_lrwadjlut0
    /*DRANGADJSUTLUT*/
    {0xf8cb2370,     8,      11,        0,        0,    0x400,        4},         //dr_angadjlut8
    {0xf8cb2370,     4,       7,        0,        0,    0x400,        4},         //dr_angadjlut7
    {0xf8cb2370,     0,       3,        0,        0,    0x400,        4},         //dr_angadjlut6
    {0xf8cb236c,     8,      11,        0,        0,    0x400,        4},         //dr_angadjlut5
    {0xf8cb236c,     4,       7,        0,        0,    0x400,        4},         //dr_angadjlut4
    {0xf8cb236c,     0,       3,        0,        0,    0x400,        4},         //dr_angadjlut3
    {0xf8cb2368,     8,      11,        0,        0,    0x400,        6},         //dr_angadjlut2
    {0xf8cb2368,     4,       7,        0,        0,    0x400,        8},         //dr_angadjlut1
    {0xf8cb2368,     0,       3,        0,        0,    0x400,       12},         //dr_angadjlut0
    /*DRANGLESTRLUT1*/
    {0xf8cb237c,    20,      24,        0,        0,    0x400,       16},         //dr_angdifflut16
    {0xf8cb237c,    15,      19,        0,        0,    0x400,       15},         //dr_angdifflut15
    {0xf8cb237c,    10,      14,        0,        0,    0x400,       14},         //dr_angdifflut14
    {0xf8cb237c,     5,       9,        0,        0,    0x400,       13},         //dr_angdifflut13
    {0xf8cb237c,     0,       4,        0,        0,    0x400,       12},         //dr_angdifflut12
    {0xf8cb2378,    25,      29,        0,        0,    0x400,        8},         //dr_angdifflut11
    {0xf8cb2378,    20,      24,        0,        0,    0x400,        7},         //dr_angdifflut10
    {0xf8cb2378,    15,      19,        0,        0,    0x400,        6},         //dr_angdifflut9
    {0xf8cb2378,    10,      14,        0,        0,    0x400,        5},         //dr_angdifflut8
    {0xf8cb2378,     5,       9,        0,        0,    0x400,        4},         //dr_angdifflut7
    {0xf8cb2378,     0,       4,        0,        0,    0x400,        3},         //dr_angdifflut6
    {0xf8cb2374,    25,      29,        0,        0,    0x400,        2},         //dr_angdifflut5
    {0xf8cb2374,    20,      24,        0,        0,    0x400,        1},         //dr_angdifflut4
    {0xf8cb2374,    15,      19,        0,        0,    0x400,        1},         //dr_angdifflut3
    {0xf8cb2374,    10,      14,        0,        0,    0x400,        0},         //dr_angdifflut2
    {0xf8cb2374,     5,       9,        0,        0,    0x400,        0},         //dr_angdifflut1
    {0xf8cb2374,     0,       4,        0,        0,    0x400,        0},         //dr_angdifflut0
    /*DRANGLESTRLUT2*/
    {0xf8cb2388,    20,      24,        0,        0,    0x400,        0},         //dr_angminlut16
    {0xf8cb2388,    15,      19,        0,        0,    0x400,        0},         //dr_angminlut15
    {0xf8cb2388,    10,      14,        0,        0,    0x400,        0},         //dr_angminlut14
    {0xf8cb2388,     5,       9,        0,        0,    0x400,        0},         //dr_angminlut13
    {0xf8cb2388,     0,       4,        0,        0,    0x400,        0},         //dr_angminlut12
    {0xf8cb2384,    25,      29,        0,        0,    0x400,        0},         //dr_angminlut11
    {0xf8cb2384,    20,      24,        0,        0,    0x400,        0},         //dr_angminlut10
    {0xf8cb2384,    15,      19,        0,        0,    0x400,        0},         //dr_angminlut9
    {0xf8cb2384,    10,      14,        0,        0,    0x400,        0},         //dr_angminlut8
    {0xf8cb2384,     5,       9,        0,        0,    0x400,        0},         //dr_angminlut7
    {0xf8cb2384,     0,       4,        0,        0,    0x400,        0},         //dr_angminlut6
    {0xf8cb2380,    25,      29,        0,        0,    0x400,        2},         //dr_angminlut5
    {0xf8cb2380,    20,      24,        0,        0,    0x400,        6},         //dr_angminlut4
    {0xf8cb2380,    15,      19,        0,        0,    0x400,       12},         //dr_angminlut3
    {0xf8cb2380,    10,      14,        0,        0,    0x400,       14},         //dr_angminlut2
    {0xf8cb2380,     5,       9,        0,        0,    0x400,       16},         //dr_angminlut1
    {0xf8cb2380,     0,       4,        0,        0,    0x400,       16},         //dr_angminlut0
    /*DROVTSTRADJUSTLUT*/
    {0xf8cb2394,     8,      11,        0,        0,    0x400,        4},         //dr_ovadjlut8
    {0xf8cb2394,     4,       7,        0,        0,    0x400,        4},         //dr_ovadjlut7
    {0xf8cb2394,     0,       3,        0,        0,    0x400,        4},         //dr_ovadjlut6
    {0xf8cb2390,     8,      11,        0,        0,    0x400,        4},         //dr_ovadjlut5
    {0xf8cb2390,     4,       7,        0,        0,    0x400,        4},         //dr_ovadjlut4
    {0xf8cb2390,     0,       3,        0,        0,    0x400,        3},         //dr_ovadjlut3
    {0xf8cb238c,     8,      11,        0,        0,    0x400,        2},         //dr_ovadjlut2
    {0xf8cb238c,     4,       7,        0,        0,    0x400,        1},         //dr_ovadjlut1
    {0xf8cb238c,     0,       3,        0,        0,    0x400,        0},         //dr_ovadjlut0
    /*DROVTSTRLUT*/
    {0xf8cb23a0,     8,      11,        0,        0,    0x400,        0},         //dr_ovstrlut8
    {0xf8cb23a0,     4,       7,        0,        0,    0x400,        1},         //dr_ovstrlut7
    {0xf8cb23a0,     0,       3,        0,        0,    0x400,        2},         //dr_ovstrlut6
    {0xf8cb239c,     8,      11,        0,        0,    0x400,        4},         //dr_ovstrlut5
    {0xf8cb239c,     4,       7,        0,        0,    0x400,        6},         //dr_ovstrlut4
    {0xf8cb239c,     0,       3,        0,        0,    0x400,        8},         //dr_ovstrlut3
    {0xf8cb2398,     8,      11,        0,        0,    0x400,        8},         //dr_ovstrlut2
    {0xf8cb2398,     4,       7,        0,        0,    0x400,        8},         //dr_ovstrlut1
    {0xf8cb2398,     0,       3,        0,        0,    0x400,        8},         //dr_ovstrlut0
    /*************************************SD_DR*****************************************/
    /*DNR_CTRL*/
    {0xf8cb0000,     9,       9,        0,        0,      0x8,        1},         //dnr_en
    {0xf8cb0074,    24,      24,        0,        0,      0x8,        0},         //dr_demo_en
    /*DR_FILTER*/
    {0xf8cb0070,    24,      28,        0,        0,      0x8,        8},         //dr_thr_flat3x3zone
    {0xf8cb0070,    16,      20,        0,        0,      0x8,       16},         //dr_thr_maxsimilarpixdiff
    {0xf8cb0074,    16,      20,        0,        0,      0x8,        8},         //dr_betascale
    {0xf8cb0074,     0,       4,        0,        0,      0x8,        8},         //dr_alphascale
    /*DETECT_INFO*/
    {0xf8cb0078,    25,      25,        0,        0,      0x8,        0},         //dnr_dither_mode
    {0xf8cb0078,    24,      24,        0,        0,      0x8,        1},         //dnr_dither_en
    {0xf8cb0078,    16,      23,        0,        0,      0x8,       64},         //thd_cntrst8
    {0xf8cb0078,     8,      15,        0,        0,      0x8,       40},         //thd_maxgrad
    {0xf8cb0078,     4,       7,        0,        0,      0x8,        4},         //thr_intl_cnt
    {0xf8cb0078,     0,       3,        0,        0,      0x8,        1},         //thr_intl_colcnt

    /**************************************DCI******************************************/
    /*DCI_CTRL_SET*/
    {0xF8cc6100,    31,      31,        0,        0,      0x10,       0},         //dci_en
    {0xF8cc6100,    30,      30,        0,        0,      0x10,       0},         //dci_dbg_en
    {0xF8cc6100,    29,      29,        0,        0,      0x10,       0},         //dci_dbg_mode
    {0xF8cc6100,    27,      27,        0,        0,      0x10,       0},         //dci_man_adj0
    {0xF8cc6100,    26,      26,        0,        0,      0x10,       0},         //dci_man_adj1
    {0xF8cc6100,    25,      25,        0,        0,      0x10,       0},         //dci_man_adj2
    {0xF8cc6100,    24,      24,        0,        0,      0x10,       1},         //dci_cbcrcmp_en
    {0xF8cc6100,    23,      23,        0,        0,      0x10,       0},         //dci_cbcrsta_en
    {0xF8cc6100,    22,      22,        0,        0,      0x10,       1},         //dci_in_range
    {0xF8cc6100,    21,      21,        0,        0,      0x10,       1},         //dci_out_range
    {0xF8cc6100,    19,      20,        0,        0,      0x10,       2},         //dci_shift_ctrl
    {0xF8cc6100,    18,      18,        0,        0,      0x10,       1},         //dci_histlpf_en
    {0xF8cc6100,    17,      17,        0,        0,      0x10,       0},         //dci_swcfg_en
    {0xF8cc6100,    16,      16,        0,        0,      0x10,       0},         //dci_hist_state
    {0xF8cc6100,    14,      15,        0,        0,      0x10,       0},         //dci_stat_ctrl
    {0xF8cc6100,     3,      13,        0,        0,      0x10,       0},         //dci_y_linear
    /*DCI_HIS_BLD*/
    {0xF8cc610c,    24,      31,        0,        0,      0x10,      64},         //dci_cbcrsta_y
    {0xF8cc610c,    16,      23,        0,        0,      0x10,       0},         //dci_cbcrsta_cb
    {0xF8cc610c,    8,       15,        0,        0,      0x10,       0},         //dci_cbcrsta_cr
    /*DCI_HIS_OFT*/
    {0xF8cc6110,    23,      31,        0,        0,      0x10,       0},         //dci_cbcrsta_oft
    /*DCI_HIS_COR*/
    {0xF8cc6114,    24,      31,        0,        0,      0x10,       0},         //dci_histcor_thr0
    {0xF8cc6114,    16,      23,        0,        0,      0x10,       0},         //dci_histcor_thr1
    {0xF8cc6114,    8,       15,        0,        0,      0x10,       0},         //dci_histcor_thr2
    /*DCI_MER_BLD*/
    {0xF8cc6118,    26,      31,        0,        0,      0x10,      18},         //dci_metrc_abld0
    {0xF8cc6118,    20,      25,        0,        0,      0x10,      18},         //dci_metrc_abld1
    {0xF8cc6118,    14,      19,        0,        0,      0x10,      18},         //dci_metrc_abld2
    {0xF8cc6118,    8,       13,        0,        0,      0x10,      16},         //dci_hist_abld
    {0xF8cc6118,    2,       7,         0,        0,      0x10,      17},         //dci_org_abld
    /*DCI_ADJ_WGT*/
    {0xF8cc611c,    24,      31,        0,        0,      0x10,      50},         //dci_man_adjwgt0
    {0xF8cc611c,    16,      23,        0,        0,      0x10,      60},         //dci_man_adjwgt1
    {0xF8cc611c,    8,       15,        0,        0,      0x10,      70},         //dci_man_adjwgt2
    /*DCI_CLIP*/
    {0xF8cc6120,    24,      31,        0,        0,      0x10,       0},         //dci_wgt_cliplow0
    {0xF8cc6120,    16,      23,        0,        0,      0x10,     255},         //dci_wgt_cliphigh0
    {0xF8cc6124,    24,      31,        0,        0,      0x10,       0},         //dci_wgt_cliplow1
    {0xF8cc6124,    16,      23,        0,        0,      0x10,     255},         //dci_wgt_cliphigh1
    {0xF8cc6128,    24,      31,        0,        0,      0x10,       0},         //dci_wgt_cliplow2
    {0xF8cc6128,    16,      23,        0,        0,      0x10,     255},         //dci_wgt_cliphigh2
    /*DCI_GLB_GAIN_SD_SOURCE*/
    {0xF8cc612c,    26,      31,        1,        0,      0x10,      32},         //dci_glb_gain_sd0
    {0xF8cc612c,    20,      25,        1,        0,      0x10,      32},         //dci_glb_gain_sd1
    {0xF8cc612c,    14,      19,        1,        0,      0x10,      32},         //dci_glb_gain_sd2
    /*DCI_GLB_GAIN_HD_SOURCE*/
    {0xF8cc612c,    26,      31,        2,        0,      0x10,      42},         //dci_glb_gain_hd0
    {0xF8cc612c,    20,      25,        2,        0,      0x10,      42},         //dci_glb_gain_hd1
    {0xF8cc612c,    14,      19,        2,        0,      0x10,      42},         //dci_glb_gain_hd2
    /*DCI_GLB_GAIN_UHD_SOURCE*/
    {0xF8cc612c,    26,      31,        3,        0,      0x10,      48},         //dci_glb_gain_uhd0
    {0xF8cc612c,    20,      25,        3,        0,      0x10,      48},         //dci_glb_gain_uhd0
    {0xF8cc612c,    14,      19,        3,        0,      0x10,      42},         //dci_glb_gain_uhd0
    /*DCI_POS_THR*/
    {0xF8cc6130,    24,      31,        0,        0,      0x10,      20},         //dci_gainpos_thr1
    {0xF8cc6130,    16,      23,        0,        0,      0x10,      30},         //dci_gainpos_thr2
    {0xF8cc6130,    8,       15,        0,        0,      0x10,      40},         //dci_gainpos_thr3
    {0xF8cc6130,    0,       7,         0,        0,      0x10,      50},         //dci_gainpos_thr4
    {0xF8cc6134,    24,      31,        0,        0,      0x10,      60},         //dci_gainpos_thr5
    {0xF8cc6134,    16,      23,        0,        0,      0x10,      70},         //dci_gainpos_thr6
    {0xF8cc6134,    8,       15,        0,        0,      0x10,     115},         //dci_gainpos_thr7
    /*DCI_POS_GAIN*/
    {0xF8cc6138,    28,      31,        0,        0,      0x10,       0},         //dci_gainpos_cbcr0
    {0xF8cc6138,    24,      27,        0,        0,      0x10,       2},         //dci_gainpos_cbcr1
    {0xF8cc6138,    20,      23,        0,        0,      0x10,       4},         //dci_gainpos_cbcr2
    {0xF8cc6138,    16,      19,        0,        0,      0x10,       6},         //dci_gainpos_cbcr3
    {0xF8cc6138,    12,      15,        0,        0,      0x10,       7},         //dci_gainpos_cbcr4
    {0xF8cc6138,    8,       11,        0,        0,      0x10,       8},         //dci_gainpos_cbcr5
    {0xF8cc6138,    4,       7,         0,        0,      0x10,       9},         //dci_gainpos_cbcr6
    {0xF8cc6138,    0,       3,         0,        0,      0x10,      10},         //dci_gainpos_cbcr7
    {0xF8cc613c,    28,      31,        0,        0,      0x10,      10},         //dci_gainpos_cbcr8
    /*DCI_POS_SLP*/
    {0xF8cc6140,    22,      31,        0,        0,      0x10,      25},         //dci_gainpos_slp0
    {0xF8cc6140,    12,      21,        0,        0,      0x10,      51},         //dci_gainpos_slp1
    {0xF8cc6140,    2,       11,        0,        0,      0x10,      51},         //dci_gainpos_slp2
    {0xF8cc6144,    22,      31,        0,        0,      0x10,      25},         //dci_gainpos_slp3
    {0xF8cc6144,    12,      21,        0,        0,      0x10,      25},         //dci_gainpos_slp4
    {0xF8cc6144,    2,       11,        0,        0,      0x10,      25},         //dci_gainpos_slp5
    {0xF8cc6148,    22,      31,        0,        0,      0x10,       5},         //dci_gainpos_slp6
    {0xF8cc6148,    12,      21,        0,        0,      0x10,       0},         //dci_gainpos_slp7
    /*DCI_NEG_THR*/
    {0xF8cc614c,    24,      31,        0,        0,      0x10,      30},         //dci_gainneg_thr1
    {0xF8cc614c,    16,      23,        0,        0,      0x10,      50},         //dci_gainneg_thr2
    {0xF8cc614c,    8,       15,        0,        0,      0x10,      70},         //dci_gainneg_thr3
    {0xF8cc614c,    0,       7,         0,        0,      0x10,      90},         //dci_gainneg_thr4
    {0xF8cc6150,    24,      31,        0,        0,      0x10,     130},         //dci_gainneg_thr5
    {0xF8cc6150,    16,      23,        0,        0,      0x10,     150},         //dci_gainneg_thr6
    {0xF8cc6150,    8,       15,        0,        0,      0x10,     170},         //dci_gainneg_thr7
    /*DCI_NEG_CBCR_THR*/
    {0xF8cc6154,    28,      31,        0,        0,      0x10,       0},         //dci_gainneg_cbcr0
    {0xF8cc6154,    24,      27,        0,        0,      0x10,       1},         //dci_gainneg_cbcr1
    {0xF8cc6154,    20,      23,        0,        0,      0x10,       2},         //dci_gainneg_cbcr2
    {0xF8cc6154,    16,      19,        0,        0,      0x10,       2},         //dci_gainneg_cbcr3
    {0xF8cc6154,    12,      15,        0,        0,      0x10,       2},         //dci_gainneg_cbcr4
    {0xF8cc6154,    8,       11,        0,        0,      0x10,       2},         //dci_gainneg_cbcr5
    {0xF8cc6154,    4,       7,         0,        0,      0x10,       2},         //dci_gainneg_cbcr6
    {0xF8cc6154,    0,       3,         0,        0,      0x10,       2},         //dci_gainneg_cbcr7
    {0xF8cc6158,    28,      31,        0,        0,      0x10,       2},         //dci_gainneg_cbcr8
    /*DCI_NEG_SLP*/
    {0xF8cc615c,    22,      31,        0,        0,      0x10,       8},         //dci_gainneg_slp0
    {0xF8cc615c,    12,      21,        0,        0,      0x10,      12},         //dci_gainneg_slp1
    {0xF8cc615c,    2,       11,        0,        0,      0x10,       0},         //dci_gainneg_slp2
    {0xF8cc6160,    22,      31,        0,        0,      0x10,       0},         //dci_gainneg_slp3
    {0xF8cc6160,    12,      21,        0,        0,      0x10,       0},         //dci_gainneg_slp4
    {0xF8cc6160,    2,       11,        0,        0,      0x10,       0},         //dci_gainneg_slp5
    {0xF8cc6164,    22,      31,        0,        0,      0x10,       0},         //dci_gainneg_slp6
    {0xF8cc6164,    12,      21,        0,        0,      0x10,       0},         //dci_gainneg_slp7
    /*DCI_ADJ_SW*/
    {0xF8cc6168,    22,      31,        0,        0,      0x10,       0},         //dci_adj_sw0
    {0xF8cc6168,    12,      21,        0,        0,      0x10,       0},         //dci_adj_sw1
    {0xF8cc6168,    2,       11,        0,        0,      0x10,       0},         //dci_adj_sw2
    {0xF8cc616c,    22,      31,        0,        0,      0x10,     -16},         //dci_adj_sw3
    {0xF8cc616c,    12,      21,        0,        0,      0x10,     -30},         //dci_adj_sw4
    {0xF8cc616c,    2,       11,        0,        0,      0x10,     -41},         //dci_adj_sw5
    {0xF8cc6170,    22,      31,        0,        0,      0x10,     -49},         //dci_adj_sw6
    {0xF8cc6170,    12,      21,        0,        0,      0x10,     -45},         //dci_adj_sw7
    {0xF8cc6170,    2,       11,        0,        0,      0x10,     -38},         //dci_adj_sw8
    {0xF8cc6174,    22,      31,        0,        0,      0x10,     -32},         //dci_adj_sw9
    {0xF8cc6174,    12,      21,        0,        0,      0x10,     -26},         //dci_adj_sw10
    {0xF8cc6174,    2,       11,        0,        0,      0x10,     -12},         //dci_adj_sw11
    {0xF8cc6178,    22,      31,        0,        0,      0x10,      -6},         //dci_adj_sw12
    {0xF8cc6178,    12,      21,        0,        0,      0x10,      13},         //dci_adj_sw13
    {0xF8cc6178,    2,       11,        0,        0,      0x10,      28},         //dci_adj_sw14
    {0xF8cc617c,    22,      31,        0,        0,      0x10,      38},         //dci_adj_sw15
    {0xF8cc617c,    12,      21,        0,        0,      0x10,      53},         //dci_adj_sw16
    {0xF8cc617c,    2,       11,        0,        0,      0x10,      62},         //dci_adj_sw17
    {0xF8cc6180,    22,      31,        0,        0,      0x10,      69},         //dci_adj_sw18
    {0xF8cc6180,    12,      21,        0,        0,      0x10,      70},         //dci_adj_sw19
    {0xF8cc6180,    2,       11,        0,        0,      0x10,      72},         //dci_adj_sw20
    {0xF8cc6184,    22,      31,        0,        0,      0x10,      73},         //dci_adj_sw21
    {0xF8cc6184,    12,      21,        0,        0,      0x10,      70},         //dci_adj_sw22
    {0xF8cc6184,    2,       11,        0,        0,      0x10,      66},         //dci_adj_sw23
    {0xF8cc6188,    22,      31,        0,        0,      0x10,      58},         //dci_adj_sw24
    {0xF8cc6188,    12,      21,        0,        0,      0x10,      52},         //dci_adj_sw25
    {0xF8cc6188,    2,       11,        0,        0,      0x10,      44},         //dci_adj_sw26
    {0xF8cc618c,    22,      31,        0,        0,      0x10,      34},         //dci_adj_sw27
    {0xF8cc618c,    12,      21,        0,        0,      0x10,      10},         //dci_adj_sw28
    {0xF8cc618c,    2,       11,        0,        0,      0x10,       0},         //dci_adj_sw29
    {0xF8cc6190,    22,      31,        0,        0,      0x10,       0},         //dci_adj_sw30
    {0xF8cc6190,    12,      21,        0,        0,      0x10,       0},         //dci_adj_sw31
    {0xF8cc6190,    2,       11,        0,        0,      0x10,       0},         //dci_adj_sw32

    /*************************************ACM****************************************/
    /*VP0_ACM_CTRL*/
    {0xf8cc61a0,    31,      31,        0,        0,      0x20,       0},         //acm_en
    {0xf8cc61a0,    30,      30,        0,        0,      0x20,       0},         //acm_dbg_en
    {0xf8cc61a0,    29,      29,        0,        0,      0x20,       1},         //acm_stretch
    {0xf8cc61a0,    28,      28,        0,        0,      0x20,       1},         //acm_cliprange
    {0xf8cc61a0,    27,      27,        0,        0,      0x20,       0},         //acm_cliporwrap
    {0xf8cc61a0,    9,       9,         0,        0,      0x20,       0},         //acm_dbg_mode
    {0xf8cc61a0,    0,       8,         0,        0,      0x20,       0},         //acm_cbcrthr
    /*VP0_ACM_GAIN_SD_SOURCE*/
    {0xf8cc61a4,    20,      29,        1,        0,      0x20,      80},         //acm_gain_luma_sd
    {0xf8cc61a4,    10,      19,        1,        0,      0x20,      80},         //acm_gain_hue_sd
    {0xf8cc61a4,    0,       9,         1,        0,      0x20,      80},         //acm_gain_sat_sd
    /*VP0_ACM_GAIN_HD_SOURCE*/
    {0xf8cc61a4,    20,      29,        2,        0,      0x20,      80},         //acm_gain_luma_hd
    {0xf8cc61a4,    10,      19,        2,        0,      0x20,      80},         //acm_gain_hue_hd
    {0xf8cc61a4,    0,       9,         2,        0,      0x20,      80},         //acm_gain_sat_hd
    /*VP0_ACM_GAIN_UHD_SOURCE*/
    {0xf8cc61a4,    20,      29,        3,        0,      0x20,      80},         //acm_gain_luma_uhd
    {0xf8cc61a4,    10,      19,        3,        0,      0x20,      80},         //acm_gain_hue_uhd
    {0xf8cc61a4,    0,       9,         3,        0,      0x20,      80},         //acm_gain_sat_uhd

    /************************************SR*************************************/
    /*SD Source*/
    /*SR_CTRL*/
    {0xF8ccb880,    29,      30,        1,        0,     0x100,       0},         //sr_disp_state
    {0xF8ccb894,    31,      31,        1,        0,     0x100,       1},         //sharp_en
    {0xF8ccb898,    28,      31,        1,        0,     0x100,       4},         //sharp_coring
    {0xF8ccb898,     0,      10,        1,        0,     0x100,     128},         //sharp_gain
    /*SR_LDIR_RATIO*/
    {0xF8ccb884,    0,        5,        1,        0,     0x100,      14},         //grd_sub_ratio
    /*SR_LTEX_THR*/
    {0xF8ccb88c,    16,      31,        1,        0,     0x100,       4},         //grd_thrh_subl
    {0xF8ccb888,    12,      22,        1,        0,     0x100,      16},         //grd_thrh
    {0xF8ccb888,    0,       10,        1,        0,     0x100,     -48},         //grd_thrl
    /*SR_LTEX_ADD*/
    {0xF8ccb88c,    0,        7,        1,        0,     0x100,       8},         //grd_add_ctrl
    /*SR_DIPL_DETAIL*/
    {0xF8ccb890,    0,       11,        1,        0,     0x100,       0},         //disc_detail_gain
    /*SR_SMOOTH*/
    {0xF8ccb89C,    31,      31,        1,        0,     0x100,       1},         //smooth_en
    {0xF8ccb89C,    30,      30,        1,        0,     0x100,       1},         //plus_en
    {0xF8ccb89C,    0,        9,        1,        0,     0x100,     140},         //plus_ctrl
    /*SR_RINGDT*/
    {0xF8ccb900,    30,      30,        1,        0,     0x100,       1},         //ringdt_en
    {0xF8ccb900,    20,      29,        1,        0,     0x100,       1},         //grad_flat_thr
    {0xF8ccb900,    10,      19,        1,        0,     0x100,     140},         //grad_thres_l
    {0xF8ccb900,     0,       9,        1,        0,     0x100,       1},         //grad_thres_h
    {0xF8ccb904,    16,      31,        1,        0,     0x100,       1},         //grad_slop
    {0xF8ccb904,     0,       9,        1,        0,     0x100,     140},         //grad_flat_thr2
    /*SR_SKINDT_CTRL*/
    {0xF8ccb908,    31,      31,        1,        0,     0x100,       1},         //skindt_en
    {0xF8ccb908,     0,       7,        1,        0,     0x100,       1},         //clip_data
    {0xF8ccb90c,    20,      29,        1,        0,     0x100,     140},         //v_expect_value
    {0xF8ccb90c,    10,      19,        1,        0,     0x100,       1},         //u_expect_value
    {0xF8ccb90c,     0,       9,        1,        0,     0x100,       1},         //y_expect_value
    /*SR_SKINDT_THR*/
    {0xF8ccb910,    20,      31,        1,        0,     0x100,       1},         //y_slop
    {0xF8ccb910,    10,      19,        1,        0,     0x100,       1},         //y_thres_l
    {0xF8ccb910,     0,       9,        1,        0,     0x100,     140},         //y_thres_h
    {0xF8ccb914,    20,      31,        1,        0,     0x100,       1},         //u_slop
    {0xF8ccb914,    10,      19,        1,        0,     0x100,       1},         //u_thres_l
    {0xF8ccb914,     0,       9,        1,        0,     0x100,       1},         //u_thres_h
    {0xF8ccb918,    20,      31,        1,        0,     0x100,       1},         //v_slop
    {0xF8ccb918,    10,      19,        1,        0,     0x100,     140},         //v_thres_l
    {0xF8ccb918,     0,       9,        1,        0,     0x100,       1},         //v_thres_h
    /*SR_HANCE*/
    {0xF8ccb91c,    16,      27,        1,        0,     0x100,       1},         //sharp_low_slop
    {0xF8ccb91c,     0,      15,        1,        0,     0x100,       1},         //sharp_high_slop
    {0xF8ccb920,    10,      19,        1,        0,     0x100,     140},         //sharp_thres_l
    {0xF8ccb920,     0,       9,        1,        0,     0x100,       1},         //sharp_thres_h
    {0xF8ccb924,    10,      19,        1,        0,     0x100,       1},         //sharp_low_ctrl_l
    {0xF8ccb924,     0,       9,        1,        0,     0x100,       1},         //sharp_low_ctrl_h
    {0xF8ccb928,    10,      18,        1,        0,     0x100,       1},         //sharp_high_ctrl_l
    {0xF8ccb928,     0,       8,        1,        0,     0x100,     140},         //sharp_high_ctrl_h
    /*SR_HANCE_THR*/
    {0xF8ccb92c,    18,      22,        1,        0,     0x100,       1},         //edge_factor_ctrl
    {0xF8ccb92c,    13,      17,        1,        0,     0x100,       1},         //strengh
    {0xF8ccb92c,     5,      12,        1,        0,     0x100,     140},         //sharp_high_start
    {0xF8ccb92c,     0,       4,        1,        0,     0x100,       1},         //sharp_low_start
    {0xF8ccb930,    24,      31,        1,        0,     0x100,       1},         //ring_thres_l
    {0xF8ccb930,    16,      23,        1,        0,     0x100,       1},         //ring_thres_h
    {0xF8ccb930,     5,       9,        1,        0,     0x100,       1},         //skin_thres_l
    {0xF8ccb930,     0,       4,        1,        0,     0x100,     140},         //skin_thres_h
    {0xF8ccb934,    20,      29,        1,        0,     0x100,       1},         //shoot_thrsh
    {0xF8ccb934,    10,      19,        1,        0,     0x100,     140},         //nolinear_ctrl
    {0xF8ccb934,     0,       9,        1,        0,     0x100,       1},         //sharp_high_ctrl
    /*SR_DIR_CTRL*/
    {0xF8ccb938,    10,      19,        1,        0,     0x100,     140},         //dir_dg_ctrl
    {0xF8ccb938,     0,       9,        1,        0,     0x100,       1},         //dir_hv_ctrl

    /*HD Source*/
    /*SR_CTRL*/
    {0xF8ccb880,    29,      30,        2,        0,     0x100,       0},         //sr_disp_state
    {0xF8ccb894,    31,      31,        2,        0,     0x100,       1},         //sharp_en
    {0xF8ccb898,    28,      31,        2,        0,     0x100,       4},         //sharp_coring
    {0xF8ccb898,     0,      10,        2,        0,     0x100,     128},         //sharp_gain
    /*SR_LDIR_RATIO*/
    {0xF8ccb884,    0,        5,        2,        0,     0x100,      14},         //grd_sub_ratio
    /*SR_LTEX_THR*/
    {0xF8ccb88c,    16,      31,        2,        0,     0x100,       4},         //grd_thrh_subl
    {0xF8ccb888,    12,      22,        2,        0,     0x100,      16},         //grd_thrh
    {0xF8ccb888,    0,       10,        2,        0,     0x100,     -48},         //grd_thrl
    /*SR_LTEX_ADD*/
    {0xF8ccb88c,    0,        7,        2,        0,     0x100,       8},         //grd_add_ctrl
    /*SR_DIPL_DETAIL*/
    {0xF8ccb890,    0,       11,        2,        0,     0x100,       0},         //disc_detail_gain
    /*SR_SMOOTH*/
    {0xF8ccb89C,    31,      31,        2,        0,     0x100,       1},         //smooth_en
    {0xF8ccb89C,    30,      30,        2,        0,     0x100,       1},         //plus_en
    {0xF8ccb89C,    0,        9,        2,        0,     0x100,     140},         //plus_ctrl
    /*SR_RINGDT*/
    {0xF8ccb900,    30,      30,        2,        0,     0x100,       1},         //ringdt_en
    {0xF8ccb900,    20,      29,        2,        0,     0x100,       1},         //grad_flat_thr
    {0xF8ccb900,    10,      19,        2,        0,     0x100,     140},         //grad_thres_l
    {0xF8ccb900,     0,       9,        2,        0,     0x100,       1},         //grad_thres_h
    {0xF8ccb904,    16,      31,        2,        0,     0x100,       1},         //grad_slop
    {0xF8ccb904,     0,       9,        2,        0,     0x100,     140},         //grad_flat_thr2
    /*SR_SKINDT_CTRL*/
    {0xF8ccb908,    31,      31,        2,        0,     0x100,       1},         //skindt_en
    {0xF8ccb908,     0,       7,        2,        0,     0x100,       1},         //clip_data
    {0xF8ccb90c,    20,      29,        2,        0,     0x100,     140},         //v_expect_value
    {0xF8ccb90c,    10,      19,        2,        0,     0x100,       1},         //u_expect_value
    {0xF8ccb90c,     0,       9,        2,        0,     0x100,       1},         //y_expect_value
    /*SR_SKINDT_THR*/
    {0xF8ccb910,    20,      31,        2,        0,     0x100,       1},         //y_slop
    {0xF8ccb910,    10,      19,        2,        0,     0x100,       1},         //y_thres_l
    {0xF8ccb910,     0,       9,        2,        0,     0x100,     140},         //y_thres_h
    {0xF8ccb914,    20,      31,        2,        0,     0x100,       1},         //u_slop
    {0xF8ccb914,    10,      19,        2,        0,     0x100,       1},         //u_thres_l
    {0xF8ccb914,     0,       9,        2,        0,     0x100,       1},         //u_thres_h
    {0xF8ccb918,    20,      31,        2,        0,     0x100,       1},         //v_slop
    {0xF8ccb918,    10,      19,        2,        0,     0x100,     140},         //v_thres_l
    {0xF8ccb918,     0,       9,        2,        0,     0x100,       1},         //v_thres_h
    /*SR_HANCE*/
    {0xF8ccb91c,    16,      27,        2,        0,     0x100,       1},         //sharp_low_slop
    {0xF8ccb91c,     0,      15,        2,        0,     0x100,       1},         //sharp_high_slop
    {0xF8ccb920,    10,      19,        2,        0,     0x100,     140},         //sharp_thres_l
    {0xF8ccb920,     0,       9,        2,        0,     0x100,       1},         //sharp_thres_h
    {0xF8ccb924,    10,      19,        2,        0,     0x100,       1},         //sharp_low_ctrl_l
    {0xF8ccb924,     0,       9,        2,        0,     0x100,       1},         //sharp_low_ctrl_h
    {0xF8ccb928,    10,      18,        2,        0,     0x100,       1},         //sharp_high_ctrl_l
    {0xF8ccb928,     0,       8,        2,        0,     0x100,     140},         //sharp_high_ctrl_h
    /*SR_HANCE_THR*/
    {0xF8ccb92c,    18,      22,        2,        0,     0x100,       1},         //edge_factor_ctrl
    {0xF8ccb92c,    13,      17,        2,        0,     0x100,       1},         //strengh
    {0xF8ccb92c,     5,      12,        2,        0,     0x100,     140},         //sharp_high_start
    {0xF8ccb92c,     0,       4,        2,        0,     0x100,       1},         //sharp_low_start
    {0xF8ccb930,    24,      31,        2,        0,     0x100,       1},         //ring_thres_l
    {0xF8ccb930,    16,      23,        2,        0,     0x100,       1},         //ring_thres_h
    {0xF8ccb930,     5,       9,        2,        0,     0x100,       1},         //skin_thres_l
    {0xF8ccb930,     0,       4,        2,        0,     0x100,     140},         //skin_thres_h
    {0xF8ccb934,    20,      29,        2,        0,     0x100,       1},         //shoot_thrsh
    {0xF8ccb934,    10,      19,        2,        0,     0x100,     140},         //nolinear_ctrl
    {0xF8ccb934,     0,       9,        2,        0,     0x100,       1},         //sharp_high_ctrl
    /*SR_DIR_CTRL*/
    {0xF8ccb938,    10,      19,        2,        0,     0x100,     140},         //dir_dg_ctrl
    {0xF8ccb938,     0,       9,        2,        0,     0x100,       1},         //dir_hv_ctrl

    /*UHD Source*/
    /*SR_CTRL*/
    {0xF8ccb880,    29,      30,        3,        0,     0x100,       0},         //sr_disp_state
    {0xF8ccb894,    31,      31,        3,        0,     0x100,       1},         //sharp_en
    {0xF8ccb898,    28,      31,        3,        0,     0x100,       4},         //sharp_coring
    {0xF8ccb898,     0,      10,        3,        0,     0x100,     128},         //sharp_gain
    /*SR_LDIR_RATIO*/
    {0xF8ccb884,    0,        5,        3,        0,     0x100,      14},         //grd_sub_ratio
    /*SR_LTEX_THR*/
    {0xF8ccb88c,    16,      31,        3,        0,     0x100,       4},         //grd_thrh_subl
    {0xF8ccb888,    12,      22,        3,        0,     0x100,      16},         //grd_thrh
    {0xF8ccb888,    0,       10,        3,        0,     0x100,     -48},         //grd_thrl
    /*SR_LTEX_ADD*/
    {0xF8ccb88c,    0,        7,        3,        0,     0x100,       8},         //grd_add_ctrl
    /*SR_DIPL_DETAIL*/
    {0xF8ccb890,    0,       11,        3,        0,     0x100,       0},         //disc_detail_gain
    /*SR_SMOOTH*/
    {0xF8ccb89C,    31,      31,        3,        0,     0x100,       1},         //smooth_en
    {0xF8ccb89C,    30,      30,        3,        0,     0x100,       1},         //plus_en
    {0xF8ccb89C,    0,        9,        3,        0,     0x100,     140},         //plus_ctrl
    /*SR_RINGDT*/
    {0xF8ccb900,    30,      30,        3,        0,     0x100,       1},         //ringdt_en
    {0xF8ccb900,    20,      29,        3,        0,     0x100,       1},         //grad_flat_thr
    {0xF8ccb900,    10,      19,        3,        0,     0x100,     140},         //grad_thres_l
    {0xF8ccb900,     0,       9,        3,        0,     0x100,       1},         //grad_thres_h
    {0xF8ccb904,    16,      31,        3,        0,     0x100,       1},         //grad_slop
    {0xF8ccb904,     0,       9,        3,        0,     0x100,     140},         //grad_flat_thr2
    /*SR_SKINDT_CTRL*/
    {0xF8ccb908,    31,      31,        3,        0,     0x100,       1},         //skindt_en
    {0xF8ccb908,     0,       7,        3,        0,     0x100,       1},         //clip_data
    {0xF8ccb90c,    20,      29,        3,        0,     0x100,     140},         //v_expect_value
    {0xF8ccb90c,    10,      19,        3,        0,     0x100,       1},         //u_expect_value
    {0xF8ccb90c,     0,       9,        3,        0,     0x100,       1},         //y_expect_value
    /*SR_SKINDT_THR*/
    {0xF8ccb910,    20,      31,        3,        0,     0x100,       1},         //y_slop
    {0xF8ccb910,    10,      19,        3,        0,     0x100,       1},         //y_thres_l
    {0xF8ccb910,     0,       9,        3,        0,     0x100,     140},         //y_thres_h
    {0xF8ccb914,    20,      31,        3,        0,     0x100,       1},         //u_slop
    {0xF8ccb914,    10,      19,        3,        0,     0x100,       1},         //u_thres_l
    {0xF8ccb914,     0,       9,        3,        0,     0x100,       1},         //u_thres_h
    {0xF8ccb918,    20,      31,        3,        0,     0x100,       1},         //v_slop
    {0xF8ccb918,    10,      19,        3,        0,     0x100,     140},         //v_thres_l
    {0xF8ccb918,     0,       9,        3,        0,     0x100,       1},         //v_thres_h
    /*SR_HANCE*/
    {0xF8ccb91c,    16,      27,        3,        0,     0x100,       1},         //sharp_low_slop
    {0xF8ccb91c,     0,      15,        3,        0,     0x100,       1},         //sharp_high_slop
    {0xF8ccb920,    10,      19,        3,        0,     0x100,     140},         //sharp_thres_l
    {0xF8ccb920,     0,       9,        3,        0,     0x100,       1},         //sharp_thres_h
    {0xF8ccb924,    10,      19,        3,        0,     0x100,       1},         //sharp_low_ctrl_l
    {0xF8ccb924,     0,       9,        3,        0,     0x100,       1},         //sharp_low_ctrl_h
    {0xF8ccb928,    10,      18,        3,        0,     0x100,       1},         //sharp_high_ctrl_l
    {0xF8ccb928,     0,       8,        3,        0,     0x100,     140},         //sharp_high_ctrl_h
    /*SR_HANCE_THR*/
    {0xF8ccb92c,    18,      22,        3,        0,     0x100,       1},         //edge_factor_ctrl
    {0xF8ccb92c,    13,      17,        3,        0,     0x100,       1},         //strengh
    {0xF8ccb92c,     5,      12,        3,        0,     0x100,     140},         //sharp_high_start
    {0xF8ccb92c,     0,       4,        3,        0,     0x100,       1},         //sharp_low_start
    {0xF8ccb930,    24,      31,        3,        0,     0x100,       1},         //ring_thres_l
    {0xF8ccb930,    16,      23,        3,        0,     0x100,       1},         //ring_thres_h
    {0xF8ccb930,     5,       9,        3,        0,     0x100,       1},         //skin_thres_l
    {0xF8ccb930,     0,       4,        3,        0,     0x100,     140},         //skin_thres_h
    {0xF8ccb934,    20,      29,        3,        0,     0x100,       1},         //shoot_thrsh
    {0xF8ccb934,    10,      19,        3,        0,     0x100,     140},         //nolinear_ctrl
    {0xF8ccb934,     0,       9,        3,        0,     0x100,       1},         //sharp_high_ctrl
    /*SR_DIR_CTRL*/
    {0xF8ccb938,    10,      19,        3,        0,     0x100,     140},         //dir_dg_ctrl
    {0xF8ccb938,     0,       9,        3,        0,     0x100,       1},         //dir_hv_ctrl


    /***************************************************************************
      * Special Register
     ***************************************************************************/
    /*IP_DETECT*/
    {0xfffe0001,    0,       1,        0,        0,        0x0,       0},         //ip_detect_choice
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

