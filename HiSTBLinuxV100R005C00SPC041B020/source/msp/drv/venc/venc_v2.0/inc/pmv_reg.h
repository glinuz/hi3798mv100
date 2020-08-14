//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_pmv.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module pmv
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_PMV_H__
#define __C_UNION_DEFINE_PMV_H__

/* Define the union U_PMV_CNT_FRAME */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_cnt_lcu_done      : 8   ; /* [7..0]  */
        unsigned int    pmv_cnt_lcu_start     : 8   ; /* [15..8]  */
        unsigned int    cnt_frame             : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_CNT_FRAME;

/* Define the union U_PMV_TMV_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_0_ver         : 12  ; /* [11..0]  */
        unsigned int    Reserved_1            : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_0_hor         : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_0_vld         : 1   ; /* [29]  */
        unsigned int    Reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_0;

/* Define the union U_PMV_TMV_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_1_ver         : 12  ; /* [11..0]  */
        unsigned int    Reserved_3            : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_1_hor         : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_1_vld         : 1   ; /* [29]  */
        unsigned int    Reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_1;

/* Define the union U_PMV_TMV_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_2_ver         : 12  ; /* [11..0]  */
        unsigned int    Reserved_5            : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_2_hor         : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_2_vld         : 1   ; /* [29]  */
        unsigned int    Reserved_4            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_2;

/* Define the union U_PMV_TMV_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_3_ver         : 12  ; /* [11..0]  */
        unsigned int    Reserved_7            : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_3_hor         : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_3_vld         : 1   ; /* [29]  */
        unsigned int    Reserved_6            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_3;

/* Define the union U_PMV_TMV_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_4_ver         : 12  ; /* [11..0]  */
        unsigned int    Reserved_9            : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_4_hor         : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_4_vld         : 1   ; /* [29]  */
        unsigned int    Reserved_8            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_4;

/* Define the union U_PMV_TMV_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_5_ver         : 12  ; /* [11..0]  */
        unsigned int    Reserved_11           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_5_hor         : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_5_vld         : 1   ; /* [29]  */
        unsigned int    Reserved_10           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_5;

/* Define the union U_PMV_TMV_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_6_ver         : 12  ; /* [11..0]  */
        unsigned int    Reserved_13           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_6_hor         : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_6_vld         : 1   ; /* [29]  */
        unsigned int    Reserved_12           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_6;

/* Define the union U_PMV_TMV_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_7_ver         : 12  ; /* [11..0]  */
        unsigned int    Reserved_15           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_7_hor         : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_7_vld         : 1   ; /* [29]  */
        unsigned int    Reserved_14           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_7;

/* Define the union U_PMV_TMV_8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_8_ver         : 12  ; /* [11..0]  */
        unsigned int    Reserved_17           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_8_hor         : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_8_vld         : 1   ; /* [29]  */
        unsigned int    Reserved_16           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_8;

/* Define the union U_PMV_TMV_9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_9_ver         : 12  ; /* [11..0]  */
        unsigned int    Reserved_19           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_9_hor         : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_9_vld         : 1   ; /* [29]  */
        unsigned int    Reserved_18           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_9;

/* Define the union U_PMV_TMV_10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_10_ver        : 12  ; /* [11..0]  */
        unsigned int    Reserved_21           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_10_hor        : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_10_vld        : 1   ; /* [29]  */
        unsigned int    Reserved_20           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_10;

/* Define the union U_PMV_TMV_11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_11_ver        : 12  ; /* [11..0]  */
        unsigned int    Reserved_23           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_11_hor        : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_11_vld        : 1   ; /* [29]  */
        unsigned int    Reserved_22           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_11;

/* Define the union U_PMV_TMV_12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_12_ver        : 12  ; /* [11..0]  */
        unsigned int    Reserved_25           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_12_hor        : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_12_vld        : 1   ; /* [29]  */
        unsigned int    Reserved_24           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_12;

/* Define the union U_PMV_TMV_13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_13_ver        : 12  ; /* [11..0]  */
        unsigned int    Reserved_27           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_13_hor        : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_13_vld        : 1   ; /* [29]  */
        unsigned int    Reserved_26           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_13;

/* Define the union U_PMV_TMV_14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_14_ver        : 12  ; /* [11..0]  */
        unsigned int    Reserved_29           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_14_hor        : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_14_vld        : 1   ; /* [29]  */
        unsigned int    Reserved_28           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_14;

/* Define the union U_PMV_TMV_15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_15_ver        : 12  ; /* [11..0]  */
        unsigned int    Reserved_31           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_15_hor        : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_15_vld        : 1   ; /* [29]  */
        unsigned int    Reserved_30           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_15;

/* Define the union U_PMV_TMV_16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_16_ver        : 12  ; /* [11..0]  */
        unsigned int    Reserved_33           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_16_hor        : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_16_vld        : 1   ; /* [29]  */
        unsigned int    Reserved_32           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_16;

/* Define the union U_PMV_TMV_17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_17_ver        : 12  ; /* [11..0]  */
        unsigned int    Reserved_35           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_17_hor        : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_17_vld        : 1   ; /* [29]  */
        unsigned int    Reserved_34           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_17;

/* Define the union U_PMV_TMV_18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_18_ver        : 12  ; /* [11..0]  */
        unsigned int    Reserved_37           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_18_hor        : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_18_vld        : 1   ; /* [29]  */
        unsigned int    Reserved_36           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_18;

/* Define the union U_PMV_TMV_19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_19_ver        : 12  ; /* [11..0]  */
        unsigned int    Reserved_39           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_19_hor        : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_19_vld        : 1   ; /* [29]  */
        unsigned int    Reserved_38           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_19;

/* Define the union U_PMV_TMV_20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_0_ver_1       : 12  ; /* [11..0]  */
        unsigned int    Reserved_41           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_0_hor_1       : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_0_vld_1       : 1   ; /* [29]  */
        unsigned int    Reserved_40           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_20;

/* Define the union U_PMV_TMV_21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_1_ver_1       : 12  ; /* [11..0]  */
        unsigned int    Reserved_43           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_1_hor_1       : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_1_vld_1       : 1   ; /* [29]  */
        unsigned int    Reserved_42           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_21;

/* Define the union U_PMV_TMV_22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_2_ver_1       : 12  ; /* [11..0]  */
        unsigned int    Reserved_45           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_2_hor_1       : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_2_vld_1       : 1   ; /* [29]  */
        unsigned int    Reserved_44           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_22;

/* Define the union U_PMV_TMV_23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_3_ver_1       : 12  ; /* [11..0]  */
        unsigned int    Reserved_47           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_3_hor_1       : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_3_vld_1       : 1   ; /* [29]  */
        unsigned int    Reserved_46           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_23;

/* Define the union U_PMV_TMV_24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_4_ver_1       : 12  ; /* [11..0]  */
        unsigned int    Reserved_49           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_4_hor_1       : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_4_vld_1       : 1   ; /* [29]  */
        unsigned int    Reserved_48           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_24;

/* Define the union U_PMV_TMV_25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_5_ver_1       : 12  ; /* [11..0]  */
        unsigned int    Reserved_51           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_5_hor_1       : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_5_vld_1       : 1   ; /* [29]  */
        unsigned int    Reserved_50           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_25;

/* Define the union U_PMV_TMV_26 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_6_ver_1       : 12  ; /* [11..0]  */
        unsigned int    Reserved_53           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_6_hor_1       : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_6_vld_1       : 1   ; /* [29]  */
        unsigned int    Reserved_52           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_26;

/* Define the union U_PMV_TMV_27 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_7_ver_1       : 12  ; /* [11..0]  */
        unsigned int    Reserved_55           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_7_hor_1       : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_7_vld_1       : 1   ; /* [29]  */
        unsigned int    Reserved_54           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_27;

/* Define the union U_PMV_TMV_28 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_8_ver_1       : 12  ; /* [11..0]  */
        unsigned int    Reserved_57           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_8_hor_1       : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_8_vld_1       : 1   ; /* [29]  */
        unsigned int    Reserved_56           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_28;

/* Define the union U_PMV_TMV_29 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_9_ver_1       : 12  ; /* [11..0]  */
        unsigned int    Reserved_59           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_9_hor_1       : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_9_vld_1       : 1   ; /* [29]  */
        unsigned int    Reserved_58           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_29;

/* Define the union U_PMV_TMV_30 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_10_ver_1      : 12  ; /* [11..0]  */
        unsigned int    Reserved_61           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_10_hor_1      : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_10_vld_1      : 1   ; /* [29]  */
        unsigned int    Reserved_60           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_30;

/* Define the union U_PMV_TMV_31 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_11_ver_1      : 12  ; /* [11..0]  */
        unsigned int    Reserved_63           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_11_hor_1      : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_11_vld_1      : 1   ; /* [29]  */
        unsigned int    Reserved_62           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_31;

/* Define the union U_PMV_TMV_32 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_12_ver_1      : 12  ; /* [11..0]  */
        unsigned int    Reserved_65           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_12_hor_1      : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_12_vld_1      : 1   ; /* [29]  */
        unsigned int    Reserved_64           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_32;

/* Define the union U_PMV_TMV_33 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_13_ver_1      : 12  ; /* [11..0]  */
        unsigned int    Reserved_67           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_13_hor_1      : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_13_vld_1      : 1   ; /* [29]  */
        unsigned int    Reserved_66           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_33;

/* Define the union U_PMV_TMV_34 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_14_ver_1      : 12  ; /* [11..0]  */
        unsigned int    Reserved_69           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_14_hor_1      : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_14_vld_1      : 1   ; /* [29]  */
        unsigned int    Reserved_68           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_34;

/* Define the union U_PMV_TMV_35 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_15_ver_1      : 12  ; /* [11..0]  */
        unsigned int    Reserved_71           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_15_hor_1      : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_15_vld_1      : 1   ; /* [29]  */
        unsigned int    Reserved_70           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_35;

/* Define the union U_PMV_TMV_36 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_16_ver_1      : 12  ; /* [11..0]  */
        unsigned int    Reserved_73           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_16_hor_1      : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_16_vld_1      : 1   ; /* [29]  */
        unsigned int    Reserved_72           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_36;

/* Define the union U_PMV_TMV_37 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_17_ver_1      : 12  ; /* [11..0]  */
        unsigned int    Reserved_75           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_17_hor_1      : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_17_vld_1      : 1   ; /* [29]  */
        unsigned int    Reserved_74           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_37;

/* Define the union U_PMV_TMV_38 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_18_ver_1      : 12  ; /* [11..0]  */
        unsigned int    Reserved_77           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_18_hor_1      : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_18_vld_1      : 1   ; /* [29]  */
        unsigned int    Reserved_76           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_38;

/* Define the union U_PMV_TMV_39 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_19_ver_1      : 12  ; /* [11..0]  */
        unsigned int    Reserved_79           : 4   ; /* [15..12]  */
        unsigned int    pmv_tmv_19_hor_1      : 13  ; /* [28..16]  */
        unsigned int    pmv_tmv_19_vld_1      : 1   ; /* [29]  */
        unsigned int    Reserved_78           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_39;

/* Define the union U_PMV_TMV_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_en            : 1   ; /* [0]  */
        unsigned int    Reserved_80           : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PMV_TMV_EN;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_PMV_CNT_FRAME        PMV_CNT_FRAME;
    volatile U_PMV_TMV_0            PMV_TMV_0;
    volatile U_PMV_TMV_1            PMV_TMV_1;
    volatile U_PMV_TMV_2            PMV_TMV_2;
    volatile U_PMV_TMV_3            PMV_TMV_3;
    volatile U_PMV_TMV_4            PMV_TMV_4;
    volatile U_PMV_TMV_5            PMV_TMV_5;
    volatile U_PMV_TMV_6            PMV_TMV_6;
    volatile U_PMV_TMV_7            PMV_TMV_7;
    volatile U_PMV_TMV_8            PMV_TMV_8;
    volatile U_PMV_TMV_9            PMV_TMV_9;
    volatile U_PMV_TMV_10           PMV_TMV_10;
    volatile U_PMV_TMV_11           PMV_TMV_11;
    volatile U_PMV_TMV_12           PMV_TMV_12;
    volatile U_PMV_TMV_13           PMV_TMV_13;
    volatile U_PMV_TMV_14           PMV_TMV_14;
    volatile U_PMV_TMV_15           PMV_TMV_15;
    volatile U_PMV_TMV_16           PMV_TMV_16;
    volatile U_PMV_TMV_17           PMV_TMV_17;
    volatile U_PMV_TMV_18           PMV_TMV_18;
    volatile U_PMV_TMV_19           PMV_TMV_19;
    volatile U_PMV_TMV_20           PMV_TMV_20;
    volatile U_PMV_TMV_21           PMV_TMV_21;
    volatile U_PMV_TMV_22           PMV_TMV_22;
    volatile U_PMV_TMV_23           PMV_TMV_23;
    volatile U_PMV_TMV_24           PMV_TMV_24;
    volatile U_PMV_TMV_25           PMV_TMV_25;
    volatile U_PMV_TMV_26           PMV_TMV_26;
    volatile U_PMV_TMV_27           PMV_TMV_27;
    volatile U_PMV_TMV_28           PMV_TMV_28;
    volatile U_PMV_TMV_29           PMV_TMV_29;
    volatile U_PMV_TMV_30           PMV_TMV_30;
    volatile U_PMV_TMV_31           PMV_TMV_31;
    volatile U_PMV_TMV_32           PMV_TMV_32;
    volatile U_PMV_TMV_33           PMV_TMV_33;
    volatile U_PMV_TMV_34           PMV_TMV_34;
    volatile U_PMV_TMV_35           PMV_TMV_35;
    volatile U_PMV_TMV_36           PMV_TMV_36;
    volatile U_PMV_TMV_37           PMV_TMV_37;
    volatile U_PMV_TMV_38           PMV_TMV_38;
    volatile U_PMV_TMV_39           PMV_TMV_39;
    volatile unsigned int           PMV_POC_0;
    volatile unsigned int           PMV_POC_1;
    volatile unsigned int           PMV_POC_2;
    volatile unsigned int           PMV_POC_3;
    volatile unsigned int           PMV_POC_4;
    volatile unsigned int           PMV_POC_5;
    volatile U_PMV_TMV_EN           PMV_TMV_EN;
    volatile unsigned int           RESERVED0[80];


} S_pmv_REGS_TYPE;

/* Declare the struct pointor of the module pmv */
extern volatile S_pmv_REGS_TYPE *goppmvAllReg;

/* Declare the functions that set the member value */
int iSetPMV_POC_0pmv_curr_pic_poc(unsigned int upmv_curr_pic_poc);
int iSetPMV_POC_1pmv_curr_pic_ref0_poc(unsigned int upmv_curr_pic_ref0_poc);
int iSetPMV_POC_2pmv_curr_pic_ref1_poc(unsigned int upmv_curr_pic_ref1_poc);
int iSetPMV_POC_3pmv_curr_col_poc(unsigned int upmv_curr_col_poc);
int iSetPMV_POC_4pmv_curr_col_ref0_poc(unsigned int upmv_curr_col_ref0_poc);
int iSetPMV_POC_5pmv_curr_col_ref1_poc(unsigned int upmv_curr_col_ref1_poc);
int iSetPMV_TMV_ENpmv_tmv_en(unsigned int upmv_tmv_en);



#endif /* __C_UNION_DEFINE_PMV_H__ */
