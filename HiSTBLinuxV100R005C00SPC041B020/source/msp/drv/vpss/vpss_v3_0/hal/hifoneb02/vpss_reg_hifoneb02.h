//****************************************************************************** 
// Copyright     :  Copyright (C) 2014, Hisilicon Technologies Co., Ltd.
// File name     :  c_union_define.h
// Author        :  x57490
// Version       :  1.0 
// Date          :  2014-04-07
// Description   :  Define all registers/tables for S40V200
// Others        :  Generated automatically by nManager V4.0 
// History       :  x57490 2014-04-07 Create file
//******************************************************************************

#ifndef __VPSS_REG_HIFONEB02__
#define __VPSS_REG_HIFONEB02__

#include "vpss_common.h"
#include "vpss_reg_struct.h"
#include "hi_drv_reg.h"
#include "hi_reg_common.h"

//CV200
#define VPSS_BASE_ADDR  0xf8cb0000    
#if 0

/* Define the union U_VPSS_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    vsd_en                : 1   ; /* [1]  */
        unsigned int    reserved_1            : 1   ; /* [2]  */
        unsigned int    vhd0_en               : 1   ; /* [3]  */
        unsigned int    reserved_2            : 1   ; /* [4]  */
        unsigned int    vc1_en                : 1   ; /* [5]  */
        unsigned int    chk_sum_en            : 1   ; /* [6]  */
        unsigned int    dei_en                : 1   ; /* [7]  */
        unsigned int    hsp_en                : 1   ; /* [8]  */
        unsigned int    dnr_en                : 1   ; /* [9]  */
        unsigned int    db_en                 : 1   ; /* [10]  */
        unsigned int    dr_en                 : 1   ; /* [11]  */
        unsigned int    two_pix_en            : 1   ; /* [12]  */
        unsigned int    str_det_en            : 1   ; /* [13]  */
        unsigned int    reserved_3            : 7   ; /* [20..14]  */
        unsigned int    rotate_angle          : 1   ; /* [21]  */
        unsigned int    rotate_en             : 1   ; /* [22]  */
        unsigned int    prot                  : 2   ; /* [24..23]  */
        unsigned int    ifmd_en               : 1   ; /* [25]  */
        unsigned int    iglb_en               : 1   ; /* [26]  */
        unsigned int    img_pro_mode          : 2   ; /* [28..27]  */
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
        unsigned int    vsd_format            : 4   ; /* [7..4]  */
        unsigned int    reserved_1            : 4   ; /* [11..8]  */
        unsigned int    vhd0_format           : 4   ; /* [15..12]  */
        unsigned int    in_format             : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 3   ; /* [23..21]  */
        unsigned int    in_pix_bitw           : 1   ; /* [24]  */
        unsigned int    uv_invert             : 1   ; /* [25]  */
        unsigned int    reserved_3            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CTRL2;

/* Define the union U_VPSS_CTRL3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    zme_vsd_en            : 1   ; /* [1]  */
        unsigned int    reserved_1            : 1   ; /* [2]  */
        unsigned int    zme_vhd0_en           : 1   ; /* [3]  */
        unsigned int    vhd0_pre_vfir_mode    : 2   ; /* [5..4]  */
        unsigned int    vhd0_pre_vfir_en      : 1   ; /* [6]  */
        unsigned int    vhd0_pre_hfir_mode    : 2   ; /* [8..7]  */
        unsigned int    vhd0_pre_hfir_en      : 1   ; /* [9]  */
        unsigned int    vsd_pre_vfir_mode     : 2   ; /* [11..10]  */
        unsigned int    vsd_pre_vfir_en       : 1   ; /* [12]  */
        unsigned int    vsd_pre_hfir_mode     : 2   ; /* [14..13]  */
        unsigned int    vsd_pre_hfir_en       : 1   ; /* [15]  */
        unsigned int    in_crop_en            : 1   ; /* [16]  */
        unsigned int    vhd0_crop_en          : 1   ; /* [17]  */
        unsigned int    vsd_crop_en           : 1   ; /* [18]  */
        unsigned int    vsd_lba_en            : 1   ; /* [19]  */
        unsigned int    vhd0_lba_en           : 1   ; /* [20]  */
        unsigned int    reserved_2            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CTRL3;

/* Define the union U_VPSS_IMGSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    imgwidth              : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    imgheight             : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_IMGSIZE;

/* Define the union U_VPSS_INCROP_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    in_crop_x             : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    in_crop_y             : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
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
        unsigned int    in_crop_width         : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    in_crop_height        : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
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
        unsigned int    vhd0_crop_x           : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vhd0_crop_y           : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
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
        unsigned int    vhd0_crop_width       : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vhd0_crop_height      : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0CROP_SIZE;

/* Define the union U_VPSS_VSDCROP_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsd_crop_x            : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vsd_crop_y            : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VSDCROP_POS;

/* Define the union U_VPSS_VSDCROP_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsd_crop_width        : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vsd_crop_height       : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VSDCROP_SIZE;

/* Define the union U_VPSS_VHD0_ZMEORESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0_zme_ow           : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vhd0_zme_oh           : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_ZMEORESO;

/* Define the union U_VPSS_VSD_ZMEORESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsd_zme_ow            : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vsd_zme_oh            : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VSD_ZMEORESO;

/* Define the union U_VPSS_VHD0LBA_DSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0_lba_width        : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vhd0_lba_height       : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
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
        unsigned int    vhd0_lba_xfpos        : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vhd0_lba_yfpos        : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0LBA_VFPOS;

/* Define the union U_VPSS_VSDLBA_DSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsd_lba_width         : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vsd_lba_height        : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VSDLBA_DSIZE;

/* Define the union U_VPSS_VSDLBA_VFPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsd_lba_xfpos         : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vsd_lba_yfpos         : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VSDLBA_VFPOS;

/* Define the union U_VPSS_VHD0LBA_BK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vbk_cr                : 10  ; /* [9..0]  */
        unsigned int    vbk_cb                : 10  ; /* [19..10]  */
        unsigned int    vbk_y                 : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0LBA_BK;

/* Define the union U_VPSS_VSDLBA_BK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vbk_cr                : 10  ; /* [9..0]  */
        unsigned int    vbk_cb                : 10  ; /* [19..10]  */
        unsigned int    vbk_y                 : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VSDLBA_BK;

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
        unsigned int    dnr_dither_en         : 1   ; /* [24]  */
        unsigned int    dnr_dither_mode       : 1   ; /* [25]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DNR_INFO;

/* Define the union U_VPSS_AXI_RID0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cury_rid              : 4   ; /* [3..0]  */
        unsigned int    curc_rid              : 4   ; /* [7..4]  */
        unsigned int    refy_rid              : 4   ; /* [11..8]  */
        unsigned int    refc_rid              : 4   ; /* [15..12]  */
        unsigned int    nx1y_rid              : 4   ; /* [19..16]  */
        unsigned int    nx1c_rid              : 4   ; /* [23..20]  */
        unsigned int    nx2y_rid              : 4   ; /* [27..24]  */
        unsigned int    nx2c_rid              : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_AXI_RID0;

/* Define the union U_VPSS_AXI_RID1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rst_rid               : 4   ; /* [3..0]  */
        unsigned int    cas_rid               : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_AXI_RID1;

/* Define the union U_VPSS_AXIID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 20  ; /* [19..0]  */
        unsigned int    awid_cfg0             : 4   ; /* [23..20]  */
        unsigned int    reserved_1            : 4   ; /* [27..24]  */
        unsigned int    arid_cfg0             : 4   ; /* [31..28]  */
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
        unsigned int    bus_r_err_mask        : 1   ; /* [2]  */
        unsigned int    eof_end_mask          : 1   ; /* [3]  */
        unsigned int    bus_w_err_mask        : 1   ; /* [4]  */
        unsigned int    vhd0_tunl_mask        : 1   ; /* [5]  */
        unsigned int    vsd_tunl_mask         : 1   ; /* [6]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_INTMASK;

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

/* Define the union U_VPSS_CURSTRIDE_LB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cury_stride_lb        : 16  ; /* [15..0]  */
        unsigned int    curc_stride_lb        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CURSTRIDE_LB;

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

/* Define the union U_VPSS_REFSTRIDE_LB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    refy_stride_lb        : 16  ; /* [15..0]  */
        unsigned int    refc_stride_lb        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_REFSTRIDE_LB;

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

/* Define the union U_VPSS_NXT1STRIDE_LB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nxt1y_stride_lb       : 16  ; /* [15..0]  */
        unsigned int    nxt1c_stride_lb       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NXT1STRIDE_LB;

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

/* Define the union U_VPSS_NXT2STRIDE_LB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nxt2y_stride_lb       : 16  ; /* [15..0]  */
        unsigned int    nxt2c_stride_lb       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NXT2STRIDE_LB;

/* Define the union U_VPSS_VHD0CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    vhd0_mirror           : 1   ; /* [4]  */
        unsigned int    vhd0_flip             : 1   ; /* [5]  */
        unsigned int    vhd0_pix_bitw         : 1   ; /* [6]  */
        unsigned int    vhd0_dither_en        : 1   ; /* [7]  */
        unsigned int    vhd0_uv_invert        : 1   ; /* [8]  */
        unsigned int    vhd0_lcmp_en          : 1   ; /* [9]  */
        unsigned int    vhd0_dither_mode      : 1   ; /* [10]  */
        unsigned int    reserved_1            : 21  ; /* [31..11]  */
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

/* Define the union U_VPSS_VSDCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    vsd_mirror            : 1   ; /* [4]  */
        unsigned int    vsd_flip              : 1   ; /* [5]  */
        unsigned int    vsd_pix_bitw          : 1   ; /* [6]  */
        unsigned int    vsd_dither_en         : 1   ; /* [7]  */
        unsigned int    vsd_uv_invert         : 1   ; /* [8]  */
        unsigned int    reserved_1            : 1   ; /* [9]  */
        unsigned int    vsd_dither_mode       : 1   ; /* [10]  */
        unsigned int    reserved_2            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VSDCTRL;

/* Define the union U_VPSS_VSDSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsd_width             : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vsd_height            : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VSDSIZE;

/* Define the union U_VPSS_VSDSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsdy_stride           : 16  ; /* [15..0]  */
        unsigned int    vsdc_stride           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VSDSTRIDE;

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

/* Define the union U_VPSS_TUNL_CTRL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0_tunl_finish_line : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 16  ; /* [27..12]  */
        unsigned int    vhd0_tunl_en          : 1   ; /* [28]  */
        unsigned int    reserved_1            : 1   ; /* [29]  */
        unsigned int    vsd_tunl_en           : 1   ; /* [30]  */
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
        unsigned int    vsd_tunl_finish_line  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    cur_tunl_rd_interval  : 8   ; /* [23..16]  */
        unsigned int    vhd0_tunl_mode        : 2   ; /* [25..24]  */
        unsigned int    reserved_1            : 2   ; /* [27..26]  */
        unsigned int    vsd_tunl_mode         : 2   ; /* [29..28]  */
        unsigned int    reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TUNL_CTRL1;

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
        unsigned int    bus_r_err             : 1   ; /* [2]  */
        unsigned int    eof_end_state         : 1   ; /* [3]  */
        unsigned int    bus_w_err             : 1   ; /* [4]  */
        unsigned int    vhd0_tunl_state       : 1   ; /* [5]  */
        unsigned int    vsd_tunl_state        : 1   ; /* [6]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
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
        unsigned int    bus_r_err_clr         : 1   ; /* [2]  */
        unsigned int    eof_end_clr           : 1   ; /* [3]  */
        unsigned int    bus_w_err_clr         : 1   ; /* [4]  */
        unsigned int    vhd0_tunl_clr         : 1   ; /* [5]  */
        unsigned int    vsd_tunl_clr          : 1   ; /* [6]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
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
        unsigned int    raw_bus_r_err         : 1   ; /* [2]  */
        unsigned int    raw_eof_end           : 1   ; /* [3]  */
        unsigned int    raw_bus_w_err         : 1   ; /* [4]  */
        unsigned int    raw_vhd0_tunl_state   : 1   ; /* [5]  */
        unsigned int    raw_vsd_tunl_state    : 1   ; /* [6]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_RAWINT;

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

/* Define the union U_VPSS_VHD0_HSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hratio                : 24  ; /* [23..0]  */
        unsigned int    hfir_order            : 1   ; /* [24]  */
        unsigned int    hchfir_en             : 1   ; /* [25]  */
        unsigned int    hlfir_en              : 1   ; /* [26]  */
        unsigned int    reserved_0            : 1   ; /* [27]  */
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
        unsigned int    reserved_1            : 3   ; /* [27..25]  */
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

/* Define the union U_VPSS_VSD_HSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hratio                : 24  ; /* [23..0]  */
        unsigned int    hfir_order            : 1   ; /* [24]  */
        unsigned int    hchfir_en             : 1   ; /* [25]  */
        unsigned int    hlfir_en              : 1   ; /* [26]  */
        unsigned int    reserved_0            : 1   ; /* [27]  */
        unsigned int    hchmid_en             : 1   ; /* [28]  */
        unsigned int    hlmid_en              : 1   ; /* [29]  */
        unsigned int    hchmsc_en             : 1   ; /* [30]  */
        unsigned int    hlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VSD_HSP;

/* Define the union U_VPSS_VSD_HLOFFSET */
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

} U_VPSS_VSD_HLOFFSET;

/* Define the union U_VPSS_VSD_HCOFFSET */
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

} U_VPSS_VSD_HCOFFSET;

/* Define the union U_VPSS_VSD_VSP */
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
        unsigned int    reserved_1            : 3   ; /* [27..25]  */
        unsigned int    vchmid_en             : 1   ; /* [28]  */
        unsigned int    vlmid_en              : 1   ; /* [29]  */
        unsigned int    vchmsc_en             : 1   ; /* [30]  */
        unsigned int    vlmsc_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VSD_VSP;

/* Define the union U_VPSS_VSD_VSR */
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

} U_VPSS_VSD_VSR;

/* Define the union U_VPSS_VSD_VOFFSET */
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

} U_VPSS_VSD_VOFFSET;

/* Define the union U_VPSS_HSPCFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsp_hf0_tmp0          : 8   ; /* [7..0]  */
        unsigned int    hsp_hf0_tmp1          : 8   ; /* [15..8]  */
        unsigned int    hsp_hf0_tmp2          : 8   ; /* [23..16]  */
        unsigned int    hsp_hf0_tmp3          : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSPCFG1;

/* Define the union U_VPSS_HSPCFG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsp_hf0_coring        : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSPCFG2;

/* Define the union U_VPSS_HSPCFG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsp_hf0_gainpos       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    hsp_hf0_gainneg       : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSPCFG3;

/* Define the union U_VPSS_HSPCFG4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsp_hf0_overth        : 8   ; /* [7..0]  */
        unsigned int    hsp_hf0_underth       : 8   ; /* [15..8]  */
        unsigned int    hsp_hf0_mixratio      : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 4   ; /* [27..24]  */
        unsigned int    hsp_hf0_winsize       : 3   ; /* [30..28]  */
        unsigned int    hsp_hf0_adpshoot_en   : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSPCFG4;

/* Define the union U_VPSS_HSPCFG5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsp_hf1_tmp0          : 8   ; /* [7..0]  */
        unsigned int    hsp_hf1_tmp1          : 8   ; /* [15..8]  */
        unsigned int    hsp_hf1_tmp2          : 8   ; /* [23..16]  */
        unsigned int    hsp_hf1_tmp3          : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSPCFG5;

/* Define the union U_VPSS_HSPCFG6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsp_hf1_coring        : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSPCFG6;

/* Define the union U_VPSS_HSPCFG7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsp_hf1_gainpos       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    hsp_hf1_gainneg       : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSPCFG7;

/* Define the union U_VPSS_HSPCFG8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsp_hf1_overth        : 8   ; /* [7..0]  */
        unsigned int    hsp_hf1_underth       : 8   ; /* [15..8]  */
        unsigned int    hsp_hf1_mixratio      : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 4   ; /* [27..24]  */
        unsigned int    hsp_hf1_winsize       : 3   ; /* [30..28]  */
        unsigned int    hsp_hf1_adpshoot_en   : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSPCFG8;

/* Define the union U_VPSS_HSPCFG9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsp_cdti_gain         : 8   ; /* [7..0]  */
        unsigned int    hsp_ldti_gain         : 8   ; /* [15..8]  */
        unsigned int    hsp_lti_ratio         : 8   ; /* [23..16]  */
        unsigned int    hsp_hf_shootdiv       : 3   ; /* [26..24]  */
        unsigned int    reserved_0            : 1   ; /* [27]  */
        unsigned int    hsp_ctih_en           : 1   ; /* [28]  */
        unsigned int    hsp_ltih_en           : 1   ; /* [29]  */
        unsigned int    hsp_h1_en             : 1   ; /* [30]  */
        unsigned int    hsp_h0_en             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSPCFG9;

/* Define the union U_VPSS_HSPCFG10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsp_glb_underth       : 9   ; /* [8..0]  */
        unsigned int    hsp_mk_en             : 1   ; /* [9]  */
        unsigned int    hsp_glb_overth        : 9   ; /* [18..10]  */
        unsigned int    hsp_mk_mode           : 1   ; /* [19]  */
        unsigned int    hsp_peak_ratio        : 8   ; /* [27..20]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_HSPCFG10;

/* Define the union U_VPSS_DR_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    enep                  : 1   ; /* [0]  */
        unsigned int    enovt                 : 4   ; /* [4..1]  */
        unsigned int    enstradjstbylm        : 1   ; /* [5]  */
        unsigned int    enangadjstbylm        : 1   ; /* [6]  */
        unsigned int    clipmode              : 1   ; /* [7]  */
        unsigned int    demo_en               : 1   ; /* [8]  */
        unsigned int    demo_mode             : 1   ; /* [9]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_CTRL;

/* Define the union U_VPSS_DR_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    regthrmdfangctrst     : 4   ; /* [3..0]  */
        unsigned int    regthrovshtminang     : 4   ; /* [7..4]  */
        unsigned int    regdrstrenth          : 5   ; /* [12..8]  */
        unsigned int    regovtstrenth         : 3   ; /* [15..13]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR;

/* Define the union U_VPSS_DRLSWSTRLUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lswstrlut00           : 5   ; /* [4..0]  */
        unsigned int    lswstrlut10           : 5   ; /* [9..5]  */
        unsigned int    lswstrlut20           : 5   ; /* [14..10]  */
        unsigned int    lswstrlut30           : 5   ; /* [19..15]  */
        unsigned int    lswstrlut40           : 5   ; /* [24..20]  */
        unsigned int    lswstrlut50           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLSWSTRLUT0;

/* Define the union U_VPSS_DRLSWSTRLUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lswstrlut01           : 5   ; /* [4..0]  */
        unsigned int    lswstrlut11           : 5   ; /* [9..5]  */
        unsigned int    lswstrlut21           : 5   ; /* [14..10]  */
        unsigned int    lswstrlut31           : 5   ; /* [19..15]  */
        unsigned int    lswstrlut41           : 5   ; /* [24..20]  */
        unsigned int    lswstrlut51           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLSWSTRLUT1;

/* Define the union U_VPSS_DRLSWSTRLUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lswstrlut02           : 5   ; /* [4..0]  */
        unsigned int    lswstrlut12           : 5   ; /* [9..5]  */
        unsigned int    lswstrlut22           : 5   ; /* [14..10]  */
        unsigned int    lswstrlut32           : 5   ; /* [19..15]  */
        unsigned int    lswstrlut42           : 5   ; /* [24..20]  */
        unsigned int    lswstrlut52           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLSWSTRLUT2;

/* Define the union U_VPSS_DRLSWSTRLUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lswstrlut03           : 5   ; /* [4..0]  */
        unsigned int    lswstrlut13           : 5   ; /* [9..5]  */
        unsigned int    lswstrlut23           : 5   ; /* [14..10]  */
        unsigned int    lswstrlut33           : 5   ; /* [19..15]  */
        unsigned int    lswstrlut43           : 5   ; /* [24..20]  */
        unsigned int    lswstrlut53           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLSWSTRLUT3;

/* Define the union U_VPSS_DRLSWSTRLUT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lswstrlut04           : 5   ; /* [4..0]  */
        unsigned int    lswstrlut14           : 5   ; /* [9..5]  */
        unsigned int    lswstrlut24           : 5   ; /* [14..10]  */
        unsigned int    lswstrlut34           : 5   ; /* [19..15]  */
        unsigned int    lswstrlut44           : 5   ; /* [24..20]  */
        unsigned int    lswstrlut54           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLSWSTRLUT4;

/* Define the union U_VPSS_DRLSWSTRLUT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lswstrlut05           : 5   ; /* [4..0]  */
        unsigned int    lswstrlut15           : 5   ; /* [9..5]  */
        unsigned int    lswstrlut25           : 5   ; /* [14..10]  */
        unsigned int    lswstrlut35           : 5   ; /* [19..15]  */
        unsigned int    lswstrlut45           : 5   ; /* [24..20]  */
        unsigned int    lswstrlut55           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLSWSTRLUT5;

/* Define the union U_VPSS_DRLSWSTRLUT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lswstrlut06           : 5   ; /* [4..0]  */
        unsigned int    lswstrlut16           : 5   ; /* [9..5]  */
        unsigned int    lswstrlut26           : 5   ; /* [14..10]  */
        unsigned int    lswstrlut36           : 5   ; /* [19..15]  */
        unsigned int    lswstrlut46           : 5   ; /* [24..20]  */
        unsigned int    lswstrlut56           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLSWSTRLUT6;

/* Define the union U_VPSS_DRLSWSTRLUT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lswstrlut07           : 5   ; /* [4..0]  */
        unsigned int    lswstrlut17           : 5   ; /* [9..5]  */
        unsigned int    lswstrlut27           : 5   ; /* [14..10]  */
        unsigned int    lswstrlut37           : 5   ; /* [19..15]  */
        unsigned int    lswstrlut47           : 5   ; /* [24..20]  */
        unsigned int    lswstrlut57           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLSWSTRLUT7;

/* Define the union U_VPSS_DRLSWSTRLUT8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lswstrlut08           : 5   ; /* [4..0]  */
        unsigned int    lswstrlut18           : 5   ; /* [9..5]  */
        unsigned int    lswstrlut28           : 5   ; /* [14..10]  */
        unsigned int    lswstrlut38           : 5   ; /* [19..15]  */
        unsigned int    lswstrlut48           : 5   ; /* [24..20]  */
        unsigned int    lswstrlut58           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLSWSTRLUT8;

/* Define the union U_VPSS_DRLRWSTRLUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lrwstrlut00           : 5   ; /* [4..0]  */
        unsigned int    lrwstrlut10           : 5   ; /* [9..5]  */
        unsigned int    lrwstrlut20           : 5   ; /* [14..10]  */
        unsigned int    lrwstrlut30           : 5   ; /* [19..15]  */
        unsigned int    lrwstrlut40           : 5   ; /* [24..20]  */
        unsigned int    lrwstrlut50           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLRWSTRLUT0;

/* Define the union U_VPSS_DRLRWSTRLUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lrwstrlut01           : 5   ; /* [4..0]  */
        unsigned int    lrwstrlut11           : 5   ; /* [9..5]  */
        unsigned int    lrwstrlut21           : 5   ; /* [14..10]  */
        unsigned int    lrwstrlut31           : 5   ; /* [19..15]  */
        unsigned int    lrwstrlut41           : 5   ; /* [24..20]  */
        unsigned int    lrwstrlut51           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLRWSTRLUT1;

/* Define the union U_VPSS_DRLRWSTRLUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lrwstrlut02           : 5   ; /* [4..0]  */
        unsigned int    lrwstrlut12           : 5   ; /* [9..5]  */
        unsigned int    lrwstrlut22           : 5   ; /* [14..10]  */
        unsigned int    lrwstrlut32           : 5   ; /* [19..15]  */
        unsigned int    lrwstrlut42           : 5   ; /* [24..20]  */
        unsigned int    lrwstrlut52           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLRWSTRLUT2;

/* Define the union U_VPSS_DRLRWSTRLUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lrwstrlut03           : 5   ; /* [4..0]  */
        unsigned int    lrwstrlut13           : 5   ; /* [9..5]  */
        unsigned int    lrwstrlut23           : 5   ; /* [14..10]  */
        unsigned int    lrwstrlut33           : 5   ; /* [19..15]  */
        unsigned int    lrwstrlut43           : 5   ; /* [24..20]  */
        unsigned int    lrwstrlut53           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLRWSTRLUT3;

/* Define the union U_VPSS_DRLRWSTRLUT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lrwstrlut04           : 5   ; /* [4..0]  */
        unsigned int    lrwstrlut14           : 5   ; /* [9..5]  */
        unsigned int    lrwstrlut24           : 5   ; /* [14..10]  */
        unsigned int    lrwstrlut34           : 5   ; /* [19..15]  */
        unsigned int    lrwstrlut44           : 5   ; /* [24..20]  */
        unsigned int    lrwstrlut54           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLRWSTRLUT4;

/* Define the union U_VPSS_DRLRWSTRLUT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lrwstrlut05           : 5   ; /* [4..0]  */
        unsigned int    lrwstrlut15           : 5   ; /* [9..5]  */
        unsigned int    lrwstrlut25           : 5   ; /* [14..10]  */
        unsigned int    lrwstrlut35           : 5   ; /* [19..15]  */
        unsigned int    lrwstrlut45           : 5   ; /* [24..20]  */
        unsigned int    lrwstrlut55           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLRWSTRLUT5;

/* Define the union U_VPSS_DRLRWSTRLUT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lrwstrlut06           : 5   ; /* [4..0]  */
        unsigned int    lrwstrlut16           : 5   ; /* [9..5]  */
        unsigned int    lrwstrlut26           : 5   ; /* [14..10]  */
        unsigned int    lrwstrlut36           : 5   ; /* [19..15]  */
        unsigned int    lrwstrlut46           : 5   ; /* [24..20]  */
        unsigned int    lrwstrlut56           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLRWSTRLUT6;

/* Define the union U_VPSS_DRLRWSTRLUT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lrwstrlut07           : 5   ; /* [4..0]  */
        unsigned int    lrwstrlut17           : 5   ; /* [9..5]  */
        unsigned int    lrwstrlut27           : 5   ; /* [14..10]  */
        unsigned int    lrwstrlut37           : 5   ; /* [19..15]  */
        unsigned int    lrwstrlut47           : 5   ; /* [24..20]  */
        unsigned int    lrwstrlut57           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLRWSTRLUT7;

/* Define the union U_VPSS_DRLRWSTRLUT8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lrwstrlut08           : 5   ; /* [4..0]  */
        unsigned int    lrwstrlut18           : 5   ; /* [9..5]  */
        unsigned int    lrwstrlut28           : 5   ; /* [14..10]  */
        unsigned int    lrwstrlut38           : 5   ; /* [19..15]  */
        unsigned int    lrwstrlut48           : 5   ; /* [24..20]  */
        unsigned int    lrwstrlut58           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLRWSTRLUT8;

/* Define the union U_VPSS_DRLSWSTRADJUSTLUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_lswadjlut0         : 4   ; /* [3..0]  */
        unsigned int    dr_lswadjlut1         : 4   ; /* [7..4]  */
        unsigned int    dr_lswadjlut2         : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLSWSTRADJUSTLUT0;

/* Define the union U_VPSS_DRLSWSTRADJUSTLUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_lswadjlut3         : 4   ; /* [3..0]  */
        unsigned int    dr_lswadjlut4         : 4   ; /* [7..4]  */
        unsigned int    dr_lswadjlut5         : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLSWSTRADJUSTLUT1;

/* Define the union U_VPSS_DRLSWSTRADJUSTLUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_lswadjlut6         : 4   ; /* [3..0]  */
        unsigned int    dr_lswadjlut7         : 4   ; /* [7..4]  */
        unsigned int    dr_lswadjlut8         : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLSWSTRADJUSTLUT2;

/* Define the union U_VPSS_DRLRWSTRADJUSTLUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_lrwadjlut0         : 4   ; /* [3..0]  */
        unsigned int    dr_lrwadjlut1         : 4   ; /* [7..4]  */
        unsigned int    dr_lrwadjlut2         : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLRWSTRADJUSTLUT0;

/* Define the union U_VPSS_DRLRWSTRADJUSTLUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_lrwadjlut3         : 4   ; /* [3..0]  */
        unsigned int    dr_lrwadjlut4         : 4   ; /* [7..4]  */
        unsigned int    dr_lrwadjlut5         : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLRWSTRADJUSTLUT1;

/* Define the union U_VPSS_DRLRWSTRADJUSTLUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_lrwadjlut6         : 4   ; /* [3..0]  */
        unsigned int    dr_lrwadjlut7         : 4   ; /* [7..4]  */
        unsigned int    dr_lrwadjlut8         : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRLRWSTRADJUSTLUT2;

/* Define the union U_VPSS_DRANGADJSUTLUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_angadjlut0         : 4   ; /* [3..0]  */
        unsigned int    dr_angadjlut1         : 4   ; /* [7..4]  */
        unsigned int    dr_angadjlut2         : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRANGADJSUTLUT0;

/* Define the union U_VPSS_DRANGADJSUTLUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_angadjlut3         : 4   ; /* [3..0]  */
        unsigned int    dr_angadjlut4         : 4   ; /* [7..4]  */
        unsigned int    dr_angadjlut5         : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRANGADJSUTLUT1;

/* Define the union U_VPSS_DRANGADJSUTLUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_angadjlut6         : 4   ; /* [3..0]  */
        unsigned int    dr_angadjlut7         : 4   ; /* [7..4]  */
        unsigned int    dr_angadjlut8         : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRANGADJSUTLUT2;

/* Define the union U_VPSS_DRANGLESTRLUT1_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_angdifflut0        : 5   ; /* [4..0]  */
        unsigned int    dr_angdifflut1        : 5   ; /* [9..5]  */
        unsigned int    dr_angdifflut2        : 5   ; /* [14..10]  */
        unsigned int    dr_angdifflut3        : 5   ; /* [19..15]  */
        unsigned int    dr_angdifflut4        : 5   ; /* [24..20]  */
        unsigned int    dr_angdifflut5        : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRANGLESTRLUT1_0;

/* Define the union U_VPSS_DRANGLESTRLUT1_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_angdifflut6        : 5   ; /* [4..0]  */
        unsigned int    dr_angdifflut7        : 5   ; /* [9..5]  */
        unsigned int    dr_angdifflut8        : 5   ; /* [14..10]  */
        unsigned int    dr_angdifflut9        : 5   ; /* [19..15]  */
        unsigned int    dr_angdifflut10       : 5   ; /* [24..20]  */
        unsigned int    dr_angdifflut11       : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRANGLESTRLUT1_1;

/* Define the union U_VPSS_DRANGLESTRLUT1_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_angdifflut12       : 5   ; /* [4..0]  */
        unsigned int    dr_angdifflut13       : 5   ; /* [9..5]  */
        unsigned int    dr_angdifflut14       : 5   ; /* [14..10]  */
        unsigned int    dr_angdifflut15       : 5   ; /* [19..15]  */
        unsigned int    dr_angdifflut16       : 5   ; /* [24..20]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRANGLESTRLUT1_2;

/* Define the union U_VPSS_DRANGLESTRLUT2_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_angminlut0         : 5   ; /* [4..0]  */
        unsigned int    dr_angminlut1         : 5   ; /* [9..5]  */
        unsigned int    dr_angminlut2         : 5   ; /* [14..10]  */
        unsigned int    dr_angminlut3         : 5   ; /* [19..15]  */
        unsigned int    dr_angminlut4         : 5   ; /* [24..20]  */
        unsigned int    dr_angminlut5         : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRANGLESTRLUT2_0;

/* Define the union U_VPSS_DRANGLESTRLUT2_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_angminlut6         : 5   ; /* [4..0]  */
        unsigned int    dr_angminlut7         : 5   ; /* [9..5]  */
        unsigned int    dr_angminlut8         : 5   ; /* [14..10]  */
        unsigned int    dr_angminlut9         : 5   ; /* [19..15]  */
        unsigned int    dr_angminlut10        : 5   ; /* [24..20]  */
        unsigned int    dr_angminlut11        : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRANGLESTRLUT2_1;

/* Define the union U_VPSS_DRANGLESTRLUT2_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_angminlut12        : 5   ; /* [4..0]  */
        unsigned int    dr_angminlut13        : 5   ; /* [9..5]  */
        unsigned int    dr_angminlut14        : 5   ; /* [14..10]  */
        unsigned int    dr_angminlut15        : 5   ; /* [19..15]  */
        unsigned int    dr_angminlut16        : 5   ; /* [24..20]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DRANGLESTRLUT2_2;

/* Define the union U_VPSS_DROVTSTRADJUSTLUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_ovadjlut0          : 4   ; /* [3..0]  */
        unsigned int    dr_ovadjlut1          : 4   ; /* [7..4]  */
        unsigned int    dr_ovadjlut2          : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DROVTSTRADJUSTLUT0;

/* Define the union U_VPSS_DROVTSTRADJUSTLUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_ovadjlut3          : 4   ; /* [3..0]  */
        unsigned int    dr_ovadjlut4          : 4   ; /* [7..4]  */
        unsigned int    dr_ovadjlut5          : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DROVTSTRADJUSTLUT1;

/* Define the union U_VPSS_DROVTSTRADJUSTLUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_ovadjlut6          : 4   ; /* [3..0]  */
        unsigned int    dr_ovadjlut7          : 4   ; /* [7..4]  */
        unsigned int    dr_ovadjlut8          : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DROVTSTRADJUSTLUT2;

/* Define the union U_VPSS_DROVTSTRLUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_ovstrlut0          : 4   ; /* [3..0]  */
        unsigned int    dr_ovstrlut1          : 4   ; /* [7..4]  */
        unsigned int    dr_ovstrlut2          : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DROVTSTRLUT0;

/* Define the union U_VPSS_DROVTSTRLUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_ovstrlut3          : 4   ; /* [3..0]  */
        unsigned int    dr_ovstrlut4          : 4   ; /* [7..4]  */
        unsigned int    dr_ovstrlut5          : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DROVTSTRLUT1;

/* Define the union U_VPSS_DROVTSTRLUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_ovstrlut6          : 4   ; /* [3..0]  */
        unsigned int    dr_ovstrlut7          : 4   ; /* [7..4]  */
        unsigned int    dr_ovstrlut8          : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DROVTSTRLUT2;

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
        unsigned int    reserved_2            : 17  ; /* [31..15]  */
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
        unsigned int    reserved_2            : 17  ; /* [31..15]  */
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
        unsigned int    reserved_2            : 17  ; /* [31..15]  */
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
        unsigned int    reserved_2            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VC1_CTRL3;

/* Define the union U_VPSS_DB_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_en             : 1   ; /* [0]  */
        unsigned int    db_lum_ver_en         : 1   ; /* [1]  */
        unsigned int    db_lum_hor_en         : 1   ; /* [2]  */
        unsigned int    db_cb_en              : 1   ; /* [3]  */
        unsigned int    db_cr_en              : 1   ; /* [4]  */
        unsigned int    db_chr_ver_en         : 1   ; /* [5]  */
        unsigned int    db_chr_hor_en         : 1   ; /* [6]  */
        unsigned int    db_chr_hor_mid_grey_en : 1   ; /* [7]  */
        unsigned int    reserved_0            : 2   ; /* [9..8]  */
        unsigned int    db_demo_en            : 1   ; /* [10]  */
        unsigned int    db_demo_mode          : 1   ; /* [11]  */
        unsigned int    det_size_en           : 1   ; /* [12]  */
        unsigned int    det_hy_en             : 1   ; /* [13]  */
        unsigned int    det_hc_en             : 1   ; /* [14]  */
        unsigned int    det_vy_en             : 1   ; /* [15]  */
        unsigned int    reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_CTRL;

/* Define the union U_VPSS_DB_DIR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    grad_sub_ratio        : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    ctrst_thresh          : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_DIR;

/* Define the union U_VPSS_DB_BLK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lum_h_blk_size        : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    lum_v_blk_size        : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    chr_h_blk_size        : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    chr_v_blk_size        : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_BLK;

/* Define the union U_VPSS_DB_RATIO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lum_hor_scale_ratio   : 2   ; /* [1..0]  */
        unsigned int    lum_ver_scale_ratio   : 2   ; /* [3..2]  */
        unsigned int    chr_hor_scale_ratio   : 2   ; /* [5..4]  */
        unsigned int    chr_ver_scale_ratio   : 2   ; /* [7..6]  */
        unsigned int    lum_hor_filter_sel    : 2   ; /* [9..8]  */
        unsigned int    chr_hor_filter_sel    : 2   ; /* [11..10]  */
        unsigned int    glabal_db_strenth_lum : 4   ; /* [15..12]  */
        unsigned int    glabal_db_strenth_chr : 4   ; /* [19..16]  */
        unsigned int    lum_hor_txt_win_size  : 2   ; /* [21..20]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_RATIO;

/* Define the union U_VPSS_DB_LHHF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lum_hor_hf_diff_core  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    lum_hor_hf_diff_gain1 : 4   ; /* [11..8]  */
        unsigned int    lum_hor_hf_diff_gain2 : 4   ; /* [15..12]  */
        unsigned int    lum_hor_hf_var_core   : 5   ; /* [20..16]  */
        unsigned int    lum_hor_bord_adj_gain : 3   ; /* [23..21]  */
        unsigned int    lum_hor_hf_var_gain1  : 4   ; /* [27..24]  */
        unsigned int    lum_hor_hf_var_gain2  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LHHF;

/* Define the union U_VPSS_DB_LVHF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lum_ver_hf_var_core   : 5   ; /* [4..0]  */
        unsigned int    lum_ver_bord_adj_gain : 3   ; /* [7..5]  */
        unsigned int    lum_ver_hf_var_gain1  : 4   ; /* [11..8]  */
        unsigned int    lum_ver_hf_var_gain2  : 4   ; /* [15..12]  */
        unsigned int    lum_hor_adj_gain      : 5   ; /* [20..16]  */
        unsigned int    reserved_0            : 3   ; /* [23..21]  */
        unsigned int    lum_ver_adj_gain      : 5   ; /* [28..24]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LVHF;

/* Define the union U_VPSS_DB_CTRS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ctrst_adj_core        : 5   ; /* [4..0]  */
        unsigned int    dir_smooth_mode       : 2   ; /* [6..5]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    ctrst_adj_gain1       : 4   ; /* [11..8]  */
        unsigned int    ctrst_adj_gain2       : 4   ; /* [15..12]  */
        unsigned int    max_lum_hor_db_dist   : 4   ; /* [19..16]  */
        unsigned int    max_chr_hor_db_dist   : 4   ; /* [23..20]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_CTRS;

/* Define the union U_VPSS_DB_LUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lum_hor_delta_lut_p0  : 4   ; /* [3..0]  */
        unsigned int    lum_hor_delta_lut_p1  : 4   ; /* [7..4]  */
        unsigned int    lum_hor_delta_lut_p2  : 4   ; /* [11..8]  */
        unsigned int    lum_hor_delta_lut_p3  : 4   ; /* [15..12]  */
        unsigned int    lum_hor_delta_lut_p4  : 4   ; /* [19..16]  */
        unsigned int    lum_hor_delta_lut_p5  : 4   ; /* [23..20]  */
        unsigned int    lum_hor_delta_lut_p6  : 4   ; /* [27..24]  */
        unsigned int    lum_hor_delta_lut_p7  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT0;

/* Define the union U_VPSS_DB_LUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lum_hor_delta_lut_p8  : 4   ; /* [3..0]  */
        unsigned int    lum_hor_delta_lut_p9  : 4   ; /* [7..4]  */
        unsigned int    lum_hor_delta_lut_p10 : 4   ; /* [11..8]  */
        unsigned int    lum_hor_delta_lut_p11 : 4   ; /* [15..12]  */
        unsigned int    lum_hor_delta_lut_p12 : 4   ; /* [19..16]  */
        unsigned int    lum_hor_delta_lut_p13 : 4   ; /* [23..20]  */
        unsigned int    lum_hor_delta_lut_p14 : 4   ; /* [27..24]  */
        unsigned int    lum_hor_delta_lut_p15 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT1;

/* Define the union U_VPSS_DB_LUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lum_ver_delta_lut_p0  : 4   ; /* [3..0]  */
        unsigned int    lum_ver_delta_lut_p1  : 4   ; /* [7..4]  */
        unsigned int    lum_ver_delta_lut_p2  : 4   ; /* [11..8]  */
        unsigned int    lum_ver_delta_lut_p3  : 4   ; /* [15..12]  */
        unsigned int    lum_ver_delta_lut_p4  : 4   ; /* [19..16]  */
        unsigned int    lum_ver_delta_lut_p5  : 4   ; /* [23..20]  */
        unsigned int    lum_ver_delta_lut_p6  : 4   ; /* [27..24]  */
        unsigned int    lum_ver_delta_lut_p7  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT2;

/* Define the union U_VPSS_DB_LUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lum_ver_delta_lut_p8  : 4   ; /* [3..0]  */
        unsigned int    lum_ver_delta_lut_p9  : 4   ; /* [7..4]  */
        unsigned int    lum_ver_delta_lut_p10 : 4   ; /* [11..8]  */
        unsigned int    lum_ver_delta_lut_p11 : 4   ; /* [15..12]  */
        unsigned int    lum_ver_delta_lut_p12 : 4   ; /* [19..16]  */
        unsigned int    lum_ver_delta_lut_p13 : 4   ; /* [23..20]  */
        unsigned int    lum_ver_delta_lut_p14 : 4   ; /* [27..24]  */
        unsigned int    lum_ver_delta_lut_p15 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT3;

/* Define the union U_VPSS_DB_LUT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chr_hor_delta_lut_p0  : 4   ; /* [3..0]  */
        unsigned int    chr_hor_delta_lut_p1  : 4   ; /* [7..4]  */
        unsigned int    chr_hor_delta_lut_p2  : 4   ; /* [11..8]  */
        unsigned int    chr_hor_delta_lut_p3  : 4   ; /* [15..12]  */
        unsigned int    chr_hor_delta_lut_p4  : 4   ; /* [19..16]  */
        unsigned int    chr_hor_delta_lut_p5  : 4   ; /* [23..20]  */
        unsigned int    chr_hor_delta_lut_p6  : 4   ; /* [27..24]  */
        unsigned int    chr_hor_delta_lut_p7  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT4;

/* Define the union U_VPSS_DB_LUT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chr_hor_delta_lut_p8  : 4   ; /* [3..0]  */
        unsigned int    chr_hor_delta_lut_p9  : 4   ; /* [7..4]  */
        unsigned int    chr_hor_delta_lut_p10 : 4   ; /* [11..8]  */
        unsigned int    chr_hor_delta_lut_p11 : 4   ; /* [15..12]  */
        unsigned int    chr_hor_delta_lut_p12 : 4   ; /* [19..16]  */
        unsigned int    chr_hor_delta_lut_p13 : 4   ; /* [23..20]  */
        unsigned int    chr_hor_delta_lut_p14 : 4   ; /* [27..24]  */
        unsigned int    chr_hor_delta_lut_p15 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT5;

/* Define the union U_VPSS_DB_LUT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lum_h_str_fade_lut_p0 : 3   ; /* [2..0]  */
        unsigned int    lum_h_str_fade_lut_p1 : 3   ; /* [5..3]  */
        unsigned int    lum_h_str_fade_lut_p2 : 3   ; /* [8..6]  */
        unsigned int    lum_h_str_fade_lut_p3 : 3   ; /* [11..9]  */
        unsigned int    lum_h_str_fade_lut_p4 : 3   ; /* [14..12]  */
        unsigned int    lum_h_str_fade_lut_p5 : 3   ; /* [17..15]  */
        unsigned int    lum_h_str_fade_lut_p6 : 3   ; /* [20..18]  */
        unsigned int    lum_h_str_fade_lut_p7 : 3   ; /* [23..21]  */
        unsigned int    lum_h_str_fade_lut_p8 : 3   ; /* [26..24]  */
        unsigned int    lum_h_str_fade_lut_p9 : 3   ; /* [29..27]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT6;

/* Define the union U_VPSS_DB_LUT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lum_v_str_fade_lut_p0 : 3   ; /* [2..0]  */
        unsigned int    lum_v_str_fade_lut_p1 : 3   ; /* [5..3]  */
        unsigned int    lum_v_str_fade_lut_p2 : 3   ; /* [8..6]  */
        unsigned int    lum_v_str_fade_lut_p3 : 3   ; /* [11..9]  */
        unsigned int    lum_v_str_fade_lut_p4 : 3   ; /* [14..12]  */
        unsigned int    lum_v_str_fade_lut_p5 : 3   ; /* [17..15]  */
        unsigned int    lum_v_str_fade_lut_p6 : 3   ; /* [20..18]  */
        unsigned int    lum_v_str_fade_lut_p7 : 3   ; /* [23..21]  */
        unsigned int    lum_v_str_fade_lut_p8 : 3   ; /* [26..24]  */
        unsigned int    lum_v_str_fade_lut_p9 : 3   ; /* [29..27]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT7;

/* Define the union U_VPSS_DB_LUT8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chr_h_str_fade_lut_p0 : 3   ; /* [2..0]  */
        unsigned int    chr_h_str_fade_lut_p1 : 3   ; /* [5..3]  */
        unsigned int    chr_h_str_fade_lut_p2 : 3   ; /* [8..6]  */
        unsigned int    chr_h_str_fade_lut_p3 : 3   ; /* [11..9]  */
        unsigned int    chr_h_str_fade_lut_p4 : 3   ; /* [14..12]  */
        unsigned int    chr_h_str_fade_lut_p5 : 3   ; /* [17..15]  */
        unsigned int    chr_h_str_fade_lut_p6 : 3   ; /* [20..18]  */
        unsigned int    chr_h_str_fade_lut_p7 : 3   ; /* [23..21]  */
        unsigned int    chr_h_str_fade_lut_p8 : 3   ; /* [26..24]  */
        unsigned int    chr_h_str_fade_lut_p9 : 3   ; /* [29..27]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT8;

/* Define the union U_VPSS_DB_LUT9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lum_h_str_fade_lut_p10 : 3   ; /* [2..0]  */
        unsigned int    lum_h_str_fade_lut_p11 : 3   ; /* [5..3]  */
        unsigned int    lum_v_str_fade_lut_p10 : 3   ; /* [8..6]  */
        unsigned int    lum_v_str_fade_lut_p11 : 3   ; /* [11..9]  */
        unsigned int    chr_h_str_fade_lut_p10 : 3   ; /* [14..12]  */
        unsigned int    chr_h_str_fade_lut_p11 : 3   ; /* [17..15]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT9;

/* Define the union U_VPSS_DB_LUT10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dir_str_gain_lut_p0   : 4   ; /* [3..0]  */
        unsigned int    dir_str_gain_lut_p1   : 4   ; /* [7..4]  */
        unsigned int    dir_str_gain_lut_p2   : 4   ; /* [11..8]  */
        unsigned int    dir_str_gain_lut_p3   : 4   ; /* [15..12]  */
        unsigned int    dir_str_gain_lut_p4   : 4   ; /* [19..16]  */
        unsigned int    dir_str_gain_lut_p5   : 4   ; /* [23..20]  */
        unsigned int    dir_str_gain_lut_p6   : 4   ; /* [27..24]  */
        unsigned int    dir_str_gain_lut_p7   : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT10;

/* Define the union U_VPSS_DBD_THDEDGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_thd_edge           : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    hc_thd_edge           : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    vy_thd_edge           : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_THDEDGE;

/* Define the union U_VPSS_DBD_THDTXT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_thd_txt            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    hc_thd_txt            : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    vy_thd_txt            : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_THDTXT;

/* Define the union U_VPSS_DBD_LUTWGT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    det_lut_wgt0          : 4   ; /* [3..0]  */
        unsigned int    det_lut_wgt1          : 4   ; /* [7..4]  */
        unsigned int    det_lut_wgt2          : 4   ; /* [11..8]  */
        unsigned int    det_lut_wgt3          : 4   ; /* [15..12]  */
        unsigned int    det_lut_wgt4          : 4   ; /* [19..16]  */
        unsigned int    det_lut_wgt5          : 4   ; /* [23..20]  */
        unsigned int    det_lut_wgt6          : 4   ; /* [27..24]  */
        unsigned int    det_lut_wgt7          : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_LUTWGT0;

/* Define the union U_VPSS_DBD_LUTWGT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    det_lut_wgt8          : 4   ; /* [3..0]  */
        unsigned int    det_lut_wgt9          : 4   ; /* [7..4]  */
        unsigned int    det_lut_wgt10         : 4   ; /* [11..8]  */
        unsigned int    det_lut_wgt11         : 4   ; /* [15..12]  */
        unsigned int    det_lut_wgt12         : 4   ; /* [19..16]  */
        unsigned int    det_lut_wgt13         : 4   ; /* [23..20]  */
        unsigned int    det_lut_wgt14         : 4   ; /* [27..24]  */
        unsigned int    det_lut_wgt15         : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_LUTWGT1;

/* Define the union U_VPSS_DBD_RTNBDTXT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_rtn_bd_txt         : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    hc_rtn_bd_txt         : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    vy_rtn_bd_txt         : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_RTNBDTXT;

/* Define the union U_VPSS_DBD_TSTBLKNUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_tst_blk_num        : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 1   ; /* [9]  */
        unsigned int    hc_tst_blk_num        : 9   ; /* [18..10]  */
        unsigned int    reserved_1            : 1   ; /* [19]  */
        unsigned int    vy_tst_blk_num        : 9   ; /* [28..20]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_TSTBLKNUM;

/* Define the union U_VPSS_DBD_HYRDX8BINLUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_rdx8bin_lut0       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    hy_rdx8bin_lut1       : 11  ; /* [22..12]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_HYRDX8BINLUT0;

/* Define the union U_VPSS_DBD_HYRDX8BINLUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_rdx8bin_lut2       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    hy_rdx8bin_lut3       : 11  ; /* [22..12]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_HYRDX8BINLUT1;

/* Define the union U_VPSS_DBD_HYRDX8BINLUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_rdx8bin_lut4       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    hy_rdx8bin_lut5       : 11  ; /* [22..12]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_HYRDX8BINLUT2;

/* Define the union U_VPSS_DBD_HYRDX8BINLUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hy_rdx8bin_lut6       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_HYRDX8BINLUT3;

/* Define the union U_VPSS_DBD_HCRDX8BINLUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hc_rdx8bin_lut0       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    hc_rdx8bin_lut1       : 11  ; /* [22..12]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_HCRDX8BINLUT0;

/* Define the union U_VPSS_DBD_HCRDX8BINLUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hc_rdx8bin_lut2       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    hc_rdx8bin_lut3       : 11  ; /* [22..12]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_HCRDX8BINLUT1;

/* Define the union U_VPSS_DBD_HCRDX8BINLUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hc_rdx8bin_lut4       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    hc_rdx8bin_lut5       : 11  ; /* [22..12]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_HCRDX8BINLUT2;

/* Define the union U_VPSS_DBD_HCRDX8BINLUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hc_rdx8bin_lut6       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_HCRDX8BINLUT3;

/* Define the union U_VPSS_DBD_VYRDX8BINLUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_rdx8bin_lut0       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    vy_rdx8bin_lut1       : 11  ; /* [22..12]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_VYRDX8BINLUT0;

/* Define the union U_VPSS_DBD_VYRDX8BINLUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_rdx8bin_lut2       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    vy_rdx8bin_lut3       : 11  ; /* [22..12]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_VYRDX8BINLUT1;

/* Define the union U_VPSS_DBD_VYRDX8BINLUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_rdx8bin_lut4       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    vy_rdx8bin_lut5       : 11  ; /* [22..12]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_VYRDX8BINLUT2;

/* Define the union U_VPSS_DBD_VYRDX8BINLUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vy_rdx8bin_lut6       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_VYRDX8BINLUT3;

/* Define the union U_VPSS_DBD_BLKSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    min_blk_size          : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    hy_max_blk_size       : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    hc_max_blk_size       : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    vy_max_blk_size       : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_BLKSIZE;

#if 0
//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_VPSS_CTRL            VPSS_CTRL                ; /* 0x0 */
    volatile U_VPSS_CTRL2           VPSS_CTRL2               ; /* 0x4 */
    volatile U_VPSS_CTRL3           VPSS_CTRL3               ; /* 0x8 */
    volatile unsigned int           reserved_0                     ; /* 0xc */
    volatile U_VPSS_IMGSIZE         VPSS_IMGSIZE             ; /* 0x10 */
    volatile U_VPSS_INCROP_POS      VPSS_INCROP_POS          ; /* 0x14 */
    volatile U_VPSS_INCROP_SIZE     VPSS_INCROP_SIZE         ; /* 0x18 */
    volatile U_VPSS_VHD0CROP_POS    VPSS_VHD0CROP_POS        ; /* 0x1c */
    volatile U_VPSS_VHD0CROP_SIZE   VPSS_VHD0CROP_SIZE       ; /* 0x20 */
    volatile U_VPSS_VSDCROP_POS     VPSS_VSDCROP_POS         ; /* 0x24 */
    volatile U_VPSS_VSDCROP_SIZE    VPSS_VSDCROP_SIZE        ; /* 0x28 */
    volatile U_VPSS_VHD0_ZMEORESO   VPSS_VHD0_ZMEORESO       ; /* 0x2c */
    volatile U_VPSS_VSD_ZMEORESO    VPSS_VSD_ZMEORESO        ; /* 0x30 */
    volatile U_VPSS_VHD0LBA_DSIZE   VPSS_VHD0LBA_DSIZE       ; /* 0x34 */
    volatile U_VPSS_VHD0LBA_VFPOS   VPSS_VHD0LBA_VFPOS       ; /* 0x38 */
    volatile U_VPSS_VSDLBA_DSIZE    VPSS_VSDLBA_DSIZE        ; /* 0x3c */
    volatile U_VPSS_VSDLBA_VFPOS    VPSS_VSDLBA_VFPOS        ; /* 0x40 */
    volatile unsigned int           reserved_1[3]                     ; /* 0x44~0x4c */
    volatile U_VPSS_VHD0LBA_BK      VPSS_VHD0LBA_BK          ; /* 0x50 */
    volatile U_VPSS_VSDLBA_BK       VPSS_VSDLBA_BK           ; /* 0x54 */
    volatile unsigned int           reserved_2[2]                     ; /* 0x58~0x5c */
    volatile U_STR_DET_VIDCTRL      STR_DET_VIDCTRL          ; /* 0x60 */
    volatile U_STR_DET_VIDBLKPOS0_1   STR_DET_VIDBLKPOS0_1   ; /* 0x64 */
    volatile U_STR_DET_VIDBLKPOS2_3   STR_DET_VIDBLKPOS2_3   ; /* 0x68 */
    volatile U_STR_DET_VIDBLKPOS4_5   STR_DET_VIDBLKPOS4_5   ; /* 0x6c */
    volatile U_VPSS_DR_CFG0         VPSS_DR_CFG0             ; /* 0x70 */
    volatile U_VPSS_DR_CFG1         VPSS_DR_CFG1             ; /* 0x74 */
    volatile U_VPSS_DNR_INFO        VPSS_DNR_INFO            ; /* 0x78 */
    volatile unsigned int           reserved_3[25]                     ; /* 0x7c~0xdc */
    volatile U_VPSS_AXI_RID0        VPSS_AXI_RID0            ; /* 0xe0 */
    volatile U_VPSS_AXI_RID1        VPSS_AXI_RID1            ; /* 0xe4 */
    volatile unsigned int           VPSS_NODEID              ; /* 0xe8 */
    volatile U_VPSS_AXIID           VPSS_AXIID               ; /* 0xec */
    volatile U_VPSS_INTMASK         VPSS_INTMASK             ; /* 0xf0 */
    volatile unsigned int           reserved_4[3]                     ; /* 0xf4~0xfc */
    volatile U_VPSS_CUR_CTRL        VPSS_CUR_CTRL            ; /* 0x100 */
    volatile unsigned int           VPSS_CURYADDR            ; /* 0x104 */
    volatile unsigned int           VPSS_CURCADDR            ; /* 0x108 */
    volatile unsigned int           VPSS_CURCRADDR           ; /* 0x10c */
    volatile U_VPSS_CURSTRIDE       VPSS_CURSTRIDE           ; /* 0x110 */
    volatile unsigned int           VPSS_CURYADDR_LB         ; /* 0x114 */
    volatile unsigned int           VPSS_CURCADDR_LB         ; /* 0x118 */
    volatile U_VPSS_CURSTRIDE_LB    VPSS_CURSTRIDE_LB        ; /* 0x11c */
    volatile U_VPSS_REF_CTRL        VPSS_REF_CTRL            ; /* 0x120 */
    volatile unsigned int           VPSS_REFYADDR            ; /* 0x124 */
    volatile unsigned int           VPSS_REFCADDR            ; /* 0x128 */
    volatile unsigned int           VPSS_REFCRADDR           ; /* 0x12c */
    volatile U_VPSS_REFSTRIDE       VPSS_REFSTRIDE           ; /* 0x130 */
    volatile unsigned int           VPSS_REFYADDR_LB         ; /* 0x134 */
    volatile unsigned int           VPSS_REFCADDR_LB         ; /* 0x138 */
    volatile U_VPSS_REFSTRIDE_LB    VPSS_REFSTRIDE_LB        ; /* 0x13c */
    volatile U_VPSS_NXT1_CTRL       VPSS_NXT1_CTRL           ; /* 0x140 */
    volatile unsigned int           VPSS_NXT1YADDR           ; /* 0x144 */
    volatile unsigned int           VPSS_NXT1CADDR           ; /* 0x148 */
    volatile unsigned int           VPSS_NXT1CRADDR          ; /* 0x14c */
    volatile U_VPSS_NXT1STRIDE      VPSS_NXT1STRIDE          ; /* 0x150 */
    volatile unsigned int           VPSS_NXT1YADDR_LB        ; /* 0x154 */
    volatile unsigned int           VPSS_NXT1CADDR_LB        ; /* 0x158 */
    volatile U_VPSS_NXT1STRIDE_LB   VPSS_NXT1STRIDE_LB       ; /* 0x15c */
    volatile U_VPSS_NXT2_CTRL       VPSS_NXT2_CTRL           ; /* 0x160 */
    volatile unsigned int           VPSS_NXT2YADDR           ; /* 0x164 */
    volatile unsigned int           VPSS_NXT2CADDR           ; /* 0x168 */
    volatile unsigned int           VPSS_NXT2CRADDR          ; /* 0x16c */
    volatile U_VPSS_NXT2STRIDE      VPSS_NXT2STRIDE          ; /* 0x170 */
    volatile unsigned int           VPSS_NXT2YADDR_LB        ; /* 0x174 */
    volatile unsigned int           VPSS_NXT2CADDR_LB        ; /* 0x178 */
    volatile U_VPSS_NXT2STRIDE_LB   VPSS_NXT2STRIDE_LB       ; /* 0x17c */
    volatile U_VPSS_VHD0CTRL        VPSS_VHD0CTRL            ; /* 0x180 */
    volatile U_VPSS_VHD0SIZE        VPSS_VHD0SIZE            ; /* 0x184 */
    volatile unsigned int           VPSS_VHD0YADDR           ; /* 0x188 */
    volatile unsigned int           VPSS_VHD0CADDR           ; /* 0x18c */
    volatile U_VPSS_VHD0STRIDE      VPSS_VHD0STRIDE          ; /* 0x190 */
    volatile U_VPSS_VSDCTRL         VPSS_VSDCTRL             ; /* 0x194 */
    volatile U_VPSS_VSDSIZE         VPSS_VSDSIZE             ; /* 0x198 */
    volatile unsigned int           VPSS_VSDYADDR            ; /* 0x19c */
    volatile unsigned int           VPSS_VSDCADDR            ; /* 0x1a0 */
    volatile U_VPSS_VSDSTRIDE       VPSS_VSDSTRIDE           ; /* 0x1a4 */
    volatile unsigned int           VPSS_STRADDR             ; /* 0x1a8 */
    volatile unsigned int           VPSS_STWADDR             ; /* 0x1ac */
    volatile U_VPSS_STSTRIDE        VPSS_STSTRIDE            ; /* 0x1b0 */
    volatile unsigned int           VPSS_STT_W_ADDR          ; /* 0x1b4 */
    volatile U_VPSS_TUNL_CTRL0      VPSS_TUNL_CTRL0          ; /* 0x1b8 */
    volatile U_VPSS_TUNL_CTRL1      VPSS_TUNL_CTRL1          ; /* 0x1bc */
    volatile unsigned int           VPSS_VHD0_TUNL_ADDR      ; /* 0x1c0 */
    volatile unsigned int           VPSS_VSD_TUNL_ADDR       ; /* 0x1c4 */
    volatile unsigned int           VPSS_CUR_TUNL_ADDR       ; /* 0x1c8 */
    volatile unsigned int           reserved_5[13]                     ; /* 0x1cc~0x1fc */
    volatile unsigned int           VPSS_VHD0_ZME_LHADDR     ; /* 0x200 */
    volatile unsigned int           VPSS_VHD0_ZME_LVADDR     ; /* 0x204 */
    volatile unsigned int           VPSS_VHD0_ZME_CHADDR     ; /* 0x208 */
    volatile unsigned int           VPSS_VHD0_ZME_CVADDR     ; /* 0x20c */
    volatile unsigned int           VPSS_VSD_ZME_LHADDR      ; /* 0x210 */
    volatile unsigned int           VPSS_VSD_ZME_LVADDR      ; /* 0x214 */
    volatile unsigned int           VPSS_VSD_ZME_CHADDR      ; /* 0x218 */
    volatile unsigned int           VPSS_VSD_ZME_CVADDR      ; /* 0x21c */
    volatile unsigned int           VPSS_VC1_STR_ADDR        ; /* 0x220 */
    volatile unsigned int           VPSS_ZME_ADDR            ; /* 0x224 */
    volatile unsigned int           VPSS_HSP_ADDR            ; /* 0x228 */
    volatile unsigned int           VPSS_DB_ADDR             ; /* 0x22c */
    volatile unsigned int           VPSS_DR_ADDR             ; /* 0x230 */
    volatile unsigned int           VPSS_DEI_ADDR            ; /* 0x234 */
    volatile unsigned int           VPSS_CHK_SUM_W_ADDR      ; /* 0x238 */
    volatile unsigned int           VPSS_VHD0_LCMP_ADDR      ; /* 0x23c */
    volatile unsigned int           reserved_6[47]                     ; /* 0x240~0x2f8 */
    volatile unsigned int           VPSS_PNEXT               ; /* 0x2fc */
    volatile U_VPSS_START           VPSS_START               ; /* 0x300 */
    volatile U_VPSS_INTSTATE        VPSS_INTSTATE            ; /* 0x304 */
    volatile U_VPSS_INTCLR          VPSS_INTCLR              ; /* 0x308 */
    volatile U_VPSS_RAWINT          VPSS_RAWINT              ; /* 0x30c */
    volatile unsigned int           VPSS_PFCNT               ; /* 0x310 */
    volatile U_VPSS_MISCELLANEOUS   VPSS_MISCELLANEOUS       ; /* 0x314 */
    volatile unsigned int           VPSS_MACCFG              ; /* 0x318 */
    volatile unsigned int           VPSS_TIMEOUT             ; /* 0x31c */
    volatile unsigned int           VPSS_EOFCNT              ; /* 0x320 */
    volatile unsigned int           reserved_7[3]                     ; /* 0x324~0x32c */
    volatile unsigned int           VPSS_VERSION             ; /* 0x330 */
    volatile unsigned int           reserved_8[51]                     ; /* 0x334~0x3fc */
    volatile U_VPSS_VHD0_LCMP_YINFO   VPSS_VHD0_LCMP_YINFO   ; /* 0x400 */
    volatile U_VPSS_VHD0_LCMP_YSIZE   VPSS_VHD0_LCMP_YSIZE   ; /* 0x404 */
    volatile U_VPSS_VHD0_LCMP_YRCCFG0   VPSS_VHD0_LCMP_YRCCFG0 ; /* 0x408 */
    volatile U_VPSS_VHD0_LCMP_YRCCFG1   VPSS_VHD0_LCMP_YRCCFG1 ; /* 0x40c */
    volatile U_VPSS_VHD0_LCMP_YRCCFG2   VPSS_VHD0_LCMP_YRCCFG2 ; /* 0x410 */
    volatile U_VPSS_VHD0_LCMP_YRCCFG3   VPSS_VHD0_LCMP_YRCCFG3 ; /* 0x414 */
    volatile U_VPSS_VHD0_LCMP_YRCCFG4   VPSS_VHD0_LCMP_YRCCFG4 ; /* 0x418 */
    volatile U_VPSS_VHD0_LCMP_YRCCFG5   VPSS_VHD0_LCMP_YRCCFG5 ; /* 0x41c */
    volatile U_VPSS_VHD0_LCMP_YMAXLEN   VPSS_VHD0_LCMP_YMAXLEN ; /* 0x420 */
    volatile unsigned int           reserved_9[7]                     ; /* 0x424~0x43c */
    volatile U_VPSS_VHD0_LCMP_CINFO   VPSS_VHD0_LCMP_CINFO   ; /* 0x440 */
    volatile U_VPSS_VHD0_LCMP_CSIZE   VPSS_VHD0_LCMP_CSIZE   ; /* 0x444 */
    volatile U_VPSS_VHD0_LCMP_CRCCFG0   VPSS_VHD0_LCMP_CRCCFG0 ; /* 0x448 */
    volatile U_VPSS_VHD0_LCMP_CRCCFG1   VPSS_VHD0_LCMP_CRCCFG1 ; /* 0x44c */
    volatile U_VPSS_VHD0_LCMP_CRCCFG2   VPSS_VHD0_LCMP_CRCCFG2 ; /* 0x450 */
    volatile U_VPSS_VHD0_LCMP_CRCCFG3   VPSS_VHD0_LCMP_CRCCFG3 ; /* 0x454 */
    volatile U_VPSS_VHD0_LCMP_CRCCFG4   VPSS_VHD0_LCMP_CRCCFG4 ; /* 0x458 */
    volatile U_VPSS_VHD0_LCMP_CRCCFG5   VPSS_VHD0_LCMP_CRCCFG5 ; /* 0x45c */
    volatile U_VPSS_VHD0_LCMP_CMAXLEN   VPSS_VHD0_LCMP_CMAXLEN ; /* 0x460 */
    volatile unsigned int           reserved_10[743]                     ; /* 0x464~0xffc */
    volatile U_VPSS_DIECTRL         VPSS_DIECTRL             ; /* 0x1000 */
    volatile U_VPSS_DIELMA2         VPSS_DIELMA2             ; /* 0x1004 */
    volatile U_VPSS_DIEINTEN        VPSS_DIEINTEN            ; /* 0x1008 */
    volatile U_VPSS_DIESCALE        VPSS_DIESCALE            ; /* 0x100c */
    volatile U_VPSS_DIECHECK1       VPSS_DIECHECK1           ; /* 0x1010 */
    volatile U_VPSS_DIECHECK2       VPSS_DIECHECK2           ; /* 0x1014 */
    volatile U_VPSS_DIEDIR0_3       VPSS_DIEDIR0_3           ; /* 0x1018 */
    volatile U_VPSS_DIEDIR4_7       VPSS_DIEDIR4_7           ; /* 0x101c */
    volatile U_VPSS_DIEDIR8_11      VPSS_DIEDIR8_11          ; /* 0x1020 */
    volatile U_VPSS_DIEDIR12_14     VPSS_DIEDIR12_14         ; /* 0x1024 */
    volatile U_VPSS_DIESTA          VPSS_DIESTA              ; /* 0x1028 */
    volatile unsigned int           VPSS_DIESTPPREADDR       ; /* 0x102c */
    volatile unsigned int           VPSS_DIESTPREADDR        ; /* 0x1030 */
    volatile U_VPSS_DIEINTPSCL0     VPSS_DIEINTPSCL0         ; /* 0x1034 */
    volatile U_VPSS_DIEINTPSCL1     VPSS_DIEINTPSCL1         ; /* 0x1038 */
    volatile U_VPSS_DIEDIRTHD       VPSS_DIEDIRTHD           ; /* 0x103c */
    volatile U_VPSS_DIEJITMTN       VPSS_DIEJITMTN           ; /* 0x1040 */
    volatile U_VPSS_DIEFLDMTN       VPSS_DIEFLDMTN           ; /* 0x1044 */
    volatile U_VPSS_DIEMTNDIFFTHD0   VPSS_DIEMTNDIFFTHD0     ; /* 0x1048 */
    volatile U_VPSS_DIEMTNDIFFTHD1   VPSS_DIEMTNDIFFTHD1     ; /* 0x104c */
    volatile U_VPSS_DIEMTNIIRSLP    VPSS_DIEMTNIIRSLP        ; /* 0x1050 */
    volatile U_VPSS_DIEMTNIIRRAT    VPSS_DIEMTNIIRRAT        ; /* 0x1054 */
    volatile U_VPSS_DIEHISMODE      VPSS_DIEHISMODE          ; /* 0x1058 */
    volatile U_VPSS_DIEMORFLT       VPSS_DIEMORFLT           ; /* 0x105c */
    volatile U_VPSS_DIECOMBCHK0     VPSS_DIECOMBCHK0         ; /* 0x1060 */
    volatile U_VPSS_DIECOMBCHK1     VPSS_DIECOMBCHK1         ; /* 0x1064 */
    volatile U_VPSS_DIEFRMMTNSMTHTHD0   VPSS_DIEFRMMTNSMTHTHD0 ; /* 0x1068 */
    volatile U_VPSS_DIEFRMMTNSMTHTHD1   VPSS_DIEFRMMTNSMTHTHD1 ; /* 0x106c */
    volatile U_VPSS_DIEFRMMTNSMTHSLP   VPSS_DIEFRMMTNSMTHSLP ; /* 0x1070 */
    volatile U_VPSS_DIEFRMMTNSMTHRAT   VPSS_DIEFRMMTNSMTHRAT ; /* 0x1074 */
    volatile U_VPSS_DIEFRMFLDBLENDTHD0   VPSS_DIEFRMFLDBLENDTHD0 ; /* 0x1078 */
    volatile U_VPSS_DIEFRMFLDBLENDTHD1   VPSS_DIEFRMFLDBLENDTHD1 ; /* 0x107c */
    volatile U_VPSS_DIEFRMFLDBLENDSLP   VPSS_DIEFRMFLDBLENDSLP ; /* 0x1080 */
    volatile U_VPSS_DIEFRMFLDBLENDRAT   VPSS_DIEFRMFLDBLENDRAT ; /* 0x1084 */
    volatile U_VPSS_DIEMTNADJ       VPSS_DIEMTNADJ           ; /* 0x1088 */
    volatile U_VPSS_DIEGLBMTNTHD0   VPSS_DIEGLBMTNTHD0       ; /* 0x108c */
    volatile U_VPSS_DIEGLBMTNTHD1   VPSS_DIEGLBMTNTHD1       ; /* 0x1090 */
    volatile U_VPSS_CCRSCLR0        VPSS_CCRSCLR0            ; /* 0x1094 */
    volatile U_VPSS_CCRSCLR1        VPSS_CCRSCLR1            ; /* 0x1098 */
    volatile unsigned int           reserved_11[42]                     ; /* 0x109c~0x1140 */
    volatile U_VPSS_PDPHISTTHD1     VPSS_PDPHISTTHD1         ; /* 0x1144 */
    volatile U_VPSS_PDPCCMOV        VPSS_PDPCCMOV            ; /* 0x1148 */
    volatile unsigned int           VPSS_PDICHD              ; /* 0x114c */
    volatile U_VPSS_PDCTRL          VPSS_PDCTRL              ; /* 0x1150 */
    volatile U_VPSS_PDBLKTHD        VPSS_PDBLKTHD            ; /* 0x1154 */
    volatile U_VPSS_PDHISTTHD       VPSS_PDHISTTHD           ; /* 0x1158 */
    volatile U_VPSS_PDUMTHD         VPSS_PDUMTHD             ; /* 0x115c */
    volatile U_VPSS_PDPCCCORING     VPSS_PDPCCCORING         ; /* 0x1160 */
    volatile U_VPSS_PDPCCHTHD       VPSS_PDPCCHTHD           ; /* 0x1164 */
    volatile U_VPSS_PDPCCVTHD       VPSS_PDPCCVTHD           ; /* 0x1168 */
    volatile U_VPSS_PDITDIFFVTHD    VPSS_PDITDIFFVTHD        ; /* 0x116c */
    volatile U_VPSS_PDLASITHD       VPSS_PDLASITHD           ; /* 0x1170 */
    volatile U_VPSS_PDLASIMODE0COEF   VPSS_PDLASIMODE0COEF   ; /* 0x1174 */
    volatile U_VPSS_PDLASIMODE1COEF   VPSS_PDLASIMODE1COEF   ; /* 0x1178 */
    volatile U_VPSS_PDREGION        VPSS_PDREGION            ; /* 0x117c */
    volatile unsigned int           reserved_12[928]                     ; /* 0x1180~0x1ffc */
    volatile U_VPSS_VHD0_HSP        VPSS_VHD0_HSP            ; /* 0x2000 */
    volatile U_VPSS_VHD0_HLOFFSET   VPSS_VHD0_HLOFFSET       ; /* 0x2004 */
    volatile U_VPSS_VHD0_HCOFFSET   VPSS_VHD0_HCOFFSET       ; /* 0x2008 */
    volatile U_VPSS_VHD0_VSP        VPSS_VHD0_VSP            ; /* 0x200c */
    volatile U_VPSS_VHD0_VSR        VPSS_VHD0_VSR            ; /* 0x2010 */
    volatile U_VPSS_VHD0_VOFFSET    VPSS_VHD0_VOFFSET        ; /* 0x2014 */
    volatile unsigned int           reserved_13[26]                     ; /* 0x2018~0x207c */
    volatile U_VPSS_VSD_HSP         VPSS_VSD_HSP             ; /* 0x2080 */
    volatile U_VPSS_VSD_HLOFFSET    VPSS_VSD_HLOFFSET        ; /* 0x2084 */
    volatile U_VPSS_VSD_HCOFFSET    VPSS_VSD_HCOFFSET        ; /* 0x2088 */
    volatile U_VPSS_VSD_VSP         VPSS_VSD_VSP             ; /* 0x208c */
    volatile U_VPSS_VSD_VSR         VPSS_VSD_VSR             ; /* 0x2090 */
    volatile U_VPSS_VSD_VOFFSET     VPSS_VSD_VOFFSET         ; /* 0x2094 */
    volatile unsigned int           reserved_14[26]                     ; /* 0x2098~0x20fc */
    volatile U_VPSS_HSPCFG1         VPSS_HSPCFG1             ; /* 0x2100 */
    volatile U_VPSS_HSPCFG2         VPSS_HSPCFG2             ; /* 0x2104 */
    volatile unsigned int           reserved_15[3]                     ; /* 0x2108~0x2110 */
    volatile U_VPSS_HSPCFG3         VPSS_HSPCFG3             ; /* 0x2114 */
    volatile U_VPSS_HSPCFG4         VPSS_HSPCFG4             ; /* 0x2118 */
    volatile U_VPSS_HSPCFG5         VPSS_HSPCFG5             ; /* 0x211c */
    volatile U_VPSS_HSPCFG6         VPSS_HSPCFG6             ; /* 0x2120 */
    volatile unsigned int           reserved_16[3]                     ; /* 0x2124~0x212c */
    volatile U_VPSS_HSPCFG7         VPSS_HSPCFG7             ; /* 0x2130 */
    volatile U_VPSS_HSPCFG8         VPSS_HSPCFG8             ; /* 0x2134 */
    volatile U_VPSS_HSPCFG9         VPSS_HSPCFG9             ; /* 0x2138 */
    volatile U_VPSS_HSPCFG10        VPSS_HSPCFG10            ; /* 0x213c */
    volatile unsigned int           reserved_17[112]                     ; /* 0x2140~0x22fc */
    volatile U_VPSS_DR_CTRL         VPSS_DR_CTRL             ; /* 0x2300 */
    volatile U_VPSS_DR_THR          VPSS_DR_THR              ; /* 0x2304 */
    volatile U_VPSS_DRLSWSTRLUT0    VPSS_DRLSWSTRLUT0        ; /* 0x2308 */
    volatile U_VPSS_DRLSWSTRLUT1    VPSS_DRLSWSTRLUT1        ; /* 0x230c */
    volatile U_VPSS_DRLSWSTRLUT2    VPSS_DRLSWSTRLUT2        ; /* 0x2310 */
    volatile U_VPSS_DRLSWSTRLUT3    VPSS_DRLSWSTRLUT3        ; /* 0x2314 */
    volatile U_VPSS_DRLSWSTRLUT4    VPSS_DRLSWSTRLUT4        ; /* 0x2318 */
    volatile U_VPSS_DRLSWSTRLUT5    VPSS_DRLSWSTRLUT5        ; /* 0x231c */
    volatile U_VPSS_DRLSWSTRLUT6    VPSS_DRLSWSTRLUT6        ; /* 0x2320 */
    volatile U_VPSS_DRLSWSTRLUT7    VPSS_DRLSWSTRLUT7        ; /* 0x2324 */
    volatile U_VPSS_DRLSWSTRLUT8    VPSS_DRLSWSTRLUT8        ; /* 0x2328 */
    volatile U_VPSS_DRLRWSTRLUT0    VPSS_DRLRWSTRLUT0        ; /* 0x232c */
    volatile U_VPSS_DRLRWSTRLUT1    VPSS_DRLRWSTRLUT1        ; /* 0x2330 */
    volatile U_VPSS_DRLRWSTRLUT2    VPSS_DRLRWSTRLUT2        ; /* 0x2334 */
    volatile U_VPSS_DRLRWSTRLUT3    VPSS_DRLRWSTRLUT3        ; /* 0x2338 */
    volatile U_VPSS_DRLRWSTRLUT4    VPSS_DRLRWSTRLUT4        ; /* 0x233c */
    volatile U_VPSS_DRLRWSTRLUT5    VPSS_DRLRWSTRLUT5        ; /* 0x2340 */
    volatile U_VPSS_DRLRWSTRLUT6    VPSS_DRLRWSTRLUT6        ; /* 0x2344 */
    volatile U_VPSS_DRLRWSTRLUT7    VPSS_DRLRWSTRLUT7        ; /* 0x2348 */
    volatile U_VPSS_DRLRWSTRLUT8    VPSS_DRLRWSTRLUT8        ; /* 0x234c */
    volatile U_VPSS_DRLSWSTRADJUSTLUT0   VPSS_DRLSWSTRADJUSTLUT0 ; /* 0x2350 */
    volatile U_VPSS_DRLSWSTRADJUSTLUT1   VPSS_DRLSWSTRADJUSTLUT1 ; /* 0x2354 */
    volatile U_VPSS_DRLSWSTRADJUSTLUT2   VPSS_DRLSWSTRADJUSTLUT2 ; /* 0x2358 */
    volatile U_VPSS_DRLRWSTRADJUSTLUT0   VPSS_DRLRWSTRADJUSTLUT0 ; /* 0x235c */
    volatile U_VPSS_DRLRWSTRADJUSTLUT1   VPSS_DRLRWSTRADJUSTLUT1 ; /* 0x2360 */
    volatile U_VPSS_DRLRWSTRADJUSTLUT2   VPSS_DRLRWSTRADJUSTLUT2 ; /* 0x2364 */
    volatile U_VPSS_DRANGADJSUTLUT0   VPSS_DRANGADJSUTLUT0   ; /* 0x2368 */
    volatile U_VPSS_DRANGADJSUTLUT1   VPSS_DRANGADJSUTLUT1   ; /* 0x236c */
    volatile U_VPSS_DRANGADJSUTLUT2   VPSS_DRANGADJSUTLUT2   ; /* 0x2370 */
    volatile U_VPSS_DRANGLESTRLUT1_0   VPSS_DRANGLESTRLUT1_0 ; /* 0x2374 */
    volatile U_VPSS_DRANGLESTRLUT1_1   VPSS_DRANGLESTRLUT1_1 ; /* 0x2378 */
    volatile U_VPSS_DRANGLESTRLUT1_2   VPSS_DRANGLESTRLUT1_2 ; /* 0x237c */
    volatile U_VPSS_DRANGLESTRLUT2_0   VPSS_DRANGLESTRLUT2_0 ; /* 0x2380 */
    volatile U_VPSS_DRANGLESTRLUT2_1   VPSS_DRANGLESTRLUT2_1 ; /* 0x2384 */
    volatile U_VPSS_DRANGLESTRLUT2_2   VPSS_DRANGLESTRLUT2_2 ; /* 0x2388 */
    volatile U_VPSS_DROVTSTRADJUSTLUT0   VPSS_DROVTSTRADJUSTLUT0 ; /* 0x238c */
    volatile U_VPSS_DROVTSTRADJUSTLUT1   VPSS_DROVTSTRADJUSTLUT1 ; /* 0x2390 */
    volatile U_VPSS_DROVTSTRADJUSTLUT2   VPSS_DROVTSTRADJUSTLUT2 ; /* 0x2394 */
    volatile U_VPSS_DROVTSTRLUT0    VPSS_DROVTSTRLUT0        ; /* 0x2398 */
    volatile U_VPSS_DROVTSTRLUT1    VPSS_DROVTSTRLUT1        ; /* 0x239c */
    volatile U_VPSS_DROVTSTRLUT2    VPSS_DROVTSTRLUT2        ; /* 0x23a0 */
    volatile unsigned int           reserved_18[23]                     ; /* 0x23a4~0x23fc */
    volatile U_VPSS_VC1_CTRL0       VPSS_VC1_CTRL0           ; /* 0x2400 */
    volatile U_VPSS_VC1_CTRL1       VPSS_VC1_CTRL1           ; /* 0x2404 */
    volatile U_VPSS_VC1_CTRL2       VPSS_VC1_CTRL2           ; /* 0x2408 */
    volatile U_VPSS_VC1_CTRL3       VPSS_VC1_CTRL3           ; /* 0x2408 */
    volatile unsigned int           reserved_19[61]                     ; /* 0x240c~0x24fc */
    volatile U_VPSS_DB_CTRL         VPSS_DB_CTRL             ; /* 0x2500 */
    volatile U_VPSS_DB_DIR          VPSS_DB_DIR              ; /* 0x2504 */
    volatile U_VPSS_DB_BLK          VPSS_DB_BLK              ; /* 0x2508 */
    volatile U_VPSS_DB_RATIO        VPSS_DB_RATIO            ; /* 0x250c */
    volatile U_VPSS_DB_LHHF         VPSS_DB_LHHF             ; /* 0x2510 */
    volatile U_VPSS_DB_LVHF         VPSS_DB_LVHF             ; /* 0x2514 */
    volatile U_VPSS_DB_CTRS         VPSS_DB_CTRS             ; /* 0x2518 */
    volatile U_VPSS_DB_LUT0         VPSS_DB_LUT0             ; /* 0x251c */
    volatile U_VPSS_DB_LUT1         VPSS_DB_LUT1             ; /* 0x2520 */
    volatile U_VPSS_DB_LUT2         VPSS_DB_LUT2             ; /* 0x2524 */
    volatile U_VPSS_DB_LUT3         VPSS_DB_LUT3             ; /* 0x2528 */
    volatile U_VPSS_DB_LUT4         VPSS_DB_LUT4             ; /* 0x252c */
    volatile U_VPSS_DB_LUT5         VPSS_DB_LUT5             ; /* 0x2530 */
    volatile U_VPSS_DB_LUT6         VPSS_DB_LUT6             ; /* 0x2534 */
    volatile U_VPSS_DB_LUT7         VPSS_DB_LUT7             ; /* 0x2538 */
    volatile U_VPSS_DB_LUT8         VPSS_DB_LUT8             ; /* 0x253c */
    volatile U_VPSS_DB_LUT9         VPSS_DB_LUT9             ; /* 0x2540 */
    volatile U_VPSS_DB_LUT10        VPSS_DB_LUT10            ; /* 0x2544 */
    volatile unsigned int           reserved_20[2]                     ; /* 0x2548~0x254c */
    volatile U_VPSS_DBD_THDEDGE     VPSS_DBD_THDEDGE         ; /* 0x2550 */
    volatile U_VPSS_DBD_THDTXT      VPSS_DBD_THDTXT          ; /* 0x2554 */
    volatile U_VPSS_DBD_LUTWGT0     VPSS_DBD_LUTWGT0         ; /* 0x2558 */
    volatile U_VPSS_DBD_LUTWGT1     VPSS_DBD_LUTWGT1         ; /* 0x255c */
    volatile U_VPSS_DBD_RTNBDTXT    VPSS_DBD_RTNBDTXT        ; /* 0x2560 */
    volatile U_VPSS_DBD_TSTBLKNUM   VPSS_DBD_TSTBLKNUM       ; /* 0x2564 */
    volatile U_VPSS_DBD_HYRDX8BINLUT0   VPSS_DBD_HYRDX8BINLUT0 ; /* 0x2568 */
    volatile U_VPSS_DBD_HYRDX8BINLUT1   VPSS_DBD_HYRDX8BINLUT1 ; /* 0x256c */
    volatile U_VPSS_DBD_HYRDX8BINLUT2   VPSS_DBD_HYRDX8BINLUT2 ; /* 0x2570 */
    volatile U_VPSS_DBD_HYRDX8BINLUT3   VPSS_DBD_HYRDX8BINLUT3 ; /* 0x2574 */
    volatile U_VPSS_DBD_HCRDX8BINLUT0   VPSS_DBD_HCRDX8BINLUT0 ; /* 0x2578 */
    volatile U_VPSS_DBD_HCRDX8BINLUT1   VPSS_DBD_HCRDX8BINLUT1 ; /* 0x257c */
    volatile U_VPSS_DBD_HCRDX8BINLUT2   VPSS_DBD_HCRDX8BINLUT2 ; /* 0x2580 */
    volatile U_VPSS_DBD_HCRDX8BINLUT3   VPSS_DBD_HCRDX8BINLUT3 ; /* 0x2584 */
    volatile U_VPSS_DBD_VYRDX8BINLUT0   VPSS_DBD_VYRDX8BINLUT0 ; /* 0x2588 */
    volatile U_VPSS_DBD_VYRDX8BINLUT1   VPSS_DBD_VYRDX8BINLUT1 ; /* 0x258c */
    volatile U_VPSS_DBD_VYRDX8BINLUT2   VPSS_DBD_VYRDX8BINLUT2 ; /* 0x2590 */
    volatile U_VPSS_DBD_VYRDX8BINLUT3   VPSS_DBD_VYRDX8BINLUT3 ; /* 0x2594 */
    volatile U_VPSS_DBD_BLKSIZE     VPSS_DBD_BLKSIZE         ; /* 0x2598 */
    volatile unsigned int           reserved_21[25]                     ; /* 0x259c~0x25fc */
    volatile unsigned int           VPSS_DB_BORD_FLAG[128]   ; /* 0x2600~0x27fc */
    volatile unsigned int           reserved_22[1536]                     ; /* 0x2800~0x3ffc */
    volatile unsigned int           VPSS_DEBUG0              ; /* 0x4000 */
    volatile unsigned int           VPSS_DEBUG1              ; /* 0x4004 */
    volatile unsigned int           VPSS_DEBUG2              ; /* 0x4008 */
    volatile unsigned int           VPSS_DEBUG3              ; /* 0x400c */
    volatile unsigned int           VPSS_DEBUG4              ; /* 0x4010 */
    volatile unsigned int           VPSS_DEBUG5              ; /* 0x4014 */
    volatile unsigned int           VPSS_DEBUG6              ; /* 0x4018 */
    volatile unsigned int           VPSS_DEBUG7              ; /* 0x401c */
    volatile unsigned int           VPSS_DEBUG8              ; /* 0x4020 */
    volatile unsigned int           VPSS_DEBUG9              ; /* 0x4024 */
    volatile unsigned int           VPSS_DEBUG10             ; /* 0x4028 */
    volatile unsigned int           VPSS_DEBUG11             ; /* 0x402c */
    volatile unsigned int           VPSS_DEBUG12             ; /* 0x4030 */
    volatile unsigned int           VPSS_DEBUG13             ; /* 0x4034 */
    volatile unsigned int           VPSS_DEBUG14             ; /* 0x4038 */
    volatile unsigned int           VPSS_DEBUG15             ; /* 0x403c */
    volatile unsigned int           VPSS_DEBUG16             ; /* 0x4040 */
    volatile unsigned int           VPSS_DEBUG17             ; /* 0x4044 */
    volatile unsigned int           VPSS_DEBUG18             ; /* 0x4048 */
    volatile unsigned int           VPSS_DEBUG19             ; /* 0x404c */
    volatile unsigned int           VPSS_DEBUG20             ; /* 0x4050 */
    volatile unsigned int           VPSS_DEBUG21             ; /* 0x4054 */
    volatile unsigned int           VPSS_DEBUG22             ; /* 0x4058 */
    volatile unsigned int           VPSS_DEBUG23             ; /* 0x405c */
    volatile unsigned int           VPSS_DEBUG24             ; /* 0x4060 */
    volatile unsigned int           VPSS_DEBUG25             ; /* 0x4064 */
    volatile unsigned int           VPSS_DEBUG26             ; /* 0x4068 */
    volatile unsigned int           VPSS_DEBUG27             ; /* 0x406c */
    volatile unsigned int           VPSS_DEBUG28             ; /* 0x4070 */
    volatile unsigned int           VPSS_DEBUG29             ; /* 0x4074 */
    volatile unsigned int           VPSS_DEBUG30             ; /* 0x4078 */
    volatile unsigned int           VPSS_DEBUG31             ; /* 0x407c */
    volatile unsigned int           VPSS_DEBUG32             ; /* 0x4080 */
    volatile unsigned int           VPSS_DEBUG33             ; /* 0x4084 */
    volatile unsigned int           VPSS_DEBUG34             ; /* 0x4088 */
    volatile unsigned int           VPSS_DEBUG35             ; /* 0x408c */
    volatile unsigned int           VPSS_DEBUG36             ; /* 0x4090 */
    volatile unsigned int           VPSS_DEBUG37             ; /* 0x4094 */
    volatile unsigned int           VPSS_DEBUG38             ; /* 0x4098 */
    volatile unsigned int           VPSS_DEBUG39             ; /* 0x409c */
    volatile unsigned int           VPSS_DEBUG40             ; /* 0x40a0 */
    volatile unsigned int           VPSS_DEBUG41             ; /* 0x40a4 */
    volatile unsigned int           VPSS_DEBUG42             ; /* 0x40a8 */
    volatile unsigned int           VPSS_DEBUG43             ; /* 0x40ac */

} VPSS_REG_S;
#endif

#endif
/* Declare the struct pointor of the module CAS */
//extern VPSS_REG_S *gopCASAllReg;

/*read/write reg*/
HI_S32 VPSS_REG_RegWrite(volatile HI_U32 *a, HI_U32 b);
HI_U32 VPSS_REG_RegRead(volatile HI_U32* a);

/*reset reg*/
HI_S32 VPSS_REG_ReSetCRG(HI_U32 u32Type);

HI_S32 VPSS_REG_BaseRegInit(VPSS_REG_S **ppstPhyReg);

HI_S32 VPSS_REG_AppRegInit(VPSS_REG_S **ppstAppReg,HI_U32 u32VirAddr);


//HI_S32 VPSS_REG_ResetAppReg(HI_U32 u32AppAddr, HI_U32 u32AppVirAddr);
HI_S32 VPSS_REG_ResetAppReg(HI_U32 u32AppAddr,VPSS_REG_S *pstPqReg);

/********************************/
HI_S32 VPSS_REG_SetIntMask(HI_U32 u32AppAddr,HI_U32 u32Mask);
HI_S32 VPSS_REG_GetIntMask(HI_U32 u32AppAddr,HI_U32 *pu32Mask);
HI_S32 VPSS_REG_GetIntState(HI_U32 u32AppAddr,HI_U32 *pu32Int);
HI_S32 VPSS_REG_GetRawIntState(HI_U32 u32AppAddr,HI_U32 *pu32RawInt);
HI_S32 VPSS_REG_ClearIntState(HI_U32 u32AppAddr,HI_U32 u32Data);
/********************************/

HI_S32 VPSS_REG_SetClockEn(HI_BOOL bClkEn);
HI_S32 VPSS_REG_CloseClock(HI_VOID);
HI_S32 VPSS_REG_OpenClock(HI_VOID);
HI_S32 VPSS_REG_GetClockState(HI_BOOL *pbOpened);

HI_S32 VPSS_REG_SetTimeOut(HI_U32 u32AppAddr,HI_U32 u32Data);

HI_S32 VPSS_REG_StartLogic(HI_U32 u32AppAddr,HI_U32 u32PhyAddr);

HI_S32 VPSS_REG_EnPort(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bEnable);




/**************   TUNL  ******************/

HI_S32 VPSS_REG_SetCurTunlAddr(HI_U32 u32AppAddr,REG_FRAMEPOS_E  ePort,HI_U32 u32TunlAddr);
HI_S32 VPSS_REG_SetCurTunlEn(HI_U32 u32AppAddr, REG_CHANELPOS_E ePort, HI_BOOL u32TunlEn);
HI_S32 VPSS_REG_SetCurTunlInterval(HI_U32 u32AppAddr,REG_FRAMEPOS_E ePort,HI_S32  s32CurTunlInterval);
HI_S32 VPSS_REG_SetTunlAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32TunlAddr);
HI_S32 VPSS_REG_SetTunlEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnTunl);
HI_S32 VPSS_REG_SetTunlFinishLine(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32FinishLine);
HI_S32 VPSS_REG_SetTunlMode(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, REG_TUNLPOS_E  s32TunlMode);
/********************************/
/************** Stt_W/R_Addr ******************/
HI_S32 VPSS_REG_SetSttWrAddr(HI_U32 u32AppAddr,HI_U32 u32SttWrAddr);
HI_S32 VPSS_REG_SetStWrAddr(HI_U32 u32AppAddr,HI_U32 u32Addr);
HI_S32 VPSS_REG_SetStRdAddr(HI_U32 u32AppAddr,HI_U32 u32Addr);
HI_S32 VPSS_REG_SetStStride(HI_U32 u32AppAddr,HI_U32 u32Stride);
HI_S32 VPSS_REG_SetDeiParaAddr(HI_U32 u32AppAddr,HI_U32 u32ParaPhyAddr);

/********************************/


/**************PORT  CONFIG******************/
HI_S32 VPSS_REG_SetPortMirrorEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bMirrorEn);
HI_S32 VPSS_REG_SetPortFlipEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bFlipEn);
HI_S32 VPSS_REG_SetPortPixBitW(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_DRV_PIXEL_BITWIDTH_E ePixBitW);
HI_S32 VPSS_REG_SetPortDitherEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bDitherEn);
HI_S32 VPSS_REG_SetPortUVInvert(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bUVInvert);
HI_S32 VPSS_REG_SetPortDitherMode(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, REG_DITHER_MODE_E eDitherMode);
HI_S32 VPSS_REG_SetPortLCmpEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bLCmpEn);
HI_S32 VPSS_REG_SetFrmSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetFrmAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Yaddr,HI_U32 u32Caddr);
HI_S32 VPSS_REG_SetFrmStride(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32YStride,HI_U32 u32CStride);
/********************************/


/**************IMAGE  CONFIG******************/
HI_S32 VPSS_REG_SetImgPackage(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 EnPackage);
HI_S32 VPSS_REG_SetImgTile(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 EnTile);
HI_S32 VPSS_REG_SetImgOffset(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32HorOffset,HI_U32 u32VerOffset);
HI_S32 VPSS_REG_SetImgAddr(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32Yaddr,HI_U32 u32Cbaddr,HI_U32 u32Craddr);
HI_S32 VPSS_REG_SetImgAddrLB(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32Yaddr,HI_U32 u32Cbaddr,HI_U32 u32Craddr);
HI_S32 VPSS_REG_SetImgStride(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32YStride,HI_U32 u32CStride);
HI_S32 VPSS_REG_SetImgStrideLB(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32YStride,HI_U32 u32CStride);
/********************************/

/**************MAC  CONFIG******************/
HI_S32 VPSS_REG_SetAxiID(HI_U32 u32AppAddr,REG_AXI_CHN_E eChn, HI_U32 u32AxiID);
HI_S32 VPSS_REG_SetAxiRID(HI_U32 u32AppAddr,REG_AXI_RID_CHN_E eChn, HI_U32 u32AxiRID);
HI_S32 VPSS_REG_SetNodeID(HI_U32 u32AppAddr, HI_U32 u32NodeID);
/********************************/
    
/**************DNR  CONFIG******************/
HI_S32 VPSS_REG_SetDnrInfo(HI_U32 u32AppAddr,HI_U32  u32Rcnt,HI_U32  u32Bcnt,HI_U32  u32MaxGrad,HI_U32  u32Cntrst8);
HI_S32 VPSS_REG_SetDNREn(HI_U32 u32AppAddr,HI_BOOL  bEnDR);
HI_S32 VPSS_REG_SetDNRDemoEn(HI_U32 u32AppAddr,HI_BOOL  bEnDRDemo);
HI_S32 VPSS_REG_SetDNRPara(HI_U32 u32AppAddr,HI_S32  s32FlatThd,HI_S32  s32SimiThd,HI_S32 s32AlphaScale,HI_S32 s32BetaScale);
/********************************/
        
/**************STREAM DETECTOR  CONFIG******************/
HI_VOID VPSS_REG_SetDetEn(HI_U32 u32AppAddr,HI_BOOL bEnable);
HI_VOID VPSS_REG_SetDetMode(HI_U32 u32AppAddr,HI_U32 u32Mode);
HI_VOID VPSS_REG_SetDetBlk(HI_U32 u32AppAddr,HI_U32 blk_id, HI_U32 *pu32Addr);
/********************************/
        
/**************LBA  CONFIG******************/
HI_S32 VPSS_REG_SetLBAEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bEnLba);
HI_S32 VPSS_REG_SetLBABg(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Color,HI_U32 u32Alpha);
HI_S32 VPSS_REG_SetLBADispPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetLBAVidPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);
/********************************/
        
/**************incrop  CONFIG******************/
HI_S32 VPSS_REG_SetOutCropVidPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                            HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetInCropEn(HI_U32 u32AppAddr,HI_BOOL bInCropEn);
HI_S32 VPSS_REG_SetInCropPos(HI_U32 u32AppAddr,HI_U32 u32InCropY,HI_U32 u32InCropX);
HI_S32 VPSS_REG_SetInCropSize(HI_U32 u32AppAddr,HI_U32 u32InCropHeight,HI_U32 u32InCropWidth);
/********************************/
        
/**************image  CONFIG******************/
HI_S32 VPSS_REG_SetImgSize(HI_U32 u32AppAddr,HI_U32 u32Width,HI_U32 u32Height,HI_BOOL bProgressive);
HI_S32 VPSS_REG_SetPortCropEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bPortCropEn);
HI_S32 VPSS_REG_SetPreHfirEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bHfirEn);
HI_S32 VPSS_REG_SetPreHfirMode(HI_U32 u32AppAddr, VPSS_REG_PORT_E ePort, HI_U32 u32HfirMode);

HI_S32 VPSS_REG_SetPreVfirEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bVfirEn);
HI_S32 VPSS_REG_SetPreVfirMode(HI_U32 u32AppAddr, VPSS_REG_PORT_E ePort, HI_U32 u32VfirMode);
/********************************/
        
/**************ctrl cfg******************/
HI_S32 VPSS_REG_SetZmeEn(HI_U32 u32AppAddr, VPSS_REG_PORT_E ePort, HI_U32 u32ZmeEn);
HI_S32 VPSS_REG_SetUVConvertEn(HI_U32 u32AppAddr,HI_U32 u32EnUV);
HI_S32 VPSS_REG_SetPixBitW(HI_U32 u32AppAddr,HI_DRV_PIXEL_BITWIDTH_E ePixBitW);
HI_S32 VPSS_REG_SetImgFormat(HI_U32 u32AppAddr,HI_DRV_PIX_FORMAT_E eFormat);
HI_S32 VPSS_REG_SetFrmFormat(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat);
HI_S32 VPSS_REG_SetImgReadMod(HI_U32 u32AppAddr,HI_BOOL bField);
HI_S32 VPSS_REG_SetDeiFieldMode(HI_U32 u32AppAddr,HI_BOOL bBottom);
HI_S32 VPSS_REG_SetDeiTopFirst(HI_U32 u32AppAddr,HI_BOOL bTopFirst);
HI_S32 VPSS_REG_SetRotation(HI_U32 u32AppAddr,HI_U32 u32Angle, HI_U32 u32ProMode);
HI_S32 VPSS_REG_SetiGlbEn(HI_U32 u32AppAddr,HI_U32 u32EniGlb);
HI_S32 VPSS_REG_SetiFmdEn(HI_U32 u32AppAddr,HI_U32 u32EniFmd);
HI_S32 VPSS_REG_SetProtEn(HI_U32 u32AppAddr,HI_U32 u32Prot);
HI_S32 VPSS_REG_SetTwoPix(HI_U32 u32AppAddr,HI_U32 u32En2Pix);
HI_S32 VPSS_REG_SetDrEn(HI_U32 u32AppAddr,HI_U32 u32EnDr);
HI_S32 VPSS_REG_SetDbEn(HI_U32 u32AppAddr,HI_U32 u32EnDb);
HI_S32 VPSS_REG_SetHspEn(HI_U32 u32AppAddr,HI_U32 u32EnHsp);
HI_S32 VPSS_REG_SetChkSumEn(HI_U32 u32AppAddr,HI_U32 u32EnCSum);
HI_S32 VPSS_REG_EnDei(HI_U32 u32AppAddr,HI_BOOL bEnDei);
HI_S32 VPSS_REG_SetVc1En(HI_U32 u32AppAddr,HI_U32 u32EnVc1);
HI_S32 VPSS_REG_EnPort(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bEnable);


/********************************/

//98m
HI_S32 VPSS_REG_StartLogic(HI_U32 u32AppAddr,HI_U32 u32PhyAddr);

HI_S32 VPSS_REG_SetZmeCoefAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_U32 u32Addr);

HI_S32 VPSS_REG_SetAddr(HI_U32 u32AppAddr,VPSS_REG_ADDR_E eType,HI_U32 u32Addr);

HI_S32 VPSS_REG_SetIntMask(HI_U32 u32AppAddr,HI_U32 u32Mask);

/* 98m */
HI_S32 VPSS_REG_GetIntState(HI_U32 u32AppAddr,HI_U32 *pu32Int);

/* 98m */
HI_S32 VPSS_REG_ClearIntState(HI_U32 u32AppAddr,HI_U32 u32Data);

/* 98m */
HI_S32 VPSS_REG_GetRawIntState(HI_U32 u32AppAddr,HI_U32 *pu32RawInt);

/*98m (98m读错)========VPSS_PFCNT*/
HI_S32 VPSS_REG_GetReg(HI_U32 u32AppAddr,HI_U32 *pu32Int);

/* 98m */
HI_S32 VPSS_REG_SetMisCellaneous(HI_U32 u32AppAddr,HI_U32 u32Value);

HI_S32 VPSS_REG_SetMacCfg(HI_U32 u32AppAddr,HI_U32 u32Value);
/* 98m */
 HI_S32 VPSS_REG_SetTimeOut(HI_U32 u32AppAddr,HI_U32 u32Data);
 HI_S32 VPSS_REG_GetEofcnt(HI_U32 u32AppAddr,HI_U32 *pu32Eofcnt);

HI_S32 VPSS_REG_GetVersion(HI_U32 u32AppAddr,HI_U32 *pu32Version);

/******************************* for cmp ***************************************/
/*0：有损定带宽模式   1：无损压缩模式*/
HI_S32 VPSS_REG_SetLossLess_Y(HI_U32 u32AppVAddr,HI_BOOL bIsLossLess);
/*0：有损定带宽模式   1：无损压缩模式*/
HI_S32 VPSS_REG_SetLossLess_C(HI_U32 u32AppVAddr,HI_BOOL bIsLossLess);

/*压缩模式，0: 压缩   1：raw模式*/
HI_S32 VPSS_REG_SetVhd0CmpMode_Y(HI_U32 u32AppVAddr,HI_BOOL bIsCmpRaw);

/*压缩模式，0: 压缩   1：raw模式*/
HI_S32 VPSS_REG_SetVhd0CmpMode_C(HI_U32 u32AppVAddr,HI_BOOL bIsCmpRaw);


/*输入像素精度，0: 10 bit   1: 8bit*/
HI_S32 VPSS_REG_SetVhd0DwMode_Y(HI_U32 u32AppVAddr,HI_BOOL bBitMode);

/*输入像素精度，0: 10 bit   1: 8bit*/
HI_S32 VPSS_REG_SetVhd0DwMode_C(HI_U32 u32AppVAddr,HI_BOOL bBitMode);


HI_S32 VPSS_REG_SetVhd0LcmpSize_Y(HI_U32 u32AppAddr,HI_U32 u32LcmpWidth,HI_U32 u32LcmpHeight);
HI_S32 VPSS_REG_SetVhd0LcmpSize_C(HI_U32 u32AppAddr,HI_U32 u32LcmpWidth,HI_U32 u32LcmpHeight);

/*================= VPSS_VHD0_LCMP_YRCCFG0 =============*/
HI_S32 VPSS_REG_SetVhd0LcmpMbBit_Y(HI_U32 u32AppAddr,HI_U32 mb_bits);

HI_S32 VPSS_REG_SetVhd0LcmpMbBit_C(HI_U32 u32AppAddr,HI_U32 mb_bits);
HI_S32 VPSS_REG_SetVhd0LcmpRiBit_Y(HI_U32 u32AppAddr,HI_U32 ri_bits);

HI_S32 VPSS_REG_SetVhd0LcmpRiBit_C(HI_U32 u32AppAddr,HI_U32 ri_bits);

HI_S32 VPSS_REG_SetVhd0LcmpMaxRg_Y(HI_U32 u32AppAddr,HI_U32 max_rg_comp_bits);
HI_S32 VPSS_REG_SetVhd0LcmpMaxRg_C(HI_U32 u32AppAddr,HI_U32 max_rg_comp_bits);

/*================= VPSS_VHD0_LCMP_YRCCFG1 =============*/
HI_S32 VPSS_REG_SetVhd0LcmpMaxQp_Y(HI_U32 u32AppAddr,HI_U32 max_qp);
HI_S32 VPSS_REG_SetVhd0LcmpMaxQp_C(HI_U32 u32AppAddr,HI_U32 max_qp);


HI_S32 VPSS_REG_SetVhd0LcmpSadBit_Y(HI_U32 u32AppAddr,HI_U32 sad_bits_ngain );
HI_S32 VPSS_REG_SetVhd0LcmpSadBit_C(HI_U32 u32AppAddr,HI_U32 sad_bits_ngain );
HI_S32 VPSS_REG_SetVhd0LcmpRcSmth_Y(HI_U32 u32AppAddr,HI_U32 rc_smth_ngain );
HI_S32 VPSS_REG_SetVhd0LcmpRcSmth_C(HI_U32 u32AppAddr,HI_U32 rc_smth_ngain );


/*================= VPSS_VHD0_LCMP_YRCCFG2 =============*/

HI_S32 VPSS_REG_SetVhd0LcmpSmthThr_Y(HI_U32 u32AppAddr,HI_U32 smth_thr );
HI_S32 VPSS_REG_SetVhd0LcmpSmthThr_C(HI_U32 u32AppAddr,HI_U32 smth_thr );


HI_S32 VPSS_REG_SetVhd0LcmpSmthPixNum_Y(HI_U32 u32AppAddr,HI_U32 smth_pix_num_thr);
HI_S32 VPSS_REG_SetVhd0LcmpSmthPixNum_C(HI_U32 u32AppAddr,HI_U32 smth_pix_num_thr );

HI_S32 VPSS_REG_SetVhd0LcmpPixDiff_Y(HI_U32 u32AppAddr,HI_U32 pix_diff_thr );
HI_S32 VPSS_REG_SetVhd0LcmpPixDiff_C(HI_U32 u32AppAddr,HI_U32 pix_diff_thr );

/*================= VPSS_VHD0_LCMP_YRCCFG3 =============*/


HI_S32 VPSS_REG_SetVhd0LcmpAdjSad_Y(HI_U32 u32AppAddr,HI_U32 adj_sad_thr );
HI_S32 VPSS_REG_SetVhd0LcmpAdjSad_C(HI_U32 u32AppAddr,HI_U32 adj_sad_thr );

HI_S32 VPSS_REG_SetVhd0LcmpAdjBit_Y(HI_U32 u32AppAddr,HI_U32 adj_sad_bit_thr );
HI_S32 VPSS_REG_SetVhd0LcmpAdjBit_C(HI_U32 u32AppAddr,HI_U32 adj_sad_bit_thr);

/*================= VPSS_VHD0_LCMP_YRCCFG4 =============*/

HI_S32 VPSS_REG_SetVhd0LcmpQpChg1_Y(HI_U32 u32AppAddr,HI_U32 qp_chg1_bits_thr );
HI_S32 VPSS_REG_SetVhd0LcmpQpChg1_C(HI_U32 u32AppAddr,HI_U32 qp_chg1_bits_thr);
HI_S32 VPSS_REG_SetVhd0LcmpQpChg2_Y(HI_U32 u32AppAddr,HI_U32 qp_chg2_bits_thr );
HI_S32 VPSS_REG_SetVhd0LcmpQpChg2_C(HI_U32 u32AppAddr,HI_U32 qp_chg2_bits_thr);

/*================= VPSS_VHD0_LCMP_YRCCFG5 =============*/

HI_S32 VPSS_REG_SetVhd0LcmpSmthQp0_Y(HI_U32 u32AppAddr,HI_U32 smth_qp0 );
HI_S32 VPSS_REG_SetVhd0LcmpSmthQp0_C(HI_U32 u32AppAddr,HI_U32 smth_qp0);



HI_S32 VPSS_REG_SetVhd0LcmpSmthQp1_Y(HI_U32 u32AppAddr,HI_U32 smth_qp1 );
HI_S32 VPSS_REG_SetVhd0LcmpSmthQp1_C(HI_U32 u32AppAddr,HI_U32 smth_qp1);

HI_S32 VPSS_REG_SetVhd0LcmpSmthThr0_Y(HI_U32 u32AppAddr,HI_U32 smth_lftbits_thr0 );
HI_S32 VPSS_REG_SetVhd0LcmpSmthThr0_C(HI_U32 u32AppAddr,HI_U32 smth_lftbits_thr0);
HI_S32 VPSS_REG_SetVhd0LcmpSmthThr1_Y(HI_U32 u32AppAddr,HI_U32 smth_lftbits_thr1 );
HI_S32 VPSS_REG_SetVhd0LcmpSmthThr1_C(HI_U32 u32AppAddr,HI_U32 smth_lftbits_thr1);
/*================= VPSS_VHD0_LCMP_YMAXLEN =============*/

HI_S32 VPSS_REG_SetVhd0LcmpMaxRowL_Y(HI_U32 u32AppAddr,HI_U32 max_row_len);


HI_S32 VPSS_REG_SetVhd0LcmpMaxRowL_C(HI_U32 u32AppAddr,HI_U32 max_row_len);
/******************************* for cmp ***************************************/


/* 98m DEI*/
HI_S32 VPSS_REG_EnDei(HI_U32 u32AppAddr,HI_BOOL bEnDei);

HI_S32 VPSS_REG_RstDei(HI_U32 u32AppAddr,HI_BOOL bRst,HI_U32 u32RstValue);

HI_S32 VPSS_REG_SetDeiTopFirst(HI_U32 u32AppAddr,HI_BOOL bTopFirst);
HI_S32 VPSS_REG_SetDeiFieldMode(HI_U32 u32AppAddr,HI_BOOL bBottom);
HI_S32 VPSS_REG_SetDeiAddr(HI_U32 u32AppAddr,REG_FRAMEPOS_E eField,HI_U32 u32YAddr,HI_U32 u32CAddr, HI_U32 u32CrAddr);
 
   
HI_S32 VPSS_REG_SetDeiStride(HI_U32 u32AppAddr,REG_FRAMEPOS_E eField,HI_U32 u32YStride,HI_U32 u32CStride);
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
HI_S32 VPSS_REG_SetStMode(HI_U32 u32AppVAddr,HI_BOOL bLumaMax,HI_BOOL bChromaMax);
HI_S32 VPSS_REG_SetChromeOffset(HI_U32 u32AppAddr,HI_U8 u8Offset);
HI_S32 VPSS_REG_SetMinIntern(HI_U32 u32AppAddr,HI_U32 u32MinIntern);
HI_S32 VPSS_REG_SetInternVer(HI_U32 u32AppAddr,HI_U32 u32InternVer);
HI_S32 VPSS_REG_SetRangeScale(HI_U32 u32AppAddr,HI_U32 u32Scale);
HI_S32 VPSS_REG_SetCK1(HI_U32 u32AppAddr,HI_U32 u32Gain,HI_U32 u32Range,HI_U32 u32Max);
HI_S32 VPSS_REG_SetCK2(HI_U32 u32AppAddr,HI_U32 u32Gain,HI_U32 u32Range,HI_U32 u32Max);
HI_S32 VPSS_REG_SetDIR(HI_U32 u32AppAddr,HI_S32 *ps32MultDir);


//==================================================new add
/*98m========================VPSS_DIEDIR0_3~ VPSS_DIEDIR12_14*/

HI_S32 VPSS_REG_SetDIR(HI_U32 u32AppAddr,HI_S32 *ps32MultDir);


/*98m no========================VPSS_DIESTA*/
HI_S32 VPSS_REG_GetDieCurState(HI_U32 u32AppAddr,HI_U32 *pcur_state);
HI_S32 VPSS_REG_GetDieCurCstate(HI_U32 u32AppAddr,HI_U32 *pcur_cstate);
HI_S32 VPSS_REG_GetDieHeightCnt(HI_U32 u32AppAddr,
											HI_U32 *pl_height_cnt,
											HI_U32 *pc_height_cnt );

/*98m no =================VPSS_DIESTPPREADDR VPSS_DIESTPREADDR*/
HI_S32 VPSS_REG_SetDieStpreAddr(HI_U32 u32AppAddr,
									HI_U32 VPSS_DIESTPPREADDR,
									HI_U32 VPSS_DIESTPREADDR);

/*98m========================VPSS_DIEINTPSCL0 VPSS_DIEINTPSCL1*/
HI_S32 VPSS_REG_SetIntpSclRat(HI_U32 u32AppAddr,HI_S32 *ps32Rat);


/*98m========================VPSS_DIEDIRTHD*/
HI_S32 VPSS_REG_SetStrenThd(HI_U32 u32AppAddr,HI_S32 s32Thd);
HI_S32 VPSS_REG_SetDirThd(HI_U32 u32AppAddr,HI_S32 s32Thd);
HI_S32 VPSS_REG_SetBcGain(HI_U32 u32AppAddr,HI_S32 s32BcGain);
HI_S32 VPSS_REG_EnHorEdge(HI_U32 u32AppAddr,HI_BOOL bEdgeEn);
HI_S32 VPSS_REG_EnEdgeMode(HI_U32 u32AppAddr,HI_BOOL bEdgeModeEn);


/*98m========================VPSS_DIEJITMTN*/
HI_S32 VPSS_REG_SetJitterCoring(HI_U32 u32AppAddr,HI_S32 s32Coring);
HI_S32 VPSS_REG_SetJitterGain(HI_U32 u32AppAddr,HI_S32 s32Gain);
HI_S32 VPSS_REG_SetMotionCoring(HI_U32 u32AppAddr,HI_S32 s32Coring);


/*98m========================VPSS_DIEFLDMTN*/
HI_S32 VPSS_REG_SetMotionSlope(HI_U32 u32AppAddr,HI_S32 s32Slope);
HI_S32 VPSS_REG_SetMotionGain(HI_U32 u32AppAddr,HI_S32 s32Gain);
HI_S32 VPSS_REG_SetMotionHThd(HI_U32 u32AppAddr,HI_S32 s32HThd);
HI_S32 VPSS_REG_SetMotionLThd(HI_U32 u32AppAddr,HI_S32 s32LThd);


/*98m========================VPSS_DIEMTNDIFFTHD0*/
HI_S32 VPSS_REG_SetMotionDiffThd(HI_U32 u32AppAddr,HI_S32 *ps32Thd);

/*98m========================VPSS_DIEMTNDIFFTHD1*/
HI_S32 VPSS_REG_SetMotionIIrRatioMaxMin(HI_U32 u32AppAddr,HI_S32 s32Max,HI_S32 s32Min);

/*98m========================VPSS_DIEMTNIIRSLP*/
HI_S32 VPSS_REG_SetMotionIIrSlope(HI_U32 u32AppAddr,HI_S32 *ps32Slope);

/*98m========================VPSS_DIEMTNIIRRAT*/
HI_S32 VPSS_REG_SetMotionIIrRatio(HI_U32 u32AppAddr,HI_S32 *ps32Ratio);


/*98m========================VPSS_DIEHISMODE*/
HI_S32 VPSS_REG_SetRecFldStep(HI_U32 u32AppAddr,HI_S32 *ps32Step);
HI_S32 VPSS_REG_SetRecFrmStep(HI_U32 u32AppAddr,HI_S32 *ps32Step);

HI_S32 VPSS_REG_SetHisEn(HI_U32 u32AppAddr,HI_BOOL bEnHis);
HI_S32 VPSS_REG_SetHisInfoWrMode(HI_U32 u32AppAddr,HI_BOOL bHisMtnInfoWrMd);
HI_S32 VPSS_REG_SetHisWrMode(HI_U32 u32AppAddr,HI_BOOL bHisMtnWrMd);

HI_S32 VPSS_REG_SetHisUseMode(HI_U32 u32AppAddr,HI_BOOL bHisMtnUseMd);
HI_S32 VPSS_REG_SetHisPreEn(HI_U32 u32AppAddr,HI_BOOL bEnPre);
HI_S32 VPSS_REG_SetHisPpreEn(HI_U32 u32AppAddr,HI_BOOL bEnPpre);


/*98m========================VPSS_DIEMORFLT*/
HI_S32 VPSS_REG_SetMorFlt(HI_U32 u32AppAddr,HI_BOOL bEnflt,HI_S8 s8FltSize,HI_S8 s8FltThd);
HI_S32 VPSS_REG_SetBlendEn(HI_U32 u32AppAddr,HI_BOOL bEnBlend);


/*98m========================VPSS_DIECOMBCHK0*/
HI_S32 VPSS_REG_SetCombLimit(HI_U32 u32AppAddr,HI_S32  s32UpLimit,HI_S32  s32LowLimit);
HI_S32 VPSS_REG_SetCombThd(HI_U32 u32AppAddr,HI_S32  s32Hthd,HI_S32  s32Vthd);


/*98m========================VPSS_DIECOMBCHK1*/
HI_S32 VPSS_REG_SetCombEn(HI_U32 u32AppAddr,HI_BOOL bEnComb);
HI_S32 VPSS_REG_SetCombMdThd(HI_U32 u32AppAddr,HI_S32 s32MdThd);
HI_S32 VPSS_REG_SetCombEdgeThd(HI_U32 u32AppAddr,HI_S32 s32EdgeThd);



/*98m========================VPSS_DIEFRMMTNSMTHTHD0*/
HI_S32 VPSS_REG_SetSmoothDiffThd(HI_U32 u32AppAddr,HI_S32 *ps32Thd);

/*98m========================VPSS_DIEFRMMTNSMTHTHD1*/
HI_S32 VPSS_REG_SetSmoothRatioMaxMin(HI_U32 u32AppAddr,HI_S32 s32Max,HI_S32 s32Min);


/*98m========================VPSS_DIEFRMMTNSMTHSLP*/
HI_S32 VPSS_REG_SetSmoothSlope(HI_U32 u32AppAddr,HI_S32 *ps32Slope);


/*98m========================VPSS_DIEFRMMTNSMTHRAT*/
HI_S32 VPSS_REG_SetSmoothRatio(HI_U32 u32AppAddr,HI_S32 *ps32Ratio);


/*98m========================VPSS_DIEFRMFLDBLENDTHD0*/
HI_S32 VPSS_REG_SetBlendDiffThd(HI_U32 u32AppAddr,HI_S32 *ps32Thd);

/*98m========================VPSS_DIEFRMFLDBLENDTHD1*/
HI_S32 VPSS_REG_SetBlendRatioMaxMin(HI_U32 u32AppAddr,HI_S32 s32Max,HI_S32 s32Min);

/*98m========================VPSS_DIEMTNADJ*/
HI_S32 VPSS_REG_SetMotionAdj(HI_U32 u32AppAddr,HI_S32 s32Gain,HI_S32 s32Chom,HI_S32 s32Coring);

/*98m========================VPSS_DIEFRMFLDBLENDSLP*/

HI_S32 VPSS_REG_SetBlendSlope(HI_U32 u32AppAddr,HI_S32 *ps32Slope);
/*98m========================VPSS_DIEFRMFLDBLENDRAT*/

HI_S32 VPSS_REG_SetBlendRatio(HI_U32 u32AppAddr,HI_S32 *ps32Ratio);

/*98m no =================VPSS_DIEGLBMTNTHD0 VPSS_DIEGLBMTNTHD1*/
HI_S32 VPSS_REG_SetGlbMtnthd(HI_U32 u32AppAddr,
									HI_U32  motion_thd0,
									HI_U32 motion_thd1,
									HI_U32  motion_thd2,
									HI_U32 motion_thd3);
//==================================================new add end







HI_S32 VPSS_REG_SetCcEn(HI_U32 u32AppAddr,HI_BOOL bEnCc);
HI_S32 VPSS_REG_SetCcOffset(HI_U32 u32AppAddr,HI_S32 s32Offset);
HI_S32 VPSS_REG_SetCcDetMax(HI_U32 u32AppAddr,HI_S32 s32Max);
HI_S32 VPSS_REG_SetCcDetThd(HI_U32 u32AppAddr,HI_S32 s32Thd);
HI_S32 VPSS_REG_SetSimiMax(HI_U32 u32AppAddr,HI_S32 s32SimiMax);
HI_S32 VPSS_REG_SetSimiThd(HI_U32 u32AppAddr,HI_S32 s32SimiThd);
HI_S32 VPSS_REG_SetDetBlend(HI_U32 u32AppAddr,HI_S32 s32DetBlend);
HI_S32 VPSS_REG_SetMaxXChroma(HI_U32 u32AppAddr,HI_S32 s32Max);
HI_S32 VPSS_REG_SetDeiHistThd(HI_U32 u32AppAddr,HI_S8 *pu8Data);
HI_S32 VPSS_REG_SetDeiMovCoring(HI_U32 u32AppAddr,HI_S8 s8Blk,HI_S8 s8Norm,HI_S8 s8Tkr);

/*98m ====================VPSS_PDICHD====================*/
HI_S32 VPSS_REG_GetPdIchd(HI_U32 u32AppAddr,HI_S32 *pstData);
/*98m ====================VPSS_PDCTRL====================*/
HI_S32 VPSS_REG_SetDeiEdgeSmoothRatio(HI_U32 u32AppAddr,HI_S8 u8Data);

HI_S32 VPSS_REG_SetDeiPdBitMove(HI_U32 u32AppAddr,HI_S32  s32Data);

HI_S32 VPSS_REG_SetDeiDirMch(HI_U32 u32AppAddr,HI_BOOL  bNext);
HI_S32 VPSS_REG_SetEdgeSmooth(HI_U32 u32AppAddr,HI_BOOL  bEdgeSmooth);

HI_S32 VPSS_REG_SetLasiMode(HI_U32 u32AppAddr,HI_U32  lasi_mode );

/*98m 错=============VPSS_PDBLKTHD*/
HI_S32 VPSS_REG_SetDeiStillBlkThd(HI_U32 u32AppAddr,HI_S8 stillblk_thd);
HI_S32 VPSS_REG_SetDeiDiffMovThd(HI_U32 u32AppAddr,HI_S8 diff_movblk_thd);

/*=======================VPSS_PDHISTTHD*/
HI_S32 VPSS_REG_SetMonTkrThd(HI_U32 u32AppAddr,HI_U32 mon_tkr_thr);
HI_S32 VPSS_REG_SetMonStartIdx(HI_U32 u32AppAddr,HI_U32 mon_start_idx );
/*98m =======================VPSS_PDUMTHD*/
HI_S32 VPSS_REG_SetDeiUmThd(HI_U32 u32AppAddr,HI_S8 *pu8Data);
/*98m =======================VPSS_PDPCCCORING*/
HI_S32 VPSS_REG_SetDeiCoring(HI_U32 u32AppAddr,HI_S8 s8CorBlk,HI_S8 s8CorNorm,HI_S8 s8CorTkr);
/*98m =======================VPSS_PDPCCHTHD*/
HI_S32 VPSS_REG_SetDeiPccHThd(HI_U32 u32AppAddr,HI_S8 s8Data);
/*98m =======================VPSS_PDPCCVTHD*/
HI_S32 VPSS_REG_SetDeiPccVThd(HI_U32 u32AppAddr,HI_S8 *ps8Data);
/*98m =======================VPSS_PDITDIFFVTHD*/
HI_S32 VPSS_REG_SetDeiItDiff(HI_U32 u32AppAddr,HI_S8 *ps8Data);

/*98m =======================VPSS_PDLASITHD*/
HI_S32 VPSS_REG_SetDeiLasiCtrl(HI_U32 u32AppAddr,HI_S8 s8Thr,HI_S8 s8EdgeThr,HI_S8 s8lasiThr);

/* =======================VPSS_PDLASIMODE0COEF*/
HI_S32 VPSS_REG_SetPdLasiMode0(HI_U32 u32AppAddr,HI_S8 lasi_txt_alpha,HI_S8 lasi_txt_coring );
/* =======================VPSS_PDLASIMODE1COEF*/


HI_S32 VPSS_REG_SetPdLasiMode1(HI_U32 u32AppAddr,HI_S8 *ps8Data );


/* =======================VPSS_PDREGION*/
HI_S32 VPSS_REG_SetPdRegion(HI_U32 u32AppAddr,HI_U32 region1_y_end, HI_U32  region1_y_stt);



/* =======================ZME CFG==============================================*/
HI_S32 VPSS_REG_SetZmeOutSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width);


/*=======================VPSS_HSPCFG*/

HI_S32 VPSS_REG_SetHspCfg1(HI_U32 u32AppAddr,HI_S8 const *pData );

HI_S32 VPSS_REG_SetHspCfg2(HI_U32 u32AppAddr,HI_U32 hsp_hf0_coring );
HI_S32 VPSS_RSEG_SetHspCfg3(HI_U32 u32AppAddr,
										HI_S32 hsp_hf0_gainpos,
										HI_S32 hsp_hf0_gainneg  );

HI_S32 VPSS_RSEG_SetHspCfg4(HI_U32 u32AppAddr,
						HI_U8 hsp_hf0_overth,
						HI_U8 hsp_hf0_underth,
						HI_U8 hsp_hf0_mixratio,
						HI_U8 hsp_hf0_winsize,
						HI_BOOL hsp_hf0_adpshoot_en);
HI_S32 VPSS_REG_SetHspCfg5(HI_U32 u32AppAddr,HI_S8 const *pData );


HI_S32 VPSS_REG_SetHspCfg6(HI_U32 u32AppAddr,HI_U32 hsp_hf1_coring );

HI_S32 VPSS_REG_SetHspCfg7(HI_U32 u32AppAddr,
										HI_S32 hsp_hf1_gainpos,
										HI_S32 hsp_hf1_gainneg );

HI_S32 VPSS_RSEG_SetHspCfg8(HI_U32 u32AppAddr,
						HI_U8 hsp_hf1_overth,
						HI_U8 hsp_hf1_underth,
						HI_U8 hsp_hf1_mixratio,
						HI_U8 hsp_hf1_winsize,
						HI_BOOL hsp_hf1_adpshoot_en);
HI_S32 VPSS_RSEG_SetHspCfg9(HI_U32 u32AppAddr,
						HI_U8	hsp_cdti_gain,
						HI_U8	 hsp_ldti_gain,
						HI_U8	 hsp_lti_ratio,
						HI_U8	 hsp_hf_shootdiv);
HI_S32 VPSS_RSEG_SetHspCfg9Ct_En(HI_U32 u32AppAddr,HI_BOOL hsp_ctih_en);
HI_S32 VPSS_RSEG_SetHspCfg9Lt_En(HI_U32 u32AppAddr,HI_BOOL hsp_ltih_en);
HI_S32 VPSS_RSEG_SetHspCfg9H1_En(HI_U32 u32AppAddr,HI_BOOL hsp_h1_en);

HI_S32 VPSS_RSEG_SetHspCfg9H0_En(HI_U32 u32AppAddr,HI_BOOL hsp_h0_en);
HI_S32 VPSS_RSEG_SetHspCfg10(HI_U32 u32AppAddr,
						HI_U32 hsp_glb_underth,
						HI_U32 hsp_glb_overth,
						HI_U32 hsp_mk_mode,
						HI_U32 hsp_peak_ratio,
						HI_BOOL hsp_mk_en);

/*=======================DR  CFG===========================*/
HI_S32 VPSS_REG_SetDREn(HI_U32 u32AppAddr, HI_BOOL bEnDR);
HI_S32 VPSS_REG_SetDRDemoEn(HI_U32 u32AppAddr, HI_BOOL bEnDRDemo);
HI_S32 VPSS_REG_SetDRThl(HI_U32 u32AppAddr, HI_U8 u8thrmdfangctrst, HI_U8 u8throvshtminang,
                         HI_U8 u8drstrenth, HI_U8 u8ovtstrenth);
HI_S32 VPSS_REG_SetDRLSWStrLUT0(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLSWStrLUT1(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLSWStrLUT2(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLSWStrLUT3(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLSWStrLUT4(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLSWStrLUT5(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLSWStrLUT6(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLSWStrLUT7(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLSWStrLUT8(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLRWStrLUT0(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLRWStrLUT1(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLRWStrLUT2(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLRWStrLUT3(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLRWStrLUT4(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLRWStrLUT5(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLRWStrLUT6(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLRWStrLUT7(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLRWStrLUT8(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLSWAdjustLUT(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRLRWAdjustLUT(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRAngAdjustLUT(HI_U32 u32AppAddr, HI_U8 const* pu8Lut);
HI_S32 VPSS_REG_SetDRAngDiffLUT(HI_U32 u32AppAddr, HI_U8 const* pu8DrAngDiffLUT);
HI_S32 VPSS_REG_SetDRAngMinLUT(HI_U32 u32AppAddr, HI_U8 const* pu8DrAngMinLUT);
HI_S32 VPSS_REG_SetDROvtStrAdjustLut(HI_U32 u32AppAddr, HI_U8 const* pu8DrOvtAdjustLUT);
HI_S32 VPSS_REG_SetDROvtStrLut(HI_U32 u32AppAddr, HI_U8 const* pu8DrOvtStrLUT);
HI_S32 VPSS_REG_SetDRPara(HI_U32 u32AppAddr, HI_S32 s32FlatThd, HI_S32 s32SimiThd,
                          HI_S32 s32AlphaScale, HI_S32 s32BetaScale);





/*=======================VC1  CFG===========================*/
HI_S32 VPSS_REG_SetVc1Profile(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32Profile);
HI_S32 VPSS_REG_SetVc1Map(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32MapY,HI_U32 u32MapC,
                            HI_U32 u32BMapY,HI_U32 u32BMapC);
HI_S32 VPSS_REG_SetVc1MapFlag(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32YFlag,HI_U32 u32CFlag,
                            HI_U32 u32BYFlag,HI_U32 u32BCFlag);
HI_S32 VPSS_REG_SetVc1RangeEn(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32EnVc1);



/*=======================DB  CFG===========================*/
HI_S32 VPSS_REG_SetDBEn(HI_U32 u32AppAddr, HI_BOOL bEnDB);
HI_S32 VPSS_REG_SetDBDemoEn(HI_U32 u32AppAddr, HI_BOOL bEnDBDemo);
HI_S32 VPSS_REG_SetDBBlk(HI_U32 u32AppAddr, HI_U8 u8LumHSize, HI_U8 u8LumVSize,
                         HI_U8 u8ChrHSize, HI_U8 u8ChrVSize);
HI_S32 VPSS_REG_SetDBRatio(HI_U32 u32AppAddr, HI_U8 u8LumHRatio, HI_U8 u8LumVRatio,
                           HI_U8 u8ChrHRatio, HI_U8 u8ChrVRatio);
HI_S32 VPSS_REG_SetDBHorFilterSel(HI_U32 u32AppAddr, HI_U8 u8LumHFilterSel, HI_U8 u8ChrHFilterSel);
HI_S32 VPSS_REG_SetDBGlabalStrenth(HI_U32 u32AppAddr, HI_U8 u8LumStrenth, HI_U8 u8ChrStrenth);
HI_S32 VPSS_REG_SetDBLumHTxtWinSize(HI_U32 u32AppAddr, HI_U8 u8Size);
HI_S32 VPSS_REG_SetDBLHHFGain(HI_U32 u32AppAddr, HI_U8 u8DiffGain1, HI_U8 u8DiffGain2,
                              HI_U8 u8VarGain1, HI_U8 u8VarGain2, HI_U8 u8BoadAdjGain,
                              HI_U8 u8HorAdjGain);
HI_S32 VPSS_REG_SetDBLHHFCore(HI_U32 u32AppAddr, HI_U8 u8DiffCore, HI_U8 u8VarCore);
HI_S32 VPSS_REG_SetDBLVHFGain(HI_U32 u32AppAddr, HI_U8 u8VarGain1, HI_U8 u8VarGain2, HI_U8 u8BoadAdjGain, HI_U8 u8VerAdjGain);
HI_S32 VPSS_REG_SetDBLVHFCore(HI_U32 u32AppAddr, HI_U8 u8VarCore);
HI_S32 VPSS_REG_SetDBCtrsDirSmoothMode(HI_U32 u32AppAddr, HI_U8 u8Mode);
HI_S32 VPSS_REG_SetDBCtrsGain(HI_U32 u32AppAddr, HI_U8 u8Gain1, HI_U8 u8Gain2);
HI_S32 VPSS_REG_SetDBLumHorDeltaLut(HI_U32 u32AppAddr, HI_U8 const* pu8LUT);
HI_S32 VPSS_REG_SetDBLumVerDeltaLut(HI_U32 u32AppAddr, HI_U8 const* pu8LUT);
HI_S32 VPSS_REG_SetDBChrHorDeltaLut(HI_U32 u32AppAddr, HI_U8 const* pu8LUT);
HI_S32 VPSS_REG_SetDBLumHStrFadeLut(HI_U32 u32AppAddr, HI_U8 const* pu8LUT);
HI_S32 VPSS_REG_SetDBLumVStrFadeLut(HI_U32 u32AppAddr, HI_U8 const* pu8LUT);
HI_S32 VPSS_REG_SetDBChrHStrFadeLut(HI_U32 u32AppAddr, HI_U8 const* pu8LUT);
HI_S32 VPSS_REG_SetDBDirStrGainLut(HI_U32 u32AppAddr, HI_U8 const* pu8LUT);
HI_S32 VPSS_REG_SetDBThdEdge(HI_U32 u32AppAddr, HI_U8 u8HY, HI_U8 u8HC, HI_U8 u8VY);
HI_S32 VPSS_REG_SetDBThdTxt(HI_U32 u32AppAddr, HI_U8 u8HY, HI_U8 u8HC, HI_U8 u8VY);
HI_S32 VPSS_REG_SetDBDLutWgt(HI_U32 u32AppAddr, HI_U8 const* pu8LUT);
HI_S32 VPSS_REG_SetDBDRtnBdTxt(HI_U32 u32AppAddr, HI_U8 u8HY, HI_U8 u8HC, HI_U8 u8VY);
HI_S32 VPSS_REG_SetDBDTstBlkNum(HI_U32 u32AppAddr, HI_U8 u8HY, HI_U8 u8HC, HI_U8 u8VY);
HI_S32 VPSS_REG_SetDBDHyRdxLut(HI_U32 u32AppAddr, HI_U16 const* pu16LUT);
HI_S32 VPSS_REG_SetDBDHcRdxLut(HI_U32 u32AppAddr, HI_U16 const* pu16LUT);
HI_S32 VPSS_REG_SetDBDVyRdxLut(HI_U32 u32AppAddr, HI_U16 const* pu16LUT);
HI_S32 VPSS_REG_SetDBMinBlkSize(HI_U32 u32AppAddr, HI_U8 u8Size);
HI_S32 VPSS_REG_SetDBHyMaxBlkSize(HI_U32 u32AppAddr, HI_U8 u8Size);
HI_S32 VPSS_REG_SetDBHcMaxBlkSize(HI_U32 u32AppAddr, HI_U8 u8Size);
HI_S32 VPSS_REG_SetDBVyMaxBlkSize(HI_U32 u32AppAddr, HI_U8 u8Size);



//=============================== READ SST ====================================
HI_VOID VPSS_REG_GetDetPixel(HI_U32 u32AppAddr,HI_U32 BlkNum, HI_U8* pstData);
HI_S32 VPSS_REG_GetHisBin(HI_U32 u32AppAddr,HI_S32 *pstData);
HI_S32 VPSS_REG_GetItDiff(HI_U32 u32AppAddr,HI_S32 *pstData);
HI_S32 VPSS_REG_GetPccData(HI_U32 u32AppAddr,HI_S32 *ps32FFWD,
                             HI_S32 *ps32FWD,HI_S32 *ps32BWD,HI_S32 *ps32CRSS,
                             /*HI_S32 *ps32PW,*/HI_S32 *ps32FWDTKR,HI_S32 *ps32WDTKR);
HI_S32 VPSS_REG_GetPdMatch(HI_U32 u32AppAddr,
                            HI_S32 *ps32Match0,HI_S32 *ps32UnMatch0,
                            HI_S32 *ps32Match1,HI_S32 *ps32UnMatch1);
HI_S32 VPSS_REG_GetLasiCnt(HI_U32 u32AppAddr,
                            HI_S32 *ps32Cnt14,HI_S32 *ps32Cnt32,
                            HI_S32 *ps32Cnt34);
HI_S32 VPSS_REG_GetBlkSad(HI_U32 u32AppAddr,HI_S32 *pstData);
HI_S32 VPSS_REG_GetDbHyCnt(HI_U32 u32AppAddr,HI_U32 *pu32DbHyCnt);
HI_S32 VPSS_REG_GetDbBorder(HI_U32 u32AppAddr,HI_U32 *pu32DbBorder);
HI_S32 VPSS_REG_GetDbHySizeHyCnt(HI_U32 u32AppAddr,HI_U32 *pu32DbHySizeHyCounter);
HI_S32 VPSS_REG_GetDbHySizeHySize(HI_U32 u32AppAddr,HI_U32 *pu32DbHySizeHySize);
HI_S32 VPSS_REG_GetDbHcSizeHcCnt(HI_U32 u32AppAddr,HI_U32 *pu32DbHcSizeHcCounter);
HI_S32 VPSS_REG_GetDbHcSizeHcSize(HI_U32 u32AppAddr,HI_U32 *pu32DbHcSizeHcSize);
HI_S32 VPSS_REG_GetDbVySizeVyCnt(HI_U32 u32AppAddr,HI_U32 *pu32DbVySizeVyCounter);
HI_S32 VPSS_REG_GetDbVySizeVySize(HI_U32 u32AppAddr,HI_U32 *pu32DbVySizeVySize);

HI_S32 VPSS_REG_SetPreZme(HI_U32 u32AppVAddr,VPSS_REG_PORT_E ePort,
                            VPSS_REG_PREZME_E enHor,VPSS_REG_PREZME_E enVer);


HI_S32 VPSS_REG_SetVc1ParaAddr(HI_U32 u32AppAddr,HI_U32 u32ParaPhyAddr);
HI_S32 VPSS_REG_SetZmeParaAddr(HI_U32 u32AppAddr,HI_U32 u32ParaPhyAddr);
HI_S32 VPSS_REG_SetHspParaAddr(HI_U32 u32AppAddr,HI_U32 u32ParaPhyAddr);
HI_S32 VPSS_REG_SetDbParaAddr(HI_U32 u32AppAddr,HI_U32 u32ParaPhyAddr);
HI_S32 VPSS_REG_SetDrParaAddr(HI_U32 u32AppAddr,HI_U32 u32ParaPhyAddr);

HI_S32 VPSS_REG_SetGlobalMotionEn(HI_U32 u32AppAddr,HI_BOOL bEnGlobal);
HI_S32 VPSS_REG_SetIfmdEn(HI_U32 u32AppAddr,HI_BOOL bEnIfmd);

HI_S32 VPSS_REG_SetDnrDitherEn(HI_U32 u32AppAddr,HI_BOOL bEnDnrDither);

#endif /* __VPSS_REG_HIFONEB02__ */
