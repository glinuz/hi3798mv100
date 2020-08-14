/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hal_aiao_priv.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/09/22
Last Modified :
Description   : aiao
Function List :
History       :
* main\1    2012-09-22   z40717     init.
******************************************************************************/
#ifndef __HI_HAL_AIAO_PRIV_H__
#define __HI_HAL_AIAO_PRIV_H__

#include "hi_type.h"
#include "hi_drv_mmz.h"

#include "hi_drv_ao.h"
#include "hi_drv_ai.h"
#include "hal_aiao_common.h"
#include "circ_buf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#define HW_CHN_PTR_BUG

typedef struct
{
    AIAO_PORT_USER_CFG_S stUserCongfig;
    AIAO_PORT_ID_E       enPortID;

    /* internal state */
    AIAO_PORT_STATUS_E enStatus;
    AIAO_PROC_STAUTS_S stStatus;
    MMZ_BUFFER_S       stMmz;
    CIRC_BUF_S         stCB;
    AIAO_BufInfo_S     stBuf;
    HI_S32             bStopFadeOutMute;
    HI_CHAR            szProcMmzName[16];
} AIAO_PORT_S;

#ifdef HI_AIAO_TIMER_SUPPORT
typedef struct
{
    AIAO_TIMER_ID_E       enTimerID;
    AIAO_Timer_Create_S  stTimerParam;
    AIAO_TIMER_Status_S  stTimerStatus;
    AIAO_TIMER_Attr_S    stTimerAttr;
} AIAO_TIMER_CTX_S, *AIAO_TIMER_S;
#endif

// Define the union U_AIAO_SYS_CRG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int aiao_cken             : 1; // [0]
        unsigned int Reserved_0            : 3; // [3..1]
        unsigned int aiao_srst_req         : 1; // [4]
        unsigned int Reserved_1            : 3; // [7..5]
        unsigned int aiao_clk_sel          : 1; // [8]
        unsigned int Reserved_2            : 3; // [11..9]
        unsigned int aiaoclk_skipcfg       : 5; // [16..12]
        unsigned int aiaoclk_loaden        : 1; // [17]
#if   defined(CHIP_TYPE_hi3751v100)     \
    || defined(CHIP_TYPE_hi3751v100b)   \
    || defined(CHIP_TYPE_hi3751v600)    \
    || defined(CHIP_TYPE_hi3751lv500)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100)   \
    || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3798cv200_a) \
    || defined(CHIP_TYPE_hi3798cv200_b) \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a) \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)

        unsigned int Reserved_3            : 2; // [19..18]
        unsigned int aiao_mclk_sel         : 2; // [21..20]
        unsigned int Reserved_4            : 10; // [31..22]
#elif  defined(CHIP_TYPE_hi3716cv200es)  \
    || defined (CHIP_TYPE_hi3751v500)    \
    || defined(CHIP_TYPE_hi3751v620)
        unsigned int Reserved_3            : 14;// [31..18]
#endif
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_S40_AIAO_SYS_CRG;

// Define the union U_AIAO_INT_ENA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_ch0_int_ena        : 1; // [0]
        unsigned int rx_ch1_int_ena        : 1; // [1]
        unsigned int rx_ch2_int_ena        : 1; // [2]
        unsigned int rx_ch3_int_ena        : 1; // [3]
        unsigned int rx_ch4_int_ena        : 1; // [4]
        unsigned int rx_ch5_int_ena        : 1; // [5]
        unsigned int rx_ch6_int_ena        : 1; // [6]
        unsigned int rx_ch7_int_ena        : 1; // [7]
        unsigned int Reserved_1            : 8; // [15..8]
        unsigned int tx_ch0_int_ena        : 1; // [16]
        unsigned int tx_ch1_int_ena        : 1; // [17]
        unsigned int tx_ch2_int_ena        : 1; // [18]
        unsigned int tx_ch3_int_ena        : 1; // [19]
        unsigned int tx_ch4_int_ena        : 1; // [20]
        unsigned int tx_ch5_int_ena        : 1; // [21]
        unsigned int tx_ch6_int_ena        : 1; // [22]
        unsigned int tx_ch7_int_ena        : 1; // [23]
        unsigned int spdiftx_ch0_int_ena   : 1; // [24]
        unsigned int spdiftx_ch1_int_ena   : 1; // [25]
        unsigned int spdiftx_ch2_int_ena   : 1; // [26]
#ifdef HI_AIAO_TIMER_SUPPORT
        unsigned int timer_ch0_int_ena     : 1; // [27]  /* its name in AIAO register is: timer0_int_ena */
        unsigned int timer_ch1_int_ena     : 1; // [28]  /* its name in AIAO register is: timer1_int_ena */
        unsigned int Reserved_0            : 3; // [31..29]
#else
        unsigned int spdiftx_ch3_int_ena   : 1; // [27]
        unsigned int Reserved_0            : 4; // [31..28]
#endif
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_AIAO_INT_ENA;

// Define the union U_AIAO_INT_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_ch0_int_status     : 1; // [0]
        unsigned int rx_ch1_int_status     : 1; // [1]
        unsigned int rx_ch2_int_status     : 1; // [2]
        unsigned int rx_ch3_int_status     : 1; // [3]
        unsigned int rx_ch4_int_status     : 1; // [4]
        unsigned int rx_ch5_int_status     : 1; // [5]
        unsigned int rx_ch6_int_status     : 1; // [6]
        unsigned int rx_ch7_int_status     : 1; // [7]
        unsigned int Reserved_1            : 8; // [15..8]
        unsigned int tx_ch0_int_status     : 1; // [16]
        unsigned int tx_ch1_int_status     : 1; // [17]
        unsigned int tx_ch2_int_status     : 1; // [18]
        unsigned int tx_ch3_int_status     : 1; // [19]
        unsigned int tx_ch4_int_status     : 1; // [20]
        unsigned int tx_ch5_int_status     : 1; // [21]
        unsigned int tx_ch6_int_status     : 1; // [22]
        unsigned int tx_ch7_int_status     : 1; // [23]
        unsigned int spdiftx_ch0_int_status  : 1; // [24]
        unsigned int spdiftx_ch1_int_status  : 1; // [25]
        unsigned int spdiftx_ch2_int_status  : 1; // [26]
#ifdef HI_AIAO_TIMER_SUPPORT
        unsigned int timer_ch0_int_status    : 1; // [27]
        unsigned int timer_ch1_int_status    : 1; // [28]
        unsigned int Reserved_0              : 3; // [31..29]
#else
        unsigned int spdiftx_ch3_int_status  : 1; // [27]
        unsigned int Reserved_0            : 4; // [31..28]
#endif
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_AIAO_INT_STATUS;

// Define the union U_AIAO_INT_RAW
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_ch0_int_raw        : 1; // [0]
        unsigned int rx_ch1_int_raw        : 1; // [1]
        unsigned int rx_ch2_int_raw        : 1; // [2]
        unsigned int rx_ch3_int_raw        : 1; // [3]
        unsigned int rx_ch4_int_raw        : 1; // [4]
        unsigned int rx_ch5_int_raw        : 1; // [5]
        unsigned int rx_ch6_int_raw        : 1; // [6]
        unsigned int rx_ch7_int_raw        : 1; // [7]
        unsigned int Reserved_1            : 8; // [15..8]
        unsigned int tx_ch0_int_raw        : 1; // [16]
        unsigned int tx_ch1_int_raw        : 1; // [17]
        unsigned int tx_ch2_int_raw        : 1; // [18]
        unsigned int tx_ch3_int_raw        : 1; // [19]
        unsigned int tx_ch4_int_raw        : 1; // [20]
        unsigned int tx_ch5_int_raw        : 1; // [21]
        unsigned int tx_ch6_int_raw        : 1; // [22]
        unsigned int tx_ch7_int_raw        : 1; // [23]
        unsigned int spdiftx_ch0_int_raw   : 1; // [24]
        unsigned int spdiftx_ch1_int_raw   : 1; // [25]
        unsigned int spdiftx_ch2_int_raw   : 1; // [26]
#ifdef HI_AIAO_TIMER_SUPPORT
        unsigned int timer_ch0_int_raw     : 1; // [27]
        unsigned int timer_ch1_int_raw     : 1; // [28]
        unsigned int Reserved_0            : 3; // [31..29]
#else
        unsigned int spdiftx_ch3_int_raw   : 1; // [27]
        unsigned int Reserved_0            : 4; // [31..28]
#endif
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_AIAO_INT_RAW;

// Define the union U_HW_CAPABILITY
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_ch0_cap            : 1; // [0]
        unsigned int rx_ch1_cap            : 1; // [1]
        unsigned int rx_ch2_cap            : 1; // [2]
        unsigned int rx_ch3_cap            : 1; // [3]
        unsigned int rx_ch4_cap            : 1; // [4]
        unsigned int rx_ch5_cap            : 1; // [5]
        unsigned int rx_ch6_cap            : 1; // [6]
        unsigned int rx_ch7_cap            : 1; // [7]
        unsigned int Reserved_1            : 8; // [15..8]
        unsigned int tx_ch0_cap            : 1; // [16]
        unsigned int tx_ch1_cap            : 1; // [17]
        unsigned int tx_ch2_cap            : 1; // [18]
        unsigned int tx_ch3_cap            : 1; // [19]
        unsigned int tx_ch4_cap            : 1; // [20]
        unsigned int tx_ch5_cap            : 1; // [21]
        unsigned int tx_ch6_cap            : 1; // [22]
        unsigned int tx_ch7_cap            : 1; // [23]
        unsigned int spdiftx_ch0_cap       : 1; // [24]
        unsigned int spdiftx_ch1_cap       : 1; // [25]
        unsigned int spdiftx_ch2_cap       : 1; // [26]
#ifdef HI_AIAO_TIMER_SUPPORT
        unsigned int timer_ch0_cap         : 1; // [27]
        unsigned int timer_ch1_cap         : 1; // [28]
        unsigned int Reserved_0            : 3; // [31..29]
#else
        unsigned int spdiftx_ch3_cap       : 1; // [27]
        unsigned int Reserved_0            : 4; // [31..28]
#endif
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_HW_CAPABILITY;

// Define the union U_SPDIF_TX_MUX
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int spdif_tx_0_port_sel   : 3; // [2..0]
        unsigned int spdif_tx_0_port_en    : 1; // [3]
        unsigned int Reserved_3            : 4; // [7..4]
        unsigned int spdif_tx_1_port_sel   : 3; // [10..8]
        unsigned int spdif_tx_1_port_en    : 1; // [11]
        unsigned int Reserved_2            : 4; // [15..12]
        unsigned int spdif_tx_2_port_sel   : 3; // [18..16]
        unsigned int spdif_tx_2_port_en    : 1; // [19]
        unsigned int Reserved_1            : 4; // [23..20]
        unsigned int spdif_tx_3_port_sel   : 3; // [26..24]
        unsigned int spdif_tx_3_port_en    : 1; // [27]
        unsigned int Reserved_0            : 4; // [31..28]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_TX_MUX;

/* Define the union U_AIAO_SWITCH_RX_BCLK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    inner_bclk_ws_sel_rx_00 : 4   ; /* [3..0]  */
        unsigned int    inner_bclk_ws_sel_rx_01 : 4   ; /* [7..4]  */
        unsigned int    inner_bclk_ws_sel_rx_02 : 4   ; /* [11..8]  */
        unsigned int    inner_bclk_ws_sel_rx_03 : 4   ; /* [15..12]  */
        unsigned int    inner_bclk_ws_sel_rx_04 : 4   ; /* [19..16]  */
        unsigned int    inner_bclk_ws_sel_rx_05 : 4   ; /* [23..20]  */
        unsigned int    inner_bclk_ws_sel_rx_06 : 4   ; /* [27..24]  */
        unsigned int    inner_bclk_ws_sel_rx_07 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIAO_SWITCH_RX_BCLK;

/* Define the union U_AIAO_SWITCH_TX_BCLK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    inner_bclk_ws_sel_tx_00 : 4   ; /* [3..0]  */
        unsigned int    inner_bclk_ws_sel_tx_01 : 4   ; /* [7..4]  */
        unsigned int    inner_bclk_ws_sel_tx_02 : 4   ; /* [11..8]  */
        unsigned int    inner_bclk_ws_sel_tx_03 : 4   ; /* [15..12]  */
        unsigned int    inner_bclk_ws_sel_tx_04 : 4   ; /* [19..16]  */
        unsigned int    inner_bclk_ws_sel_tx_05 : 4   ; /* [23..20]  */
        unsigned int    inner_bclk_ws_sel_tx_06 : 4   ; /* [27..24]  */
        unsigned int    inner_bclk_ws_sel_tx_07 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIAO_SWITCH_TX_BCLK;

/* Define the union U_AIAO_STATUS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    srst_rdy              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIAO_STATUS;

/* Define the union U_VHB_OUTSTANDING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vhb_outst_num         : 3   ; /* [2..0]  */
        unsigned int    Reserved_14           : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VHB_OUTSTANDING;

#if defined(CHIP_TYPE_hi3751v620)
/* Define the union U_CRG_RST_EN_N */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aiao_crg_rst_en_n     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CRG_RST_EN_N;
#endif


// Define the union U_I2S_CRG_CFG0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int aiao_mclk_div         : 27; // [26..0]
#if defined(CHIP_TYPE_hi3751v620)
        unsigned int int_div_mode          : 1;  // [27]
        unsigned int Reserved_0            : 4;  // [31..28]
#else
        unsigned int Reserved_0            : 5;  // [31..27]
#endif
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_I2S_CRG_CFG0;

// Define the union U_I2S_CRG_CFG1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int aiao_bclk_div         : 4; // [3..0]
        unsigned int aiao_fsclk_div        : 3; // [6..4]
        unsigned int Reserved_1            : 1; // [7]
        unsigned int aiao_cken             : 1; // [8]
        unsigned int aiao_srst_req         : 1; // [9]
        unsigned int aiao_bclk_oen         : 1; // [10]
        unsigned int aiao_bclk_sel         : 1; // [11]
        unsigned int aiao_bclkin_pctrl     : 1; // [12]
        unsigned int aiao_bclkout_pctrl    : 1; // [13]
        unsigned int Reserved_0            : 18; // [31..14]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_I2S_CRG_CFG1;

// Define the union U_SPDIF_CRG_CFG0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int aiao_mclk_div_spdif   : 27; // [26..0]
#if defined(CHIP_TYPE_hi3751v620)
        unsigned int int_div_mode          : 1;  // [27]
        unsigned int Reserved_0            : 4;  // [31..28]
#else
        unsigned int Reserved_0            : 5;  // [31..27]
#endif
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_CRG_CFG0;

// Define the union U_SPDIF_CRG_CFG1_03
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int aiao_bclk_div_spdif   : 4; // [3..0]
        unsigned int aiao_fsclk_div_spdif  : 3; // [6..4]
        unsigned int Reserved_1            : 1; // [7]
        unsigned int aiao_cken_spdif       : 1; // [8]
        unsigned int aiao_srst_req_spdif   : 1; // [9]
        unsigned int Reserved_0            : 22; // [31..10]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_CRG_CFG1;

// Define the union U_RX_IF_ATTRI
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_mode               : 2; // [1..0]
        unsigned int rx_i2s_precision      : 2; // [3..2]
        unsigned int rx_ch_num             : 2; // [5..4]
        unsigned int Reserved_1            : 1; // [6]
        unsigned int rx_multislot_en       : 1; // [7]
        unsigned int rx_sd_offset          : 8; // [15..8]
        unsigned int rx_trackmode          : 3; // [18..16]
        unsigned int Reserved_0            : 1; // [19]
        unsigned int rx_sd_source_sel      : 4; // [23..20]
        unsigned int rx_sd0_sel            : 2; // [25..24]
        unsigned int rx_sd1_sel            : 2; // [27..26]
        unsigned int rx_sd2_sel            : 2; // [29..28]
        unsigned int rx_sd3_sel            : 2; // [31..30]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_IF_ATTRI;

// Define the union U_RX_DSP_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int mute_en               : 1; // [0]
        unsigned int mute_fade_en          : 1; // [1]
        unsigned int pause_en              : 1; // [2]
        unsigned int pause_fade_en         : 1; // [3]
        unsigned int Reserved_3            : 4; // [7..4]
        unsigned int volume                : 7; // [14..8]
        unsigned int Reserved_2            : 1; // [15]
        unsigned int fade_in_rate          : 4; // [19..16]
        unsigned int fade_out_rate         : 4; // [23..20]
        unsigned int Reserved_1            : 3; // [26..24]
        unsigned int bypass_en             : 1; // [27]
        unsigned int rx_enable             : 1; // [28]
        unsigned int rx_disable_done       : 1; // [29]
        unsigned int Reserved_0            : 2; // [31..30]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_DSP_CTRL;

// Define the union U_RX_WS_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int ws_count              : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_WS_CNT;

// Define the union U_RX_BCLK_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int bclk_count            : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_BCLK_CNT;

// Define the union U_RX_BUFF_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_buff_size          : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_BUFF_SIZE;

// Define the union U_RX_BUFF_WPTR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_buff_wptr          : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_BUFF_WPTR;

// Define the union U_RX_BUFF_RPTR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_buff_rptr          : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_BUFF_RPTR;

// Define the union U_RX_BUFF_ALFULL_TH
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_buff_alfull_th     : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_BUFF_ALFULL_TH;

// Define the union U_RX_TRANS_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_trans_size         : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_TRANS_SIZE;

// Define the union U_RX_WPTR_TMP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_wptr_tmp           : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_WPTR_TMP;

// Define the union U_RX_INT_ENA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_trans_int_ena      : 1; // [0]
        unsigned int rx_full_int_ena       : 1; // [1]
        unsigned int rx_alfull_int_ena     : 1; // [2]
        unsigned int rx_bfifo_full_int_ena  : 1; // [3]
        unsigned int rx_ififo_full_int_ena  : 1; // [4]
        unsigned int rx_stop_int_ena       : 1; // [5]
        unsigned int Reserved_0            : 26; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_INT_ENA;

// Define the union U_RX_INT_RAW
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_trans_int_raw      : 1; // [0]
        unsigned int rx_full_int_raw       : 1; // [1]
        unsigned int rx_alfull_int_raw     : 1; // [2]
        unsigned int rx_bfifo_full_int_raw  : 1; // [3]
        unsigned int rx_ififo_full_int_raw  : 1; // [4]
        unsigned int rx_stop_int_raw       : 1; // [5]
        unsigned int Reserved_0            : 26; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_INT_RAW;

// Define the union U_RX_INT_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_trans_int_status   : 1; // [0]
        unsigned int rx_full_int_status    : 1; // [1]
        unsigned int rx_alfull_int_status  : 1; // [2]
        unsigned int rx_bfifo_full_int_status  : 1; // [3]
        unsigned int rx_ififo_full_int_status  : 1; // [4]
        unsigned int rx_stop_int_status    : 1; // [5]
        unsigned int Reserved_0            : 26; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_INT_STATUS;

// Define the union U_RX_INT_CLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int rx_trans_int_clear    : 1; // [0]
        unsigned int rx_full_int_clear     : 1; // [1]
        unsigned int rx_alfull_int_clear   : 1; // [2]
        unsigned int rx_bfifo_full_int_clear  : 1; // [3]
        unsigned int rx_ififo_full_int_clear  : 1; // [4]
        unsigned int rx_stop_int_clear     : 1; // [5]
        unsigned int Reserved_0            : 26; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_RX_INT_CLR;

// Define the union U_TX_IF_ATTRI
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_mode               : 2; // [1..0]
        unsigned int tx_i2s_precision      : 2; // [3..2]
        unsigned int tx_ch_num             : 2; // [5..4]
        unsigned int Reserved_2            : 1; // [6]
        unsigned int Reserved_1            : 1; // [7]
        unsigned int tx_sd_offset          : 8; // [15..8]
        unsigned int tx_trackmode          : 3; // [18..16]
        unsigned int tx_spd_i2s_sel        : 1; // [19]
        unsigned int tx_sd_source_sel      : 4; // [23..20]
        unsigned int tx_sd0_sel            : 2; // [25..24]
        unsigned int tx_sd1_sel            : 2; // [27..26]
        unsigned int tx_sd2_sel            : 2; // [29..28]
        unsigned int tx_sd3_sel            : 2; // [31..30]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_IF_ATTRI;

// Define the union U_TX_DSP_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mute_en               : 1   ; // [0]
        unsigned int    mute_fade_en          : 1   ; // [1]
        unsigned int    Reserved_3            : 6   ; // [7..2]
        unsigned int    volume                : 7   ; // [14..8]
        unsigned int    Reserved_2            : 1   ; // [15]
        unsigned int    fade_in_rate          : 4   ; // [19..16]
        unsigned int    fade_out_rate         : 4   ; // [23..20]
        unsigned int    Reserved_1            : 3   ; // [26..24]
        unsigned int    bypass_en             : 1   ; // [27]
        unsigned int    tx_enable             : 1   ; // [28]
        unsigned int    tx_disable_done       : 1   ; // [29]
        unsigned int    Reserved_0            : 2   ; // [31..30]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_DSP_CTRL;

// Define the union U_TX_DSP_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mute_fade_vol         : 7   ; // [6..0]
        unsigned int    Reserved_0            : 25  ; // [31..7]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_DSP_STATUS;

// Define the union U_TX_WS_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int ws_count              : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_WS_CNT;

// Define the union U_TX_BCLK_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int bclk_count            : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_BCLK_CNT;

// Define the union U_TX_BUFF_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_buff_size          : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_BUFF_SIZE;

// Define the union U_TX_BUFF_WPTR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_buff_wptr          : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_BUFF_WPTR;

// Define the union U_TX_BUFF_RPTR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_buff_rptr          : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_BUFF_RPTR;

// Define the union U_TX_BUFF_ALEMPTY_TH
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_buff_alempty_th    : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_BUFF_ALEMPTY_TH;

// Define the union U_TX_TRANS_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_trans_size         : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_TRANS_SIZE;

// Define the union U_TX_RPTR_TMP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_rptr_tmp           : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_RPTR_TMP;

// Define the union U_TX_INT_ENA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tx_trans_int_ena      : 1   ; // [0]
        unsigned int    tx_empty_int_ena      : 1   ; // [1]
        unsigned int    tx_alempty_int_ena    : 1   ; // [2]
        unsigned int    tx_bfifo_empty_int_ena  : 1   ; // [3]
        unsigned int    tx_ififo_empty_int_ena  : 1   ; // [4]
        unsigned int    tx_stop_int_ena       : 1   ; // [5]
        unsigned int    tx_mfade_int_ena      : 1   ; // [6]
        unsigned int    tx_dat_break_int_ena  : 1   ; // [7]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_INT_ENA;

// Define the union U_TX_INT_RAW
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tx_trans_int_raw      : 1   ; // [0]
        unsigned int    tx_empty_int_raw      : 1   ; // [1]
        unsigned int    tx_alempty_int_raw    : 1   ; // [2]
        unsigned int    tx_bfifo_empty_int_raw  : 1   ; // [3]
        unsigned int    tx_ififo_empty_int_raw  : 1   ; // [4]
        unsigned int    tx_stop_int_raw       : 1   ; // [5]
        unsigned int    tx_mfade_int_raw      : 1   ; // [6]
        unsigned int    tx_dat_break_int_raw  : 1   ; // [7]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_INT_RAW;

// Define the union U_TX_INT_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tx_trans_int_status   : 1   ; // [0]
        unsigned int    tx_empty_int_status   : 1   ; // [1]
        unsigned int    tx_alempty_int_status  : 1   ; // [2]
        unsigned int    tx_bfifo_empty_int_status  : 1   ; // [3]
        unsigned int    tx_ififo_empty_int_status  : 1   ; // [4]
        unsigned int    tx_stop_int_status    : 1   ; // [5]
        unsigned int    tx_mfade_int_status   : 1   ; // [6]
        unsigned int    tx_dat_break_int_status  : 1   ; // [7]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_INT_STATUS;

// Define the union U_TX_INT_CLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tx_trans_int_clear    : 1   ; // [0]
        unsigned int    tx_empty_int_clear    : 1   ; // [1]
        unsigned int    tx_alempty_int_clear  : 1   ; // [2]
        unsigned int    tx_bfifo_empty_int_clear  : 1   ; // [3]
        unsigned int    tx_ififo_empty_int_clear  : 1   ; // [4]
        unsigned int    tx_stop_int_clear     : 1   ; // [5]
        unsigned int    tx_mfade_int_clear    : 1   ; // [6]
        unsigned int    tx_dat_break_int_clear  : 1   ; // [7]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_TX_INT_CLR;

// Define the union U_SPDIFTX_IF_ATTRI
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_2            : 2   ; // [1..0]
        unsigned int    tx_i2s_precision      : 2   ; // [3..2]
        unsigned int    tx_ch_num             : 2   ; // [5..4]
        unsigned int    Reserved_1            : 10  ; // [15..6]
        unsigned int    tx_trackmode          : 3   ; // [18..16]
        unsigned int    tx_spd_i2s_sel        : 1   ; // [19]
        unsigned int    Reserved_0            : 12  ; // [31..20]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_IF_ATTRI;

// Define the union U_SPDIFTX_DSP_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mute_en               : 1   ; // [0]
        unsigned int    mute_fade_en          : 1   ; // [1]
        unsigned int    Reserved_3            : 6   ; // [7..2]
        unsigned int    volume                : 7   ; // [14..8]
        unsigned int    Reserved_2            : 1   ; // [15]
        unsigned int    fade_in_rate          : 4   ; // [19..16]
        unsigned int    fade_out_rate         : 4   ; // [23..20]
        unsigned int    Reserved_1            : 3   ; // [26..24]
        unsigned int    bypass_en             : 1   ; // [27]
        unsigned int    tx_enable             : 1   ; // [28]
        unsigned int    tx_disable_done       : 1   ; // [29]
        unsigned int    Reserved_0            : 2   ; // [31..30]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_DSP_CTRL;

// Define the union U_SPDIFTX_DSP_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mute_fade_vol         : 7   ; // [6..0]
        unsigned int    Reserved_0            : 25  ; // [31..7]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_DSP_STATUS;

// Define the union U_SPDIFTX_WS_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int ws_count              : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_WS_CNT;

// Define the union U_SPDIFTX_BCLK_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int bclk_count            : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_BCLK_CNT;

// Define the union U_SPDIFTX_BUFF_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_buff_size          : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_BUFF_SIZE;

// Define the union U_SPDIFTX_BUFF_WPTR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_buff_wptr          : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_BUFF_WPTR;

// Define the union U_SPDIFTX_BUFF_RPTR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_buff_rptr          : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_BUFF_RPTR;

// Define the union U_SPDIFTX_BUFF_ALEMPTY_TH
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_buff_alempty_th    : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_BUFF_ALEMPTY_TH;

// Define the union U_SPDIFTX_TRANS_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_trans_size         : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_TRANS_SIZE;

// Define the union U_SPDIFTX_RPTR_TMP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int tx_rptr_tmp           : 24; // [23..0]
        unsigned int Reserved_0            : 8; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_RPTR_TMP;

// Define the union U_SPDIFTX_INT_ENA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tx_trans_int_ena      : 1   ; // [0]
        unsigned int    tx_empty_int_ena      : 1   ; // [1]
        unsigned int    tx_alempty_int_ena    : 1   ; // [2]
        unsigned int    tx_bfifo_empty_int_ena  : 1   ; // [3]
        unsigned int    tx_ififo_empty_int_ena  : 1   ; // [4]
        unsigned int    tx_stop_int_ena       : 1   ; // [5]
        unsigned int    tx_mfade_int_ena      : 1   ; // [6]
        unsigned int    tx_dat_break_int_ena  : 1   ; // [7]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_INT_ENA;

// Define the union U_SPDIFTX_INT_RAW
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tx_trans_int_raw      : 1   ; // [0]
        unsigned int    tx_empty_int_raw      : 1   ; // [1]
        unsigned int    tx_alempty_int_raw    : 1   ; // [2]
        unsigned int    tx_bfifo_empty_int_raw  : 1   ; // [3]
        unsigned int    tx_ififo_empty_int_raw  : 1   ; // [4]
        unsigned int    tx_stop_int_raw       : 1   ; // [5]
        unsigned int    tx_mfade_int_raw      : 1   ; // [6]
        unsigned int    tx_dat_break_int_raw  : 1   ; // [7]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_INT_RAW;

// Define the union U_SPDIFTX_INT_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tx_trans_int_status   : 1   ; // [0]
        unsigned int    tx_empty_int_status   : 1   ; // [1]
        unsigned int    tx_alempty_int_status  : 1   ; // [2]
        unsigned int    tx_bfifo_empty_int_status  : 1   ; // [3]
        unsigned int    tx_ififo_empty_int_status  : 1   ; // [4]
        unsigned int    tx_stop_int_status    : 1   ; // [5]
        unsigned int    tx_mfade_int_status   : 1   ; // [6]
        unsigned int    tx_dat_break_int_status  : 1   ; // [7]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_INT_STATUS;

// Define the union U_SPDIFTX_INT_CLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tx_trans_int_clear    : 1   ; // [0]
        unsigned int    tx_empty_int_clear    : 1   ; // [1]
        unsigned int    tx_alempty_int_clear  : 1   ; // [2]
        unsigned int    tx_bfifo_empty_int_clear  : 1   ; // [3]
        unsigned int    tx_ififo_empty_int_clear  : 1   ; // [4]
        unsigned int    tx_stop_int_clear     : 1   ; // [5]
        unsigned int    tx_mfade_int_clear    : 1   ; // [6]
        unsigned int    tx_dat_break_int_clear  : 1   ; // [7]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIFTX_INT_CLR;

// Define the union U_SPDIF_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int spdif_en              : 1; // [0]
        unsigned int Reserved_0            : 31; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_CTRL;

// Define the union U_SPDIF_CONFIG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int txdata_en             : 1; // [0]
        unsigned int mode                  : 2; // [2..1]
        unsigned int Reserved_0            : 29; // [31..3]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_CONFIG;

// Define the union U_SPDIF_INTRUPT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int lfifo_empty_intr      : 1; // [0]
        unsigned int rfifo_empty_intr      : 1; // [1]
        unsigned int lfifo_single_intr     : 1; // [2]
        unsigned int rfifo_single_intr     : 1; // [3]
        unsigned int Reserved_0            : 28; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_INTRUPT;

// Define the union U_SPDIF_INTR_MASK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int lfifo_empty_mask      : 1; // [0]
        unsigned int rfifo_empty__mask     : 1; // [1]
        unsigned int lfifo_single__mask    : 1; // [2]
        unsigned int rfifo_single__mask    : 1; // [3]
        unsigned int Reserved_0            : 28; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_INTR_MASK;

// Define the union U_SPDIF_INTR_STATE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int lfifo_empty_state     : 1; // [0]
        unsigned int rfifo_empty_state     : 1; // [1]
        unsigned int lfifo_single_state    : 1; // [2]
        unsigned int rfifo_single_state    : 1; // [3]
        unsigned int Reserved_0            : 28; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_INTR_STATE;

// Define the union U_SPDIF_CH_STATUS1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int l_use_a               : 1; // [0]
        unsigned int l_use_b               : 1; // [1]
        unsigned int l_use_c               : 1; // [2]
        unsigned int l_use_d               : 3; // [5..3]
        unsigned int l_use_mode            : 2; // [7..6]
        unsigned int r_use_a               : 1; // [8]
        unsigned int r_use_b               : 1; // [9]
        unsigned int r_use_c               : 1; // [10]
        unsigned int r_use_d               : 3; // [13..11]
        unsigned int r_use_mode            : 2; // [15..14]
        unsigned int Reserved_0            : 16; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_CH_STATUS1;

// Define the union U_SPDIF_CH_STATUS2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int l_category_code       : 8; // [7..0]
        unsigned int r_category_code       : 8; // [15..8]
        unsigned int Reserved_0            : 16; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_CH_STATUS2;

// Define the union U_SPDIF_CH_STATUS3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int l_src_nb              : 4; // [3..0]
        unsigned int l_ch_nb               : 4; // [7..4]
        unsigned int r_src_nb              : 4; // [11..8]
        unsigned int r_ch_nb               : 4; // [15..12]
        unsigned int Reserved_0            : 16; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_CH_STATUS3;

// Define the union U_SPDIF_CH_STATUS4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int l_freq                : 4; // [3..0]
        unsigned int l_clk_accuracy        : 2; // [5..4]
        unsigned int Reserved_1            : 2; // [7..6]
        unsigned int r_freq                : 4; // [11..8]
        unsigned int r_clk_accuracy        : 2; // [13..12]
        unsigned int Reserved_0            : 18; // [31..14]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_CH_STATUS4;

// Define the union U_SPDIF_CH_STATUS5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int l_bit_max             : 1; // [0]
        unsigned int l_bit_width           : 3; // [3..1]
        unsigned int l_org_freq            : 4; // [7..4]
        unsigned int r_bit_max             : 1; // [8]
        unsigned int r_bit_width           : 3; // [11..9]
        unsigned int r_org_freq            : 4; // [15..12]
        unsigned int Reserved_0            : 16; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_SPDIF_CH_STATUS5;

typedef struct
{
    volatile U_I2S_CRG_CFG0 I2S_CRG_CFG0;
    volatile U_I2S_CRG_CFG1 I2S_CRG_CFG1;
} S_AIAO_I2S_CRG_CFG_REGS_TYPE;

typedef struct
{
    volatile U_SPDIF_CRG_CFG0 SPDIF_CRG_CFG0;
    volatile U_SPDIF_CRG_CFG1 SPDIF_CRG_CFG1;
} S_AIAO_SPDIF_CRG_CFG_REGS_TYPE;

#ifdef HI_AIAO_TIMER_SUPPORT
typedef union
{
    struct
    {
        unsigned int aiao_mclk_div         : 27; // [26..0]
        unsigned int Reserved_0            : 5; // [31..27]
    } bits;
    unsigned int u32;
} U_TIMER_CRG_CFG0;

typedef union
{
    struct
    {
        unsigned int aiao_bclk_div         : 4; // [3..0]
        unsigned int aiao_fsclk_div        : 3; // [6..4]
        unsigned int Reserved_1            : 1; // [7]
        unsigned int aiao_cken             : 1; // [8]
        unsigned int aiao_srst_req         : 1; // [9]
        unsigned int aiao_bclk_oen         : 1; // [10]
        unsigned int aiao_bclk_sel         : 1; // [11]
        unsigned int aiao_bclkin_pctrl     : 1; // [12]
        unsigned int aiao_bclkout_pctrl    : 1; // [13]
        unsigned int Reserved_0            : 18; // [31..14]
    } bits;
    unsigned int u32;
} U_TIMER_CRG_CFG1;

typedef union
{
    struct
    {
        unsigned int timer_ch0_clr         : 1; // [0]
        unsigned int timer_ch1_clr         : 1; // [1]
        unsigned int Reserved_0            : 30; // [31..2]
    } bits;
    unsigned int u32;
} U_TIMER_CH_CLEAR;

typedef union
{
    struct
    {
        unsigned int timer_config          : 13; // [12..0]
        unsigned int Reserved_0            : 19; // [31..13]
    } bits;
    unsigned int u32;
} U_TIMER_CH_CONFIG;

typedef struct
{
    volatile U_TIMER_CH_CONFIG TIMER_CONFIG;
} S_AIAO_TIER_CONFIG;

typedef struct
{
    volatile U_TIMER_CH_CLEAR TIMER_INT_CLEAR;
} S_AIAO_TIER_CLEAR;

typedef struct
{
    volatile U_TIMER_CRG_CFG0 TIMER_CRG_CFG0;
    volatile U_TIMER_CRG_CFG1 TIMER_CRG_CFG1;
} S_AIAO_TIER_CRG_CFG_REGS_TYPE;
#endif

//==============================================================================
// Define the global struct
typedef struct
{
    volatile U_AIAO_INT_ENA                 AIAO_INT_ENA;
    volatile U_AIAO_INT_STATUS              AIAO_INT_STATUS;
    volatile U_AIAO_INT_RAW                 AIAO_INT_RAW;
    volatile unsigned int                   HW_CAPABILITY;
    volatile unsigned int                   SW_KEY0;
    volatile unsigned int                   SW_KEY1;
    volatile unsigned int                   SW_KEY2;
    volatile unsigned int                   SW_KEY3;
    volatile U_SPDIF_TX_MUX                 SPDIF_TX_MUX;
    volatile unsigned int                   HW_VERSION;
    volatile U_AIAO_SWITCH_RX_BCLK          AIAO_SWITCH_RX_BCLK;
    volatile U_AIAO_SWITCH_TX_BCLK          AIAO_SWITCH_TX_BCLK;
    volatile U_AIAO_STATUS                  AIAO_STATUS;
    volatile unsigned int                   Reserved0[((0x34 - 0x030) >> 2) - 1];
    volatile U_VHB_OUTSTANDING              OUT_STANDING;
#ifdef HI_AIAO_TIMER_SUPPORT
    volatile unsigned int                   Reserved1[((0x40 - 0x034) >> 2) - 1];
    volatile S_AIAO_TIER_CONFIG             TIMER_CONFIG[2];
    volatile S_AIAO_TIER_CLEAR              TIMER_CLEAR;
    volatile unsigned int                   Reserved2[((0x100 - 0x048) >> 2) - 1];
    volatile S_AIAO_I2S_CRG_CFG_REGS_TYPE   I2S_RX_CRG[6];
    volatile S_AIAO_TIER_CRG_CFG_REGS_TYPE  TIMER_CRG[2];
    volatile S_AIAO_I2S_CRG_CFG_REGS_TYPE   I2S_TX_CRG[8];
    volatile S_AIAO_SPDIF_CRG_CFG_REGS_TYPE SPDIF_CRG[3];
#else
    volatile unsigned int                   Reserved1[((0x100 - 0x034) >> 2) - 1];
    volatile S_AIAO_I2S_CRG_CFG_REGS_TYPE   I2S_RX_CRG[8];
    volatile S_AIAO_I2S_CRG_CFG_REGS_TYPE   I2S_TX_CRG[8];
    volatile S_AIAO_SPDIF_CRG_CFG_REGS_TYPE SPDIF_CRG[4];
#endif
} S_AIAO_COM_REGS_TYPE;

typedef struct
{
    volatile U_RX_IF_ATTRI       RX_IF_ATTRI;
    volatile U_RX_DSP_CTRL       RX_DSP_CTRL;
    volatile unsigned int        Reserved0[((0x1020 - 0x1004) >> 2) - 1];
    volatile U_RX_WS_CNT         RX_WS_CNT;
    volatile U_RX_BCLK_CNT       RX_BCLK_CNT;
    volatile unsigned int        Reserved1[((0x1080 - 0x1024) >> 2) - 1];
    volatile unsigned int        RX_BUFF_SADDR;
    volatile U_RX_BUFF_SIZE      RX_BUFF_SIZE;
    volatile U_RX_BUFF_WPTR      RX_BUFF_WPTR;
    volatile U_RX_BUFF_RPTR      RX_BUFF_RPTR;
    volatile U_RX_BUFF_ALFULL_TH RX_BUFF_ALFULL_TH;
    volatile U_RX_TRANS_SIZE     RX_TRANS_SIZE;
    volatile U_RX_WPTR_TMP       RX_WPTR_TMP;
    volatile unsigned int        Reserved2;
    volatile U_RX_INT_ENA        RX_INT_ENA;
    volatile U_RX_INT_RAW        RX_INT_RAW;
    volatile U_RX_INT_STATUS     RX_INT_STATUS;
    volatile U_RX_INT_CLR        RX_INT_CLR;
} S_AIAO_RX_REGS_TYPE;

typedef struct
{
    volatile U_TX_IF_ATTRI        TX_IF_ATTRI;
    volatile U_TX_DSP_CTRL        TX_DSP_CTRL;
    volatile U_TX_DSP_STATUS      TX_DSP_STATUS;
    volatile unsigned int         Reserved0[((0x2020 - 0x2008) >> 2) - 1];
    volatile U_TX_WS_CNT          TX_WS_CNT;
    volatile U_TX_BCLK_CNT        TX_BCLK_CNT;
    volatile unsigned int         Reserved1[((0x2080 - 0x2024) >> 2) - 1];
    volatile unsigned int         TX_BUFF_SADDR;
    volatile U_TX_BUFF_SIZE       TX_BUFF_SIZE;
    volatile U_TX_BUFF_WPTR       TX_BUFF_WPTR;
    volatile U_TX_BUFF_RPTR       TX_BUFF_RPTR;
    volatile U_TX_BUFF_ALEMPTY_TH TX_BUFF_ALEMPTY_TH;
    volatile U_TX_TRANS_SIZE      TX_TRANS_SIZE;
    volatile U_TX_RPTR_TMP        TX_RPTR_TMP;
    volatile unsigned int         Reserved2;
    volatile U_TX_INT_ENA         TX_INT_ENA;
    volatile U_TX_INT_RAW         TX_INT_RAW;
    volatile U_TX_INT_STATUS      TX_INT_STATUS;
    volatile U_TX_INT_CLR         TX_INT_CLR;
} S_AIAO_TX_REGS_TYPE;

typedef struct
{
    volatile U_SPDIFTX_IF_ATTRI        SPDIFTX_IF_ATTRI;
    volatile U_SPDIFTX_DSP_CTRL        SPDIFTX_DSP_CTRL;
    volatile U_SPDIFTX_DSP_STATUS      SPDIFTX_DSP_STATUS;
    volatile unsigned int              Reserved0[((0x3020 - 0x3008) >> 2) - 1];
    volatile U_SPDIFTX_WS_CNT          SPDIFTX_WS_CNT;
    volatile U_SPDIFTX_BCLK_CNT        SPDIFTX_BCLK_CNT;
    volatile unsigned int              Reserved1[((0x3080 - 0x3024) >> 2) - 1];
    volatile unsigned int              SPDIFTX_BUFF_SADDR;
    volatile U_SPDIFTX_BUFF_SIZE       SPDIFTX_BUFF_SIZE;
    volatile U_SPDIFTX_BUFF_WPTR       SPDIFTX_BUFF_WPTR;
    volatile U_SPDIFTX_BUFF_RPTR       SPDIFTX_BUFF_RPTR;
    volatile U_SPDIFTX_BUFF_ALEMPTY_TH SPDIFTX_BUFF_ALEMPTY_TH;
    volatile U_SPDIFTX_TRANS_SIZE      SPDIFTX_TRANS_SIZE;
    volatile U_SPDIFTX_RPTR_TMP        SPDIFTX_RPTR_TMP;
    volatile unsigned int              Reserved2;
    volatile U_SPDIFTX_INT_ENA         SPDIFTX_INT_ENA;
    volatile U_SPDIFTX_INT_RAW         SPDIFTX_INT_RAW;
    volatile U_SPDIFTX_INT_STATUS      SPDIFTX_INT_STATUS;
    volatile U_SPDIFTX_INT_CLR         SPDIFTX_INT_CLR;
} S_AIAO_TXSPDIF_REGS_TYPE;

typedef struct
{
    volatile U_SPDIF_CTRL       SPDIF_CTRL;
    volatile U_SPDIF_CONFIG     SPDIF_CONFIG;
    volatile U_SPDIF_INTRUPT    SPDIF_INTRUPT;
    volatile U_SPDIF_INTR_MASK  SPDIF_INTR_MASK;
    volatile U_SPDIF_INTR_STATE SPDIF_INTR_STATE;
    volatile unsigned int       Reserved0[((0x4020 - 0x4010) >> 2) - 1];
    volatile U_SPDIF_CH_STATUS1 SPDIF_CH_STATUS1;
    volatile U_SPDIF_CH_STATUS2 SPDIF_CH_STATUS2;
    volatile U_SPDIF_CH_STATUS3 SPDIF_CH_STATUS3;
    volatile U_SPDIF_CH_STATUS4 SPDIF_CH_STATUS4;
    volatile U_SPDIF_CH_STATUS5 SPDIF_CH_STATUS5;
    volatile unsigned int       Reserved1[((0x4040 - 0x4030) >> 2) - 1];
    volatile unsigned int       SPDIF_USER_DATA1;
    volatile unsigned int       SPDIF_USER_DATA2;
    volatile unsigned int       SPDIF_USER_DATA3;
    volatile unsigned int       SPDIF_USER_DATA4;
    volatile unsigned int       SPDIF_USER_DATA5;
    volatile unsigned int       Reserved2[((0x40f0 - 0x4050) >> 2) - 1];
    volatile unsigned int       SPDIF_EDITION;
} S_AIAO_SPDIFER_REGS_TYPE;

/*****************************************************************************
 Description  : AIAO Golbal HAL API
*****************************************************************************/
HI_S32                  AIAO_HW_PowerOn(HI_VOID);
HI_VOID                 AIAO_HW_PowerOff(HI_VOID);
HI_S32                  AIAO_HW_Init(HI_VOID);
HI_VOID                 AIAO_HW_DeInit(HI_VOID);
HI_VOID                 AIAO_HW_GetHwCapability(HI_U32* pu32Capability);
HI_VOID                 AIAO_HW_GetHwVersion(HI_U32* pu32Version);
HI_VOID                 AIAO_HW_DBG_RWReg(AIAO_Dbg_Reg_S* pstReg);

HI_VOID                 AIAO_HW_SetTopInt(HI_U32 u32Multibit);
HI_U32                  AIAO_HW_GetTopIntRawStatus(HI_VOID);
HI_U32                  AIAO_HW_GetTopIntStatus(HI_VOID);

/*****************************************************************************
 Description  : AIAO Rx/Tx Port CRG/I2s Attr HAL API
*****************************************************************************/
HI_VOID                 AIAO_HW_SetIfAttr(AIAO_PORT_ID_E enPortID, AIAO_IfAttr_S* pstIfAttr);
HI_VOID                 AIAO_HW_SetSPDIFPortEn(AIAO_PORT_ID_E enPortID, HI_S32 bEn);
HI_VOID                 AIAO_HW_SetSPDIFPortSelect(AIAO_PORT_ID_E enPortID, AIAO_SPDIFPORT_SOURCE_E eSrcChnId);
HI_VOID                 AIAO_HW_SetI2SDataSelect(AIAO_PORT_ID_E enPortID, AIAO_I2S_SD_E eOrgSd, AIAO_I2S_SD_E eSrcSd);

/*****************************************************************************
 Description  : AIAO TX/RX Port DSP Control HAL API
*****************************************************************************/
HI_VOID                 AIAO_HW_SetInt(AIAO_PORT_ID_E enPortID, HI_U32 u32Multibit);
HI_VOID                 AIAO_HW_ClrInt(AIAO_PORT_ID_E enPortID, HI_U32 u32Multibit);
HI_U32                  AIAO_HW_GetIntStatusRaw(AIAO_PORT_ID_E enPortID);
HI_U32                  AIAO_HW_GetIntStatus(AIAO_PORT_ID_E enPortID);

/*****************************************************************************
 Description  : AIAO TX/RX Port DSP Control HAL API
*****************************************************************************/
HI_S32                  AIAO_HW_SetStart(AIAO_PORT_ID_E enPortID, HI_S32 bEn);
HI_S32                  AIAO_HW_GetStopDoneStatus(AIAO_PORT_ID_E enPortID);
HI_VOID                 AIAO_HW_SetBypass(AIAO_PORT_ID_E enPortID, HI_S32 bEn);
HI_VOID                 AIAO_HW_SetMode(AIAO_PORT_ID_E enPortID, HI_S32 bEn);
HI_VOID                 AIAO_HW_SetVolumedB(AIAO_PORT_ID_E enPortID, HI_S32 VoldB);
HI_VOID                 AIAO_HW_SetSPDIF_Category_SCMS(AIAO_PORT_ID_E enPortID, AIAO_SPDIF_SCMS_MODE_E eSCMSMode, AIAO_SPDIF_CATEGORYCODE_E enCategoryCode);
HI_VOID                 AIAO_HW_SetFadeOutRate(AIAO_PORT_ID_E enPortID, AIAO_FADE_RATE_E eFadeRate);
HI_VOID                 AIAO_HW_SetFadeInRate(AIAO_PORT_ID_E enPortID, AIAO_FADE_RATE_E eFadeRate);
HI_VOID                 AIAO_HW_SetMuteFade(AIAO_PORT_ID_E enPortID, HI_S32 bEn);
HI_VOID                 AIAO_HW_SetMute(AIAO_PORT_ID_E enPortID, HI_S32 bEn);
HI_VOID                 AIAO_HW_SetTrackMode(AIAO_PORT_ID_E enPortID, AIAO_TRACK_MODE_E eTrackMode);
HI_VOID                 AIAO_HW_SetI2SMasterClk(AIAO_PORT_ID_E enPortID, AIAO_IfAttr_S* pstIfAttrMaster);
HI_VOID                 AIAO_HW_SetI2SSlaveClk(AIAO_PORT_ID_E enPortID, AIAO_IfAttr_S* pstIfAttrSlave);

/*****************************************************************************
 Description  : AIAO TX/RX Port Buffer HAL API
*****************************************************************************/
HI_VOID                 AIAO_HW_GetRptrAndWptrRegAddr(AIAO_PORT_ID_E enPortID, HI_U32** ppu32WptrReg,
        HI_U32** ppu32RptrReg);
// new
HI_VOID AIAO_HW_GetRptrAndWptrRegPhyAddr(AIAO_PORT_ID_E enPortID, HI_U32* pu32WptrReg, HI_U32* pu32RptrReg);
HI_VOID                 AIAO_HW_GetBufu32Wptr(AIAO_PORT_ID_E enPortID, HI_U32* pu32Wptr);
HI_VOID                 AIAO_HW_GetBufu32Rptr(AIAO_PORT_ID_E enPortID, HI_U32* pu32Rptr);
HI_VOID                 AIAO_HW_SetBufAddrAndSize(AIAO_PORT_ID_E enPortID, HI_U32 u32StartAddr, HI_U32 u32Size);
HI_VOID                 AIAO_HW_SetBufWptr(AIAO_PORT_ID_E enPortID, HI_U32 u32Wptr);
HI_VOID                 AIAO_HW_SetBufRptr(AIAO_PORT_ID_E enPortID, HI_U32 u32Rptr);
HI_VOID                 AIAO_HW_SetBuf(AIAO_PORT_ID_E enPortID, AIAO_BufInfo_S* pstBuf);
HI_VOID                 AIAO_HW_GetDbgBclkCnt(AIAO_PORT_ID_E enPortID, HI_U32* pu32BclkCnt);
HI_VOID                 AIAO_HW_GetDbgWsCnt(AIAO_PORT_ID_E enPortID, HI_U32* pu32WsCnt);
HI_VOID                 AIAO_HW_SetOpType(AIAO_PORT_ID_E enPortID, AIAO_OP_TYPE_E enOpType);
#ifdef HI_AIAO_TIMER_SUPPORT
HI_VOID  AIAO_TIMER_SetEnable(AIAO_TIMER_ID_E enTimerID, HI_S32 bEn);
HI_VOID  AIAO_TIMER_SetAttr(AIAO_TIMER_ID_E enTimerID, const AIAO_IfTimerAttr_S* pstIfAttr, HI_U32 u32Config);
HI_VOID  AIAO_TIMER_ClearTimer(AIAO_TIMER_ID_E enTimerID);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // __HI_HAL_AIAO_PRIV_H__
