//****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co., Ltd.
// File name     :  hi_reg_sys.h
// Author        :  fangqiubin 00179307
// Version       :  1.0 
// Date          :  2015-03-06
// Description   :  Define all registers/tables for SYS_CTRL
// Others        :  Generated automatically by nManager V4.0 
// History       :  fangqiubin 00179307 2015-03-06 Create file
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
        unsigned int    stb_gpio_sel          : 1   ; /* [0]  */
        unsigned int    data_gpio_sel         : 1   ; /* [1]  */
        unsigned int    clk_gpio_sel          : 1   ; /* [2]  */
        unsigned int    key0_gpio_sel         : 1   ; /* [3]  */
        unsigned int    ir_gpio_sel           : 1   ; /* [4]  */
        unsigned int    gpio_sel              : 1   ; /* [5]  */
        unsigned int    uart_rxd_gpio_sel     : 1   ; /* [6]  */
        unsigned int    uart_txd_gpio_sel     : 1   ; /* [7]  */
        unsigned int    pwm_gpio_sel          : 1   ; /* [8]  */
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
        unsigned int    reserved_2            : 14  ; /* [27..14]  */
        unsigned int    pd_rst_req            : 1   ; /* [28]  */
        unsigned int    uart_sel              : 1   ; /* [29]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
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
        unsigned int    gpio5_3_od_sel        : 1   ; /* [3]  */
        unsigned int    gpio5_4_od_sel        : 1   ; /* [4]  */
        unsigned int    gpio5_5_od_sel        : 1   ; /* [5]  */
        unsigned int    gpio5_6_od_sel        : 1   ; /* [6]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_GPIO_OD_CTRL;

/* Define the union U_DDRCA_EN_DDRC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddrca_en_ddrc         : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDRCA_EN_DDRC;

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
    volatile unsigned int           reserved_1                     ; /* 0x4c */
    volatile U_SC_WDG_RST_CTRL      SC_WDG_RST_CTRL          ; /* 0x50 */
    volatile unsigned int           reserved_2                     ; /* 0x54 */
    volatile U_SC_DDRPHY_LP_EN      SC_DDRPHY_LP_EN          ; /* 0x58 */
    volatile unsigned int           reserved_3                     ; /* 0x5c */
    volatile U_SC_MCU_HPM_CTRL      SC_MCU_HPM_CTRL          ; /* 0x60 */
    volatile U_SC_MCU_HPM_STAT      SC_MCU_HPM_STAT          ; /* 0x64 */
    volatile U_SC_MCU_LDO_CTRL      SC_MCU_LDO_CTRL          ; /* 0x68 */
    volatile unsigned int           reserved_4[5]                     ; /* 0x6c~0x7c */
	volatile unsigned int           SC_GEN0;    /* 0x80~0xfc */
    volatile unsigned int           SC_GEN1;
    volatile unsigned int           SC_GEN2;
    volatile unsigned int           SC_GEN3;
    volatile unsigned int           SC_GEN4;
    volatile unsigned int           SC_GEN5;
    volatile unsigned int           SC_GEN6;
    volatile unsigned int           SC_GEN7;
    volatile unsigned int           SC_GEN8;
    volatile unsigned int           SC_GEN9;
    volatile unsigned int           SC_GEN10;
    volatile unsigned int           SC_GEN11;
    volatile unsigned int           SC_GEN12;
    volatile unsigned int           SC_GEN13;
    volatile unsigned int           SC_GEN14;
    volatile unsigned int           SC_GEN15;
    volatile unsigned int           SC_GEN16;
    volatile unsigned int           SC_GEN17;
    volatile unsigned int           SC_GEN18;
    volatile unsigned int           SC_GEN19;
    volatile unsigned int           SC_GEN20;
    volatile unsigned int           SC_GEN21;
    volatile unsigned int           SC_GEN22;
    volatile unsigned int           SC_GEN23;
    volatile unsigned int           SC_GEN24;
    volatile unsigned int           SC_GEN25;
    volatile unsigned int           SC_GEN26;
    volatile unsigned int           SC_GEN27;
    volatile unsigned int           SC_GEN28;
    volatile unsigned int           SC_GEN29;
    volatile unsigned int           SC_GEN30;
    volatile unsigned int           SC_GEN31;

    volatile U_SC_GPIO_OD_CTRL      SC_GPIO_OD_CTRL          ; /* 0x100 */
    volatile unsigned int           reserved_5[66]                     ; /* 0x104~0x208 */
    volatile unsigned int           SC_LOCKEN                ; /* 0x20c */
    volatile unsigned int           reserved_6[820]                     ; /* 0x210~0xedc */
    volatile unsigned int           SC_SYSID                 ; /* 0xee0 */
    volatile unsigned int           reserved_7[7]                     ; /* 0xee4~0xefc */
    volatile unsigned int           SC_GEN32;                /* 0xf00~0xf7c */
    volatile unsigned int           SC_GEN33;
    volatile unsigned int           SC_GEN34;
    volatile unsigned int           SC_GEN35;
    volatile unsigned int           SC_GEN36;
    volatile unsigned int           SC_GEN37;
    volatile unsigned int           SC_GEN38;
    volatile unsigned int           SC_GEN39;
    volatile unsigned int           SC_GEN40;
    volatile unsigned int           SC_GEN41;
    volatile unsigned int           SC_GEN42;
    volatile unsigned int           SC_GEN43;
    volatile unsigned int           SC_GEN44;
    volatile unsigned int           SC_GEN45;
    volatile unsigned int           SC_GEN46;
    volatile unsigned int           SC_GEN47;
    volatile unsigned int           SC_GEN48;
    volatile unsigned int           SC_GEN49;
    volatile unsigned int           SC_GEN50;
    volatile unsigned int           SC_GEN51;
    volatile unsigned int           SC_GEN52;
    volatile unsigned int           SC_GEN53;
    volatile unsigned int           SC_GEN54;
    volatile unsigned int           SC_GEN55;
    volatile unsigned int           SC_GEN56;
    volatile unsigned int           SC_GEN57;
    volatile unsigned int           SC_GEN58;
    volatile unsigned int           SC_GEN59;
    volatile unsigned int           SC_GEN60;
    volatile unsigned int           SC_GEN61;
    volatile unsigned int           SC_GEN62;
    volatile unsigned int           SC_GEN63;

    volatile unsigned int           reserved_8[4]                     ; /* 0xf80~0xf8c */
    volatile unsigned int           DDRCA_RANDOM0_DDRC       ; /* 0xf90 */
    volatile unsigned int           DDRCA_RANDOM1_DDRC       ; /* 0xf94 */
    volatile U_DDRCA_EN_DDRC        DDRCA_EN_DDRC            ; /* 0xf98 */

} S_SYSCTRL_REGS_TYPE;

#endif /* __HI_REG_SYS_H__ */
