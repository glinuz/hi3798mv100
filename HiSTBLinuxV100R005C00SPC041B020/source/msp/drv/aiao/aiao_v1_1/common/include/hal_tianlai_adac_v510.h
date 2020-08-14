/***********************************************************************************/
#ifndef __S5_AUD_TIANLAI_HAL_H
#define __S5_AUD_TIANLAI_HAL_H

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#include "hi_type.h"

#define TIANLAI_BASE_ADDR_V100B 0xFF280000
#define TIANLAI_BASE_ADDR_V100  0xF8B40000
#define TIANLAI_BASE_ADDR_V600  0xFF2d0000
#define TIANLAI_BASE_ADDR_LV500 0xFF2d0000
#define TIANLAI_BASE_ADDR_V500 0xFF2d0000
#define TIANLAI_BASE_ADDR_V620 0xFF2d0000
#define TIANLAI_REGS_SIZE         0x1000
#define S5_CRG_BASE_ADDR        0xF8A22000
#define S5_CRG70_AIAO_OFFSET    0x0118
#define S5_CRG69_AIAO_OFFSET    0x0114
#define S5_CRG52_DSP_OFFSET     0x00D0

/* Define the union U_TOP_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    switch_info           : 2   ; /* [1..0]  */
        unsigned int    Reserved_0            : 14  ; /* [15..2]  */
        unsigned int    edition_info          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TOP_INFO;

/* Define the union U_ADC_CFG_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_3            : 8   ; /* [7..0]  */
        unsigned int    i2s2_fs_sel           : 5   ; /* [12..8]  */
        unsigned int    i2s1_fs_sel           : 5   ; /* [17..13]  */
        unsigned int    dig_loop              : 1   ; /* [18]  */
        unsigned int    dig_bypass            : 1   ; /* [19]  */
        unsigned int    i2s2_data_bits        : 2   ; /* [21..20]  */
        unsigned int    i2s1_data_bits        : 2   ; /* [23..22]  */
        unsigned int    adcr_enable           : 1   ; /* [24]  */
        unsigned int    adcl_enable           : 1   ; /* [25]  */
        unsigned int    Reserved_2            : 2   ; /* [27..26]  */
        unsigned int    adcr_rst_n            : 1   ; /* [28]  */
        unsigned int    adcl_rst_n            : 1   ; /* [29]  */
        unsigned int    Reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ADC_CFG_VALUE;

/* Define the union U_ADC_REG_0_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adcr_lrsel            : 1   ; /* [0]  */
        unsigned int    adcr_i2ssel           : 1   ; /* [1]  */
        unsigned int    adcl_lrsel            : 1   ; /* [2]  */
        unsigned int    adcl_i2ssel           : 1   ; /* [3]  */
        unsigned int    Reserved_4            : 10  ; /* [13..4]  */
        unsigned int    adcr_hpf_en           : 1   ; /* [14]  */
        unsigned int    adcl_hpf_en           : 1   ; /* [15]  */
        unsigned int    adcr_vol              : 7   ; /* [22..16]  */
        unsigned int    adcr_mute             : 1   ; /* [23]  */
        unsigned int    adcl_vol              : 7   ; /* [30..24]  */
        unsigned int    adcl_mute             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ADC_REG_0_VALUE;

/* Define the union U_ADC_REG_1_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adcr2dacr0_mix_vol    : 7   ; /* [6..0]  */
        unsigned int    adcr2dacr0_mix_en     : 1   ; /* [7]  */
        unsigned int    adcl2dacr0_mix_vol    : 7   ; /* [14..8]  */
        unsigned int    adcl2dacr0_mix_en     : 1   ; /* [15]  */
        unsigned int    adcr2dacl0_mix_vol    : 7   ; /* [22..16]  */
        unsigned int    adcr2dacl0_mix_en     : 1   ; /* [23]  */
        unsigned int    adcl2dacl0_mix_vol    : 7   ; /* [30..24]  */
        unsigned int    adcl2dacl0_mix_en     : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ADC_REG_1_VALUE;

/* Define the union U_ADC_REG_2_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adcr2dacr1_mix_vol    : 7   ; /* [6..0]  */
        unsigned int    adcr2dacr1_mix_en     : 1   ; /* [7]  */
        unsigned int    adcl2dacr1_mix_vol    : 7   ; /* [14..8]  */
        unsigned int    adcl2dacr1_mix_en     : 1   ; /* [15]  */
        unsigned int    adcr2dacl1_mix_vol    : 7   ; /* [22..16]  */
        unsigned int    adcr2dacl1_mix_en     : 1   ; /* [23]  */
        unsigned int    adcl2dacl1_mix_vol    : 7   ; /* [30..24]  */
        unsigned int    adcl2dacl1_mix_en     : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ADC_REG_2_VALUE;

/* Define the union U_ADC_REG_3_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adcr2dacr2_mix_vol    : 7   ; /* [6..0]  */
        unsigned int    adcr2dacr2_mix_en     : 1   ; /* [7]  */
        unsigned int    adcl2dacr2_mix_vol    : 7   ; /* [14..8]  */
        unsigned int    adcl2dacr2_mix_en     : 1   ; /* [15]  */
        unsigned int    adcr2dacl2_mix_vol    : 7   ; /* [22..16]  */
        unsigned int    adcr2dacl2_mix_en     : 1   ; /* [23]  */
        unsigned int    adcl2dacl2_mix_vol    : 7   ; /* [30..24]  */
        unsigned int    adcl2dacl2_mix_en     : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ADC_REG_3_VALUE;

/* Define the union U_ADC_REG_4_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adcr2dacr3_mix_vol    : 7   ; /* [6..0]  */
        unsigned int    adcr2dacr3_mix_en     : 1   ; /* [7]  */
        unsigned int    adcl2dacr3_mix_vol    : 7   ; /* [14..8]  */
        unsigned int    adcl2dacr3_mix_en     : 1   ; /* [15]  */
        unsigned int    adcr2dacl3_mix_vol    : 7   ; /* [22..16]  */
        unsigned int    adcr2dacl3_mix_en     : 1   ; /* [23]  */
        unsigned int    adcl2dacl3_mix_vol    : 7   ; /* [30..24]  */
        unsigned int    adcl2dacl3_mix_en     : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ADC_REG_4_VALUE;

/* Define the union U_DAC0_CFG_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_8            : 8   ; /* [7..0]  */
        unsigned int    i2s2_fs_sel           : 5   ; /* [12..8]  */
        unsigned int    i2s1_fs_sel           : 5   ; /* [17..13]  */
        unsigned int    Reserved_7            : 1   ; /* [18]  */
        unsigned int    dig_bypass            : 1   ; /* [19]  */
        unsigned int    i2s2_data_bits        : 2   ; /* [21..20]  */
        unsigned int    i2s1_data_bits        : 2   ; /* [23..22]  */
        unsigned int    Reserved_6            : 2   ; /* [25..24]  */
        unsigned int    dacr_enable           : 1   ; /* [26]  */
        unsigned int    dacl_enable           : 1   ; /* [27]  */
        unsigned int    Reserved_5            : 2   ; /* [29..28]  */
        unsigned int    dacr_reset_n          : 1   ; /* [30]  */
        unsigned int    dacl_reset_n          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DAC0_CFG_VALUE;

/* Define the union U_DAC0_REG_0_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dacr_lrsel            : 1   ; /* [0]  */
        unsigned int    dacr_i2ssel           : 1   ; /* [1]  */
        unsigned int    dacl_lrsel            : 1   ; /* [2]  */
        unsigned int    dacl_i2ssel           : 1   ; /* [3]  */
        unsigned int    Reserved_9            : 15  ; /* [18..4]  */
        unsigned int    dacr_deemphasis_fs    : 2   ; /* [20..19]  */
        unsigned int    dacl_deemphasis_fs    : 2   ; /* [22..21]  */
        unsigned int    muter_rate            : 2   ; /* [24..23]  */
        unsigned int    mutel_rate            : 2   ; /* [26..25]  */
        unsigned int    dacvu                 : 1   ; /* [27]  */
        unsigned int    sunmuter              : 1   ; /* [28]  */
        unsigned int    sunmutel              : 1   ; /* [29]  */
        unsigned int    smuter                : 1   ; /* [30]  */
        unsigned int    smutel                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DAC0_REG_0_VALUE;

/* Define the union U_DAC0_REG_1_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dacl2dacr_mix_vol     : 7   ; /* [6..0]  */
        unsigned int    dacl2dacr_mix_en      : 1   ; /* [7]  */
        unsigned int    dacr2dacl_mix_vol     : 7   ; /* [14..8]  */
        unsigned int    dacr2dacl_mix_en      : 1   ; /* [15]  */
        unsigned int    dacr_vol              : 7   ; /* [22..16]  */
        unsigned int    dacr_mute             : 1   ; /* [23]  */
        unsigned int    dacl_vol              : 7   ; /* [30..24]  */
        unsigned int    dacl_mute             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DAC0_REG_1_VALUE;

/* Define the union U_DAC1_CFG_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_13           : 8   ; /* [7..0]  */
        unsigned int    i2s2_fs_sel           : 5   ; /* [12..8]  */
        unsigned int    i2s1_fs_sel           : 5   ; /* [17..13]  */
        unsigned int    Reserved_12           : 1   ; /* [18]  */
        unsigned int    dig_bypass            : 1   ; /* [19]  */
        unsigned int    i2s2_data_bits        : 2   ; /* [21..20]  */
        unsigned int    i2s1_data_bits        : 2   ; /* [23..22]  */
        unsigned int    Reserved_11           : 2   ; /* [25..24]  */
        unsigned int    dacr_enable           : 1   ; /* [26]  */
        unsigned int    dacl_enable           : 1   ; /* [27]  */
        unsigned int    Reserved_10           : 2   ; /* [29..28]  */
        unsigned int    dacr_reset_n          : 1   ; /* [30]  */
        unsigned int    dacl_reset_n          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DAC1_CFG_VALUE;

/* Define the union U_DAC1_REG_0_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dacr_lrsel            : 1   ; /* [0]  */
        unsigned int    dacr_i2ssel           : 1   ; /* [1]  */
        unsigned int    dacl_lrsel            : 1   ; /* [2]  */
        unsigned int    dacl_i2ssel           : 1   ; /* [3]  */
        unsigned int    Reserved_14           : 15  ; /* [18..4]  */
        unsigned int    dacr_deemphasis_fs    : 2   ; /* [20..19]  */
        unsigned int    dacl_deemphasis_fs    : 2   ; /* [22..21]  */
        unsigned int    muter_rate            : 2   ; /* [24..23]  */
        unsigned int    mutel_rate            : 2   ; /* [26..25]  */
        unsigned int    dacvu                 : 1   ; /* [27]  */
        unsigned int    sunmuter              : 1   ; /* [28]  */
        unsigned int    sunmutel              : 1   ; /* [29]  */
        unsigned int    smuter                : 1   ; /* [30]  */
        unsigned int    smutel                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DAC1_REG_0_VALUE;

/* Define the union U_DAC1_REG_1_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dacl2dacr_mix_vol     : 7   ; /* [6..0]  */
        unsigned int    dacl2dacr_mix_en      : 1   ; /* [7]  */
        unsigned int    dacr2dacl_mix_vol     : 7   ; /* [14..8]  */
        unsigned int    dacr2dacl_mix_en      : 1   ; /* [15]  */
        unsigned int    dacr_vol              : 7   ; /* [22..16]  */
        unsigned int    dacr_mute             : 1   ; /* [23]  */
        unsigned int    dacl_vol              : 7   ; /* [30..24]  */
        unsigned int    dacl_mute             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DAC1_REG_1_VALUE;

/* Define the union U_DAC2_CFG_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_18           : 8   ; /* [7..0]  */
        unsigned int    i2s2_fs_sel           : 5   ; /* [12..8]  */
        unsigned int    i2s1_fs_sel           : 5   ; /* [17..13]  */
        unsigned int    Reserved_17           : 1   ; /* [18]  */
        unsigned int    dig_bypass            : 1   ; /* [19]  */
        unsigned int    i2s2_data_bits        : 2   ; /* [21..20]  */
        unsigned int    i2s1_data_bits        : 2   ; /* [23..22]  */
        unsigned int    Reserved_16           : 2   ; /* [25..24]  */
        unsigned int    dacr_enable           : 1   ; /* [26]  */
        unsigned int    dacl_enable           : 1   ; /* [27]  */
        unsigned int    Reserved_15           : 2   ; /* [29..28]  */
        unsigned int    dacr_reset_n          : 1   ; /* [30]  */
        unsigned int    dacl_reset_n          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DAC2_CFG_VALUE;

/* Define the union U_DAC2_REG_0_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dacr_lrsel            : 1   ; /* [0]  */
        unsigned int    dacr_i2ssel           : 1   ; /* [1]  */
        unsigned int    dacl_lrsel            : 1   ; /* [2]  */
        unsigned int    dacl_i2ssel           : 1   ; /* [3]  */
        unsigned int    Reserved_19           : 15  ; /* [18..4]  */
        unsigned int    dacr_deemphasis_fs    : 2   ; /* [20..19]  */
        unsigned int    dacl_deemphasis_fs    : 2   ; /* [22..21]  */
        unsigned int    muter_rate            : 2   ; /* [24..23]  */
        unsigned int    mutel_rate            : 2   ; /* [26..25]  */
        unsigned int    dacvu                 : 1   ; /* [27]  */
        unsigned int    sunmuter              : 1   ; /* [28]  */
        unsigned int    sunmutel              : 1   ; /* [29]  */
        unsigned int    smuter                : 1   ; /* [30]  */
        unsigned int    smutel                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DAC2_REG_0_VALUE;

/* Define the union U_DAC2_REG_1_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dacl2dacr_mix_vol     : 7   ; /* [6..0]  */
        unsigned int    dacl2dacr_mix_en      : 1   ; /* [7]  */
        unsigned int    dacr2dacl_mix_vol     : 7   ; /* [14..8]  */
        unsigned int    dacr2dacl_mix_en      : 1   ; /* [15]  */
        unsigned int    dacr_vol              : 7   ; /* [22..16]  */
        unsigned int    dacr_mute             : 1   ; /* [23]  */
        unsigned int    dacl_vol              : 7   ; /* [30..24]  */
        unsigned int    dacl_mute             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DAC2_REG_1_VALUE;

/* Define the union U_DAC3_CFG_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_23           : 8   ; /* [7..0]  */
        unsigned int    i2s2_fs_sel           : 5   ; /* [12..8]  */
        unsigned int    i2s1_fs_sel           : 5   ; /* [17..13]  */
        unsigned int    Reserved_22           : 1   ; /* [18]  */
        unsigned int    dig_bypass            : 1   ; /* [19]  */
        unsigned int    i2s2_data_bits        : 2   ; /* [21..20]  */
        unsigned int    i2s1_data_bits        : 2   ; /* [23..22]  */
        unsigned int    Reserved_21           : 2   ; /* [25..24]  */
        unsigned int    dacr_enable           : 1   ; /* [26]  */
        unsigned int    dacl_enable           : 1   ; /* [27]  */
        unsigned int    Reserved_20           : 2   ; /* [29..28]  */
        unsigned int    dacr_reset_n          : 1   ; /* [30]  */
        unsigned int    dacl_reset_n          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DAC3_CFG_VALUE;

/* Define the union U_DAC3_REG_0_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dacr_lrsel            : 1   ; /* [0]  */
        unsigned int    dacr_i2ssel           : 1   ; /* [1]  */
        unsigned int    dacl_lrsel            : 1   ; /* [2]  */
        unsigned int    dacl_i2ssel           : 1   ; /* [3]  */
        unsigned int    Reserved_24           : 15  ; /* [18..4]  */
        unsigned int    dacr_deemphasis_fs    : 2   ; /* [20..19]  */
        unsigned int    dacl_deemphasis_fs    : 2   ; /* [22..21]  */
        unsigned int    muter_rate            : 2   ; /* [24..23]  */
        unsigned int    mutel_rate            : 2   ; /* [26..25]  */
        unsigned int    dacvu                 : 1   ; /* [27]  */
        unsigned int    sunmuter              : 1   ; /* [28]  */
        unsigned int    sunmutel              : 1   ; /* [29]  */
        unsigned int    smuter                : 1   ; /* [30]  */
        unsigned int    smutel                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DAC3_REG_0_VALUE;

/* Define the union U_DAC3_REG_1_VALUE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dacl2dacr_mix_vol     : 7   ; /* [6..0]  */
        unsigned int    dacl2dacr_mix_en      : 1   ; /* [7]  */
        unsigned int    dacr2dacl_mix_vol     : 7   ; /* [14..8]  */
        unsigned int    dacr2dacl_mix_en      : 1   ; /* [15]  */
        unsigned int    dacr_vol              : 7   ; /* [22..16]  */
        unsigned int    dacr_mute             : 1   ; /* [23]  */
        unsigned int    dacl_vol              : 7   ; /* [30..24]  */
        unsigned int    dacl_mute             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DAC3_REG_1_VALUE;

/* Define the union U_ANA_REG_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vref_fs               : 1   ; /* [0]  */
        unsigned int    vref_pd               : 1   ; /* [1]  */
        unsigned int    ctcm_pd               : 1   ; /* [2]  */
        unsigned int    ibias_pd              : 1   ; /* [3]  */
        unsigned int    rpga_pd               : 1   ; /* [4]  */
        unsigned int    lpga_pd               : 1   ; /* [5]  */
        unsigned int    adcr_pd               : 1   ; /* [6]  */
        unsigned int    adcl_pd               : 1   ; /* [7]  */
        unsigned int    dacr4_pd              : 1   ; /* [8]  */
        unsigned int    dacr3_pd              : 1   ; /* [9]  */
        unsigned int    dacr2_pd              : 1   ; /* [10]  */
        unsigned int    dacr1_pd              : 1   ; /* [11]  */
        unsigned int    dacl4_pd              : 1   ; /* [12]  */
        unsigned int    dacl3_pd              : 1   ; /* [13]  */
        unsigned int    dacl2_pd              : 1   ; /* [14]  */
        unsigned int    dacl1_pd              : 1   ; /* [15]  */
        unsigned int    lpga_sel              : 4   ; /* [19..16]  */
        unsigned int    lpga_gain             : 3   ; /* [22..20]  */
        unsigned int    lpga_boost            : 1   ; /* [23]  */
        unsigned int    rpga_sel              : 4   ; /* [27..24]  */
        unsigned int    rpga_gain             : 3   ; /* [30..28]  */
        unsigned int    rpga_boost            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ANA_REG_0;

/* Define the union U_ANA_REG_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adc_timing_sel        : 1   ; /* [0]  */
        unsigned int    flstn                 : 2   ; /* [2..1]  */
        unsigned int    adc_dwa_byp           : 1   ; /* [3]  */
        unsigned int    adc_clkdly_sel        : 1   ; /* [4]  */
        unsigned int    adc_adatn             : 2   ; /* [6..5]  */
        unsigned int    Reserved_25           : 1   ; /* [7]  */
        unsigned int    dacr4_mute            : 1   ; /* [8]  */
        unsigned int    dacr3_mute            : 1   ; /* [9]  */
        unsigned int    dacr2_mute            : 1   ; /* [10]  */
        unsigned int    dacr1_mute            : 1   ; /* [11]  */
        unsigned int    dacl4_mute            : 1   ; /* [12]  */
        unsigned int    dacl3_mute            : 1   ; /* [13]  */
        unsigned int    dacl2_mute            : 1   ; /* [14]  */
        unsigned int    dacl1_mute            : 1   ; /* [15]  */
        unsigned int    clk_ctcm_sel          : 2   ; /* [17..16]  */
        unsigned int    clk_st2_sel           : 2   ; /* [19..18]  */
        unsigned int    clk_st1_sel           : 2   ; /* [21..20]  */
        unsigned int    clk_pga_sel           : 2   ; /* [23..22]  */
        unsigned int    byp_chopper_st2       : 1   ; /* [24]  */
        unsigned int    byp_chopper_st1       : 1   ; /* [25]  */
        unsigned int    byp_chopper_linein    : 1   ; /* [26]  */
        unsigned int    byp_chopper_ctcm      : 1   ; /* [27]  */
        unsigned int    adj4                  : 2   ; /* [29..28]  */
        unsigned int    lpga_mute             : 1   ; /* [30]  */
        unsigned int    rpga_mute             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ANA_REG_1;

/* Define the union U_ANA_REG_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj3                  : 2   ; /* [1..0]  */
        unsigned int    adj2                  : 2   ; /* [3..2]  */
        unsigned int    adj1                  : 2   ; /* [5..4]  */
        unsigned int    adj0                  : 2   ; /* [7..6]  */
        unsigned int    popfree_r4            : 1   ; /* [8]  */
        unsigned int    popfree_r3            : 1   ; /* [9]  */
        unsigned int    popfree_r2            : 1   ; /* [10]  */
        unsigned int    popfree_r1            : 1   ; /* [11]  */
        unsigned int    popfree_l4            : 1   ; /* [12]  */
        unsigned int    popfree_l3            : 1   ; /* [13]  */
        unsigned int    popfree_l2            : 1   ; /* [14]  */
        unsigned int    popfree_l1            : 1   ; /* [15]  */
        unsigned int    ana_loop              : 1   ; /* [16]  */
        unsigned int    clk_ctrl_adc          : 1   ; /* [17]  */
        unsigned int    clk_ctrl1             : 1   ; /* [18]  */
        unsigned int    clk_a2d_inv           : 1   ; /* [19]  */
        unsigned int    clk_ctrl2             : 1   ; /* [20]  */
        unsigned int    clk_ctrl3             : 1   ; /* [21]  */
        unsigned int    clk_ctrl4             : 1   ; /* [22]  */
        unsigned int    Reserved_26           : 1   ; /* [23]  */
        unsigned int    linein_avout2         : 4   ; /* [27..24]  */
        unsigned int    linein_avout1         : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ANA_REG_2;

/* Define the union U_ANA_REG_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    linein_hp             : 4   ; /* [3..0]  */
        unsigned int    linein_sp             : 4   ; /* [7..4]  */
#if defined (CHIP_TYPE_hi3751v620)
        unsigned int    hp_gain               : 3   ; /* [10..8]  */
        unsigned int    hp_pd_vcm_gen         : 1   ; /* [11]  */
        unsigned int    Reserved_0            : 4   ; /* [15..12]  */
        unsigned int    pop_spd_cfg           : 2   ; /* [17..16]  */
        unsigned int    hp_pull_en            : 1   ; /* [18]  */
        unsigned int    clk_pop_sel           : 2   ; /* [20..19]  */
        unsigned int    byp_popclk            : 1   ; /* [21]  */
        unsigned int    Reserved_1            : 2   ; /* [23..22]  */
        unsigned int    pop_pdm_delay_cfg_12  : 2   ; /* [25..24]  */
        unsigned int    ctl_rst_pop_12        : 1   ; /* [26..26]  */
        unsigned int    hp_pd_org_12          : 1   ; /* [27..27]  */
        unsigned int    pop_dis_12            : 1   ; /* [28..28]  */
        unsigned int    Reserved_2            : 3   ; /* [31..29]  */
#else
        unsigned int    Reserved0             : 24  ; /* [31..8]  */
#endif
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ANA_REG_3;



/* Define the union U_ANA_DAC_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    popfreer              : 1   ; /* [0]  */
        unsigned int    popfreel              : 1   ; /* [1]  */
        unsigned int    fs                    : 1   ; /* [2]  */
        unsigned int    pd_vref               : 1   ; /* [3]  */
        unsigned int    mute_dacr             : 1   ; /* [4]  */
        unsigned int    mute_dacl             : 1   ; /* [5]  */
        unsigned int    pd_dacr               : 1   ; /* [6]  */
        unsigned int    pd_dacl               : 1   ; /* [7]  */
        unsigned int    clkdgesel             : 1   ; /* [8]  */
        unsigned int    clksel2               : 1   ; /* [9]  */
        unsigned int    adj_refbf             : 2   ; /* [11..10]  */
        unsigned int    rst                   : 1   ; /* [12]  */
        unsigned int    adj_ctcm              : 1   ; /* [13]  */
        unsigned int    adj_dac               : 2   ; /* [15..14]  */
        unsigned int    Reserved_28           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ANA_DAC_CTRL;

/* Define the union U_ANA_ADAC_SRC_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int adac1_source_sel    : 2 ;/*[1:0]*/
        unsigned int reserved_3          : 6 ;/*[7:2]*/
        unsigned int adac2_source_sel    : 2 ;/*[9:8]*/
        unsigned int reserved_2          : 6 ;/*[15:10]*/
        unsigned int adac3_source_sel    : 2 ;/*[17:16]*/
        unsigned int reserved_1          : 6 ;/*[23:18]*/
        unsigned int adac4_source_sel    : 2 ;/*[25:24]*/
        unsigned int reserved_0          : 6 ;/*[31:26]*/
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ANA_ADAC_SRC_SEL;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_TOP_INFO             TOP_INFO                                ; /* 0x0 */
    volatile U_ADC_CFG_VALUE        ADC_CFG_VALUE                           ; /* 0x4 */
    volatile U_ADC_REG_0_VALUE      ADC_REG_0_VALUE                         ; /* 0x8 */
    volatile U_ADC_REG_1_VALUE      ADC_REG_1_VALUE                         ; /* 0xc */
    volatile U_ADC_REG_2_VALUE      ADC_REG_2_VALUE                         ; /* 0x10 */
    volatile U_ADC_REG_3_VALUE      ADC_REG_3_VALUE                         ; /* 0x14 */
    volatile U_ADC_REG_4_VALUE      ADC_REG_4_VALUE                         ; /* 0x18 */
    volatile U_DAC0_CFG_VALUE       DAC0_CFG_VALUE                          ; /* 0x1c */
    volatile U_DAC0_REG_0_VALUE     DAC0_REG_0_VALUE                        ; /* 0x20 */
    volatile U_DAC0_REG_1_VALUE     DAC0_REG_1_VALUE                        ; /* 0x24 */
    volatile U_DAC1_CFG_VALUE       DAC1_CFG_VALUE                          ; /* 0x28 */
    volatile U_DAC1_REG_0_VALUE     DAC1_REG_0_VALUE                        ; /* 0x2c */
    volatile U_DAC1_REG_1_VALUE     DAC1_REG_1_VALUE                        ; /* 0x30 */
    volatile U_DAC2_CFG_VALUE       DAC2_CFG_VALUE                          ; /* 0x34 */
    volatile U_DAC2_REG_0_VALUE     DAC2_REG_0_VALUE                        ; /* 0x38 */
    volatile U_DAC2_REG_1_VALUE     DAC2_REG_1_VALUE                        ; /* 0x3c */
    volatile U_DAC3_CFG_VALUE       DAC3_CFG_VALUE                          ; /* 0x40 */
    volatile U_DAC3_REG_0_VALUE     DAC3_REG_0_VALUE                        ; /* 0x44 */
    volatile U_DAC3_REG_1_VALUE     DAC3_REG_1_VALUE                        ; /* 0x48 */
    volatile U_ANA_REG_0            ANA_REG_0                               ; /* 0x4c */
    volatile U_ANA_REG_1            ANA_REG_1                               ; /* 0x50 */
    volatile U_ANA_REG_2            ANA_REG_2                               ; /* 0x54 */
    volatile U_ANA_REG_3            ANA_REG_3                               ; /* 0x58 */
    volatile unsigned int            ANA_Reserved                            ; /* 0x5c */
    volatile U_ANA_DAC_CTRL         ANA_DAC_CTRL                            ; /* 0x60 */
#if defined (CHIP_TYPE_hi3751v620)
    volatile U_ANA_ADAC_SRC_SEL     ANA_ADAC_SRC_SEL                        ; /* 0x64 */
#endif
} S_Audio_Codec_REGS_TYPE;

//volatile unsigned int            Reserved                                ;/* 0x5c */

typedef enum
{
    S5_TIANLAI_TOP_INFO_ADC2DAC0 = 0,
    S5_TIANLAI_TOP_INFO_ADC2DAC1,
    S5_TIANLAI_TOP_INFO_ADC2DAC2,
    S5_TIANLAI_TOP_INFO_ADC2DAC3,
    S5_TIANLAI_TOP_INFO_BUTT,
}S5_TIANLAI_TOP_INFO_E;

typedef enum
{
    S5_TIANLAI_BIT_DEPTH_16 = 0,
    S5_TIANLAI_BIT_DEPTH_18,
    S5_TIANLAI_BIT_DEPTH_20,
    S5_TIANLAI_BIT_DEPTH_24,
    S5_TIANLAI_BIT_DEPTH_BUTT,
}S5_TIANLAI_BIT_DEPTH_E;

typedef enum
{
    S5_TIANLAI_SAMPLE_RATE_MCLK_512_2 = 24,
    S5_TIANLAI_SAMPLE_RATE_MCLK_256_2,
    S5_TIANLAI_SAMPLE_RATE_MCLK_128_2,
    S5_TIANLAI_SAMPLE_RATE_MCLK_64_2,
    S5_TIANLAI_SAMPLE_RATE_MCLK_32_2,
    S5_TIANLAI_SAMPLE_RATE__BUTT
}S5_TIANLAI_SAMPLERATE_E;

typedef enum
{
    S5_TIANLAI_MIXER_TYPE_ADCL_DACL0 = 0x0,
    S5_TIANLAI_MIXER_TYPE_ADCR_DACL0,
    S5_TIANLAI_MIXER_TYPE_ADCL_DACR0,
    S5_TIANLAI_MIXER_TYPE_ADCR_DACR0,
    S5_TIANLAI_MIXER_TYPE_ADCL_DACL1,
    S5_TIANLAI_MIXER_TYPE_ADCR_DACL1,
    S5_TIANLAI_MIXER_TYPE_ADCL_DACR1,
    S5_TIANLAI_MIXER_TYPE_ADCR_DACR1,
    S5_TIANLAI_MIXER_TYPE_ADCL_DACL2,
    S5_TIANLAI_MIXER_TYPE_ADCR_DACL2,
    S5_TIANLAI_MIXER_TYPE_ADCL_DACR2,
    S5_TIANLAI_MIXER_TYPE_ADCR_DACR2,
    S5_TIANLAI_MIXER_TYPE_ADCL_DACL3,
    S5_TIANLAI_MIXER_TYPE_ADCR_DACL3,
    S5_TIANLAI_MIXER_TYPE_ADCL_DACR3,
    S5_TIANLAI_MIXER_TYPE_ADCR_DACR3,
    S5_TIANLAI_MIXER_TYPE_BUTT
}S5_TIANLAI_MIXER_TYPE_E;

typedef enum
{
    S5_TIANLAI_DAC_NUM_0 = 0x0,
    S5_TIANLAI_DAC_NUM_1,
    S5_TIANLAI_DAC_NUM_2,
    S5_TIANLAI_DAC_NUM_3,
    S5_TIANLAI_DAC_NUM_BUTT
}S5_TIANLAI_DAC_NUM_E;

typedef enum
{
    S5_TIANLAI_ANA_PGA_GAIN_0dB = 0x0,
    S5_TIANLAI_ANA_PGA_GAIN_3dB,
    S5_TIANLAI_ANA_PGA_GAIN_6dB,
    S5_TIANLAI_ANA_PGA_GAIN_9dB,
    S5_TIANLAI_ANA_PGA_GAIN_12dB,
    S5_TIANLAI_ANA_PGA_GAIN_15dB,
    S5_TIANLAI_ANA_PGA_GAIN_18dB,
    S5_TIANLAI_ANA_PGA_GAIN_21dB,
    S5_TIANLAI_ANA_PGA_GAIN_BUTT
}S5_TIANLAI_ANA_PGA_GAIN_E;

typedef enum
{
    S5_TIANLAI_DAC_DEEPFS_NULL = 0x0,
    S5_TIANLAI_DAC_DEEPFS_32k,
    S5_TIANLAI_DAC_DEEPFS_44k,
    S5_TIANLAI_DAC_DEEPFS_48k,
    S5_TIANLAI_DAC_DEEPFS_BUTT
}S5_TIANLAI_DAC_DEEPFS_E;

typedef enum
{
    S5_TIANLAI_ANAREG_NUM_0 = 0x0,
    S5_TIANLAI_ANAREG_NUM_1,
    S5_TIANLAI_ANAREG_NUM_2,
    S5_TIANLAI_ANAREG_NUM_3,
    S5_TIANLAI_ANAREG_NUM_BUTT
}S5_TIANLAI_ANAREG_NUM_E;

                                         //TST
typedef enum
{
    S5_TIANLAI_LINEIN_SEL_MIC = 0x0,     //MIC 0
    S5_TIANLAI_LINEIN_SEL_L1,            //VGA IN
    S5_TIANLAI_LINEIN_SEL_L2,            //AV0
    S5_TIANLAI_LINEIN_SEL_L3,            //YPbPr
    S5_TIANLAI_LINEIN_SEL_L4,            //AV1
    S5_TIANLAI_LINEIN_SEL_BUTT
}S5_TIANLAI_LINEIN_SEL_E;

typedef enum
{
    S5_TIANLAI_LINOUT_LINEIN_DAC   = 0x0,  //?¨¤¨®|¦Ì?DAC¨º?3?
    S5_TIANLAI_LINOUT_LINEIN_L1_R1 = 0x1,  //LineIn1
    S5_TIANLAI_LINOUT_LINEIN_L2_R2 = 0x2,  //LineIn2
    S5_TIANLAI_LINOUT_LINEIN_L3_R3 = 0x4,  //LineIn3
    S5_TIANLAI_LINOUT_LINEIN_L4_R4 = 0x8,  //LineIn4
    S5_TIANLAI_LINOUT_LINEIN_BUTT
}S5_TIANLAI_LINOUT_LINEIN_E;

typedef enum
{
    S5_TIANLAI_LINEOUT_1 = 0,
    S5_TIANLAI_LINEOUT_2,
    S5_TIANLAI_LINEOUT_3,
    S5_TIANLAI_LINEOUT_4,
    S5_TIANLAI_LINEOUT_BUTT
}S5_TIANLAI_LINEOUT_E;

HI_U32 HAL_TIANLAI_V510_SetAanReg(S5_TIANLAI_ANAREG_NUM_E enAnaRegNum, HI_U32 u32RegVal);
HI_U32 HAL_TIANLAI_V510_SetLineInSuorce(S5_TIANLAI_LINEIN_SEL_E enLineInSel, HI_BOOL bByPass, HI_BOOL bAdcWork);
HI_U32 HAL_TIANLAI_V510_SetLineOutSel(S5_TIANLAI_LINEOUT_E enLineOut, S5_TIANLAI_LINOUT_LINEIN_E enLineIn);
HI_U32 HAL_TIANLAI_V510_Init(S5_TIANLAI_SAMPLERATE_E enSampleRate);
HI_U32 HAL_TIANLAI_V510_DeInit(HI_VOID);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __AUD_TIANLAI_ADAC_HAL_H__ */

