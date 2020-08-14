//******************************************************************************
// Copyright     :  Copyright (C) 2016, Hisilicon Technologies Co., Ltd.
// File name     :  hi_reg_vpss_wbc.h
// Author        :
// Version       :  1.0
// Date          :  2016/9/24
// Description   :
// Others        :  Generated automatically by nManager V4.1
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

/* Define the union U_DB_MAX_INDEX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_val_idx            : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    hy_wgt_idx            : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 25  ; /* [31..7]  */
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
        unsigned int    hy_size_idx7          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    hy_counter_idx7       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DB_HY_SIZE_IDX7;

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
        unsigned int iglb_mtn_num3          : 32  ; /* [31..0]  */
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
        unsigned int iglb_mtn_sum3          : 32  ; /* [31..0]  */
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
        unsigned int iglb_mtn_num4          : 32  ; /* [31..0]  */
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
        unsigned int iglb_mtn_sum4          : 32  ; /* [31..0]  */
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
/* Define the union U_TNR_NOISEPOINTCNT_MIN */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int noisepointcntmin       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_TNR_NOISEPOINTCNT_MIN;
/* Define the union U_TNR_NOISEPOINTCNT_MED */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int noisepointcntmed       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_TNR_NOISEPOINTCNT_MED;
/* Define the union U_TNR_NOISEPOINTCNT_MAX */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int noisepointcntmax       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_TNR_NOISEPOINTCNT_MAX;
/* Define the union U_TNR_NOISEPOINTNUM */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int noisepointnum          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_TNR_NOISEPOINTNUM;
/* Define the union U_TNR_SUMNOISEINFO_MIN */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sumnoiseinfomin        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_TNR_SUMNOISEINFO_MIN;
/* Define the union U_TNR_SUMNOISEINFO_MED */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sumnoiseinfomed        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_TNR_SUMNOISEINFO_MED;
/* Define the union U_TNR_SUMNOISEINFO_MAX */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sumnoiseinfomax        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_TNR_SUMNOISEINFO_MAX;
//==============================================================================
/* Define the global struct */
typedef struct
{
    U_DB_HY_COUNTER        DB_HY_COUNTER[960]                ; /* 0x0~0xefc */
    U_DB_BORDER            DB_BORDER[128]                    ; /* 0xf00~0x10fc */
    U_DB_HY_SIZE           DB_HY_SIZE                        ; /* 0x1100 */
    unsigned int           reserved_0[2]                     ; /* 0x1104~0x1108 */
    U_DB_MAX_INDEX         DB_MAX_INDEX                      ; /* 0x110c */
    U_DB_HY_SIZE_IDX0      DB_HY_SIZE_IDX0                   ; /* 0x1110 */
    U_DB_HY_SIZE_IDX1      DB_HY_SIZE_IDX1                   ; /* 0x1114 */
    U_DB_HY_SIZE_IDX2      DB_HY_SIZE_IDX2                   ; /* 0x1118 */
    U_DB_HY_SIZE_IDX3      DB_HY_SIZE_IDX3                   ; /* 0x111c */
    U_DB_HY_SIZE_IDX4      DB_HY_SIZE_IDX4                   ; /* 0x1120 */
    U_DB_HY_SIZE_IDX5      DB_HY_SIZE_IDX5                   ; /* 0x1124 */
    U_DB_HY_SIZE_IDX6      DB_HY_SIZE_IDX6                   ; /* 0x1128 */
    U_DB_HY_SIZE_IDX7      DB_HY_SIZE_IDX7                   ; /* 0x112c */
    unsigned int           reserved_1[24]                     ; /* 0x1130~0x118c */
    U_VPSS_PDFRMITDIFF     VPSS_PDFRMITDIFF                  ; /* 0x1190 */
    U_VPSS_PDSTLBLKSAD     VPSS_PDSTLBLKSAD[16]              ; /* 0x1194~0x11d0 */
    U_VPSS_PDHISTBIN       VPSS_PDHISTBIN[4]                 ; /* 0x11d4~0x11e0 */
    U_VPSS_PDUMMATCH0      VPSS_PDUMMATCH0                   ; /* 0x11e4 */
    U_VPSS_PDUMNOMATCH0    VPSS_PDUMNOMATCH0                 ; /* 0x11e8 */
    U_VPSS_PDUMMATCH1      VPSS_PDUMMATCH1                   ; /* 0x11ec */
    U_VPSS_PDUMNOMATCH1    VPSS_PDUMNOMATCH1                 ; /* 0x11f0 */
    U_VPSS_PDPCCFFWD0      VPSS_PDPCCFFWD0                   ; /* 0x11f4 */
    U_VPSS_PDPCCFWD0       VPSS_PDPCCFWD0                    ; /* 0x11f8 */
    U_VPSS_PDPCCBWD0       VPSS_PDPCCBWD0                    ; /* 0x11fc */
    U_VPSS_PDPCCCRSS0      VPSS_PDPCCCRSS0                   ; /* 0x1200 */
    U_VPSS_PDPCCFWDTKR0    VPSS_PDPCCFWDTKR0                 ; /* 0x1204 */
    U_VPSS_PDPCCBWDTKR0    VPSS_PDPCCBWDTKR0                 ; /* 0x1208 */
    U_VPSS_PDPCCFFWD1      VPSS_PDPCCFFWD1                   ; /* 0x120c */
    U_VPSS_PDPCCFWD1       VPSS_PDPCCFWD1                    ; /* 0x1210 */
    U_VPSS_PDPCCBWD1       VPSS_PDPCCBWD1                    ; /* 0x1214 */
    U_VPSS_PDPCCCRSS1      VPSS_PDPCCCRSS1                   ; /* 0x1218 */
    U_VPSS_PDPCCFWDTKR1    VPSS_PDPCCFWDTKR1                 ; /* 0x121c */
    U_VPSS_PDPCCBWDTKR1    VPSS_PDPCCBWDTKR1                 ; /* 0x1220 */
    U_VPSS_PDLASICNT140    VPSS_PDLASICNT140                 ; /* 0x1224 */
    U_VPSS_PDLASICNT320    VPSS_PDLASICNT320                 ; /* 0x1228 */
    U_VPSS_PDLASICNT340    VPSS_PDLASICNT340                 ; /* 0x122c */
    U_VPSS_PDLASICNT141    VPSS_PDLASICNT141                 ; /* 0x1230 */
    U_VPSS_PDLASICNT321    VPSS_PDLASICNT321                 ; /* 0x1234 */
    U_VPSS_PDLASICNT341    VPSS_PDLASICNT341                 ; /* 0x1238 */
    U_VPSS_PDICHD          VPSS_PDICHD                       ; /* 0x123c */
    U_iglb_mtn_bin0        iglb_mtn_bin0                     ; /* 0x1240 */
    U_iglb_mtn_bin1        iglb_mtn_bin1                     ; /* 0x1244 */
    U_iglb_mtn_bin2        iglb_mtn_bin2                     ; /* 0x1248 */
    U_iglb_mtn_bin3        iglb_mtn_bin3                     ; /* 0x124c */
    U_iglb_mtn_bin4        iglb_mtn_bin4                     ; /* 0x1250 */
    U_iglb_mtn_bin5        iglb_mtn_bin5                     ; /* 0x1254 */
    U_iglb_mtn_bin6        iglb_mtn_bin6                     ; /* 0x1258 */
    U_iglb_mtn_bin7        iglb_mtn_bin7                     ; /* 0x125c */
    U_iglb_mtn_bin8        iglb_mtn_bin8                     ; /* 0x1260 */
    U_iglb_mtn_bin9        iglb_mtn_bin9                     ; /* 0x1264 */
    U_iglb_mtn_bin10       iglb_mtn_bin10                    ; /* 0x1268 */
    U_iglb_mtn_bin11       iglb_mtn_bin11                    ; /* 0x126c */
    U_iglb_mtn_bin12       iglb_mtn_bin12                    ; /* 0x1270 */
    U_iglb_mtn_bin13       iglb_mtn_bin13                    ; /* 0x1274 */
    U_iglb_mtn_bin14       iglb_mtn_bin14                    ; /* 0x1278 */
    U_iglb_mtn_bin15       iglb_mtn_bin15                    ; /* 0x127c */
    U_iglb_mtn_bin16       iglb_mtn_bin16                    ; /* 0x1280 */
    U_iglb_mtn_bin17       iglb_mtn_bin17                    ; /* 0x1284 */
    U_iglb_mtn_bin18       iglb_mtn_bin18                    ; /* 0x1288 */
    U_iglb_mtn_bin19       iglb_mtn_bin19                    ; /* 0x128c */
    U_iglb_mtn_bin20       iglb_mtn_bin20                    ; /* 0x1290 */
    U_iglb_mtn_bin21       iglb_mtn_bin21                    ; /* 0x1294 */
    U_iglb_mtn_bin22       iglb_mtn_bin22                    ; /* 0x1298 */
    U_iglb_mtn_bin23       iglb_mtn_bin23                    ; /* 0x129c */
    U_iglb_mtn_bin24       iglb_mtn_bin24                    ; /* 0x12a0 */
    U_iglb_mtn_bin25       iglb_mtn_bin25                    ; /* 0x12a4 */
    U_iglb_mtn_bin26       iglb_mtn_bin26                    ; /* 0x12a8 */
    U_iglb_mtn_bin27       iglb_mtn_bin27                    ; /* 0x12ac */
    U_iglb_mtn_bin28       iglb_mtn_bin28                    ; /* 0x12b0 */
    U_iglb_mtn_bin29       iglb_mtn_bin29                    ; /* 0x12b4 */
    U_iglb_mtn_bin30       iglb_mtn_bin30                    ; /* 0x12b8 */
    U_iglb_mtn_bin31       iglb_mtn_bin31                    ; /* 0x12bc */
    U_iglb_mtn_num0        iglb_mtn_num0                     ; /* 0x12c0 */
    U_iglb_mtn_sum0        iglb_mtn_sum0                     ; /* 0x12c4 */
    U_iglb_mtn_num1        iglb_mtn_num1                     ; /* 0x12c8 */
    U_iglb_mtn_sum1        iglb_mtn_sum1                     ; /* 0x12cc */
    U_iglb_mtn_num2        iglb_mtn_num2                     ; /* 0x12d0 */
    U_iglb_mtn_sum2        iglb_mtn_sum2                     ; /* 0x12d4 */
    U_iglb_mtn_num3        iglb_mtn_num3                     ; /* 0x12d8 */
    U_iglb_mtn_sum3        iglb_mtn_sum3                     ; /* 0x12dc */
    U_iglb_mtn_num4        iglb_mtn_num4                     ; /* 0x12e0 */
    U_iglb_mtn_sum4        iglb_mtn_sum4                     ; /* 0x12e4 */
    U_iglb1_mtn_num0       iglb1_mtn_num0                    ; /* 0x12e8 */
    U_iglb1_mtn_num1       iglb1_mtn_num1                    ; /* 0x12ec */
    U_iglb1_mtn_num2       iglb1_mtn_num2                    ; /* 0x12f0 */
    U_iglb1_mtn_num3       iglb1_mtn_num3                    ; /* 0x12f4 */
    U_iglb2_mtn_num0       iglb2_mtn_num0                    ; /* 0x12f8 */
    U_iglb2_mtn_num1       iglb2_mtn_num1                    ; /* 0x12fc */
    U_iglb2_mtn_num2       iglb2_mtn_num2                    ; /* 0x1300 */
    U_iglb2_mtn_num3       iglb2_mtn_num3                    ; /* 0x1304 */
    unsigned int           reserved_2[2]                     ; /* 0x1308~0x130c */
    U_TNR_NOISEPOINTCNT_MIN   TNR_NOISEPOINTCNT_MIN          ; /* 0x1310 */
    U_TNR_NOISEPOINTCNT_MED   TNR_NOISEPOINTCNT_MED          ; /* 0x1314 */
    U_TNR_NOISEPOINTCNT_MAX   TNR_NOISEPOINTCNT_MAX          ; /* 0x1318 */
    U_TNR_NOISEPOINTNUM    TNR_NOISEPOINTNUM                 ; /* 0x131c */
    U_TNR_SUMNOISEINFO_MIN   TNR_SUMNOISEINFO_MIN            ; /* 0x1320 */
    U_TNR_SUMNOISEINFO_MED   TNR_SUMNOISEINFO_MED            ; /* 0x1324 */
    U_TNR_SUMNOISEINFO_MAX   TNR_SUMNOISEINFO_MAX            ; /* 0x1328 */

} S_VPSSWB_REGS_TYPE, S_STT_REGS_TYPE;

#endif // __HI_REG_VPSS_WBC_H__
