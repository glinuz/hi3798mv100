//******************************************************************************
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co., Ltd.
// File name     :  hi_reg_vpss.h
// Author        :  x57490
// Version       :  1.0
// Date          :  2015-08-28
// Description   :  Define all registers/tables for S40V200
// Others        :  Generated automatically by nManager V4.0
// History       :  x57490 2015-08-28 Create file
//******************************************************************************

#ifndef __HI_REG_VPSS_H__
#define __HI_REG_VPSS_H__

/* Define the union U_VPSS_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 3   ; /* [2..0]  */
        unsigned int    vhd0_en               : 1   ; /* [3]  */
        unsigned int    reserved_1            : 2   ; /* [5..4]  */
        unsigned int    chk_sum_en            : 1   ; /* [6]  */
        unsigned int    dei_en                : 1   ; /* [7]  */
        unsigned int    mcdi_en               : 1   ; /* [8]  */
        unsigned int    meds_en               : 1   ; /* [9]  */
        unsigned int    vc1_en                : 1   ; /* [10]  */
        unsigned int    dbm_en                : 1   ; /* [11]  */
        unsigned int    pgbm_en               : 1   ; /* [12]  */
        unsigned int    blk_det_en            : 1   ; /* [13]  */
        unsigned int    str_det_en            : 1   ; /* [14]  */
        unsigned int    reserved_2            : 2   ; /* [16..15]  */
        unsigned int    snr_en                : 1   ; /* [17]  */
        unsigned int    tnr_en                : 1   ; /* [18]  */
        unsigned int    rfr_en                : 1   ; /* [19]  */
        unsigned int    four_pix_en           : 1   ; /* [20]  */
        unsigned int    rotate_angle          : 1   ; /* [21]  */
        unsigned int    rotate_en             : 1   ; /* [22]  */
        unsigned int    prot                  : 2   ; /* [24..23]  */
        unsigned int    ifmd_en               : 1   ; /* [25]  */
        unsigned int    igbm_en               : 1   ; /* [26]  */
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
        unsigned int    reserved_0            : 12  ; /* [11..0]  */
        unsigned int    vhd0_format           : 4   ; /* [15..12]  */
        unsigned int    in_format             : 5   ; /* [20..16]  */
        unsigned int    in_pix_bitw           : 1   ; /* [21]  */
        unsigned int    ref_nxt_pix_bitw      : 1   ; /* [22]  */
        unsigned int    rfr_pix_bitw          : 1   ; /* [23]  */
        unsigned int    uv_invert             : 1   ; /* [24]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
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
        unsigned int    reserved_0            : 3   ; /* [2..0]  */
        unsigned int    zme_vhd0_en           : 1   ; /* [3]  */
        unsigned int    vhd0_pre_vfir_mode    : 2   ; /* [5..4]  */
        unsigned int    vhd0_pre_vfir_en      : 1   ; /* [6]  */
        unsigned int    reserved_1            : 9   ; /* [15..7]  */
        unsigned int    in_crop_en            : 1   ; /* [16]  */
        unsigned int    vhd0_crop_en          : 1   ; /* [17]  */
        unsigned int    reserved_2            : 2   ; /* [19..18]  */
        unsigned int    vhd0_lba_en           : 1   ; /* [20]  */
        unsigned int    tnr_mode              : 1   ; /* [21]  */
        unsigned int    tnr_mad_mode          : 1   ; /* [22]  */
        unsigned int    reserved_3            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CTRL3;

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

/* Define the union U_VPSS_VHD0LBA_BK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0_vbk_cr           : 10  ; /* [9..0]  */
        unsigned int    vhd0_vbk_cb           : 10  ; /* [19..10]  */
        unsigned int    vhd0_vbk_y            : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
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

/* Define the union U_VPSS_TESTPATTERN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    test_pat_en           : 1   ; /* [0]  */
        unsigned int    pat_angle             : 1   ; /* [1]  */
        unsigned int    reserved_0            : 6   ; /* [7..2]  */
        unsigned int    pat_dis_width         : 8   ; /* [15..8]  */
        unsigned int    pat_bkgrnd            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TESTPATTERN;

/* Define the union U_VPSS_NX2_HEAD_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nx2_head_stride       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NX2_HEAD_STRIDE;

/* Define the union U_VPSS_VDH_DB_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vdh_db_stride         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 15  ; /* [30..16]  */
        unsigned int    vdh_db_en             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VDH_DB_CTRL;

/* Define the union U_VPSS_VDH_DB_IMG_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vdh_db_width          : 16  ; /* [15..0]  */
        unsigned int    vdh_db_height         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VDH_DB_IMG_SIZE;

/* Define the union U_VPSS_TRANS_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    trans_size            : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 15  ; /* [30..16]  */
        unsigned int    trans_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TRANS_INFO;

/* Define the union U_VPSS_NX2_LB_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nx2lb_stride          : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NX2_LB_STRIDE;

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
        unsigned int    dcmp_err_mask         : 1   ; /* [5]  */
        unsigned int    bus_r_err_mask        : 1   ; /* [6]  */
        unsigned int    ip_used_mask          : 1   ; /* [7]  */
        unsigned int    ip_using_err          : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
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
        unsigned int    reserved_1            : 1   ; /* [31]  */
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
        unsigned int    reserved_1            : 1   ; /* [31]  */
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
        unsigned int    nxt2_dcmp_en          : 1   ; /* [31]  */
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

/* Define the union U_VPSS_RFRCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 7   ; /* [6..0]  */
        unsigned int    rfr_dither_en         : 1   ; /* [7]  */
        unsigned int    reserved_1            : 2   ; /* [9..8]  */
        unsigned int    rfr_dither_mode       : 1   ; /* [10]  */
        unsigned int    reserved_2            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_RFRCTRL;

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
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TUNL_CTRL1;

/* Define the union U_VPSS_REESTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reey_stride           : 16  ; /* [15..0]  */
        unsigned int    reec_stride           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_REESTRIDE;

/* Define the union U_VPSS_PRJCURSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    prjv_cur_stride       : 16  ; /* [15..0]  */
        unsigned int    prjh_cur_stride       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PRJCURSTRIDE;

/* Define the union U_VPSS_RGMVSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgmv_cur_stride       : 16  ; /* [15..0]  */
        unsigned int    rgmv_nx1_stride       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_RGMVSTRIDE;

/* Define the union U_VPSS_BLKMVSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blkmv_ref_stride      : 16  ; /* [15..0]  */
        unsigned int    blkmv_cur_stride      : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_BLKMVSTRIDE;

/* Define the union U_VPSS_CUESTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cuey_stride           : 16  ; /* [15..0]  */
        unsigned int    cuec_stride           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CUESTRIDE;

/* Define the union U_VPSS_PRJNX2STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    prjv_nx2_stride       : 16  ; /* [15..0]  */
        unsigned int    prjh_nx2_stride       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PRJNX2STRIDE;

/* Define the union U_VPSS_RGMVNX2STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgmv_nx2_stride       : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_RGMVNX2STRIDE;

/* Define the union U_VPSS_BLKMVNX1STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blkmv_nx1_stride      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_BLKMVNX1STRIDE;

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

/* Define the union U_VPSS_BLKMVNX2STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blkmv_nx2_stride      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_BLKMVNX2STRIDE;

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

/* Define the union U_VPSS_RCH_BYPASS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    rch_cury_bypass       : 1   ; /* [1]  */
        unsigned int    rch_curc_bypass       : 1   ; /* [2]  */
        unsigned int    rch_refy_bypass       : 1   ; /* [3]  */
        unsigned int    rch_refc_bypass       : 1   ; /* [4]  */
        unsigned int    rch_nx1y_bypass       : 1   ; /* [5]  */
        unsigned int    rch_nx1c_bypass       : 1   ; /* [6]  */
        unsigned int    rch_nx2y_bypass       : 1   ; /* [7]  */
        unsigned int    rch_nx2c_bypass       : 1   ; /* [8]  */
        unsigned int    rch_nx2cr_bypass      : 1   ; /* [9]  */
        unsigned int    rch_trmd_bypass       : 1   ; /* [10]  */
        unsigned int    rch_srmd_bypass       : 1   ; /* [11]  */
        unsigned int    rch_rst_bypass        : 1   ; /* [12]  */
        unsigned int    rch_prjh_cur_bypass   : 1   ; /* [13]  */
        unsigned int    rch_blkmv_nx1_bypass  : 1   ; /* [14]  */
        unsigned int    rch_blkmv_cur_bypass  : 1   ; /* [15]  */
        unsigned int    rch_blkmv_ref_bypass  : 1   ; /* [16]  */
        unsigned int    rch_prjv_cur_bypass   : 1   ; /* [17]  */
        unsigned int    rch_rgmv_nx1_bypass   : 1   ; /* [18]  */
        unsigned int    rch_rgmv_cur_bypass   : 1   ; /* [19]  */
        unsigned int    rch_reey_bypass       : 1   ; /* [20]  */
        unsigned int    rch_reec_bypass       : 1   ; /* [21]  */
        unsigned int    rch_tunl_bypass       : 1   ; /* [22]  */
        unsigned int    rch_nx2y_head_bypass  : 1   ; /* [23]  */
        unsigned int    rch_nx2c_head_bypass  : 1   ; /* [24]  */
        unsigned int    rch_nx2y_tile_2bit_bypass : 1   ; /* [25]  */
        unsigned int    rch_nx2c_tile_2bit_bypass : 1   ; /* [26]  */
        unsigned int    rch_vdh_db_bypass     : 1   ; /* [27]  */
        unsigned int    rch_transr_bypass     : 1   ; /* [28]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_RCH_BYPASS;

/* Define the union U_VPSS_WCH_BYPASS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wch_vhd0y_bypass      : 1   ; /* [0]  */
        unsigned int    wch_vhd0c_bypass      : 1   ; /* [1]  */
        unsigned int    wch_rfry_bypass       : 1   ; /* [2]  */
        unsigned int    wch_rfrc_bypass       : 1   ; /* [3]  */
        unsigned int    reserved_0            : 1   ; /* [4]  */
        unsigned int    wch_wst_bypass        : 1   ; /* [5]  */
        unsigned int    wch_twmd_bypass       : 1   ; /* [6]  */
        unsigned int    wch_stt_bypass        : 1   ; /* [7]  */
        unsigned int    reserved_1            : 1   ; /* [8]  */
        unsigned int    wch_prjhnx2_bypass    : 1   ; /* [9]  */
        unsigned int    wch_blkmvnx2_bypass   : 1   ; /* [10]  */
        unsigned int    wch_prjvnx2_bypass    : 1   ; /* [11]  */
        unsigned int    wch_rgmvnx2_bypass    : 1   ; /* [12]  */
        unsigned int    wch_cuey_bypass       : 1   ; /* [13]  */
        unsigned int    wch_cuec_bypass       : 1   ; /* [14]  */
        unsigned int    wch_transw_bypass     : 1   ; /* [15]  */
        unsigned int    wch_tunl_bypass       : 1   ; /* [16]  */
        unsigned int    reserved_2            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_WCH_BYPASS;

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
        unsigned int    bus_w_err_state       : 1   ; /* [2]  */
        unsigned int    eof_end_state         : 1   ; /* [3]  */
        unsigned int    vhd0_tunl_state       : 1   ; /* [4]  */
        unsigned int    dcmp_err_state        : 1   ; /* [5]  */
        unsigned int    bus_r_err_state       : 1   ; /* [6]  */
        unsigned int    ip_used_state         : 1   ; /* [7]  */
        unsigned int    ip_using_err_state    : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
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
        unsigned int    dcmp_err_clr          : 1   ; /* [5]  */
        unsigned int    bus_r_err_clr         : 1   ; /* [6]  */
        unsigned int    ip_used_clr           : 1   ; /* [7]  */
        unsigned int    ip_using_err_clr      : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
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
        unsigned int    raw_bus_w_err         : 1   ; /* [2]  */
        unsigned int    raw_eof_end           : 1   ; /* [3]  */
        unsigned int    raw_vhd0_tunl         : 1   ; /* [4]  */
        unsigned int    raw_dcmp_err          : 1   ; /* [5]  */
        unsigned int    raw_bus_r_err         : 1   ; /* [6]  */
        unsigned int    raw_ip_used           : 1   ; /* [7]  */
        unsigned int    raw_ip_using_err      : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
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
        unsigned int    burst_len_cfg         : 2   ; /* [27..26]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MISCELLANEOUS;

/* Define the union U_VPSS_FTCONFIG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    node_rst_en           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FTCONFIG;

/* Define the union U_VPSS_BUS_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    b256_split_en         : 1   ; /* [0]  */
        unsigned int    b128_split_en         : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_BUS_CTRL;

/* Define the union U_VPSS_DIECTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    edge_smooth_ratio     : 8   ; /* [15..8]  */
        unsigned int    stinfo_stop           : 1   ; /* [16]  */
        unsigned int    die_rst               : 1   ; /* [17]  */
        unsigned int    mchdir_c              : 1   ; /* [18]  */
        unsigned int    mchdir_l              : 1   ; /* [19]  */
        unsigned int    edge_smooth_en        : 1   ; /* [20]  */
        unsigned int    ma_only               : 1   ; /* [21]  */
        unsigned int    mc_only               : 1   ; /* [22]  */
        unsigned int    reserved_1            : 1   ; /* [23]  */
        unsigned int    die_c_mode            : 2   ; /* [25..24]  */
        unsigned int    die_l_mode            : 2   ; /* [27..26]  */
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
        unsigned int    dei_st_rst_value      : 16  ; /* [23..8]  */
        unsigned int    chroma_mf_offset      : 8   ; /* [31..24]  */
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
        unsigned int    dir_ratio_north       : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    min_north_strength    : 16  ; /* [31..16]  */
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
        unsigned int    bc1_max_dz            : 8   ; /* [7..0]  */
        unsigned int    bc1_autodz_gain       : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    bc1_gain              : 4   ; /* [19..16]  */
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
        unsigned int    bc2_max_dz            : 8   ; /* [7..0]  */
        unsigned int    bc2_autodz_gain       : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    bc2_gain              : 4   ; /* [19..16]  */
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
        unsigned int    dir0_scale            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dir1_scale            : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dir2_scale            : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    dir3_scale            : 6   ; /* [29..24]  */
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
        unsigned int    dir4_scale            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dir5_scale            : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dir6_scale            : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    dir7_scale            : 6   ; /* [29..24]  */
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
        unsigned int    dir8_scale            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dir9_scale            : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dir10_scale           : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    dir11_scale           : 6   ; /* [29..24]  */
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
        unsigned int    dir12_scale           : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dir13_scale           : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dir14_scale           : 6   ; /* [21..16]  */
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
        unsigned int    fld_motion_wnd_mode   : 1   ; /* [28]  */
        unsigned int    long_motion_shf       : 1   ; /* [29]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
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

/* Define the union U_VPSS_DIEEDGENORM0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edge_norm_0           : 12  ; /* [11..0]  */
        unsigned int    edge_norm_1           : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEEDGENORM0;

/* Define the union U_VPSS_DIEEDGENORM1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edge_norm_2           : 12  ; /* [11..0]  */
        unsigned int    edge_norm_3           : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEEDGENORM1;

/* Define the union U_VPSS_DIEEDGENORM2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edge_norm_4           : 12  ; /* [11..0]  */
        unsigned int    edge_norm_5           : 12  ; /* [23..12]  */
        unsigned int    mc_strength_k3        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEEDGENORM2;

/* Define the union U_VPSS_DIEEDGENORM3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edge_norm_6           : 12  ; /* [11..0]  */
        unsigned int    edge_norm_7           : 12  ; /* [23..12]  */
        unsigned int    mc_strength_g3        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEEDGENORM3;

/* Define the union U_VPSS_DIEEDGENORM4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edge_norm_8           : 12  ; /* [11..0]  */
        unsigned int    edge_norm_9           : 12  ; /* [23..12]  */
        unsigned int    inter_diff_thd0       : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEEDGENORM4;

/* Define the union U_VPSS_DIEEDGENORM5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edge_norm_10          : 12  ; /* [11..0]  */
        unsigned int    edge_norm_11          : 12  ; /* [23..12]  */
        unsigned int    inter_diff_thd1       : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEEDGENORM5;

/* Define the union U_VPSS_DIEEDGEFORMC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edge_coring           : 14  ; /* [13..0]  */
        unsigned int    edge_scale            : 10  ; /* [23..14]  */
        unsigned int    inter_diff_thd2       : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEEDGEFORMC;

/* Define the union U_VPSS_DIEMCSTRENGTH0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mc_strength_k0        : 8   ; /* [7..0]  */
        unsigned int    mc_strength_k1        : 8   ; /* [15..8]  */
        unsigned int    mc_strength_k2        : 8   ; /* [23..16]  */
        unsigned int    mc_strength_g0        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMCSTRENGTH0;

/* Define the union U_VPSS_DIEMCSTRENGTH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mc_strength_g1        : 8   ; /* [7..0]  */
        unsigned int    mc_strength_g2        : 8   ; /* [15..8]  */
        unsigned int    mc_strength_ming      : 8   ; /* [23..16]  */
        unsigned int    mc_strength_maxg      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMCSTRENGTH1;

/* Define the union U_VPSS_DIEFUSION0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k_y_mcw               : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    k_y_mcbld             : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    k_c_mcw               : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    k_c_mcbld             : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFUSION0;

/* Define the union U_VPSS_DIEFUSION1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    x0_mcw_adj            : 8   ; /* [7..0]  */
        unsigned int    k0_mcw_adj            : 8   ; /* [15..8]  */
        unsigned int    g0_mcw_adj            : 12  ; /* [27..16]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFUSION1;

/* Define the union U_VPSS_DIEFUSION2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k1_mcw_adj            : 8   ; /* [7..0]  */
        unsigned int    x0_mcbld              : 8   ; /* [15..8]  */
        unsigned int    k0_mcbld              : 8   ; /* [23..16]  */
        unsigned int    k1_mcbld              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFUSION2;

/* Define the union U_VPSS_DIEFUSION3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g0_mcbld              : 12  ; /* [11..0]  */
        unsigned int    k_curw_mcbld          : 5   ; /* [16..12]  */
        unsigned int    reserved_0            : 3   ; /* [19..17]  */
        unsigned int    mc_lai_bldmode        : 1   ; /* [20]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEFUSION3;

/* Define the union U_VPSS_DIEMAGBMTHD0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ma_gbm_thd1           : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    ma_gbm_thd0           : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMAGBMTHD0;

/* Define the union U_VPSS_DIEMAGBMTHD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ma_gbm_thd3           : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    ma_gbm_thd2           : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMAGBMTHD1;

/* Define the union U_VPSS_DIEMCGBMCOEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mtth0_gmd             : 4   ; /* [3..0]  */
        unsigned int    mtth1_gmd             : 5   ; /* [8..4]  */
        unsigned int    reserved_0            : 3   ; /* [11..9]  */
        unsigned int    mtth2_gmd             : 6   ; /* [17..12]  */
        unsigned int    reserved_1            : 2   ; /* [19..18]  */
        unsigned int    mtth3_gmd             : 6   ; /* [25..20]  */
        unsigned int    reserved_2            : 2   ; /* [27..26]  */
        unsigned int    mtfilten_gmd          : 1   ; /* [28]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMCGBMCOEF0;

/* Define the union U_VPSS_DIEMCGBMCOEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k_maxmag_gmd          : 4   ; /* [3..0]  */
        unsigned int    k_difh_gmd            : 7   ; /* [10..4]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    k_mag_gmd             : 8   ; /* [19..12]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEMCGBMCOEF1;

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
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
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
        unsigned int    reserved_0            : 20  ; /* [19..0]  */
        unsigned int    diff_movblk_thd       : 8   ; /* [27..20]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_PDBLKTHD;

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
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
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

/* Define the union U_VPSS_MCDI_RGDIFY */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lmt_rgdify            : 10  ; /* [9..0]  */
        unsigned int    core_rgdify           : 4   ; /* [13..10]  */
        unsigned int    g_rgdifycore          : 10  ; /* [23..14]  */
        unsigned int    k_rgdifycore          : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGDIFY;

/* Define the union U_VPSS_MCDI_RGSAD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_tpdif_rgsad         : 9   ; /* [8..0]  */
        unsigned int    k_tpdif_rgsad         : 6   ; /* [14..9]  */
        unsigned int    kmv_rgsad             : 7   ; /* [21..15]  */
        unsigned int    reserved_0            : 1   ; /* [22]  */
        unsigned int    coef_sadlpf           : 3   ; /* [25..23]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGSAD;

/* Define the union U_VPSS_MCDI_RGMV */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    th_0mvsad_rgmv        : 10  ; /* [9..0]  */
        unsigned int    th_saddif_rgmv        : 9   ; /* [18..10]  */
        unsigned int    thmag_rgmv            : 8   ; /* [26..19]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGMV;

/* Define the union U_VPSS_MCDI_RGMAG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lmt_mag_rg            : 10  ; /* [9..0]  */
        unsigned int    core_mag_rg           : 4   ; /* [13..10]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGMAG0;

/* Define the union U_VPSS_MCDI_RGMAG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en_mvadj_rgmvls       : 1   ; /* [0]  */
        unsigned int    th_mvadj_rgmvls       : 4   ; /* [4..1]  */
        unsigned int    k_mag_rgmvls          : 4   ; /* [8..5]  */
        unsigned int    core_mag_rgmvls       : 7   ; /* [15..9]  */
        unsigned int    k_mv_rgmvls           : 5   ; /* [20..16]  */
        unsigned int    core_mv_rgmvls        : 5   ; /* [25..21]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGMAG1;

/* Define the union U_VPSS_MCDI_RGLS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k_sadcore_rgmv        : 4   ; /* [3..0]  */
        unsigned int    th_sad_rgls           : 5   ; /* [8..4]  */
        unsigned int    th_mag_rgls           : 6   ; /* [14..9]  */
        unsigned int    k_sad_rgls            : 4   ; /* [18..15]  */
        unsigned int    reserved_0            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGLS;

/* Define the union U_VPSS_MCDI_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    force_mvx             : 8   ; /* [7..0]  */
        unsigned int    force_mven            : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_SEL;

/* Define the union U_VPSS_MCDI_DLT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    th_hblkdist_mvdlt     : 4   ; /* [3..0]  */
        unsigned int    th_vblkdist_mvdlt     : 4   ; /* [7..4]  */
        unsigned int    th_ls_mvdlt           : 4   ; /* [11..8]  */
        unsigned int    th_rgmvx_mvdlt        : 4   ; /* [15..12]  */
        unsigned int    th_blkmvx_mvdlt       : 3   ; /* [18..16]  */
        unsigned int    reserved_0            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_DLT0;

/* Define the union U_VPSS_MCDI_DLT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    thh_sad_mvdlt         : 5   ; /* [4..0]  */
        unsigned int    thl_sad_mvdlt         : 7   ; /* [11..5]  */
        unsigned int    g_mag_mvdlt           : 6   ; /* [17..12]  */
        unsigned int    th_mag_mvdlt          : 5   ; /* [22..18]  */
        unsigned int    k_sadcore_mvdlt       : 3   ; /* [25..23]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_DLT1;

/* Define the union U_VPSS_MCDI_RGMCW0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k_core_simimv         : 5   ; /* [4..0]  */
        unsigned int    gl_core_simimv        : 3   ; /* [7..5]  */
        unsigned int    gh_core_simimv        : 6   ; /* [13..8]  */
        unsigned int    k_simimvw             : 6   ; /* [19..14]  */
        unsigned int    k_rglsw               : 5   ; /* [24..20]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGMCW0;

/* Define the union U_VPSS_MCDI_RGMCW1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgtb_en_mcw           : 1   ; /* [0]  */
        unsigned int    core_mvy_mcw          : 3   ; /* [3..1]  */
        unsigned int    k_mvy_mcw             : 6   ; /* [9..4]  */
        unsigned int    core_rgsadadj_mcw     : 9   ; /* [18..10]  */
        unsigned int    k_rgsadadj_mcw        : 4   ; /* [22..19]  */
        unsigned int    k_core_vsaddif        : 5   ; /* [27..23]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGMCW1;

/* Define the union U_VPSS_MCDI_RGMCW2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gl_core_vsaddif       : 5   ; /* [4..0]  */
        unsigned int    gh_core_vsad_dif      : 7   ; /* [11..5]  */
        unsigned int    k_vsaddifw            : 6   ; /* [17..12]  */
        unsigned int    mode_rgysad_mcw       : 1   ; /* [18]  */
        unsigned int    core_rgmag_mcw        : 8   ; /* [26..19]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGMCW2;

/* Define the union U_VPSS_MCDI_RGMCW3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    x0_rgmag_mcw          : 9   ; /* [8..0]  */
        unsigned int    k0_rgmag_mcw          : 9   ; /* [17..9]  */
        unsigned int    g0_rgmag_mcw          : 8   ; /* [25..18]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGMCW3;

/* Define the union U_VPSS_MCDI_RGMCW4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k1_rgmag_mcw          : 9   ; /* [8..0]  */
        unsigned int    core_rgsad_mcw        : 8   ; /* [16..9]  */
        unsigned int    x0_rgsad_mcw          : 10  ; /* [26..17]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGMCW4;

/* Define the union U_VPSS_MCDI_RGMCW5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k0_rgsad_mcw          : 9   ; /* [8..0]  */
        unsigned int    g0_rgsad_mcw          : 8   ; /* [16..9]  */
        unsigned int    k1_rgsad_mcw          : 9   ; /* [25..17]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGMCW5;

/* Define the union U_VPSS_MCDI_RGMCW6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    x0_smrg_mcw           : 8   ; /* [7..0]  */
        unsigned int    k0_smrg_mcw           : 8   ; /* [15..8]  */
        unsigned int    x_rgsad_mcw           : 8   ; /* [23..16]  */
        unsigned int    k_rgsad_mcw           : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_RGMCW6;

/* Define the union U_VPSS_MCDI_TPMCW0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    x0_tpmvdist_mcw       : 8   ; /* [7..0]  */
        unsigned int    k0_tpmvdist_mcw       : 7   ; /* [14..8]  */
        unsigned int    g0_tpmvdist_mcw       : 8   ; /* [22..15]  */
        unsigned int    k1_tpmvdist_mcw       : 7   ; /* [29..23]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_TPMCW0;

/* Define the union U_VPSS_MCDI_TPMCW1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k_minmvdist_mcw       : 4   ; /* [3..0]  */
        unsigned int    k_avgmvdist_mcw       : 4   ; /* [7..4]  */
        unsigned int    b_core_tpmvdist_mcw   : 3   ; /* [10..8]  */
        unsigned int    k_core_tpmvdist_mcw   : 3   ; /* [13..11]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_TPMCW1;

/* Define the union U_VPSS_MCDI_WNDMCW0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k_difhcore_mcw        : 5   ; /* [4..0]  */
        unsigned int    k_difvcore_mcw        : 6   ; /* [10..5]  */
        unsigned int    k_max_core_mcw        : 4   ; /* [14..11]  */
        unsigned int    k_max_dif_mcw         : 4   ; /* [18..15]  */
        unsigned int    k1_max_mag_mcw        : 4   ; /* [22..19]  */
        unsigned int    k0_max_mag_mcw        : 4   ; /* [26..23]  */
        unsigned int    k_tbdif_mcw           : 4   ; /* [30..27]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_WNDMCW0;

/* Define the union U_VPSS_MCDI_WNDMCW1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k_tbmag_mcw           : 4   ; /* [3..0]  */
        unsigned int    x0_mag_wnd_mcw        : 7   ; /* [10..4]  */
        unsigned int    k0_mag_wnd_mcw        : 4   ; /* [14..11]  */
        unsigned int    g0_mag_wnd_mcw        : 6   ; /* [20..15]  */
        unsigned int    k1_mag_wnd_mcw        : 4   ; /* [24..21]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_WNDMCW1;

/* Define the union U_VPSS_MCDI_WNDMCW2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g1_mag_wnd_mcw        : 9   ; /* [8..0]  */
        unsigned int    x0_sad_wnd_mcw        : 7   ; /* [15..9]  */
        unsigned int    k0_sad_wnd_mcw        : 5   ; /* [20..16]  */
        unsigned int    g0_sad_wnd_mcw        : 8   ; /* [28..21]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_WNDMCW2;

/* Define the union U_VPSS_MCDI_WNDMCW3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k1_sad_wnd_mcw        : 5   ; /* [4..0]  */
        unsigned int    g1_sad_wnd_mcw        : 9   ; /* [13..5]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_WNDMCW3;

/* Define the union U_VPSS_MCDI_VERTWEIGHT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k_max_hvdif_dw        : 4   ; /* [3..0]  */
        unsigned int    gain_lpf_dw           : 4   ; /* [7..4]  */
        unsigned int    core_bhvdif_dw        : 4   ; /* [11..8]  */
        unsigned int    k_bhvdif_dw           : 7   ; /* [18..12]  */
        unsigned int    reserved_0            : 1   ; /* [19]  */
        unsigned int    b_bhvdif_dw           : 3   ; /* [22..20]  */
        unsigned int    reserved_1            : 1   ; /* [23]  */
        unsigned int    b_hvdif_dw            : 3   ; /* [26..24]  */
        unsigned int    reserved_2            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_VERTWEIGHT0;

/* Define the union U_VPSS_MCDI_VERTWEIGHT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    core_hvdif_dw         : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    k_difv_dw             : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    core_mv_dw            : 4   ; /* [19..16]  */
        unsigned int    b_mv_dw               : 6   ; /* [25..20]  */
        unsigned int    reserved_2            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_VERTWEIGHT1;

/* Define the union U_VPSS_MCDI_VERTWEIGHT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    x0_hvdif_dw           : 9   ; /* [8..0]  */
        unsigned int    k0_hvdif_dw           : 6   ; /* [14..9]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    g0_hvdif_dw           : 9   ; /* [24..16]  */
        unsigned int    k1_hvdif_dw           : 6   ; /* [30..25]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_VERTWEIGHT2;

/* Define the union U_VPSS_MCDI_VERTWEIGHT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    x0_mv_dw              : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    k0_mv_dw              : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    g0_mv_dw              : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    k1_mv_dw              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_VERTWEIGHT3;

/* Define the union U_VPSS_MCDI_VERTWEIGHT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    x0_mt_dw              : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    k0_mt_dw              : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    g0_mt_dw              : 8   ; /* [23..16]  */
        unsigned int    k1_mt_dw              : 6   ; /* [29..24]  */
        unsigned int    reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_VERTWEIGHT4;

/* Define the union U_VPSS_MCDI_VERTWEIGHT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g0_mv_mt              : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    x0_mv_mt              : 2   ; /* [9..8]  */
        unsigned int    reserved_1            : 2   ; /* [11..10]  */
        unsigned int    k1_mv_mt              : 5   ; /* [16..12]  */
        unsigned int    reserved_2            : 7   ; /* [23..17]  */
        unsigned int    b_mt_dw               : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_VERTWEIGHT5;

/* Define the union U_VPSS_MCDI_MC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gain_up_mclpfv        : 5   ; /* [4..0]  */
        unsigned int    gain_dn_mclpfv        : 5   ; /* [9..5]  */
        unsigned int    gain_mclpfh           : 5   ; /* [14..10]  */
        unsigned int    rs_pxlmag_mcw         : 1   ; /* [15]  */
        unsigned int    x_pxlmag_mcw          : 3   ; /* [18..16]  */
        unsigned int    reserved_0            : 1   ; /* [19]  */
        unsigned int    k_pxlmag_mcw          : 7   ; /* [26..20]  */
        unsigned int    reserved_1            : 1   ; /* [27]  */
        unsigned int    mclpf_mode            : 1   ; /* [28]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_MC0;

/* Define the union U_VPSS_MCDI_MC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_pxlmag_mcw          : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_MC1;

/* Define the union U_VPSS_MCDI_MC2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k_y_vertw             : 5   ; /* [4..0]  */
        unsigned int    k_c_vertw             : 7   ; /* [11..5]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_MC2;

/* Define the union U_VPSS_MCDI_MC3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k_delta               : 5   ; /* [4..0]  */
        unsigned int    bdh_mcpos             : 3   ; /* [7..5]  */
        unsigned int    bdv_mcpos             : 3   ; /* [10..8]  */
        unsigned int    x0_mv_mc              : 3   ; /* [13..11]  */
        unsigned int    k1_mv_mc              : 6   ; /* [19..14]  */
        unsigned int    x_fstmt_mc            : 5   ; /* [24..20]  */
        unsigned int    k_fstmt_mc            : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_MC3;

/* Define the union U_VPSS_MCDI_MC4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_fstmt_mc            : 5   ; /* [4..0]  */
        unsigned int    x_slmt_mc             : 5   ; /* [9..5]  */
        unsigned int    k_slmt_mc             : 5   ; /* [14..10]  */
        unsigned int    g_slmt_mc             : 6   ; /* [20..15]  */
        unsigned int    x_hfcore_mc           : 5   ; /* [25..21]  */
        unsigned int    k_hfcore_mc           : 5   ; /* [30..26]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_MC4;

/* Define the union U_VPSS_MCDI_MC5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_hfcore_mc           : 6   ; /* [5..0]  */
        unsigned int    c0_mc                 : 12  ; /* [17..6]  */
        unsigned int    r0_mc                 : 12  ; /* [29..18]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_MC5;

/* Define the union U_VPSS_MCDI_MC6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    c1_mc                 : 12  ; /* [11..0]  */
        unsigned int    r1_mc                 : 12  ; /* [23..12]  */
        unsigned int    mcmvrange             : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_MC6;

/* Define the union U_VPSS_MCDI_MC7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scenechange_mc        : 1   ; /* [0]  */
        unsigned int    x_frcount_mc          : 5   ; /* [5..1]  */
        unsigned int    k_frcount_mc          : 7   ; /* [12..6]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_MC7;

/* Define the union U_VPSS_MCDI_LCDINEW0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcstartr              : 8   ; /* [7..0]  */
        unsigned int    mcstartc              : 8   ; /* [15..8]  */
        unsigned int    mcendr                : 8   ; /* [23..16]  */
        unsigned int    mcendc                : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_LCDINEW0;

/* Define the union U_VPSS_MCDI_LCDINEW1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    movethdh              : 6   ; /* [5..0]  */
        unsigned int    movethdl              : 6   ; /* [11..6]  */
        unsigned int    movecorig             : 6   ; /* [17..12]  */
        unsigned int    movegain              : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_LCDINEW1;

/* Define the union U_VPSS_MCDI_NUMT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    numt_lpf_en           : 1   ; /* [0]  */
        unsigned int    numt_coring           : 6   ; /* [6..1]  */
        unsigned int    numt_gain             : 8   ; /* [14..7]  */
        unsigned int    mc_numt_blden         : 1   ; /* [15]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_NUMT;

/* Define the union U_VPSS_MCDI_DEMO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    demo_en               : 1   ; /* [0]  */
        unsigned int    demo_mode_l           : 2   ; /* [2..1]  */
        unsigned int    demo_mode_r           : 2   ; /* [4..3]  */
        unsigned int    demo_border           : 12  ; /* [16..5]  */
        unsigned int    reserved_0            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_DEMO;

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

/* Define the union U_VPSS_DB_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_en                 : 1   ; /* [0]  */
        unsigned int    dm_en                 : 1   ; /* [1]  */
        unsigned int    ds_en                 : 1   ; /* [2]  */
        unsigned int    dr_en                 : 1   ; /* [3]  */
        unsigned int    db_lum_ver_en         : 1   ; /* [4]  */
        unsigned int    db_lum_hor_en         : 1   ; /* [5]  */
        unsigned int    reserved_0            : 3   ; /* [8..6]  */
        unsigned int    dbm_demo_en           : 1   ; /* [9]  */
        unsigned int    dbm_demo_mode         : 1   ; /* [10]  */
        unsigned int    reserved_1            : 2   ; /* [12..11]  */
        unsigned int    det_hy_en             : 1   ; /* [13]  */
        unsigned int    det_hc_en             : 1   ; /* [14]  */
        unsigned int    det_vy_en             : 1   ; /* [15]  */
        unsigned int    dbm_out_mode          : 3   ; /* [18..16]  */
        unsigned int    dbm_out_sel_gain      : 6   ; /* [24..19]  */
        unsigned int    dbm_protocol          : 3   ; /* [27..25]  */
        unsigned int    reserved_2            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_CTRL;

/* Define the union U_VPSS_DB_LUT34 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_h_str_fade_lut1_p0 : 5   ; /* [4..0]  */
        unsigned int    db_lum_h_str_fade_lut1_p1 : 5   ; /* [9..5]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT34;

/* Define the union U_VPSS_DB_LUT35 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_h_str_fade_lut2_p0 : 5   ; /* [4..0]  */
        unsigned int    db_lum_h_str_fade_lut2_p1 : 5   ; /* [9..5]  */
        unsigned int    db_lum_h_str_fade_lut2_p2 : 5   ; /* [14..10]  */
        unsigned int    db_lum_h_str_fade_lut2_p3 : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT35;

/* Define the union U_VPSS_DB_LUT36 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_h_str_fade_lut3_p0 : 5   ; /* [4..0]  */
        unsigned int    db_lum_h_str_fade_lut3_p1 : 5   ; /* [9..5]  */
        unsigned int    db_lum_h_str_fade_lut3_p2 : 5   ; /* [14..10]  */
        unsigned int    db_lum_h_str_fade_lut3_p3 : 5   ; /* [19..15]  */
        unsigned int    db_lum_h_str_fade_lut3_p4 : 5   ; /* [24..20]  */
        unsigned int    db_lum_h_str_fade_lut3_p5 : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT36;

/* Define the union U_VPSS_DB_LUT37 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_h_str_fade_lut3_p6 : 5   ; /* [4..0]  */
        unsigned int    db_lum_h_str_fade_lut3_p7 : 5   ; /* [9..5]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT37;

/* Define the union U_VPSS_DB_LUT38 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_h_str_fade_lut4_p0 : 5   ; /* [4..0]  */
        unsigned int    db_lum_h_str_fade_lut4_p1 : 5   ; /* [9..5]  */
        unsigned int    db_lum_h_str_fade_lut4_p2 : 5   ; /* [14..10]  */
        unsigned int    db_lum_h_str_fade_lut4_p3 : 5   ; /* [19..15]  */
        unsigned int    db_lum_h_str_fade_lut4_p4 : 5   ; /* [24..20]  */
        unsigned int    db_lum_h_str_fade_lut4_p5 : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT38;

/* Define the union U_VPSS_DB_LUT39 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_h_str_fade_lut4_p6 : 5   ; /* [4..0]  */
        unsigned int    db_lum_h_str_fade_lut4_p7 : 5   ; /* [9..5]  */
        unsigned int    db_lum_h_str_fade_lut4_p8 : 5   ; /* [14..10]  */
        unsigned int    db_lum_h_str_fade_lut4_p9 : 5   ; /* [19..15]  */
        unsigned int    db_lum_h_str_fade_lut4_p10 : 5   ; /* [24..20]  */
        unsigned int    db_lum_h_str_fade_lut4_p11 : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT39;

/* Define the union U_VPSS_DB_LUT40 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_v_str_fade_lut_p0 : 5   ; /* [4..0]  */
        unsigned int    db_lum_v_str_fade_lut_p1 : 5   ; /* [9..5]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT40;

/* Define the union U_VPSS_DB_LUT44 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_sobel_str_lut_p0   : 5   ; /* [4..0]  */
        unsigned int    db_sobel_str_lut_p1   : 5   ; /* [9..5]  */
        unsigned int    db_sobel_str_lut_p2   : 5   ; /* [14..10]  */
        unsigned int    db_sobel_str_lut_p3   : 5   ; /* [19..15]  */
        unsigned int    db_sobel_str_lut_p4   : 5   ; /* [24..20]  */
        unsigned int    db_sobel_str_lut_p5   : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT44;

/* Define the union U_VPSS_DB_LUT45 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_sobel_str_lut_p6   : 5   ; /* [4..0]  */
        unsigned int    db_sobel_str_lut_p7   : 5   ; /* [9..5]  */
        unsigned int    db_sobel_str_lut_p8   : 5   ; /* [14..10]  */
        unsigned int    db_sobel_str_lut_p9   : 5   ; /* [19..15]  */
        unsigned int    db_sobel_str_lut_p10  : 5   ; /* [24..20]  */
        unsigned int    db_sobel_str_lut_p11  : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT45;

/* Define the union U_VPSS_DB_LUT46 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_sobel_str_lut_p12  : 5   ; /* [4..0]  */
        unsigned int    db_sobel_str_lut_p13  : 5   ; /* [9..5]  */
        unsigned int    db_sobel_str_lut_p14  : 5   ; /* [14..10]  */
        unsigned int    db_sobel_str_lut_p15  : 5   ; /* [19..15]  */
        unsigned int    db_sobel_str_lut_p16  : 5   ; /* [24..20]  */
        unsigned int    db_sobel_str_lut_p17  : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT46;

/* Define the union U_VPSS_DB_LUT47 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_sobel_str_lut_p18  : 5   ; /* [4..0]  */
        unsigned int    db_sobel_str_lut_p19  : 5   ; /* [9..5]  */
        unsigned int    db_sobel_str_lut_p20  : 5   ; /* [14..10]  */
        unsigned int    db_sobel_str_lut_p21  : 5   ; /* [19..15]  */
        unsigned int    db_sobel_str_lut_p22  : 5   ; /* [24..20]  */
        unsigned int    db_sobel_str_lut_p23  : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT47;

/* Define the union U_VPSS_DB_LUT48 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_sobel_str_lut_p24  : 5   ; /* [4..0]  */
        unsigned int    db_sobel_str_lut_p25  : 5   ; /* [9..5]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT48;

/* Define the union U_VPSS_DB_LUT49 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_dir_str_lut_p0     : 5   ; /* [4..0]  */
        unsigned int    db_dir_str_lut_p1     : 5   ; /* [9..5]  */
        unsigned int    db_dir_str_lut_p2     : 5   ; /* [14..10]  */
        unsigned int    db_dir_str_lut_p3     : 5   ; /* [19..15]  */
        unsigned int    db_dir_str_lut_p4     : 5   ; /* [24..20]  */
        unsigned int    db_dir_str_lut_p5     : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT49;

/* Define the union U_VPSS_DB_LUT50 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_dir_str_lut_p6     : 5   ; /* [4..0]  */
        unsigned int    db_dir_str_lut_p7     : 5   ; /* [9..5]  */
        unsigned int    db_dir_str_lut_p8     : 5   ; /* [14..10]  */
        unsigned int    db_dir_str_lut_p9     : 5   ; /* [19..15]  */
        unsigned int    db_dir_str_lut_p10    : 5   ; /* [24..20]  */
        unsigned int    db_dir_str_lut_p11    : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT50;

/* Define the union U_VPSS_DB_LUT51 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_dir_str_lut_p12    : 5   ; /* [4..0]  */
        unsigned int    db_dir_str_lut_p13    : 5   ; /* [9..5]  */
        unsigned int    db_dir_str_lut_p14    : 5   ; /* [14..10]  */
        unsigned int    db_dir_str_lut_p15    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT51;

/* Define the union U_VPSS_DB_LUT52 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_dir_str_gain_lut_p0 : 5   ; /* [4..0]  */
        unsigned int    db_dir_str_gain_lut_p1 : 5   ; /* [9..5]  */
        unsigned int    db_dir_str_gain_lut_p2 : 5   ; /* [14..10]  */
        unsigned int    db_dir_str_gain_lut_p3 : 5   ; /* [19..15]  */
        unsigned int    db_dir_str_gain_lut_p4 : 5   ; /* [24..20]  */
        unsigned int    db_dir_str_gain_lut_p5 : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT52;

/* Define the union U_VPSS_DB_LUT53 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_dir_str_gain_lut_p6 : 5   ; /* [4..0]  */
        unsigned int    db_dir_str_gain_lut_p7 : 5   ; /* [9..5]  */
        unsigned int    db_dir_str_gain_lut_p8 : 5   ; /* [14..10]  */
        unsigned int    db_dir_str_gain_lut_p9 : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT53;

/* Define the union U_VPSS_DB_LUT54 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_diff_map_lut_p0    : 8   ; /* [7..0]  */
        unsigned int    db_diff_map_lut_p1    : 8   ; /* [15..8]  */
        unsigned int    db_diff_map_lut_p2    : 8   ; /* [23..16]  */
        unsigned int    db_diff_map_lut_p3    : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT54;

/* Define the union U_VPSS_DB_LUT55 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_diff_map_lut_p4    : 8   ; /* [7..0]  */
        unsigned int    db_diff_map_lut_p5    : 8   ; /* [15..8]  */
        unsigned int    db_diff_map_lut_p6    : 8   ; /* [23..16]  */
        unsigned int    db_diff_map_lut_p7    : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT55;

/* Define the union U_VPSS_DB_LUT56 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_diff_weight_lut_p0 : 10  ; /* [9..0]  */
        unsigned int    db_diff_weight_lut_p1 : 10  ; /* [19..10]  */
        unsigned int    db_diff_weight_lut_p2 : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT56;

/* Define the union U_VPSS_DB_LUT57 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_diff_weight_lut_p3 : 10  ; /* [9..0]  */
        unsigned int    db_diff_weight_lut_p4 : 10  ; /* [19..10]  */
        unsigned int    db_diff_weight_lut_p5 : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT57;

/* Define the union U_VPSS_DB_LUT59 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_diff_str_lut_p0    : 6   ; /* [5..0]  */
        unsigned int    db_diff_str_lut_p1    : 6   ; /* [11..6]  */
        unsigned int    db_diff_str_lut_p2    : 6   ; /* [17..12]  */
        unsigned int    db_diff_str_lut_p3    : 6   ; /* [23..18]  */
        unsigned int    db_diff_str_lut_p4    : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT59;

/* Define the union U_VPSS_DB_LUT60 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_diff_str_lut_p5    : 6   ; /* [5..0]  */
        unsigned int    db_diff_str_lut_p6    : 6   ; /* [11..6]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT60;

/* Define the union U_VPSS_DB_CTRL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 5   ; /* [4..0]  */
        unsigned int    db_det_en             : 1   ; /* [5]  */
        unsigned int    db_vdh_trans_info_en  : 1   ; /* [6]  */
        unsigned int    reserved_1            : 5   ; /* [11..7]  */
        unsigned int    db_blk_mode           : 1   ; /* [12]  */
        unsigned int    db_blk_pos_mode       : 1   ; /* [13]  */
        unsigned int    db_streth_en          : 1   ; /* [14]  */
        unsigned int    reserved_2            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_CTRL1;

/* Define the union U_VPSS_DB_THR0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_h_border_str_th : 6   ; /* [5..0]  */
        unsigned int    db_lum_v_border_str_th : 6   ; /* [11..6]  */
        unsigned int    db_dir_smooth_mode    : 2   ; /* [13..12]  */
        unsigned int    db_grad_sub_ratio     : 5   ; /* [18..14]  */
        unsigned int    db_ctrst_thresh       : 5   ; /* [23..19]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_THR0;

/* Define the union U_VPSS_DB_THR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 14  ; /* [13..0]  */
        unsigned int    db_text_str_gain      : 5   ; /* [18..14]  */
        unsigned int    db_detect_ratio_h     : 5   ; /* [23..19]  */
        unsigned int    db_detect_ratio_v     : 5   ; /* [28..24]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_THR1;

/* Define the union U_VPSS_DB_OST0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_stt_offset_x       : 16  ; /* [15..0]  */
        unsigned int    db_stt_offset_y       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_OST0;

/* Define the union U_VPSS_DB_OST1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_end_offset_x       : 16  ; /* [15..0]  */
        unsigned int    db_end_offset_y       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_OST1;

/* Define the union U_VPSS_DB_DEMO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_demo_pos_x         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_DEMO;

/* Define the union U_VPSS_DB_LUT61 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_hor_delta_lut_p0 : 4   ; /* [3..0]  */
        unsigned int    db_lum_hor_delta_lut_p1 : 4   ; /* [7..4]  */
        unsigned int    db_lum_hor_delta_lut_p2 : 4   ; /* [11..8]  */
        unsigned int    db_lum_hor_delta_lut_p3 : 4   ; /* [15..12]  */
        unsigned int    db_lum_hor_delta_lut_p4 : 4   ; /* [19..16]  */
        unsigned int    db_lum_hor_delta_lut_p5 : 4   ; /* [23..20]  */
        unsigned int    db_lum_hor_delta_lut_p6 : 4   ; /* [27..24]  */
        unsigned int    db_lum_hor_delta_lut_p7 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT61;

/* Define the union U_VPSS_DB_LUT62 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_hor_delta_lut_p8 : 4   ; /* [3..0]  */
        unsigned int    db_lum_hor_delta_lut_p9 : 4   ; /* [7..4]  */
        unsigned int    db_lum_hor_delta_lut_p10 : 4   ; /* [11..8]  */
        unsigned int    db_lum_hor_delta_lut_p11 : 4   ; /* [15..12]  */
        unsigned int    db_lum_hor_delta_lut_p12 : 4   ; /* [19..16]  */
        unsigned int    db_lum_hor_delta_lut_p13 : 4   ; /* [23..20]  */
        unsigned int    db_lum_hor_delta_lut_p14 : 4   ; /* [27..24]  */
        unsigned int    db_lum_hor_delta_lut_p15 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT62;

/* Define the union U_VPSS_DB_LUT63 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_ver_delta_lut_p0 : 4   ; /* [3..0]  */
        unsigned int    db_lum_ver_delta_lut_p1 : 4   ; /* [7..4]  */
        unsigned int    db_lum_ver_delta_lut_p2 : 4   ; /* [11..8]  */
        unsigned int    db_lum_ver_delta_lut_p3 : 4   ; /* [15..12]  */
        unsigned int    db_lum_ver_delta_lut_p4 : 4   ; /* [19..16]  */
        unsigned int    db_lum_ver_delta_lut_p5 : 4   ; /* [23..20]  */
        unsigned int    db_lum_ver_delta_lut_p6 : 4   ; /* [27..24]  */
        unsigned int    db_lum_ver_delta_lut_p7 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT63;

/* Define the union U_VPSS_DB_LUT64 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_ver_delta_lut_p8 : 4   ; /* [3..0]  */
        unsigned int    db_lum_ver_delta_lut_p9 : 4   ; /* [7..4]  */
        unsigned int    db_lum_ver_delta_lut_p10 : 4   ; /* [11..8]  */
        unsigned int    db_lum_ver_delta_lut_p11 : 4   ; /* [15..12]  */
        unsigned int    db_lum_ver_delta_lut_p12 : 4   ; /* [19..16]  */
        unsigned int    db_lum_ver_delta_lut_p13 : 4   ; /* [23..20]  */
        unsigned int    db_lum_ver_delta_lut_p14 : 4   ; /* [27..24]  */
        unsigned int    db_lum_ver_delta_lut_p15 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT64;

/* Define the union U_VPSS_DB_THR2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_global_db_strength_lum : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_THR2;

/* Define the union U_VPSS_DB_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_blk_width          : 16  ; /* [15..0]  */
        unsigned int    db_blk_height         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_INFO;

/* Define the union U_VPSS_DB_THR3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_hor_var_core   : 8   ; /* [7..0]  */
        unsigned int    db_lum_hor_gain1      : 4   ; /* [11..8]  */
        unsigned int    db_lum_hor_gain2      : 4   ; /* [15..12]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_THR3;

/* Define the union U_VPSS_DBD_THDEDGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbd_hy_thd_edge       : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dbd_hc_thd_edge       : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dbd_vy_thd_edge       : 6   ; /* [21..16]  */
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
        unsigned int    dbd_hy_thd_txt        : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dbd_hc_thd_txt        : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    dbd_vy_thd_txt        : 6   ; /* [21..16]  */
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
        unsigned int    dbd_det_lut_wgt0      : 4   ; /* [3..0]  */
        unsigned int    dbd_det_lut_wgt1      : 4   ; /* [7..4]  */
        unsigned int    dbd_det_lut_wgt2      : 4   ; /* [11..8]  */
        unsigned int    dbd_det_lut_wgt3      : 4   ; /* [15..12]  */
        unsigned int    dbd_det_lut_wgt4      : 4   ; /* [19..16]  */
        unsigned int    dbd_det_lut_wgt5      : 4   ; /* [23..20]  */
        unsigned int    dbd_det_lut_wgt6      : 4   ; /* [27..24]  */
        unsigned int    dbd_det_lut_wgt7      : 4   ; /* [31..28]  */
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
        unsigned int    dbd_det_lut_wgt8      : 4   ; /* [3..0]  */
        unsigned int    dbd_det_lut_wgt9      : 4   ; /* [7..4]  */
        unsigned int    dbd_det_lut_wgt10     : 4   ; /* [11..8]  */
        unsigned int    dbd_det_lut_wgt11     : 4   ; /* [15..12]  */
        unsigned int    dbd_det_lut_wgt12     : 4   ; /* [19..16]  */
        unsigned int    dbd_det_lut_wgt13     : 4   ; /* [23..20]  */
        unsigned int    dbd_det_lut_wgt14     : 4   ; /* [27..24]  */
        unsigned int    dbd_det_lut_wgt15     : 4   ; /* [31..28]  */
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
        unsigned int    dbd_hy_rtn_bd_txt     : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    dbd_hc_rtn_bd_txt     : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    dbd_vy_rtn_bd_txt     : 5   ; /* [20..16]  */
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
        unsigned int    dbd_hy_tst_blk_num    : 10  ; /* [9..0]  */
        unsigned int    dbd_hc_tst_blk_num    : 10  ; /* [19..10]  */
        unsigned int    dbd_vy_tst_blk_num    : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
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
        unsigned int    dbd_hy_rdx8bin_lut0   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    dbd_hy_rdx8bin_lut1   : 11  ; /* [22..12]  */
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
        unsigned int    dbd_hy_rdx8bin_lut2   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    dbd_hy_rdx8bin_lut3   : 11  ; /* [22..12]  */
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
        unsigned int    dbd_hy_rdx8bin_lut4   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    dbd_hy_rdx8bin_lut5   : 11  ; /* [22..12]  */
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
        unsigned int    dbd_hy_rdx8bin_lut6   : 11  ; /* [10..0]  */
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
        unsigned int    dbd_hc_rdx8bin_lut0   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    dbd_hc_rdx8bin_lut1   : 11  ; /* [22..12]  */
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
        unsigned int    dbd_hc_rdx8bin_lut2   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    dbd_hc_rdx8bin_lut3   : 11  ; /* [22..12]  */
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
        unsigned int    dbd_hc_rdx8bin_lut4   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    dbd_hc_rdx8bin_lut5   : 11  ; /* [22..12]  */
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
        unsigned int    dbd_hc_rdx8bin_lut6   : 11  ; /* [10..0]  */
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
        unsigned int    dbd_vy_rdx8bin_lut0   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    dbd_vy_rdx8bin_lut1   : 11  ; /* [22..12]  */
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
        unsigned int    dbd_vy_rdx8bin_lut2   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    dbd_vy_rdx8bin_lut3   : 11  ; /* [22..12]  */
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
        unsigned int    dbd_vy_rdx8bin_lut4   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    dbd_vy_rdx8bin_lut5   : 11  ; /* [22..12]  */
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
        unsigned int    dbd_vy_rdx8bin_lut6   : 11  ; /* [10..0]  */
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
        unsigned int    dbd_min_blk_size      : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dbd_hy_max_blk_size   : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    dbd_hc_max_blk_size   : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    dbd_vy_max_blk_size   : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_BLKSIZE;

/* Define the union U_VPSS_DM_DIR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_grad_sub_ratio     : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 1   ; /* [5]  */
        unsigned int    dm_ctrst_thresh       : 2   ; /* [7..6]  */
        unsigned int    dm_opp_ang_ctrst_t    : 8   ; /* [15..8]  */
        unsigned int    dm_opp_ang_ctrst_div  : 2   ; /* [17..16]  */
        unsigned int    reserved_1            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_DIR;

/* Define the union U_VPSS_DM_EDGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_edge_thr           : 6   ; /* [5..0]  */
        unsigned int    dm_asymtrc_gain       : 4   ; /* [9..6]  */
        unsigned int    dm_edge_gain1         : 3   ; /* [12..10]  */
        unsigned int    reserved_0            : 1   ; /* [13]  */
        unsigned int    dm_edge_gain2         : 3   ; /* [16..14]  */
        unsigned int    reserved_1            : 1   ; /* [17]  */
        unsigned int    dm_sw_thr_gain        : 3   ; /* [20..18]  */
        unsigned int    reserved_2            : 1   ; /* [21]  */
        unsigned int    dm_global_str         : 4   ; /* [25..22]  */
        unsigned int    dm_str_idx_flag       : 1   ; /* [26]  */
        unsigned int    reserved_3            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_EDGE;

/* Define the union U_VPSS_DM_LUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_sw_wht_lut_p0      : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    dm_sw_wht_lut_p1      : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    dm_sw_wht_lut_p2      : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    dm_sw_wht_lut_p3      : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT0;

/* Define the union U_VPSS_DM_LUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_sw_wht_lut_p4      : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    dm_limit_t            : 8   ; /* [15..8]  */
        unsigned int    dm_limit_t_10         : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT1;

/* Define the union U_VPSS_DM_LUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_dir_str_gain_lut_p0 : 4   ; /* [3..0]  */
        unsigned int    dm_dir_str_gain_lut_p1 : 4   ; /* [7..4]  */
        unsigned int    dm_dir_str_gain_lut_p2 : 4   ; /* [11..8]  */
        unsigned int    dm_dir_str_gain_lut_p3 : 4   ; /* [15..12]  */
        unsigned int    dm_dir_str_gain_lut_p4 : 4   ; /* [19..16]  */
        unsigned int    dm_dir_str_gain_lut_p5 : 4   ; /* [23..20]  */
        unsigned int    dm_dir_str_gain_lut_p6 : 4   ; /* [27..24]  */
        unsigned int    dm_dir_str_gain_lut_p7 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT2;

/* Define the union U_VPSS_DM_LUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_dir_str_lut_p0     : 4   ; /* [3..0]  */
        unsigned int    dm_dir_str_lut_p1     : 4   ; /* [7..4]  */
        unsigned int    dm_dir_str_lut_p2     : 4   ; /* [11..8]  */
        unsigned int    dm_dir_str_lut_p3     : 4   ; /* [15..12]  */
        unsigned int    dm_dir_str_lut_p4     : 4   ; /* [19..16]  */
        unsigned int    dm_dir_str_lut_p5     : 4   ; /* [23..20]  */
        unsigned int    dm_dir_str_lut_p6     : 4   ; /* [27..24]  */
        unsigned int    dm_dir_str_lut_p7     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT3;

/* Define the union U_VPSS_DM_LUT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_dir_str_lut_p8     : 4   ; /* [3..0]  */
        unsigned int    dm_dir_str_lut_p9     : 4   ; /* [7..4]  */
        unsigned int    dm_dir_str_lut_p10    : 4   ; /* [11..8]  */
        unsigned int    dm_dir_str_lut_p11    : 4   ; /* [15..12]  */
        unsigned int    dm_dir_str_lut_p12    : 4   ; /* [19..16]  */
        unsigned int    dm_dir_str_lut_p13    : 4   ; /* [23..20]  */
        unsigned int    dm_dir_str_lut_p14    : 4   ; /* [27..24]  */
        unsigned int    dm_dir_str_lut_p15    : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT4;

/* Define the union U_VPSS_DM_DIRC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_dir_ctrst_t        : 8   ; /* [7..0]  */
        unsigned int    dm_dir_ctrst_t_10     : 10  ; /* [17..8]  */
        unsigned int    dm_mndir_opp_ctrst_t  : 8   ; /* [25..18]  */
        unsigned int    dm_sim_dir_pt_count_t : 4   ; /* [29..26]  */
        unsigned int    dm_dir_mmf_en         : 1   ; /* [30]  */
        unsigned int    dm_mmf_limit_en       : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_DIRC;

/* Define the union U_VPSS_DM_DIRO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_dir_opp_ctrst_t    : 8   ; /* [7..0]  */
        unsigned int    dm_dir_opp_ctrst_t_10 : 10  ; /* [17..8]  */
        unsigned int    dm_dir_stren_shift_fac : 4   ; /* [21..18]  */
        unsigned int    dm_csw_trsnt_st_10    : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_DIRO;

/* Define the union U_VPSS_DM_LSW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_lw_ctrst_t         : 8   ; /* [7..0]  */
        unsigned int    dm_lw_ctrst_t_10      : 10  ; /* [17..8]  */
        unsigned int    dm_csw_trsnt_st       : 8   ; /* [25..18]  */
        unsigned int    dm_sdbw_mode          : 1   ; /* [26]  */
        unsigned int    dm_lsw_ratio          : 3   ; /* [29..27]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LSW;

/* Define the union U_VPSS_DM_MMFLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_mmf_lr             : 9   ; /* [8..0]  */
        unsigned int    dm_mmf_lr_10          : 11  ; /* [19..9]  */
        unsigned int    dm_csw_trsnt_lt       : 8   ; /* [27..20]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_MMFLR;

/* Define the union U_VPSS_DM_MMFSR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_mmf_sr             : 9   ; /* [8..0]  */
        unsigned int    dm_mmf_sr_10          : 11  ; /* [19..9]  */
        unsigned int    dm_csw_trsnt_lt_10    : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_MMFSR;

/* Define the union U_VPSS_DM_DIRLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_dir_mmf_lr         : 9   ; /* [8..0]  */
        unsigned int    dm_dir_mmf_lr_10      : 11  ; /* [19..9]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_DIRLR;

/* Define the union U_VPSS_DM_DIRSR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_dir_mmf_sr         : 9   ; /* [8..0]  */
        unsigned int    dm_dir_mmf_sr_10      : 11  ; /* [19..9]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_DIRSR;

/* Define the union U_VPSS_DM_THR0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_limit_lsw_ratio    : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    dm_lim_res_blend_str1 : 4   ; /* [7..4]  */
        unsigned int    dm_lim_res_blend_str2 : 4   ; /* [11..8]  */
        unsigned int    dm_dir_blend_str      : 4   ; /* [15..12]  */
        unsigned int    reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_THR0;

/* Define the union U_VPSS_DM_THR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_min_lim_trans_wid  : 8   ; /* [7..0]  */
        unsigned int    dm_min_lim_val_trans  : 8   ; /* [15..8]  */
        unsigned int    dm_lim_trans_wid      : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_THR1;

/* Define the union U_VPSS_DM_THR2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_min_lim_trans_wid_10 : 10  ; /* [9..0]  */
        unsigned int    dm_min_lim_val_trans_10 : 10  ; /* [19..10]  */
        unsigned int    dm_lim_trans_wid_10   : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_THR2;

/* Define the union U_VPSS_DS_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    ds_blendgain          : 11  ; /* [11..1]  */
        unsigned int    ds_cordirthr          : 7   ; /* [18..12]  */
        unsigned int    ds_flatthrscl         : 7   ; /* [25..19]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_CTRL;

/* Define the union U_VPSS_DS_THR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_patcond6scl62      : 7   ; /* [6..0]  */
        unsigned int    ds_patcond6scl61      : 7   ; /* [13..7]  */
        unsigned int    ds_patcond5thr5       : 7   ; /* [20..14]  */
        unsigned int    ds_patcond3thr3       : 7   ; /* [27..21]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR1;

/* Define the union U_VPSS_DS_THR2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_patcond6scl66      : 7   ; /* [6..0]  */
        unsigned int    ds_patcond6scl65      : 7   ; /* [13..7]  */
        unsigned int    ds_patcond6scl64      : 7   ; /* [20..14]  */
        unsigned int    ds_patcond6scl63      : 7   ; /* [27..21]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR2;

/* Define the union U_VPSS_DS_THR3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_wincond2thr1       : 8   ; /* [7..0]  */
        unsigned int    ds_wincond1val1       : 8   ; /* [15..8]  */
        unsigned int    ds_wincond1thr1       : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR3;

/* Define the union U_VPSS_DS_THR4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_wincond2val2       : 8   ; /* [7..0]  */
        unsigned int    ds_wincond2thr2       : 8   ; /* [15..8]  */
        unsigned int    ds_wincond2val1       : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR4;

/* Define the union U_VPSS_DS_THR5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_wincond3val2       : 8   ; /* [7..0]  */
        unsigned int    ds_wincond3val1       : 8   ; /* [15..8]  */
        unsigned int    ds_wincond3thr1       : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR5;

/* Define the union U_VPSS_DS_THR6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_wincond3val4       : 9   ; /* [8..0]  */
        unsigned int    ds_wincond3val3       : 8   ; /* [16..9]  */
        unsigned int    ds_wincond3thr2       : 8   ; /* [24..17]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR6;

/* Define the union U_VPSS_DS_THR7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_wincond4thr2       : 8   ; /* [7..0]  */
        unsigned int    ds_wincond4thr1       : 8   ; /* [15..8]  */
        unsigned int    ds_wincond3scl1       : 9   ; /* [24..16]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR7;

/* Define the union U_VPSS_DS_THR8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_wincond4val3       : 8   ; /* [7..0]  */
        unsigned int    ds_wincond4val2       : 8   ; /* [15..8]  */
        unsigned int    ds_wincond4val1       : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR8;

/* Define the union U_VPSS_DS_THR9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_wincond4scl1       : 9   ; /* [8..0]  */
        unsigned int    ds_wincond4val5       : 8   ; /* [16..9]  */
        unsigned int    ds_wincond4val4       : 8   ; /* [24..17]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR9;

/* Define the union U_VPSS_DS_THR10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_wincond5val2       : 8   ; /* [7..0]  */
        unsigned int    ds_wincond5val1       : 8   ; /* [15..8]  */
        unsigned int    ds_wincond5thr1       : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR10;

/* Define the union U_VPSS_DS_THR11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_wincond5scl2       : 9   ; /* [8..0]  */
        unsigned int    ds_wincond5scl1       : 9   ; /* [17..9]  */
        unsigned int    ds_wincond5val3       : 8   ; /* [25..18]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR11;

/* Define the union U_VPSS_DS_THR12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_picresscl2         : 9   ; /* [8..0]  */
        unsigned int    ds_picresscl1         : 9   ; /* [17..9]  */
        unsigned int    ds_picresscl0         : 9   ; /* [26..18]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR12;

/* Define the union U_VPSS_DS_THR13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_picresofst         : 14  ; /* [13..0]  */
        unsigned int    ds_winactthr          : 13  ; /* [26..14]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR13;

/* Define the union U_VPSS_DS_THR14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_patcond6normofst   : 7   ; /* [6..0]  */
        unsigned int    ds_patcond6norm63     : 7   ; /* [13..7]  */
        unsigned int    ds_patcond6norm62     : 7   ; /* [20..14]  */
        unsigned int    ds_patcond6norm61     : 7   ; /* [27..21]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR14;

/* Define the union U_VPSS_DS_THR15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_patcond6clipscl6   : 7   ; /* [6..0]  */
        unsigned int    ds_patcond6norm6      : 7   ; /* [13..7]  */
        unsigned int    ds_patcond6norm5      : 7   ; /* [20..14]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR15;

/* Define the union U_VPSS_DS_THR16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_resadpten          : 1   ; /* [0]  */
        unsigned int    ds_sclctrst           : 9   ; /* [9..1]  */
        unsigned int    ds_patcond6ofst       : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR16;

/* Define the union U_VPSS_DS_THR17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_shtctrstscl2       : 9   ; /* [8..0]  */
        unsigned int    ds_shtctrstscl1       : 9   ; /* [17..9]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR17;

/* Define the union U_VPSS_DS_THR18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_cormmthr1          : 9   ; /* [8..0]  */
        unsigned int    ds_cormmthr0          : 9   ; /* [17..9]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR18;

/* Define the union U_VPSS_DS_READSTATE0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_flag_2p            : 2   ; /* [1..0]  */
        unsigned int    ds_pixeldiff_2p       : 20  ; /* [21..2]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_READSTATE0;

/* Define the union U_VPSS_DS_READSTATE1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_strctrast_2p       : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_READSTATE1;

/* Define the union U_VPSS_DS_THR19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_patcond6ofst61     : 10  ; /* [9..0]  */
        unsigned int    ds_patcond6ofst6      : 10  ; /* [19..10]  */
        unsigned int    ds_patcond6ofst5      : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR19;

/* Define the union U_VPSS_DS_THR20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ds_overshootflag      : 1   ; /* [0]  */
        unsigned int    ds_patcond6ofst62     : 10  ; /* [10..1]  */
        unsigned int    ds_patcond6ofst63     : 10  ; /* [20..11]  */
        unsigned int    ds_patcond6scl7       : 8   ; /* [28..21]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DS_THR20;

/* Define the union U_VPSS_DR_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    dr_enconpro           : 1   ; /* [1]  */
        unsigned int    dr_encondarkpro       : 1   ; /* [2]  */
        unsigned int    dr_enunderpro         : 1   ; /* [3]  */
        unsigned int    dr_enlimitpro         : 1   ; /* [4]  */
        unsigned int    dr_enfarthestsmooth   : 1   ; /* [5]  */
        unsigned int    dr_en3x3filter        : 1   ; /* [6]  */
        unsigned int    dr_ennosmoothwavepro  : 1   ; /* [7]  */
        unsigned int    reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_CTRL;

/* Define the union U_VPSS_DR_THR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_gradratio          : 7   ; /* [6..0]  */
        unsigned int    dr_shootmaxthd        : 10  ; /* [16..7]  */
        unsigned int    dr_gradmaxthd         : 10  ; /* [26..17]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR1;

/* Define the union U_VPSS_DR_THR2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_highfrequent       : 8   ; /* [7..0]  */
        unsigned int    dr_srhtrsngradbeta    : 3   ; /* [10..8]  */
        unsigned int    dr_srhtrsngradalpha   : 8   ; /* [18..11]  */
        unsigned int    dr_underratio         : 4   ; /* [22..19]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR2;

/* Define the union U_VPSS_DR_THR3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_ringx2             : 10  ; /* [9..0]  */
        unsigned int    dr_ringx1             : 10  ; /* [19..10]  */
        unsigned int    dr_ringx0             : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR3;

/* Define the union U_VPSS_DR_THR4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_ringy1             : 10  ; /* [9..0]  */
        unsigned int    dr_ringy0             : 10  ; /* [19..10]  */
        unsigned int    dr_ringx3             : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR4;

/* Define the union U_VPSS_DR_THR5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_shttornum1         : 4   ; /* [3..0]  */
        unsigned int    dr_shttornum0         : 4   ; /* [7..4]  */
        unsigned int    dr_ringy3             : 10  ; /* [17..8]  */
        unsigned int    dr_ringy2             : 10  ; /* [27..18]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR5;

/* Define the union U_VPSS_DR_THR6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_restornum          : 4   ; /* [3..0]  */
        unsigned int    dr_shttornum3         : 4   ; /* [7..4]  */
        unsigned int    dr_shttornum2         : 4   ; /* [11..8]  */
        unsigned int    dr_ringk0             : 16  ; /* [27..12]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR6;

/* Define the union U_VPSS_DR_THR7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_ringmax            : 10  ; /* [9..0]  */
        unsigned int    dr_ringk1             : 16  ; /* [25..10]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR7;

/* Define the union U_VPSS_DR_THR8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_ringmin            : 10  ; /* [9..0]  */
        unsigned int    dr_ringk2             : 16  ; /* [25..10]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR8;

/* Define the union U_VPSS_DR_THR9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_estproringthd      : 8   ; /* [7..0]  */
        unsigned int    dr_estresaltertmp     : 10  ; /* [17..8]  */
        unsigned int    dr_estresalterthd     : 10  ; /* [27..18]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR9;

/* Define the union U_VPSS_DR_THR10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_shtsmooththd2      : 8   ; /* [7..0]  */
        unsigned int    dr_shtsmooththd1      : 8   ; /* [15..8]  */
        unsigned int    dr_shtsmooththd0      : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR10;

/* Define the union U_VPSS_DR_THR11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_shtsmoothvalue2    : 8   ; /* [7..0]  */
        unsigned int    dr_shtsmoothvalue1    : 8   ; /* [15..8]  */
        unsigned int    dr_shtsmoothvalue0    : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR11;

/* Define the union U_VPSS_DR_THR12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_estprogradcoef     : 7   ; /* [6..0]  */
        unsigned int    dr_estprogradthd0     : 10  ; /* [16..7]  */
        unsigned int    dr_estprogradthd1     : 10  ; /* [26..17]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR12;

/* Define the union U_VPSS_DR_THR13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_estcomtmp          : 8   ; /* [7..0]  */
        unsigned int    dr_estcomratio2       : 6   ; /* [13..8]  */
        unsigned int    dr_estcomratio1       : 6   ; /* [19..14]  */
        unsigned int    dr_estcomthd          : 8   ; /* [27..20]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR13;

/* Define the union U_VPSS_DR_THR14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_limitwavescope1    : 8   ; /* [7..0]  */
        unsigned int    dr_limitwavescope0    : 8   ; /* [15..8]  */
        unsigned int    dr_estpixeldiff       : 11  ; /* [26..16]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR14;

/* Define the union U_VPSS_DR_THR15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_limitscope         : 8   ; /* [7..0]  */
        unsigned int    dr_limitgrad          : 10  ; /* [17..8]  */
        unsigned int    dr_limitwavenum2      : 4   ; /* [21..18]  */
        unsigned int    dr_limitwavenum1      : 4   ; /* [25..22]  */
        unsigned int    dr_limitwavenum0      : 4   ; /* [29..26]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR15;

/* Define the union U_VPSS_DR_THR16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_largewavesumblend  : 7   ; /* [6..0]  */
        unsigned int    dr_largewaveblend     : 7   ; /* [13..7]  */
        unsigned int    dr_calcsum5error      : 10  ; /* [23..14]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR16;

/* Define the union U_VPSS_DR_THR17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_wavepronumlim      : 4   ; /* [3..0]  */
        unsigned int    dr_largewaveminthd    : 10  ; /* [13..4]  */
        unsigned int    dr_calcwaverange      : 8   ; /* [21..14]  */
        unsigned int    dr_pixelrange         : 8   ; /* [29..22]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR17;

/* Define the union U_VPSS_DR_THR18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_smoothblendx2      : 8   ; /* [7..0]  */
        unsigned int    dr_smoothblendx1      : 8   ; /* [15..8]  */
        unsigned int    dr_smoothblendx0      : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR18;

/* Define the union U_VPSS_DR_THR19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_smoothblendy1      : 8   ; /* [7..0]  */
        unsigned int    dr_smoothblendy0      : 8   ; /* [15..8]  */
        unsigned int    dr_smoothblendx3      : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR19;

/* Define the union U_VPSS_DR_THR20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_smoothblendmax     : 8   ; /* [7..0]  */
        unsigned int    dr_smoothblendy3      : 8   ; /* [15..8]  */
        unsigned int    dr_smoothblendy2      : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR20;

/* Define the union U_VPSS_DR_THR21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_smoothblendmin     : 8   ; /* [7..0]  */
        unsigned int    dr_smoothblendk0      : 16  ; /* [23..8]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR21;

/* Define the union U_VPSS_DR_THR22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_noflagareablend    : 7   ; /* [6..0]  */
        unsigned int    dr_smoothblendk1      : 16  ; /* [22..7]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR22;

/* Define the union U_VPSS_DR_THR23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_pixellargeerror    : 10  ; /* [9..0]  */
        unsigned int    dr_smoothblendk2      : 16  ; /* [25..10]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR23;

/* Define the union U_VPSS_DR_THR24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_conallpixelerror   : 12  ; /* [11..0]  */
        unsigned int    dr_con5pixelerror     : 8   ; /* [19..12]  */
        unsigned int    dr_conpixelerror      : 8   ; /* [27..20]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR24;

/* Define the union U_VPSS_DR_THR25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_limitwavepronum    : 4   ; /* [3..0]  */
        unsigned int    dr_limitsmoothpronum  : 4   ; /* [7..4]  */
        unsigned int    dr_limitpixelnum      : 4   ; /* [11..8]  */
        unsigned int    dr_consumblend        : 7   ; /* [18..12]  */
        unsigned int    dr_returnblend        : 7   ; /* [25..19]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR25;

/* Define the union U_VPSS_DR_THR26 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dr_resblend           : 9   ; /* [8..0]  */
        unsigned int    dr_borderprcwidth     : 8   ; /* [16..9]  */
        unsigned int    dr_shootblend         : 7   ; /* [23..17]  */
        unsigned int    dr_crossblend         : 7   ; /* [30..24]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DR_THR26;

/* Define the union U_VPSS_SNR_ENABLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    lumasnren             : 1   ; /* [1]  */
        unsigned int    chromasnren           : 1   ; /* [2]  */
        unsigned int    edgeprelpfen          : 1   ; /* [3]  */
        unsigned int    colorweighten         : 1   ; /* [4]  */
        unsigned int    motionedgeen          : 1   ; /* [5]  */
        unsigned int    edgeonlyen            : 1   ; /* [6]  */
        unsigned int    blendingmode          : 1   ; /* [7]  */
        unsigned int    reserved_1            : 1   ; /* [8]  */
        unsigned int    demomodeen            : 1   ; /* [9]  */
        unsigned int    demomodelr            : 1   ; /* [10]  */
        unsigned int    debugmodeen           : 1   ; /* [11]  */
        unsigned int    debugmode             : 2   ; /* [13..12]  */
        unsigned int    diredgesmoothen       : 1   ; /* [14]  */
        unsigned int    reserved_2            : 4   ; /* [18..15]  */
        unsigned int    edgelpfmode           : 2   ; /* [20..19]  */
        unsigned int    meanedgeratioen       : 1   ; /* [21]  */
        unsigned int    snrfiltermode         : 1   ; /* [22]  */
        unsigned int    snrstrength           : 5   ; /* [27..23]  */
        unsigned int    reserved_3            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_ENABLE;

/* Define the union U_VPSS_SNR_EDGE_RATIOX0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    meanedgeratiox0       : 10  ; /* [9..0]  */
        unsigned int    meanedgeratiox1       : 10  ; /* [19..10]  */
        unsigned int    meanedgeratiox2       : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_RATIOX0;

/* Define the union U_VPSS_SNR_EDGE_RATIOX1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    meanedgeratiox3       : 10  ; /* [9..0]  */
        unsigned int    meanedgeratiox4       : 10  ; /* [19..10]  */
        unsigned int    meanedgeratiox5       : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_RATIOX1;

/* Define the union U_VPSS_SNR_EDGE_RATIOY0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    meanedgeratioy0       : 7   ; /* [6..0]  */
        unsigned int    meanedgeratioy1       : 7   ; /* [13..7]  */
        unsigned int    meanedgeratioy2       : 7   ; /* [20..14]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_RATIOY0;

/* Define the union U_VPSS_SNR_EDGE_RATIOY1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    meanedgeratioy3       : 7   ; /* [6..0]  */
        unsigned int    meanedgeratioy4       : 7   ; /* [13..7]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_RATIOY1;

/* Define the union U_VPSS_SNR_EDGE_RATIOK0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    meanedgeratiok0       : 8   ; /* [7..0]  */
        unsigned int    meanedgeratiok1       : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_RATIOK0;

/* Define the union U_VPSS_SNR_EDGE_RATIOK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    meanedgeratiok2       : 8   ; /* [7..0]  */
        unsigned int    meanedgeratiok3       : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_RATIOK1;

/* Define the union U_VPSS_SNR_EDGE_RATIO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    meanedgeratiomax      : 7   ; /* [6..0]  */
        unsigned int    meanedgeratiomin      : 7   ; /* [13..7]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_RATIO;

/* Define the union U_VPSS_SNR_EDGE_STRX0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgestrmappingx0      : 10  ; /* [9..0]  */
        unsigned int    edgestrmappingx1      : 10  ; /* [19..10]  */
        unsigned int    edgestrmappingx2      : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_STRX0;

/* Define the union U_VPSS_SNR_EDGE_STRX1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgestrmappingx3      : 10  ; /* [9..0]  */
        unsigned int    edgestrmappingx4      : 10  ; /* [19..10]  */
        unsigned int    edgestrmappingx5      : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_STRX1;

/* Define the union U_VPSS_SNR_EDGE_STRX2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgestrmappingx6      : 10  ; /* [9..0]  */
        unsigned int    edgestrmappingx7      : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_STRX2;

/* Define the union U_VPSS_SNR_EDGE_STRY0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgestrmappingy0      : 6   ; /* [5..0]  */
        unsigned int    edgestrmappingy1      : 6   ; /* [11..6]  */
        unsigned int    edgestrmappingy2      : 6   ; /* [17..12]  */
        unsigned int    edgestrmappingy3      : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_STRY0;

/* Define the union U_VPSS_SNR_EDGE_STRY1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgestrmappingy4      : 6   ; /* [5..0]  */
        unsigned int    edgestrmappingy5      : 6   ; /* [11..6]  */
        unsigned int    edgestrmappingy6      : 6   ; /* [17..12]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_STRY1;

/* Define the union U_VPSS_SNR_EDGE_STRK0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgestrmappingk0      : 8   ; /* [7..0]  */
        unsigned int    edgestrmappingk1      : 8   ; /* [15..8]  */
        unsigned int    edgestrmappingk2      : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_STRK0;

/* Define the union U_VPSS_SNR_EDGE_STRK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgestrmappingk3      : 8   ; /* [7..0]  */
        unsigned int    edgestrmappingk4      : 8   ; /* [15..8]  */
        unsigned int    edgestrmappingk5      : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_STRK1;

/* Define the union U_VPSS_SNR_EDGE_STR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgestrmappingmin     : 6   ; /* [5..0]  */
        unsigned int    edgestrmappingmax     : 6   ; /* [11..6]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_STR;

/* Define the union U_VPSS_SNR_SFYMAX_VALTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sfymaxedgevalthd1     : 5   ; /* [4..0]  */
        unsigned int    sfymaxedgevalthd2     : 5   ; /* [9..5]  */
        unsigned int    sfymaxedgevalthd3     : 5   ; /* [14..10]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_SFYMAX_VALTH;

/* Define the union U_VPSS_SNR_SFYMAX_IDXTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sfymaxedgeidxthd1     : 4   ; /* [3..0]  */
        unsigned int    sfymaxedgeidxthd2     : 4   ; /* [7..4]  */
        unsigned int    sfymaxedgeidxthd3     : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_SFYMAX_IDXTH;

/* Define the union U_VPSS_SNR_SFYMEAN_VALTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sfymeanedgevalthd1    : 5   ; /* [4..0]  */
        unsigned int    sfymeanedgevalthd2    : 5   ; /* [9..5]  */
        unsigned int    sfymeanedgevalthd3    : 5   ; /* [14..10]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_SFYMEAN_VALTH;

/* Define the union U_VPSS_SNR_WFYMAX_VALTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wfymaxedgevalthd1     : 5   ; /* [4..0]  */
        unsigned int    wfymaxedgevalthd2     : 5   ; /* [9..5]  */
        unsigned int    wfymaxedgevalthd3     : 5   ; /* [14..10]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_WFYMAX_VALTH;

/* Define the union U_VPSS_SNR_WFYMAX_IDXTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wfymaxedgeidxthd1     : 4   ; /* [3..0]  */
        unsigned int    wfymaxedgeidxthd2     : 4   ; /* [7..4]  */
        unsigned int    wfymaxedgeidxthd3     : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_WFYMAX_IDXTH;

/* Define the union U_VPSS_SNR_WFYMEAN_VALTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wfymeanedgevalthd1    : 5   ; /* [4..0]  */
        unsigned int    wfymeanedgevalthd2    : 5   ; /* [9..5]  */
        unsigned int    wfymeanedgevalthd3    : 5   ; /* [14..10]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_WFYMEAN_VALTH;

/* Define the union U_VPSS_SNR_SFCMAX_VALTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sfcmaxedgevalthd1     : 5   ; /* [4..0]  */
        unsigned int    sfcmaxedgevalthd2     : 5   ; /* [9..5]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_SFCMAX_VALTH;

/* Define the union U_VPSS_SNR_SFCMAX_IDXTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sfcmaxedgeidxthd1     : 4   ; /* [3..0]  */
        unsigned int    sfcmaxedgeidxthd2     : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_SFCMAX_IDXTH;

/* Define the union U_VPSS_SNR_SFCMEAN_VALTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sfcmeanedgevalthd1    : 5   ; /* [4..0]  */
        unsigned int    sfcmeanedgevalthd2    : 5   ; /* [9..5]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_SFCMEAN_VALTH;

/* Define the union U_VPSS_SNR_WFCMAX_VALTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wfcmaxedgevalthd1     : 5   ; /* [4..0]  */
        unsigned int    wfcmaxedgevalthd2     : 5   ; /* [9..5]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_WFCMAX_VALTH;

/* Define the union U_VPSS_SNR_WFCMAX_IDXTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wfcmaxedgeidxthd1     : 4   ; /* [3..0]  */
        unsigned int    wfcmaxedgeidxthd2     : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_WFCMAX_IDXTH;

/* Define the union U_VPSS_SNR_WFCMEAN_VALTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wfcmeanedgevalthd1    : 5   ; /* [4..0]  */
        unsigned int    wfcmeanedgevalthd2    : 5   ; /* [9..5]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_WFCMEAN_VALTH;

/* Define the union U_VPSS_SNR_FILTER_TYPE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ydefaultfiltertype    : 3   ; /* [2..0]  */
        unsigned int    cdefaultfiltertype    : 3   ; /* [5..3]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_FILTER_TYPE;

/* Define the union U_VPSS_SNR_NOISE_CORE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    noisecore             : 5   ; /* [4..0]  */
        unsigned int    nonoisegain           : 6   ; /* [10..5]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_NOISE_CORE;

/* Define the union U_VPSS_SNR_ADD_NOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    addrandyinnoiseen     : 1   ; /* [0]  */
        unsigned int    randyinnoiselevel     : 5   ; /* [5..1]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_ADD_NOISE;

/* Define the union U_VPSS_SNR_ABS_SUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgeindicatorsel      : 4   ; /* [3..0]  */
        unsigned int    sclabssum             : 12  ; /* [15..4]  */
        unsigned int    diffabssumofst        : 12  ; /* [27..16]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_ABS_SUM;

/* Define the union U_VPSS_SNR_WIN_ACT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    winactthr             : 16  ; /* [15..0]  */
        unsigned int    winactdirdiffthr      : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_WIN_ACT;

/* Define the union U_VPSS_SNR_DIR_THR0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pixdirstrthr0         : 10  ; /* [9..0]  */
        unsigned int    pixdirstrthr1         : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_DIR_THR0;

/* Define the union U_VPSS_SNR_DIR_THR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pixdirstrthr2         : 10  ; /* [9..0]  */
        unsigned int    pixdirstrthr3         : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_DIR_THR1;

/* Define the union U_VPSS_SNR_DIR_THR2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pixdirstrthr4         : 10  ; /* [9..0]  */
        unsigned int    pixdirstrthr5         : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_DIR_THR2;

/* Define the union U_VPSS_SNR_DIR_THR3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pixdirstrthr6         : 10  ; /* [9..0]  */
        unsigned int    pixdirstrthr7         : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_DIR_THR3;

/* Define the union U_VPSS_SNR_THR_DIR0A */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntmintenthrdir0 : 10  ; /* [9..0]  */
        unsigned int    mediff3thrdir0        : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR0A;

/* Define the union U_VPSS_SNR_THR_DIR0B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntminnorthrdir0 : 10  ; /* [9..0]  */
        unsigned int    winmeanhordiffthrdir0 : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR0B;

/* Define the union U_VPSS_SNR_THR_DIR1A */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntmintenthrdir1 : 10  ; /* [9..0]  */
        unsigned int    mediff3thrdir1        : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR1A;

/* Define the union U_VPSS_SNR_THR_DIR1B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntminnorthrdir1 : 10  ; /* [9..0]  */
        unsigned int    winmeanhordiffthrdir1 : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR1B;

/* Define the union U_VPSS_SNR_THR_DIR2A */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntmintenthrdir2 : 10  ; /* [9..0]  */
        unsigned int    mediff3thrdir2        : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR2A;

/* Define the union U_VPSS_SNR_THR_DIR2B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntminnorthrdir2 : 10  ; /* [9..0]  */
        unsigned int    winmeanhordiffthrdir2 : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR2B;

/* Define the union U_VPSS_SNR_THR_DIR3A */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntmintenthrdir3 : 10  ; /* [9..0]  */
        unsigned int    mediff3thrdir3        : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR3A;

/* Define the union U_VPSS_SNR_THR_DIR3B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntminnorthrdir3 : 10  ; /* [9..0]  */
        unsigned int    winmeanhordiffthrdir3 : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR3B;

/* Define the union U_VPSS_SNR_THR_DIR4A */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntmintenthrdir4 : 10  ; /* [9..0]  */
        unsigned int    mediff3thrdir4        : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR4A;

/* Define the union U_VPSS_SNR_THR_DIR4B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntminnorthrdir4 : 10  ; /* [9..0]  */
        unsigned int    winmeanhordiffthrdir4 : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR4B;

/* Define the union U_VPSS_SNR_THR_DIR5A */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntmintenthrdir5 : 10  ; /* [9..0]  */
        unsigned int    mediff3thrdir5        : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR5A;

/* Define the union U_VPSS_SNR_THR_DIR5B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntminnorthrdir5 : 10  ; /* [9..0]  */
        unsigned int    winmeanhordiffthrdir5 : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR5B;

/* Define the union U_VPSS_SNR_THR_DIR6A */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntmintenthrdir6 : 10  ; /* [9..0]  */
        unsigned int    mediff3thrdir6        : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR6A;

/* Define the union U_VPSS_SNR_THR_DIR6B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntminnorthrdir6 : 10  ; /* [9..0]  */
        unsigned int    winmeanhordiffthrdir6 : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR6B;

/* Define the union U_VPSS_SNR_THR_DIR7A */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntmintenthrdir7 : 10  ; /* [9..0]  */
        unsigned int    mediff3thrdir7        : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR7A;

/* Define the union U_VPSS_SNR_THR_DIR7B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mediffcntminnorthrdir7 : 10  ; /* [9..0]  */
        unsigned int    winmeanhordiffthrdir7 : 12  ; /* [21..10]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_THR_DIR7B;

/* Define the union U_VPSS_SNR_DEMO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    demomodecoor          : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_DEMO;

/* Define the union U_MOTION_EDGE_LUT_01a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_0__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_0__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_0__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_0__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_01a;

/* Define the union U_MOTION_EDGE_LUT_01b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_0__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_0__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_0__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_0__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_01b;

/* Define the union U_MOTION_EDGE_LUT_01c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_0__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_0__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_0__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_0__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_01c;

/* Define the union U_MOTION_EDGE_LUT_01d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_0__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_0__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_0__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_0__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_01d;

/* Define the union U_MOTION_EDGE_LUT_01e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_0__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_0__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_0__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_0__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_01e;

/* Define the union U_MOTION_EDGE_LUT_01f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_0__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_0__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_0__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_0__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_01f;

/* Define the union U_MOTION_EDGE_LUT_01g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_0__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_0__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_0__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_0__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_01g;

/* Define the union U_MOTION_EDGE_LUT_01h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_0__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_0__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_0__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_0__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_01h;

/* Define the union U_MOTION_EDGE_LUT_02a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_1__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_1__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_1__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_1__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_02a;

/* Define the union U_MOTION_EDGE_LUT_02b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_1__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_1__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_1__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_1__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_02b;

/* Define the union U_MOTION_EDGE_LUT_02c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_1__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_1__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_1__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_1__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_02c;

/* Define the union U_MOTION_EDGE_LUT_02d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_1__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_1__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_1__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_1__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_02d;

/* Define the union U_MOTION_EDGE_LUT_02e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_1__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_1__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_1__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_1__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_02e;

/* Define the union U_MOTION_EDGE_LUT_02f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_1__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_1__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_1__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_1__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_02f;

/* Define the union U_MOTION_EDGE_LUT_02g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_1__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_1__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_1__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_1__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_02g;

/* Define the union U_MOTION_EDGE_LUT_02h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_1__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_1__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_1__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_1__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_02h;

/* Define the union U_MOTION_EDGE_LUT_03a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_2__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_2__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_2__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_2__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_03a;

/* Define the union U_MOTION_EDGE_LUT_03b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_2__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_2__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_2__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_2__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_03b;

/* Define the union U_MOTION_EDGE_LUT_03c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_2__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_2__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_2__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_2__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_03c;

/* Define the union U_MOTION_EDGE_LUT_03d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_2__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_2__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_2__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_2__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_03d;

/* Define the union U_MOTION_EDGE_LUT_03e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_2__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_2__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_2__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_2__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_03e;

/* Define the union U_MOTION_EDGE_LUT_03f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_2__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_2__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_2__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_2__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_03f;

/* Define the union U_MOTION_EDGE_LUT_03g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_2__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_2__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_2__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_2__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_03g;

/* Define the union U_MOTION_EDGE_LUT_03h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_2__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_2__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_2__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_2__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_03h;

/* Define the union U_MOTION_EDGE_LUT_04a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_3__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_3__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_3__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_3__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_04a;

/* Define the union U_MOTION_EDGE_LUT_04b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_3__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_3__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_3__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_3__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_04b;

/* Define the union U_MOTION_EDGE_LUT_04c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_3__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_3__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_3__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_3__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_04c;

/* Define the union U_MOTION_EDGE_LUT_04d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_3__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_3__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_3__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_3__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_04d;

/* Define the union U_MOTION_EDGE_LUT_04e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_3__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_3__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_3__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_3__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_04e;

/* Define the union U_MOTION_EDGE_LUT_04f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_3__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_3__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_3__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_3__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_04f;

/* Define the union U_MOTION_EDGE_LUT_04g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_3__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_3__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_3__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_3__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_04g;

/* Define the union U_MOTION_EDGE_LUT_04h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_3__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_3__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_3__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_3__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_04h;

/* Define the union U_MOTION_EDGE_LUT_05a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_4__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_4__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_4__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_4__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_05a;

/* Define the union U_MOTION_EDGE_LUT_05b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_4__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_4__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_4__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_4__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_05b;

/* Define the union U_MOTION_EDGE_LUT_05c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_4__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_4__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_4__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_4__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_05c;

/* Define the union U_MOTION_EDGE_LUT_05d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_4__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_4__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_4__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_4__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_05d;

/* Define the union U_MOTION_EDGE_LUT_05e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_4__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_4__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_4__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_4__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_05e;

/* Define the union U_MOTION_EDGE_LUT_05f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_4__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_4__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_4__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_4__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_05f;

/* Define the union U_MOTION_EDGE_LUT_05g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_4__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_4__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_4__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_4__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_05g;

/* Define the union U_MOTION_EDGE_LUT_05h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_4__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_4__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_4__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_4__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_05h;

/* Define the union U_MOTION_EDGE_LUT_06a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_5__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_5__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_5__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_5__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_06a;

/* Define the union U_MOTION_EDGE_LUT_06b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_5__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_5__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_5__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_5__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_06b;

/* Define the union U_MOTION_EDGE_LUT_06c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_5__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_5__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_5__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_5__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_06c;

/* Define the union U_MOTION_EDGE_LUT_06d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_5__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_5__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_5__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_5__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_06d;

/* Define the union U_MOTION_EDGE_LUT_06e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_5__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_5__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_5__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_5__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_06e;

/* Define the union U_MOTION_EDGE_LUT_06f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_5__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_5__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_5__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_5__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_06f;

/* Define the union U_MOTION_EDGE_LUT_06g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_5__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_5__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_5__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_5__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_06g;

/* Define the union U_MOTION_EDGE_LUT_06h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_5__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_5__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_5__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_5__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_06h;

/* Define the union U_MOTION_EDGE_LUT_07a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_6__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_6__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_6__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_6__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_07a;

/* Define the union U_MOTION_EDGE_LUT_07b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_6__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_6__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_6__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_6__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_07b;

/* Define the union U_MOTION_EDGE_LUT_07c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_6__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_6__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_6__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_6__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_07c;

/* Define the union U_MOTION_EDGE_LUT_07d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_6__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_6__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_6__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_6__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_07d;

/* Define the union U_MOTION_EDGE_LUT_07e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_6__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_6__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_6__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_6__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_07e;

/* Define the union U_MOTION_EDGE_LUT_07f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_6__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_6__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_6__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_6__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_07f;

/* Define the union U_MOTION_EDGE_LUT_07g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_6__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_6__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_6__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_6__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_07g;

/* Define the union U_MOTION_EDGE_LUT_07h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_6__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_6__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_6__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_6__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_07h;

/* Define the union U_MOTION_EDGE_LUT_08a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_7__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_7__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_7__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_7__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_08a;

/* Define the union U_MOTION_EDGE_LUT_08b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_7__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_7__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_7__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_7__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_08b;

/* Define the union U_MOTION_EDGE_LUT_08c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_7__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_7__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_7__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_7__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_08c;

/* Define the union U_MOTION_EDGE_LUT_08d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_7__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_7__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_7__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_7__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_08d;

/* Define the union U_MOTION_EDGE_LUT_08e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_7__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_7__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_7__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_7__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_08e;

/* Define the union U_MOTION_EDGE_LUT_08f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_7__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_7__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_7__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_7__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_08f;

/* Define the union U_MOTION_EDGE_LUT_08g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_7__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_7__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_7__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_7__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_08g;

/* Define the union U_MOTION_EDGE_LUT_08h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_7__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_7__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_7__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_7__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_08h;

/* Define the union U_MOTION_EDGE_LUT_09a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_8__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_8__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_8__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_8__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_09a;

/* Define the union U_MOTION_EDGE_LUT_09b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_8__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_8__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_8__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_8__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_09b;

/* Define the union U_MOTION_EDGE_LUT_09c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_8__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_8__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_8__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_8__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_09c;

/* Define the union U_MOTION_EDGE_LUT_09d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_8__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_8__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_8__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_8__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_09d;

/* Define the union U_MOTION_EDGE_LUT_09e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_8__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_8__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_8__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_8__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_09e;

/* Define the union U_MOTION_EDGE_LUT_09f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_8__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_8__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_8__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_8__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_09f;

/* Define the union U_MOTION_EDGE_LUT_09g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_8__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_8__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_8__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_8__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_09g;

/* Define the union U_MOTION_EDGE_LUT_09h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_8__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_8__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_8__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_8__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_09h;

/* Define the union U_MOTION_EDGE_LUT_10a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_9__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_9__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_9__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_9__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_10a;

/* Define the union U_MOTION_EDGE_LUT_10b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_9__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_9__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_9__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_9__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_10b;

/* Define the union U_MOTION_EDGE_LUT_10c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_9__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_9__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_9__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_9__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_10c;

/* Define the union U_MOTION_EDGE_LUT_10d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_9__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_9__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_9__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_9__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_10d;

/* Define the union U_MOTION_EDGE_LUT_10e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_9__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_9__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_9__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_9__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_10e;

/* Define the union U_MOTION_EDGE_LUT_10f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_9__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_9__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_9__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_9__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_10f;

/* Define the union U_MOTION_EDGE_LUT_10g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_9__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_9__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_9__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_9__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_10g;

/* Define the union U_MOTION_EDGE_LUT_10h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_9__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_9__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_9__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_9__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_10h;

/* Define the union U_MOTION_EDGE_LUT_11a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_10__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_10__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_10__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_10__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_11a;

/* Define the union U_MOTION_EDGE_LUT_11b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_10__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_10__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_10__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_10__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_11b;

/* Define the union U_MOTION_EDGE_LUT_11c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_10__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_10__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_10__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_10__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_11c;

/* Define the union U_MOTION_EDGE_LUT_11d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_10__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_10__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_10__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_10__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_11d;

/* Define the union U_MOTION_EDGE_LUT_11e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_10__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_10__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_10__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_10__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_11e;

/* Define the union U_MOTION_EDGE_LUT_11f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_10__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_10__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_10__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_10__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_11f;

/* Define the union U_MOTION_EDGE_LUT_11g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_10__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_10__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_10__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_10__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_11g;

/* Define the union U_MOTION_EDGE_LUT_11h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_10__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_10__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_10__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_10__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_11h;

/* Define the union U_MOTION_EDGE_LUT_12a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_11__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_11__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_11__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_11__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_12a;

/* Define the union U_MOTION_EDGE_LUT_12b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_11__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_11__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_11__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_11__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_12b;

/* Define the union U_MOTION_EDGE_LUT_12c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_11__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_11__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_11__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_11__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_12c;

/* Define the union U_MOTION_EDGE_LUT_12d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_11__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_11__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_11__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_11__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_12d;

/* Define the union U_MOTION_EDGE_LUT_12e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_11__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_11__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_11__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_11__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_12e;

/* Define the union U_MOTION_EDGE_LUT_12f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_11__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_11__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_11__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_11__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_12f;

/* Define the union U_MOTION_EDGE_LUT_12g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_11__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_11__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_11__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_11__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_12g;

/* Define the union U_MOTION_EDGE_LUT_12h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_11__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_11__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_11__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_11__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_12h;

/* Define the union U_MOTION_EDGE_LUT_13a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_12__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_12__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_12__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_12__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_13a;

/* Define the union U_MOTION_EDGE_LUT_13b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_12__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_12__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_12__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_12__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_13b;

/* Define the union U_MOTION_EDGE_LUT_13c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_12__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_12__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_12__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_12__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_13c;

/* Define the union U_MOTION_EDGE_LUT_13d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_12__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_12__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_12__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_12__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_13d;

/* Define the union U_MOTION_EDGE_LUT_13e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_12__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_12__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_12__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_12__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_13e;

/* Define the union U_MOTION_EDGE_LUT_13f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_12__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_12__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_12__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_12__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_13f;

/* Define the union U_MOTION_EDGE_LUT_13g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_12__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_12__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_12__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_12__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_13g;

/* Define the union U_MOTION_EDGE_LUT_13h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_12__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_12__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_12__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_12__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_13h;

/* Define the union U_MOTION_EDGE_LUT_14a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_13__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_13__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_13__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_13__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_14a;

/* Define the union U_MOTION_EDGE_LUT_14b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_13__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_13__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_13__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_13__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_14b;

/* Define the union U_MOTION_EDGE_LUT_14c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_13__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_13__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_13__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_13__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_14c;

/* Define the union U_MOTION_EDGE_LUT_14d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_13__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_13__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_13__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_13__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_14d;

/* Define the union U_MOTION_EDGE_LUT_14e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_13__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_13__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_13__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_13__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_14e;

/* Define the union U_MOTION_EDGE_LUT_14f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_13__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_13__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_13__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_13__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_14f;

/* Define the union U_MOTION_EDGE_LUT_14g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_13__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_13__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_13__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_13__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_14g;

/* Define the union U_MOTION_EDGE_LUT_14h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_13__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_13__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_13__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_13__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_14h;

/* Define the union U_MOTION_EDGE_LUT_15a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_14__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_14__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_14__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_14__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_15a;

/* Define the union U_MOTION_EDGE_LUT_15b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_14__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_14__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_14__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_14__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_15b;

/* Define the union U_MOTION_EDGE_LUT_15c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_14__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_14__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_14__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_14__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_15c;

/* Define the union U_MOTION_EDGE_LUT_15d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_14__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_14__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_14__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_14__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_15d;

/* Define the union U_MOTION_EDGE_LUT_15e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_14__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_14__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_14__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_14__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_15e;

/* Define the union U_MOTION_EDGE_LUT_15f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_14__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_14__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_14__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_14__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_15f;

/* Define the union U_MOTION_EDGE_LUT_15g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_14__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_14__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_14__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_14__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_15g;

/* Define the union U_MOTION_EDGE_LUT_15h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_14__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_14__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_14__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_14__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_15h;

/* Define the union U_MOTION_EDGE_LUT_16a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_15__0_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_15__1_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_15__2_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_15__3_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_16a;

/* Define the union U_MOTION_EDGE_LUT_16b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_15__4_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_15__5_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_15__6_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_15__7_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_16b;

/* Define the union U_MOTION_EDGE_LUT_16c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_15__8_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_15__9_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_15__10_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_15__11_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_16c;

/* Define the union U_MOTION_EDGE_LUT_16d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_15__12_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_15__13_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_15__14_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_15__15_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_16d;

/* Define the union U_MOTION_EDGE_LUT_16e */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_15__16_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_15__17_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_15__18_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_15__19_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_16e;

/* Define the union U_MOTION_EDGE_LUT_16f */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_15__20_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_15__21_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_15__22_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_15__23_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_16f;

/* Define the union U_MOTION_EDGE_LUT_16g */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_15__24_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_15__25_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_15__26_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_15__27_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_16g;

/* Define the union U_MOTION_EDGE_LUT_16h */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrmotionedgelut_15__28_ : 5   ; /* [4..0]  */
        unsigned int    snrmotionedgelut_15__29_ : 5   ; /* [9..5]  */
        unsigned int    snrmotionedgelut_15__30_ : 5   ; /* [14..10]  */
        unsigned int    snrmotionedgelut_15__31_ : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_16h;

/* Define the union U_COLOR_BASED_LUT_01a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_0__0_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_0__1_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_0__2_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_0__3_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_01a;

/* Define the union U_COLOR_BASED_LUT_01b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_0__4_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_0__5_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_0__6_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_0__7_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_01b;

/* Define the union U_COLOR_BASED_LUT_01c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_0__8_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_0__9_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_0__10_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_0__11_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_01c;

/* Define the union U_COLOR_BASED_LUT_01d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_0__12_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_0__13_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_0__14_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_0__15_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_01d;

/* Define the union U_COLOR_BASED_LUT_02a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_1__0_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_1__1_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_1__2_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_1__3_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_02a;

/* Define the union U_COLOR_BASED_LUT_02b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_1__4_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_1__5_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_1__6_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_1__7_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_02b;

/* Define the union U_COLOR_BASED_LUT_02c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_1__8_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_1__9_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_1__10_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_1__11_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_02c;

/* Define the union U_COLOR_BASED_LUT_02d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_1__12_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_1__13_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_1__14_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_1__15_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_02d;

/* Define the union U_COLOR_BASED_LUT_03a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_2__0_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_2__1_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_2__2_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_2__3_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_03a;

/* Define the union U_COLOR_BASED_LUT_03b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_2__4_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_2__5_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_2__6_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_2__7_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_03b;

/* Define the union U_COLOR_BASED_LUT_03c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_2__8_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_2__9_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_2__10_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_2__11_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_03c;

/* Define the union U_COLOR_BASED_LUT_03d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_2__12_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_2__13_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_2__14_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_2__15_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_03d;

/* Define the union U_COLOR_BASED_LUT_04a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_3__0_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_3__1_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_3__2_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_3__3_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_04a;

/* Define the union U_COLOR_BASED_LUT_04b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_3__4_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_3__5_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_3__6_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_3__7_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_04b;

/* Define the union U_COLOR_BASED_LUT_04c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_3__8_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_3__9_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_3__10_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_3__11_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_04c;

/* Define the union U_COLOR_BASED_LUT_04d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_3__12_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_3__13_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_3__14_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_3__15_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_04d;

/* Define the union U_COLOR_BASED_LUT_05a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_4__0_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_4__1_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_4__2_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_4__3_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_05a;

/* Define the union U_COLOR_BASED_LUT_05b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_4__4_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_4__5_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_4__6_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_4__7_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_05b;

/* Define the union U_COLOR_BASED_LUT_05c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_4__8_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_4__9_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_4__10_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_4__11_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_05c;

/* Define the union U_COLOR_BASED_LUT_05d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_4__12_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_4__13_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_4__14_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_4__15_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_05d;

/* Define the union U_COLOR_BASED_LUT_06a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_5__0_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_5__1_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_5__2_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_5__3_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_06a;

/* Define the union U_COLOR_BASED_LUT_06b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_5__4_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_5__5_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_5__6_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_5__7_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_06b;

/* Define the union U_COLOR_BASED_LUT_06c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_5__8_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_5__9_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_5__10_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_5__11_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_06c;

/* Define the union U_COLOR_BASED_LUT_06d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_5__12_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_5__13_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_5__14_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_5__15_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_06d;

/* Define the union U_COLOR_BASED_LUT_07a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_6__0_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_6__1_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_6__2_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_6__3_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_07a;

/* Define the union U_COLOR_BASED_LUT_07b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_6__4_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_6__5_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_6__6_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_6__7_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_07b;

/* Define the union U_COLOR_BASED_LUT_07c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_6__8_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_6__9_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_6__10_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_6__11_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_07c;

/* Define the union U_COLOR_BASED_LUT_07d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_6__12_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_6__13_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_6__14_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_6__15_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_07d;

/* Define the union U_COLOR_BASED_LUT_08a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_7__0_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_7__1_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_7__2_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_7__3_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_08a;

/* Define the union U_COLOR_BASED_LUT_08b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_7__4_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_7__5_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_7__6_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_7__7_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_08b;

/* Define the union U_COLOR_BASED_LUT_08c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_7__8_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_7__9_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_7__10_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_7__11_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_08c;

/* Define the union U_COLOR_BASED_LUT_08d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_7__12_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_7__13_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_7__14_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_7__15_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_08d;

/* Define the union U_COLOR_BASED_LUT_09a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_8__0_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_8__1_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_8__2_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_8__3_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_09a;

/* Define the union U_COLOR_BASED_LUT_09b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_8__4_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_8__5_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_8__6_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_8__7_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_09b;

/* Define the union U_COLOR_BASED_LUT_09c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_8__8_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_8__9_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_8__10_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_8__11_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_09c;

/* Define the union U_COLOR_BASED_LUT_09d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_8__12_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_8__13_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_8__14_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_8__15_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_09d;

/* Define the union U_COLOR_BASED_LUT_10a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_9__0_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_9__1_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_9__2_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_9__3_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_10a;

/* Define the union U_COLOR_BASED_LUT_10b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_9__4_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_9__5_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_9__6_      : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_9__7_      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_10b;

/* Define the union U_COLOR_BASED_LUT_10c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_9__8_      : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_9__9_      : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_9__10_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_9__11_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_10c;

/* Define the union U_COLOR_BASED_LUT_10d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_9__12_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_9__13_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_9__14_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_9__15_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_10d;

/* Define the union U_COLOR_BASED_LUT_11a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_10__0_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_10__1_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_10__2_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_10__3_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_11a;

/* Define the union U_COLOR_BASED_LUT_11b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_10__4_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_10__5_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_10__6_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_10__7_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_11b;

/* Define the union U_COLOR_BASED_LUT_11c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_10__8_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_10__9_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_10__10_    : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_10__11_    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_11c;

/* Define the union U_COLOR_BASED_LUT_11d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_10__12_    : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_10__13_    : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_10__14_    : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_10__15_    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_11d;

/* Define the union U_COLOR_BASED_LUT_12a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_11__0_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_11__1_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_11__2_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_11__3_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_12a;

/* Define the union U_COLOR_BASED_LUT_12b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_11__4_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_11__5_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_11__6_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_11__7_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_12b;

/* Define the union U_COLOR_BASED_LUT_12c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_11__8_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_11__9_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_11__10_    : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_11__11_    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_12c;

/* Define the union U_COLOR_BASED_LUT_12d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_11__12_    : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_11__13_    : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_11__14_    : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_11__15_    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_12d;

/* Define the union U_COLOR_BASED_LUT_13a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_12__0_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_12__1_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_12__2_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_12__3_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_13a;

/* Define the union U_COLOR_BASED_LUT_13b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_12__4_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_12__5_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_12__6_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_12__7_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_13b;

/* Define the union U_COLOR_BASED_LUT_13c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_12__8_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_12__9_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_12__10_    : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_12__11_    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_13c;

/* Define the union U_COLOR_BASED_LUT_13d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_12__12_    : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_12__13_    : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_12__14_    : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_12__15_    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_13d;

/* Define the union U_COLOR_BASED_LUT_14a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_13__0_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_13__1_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_13__2_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_13__3_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_14a;

/* Define the union U_COLOR_BASED_LUT_14b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_13__4_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_13__5_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_13__6_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_13__7_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_14b;

/* Define the union U_COLOR_BASED_LUT_14c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_13__8_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_13__9_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_13__10_    : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_13__11_    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_14c;

/* Define the union U_COLOR_BASED_LUT_14d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_13__12_    : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_13__13_    : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_13__14_    : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_13__15_    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_14d;

/* Define the union U_COLOR_BASED_LUT_15a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_14__0_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_14__1_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_14__2_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_14__3_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_15a;

/* Define the union U_COLOR_BASED_LUT_15b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_14__4_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_14__5_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_14__6_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_14__7_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_15b;

/* Define the union U_COLOR_BASED_LUT_15c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_14__8_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_14__9_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_14__10_    : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_14__11_    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_15c;

/* Define the union U_COLOR_BASED_LUT_15d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_14__12_    : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_14__13_    : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_14__14_    : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_14__15_    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_15d;

/* Define the union U_COLOR_BASED_LUT_16a */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_15__0_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_15__1_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_15__2_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_15__3_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_16a;

/* Define the union U_COLOR_BASED_LUT_16b */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_15__4_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_15__5_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_15__6_     : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_15__7_     : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_16b;

/* Define the union U_COLOR_BASED_LUT_16c */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_15__8_     : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_15__9_     : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_15__10_    : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_15__11_    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_16c;

/* Define the union U_COLOR_BASED_LUT_16d */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    snrcbcrlut_15__12_    : 5   ; /* [4..0]  */
        unsigned int    snrcbcrlut_15__13_    : 5   ; /* [9..5]  */
        unsigned int    snrcbcrlut_15__14_    : 5   ; /* [14..10]  */
        unsigned int    snrcbcrlut_15__15_    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_COLOR_BASED_LUT_16d;

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

/* Define the union U_VPSS_TNR_MARKET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    market_coordinate     : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MARKET;

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

/* Define the union U_VPSS_DEBUG22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpss_zme_done         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DEBUG22;

/* Define the union U_VPSS_DEBUG27 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmdlen_cnt_wr_chn     : 8   ; /* [7..0]  */
        unsigned int    cmd_cnt_wr_chn        : 8   ; /* [15..8]  */
        unsigned int    cmdlen_cnt_rd_chn     : 8   ; /* [23..16]  */
        unsigned int    cmd_cnt_rd_chn        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DEBUG27;

#if 0
/* Define the union U_SMMU_SCR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    glb_bypass            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 2   ; /* [2..1]  */
        unsigned int    int_en                : 1   ; /* [3]  */
        unsigned int    reserved_1            : 12  ; /* [15..4]  */
        unsigned int    ptw_pf                : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_SCR;

/* Define the union U_SMMU_LP_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    auto_clk_gt_en        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 1   ; /* [1]  */
        unsigned int    smmu_idle             : 1   ; /* [2]  */
        unsigned int    reserved_1            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_LP_CTRL;

/* Define the union U_SMMU_INTMASK_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ints_tlbmiss_msk      : 1   ; /* [0]  */
        unsigned int    ints_ptw_trans_msk    : 1   ; /* [1]  */
        unsigned int    ints_tlbinvalid_msk   : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_INTMASK_S;

/* Define the union U_SMMU_INTRAW_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ints_tlbmiss_raw      : 1   ; /* [0]  */
        unsigned int    ints_ptw_trans_raw    : 1   ; /* [1]  */
        unsigned int    ints_tlbinvalid_raw   : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_INTRAW_S;

/* Define the union U_SMMU_INTSTAT_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ints_tlbmiss_stat     : 1   ; /* [0]  */
        unsigned int    ints_ptw_trans_stat   : 1   ; /* [1]  */
        unsigned int    ints_tlbinvalid_stat  : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_INTSTAT_S;

/* Define the union U_SMMU_INTCLR_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ints_tlbmiss_clr      : 1   ; /* [0]  */
        unsigned int    ints_ptw_trans_clr    : 1   ; /* [1]  */
        unsigned int    ints_tlbinvalid_clr   : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_INTCLR_S;

/* Define the union U_SMMU_INTMASK_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intns_tlbmiss_msk     : 1   ; /* [0]  */
        unsigned int    intns_ptw_trans_msk   : 1   ; /* [1]  */
        unsigned int    intns_tlbinvalid_msk  : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_INTMASK_NS;

/* Define the union U_SMMU_INTRAW_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intns_tlbmiss_raw     : 1   ; /* [0]  */
        unsigned int    intns_ptw_trans_raw   : 1   ; /* [1]  */
        unsigned int    intns_tlbinvalid_raw  : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_INTRAW_NS;

/* Define the union U_SMMU_INTSTAT_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intns_tlbmiss_stat    : 1   ; /* [0]  */
        unsigned int    intns_ptw_trans_stat  : 1   ; /* [1]  */
        unsigned int    intns_tlbinvalid_stat : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_INTSTAT_NS;

/* Define the union U_SMMU_INTCLR_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intns_tlbmiss_clr     : 1   ; /* [0]  */
        unsigned int    intns_ptw_trans_clr   : 1   ; /* [1]  */
        unsigned int    intns_tlbinvalid_clr  : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_INTCLR_NS;

/* Define the union U_SMMU_FAULT_ID_PTW_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_index_id_s      : 16  ; /* [15..0]  */
        unsigned int    fault_strm_id_s       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_FAULT_ID_PTW_S;

/* Define the union U_SMMU_FAULT_ID_PTW_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_index_id_ns     : 16  ; /* [15..0]  */
        unsigned int    fault_strm_id_ns      : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_FAULT_ID_PTW_NS;

/* Define the union U_SMMU_FAULT_PTW_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_s_ptw_num       : 4   ; /* [3..0]  */
        unsigned int    fault_ns_ptw_num      : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_FAULT_PTW_NUM;

/* Define the union U_SMMU_FAULT_ID_WR_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_index_id_wr_s   : 16  ; /* [15..0]  */
        unsigned int    fault_str_id_wr_s     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_FAULT_ID_WR_S;

/* Define the union U_SMMU_FAULT_ID_WR_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_index_id_wr_ns  : 16  ; /* [15..0]  */
        unsigned int    fault_str_id_wr_ns    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_FAULT_ID_WR_NS;

/* Define the union U_SMMU_FAULT_ID_RD_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_index_id_rd_s   : 16  ; /* [15..0]  */
        unsigned int    fault_str_id_rd_s     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_FAULT_ID_RD_S;

/* Define the union U_SMMU_FAULT_ID_RD_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_index_id_rd_ns  : 16  ; /* [15..0]  */
        unsigned int    fault_str_id_rd_ns    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_FAULT_ID_RD_NS;

/* Define the union U_SMMU_FAULT_TBU_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_tlbmiss_err_s   : 2   ; /* [1..0]  */
        unsigned int    fault_tlbinvalid_err_s : 2   ; /* [3..2]  */
        unsigned int    fault_tlbmiss_err_ns  : 2   ; /* [5..4]  */
        unsigned int    fault_tlbinvalid_err_ns : 2   ; /* [7..6]  */
        unsigned int    fault_tbu_num         : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_FAULT_TBU_INFO;

/* Define the union U_SMMU_FAULT_TBU_DBG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aw_valids             : 1   ; /* [0]  */
        unsigned int    aw_readys             : 1   ; /* [1]  */
        unsigned int    tbu_awreadym          : 1   ; /* [2]  */
        unsigned int    awch_stall_n          : 1   ; /* [3]  */
        unsigned int    write_command_counter : 2   ; /* [5..4]  */
        unsigned int    ar_valids             : 1   ; /* [6]  */
        unsigned int    ar_readys             : 1   ; /* [7]  */
        unsigned int    tbu_arreadym          : 1   ; /* [8]  */
        unsigned int    arch_stall_n          : 1   ; /* [9]  */
        unsigned int    read_command_counter  : 2   ; /* [11..10]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_FAULT_TBU_DBG;

/* Define the union U_SMMU_PREF_BUFFER_EMPTY */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pref_buffer_empty     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_PREF_BUFFER_EMPTY;

/* Define the union U_SMMU_PTWQ_IDLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ptwq0_idle_state      : 1   ; /* [0]  */
        unsigned int    ptwq1_idle_state      : 1   ; /* [1]  */
        unsigned int    ptwq2_idle_state      : 1   ; /* [2]  */
        unsigned int    ptwq3_idle_state      : 1   ; /* [3]  */
        unsigned int    ptwq4_idle_state      : 1   ; /* [4]  */
        unsigned int    ptwq5_idle_state      : 1   ; /* [5]  */
        unsigned int    ptwq6_idle_state      : 1   ; /* [6]  */
        unsigned int    ptwq7_idle_state      : 1   ; /* [7]  */
        unsigned int    ptwq8_idle_state      : 1   ; /* [8]  */
        unsigned int    ptwq9_idle_state      : 1   ; /* [9]  */
        unsigned int    ptwq10_idle_state     : 1   ; /* [10]  */
        unsigned int    ptwq11_idle_state     : 1   ; /* [11]  */
        unsigned int    ptwq12_idle_state     : 1   ; /* [12]  */
        unsigned int    ptwq13_idle_state     : 1   ; /* [13]  */
        unsigned int    ptwq14_idle_state     : 1   ; /* [14]  */
        unsigned int    ptwq15_idle_state     : 1   ; /* [15]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_PTWQ_IDLE;

/* Define the union U_SMMU_RESET_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    smmu_rst_state        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_RESET_STATE;

/* Define the union U_SMMU_MASTER_DBG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    last_miss_cnt_rd      : 10  ; /* [9..0]  */
        unsigned int    cur_miss_cnt_rd       : 10  ; /* [19..10]  */
        unsigned int    vld_debug_rd          : 4   ; /* [23..20]  */
        unsigned int    rdy_debug_rd          : 4   ; /* [27..24]  */
        unsigned int    in_out_cmd_cnt_rd     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_MASTER_DBG0;

/* Define the union U_SMMU_MASTER_DBG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    last_miss_cnt_wr      : 10  ; /* [9..0]  */
        unsigned int    cur_miss_cnt_wr       : 10  ; /* [19..10]  */
        unsigned int    vld_debug_wr          : 4   ; /* [23..20]  */
        unsigned int    rdy_debug_wr          : 4   ; /* [27..24]  */
        unsigned int    in_out_cmd_cnt_wr     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_MASTER_DBG1;

/* Define the union U_SMMU_MASTER_DBG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    last_double_miss_cnt_rd : 8   ; /* [7..0]  */
        unsigned int    cur_double_miss_cnt_rd : 8   ; /* [15..8]  */
        unsigned int    last_double_upd_cnt_rd : 8   ; /* [23..16]  */
        unsigned int    cur_double_upd_cnt_rd : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_MASTER_DBG2;

/* Define the union U_SMMU_MASTER_DBG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    last_double_miss_cnt_wr : 8   ; /* [7..0]  */
        unsigned int    cur_double_miss_cnt_wr : 8   ; /* [15..8]  */
        unsigned int    last_double_upd_cnt_wr : 7   ; /* [22..16]  */
        unsigned int    cur_double_upd_cnt_wr : 7   ; /* [29..23]  */
        unsigned int    mst_fsm_cur           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_MASTER_DBG3;

/* Define the union U_SMMU_MASTER_DBG4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    last_sel1_chn_miss_cnt_rd : 16  ; /* [15..0]  */
        unsigned int    cur_sel1_chn_miss_cnt_rd : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_MASTER_DBG4;

/* Define the union U_SMMU_MASTER_DBG5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    last_sel2_chn_miss_cnt_rd : 16  ; /* [15..0]  */
        unsigned int    cur_sel2_chn_miss_cnt_rd : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_MASTER_DBG5;

/* Define the union U_SMMU_MASTER_DBG6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    last_sel1_chn_miss_cnt_wr : 16  ; /* [15..0]  */
        unsigned int    cur_sel1_chn_miss_cnt_wr : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_MASTER_DBG6;

/* Define the union U_SMMU_MASTER_DBG7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    last_sel2_chn_miss_cnt_wr : 16  ; /* [15..0]  */
        unsigned int    cur_sel2_chn_miss_cnt_wr : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_MASTER_DBG7;

/* Define the union U_SMMU_MASTER_DBG8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sel2_chn_wr           : 8   ; /* [7..0]  */
        unsigned int    sel1_chn_wr           : 8   ; /* [15..8]  */
        unsigned int    sel2_chn_rd           : 8   ; /* [23..16]  */
        unsigned int    sel1_chn_rd           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMMU_MASTER_DBG8;
#endif
//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_VPSS_CTRL            VPSS_CTRL                ; /* 0x0 */
    volatile U_VPSS_CTRL2           VPSS_CTRL2               ; /* 0x4 */
    volatile U_VPSS_CTRL3           VPSS_CTRL3               ; /* 0x8 */
    volatile unsigned int           reserved_0[2]                     ; /* 0xc~0x10 */
    volatile U_VPSS_REFSIZE         VPSS_REFSIZE             ; /* 0x14 */
    volatile U_VPSS_IMGSIZE         VPSS_IMGSIZE             ; /* 0x18 */
    volatile U_VPSS_INCROP_POS      VPSS_INCROP_POS          ; /* 0x1c */
    volatile U_VPSS_INCROP_SIZE     VPSS_INCROP_SIZE         ; /* 0x20 */
    volatile U_VPSS_VHD0CROP_POS    VPSS_VHD0CROP_POS        ; /* 0x24 */
    volatile U_VPSS_VHD0CROP_SIZE   VPSS_VHD0CROP_SIZE       ; /* 0x28 */
    volatile U_VPSS_VHD0_ZMEORESO   VPSS_VHD0_ZMEORESO       ; /* 0x2c */
    volatile U_VPSS_VHD0LBA_DSIZE   VPSS_VHD0LBA_DSIZE       ; /* 0x30 */
    volatile U_VPSS_VHD0LBA_VFPOS   VPSS_VHD0LBA_VFPOS       ; /* 0x34 */
    volatile U_VPSS_VHD0LBA_BK      VPSS_VHD0LBA_BK          ; /* 0x38 */
    volatile U_STR_DET_VIDCTRL      STR_DET_VIDCTRL          ; /* 0x3c */
    volatile U_STR_DET_VIDBLKPOS0_1   STR_DET_VIDBLKPOS0_1   ; /* 0x40 */
    volatile U_STR_DET_VIDBLKPOS2_3   STR_DET_VIDBLKPOS2_3   ; /* 0x44 */
    volatile U_STR_DET_VIDBLKPOS4_5   STR_DET_VIDBLKPOS4_5   ; /* 0x48 */
    volatile U_VPSS_VC1_CTRL0       VPSS_VC1_CTRL0           ; /* 0x4c */
    volatile U_VPSS_TESTPATTERN     VPSS_TESTPATTERN         ; /* 0x50 */
    volatile unsigned int           reserved_1[5]                     ; /* 0x54~0x64 */
    volatile unsigned int           VPSS_NX2Y_TOP_HEAD_ADDR  ; /* 0x68 */
    volatile unsigned int           VPSS_NX2C_TOP_HEAD_ADDR  ; /* 0x6c */
    volatile unsigned int           VPSS_NX2Y_BOT_HEAD_ADDR  ; /* 0x70 */
    volatile unsigned int           VPSS_NX2C_BOT_HEAD_ADDR  ; /* 0x74 */
    volatile U_VPSS_NX2_HEAD_STRIDE   VPSS_NX2_HEAD_STRIDE   ; /* 0x78 */
    volatile unsigned int           VPSS_VDH_DB_ADDR         ; /* 0x7c */
    volatile U_VPSS_VDH_DB_CTRL     VPSS_VDH_DB_CTRL         ; /* 0x80 */
    volatile U_VPSS_VDH_DB_IMG_SIZE   VPSS_VDH_DB_IMG_SIZE   ; /* 0x84 */
    volatile unsigned int           VPSS_TRANS_SRC_ADDR      ; /* 0x88 */
    volatile unsigned int           VPSS_TRANS_DES_ADDR      ; /* 0x8c */
    volatile U_VPSS_TRANS_INFO      VPSS_TRANS_INFO          ; /* 0x90 */
    volatile unsigned int           VPSS_NX2Y_LB_ADDR        ; /* 0x94 */
    volatile unsigned int           VPSS_NX2C_LB_ADDR        ; /* 0x98 */
    volatile U_VPSS_NX2_LB_STRIDE   VPSS_NX2_LB_STRIDE       ; /* 0x9c */
    volatile unsigned int           reserved_2[22]                     ; /* 0xa0~0xf4 */
    volatile unsigned int           VPSS_NODEID              ; /* 0xf8 */
    volatile U_VPSS_INTMASK         VPSS_INTMASK             ; /* 0xfc */
    volatile U_VPSS_CUR_CTRL        VPSS_CUR_CTRL            ; /* 0x100 */
    volatile unsigned int           VPSS_CURYADDR            ; /* 0x104 */
    volatile unsigned int           VPSS_CURCADDR            ; /* 0x108 */
    volatile U_VPSS_CURSTRIDE       VPSS_CURSTRIDE           ; /* 0x10c */
    volatile U_VPSS_REF_CTRL        VPSS_REF_CTRL            ; /* 0x110 */
    volatile unsigned int           VPSS_REFYADDR            ; /* 0x114 */
    volatile unsigned int           VPSS_REFCADDR            ; /* 0x118 */
    volatile U_VPSS_REFSTRIDE       VPSS_REFSTRIDE           ; /* 0x11c */
    volatile U_VPSS_NXT1_CTRL       VPSS_NXT1_CTRL           ; /* 0x120 */
    volatile unsigned int           VPSS_NXT1YADDR           ; /* 0x124 */
    volatile unsigned int           VPSS_NXT1CADDR           ; /* 0x128 */
    volatile U_VPSS_NXT1STRIDE      VPSS_NXT1STRIDE          ; /* 0x12c */
    volatile U_VPSS_NXT2_CTRL       VPSS_NXT2_CTRL           ; /* 0x130 */
    volatile unsigned int           VPSS_NXT2YADDR           ; /* 0x134 */
    volatile unsigned int           VPSS_NXT2CADDR           ; /* 0x138 */
    volatile U_VPSS_NXT2STRIDE      VPSS_NXT2STRIDE          ; /* 0x13c */
    volatile U_VPSS_RFRCTRL         VPSS_RFRCTRL             ; /* 0x140 */
    volatile unsigned int           VPSS_RFRYADDR            ; /* 0x144 */
    volatile unsigned int           VPSS_RFRCADDR            ; /* 0x148 */
    volatile U_VPSS_RFRSTRIDE       VPSS_RFRSTRIDE           ; /* 0x14c */
    volatile U_VPSS_VHD0CTRL        VPSS_VHD0CTRL            ; /* 0x150 */
    volatile U_VPSS_VHD0SIZE        VPSS_VHD0SIZE            ; /* 0x154 */
    volatile unsigned int           VPSS_VHD0YADDR           ; /* 0x158 */
    volatile unsigned int           VPSS_VHD0CADDR           ; /* 0x15c */
    volatile U_VPSS_VHD0STRIDE      VPSS_VHD0STRIDE          ; /* 0x160 */
    volatile unsigned int           VPSS_STRADDR             ; /* 0x164 */
    volatile unsigned int           VPSS_STWADDR             ; /* 0x168 */
    volatile U_VPSS_STSTRIDE        VPSS_STSTRIDE            ; /* 0x16c */
    volatile unsigned int           VPSS_STT_W_ADDR          ; /* 0x170 */
    volatile U_VPSS_TUNL_CTRL0      VPSS_TUNL_CTRL0          ; /* 0x174 */
    volatile U_VPSS_TUNL_CTRL1      VPSS_TUNL_CTRL1          ; /* 0x178 */
    volatile unsigned int           VPSS_VHD0_TUNL_ADDR      ; /* 0x17c */
    volatile unsigned int           VPSS_CUR_TUNL_ADDR       ; /* 0x180 */
    volatile unsigned int           VPSS_REE_CTRL            ; /* 0x184 */
    volatile unsigned int           VPSS_REEYADDR            ; /* 0x188 */
    volatile unsigned int           VPSS_REECADDR            ; /* 0x18c */
    volatile U_VPSS_REESTRIDE       VPSS_REESTRIDE           ; /* 0x190 */
    volatile unsigned int           VPSS_PRJVCURADDR         ; /* 0x194 */
    volatile unsigned int           VPSS_PRJHCURADDR         ; /* 0x198 */
    volatile U_VPSS_PRJCURSTRIDE    VPSS_PRJCURSTRIDE        ; /* 0x19c */
    volatile unsigned int           VPSS_RGMVCURADDR         ; /* 0x1a0 */
    volatile unsigned int           VPSS_RGMVNX1ADDR         ; /* 0x1a4 */
    volatile U_VPSS_RGMVSTRIDE      VPSS_RGMVSTRIDE          ; /* 0x1a8 */
    volatile unsigned int           VPSS_BLKMVCURADDR        ; /* 0x1ac */
    volatile unsigned int           VPSS_BLKMVREFADDR        ; /* 0x1b0 */
    volatile U_VPSS_BLKMVSTRIDE     VPSS_BLKMVSTRIDE         ; /* 0x1b4 */
    volatile unsigned int           VPSS_CUE_CTRL            ; /* 0x1b8 */
    volatile unsigned int           VPSS_CUEYADDR            ; /* 0x1bc */
    volatile unsigned int           VPSS_CUECADDR            ; /* 0x1c0 */
    volatile U_VPSS_CUESTRIDE       VPSS_CUESTRIDE           ; /* 0x1c4 */
    volatile unsigned int           VPSS_PRJVNX2ADDR         ; /* 0x1c8 */
    volatile unsigned int           VPSS_PRJHNX2ADDR         ; /* 0x1cc */
    volatile U_VPSS_PRJNX2STRIDE    VPSS_PRJNX2STRIDE        ; /* 0x1d0 */
    volatile unsigned int           VPSS_RGMVNX2ADDR         ; /* 0x1d4 */
    volatile U_VPSS_RGMVNX2STRIDE   VPSS_RGMVNX2STRIDE       ; /* 0x1d8 */
    volatile unsigned int           VPSS_BLKMVNX1ADDR        ; /* 0x1dc */
    volatile U_VPSS_BLKMVNX1STRIDE   VPSS_BLKMVNX1STRIDE     ; /* 0x1e0 */
    volatile unsigned int           VPSS_MAD_RADDR           ; /* 0x1e4 */
    volatile unsigned int           VPSS_MAD_WADDR           ; /* 0x1e8 */
    volatile unsigned int           VPSS_SNR_MAD_RADDR       ; /* 0x1ec */
    volatile U_VPSS_MADSTRIDE       VPSS_MADSTRIDE           ; /* 0x1f0 */
    volatile unsigned int           VPSS_BLKMVNX2ADDR        ; /* 0x1f4 */
    volatile U_VPSS_BLKMVNX2STRIDE   VPSS_BLKMVNX2STRIDE     ; /* 0x1f8 */
    volatile unsigned int           VPSS_NXT2CRADDR          ; /* 0x1fc */
    volatile unsigned int           VPSS_VHD0_ZME_LHADDR     ; /* 0x200 */
    volatile unsigned int           VPSS_VHD0_ZME_LVADDR     ; /* 0x204 */
    volatile unsigned int           VPSS_VHD0_ZME_CHADDR     ; /* 0x208 */
    volatile unsigned int           VPSS_VHD0_ZME_CVADDR     ; /* 0x20c */
    volatile unsigned int           VPSS_TNR_ADDR            ; /* 0x210 */
    volatile unsigned int           VPSS_TNR_CLUT_ADDR       ; /* 0x214 */
    volatile unsigned int           VPSS_SNR_ADDR            ; /* 0x218 */
    volatile unsigned int           VPSS_DBM_ADDR            ; /* 0x21c */
    volatile unsigned int           reserved_3[8]                     ; /* 0x220~0x23c */
    volatile unsigned int           VPSS_ZME_ADDR            ; /* 0x240 */
    volatile unsigned int           reserved_4[5]                     ; /* 0x244~0x254 */
    volatile unsigned int           VPSS_DEI_ADDR            ; /* 0x258 */
    volatile unsigned int           reserved_5[7]                     ; /* 0x25c~0x274 */
    volatile unsigned int           VPSS_CHK_SUM_W_ADDR      ; /* 0x278 */
    volatile U_VPSS_CHK_SUM_SEL     VPSS_CHK_SUM_SEL         ; /* 0x27c */
    volatile U_VPSS_RCH_BYPASS      VPSS_RCH_BYPASS          ; /* 0x280 */
    volatile U_VPSS_WCH_BYPASS      VPSS_WCH_BYPASS          ; /* 0x284 */
    volatile unsigned int           reserved_6[29]                     ; /* 0x288~0x2f8 */
    volatile unsigned int           VPSS_PNEXT               ; /* 0x2fc */
    volatile U_VPSS_START           VPSS_START               ; /* 0x300 */
    volatile U_VPSS_INTSTATE        VPSS_INTSTATE            ; /* 0x304 */
    volatile U_VPSS_INTCLR          VPSS_INTCLR              ; /* 0x308 */
    volatile U_VPSS_RAWINT          VPSS_RAWINT              ; /* 0x30c */
    volatile unsigned int           VPSS_PFCNT               ; /* 0x310 */
    volatile U_VPSS_MISCELLANEOUS   VPSS_MISCELLANEOUS       ; /* 0x314 */
    volatile unsigned int           VPSS_MACCFG              ; /* 0x318 */
    volatile unsigned int           VPSS_TIMEOUT             ; /* 0x31c */
    volatile U_VPSS_FTCONFIG        VPSS_FTCONFIG            ; /* 0x320 */
    volatile unsigned int           reserved_7[3]                     ; /* 0x324~0x32c */
    volatile unsigned int           VPSS_EOFCNT              ; /* 0x330 */
    volatile unsigned int           reserved_8[3]                     ; /* 0x334~0x33c */
    volatile unsigned int           VPSS_VERSION             ; /* 0x340 */
    volatile U_VPSS_BUS_CTRL        VPSS_BUS_CTRL            ; /* 0x344 */
    volatile unsigned int           reserved_9[814]                     ; /* 0x348~0xffc */
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
    volatile U_VPSS_DIEINTPSCL0     VPSS_DIEINTPSCL0         ; /* 0x102c */
    volatile U_VPSS_DIEINTPSCL1     VPSS_DIEINTPSCL1         ; /* 0x1030 */
    volatile U_VPSS_DIEDIRTHD       VPSS_DIEDIRTHD           ; /* 0x1034 */
    volatile U_VPSS_DIEJITMTN       VPSS_DIEJITMTN           ; /* 0x1038 */
    volatile U_VPSS_DIEFLDMTN       VPSS_DIEFLDMTN           ; /* 0x103c */
    volatile U_VPSS_DIEMTNDIFFTHD0   VPSS_DIEMTNDIFFTHD0     ; /* 0x1040 */
    volatile U_VPSS_DIEMTNDIFFTHD1   VPSS_DIEMTNDIFFTHD1     ; /* 0x1044 */
    volatile U_VPSS_DIEMTNIIRSLP    VPSS_DIEMTNIIRSLP        ; /* 0x1048 */
    volatile U_VPSS_DIEMTNIIRRAT    VPSS_DIEMTNIIRRAT        ; /* 0x104c */
    volatile U_VPSS_DIEHISMODE      VPSS_DIEHISMODE          ; /* 0x1050 */
    volatile U_VPSS_DIEMORFLT       VPSS_DIEMORFLT           ; /* 0x1054 */
    volatile U_VPSS_DIECOMBCHK0     VPSS_DIECOMBCHK0         ; /* 0x1058 */
    volatile U_VPSS_DIECOMBCHK1     VPSS_DIECOMBCHK1         ; /* 0x105c */
    volatile U_VPSS_DIEFRMMTNSMTHTHD0   VPSS_DIEFRMMTNSMTHTHD0 ; /* 0x1060 */
    volatile U_VPSS_DIEFRMMTNSMTHTHD1   VPSS_DIEFRMMTNSMTHTHD1 ; /* 0x1064 */
    volatile U_VPSS_DIEFRMMTNSMTHSLP   VPSS_DIEFRMMTNSMTHSLP ; /* 0x1068 */
    volatile U_VPSS_DIEFRMMTNSMTHRAT   VPSS_DIEFRMMTNSMTHRAT ; /* 0x106c */
    volatile U_VPSS_DIEFRMFLDBLENDTHD0   VPSS_DIEFRMFLDBLENDTHD0 ; /* 0x1070 */
    volatile U_VPSS_DIEFRMFLDBLENDTHD1   VPSS_DIEFRMFLDBLENDTHD1 ; /* 0x1074 */
    volatile U_VPSS_DIEFRMFLDBLENDSLP   VPSS_DIEFRMFLDBLENDSLP ; /* 0x1078 */
    volatile U_VPSS_DIEFRMFLDBLENDRAT   VPSS_DIEFRMFLDBLENDRAT ; /* 0x107c */
    volatile U_VPSS_DIEMTNADJ       VPSS_DIEMTNADJ           ; /* 0x1080 */
    volatile U_VPSS_DIEEDGENORM0    VPSS_DIEEDGENORM0        ; /* 0x1084 */
    volatile U_VPSS_DIEEDGENORM1    VPSS_DIEEDGENORM1        ; /* 0x1088 */
    volatile U_VPSS_DIEEDGENORM2    VPSS_DIEEDGENORM2        ; /* 0x108c */
    volatile U_VPSS_DIEEDGENORM3    VPSS_DIEEDGENORM3        ; /* 0x1090 */
    volatile U_VPSS_DIEEDGENORM4    VPSS_DIEEDGENORM4        ; /* 0x1094 */
    volatile U_VPSS_DIEEDGENORM5    VPSS_DIEEDGENORM5        ; /* 0x1098 */
    volatile U_VPSS_DIEEDGEFORMC    VPSS_DIEEDGEFORMC        ; /* 0x109c */
    volatile U_VPSS_DIEMCSTRENGTH0   VPSS_DIEMCSTRENGTH0     ; /* 0x10a0 */
    volatile U_VPSS_DIEMCSTRENGTH1   VPSS_DIEMCSTRENGTH1     ; /* 0x10a4 */
    volatile U_VPSS_DIEFUSION0      VPSS_DIEFUSION0          ; /* 0x10a8 */
    volatile U_VPSS_DIEFUSION1      VPSS_DIEFUSION1          ; /* 0x10ac */
    volatile U_VPSS_DIEFUSION2      VPSS_DIEFUSION2          ; /* 0x10b0 */
    volatile U_VPSS_DIEFUSION3      VPSS_DIEFUSION3          ; /* 0x10b4 */
    volatile U_VPSS_DIEMAGBMTHD0    VPSS_DIEMAGBMTHD0        ; /* 0x10b8 */
    volatile U_VPSS_DIEMAGBMTHD1    VPSS_DIEMAGBMTHD1        ; /* 0x10bc */
    volatile U_VPSS_DIEMCGBMCOEF0   VPSS_DIEMCGBMCOEF0       ; /* 0x10c0 */
    volatile U_VPSS_DIEMCGBMCOEF1   VPSS_DIEMCGBMCOEF1       ; /* 0x10c4 */
    volatile U_VPSS_PDPHISTTHD1     VPSS_PDPHISTTHD1         ; /* 0x10c8 */
    volatile U_VPSS_PDPCCMOV        VPSS_PDPCCMOV            ; /* 0x10cc */
    volatile U_VPSS_PDCTRL          VPSS_PDCTRL              ; /* 0x10d0 */
    volatile U_VPSS_PDBLKTHD        VPSS_PDBLKTHD            ; /* 0x10d4 */
    volatile U_VPSS_PDUMTHD         VPSS_PDUMTHD             ; /* 0x10d8 */
    volatile U_VPSS_PDPCCCORING     VPSS_PDPCCCORING         ; /* 0x10dc */
    volatile U_VPSS_PDPCCHTHD       VPSS_PDPCCHTHD           ; /* 0x10e0 */
    volatile U_VPSS_PDPCCVTHD       VPSS_PDPCCVTHD           ; /* 0x10e4 */
    volatile U_VPSS_PDITDIFFVTHD    VPSS_PDITDIFFVTHD        ; /* 0x10e8 */
    volatile U_VPSS_PDLASITHD       VPSS_PDLASITHD           ; /* 0x10ec */
    volatile U_VPSS_PDLASIMODE0COEF   VPSS_PDLASIMODE0COEF   ; /* 0x10f0 */
    volatile U_VPSS_PDLASIMODE1COEF   VPSS_PDLASIMODE1COEF   ; /* 0x10f4 */
    volatile U_VPSS_PDREGION        VPSS_PDREGION            ; /* 0x10f8 */
    volatile U_VPSS_MCDI_RGDIFY     VPSS_MCDI_RGDIFY         ; /* 0x10fc */
    volatile U_VPSS_MCDI_RGSAD      VPSS_MCDI_RGSAD          ; /* 0x1100 */
    volatile U_VPSS_MCDI_RGMV       VPSS_MCDI_RGMV           ; /* 0x1104 */
    volatile U_VPSS_MCDI_RGMAG0     VPSS_MCDI_RGMAG0         ; /* 0x1108 */
    volatile U_VPSS_MCDI_RGMAG1     VPSS_MCDI_RGMAG1         ; /* 0x110c */
    volatile U_VPSS_MCDI_RGLS       VPSS_MCDI_RGLS           ; /* 0x1110 */
    volatile U_VPSS_MCDI_SEL        VPSS_MCDI_SEL            ; /* 0x1114 */
    volatile U_VPSS_MCDI_DLT0       VPSS_MCDI_DLT0           ; /* 0x1118 */
    volatile U_VPSS_MCDI_DLT1       VPSS_MCDI_DLT1           ; /* 0x111c */
    volatile U_VPSS_MCDI_RGMCW0     VPSS_MCDI_RGMCW0         ; /* 0x1120 */
    volatile U_VPSS_MCDI_RGMCW1     VPSS_MCDI_RGMCW1         ; /* 0x1124 */
    volatile U_VPSS_MCDI_RGMCW2     VPSS_MCDI_RGMCW2         ; /* 0x1128 */
    volatile U_VPSS_MCDI_RGMCW3     VPSS_MCDI_RGMCW3         ; /* 0x112c */
    volatile U_VPSS_MCDI_RGMCW4     VPSS_MCDI_RGMCW4         ; /* 0x1130 */
    volatile U_VPSS_MCDI_RGMCW5     VPSS_MCDI_RGMCW5         ; /* 0x1134 */
    volatile U_VPSS_MCDI_RGMCW6     VPSS_MCDI_RGMCW6         ; /* 0x1138 */
    volatile U_VPSS_MCDI_TPMCW0     VPSS_MCDI_TPMCW0         ; /* 0x113c */
    volatile U_VPSS_MCDI_TPMCW1     VPSS_MCDI_TPMCW1         ; /* 0x1140 */
    volatile U_VPSS_MCDI_WNDMCW0    VPSS_MCDI_WNDMCW0        ; /* 0x1144 */
    volatile U_VPSS_MCDI_WNDMCW1    VPSS_MCDI_WNDMCW1        ; /* 0x1148 */
    volatile U_VPSS_MCDI_WNDMCW2    VPSS_MCDI_WNDMCW2        ; /* 0x114c */
    volatile U_VPSS_MCDI_WNDMCW3    VPSS_MCDI_WNDMCW3        ; /* 0x1150 */
    volatile U_VPSS_MCDI_VERTWEIGHT0   VPSS_MCDI_VERTWEIGHT0 ; /* 0x1154 */
    volatile U_VPSS_MCDI_VERTWEIGHT1   VPSS_MCDI_VERTWEIGHT1 ; /* 0x1158 */
    volatile U_VPSS_MCDI_VERTWEIGHT2   VPSS_MCDI_VERTWEIGHT2 ; /* 0x115c */
    volatile U_VPSS_MCDI_VERTWEIGHT3   VPSS_MCDI_VERTWEIGHT3 ; /* 0x1160 */
    volatile U_VPSS_MCDI_VERTWEIGHT4   VPSS_MCDI_VERTWEIGHT4 ; /* 0x1164 */
    volatile U_VPSS_MCDI_VERTWEIGHT5   VPSS_MCDI_VERTWEIGHT5 ; /* 0x1168 */
    volatile U_VPSS_MCDI_MC0        VPSS_MCDI_MC0            ; /* 0x116c */
    volatile U_VPSS_MCDI_MC1        VPSS_MCDI_MC1            ; /* 0x1170 */
    volatile U_VPSS_MCDI_MC2        VPSS_MCDI_MC2            ; /* 0x1174 */
    volatile U_VPSS_MCDI_MC3        VPSS_MCDI_MC3            ; /* 0x1178 */
    volatile U_VPSS_MCDI_MC4        VPSS_MCDI_MC4            ; /* 0x117c */
    volatile U_VPSS_MCDI_MC5        VPSS_MCDI_MC5            ; /* 0x1180 */
    volatile U_VPSS_MCDI_MC6        VPSS_MCDI_MC6            ; /* 0x1184 */
    volatile U_VPSS_MCDI_MC7        VPSS_MCDI_MC7            ; /* 0x1188 */
    volatile U_VPSS_MCDI_LCDINEW0   VPSS_MCDI_LCDINEW0       ; /* 0x118c */
    volatile U_VPSS_MCDI_LCDINEW1   VPSS_MCDI_LCDINEW1       ; /* 0x1190 */
    volatile U_VPSS_MCDI_NUMT       VPSS_MCDI_NUMT           ; /* 0x1194 */
    volatile U_VPSS_MCDI_DEMO       VPSS_MCDI_DEMO           ; /* 0x1198 */
    volatile unsigned int           reserved_10[921]                     ; /* 0x119c~0x1ffc */
    volatile U_VPSS_VHD0_HSP        VPSS_VHD0_HSP            ; /* 0x2000 */
    volatile U_VPSS_VHD0_HLOFFSET   VPSS_VHD0_HLOFFSET       ; /* 0x2004 */
    volatile U_VPSS_VHD0_HCOFFSET   VPSS_VHD0_HCOFFSET       ; /* 0x2008 */
    volatile U_VPSS_VHD0_VSP        VPSS_VHD0_VSP            ; /* 0x200c */
    volatile U_VPSS_VHD0_VSR        VPSS_VHD0_VSR            ; /* 0x2010 */
    volatile U_VPSS_VHD0_VOFFSET    VPSS_VHD0_VOFFSET        ; /* 0x2014 */
    volatile unsigned int           reserved_11[314]                     ; /* 0x2018~0x24fc */
    volatile U_VPSS_DB_CTRL         VPSS_DB_CTRL             ; /* 0x2500 */
    volatile unsigned int           reserved_12[34]                     ; /* 0x2504~0x2588 */
    volatile U_VPSS_DB_LUT34        VPSS_DB_LUT34            ; /* 0x258c */
    volatile U_VPSS_DB_LUT35        VPSS_DB_LUT35            ; /* 0x2590 */
    volatile U_VPSS_DB_LUT36        VPSS_DB_LUT36            ; /* 0x2594 */
    volatile U_VPSS_DB_LUT37        VPSS_DB_LUT37            ; /* 0x2598 */
    volatile U_VPSS_DB_LUT38        VPSS_DB_LUT38            ; /* 0x259c */
    volatile U_VPSS_DB_LUT39        VPSS_DB_LUT39            ; /* 0x25a0 */
    volatile U_VPSS_DB_LUT40        VPSS_DB_LUT40            ; /* 0x25a4 */
    volatile unsigned int           reserved_13[3]                     ; /* 0x25a8~0x25b0 */
    volatile U_VPSS_DB_LUT44        VPSS_DB_LUT44            ; /* 0x25b4 */
    volatile U_VPSS_DB_LUT45        VPSS_DB_LUT45            ; /* 0x25b8 */
    volatile U_VPSS_DB_LUT46        VPSS_DB_LUT46            ; /* 0x25bc */
    volatile U_VPSS_DB_LUT47        VPSS_DB_LUT47            ; /* 0x25c0 */
    volatile U_VPSS_DB_LUT48        VPSS_DB_LUT48            ; /* 0x25c4 */
    volatile U_VPSS_DB_LUT49        VPSS_DB_LUT49            ; /* 0x25c8 */
    volatile U_VPSS_DB_LUT50        VPSS_DB_LUT50            ; /* 0x25cc */
    volatile U_VPSS_DB_LUT51        VPSS_DB_LUT51            ; /* 0x25d0 */
    volatile U_VPSS_DB_LUT52        VPSS_DB_LUT52            ; /* 0x25d4 */
    volatile U_VPSS_DB_LUT53        VPSS_DB_LUT53            ; /* 0x25d8 */
    volatile U_VPSS_DB_LUT54        VPSS_DB_LUT54            ; /* 0x25dc */
    volatile U_VPSS_DB_LUT55        VPSS_DB_LUT55            ; /* 0x25e0 */
    volatile U_VPSS_DB_LUT56        VPSS_DB_LUT56            ; /* 0x25e4 */
    volatile U_VPSS_DB_LUT57        VPSS_DB_LUT57            ; /* 0x25e8 */
    volatile unsigned int           reserved_14                     ; /* 0x25ec */
    volatile U_VPSS_DB_LUT59        VPSS_DB_LUT59            ; /* 0x25f0 */
    volatile U_VPSS_DB_LUT60        VPSS_DB_LUT60            ; /* 0x25f4 */
    volatile U_VPSS_DB_CTRL1        VPSS_DB_CTRL1            ; /* 0x25f8 */
    volatile U_VPSS_DB_THR0         VPSS_DB_THR0             ; /* 0x25fc */
    volatile U_VPSS_DB_THR1         VPSS_DB_THR1             ; /* 0x2600 */
    volatile U_VPSS_DB_OST0         VPSS_DB_OST0             ; /* 0x2604 */
    volatile U_VPSS_DB_OST1         VPSS_DB_OST1             ; /* 0x2608 */
    volatile U_VPSS_DB_DEMO         VPSS_DB_DEMO             ; /* 0x260c */
    volatile U_VPSS_DB_LUT61        VPSS_DB_LUT61            ; /* 0x2610 */
    volatile U_VPSS_DB_LUT62        VPSS_DB_LUT62            ; /* 0x2614 */
    volatile U_VPSS_DB_LUT63        VPSS_DB_LUT63            ; /* 0x2618 */
    volatile U_VPSS_DB_LUT64        VPSS_DB_LUT64            ; /* 0x261c */
    volatile U_VPSS_DB_THR2         VPSS_DB_THR2             ; /* 0x2620 */
    volatile U_VPSS_DB_INFO         VPSS_DB_INFO             ; /* 0x2624 */
    volatile U_VPSS_DB_THR3         VPSS_DB_THR3             ; /* 0x2628 */
    volatile unsigned int           reserved_15[5]                     ; /* 0x262c~0x263c */
    volatile U_VPSS_DBD_THDEDGE     VPSS_DBD_THDEDGE         ; /* 0x2640 */
    volatile U_VPSS_DBD_THDTXT      VPSS_DBD_THDTXT          ; /* 0x2644 */
    volatile U_VPSS_DBD_LUTWGT0     VPSS_DBD_LUTWGT0         ; /* 0x2648 */
    volatile U_VPSS_DBD_LUTWGT1     VPSS_DBD_LUTWGT1         ; /* 0x264c */
    volatile U_VPSS_DBD_RTNBDTXT    VPSS_DBD_RTNBDTXT        ; /* 0x2650 */
    volatile U_VPSS_DBD_TSTBLKNUM   VPSS_DBD_TSTBLKNUM       ; /* 0x2654 */
    volatile U_VPSS_DBD_HYRDX8BINLUT0   VPSS_DBD_HYRDX8BINLUT0 ; /* 0x2658 */
    volatile U_VPSS_DBD_HYRDX8BINLUT1   VPSS_DBD_HYRDX8BINLUT1 ; /* 0x265c */
    volatile U_VPSS_DBD_HYRDX8BINLUT2   VPSS_DBD_HYRDX8BINLUT2 ; /* 0x2660 */
    volatile U_VPSS_DBD_HYRDX8BINLUT3   VPSS_DBD_HYRDX8BINLUT3 ; /* 0x2664 */
    volatile U_VPSS_DBD_HCRDX8BINLUT0   VPSS_DBD_HCRDX8BINLUT0 ; /* 0x2668 */
    volatile U_VPSS_DBD_HCRDX8BINLUT1   VPSS_DBD_HCRDX8BINLUT1 ; /* 0x266c */
    volatile U_VPSS_DBD_HCRDX8BINLUT2   VPSS_DBD_HCRDX8BINLUT2 ; /* 0x2670 */
    volatile U_VPSS_DBD_HCRDX8BINLUT3   VPSS_DBD_HCRDX8BINLUT3 ; /* 0x2674 */
    volatile U_VPSS_DBD_VYRDX8BINLUT0   VPSS_DBD_VYRDX8BINLUT0 ; /* 0x2678 */
    volatile U_VPSS_DBD_VYRDX8BINLUT1   VPSS_DBD_VYRDX8BINLUT1 ; /* 0x267c */
    volatile U_VPSS_DBD_VYRDX8BINLUT2   VPSS_DBD_VYRDX8BINLUT2 ; /* 0x2680 */
    volatile U_VPSS_DBD_VYRDX8BINLUT3   VPSS_DBD_VYRDX8BINLUT3 ; /* 0x2684 */
    volatile U_VPSS_DBD_BLKSIZE     VPSS_DBD_BLKSIZE         ; /* 0x2688 */
    volatile unsigned int           reserved_16[29]                     ; /* 0x268c~0x26fc */
    volatile U_VPSS_DM_DIR          VPSS_DM_DIR              ; /* 0x2700 */
    volatile U_VPSS_DM_EDGE         VPSS_DM_EDGE             ; /* 0x2704 */
    volatile U_VPSS_DM_LUT0         VPSS_DM_LUT0             ; /* 0x2708 */
    volatile U_VPSS_DM_LUT1         VPSS_DM_LUT1             ; /* 0x270c */
    volatile U_VPSS_DM_LUT2         VPSS_DM_LUT2             ; /* 0x2710 */
    volatile U_VPSS_DM_LUT3         VPSS_DM_LUT3             ; /* 0x2714 */
    volatile U_VPSS_DM_LUT4         VPSS_DM_LUT4             ; /* 0x2718 */
    volatile U_VPSS_DM_DIRC         VPSS_DM_DIRC             ; /* 0x271c */
    volatile U_VPSS_DM_DIRO         VPSS_DM_DIRO             ; /* 0x2720 */
    volatile U_VPSS_DM_LSW          VPSS_DM_LSW              ; /* 0x2724 */
    volatile U_VPSS_DM_MMFLR        VPSS_DM_MMFLR            ; /* 0x2728 */
    volatile U_VPSS_DM_MMFSR        VPSS_DM_MMFSR            ; /* 0x272c */
    volatile U_VPSS_DM_DIRLR        VPSS_DM_DIRLR            ; /* 0x2730 */
    volatile U_VPSS_DM_DIRSR        VPSS_DM_DIRSR            ; /* 0x2734 */
    volatile U_VPSS_DM_THR0         VPSS_DM_THR0             ; /* 0x2738 */
    volatile U_VPSS_DM_THR1         VPSS_DM_THR1             ; /* 0x273c */
    volatile U_VPSS_DM_THR2         VPSS_DM_THR2             ; /* 0x2740 */
    volatile unsigned int           reserved_17[7]                     ; /* 0x2744~0x275c */
    volatile U_VPSS_DS_CTRL         VPSS_DS_CTRL             ; /* 0x2760 */
    volatile U_VPSS_DS_THR1         VPSS_DS_THR1             ; /* 0x2764 */
    volatile U_VPSS_DS_THR2         VPSS_DS_THR2             ; /* 0x2768 */
    volatile U_VPSS_DS_THR3         VPSS_DS_THR3             ; /* 0x276c */
    volatile U_VPSS_DS_THR4         VPSS_DS_THR4             ; /* 0x2770 */
    volatile U_VPSS_DS_THR5         VPSS_DS_THR5             ; /* 0x2774 */
    volatile U_VPSS_DS_THR6         VPSS_DS_THR6             ; /* 0x2778 */
    volatile U_VPSS_DS_THR7         VPSS_DS_THR7             ; /* 0x277c */
    volatile U_VPSS_DS_THR8         VPSS_DS_THR8             ; /* 0x2780 */
    volatile U_VPSS_DS_THR9         VPSS_DS_THR9             ; /* 0x2784 */
    volatile U_VPSS_DS_THR10        VPSS_DS_THR10            ; /* 0x2788 */
    volatile U_VPSS_DS_THR11        VPSS_DS_THR11            ; /* 0x278c */
    volatile U_VPSS_DS_THR12        VPSS_DS_THR12            ; /* 0x2790 */
    volatile U_VPSS_DS_THR13        VPSS_DS_THR13            ; /* 0x2794 */
    volatile U_VPSS_DS_THR14        VPSS_DS_THR14            ; /* 0x2798 */
    volatile U_VPSS_DS_THR15        VPSS_DS_THR15            ; /* 0x279c */
    volatile U_VPSS_DS_THR16        VPSS_DS_THR16            ; /* 0x27a0 */
    volatile U_VPSS_DS_THR17        VPSS_DS_THR17            ; /* 0x27a4 */
    volatile U_VPSS_DS_THR18        VPSS_DS_THR18            ; /* 0x27a8 */
    volatile U_VPSS_DS_READSTATE0   VPSS_DS_READSTATE0       ; /* 0x27ac */
    volatile U_VPSS_DS_READSTATE1   VPSS_DS_READSTATE1       ; /* 0x27b0 */
    volatile U_VPSS_DS_THR19        VPSS_DS_THR19            ; /* 0x27b4 */
    volatile U_VPSS_DS_THR20        VPSS_DS_THR20            ; /* 0x27b8 */
    volatile unsigned int           reserved_18[17]                     ; /* 0x27bc~0x27fc */
    volatile U_VPSS_DR_CTRL         VPSS_DR_CTRL             ; /* 0x2800 */
    volatile U_VPSS_DR_THR1         VPSS_DR_THR1             ; /* 0x2804 */
    volatile U_VPSS_DR_THR2         VPSS_DR_THR2             ; /* 0x2808 */
    volatile U_VPSS_DR_THR3         VPSS_DR_THR3             ; /* 0x280c */
    volatile U_VPSS_DR_THR4         VPSS_DR_THR4             ; /* 0x2810 */
    volatile U_VPSS_DR_THR5         VPSS_DR_THR5             ; /* 0x2814 */
    volatile U_VPSS_DR_THR6         VPSS_DR_THR6             ; /* 0x2818 */
    volatile U_VPSS_DR_THR7         VPSS_DR_THR7             ; /* 0x281c */
    volatile U_VPSS_DR_THR8         VPSS_DR_THR8             ; /* 0x2820 */
    volatile U_VPSS_DR_THR9         VPSS_DR_THR9             ; /* 0x2824 */
    volatile U_VPSS_DR_THR10        VPSS_DR_THR10            ; /* 0x2828 */
    volatile U_VPSS_DR_THR11        VPSS_DR_THR11            ; /* 0x282c */
    volatile U_VPSS_DR_THR12        VPSS_DR_THR12            ; /* 0x2830 */
    volatile U_VPSS_DR_THR13        VPSS_DR_THR13            ; /* 0x2834 */
    volatile U_VPSS_DR_THR14        VPSS_DR_THR14            ; /* 0x2838 */
    volatile U_VPSS_DR_THR15        VPSS_DR_THR15            ; /* 0x283c */
    volatile U_VPSS_DR_THR16        VPSS_DR_THR16            ; /* 0x2840 */
    volatile U_VPSS_DR_THR17        VPSS_DR_THR17            ; /* 0x2844 */
    volatile U_VPSS_DR_THR18        VPSS_DR_THR18            ; /* 0x2848 */
    volatile U_VPSS_DR_THR19        VPSS_DR_THR19            ; /* 0x284c */
    volatile U_VPSS_DR_THR20        VPSS_DR_THR20            ; /* 0x2850 */
    volatile U_VPSS_DR_THR21        VPSS_DR_THR21            ; /* 0x2854 */
    volatile U_VPSS_DR_THR22        VPSS_DR_THR22            ; /* 0x2858 */
    volatile U_VPSS_DR_THR23        VPSS_DR_THR23            ; /* 0x285c */
    volatile U_VPSS_DR_THR24        VPSS_DR_THR24            ; /* 0x2860 */
    volatile U_VPSS_DR_THR25        VPSS_DR_THR25            ; /* 0x2864 */
    volatile U_VPSS_DR_THR26        VPSS_DR_THR26            ; /* 0x2868 */
    volatile unsigned int           reserved_19[37]                     ; /* 0x286c~0x28fc */
    volatile unsigned int           VPSS_DB_BORD_FLAG[128]   ; /* 0x2900~0x2afc */
    volatile unsigned int           reserved_20[320]                     ; /* 0x2b00~0x2ffc */
    volatile U_VPSS_SNR_ENABLE      VPSS_SNR_ENABLE          ; /* 0x3000 */
    volatile U_VPSS_SNR_EDGE_RATIOX0   VPSS_SNR_EDGE_RATIOX0 ; /* 0x3004 */
    volatile U_VPSS_SNR_EDGE_RATIOX1   VPSS_SNR_EDGE_RATIOX1 ; /* 0x3008 */
    volatile U_VPSS_SNR_EDGE_RATIOY0   VPSS_SNR_EDGE_RATIOY0 ; /* 0x300c */
    volatile U_VPSS_SNR_EDGE_RATIOY1   VPSS_SNR_EDGE_RATIOY1 ; /* 0x3010 */
    volatile U_VPSS_SNR_EDGE_RATIOK0   VPSS_SNR_EDGE_RATIOK0 ; /* 0x3014 */
    volatile U_VPSS_SNR_EDGE_RATIOK1   VPSS_SNR_EDGE_RATIOK1 ; /* 0x3018 */
    volatile U_VPSS_SNR_EDGE_RATIO   VPSS_SNR_EDGE_RATIO     ; /* 0x301c */
    volatile U_VPSS_SNR_EDGE_STRX0   VPSS_SNR_EDGE_STRX0     ; /* 0x3020 */
    volatile U_VPSS_SNR_EDGE_STRX1   VPSS_SNR_EDGE_STRX1     ; /* 0x3024 */
    volatile U_VPSS_SNR_EDGE_STRX2   VPSS_SNR_EDGE_STRX2     ; /* 0x3028 */
    volatile U_VPSS_SNR_EDGE_STRY0   VPSS_SNR_EDGE_STRY0     ; /* 0x302c */
    volatile U_VPSS_SNR_EDGE_STRY1   VPSS_SNR_EDGE_STRY1     ; /* 0x3030 */
    volatile U_VPSS_SNR_EDGE_STRK0   VPSS_SNR_EDGE_STRK0     ; /* 0x3034 */
    volatile U_VPSS_SNR_EDGE_STRK1   VPSS_SNR_EDGE_STRK1     ; /* 0x3038 */
    volatile U_VPSS_SNR_EDGE_STR    VPSS_SNR_EDGE_STR        ; /* 0x303c */
    volatile U_VPSS_SNR_SFYMAX_VALTH   VPSS_SNR_SFYMAX_VALTH ; /* 0x3040 */
    volatile U_VPSS_SNR_SFYMAX_IDXTH   VPSS_SNR_SFYMAX_IDXTH ; /* 0x3044 */
    volatile U_VPSS_SNR_SFYMEAN_VALTH   VPSS_SNR_SFYMEAN_VALTH ; /* 0x3048 */
    volatile U_VPSS_SNR_WFYMAX_VALTH   VPSS_SNR_WFYMAX_VALTH ; /* 0x304c */
    volatile U_VPSS_SNR_WFYMAX_IDXTH   VPSS_SNR_WFYMAX_IDXTH ; /* 0x3050 */
    volatile U_VPSS_SNR_WFYMEAN_VALTH   VPSS_SNR_WFYMEAN_VALTH ; /* 0x3054 */
    volatile U_VPSS_SNR_SFCMAX_VALTH   VPSS_SNR_SFCMAX_VALTH ; /* 0x3058 */
    volatile U_VPSS_SNR_SFCMAX_IDXTH   VPSS_SNR_SFCMAX_IDXTH ; /* 0x305c */
    volatile U_VPSS_SNR_SFCMEAN_VALTH   VPSS_SNR_SFCMEAN_VALTH ; /* 0x3060 */
    volatile U_VPSS_SNR_WFCMAX_VALTH   VPSS_SNR_WFCMAX_VALTH ; /* 0x3064 */
    volatile U_VPSS_SNR_WFCMAX_IDXTH   VPSS_SNR_WFCMAX_IDXTH ; /* 0x3068 */
    volatile U_VPSS_SNR_WFCMEAN_VALTH   VPSS_SNR_WFCMEAN_VALTH ; /* 0x306c */
    volatile U_VPSS_SNR_FILTER_TYPE   VPSS_SNR_FILTER_TYPE   ; /* 0x3070 */
    volatile U_VPSS_SNR_NOISE_CORE   VPSS_SNR_NOISE_CORE     ; /* 0x3074 */
    volatile U_VPSS_SNR_ADD_NOISE   VPSS_SNR_ADD_NOISE       ; /* 0x3078 */
    volatile U_VPSS_SNR_ABS_SUM     VPSS_SNR_ABS_SUM         ; /* 0x307c */
    volatile U_VPSS_SNR_WIN_ACT     VPSS_SNR_WIN_ACT         ; /* 0x3080 */
    volatile U_VPSS_SNR_DIR_THR0    VPSS_SNR_DIR_THR0        ; /* 0x3084 */
    volatile U_VPSS_SNR_DIR_THR1    VPSS_SNR_DIR_THR1        ; /* 0x3088 */
    volatile U_VPSS_SNR_DIR_THR2    VPSS_SNR_DIR_THR2        ; /* 0x308c */
    volatile U_VPSS_SNR_DIR_THR3    VPSS_SNR_DIR_THR3        ; /* 0x3090 */
    volatile U_VPSS_SNR_THR_DIR0A   VPSS_SNR_THR_DIR0A       ; /* 0x3094 */
    volatile U_VPSS_SNR_THR_DIR0B   VPSS_SNR_THR_DIR0B       ; /* 0x3098 */
    volatile U_VPSS_SNR_THR_DIR1A   VPSS_SNR_THR_DIR1A       ; /* 0x309c */
    volatile U_VPSS_SNR_THR_DIR1B   VPSS_SNR_THR_DIR1B       ; /* 0x30a0 */
    volatile U_VPSS_SNR_THR_DIR2A   VPSS_SNR_THR_DIR2A       ; /* 0x30a4 */
    volatile U_VPSS_SNR_THR_DIR2B   VPSS_SNR_THR_DIR2B       ; /* 0x30a8 */
    volatile U_VPSS_SNR_THR_DIR3A   VPSS_SNR_THR_DIR3A       ; /* 0x30ac */
    volatile U_VPSS_SNR_THR_DIR3B   VPSS_SNR_THR_DIR3B       ; /* 0x30b0 */
    volatile U_VPSS_SNR_THR_DIR4A   VPSS_SNR_THR_DIR4A       ; /* 0x30b4 */
    volatile U_VPSS_SNR_THR_DIR4B   VPSS_SNR_THR_DIR4B       ; /* 0x30b8 */
    volatile U_VPSS_SNR_THR_DIR5A   VPSS_SNR_THR_DIR5A       ; /* 0x30bc */
    volatile U_VPSS_SNR_THR_DIR5B   VPSS_SNR_THR_DIR5B       ; /* 0x30c0 */
    volatile U_VPSS_SNR_THR_DIR6A   VPSS_SNR_THR_DIR6A       ; /* 0x30c4 */
    volatile U_VPSS_SNR_THR_DIR6B   VPSS_SNR_THR_DIR6B       ; /* 0x30c8 */
    volatile U_VPSS_SNR_THR_DIR7A   VPSS_SNR_THR_DIR7A       ; /* 0x30cc */
    volatile U_VPSS_SNR_THR_DIR7B   VPSS_SNR_THR_DIR7B       ; /* 0x30d0 */
    volatile U_VPSS_SNR_DEMO        VPSS_SNR_DEMO            ; /* 0x30d4 */
    volatile unsigned int           reserved_21[10]                     ; /* 0x30d8~0x30fc */
    volatile U_MOTION_EDGE_LUT_01a   MOTION_EDGE_LUT_01a     ; /* 0x3100 */
    volatile U_MOTION_EDGE_LUT_01b   MOTION_EDGE_LUT_01b     ; /* 0x3104 */
    volatile U_MOTION_EDGE_LUT_01c   MOTION_EDGE_LUT_01c     ; /* 0x3108 */
    volatile U_MOTION_EDGE_LUT_01d   MOTION_EDGE_LUT_01d     ; /* 0x310c */
    volatile U_MOTION_EDGE_LUT_01e   MOTION_EDGE_LUT_01e     ; /* 0x3110 */
    volatile U_MOTION_EDGE_LUT_01f   MOTION_EDGE_LUT_01f     ; /* 0x3114 */
    volatile U_MOTION_EDGE_LUT_01g   MOTION_EDGE_LUT_01g     ; /* 0x3118 */
    volatile U_MOTION_EDGE_LUT_01h   MOTION_EDGE_LUT_01h     ; /* 0x311c */
    volatile U_MOTION_EDGE_LUT_02a   MOTION_EDGE_LUT_02a     ; /* 0x3120 */
    volatile U_MOTION_EDGE_LUT_02b   MOTION_EDGE_LUT_02b     ; /* 0x3124 */
    volatile U_MOTION_EDGE_LUT_02c   MOTION_EDGE_LUT_02c     ; /* 0x3128 */
    volatile U_MOTION_EDGE_LUT_02d   MOTION_EDGE_LUT_02d     ; /* 0x312c */
    volatile U_MOTION_EDGE_LUT_02e   MOTION_EDGE_LUT_02e     ; /* 0x3130 */
    volatile U_MOTION_EDGE_LUT_02f   MOTION_EDGE_LUT_02f     ; /* 0x3134 */
    volatile U_MOTION_EDGE_LUT_02g   MOTION_EDGE_LUT_02g     ; /* 0x3138 */
    volatile U_MOTION_EDGE_LUT_02h   MOTION_EDGE_LUT_02h     ; /* 0x313c */
    volatile U_MOTION_EDGE_LUT_03a   MOTION_EDGE_LUT_03a     ; /* 0x3140 */
    volatile U_MOTION_EDGE_LUT_03b   MOTION_EDGE_LUT_03b     ; /* 0x3144 */
    volatile U_MOTION_EDGE_LUT_03c   MOTION_EDGE_LUT_03c     ; /* 0x3148 */
    volatile U_MOTION_EDGE_LUT_03d   MOTION_EDGE_LUT_03d     ; /* 0x314c */
    volatile U_MOTION_EDGE_LUT_03e   MOTION_EDGE_LUT_03e     ; /* 0x3150 */
    volatile U_MOTION_EDGE_LUT_03f   MOTION_EDGE_LUT_03f     ; /* 0x3154 */
    volatile U_MOTION_EDGE_LUT_03g   MOTION_EDGE_LUT_03g     ; /* 0x3158 */
    volatile U_MOTION_EDGE_LUT_03h   MOTION_EDGE_LUT_03h     ; /* 0x315c */
    volatile U_MOTION_EDGE_LUT_04a   MOTION_EDGE_LUT_04a     ; /* 0x3160 */
    volatile U_MOTION_EDGE_LUT_04b   MOTION_EDGE_LUT_04b     ; /* 0x3164 */
    volatile U_MOTION_EDGE_LUT_04c   MOTION_EDGE_LUT_04c     ; /* 0x3168 */
    volatile U_MOTION_EDGE_LUT_04d   MOTION_EDGE_LUT_04d     ; /* 0x316c */
    volatile U_MOTION_EDGE_LUT_04e   MOTION_EDGE_LUT_04e     ; /* 0x3170 */
    volatile U_MOTION_EDGE_LUT_04f   MOTION_EDGE_LUT_04f     ; /* 0x3174 */
    volatile U_MOTION_EDGE_LUT_04g   MOTION_EDGE_LUT_04g     ; /* 0x3178 */
    volatile U_MOTION_EDGE_LUT_04h   MOTION_EDGE_LUT_04h     ; /* 0x317c */
    volatile U_MOTION_EDGE_LUT_05a   MOTION_EDGE_LUT_05a     ; /* 0x3180 */
    volatile U_MOTION_EDGE_LUT_05b   MOTION_EDGE_LUT_05b     ; /* 0x3184 */
    volatile U_MOTION_EDGE_LUT_05c   MOTION_EDGE_LUT_05c     ; /* 0x3188 */
    volatile U_MOTION_EDGE_LUT_05d   MOTION_EDGE_LUT_05d     ; /* 0x318c */
    volatile U_MOTION_EDGE_LUT_05e   MOTION_EDGE_LUT_05e     ; /* 0x3190 */
    volatile U_MOTION_EDGE_LUT_05f   MOTION_EDGE_LUT_05f     ; /* 0x3194 */
    volatile U_MOTION_EDGE_LUT_05g   MOTION_EDGE_LUT_05g     ; /* 0x3198 */
    volatile U_MOTION_EDGE_LUT_05h   MOTION_EDGE_LUT_05h     ; /* 0x319c */
    volatile U_MOTION_EDGE_LUT_06a   MOTION_EDGE_LUT_06a     ; /* 0x31a0 */
    volatile U_MOTION_EDGE_LUT_06b   MOTION_EDGE_LUT_06b     ; /* 0x31a4 */
    volatile U_MOTION_EDGE_LUT_06c   MOTION_EDGE_LUT_06c     ; /* 0x31a8 */
    volatile U_MOTION_EDGE_LUT_06d   MOTION_EDGE_LUT_06d     ; /* 0x31ac */
    volatile U_MOTION_EDGE_LUT_06e   MOTION_EDGE_LUT_06e     ; /* 0x31b0 */
    volatile U_MOTION_EDGE_LUT_06f   MOTION_EDGE_LUT_06f     ; /* 0x31b4 */
    volatile U_MOTION_EDGE_LUT_06g   MOTION_EDGE_LUT_06g     ; /* 0x31b8 */
    volatile U_MOTION_EDGE_LUT_06h   MOTION_EDGE_LUT_06h     ; /* 0x31bc */
    volatile U_MOTION_EDGE_LUT_07a   MOTION_EDGE_LUT_07a     ; /* 0x31c0 */
    volatile U_MOTION_EDGE_LUT_07b   MOTION_EDGE_LUT_07b     ; /* 0x31c4 */
    volatile U_MOTION_EDGE_LUT_07c   MOTION_EDGE_LUT_07c     ; /* 0x31c8 */
    volatile U_MOTION_EDGE_LUT_07d   MOTION_EDGE_LUT_07d     ; /* 0x31cc */
    volatile U_MOTION_EDGE_LUT_07e   MOTION_EDGE_LUT_07e     ; /* 0x31d0 */
    volatile U_MOTION_EDGE_LUT_07f   MOTION_EDGE_LUT_07f     ; /* 0x31d4 */
    volatile U_MOTION_EDGE_LUT_07g   MOTION_EDGE_LUT_07g     ; /* 0x31d8 */
    volatile U_MOTION_EDGE_LUT_07h   MOTION_EDGE_LUT_07h     ; /* 0x31dc */
    volatile U_MOTION_EDGE_LUT_08a   MOTION_EDGE_LUT_08a     ; /* 0x31e0 */
    volatile U_MOTION_EDGE_LUT_08b   MOTION_EDGE_LUT_08b     ; /* 0x31e4 */
    volatile U_MOTION_EDGE_LUT_08c   MOTION_EDGE_LUT_08c     ; /* 0x31e8 */
    volatile U_MOTION_EDGE_LUT_08d   MOTION_EDGE_LUT_08d     ; /* 0x31ec */
    volatile U_MOTION_EDGE_LUT_08e   MOTION_EDGE_LUT_08e     ; /* 0x31f0 */
    volatile U_MOTION_EDGE_LUT_08f   MOTION_EDGE_LUT_08f     ; /* 0x31f4 */
    volatile U_MOTION_EDGE_LUT_08g   MOTION_EDGE_LUT_08g     ; /* 0x31f8 */
    volatile U_MOTION_EDGE_LUT_08h   MOTION_EDGE_LUT_08h     ; /* 0x31fc */
    volatile U_MOTION_EDGE_LUT_09a   MOTION_EDGE_LUT_09a     ; /* 0x3200 */
    volatile U_MOTION_EDGE_LUT_09b   MOTION_EDGE_LUT_09b     ; /* 0x3204 */
    volatile U_MOTION_EDGE_LUT_09c   MOTION_EDGE_LUT_09c     ; /* 0x3208 */
    volatile U_MOTION_EDGE_LUT_09d   MOTION_EDGE_LUT_09d     ; /* 0x320c */
    volatile U_MOTION_EDGE_LUT_09e   MOTION_EDGE_LUT_09e     ; /* 0x3210 */
    volatile U_MOTION_EDGE_LUT_09f   MOTION_EDGE_LUT_09f     ; /* 0x3214 */
    volatile U_MOTION_EDGE_LUT_09g   MOTION_EDGE_LUT_09g     ; /* 0x3218 */
    volatile U_MOTION_EDGE_LUT_09h   MOTION_EDGE_LUT_09h     ; /* 0x321c */
    volatile U_MOTION_EDGE_LUT_10a   MOTION_EDGE_LUT_10a     ; /* 0x3220 */
    volatile U_MOTION_EDGE_LUT_10b   MOTION_EDGE_LUT_10b     ; /* 0x3224 */
    volatile U_MOTION_EDGE_LUT_10c   MOTION_EDGE_LUT_10c     ; /* 0x3228 */
    volatile U_MOTION_EDGE_LUT_10d   MOTION_EDGE_LUT_10d     ; /* 0x322c */
    volatile U_MOTION_EDGE_LUT_10e   MOTION_EDGE_LUT_10e     ; /* 0x3230 */
    volatile U_MOTION_EDGE_LUT_10f   MOTION_EDGE_LUT_10f     ; /* 0x3234 */
    volatile U_MOTION_EDGE_LUT_10g   MOTION_EDGE_LUT_10g     ; /* 0x3238 */
    volatile U_MOTION_EDGE_LUT_10h   MOTION_EDGE_LUT_10h     ; /* 0x323c */
    volatile U_MOTION_EDGE_LUT_11a   MOTION_EDGE_LUT_11a     ; /* 0x3240 */
    volatile U_MOTION_EDGE_LUT_11b   MOTION_EDGE_LUT_11b     ; /* 0x3244 */
    volatile U_MOTION_EDGE_LUT_11c   MOTION_EDGE_LUT_11c     ; /* 0x3248 */
    volatile U_MOTION_EDGE_LUT_11d   MOTION_EDGE_LUT_11d     ; /* 0x324c */
    volatile U_MOTION_EDGE_LUT_11e   MOTION_EDGE_LUT_11e     ; /* 0x3250 */
    volatile U_MOTION_EDGE_LUT_11f   MOTION_EDGE_LUT_11f     ; /* 0x3254 */
    volatile U_MOTION_EDGE_LUT_11g   MOTION_EDGE_LUT_11g     ; /* 0x3258 */
    volatile U_MOTION_EDGE_LUT_11h   MOTION_EDGE_LUT_11h     ; /* 0x325c */
    volatile U_MOTION_EDGE_LUT_12a   MOTION_EDGE_LUT_12a     ; /* 0x3260 */
    volatile U_MOTION_EDGE_LUT_12b   MOTION_EDGE_LUT_12b     ; /* 0x3264 */
    volatile U_MOTION_EDGE_LUT_12c   MOTION_EDGE_LUT_12c     ; /* 0x3268 */
    volatile U_MOTION_EDGE_LUT_12d   MOTION_EDGE_LUT_12d     ; /* 0x326c */
    volatile U_MOTION_EDGE_LUT_12e   MOTION_EDGE_LUT_12e     ; /* 0x3270 */
    volatile U_MOTION_EDGE_LUT_12f   MOTION_EDGE_LUT_12f     ; /* 0x3274 */
    volatile U_MOTION_EDGE_LUT_12g   MOTION_EDGE_LUT_12g     ; /* 0x3278 */
    volatile U_MOTION_EDGE_LUT_12h   MOTION_EDGE_LUT_12h     ; /* 0x327c */
    volatile U_MOTION_EDGE_LUT_13a   MOTION_EDGE_LUT_13a     ; /* 0x3280 */
    volatile U_MOTION_EDGE_LUT_13b   MOTION_EDGE_LUT_13b     ; /* 0x3284 */
    volatile U_MOTION_EDGE_LUT_13c   MOTION_EDGE_LUT_13c     ; /* 0x3288 */
    volatile U_MOTION_EDGE_LUT_13d   MOTION_EDGE_LUT_13d     ; /* 0x328c */
    volatile U_MOTION_EDGE_LUT_13e   MOTION_EDGE_LUT_13e     ; /* 0x3290 */
    volatile U_MOTION_EDGE_LUT_13f   MOTION_EDGE_LUT_13f     ; /* 0x3294 */
    volatile U_MOTION_EDGE_LUT_13g   MOTION_EDGE_LUT_13g     ; /* 0x3298 */
    volatile U_MOTION_EDGE_LUT_13h   MOTION_EDGE_LUT_13h     ; /* 0x329c */
    volatile U_MOTION_EDGE_LUT_14a   MOTION_EDGE_LUT_14a     ; /* 0x32a0 */
    volatile U_MOTION_EDGE_LUT_14b   MOTION_EDGE_LUT_14b     ; /* 0x32a4 */
    volatile U_MOTION_EDGE_LUT_14c   MOTION_EDGE_LUT_14c     ; /* 0x32a8 */
    volatile U_MOTION_EDGE_LUT_14d   MOTION_EDGE_LUT_14d     ; /* 0x32ac */
    volatile U_MOTION_EDGE_LUT_14e   MOTION_EDGE_LUT_14e     ; /* 0x32b0 */
    volatile U_MOTION_EDGE_LUT_14f   MOTION_EDGE_LUT_14f     ; /* 0x32b4 */
    volatile U_MOTION_EDGE_LUT_14g   MOTION_EDGE_LUT_14g     ; /* 0x32b8 */
    volatile U_MOTION_EDGE_LUT_14h   MOTION_EDGE_LUT_14h     ; /* 0x32bc */
    volatile U_MOTION_EDGE_LUT_15a   MOTION_EDGE_LUT_15a     ; /* 0x32c0 */
    volatile U_MOTION_EDGE_LUT_15b   MOTION_EDGE_LUT_15b     ; /* 0x32c4 */
    volatile U_MOTION_EDGE_LUT_15c   MOTION_EDGE_LUT_15c     ; /* 0x32c8 */
    volatile U_MOTION_EDGE_LUT_15d   MOTION_EDGE_LUT_15d     ; /* 0x32cc */
    volatile U_MOTION_EDGE_LUT_15e   MOTION_EDGE_LUT_15e     ; /* 0x32d0 */
    volatile U_MOTION_EDGE_LUT_15f   MOTION_EDGE_LUT_15f     ; /* 0x32d4 */
    volatile U_MOTION_EDGE_LUT_15g   MOTION_EDGE_LUT_15g     ; /* 0x32d8 */
    volatile U_MOTION_EDGE_LUT_15h   MOTION_EDGE_LUT_15h     ; /* 0x32dc */
    volatile U_MOTION_EDGE_LUT_16a   MOTION_EDGE_LUT_16a     ; /* 0x32e0 */
    volatile U_MOTION_EDGE_LUT_16b   MOTION_EDGE_LUT_16b     ; /* 0x32e4 */
    volatile U_MOTION_EDGE_LUT_16c   MOTION_EDGE_LUT_16c     ; /* 0x32e8 */
    volatile U_MOTION_EDGE_LUT_16d   MOTION_EDGE_LUT_16d     ; /* 0x32ec */
    volatile U_MOTION_EDGE_LUT_16e   MOTION_EDGE_LUT_16e     ; /* 0x32f0 */
    volatile U_MOTION_EDGE_LUT_16f   MOTION_EDGE_LUT_16f     ; /* 0x32f4 */
    volatile U_MOTION_EDGE_LUT_16g   MOTION_EDGE_LUT_16g     ; /* 0x32f8 */
    volatile U_MOTION_EDGE_LUT_16h   MOTION_EDGE_LUT_16h     ; /* 0x32fc */
    volatile U_COLOR_BASED_LUT_01a   COLOR_BASED_LUT_01a     ; /* 0x3300 */
    volatile U_COLOR_BASED_LUT_01b   COLOR_BASED_LUT_01b     ; /* 0x3304 */
    volatile U_COLOR_BASED_LUT_01c   COLOR_BASED_LUT_01c     ; /* 0x3308 */
    volatile U_COLOR_BASED_LUT_01d   COLOR_BASED_LUT_01d     ; /* 0x330c */
    volatile U_COLOR_BASED_LUT_02a   COLOR_BASED_LUT_02a     ; /* 0x3310 */
    volatile U_COLOR_BASED_LUT_02b   COLOR_BASED_LUT_02b     ; /* 0x3314 */
    volatile U_COLOR_BASED_LUT_02c   COLOR_BASED_LUT_02c     ; /* 0x3318 */
    volatile U_COLOR_BASED_LUT_02d   COLOR_BASED_LUT_02d     ; /* 0x331c */
    volatile U_COLOR_BASED_LUT_03a   COLOR_BASED_LUT_03a     ; /* 0x3320 */
    volatile U_COLOR_BASED_LUT_03b   COLOR_BASED_LUT_03b     ; /* 0x3324 */
    volatile U_COLOR_BASED_LUT_03c   COLOR_BASED_LUT_03c     ; /* 0x3328 */
    volatile U_COLOR_BASED_LUT_03d   COLOR_BASED_LUT_03d     ; /* 0x332c */
    volatile U_COLOR_BASED_LUT_04a   COLOR_BASED_LUT_04a     ; /* 0x3330 */
    volatile U_COLOR_BASED_LUT_04b   COLOR_BASED_LUT_04b     ; /* 0x3334 */
    volatile U_COLOR_BASED_LUT_04c   COLOR_BASED_LUT_04c     ; /* 0x3338 */
    volatile U_COLOR_BASED_LUT_04d   COLOR_BASED_LUT_04d     ; /* 0x333c */
    volatile U_COLOR_BASED_LUT_05a   COLOR_BASED_LUT_05a     ; /* 0x3340 */
    volatile U_COLOR_BASED_LUT_05b   COLOR_BASED_LUT_05b     ; /* 0x3344 */
    volatile U_COLOR_BASED_LUT_05c   COLOR_BASED_LUT_05c     ; /* 0x3348 */
    volatile U_COLOR_BASED_LUT_05d   COLOR_BASED_LUT_05d     ; /* 0x334c */
    volatile U_COLOR_BASED_LUT_06a   COLOR_BASED_LUT_06a     ; /* 0x3350 */
    volatile U_COLOR_BASED_LUT_06b   COLOR_BASED_LUT_06b     ; /* 0x3354 */
    volatile U_COLOR_BASED_LUT_06c   COLOR_BASED_LUT_06c     ; /* 0x3358 */
    volatile U_COLOR_BASED_LUT_06d   COLOR_BASED_LUT_06d     ; /* 0x335c */
    volatile U_COLOR_BASED_LUT_07a   COLOR_BASED_LUT_07a     ; /* 0x3360 */
    volatile U_COLOR_BASED_LUT_07b   COLOR_BASED_LUT_07b     ; /* 0x3364 */
    volatile U_COLOR_BASED_LUT_07c   COLOR_BASED_LUT_07c     ; /* 0x3368 */
    volatile U_COLOR_BASED_LUT_07d   COLOR_BASED_LUT_07d     ; /* 0x336c */
    volatile U_COLOR_BASED_LUT_08a   COLOR_BASED_LUT_08a     ; /* 0x3370 */
    volatile U_COLOR_BASED_LUT_08b   COLOR_BASED_LUT_08b     ; /* 0x3374 */
    volatile U_COLOR_BASED_LUT_08c   COLOR_BASED_LUT_08c     ; /* 0x3378 */
    volatile U_COLOR_BASED_LUT_08d   COLOR_BASED_LUT_08d     ; /* 0x337c */
    volatile U_COLOR_BASED_LUT_09a   COLOR_BASED_LUT_09a     ; /* 0x3380 */
    volatile U_COLOR_BASED_LUT_09b   COLOR_BASED_LUT_09b     ; /* 0x3384 */
    volatile U_COLOR_BASED_LUT_09c   COLOR_BASED_LUT_09c     ; /* 0x3388 */
    volatile U_COLOR_BASED_LUT_09d   COLOR_BASED_LUT_09d     ; /* 0x338c */
    volatile U_COLOR_BASED_LUT_10a   COLOR_BASED_LUT_10a     ; /* 0x3390 */
    volatile U_COLOR_BASED_LUT_10b   COLOR_BASED_LUT_10b     ; /* 0x3394 */
    volatile U_COLOR_BASED_LUT_10c   COLOR_BASED_LUT_10c     ; /* 0x3398 */
    volatile U_COLOR_BASED_LUT_10d   COLOR_BASED_LUT_10d     ; /* 0x339c */
    volatile U_COLOR_BASED_LUT_11a   COLOR_BASED_LUT_11a     ; /* 0x33a0 */
    volatile U_COLOR_BASED_LUT_11b   COLOR_BASED_LUT_11b     ; /* 0x33a4 */
    volatile U_COLOR_BASED_LUT_11c   COLOR_BASED_LUT_11c     ; /* 0x33a8 */
    volatile U_COLOR_BASED_LUT_11d   COLOR_BASED_LUT_11d     ; /* 0x33ac */
    volatile U_COLOR_BASED_LUT_12a   COLOR_BASED_LUT_12a     ; /* 0x33b0 */
    volatile U_COLOR_BASED_LUT_12b   COLOR_BASED_LUT_12b     ; /* 0x33b4 */
    volatile U_COLOR_BASED_LUT_12c   COLOR_BASED_LUT_12c     ; /* 0x33b8 */
    volatile U_COLOR_BASED_LUT_12d   COLOR_BASED_LUT_12d     ; /* 0x33bc */
    volatile U_COLOR_BASED_LUT_13a   COLOR_BASED_LUT_13a     ; /* 0x33c0 */
    volatile U_COLOR_BASED_LUT_13b   COLOR_BASED_LUT_13b     ; /* 0x33c4 */
    volatile U_COLOR_BASED_LUT_13c   COLOR_BASED_LUT_13c     ; /* 0x33c8 */
    volatile U_COLOR_BASED_LUT_13d   COLOR_BASED_LUT_13d     ; /* 0x33cc */
    volatile U_COLOR_BASED_LUT_14a   COLOR_BASED_LUT_14a     ; /* 0x33d0 */
    volatile U_COLOR_BASED_LUT_14b   COLOR_BASED_LUT_14b     ; /* 0x33d4 */
    volatile U_COLOR_BASED_LUT_14c   COLOR_BASED_LUT_14c     ; /* 0x33d8 */
    volatile U_COLOR_BASED_LUT_14d   COLOR_BASED_LUT_14d     ; /* 0x33dc */
    volatile U_COLOR_BASED_LUT_15a   COLOR_BASED_LUT_15a     ; /* 0x33e0 */
    volatile U_COLOR_BASED_LUT_15b   COLOR_BASED_LUT_15b     ; /* 0x33e4 */
    volatile U_COLOR_BASED_LUT_15c   COLOR_BASED_LUT_15c     ; /* 0x33e8 */
    volatile U_COLOR_BASED_LUT_15d   COLOR_BASED_LUT_15d     ; /* 0x33ec */
    volatile U_COLOR_BASED_LUT_16a   COLOR_BASED_LUT_16a     ; /* 0x33f0 */
    volatile U_COLOR_BASED_LUT_16b   COLOR_BASED_LUT_16b     ; /* 0x33f4 */
    volatile U_COLOR_BASED_LUT_16c   COLOR_BASED_LUT_16c     ; /* 0x33f8 */
    volatile U_COLOR_BASED_LUT_16d   COLOR_BASED_LUT_16d     ; /* 0x33fc */
    volatile unsigned int           reserved_22[256]                     ; /* 0x3400~0x37fc */
    volatile U_VPSS_TNR_MODE        VPSS_TNR_MODE            ; /* 0x3800 */
    volatile U_VPSS_TNR_YMOTION_MAP_CTRL   VPSS_TNR_YMOTION_MAP_CTRL ; /* 0x3804 */
    volatile U_VPSS_TNR_YMOTION_MAP_R0   VPSS_TNR_YMOTION_MAP_R0 ; /* 0x3808 */
    volatile U_VPSS_TNR_YMOTION_MAP_R1   VPSS_TNR_YMOTION_MAP_R1 ; /* 0x380c */
    volatile U_VPSS_TNR_YMOTION_MAP_T1   VPSS_TNR_YMOTION_MAP_T1 ; /* 0x3810 */
    volatile U_VPSS_TNR_YMOTION_MAP_T2   VPSS_TNR_YMOTION_MAP_T2 ; /* 0x3814 */
    volatile U_VPSS_TNR_YMOTION_MAP_K1   VPSS_TNR_YMOTION_MAP_K1 ; /* 0x3818 */
    volatile U_VPSS_TNR_YMOTION_MAP_K2   VPSS_TNR_YMOTION_MAP_K2 ; /* 0x381c */
    volatile U_VPSS_TNR_CMOTION_MAP_CTRL   VPSS_TNR_CMOTION_MAP_CTRL ; /* 0x3820 */
    volatile U_VPSS_TNR_CMOTION_MAP_R0   VPSS_TNR_CMOTION_MAP_R0 ; /* 0x3824 */
    volatile U_VPSS_TNR_CMOTION_MAP_R1   VPSS_TNR_CMOTION_MAP_R1 ; /* 0x3828 */
    volatile U_VPSS_TNR_CMOTION_MAP_T1   VPSS_TNR_CMOTION_MAP_T1 ; /* 0x382c */
    volatile U_VPSS_TNR_CMOTION_MAP_T2   VPSS_TNR_CMOTION_MAP_T2 ; /* 0x3830 */
    volatile U_VPSS_TNR_CMOTION_MAP_K1   VPSS_TNR_CMOTION_MAP_K1 ; /* 0x3834 */
    volatile U_VPSS_TNR_CMOTION_MAP_K2   VPSS_TNR_CMOTION_MAP_K2 ; /* 0x3838 */
    volatile U_VPSS_TNR_YMOTION_MEANRATIO_CTRL   VPSS_TNR_YMOTION_MEANRATIO_CTRL ; /* 0x383c */
    volatile U_VPSS_TNR_YMOTION_MEANRATIO_R0   VPSS_TNR_YMOTION_MEANRATIO_R0 ; /* 0x3840 */
    volatile U_VPSS_TNR_YMOTION_MEANRATIO_R1   VPSS_TNR_YMOTION_MEANRATIO_R1 ; /* 0x3844 */
    volatile U_VPSS_TNR_YMOTION_MEANRATIO_T1   VPSS_TNR_YMOTION_MEANRATIO_T1 ; /* 0x3848 */
    volatile U_VPSS_TNR_YMOTION_MEANRATIO_T2   VPSS_TNR_YMOTION_MEANRATIO_T2 ; /* 0x384c */
    volatile U_VPSS_TNR_YMOTION_MEANRATIO_K1   VPSS_TNR_YMOTION_MEANRATIO_K1 ; /* 0x3850 */
    volatile U_VPSS_TNR_YMOTION_MEANRATIO_K2   VPSS_TNR_YMOTION_MEANRATIO_K2 ; /* 0x3854 */
    volatile U_VPSS_TNR_CMOTION_MEANRATIO_CTRL   VPSS_TNR_CMOTION_MEANRATIO_CTRL ; /* 0x3858 */
    volatile U_VPSS_TNR_CMOTION_MEANRATIO_R0   VPSS_TNR_CMOTION_MEANRATIO_R0 ; /* 0x385c */
    volatile U_VPSS_TNR_CMOTION_MEANRATIO_R1   VPSS_TNR_CMOTION_MEANRATIO_R1 ; /* 0x3860 */
    volatile U_VPSS_TNR_CMOTION_MEANRATIO_T1   VPSS_TNR_CMOTION_MEANRATIO_T1 ; /* 0x3864 */
    volatile U_VPSS_TNR_CMOTION_MEANRATIO_T2   VPSS_TNR_CMOTION_MEANRATIO_T2 ; /* 0x3868 */
    volatile U_VPSS_TNR_CMOTION_MEANRATIO_K1   VPSS_TNR_CMOTION_MEANRATIO_K1 ; /* 0x386c */
    volatile U_VPSS_TNR_CMOTION_MEANRATIO_K2   VPSS_TNR_CMOTION_MEANRATIO_K2 ; /* 0x3870 */
    volatile U_VPSS_TNR_TM_YCMTCOEF   VPSS_TNR_TM_YCMTCOEF   ; /* 0x3874 */
    volatile U_VPSS_TNR_TM_YCLPFCOEF   VPSS_TNR_TM_YCLPFCOEF ; /* 0x3878 */
    volatile U_VPSS_TNR_TE_YHPF1_0   VPSS_TNR_TE_YHPF1_0     ; /* 0x387c */
    volatile U_VPSS_TNR_TE_YHPF1_1   VPSS_TNR_TE_YHPF1_1     ; /* 0x3880 */
    volatile U_VPSS_TNR_TE_YHPF2_0   VPSS_TNR_TE_YHPF2_0     ; /* 0x3884 */
    volatile U_VPSS_TNR_TE_YHPF2_1   VPSS_TNR_TE_YHPF2_1     ; /* 0x3888 */
    volatile U_VPSS_TNR_TE_YHPF3_0   VPSS_TNR_TE_YHPF3_0     ; /* 0x388c */
    volatile U_VPSS_TNR_TE_YHPF3_1   VPSS_TNR_TE_YHPF3_1     ; /* 0x3890 */
    volatile U_VPSS_TNR_TE_YHPF4_0   VPSS_TNR_TE_YHPF4_0     ; /* 0x3894 */
    volatile U_VPSS_TNR_TE_YHPF4_1   VPSS_TNR_TE_YHPF4_1     ; /* 0x3898 */
    volatile U_VPSS_TNR_TE_YMTMEAN_LPFCOEF_0   VPSS_TNR_TE_YMTMEAN_LPFCOEF_0 ; /* 0x389c */
    volatile U_VPSS_TNR_TE_YMTMEAN_LPFCOEF_1   VPSS_TNR_TE_YMTMEAN_LPFCOEF_1 ; /* 0x38a0 */
    volatile U_VPSS_TNR_TE_YCEG_MEANRATIO_CTRL   VPSS_TNR_TE_YCEG_MEANRATIO_CTRL ; /* 0x38a4 */
    volatile U_VPSS_TNR_TE_YCEG_MEANRATIO_R0   VPSS_TNR_TE_YCEG_MEANRATIO_R0 ; /* 0x38a8 */
    volatile U_VPSS_TNR_TE_YCEG_MEANRATIO_R1   VPSS_TNR_TE_YCEG_MEANRATIO_R1 ; /* 0x38ac */
    volatile U_VPSS_TNR_TE_YCEG_MEANRATIO_T1   VPSS_TNR_TE_YCEG_MEANRATIO_T1 ; /* 0x38b0 */
    volatile U_VPSS_TNR_TE_YCEG_MEANRATIO_T2   VPSS_TNR_TE_YCEG_MEANRATIO_T2 ; /* 0x38b4 */
    volatile U_VPSS_TNR_TE_YCEG_MEANRATIO_K1   VPSS_TNR_TE_YCEG_MEANRATIO_K1 ; /* 0x38b8 */
    volatile U_VPSS_TNR_TE_YCEG_MEANRATIO_K2   VPSS_TNR_TE_YCEG_MEANRATIO_K2 ; /* 0x38bc */
    volatile U_VPSS_TNR_TE_CMT_MEANRATIO_CTRL   VPSS_TNR_TE_CMT_MEANRATIO_CTRL ; /* 0x38c0 */
    volatile U_VPSS_TNR_TE_CMT_MEANRATIO_R0   VPSS_TNR_TE_CMT_MEANRATIO_R0 ; /* 0x38c4 */
    volatile U_VPSS_TNR_TE_CMT_MEANRATIO_R1   VPSS_TNR_TE_CMT_MEANRATIO_R1 ; /* 0x38c8 */
    volatile U_VPSS_TNR_TE_CMT_MEANRATIO_T1   VPSS_TNR_TE_CMT_MEANRATIO_T1 ; /* 0x38cc */
    volatile U_VPSS_TNR_TE_CMT_MEANRATIO_T2   VPSS_TNR_TE_CMT_MEANRATIO_T2 ; /* 0x38d0 */
    volatile U_VPSS_TNR_TE_CMT_MEANRATIO_K1   VPSS_TNR_TE_CMT_MEANRATIO_K1 ; /* 0x38d4 */
    volatile U_VPSS_TNR_TE_CMT_MEANRATIO_K2   VPSS_TNR_TE_CMT_MEANRATIO_K2 ; /* 0x38d8 */
    volatile U_VPSS_TNR_TE_YCMT_MAPPING_CTRL   VPSS_TNR_TE_YCMT_MAPPING_CTRL ; /* 0x38dc */
    volatile U_VPSS_TNR_TE_YCMT_MAPPING_R0   VPSS_TNR_TE_YCMT_MAPPING_R0 ; /* 0x38e0 */
    volatile U_VPSS_TNR_TE_YCMT_MAPPING_R1   VPSS_TNR_TE_YCMT_MAPPING_R1 ; /* 0x38e4 */
    volatile U_VPSS_TNR_TE_YCMT_MAPPING_T1   VPSS_TNR_TE_YCMT_MAPPING_T1 ; /* 0x38e8 */
    volatile U_VPSS_TNR_TE_YCMT_MAPPING_T2   VPSS_TNR_TE_YCMT_MAPPING_T2 ; /* 0x38ec */
    volatile U_VPSS_TNR_TE_YCMT_MAPPING_K1   VPSS_TNR_TE_YCMT_MAPPING_K1 ; /* 0x38f0 */
    volatile U_VPSS_TNR_TE_YCMT_MAPPING_K2   VPSS_TNR_TE_YCMT_MAPPING_K2 ; /* 0x38f4 */
    volatile unsigned int           reserved_23[7]                     ; /* 0x38f8~0x3910 */
    volatile U_VPSS_TNR_TE_YCEG_MAPPING_CTRL   VPSS_TNR_TE_YCEG_MAPPING_CTRL ; /* 0x3914 */
    volatile U_VPSS_TNR_TE_YCEG_MAPPING_R0   VPSS_TNR_TE_YCEG_MAPPING_R0 ; /* 0x3918 */
    volatile U_VPSS_TNR_TE_YCEG_MAPPING_R1   VPSS_TNR_TE_YCEG_MAPPING_R1 ; /* 0x391c */
    volatile U_VPSS_TNR_TE_YCEG_MAPPING_T1   VPSS_TNR_TE_YCEG_MAPPING_T1 ; /* 0x3920 */
    volatile U_VPSS_TNR_TE_YCEG_MAPPING_T2   VPSS_TNR_TE_YCEG_MAPPING_T2 ; /* 0x3924 */
    volatile U_VPSS_TNR_TE_YCEG_MAPPING_K1   VPSS_TNR_TE_YCEG_MAPPING_K1 ; /* 0x3928 */
    volatile U_VPSS_TNR_TE_YCEG_MAPPING_K2   VPSS_TNR_TE_YCEG_MAPPING_K2 ; /* 0x392c */
    volatile U_VPSS_TNR_TE_CEG_CK   VPSS_TNR_TE_CEG_CK       ; /* 0x3930 */
    volatile U_VPSS_TNR_TE_CEG_CK2   VPSS_TNR_TE_CEG_CK2     ; /* 0x3934 */
    volatile U_VPSS_TNR_TE_CMT_CK1   VPSS_TNR_TE_CMT_CK1     ; /* 0x3938 */
    volatile U_VPSS_TNR_TE_CMT_CK2   VPSS_TNR_TE_CMT_CK2     ; /* 0x393c */
    volatile U_VPSS_TNR_TE_CMT_CK3   VPSS_TNR_TE_CMT_CK3     ; /* 0x3940 */
    volatile U_VPSS_TNR_TE_CHPF4_COEF0   VPSS_TNR_TE_CHPF4_COEF0 ; /* 0x3944 */
    volatile U_VPSS_TNR_TE_CHPF4_COEF1   VPSS_TNR_TE_CHPF4_COEF1 ; /* 0x3948 */
    volatile U_VPSS_TNR_TE_YEG_WEIGHT   VPSS_TNR_TE_YEG_WEIGHT ; /* 0x394c */
    volatile U_VPSS_TNR_TE_CEG_WEIGHT   VPSS_TNR_TE_CEG_WEIGHT ; /* 0x3950 */
    volatile U_VPSS_TNR_TE_CMEAN_LPFCOEF1   VPSS_TNR_TE_CMEAN_LPFCOEF1 ; /* 0x3954 */
    volatile U_VPSS_TNR_TE_CMEAN_LPFCOEF2   VPSS_TNR_TE_CMEAN_LPFCOEF2 ; /* 0x3958 */
    volatile U_VPSS_TNR_TE_YMT_CK1   VPSS_TNR_TE_YMT_CK1     ; /* 0x395c */
    volatile U_VPSS_TNR_TE_YMT_CK2   VPSS_TNR_TE_YMT_CK2     ; /* 0x3960 */
    volatile U_VPSS_TNR_TE_YEG_CK1   VPSS_TNR_TE_YEG_CK1     ; /* 0x3964 */
    volatile U_VPSS_TNR_TE_YMT_CK   VPSS_TNR_TE_YMT_CK       ; /* 0x3968 */
    volatile U_VPSS_TNR_TE_YEG_CK   VPSS_TNR_TE_YEG_CK       ; /* 0x396c */
    volatile U_VPSS_TNR_TE_YMOVING_CK   VPSS_TNR_TE_YMOVING_CK ; /* 0x3970 */
    volatile U_VPSS_TNR_TFM_CMOTION_CK   VPSS_TNR_TFM_CMOTION_CK ; /* 0x3974 */
    volatile U_VPSS_TNR_TFM_CEDGE_CK   VPSS_TNR_TFM_CEDGE_CK ; /* 0x3978 */
    volatile U_VPSS_TNR_TFM_Y_MAPPING_CTRL   VPSS_TNR_TFM_Y_MAPPING_CTRL ; /* 0x397c */
    volatile U_VPSS_TNR_TFM_Y_MAPPING_R0   VPSS_TNR_TFM_Y_MAPPING_R0 ; /* 0x3980 */
    volatile U_VPSS_TNR_TFM_Y_MAPPING_R1   VPSS_TNR_TFM_Y_MAPPING_R1 ; /* 0x3984 */
    volatile U_VPSS_TNR_TFM_Y_MAPPING_T1   VPSS_TNR_TFM_Y_MAPPING_T1 ; /* 0x3988 */
    volatile U_VPSS_TNR_TFM_Y_MAPPING_T2   VPSS_TNR_TFM_Y_MAPPING_T2 ; /* 0x398c */
    volatile U_VPSS_TNR_TFM_Y_MAPPING_K1   VPSS_TNR_TFM_Y_MAPPING_K1 ; /* 0x3990 */
    volatile U_VPSS_TNR_TFM_Y_MAPPING_K2   VPSS_TNR_TFM_Y_MAPPING_K2 ; /* 0x3994 */
    volatile U_VPSS_TNR_TFM_C_MAPPING_CTRL   VPSS_TNR_TFM_C_MAPPING_CTRL ; /* 0x3998 */
    volatile U_VPSS_TNR_TFM_C_MAPPING_R0   VPSS_TNR_TFM_C_MAPPING_R0 ; /* 0x399c */
    volatile U_VPSS_TNR_TFM_C_MAPPING_R1   VPSS_TNR_TFM_C_MAPPING_R1 ; /* 0x39a0 */
    volatile U_VPSS_TNR_TFM_C_MAPPING_T1   VPSS_TNR_TFM_C_MAPPING_T1 ; /* 0x39a4 */
    volatile U_VPSS_TNR_TFM_C_MAPPING_T2   VPSS_TNR_TFM_C_MAPPING_T2 ; /* 0x39a8 */
    volatile U_VPSS_TNR_TFM_C_MAPPING_K1   VPSS_TNR_TFM_C_MAPPING_K1 ; /* 0x39ac */
    volatile U_VPSS_TNR_TFM_C_MAPPING_K2   VPSS_TNR_TFM_C_MAPPING_K2 ; /* 0x39b0 */
    volatile U_VPSS_TNR_TFM_HISTMOTION_BLEND   VPSS_TNR_TFM_HISTMOTION_BLEND ; /* 0x39b4 */
    volatile U_VPSS_TNR_TFM_PARA    VPSS_TNR_TFM_PARA        ; /* 0x39b8 */
    volatile U_VPSS_TNR_TFM_YMOTION_CK   VPSS_TNR_TFM_YMOTION_CK ; /* 0x39bc */
    volatile U_VPSS_TNR_TFM_YEG_CK   VPSS_TNR_TFM_YEG_CK     ; /* 0x39c0 */
    volatile U_VPSS_TNR_TM_YMTMEAN_CK   VPSS_TNR_TM_YMTMEAN_CK ; /* 0x39c4 */
    volatile U_VPSS_TNR_TYMYCK      VPSS_TNR_TYMYCK          ; /* 0x39c8 */
    volatile U_VPSS_TNR_TYMEYCK     VPSS_TNR_TYMEYCK         ; /* 0x39cc */
    volatile U_VPSS_TNR_TCMYCK      VPSS_TNR_TCMYCK          ; /* 0x39d0 */
    volatile U_VPSS_TNR_TCMEYCK     VPSS_TNR_TCMEYCK         ; /* 0x39d4 */
    volatile U_VPSS_TNR_TM_CMT_CK   VPSS_TNR_TM_CMT_CK       ; /* 0x39d8 */
    volatile U_VPSS_TNR_TM_YMT_CK   VPSS_TNR_TM_YMT_CK       ; /* 0x39dc */
    volatile U_VPSS_TNR_TM_CMTMEAN_CK   VPSS_TNR_TM_CMTMEAN_CK ; /* 0x39e0 */
    volatile U_VPSS_TNR_SNR_CTRL    VPSS_TNR_SNR_CTRL        ; /* 0x39e4 */
    volatile U_VPSS_TNR_BLEND_CTRL   VPSS_TNR_BLEND_CTRL     ; /* 0x39e8 */
    volatile U_VPSS_TNR_T2D_EDGE_CK   VPSS_TNR_T2D_EDGE_CK   ; /* 0x39ec */
    volatile U_VPSS_TNR_T2D_NOISE_CK   VPSS_TNR_T2D_NOISE_CK ; /* 0x39f0 */
    volatile U_VPSS_TNR_EDGE_CMOVING_CK   VPSS_TNR_EDGE_CMOVING_CK ; /* 0x39f4 */
    volatile unsigned int           reserved_24                     ; /* 0x39f8 */
    volatile U_VPSS_TNR_MARKET      VPSS_TNR_MARKET          ; /* 0x39fc */
    volatile unsigned int           reserved_25[64]                     ; /* 0x3a00~0x3afc */
    volatile U_VPSS_TNR_CLUT10      VPSS_TNR_CLUT10          ; /* 0x3b00 */
    volatile U_VPSS_TNR_CLUT11      VPSS_TNR_CLUT11          ; /* 0x3b04 */
    volatile U_VPSS_TNR_CLUT12      VPSS_TNR_CLUT12          ; /* 0x3b08 */
    volatile U_VPSS_TNR_CLUT13      VPSS_TNR_CLUT13          ; /* 0x3b0c */
    volatile U_VPSS_TNR_CLUT14      VPSS_TNR_CLUT14          ; /* 0x3b10 */
    volatile U_VPSS_TNR_CLUT15      VPSS_TNR_CLUT15          ; /* 0x3b14 */
    volatile U_VPSS_TNR_CLUT16      VPSS_TNR_CLUT16          ; /* 0x3b18 */
    volatile U_VPSS_TNR_CLUT17      VPSS_TNR_CLUT17          ; /* 0x3b1c */
    volatile U_VPSS_TNR_CLUT18      VPSS_TNR_CLUT18          ; /* 0x3b20 */
    volatile U_VPSS_TNR_CLUT19      VPSS_TNR_CLUT19          ; /* 0x3b24 */
    volatile U_VPSS_TNR_CLUT110     VPSS_TNR_CLUT110         ; /* 0x3b28 */
    volatile U_VPSS_TNR_CLUT111     VPSS_TNR_CLUT111         ; /* 0x3b2c */
    volatile U_VPSS_TNR_CLUT112     VPSS_TNR_CLUT112         ; /* 0x3b30 */
    volatile U_VPSS_TNR_CLUT113     VPSS_TNR_CLUT113         ; /* 0x3b34 */
    volatile U_VPSS_TNR_CLUT114     VPSS_TNR_CLUT114         ; /* 0x3b38 */
    volatile U_VPSS_TNR_CLUT115     VPSS_TNR_CLUT115         ; /* 0x3b3c */
    volatile U_VPSS_TNR_CLUT20      VPSS_TNR_CLUT20          ; /* 0x3b40 */
    volatile U_VPSS_TNR_CLUT21      VPSS_TNR_CLUT21          ; /* 0x3b44 */
    volatile U_VPSS_TNR_CLUT22      VPSS_TNR_CLUT22          ; /* 0x3b48 */
    volatile U_VPSS_TNR_CLUT23      VPSS_TNR_CLUT23          ; /* 0x3b4c */
    volatile U_VPSS_TNR_CLUT24      VPSS_TNR_CLUT24          ; /* 0x3b50 */
    volatile U_VPSS_TNR_CLUT25      VPSS_TNR_CLUT25          ; /* 0x3b54 */
    volatile U_VPSS_TNR_CLUT26      VPSS_TNR_CLUT26          ; /* 0x3b58 */
    volatile U_VPSS_TNR_CLUT27      VPSS_TNR_CLUT27          ; /* 0x3b5c */
    volatile U_VPSS_TNR_CLUT28      VPSS_TNR_CLUT28          ; /* 0x3b60 */
    volatile U_VPSS_TNR_CLUT29      VPSS_TNR_CLUT29          ; /* 0x3b64 */
    volatile U_VPSS_TNR_CLUT210     VPSS_TNR_CLUT210         ; /* 0x3b68 */
    volatile U_VPSS_TNR_CLUT211     VPSS_TNR_CLUT211         ; /* 0x3b6c */
    volatile U_VPSS_TNR_CLUT212     VPSS_TNR_CLUT212         ; /* 0x3b70 */
    volatile U_VPSS_TNR_CLUT213     VPSS_TNR_CLUT213         ; /* 0x3b74 */
    volatile U_VPSS_TNR_CLUT214     VPSS_TNR_CLUT214         ; /* 0x3b78 */
    volatile U_VPSS_TNR_CLUT215     VPSS_TNR_CLUT215         ; /* 0x3b7c */
    volatile U_VPSS_TNR_CLUT216     VPSS_TNR_CLUT216         ; /* 0x3b80 */
    volatile U_VPSS_TNR_CLUT217     VPSS_TNR_CLUT217         ; /* 0x3b84 */
    volatile U_VPSS_TNR_CLUT218     VPSS_TNR_CLUT218         ; /* 0x3b88 */
    volatile U_VPSS_TNR_CLUT219     VPSS_TNR_CLUT219         ; /* 0x3b8c */
    volatile U_VPSS_TNR_CLUT220     VPSS_TNR_CLUT220         ; /* 0x3b90 */
    volatile U_VPSS_TNR_CLUT221     VPSS_TNR_CLUT221         ; /* 0x3b94 */
    volatile U_VPSS_TNR_CLUT222     VPSS_TNR_CLUT222         ; /* 0x3b98 */
    volatile U_VPSS_TNR_CLUT223     VPSS_TNR_CLUT223         ; /* 0x3b9c */
    volatile U_VPSS_TNR_CLUT224     VPSS_TNR_CLUT224         ; /* 0x3ba0 */
    volatile U_VPSS_TNR_CLUT225     VPSS_TNR_CLUT225         ; /* 0x3ba4 */
    volatile U_VPSS_TNR_CLUT226     VPSS_TNR_CLUT226         ; /* 0x3ba8 */
    volatile U_VPSS_TNR_CLUT227     VPSS_TNR_CLUT227         ; /* 0x3bac */
    volatile U_VPSS_TNR_CLUT228     VPSS_TNR_CLUT228         ; /* 0x3bb0 */
    volatile U_VPSS_TNR_CLUT229     VPSS_TNR_CLUT229         ; /* 0x3bb4 */
    volatile U_VPSS_TNR_CLUT230     VPSS_TNR_CLUT230         ; /* 0x3bb8 */
    volatile U_VPSS_TNR_CLUT231     VPSS_TNR_CLUT231         ; /* 0x3bbc */
    volatile U_VPSS_TNR_CLUT232     VPSS_TNR_CLUT232         ; /* 0x3bc0 */
    volatile U_VPSS_TNR_CLUT233     VPSS_TNR_CLUT233         ; /* 0x3bc4 */
    volatile U_VPSS_TNR_CLUT234     VPSS_TNR_CLUT234         ; /* 0x3bc8 */
    volatile U_VPSS_TNR_CLUT235     VPSS_TNR_CLUT235         ; /* 0x3bcc */
    volatile U_VPSS_TNR_CLUT236     VPSS_TNR_CLUT236         ; /* 0x3bd0 */
    volatile U_VPSS_TNR_CLUT237     VPSS_TNR_CLUT237         ; /* 0x3bd4 */
    volatile U_VPSS_TNR_CLUT238     VPSS_TNR_CLUT238         ; /* 0x3bd8 */
    volatile U_VPSS_TNR_CLUT239     VPSS_TNR_CLUT239         ; /* 0x3bdc */
    volatile U_VPSS_TNR_CLUT240     VPSS_TNR_CLUT240         ; /* 0x3be0 */
    volatile U_VPSS_TNR_CLUT241     VPSS_TNR_CLUT241         ; /* 0x3be4 */
    volatile U_VPSS_TNR_CLUT242     VPSS_TNR_CLUT242         ; /* 0x3be8 */
    volatile U_VPSS_TNR_CLUT243     VPSS_TNR_CLUT243         ; /* 0x3bec */
    volatile U_VPSS_TNR_CLUT244     VPSS_TNR_CLUT244         ; /* 0x3bf0 */
    volatile U_VPSS_TNR_CLUT245     VPSS_TNR_CLUT245         ; /* 0x3bf4 */
    volatile U_VPSS_TNR_CLUT246     VPSS_TNR_CLUT246         ; /* 0x3bf8 */
    volatile U_VPSS_TNR_CLUT247     VPSS_TNR_CLUT247         ; /* 0x3bfc */
    volatile U_VPSS_TNR_CLUT248     VPSS_TNR_CLUT248         ; /* 0x3c00 */
    volatile U_VPSS_TNR_CLUT249     VPSS_TNR_CLUT249         ; /* 0x3c04 */
    volatile U_VPSS_TNR_CLUT250     VPSS_TNR_CLUT250         ; /* 0x3c08 */
    volatile U_VPSS_TNR_CLUT251     VPSS_TNR_CLUT251         ; /* 0x3c0c */
    volatile U_VPSS_TNR_CLUT252     VPSS_TNR_CLUT252         ; /* 0x3c10 */
    volatile U_VPSS_TNR_CLUT253     VPSS_TNR_CLUT253         ; /* 0x3c14 */
    volatile U_VPSS_TNR_CLUT254     VPSS_TNR_CLUT254         ; /* 0x3c18 */
    volatile U_VPSS_TNR_CLUT255     VPSS_TNR_CLUT255         ; /* 0x3c1c */
    volatile U_VPSS_TNR_CLUT256     VPSS_TNR_CLUT256         ; /* 0x3c20 */
    volatile U_VPSS_TNR_CLUT257     VPSS_TNR_CLUT257         ; /* 0x3c24 */
    volatile U_VPSS_TNR_CLUT258     VPSS_TNR_CLUT258         ; /* 0x3c28 */
    volatile U_VPSS_TNR_CLUT259     VPSS_TNR_CLUT259         ; /* 0x3c2c */
    volatile U_VPSS_TNR_CLUT260     VPSS_TNR_CLUT260         ; /* 0x3c30 */
    volatile U_VPSS_TNR_CLUT261     VPSS_TNR_CLUT261         ; /* 0x3c34 */
    volatile U_VPSS_TNR_CLUT262     VPSS_TNR_CLUT262         ; /* 0x3c38 */
    volatile U_VPSS_TNR_CLUT263     VPSS_TNR_CLUT263         ; /* 0x3c3c */
    volatile U_VPSS_TNR_CLUT264     VPSS_TNR_CLUT264         ; /* 0x3c40 */
    volatile U_VPSS_TNR_CLUT265     VPSS_TNR_CLUT265         ; /* 0x3c44 */
    volatile U_VPSS_TNR_CLUT266     VPSS_TNR_CLUT266         ; /* 0x3c48 */
    volatile U_VPSS_TNR_CLUT267     VPSS_TNR_CLUT267         ; /* 0x3c4c */
    volatile U_VPSS_TNR_CLUT268     VPSS_TNR_CLUT268         ; /* 0x3c50 */
    volatile U_VPSS_TNR_CLUT269     VPSS_TNR_CLUT269         ; /* 0x3c54 */
    volatile U_VPSS_TNR_CLUT270     VPSS_TNR_CLUT270         ; /* 0x3c58 */
    volatile U_VPSS_TNR_CLUT271     VPSS_TNR_CLUT271         ; /* 0x3c5c */
    volatile U_VPSS_TNR_CLUT272     VPSS_TNR_CLUT272         ; /* 0x3c60 */
    volatile U_VPSS_TNR_CLUT273     VPSS_TNR_CLUT273         ; /* 0x3c64 */
    volatile U_VPSS_TNR_CLUT274     VPSS_TNR_CLUT274         ; /* 0x3c68 */
    volatile U_VPSS_TNR_CLUT275     VPSS_TNR_CLUT275         ; /* 0x3c6c */
    volatile U_VPSS_TNR_CLUT276     VPSS_TNR_CLUT276         ; /* 0x3c70 */
    volatile U_VPSS_TNR_CLUT277     VPSS_TNR_CLUT277         ; /* 0x3c74 */
    volatile U_VPSS_TNR_CLUT278     VPSS_TNR_CLUT278         ; /* 0x3c78 */
    volatile U_VPSS_TNR_CLUT279     VPSS_TNR_CLUT279         ; /* 0x3c7c */
    volatile U_VPSS_TNR_CLUT280     VPSS_TNR_CLUT280         ; /* 0x3c80 */
    volatile U_VPSS_TNR_CLUT281     VPSS_TNR_CLUT281         ; /* 0x3c84 */
    volatile U_VPSS_TNR_CLUT282     VPSS_TNR_CLUT282         ; /* 0x3c88 */
    volatile U_VPSS_TNR_CLUT283     VPSS_TNR_CLUT283         ; /* 0x3c8c */
    volatile U_VPSS_TNR_CLUT284     VPSS_TNR_CLUT284         ; /* 0x3c90 */
    volatile U_VPSS_TNR_CLUT285     VPSS_TNR_CLUT285         ; /* 0x3c94 */
    volatile U_VPSS_TNR_CLUT286     VPSS_TNR_CLUT286         ; /* 0x3c98 */
    volatile U_VPSS_TNR_CLUT287     VPSS_TNR_CLUT287         ; /* 0x3c9c */
    volatile U_VPSS_TNR_CLUT288     VPSS_TNR_CLUT288         ; /* 0x3ca0 */
    volatile U_VPSS_TNR_CLUT289     VPSS_TNR_CLUT289         ; /* 0x3ca4 */
    volatile U_VPSS_TNR_CLUT290     VPSS_TNR_CLUT290         ; /* 0x3ca8 */
    volatile U_VPSS_TNR_CLUT291     VPSS_TNR_CLUT291         ; /* 0x3cac */
    volatile U_VPSS_TNR_CLUT292     VPSS_TNR_CLUT292         ; /* 0x3cb0 */
    volatile U_VPSS_TNR_CLUT293     VPSS_TNR_CLUT293         ; /* 0x3cb4 */
    volatile U_VPSS_TNR_CLUT294     VPSS_TNR_CLUT294         ; /* 0x3cb8 */
    volatile U_VPSS_TNR_CLUT295     VPSS_TNR_CLUT295         ; /* 0x3cbc */
    volatile U_VPSS_TNR_CLUT296     VPSS_TNR_CLUT296         ; /* 0x3cc0 */
    volatile U_VPSS_TNR_CLUT297     VPSS_TNR_CLUT297         ; /* 0x3cc4 */
    volatile U_VPSS_TNR_CLUT298     VPSS_TNR_CLUT298         ; /* 0x3cc8 */
    volatile U_VPSS_TNR_CLUT299     VPSS_TNR_CLUT299         ; /* 0x3ccc */
    volatile U_VPSS_TNR_CLUT2100    VPSS_TNR_CLUT2100        ; /* 0x3cd0 */
    volatile U_VPSS_TNR_CLUT2101    VPSS_TNR_CLUT2101        ; /* 0x3cd4 */
    volatile U_VPSS_TNR_CLUT2102    VPSS_TNR_CLUT2102        ; /* 0x3cd8 */
    volatile U_VPSS_TNR_CLUT2103    VPSS_TNR_CLUT2103        ; /* 0x3cdc */
    volatile U_VPSS_TNR_CLUT2104    VPSS_TNR_CLUT2104        ; /* 0x3ce0 */
    volatile U_VPSS_TNR_CLUT2105    VPSS_TNR_CLUT2105        ; /* 0x3ce4 */
    volatile U_VPSS_TNR_CLUT2106    VPSS_TNR_CLUT2106        ; /* 0x3ce8 */
    volatile U_VPSS_TNR_CLUT2107    VPSS_TNR_CLUT2107        ; /* 0x3cec */
    volatile U_VPSS_TNR_CLUT2108    VPSS_TNR_CLUT2108        ; /* 0x3cf0 */
    volatile U_VPSS_TNR_CLUT2109    VPSS_TNR_CLUT2109        ; /* 0x3cf4 */
    volatile U_VPSS_TNR_CLUT2110    VPSS_TNR_CLUT2110        ; /* 0x3cf8 */
    volatile U_VPSS_TNR_CLUT2111    VPSS_TNR_CLUT2111        ; /* 0x3cfc */
    volatile U_VPSS_TNR_CLUT2112    VPSS_TNR_CLUT2112        ; /* 0x3d00 */
    volatile U_VPSS_TNR_CLUT2113    VPSS_TNR_CLUT2113        ; /* 0x3d04 */
    volatile U_VPSS_TNR_CLUT2114    VPSS_TNR_CLUT2114        ; /* 0x3d08 */
    volatile U_VPSS_TNR_CLUT2115    VPSS_TNR_CLUT2115        ; /* 0x3d0c */
    volatile U_VPSS_TNR_CLUT2116    VPSS_TNR_CLUT2116        ; /* 0x3d10 */
    volatile U_VPSS_TNR_CLUT2117    VPSS_TNR_CLUT2117        ; /* 0x3d14 */
    volatile U_VPSS_TNR_CLUT2118    VPSS_TNR_CLUT2118        ; /* 0x3d18 */
    volatile U_VPSS_TNR_CLUT2119    VPSS_TNR_CLUT2119        ; /* 0x3d1c */
    volatile U_VPSS_TNR_CLUT2120    VPSS_TNR_CLUT2120        ; /* 0x3d20 */
    volatile U_VPSS_TNR_CLUT2121    VPSS_TNR_CLUT2121        ; /* 0x3d24 */
    volatile U_VPSS_TNR_CLUT2122    VPSS_TNR_CLUT2122        ; /* 0x3d28 */
    volatile U_VPSS_TNR_CLUT2123    VPSS_TNR_CLUT2123        ; /* 0x3d2c */
    volatile U_VPSS_TNR_CLUT2124    VPSS_TNR_CLUT2124        ; /* 0x3d30 */
    volatile U_VPSS_TNR_CLUT2125    VPSS_TNR_CLUT2125        ; /* 0x3d34 */
    volatile U_VPSS_TNR_CLUT2126    VPSS_TNR_CLUT2126        ; /* 0x3d38 */
    volatile U_VPSS_TNR_CLUT2127    VPSS_TNR_CLUT2127        ; /* 0x3d3c */
    volatile U_VPSS_TNR_CLUT30      VPSS_TNR_CLUT30          ; /* 0x3d40 */
    volatile U_VPSS_TNR_CLUT31      VPSS_TNR_CLUT31          ; /* 0x3d44 */
    volatile U_VPSS_TNR_CLUT32      VPSS_TNR_CLUT32          ; /* 0x3d48 */
    volatile U_VPSS_TNR_CLUT33      VPSS_TNR_CLUT33          ; /* 0x3d4c */
    volatile U_VPSS_TNR_CLUT34      VPSS_TNR_CLUT34          ; /* 0x3d50 */
    volatile U_VPSS_TNR_CLUT35      VPSS_TNR_CLUT35          ; /* 0x3d54 */
    volatile U_VPSS_TNR_CLUT36      VPSS_TNR_CLUT36          ; /* 0x3d58 */
    volatile U_VPSS_TNR_CLUT37      VPSS_TNR_CLUT37          ; /* 0x3d5c */
    volatile U_VPSS_TNR_CLUT38      VPSS_TNR_CLUT38          ; /* 0x3d60 */
    volatile U_VPSS_TNR_CLUT39      VPSS_TNR_CLUT39          ; /* 0x3d64 */
    volatile U_VPSS_TNR_CLUT310     VPSS_TNR_CLUT310         ; /* 0x3d68 */
    volatile U_VPSS_TNR_CLUT311     VPSS_TNR_CLUT311         ; /* 0x3d6c */
    volatile U_VPSS_TNR_CLUT312     VPSS_TNR_CLUT312         ; /* 0x3d70 */
    volatile U_VPSS_TNR_CLUT313     VPSS_TNR_CLUT313         ; /* 0x3d74 */
    volatile U_VPSS_TNR_CLUT314     VPSS_TNR_CLUT314         ; /* 0x3d78 */
    volatile U_VPSS_TNR_CLUT315     VPSS_TNR_CLUT315         ; /* 0x3d7c */
    volatile U_VPSS_TNR_CLUT316     VPSS_TNR_CLUT316         ; /* 0x3d80 */
    volatile U_VPSS_TNR_CLUT317     VPSS_TNR_CLUT317         ; /* 0x3d84 */
    volatile U_VPSS_TNR_CLUT318     VPSS_TNR_CLUT318         ; /* 0x3d88 */
    volatile U_VPSS_TNR_CLUT319     VPSS_TNR_CLUT319         ; /* 0x3d8c */
    volatile U_VPSS_TNR_CLUT320     VPSS_TNR_CLUT320         ; /* 0x3d90 */
    volatile U_VPSS_TNR_CLUT321     VPSS_TNR_CLUT321         ; /* 0x3d94 */
    volatile U_VPSS_TNR_CLUT322     VPSS_TNR_CLUT322         ; /* 0x3d98 */
    volatile U_VPSS_TNR_CLUT323     VPSS_TNR_CLUT323         ; /* 0x3d9c */
    volatile U_VPSS_TNR_CLUT324     VPSS_TNR_CLUT324         ; /* 0x3da0 */
    volatile U_VPSS_TNR_CLUT325     VPSS_TNR_CLUT325         ; /* 0x3da4 */
    volatile U_VPSS_TNR_CLUT326     VPSS_TNR_CLUT326         ; /* 0x3da8 */
    volatile U_VPSS_TNR_CLUT327     VPSS_TNR_CLUT327         ; /* 0x3dac */
    volatile U_VPSS_TNR_CLUT328     VPSS_TNR_CLUT328         ; /* 0x3db0 */
    volatile U_VPSS_TNR_CLUT329     VPSS_TNR_CLUT329         ; /* 0x3db4 */
    volatile U_VPSS_TNR_CLUT330     VPSS_TNR_CLUT330         ; /* 0x3db8 */
    volatile U_VPSS_TNR_CLUT331     VPSS_TNR_CLUT331         ; /* 0x3dbc */
    volatile U_VPSS_TNR_CLUT40      VPSS_TNR_CLUT40          ; /* 0x3dc0 */
    volatile U_VPSS_TNR_CLUT41      VPSS_TNR_CLUT41          ; /* 0x3dc4 */
    volatile U_VPSS_TNR_CLUT42      VPSS_TNR_CLUT42          ; /* 0x3dc8 */
    volatile U_VPSS_TNR_CLUT43      VPSS_TNR_CLUT43          ; /* 0x3dcc */
    volatile U_VPSS_TNR_CLUT44      VPSS_TNR_CLUT44          ; /* 0x3dd0 */
    volatile U_VPSS_TNR_CLUT45      VPSS_TNR_CLUT45          ; /* 0x3dd4 */
    volatile U_VPSS_TNR_CLUT46      VPSS_TNR_CLUT46          ; /* 0x3dd8 */
    volatile U_VPSS_TNR_CLUT47      VPSS_TNR_CLUT47          ; /* 0x3ddc */
    volatile U_VPSS_TNR_CLUT48      VPSS_TNR_CLUT48          ; /* 0x3de0 */
    volatile U_VPSS_TNR_CLUT49      VPSS_TNR_CLUT49          ; /* 0x3de4 */
    volatile U_VPSS_TNR_CLUT410     VPSS_TNR_CLUT410         ; /* 0x3de8 */
    volatile U_VPSS_TNR_CLUT411     VPSS_TNR_CLUT411         ; /* 0x3dec */
    volatile U_VPSS_TNR_CLUT412     VPSS_TNR_CLUT412         ; /* 0x3df0 */
    volatile U_VPSS_TNR_CLUT413     VPSS_TNR_CLUT413         ; /* 0x3df4 */
    volatile U_VPSS_TNR_CLUT414     VPSS_TNR_CLUT414         ; /* 0x3df8 */
    volatile U_VPSS_TNR_CLUT415     VPSS_TNR_CLUT415         ; /* 0x3dfc */
    volatile U_VPSS_TNR_CLUT416     VPSS_TNR_CLUT416         ; /* 0x3e00 */
    volatile U_VPSS_TNR_CLUT417     VPSS_TNR_CLUT417         ; /* 0x3e04 */
    volatile U_VPSS_TNR_CLUT418     VPSS_TNR_CLUT418         ; /* 0x3e08 */
    volatile U_VPSS_TNR_CLUT419     VPSS_TNR_CLUT419         ; /* 0x3e0c */
    volatile U_VPSS_TNR_CLUT420     VPSS_TNR_CLUT420         ; /* 0x3e10 */
    volatile U_VPSS_TNR_CLUT421     VPSS_TNR_CLUT421         ; /* 0x3e14 */
    volatile U_VPSS_TNR_CLUT422     VPSS_TNR_CLUT422         ; /* 0x3e18 */
    volatile U_VPSS_TNR_CLUT423     VPSS_TNR_CLUT423         ; /* 0x3e1c */
    volatile U_VPSS_TNR_CLUT424     VPSS_TNR_CLUT424         ; /* 0x3e20 */
    volatile U_VPSS_TNR_CLUT425     VPSS_TNR_CLUT425         ; /* 0x3e24 */
    volatile U_VPSS_TNR_CLUT426     VPSS_TNR_CLUT426         ; /* 0x3e28 */
    volatile U_VPSS_TNR_CLUT427     VPSS_TNR_CLUT427         ; /* 0x3e2c */
    volatile U_VPSS_TNR_CLUT428     VPSS_TNR_CLUT428         ; /* 0x3e30 */
    volatile U_VPSS_TNR_CLUT429     VPSS_TNR_CLUT429         ; /* 0x3e34 */
    volatile U_VPSS_TNR_CLUT430     VPSS_TNR_CLUT430         ; /* 0x3e38 */
    volatile U_VPSS_TNR_CLUT431     VPSS_TNR_CLUT431         ; /* 0x3e3c */
    volatile U_VPSS_TNR_CLUT50      VPSS_TNR_CLUT50          ; /* 0x3e40 */
    volatile U_VPSS_TNR_CLUT51      VPSS_TNR_CLUT51          ; /* 0x3e44 */
    volatile U_VPSS_TNR_CLUT52      VPSS_TNR_CLUT52          ; /* 0x3e48 */
    volatile U_VPSS_TNR_CLUT53      VPSS_TNR_CLUT53          ; /* 0x3e4c */
    volatile U_VPSS_TNR_CLUT54      VPSS_TNR_CLUT54          ; /* 0x3e50 */
    volatile U_VPSS_TNR_CLUT55      VPSS_TNR_CLUT55          ; /* 0x3e54 */
    volatile U_VPSS_TNR_CLUT56      VPSS_TNR_CLUT56          ; /* 0x3e58 */
    volatile U_VPSS_TNR_CLUT57      VPSS_TNR_CLUT57          ; /* 0x3e5c */
    volatile U_VPSS_TNR_CLUT60      VPSS_TNR_CLUT60          ; /* 0x3e60 */
    volatile U_VPSS_TNR_CLUT61      VPSS_TNR_CLUT61          ; /* 0x3e64 */
    volatile U_VPSS_TNR_CLUT62      VPSS_TNR_CLUT62          ; /* 0x3e68 */
    volatile U_VPSS_TNR_CLUT63      VPSS_TNR_CLUT63          ; /* 0x3e6c */
    volatile U_VPSS_TNR_CLUT64      VPSS_TNR_CLUT64          ; /* 0x3e70 */
    volatile U_VPSS_TNR_CLUT65      VPSS_TNR_CLUT65          ; /* 0x3e74 */
    volatile U_VPSS_TNR_CLUT66      VPSS_TNR_CLUT66          ; /* 0x3e78 */
    volatile U_VPSS_TNR_CLUT67      VPSS_TNR_CLUT67          ; /* 0x3e7c */
    volatile unsigned int           reserved_26[96]                     ; /* 0x3e80~0x3ffc */
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
    volatile U_VPSS_DEBUG22         VPSS_DEBUG22             ; /* 0x4058 */
    volatile unsigned int           VPSS_DEBUG23             ; /* 0x405c */
    volatile unsigned int           VPSS_DEBUG24             ; /* 0x4060 */
    volatile unsigned int           VPSS_DEBUG25             ; /* 0x4064 */
    volatile unsigned int           VPSS_DEBUG26             ; /* 0x4068 */
    volatile U_VPSS_DEBUG27         VPSS_DEBUG27             ; /* 0x406c */
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
    volatile unsigned int           reserved_27[11220]                     ; /* 0x40b0~0xeffc */
#if 1
    volatile U_SMMU_SCR             VPSS_SMMU_SCR                 ; /* 0xf000 */
    volatile unsigned int           reserved_28                     ; /* 0xf004 */
    volatile U_SMMU_LP_CTRL         VPSS_SMMU_LP_CTRL             ; /* 0xf008 */
    volatile unsigned int           reserved_29                     ; /* 0xf00c */
    volatile U_SMMU_INTMASK_S       VPSS_SMMU_INTMASK_S           ; /* 0xf010 */
    volatile U_SMMU_INTRAW_S        VPSS_SMMU_INTRAW_S            ; /* 0xf014 */
    volatile U_SMMU_INTSTAT_S       VPSS_SMMU_INTSTAT_S           ; /* 0xf018 */
    volatile U_SMMU_INTCLR_S        VPSS_SMMU_INTCLR_S            ; /* 0xf01c */
    volatile U_SMMU_INTMASK_NS      VPSS_SMMU_INTMASK_NS          ; /* 0xf020 */
    volatile U_SMMU_INTRAW_NS       VPSS_SMMU_INTRAW_NS           ; /* 0xf024 */
    volatile U_SMMU_INTSTAT_NS      VPSS_SMMU_INTSTAT_NS          ; /* 0xf028 */
    volatile U_SMMU_INTCLR_NS       VPSS_SMMU_INTCLR_NS           ; /* 0xf02c */
    volatile unsigned int           reserved_30[118]                     ; /* 0xf030~0xf204 */
    volatile unsigned int           VPSS_SMMU_SCB_TTBR            ; /* 0xf208 */
    volatile unsigned int           VPSS_SMMU_CB_TTBR             ; /* 0xf20c */
    volatile unsigned int           reserved_31[56]                     ; /* 0xf210~0xf2ec */
    volatile unsigned int           VPSS_SMMU_ERR_RDADDR_S        ; /* 0xf2f0 */
    volatile unsigned int           VPSS_SMMU_ERR_WRADDR_S        ; /* 0xf2f4 */
    volatile unsigned int           reserved_32[3]                     ; /* 0xf2f8~0xf300 */
    volatile unsigned int           VPSS_SMMU_ERR_RDADDR_NS       ; /* 0xf304 */
    volatile unsigned int           VPSS_SMMU_ERR_WRADDR_NS       ; /* 0xf308 */
    volatile unsigned int           reserved_33                     ; /* 0xf30c */
    volatile unsigned int           VPSS_SMMU_FAULT_ADDR_PTW_S    ; /* 0xf310 */
    volatile U_SMMU_FAULT_ID_PTW_S   VPSS_SMMU_FAULT_ID_PTW_S     ; /* 0xf314 */
    volatile unsigned int           reserved_34[2]                     ; /* 0xf318~0xf31c */
    volatile unsigned int           VPSS_SMMU_FAULT_ADDR_PTW_NS   ; /* 0xf320 */
    volatile U_SMMU_FAULT_ID_PTW_NS   VPSS_SMMU_FAULT_ID_PTW_NS   ; /* 0xf324 */
    volatile U_SMMU_FAULT_PTW_NUM   VPSS_SMMU_FAULT_PTW_NUM       ; /* 0xf328 */
    volatile unsigned int           reserved_35                     ; /* 0xf32c */
    volatile unsigned int           VPSS_SMMU_FAULT_ADDR_WR_S     ; /* 0xf330 */
    volatile unsigned int           VPSS_SMMU_FAULT_TLB_WR_S      ; /* 0xf334 */
    volatile U_SMMU_FAULT_ID_WR_S   VPSS_SMMU_FAULT_ID_WR_S       ; /* 0xf338 */
    volatile unsigned int           reserved_36                     ; /* 0xf33c */
    volatile unsigned int           VPSS_SMMU_FAULT_ADDR_WR_NS    ; /* 0xf340 */
    volatile unsigned int           VPSS_SMMU_FAULT_TLB_WR_NS     ; /* 0xf344 */
    volatile U_SMMU_FAULT_ID_WR_NS   VPSS_SMMU_FAULT_ID_WR_NS     ; /* 0xf348 */
    volatile unsigned int           reserved_37                     ; /* 0xf34c */
    volatile unsigned int           VPSS_SMMU_FAULT_ADDR_RD_S     ; /* 0xf350 */
    volatile unsigned int           VPSS_SMMU_FAULT_TLB_RD_S      ; /* 0xf354 */
    volatile U_SMMU_FAULT_ID_RD_S   VPSS_SMMU_FAULT_ID_RD_S       ; /* 0xf358 */
    volatile unsigned int           reserved_38                     ; /* 0xf35c */
    volatile unsigned int           VPSS_SMMU_FAULT_ADDR_RD_NS    ; /* 0xf360 */
    volatile unsigned int           VPSS_SMMU_FAULT_TLB_RD_NS     ; /* 0xf364 */
    volatile U_SMMU_FAULT_ID_RD_NS   VPSS_SMMU_FAULT_ID_RD_NS     ; /* 0xf368 */
    volatile U_SMMU_FAULT_TBU_INFO   VPSS_SMMU_FAULT_TBU_INFO     ; /* 0xf36c */
    volatile U_SMMU_FAULT_TBU_DBG   VPSS_SMMU_FAULT_TBU_DBG       ; /* 0xf370 */
    volatile U_SMMU_PREF_BUFFER_EMPTY   VPSS_SMMU_PREF_BUFFER_EMPTY ; /* 0xf374 */
    volatile U_SMMU_PTWQ_IDLE       VPSS_SMMU_PTWQ_IDLE           ; /* 0xf378 */
    volatile U_SMMU_RESET_STATE     VPSS_SMMU_RESET_STATE         ; /* 0xf37c */
    volatile U_SMMU_MASTER_DBG0     VPSS_SMMU_MASTER_DBG0         ; /* 0xf380 */
    volatile U_SMMU_MASTER_DBG1     VPSS_SMMU_MASTER_DBG1         ; /* 0xf384 */
    volatile U_SMMU_MASTER_DBG2     VPSS_SMMU_MASTER_DBG2         ; /* 0xf388 */
    volatile U_SMMU_MASTER_DBG3     VPSS_SMMU_MASTER_DBG3         ; /* 0xf38c */
    volatile unsigned int           reserved_39[100]                     ; /* 0xf390~0xf51c */
    volatile U_SMMU_MASTER_DBG4     VPSS_SMMU_MASTER_DBG4         ; /* 0xf520 */
    volatile U_SMMU_MASTER_DBG5     VPSS_SMMU_MASTER_DBG5         ; /* 0xf524 */
    volatile U_SMMU_MASTER_DBG6     VPSS_SMMU_MASTER_DBG6         ; /* 0xf528 */
    volatile U_SMMU_MASTER_DBG7     VPSS_SMMU_MASTER_DBG7         ; /* 0xf52c */
    volatile U_SMMU_MASTER_DBG8     VPSS_SMMU_MASTER_DBG8         ; /* 0xf530 */
#endif
} VPSS_REG_S,S_VPSS_REGS_TYPE, S_CAS_REGS_TYPE;

#endif /* __HI_REG_VPSS_H__ */

