//******************************************************************************
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co., Ltd.
// File name     :  hi_reg_vpss_wbc.h
// Author        :  xxx
// Version       :  1.0
// Date          :  2015-07-06
// Description   :  Define all registers/tables for xxx
// Others        :  Generated automatically by nManager V4.0
// History       :  xxx 2015-07-06 Create file
//******************************************************************************

#ifndef __HI_REG_VPSS_WBC_H__
#define __HI_REG_VPSS_WBC_H__

/* Define the union U_DB_HY_COUNTER */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int db_hy_counter          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_DB_HY_COUNTER;
/* Define the union U_DB_BORDER */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int db_border              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_DB_BORDER;
/* Define the union U_DB_HY_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_size               : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hy_counter            : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HY_SIZE;

/* Define the union U_DB_HC_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hc_size               : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hc_counter            : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HC_SIZE;

/* Define the union U_DB_VY_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_size               : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    vy_counter            : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_VY_SIZE;

/* Define the union U_DB_MAX_INDEX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_val_idx            : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    hy_wgt_idx            : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    vy_val_idx            : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    vy_wgt_idx            : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 1   ; /* [15]  */
        unsigned int    hu_val_idx            : 3   ; /* [18..16]  */
        unsigned int    reserved_4            : 1   ; /* [19]  */
        unsigned int    hu_wgt_idx            : 3   ; /* [22..20]  */
        unsigned int    reserved_5            : 1   ; /* [23]  */
        unsigned int    hv_val_idx            : 3   ; /* [26..24]  */
        unsigned int    reserved_6            : 1   ; /* [27]  */
        unsigned int    hv_wgt_idx            : 3   ; /* [30..28]  */
        unsigned int    reserved_7            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_MAX_INDEX;

/* Define the union U_DB_HY_SIZE_IDX0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_size_idx0          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hy_counter_idx0       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HY_SIZE_IDX0;

/* Define the union U_DB_HY_SIZE_IDX1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_size_idx1          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hy_counter_idx1       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HY_SIZE_IDX1;

/* Define the union U_DB_HY_SIZE_IDX2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_size_idx2          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hy_counter_idx2       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HY_SIZE_IDX2;

/* Define the union U_DB_HY_SIZE_IDX3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_size_idx3          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hy_counter_idx3       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HY_SIZE_IDX3;

/* Define the union U_DB_HY_SIZE_IDX4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_size_idx4          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hy_counter_idx4       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HY_SIZE_IDX4;

/* Define the union U_DB_HY_SIZE_IDX5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_size_idx5          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hy_counter_idx5       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HY_SIZE_IDX5;

/* Define the union U_DB_HY_SIZE_IDX6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_size_idx6          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hy_counter_idx6       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HY_SIZE_IDX6;

/* Define the union U_DB_HY_SIZE_IDX7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_size_idx7          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    vy_counter_idx7       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HY_SIZE_IDX7;

/* Define the union U_DB_VY_SIZE_IDX0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_size_idx0          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    vy_counter_idx0       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_VY_SIZE_IDX0;

/* Define the union U_DB_VY_SIZE_IDX1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_size_idx1          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    vy_counter_idx1       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_VY_SIZE_IDX1;

/* Define the union U_DB_VY_SIZE_IDX2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_size_idx2          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    vy_counter_idx2       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_VY_SIZE_IDX2;

/* Define the union U_DB_VY_SIZE_IDX3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_size_idx3          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    vy_counter_idx3       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_VY_SIZE_IDX3;

/* Define the union U_DB_VY_SIZE_IDX4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_size_idx4          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    vy_counter_idx4       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_VY_SIZE_IDX4;

/* Define the union U_DB_VY_SIZE_IDX5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_size_idx5          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    vy_counter_idx5       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_VY_SIZE_IDX5;

/* Define the union U_DB_VY_SIZE_IDX6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_size_idx6          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    vy_counter_idx6       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_VY_SIZE_IDX6;

/* Define the union U_DB_VY_SIZE_IDX7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_size_idx7          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    vy_counter_idx7       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_VY_SIZE_IDX7;

/* Define the union U_DB_HU_SIZE_IDX0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hu_size_idx0          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hu_counter_idx0       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HU_SIZE_IDX0;

/* Define the union U_DB_HU_SIZE_IDX1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hu_size_idx1          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hu_counter_idx1       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HU_SIZE_IDX1;

/* Define the union U_DB_HU_SIZE_IDX2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hu_size_idx2          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hu_counter_idx2       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HU_SIZE_IDX2;

/* Define the union U_DB_HU_SIZE_IDX3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hu_size_idx3          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hu_counter_idx3       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HU_SIZE_IDX3;

/* Define the union U_DB_HU_SIZE_IDX4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hu_size_idx4          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hu_counter_idx4       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HU_SIZE_IDX4;

/* Define the union U_DB_HU_SIZE_IDX5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hu_size_idx5          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hu_counter_idx5       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HU_SIZE_IDX5;

/* Define the union U_DB_HU_SIZE_IDX6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hu_size_idx6          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hu_counter_idx6       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HU_SIZE_IDX6;

/* Define the union U_DB_HU_SIZE_IDX7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hv_size_idx7          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hv_counter_idx7       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HU_SIZE_IDX7;

/* Define the union U_DB_HV_SIZE_IDX0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hv_size_idx0          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hv_counter_idx0       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HV_SIZE_IDX0;

/* Define the union U_DB_HV_SIZE_IDX1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hv_size_idx1          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hv_counter_idx1       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HV_SIZE_IDX1;

/* Define the union U_DB_HV_SIZE_IDX2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hv_size_idx2          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hv_counter_idx2       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HV_SIZE_IDX2;

/* Define the union U_DB_HV_SIZE_IDX3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hv_size_idx3          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hv_counter_idx3       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HV_SIZE_IDX3;

/* Define the union U_DB_HV_SIZE_IDX4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hv_size_idx4          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hv_counter_idx4       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HV_SIZE_IDX4;

/* Define the union U_DB_HV_SIZE_IDX5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hv_size_idx5          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hv_counter_idx5       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HV_SIZE_IDX5;

/* Define the union U_DB_HV_SIZE_IDX6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hv_size_idx6          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hv_counter_idx6       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HV_SIZE_IDX6;

/* Define the union U_DB_HV_SIZE_IDX7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hv_size_idx7          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hv_counter_idx7       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HV_SIZE_IDX7;

/* Define the union U_VPSS_PDFRMITDIFF */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int frm_it_diff            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDFRMITDIFF;
/* Define the union U_VPSS_PDSTLBLKSAD */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int stlblk_sad             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDSTLBLKSAD;
/* Define the union U_VPSS_PDHISTBIN */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hsit_bin               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDHISTBIN;
/* Define the union U_VPSS_PDUMMATCH0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int match_um               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDUMMATCH0;
/* Define the union U_VPSS_PDUMNOMATCH0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nomatch_um             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDUMNOMATCH0;
/* Define the union U_VPSS_PDUMMATCH1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int match_um               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDUMMATCH1;
/* Define the union U_VPSS_PDUMNOMATCH1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nomatch_um             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDUMNOMATCH1;
/* Define the union U_VPSS_PDPCCFFWD0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_ffwd_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCFFWD0;
/* Define the union U_VPSS_PDPCCFWD0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_fwd_0              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCFWD0;
/* Define the union U_VPSS_PDPCCBWD0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_bwd_0              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCBWD0;
/* Define the union U_VPSS_PDPCCCRSS0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_crss_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCCRSS0;
/* Define the union U_VPSS_PDPCCFWDTKR0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_fwd_tkr_0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCFWDTKR0;
/* Define the union U_VPSS_PDPCCBWDTKR0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_bwd_tkr_0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCBWDTKR0;
/* Define the union U_VPSS_PDPCCFFWD1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_ffwd_1             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCFFWD1;
/* Define the union U_VPSS_PDPCCFWD1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_fwd_1              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCFWD1;
/* Define the union U_VPSS_PDPCCBWD1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_bwd_1              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCBWD1;
/* Define the union U_VPSS_PDPCCCRSS1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_crss_1             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCCRSS1;
/* Define the union U_VPSS_PDPCCFWDTKR1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_fwd_tkr_1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCFWDTKR1;
/* Define the union U_VPSS_PDPCCBWDTKR1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pcc_bwd_tkr_1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDPCCBWDTKR1;
/* Define the union U_VPSS_PDLASICNT140 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lasi_14_cnt_0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDLASICNT140;
/* Define the union U_VPSS_PDLASICNT320 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lasi_32_cnt_0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDLASICNT320;
/* Define the union U_VPSS_PDLASICNT340 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lasi_34_cnt_0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDLASICNT340;
/* Define the union U_VPSS_PDLASICNT141 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lasi_14_cnt_1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDLASICNT141;
/* Define the union U_VPSS_PDLASICNT321 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lasi_32_cnt_1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDLASICNT321;
/* Define the union U_VPSS_PDLASICNT341 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lasi_34_cnt_1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDLASICNT341;
/* Define the union U_VPSS_PDICHD */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int ichd                   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PDICHD;
/* Define the union U_iglb_mtn_bin0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin0;
/* Define the union U_iglb_mtn_bin1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin1;
/* Define the union U_iglb_mtn_bin2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin2          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin2;
/* Define the union U_iglb_mtn_bin3 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin3          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin3;
/* Define the union U_iglb_mtn_bin4 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin4          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin4;
/* Define the union U_iglb_mtn_bin5 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin5          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin5;
/* Define the union U_iglb_mtn_bin6 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin6          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin6;
/* Define the union U_iglb_mtn_bin7 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin7          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin7;
/* Define the union U_iglb_mtn_bin8 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin8          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin8;
/* Define the union U_iglb_mtn_bin9 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin9          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin9;
/* Define the union U_iglb_mtn_bin10 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin10         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin10;
/* Define the union U_iglb_mtn_bin11 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin11         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin11;
/* Define the union U_iglb_mtn_bin12 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin12         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin12;
/* Define the union U_iglb_mtn_bin13 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin13         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin13;
/* Define the union U_iglb_mtn_bin14 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin14         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin14;
/* Define the union U_iglb_mtn_bin15 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin15         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin15;
/* Define the union U_iglb_mtn_bin16 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin16         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin16;
/* Define the union U_iglb_mtn_bin17 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin17         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin17;
/* Define the union U_iglb_mtn_bin18 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin18         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin18;
/* Define the union U_iglb_mtn_bin19 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin19         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin19;
/* Define the union U_iglb_mtn_bin20 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin20         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin20;
/* Define the union U_iglb_mtn_bin21 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin21         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin21;
/* Define the union U_iglb_mtn_bin22 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin22         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin22;
/* Define the union U_iglb_mtn_bin23 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin23         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin23;
/* Define the union U_iglb_mtn_bin24 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin24         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin24;
/* Define the union U_iglb_mtn_bin25 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin25         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin25;
/* Define the union U_iglb_mtn_bin26 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin26         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin26;
/* Define the union U_iglb_mtn_bin27 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin27         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin27;
/* Define the union U_iglb_mtn_bin28 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin28         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin28;
/* Define the union U_iglb_mtn_bin29 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin29         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin29;
/* Define the union U_iglb_mtn_bin30 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin30         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin30;
/* Define the union U_iglb_mtn_bin31 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_bin31         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_bin31;
/* Define the union U_iglb_mtn_num0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_num0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_num0;
/* Define the union U_iglb_mtn_sum0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_sum0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_sum0;
/* Define the union U_iglb_mtn_num1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_num1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_num1;
/* Define the union U_iglb_mtn_sum1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_sum1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_sum1;
/* Define the union U_iglb_mtn_num2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_num2          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_num2;
/* Define the union U_iglb_mtn_sum2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_sum2          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_sum2;
/* Define the union U_iglb_mtn_num3 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_num1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_num3;
/* Define the union U_iglb_mtn_sum3 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_sum1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_sum3;
/* Define the union U_iglb_mtn_num4 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_num2          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_num4;
/* Define the union U_iglb_mtn_sum4 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb_mtn_sum2          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb_mtn_sum4;
/* Define the union U_iglb1_mtn_num0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb1_mtn_num0         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb1_mtn_num0;
/* Define the union U_iglb1_mtn_num1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb1_mtn_num1         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb1_mtn_num1;
/* Define the union U_iglb1_mtn_num2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb1_mtn_num2         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb1_mtn_num2;
/* Define the union U_iglb1_mtn_num3 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb1_mtn_num3         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb1_mtn_num3;
/* Define the union U_iglb2_mtn_num0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb2_mtn_num0         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb2_mtn_num0;
/* Define the union U_iglb2_mtn_num1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb2_mtn_num1         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb2_mtn_num1;
/* Define the union U_iglb2_mtn_num2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb2_mtn_num2         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb2_mtn_num2;
/* Define the union U_iglb2_mtn_num3 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int iglb2_mtn_num3         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_iglb2_mtn_num3;
/* Define the union U_STR_DET_VIDBLK0TOL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix0              : 8   ; /* [7..0]  */
        unsigned int    blk_pix1              : 8   ; /* [15..8]  */
        unsigned int    blk_pix2              : 8   ; /* [23..16]  */
        unsigned int    blk_pix3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK0TOL0;

/* Define the union U_STR_DET_VIDBLK0TOL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix4              : 8   ; /* [7..0]  */
        unsigned int    blk_pix5              : 8   ; /* [15..8]  */
        unsigned int    blk_pix6              : 8   ; /* [23..16]  */
        unsigned int    blk_pix7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK0TOL1;

/* Define the union U_STR_DET_VIDBLK1TOL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix0              : 8   ; /* [7..0]  */
        unsigned int    blk_pix1              : 8   ; /* [15..8]  */
        unsigned int    blk_pix2              : 8   ; /* [23..16]  */
        unsigned int    blk_pix3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK1TOL0;

/* Define the union U_STR_DET_VIDBLK1TOL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix4              : 8   ; /* [7..0]  */
        unsigned int    blk_pix5              : 8   ; /* [15..8]  */
        unsigned int    blk_pix6              : 8   ; /* [23..16]  */
        unsigned int    blk_pix7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK1TOL1;

/* Define the union U_STR_DET_VIDBLK2TOL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix0              : 8   ; /* [7..0]  */
        unsigned int    blk_pix1              : 8   ; /* [15..8]  */
        unsigned int    blk_pix2              : 8   ; /* [23..16]  */
        unsigned int    blk_pix3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK2TOL0;

/* Define the union U_STR_DET_VIDBLK2TOL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix4              : 8   ; /* [7..0]  */
        unsigned int    blk_pix5              : 8   ; /* [15..8]  */
        unsigned int    blk_pix6              : 8   ; /* [23..16]  */
        unsigned int    blk_pix7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK2TOL1;

/* Define the union U_STR_DET_VIDBLK3TOL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix0              : 8   ; /* [7..0]  */
        unsigned int    blk_pix1              : 8   ; /* [15..8]  */
        unsigned int    blk_pix2              : 8   ; /* [23..16]  */
        unsigned int    blk_pix3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK3TOL0;

/* Define the union U_STR_DET_VIDBLK3TOL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix4              : 8   ; /* [7..0]  */
        unsigned int    blk_pix5              : 8   ; /* [15..8]  */
        unsigned int    blk_pix6              : 8   ; /* [23..16]  */
        unsigned int    blk_pix7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK3TOL1;

/* Define the union U_STR_DET_VIDBLK4TOL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix0              : 8   ; /* [7..0]  */
        unsigned int    blk_pix1              : 8   ; /* [15..8]  */
        unsigned int    blk_pix2              : 8   ; /* [23..16]  */
        unsigned int    blk_pix3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK4TOL0;

/* Define the union U_STR_DET_VIDBLK4TOL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix4              : 8   ; /* [7..0]  */
        unsigned int    blk_pix5              : 8   ; /* [15..8]  */
        unsigned int    blk_pix6              : 8   ; /* [23..16]  */
        unsigned int    blk_pix7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK4TOL1;

/* Define the union U_STR_DET_VIDBLK5TOL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix0              : 8   ; /* [7..0]  */
        unsigned int    blk_pix1              : 8   ; /* [15..8]  */
        unsigned int    blk_pix2              : 8   ; /* [23..16]  */
        unsigned int    blk_pix3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK5TOL0;

/* Define the union U_STR_DET_VIDBLK5TOL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_pix4              : 8   ; /* [7..0]  */
        unsigned int    blk_pix5              : 8   ; /* [15..8]  */
        unsigned int    blk_pix6              : 8   ; /* [23..16]  */
        unsigned int    blk_pix7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLK5TOL1;

//==============================================================================
/* Define the global struct */
typedef struct
{
    U_DB_HY_COUNTER        DB_HY_COUNTER[2048]               ; /* 0x0~0x1ffc */
    U_DB_BORDER            DB_BORDER[128]                    ; /* 0x2000~0x21fc */
    U_DB_HY_SIZE           DB_HY_SIZE                        ; /* 0x2200 */
    U_DB_HC_SIZE           DB_HC_SIZE                        ; /* 0x2204 */
    U_DB_VY_SIZE           DB_VY_SIZE                        ; /* 0x2208 */
    U_DB_MAX_INDEX         DB_MAX_INDEX                      ; /* 0x220c */
    U_DB_HY_SIZE_IDX0      DB_HY_SIZE_IDX0                   ; /* 0x2210 */
    U_DB_HY_SIZE_IDX1      DB_HY_SIZE_IDX1                   ; /* 0x2214 */
    U_DB_HY_SIZE_IDX2      DB_HY_SIZE_IDX2                   ; /* 0x2218 */
    U_DB_HY_SIZE_IDX3      DB_HY_SIZE_IDX3                   ; /* 0x221c */
    U_DB_HY_SIZE_IDX4      DB_HY_SIZE_IDX4                   ; /* 0x2220 */
    U_DB_HY_SIZE_IDX5      DB_HY_SIZE_IDX5                   ; /* 0x2224 */
    U_DB_HY_SIZE_IDX6      DB_HY_SIZE_IDX6                   ; /* 0x2228 */
    U_DB_HY_SIZE_IDX7      DB_HY_SIZE_IDX7                   ; /* 0x222c */
    U_DB_VY_SIZE_IDX0      DB_VY_SIZE_IDX0                   ; /* 0x2230 */
    U_DB_VY_SIZE_IDX1      DB_VY_SIZE_IDX1                   ; /* 0x2234 */
    U_DB_VY_SIZE_IDX2      DB_VY_SIZE_IDX2                   ; /* 0x2238 */
    U_DB_VY_SIZE_IDX3      DB_VY_SIZE_IDX3                   ; /* 0x223c */
    U_DB_VY_SIZE_IDX4      DB_VY_SIZE_IDX4                   ; /* 0x2240 */
    U_DB_VY_SIZE_IDX5      DB_VY_SIZE_IDX5                   ; /* 0x2244 */
    U_DB_VY_SIZE_IDX6      DB_VY_SIZE_IDX6                   ; /* 0x2248 */
    U_DB_VY_SIZE_IDX7      DB_VY_SIZE_IDX7                   ; /* 0x224c */
    U_DB_HU_SIZE_IDX0      DB_HU_SIZE_IDX0                   ; /* 0x2250 */
    U_DB_HU_SIZE_IDX1      DB_HU_SIZE_IDX1                   ; /* 0x2254 */
    U_DB_HU_SIZE_IDX2      DB_HU_SIZE_IDX2                   ; /* 0x2258 */
    U_DB_HU_SIZE_IDX3      DB_HU_SIZE_IDX3                   ; /* 0x225c */
    U_DB_HU_SIZE_IDX4      DB_HU_SIZE_IDX4                   ; /* 0x2260 */
    U_DB_HU_SIZE_IDX5      DB_HU_SIZE_IDX5                   ; /* 0x2264 */
    U_DB_HU_SIZE_IDX6      DB_HU_SIZE_IDX6                   ; /* 0x2268 */
    U_DB_HU_SIZE_IDX7      DB_HU_SIZE_IDX7                   ; /* 0x226c */
    U_DB_HV_SIZE_IDX0      DB_HV_SIZE_IDX0                   ; /* 0x2270 */
    U_DB_HV_SIZE_IDX1      DB_HV_SIZE_IDX1                   ; /* 0x2274 */
    U_DB_HV_SIZE_IDX2      DB_HV_SIZE_IDX2                   ; /* 0x2278 */
    U_DB_HV_SIZE_IDX3      DB_HV_SIZE_IDX3                   ; /* 0x227c */
    U_DB_HV_SIZE_IDX4      DB_HV_SIZE_IDX4                   ; /* 0x2280 */
    U_DB_HV_SIZE_IDX5      DB_HV_SIZE_IDX5                   ; /* 0x2284 */
    U_DB_HV_SIZE_IDX6      DB_HV_SIZE_IDX6                   ; /* 0x2288 */
    U_DB_HV_SIZE_IDX7      DB_HV_SIZE_IDX7                   ; /* 0x228c */
    unsigned int           reserved_0[28]                     ; /* 0x2290~0x22fc */
    U_VPSS_PDFRMITDIFF     VPSS_PDFRMITDIFF                  ; /* 0x2300 */
    U_VPSS_PDSTLBLKSAD     VPSS_PDSTLBLKSAD[16]              ; /* 0x2304~0x2340 */
    U_VPSS_PDHISTBIN       VPSS_PDHISTBIN[4]                 ; /* 0x2344~0x2350 */
    U_VPSS_PDUMMATCH0      VPSS_PDUMMATCH0                   ; /* 0x2354 */
    U_VPSS_PDUMNOMATCH0    VPSS_PDUMNOMATCH0                 ; /* 0x2358 */
    U_VPSS_PDUMMATCH1      VPSS_PDUMMATCH1                   ; /* 0x235c */
    U_VPSS_PDUMNOMATCH1    VPSS_PDUMNOMATCH1                 ; /* 0x2360 */
    U_VPSS_PDPCCFFWD0      VPSS_PDPCCFFWD0                   ; /* 0x2364 */
    U_VPSS_PDPCCFWD0       VPSS_PDPCCFWD0                    ; /* 0x2368 */
    U_VPSS_PDPCCBWD0       VPSS_PDPCCBWD0                    ; /* 0x236c */
    U_VPSS_PDPCCCRSS0      VPSS_PDPCCCRSS0                   ; /* 0x2370 */
    U_VPSS_PDPCCFWDTKR0    VPSS_PDPCCFWDTKR0                 ; /* 0x2374 */
    U_VPSS_PDPCCBWDTKR0    VPSS_PDPCCBWDTKR0                 ; /* 0x2378 */
    U_VPSS_PDPCCFFWD1      VPSS_PDPCCFFWD1                   ; /* 0x237c */
    U_VPSS_PDPCCFWD1       VPSS_PDPCCFWD1                    ; /* 0x2380 */
    U_VPSS_PDPCCBWD1       VPSS_PDPCCBWD1                    ; /* 0x2384 */
    U_VPSS_PDPCCCRSS1      VPSS_PDPCCCRSS1                   ; /* 0x2388 */
    U_VPSS_PDPCCFWDTKR1    VPSS_PDPCCFWDTKR1                 ; /* 0x238c */
    U_VPSS_PDPCCBWDTKR1    VPSS_PDPCCBWDTKR1                 ; /* 0x2390 */
    U_VPSS_PDLASICNT140    VPSS_PDLASICNT140                 ; /* 0x2394 */
    U_VPSS_PDLASICNT320    VPSS_PDLASICNT320                 ; /* 0x2398 */
    U_VPSS_PDLASICNT340    VPSS_PDLASICNT340                 ; /* 0x239c */
    U_VPSS_PDLASICNT141    VPSS_PDLASICNT141                 ; /* 0x23a0 */
    U_VPSS_PDLASICNT321    VPSS_PDLASICNT321                 ; /* 0x23a4 */
    U_VPSS_PDLASICNT341    VPSS_PDLASICNT341                 ; /* 0x23a8 */
    U_VPSS_PDICHD          VPSS_PDICHD                       ; /* 0x23ac */
    unsigned int           reserved_1[20]                     ; /* 0x23b0~0x23fc */
    U_iglb_mtn_bin0        iglb_mtn_bin0                     ; /* 0x2400 */
    U_iglb_mtn_bin1        iglb_mtn_bin1                     ; /* 0x2404 */
    U_iglb_mtn_bin2        iglb_mtn_bin2                     ; /* 0x2408 */
    U_iglb_mtn_bin3        iglb_mtn_bin3                     ; /* 0x240c */
    U_iglb_mtn_bin4        iglb_mtn_bin4                     ; /* 0x2410 */
    U_iglb_mtn_bin5        iglb_mtn_bin5                     ; /* 0x2414 */
    U_iglb_mtn_bin6        iglb_mtn_bin6                     ; /* 0x2418 */
    U_iglb_mtn_bin7        iglb_mtn_bin7                     ; /* 0x241c */
    U_iglb_mtn_bin8        iglb_mtn_bin8                     ; /* 0x2420 */
    U_iglb_mtn_bin9        iglb_mtn_bin9                     ; /* 0x2424 */
    U_iglb_mtn_bin10       iglb_mtn_bin10                    ; /* 0x2428 */
    U_iglb_mtn_bin11       iglb_mtn_bin11                    ; /* 0x242c */
    U_iglb_mtn_bin12       iglb_mtn_bin12                    ; /* 0x2430 */
    U_iglb_mtn_bin13       iglb_mtn_bin13                    ; /* 0x2434 */
    U_iglb_mtn_bin14       iglb_mtn_bin14                    ; /* 0x2438 */
    U_iglb_mtn_bin15       iglb_mtn_bin15                    ; /* 0x243c */
    U_iglb_mtn_bin16       iglb_mtn_bin16                    ; /* 0x2440 */
    U_iglb_mtn_bin17       iglb_mtn_bin17                    ; /* 0x2444 */
    U_iglb_mtn_bin18       iglb_mtn_bin18                    ; /* 0x2448 */
    U_iglb_mtn_bin19       iglb_mtn_bin19                    ; /* 0x244c */
    U_iglb_mtn_bin20       iglb_mtn_bin20                    ; /* 0x2450 */
    U_iglb_mtn_bin21       iglb_mtn_bin21                    ; /* 0x2454 */
    U_iglb_mtn_bin22       iglb_mtn_bin22                    ; /* 0x2458 */
    U_iglb_mtn_bin23       iglb_mtn_bin23                    ; /* 0x245c */
    U_iglb_mtn_bin24       iglb_mtn_bin24                    ; /* 0x2460 */
    U_iglb_mtn_bin25       iglb_mtn_bin25                    ; /* 0x2464 */
    U_iglb_mtn_bin26       iglb_mtn_bin26                    ; /* 0x2468 */
    U_iglb_mtn_bin27       iglb_mtn_bin27                    ; /* 0x246c */
    U_iglb_mtn_bin28       iglb_mtn_bin28                    ; /* 0x2470 */
    U_iglb_mtn_bin29       iglb_mtn_bin29                    ; /* 0x2474 */
    U_iglb_mtn_bin30       iglb_mtn_bin30                    ; /* 0x2478 */
    U_iglb_mtn_bin31       iglb_mtn_bin31                    ; /* 0x247c */
    U_iglb_mtn_num0        iglb_mtn_num0                     ; /* 0x2480 */
    U_iglb_mtn_sum0        iglb_mtn_sum0                     ; /* 0x2484 */
    U_iglb_mtn_num1        iglb_mtn_num1                     ; /* 0x2488 */
    U_iglb_mtn_sum1        iglb_mtn_sum1                     ; /* 0x248c */
    U_iglb_mtn_num2        iglb_mtn_num2                     ; /* 0x2490 */
    U_iglb_mtn_sum2        iglb_mtn_sum2                     ; /* 0x2494 */
    U_iglb_mtn_num3        iglb_mtn_num3                     ; /* 0x2498 */
    U_iglb_mtn_sum3        iglb_mtn_sum3                     ; /* 0x249c */
    U_iglb_mtn_num4        iglb_mtn_num4                     ; /* 0x24a0 */
    U_iglb_mtn_sum4        iglb_mtn_sum4                     ; /* 0x24a4 */
    U_iglb1_mtn_num0       iglb1_mtn_num0                    ; /* 0x24a8 */
    U_iglb1_mtn_num1       iglb1_mtn_num1                    ; /* 0x24ac */
    U_iglb1_mtn_num2       iglb1_mtn_num2                    ; /* 0x24b0 */
    U_iglb1_mtn_num3       iglb1_mtn_num3                    ; /* 0x24b4 */
    U_iglb2_mtn_num0       iglb2_mtn_num0                    ; /* 0x24b8 */
    U_iglb2_mtn_num1       iglb2_mtn_num1                    ; /* 0x24bc */
    U_iglb2_mtn_num2       iglb2_mtn_num2                    ; /* 0x24c0 */
    U_iglb2_mtn_num3       iglb2_mtn_num3                    ; /* 0x24c4 */
    unsigned int           reserved_2[14]                     ; /* 0x24c8~0x24fc */
    U_STR_DET_VIDBLK0TOL0   STR_DET_VIDBLK0TOL0              ; /* 0x2500 */
    U_STR_DET_VIDBLK0TOL1   STR_DET_VIDBLK0TOL1              ; /* 0x2504 */
    U_STR_DET_VIDBLK1TOL0   STR_DET_VIDBLK1TOL0              ; /* 0x2508 */
    U_STR_DET_VIDBLK1TOL1   STR_DET_VIDBLK1TOL1              ; /* 0x250c */
    U_STR_DET_VIDBLK2TOL0   STR_DET_VIDBLK2TOL0              ; /* 0x2510 */
    U_STR_DET_VIDBLK2TOL1   STR_DET_VIDBLK2TOL1              ; /* 0x2514 */
    U_STR_DET_VIDBLK3TOL0   STR_DET_VIDBLK3TOL0              ; /* 0x2518 */
    U_STR_DET_VIDBLK3TOL1   STR_DET_VIDBLK3TOL1              ; /* 0x251c */
    U_STR_DET_VIDBLK4TOL0   STR_DET_VIDBLK4TOL0              ; /* 0x2520 */
    U_STR_DET_VIDBLK4TOL1   STR_DET_VIDBLK4TOL1              ; /* 0x2524 */
    U_STR_DET_VIDBLK5TOL0   STR_DET_VIDBLK5TOL0              ; /* 0x2528 */
    U_STR_DET_VIDBLK5TOL1   STR_DET_VIDBLK5TOL1              ; /* 0x252c */
}S_VPSSWB_REGS_TYPE,S_STT_REGS_TYPE;

#endif // __VPSS_REG_STT_H__
