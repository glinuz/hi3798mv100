// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  tx_hdcp_reg_c_union_define.h
// Project line  :  Platform And Key Technologies Development
// Department    :  Device Chipset Source Algorithm and Chipset Platform Development Dept
// Author        :  w00226427
// Version       :  1.0
// Date          :  2015/1/1
// Description   :  HDMIMHL_TX_HISI
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00226427 2015/10/08 19:23:58 Create file
// ******************************************************************************

#ifndef __TX_HDCP_REG_C_UNION_DEFINE_H__
#define __TX_HDCP_REG_C_UNION_DEFINE_H__

/* Define the union U_HDCP1X_MEM_CTRL */
/* Define the union U_HDCP1X_MEM_CTRL,ATTR:,ADDR:2000,INIT:00000000,MASK:7ff,CMMNT:HDCP1X KEY ¿ØÖÆ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_key_load      : 1  ; /* [0] */
        unsigned int    hdcp1x_key_cmd       : 5  ; /* [5:1] */
        unsigned int    hdcp1x_key_cmd_valid : 1  ; /* [6] */
        unsigned int    hdcp1x_cmd_done      : 1  ; /* [7] */
        unsigned int    hdcp1x_bist0_err     : 1  ; /* [8] */
        unsigned int    hdcp1x_bist1_err     : 1  ; /* [9] */
        unsigned int    hdcp1x_bist2_err     : 1  ; /* [10] */
        unsigned int    rsv_0                : 21  ; /* [31:11] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_MEM_CTRL;

/* Define the union U_HDCP1X_KEY_AKSV0 */
/* Define the union U_HDCP1X_KEY_AKSV0,ATTR:,ADDR:2004,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X AKSV¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_key_aksv_byte0 : 8  ; /* [7:0] */
        unsigned int    hdcp1x_key_aksv_byte1 : 8  ; /* [15:8] */
        unsigned int    hdcp1x_key_aksv_byte2 : 8  ; /* [23:16] */
        unsigned int    hdcp1x_key_aksv_byte3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_KEY_AKSV0;

/* Define the union U_HDCP1X_KEY_AKSV1 */
/* Define the union U_HDCP1X_KEY_AKSV1,ATTR:,ADDR:2008,INIT:00000000,MASK:ff,CMMNT:HDCP1X AKSV¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_key_aksv_byte4 : 8  ; /* [7:0] */
        unsigned int    rsv_1                 : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_KEY_AKSV1;

/* Define the union U_HDCP_MUTE_CTRL */
/* Define the union U_HDCP_MUTE_CTRL,ATTR:,ADDR:200C,INIT:00000000,MASK:3f,CMMNT:HDCPÒôÊÓÆµMUTE¿ØÖÆ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_vmute_ctrl : 1  ; /* [0] */
        unsigned int    hdcp1x_amute_ctrl : 2  ; /* [2:1] */
        unsigned int    hdcp2x_vmute_ctrl : 1  ; /* [3] */
        unsigned int    hdcp2x_amute_ctrl : 2  ; /* [5:4] */
        unsigned int    rsv_2             : 26  ; /* [31:6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP_MUTE_CTRL;

/* Define the union U_HDCP_FUN_SEL */
/* Define the union U_HDCP_FUN_SEL,ATTR:,ADDR:2014,INIT:00000001,MASK:1,CMMNT:HDCP¹¦ÄÜÑ¡ÔñÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp_fun_sel : 1  ; /* [0] */
        unsigned int    rsv_3        : 31  ; /* [31:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP_FUN_SEL;

/* Define the union U_HDCP1X_SEC_CFG */
/* Define the union U_HDCP1X_SEC_CFG,ATTR:,ADDR:201C,INIT:00000000,MASK:3,CMMNT:HDCP1X °²È«CPUÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_sec_enc_en : 1  ; /* [0] */
        unsigned int    hdcp1x_sec_rpt_on : 1  ; /* [1] */
        unsigned int    rsv_4             : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_SEC_CFG;

/* Define the union U_HDCP1X_ENG_CTRL */
/* Define the union U_HDCP1X_ENG_CTRL,ATTR:,ADDR:2020,INIT:00000FE1,MASK:fff,CMMNT:HDCP1X ENG¿ØÖÆ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_enc_en   : 1  ; /* [0] */
        unsigned int    hdcp1x_an_stop  : 1  ; /* [1] */
        unsigned int    hdcp1x_rpt_on   : 1  ; /* [2] */
        unsigned int    hdcp1x_ri_trush : 1  ; /* [3] */
        unsigned int    hdcp1x_ri_hold  : 1  ; /* [4] */
        unsigned int    hdcp1x_ri_comp  : 7  ; /* [11:5] */
        unsigned int    rsv_5           : 20  ; /* [31:12] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_ENG_CTRL;

/* Define the union U_HDCP1X_ENG_STATUS */
/* Define the union U_HDCP1X_ENG_STATUS,ATTR:,ADDR:2024,INIT:00000000,MASK:1ff,CMMNT:HDCP1X ENG×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_i_cnt    : 7  ; /* [6:0] */
        unsigned int    hdcp1x_bksv_err : 1  ; /* [7] */
        unsigned int    hdcp1x_ri_rdy   : 1  ; /* [8] */
        unsigned int    rsv_6           : 23  ; /* [31:9] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_ENG_STATUS;

/* Define the union U_HDCP1X_ENG_BKSV0 */
/* Define the union U_HDCP1X_ENG_BKSV0,ATTR:,ADDR:2028,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X BKSV¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_key_bksv_byte0 : 8  ; /* [7:0] */
        unsigned int    hdcp1x_key_bksv_byte1 : 8  ; /* [15:8] */
        unsigned int    hdcp1x_key_bksv_byte2 : 8  ; /* [23:16] */
        unsigned int    hdcp1x_key_bksv_byte3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_ENG_BKSV0;

/* Define the union U_HDCP1X_ENG_BKSV1 */
/* Define the union U_HDCP1X_ENG_BKSV1,ATTR:,ADDR:202C,INIT:00000000,MASK:ff,CMMNT:HDCP1X BKSV¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_key_bksv_byte4 : 8  ; /* [7:0] */
        unsigned int    rsv_7                 : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_ENG_BKSV1;

/* Define the union U_HDCP1X_ENG_GEN_AN0 */
/* Define the union U_HDCP1X_ENG_GEN_AN0,ATTR:,ADDR:2030,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X ENG GEN AN ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_eng_gen_an0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_ENG_GEN_AN0;

/* Define the union U_HDCP1X_ENG_GEN_AN1 */
/* Define the union U_HDCP1X_ENG_GEN_AN1,ATTR:,ADDR:2034,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X ENG GEN AN ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_eng_gen_an1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_ENG_GEN_AN1;

/* Define the union U_HDCP1X_ENG_FUN_AN0 */
/* Define the union U_HDCP1X_ENG_FUN_AN0,ATTR:,ADDR:2038,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X ENG FUN AN ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_eng_fun_an0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_ENG_FUN_AN0;

/* Define the union U_HDCP1X_ENG_FUN_AN1 */
/* Define the union U_HDCP1X_ENG_FUN_AN1,ATTR:,ADDR:203C,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X ENG FUN AN ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_eng_fun_an1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_ENG_FUN_AN1;

/* Define the union U_HDCP1X_ENG_RI */
/* Define the union U_HDCP1X_ENG_RI,ATTR:,ADDR:2040,INIT:00000000,MASK:ffff,CMMNT:HDCP1X ENG RI ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_eng_ri0 : 8  ; /* [7:0] */
        unsigned int    hdcp1x_eng_ri1 : 8  ; /* [15:8] */
        unsigned int    rsv_8          : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_ENG_RI;

/* Define the union U_HDCP1X_RPT_BSTATUS */
/* Define the union U_HDCP1X_RPT_BSTATUS,ATTR:,ADDR:2058,INIT:00000000,MASK:ffff,CMMNT:HDCP1X Repeater BSTATUS ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_bstatus : 16  ; /* [15:0] */
        unsigned int    rsv_9          : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_RPT_BSTATUS;

/* Define the union U_HDCP1X_RPT_KLIST */
/* Define the union U_HDCP1X_RPT_KLIST,ATTR:,ADDR:205C,INIT:00000000,MASK:ff,CMMNT:HDCP1X Repeater KSV LIST ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_rpt_klist : 8  ; /* [7:0] */
        unsigned int    rsv_10           : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_RPT_KLIST;

/* Define the union U_HDCP1X_SHA_CTRL */
/* Define the union U_HDCP1X_SHA_CTRL,ATTR:,ADDR:2060,INIT:00000000,MASK:7,CMMNT:HDCP1X SHA1 ¿ØÖÆ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_sha_start : 1  ; /* [0] */
        unsigned int    hdcp1x_sha_no_ds : 1  ; /* [1] */
        unsigned int    hdcp1x_sha_ready : 1  ; /* [2] */
        unsigned int    rsv_11           : 29  ; /* [31:3] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_SHA_CTRL;

/* Define the union U_HDCP1X_SHA_V0 */
/* Define the union U_HDCP1X_SHA_V0,ATTR:,ADDR:2064,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X SHA1 V0 ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_rpt_vh0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_SHA_V0;

/* Define the union U_HDCP1X_SHA_V1 */
/* Define the union U_HDCP1X_SHA_V1,ATTR:,ADDR:2068,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X SHA1 V1 ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_rpt_vh1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_SHA_V1;

/* Define the union U_HDCP1X_SHA_V2 */
/* Define the union U_HDCP1X_SHA_V2,ATTR:,ADDR:206C,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X SHA1 V2 ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_rpt_vh2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_SHA_V2;

/* Define the union U_HDCP1X_SHA_V3 */
/* Define the union U_HDCP1X_SHA_V3,ATTR:,ADDR:2070,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X SHA1 V3 ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_rpt_vh3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_SHA_V3;

/* Define the union U_HDCP1X_SHA_V4 */
/* Define the union U_HDCP1X_SHA_V4,ATTR:,ADDR:2074,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X SHA1 V4¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_rpt_vh4 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_SHA_V4;

/* Define the union U_HDCP1X_CHK_CTRL */
/* Define the union U_HDCP1X_CHK_CTRL,ATTR:,ADDR:2080,INIT:00000012,MASK:1f,CMMNT:HDCP1X RI ×Ô¶¯¼ì²é¿ØÖÆ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_auto_check_en    : 1  ; /* [0] */
        unsigned int    hdcp1x_ddc_req_priority : 1  ; /* [1] */
        unsigned int    hdcp1x_check_start_sel  : 2  ; /* [3:2] */
        unsigned int    hdcp1x_check_errcnt_en  : 1  ; /* [4] */
        unsigned int    rsv_12                  : 27  ; /* [31:5] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_CHK_CTRL;

/* Define the union U_HDCP1X_CHK_START */
/* Define the union U_HDCP1X_CHK_START,ATTR:,ADDR:2084,INIT:107E007F,MASK:ffffffff,CMMNT:HDCP1X RI ×Ô¶¯¼ì²éÆðÊ¼ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_ri_chk1_fcnt : 8  ; /* [7:0] */
        unsigned int    hdcp1x_ri_chk2_fcnt : 8  ; /* [15:8] */
        unsigned int    hdcp1x_ddc_req_fcnt : 8  ; /* [23:16] */
        unsigned int    hdcp1x_linecnt_thre : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_CHK_START;

/* Define the union U_HDCP1X_CHK_ERR */
/* Define the union U_HDCP1X_CHK_ERR,ATTR:,ADDR:2088,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X RI×Ô¶¯¼ì²é´íÎóÍ³¼Æ×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_127frm_err_cnt : 8  ; /* [7:0] */
        unsigned int    hdcp1x_000frm_err_cnt : 8  ; /* [15:8] */
        unsigned int    hdcp1x_notchg_err_cnt : 8  ; /* [23:16] */
        unsigned int    hdcp1x_nodone_err_cnt : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_CHK_ERR;

/* Define the union U_HDCP1X_CHK_STATE */
/* Define the union U_HDCP1X_CHK_STATE,ATTR:,ADDR:208C,INIT:00000000,MASK:f,CMMNT:HDCP1X RI×Ô¶¯¼ì²éÄÚ²¿×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_fsm_state : 4  ; /* [3:0] */
        unsigned int    rsv_13           : 28  ; /* [31:4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_CHK_STATE;

/* Define the union U_HDCP2X_ENC_CTRL */
/* Define the union U_HDCP2X_ENC_CTRL,ATTR:,ADDR:20A0,INIT:00000001,MASK:1,CMMNT:HDCP2X ENC¼ÓÃÜ¿ØÖÆ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_enc_en : 1  ; /* [0] */
        unsigned int    rsv_14        : 31  ; /* [31:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_ENC_CTRL;

/* Define the union U_HDCP2X_DBG_CTRL */
/* Define the union U_HDCP2X_DBG_CTRL,ATTR:,ADDR:20A4,INIT:00000000,MASK:1,CMMNT:HDCP2X UART DEBUGÑ¡Ôñ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_uart_sel : 1  ; /* [0] */
        unsigned int    rsv_15          : 31  ; /* [31:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_DBG_CTRL;

/* Define the union U_HDCP2X_ENC_STATE */
/* Define the union U_HDCP2X_ENC_STATE,ATTR:,ADDR:20B4,INIT:00000000,MASK:1,CMMNT:HDCP2X ENC¼ÓÃÜ×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_enc_state : 1  ; /* [0] */
        unsigned int    rsv_16           : 31  ; /* [31:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_ENC_STATE;

/* Define the union U_HDCP1X_RPT_V0 */
/* Define the union U_HDCP1X_RPT_V0,ATTR:,ADDR:20E0,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X SHA1 V0 ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_rpt_calc_vh0 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_RPT_V0;

/* Define the union U_HDCP1X_RPT_V1 */
/* Define the union U_HDCP1X_RPT_V1,ATTR:,ADDR:20E4,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X SHA1 V1 ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_rpt_calc_vh1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_RPT_V1;

/* Define the union U_HDCP1X_RPT_V2 */
/* Define the union U_HDCP1X_RPT_V2,ATTR:,ADDR:20E8,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X SHA1 V2 ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_rpt_calc_vh2 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_RPT_V2;

/* Define the union U_HDCP1X_RPT_V3 */
/* Define the union U_HDCP1X_RPT_V3,ATTR:,ADDR:20EC,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X SHA1 V3 ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_rpt_calc_vh3 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_RPT_V3;

/* Define the union U_HDCP1X_RPT_V4 */
/* Define the union U_HDCP1X_RPT_V4,ATTR:,ADDR:20F0,INIT:00000000,MASK:ffffffff,CMMNT:HDCP1X SHA1 V4¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_rpt_calc_vh4 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_RPT_V4;

/* Define the union U_HDCP1X_SHA_CHECK */
/* Define the union U_HDCP1X_SHA_CHECK,ATTR:,ADDR:20F4,INIT:00000000,MASK:1,CMMNT:HDCP1X SHA±È½Ï¿ØÖÆ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_sha_check : 1  ; /* [0] */
        unsigned int    rsv_17           : 31  ; /* [31:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_SHA_CHECK;

/* Define the union U_HDCP1X_SHA_RESULT */
/* Define the union U_HDCP1X_SHA_RESULT,ATTR:,ADDR:20F8,INIT:00000000,MASK:3,CMMNT:HDCP1X SHA±È½Ï½á¹û¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_sha_ok      : 1  ; /* [0] */
        unsigned int    hdcp1x_sha_invalid : 1  ; /* [1] */
        unsigned int    rsv_18             : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP1X_SHA_RESULT;


//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_HDCP1X_MEM_CTRL    HDCP1X_MEM_CTRL    ; /* 2000 */
    volatile U_HDCP1X_KEY_AKSV0   HDCP1X_KEY_AKSV0   ; /* 2004 */
    volatile U_HDCP1X_KEY_AKSV1   HDCP1X_KEY_AKSV1   ; /* 2008 */
	volatile U_HDCP_MUTE_CTRL     HDCP_MUTE_CTRL     ; /* 200C */
	unsigned int				  Reserved_0[1];	   /* 2010 */
    volatile U_HDCP_FUN_SEL       HDCP_FUN_SEL       ; /* 2014 */
	unsigned int				  Reserved_1[1];	   /* 2018-2018 */
    volatile U_HDCP1X_SEC_CFG     HDCP1X_SEC_CFG     ; /* 201C */	
    volatile U_HDCP1X_ENG_CTRL    HDCP1X_ENG_CTRL    ; /* 2020 */
    volatile U_HDCP1X_ENG_STATUS  HDCP1X_ENG_STATUS  ; /* 2024 */
    volatile U_HDCP1X_ENG_BKSV0   HDCP1X_ENG_BKSV0   ; /* 2028 */
    volatile U_HDCP1X_ENG_BKSV1   HDCP1X_ENG_BKSV1   ; /* 202C */
    volatile U_HDCP1X_ENG_GEN_AN0 HDCP1X_ENG_GEN_AN0 ; /* 2030 */
    volatile U_HDCP1X_ENG_GEN_AN1 HDCP1X_ENG_GEN_AN1 ; /* 2034 */
    volatile U_HDCP1X_ENG_FUN_AN0 HDCP1X_ENG_FUN_AN0 ; /* 2038 */
    volatile U_HDCP1X_ENG_FUN_AN1 HDCP1X_ENG_FUN_AN1 ; /* 203C */
    volatile U_HDCP1X_ENG_RI      HDCP1X_ENG_RI      ; /* 2040 */
	unsigned int				  Reserved_2[5];	   /* 2044-2054 */
    volatile U_HDCP1X_RPT_BSTATUS HDCP1X_RPT_BSTATUS ; /* 2058 */
    volatile U_HDCP1X_RPT_KLIST   HDCP1X_RPT_KLIST   ; /* 205C */
    volatile U_HDCP1X_SHA_CTRL    HDCP1X_SHA_CTRL    ; /* 2060 */
    volatile U_HDCP1X_SHA_V0      HDCP1X_SHA_V0      ; /* 2064 */
    volatile U_HDCP1X_SHA_V1      HDCP1X_SHA_V1      ; /* 2068 */
    volatile U_HDCP1X_SHA_V2      HDCP1X_SHA_V2      ; /* 206C */
    volatile U_HDCP1X_SHA_V3      HDCP1X_SHA_V3      ; /* 2070 */
    volatile U_HDCP1X_SHA_V4      HDCP1X_SHA_V4      ; /* 2074 */
	unsigned int				  Reserved_3[2];	   /* 2078-207c */
    volatile U_HDCP1X_CHK_CTRL    HDCP1X_CHK_CTRL    ; /* 2080 */
    volatile U_HDCP1X_CHK_START   HDCP1X_CHK_START   ; /* 2084 */
    volatile U_HDCP1X_CHK_ERR     HDCP1X_CHK_ERR     ; /* 2088 */
    volatile U_HDCP1X_CHK_STATE   HDCP1X_CHK_STATE   ; /* 208C */
	unsigned int				  Reserved_4[4];	   /* 2090-209c */	
    volatile U_HDCP2X_ENC_CTRL    HDCP2X_ENC_CTRL    ; /* 20A0 */
    volatile U_HDCP2X_DBG_CTRL    HDCP2X_DBG_CTRL    ; /* 20A4 */
	unsigned int				  Reserved_5[3];	   /* 20A8-20B0 */
    volatile U_HDCP2X_ENC_STATE   HDCP2X_ENC_STATE   ; /* 20B4 */
	unsigned int				  Reserved_6[10];	   /* 20B8-20DC */	
    volatile U_HDCP1X_RPT_V0      HDCP1X_RPT_V0      ; /* 20E0 */
    volatile U_HDCP1X_RPT_V1      HDCP1X_RPT_V1      ; /* 20E4 */
    volatile U_HDCP1X_RPT_V2      HDCP1X_RPT_V2      ; /* 20E8 */
    volatile U_HDCP1X_RPT_V3      HDCP1X_RPT_V3      ; /* 20EC */
    volatile U_HDCP1X_RPT_V4      HDCP1X_RPT_V4      ; /* 20F0 */
    volatile U_HDCP1X_SHA_CHECK   HDCP1X_SHA_CHECK   ; /* 20F4 */
    volatile U_HDCP1X_SHA_RESULT  HDCP1X_SHA_RESULT  ; /* 20F8 */

} S_tx_hdcp_reg_REGS_TYPE;

/* Declare the struct pointor of the module tx_hdcp_reg */
//extern volatile S_tx_hdcp_reg_REGS_TYPE *goptx_hdcp_regAllReg;
int HDMI_TX_S_tx_hdcp_reg_REGS_TYPE_Init(HI_VOID);
int HDMI_TX_S_tx_hdcp_reg_REGS_TYPE_DeInit(HI_VOID);

/* Declare the functions that set the member value */
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_loadSet(unsigned int uhdcp1x_key_load);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_loadGet(HI_VOID);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmdSet(unsigned int uhdcp1x_key_cmd);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmdGet(HI_VOID);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmd_validSet(unsigned int uhdcp1x_key_cmd_valid);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmd_validGet(HI_VOID);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_cmd_doneSet(unsigned int uhdcp1x_cmd_done);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_cmd_doneGet(HI_VOID);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist0_errSet(unsigned int uhdcp1x_bist0_err);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist0_errGet(HI_VOID);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist1_errSet(unsigned int uhdcp1x_bist1_err);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist1_errGet(HI_VOID);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist2_errSet(unsigned int uhdcp1x_bist2_err);
int HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist2_errGet(HI_VOID);
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte0Set(unsigned int uhdcp1x_key_aksv_byte0);
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte0Get(HI_VOID);
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte1Set(unsigned int uhdcp1x_key_aksv_byte1);
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte1Get(HI_VOID);
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte2Set(unsigned int uhdcp1x_key_aksv_byte2);
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte2Get(HI_VOID);
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte3Set(unsigned int uhdcp1x_key_aksv_byte3);
int HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte3Get(HI_VOID);
int HDMI_HDCP1X_KEY_AKSV1_hdcp1x_key_aksv_byte4Set(unsigned int uhdcp1x_key_aksv_byte4);
int HDMI_HDCP1X_KEY_AKSV1_hdcp1x_key_aksv_byte4Get(HI_VOID);
int HDMI_HDCP_MUTE_CTRL_hdcp1x_vmute_ctrlSet(unsigned int uhdcp1x_vmute_ctrl);
int HDMI_HDCP_MUTE_CTRL_hdcp1x_vmute_ctrlGet(HI_VOID);
int HDMI_HDCP_MUTE_CTRL_hdcp1x_amute_ctrlSet(unsigned int uhdcp1x_amute_ctrl);
int HDMI_HDCP_MUTE_CTRL_hdcp1x_amute_ctrlGet(HI_VOID);
int HDMI_HDCP_MUTE_CTRL_hdcp2x_vmute_ctrlSet(unsigned int uhdcp2x_vmute_ctrl);
int HDMI_HDCP_MUTE_CTRL_hdcp2x_vmute_ctrlGet(HI_VOID);
int HDMI_HDCP_MUTE_CTRL_hdcp2x_amute_ctrlSet(unsigned int uhdcp2x_amute_ctrl);
int HDMI_HDCP_MUTE_CTRL_hdcp2x_amute_ctrlGet(HI_VOID);
int HDMI_HDCP_FUN_SEL_hdcp_fun_selSet(unsigned int uhdcp_fun_sel);
int HDMI_HDCP_FUN_SEL_hdcp_fun_selGet(HI_VOID);
int HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_enc_enSet(unsigned int uhdcp1x_sec_enc_en);
int HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_enc_enGet(HI_VOID);
int HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_rpt_onSet(unsigned int uhdcp1x_sec_rpt_on);
int HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_rpt_onGet(HI_VOID);
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_enc_enSet(unsigned int uhdcp1x_enc_en);
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_enc_enGet(HI_VOID);
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_an_stopSet(unsigned int uhdcp1x_an_stop);
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_an_stopGet(HI_VOID);
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_rpt_onSet(unsigned int uhdcp1x_rpt_on);
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_rpt_onGet(HI_VOID);
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_trushSet(unsigned int uhdcp1x_ri_trush);
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_trushGet(HI_VOID);
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_holdSet(unsigned int uhdcp1x_ri_hold);
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_holdGet(HI_VOID);
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_compSet(unsigned int uhdcp1x_ri_comp);
int HDMI_HDCP1X_ENG_CTRL_hdcp1x_ri_compGet(HI_VOID);
int HDMI_HDCP1X_ENG_STATUS_hdcp1x_i_cntSet(unsigned int uhdcp1x_i_cnt);
int HDMI_HDCP1X_ENG_STATUS_hdcp1x_i_cntGet(HI_VOID);
int HDMI_HDCP1X_ENG_STATUS_hdcp1x_bksv_errSet(unsigned int uhdcp1x_bksv_err);
int HDMI_HDCP1X_ENG_STATUS_hdcp1x_bksv_errGet(HI_VOID);
int HDMI_HDCP1X_ENG_STATUS_hdcp1x_ri_rdySet(unsigned int uhdcp1x_ri_rdy);
int HDMI_HDCP1X_ENG_STATUS_hdcp1x_ri_rdyGet(HI_VOID);
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte0Set(unsigned int uhdcp1x_key_bksv_byte0);
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte0Get(HI_VOID);
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte1Set(unsigned int uhdcp1x_key_bksv_byte1);
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte1Get(HI_VOID);
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte2Set(unsigned int uhdcp1x_key_bksv_byte2);
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte2Get(HI_VOID);
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte3Set(unsigned int uhdcp1x_key_bksv_byte3);
int HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte3Get(HI_VOID);
int HDMI_HDCP1X_ENG_BKSV1_hdcp1x_key_bksv_byte4Set(unsigned int uhdcp1x_key_bksv_byte4);
int HDMI_HDCP1X_ENG_BKSV1_hdcp1x_key_bksv_byte4Get(HI_VOID);
int HDMI_HDCP1X_ENG_GEN_AN0_hdcp1x_eng_gen_an0Set(unsigned int uhdcp1x_eng_gen_an0);
int HDMI_HDCP1X_ENG_GEN_AN0_hdcp1x_eng_gen_an0Get(HI_VOID);
int HDMI_HDCP1X_ENG_GEN_AN1_hdcp1x_eng_gen_an1Set(unsigned int uhdcp1x_eng_gen_an1);
int HDMI_HDCP1X_ENG_GEN_AN1_hdcp1x_eng_gen_an1Get(HI_VOID);
int HDMI_HDCP1X_ENG_FUN_AN0_hdcp1x_eng_fun_an0Set(unsigned int uhdcp1x_eng_fun_an0);
int HDMI_HDCP1X_ENG_FUN_AN0_hdcp1x_eng_fun_an0Get(HI_VOID);
int HDMI_HDCP1X_ENG_FUN_AN1_hdcp1x_eng_fun_an1Set(unsigned int uhdcp1x_eng_fun_an1);
int HDMI_HDCP1X_ENG_FUN_AN1_hdcp1x_eng_fun_an1Get(HI_VOID);
int HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri0Set(unsigned int uhdcp1x_eng_ri0);
int HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri0Get(HI_VOID);
int HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri1Set(unsigned int uhdcp1x_eng_ri1);
int HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri1Get(HI_VOID);
int HDMI_HDCP1X_RPT_BSTATUS_hdcp1x_bstatusSet(unsigned int uhdcp1x_bstatus);
int HDMI_HDCP1X_RPT_BSTATUS_hdcp1x_bstatusGet(HI_VOID);
int HDMI_HDCP1X_RPT_KLIST_hdcp1x_rpt_klistSet(unsigned int uhdcp1x_rpt_klist);
int HDMI_HDCP1X_RPT_KLIST_hdcp1x_rpt_klistGet(HI_VOID);
int HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_startSet(unsigned int uhdcp1x_sha_start);
int HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_startGet(HI_VOID);
int HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_no_dsSet(unsigned int uhdcp1x_sha_no_ds);
int HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_no_dsGet(HI_VOID);
int HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_readySet(unsigned int uhdcp1x_sha_ready);
int HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_readyGet(HI_VOID);
int HDMI_HDCP1X_SHA_V0_hdcp1x_rpt_vh0Set(unsigned int uhdcp1x_rpt_vh0);
int HDMI_HDCP1X_SHA_V0_hdcp1x_rpt_vh0Get(HI_VOID);
int HDMI_HDCP1X_SHA_V1_hdcp1x_rpt_vh1Set(unsigned int uhdcp1x_rpt_vh1);
int HDMI_HDCP1X_SHA_V1_hdcp1x_rpt_vh1Get(HI_VOID);
int HDMI_HDCP1X_SHA_V2_hdcp1x_rpt_vh2Set(unsigned int uhdcp1x_rpt_vh2);
int HDMI_HDCP1X_SHA_V2_hdcp1x_rpt_vh2Get(HI_VOID);
int HDMI_HDCP1X_SHA_V3_hdcp1x_rpt_vh3Set(unsigned int uhdcp1x_rpt_vh3);
int HDMI_HDCP1X_SHA_V3_hdcp1x_rpt_vh3Get(HI_VOID);
int HDMI_HDCP1X_SHA_V4_hdcp1x_rpt_vh4Set(unsigned int uhdcp1x_rpt_vh4);
int HDMI_HDCP1X_SHA_V4_hdcp1x_rpt_vh4Get(HI_VOID);
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_auto_check_enSet(unsigned int uhdcp1x_auto_check_en);
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_auto_check_enGet(HI_VOID);
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_ddc_req_prioritySet(unsigned int uhdcp1x_ddc_req_priority);
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_ddc_req_priorityGet(HI_VOID);
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_check_start_selSet(unsigned int uhdcp1x_check_start_sel);
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_check_start_selGet(HI_VOID);
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_check_errcnt_enSet(unsigned int uhdcp1x_check_errcnt_en);
int HDMI_HDCP1X_CHK_CTRL_hdcp1x_check_errcnt_enGet(HI_VOID);
int HDMI_HDCP1X_CHK_START_hdcp1x_ri_chk1_fcntSet(unsigned int uhdcp1x_ri_chk1_fcnt);
int HDMI_HDCP1X_CHK_START_hdcp1x_ri_chk1_fcntGet(HI_VOID);
int HDMI_HDCP1X_CHK_START_hdcp1x_ri_chk2_fcntSet(unsigned int uhdcp1x_ri_chk2_fcnt);
int HDMI_HDCP1X_CHK_START_hdcp1x_ri_chk2_fcntGet(HI_VOID);
int HDMI_HDCP1X_CHK_START_hdcp1x_ddc_req_fcntSet(unsigned int uhdcp1x_ddc_req_fcnt);
int HDMI_HDCP1X_CHK_START_hdcp1x_ddc_req_fcntGet(HI_VOID);
int HDMI_HDCP1X_CHK_START_hdcp1x_linecnt_threSet(unsigned int uhdcp1x_linecnt_thre);
int HDMI_HDCP1X_CHK_START_hdcp1x_linecnt_threGet(HI_VOID);
int HDMI_HDCP1X_CHK_ERR_hdcp1x_127frm_err_cntSet(unsigned int uhdcp1x_127frm_err_cnt);
int HDMI_HDCP1X_CHK_ERR_hdcp1x_127frm_err_cntGet(HI_VOID);
int HDMI_HDCP1X_CHK_ERR_hdcp1x_000frm_err_cntSet(unsigned int uhdcp1x_000frm_err_cnt);
int HDMI_HDCP1X_CHK_ERR_hdcp1x_000frm_err_cntGet(HI_VOID);
int HDMI_HDCP1X_CHK_ERR_hdcp1x_notchg_err_cntSet(unsigned int uhdcp1x_notchg_err_cnt);
int HDMI_HDCP1X_CHK_ERR_hdcp1x_notchg_err_cntGet(HI_VOID);
int HDMI_HDCP1X_CHK_ERR_hdcp1x_nodone_err_cntSet(unsigned int uhdcp1x_nodone_err_cnt);
int HDMI_HDCP1X_CHK_ERR_hdcp1x_nodone_err_cntGet(HI_VOID);
int HDMI_HDCP1X_CHK_STATE_hdcp1x_fsm_stateSet(unsigned int uhdcp1x_fsm_state);
int HDMI_HDCP1X_CHK_STATE_hdcp1x_fsm_stateGet(HI_VOID);
int HDMI_HDCP2X_ENC_CTRL_hdcp2x_enc_enSet(unsigned int uhdcp2x_enc_en);
int HDMI_HDCP2X_ENC_CTRL_hdcp2x_enc_enGet(HI_VOID);
int HDMI_HDCP2X_DBG_CTRL_hdcp2x_uart_selSet(unsigned int uhdcp2x_uart_sel);
int HDMI_HDCP2X_DBG_CTRL_hdcp2x_uart_selGet(HI_VOID);
int HDMI_HDCP2X_ENC_STATE_hdcp2x_enc_stateSet(unsigned int uhdcp2x_enc_state);
int HDMI_HDCP2X_ENC_STATE_hdcp2x_enc_stateGet(HI_VOID);
int HDMI_HDCP1X_RPT_V0_hdcp1x_rpt_calc_vh0Set(unsigned int uhdcp1x_rpt_calc_vh0);
int HDMI_HDCP1X_RPT_V0_hdcp1x_rpt_calc_vh0Get(HI_VOID);
int HDMI_HDCP1X_RPT_V1_hdcp1x_rpt_calc_vh1Set(unsigned int uhdcp1x_rpt_calc_vh1);
int HDMI_HDCP1X_RPT_V1_hdcp1x_rpt_calc_vh1Get(HI_VOID);
int HDMI_HDCP1X_RPT_V2_hdcp1x_rpt_calc_vh2Set(unsigned int uhdcp1x_rpt_calc_vh2);
int HDMI_HDCP1X_RPT_V2_hdcp1x_rpt_calc_vh2Get(HI_VOID);
int HDMI_HDCP1X_RPT_V3_hdcp1x_rpt_calc_vh3Set(unsigned int uhdcp1x_rpt_calc_vh3);
int HDMI_HDCP1X_RPT_V3_hdcp1x_rpt_calc_vh3Get(HI_VOID);
int HDMI_HDCP1X_RPT_V4_hdcp1x_rpt_calc_vh4Set(unsigned int uhdcp1x_rpt_calc_vh4);
int HDMI_HDCP1X_RPT_V4_hdcp1x_rpt_calc_vh4Get(HI_VOID);
int HDMI_HDCP1X_SHA_CHECK_hdcp1x_sha_checkSet(unsigned int uhdcp1x_sha_check);
int HDMI_HDCP1X_SHA_CHECK_hdcp1x_sha_checkGet(HI_VOID);
int HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_okSet(unsigned int uhdcp1x_sha_ok);
int HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_okGet(HI_VOID);
int HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_invalidSet(unsigned int uhdcp1x_sha_invalid);
int HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_invalidGet(HI_VOID);

#endif // __TX_HDCP_REG_C_UNION_DEFINE_H__
