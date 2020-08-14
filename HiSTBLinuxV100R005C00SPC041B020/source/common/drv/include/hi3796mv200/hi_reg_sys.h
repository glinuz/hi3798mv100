//******************************************************************************
// Copyright     :  Copyright (C) 2016, Hisilicon Technologies Co., Ltd.
// File name     :  hi_reg_sys.h
// Author        :  fangqiubin 00179307
// Version       :  1.0
// Date          :  2016/09/13
// Description   :  项目描述信息
// Others        :  Generated automatically by nManager V4.1
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
        unsigned int    mcu_lp_subsys_iso     : 1   ; /* [3]  */
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
        unsigned int    uart_rx_sel           : 3   ; /* [2..0]  */
        unsigned int    uart_tx_sel           : 3   ; /* [5..3]  */
        unsigned int    clk_gpio_sel          : 1   ; /* [6]  */
        unsigned int    data_gpio_sel         : 2   ; /* [8..7]  */
        unsigned int    key_gpio_sel          : 2   ; /* [10..9]  */
        unsigned int    ir_gpio_sel           : 1   ; /* [11]  */
        unsigned int    stb_gpio_sel          : 1   ; /* [12]  */
        unsigned int    dq4_gpio_sel          : 1   ; /* [13]  */
        unsigned int    reserved_0            : 1   ; /* [14]  */
        unsigned int    i2c_sda_sel           : 2   ; /* [16..15]  */
        unsigned int    i2c_scl_sel           : 2   ; /* [18..17]  */
        unsigned int    dq3_gpio_sel          : 2   ; /* [20..19]  */
        unsigned int    hdmitx_hotplug_sel    : 1   ; /* [21]  */
        unsigned int    hdmitx_sda_sel        : 1   ; /* [22]  */
        unsigned int    hdmitx_scl_sel        : 1   ; /* [23]  */
        unsigned int    hdmitx_cec_sel        : 1   ; /* [24]  */
        unsigned int    lsadc1_sel            : 2   ; /* [26..25]  */
        unsigned int    lsadc0_sel            : 1   ; /* [27]  */
        unsigned int    led_data_DS0          : 1   ; /* [28]  */
        unsigned int    led_data_DS1          : 1   ; /* [29]  */
        unsigned int    led_data_DS2          : 1   ; /* [30]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
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
        unsigned int    hdmi_cec_cken         : 1   ; /* [19]  */
        unsigned int    hdmi_cec_srst_req     : 1   ; /* [20]  */
        unsigned int    hdmi_osc_cken         : 1   ; /* [21]  */
        unsigned int    hdmi_apb_cken         : 1   ; /* [22]  */
        unsigned int    hdmi_bus_srst_req     : 1   ; /* [23]  */
        unsigned int    hdmi_srst_req         : 1   ; /* [24]  */
        unsigned int    reserved_3            : 4   ; /* [28..25]  */
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
        unsigned int    led_clk_DS0           : 1   ; /* [0]  */
        unsigned int    led_clk_DS1           : 1   ; /* [1]  */
        unsigned int    led_clk_DS2           : 1   ; /* [2]  */
        unsigned int    led_key_DS0           : 1   ; /* [3]  */
        unsigned int    led_key_DS1           : 1   ; /* [4]  */
        unsigned int    led_key_SR            : 1   ; /* [5]  */
        unsigned int    led_key_PU            : 1   ; /* [6]  */
        unsigned int    led_key_PD            : 1   ; /* [7]  */
        unsigned int    gpio5_4_DS0           : 1   ; /* [8]  */
        unsigned int    gpio5_4_DS1           : 1   ; /* [9]  */
        unsigned int    gpio5_4_DS2           : 1   ; /* [10]  */
        unsigned int    lsadc1_DS0            : 1   ; /* [11]  */
        unsigned int    lsadc1_DS1            : 1   ; /* [12]  */
        unsigned int    i2c3_sda_DS0          : 1   ; /* [13]  */
        unsigned int    i2c3_sda_DS1          : 1   ; /* [14]  */
        unsigned int    i2c3_sda_SR           : 1   ; /* [15]  */
        unsigned int    i2c3_sda_PU           : 1   ; /* [16]  */
        unsigned int    i2c3_sda_PD           : 1   ; /* [17]  */
        unsigned int    i2c3_scl_DS0          : 1   ; /* [18]  */
        unsigned int    i2c3_scl_DS1          : 1   ; /* [19]  */
        unsigned int    i2c3_scl_SR           : 1   ; /* [20]  */
        unsigned int    i2c3_scl_PU           : 1   ; /* [21]  */
        unsigned int    i2c3_scl_PD           : 1   ; /* [22]  */
        unsigned int    gpio5_6_DS0           : 1   ; /* [23]  */
        unsigned int    gpio5_6_DS1           : 1   ; /* [24]  */
        unsigned int    gpio5_6_DS2           : 1   ; /* [25]  */
        unsigned int    ioctrl_htp_DS0        : 1   ; /* [26]  */
        unsigned int    ioctrl_htp_DS1        : 1   ; /* [27]  */
        unsigned int    ioctrl_htp_SR         : 1   ; /* [28]  */
        unsigned int    ioctrl_htp_PU         : 1   ; /* [29]  */
        unsigned int    ioctrl_htp_PD         : 1   ; /* [30]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
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

/* Define the union U_MEM_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rfs_ema               : 3   ; /* [2..0]  */
        unsigned int    rfs_emaw              : 2   ; /* [4..3]  */
        unsigned int    rft_emaa              : 3   ; /* [7..5]  */
        unsigned int    rft_emab              : 3   ; /* [10..8]  */
        unsigned int    rft_emasa             : 1   ; /* [11]  */
        unsigned int    rft_colldisn          : 1   ; /* [12]  */
        unsigned int    ras_ema               : 3   ; /* [15..13]  */
        unsigned int    ras_emaw              : 2   ; /* [17..16]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MEM_CTRL;

/* Define the union U_SC_IO_REUSE3_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ioctrl_sda_DS0        : 1   ; /* [0]  */
        unsigned int    ioctrl_sda_DS1        : 1   ; /* [1]  */
        unsigned int    ioctrl_sda_SR         : 1   ; /* [2]  */
        unsigned int    ioctrl_sda_PU         : 1   ; /* [3]  */
        unsigned int    ioctrl_sda_PD         : 1   ; /* [4]  */
        unsigned int    ioctrl_scl_DS0        : 1   ; /* [5]  */
        unsigned int    ioctrl_scl_DS1        : 1   ; /* [6]  */
        unsigned int    ioctrl_scl_SR         : 1   ; /* [7]  */
        unsigned int    ioctrl_scl_PU         : 1   ; /* [8]  */
        unsigned int    ioctrl_scl_PD         : 1   ; /* [9]  */
        unsigned int    ioctrl_cec_ds0        : 1   ; /* [10]  */
        unsigned int    ioctrl_cec_ds1        : 1   ; /* [11]  */
        unsigned int    ioctrl_cec_sw0        : 1   ; /* [12]  */
        unsigned int    ioctrl_cec_sw1        : 1   ; /* [13]  */
        unsigned int    ioctrl_cec_sw2        : 1   ; /* [14]  */
        unsigned int    ioctrl_cec_sw3        : 1   ; /* [15]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SC_IO_REUSE3_SEL;

/* Define the union U_LP_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lp_start              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LP_START;

/* Define the union U_SMCU_FW_ADDRESS_LOCK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    smcu_fw_address_lock  : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SMCU_FW_ADDRESS_LOCK;

/* Define the union U_DDR_PHY_PARA_LOCK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddr_phy_para_lock     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDR_PHY_PARA_LOCK;

/* Define the union U_SCPU_LOCK0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scpu_lock0            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SCPU_LOCK0;

/* Define the union U_SCPU_LOCK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scpu_lock1            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SCPU_LOCK1;

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
    volatile U_MEM_CTRL             MEM_CTRL                 ; /* 0x100 */
    volatile U_SC_IO_REUSE3_SEL     SC_IO_REUSE3_SEL         ; /* 0x104 */
    volatile unsigned int           reserved_5[65]                     ; /* 0x108~0x208 */
    volatile unsigned int           SC_LOCKEN                ; /* 0x20c */
    volatile unsigned int           CPU_CEC_INFO_0           ; /* 0x210 */
    volatile unsigned int           CPU_CEC_INFO_1           ; /* 0x214 */
    volatile unsigned int           CPU_MCU_SWITCH_INFO      ; /* 0x218 */
    volatile unsigned int           CPU_HDMITX_RESERVED_0    ; /* 0x21c */
    volatile unsigned int           CPU_HDMITX_RESERVED_1    ; /* 0x220 */
    volatile unsigned int           CPU_HDMITX_RESERVED_2    ; /* 0x224 */
    volatile unsigned int           CPU_HDMITX_RESERVED_3    ; /* 0x228 */
    volatile unsigned int           CPU_HDMITX_RESERVED_4    ; /* 0x22c */
    volatile unsigned int           MCU_CEC_INFO_0           ; /* 0x230 */
    volatile unsigned int           MCU_CEC_INFO_1           ; /* 0x234 */
    volatile unsigned int           MCU_CPU_SWITCH_INFO      ; /* 0x238 */
    volatile unsigned int           MCU_HDMITX_RESERVED_0    ; /* 0x23c */
    volatile unsigned int           MCU_HDMITX_RESERVED_1    ; /* 0x240 */
    volatile unsigned int           MCU_HDMITX_RESERVED_2    ; /* 0x244 */
    volatile unsigned int           MCU_HDMITX_RESERVED_3    ; /* 0x248 */
    volatile unsigned int           MCU_HDMITX_RESERVED_4    ; /* 0x24c */
    volatile unsigned int           reserved_6[108]                     ; /* 0x250~0x3fc */
    volatile unsigned int           DDR_PHY_PARA[32]         ; /* 0x400~0x47c */
    volatile unsigned int           reserved_7[224]                     ; /* 0x480~0x7fc */
    volatile unsigned int           LPDS_OPER_DATA[32]       ; /* 0x800~0x87c */
    volatile unsigned int           reserved_8[96]                     ; /* 0x880~0x9fc */
    volatile unsigned int           SMCU_FW_ADDRESS_0        ; /* 0xa00 */
    volatile unsigned int           SMCU_FW_LENGTH_0         ; /* 0xa04 */
    volatile U_LP_START             LP_START                 ; /* 0xa08 */
    volatile unsigned int           reserved_9[61]                     ; /* 0xa0c~0xafc */
    volatile unsigned int           SMCU_FW_ADDRESS          ; /* 0xb00 */
    volatile unsigned int           SMCU_FW_LENGTH           ; /* 0xb04 */
    volatile unsigned int           SMCU_GEN0                ; /* 0xb08 */
    volatile unsigned int           SMCU_GEN1                ; /* 0xb0c */
    volatile unsigned int           reserved_10[4]                     ; /* 0xb10~0xb1c */
    volatile U_SMCU_FW_ADDRESS_LOCK   SMCU_FW_ADDRESS_LOCK   ; /* 0xb20 */
    volatile unsigned int           reserved_11[3]                     ; /* 0xb24~0xb2c */
    volatile U_DDR_PHY_PARA_LOCK    DDR_PHY_PARA_LOCK        ; /* 0xb30 */
    volatile unsigned int           reserved_12[3]                     ; /* 0xb34~0xb3c */
    volatile U_SCPU_LOCK0           SCPU_LOCK0               ; /* 0xb40 */
    volatile U_SCPU_LOCK1           SCPU_LOCK1               ; /* 0xb44 */
    volatile unsigned int           reserved_13[14]                     ; /* 0xb48~0xb7c */
    volatile unsigned int           SCPU_GEN0                ; /* 0xb80 */
    volatile unsigned int           SCPU_GEN1                ; /* 0xb84 */
    volatile unsigned int           SCPU_GEN2                ; /* 0xb88 */
    volatile unsigned int           SCPU_GEN3                ; /* 0xb8c */
    volatile unsigned int           SCPU_GEN4                ; /* 0xb90 */
    volatile unsigned int           SCPU_GEN5                ; /* 0xb94 */
    volatile unsigned int           SCPU_GEN6                ; /* 0xb98 */
    volatile unsigned int           SCPU_GEN7                ; /* 0xb9c */
    volatile unsigned int           reserved_14[24]                     ; /* 0xba0~0xbfc */
    volatile unsigned int           NONCE_0                  ; /* 0xc00 */
    volatile unsigned int           NONCE_1                  ; /* 0xc04 */
    volatile unsigned int           NONCE_2                  ; /* 0xc08 */
    volatile unsigned int           NONCE_3                  ; /* 0xc0c */
    volatile unsigned int           reserved_15[177]                     ; /* 0xc10~0xed0 */
    volatile unsigned int           DDRCA_RANDOM0_DDRC       ; /* 0xed4 */
    volatile unsigned int           DDRCA_RANDOM1_DDRC       ; /* 0xed8 */
    volatile unsigned int           reserved_16                     ; /* 0xedc */
    volatile unsigned int           SC_SYSID                 ; /* 0xee0 */
    volatile unsigned int           reserved_17[7]                     ; /* 0xee4~0xefc */
    volatile unsigned int           SC_GENy[32]              ; /* 0xf00~0xf7c */

} S_SYSCTRL_REGS_TYPE;

#endif /* __HI_REG_SYS_H__ */
