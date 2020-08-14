//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_vcpi.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module vcpi
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_VCPI_H__
#define __C_UNION_DEFINE_VCPI_H__

/* Define the union U_VCPI_INTSTAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_int_ve_eop       : 1   ; /* [0]  */
        unsigned int    vcpi_int_ve_buffull   : 1   ; /* [1]  */
        unsigned int    Reserved_5            : 1   ; /* [2]  */
        unsigned int    vcpi_int_ve_pbitsover  : 1   ; /* [3]  */
        unsigned int    Reserved_4            : 6   ; /* [9..4]  */
        unsigned int    vcpi_int_vedu_slice_end  : 1   ; /* [10]  */
        unsigned int    Reserved_3            : 5   ; /* [15..11]  */
        unsigned int    vcpi_int_vedu_brkpt   : 1   ; /* [16]  */
        unsigned int    vcpi_int_vedu_step    : 1   ; /* [17]  */
        unsigned int    Reserved_2            : 9   ; /* [26..18]  */
        unsigned int    vcpi_int_vedu_timeout  : 1   ; /* [27]  */
        unsigned int    Reserved_1            : 2   ; /* [29..28]  */
        unsigned int    vcpi_int_cfg_err      : 1   ; /* [30]  */
        unsigned int    Reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_INTSTAT;

/* Define the union U_VCPI_INTMASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_enable_ve_eop    : 1   ; /* [0]  */
        unsigned int    vcpi_enable_ve_buffull  : 1   ; /* [1]  */
        unsigned int    Reserved_11           : 1   ; /* [2]  */
        unsigned int    vcpi_enable_ve_pbitsover  : 1   ; /* [3]  */
        unsigned int    Reserved_10           : 6   ; /* [9..4]  */
        unsigned int    vcpi_enable_vedu_slice_end  : 1   ; /* [10]  */
        unsigned int    Reserved_9            : 5   ; /* [15..11]  */
        unsigned int    vcpi_enable_vedu_brkpt  : 1   ; /* [16]  */
        unsigned int    vcpi_enable_vedu_step  : 1   ; /* [17]  */
        unsigned int    Reserved_8            : 9   ; /* [26..18]  */
        unsigned int    vcpi_enable_vedu_timeout  : 1   ; /* [27]  */
        unsigned int    Reserved_7            : 2   ; /* [29..28]  */
        unsigned int    vcpi_enable_cfg_err   : 1   ; /* [30]  */
        unsigned int    Reserved_6            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_INTMASK;

/* Define the union U_VCPI_RAWINT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_rint_ve_eop      : 1   ; /* [0]  */
        unsigned int    vcpi_rint_ve_buffull  : 1   ; /* [1]  */
        unsigned int    Reserved_17           : 1   ; /* [2]  */
        unsigned int    vcpi_rint_ve_pbitsover  : 1   ; /* [3]  */
        unsigned int    Reserved_16           : 6   ; /* [9..4]  */
        unsigned int    vcpi_rint_vedu_slice_end  : 1   ; /* [10]  */
        unsigned int    Reserved_15           : 5   ; /* [15..11]  */
        unsigned int    vcpi_rint_vedu_brkpt  : 1   ; /* [16]  */
        unsigned int    vcpi_rint_vedu_step   : 1   ; /* [17]  */
        unsigned int    Reserved_14           : 9   ; /* [26..18]  */
        unsigned int    vcpi_rint_vedu_timeout  : 1   ; /* [27]  */
        unsigned int    Reserved_13           : 2   ; /* [29..28]  */
        unsigned int    vcpi_rint_cfg_err     : 1   ; /* [30]  */
        unsigned int    Reserved_12           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_RAWINT;

/* Define the union U_VCPI_INTCLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_clr_ve_eop       : 1   ; /* [0]  */
        unsigned int    vcpi_clr_ve_buffull   : 1   ; /* [1]  */
        unsigned int    Reserved_23           : 1   ; /* [2]  */
        unsigned int    vcpi_clr_ve_pbitsover  : 1   ; /* [3]  */
        unsigned int    Reserved_22           : 6   ; /* [9..4]  */
        unsigned int    vcpi_clr_vedu_slice_end  : 1   ; /* [10]  */
        unsigned int    Reserved_21           : 5   ; /* [15..11]  */
        unsigned int    vcpi_clr_vedu_brkpt   : 1   ; /* [16]  */
        unsigned int    vcpi_clr_vedu_step    : 1   ; /* [17]  */
        unsigned int    Reserved_20           : 9   ; /* [26..18]  */
        unsigned int    vcpi_clr_vedu_timeout  : 1   ; /* [27]  */
        unsigned int    Reserved_19           : 2   ; /* [29..28]  */
        unsigned int    vcpi_clr_cfg_err      : 1   ; /* [30]  */
        unsigned int    Reserved_18           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_INTCLR;

/* Define the union U_VCPI_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_vstart           : 1   ; /* [0]  */
        unsigned int    llild_ld_end          : 1   ; /* [1]  */
        unsigned int    Reserved_25           : 14  ; /* [15..2]  */
        unsigned int    vcpi_vstep            : 1   ; /* [16]  */
        unsigned int    Reserved_24           : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_START;

/* Define the union U_VCPI_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_vedsel           : 1   ; /* [0]  */
        unsigned int    vcpi_lcu_time_sel     : 1   ; /* [1]  */
        unsigned int    vcpi_cfg_mode         : 1   ; /* [2]  */
        unsigned int    vcpi_protocol         : 2   ; /* [4..3]  */
        unsigned int    vcpi_slice_int_en     : 1   ; /* [5]  */
        unsigned int    vcpi_sao_luma         : 1   ; /* [6]  */
        unsigned int    vcpi_sao_chroma       : 1   ; /* [7]  */
        unsigned int    vcpi_rec_cmp_en       : 1   ; /* [8]  */
        unsigned int    vcpi_img_improve_en   : 1   ; /* [9]  */
        unsigned int    vcpi_frame_type       : 2   ; /* [11..10]  */
        unsigned int    vcpi_entropy_mode     : 1   ; /* [12]  */
        unsigned int    Reserved_28           : 3   ; /* [15..13]  */
        unsigned int    vcpi_idr_pic          : 1   ; /* [16]  */
        unsigned int    vcpi_pskp_en          : 1   ; /* [17]  */
        unsigned int    vcpi_trans_mode       : 2   ; /* [19..18]  */
        unsigned int    Reserved_27           : 1   ; /* [20]  */
        unsigned int    vcpi_sobel_weight_en  : 1   ; /* [21]  */
        unsigned int    vcpi_high_speed_en    : 1   ; /* [22]  */
        unsigned int    Reserved_26           : 1   ; /* [23]  */
        unsigned int    vcpi_ref0_cmp_en      : 1   ; /* [24]  */
        unsigned int    vcpi_tiles_en         : 1   ; /* [25]  */
        unsigned int    vcpi_lcu_size         : 2   ; /* [27..26]  */
        unsigned int    vcpi_time_en          : 2   ; /* [29..28]  */
        unsigned int    vcpi_bkp_en           : 1   ; /* [30]  */
        unsigned int    vcpi_dbgmod           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_MODE;

/* Define the union U_VCPI_TILE_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_tile_width       : 9   ; /* [8..0]  */
        unsigned int    Reserved_30           : 7   ; /* [15..9]  */
        unsigned int    vcpi_tile_height      : 9   ; /* [24..16]  */
        unsigned int    Reserved_29           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_TILE_SIZE;

/* Define the union U_VCPI_SLICE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_slice_size       : 16  ; /* [15..0]  */
        unsigned int    vcpi_slcspilt_mod     : 1   ; /* [16]  */
        unsigned int    vcpi_multislc_en      : 1   ; /* [17]  */
        unsigned int    Reserved_31           : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_SLICE;

/* Define the union U_VCPI_QPCFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_cr_qp_offset     : 5   ; /* [4..0]  */
        unsigned int    vcpi_cb_qp_offset     : 5   ; /* [9..5]  */
        unsigned int    vcpi_frm_qp           : 6   ; /* [15..10]  */
        unsigned int    Reserved_33           : 2   ; /* [17..16]  */
        unsigned int    vcpi_intra_det_qp_en  : 1   ; /* [18]  */
        unsigned int    vcpi_rc_cu_madi_en    : 1   ; /* [19]  */
        unsigned int    Reserved_32           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_QPCFG;

/* Define the union U_VCPI_DBLKCFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_dblk_beta        : 4   ; /* [3..0]  */
        unsigned int    vcpi_dblk_alpha       : 4   ; /* [7..4]  */
        unsigned int    vcpi_dblk_filter_flag  : 2   ; /* [9..8]  */
        unsigned int    Reserved_34           : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_DBLKCFG;

/* Define the union U_VCPI_LOW_POWER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_intra_lowpow     : 1   ; /* [0]  */
        unsigned int    vcpi_fme_lowpow       : 1   ; /* [1]  */
        unsigned int    vcpi_ime_lowpow       : 1   ; /* [2]  */
        unsigned int    vcpi_force_intra_lowpwr_en  : 1   ; /* [3]  */
        unsigned int    Reserved_37           : 12  ; /* [15..4]  */
        unsigned int    vcpi_tqitq_gtck_en    : 1   ; /* [16]  */
        unsigned int    vcpi_mrg_gtck_en      : 1   ; /* [17]  */
        unsigned int    vcpi_fme_gtck_en      : 1   ; /* [18]  */
        unsigned int    vcpi_clkgate_en       : 2   ; /* [20..19]  */
        unsigned int    vcpi_mem_clkgate_en   : 1   ; /* [21]  */
        unsigned int    Reserved_36           : 1   ; /* [22]  */
        unsigned int    vcpi_ddr_cross_en     : 1   ; /* [23]  */
        unsigned int    Reserved_35           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_LOW_POWER;

/* Define the union U_VCPI_PICSIZE_PIX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_imgwidth_pix     : 13  ; /* [12..0]  */
        unsigned int    Reserved_39           : 3   ; /* [15..13]  */
        unsigned int    vcpi_imgheight_pix    : 13  ; /* [28..16]  */
        unsigned int    Reserved_38           : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_PICSIZE_PIX;

/* Define the union U_VCPI_BP_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_bp_lcu_x         : 9   ; /* [8..0]  */
        unsigned int    Reserved_41           : 7   ; /* [15..9]  */
        unsigned int    vcpi_bp_lcu_y         : 9   ; /* [24..16]  */
        unsigned int    Reserved_40           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_BP_POS;

/* Define the union U_VCPI_OUTSTD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_r_outstanding    : 4   ; /* [3..0]  */
        unsigned int    vcpi_w_outstanding    : 4   ; /* [7..4]  */
        unsigned int    Reserved_42           : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_OUTSTD;

/* Define the union U_VCPI_FRAME_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_frame_cnt        : 16  ; /* [15..0]  */
        unsigned int    Reserved_43           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_FRAME_CNT;

/* Define the union U_VCPI_SW_L0_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_sw_l0_width      : 7   ; /* [6..0]  */
        unsigned int    Reserved_45           : 9   ; /* [15..7]  */
        unsigned int    vcpi_sw_l0_height     : 6   ; /* [21..16]  */
        unsigned int    Reserved_44           : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_SW_L0_SIZE;

/* Define the union U_VCPI_MV_LOAD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_tmv_wr_rd_avail  : 2   ; /* [1..0]  */
        unsigned int    Reserved_46           : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_MV_LOAD;

/* Define the union U_VCPI_CROSS_TILE_SLC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_cross_slice      : 1   ; /* [0]  */
        unsigned int    Reserved_48           : 1   ; /* [1]  */
        unsigned int    Reserved_47           : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_CROSS_TILE_SLC;

/* Define the union U_VCPI_MEM_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_ema              : 3   ; /* [2..0]  */
        unsigned int    vcpi_emaw             : 2   ; /* [4..3]  */
        unsigned int    vcpi_emaa             : 3   ; /* [7..5]  */
        unsigned int    vcpi_emab             : 3   ; /* [10..8]  */
        unsigned int    Reserved_49           : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_MEM_CTRL;

/* Define the union U_VCPI_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_curld_c_stride   : 16  ; /* [15..0]  */
        unsigned int    vcpi_curld_y_stride   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_STRIDE;

/* Define the union U_VCPI_REC_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_recst_ystride    : 16  ; /* [15..0]  */
        unsigned int    vcpi_recst_cstride    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_REC_STRIDE;

/* Define the union U_VCPI_REC_HEAD_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_recst_head_stride  : 16  ; /* [15..0]  */
        unsigned int    Reserved_52           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_REC_HEAD_STRIDE;

/* Define the union U_VCPI_REF_L0_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_refc_l0_stride   : 16  ; /* [15..0]  */
        unsigned int    vcpi_refy_l0_stride   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_REF_L0_STRIDE;

/* Define the union U_VCPI_REFH_L0_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_refch_l0_stride  : 16  ; /* [15..0]  */
        unsigned int    vcpi_refyh_l0_stride  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_REFH_L0_STRIDE;

/* Define the union U_VCPI_STRFMT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_crop_en          : 1   ; /* [0]  */
        unsigned int    vcpi_scale_en         : 1   ; /* [1]  */
        unsigned int    vcpi_store_mode       : 1   ; /* [2]  */
        unsigned int    vcpi_blk_type         : 3   ; /* [5..3]  */
        unsigned int    vcpi_str_fmt          : 4   ; /* [9..6]  */
        unsigned int    Reserved_56           : 4   ; /* [13..10]  */
        unsigned int    vcpi_recst_disable    : 1   ; /* [14]  */
        unsigned int    Reserved_55           : 1   ; /* [15]  */
        unsigned int    vcpi_package_sel      : 8   ; /* [23..16]  */
        unsigned int    Reserved_54           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_STRFMT;

/* Define the union U_VCPI_CROP_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_crop_xstart      : 13  ; /* [12..0]  */
        unsigned int    Reserved_58           : 3   ; /* [15..13]  */
        unsigned int    vcpi_crop_ystart      : 13  ; /* [28..16]  */
        unsigned int    Reserved_57           : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_CROP_START;

/* Define the union U_VCPI_CROP_END */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_crop_xend        : 13  ; /* [12..0]  */
        unsigned int    Reserved_60           : 3   ; /* [15..13]  */
        unsigned int    vcpi_crop_yend        : 13  ; /* [28..16]  */
        unsigned int    Reserved_59           : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_CROP_END;

/* Define the union U_VCPI_SCALE_PARA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_xscale           : 16  ; /* [15..0]  */
        unsigned int    vcpi_yscale           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_SCALE_PARA;

/* Define the union U_VCPI_INTRA_INTER_CU_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_intra_cu_en      : 3   ; /* [2..0]  */
        unsigned int    Reserved_66           : 1   ; /* [3]  */
        unsigned int    vcpi_ipcm_en          : 1   ; /* [4]  */
        unsigned int    vcpi_intra_h264_cutdiag  : 1   ; /* [5]  */
        unsigned int    Reserved_65           : 2   ; /* [7..6]  */
        unsigned int    vcpi_fme_cu_en        : 3   ; /* [10..8]  */
        unsigned int    Reserved_64           : 1   ; /* [11]  */
        unsigned int    vcpi_mrg_cu_en        : 3   ; /* [14..12]  */
        unsigned int    Reserved_63           : 1   ; /* [15]  */
        unsigned int    Reserved_62           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_INTRA_INTER_CU_EN;

/* Define the union U_VCPI_VLC_CONFIG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_ref_idc          : 2   ; /* [1..0]  */
        unsigned int    Reserved_69           : 2   ; /* [3..2]  */
        unsigned int    vcpi_cabac_init_idc   : 2   ; /* [5..4]  */
        unsigned int    Reserved_68           : 6   ; /* [11..6]  */
        unsigned int    vcpi_byte_stuffing    : 1   ; /* [12]  */
        unsigned int    Reserved_67           : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_VLC_CONFIG;

/* Define the union U_VCPI_PRE_JUDGE_EXT_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_iblk_pre_en      : 1   ; /* [0]  */
        unsigned int    vcpi_pblk_pre_en      : 1   ; /* [1]  */
        unsigned int    vcpi_force_inter      : 1   ; /* [2]  */
        unsigned int    vcpi_pintra_inter_flag_disable  : 1   ; /* [3]  */
        unsigned int    vcpi_ext_edge_en      : 1   ; /* [4]  */
        unsigned int    Reserved_70           : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_PRE_JUDGE_EXT_EN;

/* Define the union U_VCPI_PRE_JUDGE_COST_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_iblk_pre_cost_thr  : 16  ; /* [15..0]  */
        unsigned int    vcpi_pblk_pre_cost_thr  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_PRE_JUDGE_COST_THR;

/* Define the union U_VCPI_IBLK_PRE_MV_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_iblk_pre_mv_dif_thr0  : 8   ; /* [7..0]  */
        unsigned int    vcpi_iblk_pre_mv_dif_thr1  : 8   ; /* [15..8]  */
        unsigned int    vcpi_iblk_pre_mvx_thr  : 8   ; /* [23..16]  */
        unsigned int    vcpi_iblk_pre_mvy_thr  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_IBLK_PRE_MV_THR;

/* Define the union U_VCPI_PIC_STRONG_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_skin_en          : 1   ; /* [0]  */
        unsigned int    vcpi_highedge_en      : 1   ; /* [1]  */
        unsigned int    vcpi_static_en        : 1   ; /* [2]  */
        unsigned int    vcpi_skin_close_angle  : 1   ; /* [3]  */
        unsigned int    vcpi_rounding_sobel_en  : 1   ; /* [4]  */
        unsigned int    Reserved_72           : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_PIC_STRONG_EN;

/* Define the union U_VCPI_RC_ENABLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_rc_cu_qp_en      : 1   ; /* [0]  */
        unsigned int    vcpi_rc_row_qp_en     : 1   ; /* [1]  */
        unsigned int    vcpi_move_scene_en    : 1   ; /* [2]  */
        unsigned int    Reserved_78           : 1   ; /* [3]  */
        unsigned int    vcpi_strong_edge_move_en  : 1   ; /* [4]  */
        unsigned int    Reserved_77           : 3   ; /* [7..5]  */
        unsigned int    vcpi_rc_low_luma_en   : 1   ; /* [8]  */
        unsigned int    vcpi_rc_h264_smooth_mb_en  : 1   ; /* [9]  */
        unsigned int    vcpi_rd_smooth_mb_en  : 1   ; /* [10]  */
        unsigned int    vcpi_rd_min_sad_flag_en  : 1   ; /* [11]  */
        unsigned int    vcpi_wr_min_sad_flag_en  : 1   ; /* [12]  */
        unsigned int    vcpi_high_min_sad_en  : 1   ; /* [13]  */
        unsigned int    vcpi_low_min_sad_en   : 1   ; /* [14]  */
        unsigned int    vcpi_prev_min_sad_en  : 1   ; /* [15]  */
        unsigned int    Reserved_76           : 2   ; /* [17..16]  */
        unsigned int    Reserved_75           : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_RC_ENABLE;

/* Define the union U_VCPI_PME_PARAM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_80           : 1   ; /* [0]  */
        unsigned int    vcpi_move_sad_en      : 1   ; /* [1]  */
        unsigned int    Reserved_79           : 14  ; /* [15..2]  */
        unsigned int    vcpi_pblk_pre_mvx_thr  : 8   ; /* [23..16]  */
        unsigned int    vcpi_pblk_pre_mvy_thr  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_PME_PARAM;

/* Define the union U_VCPI_PINTRA_THRESH0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_pintra_pu16_amp_th  : 8   ; /* [7..0]  */
        unsigned int    vcpi_pintra_pu32_amp_th  : 8   ; /* [15..8]  */
        unsigned int    vcpi_pintra_pu64_amp_th  : 8   ; /* [23..16]  */
        unsigned int    Reserved_82           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_PINTRA_THRESH0;

/* Define the union U_VCPI_PINTRA_THRESH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_pintra_pu16_std_th  : 8   ; /* [7..0]  */
        unsigned int    vcpi_pintra_pu32_std_th  : 8   ; /* [15..8]  */
        unsigned int    Reserved_83           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_PINTRA_THRESH1;

/* Define the union U_VCPI_PINTRA_THRESH2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_pintra_pu16_angel_cost_th  : 10  ; /* [9..0]  */
        unsigned int    vcpi_pintra_pu32_angel_cost_th  : 10  ; /* [19..10]  */
        unsigned int    Reserved_84           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_PINTRA_THRESH2;

/* Define the union U_VCPI_I_SLC_INSERT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_insert_i_slc_en  : 1   ; /* [0]  */
        unsigned int    vcpi_insert_i_slc_idx  : 8   ; /* [8..1]  */
        unsigned int    Reserved_85           : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_I_SLC_INSERT;

/* Define the union U_VCPI_CLKDIV_ENABLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_clkdiv_en        : 1   ; /* [0]  */
        unsigned int    Reserved_87           : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_CLKDIV_ENABLE;

/* Define the union U_VCPI_RCHN_BYPASS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_nbild_bypass     : 1   ; /* [0]  */
        unsigned int    vcpi_pmeld_bypass     : 1   ; /* [1]  */
        unsigned int    vcpi_refyh_bypass     : 1   ; /* [2]  */
        unsigned int    vcpi_refch_bypass     : 1   ; /* [3]  */
        unsigned int    vcpi_refy_bypass      : 1   ; /* [4]  */
        unsigned int    vcpi_refc_bypass      : 1   ; /* [5]  */
        unsigned int    vcpi_pmeinfold0_bypass  : 1   ; /* [6]  */
        unsigned int    vcpi_pmeinfold1_bypass  : 1   ; /* [7]  */
        unsigned int    vcpi_srcy_bypass      : 1   ; /* [8]  */
        unsigned int    vcpi_srcu_bypass      : 1   ; /* [9]  */
        unsigned int    vcpi_srcv_bypass      : 1   ; /* [10]  */
        unsigned int    Reserved_89           : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_RCHN_BYPASS;

/* Define the union U_VCPI_WCHN_BYPASS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_nbist_bypass     : 1   ; /* [0]  */
        unsigned int    vcpi_pmest_bypass     : 1   ; /* [1]  */
        unsigned int    vcpi_recyh_bypass     : 1   ; /* [2]  */
        unsigned int    vcpi_recch_bypass     : 1   ; /* [3]  */
        unsigned int    vcpi_recy_bypass      : 1   ; /* [4]  */
        unsigned int    vcpi_recc_bypass      : 1   ; /* [5]  */
        unsigned int    vcpi_pmeinfost_bypass  : 1   ; /* [6]  */
        unsigned int    vcpi_strm_bypass      : 1   ; /* [7]  */
        unsigned int    Reserved_90           : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCPI_WCHN_BYPASS;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_VCPI_INTSTAT         VCPI_INTSTAT;
    volatile U_VCPI_INTMASK         VCPI_INTMASK;
    volatile U_VCPI_RAWINT          VCPI_RAWINT;
    volatile U_VCPI_INTCLR          VCPI_INTCLR;
    volatile U_VCPI_START           VCPI_START;
    volatile U_VCPI_MODE            VCPI_MODE;
    volatile U_VCPI_TILE_SIZE       VCPI_TILE_SIZE;
    volatile U_VCPI_SLICE           VCPI_SLICE;
    volatile U_VCPI_QPCFG           VCPI_QPCFG;
    volatile unsigned int           VCPI_CNTCLR;
    volatile unsigned int           VCPI_FRAMENO;
    volatile U_VCPI_DBLKCFG         VCPI_DBLKCFG;
    volatile U_VCPI_LOW_POWER       VCPI_LOW_POWER;
    volatile U_VCPI_PICSIZE_PIX     VCPI_PICSIZE_PIX;
    volatile U_VCPI_BP_POS          VCPI_BP_POS;
    volatile unsigned int           VCPI_TIMEOUT;
    volatile U_VCPI_OUTSTD          VCPI_OUTSTD;
    volatile unsigned int           VCPI_TIMER;
    volatile unsigned int           VCPI_IDLE_TIMER;
    volatile unsigned int           VCPI_VERSION0;
    volatile unsigned int           VCPI_VERSION1;
    volatile U_VCPI_FRAME_CNT       VCPI_FRAME_CNT;
    volatile U_VCPI_SW_L0_SIZE      VCPI_SW_L0_SIZE;
    volatile U_VCPI_MV_LOAD         VCPI_MV_LOAD;
    volatile U_VCPI_CROSS_TILE_SLC   VCPI_CROSS_TILE_SLC;
    volatile U_VCPI_MEM_CTRL        VCPI_MEM_CTRL;
    volatile unsigned int           RESERVED0[20];
    volatile unsigned int           VCPI_TUNLCELL_ADDR;
    volatile unsigned int           VCPI_SRC_YADDR;
    volatile unsigned int           VCPI_SRC_CADDR;
    volatile unsigned int           VCPI_SRC_VADDR;
    volatile unsigned int           RESERVED1[2];
    volatile U_VCPI_STRIDE          VCPI_STRIDE;
    volatile unsigned int           VCPI_PMEINFO_ST_ADDR;
    volatile unsigned int           VCPI_REC_YADDR;
    volatile unsigned int           VCPI_REC_CADDR;
    volatile U_VCPI_REC_STRIDE      VCPI_REC_STRIDE;
    volatile unsigned int           VCPI_REC_YH_ADDR;
    volatile unsigned int           VCPI_REC_CH_ADDR;
    volatile U_VCPI_REC_HEAD_STRIDE   VCPI_REC_HEAD_STRIDE;
    volatile unsigned int           VCPI_REFY_L0_ADDR;
    volatile unsigned int           VCPI_REFC_L0_ADDR;
    volatile U_VCPI_REF_L0_STRIDE   VCPI_REF_L0_STRIDE;
    volatile unsigned int           VCPI_REFYH_L0_ADDR;
    volatile unsigned int           VCPI_REFCH_L0_ADDR;
    volatile U_VCPI_REFH_L0_STRIDE   VCPI_REFH_L0_STRIDE;
    volatile unsigned int           VCPI_PMELD_L0_ADDR;
    volatile unsigned int           VCPI_PMEST_ADDR;
    volatile unsigned int           RESERVED2;
    volatile unsigned int           VCPI_NBI_MVST_ADDR;
    volatile unsigned int           VCPI_NBI_MVLD_ADDR;
    volatile unsigned int           VCPI_STRMADDR;
    volatile unsigned int           VCPI_SWPTRADDR;
    volatile unsigned int           VCPI_SRPTRADDR;
    volatile U_VCPI_STRFMT          VCPI_STRFMT;
    volatile U_VCPI_CROP_START      VCPI_CROP_START;
    volatile U_VCPI_CROP_END        VCPI_CROP_END;
    volatile U_VCPI_SCALE_PARA      VCPI_SCALE_PARA;
    volatile unsigned int           RESERVED3;
    volatile U_VCPI_INTRA_INTER_CU_EN   VCPI_INTRA_INTER_CU_EN;
    volatile U_VCPI_VLC_CONFIG      VCPI_VLC_CONFIG;
    volatile U_VCPI_PRE_JUDGE_EXT_EN   VCPI_PRE_JUDGE_EXT_EN;
    volatile U_VCPI_PRE_JUDGE_COST_THR   VCPI_PRE_JUDGE_COST_THR;
    volatile U_VCPI_IBLK_PRE_MV_THR   VCPI_IBLK_PRE_MV_THR;
    volatile unsigned int           RESERVED4[4];
    volatile U_VCPI_PIC_STRONG_EN   VCPI_PIC_STRONG_EN;
    volatile unsigned int           RESERVED5[9];
    volatile unsigned int           VCPI_LLILD_ADDR;
    volatile unsigned int           RESERVED6[11];
    volatile U_VCPI_RC_ENABLE       VCPI_RC_ENABLE;
    volatile U_VCPI_PME_PARAM       VCPI_PME_PARAM;
    volatile unsigned int           RESERVED7[4];
    volatile U_VCPI_PINTRA_THRESH0   VCPI_PINTRA_THRESH0;
    volatile U_VCPI_PINTRA_THRESH1   VCPI_PINTRA_THRESH1;
    volatile U_VCPI_PINTRA_THRESH2   VCPI_PINTRA_THRESH2;
    volatile U_VCPI_I_SLC_INSERT    VCPI_I_SLC_INSERT;
    volatile unsigned int           VCPI_PMEINFO_LD0_ADDR;
    volatile unsigned int           VCPI_PMEINFO_LD1_ADDR;
    volatile unsigned int           VCPI_HEAD_STRIDE;
    volatile U_VCPI_CLKDIV_ENABLE   VCPI_CLKDIV_ENABLE;
    volatile unsigned int           RESERVED8[2];
    volatile U_VCPI_RCHN_BYPASS     VCPI_RCHN_BYPASS;
    volatile U_VCPI_WCHN_BYPASS     VCPI_WCHN_BYPASS;


} S_vcpi_REGS_TYPE;

/* Declare the struct pointor of the module vcpi */
extern volatile S_vcpi_REGS_TYPE *gopvcpiAllReg;

/* Declare the functions that set the member value */
int iSetVCPI_INTMASKvcpi_enable_ve_eop(unsigned int uvcpi_enable_ve_eop);
int iSetVCPI_INTMASKvcpi_enable_ve_buffull(unsigned int uvcpi_enable_ve_buffull);
int iSetVCPI_INTMASKvcpi_enable_ve_pbitsover(unsigned int uvcpi_enable_ve_pbitsover);
int iSetVCPI_INTMASKvcpi_enable_vedu_slice_end(unsigned int uvcpi_enable_vedu_slice_end);
int iSetVCPI_INTMASKvcpi_enable_vedu_brkpt(unsigned int uvcpi_enable_vedu_brkpt);
int iSetVCPI_INTMASKvcpi_enable_vedu_step(unsigned int uvcpi_enable_vedu_step);
int iSetVCPI_INTMASKvcpi_enable_vedu_timeout(unsigned int uvcpi_enable_vedu_timeout);
int iSetVCPI_INTMASKvcpi_enable_cfg_err(unsigned int uvcpi_enable_cfg_err);
int iSetVCPI_INTCLRvcpi_clr_ve_eop(unsigned int uvcpi_clr_ve_eop);
int iSetVCPI_INTCLRvcpi_clr_ve_buffull(unsigned int uvcpi_clr_ve_buffull);
int iSetVCPI_INTCLRvcpi_clr_ve_pbitsover(unsigned int uvcpi_clr_ve_pbitsover);
int iSetVCPI_INTCLRvcpi_clr_vedu_slice_end(unsigned int uvcpi_clr_vedu_slice_end);
int iSetVCPI_INTCLRvcpi_clr_vedu_brkpt(unsigned int uvcpi_clr_vedu_brkpt);
int iSetVCPI_INTCLRvcpi_clr_vedu_step(unsigned int uvcpi_clr_vedu_step);
int iSetVCPI_INTCLRvcpi_clr_vedu_timeout(unsigned int uvcpi_clr_vedu_timeout);
int iSetVCPI_INTCLRvcpi_clr_cfg_err(unsigned int uvcpi_clr_cfg_err);
int iSetVCPI_STARTvcpi_vstart(unsigned int uvcpi_vstart);
int iSetVCPI_STARTvcpi_vstep(unsigned int uvcpi_vstep);
int iSetVCPI_MODEvcpi_vedsel(unsigned int uvcpi_vedsel);
int iSetVCPI_MODEvcpi_lcu_time_sel(unsigned int uvcpi_lcu_time_sel);
int iSetVCPI_MODEvcpi_cfg_mode(unsigned int uvcpi_cfg_mode);
int iSetVCPI_MODEvcpi_protocol(unsigned int uvcpi_protocol);
int iSetVCPI_MODEvcpi_slice_int_en(unsigned int uvcpi_slice_int_en);
int iSetVCPI_MODEvcpi_sao_luma(unsigned int uvcpi_sao_luma);
int iSetVCPI_MODEvcpi_sao_chroma(unsigned int uvcpi_sao_chroma);
int iSetVCPI_MODEvcpi_rec_cmp_en(unsigned int uvcpi_rec_cmp_en);
int iSetVCPI_MODEvcpi_img_improve_en(unsigned int uvcpi_img_improve_en);
int iSetVCPI_MODEvcpi_frame_type(unsigned int uvcpi_frame_type);
int iSetVCPI_MODEvcpi_entropy_mode(unsigned int uvcpi_entropy_mode);
int iSetVCPI_MODEvcpi_idr_pic(unsigned int uvcpi_idr_pic);
int iSetVCPI_MODEvcpi_pskp_en(unsigned int uvcpi_pskp_en);
int iSetVCPI_MODEvcpi_trans_mode(unsigned int uvcpi_trans_mode);
int iSetVCPI_MODEvcpi_sobel_weight_en(unsigned int uvcpi_sobel_weight_en);
int iSetVCPI_MODEvcpi_high_speed_en(unsigned int uvcpi_high_speed_en);
int iSetVCPI_MODEvcpi_ref0_cmp_en(unsigned int uvcpi_ref0_cmp_en);
int iSetVCPI_MODEvcpi_tiles_en(unsigned int uvcpi_tiles_en);
int iSetVCPI_MODEvcpi_lcu_size(unsigned int uvcpi_lcu_size);
int iSetVCPI_MODEvcpi_time_en(unsigned int uvcpi_time_en);
int iSetVCPI_MODEvcpi_bkp_en(unsigned int uvcpi_bkp_en);
int iSetVCPI_MODEvcpi_dbgmod(unsigned int uvcpi_dbgmod);
int iSetVCPI_TILE_SIZEvcpi_tile_width(unsigned int uvcpi_tile_width);
int iSetVCPI_TILE_SIZEvcpi_tile_height(unsigned int uvcpi_tile_height);
int iSetVCPI_SLICEvcpi_slice_size(unsigned int uvcpi_slice_size);
int iSetVCPI_SLICEvcpi_slcspilt_mod(unsigned int uvcpi_slcspilt_mod);
int iSetVCPI_SLICEvcpi_multislc_en(unsigned int uvcpi_multislc_en);
int iSetVCPI_QPCFGvcpi_cr_qp_offset(unsigned int uvcpi_cr_qp_offset);
int iSetVCPI_QPCFGvcpi_cb_qp_offset(unsigned int uvcpi_cb_qp_offset);
int iSetVCPI_QPCFGvcpi_frm_qp(unsigned int uvcpi_frm_qp);
int iSetVCPI_QPCFGvcpi_intra_det_qp_en(unsigned int uvcpi_intra_det_qp_en);
int iSetVCPI_QPCFGvcpi_rc_cu_madi_en(unsigned int uvcpi_rc_cu_madi_en);
int iSetVCPI_CNTCLRvcpi_cnt_clr(unsigned int uvcpi_cnt_clr);
int iSetVCPI_FRAMENOvcpi_frame_no(unsigned int uvcpi_frame_no);
int iSetVCPI_DBLKCFGvcpi_dblk_beta(unsigned int uvcpi_dblk_beta);
int iSetVCPI_DBLKCFGvcpi_dblk_alpha(unsigned int uvcpi_dblk_alpha);
int iSetVCPI_DBLKCFGvcpi_dblk_filter_flag(unsigned int uvcpi_dblk_filter_flag);
int iSetVCPI_LOW_POWERvcpi_intra_lowpow(unsigned int uvcpi_intra_lowpow);
int iSetVCPI_LOW_POWERvcpi_fme_lowpow(unsigned int uvcpi_fme_lowpow);
int iSetVCPI_LOW_POWERvcpi_ime_lowpow(unsigned int uvcpi_ime_lowpow);
int iSetVCPI_LOW_POWERvcpi_force_intra_lowpwr_en(unsigned int uvcpi_force_intra_lowpwr_en);
int iSetVCPI_LOW_POWERvcpi_tqitq_gtck_en(unsigned int uvcpi_tqitq_gtck_en);
int iSetVCPI_LOW_POWERvcpi_mrg_gtck_en(unsigned int uvcpi_mrg_gtck_en);
int iSetVCPI_LOW_POWERvcpi_fme_gtck_en(unsigned int uvcpi_fme_gtck_en);
int iSetVCPI_LOW_POWERvcpi_clkgate_en(unsigned int uvcpi_clkgate_en);
int iSetVCPI_LOW_POWERvcpi_mem_clkgate_en(unsigned int uvcpi_mem_clkgate_en);
int iSetVCPI_LOW_POWERvcpi_ddr_cross_en(unsigned int uvcpi_ddr_cross_en);
int iSetVCPI_PICSIZE_PIXvcpi_imgwidth_pix(unsigned int uvcpi_imgwidth_pix);
int iSetVCPI_PICSIZE_PIXvcpi_imgheight_pix(unsigned int uvcpi_imgheight_pix);
int iSetVCPI_BP_POSvcpi_bp_lcu_x(unsigned int uvcpi_bp_lcu_x);
int iSetVCPI_BP_POSvcpi_bp_lcu_y(unsigned int uvcpi_bp_lcu_y);
int iSetVCPI_TIMEOUTvcpi_timeout(unsigned int uvcpi_timeout);
int iSetVCPI_OUTSTDvcpi_r_outstanding(unsigned int uvcpi_r_outstanding);
int iSetVCPI_OUTSTDvcpi_w_outstanding(unsigned int uvcpi_w_outstanding);
int iSetVCPI_SW_L0_SIZEvcpi_sw_l0_width(unsigned int uvcpi_sw_l0_width);
int iSetVCPI_SW_L0_SIZEvcpi_sw_l0_height(unsigned int uvcpi_sw_l0_height);
int iSetVCPI_MV_LOADvcpi_tmv_wr_rd_avail(unsigned int uvcpi_tmv_wr_rd_avail);
int iSetVCPI_CROSS_TILE_SLCvcpi_cross_slice(unsigned int uvcpi_cross_slice);
int iSetVCPI_MEM_CTRLvcpi_ema(unsigned int uvcpi_ema);
int iSetVCPI_MEM_CTRLvcpi_emaw(unsigned int uvcpi_emaw);
int iSetVCPI_MEM_CTRLvcpi_emaa(unsigned int uvcpi_emaa);
int iSetVCPI_MEM_CTRLvcpi_emab(unsigned int uvcpi_emab);
int iSetVCPI_TUNLCELL_ADDRvcpi_curld_tunlcell_addr(unsigned int uvcpi_curld_tunlcell_addr);
int iSetVCPI_SRC_YADDRvcpi_curld_y_addr(unsigned int uvcpi_curld_y_addr);
int iSetVCPI_SRC_CADDRvcpi_curld_c_addr(unsigned int uvcpi_curld_c_addr);
int iSetVCPI_SRC_VADDRvcpi_curld_v_addr(unsigned int uvcpi_curld_v_addr);
int iSetVCPI_STRIDEvcpi_curld_c_stride(unsigned int uvcpi_curld_c_stride);
int iSetVCPI_STRIDEvcpi_curld_y_stride(unsigned int uvcpi_curld_y_stride);
int iSetVCPI_PMEINFO_ST_ADDRvcpi_pmest_info_addr(unsigned int uvcpi_pmest_info_addr);
int iSetVCPI_REC_YADDRvcpi_recst_yaddr(unsigned int uvcpi_recst_yaddr);
int iSetVCPI_REC_CADDRvcpi_recst_caddr(unsigned int uvcpi_recst_caddr);
int iSetVCPI_REC_STRIDEvcpi_recst_ystride(unsigned int uvcpi_recst_ystride);
int iSetVCPI_REC_STRIDEvcpi_recst_cstride(unsigned int uvcpi_recst_cstride);
int iSetVCPI_REC_YH_ADDRvcpi_recst_yh_addr(unsigned int uvcpi_recst_yh_addr);
int iSetVCPI_REC_CH_ADDRvcpi_recst_ch_addr(unsigned int uvcpi_recst_ch_addr);
int iSetVCPI_REC_HEAD_STRIDEvcpi_recst_head_stride(unsigned int uvcpi_recst_head_stride);
int iSetVCPI_REFY_L0_ADDRvcpi_refy_l0_addr(unsigned int uvcpi_refy_l0_addr);
int iSetVCPI_REFC_L0_ADDRvcpi_refc_l0_addr(unsigned int uvcpi_refc_l0_addr);
int iSetVCPI_REF_L0_STRIDEvcpi_refc_l0_stride(unsigned int uvcpi_refc_l0_stride);
int iSetVCPI_REF_L0_STRIDEvcpi_refy_l0_stride(unsigned int uvcpi_refy_l0_stride);
int iSetVCPI_REFYH_L0_ADDRvcpi_refyh_l0_addr(unsigned int uvcpi_refyh_l0_addr);
int iSetVCPI_REFCH_L0_ADDRvcpi_refch_l0_addr(unsigned int uvcpi_refch_l0_addr);
int iSetVCPI_REFH_L0_STRIDEvcpi_refch_l0_stride(unsigned int uvcpi_refch_l0_stride);
int iSetVCPI_REFH_L0_STRIDEvcpi_refyh_l0_stride(unsigned int uvcpi_refyh_l0_stride);
int iSetVCPI_PMELD_L0_ADDRvcpi_pmeld_l0_addr(unsigned int uvcpi_pmeld_l0_addr);
int iSetVCPI_PMEST_ADDRvcpi_pmest_addr(unsigned int uvcpi_pmest_addr);
int iSetVCPI_NBI_MVST_ADDRvcpi_mvst_address(unsigned int uvcpi_mvst_address);
int iSetVCPI_NBI_MVLD_ADDRvcpi_mvld_address(unsigned int uvcpi_mvld_address);
int iSetVCPI_STRMADDRvcpi_csst_strmaddr(unsigned int uvcpi_csst_strmaddr);
int iSetVCPI_SWPTRADDRvcpi_csst_swptraddr(unsigned int uvcpi_csst_swptraddr);
int iSetVCPI_SRPTRADDRvcpi_csst_srptraddr(unsigned int uvcpi_csst_srptraddr);
int iSetVCPI_STRFMTvcpi_crop_en(unsigned int uvcpi_crop_en);
int iSetVCPI_STRFMTvcpi_scale_en(unsigned int uvcpi_scale_en);
int iSetVCPI_STRFMTvcpi_store_mode(unsigned int uvcpi_store_mode);
int iSetVCPI_STRFMTvcpi_blk_type(unsigned int uvcpi_blk_type);
int iSetVCPI_STRFMTvcpi_str_fmt(unsigned int uvcpi_str_fmt);
int iSetVCPI_STRFMTvcpi_recst_disable(unsigned int uvcpi_recst_disable);
int iSetVCPI_STRFMTvcpi_package_sel(unsigned int uvcpi_package_sel);
int iSetVCPI_CROP_STARTvcpi_crop_xstart(unsigned int uvcpi_crop_xstart);
int iSetVCPI_CROP_STARTvcpi_crop_ystart(unsigned int uvcpi_crop_ystart);
int iSetVCPI_CROP_ENDvcpi_crop_xend(unsigned int uvcpi_crop_xend);
int iSetVCPI_CROP_ENDvcpi_crop_yend(unsigned int uvcpi_crop_yend);
int iSetVCPI_SCALE_PARAvcpi_xscale(unsigned int uvcpi_xscale);
int iSetVCPI_SCALE_PARAvcpi_yscale(unsigned int uvcpi_yscale);
int iSetVCPI_INTRA_INTER_CU_ENvcpi_intra_cu_en(unsigned int uvcpi_intra_cu_en);
int iSetVCPI_INTRA_INTER_CU_ENvcpi_ipcm_en(unsigned int uvcpi_ipcm_en);
int iSetVCPI_INTRA_INTER_CU_ENvcpi_intra_h264_cutdiag(unsigned int uvcpi_intra_h264_cutdiag);
int iSetVCPI_INTRA_INTER_CU_ENvcpi_fme_cu_en(unsigned int uvcpi_fme_cu_en);
int iSetVCPI_INTRA_INTER_CU_ENvcpi_mrg_cu_en(unsigned int uvcpi_mrg_cu_en);
int iSetVCPI_VLC_CONFIGvcpi_ref_idc(unsigned int uvcpi_ref_idc);
int iSetVCPI_VLC_CONFIGvcpi_cabac_init_idc(unsigned int uvcpi_cabac_init_idc);
int iSetVCPI_VLC_CONFIGvcpi_byte_stuffing(unsigned int uvcpi_byte_stuffing);
int iSetVCPI_PRE_JUDGE_EXT_ENvcpi_iblk_pre_en(unsigned int uvcpi_iblk_pre_en);
int iSetVCPI_PRE_JUDGE_EXT_ENvcpi_pblk_pre_en(unsigned int uvcpi_pblk_pre_en);
int iSetVCPI_PRE_JUDGE_EXT_ENvcpi_force_inter(unsigned int uvcpi_force_inter);
int iSetVCPI_PRE_JUDGE_EXT_ENvcpi_pintra_inter_flag_disable(unsigned int uvcpi_pintra_inter_flag_disable);
int iSetVCPI_PRE_JUDGE_EXT_ENvcpi_ext_edge_en(unsigned int uvcpi_ext_edge_en);
int iSetVCPI_PRE_JUDGE_COST_THRvcpi_iblk_pre_cost_thr(unsigned int uvcpi_iblk_pre_cost_thr);
int iSetVCPI_PRE_JUDGE_COST_THRvcpi_pblk_pre_cost_thr(unsigned int uvcpi_pblk_pre_cost_thr);
int iSetVCPI_IBLK_PRE_MV_THRvcpi_iblk_pre_mv_dif_thr0(unsigned int uvcpi_iblk_pre_mv_dif_thr0);
int iSetVCPI_IBLK_PRE_MV_THRvcpi_iblk_pre_mv_dif_thr1(unsigned int uvcpi_iblk_pre_mv_dif_thr1);
int iSetVCPI_IBLK_PRE_MV_THRvcpi_iblk_pre_mvx_thr(unsigned int uvcpi_iblk_pre_mvx_thr);
int iSetVCPI_IBLK_PRE_MV_THRvcpi_iblk_pre_mvy_thr(unsigned int uvcpi_iblk_pre_mvy_thr);
int iSetVCPI_PIC_STRONG_ENvcpi_skin_en(unsigned int uvcpi_skin_en);
int iSetVCPI_PIC_STRONG_ENvcpi_highedge_en(unsigned int uvcpi_highedge_en);
int iSetVCPI_PIC_STRONG_ENvcpi_static_en(unsigned int uvcpi_static_en);
int iSetVCPI_PIC_STRONG_ENvcpi_skin_close_angle(unsigned int uvcpi_skin_close_angle);
int iSetVCPI_PIC_STRONG_ENvcpi_rounding_sobel_en(unsigned int uvcpi_rounding_sobel_en);
int iSetVCPI_LLILD_ADDRvcpi_llild_addr(unsigned int uvcpi_llild_addr);
int iSetVCPI_RC_ENABLEvcpi_rc_cu_qp_en(unsigned int uvcpi_rc_cu_qp_en);
int iSetVCPI_RC_ENABLEvcpi_rc_row_qp_en(unsigned int uvcpi_rc_row_qp_en);
int iSetVCPI_RC_ENABLEvcpi_move_scene_en(unsigned int uvcpi_move_scene_en);
int iSetVCPI_RC_ENABLEvcpi_strong_edge_move_en(unsigned int uvcpi_strong_edge_move_en);
int iSetVCPI_RC_ENABLEvcpi_rc_low_luma_en(unsigned int uvcpi_rc_low_luma_en);
int iSetVCPI_RC_ENABLEvcpi_rc_h264_smooth_mb_en(unsigned int uvcpi_rc_h264_smooth_mb_en);
int iSetVCPI_RC_ENABLEvcpi_rd_smooth_mb_en(unsigned int uvcpi_rd_smooth_mb_en);
int iSetVCPI_RC_ENABLEvcpi_rd_min_sad_flag_en(unsigned int uvcpi_rd_min_sad_flag_en);
int iSetVCPI_RC_ENABLEvcpi_wr_min_sad_flag_en(unsigned int uvcpi_wr_min_sad_flag_en);
int iSetVCPI_RC_ENABLEvcpi_high_min_sad_en(unsigned int uvcpi_high_min_sad_en);
int iSetVCPI_RC_ENABLEvcpi_low_min_sad_en(unsigned int uvcpi_low_min_sad_en);
int iSetVCPI_RC_ENABLEvcpi_prev_min_sad_en(unsigned int uvcpi_prev_min_sad_en);
int iSetVCPI_PME_PARAMvcpi_move_sad_en(unsigned int uvcpi_move_sad_en);
int iSetVCPI_PME_PARAMvcpi_pblk_pre_mvx_thr(unsigned int uvcpi_pblk_pre_mvx_thr);
int iSetVCPI_PME_PARAMvcpi_pblk_pre_mvy_thr(unsigned int uvcpi_pblk_pre_mvy_thr);
int iSetVCPI_PINTRA_THRESH0vcpi_pintra_pu16_amp_th(unsigned int uvcpi_pintra_pu16_amp_th);
int iSetVCPI_PINTRA_THRESH0vcpi_pintra_pu32_amp_th(unsigned int uvcpi_pintra_pu32_amp_th);
int iSetVCPI_PINTRA_THRESH0vcpi_pintra_pu64_amp_th(unsigned int uvcpi_pintra_pu64_amp_th);
int iSetVCPI_PINTRA_THRESH1vcpi_pintra_pu16_std_th(unsigned int uvcpi_pintra_pu16_std_th);
int iSetVCPI_PINTRA_THRESH1vcpi_pintra_pu32_std_th(unsigned int uvcpi_pintra_pu32_std_th);
int iSetVCPI_PINTRA_THRESH2vcpi_pintra_pu16_angel_cost_th(unsigned int uvcpi_pintra_pu16_angel_cost_th);
int iSetVCPI_PINTRA_THRESH2vcpi_pintra_pu32_angel_cost_th(unsigned int uvcpi_pintra_pu32_angel_cost_th);
int iSetVCPI_I_SLC_INSERTvcpi_insert_i_slc_en(unsigned int uvcpi_insert_i_slc_en);
int iSetVCPI_I_SLC_INSERTvcpi_insert_i_slc_idx(unsigned int uvcpi_insert_i_slc_idx);
int iSetVCPI_PMEINFO_LD0_ADDRvcpi_pmeinfo_ld_0_addr(unsigned int uvcpi_pmeinfo_ld_0_addr);
int iSetVCPI_PMEINFO_LD1_ADDRvcpi_pmeinfo_ld_1_addr(unsigned int uvcpi_pmeinfo_ld_1_addr);
int iSetVCPI_CLKDIV_ENABLEvcpi_clkdiv_en(unsigned int uvcpi_clkdiv_en);
int iSetVCPI_RCHN_BYPASSvcpi_nbild_bypass(unsigned int uvcpi_nbild_bypass);
int iSetVCPI_RCHN_BYPASSvcpi_pmeld_bypass(unsigned int uvcpi_pmeld_bypass);
int iSetVCPI_RCHN_BYPASSvcpi_refyh_bypass(unsigned int uvcpi_refyh_bypass);
int iSetVCPI_RCHN_BYPASSvcpi_refch_bypass(unsigned int uvcpi_refch_bypass);
int iSetVCPI_RCHN_BYPASSvcpi_refy_bypass(unsigned int uvcpi_refy_bypass);
int iSetVCPI_RCHN_BYPASSvcpi_refc_bypass(unsigned int uvcpi_refc_bypass);
int iSetVCPI_RCHN_BYPASSvcpi_pmeinfold0_bypass(unsigned int uvcpi_pmeinfold0_bypass);
int iSetVCPI_RCHN_BYPASSvcpi_pmeinfold1_bypass(unsigned int uvcpi_pmeinfold1_bypass);
int iSetVCPI_RCHN_BYPASSvcpi_srcy_bypass(unsigned int uvcpi_srcy_bypass);
int iSetVCPI_RCHN_BYPASSvcpi_srcu_bypass(unsigned int uvcpi_srcu_bypass);
int iSetVCPI_RCHN_BYPASSvcpi_srcv_bypass(unsigned int uvcpi_srcv_bypass);
int iSetVCPI_WCHN_BYPASSvcpi_nbist_bypass(unsigned int uvcpi_nbist_bypass);
int iSetVCPI_WCHN_BYPASSvcpi_pmest_bypass(unsigned int uvcpi_pmest_bypass);
int iSetVCPI_WCHN_BYPASSvcpi_recyh_bypass(unsigned int uvcpi_recyh_bypass);
int iSetVCPI_WCHN_BYPASSvcpi_recch_bypass(unsigned int uvcpi_recch_bypass);
int iSetVCPI_WCHN_BYPASSvcpi_recy_bypass(unsigned int uvcpi_recy_bypass);
int iSetVCPI_WCHN_BYPASSvcpi_recc_bypass(unsigned int uvcpi_recc_bypass);
int iSetVCPI_WCHN_BYPASSvcpi_pmeinfost_bypass(unsigned int uvcpi_pmeinfost_bypass);
int iSetVCPI_WCHN_BYPASSvcpi_strm_bypass(unsigned int uvcpi_strm_bypass);



#endif /* __C_UNION_DEFINE_VCPI_H__ */
