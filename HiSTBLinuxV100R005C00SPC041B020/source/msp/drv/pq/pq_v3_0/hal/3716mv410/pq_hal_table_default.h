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
   Author      : p00203646
 ******************************************************************************/

#ifndef __PQ_HAL_PQ_TABLE_DEFAULT_H__
#define __PQ_HAL_PQ_TABLE_DEFAULT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


static PQ_PHY_REG_S sg_stPhyReg[] =
{
    /*Addr         Lsb       Msb  SourceMode  OutputMode   Module     Value        Description*/

    /****************************************DEI****************************************/
    /*DEI_CTRL*/
    {0xf8cb1000,    31,      31,        0,        0,      0x1,        0},         //die_demo_mode
    {0xf8cb1000,    30,      30,        0,        0,      0x1,        0},         //die_demo_en
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
    {0xf8cb1094,    24,      24,        0,        0,      0x1,        1},         //chroma_ccr_en
    {0xf8cb1094,    16,      23,        0,        0,      0x1,        4},         //chroma_ma_offset
    {0xf8cb1094,    8,       15,        0,        0,      0x1,        8},         //no_ccr_detect_max
    {0xf8cb1094,    0,       7,         0,        0,      0x1,        4},         //no_ccr_detect_thd
    {0xf8cb1098,    20,      26,        0,        0,      0x1,        8},         //similar_max
    {0xf8cb1098,    12,      18,        0,        0,      0x1,        4},         //similar_thd
    {0xf8cb1098,     8,      11,        0,        0,      0x1,        6},         //no_ccr_detect_blend
    {0xf8cb1098,     0,       7,        0,        0,      0x1,      192},         //max_xchroma

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

    /***********************************SHARPEN***************************************/
    /*SD Source-->HD Output*/
    /* SPCTRL */
    {0xF8cc4400,    1,       1,         1,        2,      0x4,        1},         //sharpen_en
    {0xF8cc4400,    0,       0,         1,        2,      0x4,        0},         //debug_en
    {0xF8cc4400,    2,       3,         1,        2,      0x4,        0},         //demo_mode
    {0xF8cc4400,    4,       16,        1,        2,      0x4,        0},         //demo_pos
    /* SPENABLE */
    {0xF8cc4404,    4,       4,         1,        2,      0x4,        1},         //hfreq_en_d
    {0xF8cc4404,    3,       3,         1,        2,      0x4,        1},         //hfreq_en_v
    {0xF8cc4404,    2,       2,         1,        2,      0x4,        1},         //hfreq_en_h
    {0xF8cc4404,    1,       1,         1,        2,      0x4,        1},         //detec_en
    {0xF8cc4404,    0,       0,         1,        2,      0x4,        1},         //peak_en
    /* SPWINSIZEHVD */
    {0xF8cc4408,    6,       8,         1,        2,      0x4,        2},         //winsize_d
    {0xF8cc4408,    3,       5,         1,        2,      0x4,        2},         //winsize_v
    {0xF8cc4408,    0,       2,         1,        2,      0x4,        2},         //winsize_h
    /* SPTMPH1 */
    {0xF8cc440c,    16,      24,        1,        2,      0x4,       78},         //tmp2_h1
    {0xF8cc440c,    8,       15,        1,        2,      0x4,       61},         //tmp1_h1
    {0xF8cc440c,    0,       7,         1,        2,      0x4,       28},         //tmp0_h1
    /* SPTMPH2 */
    {0xF8cc4410,    16,      24,        1,        2,      0x4,      128},         //tmp2_h2
    {0xF8cc4410,    8,       15,        1,        2,      0x4,        0},         //tmp1_h2
    {0xF8cc4410,    0,       7,         1,        2,      0x4,      -64},         //tmp0_h2
    /* SPTMPV1 */
    {0xF8cc4414,    16,      24,        1,        2,      0x4,       78},         //tmp2_v1
    {0xF8cc4414,    8,       15,        1,        2,      0x4,       61},         //tmp1_v1
    {0xF8cc4414,    0,       7,         1,        2,      0x4,       28},         //tmp0_v1
    /* SPTMPV2 */
    {0xF8cc4418,    16,      24,        1,        2,      0x4,      128},         //tmp2_v2
    {0xF8cc4418,    8,       15,        1,        2,      0x4,        0},         //tmp1_v2
    {0xF8cc4418,    0,       7,         1,        2,      0x4,      -64},         //tmp0_v2
    /* SPGAIN */
    {0xF8cc441c,    20,      27,        1,        2,      0x4,        8},         //dir_gain
    {0xF8cc441c,    10,      19,        1,        2,      0x4,      128},         //edge_gain  352
    {0xF8cc441c,    0,       9,         1,        2,      0x4,      256},         //peak_gain  352
    {0xF8cc4420,    0,       7,         1,        2,      0x4,       20},         //peak_coring
    /* SPPEAKSHOOTCTL */
    {0xF8cc4424,    16,      23,        1,        2,      0x4,       96},         //peak_mixratio
    {0xF8cc4424,    8,       15,        1,        2,      0x4,       30},         //peak_under_thr
    {0xF8cc4424,    0,       7,         1,        2,      0x4,       30},         //peak_over_thr
    /* SPGAINPOS */
    {0xF8cc4428,    18,      26,        1,        2,      0x4,      128},         //gain_pos_d
    {0xF8cc4428,    9,       17,        1,        2,      0x4,      128},         //gain_pos_v
    {0xF8cc4428,    0,       8,         1,        2,      0x4,      128},         //gain_pos_h
    /* SPGAINNEG */
    {0xF8cc442c,    18,      26,        1,        2,      0x4,      128},         //gain_neg_d
    {0xF8cc442c,    9,       17,        1,        2,      0x4,      128},         //gain_neg_v
    {0xF8cc442c,    0,       8,         1,        2,      0x4,      128},         //gain_neg_h
    /* SPOVERTHR */
    {0xF8cc4430,    16,      23,        1,        2,      0x4,       20},         //over_thr_d
    {0xF8cc4430,    8,       15,        1,        2,      0x4,       20},         //over_thr_v
    {0xF8cc4430,    0,       7,         1,        2,      0x4,       20},         //over_thr_h
    /* SPUNDERTHR */
    {0xF8cc4434,    16,      23,        1,        2,      0x4,       20},         //under_thr_d
    {0xF8cc4434,    8,       15,        1,        2,      0x4,       20},         //under_thr_v
    {0xF8cc4434,    0,       7,         1,        2,      0x4,       20},         //under_thr_h
    /* SPMIXRATIO */
    {0xF8cc4438,    16,      23,        1,        2,      0x4,       96},         //mixratio_d
    {0xF8cc4438,    8,       15,        1,        2,      0x4,       96},         //mixratio_v
    {0xF8cc4438,    0,       7,         1,        2,      0x4,       96},         //mixratio_h
    /* SPCORINGZERO */
    {0xF8cc443c,    16,      23,        1,        2,      0x4,        1},         //coringzero_d
    {0xF8cc443c,    8,       15,        1,        2,      0x4,        1},         //coringzero_v
    {0xF8cc443c,    0,       7,         1,        2,      0x4,        1},         //coringzero_h
    /* SPCORINGRATIO */
    {0xF8cc4440,    10,      14,        1,        2,      0x4,       12},         //coringratio_d
    {0xF8cc4440,    5,       9,         1,        2,      0x4,       12},         //coringratio_v
    {0xF8cc4440,    0,       4,         1,        2,      0x4,       12},         //coringratio_h
    /* SPCORINGTHR */
    {0xF8cc4444,    16,      23,        1,        2,      0x4,        8},         //coringthr_d
    {0xF8cc4444,    8,       15,        1,        2,      0x4,        8},         //coringthr_v
    {0xF8cc4444,    0,       7,         1,        2,      0x4,        8},         //coringthr_h
    /* SPLMTRATIO */
    {0xF8cc4448,    6,       8,         1,        2,      0x4,        6},         //limit_ratio_d
    {0xF8cc4448,    3,       5,         1,        2,      0x4,        6},         //limit_ratio_v
    {0xF8cc4448,    0,       2,         1,        2,      0x4,        6},         //limit_ratio_h
    /* SPLMTPOS0 */
    {0xF8cc444c,    16,      23,        1,        2,      0x4,        64},         //limitpos0_d
    {0xF8cc444c,    8,       15,        1,        2,      0x4,        64},         //limitpos0_v
    {0xF8cc444c,    0,       7,         1,        2,      0x4,        64},         //limitpos0_h
    /* SPLMTPOS1 */
    {0xF8cc4450,    16,      23,        1,        2,      0x4,       128},         //limitpos1_d
    {0xF8cc4450,    8,       15,        1,        2,      0x4,       128},         //limitpos1_v
    {0xF8cc4450,    0,       7,         1,        2,      0x4,       128},         //limitpos1_h
    /* SPBOUNDPOS */
    {0xF8cc4454,    16,      23,        1,        2,      0x4,       112},         //boundpos_d
    {0xF8cc4454,    8,       15,        1,        2,      0x4,       112},         //boundpos_v
    {0xF8cc4454,    0,       7,         1,        2,      0x4,       112},         //boundpos_h
    /* SPLMTNEG0 */
    {0xF8cc4458,    18,      26,        1,        2,      0x4,       -64},         //limitneg0_d
    {0xF8cc4458,    9,       17,        1,        2,      0x4,       -64},         //limitneg0_v
    {0xF8cc4458,    0,       8,         1,        2,      0x4,       -64},         //limitneg0_h
    /* SPLMTNEG1 */
    {0xF8cc445c,    18,      26,        1,        2,      0x4,      -128},         //limitneg1_d
    {0xF8cc445c,    9,       17,        1,        2,      0x4,      -128},         //limitneg1_v
    {0xF8cc445c,    0,       8,         1,        2,      0x4,      -128},         //limitneg1_h
    /* SPBOUNDNEG */
    {0xF8cc4460,    18,      26,        1,        2,      0x4,      -112},         //boundneg_d
    {0xF8cc4460,    9,       17,        1,        2,      0x4,      -112},         //boundneg_v
    {0xF8cc4460,    0,       8,         1,        2,      0x4,      -112},         //boundneg_h
    /* SPEDGESHOOTCTL */
    {0xF8cc4464,    16,      23,        1,        2,      0x4,       96},         //edge_mixratio
    {0xF8cc4464,    8,       15,        1,        2,      0x4,       20},         //edge_under_thr
    {0xF8cc4464,    0,       7,         1,        2,      0x4,       20},         //edge_over_thr
    /* SPEDGETMPH */
    {0xF8cc4468,    16,      24,        1,        2,      0x4,      128},         //tmp2_sx
    {0xF8cc4468,    8,       15,        1,        2,      0x4,        0},         //tmp1_sx
    {0xF8cc4468,    0,       7,         1,        2,      0x4,      -64},         //tmp0_sx
    /* SPEDGETMPV */
    {0xF8cc446c,    16,      24,        1,        2,      0x4,      128},         //tmp2_sy
    {0xF8cc446c,    8,       15,        1,        2,      0x4,        0},         //tmp1_sy
    {0xF8cc446c,    0,       7,         1,        2,      0x4,      -64},         //tmp0_sy
    /* SPEDGEEI */
    {0xF8cc4470,    16,      23,        1,        2,      0x4,        4},         //edge_eiratio
    {0xF8cc4470,    8,       15,        1,        2,      0x4,       70},         //edge_ei1
    {0xF8cc4470,    0,       7,         1,        2,      0x4,       38},         //edge_ei0

    /*HD Source-->HD Output*/
    /* SPCTRL */
    {0xF8cc4400,    1,       1,         2,        2,      0x4,        0},         //sharpen_en
    {0xF8cc4400,    0,       0,         2,        2,      0x4,        0},         //debug_en
    {0xF8cc4400,    2,       3,         2,        2,      0x4,        0},         //demo_mode
    {0xF8cc4400,    4,       16,        2,        2,      0x4,        0},         //demo_pos
    /* SPENABLE */
    {0xF8cc4404,    4,       4,         2,        2,      0x4,        1},         //hfreq_en_d
    {0xF8cc4404,    3,       3,         2,        2,      0x4,        1},         //hfreq_en_v
    {0xF8cc4404,    2,       2,         2,        2,      0x4,        1},         //hfreq_en_h
    {0xF8cc4404,    1,       1,         2,        2,      0x4,        1},         //detec_en
    {0xF8cc4404,    0,       0,         2,        2,      0x4,        1},         //peak_en
    /* SPWINSIZEHVD */
    {0xF8cc4408,    6,       8,         2,        2,      0x4,        2},         //winsize_d
    {0xF8cc4408,    3,       5,         2,        2,      0x4,        2},         //winsize_v
    {0xF8cc4408,    0,       2,         2,        2,      0x4,        2},         //winsize_h
    /* SPTMPH1 */
    {0xF8cc440c,    16,      24,        2,        2,      0x4,       78},         //tmp2_h1
    {0xF8cc440c,    8,       15,        2,        2,      0x4,       61},         //tmp1_h1
    {0xF8cc440c,    0,       7,         2,        2,      0x4,       28},         //tmp0_h1
    /* SPTMPH2 */
    {0xF8cc4410,    16,      24,        2,        2,      0x4,      128},         //tmp2_h2
    {0xF8cc4410,    8,       15,        2,        2,      0x4,        0},         //tmp1_h2
    {0xF8cc4410,    0,       7,         2,        2,      0x4,      -64},         //tmp0_h2
    /* SPTMPV1 */
    {0xF8cc4414,    16,      24,        2,        2,      0x4,       78},         //tmp2_v1
    {0xF8cc4414,    8,       15,        2,        2,      0x4,       61},         //tmp1_v1
    {0xF8cc4414,    0,       7,         2,        2,      0x4,       28},         //tmp0_v1
    /* SPTMPV2 */
    {0xF8cc4418,    16,      24,        2,        2,      0x4,      128},         //tmp2_v2
    {0xF8cc4418,    8,       15,        2,        2,      0x4,        0},         //tmp1_v2
    {0xF8cc4418,    0,       7,         2,        2,      0x4,      -64},         //tmp0_v2
    /* SPGAIN */
    {0xF8cc441c,    20,      27,        2,        2,      0x4,        8},         //dir_gain
    {0xF8cc441c,    10,      19,        2,        2,      0x4,      128},         //edge_gain  352
    {0xF8cc441c,    0,       9,         2,        2,      0x4,      256},         //peak_gain  352
    {0xF8cc4420,    0,       7,         2,        2,      0x4,       20},         //peak_coring
    /* SPPEAKSHOOTCTL */
    {0xF8cc4424,    16,      23,        2,        2,      0x4,       96},         //peak_mixratio
    {0xF8cc4424,    8,       15,        2,        2,      0x4,       30},         //peak_under_thr
    {0xF8cc4424,    0,       7,         2,        2,      0x4,       30},         //peak_over_thr
    /* SPGAINPOS */
    {0xF8cc4428,    18,      26,        2,        2,      0x4,      128},         //gain_pos_d
    {0xF8cc4428,    9,       17,        2,        2,      0x4,      128},         //gain_pos_v
    {0xF8cc4428,    0,       8,         2,        2,      0x4,      128},         //gain_pos_h
    /* SPGAINNEG */
    {0xF8cc442c,    18,      26,        2,        2,      0x4,      128},         //gain_neg_d
    {0xF8cc442c,    9,       17,        2,        2,      0x4,      128},         //gain_neg_v
    {0xF8cc442c,    0,       8,         2,        2,      0x4,      128},         //gain_neg_h
    /* SPOVERTHR */
    {0xF8cc4430,    16,      23,        2,        2,      0x4,       20},         //over_thr_d
    {0xF8cc4430,    8,       15,        2,        2,      0x4,       20},         //over_thr_v
    {0xF8cc4430,    0,       7,         2,        2,      0x4,       20},         //over_thr_h
    /* SPUNDERTHR */
    {0xF8cc4434,    16,      23,        2,        2,      0x4,       20},         //under_thr_d
    {0xF8cc4434,    8,       15,        2,        2,      0x4,       20},         //under_thr_v
    {0xF8cc4434,    0,       7,         2,        2,      0x4,       20},         //under_thr_h
    /* SPMIXRATIO */
    {0xF8cc4438,    16,      23,        2,        2,      0x4,       96},         //mixratio_d
    {0xF8cc4438,    8,       15,        2,        2,      0x4,       96},         //mixratio_v
    {0xF8cc4438,    0,       7,         2,        2,      0x4,       96},         //mixratio_h
    /* SPCORINGZERO */
    {0xF8cc443c,    16,      23,        2,        2,      0x4,        1},         //coringzero_d
    {0xF8cc443c,    8,       15,        2,        2,      0x4,        1},         //coringzero_v
    {0xF8cc443c,    0,       7,         2,        2,      0x4,        1},         //coringzero_h
    /* SPCORINGRATIO */
    {0xF8cc4440,    10,      14,        2,        2,      0x4,       12},         //coringratio_d
    {0xF8cc4440,    5,       9,         2,        2,      0x4,       12},         //coringratio_v
    {0xF8cc4440,    0,       4,         2,        2,      0x4,       12},         //coringratio_h
    /* SPCORINGTHR */
    {0xF8cc4444,    16,      23,        2,        2,      0x4,        8},         //coringthr_d
    {0xF8cc4444,    8,       15,        2,        2,      0x4,        8},         //coringthr_v
    {0xF8cc4444,    0,       7,         2,        2,      0x4,        8},         //coringthr_h
    /* SPLMTRATIO */
    {0xF8cc4448,    6,       8,         2,        2,      0x4,        6},         //limit_ratio_d
    {0xF8cc4448,    3,       5,         2,        2,      0x4,        6},         //limit_ratio_v
    {0xF8cc4448,    0,       2,         2,        2,      0x4,        6},         //limit_ratio_h
    /* SPLMTPOS0 */
    {0xF8cc444c,    16,      23,        2,        2,      0x4,        64},         //limitpos0_d
    {0xF8cc444c,    8,       15,        2,        2,      0x4,        64},         //limitpos0_v
    {0xF8cc444c,    0,       7,         2,        2,      0x4,        64},         //limitpos0_h
    /* SPLMTPOS1 */
    {0xF8cc4450,    16,      23,        2,        2,      0x4,       128},         //limitpos1_d
    {0xF8cc4450,    8,       15,        2,        2,      0x4,       128},         //limitpos1_v
    {0xF8cc4450,    0,       7,         2,        2,      0x4,       128},         //limitpos1_h
    /* SPBOUNDPOS */
    {0xF8cc4454,    16,      23,        2,        2,      0x4,       112},         //boundpos_d
    {0xF8cc4454,    8,       15,        2,        2,      0x4,       112},         //boundpos_v
    {0xF8cc4454,    0,       7,         2,        2,      0x4,       112},         //boundpos_h
    /* SPLMTNEG0 */
    {0xF8cc4458,    18,      26,        2,        2,      0x4,       -64},         //limitneg0_d
    {0xF8cc4458,    9,       17,        2,        2,      0x4,       -64},         //limitneg0_v
    {0xF8cc4458,    0,       8,         2,        2,      0x4,       -64},         //limitneg0_h
    /* SPLMTNEG1 */
    {0xF8cc445c,    18,      26,        2,        2,      0x4,      -128},         //limitneg1_d
    {0xF8cc445c,    9,       17,        2,        2,      0x4,      -128},         //limitneg1_v
    {0xF8cc445c,    0,       8,         2,        2,      0x4,      -128},         //limitneg1_h
    /* SPBOUNDNEG */
    {0xF8cc4460,    18,      26,        2,        2,      0x4,      -112},         //boundneg_d
    {0xF8cc4460,    9,       17,        2,        2,      0x4,      -112},         //boundneg_v
    {0xF8cc4460,    0,       8,         2,        2,      0x4,      -112},         //boundneg_h
    /* SPEDGESHOOTCTL */
    {0xF8cc4464,    16,      23,        2,        2,      0x4,       96},         //edge_mixratio
    {0xF8cc4464,    8,       15,        2,        2,      0x4,       20},         //edge_under_thr
    {0xF8cc4464,    0,       7,         2,        2,      0x4,       20},         //edge_over_thr
    /* SPEDGETMPH */
    {0xF8cc4468,    16,      24,        2,        2,      0x4,      128},         //tmp2_sx
    {0xF8cc4468,    8,       15,        2,        2,      0x4,        0},         //tmp1_sx
    {0xF8cc4468,    0,       7,         2,        2,      0x4,      -64},         //tmp0_sx
    /* SPEDGETMPV */
    {0xF8cc446c,    16,      24,        2,        2,      0x4,      128},         //tmp2_sy
    {0xF8cc446c,    8,       15,        2,        2,      0x4,        0},         //tmp1_sy
    {0xF8cc446c,    0,       7,         2,        2,      0x4,      -64},         //tmp0_sy
    /* SPEDGEEI */
    {0xF8cc4470,    16,      23,        2,        2,      0x4,        4},         //edge_eiratio
    {0xF8cc4470,    8,       15,        2,        2,      0x4,       70},         //edge_ei1
    {0xF8cc4470,    0,       7,         2,        2,      0x4,       38},         //edge_ei0

    /*************************************HD_DB*****************************************/
    /*DB_CTRL*/
    {0xf8cb0000,    11,      11,        0,        0,    0x200,        1},         //dbm_en
    {0xf8cb2500,    10,      10,        0,        0,    0x200,        0},         //dbm_demo_en
    {0xf8cb2500,    11,      11,        0,        0,    0x200,        0},         //dbm_demo_mode
    {0xf8cb2500,    15,      15,        0,        0,    0x200,        1},         //det_vy_en
    {0xf8cb2500,    14,      14,        0,        0,    0x200,        1},         //det_hc_en
    {0xf8cb2500,    13,      13,        0,        0,    0x200,        1},         //det_hy_en
    {0xf8cb2500,    12,      12,        0,        0,    0x200,        1},         //det_size_en
    {0xf8cb2500,     4,       4,        0,        0,    0x200,        1},         //db_chr_hor_mid_grey_en
    {0xf8cb2500,     3,       3,        0,        0,    0x200,        1},         //db_chr_hor_en
    {0xf8cb2500,     2,       2,        0,        0,    0x200,        0},         //db_chr_ver_en
    {0xf8cb2500,     1,       1,        0,        0,    0x200,        1},         //db_lum_hor_en
    {0xf8cb2500,     0,       0,        0,        0,    0x200,        1},         //db_lum_ver_en
    {0xf8cb2500,    18,      23,        0,        0,    0x200,        4},         //dbm_out_sel_gain
    {0xf8cb2500,    16,      17,        0,        0,    0x200,        3},         //dbm_out_mode
    /*DB_DIR*/
    {0xf8cb2504,     8,      12,        0,        0,    0x200,        2},         //ctrst_thresh
    {0xf8cb2504,     0,       4,        0,        0,    0x200,       24},         //grad_sub_ratio
    /*DB_BLK*/
    {0xf8cb2508,    24,      30,        0,        0,    0x200,        0},         //chr_v_blk_size
    {0xf8cb2508,    16,      22,        0,        0,    0x200,        8},         //chr_h_blk_size
    {0xf8cb2508,     8,      14,        0,        0,    0x200,        8},         //lum_v_blk_size
    {0xf8cb2508,     0,       6,        0,        0,    0x200,        8},         //lum_h_blk_size
    /*DB_RATIO*/
    {0xf8cb250c,    20,      21,        0,        0,    0x200,        2},         //lum_hor_txt_win_size
    {0xf8cb250c,    16,      19,        0,        0,    0x200,        8},         //global_db_strenth_chr
    {0xf8cb250c,    12,      15,        0,        0,    0x200,        8},         //global_db_strenth_lum
    {0xf8cb250c,    10,      11,        0,        0,    0x200,        1},         //chr_hor_filter_sel
    {0xf8cb250c,     8,       9,        0,        0,    0x200,        1},         //lum_hor_filter_sel
    {0xf8cb250c,     6,       7,        0,        0,    0x200,        0},         //chr_ver_scale_ratio
    {0xf8cb250c,     4,       5,        0,        0,    0x200,        1},         //chr_hor_scale_ratio
    {0xf8cb250c,     2,       3,        0,        0,    0x200,        3},         //lum_ver_scale_ratio
    {0xf8cb250c,     0,       1,        0,        0,    0x200,        3},         //lum_hor_scale_ratio
    /*DB_LHHF*/
    {0xf8cb2510,    28,      31,        0,        0,    0x200,        2},         //lum_hor_hf_var_gain2
    {0xf8cb2510,    24,      27,        0,        0,    0x200,        1},         //lum_hor_hf_var_gain1
    {0xf8cb2510,    16,      22,        0,        0,    0x200,        0},         //lum_hor_hf_var_core
    {0xf8cb2510,    12,      15,        0,        0,    0x200,        8},         //lum_hor_hf_diff_gain2
    {0xf8cb2510,     8,      11,        0,        0,    0x200,        0},         //lum_hor_hf_diff_gain1
    {0xf8cb2510,     0,       6,        0,        0,    0x200,        0},         //lum_hor_hf_diff_core
    /*DB_LVHF*/
    {0xf8cb2514,    24,      30,        0,        0,    0x200,        0},         //lum_ver_hf_var_core
    {0xf8cb2514,    21,      23,        0,        0,    0x200,        7},         //lum_hor_bord_adj_gain
    {0xf8cb2514,    16,      20,        0,        0,    0x200,       12},         //lum_hor_adj_gain
    {0xf8cb2514,    12,      15,        0,        0,    0x200,        4},         //lum_ver_hf_var_gain2
    {0xf8cb2514,     8,      11,        0,        0,    0x200,        8},         //lum_ver_hf_var_gain1
    {0xf8cb2514,     5,       7,        0,        0,    0x200,        0},         //lum_ver_bord_adj_gain
    {0xf8cb2514,     0,       4,        0,        0,    0x200,        4},         //lum_ver_adj_gain
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
    {0xf8cb2524,    28,      31,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p7
    {0xf8cb2524,    24,      27,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p6
    {0xf8cb2524,    20,      23,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p5
    {0xf8cb2524,    16,      19,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p4
    {0xf8cb2524,    12,      15,        0,        0,    0x200,        2},         //lum_ver_delta_lut_p3
    {0xf8cb2524,     8,      11,        0,        0,    0x200,        4},         //lum_ver_delta_lut_p2
    {0xf8cb2524,     4,       7,        0,        0,    0x200,        6},         //lum_ver_delta_lut_p1
    {0xf8cb2524,     0,       3,        0,        0,    0x200,        0},         //lum_ver_delta_lut_p0
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
    {0xf8cb2538,     3,       5,        0,        0,    0x200,        1},         //lum_v_str_fade_lut_p1
    {0xf8cb2538,     0,       2,        0,        0,    0x200,        2},         //lum_v_str_fade_lut_p0
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
    /*DB_CHSF_LUT*/
    {0xf8cb254c,    28,      31,        0,        0,    0x200,        0},         //dir_str_lut_p15
    {0xf8cb254c,    24,      27,        0,        0,    0x200,        0},         //dir_str_lut_p14
    {0xf8cb254c,    20,      23,        0,        0,    0x200,        0},         //dir_str_lut_p13
    {0xf8cb254c,    16,      19,        0,        0,    0x200,        0},         //dir_str_lut_p12
    {0xf8cb254c,    12,      15,        0,        0,    0x200,        0},         //dir_str_lut_p11
    {0xf8cb254c,     8,      11,        0,        0,    0x200,        0},         //dir_str_lut_p10
    {0xf8cb254c,     4,       7,        0,        0,    0x200,        1},         //dir_str_lut_p9
    {0xf8cb254c,     0,       3,        0,        0,    0x200,        1},         //dir_str_lut_p8
    {0xf8cb2548,    28,      31,        0,        0,    0x200,        6},         //dir_str_lut_p7
    {0xf8cb2548,    24,      27,        0,        0,    0x200,        8},         //dir_str_lut_p6
    {0xf8cb2548,    20,      23,        0,        0,    0x200,        8},         //dir_str_lut_p5
    {0xf8cb2548,    16,      19,        0,        0,    0x200,        8},         //dir_str_lut_p4
    {0xf8cb2548,    12,      15,        0,        0,    0x200,        8},         //dir_str_lut_p3
    {0xf8cb2548,     8,      11,        0,        0,    0x200,        8},         //dir_str_lut_p2
    {0xf8cb2548,     4,       7,        0,        0,    0x200,        8},         //dir_str_lut_p1
    {0xf8cb2548,     0,       3,        0,        0,    0x200,        8},         //dir_str_lut_p0
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
    /*DBD_TSTBLKNUM*/
    {0xf8cb2564,    20,      28,        0,        0,    0x200,       60},         //vy_tst_blk_num
    {0xf8cb2564,    10,      18,        0,        0,    0x200,       45},         //hc_tst_blk_num
    {0xf8cb2564,     0,       8,        0,        0,    0x200,       90},         //hy_tst_blk_num
    
    /*DBD_RTNBDTXT*/
    {0xf8cb2560,    16,      20,        1,        0,    0x200,       16},         //vy_rtn_bd_txt
    {0xf8cb2560,     8,      12,        1,        0,    0x200,       16},         //hc_rtn_bd_txt
    {0xf8cb2560,     0,       4,        1,        0,    0x200,       20},         //hy_rtn_bd_txt
    /*DBD_HYRDX8BINLUT*/
    {0xf8cb2574,     0,      10,        1,        0,    0x200,      208},         //hy_rdx8bin_lut6
    {0xf8cb2570,    12,      22,        1,        0,    0x200,      176},         //hy_rdx8bin_lut5
    {0xf8cb2570,     0,      10,        1,        0,    0x200,      144},         //hy_rdx8bin_lut4
    {0xf8cb256c,    12,      22,        1,        0,    0x200,      112},         //hy_rdx8bin_lut3
    {0xf8cb256c,     0,      10,        1,        0,    0x200,       80},         //hy_rdx8bin_lut2
    {0xf8cb2568,    12,      22,        1,        0,    0x200,       48},         //hy_rdx8bin_lut1
    {0xf8cb2568,     0,      10,        1,        0,    0x200,       16},         //hy_rdx8bin_lut0
    /*DBD_HCRDX8BINLUT*/
    {0xf8cb2584,     0,      10,        1,        0,    0x200,      208},         //hc_rdx8bin_lut6
    {0xf8cb2580,    12,      22,        1,        0,    0x200,      176},         //hc_rdx8bin_lut5
    {0xf8cb2580,     0,      10,        1,        0,    0x200,      144},         //hc_rdx8bin_lut4
    {0xf8cb257c,    12,      22,        1,        0,    0x200,      112},         //hc_rdx8bin_lut3
    {0xf8cb257c,     0,      10,        1,        0,    0x200,       80},         //hc_rdx8bin_lut2
    {0xf8cb2578,    12,      22,        1,        0,    0x200,       48},         //hc_rdx8bin_lut1
    {0xf8cb2578,     0,      10,        1,        0,    0x200,       16},         //hc_rdx8bin_lut0
    /*DBD_VYRDX8BINLUT*/
    {0xf8cb2594,     0,      10,        1,        0,    0x200,      208},         //vy_rdx8bin_lut6
    {0xf8cb2590,    12,      22,        1,        0,    0x200,      176},         //vy_rdx8bin_lut5
    {0xf8cb2590,     0,      10,        1,        0,    0x200,      144},         //vy_rdx8bin_lut4
    {0xf8cb258c,    12,      22,        1,        0,    0x200,      112},         //vy_rdx8bin_lut3
    {0xf8cb258c,     0,      10,        1,        0,    0x200,       80},         //vy_rdx8bin_lut2
    {0xf8cb2588,    12,      22,        1,        0,    0x200,       48},         //vy_rdx8bin_lut1
    {0xf8cb2588,     0,      10,        1,        0,    0x200,       16},         //vy_rdx8bin_lut0
    /*DBD_BLKSIZE*/
    {0xf8cb2598,    24,      29,        1,        0,    0x200,       16},         //vy_max_blk_size
    {0xf8cb2598,    16,      21,        1,        0,    0x200,       16},         //hc_max_blk_size
    {0xf8cb2598,     8,      13,        1,        0,    0x200,       16},         //hy_max_blk_size
    {0xf8cb2598,     0,       5,        1,        0,    0x200,        6},         //min_blk_size

    /*DBD_RTNBDTXT*/
    {0xf8cb2560,    16,      20,        2,        0,    0x200,       16},         //vy_rtn_bd_txt
    {0xf8cb2560,     8,      12,        2,        0,    0x200,       16},         //hc_rtn_bd_txt
    {0xf8cb2560,     0,       4,        2,        0,    0x200,       20},         //hy_rtn_bd_txt
    /*DBD_HYRDX8BINLUT*/
    {0xf8cb2574,     0,      10,        2,        0,    0x200,      208},         //hy_rdx8bin_lut6
    {0xf8cb2570,    12,      22,        2,        0,    0x200,      176},         //hy_rdx8bin_lut5
    {0xf8cb2570,     0,      10,        2,        0,    0x200,      144},         //hy_rdx8bin_lut4
    {0xf8cb256c,    12,      22,        2,        0,    0x200,      112},         //hy_rdx8bin_lut3
    {0xf8cb256c,     0,      10,        2,        0,    0x200,       80},         //hy_rdx8bin_lut2
    {0xf8cb2568,    12,      22,        2,        0,    0x200,       48},         //hy_rdx8bin_lut1
    {0xf8cb2568,     0,      10,        2,        0,    0x200,       16},         //hy_rdx8bin_lut0
    /*DBD_HCRDX8BINLUT*/
    {0xf8cb2584,     0,      10,        2,        0,    0x200,      208},         //hc_rdx8bin_lut6
    {0xf8cb2580,    12,      22,        2,        0,    0x200,      176},         //hc_rdx8bin_lut5
    {0xf8cb2580,     0,      10,        2,        0,    0x200,      144},         //hc_rdx8bin_lut4
    {0xf8cb257c,    12,      22,        2,        0,    0x200,      112},         //hc_rdx8bin_lut3
    {0xf8cb257c,     0,      10,        2,        0,    0x200,       80},         //hc_rdx8bin_lut2
    {0xf8cb2578,    12,      22,        2,        0,    0x200,       48},         //hc_rdx8bin_lut1
    {0xf8cb2578,     0,      10,        2,        0,    0x200,       16},         //hc_rdx8bin_lut0
    /*DBD_VYRDX8BINLUT*/
    {0xf8cb2594,     0,      10,        2,        0,    0x200,      208},         //vy_rdx8bin_lut6
    {0xf8cb2590,    12,      22,        2,        0,    0x200,      176},         //vy_rdx8bin_lut5
    {0xf8cb2590,     0,      10,        2,        0,    0x200,      144},         //vy_rdx8bin_lut4
    {0xf8cb258c,    12,      22,        2,        0,    0x200,      112},         //vy_rdx8bin_lut3
    {0xf8cb258c,     0,      10,        2,        0,    0x200,       80},         //vy_rdx8bin_lut2
    {0xf8cb2588,    12,      22,        2,        0,    0x200,       48},         //vy_rdx8bin_lut1
    {0xf8cb2588,     0,      10,        2,        0,    0x200,       16},         //vy_rdx8bin_lut0
    /*DBD_BLKSIZE*/
    {0xf8cb2598,    24,      29,        2,        0,    0x200,       16},         //vy_max_blk_size
    {0xf8cb2598,    16,      21,        2,        0,    0x200,       16},         //hc_max_blk_size
    {0xf8cb2598,     8,      13,        2,        0,    0x200,       16},         //hy_max_blk_size
    {0xf8cb2598,     0,       5,        2,        0,    0x200,        6},         //min_blk_size

    /************************************* DM *****************************************/
    /*DM_CTRL*/
    {0xf8cb2500,     5,       5,        0,        0,    0x400,        1},         //dm_en
    {0xF8cb25a0,     6,       7,        0,        0,    0x400,        2},         //dm_ctrst_thresh
    {0xF8cb25a0,     0,       4,        0,        0,    0x400,       24},         //dm_grad_sub_ratio
    {0xF8cb25a0,     8,      11,        0,        0,    0x400,        4},         //dm_str_idx_t
    {0xF8cb25a0,    12,      19,        0,        0,    0x400,        8},         //dm_limit_t
    /*DM_EDGE*/
    {0xf8cb25a4,    26,      29,        0,        0,    0x400,        0},         //dm_str_idx
    {0xF8cb25a4,    22,      25,        0,        0,    0x400,        8},         //dm_global_str
    {0xF8cb25a4,    18,      20,        0,        0,    0x400,        2},         //dm_sw_thr_gain
    {0xF8cb25a4,    14,      16,        0,        0,    0x400,        1},         //dm_edge_gain2
    {0xF8cb25a4,    10,      12,        0,        0,    0x400,        0},         //dm_edge_gain1
    {0xF8cb25a4,     6,       9,        0,        0,    0x400,        0},         //dm_asymtrc_gain
    {0xF8cb25a4,     0,       5,        0,        0,    0x400,       60},         //dm_edge_thr
    /*DM_LUT1*/
    {0xf8cb25ac,     0,       6,        0,        0,    0x400,       64},         //dm_sw_wht_lut_p4
    {0xF8cb25a8,    24,      30,        0,        0,    0x400,       32},         //dm_sw_wht_lut_p3
    {0xF8cb25a8,    16,      22,        0,        0,    0x400,       16},         //dm_sw_wht_lut_p2
    {0xF8cb25a8,     8,      14,        0,        0,    0x400,        8},         //dm_sw_wht_lut_p1
    {0xF8cb25a8,     0,       6,        0,        0,    0x400,        4},         //dm_sw_wht_lut_p0
    /*DM_LUT2*/
    {0xf8cb25b0,    28,      31,        0,        0,    0x400,        5},         //dm_dir_str_gain_lut_p7
    {0xf8cb25b0,    24,      27,        0,        0,    0x400,        6},         //dm_dir_str_gain_lut_p6
    {0xf8cb25b0,    20,      23,        0,        0,    0x400,        8},         //dm_dir_str_gain_lut_p5
    {0xf8cb25b0,    16,      19,        0,        0,    0x400,        8},         //dm_dir_str_gain_lut_p4
    {0xf8cb25b0,    12,      15,        0,        0,    0x400,        8},         //dm_dir_str_gain_lut_p3
    {0xf8cb25b0,     8,      11,        0,        0,    0x400,        8},         //dm_dir_str_gain_lut_p2
    {0xf8cb25b0,     4,       7,        0,        0,    0x400,        8},         //dm_dir_str_gain_lut_p1
    {0xf8cb25b0,     0,       3,        0,        0,    0x400,        8},         //dm_dir_str_gain_lut_p0
    /*DM_LUT3*/
    {0xf8cb25b8,    28,      31,        0,        0,    0x400,        0},         //dm_dir_str_lut_p15
    {0xf8cb25b8,    24,      27,        0,        0,    0x400,        0},         //dm_dir_str_lut_p14
    {0xf8cb25b8,    20,      23,        0,        0,    0x400,        0},         //dm_dir_str_lut_p13
    {0xf8cb25b8,    16,      19,        0,        0,    0x400,        0},         //dm_dir_str_lut_p12
    {0xf8cb25b8,    12,      15,        0,        0,    0x400,        0},         //dm_dir_str_lut_p11
    {0xf8cb25b8,     8,      11,        0,        0,    0x400,        0},         //dm_dir_str_lut_p10
    {0xf8cb25b8,     4,       7,        0,        0,    0x400,        1},         //dm_dir_str_lut_p9
    {0xf8cb25b8,     0,       3,        0,        0,    0x400,        1},         //dm_dir_str_lut_p8
    {0xf8cb25b4,    28,      31,        0,        0,    0x400,        6},         //dm_dir_str_lut_p7
    {0xf8cb25b4,    24,      27,        0,        0,    0x400,        8},         //dm_dir_str_lut_p6
    {0xf8cb25b4,    20,      23,        0,        0,    0x400,        8},         //dm_dir_str_lut_p5
    {0xf8cb25b4,    16,      19,        0,        0,    0x400,        8},         //dm_dir_str_lut_p4
    {0xf8cb25b4,    12,      15,        0,        0,    0x400,        8},         //dm_dir_str_lut_p3
    {0xf8cb25b4,     8,      11,        0,        0,    0x400,        8},         //dm_dir_str_lut_p2
    {0xf8cb25b4,     4,       7,        0,        0,    0x400,        8},         //dm_dir_str_lut_p1
    {0xf8cb25b4,     0,       3,        0,        0,    0x400,        8},         //dm_dir_str_lut_p0
    /*DBM_POS*/
    {0xf8cb25bc,     0,      15,        0,        0,    0x400,        0},         //dbm_demo_xpos

    /************************************* DS *****************************************/
    /* SHOOTCTRL_HL */
    {0xF8cc08f8,    17,      17,        0,        0,   0x1000,        1},         //hl_shootctrl_en
    {0xF8cc08f8,    16,      16,        0,        0,   0x1000,        0},         //hl_shootctrl_mode
    {0xF8cc08f8,    15,      15,        0,        0,   0x1000,        1},         //hl_flatdect_mode
    {0xF8cc08f8,    14,      14,        0,        0,   0x1000,        1},         //hl_coringadj_en
    {0xF8cc08f8,     8,      13,        0,        0,   0x1000,       32},         //hl_gain
    {0xF8cc08f8,     0,       7,        0,        0,   0x1000,       32},         //hl_coring
    /* SHOOTCTRL_VL */
    {0xF8cc0900,    17,      17,        0,        0,   0x1000,        1},         //vl_shootctrl_en
    {0xF8cc0900,    16,      16,        0,        0,   0x1000,        0},         //vl_shootctrl_mode
    {0xF8cc0900,    15,      15,        0,        0,   0x1000,        1},         //vl_flatdect_mode
    {0xF8cc0900,    14,      14,        0,        0,   0x1000,        1},         //vl_coringadj_en
    {0xF8cc0900,     8,      13,        0,        0,   0x1000,       32},         //vl_gain
    {0xF8cc0900,     0,       7,        0,        0,   0x1000,       32},         //vl_coring

    /************************************** DCI ****************************************/
    /*DCI_CTRL_SET*/
    {0xF8cc4100,    31,      31,        0,        0,      0x10,       1},         //dci_en
    {0xF8cc4100,    30,      30,        0,        0,      0x10,       0},         //dci_dbg_en
    {0xF8cc4100,    28,      28,        0,        0,      0x10,       0},         //dci_man_adj0
    {0xF8cc4100,    27,      27,        0,        0,      0x10,       0},         //dci_man_adj1
    {0xF8cc4100,    26,      26,        0,        0,      0x10,       0},         //dci_man_adj2
    {0xF8cc4100,    25,      25,        0,        0,      0x10,       1},         //dci_cbcrcmp_en
    {0xF8cc4100,    24,      24,        0,        0,      0x10,       0},         //dci_cbcrsta_en
    {0xF8cc4100,    23,      23,        0,        0,      0x10,       1},         //dci_in_range
    {0xF8cc4100,    22,      22,        0,        0,      0x10,       1},         //dci_out_range
    {0xF8cc4100,    20,      21,        0,        0,      0x10,       2},         //dci_shift_ctrl
    {0xF8cc4100,    19,      19,        0,        0,      0x10,       1},         //dci_histlpf_en
    {0xF8cc4100,    18,      18,        0,        0,      0x10,       0},         //dci_swcfg_en
    {0xF8cc4100,    17,      17,        0,        0,      0x10,       0},         //dci_hist_state
    {0xF8cc4100,    16,      16,        0,        0,      0x10,       0},         //dci_dbg_mode
    {0xF8cc4100,    0,       11,        0,        0,      0x10,       0},         //dci_dbg_pos
    /*DCI_HIS_BLD*/
    {0xF8cc410c,    24,      31,        0,        0,      0x10,      64},         //dci_cbcrsta_y
    {0xF8cc410c,    16,      23,        0,        0,      0x10,       0},         //dci_cbcrsta_cb
    {0xF8cc410c,    8,       15,        0,        0,      0x10,       0},         //dci_cbcrsta_cr
    /*DCI_HIS_OFT*/
    {0xF8cc4110,    23,      31,        0,        0,      0x10,       0},         //dci_cbcrsta_oft
    /*DCI_HIS_COR*/
    {0xF8cc4114,    24,      31,        0,        0,      0x10,       0},         //dci_histcor_thr0
    {0xF8cc4114,    16,      23,        0,        0,      0x10,       0},         //dci_histcor_thr1
    {0xF8cc4114,    8,       15,        0,        0,      0x10,       0},         //dci_histcor_thr2
    /*DCI_MER_BLD*/
    {0xF8cc4118,    26,      31,        0,        0,      0x10,      18},         //dci_metrc_abld0
    {0xF8cc4118,    20,      25,        0,        0,      0x10,      18},         //dci_metrc_abld1
    {0xF8cc4118,    14,      19,        0,        0,      0x10,      18},         //dci_metrc_abld2
    {0xF8cc4118,    8,       13,        0,        0,      0x10,      16},         //dci_hist_abld
    {0xF8cc4118,    2,       7,         0,        0,      0x10,      17},         //dci_org_abld
    /*DCI_ADJ_WGT*/
    {0xF8cc411c,    24,      31,        0,        0,      0x10,      50},         //dci_man_adjwgt0
    {0xF8cc411c,    16,      23,        0,        0,      0x10,      60},         //dci_man_adjwgt1
    {0xF8cc411c,    8,       15,        0,        0,      0x10,      70},         //dci_man_adjwgt2
    /*DCI_CLIP*/
    {0xF8cc4120,    24,      31,        0,        0,      0x10,       0},         //dci_wgt_cliplow0
    {0xF8cc4120,    16,      23,        0,        0,      0x10,     255},         //dci_wgt_cliphigh0
    {0xF8cc4124,    24,      31,        0,        0,      0x10,       0},         //dci_wgt_cliplow1
    {0xF8cc4124,    16,      23,        0,        0,      0x10,     255},         //dci_wgt_cliphigh1
    {0xF8cc4128,    24,      31,        0,        0,      0x10,       0},         //dci_wgt_cliplow2
    {0xF8cc4128,    16,      23,        0,        0,      0x10,     255},         //dci_wgt_cliphigh2
    /*DCI_GLB_GAIN_SD_SOURCE*/
    {0xF8cc412c,    26,      31,        1,        0,      0x10,      32},         //dci_glb_gain_sd0
    {0xF8cc412c,    20,      25,        1,        0,      0x10,      32},         //dci_glb_gain_sd1
    {0xF8cc412c,    14,      19,        1,        0,      0x10,      32},         //dci_glb_gain_sd2
    /*DCI_GLB_GAIN_HD_SOURCE*/
    {0xF8cc412c,    26,      31,        2,        0,      0x10,      42},         //dci_glb_gain_hd0
    {0xF8cc412c,    20,      25,        2,        0,      0x10,      42},         //dci_glb_gain_hd1
    {0xF8cc412c,    14,      19,        2,        0,      0x10,      42},         //dci_glb_gain_hd2
    /*DCI_POS_THR*/
    {0xF8cc4130,    24,      31,        0,        0,      0x10,      20},         //dci_gainpos_thr1
    {0xF8cc4130,    16,      23,        0,        0,      0x10,      30},         //dci_gainpos_thr2
    {0xF8cc4130,    8,       15,        0,        0,      0x10,      40},         //dci_gainpos_thr3
    {0xF8cc4130,    0,       7,         0,        0,      0x10,      50},         //dci_gainpos_thr4
    {0xF8cc4134,    24,      31,        0,        0,      0x10,      60},         //dci_gainpos_thr5
    {0xF8cc4134,    16,      23,        0,        0,      0x10,      70},         //dci_gainpos_thr6
    {0xF8cc4134,    8,       15,        0,        0,      0x10,     115},         //dci_gainpos_thr7
    /*DCI_POS_GAIN*/
    {0xF8cc4138,    28,      31,        0,        0,      0x10,       0},         //dci_gainpos_cbcr0
    {0xF8cc4138,    24,      27,        0,        0,      0x10,       2},         //dci_gainpos_cbcr1
    {0xF8cc4138,    20,      23,        0,        0,      0x10,       4},         //dci_gainpos_cbcr2
    {0xF8cc4138,    16,      19,        0,        0,      0x10,       6},         //dci_gainpos_cbcr3
    {0xF8cc4138,    12,      15,        0,        0,      0x10,       7},         //dci_gainpos_cbcr4
    {0xF8cc4138,    8,       11,        0,        0,      0x10,       8},         //dci_gainpos_cbcr5
    {0xF8cc4138,    4,       7,         0,        0,      0x10,       9},         //dci_gainpos_cbcr6
    {0xF8cc4138,    0,       3,         0,        0,      0x10,      10},         //dci_gainpos_cbcr7
    {0xF8cc413c,    28,      31,        0,        0,      0x10,      10},         //dci_gainpos_cbcr8
    /*DCI_POS_SLP*/
    {0xF8cc4140,    22,      31,        0,        0,      0x10,      25},         //dci_gainpos_slp0
    {0xF8cc4140,    12,      21,        0,        0,      0x10,      51},         //dci_gainpos_slp1
    {0xF8cc4140,    2,       11,        0,        0,      0x10,      51},         //dci_gainpos_slp2
    {0xF8cc4144,    22,      31,        0,        0,      0x10,      25},         //dci_gainpos_slp3
    {0xF8cc4144,    12,      21,        0,        0,      0x10,      25},         //dci_gainpos_slp4
    {0xF8cc4144,    2,       11,        0,        0,      0x10,      25},         //dci_gainpos_slp5
    {0xF8cc4148,    22,      31,        0,        0,      0x10,       5},         //dci_gainpos_slp6
    {0xF8cc4148,    12,      21,        0,        0,      0x10,       0},         //dci_gainpos_slp7
    /*DCI_NEG_THR*/
    {0xF8cc414c,    24,      31,        0,        0,      0x10,      30},         //dci_gainneg_thr1
    {0xF8cc414c,    16,      23,        0,        0,      0x10,      50},         //dci_gainneg_thr2
    {0xF8cc414c,    8,       15,        0,        0,      0x10,      70},         //dci_gainneg_thr3
    {0xF8cc414c,    0,       7,         0,        0,      0x10,      90},         //dci_gainneg_thr4
    {0xF8cc4150,    24,      31,        0,        0,      0x10,     130},         //dci_gainneg_thr5
    {0xF8cc4150,    16,      23,        0,        0,      0x10,     150},         //dci_gainneg_thr6
    {0xF8cc4150,    8,       15,        0,        0,      0x10,     170},         //dci_gainneg_thr7
    /*DCI_NEG_CBCR_THR*/
    {0xF8cc4154,    28,      31,        0,        0,      0x10,       0},         //dci_gainneg_cbcr0
    {0xF8cc4154,    24,      27,        0,        0,      0x10,       1},         //dci_gainneg_cbcr1
    {0xF8cc4154,    20,      23,        0,        0,      0x10,       2},         //dci_gainneg_cbcr2
    {0xF8cc4154,    16,      19,        0,        0,      0x10,       2},         //dci_gainneg_cbcr3
    {0xF8cc4154,    12,      15,        0,        0,      0x10,       2},         //dci_gainneg_cbcr4
    {0xF8cc4154,    8,       11,        0,        0,      0x10,       2},         //dci_gainneg_cbcr5
    {0xF8cc4154,    4,       7,         0,        0,      0x10,       2},         //dci_gainneg_cbcr6
    {0xF8cc4154,    0,       3,         0,        0,      0x10,       2},         //dci_gainneg_cbcr7
    {0xF8cc4158,    28,      31,        0,        0,      0x10,       2},         //dci_gainneg_cbcr8
    /*DCI_NEG_SLP*/
    {0xF8cc415c,    22,      31,        0,        0,      0x10,       8},         //dci_gainneg_slp0
    {0xF8cc415c,    12,      21,        0,        0,      0x10,       12},        //dci_gainneg_slp1
    {0xF8cc415c,    2,       11,        0,        0,      0x10,       0},         //dci_gainneg_slp2
    {0xF8cc4160,    22,      31,        0,        0,      0x10,       0},         //dci_gainneg_slp3
    {0xF8cc4160,    12,      21,        0,        0,      0x10,       0},         //dci_gainneg_slp4
    {0xF8cc4160,    2,       11,        0,        0,      0x10,       0},         //dci_gainneg_slp5
    {0xF8cc4164,    22,      31,        0,        0,      0x10,       0},         //dci_gainneg_slp6
    {0xF8cc4164,    12,      21,        0,        0,      0x10,       0},         //dci_gainneg_slp7
    /*DCI_ADJ_SW*/
    {0xF8cc4168,    22,      31,        0,        0,      0x10,       0},         //dci_adj_sw0
    {0xF8cc4168,    12,      21,        0,        0,      0x10,       0},         //dci_adj_sw1
    {0xF8cc4168,    2,       11,        0,        0,      0x10,       0},         //dci_adj_sw2
    {0xF8cc416c,    22,      31,        0,        0,      0x10,     -16},         //dci_adj_sw3
    {0xF8cc416c,    12,      21,        0,        0,      0x10,     -30},         //dci_adj_sw4
    {0xF8cc416c,    2,       11,        0,        0,      0x10,     -41},         //dci_adj_sw5
    {0xF8cc4170,    22,      31,        0,        0,      0x10,     -49},         //dci_adj_sw6
    {0xF8cc4170,    12,      21,        0,        0,      0x10,     -45},         //dci_adj_sw7
    {0xF8cc4170,    2,       11,        0,        0,      0x10,     -38},         //dci_adj_sw8
    {0xF8cc4174,    22,      31,        0,        0,      0x10,     -32},         //dci_adj_sw9
    {0xF8cc4174,    12,      21,        0,        0,      0x10,     -26},         //dci_adj_sw10
    {0xF8cc4174,    2,       11,        0,        0,      0x10,     -12},         //dci_adj_sw11
    {0xF8cc4178,    22,      31,        0,        0,      0x10,      -6},         //dci_adj_sw12
    {0xF8cc4178,    12,      21,        0,        0,      0x10,      13},         //dci_adj_sw13
    {0xF8cc4178,    2,       11,        0,        0,      0x10,      28},         //dci_adj_sw14
    {0xF8cc417c,    22,      31,        0,        0,      0x10,      38},         //dci_adj_sw15
    {0xF8cc417c,    12,      21,        0,        0,      0x10,      53},         //dci_adj_sw16
    {0xF8cc417c,    2,       11,        0,        0,      0x10,      62},         //dci_adj_sw17
    {0xF8cc4180,    22,      31,        0,        0,      0x10,      69},         //dci_adj_sw18
    {0xF8cc4180,    12,      21,        0,        0,      0x10,      70},         //dci_adj_sw19
    {0xF8cc4180,    2,       11,        0,        0,      0x10,      72},         //dci_adj_sw20
    {0xF8cc4184,    22,      31,        0,        0,      0x10,      73},         //dci_adj_sw21
    {0xF8cc4184,    12,      21,        0,        0,      0x10,      70},         //dci_adj_sw22
    {0xF8cc4184,    2,       11,        0,        0,      0x10,      66},         //dci_adj_sw23
    {0xF8cc4188,    22,      31,        0,        0,      0x10,      58},         //dci_adj_sw24
    {0xF8cc4188,    12,      21,        0,        0,      0x10,      52},         //dci_adj_sw25
    {0xF8cc4188,    2,       11,        0,        0,      0x10,      44},         //dci_adj_sw26
    {0xF8cc418c,    22,      31,        0,        0,      0x10,      34},         //dci_adj_sw27
    {0xF8cc418c,    12,      21,        0,        0,      0x10,      10},         //dci_adj_sw28
    {0xF8cc418c,    2,       11,        0,        0,      0x10,       0},         //dci_adj_sw29
    {0xF8cc4190,    22,      31,        0,        0,      0x10,       0},         //dci_adj_sw30
    {0xF8cc4190,    12,      21,        0,        0,      0x10,       0},         //dci_adj_sw31
    {0xF8cc4190,    2,       11,        0,        0,      0x10,       0},         //dci_adj_sw32

    /*************************************ACM****************************************/
    /*VP0_ACM_CTRL*/
    {0xf8cc41a0,    31,      31,        0,        0,      0x20,       0},         //acm_en
    {0xf8cc41a0,    30,      30,        0,        0,      0x20,       0},         //acm_dbg_en
    {0xf8cc41a0,    29,      29,        0,        0,      0x20,       1},         //acm_stretch
    {0xf8cc41a0,    28,      28,        0,        0,      0x20,       1},         //acm_cliprange
    {0xf8cc41a0,    27,      27,        0,        0,      0x20,       0},         //acm_cliporwrap
    {0xf8cc41a0,    10,      21,        0,        0,      0x20,       0},         //acm_dbg_pos
    {0xf8cc41a0,    9,       9,         0,        0,      0x20,       0},         //acm_dbg_mode
    {0xf8cc41a0,    0,       8,         0,        0,      0x20,       0},         //acm_cbcrthr
    /*VP0_ACM_GAIN_SD_SOURCE*/
    {0xf8cc41a4,    20,      29,        1,        0,      0x20,      80},         //acm_gain_luma_sd
    {0xf8cc41a4,    10,      19,        1,        0,      0x20,      80},         //acm_gain_hue_sd
    {0xf8cc41a4,    0,       9,         1,        0,      0x20,      80},         //acm_gain_sat_sd
    /*VP0_ACM_GAIN_HD_SOURCE*/
    {0xf8cc41a4,    20,      29,        2,        0,      0x20,      80},         //acm_gain_luma_hd
    {0xf8cc41a4,    10,      19,        2,        0,      0x20,      80},         //acm_gain_hue_hd
    {0xf8cc41a4,    0,       9,         2,        0,      0x20,      80},         //acm_gain_sat_hd

    /*************************************TNR****************************************/
    /*TNR_MODE*/
    {0xf8cb3800,    31,      31,        0,        0,      0x800,      0},         //market_en
    {0xf8cb3800,    30,      30,        0,        0,      0x800,      0},         //market_mode
    {0xf8cb3800,    28,      28,        0,        0,      0x800,      0},         //tnr_y_bypass
    {0xf8cb3800,    27,      27,        0,        0,      0x800,      0},         //tnr_c_bypass
    {0xf8cb3800,    26,      26,        0,        0,      0x800,      1},         //hist_motion_en
    {0xf8cb3800,    21,      21,        0,        0,      0x800,      1},         //tfm_lpf_en
    {0xf8cb3800,    12,      13,        0,        0,      0x800,      2},         //tm_cmt_mode (fixed)
    {0xf8cb3800,    10,      11,        0,        0,      0x800,      2},         //tm_ymt_mode (fixed)
    {0xf8cb3800,    8,       9,         0,        0,      0x800,      3},         //tm_cmt_mean_mode
    {0xf8cb3800,    6,       7,         0,        0,      0x800,      3},         //tm_ymt_mean_mode
    {0xf8cb3800,    0,       1,         0,        0,      0x800,      2},         //tm_lpf_mode
    /*TNR_YMOTION_MAP*/
    {0xf8cb3804,    31,      31,        0,        0,      0x800,      1},         //tm_ymt_meanratio_en
    {0xf8cb3804,    0,       9,         0,        0,      0x800,      0},         //tm_ymt_mapping_min
    {0xf8cb3804,    10,      19,        0,        0,      0x800,    255},         //tm_ymt_mapping_max
    {0xf8cb3808,    0,       9,         0,        0,      0x800,      5},         //tm_ymt_mapping_r0
    {0xf8cb3808,    10,      19,        0,        0,      0x800,     56},         //tm_ymt_mapping_r1
    {0xf8cb3808,    20,      29,        0,        0,      0x800,     96},         //tm_ymt_mapping_r2
    {0xf8cb380c,    0,       9,         0,        0,      0x800,    144},         //tm_ymt_mapping_r3
    {0xf8cb380c,    10,      19,        0,        0,      0x800,    255},         //tm_ymt_mapping_r4
    {0xf8cb3810,    0,       9,         0,        0,      0x800,      0},         //tm_ymt_mapping_t1
    {0xf8cb3810,    10,      19,        0,        0,      0x800,     32},         //tm_ymt_mapping_t2
    {0xf8cb3810,    20,      29,        0,        0,      0x800,     64},         //tm_ymt_mapping_t3
    {0xf8cb3814,    0,       9,         0,        0,      0x800,    128},         //tm_ymt_mapping_t4
    {0xf8cb3814,    10,      19,        0,        0,      0x800,    255},         //tm_ymt_mapping_t5
    {0xf8cb3814,    20,      29,        0,        0,      0x800,    255},         //tm_ymt_mapping_t6
    {0xf8cb3818,    0,       9,         0,        0,      0x800,     26},         //tm_ymt_mapping_k1
    {0xf8cb3818,    10,      19,        0,        0,      0x800,     20},         //tm_ymt_mapping_k2
    {0xf8cb381c,    0,       9,         0,        0,      0x800,     12},         //tm_ymt_mapping_k3
    {0xf8cb381c,    10,      19,        0,        0,      0x800,     14},         //tm_ymt_mapping_k4
    /*TNR_CMOTION_MAP*/
    {0xf8cb3820,    31,      31,        0,        0,      0x800,      0},         //tm_cmt_meanratio_en
    {0xf8cb3820,    0,       9,         0,        0,      0x800,      0},         //tm_cmt_mapping_min
    {0xf8cb3820,    10,      19,        0,        0,      0x800,    255},         //tm_cmt_mapping_max
    {0xf8cb3824,    0,       9,         0,        0,      0x800,      5},         //tm_cmt_mapping_r0
    {0xf8cb3824,    10,      19,        0,        0,      0x800,     56},         //tm_cmt_mapping_r1
    {0xf8cb3824,    20,      29,        0,        0,      0x800,     96},         //tm_cmt_mapping_r2
    {0xf8cb3828,    0,       9,         0,        0,      0x800,    144},         //tm_cmt_mapping_r3
    {0xf8cb3828,    10,      19,        0,        0,      0x800,    255},         //tm_cmt_mapping_r4
    {0xf8cb382c,    0,       9,         0,        0,      0x800,      0},         //tm_cmt_mapping_t1
    {0xf8cb382c,    10,      19,        0,        0,      0x800,     32},         //tm_cmt_mapping_t2
    {0xf8cb382c,    20,      29,        0,        0,      0x800,     64},         //tm_cmt_mapping_t3
    {0xf8cb3830,    0,       9,         0,        0,      0x800,    128},         //tm_cmt_mapping_t4
    {0xf8cb3830,    10,      19,        0,        0,      0x800,    255},         //tm_cmt_mapping_t5
    {0xf8cb3830,    20,      29,        0,        0,      0x800,    255},         //tm_cmt_mapping_t6
    {0xf8cb3834,    0,       9,         0,        0,      0x800,     26},         //tm_cmt_mapping_k1
    {0xf8cb3834,    10,      19,        0,        0,      0x800,     20},         //tm_cmt_mapping_k2
    {0xf8cb3838,    0,       9,         0,        0,      0x800,     12},         //tm_cmt_mapping_k3
    {0xf8cb3838,    10,      19,        0,        0,      0x800,     14},         //tm_cmt_mapping_k4
    /*TNR_YMOTION_MEANRATIO*/
    {0xf8cb383c,    0,       9,         0,        0,      0x800,      0},         //tm_ymt_meanratio_min
    {0xf8cb383c,    10,      19,        0,        0,      0x800,   1023},         //tm_ymt_meanratio_max
    {0xf8cb3840,    0,       9,         0,        0,      0x800,    120},         //tm_ymt_meanratio_r0
    {0xf8cb3840,    10,      19,        0,        0,      0x800,     32},         //tm_ymt_meanratio_r1
    {0xf8cb3840,    20,      29,        0,        0,      0x800,     32},         //tm_ymt_meanratio_r2
    {0xf8cb3844,    0,       9,         0,        0,      0x800,     32},         //tm_ymt_meanratio_r3
    {0xf8cb3844,    10,      19,        0,        0,      0x800,     32},         //tm_ymt_meanratio_r4
    {0xf8cb3848,    0,       9,         0,        0,      0x800,     24},         //tm_ymt_meanratio_t1
    {0xf8cb3848,    10,      19,        0,        0,      0x800,     36},         //tm_ymt_meanratio_t2
    {0xf8cb3848,    20,      29,        0,        0,      0x800,    256},         //tm_ymt_meanratio_t3
    {0xf8cb384c,    0,       9,         0,        0,      0x800,   1023},         //tm_ymt_meanratio_t4
    {0xf8cb384c,    10,      19,        0,        0,      0x800,   1023},         //tm_ymt_meanratio_t5
    {0xf8cb384c,    20,      29,        0,        0,      0x800,   1023},         //tm_ymt_meanratio_t6
    {0xf8cb3850,    0,       9,         0,        0,      0x800,   -117},         //tm_ymt_meanratio_k1
    {0xf8cb3850,    10,      19,        0,        0,      0x800,      0},         //tm_ymt_meanratio_k2
    {0xf8cb3854,    0,       9,         0,        0,      0x800,      0},         //tm_ymt_meanratio_k3
    {0xf8cb3854,    10,      19,        0,        0,      0x800,      0},         //tm_ymt_meanratio_k4
    /*TNR_CMOTION_MEANRATIO*/
    {0xf8cb3858,    0,       9,         0,        0,      0x800,      0},         //tm_cmt_meanratio_min
    {0xf8cb3858,    10,      19,        0,        0,      0x800,   1023},         //tm_cmt_meanratio_max
    {0xf8cb385c,    0,       9,         0,        0,      0x800,     32},         //tm_cmt_meanratio_r0
    {0xf8cb385c,    10,      19,        0,        0,      0x800,     32},         //tm_cmt_meanratio_r1
    {0xf8cb385c,    20,      29,        0,        0,      0x800,     32},         //tm_cmt_meanratio_r2
    {0xf8cb3860,    0,       9,         0,        0,      0x800,     32},         //tm_cmt_meanratio_r3
    {0xf8cb3860,    10,      19,        0,        0,      0x800,     32},         //tm_cmt_meanratio_r4
    {0xf8cb3864,    0,       9,         0,        0,      0x800,      1},         //tm_cmt_meanratio_t1
    {0xf8cb3864,    10,      19,        0,        0,      0x800,    100},         //tm_cmt_meanratio_t2
    {0xf8cb3864,    20,      29,        0,        0,      0x800,    200},         //tm_cmt_meanratio_t3
    {0xf8cb3868,    0,       9,         0,        0,      0x800,    300},         //tm_cmt_meanratio_t4
    {0xf8cb3868,    10,      19,        0,        0,      0x800,    400},         //tm_cmt_meanratio_t5
    {0xf8cb3868,    20,      29,        0,        0,      0x800,   1023},         //tm_cmt_meanratio_t6
    {0xf8cb386c,    0,       9,         0,        0,      0x800,      0},         //tm_cmt_meanratio_k1
    {0xf8cb386c,    10,      19,        0,        0,      0x800,      0},         //tm_cmt_meanratio_k2
    {0xf8cb3870,    0,       9,         0,        0,      0x800,      0},         //tm_cmt_meanratio_k3
    {0xf8cb3870,    10,      19,        0,        0,      0x800,      0},         //tm_cmt_meanratio_k4
    /*TNR_TM_YCMTCOEF*/
    {0xf8cb3874,    0,       5,         0,        0,      0x800,      2},         //tm_ycmtcoef0
    {0xf8cb3874,    6,       11,        0,        0,      0x800,      6},         //tm_ycmtcoef1
    {0xf8cb3874,    12,      17,        0,        0,      0x800,     16},         //tm_ycmtcoef2
    {0xf8cb3874,    18,      23,        0,        0,      0x800,      6},         //tm_ycmtcoef3
    {0xf8cb3874,    24,      29,        0,        0,      0x800,      2},         //tm_ycmtcoef4
    /*TNR_TM_YCLPFCOEF*/
    {0xf8cb3878,    0,       5,         0,        0,      0x800,      2},         //tm_yclpfcoef0
    {0xf8cb3878,    6,       11,        0,        0,      0x800,      6},         //tm_yclpfcoef1
    {0xf8cb3878,    12,      17,        0,        0,      0x800,     16},         //tm_yclpfcoef2
    {0xf8cb3878,    18,      23,        0,        0,      0x800,      6},         //tm_yclpfcoef3
    {0xf8cb3878,    24,      29,        0,        0,      0x800,      2},         //tm_yclpfcoef4
    /*TNR_TFM_CMOTION_CK*/
    {0xf8cb3974,    0,       9,         0,        0,      0x800,      0},         //tfm_cmotion_core
    {0xf8cb3974,    10,      19,        0,        0,      0x800,     32},         //tfm_cmotion_k
    /*TNR_TFM_CEDGE_CK*/
    {0xf8cb3978,    0,       9,         0,        0,      0x800,      0},         //tfm_cedge_core (fixed)
    {0xf8cb3978,    10,      19,        0,        0,      0x800,      0},         //tfm_cedge_k (fixed)
    {0xf8cb3978,    20,      20,        0,        0,      0x800,      0},         //tfm_2dupdate (fixed)
    {0xf8cb3978,    21,      21,        0,        0,      0x800,      1},         //tfm_cbcrupdate
    /*TNR_TFM_Y_MAP*/
    {0xf8cb397c,    0,       9,         0,        0,      0x800,      0},         //tfm_y_mapping_min
    {0xf8cb397c,    10,      19,        0,        0,      0x800,     32},         //tfm_y_mapping_max
    {0xf8cb3980,    0,       9,         0,        0,      0x800,     10},         //tfm_y_mapping_r0
    {0xf8cb3980,    10,      19,        0,        0,      0x800,      5},         //tfm_y_mapping_r1
    {0xf8cb3980,    20,      29,        0,        0,      0x800,     20},         //tfm_y_mapping_r2
    {0xf8cb3984,    0,       9,         0,        0,      0x800,     27},         //tfm_y_mapping_r3
    {0xf8cb3984,    10,      19,        0,        0,      0x800,     32},         //tfm_y_mapping_r4
    {0xf8cb3988,    0,       9,         0,        0,      0x800,      0},         //tfm_y_mapping_t1
    {0xf8cb3988,    10,      19,        0,        0,      0x800,      4},         //tfm_y_mapping_t2
    {0xf8cb3988,    20,      29,        0,        0,      0x800,     32},         //tfm_y_mapping_t3
    {0xf8cb398c,    0,       9,         0,        0,      0x800,     46},         //tfm_y_mapping_t4
    {0xf8cb398c,    10,      19,        0,        0,      0x800,     58},         //tfm_y_mapping_t5
    {0xf8cb398c,    20,      29,        0,        0,      0x800,     63},         //tfm_y_mapping_t6
    {0xf8cb3990,    0,       9,         0,        0,      0x800,    -20},         //tfm_y_mapping_k1
    {0xf8cb3990,    10,      19,        0,        0,      0x800,      9},         //tfm_y_mapping_k2
    {0xf8cb3994,    0,       9,         0,        0,      0x800,      8},         //tfm_y_mapping_k3
    {0xf8cb3994,    10,      19,        0,        0,      0x800,      7},         //tfm_y_mapping_k4
    /*TNR_TFM_C_MAP*/
    {0xf8cb3998,    0,       9,         0,        0,      0x800,      0},         //tfm_c_mapping_min
    {0xf8cb3998,    10,      19,        0,        0,      0x800,     32},         //tfm_c_mapping_max
    {0xf8cb399c,    0,       9,         0,        0,      0x800,     10},         //tfm_c_mapping_r0
    {0xf8cb399c,    10,      19,        0,        0,      0x800,      5},         //tfm_c_mapping_r1
    {0xf8cb399c,    20,      29,        0,        0,      0x800,     20},         //tfm_c_mapping_r2
    {0xf8cb39a0,    0,       9,         0,        0,      0x800,     27},         //tfm_c_mapping_r3
    {0xf8cb39a0,    10,      19,        0,        0,      0x800,     32},         //tfm_c_mapping_r4
    {0xf8cb39a4,    0,       9,         0,        0,      0x800,      0},         //tfm_c_mapping_t1
    {0xf8cb39a4,    10,      19,        0,        0,      0x800,      4},         //tfm_c_mapping_t2
    {0xf8cb39a4,    20,      29,        0,        0,      0x800,     32},         //tfm_c_mapping_t3
    {0xf8cb39a8,    0,       9,         0,        0,      0x800,     46},         //tfm_c_mapping_t4
    {0xf8cb39a8,    10,      19,        0,        0,      0x800,     58},         //tfm_c_mapping_t5
    {0xf8cb39a8,    20,      29,        0,        0,      0x800,     63},         //tfm_c_mapping_t6
    {0xf8cb39ac,    0,       9,         0,        0,      0x800,    -70},         //tfm_c_mapping_k1
    {0xf8cb39ac,    10,      19,        0,        0,      0x800,      9},         //tfm_c_mapping_k2
    {0xf8cb39b0,    0,       9,         0,        0,      0x800,      8},         //tfm_c_mapping_k3
    {0xf8cb39b0,    10,      19,        0,        0,      0x800,      7},         //tfm_c_mapping_k4
    /*TNR_TFM_HISTMOTION_BLEND*/
    {0xf8cb39b4,    0,       9,         0,        0,      0x800,     32},         //tfm_hism_term1
    {0xf8cb39b4,    10,      19,        0,        0,      0x800,   1023},         //tfm_hism_term2
    {0xf8cb39b4,    20,      25,        0,        0,      0x800,     28},         //tfm_hism_alpha1
    {0xf8cb39b4,    26,      31,        0,        0,      0x800,     26},         //tfm_hism_alpha2
    /*TNR_TFM_PARA*/
    {0xf8cb39b8,    0,       0,         0,        0,      0x800,      0},         //tfm_filter_mode
    {0xf8cb39b8,    1,       2,         0,        0,      0x800,      1},         //tfm_calc_mode  (fixed)
    {0xf8cb39b8,    3,       3,         0,        0,      0x800,      0},         //tfm_scenechange_en  (fixed)
    {0xf8cb39b8,    4,       4,         0,        0,      0x800,      0},         //tfm_scenechangeformotion_en  (fixed)
    {0xf8cb39b8,    5,       5,         0,        0,      0x800,      0},         //tfm_globalmotion_en (fixed)
    {0xf8cb39b8,    6,       6,         0,        0,      0x800,      0},         //tfm_cbcrupdate_mode
    {0xf8cb39b8,    7,       7,         0,        0,      0x800,      1},         //tfm_1dlpf_mode
    {0xf8cb39b8,    8,       8,         0,        0,      0x800,      0},         //ymotion_mode
    {0xf8cb39b8,    9,       9,         0,        0,      0x800,      1},         //cmotion_mode
    {0xf8cb39b8,    10,      17,        0,        0,      0x800,      0},         //tfm_globalmotion  (fixed)
    {0xf8cb39b8,    29,      29,        0,        0,      0x800,      0},         //dither_round
    {0xf8cb39b8,    30,      30,        0,        0,      0x800,      1},         //dither_en_c
    {0xf8cb39b8,    31,      31,        0,        0,      0x800,      1},         //dither_en
    /*TNR_TFM_YMOTION_CK*/
    {0xf8cb39bc,    0,       9,         0,        0,      0x800,      0},         //tfm_ymotion_core
    {0xf8cb39bc,    10,      19,        0,        0,      0x800,     32},         //tfm_ymotion_k
    /*TNR_TFM_YEG_CK*/
    {0xf8cb39c0,    0,       9,         0,        0,      0x800,      0},         //tfm_yedge_core (fixed)
    {0xf8cb39c0,    10,      19,        0,        0,      0x800,      0},         //tfm_yedge_k (fixed)
    /*TNR_TM_YMTMEAN_CK*/
    {0xf8cb39c4,    0,       9,         0,        0,      0x800,      0},         //tm_ymtmean_core
    {0xf8cb39c4,    10,      19,        0,        0,      0x800,     16},         //tm_ymtmean_k
    /*TNR_TYMYCK*/
    {0xf8cb39c8,    0,       9,         0,        0,      0x800,      0},         //tymy_core (fixed)
    {0xf8cb39c8,    10,      19,        0,        0,      0x800,     32},         //tymy_k (fixed)
    /*TNR_TYMEYCK*/
    {0xf8cb39cc,    0,       9,         0,        0,      0x800,      0},         //tymey_core (fixed)
    {0xf8cb39cc,    10,      19,        0,        0,      0x800,      0},         //tymey_k (fixed)
    /*TNR_TCMYCK*/
    {0xf8cb39d0,    0,       9,         0,        0,      0x800,      0},         //tcmy_core (fixed)
    {0xf8cb39d0,    10,      19,        0,        0,      0x800,     32},         //tcmy_k (fixed)
    /*TNR_TCMEYCK*/
    {0xf8cb39d4,    0,       9,         0,        0,      0x800,      0},         //tcmey_core (fixed)
    {0xf8cb39d4,    10,      19,        0,        0,      0x800,      0},         //tcmey_k (fixed)
    /*TNR_TM_CMT_CK*/
    {0xf8cb39d8,    0,       9,         0,        0,      0x800,      0},         //tm_cmt_core
    {0xf8cb39d8,    10,      19,        0,        0,      0x800,     25},         //tm_cmt_k
    /*TNR_TM_YMT_CK*/
    {0xf8cb39dc,    0,       9,         0,        0,      0x800,      0},         //tm_ymt_core
    {0xf8cb39dc,    10,      19,        0,        0,      0x800,     28},         //tm_ymt_k
    /*TNR_TM_CMTMEAN_CK*/
    {0xf8cb39e0,    0,       9,         0,        0,      0x800,      0},         //tm_cmtmean_core
    {0xf8cb39e0,    10,      19,        0,        0,      0x800,     16},         //tm_cmtmean_k
    /*TNR_SNR_CTRL*/
    {0xf8cb39e4,    1,       1,         0,        0,      0x800,      1},         //ynr_1d_en
    {0xf8cb39e4,    2,       2,         0,        0,      0x800,      1},         //cnr_1d_en
    {0xf8cb39e4,    3,       10,        0,        0,      0x800,      0},         //sencechange  (fixed)
    {0xf8cb39e4,    19,      19,        0,        0,      0x800,      0},         //ctrandnoise_en
    {0xf8cb39e4,    20,      22,        0,        0,      0x800,      2},         //ctrandnoise_bit
    {0xf8cb39e4,    23,      23,        0,        0,      0x800,      0},         //ytrandnoise_en
    {0xf8cb39e4,    24,      26,        0,        0,      0x800,      2},         //ytrandnoise_bit
    /*TNR_BLEND_CTRL*/
    {0xf8cb39e8,    2,       9,         0,        0,      0x800,      0},         //s_change (fixed)
    {0xf8cb39e8,    10,      10,        0,        0,      0x800,      1},         //cmotion_strength_en
    {0xf8cb39e8,    11,      11,        0,        0,      0x800,      1},         //ymotion_strength_en
    /*TNR_Y_FMOTION_MAP*/
    {0xf8cb3e40,    0,       7,         0,        0,      0x800,      7},         //data0
    {0xf8cb3e40,    8,       15,        0,        0,      0x800,      7},         //data1
    {0xf8cb3e40,    16,      23,        0,        0,      0x800,      7},         //data2
    {0xf8cb3e40,    24,      31,        0,        0,      0x800,      6},         //data3
    {0xf8cb3e44,    0,       7,         0,        0,      0x800,      6},         //data4
    {0xf8cb3e44,    8,       15,        0,        0,      0x800,      7},         //data5
    {0xf8cb3e44,    16,      23,        0,        0,      0x800,      7},         //data6
    {0xf8cb3e44,    24,      31,        0,        0,      0x800,      8},         //data7
    {0xf8cb3e48,    0,       7,         0,        0,      0x800,      9},         //data8
    {0xf8cb3e48,    8,       15,        0,        0,      0x800,     10},         //data9
    {0xf8cb3e48,    16,      23,        0,        0,      0x800,     11},         //data10
    {0xf8cb3e48,    24,      31,        0,        0,      0x800,     12},         //data11
    {0xf8cb3e4c,    0,       7,         0,        0,      0x800,     13},         //data12
    {0xf8cb3e4c,    8,       15,        0,        0,      0x800,     15},         //data13
    {0xf8cb3e4c,    16,      23,        0,        0,      0x800,     16},         //data14
    {0xf8cb3e4c,    24,      31,        0,        0,      0x800,     17},         //data15
    {0xf8cb3e50,    0,       7,         0,        0,      0x800,     18},         //data16
    {0xf8cb3e50,    8,       15,        0,        0,      0x800,     19},         //data17
    {0xf8cb3e50,    16,      23,        0,        0,      0x800,     20},         //data18
    {0xf8cb3e50,    24,      31,        0,        0,      0x800,     21},         //data19
    {0xf8cb3e54,    0,       7,         0,        0,      0x800,     22},         //data20
    {0xf8cb3e54,    8,       15,        0,        0,      0x800,     23},         //data21
    {0xf8cb3e54,    16,      23,        0,        0,      0x800,     24},         //data22
    {0xf8cb3e54,    24,      31,        0,        0,      0x800,     25},         //data23
    {0xf8cb3e58,    0,       7,         0,        0,      0x800,     26},         //data24
    {0xf8cb3e58,    8,       15,        0,        0,      0x800,     27},         //data25
    {0xf8cb3e58,    16,      23,        0,        0,      0x800,     28},         //data26
    {0xf8cb3e58,    24,      31,        0,        0,      0x800,     29},         //data27
    {0xf8cb3e5c,    0,       7,         0,        0,      0x800,     30},         //data28
    {0xf8cb3e5c,    8,       15,        0,        0,      0x800,     31},         //data29
    {0xf8cb3e5c,    16,      23,        0,        0,      0x800,     31},         //data30
    {0xf8cb3e5c,    24,      31,        0,        0,      0x800,     31},         //data31
    /*TNR_C_FMOTION_MAP*/
    {0xf8cb3e60,    0,       7,         0,        0,      0x800,      7},         //data0
    {0xf8cb3e60,    8,       15,        0,        0,      0x800,      7},         //data1
    {0xf8cb3e60,    16,      23,        0,        0,      0x800,      7},         //data2
    {0xf8cb3e60,    24,      31,        0,        0,      0x800,      6},         //data3
    {0xf8cb3e64,    0,       7,         0,        0,      0x800,      6},         //data4
    {0xf8cb3e64,    8,       15,        0,        0,      0x800,      7},         //data5
    {0xf8cb3e64,    16,      23,        0,        0,      0x800,      7},         //data6
    {0xf8cb3e64,    24,      31,        0,        0,      0x800,      8},         //data7
    {0xf8cb3e68,    0,       7,         0,        0,      0x800,      9},         //data8
    {0xf8cb3e68,    8,       15,        0,        0,      0x800,     10},         //data9
    {0xf8cb3e68,    16,      23,        0,        0,      0x800,     11},         //data10
    {0xf8cb3e68,    24,      31,        0,        0,      0x800,     12},         //data11
    {0xf8cb3e6c,    0,       7,         0,        0,      0x800,     13},         //data12
    {0xf8cb3e6c,    8,       15,        0,        0,      0x800,     15},         //data13
    {0xf8cb3e6c,    16,      23,        0,        0,      0x800,     16},         //data14
    {0xf8cb3e6c,    24,      31,        0,        0,      0x800,     17},         //data15
    {0xf8cb3e70,    0,       7,         0,        0,      0x800,     18},         //data16
    {0xf8cb3e70,    8,       15,        0,        0,      0x800,     19},         //data17
    {0xf8cb3e70,    16,      23,        0,        0,      0x800,     20},         //data18
    {0xf8cb3e70,    24,      31,        0,        0,      0x800,     21},         //data19
    {0xf8cb3e74,    0,       7,         0,        0,      0x800,     22},         //data20
    {0xf8cb3e74,    8,       15,        0,        0,      0x800,     23},         //data21
    {0xf8cb3e74,    16,      23,        0,        0,      0x800,     24},         //data22
    {0xf8cb3e74,    24,      31,        0,        0,      0x800,     25},         //data23
    {0xf8cb3e78,    0,       7,         0,        0,      0x800,     26},         //data24
    {0xf8cb3e78,    8,       15,        0,        0,      0x800,     27},         //data25
    {0xf8cb3e78,    16,      23,        0,        0,      0x800,     28},         //data26
    {0xf8cb3e78,    24,      31,        0,        0,      0x800,     29},         //data27
    {0xf8cb3e7c,    0,       7,         0,        0,      0x800,     30},         //data28
    {0xf8cb3e7c,    8,       15,        0,        0,      0x800,     31},         //data29
    {0xf8cb3e7c,    16,      23,        0,        0,      0x800,     31},         //data30
    {0xf8cb3e7c,    24,      31,        0,        0,      0x800,     31},         //data31
    

    /***************************************************************************
      * Special Register
     ***************************************************************************/
    /*IP_DETECT*/
    {0xfffe0001,    0,       1,        0,        0,        0x0,       0},         //ip_detect_choice
    {0xfffe0005,    0,      15,        0,        0,        0x0,   23800},         //sg_u32FrmRateThr_L
    {0xfffe0009,    0,      15,        0,        0,        0x0,   24200},         //sg_u32FrmRateThr_H

    /*DB_SOFT_PARA*/
    {0xfffe0105,    0,       3,        0,        0,        0x0,       6},         //NumberWindow
    {0xfffe0109,    0,       3,        0,        0,        0x0,      10},         //HyMINBlkNum
    {0xfffe010d,    0,       3,        0,        0,        0x0,      10},         //HchrMINBlkNum
    {0xfffe0111,    0,       3,        0,        0,        0x0,      10},         //VyMINBlkNum
    {0xfffe0115,    0,       3,        0,        0,        0x0,       4},         //keyfrm_num
    {0xfffe0119,    0,       4,        0,        0,        0x0,      15},         //HyBordCountThreshold
    {0xfffe011d,    0,       4,        0,        0,        0x0,      15},         //HchrBordCountThreshold
    {0xfffe0121,    0,       4,        0,        0,        0x0,      15},         //VyBordCountThreshold
    {0xfffe0125,    0,       3,        0,        0,        0x0,       4},         //frmsWindowTemp
    {0xfffe0129,    0,       3,        0,        0,        0x0,       8},         //frmsSatWindowTemp

    /*PQ_SOFT_DB_HL_DELTA_STRONG_2*/
    {0xfffe0141,    28,      31,       0,        0,        0x0,       0},         //lum_hor_delta_lut_p15
    {0xfffe0141,    24,      27,       0,        0,        0x0,       0},         //lum_hor_delta_lut_p14
    {0xfffe0141,    20,      23,       0,        0,        0x0,       2},         //lum_hor_delta_lut_p13
    {0xfffe0141,    16,      19,       0,        0,        0x0,       4},         //lum_hor_delta_lut_p12
    {0xfffe0141,    12,      15,       0,        0,        0x0,       6},         //lum_hor_delta_lut_p11
    {0xfffe0141,     8,      13,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p10
    {0xfffe0141,     4,       7,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p9
    {0xfffe0141,     0,       3,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p8
    /*PQ_SOFT_DB_HL_DELTA_STRONG_1*/
    {0xfffe0142,    28,      31,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p7
    {0xfffe0142,    24,      27,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p6
    {0xfffe0142,    20,      23,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p5
    {0xfffe0142,    16,      19,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p4
    {0xfffe0142,    12,      15,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p3
    {0xfffe0142,     8,      11,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p2
    {0xfffe0142,     4,       7,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p1
    {0xfffe0142,     0,       3,       0,        0,        0x0,       4},         //lum_hor_delta_lut_p0
    /*PQ_SOFT_DB_HL_DELTA_ULT_STR_2*/
    {0xfffe0143,    28,      31,       0,        0,        0x0,       0},         //lum_hor_delta_lut_p15
    {0xfffe0143,    24,      27,       0,        0,        0x0,       0},         //lum_hor_delta_lut_p14
    {0xfffe0143,    20,      23,       0,        0,        0x0,       2},         //lum_hor_delta_lut_p13
    {0xfffe0143,    16,      19,       0,        0,        0x0,       4},         //lum_hor_delta_lut_p12
    {0xfffe0143,    12,      15,       0,        0,        0x0,       6},         //lum_hor_delta_lut_p11
    {0xfffe0143,     8,      13,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p10
    {0xfffe0143,     4,       7,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p9
    {0xfffe0143,     0,       3,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p8
    /*PQ_SOFT_DB_HL_DELTA_ULT_STR_1*/
    {0xfffe0144,    28,      31,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p7
    {0xfffe0144,    24,      27,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p6
    {0xfffe0144,    20,      23,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p5
    {0xfffe0144,    16,      19,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p4
    {0xfffe0144,    12,      15,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p3
    {0xfffe0144,     8,      11,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p2
    {0xfffe0144,     4,       7,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p1
    {0xfffe0144,     0,       3,       0,        0,        0x0,       4},         //lum_hor_delta_lut_p0
    /*PQ_SOFT_DB_HL_DELTA_MID_2*/
    {0xfffe0145,    28,      31,       0,        0,        0x0,       0},         //lum_hor_delta_lut_p15
    {0xfffe0145,    24,      27,       0,        0,        0x0,       0},         //lum_hor_delta_lut_p14
    {0xfffe0145,    20,      23,       0,        0,        0x0,       2},         //lum_hor_delta_lut_p13
    {0xfffe0145,    16,      19,       0,        0,        0x0,       4},         //lum_hor_delta_lut_p12
    {0xfffe0145,    12,      15,       0,        0,        0x0,       6},         //lum_hor_delta_lut_p11
    {0xfffe0145,     8,      13,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p10
    {0xfffe0145,     4,       7,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p9
    {0xfffe0145,     0,       3,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p8
    /*PQ_SOFT_DB_HL_DELTA_MID_1*/
    {0xfffe0146,    28,      31,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p7
    {0xfffe0146,    24,      27,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p6
    {0xfffe0146,    20,      23,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p5
    {0xfffe0146,    16,      19,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p4
    {0xfffe0146,    12,      15,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p3
    {0xfffe0146,     8,      11,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p2
    {0xfffe0146,     4,       7,       0,        0,        0x0,       8},         //lum_hor_delta_lut_p1
    {0xfffe0146,     0,       3,       0,        0,        0x0,       4},         //lum_hor_delta_lut_p0

    /*PQ_SOFT_DB_LV_DELTA_2*/
    {0xfffe0147,    28,      31,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p15
    {0xfffe0147,    24,      27,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p14
    {0xfffe0147,    20,      23,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p13
    {0xfffe0147,    16,      19,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p12
    {0xfffe0147,    12,      15,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p11
    {0xfffe0147,     8,      11,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p10
    {0xfffe0147,     4,       7,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p9
    {0xfffe0147,     0,       3,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p8
    /*PQ_SOFT_DB_LV_DELTA_1*/
    {0xfffe0148,    28,      31,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p7
    {0xfffe0148,    24,      27,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p6
    {0xfffe0148,    20,      23,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p5
    {0xfffe0148,    16,      19,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p4
    {0xfffe0148,    12,      15,       0,        0,        0x0,       2},         //lum_ver_delta_lut_p3
    {0xfffe0148,     8,      11,       0,        0,        0x0,       4},         //lum_ver_delta_lut_p2
    {0xfffe0148,     4,       7,       0,        0,        0x0,       6},         //lum_ver_delta_lut_p1
    {0xfffe0148,     0,       3,       0,        0,        0x0,       0},         //lum_ver_delta_lut_p0

    /*PQ_SOFT_DB_HC_DELTA_2*/
    {0xfffe0149,    28,      31,       0,        0,        0x0,       0},         //chr_hor_delta_lut_p15
    {0xfffe0149,    24,      27,       0,        0,        0x0,       0},         //chr_hor_delta_lut_p14
    {0xfffe0149,    20,      23,       0,        0,        0x0,       0},         //chr_hor_delta_lut_p13
    {0xfffe0149,    16,      19,       0,        0,        0x0,       0},         //chr_hor_delta_lut_p12
    {0xfffe0149,    12,      15,       0,        0,        0x0,       0},         //chr_hor_delta_lut_p11
    {0xfffe0149,     8,      11,       0,        0,        0x0,       0},         //chr_hor_delta_lut_p10
    {0xfffe0149,     4,       7,       0,        0,        0x0,       0},         //chr_hor_delta_lut_p9
    {0xfffe0149,     0,       3,       0,        0,        0x0,       0},         //chr_hor_delta_lut_p8
    /*PQ_SOFT_DB_HC_DELTA_1*/
    {0xfffe014a,    28,      31,       0,        0,        0x0,       0},         //chr_hor_delta_lut_p7
    {0xfffe014a,    24,      27,       0,        0,        0x0,       0},         //chr_hor_delta_lut_p6
    {0xfffe014a,    20,      23,       0,        0,        0x0,       0},         //chr_hor_delta_lut_p5
    {0xfffe014a,    16,      19,       0,        0,        0x0,       0},         //chr_hor_delta_lut_p4
    {0xfffe014a,    12,      15,       0,        0,        0x0,       4},         //chr_hor_delta_lut_p3
    {0xfffe014a,     8,      11,       0,        0,        0x0,       8},         //chr_hor_delta_lut_p2
    {0xfffe014a,     4,       7,       0,        0,        0x0,       8},         //chr_hor_delta_lut_p1
    {0xfffe014a,     0,       3,       0,        0,        0x0,       8},         //chr_hor_delta_lut_p0

    /*PQ_SOFT_DB_STR_FADE_1_2*/
    {0xfffe0151,     3,       5,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p11
    {0xfffe0151,     0,       2,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p10
    /*PQ_SOFT_DB_STR_FADE_1_1*/
    {0xfffe0152,    27,      29,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p9
    {0xfffe0152,    24,      26,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p8
    {0xfffe0152,    21,      23,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p7
    {0xfffe0152,    18,      20,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p6
    {0xfffe0152,    15,      17,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p5
    {0xfffe0152,    12,      14,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p4
    {0xfffe0152,     9,      11,        0,        0,    0x200,        3},         //lum_h_str_fade_lut_p3
    {0xfffe0152,     6,       8,        0,        0,    0x200,        3},         //lum_h_str_fade_lut_p2
    {0xfffe0152,     3,       5,        0,        0,    0x200,        4},         //lum_h_str_fade_lut_p1
    {0xfffe0152,     0,       2,        0,        0,    0x200,        4},         //lum_h_str_fade_lut_p0
    /*PQ_SOFT_DB_STR_FADE_1_4_2*/
    {0xfffe0153,     3,       5,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p11
    {0xfffe0153,     0,       2,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p10
    /*PQ_SOFT_DB_STR_FADE_1_4_1*/
    {0xfffe0154,    27,      29,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p9
    {0xfffe0154,    24,      26,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p8
    {0xfffe0154,    21,      23,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p7
    {0xfffe0154,    18,      20,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p6
    {0xfffe0154,    15,      17,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p5
    {0xfffe0154,    12,      14,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p4
    {0xfffe0154,     9,      11,        0,        0,    0x200,        3},         //lum_h_str_fade_lut_p3
    {0xfffe0154,     6,       8,        0,        0,    0x200,        3},         //lum_h_str_fade_lut_p2
    {0xfffe0154,     3,       5,        0,        0,    0x200,        4},         //lum_h_str_fade_lut_p1
    {0xfffe0154,     0,       2,        0,        0,    0x200,        4},         //lum_h_str_fade_lut_p0
    /*PQ_SOFT_DB_STR_FADE_2_2*/
    {0xfffe0155,     3,       5,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p11
    {0xfffe0155,     0,       2,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p10
    /*PQ_SOFT_DB_STR_FADE_2_1*/
    {0xfffe0156,    27,      29,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p9
    {0xfffe0156,    24,      26,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p8
    {0xfffe0156,    21,      23,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p7
    {0xfffe0156,    18,      20,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p6
    {0xfffe0156,    15,      17,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p5
    {0xfffe0156,    12,      14,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p4
    {0xfffe0156,     9,      11,        0,        0,    0x200,        3},         //lum_h_str_fade_lut_p3
    {0xfffe0156,     6,       8,        0,        0,    0x200,        3},         //lum_h_str_fade_lut_p2
    {0xfffe0156,     3,       5,        0,        0,    0x200,        4},         //lum_h_str_fade_lut_p1
    {0xfffe0156,     0,       2,        0,        0,    0x200,        4},         //lum_h_str_fade_lut_p0
    /*PQ_SOFT_DB_STR_FADE_3_2*/
    {0xfffe0157,     3,       5,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p11
    {0xfffe0157,     0,       2,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p10
    /*PQ_SOFT_DB_STR_FADE_3_1*/
    {0xfffe0158,    27,      29,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p9
    {0xfffe0158,    24,      26,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p8
    {0xfffe0158,    21,      23,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p7
    {0xfffe0158,    18,      20,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p6
    {0xfffe0158,    15,      17,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p5
    {0xfffe0158,    12,      14,        0,        0,    0x200,        0},         //lum_h_str_fade_lut_p4
    {0xfffe0158,     9,      11,        0,        0,    0x200,        3},         //lum_h_str_fade_lut_p3
    {0xfffe0158,     6,       8,        0,        0,    0x200,        3},         //lum_h_str_fade_lut_p2
    {0xfffe0158,     3,       5,        0,        0,    0x200,        4},         //lum_h_str_fade_lut_p1
    {0xfffe0158,     0,       2,        0,        0,    0x200,        4},         //lum_h_str_fade_lut_p0

    /*PQ_SOFT_DB_STR_V_FADE_2*/
    {0xfffe0159,     9,      11,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p11
    {0xfffe0159,     6,       8,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p10
    /*PQ_SOFT_DB_STR_V_FADE_1*/
    {0xfffe015a,    27,      29,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p9
    {0xfffe015a,    24,      26,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p8
    {0xfffe015a,    21,      23,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p7
    {0xfffe015a,    18,      20,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p6
    {0xfffe015a,    15,      17,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p5
    {0xfffe015a,    12,      14,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p4
    {0xfffe015a,     9,      11,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p3
    {0xfffe015a,     6,       8,        0,        0,    0x200,        0},         //lum_v_str_fade_lut_p2
    {0xfffe015a,     3,       5,        0,        0,    0x200,        1},         //lum_v_str_fade_lut_p1
    {0xfffe015a,     0,       2,        0,        0,    0x200,        2},         //lum_v_str_fade_lut_p0

    /*PQ_SOFT_DB_DIR_STR*/
    {0xfffe0161,    28,      31,        0,        0,    0x200,        0},         //dir_str_gain_lut_p7
    {0xfffe0161,    24,      27,        0,        0,    0x200,        0},         //dir_str_gain_lut_p6
    {0xfffe0161,    20,      23,        0,        0,    0x200,        0},         //dir_str_gain_lut_p5
    {0xfffe0161,    16,      19,        0,        0,    0x200,        0},         //dir_str_gain_lut_p4
    {0xfffe0161,    12,      15,        0,        0,    0x200,        1},         //dir_str_gain_lut_p3
    {0xfffe0161,     8,      11,        0,        0,    0x200,        3},         //dir_str_gain_lut_p2
    {0xfffe0161,     4,       7,        0,        0,    0x200,        5},         //dir_str_gain_lut_p1
    {0xfffe0161,     0,       3,        0,        0,    0x200,        8},         //dir_str_gain_lut_p0
    /*PQ_SOFT_DB_DIR_GAIN_2*/
    {0xfffe0163,    28,      31,        0,        0,    0x200,        0},         //dir_str_lut_p15
    {0xfffe0163,    24,      27,        0,        0,    0x200,        0},         //dir_str_lut_p14
    {0xfffe0163,    20,      23,        0,        0,    0x200,        0},         //dir_str_lut_p13
    {0xfffe0163,    16,      19,        0,        0,    0x200,        0},         //dir_str_lut_p12
    {0xfffe0163,    12,      15,        0,        0,    0x200,        0},         //dir_str_lut_p11
    {0xfffe0163,     8,      11,        0,        0,    0x200,        0},         //dir_str_lut_p10
    {0xfffe0163,     4,       7,        0,        0,    0x200,        1},         //dir_str_lut_p9
    {0xfffe0163,     0,       3,        0,        0,    0x200,        1},         //dir_str_lut_p8
    /*PQ_SOFT_DB_DIR_GAIN_1*/
    {0xfffe0164,    28,      31,        0,        0,    0x200,        6},         //dir_str_lut_p7
    {0xfffe0164,    24,      27,        0,        0,    0x200,        8},         //dir_str_lut_p6
    {0xfffe0164,    20,      23,        0,        0,    0x200,        8},         //dir_str_lut_p5
    {0xfffe0164,    16,      19,        0,        0,    0x200,        8},         //dir_str_lut_p4
    {0xfffe0164,    12,      15,        0,        0,    0x200,        8},         //dir_str_lut_p3
    {0xfffe0164,     8,      11,        0,        0,    0x200,        8},         //dir_str_lut_p2
    {0xfffe0164,     4,       7,        0,        0,    0x200,        8},         //dir_str_lut_p1
    {0xfffe0164,     0,       3,        0,        0,    0x200,        8},         //dir_str_lut_p0

    /*PQ_SOFT_DB_DYN_PARA_STR*/
    {0xfffe0171,     0,       7,        0,        0,    0x200,        8},         //lum_hor_adj_gain
    {0xfffe0172,     0,       7,        0,        0,    0x200,        6},         //lum_hor_hf_diff_core
    {0xfffe0173,     0,       7,        0,        0,    0x200,        8},         //lum_hor_hf_diff_gain1
    {0xfffe0174,     0,       7,        0,        0,    0x200,        8},         //lum_hor_hf_diff_gain2
    {0xfffe0175,     0,       7,        0,        0,    0x200,        8},         //lum_hor_hf_var_core
    {0xfffe0176,     0,       7,        0,        0,    0x200,        6},         //lum_hor_hf_var_gain1
    {0xfffe0177,     0,       7,        0,        0,    0x200,        8},         //lum_hor_hf_var_gain2
    {0xfffe0178,     0,       7,        0,        0,    0x200,        8},         //lum_hor_bord_adj_gain
    {0xfffe0179,     0,       7,        0,        0,    0x200,        8},         //grad_sub_ratio

    /*PQ_SOFT_DB_DYN_PARA_MID*/
    {0xfffe0181,     0,       7,        0,        0,    0x200,        8},         //lum_hor_adj_gain
    {0xfffe0182,     0,       7,        0,        0,    0x200,        6},         //lum_hor_hf_diff_core
    {0xfffe0183,     0,       7,        0,        0,    0x200,        8},         //lum_hor_hf_diff_gain1
    {0xfffe0184,     0,       7,        0,        0,    0x200,        8},         //lum_hor_hf_diff_gain2
    {0xfffe0185,     0,       7,        0,        0,    0x200,        8},         //lum_hor_hf_var_core
    {0xfffe0186,     0,       7,        0,        0,    0x200,        6},         //lum_hor_hf_var_gain1
    {0xfffe0187,     0,       7,        0,        0,    0x200,        8},         //lum_hor_hf_var_gain2
    {0xfffe0188,     0,       7,        0,        0,    0x200,        8},         //lum_hor_bord_adj_gain
    {0xfffe0189,     0,       7,        0,        0,    0x200,        8},         //grad_sub_ratio

    /*PQ_SOFT_DB_DYN_PARA_WEK*/
    {0xfffe0191,     0,       7,        0,        0,    0x200,        8},         //lum_hor_adj_gain
    {0xfffe0192,     0,       7,        0,        0,    0x200,        6},         //lum_hor_hf_diff_core
    {0xfffe0193,     0,       7,        0,        0,    0x200,        8},         //lum_hor_hf_diff_gain1
    {0xfffe0194,     0,       7,        0,        0,    0x200,        8},         //lum_hor_hf_diff_gain2
    {0xfffe0195,     0,       7,        0,        0,    0x200,        8},         //lum_hor_hf_var_core
    {0xfffe0196,     0,       7,        0,        0,    0x200,        6},         //lum_hor_hf_var_gain1
    {0xfffe0197,     0,       7,        0,        0,    0x200,        8},         //lum_hor_hf_var_gain2
    {0xfffe0198,     0,       7,        0,        0,    0x200,        8},         //lum_hor_bord_adj_gain
    {0xfffe0199,     0,       7,        0,        0,    0x200,        8},         //grad_sub_ratio
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

