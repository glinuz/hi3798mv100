//******************************************************************************
// Copyright     :  Copyright (C) 2016, Hisilicon Technologies Co., Ltd.
// File name     :  hi_reg_vpss.h
// Author        :
// Version       :
// Date          :  2016/9/24
// Description   :
// Others        :  Generated automatically by nManager V4.1
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
        unsigned int    nx2_vc1_en            : 1   ; /* [10]  */
        unsigned int    rgme_en               : 1   ; /* [11]  */
        unsigned int    reserved_2            : 1   ; /* [12]  */
        unsigned int    blk_det_en            : 1   ; /* [13]  */
        unsigned int    reserved_3            : 1   ; /* [14]  */
        unsigned int    reserved_4            : 1   ; /* [15]  */
        unsigned int    reserved_5            : 1   ; /* [16]  */
        unsigned int    snr_en                : 1   ; /* [17]  */
        unsigned int    tnr_en                : 1   ; /* [18]  */
        unsigned int    rfr_en                : 1   ; /* [19]  */
        unsigned int    four_pix_en           : 1   ; /* [20]  */
        unsigned int    fmtc_en               : 1   ; /* [21]  */
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
        unsigned int    ram_bank              : 4   ; /* [3..0]  */
        unsigned int    ref_dither_en         : 1   ; /* [4]  */
        unsigned int    ref_dither_mode       : 1   ; /* [5]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
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
        unsigned int    reserved_1            : 1   ; /* [4]  */
        unsigned int    reserved_2            : 1   ; /* [5]  */
        unsigned int    reserved_3            : 1   ; /* [6]  */
        unsigned int    reserved_4            : 9   ; /* [15..7]  */
        unsigned int    reserved_5            : 1   ; /* [16]  */
        unsigned int    vhd0_crop_en          : 1   ; /* [17]  */
        unsigned int    reserved_6            : 2   ; /* [19..18]  */
        unsigned int    vhd0_lba_en           : 1   ; /* [20]  */
        unsigned int    reserved_7            : 1   ; /* [21]  */
        unsigned int    reserved_8            : 1   ; /* [22]  */
        unsigned int    reserved_9            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_CTRL3;

/* Define the union U_VPSS_CMD_CNT_CHN */
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

} U_VPSS_CMD_CNT_CHN;

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
        unsigned int    rch_trmd_bypass       : 1   ; /* [9]  */
        unsigned int    rch_srmd_bypass       : 1   ; /* [10]  */
        unsigned int    rch_rst_bypass        : 1   ; /* [11]  */
        unsigned int    rch_prjh_cur_bypass   : 1   ; /* [12]  */
        unsigned int    rch_prjv_cur_bypass   : 1   ; /* [13]  */
        unsigned int    rch_blkmv_ref_bypass  : 1   ; /* [14]  */
        unsigned int    rch_blkmv_cur_bypass  : 1   ; /* [15]  */
        unsigned int    rch_rgmv_cur_bypass   : 1   ; /* [16]  */
        unsigned int    rch_rgmv_nx1_bypass   : 1   ; /* [17]  */
        unsigned int    rch_blkmt_ref_bypass  : 1   ; /* [18]  */
        unsigned int    rch_reey_bypass       : 1   ; /* [19]  */
        unsigned int    rch_reec_bypass       : 1   ; /* [20]  */
        unsigned int    rch_tunl_bypass       : 1   ; /* [21]  */
        unsigned int    rch_nx2y_tile_2bit_bypass : 1   ; /* [22]  */
        unsigned int    rch_nx2c_tile_2bit_bypass : 1   ; /* [23]  */
        unsigned int    rch_transr_bypass     : 1   ; /* [24]  */
        unsigned int    rch_nx2y_head_bypass  : 1   ; /* [25]  */
        unsigned int    rch_nx2c_head_bypass  : 1   ; /* [26]  */
        unsigned int    rch_di_rcnt_bypass    : 1   ; /* [27]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
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
        unsigned int    wch_prjhnx2_bypass    : 1   ; /* [8]  */
        unsigned int    wch_prjvnx2_bypass    : 1   ; /* [9]  */
        unsigned int    wch_blkmvnx1_bypass   : 1   ; /* [10]  */
        unsigned int    wch_blkmtcur_bypass   : 1   ; /* [11]  */
        unsigned int    wch_rgmvnx2_bypass    : 1   ; /* [12]  */
        unsigned int    wch_cuey_bypass       : 1   ; /* [13]  */
        unsigned int    wch_cuec_bypass       : 1   ; /* [14]  */
        unsigned int    wch_transw_bypass     : 1   ; /* [15]  */
        unsigned int    wch_tunl_bypass       : 1   ; /* [16]  */
        unsigned int    wch_di_wcnt_bypass    : 1   ; /* [17]  */
        unsigned int    reserved_1            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_WCH_BYPASS;

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
/* Define the union U_VPSS_SNR_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int snr_cfg_addr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SNR_ADDR;
/* Define the union U_VPSS_ZME_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int zme_cfg_addr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_ZME_ADDR;
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
/* Define the union U_VPSS_FMTC_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fmtc_cfg_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_FMTC_ADDR;
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
        unsigned int    bus_w_err_state       : 1   ; /* [2]  */
        unsigned int    eof_end_state         : 1   ; /* [3]  */
        unsigned int    vhd0_tunl_state       : 1   ; /* [4]  */
        unsigned int    dcmp_err_state        : 1   ; /* [5]  */
        unsigned int    bus_r_err_state       : 1   ; /* [6]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    reserved_1            : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9]  */
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
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    reserved_1            : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9]  */
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
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    reserved_1            : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9]  */
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
        unsigned int    burst_len_cfg         : 2   ; /* [27..26]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MISCELLANEOUS;

/* Define the union U_VPSS_MACCFG */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int mac_ch_prio            : 32  ; /* [31..0]  */
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
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    reserved_1            : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_INTMASK;

/* Define the union U_VPSS_MEMCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    emab                  : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    emaa                  : 3   ; /* [6..4]  */
        unsigned int    emasa                 : 1   ; /* [7]  */
        unsigned int    emaw                  : 2   ; /* [9..8]  */
        unsigned int    reserved_1            : 2   ; /* [11..10]  */
        unsigned int    ema                   : 3   ; /* [14..12]  */
        unsigned int    reserved_2            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MEMCTRL;

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
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_VERSION;
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

/* Define the union U_VPSS_IN_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    in_format             : 5   ; /* [20..16]  */
        unsigned int    in_pix_bitw           : 1   ; /* [21]  */
        unsigned int    ref_nxt_pix_bitw      : 1   ; /* [22]  */
        unsigned int    rfr_pix_bitw          : 1   ; /* [23]  */
        unsigned int    uv_invert             : 1   ; /* [24]  */
        unsigned int    snr_mad_upfir_en      : 1   ; /* [25]  */
        unsigned int    cts_en                : 1   ; /* [26]  */
        unsigned int    cts_bit_sel           : 1   ; /* [27]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_IN_CTRL;

/* Define the union U_VPSS_NXT2_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 3   ; /* [2..0]  */
        unsigned int    nxt2_tile_spec_field  : 1   ; /* [3]  */
        unsigned int    nxt2_tile_format      : 1   ; /* [4]  */
        unsigned int    nxt2_hor_offset       : 13  ; /* [17..5]  */
        unsigned int    nxt2_ver_offset       : 13  ; /* [30..18]  */
        unsigned int    nxt2_dcmp_en          : 1   ; /* [31]  */
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

/* Define the union U_VPSS_NX2Y_HEAD_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nx2y_top_head_addr     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NX2Y_HEAD_ADDR;
/* Define the union U_VPSS_NX2Y_BOT_HEAD_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nx2y_bot_head_addr     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NX2Y_BOT_HEAD_ADDR;
/* Define the union U_VPSS_NX2C_HEAD_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nx2c_top_head_addr     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NX2C_HEAD_ADDR;
/* Define the union U_VPSS_NX2C_BOT_HEAD_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nx2c_bot_head_addr     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NX2C_BOT_HEAD_ADDR;
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

/* Define the union U_VPSS_NX2_TUNL_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nx2_tunl_en           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 7   ; /* [7..1]  */
        unsigned int    nx2_tunl_rd_interval  : 8   ; /* [15..8]  */
        unsigned int    reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NX2_TUNL_CTRL;

/* Define the union U_VPSS_NX2_TUNL_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nx2_tunl_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NX2_TUNL_ADDR;
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

/* Define the union U_VPSS_REE_CTRL */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_REE_CTRL;
/* Define the union U_VPSS_REEYADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reey_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_REEYADDR;
/* Define the union U_VPSS_REECADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reec_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_REECADDR;
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

/* Define the union U_VPSS_PRJVCURADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int prjv_cur_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PRJVCURADDR;
/* Define the union U_VPSS_PRJHCURADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int prjh_cur_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PRJHCURADDR;
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

/* Define the union U_VPSS_RGMVCURADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int rgmv_cur_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_RGMVCURADDR;
/* Define the union U_VPSS_RGMVNX1ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int rgmv_nx1_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_RGMVNX1ADDR;
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

/* Define the union U_VPSS_BLKMVCURADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int blkmv_cur_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_BLKMVCURADDR;
/* Define the union U_VPSS_BLKMVREFADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int blkmv_ref_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_BLKMVREFADDR;
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

/* Define the union U_VPSS_BLKMTREFADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int blkmt_ref_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_BLKMTREFADDR;
/* Define the union U_VPSS_BLKMTREFSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blkmt_ref_stride      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_BLKMTREFSTRIDE;

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
/* Define the union U_VPSS_SNR_MAD_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int snr_mad_raddr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SNR_MAD_RADDR;
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

/* Define the union U_VPSS_TRANS_SRC_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int trans_src_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_TRANS_SRC_ADDR;
/* Define the union U_VPSS_TRANS_DES_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int trans_des_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_TRANS_DES_ADDR;
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

/* Define the union U_VPSS_VHD0CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0_format           : 4   ; /* [3..0]  */
        unsigned int    vhd0_mirror           : 1   ; /* [4]  */
        unsigned int    vhd0_flip             : 1   ; /* [5]  */
        unsigned int    vhd0_pix_bitw         : 1   ; /* [6]  */
        unsigned int    vhd0_dither_en        : 1   ; /* [7]  */
        unsigned int    vhd0_uv_invert        : 1   ; /* [8]  */
        unsigned int    vhd0_lcmp_en          : 1   ; /* [9]  */
        unsigned int    vhd0_dither_mode      : 1   ; /* [10]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
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

/* Define the union U_VPSS_CUE_CTRL */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_CUE_CTRL;
/* Define the union U_VPSS_CUEYADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int cuey_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_CUEYADDR;
/* Define the union U_VPSS_CUECADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int cuec_addr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_CUECADDR;
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

/* Define the union U_VPSS_PRJVNX2ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int prjv_nx2_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PRJVNX2ADDR;
/* Define the union U_VPSS_PRJHNX2ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int prjh_nx2_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_PRJHNX2ADDR;
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

/* Define the union U_VPSS_BLKMVNX1ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int blkmv_nx1_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_BLKMVNX1ADDR;
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

/* Define the union U_VPSS_RGMVNX2ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int rgmv_nx2_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_RGMVNX2ADDR;
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

/* Define the union U_VPSS_BLKMTCURADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int blkmt_cur_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_BLKMTCURADDR;
/* Define the union U_VPSS_BLKMTCURSTRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blkmt_cur_stride      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_BLKMTCURSTRIDE;

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
/* Define the union U_VPSS_NX2Y_LB_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nx2ylb_addr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NX2Y_LB_ADDR;
/* Define the union U_VPSS_NX2C_LB_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int nx2clb_addr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_NX2C_LB_ADDR;
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

/* Define the union U_VPSS_VHD0_TUNL_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhd0_tunl_finish_line : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vhd0_tunl_mode        : 2   ; /* [17..16]  */
        unsigned int    vhd0_tunl_en          : 1   ; /* [18]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0_TUNL_CTRL;

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
/* Define the union U_VPSS_DICNT_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    di_cnt_width          : 16  ; /* [15..0]  */
        unsigned int    di_cnt_height         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DICNT_SIZE;

/* Define the union U_VPSS_DIRCNT_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int di_rcnt_addr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIRCNT_ADDR;
/* Define the union U_VPSS_DIWCNT_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int di_wcnt_addr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DIWCNT_ADDR;
/* Define the union U_VPSS_DICNT_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    di_rcnt_stride        : 16  ; /* [15..0]  */
        unsigned int    di_wcnt_stride        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DICNT_STRIDE;

/* Define the union U_VPSS_RTT_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rotate_angle          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_RTT_CFG;

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
        unsigned int    vhd0_vbk_y            : 10  ; /* [9..0]  */
        unsigned int    vhd0_vbk_cb           : 10  ; /* [19..10]  */
        unsigned int    vhd0_vbk_cr           : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_VHD0LBA_BK;

/* Define the union U_VPSS_NX2_VC1_CTRL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nx2_vc1_profile       : 2   ; /* [1..0]  */
        unsigned int    nx2_vc1_rangedfrm     : 1   ; /* [2]  */
        unsigned int    nx2_vc1_mapyflg       : 1   ; /* [3]  */
        unsigned int    nx2_vc1_mapcflg       : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    nx2_vc1_mapy          : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    nx2_vc1_mapc          : 3   ; /* [14..12]  */
        unsigned int    reserved_2            : 16  ; /* [30..15]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_NX2_VC1_CTRL0;

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
        unsigned int    edge_coring           : 10  ; /* [9..0]  */
        unsigned int    edge_scale            : 10  ; /* [19..10]  */
        unsigned int    inter_diff_thd2       : 12  ; /* [31..20]  */
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

/* Define the union U_VPSS_DIEKMAG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    kmagh_1               : 7   ; /* [6..0]  */
        unsigned int    kmagh_2               : 7   ; /* [13..7]  */
        unsigned int    kmagv_1               : 5   ; /* [18..14]  */
        unsigned int    kmagv_2               : 5   ; /* [23..19]  */
        unsigned int    khoredge              : 5   ; /* [28..24]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEKMAG1;

/* Define the union U_VPSS_DIEKMAG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scaler_horedge        : 3   ; /* [2..0]  */
        unsigned int    frame_mag_en          : 1   ; /* [3]  */
        unsigned int    motion_limt_1         : 8   ; /* [11..4]  */
        unsigned int    motion_limt_2         : 8   ; /* [19..12]  */
        unsigned int    scaler_framemotion    : 3   ; /* [22..20]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIEKMAG2;

/* Define the union U_VPSS_DIE_S3ADD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dir_ratio_c           : 4   ; /* [3..0]  */
        unsigned int    edge_str_coring_c     : 5   ; /* [8..4]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DIE_S3ADD;

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
        unsigned int    k0_hvdif_dw           : 4   ; /* [12..9]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
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

/* Define the union U_VPSS_MCDI_S3ADD0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    core_rglsw            : 3   ; /* [2..0]  */
        unsigned int    k0_hfvline            : 3   ; /* [5..3]  */
        unsigned int    k1_hfvline            : 3   ; /* [8..6]  */
        unsigned int    core_hfvline          : 6   ; /* [14..9]  */
        unsigned int    k0_hw                 : 6   ; /* [20..15]  */
        unsigned int    k1_hw                 : 6   ; /* [26..21]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_S3ADD0;

/* Define the union U_VPSS_MCDI_S3ADD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_hw                  : 7   ; /* [6..0]  */
        unsigned int    b_core0_hw            : 4   ; /* [10..7]  */
        unsigned int    k_core0_hw            : 4   ; /* [14..11]  */
        unsigned int    core1_hw              : 4   ; /* [18..15]  */
        unsigned int    subpix_mc_en          : 1   ; /* [19]  */
        unsigned int    g_difcore_mcw         : 9   ; /* [28..20]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_S3ADD1;

/* Define the union U_VPSS_MCDI_S3ADD2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rp_k1_mag_wnd_mcw     : 4   ; /* [3..0]  */
        unsigned int    rp_k1_sad_wnd_mcw     : 5   ; /* [8..4]  */
        unsigned int    x0_sadr_wnd_mcw       : 7   ; /* [15..9]  */
        unsigned int    k0_sadr_wnd_mcw       : 4   ; /* [19..16]  */
        unsigned int    g0_sadr_wnd_mcw       : 8   ; /* [27..20]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_S3ADD2;

/* Define the union U_VPSS_MCDI_S3ADD3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    k1_sadr_wnd_mcw       : 4   ; /* [3..0]  */
        unsigned int    g1_sadr_wnd_mcw       : 9   ; /* [12..4]  */
        unsigned int    k_p1cfdifh_mcw        : 4   ; /* [16..13]  */
        unsigned int    k_mcdifv_mcw          : 4   ; /* [20..17]  */
        unsigned int    th_cur_blksad         : 9   ; /* [29..21]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_S3ADD3;

/* Define the union U_VPSS_MCDI_S3ADD4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    thh_neigh_blksad      : 9   ; /* [8..0]  */
        unsigned int    thl_neigh_blksad      : 9   ; /* [17..9]  */
        unsigned int    th_cur_blkmotion      : 5   ; /* [22..18]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_S3ADD4;

/* Define the union U_VPSS_MCDI_S3ADD5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    th_rgmv_mag           : 10  ; /* [9..0]  */
        unsigned int    blkmv_update_en       : 1   ; /* [10]  */
        unsigned int    k_rpcounter           : 3   ; /* [13..11]  */
        unsigned int    rpcounterth           : 3   ; /* [16..14]  */
        unsigned int    rp_mvxth_rgmv         : 3   ; /* [19..17]  */
        unsigned int    rp_difmvxth_rgmv      : 4   ; /* [23..20]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_S3ADD5;

/* Define the union U_VPSS_MCDI_S3ADD6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rp_difsadth_rgmv      : 8   ; /* [7..0]  */
        unsigned int    rp_sadth_rgmv         : 9   ; /* [16..8]  */
        unsigned int    rp_magth_rgmv         : 10  ; /* [26..17]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_S3ADD6;

/* Define the union U_VPSS_MCDI_S3ADD7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rp_difmvxth_sp        : 3   ; /* [2..0]  */
        unsigned int    rp_difsadth_tb        : 9   ; /* [11..3]  */
        unsigned int    k1_tpdif_rgsad        : 6   ; /* [17..12]  */
        unsigned int    submv_sadchk_en       : 1   ; /* [18]  */
        unsigned int    rp_en                 : 1   ; /* [19]  */
        unsigned int    difvt_mode            : 1   ; /* [20]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_S3ADD7;

/* Define the union U_VPSS_MCDI_STILLCNT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cntlut_0              : 5   ; /* [4..0]  */
        unsigned int    cntlut_1              : 5   ; /* [9..5]  */
        unsigned int    cntlut_2              : 5   ; /* [14..10]  */
        unsigned int    cntlut_3              : 5   ; /* [19..15]  */
        unsigned int    cntlut_4              : 5   ; /* [24..20]  */
        unsigned int    cntlut_5              : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_STILLCNT0;

/* Define the union U_VPSS_MCDI_STILLCNT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cntlut_6              : 5   ; /* [4..0]  */
        unsigned int    cntlut_7              : 5   ; /* [9..5]  */
        unsigned int    cntlut_8              : 5   ; /* [14..10]  */
        unsigned int    mc_mtshift            : 1   ; /* [15]  */
        unsigned int    mcw_scnt_en           : 1   ; /* [16]  */
        unsigned int    mcw_scnt_gain         : 9   ; /* [25..17]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_MCDI_STILLCNT1;

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
        unsigned int    reserved_0            : 21  ; /* [20..0]  */
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

/* Define the union U_VPSS_SNR_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_en                 : 1   ; /* [0]  */
        unsigned int    dm_en                 : 1   ; /* [1]  */
        unsigned int    snr_nr_en             : 1   ; /* [2]  */
        unsigned int    db_lum_hor_en         : 1   ; /* [3]  */
        unsigned int    reserved_0            : 5   ; /* [8..4]  */
        unsigned int    dbm_demo_en           : 1   ; /* [9]  */
        unsigned int    dbm_demo_mode         : 1   ; /* [10]  */
        unsigned int    reserved_1            : 2   ; /* [12..11]  */
        unsigned int    det_hy_en             : 1   ; /* [13]  */
        unsigned int    mad_mode              : 2   ; /* [15..14]  */
        unsigned int    dbm_out_mode          : 3   ; /* [18..16]  */
        unsigned int    reserved_2            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_CTRL;

/* Define the union U_VPSS_DBM_DEMO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbm_demo_pos_x        : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBM_DEMO;

/* Define the union U_VPSS_DB_DIR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_grad_sub_ratio     : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    db_ctrst_thresh       : 5   ; /* [12..8]  */
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
        unsigned int    db_lum_h_blk_size     : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
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
        unsigned int    db_lum_hor_scale_ratio : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 6   ; /* [7..2]  */
        unsigned int    db_lum_hor_filter_sel : 2   ; /* [9..8]  */
        unsigned int    reserved_1            : 2   ; /* [11..10]  */
        unsigned int    db_global_db_strenth_lum : 4   ; /* [15..12]  */
        unsigned int    reserved_2            : 4   ; /* [19..16]  */
        unsigned int    db_lum_hor_txt_win_size : 2   ; /* [21..20]  */
        unsigned int    reserved_3            : 10  ; /* [31..22]  */
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
        unsigned int    db_lum_hor_hf_diff_core : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    db_lum_hor_hf_diff_gain1 : 4   ; /* [11..8]  */
        unsigned int    db_lum_hor_hf_diff_gain2 : 4   ; /* [15..12]  */
        unsigned int    db_lum_hor_hf_var_core : 7   ; /* [22..16]  */
        unsigned int    reserved_1            : 1   ; /* [23]  */
        unsigned int    db_lum_hor_hf_var_gain1 : 4   ; /* [27..24]  */
        unsigned int    db_lum_hor_hf_var_gain2 : 4   ; /* [31..28]  */
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
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    db_lum_hor_adj_gain   : 5   ; /* [20..16]  */
        unsigned int    db_lum_hor_bord_adj_gain : 3   ; /* [23..21]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
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
        unsigned int    db_ctrst_adj_core     : 5   ; /* [4..0]  */
        unsigned int    db_dir_smooth_mode    : 2   ; /* [6..5]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    db_ctrst_adj_gain1    : 4   ; /* [11..8]  */
        unsigned int    db_ctrst_adj_gain2    : 4   ; /* [15..12]  */
        unsigned int    db_max_lum_hor_db_dist : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
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

} U_VPSS_DB_LUT0;

/* Define the union U_VPSS_DB_LUT1 */
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

} U_VPSS_DB_LUT1;

/* Define the union U_VPSS_DB_LUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_lum_h_str_fade_lut_p0 : 3   ; /* [2..0]  */
        unsigned int    db_lum_h_str_fade_lut_p1 : 3   ; /* [5..3]  */
        unsigned int    db_lum_h_str_fade_lut_p2 : 3   ; /* [8..6]  */
        unsigned int    db_lum_h_str_fade_lut_p3 : 3   ; /* [11..9]  */
        unsigned int    db_lum_h_str_fade_lut_p4 : 3   ; /* [14..12]  */
        unsigned int    db_lum_h_str_fade_lut_p5 : 3   ; /* [17..15]  */
        unsigned int    db_lum_h_str_fade_lut_p6 : 3   ; /* [20..18]  */
        unsigned int    db_lum_h_str_fade_lut_p7 : 3   ; /* [23..21]  */
        unsigned int    db_lum_h_str_fade_lut_p8 : 3   ; /* [26..24]  */
        unsigned int    db_lum_h_str_fade_lut_p9 : 3   ; /* [29..27]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
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
        unsigned int    db_lum_h_str_fade_lut_p10 : 3   ; /* [2..0]  */
        unsigned int    db_lum_h_str_fade_lut_p11 : 3   ; /* [5..3]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
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
        unsigned int    db_dir_str_gain_lut_p0 : 4   ; /* [3..0]  */
        unsigned int    db_dir_str_gain_lut_p1 : 4   ; /* [7..4]  */
        unsigned int    db_dir_str_gain_lut_p2 : 4   ; /* [11..8]  */
        unsigned int    db_dir_str_gain_lut_p3 : 4   ; /* [15..12]  */
        unsigned int    db_dir_str_gain_lut_p4 : 4   ; /* [19..16]  */
        unsigned int    db_dir_str_gain_lut_p5 : 4   ; /* [23..20]  */
        unsigned int    db_dir_str_gain_lut_p6 : 4   ; /* [27..24]  */
        unsigned int    db_dir_str_gain_lut_p7 : 4   ; /* [31..28]  */
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
        unsigned int    db_dir_str_lut_p0     : 4   ; /* [3..0]  */
        unsigned int    db_dir_str_lut_p1     : 4   ; /* [7..4]  */
        unsigned int    db_dir_str_lut_p2     : 4   ; /* [11..8]  */
        unsigned int    db_dir_str_lut_p3     : 4   ; /* [15..12]  */
        unsigned int    db_dir_str_lut_p4     : 4   ; /* [19..16]  */
        unsigned int    db_dir_str_lut_p5     : 4   ; /* [23..20]  */
        unsigned int    db_dir_str_lut_p6     : 4   ; /* [27..24]  */
        unsigned int    db_dir_str_lut_p7     : 4   ; /* [31..28]  */
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
        unsigned int    db_dir_str_lut_p8     : 4   ; /* [3..0]  */
        unsigned int    db_dir_str_lut_p9     : 4   ; /* [7..4]  */
        unsigned int    db_dir_str_lut_p10    : 4   ; /* [11..8]  */
        unsigned int    db_dir_str_lut_p11    : 4   ; /* [15..12]  */
        unsigned int    db_dir_str_lut_p12    : 4   ; /* [19..16]  */
        unsigned int    db_dir_str_lut_p13    : 4   ; /* [23..20]  */
        unsigned int    db_dir_str_lut_p14    : 4   ; /* [27..24]  */
        unsigned int    db_dir_str_lut_p15    : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_LUT6;

/* Define the union U_VPSS_DB_ADPT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_adpt_flt_mode      : 2   ; /* [1..0]  */
        unsigned int    db_adpt_delta_adj0    : 5   ; /* [6..2]  */
        unsigned int    reserved_0            : 5   ; /* [11..7]  */
        unsigned int    db_adpt_delta_scl     : 17  ; /* [28..12]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DB_ADPT;

/* Define the union U_VPSS_DBD_THDEDGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbd_hy_thd_edge       : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
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
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
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
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
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
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
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

/* Define the union U_VPSS_DBD_BLKSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbd_min_blk_size      : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    dbd_hy_max_blk_size   : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_BLKSIZE;

/* Define the union U_VPSS_DBD_FLAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbd_thr_flat          : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DBD_FLAT;

/* Define the union U_VPSS_DM_DIR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 5   ; /* [4..0]  */
        unsigned int    reserved_1            : 1   ; /* [5]  */
        unsigned int    dm_ctrst_thresh       : 2   ; /* [7..6]  */
        unsigned int    reserved_2            : 8   ; /* [15..8]  */
        unsigned int    reserved_3            : 2   ; /* [17..16]  */
        unsigned int    reserved_4            : 14  ; /* [31..18]  */
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
        unsigned int    dm_mmf_set            : 1   ; /* [0]  */
        unsigned int    dm_init_val_step      : 2   ; /* [2..1]  */
        unsigned int    reserved_0            : 19  ; /* [21..3]  */
        unsigned int    dm_global_str         : 4   ; /* [25..22]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
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
        unsigned int    reserved_0            : 18  ; /* [17..0]  */
        unsigned int    reserved_1            : 8   ; /* [25..18]  */
        unsigned int    reserved_2            : 5   ; /* [30..26]  */
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
        unsigned int    reserved_0            : 22  ; /* [21..0]  */
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
        unsigned int    reserved_0            : 1   ; /* [26]  */
        unsigned int    dm_lsw_ratio          : 3   ; /* [29..27]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
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

/* Define the union U_VPSS_DM_LUT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_trans_band_lut_p0  : 6   ; /* [5..0]  */
        unsigned int    dm_trans_band_lut_p1  : 6   ; /* [11..6]  */
        unsigned int    dm_trans_band_lut_p2  : 6   ; /* [17..12]  */
        unsigned int    dm_trans_band_lut_p3  : 6   ; /* [23..18]  */
        unsigned int    dm_trans_band_lut_p4  : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT5;

/* Define the union U_VPSS_DM_LUT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_trans_band_lut_p5  : 6   ; /* [5..0]  */
        unsigned int    dm_trans_band_lut_p6  : 6   ; /* [11..6]  */
        unsigned int    dm_trans_band_lut_p7  : 6   ; /* [17..12]  */
        unsigned int    dm_trans_band_lut_p8  : 6   ; /* [23..18]  */
        unsigned int    dm_trans_band_lut_p9  : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT6;

/* Define the union U_VPSS_DM_LUT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_trans_band_lut_p10 : 6   ; /* [5..0]  */
        unsigned int    dm_trans_band_lut_p11 : 6   ; /* [11..6]  */
        unsigned int    dm_trans_band_lut_p12 : 6   ; /* [17..12]  */
        unsigned int    dm_trans_band_lut_p13 : 6   ; /* [23..18]  */
        unsigned int    dm_trans_band_lut_p14 : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT7;

/* Define the union U_VPSS_DM_LUT8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_trans_band_lut_p15 : 6   ; /* [5..0]  */
        unsigned int    dm_trans_band_lut_p16 : 6   ; /* [11..6]  */
        unsigned int    dm_trans_band_lut_p17 : 6   ; /* [17..12]  */
        unsigned int    dm_trans_band_lut_p18 : 6   ; /* [23..18]  */
        unsigned int    dm_trans_band_lut_p19 : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT8;

/* Define the union U_VPSS_DM_LUT9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_trans_band_lut_p20 : 6   ; /* [5..0]  */
        unsigned int    dm_trans_band_lut_p21 : 6   ; /* [11..6]  */
        unsigned int    dm_trans_band_lut_p22 : 6   ; /* [17..12]  */
        unsigned int    dm_trans_band_lut_p23 : 6   ; /* [23..18]  */
        unsigned int    dm_trans_band_lut_p24 : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT9;

/* Define the union U_VPSS_DM_LUT10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_trans_band_lut_p25 : 6   ; /* [5..0]  */
        unsigned int    dm_trans_band_lut_p26 : 6   ; /* [11..6]  */
        unsigned int    dm_trans_band_lut_p27 : 6   ; /* [17..12]  */
        unsigned int    dm_trans_band_lut_p28 : 6   ; /* [23..18]  */
        unsigned int    dm_trans_band_lut_p29 : 6   ; /* [29..24]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT10;

/* Define the union U_VPSS_DM_LUT11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_trans_band_lut_p30 : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT11;

/* Define the union U_VPSS_DM_LUT12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_dir_str_lut_p16    : 4   ; /* [3..0]  */
        unsigned int    dm_dir_str_lut_p17    : 4   ; /* [7..4]  */
        unsigned int    dm_dir_str_lut_p18    : 4   ; /* [11..8]  */
        unsigned int    dm_dir_str_lut_p19    : 4   ; /* [15..12]  */
        unsigned int    dm_dir_str_lut_p20    : 4   ; /* [19..16]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT12;

/* Define the union U_VPSS_DM_LUT13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_ang_ctrst_pun_lutp0 : 5   ; /* [4..0]  */
        unsigned int    dm_ang_ctrst_pun_lutp1 : 5   ; /* [9..5]  */
        unsigned int    dm_ang_ctrst_pun_lutp2 : 5   ; /* [14..10]  */
        unsigned int    dm_ang_ctrst_pun_lutp3 : 5   ; /* [19..15]  */
        unsigned int    dm_ang_ctrst_pun_lutp4 : 5   ; /* [24..20]  */
        unsigned int    dm_ang_ctrst_pun_lutp5 : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT13;

/* Define the union U_VPSS_DM_LUT14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_ang_ctrst_pun_lutp6 : 5   ; /* [4..0]  */
        unsigned int    dm_ang_ctrst_pun_lutp7 : 5   ; /* [9..5]  */
        unsigned int    dm_ang_ctrst_pun_lutp8 : 5   ; /* [14..10]  */
        unsigned int    dm_ang_ctrst_pun_lutp9 : 5   ; /* [19..15]  */
        unsigned int    dm_ang_ctrst_pun_lutp10 : 5   ; /* [24..20]  */
        unsigned int    dm_ang_ctrst_pun_lutp11 : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT14;

/* Define the union U_VPSS_DM_LUT15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_ang_ctrst_pun_lutp12 : 5   ; /* [4..0]  */
        unsigned int    dm_ang_ctrst_pun_lutp13 : 5   ; /* [9..5]  */
        unsigned int    dm_ang_ctrst_pun_lutp14 : 5   ; /* [14..10]  */
        unsigned int    dm_ang_ctrst_pun_lutp15 : 5   ; /* [19..15]  */
        unsigned int    dm_ang_ctrst_pun_lutp16 : 5   ; /* [24..20]  */
        unsigned int    dm_ang_ctrst_pun_lutp17 : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT15;

/* Define the union U_VPSS_DM_LUT16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_ang_ctrst_pun_lutp18 : 5   ; /* [4..0]  */
        unsigned int    dm_ang_ctrst_pun_lutp19 : 5   ; /* [9..5]  */
        unsigned int    dm_ang_ctrst_pun_lutp20 : 5   ; /* [14..10]  */
        unsigned int    dm_ang_ctrst_pun_lutp21 : 5   ; /* [19..15]  */
        unsigned int    dm_ang_ctrst_pun_lutp22 : 5   ; /* [24..20]  */
        unsigned int    dm_ang_ctrst_pun_lutp23 : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT16;

/* Define the union U_VPSS_DM_LUT17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_ang_ctrst_pun_lutp24 : 5   ; /* [4..0]  */
        unsigned int    dm_ang_ctrst_pun_lutp25 : 5   ; /* [9..5]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_LUT17;

/* Define the union U_VPSS_DM_TRANS_WID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dm_trans_wid          : 8   ; /* [7..0]  */
        unsigned int    dm_trans_wid_10       : 10  ; /* [17..8]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_TRANS_WID;

/* Define the union U_VPSS_DB_BORD_FLAG0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_0    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG0;
/* Define the union U_VPSS_DB_BORD_FLAG1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_1    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG1;
/* Define the union U_VPSS_DB_BORD_FLAG2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_2    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG2;
/* Define the union U_VPSS_DB_BORD_FLAG3 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_3    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG3;
/* Define the union U_VPSS_DB_BORD_FLAG4 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_4    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG4;
/* Define the union U_VPSS_DB_BORD_FLAG5 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_5    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG5;
/* Define the union U_VPSS_DB_BORD_FLAG6 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_6    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG6;
/* Define the union U_VPSS_DB_BORD_FLAG7 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_7    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG7;
/* Define the union U_VPSS_DB_BORD_FLAG8 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_8    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG8;
/* Define the union U_VPSS_DB_BORD_FLAG9 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_9    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG9;
/* Define the union U_VPSS_DB_BORD_FLAG10 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_10   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG10;
/* Define the union U_VPSS_DB_BORD_FLAG11 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_11   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG11;
/* Define the union U_VPSS_DB_BORD_FLAG12 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_12   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG12;
/* Define the union U_VPSS_DB_BORD_FLAG13 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_13   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG13;
/* Define the union U_VPSS_DB_BORD_FLAG14 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_14   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG14;
/* Define the union U_VPSS_DB_BORD_FLAG15 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_15   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG15;
/* Define the union U_VPSS_DB_BORD_FLAG16 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_16   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG16;
/* Define the union U_VPSS_DB_BORD_FLAG17 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_17   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG17;
/* Define the union U_VPSS_DB_BORD_FLAG18 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_18   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG18;
/* Define the union U_VPSS_DB_BORD_FLAG19 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_19   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG19;
/* Define the union U_VPSS_DB_BORD_FLAG20 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_20   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG20;
/* Define the union U_VPSS_DB_BORD_FLAG21 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_21   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG21;
/* Define the union U_VPSS_DB_BORD_FLAG22 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_22   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG22;
/* Define the union U_VPSS_DB_BORD_FLAG23 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_23   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG23;
/* Define the union U_VPSS_DB_BORD_FLAG24 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_24   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG24;
/* Define the union U_VPSS_DB_BORD_FLAG25 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_25   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG25;
/* Define the union U_VPSS_DB_BORD_FLAG26 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_26   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG26;
/* Define the union U_VPSS_DB_BORD_FLAG27 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_27   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG27;
/* Define the union U_VPSS_DB_BORD_FLAG28 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_28   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG28;
/* Define the union U_VPSS_DB_BORD_FLAG29 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_29   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG29;
/* Define the union U_VPSS_DB_BORD_FLAG30 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_30   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG30;
/* Define the union U_VPSS_DB_BORD_FLAG31 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_31   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG31;
/* Define the union U_VPSS_DB_BORD_FLAG32 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_32   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG32;
/* Define the union U_VPSS_DB_BORD_FLAG33 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_33   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG33;
/* Define the union U_VPSS_DB_BORD_FLAG34 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_34   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG34;
/* Define the union U_VPSS_DB_BORD_FLAG35 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_35   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG35;
/* Define the union U_VPSS_DB_BORD_FLAG36 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_36   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG36;
/* Define the union U_VPSS_DB_BORD_FLAG37 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_37   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG37;
/* Define the union U_VPSS_DB_BORD_FLAG38 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_38   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG38;
/* Define the union U_VPSS_DB_BORD_FLAG39 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_39   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG39;
/* Define the union U_VPSS_DB_BORD_FLAG40 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_40   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG40;
/* Define the union U_VPSS_DB_BORD_FLAG41 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_41   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG41;
/* Define the union U_VPSS_DB_BORD_FLAG42 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_42   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG42;
/* Define the union U_VPSS_DB_BORD_FLAG43 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_43   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG43;
/* Define the union U_VPSS_DB_BORD_FLAG44 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_44   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG44;
/* Define the union U_VPSS_DB_BORD_FLAG45 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_45   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG45;
/* Define the union U_VPSS_DB_BORD_FLAG46 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_46   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG46;
/* Define the union U_VPSS_DB_BORD_FLAG47 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_47   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG47;
/* Define the union U_VPSS_DB_BORD_FLAG48 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_48   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG48;
/* Define the union U_VPSS_DB_BORD_FLAG49 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_49   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG49;
/* Define the union U_VPSS_DB_BORD_FLAG50 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_50   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG50;
/* Define the union U_VPSS_DB_BORD_FLAG51 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_51   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG51;
/* Define the union U_VPSS_DB_BORD_FLAG52 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_52   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG52;
/* Define the union U_VPSS_DB_BORD_FLAG53 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_53   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG53;
/* Define the union U_VPSS_DB_BORD_FLAG54 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_54   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG54;
/* Define the union U_VPSS_DB_BORD_FLAG55 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_55   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG55;
/* Define the union U_VPSS_DB_BORD_FLAG56 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_56   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG56;
/* Define the union U_VPSS_DB_BORD_FLAG57 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_57   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG57;
/* Define the union U_VPSS_DB_BORD_FLAG58 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_58   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG58;
/* Define the union U_VPSS_DB_BORD_FLAG59 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_59   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG59;
/* Define the union U_VPSS_DB_BORD_FLAG60 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_60   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG60;
/* Define the union U_VPSS_DB_BORD_FLAG61 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_61   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG61;
/* Define the union U_VPSS_DB_BORD_FLAG62 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_62   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG62;
/* Define the union U_VPSS_DB_BORD_FLAG63 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int vpss_db_bord_flag_63   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_DB_BORD_FLAG63;
/* Define the union U_VPSS_SNR_ENABLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    ynr2den               : 1   ; /* [1]  */
        unsigned int    cnr2den               : 1   ; /* [2]  */
        unsigned int    motionedgeweighten    : 1   ; /* [3]  */
        unsigned int    colorweighten         : 1   ; /* [4]  */
        unsigned int    edgeprefilteren       : 1   ; /* [5]  */
        unsigned int    reserved_1            : 1   ; /* [6]  */
        unsigned int    test_en               : 1   ; /* [7]  */
        unsigned int    reserved_2            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_ENABLE;

/* Define the union U_VPSS_SNR_EDGE_RATIORANGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgemaxratio          : 4   ; /* [3..0]  */
        unsigned int    edgeminratio          : 4   ; /* [7..4]  */
        unsigned int    edgeoriratio          : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_RATIORANGE;

/* Define the union U_VPSS_SNR_EDGE_STRRANGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgemaxstrength       : 4   ; /* [3..0]  */
        unsigned int    edgeminstrength       : 4   ; /* [7..4]  */
        unsigned int    edgeoristrength       : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_STRRANGE;

/* Define the union U_VPSS_SNR_EDGE_STRTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgestrth1            : 10  ; /* [9..0]  */
        unsigned int    edgestrth2            : 10  ; /* [19..10]  */
        unsigned int    edgestrth3            : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_STRTH;

/* Define the union U_VPSS_SNR_EDGE_STRK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgestrk1             : 6   ; /* [5..0]  */
        unsigned int    edgestrk2             : 6   ; /* [11..6]  */
        unsigned int    edgestrk3             : 6   ; /* [17..12]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_STRK;

/* Define the union U_VPSS_SNR_EDGE_MEANTH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgemeanth1           : 10  ; /* [9..0]  */
        unsigned int    edgemeanth2           : 10  ; /* [19..10]  */
        unsigned int    edgemeanth3           : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_MEANTH1;

/* Define the union U_VPSS_SNR_EDGE_MEANTH2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgemeanth4           : 10  ; /* [9..0]  */
        unsigned int    edgemeanth5           : 10  ; /* [19..10]  */
        unsigned int    edgemeanth6           : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_MEANTH2;

/* Define the union U_VPSS_SNR_EDGE_MEANTH3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgemeanth7           : 10  ; /* [9..0]  */
        unsigned int    edgemeanth8           : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_MEANTH3;

/* Define the union U_VPSS_SNR_EDGE_MEANK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgemeank1            : 6   ; /* [5..0]  */
        unsigned int    edgemeank2            : 6   ; /* [11..6]  */
        unsigned int    edgemeank3            : 6   ; /* [17..12]  */
        unsigned int    edgemeank4            : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_MEANK1;

/* Define the union U_VPSS_SNR_EDGE_MEANK2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgemeank5            : 6   ; /* [5..0]  */
        unsigned int    edgemeank6            : 6   ; /* [11..6]  */
        unsigned int    edgemeank7            : 6   ; /* [17..12]  */
        unsigned int    edgemeank8            : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_MEANK2;

/* Define the union U_VPSS_SNR_WIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    y2dwinwidth           : 5   ; /* [4..0]  */
        unsigned int    y2dwinheight          : 4   ; /* [8..5]  */
        unsigned int    c2dwinwidth           : 4   ; /* [12..9]  */
        unsigned int    c2dwinheight          : 2   ; /* [14..13]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_WIN;

/* Define the union U_VPSS_SNR_STR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ysnrstr               : 8   ; /* [7..0]  */
        unsigned int    csnrstr               : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_STR;

/* Define the union U_VPSS_SNR_STR_OFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    stroffset0            : 2   ; /* [1..0]  */
        unsigned int    stroffset1            : 2   ; /* [3..2]  */
        unsigned int    stroffset2            : 2   ; /* [5..4]  */
        unsigned int    stroffset3            : 2   ; /* [7..6]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_STR_OFFSET;

/* Define the union U_VPSS_SNR_STR_ADJUST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ystradjust            : 6   ; /* [5..0]  */
        unsigned int    cstradjust            : 6   ; /* [11..6]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_STR_ADJUST;

/* Define the union U_VPSS_SNR_SCHANGE_TH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scenechangeth         : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_SCHANGE_TH;

/* Define the union U_VPSS_SNR_EDGE_RATIO_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ratio1                : 9   ; /* [8..0]  */
        unsigned int    ratio2                : 9   ; /* [17..9]  */
        unsigned int    ratio3                : 9   ; /* [26..18]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_RATIO_1;

/* Define the union U_VPSS_SNR_EDGE_RATIO_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ratio4                : 9   ; /* [8..0]  */
        unsigned int    ratio5                : 9   ; /* [17..9]  */
        unsigned int    ratio6                : 9   ; /* [26..18]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_RATIO_2;

/* Define the union U_VPSS_SNR_EDGE_RATIO_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ratio7                : 9   ; /* [8..0]  */
        unsigned int    ratio8                : 9   ; /* [17..9]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_RATIO_3;

/* Define the union U_VPSS_SNR_EDGE_STR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edge1                 : 4   ; /* [3..0]  */
        unsigned int    edge2                 : 4   ; /* [7..4]  */
        unsigned int    edge3                 : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_EDGE_STR;

/* Define the union U_VPSS_SNR_TEST_COLOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    test_color_cb         : 10  ; /* [9..0]  */
        unsigned int    test_color_cr         : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_TEST_COLOR;

/* Define the union U_VPSS_SNR_SCHANGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scenechange_info      : 8   ; /* [7..0]  */
        unsigned int    scenechange_en        : 1   ; /* [8]  */
        unsigned int    scenechange_mode1_en  : 1   ; /* [9]  */
        unsigned int    scenechange_bldk      : 10  ; /* [19..10]  */
        unsigned int    scenechange_bldcore   : 10  ; /* [29..20]  */
        unsigned int    scenechange_mode2_en  : 1   ; /* [30]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_SCHANGE;

/* Define the union U_VPSS_SNR_MT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motionalpha           : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_MT;

/* Define the union U_VPSS_SNR_CBCR_W1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cb_begin1             : 6   ; /* [5..0]  */
        unsigned int    cr_begin1             : 6   ; /* [11..6]  */
        unsigned int    cb_end1               : 6   ; /* [17..12]  */
        unsigned int    cr_end1               : 6   ; /* [23..18]  */
        unsigned int    cbcr_weight1          : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_CBCR_W1;

/* Define the union U_VPSS_SNR_CBCR_W2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cb_begin2             : 6   ; /* [5..0]  */
        unsigned int    cr_begin2             : 6   ; /* [11..6]  */
        unsigned int    cb_end2               : 6   ; /* [17..12]  */
        unsigned int    cr_end2               : 6   ; /* [23..18]  */
        unsigned int    cbcr_weight2          : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SNR_CBCR_W2;

/* Define the union U_MOTION_EDGE_LUT_01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut00     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut01     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut02     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut03     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut04     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut05     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut06     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut07     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_01;

/* Define the union U_MOTION_EDGE_LUT_02 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut08     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut09     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut0a     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut0b     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut0c     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut0d     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut0e     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut0f     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_02;

/* Define the union U_MOTION_EDGE_LUT_03 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut10     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut11     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut12     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut13     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut14     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut15     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut16     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut17     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_03;

/* Define the union U_MOTION_EDGE_LUT_04 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut18     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut19     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut1a     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut1b     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut1c     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut1d     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut1e     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut1f     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_04;

/* Define the union U_MOTION_EDGE_LUT_05 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut20     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut21     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut22     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut23     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut24     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut25     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut26     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut27     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_05;

/* Define the union U_MOTION_EDGE_LUT_06 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut28     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut29     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut2a     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut2b     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut2c     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut2d     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut2e     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut2f     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_06;

/* Define the union U_MOTION_EDGE_LUT_07 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut30     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut31     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut32     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut33     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut34     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut35     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut36     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut37     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_07;

/* Define the union U_MOTION_EDGE_LUT_08 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut38     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut39     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut3a     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut3b     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut3c     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut3d     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut3e     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut3f     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_08;

/* Define the union U_MOTION_EDGE_LUT_09 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut40     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut41     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut42     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut43     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut44     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut45     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut46     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut47     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_09;

/* Define the union U_MOTION_EDGE_LUT_10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut48     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut49     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut4a     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut4b     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut4c     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut4d     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut4e     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut4f     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_10;

/* Define the union U_MOTION_EDGE_LUT_11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut50     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut51     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut52     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut53     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut54     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut55     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut56     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut57     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_11;

/* Define the union U_MOTION_EDGE_LUT_12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut58     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut59     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut5a     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut5b     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut5c     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut5d     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut5e     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut5f     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_12;

/* Define the union U_MOTION_EDGE_LUT_13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut60     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut61     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut62     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut63     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut64     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut65     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut66     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut67     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_13;

/* Define the union U_MOTION_EDGE_LUT_14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut68     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut69     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut6a     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut6b     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut6c     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut6d     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut6e     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut6f     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_14;

/* Define the union U_MOTION_EDGE_LUT_15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut70     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut71     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut72     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut73     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut74     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut75     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut76     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut77     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_15;

/* Define the union U_MOTION_EDGE_LUT_16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut78     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut79     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut7a     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut7b     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut7c     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut7d     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut7e     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut7f     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_16;

/* Define the union U_MOTION_EDGE_LUT_17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut80     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut81     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut82     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut83     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut84     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut85     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut86     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut87     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_17;

/* Define the union U_MOTION_EDGE_LUT_18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut88     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut89     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut8a     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut8b     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut8c     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut8d     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut8e     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut8f     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_18;

/* Define the union U_MOTION_EDGE_LUT_19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut90     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut91     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut92     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut93     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut94     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut95     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut96     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut97     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_19;

/* Define the union U_MOTION_EDGE_LUT_20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lut98     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lut99     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lut9a     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lut9b     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lut9c     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lut9d     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lut9e     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lut9f     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_20;

/* Define the union U_MOTION_EDGE_LUT_21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_luta0     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_luta1     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_luta2     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_luta3     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_luta4     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_luta5     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_luta6     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_luta7     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_21;

/* Define the union U_MOTION_EDGE_LUT_22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_luta8     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_luta9     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lutaa     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lutab     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lutac     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lutad     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lutae     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lutaf     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_22;

/* Define the union U_MOTION_EDGE_LUT_23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lutb0     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lutb1     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lutb2     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lutb3     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lutb4     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lutb5     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lutb6     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lutb7     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_23;

/* Define the union U_MOTION_EDGE_LUT_24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lutb8     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lutb9     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lutba     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lutbb     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lutbc     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lutbd     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lutbe     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lutbf     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_24;

/* Define the union U_MOTION_EDGE_LUT_25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lutc0     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lutc1     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lutc2     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lutc3     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lutc4     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lutc5     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lutc6     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lutc7     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_25;

/* Define the union U_MOTION_EDGE_LUT_26 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lutc8     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lutc9     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lutca     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lutcb     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lutcc     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lutcd     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lutce     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lutcf     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_26;

/* Define the union U_MOTION_EDGE_LUT_27 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lutd0     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lutd1     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lutd2     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lutd3     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lutd4     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lutd5     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lutd6     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lutd7     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_27;

/* Define the union U_MOTION_EDGE_LUT_28 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lutd8     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lutd9     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lutda     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lutdb     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lutdc     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lutdd     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lutde     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lutdf     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_28;

/* Define the union U_MOTION_EDGE_LUT_29 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lute0     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lute1     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lute2     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lute3     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lute4     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lute5     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lute6     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lute7     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_29;

/* Define the union U_MOTION_EDGE_LUT_30 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lute8     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lute9     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lutea     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_luteb     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lutec     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_luted     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lutee     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lutef     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_30;

/* Define the union U_MOTION_EDGE_LUT_31 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lutf0     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lutf1     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lutf2     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lutf3     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lutf4     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lutf5     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lutf6     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lutf7     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_31;

/* Define the union U_MOTION_EDGE_LUT_32 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motion_edge_lutf8     : 4   ; /* [3..0]  */
        unsigned int    motion_edge_lutf9     : 4   ; /* [7..4]  */
        unsigned int    motion_edge_lutfa     : 4   ; /* [11..8]  */
        unsigned int    motion_edge_lutfb     : 4   ; /* [15..12]  */
        unsigned int    motion_edge_lutfc     : 4   ; /* [19..16]  */
        unsigned int    motion_edge_lutfd     : 4   ; /* [23..20]  */
        unsigned int    motion_edge_lutfe     : 4   ; /* [27..24]  */
        unsigned int    motion_edge_lutff     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MOTION_EDGE_LUT_32;

/* Define the union U_VPSS_DM_DR_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    enep                  : 1   ; /* [0]  */
        unsigned int    reserved_0            : 4   ; /* [4..1]  */
        unsigned int    enstradjstbylm        : 1   ; /* [5]  */
        unsigned int    enangadjstbylm        : 1   ; /* [6]  */
        unsigned int    clipmode              : 1   ; /* [7]  */
        unsigned int    reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DM_DR_CTRL;

/* Define the union U_VPSS_DR_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    regthrmdfangctrst     : 4   ; /* [3..0]  */
        unsigned int    regdrstrenth          : 5   ; /* [8..4]  */
        unsigned int    lradjpixrange         : 9   ; /* [17..9]  */
        unsigned int    lradjpixrange_10      : 11  ; /* [28..18]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
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

/* Define the union U_VPSS_DMDR_MERGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    u8drdm_merge_mode     : 3   ; /* [2..0]  */
        unsigned int    u8drdm_blend_str      : 4   ; /* [6..3]  */
        unsigned int    dmcond_app_dren       : 1   ; /* [7]  */
        unsigned int    drdir_blend_str       : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DMDR_MERGE;

/* Define the union U_VPSS_DMDR_MERGE_DELTA0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    alpha_acthvratio      : 11  ; /* [10..0]  */
        unsigned int    alpha_acthvrange      : 15  ; /* [25..11]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DMDR_MERGE_DELTA0;

/* Define the union U_VPSS_DMDR_MERGE_DELTA1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    winact_cliplimit      : 16  ; /* [15..0]  */
        unsigned int    winact_limit          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DMDR_MERGE_DELTA1;

/* Define the union U_VPSS_DMDR_MERGE_DELTA2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    winact_ratio          : 11  ; /* [10..0]  */
        unsigned int    angstr1_minusfac      : 13  ; /* [23..11]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_DMDR_MERGE_DELTA2;

/* Define the union U_VPSS_FMTC_GAMMA_CSC_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpss_csc_en           : 1   ; /* [0]  */
        unsigned int    vpss_gamma_en         : 1   ; /* [1]  */
        unsigned int    vpss_fmtc_demo_en     : 1   ; /* [2]  */
        unsigned int    vpss_fmtc_demo_mode   : 1   ; /* [3]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    vpss_fmtc_demo_offset : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_CSC_CTRL;

/* Define the union U_VPSS_FMTC_GAMMA_YLUT_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_y_lut_00        : 8   ; /* [7..0]  */
        unsigned int    gamma_y_lut_01        : 8   ; /* [15..8]  */
        unsigned int    gamma_y_lut_02        : 8   ; /* [23..16]  */
        unsigned int    gamma_y_lut_03        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_YLUT_0;

/* Define the union U_VPSS_FMTC_GAMMA_YLUT_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_y_lut_04        : 8   ; /* [7..0]  */
        unsigned int    gamma_y_lut_05        : 8   ; /* [15..8]  */
        unsigned int    gamma_y_lut_06        : 8   ; /* [23..16]  */
        unsigned int    gamma_y_lut_07        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_YLUT_1;

/* Define the union U_VPSS_FMTC_GAMMA_YLUT_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_y_lut_08        : 8   ; /* [7..0]  */
        unsigned int    gamma_y_lut_09        : 8   ; /* [15..8]  */
        unsigned int    gamma_y_lut_10        : 8   ; /* [23..16]  */
        unsigned int    gamma_y_lut_11        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_YLUT_2;

/* Define the union U_VPSS_FMTC_GAMMA_YLUT_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_y_lut_12        : 8   ; /* [7..0]  */
        unsigned int    gamma_y_lut_13        : 8   ; /* [15..8]  */
        unsigned int    gamma_y_lut_14        : 8   ; /* [23..16]  */
        unsigned int    gamma_y_lut_15        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_YLUT_3;

/* Define the union U_VPSS_FMTC_GAMMA_YLUT_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_y_lut_16        : 8   ; /* [7..0]  */
        unsigned int    gamma_y_lut_17        : 8   ; /* [15..8]  */
        unsigned int    gamma_y_lut_18        : 8   ; /* [23..16]  */
        unsigned int    gamma_y_lut_19        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_YLUT_4;

/* Define the union U_VPSS_FMTC_GAMMA_YLUT_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_y_lut_20        : 8   ; /* [7..0]  */
        unsigned int    gamma_y_lut_21        : 8   ; /* [15..8]  */
        unsigned int    gamma_y_lut_22        : 8   ; /* [23..16]  */
        unsigned int    gamma_y_lut_23        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_YLUT_5;

/* Define the union U_VPSS_FMTC_GAMMA_YLUT_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_y_lut_24        : 8   ; /* [7..0]  */
        unsigned int    gamma_y_lut_25        : 8   ; /* [15..8]  */
        unsigned int    gamma_y_lut_26        : 8   ; /* [23..16]  */
        unsigned int    gamma_y_lut_27        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_YLUT_6;

/* Define the union U_VPSS_FMTC_GAMMA_YLUT_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_y_lut_28        : 8   ; /* [7..0]  */
        unsigned int    gamma_y_lut_29        : 8   ; /* [15..8]  */
        unsigned int    gamma_y_lut_30        : 8   ; /* [23..16]  */
        unsigned int    gamma_y_lut_31        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_YLUT_7;

/* Define the union U_VPSS_FMTC_GAMMA_YLUT_8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_y_lut_32        : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_YLUT_8;

/* Define the union U_VPSS_FMTC_GAMMA_ULUT_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_u_lut_00        : 8   ; /* [7..0]  */
        unsigned int    gamma_u_lut_01        : 8   ; /* [15..8]  */
        unsigned int    gamma_u_lut_02        : 8   ; /* [23..16]  */
        unsigned int    gamma_u_lut_03        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_ULUT_0;

/* Define the union U_VPSS_FMTC_GAMMA_ULUT_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_u_lut_04        : 8   ; /* [7..0]  */
        unsigned int    gamma_u_lut_05        : 8   ; /* [15..8]  */
        unsigned int    gamma_u_lut_06        : 8   ; /* [23..16]  */
        unsigned int    gamma_u_lut_07        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_ULUT_1;

/* Define the union U_VPSS_FMTC_GAMMA_ULUT_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_u_lut_08        : 8   ; /* [7..0]  */
        unsigned int    gamma_u_lut_09        : 8   ; /* [15..8]  */
        unsigned int    gamma_u_lut_10        : 8   ; /* [23..16]  */
        unsigned int    gamma_u_lut_11        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_ULUT_2;

/* Define the union U_VPSS_FMTC_GAMMA_ULUT_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_u_lut_12        : 8   ; /* [7..0]  */
        unsigned int    gamma_u_lut_13        : 8   ; /* [15..8]  */
        unsigned int    gamma_u_lut_14        : 8   ; /* [23..16]  */
        unsigned int    gamma_u_lut_15        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_ULUT_3;

/* Define the union U_VPSS_FMTC_GAMMA_ULUT_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_u_lut_16        : 8   ; /* [7..0]  */
        unsigned int    gamma_u_lut_17        : 8   ; /* [15..8]  */
        unsigned int    gamma_u_lut_18        : 8   ; /* [23..16]  */
        unsigned int    gamma_u_lut_19        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_ULUT_4;

/* Define the union U_VPSS_FMTC_GAMMA_ULUT_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_u_lut_20        : 8   ; /* [7..0]  */
        unsigned int    gamma_u_lut_21        : 8   ; /* [15..8]  */
        unsigned int    gamma_u_lut_22        : 8   ; /* [23..16]  */
        unsigned int    gamma_u_lut_23        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_ULUT_5;

/* Define the union U_VPSS_FMTC_GAMMA_ULUT_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_u_lut_24        : 8   ; /* [7..0]  */
        unsigned int    gamma_u_lut_25        : 8   ; /* [15..8]  */
        unsigned int    gamma_u_lut_26        : 8   ; /* [23..16]  */
        unsigned int    gamma_u_lut_27        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_ULUT_6;

/* Define the union U_VPSS_FMTC_GAMMA_ULUT_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_u_lut_28        : 8   ; /* [7..0]  */
        unsigned int    gamma_u_lut_29        : 8   ; /* [15..8]  */
        unsigned int    gamma_u_lut_30        : 8   ; /* [23..16]  */
        unsigned int    gamma_u_lut_31        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_ULUT_7;

/* Define the union U_VPSS_FMTC_GAMMA_ULUT_8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_u_lut_32        : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_ULUT_8;

/* Define the union U_VPSS_FMTC_GAMMA_VLUT_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_v_lut_00        : 8   ; /* [7..0]  */
        unsigned int    gamma_v_lut_01        : 8   ; /* [15..8]  */
        unsigned int    gamma_v_lut_02        : 8   ; /* [23..16]  */
        unsigned int    gamma_v_lut_03        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_VLUT_0;

/* Define the union U_VPSS_FMTC_GAMMA_VLUT_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_v_lut_04        : 8   ; /* [7..0]  */
        unsigned int    gamma_v_lut_05        : 8   ; /* [15..8]  */
        unsigned int    gamma_v_lut_06        : 8   ; /* [23..16]  */
        unsigned int    gamma_v_lut_07        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_VLUT_1;

/* Define the union U_VPSS_FMTC_GAMMA_VLUT_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_v_lut_08        : 8   ; /* [7..0]  */
        unsigned int    gamma_v_lut_09        : 8   ; /* [15..8]  */
        unsigned int    gamma_v_lut_10        : 8   ; /* [23..16]  */
        unsigned int    gamma_v_lut_11        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_VLUT_2;

/* Define the union U_VPSS_FMTC_GAMMA_VLUT_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_v_lut_12        : 8   ; /* [7..0]  */
        unsigned int    gamma_v_lut_13        : 8   ; /* [15..8]  */
        unsigned int    gamma_v_lut_14        : 8   ; /* [23..16]  */
        unsigned int    gamma_v_lut_15        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_VLUT_3;

/* Define the union U_VPSS_FMTC_GAMMA_VLUT_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_v_lut_16        : 8   ; /* [7..0]  */
        unsigned int    gamma_v_lut_17        : 8   ; /* [15..8]  */
        unsigned int    gamma_v_lut_18        : 8   ; /* [23..16]  */
        unsigned int    gamma_v_lut_19        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_VLUT_4;

/* Define the union U_VPSS_FMTC_GAMMA_VLUT_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_v_lut_20        : 8   ; /* [7..0]  */
        unsigned int    gamma_v_lut_21        : 8   ; /* [15..8]  */
        unsigned int    gamma_v_lut_22        : 8   ; /* [23..16]  */
        unsigned int    gamma_v_lut_23        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_VLUT_5;

/* Define the union U_VPSS_FMTC_GAMMA_VLUT_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_v_lut_24        : 8   ; /* [7..0]  */
        unsigned int    gamma_v_lut_25        : 8   ; /* [15..8]  */
        unsigned int    gamma_v_lut_26        : 8   ; /* [23..16]  */
        unsigned int    gamma_v_lut_27        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_VLUT_6;

/* Define the union U_VPSS_FMTC_GAMMA_VLUT_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_v_lut_28        : 8   ; /* [7..0]  */
        unsigned int    gamma_v_lut_29        : 8   ; /* [15..8]  */
        unsigned int    gamma_v_lut_30        : 8   ; /* [23..16]  */
        unsigned int    gamma_v_lut_31        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_VLUT_7;

/* Define the union U_VPSS_FMTC_GAMMA_VLUT_8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_v_lut_32        : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_GAMMA_VLUT_8;

/* Define the union U_VPSS_FMTC_CSC_COEF_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    csc_coef_00           : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    csc_coef_01           : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_CSC_COEF_0;

/* Define the union U_VPSS_FMTC_CSC_COEF_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    csc_coef_02           : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    csc_coef_10           : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_CSC_COEF_1;

/* Define the union U_VPSS_FMTC_CSC_COEF_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    csc_coef_11           : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    csc_coef_12           : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_CSC_COEF_2;

/* Define the union U_VPSS_FMTC_CSC_COEF_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    csc_coef_20           : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    csc_coef_21           : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_CSC_COEF_3;

/* Define the union U_VPSS_FMTC_CSC_COEF_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    csc_coef_22           : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_CSC_COEF_4;

/* Define the union U_VPSS_FMTC_CSC_COEF_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    csc_in_dc0            : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    csc_in_dc1            : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_CSC_COEF_5;

/* Define the union U_VPSS_FMTC_CSC_COEF_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    csc_in_dc2            : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    csc_out_dc0           : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_CSC_COEF_6;

/* Define the union U_VPSS_FMTC_CSC_COEF_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    csc_out_dc1           : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    csc_out_dc2           : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_FMTC_CSC_COEF_7;

/* Define the union U_VPSS_TNR_CONTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nren                  : 1   ; /* [0]  */
        unsigned int    ynren                 : 1   ; /* [1]  */
        unsigned int    cnren                 : 1   ; /* [2]  */
        unsigned int    mcnr_en               : 1   ; /* [3]  */
        unsigned int    mcnr_meds_en          : 1   ; /* [4]  */
        unsigned int    marketmodeen          : 1   ; /* [5]  */
        unsigned int    marketmode            : 1   ; /* [6]  */
        unsigned int    marketcoor            : 13  ; /* [19..7]  */
        unsigned int    motion_mode           : 2   ; /* [21..20]  */
        unsigned int    mcnr_rgme_mode        : 2   ; /* [23..22]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CONTRL;

/* Define the union U_VPSS_TNR_MAMD_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mdprelpfen            : 1   ; /* [0]  */
        unsigned int    mean_ymotion_adj_en   : 1   ; /* [1]  */
        unsigned int    mean_cmotion_adj_en   : 1   ; /* [2]  */
        unsigned int    meanyadjshift         : 3   ; /* [5..3]  */
        unsigned int    meancadjshift         : 3   ; /* [8..6]  */
        unsigned int    ymotioncalcmode       : 2   ; /* [10..9]  */
        unsigned int    cmotioncalcmode       : 2   ; /* [12..11]  */
        unsigned int    ymotionlpfmode        : 2   ; /* [14..13]  */
        unsigned int    cmotionlpfmode        : 2   ; /* [16..15]  */
        unsigned int    alpha1                : 3   ; /* [19..17]  */
        unsigned int    alpha2                : 3   ; /* [22..20]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAMD_CTRL;

/* Define the union U_VPSS_TNR_MAMD_GAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymdgain               : 6   ; /* [5..0]  */
        unsigned int    ymdcore               : 6   ; /* [11..6]  */
        unsigned int    cmdgain               : 6   ; /* [17..12]  */
        unsigned int    cmdcore               : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAMD_GAIN;

/* Define the union U_VPSS_TNR_MAND_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    noisedetecten         : 1   ; /* [0]  */
        unsigned int    blkdiffthd            : 8   ; /* [8..1]  */
        unsigned int    randomctrlmode        : 2   ; /* [10..9]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAND_CTRL;

/* Define the union U_VPSS_TNR_MAND_THD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    flatthdmin            : 8   ; /* [7..0]  */
        unsigned int    flatthdmed            : 8   ; /* [15..8]  */
        unsigned int    flatthdmax            : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAND_THD;

/* Define the union U_VPSS_TNR_MAME_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motionestimateen      : 1   ; /* [0]  */
        unsigned int    mvrefmode             : 2   ; /* [2..1]  */
        unsigned int    mvlpfmode             : 2   ; /* [4..3]  */
        unsigned int    gm_adj                : 3   ; /* [7..5]  */
        unsigned int    discardsmvxen         : 1   ; /* [8]  */
        unsigned int    discardsmvyen         : 1   ; /* [9]  */
        unsigned int    smvxthd               : 3   ; /* [12..10]  */
        unsigned int    smvythd               : 3   ; /* [15..13]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAME_CTRL;

/* Define the union U_VPSS_TNR_MAME_GAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mag_pnl_gain_0mv      : 8   ; /* [7..0]  */
        unsigned int    mag_pnl_core_0mv      : 8   ; /* [15..8]  */
        unsigned int    mag_pnl_gain_xmv      : 8   ; /* [23..16]  */
        unsigned int    mag_pnl_core_xmv      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAME_GAIN;

/* Define the union U_VPSS_TNR_MAME_STD_CORE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    std_core_0mv          : 5   ; /* [4..0]  */
        unsigned int    std_core_xmv          : 5   ; /* [9..5]  */
        unsigned int    std_pnl_gain_0mv      : 5   ; /* [14..10]  */
        unsigned int    std_pnl_core_0mv      : 5   ; /* [19..15]  */
        unsigned int    std_pnl_gain_xmv      : 5   ; /* [24..20]  */
        unsigned int    std_pnl_core_xmv      : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAME_STD_CORE;

/* Define the union U_VPSS_TNR_MAME_STD_GAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    std_pnl_gain          : 5   ; /* [4..0]  */
        unsigned int    std_pnl_core          : 5   ; /* [9..5]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAME_STD_GAIN;

/* Define the union U_VPSS_TNR_MAME_MV_THD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj_mv_max            : 5   ; /* [4..0]  */
        unsigned int    adj_mv_min            : 5   ; /* [9..5]  */
        unsigned int    adj_0mv_min           : 5   ; /* [14..10]  */
        unsigned int    adj_xmv_min           : 5   ; /* [19..15]  */
        unsigned int    adj_0mv_max           : 5   ; /* [24..20]  */
        unsigned int    adj_xmv_max           : 5   ; /* [29..25]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAME_MV_THD;

/* Define the union U_VPSS_TNR_MAMC_CBCR_UPEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cbcrupdateen          : 1   ; /* [0]  */
        unsigned int    cbcrweight1           : 4   ; /* [4..1]  */
        unsigned int    cbcrweight2           : 4   ; /* [8..5]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAMC_CBCR_UPEN;

/* Define the union U_VPSS_TNR_MAMC_CBCR_UPTHD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cbbegin1              : 6   ; /* [5..0]  */
        unsigned int    crbegin1              : 6   ; /* [11..6]  */
        unsigned int    cbend1                : 6   ; /* [17..12]  */
        unsigned int    crend1                : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAMC_CBCR_UPTHD1;

/* Define the union U_VPSS_TNR_MAMC_CBCR_UPTHD2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cbbegin2              : 6   ; /* [5..0]  */
        unsigned int    crbegin2              : 6   ; /* [11..6]  */
        unsigned int    cbend2                : 6   ; /* [17..12]  */
        unsigned int    crend2                : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAMC_CBCR_UPTHD2;

/* Define the union U_VPSS_TNR_MAMC_PRE_MAD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    premotionmergemode    : 1   ; /* [0]  */
        unsigned int    premotionalpha        : 5   ; /* [5..1]  */
        unsigned int    motionmergeratio      : 5   ; /* [10..6]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAMC_PRE_MAD;

/* Define the union U_VPSS_TNR_MAMC_MAP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    motionmappingen       : 1   ; /* [0]  */
        unsigned int    ymotiongain           : 5   ; /* [5..1]  */
        unsigned int    ymotioncore           : 6   ; /* [11..6]  */
        unsigned int    cmotiongain           : 5   ; /* [16..12]  */
        unsigned int    cmotioncore           : 6   ; /* [22..17]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAMC_MAP;

/* Define the union U_VPSS_TNR_MAMC_SALTUS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    saltusdecten          : 1   ; /* [0]  */
        unsigned int    saltuslevel           : 5   ; /* [5..1]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAMC_SALTUS;

/* Define the union U_VPSS_TNR_MAMC_MCADJ */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymcadjen              : 1   ; /* [0]  */
        unsigned int    cmcadjen              : 1   ; /* [1]  */
        unsigned int    ymclpfmode            : 2   ; /* [3..2]  */
        unsigned int    cmclpfmode            : 2   ; /* [5..4]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MAMC_MCADJ;

/* Define the union U_VPSS_TNR_FILR_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionmode           : 1   ; /* [0]  */
        unsigned int    cmotionmode           : 1   ; /* [1]  */
        unsigned int    ynonrrange            : 5   ; /* [6..2]  */
        unsigned int    cnonrrange            : 5   ; /* [11..7]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_FILR_MODE;

/* Define the union U_VPSS_TNR_SCENE_CHANGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scenechangeen         : 1   ; /* [0]  */
        unsigned int    scenechangeinfo       : 8   ; /* [8..1]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_SCENE_CHANGE;

/* Define the union U_VPSS_TNR_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cfg_ymdgain           : 6   ; /* [5..0]  */
        unsigned int    cfg_ymdcore           : 5   ; /* [10..6]  */
        unsigned int    cfg_cmdgain           : 6   ; /* [16..11]  */
        unsigned int    cfg_cmdcore           : 5   ; /* [21..17]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CFG;

/* Define the union U_VPSS_TNR_FLAT_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    flatinfoxmin          : 8   ; /* [7..0]  */
        unsigned int    flatinfoymin          : 8   ; /* [15..8]  */
        unsigned int    flatinfoxmax          : 8   ; /* [23..16]  */
        unsigned int    flatinfoymax          : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_FLAT_INFO;

/* Define the union U_VPSS_TNR_YBLEND */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yblendingxmin         : 5   ; /* [4..0]  */
        unsigned int    yblendingymin         : 5   ; /* [9..5]  */
        unsigned int    yblendingxmax         : 5   ; /* [14..10]  */
        unsigned int    yblendingymax         : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YBLEND;

/* Define the union U_VPSS_TNR_CBLEND */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cblendingxmin         : 5   ; /* [4..0]  */
        unsigned int    cblendingymin         : 5   ; /* [9..5]  */
        unsigned int    cblendingxmax         : 5   ; /* [14..10]  */
        unsigned int    cblendingymax         : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CBLEND;

/* Define the union U_VPSS_TNR_DTBLEND */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dtblendingxmin        : 5   ; /* [4..0]  */
        unsigned int    dtblendingymin        : 5   ; /* [9..5]  */
        unsigned int    dtblendingxmax        : 5   ; /* [14..10]  */
        unsigned int    dtblendingymax        : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_DTBLEND;

/* Define the union U_VPSS_TNR_MEAN_MOTION_LUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mean_motion_ratio0    : 6   ; /* [5..0]  */
        unsigned int    mean_motion_ratio1    : 6   ; /* [11..6]  */
        unsigned int    mean_motion_ratio2    : 6   ; /* [17..12]  */
        unsigned int    mean_motion_ratio3    : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MEAN_MOTION_LUT0;

/* Define the union U_VPSS_TNR_MEAN_MOTION_LUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mean_motion_ratio4    : 6   ; /* [5..0]  */
        unsigned int    mean_motion_ratio5    : 6   ; /* [11..6]  */
        unsigned int    mean_motion_ratio6    : 6   ; /* [17..12]  */
        unsigned int    mean_motion_ratio7    : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MEAN_MOTION_LUT1;

/* Define the union U_VPSS_TNR_MEAN_MOTION_LUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mean_motion_ratio8    : 6   ; /* [5..0]  */
        unsigned int    mean_motion_ratio9    : 6   ; /* [11..6]  */
        unsigned int    mean_motion_ratio10   : 6   ; /* [17..12]  */
        unsigned int    mean_motion_ratio11   : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MEAN_MOTION_LUT2;

/* Define the union U_VPSS_TNR_MEAN_MOTION_LUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mean_motion_ratio12   : 6   ; /* [5..0]  */
        unsigned int    mean_motion_ratio13   : 6   ; /* [11..6]  */
        unsigned int    mean_motion_ratio14   : 6   ; /* [17..12]  */
        unsigned int    mean_motion_ratio15   : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_MEAN_MOTION_LUT3;

/* Define the union U_VPSS_TNR_YBLENDING_LUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yblendingalphalut0    : 5   ; /* [4..0]  */
        unsigned int    yblendingalphalut1    : 5   ; /* [9..5]  */
        unsigned int    yblendingalphalut2    : 5   ; /* [14..10]  */
        unsigned int    yblendingalphalut3    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YBLENDING_LUT0;

/* Define the union U_VPSS_TNR_YBLENDING_LUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yblendingalphalut4    : 5   ; /* [4..0]  */
        unsigned int    yblendingalphalut5    : 5   ; /* [9..5]  */
        unsigned int    yblendingalphalut6    : 5   ; /* [14..10]  */
        unsigned int    yblendingalphalut7    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YBLENDING_LUT1;

/* Define the union U_VPSS_TNR_YBLENDING_LUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yblendingalphalut8    : 5   ; /* [4..0]  */
        unsigned int    yblendingalphalut9    : 5   ; /* [9..5]  */
        unsigned int    yblendingalphalut10   : 5   ; /* [14..10]  */
        unsigned int    yblendingalphalut11   : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YBLENDING_LUT2;

/* Define the union U_VPSS_TNR_YBLENDING_LUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yblendingalphalut12   : 5   ; /* [4..0]  */
        unsigned int    yblendingalphalut13   : 5   ; /* [9..5]  */
        unsigned int    yblendingalphalut14   : 5   ; /* [14..10]  */
        unsigned int    yblendingalphalut15   : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YBLENDING_LUT3;

/* Define the union U_VPSS_TNR_YBLENDING_LUT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yblendingalphalut16   : 5   ; /* [4..0]  */
        unsigned int    yblendingalphalut17   : 5   ; /* [9..5]  */
        unsigned int    yblendingalphalut18   : 5   ; /* [14..10]  */
        unsigned int    yblendingalphalut19   : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YBLENDING_LUT4;

/* Define the union U_VPSS_TNR_YBLENDING_LUT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yblendingalphalut20   : 5   ; /* [4..0]  */
        unsigned int    yblendingalphalut21   : 5   ; /* [9..5]  */
        unsigned int    yblendingalphalut22   : 5   ; /* [14..10]  */
        unsigned int    yblendingalphalut23   : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YBLENDING_LUT5;

/* Define the union U_VPSS_TNR_YBLENDING_LUT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yblendingalphalut24   : 5   ; /* [4..0]  */
        unsigned int    yblendingalphalut25   : 5   ; /* [9..5]  */
        unsigned int    yblendingalphalut26   : 5   ; /* [14..10]  */
        unsigned int    yblendingalphalut27   : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YBLENDING_LUT6;

/* Define the union U_VPSS_TNR_YBLENDING_LUT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yblendingalphalut28   : 5   ; /* [4..0]  */
        unsigned int    yblendingalphalut29   : 5   ; /* [9..5]  */
        unsigned int    yblendingalphalut30   : 5   ; /* [14..10]  */
        unsigned int    yblendingalphalut31   : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YBLENDING_LUT7;

/* Define the union U_VPSS_TNR_CBLENDING_LUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cblendingalphalut0    : 5   ; /* [4..0]  */
        unsigned int    cblendingalphalut1    : 5   ; /* [9..5]  */
        unsigned int    cblendingalphalut2    : 5   ; /* [14..10]  */
        unsigned int    cblendingalphalut3    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CBLENDING_LUT0;

/* Define the union U_VPSS_TNR_CBLENDING_LUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cblendingalphalut4    : 5   ; /* [4..0]  */
        unsigned int    cblendingalphalut5    : 5   ; /* [9..5]  */
        unsigned int    cblendingalphalut6    : 5   ; /* [14..10]  */
        unsigned int    cblendingalphalut7    : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CBLENDING_LUT1;

/* Define the union U_VPSS_TNR_CBLENDING_LUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cblendingalphalut8    : 5   ; /* [4..0]  */
        unsigned int    cblendingalphalut9    : 5   ; /* [9..5]  */
        unsigned int    cblendingalphalut10   : 5   ; /* [14..10]  */
        unsigned int    cblendingalphalut11   : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CBLENDING_LUT2;

/* Define the union U_VPSS_TNR_CBLENDING_LUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cblendingalphalut12   : 5   ; /* [4..0]  */
        unsigned int    cblendingalphalut13   : 5   ; /* [9..5]  */
        unsigned int    cblendingalphalut14   : 5   ; /* [14..10]  */
        unsigned int    cblendingalphalut15   : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CBLENDING_LUT3;

/* Define the union U_VPSS_TNR_CBLENDING_LUT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cblendingalphalut16   : 5   ; /* [4..0]  */
        unsigned int    cblendingalphalut17   : 5   ; /* [9..5]  */
        unsigned int    cblendingalphalut18   : 5   ; /* [14..10]  */
        unsigned int    cblendingalphalut19   : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CBLENDING_LUT4;

/* Define the union U_VPSS_TNR_CBLENDING_LUT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cblendingalphalut20   : 5   ; /* [4..0]  */
        unsigned int    cblendingalphalut21   : 5   ; /* [9..5]  */
        unsigned int    cblendingalphalut22   : 5   ; /* [14..10]  */
        unsigned int    cblendingalphalut23   : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CBLENDING_LUT5;

/* Define the union U_VPSS_TNR_CBLENDING_LUT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cblendingalphalut24   : 5   ; /* [4..0]  */
        unsigned int    cblendingalphalut25   : 5   ; /* [9..5]  */
        unsigned int    cblendingalphalut26   : 5   ; /* [14..10]  */
        unsigned int    cblendingalphalut27   : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CBLENDING_LUT6;

/* Define the union U_VPSS_TNR_CBLENDING_LUT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cblendingalphalut28   : 5   ; /* [4..0]  */
        unsigned int    cblendingalphalut29   : 5   ; /* [9..5]  */
        unsigned int    cblendingalphalut30   : 5   ; /* [14..10]  */
        unsigned int    cblendingalphalut31   : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CBLENDING_LUT7;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut0        : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut1        : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut2        : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut3        : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT0;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut4        : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut5        : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut6        : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut7        : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT1;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut8        : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut9        : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut10       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut11       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT2;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut12       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut13       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut14       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut15       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT3;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut16       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut17       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut18       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut19       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT4;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut20       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut21       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut22       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut23       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT5;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut24       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut25       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut26       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut27       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT6;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut28       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut29       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut30       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut31       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT7;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut32       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut33       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut34       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut35       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT8;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut36       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut37       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut38       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut39       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT9;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut40       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut41       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut42       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut43       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT10;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut44       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut45       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut46       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut47       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT11;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut48       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut49       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut50       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut51       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT12;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut52       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut53       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut54       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut55       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT13;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut56       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut57       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut58       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut59       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT14;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut60       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut61       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut62       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut63       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT15;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut64       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut65       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut66       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut67       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT16;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut68       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut69       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut70       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut71       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT17;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut72       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut73       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut74       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut75       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT18;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut76       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut77       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut78       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut79       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT19;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut80       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut81       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut82       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut83       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT20;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut84       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut85       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut86       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut87       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT21;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut88       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut89       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut90       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut91       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT22;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut92       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut93       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut94       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut95       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT23;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut96       : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut97       : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut98       : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut99       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT24;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut100      : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut101      : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut102      : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut103      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT25;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT26 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut104      : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut105      : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut106      : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut107      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT26;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT27 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut108      : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut109      : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut110      : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut111      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT27;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT28 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut112      : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut113      : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut114      : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut115      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT28;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT29 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut116      : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut117      : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut118      : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut119      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT29;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT30 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut120      : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut121      : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut122      : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut123      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT30;

/* Define the union U_VPSS_TNR_YMOTIONSTR_LUT31 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ymotionstrlut124      : 5   ; /* [4..0]  */
        unsigned int    ymotionstrlut125      : 5   ; /* [9..5]  */
        unsigned int    ymotionstrlut126      : 5   ; /* [14..10]  */
        unsigned int    ymotionstrlut127      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_YMOTIONSTR_LUT31;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut0        : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut1        : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut2        : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut3        : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT0;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut4        : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut5        : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut6        : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut7        : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT1;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut8        : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut9        : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut10       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut11       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT2;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut12       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut13       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut14       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut15       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT3;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut16       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut17       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut18       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut19       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT4;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut20       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut21       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut22       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut23       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT5;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut24       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut25       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut26       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut27       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT6;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut28       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut29       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut30       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut31       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT7;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut32       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut33       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut34       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut35       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT8;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut36       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut37       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut38       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut39       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT9;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut40       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut41       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut42       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut43       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT10;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut44       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut45       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut46       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut47       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT11;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut48       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut49       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut50       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut51       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT12;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut52       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut53       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut54       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut55       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT13;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut56       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut57       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut58       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut59       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT14;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut60       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut61       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut62       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut63       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT15;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut64       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut65       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut66       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut67       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT16;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut68       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut69       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut70       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut71       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT17;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut72       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut73       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut74       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut75       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT18;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut76       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut77       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut78       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut79       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT19;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut80       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut81       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut82       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut83       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT20;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut84       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut85       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut86       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut87       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT21;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut88       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut89       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut90       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut91       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT22;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut92       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut93       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut94       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut95       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT23;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut96       : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut97       : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut98       : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut99       : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT24;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut100      : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut101      : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut102      : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut103      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT25;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT26 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut104      : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut105      : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut106      : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut107      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT26;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT27 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut108      : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut109      : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut110      : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut111      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT27;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT28 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut112      : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut113      : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut114      : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut115      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT28;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT29 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut116      : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut117      : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut118      : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut119      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT29;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT30 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut120      : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut121      : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut122      : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut123      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT30;

/* Define the union U_VPSS_TNR_CMOTIONSTR_LUT31 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmotionstrlut124      : 5   ; /* [4..0]  */
        unsigned int    cmotionstrlut125      : 5   ; /* [9..5]  */
        unsigned int    cmotionstrlut126      : 5   ; /* [14..10]  */
        unsigned int    cmotionstrlut127      : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_TNR_CMOTIONSTR_LUT31;

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
/* Define the union U_VPSS_SMMU_SCR */
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

} U_VPSS_SMMU_SCR;

/* Define the union U_VPSS_SMMU_LP_CTRL */
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

} U_VPSS_SMMU_LP_CTRL;

/* Define the union U_VPSS_SMMU_MEM_SPEEDCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rfs_emaw              : 2   ; /* [1..0]  */
        unsigned int    rfs_ema               : 3   ; /* [4..2]  */
        unsigned int    rfs_ret1n             : 1   ; /* [5]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SMMU_MEM_SPEEDCTRL;

/* Define the union U_VPSS_SMMU_INTMASK_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ints_tlbmiss_msk      : 1   ; /* [0]  */
        unsigned int    ints_ptw_trans_msk    : 1   ; /* [1]  */
        unsigned int    ints_tlbinvalid_rd_msk : 1   ; /* [2]  */
        unsigned int    ints_tlbinvalid_wr_msk : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SMMU_INTMASK_S;

/* Define the union U_VPSS_SMMU_INTRAW_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ints_tlbmiss_raw      : 1   ; /* [0]  */
        unsigned int    ints_ptw_trans_raw    : 1   ; /* [1]  */
        unsigned int    ints_tlbinvalid_rd_raw : 1   ; /* [2]  */
        unsigned int    ints_tlbinvalid_wr_raw : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SMMU_INTRAW_S;

/* Define the union U_VPSS_SMMU_INTSTAT_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ints_tlbmiss_stat     : 1   ; /* [0]  */
        unsigned int    ints_ptw_trans_stat   : 1   ; /* [1]  */
        unsigned int    ints_tlbinvalid_rd_stat : 1   ; /* [2]  */
        unsigned int    ints_tlbinvalid_wr_stat : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SMMU_INTSTAT_S;

/* Define the union U_VPSS_SMMU_INTCLR_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ints_tlbmiss_clr      : 1   ; /* [0]  */
        unsigned int    ints_ptw_trans_clr    : 1   ; /* [1]  */
        unsigned int    ints_tlbinvalid_rd_clr : 1   ; /* [2]  */
        unsigned int    ints_tlbinvalid_wr_clr : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SMMU_INTCLR_S;

/* Define the union U_VPSS_SMMU_INTMASK_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intns_tlbmiss_msk     : 1   ; /* [0]  */
        unsigned int    intns_ptw_trans_msk   : 1   ; /* [1]  */
        unsigned int    intns_tlbinvalid_rd_msk : 1   ; /* [2]  */
        unsigned int    intns_tlbinvalid_wr_msk : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SMMU_INTMASK_NS;

/* Define the union U_VPSS_SMMU_INTRAW_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intns_tlbmiss_raw     : 1   ; /* [0]  */
        unsigned int    intns_ptw_trans_raw   : 1   ; /* [1]  */
        unsigned int    intns_tlbinvalid_rd_raw : 1   ; /* [2]  */
        unsigned int    intns_tlbinvalid_wr_raw : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SMMU_INTRAW_NS;

/* Define the union U_VPSS_SMMU_INTSTAT_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intns_tlbmiss_stat    : 1   ; /* [0]  */
        unsigned int    intns_ptw_trans_stat  : 1   ; /* [1]  */
        unsigned int    intns_tlbinvalid_rd_stat : 1   ; /* [2]  */
        unsigned int    intns_tlbinvalid_wr_stat : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SMMU_INTSTAT_NS;

/* Define the union U_VPSS_SMMU_INTCLR_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intns_tlbmiss_clr     : 1   ; /* [0]  */
        unsigned int    intns_ptw_trans_clr   : 1   ; /* [1]  */
        unsigned int    intns_tlbinvalid_rd_clr : 1   ; /* [2]  */
        unsigned int    intns_tlbinvalid_wr_clr : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VPSS_SMMU_INTCLR_NS;

/* Define the union U_VPSS_SMMU_SCB_TTBR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int scb_ttbr               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_SCB_TTBR;
/* Define the union U_VPSS_SMMU_CB_TTBR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int cb_ttbr                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_CB_TTBR;
/* Define the union U_VPSS_SMMU_ERR_RDADDR_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int err_s_rd_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_ERR_RDADDR_S;
/* Define the union U_VPSS_SMMU_ERR_WRADDR_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int err_s_wr_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_ERR_WRADDR_S;
/* Define the union U_VPSS_SMMU_ERR_RDADDR_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int err_ns_rd_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_ERR_RDADDR_NS;
/* Define the union U_VPSS_SMMU_ERR_WRADDR_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int err_ns_wr_addr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_ERR_WRADDR_NS;
/* Define the union U_VPSS_SMMU_FAULT_ADDR_PTW_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fault_addr_ptw_s       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_FAULT_ADDR_PTW_S;
/* Define the union U_VPSS_SMMU_FAULT_ID_PTW_S */
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

} U_VPSS_SMMU_FAULT_ID_PTW_S;

/* Define the union U_VPSS_SMMU_FAULT_ADDR_PTW_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fault_addr_ptw_ns      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_FAULT_ADDR_PTW_NS;
/* Define the union U_VPSS_SMMU_FAULT_ID_PTW_NS */
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

} U_VPSS_SMMU_FAULT_ID_PTW_NS;

/* Define the union U_VPSS_SMMU_FAULT_PTW_NUM */
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

} U_VPSS_SMMU_FAULT_PTW_NUM;

/* Define the union U_VPSS_SMMU_FAULT_ADDR_WR_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fault_addr_wr_s        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_FAULT_ADDR_WR_S;
/* Define the union U_VPSS_SMMU_FAULT_TLB_WR_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fault_tlb_wr_s         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_FAULT_TLB_WR_S;
/* Define the union U_VPSS_SMMU_FAULT_ID_WR_S */
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

} U_VPSS_SMMU_FAULT_ID_WR_S;

/* Define the union U_VPSS_SMMU_FAULT_ADDR_WR_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fault_addr_wr_ns       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_FAULT_ADDR_WR_NS;
/* Define the union U_VPSS_SMMU_FAULT_TLB_WR_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fault_tlb_wr_ns        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_FAULT_TLB_WR_NS;
/* Define the union U_VPSS_SMMU_FAULT_ID_WR_NS */
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

} U_VPSS_SMMU_FAULT_ID_WR_NS;

/* Define the union U_VPSS_SMMU_FAULT_ADDR_RD_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fault_addr_rd_s        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_FAULT_ADDR_RD_S;
/* Define the union U_VPSS_SMMU_FAULT_TLB_RD_S */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fault_tlb_rd_s         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_FAULT_TLB_RD_S;
/* Define the union U_VPSS_SMMU_FAULT_ID_RD_S */
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

} U_VPSS_SMMU_FAULT_ID_RD_S;

/* Define the union U_VPSS_SMMU_FAULT_ADDR_RD_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fault_addr_rd_ns       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_FAULT_ADDR_RD_NS;
/* Define the union U_VPSS_SMMU_FAULT_TLB_RD_NS */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fault_tlb_rd_ns        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_VPSS_SMMU_FAULT_TLB_RD_NS;
/* Define the union U_VPSS_SMMU_FAULT_ID_RD_NS */
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

} U_VPSS_SMMU_FAULT_ID_RD_NS;

/* Define the union U_VPSS_SMMU_FAULT_TBU_INFO */
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

} U_VPSS_SMMU_FAULT_TBU_INFO;

/* Define the union U_VPSS_SMMU_FAULT_TBU_DBG */
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

} U_VPSS_SMMU_FAULT_TBU_DBG;

/* Define the union U_VPSS_SMMU_PREF_BUFFER_EMPTY */
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

} U_VPSS_SMMU_PREF_BUFFER_EMPTY;

/* Define the union U_VPSS_SMMU_PTWQ_IDLE */
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

} U_VPSS_SMMU_PTWQ_IDLE;

/* Define the union U_VPSS_SMMU_RESET_STATE */
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

} U_VPSS_SMMU_RESET_STATE;

/* Define the union U_VPSS_SMMU_MASTER_DBG0 */
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

} U_VPSS_SMMU_MASTER_DBG0;

/* Define the union U_VPSS_SMMU_MASTER_DBG1 */
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

} U_VPSS_SMMU_MASTER_DBG1;

/* Define the union U_VPSS_SMMU_MASTER_DBG2 */
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

} U_VPSS_SMMU_MASTER_DBG2;

/* Define the union U_VPSS_SMMU_MASTER_DBG3 */
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

} U_VPSS_SMMU_MASTER_DBG3;

/* Define the union U_VPSS_SMMU_MASTER_DBG4 */
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

} U_VPSS_SMMU_MASTER_DBG4;

/* Define the union U_VPSS_SMMU_MASTER_DBG5 */
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

} U_VPSS_SMMU_MASTER_DBG5;

/* Define the union U_VPSS_SMMU_MASTER_DBG6 */
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

} U_VPSS_SMMU_MASTER_DBG6;

/* Define the union U_VPSS_SMMU_MASTER_DBG7 */
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

} U_VPSS_SMMU_MASTER_DBG7;

/* Define the union U_VPSS_SMMU_MASTER_DBG8 */
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

} U_VPSS_SMMU_MASTER_DBG8;

//==============================================================================
/* Define the global struct */
typedef struct
{
    U_VPSS_CTRL            VPSS_CTRL                         ; /* 0x0 */
    U_VPSS_CTRL2           VPSS_CTRL2                        ; /* 0x4 */
    U_VPSS_CTRL3           VPSS_CTRL3                        ; /* 0x8 */
    U_VPSS_CMD_CNT_CHN     VPSS_CMD_CNT_CHN                  ; /* 0xc */
    U_VPSS_RCH_BYPASS      VPSS_RCH_BYPASS                   ; /* 0x10 */
    U_VPSS_WCH_BYPASS      VPSS_WCH_BYPASS                   ; /* 0x14 */
    U_VPSS_VHD0_ZMEORESO   VPSS_VHD0_ZMEORESO                ; /* 0x18 */
    U_VPSS_VHD0_ZME_LHADDR   VPSS_VHD0_ZME_LHADDR            ; /* 0x1c */
    U_VPSS_VHD0_ZME_LVADDR   VPSS_VHD0_ZME_LVADDR            ; /* 0x20 */
    U_VPSS_VHD0_ZME_CHADDR   VPSS_VHD0_ZME_CHADDR            ; /* 0x24 */
    U_VPSS_VHD0_ZME_CVADDR   VPSS_VHD0_ZME_CVADDR            ; /* 0x28 */
    U_VPSS_TNR_ADDR        VPSS_TNR_ADDR                     ; /* 0x2c */
    unsigned int           reserved_0                     ; /* 0x30 */
    U_VPSS_SNR_ADDR        VPSS_SNR_ADDR                     ; /* 0x34 */
    unsigned int           reserved_1                     ; /* 0x38 */
    U_VPSS_ZME_ADDR        VPSS_ZME_ADDR                     ; /* 0x3c */
    U_VPSS_DEI_ADDR        VPSS_DEI_ADDR                     ; /* 0x40 */
    U_VPSS_FMTC_ADDR       VPSS_FMTC_ADDR                    ; /* 0x44 */
    unsigned int           reserved_2[172]                     ; /* 0x48~0x2f4 */
    U_VPSS_NODEID          VPSS_NODEID                       ; /* 0x2f8 */
    U_VPSS_PNEXT           VPSS_PNEXT                        ; /* 0x2fc */
    U_VPSS_START           VPSS_START                        ; /* 0x300 */
    U_VPSS_INTSTATE        VPSS_INTSTATE                     ; /* 0x304 */
    U_VPSS_INTCLR          VPSS_INTCLR                       ; /* 0x308 */
    U_VPSS_RAWINT          VPSS_RAWINT                       ; /* 0x30c */
    U_VPSS_PFCNT           VPSS_PFCNT                        ; /* 0x310 */
    U_VPSS_MISCELLANEOUS   VPSS_MISCELLANEOUS                ; /* 0x314 */
    U_VPSS_MACCFG          VPSS_MACCFG                       ; /* 0x318 */
    U_VPSS_TIMEOUT         VPSS_TIMEOUT                      ; /* 0x31c */
    U_VPSS_FTCONFIG        VPSS_FTCONFIG                     ; /* 0x320 */
    U_VPSS_INTMASK         VPSS_INTMASK                      ; /* 0x324 */
    U_VPSS_MEMCTRL         VPSS_MEMCTRL                      ; /* 0x328 */
    unsigned int           reserved_3                     ; /* 0x32c */
    U_VPSS_EOFCNT          VPSS_EOFCNT                       ; /* 0x330 */
    unsigned int           reserved_4[3]                     ; /* 0x334~0x33c */
    U_VPSS_VERSION         VPSS_VERSION                      ; /* 0x340 */
    U_VPSS_BUS_CTRL        VPSS_BUS_CTRL                     ; /* 0x344 */
    unsigned int           reserved_5[46]                     ; /* 0x348~0x3fc */
    U_VPSS_IMGSIZE         VPSS_IMGSIZE                      ; /* 0x400 */
    U_VPSS_REFSIZE         VPSS_REFSIZE                      ; /* 0x404 */
    U_VPSS_IN_CTRL         VPSS_IN_CTRL                      ; /* 0x408 */
    U_VPSS_NXT2_CTRL       VPSS_NXT2_CTRL                    ; /* 0x40c */
    U_VPSS_NXT2YADDR       VPSS_NXT2YADDR                    ; /* 0x410 */
    U_VPSS_NXT2CADDR       VPSS_NXT2CADDR                    ; /* 0x414 */
    U_VPSS_NXT2STRIDE      VPSS_NXT2STRIDE                   ; /* 0x418 */
    U_VPSS_NX2Y_HEAD_ADDR   VPSS_NX2Y_HEAD_ADDR              ; /* 0x41c */
    U_VPSS_NX2Y_BOT_HEAD_ADDR   VPSS_NX2Y_BOT_HEAD_ADDR      ; /* 0x420 */
    U_VPSS_NX2C_HEAD_ADDR   VPSS_NX2C_HEAD_ADDR              ; /* 0x424 */
    U_VPSS_NX2C_BOT_HEAD_ADDR   VPSS_NX2C_BOT_HEAD_ADDR      ; /* 0x428 */
    U_VPSS_NX2_HEAD_STRIDE   VPSS_NX2_HEAD_STRIDE            ; /* 0x42c */
    U_VPSS_NX2_TUNL_CTRL   VPSS_NX2_TUNL_CTRL                ; /* 0x430 */
    U_VPSS_NX2_TUNL_ADDR   VPSS_NX2_TUNL_ADDR                ; /* 0x434 */
    unsigned int           reserved_6                     ; /* 0x438 */
    U_VPSS_NXT1_CTRL       VPSS_NXT1_CTRL                    ; /* 0x43c */
    U_VPSS_NXT1YADDR       VPSS_NXT1YADDR                    ; /* 0x440 */
    U_VPSS_NXT1CADDR       VPSS_NXT1CADDR                    ; /* 0x444 */
    U_VPSS_NXT1STRIDE      VPSS_NXT1STRIDE                   ; /* 0x448 */
    U_VPSS_CUR_CTRL        VPSS_CUR_CTRL                     ; /* 0x44c */
    U_VPSS_CURYADDR        VPSS_CURYADDR                     ; /* 0x450 */
    U_VPSS_CURCADDR        VPSS_CURCADDR                     ; /* 0x454 */
    U_VPSS_CURSTRIDE       VPSS_CURSTRIDE                    ; /* 0x458 */
    U_VPSS_REF_CTRL        VPSS_REF_CTRL                     ; /* 0x45c */
    U_VPSS_REFYADDR        VPSS_REFYADDR                     ; /* 0x460 */
    U_VPSS_REFCADDR        VPSS_REFCADDR                     ; /* 0x464 */
    U_VPSS_REFSTRIDE       VPSS_REFSTRIDE                    ; /* 0x468 */
    U_VPSS_REE_CTRL        VPSS_REE_CTRL                     ; /* 0x46c */
    U_VPSS_REEYADDR        VPSS_REEYADDR                     ; /* 0x470 */
    U_VPSS_REECADDR        VPSS_REECADDR                     ; /* 0x474 */
    U_VPSS_REESTRIDE       VPSS_REESTRIDE                    ; /* 0x478 */
    U_VPSS_PRJVCURADDR     VPSS_PRJVCURADDR                  ; /* 0x47c */
    U_VPSS_PRJHCURADDR     VPSS_PRJHCURADDR                  ; /* 0x480 */
    U_VPSS_PRJCURSTRIDE    VPSS_PRJCURSTRIDE                 ; /* 0x484 */
    U_VPSS_RGMVCURADDR     VPSS_RGMVCURADDR                  ; /* 0x488 */
    U_VPSS_RGMVNX1ADDR     VPSS_RGMVNX1ADDR                  ; /* 0x48c */
    U_VPSS_RGMVSTRIDE      VPSS_RGMVSTRIDE                   ; /* 0x490 */
    U_VPSS_BLKMVCURADDR    VPSS_BLKMVCURADDR                 ; /* 0x494 */
    U_VPSS_BLKMVREFADDR    VPSS_BLKMVREFADDR                 ; /* 0x498 */
    U_VPSS_BLKMVSTRIDE     VPSS_BLKMVSTRIDE                  ; /* 0x49c */
    U_VPSS_BLKMTREFADDR    VPSS_BLKMTREFADDR                 ; /* 0x4a0 */
    U_VPSS_BLKMTREFSTRIDE   VPSS_BLKMTREFSTRIDE              ; /* 0x4a4 */
    U_VPSS_MAD_RADDR       VPSS_MAD_RADDR                    ; /* 0x4a8 */
    U_VPSS_SNR_MAD_RADDR   VPSS_SNR_MAD_RADDR                ; /* 0x4ac */
    U_VPSS_MADSTRIDE       VPSS_MADSTRIDE                    ; /* 0x4b0 */
    U_VPSS_STRADDR         VPSS_STRADDR                      ; /* 0x4b4 */
    U_VPSS_STSTRIDE        VPSS_STSTRIDE                     ; /* 0x4b8 */
    U_VPSS_TRANS_SRC_ADDR   VPSS_TRANS_SRC_ADDR              ; /* 0x4bc */
    U_VPSS_TRANS_DES_ADDR   VPSS_TRANS_DES_ADDR              ; /* 0x4c0 */
    U_VPSS_TRANS_INFO      VPSS_TRANS_INFO                   ; /* 0x4c4 */
    U_VPSS_VHD0CTRL        VPSS_VHD0CTRL                     ; /* 0x4c8 */
    U_VPSS_VHD0SIZE        VPSS_VHD0SIZE                     ; /* 0x4cc */
    U_VPSS_VHD0YADDR       VPSS_VHD0YADDR                    ; /* 0x4d0 */
    U_VPSS_VHD0CADDR       VPSS_VHD0CADDR                    ; /* 0x4d4 */
    U_VPSS_VHD0STRIDE      VPSS_VHD0STRIDE                   ; /* 0x4d8 */
    U_VPSS_RFRCTRL         VPSS_RFRCTRL                      ; /* 0x4dc */
    U_VPSS_RFRYADDR        VPSS_RFRYADDR                     ; /* 0x4e0 */
    U_VPSS_RFRCADDR        VPSS_RFRCADDR                     ; /* 0x4e4 */
    U_VPSS_RFRSTRIDE       VPSS_RFRSTRIDE                    ; /* 0x4e8 */
    U_VPSS_CUE_CTRL        VPSS_CUE_CTRL                     ; /* 0x4ec */
    U_VPSS_CUEYADDR        VPSS_CUEYADDR                     ; /* 0x4f0 */
    U_VPSS_CUECADDR        VPSS_CUECADDR                     ; /* 0x4f4 */
    U_VPSS_CUESTRIDE       VPSS_CUESTRIDE                    ; /* 0x4f8 */
    U_VPSS_PRJVNX2ADDR     VPSS_PRJVNX2ADDR                  ; /* 0x4fc */
    U_VPSS_PRJHNX2ADDR     VPSS_PRJHNX2ADDR                  ; /* 0x500 */
    U_VPSS_PRJNX2STRIDE    VPSS_PRJNX2STRIDE                 ; /* 0x504 */
    U_VPSS_BLKMVNX1ADDR    VPSS_BLKMVNX1ADDR                 ; /* 0x508 */
    U_VPSS_BLKMVNX1STRIDE   VPSS_BLKMVNX1STRIDE              ; /* 0x50c */
    U_VPSS_RGMVNX2ADDR     VPSS_RGMVNX2ADDR                  ; /* 0x510 */
    U_VPSS_RGMVNX2STRIDE   VPSS_RGMVNX2STRIDE                ; /* 0x514 */
    U_VPSS_BLKMTCURADDR    VPSS_BLKMTCURADDR                 ; /* 0x518 */
    U_VPSS_BLKMTCURSTRIDE   VPSS_BLKMTCURSTRIDE              ; /* 0x51c */
    U_VPSS_CHK_SUM_W_ADDR   VPSS_CHK_SUM_W_ADDR              ; /* 0x520 */
    U_VPSS_CHK_SUM_SEL     VPSS_CHK_SUM_SEL                  ; /* 0x524 */
    U_VPSS_STWADDR         VPSS_STWADDR                      ; /* 0x528 */
    U_VPSS_MAD_WADDR       VPSS_MAD_WADDR                    ; /* 0x52c */
    U_VPSS_STT_W_ADDR      VPSS_STT_W_ADDR                   ; /* 0x530 */
    U_VPSS_NX2Y_LB_ADDR    VPSS_NX2Y_LB_ADDR                 ; /* 0x534 */
    U_VPSS_NX2C_LB_ADDR    VPSS_NX2C_LB_ADDR                 ; /* 0x538 */
    U_VPSS_NX2_LB_STRIDE   VPSS_NX2_LB_STRIDE                ; /* 0x53c */
    U_VPSS_VHD0_TUNL_CTRL   VPSS_VHD0_TUNL_CTRL              ; /* 0x540 */
    U_VPSS_VHD0_TUNL_ADDR   VPSS_VHD0_TUNL_ADDR              ; /* 0x544 */
    U_VPSS_DICNT_SIZE      VPSS_DICNT_SIZE                   ; /* 0x548 */
    U_VPSS_DIRCNT_ADDR     VPSS_DIRCNT_ADDR                  ; /* 0x54c */
    U_VPSS_DIWCNT_ADDR     VPSS_DIWCNT_ADDR                  ; /* 0x550 */
    U_VPSS_DICNT_STRIDE    VPSS_DICNT_STRIDE                 ; /* 0x554 */
    unsigned int           reserved_7[11]                     ; /* 0x558~0x580 */
    U_VPSS_RTT_CFG         VPSS_RTT_CFG                      ; /* 0x584 */
    U_VPSS_VHD0CROP_SIZE   VPSS_VHD0CROP_SIZE                ; /* 0x588 */
    U_VPSS_VHD0CROP_POS    VPSS_VHD0CROP_POS                 ; /* 0x58c */
    U_VPSS_VHD0LBA_DSIZE   VPSS_VHD0LBA_DSIZE                ; /* 0x590 */
    U_VPSS_VHD0LBA_VFPOS   VPSS_VHD0LBA_VFPOS                ; /* 0x594 */
    U_VPSS_VHD0LBA_BK      VPSS_VHD0LBA_BK                   ; /* 0x598 */
    U_VPSS_NX2_VC1_CTRL0   VPSS_NX2_VC1_CTRL0                ; /* 0x59c */
    unsigned int           reserved_8[664]                     ; /* 0x5a0~0xffc */
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
    U_VPSS_DIEINTPSCL0     VPSS_DIEINTPSCL0                  ; /* 0x102c */
    U_VPSS_DIEINTPSCL1     VPSS_DIEINTPSCL1                  ; /* 0x1030 */
    U_VPSS_DIEDIRTHD       VPSS_DIEDIRTHD                    ; /* 0x1034 */
    U_VPSS_DIEJITMTN       VPSS_DIEJITMTN                    ; /* 0x1038 */
    U_VPSS_DIEFLDMTN       VPSS_DIEFLDMTN                    ; /* 0x103c */
    U_VPSS_DIEMTNDIFFTHD0   VPSS_DIEMTNDIFFTHD0              ; /* 0x1040 */
    U_VPSS_DIEMTNDIFFTHD1   VPSS_DIEMTNDIFFTHD1              ; /* 0x1044 */
    U_VPSS_DIEMTNIIRSLP    VPSS_DIEMTNIIRSLP                 ; /* 0x1048 */
    U_VPSS_DIEMTNIIRRAT    VPSS_DIEMTNIIRRAT                 ; /* 0x104c */
    U_VPSS_DIEHISMODE      VPSS_DIEHISMODE                   ; /* 0x1050 */
    U_VPSS_DIEMORFLT       VPSS_DIEMORFLT                    ; /* 0x1054 */
    U_VPSS_DIECOMBCHK0     VPSS_DIECOMBCHK0                  ; /* 0x1058 */
    U_VPSS_DIECOMBCHK1     VPSS_DIECOMBCHK1                  ; /* 0x105c */
    U_VPSS_DIEFRMMTNSMTHTHD0   VPSS_DIEFRMMTNSMTHTHD0        ; /* 0x1060 */
    U_VPSS_DIEFRMMTNSMTHTHD1   VPSS_DIEFRMMTNSMTHTHD1        ; /* 0x1064 */
    U_VPSS_DIEFRMMTNSMTHSLP   VPSS_DIEFRMMTNSMTHSLP          ; /* 0x1068 */
    U_VPSS_DIEFRMMTNSMTHRAT   VPSS_DIEFRMMTNSMTHRAT          ; /* 0x106c */
    U_VPSS_DIEFRMFLDBLENDTHD0   VPSS_DIEFRMFLDBLENDTHD0      ; /* 0x1070 */
    U_VPSS_DIEFRMFLDBLENDTHD1   VPSS_DIEFRMFLDBLENDTHD1      ; /* 0x1074 */
    U_VPSS_DIEFRMFLDBLENDSLP   VPSS_DIEFRMFLDBLENDSLP        ; /* 0x1078 */
    U_VPSS_DIEFRMFLDBLENDRAT   VPSS_DIEFRMFLDBLENDRAT        ; /* 0x107c */
    U_VPSS_DIEMTNADJ       VPSS_DIEMTNADJ                    ; /* 0x1080 */
    U_VPSS_DIEEDGENORM0    VPSS_DIEEDGENORM0                 ; /* 0x1084 */
    U_VPSS_DIEEDGENORM1    VPSS_DIEEDGENORM1                 ; /* 0x1088 */
    U_VPSS_DIEEDGENORM2    VPSS_DIEEDGENORM2                 ; /* 0x108c */
    U_VPSS_DIEEDGENORM3    VPSS_DIEEDGENORM3                 ; /* 0x1090 */
    U_VPSS_DIEEDGENORM4    VPSS_DIEEDGENORM4                 ; /* 0x1094 */
    U_VPSS_DIEEDGENORM5    VPSS_DIEEDGENORM5                 ; /* 0x1098 */
    U_VPSS_DIEEDGEFORMC    VPSS_DIEEDGEFORMC                 ; /* 0x109c */
    U_VPSS_DIEMCSTRENGTH0   VPSS_DIEMCSTRENGTH0              ; /* 0x10a0 */
    U_VPSS_DIEMCSTRENGTH1   VPSS_DIEMCSTRENGTH1              ; /* 0x10a4 */
    U_VPSS_DIEFUSION0      VPSS_DIEFUSION0                   ; /* 0x10a8 */
    U_VPSS_DIEFUSION1      VPSS_DIEFUSION1                   ; /* 0x10ac */
    U_VPSS_DIEFUSION2      VPSS_DIEFUSION2                   ; /* 0x10b0 */
    U_VPSS_DIEFUSION3      VPSS_DIEFUSION3                   ; /* 0x10b4 */
    U_VPSS_DIEMAGBMTHD0    VPSS_DIEMAGBMTHD0                 ; /* 0x10b8 */
    U_VPSS_DIEMAGBMTHD1    VPSS_DIEMAGBMTHD1                 ; /* 0x10bc */
    U_VPSS_DIEMCGBMCOEF0   VPSS_DIEMCGBMCOEF0                ; /* 0x10c0 */
    U_VPSS_DIEMCGBMCOEF1   VPSS_DIEMCGBMCOEF1                ; /* 0x10c4 */
    U_VPSS_DIEKMAG1        VPSS_DIEKMAG1                     ; /* 0x10c8 */
    U_VPSS_DIEKMAG2        VPSS_DIEKMAG2                     ; /* 0x10cc */
    U_VPSS_DIE_S3ADD       VPSS_DIE_S3ADD                    ; /* 0x10d0 */
    U_VPSS_PDPHISTTHD1     VPSS_PDPHISTTHD1                  ; /* 0x10d4 */
    U_VPSS_PDPCCMOV        VPSS_PDPCCMOV                     ; /* 0x10d8 */
    U_VPSS_PDCTRL          VPSS_PDCTRL                       ; /* 0x10dc */
    U_VPSS_PDBLKTHD        VPSS_PDBLKTHD                     ; /* 0x10e0 */
    U_VPSS_PDUMTHD         VPSS_PDUMTHD                      ; /* 0x10e4 */
    U_VPSS_PDPCCCORING     VPSS_PDPCCCORING                  ; /* 0x10e8 */
    U_VPSS_PDPCCHTHD       VPSS_PDPCCHTHD                    ; /* 0x10ec */
    U_VPSS_PDPCCVTHD       VPSS_PDPCCVTHD                    ; /* 0x10f0 */
    U_VPSS_PDITDIFFVTHD    VPSS_PDITDIFFVTHD                 ; /* 0x10f4 */
    U_VPSS_PDLASITHD       VPSS_PDLASITHD                    ; /* 0x10f8 */
    U_VPSS_PDLASIMODE0COEF   VPSS_PDLASIMODE0COEF            ; /* 0x10fc */
    U_VPSS_PDLASIMODE1COEF   VPSS_PDLASIMODE1COEF            ; /* 0x1100 */
    U_VPSS_PDREGION        VPSS_PDREGION                     ; /* 0x1104 */
    U_VPSS_MCDI_RGDIFY     VPSS_MCDI_RGDIFY                  ; /* 0x1108 */
    U_VPSS_MCDI_RGSAD      VPSS_MCDI_RGSAD                   ; /* 0x110c */
    U_VPSS_MCDI_RGMV       VPSS_MCDI_RGMV                    ; /* 0x1110 */
    U_VPSS_MCDI_RGMAG0     VPSS_MCDI_RGMAG0                  ; /* 0x1114 */
    U_VPSS_MCDI_RGMAG1     VPSS_MCDI_RGMAG1                  ; /* 0x1118 */
    U_VPSS_MCDI_RGLS       VPSS_MCDI_RGLS                    ; /* 0x111c */
    U_VPSS_MCDI_SEL        VPSS_MCDI_SEL                     ; /* 0x1120 */
    U_VPSS_MCDI_DLT0       VPSS_MCDI_DLT0                    ; /* 0x1124 */
    U_VPSS_MCDI_DLT1       VPSS_MCDI_DLT1                    ; /* 0x1128 */
    U_VPSS_MCDI_RGMCW0     VPSS_MCDI_RGMCW0                  ; /* 0x112c */
    U_VPSS_MCDI_RGMCW1     VPSS_MCDI_RGMCW1                  ; /* 0x1130 */
    U_VPSS_MCDI_RGMCW2     VPSS_MCDI_RGMCW2                  ; /* 0x1134 */
    U_VPSS_MCDI_RGMCW3     VPSS_MCDI_RGMCW3                  ; /* 0x1138 */
    U_VPSS_MCDI_RGMCW4     VPSS_MCDI_RGMCW4                  ; /* 0x113c */
    U_VPSS_MCDI_RGMCW5     VPSS_MCDI_RGMCW5                  ; /* 0x1140 */
    U_VPSS_MCDI_RGMCW6     VPSS_MCDI_RGMCW6                  ; /* 0x1144 */
    U_VPSS_MCDI_TPMCW0     VPSS_MCDI_TPMCW0                  ; /* 0x1148 */
    U_VPSS_MCDI_TPMCW1     VPSS_MCDI_TPMCW1                  ; /* 0x114c */
    U_VPSS_MCDI_WNDMCW0    VPSS_MCDI_WNDMCW0                 ; /* 0x1150 */
    U_VPSS_MCDI_WNDMCW1    VPSS_MCDI_WNDMCW1                 ; /* 0x1154 */
    U_VPSS_MCDI_WNDMCW2    VPSS_MCDI_WNDMCW2                 ; /* 0x1158 */
    U_VPSS_MCDI_WNDMCW3    VPSS_MCDI_WNDMCW3                 ; /* 0x115c */
    U_VPSS_MCDI_VERTWEIGHT0   VPSS_MCDI_VERTWEIGHT0          ; /* 0x1160 */
    U_VPSS_MCDI_VERTWEIGHT1   VPSS_MCDI_VERTWEIGHT1          ; /* 0x1164 */
    U_VPSS_MCDI_VERTWEIGHT2   VPSS_MCDI_VERTWEIGHT2          ; /* 0x1168 */
    U_VPSS_MCDI_VERTWEIGHT3   VPSS_MCDI_VERTWEIGHT3          ; /* 0x116c */
    U_VPSS_MCDI_VERTWEIGHT4   VPSS_MCDI_VERTWEIGHT4          ; /* 0x1170 */
    U_VPSS_MCDI_VERTWEIGHT5   VPSS_MCDI_VERTWEIGHT5          ; /* 0x1174 */
    U_VPSS_MCDI_MC0        VPSS_MCDI_MC0                     ; /* 0x1178 */
    U_VPSS_MCDI_MC1        VPSS_MCDI_MC1                     ; /* 0x117c */
    U_VPSS_MCDI_MC2        VPSS_MCDI_MC2                     ; /* 0x1180 */
    U_VPSS_MCDI_MC3        VPSS_MCDI_MC3                     ; /* 0x1184 */
    U_VPSS_MCDI_MC4        VPSS_MCDI_MC4                     ; /* 0x1188 */
    U_VPSS_MCDI_MC5        VPSS_MCDI_MC5                     ; /* 0x118c */
    U_VPSS_MCDI_MC6        VPSS_MCDI_MC6                     ; /* 0x1190 */
    U_VPSS_MCDI_MC7        VPSS_MCDI_MC7                     ; /* 0x1194 */
    U_VPSS_MCDI_LCDINEW0   VPSS_MCDI_LCDINEW0                ; /* 0x1198 */
    U_VPSS_MCDI_LCDINEW1   VPSS_MCDI_LCDINEW1                ; /* 0x119c */
    U_VPSS_MCDI_NUMT       VPSS_MCDI_NUMT                    ; /* 0x11a0 */
    U_VPSS_MCDI_S3ADD0     VPSS_MCDI_S3ADD0                  ; /* 0x11a4 */
    U_VPSS_MCDI_S3ADD1     VPSS_MCDI_S3ADD1                  ; /* 0x11a8 */
    U_VPSS_MCDI_S3ADD2     VPSS_MCDI_S3ADD2                  ; /* 0x11ac */
    U_VPSS_MCDI_S3ADD3     VPSS_MCDI_S3ADD3                  ; /* 0x11b0 */
    U_VPSS_MCDI_S3ADD4     VPSS_MCDI_S3ADD4                  ; /* 0x11b4 */
    U_VPSS_MCDI_S3ADD5     VPSS_MCDI_S3ADD5                  ; /* 0x11b8 */
    U_VPSS_MCDI_S3ADD6     VPSS_MCDI_S3ADD6                  ; /* 0x11bc */
    U_VPSS_MCDI_S3ADD7     VPSS_MCDI_S3ADD7                  ; /* 0x11c0 */
    U_VPSS_MCDI_STILLCNT0   VPSS_MCDI_STILLCNT0              ; /* 0x11c4 */
    U_VPSS_MCDI_STILLCNT1   VPSS_MCDI_STILLCNT1              ; /* 0x11c8 */
    U_VPSS_MCDI_DEMO       VPSS_MCDI_DEMO                    ; /* 0x11cc */
    unsigned int           reserved_9[908]                     ; /* 0x11d0~0x1ffc */
    U_VPSS_VHD0_HSP        VPSS_VHD0_HSP                     ; /* 0x2000 */
    U_VPSS_VHD0_HLOFFSET   VPSS_VHD0_HLOFFSET                ; /* 0x2004 */
    U_VPSS_VHD0_HCOFFSET   VPSS_VHD0_HCOFFSET                ; /* 0x2008 */
    U_VPSS_VHD0_VSP        VPSS_VHD0_VSP                     ; /* 0x200c */
    U_VPSS_VHD0_VSR        VPSS_VHD0_VSR                     ; /* 0x2010 */
    U_VPSS_VHD0_VOFFSET    VPSS_VHD0_VOFFSET                 ; /* 0x2014 */
    unsigned int           reserved_10[58]                     ; /* 0x2018~0x20fc */
    U_VPSS_SNR_CTRL        VPSS_SNR_CTRL                     ; /* 0x2100 */
    U_VPSS_DBM_DEMO        VPSS_DBM_DEMO                     ; /* 0x2104 */
    U_VPSS_DB_DIR          VPSS_DB_DIR                       ; /* 0x2108 */
    U_VPSS_DB_BLK          VPSS_DB_BLK                       ; /* 0x210c */
    U_VPSS_DB_RATIO        VPSS_DB_RATIO                     ; /* 0x2110 */
    U_VPSS_DB_LHHF         VPSS_DB_LHHF                      ; /* 0x2114 */
    U_VPSS_DB_LVHF         VPSS_DB_LVHF                      ; /* 0x2118 */
    U_VPSS_DB_CTRS         VPSS_DB_CTRS                      ; /* 0x211c */
    U_VPSS_DB_LUT0         VPSS_DB_LUT0                      ; /* 0x2120 */
    U_VPSS_DB_LUT1         VPSS_DB_LUT1                      ; /* 0x2124 */
    U_VPSS_DB_LUT2         VPSS_DB_LUT2                      ; /* 0x2128 */
    U_VPSS_DB_LUT3         VPSS_DB_LUT3                      ; /* 0x212c */
    U_VPSS_DB_LUT4         VPSS_DB_LUT4                      ; /* 0x2130 */
    U_VPSS_DB_LUT5         VPSS_DB_LUT5                      ; /* 0x2134 */
    U_VPSS_DB_LUT6         VPSS_DB_LUT6                      ; /* 0x2138 */
    U_VPSS_DB_ADPT         VPSS_DB_ADPT                      ; /* 0x213c */
    unsigned int           reserved_11[4]                     ; /* 0x2140~0x214c */
    U_VPSS_DBD_THDEDGE     VPSS_DBD_THDEDGE                  ; /* 0x2150 */
    U_VPSS_DBD_THDTXT      VPSS_DBD_THDTXT                   ; /* 0x2154 */
    U_VPSS_DBD_LUTWGT0     VPSS_DBD_LUTWGT0                  ; /* 0x2158 */
    U_VPSS_DBD_LUTWGT1     VPSS_DBD_LUTWGT1                  ; /* 0x215c */
    U_VPSS_DBD_RTNBDTXT    VPSS_DBD_RTNBDTXT                 ; /* 0x2160 */
    U_VPSS_DBD_TSTBLKNUM   VPSS_DBD_TSTBLKNUM                ; /* 0x2164 */
    U_VPSS_DBD_HYRDX8BINLUT0   VPSS_DBD_HYRDX8BINLUT0        ; /* 0x2168 */
    U_VPSS_DBD_HYRDX8BINLUT1   VPSS_DBD_HYRDX8BINLUT1        ; /* 0x216c */
    U_VPSS_DBD_HYRDX8BINLUT2   VPSS_DBD_HYRDX8BINLUT2        ; /* 0x2170 */
    U_VPSS_DBD_HYRDX8BINLUT3   VPSS_DBD_HYRDX8BINLUT3        ; /* 0x2174 */
    U_VPSS_DBD_BLKSIZE     VPSS_DBD_BLKSIZE                  ; /* 0x2178 */
    U_VPSS_DBD_FLAT        VPSS_DBD_FLAT                     ; /* 0x217c */
    unsigned int           reserved_12[4]                     ; /* 0x2180~0x218c */
    U_VPSS_DM_DIR          VPSS_DM_DIR                       ; /* 0x2190 */
    U_VPSS_DM_EDGE         VPSS_DM_EDGE                      ; /* 0x2194 */
    U_VPSS_DM_LUT0         VPSS_DM_LUT0                      ; /* 0x2198 */
    U_VPSS_DM_LUT1         VPSS_DM_LUT1                      ; /* 0x219c */
    U_VPSS_DM_LUT2         VPSS_DM_LUT2                      ; /* 0x21a0 */
    U_VPSS_DM_LUT3         VPSS_DM_LUT3                      ; /* 0x21a4 */
    U_VPSS_DM_LUT4         VPSS_DM_LUT4                      ; /* 0x21a8 */
    U_VPSS_DM_DIRC         VPSS_DM_DIRC                      ; /* 0x21ac */
    U_VPSS_DM_DIRO         VPSS_DM_DIRO                      ; /* 0x21b0 */
    U_VPSS_DM_LSW          VPSS_DM_LSW                       ; /* 0x21b4 */
    U_VPSS_DM_MMFLR        VPSS_DM_MMFLR                     ; /* 0x21b8 */
    U_VPSS_DM_MMFSR        VPSS_DM_MMFSR                     ; /* 0x21bc */
    U_VPSS_DM_THR0         VPSS_DM_THR0                      ; /* 0x21c0 */
    U_VPSS_DM_LUT5         VPSS_DM_LUT5                      ; /* 0x21c4 */
    U_VPSS_DM_LUT6         VPSS_DM_LUT6                      ; /* 0x21c8 */
    U_VPSS_DM_LUT7         VPSS_DM_LUT7                      ; /* 0x21cc */
    U_VPSS_DM_LUT8         VPSS_DM_LUT8                      ; /* 0x21d0 */
    U_VPSS_DM_LUT9         VPSS_DM_LUT9                      ; /* 0x21d4 */
    U_VPSS_DM_LUT10        VPSS_DM_LUT10                     ; /* 0x21d8 */
    U_VPSS_DM_LUT11        VPSS_DM_LUT11                     ; /* 0x21dc */
    U_VPSS_DM_LUT12        VPSS_DM_LUT12                     ; /* 0x21e0 */
    U_VPSS_DM_LUT13        VPSS_DM_LUT13                     ; /* 0x21e4 */
    U_VPSS_DM_LUT14        VPSS_DM_LUT14                     ; /* 0x21e8 */
    U_VPSS_DM_LUT15        VPSS_DM_LUT15                     ; /* 0x21ec */
    U_VPSS_DM_LUT16        VPSS_DM_LUT16                     ; /* 0x21f0 */
    U_VPSS_DM_LUT17        VPSS_DM_LUT17                     ; /* 0x21f4 */
    U_VPSS_DM_TRANS_WID    VPSS_DM_TRANS_WID                 ; /* 0x21f8 */
    unsigned int           reserved_13                     ; /* 0x21fc */
    U_VPSS_DB_BORD_FLAG0   VPSS_DB_BORD_FLAG0                ; /* 0x2200 */
    U_VPSS_DB_BORD_FLAG1   VPSS_DB_BORD_FLAG1                ; /* 0x2204 */
    U_VPSS_DB_BORD_FLAG2   VPSS_DB_BORD_FLAG2                ; /* 0x2208 */
    U_VPSS_DB_BORD_FLAG3   VPSS_DB_BORD_FLAG3                ; /* 0x220c */
    U_VPSS_DB_BORD_FLAG4   VPSS_DB_BORD_FLAG4                ; /* 0x2210 */
    U_VPSS_DB_BORD_FLAG5   VPSS_DB_BORD_FLAG5                ; /* 0x2214 */
    U_VPSS_DB_BORD_FLAG6   VPSS_DB_BORD_FLAG6                ; /* 0x2218 */
    U_VPSS_DB_BORD_FLAG7   VPSS_DB_BORD_FLAG7                ; /* 0x221c */
    U_VPSS_DB_BORD_FLAG8   VPSS_DB_BORD_FLAG8                ; /* 0x2220 */
    U_VPSS_DB_BORD_FLAG9   VPSS_DB_BORD_FLAG9                ; /* 0x2224 */
    U_VPSS_DB_BORD_FLAG10   VPSS_DB_BORD_FLAG10              ; /* 0x2228 */
    U_VPSS_DB_BORD_FLAG11   VPSS_DB_BORD_FLAG11              ; /* 0x222c */
    U_VPSS_DB_BORD_FLAG12   VPSS_DB_BORD_FLAG12              ; /* 0x2230 */
    U_VPSS_DB_BORD_FLAG13   VPSS_DB_BORD_FLAG13              ; /* 0x2234 */
    U_VPSS_DB_BORD_FLAG14   VPSS_DB_BORD_FLAG14              ; /* 0x2238 */
    U_VPSS_DB_BORD_FLAG15   VPSS_DB_BORD_FLAG15              ; /* 0x223c */
    U_VPSS_DB_BORD_FLAG16   VPSS_DB_BORD_FLAG16              ; /* 0x2240 */
    U_VPSS_DB_BORD_FLAG17   VPSS_DB_BORD_FLAG17              ; /* 0x2244 */
    U_VPSS_DB_BORD_FLAG18   VPSS_DB_BORD_FLAG18              ; /* 0x2248 */
    U_VPSS_DB_BORD_FLAG19   VPSS_DB_BORD_FLAG19              ; /* 0x224c */
    U_VPSS_DB_BORD_FLAG20   VPSS_DB_BORD_FLAG20              ; /* 0x2250 */
    U_VPSS_DB_BORD_FLAG21   VPSS_DB_BORD_FLAG21              ; /* 0x2254 */
    U_VPSS_DB_BORD_FLAG22   VPSS_DB_BORD_FLAG22              ; /* 0x2258 */
    U_VPSS_DB_BORD_FLAG23   VPSS_DB_BORD_FLAG23              ; /* 0x225c */
    U_VPSS_DB_BORD_FLAG24   VPSS_DB_BORD_FLAG24              ; /* 0x2260 */
    U_VPSS_DB_BORD_FLAG25   VPSS_DB_BORD_FLAG25              ; /* 0x2264 */
    U_VPSS_DB_BORD_FLAG26   VPSS_DB_BORD_FLAG26              ; /* 0x2268 */
    U_VPSS_DB_BORD_FLAG27   VPSS_DB_BORD_FLAG27              ; /* 0x226c */
    U_VPSS_DB_BORD_FLAG28   VPSS_DB_BORD_FLAG28              ; /* 0x2270 */
    U_VPSS_DB_BORD_FLAG29   VPSS_DB_BORD_FLAG29              ; /* 0x2274 */
    U_VPSS_DB_BORD_FLAG30   VPSS_DB_BORD_FLAG30              ; /* 0x2278 */
    U_VPSS_DB_BORD_FLAG31   VPSS_DB_BORD_FLAG31              ; /* 0x227c */
    U_VPSS_DB_BORD_FLAG32   VPSS_DB_BORD_FLAG32              ; /* 0x2280 */
    U_VPSS_DB_BORD_FLAG33   VPSS_DB_BORD_FLAG33              ; /* 0x2284 */
    U_VPSS_DB_BORD_FLAG34   VPSS_DB_BORD_FLAG34              ; /* 0x2288 */
    U_VPSS_DB_BORD_FLAG35   VPSS_DB_BORD_FLAG35              ; /* 0x228c */
    U_VPSS_DB_BORD_FLAG36   VPSS_DB_BORD_FLAG36              ; /* 0x2290 */
    U_VPSS_DB_BORD_FLAG37   VPSS_DB_BORD_FLAG37              ; /* 0x2294 */
    U_VPSS_DB_BORD_FLAG38   VPSS_DB_BORD_FLAG38              ; /* 0x2298 */
    U_VPSS_DB_BORD_FLAG39   VPSS_DB_BORD_FLAG39              ; /* 0x229c */
    U_VPSS_DB_BORD_FLAG40   VPSS_DB_BORD_FLAG40              ; /* 0x22a0 */
    U_VPSS_DB_BORD_FLAG41   VPSS_DB_BORD_FLAG41              ; /* 0x22a4 */
    U_VPSS_DB_BORD_FLAG42   VPSS_DB_BORD_FLAG42              ; /* 0x22a8 */
    U_VPSS_DB_BORD_FLAG43   VPSS_DB_BORD_FLAG43              ; /* 0x22ac */
    U_VPSS_DB_BORD_FLAG44   VPSS_DB_BORD_FLAG44              ; /* 0x22b0 */
    U_VPSS_DB_BORD_FLAG45   VPSS_DB_BORD_FLAG45              ; /* 0x22b4 */
    U_VPSS_DB_BORD_FLAG46   VPSS_DB_BORD_FLAG46              ; /* 0x22b8 */
    U_VPSS_DB_BORD_FLAG47   VPSS_DB_BORD_FLAG47              ; /* 0x22bc */
    U_VPSS_DB_BORD_FLAG48   VPSS_DB_BORD_FLAG48              ; /* 0x22c0 */
    U_VPSS_DB_BORD_FLAG49   VPSS_DB_BORD_FLAG49              ; /* 0x22c4 */
    U_VPSS_DB_BORD_FLAG50   VPSS_DB_BORD_FLAG50              ; /* 0x22c8 */
    U_VPSS_DB_BORD_FLAG51   VPSS_DB_BORD_FLAG51              ; /* 0x22cc */
    U_VPSS_DB_BORD_FLAG52   VPSS_DB_BORD_FLAG52              ; /* 0x22d0 */
    U_VPSS_DB_BORD_FLAG53   VPSS_DB_BORD_FLAG53              ; /* 0x22d4 */
    U_VPSS_DB_BORD_FLAG54   VPSS_DB_BORD_FLAG54              ; /* 0x22d8 */
    U_VPSS_DB_BORD_FLAG55   VPSS_DB_BORD_FLAG55              ; /* 0x22dc */
    U_VPSS_DB_BORD_FLAG56   VPSS_DB_BORD_FLAG56              ; /* 0x22e0 */
    U_VPSS_DB_BORD_FLAG57   VPSS_DB_BORD_FLAG57              ; /* 0x22e4 */
    U_VPSS_DB_BORD_FLAG58   VPSS_DB_BORD_FLAG58              ; /* 0x22e8 */
    U_VPSS_DB_BORD_FLAG59   VPSS_DB_BORD_FLAG59              ; /* 0x22ec */
    U_VPSS_DB_BORD_FLAG60   VPSS_DB_BORD_FLAG60              ; /* 0x22f0 */
    U_VPSS_DB_BORD_FLAG61   VPSS_DB_BORD_FLAG61              ; /* 0x22f4 */
    U_VPSS_DB_BORD_FLAG62   VPSS_DB_BORD_FLAG62              ; /* 0x22f8 */
    U_VPSS_DB_BORD_FLAG63   VPSS_DB_BORD_FLAG63              ; /* 0x22fc */
    U_VPSS_SNR_ENABLE      VPSS_SNR_ENABLE                   ; /* 0x2300 */
    U_VPSS_SNR_EDGE_RATIORANGE   VPSS_SNR_EDGE_RATIORANGE    ; /* 0x2304 */
    U_VPSS_SNR_EDGE_STRRANGE   VPSS_SNR_EDGE_STRRANGE        ; /* 0x2308 */
    U_VPSS_SNR_EDGE_STRTH   VPSS_SNR_EDGE_STRTH              ; /* 0x230c */
    U_VPSS_SNR_EDGE_STRK   VPSS_SNR_EDGE_STRK                ; /* 0x2310 */
    U_VPSS_SNR_EDGE_MEANTH1   VPSS_SNR_EDGE_MEANTH1          ; /* 0x2314 */
    U_VPSS_SNR_EDGE_MEANTH2   VPSS_SNR_EDGE_MEANTH2          ; /* 0x2318 */
    U_VPSS_SNR_EDGE_MEANTH3   VPSS_SNR_EDGE_MEANTH3          ; /* 0x231c */
    U_VPSS_SNR_EDGE_MEANK1   VPSS_SNR_EDGE_MEANK1            ; /* 0x2320 */
    U_VPSS_SNR_EDGE_MEANK2   VPSS_SNR_EDGE_MEANK2            ; /* 0x2324 */
    U_VPSS_SNR_WIN         VPSS_SNR_WIN                      ; /* 0x2328 */
    U_VPSS_SNR_STR         VPSS_SNR_STR                      ; /* 0x232c */
    U_VPSS_SNR_STR_OFFSET   VPSS_SNR_STR_OFFSET              ; /* 0x2330 */
    U_VPSS_SNR_STR_ADJUST   VPSS_SNR_STR_ADJUST              ; /* 0x2334 */
    U_VPSS_SNR_SCHANGE_TH   VPSS_SNR_SCHANGE_TH              ; /* 0x2338 */
    U_VPSS_SNR_EDGE_RATIO_1   VPSS_SNR_EDGE_RATIO_1          ; /* 0x233c */
    U_VPSS_SNR_EDGE_RATIO_2   VPSS_SNR_EDGE_RATIO_2          ; /* 0x2340 */
    U_VPSS_SNR_EDGE_RATIO_3   VPSS_SNR_EDGE_RATIO_3          ; /* 0x2344 */
    U_VPSS_SNR_EDGE_STR    VPSS_SNR_EDGE_STR                 ; /* 0x2348 */
    U_VPSS_SNR_TEST_COLOR   VPSS_SNR_TEST_COLOR              ; /* 0x234c */
    U_VPSS_SNR_SCHANGE     VPSS_SNR_SCHANGE                  ; /* 0x2350 */
    U_VPSS_SNR_MT          VPSS_SNR_MT                       ; /* 0x2354 */
    U_VPSS_SNR_CBCR_W1     VPSS_SNR_CBCR_W1                  ; /* 0x2358 */
    U_VPSS_SNR_CBCR_W2     VPSS_SNR_CBCR_W2                  ; /* 0x235c */
    U_MOTION_EDGE_LUT_01   MOTION_EDGE_LUT_01                ; /* 0x2360 */
    U_MOTION_EDGE_LUT_02   MOTION_EDGE_LUT_02                ; /* 0x2364 */
    U_MOTION_EDGE_LUT_03   MOTION_EDGE_LUT_03                ; /* 0x2368 */
    U_MOTION_EDGE_LUT_04   MOTION_EDGE_LUT_04                ; /* 0x236c */
    U_MOTION_EDGE_LUT_05   MOTION_EDGE_LUT_05                ; /* 0x2370 */
    U_MOTION_EDGE_LUT_06   MOTION_EDGE_LUT_06                ; /* 0x2374 */
    U_MOTION_EDGE_LUT_07   MOTION_EDGE_LUT_07                ; /* 0x2378 */
    U_MOTION_EDGE_LUT_08   MOTION_EDGE_LUT_08                ; /* 0x237c */
    U_MOTION_EDGE_LUT_09   MOTION_EDGE_LUT_09                ; /* 0x2380 */
    U_MOTION_EDGE_LUT_10   MOTION_EDGE_LUT_10                ; /* 0x2384 */
    U_MOTION_EDGE_LUT_11   MOTION_EDGE_LUT_11                ; /* 0x2388 */
    U_MOTION_EDGE_LUT_12   MOTION_EDGE_LUT_12                ; /* 0x238c */
    U_MOTION_EDGE_LUT_13   MOTION_EDGE_LUT_13                ; /* 0x2390 */
    U_MOTION_EDGE_LUT_14   MOTION_EDGE_LUT_14                ; /* 0x2394 */
    U_MOTION_EDGE_LUT_15   MOTION_EDGE_LUT_15                ; /* 0x2398 */
    U_MOTION_EDGE_LUT_16   MOTION_EDGE_LUT_16                ; /* 0x239c */
    U_MOTION_EDGE_LUT_17   MOTION_EDGE_LUT_17                ; /* 0x23a0 */
    U_MOTION_EDGE_LUT_18   MOTION_EDGE_LUT_18                ; /* 0x23a4 */
    U_MOTION_EDGE_LUT_19   MOTION_EDGE_LUT_19                ; /* 0x23a8 */
    U_MOTION_EDGE_LUT_20   MOTION_EDGE_LUT_20                ; /* 0x23ac */
    U_MOTION_EDGE_LUT_21   MOTION_EDGE_LUT_21                ; /* 0x23b0 */
    U_MOTION_EDGE_LUT_22   MOTION_EDGE_LUT_22                ; /* 0x23b4 */
    U_MOTION_EDGE_LUT_23   MOTION_EDGE_LUT_23                ; /* 0x23b8 */
    U_MOTION_EDGE_LUT_24   MOTION_EDGE_LUT_24                ; /* 0x23bc */
    U_MOTION_EDGE_LUT_25   MOTION_EDGE_LUT_25                ; /* 0x23c0 */
    U_MOTION_EDGE_LUT_26   MOTION_EDGE_LUT_26                ; /* 0x23c4 */
    U_MOTION_EDGE_LUT_27   MOTION_EDGE_LUT_27                ; /* 0x23c8 */
    U_MOTION_EDGE_LUT_28   MOTION_EDGE_LUT_28                ; /* 0x23cc */
    U_MOTION_EDGE_LUT_29   MOTION_EDGE_LUT_29                ; /* 0x23d0 */
    U_MOTION_EDGE_LUT_30   MOTION_EDGE_LUT_30                ; /* 0x23d4 */
    U_MOTION_EDGE_LUT_31   MOTION_EDGE_LUT_31                ; /* 0x23d8 */
    U_MOTION_EDGE_LUT_32   MOTION_EDGE_LUT_32                ; /* 0x23dc */
    unsigned int           reserved_14[72]                     ; /* 0x23e0~0x24fc */
    U_VPSS_DM_DR_CTRL      VPSS_DM_DR_CTRL                   ; /* 0x2500 */
    U_VPSS_DR_THR          VPSS_DR_THR                       ; /* 0x2504 */
    U_VPSS_DRLSWSTRLUT0    VPSS_DRLSWSTRLUT0                 ; /* 0x2508 */
    U_VPSS_DRLSWSTRLUT1    VPSS_DRLSWSTRLUT1                 ; /* 0x250c */
    U_VPSS_DRLSWSTRLUT2    VPSS_DRLSWSTRLUT2                 ; /* 0x2510 */
    U_VPSS_DRLSWSTRLUT3    VPSS_DRLSWSTRLUT3                 ; /* 0x2514 */
    U_VPSS_DRLSWSTRLUT4    VPSS_DRLSWSTRLUT4                 ; /* 0x2518 */
    U_VPSS_DRLSWSTRLUT5    VPSS_DRLSWSTRLUT5                 ; /* 0x251c */
    U_VPSS_DRLSWSTRLUT6    VPSS_DRLSWSTRLUT6                 ; /* 0x2520 */
    U_VPSS_DRLSWSTRLUT7    VPSS_DRLSWSTRLUT7                 ; /* 0x2524 */
    U_VPSS_DRLSWSTRLUT8    VPSS_DRLSWSTRLUT8                 ; /* 0x2528 */
    U_VPSS_DRLRWSTRLUT0    VPSS_DRLRWSTRLUT0                 ; /* 0x252c */
    U_VPSS_DRLRWSTRLUT1    VPSS_DRLRWSTRLUT1                 ; /* 0x2530 */
    U_VPSS_DRLRWSTRLUT2    VPSS_DRLRWSTRLUT2                 ; /* 0x2534 */
    U_VPSS_DRLRWSTRLUT3    VPSS_DRLRWSTRLUT3                 ; /* 0x2538 */
    U_VPSS_DRLRWSTRLUT4    VPSS_DRLRWSTRLUT4                 ; /* 0x253c */
    U_VPSS_DRLRWSTRLUT5    VPSS_DRLRWSTRLUT5                 ; /* 0x2540 */
    U_VPSS_DRLRWSTRLUT6    VPSS_DRLRWSTRLUT6                 ; /* 0x2544 */
    U_VPSS_DRLRWSTRLUT7    VPSS_DRLRWSTRLUT7                 ; /* 0x2548 */
    U_VPSS_DRLRWSTRLUT8    VPSS_DRLRWSTRLUT8                 ; /* 0x254c */
    U_VPSS_DRLSWSTRADJUSTLUT0   VPSS_DRLSWSTRADJUSTLUT0      ; /* 0x2550 */
    U_VPSS_DRLSWSTRADJUSTLUT1   VPSS_DRLSWSTRADJUSTLUT1      ; /* 0x2554 */
    U_VPSS_DRLSWSTRADJUSTLUT2   VPSS_DRLSWSTRADJUSTLUT2      ; /* 0x2558 */
    U_VPSS_DRLRWSTRADJUSTLUT0   VPSS_DRLRWSTRADJUSTLUT0      ; /* 0x255c */
    U_VPSS_DRLRWSTRADJUSTLUT1   VPSS_DRLRWSTRADJUSTLUT1      ; /* 0x2560 */
    U_VPSS_DRLRWSTRADJUSTLUT2   VPSS_DRLRWSTRADJUSTLUT2      ; /* 0x2564 */
    U_VPSS_DRANGADJSUTLUT0   VPSS_DRANGADJSUTLUT0            ; /* 0x2568 */
    U_VPSS_DRANGADJSUTLUT1   VPSS_DRANGADJSUTLUT1            ; /* 0x256c */
    U_VPSS_DRANGADJSUTLUT2   VPSS_DRANGADJSUTLUT2            ; /* 0x2570 */
    U_VPSS_DRANGLESTRLUT1_0   VPSS_DRANGLESTRLUT1_0          ; /* 0x2574 */
    U_VPSS_DRANGLESTRLUT1_1   VPSS_DRANGLESTRLUT1_1          ; /* 0x2578 */
    U_VPSS_DRANGLESTRLUT1_2   VPSS_DRANGLESTRLUT1_2          ; /* 0x257c */
    U_VPSS_DRANGLESTRLUT2_0   VPSS_DRANGLESTRLUT2_0          ; /* 0x2580 */
    U_VPSS_DRANGLESTRLUT2_1   VPSS_DRANGLESTRLUT2_1          ; /* 0x2584 */
    U_VPSS_DRANGLESTRLUT2_2   VPSS_DRANGLESTRLUT2_2          ; /* 0x2588 */
    U_VPSS_DMDR_MERGE      VPSS_DMDR_MERGE                   ; /* 0x258c */
    U_VPSS_DMDR_MERGE_DELTA0   VPSS_DMDR_MERGE_DELTA0        ; /* 0x2590 */
    U_VPSS_DMDR_MERGE_DELTA1   VPSS_DMDR_MERGE_DELTA1        ; /* 0x2594 */
    U_VPSS_DMDR_MERGE_DELTA2   VPSS_DMDR_MERGE_DELTA2        ; /* 0x2598 */
    unsigned int           reserved_15[25]                     ; /* 0x259c~0x25fc */
    U_VPSS_FMTC_GAMMA_CSC_CTRL   VPSS_FMTC_GAMMA_CSC_CTRL    ; /* 0x2600 */
    U_VPSS_FMTC_GAMMA_YLUT_0   VPSS_FMTC_GAMMA_YLUT_0        ; /* 0x2604 */
    U_VPSS_FMTC_GAMMA_YLUT_1   VPSS_FMTC_GAMMA_YLUT_1        ; /* 0x2608 */
    U_VPSS_FMTC_GAMMA_YLUT_2   VPSS_FMTC_GAMMA_YLUT_2        ; /* 0x260c */
    U_VPSS_FMTC_GAMMA_YLUT_3   VPSS_FMTC_GAMMA_YLUT_3        ; /* 0x2610 */
    U_VPSS_FMTC_GAMMA_YLUT_4   VPSS_FMTC_GAMMA_YLUT_4        ; /* 0x2614 */
    U_VPSS_FMTC_GAMMA_YLUT_5   VPSS_FMTC_GAMMA_YLUT_5        ; /* 0x2618 */
    U_VPSS_FMTC_GAMMA_YLUT_6   VPSS_FMTC_GAMMA_YLUT_6        ; /* 0x261c */
    U_VPSS_FMTC_GAMMA_YLUT_7   VPSS_FMTC_GAMMA_YLUT_7        ; /* 0x2620 */
    U_VPSS_FMTC_GAMMA_YLUT_8   VPSS_FMTC_GAMMA_YLUT_8        ; /* 0x2624 */
    U_VPSS_FMTC_GAMMA_ULUT_0   VPSS_FMTC_GAMMA_ULUT_0        ; /* 0x2628 */
    U_VPSS_FMTC_GAMMA_ULUT_1   VPSS_FMTC_GAMMA_ULUT_1        ; /* 0x262c */
    U_VPSS_FMTC_GAMMA_ULUT_2   VPSS_FMTC_GAMMA_ULUT_2        ; /* 0x2630 */
    U_VPSS_FMTC_GAMMA_ULUT_3   VPSS_FMTC_GAMMA_ULUT_3        ; /* 0x2634 */
    U_VPSS_FMTC_GAMMA_ULUT_4   VPSS_FMTC_GAMMA_ULUT_4        ; /* 0x2638 */
    U_VPSS_FMTC_GAMMA_ULUT_5   VPSS_FMTC_GAMMA_ULUT_5        ; /* 0x263c */
    U_VPSS_FMTC_GAMMA_ULUT_6   VPSS_FMTC_GAMMA_ULUT_6        ; /* 0x2640 */
    U_VPSS_FMTC_GAMMA_ULUT_7   VPSS_FMTC_GAMMA_ULUT_7        ; /* 0x2644 */
    U_VPSS_FMTC_GAMMA_ULUT_8   VPSS_FMTC_GAMMA_ULUT_8        ; /* 0x2648 */
    U_VPSS_FMTC_GAMMA_VLUT_0   VPSS_FMTC_GAMMA_VLUT_0        ; /* 0x264c */
    U_VPSS_FMTC_GAMMA_VLUT_1   VPSS_FMTC_GAMMA_VLUT_1        ; /* 0x2650 */
    U_VPSS_FMTC_GAMMA_VLUT_2   VPSS_FMTC_GAMMA_VLUT_2        ; /* 0x2654 */
    U_VPSS_FMTC_GAMMA_VLUT_3   VPSS_FMTC_GAMMA_VLUT_3        ; /* 0x2658 */
    U_VPSS_FMTC_GAMMA_VLUT_4   VPSS_FMTC_GAMMA_VLUT_4        ; /* 0x265c */
    U_VPSS_FMTC_GAMMA_VLUT_5   VPSS_FMTC_GAMMA_VLUT_5        ; /* 0x2660 */
    U_VPSS_FMTC_GAMMA_VLUT_6   VPSS_FMTC_GAMMA_VLUT_6        ; /* 0x2664 */
    U_VPSS_FMTC_GAMMA_VLUT_7   VPSS_FMTC_GAMMA_VLUT_7        ; /* 0x2668 */
    U_VPSS_FMTC_GAMMA_VLUT_8   VPSS_FMTC_GAMMA_VLUT_8        ; /* 0x266c */
    U_VPSS_FMTC_CSC_COEF_0   VPSS_FMTC_CSC_COEF_0            ; /* 0x2670 */
    U_VPSS_FMTC_CSC_COEF_1   VPSS_FMTC_CSC_COEF_1            ; /* 0x2674 */
    U_VPSS_FMTC_CSC_COEF_2   VPSS_FMTC_CSC_COEF_2            ; /* 0x2678 */
    U_VPSS_FMTC_CSC_COEF_3   VPSS_FMTC_CSC_COEF_3            ; /* 0x267c */
    U_VPSS_FMTC_CSC_COEF_4   VPSS_FMTC_CSC_COEF_4            ; /* 0x2680 */
    U_VPSS_FMTC_CSC_COEF_5   VPSS_FMTC_CSC_COEF_5            ; /* 0x2684 */
    U_VPSS_FMTC_CSC_COEF_6   VPSS_FMTC_CSC_COEF_6            ; /* 0x2688 */
    U_VPSS_FMTC_CSC_COEF_7   VPSS_FMTC_CSC_COEF_7            ; /* 0x268c */
    unsigned int           reserved_16[1116]                     ; /* 0x2690~0x37fc */
    U_VPSS_TNR_CONTRL      VPSS_TNR_CONTRL                   ; /* 0x3800 */
    U_VPSS_TNR_MAMD_CTRL   VPSS_TNR_MAMD_CTRL                ; /* 0x3804 */
    U_VPSS_TNR_MAMD_GAIN   VPSS_TNR_MAMD_GAIN                ; /* 0x3808 */
    U_VPSS_TNR_MAND_CTRL   VPSS_TNR_MAND_CTRL                ; /* 0x380c */
    U_VPSS_TNR_MAND_THD    VPSS_TNR_MAND_THD                 ; /* 0x3810 */
    U_VPSS_TNR_MAME_CTRL   VPSS_TNR_MAME_CTRL                ; /* 0x3814 */
    U_VPSS_TNR_MAME_GAIN   VPSS_TNR_MAME_GAIN                ; /* 0x3818 */
    U_VPSS_TNR_MAME_STD_CORE   VPSS_TNR_MAME_STD_CORE        ; /* 0x381c */
    U_VPSS_TNR_MAME_STD_GAIN   VPSS_TNR_MAME_STD_GAIN        ; /* 0x3820 */
    U_VPSS_TNR_MAME_MV_THD   VPSS_TNR_MAME_MV_THD            ; /* 0x3824 */
    U_VPSS_TNR_MAMC_CBCR_UPEN   VPSS_TNR_MAMC_CBCR_UPEN      ; /* 0x3828 */
    U_VPSS_TNR_MAMC_CBCR_UPTHD1   VPSS_TNR_MAMC_CBCR_UPTHD1  ; /* 0x382c */
    U_VPSS_TNR_MAMC_CBCR_UPTHD2   VPSS_TNR_MAMC_CBCR_UPTHD2  ; /* 0x3830 */
    U_VPSS_TNR_MAMC_PRE_MAD   VPSS_TNR_MAMC_PRE_MAD          ; /* 0x3834 */
    U_VPSS_TNR_MAMC_MAP    VPSS_TNR_MAMC_MAP                 ; /* 0x3838 */
    U_VPSS_TNR_MAMC_SALTUS   VPSS_TNR_MAMC_SALTUS            ; /* 0x383c */
    U_VPSS_TNR_MAMC_MCADJ   VPSS_TNR_MAMC_MCADJ              ; /* 0x3840 */
    U_VPSS_TNR_FILR_MODE   VPSS_TNR_FILR_MODE                ; /* 0x3844 */
    U_VPSS_TNR_SCENE_CHANGE   VPSS_TNR_SCENE_CHANGE          ; /* 0x3848 */
    U_VPSS_TNR_CFG         VPSS_TNR_CFG                      ; /* 0x384c */
    U_VPSS_TNR_FLAT_INFO   VPSS_TNR_FLAT_INFO                ; /* 0x3850 */
    U_VPSS_TNR_YBLEND      VPSS_TNR_YBLEND                   ; /* 0x3854 */
    U_VPSS_TNR_CBLEND      VPSS_TNR_CBLEND                   ; /* 0x3858 */
    U_VPSS_TNR_DTBLEND     VPSS_TNR_DTBLEND                  ; /* 0x385c */
    unsigned int           reserved_17[40]                     ; /* 0x3860~0x38fc */
    U_VPSS_TNR_MEAN_MOTION_LUT0   VPSS_TNR_MEAN_MOTION_LUT0  ; /* 0x3900 */
    U_VPSS_TNR_MEAN_MOTION_LUT1   VPSS_TNR_MEAN_MOTION_LUT1  ; /* 0x3904 */
    U_VPSS_TNR_MEAN_MOTION_LUT2   VPSS_TNR_MEAN_MOTION_LUT2  ; /* 0x3908 */
    U_VPSS_TNR_MEAN_MOTION_LUT3   VPSS_TNR_MEAN_MOTION_LUT3  ; /* 0x390c */
    unsigned int           reserved_18[12]                     ; /* 0x3910~0x393c */
    U_VPSS_TNR_YBLENDING_LUT0   VPSS_TNR_YBLENDING_LUT0      ; /* 0x3940 */
    U_VPSS_TNR_YBLENDING_LUT1   VPSS_TNR_YBLENDING_LUT1      ; /* 0x3944 */
    U_VPSS_TNR_YBLENDING_LUT2   VPSS_TNR_YBLENDING_LUT2      ; /* 0x3948 */
    U_VPSS_TNR_YBLENDING_LUT3   VPSS_TNR_YBLENDING_LUT3      ; /* 0x394c */
    U_VPSS_TNR_YBLENDING_LUT4   VPSS_TNR_YBLENDING_LUT4      ; /* 0x3950 */
    U_VPSS_TNR_YBLENDING_LUT5   VPSS_TNR_YBLENDING_LUT5      ; /* 0x3954 */
    U_VPSS_TNR_YBLENDING_LUT6   VPSS_TNR_YBLENDING_LUT6      ; /* 0x3958 */
    U_VPSS_TNR_YBLENDING_LUT7   VPSS_TNR_YBLENDING_LUT7      ; /* 0x395c */
    U_VPSS_TNR_CBLENDING_LUT0   VPSS_TNR_CBLENDING_LUT0      ; /* 0x3960 */
    U_VPSS_TNR_CBLENDING_LUT1   VPSS_TNR_CBLENDING_LUT1      ; /* 0x3964 */
    U_VPSS_TNR_CBLENDING_LUT2   VPSS_TNR_CBLENDING_LUT2      ; /* 0x3968 */
    U_VPSS_TNR_CBLENDING_LUT3   VPSS_TNR_CBLENDING_LUT3      ; /* 0x396c */
    U_VPSS_TNR_CBLENDING_LUT4   VPSS_TNR_CBLENDING_LUT4      ; /* 0x3970 */
    U_VPSS_TNR_CBLENDING_LUT5   VPSS_TNR_CBLENDING_LUT5      ; /* 0x3974 */
    U_VPSS_TNR_CBLENDING_LUT6   VPSS_TNR_CBLENDING_LUT6      ; /* 0x3978 */
    U_VPSS_TNR_CBLENDING_LUT7   VPSS_TNR_CBLENDING_LUT7      ; /* 0x397c */
    U_VPSS_TNR_YMOTIONSTR_LUT0   VPSS_TNR_YMOTIONSTR_LUT0    ; /* 0x3980 */
    U_VPSS_TNR_YMOTIONSTR_LUT1   VPSS_TNR_YMOTIONSTR_LUT1    ; /* 0x3984 */
    U_VPSS_TNR_YMOTIONSTR_LUT2   VPSS_TNR_YMOTIONSTR_LUT2    ; /* 0x3988 */
    U_VPSS_TNR_YMOTIONSTR_LUT3   VPSS_TNR_YMOTIONSTR_LUT3    ; /* 0x398c */
    U_VPSS_TNR_YMOTIONSTR_LUT4   VPSS_TNR_YMOTIONSTR_LUT4    ; /* 0x3990 */
    U_VPSS_TNR_YMOTIONSTR_LUT5   VPSS_TNR_YMOTIONSTR_LUT5    ; /* 0x3994 */
    U_VPSS_TNR_YMOTIONSTR_LUT6   VPSS_TNR_YMOTIONSTR_LUT6    ; /* 0x3998 */
    U_VPSS_TNR_YMOTIONSTR_LUT7   VPSS_TNR_YMOTIONSTR_LUT7    ; /* 0x399c */
    U_VPSS_TNR_YMOTIONSTR_LUT8   VPSS_TNR_YMOTIONSTR_LUT8    ; /* 0x39a0 */
    U_VPSS_TNR_YMOTIONSTR_LUT9   VPSS_TNR_YMOTIONSTR_LUT9    ; /* 0x39a4 */
    U_VPSS_TNR_YMOTIONSTR_LUT10   VPSS_TNR_YMOTIONSTR_LUT10  ; /* 0x39a8 */
    U_VPSS_TNR_YMOTIONSTR_LUT11   VPSS_TNR_YMOTIONSTR_LUT11  ; /* 0x39ac */
    U_VPSS_TNR_YMOTIONSTR_LUT12   VPSS_TNR_YMOTIONSTR_LUT12  ; /* 0x39b0 */
    U_VPSS_TNR_YMOTIONSTR_LUT13   VPSS_TNR_YMOTIONSTR_LUT13  ; /* 0x39b4 */
    U_VPSS_TNR_YMOTIONSTR_LUT14   VPSS_TNR_YMOTIONSTR_LUT14  ; /* 0x39b8 */
    U_VPSS_TNR_YMOTIONSTR_LUT15   VPSS_TNR_YMOTIONSTR_LUT15  ; /* 0x39bc */
    U_VPSS_TNR_YMOTIONSTR_LUT16   VPSS_TNR_YMOTIONSTR_LUT16  ; /* 0x39c0 */
    U_VPSS_TNR_YMOTIONSTR_LUT17   VPSS_TNR_YMOTIONSTR_LUT17  ; /* 0x39c4 */
    U_VPSS_TNR_YMOTIONSTR_LUT18   VPSS_TNR_YMOTIONSTR_LUT18  ; /* 0x39c8 */
    U_VPSS_TNR_YMOTIONSTR_LUT19   VPSS_TNR_YMOTIONSTR_LUT19  ; /* 0x39cc */
    U_VPSS_TNR_YMOTIONSTR_LUT20   VPSS_TNR_YMOTIONSTR_LUT20  ; /* 0x39d0 */
    U_VPSS_TNR_YMOTIONSTR_LUT21   VPSS_TNR_YMOTIONSTR_LUT21  ; /* 0x39d4 */
    U_VPSS_TNR_YMOTIONSTR_LUT22   VPSS_TNR_YMOTIONSTR_LUT22  ; /* 0x39d8 */
    U_VPSS_TNR_YMOTIONSTR_LUT23   VPSS_TNR_YMOTIONSTR_LUT23  ; /* 0x39dc */
    U_VPSS_TNR_YMOTIONSTR_LUT24   VPSS_TNR_YMOTIONSTR_LUT24  ; /* 0x39e0 */
    U_VPSS_TNR_YMOTIONSTR_LUT25   VPSS_TNR_YMOTIONSTR_LUT25  ; /* 0x39e4 */
    U_VPSS_TNR_YMOTIONSTR_LUT26   VPSS_TNR_YMOTIONSTR_LUT26  ; /* 0x39e8 */
    U_VPSS_TNR_YMOTIONSTR_LUT27   VPSS_TNR_YMOTIONSTR_LUT27  ; /* 0x39ec */
    U_VPSS_TNR_YMOTIONSTR_LUT28   VPSS_TNR_YMOTIONSTR_LUT28  ; /* 0x39f0 */
    U_VPSS_TNR_YMOTIONSTR_LUT29   VPSS_TNR_YMOTIONSTR_LUT29  ; /* 0x39f4 */
    U_VPSS_TNR_YMOTIONSTR_LUT30   VPSS_TNR_YMOTIONSTR_LUT30  ; /* 0x39f8 */
    U_VPSS_TNR_YMOTIONSTR_LUT31   VPSS_TNR_YMOTIONSTR_LUT31  ; /* 0x39fc */
    U_VPSS_TNR_CMOTIONSTR_LUT0   VPSS_TNR_CMOTIONSTR_LUT0    ; /* 0x3a00 */
    U_VPSS_TNR_CMOTIONSTR_LUT1   VPSS_TNR_CMOTIONSTR_LUT1    ; /* 0x3a04 */
    U_VPSS_TNR_CMOTIONSTR_LUT2   VPSS_TNR_CMOTIONSTR_LUT2    ; /* 0x3a08 */
    U_VPSS_TNR_CMOTIONSTR_LUT3   VPSS_TNR_CMOTIONSTR_LUT3    ; /* 0x3a0c */
    U_VPSS_TNR_CMOTIONSTR_LUT4   VPSS_TNR_CMOTIONSTR_LUT4    ; /* 0x3a10 */
    U_VPSS_TNR_CMOTIONSTR_LUT5   VPSS_TNR_CMOTIONSTR_LUT5    ; /* 0x3a14 */
    U_VPSS_TNR_CMOTIONSTR_LUT6   VPSS_TNR_CMOTIONSTR_LUT6    ; /* 0x3a18 */
    U_VPSS_TNR_CMOTIONSTR_LUT7   VPSS_TNR_CMOTIONSTR_LUT7    ; /* 0x3a1c */
    U_VPSS_TNR_CMOTIONSTR_LUT8   VPSS_TNR_CMOTIONSTR_LUT8    ; /* 0x3a20 */
    U_VPSS_TNR_CMOTIONSTR_LUT9   VPSS_TNR_CMOTIONSTR_LUT9    ; /* 0x3a24 */
    U_VPSS_TNR_CMOTIONSTR_LUT10   VPSS_TNR_CMOTIONSTR_LUT10  ; /* 0x3a28 */
    U_VPSS_TNR_CMOTIONSTR_LUT11   VPSS_TNR_CMOTIONSTR_LUT11  ; /* 0x3a2c */
    U_VPSS_TNR_CMOTIONSTR_LUT12   VPSS_TNR_CMOTIONSTR_LUT12  ; /* 0x3a30 */
    U_VPSS_TNR_CMOTIONSTR_LUT13   VPSS_TNR_CMOTIONSTR_LUT13  ; /* 0x3a34 */
    U_VPSS_TNR_CMOTIONSTR_LUT14   VPSS_TNR_CMOTIONSTR_LUT14  ; /* 0x3a38 */
    U_VPSS_TNR_CMOTIONSTR_LUT15   VPSS_TNR_CMOTIONSTR_LUT15  ; /* 0x3a3c */
    U_VPSS_TNR_CMOTIONSTR_LUT16   VPSS_TNR_CMOTIONSTR_LUT16  ; /* 0x3a40 */
    U_VPSS_TNR_CMOTIONSTR_LUT17   VPSS_TNR_CMOTIONSTR_LUT17  ; /* 0x3a44 */
    U_VPSS_TNR_CMOTIONSTR_LUT18   VPSS_TNR_CMOTIONSTR_LUT18  ; /* 0x3a48 */
    U_VPSS_TNR_CMOTIONSTR_LUT19   VPSS_TNR_CMOTIONSTR_LUT19  ; /* 0x3a4c */
    U_VPSS_TNR_CMOTIONSTR_LUT20   VPSS_TNR_CMOTIONSTR_LUT20  ; /* 0x3a50 */
    U_VPSS_TNR_CMOTIONSTR_LUT21   VPSS_TNR_CMOTIONSTR_LUT21  ; /* 0x3a54 */
    U_VPSS_TNR_CMOTIONSTR_LUT22   VPSS_TNR_CMOTIONSTR_LUT22  ; /* 0x3a58 */
    U_VPSS_TNR_CMOTIONSTR_LUT23   VPSS_TNR_CMOTIONSTR_LUT23  ; /* 0x3a5c */
    U_VPSS_TNR_CMOTIONSTR_LUT24   VPSS_TNR_CMOTIONSTR_LUT24  ; /* 0x3a60 */
    U_VPSS_TNR_CMOTIONSTR_LUT25   VPSS_TNR_CMOTIONSTR_LUT25  ; /* 0x3a64 */
    U_VPSS_TNR_CMOTIONSTR_LUT26   VPSS_TNR_CMOTIONSTR_LUT26  ; /* 0x3a68 */
    U_VPSS_TNR_CMOTIONSTR_LUT27   VPSS_TNR_CMOTIONSTR_LUT27  ; /* 0x3a6c */
    U_VPSS_TNR_CMOTIONSTR_LUT28   VPSS_TNR_CMOTIONSTR_LUT28  ; /* 0x3a70 */
    U_VPSS_TNR_CMOTIONSTR_LUT29   VPSS_TNR_CMOTIONSTR_LUT29  ; /* 0x3a74 */
    U_VPSS_TNR_CMOTIONSTR_LUT30   VPSS_TNR_CMOTIONSTR_LUT30  ; /* 0x3a78 */
    U_VPSS_TNR_CMOTIONSTR_LUT31   VPSS_TNR_CMOTIONSTR_LUT31  ; /* 0x3a7c */
    unsigned int           reserved_19[352]                     ; /* 0x3a80~0x3ffc */
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
    unsigned int           reserved_20[11220]                     ; /* 0x40b0~0xeffc */
    U_VPSS_SMMU_SCR        VPSS_SMMU_SCR                     ; /* 0xf000 */
    unsigned int           reserved_21                     ; /* 0xf004 */
    U_VPSS_SMMU_LP_CTRL    VPSS_SMMU_LP_CTRL                 ; /* 0xf008 */
    U_VPSS_SMMU_MEM_SPEEDCTRL   VPSS_SMMU_MEM_SPEEDCTRL      ; /* 0xf00c */
    U_VPSS_SMMU_INTMASK_S   VPSS_SMMU_INTMASK_S              ; /* 0xf010 */
    U_VPSS_SMMU_INTRAW_S   VPSS_SMMU_INTRAW_S                ; /* 0xf014 */
    U_VPSS_SMMU_INTSTAT_S   VPSS_SMMU_INTSTAT_S              ; /* 0xf018 */
    U_VPSS_SMMU_INTCLR_S   VPSS_SMMU_INTCLR_S                ; /* 0xf01c */
    U_VPSS_SMMU_INTMASK_NS   VPSS_SMMU_INTMASK_NS            ; /* 0xf020 */
    U_VPSS_SMMU_INTRAW_NS   VPSS_SMMU_INTRAW_NS              ; /* 0xf024 */
    U_VPSS_SMMU_INTSTAT_NS   VPSS_SMMU_INTSTAT_NS            ; /* 0xf028 */
    U_VPSS_SMMU_INTCLR_NS   VPSS_SMMU_INTCLR_NS              ; /* 0xf02c */
    unsigned int           reserved_22[118]                     ; /* 0xf030~0xf204 */
    U_VPSS_SMMU_SCB_TTBR   VPSS_SMMU_SCB_TTBR                ; /* 0xf208 */
    U_VPSS_SMMU_CB_TTBR    VPSS_SMMU_CB_TTBR                 ; /* 0xf20c */
    unsigned int           reserved_23[56]                     ; /* 0xf210~0xf2ec */
    U_VPSS_SMMU_ERR_RDADDR_S   VPSS_SMMU_ERR_RDADDR_S        ; /* 0xf2f0 */
    U_VPSS_SMMU_ERR_WRADDR_S   VPSS_SMMU_ERR_WRADDR_S        ; /* 0xf2f4 */
    unsigned int           reserved_24[3]                     ; /* 0xf2f8~0xf300 */
    U_VPSS_SMMU_ERR_RDADDR_NS   VPSS_SMMU_ERR_RDADDR_NS      ; /* 0xf304 */
    U_VPSS_SMMU_ERR_WRADDR_NS   VPSS_SMMU_ERR_WRADDR_NS      ; /* 0xf308 */
    unsigned int           reserved_25                     ; /* 0xf30c */
    U_VPSS_SMMU_FAULT_ADDR_PTW_S   VPSS_SMMU_FAULT_ADDR_PTW_S ; /* 0xf310 */
    U_VPSS_SMMU_FAULT_ID_PTW_S   VPSS_SMMU_FAULT_ID_PTW_S    ; /* 0xf314 */
    unsigned int           reserved_26[2]                     ; /* 0xf318~0xf31c */
    U_VPSS_SMMU_FAULT_ADDR_PTW_NS   VPSS_SMMU_FAULT_ADDR_PTW_NS ; /* 0xf320 */
    U_VPSS_SMMU_FAULT_ID_PTW_NS   VPSS_SMMU_FAULT_ID_PTW_NS  ; /* 0xf324 */
    U_VPSS_SMMU_FAULT_PTW_NUM   VPSS_SMMU_FAULT_PTW_NUM      ; /* 0xf328 */
    unsigned int           reserved_27                     ; /* 0xf32c */
    U_VPSS_SMMU_FAULT_ADDR_WR_S   VPSS_SMMU_FAULT_ADDR_WR_S  ; /* 0xf330 */
    U_VPSS_SMMU_FAULT_TLB_WR_S   VPSS_SMMU_FAULT_TLB_WR_S    ; /* 0xf334 */
    U_VPSS_SMMU_FAULT_ID_WR_S   VPSS_SMMU_FAULT_ID_WR_S      ; /* 0xf338 */
    unsigned int           reserved_28                     ; /* 0xf33c */
    U_VPSS_SMMU_FAULT_ADDR_WR_NS   VPSS_SMMU_FAULT_ADDR_WR_NS ; /* 0xf340 */
    U_VPSS_SMMU_FAULT_TLB_WR_NS   VPSS_SMMU_FAULT_TLB_WR_NS  ; /* 0xf344 */
    U_VPSS_SMMU_FAULT_ID_WR_NS   VPSS_SMMU_FAULT_ID_WR_NS    ; /* 0xf348 */
    unsigned int           reserved_29                     ; /* 0xf34c */
    U_VPSS_SMMU_FAULT_ADDR_RD_S   VPSS_SMMU_FAULT_ADDR_RD_S  ; /* 0xf350 */
    U_VPSS_SMMU_FAULT_TLB_RD_S   VPSS_SMMU_FAULT_TLB_RD_S    ; /* 0xf354 */
    U_VPSS_SMMU_FAULT_ID_RD_S   VPSS_SMMU_FAULT_ID_RD_S      ; /* 0xf358 */
    unsigned int           reserved_30                     ; /* 0xf35c */
    U_VPSS_SMMU_FAULT_ADDR_RD_NS   VPSS_SMMU_FAULT_ADDR_RD_NS ; /* 0xf360 */
    U_VPSS_SMMU_FAULT_TLB_RD_NS   VPSS_SMMU_FAULT_TLB_RD_NS  ; /* 0xf364 */
    U_VPSS_SMMU_FAULT_ID_RD_NS   VPSS_SMMU_FAULT_ID_RD_NS    ; /* 0xf368 */
    U_VPSS_SMMU_FAULT_TBU_INFO   VPSS_SMMU_FAULT_TBU_INFO    ; /* 0xf36c */
    U_VPSS_SMMU_FAULT_TBU_DBG   VPSS_SMMU_FAULT_TBU_DBG      ; /* 0xf370 */
    U_VPSS_SMMU_PREF_BUFFER_EMPTY   VPSS_SMMU_PREF_BUFFER_EMPTY ; /* 0xf374 */
    U_VPSS_SMMU_PTWQ_IDLE   VPSS_SMMU_PTWQ_IDLE              ; /* 0xf378 */
    U_VPSS_SMMU_RESET_STATE   VPSS_SMMU_RESET_STATE          ; /* 0xf37c */
    U_VPSS_SMMU_MASTER_DBG0   VPSS_SMMU_MASTER_DBG0          ; /* 0xf380 */
    U_VPSS_SMMU_MASTER_DBG1   VPSS_SMMU_MASTER_DBG1          ; /* 0xf384 */
    U_VPSS_SMMU_MASTER_DBG2   VPSS_SMMU_MASTER_DBG2          ; /* 0xf388 */
    U_VPSS_SMMU_MASTER_DBG3   VPSS_SMMU_MASTER_DBG3          ; /* 0xf38c */
    unsigned int           reserved_31[100]                     ; /* 0xf390~0xf51c */
    U_VPSS_SMMU_MASTER_DBG4   VPSS_SMMU_MASTER_DBG4          ; /* 0xf520 */
    U_VPSS_SMMU_MASTER_DBG5   VPSS_SMMU_MASTER_DBG5          ; /* 0xf524 */
    U_VPSS_SMMU_MASTER_DBG6   VPSS_SMMU_MASTER_DBG6          ; /* 0xf528 */
    U_VPSS_SMMU_MASTER_DBG7   VPSS_SMMU_MASTER_DBG7          ; /* 0xf52c */
    U_VPSS_SMMU_MASTER_DBG8   VPSS_SMMU_MASTER_DBG8          ; /* 0xf530 */

} VPSS_REG_S, S_VPSS_REGS_TYPE, S_CAS_REGS_TYPE;

/* Declare the struct pointor of the module VPSS */
extern S_VPSS_REGS_TYPE *gopVPSSAllReg;

#endif //#ifndef __HI_REG_VPSS_H__