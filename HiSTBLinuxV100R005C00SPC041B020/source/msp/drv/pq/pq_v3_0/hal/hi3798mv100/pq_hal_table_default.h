#ifndef __PQ_MNG_PQ_TABLE_DEFAULT_H__
#define __PQ_MNG_PQ_TABLE_DEFAULT_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

static PQ_PHY_REG_S sg_stPhyReg[] =
{
    /*Addr              Lsb          Msb      SourceMode   OutputMode  Module     Value             Description*/

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
    /*Sharpen_Luma_VPSS_VHD*/
    {0xf8cb0180,    31,      31,        1,        2,      0x4,        1},         //lti_en
    {0xf8cb0180,    30,      30,        1,        2,      0x4,        0},         //lti_demo_en
    {0xf8cb0180,    16,      27,        1,        2,      0x4,      180},         //lgain_ratio
    {0xf8cb0180,     8,      15,        1,        2,      0x4,      127},         //lmixing_ratio
    {0xf8cb0184,     0,       7,        1,        2,      0x4,      -19},         //lhpass_coef0
    {0xf8cb0184,     8,      15,        1,        2,      0x4,       -8},         //lhpass_coef1
    {0xf8cb0184,    16,      23,        1,        2,      0x4,       -2},         //lhpass_coef2
    {0xf8cb0184,    24,      31,        1,        2,      0x4,       -1},         //lhpass_coef3
    {0xf8cb0180,     0,       7,        1,        2,      0x4,       -1},         //lhpass_coef4
    {0xf8cb0188,    20,      31,        1,        2,      0x4,        2},         //lcoring_thd
    {0xf8cb0188,    10,      19,        1,        2,      0x4,        3},         //lunder_swing
    {0xf8cb0188,     0,       9,        1,        2,      0x4,        3},         //lover_swing
    {0xf8cb018c,     0,      15,        1,        2,      0x4,       50},         //lhfreq_thd0
    {0xf8cb018c,    16,      31,        1,        2,      0x4,       51},         //lhfreq_thd1
    {0xf8cb0190,     0,       7,        1,        2,      0x4,      128},         //lgain_coef0
    {0xf8cb0190,     8,      15,        1,        2,      0x4,      128},         //lgain_coef1
    {0xf8cb0190,    16,      23,        1,        2,      0x4,      128},         //lgain_coef2
    /*Sharpen_Chroma_VPSS_VHD*/
    {0xf8cb0194,    31,      31,        1,        2,      0x4,        1},         //cti_en
    {0xf8cb0194,    30,      30,        1,        2,      0x4,        0},         //cti_demo_en
    {0xf8cb0194,    16,      27,        1,        2,      0x4,      256},         //cgain_ratio
    {0xf8cb0194,     8,      15,        1,        2,      0x4,      127},         //cmixing_ratio
    {0xf8cb0198,     0,       7,        1,        2,      0x4,      -21},         //chpass_coef0
    {0xf8cb0198,     8,      15,        1,        2,      0x4,      -21},         //chpass_coef1
    {0xf8cb0198,    16,      23,        1,        2,      0x4,      -11},         //chpass_coef2
    {0xf8cb019c,    20,      31,        1,        2,      0x4,        4},         //ccoring_thd
    {0xf8cb019c,    10,      19,        1,        2,      0x4,        4},         //cunder_swing
    {0xf8cb019c,     0,       9,        1,        2,      0x4,        4},         //cover_swing
    /*HD Source-->HD Output*/
    /*Sharpen_Luma_VPSS_VHD*/
    {0xf8cb0180,    31,      31,        2,        2,      0x4,        1},         //lti_en
    {0xf8cb0180,    30,      30,        2,        2,      0x4,        0},         //lti_demo_en
    {0xf8cb0180,    16,      27,        2,        2,      0x4,      180},         //lgain_ratio
    {0xf8cb0180,     8,      15,        2,        2,      0x4,      127},         //lmixing_ratio
    {0xf8cb0184,     0,       7,        2,        2,      0x4,      -19},         //lhpass_coef0
    {0xf8cb0184,     8,      15,        2,        2,      0x4,       -8},         //lhpass_coef1
    {0xf8cb0184,    16,      23,        2,        2,      0x4,       -2},         //lhpass_coef2
    {0xf8cb0184,    24,      31,        2,        2,      0x4,       -1},         //lhpass_coef3
    {0xf8cb0180,     0,       7,        2,        2,      0x4,       -1},         //lhpass_coef4
    {0xf8cb0188,    20,      31,        2,        2,      0x4,        2},         //lcoring_thd
    {0xf8cb0188,    10,      19,        2,        2,      0x4,        3},         //lunder_swing
    {0xf8cb0188,     0,       9,        2,        2,      0x4,        3},         //lover_swing
    {0xf8cb018c,     0,      15,        2,        2,      0x4,       50},         //lhfreq_thd0
    {0xf8cb018c,    16,      31,        2,        2,      0x4,       51},         //lhfreq_thd1
    {0xf8cb0190,     0,       7,        2,        2,      0x4,      128},         //lgain_coef0
    {0xf8cb0190,     8,      15,        2,        2,      0x4,      128},         //lgain_coef1
    {0xf8cb0190,    16,      23,        2,        2,      0x4,      128},         //lgain_coef2
    /*Sharpen_Chroma_VPSS_VHD*/
    {0xf8cb0194,    31,      31,        2,        2,      0x4,        1},         //cti_en
    {0xf8cb0194,    30,      30,        2,        2,      0x4,        0},         //cti_demo_en
    {0xf8cb0194,    16,      27,        2,        2,      0x4,      256},         //cgain_ratio
    {0xf8cb0194,     8,      15,        2,        2,      0x4,      127},         //cmixing_ratio
    {0xf8cb0198,     0,       7,        2,        2,      0x4,      -21},         //chpass_coef0
    {0xf8cb0198,     8,      15,        2,        2,      0x4,      -21},         //chpass_coef1
    {0xf8cb0198,    16,      23,        2,        2,      0x4,      -11},         //chpass_coef2
    {0xf8cb019c,    20,      31,        2,        2,      0x4,        4},         //ccoring_thd
    {0xf8cb019c,    10,      19,        2,        2,      0x4,        4},         //cunder_swing
    {0xf8cb019c,     0,       9,        2,        2,      0x4,        4},         //cover_swing
    /*UHD Source-->HD Output*/
    /*Sharpen_Luma_VPSS_VHD*/
    {0xf8cb0180,    31,      31,        3,        2,      0x4,        1},         //lti_en
    {0xf8cb0180,    30,      30,        3,        2,      0x4,        0},         //lti_demo_en
    {0xf8cb0180,    16,      27,        3,        2,      0x4,      180},         //lgain_ratio
    {0xf8cb0180,     8,      15,        3,        2,      0x4,      127},         //lmixing_ratio
    {0xf8cb0184,     0,       7,        3,        2,      0x4,      -19},         //lhpass_coef0
    {0xf8cb0184,     8,      15,        3,        2,      0x4,       -8},         //lhpass_coef1
    {0xf8cb0184,    16,      23,        3,        2,      0x4,       -2},         //lhpass_coef2
    {0xf8cb0184,    24,      31,        3,        2,      0x4,       -1},         //lhpass_coef3
    {0xf8cb0180,     0,       7,        3,        2,      0x4,       -1},         //lhpass_coef4
    {0xf8cb0188,    20,      31,        3,        2,      0x4,        2},         //lcoring_thd
    {0xf8cb0188,    10,      19,        3,        2,      0x4,        3},         //lunder_swing
    {0xf8cb0188,     0,       9,        3,        2,      0x4,        3},         //lover_swing
    {0xf8cb018c,     0,      15,        3,        2,      0x4,       50},         //lhfreq_thd0
    {0xf8cb018c,    16,      31,        3,        2,      0x4,       51},         //lhfreq_thd1
    {0xf8cb0190,     0,       7,        3,        2,      0x4,      128},         //lgain_coef0
    {0xf8cb0190,     8,      15,        3,        2,      0x4,      128},         //lgain_coef1
    {0xf8cb0190,    16,      23,        3,        2,      0x4,      128},         //lgain_coef2
    /*Sharpen_Chroma_VPSS_VHD*/
    {0xf8cb0194,    31,      31,        3,        2,      0x4,        1},         //cti_en
    {0xf8cb0194,    30,      30,        3,        2,      0x4,        0},         //cti_demo_en
    {0xf8cb0194,    16,      27,        3,        2,      0x4,      256},         //cgain_ratio
    {0xf8cb0194,     8,      15,        3,        2,      0x4,      127},         //cmixing_ratio
    {0xf8cb0198,     0,       7,        3,        2,      0x4,      -21},         //chpass_coef0
    {0xf8cb0198,     8,      15,        3,        2,      0x4,      -21},         //chpass_coef1
    {0xf8cb0198,    16,      23,        3,        2,      0x4,      -11},         //chpass_coef2
    {0xf8cb019c,    20,      31,        3,        2,      0x4,        4},         //ccoring_thd
    {0xf8cb019c,    10,      19,        3,        2,      0x4,        4},         //cunder_swing
    {0xf8cb019c,     0,       9,        3,        2,      0x4,        4},         //cover_swing
    /*SD Source-->UHD Output*/
    /*Sharpen_Luma_VPSS_VHD*/
    {0xf8cb0180,    31,      31,        1,        3,      0x4,        1},         //lti_en
    {0xf8cb0180,    30,      30,        1,        3,      0x4,        0},         //lti_demo_en
    {0xf8cb0180,    16,      27,        1,        3,      0x4,      180},         //lgain_ratio
    {0xf8cb0180,     8,      15,        1,        3,      0x4,      127},         //lmixing_ratio
    {0xf8cb0184,     0,       7,        1,        3,      0x4,      -19},         //lhpass_coef0
    {0xf8cb0184,     8,      15,        1,        3,      0x4,       -8},         //lhpass_coef1
    {0xf8cb0184,    16,      23,        1,        3,      0x4,       -2},         //lhpass_coef2
    {0xf8cb0184,    24,      31,        1,        3,      0x4,       -1},         //lhpass_coef3
    {0xf8cb0180,     0,       7,        1,        3,      0x4,       -1},         //lhpass_coef4
    {0xf8cb0188,    20,      31,        1,        3,      0x4,        2},         //lcoring_thd
    {0xf8cb0188,    10,      19,        1,        3,      0x4,        3},         //lunder_swing
    {0xf8cb0188,     0,       9,        1,        3,      0x4,        3},         //lover_swing
    {0xf8cb018c,     0,      15,        1,        3,      0x4,       50},         //lhfreq_thd0
    {0xf8cb018c,    16,      31,        1,        3,      0x4,       51},         //lhfreq_thd1
    {0xf8cb0190,     0,       7,        1,        3,      0x4,      128},         //lgain_coef0
    {0xf8cb0190,     8,      15,        1,        3,      0x4,      128},         //lgain_coef1
    {0xf8cb0190,    16,      23,        1,        3,      0x4,      128},         //lgain_coef2
    /*Sharpen_Chroma_VPSS_VHD*/
    {0xf8cb0194,    31,      31,        1,        3,      0x4,        1},         //cti_en
    {0xf8cb0194,    30,      30,        1,        3,      0x4,        0},         //cti_demo_en
    {0xf8cb0194,    16,      27,        1,        3,      0x4,      256},         //cgain_ratio
    {0xf8cb0194,     8,      15,        1,        3,      0x4,      127},         //cmixing_ratio
    {0xf8cb0198,     0,       7,        1,        3,      0x4,      -21},         //chpass_coef0
    {0xf8cb0198,     8,      15,        1,        3,      0x4,      -21},         //chpass_coef1
    {0xf8cb0198,    16,      23,        1,        3,      0x4,      -11},         //chpass_coef2
    {0xf8cb019c,    20,      31,        1,        3,      0x4,        4},         //ccoring_thd
    {0xf8cb019c,    10,      19,        1,        3,      0x4,        4},         //cunder_swing
    {0xf8cb019c,     0,       9,        1,        3,      0x4,        4},         //cover_swing
    /*HD Source-->UHD Output*/
    /*Sharpen_Luma_VPSS_VHD*/
    {0xf8cb0180,    31,      31,        2,        3,      0x4,        1},         //lti_en
    {0xf8cb0180,    30,      30,        2,        3,      0x4,        0},         //lti_demo_en
    {0xf8cb0180,    16,      27,        2,        3,      0x4,      180},         //lgain_ratio
    {0xf8cb0180,     8,      15,        2,        3,      0x4,      127},         //lmixing_ratio
    {0xf8cb0184,     0,       7,        2,        3,      0x4,      -19},         //lhpass_coef0
    {0xf8cb0184,     8,      15,        2,        3,      0x4,       -8},         //lhpass_coef1
    {0xf8cb0184,    16,      23,        2,        3,      0x4,       -2},         //lhpass_coef2
    {0xf8cb0184,    24,      31,        2,        3,      0x4,       -1},         //lhpass_coef3
    {0xf8cb0180,     0,       7,        2,        3,      0x4,       -1},         //lhpass_coef4
    {0xf8cb0188,    20,      31,        2,        3,      0x4,        2},         //lcoring_thd
    {0xf8cb0188,    10,      19,        2,        3,      0x4,        3},         //lunder_swing
    {0xf8cb0188,     0,       9,        2,        3,      0x4,        3},         //lover_swing
    {0xf8cb018c,     0,      15,        2,        3,      0x4,       50},         //lhfreq_thd0
    {0xf8cb018c,    16,      31,        2,        3,      0x4,       51},         //lhfreq_thd1
    {0xf8cb0190,     0,       7,        2,        3,      0x4,      128},         //lgain_coef0
    {0xf8cb0190,     8,      15,        2,        3,      0x4,      128},         //lgain_coef1
    {0xf8cb0190,    16,      23,        2,        3,      0x4,      128},         //lgain_coef2
    /*Sharpen_Chroma_VPSS_VHD*/
    {0xf8cb0194,    31,      31,        2,        3,      0x4,        1},         //cti_en
    {0xf8cb0194,    30,      30,        2,        3,      0x4,        0},         //cti_demo_en
    {0xf8cb0194,    16,      27,        2,        3,      0x4,      256},         //cgain_ratio
    {0xf8cb0194,     8,      15,        2,        3,      0x4,      127},         //cmixing_ratio
    {0xf8cb0198,     0,       7,        2,        3,      0x4,      -21},         //chpass_coef0
    {0xf8cb0198,     8,      15,        2,        3,      0x4,      -21},         //chpass_coef1
    {0xf8cb0198,    16,      23,        2,        3,      0x4,      -11},         //chpass_coef2
    {0xf8cb019c,    20,      31,        2,        3,      0x4,        4},         //ccoring_thd
    {0xf8cb019c,    10,      19,        2,        3,      0x4,        4},         //cunder_swing
    {0xf8cb019c,     0,       9,        2,        3,      0x4,        4},         //cover_swing
    /*UHD Source-->UHD Output*/
    /*Sharpen_Luma_VPSS_VHD*/
    {0xf8cb0180,    31,      31,        3,        3,      0x4,        1},         //lti_en
    {0xf8cb0180,    30,      30,        3,        3,      0x4,        0},         //lti_demo_en
    {0xf8cb0180,    16,      27,        3,        3,      0x4,      180},         //lgain_ratio
    {0xf8cb0180,     8,      15,        3,        3,      0x4,      127},         //lmixing_ratio
    {0xf8cb0184,     0,       7,        3,        3,      0x4,      -19},         //lhpass_coef0
    {0xf8cb0184,     8,      15,        3,        3,      0x4,       -8},         //lhpass_coef1
    {0xf8cb0184,    16,      23,        3,        3,      0x4,       -2},         //lhpass_coef2
    {0xf8cb0184,    24,      31,        3,        3,      0x4,       -1},         //lhpass_coef3
    {0xf8cb0180,     0,       7,        3,        3,      0x4,       -1},         //lhpass_coef4
    {0xf8cb0188,    20,      31,        3,        3,      0x4,        2},         //lcoring_thd
    {0xf8cb0188,    10,      19,        3,        3,      0x4,        3},         //lunder_swing
    {0xf8cb0188,     0,       9,        3,        3,      0x4,        3},         //lover_swing
    {0xf8cb018c,     0,      15,        3,        3,      0x4,       50},         //lhfreq_thd0
    {0xf8cb018c,    16,      31,        3,        3,      0x4,       51},         //lhfreq_thd1
    {0xf8cb0190,     0,       7,        3,        3,      0x4,      128},         //lgain_coef0
    {0xf8cb0190,     8,      15,        3,        3,      0x4,      128},         //lgain_coef1
    {0xf8cb0190,    16,      23,        3,        3,      0x4,      128},         //lgain_coef2
    /*Sharpen_Chroma_VPSS_VHD*/
    {0xf8cb0194,    31,      31,        3,        3,      0x4,        1},         //cti_en
    {0xf8cb0194,    30,      30,        3,        3,      0x4,        0},         //cti_demo_en
    {0xf8cb0194,    16,      27,        3,        3,      0x4,      256},         //cgain_ratio
    {0xf8cb0194,     8,      15,        3,        3,      0x4,      127},         //cmixing_ratio
    {0xf8cb0198,     0,       7,        3,        3,      0x4,      -21},         //chpass_coef0
    {0xf8cb0198,     8,      15,        3,        3,      0x4,      -21},         //chpass_coef1
    {0xf8cb0198,    16,      23,        3,        3,      0x4,      -11},         //chpass_coef2
    {0xf8cb019c,    20,      31,        3,        3,      0x4,        4},         //ccoring_thd
    {0xf8cb019c,    10,      19,        3,        3,      0x4,        4},         //cunder_swing
    {0xf8cb019c,     0,       9,        3,        3,      0x4,        4},         //cover_swing

    /*************************************DNR*****************************************/
    /*DNR_CTRL*/
    {0xf8cb0000,    11,      11,        0,        0,      0x8,        1},         //dr_en
    {0xf8cb01bc,    24,      24,        0,        0,      0x8,        0},         //dr_demo_en
    /*DR_FILTER*/
    {0xf8cb01b8,    24,      28,        0,        0,      0x8,        8},         //dr_thr_flat3x3zone
    {0xf8cb01b8,    16,      20,        0,        0,      0x8,       16},         //dr_thr_maxsimilarpixdiff
    {0xf8cb01bc,    16,      20,        0,        0,      0x8,        8},         //dr_betascale
    {0xf8cb01bc,     0,       4,        0,        0,      0x8,        8},         //dr_alphascale
    /*DETECT_INFO*/
    {0xf8cb01cc,    16,      23,        0,        0,      0x8,       64},         //thd_cntrst8
    {0xf8cb01cc,     8,      15,        0,        0,      0x8,       40},         //thd_maxgrad
    {0xf8cb01cc,     4,       7,        0,        0,      0x8,        4},         //thr_intl_cnt
    {0xf8cb01cc,     0,       3,        0,        0,      0x8,        1},         //thr_intl_colcnt

    /**************************************DCI****************************************/
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
    /*DCI_GLB_GAIN_UHD_SOURCE*/
    {0xF8cc412c,    26,      31,        3,        0,      0x10,      48},         //dci_glb_gain_uhd0
    {0xF8cc412c,    20,      25,        3,        0,      0x10,      48},         //dci_glb_gain_uhd0
    {0xF8cc412c,    14,      19,        3,        0,      0x10,      42},         //dci_glb_gain_uhd0
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
    {0xF8cc415c,    12,      21,        0,        0,      0x10,      12},        //dci_gainneg_slp1
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
    {0xf8cc41a0,    31,      31,        0,        0,      0x20,       1},         //acm_en
    {0xf8cc41a0,    30,      30,        0,        0,      0x20,       0},         //acm_dbg_en
    {0xf8cc41a0,    29,      29,        0,        0,      0x20,       1},         //acm_stretch
    {0xf8cc41a0,    28,      28,        0,        0,      0x20,       1},         //acm_cliprange
    {0xf8cc41a0,    27,      27,        0,        0,      0x20,       0},         //acm_cliporwrap
    {0xf8cc41a0,    0,       8,         0,        0,      0x20,       0},         //acm_cbcrthr
    /*VP0_ACM_GAIN_SD_SOURCE*/
    {0xf8cc41a4,    20,      29,        1,        0,      0x20,      80},         //acm_gain_luma_sd
    {0xf8cc41a4,    10,      19,        1,        0,      0x20,      80},         //acm_gain_hue_sd
    {0xf8cc41a4,    0,       9,         1,        0,      0x20,      80},         //acm_gain_sat_sd
    /*VP0_ACM_GAIN_HD_SOURCE*/
    {0xf8cc41a4,    20,      29,        2,        0,      0x20,      80},         //acm_gain_luma_hd
    {0xf8cc41a4,    10,      19,        2,        0,      0x20,      80},         //acm_gain_hue_hd
    {0xf8cc41a4,    0,       9,         2,        0,      0x20,      80},         //acm_gain_sat_hd
    /*VP0_ACM_GAIN_UHD_SOURCE*/
    {0xf8cc41a4,    20,      29,        3,        0,      0x20,      80},         //acm_gain_luma_uhd
    {0xf8cc41a4,    10,      19,        3,        0,      0x20,      80},         //acm_gain_hue_uhd
    {0xf8cc41a4,    0,       9,         3,        0,      0x20,      80},         //acm_gain_sat_uhd

    /************************************SR*************************************/
    /*SD Source*/
    /*SR_CTRL*/
    {0xF8cc0d80,    29,      30,        1,        0,     0x100,       0},         //sr_disp_state
    {0xF8cc0d94,    31,      31,        1,        0,     0x100,       1},         //sharp_en
    {0xF8cc0d94,    30,      30,        1,        0,     0x100,       0},         //hsh_nodir
    {0xF8cc0d94,     0,      11,        1,        0,     0x100,     128},         //ensc_detail_gain
    {0xF8cc0d98,    28,      31,        1,        0,     0x100,       4},         //sharp_coring
    {0xF8cc0d98,    16,      25,        1,        0,     0x100,     128},         //sharp_gain
    /*SR_LDIR_RATIO*/
    {0xF8cc0d84,    0,        5,        1,        0,     0x100,      14},         //grd_sub_ratio
    /*SR_LTEX_THR*/
    {0xF8cc0d88,    24,      31,        1,        0,     0x100,       4},         //grd_thrh_subl
    {0xF8cc0d88,    12,      22,        1,        0,     0x100,      16},         //grd_thrh
    {0xF8cc0d88,    0,       10,        1,        0,     0x100,     -48},         //grd_thrl
    /*SR_LTEX_ADD*/
    {0xF8cc0d8c,    0,        7,        1,        0,     0x100,       8},         //grd_add_ctrl
    /*SR_DIPL_DETAIL*/
    {0xF8cc0d90,    0,       11,        1,        0,     0x100,       0},         //disc_detail_gain
    /*SR_SHARPEN_EN*/
    {0xF8cc0d98,    8,       15,        1,        0,     0x100,      80},         //over_thrsh
    {0xF8cc0d98,    0,        7,        1,        0,     0x100,      80},         //under_thrsh
    /*SR_SMOOTH*/
    {0xF8cc0d9C,    31,      31,        1,        0,     0x100,       1},         //smooth_en
    {0xF8cc0d9C,    30,      30,        1,        0,     0x100,       1},         //plus_en
    {0xF8cc0d9C,    0,        9,        1,        0,     0x100,     140},         //plus_ctrl
    /*HD Source*/
    /*SR_CTRL*/
    {0xF8cc0d80,    29,      30,        2,        0,     0x100,       0},         //sr_disp_state
    {0xF8cc0d94,    31,      31,        2,        0,     0x100,       1},         //sharp_en
    {0xF8cc0d94,    30,      30,        2,        0,     0x100,       0},         //hsh_nodir
    {0xF8cc0d94,     0,      11,        2,        0,     0x100,     128},         //ensc_detail_gain
    {0xF8cc0d98,    28,      31,        2,        0,     0x100,       4},         //sharp_coring
    {0xF8cc0d98,    16,      25,        2,        0,     0x100,      80},         //sharp_gain
    /*SR_LDIR_RATIO*/
    {0xF8cc0d84,    0,        5,        2,        0,     0x100,      14},         //grd_sub_ratio
    /*SR_LTEX_THR*/
    {0xF8cc0d88,    24,      31,        2,        0,     0x100,       4},         //grd_thrh_subl
    {0xF8cc0d88,    12,      22,        2,        0,     0x100,      16},         //grd_thrh
    {0xF8cc0d88,    0,       10,        2,        0,     0x100,     -48},         //grd_thrl
    /*SR_LTEX_ADD*/
    {0xF8cc0d8c,    0,        7,        2,        0,     0x100,       8},         //grd_add_ctrl
    /*SR_DIPL_DETAIL*/
    {0xF8cc0d90,    0,       11,        2,        0,     0x100,       0},         //disc_detail_gain
    /*SR_SHARPEN_EN*/
    {0xF8cc0d98,    8,       15,        2,        0,     0x100,      80},         //over_thrsh
    {0xF8cc0d98,    0,        7,        2,        0,     0x100,      80},         //under_thrsh
    /*SR_SMOOTH*/
    {0xF8cc0d9C,    31,      31,        2,        0,     0x100,       1},         //smooth_en
    {0xF8cc0d9C,    30,      30,        2,        0,     0x100,       1},         //plus_en
    {0xF8cc0d9C,    0,        9,        2,        0,     0x100,     140},         //plus_ctrl
    /*UHD Source*/
    /*SR_CTRL*/
    {0xF8cc0d80,    29,      30,        3,        0,     0x100,       0},         //sr_disp_state
    {0xF8cc0d94,    31,      31,        3,        0,     0x100,       1},         //sharp_en
    {0xF8cc0d94,    30,      30,        3,        0,     0x100,       0},         //hsh_nodir
    {0xF8cc0d94,     0,      11,        3,        0,     0x100,     128},         //ensc_detail_gain
    {0xF8cc0d98,    28,      31,        3,        0,     0x100,       4},         //sharp_coring
    {0xF8cc0d98,    16,      25,        3,        0,     0x100,      80},         //sharp_gain
    /*SR_LDIR_RATIO*/
    {0xF8cc0d84,    0,        5,        3,        0,     0x100,      14},         //grd_sub_ratio
    /*SR_LTEX_THR*/
    {0xF8cc0d88,    24,      31,        3,        0,     0x100,       4},         //grd_thrh_subl
    {0xF8cc0d88,    12,      22,        3,        0,     0x100,      16},         //grd_thrh
    {0xF8cc0d88,    0,       10,        3,        0,     0x100,     -48},         //grd_thrl
    /*SR_LTEX_ADD*/
    {0xF8cc0d8c,    0,        7,        3,        0,     0x100,       8},         //grd_add_ctrl
    /*SR_DIPL_DETAIL*/
    {0xF8cc0d90,    0,       11,        3,        0,     0x100,       0},         //disc_detail_gain
    /*SR_SHARPEN_EN*/
    {0xF8cc0d98,    8,       15,        3,        0,     0x100,      80},         //over_thrsh
    {0xF8cc0d98,    0,        7,        3,        0,     0x100,      80},         //under_thrsh
    /*SR_SMOOTH*/
    {0xF8cc0d9C,    31,      31,        3,        0,     0x100,       1},         //smooth_en
    {0xF8cc0d9C,    30,      30,        3,        0,     0x100,       1},         //plus_en
    {0xF8cc0d9C,    0,        9,        3,        0,     0x100,     140},         //plus_ctrl

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

