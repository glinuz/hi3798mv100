//******************************************************************************
//  Copyright (C), 2007-2014, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define.h
// Version       : 2.0
// Author        : xxx
// Created       : 2014-07-24
// Last Modified : 
// Description   :  The C union definition file for the module VDP
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __VDP_REG_98M_H__
#define __VDP_REG_98M_H__

// Define the union U_VOCTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    m0_arb_mode           : 4   ; // [3..0] 
        unsigned int    Reserved_0            : 25  ; // [28..4] 
        unsigned int    g3_dhd1_sel           : 1   ; // [29] 
        unsigned int    chk_sum_en            : 1   ; // [30] 
        unsigned int    vo_ck_gt_en           : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOCTRL;

// Define the union U_VOINTSTA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dhd0vtthd1_int        : 1   ; // [0] 
        unsigned int    dhd0vtthd2_int        : 1   ; // [1] 
        unsigned int    dhd0vtthd3_int        : 1   ; // [2] 
        unsigned int    dhd0uf_int            : 1   ; // [3] 
        unsigned int    dhd1vtthd1_int        : 1   ; // [4] 
        unsigned int    dhd1vtthd2_int        : 1   ; // [5] 
        unsigned int    dhd1vtthd3_int        : 1   ; // [6] 
        unsigned int    dhd1uf_int            : 1   ; // [7] 
        unsigned int    gwbc0_vte_int         : 1   ; // [8] 
        unsigned int    dwbc0_vte_int         : 1   ; // [9] 
        unsigned int    g0wbc_vte_int         : 1   ; // [10] 
        unsigned int    g4wbc_vte_int         : 1   ; // [11] 
        unsigned int    Reserved_6            : 4   ; // [15..12] 
        unsigned int    v0rr_int              : 1   ; // [16] 
        unsigned int    v1rr_int              : 1   ; // [17] 
        unsigned int    Reserved_5            : 1   ; // [18] 
        unsigned int    v3rr_int              : 1   ; // [19] 
        unsigned int    v4rr_int              : 1   ; // [20] 
        unsigned int    Reserved_4            : 1   ; // [21] 
        unsigned int    g0rr_int              : 1   ; // [22] 
        unsigned int    g1rr_int              : 1   ; // [23] 
        unsigned int    g2rr_int              : 1   ; // [24] 
        unsigned int    Reserved_3            : 1   ; // [25] 
        unsigned int    g4rr_int              : 1   ; // [26] 
        unsigned int    Reserved_2            : 1   ; // [27] 
        unsigned int    wbcdhd_partfns_int    : 1   ; // [28] 
        unsigned int    ut_end_int            : 1   ; // [29] 
        unsigned int    m0_be_int             : 1   ; // [30] 
        unsigned int    Reserved_1            : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOINTSTA;

// Define the union U_VOMSKINTSTA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dhd0vtthd1_clr        : 1   ; // [0] 
        unsigned int    dhd0vtthd2_clr        : 1   ; // [1] 
        unsigned int    dhd0vtthd3_clr        : 1   ; // [2] 
        unsigned int    dhd0uf_clr            : 1   ; // [3] 
        unsigned int    dhd1vtthd1_clr        : 1   ; // [4] 
        unsigned int    dhd1vtthd2_clr        : 1   ; // [5] 
        unsigned int    dhd1vtthd3_clr        : 1   ; // [6] 
        unsigned int    dhd1uf_clr            : 1   ; // [7] 
        unsigned int    gwbc0_vte_clr         : 1   ; // [8] 
        unsigned int    dwbc0_vte_clr         : 1   ; // [9] 
        unsigned int    g0wbc_vte_clr         : 1   ; // [10] 
        unsigned int    g4wbc_vte_clr         : 1   ; // [11] 
        unsigned int    Reserved_12           : 4   ; // [15..12] 
        unsigned int    v0rr_clr              : 1   ; // [16] 
        unsigned int    v1rr_clr              : 1   ; // [17] 
        unsigned int    Reserved_11           : 1   ; // [18] 
        unsigned int    v3rr_clr              : 1   ; // [19] 
        unsigned int    v4rr_clr              : 1   ; // [20] 
        unsigned int    Reserved_10           : 1   ; // [21] 
        unsigned int    g0rr_clr              : 1   ; // [22] 
        unsigned int    g1rr_clr              : 1   ; // [23] 
        unsigned int    g2rr_clr              : 1   ; // [24] 
        unsigned int    Reserved_9            : 1   ; // [25] 
        unsigned int    g4rr_clr              : 1   ; // [26] 
        unsigned int    Reserved_8            : 1   ; // [27] 
        unsigned int    wbcdhd_partfns_clr    : 1   ; // [28] 
        unsigned int    ut_end_clr            : 1   ; // [29] 
        unsigned int    m0_be_clr             : 1   ; // [30] 
        unsigned int    Reserved_7            : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOMSKINTSTA;

// Define the union U_VOINTMSK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dhd0vtthd1_intmsk     : 1   ; // [0] 
        unsigned int    dhd0vtthd2_intmsk     : 1   ; // [1] 
        unsigned int    dhd0vtthd3_intmsk     : 1   ; // [2] 
        unsigned int    dhd0uf_intmsk         : 1   ; // [3] 
        unsigned int    dhd1vtthd1_intmsk     : 1   ; // [4] 
        unsigned int    dhd1vtthd2_intmsk     : 1   ; // [5] 
        unsigned int    dhd1vtthd3_intmsk     : 1   ; // [6] 
        unsigned int    dhd1uf_intmsk         : 1   ; // [7] 
        unsigned int    gwbc0_vte_intmsk      : 1   ; // [8] 
        unsigned int    dwbc0_vte_intmsk      : 1   ; // [9] 
        unsigned int    g0wbc_vte_intmsk      : 1   ; // [10] 
        unsigned int    g4wbc_vte_intmsk      : 1   ; // [11] 
        unsigned int    Reserved_18           : 4   ; // [15..12] 
        unsigned int    v0rr_intmsk           : 1   ; // [16] 
        unsigned int    v1rr_intmsk           : 1   ; // [17] 
        unsigned int    Reserved_17           : 1   ; // [18] 
        unsigned int    v3rr_intmsk           : 1   ; // [19] 
        unsigned int    v4rr_intmsk           : 1   ; // [20] 
        unsigned int    Reserved_16           : 1   ; // [21] 
        unsigned int    g0rr_intmsk           : 1   ; // [22] 
        unsigned int    g1rr_intmsk           : 1   ; // [23] 
        unsigned int    g2rr_intmsk           : 1   ; // [24] 
        unsigned int    Reserved_15           : 1   ; // [25] 
        unsigned int    g4rr_intmsk           : 1   ; // [26] 
        unsigned int    Reserved_14           : 1   ; // [27] 
        unsigned int    wbcdhd_partfns_intmsk  : 1   ; // [28] 
        unsigned int    ut_end_intmsk         : 1   ; // [29] 
        unsigned int    m0_be_intmsk          : 1   ; // [30] 
        unsigned int    Reserved_13           : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOINTMSK;

// Define the union U_VDPVERSION1
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int vdpversion1             : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_VDPVERSION1;
// Define the union U_VDPVERSION2
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int vdpversion2             : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_VDPVERSION2;
// Define the union U_VODEBUG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    rm_en_chn             : 4   ; // [3..0] 
        unsigned int    dhd0_ff_info          : 2   ; // [5..4] 
        unsigned int    dhd1_ff_info          : 2   ; // [7..6] 
        unsigned int    bfm_vga_en            : 1   ; // [8] 
        unsigned int    bfm_cvbs_en           : 1   ; // [9] 
        unsigned int    bfm_lcd_en            : 1   ; // [10] 
        unsigned int    bfm_bt1120_en         : 1   ; // [11] 
        unsigned int    wbc2_ff_info          : 2   ; // [13..12] 
        unsigned int    wbc_mode              : 4   ; // [17..14] 
        unsigned int    node_num              : 6   ; // [23..18] 
        unsigned int    wbc_cmp_mode          : 2   ; // [25..24] 
        unsigned int    bfm_mode              : 3   ; // [28..26] 
        unsigned int    Reserved_19           : 3   ; // [31..29] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VODEBUG;

// Define the union U_VOAXICTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    m0_outstd_rid0        : 4   ; // [3..0] 
        unsigned int    m0_outstd_rid1        : 4   ; // [7..4] 
        unsigned int    m0_wr_ostd            : 4   ; // [11..8] 
        unsigned int    Reserved_21           : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOAXICTRL;

// Define the union U_VOWBCARB0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    prio0                 : 3   ; // [2..0] 
        unsigned int    prio1                 : 3   ; // [5..3] 
        unsigned int    prio2                 : 3   ; // [8..6] 
        unsigned int    prio3                 : 3   ; // [11..9] 
        unsigned int    prio4                 : 3   ; // [14..12] 
        unsigned int    prio5                 : 3   ; // [17..15] 
        unsigned int    prio6                 : 3   ; // [20..18] 
        unsigned int    Reserved_23           : 7   ; // [27..21] 
        unsigned int    w_arb_mode            : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOWBCARB0;

// Define the union U_VOWBCARB1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    prio0                 : 3   ; // [2..0] 
        unsigned int    prio1                 : 3   ; // [5..3] 
        unsigned int    prio2                 : 3   ; // [8..6] 
        unsigned int    prio3                 : 3   ; // [11..9] 
        unsigned int    prio4                 : 3   ; // [14..12] 
        unsigned int    prio5                 : 3   ; // [17..15] 
        unsigned int    prio6                 : 3   ; // [20..18] 
        unsigned int    Reserved_24           : 7   ; // [27..21] 
        unsigned int    w_arb_mode            : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOWBCARB1;

// Define the union U_VOUFSTA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    v0_uf_sta             : 1   ; // [0] 
        unsigned int    v1_uf_sta             : 1   ; // [1] 
        unsigned int    Reserved_28           : 1   ; // [2] 
        unsigned int    v3_uf_sta             : 1   ; // [3] 
        unsigned int    Reserved_27           : 4   ; // [7..4] 
        unsigned int    g0_uf_sta             : 1   ; // [8] 
        unsigned int    g1_uf_sta             : 1   ; // [9] 
        unsigned int    g2_uf_sta             : 1   ; // [10] 
        unsigned int    Reserved_26           : 1   ; // [11] 
        unsigned int    g4_uf_sta             : 1   ; // [12] 
        unsigned int    Reserved_25           : 19  ; // [31..13] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOUFSTA;

// Define the union U_VOUFCLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    v0_uf_clr             : 1   ; // [0] 
        unsigned int    v1_uf_clr             : 1   ; // [1] 
        unsigned int    Reserved_32           : 1   ; // [2] 
        unsigned int    v3_uf_clr             : 1   ; // [3] 
        unsigned int    Reserved_31           : 4   ; // [7..4] 
        unsigned int    g0_uf_clr             : 1   ; // [8] 
        unsigned int    g1_uf_clr             : 1   ; // [9] 
        unsigned int    g2_uf_clr             : 1   ; // [10] 
        unsigned int    Reserved_30           : 1   ; // [11] 
        unsigned int    g4_uf_clr             : 1   ; // [12] 
        unsigned int    Reserved_29           : 19  ; // [31..13] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOUFCLR;

// Define the union U_VOINTPROC_TIM
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vointproc_time        : 24  ; // [23..0] 
        unsigned int    Reserved_33           : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOINTPROC_TIM;

// Define the union U_VO_MUX
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sddate_sel            : 1   ; // [0] 
        unsigned int    hddate_sel            : 1   ; // [1] 
        unsigned int    vga_sel               : 1   ; // [2] 
        unsigned int    hdmi_sel              : 1   ; // [3] 
        unsigned int    digital_sel           : 2   ; // [5..4] 
        unsigned int    Reserved_35           : 26  ; // [31..6] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VO_MUX;

// Define the union U_VO_MUX_DAC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dac0_sel              : 4   ; // [3..0] 
        unsigned int    dac1_sel              : 4   ; // [7..4] 
        unsigned int    dac2_sel              : 4   ; // [11..8] 
        unsigned int    dac3_sel              : 4   ; // [15..12] 
        unsigned int    Reserved_37           : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VO_MUX_DAC;

// Define the union U_VO_MUX_TESTSYNC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    test_dv               : 1   ; // [0] 
        unsigned int    test_hsync            : 1   ; // [1] 
        unsigned int    test_vsync            : 1   ; // [2] 
        unsigned int    test_field            : 1   ; // [3] 
        unsigned int    Reserved_38           : 27  ; // [30..4] 
        unsigned int    vo_test_en            : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VO_MUX_TESTSYNC;

// Define the union U_VO_MUX_TESTDATA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    test_data             : 30  ; // [29..0] 
        unsigned int    Reserved_39           : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VO_MUX_TESTDATA;

// Define the union U_VO_DAC_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_41           : 16  ; // [15..0] 
        unsigned int    trim_vcml             : 3   ; // [18..16] 
        unsigned int    Reserved_40           : 11  ; // [29..19] 
        unsigned int    mode_bg               : 1   ; // [30] 
        unsigned int    en_bg                 : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VO_DAC_CTRL;

// Define the union U_VO_CABLE_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cable_ctrl0           : 2   ; // [1..0] 
        unsigned int    cable_ctrl1           : 2   ; // [3..2] 
        unsigned int    cable_ctrl2           : 2   ; // [5..4] 
        unsigned int    cable_ctrl3           : 2   ; // [7..6] 
        unsigned int    pdchopper             : 1   ; // [8] 
        unsigned int    Reserved_43           : 23  ; // [31..9] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VO_CABLE_CTRL;

// Define the union U_VO_DAC_C_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_45           : 15  ; // [14..0] 
        unsigned int    ct_c                  : 6   ; // [20..15] 
        unsigned int    Reserved_44           : 10  ; // [30..21] 
        unsigned int    en_c                  : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VO_DAC_C_CTRL;

// Define the union U_VO_DAC_R_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_48           : 15  ; // [14..0] 
        unsigned int    ct_r                  : 6   ; // [20..15] 
        unsigned int    Reserved_47           : 10  ; // [30..21] 
        unsigned int    en_r                  : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VO_DAC_R_CTRL;

// Define the union U_VO_DAC_G_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_50           : 15  ; // [14..0] 
        unsigned int    ct_g                  : 6   ; // [20..15] 
        unsigned int    Reserved_49           : 10  ; // [30..21] 
        unsigned int    en_g                  : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VO_DAC_G_CTRL;

// Define the union U_VO_DAC_B_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_52           : 15  ; // [14..0] 
        unsigned int    ct_b                  : 6   ; // [20..15] 
        unsigned int    Reserved_51           : 10  ; // [30..21] 
        unsigned int    en_b                  : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VO_DAC_B_CTRL;

// Define the union U_WBC_DHD_LOCATE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wbc_dhd_locate        : 1   ; // [0] 
        unsigned int    Reserved_53           : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD_LOCATE;

// Define the union U_WBC_OFL_EN
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wbc_ofl_en            : 1   ; // [0] 
        unsigned int    wbc_ofl_pro           : 1   ; // [1] 
        unsigned int    Reserved_55           : 30  ; // [31..2] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_OFL_EN;

// Define the union U_COEF_DATA
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_data               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_COEF_DATA;
// Define the union U_V0_PARARD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    v0_hlcoef_rd          : 1   ; // [0] 
        unsigned int    v0_hccoef_rd          : 1   ; // [1] 
        unsigned int    v0_vlcoef_rd          : 1   ; // [2] 
        unsigned int    v0_vccoef_rd          : 1   ; // [3] 
        unsigned int    v0_sr_hlcoef_rd       : 1   ; // [4] 
        unsigned int    v0_sr_hccoef_rd       : 1   ; // [5] 
        unsigned int    v0_sr_vlcoef_rd       : 1   ; // [6] 
        unsigned int    v0_sr_vccoef_rd       : 1   ; // [7] 
        unsigned int    Reserved_57           : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_PARARD;

// Define the union U_V1_PARARD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_60           : 3   ; // [2..0] 
        unsigned int    v1_vccoef_rd          : 1   ; // [3] 
        unsigned int    Reserved_59           : 28  ; // [31..4] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V1_PARARD;

// Define the union U_V3_PARARD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    v3_hlcoef_rd          : 1   ; // [0] 
        unsigned int    v3_hccoef_rd          : 1   ; // [1] 
        unsigned int    v3_vlcoef_rd          : 1   ; // [2] 
        unsigned int    v3_vccoef_rd          : 1   ; // [3] 
        unsigned int    Reserved_61           : 28  ; // [31..4] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V3_PARARD;

// Define the union U_VP0_PARARD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vp0_acmlut_rd         : 1   ; // [0] 
        unsigned int    vp0_dcihbw_rd         : 1   ; // [1] 
        unsigned int    vp0_dcidiv_rd         : 1   ; // [2] 
        unsigned int    vp0_dcilut_rd         : 1   ; // [3] 
        unsigned int    Reserved_63           : 28  ; // [31..4] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_PARARD;

// Define the union U_GP0_PARARD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    gp0_hlcoef_rd         : 1   ; // [0] 
        unsigned int    gp0_hccoef_rd         : 1   ; // [1] 
        unsigned int    gp0_vlcoef_rd         : 1   ; // [2] 
        unsigned int    gp0_vccoef_rd         : 1   ; // [3] 
        unsigned int    gp0_gti_hlcoef_rd     : 1   ; // [4] 
        unsigned int    gp0_gti_hccoef_rd     : 1   ; // [5] 
        unsigned int    gp0_gti_vlcoef_rd     : 1   ; // [6] 
        unsigned int    gp0_gti_vccoef_rd     : 1   ; // [7] 
        unsigned int    Reserved_65           : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_PARARD;

// Define the union U_GP1_PARARD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    gp1_hlcoef_rd         : 1   ; // [0] 
        unsigned int    gp1_hccoef_rd         : 1   ; // [1] 
        unsigned int    gp1_vlcoef_rd         : 1   ; // [2] 
        unsigned int    gp1_vccoef_rd         : 1   ; // [3] 
        unsigned int    gp1_gti_hlcoef_rd     : 1   ; // [4] 
        unsigned int    gp1_gti_hccoef_rd     : 1   ; // [5] 
        unsigned int    gp1_gti_vlcoef_rd     : 1   ; // [6] 
        unsigned int    gp1_gti_vccoef_rd     : 1   ; // [7] 
        unsigned int    Reserved_67           : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP1_PARARD;

// Define the union U_WBCDHD_PARARD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wbcdhd_hlcoef_rd      : 1   ; // [0] 
        unsigned int    wbcdhd_hccoef_rd      : 1   ; // [1] 
        unsigned int    wbcdhd_vlcoef_rd      : 1   ; // [2] 
        unsigned int    wbcdhd_vccoef_rd      : 1   ; // [3] 
        unsigned int    Reserved_68           : 28  ; // [31..4] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBCDHD_PARARD;

// Define the union U_DHD0_PARARD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dhd0_gmmr_rd          : 1   ; // [0] 
        unsigned int    dhd0_gmmg_rd          : 1   ; // [1] 
        unsigned int    dhd0_gmmb_rd          : 1   ; // [2] 
        unsigned int    Reserved_70           : 29  ; // [31..3] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_PARARD;

// Define the union U_DHD1_PARARD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dhd1_gmmr_rd          : 1   ; // [0] 
        unsigned int    dhd1_gmmg_rd          : 1   ; // [1] 
        unsigned int    dhd1_gmmb_rd          : 1   ; // [2] 
        unsigned int    Reserved_72           : 29  ; // [31..3] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD1_PARARD;

// Define the union U_V0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ifmt                  : 4   ; // [3..0] 
        unsigned int    Reserved_77           : 1   ; // [4] 
        unsigned int    time_out              : 3   ; // [7..5] 
        unsigned int    dcmp_en               : 1   ; // [8] 
        unsigned int    Reserved_76           : 1   ; // [9] 
        unsigned int    nosec_flag            : 1   ; // [10] 
        unsigned int    uv_order              : 1   ; // [11] 
        unsigned int    chm_rmode             : 2   ; // [13..12] 
        unsigned int    lm_rmode              : 2   ; // [15..14] 
        unsigned int    Reserved_75           : 1   ; // [16] 
        unsigned int    vup_mode              : 1   ; // [17] 
        unsigned int    ifir_mode             : 2   ; // [19..18] 
        unsigned int    upd_mode              : 1   ; // [20] 
        unsigned int    zme_mem_mode          : 1   ; // [21] 
        unsigned int    Reserved_74           : 4   ; // [25..22] 
        unsigned int    flip_en               : 1   ; // [26] 
        unsigned int    mute_en               : 1   ; // [27] 
        unsigned int    Reserved_73           : 3   ; // [30..28] 
        unsigned int    surface_en            : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_CTRL;

// Define the union U_V0_UPD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0] 
        unsigned int    Reserved_79           : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_UPD;

// Define the union U_V0_PRERD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_80           : 31  ; // [30..0] 
        unsigned int    pre_rd_en             : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_PRERD;

// Define the union U_V0_IRESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iw                    : 12  ; // [11..0] 
        unsigned int    ih                    : 12  ; // [23..12] 
        unsigned int    Reserved_82           : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_IRESO;

// Define the union U_V0_ORESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ow                    : 12  ; // [11..0] 
        unsigned int    oh                    : 12  ; // [23..12] 
        unsigned int    Reserved_84           : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_ORESO;

// Define the union U_V0_CBMPARA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    galpha                : 8   ; // [7..0] 
        unsigned int    Reserved_85           : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_CBMPARA;

// Define the union U_V0_SR_ORESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ow                    : 12  ; // [11..0] 
        unsigned int    oh                    : 12  ; // [23..12] 
        unsigned int    Reserved_87           : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_SR_ORESO;

// Define the union U_V0_PARAUP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    v0_hlcoef_upd         : 1   ; // [0] 
        unsigned int    v0_hccoef_upd         : 1   ; // [1] 
        unsigned int    v0_vlcoef_upd         : 1   ; // [2] 
        unsigned int    v0_vccoef_upd         : 1   ; // [3] 
        unsigned int    v0_sr_hlcoef_upd      : 1   ; // [4] 
        unsigned int    v0_sr_hccoef_upd      : 1   ; // [5] 
        unsigned int    v0_sr_vlcoef_upd      : 1   ; // [6] 
        unsigned int    v0_sr_vccoef_upd      : 1   ; // [7] 
        unsigned int    Reserved_88           : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_PARAUP;

// Define the union U_V0_CPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    src_xfpos             : 12  ; // [11..0] 
        unsigned int    src_xlpos             : 12  ; // [23..12] 
        unsigned int    Reserved_89           : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_CPOS;

// Define the union U_V0_DRAWMODE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    draw_mode             : 2   ; // [1..0] 
        unsigned int    Reserved_90           : 30  ; // [31..2] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_DRAWMODE;

// Define the union U_V0_HLCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_HLCOEFAD;
// Define the union U_V0_HCCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_HCCOEFAD;
// Define the union U_V0_VLCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_VLCOEFAD;
// Define the union U_V0_VCCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_VCCOEFAD;
// Define the union U_V0_DFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disp_xfpos            : 12  ; // [11..0] 
        unsigned int    disp_yfpos            : 12  ; // [23..12] 
        unsigned int    Reserved_92           : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_DFPOS;

// Define the union U_V0_DLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disp_xlpos            : 12  ; // [11..0] 
        unsigned int    disp_ylpos            : 12  ; // [23..12] 
        unsigned int    Reserved_93           : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_DLPOS;

// Define the union U_V0_VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_94           : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_VFPOS;

// Define the union U_V0_VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_95           : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_VLPOS;

// Define the union U_V0_BK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbk_cr                : 10  ; // [9..0] 
        unsigned int    vbk_cb                : 10  ; // [19..10] 
        unsigned int    vbk_y                 : 10  ; // [29..20] 
        unsigned int    Reserved_96           : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_BK;

// Define the union U_V0_ALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbk_alpha             : 8   ; // [7..0] 
        unsigned int    Reserved_97           : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_ALPHA;

// Define the union U_V0_CSC_IDC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc0               : 11  ; // [10..0] 
        unsigned int    cscidc1               : 11  ; // [21..11] 
        unsigned int    csc_en                : 1   ; // [22] 
        unsigned int    Reserved_98           : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_CSC_IDC;

// Define the union U_V0_CSC_ODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscodc0               : 11  ; // [10..0] 
        unsigned int    cscodc1               : 11  ; // [21..11] 
        unsigned int    csc_sign_mode         : 1   ; // [22] 
        unsigned int    Reserved_100          : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_CSC_ODC;

// Define the union U_V0_CSC_IODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc2               : 11  ; // [10..0] 
        unsigned int    cscodc2               : 11  ; // [21..11] 
        unsigned int    Reserved_101          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_CSC_IODC;

// Define the union U_V0_CSC_P0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp00                : 15  ; // [14..0] 
        unsigned int    Reserved_103          : 1   ; // [15] 
        unsigned int    cscp01                : 15  ; // [30..16] 
        unsigned int    Reserved_102          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_CSC_P0;

// Define the union U_V0_CSC_P1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp02                : 15  ; // [14..0] 
        unsigned int    Reserved_105          : 1   ; // [15] 
        unsigned int    cscp10                : 15  ; // [30..16] 
        unsigned int    Reserved_104          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_CSC_P1;

// Define the union U_V0_CSC_P2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp11                : 15  ; // [14..0] 
        unsigned int    Reserved_107          : 1   ; // [15] 
        unsigned int    cscp12                : 15  ; // [30..16] 
        unsigned int    Reserved_106          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_CSC_P2;

// Define the union U_V0_CSC_P3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp20                : 15  ; // [14..0] 
        unsigned int    Reserved_109          : 1   ; // [15] 
        unsigned int    cscp21                : 15  ; // [30..16] 
        unsigned int    Reserved_108          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_CSC_P3;

// Define the union U_V0_CSC_P4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp22                : 15  ; // [14..0] 
        unsigned int    Reserved_110          : 17  ; // [31..15] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_CSC_P4;

// Define the union U_SR_HLCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int sr_hlcoefad             : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_SR_HLCOEFAD;
// Define the union U_SR_HCCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int sr_hccoefad             : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_SR_HCCOEFAD;
// Define the union U_SR_VLCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int sr_vlcoefad             : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_SR_VLCOEFAD;
// Define the union U_SR_VCCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int sr_vccoefad             : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_SR_VCCOEFAD;
// Define the union U_V0_HSP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hratio                : 24  ; // [23..0] 
        unsigned int    hfir_order            : 1   ; // [24] 
        unsigned int    hchfir_en             : 1   ; // [25] 
        unsigned int    hlfir_en              : 1   ; // [26] 
        unsigned int    Reserved_111          : 1   ; // [27] 
        unsigned int    hchmid_en             : 1   ; // [28] 
        unsigned int    hlmid_en              : 1   ; // [29] 
        unsigned int    hchmsc_en             : 1   ; // [30] 
        unsigned int    hlmsc_en              : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_HSP;

// Define the union U_V0_HLOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hor_loffset           : 28  ; // [27..0] 
        unsigned int    Reserved_113          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_HLOFFSET;

// Define the union U_V0_HCOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hor_coffset           : 28  ; // [27..0] 
        unsigned int    Reserved_114          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_HCOFFSET;

// Define the union U_V0_VSP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_116          : 19  ; // [18..0] 
        unsigned int    zme_in_fmt            : 2   ; // [20..19] 
        unsigned int    zme_out_fmt           : 2   ; // [22..21] 
        unsigned int    vchfir_en             : 1   ; // [23] 
        unsigned int    vlfir_en              : 1   ; // [24] 
        unsigned int    Reserved_115          : 3   ; // [27..25] 
        unsigned int    vchmid_en             : 1   ; // [28] 
        unsigned int    vlmid_en              : 1   ; // [29] 
        unsigned int    vchmsc_en             : 1   ; // [30] 
        unsigned int    vlmsc_en              : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_VSP;

// Define the union U_V0_VSR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vratio                : 16  ; // [15..0] 
        unsigned int    Reserved_118          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_VSR;

// Define the union U_V0_VOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vchroma_offset        : 16  ; // [15..0] 
        unsigned int    vluma_offset          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_VOFFSET;

// Define the union U_V0_VBOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbchroma_offset       : 16  ; // [15..0] 
        unsigned int    vbluma_offset         : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_VBOFFSET;

// Define the union U_V0_IFIRCOEF01
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef0                 : 10  ; // [9..0] 
        unsigned int    Reserved_120          : 6   ; // [15..10] 
        unsigned int    coef1                 : 10  ; // [25..16] 
        unsigned int    Reserved_119          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_IFIRCOEF01;

// Define the union U_V0_IFIRCOEF23
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef2                 : 10  ; // [9..0] 
        unsigned int    Reserved_123          : 6   ; // [15..10] 
        unsigned int    coef3                 : 10  ; // [25..16] 
        unsigned int    Reserved_122          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_IFIRCOEF23;

// Define the union U_V0_IFIRCOEF45
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef4                 : 10  ; // [9..0] 
        unsigned int    Reserved_125          : 6   ; // [15..10] 
        unsigned int    coef5                 : 10  ; // [25..16] 
        unsigned int    Reserved_124          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_IFIRCOEF45;

// Define the union U_V0_IFIRCOEF67
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef6                 : 10  ; // [9..0] 
        unsigned int    Reserved_127          : 6   ; // [15..10] 
        unsigned int    coef7                 : 10  ; // [25..16] 
        unsigned int    Reserved_126          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_IFIRCOEF67;

// Define the union U_V0_P0RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_128          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P0RESO;

// Define the union U_V0_P0LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P0LADDR;
// Define the union U_V0_P0CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P0CADDR;
// Define the union U_V0_P0STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P0STRIDE;

// Define the union U_V0_P0VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_130          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P0VFPOS;

// Define the union U_V0_P0VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_131          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P0VLPOS;

// Define the union U_V0_P1RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_132          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P1RESO;

// Define the union U_V0_P1LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P1LADDR;
// Define the union U_V0_P1CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P1CADDR;
// Define the union U_V0_P1STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P1STRIDE;

// Define the union U_V0_P1VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_134          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P1VFPOS;

// Define the union U_V0_P1VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_135          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P1VLPOS;

// Define the union U_V0_P2RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_136          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P2RESO;

// Define the union U_V0_P2LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P2LADDR;
// Define the union U_V0_P2CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P2CADDR;
// Define the union U_V0_P2STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P2STRIDE;

// Define the union U_V0_P2VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_138          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P2VFPOS;

// Define the union U_V0_P2VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_139          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P2VLPOS;

// Define the union U_V0_P3RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_140          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P3RESO;

// Define the union U_V0_P3LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P3LADDR;
// Define the union U_V0_P3CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P3CADDR;
// Define the union U_V0_P3STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P3STRIDE;

// Define the union U_V0_P3VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_142          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P3VFPOS;

// Define the union U_V0_P3VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_143          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P3VLPOS;

// Define the union U_V0_P4RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_144          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P4RESO;

// Define the union U_V0_P4LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P4LADDR;
// Define the union U_V0_P4CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P4CADDR;
// Define the union U_V0_P4STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P4STRIDE;

// Define the union U_V0_P4VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_146          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P4VFPOS;

// Define the union U_V0_P4VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_147          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P4VLPOS;

// Define the union U_V0_P5RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_148          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P5RESO;

// Define the union U_V0_P5LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P5LADDR;
// Define the union U_V0_P5CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P5CADDR;
// Define the union U_V0_P5STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P5STRIDE;

// Define the union U_V0_P5VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_150          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P5VFPOS;

// Define the union U_V0_P5VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_151          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P5VLPOS;

// Define the union U_V0_P6RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_152          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P6RESO;

// Define the union U_V0_P6LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P6LADDR;
// Define the union U_V0_P6CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P6CADDR;
// Define the union U_V0_P6STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P6STRIDE;

// Define the union U_V0_P6VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_154          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P6VFPOS;

// Define the union U_V0_P6VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_155          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P6VLPOS;

// Define the union U_V0_P7RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_156          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P7RESO;

// Define the union U_V0_P7LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P7LADDR;
// Define the union U_V0_P7CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P7CADDR;
// Define the union U_V0_P7STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P7STRIDE;

// Define the union U_V0_P7VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_158          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P7VFPOS;

// Define the union U_V0_P7VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_159          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P7VLPOS;

// Define the union U_V0_P8RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_160          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P8RESO;

// Define the union U_V0_P8LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P8LADDR;
// Define the union U_V0_P8CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P8CADDR;
// Define the union U_V0_P8STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P8STRIDE;

// Define the union U_V0_P8VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_162          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P8VFPOS;

// Define the union U_V0_P8VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_163          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P8VLPOS;

// Define the union U_V0_P9RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_164          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P9RESO;

// Define the union U_V0_P9LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P9LADDR;
// Define the union U_V0_P9CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P9CADDR;
// Define the union U_V0_P9STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P9STRIDE;

// Define the union U_V0_P9VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_166          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P9VFPOS;

// Define the union U_V0_P9VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_167          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P9VLPOS;

// Define the union U_V0_P10RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_168          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P10RESO;

// Define the union U_V0_P10LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P10LADDR;
// Define the union U_V0_P10CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P10CADDR;
// Define the union U_V0_P10STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P10STRIDE;

// Define the union U_V0_P10VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_170          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P10VFPOS;

// Define the union U_V0_P10VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_171          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P10VLPOS;

// Define the union U_V0_P11RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_172          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P11RESO;

// Define the union U_V0_P11LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P11LADDR;
// Define the union U_V0_P11CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P11CADDR;
// Define the union U_V0_P11STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P11STRIDE;

// Define the union U_V0_P11VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_174          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P11VFPOS;

// Define the union U_V0_P11VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_175          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P11VLPOS;

// Define the union U_V0_P12RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_176          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P12RESO;

// Define the union U_V0_P12LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P12LADDR;
// Define the union U_V0_P12CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P12CADDR;
// Define the union U_V0_P12STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P12STRIDE;

// Define the union U_V0_P12VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_178          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P12VFPOS;

// Define the union U_V0_P12VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_179          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P12VLPOS;

// Define the union U_V0_P13RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_180          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P13RESO;

// Define the union U_V0_P13LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P13LADDR;
// Define the union U_V0_P13CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P13CADDR;
// Define the union U_V0_P13STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P13STRIDE;

// Define the union U_V0_P13VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_182          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P13VFPOS;

// Define the union U_V0_P13VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_183          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P13VLPOS;

// Define the union U_V0_P14RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_184          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P14RESO;

// Define the union U_V0_P14LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P14LADDR;
// Define the union U_V0_P14CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P14CADDR;
// Define the union U_V0_P14STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P14STRIDE;

// Define the union U_V0_P14VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_186          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P14VFPOS;

// Define the union U_V0_P14VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_187          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P14VLPOS;

// Define the union U_V0_P15RESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    w                     : 12  ; // [11..0] 
        unsigned int    Reserved_188          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P15RESO;

// Define the union U_V0_P15LADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P15LADDR;
// Define the union U_V0_P15CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_P15CADDR;
// Define the union U_V0_P15STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    surface_cstride       : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P15STRIDE;

// Define the union U_V0_P15VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_190          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P15VFPOS;

// Define the union U_V0_P15VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_191          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_P15VLPOS;

// Define the union U_SR_HCOEF0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hlcoef02              : 10  ; // [9..0] 
        unsigned int    hlcoef01              : 10  ; // [19..10] 
        unsigned int    hlcoef00              : 10  ; // [29..20] 
        unsigned int    Reserved_192          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HCOEF0;

// Define the union U_SR_HCOEF1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hlcoef05              : 10  ; // [9..0] 
        unsigned int    hlcoef04              : 10  ; // [19..10] 
        unsigned int    hlcoef03              : 10  ; // [29..20] 
        unsigned int    Reserved_194          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HCOEF1;

// Define the union U_SR_HCOEF2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hlcoef10              : 10  ; // [9..0] 
        unsigned int    hlcoef07              : 10  ; // [19..10] 
        unsigned int    hlcoef06              : 10  ; // [29..20] 
        unsigned int    Reserved_195          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HCOEF2;

// Define the union U_SR_HCOEF3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hlcoef13              : 10  ; // [9..0] 
        unsigned int    hlcoef12              : 10  ; // [19..10] 
        unsigned int    hlcoef11              : 10  ; // [29..20] 
        unsigned int    Reserved_196          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HCOEF3;

// Define the union U_SR_HCOEF4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hlcoef16              : 10  ; // [9..0] 
        unsigned int    hlcoef15              : 10  ; // [19..10] 
        unsigned int    hlcoef14              : 10  ; // [29..20] 
        unsigned int    Reserved_197          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HCOEF4;

// Define the union U_SR_HCOEF5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hccoef01              : 10  ; // [9..0] 
        unsigned int    hccoef00              : 10  ; // [19..10] 
        unsigned int    hlcoef17              : 10  ; // [29..20] 
        unsigned int    Reserved_198          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HCOEF5;

// Define the union U_SR_HCOEF6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hccoef04              : 10  ; // [9..0] 
        unsigned int    hccoef03              : 10  ; // [19..10] 
        unsigned int    hccoef02              : 10  ; // [29..20] 
        unsigned int    Reserved_199          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HCOEF6;

// Define the union U_SR_HCOEF7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hccoef07              : 10  ; // [9..0] 
        unsigned int    hccoef06              : 10  ; // [19..10] 
        unsigned int    hccoef05              : 10  ; // [29..20] 
        unsigned int    Reserved_200          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HCOEF7;

// Define the union U_SR_HCOEF8
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hccoef12              : 10  ; // [9..0] 
        unsigned int    hccoef11              : 10  ; // [19..10] 
        unsigned int    hccoef10              : 10  ; // [29..20] 
        unsigned int    Reserved_201          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HCOEF8;

// Define the union U_SR_HCOEF9
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hccoef15              : 10  ; // [9..0] 
        unsigned int    hccoef14              : 10  ; // [19..10] 
        unsigned int    hccoef13              : 10  ; // [29..20] 
        unsigned int    Reserved_202          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HCOEF9;

// Define the union U_SR_HCOEF10
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hccoef17              : 10  ; // [9..0] 
        unsigned int    hccoef16              : 10  ; // [19..10] 
        unsigned int    Reserved_203          : 12  ; // [31..20] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HCOEF10;

// Define the union U_SR_VCOEF0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vlcoef02              : 10  ; // [9..0] 
        unsigned int    vlcoef01              : 10  ; // [19..10] 
        unsigned int    vlcoef00              : 10  ; // [29..20] 
        unsigned int    Reserved_204          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_VCOEF0;

// Define the union U_SR_VCOEF1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vlcoef05              : 10  ; // [9..0] 
        unsigned int    vlcoef04              : 10  ; // [19..10] 
        unsigned int    vlcoef03              : 10  ; // [29..20] 
        unsigned int    Reserved_205          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_VCOEF1;

// Define the union U_SR_VCOEF2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vlcoef12              : 10  ; // [9..0] 
        unsigned int    vlcoef11              : 10  ; // [19..10] 
        unsigned int    vlcoef10              : 10  ; // [29..20] 
        unsigned int    Reserved_206          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_VCOEF2;

// Define the union U_SR_VCOEF3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vlcoef15              : 10  ; // [9..0] 
        unsigned int    vlcoef14              : 10  ; // [19..10] 
        unsigned int    vlcoef13              : 10  ; // [29..20] 
        unsigned int    Reserved_207          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_VCOEF3;

// Define the union U_SR_VCOEF4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vccoef02              : 10  ; // [9..0] 
        unsigned int    vccoef01              : 10  ; // [19..10] 
        unsigned int    vccoef00              : 10  ; // [29..20] 
        unsigned int    Reserved_208          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_VCOEF4;

// Define the union U_SR_VCOEF5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vccoef05              : 10  ; // [9..0] 
        unsigned int    vccoef04              : 10  ; // [19..10] 
        unsigned int    vccoef03              : 10  ; // [29..20] 
        unsigned int    Reserved_209          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_VCOEF5;

// Define the union U_SR_VCOEF6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vccoef12              : 10  ; // [9..0] 
        unsigned int    vccoef11              : 10  ; // [19..10] 
        unsigned int    vccoef10              : 10  ; // [29..20] 
        unsigned int    Reserved_210          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_VCOEF6;

// Define the union U_SR_VCOEF7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vccoef15              : 10  ; // [9..0] 
        unsigned int    vccoef14              : 10  ; // [19..10] 
        unsigned int    vccoef13              : 10  ; // [29..20] 
        unsigned int    Reserved_211          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_VCOEF7;

// Define the union U_SR_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_212          : 29  ; // [28..0] 
        unsigned int    sr_disp_state         : 2   ; // [30..29] 
        unsigned int    sr_en                 : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_CTRL;

// Define the union U_SR_LDIR_RATIO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    grd_sub_ratio         : 6   ; // [5..0] 
        unsigned int    Reserved_214          : 26  ; // [31..6] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_LDIR_RATIO;

// Define the union U_SR_LTEX_THR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    grd_thrl              : 11  ; // [10..0] 
        unsigned int    Reserved_216          : 1   ; // [11] 
        unsigned int    grd_thrh              : 11  ; // [22..12] 
        unsigned int    Reserved_215          : 1   ; // [23] 
        unsigned int    grd_thrh_subl         : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_LTEX_THR;

// Define the union U_SR_LTEX_ADD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    grd_add_ctrl          : 8   ; // [7..0] 
        unsigned int    Reserved_217          : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_LTEX_ADD;

// Define the union U_SR_DIPL_DETAIL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disc_detail_gain      : 12  ; // [11..0] 
        unsigned int    Reserved_218          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_DIPL_DETAIL;

// Define the union U_SR_SHARP_EN
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ensc_detail_gain      : 12  ; // [11..0] 
        unsigned int    resvered              : 18  ; // [29..12] 
        unsigned int    hsh_nodir             : 1   ; // [30] 
        unsigned int    sharp_en              : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_SHARP_EN;

// Define the union U_SR_SHARP_THR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    under_thrsh           : 8   ; // [7..0] 
        unsigned int    over_thrsh            : 8   ; // [15..8] 
        unsigned int    sharp_gain            : 10  ; // [25..16] 
        unsigned int    resvered              : 2   ; // [27..26] 
        unsigned int    sharp_coring          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_SHARP_THR;

// Define the union U_SR_SMOOTH
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    plus_ctrl             : 10  ; // [9..0] 
        unsigned int    resvered              : 20  ; // [29..10] 
        unsigned int    plus_en               : 1   ; // [30] 
        unsigned int    smooth_en             : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_SMOOTH;

// Define the union U_SR_HSP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hratio                : 24  ; // [23..0] 
        unsigned int    Reserved_219          : 1   ; // [24] 
        unsigned int    hchfir_en             : 1   ; // [25] 
        unsigned int    hlfir_en              : 1   ; // [26] 
        unsigned int    hfir_order            : 1   ; // [27] 
        unsigned int    hchmid_en             : 1   ; // [28] 
        unsigned int    hlmid_en              : 1   ; // [29] 
        unsigned int    hchmsc_en             : 1   ; // [30] 
        unsigned int    hlmsc_en              : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HSP;

// Define the union U_SR_HLOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hor_loffset           : 28  ; // [27..0] 
        unsigned int    Reserved_221          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HLOFFSET;

// Define the union U_SR_HCOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hor_coffset           : 28  ; // [27..0] 
        unsigned int    Reserved_222          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_HCOFFSET;

// Define the union U_SR_VSP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_224          : 19  ; // [18..0] 
        unsigned int    zme_in_fmt            : 2   ; // [20..19] 
        unsigned int    zme_out_fmt           : 2   ; // [22..21] 
        unsigned int    vchfir_en             : 1   ; // [23] 
        unsigned int    vlfir_en              : 1   ; // [24] 
        unsigned int    Reserved_223          : 3   ; // [27..25] 
        unsigned int    vchmid_en             : 1   ; // [28] 
        unsigned int    vlmid_en              : 1   ; // [29] 
        unsigned int    vchmsc_en             : 1   ; // [30] 
        unsigned int    vlmsc_en              : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_VSP;

// Define the union U_SR_VSR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vratio                : 16  ; // [15..0] 
        unsigned int    Reserved_226          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_VSR;

// Define the union U_SR_VOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vchroma_offset        : 16  ; // [15..0] 
        unsigned int    vluma_offset          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_VOFFSET;

// Define the union U_SR_VBOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbchroma_offset       : 16  ; // [15..0] 
        unsigned int    vbluma_offset         : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SR_VBOFFSET;

// Define the union U_V0_NADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_naddr           : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_NADDR;
// Define the union U_V0_NCADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_ncaddr          : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_NCADDR;
// Define the union U_V0_MULTI_MODE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mrg_mode              : 1   ; // [0] 
        unsigned int    Reserved_228          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_MULTI_MODE;

// Define the union U_V0_LADDROFFSET
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int laddr_offset            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_LADDROFFSET;
// Define the union U_V0_CADDROFFSET
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int caddr_offset            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_CADDROFFSET;
// Define the union U_V0_16REGIONENL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    p0_en                 : 1   ; // [0] 
        unsigned int    p1_en                 : 1   ; // [1] 
        unsigned int    p2_en                 : 1   ; // [2] 
        unsigned int    p3_en                 : 1   ; // [3] 
        unsigned int    p4_en                 : 1   ; // [4] 
        unsigned int    p5_en                 : 1   ; // [5] 
        unsigned int    p6_en                 : 1   ; // [6] 
        unsigned int    p7_en                 : 1   ; // [7] 
        unsigned int    p8_en                 : 1   ; // [8] 
        unsigned int    p9_en                 : 1   ; // [9] 
        unsigned int    p10_en                : 1   ; // [10] 
        unsigned int    p11_en                : 1   ; // [11] 
        unsigned int    p12_en                : 1   ; // [12] 
        unsigned int    p13_en                : 1   ; // [13] 
        unsigned int    p14_en                : 1   ; // [14] 
        unsigned int    p15_en                : 1   ; // [15] 
        unsigned int    Reserved_231          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_16REGIONENL;

// Define the union U_V0_16MUTE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    p0_mute_en            : 1   ; // [0] 
        unsigned int    p1_mute_en            : 1   ; // [1] 
        unsigned int    p2_mute_en            : 1   ; // [2] 
        unsigned int    p3_mute_en            : 1   ; // [3] 
        unsigned int    p4_mute_en            : 1   ; // [4] 
        unsigned int    p5_mute_en            : 1   ; // [5] 
        unsigned int    p6_mute_en            : 1   ; // [6] 
        unsigned int    p7_mute_en            : 1   ; // [7] 
        unsigned int    p8_mute_en            : 1   ; // [8] 
        unsigned int    p9_mute_en            : 1   ; // [9] 
        unsigned int    p10_mute_en           : 1   ; // [10] 
        unsigned int    p11_mute_en           : 1   ; // [11] 
        unsigned int    p12_mute_en           : 1   ; // [12] 
        unsigned int    p13_mute_en           : 1   ; // [13] 
        unsigned int    p14_mute_en           : 1   ; // [14] 
        unsigned int    p15_mute_en           : 1   ; // [15] 
        unsigned int    Reserved_233          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_16MUTE;

// Define the union U_V0_DCMP_LSTATE0
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int dcmp_l_state0           : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_DCMP_LSTATE0;
// Define the union U_V0_DCMP_LSTATE1
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int dcmp_l_state1           : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_DCMP_LSTATE1;
// Define the union U_V0_DCMP_CSTATE0
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int dcmp_c_state0           : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_DCMP_CSTATE0;
// Define the union U_V0_DCMP_CSTATE1
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int dcmp_c_state1           : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_V0_DCMP_CSTATE1;
// Define the union U_VO_DCMPERRCLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dcmp_l_errclr         : 1   ; // [0] 
        unsigned int    dcmp_c_errclr         : 1   ; // [1] 
        unsigned int    Reserved_236          : 30  ; // [31..2] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VO_DCMPERRCLR;

// Define the union U_V0_DCMP_ERR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dcmp_l_wrong          : 1   ; // [0] 
        unsigned int    dcmp_c_wrong          : 1   ; // [1] 
        unsigned int    Reserved_237          : 30  ; // [31..2] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_V0_DCMP_ERR;

// Define the union U_VP0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vp_galpha             : 8   ; // [7..0] 
        unsigned int    mute_en               : 1   ; // [8] 
        unsigned int    Reserved_238          : 23  ; // [31..9] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CTRL;

// Define the union U_VP0_UPD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0] 
        unsigned int    Reserved_240          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_UPD;

// Define the union U_VP0_DCI_CAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_VP0_DCI_CAD;
// Define the union U_VP0_ACM_CAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_VP0_ACM_CAD;
// Define the union U_VP0_PARAUP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vp0_acmcoef_upd       : 1   ; // [0] 
        unsigned int    vp0_acccoef_upd       : 1   ; // [1] 
        unsigned int    Reserved_242          : 30  ; // [31..2] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_PARAUP;

// Define the union U_VP0_IRESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iw                    : 12  ; // [11..0] 
        unsigned int    ih                    : 12  ; // [23..12] 
        unsigned int    Reserved_243          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_IRESO;

// Define the union U_VP0_DOF_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_245          : 31  ; // [30..0] 
        unsigned int    dof_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DOF_CTRL;

// Define the union U_VP0_DOF_STEP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    left_step             : 8   ; // [7..0] 
        unsigned int    right_step            : 8   ; // [15..8] 
        unsigned int    Reserved_247          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DOF_STEP;

// Define the union U_VP0_DCICTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_248          : 17  ; // [16..0] 
        unsigned int    dci_hist_state        : 1   ; // [17] 
        unsigned int    dci_swcfg_en          : 1   ; // [18] 
        unsigned int    dci_histlpf_en        : 1   ; // [19] 
        unsigned int    dci_shift_ctrl        : 2   ; // [21..20] 
        unsigned int    dci_out_range         : 1   ; // [22] 
        unsigned int    dci_in_range          : 1   ; // [23] 
        unsigned int    dci_cbcrsta_en        : 1   ; // [24] 
        unsigned int    dci_cbcrcmp_en        : 1   ; // [25] 
        unsigned int    dci_man_adj2          : 1   ; // [26] 
        unsigned int    dci_man_adj1          : 1   ; // [27] 
        unsigned int    dci_man_adj0          : 1   ; // [28] 
        unsigned int    dci_scene_flg         : 1   ; // [29] 
        unsigned int    dci_dbg_en            : 1   ; // [30] 
        unsigned int    dci_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCICTRL;

// Define the union U_VP0_DCIHPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_250          : 8   ; // [7..0] 
        unsigned int    dci_hend              : 12  ; // [19..8] 
        unsigned int    dci_hstart            : 12  ; // [31..20] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIHPOS;

// Define the union U_VP0_DCIVPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_251          : 8   ; // [7..0] 
        unsigned int    dci_vend              : 12  ; // [19..8] 
        unsigned int    dci_vstart            : 12  ; // [31..20] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIVPOS;

// Define the union U_VP0_DCIHISBLD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_252          : 8   ; // [7..0] 
        unsigned int    dci_cbcrsta_cr        : 8   ; // [15..8] 
        unsigned int    dci_cbcrsta_cb        : 8   ; // [23..16] 
        unsigned int    dci_cbcrsta_y         : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIHISBLD;

// Define the union U_VP0_DCIHISOFT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_253          : 23  ; // [22..0] 
        unsigned int    dci_cbcrsta_oft       : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIHISOFT;

// Define the union U_VP0_DCIHISCOR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_254          : 8   ; // [7..0] 
        unsigned int    dci_histcor_thr2      : 8   ; // [15..8] 
        unsigned int    dci_histcor_thr1      : 8   ; // [23..16] 
        unsigned int    dci_histcor_thr0      : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIHISCOR;

// Define the union U_VP0_DCIMERBLD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_255          : 2   ; // [1..0] 
        unsigned int    dci_org_abld          : 6   ; // [7..2] 
        unsigned int    dci_hist_abld         : 6   ; // [13..8] 
        unsigned int    dci_metrc_abld2       : 6   ; // [19..14] 
        unsigned int    dci_metrc_abld1       : 6   ; // [25..20] 
        unsigned int    dci_metrc_abld0       : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIMERBLD;

// Define the union U_VP0_DCIADJWGT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_256          : 8   ; // [7..0] 
        unsigned int    dci_man_adjwgt2       : 8   ; // [15..8] 
        unsigned int    dci_man_adjwgt1       : 8   ; // [23..16] 
        unsigned int    dci_man_adjwgt0       : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIADJWGT;

// Define the union U_VP0_DCICLIP0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_257          : 16  ; // [15..0] 
        unsigned int    dci_wgt_cliphigh0     : 8   ; // [23..16] 
        unsigned int    dci_wgt_cliplow0      : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCICLIP0;

// Define the union U_VP0_DCICLIP1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_258          : 16  ; // [15..0] 
        unsigned int    dci_wgt_cliphigh1     : 8   ; // [23..16] 
        unsigned int    dci_wgt_cliplow1      : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCICLIP1;

// Define the union U_VP0_DCICLIP2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_259          : 16  ; // [15..0] 
        unsigned int    dci_wgt_cliphigh2     : 8   ; // [23..16] 
        unsigned int    dci_wgt_cliplow2      : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCICLIP2;

// Define the union U_VP0_DCIGLBGAIN
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_260          : 14  ; // [13..0] 
        unsigned int    dci_glb_gain2         : 6   ; // [19..14] 
        unsigned int    dci_glb_gain1         : 6   ; // [25..20] 
        unsigned int    dci_glb_gain0         : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIGLBGAIN;

// Define the union U_VP0_DCIPOSTHR0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dci_gainpos_thr4      : 8   ; // [7..0] 
        unsigned int    dci_gainpos_thr3      : 8   ; // [15..8] 
        unsigned int    dci_gainpos_thr2      : 8   ; // [23..16] 
        unsigned int    dci_gainpos_thr1      : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIPOSTHR0;

// Define the union U_VP0_DCIPOSTHR1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_261          : 8   ; // [7..0] 
        unsigned int    dci_gainpos_thr7      : 8   ; // [15..8] 
        unsigned int    dci_gainpos_thr6      : 8   ; // [23..16] 
        unsigned int    dci_gainpos_thr5      : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIPOSTHR1;

// Define the union U_VP0_DCIPOSGAIN0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dci_gainpos_cbcr7     : 4   ; // [3..0] 
        unsigned int    dci_gainpos_cbcr6     : 4   ; // [7..4] 
        unsigned int    dci_gainpos_cbcr5     : 4   ; // [11..8] 
        unsigned int    dci_gainpos_cbcr4     : 4   ; // [15..12] 
        unsigned int    dci_gainpos_cbcr3     : 4   ; // [19..16] 
        unsigned int    dci_gainpos_cbcr2     : 4   ; // [23..20] 
        unsigned int    dci_gainpos_cbcr1     : 4   ; // [27..24] 
        unsigned int    dci_gainpos_cbcr0     : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIPOSGAIN0;

// Define the union U_VP0_DCIPOSGAIN1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_262          : 28  ; // [27..0] 
        unsigned int    dci_gainpos_cbcr8     : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIPOSGAIN1;

// Define the union U_VP0_DCIPOSSLP0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_263          : 2   ; // [1..0] 
        unsigned int    dci_gainpos_slp2      : 10  ; // [11..2] 
        unsigned int    dci_gainpos_slp1      : 10  ; // [21..12] 
        unsigned int    dci_gainpos_slp0      : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIPOSSLP0;

// Define the union U_VP0_DCIPOSSLP1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_264          : 2   ; // [1..0] 
        unsigned int    dci_gainpos_slp5      : 10  ; // [11..2] 
        unsigned int    dci_gainpos_slp4      : 10  ; // [21..12] 
        unsigned int    dci_gainpos_slp3      : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIPOSSLP1;

// Define the union U_VP0_DCIPOSSLP2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_265          : 12  ; // [11..0] 
        unsigned int    dci_gainpos_slp7      : 10  ; // [21..12] 
        unsigned int    dci_gainpos_slp6      : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIPOSSLP2;

// Define the union U_VP0_DCINEGTHR0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dci_gainneg_thr4      : 8   ; // [7..0] 
        unsigned int    dci_gainneg_thr3      : 8   ; // [15..8] 
        unsigned int    dci_gainneg_thr2      : 8   ; // [23..16] 
        unsigned int    dci_gainneg_thr1      : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCINEGTHR0;

// Define the union U_VP0_DCINEGTHR1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_266          : 8   ; // [7..0] 
        unsigned int    dci_gainneg_thr7      : 8   ; // [15..8] 
        unsigned int    dci_gainneg_thr6      : 8   ; // [23..16] 
        unsigned int    dci_gainneg_thr5      : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCINEGTHR1;

// Define the union U_VP0_DCINEGGAIN0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dci_gainneg_cbcr7     : 4   ; // [3..0] 
        unsigned int    dci_gainneg_cbcr6     : 4   ; // [7..4] 
        unsigned int    dci_gainneg_cbcr5     : 4   ; // [11..8] 
        unsigned int    dci_gainneg_cbcr4     : 4   ; // [15..12] 
        unsigned int    dci_gainneg_cbcr3     : 4   ; // [19..16] 
        unsigned int    dci_gainneg_cbcr2     : 4   ; // [23..20] 
        unsigned int    dci_gainneg_cbcr1     : 4   ; // [27..24] 
        unsigned int    dci_gainneg_cbcr0     : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCINEGGAIN0;

// Define the union U_VP0_DCINEGGAIN1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_267          : 28  ; // [27..0] 
        unsigned int    dci_gainneg_cbcr8     : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCINEGGAIN1;

// Define the union U_VP0_DCINEGSLP0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_268          : 2   ; // [1..0] 
        unsigned int    dci_gainneg_slp2      : 10  ; // [11..2] 
        unsigned int    dci_gainneg_slp1      : 10  ; // [21..12] 
        unsigned int    dci_gainneg_slp0      : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCINEGSLP0;

// Define the union U_VP0_DCINEGSLP1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_269          : 2   ; // [1..0] 
        unsigned int    dci_gainneg_slp5      : 10  ; // [11..2] 
        unsigned int    dci_gainneg_slp4      : 10  ; // [21..12] 
        unsigned int    dci_gainneg_slp3      : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCINEGSLP1;

// Define the union U_VP0_DCINEGSLP2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_270          : 12  ; // [11..0] 
        unsigned int    dci_gainneg_slp7      : 10  ; // [21..12] 
        unsigned int    dci_gainneg_slp6      : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCINEGSLP2;

// Define the union U_VP0_DCIADJSW0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_271          : 2   ; // [1..0] 
        unsigned int    dci_adj_sw2           : 10  ; // [11..2] 
        unsigned int    dci_adj_sw1           : 10  ; // [21..12] 
        unsigned int    dci_adj_sw0           : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIADJSW0;

// Define the union U_VP0_DCIADJSW1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_272          : 2   ; // [1..0] 
        unsigned int    dci_adj_sw5           : 10  ; // [11..2] 
        unsigned int    dci_adj_sw4           : 10  ; // [21..12] 
        unsigned int    dci_adj_sw3           : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIADJSW1;

// Define the union U_VP0_DCIADJSW2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_273          : 2   ; // [1..0] 
        unsigned int    dci_adj_sw8           : 10  ; // [11..2] 
        unsigned int    dci_adj_sw7           : 10  ; // [21..12] 
        unsigned int    dci_adj_sw6           : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIADJSW2;

// Define the union U_VP0_DCIADJSW3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_274          : 2   ; // [1..0] 
        unsigned int    dci_adj_sw11          : 10  ; // [11..2] 
        unsigned int    dci_adj_sw10          : 10  ; // [21..12] 
        unsigned int    dci_adj_sw9           : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIADJSW3;

// Define the union U_VP0_DCIADJSW4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_275          : 2   ; // [1..0] 
        unsigned int    dci_adj_sw14          : 10  ; // [11..2] 
        unsigned int    dci_adj_sw13          : 10  ; // [21..12] 
        unsigned int    dci_adj_sw12          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIADJSW4;

// Define the union U_VP0_DCIADJSW5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_276          : 2   ; // [1..0] 
        unsigned int    dci_adj_sw17          : 10  ; // [11..2] 
        unsigned int    dci_adj_sw16          : 10  ; // [21..12] 
        unsigned int    dci_adj_sw15          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIADJSW5;

// Define the union U_VP0_DCIADJSW6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_277          : 2   ; // [1..0] 
        unsigned int    dci_adj_sw20          : 10  ; // [11..2] 
        unsigned int    dci_adj_sw19          : 10  ; // [21..12] 
        unsigned int    dci_adj_sw18          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIADJSW6;

// Define the union U_VP0_DCIADJSW7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_278          : 2   ; // [1..0] 
        unsigned int    dci_adj_sw23          : 10  ; // [11..2] 
        unsigned int    dci_adj_sw22          : 10  ; // [21..12] 
        unsigned int    dci_adj_sw21          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIADJSW7;

// Define the union U_VP0_DCIADJSW8
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_279          : 2   ; // [1..0] 
        unsigned int    dci_adj_sw26          : 10  ; // [11..2] 
        unsigned int    dci_adj_sw25          : 10  ; // [21..12] 
        unsigned int    dci_adj_sw24          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIADJSW8;

// Define the union U_VP0_DCIADJSW9
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_280          : 2   ; // [1..0] 
        unsigned int    dci_adj_sw29          : 10  ; // [11..2] 
        unsigned int    dci_adj_sw28          : 10  ; // [21..12] 
        unsigned int    dci_adj_sw27          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIADJSW9;

// Define the union U_VP0_DCIADJSW10
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_281          : 2   ; // [1..0] 
        unsigned int    dci_adj_sw32          : 10  ; // [11..2] 
        unsigned int    dci_adj_sw31          : 10  ; // [21..12] 
        unsigned int    dci_adj_sw30          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIADJSW10;

// Define the union U_VP0_DCIWGTRD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_282          : 8   ; // [7..0] 
        unsigned int    dci_cal_adjwgt2       : 8   ; // [15..8] 
        unsigned int    dci_cal_adjwgt1       : 8   ; // [23..16] 
        unsigned int    dci_cal_adjwgt0       : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DCIWGTRD;

// Define the union U_VP0_ACM_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    acm_cbcrthr           : 8   ; // [7..0] 
        unsigned int    Reserved_283          : 19  ; // [26..8] 
        unsigned int    acm_cliporwrap        : 1   ; // [27] 
        unsigned int    acm_cliprange         : 1   ; // [28] 
        unsigned int    acm_stretch           : 1   ; // [29] 
        unsigned int    acm_dbg_en            : 1   ; // [30] 
        unsigned int    acm_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_ACM_CTRL;

// Define the union U_VP0_ACM_ADJ
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    acm_gain2             : 10  ; // [9..0] 
        unsigned int    acm_gain1             : 10  ; // [19..10] 
        unsigned int    acm_gain0             : 10  ; // [29..20] 
        unsigned int    Reserved_285          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_ACM_ADJ;

// Define the union U_VP0_DFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disp_xfpos            : 12  ; // [11..0] 
        unsigned int    disp_yfpos            : 12  ; // [23..12] 
        unsigned int    Reserved_286          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DFPOS;

// Define the union U_VP0_DLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disp_xlpos            : 12  ; // [11..0] 
        unsigned int    disp_ylpos            : 12  ; // [23..12] 
        unsigned int    Reserved_288          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_DLPOS;

// Define the union U_VP0_VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_289          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_VFPOS;

// Define the union U_VP0_VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_290          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_VLPOS;

// Define the union U_VP0_BK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbk_cr                : 10  ; // [9..0] 
        unsigned int    vbk_cb                : 10  ; // [19..10] 
        unsigned int    vbk_y                 : 10  ; // [29..20] 
        unsigned int    Reserved_291          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_BK;

// Define the union U_VP0_ALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbk_alpha             : 8   ; // [7..0] 
        unsigned int    Reserved_292          : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_ALPHA;

// Define the union U_VP0_CSC0_IDC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc0               : 11  ; // [10..0] 
        unsigned int    cscidc1               : 11  ; // [21..11] 
        unsigned int    csc_en                : 1   ; // [22] 
        unsigned int    Reserved_293          : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC0_IDC;

// Define the union U_VP0_CSC0_ODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscodc0               : 11  ; // [10..0] 
        unsigned int    cscodc1               : 11  ; // [21..11] 
        unsigned int    csc_sign_mode         : 1   ; // [22] 
        unsigned int    Reserved_295          : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC0_ODC;

// Define the union U_VP0_CSC0_IODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc2               : 11  ; // [10..0] 
        unsigned int    cscodc2               : 11  ; // [21..11] 
        unsigned int    Reserved_296          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC0_IODC;

// Define the union U_VP0_CSC0_P0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp00                : 15  ; // [14..0] 
        unsigned int    Reserved_298          : 1   ; // [15] 
        unsigned int    cscp01                : 15  ; // [30..16] 
        unsigned int    Reserved_297          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC0_P0;

// Define the union U_VP0_CSC0_P1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp02                : 15  ; // [14..0] 
        unsigned int    Reserved_300          : 1   ; // [15] 
        unsigned int    cscp10                : 15  ; // [30..16] 
        unsigned int    Reserved_299          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC0_P1;

// Define the union U_VP0_CSC0_P2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp11                : 15  ; // [14..0] 
        unsigned int    Reserved_302          : 1   ; // [15] 
        unsigned int    cscp12                : 15  ; // [30..16] 
        unsigned int    Reserved_301          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC0_P2;

// Define the union U_VP0_CSC0_P3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp20                : 15  ; // [14..0] 
        unsigned int    Reserved_304          : 1   ; // [15] 
        unsigned int    cscp21                : 15  ; // [30..16] 
        unsigned int    Reserved_303          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC0_P3;

// Define the union U_VP0_CSC0_P4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp22                : 15  ; // [14..0] 
        unsigned int    Reserved_305          : 17  ; // [31..15] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC0_P4;

// Define the union U_VP0_CSC1_IDC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc0               : 11  ; // [10..0] 
        unsigned int    cscidc1               : 11  ; // [21..11] 
        unsigned int    csc_en                : 1   ; // [22] 
        unsigned int    Reserved_306          : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC1_IDC;

// Define the union U_VP0_CSC1_ODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscodc0               : 11  ; // [10..0] 
        unsigned int    cscodc1               : 11  ; // [21..11] 
        unsigned int    csc_sign_mode         : 1   ; // [22] 
        unsigned int    Reserved_307          : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC1_ODC;

// Define the union U_VP0_CSC1_IODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc2               : 11  ; // [10..0] 
        unsigned int    cscodc2               : 11  ; // [21..11] 
        unsigned int    Reserved_308          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC1_IODC;

// Define the union U_VP0_CSC1_P0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp00                : 15  ; // [14..0] 
        unsigned int    Reserved_310          : 1   ; // [15] 
        unsigned int    cscp01                : 15  ; // [30..16] 
        unsigned int    Reserved_309          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC1_P0;

// Define the union U_VP0_CSC1_P1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp02                : 15  ; // [14..0] 
        unsigned int    Reserved_312          : 1   ; // [15] 
        unsigned int    cscp10                : 15  ; // [30..16] 
        unsigned int    Reserved_311          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC1_P1;

// Define the union U_VP0_CSC1_P2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp11                : 15  ; // [14..0] 
        unsigned int    Reserved_314          : 1   ; // [15] 
        unsigned int    cscp12                : 15  ; // [30..16] 
        unsigned int    Reserved_313          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC1_P2;

// Define the union U_VP0_CSC1_P3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp20                : 15  ; // [14..0] 
        unsigned int    Reserved_316          : 1   ; // [15] 
        unsigned int    cscp21                : 15  ; // [30..16] 
        unsigned int    Reserved_315          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC1_P3;

// Define the union U_VP0_CSC1_P4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp22                : 15  ; // [14..0] 
        unsigned int    Reserved_317          : 17  ; // [31..15] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VP0_CSC1_P4;

// Define the union U_G0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ifmt                  : 8   ; // [7..0] 
        unsigned int    bitext                : 2   ; // [9..8] 
        unsigned int    Reserved_319          : 14  ; // [23..10] 
        unsigned int    flip_en               : 1   ; // [24] 
        unsigned int    dcmp_en               : 1   ; // [25] 
        unsigned int    read_mode             : 1   ; // [26] 
        unsigned int    upd_mode              : 1   ; // [27] 
        unsigned int    mute_en               : 1   ; // [28] 
        unsigned int    Reserved_318          : 1   ; // [29] 
        unsigned int    nosec_flag            : 1   ; // [30] 
        unsigned int    surface_en            : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_CTRL;

// Define the union U_G0_UPD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0] 
        unsigned int    Reserved_321          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_UPD;

// Define the union U_G0_ADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_G0_ADDR;
// Define the union U_G0_NADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_G0_NADDR;
// Define the union U_G0_STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0] 
        unsigned int    Reserved_324          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_STRIDE;

// Define the union U_G0_IRESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iw                    : 12  ; // [11..0] 
        unsigned int    ih                    : 12  ; // [23..12] 
        unsigned int    Reserved_325          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_IRESO;

// Define the union U_G0_SFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    src_xfpos             : 7   ; // [6..0] 
        unsigned int    Reserved_326          : 25  ; // [31..7] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_SFPOS;

// Define the union U_G0_CBMPARA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    galpha                : 8   ; // [7..0] 
        unsigned int    palpha_range          : 1   ; // [8] 
        unsigned int    Reserved_328          : 1   ; // [9] 
        unsigned int    vedge_p               : 1   ; // [10] 
        unsigned int    hedge_p               : 1   ; // [11] 
        unsigned int    palpha_en             : 1   ; // [12] 
        unsigned int    premult_en            : 1   ; // [13] 
        unsigned int    key_en                : 1   ; // [14] 
        unsigned int    key_mode              : 1   ; // [15] 
        unsigned int    Reserved_327          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_CBMPARA;

// Define the union U_G0_CKEYMAX
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    keyb_max              : 8   ; // [7..0] 
        unsigned int    keyg_max              : 8   ; // [15..8] 
        unsigned int    keyr_max              : 8   ; // [23..16] 
        unsigned int    va0                   : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_CKEYMAX;

// Define the union U_G0_CKEYMIN
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    keyb_min              : 8   ; // [7..0] 
        unsigned int    keyg_min              : 8   ; // [15..8] 
        unsigned int    keyr_min              : 8   ; // [23..16] 
        unsigned int    va1                   : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_CKEYMIN;

// Define the union U_G0_CMASK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    kmsk_b                : 8   ; // [7..0] 
        unsigned int    kmsk_g                : 8   ; // [15..8] 
        unsigned int    kmsk_r                : 8   ; // [23..16] 
        unsigned int    Reserved_330          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_CMASK;

// Define the union U_G0_PARAADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_G0_PARAADDR;
// Define the union U_G0_PARAUP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    gdc_paraup            : 1   ; // [0] 
        unsigned int    Reserved_331          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_PARAUP;

// Define the union U_G0_DCMP_ADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_G0_DCMP_ADDR;
// Define the union U_G0_DCMP_NADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_G0_DCMP_NADDR;
// Define the union U_G0_DCMP_OFFSET
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int offset                  : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_G0_DCMP_OFFSET;
// Define the union U_G0_DFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disp_xfpos            : 12  ; // [11..0] 
        unsigned int    disp_yfpos            : 12  ; // [23..12] 
        unsigned int    Reserved_333          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_DFPOS;

// Define the union U_G0_DLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disp_xlpos            : 12  ; // [11..0] 
        unsigned int    disp_ylpos            : 12  ; // [23..12] 
        unsigned int    Reserved_335          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_DLPOS;

// Define the union U_G0_VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_336          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_VFPOS;

// Define the union U_G0_VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_337          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_VLPOS;

// Define the union U_G0_BK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbk_cr                : 10  ; // [9..0] 
        unsigned int    vbk_cb                : 10  ; // [19..10] 
        unsigned int    vbk_y                 : 10  ; // [29..20] 
        unsigned int    Reserved_338          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_BK;

// Define the union U_G0_ALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbk_alpha             : 8   ; // [7..0] 
        unsigned int    Reserved_339          : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_ALPHA;

// Define the union U_G0_DOF_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_340          : 31  ; // [30..0] 
        unsigned int    dof_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_DOF_CTRL;

// Define the union U_G0_DOF_STEP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    left_step             : 8   ; // [7..0] 
        unsigned int    right_step            : 8   ; // [15..8] 
        unsigned int    Reserved_342          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_DOF_STEP;

// Define the union U_GP0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mux1_sel              : 2   ; // [1..0] 
        unsigned int    Reserved_347          : 2   ; // [3..2] 
        unsigned int    mux2_sel              : 2   ; // [5..4] 
        unsigned int    Reserved_346          : 2   ; // [7..6] 
        unsigned int    aout_sel              : 2   ; // [9..8] 
        unsigned int    Reserved_345          : 2   ; // [11..10] 
        unsigned int    bout_sel              : 2   ; // [13..12] 
        unsigned int    Reserved_344          : 12  ; // [25..14] 
        unsigned int    ffc_sel               : 1   ; // [26] 
        unsigned int    Reserved_343          : 3   ; // [29..27] 
        unsigned int    mute_en               : 1   ; // [30] 
        unsigned int    read_mode             : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CTRL;

// Define the union U_GP0_UPD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0] 
        unsigned int    Reserved_349          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_UPD;

// Define the union U_GP0_ORESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ow                    : 12  ; // [11..0] 
        unsigned int    oh                    : 12  ; // [23..12] 
        unsigned int    Reserved_350          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ORESO;

// Define the union U_GP0_IRESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iw                    : 12  ; // [11..0] 
        unsigned int    ih                    : 12  ; // [23..12] 
        unsigned int    Reserved_351          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_IRESO;

// Define the union U_GP0_HCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_GP0_HCOEFAD;
// Define the union U_GP0_VCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_GP0_VCOEFAD;
// Define the union U_GP0_PARAUP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    gp0_hcoef_upd         : 1   ; // [0] 
        unsigned int    gp0_vcoef_upd         : 1   ; // [1] 
        unsigned int    Reserved_352          : 30  ; // [31..2] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_PARAUP;

// Define the union U_GP0_GALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    galpha                : 8   ; // [7..0] 
        unsigned int    Reserved_353          : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_GALPHA;

// Define the union U_GP0_DFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disp_xfpos            : 12  ; // [11..0] 
        unsigned int    disp_yfpos            : 12  ; // [23..12] 
        unsigned int    Reserved_355          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_DFPOS;

// Define the union U_GP0_DLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disp_xlpos            : 12  ; // [11..0] 
        unsigned int    disp_ylpos            : 12  ; // [23..12] 
        unsigned int    Reserved_357          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_DLPOS;

// Define the union U_GP0_VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0] 
        unsigned int    video_yfpos           : 12  ; // [23..12] 
        unsigned int    Reserved_358          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_VFPOS;

// Define the union U_GP0_VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0] 
        unsigned int    video_ylpos           : 12  ; // [23..12] 
        unsigned int    Reserved_359          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_VLPOS;

// Define the union U_GP0_BK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbk_cr                : 10  ; // [9..0] 
        unsigned int    vbk_cb                : 10  ; // [19..10] 
        unsigned int    vbk_y                 : 10  ; // [29..20] 
        unsigned int    Reserved_360          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_BK;

// Define the union U_GP0_ALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbk_alpha             : 8   ; // [7..0] 
        unsigned int    Reserved_361          : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ALPHA;

// Define the union U_GP0_CSC_IDC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc0               : 11  ; // [10..0] 
        unsigned int    cscidc1               : 11  ; // [21..11] 
        unsigned int    csc_en                : 1   ; // [22] 
        unsigned int    Reserved_362          : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_IDC;

// Define the union U_GP0_CSC_ODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscodc0               : 11  ; // [10..0] 
        unsigned int    cscodc1               : 11  ; // [21..11] 
        unsigned int    csc_sign_mode         : 1   ; // [22] 
        unsigned int    Reserved_364          : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_ODC;

// Define the union U_GP0_CSC_IODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc2               : 11  ; // [10..0] 
        unsigned int    cscodc2               : 11  ; // [21..11] 
        unsigned int    Reserved_365          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_IODC;

// Define the union U_GP0_CSC_P0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp00                : 15  ; // [14..0] 
        unsigned int    Reserved_367          : 1   ; // [15] 
        unsigned int    cscp01                : 15  ; // [30..16] 
        unsigned int    Reserved_366          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_P0;

// Define the union U_GP0_CSC_P1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp02                : 15  ; // [14..0] 
        unsigned int    Reserved_369          : 1   ; // [15] 
        unsigned int    cscp10                : 15  ; // [30..16] 
        unsigned int    Reserved_368          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_P1;

// Define the union U_GP0_CSC_P2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp11                : 15  ; // [14..0] 
        unsigned int    Reserved_371          : 1   ; // [15] 
        unsigned int    cscp12                : 15  ; // [30..16] 
        unsigned int    Reserved_370          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_P2;

// Define the union U_GP0_CSC_P3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp20                : 15  ; // [14..0] 
        unsigned int    Reserved_373          : 1   ; // [15] 
        unsigned int    cscp21                : 15  ; // [30..16] 
        unsigned int    Reserved_372          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_P3;

// Define the union U_GP0_CSC_P4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp22                : 15  ; // [14..0] 
        unsigned int    Reserved_374          : 17  ; // [31..15] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_P4;

// Define the union U_GP0_ZME_HSP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hratio                : 24  ; // [23..0] 
        unsigned int    hfir_order            : 1   ; // [24] 
        unsigned int    hafir_en              : 1   ; // [25] 
        unsigned int    hfir_en               : 1   ; // [26] 
        unsigned int    Reserved_375          : 1   ; // [27] 
        unsigned int    hchmid_en             : 1   ; // [28] 
        unsigned int    hlmid_en              : 1   ; // [29] 
        unsigned int    hamid_en              : 1   ; // [30] 
        unsigned int    hsc_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_HSP;

// Define the union U_GP0_ZME_HLOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hor_loffset           : 28  ; // [27..0] 
        unsigned int    Reserved_376          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_HLOFFSET;

// Define the union U_GP0_ZME_HCOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hor_coffset           : 28  ; // [27..0] 
        unsigned int    Reserved_377          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_HCOFFSET;

// Define the union U_GP0_ZME_VSP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_379          : 23  ; // [22..0] 
        unsigned int    vafir_en              : 1   ; // [23] 
        unsigned int    vfir_en               : 1   ; // [24] 
        unsigned int    Reserved_378          : 2   ; // [26..25] 
        unsigned int    vsc_luma_tap          : 1   ; // [27] 
        unsigned int    vchmid_en             : 1   ; // [28] 
        unsigned int    vlmid_en              : 1   ; // [29] 
        unsigned int    vamid_en              : 1   ; // [30] 
        unsigned int    vsc_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_VSP;

// Define the union U_GP0_ZME_VSR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vratio                : 16  ; // [15..0] 
        unsigned int    Reserved_380          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_VSR;

// Define the union U_GP0_ZME_VOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbtm_offset           : 16  ; // [15..0] 
        unsigned int    vtp_offset            : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_VOFFSET;

// Define the union U_GP0_ZME_LTICTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    lhpass_coef4          : 8   ; // [7..0] 
        unsigned int    lmixing_ratio         : 8   ; // [15..8] 
        unsigned int    lgain_ratio           : 12  ; // [27..16] 
        unsigned int    Reserved_381          : 3   ; // [30..28] 
        unsigned int    lti_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_LTICTRL;

// Define the union U_GP0_ZME_LHPASSCOEF
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    lhpass_coef0          : 8   ; // [7..0] 
        unsigned int    lhpass_coef1          : 8   ; // [15..8] 
        unsigned int    lhpass_coef2          : 8   ; // [23..16] 
        unsigned int    lhpass_coef3          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_LHPASSCOEF;

// Define the union U_GP0_ZME_LTITHD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    lover_swing           : 10  ; // [9..0] 
        unsigned int    lunder_swing          : 10  ; // [19..10] 
        unsigned int    lcoring_thd           : 12  ; // [31..20] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_LTITHD;

// Define the union U_GP0_ZME_LHFREQTHD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    lhfreq_thd0           : 16  ; // [15..0] 
        unsigned int    lhfreq_thd1           : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_LHFREQTHD;

// Define the union U_GP0_ZME_LGAINCOEF
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    lgain_coef0           : 8   ; // [7..0] 
        unsigned int    lgain_coef1           : 8   ; // [15..8] 
        unsigned int    lgain_coef2           : 8   ; // [23..16] 
        unsigned int    Reserved_384          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_LGAINCOEF;

// Define the union U_GP0_ZME_CTICTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_386          : 8   ; // [7..0] 
        unsigned int    cmixing_ratio         : 8   ; // [15..8] 
        unsigned int    cgain_ratio           : 12  ; // [27..16] 
        unsigned int    Reserved_385          : 3   ; // [30..28] 
        unsigned int    cti_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_CTICTRL;

// Define the union U_GP0_ZME_CHPASSCOEF
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    chpass_coef0          : 8   ; // [7..0] 
        unsigned int    chpass_coef1          : 8   ; // [15..8] 
        unsigned int    chpass_coef2          : 8   ; // [23..16] 
        unsigned int    Reserved_388          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_CHPASSCOEF;

// Define the union U_GP0_ZME_CTITHD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cover_swing           : 10  ; // [9..0] 
        unsigned int    cunder_swing          : 10  ; // [19..10] 
        unsigned int    ccoring_thd           : 12  ; // [31..20] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_CTITHD;

// Define the union U_WBC_G0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    req_interval          : 10  ; // [9..0] 
        unsigned int    auto_stop_en          : 1   ; // [10] 
        unsigned int    Reserved_390          : 15  ; // [25..11] 
        unsigned int    format_out            : 2   ; // [27..26] 
        unsigned int    Reserved_389          : 3   ; // [30..28] 
        unsigned int    wbc_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_CTRL;

// Define the union U_WBC_G0_UPD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0] 
        unsigned int    Reserved_392          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_UPD;

// Define the union U_WBC_G0_CMP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cmp_lossy_en          : 1   ; // [0] 
        unsigned int    Reserved_394          : 3   ; // [3..1] 
        unsigned int    cmp_drr               : 4   ; // [7..4] 
        unsigned int    Reserved_393          : 23  ; // [30..8] 
        unsigned int    cmp_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_CMP;

// Define the union U_WBC_G0_AR_ADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int wbcaddr                 : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_G0_AR_ADDR;
// Define the union U_WBC_G0_GB_ADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int wbccaddr                : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_G0_GB_ADDR;
// Define the union U_WBC_G0_STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wbcstride             : 16  ; // [15..0] 
        unsigned int    Reserved_396          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_STRIDE;

// Define the union U_WBC_G0_OFFSET
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int wbcoffset               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_G0_OFFSET;
// Define the union U_WBC_G0_ORESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ow                    : 12  ; // [11..0] 
        unsigned int    oh                    : 12  ; // [23..12] 
        unsigned int    Reserved_397          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_ORESO;

// Define the union U_WBC_G0_FCROP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wfcrop                : 12  ; // [11..0] 
        unsigned int    hfcrop                : 12  ; // [23..12] 
        unsigned int    Reserved_398          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_FCROP;

// Define the union U_WBC_G0_LCROP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wlcrop                : 12  ; // [11..0] 
        unsigned int    hlcrop                : 12  ; // [23..12] 
        unsigned int    Reserved_399          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_LCROP;

// Define the union U_WBC_GP0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    req_interval          : 10  ; // [9..0] 
        unsigned int    auto_stop_en          : 1   ; // [10] 
        unsigned int    Reserved_402          : 1   ; // [11] 
        unsigned int    wbc_vtthd_mode        : 1   ; // [12] 
        unsigned int    Reserved_401          : 5   ; // [17..13] 
        unsigned int    three_d_mode          : 2   ; // [19..18] 
        unsigned int    Reserved_400          : 4   ; // [23..20] 
        unsigned int    format_out            : 4   ; // [27..24] 
        unsigned int    mode_out              : 2   ; // [29..28] 
        unsigned int    uv_order              : 1   ; // [30] 
        unsigned int    wbc_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_CTRL;

// Define the union U_WBC_GP0_UPD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0] 
        unsigned int    Reserved_404          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_UPD;

// Define the union U_WBC_GP0_YADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int wbcaddr                 : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_GP0_YADDR;
// Define the union U_WBC_GP0_CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int wbccaddr                : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_GP0_CADDR;
// Define the union U_WBC_GP0_STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wbclstride            : 16  ; // [15..0] 
        unsigned int    wbccstride            : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_STRIDE;

// Define the union U_WBC_GP0_ORESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ow                    : 12  ; // [11..0] 
        unsigned int    oh                    : 12  ; // [23..12] 
        unsigned int    Reserved_406          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_ORESO;

// Define the union U_WBC_GP0_FCROP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wfcrop                : 12  ; // [11..0] 
        unsigned int    hfcrop                : 12  ; // [23..12] 
        unsigned int    Reserved_408          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_FCROP;

// Define the union U_WBC_GP0_LCROP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wlcrop                : 12  ; // [11..0] 
        unsigned int    hlcrop                : 12  ; // [23..12] 
        unsigned int    Reserved_409          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_LCROP;

// Define the union U_WBC_GP0_DITHER_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_410          : 29  ; // [28..0] 
        unsigned int    dither_md             : 3   ; // [31..29] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_DITHER_CTRL;

// Define the union U_WBC_GP0_DITHER_COEF0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dither_coef0          : 8   ; // [7..0] 
        unsigned int    dither_coef1          : 8   ; // [15..8] 
        unsigned int    dither_coef2          : 8   ; // [23..16] 
        unsigned int    dither_coef3          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_DITHER_COEF0;

// Define the union U_WBC_GP0_DITHER_COEF1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dither_coef4          : 8   ; // [7..0] 
        unsigned int    dither_coef5          : 8   ; // [15..8] 
        unsigned int    dither_coef6          : 8   ; // [23..16] 
        unsigned int    dither_coef7          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_DITHER_COEF1;

// Define the union U_WBC_DHD0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    req_interval          : 10  ; // [9..0] 
        unsigned int    auto_stop_en          : 1   ; // [10] 
        unsigned int    histogam_en           : 1   ; // [11] 
        unsigned int    wbc_vtthd_mode        : 1   ; // [12] 
        unsigned int    Reserved_413          : 5   ; // [17..13] 
        unsigned int    three_d_mode          : 2   ; // [19..18] 
        unsigned int    Reserved_412          : 4   ; // [23..20] 
        unsigned int    format_out            : 4   ; // [27..24] 
        unsigned int    mode_out              : 2   ; // [29..28] 
        unsigned int    uv_order              : 1   ; // [30] 
        unsigned int    wbc_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_CTRL;

// Define the union U_WBC_DHD0_UPD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0] 
        unsigned int    Reserved_415          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_UPD;

// Define the union U_WBC_DHD0_YADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int wbcaddr                 : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_DHD0_YADDR;
// Define the union U_WBC_DHD0_CADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int wbccaddr                : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_DHD0_CADDR;
// Define the union U_WBC_DHD0_STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wbclstride            : 16  ; // [15..0] 
        unsigned int    wbccstride            : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_STRIDE;

// Define the union U_WBC_DHD0_ORESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ow                    : 12  ; // [11..0] 
        unsigned int    oh                    : 12  ; // [23..12] 
        unsigned int    Reserved_417          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_ORESO;

// Define the union U_WBC_DHD0_FCROP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wfcrop                : 12  ; // [11..0] 
        unsigned int    hfcrop                : 12  ; // [23..12] 
        unsigned int    Reserved_419          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_FCROP;

// Define the union U_WBC_DHD0_LCROP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wlcrop                : 12  ; // [11..0] 
        unsigned int    hlcrop                : 12  ; // [23..12] 
        unsigned int    Reserved_420          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_LCROP;

// Define the union U_WBC_DHD0_LOWDLYCTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wb_per_line_num       : 12  ; // [11..0] 
        unsigned int    partfns_line_num      : 12  ; // [23..12] 
        unsigned int    Reserved_421          : 6   ; // [29..24] 
        unsigned int    lowdly_test           : 1   ; // [30] 
        unsigned int    lowdly_en             : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_LOWDLYCTRL;

// Define the union U_WBC_DHD0_TUNLADDR
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int tunl_cell_addr          : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_DHD0_TUNLADDR;
// Define the union U_WBC_DHD0_LOWDLYSTA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_423          : 31  ; // [30..0] 
        unsigned int    part_finish           : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_LOWDLYSTA;

// Define the union U_WBC_DHD0_PARAUP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wbc_hlcoef_upd        : 1   ; // [0] 
        unsigned int    wbc_hccoef_upd        : 1   ; // [1] 
        unsigned int    wbc_vlcoef_upd        : 1   ; // [2] 
        unsigned int    wbc_vccoef_upd        : 1   ; // [3] 
        unsigned int    Reserved_424          : 28  ; // [31..4] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_PARAUP;

// Define the union U_WBC_DHD0_HLCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_DHD0_HLCOEFAD;
// Define the union U_WBC_DHD0_HCCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_DHD0_HCCOEFAD;
// Define the union U_WBC_DHD0_VLCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_DHD0_VLCOEFAD;
// Define the union U_WBC_DHD0_VCCOEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_DHD0_VCCOEFAD;
// Define the union U_WBC_DHD0_HISTOGRAM0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt0            : 24  ; // [23..0] 
        unsigned int    Reserved_427          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM0;

// Define the union U_WBC_DHD0_HISTOGRAM1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt1            : 24  ; // [23..0] 
        unsigned int    Reserved_429          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM1;

// Define the union U_WBC_DHD0_HISTOGRAM2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt2            : 24  ; // [23..0] 
        unsigned int    Reserved_430          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM2;

// Define the union U_WBC_DHD0_HISTOGRAM3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt3            : 24  ; // [23..0] 
        unsigned int    Reserved_431          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM3;

// Define the union U_WBC_DHD0_HISTOGRAM4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt4            : 24  ; // [23..0] 
        unsigned int    Reserved_432          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM4;

// Define the union U_WBC_DHD0_HISTOGRAM5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt5            : 24  ; // [23..0] 
        unsigned int    Reserved_433          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM5;

// Define the union U_WBC_DHD0_HISTOGRAM6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt6            : 24  ; // [23..0] 
        unsigned int    Reserved_434          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM6;

// Define the union U_WBC_DHD0_HISTOGRAM7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt7            : 24  ; // [23..0] 
        unsigned int    Reserved_435          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM7;

// Define the union U_WBC_DHD0_HISTOGRAM8
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt8            : 24  ; // [23..0] 
        unsigned int    Reserved_436          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM8;

// Define the union U_WBC_DHD0_HISTOGRAM9
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt9            : 24  ; // [23..0] 
        unsigned int    Reserved_437          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM9;

// Define the union U_WBC_DHD0_HISTOGRAM10
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt10           : 24  ; // [23..0] 
        unsigned int    Reserved_438          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM10;

// Define the union U_WBC_DHD0_HISTOGRAM11
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt11           : 24  ; // [23..0] 
        unsigned int    Reserved_439          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM11;

// Define the union U_WBC_DHD0_HISTOGRAM12
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt12           : 24  ; // [23..0] 
        unsigned int    Reserved_440          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM12;

// Define the union U_WBC_DHD0_HISTOGRAM13
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt13           : 24  ; // [23..0] 
        unsigned int    Reserved_441          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM13;

// Define the union U_WBC_DHD0_HISTOGRAM14
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt14           : 24  ; // [23..0] 
        unsigned int    Reserved_442          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM14;

// Define the union U_WBC_DHD0_HISTOGRAM15
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hgram_cnt15           : 24  ; // [23..0] 
        unsigned int    Reserved_443          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_HISTOGRAM15;

// Define the union U_WBC_DHD0_CHECKSUM_Y
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int Reserved_444            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_DHD0_CHECKSUM_Y;
// Define the union U_WBC_DHD0_CHECKSUM_C
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int Reserved_446            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_WBC_DHD0_CHECKSUM_C;
// Define the union U_WBC_DHD0_DITHER_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_447          : 29  ; // [28..0] 
        unsigned int    dither_md             : 3   ; // [31..29] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_DITHER_CTRL;

// Define the union U_WBC_DHD0_DITHER_COEF0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dither_coef0          : 8   ; // [7..0] 
        unsigned int    dither_coef1          : 8   ; // [15..8] 
        unsigned int    dither_coef2          : 8   ; // [23..16] 
        unsigned int    dither_coef3          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_DITHER_COEF0;

// Define the union U_WBC_DHD0_DITHER_COEF1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dither_coef4          : 8   ; // [7..0] 
        unsigned int    dither_coef5          : 8   ; // [15..8] 
        unsigned int    dither_coef6          : 8   ; // [23..16] 
        unsigned int    dither_coef7          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_DITHER_COEF1;

// Define the union U_WBC_DHD0_ZME_HSP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hratio                : 24  ; // [23..0] 
        unsigned int    hfir_order            : 1   ; // [24] 
        unsigned int    hchfir_en             : 1   ; // [25] 
        unsigned int    hlfir_en              : 1   ; // [26] 
        unsigned int    Reserved_449          : 1   ; // [27] 
        unsigned int    hchmid_en             : 1   ; // [28] 
        unsigned int    hlmid_en              : 1   ; // [29] 
        unsigned int    hchmsc_en             : 1   ; // [30] 
        unsigned int    hlmsc_en              : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_ZME_HSP;

// Define the union U_WBC_DHD0_ZME_HLOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hor_loffset           : 28  ; // [27..0] 
        unsigned int    Reserved_451          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_ZME_HLOFFSET;

// Define the union U_WBC_DHD0_ZME_HCOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hor_coffset           : 28  ; // [27..0] 
        unsigned int    Reserved_452          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_ZME_HCOFFSET;

// Define the union U_WBC_DHD0_ZME_VSP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_454          : 19  ; // [18..0] 
        unsigned int    zme_in_fmt            : 2   ; // [20..19] 
        unsigned int    zme_out_fmt           : 2   ; // [22..21] 
        unsigned int    vchfir_en             : 1   ; // [23] 
        unsigned int    vlfir_en              : 1   ; // [24] 
        unsigned int    Reserved_453          : 3   ; // [27..25] 
        unsigned int    vchmid_en             : 1   ; // [28] 
        unsigned int    vlmid_en              : 1   ; // [29] 
        unsigned int    vchmsc_en             : 1   ; // [30] 
        unsigned int    vlmsc_en              : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_ZME_VSP;

// Define the union U_WBC_DHD0_ZME_VSR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vratio                : 16  ; // [15..0] 
        unsigned int    Reserved_456          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_ZME_VSR;

// Define the union U_WBC_DHD0_ZME_VOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vchroma_offset        : 16  ; // [15..0] 
        unsigned int    vluma_offset          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_ZME_VOFFSET;

// Define the union U_WBC_DHD0_ZME_VBOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbchroma_offset       : 16  ; // [15..0] 
        unsigned int    vbluma_offset         : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_ZME_VBOFFSET;

// Define the union U_WBC_DHD0_CSCIDC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc0               : 9   ; // [8..0] 
        unsigned int    cscidc1               : 9   ; // [17..9] 
        unsigned int    cscidc2               : 9   ; // [26..18] 
        unsigned int    csc_en                : 1   ; // [27] 
        unsigned int    csc_mode              : 3   ; // [30..28] 
        unsigned int    Reserved_457          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_CSCIDC;

// Define the union U_WBC_DHD0_CSCODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscodc0               : 9   ; // [8..0] 
        unsigned int    cscodc1               : 9   ; // [17..9] 
        unsigned int    cscodc2               : 9   ; // [26..18] 
        unsigned int    Reserved_459          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_CSCODC;

// Define the union U_WBC_DHD0_CSCP0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp00                : 15  ; // [14..0] 
        unsigned int    Reserved_461          : 1   ; // [15] 
        unsigned int    cscp01                : 15  ; // [30..16] 
        unsigned int    Reserved_460          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_CSCP0;

// Define the union U_WBC_DHD0_CSCP1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp02                : 15  ; // [14..0] 
        unsigned int    Reserved_463          : 1   ; // [15] 
        unsigned int    cscp10                : 15  ; // [30..16] 
        unsigned int    Reserved_462          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_CSCP1;

// Define the union U_WBC_DHD0_CSCP2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp11                : 15  ; // [14..0] 
        unsigned int    Reserved_465          : 1   ; // [15] 
        unsigned int    cscp12                : 15  ; // [30..16] 
        unsigned int    Reserved_464          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_CSCP2;

// Define the union U_WBC_DHD0_CSCP3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp20                : 15  ; // [14..0] 
        unsigned int    Reserved_467          : 1   ; // [15] 
        unsigned int    cscp21                : 15  ; // [30..16] 
        unsigned int    Reserved_466          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_CSCP3;

// Define the union U_WBC_DHD0_CSCP4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp22                : 15  ; // [14..0] 
        unsigned int    Reserved_468          : 17  ; // [31..15] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_DHD0_CSCP4;

// Define the union U_MIXV0_BKG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mixer_bkgcr           : 10  ; // [9..0] 
        unsigned int    mixer_bkgcb           : 10  ; // [19..10] 
        unsigned int    mixer_bkgy            : 10  ; // [29..20] 
        unsigned int    Reserved_469          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_MIXV0_BKG;

// Define the union U_MIXV0_MIX
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mixer_prio0           : 4   ; // [3..0] 
        unsigned int    mixer_prio1           : 4   ; // [7..4] 
        unsigned int    Reserved_471          : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_MIXV0_MIX;

// Define the union U_MIXG0_BKG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mixer_bkgcr           : 10  ; // [9..0] 
        unsigned int    mixer_bkgcb           : 10  ; // [19..10] 
        unsigned int    mixer_bkgy            : 10  ; // [29..20] 
        unsigned int    Reserved_473          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_MIXG0_BKG;

// Define the union U_MIXG0_BKALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mixer_alpha           : 8   ; // [7..0] 
        unsigned int    Reserved_475          : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_MIXG0_BKALPHA;

// Define the union U_MIXG0_MIX
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mixer_prio0           : 4   ; // [3..0] 
        unsigned int    mixer_prio1           : 4   ; // [7..4] 
        unsigned int    mixer_prio2           : 4   ; // [11..8] 
        unsigned int    mixer_prio3           : 4   ; // [15..12] 
        unsigned int    Reserved_476          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_MIXG0_MIX;

// Define the union U_CBM_BKG1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cbm_bkgcr1            : 10  ; // [9..0] 
        unsigned int    cbm_bkgcb1            : 10  ; // [19..10] 
        unsigned int    cbm_bkgy1             : 10  ; // [29..20] 
        unsigned int    Reserved_477          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CBM_BKG1;

// Define the union U_CBM_MIX1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mixer_prio0           : 4   ; // [3..0] 
        unsigned int    mixer_prio1           : 4   ; // [7..4] 
        unsigned int    Reserved_479          : 24  ; // [31..8] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CBM_MIX1;

// Define the union U_CBM_BKG2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cbm_bkgcr2            : 10  ; // [9..0] 
        unsigned int    cbm_bkgcb2            : 10  ; // [19..10] 
        unsigned int    cbm_bkgy2             : 10  ; // [29..20] 
        unsigned int    Reserved_481          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CBM_BKG2;

// Define the union U_CBM_MIX2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mixer_prio0           : 4   ; // [3..0] 
        unsigned int    mixer_prio1           : 4   ; // [7..4] 
        unsigned int    mixer_prio2           : 4   ; // [11..8] 
        unsigned int    mixer_prio3           : 4   ; // [15..12] 
        unsigned int    Reserved_483          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CBM_MIX2;

// Define the union U_CBM_ATTR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sur_attr0             : 1   ; // [0] 
        unsigned int    sur_attr1             : 1   ; // [1] 
        unsigned int    sur_attr2             : 1   ; // [2] 
        unsigned int    sur_attr3             : 1   ; // [3] 
        unsigned int    sur_attr4             : 1   ; // [4] 
        unsigned int    sur_attr5             : 1   ; // [5] 
        unsigned int    Reserved_485          : 26  ; // [31..6] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CBM_ATTR;

// Define the union U_DHD0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0] 
        unsigned int    disp_mode             : 3   ; // [3..1] 
        unsigned int    iop                   : 1   ; // [4] 
        unsigned int    Reserved_489          : 7   ; // [11..5] 
        unsigned int    gmm_mode              : 1   ; // [12] 
        unsigned int    gmm_en                : 1   ; // [13] 
        unsigned int    hdmi_mode             : 1   ; // [14] 
        unsigned int    Reserved_488          : 3   ; // [17..15] 
        unsigned int    sin_en                : 1   ; // [18] 
        unsigned int    Reserved_487          : 1   ; // [19] 
        unsigned int    fpga_lmt_width        : 7   ; // [26..20] 
        unsigned int    fpga_lmt_en           : 1   ; // [27] 
        unsigned int    p2i_en                : 1   ; // [28] 
        unsigned int    cbar_sel              : 1   ; // [29] 
        unsigned int    cbar_en               : 1   ; // [30] 
        unsigned int    intf_en               : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CTRL;

// Define the union U_DHD0_VSYNC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vact                  : 12  ; // [11..0] 
        unsigned int    vbb                   : 8   ; // [19..12] 
        unsigned int    vfb                   : 8   ; // [27..20] 
        unsigned int    Reserved_491          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_VSYNC;

// Define the union U_DHD0_HSYNC1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hact                  : 16  ; // [15..0] 
        unsigned int    hbb                   : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_HSYNC1;

// Define the union U_DHD0_HSYNC2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hfb                   : 16  ; // [15..0] 
        unsigned int    hmid                  : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_HSYNC2;

// Define the union U_DHD0_VPLUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    bvact                 : 12  ; // [11..0] 
        unsigned int    bvbb                  : 8   ; // [19..12] 
        unsigned int    bvfb                  : 8   ; // [27..20] 
        unsigned int    Reserved_492          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_VPLUS;

// Define the union U_DHD0_PWR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hpw                   : 16  ; // [15..0] 
        unsigned int    vpw                   : 8   ; // [23..16] 
        unsigned int    Reserved_493          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_PWR;

// Define the union U_DHD0_VTTHD3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vtmgthd3              : 13  ; // [12..0] 
        unsigned int    Reserved_495          : 2   ; // [14..13] 
        unsigned int    thd3_mode             : 1   ; // [15] 
        unsigned int    vtmgthd4              : 13  ; // [28..16] 
        unsigned int    Reserved_494          : 2   ; // [30..29] 
        unsigned int    thd4_mode             : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_VTTHD3;

// Define the union U_DHD0_VTTHD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vtmgthd1              : 13  ; // [12..0] 
        unsigned int    Reserved_497          : 2   ; // [14..13] 
        unsigned int    thd1_mode             : 1   ; // [15] 
        unsigned int    vtmgthd2              : 13  ; // [28..16] 
        unsigned int    Reserved_496          : 2   ; // [30..29] 
        unsigned int    thd2_mode             : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_VTTHD;

// Define the union U_DHD0_SYNC_INV
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    lcd_dv_inv            : 1   ; // [0] 
        unsigned int    lcd_hs_inv            : 1   ; // [1] 
        unsigned int    lcd_vs_inv            : 1   ; // [2] 
        unsigned int    Reserved_500          : 1   ; // [3] 
        unsigned int    vga_dv_inv            : 1   ; // [4] 
        unsigned int    vga_hs_inv            : 1   ; // [5] 
        unsigned int    vga_vs_inv            : 1   ; // [6] 
        unsigned int    Reserved_499          : 1   ; // [7] 
        unsigned int    hdmi_dv_inv           : 1   ; // [8] 
        unsigned int    hdmi_hs_inv           : 1   ; // [9] 
        unsigned int    hdmi_vs_inv           : 1   ; // [10] 
        unsigned int    hdmi_f_inv            : 1   ; // [11] 
        unsigned int    date_dv_inv           : 1   ; // [12] 
        unsigned int    date_hs_inv           : 1   ; // [13] 
        unsigned int    date_vs_inv           : 1   ; // [14] 
        unsigned int    date_f_inv            : 1   ; // [15] 
        unsigned int    Reserved_498          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_SYNC_INV;

// Define the union U_DHD0_DATA_SEL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    date_data_sel         : 1   ; // [0] 
        unsigned int    vga_data_sel          : 1   ; // [1] 
        unsigned int    Reserved_502          : 1   ; // [2] 
        unsigned int    hdmi_data_sel         : 1   ; // [3] 
        unsigned int    Reserved_501          : 28  ; // [31..4] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_DATA_SEL;

// Define the union U_DHD0_ABUFTHD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dhd_buf_thd           : 11  ; // [10..0] 
        unsigned int    Reserved_504          : 21  ; // [31..11] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_ABUFTHD;

// Define the union U_DHD0_CSC_IDC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc0               : 11  ; // [10..0] 
        unsigned int    cscidc1               : 11  ; // [21..11] 
        unsigned int    csc_en                : 1   ; // [22] 
        unsigned int    Reserved_506          : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CSC_IDC;

// Define the union U_DHD0_CSC_ODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscodc0               : 11  ; // [10..0] 
        unsigned int    cscodc1               : 11  ; // [21..11] 
        unsigned int    csc_sign_mode         : 1   ; // [22] 
        unsigned int    Reserved_508          : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CSC_ODC;

// Define the union U_DHD0_CSC_IODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc2               : 11  ; // [10..0] 
        unsigned int    cscodc2               : 11  ; // [21..11] 
        unsigned int    Reserved_509          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CSC_IODC;

// Define the union U_DHD0_CSC_P0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp00                : 15  ; // [14..0] 
        unsigned int    Reserved_511          : 1   ; // [15] 
        unsigned int    cscp01                : 15  ; // [30..16] 
        unsigned int    Reserved_510          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CSC_P0;

// Define the union U_DHD0_CSC_P1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp02                : 15  ; // [14..0] 
        unsigned int    Reserved_513          : 1   ; // [15] 
        unsigned int    cscp10                : 15  ; // [30..16] 
        unsigned int    Reserved_512          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CSC_P1;

// Define the union U_DHD0_CSC_P2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp11                : 15  ; // [14..0] 
        unsigned int    Reserved_515          : 1   ; // [15] 
        unsigned int    cscp12                : 15  ; // [30..16] 
        unsigned int    Reserved_514          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CSC_P2;

// Define the union U_DHD0_CSC_P3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp20                : 15  ; // [14..0] 
        unsigned int    Reserved_517          : 1   ; // [15] 
        unsigned int    cscp21                : 15  ; // [30..16] 
        unsigned int    Reserved_516          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CSC_P3;

// Define the union U_DHD0_CSC_P4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp22                : 15  ; // [14..0] 
        unsigned int    Reserved_518          : 17  ; // [31..15] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CSC_P4;

// Define the union U_DHD0_CLIP1_L
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    clip_cl0              : 10  ; // [9..0] 
        unsigned int    clip_cl1              : 10  ; // [19..10] 
        unsigned int    clip_cl2              : 10  ; // [29..20] 
        unsigned int    Reserved_519          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CLIP1_L;

// Define the union U_DHD0_CLIP1_H
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    clip_ch0              : 10  ; // [9..0] 
        unsigned int    clip_ch1              : 10  ; // [19..10] 
        unsigned int    clip_ch2              : 10  ; // [29..20] 
        unsigned int    Reserved_521          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CLIP1_H;

// Define the union U_DHD0_CLIP2_L
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    clip_cl0              : 10  ; // [9..0] 
        unsigned int    clip_cl1              : 10  ; // [19..10] 
        unsigned int    clip_cl2              : 10  ; // [29..20] 
        unsigned int    Reserved_522          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CLIP2_L;

// Define the union U_DHD0_CLIP2_H
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    clip_ch0              : 10  ; // [9..0] 
        unsigned int    clip_ch1              : 10  ; // [19..10] 
        unsigned int    clip_ch2              : 10  ; // [29..20] 
        unsigned int    Reserved_523          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CLIP2_H;

// Define the union U_DHD0_CLIP3_L
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    clip_cl0              : 10  ; // [9..0] 
        unsigned int    clip_cl1              : 10  ; // [19..10] 
        unsigned int    clip_cl2              : 10  ; // [29..20] 
        unsigned int    Reserved_524          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CLIP3_L;

// Define the union U_DHD0_CLIP3_H
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    clip_ch0              : 10  ; // [9..0] 
        unsigned int    clip_ch1              : 10  ; // [19..10] 
        unsigned int    clip_ch2              : 10  ; // [29..20] 
        unsigned int    Reserved_525          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CLIP3_H;

// Define the union U_DHD0_PARATHD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    para_thd              : 6   ; // [5..0] 
        unsigned int    Reserved_526          : 25  ; // [30..6] 
        unsigned int    dfs_en                : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_PARATHD;

// Define the union U_DHD0_START_POS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    start_pos             : 8   ; // [7..0] 
        unsigned int    timing_start_pos      : 8   ; // [15..8] 
        unsigned int    Reserved_528          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_START_POS;

// Define the union U_DHD0_LOCKCFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    measure_en            : 1   ; // [0] 
        unsigned int    lock_cnt_en           : 1   ; // [1] 
        unsigned int    Reserved_530          : 30  ; // [31..2] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_LOCKCFG;

// Define the union U_DHD0_LOCK_STATE1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cap_frm_cnt           : 26  ; // [25..0] 
        unsigned int    Reserved_532          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_LOCK_STATE1;

// Define the union U_DHD0_LOCK_STATE2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vsync_cap_vdp_init    : 26  ; // [25..0] 
        unsigned int    Reserved_533          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_LOCK_STATE2;

// Define the union U_DHD0_LOCK_STATE3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vsync_cap_vdp_cnt     : 26  ; // [25..0] 
        unsigned int    Reserved_534          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_LOCK_STATE3;

// Define the union U_DHD0_GMM_COEFAD
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DHD0_GMM_COEFAD;
// Define the union U_DHD0_PARAUP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dhd0_gmm_upd          : 1   ; // [0] 
        unsigned int    Reserved_535          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_PARAUP;

// Define the union U_DHD0_STATE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vback_blank           : 1   ; // [0] 
        unsigned int    vblank                : 1   ; // [1] 
        unsigned int    bottom_field          : 1   ; // [2] 
        unsigned int    vcnt                  : 13  ; // [15..3] 
        unsigned int    count_int             : 8   ; // [23..16] 
        unsigned int    Reserved_537          : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_STATE;

// Define the union U_DHD0_DEBUG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    pix_h                 : 16  ; // [15..0] 
        unsigned int    pix_v                 : 12  ; // [27..16] 
        unsigned int    pix_src               : 3   ; // [30..28] 
        unsigned int    Reserved_538          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_DEBUG;

// Define the union U_DHD0_DEBUG_STATE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    pixel_value           : 30  ; // [29..0] 
        unsigned int    Reserved_540          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_DEBUG_STATE;

// Define the union U_HDATE_VERSION
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int hdate_version           : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_HDATE_VERSION;
// Define the union U_HDATE_EN
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hd_en                 : 1   ; // [0] 
        unsigned int    Reserved_542          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_EN;

// Define the union U_HDATE_POLA_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hsync_in_pola         : 1   ; // [0] 
        unsigned int    vsync_in_pola         : 1   ; // [1] 
        unsigned int    fid_in_pola           : 1   ; // [2] 
        unsigned int    hsync_out_pola        : 1   ; // [3] 
        unsigned int    vsync_out_pola        : 1   ; // [4] 
        unsigned int    fid_out_pola          : 1   ; // [5] 
        unsigned int    Reserved_543          : 26  ; // [31..6] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_POLA_CTRL;

// Define the union U_HDATE_VIDEO_FORMAT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_ft              : 4   ; // [3..0] 
        unsigned int    sync_add_ctrl         : 3   ; // [6..4] 
        unsigned int    video_out_ctrl        : 2   ; // [8..7] 
        unsigned int    csc_ctrl              : 3   ; // [11..9] 
        unsigned int    csc_round_disable     : 1   ; // [12] 
        unsigned int    Reserved_544          : 19  ; // [31..13] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_VIDEO_FORMAT;

// Define the union U_HDATE_STATE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    line_len              : 13  ; // [12..0] 
        unsigned int    Reserved_547          : 3   ; // [15..13] 
        unsigned int    frame_len             : 11  ; // [26..16] 
        unsigned int    Reserved_546          : 1   ; // [27] 
        unsigned int    mv_en_pin             : 1   ; // [28] 
        unsigned int    Reserved_545          : 3   ; // [31..29] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_STATE;

// Define the union U_HDATE_OUT_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vsync_sel             : 2   ; // [1..0] 
        unsigned int    hsync_sel             : 2   ; // [3..2] 
        unsigned int    video3_sel            : 2   ; // [5..4] 
        unsigned int    video2_sel            : 2   ; // [7..6] 
        unsigned int    video1_sel            : 2   ; // [9..8] 
        unsigned int    src_ctrl              : 2   ; // [11..10] 
        unsigned int    sync_lpf_en           : 1   ; // [12] 
        unsigned int    sd_sel                : 1   ; // [13] 
        unsigned int    src_round_disable     : 1   ; // [14] 
        unsigned int    Reserved_548          : 17  ; // [31..15] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_OUT_CTRL;

// Define the union U_HDATE_SRC_13_COEF1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap1_1           : 11  ; // [10..0] 
        unsigned int    Reserved_550          : 5   ; // [15..11] 
        unsigned int    coef_tap1_3           : 11  ; // [26..16] 
        unsigned int    Reserved_549          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF1;

// Define the union U_HDATE_SRC_13_COEF2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap2_1           : 11  ; // [10..0] 
        unsigned int    Reserved_552          : 5   ; // [15..11] 
        unsigned int    coef_tap2_3           : 11  ; // [26..16] 
        unsigned int    Reserved_551          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF2;

// Define the union U_HDATE_SRC_13_COEF3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap3_1           : 11  ; // [10..0] 
        unsigned int    Reserved_554          : 5   ; // [15..11] 
        unsigned int    coef_tap3_3           : 11  ; // [26..16] 
        unsigned int    Reserved_553          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF3;

// Define the union U_HDATE_SRC_13_COEF4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap4_1           : 11  ; // [10..0] 
        unsigned int    Reserved_556          : 5   ; // [15..11] 
        unsigned int    coef_tap4_3           : 11  ; // [26..16] 
        unsigned int    Reserved_555          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF4;

// Define the union U_HDATE_SRC_13_COEF5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap5_1           : 11  ; // [10..0] 
        unsigned int    Reserved_558          : 5   ; // [15..11] 
        unsigned int    coef_tap5_3           : 11  ; // [26..16] 
        unsigned int    Reserved_557          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF5;

// Define the union U_HDATE_SRC_13_COEF6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap6_1           : 11  ; // [10..0] 
        unsigned int    Reserved_560          : 5   ; // [15..11] 
        unsigned int    coef_tap6_3           : 11  ; // [26..16] 
        unsigned int    Reserved_559          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF6;

// Define the union U_HDATE_SRC_13_COEF7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap7_1           : 11  ; // [10..0] 
        unsigned int    Reserved_562          : 5   ; // [15..11] 
        unsigned int    coef_tap7_3           : 11  ; // [26..16] 
        unsigned int    Reserved_561          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF7;

// Define the union U_HDATE_SRC_13_COEF8
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap8_1           : 11  ; // [10..0] 
        unsigned int    Reserved_564          : 5   ; // [15..11] 
        unsigned int    coef_tap8_3           : 11  ; // [26..16] 
        unsigned int    Reserved_563          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF8;

// Define the union U_HDATE_SRC_13_COEF9
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap9_1           : 11  ; // [10..0] 
        unsigned int    Reserved_566          : 5   ; // [15..11] 
        unsigned int    coef_tap9_3           : 11  ; // [26..16] 
        unsigned int    Reserved_565          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF9;

// Define the union U_HDATE_SRC_13_COEF10
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap10_1          : 11  ; // [10..0] 
        unsigned int    Reserved_568          : 5   ; // [15..11] 
        unsigned int    coef_tap10_3          : 11  ; // [26..16] 
        unsigned int    Reserved_567          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF10;

// Define the union U_HDATE_SRC_13_COEF11
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap11_1          : 11  ; // [10..0] 
        unsigned int    Reserved_570          : 5   ; // [15..11] 
        unsigned int    coef_tap11_3          : 11  ; // [26..16] 
        unsigned int    Reserved_569          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF11;

// Define the union U_HDATE_SRC_13_COEF12
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap12_1          : 11  ; // [10..0] 
        unsigned int    Reserved_572          : 5   ; // [15..11] 
        unsigned int    coef_tap12_3          : 11  ; // [26..16] 
        unsigned int    Reserved_571          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF12;

// Define the union U_HDATE_SRC_13_COEF13
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap13_1          : 11  ; // [10..0] 
        unsigned int    Reserved_574          : 5   ; // [15..11] 
        unsigned int    coef_tap13_3          : 11  ; // [26..16] 
        unsigned int    Reserved_573          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF13;

// Define the union U_HDATE_SRC_24_COEF1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap1_2           : 11  ; // [10..0] 
        unsigned int    Reserved_576          : 5   ; // [15..11] 
        unsigned int    coef_tap1_4           : 11  ; // [26..16] 
        unsigned int    Reserved_575          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF1;

// Define the union U_HDATE_SRC_24_COEF2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap2_2           : 11  ; // [10..0] 
        unsigned int    Reserved_578          : 5   ; // [15..11] 
        unsigned int    coef_tap2_4           : 11  ; // [26..16] 
        unsigned int    Reserved_577          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF2;

// Define the union U_HDATE_SRC_24_COEF3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap3_2           : 11  ; // [10..0] 
        unsigned int    Reserved_580          : 5   ; // [15..11] 
        unsigned int    coef_tap3_4           : 11  ; // [26..16] 
        unsigned int    Reserved_579          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF3;

// Define the union U_HDATE_SRC_24_COEF4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap4_2           : 11  ; // [10..0] 
        unsigned int    Reserved_582          : 5   ; // [15..11] 
        unsigned int    coef_tap4_4           : 11  ; // [26..16] 
        unsigned int    Reserved_581          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF4;

// Define the union U_HDATE_SRC_24_COEF5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap5_2           : 11  ; // [10..0] 
        unsigned int    Reserved_584          : 5   ; // [15..11] 
        unsigned int    coef_tap5_4           : 11  ; // [26..16] 
        unsigned int    Reserved_583          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF5;

// Define the union U_HDATE_SRC_24_COEF6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap6_2           : 11  ; // [10..0] 
        unsigned int    Reserved_586          : 5   ; // [15..11] 
        unsigned int    coef_tap6_4           : 11  ; // [26..16] 
        unsigned int    Reserved_585          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF6;

// Define the union U_HDATE_SRC_24_COEF7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap7_2           : 11  ; // [10..0] 
        unsigned int    Reserved_588          : 5   ; // [15..11] 
        unsigned int    coef_tap7_4           : 11  ; // [26..16] 
        unsigned int    Reserved_587          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF7;

// Define the union U_HDATE_SRC_24_COEF8
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap8_2           : 11  ; // [10..0] 
        unsigned int    Reserved_590          : 5   ; // [15..11] 
        unsigned int    coef_tap8_4           : 11  ; // [26..16] 
        unsigned int    Reserved_589          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF8;

// Define the union U_HDATE_SRC_24_COEF9
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap9_2           : 11  ; // [10..0] 
        unsigned int    Reserved_592          : 5   ; // [15..11] 
        unsigned int    coef_tap9_4           : 11  ; // [26..16] 
        unsigned int    Reserved_591          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF9;

// Define the union U_HDATE_SRC_24_COEF10
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap10_2          : 11  ; // [10..0] 
        unsigned int    Reserved_594          : 5   ; // [15..11] 
        unsigned int    coef_tap10_4          : 11  ; // [26..16] 
        unsigned int    Reserved_593          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF10;

// Define the union U_HDATE_SRC_24_COEF11
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap11_2          : 11  ; // [10..0] 
        unsigned int    Reserved_596          : 5   ; // [15..11] 
        unsigned int    coef_tap11_4          : 11  ; // [26..16] 
        unsigned int    Reserved_595          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF11;

// Define the union U_HDATE_SRC_24_COEF12
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap12_2          : 11  ; // [10..0] 
        unsigned int    Reserved_598          : 5   ; // [15..11] 
        unsigned int    coef_tap12_4          : 11  ; // [26..16] 
        unsigned int    Reserved_597          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF12;

// Define the union U_HDATE_SRC_24_COEF13
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap13_2          : 11  ; // [10..0] 
        unsigned int    Reserved_600          : 5   ; // [15..11] 
        unsigned int    coef_tap13_4          : 11  ; // [26..16] 
        unsigned int    Reserved_599          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF13;

// Define the union U_HDATE_CSC_COEF1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    csc_coef_r_y          : 12  ; // [11..0] 
        unsigned int    Reserved_602          : 4   ; // [15..12] 
        unsigned int    csc_coef_r_cb         : 12  ; // [27..16] 
        unsigned int    Reserved_601          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_CSC_COEF1;

// Define the union U_HDATE_CSC_COEF2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    csc_coef_r_cr         : 12  ; // [11..0] 
        unsigned int    Reserved_604          : 4   ; // [15..12] 
        unsigned int    csc_coef_g_y          : 12  ; // [27..16] 
        unsigned int    Reserved_603          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_CSC_COEF2;

// Define the union U_HDATE_CSC_COEF3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    csc_coef_g_cb         : 12  ; // [11..0] 
        unsigned int    Reserved_606          : 4   ; // [15..12] 
        unsigned int    csc_coef_g_cr         : 12  ; // [27..16] 
        unsigned int    Reserved_605          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_CSC_COEF3;

// Define the union U_HDATE_CSC_COEF4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    csc_coef_b_y          : 12  ; // [11..0] 
        unsigned int    Reserved_608          : 4   ; // [15..12] 
        unsigned int    csc_coef_b_cb         : 12  ; // [27..16] 
        unsigned int    Reserved_607          : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_CSC_COEF4;

// Define the union U_HDATE_CSC_COEF5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    csc_coef_b_cr         : 12  ; // [11..0] 
        unsigned int    Reserved_609          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_CSC_COEF5;

// Define the union U_HDATE_TEST
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    test_data             : 10  ; // [9..0] 
        unsigned int    Reserved_610          : 22  ; // [31..10] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_TEST;

// Define the union U_HDATE_VBI_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cgmsb_add_en          : 1   ; // [0] 
        unsigned int    cgmsa_add_en          : 1   ; // [1] 
        unsigned int    mv_en                 : 1   ; // [2] 
        unsigned int    vbi_lpf_en            : 1   ; // [3] 
        unsigned int    Reserved_612          : 28  ; // [31..4] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_VBI_CTRL;

// Define the union U_HDATE_CGMSA_DATA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cgmsa_data            : 20  ; // [19..0] 
        unsigned int    Reserved_613          : 12  ; // [31..20] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_CGMSA_DATA;

// Define the union U_HDATE_CGMSB_H
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hdate_cgmsb_h         : 6   ; // [5..0] 
        unsigned int    Reserved_614          : 26  ; // [31..6] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_CGMSB_H;

// Define the union U_HDATE_CGMSB_DATA1
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int cgmsb_data1             : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_HDATE_CGMSB_DATA1;
// Define the union U_HDATE_CGMSB_DATA2
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int cgmsb_data2             : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_HDATE_CGMSB_DATA2;
// Define the union U_HDATE_CGMSB_DATA3
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int cgmsb_data3             : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_HDATE_CGMSB_DATA3;
// Define the union U_HDATE_CGMSB_DATA4
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int cgmsb_data4             : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_HDATE_CGMSB_DATA4;
// Define the union U_HDATE_DACDET1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vdac_det_high         : 10  ; // [9..0] 
        unsigned int    Reserved_616          : 6   ; // [15..10] 
        unsigned int    det_line              : 10  ; // [25..16] 
        unsigned int    Reserved_615          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_DACDET1;

// Define the union U_HDATE_DACDET2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    det_pixel_sta         : 11  ; // [10..0] 
        unsigned int    Reserved_618          : 5   ; // [15..11] 
        unsigned int    det_pixel_wid         : 11  ; // [26..16] 
        unsigned int    Reserved_617          : 4   ; // [30..27] 
        unsigned int    vdac_det_en           : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_DACDET2;

// Define the union U_HDATE_SRC_13_COEF14
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap14_1          : 11  ; // [10..0] 
        unsigned int    Reserved_620          : 5   ; // [15..11] 
        unsigned int    coef_tap14_3          : 11  ; // [26..16] 
        unsigned int    Reserved_619          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF14;

// Define the union U_HDATE_SRC_13_COEF15
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap15_1          : 11  ; // [10..0] 
        unsigned int    Reserved_622          : 5   ; // [15..11] 
        unsigned int    coef_tap15_3          : 11  ; // [26..16] 
        unsigned int    Reserved_621          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF15;

// Define the union U_HDATE_SRC_13_COEF16
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap16_1          : 11  ; // [10..0] 
        unsigned int    Reserved_624          : 5   ; // [15..11] 
        unsigned int    coef_tap16_3          : 11  ; // [26..16] 
        unsigned int    Reserved_623          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF16;

// Define the union U_HDATE_SRC_13_COEF17
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap17_1          : 11  ; // [10..0] 
        unsigned int    Reserved_626          : 5   ; // [15..11] 
        unsigned int    coef_tap17_3          : 11  ; // [26..16] 
        unsigned int    Reserved_625          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF17;

// Define the union U_HDATE_SRC_13_COEF18
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap18_1          : 11  ; // [10..0] 
        unsigned int    Reserved_628          : 5   ; // [15..11] 
        unsigned int    coef_tap18_3          : 11  ; // [26..16] 
        unsigned int    Reserved_627          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_13_COEF18;

// Define the union U_HDATE_SRC_24_COEF14
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap14_2          : 11  ; // [10..0] 
        unsigned int    Reserved_630          : 5   ; // [15..11] 
        unsigned int    coef_tap14_4          : 11  ; // [26..16] 
        unsigned int    Reserved_629          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF14;

// Define the union U_HDATE_SRC_24_COEF15
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap15_2          : 11  ; // [10..0] 
        unsigned int    Reserved_632          : 5   ; // [15..11] 
        unsigned int    coef_tap15_4          : 11  ; // [26..16] 
        unsigned int    Reserved_631          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF15;

// Define the union U_HDATE_SRC_24_COEF16
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap16_2          : 11  ; // [10..0] 
        unsigned int    Reserved_634          : 5   ; // [15..11] 
        unsigned int    coef_tap16_4          : 11  ; // [26..16] 
        unsigned int    Reserved_633          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF16;

// Define the union U_HDATE_SRC_24_COEF17
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap17_2          : 11  ; // [10..0] 
        unsigned int    Reserved_636          : 5   ; // [15..11] 
        unsigned int    coef_tap17_4          : 11  ; // [26..16] 
        unsigned int    Reserved_635          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF17;

// Define the union U_HDATE_SRC_24_COEF18
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_tap18_2          : 11  ; // [10..0] 
        unsigned int    Reserved_638          : 5   ; // [15..11] 
        unsigned int    coef_tap18_4          : 11  ; // [26..16] 
        unsigned int    Reserved_637          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_SRC_24_COEF18;

// Define the union U_HDATE_CLIP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    clip_thdl             : 10  ; // [9..0] 
        unsigned int    Reserved_639          : 5   ; // [14..10] 
        unsigned int    clip_disable          : 1   ; // [15] 
        unsigned int    clip_fb               : 8   ; // [23..16] 
        unsigned int    clip_bb               : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_HDATE_CLIP;

// Define the union U_DATE_COEFF0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tt_seq                : 1   ; // [0] 
        unsigned int    chgain_en             : 1   ; // [1] 
        unsigned int    sylp_en               : 1   ; // [2] 
        unsigned int    chlp_en               : 1   ; // [3] 
        unsigned int    oversam2_en           : 1   ; // [4] 
        unsigned int    lunt_en               : 1   ; // [5] 
        unsigned int    oversam_en            : 2   ; // [7..6] 
        unsigned int    Reserved_641          : 1   ; // [8] 
        unsigned int    luma_dl               : 4   ; // [12..9] 
        unsigned int    agc_amp_sel           : 1   ; // [13] 
        unsigned int    length_sel            : 1   ; // [14] 
        unsigned int    sync_mode_scart       : 1   ; // [15] 
        unsigned int    sync_mode_sel         : 2   ; // [17..16] 
        unsigned int    style_sel             : 4   ; // [21..18] 
        unsigned int    fm_sel                : 1   ; // [22] 
        unsigned int    vbi_lpf_en            : 1   ; // [23] 
        unsigned int    rgb_en                : 1   ; // [24] 
        unsigned int    scanline              : 1   ; // [25] 
        unsigned int    pbpr_lpf_en           : 1   ; // [26] 
        unsigned int    pal_half_en           : 1   ; // [27] 
        unsigned int    Reserved_640          : 1   ; // [28] 
        unsigned int    dis_ire               : 1   ; // [29] 
        unsigned int    clpf_sel              : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF0;

// Define the union U_DATE_COEFF1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dac_test              : 10  ; // [9..0] 
        unsigned int    date_test_mode        : 2   ; // [11..10] 
        unsigned int    date_test_en          : 1   ; // [12] 
        unsigned int    amp_outside           : 10  ; // [22..13] 
        unsigned int    c_limit_en            : 1   ; // [23] 
        unsigned int    cc_seq                : 1   ; // [24] 
        unsigned int    cgms_seq              : 1   ; // [25] 
        unsigned int    vps_seq               : 1   ; // [26] 
        unsigned int    wss_seq               : 1   ; // [27] 
        unsigned int    cvbs_limit_en         : 1   ; // [28] 
        unsigned int    c_gain                : 3   ; // [31..29] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF1;

// Define the union U_DATE_COEFF2
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int coef02                  : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DATE_COEFF2;
// Define the union U_DATE_COEFF3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef03                : 26  ; // [25..0] 
        unsigned int    Reserved_643          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF3;

// Define the union U_DATE_COEFF4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef04                : 30  ; // [29..0] 
        unsigned int    Reserved_644          : 2   ; // [31..30] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF4;

// Define the union U_DATE_COEFF5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef05                : 29  ; // [28..0] 
        unsigned int    Reserved_645          : 3   ; // [31..29] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF5;

// Define the union U_DATE_COEFF6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef06_1              : 23  ; // [22..0] 
        unsigned int    Reserved_646          : 8   ; // [30..23] 
        unsigned int    coef06_0              : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF6;

// Define the union U_DATE_COEFF7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tt07_enf2             : 1   ; // [0] 
        unsigned int    tt08_enf2             : 1   ; // [1] 
        unsigned int    tt09_enf2             : 1   ; // [2] 
        unsigned int    tt10_enf2             : 1   ; // [3] 
        unsigned int    tt11_enf2             : 1   ; // [4] 
        unsigned int    tt12_enf2             : 1   ; // [5] 
        unsigned int    tt13_enf2             : 1   ; // [6] 
        unsigned int    tt14_enf2             : 1   ; // [7] 
        unsigned int    tt15_enf2             : 1   ; // [8] 
        unsigned int    tt16_enf2             : 1   ; // [9] 
        unsigned int    tt17_enf2             : 1   ; // [10] 
        unsigned int    tt18_enf2             : 1   ; // [11] 
        unsigned int    tt19_enf2             : 1   ; // [12] 
        unsigned int    tt20_enf2             : 1   ; // [13] 
        unsigned int    tt21_enf2             : 1   ; // [14] 
        unsigned int    tt22_enf2             : 1   ; // [15] 
        unsigned int    tt07_enf1             : 1   ; // [16] 
        unsigned int    tt08_enf1             : 1   ; // [17] 
        unsigned int    tt09_enf1             : 1   ; // [18] 
        unsigned int    tt10_enf1             : 1   ; // [19] 
        unsigned int    tt11_enf1             : 1   ; // [20] 
        unsigned int    tt12_enf1             : 1   ; // [21] 
        unsigned int    tt13_enf1             : 1   ; // [22] 
        unsigned int    tt14_enf1             : 1   ; // [23] 
        unsigned int    tt15_enf1             : 1   ; // [24] 
        unsigned int    tt16_enf1             : 1   ; // [25] 
        unsigned int    tt17_enf1             : 1   ; // [26] 
        unsigned int    tt18_enf1             : 1   ; // [27] 
        unsigned int    tt19_enf1             : 1   ; // [28] 
        unsigned int    tt20_enf1             : 1   ; // [29] 
        unsigned int    tt21_enf1             : 1   ; // [30] 
        unsigned int    tt22_enf1             : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF7;

// Define the union U_DATE_COEFF8
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int tt_staddr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DATE_COEFF8;
// Define the union U_DATE_COEFF9
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int tt_edaddr               : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DATE_COEFF9;
// Define the union U_DATE_COEFF10
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tt_pktoff             : 8   ; // [7..0] 
        unsigned int    tt_mode               : 2   ; // [9..8] 
        unsigned int    tt_highest            : 1   ; // [10] 
        unsigned int    full_page             : 1   ; // [11] 
        unsigned int    nabts_100ire          : 1   ; // [12] 
        unsigned int    Reserved_647          : 18  ; // [30..13] 
        unsigned int    tt_ready              : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF10;

// Define the union U_DATE_COEFF11
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    date_clf2             : 10  ; // [9..0] 
        unsigned int    date_clf1             : 10  ; // [19..10] 
        unsigned int    cc_enf2               : 1   ; // [20] 
        unsigned int    cc_enf1               : 1   ; // [21] 
        unsigned int    Reserved_648          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF11;

// Define the union U_DATE_COEFF12
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cc_f2data             : 16  ; // [15..0] 
        unsigned int    cc_f1data             : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF12;

// Define the union U_DATE_COEFF13
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cg_f1data             : 20  ; // [19..0] 
        unsigned int    cg_enf2               : 1   ; // [20] 
        unsigned int    cg_enf1               : 1   ; // [21] 
        unsigned int    Reserved_649          : 10  ; // [31..22] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF13;

// Define the union U_DATE_COEFF14
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cg_f2data             : 20  ; // [19..0] 
        unsigned int    Reserved_650          : 12  ; // [31..20] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF14;

// Define the union U_DATE_COEFF15
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wss_data              : 14  ; // [13..0] 
        unsigned int    wss_en                : 1   ; // [14] 
        unsigned int    Reserved_651          : 17  ; // [31..15] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF15;

// Define the union U_DATE_COEFF16
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vps_data              : 24  ; // [23..0] 
        unsigned int    vps_en                : 1   ; // [24] 
        unsigned int    Reserved_652          : 7   ; // [31..25] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF16;

// Define the union U_DATE_COEFF17
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int vps_data                : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DATE_COEFF17;
// Define the union U_DATE_COEFF18
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int vps_data                : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DATE_COEFF18;
// Define the union U_DATE_COEFF19
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vps_data              : 16  ; // [15..0] 
        unsigned int    Reserved_653          : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF19;

// Define the union U_DATE_COEFF20
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tt05_enf2             : 1   ; // [0] 
        unsigned int    tt06_enf2             : 1   ; // [1] 
        unsigned int    tt06_enf1             : 1   ; // [2] 
        unsigned int    Reserved_654          : 29  ; // [31..3] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF20;

// Define the union U_DATE_COEFF21
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dac0_in_sel           : 3   ; // [2..0] 
        unsigned int    Reserved_660          : 1   ; // [3] 
        unsigned int    dac1_in_sel           : 3   ; // [6..4] 
        unsigned int    Reserved_659          : 1   ; // [7] 
        unsigned int    dac2_in_sel           : 3   ; // [10..8] 
        unsigned int    Reserved_658          : 1   ; // [11] 
        unsigned int    dac3_in_sel           : 3   ; // [14..12] 
        unsigned int    Reserved_657          : 1   ; // [15] 
        unsigned int    dac4_in_sel           : 3   ; // [18..16] 
        unsigned int    Reserved_656          : 1   ; // [19] 
        unsigned int    dac5_in_sel           : 3   ; // [22..20] 
        unsigned int    Reserved_655          : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF21;

// Define the union U_DATE_COEFF22
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_phase_delta     : 11  ; // [10..0] 
        unsigned int    Reserved_661          : 21  ; // [31..11] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF22;

// Define the union U_DATE_COEFF23
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dac0_out_dly          : 3   ; // [2..0] 
        unsigned int    Reserved_667          : 1   ; // [3] 
        unsigned int    dac1_out_dly          : 3   ; // [6..4] 
        unsigned int    Reserved_666          : 1   ; // [7] 
        unsigned int    dac2_out_dly          : 3   ; // [10..8] 
        unsigned int    Reserved_665          : 1   ; // [11] 
        unsigned int    dac3_out_dly          : 3   ; // [14..12] 
        unsigned int    Reserved_664          : 1   ; // [15] 
        unsigned int    dac4_out_dly          : 3   ; // [18..16] 
        unsigned int    Reserved_663          : 1   ; // [19] 
        unsigned int    dac5_out_dly          : 3   ; // [22..20] 
        unsigned int    Reserved_662          : 9   ; // [31..23] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF23;

// Define the union U_DATE_COEFF24
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int burst_start             : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DATE_COEFF24;
// Define the union U_DATE_COEFF25
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    x_n_coef              : 13  ; // [12..0] 
        unsigned int    Reserved_669          : 3   ; // [15..13] 
        unsigned int    x_n_1_coef            : 13  ; // [28..16] 
        unsigned int    Reserved_668          : 3   ; // [31..29] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF25;

// Define the union U_DATE_COEFF26
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    x_n_1_coef            : 13  ; // [12..0] 
        unsigned int    Reserved_670          : 19  ; // [31..13] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF26;

// Define the union U_DATE_COEFF27
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    y_n_coef              : 11  ; // [10..0] 
        unsigned int    Reserved_672          : 5   ; // [15..11] 
        unsigned int    y_n_1_coef            : 11  ; // [26..16] 
        unsigned int    Reserved_671          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF27;

// Define the union U_DATE_COEFF28
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    pixel_begin1          : 11  ; // [10..0] 
        unsigned int    Reserved_674          : 5   ; // [15..11] 
        unsigned int    pixel_begin2          : 11  ; // [26..16] 
        unsigned int    Reserved_673          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF28;

// Define the union U_DATE_COEFF29
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    pixel_end             : 11  ; // [10..0] 
        unsigned int    Reserved_675          : 21  ; // [31..11] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF29;

// Define the union U_DATE_COEFF30
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    g_secam               : 7   ; // [6..0] 
        unsigned int    Reserved_676          : 25  ; // [31..7] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF30;

// Define the union U_DATE_ISRMASK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tt_mask               : 1   ; // [0] 
        unsigned int    Reserved_677          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_ISRMASK;

// Define the union U_DATE_ISRSTATE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tt_status             : 1   ; // [0] 
        unsigned int    Reserved_679          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_ISRSTATE;

// Define the union U_DATE_ISR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tt_int                : 1   ; // [0] 
        unsigned int    Reserved_680          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_ISR;

// Define the union U_DATE_VERSION
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int Reserved_681            : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DATE_VERSION;
// Define the union U_DATE_COEFF37
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fir_y1_coeff0         : 8   ; // [7..0] 
        unsigned int    fir_y1_coeff1         : 8   ; // [15..8] 
        unsigned int    fir_y1_coeff2         : 8   ; // [23..16] 
        unsigned int    fir_y1_coeff3         : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF37;

// Define the union U_DATE_COEFF38
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fir_y2_coeff0         : 16  ; // [15..0] 
        unsigned int    fir_y2_coeff1         : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF38;

// Define the union U_DATE_COEFF39
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fir_y2_coeff2         : 16  ; // [15..0] 
        unsigned int    fir_y2_coeff3         : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF39;

// Define the union U_DATE_COEFF40
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fir_c1_coeff0         : 8   ; // [7..0] 
        unsigned int    fir_c1_coeff1         : 8   ; // [15..8] 
        unsigned int    fir_c1_coeff2         : 8   ; // [23..16] 
        unsigned int    fir_c1_coeff3         : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF40;

// Define the union U_DATE_COEFF41
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fir_c2_coeff0         : 16  ; // [15..0] 
        unsigned int    fir_c2_coeff1         : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF41;

// Define the union U_DATE_COEFF42
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fir_c2_coeff2         : 16  ; // [15..0] 
        unsigned int    fir_c2_coeff3         : 16  ; // [31..16] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF42;

// Define the union U_DATE_DACDET1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vdac_det_high         : 10  ; // [9..0] 
        unsigned int    Reserved_684          : 6   ; // [15..10] 
        unsigned int    det_line              : 10  ; // [25..16] 
        unsigned int    Reserved_683          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_DACDET1;

// Define the union U_DATE_DACDET2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    det_pixel_sta         : 11  ; // [10..0] 
        unsigned int    Reserved_687          : 5   ; // [15..11] 
        unsigned int    det_pixel_wid         : 11  ; // [26..16] 
        unsigned int    Reserved_686          : 4   ; // [30..27] 
        unsigned int    vdac_det_en           : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_DACDET2;

// Define the union U_DATE_COEFF50
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ovs_coeff0            : 11  ; // [10..0] 
        unsigned int    Reserved_689          : 5   ; // [15..11] 
        unsigned int    ovs_coeff1            : 11  ; // [26..16] 
        unsigned int    Reserved_688          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF50;

// Define the union U_DATE_COEFF51
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ovs_coeff0            : 11  ; // [10..0] 
        unsigned int    Reserved_691          : 5   ; // [15..11] 
        unsigned int    ovs_coeff1            : 11  ; // [26..16] 
        unsigned int    Reserved_690          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF51;

// Define the union U_DATE_COEFF52
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ovs_coeff0            : 11  ; // [10..0] 
        unsigned int    Reserved_693          : 5   ; // [15..11] 
        unsigned int    ovs_coeff1            : 11  ; // [26..16] 
        unsigned int    Reserved_692          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF52;

// Define the union U_DATE_COEFF53
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ovs_coeff0            : 11  ; // [10..0] 
        unsigned int    Reserved_695          : 5   ; // [15..11] 
        unsigned int    ovs_coeff1            : 11  ; // [26..16] 
        unsigned int    Reserved_694          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF53;

// Define the union U_DATE_COEFF54
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ovs_coeff0            : 11  ; // [10..0] 
        unsigned int    Reserved_697          : 5   ; // [15..11] 
        unsigned int    ovs_coeff1            : 11  ; // [26..16] 
        unsigned int    Reserved_696          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF54;

// Define the union U_DATE_COEFF55
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ovs_coeff0            : 11  ; // [10..0] 
        unsigned int    Reserved_699          : 5   ; // [15..11] 
        unsigned int    ovs_coeff1            : 11  ; // [26..16] 
        unsigned int    Reserved_698          : 5   ; // [31..27] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF55;

// Define the union U_DATE_COEFF56
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    oversam2_round_en     : 1   ; // [0] 
        unsigned int    Reserved_700          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF56;

// Define the union U_DATE_COEFF57
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    v_gain                : 8   ; // [7..0] 
        unsigned int    u_gain                : 8   ; // [15..8] 
        unsigned int    ycvbs_gain            : 8   ; // [23..16] 
        unsigned int    Reserved_701          : 7   ; // [30..24] 
        unsigned int    cvbs_gain_en          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF57;

// Define the union U_DATE_COEFF58
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    pr_gain               : 8   ; // [7..0] 
        unsigned int    pb_gain               : 8   ; // [15..8] 
        unsigned int    ycomp_gain            : 8   ; // [23..16] 
        unsigned int    Reserved_702          : 7   ; // [30..24] 
        unsigned int    comp_gain_en          : 1   ; // [31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF58;

// Define the union U_DATE_COEFF59
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ynotch_clip_fullrange  : 1   ; // [0] 
        unsigned int    clpf_clip_fullrange   : 1   ; // [1] 
        unsigned int    Reserved_707          : 2   ; // [3..2] 
        unsigned int    y_os_clip_fullrange   : 1   ; // [4] 
        unsigned int    Reserved_706          : 3   ; // [7..5] 
        unsigned int    u_os_clip_fullrange   : 1   ; // [8] 
        unsigned int    v_os_clip_fullrange   : 1   ; // [9] 
        unsigned int    Reserved_705          : 2   ; // [11..10] 
        unsigned int    cb_os_clip_fullrange  : 1   ; // [12] 
        unsigned int    cr_os_clip_fullrange  : 1   ; // [13] 
        unsigned int    Reserved_704          : 2   ; // [15..14] 
        unsigned int    cb_gain_polar         : 1   ; // [16] 
        unsigned int    Reserved_703          : 15  ; // [31..17] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF59;

// Define the union U_DATE_COEFF60
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    line_960h_en          : 1   ; // [0] 
        unsigned int    Reserved_708          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF60;

// Define the union U_DATE_COEFF61
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    burst_start_ovrd      : 1   ; // [0] 
        unsigned int    burst_dura_ovrd       : 1   ; // [1] 
        unsigned int    cb_bound_ovrd         : 1   ; // [2] 
        unsigned int    pal_half_ovrd         : 1   ; // [3] 
        unsigned int    rgb_acrive_ovrd       : 1   ; // [4] 
        unsigned int    Reserved_709          : 27  ; // [31..5] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF61;

// Define the union U_DATE_COEFF62
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int burst_dura_coeff        : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DATE_COEFF62;
// Define the union U_DATE_COEFF63
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int cb_bound_coeff          : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DATE_COEFF63;
// Define the union U_DATE_COEFF64
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int pal_half_coeff          : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DATE_COEFF64;
// Define the union U_DATE_COEFF65
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int rgb_active_coeff        : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DATE_COEFF65;
// Define the union U_DATE_COEFF66
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    htotal_ovrd           : 1   ; // [0] 
        unsigned int    wid_sync_ovrd         : 1   ; // [1] 
        unsigned int    wid_hfb_ovrd          : 1   ; // [2] 
        unsigned int    wid_act_ovrd          : 1   ; // [3] 
        unsigned int    Reserved_710          : 28  ; // [31..4] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF66;

// Define the union U_DATE_COEFF67
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    htotal_sw             : 12  ; // [11..0] 
        unsigned int    Reserved_711          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF67;

// Define the union U_DATE_COEFF68
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wid_sync_sw           : 12  ; // [11..0] 
        unsigned int    Reserved_712          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF68;

// Define the union U_DATE_COEFF69
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wid_hfb_sw            : 12  ; // [11..0] 
        unsigned int    Reserved_713          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF69;

// Define the union U_DATE_COEFF70
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wid_act_sw            : 12  ; // [11..0] 
        unsigned int    Reserved_714          : 20  ; // [31..12] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF70;

// Define the union U_DATE_COEFF71
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    phase_shift_ovrd      : 1   ; // [0] 
        unsigned int    Reserved_715          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF71;

// Define the union U_DATE_COEFF72
typedef union
{
    // Define the struct bits 
    struct
    {
        unsigned int phase_shift_coeff       : 32  ; // [31..0] 
    } bits;
 
    // Define an unsigned member
        unsigned int    u32;
 
} U_DATE_COEFF72;
// Define the union U_DATE_COEFF73
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_notch_ovrd       : 1   ; // [0] 
        unsigned int    Reserved_716          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF73;

// Define the union U_DATE_COEFF74
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_notch_1          : 10  ; // [9..0] 
        unsigned int    Reserved_718          : 6   ; // [15..10] 
        unsigned int    coef_notch_2          : 10  ; // [25..16] 
        unsigned int    Reserved_717          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF74;

// Define the union U_DATE_COEFF75
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_notch_3          : 10  ; // [9..0] 
        unsigned int    Reserved_720          : 6   ; // [15..10] 
        unsigned int    coef_notch_4          : 10  ; // [25..16] 
        unsigned int    Reserved_719          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF75;

// Define the union U_DATE_COEFF76
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_notch_5          : 10  ; // [9..0] 
        unsigned int    Reserved_722          : 6   ; // [15..10] 
        unsigned int    coef_notch_6          : 10  ; // [25..16] 
        unsigned int    Reserved_721          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF76;

// Define the union U_DATE_COEFF77
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_notch_7          : 10  ; // [9..0] 
        unsigned int    Reserved_724          : 6   ; // [15..10] 
        unsigned int    coef_notch_8          : 10  ; // [25..16] 
        unsigned int    Reserved_723          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF77;

// Define the union U_DATE_COEFF78
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_notch_9          : 10  ; // [9..0] 
        unsigned int    Reserved_726          : 6   ; // [15..10] 
        unsigned int    coef_notch_10         : 10  ; // [25..16] 
        unsigned int    Reserved_725          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF78;

// Define the union U_DATE_COEFF79
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_notch_11         : 10  ; // [9..0] 
        unsigned int    Reserved_728          : 6   ; // [15..10] 
        unsigned int    coef_notch_12         : 10  ; // [25..16] 
        unsigned int    Reserved_727          : 6   ; // [31..26] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF79;

// Define the union U_DATE_COEFF80
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_chra_lpf_ovrd    : 1   ; // [0] 
        unsigned int    Reserved_729          : 31  ; // [31..1] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF80;

// Define the union U_DATE_COEFF81
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_chra_lpf_1       : 9   ; // [8..0] 
        unsigned int    Reserved_731          : 7   ; // [15..9] 
        unsigned int    coef_chra_lpf_2       : 9   ; // [24..16] 
        unsigned int    Reserved_730          : 7   ; // [31..25] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF81;

// Define the union U_DATE_COEFF82
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_chra_lpf_3       : 9   ; // [8..0] 
        unsigned int    Reserved_733          : 7   ; // [15..9] 
        unsigned int    coef_chra_lpf_4       : 9   ; // [24..16] 
        unsigned int    Reserved_732          : 7   ; // [31..25] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF82;

// Define the union U_DATE_COEFF83
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_chra_lpf_5       : 9   ; // [8..0] 
        unsigned int    Reserved_735          : 7   ; // [15..9] 
        unsigned int    coef_chra_lpf_6       : 9   ; // [24..16] 
        unsigned int    Reserved_734          : 7   ; // [31..25] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF83;

// Define the union U_DATE_COEFF84
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    coef_chra_lpf_7       : 9   ; // [8..0] 
        unsigned int    Reserved_736          : 23  ; // [31..9] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DATE_COEFF84;

//==============================================================================
// Define the global struct
typedef struct
{
    U_VOCTRL             VOCTRL;
    U_VOINTSTA           VOINTSTA;
    U_VOMSKINTSTA        VOMSKINTSTA;
    U_VOINTMSK           VOINTMSK;
    U_VDPVERSION1        VDPVERSION1;
    U_VDPVERSION2        VDPVERSION2;
    unsigned int         Reserved_0[2];
    U_VODEBUG            VODEBUG;
    unsigned int         Reserved_1[4];
    U_VOAXICTRL          VOAXICTRL;
    U_VOWBCARB0          VOWBCARB0;
    U_VOWBCARB1          VOWBCARB1;
    U_VOUFSTA            VOUFSTA;
    U_VOUFCLR            VOUFCLR;
    unsigned int         Reserved_2[2];
    U_VOINTPROC_TIM      VOINTPROC_TIM;
    unsigned int         Reserved_3[43];
    U_VO_MUX             VO_MUX;
    U_VO_MUX_DAC         VO_MUX_DAC;
    U_VO_MUX_TESTSYNC    VO_MUX_TESTSYNC;
    U_VO_MUX_TESTDATA    VO_MUX_TESTDATA;
    unsigned int         Reserved_4[4];
    U_VO_DAC_CTRL        VO_DAC_CTRL;
    U_VO_CABLE_CTRL      VO_CABLE_CTRL;
    unsigned int         Reserved_5[2];
    U_VO_DAC_C_CTRL      VO_DAC_C_CTRL;
    U_VO_DAC_R_CTRL      VO_DAC_R_CTRL;
    U_VO_DAC_G_CTRL      VO_DAC_G_CTRL;
    U_VO_DAC_B_CTRL      VO_DAC_B_CTRL;
    unsigned int         Reserved_6[112];
    U_WBC_DHD_LOCATE     WBC_DHD_LOCATE;
    U_WBC_OFL_EN         WBC_OFL_EN;
    unsigned int         Reserved_7[62];
    U_COEF_DATA          COEF_DATA;
    unsigned int         Reserved_8[3];
    U_V0_PARARD          V0_PARARD;
    U_V1_PARARD          V1_PARARD;
    unsigned int         Reserved_9;
    U_V3_PARARD          V3_PARARD;
    unsigned int         Reserved_10[8];
    U_VP0_PARARD         VP0_PARARD;
    unsigned int         Reserved_11[19];
    U_GP0_PARARD         GP0_PARARD;
    U_GP1_PARARD         GP1_PARARD;
    unsigned int         Reserved_12[10];
    U_WBCDHD_PARARD      WBCDHD_PARARD;
    unsigned int         Reserved_13[11];
    U_DHD0_PARARD        DHD0_PARARD;
    U_DHD1_PARARD        DHD1_PARARD;
    unsigned int         Reserved_14[194];
    U_V0_CTRL            V0_CTRL;
    U_V0_UPD             V0_UPD;
    unsigned int         Reserved_15[6];
    U_V0_PRERD           V0_PRERD;
    unsigned int         Reserved_16;
    U_V0_IRESO           V0_IRESO;
    U_V0_ORESO           V0_ORESO;
    unsigned int         Reserved_17[2];
    U_V0_CBMPARA         V0_CBMPARA;
    U_V0_SR_ORESO        V0_SR_ORESO;
    U_V0_PARAUP          V0_PARAUP;
    U_V0_CPOS            V0_CPOS;
    U_V0_DRAWMODE        V0_DRAWMODE;
    unsigned int         Reserved_18;
    U_V0_HLCOEFAD        V0_HLCOEFAD;
    U_V0_HCCOEFAD        V0_HCCOEFAD;
    U_V0_VLCOEFAD        V0_VLCOEFAD;
    U_V0_VCCOEFAD        V0_VCCOEFAD;
    U_V0_DFPOS           V0_DFPOS;
    U_V0_DLPOS           V0_DLPOS;
    U_V0_VFPOS           V0_VFPOS;
    U_V0_VLPOS           V0_VLPOS;
    U_V0_BK              V0_BK;
    U_V0_ALPHA           V0_ALPHA;
    unsigned int         Reserved_19[2];
    U_V0_CSC_IDC         V0_CSC_IDC;
    U_V0_CSC_ODC         V0_CSC_ODC;
    U_V0_CSC_IODC        V0_CSC_IODC;
    U_V0_CSC_P0          V0_CSC_P0;
    U_V0_CSC_P1          V0_CSC_P1;
    U_V0_CSC_P2          V0_CSC_P2;
    U_V0_CSC_P3          V0_CSC_P3;
    U_V0_CSC_P4          V0_CSC_P4;
    U_SR_HLCOEFAD        SR_HLCOEFAD;
    U_SR_HCCOEFAD        SR_HCCOEFAD;
    U_SR_VLCOEFAD        SR_VLCOEFAD;
    U_SR_VCCOEFAD        SR_VCCOEFAD;
    unsigned int         Reserved_20[4];
    U_V0_HSP             V0_HSP;
    U_V0_HLOFFSET        V0_HLOFFSET;
    U_V0_HCOFFSET        V0_HCOFFSET;
    unsigned int         Reserved_21[3];
    U_V0_VSP             V0_VSP;
    U_V0_VSR             V0_VSR;
    U_V0_VOFFSET         V0_VOFFSET;
    U_V0_VBOFFSET        V0_VBOFFSET;
    unsigned int         Reserved_22[38];
    U_V0_IFIRCOEF01      V0_IFIRCOEF01;
    U_V0_IFIRCOEF23      V0_IFIRCOEF23;
    U_V0_IFIRCOEF45      V0_IFIRCOEF45;
    U_V0_IFIRCOEF67      V0_IFIRCOEF67;
    unsigned int         Reserved_23[28];
    U_V0_P0RESO          V0_P0RESO;
    U_V0_P0LADDR         V0_P0LADDR;
    U_V0_P0CADDR         V0_P0CADDR;
    U_V0_P0STRIDE        V0_P0STRIDE;
    U_V0_P0VFPOS         V0_P0VFPOS;
    U_V0_P0VLPOS         V0_P0VLPOS;
    unsigned int         Reserved_24[2];
    U_V0_P1RESO          V0_P1RESO;
    U_V0_P1LADDR         V0_P1LADDR;
    U_V0_P1CADDR         V0_P1CADDR;
    U_V0_P1STRIDE        V0_P1STRIDE;
    U_V0_P1VFPOS         V0_P1VFPOS;
    U_V0_P1VLPOS         V0_P1VLPOS;
    unsigned int         Reserved_25[2];
    U_V0_P2RESO          V0_P2RESO;
    U_V0_P2LADDR         V0_P2LADDR;
    U_V0_P2CADDR         V0_P2CADDR;
    U_V0_P2STRIDE        V0_P2STRIDE;
    U_V0_P2VFPOS         V0_P2VFPOS;
    U_V0_P2VLPOS         V0_P2VLPOS;
    unsigned int         Reserved_26[2];
    U_V0_P3RESO          V0_P3RESO;
    U_V0_P3LADDR         V0_P3LADDR;
    U_V0_P3CADDR         V0_P3CADDR;
    U_V0_P3STRIDE        V0_P3STRIDE;
    U_V0_P3VFPOS         V0_P3VFPOS;
    U_V0_P3VLPOS         V0_P3VLPOS;
    unsigned int         Reserved_27[2];
    U_V0_P4RESO          V0_P4RESO;
    U_V0_P4LADDR         V0_P4LADDR;
    U_V0_P4CADDR         V0_P4CADDR;
    U_V0_P4STRIDE        V0_P4STRIDE;
    U_V0_P4VFPOS         V0_P4VFPOS;
    U_V0_P4VLPOS         V0_P4VLPOS;
    unsigned int         Reserved_28[2];
    U_V0_P5RESO          V0_P5RESO;
    U_V0_P5LADDR         V0_P5LADDR;
    U_V0_P5CADDR         V0_P5CADDR;
    U_V0_P5STRIDE        V0_P5STRIDE;
    U_V0_P5VFPOS         V0_P5VFPOS;
    U_V0_P5VLPOS         V0_P5VLPOS;
    unsigned int         Reserved_29[2];
    U_V0_P6RESO          V0_P6RESO;
    U_V0_P6LADDR         V0_P6LADDR;
    U_V0_P6CADDR         V0_P6CADDR;
    U_V0_P6STRIDE        V0_P6STRIDE;
    U_V0_P6VFPOS         V0_P6VFPOS;
    U_V0_P6VLPOS         V0_P6VLPOS;
    unsigned int         Reserved_30[2];
    U_V0_P7RESO          V0_P7RESO;
    U_V0_P7LADDR         V0_P7LADDR;
    U_V0_P7CADDR         V0_P7CADDR;
    U_V0_P7STRIDE        V0_P7STRIDE;
    U_V0_P7VFPOS         V0_P7VFPOS;
    U_V0_P7VLPOS         V0_P7VLPOS;
    unsigned int         Reserved_31[2];
    U_V0_P8RESO          V0_P8RESO;
    U_V0_P8LADDR         V0_P8LADDR;
    U_V0_P8CADDR         V0_P8CADDR;
    U_V0_P8STRIDE        V0_P8STRIDE;
    U_V0_P8VFPOS         V0_P8VFPOS;
    U_V0_P8VLPOS         V0_P8VLPOS;
    unsigned int         Reserved_32[2];
    U_V0_P9RESO          V0_P9RESO;
    U_V0_P9LADDR         V0_P9LADDR;
    U_V0_P9CADDR         V0_P9CADDR;
    U_V0_P9STRIDE        V0_P9STRIDE;
    U_V0_P9VFPOS         V0_P9VFPOS;
    U_V0_P9VLPOS         V0_P9VLPOS;
    unsigned int         Reserved_33[2];
    U_V0_P10RESO         V0_P10RESO;
    U_V0_P10LADDR        V0_P10LADDR;
    U_V0_P10CADDR        V0_P10CADDR;
    U_V0_P10STRIDE       V0_P10STRIDE;
    U_V0_P10VFPOS        V0_P10VFPOS;
    U_V0_P10VLPOS        V0_P10VLPOS;
    unsigned int         Reserved_34[2];
    U_V0_P11RESO         V0_P11RESO;
    U_V0_P11LADDR        V0_P11LADDR;
    U_V0_P11CADDR        V0_P11CADDR;
    U_V0_P11STRIDE       V0_P11STRIDE;
    U_V0_P11VFPOS        V0_P11VFPOS;
    U_V0_P11VLPOS        V0_P11VLPOS;
    unsigned int         Reserved_35[2];
    U_V0_P12RESO         V0_P12RESO;
    U_V0_P12LADDR        V0_P12LADDR;
    U_V0_P12CADDR        V0_P12CADDR;
    U_V0_P12STRIDE       V0_P12STRIDE;
    U_V0_P12VFPOS        V0_P12VFPOS;
    U_V0_P12VLPOS        V0_P12VLPOS;
    unsigned int         Reserved_36[2];
    U_V0_P13RESO         V0_P13RESO;
    U_V0_P13LADDR        V0_P13LADDR;
    U_V0_P13CADDR        V0_P13CADDR;
    U_V0_P13STRIDE       V0_P13STRIDE;
    U_V0_P13VFPOS        V0_P13VFPOS;
    U_V0_P13VLPOS        V0_P13VLPOS;
    unsigned int         Reserved_37[2];
    U_V0_P14RESO         V0_P14RESO;
    U_V0_P14LADDR        V0_P14LADDR;
    U_V0_P14CADDR        V0_P14CADDR;
    U_V0_P14STRIDE       V0_P14STRIDE;
    U_V0_P14VFPOS        V0_P14VFPOS;
    U_V0_P14VLPOS        V0_P14VLPOS;
    unsigned int         Reserved_38[2];
    U_V0_P15RESO         V0_P15RESO;
    U_V0_P15LADDR        V0_P15LADDR;
    U_V0_P15CADDR        V0_P15CADDR;
    U_V0_P15STRIDE       V0_P15STRIDE;
    U_V0_P15VFPOS        V0_P15VFPOS;
    U_V0_P15VLPOS        V0_P15VLPOS;
    unsigned int         Reserved_39[66];
    U_SR_HCOEF0          SR_HCOEF0;
    U_SR_HCOEF1          SR_HCOEF1;
    U_SR_HCOEF2          SR_HCOEF2;
    U_SR_HCOEF3          SR_HCOEF3;
    U_SR_HCOEF4          SR_HCOEF4;
    U_SR_HCOEF5          SR_HCOEF5;
    U_SR_HCOEF6          SR_HCOEF6;
    U_SR_HCOEF7          SR_HCOEF7;
    U_SR_HCOEF8          SR_HCOEF8;
    U_SR_HCOEF9          SR_HCOEF9;
    U_SR_HCOEF10         SR_HCOEF10;
    U_SR_VCOEF0          SR_VCOEF0;
    U_SR_VCOEF1          SR_VCOEF1;
    U_SR_VCOEF2          SR_VCOEF2;
    U_SR_VCOEF3          SR_VCOEF3;
    U_SR_VCOEF4          SR_VCOEF4;
    U_SR_VCOEF5          SR_VCOEF5;
    U_SR_VCOEF6          SR_VCOEF6;
    U_SR_VCOEF7          SR_VCOEF7;
    unsigned int         Reserved_40[13];
    U_SR_CTRL            SR_CTRL;
    U_SR_LDIR_RATIO      SR_LDIR_RATIO;
    U_SR_LTEX_THR        SR_LTEX_THR;
    U_SR_LTEX_ADD        SR_LTEX_ADD;
    U_SR_DIPL_DETAIL     SR_DIPL_DETAIL;
    U_SR_SHARP_EN        SR_SHARP_EN;
    U_SR_SHARP_THR       SR_SHARP_THR;
    U_SR_SMOOTH          SR_SMOOTH;
    unsigned int         Reserved_41[8];
    U_SR_HSP             SR_HSP;
    U_SR_HLOFFSET        SR_HLOFFSET;
    U_SR_HCOFFSET        SR_HCOFFSET;
    unsigned int         Reserved_42[3];
    U_SR_VSP             SR_VSP;
    U_SR_VSR             SR_VSR;
    U_SR_VOFFSET         SR_VOFFSET;
    U_SR_VBOFFSET        SR_VBOFFSET;
    unsigned int         Reserved_43[6];
    U_V0_NADDR           V0_NADDR;
    U_V0_NCADDR          V0_NCADDR;
    unsigned int         Reserved_44[10];
    U_V0_MULTI_MODE      V0_MULTI_MODE;
    unsigned int         Reserved_45[3];
    U_V0_LADDROFFSET     V0_LADDROFFSET;
    U_V0_CADDROFFSET     V0_CADDROFFSET;
    unsigned int         Reserved_46[46];
    U_V0_16REGIONENL     V0_16REGIONENL;
    unsigned int         Reserved_47;
    U_V0_16MUTE          V0_16MUTE;
    unsigned int         Reserved_48[5];
    U_V0_DCMP_LSTATE0    V0_DCMP_LSTATE0;
    U_V0_DCMP_LSTATE1    V0_DCMP_LSTATE1;
    U_V0_DCMP_CSTATE0    V0_DCMP_CSTATE0;
    U_V0_DCMP_CSTATE1    V0_DCMP_CSTATE1;
    U_VO_DCMPERRCLR      VO_DCMPERRCLR;
    U_V0_DCMP_ERR        V0_DCMP_ERR;
    unsigned int         Reserved_49[3122];
    U_VP0_CTRL           VP0_CTRL;
    U_VP0_UPD            VP0_UPD;
    unsigned int         Reserved_50[2];
    U_VP0_DCI_CAD        VP0_DCI_CAD;
    U_VP0_ACM_CAD        VP0_ACM_CAD;
    U_VP0_PARAUP         VP0_PARAUP;
    unsigned int         Reserved_51;
    U_VP0_IRESO          VP0_IRESO;
    unsigned int         Reserved_52[31];
    U_VP0_DOF_CTRL       VP0_DOF_CTRL;
    U_VP0_DOF_STEP       VP0_DOF_STEP;
    unsigned int         Reserved_53[22];
    U_VP0_DCICTRL        VP0_DCICTRL;
    U_VP0_DCIHPOS        VP0_DCIHPOS;
    U_VP0_DCIVPOS        VP0_DCIVPOS;
    U_VP0_DCIHISBLD      VP0_DCIHISBLD;
    U_VP0_DCIHISOFT      VP0_DCIHISOFT;
    U_VP0_DCIHISCOR      VP0_DCIHISCOR;
    U_VP0_DCIMERBLD      VP0_DCIMERBLD;
    U_VP0_DCIADJWGT      VP0_DCIADJWGT;
    U_VP0_DCICLIP0       VP0_DCICLIP0;
    U_VP0_DCICLIP1       VP0_DCICLIP1;
    U_VP0_DCICLIP2       VP0_DCICLIP2;
    U_VP0_DCIGLBGAIN     VP0_DCIGLBGAIN;
    U_VP0_DCIPOSTHR0     VP0_DCIPOSTHR0;
    U_VP0_DCIPOSTHR1     VP0_DCIPOSTHR1;
    U_VP0_DCIPOSGAIN0    VP0_DCIPOSGAIN0;
    U_VP0_DCIPOSGAIN1    VP0_DCIPOSGAIN1;
    U_VP0_DCIPOSSLP0     VP0_DCIPOSSLP0;
    U_VP0_DCIPOSSLP1     VP0_DCIPOSSLP1;
    U_VP0_DCIPOSSLP2     VP0_DCIPOSSLP2;
    U_VP0_DCINEGTHR0     VP0_DCINEGTHR0;
    U_VP0_DCINEGTHR1     VP0_DCINEGTHR1;
    U_VP0_DCINEGGAIN0    VP0_DCINEGGAIN0;
    U_VP0_DCINEGGAIN1    VP0_DCINEGGAIN1;
    U_VP0_DCINEGSLP0     VP0_DCINEGSLP0;
    U_VP0_DCINEGSLP1     VP0_DCINEGSLP1;
    U_VP0_DCINEGSLP2     VP0_DCINEGSLP2;
    U_VP0_DCIADJSW0      VP0_DCIADJSW0;
    U_VP0_DCIADJSW1      VP0_DCIADJSW1;
    U_VP0_DCIADJSW2      VP0_DCIADJSW2;
    U_VP0_DCIADJSW3      VP0_DCIADJSW3;
    U_VP0_DCIADJSW4      VP0_DCIADJSW4;
    U_VP0_DCIADJSW5      VP0_DCIADJSW5;
    U_VP0_DCIADJSW6      VP0_DCIADJSW6;
    U_VP0_DCIADJSW7      VP0_DCIADJSW7;
    U_VP0_DCIADJSW8      VP0_DCIADJSW8;
    U_VP0_DCIADJSW9      VP0_DCIADJSW9;
    U_VP0_DCIADJSW10     VP0_DCIADJSW10;
    U_VP0_DCIWGTRD       VP0_DCIWGTRD;
    unsigned int         Reserved_54[2];
    U_VP0_ACM_CTRL       VP0_ACM_CTRL;
    U_VP0_ACM_ADJ        VP0_ACM_ADJ;
    unsigned int         Reserved_55[22];
    U_VP0_DFPOS          VP0_DFPOS;
    U_VP0_DLPOS          VP0_DLPOS;
    U_VP0_VFPOS          VP0_VFPOS;
    U_VP0_VLPOS          VP0_VLPOS;
    U_VP0_BK             VP0_BK;
    U_VP0_ALPHA          VP0_ALPHA;
    unsigned int         Reserved_56[58];
    U_VP0_CSC0_IDC       VP0_CSC0_IDC;
    U_VP0_CSC0_ODC       VP0_CSC0_ODC;
    U_VP0_CSC0_IODC      VP0_CSC0_IODC;
    U_VP0_CSC0_P0        VP0_CSC0_P0;
    U_VP0_CSC0_P1        VP0_CSC0_P1;
    U_VP0_CSC0_P2        VP0_CSC0_P2;
    U_VP0_CSC0_P3        VP0_CSC0_P3;
    U_VP0_CSC0_P4        VP0_CSC0_P4;
    U_VP0_CSC1_IDC       VP0_CSC1_IDC;
    U_VP0_CSC1_ODC       VP0_CSC1_ODC;
    U_VP0_CSC1_IODC      VP0_CSC1_IODC;
    U_VP0_CSC1_P0        VP0_CSC1_P0;
    U_VP0_CSC1_P1        VP0_CSC1_P1;
    U_VP0_CSC1_P2        VP0_CSC1_P2;
    U_VP0_CSC1_P3        VP0_CSC1_P3;
    U_VP0_CSC1_P4        VP0_CSC1_P4;
    unsigned int         Reserved_57[1840];
    U_G0_CTRL            G0_CTRL;
    U_G0_UPD             G0_UPD;
    unsigned int         Reserved_58[2];
    U_G0_ADDR            G0_ADDR;
    unsigned int         Reserved_59;
    U_G0_NADDR           G0_NADDR;
    U_G0_STRIDE          G0_STRIDE;
    U_G0_IRESO           G0_IRESO;
    U_G0_SFPOS           G0_SFPOS;
    unsigned int         Reserved_60[2];
    U_G0_CBMPARA         G0_CBMPARA;
    U_G0_CKEYMAX         G0_CKEYMAX;
    U_G0_CKEYMIN         G0_CKEYMIN;
    U_G0_CMASK           G0_CMASK;
    U_G0_PARAADDR        G0_PARAADDR;
    U_G0_PARAUP          G0_PARAUP;
    unsigned int         Reserved_61[2];
    U_G0_DCMP_ADDR       G0_DCMP_ADDR;
    U_G0_DCMP_NADDR      G0_DCMP_NADDR;
    U_G0_DCMP_OFFSET     G0_DCMP_OFFSET;
    unsigned int         Reserved_62[9];
    U_G0_DFPOS           G0_DFPOS;
    U_G0_DLPOS           G0_DLPOS;
    U_G0_VFPOS           G0_VFPOS;
    U_G0_VLPOS           G0_VLPOS;
    U_G0_BK              G0_BK;
    U_G0_ALPHA           G0_ALPHA;
    unsigned int         Reserved_63[2];
    U_G0_DOF_CTRL        G0_DOF_CTRL;
    U_G0_DOF_STEP        G0_DOF_STEP;
    unsigned int         Reserved_64[3030];
    U_GP0_CTRL           GP0_CTRL;
    U_GP0_UPD            GP0_UPD;
    U_GP0_ORESO          GP0_ORESO;
    U_GP0_IRESO          GP0_IRESO;
    U_GP0_HCOEFAD        GP0_HCOEFAD;
    U_GP0_VCOEFAD        GP0_VCOEFAD;
    U_GP0_PARAUP         GP0_PARAUP;
    unsigned int         Reserved_65;
    U_GP0_GALPHA         GP0_GALPHA;
    unsigned int         Reserved_66[55];
    U_GP0_DFPOS          GP0_DFPOS;
    U_GP0_DLPOS          GP0_DLPOS;
    U_GP0_VFPOS          GP0_VFPOS;
    U_GP0_VLPOS          GP0_VLPOS;
    U_GP0_BK             GP0_BK;
    U_GP0_ALPHA          GP0_ALPHA;
    unsigned int         Reserved_67[2];
    U_GP0_CSC_IDC        GP0_CSC_IDC;
    U_GP0_CSC_ODC        GP0_CSC_ODC;
    U_GP0_CSC_IODC       GP0_CSC_IODC;
    U_GP0_CSC_P0         GP0_CSC_P0;
    U_GP0_CSC_P1         GP0_CSC_P1;
    U_GP0_CSC_P2         GP0_CSC_P2;
    U_GP0_CSC_P3         GP0_CSC_P3;
    U_GP0_CSC_P4         GP0_CSC_P4;
    U_GP0_ZME_HSP        GP0_ZME_HSP;
    U_GP0_ZME_HLOFFSET   GP0_ZME_HLOFFSET;
    U_GP0_ZME_HCOFFSET   GP0_ZME_HCOFFSET;
    U_GP0_ZME_VSP        GP0_ZME_VSP;
    U_GP0_ZME_VSR        GP0_ZME_VSR;
    U_GP0_ZME_VOFFSET    GP0_ZME_VOFFSET;
    unsigned int         Reserved_68[2];
    U_GP0_ZME_LTICTRL    GP0_ZME_LTICTRL;
    U_GP0_ZME_LHPASSCOEF GP0_ZME_LHPASSCOEF;
    U_GP0_ZME_LTITHD     GP0_ZME_LTITHD;
    unsigned int         Reserved_69;
    U_GP0_ZME_LHFREQTHD  GP0_ZME_LHFREQTHD;
    U_GP0_ZME_LGAINCOEF  GP0_ZME_LGAINCOEF;
    unsigned int         Reserved_70[2];
    U_GP0_ZME_CTICTRL    GP0_ZME_CTICTRL;
    U_GP0_ZME_CHPASSCOEF GP0_ZME_CHPASSCOEF;
    U_GP0_ZME_CTITHD     GP0_ZME_CTITHD;
    unsigned int         Reserved_71[925];
    U_WBC_G0_CTRL        WBC_G0_CTRL;
    U_WBC_G0_UPD         WBC_G0_UPD;
    U_WBC_G0_CMP         WBC_G0_CMP;
    unsigned int         Reserved_72;
    U_WBC_G0_AR_ADDR     WBC_G0_AR_ADDR;
    U_WBC_G0_GB_ADDR     WBC_G0_GB_ADDR;
    U_WBC_G0_STRIDE      WBC_G0_STRIDE;
    U_WBC_G0_OFFSET      WBC_G0_OFFSET;
    U_WBC_G0_ORESO       WBC_G0_ORESO;
    U_WBC_G0_FCROP       WBC_G0_FCROP;
    U_WBC_G0_LCROP       WBC_G0_LCROP;
    unsigned int         Reserved_73[501];
    U_WBC_GP0_CTRL       WBC_GP0_CTRL;
    U_WBC_GP0_UPD        WBC_GP0_UPD;
    unsigned int         Reserved_74[2];
    U_WBC_GP0_YADDR      WBC_GP0_YADDR;
    U_WBC_GP0_CADDR      WBC_GP0_CADDR;
    U_WBC_GP0_STRIDE     WBC_GP0_STRIDE;
    unsigned int         Reserved_75;
    U_WBC_GP0_ORESO      WBC_GP0_ORESO;
    U_WBC_GP0_FCROP      WBC_GP0_FCROP;
    U_WBC_GP0_LCROP      WBC_GP0_LCROP;
    unsigned int         Reserved_76[53];
    U_WBC_GP0_DITHER_CTRL WBC_GP0_DITHER_CTRL;
    U_WBC_GP0_DITHER_COEF0 WBC_GP0_DITHER_COEF0;
    U_WBC_GP0_DITHER_COEF1 WBC_GP0_DITHER_COEF1;
    unsigned int         Reserved_77[189];
    U_WBC_DHD0_CTRL      WBC_DHD0_CTRL;
    U_WBC_DHD0_UPD       WBC_DHD0_UPD;
    unsigned int         Reserved_78[2];
    U_WBC_DHD0_YADDR     WBC_DHD0_YADDR;
    U_WBC_DHD0_CADDR     WBC_DHD0_CADDR;
    U_WBC_DHD0_STRIDE    WBC_DHD0_STRIDE;
    unsigned int         Reserved_79;
    U_WBC_DHD0_ORESO     WBC_DHD0_ORESO;
    U_WBC_DHD0_FCROP     WBC_DHD0_FCROP;
    U_WBC_DHD0_LCROP     WBC_DHD0_LCROP;
    unsigned int         Reserved_80;
    U_WBC_DHD0_LOWDLYCTRL WBC_DHD0_LOWDLYCTRL;
    U_WBC_DHD0_TUNLADDR  WBC_DHD0_TUNLADDR;
    U_WBC_DHD0_LOWDLYSTA WBC_DHD0_LOWDLYSTA;
    unsigned int         Reserved_81;
    U_WBC_DHD0_PARAUP    WBC_DHD0_PARAUP;
    unsigned int         Reserved_82[3];
    U_WBC_DHD0_HLCOEFAD  WBC_DHD0_HLCOEFAD;
    U_WBC_DHD0_HCCOEFAD  WBC_DHD0_HCCOEFAD;
    U_WBC_DHD0_VLCOEFAD  WBC_DHD0_VLCOEFAD;
    U_WBC_DHD0_VCCOEFAD  WBC_DHD0_VCCOEFAD;
    unsigned int         Reserved_83[16];
    U_WBC_DHD0_HISTOGRAM0 WBC_DHD0_HISTOGRAM0;
    U_WBC_DHD0_HISTOGRAM1 WBC_DHD0_HISTOGRAM1;
    U_WBC_DHD0_HISTOGRAM2 WBC_DHD0_HISTOGRAM2;
    U_WBC_DHD0_HISTOGRAM3 WBC_DHD0_HISTOGRAM3;
    U_WBC_DHD0_HISTOGRAM4 WBC_DHD0_HISTOGRAM4;
    U_WBC_DHD0_HISTOGRAM5 WBC_DHD0_HISTOGRAM5;
    U_WBC_DHD0_HISTOGRAM6 WBC_DHD0_HISTOGRAM6;
    U_WBC_DHD0_HISTOGRAM7 WBC_DHD0_HISTOGRAM7;
    U_WBC_DHD0_HISTOGRAM8 WBC_DHD0_HISTOGRAM8;
    U_WBC_DHD0_HISTOGRAM9 WBC_DHD0_HISTOGRAM9;
    U_WBC_DHD0_HISTOGRAM10 WBC_DHD0_HISTOGRAM10;
    U_WBC_DHD0_HISTOGRAM11 WBC_DHD0_HISTOGRAM11;
    U_WBC_DHD0_HISTOGRAM12 WBC_DHD0_HISTOGRAM12;
    U_WBC_DHD0_HISTOGRAM13 WBC_DHD0_HISTOGRAM13;
    U_WBC_DHD0_HISTOGRAM14 WBC_DHD0_HISTOGRAM14;
    U_WBC_DHD0_HISTOGRAM15 WBC_DHD0_HISTOGRAM15;
    unsigned int         Reserved_84[4];
    U_WBC_DHD0_CHECKSUM_Y WBC_DHD0_CHECKSUM_Y;
    U_WBC_DHD0_CHECKSUM_C WBC_DHD0_CHECKSUM_C;
    unsigned int         Reserved_85[2];
    U_WBC_DHD0_DITHER_CTRL WBC_DHD0_DITHER_CTRL;
    U_WBC_DHD0_DITHER_COEF0 WBC_DHD0_DITHER_COEF0;
    U_WBC_DHD0_DITHER_COEF1 WBC_DHD0_DITHER_COEF1;
    unsigned int         Reserved_86[109];
    U_WBC_DHD0_ZME_HSP   WBC_DHD0_ZME_HSP;
    U_WBC_DHD0_ZME_HLOFFSET WBC_DHD0_ZME_HLOFFSET;
    U_WBC_DHD0_ZME_HCOFFSET WBC_DHD0_ZME_HCOFFSET;
    unsigned int         Reserved_87[3];
    U_WBC_DHD0_ZME_VSP   WBC_DHD0_ZME_VSP;
    U_WBC_DHD0_ZME_VSR   WBC_DHD0_ZME_VSR;
    U_WBC_DHD0_ZME_VOFFSET WBC_DHD0_ZME_VOFFSET;
    U_WBC_DHD0_ZME_VBOFFSET WBC_DHD0_ZME_VBOFFSET;
    unsigned int         Reserved_88[6];
    U_WBC_DHD0_CSCIDC    WBC_DHD0_CSCIDC;
    U_WBC_DHD0_CSCODC    WBC_DHD0_CSCODC;
    U_WBC_DHD0_CSCP0     WBC_DHD0_CSCP0;
    U_WBC_DHD0_CSCP1     WBC_DHD0_CSCP1;
    U_WBC_DHD0_CSCP2     WBC_DHD0_CSCP2;
    U_WBC_DHD0_CSCP3     WBC_DHD0_CSCP3;
    U_WBC_DHD0_CSCP4     WBC_DHD0_CSCP4;
    unsigned int         Reserved_89[57];
    U_MIXV0_BKG          MIXV0_BKG;
    unsigned int         Reserved_90;
    U_MIXV0_MIX          MIXV0_MIX;
    unsigned int         Reserved_91[125];
    U_MIXG0_BKG          MIXG0_BKG;
    U_MIXG0_BKALPHA      MIXG0_BKALPHA;
    U_MIXG0_MIX          MIXG0_MIX;
    unsigned int         Reserved_92[125];
    U_CBM_BKG1           CBM_BKG1;
    unsigned int         Reserved_93;
    U_CBM_MIX1           CBM_MIX1;
    unsigned int         Reserved_94[5];
    U_CBM_BKG2           CBM_BKG2;
    unsigned int         Reserved_95;
    U_CBM_MIX2           CBM_MIX2;
    unsigned int         Reserved_96[5];
    U_CBM_ATTR           CBM_ATTR;
    unsigned int         Reserved_97[751];
    U_DHD0_CTRL          DHD0_CTRL;
    U_DHD0_VSYNC         DHD0_VSYNC;
    U_DHD0_HSYNC1        DHD0_HSYNC1;
    U_DHD0_HSYNC2        DHD0_HSYNC2;
    U_DHD0_VPLUS         DHD0_VPLUS;
    U_DHD0_PWR           DHD0_PWR;
    U_DHD0_VTTHD3        DHD0_VTTHD3;
    U_DHD0_VTTHD         DHD0_VTTHD;
    U_DHD0_SYNC_INV      DHD0_SYNC_INV;
    unsigned int         Reserved_98[2];
    U_DHD0_DATA_SEL      DHD0_DATA_SEL;
    unsigned int         Reserved_99;
    U_DHD0_ABUFTHD       DHD0_ABUFTHD;
    unsigned int         Reserved_100[2];
    U_DHD0_CSC_IDC       DHD0_CSC_IDC;
    U_DHD0_CSC_ODC       DHD0_CSC_ODC;
    U_DHD0_CSC_IODC      DHD0_CSC_IODC;
    U_DHD0_CSC_P0        DHD0_CSC_P0;
    U_DHD0_CSC_P1        DHD0_CSC_P1;
    U_DHD0_CSC_P2        DHD0_CSC_P2;
    U_DHD0_CSC_P3        DHD0_CSC_P3;
    U_DHD0_CSC_P4        DHD0_CSC_P4;
    unsigned int         Reserved_101[10];
    U_DHD0_CLIP1_L       DHD0_CLIP1_L;
    U_DHD0_CLIP1_H       DHD0_CLIP1_H;
    U_DHD0_CLIP2_L       DHD0_CLIP2_L;
    U_DHD0_CLIP2_H       DHD0_CLIP2_H;
    U_DHD0_CLIP3_L       DHD0_CLIP3_L;
    U_DHD0_CLIP3_H       DHD0_CLIP3_H;
    unsigned int         Reserved_102[4];
    U_DHD0_PARATHD       DHD0_PARATHD;
    unsigned int         Reserved_103[3];
    U_DHD0_START_POS     DHD0_START_POS;
    unsigned int         Reserved_104[3];
    U_DHD0_LOCKCFG       DHD0_LOCKCFG;
    U_DHD0_LOCK_STATE1   DHD0_LOCK_STATE1;
    U_DHD0_LOCK_STATE2   DHD0_LOCK_STATE2;
    U_DHD0_LOCK_STATE3   DHD0_LOCK_STATE3;
    U_DHD0_GMM_COEFAD    DHD0_GMM_COEFAD;
    unsigned int         Reserved_105[2];
    U_DHD0_PARAUP        DHD0_PARAUP;
    U_DHD0_STATE         DHD0_STATE;
    unsigned int         Reserved_106;
    U_DHD0_DEBUG         DHD0_DEBUG;
    U_DHD0_DEBUG_STATE   DHD0_DEBUG_STATE;
    unsigned int         Reserved_107[3008];
    U_HDATE_VERSION      HDATE_VERSION;
    U_HDATE_EN           HDATE_EN;
    U_HDATE_POLA_CTRL    HDATE_POLA_CTRL;
    U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
    U_HDATE_STATE        HDATE_STATE;
    U_HDATE_OUT_CTRL     HDATE_OUT_CTRL;
    U_HDATE_SRC_13_COEF1 HDATE_SRC_13_COEF1;
    U_HDATE_SRC_13_COEF2 HDATE_SRC_13_COEF2;
    U_HDATE_SRC_13_COEF3 HDATE_SRC_13_COEF3;
    U_HDATE_SRC_13_COEF4 HDATE_SRC_13_COEF4;
    U_HDATE_SRC_13_COEF5 HDATE_SRC_13_COEF5;
    U_HDATE_SRC_13_COEF6 HDATE_SRC_13_COEF6;
    U_HDATE_SRC_13_COEF7 HDATE_SRC_13_COEF7;
    U_HDATE_SRC_13_COEF8 HDATE_SRC_13_COEF8;
    U_HDATE_SRC_13_COEF9 HDATE_SRC_13_COEF9;
    U_HDATE_SRC_13_COEF10 HDATE_SRC_13_COEF10;
    U_HDATE_SRC_13_COEF11 HDATE_SRC_13_COEF11;
    U_HDATE_SRC_13_COEF12 HDATE_SRC_13_COEF12;
    U_HDATE_SRC_13_COEF13 HDATE_SRC_13_COEF13;
    U_HDATE_SRC_24_COEF1 HDATE_SRC_24_COEF1;
    U_HDATE_SRC_24_COEF2 HDATE_SRC_24_COEF2;
    U_HDATE_SRC_24_COEF3 HDATE_SRC_24_COEF3;
    U_HDATE_SRC_24_COEF4 HDATE_SRC_24_COEF4;
    U_HDATE_SRC_24_COEF5 HDATE_SRC_24_COEF5;
    U_HDATE_SRC_24_COEF6 HDATE_SRC_24_COEF6;
    U_HDATE_SRC_24_COEF7 HDATE_SRC_24_COEF7;
    U_HDATE_SRC_24_COEF8 HDATE_SRC_24_COEF8;
    U_HDATE_SRC_24_COEF9 HDATE_SRC_24_COEF9;
    U_HDATE_SRC_24_COEF10 HDATE_SRC_24_COEF10;
    U_HDATE_SRC_24_COEF11 HDATE_SRC_24_COEF11;
    U_HDATE_SRC_24_COEF12 HDATE_SRC_24_COEF12;
    U_HDATE_SRC_24_COEF13 HDATE_SRC_24_COEF13;
    U_HDATE_CSC_COEF1    HDATE_CSC_COEF1;
    U_HDATE_CSC_COEF2    HDATE_CSC_COEF2;
    U_HDATE_CSC_COEF3    HDATE_CSC_COEF3;
    U_HDATE_CSC_COEF4    HDATE_CSC_COEF4;
    U_HDATE_CSC_COEF5    HDATE_CSC_COEF5;
    unsigned int         Reserved_108[3];
    U_HDATE_TEST         HDATE_TEST;
    U_HDATE_VBI_CTRL     HDATE_VBI_CTRL;
    U_HDATE_CGMSA_DATA   HDATE_CGMSA_DATA;
    U_HDATE_CGMSB_H      HDATE_CGMSB_H;
    U_HDATE_CGMSB_DATA1  HDATE_CGMSB_DATA1;
    U_HDATE_CGMSB_DATA2  HDATE_CGMSB_DATA2;
    U_HDATE_CGMSB_DATA3  HDATE_CGMSB_DATA3;
    U_HDATE_CGMSB_DATA4  HDATE_CGMSB_DATA4;
    U_HDATE_DACDET1      HDATE_DACDET1;
    U_HDATE_DACDET2      HDATE_DACDET2;
    U_HDATE_SRC_13_COEF14 HDATE_SRC_13_COEF14;
    U_HDATE_SRC_13_COEF15 HDATE_SRC_13_COEF15;
    U_HDATE_SRC_13_COEF16 HDATE_SRC_13_COEF16;
    U_HDATE_SRC_13_COEF17 HDATE_SRC_13_COEF17;
    U_HDATE_SRC_13_COEF18 HDATE_SRC_13_COEF18;
    U_HDATE_SRC_24_COEF14 HDATE_SRC_24_COEF14;
    U_HDATE_SRC_24_COEF15 HDATE_SRC_24_COEF15;
    U_HDATE_SRC_24_COEF16 HDATE_SRC_24_COEF16;
    U_HDATE_SRC_24_COEF17 HDATE_SRC_24_COEF17;
    U_HDATE_SRC_24_COEF18 HDATE_SRC_24_COEF18;
    U_HDATE_CLIP         HDATE_CLIP;
    unsigned int         Reserved_109[67];
    U_DATE_COEFF0        DATE_COEFF0;
    U_DATE_COEFF1        DATE_COEFF1;
    U_DATE_COEFF2        DATE_COEFF2;
    U_DATE_COEFF3        DATE_COEFF3;
    U_DATE_COEFF4        DATE_COEFF4;
    U_DATE_COEFF5        DATE_COEFF5;
    U_DATE_COEFF6        DATE_COEFF6;
    U_DATE_COEFF7        DATE_COEFF7;
    U_DATE_COEFF8        DATE_COEFF8;
    U_DATE_COEFF9        DATE_COEFF9;
    U_DATE_COEFF10       DATE_COEFF10;
    U_DATE_COEFF11       DATE_COEFF11;
    U_DATE_COEFF12       DATE_COEFF12;
    U_DATE_COEFF13       DATE_COEFF13;
    U_DATE_COEFF14       DATE_COEFF14;
    U_DATE_COEFF15       DATE_COEFF15;
    U_DATE_COEFF16       DATE_COEFF16;
    U_DATE_COEFF17       DATE_COEFF17;
    U_DATE_COEFF18       DATE_COEFF18;
    U_DATE_COEFF19       DATE_COEFF19;
    U_DATE_COEFF20       DATE_COEFF20;
    U_DATE_COEFF21       DATE_COEFF21;
    U_DATE_COEFF22       DATE_COEFF22;
    U_DATE_COEFF23       DATE_COEFF23;
    U_DATE_COEFF24       DATE_COEFF24;
    U_DATE_COEFF25       DATE_COEFF25;
    U_DATE_COEFF26       DATE_COEFF26;
    U_DATE_COEFF27       DATE_COEFF27;
    U_DATE_COEFF28       DATE_COEFF28;
    U_DATE_COEFF29       DATE_COEFF29;
    U_DATE_COEFF30       DATE_COEFF30;
    unsigned int         Reserved_110;
    U_DATE_ISRMASK       DATE_ISRMASK;
    U_DATE_ISRSTATE      DATE_ISRSTATE;
    U_DATE_ISR           DATE_ISR;
    unsigned int         Reserved_111;
    U_DATE_VERSION       DATE_VERSION;
    U_DATE_COEFF37       DATE_COEFF37;
    U_DATE_COEFF38       DATE_COEFF38;
    U_DATE_COEFF39       DATE_COEFF39;
    U_DATE_COEFF40       DATE_COEFF40;
    U_DATE_COEFF41       DATE_COEFF41;
    U_DATE_COEFF42       DATE_COEFF42;
    unsigned int         Reserved_112[5];
    U_DATE_DACDET1       DATE_DACDET1;
    U_DATE_DACDET2       DATE_DACDET2;
    U_DATE_COEFF50       DATE_COEFF50;
    U_DATE_COEFF51       DATE_COEFF51;
    U_DATE_COEFF52       DATE_COEFF52;
    U_DATE_COEFF53       DATE_COEFF53;
    U_DATE_COEFF54       DATE_COEFF54;
    U_DATE_COEFF55       DATE_COEFF55;
    U_DATE_COEFF56       DATE_COEFF56;
    U_DATE_COEFF57       DATE_COEFF57;
    U_DATE_COEFF58       DATE_COEFF58;
    U_DATE_COEFF59       DATE_COEFF59;
    U_DATE_COEFF60       DATE_COEFF60;
    U_DATE_COEFF61       DATE_COEFF61;
    U_DATE_COEFF62       DATE_COEFF62;
    U_DATE_COEFF63       DATE_COEFF63;
    U_DATE_COEFF64       DATE_COEFF64;
    U_DATE_COEFF65       DATE_COEFF65;
    U_DATE_COEFF66       DATE_COEFF66;
    U_DATE_COEFF67       DATE_COEFF67;
    U_DATE_COEFF68       DATE_COEFF68;
    U_DATE_COEFF69       DATE_COEFF69;
    U_DATE_COEFF70       DATE_COEFF70;
    U_DATE_COEFF71       DATE_COEFF71;
    U_DATE_COEFF72       DATE_COEFF72;
    U_DATE_COEFF73       DATE_COEFF73;
    U_DATE_COEFF74       DATE_COEFF74;
    U_DATE_COEFF75       DATE_COEFF75;
    U_DATE_COEFF76       DATE_COEFF76;
    U_DATE_COEFF77       DATE_COEFF77;
    U_DATE_COEFF78       DATE_COEFF78;
    U_DATE_COEFF79       DATE_COEFF79;
    U_DATE_COEFF80       DATE_COEFF80;
    U_DATE_COEFF81       DATE_COEFF81;
    U_DATE_COEFF82       DATE_COEFF82;
    U_DATE_COEFF83       DATE_COEFF83;
    U_DATE_COEFF84       DATE_COEFF84;


} S_VDP_REGS_TYPE,S_VOU_CV200_REGS_TYPE;

// Declare the struct pointor of the module VDP
extern S_VDP_REGS_TYPE *gopVDPAllReg;



#endif // __C_UNION_DEFINE_H__
