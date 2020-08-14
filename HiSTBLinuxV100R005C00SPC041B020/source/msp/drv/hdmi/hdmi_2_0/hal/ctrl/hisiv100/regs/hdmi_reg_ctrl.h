// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  tx_ctrl_reg_c_union_define.h
// Project line  :  Platform And Key Technologies Development
// Department    :  Device Chipset Source Algorithm and Chipset Platform Development Dept
// Author        :  w00226427
// Version       :  1.0
// Date          :  2015/1/1
// Description   :  HDMIMHL_TX_HISI
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00226427 2015/10/08 19:43:09 Create file
// ******************************************************************************

#ifndef __TX_CTRL_REG_C_UNION_DEFINE_H__
#define __TX_CTRL_REG_C_UNION_DEFINE_H__

/* Define the union U_TX_PWD_RST_CTRL */
/* Define the union U_TX_PWD_RST_CTRL,ATTR:,ADDR:10,INIT:00000400,MASK:7ff,CMMNT:TX PWDÄ£¿éÈí¸´Î»¿ØÖÆ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_pwd_srst_req    : 1  ; /* [0] */
        unsigned int    tx_sys_srst_req    : 1  ; /* [1] */
        unsigned int    tx_vid_srst_req    : 1  ; /* [2] */
        unsigned int    tx_hdmi_srst_req   : 1  ; /* [3] */
        unsigned int    tx_hdcp1x_srst_req : 1  ; /* [4] */
        unsigned int    tx_phy_srst_req    : 1  ; /* [5] */
        unsigned int    tx_aud_srst_req    : 1  ; /* [6] */
        unsigned int    tx_acr_srst_req    : 1  ; /* [7] */
        unsigned int    tx_afifo_srst_req  : 1  ; /* [8] */
        unsigned int    tx_hdcp2x_srst_req : 1  ; /* [9] */
        unsigned int    tx_mcu_srst_req    : 1  ; /* [10] */
        unsigned int    rsv_0              : 21  ; /* [31:11] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_PWD_RST_CTRL;

/* Define the union U_SCDC_FSM_CTRL */
/* Define the union U_SCDC_FSM_CTRL,ATTR:,ADDR:14,INIT:00000020,MASK:ff,CMMNT:SCDC Ä£¿é¿ØÖÆ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scdc_ddcm_abort      : 1  ; /* [0] */
        unsigned int    scdc_access_en       : 1  ; /* [1] */
        unsigned int    scdc_auto_reply      : 1  ; /* [2] */
        unsigned int    scdc_auto_poll       : 1  ; /* [3] */
        unsigned int    scdc_auto_reply_stop : 1  ; /* [4] */
        unsigned int    scdc_poll_sel        : 1  ; /* [5] */
        unsigned int    scdc_hdcp_det_en     : 1  ; /* [6] */
        unsigned int    scdc_stall_req       : 1  ; /* [7] */
        unsigned int    rsv_1                : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SCDC_FSM_CTRL;

/* Define the union U_SCDC_POLL_TIMER */
/* Define the union U_SCDC_POLL_TIMER,ATTR:,ADDR:18,INIT:00249F00,MASK:3fffff,CMMNT:SCDC POLL TIMERÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scdc_poll_timer : 22  ; /* [21:0] */
        unsigned int    rsv_2           : 10  ; /* [31:22] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SCDC_POLL_TIMER;

/* Define the union U_SCDC_FSM_STATE */
/* Define the union U_SCDC_FSM_STATE,ATTR:,ADDR:1C,INIT:00000000,MASK:7ff,CMMNT:SCDC ×´Ì¬Ö¸Ê¾¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scdc_fsm_state    : 4  ; /* [3:0] */
        unsigned int    scdc_rreq_state   : 4  ; /* [7:4] */
        unsigned int    scdc_active       : 1  ; /* [8] */
        unsigned int    scdc_in_prog      : 1  ; /* [9] */
        unsigned int    scdc_rreq_in_prog : 1  ; /* [10] */
        unsigned int    rsv_3             : 21  ; /* [31:11] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SCDC_FSM_STATE;

/* Define the union U_SCDC_FLAG_BTYE */
/* Define the union U_SCDC_FLAG_BTYE,ATTR:,ADDR:20,INIT:00000000,MASK:ffff,CMMNT:SCDC UPDATE FLAGÊý¾Ý¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scdc_flag_byte0 : 8  ; /* [7:0] */
        unsigned int    scdc_flag_byte1 : 8  ; /* [15:8] */
        unsigned int    rsv_4           : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SCDC_FLAG_BTYE;

/* Define the union U_PWD_FIFO_RDATA */
/* Define the union U_PWD_FIFO_RDATA,ATTR:,ADDR:38,INIT:00000000,MASK:ff,CMMNT:DDC Master FIFO ¶ÁÊý¾Ý¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pwd_fifo_data_out : 8  ; /* [7:0] */
        unsigned int    rsv_5             : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PWD_FIFO_RDATA;

/* Define the union U_PWD_FIFO_WDATA */
/* Define the union U_PWD_FIFO_WDATA,ATTR:,ADDR:3C,INIT:00000000,MASK:ff,CMMNT:DDC Master FIFO Ð´Êý¾Ý¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pwd_fifo_data_in : 8  ; /* [7:0] */
        unsigned int    rsv_6            : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PWD_FIFO_WDATA;

/* Define the union U_PWD_DATA_CNT */
/* Define the union U_PWD_DATA_CNT,ATTR:,ADDR:40,INIT:00000000,MASK:3ff1f,CMMNT:DDC Master FIFOÊý¾Ý¼ÆÊý¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pwd_fifo_data_cnt : 5  ; /* [4:0] */
        unsigned int    rsv_7             : 3  ; /* [7:5] */
        unsigned int    pwd_data_out_cnt  : 10  ; /* [17:8] */
        unsigned int    rsv_8             : 14  ; /* [31:18] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PWD_DATA_CNT;

/* Define the union U_PWD_SLAVE_CFG */
/* Define the union U_PWD_SLAVE_CFG,ATTR:,ADDR:44,INIT:000000A0,MASK:ffffff,CMMNT:DDC ·ÃÎÊSlaveÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pwd_slave_addr   : 8  ; /* [7:0] */
        unsigned int    pwd_slave_offset : 8  ; /* [15:8] */
        unsigned int    pwd_slave_seg    : 8  ; /* [23:16] */
        unsigned int    rsv_9            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PWD_SLAVE_CFG;

/* Define the union U_PWD_MST_STATE */
/* Define the union U_PWD_MST_STATE,ATTR:,ADDR:48,INIT:00000020,MASK:ff,CMMNT:DDC Master ×´Ì¬Ö¸Ê¾¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pwd_i2c_no_ack     : 1  ; /* [0] */
        unsigned int    pwd_i2c_bus_low    : 1  ; /* [1] */
        unsigned int    pwd_i2c_in_prog    : 1  ; /* [2] */
        unsigned int    pwd_fifo_wr_in_use : 1  ; /* [3] */
        unsigned int    pwd_fifo_rd_in_use : 1  ; /* [4] */
        unsigned int    pwd_fifo_empty     : 1  ; /* [5] */
        unsigned int    pwd_fifo_half_full : 1  ; /* [6] */
        unsigned int    pwd_fifo_full      : 1  ; /* [7] */
        unsigned int    rsv_10             : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PWD_MST_STATE;

/* Define the union U_PWD_MST_CMD */
/* Define the union U_PWD_MST_CMD,ATTR:,ADDR:4C,INIT:00000000,MASK:f,CMMNT:DDC Master Ö¸ÁîÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pwd_mst_cmd : 4  ; /* [3:0] */
        unsigned int    rsv_11      : 28  ; /* [31:4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PWD_MST_CMD;

/* Define the union U_PWD_MST_CLR */
/* Define the union U_PWD_MST_CLR,ATTR:,ADDR:50,INIT:00000000,MASK:3,CMMNT:±£Áô¼Ä´æÆ÷*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pwd_clr_bus_low : 1  ; /* [0] */
        unsigned int    pwd_clr_no_ack  : 1  ; /* [1] */
        unsigned int    rsv_12          : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PWD_MST_CLR;

/* Define the union U_DDC_MST_ARB_CTRL */
/* Define the union U_DDC_MST_ARB_CTRL,ATTR:,ADDR:64,INIT:00000002,MASK:3,CMMNT:±£Áô¼Ä´æÆ÷*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cpu_ddc_force_req : 1  ; /* [0] */
        unsigned int    reg_auto_abort_en : 1  ; /* [1] */
        unsigned int    rsv_13            : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_MST_ARB_CTRL;

/* Define the union U_DDC_MST_ARB_REQ */
/* Define the union U_DDC_MST_ARB_REQ,ATTR:,ADDR:68,INIT:00000000,MASK:1,CMMNT:DDC×ÜÏß·ÃÎÊÉêÇë¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cpu_ddc_req : 1  ; /* [0] */
        unsigned int    rsv_14      : 31  ; /* [31:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_MST_ARB_REQ;

/* Define the union U_DDC_MST_ARB_ACK */
/* Define the union U_DDC_MST_ARB_ACK,ATTR:,ADDR:6C,INIT:00000000,MASK:1,CMMNT:DDC×ÜÏß·ÃÎÊÓ¦´ð¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cpu_ddc_req_ack : 1  ; /* [0] */
        unsigned int    rsv_15          : 31  ; /* [31:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_MST_ARB_ACK;

/* Define the union U_DDC_MST_ARB_STATE */
/* Define the union U_DDC_MST_ARB_STATE,ATTR:,ADDR:70,INIT:00000000,MASK:1ff,CMMNT:DDC×ÜÏß·ÃÎÊÖÙ²Ã×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddc_arb_state : 9  ; /* [8:0] */
        unsigned int    rsv_16        : 23  ; /* [31:9] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_MST_ARB_STATE;

/* Define the union U_TX_PWD_INTR_STATE */
/* Define the union U_TX_PWD_INTR_STATE,ATTR:,ADDR:100,INIT:00000000,MASK:1,CMMNT:TX PWD ÖÐ¶Ï×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_pwd_intr_state : 1  ; /* [0] */
        unsigned int    rsv_17            : 31  ; /* [31:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_PWD_INTR_STATE;

/* Define the union U_PWD_SUB_INTR_STATE */
/* Define the union U_PWD_SUB_INTR_STATE,ATTR:,ADDR:104,INIT:00000000,MASK:3f,CMMNT:TX PWD Sub Module ÖÐ¶Ï×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_sys_intr_state  : 1  ; /* [0] */
        unsigned int    vidpath_intr_state : 1  ; /* [1] */
        unsigned int    audpath_intr_state : 1  ; /* [2] */
        unsigned int    txhdmi_intr_state  : 1  ; /* [3] */
        unsigned int    txhdcp_intr_state  : 1  ; /* [4] */
        unsigned int    hdcp2x_intr_state  : 1  ; /* [5] */
        unsigned int    rsv_18             : 26  ; /* [31:6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PWD_SUB_INTR_STATE;

/* Define the union U_PWD_SUB_INTR_MASK */
/* Define the union U_PWD_SUB_INTR_MASK,ATTR:,ADDR:108,INIT:00000000,MASK:3f,CMMNT:TX PWD Sub ModuleÖÐ¶ÏÆÁ±Î¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_sys_intr_mask  : 1  ; /* [0] */
        unsigned int    vidpath_intr_mask : 1  ; /* [1] */
        unsigned int    audpath_intr_mask : 1  ; /* [2] */
        unsigned int    txhdmi_intr_mask  : 1  ; /* [3] */
        unsigned int    txhdcp_intr_mask  : 1  ; /* [4] */
        unsigned int    hdcp2x_intr_mask  : 1  ; /* [5] */
        unsigned int    rsv_19            : 26  ; /* [31:6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PWD_SUB_INTR_MASK;

/* Define the union U_TXSYS_INTR_STATE */
/* Define the union U_TXSYS_INTR_STATE,ATTR:,ADDR:10C,INIT:00000000,MASK:3f,CMMNT:TX SYSÖÐ¶Ï×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_sys_intr_state0 : 1  ; /* [0] */
        unsigned int    tx_sys_intr_state1 : 1  ; /* [1] */
        unsigned int    tx_sys_intr_state2 : 1  ; /* [2] */
        unsigned int    tx_sys_intr_state3 : 1  ; /* [3] */
        unsigned int    tx_sys_intr_state4 : 1  ; /* [4] */
        unsigned int    tx_sys_intr_state5 : 1  ; /* [5] */
        unsigned int    rsv_20             : 26  ; /* [31:6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TXSYS_INTR_STATE;

/* Define the union U_TXSYS_INTR_MASK */
/* Define the union U_TXSYS_INTR_MASK,ATTR:,ADDR:110,INIT:00000000,MASK:3f,CMMNT:TX SYSÖÐ¶ÏÆÁ±Î¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_sys_intr_mask0 : 1  ; /* [0] */
        unsigned int    tx_sys_intr_mask1 : 1  ; /* [1] */
        unsigned int    tx_sys_intr_mask2 : 1  ; /* [2] */
        unsigned int    tx_sys_intr_mask3 : 1  ; /* [3] */
        unsigned int    tx_sys_intr_mask4 : 1  ; /* [4] */
        unsigned int    tx_sys_intr_mask5 : 1  ; /* [5] */
        unsigned int    rsv_21            : 26  ; /* [31:6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TXSYS_INTR_MASK;

/* Define the union U_VIDPATH_INTR_STATE */
/* Define the union U_VIDPATH_INTR_STATE,ATTR:,ADDR:114,INIT:00000000,MASK:7f,CMMNT:Video PathÖÐ¶Ï×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vidpath_intr_state0 : 1  ; /* [0] */
        unsigned int    vidpath_intr_state1 : 1  ; /* [1] */
        unsigned int    vidpath_intr_state2 : 1  ; /* [2] */
        unsigned int    vidpath_intr_state3 : 1  ; /* [3] */
        unsigned int    vidpath_intr_state4 : 1  ; /* [4] */
        unsigned int    vidpath_intr_state5 : 1  ; /* [5] */
        unsigned int    vidpath_intr_state6 : 1  ; /* [6] */
        unsigned int    rsv_22              : 25  ; /* [31:7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VIDPATH_INTR_STATE;

/* Define the union U_VIDPATH_INTR_MASK */
/* Define the union U_VIDPATH_INTR_MASK,ATTR:,ADDR:118,INIT:00000000,MASK:7f,CMMNT:Video PathÖÐ¶ÏÆÁ±Î¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vidpath_intr_mask0 : 1  ; /* [0] */
        unsigned int    vidpath_intr_mask1 : 1  ; /* [1] */
        unsigned int    vidpath_intr_mask2 : 1  ; /* [2] */
        unsigned int    vidpath_intr_mask3 : 1  ; /* [3] */
        unsigned int    vidpath_intr_mask4 : 1  ; /* [4] */
        unsigned int    vidpath_intr_mask5 : 1  ; /* [5] */
        unsigned int    vidpath_intr_mask6 : 1  ; /* [6] */
        unsigned int    rsv_23             : 25  ; /* [31:7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VIDPATH_INTR_MASK;

/* Define the union U_AUDPATH_INTR_STATE */
/* Define the union U_AUDPATH_INTR_STATE,ATTR:,ADDR:11C,INIT:00000000,MASK:1ff,CMMNT:Audio PathÖÐ¶Ï×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    audpath_intr_state0 : 1  ; /* [0] */
        unsigned int    audpath_intr_state1 : 1  ; /* [1] */
        unsigned int    audpath_intr_state2 : 1  ; /* [2] */
        unsigned int    audpath_intr_state3 : 1  ; /* [3] */
        unsigned int    audpath_intr_state4 : 1  ; /* [4] */
        unsigned int    audpath_intr_state5 : 1  ; /* [5] */
        unsigned int    audpath_intr_state6 : 1  ; /* [6] */
        unsigned int    audpath_intr_state7 : 1  ; /* [7] */
        unsigned int    audpath_intr_state8 : 1  ; /* [8] */
        unsigned int    rsv_24              : 23  ; /* [31:9] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AUDPATH_INTR_STATE;

/* Define the union U_AUDPATH_INTR_MASK */
/* Define the union U_AUDPATH_INTR_MASK,ATTR:,ADDR:120,INIT:00000000,MASK:1ff,CMMNT:Audio PathÖÐ¶ÏÆÁ±Î¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    audpath_intr_mask0 : 1  ; /* [0] */
        unsigned int    audpath_intr_mask1 : 1  ; /* [1] */
        unsigned int    audpath_intr_mask2 : 1  ; /* [2] */
        unsigned int    audpath_intr_mask3 : 1  ; /* [3] */
        unsigned int    audpath_intr_mask4 : 1  ; /* [4] */
        unsigned int    audpath_intr_mask5 : 1  ; /* [5] */
        unsigned int    audpath_intr_mask6 : 1  ; /* [6] */
        unsigned int    audpath_intr_mask7 : 1  ; /* [7] */
        unsigned int    audpath_intr_mask8 : 1  ; /* [8] */
        unsigned int    rsv_25             : 23  ; /* [31:9] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AUDPATH_INTR_MASK;

/* Define the union U_TXHDMI_INTR_STATE */
/* Define the union U_TXHDMI_INTR_STATE,ATTR:,ADDR:124,INIT:00000000,MASK:1,CMMNT:TX HDMIÖÐ¶Ï×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    txhdmi_intr_state0 : 1  ; /* [0] */
        unsigned int    rsv_26             : 31  ; /* [31:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TXHDMI_INTR_STATE;

/* Define the union U_TXHDMI_INTR_MASK */
/* Define the union U_TXHDMI_INTR_MASK,ATTR:,ADDR:128,INIT:00000000,MASK:1,CMMNT:TX HDMIÖÐ¶ÏÆÁ±Î¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    txhdmi_intr_mask0 : 1  ; /* [0] */
        unsigned int    rsv_27            : 31  ; /* [31:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TXHDMI_INTR_MASK;

/* Define the union U_HDCP_INTR_STATE */
/* Define the union U_HDCP_INTR_STATE,ATTR:,ADDR:12C,INIT:00000000,MASK:3,CMMNT:HDCPÖÐ¶Ï×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp_intr_state0 : 1  ; /* [0] */
        unsigned int    hdcp_intr_state1 : 1  ; /* [1] */
        unsigned int    rsv_28           : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP_INTR_STATE;

/* Define the union U_HDCP_INTR_MASK */
/* Define the union U_HDCP_INTR_MASK,ATTR:,ADDR:130,INIT:00000000,MASK:3,CMMNT:HDCPÖÐ¶ÏÆÁ±Î¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp_intr_mask0 : 1  ; /* [0] */
        unsigned int    hdcp_intr_mask1 : 1  ; /* [1] */
        unsigned int    rsv_29          : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP_INTR_MASK;


//==============================================================================
/* Define the global struct */
typedef struct
{
    unsigned int                             Reserved_0[4];		      /* 0-C */
    volatile U_TX_PWD_RST_CTRL    TX_PWD_RST_CTRL    ; /* 10 */
    volatile U_SCDC_FSM_CTRL      SCDC_FSM_CTRL      ; /* 14 */
    volatile U_SCDC_POLL_TIMER    SCDC_POLL_TIMER    ; /* 18 */
    volatile U_SCDC_FSM_STATE     SCDC_FSM_STATE     ; /* 1C */
    volatile U_SCDC_FLAG_BTYE     SCDC_FLAG_BTYE     ; /* 20 */
    unsigned int                             Reserved_1[5];		      /* 0-c */
    volatile U_PWD_FIFO_RDATA     PWD_FIFO_RDATA     ; /* 38 */
    volatile U_PWD_FIFO_WDATA     PWD_FIFO_WDATA     ; /* 3C */
    volatile U_PWD_DATA_CNT       PWD_DATA_CNT       ; /* 40 */
    volatile U_PWD_SLAVE_CFG      PWD_SLAVE_CFG      ; /* 44 */
    volatile U_PWD_MST_STATE      PWD_MST_STATE      ; /* 48 */
    volatile U_PWD_MST_CMD        PWD_MST_CMD        ; /* 4C */
	volatile U_PWD_MST_CLR        PWD_MST_CLR        ; /* 50 */
    unsigned int                             Reserved_2[4];	/* 54-60 */
    volatile U_DDC_MST_ARB_CTRL   DDC_MST_ARB_CTRL   ; /* 64 */
    volatile U_DDC_MST_ARB_REQ    DDC_MST_ARB_REQ    ; /* 68 */
    volatile U_DDC_MST_ARB_ACK    DDC_MST_ARB_ACK    ; /* 6C */
    volatile U_DDC_MST_ARB_STATE  DDC_MST_ARB_STATE  ; /* 70 */
    unsigned int                             Reserved_3[35];	/* 74-FC */
    volatile U_TX_PWD_INTR_STATE  TX_PWD_INTR_STATE  ; /* 100 */
    volatile U_PWD_SUB_INTR_STATE PWD_SUB_INTR_STATE ; /* 104 */
    volatile U_PWD_SUB_INTR_MASK  PWD_SUB_INTR_MASK  ; /* 108 */
    volatile U_TXSYS_INTR_STATE   TXSYS_INTR_STATE   ; /* 10C */
    volatile U_TXSYS_INTR_MASK    TXSYS_INTR_MASK    ; /* 110 */
    volatile U_VIDPATH_INTR_STATE VIDPATH_INTR_STATE ; /* 114 */
    volatile U_VIDPATH_INTR_MASK  VIDPATH_INTR_MASK  ; /* 118 */
    volatile U_AUDPATH_INTR_STATE AUDPATH_INTR_STATE ; /* 11C */
    volatile U_AUDPATH_INTR_MASK  AUDPATH_INTR_MASK  ; /* 120 */
    volatile U_TXHDMI_INTR_STATE  TXHDMI_INTR_STATE  ; /* 124 */
    volatile U_TXHDMI_INTR_MASK   TXHDMI_INTR_MASK   ; /* 128 */
    volatile U_HDCP_INTR_STATE    HDCP_INTR_STATE    ; /* 12C */
    volatile U_HDCP_INTR_MASK     HDCP_INTR_MASK     ; /* 130 */

} S_tx_ctrl_reg_REGS_TYPE;

/* Declare the struct pointor of the module tx_ctrl_reg */
//extern volatile S_tx_ctrl_reg_REGS_TYPE *goptx_ctrl_regAllReg;
int HDMI_TX_S_tx_ctrl_reg_REGS_TYPE_Init(void);
int HDMI_TX_S_tx_ctrl_reg_REGS_TYPE_DeInit(void);

/* Declare the functions that set the member value */
int HDMI_TX_PWD_RST_CTRL_tx_afifo_srst_reqSet(unsigned int utx_afifo_srst_req);
int HDMI_TX_PWD_RST_CTRL_tx_acr_srst_reqSet(unsigned int utx_acr_srst_req);
int HDMI_TX_PWD_RST_CTRL_tx_aud_srst_reqSet(unsigned int utx_aud_srst_req);
int HDMI_PWD_TIMEOUT_CFG_pwd_timeout_cntSet(unsigned int upwd_timeout_cnt);
int HDMI_PWD_TIMEOUT_CFG_apb_pready_maskSet(unsigned int uapb_pready_mask);
int HDMI_SCDC_FSM_CTRL_scdc_ddcm_abortSet(unsigned int uscdc_ddcm_abort);
int HDMI_SCDC_FSM_CTRL_scdc_access_enSet(unsigned int uscdc_access_en);
int HDMI_SCDC_FSM_CTRL_scdc_auto_replySet(unsigned int uscdc_auto_reply);
int HDMI_SCDC_FSM_CTRL_scdc_auto_pollSet(unsigned int uscdc_auto_poll);
int HDMI_SCDC_FSM_CTRL_scdc_auto_reply_stopSet(unsigned int uscdc_auto_reply_stop);
int HDMI_SCDC_FSM_CTRL_scdc_poll_selSet(unsigned int uscdc_poll_sel);
int HDMI_SCDC_FSM_CTRL_scdc_hdcp_det_enSet(unsigned int uscdc_hdcp_det_en);
int HDMI_SCDC_FSM_CTRL_scdc_stall_reqSet(unsigned int uscdc_stall_req);
int HDMI_SCDC_POLL_TIMER_scdc_poll_timerSet(unsigned int uscdc_poll_timer);
int HDMI_SCDC_FSM_STATE_scdc_fsm_stateSet(unsigned int uscdc_fsm_state);
int HDMI_SCDC_FSM_STATE_scdc_rreq_stateSet(unsigned int uscdc_rreq_state);
int HDMI_SCDC_FLAG_BTYE_scdc_flag_byte0Set(unsigned int uscdc_flag_byte0);
int HDMI_SCDC_FLAG_BTYE_scdc_flag_byte1Set(unsigned int uscdc_flag_byte1);
int HDMI_PWD_FIFO_RDATA_pwd_fifo_data_outSet(unsigned int upwd_fifo_data_out);
int HDMI_PWD_FIFO_WDATA_pwd_fifo_data_inSet(unsigned int upwd_fifo_data_in);
int HDMI_PWD_DATA_CNT_pwd_fifo_data_cntSet(unsigned int upwd_fifo_data_cnt);
int HDMI_PWD_DATA_CNT_pwd_data_out_cntSet(unsigned int upwd_data_out_cnt);
int HDMI_PWD_SLAVE_CFG_pwd_slave_addrSet(unsigned int upwd_slave_addr);
int HDMI_PWD_SLAVE_CFG_pwd_slave_offsetSet(unsigned int upwd_slave_offset);
int HDMI_PWD_SLAVE_CFG_pwd_slave_segSet(unsigned int upwd_slave_seg);
int HDMI_PWD_MST_STATE_pwd_i2c_no_ackSet(unsigned int upwd_i2c_no_ack);
int HDMI_PWD_MST_STATE_pwd_i2c_bus_lowSet(unsigned int upwd_i2c_bus_low);
int HDMI_PWD_MST_STATE_pwd_i2c_in_progSet(unsigned int upwd_i2c_in_prog);
int HDMI_PWD_MST_STATE_pwd_fifo_wr_in_useSet(unsigned int upwd_fifo_wr_in_use);
int HDMI_PWD_MST_STATE_pwd_fifo_rd_in_useSet(unsigned int upwd_fifo_rd_in_use);
int HDMI_PWD_MST_STATE_pwd_fifo_emptySet(unsigned int upwd_fifo_empty);
int HDMI_PWD_MST_STATE_pwd_fifo_half_fullSet(unsigned int upwd_fifo_half_full);
int HDMI_PWD_MST_STATE_pwd_fifo_fullSet(unsigned int upwd_fifo_full);
int HDMI_PWD_MST_CMD_pwd_mst_cmdSet(unsigned int upwd_mst_cmd);
int HDMI_PWD_MST_CLR_pwd_clr_bus_lowSet(unsigned int upwd_clr_bus_low);
int HDMI_PWD_MST_CLR_pwd_clr_no_ackSet(unsigned int upwd_clr_no_ack);
int HDMI_DDC_MST_ARB_CTRL_reg_auto_abort_enSet(unsigned int ureg_auto_abort_en);
int HDMI_DDC_MST_ARB_CTRL_cpu_ddc_force_reqSet(unsigned int ucpu_ddc_force_req);
int HDMI_DDC_MST_ARB_REQ_cpu_ddc_reqSet(unsigned int ucpu_ddc_req);
int HDMI_DDC_MST_ARB_ACK_cpu_ddc_req_ackSet(unsigned int ucpu_ddc_req_ack);
int HDMI_DDC_MST_ARB_STATE_ddc_arb_stateSet(unsigned int uddc_arb_state);
int HDMI_TX_PWD_INTR_STATE_tx_pwd_intr_stateSet(unsigned int utx_pwd_intr_state);
int HDMI_PWD_SUB_INTR_STATE_tx_sys_intr_stateSet(unsigned int utx_sys_intr_state);
int HDMI_PWD_SUB_INTR_STATE_vidpath_intr_stateSet(unsigned int uvidpath_intr_state);
int HDMI_PWD_SUB_INTR_STATE_audpath_intr_stateSet(unsigned int uaudpath_intr_state);
int HDMI_PWD_SUB_INTR_STATE_txhdmi_intr_stateSet(unsigned int utxhdmi_intr_state);
int HDMI_PWD_SUB_INTR_STATE_txhdcp_intr_stateSet(unsigned int utxhdcp_intr_state);
int HDMI_PWD_SUB_INTR_MASK_tx_sys_intr_maskSet(unsigned int utx_sys_intr_mask);
int HDMI_PWD_SUB_INTR_MASK_vidpath_intr_maskSet(unsigned int uvidpath_intr_mask);
int HDMI_PWD_SUB_INTR_MASK_audpath_intr_maskSet(unsigned int uaudpath_intr_mask);
int HDMI_PWD_SUB_INTR_MASK_txhdmi_intr_maskSet(unsigned int utxhdmi_intr_mask);
int HDMI_PWD_SUB_INTR_MASK_txhdcp_intr_maskSet(unsigned int utxhdcp_intr_mask);
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state0Set(unsigned int utx_sys_intr_state0);
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state1Set(unsigned int utx_sys_intr_state1);
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state2Set(unsigned int utx_sys_intr_state2);
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state3Set(unsigned int utx_sys_intr_state3);
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state4Set(unsigned int utx_sys_intr_state4);
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state5Set(unsigned int utx_sys_intr_state5);
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask0Set(unsigned int utx_sys_intr_mask0);
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask1Set(unsigned int utx_sys_intr_mask1);
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask2Set(unsigned int utx_sys_intr_mask2);
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask3Set(unsigned int utx_sys_intr_mask3);
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask4Set(unsigned int utx_sys_intr_mask4);
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask5Set(unsigned int utx_sys_intr_mask5);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state0Set(unsigned int uvidpath_intr_state0);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state1Set(unsigned int uvidpath_intr_state1);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state2Set(unsigned int uvidpath_intr_state2);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state3Set(unsigned int uvidpath_intr_state3);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state4Set(unsigned int uvidpath_intr_state4);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state5Set(unsigned int uvidpath_intr_state5);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state6Set(unsigned int uvidpath_intr_state6);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask0Set(unsigned int uvidpath_intr_mask0);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask1Set(unsigned int uvidpath_intr_mask1);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask2Set(unsigned int uvidpath_intr_mask2);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask3Set(unsigned int uvidpath_intr_mask3);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask4Set(unsigned int uvidpath_intr_mask4);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask5Set(unsigned int uvidpath_intr_mask5);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask6Set(unsigned int uvidpath_intr_mask6);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state0Set(unsigned int uaudpath_intr_state0);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state1Set(unsigned int uaudpath_intr_state1);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state2Set(unsigned int uaudpath_intr_state2);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state3Set(unsigned int uaudpath_intr_state3);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state4Set(unsigned int uaudpath_intr_state4);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state5Set(unsigned int uaudpath_intr_state5);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state6Set(unsigned int uaudpath_intr_state6);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state7Set(unsigned int uaudpath_intr_state7);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state8Set(unsigned int uaudpath_intr_state8);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask0Set(unsigned int uaudpath_intr_mask0);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask1Set(unsigned int uaudpath_intr_mask1);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask2Set(unsigned int uaudpath_intr_mask2);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask3Set(unsigned int uaudpath_intr_mask3);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask4Set(unsigned int uaudpath_intr_mask4);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask5Set(unsigned int uaudpath_intr_mask5);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask6Set(unsigned int uaudpath_intr_mask6);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask7Set(unsigned int uaudpath_intr_mask7);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask8Set(unsigned int uaudpath_intr_mask8);
int HDMI_TXHDMI_INTR_STATE_txhdmi_intr_state0Set(unsigned int utxhdmi_intr_state0);
int HDMI_TXHDMI_INTR_MASK_txhdmi_intr_mask0Set(unsigned int utxhdmi_intr_mask0);
int HDMI_HDCP_INTR_STATE_hdcp_intr_state0Set(unsigned int uhdcp_intr_state0);
int HDMI_HDCP_INTR_STATE_hdcp_intr_state1Set(unsigned int uhdcp_intr_state1);
int HDMI_HDCP_INTR_MASK_hdcp_intr_mask0Set(unsigned int uhdcp_intr_mask0);
int HDMI_TX_PWD_RST_CTRL_tx_mcu_srst_reqSet(unsigned int utx_mcu_srst_req);
/* Declare the functions that get the member value */
int HDMI_TX_PWD_RST_CTRL_tx_pwd_srst_reqGet(void);
int HDMI_TX_PWD_RST_CTRL_tx_sys_srst_reqGet(void);
int HDMI_TX_PWD_RST_CTRL_tx_vid_srst_reqGet(void);
int HDMI_TX_PWD_RST_CTRL_tx_hdmi_srst_reqGet(void);
int HDMI_TX_PWD_RST_CTRL_tx_hdcp1x_srst_reqGet(void);
int HDMI_TX_PWD_RST_CTRL_tx_phy_srst_reqGet(void);
int HDMI_TX_PWD_RST_CTRL_tx_aud_srst_reqGet(void);
int HDMI_TX_PWD_RST_CTRL_tx_acr_srst_reqGet(void);
int HDMI_TX_PWD_RST_CTRL_tx_afifo_srst_reqGet(void);
int HDMI_TX_PWD_RST_CTRL_tx_hdcp2x_srst_reqGet(void);
int HDMI_TX_PWD_RST_CTRL_tx_mcu_srst_reqGet(void);

int HDMI_SCDC_FSM_CTRL_scdc_ddcm_abortGet(void);
int HDMI_SCDC_FSM_CTRL_scdc_access_enGet(void);
int HDMI_SCDC_FSM_CTRL_scdc_auto_replyGet(void);
int HDMI_SCDC_FSM_CTRL_scdc_auto_pollGet(void);
int HDMI_SCDC_FSM_CTRL_scdc_auto_reply_stopGet(void);
int HDMI_SCDC_FSM_CTRL_scdc_poll_selGet(void);
int HDMI_SCDC_FSM_CTRL_scdc_hdcp_det_enGet(void);
int HDMI_SCDC_FSM_CTRL_scdc_stall_reqGet(void);
int HDMI_SCDC_POLL_TIMER_scdc_poll_timerGet(void);
int HDMI_SCDC_FSM_STATE_scdc_fsm_stateGet(void);
int HDMI_SCDC_FSM_STATE_scdc_rreq_stateGet(void);
int HDMI_SCDC_FSM_STATE_scdc_activeGet(void);
int HDMI_SCDC_FSM_STATE_scdc_in_progGet(void);
int HDMI_SCDC_FSM_STATE_scdc_rreq_in_progGet(void);
int HDMI_SCDC_FLAG_BTYE_scdc_flag_byte0Get(void);
int HDMI_SCDC_FLAG_BTYE_scdc_flag_byte1Get(void);
int HDMI_PWD_FIFO_RDATA_pwd_fifo_data_outGet(void);
int HDMI_PWD_FIFO_WDATA_pwd_fifo_data_inGet(void);
int HDMI_PWD_DATA_CNT_pwd_fifo_data_cntGet(void);
int HDMI_PWD_DATA_CNT_pwd_data_out_cntGet(void);
int HDMI_PWD_SLAVE_CFG_pwd_slave_addrGet(void);
int HDMI_PWD_SLAVE_CFG_pwd_slave_offsetGet(void);
int HDMI_PWD_SLAVE_CFG_pwd_slave_segGet(void);
int HDMI_PWD_MST_STATE_pwd_i2c_no_ackGet(void);
int HDMI_PWD_MST_STATE_pwd_i2c_bus_lowGet(void);
int HDMI_PWD_MST_STATE_pwd_i2c_in_progGet(void);
int HDMI_PWD_MST_STATE_pwd_fifo_wr_in_useGet(void);
int HDMI_PWD_MST_STATE_pwd_fifo_rd_in_useGet(void);
int HDMI_PWD_MST_STATE_pwd_fifo_emptyGet(void);
int HDMI_PWD_MST_STATE_pwd_fifo_half_fullGet(void);
int HDMI_PWD_MST_STATE_pwd_fifo_fullGet(void);
int HDMI_PWD_MST_CMD_pwd_mst_cmdGet(void);
int HDMI_PWD_MST_CLR_pwd_clr_bus_lowGet(void);
int HDMI_PWD_MST_CLR_pwd_clr_no_ackGet(void);
int HDMI_DDC_MST_ARB_CTRL_cpu_ddc_force_reqGet(void);
int HDMI_DDC_MST_ARB_CTRL_reg_auto_abort_enGet(void);
int HDMI_DDC_MST_ARB_REQ_cpu_ddc_reqGet(void);
int HDMI_DDC_MST_ARB_ACK_cpu_ddc_req_ackGet(void);
int HDMI_DDC_MST_ARB_STATE_ddc_arb_stateGet(void);
int HDMI_TX_PWD_INTR_STATE_tx_pwd_intr_stateGet(void);
int HDMI_PWD_SUB_INTR_STATE_tx_sys_intr_stateGet(void);
int HDMI_PWD_SUB_INTR_STATE_vidpath_intr_stateGet(void);
int HDMI_PWD_SUB_INTR_STATE_audpath_intr_stateGet(void);
int HDMI_PWD_SUB_INTR_STATE_txhdmi_intr_stateGet(void);
int HDMI_PWD_SUB_INTR_STATE_txhdcp_intr_stateGet(void);
int HDMI_PWD_SUB_INTR_STATE_hdcp2x_intr_stateGet(void);
int HDMI_PWD_SUB_INTR_MASK_tx_sys_intr_maskGet(void);
int HDMI_PWD_SUB_INTR_MASK_vidpath_intr_maskGet(void);
int HDMI_PWD_SUB_INTR_MASK_audpath_intr_maskGet(void);
int HDMI_PWD_SUB_INTR_MASK_txhdmi_intr_maskGet(void);
int HDMI_PWD_SUB_INTR_MASK_txhdcp_intr_maskGet(void);
int HDMI_PWD_SUB_INTR_MASK_hdcp2x_intr_maskGet(void);
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state0Get(void);
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state1Get(void);
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state2Get(void);
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state3Get(void);
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state4Get(void);
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state5Get(void);
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask0Get(void);
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask1Get(void);
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask2Get(void);
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask3Get(void);
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask4Get(void);
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask5Get(void);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state0Get(void);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state1Get(void);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state2Get(void);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state3Get(void);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state4Get(void);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state5Get(void);
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state6Get(void);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask0Get(void);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask1Get(void);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask2Get(void);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask3Get(void);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask4Get(void);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask5Get(void);
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask6Get(void);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state0Get(void);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state1Get(void);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state2Get(void);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state3Get(void);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state4Get(void);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state5Get(void);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state6Get(void);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state7Get(void);
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state8Get(void);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask0Get(void);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask1Get(void);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask2Get(void);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask3Get(void);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask4Get(void);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask5Get(void);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask6Get(void);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask7Get(void);
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask8Get(void);
int HDMI_TXHDMI_INTR_STATE_txhdmi_intr_state0Get(void);
int HDMI_TXHDMI_INTR_MASK_txhdmi_intr_mask0Get(void);
int HDMI_HDCP_INTR_STATE_hdcp_intr_state0Get(void);
int HDMI_HDCP_INTR_STATE_hdcp_intr_state1Get(void);
int HDMI_HDCP_INTR_MASK_hdcp_intr_mask0Get(void);
int HDMI_HDCP_INTR_MASK_hdcp_intr_mask1Get(void);

#endif // __TX_CTRL_REG_C_UNION_DEFINE_H__
