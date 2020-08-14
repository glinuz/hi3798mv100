// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  mcu_cpu_reg_c_union_define.h
// Project line  :  Platform And Key Technologies Development
// Department    :  Device Chipset Source Algorithm and Chipset Platform Development Dept
// Author        :  w00226427
// Version       :  1.0
// Date          :  2015/1/1
// Description   :  HDMIMHL_TX_HISI
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00226427 2015/10/14 19:23:17 Create file
// ******************************************************************************

#ifndef __MCU_CPU_REG_C_UNION_DEFINE_H__
#define __MCU_CPU_REG_C_UNION_DEFINE_H__

/* Define the union U_HDCP2X_HW_VER_B0 */
/* Define the union U_HDCP2X_HW_VER_B0,ATTR:,ADDR:1C000,INIT:00,MASK:ff,CMMNT:HDCP2X¬ﬂº≠∞Ê±æ–≈œ¢ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_hw_ver_b0 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_HW_VER_B0;

/* Define the union U_HDCP2X_HW_VER_B1 */
/* Define the union U_HDCP2X_HW_VER_B1,ATTR:,ADDR:1C004,INIT:00,MASK:ff,CMMNT:HDCP2X¬ﬂº≠∞Ê±æ–≈œ¢ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_hw_ver_b1 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_HW_VER_B1;

/* Define the union U_HDCP2X_HW_VER_B2 */
/* Define the union U_HDCP2X_HW_VER_B2,ATTR:,ADDR:1C008,INIT:00,MASK:ff,CMMNT:HDCP2X¬ﬂº≠∞Ê±æ–≈œ¢ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_hw_ver_b2 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_HW_VER_B2;

/* Define the union U_HDCP2X_HW_VER_B3 */
/* Define the union U_HDCP2X_HW_VER_B3,ATTR:,ADDR:1C00C,INIT:00,MASK:ff,CMMNT:HDCP2X¬ﬂº≠∞Ê±æ–≈œ¢ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_hw_ver_b3 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_HW_VER_B3;

/* Define the union U_HDCP2X_SW_VER_B0 */
/* Define the union U_HDCP2X_SW_VER_B0,ATTR:,ADDR:1C010,INIT:00,MASK:ff,CMMNT:HDCP2X»Ìº˛∞Ê±æ–≈œ¢ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sw_ver_b0 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SW_VER_B0;

/* Define the union U_HDCP2X_SW_VER_B1 */
/* Define the union U_HDCP2X_SW_VER_B1,ATTR:,ADDR:1C014,INIT:00,MASK:ff,CMMNT:HDCP2X»Ìº˛∞Ê±æ–≈œ¢ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sw_ver_b1 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SW_VER_B1;

/* Define the union U_HDCP2X_SW_VER_B2 */
/* Define the union U_HDCP2X_SW_VER_B2,ATTR:,ADDR:1C018,INIT:00,MASK:ff,CMMNT:HDCP2X»Ìº˛∞Ê±æ–≈œ¢ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sw_ver_b2 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SW_VER_B2;

/* Define the union U_HDCP2X_SW_VER_B3 */
/* Define the union U_HDCP2X_SW_VER_B3,ATTR:,ADDR:1C01C,INIT:00,MASK:ff,CMMNT:HDCP2X»Ìº˛∞Ê±æ–≈œ¢ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sw_ver_b3 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SW_VER_B3;

/* Define the union U_HDCP2X_AUTH_CTRL */
/* Define the union U_HDCP2X_AUTH_CTRL,ATTR:,ADDR:1C054,INIT:00,MASK:3,CMMNT:HDCP2Xƒ£øÈ»œ÷§∆Ù∂Øøÿ÷∆ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_auth_start : 1  ; /* [0] */
        unsigned int    mcu_auth_stop  : 1  ; /* [1] */
        unsigned int    rsv_0          : 6  ; /* [7:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_AUTH_CTRL;

/* Define the union U_HDCP2X_SEC_INTR_B0 */
/* Define the union U_HDCP2X_SEC_INTR_B0,ATTR:,ADDR:1C080,INIT:00,MASK:ff,CMMNT:HDCP2X SEC÷–∂œ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_intr0 : 1  ; /* [0] */
        unsigned int    hdcp2x_sec_intr1 : 1  ; /* [1] */
        unsigned int    hdcp2x_sec_intr2 : 1  ; /* [2] */
        unsigned int    hdcp2x_sec_intr3 : 1  ; /* [3] */
        unsigned int    hdcp2x_sec_intr4 : 1  ; /* [4] */
        unsigned int    hdcp2x_sec_intr5 : 1  ; /* [5] */
        unsigned int    hdcp2x_sec_intr6 : 1  ; /* [6] */
        unsigned int    hdcp2x_sec_intr7 : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_INTR_B0;

/* Define the union U_HDCP2X_SEC_INTR_B1 */
/* Define the union U_HDCP2X_SEC_INTR_B1,ATTR:,ADDR:1C084,INIT:00,MASK:ff,CMMNT:HDCP2X SEC÷–∂œ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_intr8  : 1  ; /* [0] */
        unsigned int    hdcp2x_sec_intr9  : 1  ; /* [1] */
        unsigned int    hdcp2x_sec_intr10 : 1  ; /* [2] */
        unsigned int    hdcp2x_sec_intr11 : 1  ; /* [3] */
        unsigned int    hdcp2x_sec_intr12 : 1  ; /* [4] */
        unsigned int    hdcp2x_sec_intr13 : 1  ; /* [5] */
        unsigned int    hdcp2x_sec_intr14 : 1  ; /* [6] */
        unsigned int    hdcp2x_sec_intr15 : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_INTR_B1;

/* Define the union U_HDCP2X_SEC_MASK_B0 */
/* Define the union U_HDCP2X_SEC_MASK_B0,ATTR:,ADDR:1C088,INIT:00,MASK:ff,CMMNT:HDCP2X SEC÷–∂œ∆¡±Œ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_mask0 : 1  ; /* [0] */
        unsigned int    hdcp2x_sec_mask1 : 1  ; /* [1] */
        unsigned int    hdcp2x_sec_mask2 : 1  ; /* [2] */
        unsigned int    hdcp2x_sec_mask3 : 1  ; /* [3] */
        unsigned int    hdcp2x_sec_mask4 : 1  ; /* [4] */
        unsigned int    hdcp2x_sec_mask5 : 1  ; /* [5] */
        unsigned int    hdcp2x_sec_mask6 : 1  ; /* [6] */
        unsigned int    hdcp2x_sec_mask7 : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_MASK_B0;

/* Define the union U_HDCP2X_SEC_MASK_B1 */
/* Define the union U_HDCP2X_SEC_MASK_B1,ATTR:,ADDR:1C08C,INIT:00,MASK:ff,CMMNT:HDCP2X SEC÷–∂œ∆¡±Œ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_mask8  : 1  ; /* [0] */
        unsigned int    hdcp2x_sec_mask9  : 1  ; /* [1] */
        unsigned int    hdcp2x_sec_mask10 : 1  ; /* [2] */
        unsigned int    hdcp2x_sec_mask11 : 1  ; /* [3] */
        unsigned int    hdcp2x_sec_mask12 : 1  ; /* [4] */
        unsigned int    hdcp2x_sec_mask13 : 1  ; /* [5] */
        unsigned int    hdcp2x_sec_mask14 : 1  ; /* [6] */
        unsigned int    hdcp2x_sec_mask15 : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_MASK_B1;

/* Define the union U_HDCP2X_MCU_INTR_B0 */
/* Define the union U_HDCP2X_MCU_INTR_B0,ATTR:,ADDR:1C0A4,INIT:00,MASK:ff,CMMNT:HDCP2X MCU÷–∂œ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_mcu_intr0 : 1  ; /* [0] */
        unsigned int    hdcp2x_mcu_intr1 : 1  ; /* [1] */
        unsigned int    hdcp2x_mcu_intr2 : 1  ; /* [2] */
        unsigned int    hdcp2x_mcu_intr3 : 1  ; /* [3] */
        unsigned int    hdcp2x_mcu_intr4 : 1  ; /* [4] */
        unsigned int    hdcp2x_mcu_intr5 : 1  ; /* [5] */
        unsigned int    hdcp2x_mcu_intr6 : 1  ; /* [6] */
        unsigned int    hdcp2x_mcu_intr7 : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_MCU_INTR_B0;

/* Define the union U_HDCP2X_MCU_INTR_B1 */
/* Define the union U_HDCP2X_MCU_INTR_B1,ATTR:,ADDR:1C0A8,INIT:00,MASK:ff,CMMNT:HDCP2X MCU÷–∂œ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_mcu_intr8  : 1  ; /* [0] */
        unsigned int    hdcp2x_mcu_intr9  : 1  ; /* [1] */
        unsigned int    hdcp2x_mcu_intr10 : 1  ; /* [2] */
        unsigned int    hdcp2x_mcu_intr11 : 1  ; /* [3] */
        unsigned int    hdcp2x_mcu_intr12 : 1  ; /* [4] */
        unsigned int    hdcp2x_mcu_intr13 : 1  ; /* [5] */
        unsigned int    hdcp2x_mcu_intr14 : 1  ; /* [6] */
        unsigned int    hdcp2x_mcu_intr15 : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_MCU_INTR_B1;

/* Define the union U_HDCP2X_MCU_INTR_B2 */
/* Define the union U_HDCP2X_MCU_INTR_B2,ATTR:,ADDR:1C0AC,INIT:00,MASK:ff,CMMNT:HDCP2X MCU÷–∂œ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_mcu_intr16 : 1  ; /* [0] */
        unsigned int    hdcp2x_mcu_intr17 : 1  ; /* [1] */
        unsigned int    hdcp2x_mcu_intr18 : 1  ; /* [2] */
        unsigned int    hdcp2x_mcu_intr19 : 1  ; /* [3] */
        unsigned int    hdcp2x_mcu_intr20 : 1  ; /* [4] */
        unsigned int    hdcp2x_mcu_intr21 : 1  ; /* [5] */
        unsigned int    hdcp2x_mcu_intr22 : 1  ; /* [6] */
        unsigned int    hdcp2x_mcu_intr23 : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_MCU_INTR_B2;

/* Define the union U_HDCP2X_MCU_INTR_B3 */
/* Define the union U_HDCP2X_MCU_INTR_B3,ATTR:,ADDR:1C0B0,INIT:00,MASK:ff,CMMNT:HDCP2X MCU÷–∂œ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_mcu_intr24 : 1  ; /* [0] */
        unsigned int    hdcp2x_mcu_intr25 : 1  ; /* [1] */
        unsigned int    hdcp2x_mcu_intr26 : 1  ; /* [2] */
        unsigned int    hdcp2x_mcu_intr27 : 1  ; /* [3] */
        unsigned int    hdcp2x_mcu_intr28 : 1  ; /* [4] */
        unsigned int    hdcp2x_mcu_intr29 : 1  ; /* [5] */
        unsigned int    hdcp2x_mcu_intr30 : 1  ; /* [6] */
        unsigned int    hdcp2x_mcu_intr31 : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_MCU_INTR_B3;

/* Define the union U_HDCP2X_MCU_MASK_B0 */
/* Define the union U_HDCP2X_MCU_MASK_B0,ATTR:,ADDR:1C0B4,INIT:00,MASK:ff,CMMNT:HDCP2X MCU÷–∂œ∆¡±Œ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_mcu_mask0 : 1  ; /* [0] */
        unsigned int    hdcp2x_mcu_mask1 : 1  ; /* [1] */
        unsigned int    hdcp2x_mcu_mask2 : 1  ; /* [2] */
        unsigned int    hdcp2x_mcu_mask3 : 1  ; /* [3] */
        unsigned int    hdcp2x_mcu_mask4 : 1  ; /* [4] */
        unsigned int    hdcp2x_mcu_mask5 : 1  ; /* [5] */
        unsigned int    hdcp2x_mcu_mask6 : 1  ; /* [6] */
        unsigned int    hdcp2x_mcu_mask7 : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_MCU_MASK_B0;

/* Define the union U_HDCP2X_MCU_MASK_B1 */
/* Define the union U_HDCP2X_MCU_MASK_B1,ATTR:,ADDR:1C0B8,INIT:00,MASK:ff,CMMNT:HDCP2X MCU÷–∂œ∆¡±Œ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_mcu_mask8  : 1  ; /* [0] */
        unsigned int    hdcp2x_mcu_mask9  : 1  ; /* [1] */
        unsigned int    hdcp2x_mcu_mask10 : 1  ; /* [2] */
        unsigned int    hdcp2x_mcu_mask11 : 1  ; /* [3] */
        unsigned int    hdcp2x_mcu_mask12 : 1  ; /* [4] */
        unsigned int    hdcp2x_mcu_mask13 : 1  ; /* [5] */
        unsigned int    hdcp2x_mcu_mask14 : 1  ; /* [6] */
        unsigned int    hdcp2x_mcu_mask15 : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_MCU_MASK_B1;

/* Define the union U_HDCP2X_MCU_MASK_B2 */
/* Define the union U_HDCP2X_MCU_MASK_B2,ATTR:,ADDR:1C0BC,INIT:00,MASK:ff,CMMNT:HDCP2X MCU÷–∂œ∆¡±Œ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_mcu_mask16 : 1  ; /* [0] */
        unsigned int    hdcp2x_mcu_mask17 : 1  ; /* [1] */
        unsigned int    hdcp2x_mcu_mask18 : 1  ; /* [2] */
        unsigned int    hdcp2x_mcu_mask19 : 1  ; /* [3] */
        unsigned int    hdcp2x_mcu_mask20 : 1  ; /* [4] */
        unsigned int    hdcp2x_mcu_mask21 : 1  ; /* [5] */
        unsigned int    hdcp2x_mcu_mask22 : 1  ; /* [6] */
        unsigned int    hdcp2x_mcu_mask23 : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_MCU_MASK_B2;

/* Define the union U_HDCP2X_MCU_MASK_B3 */
/* Define the union U_HDCP2X_MCU_MASK_B3,ATTR:,ADDR:1C0C0,INIT:00,MASK:ff,CMMNT:HDCP2X MCU÷–∂œ∆¡±Œ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_mcu_mask24 : 1  ; /* [0] */
        unsigned int    hdcp2x_mcu_mask25 : 1  ; /* [1] */
        unsigned int    hdcp2x_mcu_mask26 : 1  ; /* [2] */
        unsigned int    hdcp2x_mcu_mask27 : 1  ; /* [3] */
        unsigned int    hdcp2x_mcu_mask28 : 1  ; /* [4] */
        unsigned int    hdcp2x_mcu_mask29 : 1  ; /* [5] */
        unsigned int    hdcp2x_mcu_mask30 : 1  ; /* [6] */
        unsigned int    hdcp2x_mcu_mask31 : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_MCU_MASK_B3;

/* Define the union U_HDCP2X_MCU_STATE_B0 */
/* Define the union U_HDCP2X_MCU_STATE_B0,ATTR:,ADDR:1C0CC,INIT:00,MASK:ff,CMMNT:HDCP2X MCU◊¥Ã¨ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_mcu_state_b0 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_MCU_STATE_B0;

/* Define the union U_HDCP2X_MCU_STATE_B1 */
/* Define the union U_HDCP2X_MCU_STATE_B1,ATTR:,ADDR:1C0D0,INIT:00,MASK:ff,CMMNT:HDCP2X MCU◊¥Ã¨ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_mcu_state_b1 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_MCU_STATE_B1;

/* Define the union U_HDCP2X_MCU_STATE_B2 */
/* Define the union U_HDCP2X_MCU_STATE_B2,ATTR:,ADDR:1C0D4,INIT:00,MASK:ff,CMMNT:HDCP2X MCU◊¥Ã¨ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_mcu_state_b2 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_MCU_STATE_B2;

/* Define the union U_HDCP2X_MCU_STATE_B3 */
/* Define the union U_HDCP2X_MCU_STATE_B3,ATTR:,ADDR:1C0D8,INIT:00,MASK:ff,CMMNT:HDCP2X MCU◊¥Ã¨ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_mcu_state_b3 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_MCU_STATE_B3;

/* Define the union U_HDCP2X_RX_AUTH_STATE */
/* Define the union U_HDCP2X_RX_AUTH_STATE,ATTR:,ADDR:1C0E8,INIT:00,MASK:ff,CMMNT:±£¡Ùºƒ¥Ê∆˜*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_ake_init_done  : 1  ; /* [0] */
        unsigned int    hdcp2x_send_cert_done : 1  ; /* [1] */
        unsigned int    hdcp2x_km_done        : 1  ; /* [2] */
        unsigned int    hdcp2x_send_h_done    : 1  ; /* [3] */
        unsigned int    hdcp2x_pairing_done   : 1  ; /* [4] */
        unsigned int    hdcp2x_lc_init_done   : 1  ; /* [5] */
        unsigned int    hdcp2x_send_l_done    : 1  ; /* [6] */
        unsigned int    hdcp2x_send_sks_done  : 1  ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RX_AUTH_STATE;

/* Define the union U_HDCP2X_RPT_AUTH_STATE */
/* Define the union U_HDCP2X_RPT_AUTH_STATE,ATTR:,ADDR:1C0EC,INIT:00,MASK:f,CMMNT:±£¡Ùºƒ¥Ê∆˜*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_id_list_done  : 1  ; /* [0] */
        unsigned int    hdcp2x_send_ack_done : 1  ; /* [1] */
        unsigned int    hdcp2x_strm_id_done  : 1  ; /* [2] */
        unsigned int    hdcp2x_strm_rdy_done : 1  ; /* [3] */
        unsigned int    rsv_1                : 4  ; /* [7:4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_AUTH_STATE;

/* Define the union U_HDCP2X_AUTH_STATE0 */
/* Define the union U_HDCP2X_AUTH_STATE0,ATTR:,ADDR:1C0F0,INIT:00,MASK:ff,CMMNT:HDCP2X»œ÷§◊¥Ã¨ºƒ¥Ê∆˜£∫HDCP2X_AUTH_STATE0°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_auth_state0 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_AUTH_STATE0;

/* Define the union U_HDCP2X_AUTH_STATE1 */
/* Define the union U_HDCP2X_AUTH_STATE1,ATTR:,ADDR:1C0F4,INIT:00,MASK:ff,CMMNT:HDCP2X»œ÷§◊¥Ã¨ºƒ¥Ê∆˜£∫HDCP2X_AUTH_STATE1°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_auth_state1 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_AUTH_STATE1;

/* Define the union U_HDCP2X_AUTH_STATE2 */
/* Define the union U_HDCP2X_AUTH_STATE2,ATTR:,ADDR:1C0F8,INIT:00,MASK:ff,CMMNT:HDCP2X»œ÷§◊¥Ã¨ºƒ¥Ê∆˜£∫HDCP2X_AUTH_STATE2°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_auth_state2 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_AUTH_STATE2;

/* Define the union U_HDCP2X_AUTH_STATE3 */
/* Define the union U_HDCP2X_AUTH_STATE3,ATTR:,ADDR:1C0FC,INIT:00,MASK:ff,CMMNT:HDCP2X»œ÷§◊¥Ã¨ºƒ¥Ê∆˜£∫HDCP2X_AUTH_STATE3°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_auth_state3 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_AUTH_STATE3;

/* Define the union U_HDCP2X_CERT_INFO */
/* Define the union U_HDCP2X_CERT_INFO,ATTR:,ADDR:1C114,INIT:00,MASK:1,CMMNT:HDCP2X RX÷§ È–≈œ¢ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_cert_rpt : 1  ; /* [0] */
        unsigned int    rsv_2           : 7  ; /* [7:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_CERT_INFO;

/* Define the union U_HDCP2X_CERT_VER */
/* Define the union U_HDCP2X_CERT_VER,ATTR:,ADDR:1C118,INIT:00,MASK:ff,CMMNT:HDCP2X RX÷§ È–≈œ¢ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_cert_ver : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_CERT_VER;

/* Define the union U_HDCP2X_RPT_INFO */
/* Define the union U_HDCP2X_RPT_INFO,ATTR:,ADDR:1C11C,INIT:00,MASK:f,CMMNT:HDCP2X REPEATER–≈œ¢ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp1x_dev_on      : 1  ; /* [0] */
        unsigned int    hdcp20_rpt_on      : 1  ; /* [1] */
        unsigned int    hdcp2x_rpt_cas_exc : 1  ; /* [2] */
        unsigned int    hdcp2x_rpt_dev_exc : 1  ; /* [3] */
        unsigned int    rsv_3              : 4  ; /* [7:4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_INFO;

/* Define the union U_HDCP2X_RPT_DEV */
/* Define the union U_HDCP2X_RPT_DEV,ATTR:,ADDR:1C120,INIT:00,MASK:ff,CMMNT:HDCP2X REPEATER–≈œ¢ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_rpt_dev_cnt : 5  ; /* [4:0] */
        unsigned int    hdcp2x_rpt_dep_cnt : 3  ; /* [7:5] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_DEV;

/* Define the union U_HDCP2X_RPT_V0 */
/* Define the union U_HDCP2X_RPT_V0,ATTR:,ADDR:1C124,INIT:00,MASK:ff,CMMNT:HDCP2X REPEATER SEQ_NUM_V≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_rpt_v0 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_V0;

/* Define the union U_HDCP2X_RPT_V1 */
/* Define the union U_HDCP2X_RPT_V1,ATTR:,ADDR:1C128,INIT:00,MASK:ff,CMMNT:HDCP2X REPEATER SEQ_NUM_V≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_rpt_v1 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_V1;

/* Define the union U_HDCP2X_RPT_V2 */
/* Define the union U_HDCP2X_RPT_V2,ATTR:,ADDR:1C12C,INIT:00,MASK:ff,CMMNT:HDCP2X REPEATER SEQ_NUM_V≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_rpt_v2 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_V2;

/* Define the union U_HDCP2X_RPT_K0 */
/* Define the union U_HDCP2X_RPT_K0,ATTR:,ADDR:1C130,INIT:01,MASK:ff,CMMNT:HDCP2X REPEATER SEQ_NUM_V≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_rpt_k0 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_K0;

/* Define the union U_HDCP2X_RPT_K1 */
/* Define the union U_HDCP2X_RPT_K1,ATTR:,ADDR:1C134,INIT:00,MASK:ff,CMMNT:HDCP2X REPEATER SEQ_NUM_V≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_rpt_k1 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_K1;

/* Define the union U_HDCP2X_RPT_M0 */
/* Define the union U_HDCP2X_RPT_M0,ATTR:,ADDR:1C138,INIT:00,MASK:ff,CMMNT:HDCP2X REPEATER SEQ_NUM_M≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_rpt_m0 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_M0;

/* Define the union U_HDCP2X_RPT_M1 */
/* Define the union U_HDCP2X_RPT_M1,ATTR:,ADDR:1C13C,INIT:00,MASK:ff,CMMNT:HDCP2X REPEATER SEQ_NUM_M≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_rpt_m1 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_M1;

/* Define the union U_HDCP2X_RPT_M2 */
/* Define the union U_HDCP2X_RPT_M2,ATTR:,ADDR:1C140,INIT:00,MASK:ff,CMMNT:HDCP2X REPEATER SEQ_NUM_M≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_rpt_m2 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_M2;

/* Define the union U_HDCP2X_RPT_STRM_ID */
/* Define the union U_HDCP2X_RPT_STRM_ID,ATTR:,ADDR:1C144,INIT:00,MASK:ff,CMMNT:HDCP2X REPEATER STREAM ID≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_rpt_strm_id : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_STRM_ID;

/* Define the union U_HDCP2X_RPT_STRM_TPYE */
/* Define the union U_HDCP2X_RPT_STRM_TPYE,ATTR:,ADDR:1C148,INIT:01,MASK:ff,CMMNT:HDCP2X REPEATER STREAM TYPE≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_rpt_strm_type : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_RPT_STRM_TPYE;

/* Define the union U_HDCP2X_TIMING_CFG0 */
/* Define the union U_HDCP2X_TIMING_CFG0,ATTR:,ADDR:1C1A4,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg0 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG0;

/* Define the union U_HDCP2X_TIMING_CFG1 */
/* Define the union U_HDCP2X_TIMING_CFG1,ATTR:,ADDR:1C1A8,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg1 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG1;

/* Define the union U_HDCP2X_TIMING_CFG2 */
/* Define the union U_HDCP2X_TIMING_CFG2,ATTR:,ADDR:1C1AC,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg2 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG2;

/* Define the union U_HDCP2X_TIMING_CFG3 */
/* Define the union U_HDCP2X_TIMING_CFG3,ATTR:,ADDR:1C1B0,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg3 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG3;

/* Define the union U_HDCP2X_TIMING_CFG4 */
/* Define the union U_HDCP2X_TIMING_CFG4,ATTR:,ADDR:1C1B4,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg4 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG4;

/* Define the union U_HDCP2X_TIMING_CFG5 */
/* Define the union U_HDCP2X_TIMING_CFG5,ATTR:,ADDR:1C1B8,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg5 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG5;

/* Define the union U_HDCP2X_TIMING_CFG6 */
/* Define the union U_HDCP2X_TIMING_CFG6,ATTR:,ADDR:1C1BC,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg6 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG6;

/* Define the union U_HDCP2X_TIMING_CFG7 */
/* Define the union U_HDCP2X_TIMING_CFG7,ATTR:,ADDR:1C1C0,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg7 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG7;

/* Define the union U_HDCP2X_TIMING_CFG8 */
/* Define the union U_HDCP2X_TIMING_CFG8,ATTR:,ADDR:1C1C4,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg8 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG8;

/* Define the union U_HDCP2X_TIMING_CFG9 */
/* Define the union U_HDCP2X_TIMING_CFG9,ATTR:,ADDR:1C1C8,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg9 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG9;

/* Define the union U_HDCP2X_TIMING_CFG10 */
/* Define the union U_HDCP2X_TIMING_CFG10,ATTR:,ADDR:1C1CC,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg10 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG10;

/* Define the union U_HDCP2X_TIMING_CFG11 */
/* Define the union U_HDCP2X_TIMING_CFG11,ATTR:,ADDR:1C1D0,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg11 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG11;

/* Define the union U_HDCP2X_TIMING_CFG12 */
/* Define the union U_HDCP2X_TIMING_CFG12,ATTR:,ADDR:1C1D4,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg12 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG12;

/* Define the union U_HDCP2X_TIMING_CFG13 */
/* Define the union U_HDCP2X_TIMING_CFG13,ATTR:,ADDR:1C1D8,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg13 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG13;

/* Define the union U_HDCP2X_TIMING_CFG14 */
/* Define the union U_HDCP2X_TIMING_CFG14,ATTR:,ADDR:1C1DC,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg14 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG14;

/* Define the union U_HDCP2X_TIMING_CFG15 */
/* Define the union U_HDCP2X_TIMING_CFG15,ATTR:,ADDR:1C1E0,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg15 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG15;

/* Define the union U_HDCP2X_TIMING_CFG16 */
/* Define the union U_HDCP2X_TIMING_CFG16,ATTR:,ADDR:1C1E4,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg16 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG16;

/* Define the union U_HDCP2X_TIMING_CFG17 */
/* Define the union U_HDCP2X_TIMING_CFG17,ATTR:,ADDR:1C1E8,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg17 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG17;

/* Define the union U_HDCP2X_TIMING_CFG18 */
/* Define the union U_HDCP2X_TIMING_CFG18,ATTR:,ADDR:1C1EC,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg18 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG18;

/* Define the union U_HDCP2X_TIMING_CFG19 */
/* Define the union U_HDCP2X_TIMING_CFG19,ATTR:,ADDR:1C1F0,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg19 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG19;

/* Define the union U_HDCP2X_TIMING_CFG20 */
/* Define the union U_HDCP2X_TIMING_CFG20,ATTR:,ADDR:1C1F4,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg20 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG20;

/* Define the union U_HDCP2X_TIMING_CFG21 */
/* Define the union U_HDCP2X_TIMING_CFG21,ATTR:,ADDR:1C1F8,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg21 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG21;

/* Define the union U_HDCP2X_TIMING_CFG22 */
/* Define the union U_HDCP2X_TIMING_CFG22,ATTR:,ADDR:1C1FC,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg22 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG22;

/* Define the union U_HDCP2X_TIMING_CFG23 */
/* Define the union U_HDCP2X_TIMING_CFG23,ATTR:,ADDR:1C200,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg23 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG23;

/* Define the union U_HDCP2X_TIMING_CFG24 */
/* Define the union U_HDCP2X_TIMING_CFG24,ATTR:,ADDR:1C204,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg24 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG24;

/* Define the union U_HDCP2X_TIMING_CFG25 */
/* Define the union U_HDCP2X_TIMING_CFG25,ATTR:,ADDR:1C208,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg25 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG25;

/* Define the union U_HDCP2X_TIMING_CFG26 */
/* Define the union U_HDCP2X_TIMING_CFG26,ATTR:,ADDR:1C20C,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg26 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG26;

/* Define the union U_HDCP2X_TIMING_CFG27 */
/* Define the union U_HDCP2X_TIMING_CFG27,ATTR:,ADDR:1C210,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg27 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG27;

/* Define the union U_HDCP2X_TIMING_CFG28 */
/* Define the union U_HDCP2X_TIMING_CFG28,ATTR:,ADDR:1C214,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg28 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG28;

/* Define the union U_HDCP2X_TIMING_CFG29 */
/* Define the union U_HDCP2X_TIMING_CFG29,ATTR:,ADDR:1C218,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg29 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG29;

/* Define the union U_HDCP2X_TIMING_CFG30 */
/* Define the union U_HDCP2X_TIMING_CFG30,ATTR:,ADDR:1C21C,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg30 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG30;

/* Define the union U_HDCP2X_TIMING_CFG31 */
/* Define the union U_HDCP2X_TIMING_CFG31,ATTR:,ADDR:1C220,INIT:00,MASK:ff,CMMNT:HDCP2X »œ÷§≤“ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_timing_cfg31 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_TIMING_CFG31;

/* Define the union U_HDCP2X_GEN_IN0 */
/* Define the union U_HDCP2X_GEN_IN0,ATTR:,ADDR:1C224,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in0 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN0;

/* Define the union U_HDCP2X_GEN_IN1 */
/* Define the union U_HDCP2X_GEN_IN1,ATTR:,ADDR:1C228,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in1 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN1;

/* Define the union U_HDCP2X_GEN_IN2 */
/* Define the union U_HDCP2X_GEN_IN2,ATTR:,ADDR:1C22C,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in2 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN2;

/* Define the union U_HDCP2X_GEN_IN3 */
/* Define the union U_HDCP2X_GEN_IN3,ATTR:,ADDR:1C230,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in3 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN3;

/* Define the union U_HDCP2X_GEN_IN4 */
/* Define the union U_HDCP2X_GEN_IN4,ATTR:,ADDR:1C234,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in4 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN4;

/* Define the union U_HDCP2X_GEN_IN5 */
/* Define the union U_HDCP2X_GEN_IN5,ATTR:,ADDR:1C238,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in5 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN5;

/* Define the union U_HDCP2X_GEN_IN6 */
/* Define the union U_HDCP2X_GEN_IN6,ATTR:,ADDR:1C23C,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in6 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN6;

/* Define the union U_HDCP2X_GEN_IN7 */
/* Define the union U_HDCP2X_GEN_IN7,ATTR:,ADDR:1C240,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in7 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN7;

/* Define the union U_HDCP2X_GEN_IN8 */
/* Define the union U_HDCP2X_GEN_IN8,ATTR:,ADDR:1C244,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in8 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN8;

/* Define the union U_HDCP2X_GEN_IN9 */
/* Define the union U_HDCP2X_GEN_IN9,ATTR:,ADDR:1C248,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in9 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN9;

/* Define the union U_HDCP2X_GEN_IN10 */
/* Define the union U_HDCP2X_GEN_IN10,ATTR:,ADDR:1C24C,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in10 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN10;

/* Define the union U_HDCP2X_GEN_IN11 */
/* Define the union U_HDCP2X_GEN_IN11,ATTR:,ADDR:1C250,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in11 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN11;

/* Define the union U_HDCP2X_GEN_IN12 */
/* Define the union U_HDCP2X_GEN_IN12,ATTR:,ADDR:1C254,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in12 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN12;

/* Define the union U_HDCP2X_GEN_IN13 */
/* Define the union U_HDCP2X_GEN_IN13,ATTR:,ADDR:1C258,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in13 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN13;

/* Define the union U_HDCP2X_GEN_IN14 */
/* Define the union U_HDCP2X_GEN_IN14,ATTR:,ADDR:1C25C,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in14 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN14;

/* Define the union U_HDCP2X_GEN_IN15 */
/* Define the union U_HDCP2X_GEN_IN15,ATTR:,ADDR:1C260,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in15 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN15;

/* Define the union U_HDCP2X_GEN_IN16 */
/* Define the union U_HDCP2X_GEN_IN16,ATTR:,ADDR:1C264,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in16 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN16;

/* Define the union U_HDCP2X_GEN_IN17 */
/* Define the union U_HDCP2X_GEN_IN17,ATTR:,ADDR:1C268,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in17 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN17;

/* Define the union U_HDCP2X_GEN_IN18 */
/* Define the union U_HDCP2X_GEN_IN18,ATTR:,ADDR:1C26C,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in18 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN18;

/* Define the union U_HDCP2X_GEN_IN19 */
/* Define the union U_HDCP2X_GEN_IN19,ATTR:,ADDR:1C270,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in19 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN19;

/* Define the union U_HDCP2X_GEN_IN20 */
/* Define the union U_HDCP2X_GEN_IN20,ATTR:,ADDR:1C274,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in20 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN20;

/* Define the union U_HDCP2X_GEN_IN21 */
/* Define the union U_HDCP2X_GEN_IN21,ATTR:,ADDR:1C278,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in21 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN21;

/* Define the union U_HDCP2X_GEN_IN22 */
/* Define the union U_HDCP2X_GEN_IN22,ATTR:,ADDR:1C27C,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in22 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN22;

/* Define the union U_HDCP2X_GEN_IN23 */
/* Define the union U_HDCP2X_GEN_IN23,ATTR:,ADDR:1C280,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in23 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN23;

/* Define the union U_HDCP2X_GEN_IN24 */
/* Define the union U_HDCP2X_GEN_IN24,ATTR:,ADDR:1C284,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in24 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN24;

/* Define the union U_HDCP2X_GEN_IN25 */
/* Define the union U_HDCP2X_GEN_IN25,ATTR:,ADDR:1C288,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in25 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN25;

/* Define the union U_HDCP2X_GEN_IN26 */
/* Define the union U_HDCP2X_GEN_IN26,ATTR:,ADDR:1C28C,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in26 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN26;

/* Define the union U_HDCP2X_GEN_IN27 */
/* Define the union U_HDCP2X_GEN_IN27,ATTR:,ADDR:1C290,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in27 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN27;

/* Define the union U_HDCP2X_GEN_IN28 */
/* Define the union U_HDCP2X_GEN_IN28,ATTR:,ADDR:1C294,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in28 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN28;

/* Define the union U_HDCP2X_GEN_IN29 */
/* Define the union U_HDCP2X_GEN_IN29,ATTR:,ADDR:1C298,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in29 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN29;

/* Define the union U_HDCP2X_GEN_IN30 */
/* Define the union U_HDCP2X_GEN_IN30,ATTR:,ADDR:1C29C,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in30 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN30;

/* Define the union U_HDCP2X_GEN_IN31 */
/* Define the union U_HDCP2X_GEN_IN31,ATTR:,ADDR:1C2A0,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_in31 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_IN31;

/* Define the union U_HDCP2X_GEN_OUT0 */
/* Define the union U_HDCP2X_GEN_OUT0,ATTR:,ADDR:1C2A4,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out0 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT0;

/* Define the union U_HDCP2X_GEN_OUT1 */
/* Define the union U_HDCP2X_GEN_OUT1,ATTR:,ADDR:1C2A8,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out1 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT1;

/* Define the union U_HDCP2X_GEN_OUT2 */
/* Define the union U_HDCP2X_GEN_OUT2,ATTR:,ADDR:1C2AC,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out2 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT2;

/* Define the union U_HDCP2X_GEN_OUT3 */
/* Define the union U_HDCP2X_GEN_OUT3,ATTR:,ADDR:1C2B0,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out3 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT3;

/* Define the union U_HDCP2X_GEN_OUT4 */
/* Define the union U_HDCP2X_GEN_OUT4,ATTR:,ADDR:1C2B4,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out4 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT4;

/* Define the union U_HDCP2X_GEN_OUT5 */
/* Define the union U_HDCP2X_GEN_OUT5,ATTR:,ADDR:1C2B8,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out5 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT5;

/* Define the union U_HDCP2X_GEN_OUT6 */
/* Define the union U_HDCP2X_GEN_OUT6,ATTR:,ADDR:1C2BC,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out6 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT6;

/* Define the union U_HDCP2X_GEN_OUT7 */
/* Define the union U_HDCP2X_GEN_OUT7,ATTR:,ADDR:1C2C0,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out7 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT7;

/* Define the union U_HDCP2X_GEN_OUT8 */
/* Define the union U_HDCP2X_GEN_OUT8,ATTR:,ADDR:1C2C4,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out8 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT8;

/* Define the union U_HDCP2X_GEN_OUT9 */
/* Define the union U_HDCP2X_GEN_OUT9,ATTR:,ADDR:1C2C8,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out9 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT9;

/* Define the union U_HDCP2X_GEN_OUT10 */
/* Define the union U_HDCP2X_GEN_OUT10,ATTR:,ADDR:1C2CC,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out10 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT10;

/* Define the union U_HDCP2X_GEN_OUT11 */
/* Define the union U_HDCP2X_GEN_OUT11,ATTR:,ADDR:1C2D0,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out11 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT11;

/* Define the union U_HDCP2X_GEN_OUT12 */
/* Define the union U_HDCP2X_GEN_OUT12,ATTR:,ADDR:1C2D4,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out12 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT12;

/* Define the union U_HDCP2X_GEN_OUT13 */
/* Define the union U_HDCP2X_GEN_OUT13,ATTR:,ADDR:1C2D8,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out13 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT13;

/* Define the union U_HDCP2X_GEN_OUT14 */
/* Define the union U_HDCP2X_GEN_OUT14,ATTR:,ADDR:1C2DC,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out14 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT14;

/* Define the union U_HDCP2X_GEN_OUT15 */
/* Define the union U_HDCP2X_GEN_OUT15,ATTR:,ADDR:1C2E0,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out15 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT15;

/* Define the union U_HDCP2X_GEN_OUT16 */
/* Define the union U_HDCP2X_GEN_OUT16,ATTR:,ADDR:1C2E4,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out16 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT16;

/* Define the union U_HDCP2X_GEN_OUT17 */
/* Define the union U_HDCP2X_GEN_OUT17,ATTR:,ADDR:1C2E8,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out17 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT17;

/* Define the union U_HDCP2X_GEN_OUT18 */
/* Define the union U_HDCP2X_GEN_OUT18,ATTR:,ADDR:1C2EC,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out18 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT18;

/* Define the union U_HDCP2X_GEN_OUT19 */
/* Define the union U_HDCP2X_GEN_OUT19,ATTR:,ADDR:1C2F0,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out19 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT19;

/* Define the union U_HDCP2X_GEN_OUT20 */
/* Define the union U_HDCP2X_GEN_OUT20,ATTR:,ADDR:1C2F4,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out20 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT20;

/* Define the union U_HDCP2X_GEN_OUT21 */
/* Define the union U_HDCP2X_GEN_OUT21,ATTR:,ADDR:1C2F8,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out21 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT21;

/* Define the union U_HDCP2X_GEN_OUT22 */
/* Define the union U_HDCP2X_GEN_OUT22,ATTR:,ADDR:1C2FC,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out22 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT22;

/* Define the union U_HDCP2X_GEN_OUT23 */
/* Define the union U_HDCP2X_GEN_OUT23,ATTR:,ADDR:1C300,INIT:FF,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out23 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT23;

/* Define the union U_HDCP2X_GEN_OUT24 */
/* Define the union U_HDCP2X_GEN_OUT24,ATTR:,ADDR:1C304,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out24 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT24;

/* Define the union U_HDCP2X_GEN_OUT25 */
/* Define the union U_HDCP2X_GEN_OUT25,ATTR:,ADDR:1C308,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out25 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT25;

/* Define the union U_HDCP2X_GEN_OUT26 */
/* Define the union U_HDCP2X_GEN_OUT26,ATTR:,ADDR:1C30C,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out26 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT26;

/* Define the union U_HDCP2X_GEN_OUT27 */
/* Define the union U_HDCP2X_GEN_OUT27,ATTR:,ADDR:1C310,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out27 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT27;

/* Define the union U_HDCP2X_GEN_OUT28 */
/* Define the union U_HDCP2X_GEN_OUT28,ATTR:,ADDR:1C314,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out28 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT28;

/* Define the union U_HDCP2X_GEN_OUT29 */
/* Define the union U_HDCP2X_GEN_OUT29,ATTR:,ADDR:1C318,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out29 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT29;

/* Define the union U_HDCP2X_GEN_OUT30 */
/* Define the union U_HDCP2X_GEN_OUT30,ATTR:,ADDR:1C31C,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out30 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT30;

/* Define the union U_HDCP2X_GEN_OUT31 */
/* Define the union U_HDCP2X_GEN_OUT31,ATTR:,ADDR:1C320,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_gen_out31 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_GEN_OUT31;

/* Define the union U_HDCP2X_SEC_IN0 */
/* Define the union U_HDCP2X_SEC_IN0,ATTR:,ADDR:1C380,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_in0 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_IN0;

/* Define the union U_HDCP2X_SEC_IN1 */
/* Define the union U_HDCP2X_SEC_IN1,ATTR:,ADDR:1C384,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_in1 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_IN1;

/* Define the union U_HDCP2X_SEC_IN2 */
/* Define the union U_HDCP2X_SEC_IN2,ATTR:,ADDR:1C388,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_in2 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_IN2;

/* Define the union U_HDCP2X_SEC_IN3 */
/* Define the union U_HDCP2X_SEC_IN3,ATTR:,ADDR:1C38C,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_in3 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_IN3;

/* Define the union U_HDCP2X_SEC_IN4 */
/* Define the union U_HDCP2X_SEC_IN4,ATTR:,ADDR:1C390,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_in4 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_IN4;

/* Define the union U_HDCP2X_SEC_IN5 */
/* Define the union U_HDCP2X_SEC_IN5,ATTR:,ADDR:1C394,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_in5 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_IN5;

/* Define the union U_HDCP2X_SEC_IN6 */
/* Define the union U_HDCP2X_SEC_IN6,ATTR:,ADDR:1C398,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_in6 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_IN6;

/* Define the union U_HDCP2X_SEC_IN7 */
/* Define the union U_HDCP2X_SEC_IN7,ATTR:,ADDR:1C39C,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_in7 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_IN7;

/* Define the union U_HDCP2X_SEC_OUT0 */
/* Define the union U_HDCP2X_SEC_OUT0,ATTR:,ADDR:1C3A0,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_out0 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_OUT0;

/* Define the union U_HDCP2X_SEC_OUT1 */
/* Define the union U_HDCP2X_SEC_OUT1,ATTR:,ADDR:1C3A4,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_out1 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_OUT1;

/* Define the union U_HDCP2X_SEC_OUT2 */
/* Define the union U_HDCP2X_SEC_OUT2,ATTR:,ADDR:1C3A8,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_out2 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_OUT2;

/* Define the union U_HDCP2X_SEC_OUT3 */
/* Define the union U_HDCP2X_SEC_OUT3,ATTR:,ADDR:1C3AC,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_out3 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_OUT3;

/* Define the union U_HDCP2X_SEC_OUT4 */
/* Define the union U_HDCP2X_SEC_OUT4,ATTR:,ADDR:1C3B0,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_out4 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_OUT4;

/* Define the union U_HDCP2X_SEC_OUT5 */
/* Define the union U_HDCP2X_SEC_OUT5,ATTR:,ADDR:1C3B4,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_out5 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_OUT5;

/* Define the union U_HDCP2X_SEC_OUT6 */
/* Define the union U_HDCP2X_SEC_OUT6,ATTR:,ADDR:1C3B8,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_out6 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_OUT6;

/* Define the union U_HDCP2X_SEC_OUT7 */
/* Define the union U_HDCP2X_SEC_OUT7,ATTR:,ADDR:1C3BC,INIT:00,MASK:ff,CMMNT:HDCP2X Õ®”√∞≤»´≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdcp2x_sec_out7 : 8  ; /* [7:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDCP2X_SEC_OUT7;


//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_HDCP2X_HW_VER_B0      HDCP2X_HW_VER_B0      ; /* 1C000 */
    volatile U_HDCP2X_HW_VER_B1      HDCP2X_HW_VER_B1      ; /* 1C004 */
    volatile U_HDCP2X_HW_VER_B2      HDCP2X_HW_VER_B2      ; /* 1C008 */
    volatile U_HDCP2X_HW_VER_B3      HDCP2X_HW_VER_B3      ; /* 1C00C */
    volatile U_HDCP2X_SW_VER_B0      HDCP2X_SW_VER_B0      ; /* 1C010 */
    volatile U_HDCP2X_SW_VER_B1      HDCP2X_SW_VER_B1      ; /* 1C014 */
    volatile U_HDCP2X_SW_VER_B2      HDCP2X_SW_VER_B2      ; /* 1C018 */
    volatile U_HDCP2X_SW_VER_B3      HDCP2X_SW_VER_B3      ; /* 1C01C */
    unsigned int                     Reserved_0[13]        ; /* 1C020-1C050 */
    volatile U_HDCP2X_AUTH_CTRL      HDCP2X_AUTH_CTRL      ; /* 1C054 */
    unsigned int                     Reserved_1[10]        ; /* 1C058-1C07C */
    volatile U_HDCP2X_SEC_INTR_B0    HDCP2X_SEC_INTR_B0    ; /* 1C080 */
    volatile U_HDCP2X_SEC_INTR_B1    HDCP2X_SEC_INTR_B1    ; /* 1C084 */
    volatile U_HDCP2X_SEC_MASK_B0    HDCP2X_SEC_MASK_B0    ; /* 1C088 */
    volatile U_HDCP2X_SEC_MASK_B1    HDCP2X_SEC_MASK_B1    ; /* 1C08C */
    unsigned int                     Reserved_2[5]         ; /* 1C090-1C0A0 */
    volatile U_HDCP2X_MCU_INTR_B0    HDCP2X_MCU_INTR_B0    ; /* 1C0A4 */
    volatile U_HDCP2X_MCU_INTR_B1    HDCP2X_MCU_INTR_B1    ; /* 1C0A8 */
    volatile U_HDCP2X_MCU_INTR_B2    HDCP2X_MCU_INTR_B2    ; /* 1C0AC */
    volatile U_HDCP2X_MCU_INTR_B3    HDCP2X_MCU_INTR_B3    ; /* 1C0B0 */
    volatile U_HDCP2X_MCU_MASK_B0    HDCP2X_MCU_MASK_B0    ; /* 1C0B4 */
    volatile U_HDCP2X_MCU_MASK_B1    HDCP2X_MCU_MASK_B1    ; /* 1C0B8 */
    volatile U_HDCP2X_MCU_MASK_B2    HDCP2X_MCU_MASK_B2    ; /* 1C0BC */
    volatile U_HDCP2X_MCU_MASK_B3    HDCP2X_MCU_MASK_B3    ; /* 1C0C0 */
    unsigned int                     Reserved_3[2]         ; /* 1C0C4-1C0C8 */	
    volatile U_HDCP2X_MCU_STATE_B0   HDCP2X_MCU_STATE_B0   ; /* 1C0CC */
    volatile U_HDCP2X_MCU_STATE_B1   HDCP2X_MCU_STATE_B1   ; /* 1C0D0 */
    volatile U_HDCP2X_MCU_STATE_B2   HDCP2X_MCU_STATE_B2   ; /* 1C0D4 */
    volatile U_HDCP2X_MCU_STATE_B3   HDCP2X_MCU_STATE_B3   ; /* 1C0D8 */
    unsigned int                     Reserved_4[3]         ; /* 1C0DC-1C0E4 */	
    volatile U_HDCP2X_RX_AUTH_STATE  HDCP2X_RX_AUTH_STATE  ; /* 1C0E8 */
    volatile U_HDCP2X_RPT_AUTH_STATE HDCP2X_RPT_AUTH_STATE ; /* 1C0EC */
    volatile U_HDCP2X_AUTH_STATE0    HDCP2X_AUTH_STATE0    ; /* 1C0F0 */
    volatile U_HDCP2X_AUTH_STATE1    HDCP2X_AUTH_STATE1    ; /* 1C0F4 */
    volatile U_HDCP2X_AUTH_STATE2    HDCP2X_AUTH_STATE2    ; /* 1C0F8 */
    volatile U_HDCP2X_AUTH_STATE3    HDCP2X_AUTH_STATE3    ; /* 1C0FC */
    unsigned int                     Reserved_5[5]         ; /* 1C100-1C110 */	
    volatile U_HDCP2X_CERT_INFO      HDCP2X_CERT_INFO      ; /* 1C114 */
    volatile U_HDCP2X_CERT_VER       HDCP2X_CERT_VER       ; /* 1C118 */
    volatile U_HDCP2X_RPT_INFO       HDCP2X_RPT_INFO       ; /* 1C11C */
    volatile U_HDCP2X_RPT_DEV        HDCP2X_RPT_DEV        ; /* 1C120 */
    volatile U_HDCP2X_RPT_V0         HDCP2X_RPT_V0         ; /* 1C124 */
    volatile U_HDCP2X_RPT_V1         HDCP2X_RPT_V1         ; /* 1C128 */
    volatile U_HDCP2X_RPT_V2         HDCP2X_RPT_V2         ; /* 1C12C */
    volatile U_HDCP2X_RPT_K0         HDCP2X_RPT_K0         ; /* 1C130 */
    volatile U_HDCP2X_RPT_K1         HDCP2X_RPT_K1         ; /* 1C134 */
    volatile U_HDCP2X_RPT_M0         HDCP2X_RPT_M0         ; /* 1C138 */
    volatile U_HDCP2X_RPT_M1         HDCP2X_RPT_M1         ; /* 1C13C */
    volatile U_HDCP2X_RPT_M2         HDCP2X_RPT_M2         ; /* 1C140 */
    volatile U_HDCP2X_RPT_STRM_ID    HDCP2X_RPT_STRM_ID    ; /* 1C144 */
    volatile U_HDCP2X_RPT_STRM_TPYE  HDCP2X_RPT_STRM_TPYE  ; /* 1C148 */
    unsigned int                     Reserved_6[22]        ; /* 1C14C-1C1A0 */
    volatile U_HDCP2X_TIMING_CFG0    HDCP2X_TIMING_CFG0    ; /* 1C1A4 */
    volatile U_HDCP2X_TIMING_CFG1    HDCP2X_TIMING_CFG1    ; /* 1C1A8 */
    volatile U_HDCP2X_TIMING_CFG2    HDCP2X_TIMING_CFG2    ; /* 1C1AC */
    volatile U_HDCP2X_TIMING_CFG3    HDCP2X_TIMING_CFG3    ; /* 1C1B0 */
    volatile U_HDCP2X_TIMING_CFG4    HDCP2X_TIMING_CFG4    ; /* 1C1B4 */
    volatile U_HDCP2X_TIMING_CFG5    HDCP2X_TIMING_CFG5    ; /* 1C1B8 */
    volatile U_HDCP2X_TIMING_CFG6    HDCP2X_TIMING_CFG6    ; /* 1C1BC */
    volatile U_HDCP2X_TIMING_CFG7    HDCP2X_TIMING_CFG7    ; /* 1C1C0 */
    volatile U_HDCP2X_TIMING_CFG8    HDCP2X_TIMING_CFG8    ; /* 1C1C4 */
    volatile U_HDCP2X_TIMING_CFG9    HDCP2X_TIMING_CFG9    ; /* 1C1C8 */
    volatile U_HDCP2X_TIMING_CFG10   HDCP2X_TIMING_CFG10   ; /* 1C1CC */
    volatile U_HDCP2X_TIMING_CFG11   HDCP2X_TIMING_CFG11   ; /* 1C1D0 */
    volatile U_HDCP2X_TIMING_CFG12   HDCP2X_TIMING_CFG12   ; /* 1C1D4 */
    volatile U_HDCP2X_TIMING_CFG13   HDCP2X_TIMING_CFG13   ; /* 1C1D8 */
    volatile U_HDCP2X_TIMING_CFG14   HDCP2X_TIMING_CFG14   ; /* 1C1DC */
    volatile U_HDCP2X_TIMING_CFG15   HDCP2X_TIMING_CFG15   ; /* 1C1E0 */
    volatile U_HDCP2X_TIMING_CFG16   HDCP2X_TIMING_CFG16   ; /* 1C1E4 */
    volatile U_HDCP2X_TIMING_CFG17   HDCP2X_TIMING_CFG17   ; /* 1C1E8 */
    volatile U_HDCP2X_TIMING_CFG18   HDCP2X_TIMING_CFG18   ; /* 1C1EC */
    volatile U_HDCP2X_TIMING_CFG19   HDCP2X_TIMING_CFG19   ; /* 1C1F0 */
    volatile U_HDCP2X_TIMING_CFG20   HDCP2X_TIMING_CFG20   ; /* 1C1F4 */
    volatile U_HDCP2X_TIMING_CFG21   HDCP2X_TIMING_CFG21   ; /* 1C1F8 */
    volatile U_HDCP2X_TIMING_CFG22   HDCP2X_TIMING_CFG22   ; /* 1C1FC */
    volatile U_HDCP2X_TIMING_CFG23   HDCP2X_TIMING_CFG23   ; /* 1C200 */
    volatile U_HDCP2X_TIMING_CFG24   HDCP2X_TIMING_CFG24   ; /* 1C204 */
    volatile U_HDCP2X_TIMING_CFG25   HDCP2X_TIMING_CFG25   ; /* 1C208 */
    volatile U_HDCP2X_TIMING_CFG26   HDCP2X_TIMING_CFG26   ; /* 1C20C */
    volatile U_HDCP2X_TIMING_CFG27   HDCP2X_TIMING_CFG27   ; /* 1C210 */
    volatile U_HDCP2X_TIMING_CFG28   HDCP2X_TIMING_CFG28   ; /* 1C214 */
    volatile U_HDCP2X_TIMING_CFG29   HDCP2X_TIMING_CFG29   ; /* 1C218 */
    volatile U_HDCP2X_TIMING_CFG30   HDCP2X_TIMING_CFG30   ; /* 1C21C */
    volatile U_HDCP2X_TIMING_CFG31   HDCP2X_TIMING_CFG31   ; /* 1C220 */
    volatile U_HDCP2X_GEN_IN0        HDCP2X_GEN_IN0        ; /* 1C224 */
    volatile U_HDCP2X_GEN_IN1        HDCP2X_GEN_IN1        ; /* 1C228 */
    volatile U_HDCP2X_GEN_IN2        HDCP2X_GEN_IN2        ; /* 1C22C */
    volatile U_HDCP2X_GEN_IN3        HDCP2X_GEN_IN3        ; /* 1C230 */
    volatile U_HDCP2X_GEN_IN4        HDCP2X_GEN_IN4        ; /* 1C234 */
    volatile U_HDCP2X_GEN_IN5        HDCP2X_GEN_IN5        ; /* 1C238 */
    volatile U_HDCP2X_GEN_IN6        HDCP2X_GEN_IN6        ; /* 1C23C */
    volatile U_HDCP2X_GEN_IN7        HDCP2X_GEN_IN7        ; /* 1C240 */
    volatile U_HDCP2X_GEN_IN8        HDCP2X_GEN_IN8        ; /* 1C244 */
    volatile U_HDCP2X_GEN_IN9        HDCP2X_GEN_IN9        ; /* 1C248 */
    volatile U_HDCP2X_GEN_IN10       HDCP2X_GEN_IN10       ; /* 1C24C */
    volatile U_HDCP2X_GEN_IN11       HDCP2X_GEN_IN11       ; /* 1C250 */
    volatile U_HDCP2X_GEN_IN12       HDCP2X_GEN_IN12       ; /* 1C254 */
    volatile U_HDCP2X_GEN_IN13       HDCP2X_GEN_IN13       ; /* 1C258 */
    volatile U_HDCP2X_GEN_IN14       HDCP2X_GEN_IN14       ; /* 1C25C */
    volatile U_HDCP2X_GEN_IN15       HDCP2X_GEN_IN15       ; /* 1C260 */
    volatile U_HDCP2X_GEN_IN16       HDCP2X_GEN_IN16       ; /* 1C264 */
    volatile U_HDCP2X_GEN_IN17       HDCP2X_GEN_IN17       ; /* 1C268 */
    volatile U_HDCP2X_GEN_IN18       HDCP2X_GEN_IN18       ; /* 1C26C */
    volatile U_HDCP2X_GEN_IN19       HDCP2X_GEN_IN19       ; /* 1C270 */
    volatile U_HDCP2X_GEN_IN20       HDCP2X_GEN_IN20       ; /* 1C274 */
    volatile U_HDCP2X_GEN_IN21       HDCP2X_GEN_IN21       ; /* 1C278 */
    volatile U_HDCP2X_GEN_IN22       HDCP2X_GEN_IN22       ; /* 1C27C */
    volatile U_HDCP2X_GEN_IN23       HDCP2X_GEN_IN23       ; /* 1C280 */
    volatile U_HDCP2X_GEN_IN24       HDCP2X_GEN_IN24       ; /* 1C284 */
    volatile U_HDCP2X_GEN_IN25       HDCP2X_GEN_IN25       ; /* 1C288 */
    volatile U_HDCP2X_GEN_IN26       HDCP2X_GEN_IN26       ; /* 1C28C */
    volatile U_HDCP2X_GEN_IN27       HDCP2X_GEN_IN27       ; /* 1C290 */
    volatile U_HDCP2X_GEN_IN28       HDCP2X_GEN_IN28       ; /* 1C294 */
    volatile U_HDCP2X_GEN_IN29       HDCP2X_GEN_IN29       ; /* 1C298 */
    volatile U_HDCP2X_GEN_IN30       HDCP2X_GEN_IN30       ; /* 1C29C */
    volatile U_HDCP2X_GEN_IN31       HDCP2X_GEN_IN31       ; /* 1C2A0 */
    volatile U_HDCP2X_GEN_OUT0       HDCP2X_GEN_OUT0       ; /* 1C2A4 */
    volatile U_HDCP2X_GEN_OUT1       HDCP2X_GEN_OUT1       ; /* 1C2A8 */
    volatile U_HDCP2X_GEN_OUT2       HDCP2X_GEN_OUT2       ; /* 1C2AC */
    volatile U_HDCP2X_GEN_OUT3       HDCP2X_GEN_OUT3       ; /* 1C2B0 */
    volatile U_HDCP2X_GEN_OUT4       HDCP2X_GEN_OUT4       ; /* 1C2B4 */
    volatile U_HDCP2X_GEN_OUT5       HDCP2X_GEN_OUT5       ; /* 1C2B8 */
    volatile U_HDCP2X_GEN_OUT6       HDCP2X_GEN_OUT6       ; /* 1C2BC */
    volatile U_HDCP2X_GEN_OUT7       HDCP2X_GEN_OUT7       ; /* 1C2C0 */
    volatile U_HDCP2X_GEN_OUT8       HDCP2X_GEN_OUT8       ; /* 1C2C4 */
    volatile U_HDCP2X_GEN_OUT9       HDCP2X_GEN_OUT9       ; /* 1C2C8 */
    volatile U_HDCP2X_GEN_OUT10      HDCP2X_GEN_OUT10      ; /* 1C2CC */
    volatile U_HDCP2X_GEN_OUT11      HDCP2X_GEN_OUT11      ; /* 1C2D0 */
    volatile U_HDCP2X_GEN_OUT12      HDCP2X_GEN_OUT12      ; /* 1C2D4 */
    volatile U_HDCP2X_GEN_OUT13      HDCP2X_GEN_OUT13      ; /* 1C2D8 */
    volatile U_HDCP2X_GEN_OUT14      HDCP2X_GEN_OUT14      ; /* 1C2DC */
    volatile U_HDCP2X_GEN_OUT15      HDCP2X_GEN_OUT15      ; /* 1C2E0 */
    volatile U_HDCP2X_GEN_OUT16      HDCP2X_GEN_OUT16      ; /* 1C2E4 */
    volatile U_HDCP2X_GEN_OUT17      HDCP2X_GEN_OUT17      ; /* 1C2E8 */
    volatile U_HDCP2X_GEN_OUT18      HDCP2X_GEN_OUT18      ; /* 1C2EC */
    volatile U_HDCP2X_GEN_OUT19      HDCP2X_GEN_OUT19      ; /* 1C2F0 */
    volatile U_HDCP2X_GEN_OUT20      HDCP2X_GEN_OUT20      ; /* 1C2F4 */
    volatile U_HDCP2X_GEN_OUT21      HDCP2X_GEN_OUT21      ; /* 1C2F8 */
    volatile U_HDCP2X_GEN_OUT22      HDCP2X_GEN_OUT22      ; /* 1C2FC */
    volatile U_HDCP2X_GEN_OUT23      HDCP2X_GEN_OUT23      ; /* 1C300 */
    volatile U_HDCP2X_GEN_OUT24      HDCP2X_GEN_OUT24      ; /* 1C304 */
    volatile U_HDCP2X_GEN_OUT25      HDCP2X_GEN_OUT25      ; /* 1C308 */
    volatile U_HDCP2X_GEN_OUT26      HDCP2X_GEN_OUT26      ; /* 1C30C */
    volatile U_HDCP2X_GEN_OUT27      HDCP2X_GEN_OUT27      ; /* 1C310 */
    volatile U_HDCP2X_GEN_OUT28      HDCP2X_GEN_OUT28      ; /* 1C314 */
    volatile U_HDCP2X_GEN_OUT29      HDCP2X_GEN_OUT29      ; /* 1C318 */
    volatile U_HDCP2X_GEN_OUT30      HDCP2X_GEN_OUT30      ; /* 1C31C */
    volatile U_HDCP2X_GEN_OUT31      HDCP2X_GEN_OUT31      ; /* 1C320 */
    unsigned int                     Reserved_7[23]        ; /* 1C324-1C37C */
    volatile U_HDCP2X_SEC_IN0        HDCP2X_SEC_IN0        ; /* 1C380 */
    volatile U_HDCP2X_SEC_IN1        HDCP2X_SEC_IN1        ; /* 1C384 */
    volatile U_HDCP2X_SEC_IN2        HDCP2X_SEC_IN2        ; /* 1C388 */
    volatile U_HDCP2X_SEC_IN3        HDCP2X_SEC_IN3        ; /* 1C38C */
    volatile U_HDCP2X_SEC_IN4        HDCP2X_SEC_IN4        ; /* 1C390 */
    volatile U_HDCP2X_SEC_IN5        HDCP2X_SEC_IN5        ; /* 1C394 */
    volatile U_HDCP2X_SEC_IN6        HDCP2X_SEC_IN6        ; /* 1C398 */
    volatile U_HDCP2X_SEC_IN7        HDCP2X_SEC_IN7        ; /* 1C39C */
    volatile U_HDCP2X_SEC_OUT0       HDCP2X_SEC_OUT0       ; /* 1C3A0 */
    volatile U_HDCP2X_SEC_OUT1       HDCP2X_SEC_OUT1       ; /* 1C3A4 */
    volatile U_HDCP2X_SEC_OUT2       HDCP2X_SEC_OUT2       ; /* 1C3A8 */
    volatile U_HDCP2X_SEC_OUT3       HDCP2X_SEC_OUT3       ; /* 1C3AC */
    volatile U_HDCP2X_SEC_OUT4       HDCP2X_SEC_OUT4       ; /* 1C3B0 */
    volatile U_HDCP2X_SEC_OUT5       HDCP2X_SEC_OUT5       ; /* 1C3B4 */
    volatile U_HDCP2X_SEC_OUT6       HDCP2X_SEC_OUT6       ; /* 1C3B8 */
    volatile U_HDCP2X_SEC_OUT7       HDCP2X_SEC_OUT7       ; /* 1C3BC */

} S_mcu_cpu_reg_REGS_TYPE;

/* Declare the struct pointor of the module mcu_cpu_reg */
//extern volatile S_mcu_cpu_reg_REGS_TYPE *gopmcu_cpu_regAllReg;
int HDMI_TX_S_mcu_cpu_reg_REGS_TYPE_Init(void);
int HDMI_TX_S_mcu_cpu_reg_REGS_TYPE_DeInit(void);

/* Declare the functions that set the member value */
int HDMI_HDCP2X_HW_VER_B0_hdcp2x_hw_ver_b0Set(unsigned int uhdcp2x_hw_ver_b0);
int HDMI_HDCP2X_HW_VER_B0_hdcp2x_hw_ver_b0Get(HI_VOID);
int HDMI_HDCP2X_HW_VER_B1_hdcp2x_hw_ver_b1Set(unsigned int uhdcp2x_hw_ver_b1);
int HDMI_HDCP2X_HW_VER_B1_hdcp2x_hw_ver_b1Get(HI_VOID);
int HDMI_HDCP2X_HW_VER_B2_hdcp2x_hw_ver_b2Set(unsigned int uhdcp2x_hw_ver_b2);
int HDMI_HDCP2X_HW_VER_B2_hdcp2x_hw_ver_b2Get(HI_VOID);
int HDMI_HDCP2X_HW_VER_B3_hdcp2x_hw_ver_b3Set(unsigned int uhdcp2x_hw_ver_b3);
int HDMI_HDCP2X_HW_VER_B3_hdcp2x_hw_ver_b3Get(HI_VOID);
int HDMI_HDCP2X_SW_VER_B0_hdcp2x_sw_ver_b0Set(unsigned int uhdcp2x_sw_ver_b0);
int HDMI_HDCP2X_SW_VER_B0_hdcp2x_sw_ver_b0Get(HI_VOID);
int HDMI_HDCP2X_SW_VER_B1_hdcp2x_sw_ver_b1Set(unsigned int uhdcp2x_sw_ver_b1);
int HDMI_HDCP2X_SW_VER_B1_hdcp2x_sw_ver_b1Get(HI_VOID);
int HDMI_HDCP2X_SW_VER_B2_hdcp2x_sw_ver_b2Set(unsigned int uhdcp2x_sw_ver_b2);
int HDMI_HDCP2X_SW_VER_B2_hdcp2x_sw_ver_b2Get(HI_VOID);
int HDMI_HDCP2X_SW_VER_B3_hdcp2x_sw_ver_b3Set(unsigned int uhdcp2x_sw_ver_b3);
int HDMI_HDCP2X_SW_VER_B3_hdcp2x_sw_ver_b3Get(HI_VOID);
int HDMI_HDCP2X_AUTH_CTRL_mcu_auth_startSet(unsigned int umcu_auth_start);
int HDMI_HDCP2X_AUTH_CTRL_mcu_auth_startGet(HI_VOID);
int HDMI_HDCP2X_AUTH_CTRL_mcu_auth_stopSet(unsigned int umcu_auth_stop);
int HDMI_HDCP2X_AUTH_CTRL_mcu_auth_stopGet(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr0Set(unsigned int uhdcp2x_sec_intr0);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr0Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr1Set(unsigned int uhdcp2x_sec_intr1);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr1Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr2Set(unsigned int uhdcp2x_sec_intr2);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr2Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr3Set(unsigned int uhdcp2x_sec_intr3);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr3Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr4Set(unsigned int uhdcp2x_sec_intr4);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr4Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr5Set(unsigned int uhdcp2x_sec_intr5);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr5Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr6Set(unsigned int uhdcp2x_sec_intr6);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr6Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr7Set(unsigned int uhdcp2x_sec_intr7);
int HDMI_HDCP2X_SEC_INTR_B0_hdcp2x_sec_intr7Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr8Set(unsigned int uhdcp2x_sec_intr8);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr8Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr9Set(unsigned int uhdcp2x_sec_intr9);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr9Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr10Set(unsigned int uhdcp2x_sec_intr10);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr10Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr11Set(unsigned int uhdcp2x_sec_intr11);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr11Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr12Set(unsigned int uhdcp2x_sec_intr12);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr12Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr13Set(unsigned int uhdcp2x_sec_intr13);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr13Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr14Set(unsigned int uhdcp2x_sec_intr14);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr14Get(HI_VOID);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr15Set(unsigned int uhdcp2x_sec_intr15);
int HDMI_HDCP2X_SEC_INTR_B1_hdcp2x_sec_intr15Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask0Set(unsigned int uhdcp2x_sec_mask0);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask0Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask1Set(unsigned int uhdcp2x_sec_mask1);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask1Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask2Set(unsigned int uhdcp2x_sec_mask2);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask2Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask3Set(unsigned int uhdcp2x_sec_mask3);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask3Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask4Set(unsigned int uhdcp2x_sec_mask4);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask4Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask5Set(unsigned int uhdcp2x_sec_mask5);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask5Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask6Set(unsigned int uhdcp2x_sec_mask6);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask6Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask7Set(unsigned int uhdcp2x_sec_mask7);
int HDMI_HDCP2X_SEC_MASK_B0_hdcp2x_sec_mask7Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask8Set(unsigned int uhdcp2x_sec_mask8);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask8Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask9Set(unsigned int uhdcp2x_sec_mask9);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask9Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask10Set(unsigned int uhdcp2x_sec_mask10);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask10Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask11Set(unsigned int uhdcp2x_sec_mask11);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask11Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask12Set(unsigned int uhdcp2x_sec_mask12);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask12Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask13Set(unsigned int uhdcp2x_sec_mask13);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask13Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask14Set(unsigned int uhdcp2x_sec_mask14);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask14Get(HI_VOID);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask15Set(unsigned int uhdcp2x_sec_mask15);
int HDMI_HDCP2X_SEC_MASK_B1_hdcp2x_sec_mask15Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr0Set(unsigned int uhdcp2x_mcu_intr0);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr0Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr1Set(unsigned int uhdcp2x_mcu_intr1);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr1Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr2Set(unsigned int uhdcp2x_mcu_intr2);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr2Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr3Set(unsigned int uhdcp2x_mcu_intr3);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr3Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr4Set(unsigned int uhdcp2x_mcu_intr4);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr4Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr5Set(unsigned int uhdcp2x_mcu_intr5);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr5Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr6Set(unsigned int uhdcp2x_mcu_intr6);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr6Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr7Set(unsigned int uhdcp2x_mcu_intr7);
int HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr7Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr8Set(unsigned int uhdcp2x_mcu_intr8);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr8Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr9Set(unsigned int uhdcp2x_mcu_intr9);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr9Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr10Set(unsigned int uhdcp2x_mcu_intr10);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr10Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr11Set(unsigned int uhdcp2x_mcu_intr11);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr11Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr12Set(unsigned int uhdcp2x_mcu_intr12);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr12Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr13Set(unsigned int uhdcp2x_mcu_intr13);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr13Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr14Set(unsigned int uhdcp2x_mcu_intr14);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr14Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr15Set(unsigned int uhdcp2x_mcu_intr15);
int HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr15Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr16Set(unsigned int uhdcp2x_mcu_intr16);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr16Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr17Set(unsigned int uhdcp2x_mcu_intr17);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr17Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr18Set(unsigned int uhdcp2x_mcu_intr18);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr18Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr19Set(unsigned int uhdcp2x_mcu_intr19);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr19Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr20Set(unsigned int uhdcp2x_mcu_intr20);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr20Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr21Set(unsigned int uhdcp2x_mcu_intr21);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr21Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr22Set(unsigned int uhdcp2x_mcu_intr22);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr22Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr23Set(unsigned int uhdcp2x_mcu_intr23);
int HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr23Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr24Set(unsigned int uhdcp2x_mcu_intr24);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr24Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr25Set(unsigned int uhdcp2x_mcu_intr25);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr25Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr26Set(unsigned int uhdcp2x_mcu_intr26);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr26Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr27Set(unsigned int uhdcp2x_mcu_intr27);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr27Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr28Set(unsigned int uhdcp2x_mcu_intr28);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr28Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr29Set(unsigned int uhdcp2x_mcu_intr29);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr29Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr30Set(unsigned int uhdcp2x_mcu_intr30);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr30Get(HI_VOID);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr31Set(unsigned int uhdcp2x_mcu_intr31);
int HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr31Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask0Set(unsigned int uhdcp2x_mcu_mask0);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask0Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask1Set(unsigned int uhdcp2x_mcu_mask1);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask1Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask2Set(unsigned int uhdcp2x_mcu_mask2);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask2Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask3Set(unsigned int uhdcp2x_mcu_mask3);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask3Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask4Set(unsigned int uhdcp2x_mcu_mask4);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask4Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask5Set(unsigned int uhdcp2x_mcu_mask5);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask5Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask6Set(unsigned int uhdcp2x_mcu_mask6);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask6Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask7Set(unsigned int uhdcp2x_mcu_mask7);
int HDMI_HDCP2X_MCU_MASK_B0_hdcp2x_mcu_mask7Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask8Set(unsigned int uhdcp2x_mcu_mask8);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask8Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask9Set(unsigned int uhdcp2x_mcu_mask9);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask9Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask10Set(unsigned int uhdcp2x_mcu_mask10);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask10Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask11Set(unsigned int uhdcp2x_mcu_mask11);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask11Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask12Set(unsigned int uhdcp2x_mcu_mask12);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask12Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask13Set(unsigned int uhdcp2x_mcu_mask13);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask13Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask14Set(unsigned int uhdcp2x_mcu_mask14);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask14Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask15Set(unsigned int uhdcp2x_mcu_mask15);
int HDMI_HDCP2X_MCU_MASK_B1_hdcp2x_mcu_mask15Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask16Set(unsigned int uhdcp2x_mcu_mask16);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask16Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask17Set(unsigned int uhdcp2x_mcu_mask17);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask17Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask18Set(unsigned int uhdcp2x_mcu_mask18);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask18Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask19Set(unsigned int uhdcp2x_mcu_mask19);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask19Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask20Set(unsigned int uhdcp2x_mcu_mask20);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask20Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask21Set(unsigned int uhdcp2x_mcu_mask21);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask21Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask22Set(unsigned int uhdcp2x_mcu_mask22);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask22Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask23Set(unsigned int uhdcp2x_mcu_mask23);
int HDMI_HDCP2X_MCU_MASK_B2_hdcp2x_mcu_mask23Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask24Set(unsigned int uhdcp2x_mcu_mask24);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask24Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask25Set(unsigned int uhdcp2x_mcu_mask25);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask25Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask26Set(unsigned int uhdcp2x_mcu_mask26);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask26Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask27Set(unsigned int uhdcp2x_mcu_mask27);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask27Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask28Set(unsigned int uhdcp2x_mcu_mask28);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask28Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask29Set(unsigned int uhdcp2x_mcu_mask29);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask29Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask30Set(unsigned int uhdcp2x_mcu_mask30);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask30Get(HI_VOID);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask31Set(unsigned int uhdcp2x_mcu_mask31);
int HDMI_HDCP2X_MCU_MASK_B3_hdcp2x_mcu_mask31Get(HI_VOID);
int HDMI_HDCP2X_MCU_STATE_B0_hdcp2x_mcu_state_b0Set(unsigned int uhdcp2x_mcu_state_b0);
int HDMI_HDCP2X_MCU_STATE_B0_hdcp2x_mcu_state_b0Get(HI_VOID);
int HDMI_HDCP2X_MCU_STATE_B1_hdcp2x_mcu_state_b1Set(unsigned int uhdcp2x_mcu_state_b1);
int HDMI_HDCP2X_MCU_STATE_B1_hdcp2x_mcu_state_b1Get(HI_VOID);
int HDMI_HDCP2X_MCU_STATE_B2_hdcp2x_mcu_state_b2Set(unsigned int uhdcp2x_mcu_state_b2);
int HDMI_HDCP2X_MCU_STATE_B2_hdcp2x_mcu_state_b2Get(HI_VOID);
int HDMI_HDCP2X_MCU_STATE_B3_hdcp2x_mcu_state_b3Set(unsigned int uhdcp2x_mcu_state_b3);
int HDMI_HDCP2X_MCU_STATE_B3_hdcp2x_mcu_state_b3Get(HI_VOID);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_ake_init_doneSet(unsigned int uhdcp2x_ake_init_done);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_ake_init_doneGet(HI_VOID);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_cert_doneSet(unsigned int uhdcp2x_send_cert_done);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_cert_doneGet(HI_VOID);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_km_doneSet(unsigned int uhdcp2x_km_done);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_km_doneGet(HI_VOID);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_h_doneSet(unsigned int uhdcp2x_send_h_done);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_h_doneGet(HI_VOID);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_pairing_doneSet(unsigned int uhdcp2x_pairing_done);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_pairing_doneGet(HI_VOID);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_lc_init_doneSet(unsigned int uhdcp2x_lc_init_done);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_lc_init_doneGet(HI_VOID);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_l_doneSet(unsigned int uhdcp2x_send_l_done);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_l_doneGet(HI_VOID);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_sks_doneSet(unsigned int uhdcp2x_send_sks_done);
int HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_sks_doneGet(HI_VOID);
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_id_list_doneSet(unsigned int uhdcp2x_id_list_done);
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_id_list_doneGet(HI_VOID);
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_send_ack_doneSet(unsigned int uhdcp2x_send_ack_done);
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_send_ack_doneGet(HI_VOID);
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_id_doneSet(unsigned int uhdcp2x_strm_id_done);
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_id_doneGet(HI_VOID);
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_rdy_doneSet(unsigned int uhdcp2x_strm_rdy_done);
int HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_rdy_doneGet(HI_VOID);
int HDMI_HDCP2X_AUTH_STATE0_hdcp2x_auth_state0Set(unsigned int uhdcp2x_auth_state0);
int HDMI_HDCP2X_AUTH_STATE0_hdcp2x_auth_state0Get(HI_VOID);
int HDMI_HDCP2X_AUTH_STATE1_hdcp2x_auth_state1Set(unsigned int uhdcp2x_auth_state1);
int HDMI_HDCP2X_AUTH_STATE1_hdcp2x_auth_state1Get(HI_VOID);
int HDMI_HDCP2X_AUTH_STATE2_hdcp2x_auth_state2Set(unsigned int uhdcp2x_auth_state2);
int HDMI_HDCP2X_AUTH_STATE2_hdcp2x_auth_state2Get(HI_VOID);
int HDMI_HDCP2X_AUTH_STATE3_hdcp2x_auth_state3Set(unsigned int uhdcp2x_auth_state3);
int HDMI_HDCP2X_AUTH_STATE3_hdcp2x_auth_state3Get(HI_VOID);
int HDMI_HDCP2X_CERT_INFO_hdcp2x_cert_rptSet(unsigned int uhdcp2x_cert_rpt);
int HDMI_HDCP2X_CERT_INFO_hdcp2x_cert_rptGet(HI_VOID);
int HDMI_HDCP2X_CERT_VER_hdcp2x_cert_verSet(unsigned int uhdcp2x_cert_ver);
int HDMI_HDCP2X_CERT_VER_hdcp2x_cert_verGet(HI_VOID);
int HDMI_HDCP2X_RPT_INFO_hdcp1x_dev_onSet(unsigned int uhdcp1x_dev_on);
int HDMI_HDCP2X_RPT_INFO_hdcp1x_dev_onGet(HI_VOID);
int HDMI_HDCP2X_RPT_INFO_hdcp20_rpt_onSet(unsigned int uhdcp20_rpt_on);
int HDMI_HDCP2X_RPT_INFO_hdcp20_rpt_onGet(HI_VOID);
int HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_cas_excSet(unsigned int uhdcp2x_rpt_cas_exc);
int HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_cas_excGet(HI_VOID);
int HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_dev_excSet(unsigned int uhdcp2x_rpt_dev_exc);
int HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_dev_excGet(HI_VOID);
int HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dev_cntSet(unsigned int uhdcp2x_rpt_dev_cnt);
int HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dev_cntGet(HI_VOID);
int HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dep_cntSet(unsigned int uhdcp2x_rpt_dep_cnt);
int HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dep_cntGet(HI_VOID);
int HDMI_HDCP2X_RPT_V0_hdcp2x_rpt_v0Set(unsigned int uhdcp2x_rpt_v0);
int HDMI_HDCP2X_RPT_V0_hdcp2x_rpt_v0Get(HI_VOID);
int HDMI_HDCP2X_RPT_V1_hdcp2x_rpt_v1Set(unsigned int uhdcp2x_rpt_v1);
int HDMI_HDCP2X_RPT_V1_hdcp2x_rpt_v1Get(HI_VOID);
int HDMI_HDCP2X_RPT_V2_hdcp2x_rpt_v2Set(unsigned int uhdcp2x_rpt_v2);
int HDMI_HDCP2X_RPT_V2_hdcp2x_rpt_v2Get(HI_VOID);
int HDMI_HDCP2X_RPT_K0_hdcp2x_rpt_k0Set(unsigned int uhdcp2x_rpt_k0);
int HDMI_HDCP2X_RPT_K0_hdcp2x_rpt_k0Get(HI_VOID);
int HDMI_HDCP2X_RPT_K1_hdcp2x_rpt_k1Set(unsigned int uhdcp2x_rpt_k1);
int HDMI_HDCP2X_RPT_K1_hdcp2x_rpt_k1Get(HI_VOID);
int HDMI_HDCP2X_RPT_M0_hdcp2x_rpt_m0Set(unsigned int uhdcp2x_rpt_m0);
int HDMI_HDCP2X_RPT_M0_hdcp2x_rpt_m0Get(HI_VOID);
int HDMI_HDCP2X_RPT_M1_hdcp2x_rpt_m1Set(unsigned int uhdcp2x_rpt_m1);
int HDMI_HDCP2X_RPT_M1_hdcp2x_rpt_m1Get(HI_VOID);
int HDMI_HDCP2X_RPT_M2_hdcp2x_rpt_m2Set(unsigned int uhdcp2x_rpt_m2);
int HDMI_HDCP2X_RPT_M2_hdcp2x_rpt_m2Get(HI_VOID);
int HDMI_HDCP2X_RPT_STRM_ID_hdcp2x_rpt_strm_idSet(unsigned int uhdcp2x_rpt_strm_id);
int HDMI_HDCP2X_RPT_STRM_ID_hdcp2x_rpt_strm_idGet(HI_VOID);
int HDMI_HDCP2X_RPT_STRM_TPYE_hdcp2x_rpt_strm_typeSet(unsigned int uhdcp2x_rpt_strm_type);
int HDMI_HDCP2X_RPT_STRM_TPYE_hdcp2x_rpt_strm_typeGet(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG0_hdcp2x_timing_cfg0Set(unsigned int uhdcp2x_timing_cfg0);
int HDMI_HDCP2X_TIMING_CFG0_hdcp2x_timing_cfg0Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG1_hdcp2x_timing_cfg1Set(unsigned int uhdcp2x_timing_cfg1);
int HDMI_HDCP2X_TIMING_CFG1_hdcp2x_timing_cfg1Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG2_hdcp2x_timing_cfg2Set(unsigned int uhdcp2x_timing_cfg2);
int HDMI_HDCP2X_TIMING_CFG2_hdcp2x_timing_cfg2Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG3_hdcp2x_timing_cfg3Set(unsigned int uhdcp2x_timing_cfg3);
int HDMI_HDCP2X_TIMING_CFG3_hdcp2x_timing_cfg3Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG4_hdcp2x_timing_cfg4Set(unsigned int uhdcp2x_timing_cfg4);
int HDMI_HDCP2X_TIMING_CFG4_hdcp2x_timing_cfg4Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG5_hdcp2x_timing_cfg5Set(unsigned int uhdcp2x_timing_cfg5);
int HDMI_HDCP2X_TIMING_CFG5_hdcp2x_timing_cfg5Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG6_hdcp2x_timing_cfg6Set(unsigned int uhdcp2x_timing_cfg6);
int HDMI_HDCP2X_TIMING_CFG6_hdcp2x_timing_cfg6Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG7_hdcp2x_timing_cfg7Set(unsigned int uhdcp2x_timing_cfg7);
int HDMI_HDCP2X_TIMING_CFG7_hdcp2x_timing_cfg7Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG8_hdcp2x_timing_cfg8Set(unsigned int uhdcp2x_timing_cfg8);
int HDMI_HDCP2X_TIMING_CFG8_hdcp2x_timing_cfg8Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG9_hdcp2x_timing_cfg9Set(unsigned int uhdcp2x_timing_cfg9);
int HDMI_HDCP2X_TIMING_CFG9_hdcp2x_timing_cfg9Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG10_hdcp2x_timing_cfg10Set(unsigned int uhdcp2x_timing_cfg10);
int HDMI_HDCP2X_TIMING_CFG10_hdcp2x_timing_cfg10Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG11_hdcp2x_timing_cfg11Set(unsigned int uhdcp2x_timing_cfg11);
int HDMI_HDCP2X_TIMING_CFG11_hdcp2x_timing_cfg11Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG12_hdcp2x_timing_cfg12Set(unsigned int uhdcp2x_timing_cfg12);
int HDMI_HDCP2X_TIMING_CFG12_hdcp2x_timing_cfg12Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG13_hdcp2x_timing_cfg13Set(unsigned int uhdcp2x_timing_cfg13);
int HDMI_HDCP2X_TIMING_CFG13_hdcp2x_timing_cfg13Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG14_hdcp2x_timing_cfg14Set(unsigned int uhdcp2x_timing_cfg14);
int HDMI_HDCP2X_TIMING_CFG14_hdcp2x_timing_cfg14Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG15_hdcp2x_timing_cfg15Set(unsigned int uhdcp2x_timing_cfg15);
int HDMI_HDCP2X_TIMING_CFG15_hdcp2x_timing_cfg15Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG16_hdcp2x_timing_cfg16Set(unsigned int uhdcp2x_timing_cfg16);
int HDMI_HDCP2X_TIMING_CFG16_hdcp2x_timing_cfg16Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG17_hdcp2x_timing_cfg17Set(unsigned int uhdcp2x_timing_cfg17);
int HDMI_HDCP2X_TIMING_CFG17_hdcp2x_timing_cfg17Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG18_hdcp2x_timing_cfg18Set(unsigned int uhdcp2x_timing_cfg18);
int HDMI_HDCP2X_TIMING_CFG18_hdcp2x_timing_cfg18Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG19_hdcp2x_timing_cfg19Set(unsigned int uhdcp2x_timing_cfg19);
int HDMI_HDCP2X_TIMING_CFG19_hdcp2x_timing_cfg19Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG20_hdcp2x_timing_cfg20Set(unsigned int uhdcp2x_timing_cfg20);
int HDMI_HDCP2X_TIMING_CFG20_hdcp2x_timing_cfg20Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG21_hdcp2x_timing_cfg21Set(unsigned int uhdcp2x_timing_cfg21);
int HDMI_HDCP2X_TIMING_CFG21_hdcp2x_timing_cfg21Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG22_hdcp2x_timing_cfg22Set(unsigned int uhdcp2x_timing_cfg22);
int HDMI_HDCP2X_TIMING_CFG22_hdcp2x_timing_cfg22Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG23_hdcp2x_timing_cfg23Set(unsigned int uhdcp2x_timing_cfg23);
int HDMI_HDCP2X_TIMING_CFG23_hdcp2x_timing_cfg23Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG24_hdcp2x_timing_cfg24Set(unsigned int uhdcp2x_timing_cfg24);
int HDMI_HDCP2X_TIMING_CFG24_hdcp2x_timing_cfg24Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG25_hdcp2x_timing_cfg25Set(unsigned int uhdcp2x_timing_cfg25);
int HDMI_HDCP2X_TIMING_CFG25_hdcp2x_timing_cfg25Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG26_hdcp2x_timing_cfg26Set(unsigned int uhdcp2x_timing_cfg26);
int HDMI_HDCP2X_TIMING_CFG26_hdcp2x_timing_cfg26Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG27_hdcp2x_timing_cfg27Set(unsigned int uhdcp2x_timing_cfg27);
int HDMI_HDCP2X_TIMING_CFG27_hdcp2x_timing_cfg27Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG28_hdcp2x_timing_cfg28Set(unsigned int uhdcp2x_timing_cfg28);
int HDMI_HDCP2X_TIMING_CFG28_hdcp2x_timing_cfg28Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG29_hdcp2x_timing_cfg29Set(unsigned int uhdcp2x_timing_cfg29);
int HDMI_HDCP2X_TIMING_CFG29_hdcp2x_timing_cfg29Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG30_hdcp2x_timing_cfg30Set(unsigned int uhdcp2x_timing_cfg30);
int HDMI_HDCP2X_TIMING_CFG30_hdcp2x_timing_cfg30Get(HI_VOID);
int HDMI_HDCP2X_TIMING_CFG31_hdcp2x_timing_cfg31Set(unsigned int uhdcp2x_timing_cfg31);
int HDMI_HDCP2X_TIMING_CFG31_hdcp2x_timing_cfg31Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN0_hdcp2x_gen_in0Set(unsigned int uhdcp2x_gen_in0);
int HDMI_HDCP2X_GEN_IN0_hdcp2x_gen_in0Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN1_hdcp2x_gen_in1Set(unsigned int uhdcp2x_gen_in1);
int HDMI_HDCP2X_GEN_IN1_hdcp2x_gen_in1Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN2_hdcp2x_gen_in2Set(unsigned int uhdcp2x_gen_in2);
int HDMI_HDCP2X_GEN_IN2_hdcp2x_gen_in2Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN3_hdcp2x_gen_in3Set(unsigned int uhdcp2x_gen_in3);
int HDMI_HDCP2X_GEN_IN3_hdcp2x_gen_in3Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN4_hdcp2x_gen_in4Set(unsigned int uhdcp2x_gen_in4);
int HDMI_HDCP2X_GEN_IN4_hdcp2x_gen_in4Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN5_hdcp2x_gen_in5Set(unsigned int uhdcp2x_gen_in5);
int HDMI_HDCP2X_GEN_IN5_hdcp2x_gen_in5Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN6_hdcp2x_gen_in6Set(unsigned int uhdcp2x_gen_in6);
int HDMI_HDCP2X_GEN_IN6_hdcp2x_gen_in6Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN7_hdcp2x_gen_in7Set(unsigned int uhdcp2x_gen_in7);
int HDMI_HDCP2X_GEN_IN7_hdcp2x_gen_in7Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN8_hdcp2x_gen_in8Set(unsigned int uhdcp2x_gen_in8);
int HDMI_HDCP2X_GEN_IN8_hdcp2x_gen_in8Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN9_hdcp2x_gen_in9Set(unsigned int uhdcp2x_gen_in9);
int HDMI_HDCP2X_GEN_IN9_hdcp2x_gen_in9Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN10_hdcp2x_gen_in10Set(unsigned int uhdcp2x_gen_in10);
int HDMI_HDCP2X_GEN_IN10_hdcp2x_gen_in10Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN11_hdcp2x_gen_in11Set(unsigned int uhdcp2x_gen_in11);
int HDMI_HDCP2X_GEN_IN11_hdcp2x_gen_in11Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN12_hdcp2x_gen_in12Set(unsigned int uhdcp2x_gen_in12);
int HDMI_HDCP2X_GEN_IN12_hdcp2x_gen_in12Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN13_hdcp2x_gen_in13Set(unsigned int uhdcp2x_gen_in13);
int HDMI_HDCP2X_GEN_IN13_hdcp2x_gen_in13Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN14_hdcp2x_gen_in14Set(unsigned int uhdcp2x_gen_in14);
int HDMI_HDCP2X_GEN_IN14_hdcp2x_gen_in14Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN15_hdcp2x_gen_in15Set(unsigned int uhdcp2x_gen_in15);
int HDMI_HDCP2X_GEN_IN15_hdcp2x_gen_in15Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN16_hdcp2x_gen_in16Set(unsigned int uhdcp2x_gen_in16);
int HDMI_HDCP2X_GEN_IN16_hdcp2x_gen_in16Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN17_hdcp2x_gen_in17Set(unsigned int uhdcp2x_gen_in17);
int HDMI_HDCP2X_GEN_IN17_hdcp2x_gen_in17Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN18_hdcp2x_gen_in18Set(unsigned int uhdcp2x_gen_in18);
int HDMI_HDCP2X_GEN_IN18_hdcp2x_gen_in18Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN19_hdcp2x_gen_in19Set(unsigned int uhdcp2x_gen_in19);
int HDMI_HDCP2X_GEN_IN19_hdcp2x_gen_in19Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN20_hdcp2x_gen_in20Set(unsigned int uhdcp2x_gen_in20);
int HDMI_HDCP2X_GEN_IN20_hdcp2x_gen_in20Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN21_hdcp2x_gen_in21Set(unsigned int uhdcp2x_gen_in21);
int HDMI_HDCP2X_GEN_IN21_hdcp2x_gen_in21Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN22_hdcp2x_gen_in22Set(unsigned int uhdcp2x_gen_in22);
int HDMI_HDCP2X_GEN_IN22_hdcp2x_gen_in22Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN23_hdcp2x_gen_in23Set(unsigned int uhdcp2x_gen_in23);
int HDMI_HDCP2X_GEN_IN23_hdcp2x_gen_in23Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN24_hdcp2x_gen_in24Set(unsigned int uhdcp2x_gen_in24);
int HDMI_HDCP2X_GEN_IN24_hdcp2x_gen_in24Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN25_hdcp2x_gen_in25Set(unsigned int uhdcp2x_gen_in25);
int HDMI_HDCP2X_GEN_IN25_hdcp2x_gen_in25Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN26_hdcp2x_gen_in26Set(unsigned int uhdcp2x_gen_in26);
int HDMI_HDCP2X_GEN_IN26_hdcp2x_gen_in26Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN27_hdcp2x_gen_in27Set(unsigned int uhdcp2x_gen_in27);
int HDMI_HDCP2X_GEN_IN27_hdcp2x_gen_in27Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN28_hdcp2x_gen_in28Set(unsigned int uhdcp2x_gen_in28);
int HDMI_HDCP2X_GEN_IN28_hdcp2x_gen_in28Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN29_hdcp2x_gen_in29Set(unsigned int uhdcp2x_gen_in29);
int HDMI_HDCP2X_GEN_IN29_hdcp2x_gen_in29Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN30_hdcp2x_gen_in30Set(unsigned int uhdcp2x_gen_in30);
int HDMI_HDCP2X_GEN_IN30_hdcp2x_gen_in30Get(HI_VOID);
int HDMI_HDCP2X_GEN_IN31_hdcp2x_gen_in31Set(unsigned int uhdcp2x_gen_in31);
int HDMI_HDCP2X_GEN_IN31_hdcp2x_gen_in31Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT0_hdcp2x_gen_out0Set(unsigned int uhdcp2x_gen_out0);
int HDMI_HDCP2X_GEN_OUT0_hdcp2x_gen_out0Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT1_hdcp2x_gen_out1Set(unsigned int uhdcp2x_gen_out1);
int HDMI_HDCP2X_GEN_OUT1_hdcp2x_gen_out1Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT2_hdcp2x_gen_out2Set(unsigned int uhdcp2x_gen_out2);
int HDMI_HDCP2X_GEN_OUT2_hdcp2x_gen_out2Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT3_hdcp2x_gen_out3Set(unsigned int uhdcp2x_gen_out3);
int HDMI_HDCP2X_GEN_OUT3_hdcp2x_gen_out3Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT4_hdcp2x_gen_out4Set(unsigned int uhdcp2x_gen_out4);
int HDMI_HDCP2X_GEN_OUT4_hdcp2x_gen_out4Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT5_hdcp2x_gen_out5Set(unsigned int uhdcp2x_gen_out5);
int HDMI_HDCP2X_GEN_OUT5_hdcp2x_gen_out5Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT6_hdcp2x_gen_out6Set(unsigned int uhdcp2x_gen_out6);
int HDMI_HDCP2X_GEN_OUT6_hdcp2x_gen_out6Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT7_hdcp2x_gen_out7Set(unsigned int uhdcp2x_gen_out7);
int HDMI_HDCP2X_GEN_OUT7_hdcp2x_gen_out7Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT8_hdcp2x_gen_out8Set(unsigned int uhdcp2x_gen_out8);
int HDMI_HDCP2X_GEN_OUT8_hdcp2x_gen_out8Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT9_hdcp2x_gen_out9Set(unsigned int uhdcp2x_gen_out9);
int HDMI_HDCP2X_GEN_OUT9_hdcp2x_gen_out9Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT10_hdcp2x_gen_out10Set(unsigned int uhdcp2x_gen_out10);
int HDMI_HDCP2X_GEN_OUT10_hdcp2x_gen_out10Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT11_hdcp2x_gen_out11Set(unsigned int uhdcp2x_gen_out11);
int HDMI_HDCP2X_GEN_OUT11_hdcp2x_gen_out11Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT12_hdcp2x_gen_out12Set(unsigned int uhdcp2x_gen_out12);
int HDMI_HDCP2X_GEN_OUT12_hdcp2x_gen_out12Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT13_hdcp2x_gen_out13Set(unsigned int uhdcp2x_gen_out13);
int HDMI_HDCP2X_GEN_OUT13_hdcp2x_gen_out13Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT14_hdcp2x_gen_out14Set(unsigned int uhdcp2x_gen_out14);
int HDMI_HDCP2X_GEN_OUT14_hdcp2x_gen_out14Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT15_hdcp2x_gen_out15Set(unsigned int uhdcp2x_gen_out15);
int HDMI_HDCP2X_GEN_OUT15_hdcp2x_gen_out15Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT16_hdcp2x_gen_out16Set(unsigned int uhdcp2x_gen_out16);
int HDMI_HDCP2X_GEN_OUT16_hdcp2x_gen_out16Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT17_hdcp2x_gen_out17Set(unsigned int uhdcp2x_gen_out17);
int HDMI_HDCP2X_GEN_OUT17_hdcp2x_gen_out17Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT18_hdcp2x_gen_out18Set(unsigned int uhdcp2x_gen_out18);
int HDMI_HDCP2X_GEN_OUT18_hdcp2x_gen_out18Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT19_hdcp2x_gen_out19Set(unsigned int uhdcp2x_gen_out19);
int HDMI_HDCP2X_GEN_OUT19_hdcp2x_gen_out19Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT20_hdcp2x_gen_out20Set(unsigned int uhdcp2x_gen_out20);
int HDMI_HDCP2X_GEN_OUT20_hdcp2x_gen_out20Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT21_hdcp2x_gen_out21Set(unsigned int uhdcp2x_gen_out21);
int HDMI_HDCP2X_GEN_OUT21_hdcp2x_gen_out21Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT22_hdcp2x_gen_out22Set(unsigned int uhdcp2x_gen_out22);
int HDMI_HDCP2X_GEN_OUT22_hdcp2x_gen_out22Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT23_hdcp2x_gen_out23Set(unsigned int uhdcp2x_gen_out23);
int HDMI_HDCP2X_GEN_OUT23_hdcp2x_gen_out23Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT24_hdcp2x_gen_out24Set(unsigned int uhdcp2x_gen_out24);
int HDMI_HDCP2X_GEN_OUT24_hdcp2x_gen_out24Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT25_hdcp2x_gen_out25Set(unsigned int uhdcp2x_gen_out25);
int HDMI_HDCP2X_GEN_OUT25_hdcp2x_gen_out25Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT26_hdcp2x_gen_out26Set(unsigned int uhdcp2x_gen_out26);
int HDMI_HDCP2X_GEN_OUT26_hdcp2x_gen_out26Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT27_hdcp2x_gen_out27Set(unsigned int uhdcp2x_gen_out27);
int HDMI_HDCP2X_GEN_OUT27_hdcp2x_gen_out27Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT28_hdcp2x_gen_out28Set(unsigned int uhdcp2x_gen_out28);
int HDMI_HDCP2X_GEN_OUT28_hdcp2x_gen_out28Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT29_hdcp2x_gen_out29Set(unsigned int uhdcp2x_gen_out29);
int HDMI_HDCP2X_GEN_OUT29_hdcp2x_gen_out29Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT30_hdcp2x_gen_out30Set(unsigned int uhdcp2x_gen_out30);
int HDMI_HDCP2X_GEN_OUT30_hdcp2x_gen_out30Get(HI_VOID);
int HDMI_HDCP2X_GEN_OUT31_hdcp2x_gen_out31Set(unsigned int uhdcp2x_gen_out31);
int HDMI_HDCP2X_GEN_OUT31_hdcp2x_gen_out31Get(HI_VOID);
int HDMI_HDCP2X_SEC_IN0_hdcp2x_sec_in0Set(unsigned int uhdcp2x_sec_in0);
int HDMI_HDCP2X_SEC_IN0_hdcp2x_sec_in0Get(HI_VOID);
int HDMI_HDCP2X_SEC_IN1_hdcp2x_sec_in1Set(unsigned int uhdcp2x_sec_in1);
int HDMI_HDCP2X_SEC_IN1_hdcp2x_sec_in1Get(HI_VOID);
int HDMI_HDCP2X_SEC_IN2_hdcp2x_sec_in2Set(unsigned int uhdcp2x_sec_in2);
int HDMI_HDCP2X_SEC_IN2_hdcp2x_sec_in2Get(HI_VOID);
int HDMI_HDCP2X_SEC_IN3_hdcp2x_sec_in3Set(unsigned int uhdcp2x_sec_in3);
int HDMI_HDCP2X_SEC_IN3_hdcp2x_sec_in3Get(HI_VOID);
int HDMI_HDCP2X_SEC_IN4_hdcp2x_sec_in4Set(unsigned int uhdcp2x_sec_in4);
int HDMI_HDCP2X_SEC_IN4_hdcp2x_sec_in4Get(HI_VOID);
int HDMI_HDCP2X_SEC_IN5_hdcp2x_sec_in5Set(unsigned int uhdcp2x_sec_in5);
int HDMI_HDCP2X_SEC_IN5_hdcp2x_sec_in5Get(HI_VOID);
int HDMI_HDCP2X_SEC_IN6_hdcp2x_sec_in6Set(unsigned int uhdcp2x_sec_in6);
int HDMI_HDCP2X_SEC_IN6_hdcp2x_sec_in6Get(HI_VOID);
int HDMI_HDCP2X_SEC_IN7_hdcp2x_sec_in7Set(unsigned int uhdcp2x_sec_in7);
int HDMI_HDCP2X_SEC_IN7_hdcp2x_sec_in7Get(HI_VOID);
int HDMI_HDCP2X_SEC_OUT0_hdcp2x_sec_out0Set(unsigned int uhdcp2x_sec_out0);
int HDMI_HDCP2X_SEC_OUT0_hdcp2x_sec_out0Get(HI_VOID);
int HDMI_HDCP2X_SEC_OUT1_hdcp2x_sec_out1Set(unsigned int uhdcp2x_sec_out1);
int HDMI_HDCP2X_SEC_OUT1_hdcp2x_sec_out1Get(HI_VOID);
int HDMI_HDCP2X_SEC_OUT2_hdcp2x_sec_out2Set(unsigned int uhdcp2x_sec_out2);
int HDMI_HDCP2X_SEC_OUT2_hdcp2x_sec_out2Get(HI_VOID);
int HDMI_HDCP2X_SEC_OUT3_hdcp2x_sec_out3Set(unsigned int uhdcp2x_sec_out3);
int HDMI_HDCP2X_SEC_OUT3_hdcp2x_sec_out3Get(HI_VOID);
int HDMI_HDCP2X_SEC_OUT4_hdcp2x_sec_out4Set(unsigned int uhdcp2x_sec_out4);
int HDMI_HDCP2X_SEC_OUT4_hdcp2x_sec_out4Get(HI_VOID);
int HDMI_HDCP2X_SEC_OUT5_hdcp2x_sec_out5Set(unsigned int uhdcp2x_sec_out5);
int HDMI_HDCP2X_SEC_OUT5_hdcp2x_sec_out5Get(HI_VOID);
int HDMI_HDCP2X_SEC_OUT6_hdcp2x_sec_out6Set(unsigned int uhdcp2x_sec_out6);
int HDMI_HDCP2X_SEC_OUT6_hdcp2x_sec_out6Get(HI_VOID);
int HDMI_HDCP2X_SEC_OUT7_hdcp2x_sec_out7Set(unsigned int uhdcp2x_sec_out7);
int HDMI_HDCP2X_SEC_OUT7_hdcp2x_sec_out7Get(HI_VOID);

#endif // __MCU_CPU_REG_C_UNION_DEFINE_H__
