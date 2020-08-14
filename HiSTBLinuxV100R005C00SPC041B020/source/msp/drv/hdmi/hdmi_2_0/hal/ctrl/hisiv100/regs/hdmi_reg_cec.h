// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  tx_cec_reg_c_union_define.h
// Project line  :  Platform And Key Technologies Development
// Department    :  Device Chipset Source Algorithm and Chipset Platform Development Dept
// Author        :  w00226427
// Version       :  1.0
// Date          :  2015/1/1
// Description   :  HDMIMHL_TX_HISI
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00226427 2015/10/08 20:14:09 Create file
// ******************************************************************************

#ifndef __TX_CEC_REG_C_UNION_DEFINE_H__
#define __TX_CEC_REG_C_UNION_DEFINE_H__

/* Define the union U_CEC_DEV_INFO */
/* Define the union U_CEC_DEV_INFO,ATTR:,ADDR:4800,INIT:A08014CC,MASK:ff8fffff,CMMNT:CEC Éè±¸ÐÅÏ¢¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_device_id   : 8  ; /* [7:0] */
        unsigned int    cec_spec_ver    : 8  ; /* [15:8] */
        unsigned int    cec_spec_suffix : 4  ; /* [19:16] */
        unsigned int    rsv_0           : 3  ; /* [22:20] */
        unsigned int    cec_sub_sys     : 1  ; /* [23] */
        unsigned int    cec_hw_ver      : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_DEV_INFO;

/* Define the union U_CEC_START_PRIOD */
/* Define the union U_CEC_START_PRIOD,ATTR:,ADDR:4804,INIT:00000000,MASK:ffff,CMMNT:CEC STARTÎ»Debug¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_start_bit_priod : 8  ; /* [7:0] */
        unsigned int    cec_start_low_priod : 8  ; /* [15:8] */
        unsigned int    rsv_1               : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_START_PRIOD;

/* Define the union U_CEC_TX_CTRL */
/* Define the union U_CEC_TX_CTRL,ATTR:,ADDR:4808,INIT:10060000,MASK:ffffffff,CMMNT:CEC Ä£¿é¿ØÖÆ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_bus_owner                   : 4  ; /* [3:0] */
        unsigned int    cec_snoop_initiator             : 4  ; /* [7:4] */
        unsigned int    cec_reg_snoop                   : 1  ; /* [8] */
        unsigned int    cec_reg_nack_hdr                : 1  ; /* [9] */
        unsigned int    cec_inv_ack_brcst               : 1  ; /* [10] */
        unsigned int    cec_ctl_retry_cnt               : 3  ; /* [13:11] */
        unsigned int    cec_reg_flush_tx_ff             : 1  ; /* [14] */
        unsigned int    cec_reg_calib_cec               : 1  ; /* [15] */
        unsigned int    cec_reg_calib_cec_en            : 1  ; /* [16] */
        unsigned int    cec_reg_force_non_calib         : 1  ; /* [17] */
        unsigned int    cec_reg_i2c_cec_passthru        : 1  ; /* [18] */
        unsigned int    manual_cmd_set                  : 1  ; /* [19] */
        unsigned int    cec_reg_tx_auto_calc            : 1  ; /* [20] */
        unsigned int    cec_reg_tx_bfr_ac               : 1  ; /* [21] */
        unsigned int    cec_reg_tx_cmd_cnt              : 4  ; /* [25:22] */
        unsigned int    cec_reg_tx_retry_limit          : 3  ; /* [28:26] */
        unsigned int    cec_reg_rx_clr_cur_set          : 1  ; /* [29] */
        unsigned int    cec_reg_rx_clr_all              : 1  ; /* [30] */
        unsigned int    auto_clear_intrp_rx_fifo_nempty : 1  ; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_TX_CTRL;

/* Define the union U_CEC_RX_STATE */
/* Define the union U_CEC_RX_STATE,ATTR:,ADDR:480C,INIT:00000000,MASK:ffbf,CMMNT:CEC Ä£¿éRX ×´Ì¬´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_reg_rx_cmd_byte_cnt : 4  ; /* [3:0] */
        unsigned int    cec_reg_rx_ff_wr_sel    : 2  ; /* [5:4] */
        unsigned int    rsv_2                   : 1  ; /* [6] */
        unsigned int    cec_rx_error            : 1  ; /* [7] */
        unsigned int    cec_rx_dest_cmd_header  : 4  ; /* [11:8] */
        unsigned int    cec_rx_init_cmd_header  : 4  ; /* [15:12] */
        unsigned int    rsv_3                   : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_RX_STATE;

/* Define the union U_CEC_AUTO_DISC */
/* Define the union U_CEC_AUTO_DISC,ATTR:,ADDR:4810,INIT:00000000,MASK:7,CMMNT:CEC ×Ô¶¯DiscoveryÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_auto_ping_start : 1  ; /* [0] */
        unsigned int    cec_auto_ping_clear : 1  ; /* [1] */
        unsigned int    cec_auto_ping_done  : 1  ; /* [2] */
        unsigned int    rsv_4               : 29  ; /* [31:3] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_AUTO_DISC;

/* Define the union U_CEC_CDC_CTRL */
/* Define the union U_CEC_CDC_CTRL,ATTR:,ADDR:4814,INIT:0000F823,MASK:ff3f,CMMNT:CEC CDC ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_cdc_arb_cnt : 5  ; /* [4:0] */
        unsigned int    cec_cdc_arb_en  : 1  ; /* [5] */
        unsigned int    rsv_5           : 2  ; /* [7:6] */
        unsigned int    cec_cdc_opcode  : 8  ; /* [15:8] */
        unsigned int    rsv_6           : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_CDC_CTRL;

/* Define the union U_CEC_AUTODISC_MAP */
/* Define the union U_CEC_AUTODISC_MAP,ATTR:,ADDR:4818,INIT:00000000,MASK:ffff,CMMNT:CEC PING Mapping¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_auto_ping_map : 16  ; /* [15:0] */
        unsigned int    rsv_7             : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_AUTODISC_MAP;

/* Define the union U_CEC_TX_INIT */
/* Define the union U_CEC_TX_INIT,ATTR:,ADDR:481C,INIT:00000000,MASK:f,CMMNT:CEC INIT ID ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_tx_init_id : 4  ; /* [3:0] */
        unsigned int    rsv_8          : 28  ; /* [31:4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_TX_INIT;

/* Define the union U_CEC_TX_DEST */
/* Define the union U_CEC_TX_DEST,ATTR:,ADDR:4820,INIT:00000000,MASK:1f,CMMNT:CEC DEST ID ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_tx_dest_id         : 4  ; /* [3:0] */
        unsigned int    cec_reg_sd_poll_intern : 1  ; /* [4] */
        unsigned int    rsv_9                  : 27  ; /* [31:5] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_TX_DEST;

/* Define the union U_CEC_CAPTUTR_ID */
/* Define the union U_CEC_CAPTUTR_ID,ATTR:,ADDR:4824,INIT:00000000,MASK:ffff,CMMNT:CEC CAPTURE IDÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_capture_id : 16  ; /* [15:0] */
        unsigned int    rsv_10         : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_CAPTUTR_ID;

/* Define the union U_TX_CEC_CMD0 */
/* Define the union U_TX_CEC_CMD0,ATTR:,ADDR:4828,INIT:00000000,MASK:ffffffff,CMMNT:TX CEC Ö¸Áî¼Ä´æÆ÷0¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_cec_command  : 8  ; /* [7:0] */
        unsigned int    tx_cec_operand0 : 8  ; /* [15:8] */
        unsigned int    tx_cec_operand1 : 8  ; /* [23:16] */
        unsigned int    tx_cec_operand2 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_CEC_CMD0;

/* Define the union U_TX_CEC_CMD1 */
/* Define the union U_TX_CEC_CMD1,ATTR:,ADDR:482C,INIT:00000000,MASK:ffffffff,CMMNT:TX CEC Ö¸Áî¼Ä´æÆ÷0¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_cec_operand3 : 8  ; /* [7:0] */
        unsigned int    tx_cec_operand4 : 8  ; /* [15:8] */
        unsigned int    tx_cec_operand5 : 8  ; /* [23:16] */
        unsigned int    tx_cec_operand6 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_CEC_CMD1;

/* Define the union U_TX_CEC_CMD2 */
/* Define the union U_TX_CEC_CMD2,ATTR:,ADDR:4830,INIT:00000000,MASK:ffffffff,CMMNT:TX CEC Ö¸Áî¼Ä´æÆ÷0¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_cec_operand7  : 8  ; /* [7:0] */
        unsigned int    tx_cec_operand8  : 8  ; /* [15:8] */
        unsigned int    tx_cec_operand9  : 8  ; /* [23:16] */
        unsigned int    tx_cec_operand10 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_CEC_CMD2;

/* Define the union U_TX_CEC_CMD3 */
/* Define the union U_TX_CEC_CMD3,ATTR:,ADDR:4834,INIT:00000000,MASK:ffffffff,CMMNT:TX CEC Ö¸Áî¼Ä´æÆ÷0¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_cec_operand11 : 8  ; /* [7:0] */
        unsigned int    tx_cec_operand12 : 8  ; /* [15:8] */
        unsigned int    tx_cec_operand13 : 8  ; /* [23:16] */
        unsigned int    tx_cec_operand14 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_CEC_CMD3;

/* Define the union U_RX_CEC_CMD0 */
/* Define the union U_RX_CEC_CMD0,ATTR:,ADDR:4838,INIT:00000000,MASK:ffffffff,CMMNT:RX CEC Ö¸Áî¼Ä´æÆ÷0¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rx_cec_command  : 8  ; /* [7:0] */
        unsigned int    rx_cec_operand0 : 8  ; /* [15:8] */
        unsigned int    rx_cec_operand1 : 8  ; /* [23:16] */
        unsigned int    rx_cec_operand2 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RX_CEC_CMD0;

/* Define the union U_RX_CEC_CMD1 */
/* Define the union U_RX_CEC_CMD1,ATTR:,ADDR:483C,INIT:00000000,MASK:ffffffff,CMMNT:RX CEC Ö¸Áî¼Ä´æÆ÷0¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rx_cec_operand3 : 8  ; /* [7:0] */
        unsigned int    rx_cec_operand4 : 8  ; /* [15:8] */
        unsigned int    rx_cec_operand5 : 8  ; /* [23:16] */
        unsigned int    rx_cec_operand6 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RX_CEC_CMD1;

/* Define the union U_RX_CEC_CMD2 */
/* Define the union U_RX_CEC_CMD2,ATTR:,ADDR:4840,INIT:00000000,MASK:ffffffff,CMMNT:RX CEC Ö¸Áî¼Ä´æÆ÷0¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rx_cec_operand7  : 8  ; /* [7:0] */
        unsigned int    rx_cec_operand8  : 8  ; /* [15:8] */
        unsigned int    rx_cec_operand9  : 8  ; /* [23:16] */
        unsigned int    rx_cec_operand10 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RX_CEC_CMD2;

/* Define the union U_RX_CEC_CMD3 */
/* Define the union U_RX_CEC_CMD3,ATTR:,ADDR:4844,INIT:00000000,MASK:ffffffff,CMMNT:RX CEC Ö¸Áî¼Ä´æÆ÷0¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rx_cec_operand11 : 8  ; /* [7:0] */
        unsigned int    rx_cec_operand12 : 8  ; /* [15:8] */
        unsigned int    rx_cec_operand13 : 8  ; /* [23:16] */
        unsigned int    rx_cec_operand14 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RX_CEC_CMD3;

/* Define the union U_CEC_OP_ABORT_0 */
/* Define the union U_CEC_OP_ABORT_0,ATTR:,ADDR:4848,INIT:00000000,MASK:ffffffff,CMMNT:CEC Feature Abort ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_op_abort_0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_OP_ABORT_0;

/* Define the union U_CEC_OP_ABORT_1 */
/* Define the union U_CEC_OP_ABORT_1,ATTR:,ADDR:484C,INIT:00000000,MASK:ffffffff,CMMNT:CEC Feature Abort ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_op_abort_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_OP_ABORT_1;

/* Define the union U_CEC_OP_ABORT_2 */
/* Define the union U_CEC_OP_ABORT_2,ATTR:,ADDR:4850,INIT:00000000,MASK:ffffffff,CMMNT:CEC Feature Abort ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_op_abort_2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_OP_ABORT_2;

/* Define the union U_CEC_OP_ABORT_3 */
/* Define the union U_CEC_OP_ABORT_3,ATTR:,ADDR:4854,INIT:00000000,MASK:ffffffff,CMMNT:CEC Feature Abort ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_op_abort_3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_OP_ABORT_3;

/* Define the union U_CEC_OP_ABORT_4 */
/* Define the union U_CEC_OP_ABORT_4,ATTR:,ADDR:4858,INIT:00000000,MASK:ffffffff,CMMNT:CEC Feature Abort ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_op_abort_4 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_OP_ABORT_4;

/* Define the union U_CEC_OP_ABORT_5 */
/* Define the union U_CEC_OP_ABORT_5,ATTR:,ADDR:485C,INIT:00000000,MASK:ffffffff,CMMNT:CEC Feature Abort ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_op_abort_5 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_OP_ABORT_5;

/* Define the union U_CEC_OP_ABORT_6 */
/* Define the union U_CEC_OP_ABORT_6,ATTR:,ADDR:4860,INIT:00000000,MASK:ffffffff,CMMNT:CEC Feature Abort ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_op_abort_6 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_OP_ABORT_6;

/* Define the union U_CEC_OP_ABORT_7 */
/* Define the union U_CEC_OP_ABORT_7,ATTR:,ADDR:4864,INIT:00000000,MASK:ffffffff,CMMNT:CEC Feature Abort ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cec_op_abort_7 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEC_OP_ABORT_7;


//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_CEC_DEV_INFO     CEC_DEV_INFO     ; /* 4800 */
    volatile U_CEC_START_PRIOD  CEC_START_PRIOD  ; /* 4804 */
    volatile U_CEC_TX_CTRL      CEC_TX_CTRL      ; /* 4808 */
    volatile U_CEC_RX_STATE     CEC_RX_STATE     ; /* 480C */
    volatile U_CEC_AUTO_DISC    CEC_AUTO_DISC    ; /* 4810 */
    volatile U_CEC_CDC_CTRL     CEC_CDC_CTRL     ; /* 4814 */
    volatile U_CEC_AUTODISC_MAP CEC_AUTODISC_MAP ; /* 4818 */
    volatile U_CEC_TX_INIT      CEC_TX_INIT      ; /* 481C */
    volatile U_CEC_TX_DEST      CEC_TX_DEST      ; /* 4820 */
    volatile U_CEC_CAPTUTR_ID   CEC_CAPTUTR_ID   ; /* 4824 */
    volatile U_TX_CEC_CMD0      TX_CEC_CMD0      ; /* 4828 */
    volatile U_TX_CEC_CMD1      TX_CEC_CMD1      ; /* 482C */
    volatile U_TX_CEC_CMD2      TX_CEC_CMD2      ; /* 4830 */
    volatile U_TX_CEC_CMD3      TX_CEC_CMD3      ; /* 4834 */
    volatile U_RX_CEC_CMD0      RX_CEC_CMD0      ; /* 4838 */
    volatile U_RX_CEC_CMD1      RX_CEC_CMD1      ; /* 483C */
    volatile U_RX_CEC_CMD2      RX_CEC_CMD2      ; /* 4840 */
    volatile U_RX_CEC_CMD3      RX_CEC_CMD3      ; /* 4844 */
    volatile U_CEC_OP_ABORT_0   CEC_OP_ABORT_0   ; /* 4848 */
    volatile U_CEC_OP_ABORT_1   CEC_OP_ABORT_1   ; /* 484C */
    volatile U_CEC_OP_ABORT_2   CEC_OP_ABORT_2   ; /* 4850 */
    volatile U_CEC_OP_ABORT_3   CEC_OP_ABORT_3   ; /* 4854 */
    volatile U_CEC_OP_ABORT_4   CEC_OP_ABORT_4   ; /* 4858 */
    volatile U_CEC_OP_ABORT_5   CEC_OP_ABORT_5   ; /* 485C */
    volatile U_CEC_OP_ABORT_6   CEC_OP_ABORT_6   ; /* 4860 */
    volatile U_CEC_OP_ABORT_7   CEC_OP_ABORT_7   ; /* 4864 */

} S_tx_cec_reg_REGS_TYPE;

/* Declare the struct pointor of the module tx_cec_reg */
//extern volatile S_tx_cec_reg_REGS_TYPE *goptx_cec_regAllReg;

int HDMI_TX_S_tx_cec_reg_REGS_TYPE_Init(void);
int HDMI_TX_S_tx_cec_reg_REGS_TYPE_DeInit(void);

/* Declare the functions that set the member value */
int HDMI_CEC_DEV_INFO_cec_device_idSet(unsigned int ucec_device_id);
int HDMI_CEC_DEV_INFO_cec_spec_verSet(unsigned int ucec_spec_ver);
int HDMI_CEC_DEV_INFO_cec_spec_suffixSet(unsigned int ucec_spec_suffix);
int HDMI_CEC_DEV_INFO_cec_sub_sysSet(unsigned int ucec_sub_sys);
int HDMI_CEC_DEV_INFO_cec_hw_verSet(unsigned int ucec_hw_ver);
int HDMI_CEC_START_PRIOD_cec_start_bit_priodSet(unsigned int ucec_start_bit_priod);
int HDMI_CEC_START_PRIOD_cec_start_low_priodSet(unsigned int ucec_start_low_priod);
int HDMI_CEC_TX_CTRL_cec_bus_ownerSet(unsigned int ucec_bus_owner);
int HDMI_CEC_TX_CTRL_cec_snoop_initiatorSet(unsigned int ucec_snoop_initiator);
int HDMI_CEC_TX_CTRL_cec_reg_snoopSet(unsigned int ucec_reg_snoop);
int HDMI_CEC_TX_CTRL_cec_reg_nack_hdrSet(unsigned int ucec_reg_nack_hdr);
int HDMI_CEC_TX_CTRL_cec_inv_ack_brcstSet(unsigned int ucec_inv_ack_brcst);
int HDMI_CEC_TX_CTRL_cec_ctl_retry_cntSet(unsigned int ucec_ctl_retry_cnt);
int HDMI_CEC_TX_CTRL_cec_reg_flush_tx_ffSet(unsigned int ucec_reg_flush_tx_ff);
int HDMI_CEC_TX_CTRL_cec_reg_calib_cecSet(unsigned int ucec_reg_calib_cec);
int HDMI_CEC_TX_CTRL_cec_reg_calib_cec_enSet(unsigned int ucec_reg_calib_cec_en);
int HDMI_CEC_TX_CTRL_cec_reg_force_non_calibSet(unsigned int ucec_reg_force_non_calib);
int HDMI_CEC_TX_CTRL_cec_reg_i2c_cec_passthruSet(unsigned int ucec_reg_i2c_cec_passthru);
int HDMI_CEC_TX_CTRL_manual_cmd_setSet(unsigned int umanual_cmd_set);
int HDMI_CEC_TX_CTRL_cec_reg_tx_auto_calcSet(unsigned int ucec_reg_tx_auto_calc);
int HDMI_CEC_TX_CTRL_cec_reg_tx_bfr_acSet(unsigned int ucec_reg_tx_bfr_ac);
int HDMI_CEC_TX_CTRL_cec_reg_tx_cmd_cntSet(unsigned int ucec_reg_tx_cmd_cnt);
int HDMI_CEC_TX_CTRL_cec_reg_tx_retry_limitSet(unsigned int ucec_reg_tx_retry_limit);
int HDMI_CEC_TX_CTRL_cec_reg_rx_clr_cur_setSet(unsigned int ucec_reg_rx_clr_cur_set);
int HDMI_CEC_TX_CTRL_cec_reg_rx_clr_allSet(unsigned int ucec_reg_rx_clr_all);
int HDMI_CEC_TX_CTRL_auto_clear_intrp_rx_fifo_nemptySet(unsigned int uauto_clear_intrp_rx_fifo_nempty);
int HDMI_CEC_RX_STATE_cec_reg_rx_cmd_byte_cntSet(unsigned int ucec_reg_rx_cmd_byte_cnt);
int HDMI_CEC_RX_STATE_cec_reg_rx_ff_wr_selSet(unsigned int ucec_reg_rx_ff_wr_sel);
int HDMI_CEC_RX_STATE_cec_rx_errorSet(unsigned int ucec_rx_error);
int HDMI_CEC_RX_STATE_cec_rx_dest_cmd_headerSet(unsigned int ucec_rx_dest_cmd_header);
int HDMI_CEC_RX_STATE_cec_rx_init_cmd_headerSet(unsigned int ucec_rx_init_cmd_header);
int HDMI_CEC_AUTO_DISC_cec_auto_ping_startSet(unsigned int ucec_auto_ping_start);
int HDMI_CEC_AUTO_DISC_cec_auto_ping_clearSet(unsigned int ucec_auto_ping_clear);
int HDMI_CEC_AUTO_DISC_cec_auto_ping_doneSet(unsigned int ucec_auto_ping_done);
int HDMI_CEC_CDC_CTRL_cec_cdc_arb_cntSet(unsigned int ucec_cdc_arb_cnt);
int HDMI_CEC_CDC_CTRL_cec_cdc_arb_enSet(unsigned int ucec_cdc_arb_en);
int HDMI_CEC_CDC_CTRL_cec_cdc_opcodeSet(unsigned int ucec_cdc_opcode);
int HDMI_CEC_AUTODISC_MAP_cec_auto_ping_mapSet(unsigned int ucec_auto_ping_map);
int HDMI_CEC_TX_INIT_cec_tx_init_idSet(unsigned int ucec_tx_init_id);
int HDMI_CEC_TX_DEST_cec_tx_dest_idSet(unsigned int ucec_tx_dest_id);
int HDMI_CEC_TX_DEST_cec_reg_sd_poll_internSet(unsigned int ucec_reg_sd_poll_intern);
int HDMI_CEC_CAPTUTR_ID_cec_capture_idSet(unsigned int ucec_capture_id);
int HDMI_TX_CEC_CMD0_tx_cec_commandSet(unsigned int utx_cec_command);
int HDMI_TX_CEC_CMD0_tx_cec_operand0Set(unsigned int utx_cec_operand0);
int HDMI_TX_CEC_CMD0_tx_cec_operand1Set(unsigned int utx_cec_operand1);
int HDMI_TX_CEC_CMD0_tx_cec_operand2Set(unsigned int utx_cec_operand2);
int HDMI_TX_CEC_CMD1_tx_cec_operand3Set(unsigned int utx_cec_operand3);
int HDMI_TX_CEC_CMD1_tx_cec_operand4Set(unsigned int utx_cec_operand4);
int HDMI_TX_CEC_CMD1_tx_cec_operand5Set(unsigned int utx_cec_operand5);
int HDMI_TX_CEC_CMD1_tx_cec_operand6Set(unsigned int utx_cec_operand6);
int HDMI_TX_CEC_CMD2_tx_cec_operand7Set(unsigned int utx_cec_operand7);
int HDMI_TX_CEC_CMD2_tx_cec_operand8Set(unsigned int utx_cec_operand8);
int HDMI_TX_CEC_CMD2_tx_cec_operand9Set(unsigned int utx_cec_operand9);
int HDMI_TX_CEC_CMD2_tx_cec_operand10Set(unsigned int utx_cec_operand10);
int HDMI_TX_CEC_CMD3_tx_cec_operand11Set(unsigned int utx_cec_operand11);
int HDMI_TX_CEC_CMD3_tx_cec_operand12Set(unsigned int utx_cec_operand12);
int HDMI_TX_CEC_CMD3_tx_cec_operand13Set(unsigned int utx_cec_operand13);
int HDMI_TX_CEC_CMD3_tx_cec_operand14Set(unsigned int utx_cec_operand14);
int HDMI_RX_CEC_CMD0_rx_cec_commandSet(unsigned int urx_cec_command);
int HDMI_RX_CEC_CMD0_rx_cec_operand0Set(unsigned int urx_cec_operand0);
int HDMI_RX_CEC_CMD0_rx_cec_operand1Set(unsigned int urx_cec_operand1);
int HDMI_RX_CEC_CMD0_rx_cec_operand2Set(unsigned int urx_cec_operand2);
int HDMI_RX_CEC_CMD1_rx_cec_operand3Set(unsigned int urx_cec_operand3);
int HDMI_RX_CEC_CMD1_rx_cec_operand4Set(unsigned int urx_cec_operand4);
int HDMI_RX_CEC_CMD1_rx_cec_operand5Set(unsigned int urx_cec_operand5);
int HDMI_RX_CEC_CMD1_rx_cec_operand6Set(unsigned int urx_cec_operand6);
int HDMI_RX_CEC_CMD2_rx_cec_operand7Set(unsigned int urx_cec_operand7);
int HDMI_RX_CEC_CMD2_rx_cec_operand8Set(unsigned int urx_cec_operand8);
int HDMI_RX_CEC_CMD2_rx_cec_operand9Set(unsigned int urx_cec_operand9);
int HDMI_RX_CEC_CMD2_rx_cec_operand10Set(unsigned int urx_cec_operand10);
int HDMI_RX_CEC_CMD3_rx_cec_operand11Set(unsigned int urx_cec_operand11);
int HDMI_RX_CEC_CMD3_rx_cec_operand12Set(unsigned int urx_cec_operand12);
int HDMI_RX_CEC_CMD3_rx_cec_operand13Set(unsigned int urx_cec_operand13);
int HDMI_RX_CEC_CMD3_rx_cec_operand14Set(unsigned int urx_cec_operand14);
int HDMI_CEC_OP_ABORT_0_cec_op_abort_0Set(unsigned int ucec_op_abort_0);
int HDMI_CEC_OP_ABORT_1_cec_op_abort_1Set(unsigned int ucec_op_abort_1);
int HDMI_CEC_OP_ABORT_2_cec_op_abort_2Set(unsigned int ucec_op_abort_2);
int HDMI_CEC_OP_ABORT_3_cec_op_abort_3Set(unsigned int ucec_op_abort_3);
int HDMI_CEC_OP_ABORT_4_cec_op_abort_4Set(unsigned int ucec_op_abort_4);
int HDMI_CEC_OP_ABORT_5_cec_op_abort_5Set(unsigned int ucec_op_abort_5);
int HDMI_CEC_OP_ABORT_6_cec_op_abort_6Set(unsigned int ucec_op_abort_6);
int HDMI_CEC_OP_ABORT_7_cec_op_abort_7Set(unsigned int ucec_op_abort_7);
/* Declare the functions that get the member value */
int HDMI_CEC_DEV_INFO_cec_device_idGet(HI_VOID);
int HDMI_CEC_DEV_INFO_cec_spec_verGet(HI_VOID);
int HDMI_CEC_DEV_INFO_cec_spec_suffixGet(HI_VOID);
int HDMI_CEC_DEV_INFO_cec_sub_sysGet(HI_VOID);
int HDMI_CEC_DEV_INFO_cec_hw_verGet(HI_VOID);
int HDMI_CEC_START_PRIOD_cec_start_bit_priodGet(HI_VOID);
int HDMI_CEC_START_PRIOD_cec_start_low_priodGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_bus_ownerGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_snoop_initiatorGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_snoopGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_nack_hdrGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_inv_ack_brcstGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_ctl_retry_cntGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_flush_tx_ffGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_calib_cecGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_calib_cec_enGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_force_non_calibGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_i2c_cec_passthruGet(HI_VOID);
int HDMI_CEC_TX_CTRL_manual_cmd_setGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_tx_auto_calcGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_tx_bfr_acGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_tx_cmd_cntGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_tx_retry_limitGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_rx_clr_cur_setGet(HI_VOID);
int HDMI_CEC_TX_CTRL_cec_reg_rx_clr_allGet(HI_VOID);
int HDMI_CEC_TX_CTRL_auto_clear_intrp_rx_fifo_nemptyGet(HI_VOID);
int HDMI_CEC_RX_STATE_cec_reg_rx_cmd_byte_cntGet(HI_VOID);
int HDMI_CEC_RX_STATE_cec_reg_rx_ff_wr_selGet(HI_VOID);
int HDMI_CEC_RX_STATE_cec_rx_errorGet(HI_VOID);
int HDMI_CEC_RX_STATE_cec_rx_dest_cmd_headerGet(HI_VOID);
int HDMI_CEC_RX_STATE_cec_rx_init_cmd_headerGet(HI_VOID);
int HDMI_CEC_AUTO_DISC_cec_auto_ping_startGet(HI_VOID);
int HDMI_CEC_AUTO_DISC_cec_auto_ping_clearGet(HI_VOID);
int HDMI_CEC_AUTO_DISC_cec_auto_ping_doneGet(HI_VOID);
int HDMI_CEC_CDC_CTRL_cec_cdc_arb_cntGet(HI_VOID);
int HDMI_CEC_CDC_CTRL_cec_cdc_arb_enGet(HI_VOID);
int HDMI_CEC_CDC_CTRL_cec_cdc_opcodeGet(HI_VOID);
int HDMI_CEC_AUTODISC_MAP_cec_auto_ping_mapGet(HI_VOID);
int HDMI_CEC_TX_INIT_cec_tx_init_idGet(HI_VOID);
int HDMI_CEC_TX_DEST_cec_tx_dest_idGet(HI_VOID);
int HDMI_CEC_TX_DEST_cec_reg_sd_poll_internGet(HI_VOID);
int HDMI_CEC_CAPTUTR_ID_cec_capture_idGet(HI_VOID);
int HDMI_TX_CEC_CMD0_tx_cec_commandGet(HI_VOID);
int HDMI_TX_CEC_CMD0_tx_cec_operand0Get(HI_VOID);
int HDMI_TX_CEC_CMD0_tx_cec_operand1Get(HI_VOID);
int HDMI_TX_CEC_CMD0_tx_cec_operand2Get(HI_VOID);
int HDMI_TX_CEC_CMD1_tx_cec_operand3Get(HI_VOID);
int HDMI_TX_CEC_CMD1_tx_cec_operand4Get(HI_VOID);
int HDMI_TX_CEC_CMD1_tx_cec_operand5Get(HI_VOID);
int HDMI_TX_CEC_CMD1_tx_cec_operand6Get(HI_VOID);
int HDMI_TX_CEC_CMD2_tx_cec_operand7Get(HI_VOID);
int HDMI_TX_CEC_CMD2_tx_cec_operand8Get(HI_VOID);
int HDMI_TX_CEC_CMD2_tx_cec_operand9Get(HI_VOID);
int HDMI_TX_CEC_CMD2_tx_cec_operand10Get(HI_VOID);
int HDMI_TX_CEC_CMD3_tx_cec_operand11Get(HI_VOID);
int HDMI_TX_CEC_CMD3_tx_cec_operand12Get(HI_VOID);
int HDMI_TX_CEC_CMD3_tx_cec_operand13Get(HI_VOID);
int HDMI_TX_CEC_CMD3_tx_cec_operand14Get(HI_VOID);
int HDMI_RX_CEC_CMD0_rx_cec_commandGet(HI_VOID);
int HDMI_RX_CEC_CMD0_rx_cec_operand0Get(HI_VOID);
int HDMI_RX_CEC_CMD0_rx_cec_operand1Get(HI_VOID);
int HDMI_RX_CEC_CMD0_rx_cec_operand2Get(HI_VOID);
int HDMI_RX_CEC_CMD1_rx_cec_operand3Get(HI_VOID);
int HDMI_RX_CEC_CMD1_rx_cec_operand4Get(HI_VOID);
int HDMI_RX_CEC_CMD1_rx_cec_operand5Get(HI_VOID);
int HDMI_RX_CEC_CMD1_rx_cec_operand6Get(HI_VOID);
int HDMI_RX_CEC_CMD2_rx_cec_operand7Get(HI_VOID);
int HDMI_RX_CEC_CMD2_rx_cec_operand8Get(HI_VOID);
int HDMI_RX_CEC_CMD2_rx_cec_operand9Get(HI_VOID);
int HDMI_RX_CEC_CMD2_rx_cec_operand10Get(HI_VOID);
int HDMI_RX_CEC_CMD3_rx_cec_operand11Get(HI_VOID);
int HDMI_RX_CEC_CMD3_rx_cec_operand12Get(HI_VOID);
int HDMI_RX_CEC_CMD3_rx_cec_operand13Get(HI_VOID);
int HDMI_RX_CEC_CMD3_rx_cec_operand14Get(HI_VOID);
int HDMI_CEC_OP_ABORT_0_cec_op_abort_0Get(HI_VOID);
int HDMI_CEC_OP_ABORT_1_cec_op_abort_1Get(HI_VOID);
int HDMI_CEC_OP_ABORT_2_cec_op_abort_2Get(HI_VOID);
int HDMI_CEC_OP_ABORT_3_cec_op_abort_3Get(HI_VOID);
int HDMI_CEC_OP_ABORT_4_cec_op_abort_4Get(HI_VOID);
int HDMI_CEC_OP_ABORT_5_cec_op_abort_5Get(HI_VOID);
int HDMI_CEC_OP_ABORT_6_cec_op_abort_6Get(HI_VOID);
int HDMI_CEC_OP_ABORT_7_cec_op_abort_7Get(HI_VOID);
#endif // __TX_CEC_REG_C_UNION_DEFINE_H__
