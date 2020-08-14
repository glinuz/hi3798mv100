//****************************************************************************** 
// Copyright     :  Copyright (C) 2014, Hisilicon Technologies Co., Ltd.
// File name     :  hi_reg_sys.h
// Author        :  fangqiubin 00179307
// Version       :  1.0 
// Date          :  2014-07-21
// Description   :  Define all registers/tables for SYS_CTRL
// Others        :  Generated automatically by nManager V4.0 
// History       :  fangqiubin 00179307 2014-07-21 Create file
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
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    remapclear            : 1   ; /* [8]  */
        unsigned int    remapstat             : 1   ; /* [9]  */
        unsigned int    reserved_1            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_CTRL;

/* Define the union U_SC_LOW_POWER_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    stb_poweroff          : 1   ; /* [1]  */
        unsigned int    reserved_1            : 1   ; /* [2]  */
        unsigned int    mcu_lp_subsys_iso     : 1   ; /* [3]  */
        unsigned int    reserved_2            : 2   ; /* [5..4]  */
        unsigned int    bus_core_pd_idlereq   : 1   ; /* [6]  */
        unsigned int    bus_core_pd_idle      : 1   ; /* [7]  */
        unsigned int    bus_core_pd_idleack   : 1   ; /* [8]  */
        unsigned int    core_pwr_active       : 1   ; /* [9]  */
        unsigned int    rng_mcu_flag          : 1   ; /* [10]  */
        unsigned int    reserved_3            : 21  ; /* [31..11]  */
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
        unsigned int    uart_rxd_gpio_sel     : 1   ; /* [0]  */
        unsigned int    uart_txd_gpio_sel     : 1   ; /* [1]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    clk_gpio_sel          : 1   ; /* [4]  */
        unsigned int    data_gpio_sel         : 1   ; /* [5]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    csn0_gpio_sel         : 2   ; /* [9..8]  */
        unsigned int    ir_gpio_sel           : 1   ; /* [10]  */
        unsigned int    reserved_2            : 2   ; /* [12..11]  */
        unsigned int    stb_gpio_sel          : 1   ; /* [13]  */
        unsigned int    reserved_3            : 2   ; /* [15..14]  */
        unsigned int    led_clk_padctrl_PD    : 1   ; /* [16]  */
        unsigned int    led_data_padctrl_PD   : 1   ; /* [17]  */
        unsigned int    reserved_4            : 1   ; /* [18]  */
        unsigned int    sim0_clk_padctrl_SR   : 1   ; /* [19]  */
        unsigned int    sim0_clk_padctrl_DS   : 2   ; /* [21..20]  */
        unsigned int    sim0_clk_padctrl_PD   : 1   ; /* [22]  */
        unsigned int    sim0_data_padctrl_PD  : 1   ; /* [23]  */
        unsigned int    sim0_det_padctrl_PD   : 1   ; /* [24]  */
        unsigned int    reserved_5            : 7   ; /* [31..25]  */
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
        unsigned int    mce_srst_req          : 1   ; /* [1]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    ir_cken               : 1   ; /* [4]  */
        unsigned int    ir_srst_req           : 1   ; /* [5]  */
        unsigned int    timer01_cken          : 1   ; /* [6]  */
        unsigned int    timer01_srst_req      : 1   ; /* [7]  */
        unsigned int    led_cken              : 1   ; /* [8]  */
        unsigned int    led_srst_req          : 1   ; /* [9]  */
        unsigned int    reserved_1            : 2   ; /* [11..10]  */
        unsigned int    uart_cken             : 1   ; /* [12]  */
        unsigned int    uart_srst_req         : 1   ; /* [13]  */
        unsigned int    reserved_2            : 2   ; /* [15..14]  */
        unsigned int    reserved_3            : 12  ; /* [27..16]  */
        unsigned int    pd_rst_req            : 1   ; /* [28]  */
        unsigned int    reserved_4            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_CLKGATE_SRST_CTRL;

/* Define the union U_SC_CA_RST_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ca_rst_ctrl           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_CA_RST_CTRL;

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
        unsigned int    ddrphy_lp_en          : 2   ; /* [1..0]  */
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
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_MCU_LDO_CTRL;

/* Define the union U_SC_GPIO_OD_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gpio5_0_od_sel        : 1   ; /* [0]  */
        unsigned int    gpio5_1_od_sel        : 1   ; /* [1]  */
        unsigned int    gpio5_2_od_sel        : 1   ; /* [2]  */
        unsigned int    reserved_0            : 2   ; /* [4..3]  */
        unsigned int    gpio5_5_od_sel        : 1   ; /* [5]  */
        unsigned int    gpio5_6_od_sel        : 1   ; /* [6]  */
        unsigned int    reserved_1            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_GPIO_OD_CTRL;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_SC_CTRL              SC_CTRL                  ; /* 0x0 */
    volatile unsigned int           SC_SYSRES                ; /* 0x4 */
    volatile unsigned int           reserved_0[14]                     ; /* 0x8~0x3c */
    volatile U_SC_LOW_POWER_CTRL    SC_LOW_POWER_CTRL        ; /* 0x40 */
    volatile U_SC_IO_REUSE_SEL      SC_IO_REUSE_SEL          ; /* 0x44 */
    volatile U_SC_CLKGATE_SRST_CTRL   SC_CLKGATE_SRST_CTRL   ; /* 0x48 */
    volatile U_SC_CA_RST_CTRL       SC_CA_RST_CTRL           ; /* 0x4c */
    volatile U_SC_WDG_RST_CTRL      SC_WDG_RST_CTRL          ; /* 0x50 */
    volatile unsigned int           reserved_1                     ; /* 0x54 */
    volatile U_SC_DDRPHY_LP_EN      SC_DDRPHY_LP_EN          ; /* 0x58 */
    volatile unsigned int           reserved_2                     ; /* 0x5c */
    volatile U_SC_MCU_HPM_CTRL      SC_MCU_HPM_CTRL          ; /* 0x60 */
    volatile U_SC_MCU_HPM_STAT      SC_MCU_HPM_STAT          ; /* 0x64 */
    volatile U_SC_MCU_LDO_CTRL      SC_MCU_LDO_CTRL          ; /* 0x68 */
    volatile unsigned int           reserved_3[5]                     ; /* 0x6c~0x7c */
    volatile unsigned int           SC_GEN0                  ; /* 0x80 */
    volatile unsigned int           SC_GEN1                  ; /* 0x84 */
    volatile unsigned int           SC_GEN2                  ; /* 0x88 */
    volatile unsigned int           SC_GEN3                  ; /* 0x8c */
    volatile unsigned int           SC_GEN4                  ; /* 0x90 */
    volatile unsigned int           SC_GEN5                  ; /* 0x94 */
    volatile unsigned int           SC_GEN6                  ; /* 0x98 */
    volatile unsigned int           SC_GEN7                  ; /* 0x9c */
    volatile unsigned int           SC_GEN8                  ; /* 0xa0 */
    volatile unsigned int           SC_GEN9                  ; /* 0xa4 */
    volatile unsigned int           SC_GEN10                 ; /* 0xa8 */
    volatile unsigned int           SC_GEN11                 ; /* 0xac */
    volatile unsigned int           SC_GEN12                 ; /* 0xb0 */
    volatile unsigned int           SC_GEN13                 ; /* 0xb4 */
    volatile unsigned int           SC_GEN14                 ; /* 0xb8 */
    volatile unsigned int           SC_GEN15                 ; /* 0xbc */
    volatile unsigned int           SC_GEN16                 ; /* 0xc0 */
    volatile unsigned int           SC_GEN17                 ; /* 0xc4 */
    volatile unsigned int           SC_GEN18                 ; /* 0xc8 */
    volatile unsigned int           SC_GEN19                 ; /* 0xcc */
    volatile unsigned int           SC_GEN20                 ; /* 0xd0 */
    volatile unsigned int           SC_GEN21                 ; /* 0xd4 */
    volatile unsigned int           SC_GEN22                 ; /* 0xd8 */
    volatile unsigned int           SC_GEN23                 ; /* 0xdc */
    volatile unsigned int           SC_GEN24                 ; /* 0xe0 */
    volatile unsigned int           SC_GEN25                 ; /* 0xe4 */
    volatile unsigned int           SC_GEN26                 ; /* 0xe8 */
    volatile unsigned int           SC_GEN27                 ; /* 0xec */
    volatile unsigned int           SC_GEN28                 ; /* 0xf0 */
    volatile unsigned int           SC_GEN29                 ; /* 0xf4 */
    volatile unsigned int           SC_GEN30                 ; /* 0xf8 */
    volatile unsigned int           SC_GEN31                 ; /* 0xfc */
    volatile U_SC_GPIO_OD_CTRL      SC_GPIO_OD_CTRL          ; /* 0x100 */
    volatile unsigned int           reserved_4[66]                     ; /* 0x104~0x208 */
    volatile unsigned int           SC_LOCKEN                ; /* 0x20c */
    volatile unsigned int           reserved_5[820]                     ; /* 0x210~0xedc */
    volatile unsigned int           SC_SYSID                 ; /* 0xee0 */
    volatile unsigned int           reserved_6[7]                     ; /* 0xee4~0xefc */
    volatile unsigned int           SC_GEN32                 ; /* 0xf00 */
    volatile unsigned int           SC_GEN33                 ; /* 0xf04 */
    volatile unsigned int           SC_GEN34                 ; /* 0xf08 */
    volatile unsigned int           SC_GEN35                 ; /* 0xf0c */
    volatile unsigned int           SC_GEN36                 ; /* 0xf10 */
    volatile unsigned int           SC_GEN37                 ; /* 0xf14 */
    volatile unsigned int           SC_GEN38                 ; /* 0xf18 */
    volatile unsigned int           SC_GEN39                 ; /* 0xf1c */
    volatile unsigned int           SC_GEN40                 ; /* 0xf20 */
    volatile unsigned int           SC_GEN41                 ; /* 0xf24 */
    volatile unsigned int           SC_GEN42                 ; /* 0xf28 */
    volatile unsigned int           SC_GEN43                 ; /* 0xf2c */
    volatile unsigned int           SC_GEN44                 ; /* 0xf30 */
    volatile unsigned int           SC_GEN45                 ; /* 0xf34 */
    volatile unsigned int           SC_GEN46                 ; /* 0xf38 */
    volatile unsigned int           SC_GEN47                 ; /* 0xf3c */
    volatile unsigned int           SC_GEN48                 ; /* 0xf40 */
    volatile unsigned int           SC_GEN49                 ; /* 0xf44 */
    volatile unsigned int           SC_GEN50                 ; /* 0xf48 */
    volatile unsigned int           SC_GEN51                 ; /* 0xf4c */
    volatile unsigned int           SC_GEN52                 ; /* 0xf50 */
    volatile unsigned int           SC_GEN53                 ; /* 0xf54 */
    volatile unsigned int           SC_GEN54                 ; /* 0xf58 */
    volatile unsigned int           SC_GEN55                 ; /* 0xf5c */
    volatile unsigned int           SC_GEN56                 ; /* 0xf60 */
    volatile unsigned int           SC_GEN57                 ; /* 0xf64 */
    volatile unsigned int           SC_GEN58                 ; /* 0xf68 */
    volatile unsigned int           SC_GEN59                 ; /* 0xf6c */
    volatile unsigned int           SC_GEN60                 ; /* 0xf70 */
    volatile unsigned int           SC_GEN61                 ; /* 0xf74 */
    volatile unsigned int           SC_GEN62                 ; /* 0xf78 */
    volatile unsigned int           SC_GEN63                 ; /* 0xf7c */

} S_SYSCTRL_REGS_TYPE;

#endif /* __HI_REG_SYS_H__ */
