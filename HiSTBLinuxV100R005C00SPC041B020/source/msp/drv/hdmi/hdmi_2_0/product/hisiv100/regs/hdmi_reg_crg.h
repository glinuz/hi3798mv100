//****************************************************************************** 
// Copyright     :  Copyright (C) 2016, Hisilicon Technologies Co., Ltd.
// File name     :  hi_reg_crg.h
// Author        :  xxx
// Version       :  1.0 
// Date          :  2016-01-20
// Description   :  Define all registers/tables for S40V200
// Others        :  Generated automatically by nManager V4.0 
// History       :  xxx 2016-01-20 Create file
//******************************************************************************

#ifndef __HDMI_REG_CRG_H__
#define __HDMI_REG_CRG_H__

/* Define the union R_PERI_CRG_PLL0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cpR_pll_cfg0_apb      : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL0;

/* Define the union R_PERI_CRG_PLL1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cpR_pll_cfg1_apb      : 27  ; /* [26..0]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL1;

/* Define the union R_PERI_CRG_PLL2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpll_ctrl0            : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL2;

/* Define the union R_PERI_CRG_PLL3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpll_ctrl1            : 27  ; /* [26..0]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL3;

/* Define the union R_PERI_CRG_PLL4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddr_pll_cfg0_apb      : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL4;

/* Define the union R_PERI_CRG_PLL5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddr_pll_cfg1_apb      : 27  ; /* [26..0]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL5;

/* Define the union R_PERI_CRG_PLL8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpll_ctrl0            : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL8;

/* Define the union R_PERI_CRG_PLL9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpll_ctrl1            : 27  ; /* [26..0]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL9;

/* Define the union R_PERI_CRG_PLL10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpll_frac             : 24  ; /* [23..0]  */
        unsigned int    hpll_postdiv1         : 3   ; /* [26..24]  */
        unsigned int    hpll_rsv              : 1   ; /* [27]  */
        unsigned int    hpll_postdiv2         : 3   ; /* [30..28]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL10;

/* Define the union R_PERI_CRG_PLL11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpll_fbdiv             : 12  ; /* [11..0]  */
        unsigned int    hpll_refdiv            : 6   ; /* [17..12]  */
        unsigned int    hpll_rsv               : 2   ; /* [19..18]  */
        unsigned int    hpll_pd                : 1   ; /* [20]  */
        unsigned int    hpll_foutvcopd         : 1   ; /* [21]  */
        unsigned int    hpll_fout4phasepdPLL   : 1   ; /* [22]  */
        unsigned int    hpll_foutpostdivpd     : 1   ; /* [23]  */
        unsigned int    hpll_dacpd             : 1   ; /* [24]  */
        unsigned int    hpll_dsmpd             : 1   ; /* [25]  */
        unsigned int    hpll_bypass            : 1   ; /* [26]  */
        unsigned int    reserved_0             : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL11;

/* Define the union R_PERI_CRG_PLL12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    epll_ctrl0            : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL12;

/* Define the union R_PERI_CRG_PLL13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    epll_ctrl1            : 27  ; /* [26..0]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL13;

/* Define the union R_PERI_CRG_PLL14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpll_ctrl0            : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL14;

/* Define the union R_PERI_CRG_PLL15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpll_ctrl1            : 27  ; /* [26..0]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL15;

/* Define the union R_PERI_CRG18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cpR_freq_sel_cfg_crg  : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 6   ; /* [8..3]  */
        unsigned int    cpR_begin_cfg_bypass  : 1   ; /* [9]  */
        unsigned int    cpR_sw_begin_cfg      : 1   ; /* [10]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    cpR_clk_pctrl         : 1   ; /* [12]  */
        unsigned int    bypass_ffs            : 1   ; /* [13]  */
        unsigned int    ffs_srst_req          : 1   ; /* [14]  */
        unsigned int    cpm_srst_req          : 1   ; /* [15]  */
        unsigned int    reserved_2            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG18;

/* Define the union R_PERI_CRG19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    timeout_plllock       : 12  ; /* [11..0]  */
        unsigned int    lock_bypass           : 1   ; /* [12]  */
        unsigned int    pll_cfg_bypass        : 1   ; /* [13]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG19;

/* Define the union R_PERI_CRG20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 7   ; /* [6..0]  */
        unsigned int    arm_l2_srst_req       : 1   ; /* [7]  */
        unsigned int    arm_srst_req          : 4   ; /* [11..8]  */
        unsigned int    arm_por_srst_req      : 4   ; /* [15..12]  */
        unsigned int    a53_pclkdbg_cken      : 1   ; /* [16]  */
        unsigned int    reserved_1            : 3   ; /* [19..17]  */
        unsigned int    cluster_cold_srst_req : 1   ; /* [20]  */
        unsigned int    cs_srst_req           : 1   ; /* [21]  */
        unsigned int    clk_div_peri          : 3   ; /* [24..22]  */
        unsigned int    reserved_2            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG20;

/* Define the union R_PERI_CRG22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    core_bus_clk_sel      : 2   ; /* [1..0]  */
        unsigned int    mde0_clk_sel          : 2   ; /* [3..2]  */
        unsigned int    mde1_clk_sel          : 2   ; /* [5..4]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    mde2_clk_sel          : 2   ; /* [9..8]  */
        unsigned int    mde3_clk_sel          : 2   ; /* [11..10]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG22;

/* Define the union R_PERI_CRG25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    hipack0_cken          : 1   ; /* [1]  */
        unsigned int    reserved_1            : 1   ; /* [2]  */
        unsigned int    ddrc_cken             : 1   ; /* [3]  */
        unsigned int    reserved_2            : 4   ; /* [7..4]  */
        unsigned int    hipack0_srst_req      : 1   ; /* [8]  */
        unsigned int    reserved_3            : 1   ; /* [9]  */
        unsigned int    ddrc_srst_req         : 1   ; /* [10]  */
        unsigned int    ddrt_srst_req         : 1   ; /* [11]  */
        unsigned int    ddrt_cken             : 1   ; /* [12]  */
        unsigned int    cfg_clk_cken          : 1   ; /* [13]  */
        unsigned int    aclk_ddr_cken         : 1   ; /* [14]  */
        unsigned int    reserved_4            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG25;

/* Define the union R_PERI_CRG26 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    uart2_cken            : 1   ; /* [4]  */
        unsigned int    uart2_srst_req        : 1   ; /* [5]  */
        unsigned int    uart3_cken            : 1   ; /* [6]  */
        unsigned int    uart3_srst_req        : 1   ; /* [7]  */
        unsigned int    reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG26;

/* Define the union R_PERI_CRG27 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    i2c0_cken             : 1   ; /* [4]  */
        unsigned int    i2c0_srst_req         : 1   ; /* [5]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    i2c1_cken             : 1   ; /* [8]  */
        unsigned int    i2c1_srst_req         : 1   ; /* [9]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    i2c2_cken             : 1   ; /* [12]  */
        unsigned int    i2c2_srst_req         : 1   ; /* [13]  */
        unsigned int    reserved_3            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG27;

/* Define the union R_PERI_CRG28 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ssp0_cken             : 1   ; /* [0]  */
        unsigned int    ssp0_srst_req         : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG28;

/* Define the union R_PERI_CRG29 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sci0_cken             : 1   ; /* [0]  */
        unsigned int    sci0_srst_req         : 1   ; /* [1]  */
        unsigned int    sci1_cken             : 1   ; /* [2]  */
        unsigned int    sci1_srst_req         : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG29;

/* Define the union R_PERI_CRG30 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vdh_cken              : 1   ; /* [0]  */
        unsigned int    vdhdsp_cken           : 1   ; /* [1]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    vdh_all_srst_req      : 1   ; /* [4]  */
        unsigned int    vdh_scd_srst_req      : 1   ; /* [5]  */
        unsigned int    vdh_mfd_srst_req      : 1   ; /* [6]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    vdh_clk_sel           : 2   ; /* [9..8]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    vdhclk_skipcfg        : 5   ; /* [16..12]  */
        unsigned int    vdhclk_loaden         : 1   ; /* [17]  */
        unsigned int    reserved_3            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG30;

/* Define the union R_PERI_CRG31 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    jpgd_cken             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    jpgd_srst_req         : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    jpgd_clk_sel          : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG31;

/* Define the union R_PERI_CRG33 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pgd_cken              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    pgd_srst_req          : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG33;

/* Define the union R_PERI_CRG34 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpd_cken              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    bpd_srst_req          : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG34;

/* Define the union R_PERI_CRG35 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    venc_core_cken        : 1   ; /* [0]  */
        unsigned int    venc_axi_cken         : 1   ; /* [1]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    venc_srst_req         : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    venc_clk_sel          : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG35;

/* Define the union R_PERI_CRG36 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    jpge_cken             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    jpge_srst_req         : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG36;

/* Define the union R_PERI_CRG37 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tde_cken              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    tde_srst_req          : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG37;

/* Define the union R_PERI_CRG39 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sdio0_bus_cken        : 1   ; /* [0]  */
        unsigned int    sdio0_cken            : 1   ; /* [1]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    sdio0_srst_req        : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    sdio0_clk_sel         : 2   ; /* [9..8]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    sdio0_sap_ps_sel      : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 1   ; /* [15]  */
        unsigned int    sdio0_drv_ps_sel      : 3   ; /* [18..16]  */
        unsigned int    sdio0_clk_mode        : 1   ; /* [19]  */
        unsigned int    reserved_4            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG39;

/* Define the union R_PERI_CRG40 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    emmc_bus_cken         : 1   ; /* [0]  */
        unsigned int    emmc_cken             : 1   ; /* [1]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    emmc_srst_req         : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    emmc_clk_sel          : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    emmc_sap_ps_sel       : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 1   ; /* [15]  */
        unsigned int    emmc_drv_ps_sel       : 3   ; /* [18..16]  */
        unsigned int    emmc_clk_mode         : 1   ; /* [19]  */
        unsigned int    reserved_4            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG40;

/* Define the union R_PERI_CRG42 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sata_bus_cken         : 1   ; /* [0]  */
        unsigned int    sata_rx_cken          : 1   ; /* [1]  */
        unsigned int    sata_cko_alive_cken   : 1   ; /* [2]  */
        unsigned int    sata_tx_cken          : 1   ; /* [3]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    sata_bus_srst_req     : 1   ; /* [8]  */
        unsigned int    sata_cko_alive_srst_req : 1   ; /* [9]  */
        unsigned int    sata_rx_srst_req      : 1   ; /* [10]  */
        unsigned int    sata_srst_req         : 1   ; /* [11]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG42;

/* Define the union R_PERI_CRG44 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    usb3_bus_cken         : 1   ; /* [0]  */
        unsigned int    usb3_ref_cken         : 1   ; /* [1]  */
        unsigned int    usb3_suspend_cken     : 1   ; /* [2]  */
        unsigned int    usb3_pipe_cken        : 1   ; /* [3]  */
        unsigned int    usb3_utmi_cken        : 1   ; /* [4]  */
        unsigned int    usb3_bus_gs_cken      : 1   ; /* [5]  */
        unsigned int    usb3_bus_gm_cken      : 1   ; /* [6]  */
        unsigned int    reserved_0            : 5   ; /* [11..7]  */
        unsigned int    usb3_vcc_srst_req     : 1   ; /* [12]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG44;

/* Define the union R_PERI_CRG46 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    usb2_bus_cken         : 1   ; /* [0]  */
        unsigned int    usb2_ohci48m_cken     : 1   ; /* [1]  */
        unsigned int    usb2_ohci12m_cken     : 1   ; /* [2]  */
        unsigned int    usb2_otg_utmi_cken    : 1   ; /* [3]  */
        unsigned int    usb2_hst_phy_cken     : 1   ; /* [4]  */
        unsigned int    usb2_utmi0_cken       : 1   ; /* [5]  */
        unsigned int    usb2_utmi1_cken       : 1   ; /* [6]  */
        unsigned int    reserved_0            : 5   ; /* [11..7]  */
        unsigned int    usb2_bus_srst_req     : 1   ; /* [12]  */
        unsigned int    usb2_utmi0_srst_req   : 1   ; /* [13]  */
        unsigned int    usb2_utmi1_srst_req   : 1   ; /* [14]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    usb2_hst_phy_srst_req : 1   ; /* [16]  */
        unsigned int    usb2_otg_phy_srst_req : 1   ; /* [17]  */
        unsigned int    reserved_2            : 1   ; /* [18]  */
        unsigned int    usb2_phy01_clk_sel    : 1   ; /* [19]  */
        unsigned int    reserved_3            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG46;

/* Define the union R_PERI_CRG47 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    usb2_phy01_ref_cken   : 1   ; /* [0]  */
        unsigned int    reserved_0            : 1   ; /* [1]  */
        unsigned int    usb2_phy2_ref_cken    : 1   ; /* [2]  */
        unsigned int    reserved_1            : 1   ; /* [3]  */
        unsigned int    usb2_phy01_srst_req   : 1   ; /* [4]  */
        unsigned int    reserved_2            : 1   ; /* [5]  */
        unsigned int    usb2_phy2_srst_req    : 1   ; /* [6]  */
        unsigned int    reserved_3            : 1   ; /* [7]  */
        unsigned int    usb2_phy0_srst_treq   : 1   ; /* [8]  */
        unsigned int    usb2_phy1_srst_treq   : 1   ; /* [9]  */
        unsigned int    usb2_phy2_srst_treq   : 1   ; /* [10]  */
        unsigned int    reserved_4            : 1   ; /* [11]  */
        unsigned int    usb2_phy01_refclk_sel : 1   ; /* [12]  */
        unsigned int    reserved_5            : 1   ; /* [13]  */
        unsigned int    usb2_phy2_refclk_sel  : 1   ; /* [14]  */
        unsigned int    usb2_phy01_test_srst_req : 1   ; /* [15]  */
        unsigned int    usb2_phy2_test_srst_req : 1   ; /* [16]  */
        unsigned int    reserved_6            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG47;

/* Define the union R_PERI_CRG48 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 9   ; /* [8..0]  */
        unsigned int    ca_ci_srst_req        : 1   ; /* [9]  */
        unsigned int    otp_srst_req          : 1   ; /* [10]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    ca_ci_clk_sel         : 1   ; /* [12]  */
        unsigned int    reserved_2            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG48;

/* Define the union R_PERI_CRG49 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sha0_cken             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    sha0_srst_req         : 1   ; /* [4]  */
        unsigned int    sha1_cken             : 1   ; /* [5]  */
        unsigned int    sha1_srst_req         : 1   ; /* [6]  */
        unsigned int    reserved_1            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG49;

/* Define the union R_PERI_CRG50 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmc_cken              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    pmc_srst_req          : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG50;

/* Define the union R_PERI_CRG51 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gsf_srst_req          : 1   ; /* [0]  */
        unsigned int    gsf_cken              : 1   ; /* [1]  */
        unsigned int    gmac_if_srst_req      : 1   ; /* [2]  */
        unsigned int    gmac_if_cken          : 1   ; /* [3]  */
        unsigned int    reserved_0            : 1   ; /* [4]  */
        unsigned int    ext_netphy_srst_req   : 1   ; /* [5]  */
        unsigned int    ext_netphy_cksel      : 1   ; /* [6]  */
        unsigned int    gsf_cksel             : 1   ; /* [7]  */
        unsigned int    reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG51;

/* Define the union R_PERI_CRG52 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    eth_bus_cken          : 1   ; /* [0]  */
        unsigned int    eth_cken              : 1   ; /* [1]  */
        unsigned int    eth_clk_sel           : 1   ; /* [2]  */
        unsigned int    hrst_eth_s            : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG52;

/* Define the union R_PERI_CRG53 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gpR_cken              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    gpR_srst_req          : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG53;

/* Define the union R_PERI_CRG54 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vo_bus_cken           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 1   ; /* [1]  */
        unsigned int    vo_sd_cken            : 1   ; /* [2]  */
        unsigned int    vo_sdate_cken         : 1   ; /* [3]  */
        unsigned int    vo_hd_cken            : 1   ; /* [4]  */
        unsigned int    reserved_1            : 1   ; /* [5]  */
        unsigned int    vdac_ch0_cken         : 1   ; /* [6]  */
        unsigned int    reserved_2            : 5   ; /* [11..7]  */
        unsigned int    vo_sd_clk_sel         : 1   ; /* [12]  */
        unsigned int    reserved_3            : 1   ; /* [13]  */
        unsigned int    vo_sd_clk_div         : 2   ; /* [15..14]  */
        unsigned int    vo_hd_clk_sel         : 1   ; /* [16]  */
        unsigned int    reserved_4            : 1   ; /* [17]  */
        unsigned int    vo_hd_clk_div         : 2   ; /* [19..18]  */
        unsigned int    vdac_ch0_clk_sel      : 1   ; /* [20]  */
        unsigned int    reserved_5            : 4   ; /* [24..21]  */
        unsigned int    hdmi_clk_sel          : 1   ; /* [25]  */
        unsigned int    vo_sd_hdmi_clk_sel    : 1   ; /* [26]  */
        unsigned int    vdp_clk_sel           : 1   ; /* [27]  */
        unsigned int    reserved_6            : 1   ; /* [28]  */
        unsigned int    vo_hd_hdmi_clk_sel    : 1   ; /* [29]  */
        unsigned int    voR_srst_req          : 1   ; /* [30]  */
        unsigned int    vdp_cfg_cken          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG54;

/* Define the union R_PERI_CRG60 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpss_cken             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    vpss_srst_req         : 1   ; /* [4]  */
        unsigned int    reserved_1            : 7   ; /* [11..5]  */
        unsigned int    vpssclk_skipcfg       : 5   ; /* [16..12]  */
        unsigned int    vpssclk_loaden        : 1   ; /* [17]  */
        unsigned int    reserved_2            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG60;

/* Define the union R_PERI_CRG63 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pvr_bus_cken          : 1   ; /* [0]  */
        unsigned int    pvr_dmx_cken          : 1   ; /* [1]  */
        unsigned int    pvr_27m_cken          : 1   ; /* [2]  */
        unsigned int    pvr_tsi1_cken         : 1   ; /* [3]  */
        unsigned int    pvr_tsi2_cken         : 1   ; /* [4]  */
        unsigned int    pvr_tsi3_cken         : 1   ; /* [5]  */
        unsigned int    pvr_tsi4_cken         : 1   ; /* [6]  */
        unsigned int    reserved_0            : 3   ; /* [9..7]  */
        unsigned int    pvr_ts0_cken          : 1   ; /* [10]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    pvr_tsout0_cken       : 1   ; /* [12]  */
        unsigned int    reserved_2            : 2   ; /* [14..13]  */
        unsigned int    pvr_tsi1_pctrl        : 1   ; /* [15]  */
        unsigned int    pvr_tsi2_pctrl        : 1   ; /* [16]  */
        unsigned int    pvr_tsi3_pctrl        : 1   ; /* [17]  */
        unsigned int    pvr_tsi4_pctrl        : 1   ; /* [18]  */
        unsigned int    reserved_3            : 5   ; /* [23..19]  */
        unsigned int    pvr_srst_req          : 1   ; /* [24]  */
        unsigned int    reserved_4            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG63;

/* Define the union R_PERI_CRG64 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pvr_tsout0_pctrl      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 1   ; /* [1]  */
        unsigned int    pvr_dmx_clk_sel       : 1   ; /* [2]  */
        unsigned int    reserved_1            : 1   ; /* [3]  */
        unsigned int    sw_dmx_clk_div        : 5   ; /* [8..4]  */
        unsigned int    sw_dmxclk_loaden      : 1   ; /* [9]  */
        unsigned int    pvr_dmx_clkdiv_cfg    : 1   ; /* [10]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    pvr_ts0_clk_sel       : 2   ; /* [13..12]  */
        unsigned int    reserved_3            : 2   ; /* [15..14]  */
        unsigned int    pvr_ts0_clk_div       : 4   ; /* [19..16]  */
        unsigned int    reserved_4            : 4   ; /* [23..20]  */
        unsigned int    pvr_tsi_sel           : 1   ; /* [24]  */
        unsigned int    reserved_5            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG64;

/* Define the union R_PERI_CRG67 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ssc_in_cken           : 1   ; /* [0]  */
        unsigned int    ssc_bypass_cken       : 1   ; /* [1]  */
        unsigned int    hdmitx_ctrl_osc_24m_cken : 1   ; /* [2]  */
        unsigned int    hdmitx_ctrl_cec_cken  : 1   ; /* [3]  */
        unsigned int    hdmitx_ctrl_os_cken   : 1   ; /* [4]  */
        unsigned int    hdmitx_ctrl_as_cken   : 1   ; /* [5]  */
        unsigned int    hdmitx_ctrl_bus_srst_req : 1   ; /* [6]  */
        unsigned int    hdmitx_ctrl_srst_req  : 1   ; /* [7]  */
        unsigned int    hdmitx_ctrl_cec_srst_req : 1   ; /* [8]  */
        unsigned int    hdmitx_ssc_srst_req   : 1   ; /* [9]  */
        unsigned int    ssc_clk_div           : 4   ; /* [13..10]  */
        unsigned int    hdmitx_ctrl_cec_clk_sel : 1   ; /* [14]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG67;

/* Define the union R_PERI_CRG68 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy_tmds_cken         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    hdmitx_phy_srst_req   : 1   ; /* [4]  */
        unsigned int    phy_tmds_srst_req     : 1   ; /* [5]  */
        unsigned int    tmds_clk_div          : 3   ; /* [8..6]  */
        unsigned int    reserved_1            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG68;

/* Define the union R_PERI_CRG69 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adac_cken             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    adac_srst_req         : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG69;

/* Define the union R_PERI_CRG70 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aiao_cken             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    aiao_srst_req         : 1   ; /* [4]  */
        unsigned int    reserved_1            : 7   ; /* [11..5]  */
        unsigned int    aiaoclk_skipcfg       : 5   ; /* [16..12]  */
        unsigned int    aiaoclk_loaden        : 1   ; /* [17]  */
        unsigned int    reserved_2            : 3   ; /* [20..18]  */
        unsigned int    aiao_mclk_sel         : 1   ; /* [21]  */
        unsigned int    aiao_clk_sel          : 1   ; /* [22]  */
        unsigned int    reserved_3            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG70;

/* Define the union R_PERI_CRG71 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vdac_chop_cken        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 15  ; /* [15..1]  */
        unsigned int    vdac_0_clk_pctrl      : 1   ; /* [16]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG71;

/* Define the union R_PERI_CRG73 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gpR_freq_sel_cfg_crg  : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 6   ; /* [8..3]  */
        unsigned int    gpR_begin_cfg_bypass  : 1   ; /* [9]  */
        unsigned int    gpR_sw_begin_cfg      : 1   ; /* [10]  */
        unsigned int    reserved_1            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG73;

/* Define the union R_PERI_CRG74 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddr_freq_sel_cfg_crg  : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 6   ; /* [8..3]  */
        unsigned int    ddr_begin_cfg_bypass  : 1   ; /* [9]  */
        unsigned int    ddr_sw_begin_cfg      : 1   ; /* [10]  */
        unsigned int    reserved_1            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG74;

/* Define the union R_PERI_CRG75 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    apll_ssmod_ctrl       : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG75;

/* Define the union R_PERI_CRG76 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpll_ssmod_ctrl       : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG76;

/* Define the union R_PERI_CRG77 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dpll_ssmod_ctrl       : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG77;

/* Define the union R_PERI_CRG79 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpll_ssmod_ctrl       : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG79;

/* Define the union R_PERI_CRG80 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpll_ssmod_ctrl       : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG80;

/* Define the union R_PERI_CRG81 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    epll_ssmod_ctrl       : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG81;

/* Define the union R_PERI_CRG82 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpll_ssmod_ctrl       : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG82;

/* Define the union R_PERI_CRG84 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pll_lock              : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG84;

/* Define the union R_PERI_CRG85 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cpR_clk_sw_ok_crg     : 1   ; /* [0]  */
        unsigned int    cpR_clk_mux           : 3   ; /* [3..1]  */
        unsigned int    gpR_clk_sw_ok_crg     : 1   ; /* [4]  */
        unsigned int    gpR_clk_mux           : 3   ; /* [7..5]  */
        unsigned int    ddr_clk_sw_ok_crg     : 1   ; /* [8]  */
        unsigned int    ddr_clk_mux           : 3   ; /* [11..9]  */
        unsigned int    core_bus_clk_seled    : 2   ; /* [13..12]  */
        unsigned int    reserved_0            : 6   ; /* [19..14]  */
        unsigned int    mde0_clk_seled        : 2   ; /* [21..20]  */
        unsigned int    mde1_clk_seled        : 2   ; /* [23..22]  */
        unsigned int    mde2_clk_seled        : 2   ; /* [25..24]  */
        unsigned int    mde3_clk_seled        : 2   ; /* [27..26]  */
        unsigned int    reserved_1            : 1   ; /* [28]  */
        unsigned int    vdh_clk_seled         : 2   ; /* [30..29]  */
        unsigned int    reserved_2            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG85;

/* Define the union R_PERI_CRG_PLL86 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cpR_pll_cfg0_sw       : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL86;

/* Define the union R_PERI_CRG_PLL87 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cpR_pll_cfg1_sw       : 27  ; /* [26..0]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL87;

/* Define the union R_PERI_CRG_PLL88 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddr_pll_cfg0_sw       : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL88;

/* Define the union R_PERI_CRG_PLL89 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddr_pll_cfg1_sw       : 27  ; /* [26..0]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG_PLL89;

/* Define the union R_PERI_CRG90 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    slic_rst0_out         : 1   ; /* [0]  */
        unsigned int    fe_rstn_out           : 1   ; /* [1]  */
        unsigned int    dem_rst0_out          : 1   ; /* [2]  */
        unsigned int    mute_ctrl_out         : 1   ; /* [3]  */
        unsigned int    slic_rst1_out         : 1   ; /* [4]  */
        unsigned int    dem_rst1_out          : 1   ; /* [5]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG90;

/* Define the union R_PERI_CRG93 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vdh_all_rst_ok        : 1   ; /* [0]  */
        unsigned int    vdh_scd_rst_ok        : 1   ; /* [1]  */
        unsigned int    vdh_mfd_rst_ok        : 1   ; /* [2]  */
        unsigned int    vdh_bpd_rst_ok        : 1   ; /* [3]  */
        unsigned int    gzip_rst_ok           : 1   ; /* [4]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG93;

/* Define the union R_PERI_CRG94 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wdg0_cken             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    wdg0_srst_req         : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG94;

/* Define the union R_PERI_CRG95 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pll_test_en           : 1   ; /* [0]  */
        unsigned int    test_clk_sel          : 1   ; /* [1]  */
        unsigned int    test_clk_en           : 1   ; /* [2]  */
        unsigned int    hdmitx_phy_test_clk_en : 1   ; /* [3]  */
        unsigned int    fephy_test_clk_en     : 1   ; /* [4]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG95;

/* Define the union R_PERI_CRG96 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mem_adjust_cpu        : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG96;

/* Define the union R_PERI_CRG97 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mem_adjust_gpu        : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG97;

/* Define the union R_PERI_CRG98 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    combphy_ref_cken      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 2   ; /* [2..1]  */
        unsigned int    combphy_refclk_sel    : 1   ; /* [3]  */
        unsigned int    combphy_srst_req      : 1   ; /* [4]  */
        unsigned int    combphy_rstn_sel_ctrl : 1   ; /* [5]  */
        unsigned int    reserved_1            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG98;

/* Define the union R_PERI_CRG99 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pcie_bus_cken         : 1   ; /* [0]  */
        unsigned int    pcie_sys_cken         : 1   ; /* [1]  */
        unsigned int    pcie_pipe_cken        : 1   ; /* [2]  */
        unsigned int    pcie_aux_cken         : 1   ; /* [3]  */
        unsigned int    pcie_bus_srst_req     : 1   ; /* [4]  */
        unsigned int    pcie_sys_srst_req     : 1   ; /* [5]  */
        unsigned int    pcie_srst_req         : 1   ; /* [6]  */
        unsigned int    pcie_refclk_sel       : 1   ; /* [7]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG99;

/* Define the union R_PERI_CRG105 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    apll_tune_int_cfg     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG105;

/* Define the union R_PERI_CRG106 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    apll_tune_frac_cfg    : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG106;

/* Define the union R_PERI_CRG107 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    apll_tune_step_int    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG107;

/* Define the union R_PERI_CRG108 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    apll_tune_step_frac   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG108;

/* Define the union R_PERI_CRG109 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    apll_tune_cken        : 1   ; /* [0]  */
        unsigned int    apll_tune_srst_req    : 1   ; /* [1]  */
        unsigned int    apll_tune_mode        : 1   ; /* [2]  */
        unsigned int    apll_tune_en          : 1   ; /* [3]  */
        unsigned int    apll_tune_divval      : 4   ; /* [7..4]  */
        unsigned int    apll_tune_soft_cfg    : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG109;

/* Define the union R_PERI_CRG110 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpll_tune_int_cfg     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG110;

/* Define the union R_PERI_CRG111 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpll_tune_frac_cfg    : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG111;

/* Define the union R_PERI_CRG112 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpll_tune_step_int    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG112;

/* Define the union R_PERI_CRG113 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpll_tune_step_frac   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG113;

/* Define the union R_PERI_CRG114 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpll_tune_cken        : 1   ; /* [0]  */
        unsigned int    bpll_tune_srst_req    : 1   ; /* [1]  */
        unsigned int    bpll_tune_mode        : 1   ; /* [2]  */
        unsigned int    bpll_tune_en          : 1   ; /* [3]  */
        unsigned int    bpll_tune_divval      : 4   ; /* [7..4]  */
        unsigned int    bpll_tune_soft_cfg    : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG114;

/* Define the union R_PERI_CRG115 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dpll_tune_int_cfg     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG115;

/* Define the union R_PERI_CRG116 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dpll_tune_frac_cfg    : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG116;

/* Define the union R_PERI_CRG117 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dpll_tune_step_int    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG117;

/* Define the union R_PERI_CRG118 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dpll_tune_step_frac   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG118;

/* Define the union R_PERI_CRG119 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dpll_tune_cken        : 1   ; /* [0]  */
        unsigned int    dpll_tune_srst_req    : 1   ; /* [1]  */
        unsigned int    dpll_tune_mode        : 1   ; /* [2]  */
        unsigned int    dpll_tune_en          : 1   ; /* [3]  */
        unsigned int    dpll_tune_divval      : 4   ; /* [7..4]  */
        unsigned int    dpll_tune_soft_cfg    : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG119;

/* Define the union R_PERI_CRG120 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    epll_tune_int_cfg     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG120;

/* Define the union R_PERI_CRG121 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    epll_tune_frac_cfg    : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG121;

/* Define the union R_PERI_CRG122 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    epll_tune_step_int    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG122;

/* Define the union R_PERI_CRG123 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    epll_tune_step_frac   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG123;

/* Define the union R_PERI_CRG124 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    epll_tune_cken        : 1   ; /* [0]  */
        unsigned int    epll_tune_srst_req    : 1   ; /* [1]  */
        unsigned int    epll_tune_mode        : 1   ; /* [2]  */
        unsigned int    epll_tune_en          : 1   ; /* [3]  */
        unsigned int    epll_tune_divval      : 4   ; /* [7..4]  */
        unsigned int    epll_tune_soft_cfg    : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG124;

/* Define the union R_PERI_CRG135 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpll_tune_int_cfg     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG135;

/* Define the union R_PERI_CRG136 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpll_tune_frac_cfg    : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG136;

/* Define the union R_PERI_CRG137 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpll_tune_step_int    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG137;

/* Define the union R_PERI_CRG138 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpll_tune_step_frac   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG138;

/* Define the union R_PERI_CRG139 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpll_tune_cken        : 1   ; /* [0]  */
        unsigned int    hpll_tune_srst_req    : 1   ; /* [1]  */
        unsigned int    hpll_tune_mode        : 1   ; /* [2]  */
        unsigned int    hpll_tune_en          : 1   ; /* [3]  */
        unsigned int    hpll_tune_divval      : 4   ; /* [7..4]  */
        unsigned int    hpll_tune_soft_cfg    : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG139;

/* Define the union R_PERI_CRG140 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpll_tune_int_cfg     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG140;

/* Define the union R_PERI_CRG141 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpll_tune_frac_cfg    : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG141;

/* Define the union R_PERI_CRG142 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpll_tune_step_int    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG142;

/* Define the union R_PERI_CRG143 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpll_tune_step_frac   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG143;

/* Define the union R_PERI_CRG144 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpll_tune_cken        : 1   ; /* [0]  */
        unsigned int    vpll_tune_srst_req    : 1   ; /* [1]  */
        unsigned int    vpll_tune_mode        : 1   ; /* [2]  */
        unsigned int    vpll_tune_en          : 1   ; /* [3]  */
        unsigned int    vpll_tune_divval      : 4   ; /* [7..4]  */
        unsigned int    vpll_tune_soft_cfg    : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG144;

/* Define the union R_PERI_CRG159 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mem_adjust_cpR_1      : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG159;

/* Define the union R_PERI_CRG160 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    emmc_dll_mdly_tap_flag : 8   ; /* [7..0]  */
        unsigned int    emmc_dll_locked_flag  : 1   ; /* [8]  */
        unsigned int    emmc_dll_ready_flag   : 1   ; /* [9]  */
        unsigned int    emmc_dll_overflow_flag : 1   ; /* [10]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG160;

/* Define the union R_PERI_CRG161 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 11  ; /* [10..0]  */
        unsigned int    clk_emmc_drv_even_seled : 3   ; /* [13..11]  */
        unsigned int    clk_emmc_sap_even_seled : 3   ; /* [16..14]  */
        unsigned int    clk_sdio1_drv_even_seled : 3   ; /* [19..17]  */
        unsigned int    clk_sdio1_sap_even_seled : 3   ; /* [22..20]  */
        unsigned int    clk_sdio0_drv_even_seled : 3   ; /* [25..23]  */
        unsigned int    clk_sdio0_sap_even_seled : 3   ; /* [28..26]  */
        unsigned int    cpR_clk_seled         : 1   ; /* [29]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG161;

/* Define the union R_PERI_CRG163 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sdio1_bus_cken        : 1   ; /* [0]  */
        unsigned int    sdio1_cken            : 1   ; /* [1]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    sdio1_srst_req        : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    sdio1_clk_sel         : 2   ; /* [9..8]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    sdio1_sap_ps_sel      : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 1   ; /* [15]  */
        unsigned int    sdio1_drv_ps_sel      : 3   ; /* [18..16]  */
        unsigned int    sdio1_clk_mode        : 1   ; /* [19]  */
        unsigned int    reserved_4            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG163;

/* Define the union R_PERI_CRG164 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    emmc_dll_cken         : 1   ; /* [0]  */
        unsigned int    emmc_dll_srst_req     : 1   ; /* [1]  */
        unsigned int    emmc_dll_dsel         : 2   ; /* [3..2]  */
        unsigned int    emmc_dll_tune         : 4   ; /* [7..4]  */
        unsigned int    emmc_dll_dllssel      : 8   ; /* [15..8]  */
        unsigned int    emmc_dll_dllmode      : 1   ; /* [16]  */
        unsigned int    emmc_dll_bypass       : 1   ; /* [17]  */
        unsigned int    emmc_dll_stop         : 1   ; /* [18]  */
        unsigned int    emmc_dll_slave_en     : 1   ; /* [19]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG164;

/* Define the union R_PERI_CRG165 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    apll_tune_int         : 12  ; /* [11..0]  */
        unsigned int    apll_tune_sw_ok       : 1   ; /* [12]  */
        unsigned int    apll_tune_busy        : 1   ; /* [13]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG165;

/* Define the union R_PERI_CRG166 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    apll_tune_frac        : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG166;

/* Define the union R_PERI_CRG167 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpll_tune_int         : 12  ; /* [11..0]  */
        unsigned int    bpll_tune_sw_ok       : 1   ; /* [12]  */
        unsigned int    bpll_tune_busy        : 1   ; /* [13]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG167;

/* Define the union R_PERI_CRG168 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpll_tune_frac        : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG168;

/* Define the union R_PERI_CRG169 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dpll_tune_int         : 12  ; /* [11..0]  */
        unsigned int    dpll_tune_sw_ok       : 1   ; /* [12]  */
        unsigned int    dpll_tune_busy        : 1   ; /* [13]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG169;

/* Define the union R_PERI_CRG170 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dpll_tune_frac        : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG170;

/* Define the union R_PERI_CRG171 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    epll_tune_int         : 12  ; /* [11..0]  */
        unsigned int    epll_tune_sw_ok       : 1   ; /* [12]  */
        unsigned int    epll_tune_busy        : 1   ; /* [13]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG171;

/* Define the union R_PERI_CRG172 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    epll_tune_frac        : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG172;

/* Define the union R_PERI_CRG177 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpll_tune_int         : 12  ; /* [11..0]  */
        unsigned int    hpll_tune_sw_ok       : 1   ; /* [12]  */
        unsigned int    hpll_tune_busy        : 1   ; /* [13]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG177;

/* Define the union R_PERI_CRG178 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpll_tune_frac        : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG178;

/* Define the union R_PERI_CRG179 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpll_tune_int         : 12  ; /* [11..0]  */
        unsigned int    vpll_tune_sw_ok       : 1   ; /* [12]  */
        unsigned int    vpll_tune_busy        : 1   ; /* [13]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG179;

/* Define the union R_PERI_CRG180 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpll_tune_frac        : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG180;

/* Define the union R_PERI_CRG181 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cpR_sm_sw_mode        : 1   ; /* [0]  */
        unsigned int    cpR_sw_step_slow_up_ref : 3   ; /* [3..1]  */
        unsigned int    cpR_sw_step_slow_down_ref : 3   ; /* [6..4]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    gpR_sm_sw_mode        : 1   ; /* [8]  */
        unsigned int    gpR_sw_step_slow_up_ref : 3   ; /* [11..9]  */
        unsigned int    gpR_sw_step_slow_down_ref : 3   ; /* [14..12]  */
        unsigned int    reserved_1            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG181;

/* Define the union R_PERI_CRG182 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clk_cpR_sm_sw_seled   : 1   ; /* [0]  */
        unsigned int    cpR_sm_sw_busy        : 1   ; /* [1]  */
        unsigned int    clk_gpR_sm_sw_seled   : 1   ; /* [2]  */
        unsigned int    gpR_sm_sw_busy        : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG182;

/* Define the union R_PERI_CRG203 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    venc_smmR_cken        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    venc_smmR_srst_req    : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG203;

/* Define the union R_PERI_CRG204 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tde_smmR_cken         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    tde_smmR_srst_req     : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG204;

/* Define the union R_PERI_CRG205 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    jpgd_smmR_cken        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    jpgd_smmR_srst_req    : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG205;

/* Define the union R_PERI_CRG207 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vdh_smmR_cken         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    vdh_smmR_srst_req     : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG207;

/* Define the union R_PERI_CRG208 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vdp_smmR_cken         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    vdp_smmR_srst_req     : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG208;

/* Define the union R_PERI_CRG209 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpss_smmR_cken        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    vpss_smmR_srst_req    : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG209;

/* Define the union R_PERI_CRG210 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pgd_smmR_cken         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    pgd_smmR_srst_req     : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG210;

/* Define the union R_PERI_CRG211 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vo_bp_cken            : 1   ; /* [0]  */
        unsigned int    vdp_g4_cken           : 1   ; /* [1]  */
        unsigned int    vdp_v3_cken           : 1   ; /* [2]  */
        unsigned int    vdp_sd_cken           : 1   ; /* [3]  */
        unsigned int    vdp_wbc_mad_cken      : 1   ; /* [4]  */
        unsigned int    vdp_wbc_ref_cken      : 1   ; /* [5]  */
        unsigned int    vdp_wbc_vp_cken       : 1   ; /* [6]  */
        unsigned int    vdp_wbc_gp_cken       : 1   ; /* [7]  */
        unsigned int    vdp_wbc_hd_cken       : 1   ; /* [8]  */
        unsigned int    vdp_g3_cken           : 1   ; /* [9]  */
        unsigned int    vdp_g1_cken           : 1   ; /* [10]  */
        unsigned int    vdp_g0_cken           : 1   ; /* [11]  */
        unsigned int    vdp_v1_cken           : 1   ; /* [12]  */
        unsigned int    vdp_v0_cken           : 1   ; /* [13]  */
        unsigned int    vdp_hd_cken           : 1   ; /* [14]  */
        unsigned int    reserved_0            : 2   ; /* [16..15]  */
        unsigned int    vdp_hd_clk_div1       : 1   ; /* [17]  */
        unsigned int    reserved_1            : 2   ; /* [19..18]  */
        unsigned int    vdp_hd_clk_div0       : 1   ; /* [20]  */
        unsigned int    vdp_ini_cksel1        : 1   ; /* [21]  */
        unsigned int    vdp_ini_cksel0        : 1   ; /* [22]  */
        unsigned int    vo_hd_bp_clk_sel      : 1   ; /* [23]  */
        unsigned int    vdp_clk_div           : 5   ; /* [28..24]  */
        unsigned int    vdp_srst_req          : 1   ; /* [29]  */
        unsigned int    reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG211;

/* Define the union R_PERI_CRG212 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpll_tune_int_cfg     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG212;

/* Define the union R_PERI_CRG213 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpll_tune_frac_cfg    : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG213;

/* Define the union R_PERI_CRG214 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpll_tune_step_int    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG214;

/* Define the union R_PERI_CRG215 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpll_tune_step_frac   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG215;

/* Define the union R_PERI_CRG216 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpll_tune_cken        : 1   ; /* [0]  */
        unsigned int    qpll_tune_srst_req    : 1   ; /* [1]  */
        unsigned int    qpll_tune_mode        : 1   ; /* [2]  */
        unsigned int    qpll_tune_en          : 1   ; /* [3]  */
        unsigned int    qpll_tune_divval      : 4   ; /* [7..4]  */
        unsigned int    qpll_tune_soft_cfg    : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG216;

/* Define the union R_PERI_CRG217 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpll_tune_int         : 12  ; /* [11..0]  */
        unsigned int    qpll_tune_sw_ok       : 1   ; /* [12]  */
        unsigned int    qpll_tune_busy        : 1   ; /* [13]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG217;

/* Define the union R_PERI_CRG218 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpll_tune_frac        : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG218;

/* Define the union R_PERI_CRG221 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mem_adjust_cpR_2      : 17  ; /* [16..0]  */
        unsigned int    reserved_0            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG221;

/* Define the union R_PERI_CRG222 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cipher_smmR_cken      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    cipher_smmR_srst_req  : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG222;

/* Define the union R_PERI_CRG224 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fmc_cken              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    fmc_srst_req          : 1   ; /* [4]  */
        unsigned int    fmc_clk_sel           : 3   ; /* [7..5]  */
        unsigned int    reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG224;

/* Define the union R_PERI_CRG225 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    rsa_srst_req          : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG225;

/* Define the union R_PERI_CRG226 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fephy_cken            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    fephy_srst_req        : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG226;

/* Define the union R_PERI_CRG227 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dmac_cken             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    dmac_srst_req         : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG227;

/* Define the union R_PERI_CRG228 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gzip_cken             : 1   ; /* [0]  */
        unsigned int    gzip_acken            : 1   ; /* [1]  */
        unsigned int    gzip_pcken            : 1   ; /* [2]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    gzip_srst_req         : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG228;

/* Define the union R_PERI_CRG229 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wifi_clk_sel          : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG229;

/* Define the union R_PERI_CRG230 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pm_cken               : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    pm_srst_req           : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG230;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile R_PERI_CRG_PLL0        PERI_CRG_PLL0            ; /* 0x0 */
    volatile R_PERI_CRG_PLL1        PERI_CRG_PLL1            ; /* 0x4 */
    volatile R_PERI_CRG_PLL2        PERI_CRG_PLL2            ; /* 0x8 */
    volatile R_PERI_CRG_PLL3        PERI_CRG_PLL3            ; /* 0xc */
    volatile R_PERI_CRG_PLL4        PERI_CRG_PLL4            ; /* 0x10 */
    volatile R_PERI_CRG_PLL5        PERI_CRG_PLL5            ; /* 0x14 */
    volatile unsigned int           reserved_0[2]                     ; /* 0x18~0x1c */
    volatile R_PERI_CRG_PLL8        PERI_CRG_PLL8            ; /* 0x20 */
    volatile R_PERI_CRG_PLL9        PERI_CRG_PLL9            ; /* 0x24 */
    volatile R_PERI_CRG_PLL10       PERI_CRG_PLL10           ; /* 0x28 */
    volatile R_PERI_CRG_PLL11       PERI_CRG_PLL11           ; /* 0x2c */
    volatile R_PERI_CRG_PLL12       PERI_CRG_PLL12           ; /* 0x30 */
    volatile R_PERI_CRG_PLL13       PERI_CRG_PLL13           ; /* 0x34 */
    volatile R_PERI_CRG_PLL14       PERI_CRG_PLL14           ; /* 0x38 */
    volatile R_PERI_CRG_PLL15       PERI_CRG_PLL15           ; /* 0x3c */
    volatile unsigned int           reserved_1[2]                     ; /* 0x40~0x44 */
    volatile R_PERI_CRG18           PERI_CRG18               ; /* 0x48 */
    volatile R_PERI_CRG19           PERI_CRG19               ; /* 0x4c */
    volatile R_PERI_CRG20           PERI_CRG20               ; /* 0x50 */
    volatile unsigned int           reserved_2                     ; /* 0x54 */
    volatile R_PERI_CRG22           PERI_CRG22               ; /* 0x58 */
    volatile unsigned int           reserved_3[2]                     ; /* 0x5c~0x60 */
    volatile R_PERI_CRG25           PERI_CRG25               ; /* 0x64 */
    volatile R_PERI_CRG26           PERI_CRG26               ; /* 0x68 */
    volatile R_PERI_CRG27           PERI_CRG27               ; /* 0x6c */
    volatile R_PERI_CRG28           PERI_CRG28               ; /* 0x70 */
    volatile R_PERI_CRG29           PERI_CRG29               ; /* 0x74 */
    volatile R_PERI_CRG30           PERI_CRG30               ; /* 0x78 */
    volatile R_PERI_CRG31           PERI_CRG31               ; /* 0x7c */
    volatile unsigned int           reserved_4                     ; /* 0x80 */
    volatile R_PERI_CRG33           PERI_CRG33               ; /* 0x84 */
    volatile R_PERI_CRG34           PERI_CRG34               ; /* 0x88 */
    volatile R_PERI_CRG35           PERI_CRG35               ; /* 0x8c */
    volatile R_PERI_CRG36           PERI_CRG36               ; /* 0x90 */
    volatile R_PERI_CRG37           PERI_CRG37               ; /* 0x94 */
    volatile unsigned int           reserved_5                     ; /* 0x98 */
    volatile R_PERI_CRG39           PERI_CRG39               ; /* 0x9c */
    volatile R_PERI_CRG40           PERI_CRG40               ; /* 0xa0 */
    volatile unsigned int           reserved_6                     ; /* 0xa4 */
    volatile R_PERI_CRG42           PERI_CRG42               ; /* 0xa8 */
    volatile unsigned int           reserved_7                     ; /* 0xac */
    volatile R_PERI_CRG44           PERI_CRG44               ; /* 0xb0 */
    volatile unsigned int           reserved_8                     ; /* 0xb4 */
    volatile R_PERI_CRG46           PERI_CRG46               ; /* 0xb8 */
    volatile R_PERI_CRG47           PERI_CRG47               ; /* 0xbc */
    volatile R_PERI_CRG48           PERI_CRG48               ; /* 0xc0 */
    volatile R_PERI_CRG49           PERI_CRG49               ; /* 0xc4 */
    volatile R_PERI_CRG50           PERI_CRG50               ; /* 0xc8 */
    volatile R_PERI_CRG51           PERI_CRG51               ; /* 0xcc */
    volatile R_PERI_CRG52           PERI_CRG52               ; /* 0xd0 */
    volatile R_PERI_CRG53           PERI_CRG53               ; /* 0xd4 */
    volatile R_PERI_CRG54           PERI_CRG54               ; /* 0xd8 */
    volatile unsigned int           reserved_9[5]                     ; /* 0xdc~0xec */
    volatile R_PERI_CRG60           PERI_CRG60               ; /* 0xf0 */
    volatile unsigned int           reserved_10[2]                     ; /* 0xf4~0xf8 */
    volatile R_PERI_CRG63           PERI_CRG63               ; /* 0xfc */
    volatile R_PERI_CRG64           PERI_CRG64               ; /* 0x100 */
    volatile unsigned int           reserved_11[2]                     ; /* 0x104~0x108 */
    volatile R_PERI_CRG67           PERI_CRG67               ; /* 0x10c */
    volatile R_PERI_CRG68           PERI_CRG68               ; /* 0x110 */
    volatile R_PERI_CRG69           PERI_CRG69               ; /* 0x114 */
    volatile R_PERI_CRG70           PERI_CRG70               ; /* 0x118 */
    volatile R_PERI_CRG71           PERI_CRG71               ; /* 0x11c */
    volatile unsigned int           reserved_12                     ; /* 0x120 */
    volatile R_PERI_CRG73           PERI_CRG73               ; /* 0x124 */
    volatile R_PERI_CRG74           PERI_CRG74               ; /* 0x128 */
    volatile R_PERI_CRG75           PERI_CRG75               ; /* 0x12c */
    volatile R_PERI_CRG76           PERI_CRG76               ; /* 0x130 */
    volatile R_PERI_CRG77           PERI_CRG77               ; /* 0x134 */
    volatile unsigned int           reserved_13                     ; /* 0x138 */
    volatile R_PERI_CRG79           PERI_CRG79               ; /* 0x13c */
    volatile R_PERI_CRG80           PERI_CRG80               ; /* 0x140 */
    volatile R_PERI_CRG81           PERI_CRG81               ; /* 0x144 */
    volatile R_PERI_CRG82           PERI_CRG82               ; /* 0x148 */
    volatile unsigned int           reserved_14                     ; /* 0x14c */
    volatile R_PERI_CRG84           PERI_CRG84               ; /* 0x150 */
    volatile R_PERI_CRG85           PERI_CRG85               ; /* 0x154 */
    volatile R_PERI_CRG_PLL86       PERI_CRG_PLL86           ; /* 0x158 */
    volatile R_PERI_CRG_PLL87       PERI_CRG_PLL87           ; /* 0x15c */
    volatile R_PERI_CRG_PLL88       PERI_CRG_PLL88           ; /* 0x160 */
    volatile R_PERI_CRG_PLL89       PERI_CRG_PLL89           ; /* 0x164 */
    volatile R_PERI_CRG90           PERI_CRG90               ; /* 0x168 */
    volatile unsigned int           reserved_15[2]                     ; /* 0x16c~0x170 */
    volatile R_PERI_CRG93           PERI_CRG93               ; /* 0x174 */
    volatile R_PERI_CRG94           PERI_CRG94               ; /* 0x178 */
    volatile R_PERI_CRG95           PERI_CRG95               ; /* 0x17c */
    volatile R_PERI_CRG96           PERI_CRG96               ; /* 0x180 */
    volatile R_PERI_CRG97           PERI_CRG97               ; /* 0x184 */
    volatile R_PERI_CRG98           PERI_CRG98               ; /* 0x188 */
    volatile R_PERI_CRG99           PERI_CRG99               ; /* 0x18c */
    volatile unsigned int           reserved_16[5]                     ; /* 0x190~0x1a0 */
    volatile R_PERI_CRG105          PERI_CRG105              ; /* 0x1a4 */
    volatile R_PERI_CRG106          PERI_CRG106              ; /* 0x1a8 */
    volatile R_PERI_CRG107          PERI_CRG107              ; /* 0x1ac */
    volatile R_PERI_CRG108          PERI_CRG108              ; /* 0x1b0 */
    volatile R_PERI_CRG109          PERI_CRG109              ; /* 0x1b4 */
    volatile R_PERI_CRG110          PERI_CRG110              ; /* 0x1b8 */
    volatile R_PERI_CRG111          PERI_CRG111              ; /* 0x1bc */
    volatile R_PERI_CRG112          PERI_CRG112              ; /* 0x1c0 */
    volatile R_PERI_CRG113          PERI_CRG113              ; /* 0x1c4 */
    volatile R_PERI_CRG114          PERI_CRG114              ; /* 0x1c8 */
    volatile R_PERI_CRG115          PERI_CRG115              ; /* 0x1cc */
    volatile R_PERI_CRG116          PERI_CRG116              ; /* 0x1d0 */
    volatile R_PERI_CRG117          PERI_CRG117              ; /* 0x1d4 */
    volatile R_PERI_CRG118          PERI_CRG118              ; /* 0x1d8 */
    volatile R_PERI_CRG119          PERI_CRG119              ; /* 0x1dc */
    volatile R_PERI_CRG120          PERI_CRG120              ; /* 0x1e0 */
    volatile R_PERI_CRG121          PERI_CRG121              ; /* 0x1e4 */
    volatile R_PERI_CRG122          PERI_CRG122              ; /* 0x1e8 */
    volatile R_PERI_CRG123          PERI_CRG123              ; /* 0x1ec */
    volatile R_PERI_CRG124          PERI_CRG124              ; /* 0x1f0 */
    volatile unsigned int           reserved_17[10]                     ; /* 0x1f4~0x218 */
    volatile R_PERI_CRG135          PERI_CRG135              ; /* 0x21c */
    volatile R_PERI_CRG136          PERI_CRG136              ; /* 0x220 */
    volatile R_PERI_CRG137          PERI_CRG137              ; /* 0x224 */
    volatile R_PERI_CRG138          PERI_CRG138              ; /* 0x228 */
    volatile R_PERI_CRG139          PERI_CRG139              ; /* 0x22c */
    volatile R_PERI_CRG140          PERI_CRG140              ; /* 0x230 */
    volatile R_PERI_CRG141          PERI_CRG141              ; /* 0x234 */
    volatile R_PERI_CRG142          PERI_CRG142              ; /* 0x238 */
    volatile R_PERI_CRG143          PERI_CRG143              ; /* 0x23c */
    volatile R_PERI_CRG144          PERI_CRG144              ; /* 0x240 */
    volatile unsigned int           reserved_18[14]                     ; /* 0x244~0x278 */
    volatile R_PERI_CRG159          PERI_CRG159              ; /* 0x27c */
    volatile R_PERI_CRG160          PERI_CRG160              ; /* 0x280 */
    volatile R_PERI_CRG161          PERI_CRG161              ; /* 0x284 */
    volatile unsigned int           reserved_19                     ; /* 0x288 */
    volatile R_PERI_CRG163          PERI_CRG163              ; /* 0x28c */
    volatile R_PERI_CRG164          PERI_CRG164              ; /* 0x290 */
    volatile R_PERI_CRG165          PERI_CRG165              ; /* 0x294 */
    volatile R_PERI_CRG166          PERI_CRG166              ; /* 0x298 */
    volatile R_PERI_CRG167          PERI_CRG167              ; /* 0x29c */
    volatile R_PERI_CRG168          PERI_CRG168              ; /* 0x2a0 */
    volatile R_PERI_CRG169          PERI_CRG169              ; /* 0x2a4 */
    volatile R_PERI_CRG170          PERI_CRG170              ; /* 0x2a8 */
    volatile R_PERI_CRG171          PERI_CRG171              ; /* 0x2ac */
    volatile R_PERI_CRG172          PERI_CRG172              ; /* 0x2b0 */
    volatile unsigned int           reserved_20[4]                     ; /* 0x2b4~0x2c0 */
    volatile R_PERI_CRG177          PERI_CRG177              ; /* 0x2c4 */
    volatile R_PERI_CRG178          PERI_CRG178              ; /* 0x2c8 */
    volatile R_PERI_CRG179          PERI_CRG179              ; /* 0x2cc */
    volatile R_PERI_CRG180          PERI_CRG180              ; /* 0x2d0 */
    volatile R_PERI_CRG181          PERI_CRG181              ; /* 0x2d4 */
    volatile R_PERI_CRG182          PERI_CRG182              ; /* 0x2d8 */
    volatile unsigned int           reserved_21[20]                     ; /* 0x2dc~0x328 */
    volatile R_PERI_CRG203          PERI_CRG203              ; /* 0x32c */
    volatile R_PERI_CRG204          PERI_CRG204              ; /* 0x330 */
    volatile R_PERI_CRG205          PERI_CRG205              ; /* 0x334 */
    volatile unsigned int           reserved_22                     ; /* 0x338 */
    volatile R_PERI_CRG207          PERI_CRG207              ; /* 0x33c */
    volatile R_PERI_CRG208          PERI_CRG208              ; /* 0x340 */
    volatile R_PERI_CRG209          PERI_CRG209              ; /* 0x344 */
    volatile R_PERI_CRG210          PERI_CRG210              ; /* 0x348 */
    volatile R_PERI_CRG211          PERI_CRG211              ; /* 0x34c */
    volatile R_PERI_CRG212          PERI_CRG212              ; /* 0x350 */
    volatile R_PERI_CRG213          PERI_CRG213              ; /* 0x354 */
    volatile R_PERI_CRG214          PERI_CRG214              ; /* 0x358 */
    volatile R_PERI_CRG215          PERI_CRG215              ; /* 0x35c */
    volatile R_PERI_CRG216          PERI_CRG216              ; /* 0x360 */
    volatile R_PERI_CRG217          PERI_CRG217              ; /* 0x364 */
    volatile R_PERI_CRG218          PERI_CRG218              ; /* 0x368 */
    volatile unsigned int           reserved_23[2]                     ; /* 0x36c~0x370 */
    volatile R_PERI_CRG221          PERI_CRG221              ; /* 0x374 */
    volatile R_PERI_CRG222          PERI_CRG222              ; /* 0x378 */
    volatile unsigned int           reserved_24                     ; /* 0x37c */
    volatile R_PERI_CRG224          PERI_CRG224              ; /* 0x380 */
    volatile R_PERI_CRG225          PERI_CRG225              ; /* 0x384 */
    volatile R_PERI_CRG226          PERI_CRG226              ; /* 0x388 */
    volatile R_PERI_CRG227          PERI_CRG227              ; /* 0x38c */
    volatile R_PERI_CRG228          PERI_CRG228              ; /* 0x390 */
    volatile R_PERI_CRG229          PERI_CRG229              ; /* 0x394 */
    volatile R_PERI_CRG230          PERI_CRG230              ; /* 0x398 */

} HDMI_REG_CRG_S;

//extern volatile HDMI_REG_CRG_S *s_pstRegCrg;

int REG_HDMI_CRG_reg_Init(void);
int REG_HDMI_CRG_reg_DeInit(void);

//hpll ctrl0
int REG_HDMI_CRG_hpll_frac_Set(unsigned int uhpll_frac);
int REG_HDMI_CRG_hpll_frac_Get(HI_VOID);
int REG_HDMI_CRG_hpll_postdiv1_Set(unsigned int uhpll_postdiv1);
int REG_HDMI_CRG_hpll_postdiv1_Get(HI_VOID);
int REG_HDMI_CRG_hpll_postdiv2_Set(unsigned int uhpll_postdiv2);
int REG_HDMI_CRG_hpll_postdiv2_Get(HI_VOID);
//hpll ctrl1
int REG_HDMI_CRG_hpll_fbdiv_Set(unsigned int uhpll_fbdiv);
int REG_HDMI_CRG_hpll_fbdiv_Get(HI_VOID);
int REG_HDMI_CRG_hpll_refdiv_Set(unsigned int uhpll_refdiv);
int REG_HDMI_CRG_hpll_refdiv_Get(HI_VOID);
int REG_HDMI_CRG_hpll_pd_Set(unsigned int uhpll_pd);
int REG_HDMI_CRG_hpll_pd_Get(HI_VOID);
int REG_HDMI_CRG_hpll_foutvcopd_Set(unsigned int uhpll_foutvcopd);
int REG_HDMI_CRG_hpll_foutvcopd_Get(HI_VOID);
int REG_HDMI_CRG_hpll_fout4phasepdPLL_Set(unsigned int uhpll_fout4phasepdPLL);
int REG_HDMI_CRG_hpll_fout4phasepdPLL_Get(HI_VOID);
int REG_HDMI_CRG_hpll_foutpostdivpd_Set(unsigned int uhpll_foutpostdivpd);
int REG_HDMI_CRG_hpll_foutpostdivpd_Get(HI_VOID);
int REG_HDMI_CRG_hpll_dacpd_Set(unsigned int uhpll_dacpd);
int REG_HDMI_CRG_hpll_dacpd_Get(HI_VOID);
int REG_HDMI_CRG_hpll_dsmpd_Set(unsigned int uhpll_dsmpd);
int REG_HDMI_CRG_hpll_dsmpd_Get(HI_VOID);
int REG_HDMI_CRG_hpll_bypass_Set(unsigned int uhpll_bypass);
int REG_HDMI_CRG_hpll_bypass_Get(HI_VOID);
//vdp crg1
int REG_HDMI_CRG_vo_bus_cken_Set(unsigned int uvo_bus_cken);
int REG_HDMI_CRG_vo_bus_cken_Get(HI_VOID);
int REG_HDMI_CRG_vo_sd_cken_Set(unsigned int uvo_sd_cken);
int REG_HDMI_CRG_vo_sd_cken_Get(HI_VOID);
int REG_HDMI_CRG_vo_sdate_cken_Set(unsigned int uvo_sdate_cken);
int REG_HDMI_CRG_vo_sdate_cken_Get(HI_VOID);
int REG_HDMI_CRG_vo_hd_cken_Set(unsigned int uvo_hd_cken);
int REG_HDMI_CRG_vo_hd_cken_Get(HI_VOID);
int REG_HDMI_CRG_vdac_ch0_cken_Set(unsigned int uvdac_ch0_cken);
int REG_HDMI_CRG_vdac_ch0_cken_Get(HI_VOID);
int REG_HDMI_CRG_vo_sd_clk_sel_Set(unsigned int uvo_sd_clk_sel);
int REG_HDMI_CRG_vo_sd_clk_sel_Get(HI_VOID);
int REG_HDMI_CRG_vo_sd_clk_div_Set(unsigned int uvo_sd_clk_div);
int REG_HDMI_CRG_vo_sd_clk_div_Get(HI_VOID);
int REG_HDMI_CRG_vo_hd_clk_sel_Set(unsigned int uvo_hd_clk_sel);
int REG_HDMI_CRG_vo_hd_clk_sel_Get(HI_VOID);
int REG_HDMI_CRG_vo_hd_clk_div_Set(unsigned int uvo_hd_clk_div);
int REG_HDMI_CRG_vo_hd_clk_div_Get(HI_VOID);
int REG_HDMI_CRG_vdac_ch0_clk_sel_Set(unsigned int uvdac_ch0_clk_sel);
int REG_HDMI_CRG_vdac_ch0_clk_sel_Get(HI_VOID);
int REG_HDMI_CRG_hdmi_clk_sel_Set(unsigned int uhdmi_clk_sel);
int REG_HDMI_CRG_hdmi_clk_sel_Get(HI_VOID);
int REG_HDMI_CRG_vo_sd_hdmi_clk_sel_Set(unsigned int uvo_sd_hdmi_clk_sel);
int REG_HDMI_CRG_vo_sd_hdmi_clk_sel_Get(HI_VOID);
int REG_HDMI_CRG_vdp_clk_sel_Set(unsigned int uvdp_clk_sel);
int REG_HDMI_CRG_vdp_clk_sel_Get(HI_VOID);
int REG_HDMI_CRG_vo_hd_hdmi_clk_sel_Set(unsigned int uvo_hd_hdmi_clk_sel);
int REG_HDMI_CRG_vo_hd_hdmi_clk_sel_Get(HI_VOID);
int REG_HDMI_CRG_voR_srst_req_Set(unsigned int uvoR_srst_req);
int REG_HDMI_CRG_voR_srst_req_Get(HI_VOID);
//hdmitx ctrl
int REG_HDMI_CRG_ssc_in_cken_Set(unsigned int ussc_in_cken);
int REG_HDMI_CRG_ssc_in_cken_Get(HI_VOID);
int REG_HDMI_CRG_ssc_bypass_cken_Set(unsigned int ussc_bypass_cken);
int REG_HDMI_CRG_ssc_bypass_cken_Get(HI_VOID);
int REG_HDMI_CRG_hdmitx_ctrl_osc_24m_cken_Set(unsigned int uhdmitx_ctrl_osc_24m_cken);
int REG_HDMI_CRG_hdmitx_ctrl_osc_24m_cken_Get(HI_VOID);
int REG_HDMI_CRG_hdmitx_ctrl_cec_cken_Set(unsigned int uhdmitx_ctrl_cec_cken);
int REG_HDMI_CRG_hdmitx_ctrl_cec_cken_Get(HI_VOID);
int REG_HDMI_CRG_hdmitx_ctrl_os_cken_Set(unsigned int uhdmitx_ctrl_os_cken);
int REG_HDMI_CRG_hdmitx_ctrl_os_cken_Get(HI_VOID);
int REG_HDMI_CRG_hdmitx_ctrl_as_cken_Set(unsigned int uhdmitx_ctrl_as_cken);
int REG_HDMI_CRG_hdmitx_ctrl_as_cken_Get(HI_VOID);
int REG_HDMI_CRG_hdmitx_ctrl_bus_srst_req_Set(unsigned int uhdmitx_ctrl_bus_srst_req);
int REG_HDMI_CRG_hdmitx_ctrl_bus_srst_req_Get(HI_VOID);
int REG_HDMI_CRG_hdmitx_ctrl_srst_req_Set(unsigned int uhdmitx_ctrl_srst_req);
int REG_HDMI_CRG_hdmitx_ctrl_srst_req_Get(HI_VOID);
int REG_HDMI_CRG_hdmitx_ctrl_cec_srst_req_Set(unsigned int uhdmitx_ctrl_cec_srst_req);
int REG_HDMI_CRG_hdmitx_ctrl_cec_srst_req_Get(HI_VOID);
int REG_HDMI_CRG_hdmitx_ssc_srst_req_Set(unsigned int uhdmitx_ssc_srst_req);
int REG_HDMI_CRG_hdmitx_ssc_srst_req_Get(HI_VOID);
int REG_HDMI_CRG_ssc_clk_div_Set(unsigned int ussc_clk_div);
int REG_HDMI_CRG_ssc_clk_div_Get(HI_VOID);
int REG_HDMI_CRG_hdmitx_ctrl_cec_clk_sel_Set(unsigned int uhdmitx_ctrl_cec_clk_sel);
int REG_HDMI_CRG_hdmitx_ctrl_cec_clk_sel_Get(HI_VOID);
//hdmitx phy
int REG_HDMI_CRG_phy_tmds_cken_Set(unsigned int uphy_tmds_cken);
int REG_HDMI_CRG_phy_tmds_cken_Get(HI_VOID);
int REG_HDMI_CRG_hdmitx_phy_srst_req_Set(unsigned int uhdmitx_phy_srst_req);
int REG_HDMI_CRG_hdmitx_phy_srst_req_Get(HI_VOID);
int REG_HDMI_CRG_phy_tmds_srst_req_Set(unsigned int uphy_tmds_srst_req);
int REG_HDMI_CRG_phy_tmds_srst_req_Get(HI_VOID);
int REG_HDMI_CRG_tmds_clk_div_Set(unsigned int utmds_clk_div);
int REG_HDMI_CRG_tmds_clk_div_Get(HI_VOID);

#endif /* __HDMI_REG_HI3798MV200_CRG_H__ */
