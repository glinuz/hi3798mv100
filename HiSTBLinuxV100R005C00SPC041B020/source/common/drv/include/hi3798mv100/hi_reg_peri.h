//****************************************************************************** 
// Copyright     :  Copyright (C) 2014, Hisilicon Technologies Co., Ltd.
// File name     :  hi_reg_peri.h
// Author        :  fangqiubin 00179307
// Version       :  1.0 
// Date          :  2014-07-21
// Description   :  Define all registers/tables for PERI_CTRL
// Others        :  Generated automatically by nManager V4.0 
// History       :  fangqiubin 00179307 2014-07-21 Create file
//******************************************************************************

#ifndef __HI_REG_PERI_H__
#define __HI_REG_PERI_H__

/* Define the union U_START_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    caboot_en             : 1   ; /* [8]  */
        unsigned int    boot_sel              : 2   ; /* [10..9]  */
        unsigned int    reserved_1            : 9   ; /* [19..11]  */
        unsigned int    romboot_sel           : 1   ; /* [20]  */
        unsigned int    reserved_2            : 1   ; /* [21]  */
        unsigned int    por_sel               : 1   ; /* [22]  */
        unsigned int    jtag_sel_in           : 1   ; /* [23]  */
        unsigned int    usb_boot              : 1   ; /* [24]  */
        unsigned int    reserved_3            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_START_MODE;

/* Define the union U_PERI_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sdio1_card_det_mode   : 1   ; /* [0]  */
        unsigned int    sdio0_card_det_mode   : 1   ; /* [1]  */
        unsigned int    reserved_0            : 1   ; /* [2]  */
        unsigned int    usb2_ip_iso_ctrl      : 1   ; /* [3]  */
        unsigned int    ssp0_cs_sel           : 1   ; /* [4]  */
        unsigned int    reserved_1            : 21  ; /* [25..5]  */
        unsigned int    usb3_ip_iso_ctrl      : 1   ; /* [26]  */
        unsigned int    reserved_2            : 1   ; /* [27]  */
        unsigned int    clk_mhlnx_sel         : 1   ; /* [28]  */
        unsigned int    peri_jtag_sel         : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_CTRL;

/* Define the union U_CPU_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    smpnamp               : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CPU_STAT;

/* Define the union U_PERI_QOS_GLOB_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddrc_qos_way          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_QOS_GLOB_CTRL;

/* Define the union U_PERI_QOS_CFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aiao_arqos            : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    aiao_awqos            : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    bpd_arqos             : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    bpd_awqos             : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 1   ; /* [15]  */
        unsigned int    cpu_m1_arqos          : 3   ; /* [18..16]  */
        unsigned int    reserved_4            : 1   ; /* [19]  */
        unsigned int    cpu_m1_awqos          : 3   ; /* [22..20]  */
        unsigned int    reserved_5            : 1   ; /* [23]  */
        unsigned int    ddrt_arqos            : 3   ; /* [26..24]  */
        unsigned int    reserved_6            : 1   ; /* [27]  */
        unsigned int    ddrt_awqos            : 3   ; /* [30..28]  */
        unsigned int    reserved_7            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_QOS_CFG0;

/* Define the union U_PERI_QOS_CFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    gpu_arqos             : 3   ; /* [18..16]  */
        unsigned int    reserved_1            : 1   ; /* [19]  */
        unsigned int    gpu_awqos             : 3   ; /* [22..20]  */
        unsigned int    reserved_2            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_QOS_CFG1;

/* Define the union U_PERI_QOS_CFG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    jpgd0_arqos           : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    jpgd0_awqos           : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 9   ; /* [15..7]  */
        unsigned int    jpge_arqos            : 3   ; /* [18..16]  */
        unsigned int    reserved_2            : 1   ; /* [19]  */
        unsigned int    jpge_awqos            : 3   ; /* [22..20]  */
        unsigned int    reserved_3            : 1   ; /* [23]  */
        unsigned int    pgd_arqos             : 3   ; /* [26..24]  */
        unsigned int    reserved_4            : 1   ; /* [27]  */
        unsigned int    pgd_awqos             : 3   ; /* [30..28]  */
        unsigned int    reserved_5            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_QOS_CFG2;

/* Define the union U_PERI_QOS_CFG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 12  ; /* [11..0]  */
        unsigned int    tde_arqos             : 3   ; /* [14..12]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    tde_awqos             : 3   ; /* [18..16]  */
        unsigned int    reserved_2            : 1   ; /* [19]  */
        unsigned int    usb3_m_arqos          : 3   ; /* [22..20]  */
        unsigned int    reserved_3            : 1   ; /* [23]  */
        unsigned int    usb3_m_awqos          : 3   ; /* [26..24]  */
        unsigned int    reserved_4            : 1   ; /* [27]  */
        unsigned int    vdh_arqos             : 3   ; /* [30..28]  */
        unsigned int    reserved_5            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_QOS_CFG3;

/* Define the union U_PERI_QOS_CFG4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vdh_awqos             : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    vdp_arqos             : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    vdp_awqos             : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    vedu_arqos            : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 1   ; /* [15]  */
        unsigned int    vedu_awqos            : 3   ; /* [18..16]  */
        unsigned int    reserved_4            : 5   ; /* [23..19]  */
        unsigned int    vpss0_m0_arqos        : 3   ; /* [26..24]  */
        unsigned int    reserved_5            : 1   ; /* [27]  */
        unsigned int    vpss0_m0_awqos        : 3   ; /* [30..28]  */
        unsigned int    reserved_6            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_QOS_CFG4;

/* Define the union U_PERI_QOS_CFG5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    ca_m_qos              : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    dmac_m0_qos           : 3   ; /* [14..12]  */
        unsigned int    reserved_2            : 1   ; /* [15]  */
        unsigned int    dmac_m1_qos           : 3   ; /* [18..16]  */
        unsigned int    reserved_3            : 5   ; /* [23..19]  */
        unsigned int    nandc_m_qos           : 3   ; /* [26..24]  */
        unsigned int    reserved_4            : 1   ; /* [27]  */
        unsigned int    pvr_m_qos             : 3   ; /* [30..28]  */
        unsigned int    reserved_5            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_QOS_CFG5;

/* Define the union U_PERI_QOS_CFG6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    sdio0_m_qos           : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    sdio1_m_qos           : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    eth_m_qos             : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 9   ; /* [23..15]  */
        unsigned int    sha_m_qos             : 3   ; /* [26..24]  */
        unsigned int    reserved_4            : 1   ; /* [27]  */
        unsigned int    usb2host0_ehci_m_qos  : 3   ; /* [30..28]  */
        unsigned int    reserved_5            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_QOS_CFG6;

/* Define the union U_PERI_QOS_CFG7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    usb2otg0_m_qos        : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 13  ; /* [15..3]  */
        unsigned int    usb2host1_ehci_m_qos  : 3   ; /* [18..16]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_QOS_CFG7;

/* Define the union U_PERI_USB_RESUME_INT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    usb_phy0_suspend_int_mask : 1   ; /* [0]  */
        unsigned int    usb3_utmi_suspend_n   : 1   ; /* [1]  */
        unsigned int    usb_phy2_suspend_int_mask : 1   ; /* [2]  */
        unsigned int    usb_phy1_suspend_int_mask : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB_RESUME_INT_MASK;

/* Define the union U_PERI_USB_RESUME_INT_RAWSTAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    usb_phy0_suspend_int_rawstat : 1   ; /* [0]  */
        unsigned int    usb3_suspend_int_rawstat : 1   ; /* [1]  */
        unsigned int    usb_phy2_suspend_int_rawstat : 1   ; /* [2]  */
        unsigned int    usb_phy1_suspend_int_rawstat : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB_RESUME_INT_RAWSTAT;

/* Define the union U_PERI_USB_RESUME_INT_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    usb_phy0_suspend_int_stat : 1   ; /* [0]  */
        unsigned int    usb3_suspend_int_stat : 1   ; /* [1]  */
        unsigned int    usb_phy2_suspend_int_stat : 1   ; /* [2]  */
        unsigned int    usb_phy1_suspend_int_stat : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB_RESUME_INT_STAT;

/* Define the union U_PERI_INT_A9TOMCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    int_a9tomce           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_INT_A9TOMCE;

/* Define the union U_PERI_INT_SWI0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    int_swi0              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_INT_SWI0;

/* Define the union U_PERI_INT_SWI1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    int_swi1              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_INT_SWI1;

/* Define the union U_PERI_INT_SWI2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    int_swi2              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_INT_SWI2;

/* Define the union U_PERI_INT_SWI0_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    int_swi0_mask_a9      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_INT_SWI0_MASK;

/* Define the union U_PERI_INT_SWI1_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    int_swi1_mask_a9      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_INT_SWI1_MASK;

/* Define the union U_PERI_INT_SWI2_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    int_swi2_mask_a9      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_INT_SWI2_MASK;

/* Define the union U_PERI_TIANLA_ADAC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dacr_vol              : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    dacl_vol              : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 3   ; /* [17..15]  */
        unsigned int    deemphasis_fs         : 2   ; /* [19..18]  */
        unsigned int    dacr_deemph           : 1   ; /* [20]  */
        unsigned int    dacl_deemph           : 1   ; /* [21]  */
        unsigned int    dacr_path             : 1   ; /* [22]  */
        unsigned int    dacl_path             : 1   ; /* [23]  */
        unsigned int    popfreer              : 1   ; /* [24]  */
        unsigned int    popfreel              : 1   ; /* [25]  */
        unsigned int    fs                    : 1   ; /* [26]  */
        unsigned int    pd_vref               : 1   ; /* [27]  */
        unsigned int    mute_dacr             : 1   ; /* [28]  */
        unsigned int    mute_dacl             : 1   ; /* [29]  */
        unsigned int    pd_dacr               : 1   ; /* [30]  */
        unsigned int    pd_dacl               : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_TIANLA_ADAC0;

/* Define the union U_PERI_TIANLA_ADAC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    clkdgesel             : 1   ; /* [8]  */
        unsigned int    clksel2               : 1   ; /* [9]  */
        unsigned int    adj_refbf             : 2   ; /* [11..10]  */
        unsigned int    rst                   : 1   ; /* [12]  */
        unsigned int    adj_ctcm              : 1   ; /* [13]  */
        unsigned int    adj_dac               : 2   ; /* [15..14]  */
        unsigned int    reserved_1            : 3   ; /* [18..16]  */
        unsigned int    sample_sel            : 3   ; /* [21..19]  */
        unsigned int    data_bits             : 2   ; /* [23..22]  */
        unsigned int    reserved_2            : 1   ; /* [24]  */
        unsigned int    mute_rate             : 2   ; /* [26..25]  */
        unsigned int    dacvu                 : 1   ; /* [27]  */
        unsigned int    sunmuter              : 1   ; /* [28]  */
        unsigned int    sunmutel              : 1   ; /* [29]  */
        unsigned int    smuter                : 1   ; /* [30]  */
        unsigned int    smutel                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_TIANLA_ADAC1;

/* Define the union U_PERI_FEPHY */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fephy_phy_addr        : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    fephy_tclk_enable     : 1   ; /* [8]  */
        unsigned int    reserved_1            : 1   ; /* [9]  */
        unsigned int    fephy_patch_enable    : 1   ; /* [10]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    soft_fephy_mdio_mdc   : 1   ; /* [12]  */
        unsigned int    reserved_3            : 1   ; /* [13]  */
        unsigned int    soft_fephy_mdio_i     : 1   ; /* [14]  */
        unsigned int    reserved_4            : 1   ; /* [15]  */
        unsigned int    soft_fephy_gp_i       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_FEPHY;

/* Define the union U_PERI_SD_LDO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fuse                  : 4   ; /* [3..0]  */
        unsigned int    vset                  : 1   ; /* [4]  */
        unsigned int    en                    : 1   ; /* [5]  */
        unsigned int    bypass                : 1   ; /* [6]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_SD_LDO;

/* Define the union U_PERI_USB0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ss_word_if_i          : 1   ; /* [0]  */
        unsigned int    ohci_susp_lgcy_i      : 1   ; /* [1]  */
        unsigned int    app_start_clk_i       : 1   ; /* [2]  */
        unsigned int    ulpi_bypass_en_i      : 1   ; /* [3]  */
        unsigned int    reserved_0            : 1   ; /* [4]  */
        unsigned int    ss_autoppd_on_overcur_en_i : 1   ; /* [5]  */
        unsigned int    ss_ena_incr_align_i   : 1   ; /* [6]  */
        unsigned int    ss_ena_incr4_i        : 1   ; /* [7]  */
        unsigned int    ss_ena_incr8_i        : 1   ; /* [8]  */
        unsigned int    ss_ena_incr16_i       : 1   ; /* [9]  */
        unsigned int    reserved_1            : 2   ; /* [11..10]  */
        unsigned int    reserved_2            : 10  ; /* [21..12]  */
        unsigned int    ss_hubsetup_min_i     : 1   ; /* [22]  */
        unsigned int    reserved_3            : 5   ; /* [27..23]  */
        unsigned int    chipid                : 1   ; /* [28]  */
        unsigned int    ss_scaledown_mode     : 2   ; /* [30..29]  */
        unsigned int    ohci_0_cntsel_i_n     : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB0;

/* Define the union U_PERI_USB1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    test_wrdata           : 8   ; /* [7..0]  */
        unsigned int    test_addr             : 5   ; /* [12..8]  */
        unsigned int    test_wren             : 1   ; /* [13]  */
        unsigned int    test_clk              : 1   ; /* [14]  */
        unsigned int    test_rstn             : 1   ; /* [15]  */
        unsigned int    test_rddata           : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB1;

/* Define the union U_PERI_USB5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    hub_port_perm_attach  : 2   ; /* [3..2]  */
        unsigned int    hub_u2_port_disable   : 1   ; /* [4]  */
        unsigned int    hub_u3_port_disable   : 1   ; /* [5]  */
        unsigned int    host_port_power_control_present : 1   ; /* [6]  */
        unsigned int    host_msi_enable       : 1   ; /* [7]  */
        unsigned int    fladj_30mhz_reg       : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    bus_filter_bypass     : 4   ; /* [19..16]  */
        unsigned int    host_current_belt     : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB5;

/* Define the union U_PERI_USB6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    usb_pwr_ctrl          : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    lane0_ext_pclk_req    : 1   ; /* [8]  */
        unsigned int    lane0_power_present_ovrd : 1   ; /* [9]  */
        unsigned int    lane0_power_present_ovrd_en : 1   ; /* [10]  */
        unsigned int    lane0_tx2rx_loopbk    : 1   ; /* [11]  */
        unsigned int    pcs_tx_deemph_3p5db   : 6   ; /* [17..12]  */
        unsigned int    pcs_tx_deemh_6db      : 6   ; /* [23..18]  */
        unsigned int    pcs_tx_swing_full     : 7   ; /* [30..24]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB6;

/* Define the union U_PERI_USB7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    crdataout             : 16  ; /* [15..0]  */
        unsigned int    crdatain              : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB7;

/* Define the union U_PERI_USB8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    crcapaddr             : 1   ; /* [0]  */
        unsigned int    crcapdata             : 1   ; /* [1]  */
        unsigned int    crread                : 1   ; /* [2]  */
        unsigned int    crwrite               : 1   ; /* [3]  */
        unsigned int    crack                 : 1   ; /* [4]  */
        unsigned int    commomon0             : 1   ; /* [5]  */
        unsigned int    compdistune0          : 3   ; /* [8..6]  */
        unsigned int    drvvbus0              : 1   ; /* [9]  */
        unsigned int    iddig0                : 1   ; /* [10]  */
        unsigned int    idpullup0             : 1   ; /* [11]  */
        unsigned int    loopbackenb0          : 1   ; /* [12]  */
        unsigned int    losbias               : 3   ; /* [15..13]  */
        unsigned int    loslevel              : 4   ; /* [19..16]  */
        unsigned int    mpllmultiplier        : 7   ; /* [26..20]  */
        unsigned int    mpllrefsscclken       : 1   ; /* [27]  */
        unsigned int    otgdisable0           : 1   ; /* [28]  */
        unsigned int    otgtune               : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB8;

/* Define the union U_PERI_USB9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    retenablen            : 1   ; /* [0]  */
        unsigned int    sqrxtune0             : 3   ; /* [3..1]  */
        unsigned int    sscen                 : 1   ; /* [4]  */
        unsigned int    sscrange              : 3   ; /* [7..5]  */
        unsigned int    tx0termoffset         : 5   ; /* [12..8]  */
        unsigned int    txhsxvtune0           : 2   ; /* [14..13]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    sscrefclksel          : 9   ; /* [24..16]  */
        unsigned int    txfuslstune0          : 4   ; /* [28..25]  */
        unsigned int    txpreempamptune0      : 2   ; /* [30..29]  */
        unsigned int    txpreemppulsetune0    : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB9;

/* Define the union U_PERI_USB10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    txrestune0            : 2   ; /* [1..0]  */
        unsigned int    txrisetune0           : 2   ; /* [3..2]  */
        unsigned int    vdatdetenb0           : 1   ; /* [4]  */
        unsigned int    vdatsrcenb0           : 1   ; /* [5]  */
        unsigned int    rtunreq               : 1   ; /* [6]  */
        unsigned int    rtuneak               : 1   ; /* [7]  */
        unsigned int    testpowerdownhsp      : 1   ; /* [8]  */
        unsigned int    testpowerdownssp      : 1   ; /* [9]  */
        unsigned int    txboostlvl            : 3   ; /* [12..10]  */
        unsigned int    txvreftune0           : 4   ; /* [16..13]  */
        unsigned int    vbusvldext0           : 1   ; /* [17]  */
        unsigned int    vbusvldextsel0        : 1   ; /* [18]  */
        unsigned int    reserved_0            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB10;

/* Define the union U_PERI_USB12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pcs_rx_los_mask_val   : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB12;

/* Define the union U_PERI_USB13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ss_word_if_i          : 1   ; /* [0]  */
        unsigned int    ohci_susp_lgcy_i      : 1   ; /* [1]  */
        unsigned int    app_start_clk_i       : 1   ; /* [2]  */
        unsigned int    ulpi_bypass_en_i      : 1   ; /* [3]  */
        unsigned int    reserved_0            : 1   ; /* [4]  */
        unsigned int    ss_autoppd_on_overcur_en_i : 1   ; /* [5]  */
        unsigned int    ss_ena_incr_align_i   : 1   ; /* [6]  */
        unsigned int    ss_ena_incr4_i        : 1   ; /* [7]  */
        unsigned int    ss_ena_incr8_i        : 1   ; /* [8]  */
        unsigned int    ss_ena_incr16_i       : 1   ; /* [9]  */
        unsigned int    reserved_1            : 2   ; /* [11..10]  */
        unsigned int    reserved_2            : 10  ; /* [21..12]  */
        unsigned int    ss_hubsetup_min_i     : 1   ; /* [22]  */
        unsigned int    reserved_3            : 5   ; /* [27..23]  */
        unsigned int    reserved_4            : 3   ; /* [30..28]  */
        unsigned int    ohci_0_cntsel_i_n     : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB13;

/* Define the union U_PERI_USB14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    test_wrdata           : 8   ; /* [7..0]  */
        unsigned int    test_addr             : 5   ; /* [12..8]  */
        unsigned int    test_wren             : 1   ; /* [13]  */
        unsigned int    test_clk              : 1   ; /* [14]  */
        unsigned int    test_rstn             : 1   ; /* [15]  */
        unsigned int    test_rddata           : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_USB14;

/* Define the union U_CHIPSET_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dolby_flag            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 1   ; /* [1]  */
        unsigned int    dts_flag              : 1   ; /* [2]  */
        unsigned int    peri_chipset_info     : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHIPSET_INFO;

/* Define the union U_PERI_TIMEOUT_CFG_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aiao_rtout            : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    aiao_wtout            : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    bpd_rtout             : 2   ; /* [9..8]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    bpd_wtout             : 2   ; /* [13..12]  */
        unsigned int    reserved_3            : 2   ; /* [15..14]  */
        unsigned int    cpu_m1_rtout          : 2   ; /* [17..16]  */
        unsigned int    reserved_4            : 2   ; /* [19..18]  */
        unsigned int    cpu_m1_wtout          : 2   ; /* [21..20]  */
        unsigned int    reserved_5            : 2   ; /* [23..22]  */
        unsigned int    ddrt_rtout            : 2   ; /* [25..24]  */
        unsigned int    reserved_6            : 2   ; /* [27..26]  */
        unsigned int    ddrt_wtout            : 2   ; /* [29..28]  */
        unsigned int    reserved_7            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_TIMEOUT_CFG_0;

/* Define the union U_PERI_TIMEOUT_CFG_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    gpu_rtout             : 2   ; /* [9..8]  */
        unsigned int    reserved_1            : 2   ; /* [11..10]  */
        unsigned int    gpu_wtout             : 2   ; /* [13..12]  */
        unsigned int    reserved_2            : 10  ; /* [23..14]  */
        unsigned int    jpgd_rtout            : 2   ; /* [25..24]  */
        unsigned int    reserved_3            : 2   ; /* [27..26]  */
        unsigned int    jpgd_wtout            : 2   ; /* [29..28]  */
        unsigned int    reserved_4            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_TIMEOUT_CFG_1;

/* Define the union U_PERI_TIMEOUT_CFG_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    jpge_rtout            : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    jpge_wtout            : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    pgd_rtout             : 2   ; /* [9..8]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    pgd_wtout             : 2   ; /* [13..12]  */
        unsigned int    reserved_3            : 14  ; /* [27..14]  */
        unsigned int    tde_rtout             : 2   ; /* [29..28]  */
        unsigned int    reserved_4            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_TIMEOUT_CFG_2;

/* Define the union U_PERI_TIMEOUT_CFG_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tde_wtout             : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    vdh_rtout             : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    vdh_wtout             : 2   ; /* [9..8]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    vdp_rtout             : 2   ; /* [13..12]  */
        unsigned int    reserved_3            : 2   ; /* [15..14]  */
        unsigned int    vdp_wtout             : 2   ; /* [17..16]  */
        unsigned int    reserved_4            : 2   ; /* [19..18]  */
        unsigned int    vedu_rtout            : 2   ; /* [21..20]  */
        unsigned int    reserved_5            : 2   ; /* [23..22]  */
        unsigned int    vedu_wtout            : 2   ; /* [25..24]  */
        unsigned int    reserved_6            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_TIMEOUT_CFG_3;

/* Define the union U_PERI_TIMEOUT_CFG_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vpss_m0_rtout         : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    vpss_m0_wtout         : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    ca_m_tout             : 2   ; /* [9..8]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    dmac_m0_tout          : 2   ; /* [13..12]  */
        unsigned int    reserved_3            : 2   ; /* [15..14]  */
        unsigned int    dmac_m1_tout          : 2   ; /* [17..16]  */
        unsigned int    reserved_4            : 2   ; /* [19..18]  */
        unsigned int    nandc_m_tout          : 2   ; /* [21..20]  */
        unsigned int    reserved_5            : 2   ; /* [23..22]  */
        unsigned int    pvr_m_tout            : 2   ; /* [25..24]  */
        unsigned int    reserved_6            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_TIMEOUT_CFG_4;

/* Define the union U_PERI_TIMEOUT_CFG_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sdio0_m_tout          : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    sdio1_m_wtout         : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    eth_tout              : 2   ; /* [9..8]  */
        unsigned int    reserved_2            : 10  ; /* [19..10]  */
        unsigned int    sha_m_tout            : 2   ; /* [21..20]  */
        unsigned int    reserved_3            : 2   ; /* [23..22]  */
        unsigned int    usb2host0_ehci_m_tout : 2   ; /* [25..24]  */
        unsigned int    reserved_4            : 2   ; /* [27..26]  */
        unsigned int    usb2otg0_m_tout       : 2   ; /* [29..28]  */
        unsigned int    reserved_5            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_TIMEOUT_CFG_5;

/* Define the union U_PERI_TIMEOUT_CFG_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    usb3_m_rout_0         : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    usb3_m_wout_0         : 2   ; /* [9..8]  */
        unsigned int    reserved_2            : 10  ; /* [19..10]  */
        unsigned int    usb2host1_ehci_m_tout : 2   ; /* [21..20]  */
        unsigned int    reserved_3            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_TIMEOUT_CFG_6;

/* Define the union U_PERI_SIM_OD_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    sim0_pwren_od_sel     : 1   ; /* [1]  */
        unsigned int    sim0_rst_od_sel       : 1   ; /* [2]  */
        unsigned int    reserved_1            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_SIM_OD_CTRL;

/* Define the union U_PERI_SOC_FUSE_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    otp_control_cpu       : 1   ; /* [0]  */
        unsigned int    reserved_0            : 2   ; /* [2..1]  */
        unsigned int    otp_control_gpu       : 2   ; /* [4..3]  */
        unsigned int    otp_ddr_st            : 2   ; /* [6..5]  */
        unsigned int    otp_vd_st             : 1   ; /* [7]  */
        unsigned int    otp_control_qam       : 1   ; /* [8]  */
        unsigned int    reserved_1            : 3   ; /* [11..9]  */
        unsigned int    otp_ctrl_vdac         : 1   ; /* [12]  */
        unsigned int    reserved_2            : 3   ; /* [15..13]  */
        unsigned int    chip_id               : 5   ; /* [20..16]  */
        unsigned int    reserved_3            : 3   ; /* [23..21]  */
        unsigned int    mven                  : 1   ; /* [24]  */
        unsigned int    vedu_chip_id          : 2   ; /* [26..25]  */
        unsigned int    reserved_4            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_SOC_FUSE_0;

/* Define the union U_PERI_FEPHY_LDO_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fephy_ldo_vset        : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_FEPHY_LDO_CTRL;

/* Define the union U_PERI_SOC_FUSE_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    otp_control0_usb3     : 1   ; /* [2]  */
        unsigned int    reserved_1            : 1   ; /* [3]  */
        unsigned int    otp_ctrl_uart_core    : 1   ; /* [4]  */
        unsigned int    otp_ctrl_uart_mcu     : 1   ; /* [5]  */
        unsigned int    reserved_2            : 2   ; /* [7..6]  */
        unsigned int    romboot_sel_lock      : 1   ; /* [8]  */
        unsigned int    otp_vdh_real          : 1   ; /* [9]  */
        unsigned int    otp_vdh_divx3         : 1   ; /* [10]  */
        unsigned int    otp_vdh_vc1           : 1   ; /* [11]  */
        unsigned int    otp_vdh_vp6           : 1   ; /* [12]  */
        unsigned int    otp_vdh_vp8           : 1   ; /* [13]  */
        unsigned int    otp_vdh_h264          : 1   ; /* [14]  */
        unsigned int    otp_vdh_mpeg4         : 1   ; /* [15]  */
        unsigned int    otp_vdh_avs           : 1   ; /* [16]  */
        unsigned int    reserved_3            : 2   ; /* [18..17]  */
        unsigned int    otp_ctrl_usb_1p       : 1   ; /* [19]  */
        unsigned int    otp_ctrl_tsi          : 1   ; /* [20]  */
        unsigned int    reserved_4            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_SOC_FUSE_1;

/* Define the union U_PERI_SOC_FUSE_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    otp_vdh_mpg2          : 1   ; /* [0]  */
        unsigned int    otp_vdh_h265          : 1   ; /* [1]  */
        unsigned int    otp_vdh_4k_2k         : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_SOC_FUSE_2;

/* Define the union U_PERI_CPU_A7_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    func_mbist_en         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    l1rstdisable0         : 1   ; /* [4]  */
        unsigned int    l1rstdisable1         : 1   ; /* [5]  */
        unsigned int    l1rstdisable2         : 1   ; /* [6]  */
        unsigned int    l1rstdisable3         : 1   ; /* [7]  */
        unsigned int    l2rstdisable          : 1   ; /* [8]  */
        unsigned int    reserved_1            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_CPU_A7_CFG;

/* Define the union U_PERI_CPU_A7_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fun_mbist_done        : 1   ; /* [0]  */
        unsigned int    fun_mbist_fail        : 1   ; /* [1]  */
        unsigned int    core0_fun_mbist_ram_fail : 7   ; /* [8..2]  */
        unsigned int    core1_fun_mbist_ram_fail : 7   ; /* [15..9]  */
        unsigned int    core2_fun_mbist_ram_fail : 7   ; /* [22..16]  */
        unsigned int    core3_fun_mbist_ram_fail : 7   ; /* [29..23]  */
        unsigned int    l2_fun_mbist_dataram_fail : 1   ; /* [30]  */
        unsigned int    l2_fun_mbist_tagram_fail : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_CPU_A7_STAT;

/* Define the union U_PERI_FE_OD_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fe_led_act_od_sel     : 1   ; /* [0]  */
        unsigned int    fe_led_base_od_sel    : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_FE_OD_CTRL;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_START_MODE           START_MODE               ; /* 0x0 */
    volatile unsigned int           reserved_0                     ; /* 0x4 */
    volatile U_PERI_CTRL            PERI_CTRL                ; /* 0x8 */
    volatile U_CPU_STAT             CPU_STAT                 ; /* 0xc */
    volatile unsigned int           reserved_1[12]                     ; /* 0x10~0x3c */
    volatile U_PERI_QOS_GLOB_CTRL   PERI_QOS_GLOB_CTRL       ; /* 0x40 */
    volatile U_PERI_QOS_CFG0        PERI_QOS_CFG0            ; /* 0x44 */
    volatile U_PERI_QOS_CFG1        PERI_QOS_CFG1            ; /* 0x48 */
    volatile U_PERI_QOS_CFG2        PERI_QOS_CFG2            ; /* 0x4c */
    volatile U_PERI_QOS_CFG3        PERI_QOS_CFG3            ; /* 0x50 */
    volatile U_PERI_QOS_CFG4        PERI_QOS_CFG4            ; /* 0x54 */
    volatile U_PERI_QOS_CFG5        PERI_QOS_CFG5            ; /* 0x58 */
    volatile U_PERI_QOS_CFG6        PERI_QOS_CFG6            ; /* 0x5c */
    volatile U_PERI_QOS_CFG7        PERI_QOS_CFG7            ; /* 0x60 */
    volatile unsigned int           reserved_2[20]                     ; /* 0x64~0xb0 */
    volatile U_PERI_USB_RESUME_INT_MASK   PERI_USB_RESUME_INT_MASK ; /* 0xb4 */
    volatile U_PERI_USB_RESUME_INT_RAWSTAT   PERI_USB_RESUME_INT_RAWSTAT ; /* 0xb8 */
    volatile U_PERI_USB_RESUME_INT_STAT   PERI_USB_RESUME_INT_STAT ; /* 0xbc */
    volatile U_PERI_INT_A9TOMCE     PERI_INT_A9TOMCE         ; /* 0xc0 */
    volatile unsigned int           reserved_3[8]                     ; /* 0xc4~0xe0 */
    volatile U_PERI_INT_SWI0        PERI_INT_SWI0            ; /* 0xe4 */
    volatile U_PERI_INT_SWI1        PERI_INT_SWI1            ; /* 0xe8 */
    volatile U_PERI_INT_SWI2        PERI_INT_SWI2            ; /* 0xec */
    volatile U_PERI_INT_SWI0_MASK   PERI_INT_SWI0_MASK       ; /* 0xf0 */
    volatile U_PERI_INT_SWI1_MASK   PERI_INT_SWI1_MASK       ; /* 0xf4 */
    volatile U_PERI_INT_SWI2_MASK   PERI_INT_SWI2_MASK       ; /* 0xf8 */
    volatile unsigned int           reserved_4[5]                     ; /* 0xfc~0x10c */
    volatile U_PERI_TIANLA_ADAC0    PERI_TIANLA_ADAC0        ; /* 0x110 */
    volatile U_PERI_TIANLA_ADAC1    PERI_TIANLA_ADAC1        ; /* 0x114 */
    volatile U_PERI_FEPHY           PERI_FEPHY               ; /* 0x118 */
    volatile U_PERI_SD_LDO          PERI_SD_LDO              ; /* 0x11c */
    volatile U_PERI_USB0            PERI_USB0                ; /* 0x120 */
    volatile U_PERI_USB1            PERI_USB1                ; /* 0x124 */
    volatile unsigned int           reserved_5[3]                     ; /* 0x128~0x130 */
    volatile U_PERI_USB5            PERI_USB5                ; /* 0x134 */
    volatile U_PERI_USB6            PERI_USB6                ; /* 0x138 */
    volatile U_PERI_USB7            PERI_USB7                ; /* 0x13c */
    volatile U_PERI_USB8            PERI_USB8                ; /* 0x140 */
    volatile U_PERI_USB9            PERI_USB9                ; /* 0x144 */
    volatile U_PERI_USB10           PERI_USB10               ; /* 0x148 */
    volatile unsigned int           reserved_6                     ; /* 0x14c */
    volatile U_PERI_USB12           PERI_USB12               ; /* 0x150 */
    volatile U_PERI_USB13           PERI_USB13               ; /* 0x154 */
    volatile U_PERI_USB14           PERI_USB14               ; /* 0x158 */
    volatile unsigned int           reserved_7[33]                     ; /* 0x15c~0x1dc */
    volatile U_CHIPSET_INFO         CHIPSET_INFO             ; /* 0x1e0 */
    volatile unsigned int           reserved_8[3]                     ; /* 0x1e4~0x1ec */
    volatile unsigned int           PERI_SW_SET              ; /* 0x1f0 */
    volatile unsigned int           reserved_9[395]                     ; /* 0x1f4~0x81c */
    volatile U_PERI_TIMEOUT_CFG_0   PERI_TIMEOUT_CFG_0       ; /* 0x820 */
    volatile U_PERI_TIMEOUT_CFG_1   PERI_TIMEOUT_CFG_1       ; /* 0x824 */
    volatile U_PERI_TIMEOUT_CFG_2   PERI_TIMEOUT_CFG_2       ; /* 0x828 */
    volatile U_PERI_TIMEOUT_CFG_3   PERI_TIMEOUT_CFG_3       ; /* 0x82c */
    volatile U_PERI_TIMEOUT_CFG_4   PERI_TIMEOUT_CFG_4       ; /* 0x830 */
    volatile U_PERI_TIMEOUT_CFG_5   PERI_TIMEOUT_CFG_5       ; /* 0x834 */
    volatile U_PERI_TIMEOUT_CFG_6   PERI_TIMEOUT_CFG_6       ; /* 0x838 */
    volatile U_PERI_SIM_OD_CTRL     PERI_SIM_OD_CTRL         ; /* 0x83c */
    volatile U_PERI_SOC_FUSE_0      PERI_SOC_FUSE_0          ; /* 0x840 */
    volatile U_PERI_FEPHY_LDO_CTRL   PERI_FEPHY_LDO_CTRL     ; /* 0x844 */
    volatile unsigned int           reserved_10[10]                     ; /* 0x848~0x86c */
    volatile U_PERI_SOC_FUSE_1      PERI_SOC_FUSE_1          ; /* 0x870 */
    volatile U_PERI_SOC_FUSE_2      PERI_SOC_FUSE_2          ; /* 0x874 */
    volatile unsigned int           reserved_11[2]                     ; /* 0x878~0x87c */
    volatile U_PERI_CPU_A7_CFG      PERI_CPU_A7_CFG          ; /* 0x880 */
    volatile U_PERI_CPU_A7_STAT     PERI_CPU_A7_STAT         ; /* 0x884 */
    volatile unsigned int           reserved_12[2]                     ; /* 0x888~0x88c */
    volatile U_PERI_FE_OD_CTRL      PERI_FE_OD_CTRL          ; /* 0x890 */

} S_PERI_CTRL_REGS_TYPE;

/* Declare the struct pointor of the module PERI_CTRL */
typedef S_PERI_CTRL_REGS_TYPE S_PERICTRL_REGS_TYPE;

/* Declare the functions that set the member value */


#endif /* __HI_REG_PERI_H__ */
