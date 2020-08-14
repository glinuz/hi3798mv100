//******************************************************************************
//  Copyright (C), 2007-2015, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define.h
// Version       : 2.0
// Author        : x57490
// Created       : 2015-01-13
// Last Modified : 
// Description   :  The C union definition file for the module CAS
// Function List : 
// History       : 
// 1 Date        : 
// Author        : x57490
// Modification  : Create file
//******************************************************************************

#ifndef __REG_VPSS_WBC_H__
#define __REG_VPSS_WBC_H__

// Define the union U_VPSS_PDFRMITDIFF
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int    blk_pix0              : 8   ; // [7..0] 
        unsigned int    blk_pix1              : 8   ; // [15..8] 
        unsigned int    blk_pix2              : 8   ; // [23..16] 
        unsigned int    blk_pix3              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK0TOL0;

// Define the union U_STR_DET_VIDBLK0TOL1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix4              : 8   ; // [7..0] 
        unsigned int    blk_pix5              : 8   ; // [15..8] 
        unsigned int    blk_pix6              : 8   ; // [23..16] 
        unsigned int    blk_pix7              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK0TOL1;

// Define the union U_STR_DET_VIDBLK1TOL0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix0              : 8   ; // [7..0] 
        unsigned int    blk_pix1              : 8   ; // [15..8] 
        unsigned int    blk_pix2              : 8   ; // [23..16] 
        unsigned int    blk_pix3              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK1TOL0;

// Define the union U_STR_DET_VIDBLK1TOL1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix4              : 8   ; // [7..0] 
        unsigned int    blk_pix5              : 8   ; // [15..8] 
        unsigned int    blk_pix6              : 8   ; // [23..16] 
        unsigned int    blk_pix7              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK1TOL1;

// Define the union U_STR_DET_VIDBLK2TOL0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix0              : 8   ; // [7..0] 
        unsigned int    blk_pix1              : 8   ; // [15..8] 
        unsigned int    blk_pix2              : 8   ; // [23..16] 
        unsigned int    blk_pix3              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK2TOL0;

// Define the union U_STR_DET_VIDBLK2TOL1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix4              : 8   ; // [7..0] 
        unsigned int    blk_pix5              : 8   ; // [15..8] 
        unsigned int    blk_pix6              : 8   ; // [23..16] 
        unsigned int    blk_pix7              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK2TOL1;

// Define the union U_STR_DET_VIDBLK3TOL0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix0              : 8   ; // [7..0] 
        unsigned int    blk_pix1              : 8   ; // [15..8] 
        unsigned int    blk_pix2              : 8   ; // [23..16] 
        unsigned int    blk_pix3              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK3TOL0;

// Define the union U_STR_DET_VIDBLK3TOL1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix4              : 8   ; // [7..0] 
        unsigned int    blk_pix5              : 8   ; // [15..8] 
        unsigned int    blk_pix6              : 8   ; // [23..16] 
        unsigned int    blk_pix7              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK3TOL1;

// Define the union U_STR_DET_VIDBLK4TOL0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix0              : 8   ; // [7..0] 
        unsigned int    blk_pix1              : 8   ; // [15..8] 
        unsigned int    blk_pix2              : 8   ; // [23..16] 
        unsigned int    blk_pix3              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK4TOL0;

// Define the union U_STR_DET_VIDBLK4TOL1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix4              : 8   ; // [7..0] 
        unsigned int    blk_pix5              : 8   ; // [15..8] 
        unsigned int    blk_pix6              : 8   ; // [23..16] 
        unsigned int    blk_pix7              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK4TOL1;

// Define the union U_STR_DET_VIDBLK5TOL0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix0              : 8   ; // [7..0] 
        unsigned int    blk_pix1              : 8   ; // [15..8] 
        unsigned int    blk_pix2              : 8   ; // [23..16] 
        unsigned int    blk_pix3              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK5TOL0;

// Define the union U_STR_DET_VIDBLK5TOL1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix4              : 8   ; // [7..0] 
        unsigned int    blk_pix5              : 8   ; // [15..8] 
        unsigned int    blk_pix6              : 8   ; // [23..16] 
        unsigned int    blk_pix7              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK5TOL1;

// Define the union U_DB_HY_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hy_size               : 7   ; // [6..0] 
        unsigned int    Reserved_7            : 9   ; // [15..7] 
        unsigned int    hy_counter            : 12  ; // [27..16] 
        unsigned int    Reserved_6            : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HY_SIZE;

// Define the union U_DB_HC_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hc_size               : 7   ; // [6..0] 
        unsigned int    Reserved_10           : 9   ; // [15..7] 
        unsigned int    hc_counter            : 12  ; // [27..16] 
        unsigned int    Reserved_9            : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HC_SIZE;

// Define the union U_DB_VY_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vy_size               : 7   ; // [6..0] 
        unsigned int    Reserved_12           : 9   ; // [15..7] 
        unsigned int    vy_counter            : 12  ; // [27..16] 
        unsigned int    Reserved_11           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_VY_SIZE;

// Define the union U_DB_MAX_INDEX
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hy_val_idx            : 3   ; // [2..0] 
        unsigned int    Reserved_20           : 1   ; // [3] 
        unsigned int    hy_wgt_idx            : 3   ; // [6..4] 
        unsigned int    Reserved_19           : 1   ; // [7] 
        unsigned int    vy_val_idx            : 3   ; // [10..8] 
        unsigned int    Reserved_18           : 1   ; // [11] 
        unsigned int    vy_wgt_idx            : 3   ; // [14..12] 
        unsigned int    Reserved_17           : 1   ; // [15] 
        unsigned int    hu_val_idx            : 3   ; // [18..16] 
        unsigned int    Reserved_16           : 1   ; // [19] 
        unsigned int    hu_wgt_idx            : 3   ; // [22..20] 
        unsigned int    Reserved_15           : 1   ; // [23] 
        unsigned int    hv_val_idx            : 3   ; // [26..24] 
        unsigned int    Reserved_14           : 1   ; // [27] 
        unsigned int    hv_wgt_idx            : 3   ; // [30..28] 
        unsigned int    Reserved_13           : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_MAX_INDEX;

// Define the union U_DB_HY_SIZE_IDX0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hy_size_idx0          : 7   ; // [6..0] 
        unsigned int    Reserved_22           : 9   ; // [15..7] 
        unsigned int    hy_counter_idx0       : 12  ; // [27..16] 
        unsigned int    Reserved_21           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HY_SIZE_IDX0;

// Define the union U_DB_HY_SIZE_IDX1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hy_size_idx1          : 7   ; // [6..0] 
        unsigned int    Reserved_24           : 9   ; // [15..7] 
        unsigned int    hy_counter_idx1       : 12  ; // [27..16] 
        unsigned int    Reserved_23           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HY_SIZE_IDX1;

// Define the union U_DB_HY_SIZE_IDX2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hy_size_idx2          : 7   ; // [6..0] 
        unsigned int    Reserved_26           : 9   ; // [15..7] 
        unsigned int    hy_counter_idx2       : 12  ; // [27..16] 
        unsigned int    Reserved_25           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HY_SIZE_IDX2;

// Define the union U_DB_HY_SIZE_IDX3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hy_size_idx3          : 7   ; // [6..0] 
        unsigned int    Reserved_28           : 9   ; // [15..7] 
        unsigned int    hy_counter_idx3       : 12  ; // [27..16] 
        unsigned int    Reserved_27           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HY_SIZE_IDX3;

// Define the union U_DB_HY_SIZE_IDX4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hy_size_idx4          : 7   ; // [6..0] 
        unsigned int    Reserved_30           : 9   ; // [15..7] 
        unsigned int    hy_counter_idx4       : 12  ; // [27..16] 
        unsigned int    Reserved_29           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HY_SIZE_IDX4;

// Define the union U_DB_HY_SIZE_IDX5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hy_size_idx5          : 7   ; // [6..0] 
        unsigned int    Reserved_32           : 9   ; // [15..7] 
        unsigned int    hy_counter_idx5       : 12  ; // [27..16] 
        unsigned int    Reserved_31           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HY_SIZE_IDX5;

// Define the union U_DB_HY_SIZE_IDX6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hy_size_idx6          : 7   ; // [6..0] 
        unsigned int    Reserved_34           : 9   ; // [15..7] 
        unsigned int    hy_counter_idx6       : 12  ; // [27..16] 
        unsigned int    Reserved_33           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HY_SIZE_IDX6;

// Define the union U_DB_HY_SIZE_IDX7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vy_size_idx7          : 7   ; // [6..0] 
        unsigned int    Reserved_36           : 9   ; // [15..7] 
        unsigned int    vy_counter_idx7       : 12  ; // [27..16] 
        unsigned int    Reserved_35           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HY_SIZE_IDX7;

// Define the union U_DB_VY_SIZE_IDX0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vy_size_idx0          : 7   ; // [6..0] 
        unsigned int    Reserved_38           : 9   ; // [15..7] 
        unsigned int    vy_counter_idx0       : 12  ; // [27..16] 
        unsigned int    Reserved_37           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_VY_SIZE_IDX0;

// Define the union U_DB_VY_SIZE_IDX1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vy_size_idx1          : 7   ; // [6..0] 
        unsigned int    Reserved_40           : 9   ; // [15..7] 
        unsigned int    vy_counter_idx1       : 12  ; // [27..16] 
        unsigned int    Reserved_39           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_VY_SIZE_IDX1;

// Define the union U_DB_VY_SIZE_IDX2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vy_size_idx2          : 7   ; // [6..0] 
        unsigned int    Reserved_42           : 9   ; // [15..7] 
        unsigned int    vy_counter_idx2       : 12  ; // [27..16] 
        unsigned int    Reserved_41           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_VY_SIZE_IDX2;

// Define the union U_DB_VY_SIZE_IDX3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vy_size_idx3          : 7   ; // [6..0] 
        unsigned int    Reserved_44           : 9   ; // [15..7] 
        unsigned int    vy_counter_idx3       : 12  ; // [27..16] 
        unsigned int    Reserved_43           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_VY_SIZE_IDX3;

// Define the union U_DB_VY_SIZE_IDX4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vy_size_idx4          : 7   ; // [6..0] 
        unsigned int    Reserved_46           : 9   ; // [15..7] 
        unsigned int    vy_counter_idx4       : 12  ; // [27..16] 
        unsigned int    Reserved_45           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_VY_SIZE_IDX4;

// Define the union U_DB_VY_SIZE_IDX5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vy_size_idx5          : 7   ; // [6..0] 
        unsigned int    Reserved_48           : 9   ; // [15..7] 
        unsigned int    vy_counter_idx5       : 12  ; // [27..16] 
        unsigned int    Reserved_47           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_VY_SIZE_IDX5;

// Define the union U_DB_VY_SIZE_IDX6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vy_size_idx6          : 7   ; // [6..0] 
        unsigned int    Reserved_50           : 9   ; // [15..7] 
        unsigned int    vy_counter_idx6       : 12  ; // [27..16] 
        unsigned int    Reserved_49           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_VY_SIZE_IDX6;

// Define the union U_DB_VY_SIZE_IDX7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vy_size_idx7          : 7   ; // [6..0] 
        unsigned int    Reserved_52           : 9   ; // [15..7] 
        unsigned int    vy_counter_idx7       : 12  ; // [27..16] 
        unsigned int    Reserved_51           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_VY_SIZE_IDX7;

// Define the union U_DB_HU_SIZE_IDX0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hu_size_idx0          : 7   ; // [6..0] 
        unsigned int    Reserved_54           : 9   ; // [15..7] 
        unsigned int    hu_counter_idx0       : 12  ; // [27..16] 
        unsigned int    Reserved_53           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HU_SIZE_IDX0;

// Define the union U_DB_HU_SIZE_IDX1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hu_size_idx1          : 7   ; // [6..0] 
        unsigned int    Reserved_56           : 9   ; // [15..7] 
        unsigned int    hu_counter_idx1       : 12  ; // [27..16] 
        unsigned int    Reserved_55           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HU_SIZE_IDX1;

// Define the union U_DB_HU_SIZE_IDX2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hu_size_idx2          : 7   ; // [6..0] 
        unsigned int    Reserved_58           : 9   ; // [15..7] 
        unsigned int    hu_counter_idx2       : 12  ; // [27..16] 
        unsigned int    Reserved_57           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HU_SIZE_IDX2;

// Define the union U_DB_HU_SIZE_IDX3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hu_size_idx3          : 7   ; // [6..0] 
        unsigned int    Reserved_60           : 9   ; // [15..7] 
        unsigned int    hu_counter_idx3       : 12  ; // [27..16] 
        unsigned int    Reserved_59           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HU_SIZE_IDX3;

// Define the union U_DB_HU_SIZE_IDX4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hu_size_idx4          : 7   ; // [6..0] 
        unsigned int    Reserved_62           : 9   ; // [15..7] 
        unsigned int    hu_counter_idx4       : 12  ; // [27..16] 
        unsigned int    Reserved_61           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HU_SIZE_IDX4;

// Define the union U_DB_HU_SIZE_IDX5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hu_size_idx5          : 7   ; // [6..0] 
        unsigned int    Reserved_64           : 9   ; // [15..7] 
        unsigned int    hu_counter_idx5       : 12  ; // [27..16] 
        unsigned int    Reserved_63           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HU_SIZE_IDX5;

// Define the union U_DB_HU_SIZE_IDX6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hu_size_idx6          : 7   ; // [6..0] 
        unsigned int    Reserved_66           : 9   ; // [15..7] 
        unsigned int    hu_counter_idx6       : 12  ; // [27..16] 
        unsigned int    Reserved_65           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HU_SIZE_IDX6;

// Define the union U_DB_HU_SIZE_IDX7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hv_size_idx7          : 7   ; // [6..0] 
        unsigned int    Reserved_68           : 9   ; // [15..7] 
        unsigned int    hv_counter_idx7       : 12  ; // [27..16] 
        unsigned int    Reserved_67           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HU_SIZE_IDX7;

// Define the union U_DB_HV_SIZE_IDX0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hv_size_idx0          : 7   ; // [6..0] 
        unsigned int    Reserved_70           : 9   ; // [15..7] 
        unsigned int    hv_counter_idx0       : 12  ; // [27..16] 
        unsigned int    Reserved_69           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HV_SIZE_IDX0;

// Define the union U_DB_HV_SIZE_IDX1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hv_size_idx1          : 7   ; // [6..0] 
        unsigned int    Reserved_72           : 9   ; // [15..7] 
        unsigned int    hv_counter_idx1       : 12  ; // [27..16] 
        unsigned int    Reserved_71           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HV_SIZE_IDX1;

// Define the union U_DB_HV_SIZE_IDX2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hv_size_idx2          : 7   ; // [6..0] 
        unsigned int    Reserved_74           : 9   ; // [15..7] 
        unsigned int    hv_counter_idx2       : 12  ; // [27..16] 
        unsigned int    Reserved_73           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HV_SIZE_IDX2;

// Define the union U_DB_HV_SIZE_IDX3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hv_size_idx3          : 7   ; // [6..0] 
        unsigned int    Reserved_76           : 9   ; // [15..7] 
        unsigned int    hv_counter_idx3       : 12  ; // [27..16] 
        unsigned int    Reserved_75           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HV_SIZE_IDX3;

// Define the union U_DB_HV_SIZE_IDX4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hv_size_idx4          : 7   ; // [6..0] 
        unsigned int    Reserved_78           : 9   ; // [15..7] 
        unsigned int    hv_counter_idx4       : 12  ; // [27..16] 
        unsigned int    Reserved_77           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HV_SIZE_IDX4;

// Define the union U_DB_HV_SIZE_IDX5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hv_size_idx5          : 7   ; // [6..0] 
        unsigned int    Reserved_80           : 9   ; // [15..7] 
        unsigned int    hv_counter_idx5       : 12  ; // [27..16] 
        unsigned int    Reserved_79           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HV_SIZE_IDX5;

// Define the union U_DB_HV_SIZE_IDX6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hv_size_idx6          : 7   ; // [6..0] 
        unsigned int    Reserved_82           : 9   ; // [15..7] 
        unsigned int    hv_counter_idx6       : 12  ; // [27..16] 
        unsigned int    Reserved_81           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HV_SIZE_IDX6;

// Define the union U_DB_HV_SIZE_IDX7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hv_size_idx7          : 7   ; // [6..0] 
        unsigned int    Reserved_84           : 9   ; // [15..7] 
        unsigned int    hv_counter_idx7       : 12  ; // [27..16] 
        unsigned int    Reserved_83           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HV_SIZE_IDX7;

//==============================================================================
// Define the global struct
typedef struct
{
    volatile unsigned int           VPSS_PDFRMITDIFF;
    volatile unsigned int           VPSS_PDSTLBLKSAD[16];
    volatile unsigned int           VPSS_PDHISTBIN[4];
    volatile unsigned int           VPSS_PDUMMATCH0;
    volatile unsigned int           VPSS_PDUMNOMATCH0;
    volatile unsigned int           VPSS_PDUMMATCH1;
    volatile unsigned int           VPSS_PDUMNOMATCH1;
    volatile unsigned int           VPSS_PDPCCFFWD0;
    volatile unsigned int           VPSS_PDPCCFWD0;
    volatile unsigned int           VPSS_PDPCCBWD0;
    volatile unsigned int           VPSS_PDPCCCRSS0;
    volatile unsigned int           VPSS_PDPCCFWDTKR0;
    volatile unsigned int           VPSS_PDPCCBWDTKR0;
    volatile unsigned int           VPSS_PDPCCFFWD1;
    volatile unsigned int           VPSS_PDPCCFWD1;
    volatile unsigned int           VPSS_PDPCCBWD1;
    volatile unsigned int           VPSS_PDPCCCRSS1;
    volatile unsigned int           VPSS_PDPCCFWDTKR1;
    volatile unsigned int           VPSS_PDPCCBWDTKR1;
    volatile unsigned int           VPSS_PDLASICNT140;
    volatile unsigned int           VPSS_PDLASICNT320;
    volatile unsigned int           VPSS_PDLASICNT340;
    volatile unsigned int           VPSS_PDLASICNT141;
    volatile unsigned int           VPSS_PDLASICNT321;
    volatile unsigned int           VPSS_PDLASICNT341;
    volatile unsigned int           VPSS_PDICHD;
    volatile unsigned int           Reserved_0[20];
    volatile unsigned int           IGLB_MTN_BIN0;
    volatile unsigned int           IGLB_MTN_BIN1;
    volatile unsigned int           IGLB_MTN_BIN2;
    volatile unsigned int           IGLB_MTN_BIN3;
    volatile unsigned int           IGLB_MTN_BIN4;
    volatile unsigned int           IGLB_MTN_BIN5;
    volatile unsigned int           IGLB_MTN_BIN6;
    volatile unsigned int           IGLB_MTN_BIN7;
    volatile unsigned int           IGLB_MTN_BIN8;
    volatile unsigned int           IGLB_MTN_BIN9;
    volatile unsigned int           IGLB_MTN_BIN10;
    volatile unsigned int           IGLB_MTN_BIN11;
    volatile unsigned int           IGLB_MTN_BIN12;
    volatile unsigned int           IGLB_MTN_BIN13;
    volatile unsigned int           IGLB_MTN_BIN14;
    volatile unsigned int           IGLB_MTN_BIN15;
    volatile unsigned int           IGLB_MTN_BIN16;
    volatile unsigned int           IGLB_MTN_BIN17;
    volatile unsigned int           IGLB_MTN_BIN18;
    volatile unsigned int           IGLB_MTN_BIN19;
    volatile unsigned int           IGLB_MTN_BIN20;
    volatile unsigned int           IGLB_MTN_BIN21;
    volatile unsigned int           IGLB_MTN_BIN22;
    volatile unsigned int           IGLB_MTN_BIN23;
    volatile unsigned int           IGLB_MTN_BIN24;
    volatile unsigned int           IGLB_MTN_BIN25;
    volatile unsigned int           IGLB_MTN_BIN26;
    volatile unsigned int           IGLB_MTN_BIN27;
    volatile unsigned int           IGLB_MTN_BIN28;
    volatile unsigned int           IGLB_MTN_BIN29;
    volatile unsigned int           IGLB_MTN_BIN30;
    volatile unsigned int           IGLB_MTN_BIN31;
    volatile unsigned int           IGLB_MTN_NUM0;
    volatile unsigned int           IGLB_MTN_SUM0;
    volatile unsigned int           IGLB_MTN_NUM1;
    volatile unsigned int           IGLB_MTN_SUM1;
    volatile unsigned int           IGLB_MTN_NUM2;
    volatile unsigned int           IGLB_MTN_SUM2;
    volatile unsigned int           IGLB_MTN_NUM3;
    volatile unsigned int           IGLB_MTN_SUM3;
    volatile unsigned int           IGLB_MTN_NUM4;
    volatile unsigned int           IGLB_MTN_SUM4;
    volatile unsigned int           Reserved_1[22];
    volatile U_STR_DET_VIDBLK0TOL0   STR_DET_VIDBLK0TOL0;
    volatile U_STR_DET_VIDBLK0TOL1   STR_DET_VIDBLK0TOL1;
    volatile U_STR_DET_VIDBLK1TOL0   STR_DET_VIDBLK1TOL0;
    volatile U_STR_DET_VIDBLK1TOL1   STR_DET_VIDBLK1TOL1;
    volatile U_STR_DET_VIDBLK2TOL0   STR_DET_VIDBLK2TOL0;
    volatile U_STR_DET_VIDBLK2TOL1   STR_DET_VIDBLK2TOL1;
    volatile U_STR_DET_VIDBLK3TOL0   STR_DET_VIDBLK3TOL0;
    volatile U_STR_DET_VIDBLK3TOL1   STR_DET_VIDBLK3TOL1;
    volatile U_STR_DET_VIDBLK4TOL0   STR_DET_VIDBLK4TOL0;
    volatile U_STR_DET_VIDBLK4TOL1   STR_DET_VIDBLK4TOL1;
    volatile U_STR_DET_VIDBLK5TOL0   STR_DET_VIDBLK5TOL0;
    volatile U_STR_DET_VIDBLK5TOL1   STR_DET_VIDBLK5TOL1;
    volatile unsigned int           Reserved_2[52];
    volatile unsigned int           DB_HY_COUNTER[1920];
    volatile unsigned int           DB_BORDER[128];
    volatile U_DB_HY_SIZE           DB_HY_SIZE;
    volatile U_DB_HC_SIZE           DB_HC_SIZE;
    volatile U_DB_VY_SIZE           DB_VY_SIZE;
    U_DB_MAX_INDEX       DB_MAX_INDEX;
    U_DB_HY_SIZE_IDX0    DB_HY_SIZE_IDX0;
    U_DB_HY_SIZE_IDX1    DB_HY_SIZE_IDX1;
    U_DB_HY_SIZE_IDX2    DB_HY_SIZE_IDX2;
    U_DB_HY_SIZE_IDX3    DB_HY_SIZE_IDX3;
    U_DB_HY_SIZE_IDX4    DB_HY_SIZE_IDX4;
    U_DB_HY_SIZE_IDX5    DB_HY_SIZE_IDX5;
    U_DB_HY_SIZE_IDX6    DB_HY_SIZE_IDX6;
    U_DB_HY_SIZE_IDX7    DB_HY_SIZE_IDX7;
    U_DB_VY_SIZE_IDX0    DB_VY_SIZE_IDX0;
    U_DB_VY_SIZE_IDX1    DB_VY_SIZE_IDX1;
    U_DB_VY_SIZE_IDX2    DB_VY_SIZE_IDX2;
    U_DB_VY_SIZE_IDX3    DB_VY_SIZE_IDX3;
    U_DB_VY_SIZE_IDX4    DB_VY_SIZE_IDX4;
    U_DB_VY_SIZE_IDX5    DB_VY_SIZE_IDX5;
    U_DB_VY_SIZE_IDX6    DB_VY_SIZE_IDX6;
    U_DB_VY_SIZE_IDX7    DB_VY_SIZE_IDX7;
    U_DB_HU_SIZE_IDX0    DB_HU_SIZE_IDX0;
    U_DB_HU_SIZE_IDX1    DB_HU_SIZE_IDX1;
    U_DB_HU_SIZE_IDX2    DB_HU_SIZE_IDX2;
    U_DB_HU_SIZE_IDX3    DB_HU_SIZE_IDX3;
    U_DB_HU_SIZE_IDX4    DB_HU_SIZE_IDX4;
    U_DB_HU_SIZE_IDX5    DB_HU_SIZE_IDX5;
    U_DB_HU_SIZE_IDX6    DB_HU_SIZE_IDX6;
    U_DB_HU_SIZE_IDX7    DB_HU_SIZE_IDX7;
    U_DB_HV_SIZE_IDX0    DB_HV_SIZE_IDX0;
    U_DB_HV_SIZE_IDX1    DB_HV_SIZE_IDX1;
    U_DB_HV_SIZE_IDX2    DB_HV_SIZE_IDX2;
    U_DB_HV_SIZE_IDX3    DB_HV_SIZE_IDX3;
    U_DB_HV_SIZE_IDX4    DB_HV_SIZE_IDX4;
    U_DB_HV_SIZE_IDX5    DB_HV_SIZE_IDX5;
    U_DB_HV_SIZE_IDX6    DB_HV_SIZE_IDX6;
    U_DB_HV_SIZE_IDX7    DB_HV_SIZE_IDX7;


}S_VPSSWB_REGS_TYPE,S_STT_REGS_TYPE;

// Declare the struct pointor of the module CAS
//extern S_CAS_REGS_TYPE *gopCASAllReg;



#endif // __REG_VPSS_WBC_H__
