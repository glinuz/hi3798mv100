//****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co., Ltd.
// File name     :  hi_reg_sys.h
// Author        :  fangqiubin 00179307
// Version       :  1.0 
// Date          :  2015-08-14
// Description   :  Define all registers/tables for SYS_CTRL
// Others        :  Generated automatically by nManager V4.0 
// History       :  fangqiubin 00179307 2015-08-14 Create file
//******************************************************************************

#ifndef __HI_REG_SYS_H__
#define __HI_REG_SYS_H__

/* Define the union U_SC_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_bus_clk_sel       : 2   ; /* [1..0]  */
        unsigned int    mcu_bus_clk_sele_stat : 2   ; /* [3..2]  */
        unsigned int    mcu_bus_clk_div       : 2   ; /* [5..4]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_CTRL;

/* Define the union U_SC_POWER_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    power_state           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_POWER_STAT;

/* Define the union U_SC_L2_CACHE_SIZE_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    l2_cache_size_cfg     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_L2_CACHE_SIZE_CFG;

/* Define the union U_SC_LOW_POWER_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bus_discon_timeout_clr : 1   ; /* [0]  */
        unsigned int    reserved_0            : 2   ; /* [2..1]  */
        unsigned int    mcu_lp_subsys_iso     : 1   ; /* [3]  */
        unsigned int    sys_power_status      : 4   ; /* [7..4]  */
        unsigned int    reserved_1            : 1   ; /* [8]  */
        unsigned int    core_pwr_active       : 1   ; /* [9]  */
        unsigned int    reserved_2            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_LOW_POWER_CTRL;

/* Define the union U_SC_IO_REUSE_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    uart_rx__sel          : 1   ; /* [0]  */
        unsigned int    uart_tx_sel           : 1   ; /* [1]  */
        unsigned int    clk_gpio_sel          : 1   ; /* [2]  */
        unsigned int    data_gpio_sel         : 1   ; /* [3]  */
        unsigned int    key_gpio_sel          : 1   ; /* [4]  */
        unsigned int    ir_gpio_sel           : 1   ; /* [5]  */
        unsigned int    rst_gpio_sel          : 1   ; /* [6]  */
        unsigned int    gpio5_3_ie            : 1   ; /* [7]  */
        unsigned int    gpio5_4_ie            : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_IO_REUSE_SEL;

/* Define the union U_SC_CLKGATE_SRST_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mce_cken              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    ir_cken               : 1   ; /* [4]  */
        unsigned int    ir_srst_req           : 1   ; /* [5]  */
        unsigned int    timer01_cken          : 1   ; /* [6]  */
        unsigned int    timer01_srst_req      : 1   ; /* [7]  */
        unsigned int    led_cken              : 1   ; /* [8]  */
        unsigned int    led_srst_req          : 1   ; /* [9]  */
        unsigned int    reserved_1            : 2   ; /* [11..10]  */
        unsigned int    uart_cken             : 1   ; /* [12]  */
        unsigned int    uart_srst_req         : 1   ; /* [13]  */
        unsigned int    reserved_2            : 15  ; /* [28..14]  */
        unsigned int    uart_sel              : 1   ; /* [29]  */
        unsigned int    lsadc_cken            : 1   ; /* [30]  */
        unsigned int    lsadc_srst_req        : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_CLKGATE_SRST_CTRL;

/* Define the union U_SC_WDG_RST_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wdg_rst_ctrl          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_WDG_RST_CTRL;

/* Define the union U_SC_DDRPHY_LP_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddrphy0_lp_en         : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_DDRPHY_LP_EN;

/* Define the union U_SC_MCU_HPM_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_hpm_div           : 6   ; /* [5..0]  */
        unsigned int    mcu_hpm_en            : 1   ; /* [6]  */
        unsigned int    mcu_hpm_rst_req       : 1   ; /* [7]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_MCU_HPM_CTRL;

/* Define the union U_SC_MCU_HPM_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_hpm_pc_org        : 10  ; /* [9..0]  */
        unsigned int    mcu_hpm_valid         : 1   ; /* [10]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_MCU_HPM_STAT;

/* Define the union U_SC_MCU_LDO_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_ldo_vset          : 4   ; /* [3..0]  */
        unsigned int    mcu_ldo_fuse          : 4   ; /* [7..4]  */
        unsigned int    mcu_ldo_enz           : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_MCU_LDO_CTRL;

/* Define the union U_SC_LSADC_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lsadc_en_a            : 1   ; /* [0]  */
        unsigned int    lsadc_en_b            : 1   ; /* [1]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    lsadc_reg             : 4   ; /* [7..4]  */
        unsigned int    lsadc_pd_ctrl         : 1   ; /* [8]  */
        unsigned int    lsadc_reset           : 1   ; /* [9]  */
        unsigned int    reserved_1            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_LSADC_CTRL;

/* Define the union U_SC_LSADC_DATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lsadc_rdata_a         : 8   ; /* [7..0]  */
        unsigned int    lsadc_rdata_b         : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_LSADC_DATA;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_SC_CTRL                  SC_CTRL                     ; /* 0x0 */
    volatile unsigned int               SC_SYSRES                   ; /* 0x4 */
    volatile U_SC_POWER_STAT            SC_POWER_STAT               ; /* 0x8 */
    volatile U_SC_L2_CACHE_SIZE_CFG     SC_L2_CACHE_SIZE_CFG        ; /* 0xc */
    volatile unsigned int               reserved_0[8]               ; /* 0x10~0x2c */
    volatile unsigned int               SC_BUS_CORE_PD_COUNT        ; /* 0x30 */
    volatile unsigned int               SC_CORE_PD_RST_COUNT        ; /* 0x34 */
    volatile unsigned int               SC_CORE_PWRUP_COUNT         ; /* 0x38 */
    volatile unsigned int               SC_CORE_PWRUP_RST_COUNT     ; /* 0x3c */
    volatile U_SC_LOW_POWER_CTRL        SC_LOW_POWER_CTRL           ; /* 0x40 */
    volatile U_SC_IO_REUSE_SEL          SC_IO_REUSE_SEL             ; /* 0x44 */
    volatile U_SC_CLKGATE_SRST_CTRL     SC_CLKGATE_SRST_CTRL        ; /* 0x48 */
    volatile unsigned int               reserved_1                  ; /* 0x4c */
    volatile U_SC_WDG_RST_CTRL          SC_WDG_RST_CTRL             ; /* 0x50 */
    volatile unsigned int               reserved_2                  ; /* 0x54 */
    volatile U_SC_DDRPHY_LP_EN          SC_DDRPHY_LP_EN             ; /* 0x58 */
    volatile unsigned int               reserved_3                  ; /* 0x5c */
    volatile U_SC_MCU_HPM_CTRL          SC_MCU_HPM_CTRL             ; /* 0x60 */
    volatile U_SC_MCU_HPM_STAT          SC_MCU_HPM_STAT             ; /* 0x64 */
    volatile U_SC_MCU_LDO_CTRL          SC_MCU_LDO_CTRL             ; /* 0x68 */
    volatile U_SC_LSADC_CTRL            SC_LSADC_CTRL               ; /* 0x6c */
    volatile U_SC_LSADC_DATA            SC_LSADC_DATA               ; /* 0x70 */
    volatile unsigned int               reserved_4[3]               ; /* 0x74~0x7c */
    volatile unsigned int               SC_GENm[24]                 ; /* 0x80~0xdc */
    volatile unsigned int               SC_GENx[8]                  ; /* 0xe0~0xfc */
    volatile unsigned int               reserved_5[67]              ; /* 0x100~0x208 */
    volatile unsigned int               SC_LOCKEN                   ; /* 0x20c */
    volatile unsigned int               reserved_6[820]             ; /* 0x210~0xedc */
    volatile unsigned int               SC_SYSID                    ; /* 0xee0 */
    volatile unsigned int               reserved_7[7]               ; /* 0xee4~0xefc */
    volatile unsigned int               SC_GENy[32]                 ; /* 0xf00~0xf7c */
} S_SYSCTRL_REGS_TYPE;

#endif /* __HI_REG_SYS_H__ */
