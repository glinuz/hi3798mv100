//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_sel.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module sel
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_SEL_H__
#define __C_UNION_DEFINE_SEL_H__

/* Define the union U_SEL_OPT_8X8_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    opt_8x8_cnt           : 19  ; /* [18..0]  */
        unsigned int    Reserved_0            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_OPT_8X8_CNT;

/* Define the union U_SEL_INTRA_OPT_8X8_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_opt_8x8_cnt     : 19  ; /* [18..0]  */
        unsigned int    Reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTRA_OPT_8X8_CNT;

/* Define the union U_SEL_INTRA_NORMAL_OPT_8X8_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_normal_opt_8x8_cnt  : 19  ; /* [18..0]  */
        unsigned int    Reserved_2            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTRA_NORMAL_OPT_8X8_CNT;

/* Define the union U_SEL_INTRA_PCM_OPT_8X8_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pcm_opt_8x8_cnt       : 19  ; /* [18..0]  */
        unsigned int    Reserved_3            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTRA_PCM_OPT_8X8_CNT;

/* Define the union U_SEL_INTER_OPT_8X8_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    inter_opt_8x8_cnt     : 19  ; /* [18..0]  */
        unsigned int    Reserved_4            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_OPT_8X8_CNT;

/* Define the union U_SEL_INTER_FME_OPT_8X8_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fme_opt_8x8_cnt       : 19  ; /* [18..0]  */
        unsigned int    Reserved_5            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_FME_OPT_8X8_CNT;

/* Define the union U_SEL_INTER_MERGE_OPT_8X8_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_opt_8x8_cnt       : 19  ; /* [18..0]  */
        unsigned int    Reserved_6            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_MERGE_OPT_8X8_CNT;

/* Define the union U_SEL_INTER_SKIP_OPT_8X8_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_skip_opt_8x8_cnt  : 19  ; /* [18..0]  */
        unsigned int    Reserved_7            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_SKIP_OPT_8X8_CNT;

/* Define the union U_SEL_OPT_16X16_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    opt_16x16_cnt         : 17  ; /* [16..0]  */
        unsigned int    Reserved_8            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_OPT_16X16_CNT;

/* Define the union U_SEL_INTRA_OPT_16X16_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_opt_16x16_cnt   : 17  ; /* [16..0]  */
        unsigned int    Reserved_9            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTRA_OPT_16X16_CNT;

/* Define the union U_SEL_OPT_4X4_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    opt_4x4_cnt           : 19  ; /* [18..0]  */
        unsigned int    Reserved_10           : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_OPT_4X4_CNT;

/* Define the union U_SEL_INTER_OPT_16X16_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    inter_opt_16x16_cnt   : 17  ; /* [16..0]  */
        unsigned int    Reserved_12           : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_OPT_16X16_CNT;

/* Define the union U_SEL_INTER_FME_OPT_16X16_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fme_opt_16x16_cnt     : 17  ; /* [16..0]  */
        unsigned int    Reserved_13           : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_FME_OPT_16X16_CNT;

/* Define the union U_SEL_INTER_MERGE_OPT_16X16_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_opt_16x16_cnt     : 17  ; /* [16..0]  */
        unsigned int    Reserved_14           : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_MERGE_OPT_16X16_CNT;

/* Define the union U_SEL_INTER_SKIP_OPT_16X16_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_skip_opt_16x16_cnt  : 17  ; /* [16..0]  */
        unsigned int    Reserved_15           : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_SKIP_OPT_16X16_CNT;

/* Define the union U_SEL_OPT_32X32_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    opt_32x32_cnt         : 16  ; /* [15..0]  */
        unsigned int    Reserved_16           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_OPT_32X32_CNT;

/* Define the union U_SEL_INTER_OPT_32X32_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    inter_opt_32x32_cnt   : 16  ; /* [15..0]  */
        unsigned int    Reserved_18           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_OPT_32X32_CNT;

/* Define the union U_SEL_INTER_FME_OPT_32X32_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fme_opt_32x32_cnt     : 16  ; /* [15..0]  */
        unsigned int    Reserved_19           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_FME_OPT_32X32_CNT;

/* Define the union U_SEL_INTER_MERGE_OPT_32X32_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_opt_32x32_cnt     : 16  ; /* [15..0]  */
        unsigned int    Reserved_20           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_MERGE_OPT_32X32_CNT;

/* Define the union U_SEL_INTER_SKIP_OPT_32X32_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_skip_opt_32x32_cnt  : 16  ; /* [15..0]  */
        unsigned int    Reserved_21           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_SKIP_OPT_32X32_CNT;

/* Define the union U_SEL_TOTAL_LUMA_QP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    total_luma_qp         : 26  ; /* [25..0]  */
        unsigned int    Reserved_23           : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_TOTAL_LUMA_QP;

/* Define the union U_SEL_MAX_MIN_LUMA_QP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    min_luma_qp           : 6   ; /* [5..0]  */
        unsigned int    Reserved_25           : 2   ; /* [7..6]  */
        unsigned int    max_luma_qp           : 6   ; /* [13..8]  */
        unsigned int    Reserved_24           : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_MAX_MIN_LUMA_QP;

/* Define the union U_SEL_VERSION */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    version               : 8   ; /* [7..0]  */
        unsigned int    Reserved_27           : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_VERSION;

/* Define the union U_SEL_INTRA_ABS_OFFSET0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sel_intra4_8_abs_offset  : 16  ; /* [15..0]  */
        unsigned int    sel_intra16_abs_offset  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTRA_ABS_OFFSET0;

/* Define the union U_SEL_INTRA_ABS_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_28           : 16  ; /* [15..0]  */
        unsigned int    sel_layer32_abs_offset  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTRA_ABS_OFFSET1;

/* Define the union U_SEL_INTER_ABS_OFFSET0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sel_inter8_16_abs_offset  : 16  ; /* [15..0]  */
        unsigned int    sel_inter32_abs_offset  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_INTER_ABS_OFFSET0;

/* Define the union U_SEL_LUMA_QP0_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp0_cnt          : 21  ; /* [20..0]  */
        unsigned int    Reserved_30           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP0_CNT;

/* Define the union U_SEL_LUMA_QP1_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp1_cnt          : 21  ; /* [20..0]  */
        unsigned int    Reserved_31           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP1_CNT;

/* Define the union U_SEL_LUMA_QP2_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp2_cnt          : 21  ; /* [20..0]  */
        unsigned int    Reserved_32           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP2_CNT;

/* Define the union U_SEL_LUMA_QP3_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp3_cnt          : 21  ; /* [20..0]  */
        unsigned int    Reserved_33           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP3_CNT;

/* Define the union U_SEL_LUMA_QP4_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp4_cnt          : 21  ; /* [20..0]  */
        unsigned int    Reserved_34           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP4_CNT;

/* Define the union U_SEL_LUMA_QP5_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp5_cnt          : 21  ; /* [20..0]  */
        unsigned int    Reserved_35           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP5_CNT;

/* Define the union U_SEL_LUMA_QP6_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp6_cnt          : 21  ; /* [20..0]  */
        unsigned int    Reserved_36           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP6_CNT;

/* Define the union U_SEL_LUMA_QP7_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp7_cnt          : 21  ; /* [20..0]  */
        unsigned int    Reserved_37           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP7_CNT;

/* Define the union U_SEL_LUMA_QP8_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp8_cnt          : 21  ; /* [20..0]  */
        unsigned int    Reserved_38           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP8_CNT;

/* Define the union U_SEL_LUMA_QP9_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp9_cnt          : 21  ; /* [20..0]  */
        unsigned int    Reserved_39           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP9_CNT;

/* Define the union U_SEL_LUMA_QP10_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp10_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_40           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP10_CNT;

/* Define the union U_SEL_LUMA_QP11_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp11_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_41           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP11_CNT;

/* Define the union U_SEL_LUMA_QP12_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp12_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_42           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP12_CNT;

/* Define the union U_SEL_LUMA_QP13_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp13_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_43           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP13_CNT;

/* Define the union U_SEL_LUMA_QP14_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp14_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_44           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP14_CNT;

/* Define the union U_SEL_LUMA_QP15_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp15_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_45           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP15_CNT;

/* Define the union U_SEL_LUMA_QP16_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp16_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_46           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP16_CNT;

/* Define the union U_SEL_LUMA_QP17_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp17_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_47           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP17_CNT;

/* Define the union U_SEL_LUMA_QP18_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp18_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_48           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP18_CNT;

/* Define the union U_SEL_LUMA_QP19_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp19_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_49           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP19_CNT;

/* Define the union U_SEL_LUMA_QP20_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp20_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_50           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP20_CNT;

/* Define the union U_SEL_LUMA_QP21_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp21_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_51           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP21_CNT;

/* Define the union U_SEL_LUMA_QP22_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp22_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_52           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP22_CNT;

/* Define the union U_SEL_LUMA_QP23_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp23_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_53           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP23_CNT;

/* Define the union U_SEL_LUMA_QP24_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp24_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_54           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP24_CNT;

/* Define the union U_SEL_LUMA_QP25_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp25_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_55           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP25_CNT;

/* Define the union U_SEL_LUMA_QP26_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp26_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_56           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP26_CNT;

/* Define the union U_SEL_LUMA_QP27_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp27_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_57           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP27_CNT;

/* Define the union U_SEL_LUMA_QP28_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp28_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_58           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP28_CNT;

/* Define the union U_SEL_LUMA_QP29_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp29_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_59           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP29_CNT;

/* Define the union U_SEL_LUMA_QP30_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp30_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_60           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP30_CNT;

/* Define the union U_SEL_LUMA_QP31_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp31_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_61           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP31_CNT;

/* Define the union U_SEL_LUMA_QP32_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp32_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_62           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP32_CNT;

/* Define the union U_SEL_LUMA_QP33_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp33_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_63           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP33_CNT;

/* Define the union U_SEL_LUMA_QP34_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp34_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_64           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP34_CNT;

/* Define the union U_SEL_LUMA_QP35_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp35_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_65           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP35_CNT;

/* Define the union U_SEL_LUMA_QP36_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp36_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_66           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP36_CNT;

/* Define the union U_SEL_LUMA_QP37_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp37_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_67           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP37_CNT;

/* Define the union U_SEL_LUMA_QP38_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp38_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_68           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP38_CNT;

/* Define the union U_SEL_LUMA_QP39_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp39_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_69           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP39_CNT;

/* Define the union U_SEL_LUMA_QP40_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp40_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_70           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP40_CNT;

/* Define the union U_SEL_LUMA_QP41_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp41_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_71           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP41_CNT;

/* Define the union U_SEL_LUMA_QP42_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp42_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_72           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP42_CNT;

/* Define the union U_SEL_LUMA_QP43_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp43_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_73           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP43_CNT;

/* Define the union U_SEL_LUMA_QP44_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp44_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_74           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP44_CNT;

/* Define the union U_SEL_LUMA_QP45_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp45_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_75           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP45_CNT;

/* Define the union U_SEL_LUMA_QP46_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp46_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_76           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP46_CNT;

/* Define the union U_SEL_LUMA_QP47_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp47_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_77           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP47_CNT;

/* Define the union U_SEL_LUMA_QP48_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp48_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_78           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP48_CNT;

/* Define the union U_SEL_LUMA_QP49_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp49_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_79           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP49_CNT;

/* Define the union U_SEL_LUMA_QP50_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp50_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_80           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP50_CNT;

/* Define the union U_SEL_LUMA_QP51_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_qp51_cnt         : 21  ; /* [20..0]  */
        unsigned int    Reserved_81           : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SEL_LUMA_QP51_CNT;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_SEL_OPT_8X8_CNT      SEL_OPT_8X8_CNT;
    volatile U_SEL_INTRA_OPT_8X8_CNT   SEL_INTRA_OPT_8X8_CNT;
    volatile U_SEL_INTRA_NORMAL_OPT_8X8_CNT   SEL_INTRA_NORMAL_OPT_8X8_CNT;
    volatile U_SEL_INTRA_PCM_OPT_8X8_CNT   SEL_INTRA_PCM_OPT_8X8_CNT;
    volatile U_SEL_INTER_OPT_8X8_CNT   SEL_INTER_OPT_8X8_CNT;
    volatile U_SEL_INTER_FME_OPT_8X8_CNT   SEL_INTER_FME_OPT_8X8_CNT;
    volatile U_SEL_INTER_MERGE_OPT_8X8_CNT   SEL_INTER_MERGE_OPT_8X8_CNT;
    volatile U_SEL_INTER_SKIP_OPT_8X8_CNT   SEL_INTER_SKIP_OPT_8X8_CNT;
    volatile U_SEL_OPT_16X16_CNT    SEL_OPT_16X16_CNT;
    volatile U_SEL_INTRA_OPT_16X16_CNT   SEL_INTRA_OPT_16X16_CNT;
    volatile U_SEL_OPT_4X4_CNT      SEL_OPT_4X4_CNT;
    volatile unsigned int           RESERVED0;
    volatile U_SEL_INTER_OPT_16X16_CNT   SEL_INTER_OPT_16X16_CNT;
    volatile U_SEL_INTER_FME_OPT_16X16_CNT   SEL_INTER_FME_OPT_16X16_CNT;
    volatile U_SEL_INTER_MERGE_OPT_16X16_CNT   SEL_INTER_MERGE_OPT_16X16_CNT;
    volatile U_SEL_INTER_SKIP_OPT_16X16_CNT   SEL_INTER_SKIP_OPT_16X16_CNT;
    volatile U_SEL_OPT_32X32_CNT    SEL_OPT_32X32_CNT;
    volatile unsigned int           RESERVED1[2];
    volatile U_SEL_INTER_OPT_32X32_CNT   SEL_INTER_OPT_32X32_CNT;
    volatile U_SEL_INTER_FME_OPT_32X32_CNT   SEL_INTER_FME_OPT_32X32_CNT;
    volatile U_SEL_INTER_MERGE_OPT_32X32_CNT   SEL_INTER_MERGE_OPT_32X32_CNT;
    volatile U_SEL_INTER_SKIP_OPT_32X32_CNT   SEL_INTER_SKIP_OPT_32X32_CNT;
    volatile unsigned int           RESERVED2[4];
    volatile U_SEL_TOTAL_LUMA_QP    SEL_TOTAL_LUMA_QP;
    volatile U_SEL_MAX_MIN_LUMA_QP   SEL_MAX_MIN_LUMA_QP;
    volatile unsigned int           RESERVED3[3];
    volatile U_SEL_VERSION          SEL_VERSION;
    volatile U_SEL_INTRA_ABS_OFFSET0   SEL_INTRA_ABS_OFFSET0;
    volatile U_SEL_INTRA_ABS_OFFSET1   SEL_INTRA_ABS_OFFSET1;
    volatile U_SEL_INTER_ABS_OFFSET0   SEL_INTER_ABS_OFFSET0;
    volatile unsigned int           RESERVED4[28];
    volatile U_SEL_LUMA_QP0_CNT     SEL_LUMA_QP0_CNT;
    volatile U_SEL_LUMA_QP1_CNT     SEL_LUMA_QP1_CNT;
    volatile U_SEL_LUMA_QP2_CNT     SEL_LUMA_QP2_CNT;
    volatile U_SEL_LUMA_QP3_CNT     SEL_LUMA_QP3_CNT;
    volatile U_SEL_LUMA_QP4_CNT     SEL_LUMA_QP4_CNT;
    volatile U_SEL_LUMA_QP5_CNT     SEL_LUMA_QP5_CNT;
    volatile U_SEL_LUMA_QP6_CNT     SEL_LUMA_QP6_CNT;
    volatile U_SEL_LUMA_QP7_CNT     SEL_LUMA_QP7_CNT;
    volatile U_SEL_LUMA_QP8_CNT     SEL_LUMA_QP8_CNT;
    volatile U_SEL_LUMA_QP9_CNT     SEL_LUMA_QP9_CNT;
    volatile U_SEL_LUMA_QP10_CNT    SEL_LUMA_QP10_CNT;
    volatile U_SEL_LUMA_QP11_CNT    SEL_LUMA_QP11_CNT;
    volatile U_SEL_LUMA_QP12_CNT    SEL_LUMA_QP12_CNT;
    volatile U_SEL_LUMA_QP13_CNT    SEL_LUMA_QP13_CNT;
    volatile U_SEL_LUMA_QP14_CNT    SEL_LUMA_QP14_CNT;
    volatile U_SEL_LUMA_QP15_CNT    SEL_LUMA_QP15_CNT;
    volatile U_SEL_LUMA_QP16_CNT    SEL_LUMA_QP16_CNT;
    volatile U_SEL_LUMA_QP17_CNT    SEL_LUMA_QP17_CNT;
    volatile U_SEL_LUMA_QP18_CNT    SEL_LUMA_QP18_CNT;
    volatile U_SEL_LUMA_QP19_CNT    SEL_LUMA_QP19_CNT;
    volatile U_SEL_LUMA_QP20_CNT    SEL_LUMA_QP20_CNT;
    volatile U_SEL_LUMA_QP21_CNT    SEL_LUMA_QP21_CNT;
    volatile U_SEL_LUMA_QP22_CNT    SEL_LUMA_QP22_CNT;
    volatile U_SEL_LUMA_QP23_CNT    SEL_LUMA_QP23_CNT;
    volatile U_SEL_LUMA_QP24_CNT    SEL_LUMA_QP24_CNT;
    volatile U_SEL_LUMA_QP25_CNT    SEL_LUMA_QP25_CNT;
    volatile U_SEL_LUMA_QP26_CNT    SEL_LUMA_QP26_CNT;
    volatile U_SEL_LUMA_QP27_CNT    SEL_LUMA_QP27_CNT;
    volatile U_SEL_LUMA_QP28_CNT    SEL_LUMA_QP28_CNT;
    volatile U_SEL_LUMA_QP29_CNT    SEL_LUMA_QP29_CNT;
    volatile U_SEL_LUMA_QP30_CNT    SEL_LUMA_QP30_CNT;
    volatile U_SEL_LUMA_QP31_CNT    SEL_LUMA_QP31_CNT;
    volatile U_SEL_LUMA_QP32_CNT    SEL_LUMA_QP32_CNT;
    volatile U_SEL_LUMA_QP33_CNT    SEL_LUMA_QP33_CNT;
    volatile U_SEL_LUMA_QP34_CNT    SEL_LUMA_QP34_CNT;
    volatile U_SEL_LUMA_QP35_CNT    SEL_LUMA_QP35_CNT;
    volatile U_SEL_LUMA_QP36_CNT    SEL_LUMA_QP36_CNT;
    volatile U_SEL_LUMA_QP37_CNT    SEL_LUMA_QP37_CNT;
    volatile U_SEL_LUMA_QP38_CNT    SEL_LUMA_QP38_CNT;
    volatile U_SEL_LUMA_QP39_CNT    SEL_LUMA_QP39_CNT;
    volatile U_SEL_LUMA_QP40_CNT    SEL_LUMA_QP40_CNT;
    volatile U_SEL_LUMA_QP41_CNT    SEL_LUMA_QP41_CNT;
    volatile U_SEL_LUMA_QP42_CNT    SEL_LUMA_QP42_CNT;
    volatile U_SEL_LUMA_QP43_CNT    SEL_LUMA_QP43_CNT;
    volatile U_SEL_LUMA_QP44_CNT    SEL_LUMA_QP44_CNT;
    volatile U_SEL_LUMA_QP45_CNT    SEL_LUMA_QP45_CNT;
    volatile U_SEL_LUMA_QP46_CNT    SEL_LUMA_QP46_CNT;
    volatile U_SEL_LUMA_QP47_CNT    SEL_LUMA_QP47_CNT;
    volatile U_SEL_LUMA_QP48_CNT    SEL_LUMA_QP48_CNT;
    volatile U_SEL_LUMA_QP49_CNT    SEL_LUMA_QP49_CNT;
    volatile U_SEL_LUMA_QP50_CNT    SEL_LUMA_QP50_CNT;
    volatile U_SEL_LUMA_QP51_CNT    SEL_LUMA_QP51_CNT;
    volatile unsigned int           RESERVED5[12];


} S_sel_REGS_TYPE;

/* Declare the struct pointor of the module sel */
extern volatile S_sel_REGS_TYPE *gopselAllReg;

/* Declare the functions that set the member value */
int iSetSEL_INTRA_ABS_OFFSET0sel_intra4_8_abs_offset(unsigned int usel_intra4_8_abs_offset);
int iSetSEL_INTRA_ABS_OFFSET0sel_intra16_abs_offset(unsigned int usel_intra16_abs_offset);
int iSetSEL_INTRA_ABS_OFFSET1sel_layer32_abs_offset(unsigned int usel_layer32_abs_offset);
int iSetSEL_INTER_ABS_OFFSET0sel_inter8_16_abs_offset(unsigned int usel_inter8_16_abs_offset);
int iSetSEL_INTER_ABS_OFFSET0sel_inter32_abs_offset(unsigned int usel_inter32_abs_offset);



#endif /* __C_UNION_DEFINE_SEL_H__ */
