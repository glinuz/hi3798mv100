//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_vctrl.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module vctrl
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_VCTRL_H__
#define __C_UNION_DEFINE_VCTRL_H__

/* Define the union U_VCTRL_ROI_CFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_region0en       : 1   ; /* [0]  */
        unsigned int    vctrl_region1en       : 1   ; /* [1]  */
        unsigned int    vctrl_region2en       : 1   ; /* [2]  */
        unsigned int    vctrl_region3en       : 1   ; /* [3]  */
        unsigned int    vctrl_region4en       : 1   ; /* [4]  */
        unsigned int    vctrl_region5en       : 1   ; /* [5]  */
        unsigned int    vctrl_region6en       : 1   ; /* [6]  */
        unsigned int    vctrl_region7en       : 1   ; /* [7]  */
        unsigned int    vctrl_absqp0          : 1   ; /* [8]  */
        unsigned int    vctrl_absqp1          : 1   ; /* [9]  */
        unsigned int    vctrl_absqp2          : 1   ; /* [10]  */
        unsigned int    vctrl_absqp3          : 1   ; /* [11]  */
        unsigned int    vctrl_absqp4          : 1   ; /* [12]  */
        unsigned int    vctrl_absqp5          : 1   ; /* [13]  */
        unsigned int    vctrl_absqp6          : 1   ; /* [14]  */
        unsigned int    vctrl_absqp7          : 1   ; /* [15]  */
        unsigned int    vctrl_region0keep     : 1   ; /* [16]  */
        unsigned int    vctrl_region1keep     : 1   ; /* [17]  */
        unsigned int    vctrl_region2keep     : 1   ; /* [18]  */
        unsigned int    vctrl_region3keep     : 1   ; /* [19]  */
        unsigned int    vctrl_region4keep     : 1   ; /* [20]  */
        unsigned int    vctrl_region5keep     : 1   ; /* [21]  */
        unsigned int    vctrl_region6keep     : 1   ; /* [22]  */
        unsigned int    vctrl_region7keep     : 1   ; /* [23]  */
        unsigned int    vctrl_roi_en          : 1   ; /* [24]  */
        unsigned int    Reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_CFG0;

/* Define the union U_VCTRL_ROI_CFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roiqp0          : 7   ; /* [6..0]  */
        unsigned int    Reserved_4            : 1   ; /* [7]  */
        unsigned int    vctrl_roiqp1          : 7   ; /* [14..8]  */
        unsigned int    Reserved_3            : 1   ; /* [15]  */
        unsigned int    vctrl_roiqp2          : 7   ; /* [22..16]  */
        unsigned int    Reserved_2            : 1   ; /* [23]  */
        unsigned int    vctrl_roiqp3          : 7   ; /* [30..24]  */
        unsigned int    Reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_CFG1;

/* Define the union U_VCTRL_ROI_CFG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roiqp4          : 7   ; /* [6..0]  */
        unsigned int    Reserved_8            : 1   ; /* [7]  */
        unsigned int    vctrl_roiqp5          : 7   ; /* [14..8]  */
        unsigned int    Reserved_7            : 1   ; /* [15]  */
        unsigned int    vctrl_roiqp6          : 7   ; /* [22..16]  */
        unsigned int    Reserved_6            : 1   ; /* [23]  */
        unsigned int    vctrl_roiqp7          : 7   ; /* [30..24]  */
        unsigned int    Reserved_5            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_CFG2;

/* Define the union U_VCTRL_ROI_SIZE_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size0_roiwidth  : 10  ; /* [9..0]  */
        unsigned int    Reserved_10           : 6   ; /* [15..10]  */
        unsigned int    vctrl_size0_roiheight  : 10  ; /* [25..16]  */
        unsigned int    Reserved_9            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_SIZE_0;

/* Define the union U_VCTRL_ROI_SIZE_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size1_roiwidth  : 10  ; /* [9..0]  */
        unsigned int    Reserved_12           : 6   ; /* [15..10]  */
        unsigned int    vctrl_size1_roiheight  : 10  ; /* [25..16]  */
        unsigned int    Reserved_11           : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_SIZE_1;

/* Define the union U_VCTRL_ROI_SIZE_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size2_roiwidth  : 10  ; /* [9..0]  */
        unsigned int    Reserved_14           : 6   ; /* [15..10]  */
        unsigned int    vctrl_size2_roiheight  : 10  ; /* [25..16]  */
        unsigned int    Reserved_13           : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_SIZE_2;

/* Define the union U_VCTRL_ROI_SIZE_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size3_roiwidth  : 10  ; /* [9..0]  */
        unsigned int    Reserved_16           : 6   ; /* [15..10]  */
        unsigned int    vctrl_size3_roiheight  : 10  ; /* [25..16]  */
        unsigned int    Reserved_15           : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_SIZE_3;

/* Define the union U_VCTRL_ROI_SIZE_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size4_roiwidth  : 10  ; /* [9..0]  */
        unsigned int    Reserved_18           : 6   ; /* [15..10]  */
        unsigned int    vctrl_size4_roiheight  : 10  ; /* [25..16]  */
        unsigned int    Reserved_17           : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_SIZE_4;

/* Define the union U_VCTRL_ROI_SIZE_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size5_roiwidth  : 10  ; /* [9..0]  */
        unsigned int    Reserved_20           : 6   ; /* [15..10]  */
        unsigned int    vctrl_size5_roiheight  : 10  ; /* [25..16]  */
        unsigned int    Reserved_19           : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_SIZE_5;

/* Define the union U_VCTRL_ROI_SIZE_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size6_roiwidth  : 10  ; /* [9..0]  */
        unsigned int    Reserved_22           : 6   ; /* [15..10]  */
        unsigned int    vctrl_size6_roiheight  : 10  ; /* [25..16]  */
        unsigned int    Reserved_21           : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_SIZE_6;

/* Define the union U_VCTRL_ROI_SIZE_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size7_roiwidth  : 10  ; /* [9..0]  */
        unsigned int    Reserved_24           : 6   ; /* [15..10]  */
        unsigned int    vctrl_size7_roiheight  : 10  ; /* [25..16]  */
        unsigned int    Reserved_23           : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_SIZE_7;

/* Define the union U_VCTRL_ROI_START_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start0_roistartx  : 9   ; /* [8..0]  */
        unsigned int    Reserved_26           : 7   ; /* [15..9]  */
        unsigned int    vctrl_start0_roistarty  : 9   ; /* [24..16]  */
        unsigned int    Reserved_25           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_START_0;

/* Define the union U_VCTRL_ROI_START_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start1_roistartx  : 9   ; /* [8..0]  */
        unsigned int    Reserved_28           : 7   ; /* [15..9]  */
        unsigned int    vctrl_start1_roistarty  : 9   ; /* [24..16]  */
        unsigned int    Reserved_27           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_START_1;

/* Define the union U_VCTRL_ROI_START_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start2_roistartx  : 9   ; /* [8..0]  */
        unsigned int    Reserved_30           : 7   ; /* [15..9]  */
        unsigned int    vctrl_start2_roistarty  : 9   ; /* [24..16]  */
        unsigned int    Reserved_29           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_START_2;

/* Define the union U_VCTRL_ROI_START_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start3_roistartx  : 9   ; /* [8..0]  */
        unsigned int    Reserved_32           : 7   ; /* [15..9]  */
        unsigned int    vctrl_start3_roistarty  : 9   ; /* [24..16]  */
        unsigned int    Reserved_31           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_START_3;

/* Define the union U_VCTRL_ROI_START_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start4_roistartx  : 9   ; /* [8..0]  */
        unsigned int    Reserved_34           : 7   ; /* [15..9]  */
        unsigned int    vctrl_start4_roistarty  : 9   ; /* [24..16]  */
        unsigned int    Reserved_33           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_START_4;

/* Define the union U_VCTRL_ROI_START_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start5_roistartx  : 9   ; /* [8..0]  */
        unsigned int    Reserved_36           : 7   ; /* [15..9]  */
        unsigned int    vctrl_start5_roistarty  : 9   ; /* [24..16]  */
        unsigned int    Reserved_35           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_START_5;

/* Define the union U_VCTRL_ROI_START_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start6_roistartx  : 9   ; /* [8..0]  */
        unsigned int    Reserved_38           : 7   ; /* [15..9]  */
        unsigned int    vctrl_start6_roistarty  : 9   ; /* [24..16]  */
        unsigned int    Reserved_37           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_START_6;

/* Define the union U_VCTRL_ROI_START_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start7_roistartx  : 9   ; /* [8..0]  */
        unsigned int    Reserved_40           : 7   ; /* [15..9]  */
        unsigned int    vctrl_start7_roistarty  : 9   ; /* [24..16]  */
        unsigned int    Reserved_39           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_ROI_START_7;

/* Define the union U_VCTRL_LCU_TARGET_BIT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_target_bit  : 20  ; /* [19..0]  */
        unsigned int    Reserved_41           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_LCU_TARGET_BIT;

/* Define the union U_VCTRL_NARROW_THRESHOLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_narrow_tile_width  : 4   ; /* [3..0]  */
        unsigned int    Reserved_42           : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_NARROW_THRESHOLD;

/* Define the union U_VCTRL_LCU_BASELINE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_performance_baseline  : 16  ; /* [15..0]  */
        unsigned int    Reserved_43           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_LCU_BASELINE;

/* Define the union U_VCTRL_PIPE_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_stage_1_busy    : 1   ; /* [0]  */
        unsigned int    vctrl_stage_2_busy    : 1   ; /* [1]  */
        unsigned int    vctrl_stage_3_busy    : 1   ; /* [2]  */
        unsigned int    vctrl_stage_4_busy    : 1   ; /* [3]  */
        unsigned int    vctrl_stage_5_busy    : 1   ; /* [4]  */
        unsigned int    vctrl_stage_6_busy    : 1   ; /* [5]  */
        unsigned int    vctrl_stage_7_busy    : 1   ; /* [6]  */
        unsigned int    vctrl_stage_8_busy    : 1   ; /* [7]  */
        unsigned int    vctrl_stage_9_busy    : 1   ; /* [8]  */
        unsigned int    vctrl_stage_10_busy   : 1   ; /* [9]  */
        unsigned int    reseve                : 18  ; /* [27..10]  */
        unsigned int    vctrl_nbi_picend      : 1   ; /* [28]  */
        unsigned int    vctrl_pmest_end       : 1   ; /* [29]  */
        unsigned int    vctrl_vclst_end       : 1   ; /* [30]  */
        unsigned int    vctrl_recst_end       : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_PIPE_STATE;

/* Define the union U_VCTRL_MODULE_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_curld_valid     : 1   ; /* [0]  */
        unsigned int    vctrl_pmeld_valid     : 1   ; /* [1]  */
        unsigned int    vctrl_nbi_valid       : 1   ; /* [2]  */
        unsigned int    Reserved_51           : 1   ; /* [3]  */
        unsigned int    vctrl_pme_valid       : 1   ; /* [4]  */
        unsigned int    Reserved_50           : 3   ; /* [7..5]  */
        unsigned int    vctrl_refld_luma_ready  : 1   ; /* [8]  */
        unsigned int    vctrl_qpg_valid       : 1   ; /* [9]  */
        unsigned int    Reserved_49           : 2   ; /* [11..10]  */
        unsigned int    vctrl_ime_valid       : 1   ; /* [12]  */
        unsigned int    vctrl_pintra_valid    : 1   ; /* [13]  */
        unsigned int    vctrl_refld_chma_valid  : 1   ; /* [14]  */
        unsigned int    Reserved_48           : 1   ; /* [15]  */
        unsigned int    vctrl_intra_valid     : 1   ; /* [16]  */
        unsigned int    vctrl_mrg_valid       : 1   ; /* [17]  */
        unsigned int    vctrl_fme_valid       : 1   ; /* [18]  */
        unsigned int    vctrl_pmv_valid       : 1   ; /* [19]  */
        unsigned int    vctrl_sel_valid       : 1   ; /* [20]  */
        unsigned int    Reserved_47           : 3   ; /* [23..21]  */
        unsigned int    vctrl_dblk_valid      : 1   ; /* [24]  */
        unsigned int    vctrl_sao_valid       : 1   ; /* [25]  */
        unsigned int    Reserved_46           : 2   ; /* [27..26]  */
        unsigned int    vctrl_pack_valid      : 1   ; /* [28]  */
        unsigned int    vctrl_cabac_valid     : 1   ; /* [29]  */
        unsigned int    Reserved_45           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_MODULE_STATE;

/* Define the union U_VCTRL_CUR_TILE_INFO_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_tile_x          : 5   ; /* [4..0]  */
        unsigned int    Reserved_53           : 3   ; /* [7..5]  */
        unsigned int    vctrl_tile_y          : 5   ; /* [12..8]  */
        unsigned int    Reserved_52           : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_CUR_TILE_INFO_0;

/* Define the union U_VCTRL_CUR_TILE_INFO_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_tile_base_lcu_x  : 9   ; /* [8..0]  */
        unsigned int    Reserved_55           : 7   ; /* [15..9]  */
        unsigned int    vctrl_tile_base_lcu_y  : 9   ; /* [24..16]  */
        unsigned int    Reserved_54           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_CUR_TILE_INFO_1;

/* Define the union U_VCTRL_CUR_TILE_INFO_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_tile_lcu_x      : 9   ; /* [8..0]  */
        unsigned int    Reserved_57           : 7   ; /* [15..9]  */
        unsigned int    vctrl_tile_lcu_y      : 9   ; /* [24..16]  */
        unsigned int    Reserved_56           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_CUR_TILE_INFO_2;

/* Define the union U_VCTRL_SLICE_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_slice_base_lcu_x  : 9   ; /* [8..0]  */
        unsigned int    Reserved_59           : 7   ; /* [15..9]  */
        unsigned int    vctrl_slice_base_lcu_y  : 9   ; /* [24..16]  */
        unsigned int    Reserved_58           : 3   ; /* [27..25]  */
        unsigned int    slice_number          : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SLICE_INFO;

/* Define the union U_VCTRL_CTB_PIPE_S_COORD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_x           : 9   ; /* [8..0]  */
        unsigned int    Reserved_61           : 7   ; /* [15..9]  */
        unsigned int    vctrl_lcu_y           : 9   ; /* [24..16]  */
        unsigned int    Reserved_60           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_CTB_PIPE_S_COORD;

/* Define the union U_VCTRL_NORM_TR32X32_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_norm32_coeff_protect_num  : 10  ; /* [9..0]  */
        unsigned int    Reserved_64           : 6   ; /* [15..10]  */
        unsigned int    vctrl_norm32_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_63           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_NORM_TR32X32_COEFF_DENOISE;

/* Define the union U_VCTRL_NORM_TR16X16_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_norm16_coeff_protect_num  : 8   ; /* [7..0]  */
        unsigned int    Reserved_66           : 8   ; /* [15..8]  */
        unsigned int    vctrl_norm16_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_65           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_NORM_TR16X16_COEFF_DENOISE;

/* Define the union U_VCTRL_SKIN_TR32X32_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_skin32_coeff_protect_num  : 10  ; /* [9..0]  */
        unsigned int    Reserved_68           : 6   ; /* [15..10]  */
        unsigned int    vctrl_skin32_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_67           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SKIN_TR32X32_COEFF_DENOISE;

/* Define the union U_VCTRL_SKIN_TR16X16_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_skin16_coeff_protect_num  : 8   ; /* [7..0]  */
        unsigned int    Reserved_70           : 8   ; /* [15..8]  */
        unsigned int    vctrl_skin16_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_69           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SKIN_TR16X16_COEFF_DENOISE;

/* Define the union U_VCTRL_STATIC_TR32X32_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_static32_coeff_protect_num  : 10  ; /* [9..0]  */
        unsigned int    Reserved_72           : 6   ; /* [15..10]  */
        unsigned int    vctrl_static32_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_71           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_STATIC_TR32X32_COEFF_DENOISE;

/* Define the union U_VCTRL_STATIC_TR16X16_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_static16_coeff_protect_num  : 8   ; /* [7..0]  */
        unsigned int    Reserved_74           : 8   ; /* [15..8]  */
        unsigned int    vctrl_static16_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_73           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_STATIC_TR16X16_COEFF_DENOISE;

/* Define the union U_VCTRL_HEDGE_TR32X32_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_hedge32_coeff_protect_num  : 10  ; /* [9..0]  */
        unsigned int    Reserved_76           : 6   ; /* [15..10]  */
        unsigned int    vctrl_hedge32_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_75           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_HEDGE_TR32X32_COEFF_DENOISE;

/* Define the union U_VCTRL_HEDGE_TR16X16_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_hedge16_coeff_protect_num  : 8   ; /* [7..0]  */
        unsigned int    Reserved_78           : 8   ; /* [15..8]  */
        unsigned int    vctrl_hedge16_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_77           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_HEDGE_TR16X16_COEFF_DENOISE;

/* Define the union U_VCTRL_NORMAL_INTRA_RDO_FACTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_norm_intra_cu4_rdcost_offset  : 4   ; /* [3..0]  */
        unsigned int    vctrl_norm_intra_cu8_rdcost_offset  : 4   ; /* [7..4]  */
        unsigned int    vctrl_norm_intra_cu16_rdcost_offset  : 4   ; /* [11..8]  */
        unsigned int    vctrl_norm_intra_cu32_rdcost_offset  : 4   ; /* [15..12]  */
        unsigned int    vctrl_strmov_intra_cu4_rdcost_offset  : 4   ; /* [19..16]  */
        unsigned int    vctrl_strmov_intra_cu8_rdcost_offset  : 4   ; /* [23..20]  */
        unsigned int    vctrl_strmov_intra_cu16_rdcost_offset  : 4   ; /* [27..24]  */
        unsigned int    vctrl_strmov_intra_cu32_rdcost_offset  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_NORMAL_INTRA_RDO_FACTOR;

/* Define the union U_VCTRL_NORMAL_MRG_RDO_FACTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_norm_mrg_cu8_rdcost_offset  : 4   ; /* [3..0]  */
        unsigned int    vctrl_norm_mrg_cu16_rdcost_offset  : 4   ; /* [7..4]  */
        unsigned int    vctrl_norm_mrg_cu32_rdcost_offset  : 4   ; /* [11..8]  */
        unsigned int    vctrl_norm_mrg_cu64_rdcost_offset  : 4   ; /* [15..12]  */
        unsigned int    vctrl_strmov_mrg_cu8_rdcost_offset  : 4   ; /* [19..16]  */
        unsigned int    vctrl_strmov_mrg_cu16_rdcost_offset  : 4   ; /* [23..20]  */
        unsigned int    vctrl_strmov_mrg_cu32_rdcost_offset  : 4   ; /* [27..24]  */
        unsigned int    vctrl_strmov_mrg_cu64_rdcost_offset  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_NORMAL_MRG_RDO_FACTOR;

/* Define the union U_VCTRL_NORMAL_FME_RDO_FACTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_norm_fme_cu8_rdcost_offset  : 4   ; /* [3..0]  */
        unsigned int    vctrl_norm_fme_cu16_rdcost_offset  : 4   ; /* [7..4]  */
        unsigned int    vctrl_norm_fme_cu32_rdcost_offset  : 4   ; /* [11..8]  */
        unsigned int    vctrl_norm_fme_cu64_rdcost_offset  : 4   ; /* [15..12]  */
        unsigned int    vctrl_strmov_fme_cu8_rdcost_offset  : 4   ; /* [19..16]  */
        unsigned int    vctrl_strmov_fme_cu16_rdcost_offset  : 4   ; /* [23..20]  */
        unsigned int    vctrl_strmov_fme_cu32_rdcost_offset  : 4   ; /* [27..24]  */
        unsigned int    vctrl_strmov_fme_cu64_rdcost_offset  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_NORMAL_FME_RDO_FACTOR;

/* Define the union U_VCTRL_SKIN_REGION_INTRA_RDO_FACTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_skin_intra_cu4_rdcost_offset  : 4   ; /* [3..0]  */
        unsigned int    vctrl_skin_intra_cu8_rdcost_offset  : 4   ; /* [7..4]  */
        unsigned int    vctrl_skin_intra_cu16_rdcost_offset  : 4   ; /* [11..8]  */
        unsigned int    vctrl_skin_intra_cu32_rdcost_offset  : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobel_str_intra_cu4_rdcost_offset  : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobel_str_intra_cu8_rdcost_offset  : 4   ; /* [23..20]  */
        unsigned int    vctrl_sobel_str_intra_cu16_rdcost_offset  : 4   ; /* [27..24]  */
        unsigned int    vctrl_sobel_str_intra_cu32_rdcost_offset  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SKIN_REGION_INTRA_RDO_FACTOR;

/* Define the union U_VCTRL_SKIN_REGION_MRG_RDO_FACTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_skin_mrg_cu8_rdcost_offset  : 4   ; /* [3..0]  */
        unsigned int    vctrl_skin_mrg_cu16_rdcost_offset  : 4   ; /* [7..4]  */
        unsigned int    vctrl_skin_mrg_cu32_rdcost_offset  : 4   ; /* [11..8]  */
        unsigned int    vctrl_skin_mrg_cu64_rdcost_offset  : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobel_str_mrg_cu8_rdcost_offset  : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobel_str_mrg_cu16_rdcost_offset  : 4   ; /* [23..20]  */
        unsigned int    vctrl_sobel_str_mrg_cu32_rdcost_offset  : 4   ; /* [27..24]  */
        unsigned int    vctrl_sobel_str_mrg_cu64_rdcost_offset  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SKIN_REGION_MRG_RDO_FACTOR;

/* Define the union U_VCTRL_SKIN_REGION_FME_RDO_FACTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_skin_fme_cu8_rdcost_offset  : 4   ; /* [3..0]  */
        unsigned int    vctrl_skin_fme_cu16_rdcost_offset  : 4   ; /* [7..4]  */
        unsigned int    vctrl_skin_fme_cu32_rdcost_offset  : 4   ; /* [11..8]  */
        unsigned int    vctrl_skin_fme_cu64_rdcost_offset  : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobel_str_fme_cu8_rdcost_offset  : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobel_str_fme_cu16_rdcost_offset  : 4   ; /* [23..20]  */
        unsigned int    vctrl_sobel_str_fme_cu32_rdcost_offset  : 4   ; /* [27..24]  */
        unsigned int    vctrl_sobel_str_fme_cu64_rdcost_offset  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SKIN_REGION_FME_RDO_FACTOR;

/* Define the union U_VCTRL_HIGH_EDGE_INTRA_RDO_FACTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_hedge_intra_cu4_rdcost_offset  : 4   ; /* [3..0]  */
        unsigned int    vctrl_hedge_intra_cu8_rdcost_offset  : 4   ; /* [7..4]  */
        unsigned int    vctrl_hedge_intra_cu16_rdcost_offset  : 4   ; /* [11..8]  */
        unsigned int    vctrl_hedge_intra_cu32_rdcost_offset  : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobel_tex_intra_cu4_rdcost_offset  : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobel_tex_intra_cu8_rdcost_offset  : 4   ; /* [23..20]  */
        unsigned int    vctrl_sobel_tex_intra_cu16_rdcost_offset  : 4   ; /* [27..24]  */
        unsigned int    vctrl_sobel_tex_intra_cu32_rdcost_offset  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_HIGH_EDGE_INTRA_RDO_FACTOR;

/* Define the union U_VCTRL_HIGH_EDGE_MRG_RDO_FACTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_hedge_mrg_cu8_rdcost_offset  : 4   ; /* [3..0]  */
        unsigned int    vctrl_hedge_mrg_cu16_rdcost_offset  : 4   ; /* [7..4]  */
        unsigned int    vctrl_hedge_mrg_cu32_rdcost_offset  : 4   ; /* [11..8]  */
        unsigned int    vctrl_hedge_mrg_cu64_rdcost_offset  : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobel_tex_mrg_cu8_rdcost_offset  : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobel_tex_mrg_cu16_rdcost_offset  : 4   ; /* [23..20]  */
        unsigned int    vctrl_sobel_tex_mrg_cu32_rdcost_offset  : 4   ; /* [27..24]  */
        unsigned int    vctrl_sobel_tex_mrg_cu64_rdcost_offset  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_HIGH_EDGE_MRG_RDO_FACTOR;

/* Define the union U_VCTRL_HIGH_EDGE_FME_RDO_FACTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_hedge_fme_cu8_rdcost_offset  : 4   ; /* [3..0]  */
        unsigned int    vctrl_hedge_fme_cu16_rdcost_offset  : 4   ; /* [7..4]  */
        unsigned int    vctrl_hedge_fme_cu32_rdcost_offset  : 4   ; /* [11..8]  */
        unsigned int    vctrl_hedge_fme_cu64_rdcost_offset  : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobel_tex_fme_cu8_rdcost_offset  : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobel_tex_fme_cu16_rdcost_offset  : 4   ; /* [23..20]  */
        unsigned int    vctrl_sobel_tex_fme_cu32_rdcost_offset  : 4   ; /* [27..24]  */
        unsigned int    vctrl_sobel_tex_fme_cu64_rdcost_offset  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_HIGH_EDGE_FME_RDO_FACTOR;

/* Define the union U_VCTRL_EDGE_MOV_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roundingedgemovmechanism  : 3   ; /* [2..0]  */
        unsigned int    vctrl_roundingedgemovdegreethresh  : 7   ; /* [9..3]  */
        unsigned int    vctrl_roundingedgemovforcezeroresidthresh  : 3   ; /* [12..10]  */
        unsigned int    vctrl_roundingedgemovac32sum  : 5   ; /* [17..13]  */
        unsigned int    vctrl_roundingedgemovac16sum  : 4   ; /* [21..18]  */
        unsigned int    vctrl_roundingedgemovlowfreqacblk32  : 4   ; /* [25..22]  */
        unsigned int    vctrl_roundingedgemovlowfreqacblk16  : 4   ; /* [29..26]  */
        unsigned int    Reserved_79           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_EDGE_MOV_COEFF_DENOISE;

/* Define the union U_VCTRL_SOBEL_STRONG_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roundingsobelstrmechanism  : 3   ; /* [2..0]  */
        unsigned int    vctrl_roundingsobelstrdegreethresh  : 7   ; /* [9..3]  */
        unsigned int    vctrl_roundingsobelstrforcezeroresidthresh  : 3   ; /* [12..10]  */
        unsigned int    vctrl_roundingsobelstrac32sum  : 5   ; /* [17..13]  */
        unsigned int    vctrl_roundingsobelstrac16sum  : 4   ; /* [21..18]  */
        unsigned int    vctrl_roundingsobelstrlowfreqacblk32  : 4   ; /* [25..22]  */
        unsigned int    vctrl_roundingsobelstrlowfreqacblk16  : 4   ; /* [29..26]  */
        unsigned int    Reserved_80           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SOBEL_STRONG_COEFF_DENOISE;

/* Define the union U_VCTRL_SOBEL_WEAK_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roundingsobelweakmechanism  : 3   ; /* [2..0]  */
        unsigned int    vctrl_roundingsobelweakdegreethresh  : 7   ; /* [9..3]  */
        unsigned int    vctrl_roundingsobelweakforcezeroresidthresh  : 3   ; /* [12..10]  */
        unsigned int    vctrl_roundingsobelweakac32sum  : 5   ; /* [17..13]  */
        unsigned int    vctrl_roundingsobelweakac16sum  : 4   ; /* [21..18]  */
        unsigned int    vctrl_roundingsobelweaklowfreqacblk32  : 4   ; /* [25..22]  */
        unsigned int    vctrl_roundingsobelweaklowfreqacblk16  : 4   ; /* [29..26]  */
        unsigned int    Reserved_81           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SOBEL_WEAK_COEFF_DENOISE;

/* Define the union U_VCTRL_NORM_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roundingmechanism  : 3   ; /* [2..0]  */
        unsigned int    vctrl_roundingdegreethresh  : 7   ; /* [9..3]  */
        unsigned int    vctrl_roundingforcezeroresidthresh  : 3   ; /* [12..10]  */
        unsigned int    vctrl_roundingac32sum  : 5   ; /* [17..13]  */
        unsigned int    vctrl_roundingac16sum  : 4   ; /* [21..18]  */
        unsigned int    vctrl_roundinglowfreqacblk32  : 4   ; /* [25..22]  */
        unsigned int    vctrl_roundinglowfreqacblk16  : 4   ; /* [29..26]  */
        unsigned int    Reserved_82           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_NORM_COEFF_DENOISE;

/* Define the union U_VCTRL_SKIN_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roundingskinmechanism  : 3   ; /* [2..0]  */
        unsigned int    vctrl_roundingskindegreethresh  : 7   ; /* [9..3]  */
        unsigned int    vctrl_roundingskinforcezeroresidthresh  : 3   ; /* [12..10]  */
        unsigned int    vctrl_roundingskinac32sum  : 5   ; /* [17..13]  */
        unsigned int    vctrl_roundingskinac16sum  : 4   ; /* [21..18]  */
        unsigned int    vctrl_roundingskinlowfreqacblk32  : 4   ; /* [25..22]  */
        unsigned int    vctrl_roundingskinlowfreqacblk16  : 4   ; /* [29..26]  */
        unsigned int    Reserved_83           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SKIN_COEFF_DENOISE;

/* Define the union U_VCTRL_STILL_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roundingstillmechanism  : 3   ; /* [2..0]  */
        unsigned int    vctrl_roundingstilldegreethresh  : 7   ; /* [9..3]  */
        unsigned int    vctrl_roundingstillforcezeroresidthresh  : 3   ; /* [12..10]  */
        unsigned int    vctrl_roundingstillac32sum  : 5   ; /* [17..13]  */
        unsigned int    vctrl_roundingstillac16sum  : 4   ; /* [21..18]  */
        unsigned int    vctrl_roundingstilllowfreqacblk32  : 4   ; /* [25..22]  */
        unsigned int    vctrl_roundingstilllowfreqacblk16  : 4   ; /* [29..26]  */
        unsigned int    Reserved_84           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_STILL_COEFF_DENOISE;

/* Define the union U_VCTRL_EDGE_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roundingedgemechanism  : 3   ; /* [2..0]  */
        unsigned int    vctrl_roundingedgedegreethresh  : 7   ; /* [9..3]  */
        unsigned int    vctrl_roundingedgeforcezeroresidthresh  : 3   ; /* [12..10]  */
        unsigned int    vctrl_roundingedgeac32sum  : 5   ; /* [17..13]  */
        unsigned int    vctrl_roundingedgeac16sum  : 4   ; /* [21..18]  */
        unsigned int    vctrl_roundingedgelowfreqacblk32  : 4   ; /* [25..22]  */
        unsigned int    vctrl_roundingedgelowfreqacblk16  : 4   ; /* [29..26]  */
        unsigned int    Reserved_85           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_EDGE_COEFF_DENOISE;

/* Define the union U_VCTRL_OFFSET_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roundingoffset16x16  : 4   ; /* [3..0]  */
        unsigned int    vctrl_roundingoffset32x32  : 4   ; /* [7..4]  */
        unsigned int    vctrl_roundingskinoffset16x16  : 4   ; /* [11..8]  */
        unsigned int    vctrl_roundingskinoffset32x32  : 4   ; /* [15..12]  */
        unsigned int    vctrl_roundingstilloffset16x16  : 4   ; /* [19..16]  */
        unsigned int    vctrl_roundingstilloffset32x32  : 4   ; /* [23..20]  */
        unsigned int    vctrl_roundingedgeoffset16x16  : 4   ; /* [27..24]  */
        unsigned int    vctrl_roundingedgeoffset32x32  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OFFSET_COEFF_DENOISE;

/* Define the union U_VCTRL_NEW_OFFSET_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roundingedgemovoffset16x16  : 4   ; /* [3..0]  */
        unsigned int    vctrl_roundingedgemovoffset32x32  : 4   ; /* [7..4]  */
        unsigned int    vctrl_roundingsobelstroffset16x16  : 4   ; /* [11..8]  */
        unsigned int    vctrl_roundingsobelstroffset32x32  : 4   ; /* [15..12]  */
        unsigned int    vctrl_roundingsobelweakoffset16x16  : 4   ; /* [19..16]  */
        unsigned int    vctrl_roundingsobelweakoffset32x32  : 4   ; /* [23..20]  */
        unsigned int    Reserved_86           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_NEW_OFFSET_COEFF_DENOISE;

/* Define the union U_VCTRL_EDGEMOV_TR32X32_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_edgemov32_coeff_protect_num  : 10  ; /* [9..0]  */
        unsigned int    Reserved_88           : 6   ; /* [15..10]  */
        unsigned int    vctrl_edgemov32_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_87           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_EDGEMOV_TR32X32_COEFF_DENOISE;

/* Define the union U_VCTRL_EDGEMOV_TR16X16_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_edgemov16_coeff_protect_num  : 8   ; /* [7..0]  */
        unsigned int    Reserved_90           : 8   ; /* [15..8]  */
        unsigned int    vctrl_edgemov16_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_89           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_EDGEMOV_TR16X16_COEFF_DENOISE;

/* Define the union U_VCTRL_SOBELSTR_TR32X32_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_sobelstr32_coeff_protect_num  : 10  ; /* [9..0]  */
        unsigned int    Reserved_92           : 6   ; /* [15..10]  */
        unsigned int    vctrl_sobelstr32_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_91           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SOBELSTR_TR32X32_COEFF_DENOISE;

/* Define the union U_VCTRL_SOBELSTR_TR16X16_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_sobelstr16_coeff_protect_num  : 8   ; /* [7..0]  */
        unsigned int    Reserved_94           : 8   ; /* [15..8]  */
        unsigned int    vctrl_sobelstr16_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_93           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SOBELSTR_TR16X16_COEFF_DENOISE;

/* Define the union U_VCTRL_SOBELWEAK_TR32X32_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_sobelweak32_coeff_protect_num  : 10  ; /* [9..0]  */
        unsigned int    Reserved_96           : 6   ; /* [15..10]  */
        unsigned int    vctrl_sobelweak32_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_95           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SOBELWEAK_TR32X32_COEFF_DENOISE;

/* Define the union U_VCTRL_SOBELWEAK_TR16X16_COEFF_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_sobelweak16_coeff_protect_num  : 8   ; /* [7..0]  */
        unsigned int    Reserved_98           : 8   ; /* [15..8]  */
        unsigned int    vctrl_sobelweak16_tr1_denois_max_num  : 5   ; /* [20..16]  */
        unsigned int    Reserved_97           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SOBELWEAK_TR16X16_COEFF_DENOISE;

/* Define the union U_VCTRL_SEL_INTRA_RDO_FACTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_norm_sel_cu8_rd_offset  : 4   ; /* [3..0]  */
        unsigned int    vctrl_skin_sel_cu8_rd_offset  : 4   ; /* [7..4]  */
        unsigned int    vctrl_hedge_sel_cu8_rd_offset  : 4   ; /* [11..8]  */
        unsigned int    vctrl_strmov_sel_cu8_rd_offset  : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobelstr_sel_cu8_rd_offset  : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobeltex_sel_cu8_rd_offset  : 4   ; /* [23..20]  */
        unsigned int    Reserved_99           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SEL_INTRA_RDO_FACTOR;

/* Define the union U_VCTRL_SEL_INTER_RDO_FACTOR_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_skin_inter_cu_rdcost_offset  : 12  ; /* [11..0]  */
        unsigned int    vctrl_hedge_inter_cu_rdcost_offset  : 12  ; /* [23..12]  */
        unsigned int    vctrl_skin_layer_rdcost_offset  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SEL_INTER_RDO_FACTOR_0;

/* Define the union U_VCTRL_SEL_INTER_RDO_FACTOR_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_sobelstr_inter_cu_rdcost_offset  : 12  ; /* [11..0]  */
        unsigned int    vctrl_sobeltex_inter_cu_rdcost_offset  : 12  ; /* [23..12]  */
        unsigned int    vctrl_hedge_layer_rdcost_offset  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SEL_INTER_RDO_FACTOR_1;

/* Define the union U_VCTRL_SEL_INTER_RDO_FACTOR_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_strmov_inter_cu_rdcost_offset  : 12  ; /* [11..0]  */
        unsigned int    vctrl_norm_inter_cu_rdcost_offset  : 12  ; /* [23..12]  */
        unsigned int    vctrl_strmov_layer_rdcost_offset  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SEL_INTER_RDO_FACTOR_2;

/* Define the union U_VCTRL_SEL_LAYER_RDO_FACTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_sobelstr_layer_rdcost_offset  : 8   ; /* [7..0]  */
        unsigned int    vctrl_sobeltex_layer_rdcost_offset  : 8   ; /* [15..8]  */
        unsigned int    vctrl_norm_layer_rdcost_offset  : 8   ; /* [23..16]  */
        unsigned int    Reserved_100          : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SEL_LAYER_RDO_FACTOR;

/* Define the union U_VCTRL_NORM_ENG_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_norm_isolate_ac_enable  : 1   ; /* [0]  */
        unsigned int    vctrl_norm_force_zero_cnt  : 3   ; /* [3..1]  */
        unsigned int    vctrl_norm_engsum_32  : 6   ; /* [9..4]  */
        unsigned int    vctrl_norm_engcnt_32  : 5   ; /* [14..10]  */
        unsigned int    vctrl_norm_engsum_16  : 6   ; /* [20..15]  */
        unsigned int    vctrl_norm_engcnt_16  : 5   ; /* [25..21]  */
        unsigned int    Reserved_101          : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_NORM_ENG_DENOISE;

/* Define the union U_VCTRL_SKIN_ENG_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_skin_isolate_ac_enable  : 1   ; /* [0]  */
        unsigned int    vctrl_skin_force_zero_cnt  : 3   ; /* [3..1]  */
        unsigned int    vctrl_skin_engsum_32  : 6   ; /* [9..4]  */
        unsigned int    vctrl_skin_engcnt_32  : 5   ; /* [14..10]  */
        unsigned int    vctrl_skin_engsum_16  : 6   ; /* [20..15]  */
        unsigned int    vctrl_skin_engcnt_16  : 5   ; /* [25..21]  */
        unsigned int    Reserved_102          : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SKIN_ENG_DENOISE;

/* Define the union U_VCTRL_STILL_ENG_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_still_isolate_ac_enable  : 1   ; /* [0]  */
        unsigned int    vctrl_still_force_zero_cnt  : 3   ; /* [3..1]  */
        unsigned int    vctrl_still_engsum_32  : 6   ; /* [9..4]  */
        unsigned int    vctrl_still_engcnt_32  : 5   ; /* [14..10]  */
        unsigned int    vctrl_still_engsum_16  : 6   ; /* [20..15]  */
        unsigned int    vctrl_still_engcnt_16  : 5   ; /* [25..21]  */
        unsigned int    Reserved_103          : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_STILL_ENG_DENOISE;

/* Define the union U_VCTRL_STREDGE_ENG_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_stredge_isolate_ac_enable  : 1   ; /* [0]  */
        unsigned int    vctrl_stredge_force_zero_cnt  : 3   ; /* [3..1]  */
        unsigned int    vctrl_stredge_engsum_32  : 6   ; /* [9..4]  */
        unsigned int    vctrl_stredge_engcnt_32  : 5   ; /* [14..10]  */
        unsigned int    vctrl_stredge_engsum_16  : 6   ; /* [20..15]  */
        unsigned int    vctrl_stredge_engcnt_16  : 5   ; /* [25..21]  */
        unsigned int    Reserved_104          : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_STREDGE_ENG_DENOISE;

/* Define the union U_VCTRL_EDGEMOV_ENG_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_edgemov_isolate_ac_enable  : 1   ; /* [0]  */
        unsigned int    vctrl_edgemov_force_zero_cnt  : 3   ; /* [3..1]  */
        unsigned int    vctrl_edgemov_engsum_32  : 6   ; /* [9..4]  */
        unsigned int    vctrl_edgemov_engcnt_32  : 5   ; /* [14..10]  */
        unsigned int    vctrl_edgemov_engsum_16  : 6   ; /* [20..15]  */
        unsigned int    vctrl_edgemov_engcnt_16  : 5   ; /* [25..21]  */
        unsigned int    Reserved_105          : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_EDGEMOV_ENG_DENOISE;

/* Define the union U_VCTRL_SOBELSTR_ENG_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_sobelstr_isolate_ac_enable  : 1   ; /* [0]  */
        unsigned int    vctrl_sobelstr_force_zero_cnt  : 3   ; /* [3..1]  */
        unsigned int    vctrl_sobelstr_engsum_32  : 6   ; /* [9..4]  */
        unsigned int    vctrl_sobelstr_engcnt_32  : 5   ; /* [14..10]  */
        unsigned int    vctrl_sobelstr_engsum_16  : 6   ; /* [20..15]  */
        unsigned int    vctrl_sobelstr_engcnt_16  : 5   ; /* [25..21]  */
        unsigned int    Reserved_106          : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SOBELSTR_ENG_DENOISE;

/* Define the union U_VCTRL_SOBELWK_ENG_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_sobelwk_isolate_ac_enable  : 1   ; /* [0]  */
        unsigned int    vctrl_sobelwk_force_zero_cnt  : 3   ; /* [3..1]  */
        unsigned int    vctrl_sobelwk_engsum_32  : 6   ; /* [9..4]  */
        unsigned int    vctrl_sobelwk_engcnt_32  : 5   ; /* [14..10]  */
        unsigned int    vctrl_sobelwk_engsum_16  : 6   ; /* [20..15]  */
        unsigned int    vctrl_sobelwk_engcnt_16  : 5   ; /* [25..21]  */
        unsigned int    Reserved_107          : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_SOBELWK_ENG_DENOISE;

/* Define the union U_VCTRL_OVERHEAD_PIPE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_109          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_PIPE;

/* Define the union U_VCTRL_OVERHEAD_PMELD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_110          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_PMELD;

/* Define the union U_VCTRL_OVERHEAD_NBI */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_111          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_NBI;

/* Define the union U_VCTRL_OVERHEAD_QPG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_112          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_QPG;

/* Define the union U_VCTRL_OVERHEAD_PME */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_113          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_PME;

/* Define the union U_VCTRL_OVERHEAD_REFLD_LUMA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_114          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_REFLD_LUMA;

/* Define the union U_VCTRL_OVERHEAD_REFLD_CHMA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_115          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_REFLD_CHMA;

/* Define the union U_VCTRL_OVERHEAD_CURLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_116          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_CURLD;

/* Define the union U_VCTRL_OVERHEAD_IME */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_117          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_IME;

/* Define the union U_VCTRL_OVERHEAD_PINTRA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_118          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_PINTRA;

/* Define the union U_VCTRL_OVERHEAD_MRG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_119          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_MRG;

/* Define the union U_VCTRL_OVERHEAD_FME */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_120          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_FME;

/* Define the union U_VCTRL_OVERHEAD_PMV */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_121          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_PMV;

/* Define the union U_VCTRL_OVERHEAD_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_122          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_SEL;

/* Define the union U_VCTRL_OVERHEAD_INTRA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_123          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_INTRA;

/* Define the union U_VCTRL_OVERHEAD_SAO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_124          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_SAO;

/* Define the union U_VCTRL_OVERHEAD_DBLK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_125          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_DBLK;

/* Define the union U_VCTRL_OVERHEAD_PACK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_126          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_PACK;

/* Define the union U_VCTRL_OVERHEAD_CABAC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_num_ovr_pref_base  : 12  ; /* [11..0]  */
        unsigned int    Reserved_127          : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VCTRL_OVERHEAD_CABAC;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_VCTRL_ROI_CFG0       VCTRL_ROI_CFG0;
    volatile U_VCTRL_ROI_CFG1       VCTRL_ROI_CFG1;
    volatile U_VCTRL_ROI_CFG2       VCTRL_ROI_CFG2;
    volatile U_VCTRL_ROI_SIZE_0     VCTRL_ROI_SIZE_0;
    volatile U_VCTRL_ROI_SIZE_1     VCTRL_ROI_SIZE_1;
    volatile U_VCTRL_ROI_SIZE_2     VCTRL_ROI_SIZE_2;
    volatile U_VCTRL_ROI_SIZE_3     VCTRL_ROI_SIZE_3;
    volatile U_VCTRL_ROI_SIZE_4     VCTRL_ROI_SIZE_4;
    volatile U_VCTRL_ROI_SIZE_5     VCTRL_ROI_SIZE_5;
    volatile U_VCTRL_ROI_SIZE_6     VCTRL_ROI_SIZE_6;
    volatile U_VCTRL_ROI_SIZE_7     VCTRL_ROI_SIZE_7;
    volatile U_VCTRL_ROI_START_0    VCTRL_ROI_START_0;
    volatile U_VCTRL_ROI_START_1    VCTRL_ROI_START_1;
    volatile U_VCTRL_ROI_START_2    VCTRL_ROI_START_2;
    volatile U_VCTRL_ROI_START_3    VCTRL_ROI_START_3;
    volatile U_VCTRL_ROI_START_4    VCTRL_ROI_START_4;
    volatile U_VCTRL_ROI_START_5    VCTRL_ROI_START_5;
    volatile U_VCTRL_ROI_START_6    VCTRL_ROI_START_6;
    volatile U_VCTRL_ROI_START_7    VCTRL_ROI_START_7;
    volatile U_VCTRL_LCU_TARGET_BIT   VCTRL_LCU_TARGET_BIT;
    volatile U_VCTRL_NARROW_THRESHOLD   VCTRL_NARROW_THRESHOLD;
    volatile U_VCTRL_LCU_BASELINE   VCTRL_LCU_BASELINE;
    volatile unsigned int           RESERVED0[2];
    volatile U_VCTRL_PIPE_STATE     VCTRL_PIPE_STATE;
    volatile U_VCTRL_MODULE_STATE   VCTRL_MODULE_STATE;
    volatile U_VCTRL_CUR_TILE_INFO_0   VCTRL_CUR_TILE_INFO_0;
    volatile U_VCTRL_CUR_TILE_INFO_1   VCTRL_CUR_TILE_INFO_1;
    volatile U_VCTRL_CUR_TILE_INFO_2   VCTRL_CUR_TILE_INFO_2;
    volatile U_VCTRL_SLICE_INFO     VCTRL_SLICE_INFO;
    volatile U_VCTRL_CTB_PIPE_S_COORD   VCTRL_CTB_PIPE_S_COORD;
    volatile unsigned int           RESERVED1;
    volatile U_VCTRL_NORM_TR32X32_COEFF_DENOISE   VCTRL_NORM_TR32X32_COEFF_DENOISE;
    volatile U_VCTRL_NORM_TR16X16_COEFF_DENOISE   VCTRL_NORM_TR16X16_COEFF_DENOISE;
    volatile U_VCTRL_SKIN_TR32X32_COEFF_DENOISE   VCTRL_SKIN_TR32X32_COEFF_DENOISE;
    volatile U_VCTRL_SKIN_TR16X16_COEFF_DENOISE   VCTRL_SKIN_TR16X16_COEFF_DENOISE;
    volatile U_VCTRL_STATIC_TR32X32_COEFF_DENOISE   VCTRL_STATIC_TR32X32_COEFF_DENOISE;
    volatile U_VCTRL_STATIC_TR16X16_COEFF_DENOISE   VCTRL_STATIC_TR16X16_COEFF_DENOISE;
    volatile U_VCTRL_HEDGE_TR32X32_COEFF_DENOISE   VCTRL_HEDGE_TR32X32_COEFF_DENOISE;
    volatile U_VCTRL_HEDGE_TR16X16_COEFF_DENOISE   VCTRL_HEDGE_TR16X16_COEFF_DENOISE;
    volatile U_VCTRL_NORMAL_INTRA_RDO_FACTOR   VCTRL_NORMAL_INTRA_RDO_FACTOR;
    volatile U_VCTRL_NORMAL_MRG_RDO_FACTOR   VCTRL_NORMAL_MRG_RDO_FACTOR;
    volatile U_VCTRL_NORMAL_FME_RDO_FACTOR   VCTRL_NORMAL_FME_RDO_FACTOR;
    volatile U_VCTRL_SKIN_REGION_INTRA_RDO_FACTOR   VCTRL_SKIN_REGION_INTRA_RDO_FACTOR;
    volatile U_VCTRL_SKIN_REGION_MRG_RDO_FACTOR   VCTRL_SKIN_REGION_MRG_RDO_FACTOR;
    volatile U_VCTRL_SKIN_REGION_FME_RDO_FACTOR   VCTRL_SKIN_REGION_FME_RDO_FACTOR;
    volatile U_VCTRL_HIGH_EDGE_INTRA_RDO_FACTOR   VCTRL_HIGH_EDGE_INTRA_RDO_FACTOR;
    volatile U_VCTRL_HIGH_EDGE_MRG_RDO_FACTOR   VCTRL_HIGH_EDGE_MRG_RDO_FACTOR;
    volatile U_VCTRL_HIGH_EDGE_FME_RDO_FACTOR   VCTRL_HIGH_EDGE_FME_RDO_FACTOR;
    volatile U_VCTRL_EDGE_MOV_COEFF_DENOISE   VCTRL_EDGE_MOV_COEFF_DENOISE;
    volatile U_VCTRL_SOBEL_STRONG_COEFF_DENOISE   VCTRL_SOBEL_STRONG_COEFF_DENOISE;
    volatile U_VCTRL_SOBEL_WEAK_COEFF_DENOISE   VCTRL_SOBEL_WEAK_COEFF_DENOISE;
    volatile U_VCTRL_NORM_COEFF_DENOISE   VCTRL_NORM_COEFF_DENOISE;
    volatile U_VCTRL_SKIN_COEFF_DENOISE   VCTRL_SKIN_COEFF_DENOISE;
    volatile U_VCTRL_STILL_COEFF_DENOISE   VCTRL_STILL_COEFF_DENOISE;
    volatile U_VCTRL_EDGE_COEFF_DENOISE   VCTRL_EDGE_COEFF_DENOISE;
    volatile U_VCTRL_OFFSET_COEFF_DENOISE   VCTRL_OFFSET_COEFF_DENOISE;
    volatile U_VCTRL_NEW_OFFSET_COEFF_DENOISE   VCTRL_NEW_OFFSET_COEFF_DENOISE;
    volatile U_VCTRL_EDGEMOV_TR32X32_COEFF_DENOISE   VCTRL_EDGEMOV_TR32X32_COEFF_DENOISE;
    volatile U_VCTRL_EDGEMOV_TR16X16_COEFF_DENOISE   VCTRL_EDGEMOV_TR16X16_COEFF_DENOISE;
    volatile U_VCTRL_SOBELSTR_TR32X32_COEFF_DENOISE   VCTRL_SOBELSTR_TR32X32_COEFF_DENOISE;
    volatile U_VCTRL_SOBELSTR_TR16X16_COEFF_DENOISE   VCTRL_SOBELSTR_TR16X16_COEFF_DENOISE;
    volatile U_VCTRL_SOBELWEAK_TR32X32_COEFF_DENOISE   VCTRL_SOBELWEAK_TR32X32_COEFF_DENOISE;
    volatile U_VCTRL_SOBELWEAK_TR16X16_COEFF_DENOISE   VCTRL_SOBELWEAK_TR16X16_COEFF_DENOISE;
    volatile U_VCTRL_SEL_INTRA_RDO_FACTOR   VCTRL_SEL_INTRA_RDO_FACTOR;
    volatile U_VCTRL_SEL_INTER_RDO_FACTOR_0   VCTRL_SEL_INTER_RDO_FACTOR_0;
    volatile U_VCTRL_SEL_INTER_RDO_FACTOR_1   VCTRL_SEL_INTER_RDO_FACTOR_1;
    volatile U_VCTRL_SEL_INTER_RDO_FACTOR_2   VCTRL_SEL_INTER_RDO_FACTOR_2;
    volatile U_VCTRL_SEL_LAYER_RDO_FACTOR   VCTRL_SEL_LAYER_RDO_FACTOR;
    volatile U_VCTRL_NORM_ENG_DENOISE   VCTRL_NORM_ENG_DENOISE;
    volatile U_VCTRL_SKIN_ENG_DENOISE   VCTRL_SKIN_ENG_DENOISE;
    volatile U_VCTRL_STILL_ENG_DENOISE   VCTRL_STILL_ENG_DENOISE;
    volatile U_VCTRL_STREDGE_ENG_DENOISE   VCTRL_STREDGE_ENG_DENOISE;
    volatile U_VCTRL_EDGEMOV_ENG_DENOISE   VCTRL_EDGEMOV_ENG_DENOISE;
    volatile U_VCTRL_SOBELSTR_ENG_DENOISE   VCTRL_SOBELSTR_ENG_DENOISE;
    volatile U_VCTRL_SOBELWK_ENG_DENOISE   VCTRL_SOBELWK_ENG_DENOISE;
    volatile unsigned int           RESERVED4[14];
    volatile unsigned int           VCTRL_PERF_TOTAL_PIPE;
    volatile unsigned int           VCTRL_PERF_TOTAL_PMELD;
    volatile unsigned int           VCTRL_PERF_TOTAL_NBI;
    volatile unsigned int           VCTRL_PERF_TOTAL_QPG;
    volatile unsigned int           VCTRL_PERF_TOTAL_PME;
    volatile unsigned int           VCTRL_PERF_TOTAL_REFLD_LUMA;
    volatile unsigned int           VCTRL_PERF_TOTAL_REFLD_CHMA;
    volatile unsigned int           VCTRL_PERF_TOTAL_CURLD;
    volatile unsigned int           VCTRL_PERF_TOTAL_IME;
    volatile unsigned int           VCTRL_PERF_TOTAL_PINTRA;
    volatile unsigned int           VCTRL_PERF_TOTAL_MRG;
    volatile unsigned int           VCTRL_PERF_TOTAL_FME;
    volatile unsigned int           VCTRL_PERF_TOTAL_PMV;
    volatile unsigned int           VCTRL_PERF_TOTAL_SEL;
    volatile unsigned int           VCTRL_PERF_TOTAL_INTRA;
    volatile unsigned int           VCTRL_PERF_TOTAL_SAO;
    volatile unsigned int           VCTRL_PERF_TOTAL_DBLK;
    volatile unsigned int           VCTRL_PERF_TOTAL_PACK;
    volatile unsigned int           VCTRL_PERF_TOTAL_CABAC;
    volatile U_VCTRL_OVERHEAD_PIPE   VCTRL_OVERHEAD_PIPE;
    volatile U_VCTRL_OVERHEAD_PMELD   VCTRL_OVERHEAD_PMELD;
    volatile U_VCTRL_OVERHEAD_NBI   VCTRL_OVERHEAD_NBI;
    volatile U_VCTRL_OVERHEAD_QPG   VCTRL_OVERHEAD_QPG;
    volatile U_VCTRL_OVERHEAD_PME   VCTRL_OVERHEAD_PME;
    volatile U_VCTRL_OVERHEAD_REFLD_LUMA   VCTRL_OVERHEAD_REFLD_LUMA;
    volatile U_VCTRL_OVERHEAD_REFLD_CHMA   VCTRL_OVERHEAD_REFLD_CHMA;
    volatile U_VCTRL_OVERHEAD_CURLD   VCTRL_OVERHEAD_CURLD;
    volatile U_VCTRL_OVERHEAD_IME   VCTRL_OVERHEAD_IME;
    volatile U_VCTRL_OVERHEAD_PINTRA   VCTRL_OVERHEAD_PINTRA;
    volatile U_VCTRL_OVERHEAD_MRG   VCTRL_OVERHEAD_MRG;
    volatile U_VCTRL_OVERHEAD_FME   VCTRL_OVERHEAD_FME;
    volatile U_VCTRL_OVERHEAD_PMV   VCTRL_OVERHEAD_PMV;
    volatile U_VCTRL_OVERHEAD_SEL   VCTRL_OVERHEAD_SEL;
    volatile U_VCTRL_OVERHEAD_INTRA   VCTRL_OVERHEAD_INTRA;
    volatile U_VCTRL_OVERHEAD_SAO   VCTRL_OVERHEAD_SAO;
    volatile U_VCTRL_OVERHEAD_DBLK   VCTRL_OVERHEAD_DBLK;
    volatile U_VCTRL_OVERHEAD_PACK   VCTRL_OVERHEAD_PACK;
    volatile U_VCTRL_OVERHEAD_CABAC   VCTRL_OVERHEAD_CABAC;


} S_vctrl_REGS_TYPE;

/* Declare the struct pointor of the module vctrl */
extern volatile S_vctrl_REGS_TYPE *gopvctrlAllReg;

/* Declare the functions that set the member value */
int iSetVCTRL_ROI_CFG0vctrl_region0en(unsigned int uvctrl_region0en);
int iSetVCTRL_ROI_CFG0vctrl_region1en(unsigned int uvctrl_region1en);
int iSetVCTRL_ROI_CFG0vctrl_region2en(unsigned int uvctrl_region2en);
int iSetVCTRL_ROI_CFG0vctrl_region3en(unsigned int uvctrl_region3en);
int iSetVCTRL_ROI_CFG0vctrl_region4en(unsigned int uvctrl_region4en);
int iSetVCTRL_ROI_CFG0vctrl_region5en(unsigned int uvctrl_region5en);
int iSetVCTRL_ROI_CFG0vctrl_region6en(unsigned int uvctrl_region6en);
int iSetVCTRL_ROI_CFG0vctrl_region7en(unsigned int uvctrl_region7en);
int iSetVCTRL_ROI_CFG0vctrl_absqp0(unsigned int uvctrl_absqp0);
int iSetVCTRL_ROI_CFG0vctrl_absqp1(unsigned int uvctrl_absqp1);
int iSetVCTRL_ROI_CFG0vctrl_absqp2(unsigned int uvctrl_absqp2);
int iSetVCTRL_ROI_CFG0vctrl_absqp3(unsigned int uvctrl_absqp3);
int iSetVCTRL_ROI_CFG0vctrl_absqp4(unsigned int uvctrl_absqp4);
int iSetVCTRL_ROI_CFG0vctrl_absqp5(unsigned int uvctrl_absqp5);
int iSetVCTRL_ROI_CFG0vctrl_absqp6(unsigned int uvctrl_absqp6);
int iSetVCTRL_ROI_CFG0vctrl_absqp7(unsigned int uvctrl_absqp7);
int iSetVCTRL_ROI_CFG0vctrl_region0keep(unsigned int uvctrl_region0keep);
int iSetVCTRL_ROI_CFG0vctrl_region1keep(unsigned int uvctrl_region1keep);
int iSetVCTRL_ROI_CFG0vctrl_region2keep(unsigned int uvctrl_region2keep);
int iSetVCTRL_ROI_CFG0vctrl_region3keep(unsigned int uvctrl_region3keep);
int iSetVCTRL_ROI_CFG0vctrl_region4keep(unsigned int uvctrl_region4keep);
int iSetVCTRL_ROI_CFG0vctrl_region5keep(unsigned int uvctrl_region5keep);
int iSetVCTRL_ROI_CFG0vctrl_region6keep(unsigned int uvctrl_region6keep);
int iSetVCTRL_ROI_CFG0vctrl_region7keep(unsigned int uvctrl_region7keep);
int iSetVCTRL_ROI_CFG0vctrl_roi_en(unsigned int uvctrl_roi_en);
int iSetVCTRL_ROI_CFG1vctrl_roiqp0(unsigned int uvctrl_roiqp0);
int iSetVCTRL_ROI_CFG1vctrl_roiqp1(unsigned int uvctrl_roiqp1);
int iSetVCTRL_ROI_CFG1vctrl_roiqp2(unsigned int uvctrl_roiqp2);
int iSetVCTRL_ROI_CFG1vctrl_roiqp3(unsigned int uvctrl_roiqp3);
int iSetVCTRL_ROI_CFG2vctrl_roiqp4(unsigned int uvctrl_roiqp4);
int iSetVCTRL_ROI_CFG2vctrl_roiqp5(unsigned int uvctrl_roiqp5);
int iSetVCTRL_ROI_CFG2vctrl_roiqp6(unsigned int uvctrl_roiqp6);
int iSetVCTRL_ROI_CFG2vctrl_roiqp7(unsigned int uvctrl_roiqp7);
int iSetVCTRL_ROI_SIZE_0vctrl_size0_roiwidth(unsigned int uvctrl_size0_roiwidth);
int iSetVCTRL_ROI_SIZE_0vctrl_size0_roiheight(unsigned int uvctrl_size0_roiheight);
int iSetVCTRL_ROI_SIZE_1vctrl_size1_roiwidth(unsigned int uvctrl_size1_roiwidth);
int iSetVCTRL_ROI_SIZE_1vctrl_size1_roiheight(unsigned int uvctrl_size1_roiheight);
int iSetVCTRL_ROI_SIZE_2vctrl_size2_roiwidth(unsigned int uvctrl_size2_roiwidth);
int iSetVCTRL_ROI_SIZE_2vctrl_size2_roiheight(unsigned int uvctrl_size2_roiheight);
int iSetVCTRL_ROI_SIZE_3vctrl_size3_roiwidth(unsigned int uvctrl_size3_roiwidth);
int iSetVCTRL_ROI_SIZE_3vctrl_size3_roiheight(unsigned int uvctrl_size3_roiheight);
int iSetVCTRL_ROI_SIZE_4vctrl_size4_roiwidth(unsigned int uvctrl_size4_roiwidth);
int iSetVCTRL_ROI_SIZE_4vctrl_size4_roiheight(unsigned int uvctrl_size4_roiheight);
int iSetVCTRL_ROI_SIZE_5vctrl_size5_roiwidth(unsigned int uvctrl_size5_roiwidth);
int iSetVCTRL_ROI_SIZE_5vctrl_size5_roiheight(unsigned int uvctrl_size5_roiheight);
int iSetVCTRL_ROI_SIZE_6vctrl_size6_roiwidth(unsigned int uvctrl_size6_roiwidth);
int iSetVCTRL_ROI_SIZE_6vctrl_size6_roiheight(unsigned int uvctrl_size6_roiheight);
int iSetVCTRL_ROI_SIZE_7vctrl_size7_roiwidth(unsigned int uvctrl_size7_roiwidth);
int iSetVCTRL_ROI_SIZE_7vctrl_size7_roiheight(unsigned int uvctrl_size7_roiheight);
int iSetVCTRL_ROI_START_0vctrl_start0_roistartx(unsigned int uvctrl_start0_roistartx);
int iSetVCTRL_ROI_START_0vctrl_start0_roistarty(unsigned int uvctrl_start0_roistarty);
int iSetVCTRL_ROI_START_1vctrl_start1_roistartx(unsigned int uvctrl_start1_roistartx);
int iSetVCTRL_ROI_START_1vctrl_start1_roistarty(unsigned int uvctrl_start1_roistarty);
int iSetVCTRL_ROI_START_2vctrl_start2_roistartx(unsigned int uvctrl_start2_roistartx);
int iSetVCTRL_ROI_START_2vctrl_start2_roistarty(unsigned int uvctrl_start2_roistarty);
int iSetVCTRL_ROI_START_3vctrl_start3_roistartx(unsigned int uvctrl_start3_roistartx);
int iSetVCTRL_ROI_START_3vctrl_start3_roistarty(unsigned int uvctrl_start3_roistarty);
int iSetVCTRL_ROI_START_4vctrl_start4_roistartx(unsigned int uvctrl_start4_roistartx);
int iSetVCTRL_ROI_START_4vctrl_start4_roistarty(unsigned int uvctrl_start4_roistarty);
int iSetVCTRL_ROI_START_5vctrl_start5_roistartx(unsigned int uvctrl_start5_roistartx);
int iSetVCTRL_ROI_START_5vctrl_start5_roistarty(unsigned int uvctrl_start5_roistarty);
int iSetVCTRL_ROI_START_6vctrl_start6_roistartx(unsigned int uvctrl_start6_roistartx);
int iSetVCTRL_ROI_START_6vctrl_start6_roistarty(unsigned int uvctrl_start6_roistarty);
int iSetVCTRL_ROI_START_7vctrl_start7_roistartx(unsigned int uvctrl_start7_roistartx);
int iSetVCTRL_ROI_START_7vctrl_start7_roistarty(unsigned int uvctrl_start7_roistarty);
int iSetVCTRL_LCU_TARGET_BITvctrl_lcu_target_bit(unsigned int uvctrl_lcu_target_bit);
int iSetVCTRL_NARROW_THRESHOLDvctrl_narrow_tile_width(unsigned int uvctrl_narrow_tile_width);
int iSetVCTRL_LCU_BASELINEvctrl_lcu_performance_baseline(unsigned int uvctrl_lcu_performance_baseline);
int iSetVCTRL_NORM_TR32X32_COEFF_DENOISEvctrl_norm32_coeff_protect_num(unsigned int uvctrl_norm32_coeff_protect_num);
int iSetVCTRL_NORM_TR32X32_COEFF_DENOISEvctrl_norm32_tr1_denois_max_num(unsigned int uvctrl_norm32_tr1_denois_max_num);
int iSetVCTRL_NORM_TR16X16_COEFF_DENOISEvctrl_norm16_coeff_protect_num(unsigned int uvctrl_norm16_coeff_protect_num);
int iSetVCTRL_NORM_TR16X16_COEFF_DENOISEvctrl_norm16_tr1_denois_max_num(unsigned int uvctrl_norm16_tr1_denois_max_num);
int iSetVCTRL_SKIN_TR32X32_COEFF_DENOISEvctrl_skin32_coeff_protect_num(unsigned int uvctrl_skin32_coeff_protect_num);
int iSetVCTRL_SKIN_TR32X32_COEFF_DENOISEvctrl_skin32_tr1_denois_max_num(unsigned int uvctrl_skin32_tr1_denois_max_num);
int iSetVCTRL_SKIN_TR16X16_COEFF_DENOISEvctrl_skin16_coeff_protect_num(unsigned int uvctrl_skin16_coeff_protect_num);
int iSetVCTRL_SKIN_TR16X16_COEFF_DENOISEvctrl_skin16_tr1_denois_max_num(unsigned int uvctrl_skin16_tr1_denois_max_num);
int iSetVCTRL_STATIC_TR32X32_COEFF_DENOISEvctrl_static32_coeff_protect_num(unsigned int uvctrl_static32_coeff_protect_num);
int iSetVCTRL_STATIC_TR32X32_COEFF_DENOISEvctrl_static32_tr1_denois_max_num(unsigned int uvctrl_static32_tr1_denois_max_num);
int iSetVCTRL_STATIC_TR16X16_COEFF_DENOISEvctrl_static16_coeff_protect_num(unsigned int uvctrl_static16_coeff_protect_num);
int iSetVCTRL_STATIC_TR16X16_COEFF_DENOISEvctrl_static16_tr1_denois_max_num(unsigned int uvctrl_static16_tr1_denois_max_num);
int iSetVCTRL_HEDGE_TR32X32_COEFF_DENOISEvctrl_hedge32_coeff_protect_num(unsigned int uvctrl_hedge32_coeff_protect_num);
int iSetVCTRL_HEDGE_TR32X32_COEFF_DENOISEvctrl_hedge32_tr1_denois_max_num(unsigned int uvctrl_hedge32_tr1_denois_max_num);
int iSetVCTRL_HEDGE_TR16X16_COEFF_DENOISEvctrl_hedge16_coeff_protect_num(unsigned int uvctrl_hedge16_coeff_protect_num);
int iSetVCTRL_HEDGE_TR16X16_COEFF_DENOISEvctrl_hedge16_tr1_denois_max_num(unsigned int uvctrl_hedge16_tr1_denois_max_num);
int iSetVCTRL_NORMAL_INTRA_RDO_FACTORvctrl_norm_intra_cu4_rdcost_offset(unsigned int uvctrl_norm_intra_cu4_rdcost_offset);
int iSetVCTRL_NORMAL_INTRA_RDO_FACTORvctrl_norm_intra_cu8_rdcost_offset(unsigned int uvctrl_norm_intra_cu8_rdcost_offset);
int iSetVCTRL_NORMAL_INTRA_RDO_FACTORvctrl_norm_intra_cu16_rdcost_offset(unsigned int uvctrl_norm_intra_cu16_rdcost_offset);
int iSetVCTRL_NORMAL_INTRA_RDO_FACTORvctrl_norm_intra_cu32_rdcost_offset(unsigned int uvctrl_norm_intra_cu32_rdcost_offset);
int iSetVCTRL_NORMAL_INTRA_RDO_FACTORvctrl_strmov_intra_cu4_rdcost_offset(unsigned int uvctrl_strmov_intra_cu4_rdcost_offset);
int iSetVCTRL_NORMAL_INTRA_RDO_FACTORvctrl_strmov_intra_cu8_rdcost_offset(unsigned int uvctrl_strmov_intra_cu8_rdcost_offset);
int iSetVCTRL_NORMAL_INTRA_RDO_FACTORvctrl_strmov_intra_cu16_rdcost_offset(unsigned int uvctrl_strmov_intra_cu16_rdcost_offset);
int iSetVCTRL_NORMAL_INTRA_RDO_FACTORvctrl_strmov_intra_cu32_rdcost_offset(unsigned int uvctrl_strmov_intra_cu32_rdcost_offset);
int iSetVCTRL_NORMAL_MRG_RDO_FACTORvctrl_norm_mrg_cu8_rdcost_offset(unsigned int uvctrl_norm_mrg_cu8_rdcost_offset);
int iSetVCTRL_NORMAL_MRG_RDO_FACTORvctrl_norm_mrg_cu16_rdcost_offset(unsigned int uvctrl_norm_mrg_cu16_rdcost_offset);
int iSetVCTRL_NORMAL_MRG_RDO_FACTORvctrl_norm_mrg_cu32_rdcost_offset(unsigned int uvctrl_norm_mrg_cu32_rdcost_offset);
int iSetVCTRL_NORMAL_MRG_RDO_FACTORvctrl_norm_mrg_cu64_rdcost_offset(unsigned int uvctrl_norm_mrg_cu64_rdcost_offset);
int iSetVCTRL_NORMAL_MRG_RDO_FACTORvctrl_strmov_mrg_cu8_rdcost_offset(unsigned int uvctrl_strmov_mrg_cu8_rdcost_offset);
int iSetVCTRL_NORMAL_MRG_RDO_FACTORvctrl_strmov_mrg_cu16_rdcost_offset(unsigned int uvctrl_strmov_mrg_cu16_rdcost_offset);
int iSetVCTRL_NORMAL_MRG_RDO_FACTORvctrl_strmov_mrg_cu32_rdcost_offset(unsigned int uvctrl_strmov_mrg_cu32_rdcost_offset);
int iSetVCTRL_NORMAL_MRG_RDO_FACTORvctrl_strmov_mrg_cu64_rdcost_offset(unsigned int uvctrl_strmov_mrg_cu64_rdcost_offset);
int iSetVCTRL_NORMAL_FME_RDO_FACTORvctrl_norm_fme_cu8_rdcost_offset(unsigned int uvctrl_norm_fme_cu8_rdcost_offset);
int iSetVCTRL_NORMAL_FME_RDO_FACTORvctrl_norm_fme_cu16_rdcost_offset(unsigned int uvctrl_norm_fme_cu16_rdcost_offset);
int iSetVCTRL_NORMAL_FME_RDO_FACTORvctrl_norm_fme_cu32_rdcost_offset(unsigned int uvctrl_norm_fme_cu32_rdcost_offset);
int iSetVCTRL_NORMAL_FME_RDO_FACTORvctrl_norm_fme_cu64_rdcost_offset(unsigned int uvctrl_norm_fme_cu64_rdcost_offset);
int iSetVCTRL_NORMAL_FME_RDO_FACTORvctrl_strmov_fme_cu8_rdcost_offset(unsigned int uvctrl_strmov_fme_cu8_rdcost_offset);
int iSetVCTRL_NORMAL_FME_RDO_FACTORvctrl_strmov_fme_cu16_rdcost_offset(unsigned int uvctrl_strmov_fme_cu16_rdcost_offset);
int iSetVCTRL_NORMAL_FME_RDO_FACTORvctrl_strmov_fme_cu32_rdcost_offset(unsigned int uvctrl_strmov_fme_cu32_rdcost_offset);
int iSetVCTRL_NORMAL_FME_RDO_FACTORvctrl_strmov_fme_cu64_rdcost_offset(unsigned int uvctrl_strmov_fme_cu64_rdcost_offset);
int iSetVCTRL_SKIN_REGION_INTRA_RDO_FACTORvctrl_skin_intra_cu4_rdcost_offset(unsigned int uvctrl_skin_intra_cu4_rdcost_offset);
int iSetVCTRL_SKIN_REGION_INTRA_RDO_FACTORvctrl_skin_intra_cu8_rdcost_offset(unsigned int uvctrl_skin_intra_cu8_rdcost_offset);
int iSetVCTRL_SKIN_REGION_INTRA_RDO_FACTORvctrl_skin_intra_cu16_rdcost_offset(unsigned int uvctrl_skin_intra_cu16_rdcost_offset);
int iSetVCTRL_SKIN_REGION_INTRA_RDO_FACTORvctrl_skin_intra_cu32_rdcost_offset(unsigned int uvctrl_skin_intra_cu32_rdcost_offset);
int iSetVCTRL_SKIN_REGION_INTRA_RDO_FACTORvctrl_sobel_str_intra_cu4_rdcost_offset(unsigned int uvctrl_sobel_str_intra_cu4_rdcost_offset);
int iSetVCTRL_SKIN_REGION_INTRA_RDO_FACTORvctrl_sobel_str_intra_cu8_rdcost_offset(unsigned int uvctrl_sobel_str_intra_cu8_rdcost_offset);
int iSetVCTRL_SKIN_REGION_INTRA_RDO_FACTORvctrl_sobel_str_intra_cu16_rdcost_offset(unsigned int uvctrl_sobel_str_intra_cu16_rdcost_offset);
int iSetVCTRL_SKIN_REGION_INTRA_RDO_FACTORvctrl_sobel_str_intra_cu32_rdcost_offset(unsigned int uvctrl_sobel_str_intra_cu32_rdcost_offset);
int iSetVCTRL_SKIN_REGION_MRG_RDO_FACTORvctrl_skin_mrg_cu8_rdcost_offset(unsigned int uvctrl_skin_mrg_cu8_rdcost_offset);
int iSetVCTRL_SKIN_REGION_MRG_RDO_FACTORvctrl_skin_mrg_cu16_rdcost_offset(unsigned int uvctrl_skin_mrg_cu16_rdcost_offset);
int iSetVCTRL_SKIN_REGION_MRG_RDO_FACTORvctrl_skin_mrg_cu32_rdcost_offset(unsigned int uvctrl_skin_mrg_cu32_rdcost_offset);
int iSetVCTRL_SKIN_REGION_MRG_RDO_FACTORvctrl_skin_mrg_cu64_rdcost_offset(unsigned int uvctrl_skin_mrg_cu64_rdcost_offset);
int iSetVCTRL_SKIN_REGION_MRG_RDO_FACTORvctrl_sobel_str_mrg_cu8_rdcost_offset(unsigned int uvctrl_sobel_str_mrg_cu8_rdcost_offset);
int iSetVCTRL_SKIN_REGION_MRG_RDO_FACTORvctrl_sobel_str_mrg_cu16_rdcost_offset(unsigned int uvctrl_sobel_str_mrg_cu16_rdcost_offset);
int iSetVCTRL_SKIN_REGION_MRG_RDO_FACTORvctrl_sobel_str_mrg_cu32_rdcost_offset(unsigned int uvctrl_sobel_str_mrg_cu32_rdcost_offset);
int iSetVCTRL_SKIN_REGION_MRG_RDO_FACTORvctrl_sobel_str_mrg_cu64_rdcost_offset(unsigned int uvctrl_sobel_str_mrg_cu64_rdcost_offset);
int iSetVCTRL_SKIN_REGION_FME_RDO_FACTORvctrl_skin_fme_cu8_rdcost_offset(unsigned int uvctrl_skin_fme_cu8_rdcost_offset);
int iSetVCTRL_SKIN_REGION_FME_RDO_FACTORvctrl_skin_fme_cu16_rdcost_offset(unsigned int uvctrl_skin_fme_cu16_rdcost_offset);
int iSetVCTRL_SKIN_REGION_FME_RDO_FACTORvctrl_skin_fme_cu32_rdcost_offset(unsigned int uvctrl_skin_fme_cu32_rdcost_offset);
int iSetVCTRL_SKIN_REGION_FME_RDO_FACTORvctrl_skin_fme_cu64_rdcost_offset(unsigned int uvctrl_skin_fme_cu64_rdcost_offset);
int iSetVCTRL_SKIN_REGION_FME_RDO_FACTORvctrl_sobel_str_fme_cu8_rdcost_offset(unsigned int uvctrl_sobel_str_fme_cu8_rdcost_offset);
int iSetVCTRL_SKIN_REGION_FME_RDO_FACTORvctrl_sobel_str_fme_cu16_rdcost_offset(unsigned int uvctrl_sobel_str_fme_cu16_rdcost_offset);
int iSetVCTRL_SKIN_REGION_FME_RDO_FACTORvctrl_sobel_str_fme_cu32_rdcost_offset(unsigned int uvctrl_sobel_str_fme_cu32_rdcost_offset);
int iSetVCTRL_SKIN_REGION_FME_RDO_FACTORvctrl_sobel_str_fme_cu64_rdcost_offset(unsigned int uvctrl_sobel_str_fme_cu64_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_INTRA_RDO_FACTORvctrl_hedge_intra_cu4_rdcost_offset(unsigned int uvctrl_hedge_intra_cu4_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_INTRA_RDO_FACTORvctrl_hedge_intra_cu8_rdcost_offset(unsigned int uvctrl_hedge_intra_cu8_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_INTRA_RDO_FACTORvctrl_hedge_intra_cu16_rdcost_offset(unsigned int uvctrl_hedge_intra_cu16_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_INTRA_RDO_FACTORvctrl_hedge_intra_cu32_rdcost_offset(unsigned int uvctrl_hedge_intra_cu32_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_INTRA_RDO_FACTORvctrl_sobel_tex_intra_cu4_rdcost_offset(unsigned int uvctrl_sobel_tex_intra_cu4_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_INTRA_RDO_FACTORvctrl_sobel_tex_intra_cu8_rdcost_offset(unsigned int uvctrl_sobel_tex_intra_cu8_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_INTRA_RDO_FACTORvctrl_sobel_tex_intra_cu16_rdcost_offset(unsigned int uvctrl_sobel_tex_intra_cu16_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_INTRA_RDO_FACTORvctrl_sobel_tex_intra_cu32_rdcost_offset(unsigned int uvctrl_sobel_tex_intra_cu32_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_MRG_RDO_FACTORvctrl_hedge_mrg_cu8_rdcost_offset(unsigned int uvctrl_hedge_mrg_cu8_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_MRG_RDO_FACTORvctrl_hedge_mrg_cu16_rdcost_offset(unsigned int uvctrl_hedge_mrg_cu16_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_MRG_RDO_FACTORvctrl_hedge_mrg_cu32_rdcost_offset(unsigned int uvctrl_hedge_mrg_cu32_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_MRG_RDO_FACTORvctrl_hedge_mrg_cu64_rdcost_offset(unsigned int uvctrl_hedge_mrg_cu64_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_MRG_RDO_FACTORvctrl_sobel_tex_mrg_cu8_rdcost_offset(unsigned int uvctrl_sobel_tex_mrg_cu8_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_MRG_RDO_FACTORvctrl_sobel_tex_mrg_cu16_rdcost_offset(unsigned int uvctrl_sobel_tex_mrg_cu16_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_MRG_RDO_FACTORvctrl_sobel_tex_mrg_cu32_rdcost_offset(unsigned int uvctrl_sobel_tex_mrg_cu32_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_MRG_RDO_FACTORvctrl_sobel_tex_mrg_cu64_rdcost_offset(unsigned int uvctrl_sobel_tex_mrg_cu64_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_FME_RDO_FACTORvctrl_hedge_fme_cu8_rdcost_offset(unsigned int uvctrl_hedge_fme_cu8_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_FME_RDO_FACTORvctrl_hedge_fme_cu16_rdcost_offset(unsigned int uvctrl_hedge_fme_cu16_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_FME_RDO_FACTORvctrl_hedge_fme_cu32_rdcost_offset(unsigned int uvctrl_hedge_fme_cu32_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_FME_RDO_FACTORvctrl_hedge_fme_cu64_rdcost_offset(unsigned int uvctrl_hedge_fme_cu64_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_FME_RDO_FACTORvctrl_sobel_tex_fme_cu8_rdcost_offset(unsigned int uvctrl_sobel_tex_fme_cu8_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_FME_RDO_FACTORvctrl_sobel_tex_fme_cu16_rdcost_offset(unsigned int uvctrl_sobel_tex_fme_cu16_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_FME_RDO_FACTORvctrl_sobel_tex_fme_cu32_rdcost_offset(unsigned int uvctrl_sobel_tex_fme_cu32_rdcost_offset);
int iSetVCTRL_HIGH_EDGE_FME_RDO_FACTORvctrl_sobel_tex_fme_cu64_rdcost_offset(unsigned int uvctrl_sobel_tex_fme_cu64_rdcost_offset);
int iSetVCTRL_EDGE_MOV_COEFF_DENOISEvctrl_roundingedgemovmechanism(unsigned int uvctrl_roundingedgemovmechanism);
int iSetVCTRL_EDGE_MOV_COEFF_DENOISEvctrl_roundingedgemovdegreethresh(unsigned int uvctrl_roundingedgemovdegreethresh);
int iSetVCTRL_EDGE_MOV_COEFF_DENOISEvctrl_roundingedgemovforcezeroresidthresh(unsigned int uvctrl_roundingedgemovforcezeroresidthresh);
int iSetVCTRL_EDGE_MOV_COEFF_DENOISEvctrl_roundingedgemovac32sum(unsigned int uvctrl_roundingedgemovac32sum);
int iSetVCTRL_EDGE_MOV_COEFF_DENOISEvctrl_roundingedgemovac16sum(unsigned int uvctrl_roundingedgemovac16sum);
int iSetVCTRL_EDGE_MOV_COEFF_DENOISEvctrl_roundingedgemovlowfreqacblk32(unsigned int uvctrl_roundingedgemovlowfreqacblk32);
int iSetVCTRL_EDGE_MOV_COEFF_DENOISEvctrl_roundingedgemovlowfreqacblk16(unsigned int uvctrl_roundingedgemovlowfreqacblk16);
int iSetVCTRL_SOBEL_STRONG_COEFF_DENOISEvctrl_roundingsobelstrmechanism(unsigned int uvctrl_roundingsobelstrmechanism);
int iSetVCTRL_SOBEL_STRONG_COEFF_DENOISEvctrl_roundingsobelstrdegreethresh(unsigned int uvctrl_roundingsobelstrdegreethresh);
int iSetVCTRL_SOBEL_STRONG_COEFF_DENOISEvctrl_roundingsobelstrforcezeroresidthresh(unsigned int uvctrl_roundingsobelstrforcezeroresidthresh);
int iSetVCTRL_SOBEL_STRONG_COEFF_DENOISEvctrl_roundingsobelstrac32sum(unsigned int uvctrl_roundingsobelstrac32sum);
int iSetVCTRL_SOBEL_STRONG_COEFF_DENOISEvctrl_roundingsobelstrac16sum(unsigned int uvctrl_roundingsobelstrac16sum);
int iSetVCTRL_SOBEL_STRONG_COEFF_DENOISEvctrl_roundingsobelstrlowfreqacblk32(unsigned int uvctrl_roundingsobelstrlowfreqacblk32);
int iSetVCTRL_SOBEL_STRONG_COEFF_DENOISEvctrl_roundingsobelstrlowfreqacblk16(unsigned int uvctrl_roundingsobelstrlowfreqacblk16);
int iSetVCTRL_SOBEL_WEAK_COEFF_DENOISEvctrl_roundingsobelweakmechanism(unsigned int uvctrl_roundingsobelweakmechanism);
int iSetVCTRL_SOBEL_WEAK_COEFF_DENOISEvctrl_roundingsobelweakdegreethresh(unsigned int uvctrl_roundingsobelweakdegreethresh);
int iSetVCTRL_SOBEL_WEAK_COEFF_DENOISEvctrl_roundingsobelweakforcezeroresidthresh(unsigned int uvctrl_roundingsobelweakforcezeroresidthresh);
int iSetVCTRL_SOBEL_WEAK_COEFF_DENOISEvctrl_roundingsobelweakac32sum(unsigned int uvctrl_roundingsobelweakac32sum);
int iSetVCTRL_SOBEL_WEAK_COEFF_DENOISEvctrl_roundingsobelweakac16sum(unsigned int uvctrl_roundingsobelweakac16sum);
int iSetVCTRL_SOBEL_WEAK_COEFF_DENOISEvctrl_roundingsobelweaklowfreqacblk32(unsigned int uvctrl_roundingsobelweaklowfreqacblk32);
int iSetVCTRL_SOBEL_WEAK_COEFF_DENOISEvctrl_roundingsobelweaklowfreqacblk16(unsigned int uvctrl_roundingsobelweaklowfreqacblk16);
int iSetVCTRL_NORM_COEFF_DENOISEvctrl_roundingmechanism(unsigned int uvctrl_roundingmechanism);
int iSetVCTRL_NORM_COEFF_DENOISEvctrl_roundingdegreethresh(unsigned int uvctrl_roundingdegreethresh);
int iSetVCTRL_NORM_COEFF_DENOISEvctrl_roundingforcezeroresidthresh(unsigned int uvctrl_roundingforcezeroresidthresh);
int iSetVCTRL_NORM_COEFF_DENOISEvctrl_roundingac32sum(unsigned int uvctrl_roundingac32sum);
int iSetVCTRL_NORM_COEFF_DENOISEvctrl_roundingac16sum(unsigned int uvctrl_roundingac16sum);
int iSetVCTRL_NORM_COEFF_DENOISEvctrl_roundinglowfreqacblk32(unsigned int uvctrl_roundinglowfreqacblk32);
int iSetVCTRL_NORM_COEFF_DENOISEvctrl_roundinglowfreqacblk16(unsigned int uvctrl_roundinglowfreqacblk16);
int iSetVCTRL_SKIN_COEFF_DENOISEvctrl_roundingskinmechanism(unsigned int uvctrl_roundingskinmechanism);
int iSetVCTRL_SKIN_COEFF_DENOISEvctrl_roundingskindegreethresh(unsigned int uvctrl_roundingskindegreethresh);
int iSetVCTRL_SKIN_COEFF_DENOISEvctrl_roundingskinforcezeroresidthresh(unsigned int uvctrl_roundingskinforcezeroresidthresh);
int iSetVCTRL_SKIN_COEFF_DENOISEvctrl_roundingskinac32sum(unsigned int uvctrl_roundingskinac32sum);
int iSetVCTRL_SKIN_COEFF_DENOISEvctrl_roundingskinac16sum(unsigned int uvctrl_roundingskinac16sum);
int iSetVCTRL_SKIN_COEFF_DENOISEvctrl_roundingskinlowfreqacblk32(unsigned int uvctrl_roundingskinlowfreqacblk32);
int iSetVCTRL_SKIN_COEFF_DENOISEvctrl_roundingskinlowfreqacblk16(unsigned int uvctrl_roundingskinlowfreqacblk16);
int iSetVCTRL_STILL_COEFF_DENOISEvctrl_roundingstillmechanism(unsigned int uvctrl_roundingstillmechanism);
int iSetVCTRL_STILL_COEFF_DENOISEvctrl_roundingstilldegreethresh(unsigned int uvctrl_roundingstilldegreethresh);
int iSetVCTRL_STILL_COEFF_DENOISEvctrl_roundingstillforcezeroresidthresh(unsigned int uvctrl_roundingstillforcezeroresidthresh);
int iSetVCTRL_STILL_COEFF_DENOISEvctrl_roundingstillac32sum(unsigned int uvctrl_roundingstillac32sum);
int iSetVCTRL_STILL_COEFF_DENOISEvctrl_roundingstillac16sum(unsigned int uvctrl_roundingstillac16sum);
int iSetVCTRL_STILL_COEFF_DENOISEvctrl_roundingstilllowfreqacblk32(unsigned int uvctrl_roundingstilllowfreqacblk32);
int iSetVCTRL_STILL_COEFF_DENOISEvctrl_roundingstilllowfreqacblk16(unsigned int uvctrl_roundingstilllowfreqacblk16);
int iSetVCTRL_EDGE_COEFF_DENOISEvctrl_roundingedgemechanism(unsigned int uvctrl_roundingedgemechanism);
int iSetVCTRL_EDGE_COEFF_DENOISEvctrl_roundingedgedegreethresh(unsigned int uvctrl_roundingedgedegreethresh);
int iSetVCTRL_EDGE_COEFF_DENOISEvctrl_roundingedgeforcezeroresidthresh(unsigned int uvctrl_roundingedgeforcezeroresidthresh);
int iSetVCTRL_EDGE_COEFF_DENOISEvctrl_roundingedgeac32sum(unsigned int uvctrl_roundingedgeac32sum);
int iSetVCTRL_EDGE_COEFF_DENOISEvctrl_roundingedgeac16sum(unsigned int uvctrl_roundingedgeac16sum);
int iSetVCTRL_EDGE_COEFF_DENOISEvctrl_roundingedgelowfreqacblk32(unsigned int uvctrl_roundingedgelowfreqacblk32);
int iSetVCTRL_EDGE_COEFF_DENOISEvctrl_roundingedgelowfreqacblk16(unsigned int uvctrl_roundingedgelowfreqacblk16);
int iSetVCTRL_OFFSET_COEFF_DENOISEvctrl_roundingoffset16x16(unsigned int uvctrl_roundingoffset16x16);
int iSetVCTRL_OFFSET_COEFF_DENOISEvctrl_roundingoffset32x32(unsigned int uvctrl_roundingoffset32x32);
int iSetVCTRL_OFFSET_COEFF_DENOISEvctrl_roundingskinoffset16x16(unsigned int uvctrl_roundingskinoffset16x16);
int iSetVCTRL_OFFSET_COEFF_DENOISEvctrl_roundingskinoffset32x32(unsigned int uvctrl_roundingskinoffset32x32);
int iSetVCTRL_OFFSET_COEFF_DENOISEvctrl_roundingstilloffset16x16(unsigned int uvctrl_roundingstilloffset16x16);
int iSetVCTRL_OFFSET_COEFF_DENOISEvctrl_roundingstilloffset32x32(unsigned int uvctrl_roundingstilloffset32x32);
int iSetVCTRL_OFFSET_COEFF_DENOISEvctrl_roundingedgeoffset16x16(unsigned int uvctrl_roundingedgeoffset16x16);
int iSetVCTRL_OFFSET_COEFF_DENOISEvctrl_roundingedgeoffset32x32(unsigned int uvctrl_roundingedgeoffset32x32);
int iSetVCTRL_NEW_OFFSET_COEFF_DENOISEvctrl_roundingedgemovoffset16x16(unsigned int uvctrl_roundingedgemovoffset16x16);
int iSetVCTRL_NEW_OFFSET_COEFF_DENOISEvctrl_roundingedgemovoffset32x32(unsigned int uvctrl_roundingedgemovoffset32x32);
int iSetVCTRL_NEW_OFFSET_COEFF_DENOISEvctrl_roundingsobelstroffset16x16(unsigned int uvctrl_roundingsobelstroffset16x16);
int iSetVCTRL_NEW_OFFSET_COEFF_DENOISEvctrl_roundingsobelstroffset32x32(unsigned int uvctrl_roundingsobelstroffset32x32);
int iSetVCTRL_NEW_OFFSET_COEFF_DENOISEvctrl_roundingsobelweakoffset16x16(unsigned int uvctrl_roundingsobelweakoffset16x16);
int iSetVCTRL_NEW_OFFSET_COEFF_DENOISEvctrl_roundingsobelweakoffset32x32(unsigned int uvctrl_roundingsobelweakoffset32x32);
int iSetVCTRL_EDGEMOV_TR32X32_COEFF_DENOISEvctrl_edgemov32_coeff_protect_num(unsigned int uvctrl_edgemov32_coeff_protect_num);
int iSetVCTRL_EDGEMOV_TR32X32_COEFF_DENOISEvctrl_edgemov32_tr1_denois_max_num(unsigned int uvctrl_edgemov32_tr1_denois_max_num);
int iSetVCTRL_EDGEMOV_TR16X16_COEFF_DENOISEvctrl_edgemov16_coeff_protect_num(unsigned int uvctrl_edgemov16_coeff_protect_num);
int iSetVCTRL_EDGEMOV_TR16X16_COEFF_DENOISEvctrl_edgemov16_tr1_denois_max_num(unsigned int uvctrl_edgemov16_tr1_denois_max_num);
int iSetVCTRL_SOBELSTR_TR32X32_COEFF_DENOISEvctrl_sobelstr32_coeff_protect_num(unsigned int uvctrl_sobelstr32_coeff_protect_num);
int iSetVCTRL_SOBELSTR_TR32X32_COEFF_DENOISEvctrl_sobelstr32_tr1_denois_max_num(unsigned int uvctrl_sobelstr32_tr1_denois_max_num);
int iSetVCTRL_SOBELSTR_TR16X16_COEFF_DENOISEvctrl_sobelstr16_coeff_protect_num(unsigned int uvctrl_sobelstr16_coeff_protect_num);
int iSetVCTRL_SOBELSTR_TR16X16_COEFF_DENOISEvctrl_sobelstr16_tr1_denois_max_num(unsigned int uvctrl_sobelstr16_tr1_denois_max_num);
int iSetVCTRL_SOBELWEAK_TR32X32_COEFF_DENOISEvctrl_sobelweak32_coeff_protect_num(unsigned int uvctrl_sobelweak32_coeff_protect_num);
int iSetVCTRL_SOBELWEAK_TR32X32_COEFF_DENOISEvctrl_sobelweak32_tr1_denois_max_num(unsigned int uvctrl_sobelweak32_tr1_denois_max_num);
int iSetVCTRL_SOBELWEAK_TR16X16_COEFF_DENOISEvctrl_sobelweak16_coeff_protect_num(unsigned int uvctrl_sobelweak16_coeff_protect_num);
int iSetVCTRL_SOBELWEAK_TR16X16_COEFF_DENOISEvctrl_sobelweak16_tr1_denois_max_num(unsigned int uvctrl_sobelweak16_tr1_denois_max_num);
int iSetVCTRL_SEL_INTRA_RDO_FACTORvctrl_norm_sel_cu8_rd_offset(unsigned int uvctrl_norm_sel_cu8_rd_offset);
int iSetVCTRL_SEL_INTRA_RDO_FACTORvctrl_skin_sel_cu8_rd_offset(unsigned int uvctrl_skin_sel_cu8_rd_offset);
int iSetVCTRL_SEL_INTRA_RDO_FACTORvctrl_hedge_sel_cu8_rd_offset(unsigned int uvctrl_hedge_sel_cu8_rd_offset);
int iSetVCTRL_SEL_INTRA_RDO_FACTORvctrl_strmov_sel_cu8_rd_offset(unsigned int uvctrl_strmov_sel_cu8_rd_offset);
int iSetVCTRL_SEL_INTRA_RDO_FACTORvctrl_sobelstr_sel_cu8_rd_offset(unsigned int uvctrl_sobelstr_sel_cu8_rd_offset);
int iSetVCTRL_SEL_INTRA_RDO_FACTORvctrl_sobeltex_sel_cu8_rd_offset(unsigned int uvctrl_sobeltex_sel_cu8_rd_offset);
int iSetVCTRL_SEL_INTER_RDO_FACTOR_0vctrl_skin_inter_cu_rdcost_offset(unsigned int uvctrl_skin_inter_cu_rdcost_offset);
int iSetVCTRL_SEL_INTER_RDO_FACTOR_0vctrl_hedge_inter_cu_rdcost_offset(unsigned int uvctrl_hedge_inter_cu_rdcost_offset);
int iSetVCTRL_SEL_INTER_RDO_FACTOR_0vctrl_skin_layer_rdcost_offset(unsigned int uvctrl_skin_layer_rdcost_offset);
int iSetVCTRL_SEL_INTER_RDO_FACTOR_1vctrl_sobelstr_inter_cu_rdcost_offset(unsigned int uvctrl_sobelstr_inter_cu_rdcost_offset);
int iSetVCTRL_SEL_INTER_RDO_FACTOR_1vctrl_sobeltex_inter_cu_rdcost_offset(unsigned int uvctrl_sobeltex_inter_cu_rdcost_offset);
int iSetVCTRL_SEL_INTER_RDO_FACTOR_1vctrl_hedge_layer_rdcost_offset(unsigned int uvctrl_hedge_layer_rdcost_offset);
int iSetVCTRL_SEL_INTER_RDO_FACTOR_2vctrl_strmov_inter_cu_rdcost_offset(unsigned int uvctrl_strmov_inter_cu_rdcost_offset);
int iSetVCTRL_SEL_INTER_RDO_FACTOR_2vctrl_norm_inter_cu_rdcost_offset(unsigned int uvctrl_norm_inter_cu_rdcost_offset);
int iSetVCTRL_SEL_INTER_RDO_FACTOR_2vctrl_strmov_layer_rdcost_offset(unsigned int uvctrl_strmov_layer_rdcost_offset);
int iSetVCTRL_SEL_LAYER_RDO_FACTORvctrl_sobelstr_layer_rdcost_offset(unsigned int uvctrl_sobelstr_layer_rdcost_offset);
int iSetVCTRL_SEL_LAYER_RDO_FACTORvctrl_sobeltex_layer_rdcost_offset(unsigned int uvctrl_sobeltex_layer_rdcost_offset);
int iSetVCTRL_SEL_LAYER_RDO_FACTORvctrl_norm_layer_rdcost_offset(unsigned int uvctrl_norm_layer_rdcost_offset);
int iSetVCTRL_NORM_ENG_DENOISEvctrl_norm_isolate_ac_enable(unsigned int uvctrl_norm_isolate_ac_enable);
int iSetVCTRL_NORM_ENG_DENOISEvctrl_norm_force_zero_cnt(unsigned int uvctrl_norm_force_zero_cnt);
int iSetVCTRL_NORM_ENG_DENOISEvctrl_norm_engsum_32(unsigned int uvctrl_norm_engsum_32);
int iSetVCTRL_NORM_ENG_DENOISEvctrl_norm_engcnt_32(unsigned int uvctrl_norm_engcnt_32);
int iSetVCTRL_NORM_ENG_DENOISEvctrl_norm_engsum_16(unsigned int uvctrl_norm_engsum_16);
int iSetVCTRL_NORM_ENG_DENOISEvctrl_norm_engcnt_16(unsigned int uvctrl_norm_engcnt_16);
int iSetVCTRL_SKIN_ENG_DENOISEvctrl_skin_isolate_ac_enable(unsigned int uvctrl_skin_isolate_ac_enable);
int iSetVCTRL_SKIN_ENG_DENOISEvctrl_skin_force_zero_cnt(unsigned int uvctrl_skin_force_zero_cnt);
int iSetVCTRL_SKIN_ENG_DENOISEvctrl_skin_engsum_32(unsigned int uvctrl_skin_engsum_32);
int iSetVCTRL_SKIN_ENG_DENOISEvctrl_skin_engcnt_32(unsigned int uvctrl_skin_engcnt_32);
int iSetVCTRL_SKIN_ENG_DENOISEvctrl_skin_engsum_16(unsigned int uvctrl_skin_engsum_16);
int iSetVCTRL_SKIN_ENG_DENOISEvctrl_skin_engcnt_16(unsigned int uvctrl_skin_engcnt_16);
int iSetVCTRL_STILL_ENG_DENOISEvctrl_still_isolate_ac_enable(unsigned int uvctrl_still_isolate_ac_enable);
int iSetVCTRL_STILL_ENG_DENOISEvctrl_still_force_zero_cnt(unsigned int uvctrl_still_force_zero_cnt);
int iSetVCTRL_STILL_ENG_DENOISEvctrl_still_engsum_32(unsigned int uvctrl_still_engsum_32);
int iSetVCTRL_STILL_ENG_DENOISEvctrl_still_engcnt_32(unsigned int uvctrl_still_engcnt_32);
int iSetVCTRL_STILL_ENG_DENOISEvctrl_still_engsum_16(unsigned int uvctrl_still_engsum_16);
int iSetVCTRL_STILL_ENG_DENOISEvctrl_still_engcnt_16(unsigned int uvctrl_still_engcnt_16);
int iSetVCTRL_STREDGE_ENG_DENOISEvctrl_stredge_isolate_ac_enable(unsigned int uvctrl_stredge_isolate_ac_enable);
int iSetVCTRL_STREDGE_ENG_DENOISEvctrl_stredge_force_zero_cnt(unsigned int uvctrl_stredge_force_zero_cnt);
int iSetVCTRL_STREDGE_ENG_DENOISEvctrl_stredge_engsum_32(unsigned int uvctrl_stredge_engsum_32);
int iSetVCTRL_STREDGE_ENG_DENOISEvctrl_stredge_engcnt_32(unsigned int uvctrl_stredge_engcnt_32);
int iSetVCTRL_STREDGE_ENG_DENOISEvctrl_stredge_engsum_16(unsigned int uvctrl_stredge_engsum_16);
int iSetVCTRL_STREDGE_ENG_DENOISEvctrl_stredge_engcnt_16(unsigned int uvctrl_stredge_engcnt_16);
int iSetVCTRL_EDGEMOV_ENG_DENOISEvctrl_edgemov_isolate_ac_enable(unsigned int uvctrl_edgemov_isolate_ac_enable);
int iSetVCTRL_EDGEMOV_ENG_DENOISEvctrl_edgemov_force_zero_cnt(unsigned int uvctrl_edgemov_force_zero_cnt);
int iSetVCTRL_EDGEMOV_ENG_DENOISEvctrl_edgemov_engsum_32(unsigned int uvctrl_edgemov_engsum_32);
int iSetVCTRL_EDGEMOV_ENG_DENOISEvctrl_edgemov_engcnt_32(unsigned int uvctrl_edgemov_engcnt_32);
int iSetVCTRL_EDGEMOV_ENG_DENOISEvctrl_edgemov_engsum_16(unsigned int uvctrl_edgemov_engsum_16);
int iSetVCTRL_EDGEMOV_ENG_DENOISEvctrl_edgemov_engcnt_16(unsigned int uvctrl_edgemov_engcnt_16);
int iSetVCTRL_SOBELSTR_ENG_DENOISEvctrl_sobelstr_isolate_ac_enable(unsigned int uvctrl_sobelstr_isolate_ac_enable);
int iSetVCTRL_SOBELSTR_ENG_DENOISEvctrl_sobelstr_force_zero_cnt(unsigned int uvctrl_sobelstr_force_zero_cnt);
int iSetVCTRL_SOBELSTR_ENG_DENOISEvctrl_sobelstr_engsum_32(unsigned int uvctrl_sobelstr_engsum_32);
int iSetVCTRL_SOBELSTR_ENG_DENOISEvctrl_sobelstr_engcnt_32(unsigned int uvctrl_sobelstr_engcnt_32);
int iSetVCTRL_SOBELSTR_ENG_DENOISEvctrl_sobelstr_engsum_16(unsigned int uvctrl_sobelstr_engsum_16);
int iSetVCTRL_SOBELSTR_ENG_DENOISEvctrl_sobelstr_engcnt_16(unsigned int uvctrl_sobelstr_engcnt_16);
int iSetVCTRL_SOBELWK_ENG_DENOISEvctrl_sobelwk_isolate_ac_enable(unsigned int uvctrl_sobelwk_isolate_ac_enable);
int iSetVCTRL_SOBELWK_ENG_DENOISEvctrl_sobelwk_force_zero_cnt(unsigned int uvctrl_sobelwk_force_zero_cnt);
int iSetVCTRL_SOBELWK_ENG_DENOISEvctrl_sobelwk_engsum_32(unsigned int uvctrl_sobelwk_engsum_32);
int iSetVCTRL_SOBELWK_ENG_DENOISEvctrl_sobelwk_engcnt_32(unsigned int uvctrl_sobelwk_engcnt_32);
int iSetVCTRL_SOBELWK_ENG_DENOISEvctrl_sobelwk_engsum_16(unsigned int uvctrl_sobelwk_engsum_16);
int iSetVCTRL_SOBELWK_ENG_DENOISEvctrl_sobelwk_engcnt_16(unsigned int uvctrl_sobelwk_engcnt_16);



#endif /* __C_UNION_DEFINE_VCTRL_H__ */
