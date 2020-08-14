//****************************************************************************** 
// Copyright     :  Copyright (C) 2014, Hisilicon Technologies Co., Ltd.
// File name     :  c_union_define_vpss_mv410.h
// Author        :  x57490
// Version       :  1.0 
// Date          :  2014-12-23
// Description   :  Define all registers/tables for S40V200
// Others        :  Generated automatically by nManager V4.0 
// History       :  x57490 2014-12-23 Create file
//******************************************************************************

#ifndef __VPSS_REG_MV410_H__
#define __VPSS_REG_MV410_H__

#include "vpss_common.h"
#include "vpss_reg_struct.h"
#include "hi_drv_reg.h"
#include "hi_reg_common.h"
#include "drv_pq_ext.h"

#define DEF_LOGIC_TIMEOUT  0xffffffff//0xe4e1c0
#if 0
#define VPSS_BASE_ADDR  0xf8cb0000



/* Define the union U_VPSS_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chk_sum_en            : 1   ; /* [0]  */
        unsigned int    vhd0_en               : 1   ; /* [1]  */
        unsigned int    rfr_en                : 1   ; /* [2]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    test_pat_en           : 1   ; /* [4]  */
        unsigned int    uv_invert             : 1   ; /* [5]  */
        unsigned int    img_pro_mode          : 2   ; /* [7..6]  */
        unsigned int    str_det_en            : 1   ; /* [8]  */
        unsigned int    dei_en                : 1   ; /* [9]  */
        unsigned int    vhd0_lcmp_en          : 1   ; /* [10]  */
        unsigned int    dbm_en                : 1   ; /* [11]  */
        unsigned int    prot                  : 2   ; /* [13..12]  */
        unsigned int    tnr_en                : 1   ; /* [14]  */
        unsigned int    vhd0_lba_en           : 1   ; /* [15]  */
        unsigned int    iglb_en               : 1   ; /* [16]  */
        unsigned int    ifmd_en               : 1   ; /* [17]  */
        unsigned int    reserved_1            : 3   ; /* [20..18]  */
        unsigned int    rotate_angle          : 1   ; /* [21]  */
        unsigned int    rotate_en             : 1   ; /* [22]  */
        unsigned int    vhd0_pre_vfir_mode    : 2   ; /* [24..23]  */
        unsigned int    vhd0_pre_vfir_en      : 1   ; /* [25]  */
        unsigned int    vhd0_pre_hfir_mode    : 2   ; /* [27..26]  */
        unsigned int    vhd0_pre_hfir_en      : 1   ; /* [28]  */
        unsigned int    bfield_first          : 1   ; /* [29]  */
        unsigned int    bfield_mode           : 1   ; /* [30]  */
        unsigned int    bfield                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CTRL;

/* Define the union U_VPSS_CTRL2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    vhd0_format           : 4   ; /* [7..4]  */
        unsigned int    reserved_1            : 8   ; /* [15..8]  */
        unsigned int    in_format             : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CTRL2;

/* Define the union U_VPSS_REFSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    refwidth              : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    refheight             : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_REFSIZE;

/* Define the union U_VPSS_IMGSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    imgwidth              : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    imgheight             : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_IMGSIZE;

/* Define the union U_VPSS_RID_CFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cury_rid              : 3   ; /* [2..0]  */
        unsigned int    curc_rid              : 3   ; /* [5..3]  */
        unsigned int    refy_rid              : 3   ; /* [8..6]  */
        unsigned int    refc_rid              : 3   ; /* [11..9]  */
        unsigned int    nx1y_rid              : 3   ; /* [14..12]  */
        unsigned int    nx1c_rid              : 3   ; /* [17..15]  */
        unsigned int    nx2c_rid              : 3   ; /* [20..18]  */
        unsigned int    nx2y_rid              : 3   ; /* [23..21]  */
        unsigned int    rst_rid               : 3   ; /* [26..24]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_RID_CFG0;

/* Define the union U_VPSS_CUR_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    cur_tile_format       : 1   ; /* [4]  */
        unsigned int    cur_hor_offset        : 13  ; /* [17..5]  */
        unsigned int    cur_ver_offset        : 13  ; /* [30..18]  */
        unsigned int    cur_package422        : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CUR_CTRL;

/* Define the union U_VPSS_CURYADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int cury_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_CURYADDR;
/* Define the union U_VPSS_CURCADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int curc_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_CURCADDR;
/* Define the union U_VPSS_CURCRADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int curcr_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_CURCRADDR;
/* Define the union U_VPSS_CURSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cury_stride           : 16  ; /* [15..0]  */
        unsigned int    curc_stride           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CURSTRIDE;

/* Define the union U_VPSS_REF_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    ref_tile_format       : 1   ; /* [4]  */
        unsigned int    ref_hor_offset        : 13  ; /* [17..5]  */
        unsigned int    ref_ver_offset        : 13  ; /* [30..18]  */
        unsigned int    ref_package422        : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_REF_CTRL;

/* Define the union U_VPSS_REFYADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int refy_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_REFYADDR;
/* Define the union U_VPSS_REFCADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int refc_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_REFCADDR;
/* Define the union U_VPSS_REFCRADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int refcr_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_REFCRADDR;
/* Define the union U_VPSS_REFSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    refy_stride           : 16  ; /* [15..0]  */
        unsigned int    refc_stride           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_REFSTRIDE;

/* Define the union U_VPSS_NXT1_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    nxt1_tile_format      : 1   ; /* [4]  */
        unsigned int    nxt1_hor_offset       : 13  ; /* [17..5]  */
        unsigned int    nxt1_ver_offset       : 13  ; /* [30..18]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NXT1_CTRL;

/* Define the union U_VPSS_NXT1YADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nxt1y_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NXT1YADDR;
/* Define the union U_VPSS_NXT1CADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nxt1c_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NXT1CADDR;
/* Define the union U_VPSS_NXT1CRADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nxt1cr_addr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NXT1CRADDR;
/* Define the union U_VPSS_NXT1STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nxt1y_stride          : 16  ; /* [15..0]  */
        unsigned int    nxt1c_stride          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NXT1STRIDE;

/* Define the union U_VPSS_NXT2_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    nxt2_tile_format      : 1   ; /* [4]  */
        unsigned int    nxt2_hor_offset       : 13  ; /* [17..5]  */
        unsigned int    nxt2_ver_offset       : 13  ; /* [30..18]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NXT2_CTRL;

/* Define the union U_VPSS_NXT2YADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nxt2y_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NXT2YADDR;
/* Define the union U_VPSS_NXT2CADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nxt2c_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NXT2CADDR;
/* Define the union U_VPSS_NXT2CRADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nxt2cr_addr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NXT2CRADDR;
/* Define the union U_VPSS_NXT2STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nxt2y_stride          : 16  ; /* [15..0]  */
        unsigned int    nxt2c_stride          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NXT2STRIDE;

/* Define the union U_VPSS_STRADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int st_raddr               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STRADDR;
/* Define the union U_VPSS_STWADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int st_waddr               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STWADDR;
/* Define the union U_VPSS_STSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    st_stride             : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_STSTRIDE;

/* Define the union U_VPSS_MAD_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int mad_raddr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_MAD_RADDR;
/* Define the union U_VPSS_MAD_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int mad_waddr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_MAD_WADDR;
/* Define the union U_VPSS_MADSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mad_stride            : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MADSTRIDE;

/* Define the union U_VPSS_RFRYADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int rfry_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_RFRYADDR;
/* Define the union U_VPSS_RFRCADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int rfrc_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_RFRCADDR;
/* Define the union U_VPSS_RFRSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rfry_stride           : 16  ; /* [15..0]  */
        unsigned int    rfrc_stride           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_RFRSTRIDE;

/* Define the union U_VPSS_VHD0CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    vhd0_mirror           : 1   ; /* [4]  */
        unsigned int    vhd0_flip             : 1   ; /* [5]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    vhd0_uv_invert        : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0CTRL;

/* Define the union U_VPSS_VHD0SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0_width            : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vhd0_height           : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0SIZE;

/* Define the union U_VPSS_VHD0YADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd0y_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD0YADDR;
/* Define the union U_VPSS_VHD0CADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd0c_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD0CADDR;
/* Define the union U_VPSS_VHD0STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0y_stride          : 16  ; /* [15..0]  */
        unsigned int    vhd0c_stride          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0STRIDE;

/* Define the union U_VPSS_TUNL_CTRL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0_tunl_finish_line : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 16  ; /* [27..12]  */
        unsigned int    vhd0_tunl_en          : 1   ; /* [28]  */
        unsigned int    reserved_1            : 2   ; /* [30..29]  */
        unsigned int    cur_tunl_en           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TUNL_CTRL0;

/* Define the union U_VPSS_TUNL_CTRL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    cur_tunl_rd_interval  : 8   ; /* [23..16]  */
        unsigned int    vhd0_tunl_mode        : 2   ; /* [25..24]  */
        unsigned int    reserved_1            : 2   ; /* [27..26]  */
        unsigned int    reserved_2            : 2   ; /* [29..28]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TUNL_CTRL1;

/* Define the union U_VPSS_VHD0_TUNL_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd0_tunl_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD0_TUNL_ADDR;
/* Define the union U_VPSS_CUR_TUNL_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int cur_tunl_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_CUR_TUNL_ADDR;
/* Define the union U_VPSS_CHK_SUM_W_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int chk_sum_w_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_CHK_SUM_W_ADDR;
/* Define the union U_VPSS_CHK_SUM_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rch0_sum_sel          : 4   ; /* [3..0]  */
        unsigned int    rch1_sum_sel          : 4   ; /* [7..4]  */
        unsigned int    rch2_sum_sel          : 4   ; /* [11..8]  */
        unsigned int    rch3_sum_sel          : 4   ; /* [15..12]  */
        unsigned int    rinfo_sum_sel         : 4   ; /* [19..16]  */
        unsigned int    winfo_sum_sel         : 4   ; /* [23..20]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CHK_SUM_SEL;

/* Define the union U_VPSS_STT_W_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int stt_w_addr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STT_W_ADDR;
/* Define the union U_VPSS_INCROP_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    in_crop_x             : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    in_crop_y             : 12  ; /* [27..16]  */
        unsigned int    cur_in_crop_en        : 1   ; /* [28]  */
        unsigned int    ref_in_crop_en        : 1   ; /* [29]  */
        unsigned int    nxt1_in_crop_en       : 1   ; /* [30]  */
        unsigned int    nxt2_in_crop_en       : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_INCROP_POS;

/* Define the union U_VPSS_INCROP_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    in_crop_width         : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    in_crop_height        : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_INCROP_SIZE;

/* Define the union U_VPSS_VHD0CROP_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0_crop_x           : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vhd0_crop_y           : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0CROP_POS;

/* Define the union U_VPSS_VHD0CROP_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0_crop_width       : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vhd0_crop_height      : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 3   ; /* [30..28]  */
        unsigned int    vhd0_crop_en          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0CROP_SIZE;

/* Define the union U_VPSS_VHD0_HSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hratio                : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 1   ; /* [24]  */
        unsigned int    hchfir_en             : 1   ; /* [25]  */
        unsigned int    hlfir_en              : 1   ; /* [26]  */
        unsigned int    reserved_1            : 1   ; /* [27]  */
        unsigned int    hchmid_en             : 1   ; /* [28]  */
        unsigned int    hlmid_en              : 1   ; /* [29]  */
        unsigned int    hchmsc_en             : 1   ; /* [30]  */
        unsigned int    hlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_HSP;

/* Define the union U_VPSS_VHD0_HLOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor_loffset           : 28  ; /* [27..0]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_HLOFFSET;

/* Define the union U_VPSS_VHD0_HCOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor_coffset           : 28  ; /* [27..0]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_HCOFFSET;

/* Define the union U_VPSS_VHD0_VSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 19  ; /* [18..0]  */
        unsigned int    zme_in_fmt            : 2   ; /* [20..19]  */
        unsigned int    zme_out_fmt           : 2   ; /* [22..21]  */
        unsigned int    vchfir_en             : 1   ; /* [23]  */
        unsigned int    vlfir_en              : 1   ; /* [24]  */
        unsigned int    reserved_1            : 1   ; /* [25]  */
        unsigned int    vsc_chroma_tap        : 1   ; /* [26]  */
        unsigned int    reserved_2            : 1   ; /* [27]  */
        unsigned int    vchmid_en             : 1   ; /* [28]  */
        unsigned int    vlmid_en              : 1   ; /* [29]  */
        unsigned int    vchmsc_en             : 1   ; /* [30]  */
        unsigned int    vlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_VSP;

/* Define the union U_VPSS_VHD0_VSR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vratio                : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_VSR;

/* Define the union U_VPSS_VHD0_VOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vchroma_offset        : 16  ; /* [15..0]  */
        unsigned int    vluma_offset          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_VOFFSET;

/* Define the union U_VPSS_VHD0_ZMEORESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ow                    : 12  ; /* [11..0]  */
        unsigned int    oh                    : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_ZMEORESO;

/* Define the union U_VPSS_DR_CFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    drthrmaxsimilarpixdiff : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 3   ; /* [23..21]  */
        unsigned int    drthrflat3x3zone      : 5   ; /* [28..24]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_CFG0;

/* Define the union U_VPSS_DR_CFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dralphascale          : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    drbetascale           : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 3   ; /* [23..21]  */
        unsigned int    dr_demo_en            : 1   ; /* [24]  */
        unsigned int    reserved_2            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_CFG1;

/* Define the union U_VPSS_DNR_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    thrintlcolcnt         : 4   ; /* [3..0]  */
        unsigned int    thrintlcnt            : 4   ; /* [7..4]  */
        unsigned int    thdmaxgrad            : 8   ; /* [15..8]  */
        unsigned int    thdcntrst8            : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DNR_INFO;

/* Define the union U_VPSS_NODEID */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nodeid                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NODEID;
/* Define the union U_VPSS_AXIID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 20  ; /* [19..0]  */
        unsigned int    awid_cfg0             : 4   ; /* [23..20]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_AXIID;

/* Define the union U_VPSS_INTMASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    eof_mask              : 1   ; /* [0]  */
        unsigned int    timeout_mask          : 1   ; /* [1]  */
        unsigned int    bus_w_err_mask        : 1   ; /* [2]  */
        unsigned int    eof_end_mask          : 1   ; /* [3]  */
        unsigned int    vhd0_tunl_mask        : 1   ; /* [4]  */
        unsigned int    reserved_0            : 1   ; /* [5]  */
        unsigned int    reserved_1            : 1   ; /* [6]  */
        unsigned int    reserved_2            : 3   ; /* [9..7]  */
        unsigned int    bus_r_err_mask        : 1   ; /* [10]  */
        unsigned int    reserved_3            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_INTMASK;

/* Define the union U_VPSS_VHD0_ZME_LHADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd0_scl_lh            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD0_ZME_LHADDR;
/* Define the union U_VPSS_VHD0_ZME_LVADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd0_scl_lv            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD0_ZME_LVADDR;
/* Define the union U_VPSS_VHD0_ZME_CHADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd0_scl_ch            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD0_ZME_CHADDR;
/* Define the union U_VPSS_VHD0_ZME_CVADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd0_scl_cv            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD0_ZME_CVADDR;
/* Define the union U_VPSS_TNR_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int tnr_cfg_addr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_TNR_ADDR;
/* Define the union U_VPSS_TNR_CLUT_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int tnr_clut_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_TNR_CLUT_ADDR;
/* Define the union U_VPSS_DEI_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int dei_cfg_addr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEI_ADDR;
/* Define the union U_VPSS_VHD0_LCMP_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd0_lcmp_base_addr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD0_LCMP_ADDR;
/* Define the union U_VPSS_VC1_CTRL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fr0_vc1_profile       : 2   ; /* [1..0]  */
        unsigned int    fr0_vc1_rangedfrm     : 1   ; /* [2]  */
        unsigned int    fr0_vc1_mapyflg       : 1   ; /* [3]  */
        unsigned int    fr0_vc1_mapcflg       : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    fr0_vc1_mapy          : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    fr0_vc1_mapc          : 3   ; /* [14..12]  */
        unsigned int    reserved_2            : 16  ; /* [30..15]  */
        unsigned int    fr0_vc1_en            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VC1_CTRL0;

/* Define the union U_VPSS_VC1_CTRL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fr1_vc1_profile       : 2   ; /* [1..0]  */
        unsigned int    fr1_vc1_rangedfrm     : 1   ; /* [2]  */
        unsigned int    fr1_vc1_mapyflg       : 1   ; /* [3]  */
        unsigned int    fr1_vc1_mapcflg       : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    fr1_vc1_mapy          : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    fr1_vc1_mapc          : 3   ; /* [14..12]  */
        unsigned int    reserved_2            : 16  ; /* [30..15]  */
        unsigned int    fr1_vc1_en            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VC1_CTRL1;

/* Define the union U_VPSS_VC1_CTRL2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fr2_vc1_profile       : 2   ; /* [1..0]  */
        unsigned int    fr2_vc1_rangedfrm     : 1   ; /* [2]  */
        unsigned int    fr2_vc1_mapyflg       : 1   ; /* [3]  */
        unsigned int    fr2_vc1_mapcflg       : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    fr2_vc1_mapy          : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    fr2_vc1_mapc          : 3   ; /* [14..12]  */
        unsigned int    reserved_2            : 16  ; /* [30..15]  */
        unsigned int    fr2_vc1_en            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VC1_CTRL2;

/* Define the union U_VPSS_VC1_CTRL3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fr3_vc1_profile       : 2   ; /* [1..0]  */
        unsigned int    fr3_vc1_rangedfrm     : 1   ; /* [2]  */
        unsigned int    fr3_vc1_mapyflg       : 1   ; /* [3]  */
        unsigned int    fr3_vc1_mapcflg       : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    fr3_vc1_mapy          : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    fr3_vc1_mapc          : 3   ; /* [14..12]  */
        unsigned int    reserved_2            : 16  ; /* [30..15]  */
        unsigned int    fr3_vc1_en            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VC1_CTRL3;

/* Define the union U_VPSS_VHD0LBA_DSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0_lba_width        : 12  ; /* [11..0]  */
        unsigned int    vhd0_lba_height       : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0LBA_DSIZE;

/* Define the union U_VPSS_VHD0LBA_VFPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0_lba_xfpos        : 12  ; /* [11..0]  */
        unsigned int    vhd0_lba_yfpos        : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0LBA_VFPOS;

/* Define the union U_VPSS_VHD0LBA_BK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vbk_cr                : 8   ; /* [7..0]  */
        unsigned int    vbk_cb                : 8   ; /* [15..8]  */
        unsigned int    vbk_y                 : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0LBA_BK;

/* Define the union U_STR_DET_VIDCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 30  ; /* [29..0]  */
        unsigned int    vid_mode              : 1   ; /* [30]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDCTRL;

/* Define the union U_STR_DET_VIDBLKPOS0_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk0_xlpos            : 8   ; /* [7..0]  */
        unsigned int    blk0_ylpos            : 8   ; /* [15..8]  */
        unsigned int    blk1_xlpos            : 8   ; /* [23..16]  */
        unsigned int    blk1_ylpos            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLKPOS0_1;

/* Define the union U_STR_DET_VIDBLKPOS2_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk2_xlpos            : 8   ; /* [7..0]  */
        unsigned int    blk2_ylpos            : 8   ; /* [15..8]  */
        unsigned int    blk3_xlpos            : 8   ; /* [23..16]  */
        unsigned int    blk3_ylpos            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLKPOS2_3;

/* Define the union U_STR_DET_VIDBLKPOS4_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk4_xlpos            : 8   ; /* [7..0]  */
        unsigned int    blk4_ylpos            : 8   ; /* [15..8]  */
        unsigned int    blk5_xlpos            : 8   ; /* [23..16]  */
        unsigned int    blk5_ylpos            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STR_DET_VIDBLKPOS4_5;

/* Define the union U_VPSS_PNEXT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int p_next                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PNEXT;
/* Define the union U_VPSS_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    start                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_START;

/* Define the union U_VPSS_INTSTATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    eof_state             : 1   ; /* [0]  */
        unsigned int    timeout_state         : 1   ; /* [1]  */
        unsigned int    bus_w_err             : 1   ; /* [2]  */
        unsigned int    eof_end_state         : 1   ; /* [3]  */
        unsigned int    vhd0_tunl_state       : 1   ; /* [4]  */
        unsigned int    reserved_0            : 1   ; /* [5]  */
        unsigned int    reserved_1            : 1   ; /* [6]  */
        unsigned int    reserved_2            : 3   ; /* [9..7]  */
        unsigned int    bus_r_err             : 1   ; /* [10]  */
        unsigned int    reserved_3            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_INTSTATE;

/* Define the union U_VPSS_INTCLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    eof_clr               : 1   ; /* [0]  */
        unsigned int    timeout_clr           : 1   ; /* [1]  */
        unsigned int    bus_w_err_clr         : 1   ; /* [2]  */
        unsigned int    eof_end_clr           : 1   ; /* [3]  */
        unsigned int    vhd0_tunl_clr         : 1   ; /* [4]  */
        unsigned int    reserved_0            : 1   ; /* [5]  */
        unsigned int    reserved_1            : 1   ; /* [6]  */
        unsigned int    reserved_2            : 3   ; /* [9..7]  */
        unsigned int    bus_r_err_clr         : 1   ; /* [10]  */
        unsigned int    reserved_3            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_INTCLR;

/* Define the union U_VPSS_RAWINT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    raw_eof               : 1   ; /* [0]  */
        unsigned int    raw_timeout           : 1   ; /* [1]  */
        unsigned int    raw_w_bus_err         : 1   ; /* [2]  */
        unsigned int    raw_eof_end           : 1   ; /* [3]  */
        unsigned int    raw_vhd0_state        : 1   ; /* [4]  */
        unsigned int    reserved_0            : 1   ; /* [5]  */
        unsigned int    reserved_1            : 1   ; /* [6]  */
        unsigned int    reserved_2            : 3   ; /* [9..7]  */
        unsigned int    raw_r_bus_err         : 1   ; /* [10]  */
        unsigned int    reserved_3            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_RAWINT;

/* Define the union U_VPSS_PFCNT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int pfcnt                  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PFCNT;
/* Define the union U_VPSS_MISCELLANEOUS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    routstanding          : 4   ; /* [3..0]  */
        unsigned int    woutstanding          : 4   ; /* [7..4]  */
        unsigned int    init_timer            : 16  ; /* [23..8]  */
        unsigned int    ck_gt_en              : 1   ; /* [24]  */
        unsigned int    ck_gt_en_calc         : 1   ; /* [25]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MISCELLANEOUS;

/* Define the union U_VPSS_MACCFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mac_ch_prio           : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MACCFG;

/* Define the union U_VPSS_TIMEOUT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int timeout                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_TIMEOUT;
/* Define the union U_VPSS_EOFCNT */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int eof_cnt                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_EOFCNT;
/* Define the union U_VPSS_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_version           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VERSION;
/* Define the union U_VPSS_VHD0_LCMP_YINFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    is_lossless           : 1   ; /* [0]  */
        unsigned int    cmp_mode              : 1   ; /* [1]  */
        unsigned int    dw_mode               : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_YINFO;

/* Define the union U_VPSS_VHD0_LCMP_YSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_width           : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    frame_height          : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_YSIZE;

/* Define the union U_VPSS_VHD0_LCMP_YRCCFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mb_bits               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    ri_bits               : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 4   ; /* [23..20]  */
        unsigned int    max_rg_comp_bits      : 5   ; /* [28..24]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_YRCCFG0;

/* Define the union U_VPSS_VHD0_LCMP_YRCCFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    max_qp                : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 13  ; /* [15..3]  */
        unsigned int    sad_bits_ngain        : 2   ; /* [17..16]  */
        unsigned int    reserved_1            : 6   ; /* [23..18]  */
        unsigned int    rc_smth_ngain         : 2   ; /* [25..24]  */
        unsigned int    reserved_2            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_YRCCFG1;

/* Define the union U_VPSS_VHD0_LCMP_YRCCFG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    smth_thr              : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    smth_pix_num_thr      : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    pix_diff_thr          : 8   ; /* [23..16]  */
        unsigned int    reserved_2            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_YRCCFG2;

/* Define the union U_VPSS_VHD0_LCMP_YRCCFG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj_sad_thr           : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    adj_sad_bit_thr       : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_YRCCFG3;

/* Define the union U_VPSS_VHD0_LCMP_YRCCFG4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qp_chg1_bits_thr      : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    qp_chg2_bits_thr      : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_YRCCFG4;

/* Define the union U_VPSS_VHD0_LCMP_YRCCFG5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    smth_qp0              : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 5   ; /* [7..3]  */
        unsigned int    smth_qp1              : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 5   ; /* [15..11]  */
        unsigned int    smth_lftbits_thr0     : 8   ; /* [23..16]  */
        unsigned int    smth_lftbits_thr1     : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_YRCCFG5;

/* Define the union U_VPSS_VHD0_LCMP_YMAXLEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    max_row_len           : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_YMAXLEN;

/* Define the union U_VPSS_VHD0_LCMP_CINFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    is_lossless           : 1   ; /* [0]  */
        unsigned int    cmp_mode              : 1   ; /* [1]  */
        unsigned int    dw_mode               : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_CINFO;

/* Define the union U_VPSS_VHD0_LCMP_CSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_width           : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    frame_height          : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_CSIZE;

/* Define the union U_VPSS_VHD0_LCMP_CRCCFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mb_bits               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    ri_bits               : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 4   ; /* [23..20]  */
        unsigned int    max_rg_comp_bits      : 5   ; /* [28..24]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_CRCCFG0;

/* Define the union U_VPSS_VHD0_LCMP_CRCCFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    max_qp                : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 13  ; /* [15..3]  */
        unsigned int    sad_bits_ngain        : 2   ; /* [17..16]  */
        unsigned int    reserved_1            : 6   ; /* [23..18]  */
        unsigned int    rc_smth_ngain         : 2   ; /* [25..24]  */
        unsigned int    reserved_2            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_CRCCFG1;

/* Define the union U_VPSS_VHD0_LCMP_CRCCFG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    smth_thr              : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    smth_pix_num_thr      : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    pix_diff_thr          : 8   ; /* [23..16]  */
        unsigned int    reserved_2            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_CRCCFG2;

/* Define the union U_VPSS_VHD0_LCMP_CRCCFG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj_sad_thr           : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    adj_sad_bit_thr       : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_CRCCFG3;

/* Define the union U_VPSS_VHD0_LCMP_CRCCFG4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qp_chg1_bits_thr      : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    qp_chg2_bits_thr      : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_CRCCFG4;

/* Define the union U_VPSS_VHD0_LCMP_CRCCFG5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    smth_qp0              : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 5   ; /* [7..3]  */
        unsigned int    smth_qp1              : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 5   ; /* [15..11]  */
        unsigned int    smth_lftbits_thr0     : 8   ; /* [23..16]  */
        unsigned int    smth_lftbits_thr1     : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_CRCCFG5;

/* Define the union U_VPSS_VHD0_LCMP_CMAXLEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    max_row_len           : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_LCMP_CMAXLEN;

/* Define the union U_VPSS_DIECTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    st_rst_value          : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 1   ; /* [16]  */
        unsigned int    die_rst               : 1   ; /* [17]  */
        unsigned int    reserved_1            : 6   ; /* [23..18]  */
        unsigned int    die_chmmode           : 2   ; /* [25..24]  */
        unsigned int    die_lmmode            : 2   ; /* [27..26]  */
        unsigned int    die_out_sel_c         : 1   ; /* [28]  */
        unsigned int    die_out_sel_l         : 1   ; /* [29]  */
        unsigned int    reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIECTRL;

/* Define the union U_VPSS_DIELMA2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    recmode_frmfld_blend_mode : 1   ; /* [0]  */
        unsigned int    frame_motion_smooth_en : 1   ; /* [1]  */
        unsigned int    luma_scesdf_max       : 1   ; /* [2]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    motion_iir_en         : 1   ; /* [4]  */
        unsigned int    luma_mf_max           : 1   ; /* [5]  */
        unsigned int    chroma_mf_max         : 1   ; /* [6]  */
        unsigned int    rec_mode_en           : 1   ; /* [7]  */
        unsigned int    reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIELMA2;

/* Define the union U_VPSS_DIEINTEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    dir_inten_ver         : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    ver_min_inten         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEINTEN;

/* Define the union U_VPSS_DIESCALE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    range_scale           : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIESCALE;

/* Define the union U_VPSS_DIECHECK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ck1_max_range         : 8   ; /* [7..0]  */
        unsigned int    ck1_range_gain        : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    ck1_gain              : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIECHECK1;

/* Define the union U_VPSS_DIECHECK2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ck2_max_range         : 8   ; /* [7..0]  */
        unsigned int    ck2_range_gain        : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    ck2_gain              : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIECHECK2;

/* Define the union U_VPSS_DIEDIR0_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dir0_mult             : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dir1_mult             : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dir2_mult             : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    dir3_mult             : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEDIR0_3;

/* Define the union U_VPSS_DIEDIR4_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dir4_mult             : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dir5_mult             : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dir6_mult             : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    dir7_mult             : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEDIR4_7;

/* Define the union U_VPSS_DIEDIR8_11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dir8_mult             : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dir9_mult             : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dir10_mult            : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    dir11_mult            : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEDIR8_11;

/* Define the union U_VPSS_DIEDIR12_14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dir12_mult            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dir13_mult            : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dir14_mult            : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEDIR12_14;

/* Define the union U_VPSS_DIESTA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cur_state             : 4   ; /* [3..0]  */
        unsigned int    cur_cstate            : 4   ; /* [7..4]  */
        unsigned int    l_height_cnt          : 12  ; /* [19..8]  */
        unsigned int    c_height_cnt          : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIESTA;

/* Define the union U_VPSS_DIESTPPREADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int die_ppre_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIESTPPREADDR;
/* Define the union U_VPSS_DIESTPREADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int die_pre_addr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIESTPREADDR;
/* Define the union U_VPSS_DIEINTPSCL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intp_scale_ratio_1    : 4   ; /* [3..0]  */
        unsigned int    intp_scale_ratio_2    : 4   ; /* [7..4]  */
        unsigned int    intp_scale_ratio_3    : 4   ; /* [11..8]  */
        unsigned int    intp_scale_ratio_4    : 4   ; /* [15..12]  */
        unsigned int    intp_scale_ratio_5    : 4   ; /* [19..16]  */
        unsigned int    intp_scale_ratio_6    : 4   ; /* [23..20]  */
        unsigned int    intp_scale_ratio_7    : 4   ; /* [27..24]  */
        unsigned int    intp_scale_ratio_8    : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEINTPSCL0;

/* Define the union U_VPSS_DIEINTPSCL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intp_scale_ratio_9    : 4   ; /* [3..0]  */
        unsigned int    intp_scale_ratio_10   : 4   ; /* [7..4]  */
        unsigned int    intp_scale_ratio_11   : 4   ; /* [11..8]  */
        unsigned int    intp_scale_ratio_12   : 4   ; /* [15..12]  */
        unsigned int    intp_scale_ratio_13   : 4   ; /* [19..16]  */
        unsigned int    intp_scale_ratio_14   : 4   ; /* [23..20]  */
        unsigned int    intp_scale_ratio_15   : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEINTPSCL1;

/* Define the union U_VPSS_DIEDIRTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bc_gain               : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    dir_thd               : 4   ; /* [11..8]  */
        unsigned int    edge_mode             : 1   ; /* [12]  */
        unsigned int    hor_edge_en           : 1   ; /* [13]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    strength_thd          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEDIRTHD;

/* Define the union U_VPSS_DIEJITMTN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    jitter_gain           : 4   ; /* [3..0]  */
        unsigned int    jitter_coring         : 8   ; /* [11..4]  */
        unsigned int    fld_motion_coring     : 8   ; /* [19..12]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEJITMTN;

/* Define the union U_VPSS_DIEFLDMTN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fld_motion_thd_low    : 8   ; /* [7..0]  */
        unsigned int    fld_motion_thd_high   : 8   ; /* [15..8]  */
        unsigned int    fld_motion_curve_slope : 6   ; /* [21..16]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    fld_motion_gain       : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFLDMTN;

/* Define the union U_VPSS_DIEMTNDIFFTHD0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_diff_thd_0     : 8   ; /* [7..0]  */
        unsigned int    motion_diff_thd_1     : 8   ; /* [15..8]  */
        unsigned int    motion_diff_thd_2     : 8   ; /* [23..16]  */
        unsigned int    motion_diff_thd_3     : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMTNDIFFTHD0;

/* Define the union U_VPSS_DIEMTNDIFFTHD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_diff_thd_4     : 8   ; /* [7..0]  */
        unsigned int    motion_diff_thd_5     : 8   ; /* [15..8]  */
        unsigned int    min_motion_iir_ratio  : 7   ; /* [22..16]  */
        unsigned int    reserved_0            : 1   ; /* [23]  */
        unsigned int    max_motion_iir_ratio  : 7   ; /* [30..24]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMTNDIFFTHD1;

/* Define the union U_VPSS_DIEMTNIIRSLP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_iir_curve_slope_0 : 6   ; /* [5..0]  */
        unsigned int    motion_iir_curve_slope_1 : 6   ; /* [11..6]  */
        unsigned int    motion_iir_curve_slope_2 : 6   ; /* [17..12]  */
        unsigned int    motion_iir_curve_slope_3 : 6   ; /* [23..18]  */
        unsigned int    motion_iir_curve_ratio_0 : 7   ; /* [30..24]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMTNIIRSLP;

/* Define the union U_VPSS_DIEMTNIIRRAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_iir_curve_ratio_1 : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    motion_iir_curve_ratio_2 : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    motion_iir_curve_ratio_3 : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    motion_iir_curve_ratio_4 : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMTNIIRRAT;

/* Define the union U_VPSS_DIEHISMODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rec_mode_fld_motion_step_0 : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    rec_mode_fld_motion_step_1 : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    rec_mode_frm_motion_step_0 : 2   ; /* [9..8]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    rec_mode_frm_motion_step_1 : 2   ; /* [13..12]  */
        unsigned int    reserved_3            : 2   ; /* [15..14]  */
        unsigned int    ppre_info_en          : 1   ; /* [16]  */
        unsigned int    pre_info_en           : 1   ; /* [17]  */
        unsigned int    his_motion_en         : 1   ; /* [18]  */
        unsigned int    his_motion_using_mode : 1   ; /* [19]  */
        unsigned int    his_motion_write_mode : 1   ; /* [20]  */
        unsigned int    his_motion_info_write_mode : 1   ; /* [21]  */
        unsigned int    reserved_4            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEHISMODE;

/* Define the union U_VPSS_DIEMORFLT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mor_flt_thd           : 8   ; /* [7..0]  */
        unsigned int    mor_flt_size          : 2   ; /* [9..8]  */
        unsigned int    reserved_0            : 14  ; /* [23..10]  */
        unsigned int    mor_flt_en            : 1   ; /* [24]  */
        unsigned int    reserved_1            : 1   ; /* [25]  */
        unsigned int    med_blend_en          : 1   ; /* [26]  */
        unsigned int    reserved_2            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMORFLT;

/* Define the union U_VPSS_DIECOMBCHK0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    comb_chk_min_hthd     : 8   ; /* [7..0]  */
        unsigned int    comb_chk_min_vthd     : 8   ; /* [15..8]  */
        unsigned int    comb_chk_lower_limit  : 8   ; /* [23..16]  */
        unsigned int    comb_chk_upper_limit  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIECOMBCHK0;

/* Define the union U_VPSS_DIECOMBCHK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    comb_chk_edge_thd     : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    comb_chk_md_thd       : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    comb_chk_en           : 1   ; /* [16]  */
        unsigned int    reserved_2            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIECOMBCHK1;

/* Define the union U_VPSS_DIEFRMMTNSMTHTHD0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_motion_smooth_thd0 : 8   ; /* [7..0]  */
        unsigned int    frame_motion_smooth_thd1 : 8   ; /* [15..8]  */
        unsigned int    frame_motion_smooth_thd2 : 8   ; /* [23..16]  */
        unsigned int    frame_motion_smooth_thd3 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMMTNSMTHTHD0;

/* Define the union U_VPSS_DIEFRMMTNSMTHTHD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_motion_smooth_thd4 : 8   ; /* [7..0]  */
        unsigned int    frame_motion_smooth_thd5 : 8   ; /* [15..8]  */
        unsigned int    frame_motion_smooth_ratio_min : 7   ; /* [22..16]  */
        unsigned int    reserved_0            : 1   ; /* [23]  */
        unsigned int    frame_motion_smooth_ratio_max : 7   ; /* [30..24]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMMTNSMTHTHD1;

/* Define the union U_VPSS_DIEFRMMTNSMTHSLP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_motion_smooth_slope0 : 6   ; /* [5..0]  */
        unsigned int    frame_motion_smooth_slope1 : 6   ; /* [11..6]  */
        unsigned int    frame_motion_smooth_slope2 : 6   ; /* [17..12]  */
        unsigned int    frame_motion_smooth_slope3 : 6   ; /* [23..18]  */
        unsigned int    frame_motion_smooth_ratio0 : 7   ; /* [30..24]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMMTNSMTHSLP;

/* Define the union U_VPSS_DIEFRMMTNSMTHRAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_motion_smooth_ratio1 : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    frame_motion_smooth_ratio2 : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    frame_motion_smooth_ratio3 : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    frame_motion_smooth_ratio4 : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMMTNSMTHRAT;

/* Define the union U_VPSS_DIEFRMFLDBLENDTHD0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_field_blend_thd0 : 8   ; /* [7..0]  */
        unsigned int    frame_field_blend_thd1 : 8   ; /* [15..8]  */
        unsigned int    frame_field_blend_thd2 : 8   ; /* [23..16]  */
        unsigned int    frame_field_blend_thd3 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMFLDBLENDTHD0;

/* Define the union U_VPSS_DIEFRMFLDBLENDTHD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_field_blend_thd4 : 8   ; /* [7..0]  */
        unsigned int    frame_field_blend_thd5 : 8   ; /* [15..8]  */
        unsigned int    frame_field_blend_ratio_min : 7   ; /* [22..16]  */
        unsigned int    reserved_0            : 1   ; /* [23]  */
        unsigned int    frame_field_blend_ratio_max : 7   ; /* [30..24]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMFLDBLENDTHD1;

/* Define the union U_VPSS_DIEFRMFLDBLENDSLP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_field_blend_slope0 : 6   ; /* [5..0]  */
        unsigned int    frame_field_blend_slope1 : 6   ; /* [11..6]  */
        unsigned int    frame_field_blend_slope2 : 6   ; /* [17..12]  */
        unsigned int    frame_field_blend_slope3 : 6   ; /* [23..18]  */
        unsigned int    frame_field_blend_ratio0 : 7   ; /* [30..24]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMFLDBLENDSLP;

/* Define the union U_VPSS_DIEFRMFLDBLENDRAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_field_blend_ratio1 : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    frame_field_blend_ratio2 : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    frame_field_blend_ratio3 : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    frame_field_blend_ratio4 : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFRMFLDBLENDRAT;

/* Define the union U_VPSS_DIEMTNADJ */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_adjust_gain    : 8   ; /* [7..0]  */
        unsigned int    motion_adjust_coring  : 6   ; /* [13..8]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    motion_adjust_gain_chr : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMTNADJ;

/* Define the union U_VPSS_DIEGLBMTNTHD0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_thd0           : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    motion_thd1           : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEGLBMTNTHD0;

/* Define the union U_VPSS_DIEGLBMTNTHD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_thd2           : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    motion_thd3           : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEGLBMTNTHD1;

/* Define the union U_VPSS_CCRSCLR0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    no_ccr_detect_thd     : 8   ; /* [7..0]  */
        unsigned int    no_ccr_detect_max     : 8   ; /* [15..8]  */
        unsigned int    chroma_ma_offset      : 8   ; /* [23..16]  */
        unsigned int    chroma_ccr_en         : 1   ; /* [24]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CCRSCLR0;

/* Define the union U_VPSS_CCRSCLR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    max_xchroma           : 8   ; /* [7..0]  */
        unsigned int    no_ccr_detect_blend   : 4   ; /* [11..8]  */
        unsigned int    similar_thd           : 7   ; /* [18..12]  */
        unsigned int    reserved_0            : 1   ; /* [19]  */
        unsigned int    similar_max           : 7   ; /* [26..20]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CCRSCLR1;

/* Define the union U_VPSS_PDPHISTTHD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hist_thd0             : 8   ; /* [7..0]  */
        unsigned int    hist_thd1             : 8   ; /* [15..8]  */
        unsigned int    hist_thd2             : 8   ; /* [23..16]  */
        unsigned int    hist_thd3             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDPHISTTHD1;

/* Define the union U_VPSS_PDPCCMOV */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mov_coring_blk        : 8   ; /* [7..0]  */
        unsigned int    mov_coring_tkr        : 8   ; /* [15..8]  */
        unsigned int    mov_coring_norm       : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDPCCMOV;

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
/* Define the union U_VPSS_PDCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bitsmov2r             : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 16  ; /* [18..3]  */
        unsigned int    lasi_mode             : 1   ; /* [19]  */
        unsigned int    edge_smooth_ratio     : 8   ; /* [27..20]  */
        unsigned int    reserved_1            : 1   ; /* [28]  */
        unsigned int    dir_mch_c             : 1   ; /* [29]  */
        unsigned int    edge_smooth_en        : 1   ; /* [30]  */
        unsigned int    dir_mch_l             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDCTRL;

/* Define the union U_VPSS_PDBLKTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    stillblk_thd          : 8   ; /* [7..0]  */
        unsigned int    diff_movblk_thd       : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDBLKTHD;

/* Define the union U_VPSS_PDHISTTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mon_tkr_thr           : 8   ; /* [7..0]  */
        unsigned int    mon_start_idx         : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDHISTTHD;

/* Define the union U_VPSS_PDUMTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    um_thd0               : 8   ; /* [7..0]  */
        unsigned int    um_thd1               : 8   ; /* [15..8]  */
        unsigned int    um_thd2               : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDUMTHD;

/* Define the union U_VPSS_PDPCCCORING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    coring_tkr            : 8   ; /* [7..0]  */
        unsigned int    coring_norm           : 8   ; /* [15..8]  */
        unsigned int    coring_blk            : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDPCCCORING;

/* Define the union U_VPSS_PDPCCHTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pcc_hthd              : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDPCCHTHD;

/* Define the union U_VPSS_PDPCCVTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pcc_vthd0             : 8   ; /* [7..0]  */
        unsigned int    pcc_vthd1             : 8   ; /* [15..8]  */
        unsigned int    pcc_vthd2             : 8   ; /* [23..16]  */
        unsigned int    pcc_vthd3             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDPCCVTHD;

/* Define the union U_VPSS_PDITDIFFVTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    itdiff_vthd0          : 8   ; /* [7..0]  */
        unsigned int    itdiff_vthd1          : 8   ; /* [15..8]  */
        unsigned int    itdiff_vthd2          : 8   ; /* [23..16]  */
        unsigned int    itdiff_vthd3          : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDITDIFFVTHD;

/* Define the union U_VPSS_PDLASITHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lasi_coring_thd       : 8   ; /* [7..0]  */
        unsigned int    lasi_edge_thd         : 8   ; /* [15..8]  */
        unsigned int    lasi_mov_thd          : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDLASITHD;

/* Define the union U_VPSS_PDLASIMODE0COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lasi_txt_alpha        : 8   ; /* [7..0]  */
        unsigned int    lasi_txt_coring       : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDLASIMODE0COEF;

/* Define the union U_VPSS_PDLASIMODE1COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lasi_txt_thd0         : 8   ; /* [7..0]  */
        unsigned int    lasi_txt_thd1         : 8   ; /* [15..8]  */
        unsigned int    lasi_txt_thd2         : 8   ; /* [23..16]  */
        unsigned int    lasi_txt_thd3         : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDLASIMODE1COEF;

/* Define the union U_VPSS_PDREGION */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    region1_y_end         : 12  ; /* [11..0]  */
        unsigned int    region1_y_stt         : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDREGION;

/* Define the union U_VPSS_TNR_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_lpf_mode           : 2   ; /* [1..0]  */
        unsigned int    te_ylpf_mode          : 2   ; /* [3..2]  */
        unsigned int    te_clpf_mode          : 2   ; /* [5..4]  */
        unsigned int    tm_ymt_mean_mode      : 2   ; /* [7..6]  */
        unsigned int    tm_cmt_mean_mode      : 2   ; /* [9..8]  */
        unsigned int    tm_ymt_mode           : 2   ; /* [11..10]  */
        unsigned int    tm_cmt_mode           : 2   ; /* [13..12]  */
        unsigned int    te_yeg_mean_mode      : 2   ; /* [15..14]  */
        unsigned int    te_ceg_mean_mode      : 2   ; /* [17..16]  */
        unsigned int    te_yceg_mode          : 3   ; /* [20..18]  */
        unsigned int    tfm_lpf_en            : 1   ; /* [21]  */
        unsigned int    edge_ymoving_lpfmode  : 2   ; /* [23..22]  */
        unsigned int    edge_cmoving_lpfmode  : 2   ; /* [25..24]  */
        unsigned int    hist_motion_en        : 1   ; /* [26]  */
        unsigned int    tnr_c_bypass          : 1   ; /* [27]  */
        unsigned int    tnr_y_bypass          : 1   ; /* [28]  */
        unsigned int    reserved_0            : 1   ; /* [29]  */
        unsigned int    market_mode           : 1   ; /* [30]  */
        unsigned int    market_en             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MODE;

/* Define the union U_VPSS_TNR_YMOTION_MAP_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_mapping_min    : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_mapping_max    : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 11  ; /* [30..20]  */
        unsigned int    tm_ymt_meanratio_en   : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MAP_CTRL;

/* Define the union U_VPSS_TNR_YMOTION_MAP_R0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_mapping_r0     : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_mapping_r1     : 10  ; /* [19..10]  */
        unsigned int    tm_ymt_mapping_r2     : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MAP_R0;

/* Define the union U_VPSS_TNR_YMOTION_MAP_R1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_mapping_r3     : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_mapping_r4     : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MAP_R1;

/* Define the union U_VPSS_TNR_YMOTION_MAP_T1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_mapping_t1     : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_mapping_t2     : 10  ; /* [19..10]  */
        unsigned int    tm_ymt_mapping_t3     : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MAP_T1;

/* Define the union U_VPSS_TNR_YMOTION_MAP_T2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_mapping_t4     : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_mapping_t5     : 10  ; /* [19..10]  */
        unsigned int    tm_ymt_mapping_t6     : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MAP_T2;

/* Define the union U_VPSS_TNR_YMOTION_MAP_K1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_mapping_k1     : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_mapping_k2     : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MAP_K1;

/* Define the union U_VPSS_TNR_YMOTION_MAP_K2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_mapping_k3     : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_mapping_k4     : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MAP_K2;

/* Define the union U_VPSS_TNR_CMOTION_MAP_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_mapping_min    : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_mapping_max    : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 11  ; /* [30..20]  */
        unsigned int    tm_cmt_meanratio_en   : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MAP_CTRL;

/* Define the union U_VPSS_TNR_CMOTION_MAP_R0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_mapping_r0     : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_mapping_r1     : 10  ; /* [19..10]  */
        unsigned int    tm_cmt_mapping_r2     : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MAP_R0;

/* Define the union U_VPSS_TNR_CMOTION_MAP_R1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_mapping_r3     : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_mapping_r4     : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MAP_R1;

/* Define the union U_VPSS_TNR_CMOTION_MAP_T1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_mapping_t1     : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_mapping_t2     : 10  ; /* [19..10]  */
        unsigned int    tm_cmt_mapping_t3     : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MAP_T1;

/* Define the union U_VPSS_TNR_CMOTION_MAP_T2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_mapping_t4     : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_mapping_t5     : 10  ; /* [19..10]  */
        unsigned int    tm_cmt_mapping_t6     : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MAP_T2;

/* Define the union U_VPSS_TNR_CMOTION_MAP_K1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_mapping_k1     : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_mapping_k2     : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MAP_K1;

/* Define the union U_VPSS_TNR_CMOTION_MAP_K2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_mapping_k3     : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_mapping_k4     : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MAP_K2;

/* Define the union U_VPSS_TNR_YMOTION_MEANRATIO_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_meanratio_min  : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_meanratio_max  : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MEANRATIO_CTRL;

/* Define the union U_VPSS_TNR_YMOTION_MEANRATIO_R0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_meanratio_r0   : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_meanratio_r1   : 10  ; /* [19..10]  */
        unsigned int    tm_ymt_meanratio_r2   : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MEANRATIO_R0;

/* Define the union U_VPSS_TNR_YMOTION_MEANRATIO_R1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_meanratio_r3   : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_meanratio_r4   : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MEANRATIO_R1;

/* Define the union U_VPSS_TNR_YMOTION_MEANRATIO_T1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_meanratio_t1   : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_meanratio_t2   : 10  ; /* [19..10]  */
        unsigned int    tm_ymt_meanratio_t3   : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MEANRATIO_T1;

/* Define the union U_VPSS_TNR_YMOTION_MEANRATIO_T2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_meanratio_t4   : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_meanratio_t5   : 10  ; /* [19..10]  */
        unsigned int    tm_ymt_meanratio_t6   : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MEANRATIO_T2;

/* Define the union U_VPSS_TNR_YMOTION_MEANRATIO_K1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_meanratio_k1   : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_meanratio_k2   : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MEANRATIO_K1;

/* Define the union U_VPSS_TNR_YMOTION_MEANRATIO_K2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_meanratio_k3   : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_meanratio_k4   : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTION_MEANRATIO_K2;

/* Define the union U_VPSS_TNR_CMOTION_MEANRATIO_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_meanratio_min  : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_meanratio_max  : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MEANRATIO_CTRL;

/* Define the union U_VPSS_TNR_CMOTION_MEANRATIO_R0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_meanratio_r0   : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_meanratio_r1   : 10  ; /* [19..10]  */
        unsigned int    tm_cmt_meanratio_r2   : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MEANRATIO_R0;

/* Define the union U_VPSS_TNR_CMOTION_MEANRATIO_R1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_meanratio_r3   : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_meanratio_r4   : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MEANRATIO_R1;

/* Define the union U_VPSS_TNR_CMOTION_MEANRATIO_T1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_meanratio_t1   : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_meanratio_t2   : 10  ; /* [19..10]  */
        unsigned int    tm_cmt_meanratio_t3   : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MEANRATIO_T1;

/* Define the union U_VPSS_TNR_CMOTION_MEANRATIO_T2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_meanratio_t4   : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_meanratio_t5   : 10  ; /* [19..10]  */
        unsigned int    tm_cmt_meanratio_t6   : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MEANRATIO_T2;

/* Define the union U_VPSS_TNR_CMOTION_MEANRATIO_K1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_meanratio_k1   : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_meanratio_k2   : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MEANRATIO_K1;

/* Define the union U_VPSS_TNR_CMOTION_MEANRATIO_K2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_meanratio_k3   : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_meanratio_k4   : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTION_MEANRATIO_K2;

/* Define the union U_VPSS_TNR_TM_YCMTCOEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ycmtcoef0          : 6   ; /* [5..0]  */
        unsigned int    tm_ycmtcoef1          : 6   ; /* [11..6]  */
        unsigned int    tm_ycmtcoef2          : 6   ; /* [17..12]  */
        unsigned int    tm_ycmtcoef3          : 6   ; /* [23..18]  */
        unsigned int    tm_ycmtcoef4          : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TM_YCMTCOEF;

/* Define the union U_VPSS_TNR_TM_YCLPFCOEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_yclpfcoef0         : 6   ; /* [5..0]  */
        unsigned int    tm_yclpfcoef1         : 6   ; /* [11..6]  */
        unsigned int    tm_yclpfcoef2         : 6   ; /* [17..12]  */
        unsigned int    tm_yclpfcoef3         : 6   ; /* [23..18]  */
        unsigned int    tm_yclpfcoef4         : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TM_YCLPFCOEF;

/* Define the union U_VPSS_TNR_TE_YHPF1_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yhpf1coef0         : 4   ; /* [3..0]  */
        unsigned int    te_yhpf1coef1         : 4   ; /* [7..4]  */
        unsigned int    te_yhpf1coef2         : 4   ; /* [11..8]  */
        unsigned int    te_yhpf1coef3         : 4   ; /* [15..12]  */
        unsigned int    te_yhpf1coef4         : 4   ; /* [19..16]  */
        unsigned int    te_yhpf1coef5         : 4   ; /* [23..20]  */
        unsigned int    te_yhpf1coef6         : 4   ; /* [27..24]  */
        unsigned int    te_yhpf1coef7         : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YHPF1_0;

/* Define the union U_VPSS_TNR_TE_YHPF1_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yhpf1coef8         : 4   ; /* [3..0]  */
        unsigned int    te_yhpf1lshiift       : 3   ; /* [6..4]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YHPF1_1;

/* Define the union U_VPSS_TNR_TE_YHPF2_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yhpf2coef0         : 4   ; /* [3..0]  */
        unsigned int    te_yhpf2coef1         : 4   ; /* [7..4]  */
        unsigned int    te_yhpf2coef2         : 4   ; /* [11..8]  */
        unsigned int    te_yhpf2coef3         : 4   ; /* [15..12]  */
        unsigned int    te_yhpf2coef4         : 4   ; /* [19..16]  */
        unsigned int    te_yhpf2coef5         : 4   ; /* [23..20]  */
        unsigned int    te_yhpf2coef6         : 4   ; /* [27..24]  */
        unsigned int    te_yhpf2coef7         : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YHPF2_0;

/* Define the union U_VPSS_TNR_TE_YHPF2_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yhpf2coef8         : 4   ; /* [3..0]  */
        unsigned int    te_yhpf2lshiift       : 3   ; /* [6..4]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YHPF2_1;

/* Define the union U_VPSS_TNR_TE_YHPF3_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yhpf3coef0         : 4   ; /* [3..0]  */
        unsigned int    te_yhpf3coef1         : 4   ; /* [7..4]  */
        unsigned int    te_yhpf3coef2         : 4   ; /* [11..8]  */
        unsigned int    te_yhpf3coef3         : 4   ; /* [15..12]  */
        unsigned int    te_yhpf3coef4         : 4   ; /* [19..16]  */
        unsigned int    te_yhpf3coef5         : 4   ; /* [23..20]  */
        unsigned int    te_yhpf3coef6         : 4   ; /* [27..24]  */
        unsigned int    te_yhpf3coef7         : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YHPF3_0;

/* Define the union U_VPSS_TNR_TE_YHPF3_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yhpf3coef8         : 4   ; /* [3..0]  */
        unsigned int    te_yhpf3lshiift       : 3   ; /* [6..4]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YHPF3_1;

/* Define the union U_VPSS_TNR_TE_YHPF4_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yhpf4coef0         : 4   ; /* [3..0]  */
        unsigned int    te_yhpf4coef1         : 4   ; /* [7..4]  */
        unsigned int    te_yhpf4coef2         : 4   ; /* [11..8]  */
        unsigned int    te_yhpf4coef3         : 4   ; /* [15..12]  */
        unsigned int    te_yhpf4coef4         : 4   ; /* [19..16]  */
        unsigned int    te_yhpf4coef5         : 4   ; /* [23..20]  */
        unsigned int    te_yhpf4coef6         : 4   ; /* [27..24]  */
        unsigned int    te_yhpf4coef7         : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YHPF4_0;

/* Define the union U_VPSS_TNR_TE_YHPF4_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yhpf4coef8         : 4   ; /* [3..0]  */
        unsigned int    te_yhpf4lshiift       : 3   ; /* [6..4]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YHPF4_1;

/* Define the union U_VPSS_TNR_TE_YMTMEAN_LPFCOEF_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ymtmeanlpfcoef0    : 6   ; /* [5..0]  */
        unsigned int    te_ymtmeanlpfcoef1    : 6   ; /* [11..6]  */
        unsigned int    te_ymtmeanlpfcoef2    : 6   ; /* [17..12]  */
        unsigned int    te_ymtmeanlpfcoef3    : 6   ; /* [23..18]  */
        unsigned int    te_ymtmeanlpfcoef4    : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YMTMEAN_LPFCOEF_0;

/* Define the union U_VPSS_TNR_TE_YMTMEAN_LPFCOEF_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ymtmeanlpfcoef5    : 6   ; /* [5..0]  */
        unsigned int    te_ymtmeanlpfcoef6    : 6   ; /* [11..6]  */
        unsigned int    te_ymtmeanlpfcoef7    : 6   ; /* [17..12]  */
        unsigned int    te_ymtmeanlpfcoef8    : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YMTMEAN_LPFCOEF_1;

/* Define the union U_VPSS_TNR_TE_YCEG_MEANRATIO_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_meanratio_min : 10  ; /* [9..0]  */
        unsigned int    te_yceg_meanratio_max : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 11  ; /* [30..20]  */
        unsigned int    te_yceg_meanratio_en  : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MEANRATIO_CTRL;

/* Define the union U_VPSS_TNR_TE_YCEG_MEANRATIO_R0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_meanratio_r0  : 10  ; /* [9..0]  */
        unsigned int    te_yceg_meanratio_r1  : 10  ; /* [19..10]  */
        unsigned int    te_yceg_meanratio_r2  : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MEANRATIO_R0;

/* Define the union U_VPSS_TNR_TE_YCEG_MEANRATIO_R1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_meanratio_r3  : 10  ; /* [9..0]  */
        unsigned int    te_yceg_meanratio_r4  : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MEANRATIO_R1;

/* Define the union U_VPSS_TNR_TE_YCEG_MEANRATIO_T1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_meanratio_t1  : 10  ; /* [9..0]  */
        unsigned int    te_yceg_meanratio_t2  : 10  ; /* [19..10]  */
        unsigned int    te_yceg_meanratio_t3  : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MEANRATIO_T1;

/* Define the union U_VPSS_TNR_TE_YCEG_MEANRATIO_T2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_meanratio_t4  : 10  ; /* [9..0]  */
        unsigned int    te_yceg_meanratio_t5  : 10  ; /* [19..10]  */
        unsigned int    te_yceg_meanratio_t6  : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MEANRATIO_T2;

/* Define the union U_VPSS_TNR_TE_YCEG_MEANRATIO_K1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_meanratio_k1  : 10  ; /* [9..0]  */
        unsigned int    te_yceg_meanratio_k2  : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MEANRATIO_K1;

/* Define the union U_VPSS_TNR_TE_YCEG_MEANRATIO_K2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_meanratio_k3  : 10  ; /* [9..0]  */
        unsigned int    te_yceg_meanratio_k4  : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MEANRATIO_K2;

/* Define the union U_VPSS_TNR_TE_CMT_MEANRATIO_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_meanratio_min : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_meanratio_max : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 11  ; /* [30..20]  */
        unsigned int    te_ycmt_meanratio_en  : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CMT_MEANRATIO_CTRL;

/* Define the union U_VPSS_TNR_TE_CMT_MEANRATIO_R0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_meanratio_r0  : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_meanratio_r1  : 10  ; /* [19..10]  */
        unsigned int    te_ycmt_meanratio_r2  : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CMT_MEANRATIO_R0;

/* Define the union U_VPSS_TNR_TE_CMT_MEANRATIO_R1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_meanratio_r3  : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_meanratio_r4  : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CMT_MEANRATIO_R1;

/* Define the union U_VPSS_TNR_TE_CMT_MEANRATIO_T1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_meanratio_t1  : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_meanratio_t2  : 10  ; /* [19..10]  */
        unsigned int    te_ycmt_meanratio_t3  : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CMT_MEANRATIO_T1;

/* Define the union U_VPSS_TNR_TE_CMT_MEANRATIO_T2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_meanratio_t4  : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_meanratio_t5  : 10  ; /* [19..10]  */
        unsigned int    te_ycmt_meanratio_t6  : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CMT_MEANRATIO_T2;

/* Define the union U_VPSS_TNR_TE_CMT_MEANRATIO_K1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_meanratio_k1  : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_meanratio_k2  : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CMT_MEANRATIO_K1;

/* Define the union U_VPSS_TNR_TE_CMT_MEANRATIO_K2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_meanratio_k3  : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_meanratio_k4  : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CMT_MEANRATIO_K2;

/* Define the union U_VPSS_TNR_TE_YCMT_MAPPING_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_mapping_min   : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_mapping_max   : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCMT_MAPPING_CTRL;

/* Define the union U_VPSS_TNR_TE_YCMT_MAPPING_R0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_mapping_r0    : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_mapping_r1    : 10  ; /* [19..10]  */
        unsigned int    te_ycmt_mapping_r2    : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCMT_MAPPING_R0;

/* Define the union U_VPSS_TNR_TE_YCMT_MAPPING_R1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_mapping_r3    : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_mapping_r4    : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCMT_MAPPING_R1;

/* Define the union U_VPSS_TNR_TE_YCMT_MAPPING_T1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_mapping_t1    : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_mapping_t2    : 10  ; /* [19..10]  */
        unsigned int    te_ycmt_mapping_t3    : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCMT_MAPPING_T1;

/* Define the union U_VPSS_TNR_TE_YCMT_MAPPING_T2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_mapping_t4    : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_mapping_t5    : 10  ; /* [19..10]  */
        unsigned int    te_ycmt_mapping_t6    : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCMT_MAPPING_T2;

/* Define the union U_VPSS_TNR_TE_YCMT_MAPPING_K1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_mapping_k1    : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_mapping_k2    : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCMT_MAPPING_K1;

/* Define the union U_VPSS_TNR_TE_YCMT_MAPPING_K2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ycmt_mapping_k3    : 10  ; /* [9..0]  */
        unsigned int    te_ycmt_mapping_k4    : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCMT_MAPPING_K2;

/* Define the union U_VPSS_TNR_TE_YCEG_MAPPING_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_mapping_min   : 10  ; /* [9..0]  */
        unsigned int    te_yceg_mapping_max   : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MAPPING_CTRL;

/* Define the union U_VPSS_TNR_TE_YCEG_MAPPING_R0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_mapping_r0    : 10  ; /* [9..0]  */
        unsigned int    te_yceg_mapping_r1    : 10  ; /* [19..10]  */
        unsigned int    te_yceg_mapping_r2    : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MAPPING_R0;

/* Define the union U_VPSS_TNR_TE_YCEG_MAPPING_R1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_mapping_r3    : 10  ; /* [9..0]  */
        unsigned int    te_yceg_mapping_r4    : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MAPPING_R1;

/* Define the union U_VPSS_TNR_TE_YCEG_MAPPING_T1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_mapping_t1    : 10  ; /* [9..0]  */
        unsigned int    te_yceg_mapping_t2    : 10  ; /* [19..10]  */
        unsigned int    te_yceg_mapping_t3    : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MAPPING_T1;

/* Define the union U_VPSS_TNR_TE_YCEG_MAPPING_T2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_mapping_t4    : 10  ; /* [9..0]  */
        unsigned int    te_yceg_mapping_t5    : 10  ; /* [19..10]  */
        unsigned int    te_yceg_mapping_t6    : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MAPPING_T2;

/* Define the union U_VPSS_TNR_TE_YCEG_MAPPING_K1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_mapping_k1    : 10  ; /* [9..0]  */
        unsigned int    te_yceg_mapping_k2    : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MAPPING_K1;

/* Define the union U_VPSS_TNR_TE_YCEG_MAPPING_K2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yceg_mapping_k3    : 10  ; /* [9..0]  */
        unsigned int    te_yceg_mapping_k4    : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YCEG_MAPPING_K2;

/* Define the union U_VPSS_TNR_TE_CEG_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ceg_core           : 10  ; /* [9..0]  */
        unsigned int    te_ceg_k              : 10  ; /* [19..10]  */
        unsigned int    te_crshift            : 4   ; /* [23..20]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CEG_CK;

/* Define the union U_VPSS_TNR_TE_CEG_CK2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ceg_core2          : 10  ; /* [9..0]  */
        unsigned int    te_ceg_k2             : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CEG_CK2;

/* Define the union U_VPSS_TNR_TE_CMT_CK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_cmt_core1          : 10  ; /* [9..0]  */
        unsigned int    te_cmt_k1             : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CMT_CK1;

/* Define the union U_VPSS_TNR_TE_CMT_CK2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_cmt_core2          : 10  ; /* [9..0]  */
        unsigned int    te_cmt_k2             : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CMT_CK2;

/* Define the union U_VPSS_TNR_TE_CMT_CK3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_cmt_core3          : 10  ; /* [9..0]  */
        unsigned int    te_cmt_k3             : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CMT_CK3;

/* Define the union U_VPSS_TNR_TE_CHPF4_COEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_chpf4coef0         : 4   ; /* [3..0]  */
        unsigned int    te_chpf4coef1         : 4   ; /* [7..4]  */
        unsigned int    te_chpf4coef2         : 4   ; /* [11..8]  */
        unsigned int    te_chpf4coef3         : 4   ; /* [15..12]  */
        unsigned int    te_chpf4coef4         : 4   ; /* [19..16]  */
        unsigned int    te_chpf4coef5         : 4   ; /* [23..20]  */
        unsigned int    te_chpf4coef6         : 4   ; /* [27..24]  */
        unsigned int    te_chpf4coef7         : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CHPF4_COEF0;

/* Define the union U_VPSS_TNR_TE_CHPF4_COEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_chpf4coef8         : 4   ; /* [3..0]  */
        unsigned int    te_chpf4lshiift       : 3   ; /* [6..4]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CHPF4_COEF1;

/* Define the union U_VPSS_TNR_TE_YEG_WEIGHT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yeg_weight1        : 6   ; /* [5..0]  */
        unsigned int    te_yeg_weight2        : 6   ; /* [11..6]  */
        unsigned int    te_yeg_weight3        : 6   ; /* [17..12]  */
        unsigned int    te_yeg_weight4        : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YEG_WEIGHT;

/* Define the union U_VPSS_TNR_TE_CEG_WEIGHT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ceg_weight1        : 6   ; /* [5..0]  */
        unsigned int    te_ceg_weight2        : 6   ; /* [11..6]  */
        unsigned int    te_ceg_weight3        : 6   ; /* [17..12]  */
        unsigned int    te_ceg_weight4        : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CEG_WEIGHT;

/* Define the union U_VPSS_TNR_TE_CMEAN_LPFCOEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_cmean_lpfcoef0     : 6   ; /* [5..0]  */
        unsigned int    te_cmean_lpfcoef1     : 6   ; /* [11..6]  */
        unsigned int    te_cmean_lpfcoef2     : 6   ; /* [17..12]  */
        unsigned int    te_cmean_lpfcoef3     : 6   ; /* [23..18]  */
        unsigned int    te_cmean_lpfcoef4     : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CMEAN_LPFCOEF1;

/* Define the union U_VPSS_TNR_TE_CMEAN_LPFCOEF2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_cmean_lpfcoef5     : 6   ; /* [5..0]  */
        unsigned int    te_cmean_lpfcoef6     : 6   ; /* [11..6]  */
        unsigned int    te_cmean_lpfcoef7     : 6   ; /* [17..12]  */
        unsigned int    te_cmean_lpfcoef8     : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_CMEAN_LPFCOEF2;

/* Define the union U_VPSS_TNR_TE_YMT_CK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ymt_core1          : 10  ; /* [9..0]  */
        unsigned int    te_ymt_k1             : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YMT_CK1;

/* Define the union U_VPSS_TNR_TE_YMT_CK2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ymt_core2          : 10  ; /* [9..0]  */
        unsigned int    te_ymt_k2             : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YMT_CK2;

/* Define the union U_VPSS_TNR_TE_YEG_CK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yeg_core1          : 10  ; /* [9..0]  */
        unsigned int    te_yeg_k1             : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YEG_CK1;

/* Define the union U_VPSS_TNR_TE_YMT_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ymt_core           : 10  ; /* [9..0]  */
        unsigned int    te_ymt_k              : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YMT_CK;

/* Define the union U_VPSS_TNR_TE_YEG_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_yeg_core           : 10  ; /* [9..0]  */
        unsigned int    te_yeg_k              : 10  ; /* [19..10]  */
        unsigned int    te_yrshift            : 4   ; /* [23..20]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YEG_CK;

/* Define the union U_VPSS_TNR_TE_YMOVING_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    te_ymoving_core       : 10  ; /* [9..0]  */
        unsigned int    te_ymoving_k          : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TE_YMOVING_CK;

/* Define the union U_VPSS_TNR_TFM_CMOTION_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_cmotion_core      : 10  ; /* [9..0]  */
        unsigned int    tfm_cmotion_k         : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_CMOTION_CK;

/* Define the union U_VPSS_TNR_TFM_CEDGE_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_cedge_core        : 10  ; /* [9..0]  */
        unsigned int    tfm_cedge_k           : 10  ; /* [19..10]  */
        unsigned int    tfm_2dupdate          : 1   ; /* [20]  */
        unsigned int    tfm_cbcrupdate        : 1   ; /* [21]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_CEDGE_CK;

/* Define the union U_VPSS_TNR_TFM_Y_MAPPING_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_y_mapping_min     : 10  ; /* [9..0]  */
        unsigned int    tfm_y_mapping_max     : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_Y_MAPPING_CTRL;

/* Define the union U_VPSS_TNR_TFM_Y_MAPPING_R0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_y_mapping_r0      : 10  ; /* [9..0]  */
        unsigned int    tfm_y_mapping_r1      : 10  ; /* [19..10]  */
        unsigned int    tfm_y_mapping_r2      : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_Y_MAPPING_R0;

/* Define the union U_VPSS_TNR_TFM_Y_MAPPING_R1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_y_mapping_r3      : 10  ; /* [9..0]  */
        unsigned int    tfm_y_mapping_r4      : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_Y_MAPPING_R1;

/* Define the union U_VPSS_TNR_TFM_Y_MAPPING_T1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_y_mapping_t1      : 10  ; /* [9..0]  */
        unsigned int    tfm_y_mapping_t2      : 10  ; /* [19..10]  */
        unsigned int    tfm_y_mapping_t3      : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_Y_MAPPING_T1;

/* Define the union U_VPSS_TNR_TFM_Y_MAPPING_T2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_y_mapping_t4      : 10  ; /* [9..0]  */
        unsigned int    tfm_y_mapping_t5      : 10  ; /* [19..10]  */
        unsigned int    tfm_y_mapping_t6      : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_Y_MAPPING_T2;

/* Define the union U_VPSS_TNR_TFM_Y_MAPPING_K1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_y_mapping_k1      : 10  ; /* [9..0]  */
        unsigned int    tfm_y_mapping_k2      : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_Y_MAPPING_K1;

/* Define the union U_VPSS_TNR_TFM_Y_MAPPING_K2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_y_mapping_k3      : 10  ; /* [9..0]  */
        unsigned int    tfm_y_mapping_k4      : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_Y_MAPPING_K2;

/* Define the union U_VPSS_TNR_TFM_C_MAPPING_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_c_mapping_min     : 10  ; /* [9..0]  */
        unsigned int    tfm_c_mapping_max     : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_C_MAPPING_CTRL;

/* Define the union U_VPSS_TNR_TFM_C_MAPPING_R0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_c_mapping_r0      : 10  ; /* [9..0]  */
        unsigned int    tfm_c_mapping_r1      : 10  ; /* [19..10]  */
        unsigned int    tfm_c_mapping_r2      : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_C_MAPPING_R0;

/* Define the union U_VPSS_TNR_TFM_C_MAPPING_R1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_c_mapping_r3      : 10  ; /* [9..0]  */
        unsigned int    tfm_c_mapping_r4      : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_C_MAPPING_R1;

/* Define the union U_VPSS_TNR_TFM_C_MAPPING_T1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_c_mapping_t1      : 10  ; /* [9..0]  */
        unsigned int    tfm_c_mapping_t2      : 10  ; /* [19..10]  */
        unsigned int    tfm_c_mapping_t3      : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_C_MAPPING_T1;

/* Define the union U_VPSS_TNR_TFM_C_MAPPING_T2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_c_mapping_t4      : 10  ; /* [9..0]  */
        unsigned int    tfm_c_mapping_t5      : 10  ; /* [19..10]  */
        unsigned int    tfm_c_mapping_t6      : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_C_MAPPING_T2;

/* Define the union U_VPSS_TNR_TFM_C_MAPPING_K1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_c_mapping_k1      : 10  ; /* [9..0]  */
        unsigned int    tfm_c_mapping_k2      : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_C_MAPPING_K1;

/* Define the union U_VPSS_TNR_TFM_C_MAPPING_K2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_c_mapping_k3      : 10  ; /* [9..0]  */
        unsigned int    tfm_c_mapping_k4      : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_C_MAPPING_K2;

/* Define the union U_VPSS_TNR_TFM_HISTMOTION_BLEND */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_hism_term1        : 10  ; /* [9..0]  */
        unsigned int    tfm_hism_term2        : 10  ; /* [19..10]  */
        unsigned int    tfm_hism_alpha1       : 6   ; /* [25..20]  */
        unsigned int    tfm_hism_alpha2       : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_HISTMOTION_BLEND;

/* Define the union U_VPSS_TNR_TFM_PARA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_filter_mode       : 1   ; /* [0]  */
        unsigned int    tfm_calc_mode         : 2   ; /* [2..1]  */
        unsigned int    tfm_scenechange_en    : 1   ; /* [3]  */
        unsigned int    tfm_scenechangeformotion_en : 1   ; /* [4]  */
        unsigned int    tfm_globalmotion_en   : 1   ; /* [5]  */
        unsigned int    tfm_cbcrupdate_mode   : 1   ; /* [6]  */
        unsigned int    tfm_1dlpf_mode        : 1   ; /* [7]  */
        unsigned int    ymotion_mode          : 1   ; /* [8]  */
        unsigned int    cmotion_mode          : 1   ; /* [9]  */
        unsigned int    tfm_globalmotion      : 8   ; /* [17..10]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_PARA;

/* Define the union U_VPSS_TNR_TFM_YMOTION_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_ymotion_core      : 10  ; /* [9..0]  */
        unsigned int    tfm_ymotion_k         : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_YMOTION_CK;

/* Define the union U_VPSS_TNR_TFM_YEG_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tfm_yedge_core        : 10  ; /* [9..0]  */
        unsigned int    tfm_yedge_k           : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TFM_YEG_CK;

/* Define the union U_VPSS_TNR_TM_YMTMEAN_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymtmean_core       : 10  ; /* [9..0]  */
        unsigned int    tm_ymtmean_k          : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TM_YMTMEAN_CK;

/* Define the union U_VPSS_TNR_TYMYCK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tymy_core             : 10  ; /* [9..0]  */
        unsigned int    tymy_k                : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TYMYCK;

/* Define the union U_VPSS_TNR_TYMEYCK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tymey_core            : 10  ; /* [9..0]  */
        unsigned int    tymey_k               : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TYMEYCK;

/* Define the union U_VPSS_TNR_TCMYCK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tcmy_core             : 10  ; /* [9..0]  */
        unsigned int    tcmy_k                : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TCMYCK;

/* Define the union U_VPSS_TNR_TCMEYCK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tcmey_core            : 10  ; /* [9..0]  */
        unsigned int    tcmey_k               : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TCMEYCK;

/* Define the union U_VPSS_TNR_TM_CMT_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmt_core           : 10  ; /* [9..0]  */
        unsigned int    tm_cmt_k              : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TM_CMT_CK;

/* Define the union U_VPSS_TNR_TM_YMT_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_ymt_core           : 10  ; /* [9..0]  */
        unsigned int    tm_ymt_k              : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TM_YMT_CK;

/* Define the union U_VPSS_TNR_TM_CMTMEAN_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tm_cmtmean_core       : 10  ; /* [9..0]  */
        unsigned int    tm_cmtmean_k          : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_TM_CMTMEAN_CK;

/* Define the union U_VPSS_TNR_SNR_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    ynr_1d_en             : 1   ; /* [1]  */
        unsigned int    cnr_1d_en             : 1   ; /* [2]  */
        unsigned int    sencechange           : 8   ; /* [10..3]  */
        unsigned int    reserved_1            : 8   ; /* [18..11]  */
        unsigned int    ctrandnoise_en        : 1   ; /* [19]  */
        unsigned int    ctrandnoise_bit       : 3   ; /* [22..20]  */
        unsigned int    ytrandnoise_en        : 1   ; /* [23]  */
        unsigned int    ytrandnoise_bit       : 3   ; /* [26..24]  */
        unsigned int    t2dedgemode           : 1   ; /* [27]  */
        unsigned int    reserved_2            : 1   ; /* [28]  */
        unsigned int    t2dmeanen             : 1   ; /* [29]  */
        unsigned int    t2dmeden              : 1   ; /* [30]  */
        unsigned int    t2dmmen               : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_SNR_CTRL;

/* Define the union U_VPSS_TNR_BLEND_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    s_change              : 8   ; /* [9..2]  */
        unsigned int    cmotion_strength_en   : 1   ; /* [10]  */
        unsigned int    ymotion_strength_en   : 1   ; /* [11]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_BLEND_CTRL;

/* Define the union U_VPSS_TNR_T2D_EDGE_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    t2d_edge_core         : 10  ; /* [9..0]  */
        unsigned int    t2d_edge_k            : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_T2D_EDGE_CK;

/* Define the union U_VPSS_TNR_T2D_NOISE_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    t2d_noise_core        : 10  ; /* [9..0]  */
        unsigned int    t2d_noise_k           : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_T2D_NOISE_CK;

/* Define the union U_VPSS_TNR_EDGE_CMOVING_CK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edge_cmoving_core     : 10  ; /* [9..0]  */
        unsigned int    edge_cmoving_k        : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_EDGE_CMOVING_CK;

/* Define the union U_VPSS_TNR_GLBMTN_THD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    small_motion_thd_pglb : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 2   ; /* [11..10]  */
        unsigned int    tgm_get_mode          : 2   ; /* [13..12]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    large_motion_thd_pglb : 10  ; /* [25..16]  */
        unsigned int    reserved_2            : 2   ; /* [27..26]  */
        unsigned int    motion_lfshift_pglb   : 2   ; /* [29..28]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_GLBMTN_THD;

/* Define the union U_VPSS_NLE_LPF_COEFF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nle_lpf_coeff0        : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    nle_lpf_coeff1        : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    nle_lpf_coeff2        : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 3   ; /* [23..21]  */
        unsigned int    nle_lpf_coeff3        : 5   ; /* [28..24]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NLE_LPF_COEFF0;

/* Define the union U_VPSS_NLE_LPF_COEFF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nle_lpf_coeff4        : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    nle_lpf_coeff5        : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    nle_lpf_coeff6        : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NLE_LPF_COEFF1;

/* Define the union U_VPSS_NLE_OFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nle_w_offset          : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    nle_h_offset          : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NLE_OFFSET;

/* Define the union U_VPSS_NLE_NOISE_ZERO_RATIO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nle_noise_zero_ratio  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NLE_NOISE_ZERO_RATIO;

/* Define the union U_VPSS_TNR_CLUT10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT10;

/* Define the union U_VPSS_TNR_CLUT11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT11;

/* Define the union U_VPSS_TNR_CLUT12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT12;

/* Define the union U_VPSS_TNR_CLUT13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT13;

/* Define the union U_VPSS_TNR_CLUT14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT14;

/* Define the union U_VPSS_TNR_CLUT15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT15;

/* Define the union U_VPSS_TNR_CLUT16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT16;

/* Define the union U_VPSS_TNR_CLUT17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT17;

/* Define the union U_VPSS_TNR_CLUT18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT18;

/* Define the union U_VPSS_TNR_CLUT19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT19;

/* Define the union U_VPSS_TNR_CLUT110 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT110;

/* Define the union U_VPSS_TNR_CLUT111 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT111;

/* Define the union U_VPSS_TNR_CLUT112 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT112;

/* Define the union U_VPSS_TNR_CLUT113 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT113;

/* Define the union U_VPSS_TNR_CLUT114 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT114;

/* Define the union U_VPSS_TNR_CLUT115 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT115;

/* Define the union U_VPSS_TNR_CLUT20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT20;

/* Define the union U_VPSS_TNR_CLUT21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT21;

/* Define the union U_VPSS_TNR_CLUT22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT22;

/* Define the union U_VPSS_TNR_CLUT23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT23;

/* Define the union U_VPSS_TNR_CLUT24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT24;

/* Define the union U_VPSS_TNR_CLUT25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT25;

/* Define the union U_VPSS_TNR_CLUT26 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT26;

/* Define the union U_VPSS_TNR_CLUT27 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT27;

/* Define the union U_VPSS_TNR_CLUT28 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT28;

/* Define the union U_VPSS_TNR_CLUT29 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT29;

/* Define the union U_VPSS_TNR_CLUT210 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT210;

/* Define the union U_VPSS_TNR_CLUT211 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT211;

/* Define the union U_VPSS_TNR_CLUT212 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT212;

/* Define the union U_VPSS_TNR_CLUT213 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT213;

/* Define the union U_VPSS_TNR_CLUT214 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT214;

/* Define the union U_VPSS_TNR_CLUT215 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT215;

/* Define the union U_VPSS_TNR_CLUT216 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT216;

/* Define the union U_VPSS_TNR_CLUT217 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT217;

/* Define the union U_VPSS_TNR_CLUT218 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT218;

/* Define the union U_VPSS_TNR_CLUT219 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT219;

/* Define the union U_VPSS_TNR_CLUT220 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT220;

/* Define the union U_VPSS_TNR_CLUT221 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT221;

/* Define the union U_VPSS_TNR_CLUT222 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT222;

/* Define the union U_VPSS_TNR_CLUT223 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT223;

/* Define the union U_VPSS_TNR_CLUT224 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT224;

/* Define the union U_VPSS_TNR_CLUT225 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT225;

/* Define the union U_VPSS_TNR_CLUT226 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT226;

/* Define the union U_VPSS_TNR_CLUT227 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT227;

/* Define the union U_VPSS_TNR_CLUT228 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT228;

/* Define the union U_VPSS_TNR_CLUT229 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT229;

/* Define the union U_VPSS_TNR_CLUT230 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT230;

/* Define the union U_VPSS_TNR_CLUT231 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT231;

/* Define the union U_VPSS_TNR_CLUT232 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT232;

/* Define the union U_VPSS_TNR_CLUT233 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT233;

/* Define the union U_VPSS_TNR_CLUT234 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT234;

/* Define the union U_VPSS_TNR_CLUT235 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT235;

/* Define the union U_VPSS_TNR_CLUT236 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT236;

/* Define the union U_VPSS_TNR_CLUT237 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT237;

/* Define the union U_VPSS_TNR_CLUT238 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT238;

/* Define the union U_VPSS_TNR_CLUT239 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT239;

/* Define the union U_VPSS_TNR_CLUT240 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT240;

/* Define the union U_VPSS_TNR_CLUT241 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT241;

/* Define the union U_VPSS_TNR_CLUT242 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT242;

/* Define the union U_VPSS_TNR_CLUT243 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT243;

/* Define the union U_VPSS_TNR_CLUT244 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT244;

/* Define the union U_VPSS_TNR_CLUT245 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT245;

/* Define the union U_VPSS_TNR_CLUT246 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT246;

/* Define the union U_VPSS_TNR_CLUT247 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT247;

/* Define the union U_VPSS_TNR_CLUT248 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT248;

/* Define the union U_VPSS_TNR_CLUT249 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT249;

/* Define the union U_VPSS_TNR_CLUT250 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT250;

/* Define the union U_VPSS_TNR_CLUT251 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT251;

/* Define the union U_VPSS_TNR_CLUT252 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT252;

/* Define the union U_VPSS_TNR_CLUT253 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT253;

/* Define the union U_VPSS_TNR_CLUT254 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT254;

/* Define the union U_VPSS_TNR_CLUT255 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT255;

/* Define the union U_VPSS_TNR_CLUT256 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT256;

/* Define the union U_VPSS_TNR_CLUT257 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT257;

/* Define the union U_VPSS_TNR_CLUT258 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT258;

/* Define the union U_VPSS_TNR_CLUT259 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT259;

/* Define the union U_VPSS_TNR_CLUT260 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT260;

/* Define the union U_VPSS_TNR_CLUT261 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT261;

/* Define the union U_VPSS_TNR_CLUT262 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT262;

/* Define the union U_VPSS_TNR_CLUT263 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT263;

/* Define the union U_VPSS_TNR_CLUT264 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT264;

/* Define the union U_VPSS_TNR_CLUT265 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT265;

/* Define the union U_VPSS_TNR_CLUT266 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT266;

/* Define the union U_VPSS_TNR_CLUT267 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT267;

/* Define the union U_VPSS_TNR_CLUT268 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT268;

/* Define the union U_VPSS_TNR_CLUT269 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT269;

/* Define the union U_VPSS_TNR_CLUT270 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT270;

/* Define the union U_VPSS_TNR_CLUT271 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT271;

/* Define the union U_VPSS_TNR_CLUT272 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT272;

/* Define the union U_VPSS_TNR_CLUT273 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT273;

/* Define the union U_VPSS_TNR_CLUT274 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT274;

/* Define the union U_VPSS_TNR_CLUT275 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT275;

/* Define the union U_VPSS_TNR_CLUT276 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT276;

/* Define the union U_VPSS_TNR_CLUT277 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT277;

/* Define the union U_VPSS_TNR_CLUT278 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT278;

/* Define the union U_VPSS_TNR_CLUT279 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT279;

/* Define the union U_VPSS_TNR_CLUT280 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT280;

/* Define the union U_VPSS_TNR_CLUT281 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT281;

/* Define the union U_VPSS_TNR_CLUT282 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT282;

/* Define the union U_VPSS_TNR_CLUT283 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT283;

/* Define the union U_VPSS_TNR_CLUT284 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT284;

/* Define the union U_VPSS_TNR_CLUT285 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT285;

/* Define the union U_VPSS_TNR_CLUT286 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT286;

/* Define the union U_VPSS_TNR_CLUT287 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT287;

/* Define the union U_VPSS_TNR_CLUT288 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT288;

/* Define the union U_VPSS_TNR_CLUT289 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT289;

/* Define the union U_VPSS_TNR_CLUT290 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT290;

/* Define the union U_VPSS_TNR_CLUT291 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT291;

/* Define the union U_VPSS_TNR_CLUT292 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT292;

/* Define the union U_VPSS_TNR_CLUT293 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT293;

/* Define the union U_VPSS_TNR_CLUT294 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT294;

/* Define the union U_VPSS_TNR_CLUT295 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT295;

/* Define the union U_VPSS_TNR_CLUT296 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT296;

/* Define the union U_VPSS_TNR_CLUT297 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT297;

/* Define the union U_VPSS_TNR_CLUT298 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT298;

/* Define the union U_VPSS_TNR_CLUT299 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT299;

/* Define the union U_VPSS_TNR_CLUT2100 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2100;

/* Define the union U_VPSS_TNR_CLUT2101 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2101;

/* Define the union U_VPSS_TNR_CLUT2102 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2102;

/* Define the union U_VPSS_TNR_CLUT2103 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2103;

/* Define the union U_VPSS_TNR_CLUT2104 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2104;

/* Define the union U_VPSS_TNR_CLUT2105 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2105;

/* Define the union U_VPSS_TNR_CLUT2106 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2106;

/* Define the union U_VPSS_TNR_CLUT2107 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2107;

/* Define the union U_VPSS_TNR_CLUT2108 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2108;

/* Define the union U_VPSS_TNR_CLUT2109 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2109;

/* Define the union U_VPSS_TNR_CLUT2110 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2110;

/* Define the union U_VPSS_TNR_CLUT2111 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2111;

/* Define the union U_VPSS_TNR_CLUT2112 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2112;

/* Define the union U_VPSS_TNR_CLUT2113 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2113;

/* Define the union U_VPSS_TNR_CLUT2114 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2114;

/* Define the union U_VPSS_TNR_CLUT2115 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2115;

/* Define the union U_VPSS_TNR_CLUT2116 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2116;

/* Define the union U_VPSS_TNR_CLUT2117 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2117;

/* Define the union U_VPSS_TNR_CLUT2118 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2118;

/* Define the union U_VPSS_TNR_CLUT2119 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2119;

/* Define the union U_VPSS_TNR_CLUT2120 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2120;

/* Define the union U_VPSS_TNR_CLUT2121 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2121;

/* Define the union U_VPSS_TNR_CLUT2122 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2122;

/* Define the union U_VPSS_TNR_CLUT2123 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2123;

/* Define the union U_VPSS_TNR_CLUT2124 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2124;

/* Define the union U_VPSS_TNR_CLUT2125 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2125;

/* Define the union U_VPSS_TNR_CLUT2126 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2126;

/* Define the union U_VPSS_TNR_CLUT2127 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT2127;

/* Define the union U_VPSS_TNR_CLUT30 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT30;

/* Define the union U_VPSS_TNR_CLUT31 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT31;

/* Define the union U_VPSS_TNR_CLUT32 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT32;

/* Define the union U_VPSS_TNR_CLUT33 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT33;

/* Define the union U_VPSS_TNR_CLUT34 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT34;

/* Define the union U_VPSS_TNR_CLUT35 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT35;

/* Define the union U_VPSS_TNR_CLUT36 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT36;

/* Define the union U_VPSS_TNR_CLUT37 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT37;

/* Define the union U_VPSS_TNR_CLUT38 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT38;

/* Define the union U_VPSS_TNR_CLUT39 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT39;

/* Define the union U_VPSS_TNR_CLUT310 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT310;

/* Define the union U_VPSS_TNR_CLUT311 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT311;

/* Define the union U_VPSS_TNR_CLUT312 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT312;

/* Define the union U_VPSS_TNR_CLUT313 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT313;

/* Define the union U_VPSS_TNR_CLUT314 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT314;

/* Define the union U_VPSS_TNR_CLUT315 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT315;

/* Define the union U_VPSS_TNR_CLUT316 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT316;

/* Define the union U_VPSS_TNR_CLUT317 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT317;

/* Define the union U_VPSS_TNR_CLUT318 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT318;

/* Define the union U_VPSS_TNR_CLUT319 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT319;

/* Define the union U_VPSS_TNR_CLUT320 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT320;

/* Define the union U_VPSS_TNR_CLUT321 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT321;

/* Define the union U_VPSS_TNR_CLUT322 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT322;

/* Define the union U_VPSS_TNR_CLUT323 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT323;

/* Define the union U_VPSS_TNR_CLUT324 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT324;

/* Define the union U_VPSS_TNR_CLUT325 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT325;

/* Define the union U_VPSS_TNR_CLUT326 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT326;

/* Define the union U_VPSS_TNR_CLUT327 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT327;

/* Define the union U_VPSS_TNR_CLUT328 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT328;

/* Define the union U_VPSS_TNR_CLUT329 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT329;

/* Define the union U_VPSS_TNR_CLUT330 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT330;

/* Define the union U_VPSS_TNR_CLUT331 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT331;

/* Define the union U_VPSS_TNR_CLUT40 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT40;

/* Define the union U_VPSS_TNR_CLUT41 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT41;

/* Define the union U_VPSS_TNR_CLUT42 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT42;

/* Define the union U_VPSS_TNR_CLUT43 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT43;

/* Define the union U_VPSS_TNR_CLUT44 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT44;

/* Define the union U_VPSS_TNR_CLUT45 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT45;

/* Define the union U_VPSS_TNR_CLUT46 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT46;

/* Define the union U_VPSS_TNR_CLUT47 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT47;

/* Define the union U_VPSS_TNR_CLUT48 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT48;

/* Define the union U_VPSS_TNR_CLUT49 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT49;

/* Define the union U_VPSS_TNR_CLUT410 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT410;

/* Define the union U_VPSS_TNR_CLUT411 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT411;

/* Define the union U_VPSS_TNR_CLUT412 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT412;

/* Define the union U_VPSS_TNR_CLUT413 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT413;

/* Define the union U_VPSS_TNR_CLUT414 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT414;

/* Define the union U_VPSS_TNR_CLUT415 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT415;

/* Define the union U_VPSS_TNR_CLUT416 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT416;

/* Define the union U_VPSS_TNR_CLUT417 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT417;

/* Define the union U_VPSS_TNR_CLUT418 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT418;

/* Define the union U_VPSS_TNR_CLUT419 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT419;

/* Define the union U_VPSS_TNR_CLUT420 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT420;

/* Define the union U_VPSS_TNR_CLUT421 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT421;

/* Define the union U_VPSS_TNR_CLUT422 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT422;

/* Define the union U_VPSS_TNR_CLUT423 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT423;

/* Define the union U_VPSS_TNR_CLUT424 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT424;

/* Define the union U_VPSS_TNR_CLUT425 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT425;

/* Define the union U_VPSS_TNR_CLUT426 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT426;

/* Define the union U_VPSS_TNR_CLUT427 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT427;

/* Define the union U_VPSS_TNR_CLUT428 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT428;

/* Define the union U_VPSS_TNR_CLUT429 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT429;

/* Define the union U_VPSS_TNR_CLUT430 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT430;

/* Define the union U_VPSS_TNR_CLUT431 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT431;

/* Define the union U_VPSS_TNR_CLUT50 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT50;

/* Define the union U_VPSS_TNR_CLUT51 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT51;

/* Define the union U_VPSS_TNR_CLUT52 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT52;

/* Define the union U_VPSS_TNR_CLUT53 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT53;

/* Define the union U_VPSS_TNR_CLUT54 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT54;

/* Define the union U_VPSS_TNR_CLUT55 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT55;

/* Define the union U_VPSS_TNR_CLUT56 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT56;

/* Define the union U_VPSS_TNR_CLUT57 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT57;

/* Define the union U_VPSS_TNR_CLUT60 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT60;

/* Define the union U_VPSS_TNR_CLUT61 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT61;

/* Define the union U_VPSS_TNR_CLUT62 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT62;

/* Define the union U_VPSS_TNR_CLUT63 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT63;

/* Define the union U_VPSS_TNR_CLUT64 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT64;

/* Define the union U_VPSS_TNR_CLUT65 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT65;

/* Define the union U_VPSS_TNR_CLUT66 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT66;

/* Define the union U_VPSS_TNR_CLUT67 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0                 : 8   ; /* [7..0]  */
        unsigned int    data1                 : 8   ; /* [15..8]  */
        unsigned int    data2                 : 8   ; /* [23..16]  */
        unsigned int    data3                 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CLUT67;

/* Define the union U_VPSS_DEBUG0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug0                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG0;
/* Define the union U_VPSS_DEBUG1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug1                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG1;
/* Define the union U_VPSS_DEBUG2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug2                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG2;
/* Define the union U_VPSS_DEBUG3 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug3                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG3;
/* Define the union U_VPSS_DEBUG4 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug4                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG4;
/* Define the union U_VPSS_DEBUG5 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug5                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG5;
/* Define the union U_VPSS_DEBUG6 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug6                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG6;
/* Define the union U_VPSS_DEBUG7 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug7                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG7;
/* Define the union U_VPSS_DEBUG8 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug8                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG8;
/* Define the union U_VPSS_DEBUG9 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug9                 : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG9;
/* Define the union U_VPSS_DEBUG10 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug10                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG10;
/* Define the union U_VPSS_DEBUG11 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug11                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG11;
/* Define the union U_VPSS_DEBUG12 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug12                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG12;
/* Define the union U_VPSS_DEBUG13 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug13                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG13;
/* Define the union U_VPSS_DEBUG14 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug14                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG14;
/* Define the union U_VPSS_DEBUG15 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug15                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG15;
/* Define the union U_VPSS_DEBUG16 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug16                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG16;
/* Define the union U_VPSS_DEBUG17 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug17                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG17;
/* Define the union U_VPSS_DEBUG18 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug18                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG18;
/* Define the union U_VPSS_DEBUG19 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug19                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG19;
/* Define the union U_VPSS_DEBUG20 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug20                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG20;
/* Define the union U_VPSS_DEBUG21 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug21                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG21;
/* Define the union U_VPSS_DEBUG22 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug22                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG22;
/* Define the union U_VPSS_DEBUG23 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug23                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG23;
/* Define the union U_VPSS_DEBUG24 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug24                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG24;
/* Define the union U_VPSS_DEBUG25 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug25                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG25;
/* Define the union U_VPSS_DEBUG26 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug26                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG26;
/* Define the union U_VPSS_DEBUG27 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug27                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG27;
/* Define the union U_VPSS_DEBUG28 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug28                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG28;
/* Define the union U_VPSS_DEBUG29 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug29                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG29;
/* Define the union U_VPSS_DEBUG30 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug30                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG30;
/* Define the union U_VPSS_DEBUG31 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug31                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG31;
/* Define the union U_VPSS_DEBUG32 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug32                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG32;
/* Define the union U_VPSS_DEBUG33 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug33                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG33;
/* Define the union U_VPSS_DEBUG34 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug34                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG34;
/* Define the union U_VPSS_DEBUG35 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug35                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG35;
/* Define the union U_VPSS_DEBUG36 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug36                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG36;
/* Define the union U_VPSS_DEBUG37 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug37                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG37;
/* Define the union U_VPSS_DEBUG38 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug38                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG38;
/* Define the union U_VPSS_DEBUG39 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug39                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG39;
/* Define the union U_VPSS_DEBUG40 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug40                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG40;
/* Define the union U_VPSS_DEBUG41 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug41                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG41;
/* Define the union U_VPSS_DEBUG42 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug42                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG42;
/* Define the union U_VPSS_DEBUG43 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int debug43                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEBUG43;
/* Define the union U_VPSS_DEI_WST_CHKSUM */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int dei_st_w_chk_sum       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DEI_WST_CHKSUM;
/* Define the union U_VPSS_VHD0_Y_CHKSUM */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd0_y_chk_sum         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD0_Y_CHKSUM;
/* Define the union U_VPSS_VHD0_C_CHKSUM */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vhd0_c_chk_sum         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VHD0_C_CHKSUM;
/* Define the union U_VPSS_STR_Y_CHKSUM */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int str_y_chk_sum          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STR_Y_CHKSUM;
/* Define the union U_VPSS_STR_C_CHKSUM */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int str_c_chk_sum          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_STR_C_CHKSUM;
//==============================================================================
/* Define the global struct */
typedef struct
{
    U_VPSS_CTRL            VPSS_CTRL                         ; /* 0x0 */
    U_VPSS_CTRL2           VPSS_CTRL2                        ; /* 0x4 */
    U_VPSS_REFSIZE         VPSS_REFSIZE                      ; /* 0x8 */
    U_VPSS_IMGSIZE         VPSS_IMGSIZE                      ; /* 0xc */
    unsigned int           reserved_0[4]                     ; /* 0x10~0x1c */
    U_VPSS_RID_CFG0        VPSS_RID_CFG0                     ; /* 0x20 */
    U_VPSS_CUR_CTRL        VPSS_CUR_CTRL                     ; /* 0x24 */
    U_VPSS_CURYADDR        VPSS_CURYADDR                     ; /* 0x28 */
    U_VPSS_CURCADDR        VPSS_CURCADDR                     ; /* 0x2c */
    U_VPSS_CURCRADDR       VPSS_CURCRADDR                    ; /* 0x30 */
    U_VPSS_CURSTRIDE       VPSS_CURSTRIDE                    ; /* 0x34 */
    U_VPSS_REF_CTRL        VPSS_REF_CTRL                     ; /* 0x38 */
    U_VPSS_REFYADDR        VPSS_REFYADDR                     ; /* 0x3c */
    U_VPSS_REFCADDR        VPSS_REFCADDR                     ; /* 0x40 */
    U_VPSS_REFCRADDR       VPSS_REFCRADDR                    ; /* 0x44 */
    U_VPSS_REFSTRIDE       VPSS_REFSTRIDE                    ; /* 0x48 */
    U_VPSS_NXT1_CTRL       VPSS_NXT1_CTRL                    ; /* 0x4c */
    U_VPSS_NXT1YADDR       VPSS_NXT1YADDR                    ; /* 0x50 */
    U_VPSS_NXT1CADDR       VPSS_NXT1CADDR                    ; /* 0x54 */
    U_VPSS_NXT1CRADDR      VPSS_NXT1CRADDR                   ; /* 0x58 */
    U_VPSS_NXT1STRIDE      VPSS_NXT1STRIDE                   ; /* 0x5c */
    U_VPSS_NXT2_CTRL       VPSS_NXT2_CTRL                    ; /* 0x60 */
    U_VPSS_NXT2YADDR       VPSS_NXT2YADDR                    ; /* 0x64 */
    U_VPSS_NXT2CADDR       VPSS_NXT2CADDR                    ; /* 0x68 */
    U_VPSS_NXT2CRADDR      VPSS_NXT2CRADDR                   ; /* 0x6c */
    U_VPSS_NXT2STRIDE      VPSS_NXT2STRIDE                   ; /* 0x70 */
    U_VPSS_STRADDR         VPSS_STRADDR                      ; /* 0x74 */
    U_VPSS_STWADDR         VPSS_STWADDR                      ; /* 0x78 */
    U_VPSS_STSTRIDE        VPSS_STSTRIDE                     ; /* 0x7c */
    U_VPSS_MAD_RADDR       VPSS_MAD_RADDR                    ; /* 0x80 */
    U_VPSS_MAD_WADDR       VPSS_MAD_WADDR                    ; /* 0x84 */
    U_VPSS_MADSTRIDE       VPSS_MADSTRIDE                    ; /* 0x88 */
    U_VPSS_RFRYADDR        VPSS_RFRYADDR                     ; /* 0x8c */
    U_VPSS_RFRCADDR        VPSS_RFRCADDR                     ; /* 0x90 */
    U_VPSS_RFRSTRIDE       VPSS_RFRSTRIDE                    ; /* 0x94 */
    U_VPSS_VHD0CTRL        VPSS_VHD0CTRL                     ; /* 0x98 */
    U_VPSS_VHD0SIZE        VPSS_VHD0SIZE                     ; /* 0x9c */
    U_VPSS_VHD0YADDR       VPSS_VHD0YADDR                    ; /* 0xa0 */
    U_VPSS_VHD0CADDR       VPSS_VHD0CADDR                    ; /* 0xa4 */
    U_VPSS_VHD0STRIDE      VPSS_VHD0STRIDE                   ; /* 0xa8 */
    U_VPSS_TUNL_CTRL0      VPSS_TUNL_CTRL0                   ; /* 0xac */
    U_VPSS_TUNL_CTRL1      VPSS_TUNL_CTRL1                   ; /* 0xb0 */
    U_VPSS_VHD0_TUNL_ADDR   VPSS_VHD0_TUNL_ADDR              ; /* 0xb4 */
    U_VPSS_CUR_TUNL_ADDR   VPSS_CUR_TUNL_ADDR                ; /* 0xb8 */
    U_VPSS_CHK_SUM_W_ADDR   VPSS_CHK_SUM_W_ADDR              ; /* 0xbc */
    U_VPSS_CHK_SUM_SEL     VPSS_CHK_SUM_SEL                  ; /* 0xc0 */
    U_VPSS_STT_W_ADDR      VPSS_STT_W_ADDR                   ; /* 0xc4 */
    unsigned int           reserved_1[19]                     ; /* 0xc8~0x110 */
    U_VPSS_INCROP_POS      VPSS_INCROP_POS                   ; /* 0x114 */
    U_VPSS_INCROP_SIZE     VPSS_INCROP_SIZE                  ; /* 0x118 */
    unsigned int           reserved_2[2]                     ; /* 0x11c~0x120 */
    U_VPSS_VHD0CROP_POS    VPSS_VHD0CROP_POS                 ; /* 0x124 */
    U_VPSS_VHD0CROP_SIZE   VPSS_VHD0CROP_SIZE                ; /* 0x128 */
    unsigned int           reserved_3[13]                     ; /* 0x12c~0x15c */
    U_VPSS_VHD0_HSP        VPSS_VHD0_HSP                     ; /* 0x160 */
    U_VPSS_VHD0_HLOFFSET   VPSS_VHD0_HLOFFSET                ; /* 0x164 */
    U_VPSS_VHD0_HCOFFSET   VPSS_VHD0_HCOFFSET                ; /* 0x168 */
    U_VPSS_VHD0_VSP        VPSS_VHD0_VSP                     ; /* 0x16c */
    U_VPSS_VHD0_VSR        VPSS_VHD0_VSR                     ; /* 0x170 */
    U_VPSS_VHD0_VOFFSET    VPSS_VHD0_VOFFSET                 ; /* 0x174 */
    U_VPSS_VHD0_ZMEORESO   VPSS_VHD0_ZMEORESO                ; /* 0x178 */
    unsigned int           reserved_4[15]                     ; /* 0x17c~0x1b4 */
    U_VPSS_DR_CFG0         VPSS_DR_CFG0                      ; /* 0x1b8 */
    U_VPSS_DR_CFG1         VPSS_DR_CFG1                      ; /* 0x1bc */
    unsigned int           reserved_5[3]                     ; /* 0x1c0~0x1c8 */
    U_VPSS_DNR_INFO        VPSS_DNR_INFO                     ; /* 0x1cc */
    unsigned int           reserved_6[9]                     ; /* 0x1d0~0x1f0 */
    U_VPSS_NODEID          VPSS_NODEID                       ; /* 0x1f4 */
    U_VPSS_AXIID           VPSS_AXIID                        ; /* 0x1f8 */
    U_VPSS_INTMASK         VPSS_INTMASK                      ; /* 0x1fc */
    unsigned int           reserved_7[4]                     ; /* 0x200~0x20c */
    U_VPSS_VHD0_ZME_LHADDR   VPSS_VHD0_ZME_LHADDR            ; /* 0x210 */
    U_VPSS_VHD0_ZME_LVADDR   VPSS_VHD0_ZME_LVADDR            ; /* 0x214 */
    U_VPSS_VHD0_ZME_CHADDR   VPSS_VHD0_ZME_CHADDR            ; /* 0x218 */
    U_VPSS_VHD0_ZME_CVADDR   VPSS_VHD0_ZME_CVADDR            ; /* 0x21c */
    U_VPSS_TNR_ADDR        VPSS_TNR_ADDR                     ; /* 0x220 */
    U_VPSS_TNR_CLUT_ADDR   VPSS_TNR_CLUT_ADDR                ; /* 0x224 */
    unsigned int           reserved_8[6]                     ; /* 0x228~0x23c */
    U_VPSS_DEI_ADDR        VPSS_DEI_ADDR                     ; /* 0x240 */
    U_VPSS_VHD0_LCMP_ADDR   VPSS_VHD0_LCMP_ADDR              ; /* 0x244 */
    unsigned int           reserved_9[2]                     ; /* 0x248~0x24c */
    U_VPSS_VC1_CTRL0       VPSS_VC1_CTRL0                    ; /* 0x250 */
    U_VPSS_VC1_CTRL1       VPSS_VC1_CTRL1                    ; /* 0x254 */
    U_VPSS_VC1_CTRL2       VPSS_VC1_CTRL2                    ; /* 0x258 */
    U_VPSS_VC1_CTRL3       VPSS_VC1_CTRL3                    ; /* 0x25c */
    unsigned int           reserved_10[8]                     ; /* 0x260~0x27c */
    U_VPSS_VHD0LBA_DSIZE   VPSS_VHD0LBA_DSIZE                ; /* 0x280 */
    U_VPSS_VHD0LBA_VFPOS   VPSS_VHD0LBA_VFPOS                ; /* 0x284 */
    U_VPSS_VHD0LBA_BK      VPSS_VHD0LBA_BK                   ; /* 0x288 */
    unsigned int           reserved_11[13]                     ; /* 0x28c~0x2bc */
    U_STR_DET_VIDCTRL      STR_DET_VIDCTRL                   ; /* 0x2c0 */
    U_STR_DET_VIDBLKPOS0_1   STR_DET_VIDBLKPOS0_1            ; /* 0x2c4 */
    U_STR_DET_VIDBLKPOS2_3   STR_DET_VIDBLKPOS2_3            ; /* 0x2c8 */
    U_STR_DET_VIDBLKPOS4_5   STR_DET_VIDBLKPOS4_5            ; /* 0x2cc */
    unsigned int           reserved_12[11]                     ; /* 0x2d0~0x2f8 */
    U_VPSS_PNEXT           VPSS_PNEXT                        ; /* 0x2fc */
    U_VPSS_START           VPSS_START                        ; /* 0x300 */
    U_VPSS_INTSTATE        VPSS_INTSTATE                     ; /* 0x304 */
    U_VPSS_INTCLR          VPSS_INTCLR                       ; /* 0x308 */
    U_VPSS_RAWINT          VPSS_RAWINT                       ; /* 0x30c */
    U_VPSS_PFCNT           VPSS_PFCNT                        ; /* 0x310 */
    U_VPSS_MISCELLANEOUS   VPSS_MISCELLANEOUS                ; /* 0x314 */
    U_VPSS_MACCFG          VPSS_MACCFG                       ; /* 0x318 */
    U_VPSS_TIMEOUT         VPSS_TIMEOUT                      ; /* 0x31c */
    U_VPSS_EOFCNT          VPSS_EOFCNT                       ; /* 0x320 */
    unsigned int           reserved_13[3]                     ; /* 0x324~0x32c */
    U_VPSS_VERSION         VPSS_VERSION                      ; /* 0x330 */
    unsigned int           reserved_14[51]                     ; /* 0x334~0x3fc */
    U_VPSS_VHD0_LCMP_YINFO   VPSS_VHD0_LCMP_YINFO            ; /* 0x400 */
    U_VPSS_VHD0_LCMP_YSIZE   VPSS_VHD0_LCMP_YSIZE            ; /* 0x404 */
    U_VPSS_VHD0_LCMP_YRCCFG0   VPSS_VHD0_LCMP_YRCCFG0        ; /* 0x408 */
    U_VPSS_VHD0_LCMP_YRCCFG1   VPSS_VHD0_LCMP_YRCCFG1        ; /* 0x40c */
    U_VPSS_VHD0_LCMP_YRCCFG2   VPSS_VHD0_LCMP_YRCCFG2        ; /* 0x410 */
    U_VPSS_VHD0_LCMP_YRCCFG3   VPSS_VHD0_LCMP_YRCCFG3        ; /* 0x414 */
    U_VPSS_VHD0_LCMP_YRCCFG4   VPSS_VHD0_LCMP_YRCCFG4        ; /* 0x418 */
    U_VPSS_VHD0_LCMP_YRCCFG5   VPSS_VHD0_LCMP_YRCCFG5        ; /* 0x41c */
    U_VPSS_VHD0_LCMP_YMAXLEN   VPSS_VHD0_LCMP_YMAXLEN        ; /* 0x420 */
    unsigned int           reserved_15[7]                     ; /* 0x424~0x43c */
    U_VPSS_VHD0_LCMP_CINFO   VPSS_VHD0_LCMP_CINFO            ; /* 0x440 */
    U_VPSS_VHD0_LCMP_CSIZE   VPSS_VHD0_LCMP_CSIZE            ; /* 0x444 */
    U_VPSS_VHD0_LCMP_CRCCFG0   VPSS_VHD0_LCMP_CRCCFG0        ; /* 0x448 */
    U_VPSS_VHD0_LCMP_CRCCFG1   VPSS_VHD0_LCMP_CRCCFG1        ; /* 0x44c */
    U_VPSS_VHD0_LCMP_CRCCFG2   VPSS_VHD0_LCMP_CRCCFG2        ; /* 0x450 */
    U_VPSS_VHD0_LCMP_CRCCFG3   VPSS_VHD0_LCMP_CRCCFG3        ; /* 0x454 */
    U_VPSS_VHD0_LCMP_CRCCFG4   VPSS_VHD0_LCMP_CRCCFG4        ; /* 0x458 */
    U_VPSS_VHD0_LCMP_CRCCFG5   VPSS_VHD0_LCMP_CRCCFG5        ; /* 0x45c */
    U_VPSS_VHD0_LCMP_CMAXLEN   VPSS_VHD0_LCMP_CMAXLEN        ; /* 0x460 */
    unsigned int           reserved_16[743]                     ; /* 0x464~0xffc */
    U_VPSS_DIECTRL         VPSS_DIECTRL                      ; /* 0x1000 */
    U_VPSS_DIELMA2         VPSS_DIELMA2                      ; /* 0x1004 */
    U_VPSS_DIEINTEN        VPSS_DIEINTEN                     ; /* 0x1008 */
    U_VPSS_DIESCALE        VPSS_DIESCALE                     ; /* 0x100c */
    U_VPSS_DIECHECK1       VPSS_DIECHECK1                    ; /* 0x1010 */
    U_VPSS_DIECHECK2       VPSS_DIECHECK2                    ; /* 0x1014 */
    U_VPSS_DIEDIR0_3       VPSS_DIEDIR0_3                    ; /* 0x1018 */
    U_VPSS_DIEDIR4_7       VPSS_DIEDIR4_7                    ; /* 0x101c */
    U_VPSS_DIEDIR8_11      VPSS_DIEDIR8_11                   ; /* 0x1020 */
    U_VPSS_DIEDIR12_14     VPSS_DIEDIR12_14                  ; /* 0x1024 */
    U_VPSS_DIESTA          VPSS_DIESTA                       ; /* 0x1028 */
    U_VPSS_DIESTPPREADDR   VPSS_DIESTPPREADDR                ; /* 0x102c */
    U_VPSS_DIESTPREADDR    VPSS_DIESTPREADDR                 ; /* 0x1030 */
    U_VPSS_DIEINTPSCL0     VPSS_DIEINTPSCL0                  ; /* 0x1034 */
    U_VPSS_DIEINTPSCL1     VPSS_DIEINTPSCL1                  ; /* 0x1038 */
    U_VPSS_DIEDIRTHD       VPSS_DIEDIRTHD                    ; /* 0x103c */
    U_VPSS_DIEJITMTN       VPSS_DIEJITMTN                    ; /* 0x1040 */
    U_VPSS_DIEFLDMTN       VPSS_DIEFLDMTN                    ; /* 0x1044 */
    U_VPSS_DIEMTNDIFFTHD0   VPSS_DIEMTNDIFFTHD0              ; /* 0x1048 */
    U_VPSS_DIEMTNDIFFTHD1   VPSS_DIEMTNDIFFTHD1              ; /* 0x104c */
    U_VPSS_DIEMTNIIRSLP    VPSS_DIEMTNIIRSLP                 ; /* 0x1050 */
    U_VPSS_DIEMTNIIRRAT    VPSS_DIEMTNIIRRAT                 ; /* 0x1054 */
    U_VPSS_DIEHISMODE      VPSS_DIEHISMODE                   ; /* 0x1058 */
    U_VPSS_DIEMORFLT       VPSS_DIEMORFLT                    ; /* 0x105c */
    U_VPSS_DIECOMBCHK0     VPSS_DIECOMBCHK0                  ; /* 0x1060 */
    U_VPSS_DIECOMBCHK1     VPSS_DIECOMBCHK1                  ; /* 0x1064 */
    U_VPSS_DIEFRMMTNSMTHTHD0   VPSS_DIEFRMMTNSMTHTHD0        ; /* 0x1068 */
    U_VPSS_DIEFRMMTNSMTHTHD1   VPSS_DIEFRMMTNSMTHTHD1        ; /* 0x106c */
    U_VPSS_DIEFRMMTNSMTHSLP   VPSS_DIEFRMMTNSMTHSLP          ; /* 0x1070 */
    U_VPSS_DIEFRMMTNSMTHRAT   VPSS_DIEFRMMTNSMTHRAT          ; /* 0x1074 */
    U_VPSS_DIEFRMFLDBLENDTHD0   VPSS_DIEFRMFLDBLENDTHD0      ; /* 0x1078 */
    U_VPSS_DIEFRMFLDBLENDTHD1   VPSS_DIEFRMFLDBLENDTHD1      ; /* 0x107c */
    U_VPSS_DIEFRMFLDBLENDSLP   VPSS_DIEFRMFLDBLENDSLP        ; /* 0x1080 */
    U_VPSS_DIEFRMFLDBLENDRAT   VPSS_DIEFRMFLDBLENDRAT        ; /* 0x1084 */
    U_VPSS_DIEMTNADJ       VPSS_DIEMTNADJ                    ; /* 0x1088 */
    U_VPSS_DIEGLBMTNTHD0   VPSS_DIEGLBMTNTHD0                ; /* 0x108c */
    U_VPSS_DIEGLBMTNTHD1   VPSS_DIEGLBMTNTHD1                ; /* 0x1090 */
    U_VPSS_CCRSCLR0        VPSS_CCRSCLR0                     ; /* 0x1094 */
    U_VPSS_CCRSCLR1        VPSS_CCRSCLR1                     ; /* 0x1098 */
    unsigned int           reserved_17[42]                     ; /* 0x109c~0x1140 */
    U_VPSS_PDPHISTTHD1     VPSS_PDPHISTTHD1                  ; /* 0x1144 */
    U_VPSS_PDPCCMOV        VPSS_PDPCCMOV                     ; /* 0x1148 */
    U_VPSS_PDICHD          VPSS_PDICHD                       ; /* 0x114c */
    U_VPSS_PDCTRL          VPSS_PDCTRL                       ; /* 0x1150 */
    U_VPSS_PDBLKTHD        VPSS_PDBLKTHD                     ; /* 0x1154 */
    U_VPSS_PDHISTTHD       VPSS_PDHISTTHD                    ; /* 0x1158 */
    U_VPSS_PDUMTHD         VPSS_PDUMTHD                      ; /* 0x115c */
    U_VPSS_PDPCCCORING     VPSS_PDPCCCORING                  ; /* 0x1160 */
    U_VPSS_PDPCCHTHD       VPSS_PDPCCHTHD                    ; /* 0x1164 */
    U_VPSS_PDPCCVTHD       VPSS_PDPCCVTHD                    ; /* 0x1168 */
    U_VPSS_PDITDIFFVTHD    VPSS_PDITDIFFVTHD                 ; /* 0x116c */
    U_VPSS_PDLASITHD       VPSS_PDLASITHD                    ; /* 0x1170 */
    U_VPSS_PDLASIMODE0COEF   VPSS_PDLASIMODE0COEF            ; /* 0x1174 */
    U_VPSS_PDLASIMODE1COEF   VPSS_PDLASIMODE1COEF            ; /* 0x1178 */
    U_VPSS_PDREGION        VPSS_PDREGION                     ; /* 0x117c */
    unsigned int           reserved_18[2464]                     ; /* 0x1180~0x37fc */
    U_VPSS_TNR_MODE        VPSS_TNR_MODE                     ; /* 0x3800 */
    U_VPSS_TNR_YMOTION_MAP_CTRL   VPSS_TNR_YMOTION_MAP_CTRL  ; /* 0x3804 */
    U_VPSS_TNR_YMOTION_MAP_R0   VPSS_TNR_YMOTION_MAP_R0      ; /* 0x3808 */
    U_VPSS_TNR_YMOTION_MAP_R1   VPSS_TNR_YMOTION_MAP_R1      ; /* 0x380c */
    U_VPSS_TNR_YMOTION_MAP_T1   VPSS_TNR_YMOTION_MAP_T1      ; /* 0x3810 */
    U_VPSS_TNR_YMOTION_MAP_T2   VPSS_TNR_YMOTION_MAP_T2      ; /* 0x3814 */
    U_VPSS_TNR_YMOTION_MAP_K1   VPSS_TNR_YMOTION_MAP_K1      ; /* 0x3818 */
    U_VPSS_TNR_YMOTION_MAP_K2   VPSS_TNR_YMOTION_MAP_K2      ; /* 0x381c */
    U_VPSS_TNR_CMOTION_MAP_CTRL   VPSS_TNR_CMOTION_MAP_CTRL  ; /* 0x3820 */
    U_VPSS_TNR_CMOTION_MAP_R0   VPSS_TNR_CMOTION_MAP_R0      ; /* 0x3824 */
    U_VPSS_TNR_CMOTION_MAP_R1   VPSS_TNR_CMOTION_MAP_R1      ; /* 0x3828 */
    U_VPSS_TNR_CMOTION_MAP_T1   VPSS_TNR_CMOTION_MAP_T1      ; /* 0x382c */
    U_VPSS_TNR_CMOTION_MAP_T2   VPSS_TNR_CMOTION_MAP_T2      ; /* 0x3830 */
    U_VPSS_TNR_CMOTION_MAP_K1   VPSS_TNR_CMOTION_MAP_K1      ; /* 0x3834 */
    U_VPSS_TNR_CMOTION_MAP_K2   VPSS_TNR_CMOTION_MAP_K2      ; /* 0x3838 */
    U_VPSS_TNR_YMOTION_MEANRATIO_CTRL   VPSS_TNR_YMOTION_MEANRATIO_CTRL ; /* 0x383c */
    U_VPSS_TNR_YMOTION_MEANRATIO_R0   VPSS_TNR_YMOTION_MEANRATIO_R0 ; /* 0x3840 */
    U_VPSS_TNR_YMOTION_MEANRATIO_R1   VPSS_TNR_YMOTION_MEANRATIO_R1 ; /* 0x3844 */
    U_VPSS_TNR_YMOTION_MEANRATIO_T1   VPSS_TNR_YMOTION_MEANRATIO_T1 ; /* 0x3848 */
    U_VPSS_TNR_YMOTION_MEANRATIO_T2   VPSS_TNR_YMOTION_MEANRATIO_T2 ; /* 0x384c */
    U_VPSS_TNR_YMOTION_MEANRATIO_K1   VPSS_TNR_YMOTION_MEANRATIO_K1 ; /* 0x3850 */
    U_VPSS_TNR_YMOTION_MEANRATIO_K2   VPSS_TNR_YMOTION_MEANRATIO_K2 ; /* 0x3854 */
    U_VPSS_TNR_CMOTION_MEANRATIO_CTRL   VPSS_TNR_CMOTION_MEANRATIO_CTRL ; /* 0x3858 */
    U_VPSS_TNR_CMOTION_MEANRATIO_R0   VPSS_TNR_CMOTION_MEANRATIO_R0 ; /* 0x385c */
    U_VPSS_TNR_CMOTION_MEANRATIO_R1   VPSS_TNR_CMOTION_MEANRATIO_R1 ; /* 0x3860 */
    U_VPSS_TNR_CMOTION_MEANRATIO_T1   VPSS_TNR_CMOTION_MEANRATIO_T1 ; /* 0x3864 */
    U_VPSS_TNR_CMOTION_MEANRATIO_T2   VPSS_TNR_CMOTION_MEANRATIO_T2 ; /* 0x3868 */
    U_VPSS_TNR_CMOTION_MEANRATIO_K1   VPSS_TNR_CMOTION_MEANRATIO_K1 ; /* 0x386c */
    U_VPSS_TNR_CMOTION_MEANRATIO_K2   VPSS_TNR_CMOTION_MEANRATIO_K2 ; /* 0x3870 */
    U_VPSS_TNR_TM_YCMTCOEF   VPSS_TNR_TM_YCMTCOEF            ; /* 0x3874 */
    U_VPSS_TNR_TM_YCLPFCOEF   VPSS_TNR_TM_YCLPFCOEF          ; /* 0x3878 */
    U_VPSS_TNR_TE_YHPF1_0   VPSS_TNR_TE_YHPF1_0              ; /* 0x387c */
    U_VPSS_TNR_TE_YHPF1_1   VPSS_TNR_TE_YHPF1_1              ; /* 0x3880 */
    U_VPSS_TNR_TE_YHPF2_0   VPSS_TNR_TE_YHPF2_0              ; /* 0x3884 */
    U_VPSS_TNR_TE_YHPF2_1   VPSS_TNR_TE_YHPF2_1              ; /* 0x3888 */
    U_VPSS_TNR_TE_YHPF3_0   VPSS_TNR_TE_YHPF3_0              ; /* 0x388c */
    U_VPSS_TNR_TE_YHPF3_1   VPSS_TNR_TE_YHPF3_1              ; /* 0x3890 */
    U_VPSS_TNR_TE_YHPF4_0   VPSS_TNR_TE_YHPF4_0              ; /* 0x3894 */
    U_VPSS_TNR_TE_YHPF4_1   VPSS_TNR_TE_YHPF4_1              ; /* 0x3898 */
    U_VPSS_TNR_TE_YMTMEAN_LPFCOEF_0   VPSS_TNR_TE_YMTMEAN_LPFCOEF_0 ; /* 0x389c */
    U_VPSS_TNR_TE_YMTMEAN_LPFCOEF_1   VPSS_TNR_TE_YMTMEAN_LPFCOEF_1 ; /* 0x38a0 */
    U_VPSS_TNR_TE_YCEG_MEANRATIO_CTRL   VPSS_TNR_TE_YCEG_MEANRATIO_CTRL ; /* 0x38a4 */
    U_VPSS_TNR_TE_YCEG_MEANRATIO_R0   VPSS_TNR_TE_YCEG_MEANRATIO_R0 ; /* 0x38a8 */
    U_VPSS_TNR_TE_YCEG_MEANRATIO_R1   VPSS_TNR_TE_YCEG_MEANRATIO_R1 ; /* 0x38ac */
    U_VPSS_TNR_TE_YCEG_MEANRATIO_T1   VPSS_TNR_TE_YCEG_MEANRATIO_T1 ; /* 0x38b0 */
    U_VPSS_TNR_TE_YCEG_MEANRATIO_T2   VPSS_TNR_TE_YCEG_MEANRATIO_T2 ; /* 0x38b4 */
    U_VPSS_TNR_TE_YCEG_MEANRATIO_K1   VPSS_TNR_TE_YCEG_MEANRATIO_K1 ; /* 0x38b8 */
    U_VPSS_TNR_TE_YCEG_MEANRATIO_K2   VPSS_TNR_TE_YCEG_MEANRATIO_K2 ; /* 0x38bc */
    U_VPSS_TNR_TE_CMT_MEANRATIO_CTRL   VPSS_TNR_TE_CMT_MEANRATIO_CTRL ; /* 0x38c0 */
    U_VPSS_TNR_TE_CMT_MEANRATIO_R0   VPSS_TNR_TE_CMT_MEANRATIO_R0 ; /* 0x38c4 */
    U_VPSS_TNR_TE_CMT_MEANRATIO_R1   VPSS_TNR_TE_CMT_MEANRATIO_R1 ; /* 0x38c8 */
    U_VPSS_TNR_TE_CMT_MEANRATIO_T1   VPSS_TNR_TE_CMT_MEANRATIO_T1 ; /* 0x38cc */
    U_VPSS_TNR_TE_CMT_MEANRATIO_T2   VPSS_TNR_TE_CMT_MEANRATIO_T2 ; /* 0x38d0 */
    U_VPSS_TNR_TE_CMT_MEANRATIO_K1   VPSS_TNR_TE_CMT_MEANRATIO_K1 ; /* 0x38d4 */
    U_VPSS_TNR_TE_CMT_MEANRATIO_K2   VPSS_TNR_TE_CMT_MEANRATIO_K2 ; /* 0x38d8 */
    U_VPSS_TNR_TE_YCMT_MAPPING_CTRL   VPSS_TNR_TE_YCMT_MAPPING_CTRL ; /* 0x38dc */
    U_VPSS_TNR_TE_YCMT_MAPPING_R0   VPSS_TNR_TE_YCMT_MAPPING_R0 ; /* 0x38e0 */
    U_VPSS_TNR_TE_YCMT_MAPPING_R1   VPSS_TNR_TE_YCMT_MAPPING_R1 ; /* 0x38e4 */
    U_VPSS_TNR_TE_YCMT_MAPPING_T1   VPSS_TNR_TE_YCMT_MAPPING_T1 ; /* 0x38e8 */
    U_VPSS_TNR_TE_YCMT_MAPPING_T2   VPSS_TNR_TE_YCMT_MAPPING_T2 ; /* 0x38ec */
    U_VPSS_TNR_TE_YCMT_MAPPING_K1   VPSS_TNR_TE_YCMT_MAPPING_K1 ; /* 0x38f0 */
    U_VPSS_TNR_TE_YCMT_MAPPING_K2   VPSS_TNR_TE_YCMT_MAPPING_K2 ; /* 0x38f4 */
    unsigned int           reserved_19[7]                     ; /* 0x38f8~0x3910 */
    U_VPSS_TNR_TE_YCEG_MAPPING_CTRL   VPSS_TNR_TE_YCEG_MAPPING_CTRL ; /* 0x3914 */
    U_VPSS_TNR_TE_YCEG_MAPPING_R0   VPSS_TNR_TE_YCEG_MAPPING_R0 ; /* 0x3918 */
    U_VPSS_TNR_TE_YCEG_MAPPING_R1   VPSS_TNR_TE_YCEG_MAPPING_R1 ; /* 0x391c */
    U_VPSS_TNR_TE_YCEG_MAPPING_T1   VPSS_TNR_TE_YCEG_MAPPING_T1 ; /* 0x3920 */
    U_VPSS_TNR_TE_YCEG_MAPPING_T2   VPSS_TNR_TE_YCEG_MAPPING_T2 ; /* 0x3924 */
    U_VPSS_TNR_TE_YCEG_MAPPING_K1   VPSS_TNR_TE_YCEG_MAPPING_K1 ; /* 0x3928 */
    U_VPSS_TNR_TE_YCEG_MAPPING_K2   VPSS_TNR_TE_YCEG_MAPPING_K2 ; /* 0x392c */
    U_VPSS_TNR_TE_CEG_CK   VPSS_TNR_TE_CEG_CK                ; /* 0x3930 */
    U_VPSS_TNR_TE_CEG_CK2   VPSS_TNR_TE_CEG_CK2              ; /* 0x3934 */
    U_VPSS_TNR_TE_CMT_CK1   VPSS_TNR_TE_CMT_CK1              ; /* 0x3938 */
    U_VPSS_TNR_TE_CMT_CK2   VPSS_TNR_TE_CMT_CK2              ; /* 0x393c */
    U_VPSS_TNR_TE_CMT_CK3   VPSS_TNR_TE_CMT_CK3              ; /* 0x3940 */
    U_VPSS_TNR_TE_CHPF4_COEF0   VPSS_TNR_TE_CHPF4_COEF0      ; /* 0x3944 */
    U_VPSS_TNR_TE_CHPF4_COEF1   VPSS_TNR_TE_CHPF4_COEF1      ; /* 0x3948 */
    U_VPSS_TNR_TE_YEG_WEIGHT   VPSS_TNR_TE_YEG_WEIGHT        ; /* 0x394c */
    U_VPSS_TNR_TE_CEG_WEIGHT   VPSS_TNR_TE_CEG_WEIGHT        ; /* 0x3950 */
    U_VPSS_TNR_TE_CMEAN_LPFCOEF1   VPSS_TNR_TE_CMEAN_LPFCOEF1 ; /* 0x3954 */
    U_VPSS_TNR_TE_CMEAN_LPFCOEF2   VPSS_TNR_TE_CMEAN_LPFCOEF2 ; /* 0x3958 */
    U_VPSS_TNR_TE_YMT_CK1   VPSS_TNR_TE_YMT_CK1              ; /* 0x395c */
    U_VPSS_TNR_TE_YMT_CK2   VPSS_TNR_TE_YMT_CK2              ; /* 0x3960 */
    U_VPSS_TNR_TE_YEG_CK1   VPSS_TNR_TE_YEG_CK1              ; /* 0x3964 */
    U_VPSS_TNR_TE_YMT_CK   VPSS_TNR_TE_YMT_CK                ; /* 0x3968 */
    U_VPSS_TNR_TE_YEG_CK   VPSS_TNR_TE_YEG_CK                ; /* 0x396c */
    U_VPSS_TNR_TE_YMOVING_CK   VPSS_TNR_TE_YMOVING_CK        ; /* 0x3970 */
    U_VPSS_TNR_TFM_CMOTION_CK   VPSS_TNR_TFM_CMOTION_CK      ; /* 0x3974 */
    U_VPSS_TNR_TFM_CEDGE_CK   VPSS_TNR_TFM_CEDGE_CK          ; /* 0x3978 */
    U_VPSS_TNR_TFM_Y_MAPPING_CTRL   VPSS_TNR_TFM_Y_MAPPING_CTRL ; /* 0x397c */
    U_VPSS_TNR_TFM_Y_MAPPING_R0   VPSS_TNR_TFM_Y_MAPPING_R0  ; /* 0x3980 */
    U_VPSS_TNR_TFM_Y_MAPPING_R1   VPSS_TNR_TFM_Y_MAPPING_R1  ; /* 0x3984 */
    U_VPSS_TNR_TFM_Y_MAPPING_T1   VPSS_TNR_TFM_Y_MAPPING_T1  ; /* 0x3988 */
    U_VPSS_TNR_TFM_Y_MAPPING_T2   VPSS_TNR_TFM_Y_MAPPING_T2  ; /* 0x398c */
    U_VPSS_TNR_TFM_Y_MAPPING_K1   VPSS_TNR_TFM_Y_MAPPING_K1  ; /* 0x3990 */
    U_VPSS_TNR_TFM_Y_MAPPING_K2   VPSS_TNR_TFM_Y_MAPPING_K2  ; /* 0x3994 */
    U_VPSS_TNR_TFM_C_MAPPING_CTRL   VPSS_TNR_TFM_C_MAPPING_CTRL ; /* 0x3998 */
    U_VPSS_TNR_TFM_C_MAPPING_R0   VPSS_TNR_TFM_C_MAPPING_R0  ; /* 0x399c */
    U_VPSS_TNR_TFM_C_MAPPING_R1   VPSS_TNR_TFM_C_MAPPING_R1  ; /* 0x39a0 */
    U_VPSS_TNR_TFM_C_MAPPING_T1   VPSS_TNR_TFM_C_MAPPING_T1  ; /* 0x39a4 */
    U_VPSS_TNR_TFM_C_MAPPING_T2   VPSS_TNR_TFM_C_MAPPING_T2  ; /* 0x39a8 */
    U_VPSS_TNR_TFM_C_MAPPING_K1   VPSS_TNR_TFM_C_MAPPING_K1  ; /* 0x39ac */
    U_VPSS_TNR_TFM_C_MAPPING_K2   VPSS_TNR_TFM_C_MAPPING_K2  ; /* 0x39b0 */
    U_VPSS_TNR_TFM_HISTMOTION_BLEND   VPSS_TNR_TFM_HISTMOTION_BLEND ; /* 0x39b4 */
    U_VPSS_TNR_TFM_PARA    VPSS_TNR_TFM_PARA                 ; /* 0x39b8 */
    U_VPSS_TNR_TFM_YMOTION_CK   VPSS_TNR_TFM_YMOTION_CK      ; /* 0x39bc */
    U_VPSS_TNR_TFM_YEG_CK   VPSS_TNR_TFM_YEG_CK              ; /* 0x39c0 */
    U_VPSS_TNR_TM_YMTMEAN_CK   VPSS_TNR_TM_YMTMEAN_CK        ; /* 0x39c4 */
    U_VPSS_TNR_TYMYCK      VPSS_TNR_TYMYCK                   ; /* 0x39c8 */
    U_VPSS_TNR_TYMEYCK     VPSS_TNR_TYMEYCK                  ; /* 0x39cc */
    U_VPSS_TNR_TCMYCK      VPSS_TNR_TCMYCK                   ; /* 0x39d0 */
    U_VPSS_TNR_TCMEYCK     VPSS_TNR_TCMEYCK                  ; /* 0x39d4 */
    U_VPSS_TNR_TM_CMT_CK   VPSS_TNR_TM_CMT_CK                ; /* 0x39d8 */
    U_VPSS_TNR_TM_YMT_CK   VPSS_TNR_TM_YMT_CK                ; /* 0x39dc */
    U_VPSS_TNR_TM_CMTMEAN_CK   VPSS_TNR_TM_CMTMEAN_CK        ; /* 0x39e0 */
    U_VPSS_TNR_SNR_CTRL    VPSS_TNR_SNR_CTRL                 ; /* 0x39e4 */
    U_VPSS_TNR_BLEND_CTRL   VPSS_TNR_BLEND_CTRL              ; /* 0x39e8 */
    U_VPSS_TNR_T2D_EDGE_CK   VPSS_TNR_T2D_EDGE_CK            ; /* 0x39ec */
    U_VPSS_TNR_T2D_NOISE_CK   VPSS_TNR_T2D_NOISE_CK          ; /* 0x39f0 */
    U_VPSS_TNR_EDGE_CMOVING_CK   VPSS_TNR_EDGE_CMOVING_CK    ; /* 0x39f4 */
    U_VPSS_TNR_GLBMTN_THD   VPSS_TNR_GLBMTN_THD              ; /* 0x39f8 */
    unsigned int           reserved_20[17]                     ; /* 0x39fc~0x3a3c */
    U_VPSS_NLE_LPF_COEFF0   VPSS_NLE_LPF_COEFF0              ; /* 0x3a40 */
    U_VPSS_NLE_LPF_COEFF1   VPSS_NLE_LPF_COEFF1              ; /* 0x3a44 */
    U_VPSS_NLE_OFFSET      VPSS_NLE_OFFSET                   ; /* 0x3a48 */
    U_VPSS_NLE_NOISE_ZERO_RATIO   VPSS_NLE_NOISE_ZERO_RATIO  ; /* 0x3a4c */
    unsigned int           reserved_21[44]                     ; /* 0x3a50~0x3afc */
    U_VPSS_TNR_CLUT10      VPSS_TNR_CLUT10                   ; /* 0x3b00 */
    U_VPSS_TNR_CLUT11      VPSS_TNR_CLUT11                   ; /* 0x3b04 */
    U_VPSS_TNR_CLUT12      VPSS_TNR_CLUT12                   ; /* 0x3b08 */
    U_VPSS_TNR_CLUT13      VPSS_TNR_CLUT13                   ; /* 0x3b0c */
    U_VPSS_TNR_CLUT14      VPSS_TNR_CLUT14                   ; /* 0x3b10 */
    U_VPSS_TNR_CLUT15      VPSS_TNR_CLUT15                   ; /* 0x3b14 */
    U_VPSS_TNR_CLUT16      VPSS_TNR_CLUT16                   ; /* 0x3b18 */
    U_VPSS_TNR_CLUT17      VPSS_TNR_CLUT17                   ; /* 0x3b1c */
    U_VPSS_TNR_CLUT18      VPSS_TNR_CLUT18                   ; /* 0x3b20 */
    U_VPSS_TNR_CLUT19      VPSS_TNR_CLUT19                   ; /* 0x3b24 */
    U_VPSS_TNR_CLUT110     VPSS_TNR_CLUT110                  ; /* 0x3b28 */
    U_VPSS_TNR_CLUT111     VPSS_TNR_CLUT111                  ; /* 0x3b2c */
    U_VPSS_TNR_CLUT112     VPSS_TNR_CLUT112                  ; /* 0x3b30 */
    U_VPSS_TNR_CLUT113     VPSS_TNR_CLUT113                  ; /* 0x3b34 */
    U_VPSS_TNR_CLUT114     VPSS_TNR_CLUT114                  ; /* 0x3b38 */
    U_VPSS_TNR_CLUT115     VPSS_TNR_CLUT115                  ; /* 0x3b3c */
    U_VPSS_TNR_CLUT20      VPSS_TNR_CLUT20                   ; /* 0x3b40 */
    U_VPSS_TNR_CLUT21      VPSS_TNR_CLUT21                   ; /* 0x3b44 */
    U_VPSS_TNR_CLUT22      VPSS_TNR_CLUT22                   ; /* 0x3b48 */
    U_VPSS_TNR_CLUT23      VPSS_TNR_CLUT23                   ; /* 0x3b4c */
    U_VPSS_TNR_CLUT24      VPSS_TNR_CLUT24                   ; /* 0x3b50 */
    U_VPSS_TNR_CLUT25      VPSS_TNR_CLUT25                   ; /* 0x3b54 */
    U_VPSS_TNR_CLUT26      VPSS_TNR_CLUT26                   ; /* 0x3b58 */
    U_VPSS_TNR_CLUT27      VPSS_TNR_CLUT27                   ; /* 0x3b5c */
    U_VPSS_TNR_CLUT28      VPSS_TNR_CLUT28                   ; /* 0x3b60 */
    U_VPSS_TNR_CLUT29      VPSS_TNR_CLUT29                   ; /* 0x3b64 */
    U_VPSS_TNR_CLUT210     VPSS_TNR_CLUT210                  ; /* 0x3b68 */
    U_VPSS_TNR_CLUT211     VPSS_TNR_CLUT211                  ; /* 0x3b6c */
    U_VPSS_TNR_CLUT212     VPSS_TNR_CLUT212                  ; /* 0x3b70 */
    U_VPSS_TNR_CLUT213     VPSS_TNR_CLUT213                  ; /* 0x3b74 */
    U_VPSS_TNR_CLUT214     VPSS_TNR_CLUT214                  ; /* 0x3b78 */
    U_VPSS_TNR_CLUT215     VPSS_TNR_CLUT215                  ; /* 0x3b7c */
    U_VPSS_TNR_CLUT216     VPSS_TNR_CLUT216                  ; /* 0x3b80 */
    U_VPSS_TNR_CLUT217     VPSS_TNR_CLUT217                  ; /* 0x3b84 */
    U_VPSS_TNR_CLUT218     VPSS_TNR_CLUT218                  ; /* 0x3b88 */
    U_VPSS_TNR_CLUT219     VPSS_TNR_CLUT219                  ; /* 0x3b8c */
    U_VPSS_TNR_CLUT220     VPSS_TNR_CLUT220                  ; /* 0x3b90 */
    U_VPSS_TNR_CLUT221     VPSS_TNR_CLUT221                  ; /* 0x3b94 */
    U_VPSS_TNR_CLUT222     VPSS_TNR_CLUT222                  ; /* 0x3b98 */
    U_VPSS_TNR_CLUT223     VPSS_TNR_CLUT223                  ; /* 0x3b9c */
    U_VPSS_TNR_CLUT224     VPSS_TNR_CLUT224                  ; /* 0x3ba0 */
    U_VPSS_TNR_CLUT225     VPSS_TNR_CLUT225                  ; /* 0x3ba4 */
    U_VPSS_TNR_CLUT226     VPSS_TNR_CLUT226                  ; /* 0x3ba8 */
    U_VPSS_TNR_CLUT227     VPSS_TNR_CLUT227                  ; /* 0x3bac */
    U_VPSS_TNR_CLUT228     VPSS_TNR_CLUT228                  ; /* 0x3bb0 */
    U_VPSS_TNR_CLUT229     VPSS_TNR_CLUT229                  ; /* 0x3bb4 */
    U_VPSS_TNR_CLUT230     VPSS_TNR_CLUT230                  ; /* 0x3bb8 */
    U_VPSS_TNR_CLUT231     VPSS_TNR_CLUT231                  ; /* 0x3bbc */
    U_VPSS_TNR_CLUT232     VPSS_TNR_CLUT232                  ; /* 0x3bc0 */
    U_VPSS_TNR_CLUT233     VPSS_TNR_CLUT233                  ; /* 0x3bc4 */
    U_VPSS_TNR_CLUT234     VPSS_TNR_CLUT234                  ; /* 0x3bc8 */
    U_VPSS_TNR_CLUT235     VPSS_TNR_CLUT235                  ; /* 0x3bcc */
    U_VPSS_TNR_CLUT236     VPSS_TNR_CLUT236                  ; /* 0x3bd0 */
    U_VPSS_TNR_CLUT237     VPSS_TNR_CLUT237                  ; /* 0x3bd4 */
    U_VPSS_TNR_CLUT238     VPSS_TNR_CLUT238                  ; /* 0x3bd8 */
    U_VPSS_TNR_CLUT239     VPSS_TNR_CLUT239                  ; /* 0x3bdc */
    U_VPSS_TNR_CLUT240     VPSS_TNR_CLUT240                  ; /* 0x3be0 */
    U_VPSS_TNR_CLUT241     VPSS_TNR_CLUT241                  ; /* 0x3be4 */
    U_VPSS_TNR_CLUT242     VPSS_TNR_CLUT242                  ; /* 0x3be8 */
    U_VPSS_TNR_CLUT243     VPSS_TNR_CLUT243                  ; /* 0x3bec */
    U_VPSS_TNR_CLUT244     VPSS_TNR_CLUT244                  ; /* 0x3bf0 */
    U_VPSS_TNR_CLUT245     VPSS_TNR_CLUT245                  ; /* 0x3bf4 */
    U_VPSS_TNR_CLUT246     VPSS_TNR_CLUT246                  ; /* 0x3bf8 */
    U_VPSS_TNR_CLUT247     VPSS_TNR_CLUT247                  ; /* 0x3bfc */
    U_VPSS_TNR_CLUT248     VPSS_TNR_CLUT248                  ; /* 0x3c00 */
    U_VPSS_TNR_CLUT249     VPSS_TNR_CLUT249                  ; /* 0x3c04 */
    U_VPSS_TNR_CLUT250     VPSS_TNR_CLUT250                  ; /* 0x3c08 */
    U_VPSS_TNR_CLUT251     VPSS_TNR_CLUT251                  ; /* 0x3c0c */
    U_VPSS_TNR_CLUT252     VPSS_TNR_CLUT252                  ; /* 0x3c10 */
    U_VPSS_TNR_CLUT253     VPSS_TNR_CLUT253                  ; /* 0x3c14 */
    U_VPSS_TNR_CLUT254     VPSS_TNR_CLUT254                  ; /* 0x3c18 */
    U_VPSS_TNR_CLUT255     VPSS_TNR_CLUT255                  ; /* 0x3c1c */
    U_VPSS_TNR_CLUT256     VPSS_TNR_CLUT256                  ; /* 0x3c20 */
    U_VPSS_TNR_CLUT257     VPSS_TNR_CLUT257                  ; /* 0x3c24 */
    U_VPSS_TNR_CLUT258     VPSS_TNR_CLUT258                  ; /* 0x3c28 */
    U_VPSS_TNR_CLUT259     VPSS_TNR_CLUT259                  ; /* 0x3c2c */
    U_VPSS_TNR_CLUT260     VPSS_TNR_CLUT260                  ; /* 0x3c30 */
    U_VPSS_TNR_CLUT261     VPSS_TNR_CLUT261                  ; /* 0x3c34 */
    U_VPSS_TNR_CLUT262     VPSS_TNR_CLUT262                  ; /* 0x3c38 */
    U_VPSS_TNR_CLUT263     VPSS_TNR_CLUT263                  ; /* 0x3c3c */
    U_VPSS_TNR_CLUT264     VPSS_TNR_CLUT264                  ; /* 0x3c40 */
    U_VPSS_TNR_CLUT265     VPSS_TNR_CLUT265                  ; /* 0x3c44 */
    U_VPSS_TNR_CLUT266     VPSS_TNR_CLUT266                  ; /* 0x3c48 */
    U_VPSS_TNR_CLUT267     VPSS_TNR_CLUT267                  ; /* 0x3c4c */
    U_VPSS_TNR_CLUT268     VPSS_TNR_CLUT268                  ; /* 0x3c50 */
    U_VPSS_TNR_CLUT269     VPSS_TNR_CLUT269                  ; /* 0x3c54 */
    U_VPSS_TNR_CLUT270     VPSS_TNR_CLUT270                  ; /* 0x3c58 */
    U_VPSS_TNR_CLUT271     VPSS_TNR_CLUT271                  ; /* 0x3c5c */
    U_VPSS_TNR_CLUT272     VPSS_TNR_CLUT272                  ; /* 0x3c60 */
    U_VPSS_TNR_CLUT273     VPSS_TNR_CLUT273                  ; /* 0x3c64 */
    U_VPSS_TNR_CLUT274     VPSS_TNR_CLUT274                  ; /* 0x3c68 */
    U_VPSS_TNR_CLUT275     VPSS_TNR_CLUT275                  ; /* 0x3c6c */
    U_VPSS_TNR_CLUT276     VPSS_TNR_CLUT276                  ; /* 0x3c70 */
    U_VPSS_TNR_CLUT277     VPSS_TNR_CLUT277                  ; /* 0x3c74 */
    U_VPSS_TNR_CLUT278     VPSS_TNR_CLUT278                  ; /* 0x3c78 */
    U_VPSS_TNR_CLUT279     VPSS_TNR_CLUT279                  ; /* 0x3c7c */
    U_VPSS_TNR_CLUT280     VPSS_TNR_CLUT280                  ; /* 0x3c80 */
    U_VPSS_TNR_CLUT281     VPSS_TNR_CLUT281                  ; /* 0x3c84 */
    U_VPSS_TNR_CLUT282     VPSS_TNR_CLUT282                  ; /* 0x3c88 */
    U_VPSS_TNR_CLUT283     VPSS_TNR_CLUT283                  ; /* 0x3c8c */
    U_VPSS_TNR_CLUT284     VPSS_TNR_CLUT284                  ; /* 0x3c90 */
    U_VPSS_TNR_CLUT285     VPSS_TNR_CLUT285                  ; /* 0x3c94 */
    U_VPSS_TNR_CLUT286     VPSS_TNR_CLUT286                  ; /* 0x3c98 */
    U_VPSS_TNR_CLUT287     VPSS_TNR_CLUT287                  ; /* 0x3c9c */
    U_VPSS_TNR_CLUT288     VPSS_TNR_CLUT288                  ; /* 0x3ca0 */
    U_VPSS_TNR_CLUT289     VPSS_TNR_CLUT289                  ; /* 0x3ca4 */
    U_VPSS_TNR_CLUT290     VPSS_TNR_CLUT290                  ; /* 0x3ca8 */
    U_VPSS_TNR_CLUT291     VPSS_TNR_CLUT291                  ; /* 0x3cac */
    U_VPSS_TNR_CLUT292     VPSS_TNR_CLUT292                  ; /* 0x3cb0 */
    U_VPSS_TNR_CLUT293     VPSS_TNR_CLUT293                  ; /* 0x3cb4 */
    U_VPSS_TNR_CLUT294     VPSS_TNR_CLUT294                  ; /* 0x3cb8 */
    U_VPSS_TNR_CLUT295     VPSS_TNR_CLUT295                  ; /* 0x3cbc */
    U_VPSS_TNR_CLUT296     VPSS_TNR_CLUT296                  ; /* 0x3cc0 */
    U_VPSS_TNR_CLUT297     VPSS_TNR_CLUT297                  ; /* 0x3cc4 */
    U_VPSS_TNR_CLUT298     VPSS_TNR_CLUT298                  ; /* 0x3cc8 */
    U_VPSS_TNR_CLUT299     VPSS_TNR_CLUT299                  ; /* 0x3ccc */
    U_VPSS_TNR_CLUT2100    VPSS_TNR_CLUT2100                 ; /* 0x3cd0 */
    U_VPSS_TNR_CLUT2101    VPSS_TNR_CLUT2101                 ; /* 0x3cd4 */
    U_VPSS_TNR_CLUT2102    VPSS_TNR_CLUT2102                 ; /* 0x3cd8 */
    U_VPSS_TNR_CLUT2103    VPSS_TNR_CLUT2103                 ; /* 0x3cdc */
    U_VPSS_TNR_CLUT2104    VPSS_TNR_CLUT2104                 ; /* 0x3ce0 */
    U_VPSS_TNR_CLUT2105    VPSS_TNR_CLUT2105                 ; /* 0x3ce4 */
    U_VPSS_TNR_CLUT2106    VPSS_TNR_CLUT2106                 ; /* 0x3ce8 */
    U_VPSS_TNR_CLUT2107    VPSS_TNR_CLUT2107                 ; /* 0x3cec */
    U_VPSS_TNR_CLUT2108    VPSS_TNR_CLUT2108                 ; /* 0x3cf0 */
    U_VPSS_TNR_CLUT2109    VPSS_TNR_CLUT2109                 ; /* 0x3cf4 */
    U_VPSS_TNR_CLUT2110    VPSS_TNR_CLUT2110                 ; /* 0x3cf8 */
    U_VPSS_TNR_CLUT2111    VPSS_TNR_CLUT2111                 ; /* 0x3cfc */
    U_VPSS_TNR_CLUT2112    VPSS_TNR_CLUT2112                 ; /* 0x3d00 */
    U_VPSS_TNR_CLUT2113    VPSS_TNR_CLUT2113                 ; /* 0x3d04 */
    U_VPSS_TNR_CLUT2114    VPSS_TNR_CLUT2114                 ; /* 0x3d08 */
    U_VPSS_TNR_CLUT2115    VPSS_TNR_CLUT2115                 ; /* 0x3d0c */
    U_VPSS_TNR_CLUT2116    VPSS_TNR_CLUT2116                 ; /* 0x3d10 */
    U_VPSS_TNR_CLUT2117    VPSS_TNR_CLUT2117                 ; /* 0x3d14 */
    U_VPSS_TNR_CLUT2118    VPSS_TNR_CLUT2118                 ; /* 0x3d18 */
    U_VPSS_TNR_CLUT2119    VPSS_TNR_CLUT2119                 ; /* 0x3d1c */
    U_VPSS_TNR_CLUT2120    VPSS_TNR_CLUT2120                 ; /* 0x3d20 */
    U_VPSS_TNR_CLUT2121    VPSS_TNR_CLUT2121                 ; /* 0x3d24 */
    U_VPSS_TNR_CLUT2122    VPSS_TNR_CLUT2122                 ; /* 0x3d28 */
    U_VPSS_TNR_CLUT2123    VPSS_TNR_CLUT2123                 ; /* 0x3d2c */
    U_VPSS_TNR_CLUT2124    VPSS_TNR_CLUT2124                 ; /* 0x3d30 */
    U_VPSS_TNR_CLUT2125    VPSS_TNR_CLUT2125                 ; /* 0x3d34 */
    U_VPSS_TNR_CLUT2126    VPSS_TNR_CLUT2126                 ; /* 0x3d38 */
    U_VPSS_TNR_CLUT2127    VPSS_TNR_CLUT2127                 ; /* 0x3d3c */
    U_VPSS_TNR_CLUT30      VPSS_TNR_CLUT30                   ; /* 0x3d40 */
    U_VPSS_TNR_CLUT31      VPSS_TNR_CLUT31                   ; /* 0x3d44 */
    U_VPSS_TNR_CLUT32      VPSS_TNR_CLUT32                   ; /* 0x3d48 */
    U_VPSS_TNR_CLUT33      VPSS_TNR_CLUT33                   ; /* 0x3d4c */
    U_VPSS_TNR_CLUT34      VPSS_TNR_CLUT34                   ; /* 0x3d50 */
    U_VPSS_TNR_CLUT35      VPSS_TNR_CLUT35                   ; /* 0x3d54 */
    U_VPSS_TNR_CLUT36      VPSS_TNR_CLUT36                   ; /* 0x3d58 */
    U_VPSS_TNR_CLUT37      VPSS_TNR_CLUT37                   ; /* 0x3d5c */
    U_VPSS_TNR_CLUT38      VPSS_TNR_CLUT38                   ; /* 0x3d60 */
    U_VPSS_TNR_CLUT39      VPSS_TNR_CLUT39                   ; /* 0x3d64 */
    U_VPSS_TNR_CLUT310     VPSS_TNR_CLUT310                  ; /* 0x3d68 */
    U_VPSS_TNR_CLUT311     VPSS_TNR_CLUT311                  ; /* 0x3d6c */
    U_VPSS_TNR_CLUT312     VPSS_TNR_CLUT312                  ; /* 0x3d70 */
    U_VPSS_TNR_CLUT313     VPSS_TNR_CLUT313                  ; /* 0x3d74 */
    U_VPSS_TNR_CLUT314     VPSS_TNR_CLUT314                  ; /* 0x3d78 */
    U_VPSS_TNR_CLUT315     VPSS_TNR_CLUT315                  ; /* 0x3d7c */
    U_VPSS_TNR_CLUT316     VPSS_TNR_CLUT316                  ; /* 0x3d80 */
    U_VPSS_TNR_CLUT317     VPSS_TNR_CLUT317                  ; /* 0x3d84 */
    U_VPSS_TNR_CLUT318     VPSS_TNR_CLUT318                  ; /* 0x3d88 */
    U_VPSS_TNR_CLUT319     VPSS_TNR_CLUT319                  ; /* 0x3d8c */
    U_VPSS_TNR_CLUT320     VPSS_TNR_CLUT320                  ; /* 0x3d90 */
    U_VPSS_TNR_CLUT321     VPSS_TNR_CLUT321                  ; /* 0x3d94 */
    U_VPSS_TNR_CLUT322     VPSS_TNR_CLUT322                  ; /* 0x3d98 */
    U_VPSS_TNR_CLUT323     VPSS_TNR_CLUT323                  ; /* 0x3d9c */
    U_VPSS_TNR_CLUT324     VPSS_TNR_CLUT324                  ; /* 0x3da0 */
    U_VPSS_TNR_CLUT325     VPSS_TNR_CLUT325                  ; /* 0x3da4 */
    U_VPSS_TNR_CLUT326     VPSS_TNR_CLUT326                  ; /* 0x3da8 */
    U_VPSS_TNR_CLUT327     VPSS_TNR_CLUT327                  ; /* 0x3dac */
    U_VPSS_TNR_CLUT328     VPSS_TNR_CLUT328                  ; /* 0x3db0 */
    U_VPSS_TNR_CLUT329     VPSS_TNR_CLUT329                  ; /* 0x3db4 */
    U_VPSS_TNR_CLUT330     VPSS_TNR_CLUT330                  ; /* 0x3db8 */
    U_VPSS_TNR_CLUT331     VPSS_TNR_CLUT331                  ; /* 0x3dbc */
    U_VPSS_TNR_CLUT40      VPSS_TNR_CLUT40                   ; /* 0x3dc0 */
    U_VPSS_TNR_CLUT41      VPSS_TNR_CLUT41                   ; /* 0x3dc4 */
    U_VPSS_TNR_CLUT42      VPSS_TNR_CLUT42                   ; /* 0x3dc8 */
    U_VPSS_TNR_CLUT43      VPSS_TNR_CLUT43                   ; /* 0x3dcc */
    U_VPSS_TNR_CLUT44      VPSS_TNR_CLUT44                   ; /* 0x3dd0 */
    U_VPSS_TNR_CLUT45      VPSS_TNR_CLUT45                   ; /* 0x3dd4 */
    U_VPSS_TNR_CLUT46      VPSS_TNR_CLUT46                   ; /* 0x3dd8 */
    U_VPSS_TNR_CLUT47      VPSS_TNR_CLUT47                   ; /* 0x3ddc */
    U_VPSS_TNR_CLUT48      VPSS_TNR_CLUT48                   ; /* 0x3de0 */
    U_VPSS_TNR_CLUT49      VPSS_TNR_CLUT49                   ; /* 0x3de4 */
    U_VPSS_TNR_CLUT410     VPSS_TNR_CLUT410                  ; /* 0x3de8 */
    U_VPSS_TNR_CLUT411     VPSS_TNR_CLUT411                  ; /* 0x3dec */
    U_VPSS_TNR_CLUT412     VPSS_TNR_CLUT412                  ; /* 0x3df0 */
    U_VPSS_TNR_CLUT413     VPSS_TNR_CLUT413                  ; /* 0x3df4 */
    U_VPSS_TNR_CLUT414     VPSS_TNR_CLUT414                  ; /* 0x3df8 */
    U_VPSS_TNR_CLUT415     VPSS_TNR_CLUT415                  ; /* 0x3dfc */
    U_VPSS_TNR_CLUT416     VPSS_TNR_CLUT416                  ; /* 0x3e00 */
    U_VPSS_TNR_CLUT417     VPSS_TNR_CLUT417                  ; /* 0x3e04 */
    U_VPSS_TNR_CLUT418     VPSS_TNR_CLUT418                  ; /* 0x3e08 */
    U_VPSS_TNR_CLUT419     VPSS_TNR_CLUT419                  ; /* 0x3e0c */
    U_VPSS_TNR_CLUT420     VPSS_TNR_CLUT420                  ; /* 0x3e10 */
    U_VPSS_TNR_CLUT421     VPSS_TNR_CLUT421                  ; /* 0x3e14 */
    U_VPSS_TNR_CLUT422     VPSS_TNR_CLUT422                  ; /* 0x3e18 */
    U_VPSS_TNR_CLUT423     VPSS_TNR_CLUT423                  ; /* 0x3e1c */
    U_VPSS_TNR_CLUT424     VPSS_TNR_CLUT424                  ; /* 0x3e20 */
    U_VPSS_TNR_CLUT425     VPSS_TNR_CLUT425                  ; /* 0x3e24 */
    U_VPSS_TNR_CLUT426     VPSS_TNR_CLUT426                  ; /* 0x3e28 */
    U_VPSS_TNR_CLUT427     VPSS_TNR_CLUT427                  ; /* 0x3e2c */
    U_VPSS_TNR_CLUT428     VPSS_TNR_CLUT428                  ; /* 0x3e30 */
    U_VPSS_TNR_CLUT429     VPSS_TNR_CLUT429                  ; /* 0x3e34 */
    U_VPSS_TNR_CLUT430     VPSS_TNR_CLUT430                  ; /* 0x3e38 */
    U_VPSS_TNR_CLUT431     VPSS_TNR_CLUT431                  ; /* 0x3e3c */
    U_VPSS_TNR_CLUT50      VPSS_TNR_CLUT50                   ; /* 0x3e40 */
    U_VPSS_TNR_CLUT51      VPSS_TNR_CLUT51                   ; /* 0x3e44 */
    U_VPSS_TNR_CLUT52      VPSS_TNR_CLUT52                   ; /* 0x3e48 */
    U_VPSS_TNR_CLUT53      VPSS_TNR_CLUT53                   ; /* 0x3e4c */
    U_VPSS_TNR_CLUT54      VPSS_TNR_CLUT54                   ; /* 0x3e50 */
    U_VPSS_TNR_CLUT55      VPSS_TNR_CLUT55                   ; /* 0x3e54 */
    U_VPSS_TNR_CLUT56      VPSS_TNR_CLUT56                   ; /* 0x3e58 */
    U_VPSS_TNR_CLUT57      VPSS_TNR_CLUT57                   ; /* 0x3e5c */
    U_VPSS_TNR_CLUT60      VPSS_TNR_CLUT60                   ; /* 0x3e60 */
    U_VPSS_TNR_CLUT61      VPSS_TNR_CLUT61                   ; /* 0x3e64 */
    U_VPSS_TNR_CLUT62      VPSS_TNR_CLUT62                   ; /* 0x3e68 */
    U_VPSS_TNR_CLUT63      VPSS_TNR_CLUT63                   ; /* 0x3e6c */
    U_VPSS_TNR_CLUT64      VPSS_TNR_CLUT64                   ; /* 0x3e70 */
    U_VPSS_TNR_CLUT65      VPSS_TNR_CLUT65                   ; /* 0x3e74 */
    U_VPSS_TNR_CLUT66      VPSS_TNR_CLUT66                   ; /* 0x3e78 */
    U_VPSS_TNR_CLUT67      VPSS_TNR_CLUT67                   ; /* 0x3e7c */
    unsigned int           reserved_22[96]                     ; /* 0x3e80~0x3ffc */
    U_VPSS_DEBUG0          VPSS_DEBUG0                       ; /* 0x4000 */
    U_VPSS_DEBUG1          VPSS_DEBUG1                       ; /* 0x4004 */
    U_VPSS_DEBUG2          VPSS_DEBUG2                       ; /* 0x4008 */
    U_VPSS_DEBUG3          VPSS_DEBUG3                       ; /* 0x400c */
    U_VPSS_DEBUG4          VPSS_DEBUG4                       ; /* 0x4010 */
    U_VPSS_DEBUG5          VPSS_DEBUG5                       ; /* 0x4014 */
    U_VPSS_DEBUG6          VPSS_DEBUG6                       ; /* 0x4018 */
    U_VPSS_DEBUG7          VPSS_DEBUG7                       ; /* 0x401c */
    U_VPSS_DEBUG8          VPSS_DEBUG8                       ; /* 0x4020 */
    U_VPSS_DEBUG9          VPSS_DEBUG9                       ; /* 0x4024 */
    U_VPSS_DEBUG10         VPSS_DEBUG10                      ; /* 0x4028 */
    U_VPSS_DEBUG11         VPSS_DEBUG11                      ; /* 0x402c */
    U_VPSS_DEBUG12         VPSS_DEBUG12                      ; /* 0x4030 */
    U_VPSS_DEBUG13         VPSS_DEBUG13                      ; /* 0x4034 */
    U_VPSS_DEBUG14         VPSS_DEBUG14                      ; /* 0x4038 */
    U_VPSS_DEBUG15         VPSS_DEBUG15                      ; /* 0x403c */
    U_VPSS_DEBUG16         VPSS_DEBUG16                      ; /* 0x4040 */
    U_VPSS_DEBUG17         VPSS_DEBUG17                      ; /* 0x4044 */
    U_VPSS_DEBUG18         VPSS_DEBUG18                      ; /* 0x4048 */
    U_VPSS_DEBUG19         VPSS_DEBUG19                      ; /* 0x404c */
    U_VPSS_DEBUG20         VPSS_DEBUG20                      ; /* 0x4050 */
    U_VPSS_DEBUG21         VPSS_DEBUG21                      ; /* 0x4054 */
    U_VPSS_DEBUG22         VPSS_DEBUG22                      ; /* 0x4058 */
    U_VPSS_DEBUG23         VPSS_DEBUG23                      ; /* 0x405c */
    U_VPSS_DEBUG24         VPSS_DEBUG24                      ; /* 0x4060 */
    U_VPSS_DEBUG25         VPSS_DEBUG25                      ; /* 0x4064 */
    U_VPSS_DEBUG26         VPSS_DEBUG26                      ; /* 0x4068 */
    U_VPSS_DEBUG27         VPSS_DEBUG27                      ; /* 0x406c */
    U_VPSS_DEBUG28         VPSS_DEBUG28                      ; /* 0x4070 */
    U_VPSS_DEBUG29         VPSS_DEBUG29                      ; /* 0x4074 */
    U_VPSS_DEBUG30         VPSS_DEBUG30                      ; /* 0x4078 */
    U_VPSS_DEBUG31         VPSS_DEBUG31                      ; /* 0x407c */
    U_VPSS_DEBUG32         VPSS_DEBUG32                      ; /* 0x4080 */
    U_VPSS_DEBUG33         VPSS_DEBUG33                      ; /* 0x4084 */
    U_VPSS_DEBUG34         VPSS_DEBUG34                      ; /* 0x4088 */
    U_VPSS_DEBUG35         VPSS_DEBUG35                      ; /* 0x408c */
    U_VPSS_DEBUG36         VPSS_DEBUG36                      ; /* 0x4090 */
    U_VPSS_DEBUG37         VPSS_DEBUG37                      ; /* 0x4094 */
    U_VPSS_DEBUG38         VPSS_DEBUG38                      ; /* 0x4098 */
    U_VPSS_DEBUG39         VPSS_DEBUG39                      ; /* 0x409c */
    U_VPSS_DEBUG40         VPSS_DEBUG40                      ; /* 0x40a0 */
    U_VPSS_DEBUG41         VPSS_DEBUG41                      ; /* 0x40a4 */
    U_VPSS_DEBUG42         VPSS_DEBUG42                      ; /* 0x40a8 */
    U_VPSS_DEBUG43         VPSS_DEBUG43                      ; /* 0x40ac */
    unsigned int           reserved_23[996]                     ; /* 0x40b0~0x503c */
    U_VPSS_DEI_WST_CHKSUM   VPSS_DEI_WST_CHKSUM              ; /* 0x5040 */
    unsigned int           reserved_24[2]                     ; /* 0x5044~0x5048 */
    U_VPSS_VHD0_Y_CHKSUM   VPSS_VHD0_Y_CHKSUM                ; /* 0x504c */
    U_VPSS_VHD0_C_CHKSUM   VPSS_VHD0_C_CHKSUM                ; /* 0x5050 */
    U_VPSS_STR_Y_CHKSUM    VPSS_STR_Y_CHKSUM                 ; /* 0x5054 */
    U_VPSS_STR_C_CHKSUM    VPSS_STR_C_CHKSUM                 ; /* 0x5058 */

} VPSS_REG_S;

#endif
/* Declare the struct pointor of the module CAS */
extern VPSS_REG_S *gopCASAllReg;


HI_S32 VPSS_REG_RegWrite(volatile HI_U32 *a, HI_U32 b);
HI_U32 VPSS_REG_RegRead(volatile HI_U32* a);
HI_S32 VPSS_REG_ReSetCRG(HI_U32 u32Type);
HI_S32 VPSS_REG_BaseRegInit(VPSS_REG_S **ppstPhyReg);
HI_S32 VPSS_REG_AppRegInit(VPSS_REG_S **ppstAppReg,HI_U32 u32VirAddr);

HI_S32 VPSS_REG_SetClockEn(HI_U32 u32Type, HI_BOOL bClkEn);

HI_S32 VPSS_REG_RegWrite(volatile HI_U32 *a, HI_U32 b);


HI_U32 VPSS_REG_RegRead(volatile HI_U32* a);


HI_S32 VPSS_REG_BaseRegInit(VPSS_REG_S **ppstPhyReg);



HI_S32 VPSS_REG_AppRegInit(VPSS_REG_S **ppstAppReg,HI_U32 u32VirAddr);



HI_S32 VPSS_REG_ResetAppReg(HI_U32 u32AppAddr, VPSS_REG_S *pstPqReg);



HI_S32 VPSS_REG_SetIntMask(HI_U32 u32AppAddr,HI_U32 u32Mask);

HI_S32 VPSS_REG_GetIntMask(HI_U32 u32AppAddr,HI_U32 *pu32Mask);

HI_S32 VPSS_REG_GetIntState(HI_U32 u32AppAddr,HI_U32 *pu32Int);

HI_S32 VPSS_REG_GetRawIntState(HI_U32 u32AppAddr,HI_U32 *pu32RawInt);

HI_S32 VPSS_REG_ClearIntState(HI_U32 u32AppAddr,HI_U32 u32Data);




HI_S32 VPSS_REG_SetTimeOut(HI_U32 u32AppAddr,HI_U32 u32Data);


HI_S32 VPSS_REG_StartLogic(HI_U32 u32AppAddr,HI_U32 u32PhyAddr);

HI_S32 VPSS_REG_SetAddr(HI_U32 u32AppAddr,VPSS_REG_ADDR_E eType,HI_U32 u32Addr);


HI_S32 VPSS_REG_CloseClock(HI_VOID);

HI_S32 VPSS_REG_OpenClock(HI_VOID);

HI_S32 VPSS_REG_GetClockState(HI_BOOL *pbOpened);

HI_S32 VPSS_REG_SetCurPackage(HI_U32 u32AppAddr,HI_U32 u32CurPackage422);
 //lzx

HI_S32 VPSS_REG_SetRefPackage(HI_U32 u32AppAddr,HI_U32 u32RefPackage422);
 //lzx


HI_S32 VPSS_REG_SetProtEn(HI_U32 u32AppAddr,HI_U32 u32Prot);
 //lzx

HI_S32 VPSS_REG_SetFr0VC1_Profile(HI_U32 u32AppAddr,HI_U32 u32vc1profile);
 //lzx

HI_S32 VPSS_REG_SetFr0VC1_RangedFrm(HI_U32 u32AppAddr,HI_U32 u32vc1rangedfrm);
 //lzx

HI_S32 VPSS_REG_SetFr0VC1_MapyFlg(HI_U32 u32AppAddr,HI_U32 u32vc1mapyflg);
 //lzx

HI_S32 VPSS_REG_SetFr0VC1_MapcFlg(HI_U32 u32AppAddr,HI_U32 u32vc1mapcflg);
 //lzx

HI_S32 VPSS_REG_SetFr0VC1_Mapy(HI_U32 u32AppAddr,HI_U32 u32vc1mapy);
 //lzx

HI_S32 VPSS_REG_SetFr0VC1_Mapc(HI_U32 u32AppAddr,HI_U32 u32vc1macy);
 //lzx

HI_S32 VPSS_REG_SetFr0VC1Enable(HI_U32 u32AppAddr,HI_BOOL bVc1Enable);
 //lzx

HI_S32 VPSS_REG_SetFr1VC1_Profile(HI_U32 u32AppAddr,HI_U32 u32vc1profile);
 //lzx

HI_S32 VPSS_REG_SetFr1VC1_RangedFrm(HI_U32 u32AppAddr,HI_U32 u32vc1rangedfrm);
 //lzx

HI_S32 VPSS_REG_SetFr1VC1_MapyFlg(HI_U32 u32AppAddr,HI_U32 u32vc1mapyflg);
 //lzx

HI_S32 VPSS_REG_SetFr1VC1_MapcFlg(HI_U32 u32AppAddr,HI_U32 u32vc1mapcflg);
 //lzx

HI_S32 VPSS_REG_SetFr1VC1_Mapy(HI_U32 u32AppAddr,HI_U32 u32vc1mapy);
 //lzx

HI_S32 VPSS_REG_SetFr1VC1_Mapc(HI_U32 u32AppAddr,HI_U32 u32vc1macy);
 //lzx

HI_S32 VPSS_REG_SetFr1VC1Enable(HI_U32 u32AppAddr,HI_BOOL bVc1Enable);
 //lzx

HI_S32 VPSS_REG_SetFr2VC1_Profile(HI_U32 u32AppAddr,HI_U32 u32vc1profile);
 //lzx

HI_S32 VPSS_REG_SetFr2VC1_RangedFrm(HI_U32 u32AppAddr,HI_U32 u32vc1rangedfrm);
 //lzx

HI_S32 VPSS_REG_SetFr2VC1_MapyFlg(HI_U32 u32AppAddr,HI_U32 u32vc1mapyflg);
 //lzx

HI_S32 VPSS_REG_SetFr2VC1_MapcFlg(HI_U32 u32AppAddr,HI_U32 u32vc1mapcflg);
 //lzx

HI_S32 VPSS_REG_SetFr2VC1_Mapy(HI_U32 u32AppAddr,HI_U32 u32vc1mapy);
 //lzx

HI_S32 VPSS_REG_SetFr2VC1_Mapc(HI_U32 u32AppAddr,HI_U32 u32vc1macy);
 //lzx

HI_S32 VPSS_REG_SetFr2VC1Enable(HI_U32 u32AppAddr,HI_BOOL bVc1Enable);
 //lzx

HI_S32 VPSS_REG_SetFr3VC1_Profile(HI_U32 u32AppAddr,HI_U32 u32vc1profile);
 //lzx

HI_S32 VPSS_REG_SetFr3VC1_RangedFrm(HI_U32 u32AppAddr,HI_U32 u32vc1rangedfrm);
 //lzx

HI_S32 VPSS_REG_SetFr3VC1_MapyFlg(HI_U32 u32AppAddr,HI_U32 u32vc1mapyflg);
 //lzx

HI_S32 VPSS_REG_SetFr3VC1_MapcFlg(HI_U32 u32AppAddr,HI_U32 u32vc1mapcflg);
 //lzx

HI_S32 VPSS_REG_SetFr3VC1_Mapy(HI_U32 u32AppAddr,HI_U32 u32vc1mapy);
 //lzx

HI_S32 VPSS_REG_SetFr3VC1_Mapc(HI_U32 u32AppAddr,HI_U32 u32vc1macy);
 //lzx

HI_S32 VPSS_REG_SetFr3VC1Enable(HI_U32 u32AppAddr,HI_BOOL bVc1Enable);
 //lzx


HI_S32 VPSS_REG_SetRch0SumSel(HI_U32 u32AppAddr,HI_U32 u32rch0sumsel);
 //lzx

HI_S32 VPSS_REG_SetRch1SumSel(HI_U32 u32AppAddr,HI_U32 u32rch1sumsel);
 //lzx

HI_S32 VPSS_REG_SetRch2SumSel(HI_U32 u32AppAddr,HI_U32 u32rch2sumsel);
 //lzx

HI_S32 VPSS_REG_SetRch3SumSel(HI_U32 u32AppAddr,HI_U32 u32rch3sumsel);
 //lzx

HI_S32 VPSS_REG_SetRinfoSumSel(HI_U32 u32AppAddr,HI_U32 u32rinfosumsel);
 //lzx

HI_S32 VPSS_REG_SetWinfoSumSel(HI_U32 u32AppAddr,HI_U32 u32winfosumsel);
 //lzx


HI_S32 VPSS_REG_Set_Chk_Sum_En(HI_U32 u32AppAddr,HI_U32 u32Chksum);
 //lzx




HI_S32 VPSS_REG_SetRfrEn(HI_U32 u32AppAddr,HI_U32 u32RfrEnable);
 //lzx

HI_S32 VPSS_REG_SetTestPatEn(HI_U32 u32AppAddr,HI_U32 u32TestPatEnable);
 //lzx


HI_S32 VPSS_REG_SetMadRaddr(HI_U32 u32AppAddr,HI_U32 u32MadRaddr);
 //lzxa

HI_S32 VPSS_REG_SetMadWaddr(HI_U32 u32AppAddr,HI_U32 u32MadWaddr);
 //lzx

HI_S32 VPSS_REG_SetMadStride(HI_U32 u32AppAddr,HI_U32 u32MadStride);
 //lzx

 HI_S32 VPSS_REG_SetRfrAddr(HI_U32 u32AppAddr,HI_U32 u32RfrYaddr,HI_U32 u32RfrCaddr);
 //lzxa 	

 HI_S32 VPSS_REG_SetRfrStride(HI_U32 u32AppAddr,HI_U32 u32RfrYStride,HI_U32 u32RfrCStride);
 //lzx

HI_S32 VPSS_REG_SetVHD0_MIRROR(HI_U32 u32AppAddr,HI_U32 u32Vhd0Mirror);
 //lzx

HI_S32 VPSS_REG_SetVHD0_FLIP(HI_U32 u32AppAddr,HI_U32 u32Vhd0Flip);
 //lzx

HI_S32 VPSS_REG_SetVHD0_UVINVERT(HI_U32 u32AppAddr,HI_U32 u32Vhd0uvivt);
 //lzx


 HI_S32 VPSS_REG_SetRefSize(HI_U32 u32AppAddr,HI_U32 RefWidth,HI_U32 RefHight);
  //lzx

HI_S32 VPSS_REG_SetLcmpEn(HI_U32 u32AppAddr,HI_U32 LcmpEnable);
 //lzx

HI_S32 VPSS_REG_Set_Y_Is_Lossless(HI_U32 u32AppAddr,HI_U32 u32YIslossless);
 //lzx

HI_S32 VPSS_REG_Set_Y_Cmp_Mode(HI_U32 u32AppAddr,HI_U32 u32YCmpmode);
 //lzx

HI_S32 VPSS_REG_Set_Y_Dw_Mode(HI_U32 u32AppAddr,HI_U32 u32YDwmode);
 //lzx

HI_S32 VPSS_REG_Set_C_Is_Lossless(HI_U32 u32AppAddr,HI_U32 u32CIslossless);
 //lzx

HI_S32 VPSS_REG_Set_C_Cmp_Mode(HI_U32 u32AppAddr,HI_U32 u32CCmpmode);
 //lzx

HI_S32 VPSS_REG_Set_C_Dw_Mode(HI_U32 u32AppAddr,HI_U32 u32CDwmode);
 //lzx


HI_S32 VPSS_REG_Set_Y_Frame_Width(HI_U32 u32AppAddr,HI_U32 u32YFramewidth);
 //lzx


HI_S32 VPSS_REG_Set_Y_Frame_Height(HI_U32 u32AppAddr,HI_U32 u32YFramehight);
 //lzx

HI_S32 VPSS_REG_Set_C_Frame_Width(HI_U32 u32AppAddr,HI_U32 u32CFramewidth);
 //lzx


HI_S32 VPSS_REG_Set_C_Frame_Height(HI_U32 u32AppAddr,HI_U32 u32CFramehight);
 //lzx

HI_S32 VPSS_REG_Set_Y_MbBits(HI_U32 u32AppAddr,HI_U32 u32YMb_bits);
 //lzx

HI_S32 VPSS_REG_Set_Y_RiBits(HI_U32 u32AppAddr,HI_U32 u32YRi_bits);
 //lzx

HI_S32 VPSS_REG_Set_Y_MaxRgCompBits(HI_U32 u32AppAddr,HI_U32 u32YMaxRgCompB);
 //lzx

HI_S32 VPSS_REG_Set_Y_MaxQp(HI_U32 u32AppAddr,HI_U32 u32YMaxQp);
 //lzx

HI_S32 VPSS_REG_Set_Y_SadNgain(HI_U32 u32AppAddr,HI_U32 u32YSadNgain);
 //lzx

HI_S32 VPSS_REG_Set_Y_RcSmthNgain(HI_U32 u32AppAddr,HI_U32 u32YRcSmthNgain);
 //lzx

HI_S32 VPSS_REG_Set_Y_SmthThr(HI_U32 u32AppAddr,HI_U32 u32YSmthThr);
 //lzx

HI_S32 VPSS_REG_Set_Y_SmthPixNumThr(HI_U32 u32AppAddr,HI_U32 u32YSmthPixNumThr);
 //lzx

HI_S32 VPSS_REG_Set_Y_PixDiffThr(HI_U32 u32AppAddr,HI_U32 u32YPixDiffThr);
 //lzx

HI_S32 VPSS_REG_Set_Y_AdjSadThr(HI_U32 u32AppAddr,HI_U32 u32YAdjSadThr);
 //lzx

HI_S32 VPSS_REG_Set_Y_AdjSadBitThr(HI_U32 u32AppAddr,HI_U32 u32YAdjSadbitThr);
 //lzx

HI_S32 VPSS_REG_Set_Y_QpChg1BitsThr(HI_U32 u32AppAddr,HI_U32 u32YQpChg1Thr);
 //lzx

HI_S32 VPSS_REG_Set_Y_QpChg2BitsThr(HI_U32 u32AppAddr,HI_U32 u32YQpChg2Thr);
 //lzx

HI_S32 VPSS_REG_Set_Y_SmthQp0(HI_U32 u32AppAddr,HI_U32 u32YSmthQp0);
 //lzx

HI_S32 VPSS_REG_Set_Y_SmthQp1(HI_U32 u32AppAddr,HI_U32 u32YSmthQp1);
 //lzx

HI_S32 VPSS_REG_Set_Y_SmthLftBThr0(HI_U32 u32AppAddr,HI_U32 u32YSmthLftBThr0);
 //lzx

HI_S32 VPSS_REG_Set_Y_SmthLftBThr1(HI_U32 u32AppAddr,HI_U32 u32YSmthLftBThr1);
 //lzx

HI_S32 VPSS_REG_Set_Y_MaxRowLen(HI_U32 u32AppAddr,HI_U32 u32YMaxRowLen);
 //lzx

HI_S32 VPSS_REG_SetMax_Row_Len(HI_U32 u32AppAddr,HI_U32 u32MaxRowLen);
 //lzx

HI_S32 VPSS_REG_Set_C_MbBits(HI_U32 u32AppAddr,HI_U32 u32CMb_bits);
 //lzx

HI_S32 VPSS_REG_Set_C_RiBits(HI_U32 u32AppAddr,HI_U32 u32CRi_bits);
 //lzx

HI_S32 VPSS_REG_Set_C_MaxRgCompBits(HI_U32 u32AppAddr,HI_U32 u32CMaxRgCompB);
 //lzx

HI_S32 VPSS_REG_Set_C_Max_Qp(HI_U32 u32AppAddr,HI_U32 u32CMaxQp);
 //lzx

HI_S32 VPSS_REG_Set_C_SadNgain(HI_U32 u32AppAddr,HI_U32 u32CSadNgain);
 //lzx

HI_S32 VPSS_REG_Set_C_RcSmthNgain(HI_U32 u32AppAddr,HI_U32 u32CRcSmthNgain);
 //lzx

HI_S32 VPSS_REG_Set_C_SmthThr(HI_U32 u32AppAddr,HI_U32 u32CSmthThr);
 //lzx

HI_S32 VPSS_REG_Set_C_SmthPixNumThr(HI_U32 u32AppAddr,HI_U32 u32CSmthPixNumThr);
 //lzx

HI_S32 VPSS_REG_Set_C_PixDiffThr(HI_U32 u32AppAddr,HI_U32 u32CPixDiffThr);
 //lzx

HI_S32 VPSS_REG_Set_C_AdjSadThr(HI_U32 u32AppAddr,HI_U32 u32CAdjSadThr);
 //lzx

HI_S32 VPSS_REG_Set_C_AdjSadBitThr(HI_U32 u32AppAddr,HI_U32 u32CAdjSadbitThr);
 //lzx

HI_S32 VPSS_REG_Set_C_QpChg1BitsThr(HI_U32 u32AppAddr,HI_U32 u32CQpChg1Thr);
 //lzx

HI_S32 VPSS_REG_Set_C_QpChg2BitsThr(HI_U32 u32AppAddr,HI_U32 u32CQpChg2Thr);
 //lzx

HI_S32 VPSS_REG_Set_C_SmthQp0(HI_U32 u32AppAddr,HI_U32 u32CSmthQp0);
 //lzx

HI_S32 VPSS_REG_Set_C_SmthQp1(HI_U32 u32AppAddr,HI_U32 u32CSmthQp1);
 //lzx

HI_S32 VPSS_REG_Set_C_SmthLftBThr0(HI_U32 u32AppAddr,HI_U32 u32CSmthLftBThr0);
 //lzx

HI_S32 VPSS_REG_Set_C_SmthLftBThr1(HI_U32 u32AppAddr,HI_U32 u32CSmthLftBThr1);
 //lzx

HI_S32 VPSS_REG_Set_C_MaxRowLen(HI_U32 u32AppAddr,HI_U32 u32CMaxRowLen);
 //lzx

HI_S32 VPSS_REG_SetAxiRID(HI_U32 u32AppAddr,REG_AXI_RID_CHN_E eChn, HI_U32 u32AxiRID);
  //lzxa

HI_S32 VPSS_REG_SetDbmEn(HI_U32 u32AppAddr,HI_U32 DbmEnable);
 //lzx

HI_S32 VPSS_REG_EnPort(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bEnable);


HI_S32 VPSS_REG_SetTnrEn(HI_U32 u32AppAddr,HI_U32 TnrEnable);
 //lzx

HI_S32 VPSS_REG_SetLbaEn(HI_U32 u32AppAddr,HI_U32 LbaEnable);
 //lzx
HI_S32 VPSS_REG_SetIfmdEn(HI_U32 u32AppAddr,HI_U32 IfmdEnable);
 //lzx

HI_S32 VPSS_REG_SetIglbEn(HI_U32 u32AppAddr,HI_U32 IglbEnable);
 //lzx

HI_S32 VPSS_REG_SetImgSize(HI_U32 u32AppAddr,HI_U32 u32Height,HI_U32 u32Width,HI_BOOL bProgressive);


HI_S32 VPSS_REG_SetImgAddr(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32Yaddr,HI_U32 u32Cbaddr,HI_U32 u32Craddr);

HI_S32 VPSS_REG_SetImgStride(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32YStride,HI_U32 u32CStride);



HI_S32 VPSS_REG_SetImgTile(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 EnTile);




HI_S32 VPSS_REG_SetImgOffset(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32HorOffset,HI_U32 u32VerOffset);


HI_S32 VPSS_REG_SetImgFormat(HI_U32 u32AppAddr,HI_DRV_PIX_FORMAT_E eFormat);

HI_S32 VPSS_REG_SetImgReadMod(HI_U32 u32AppAddr,HI_BOOL bField);
//lzx
HI_S32 VPSS_REG_SetFrmSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width);

HI_S32 VPSS_REG_SetFrmAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Yaddr,HI_U32 u32Caddr);

HI_S32 VPSS_REG_SetFrmStride(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32YStride,HI_U32 u32CStride);

HI_S32 VPSS_REG_SetFrmFormat(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat);
HI_S32 VPSS_REG_SetZmeEnable(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,REG_ZME_MODE_E eMode,HI_BOOL bEnable);

HI_S32 VPSS_REG_SetZmeInSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width);

HI_S32 VPSS_REG_SetZmeOutSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width);

HI_S32 VPSS_REG_SetZmeFirEnable(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_BOOL bEnable);

HI_S32 VPSS_REG_SetZmeMidEnable(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_BOOL bEnable);

HI_S32 VPSS_REG_SetZmePhase(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_S32 s32Phase);
                                    
HI_S32 VPSS_REG_SetZmeRatio(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_U32 u32Ratio);
                                    
HI_S32 VPSS_REG_SetZmeHfirOrder(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bVfirst);

HI_S32 VPSS_REG_SetZmeInFmt(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat);

HI_S32 VPSS_REG_SetZmeOutFmt(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat);

HI_S32 VPSS_REG_SetZmeCoefAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_U32 u32Addr);
                                    
                                  

HI_VOID VPSS_REG_SetDetEn(HI_U32 u32AppAddr,HI_BOOL bEnable);
 //lzx
HI_VOID VPSS_REG_SetDetMode(HI_U32 u32AppAddr,HI_U32 u32Mode);

HI_VOID VPSS_REG_SetDetBlk(HI_U32 u32AppAddr,HI_U32 blk_id, HI_U32 *pu32Addr);

HI_VOID VPSS_REG_GetDetPixel(HI_U32 u32AppAddr,HI_U32 BlkNum, HI_U8* pstData);

HI_S32 VPSS_REG_EnDei(HI_U32 u32AppAddr,HI_BOOL bEnDei);
 //lzx

HI_S32 VPSS_REG_RstDei(HI_U32 u32AppAddr,HI_BOOL bRst,HI_U32 u32RstValue);


HI_S32 VPSS_REG_SetDeiTopFirst(HI_U32 u32AppAddr,HI_BOOL bTopFirst);
 //lzx
HI_S32 VPSS_REG_SetDeiFieldMode(HI_U32 u32AppAddr,HI_BOOL bBottom);
 //lzx
HI_S32 VPSS_REG_SetDeiAddr(HI_U32 u32AppAddr,REG_FIELDPOS_E eField,HI_U32 u32YAddr,HI_U32 u32CAddr, HI_U32 u32CrAddr);

HI_S32 VPSS_REG_SetDeiStride(HI_U32 u32AppAddr,REG_FIELDPOS_E eField,HI_U32 u32YStride,HI_U32 u32CStride);

HI_S32 VPSS_REG_SetModeEn(HI_U32 u32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bEnMode);

HI_S32 VPSS_REG_SetOutSel(HI_U32 u32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bEnMode);

HI_S32 VPSS_REG_SetMode(HI_U32 u32AppAddr,REG_DIE_MODE_E eDieMode,HI_U32  u32Mode);

HI_S32 VPSS_REG_SetStInfo(HI_U32 u32AppAddr,HI_BOOL bStop);

HI_S32 VPSS_REG_SetMfMax(HI_U32 u32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bMax);

HI_S32 VPSS_REG_SetLuSceSdfMax(HI_U32 u32AppAddr,HI_BOOL bMax);

HI_S32 VPSS_REG_SetRecModeEn(HI_U32 u32AppAddr,HI_BOOL bEnMode);

HI_S32 VPSS_REG_SetMotionIIrEn(HI_U32 u32AppAddr,HI_BOOL bEnIIr);

HI_S32 VPSS_REG_SetMotionSmoothEn(HI_U32 u32AppAddr,HI_BOOL bEnSmooth);

HI_S32 VPSS_REG_SetMotionBlendEn(HI_U32 u32AppAddr,HI_BOOL bEnBlend);

HI_S32 VPSS_REG_SetChromeOffset(HI_U32 u32AppAddr,HI_U8 u8Offset);

HI_S32 VPSS_REG_SetMinIntern(HI_U32 u32AppAddr,HI_U32 u32MinIntern);

HI_S32 VPSS_REG_SetInternVer(HI_U32 u32AppAddr,HI_U32 u32InternVer);

HI_S32 VPSS_REG_SetRangeScale(HI_U32 u32AppAddr,HI_U32 u32Scale);

HI_S32 VPSS_REG_SetCK1(HI_U32 u32AppAddr,HI_U32 u32Gain,HI_U32 u32Range,HI_U32 u32Max);

HI_S32 VPSS_REG_SetCK2(HI_U32 u32AppAddr,HI_U32 u32Gain,HI_U32 u32Range,HI_U32 u32Max);

HI_S32 VPSS_REG_SetDIR(HI_U32 u32AppAddr,HI_S32 *ps32MultDir);

HI_S32 VPSS_REG_SetCcEn(HI_U32 u32AppAddr,HI_BOOL bEnCc);

HI_S32 VPSS_REG_SetCcOffset(HI_U32 u32AppAddr,HI_S32 s32Offset);

HI_S32 VPSS_REG_SetCcDetMax(HI_U32 u32AppAddr,HI_S32 s32Max);

HI_S32 VPSS_REG_SetCcDetThd(HI_U32 u32AppAddr,HI_S32 s32Thd);

HI_S32 VPSS_REG_SetSimiMax(HI_U32 u32AppAddr,HI_S32 s32SimiMax);

HI_S32 VPSS_REG_SetSimiThd(HI_U32 u32AppAddr,HI_S32 s32SimiThd);

HI_S32 VPSS_REG_SetDetBlend(HI_U32 u32AppAddr,HI_S32 s32DetBlend);

HI_S32 VPSS_REG_SetMaxXChroma(HI_U32 u32AppAddr,HI_S32 s32Max);

HI_S32 VPSS_REG_SetIntpSclRat(HI_U32 u32AppAddr,HI_S32 *ps32Rat);

HI_S32 VPSS_REG_SetStrenThd(HI_U32 u32AppAddr,HI_S32 s32Thd);

HI_S32 VPSS_REG_SetDirThd(HI_U32 u32AppAddr,HI_S32 s32Thd);

HI_S32 VPSS_REG_SetBcGain(HI_U32 u32AppAddr,HI_S32 s32BcGain);

HI_S32 VPSS_REG_EnHorEdge(HI_U32 u32AppAddr,HI_BOOL bEdgeEn);

HI_S32 VPSS_REG_EnEdgeMode(HI_U32 u32AppAddr,HI_BOOL bEdgeModeEn);

HI_S32 VPSS_REG_SetJitterCoring(HI_U32 u32AppAddr,HI_S32 s32Coring);

HI_S32 VPSS_REG_SetJitterGain(HI_U32 u32AppAddr,HI_S32 s32Gain);


HI_S32 VPSS_REG_SetMotionSlope(HI_U32 u32AppAddr,HI_S32 s32Slope);


HI_S32 VPSS_REG_SetMotionCoring(HI_U32 u32AppAddr,HI_S32 s32Coring);

HI_S32 VPSS_REG_SetMotionGain(HI_U32 u32AppAddr,HI_S32 s32Gain);

HI_S32 VPSS_REG_SetMotionHThd(HI_U32 u32AppAddr,HI_S32 s32HThd);

HI_S32 VPSS_REG_SetMotionLThd(HI_U32 u32AppAddr,HI_S32 s32LThd);


HI_S32 VPSS_REG_SetMotionDiffThd(HI_U32 u32AppAddr,HI_S32 *ps32Thd);

HI_S32 VPSS_REG_SetMotionIIrSlope(HI_U32 u32AppAddr,HI_S32 *ps32Slope);

HI_S32 VPSS_REG_SetMotionIIrRatio(HI_U32 u32AppAddr,HI_S32 *ps32Ratio);


HI_S32 VPSS_REG_SetMotionIIrRatioMaxMin(HI_U32 u32AppAddr,HI_S32 s32Max,HI_S32 s32Min);


HI_S32 VPSS_REG_SetSmoothDiffThd(HI_U32 u32AppAddr,HI_S32 *ps32Thd);

HI_S32 VPSS_REG_SetSmoothSlope(HI_U32 u32AppAddr,HI_S32 *ps32Slope);

HI_S32 VPSS_REG_SetSmoothRatio(HI_U32 u32AppAddr,HI_S32 *ps32Ratio);


HI_S32 VPSS_REG_SetSmoothRatioMaxMin(HI_U32 u32AppAddr,HI_S32 s32Max,HI_S32 s32Min);

HI_S32 VPSS_REG_SetBlendDiffThd(HI_U32 u32AppAddr,HI_S32 *ps32Thd);

HI_S32 VPSS_REG_SetBlendSlope(HI_U32 u32AppAddr,HI_S32 *ps32Slope);

HI_S32 VPSS_REG_SetBlendRatio(HI_U32 u32AppAddr,HI_S32 *ps32Ratio);

HI_S32 VPSS_REG_SetBlendRatioMaxMin(HI_U32 u32AppAddr,HI_S32 s32Max,HI_S32 s32Min);

HI_S32 VPSS_REG_SetRecFldStep(HI_U32 u32AppAddr,HI_S32 *ps32Step);

HI_S32 VPSS_REG_SetRecFrmStep(HI_U32 u32AppAddr,HI_S32 *ps32Step);

HI_S32 VPSS_REG_SetHisEn(HI_U32 u32AppAddr,HI_BOOL bEnHis);

HI_S32 VPSS_REG_SetHisInfoWrMode(HI_U32 u32AppAddr,HI_BOOL bHisMtnInfoWrMd);


HI_S32 VPSS_REG_SetHisWrMode(HI_U32 u32AppAddr,HI_BOOL bHisMtnWrMd);

HI_S32 VPSS_REG_SetHisUseMode(HI_U32 u32AppAddr,HI_BOOL bHisMtnUseMd);

HI_S32 VPSS_REG_SetMorFlt(HI_U32 u32AppAddr,HI_BOOL bEnflt,HI_S8 s8FltSize,HI_S8 s8FltThd);

HI_S32 VPSS_REG_SetMotionAdj(HI_U32 u32AppAddr,HI_S32 s32Gain,HI_S32 s32Chom,HI_S32 s32Coring);

HI_S32 VPSS_REG_SetMotionThd(HI_U32 u32AppAddr,HI_S32 s32Small,HI_S32 s32Large);

HI_S32 VPSS_REG_SetBlendEn(HI_U32 u32AppAddr,HI_BOOL bEnBlend);

HI_S32 VPSS_REG_SetHisPreEn(HI_U32 u32AppAddr,HI_BOOL bEnPre);

HI_S32 VPSS_REG_SetHisPpreEn(HI_U32 u32AppAddr,HI_BOOL bEnPpre);

HI_S32 VPSS_REG_SetCombLimit(HI_U32 u32AppAddr,HI_S32  s32UpLimit,HI_S32  s32LowLimit);

HI_S32 VPSS_REG_SetCombThd(HI_U32 u32AppAddr,HI_S32  s32Hthd,HI_S32  s32Vthd);

HI_S32 VPSS_REG_SetCombEn(HI_U32 u32AppAddr,HI_BOOL bEnComb);

HI_S32 VPSS_REG_SetCombMdThd(HI_U32 u32AppAddr,HI_S32 s32MdThd);

HI_S32 VPSS_REG_SetCombEdgeThd(HI_U32 u32AppAddr,HI_S32 s32EdgeThd);

HI_S32 VPSS_REG_SetSttWrAddr(HI_U32 u32AppAddr,HI_U32 u32SttWrAddr);

HI_S32 VPSS_REG_SetStWrAddr(HI_U32 u32AppAddr,HI_U32 u32Addr);

HI_S32 VPSS_REG_SetStRdAddr(HI_U32 u32AppAddr,HI_U32 u32Addr);

HI_S32 VPSS_REG_SetStStride(HI_U32 u32AppAddr,HI_U32 u32Stride);


HI_S32 VPSS_REG_SetDeiParaAddr(HI_U32 u32AppAddr,HI_U32 u32ParaPhyAddr);

HI_S32 VPSS_REG_GetHisBin(HI_U32 u32AppAddr,HI_S32 *pstData);

HI_S32 VPSS_REG_GetItDiff(HI_U32 u32AppAddr,HI_S32 *pstData);

HI_S32 VPSS_REG_GetPdMatch(HI_U32 u32AppAddr,
                            HI_S32 *ps32Match0,HI_S32 *ps32UnMatch0,
                            HI_S32 *ps32Match1,HI_S32 *ps32UnMatch1);

HI_S32 VPSS_REG_GetLasiCnt(HI_U32 u32AppAddr,
                            HI_S32 *ps32Cnt14,HI_S32 *ps32Cnt32,
                            HI_S32 *ps32Cnt34);
HI_S32 VPSS_REG_GetPdIchd(HI_U32 u32AppAddr,HI_S32 *pstData);

HI_S32 VPSS_REG_GetBlkSad(HI_U32 u32AppAddr,HI_S32 *pstData);

HI_S32 VPSS_REG_GetPccData(HI_U32 u32AppAddr,HI_S32 *ps32FFWD,
                             HI_S32 *ps32FWD,HI_S32 *ps32BWD,HI_S32 *ps32CRSS,
                             /*HI_S32 *ps32PW,*/HI_S32 *ps32FWDTKR,HI_S32 *ps32WDTKR);

HI_S32 VPSS_REG_SetDeiEdgeSmoothRatio(HI_U32 u32AppAddr,HI_S8 u8Data);

HI_S32 VPSS_REG_SetDeiStillBlkThd(HI_U32 u32AppAddr,HI_S8 u8Data);


HI_S32 VPSS_REG_SetDeiHistThd(HI_U32 u32AppAddr,HI_S8 *pu8Data);

HI_S32 VPSS_REG_SetDeiUmThd(HI_U32 u32AppAddr,HI_S8 *pu8Data);

HI_S32 VPSS_REG_SetDeiCoring(HI_U32 u32AppAddr,HI_S8 s8CorBlk,HI_S8 s8CorNorm,HI_S8 s8CorTkr);

HI_S32 VPSS_REG_SetDeiMovCoring(HI_U32 u32AppAddr,HI_S8 s8Blk,HI_S8 s8Norm,HI_S8 s8Tkr);

HI_S32 VPSS_REG_SetDeiPccHThd(HI_U32 u32AppAddr,HI_S8 s8Data);


HI_S32 VPSS_REG_SetDeiPccVThd(HI_U32 u32AppAddr,HI_S8 *ps8Data);

HI_S32 VPSS_REG_SetDeiItDiff(HI_U32 u32AppAddr,HI_S8 *ps8Data);

HI_S32 VPSS_REG_SetDeiLasiCtrl(HI_U32 u32AppAddr,HI_S8 s8Thr,HI_S8 s8EdgeThr,HI_S8 s8lasiThr);

HI_S32 VPSS_REG_SetDeiPdBitMove(HI_U32 u32AppAddr,HI_S32  s32Data);

HI_S32 VPSS_REG_SetDeiDirMch(HI_U32 u32AppAddr,HI_BOOL  bNext);

HI_S32 VPSS_REG_SetEdgeSmooth(HI_U32 u32AppAddr,HI_BOOL  bEdgeSmooth);


HI_S32 VPSS_REG_SetVc1En(HI_U32 u32AppAddr,HI_U32 u32EnVc1);
//lzx

HI_S32 VPSS_REG_SetVc1Profile(HI_U32 u32AppAddr,REG_FRAMEPOS_E ePos,HI_U32 u32Profile);

HI_S32 VPSS_REG_SetVc1Map(HI_U32 u32AppAddr,REG_FRAMEPOS_E ePos,HI_U32 u32MapY,HI_U32 u32MapC,
                            HI_U32 u32BMapY,HI_U32 u32BMapC);
                            
HI_S32 VPSS_REG_SetVc1MapFlag(HI_U32 u32AppAddr,REG_FRAMEPOS_E ePos,HI_U32 u32YFlag,HI_U32 u32CFlag,
                            HI_U32 u32BYFlag,HI_U32 u32BCFlag);
                            
HI_S32 VPSS_REG_SetVc1RangeEn(HI_U32 u32AppAddr,REG_FRAMEPOS_E ePos,HI_U32 u32EnVc1);

HI_S32 VPSS_REG_SetUVConvertEn(HI_U32 u32AppAddr,HI_U32 u32EnUV);

HI_S32 VPSS_REG_SetDREn(HI_U32 u32AppAddr,HI_BOOL  bEnDR);


HI_S32 VPSS_REG_SetDRDemoEn(HI_U32 u32AppAddr,HI_BOOL  bEnDRDemo);

HI_S32 VPSS_REG_SetDRPara(HI_U32 u32AppAddr,HI_S32  s32FlatThd,HI_S32  s32SimiThd,
                            HI_S32 s32AlphaScale,HI_S32 s32BetaScale);
                          
HI_S32 VPSS_REG_SetDBEn(HI_U32 u32AppAddr,HI_BOOL  bEnDB);

HI_S32 VPSS_REG_SetDBVH(HI_U32 u32AppAddr,HI_BOOL  bEnVert, HI_BOOL bEnHor);

HI_S32 VPSS_REG_SetEdgeThd(HI_U32 u32AppAddr,HI_S32  s32Thd);

HI_S32 VPSS_REG_SetVerProg(HI_U32 u32AppAddr,HI_BOOL  bProg);

HI_S32 VPSS_REG_SetThrGrad(HI_U32 u32AppAddr,HI_BOOL  bgrad);

HI_S32 VPSS_REG_SetTextEn(HI_U32 u32AppAddr,HI_BOOL  btexten);

HI_S32 VPSS_REG_SetWeakFlt(HI_U32 u32AppAddr,HI_BOOL  bWeak);

HI_S32 VPSS_REG_SetMaxDiff(HI_U32 u32AppAddr,HI_S32  s32VerMax,HI_S32  s32HorMax);

HI_S32 VPSS_REG_SetLeastDiff(HI_U32 u32AppAddr,HI_S32  s32VerLeast,HI_S32  s32HorLeast);

HI_S32 VPSS_REG_SetScale(HI_U32 u32AppAddr,HI_S32  s32Alpha,HI_S32  s32Beta);

HI_S32 VPSS_REG_SetSmoothThd(HI_U32 u32AppAddr,HI_S32  s32Thd);

HI_S32 VPSS_REG_SetQpThd(HI_U32 u32AppAddr,HI_S32  s32Thd);

HI_S32 VPSS_REG_SetPicestQp(HI_U32 u32AppAddr,HI_S32  s32Picest);

HI_S32 VPSS_REG_SetDnrInfo(HI_U32 u32AppAddr,HI_U32  u32Rcnt,HI_U32  u32Bcnt,HI_U32  u32MaxGrad,HI_U32  u32Cntrst8);

HI_S32 VPSS_REG_SetLTIEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnLTI);

HI_S32 VPSS_REG_SetLTIDemo(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnDemoLTI);

HI_S32 VPSS_REG_SetLGainRatio(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Ratio);

HI_S32 VPSS_REG_SetLGainCoef(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S8  *ps8Coef);

HI_S32 VPSS_REG_SetLMixingRatio(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Ratio);

HI_S32 VPSS_REG_SetLCoringThd(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Thd);

HI_S32 VPSS_REG_SetLSwing(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Under,HI_S32  s32Over);

HI_S32 VPSS_REG_SetLHpassCoef(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S8  *ps8Coef);

HI_S32 VPSS_REG_SetLHfreqThd(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S16  *ps16Thd);

HI_S32 VPSS_REG_SetCTIEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnCTI);

HI_S32 VPSS_REG_SetCTIDemo(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnDemoCTI);

HI_S32 VPSS_REG_SetCGainRatio(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Ratio);

HI_S32 VPSS_REG_SetCMixingRatio(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Ratio);

HI_S32 VPSS_REG_SetCCoringThd(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Thd);

HI_S32 VPSS_REG_SetCSwing(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Under,HI_S32  s32Over);

HI_S32 VPSS_REG_SetCHpassCoef(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S8  *ps8Coef);

HI_S32 VPSS_REG_SetLBAEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bEnLba);

HI_S32 VPSS_REG_SetLBABg(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Color,HI_U32 u32Alpha);

HI_S32 VPSS_REG_SetLBADispPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);

HI_S32 VPSS_REG_SetLBAVidPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);

HI_S32 VPSS_REG_SetHsclEnable(HI_U32 u32AppAddr,REG_ZME_MODE_E eMode,HI_BOOL bEnable);


HI_S32 VPSS_REG_SetHsclOutSize(HI_U32 u32AppAddr,HI_U32 u32Width);


HI_S32 VPSS_REG_SetHsclFirEnable(HI_U32 u32AppAddr,REG_ZME_MODE_E eMode,HI_BOOL bEnable);

HI_S32 VPSS_REG_SetHsclMidEnable(HI_U32 u32AppAddr,REG_ZME_MODE_E eMode,HI_BOOL bEnable);

HI_S32 VPSS_REG_SetHsclPhase(HI_U32 u32AppAddr,REG_ZME_MODE_E eMode,HI_S32 s32Phase);
                                    
HI_S32 VPSS_REG_SetHsclRatio(HI_U32 u32AppAddr,HI_U32 u32Ratio);
         
HI_S32 VPSS_REG_SetHsclCoefAddr(HI_U32 u32AppAddr,REG_ZME_MODE_E eMode,HI_U32 u32Addr);
                                    

HI_S32 VPSS_REG_SetOutCropVidPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                            HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);
                           

HI_S32 VPSS_REG_SetVhdCmpEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnVhdCmp);



HI_S32 VPSS_REG_SetVhdCmpAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_U32 u32YHeadaddr,HI_U32 u32CHeadaddr);


HI_S32 VPSS_REG_SetVhdCmpDrr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_U32 u32VhdCmpDrr);


HI_S32 VPSS_REG_SetVhdCmpLossyEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL  bEnVhdCmpLossy);


HI_S32 VPSS_REG_SetDcmpEn(HI_U32 u32AppAddr,HI_BOOL  bEnDcmp);


HI_S32 VPSS_REG_SetDcmpHeadAddr(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32YHeadaddr,HI_U32 u32CHeadaddr);


HI_S32 VPSS_REG_SetTunlEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnTunl);


HI_S32 VPSS_REG_SetTunlFinishLine(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32FinishLine);


HI_S32 VPSS_REG_SetTunlMode(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, REG_TUNLPOS_E  s32TunlMode);


HI_S32 VPSS_REG_SetTunlAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32TunlAddr);


HI_S32 VPSS_REG_SetCurTunlAddr(HI_U32 u32AppAddr,REG_FRAMEPOS_E  ePort,HI_U32 u32TunlAddr);


HI_S32 VPSS_REG_SetCurTunlEn(HI_U32 u32AppAddr,HI_BOOL u32CurTunlEn);


HI_S32 VPSS_REG_SetCurTunlInterval(HI_U32 u32AppAddr,REG_FRAMEPOS_E ePort,HI_S32  s32CurTunlInterval);

HI_S32 VPSS_REG_SetCscEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnCSC);

HI_S32 VPSS_REG_SetCscIdc(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscIdc0,HI_U32 u32CscIdc1,HI_U32 u32CscIdc2);

HI_S32 VPSS_REG_SetCscOdc(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscOdc0,HI_U32 u32CscOdc1,HI_U32 u32CscOdc2);

HI_S32 VPSS_REG_SetCscP00(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP00);


HI_S32 VPSS_REG_SetCscP01(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP01);


HI_S32 VPSS_REG_SetCscP02(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP02);


HI_S32 VPSS_REG_SetCscP10(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP10);

HI_S32 VPSS_REG_SetCscP11(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP11);

HI_S32 VPSS_REG_SetCscP12(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP12);

HI_S32 VPSS_REG_SetCscP20(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP20);

HI_S32 VPSS_REG_SetCscP21(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP21);

HI_S32 VPSS_REG_SetCscP22(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP22);



HI_S32 VPSS_REG_SetInCropPos(HI_U32 u32AppAddr,HI_U32 u32InCropY,HI_U32 u32InCropX);



HI_S32 VPSS_REG_SetInCropEn(HI_U32 u32AppAddr,HI_BOOL bInCropEn);


HI_S32 VPSS_REG_SetCurInCropEn(HI_U32 u32AppAddr,HI_BOOL bCurInCropEn);
//lzx

HI_S32 VPSS_REG_SetRefInCropEn(HI_U32 u32AppAddr,HI_BOOL bRefInCropEn);
//lzx

HI_S32 VPSS_REG_SetNxt1InCropEn(HI_U32 u32AppAddr,HI_BOOL bNxt1InCropEn);
//lzx

HI_S32 VPSS_REG_SetNxt2InCropEn(HI_U32 u32AppAddr,HI_BOOL bNxt2InCropEn);
 //lzx
 

HI_S32 VPSS_REG_SetInCropMode(HI_U32 u32AppAddr,HI_BOOL bInCropMode);



HI_S32 VPSS_REG_SetInCropSize(HI_U32 u32AppAddr,HI_U32 u32InCropHeight,HI_U32 u32InCropWidth);



HI_S32 VPSS_REG_SetPortCropPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CropY,HI_U32 u32CropX);


HI_S32 VPSS_REG_SetPortCropSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CropHeight,HI_U32 u32CropWidth);


HI_S32 VPSS_REG_SetPortCropEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bPortCropEn);


HI_S32 VPSS_REG_SetPortMirrorEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bMirrorEn);


HI_S32 VPSS_REG_SetPortFlipEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bFlipEn);

HI_S32 VPSS_REG_SetPreHfirEn(HI_U32 u32AppAddr,HI_BOOL bHfirEn);
 //lzx


HI_S32 VPSS_REG_SetPreHfirMode(HI_U32 u32AppAddr, HI_U32 u32HfirMode);

//lzx

HI_S32 VPSS_REG_SetPreVfirEn(HI_U32 u32AppAddr,HI_BOOL bVfirEn);
 //lzx

HI_S32 VPSS_REG_SetPreVfirMode(HI_U32 u32AppAddr,HI_U32 u32VfirMode);
  //lzx

HI_S32 VPSS_REG_SetRotation(HI_U32 u32AppAddr,HI_U32 u32Angle, HI_U32 u32ProMode);

HI_S32 VPSS_REG_GetReg(HI_U32 u32AppAddr,HI_U32 *pu32Int);

HI_S32 VPSS_REG_SetFidelity(HI_U32 u32AppVAddr,HI_BOOL bEnFidelity);

HI_S32 VPSS_REG_SetStMode(HI_U32 u32AppVAddr,HI_BOOL bLumaMax,HI_BOOL bChromaMax);


HI_S32 VPSS_REG_SetPreZme(HI_U32 u32AppVAddr,
                            VPSS_REG_PREZME_E enHor,VPSS_REG_PREZME_E enVer);
HI_S32 VPSS_REG_SetVc1En(HI_U32 u32AppAddr,HI_U32 u32EnVc1);



#endif /* __C_UNION_DEFINE_VPSS_MV410_H__ */
