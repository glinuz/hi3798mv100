//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_qpg.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module qpg
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_QPG_H__
#define __C_UNION_DEFINE_QPG_H__

/* Define the union U_QPG_CU_QP_DELTA_THRESH_REG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_cu_qp_delta_thresh12  : 8   ; /* [7..0]  */
        unsigned int    qpg_cu_qp_delta_thresh13  : 8   ; /* [15..8]  */
        unsigned int    qpg_cu_qp_delta_thresh14  : 8   ; /* [23..16]  */
        unsigned int    qpg_cu_qp_delta_thresh15  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_CU_QP_DELTA_THRESH_REG3;

/* Define the union U_QPG_MAX_MIN_QP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_min_qp            : 6   ; /* [5..0]  */
        unsigned int    qpg_cu_qp_delta_enable_flag  : 1   ; /* [6]  */
        unsigned int    Reserved_3            : 1   ; /* [7]  */
        unsigned int    qpg_max_qp            : 6   ; /* [13..8]  */
        unsigned int    Reserved_2            : 2   ; /* [15..14]  */
        unsigned int    qpg_lowluma_max_qp    : 6   ; /* [21..16]  */
        unsigned int    Reserved_1            : 2   ; /* [23..22]  */
        unsigned int    qpg_lowluma_qp_delta  : 4   ; /* [27..24]  */
        unsigned int    Reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_MAX_MIN_QP;

/* Define the union U_QPG_AVERAGE_LCU_BITS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_ave_lcu_bits      : 16  ; /* [15..0]  */
        unsigned int    qpg_move_edge_max_qp  : 6   ; /* [21..16]  */
        unsigned int    Reserved_6            : 2   ; /* [23..22]  */
        unsigned int    Reserved_5            : 6   ; /* [29..24]  */
        unsigned int    Reserved_4            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_AVERAGE_LCU_BITS;

/* Define the union U_QPG_QP_DELTA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_qp_delta          : 6   ; /* [5..0]  */
        unsigned int    Reserved_10           : 2   ; /* [7..6]  */
        unsigned int    qpg_skin_max_qp       : 6   ; /* [13..8]  */
        unsigned int    Reserved_9            : 2   ; /* [15..14]  */
        unsigned int    qpg_strong_edge_max_qp  : 6   ; /* [21..16]  */
        unsigned int    Reserved_8            : 2   ; /* [23..22]  */
        unsigned int    qpg_large_move_max_qp  : 6   ; /* [29..24]  */
        unsigned int    Reserved_7            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_DELTA;

/* Define the union U_QPG_CU_QP_DELTA_THRESH_REG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_cu_qp_delta_thresh0  : 8   ; /* [7..0]  */
        unsigned int    qpg_cu_qp_delta_thresh1  : 8   ; /* [15..8]  */
        unsigned int    qpg_cu_qp_delta_thresh2  : 8   ; /* [23..16]  */
        unsigned int    qpg_cu_qp_delta_thresh3  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_CU_QP_DELTA_THRESH_REG0;

/* Define the union U_QPG_CU_QP_DELTA_THRESH_REG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_cu_qp_delta_thresh4  : 8   ; /* [7..0]  */
        unsigned int    qpg_cu_qp_delta_thresh5  : 8   ; /* [15..8]  */
        unsigned int    qpg_cu_qp_delta_thresh6  : 8   ; /* [23..16]  */
        unsigned int    qpg_cu_qp_delta_thresh7  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_CU_QP_DELTA_THRESH_REG1;

/* Define the union U_QPG_CU_QP_DELTA_THRESH_REG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_cu_qp_delta_thresh8  : 8   ; /* [7..0]  */
        unsigned int    qpg_cu_qp_delta_thresh9  : 8   ; /* [15..8]  */
        unsigned int    qpg_cu_qp_delta_thresh10  : 8   ; /* [23..16]  */
        unsigned int    qpg_cu_qp_delta_thresh11  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_CU_QP_DELTA_THRESH_REG2;

/* Define the union U_QPG_CU32_DELTA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_cu32_delta_low    : 4   ; /* [3..0]  */
        unsigned int    qpg_cu32_delta_high   : 4   ; /* [7..4]  */
        unsigned int    Reserved_11           : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_CU32_DELTA;

/* Define the union U_QPG_STRONG_EDGE_QP_DELTA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_strong_edge_qp_delta  : 4   ; /* [3..0]  */
        unsigned int    Reserved_12           : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_STRONG_EDGE_QP_DELTA;

/* Define the union U_QPG_SCENE_QP_DELTA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_large_move_qp_delta  : 4   ; /* [3..0]  */
        unsigned int    qpg_strong_edge_move_qp_delta  : 4   ; /* [7..4]  */
        unsigned int    Reserved_14           : 4   ; /* [11..8]  */
        unsigned int    Reserved_13           : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_SCENE_QP_DELTA;

/* Define the union U_QPG_SKIN_QP_DELTA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_skin_qp_delta     : 4   ; /* [3..0]  */
        unsigned int    Reserved_15           : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_SKIN_QP_DELTA;

/* Define the union U_QPG_DELTA_LEVEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_qp_delta_level_0  : 2   ; /* [1..0]  */
        unsigned int    qpg_qp_delta_level_1  : 2   ; /* [3..2]  */
        unsigned int    qpg_qp_delta_level_2  : 2   ; /* [5..4]  */
        unsigned int    qpg_qp_delta_level_3  : 2   ; /* [7..6]  */
        unsigned int    qpg_qp_delta_level_4  : 2   ; /* [9..8]  */
        unsigned int    qpg_qp_delta_level_5  : 2   ; /* [11..10]  */
        unsigned int    qpg_qp_delta_level_6  : 2   ; /* [13..12]  */
        unsigned int    qpg_qp_delta_level_7  : 2   ; /* [15..14]  */
        unsigned int    qpg_qp_delta_level_8  : 2   ; /* [17..16]  */
        unsigned int    qpg_qp_delta_level_9  : 2   ; /* [19..18]  */
        unsigned int    qpg_qp_delta_level_10  : 2   ; /* [21..20]  */
        unsigned int    qpg_qp_delta_level_11  : 2   ; /* [23..22]  */
        unsigned int    qpg_qp_delta_level_12  : 2   ; /* [25..24]  */
        unsigned int    qpg_qp_delta_level_13  : 2   ; /* [27..26]  */
        unsigned int    qpg_qp_delta_level_14  : 2   ; /* [29..28]  */
        unsigned int    qpg_qp_delta_level_15  : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_DELTA_LEVEL;

/* Define the union U_QPG_MADI_SWITCH_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_qp_madi_switch_thr  : 5   ; /* [4..0]  */
        unsigned int    Reserved_16           : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_MADI_SWITCH_THR;

/* Define the union U_QPG_INTRA_DET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_intra_det_max_qp  : 6   ; /* [5..0]  */
        unsigned int    Reserved_19           : 2   ; /* [7..6]  */
        unsigned int    qpg_intra_det_qp_delta  : 4   ; /* [11..8]  */
        unsigned int    Reserved_18           : 4   ; /* [15..12]  */
        unsigned int    qpg_h264_smooth_max_qp  : 6   ; /* [21..16]  */
        unsigned int    Reserved_17           : 2   ; /* [23..22]  */
        unsigned int    qpg_h264_smooth_qp_delta  : 4   ; /* [27..24]  */
        unsigned int    qpg_h264_smooth_qp_delta1  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_INTRA_DET;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG00 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda00          : 20  ; /* [19..0]  */
        unsigned int    Reserved_21           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG00;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda01          : 20  ; /* [19..0]  */
        unsigned int    Reserved_22           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG01;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG02 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda02          : 20  ; /* [19..0]  */
        unsigned int    Reserved_23           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG02;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG03 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda03          : 20  ; /* [19..0]  */
        unsigned int    Reserved_24           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG03;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG04 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda04          : 20  ; /* [19..0]  */
        unsigned int    Reserved_25           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG04;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG05 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda05          : 20  ; /* [19..0]  */
        unsigned int    Reserved_26           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG05;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG06 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda06          : 20  ; /* [19..0]  */
        unsigned int    Reserved_27           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG06;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG07 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda07          : 20  ; /* [19..0]  */
        unsigned int    Reserved_28           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG07;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG08 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda08          : 20  ; /* [19..0]  */
        unsigned int    Reserved_29           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG08;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG09 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda09          : 20  ; /* [19..0]  */
        unsigned int    Reserved_30           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG09;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda10          : 20  ; /* [19..0]  */
        unsigned int    Reserved_31           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG10;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda11          : 20  ; /* [19..0]  */
        unsigned int    Reserved_32           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG11;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda12          : 20  ; /* [19..0]  */
        unsigned int    Reserved_33           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG12;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda13          : 20  ; /* [19..0]  */
        unsigned int    Reserved_34           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG13;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda14          : 20  ; /* [19..0]  */
        unsigned int    Reserved_35           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG14;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda15          : 20  ; /* [19..0]  */
        unsigned int    Reserved_36           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG15;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda16          : 20  ; /* [19..0]  */
        unsigned int    Reserved_37           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG16;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda17          : 20  ; /* [19..0]  */
        unsigned int    Reserved_38           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG17;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda18          : 20  ; /* [19..0]  */
        unsigned int    Reserved_39           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG18;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda19          : 20  ; /* [19..0]  */
        unsigned int    Reserved_40           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG19;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda20          : 20  ; /* [19..0]  */
        unsigned int    Reserved_41           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG20;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda21          : 20  ; /* [19..0]  */
        unsigned int    Reserved_42           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG21;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda22          : 20  ; /* [19..0]  */
        unsigned int    Reserved_43           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG22;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda23          : 20  ; /* [19..0]  */
        unsigned int    Reserved_44           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG23;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda24          : 20  ; /* [19..0]  */
        unsigned int    Reserved_45           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG24;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda25          : 20  ; /* [19..0]  */
        unsigned int    Reserved_46           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG25;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG26 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda26          : 20  ; /* [19..0]  */
        unsigned int    Reserved_47           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG26;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG27 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda27          : 20  ; /* [19..0]  */
        unsigned int    Reserved_48           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG27;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG28 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda28          : 20  ; /* [19..0]  */
        unsigned int    Reserved_49           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG28;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG29 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda29          : 20  ; /* [19..0]  */
        unsigned int    Reserved_50           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG29;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG30 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda30          : 20  ; /* [19..0]  */
        unsigned int    Reserved_51           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG30;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG31 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda31          : 20  ; /* [19..0]  */
        unsigned int    Reserved_52           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG31;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG32 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda32          : 20  ; /* [19..0]  */
        unsigned int    Reserved_53           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG32;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG33 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda33          : 20  ; /* [19..0]  */
        unsigned int    Reserved_54           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG33;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG34 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda34          : 20  ; /* [19..0]  */
        unsigned int    Reserved_55           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG34;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG35 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda35          : 20  ; /* [19..0]  */
        unsigned int    Reserved_56           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG35;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG36 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda36          : 20  ; /* [19..0]  */
        unsigned int    Reserved_57           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG36;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG37 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda37          : 20  ; /* [19..0]  */
        unsigned int    Reserved_58           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG37;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG38 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda38          : 20  ; /* [19..0]  */
        unsigned int    Reserved_59           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG38;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG39 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda39          : 20  ; /* [19..0]  */
        unsigned int    Reserved_60           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG39;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG40 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda40          : 20  ; /* [19..0]  */
        unsigned int    Reserved_61           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG40;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG41 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda41          : 20  ; /* [19..0]  */
        unsigned int    Reserved_62           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG41;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG42 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda42          : 20  ; /* [19..0]  */
        unsigned int    Reserved_63           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG42;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG43 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda43          : 20  ; /* [19..0]  */
        unsigned int    Reserved_64           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG43;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG44 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda44          : 20  ; /* [19..0]  */
        unsigned int    Reserved_65           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG44;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG45 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda45          : 20  ; /* [19..0]  */
        unsigned int    Reserved_66           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG45;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG46 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda46          : 20  ; /* [19..0]  */
        unsigned int    Reserved_67           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG46;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG47 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda47          : 20  ; /* [19..0]  */
        unsigned int    Reserved_68           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG47;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG48 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda48          : 20  ; /* [19..0]  */
        unsigned int    Reserved_69           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG48;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG49 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda49          : 20  ; /* [19..0]  */
        unsigned int    Reserved_70           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG49;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG50 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda50          : 20  ; /* [19..0]  */
        unsigned int    Reserved_71           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG50;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG51 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda51          : 20  ; /* [19..0]  */
        unsigned int    Reserved_72           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG51;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG52 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda52          : 20  ; /* [19..0]  */
        unsigned int    Reserved_73           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG52;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG53 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda53          : 20  ; /* [19..0]  */
        unsigned int    Reserved_74           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG53;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG54 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda54          : 20  ; /* [19..0]  */
        unsigned int    Reserved_75           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG54;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG55 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda55          : 20  ; /* [19..0]  */
        unsigned int    Reserved_76           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG55;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG56 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda56          : 20  ; /* [19..0]  */
        unsigned int    Reserved_77           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG56;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG57 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda57          : 20  ; /* [19..0]  */
        unsigned int    Reserved_78           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG57;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG58 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda58          : 20  ; /* [19..0]  */
        unsigned int    Reserved_79           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG58;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG59 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda59          : 20  ; /* [19..0]  */
        unsigned int    Reserved_80           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG59;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG60 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda60          : 20  ; /* [19..0]  */
        unsigned int    Reserved_81           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG60;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG61 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda61          : 20  ; /* [19..0]  */
        unsigned int    Reserved_82           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG61;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG62 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda62          : 20  ; /* [19..0]  */
        unsigned int    Reserved_83           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG62;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG63 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda63          : 20  ; /* [19..0]  */
        unsigned int    Reserved_84           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG63;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG64 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda64          : 20  ; /* [19..0]  */
        unsigned int    Reserved_85           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG64;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG65 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda65          : 20  ; /* [19..0]  */
        unsigned int    Reserved_86           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG65;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG66 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda66          : 20  ; /* [19..0]  */
        unsigned int    Reserved_87           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG66;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG67 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda67          : 20  ; /* [19..0]  */
        unsigned int    Reserved_88           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG67;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG68 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda68          : 20  ; /* [19..0]  */
        unsigned int    Reserved_89           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG68;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG69 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda69          : 20  ; /* [19..0]  */
        unsigned int    Reserved_90           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG69;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG70 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda70          : 20  ; /* [19..0]  */
        unsigned int    Reserved_91           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG70;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG71 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda71          : 20  ; /* [19..0]  */
        unsigned int    Reserved_92           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG71;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG72 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda72          : 20  ; /* [19..0]  */
        unsigned int    Reserved_93           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG72;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG73 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda73          : 20  ; /* [19..0]  */
        unsigned int    Reserved_94           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG73;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG74 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda74          : 20  ; /* [19..0]  */
        unsigned int    Reserved_95           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG74;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG75 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda75          : 20  ; /* [19..0]  */
        unsigned int    Reserved_96           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG75;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG76 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda76          : 20  ; /* [19..0]  */
        unsigned int    Reserved_97           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG76;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG77 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda77          : 20  ; /* [19..0]  */
        unsigned int    Reserved_98           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG77;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG78 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda78          : 20  ; /* [19..0]  */
        unsigned int    Reserved_99           : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG78;

/* Define the union U_QPG_QP_LAMBDA_CTRL_REG79 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda79          : 20  ; /* [19..0]  */
        unsigned int    Reserved_100          : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_QPG_QP_LAMBDA_CTRL_REG79;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_QPG_CU_QP_DELTA_THRESH_REG3   QPG_CU_QP_DELTA_THRESH_REG3;
    volatile U_QPG_MAX_MIN_QP       QPG_MAX_MIN_QP;
    volatile U_QPG_AVERAGE_LCU_BITS   QPG_AVERAGE_LCU_BITS;
    volatile U_QPG_QP_DELTA         QPG_QP_DELTA;
    volatile U_QPG_CU_QP_DELTA_THRESH_REG0   QPG_CU_QP_DELTA_THRESH_REG0;
    volatile U_QPG_CU_QP_DELTA_THRESH_REG1   QPG_CU_QP_DELTA_THRESH_REG1;
    volatile U_QPG_CU_QP_DELTA_THRESH_REG2   QPG_CU_QP_DELTA_THRESH_REG2;
    volatile U_QPG_CU32_DELTA       QPG_CU32_DELTA;
    volatile U_QPG_STRONG_EDGE_QP_DELTA   QPG_STRONG_EDGE_QP_DELTA;
    volatile U_QPG_SCENE_QP_DELTA   QPG_SCENE_QP_DELTA;
    volatile U_QPG_SKIN_QP_DELTA    QPG_SKIN_QP_DELTA;
    volatile U_QPG_DELTA_LEVEL      QPG_DELTA_LEVEL;
    volatile U_QPG_MADI_SWITCH_THR   QPG_MADI_SWITCH_THR;
    volatile U_QPG_INTRA_DET        QPG_INTRA_DET;
    volatile unsigned int           RESERVED0[6];
    volatile U_QPG_QP_LAMBDA_CTRL_REG00   QPG_QP_LAMBDA_CTRL_REG00;
    volatile U_QPG_QP_LAMBDA_CTRL_REG01   QPG_QP_LAMBDA_CTRL_REG01;
    volatile U_QPG_QP_LAMBDA_CTRL_REG02   QPG_QP_LAMBDA_CTRL_REG02;
    volatile U_QPG_QP_LAMBDA_CTRL_REG03   QPG_QP_LAMBDA_CTRL_REG03;
    volatile U_QPG_QP_LAMBDA_CTRL_REG04   QPG_QP_LAMBDA_CTRL_REG04;
    volatile U_QPG_QP_LAMBDA_CTRL_REG05   QPG_QP_LAMBDA_CTRL_REG05;
    volatile U_QPG_QP_LAMBDA_CTRL_REG06   QPG_QP_LAMBDA_CTRL_REG06;
    volatile U_QPG_QP_LAMBDA_CTRL_REG07   QPG_QP_LAMBDA_CTRL_REG07;
    volatile U_QPG_QP_LAMBDA_CTRL_REG08   QPG_QP_LAMBDA_CTRL_REG08;
    volatile U_QPG_QP_LAMBDA_CTRL_REG09   QPG_QP_LAMBDA_CTRL_REG09;
    volatile U_QPG_QP_LAMBDA_CTRL_REG10   QPG_QP_LAMBDA_CTRL_REG10;
    volatile U_QPG_QP_LAMBDA_CTRL_REG11   QPG_QP_LAMBDA_CTRL_REG11;
    volatile U_QPG_QP_LAMBDA_CTRL_REG12   QPG_QP_LAMBDA_CTRL_REG12;
    volatile U_QPG_QP_LAMBDA_CTRL_REG13   QPG_QP_LAMBDA_CTRL_REG13;
    volatile U_QPG_QP_LAMBDA_CTRL_REG14   QPG_QP_LAMBDA_CTRL_REG14;
    volatile U_QPG_QP_LAMBDA_CTRL_REG15   QPG_QP_LAMBDA_CTRL_REG15;
    volatile U_QPG_QP_LAMBDA_CTRL_REG16   QPG_QP_LAMBDA_CTRL_REG16;
    volatile U_QPG_QP_LAMBDA_CTRL_REG17   QPG_QP_LAMBDA_CTRL_REG17;
    volatile U_QPG_QP_LAMBDA_CTRL_REG18   QPG_QP_LAMBDA_CTRL_REG18;
    volatile U_QPG_QP_LAMBDA_CTRL_REG19   QPG_QP_LAMBDA_CTRL_REG19;
    volatile U_QPG_QP_LAMBDA_CTRL_REG20   QPG_QP_LAMBDA_CTRL_REG20;
    volatile U_QPG_QP_LAMBDA_CTRL_REG21   QPG_QP_LAMBDA_CTRL_REG21;
    volatile U_QPG_QP_LAMBDA_CTRL_REG22   QPG_QP_LAMBDA_CTRL_REG22;
    volatile U_QPG_QP_LAMBDA_CTRL_REG23   QPG_QP_LAMBDA_CTRL_REG23;
    volatile U_QPG_QP_LAMBDA_CTRL_REG24   QPG_QP_LAMBDA_CTRL_REG24;
    volatile U_QPG_QP_LAMBDA_CTRL_REG25   QPG_QP_LAMBDA_CTRL_REG25;
    volatile U_QPG_QP_LAMBDA_CTRL_REG26   QPG_QP_LAMBDA_CTRL_REG26;
    volatile U_QPG_QP_LAMBDA_CTRL_REG27   QPG_QP_LAMBDA_CTRL_REG27;
    volatile U_QPG_QP_LAMBDA_CTRL_REG28   QPG_QP_LAMBDA_CTRL_REG28;
    volatile U_QPG_QP_LAMBDA_CTRL_REG29   QPG_QP_LAMBDA_CTRL_REG29;
    volatile U_QPG_QP_LAMBDA_CTRL_REG30   QPG_QP_LAMBDA_CTRL_REG30;
    volatile U_QPG_QP_LAMBDA_CTRL_REG31   QPG_QP_LAMBDA_CTRL_REG31;
    volatile U_QPG_QP_LAMBDA_CTRL_REG32   QPG_QP_LAMBDA_CTRL_REG32;
    volatile U_QPG_QP_LAMBDA_CTRL_REG33   QPG_QP_LAMBDA_CTRL_REG33;
    volatile U_QPG_QP_LAMBDA_CTRL_REG34   QPG_QP_LAMBDA_CTRL_REG34;
    volatile U_QPG_QP_LAMBDA_CTRL_REG35   QPG_QP_LAMBDA_CTRL_REG35;
    volatile U_QPG_QP_LAMBDA_CTRL_REG36   QPG_QP_LAMBDA_CTRL_REG36;
    volatile U_QPG_QP_LAMBDA_CTRL_REG37   QPG_QP_LAMBDA_CTRL_REG37;
    volatile U_QPG_QP_LAMBDA_CTRL_REG38   QPG_QP_LAMBDA_CTRL_REG38;
    volatile U_QPG_QP_LAMBDA_CTRL_REG39   QPG_QP_LAMBDA_CTRL_REG39;
    volatile U_QPG_QP_LAMBDA_CTRL_REG40   QPG_QP_LAMBDA_CTRL_REG40;
    volatile U_QPG_QP_LAMBDA_CTRL_REG41   QPG_QP_LAMBDA_CTRL_REG41;
    volatile U_QPG_QP_LAMBDA_CTRL_REG42   QPG_QP_LAMBDA_CTRL_REG42;
    volatile U_QPG_QP_LAMBDA_CTRL_REG43   QPG_QP_LAMBDA_CTRL_REG43;
    volatile U_QPG_QP_LAMBDA_CTRL_REG44   QPG_QP_LAMBDA_CTRL_REG44;
    volatile U_QPG_QP_LAMBDA_CTRL_REG45   QPG_QP_LAMBDA_CTRL_REG45;
    volatile U_QPG_QP_LAMBDA_CTRL_REG46   QPG_QP_LAMBDA_CTRL_REG46;
    volatile U_QPG_QP_LAMBDA_CTRL_REG47   QPG_QP_LAMBDA_CTRL_REG47;
    volatile U_QPG_QP_LAMBDA_CTRL_REG48   QPG_QP_LAMBDA_CTRL_REG48;
    volatile U_QPG_QP_LAMBDA_CTRL_REG49   QPG_QP_LAMBDA_CTRL_REG49;
    volatile U_QPG_QP_LAMBDA_CTRL_REG50   QPG_QP_LAMBDA_CTRL_REG50;
    volatile U_QPG_QP_LAMBDA_CTRL_REG51   QPG_QP_LAMBDA_CTRL_REG51;
    volatile U_QPG_QP_LAMBDA_CTRL_REG52   QPG_QP_LAMBDA_CTRL_REG52;
    volatile U_QPG_QP_LAMBDA_CTRL_REG53   QPG_QP_LAMBDA_CTRL_REG53;
    volatile U_QPG_QP_LAMBDA_CTRL_REG54   QPG_QP_LAMBDA_CTRL_REG54;
    volatile U_QPG_QP_LAMBDA_CTRL_REG55   QPG_QP_LAMBDA_CTRL_REG55;
    volatile U_QPG_QP_LAMBDA_CTRL_REG56   QPG_QP_LAMBDA_CTRL_REG56;
    volatile U_QPG_QP_LAMBDA_CTRL_REG57   QPG_QP_LAMBDA_CTRL_REG57;
    volatile U_QPG_QP_LAMBDA_CTRL_REG58   QPG_QP_LAMBDA_CTRL_REG58;
    volatile U_QPG_QP_LAMBDA_CTRL_REG59   QPG_QP_LAMBDA_CTRL_REG59;
    volatile U_QPG_QP_LAMBDA_CTRL_REG60   QPG_QP_LAMBDA_CTRL_REG60;
    volatile U_QPG_QP_LAMBDA_CTRL_REG61   QPG_QP_LAMBDA_CTRL_REG61;
    volatile U_QPG_QP_LAMBDA_CTRL_REG62   QPG_QP_LAMBDA_CTRL_REG62;
    volatile U_QPG_QP_LAMBDA_CTRL_REG63   QPG_QP_LAMBDA_CTRL_REG63;
    volatile U_QPG_QP_LAMBDA_CTRL_REG64   QPG_QP_LAMBDA_CTRL_REG64;
    volatile U_QPG_QP_LAMBDA_CTRL_REG65   QPG_QP_LAMBDA_CTRL_REG65;
    volatile U_QPG_QP_LAMBDA_CTRL_REG66   QPG_QP_LAMBDA_CTRL_REG66;
    volatile U_QPG_QP_LAMBDA_CTRL_REG67   QPG_QP_LAMBDA_CTRL_REG67;
    volatile U_QPG_QP_LAMBDA_CTRL_REG68   QPG_QP_LAMBDA_CTRL_REG68;
    volatile U_QPG_QP_LAMBDA_CTRL_REG69   QPG_QP_LAMBDA_CTRL_REG69;
    volatile U_QPG_QP_LAMBDA_CTRL_REG70   QPG_QP_LAMBDA_CTRL_REG70;
    volatile U_QPG_QP_LAMBDA_CTRL_REG71   QPG_QP_LAMBDA_CTRL_REG71;
    volatile U_QPG_QP_LAMBDA_CTRL_REG72   QPG_QP_LAMBDA_CTRL_REG72;
    volatile U_QPG_QP_LAMBDA_CTRL_REG73   QPG_QP_LAMBDA_CTRL_REG73;
    volatile U_QPG_QP_LAMBDA_CTRL_REG74   QPG_QP_LAMBDA_CTRL_REG74;
    volatile U_QPG_QP_LAMBDA_CTRL_REG75   QPG_QP_LAMBDA_CTRL_REG75;
    volatile U_QPG_QP_LAMBDA_CTRL_REG76   QPG_QP_LAMBDA_CTRL_REG76;
    volatile U_QPG_QP_LAMBDA_CTRL_REG77   QPG_QP_LAMBDA_CTRL_REG77;
    volatile U_QPG_QP_LAMBDA_CTRL_REG78   QPG_QP_LAMBDA_CTRL_REG78;
    volatile U_QPG_QP_LAMBDA_CTRL_REG79   QPG_QP_LAMBDA_CTRL_REG79;
    volatile unsigned int           RESERVED1[28];


} S_qpg_REGS_TYPE;

/* Declare the struct pointor of the module qpg */
extern volatile S_qpg_REGS_TYPE *gopqpgAllReg;

/* Declare the functions that set the member value */
int iSetQPG_CU_QP_DELTA_THRESH_REG3qpg_cu_qp_delta_thresh12(unsigned int uqpg_cu_qp_delta_thresh12);
int iSetQPG_CU_QP_DELTA_THRESH_REG3qpg_cu_qp_delta_thresh13(unsigned int uqpg_cu_qp_delta_thresh13);
int iSetQPG_CU_QP_DELTA_THRESH_REG3qpg_cu_qp_delta_thresh14(unsigned int uqpg_cu_qp_delta_thresh14);
int iSetQPG_CU_QP_DELTA_THRESH_REG3qpg_cu_qp_delta_thresh15(unsigned int uqpg_cu_qp_delta_thresh15);
int iSetQPG_MAX_MIN_QPqpg_min_qp(unsigned int uqpg_min_qp);
int iSetQPG_MAX_MIN_QPqpg_cu_qp_delta_enable_flag(unsigned int uqpg_cu_qp_delta_enable_flag);
int iSetQPG_MAX_MIN_QPqpg_max_qp(unsigned int uqpg_max_qp);
int iSetQPG_MAX_MIN_QPqpg_lowluma_max_qp(unsigned int uqpg_lowluma_max_qp);
int iSetQPG_MAX_MIN_QPqpg_lowluma_qp_delta(unsigned int uqpg_lowluma_qp_delta);
int iSetQPG_AVERAGE_LCU_BITSqpg_ave_lcu_bits(unsigned int uqpg_ave_lcu_bits);
int iSetQPG_AVERAGE_LCU_BITSqpg_move_edge_max_qp(unsigned int uqpg_move_edge_max_qp);
int iSetQPG_QP_DELTAqpg_qp_delta(unsigned int uqpg_qp_delta);
int iSetQPG_QP_DELTAqpg_skin_max_qp(unsigned int uqpg_skin_max_qp);
int iSetQPG_QP_DELTAqpg_strong_edge_max_qp(unsigned int uqpg_strong_edge_max_qp);
int iSetQPG_QP_DELTAqpg_large_move_max_qp(unsigned int uqpg_large_move_max_qp);
int iSetQPG_CU_QP_DELTA_THRESH_REG0qpg_cu_qp_delta_thresh0(unsigned int uqpg_cu_qp_delta_thresh0);
int iSetQPG_CU_QP_DELTA_THRESH_REG0qpg_cu_qp_delta_thresh1(unsigned int uqpg_cu_qp_delta_thresh1);
int iSetQPG_CU_QP_DELTA_THRESH_REG0qpg_cu_qp_delta_thresh2(unsigned int uqpg_cu_qp_delta_thresh2);
int iSetQPG_CU_QP_DELTA_THRESH_REG0qpg_cu_qp_delta_thresh3(unsigned int uqpg_cu_qp_delta_thresh3);
int iSetQPG_CU_QP_DELTA_THRESH_REG1qpg_cu_qp_delta_thresh4(unsigned int uqpg_cu_qp_delta_thresh4);
int iSetQPG_CU_QP_DELTA_THRESH_REG1qpg_cu_qp_delta_thresh5(unsigned int uqpg_cu_qp_delta_thresh5);
int iSetQPG_CU_QP_DELTA_THRESH_REG1qpg_cu_qp_delta_thresh6(unsigned int uqpg_cu_qp_delta_thresh6);
int iSetQPG_CU_QP_DELTA_THRESH_REG1qpg_cu_qp_delta_thresh7(unsigned int uqpg_cu_qp_delta_thresh7);
int iSetQPG_CU_QP_DELTA_THRESH_REG2qpg_cu_qp_delta_thresh8(unsigned int uqpg_cu_qp_delta_thresh8);
int iSetQPG_CU_QP_DELTA_THRESH_REG2qpg_cu_qp_delta_thresh9(unsigned int uqpg_cu_qp_delta_thresh9);
int iSetQPG_CU_QP_DELTA_THRESH_REG2qpg_cu_qp_delta_thresh10(unsigned int uqpg_cu_qp_delta_thresh10);
int iSetQPG_CU_QP_DELTA_THRESH_REG2qpg_cu_qp_delta_thresh11(unsigned int uqpg_cu_qp_delta_thresh11);
int iSetQPG_CU32_DELTAqpg_cu32_delta_low(unsigned int uqpg_cu32_delta_low);
int iSetQPG_CU32_DELTAqpg_cu32_delta_high(unsigned int uqpg_cu32_delta_high);
int iSetQPG_STRONG_EDGE_QP_DELTAqpg_strong_edge_qp_delta(unsigned int uqpg_strong_edge_qp_delta);
int iSetQPG_SCENE_QP_DELTAqpg_large_move_qp_delta(unsigned int uqpg_large_move_qp_delta);
int iSetQPG_SCENE_QP_DELTAqpg_strong_edge_move_qp_delta(unsigned int uqpg_strong_edge_move_qp_delta);
int iSetQPG_SKIN_QP_DELTAqpg_skin_qp_delta(unsigned int uqpg_skin_qp_delta);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_0(unsigned int uqpg_qp_delta_level_0);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_1(unsigned int uqpg_qp_delta_level_1);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_2(unsigned int uqpg_qp_delta_level_2);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_3(unsigned int uqpg_qp_delta_level_3);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_4(unsigned int uqpg_qp_delta_level_4);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_5(unsigned int uqpg_qp_delta_level_5);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_6(unsigned int uqpg_qp_delta_level_6);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_7(unsigned int uqpg_qp_delta_level_7);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_8(unsigned int uqpg_qp_delta_level_8);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_9(unsigned int uqpg_qp_delta_level_9);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_10(unsigned int uqpg_qp_delta_level_10);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_11(unsigned int uqpg_qp_delta_level_11);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_12(unsigned int uqpg_qp_delta_level_12);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_13(unsigned int uqpg_qp_delta_level_13);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_14(unsigned int uqpg_qp_delta_level_14);
int iSetQPG_DELTA_LEVELqpg_qp_delta_level_15(unsigned int uqpg_qp_delta_level_15);
int iSetQPG_MADI_SWITCH_THRqpg_qp_madi_switch_thr(unsigned int uqpg_qp_madi_switch_thr);
int iSetQPG_INTRA_DETqpg_intra_det_max_qp(unsigned int uqpg_intra_det_max_qp);
int iSetQPG_INTRA_DETqpg_intra_det_qp_delta(unsigned int uqpg_intra_det_qp_delta);
int iSetQPG_INTRA_DETqpg_h264_smooth_max_qp(unsigned int uqpg_h264_smooth_max_qp);
int iSetQPG_INTRA_DETqpg_h264_smooth_qp_delta(unsigned int uqpg_h264_smooth_qp_delta);
int iSetQPG_INTRA_DETqpg_h264_smooth_qp_delta1(unsigned int uqpg_h264_smooth_qp_delta1);
int iSetQPG_QP_LAMBDA_CTRL_REG00qpg_lambda00(unsigned int uqpg_lambda00);
int iSetQPG_QP_LAMBDA_CTRL_REG01qpg_lambda01(unsigned int uqpg_lambda01);
int iSetQPG_QP_LAMBDA_CTRL_REG02qpg_lambda02(unsigned int uqpg_lambda02);
int iSetQPG_QP_LAMBDA_CTRL_REG03qpg_lambda03(unsigned int uqpg_lambda03);
int iSetQPG_QP_LAMBDA_CTRL_REG04qpg_lambda04(unsigned int uqpg_lambda04);
int iSetQPG_QP_LAMBDA_CTRL_REG05qpg_lambda05(unsigned int uqpg_lambda05);
int iSetQPG_QP_LAMBDA_CTRL_REG06qpg_lambda06(unsigned int uqpg_lambda06);
int iSetQPG_QP_LAMBDA_CTRL_REG07qpg_lambda07(unsigned int uqpg_lambda07);
int iSetQPG_QP_LAMBDA_CTRL_REG08qpg_lambda08(unsigned int uqpg_lambda08);
int iSetQPG_QP_LAMBDA_CTRL_REG09qpg_lambda09(unsigned int uqpg_lambda09);
int iSetQPG_QP_LAMBDA_CTRL_REG10qpg_lambda10(unsigned int uqpg_lambda10);
int iSetQPG_QP_LAMBDA_CTRL_REG11qpg_lambda11(unsigned int uqpg_lambda11);
int iSetQPG_QP_LAMBDA_CTRL_REG12qpg_lambda12(unsigned int uqpg_lambda12);
int iSetQPG_QP_LAMBDA_CTRL_REG13qpg_lambda13(unsigned int uqpg_lambda13);
int iSetQPG_QP_LAMBDA_CTRL_REG14qpg_lambda14(unsigned int uqpg_lambda14);
int iSetQPG_QP_LAMBDA_CTRL_REG15qpg_lambda15(unsigned int uqpg_lambda15);
int iSetQPG_QP_LAMBDA_CTRL_REG16qpg_lambda16(unsigned int uqpg_lambda16);
int iSetQPG_QP_LAMBDA_CTRL_REG17qpg_lambda17(unsigned int uqpg_lambda17);
int iSetQPG_QP_LAMBDA_CTRL_REG18qpg_lambda18(unsigned int uqpg_lambda18);
int iSetQPG_QP_LAMBDA_CTRL_REG19qpg_lambda19(unsigned int uqpg_lambda19);
int iSetQPG_QP_LAMBDA_CTRL_REG20qpg_lambda20(unsigned int uqpg_lambda20);
int iSetQPG_QP_LAMBDA_CTRL_REG21qpg_lambda21(unsigned int uqpg_lambda21);
int iSetQPG_QP_LAMBDA_CTRL_REG22qpg_lambda22(unsigned int uqpg_lambda22);
int iSetQPG_QP_LAMBDA_CTRL_REG23qpg_lambda23(unsigned int uqpg_lambda23);
int iSetQPG_QP_LAMBDA_CTRL_REG24qpg_lambda24(unsigned int uqpg_lambda24);
int iSetQPG_QP_LAMBDA_CTRL_REG25qpg_lambda25(unsigned int uqpg_lambda25);
int iSetQPG_QP_LAMBDA_CTRL_REG26qpg_lambda26(unsigned int uqpg_lambda26);
int iSetQPG_QP_LAMBDA_CTRL_REG27qpg_lambda27(unsigned int uqpg_lambda27);
int iSetQPG_QP_LAMBDA_CTRL_REG28qpg_lambda28(unsigned int uqpg_lambda28);
int iSetQPG_QP_LAMBDA_CTRL_REG29qpg_lambda29(unsigned int uqpg_lambda29);
int iSetQPG_QP_LAMBDA_CTRL_REG30qpg_lambda30(unsigned int uqpg_lambda30);
int iSetQPG_QP_LAMBDA_CTRL_REG31qpg_lambda31(unsigned int uqpg_lambda31);
int iSetQPG_QP_LAMBDA_CTRL_REG32qpg_lambda32(unsigned int uqpg_lambda32);
int iSetQPG_QP_LAMBDA_CTRL_REG33qpg_lambda33(unsigned int uqpg_lambda33);
int iSetQPG_QP_LAMBDA_CTRL_REG34qpg_lambda34(unsigned int uqpg_lambda34);
int iSetQPG_QP_LAMBDA_CTRL_REG35qpg_lambda35(unsigned int uqpg_lambda35);
int iSetQPG_QP_LAMBDA_CTRL_REG36qpg_lambda36(unsigned int uqpg_lambda36);
int iSetQPG_QP_LAMBDA_CTRL_REG37qpg_lambda37(unsigned int uqpg_lambda37);
int iSetQPG_QP_LAMBDA_CTRL_REG38qpg_lambda38(unsigned int uqpg_lambda38);
int iSetQPG_QP_LAMBDA_CTRL_REG39qpg_lambda39(unsigned int uqpg_lambda39);
int iSetQPG_QP_LAMBDA_CTRL_REG40qpg_lambda40(unsigned int uqpg_lambda40);
int iSetQPG_QP_LAMBDA_CTRL_REG41qpg_lambda41(unsigned int uqpg_lambda41);
int iSetQPG_QP_LAMBDA_CTRL_REG42qpg_lambda42(unsigned int uqpg_lambda42);
int iSetQPG_QP_LAMBDA_CTRL_REG43qpg_lambda43(unsigned int uqpg_lambda43);
int iSetQPG_QP_LAMBDA_CTRL_REG44qpg_lambda44(unsigned int uqpg_lambda44);
int iSetQPG_QP_LAMBDA_CTRL_REG45qpg_lambda45(unsigned int uqpg_lambda45);
int iSetQPG_QP_LAMBDA_CTRL_REG46qpg_lambda46(unsigned int uqpg_lambda46);
int iSetQPG_QP_LAMBDA_CTRL_REG47qpg_lambda47(unsigned int uqpg_lambda47);
int iSetQPG_QP_LAMBDA_CTRL_REG48qpg_lambda48(unsigned int uqpg_lambda48);
int iSetQPG_QP_LAMBDA_CTRL_REG49qpg_lambda49(unsigned int uqpg_lambda49);
int iSetQPG_QP_LAMBDA_CTRL_REG50qpg_lambda50(unsigned int uqpg_lambda50);
int iSetQPG_QP_LAMBDA_CTRL_REG51qpg_lambda51(unsigned int uqpg_lambda51);
int iSetQPG_QP_LAMBDA_CTRL_REG52qpg_lambda52(unsigned int uqpg_lambda52);
int iSetQPG_QP_LAMBDA_CTRL_REG53qpg_lambda53(unsigned int uqpg_lambda53);
int iSetQPG_QP_LAMBDA_CTRL_REG54qpg_lambda54(unsigned int uqpg_lambda54);
int iSetQPG_QP_LAMBDA_CTRL_REG55qpg_lambda55(unsigned int uqpg_lambda55);
int iSetQPG_QP_LAMBDA_CTRL_REG56qpg_lambda56(unsigned int uqpg_lambda56);
int iSetQPG_QP_LAMBDA_CTRL_REG57qpg_lambda57(unsigned int uqpg_lambda57);
int iSetQPG_QP_LAMBDA_CTRL_REG58qpg_lambda58(unsigned int uqpg_lambda58);
int iSetQPG_QP_LAMBDA_CTRL_REG59qpg_lambda59(unsigned int uqpg_lambda59);
int iSetQPG_QP_LAMBDA_CTRL_REG60qpg_lambda60(unsigned int uqpg_lambda60);
int iSetQPG_QP_LAMBDA_CTRL_REG61qpg_lambda61(unsigned int uqpg_lambda61);
int iSetQPG_QP_LAMBDA_CTRL_REG62qpg_lambda62(unsigned int uqpg_lambda62);
int iSetQPG_QP_LAMBDA_CTRL_REG63qpg_lambda63(unsigned int uqpg_lambda63);
int iSetQPG_QP_LAMBDA_CTRL_REG64qpg_lambda64(unsigned int uqpg_lambda64);
int iSetQPG_QP_LAMBDA_CTRL_REG65qpg_lambda65(unsigned int uqpg_lambda65);
int iSetQPG_QP_LAMBDA_CTRL_REG66qpg_lambda66(unsigned int uqpg_lambda66);
int iSetQPG_QP_LAMBDA_CTRL_REG67qpg_lambda67(unsigned int uqpg_lambda67);
int iSetQPG_QP_LAMBDA_CTRL_REG68qpg_lambda68(unsigned int uqpg_lambda68);
int iSetQPG_QP_LAMBDA_CTRL_REG69qpg_lambda69(unsigned int uqpg_lambda69);
int iSetQPG_QP_LAMBDA_CTRL_REG70qpg_lambda70(unsigned int uqpg_lambda70);
int iSetQPG_QP_LAMBDA_CTRL_REG71qpg_lambda71(unsigned int uqpg_lambda71);
int iSetQPG_QP_LAMBDA_CTRL_REG72qpg_lambda72(unsigned int uqpg_lambda72);
int iSetQPG_QP_LAMBDA_CTRL_REG73qpg_lambda73(unsigned int uqpg_lambda73);
int iSetQPG_QP_LAMBDA_CTRL_REG74qpg_lambda74(unsigned int uqpg_lambda74);
int iSetQPG_QP_LAMBDA_CTRL_REG75qpg_lambda75(unsigned int uqpg_lambda75);
int iSetQPG_QP_LAMBDA_CTRL_REG76qpg_lambda76(unsigned int uqpg_lambda76);
int iSetQPG_QP_LAMBDA_CTRL_REG77qpg_lambda77(unsigned int uqpg_lambda77);
int iSetQPG_QP_LAMBDA_CTRL_REG78qpg_lambda78(unsigned int uqpg_lambda78);
int iSetQPG_QP_LAMBDA_CTRL_REG79qpg_lambda79(unsigned int uqpg_lambda79);



#endif /* __C_UNION_DEFINE_QPG_H__ */
