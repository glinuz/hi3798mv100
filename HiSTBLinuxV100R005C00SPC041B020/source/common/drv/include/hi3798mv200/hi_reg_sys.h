// ****************************************************************************** 
// Copyright     :  Copyright (C) 2016, Hisilicon Technologies Co. Ltd.
// File name     :  hi_reg_sys.h
// Project line  :  媒体产品线
// Department    :  STB开发部
// Author        :  fangqiubin 00179307
// Version       :  1.0
// Date          :  2013/1/17
// Description   :  项目描述信息
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  fangqiubin 00179307 2016/01/20 10:54:38 Create file
// ******************************************************************************

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
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
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

/* Define the union U_SC_LOW_POWER_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bus_discon_timeout_clr : 1   ; /* [0]  */
        unsigned int    reserved_0            : 2   ; /* [2..1]  */
        unsigned int    mcu_lp_subsys_iso_n   : 1   ; /* [3]  */
        unsigned int    sys_power_status      : 4   ; /* [7..4]  */
        unsigned int    reserved_1            : 8   ; /* [15..8]  */
        unsigned int    cpu_standbywfi        : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
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
        unsigned int    uart_rx_sel           : 1   ; /* [0]  */
        unsigned int    uart_tx_sel           : 1   ; /* [1]  */
        unsigned int    clk_gpio_sel          : 1   ; /* [2]  */
        unsigned int    data_gpio_sel         : 2   ; /* [4..3]  */
        unsigned int    key_gpio_sel          : 2   ; /* [6..5]  */
        unsigned int    ir_gpio_sel           : 1   ; /* [7]  */
        unsigned int    stb_gpio_sel          : 1   ; /* [8]  */
        unsigned int    lsadc0_ie             : 1   ; /* [9]  */
        unsigned int    lsadc1_ie             : 1   ; /* [10]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    led_data_DS0          : 1   ; /* [12]  */
        unsigned int    led_data_DS1          : 1   ; /* [13]  */
        unsigned int    led_data_DS2          : 1   ; /* [14]  */
        unsigned int    led_data_PD           : 1   ; /* [15]  */
        unsigned int    led_data_PU           : 1   ; /* [16]  */
        unsigned int    led_data_SR           : 1   ; /* [17]  */
        unsigned int    led_key_DS0           : 1   ; /* [18]  */
        unsigned int    led_key_DS1           : 1   ; /* [19]  */
        unsigned int    led_key_DS2           : 1   ; /* [20]  */
        unsigned int    led_key_PD            : 1   ; /* [21]  */
        unsigned int    led_key_PU            : 1   ; /* [22]  */
        unsigned int    led_key_SR            : 1   ; /* [23]  */
        unsigned int    gpio5_3_DS0           : 1   ; /* [24]  */
        unsigned int    gpio5_3_DS1           : 1   ; /* [25]  */
        unsigned int    gpio5_3_DS2           : 1   ; /* [26]  */
        unsigned int    gpio5_3_PD            : 1   ; /* [27]  */
        unsigned int    gpio5_3_PU            : 1   ; /* [28]  */
        unsigned int    gpio5_3_SR            : 1   ; /* [29]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
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
        unsigned int    reserved_2            : 2   ; /* [15..14]  */
        unsigned int    wifi_cken             : 1   ; /* [16]  */
        unsigned int    wificlk_sel           : 2   ; /* [18..17]  */
        unsigned int    reserved_3            : 10  ; /* [28..19]  */
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

/* Define the union U_SC_IO_REUSE2_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gpio5_4_DS0           : 1   ; /* [0]  */
        unsigned int    gpio5_4_DS1           : 1   ; /* [1]  */
        unsigned int    gpio5_4_DS2           : 1   ; /* [2]  */
        unsigned int    gpio5_4_PD            : 1   ; /* [3]  */
        unsigned int    gpio5_4_PU            : 1   ; /* [4]  */
        unsigned int    gpio5_4_SR            : 1   ; /* [5]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_IO_REUSE2_SEL;

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
        unsigned int    lsadc_rdata_a         : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    lsadc_rdata_b         : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_LSADC_DATA;

/* Define the union U_DDR_RNG_VLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddrca_rng_valid       : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDR_RNG_VLD;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_SC_CTRL              SC_CTRL                  ; /* 0x0 */
    volatile unsigned int           SC_SYSRES                ; /* 0x4 */
    volatile U_SC_POWER_STAT        SC_POWER_STAT            ; /* 0x8 */
    volatile unsigned int           reserved_0[9]                     ; /* 0xc~0x2c */
    volatile unsigned int           SC_BUS_CORE_PD_COUNT     ; /* 0x30 */
    volatile unsigned int           SC_CORE_PD_RST_COUNT     ; /* 0x34 */
    volatile unsigned int           SC_CORE_PWRUP_COUNT      ; /* 0x38 */
    volatile unsigned int           SC_CORE_PWRUP_RST_COUNT  ; /* 0x3c */
    volatile U_SC_LOW_POWER_CTRL    SC_LOW_POWER_CTRL        ; /* 0x40 */
    volatile U_SC_IO_REUSE_SEL      SC_IO_REUSE_SEL          ; /* 0x44 */
    volatile U_SC_CLKGATE_SRST_CTRL   SC_CLKGATE_SRST_CTRL   ; /* 0x48 */
    volatile unsigned int           reserved_1                     ; /* 0x4c */
    volatile U_SC_WDG_RST_CTRL      SC_WDG_RST_CTRL          ; /* 0x50 */
    volatile unsigned int           reserved_2                     ; /* 0x54 */
    volatile U_SC_DDRPHY_LP_EN      SC_DDRPHY_LP_EN          ; /* 0x58 */
    volatile U_SC_IO_REUSE2_SEL     SC_IO_REUSE2_SEL         ; /* 0x5c */
    volatile unsigned int           reserved_3[2]                     ; /* 0x60~0x64 */
    volatile U_SC_MCU_LDO_CTRL      SC_MCU_LDO_CTRL          ; /* 0x68 */
    volatile U_SC_LSADC_CTRL        SC_LSADC_CTRL            ; /* 0x6c */
    volatile U_SC_LSADC_DATA        SC_LSADC_DATA            ; /* 0x70 */
    volatile unsigned int           reserved_4[3]                     ; /* 0x74~0x7c */
    volatile unsigned int           SC_GENm[24]              ; /* 0x80~0xdc */
    volatile unsigned int           SC_GENx[8]               ; /* 0xe0~0xfc */
    volatile unsigned int           reserved_5[67]                     ; /* 0x100~0x208 */
    volatile unsigned int           SC_LOCKEN                ; /* 0x20c */
    volatile unsigned int           reserved_6[816]                     ; /* 0x210~0xecc */
    volatile U_DDR_RNG_VLD          DDR_RNG_VLD              ; /* 0xed0 */
    volatile unsigned int           DDRCA_RANDOM0_DDRC       ; /* 0xed4 */
    volatile unsigned int           DDRCA_RANDOM1_DDRC       ; /* 0xed8 */
    volatile unsigned int           reserved_7                     ; /* 0xedc */
    volatile unsigned int           SC_SYSID                 ; /* 0xee0 */
    volatile unsigned int           reserved_8[7]                     ; /* 0xee4~0xefc */
    volatile unsigned int           SC_GENy[32]              ; /* 0xf00~0xf7c */

} S_SYSCTRL_REGS_TYPE;

#endif /* __HI_REG_SYS_H__ */
