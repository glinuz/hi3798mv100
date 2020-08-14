// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  tx_aon_reg_c_union_define.h
// Project line  :  Platform And Key Technologies Development
// Department    :  Device Chipset Source Algorithm and Chipset Platform Development Dept
// Author        :  w00226427
// Version       :  1.0
// Date          :  2015/1/1
// Description   :  HDMIMHL_TX_HISI
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00226427 2015/10/08 20:03:04 Create file
// ******************************************************************************

#ifndef __TX_AON_REG_C_UNION_DEFINE_H__
#define __TX_AON_REG_C_UNION_DEFINE_H__

/* Define the union U_TX_HW_INFO */
/* Define the union U_TX_HW_INFO,ATTR:,ADDR:4000,INIT:07DF0912,MASK:ffffffff,CMMNT:HDMI¿ØÖÆÆ÷Âß¼­°æ±¾¸üÐÂÊ±¼ä¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_hw_day   : 8  ; /* [7:0] */
        unsigned int    tx_hw_month : 8  ; /* [15:8] */
        unsigned int    tx_hw_year  : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_HW_INFO;

/* Define the union U_TX_HW_VERSION */
/* Define the union U_TX_HW_VERSION,ATTR:,ADDR:4004,INIT:00101010,MASK:ffffff,CMMNT:HDMI¿ØÖÆÆ÷Âß¼­°æ±¾ÐÅÏ¢¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_reg_version : 8  ; /* [7:0] */
        unsigned int    tx_drv_version : 8  ; /* [15:8] */
        unsigned int    tx_rtl_version : 8  ; /* [23:16] */
        unsigned int    rsv_0          : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_HW_VERSION;

/* Define the union U_TX_HW_FEATURE */
/* Define the union U_TX_HW_FEATURE,ATTR:,ADDR:4008,INIT:00000000,MASK:0,CMMNT:HDMI¿ØÖÆÆ÷Âß¼­ÌØÐÔÐÅÏ¢¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rsv_1 : 32  ; /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_HW_FEATURE;

/* Define the union U_TX_AON_RST_CTRL */
/* Define the union U_TX_AON_RST_CTRL,ATTR:,ADDR:401C,INIT:00000000,MASK:1,CMMNT:±£Áô¼Ä´æÆ÷*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_aon_soft_arst_req : 1  ; /* [0] */
        unsigned int    rsv_2                : 31  ; /* [31:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_AON_RST_CTRL;

/* Define the union U_HOTPLUG_ST_CFG */
/* Define the union U_HOTPLUG_ST_CFG,ATTR:,ADDR:4020,INIT:00007CF1,MASK:ffff,CMMNT:HOTPLUG ×´Ì¬ÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpd_fillter_en   : 1  ; /* [0] */
        unsigned int    hpd_override_en  : 1  ; /* [1] */
        unsigned int    hpd_polarity_ctl : 1  ; /* [2] */
        unsigned int    hpd_soft_value   : 1  ; /* [3] */
        unsigned int    osc_div_cnt      : 12  ; /* [15:4] */
        unsigned int    rsv_3            : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HOTPLUG_ST_CFG;

/* Define the union U_HOTPLUG_FILLTER_CFG */
/* Define the union U_HOTPLUG_FILLTER_CFG,ATTR:,ADDR:4024,INIT:00090063,MASK:ffffffff,CMMNT:HOTPLUG Ã«´Ì¹ýÂËÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpd_high_reshold : 16  ; /* [15:0] */
        unsigned int    hpd_low_reshold  : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HOTPLUG_FILLTER_CFG;

/* Define the union U_TX_AON_STATE */
/* Define the union U_TX_AON_STATE,ATTR:,ADDR:4028,INIT:00000000,MASK:3,CMMNT:TX AON Ä£¿é×´Ì¬Ö¸Ê¾¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hotplug_state : 1  ; /* [0] */
        unsigned int    phy_rx_sense  : 1  ; /* [1] */
        unsigned int    rsv_4         : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_AON_STATE;

/* Define the union U_TX_AON_INTR_MASK */
/* Define the union U_TX_AON_INTR_MASK,ATTR:,ADDR:4030,INIT:00000000,MASK:3ff3,CMMNT:TX AON Ä£¿éÖÐ¶ÏÆÁ±Î¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aon_intr_mask0  : 1  ; /* [0] */
        unsigned int    aon_intr_mask1  : 1  ; /* [1] */
        unsigned int    rsv_5           : 1  ; /* [2] */
        unsigned int    rsv_6           : 1  ; /* [3] */
        unsigned int    aon_intr_mask4  : 1  ; /* [4] */
        unsigned int    aon_intr_mask5  : 1  ; /* [5] */
        unsigned int    aon_intr_mask6  : 1  ; /* [6] */
        unsigned int    aon_intr_mask7  : 1  ; /* [7] */
        unsigned int    aon_intr_mask8  : 1  ; /* [8] */
        unsigned int    aon_intr_mask9  : 1  ; /* [9] */
        unsigned int    aon_intr_mask10 : 1  ; /* [10] */
        unsigned int    aon_intr_mask11 : 1  ; /* [11] */
        unsigned int    aon_intr_mask12 : 1  ; /* [12] */
        unsigned int    aon_intr_mask13 : 1  ; /* [13] */
        unsigned int    rsv_7           : 18  ; /* [31:14] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_AON_INTR_MASK;

/* Define the union U_TX_AON_INTR_STATE */
/* Define the union U_TX_AON_INTR_STATE,ATTR:,ADDR:4034,INIT:00000000,MASK:3ff3,CMMNT:TX AON Ä£¿éÖÐ¶Ï×´Ì¬¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aon_intr_stat0  : 1  ; /* [0] */
        unsigned int    aon_intr_stat1  : 1  ; /* [1] */
        unsigned int    rsv_8           : 1  ; /* [2] */
        unsigned int    rsv_9           : 1  ; /* [3] */
        unsigned int    aon_intr_stat4  : 1  ; /* [4] */
        unsigned int    aon_intr_stat5  : 1  ; /* [5] */
        unsigned int    aon_intr_stat6  : 1  ; /* [6] */
        unsigned int    aon_intr_stat7  : 1  ; /* [7] */
        unsigned int    aon_intr_stat8  : 1  ; /* [8] */
        unsigned int    aon_intr_stat9  : 1  ; /* [9] */
        unsigned int    aon_intr_stat10 : 1  ; /* [10] */
        unsigned int    aon_intr_stat11 : 1  ; /* [11] */
        unsigned int    aon_intr_stat12 : 1  ; /* [12] */
        unsigned int    aon_intr_stat13 : 1  ; /* [13] */
        unsigned int    rsv_10          : 18  ; /* [31:14] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_AON_INTR_STATE;

/* Define the union U_DDC_MST_CTRL */
/* Define the union U_DDC_MST_CTRL,ATTR:,ADDR:4040,INIT:000004A0,MASK:1ff3,CMMNT:DDC MasterÄ£¿éÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddc_aon_access : 1  ; /* [0] */
        unsigned int    dcc_man_en     : 1  ; /* [1] */
        unsigned int    rsv_11         : 2  ; /* [3:2] */
        unsigned int    ddc_speed_cnt  : 9  ; /* [12:4] */
        unsigned int    rsv_12         : 19  ; /* [31:13] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_MST_CTRL;

/* Define the union U_DDC_FIFO_RDATA */
/* Define the union U_DDC_FIFO_RDATA,ATTR:,ADDR:4044,INIT:00000000,MASK:ff,CMMNT:DDC Master FIFO ¶ÁÊý¾Ý¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddc_fifo_data_out : 8  ; /* [7:0] */
        unsigned int    rsv_13            : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_FIFO_RDATA;

/* Define the union U_DDC_FIFO_WDATA */
/* Define the union U_DDC_FIFO_WDATA,ATTR:,ADDR:4048,INIT:00000000,MASK:ff,CMMNT:DDC Master FIFO Ð´Êý¾Ý¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddc_fifo_data_in : 8  ; /* [7:0] */
        unsigned int    rsv_14           : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_FIFO_WDATA;

/* Define the union U_DDC_DATA_CNT */
/* Define the union U_DDC_DATA_CNT,ATTR:,ADDR:404C,INIT:00000000,MASK:3ff1f,CMMNT:DDC Master FIFOÊý¾Ý¼ÆÊý¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddc_fifo_data_cnt : 5  ; /* [4:0] */
        unsigned int    rsv_15            : 3  ; /* [7:5] */
        unsigned int    ddc_data_out_cnt  : 10  ; /* [17:8] */
        unsigned int    rsv_16            : 14  ; /* [31:18] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_DATA_CNT;

/* Define the union U_DDC_SLAVE_CFG */
/* Define the union U_DDC_SLAVE_CFG,ATTR:,ADDR:4050,INIT:00000000,MASK:ffffff,CMMNT:DDC ·ÃÎÊSlaveÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddc_slave_addr   : 8  ; /* [7:0] */
        unsigned int    ddc_slave_offset : 8  ; /* [15:8] */
        unsigned int    ddc_slave_seg    : 8  ; /* [23:16] */
        unsigned int    rsv_17           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_SLAVE_CFG;

/* Define the union U_DDC_MST_STATE */
/* Define the union U_DDC_MST_STATE,ATTR:,ADDR:4054,INIT:00000020,MASK:ff,CMMNT:DDC Master ×´Ì¬Ö¸Ê¾¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddc_i2c_no_ack     : 1  ; /* [0] */
        unsigned int    ddc_i2c_bus_low    : 1  ; /* [1] */
        unsigned int    ddc_i2c_in_prog    : 1  ; /* [2] */
        unsigned int    ddc_fifo_wr_in_use : 1  ; /* [3] */
        unsigned int    ddc_fifo_rd_in_use : 1  ; /* [4] */
        unsigned int    ddc_fifo_empty     : 1  ; /* [5] */
        unsigned int    ddc_fifo_half_full : 1  ; /* [6] */
        unsigned int    ddc_fifo_full      : 1  ; /* [7] */
        unsigned int    rsv_18             : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_MST_STATE;

/* Define the union U_DDC_MST_CMD */
/* Define the union U_DDC_MST_CMD,ATTR:,ADDR:4058,INIT:00000000,MASK:f,CMMNT:DDC Master Ö¸ÁîÅäÖÃ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddc_mst_cmd : 4  ; /* [3:0] */
        unsigned int    rsv_19      : 28  ; /* [31:4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_MST_CMD;

/* Define the union U_DDC_MAN_CTRL */
/* Define the union U_DDC_MAN_CTRL,ATTR:,ADDR:405C,INIT:0000000C,MASK:f,CMMNT:DDC Í¨µÀÈí¼þÄ£Äâ¿ØÖÆ¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddc_scl_st  : 1  ; /* [0] */
        unsigned int    ddc_sda_st  : 1  ; /* [1] */
        unsigned int    ddc_scl_oen : 1  ; /* [2] */
        unsigned int    ddc_sda_oen : 1  ; /* [3] */
        unsigned int    rsv_20      : 28  ; /* [31:4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_MAN_CTRL;

/* Define the union U_DDC_STATE_CLR */
/* Define the union U_DDC_STATE_CLR,ATTR:,ADDR:4060,INIT:00000000,MASK:3,CMMNT:DDC Master ×´Ì¬Çå³ý¼Ä´æÆ÷¡£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddc_clr_bus_low : 1  ; /* [0] */
        unsigned int    ddc_clr_no_ack  : 1  ; /* [1] */
        unsigned int    rsv_21          : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DDC_STATE_CLR;


//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_TX_HW_INFO          TX_HW_INFO          ; /* 4000 */
    volatile U_TX_HW_VERSION       TX_HW_VERSION       ; /* 4004 */
    volatile U_TX_HW_FEATURE       TX_HW_FEATURE       ; /* 4008 */
	unsigned int                             Reserved_0[4];	  /* 400C-4018*/
    volatile U_TX_AON_RST_CTRL     TX_AON_RST_CTRL     ; /* 401C */	
    volatile U_HOTPLUG_ST_CFG      HOTPLUG_ST_CFG      ; /* 4020 */
    volatile U_HOTPLUG_FILLTER_CFG HOTPLUG_FILLTER_CFG ; /* 4024 */
    volatile U_TX_AON_STATE        TX_AON_STATE        ; /* 4028 */
	unsigned int                             Reserved_2[1];	  /* 402C*/		
    volatile U_TX_AON_INTR_MASK    TX_AON_INTR_MASK    ; /* 4030 */
    volatile U_TX_AON_INTR_STATE   TX_AON_INTR_STATE   ; /* 4034 */
	unsigned int                             Reserved_3[2];	  /* 4038-403C*/		
    volatile U_DDC_MST_CTRL        DDC_MST_CTRL        ; /* 4040 */
    volatile U_DDC_FIFO_RDATA      DDC_FIFO_RDATA      ; /* 4044 */
    volatile U_DDC_FIFO_WDATA      DDC_FIFO_WDATA      ; /* 4048 */
    volatile U_DDC_DATA_CNT        DDC_DATA_CNT        ; /* 404C */
    volatile U_DDC_SLAVE_CFG       DDC_SLAVE_CFG       ; /* 4050 */
    volatile U_DDC_MST_STATE       DDC_MST_STATE       ; /* 4054 */
    volatile U_DDC_MST_CMD         DDC_MST_CMD         ; /* 4058 */
    volatile U_DDC_MAN_CTRL        DDC_MAN_CTRL        ; /* 405C */
    volatile U_DDC_STATE_CLR       DDC_STATE_CLR       ; /* 4060 */

} S_tx_aon_reg_REGS_TYPE;

/* Declare the struct pointor of the module tx_aon_reg */
//extern volatile S_tx_aon_reg_REGS_TYPE *goptx_aon_regAllReg;

int HDMI_TX_S_tx_aon_reg_REGS_TYPE_Init(void);
int HDMI_TX_S_tx_aon_reg_REGS_TYPE_DeInit(void);


/* Declare the functions that set the member value */
int HDMI_TX_HW_INFO_tx_hw_daySet(unsigned int utx_hw_day);
int HDMI_TX_HW_INFO_tx_hw_monthSet(unsigned int utx_hw_month);
int HDMI_TX_HW_INFO_tx_hw_yearSet(unsigned int utx_hw_year);
int HDMI_TX_HW_VERSION_tx_reg_versionSet(unsigned int utx_reg_version);
int HDMI_TX_HW_VERSION_tx_drv_versionSet(unsigned int utx_drv_version);
int HDMI_TX_HW_VERSION_tx_rtl_versionSet(unsigned int utx_rtl_version);

int HDMI_TX_AON_RST_CTRL_tx_aon_soft_arst_reqSet(unsigned int utx_aon_soft_arst_req);
int HDMI_HOTPLUG_ST_CFG_hpd_fillter_enSet(unsigned int uhpd_fillter_en);
int HDMI_HOTPLUG_ST_CFG_hpd_override_enSet(unsigned int uhpd_override_en);
int HDMI_HOTPLUG_ST_CFG_hpd_polarity_ctlSet(unsigned int uhpd_polarity_ctl);
int HDMI_HOTPLUG_ST_CFG_hpd_soft_valueSet(unsigned int uhpd_soft_value);
int HDMI_HOTPLUG_ST_CFG_osc_div_cntSet(unsigned int uosc_div_cnt);
int HDMI_HOTPLUG_FILLTER_CFG_hpd_high_resholdSet(unsigned int uhpd_high_reshold);
int HDMI_HOTPLUG_FILLTER_CFG_hpd_low_resholdSet(unsigned int uhpd_low_reshold);
int HDMI_TX_AON_STATE_hotplug_stateSet(unsigned int uhotplug_state);
int HDMI_TX_AON_STATE_phy_rx_senseSet(unsigned int uphy_rx_sense);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask0Set(unsigned int uaon_intr_mask0);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask1Set(unsigned int uaon_intr_mask1);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask4Set(unsigned int uaon_intr_mask4);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask5Set(unsigned int uaon_intr_mask5);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask6Set(unsigned int uaon_intr_mask6);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask7Set(unsigned int uaon_intr_mask7);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask8Set(unsigned int uaon_intr_mask8);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask9Set(unsigned int uaon_intr_mask9);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask10Set(unsigned int uaon_intr_mask10);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask11Set(unsigned int uaon_intr_mask11);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask12Set(unsigned int uaon_intr_mask12);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask13Set(unsigned int uaon_intr_mask13);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat0Set(unsigned int uaon_intr_stat0);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat1Set(unsigned int uaon_intr_stat1);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat4Set(unsigned int uaon_intr_stat4);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat5Set(unsigned int uaon_intr_stat5);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat6Set(unsigned int uaon_intr_stat6);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat7Set(unsigned int uaon_intr_stat7);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat8Set(unsigned int uaon_intr_stat8);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat9Set(unsigned int uaon_intr_stat9);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat10Set(unsigned int uaon_intr_stat10);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat11Set(unsigned int uaon_intr_stat11);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat12Set(unsigned int uaon_intr_stat12);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat13Set(unsigned int uaon_intr_stat13);
int HDMI_DDC_MST_CTRL_ddc_aon_accessSet(unsigned int uddc_aon_access);
int HDMI_DDC_MST_CTRL_dcc_man_enSet(unsigned int udcc_man_en);
int HDMI_DDC_MST_CTRL_ddc_speed_cntSet(unsigned int uddc_speed_cnt);
int HDMI_DDC_FIFO_RDATA_ddc_fifo_data_outSet(unsigned int uddc_fifo_data_out);
int HDMI_DDC_FIFO_WDATA_ddc_fifo_data_inSet(unsigned int uddc_fifo_data_in);
int HDMI_DDC_DATA_CNT_ddc_fifo_data_cntSet(unsigned int uddc_fifo_data_cnt);
int HDMI_DDC_DATA_CNT_ddc_data_out_cntSet(unsigned int uddc_data_out_cnt);
int HDMI_DDC_SLAVE_CFG_ddc_slave_addrSet(unsigned int uddc_slave_addr);
int HDMI_DDC_SLAVE_CFG_ddc_slave_offsetSet(unsigned int uddc_slave_offset);
int HDMI_DDC_SLAVE_CFG_ddc_slave_segSet(unsigned int uddc_slave_seg);
int HDMI_DDC_MST_STATE_ddc_i2c_no_ackSet(unsigned int uddc_i2c_no_ack);
int HDMI_DDC_MST_STATE_ddc_i2c_bus_lowSet(unsigned int uddc_i2c_bus_low);
int HDMI_DDC_MST_STATE_ddc_i2c_in_progSet(unsigned int uddc_i2c_in_prog);
int HDMI_DDC_MST_STATE_ddc_fifo_wr_in_useSet(unsigned int uddc_fifo_wr_in_use);
int HDMI_DDC_MST_STATE_ddc_fifo_rd_in_useSet(unsigned int uddc_fifo_rd_in_use);
int HDMI_DDC_MST_STATE_ddc_fifo_emptySet(unsigned int uddc_fifo_empty);
int HDMI_DDC_MST_STATE_ddc_fifo_half_fullSet(unsigned int uddc_fifo_half_full);
int HDMI_DDC_MST_STATE_ddc_fifo_fullSet(unsigned int uddc_fifo_full);
int HDMI_DDC_MST_CMD_ddc_mst_cmdSet(unsigned int uddc_mst_cmd);
int HDMI_DDC_MAN_CTRL_ddc_scl_stSet(unsigned int uddc_scl_st);
int HDMI_DDC_MAN_CTRL_ddc_sda_stSet(unsigned int uddc_sda_st);
int HDMI_DDC_MAN_CTRL_ddc_scl_oenSet(unsigned int uddc_scl_oen);
int HDMI_DDC_MAN_CTRL_ddc_sda_oenSet(unsigned int uddc_sda_oen);
int HDMI_DDC_STATE_CLR_ddc_clr_bus_lowSet(unsigned int uddc_clr_bus_low);
int HDMI_DDC_STATE_CLR_ddc_clr_no_ackSet(unsigned int uddc_clr_no_ack);

/* Declare the functions that set the member value */
int HDMI_TX_HW_INFO_tx_hw_dayGet(void);
int HDMI_TX_HW_INFO_tx_hw_monthGet(void);
int HDMI_TX_HW_INFO_tx_hw_yearGet(void);
int HDMI_TX_HW_VERSION_tx_reg_versionGet(void);
int HDMI_TX_HW_VERSION_tx_drv_versionGet(void);
int HDMI_TX_HW_VERSION_tx_rtl_versionGet(void);
int HDMI_TX_AON_RST_CTRL_tx_aon_soft_arst_reqGet(void);
int HDMI_HOTPLUG_ST_CFG_hpd_fillter_enGet(void);
int HDMI_HOTPLUG_ST_CFG_hpd_override_enGet(void);
int HDMI_HOTPLUG_ST_CFG_hpd_polarity_ctlGet(void);
int HDMI_HOTPLUG_ST_CFG_hpd_soft_valueGet(void);
int HDMI_HOTPLUG_ST_CFG_osc_div_cntGet(void);
int HDMI_HOTPLUG_FILLTER_CFG_hpd_high_resholdGet(void);
int HDMI_HOTPLUG_FILLTER_CFG_hpd_low_resholdGet(void);
int HDMI_TX_AON_STATE_hotplug_stateGet(void);
int HDMI_TX_AON_STATE_phy_rx_senseGet(void);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask0Get(void);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask1Get(void);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask4Get(void);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask5Get(void);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask6Get(void);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask7Get(void);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask8Get(void);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask9Get(void);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask10Get(void);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask11Get(void);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask12Get(void);
int HDMI_TX_AON_INTR_MASK_aon_intr_mask13Get(void);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat0Get(void);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat1Get(void);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat4Get(void);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat5Get(void);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat6Get(void);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat7Get(void);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat8Get(void);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat9Get(void);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat10Get(void);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat11Get(void);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat12Get(void);
int HDMI_TX_AON_INTR_STATE_aon_intr_stat13Get(void);
int HDMI_DDC_MST_CTRL_ddc_aon_accessGet(void);
int HDMI_DDC_MST_CTRL_dcc_man_enGet(void);
int HDMI_DDC_MST_CTRL_ddc_speed_cntGet(void);
int HDMI_DDC_FIFO_RDATA_ddc_fifo_data_outGet(void);
int HDMI_DDC_FIFO_WDATA_ddc_fifo_data_inGet(void);
int HDMI_DDC_DATA_CNT_ddc_fifo_data_cntGet(void);
int HDMI_DDC_DATA_CNT_ddc_data_out_cntGet(void);
int HDMI_DDC_SLAVE_CFG_ddc_slave_addrGet(void);
int HDMI_DDC_SLAVE_CFG_ddc_slave_offsetGet(void);
int HDMI_DDC_SLAVE_CFG_ddc_slave_segGet(void);
int HDMI_DDC_MST_STATE_ddc_i2c_no_ackGet(void);
int HDMI_DDC_MST_STATE_ddc_i2c_bus_lowGet(void);
int HDMI_DDC_MST_STATE_ddc_i2c_in_progGet(void);
int HDMI_DDC_MST_STATE_ddc_fifo_wr_in_useGet(void);
int HDMI_DDC_MST_STATE_ddc_fifo_rd_in_useGet(void);
int HDMI_DDC_MST_STATE_ddc_fifo_emptyGet(void);
int HDMI_DDC_MST_STATE_ddc_fifo_half_fullGet(void);
int HDMI_DDC_MST_STATE_ddc_fifo_fullGet(void);
int HDMI_DDC_MST_CMD_ddc_mst_cmdGet(void);
int HDMI_DDC_MAN_CTRL_ddc_scl_stGet(void);
int HDMI_DDC_MAN_CTRL_ddc_sda_stGet(void);
int HDMI_DDC_MAN_CTRL_ddc_scl_oenGet(void);
int HDMI_DDC_MAN_CTRL_ddc_sda_oenGet(void);
int HDMI_DDC_STATE_CLR_ddc_clr_bus_lowGet(void);
int HDMI_DDC_STATE_CLR_ddc_clr_no_ackGet(void);

#endif // __TX_AON_REG_C_UNION_DEFINE_H__
