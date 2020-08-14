// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  tx_hdmi_reg_c_union_define.h
// Project line  :  Platform And Key Technologies Development
// Department    :  CAD Development Department
// Author        :  w00271848
// Version       :  1.0
// Date          :  Tester
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00271848 2015/04/20 11:14:24 Create file
// ******************************************************************************

#include "hi_type.h"

#ifndef __TX_HDMI_REG_C_UNION_DEFINE_H__
#define __TX_HDMI_REG_C_UNION_DEFINE_H__

/* Define the union U_TX_PACK_FIFO_CTRL */
/* Define the union U_TX_PACK_FIFO_CTRL,ATTR:,ADDR:1800,INIT:00000004,MASK:ffffffff,CMMNT:TX PACK模块模式配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tmds_pack_mode       : 2  ; /* [1:0] */
        unsigned int    reg_fifo_auto_rst_en : 1  ; /* [2] */
        unsigned int    reg_fifo_manu_rst    : 1  ; /* [3] */
        unsigned int    reg_clock_det_en     : 1  ; /* [4] */
        unsigned int    reg_ext_tmds_para    : 1  ; /* [5] */
        unsigned int    rsv_0                : 2  ; /* [7:6] */
        unsigned int    reg_fifo_delay_cnt   : 8  ; /* [15:8] */
        unsigned int    rsv_1                : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_PACK_FIFO_CTRL;

/* Define the union U_TX_PACK_FIFO_ST */
/* Define the union U_TX_PACK_FIFO_ST,ATTR:,ADDR:1804,INIT:00000000,MASK:1,CMMNT:TX PACK模块工作状态寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pclk2tclk_stable : 1  ; /* [0] */
        unsigned int    rsv_2            : 31  ; /* [31:1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_PACK_FIFO_ST;

/* Define the union U_PCLK_REFER_CNT */
/* Define the union U_PCLK_REFER_CNT,ATTR:,ADDR:1808,INIT:000003FF,MASK:3ffff,CMMNT:TMDS时钟检测PCLK参考计数器寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_pclk_refer_cnt : 18  ; /* [17:0] */
        unsigned int    rsv_3              : 14  ; /* [31:18] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PCLK_REFER_CNT;

/* Define the union U_TCLK_LOWER_THRESHOLD */
/* Define the union U_TCLK_LOWER_THRESHOLD,ATTR:,ADDR:180C,INIT:000003F8,MASK:3ffff,CMMNT:TMDS时钟检测TCLK计数器下阈值寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_tcnt_lower_threshold : 18  ; /* [17:0] */
        unsigned int    rsv_4                    : 14  ; /* [31:18] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TCLK_LOWER_THRESHOLD;

/* Define the union U_TCLK_UPPER_THRESHOLD */
/* Define the union U_TCLK_UPPER_THRESHOLD,ATTR:,ADDR:1810,INIT:00000408,MASK:3ffff,CMMNT:TMDS时钟检测TCLK计数器上阈值寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_tcnt_upper_threshold : 18  ; /* [17:0] */
        unsigned int    rsv_5                    : 14  ; /* [31:18] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TCLK_UPPER_THRESHOLD;

/* Define the union U_AVI_PKT_HEADER */
/* Define the union U_AVI_PKT_HEADER,ATTR:,ADDR:1818,INIT:00000000,MASK:ffffff,CMMNT:AVI 信息帧包头配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    avi_pkt_hb0 : 8  ; /* [7:0] */
        unsigned int    avi_pkt_hb1 : 8  ; /* [15:8] */
        unsigned int    avi_pkt_hb2 : 8  ; /* [23:16] */
        unsigned int    rsv_6       : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AVI_PKT_HEADER;

/* Define the union U_AVI_SUB_PKT0_L */
/* Define the union U_AVI_SUB_PKT0_L,ATTR:,ADDR:181C,INIT:00000000,MASK:ffffffff,CMMNT:AVI 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    avi_sub_pkt0_pb0 : 8  ; /* [7:0] */
        unsigned int    avi_sub_pkt0_pb1 : 8  ; /* [15:8] */
        unsigned int    avi_sub_pkt0_pb2 : 8  ; /* [23:16] */
        unsigned int    avi_sub_pkt0_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AVI_SUB_PKT0_L;

/* Define the union U_AVI_SUB_PKT0_H */
/* Define the union U_AVI_SUB_PKT0_H,ATTR:,ADDR:1820,INIT:00000000,MASK:ffffff,CMMNT:AVI 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    avi_sub_pkt0_pb4 : 8  ; /* [7:0] */
        unsigned int    avi_sub_pkt0_pb5 : 8  ; /* [15:8] */
        unsigned int    avi_sub_pkt0_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_7            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AVI_SUB_PKT0_H;

/* Define the union U_AVI_SUB_PKT1_L */
/* Define the union U_AVI_SUB_PKT1_L,ATTR:,ADDR:1824,INIT:00000000,MASK:ffffffff,CMMNT:AVI 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    avi_sub_pkt1_pb0 : 8  ; /* [7:0] */
        unsigned int    avi_sub_pkt1_pb1 : 8  ; /* [15:8] */
        unsigned int    avi_sub_pkt1_pb2 : 8  ; /* [23:16] */
        unsigned int    avi_sub_pkt1_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AVI_SUB_PKT1_L;

/* Define the union U_AVI_SUB_PKT1_H */
/* Define the union U_AVI_SUB_PKT1_H,ATTR:,ADDR:1828,INIT:00000000,MASK:ffffff,CMMNT:AVI 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    avi_sub_pkt1_pb4 : 8  ; /* [7:0] */
        unsigned int    avi_sub_pkt1_pb5 : 8  ; /* [15:8] */
        unsigned int    avi_sub_pkt1_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_8            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AVI_SUB_PKT1_H;

/* Define the union U_AVI_SUB_PKT2_L */
/* Define the union U_AVI_SUB_PKT2_L,ATTR:,ADDR:182C,INIT:00000000,MASK:ffffffff,CMMNT:AVI 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    avi_sub_pkt2_pb0 : 8  ; /* [7:0] */
        unsigned int    avi_sub_pkt2_pb1 : 8  ; /* [15:8] */
        unsigned int    avi_sub_pkt2_pb2 : 8  ; /* [23:16] */
        unsigned int    avi_sub_pkt2_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AVI_SUB_PKT2_L;

/* Define the union U_AVI_SUB_PKT2_H */
/* Define the union U_AVI_SUB_PKT2_H,ATTR:,ADDR:1830,INIT:00000000,MASK:ffffff,CMMNT:AVI 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    avi_sub_pkt2_pb4 : 8  ; /* [7:0] */
        unsigned int    avi_sub_pkt2_pb5 : 8  ; /* [15:8] */
        unsigned int    avi_sub_pkt2_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_9            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AVI_SUB_PKT2_H;

/* Define the union U_AVI_SUB_PKT3_L */
/* Define the union U_AVI_SUB_PKT3_L,ATTR:,ADDR:1834,INIT:00000000,MASK:ffffffff,CMMNT:AVI 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    avi_sub_pkt3_pb0 : 8  ; /* [7:0] */
        unsigned int    avi_sub_pkt3_pb1 : 8  ; /* [15:8] */
        unsigned int    avi_sub_pkt3_pb2 : 8  ; /* [23:16] */
        unsigned int    avi_sub_pkt3_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AVI_SUB_PKT3_L;

/* Define the union U_AVI_SUB_PKT3_H */
/* Define the union U_AVI_SUB_PKT3_H,ATTR:,ADDR:1838,INIT:00000000,MASK:ffffff,CMMNT:AVI 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    avi_sub_pkt3_pb4 : 8  ; /* [7:0] */
        unsigned int    avi_sub_pkt3_pb5 : 8  ; /* [15:8] */
        unsigned int    avi_sub_pkt3_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_10           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AVI_SUB_PKT3_H;

/* Define the union U_AIF_PKT_HEADER */
/* Define the union U_AIF_PKT_HEADER,ATTR:,ADDR:183C,INIT:00000000,MASK:ffffff,CMMNT:AUDIO 信息帧包头配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aif_pkt_hb0 : 8  ; /* [7:0] */
        unsigned int    aif_pkt_hb1 : 8  ; /* [15:8] */
        unsigned int    aif_pkt_hb2 : 8  ; /* [23:16] */
        unsigned int    rsv_11      : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIF_PKT_HEADER;

/* Define the union U_AIF_SUB_PKT0_L */
/* Define the union U_AIF_SUB_PKT0_L,ATTR:,ADDR:1840,INIT:00000000,MASK:ffffffff,CMMNT:AUDIO 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aif_sub_pkt0_pb0 : 8  ; /* [7:0] */
        unsigned int    aif_sub_pkt0_pb1 : 8  ; /* [15:8] */
        unsigned int    aif_sub_pkt0_pb2 : 8  ; /* [23:16] */
        unsigned int    aif_sub_pkt0_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIF_SUB_PKT0_L;

/* Define the union U_AIF_SUB_PKT0_H */
/* Define the union U_AIF_SUB_PKT0_H,ATTR:,ADDR:1844,INIT:00000000,MASK:ffffff,CMMNT:AUDIO 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aif_sub_pkt0_pb4 : 8  ; /* [7:0] */
        unsigned int    aif_sub_pkt0_pb5 : 8  ; /* [15:8] */
        unsigned int    aif_sub_pkt0_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_12           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIF_SUB_PKT0_H;

/* Define the union U_AIF_SUB_PKT1_L */
/* Define the union U_AIF_SUB_PKT1_L,ATTR:,ADDR:1848,INIT:00000000,MASK:ffffffff,CMMNT:AUDIO 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aif_sub_pkt1_pb0 : 8  ; /* [7:0] */
        unsigned int    aif_sub_pkt1_pb1 : 8  ; /* [15:8] */
        unsigned int    aif_sub_pkt1_pb2 : 8  ; /* [23:16] */
        unsigned int    aif_sub_pkt1_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIF_SUB_PKT1_L;

/* Define the union U_AIF_SUB_PKT1_H */
/* Define the union U_AIF_SUB_PKT1_H,ATTR:,ADDR:184C,INIT:00000000,MASK:ffffff,CMMNT:AUDIO 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aif_sub_pkt1_pb4 : 8  ; /* [7:0] */
        unsigned int    aif_sub_pkt1_pb5 : 8  ; /* [15:8] */
        unsigned int    aif_sub_pkt1_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_13           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIF_SUB_PKT1_H;

/* Define the union U_AIF_SUB_PKT2_L */
/* Define the union U_AIF_SUB_PKT2_L,ATTR:,ADDR:1850,INIT:00000000,MASK:ffffffff,CMMNT:AUDIO 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aif_sub_pkt2_pb0 : 8  ; /* [7:0] */
        unsigned int    aif_sub_pkt2_pb1 : 8  ; /* [15:8] */
        unsigned int    aif_sub_pkt2_pb2 : 8  ; /* [23:16] */
        unsigned int    aif_sub_pkt2_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIF_SUB_PKT2_L;

/* Define the union U_AIF_SUB_PKT2_H */
/* Define the union U_AIF_SUB_PKT2_H,ATTR:,ADDR:1854,INIT:00000000,MASK:ffffff,CMMNT:AUDIO 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aif_sub_pkt2_pb4 : 8  ; /* [7:0] */
        unsigned int    aif_sub_pkt2_pb5 : 8  ; /* [15:8] */
        unsigned int    aif_sub_pkt2_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_14           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIF_SUB_PKT2_H;

/* Define the union U_AIF_SUB_PKT3_L */
/* Define the union U_AIF_SUB_PKT3_L,ATTR:,ADDR:1858,INIT:00000000,MASK:ffffffff,CMMNT:AUDIO 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aif_sub_pkt3_pb0 : 8  ; /* [7:0] */
        unsigned int    aif_sub_pkt3_pb1 : 8  ; /* [15:8] */
        unsigned int    aif_sub_pkt3_pb2 : 8  ; /* [23:16] */
        unsigned int    aif_sub_pkt3_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIF_SUB_PKT3_L;

/* Define the union U_AIF_SUB_PKT3_H */
/* Define the union U_AIF_SUB_PKT3_H,ATTR:,ADDR:185C,INIT:00000000,MASK:ffffff,CMMNT:AUDIO 信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aif_sub_pkt3_pb4 : 8  ; /* [7:0] */
        unsigned int    aif_sub_pkt3_pb5 : 8  ; /* [15:8] */
        unsigned int    aif_sub_pkt3_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_15           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIF_SUB_PKT3_H;

/* Define the union U_SPIF_PKT_HEADER */
/* Define the union U_SPIF_PKT_HEADER,ATTR:,ADDR:1860,INIT:00000000,MASK:ffffff,CMMNT:Source Product Descriptor信息帧包头配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    spd_pkt_hb0 : 8  ; /* [7:0] */
        unsigned int    spd_pkt_hb1 : 8  ; /* [15:8] */
        unsigned int    spd_pkt_hb2 : 8  ; /* [23:16] */
        unsigned int    rsv_16      : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SPIF_PKT_HEADER;

/* Define the union U_SPIF_SUB_PKT0_L */
/* Define the union U_SPIF_SUB_PKT0_L,ATTR:,ADDR:1864,INIT:00000000,MASK:ffffffff,CMMNT:Source Product Descriptor信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    spd_sub_pkt0_pb0 : 8  ; /* [7:0] */
        unsigned int    spd_sub_pkt0_pb1 : 8  ; /* [15:8] */
        unsigned int    spd_sub_pkt0_pb2 : 8  ; /* [23:16] */
        unsigned int    spd_sub_pkt0_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SPIF_SUB_PKT0_L;

/* Define the union U_SPIF_SUB_PKT0_H */
/* Define the union U_SPIF_SUB_PKT0_H,ATTR:,ADDR:1868,INIT:00000000,MASK:ffffff,CMMNT:Source Product Descriptor信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    spd_sub_pkt0_pb4 : 8  ; /* [7:0] */
        unsigned int    spd_sub_pkt0_pb5 : 8  ; /* [15:8] */
        unsigned int    spd_sub_pkt0_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_17           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SPIF_SUB_PKT0_H;

/* Define the union U_SPIF_SUB_PKT1_L */
/* Define the union U_SPIF_SUB_PKT1_L,ATTR:,ADDR:186C,INIT:00000000,MASK:ffffffff,CMMNT:Source Product Descriptor信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    spd_sub_pkt1_pb0 : 8  ; /* [7:0] */
        unsigned int    spd_sub_pkt1_pb1 : 8  ; /* [15:8] */
        unsigned int    spd_sub_pkt1_pb2 : 8  ; /* [23:16] */
        unsigned int    spd_sub_pkt1_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SPIF_SUB_PKT1_L;

/* Define the union U_SPIF_SUB_PKT1_H */
/* Define the union U_SPIF_SUB_PKT1_H,ATTR:,ADDR:1870,INIT:00000000,MASK:ffffff,CMMNT:Source Product Descriptor信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    spd_sub_pkt1_pb4 : 8  ; /* [7:0] */
        unsigned int    spd_sub_pkt1_pb5 : 8  ; /* [15:8] */
        unsigned int    spd_sub_pkt1_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_18           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SPIF_SUB_PKT1_H;

/* Define the union U_SPIF_SUB_PKT2_L */
/* Define the union U_SPIF_SUB_PKT2_L,ATTR:,ADDR:1874,INIT:00000000,MASK:ffffffff,CMMNT:Source Product Descriptor信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    spd_sub_pkt2_pb0 : 8  ; /* [7:0] */
        unsigned int    spd_sub_pkt2_pb1 : 8  ; /* [15:8] */
        unsigned int    spd_sub_pkt2_pb2 : 8  ; /* [23:16] */
        unsigned int    spd_sub_pkt2_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SPIF_SUB_PKT2_L;

/* Define the union U_SPIF_SUB_PKT2_H */
/* Define the union U_SPIF_SUB_PKT2_H,ATTR:,ADDR:1878,INIT:00000000,MASK:ffffff,CMMNT:Source Product Descriptor信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    spd_sub_pkt2_pb4 : 8  ; /* [7:0] */
        unsigned int    spd_sub_pkt2_pb5 : 8  ; /* [15:8] */
        unsigned int    spd_sub_pkt2_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_19           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SPIF_SUB_PKT2_H;

/* Define the union U_SPIF_SUB_PKT3_L */
/* Define the union U_SPIF_SUB_PKT3_L,ATTR:,ADDR:187C,INIT:00000000,MASK:ffffffff,CMMNT:Source Product Descriptor信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    spd_sub_pkt3_pb0 : 8  ; /* [7:0] */
        unsigned int    spd_sub_pkt3_pb1 : 8  ; /* [15:8] */
        unsigned int    spd_sub_pkt3_pb2 : 8  ; /* [23:16] */
        unsigned int    spd_sub_pkt3_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SPIF_SUB_PKT3_L;

/* Define the union U_SPIF_SUB_PKT3_H */
/* Define the union U_SPIF_SUB_PKT3_H,ATTR:,ADDR:1880,INIT:00000000,MASK:ffffff,CMMNT:Source Product Descriptor信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    spd_sub_pkt3_pb4 : 8  ; /* [7:0] */
        unsigned int    spd_sub_pkt3_pb5 : 8  ; /* [15:8] */
        unsigned int    spd_sub_pkt3_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_20           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SPIF_SUB_PKT3_H;

/* Define the union U_MPEG_PKT_HEADER */
/* Define the union U_MPEG_PKT_HEADER,ATTR:,ADDR:1884,INIT:00000000,MASK:ffffff,CMMNT:MPEG Source信息帧包头配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mpeg_pkt_hb0 : 8  ; /* [7:0] */
        unsigned int    mpeg_pkt_hb1 : 8  ; /* [15:8] */
        unsigned int    mpeg_pkt_hb2 : 8  ; /* [23:16] */
        unsigned int    rsv_21       : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MPEG_PKT_HEADER;

/* Define the union U_MPEG_SUB_PKT0_L */
/* Define the union U_MPEG_SUB_PKT0_L,ATTR:,ADDR:1888,INIT:00000000,MASK:ffffffff,CMMNT:MPEG Source信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mpeg_sub_pkt0_pb0 : 8  ; /* [7:0] */
        unsigned int    mpeg_sub_pkt0_pb1 : 8  ; /* [15:8] */
        unsigned int    mpeg_sub_pkt0_pb2 : 8  ; /* [23:16] */
        unsigned int    mpeg_sub_pkt0_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MPEG_SUB_PKT0_L;

/* Define the union U_MPEG_SUB_PKT0_H */
/* Define the union U_MPEG_SUB_PKT0_H,ATTR:,ADDR:188C,INIT:00000000,MASK:ffffff,CMMNT:MPEG Source信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mpeg_sub_pkt0_pb4 : 8  ; /* [7:0] */
        unsigned int    mpeg_sub_pkt0_pb5 : 8  ; /* [15:8] */
        unsigned int    mpeg_sub_pkt0_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_22            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MPEG_SUB_PKT0_H;

/* Define the union U_MPEG_SUB_PKT1_L */
/* Define the union U_MPEG_SUB_PKT1_L,ATTR:,ADDR:1890,INIT:00000000,MASK:ffffffff,CMMNT:MPEG Source信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mpeg_sub_pkt1_pb0 : 8  ; /* [7:0] */
        unsigned int    mpeg_sub_pkt1_pb1 : 8  ; /* [15:8] */
        unsigned int    mpeg_sub_pkt1_pb2 : 8  ; /* [23:16] */
        unsigned int    mpeg_sub_pkt1_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MPEG_SUB_PKT1_L;

/* Define the union U_MPEG_SUB_PKT1_H */
/* Define the union U_MPEG_SUB_PKT1_H,ATTR:,ADDR:1894,INIT:00000000,MASK:ffffff,CMMNT:MPEG Source信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mpeg_sub_pkt1_pb4 : 8  ; /* [7:0] */
        unsigned int    mpeg_sub_pkt1_pb5 : 8  ; /* [15:8] */
        unsigned int    mpeg_sub_pkt1_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_23            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MPEG_SUB_PKT1_H;

/* Define the union U_MPEG_SUB_PKT2_L */
/* Define the union U_MPEG_SUB_PKT2_L,ATTR:,ADDR:1898,INIT:00000000,MASK:ffffffff,CMMNT:MPEG Source信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mpeg_sub_pkt2_pb0 : 8  ; /* [7:0] */
        unsigned int    mpeg_sub_pkt2_pb1 : 8  ; /* [15:8] */
        unsigned int    mpeg_sub_pkt2_pb2 : 8  ; /* [23:16] */
        unsigned int    mpeg_sub_pkt2_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MPEG_SUB_PKT2_L;

/* Define the union U_MPEG_SUB_PKT2_H */
/* Define the union U_MPEG_SUB_PKT2_H,ATTR:,ADDR:189C,INIT:00000000,MASK:ffffff,CMMNT:MPEG Source信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mpeg_sub_pkt2_pb4 : 8  ; /* [7:0] */
        unsigned int    mpeg_sub_pkt2_pb5 : 8  ; /* [15:8] */
        unsigned int    mpeg_sub_pkt2_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_24            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MPEG_SUB_PKT2_H;

/* Define the union U_MPEG_SUB_PKT3_L */
/* Define the union U_MPEG_SUB_PKT3_L,ATTR:,ADDR:18A0,INIT:00000000,MASK:ffffffff,CMMNT:MPEG Source信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mpeg_sub_pkt3_pb0 : 8  ; /* [7:0] */
        unsigned int    mpeg_sub_pkt3_pb1 : 8  ; /* [15:8] */
        unsigned int    mpeg_sub_pkt3_pb2 : 8  ; /* [23:16] */
        unsigned int    mpeg_sub_pkt3_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MPEG_SUB_PKT3_L;

/* Define the union U_MPEG_SUB_PKT3_H */
/* Define the union U_MPEG_SUB_PKT3_H,ATTR:,ADDR:18A4,INIT:00000000,MASK:ffffff,CMMNT:MPEG Source信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mpeg_sub_pkt3_pb4 : 8  ; /* [7:0] */
        unsigned int    mpeg_sub_pkt3_pb5 : 8  ; /* [15:8] */
        unsigned int    mpeg_sub_pkt3_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_25            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MPEG_SUB_PKT3_H;

/* Define the union U_GEN_PKT_HEADER */
/* Define the union U_GEN_PKT_HEADER,ATTR:,ADDR:18A8,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包头配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen_pkt_hb0 : 8  ; /* [7:0] */
        unsigned int    gen_pkt_hb1 : 8  ; /* [15:8] */
        unsigned int    gen_pkt_hb2 : 8  ; /* [23:16] */
        unsigned int    rsv_26      : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN_PKT_HEADER;

/* Define the union U_GEN_SUB_PKT0_L */
/* Define the union U_GEN_SUB_PKT0_L,ATTR:,ADDR:18AC,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen_sub_pkt0_pb0 : 8  ; /* [7:0] */
        unsigned int    gen_sub_pkt0_pb1 : 8  ; /* [15:8] */
        unsigned int    gen_sub_pkt0_pb2 : 8  ; /* [23:16] */
        unsigned int    gen_sub_pkt0_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN_SUB_PKT0_L;

/* Define the union U_GEN_SUB_PKT0_H */
/* Define the union U_GEN_SUB_PKT0_H,ATTR:,ADDR:18B0,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen_sub_pkt0_pb4 : 8  ; /* [7:0] */
        unsigned int    gen_sub_pkt0_pb5 : 8  ; /* [15:8] */
        unsigned int    gen_sub_pkt0_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_27           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN_SUB_PKT0_H;

/* Define the union U_GEN_SUB_PKT1_L */
/* Define the union U_GEN_SUB_PKT1_L,ATTR:,ADDR:18B4,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen_sub_pkt1_pb0 : 8  ; /* [7:0] */
        unsigned int    gen_sub_pkt1_pb1 : 8  ; /* [15:8] */
        unsigned int    gen_sub_pkt1_pb2 : 8  ; /* [23:16] */
        unsigned int    gen_sub_pkt1_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN_SUB_PKT1_L;

/* Define the union U_GEN_SUB_PKT1_H */
/* Define the union U_GEN_SUB_PKT1_H,ATTR:,ADDR:18B8,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen_sub_pkt1_pb4 : 8  ; /* [7:0] */
        unsigned int    gen_sub_pkt1_pb5 : 8  ; /* [15:8] */
        unsigned int    gen_sub_pkt1_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_28           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN_SUB_PKT1_H;

/* Define the union U_GEN_SUB_PKT2_L */
/* Define the union U_GEN_SUB_PKT2_L,ATTR:,ADDR:18BC,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen_sub_pkt2_pb0 : 8  ; /* [7:0] */
        unsigned int    gen_sub_pkt2_pb1 : 8  ; /* [15:8] */
        unsigned int    gen_sub_pkt2_pb2 : 8  ; /* [23:16] */
        unsigned int    gen_sub_pkt2_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN_SUB_PKT2_L;

/* Define the union U_GEN_SUB_PKT2_H */
/* Define the union U_GEN_SUB_PKT2_H,ATTR:,ADDR:18C0,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen_sub_pkt2_pb4 : 8  ; /* [7:0] */
        unsigned int    gen_sub_pkt2_pb5 : 8  ; /* [15:8] */
        unsigned int    gen_sub_pkt2_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_29           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN_SUB_PKT2_H;

/* Define the union U_GEN_SUB_PKT3_L */
/* Define the union U_GEN_SUB_PKT3_L,ATTR:,ADDR:18C4,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen_sub_pkt3_pb0 : 8  ; /* [7:0] */
        unsigned int    gen_sub_pkt3_pb1 : 8  ; /* [15:8] */
        unsigned int    gen_sub_pkt3_pb2 : 8  ; /* [23:16] */
        unsigned int    gen_sub_pkt3_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN_SUB_PKT3_L;

/* Define the union U_GEN_SUB_PKT3_H */
/* Define the union U_GEN_SUB_PKT3_H,ATTR:,ADDR:18C8,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen_sub_pkt3_pb4 : 8  ; /* [7:0] */
        unsigned int    gen_sub_pkt3_pb5 : 8  ; /* [15:8] */
        unsigned int    gen_sub_pkt3_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_30           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN_SUB_PKT3_H;

/* Define the union U_GEN2_PKT_HEADER */
/* Define the union U_GEN2_PKT_HEADER,ATTR:,ADDR:18CC,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包头配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen2_pkt_hb0 : 8  ; /* [7:0] */
        unsigned int    gen2_pkt_hb1 : 8  ; /* [15:8] */
        unsigned int    gen2_pkt_hb2 : 8  ; /* [23:16] */
        unsigned int    rsv_31       : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN2_PKT_HEADER;

/* Define the union U_GEN2_SUB_PKT0_L */
/* Define the union U_GEN2_SUB_PKT0_L,ATTR:,ADDR:18D0,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen2_sub_pkt0_pb0 : 8  ; /* [7:0] */
        unsigned int    gen2_sub_pkt0_pb1 : 8  ; /* [15:8] */
        unsigned int    gen2_sub_pkt0_pb2 : 8  ; /* [23:16] */
        unsigned int    gen2_sub_pkt0_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN2_SUB_PKT0_L;

/* Define the union U_GEN2_SUB_PKT0_H */
/* Define the union U_GEN2_SUB_PKT0_H,ATTR:,ADDR:18D4,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen2_sub_pkt0_pb4 : 8  ; /* [7:0] */
        unsigned int    gen2_sub_pkt0_pb5 : 8  ; /* [15:8] */
        unsigned int    gen2_sub_pkt0_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_32            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN2_SUB_PKT0_H;

/* Define the union U_GEN2_SUB_PKT1_L */
/* Define the union U_GEN2_SUB_PKT1_L,ATTR:,ADDR:18D8,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen2_sub_pkt1_pb0 : 8  ; /* [7:0] */
        unsigned int    gen2_sub_pkt1_pb1 : 8  ; /* [15:8] */
        unsigned int    gen2_sub_pkt1_pb2 : 8  ; /* [23:16] */
        unsigned int    gen2_sub_pkt1_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN2_SUB_PKT1_L;

/* Define the union U_GEN2_SUB_PKT1_H */
/* Define the union U_GEN2_SUB_PKT1_H,ATTR:,ADDR:18DC,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen2_sub_pkt1_pb4 : 8  ; /* [7:0] */
        unsigned int    gen2_sub_pkt1_pb5 : 8  ; /* [15:8] */
        unsigned int    gen2_sub_pkt1_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_33            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN2_SUB_PKT1_H;

/* Define the union U_GEN2_SUB_PKT2_L */
/* Define the union U_GEN2_SUB_PKT2_L,ATTR:,ADDR:18E0,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen2_sub_pkt2_pb0 : 8  ; /* [7:0] */
        unsigned int    gen2_sub_pkt2_pb1 : 8  ; /* [15:8] */
        unsigned int    gen2_sub_pkt2_pb2 : 8  ; /* [23:16] */
        unsigned int    gen2_sub_pkt2_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN2_SUB_PKT2_L;

/* Define the union U_GEN2_SUB_PKT2_H */
/* Define the union U_GEN2_SUB_PKT2_H,ATTR:,ADDR:18E4,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen2_sub_pkt2_pb4 : 8  ; /* [7:0] */
        unsigned int    gen2_sub_pkt2_pb5 : 8  ; /* [15:8] */
        unsigned int    gen2_sub_pkt2_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_34            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN2_SUB_PKT2_H;

/* Define the union U_GEN2_SUB_PKT3_L */
/* Define the union U_GEN2_SUB_PKT3_L,ATTR:,ADDR:18E8,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen2_sub_pkt3_pb0 : 8  ; /* [7:0] */
        unsigned int    gen2_sub_pkt3_pb1 : 8  ; /* [15:8] */
        unsigned int    gen2_sub_pkt3_pb2 : 8  ; /* [23:16] */
        unsigned int    gen2_sub_pkt3_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN2_SUB_PKT3_L;

/* Define the union U_GEN2_SUB_PKT3_H */
/* Define the union U_GEN2_SUB_PKT3_H,ATTR:,ADDR:18EC,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen2_sub_pkt3_pb4 : 8  ; /* [7:0] */
        unsigned int    gen2_sub_pkt3_pb5 : 8  ; /* [15:8] */
        unsigned int    gen2_sub_pkt3_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_35            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN2_SUB_PKT3_H;

/* Define the union U_GEN3_PKT_HEADER */
/* Define the union U_GEN3_PKT_HEADER,ATTR:,ADDR:18F0,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包头配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen3_pkt_hb0 : 8  ; /* [7:0] */
        unsigned int    gen3_pkt_hb1 : 8  ; /* [15:8] */
        unsigned int    gen3_pkt_hb2 : 8  ; /* [23:16] */
        unsigned int    rsv_36       : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN3_PKT_HEADER;

/* Define the union U_GEN3_SUB_PKT0_L */
/* Define the union U_GEN3_SUB_PKT0_L,ATTR:,ADDR:18F4,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen3_sub_pkt0_pb0 : 8  ; /* [7:0] */
        unsigned int    gen3_sub_pkt0_pb1 : 8  ; /* [15:8] */
        unsigned int    gen3_sub_pkt0_pb2 : 8  ; /* [23:16] */
        unsigned int    gen3_sub_pkt0_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN3_SUB_PKT0_L;

/* Define the union U_GEN3_SUB_PKT0_H */
/* Define the union U_GEN3_SUB_PKT0_H,ATTR:,ADDR:18F8,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen3_sub_pkt0_pb4 : 8  ; /* [7:0] */
        unsigned int    gen3_sub_pkt0_pb5 : 8  ; /* [15:8] */
        unsigned int    gen3_sub_pkt0_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_37            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN3_SUB_PKT0_H;

/* Define the union U_GEN3_SUB_PKT1_L */
/* Define the union U_GEN3_SUB_PKT1_L,ATTR:,ADDR:18FC,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen3_sub_pkt1_pb0 : 8  ; /* [7:0] */
        unsigned int    gen3_sub_pkt1_pb1 : 8  ; /* [15:8] */
        unsigned int    gen3_sub_pkt1_pb2 : 8  ; /* [23:16] */
        unsigned int    gen3_sub_pkt1_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN3_SUB_PKT1_L;

/* Define the union U_GEN3_SUB_PKT1_H */
/* Define the union U_GEN3_SUB_PKT1_H,ATTR:,ADDR:1900,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen3_sub_pkt1_pb4 : 8  ; /* [7:0] */
        unsigned int    gen3_sub_pkt1_pb5 : 8  ; /* [15:8] */
        unsigned int    gen3_sub_pkt1_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_38            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN3_SUB_PKT1_H;

/* Define the union U_GEN3_SUB_PKT2_L */
/* Define the union U_GEN3_SUB_PKT2_L,ATTR:,ADDR:1904,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen3_sub_pkt2_pb0 : 8  ; /* [7:0] */
        unsigned int    gen3_sub_pkt2_pb1 : 8  ; /* [15:8] */
        unsigned int    gen3_sub_pkt2_pb2 : 8  ; /* [23:16] */
        unsigned int    gen3_sub_pkt2_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN3_SUB_PKT2_L;

/* Define the union U_GEN3_SUB_PKT2_H */
/* Define the union U_GEN3_SUB_PKT2_H,ATTR:,ADDR:1908,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen3_sub_pkt2_pb4 : 8  ; /* [7:0] */
        unsigned int    gen3_sub_pkt2_pb5 : 8  ; /* [15:8] */
        unsigned int    gen3_sub_pkt2_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_39            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN3_SUB_PKT2_H;

/* Define the union U_GEN3_SUB_PKT3_L */
/* Define the union U_GEN3_SUB_PKT3_L,ATTR:,ADDR:190C,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen3_sub_pkt3_pb0 : 8  ; /* [7:0] */
        unsigned int    gen3_sub_pkt3_pb1 : 8  ; /* [15:8] */
        unsigned int    gen3_sub_pkt3_pb2 : 8  ; /* [23:16] */
        unsigned int    gen3_sub_pkt3_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN3_SUB_PKT3_L;

/* Define the union U_GEN3_SUB_PKT3_H */
/* Define the union U_GEN3_SUB_PKT3_H,ATTR:,ADDR:1910,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen3_sub_pkt3_pb4 : 8  ; /* [7:0] */
        unsigned int    gen3_sub_pkt3_pb5 : 8  ; /* [15:8] */
        unsigned int    gen3_sub_pkt3_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_40            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN3_SUB_PKT3_H;

/* Define the union U_GEN4_PKT_HEADER */
/* Define the union U_GEN4_PKT_HEADER,ATTR:,ADDR:1914,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包头配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen4_pkt_hb0 : 8  ; /* [7:0] */
        unsigned int    gen4_pkt_hb1 : 8  ; /* [15:8] */
        unsigned int    gen4_pkt_hb2 : 8  ; /* [23:16] */
        unsigned int    rsv_41       : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN4_PKT_HEADER;

/* Define the union U_GEN4_SUB_PKT0_L */
/* Define the union U_GEN4_SUB_PKT0_L,ATTR:,ADDR:1918,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen4_sub_pkt0_pb0 : 8  ; /* [7:0] */
        unsigned int    gen4_sub_pkt0_pb1 : 8  ; /* [15:8] */
        unsigned int    gen4_sub_pkt0_pb2 : 8  ; /* [23:16] */
        unsigned int    gen4_sub_pkt0_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN4_SUB_PKT0_L;

/* Define the union U_GEN4_SUB_PKT0_H */
/* Define the union U_GEN4_SUB_PKT0_H,ATTR:,ADDR:191C,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen4_sub_pkt0_pb4 : 8  ; /* [7:0] */
        unsigned int    gen4_sub_pkt0_pb5 : 8  ; /* [15:8] */
        unsigned int    gen4_sub_pkt0_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_42            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN4_SUB_PKT0_H;

/* Define the union U_GEN4_SUB_PKT1_L */
/* Define the union U_GEN4_SUB_PKT1_L,ATTR:,ADDR:1920,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen4_sub_pkt1_pb0 : 8  ; /* [7:0] */
        unsigned int    gen4_sub_pkt1_pb1 : 8  ; /* [15:8] */
        unsigned int    gen4_sub_pkt1_pb2 : 8  ; /* [23:16] */
        unsigned int    gen4_sub_pkt1_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN4_SUB_PKT1_L;

/* Define the union U_GEN4_SUB_PKT1_H */
/* Define the union U_GEN4_SUB_PKT1_H,ATTR:,ADDR:1924,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen4_sub_pkt1_pb4 : 8  ; /* [7:0] */
        unsigned int    gen4_sub_pkt1_pb5 : 8  ; /* [15:8] */
        unsigned int    gen4_sub_pkt1_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_43            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN4_SUB_PKT1_H;

/* Define the union U_GEN4_SUB_PKT2_L */
/* Define the union U_GEN4_SUB_PKT2_L,ATTR:,ADDR:1928,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen4_sub_pkt2_pb0 : 8  ; /* [7:0] */
        unsigned int    gen4_sub_pkt2_pb1 : 8  ; /* [15:8] */
        unsigned int    gen4_sub_pkt2_pb2 : 8  ; /* [23:16] */
        unsigned int    gen4_sub_pkt2_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN4_SUB_PKT2_L;

/* Define the union U_GEN4_SUB_PKT2_H */
/* Define the union U_GEN4_SUB_PKT2_H,ATTR:,ADDR:192C,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen4_sub_pkt2_pb4 : 8  ; /* [7:0] */
        unsigned int    gen4_sub_pkt2_pb5 : 8  ; /* [15:8] */
        unsigned int    gen4_sub_pkt2_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_44            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN4_SUB_PKT2_H;

/* Define the union U_GEN4_SUB_PKT3_L */
/* Define the union U_GEN4_SUB_PKT3_L,ATTR:,ADDR:1930,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen4_sub_pkt3_pb0 : 8  ; /* [7:0] */
        unsigned int    gen4_sub_pkt3_pb1 : 8  ; /* [15:8] */
        unsigned int    gen4_sub_pkt3_pb2 : 8  ; /* [23:16] */
        unsigned int    gen4_sub_pkt3_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN4_SUB_PKT3_L;

/* Define the union U_GEN4_SUB_PKT3_H */
/* Define the union U_GEN4_SUB_PKT3_H,ATTR:,ADDR:1934,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen4_sub_pkt3_pb4 : 8  ; /* [7:0] */
        unsigned int    gen4_sub_pkt3_pb5 : 8  ; /* [15:8] */
        unsigned int    gen4_sub_pkt3_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_45            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN4_SUB_PKT3_H;

/* Define the union U_GEN5_PKT_HEADER */
/* Define the union U_GEN5_PKT_HEADER,ATTR:,ADDR:1938,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包头配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen5_pkt_hb0 : 8  ; /* [7:0] */
        unsigned int    gen5_pkt_hb1 : 8  ; /* [15:8] */
        unsigned int    gen5_pkt_hb2 : 8  ; /* [23:16] */
        unsigned int    rsv_46       : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN5_PKT_HEADER;

/* Define the union U_GEN5_SUB_PKT0_L */
/* Define the union U_GEN5_SUB_PKT0_L,ATTR:,ADDR:193C,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen5_sub_pkt0_pb0 : 8  ; /* [7:0] */
        unsigned int    gen5_sub_pkt0_pb1 : 8  ; /* [15:8] */
        unsigned int    gen5_sub_pkt0_pb2 : 8  ; /* [23:16] */
        unsigned int    gen5_sub_pkt0_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN5_SUB_PKT0_L;

/* Define the union U_GEN5_SUB_PKT0_H */
/* Define the union U_GEN5_SUB_PKT0_H,ATTR:,ADDR:1940,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen5_sub_pkt0_pb4 : 8  ; /* [7:0] */
        unsigned int    gen5_sub_pkt0_pb5 : 8  ; /* [15:8] */
        unsigned int    gen5_sub_pkt0_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_47            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN5_SUB_PKT0_H;

/* Define the union U_GEN5_SUB_PKT1_L */
/* Define the union U_GEN5_SUB_PKT1_L,ATTR:,ADDR:1944,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen5_sub_pkt1_pb0 : 8  ; /* [7:0] */
        unsigned int    gen5_sub_pkt1_pb1 : 8  ; /* [15:8] */
        unsigned int    gen5_sub_pkt1_pb2 : 8  ; /* [23:16] */
        unsigned int    gen5_sub_pkt1_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN5_SUB_PKT1_L;

/* Define the union U_GEN5_SUB_PKT1_H */
/* Define the union U_GEN5_SUB_PKT1_H,ATTR:,ADDR:1948,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen5_sub_pkt1_pb4 : 8  ; /* [7:0] */
        unsigned int    gen5_sub_pkt1_pb5 : 8  ; /* [15:8] */
        unsigned int    gen5_sub_pkt1_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_48            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN5_SUB_PKT1_H;

/* Define the union U_GEN5_SUB_PKT2_L */
/* Define the union U_GEN5_SUB_PKT2_L,ATTR:,ADDR:194C,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen5_sub_pkt2_pb0 : 8  ; /* [7:0] */
        unsigned int    gen5_sub_pkt2_pb1 : 8  ; /* [15:8] */
        unsigned int    gen5_sub_pkt2_pb2 : 8  ; /* [23:16] */
        unsigned int    gen5_sub_pkt2_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN5_SUB_PKT2_L;

/* Define the union U_GEN5_SUB_PKT2_H */
/* Define the union U_GEN5_SUB_PKT2_H,ATTR:,ADDR:1950,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen5_sub_pkt2_pb4 : 8  ; /* [7:0] */
        unsigned int    gen5_sub_pkt2_pb5 : 8  ; /* [15:8] */
        unsigned int    gen5_sub_pkt2_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_49            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN5_SUB_PKT2_H;

/* Define the union U_GEN5_SUB_PKT3_L */
/* Define the union U_GEN5_SUB_PKT3_L,ATTR:,ADDR:1954,INIT:00000000,MASK:ffffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen5_sub_pkt3_pb0 : 8  ; /* [7:0] */
        unsigned int    gen5_sub_pkt3_pb1 : 8  ; /* [15:8] */
        unsigned int    gen5_sub_pkt3_pb2 : 8  ; /* [23:16] */
        unsigned int    gen5_sub_pkt3_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN5_SUB_PKT3_L;

/* Define the union U_GEN5_SUB_PKT3_H */
/* Define the union U_GEN5_SUB_PKT3_H,ATTR:,ADDR:1958,INIT:00000000,MASK:ffffff,CMMNT:通用信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gen5_sub_pkt3_pb4 : 8  ; /* [7:0] */
        unsigned int    gen5_sub_pkt3_pb5 : 8  ; /* [15:8] */
        unsigned int    gen5_sub_pkt3_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_50            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GEN5_SUB_PKT3_H;

/* Define the union U_GAMUT_PKT_HEADER */
/* Define the union U_GAMUT_PKT_HEADER,ATTR:,ADDR:195C,INIT:00000000,MASK:ffffff,CMMNT:Gamut Metadata信息帧包头配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamut_pkt_hb0 : 8  ; /* [7:0] */
        unsigned int    gamut_pkt_hb1 : 8  ; /* [15:8] */
        unsigned int    gamut_pkt_hb2 : 8  ; /* [23:16] */
        unsigned int    rsv_51        : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GAMUT_PKT_HEADER;

/* Define the union U_GAMUT_SUB_PKT0_L */
/* Define the union U_GAMUT_SUB_PKT0_L,ATTR:,ADDR:1960,INIT:00000000,MASK:ffffffff,CMMNT:Gamut Metadata信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamut_sub_pkt0_pb0 : 8  ; /* [7:0] */
        unsigned int    gamut_sub_pkt0_pb1 : 8  ; /* [15:8] */
        unsigned int    gamut_sub_pkt0_pb2 : 8  ; /* [23:16] */
        unsigned int    gamut_sub_pkt0_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GAMUT_SUB_PKT0_L;

/* Define the union U_GAMUT_SUB_PKT0_H */
/* Define the union U_GAMUT_SUB_PKT0_H,ATTR:,ADDR:1964,INIT:00000000,MASK:ffffff,CMMNT:Gamut Metadata信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamut_sub_pkt0_pb4 : 8  ; /* [7:0] */
        unsigned int    gamut_sub_pkt0_pb5 : 8  ; /* [15:8] */
        unsigned int    gamut_sub_pkt0_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_52             : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GAMUT_SUB_PKT0_H;

/* Define the union U_GAMUT_SUB_PKT1_L */
/* Define the union U_GAMUT_SUB_PKT1_L,ATTR:,ADDR:1968,INIT:00000000,MASK:ffffffff,CMMNT:Gamut Metadata信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamut_sub_pkt1_pb0 : 8  ; /* [7:0] */
        unsigned int    gamut_sub_pkt1_pb1 : 8  ; /* [15:8] */
        unsigned int    gamut_sub_pkt1_pb2 : 8  ; /* [23:16] */
        unsigned int    gamut_sub_pkt1_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GAMUT_SUB_PKT1_L;

/* Define the union U_GAMUT_SUB_PKT1_H */
/* Define the union U_GAMUT_SUB_PKT1_H,ATTR:,ADDR:196C,INIT:00000000,MASK:ffffff,CMMNT:Gamut Metadata信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamut_sub_pkt1_pb4 : 8  ; /* [7:0] */
        unsigned int    gamut_sub_pkt1_pb5 : 8  ; /* [15:8] */
        unsigned int    gamut_sub_pkt1_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_53             : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GAMUT_SUB_PKT1_H;

/* Define the union U_GAMUT_SUB_PKT2_L */
/* Define the union U_GAMUT_SUB_PKT2_L,ATTR:,ADDR:1970,INIT:00000000,MASK:ffffffff,CMMNT:Gamut Metadata信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamut_sub_pkt2_pb0 : 8  ; /* [7:0] */
        unsigned int    gamut_sub_pkt2_pb1 : 8  ; /* [15:8] */
        unsigned int    gamut_sub_pkt2_pb2 : 8  ; /* [23:16] */
        unsigned int    gamut_sub_pkt2_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GAMUT_SUB_PKT2_L;

/* Define the union U_GAMUT_SUB_PKT2_H */
/* Define the union U_GAMUT_SUB_PKT2_H,ATTR:,ADDR:1974,INIT:00000000,MASK:ffffff,CMMNT:Gamut Metadata信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamut_sub_pkt2_pb4 : 8  ; /* [7:0] */
        unsigned int    gamut_sub_pkt2_pb5 : 8  ; /* [15:8] */
        unsigned int    gamut_sub_pkt2_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_54             : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GAMUT_SUB_PKT2_H;

/* Define the union U_GAMUT_SUB_PKT3_L */
/* Define the union U_GAMUT_SUB_PKT3_L,ATTR:,ADDR:1978,INIT:00000000,MASK:ffffffff,CMMNT:Gamut Metadata信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamut_sub_pkt3_pb0 : 8  ; /* [7:0] */
        unsigned int    gamut_sub_pkt3_pb1 : 8  ; /* [15:8] */
        unsigned int    gamut_sub_pkt3_pb2 : 8  ; /* [23:16] */
        unsigned int    gamut_sub_pkt3_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GAMUT_SUB_PKT3_L;

/* Define the union U_GAMUT_SUB_PKT3_H */
/* Define the union U_GAMUT_SUB_PKT3_H,ATTR:,ADDR:197C,INIT:00000000,MASK:ffffff,CMMNT:Gamut Metadata信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamut_sub_pkt3_pb4 : 8  ; /* [7:0] */
        unsigned int    gamut_sub_pkt3_pb5 : 8  ; /* [15:8] */
        unsigned int    gamut_sub_pkt3_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_55             : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GAMUT_SUB_PKT3_H;

/* Define the union U_VSIF_PKT_HEADER */
/* Define the union U_VSIF_PKT_HEADER,ATTR:,ADDR:1980,INIT:00000000,MASK:ffffff,CMMNT:Vendor-Specific信息帧包头配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsif_pkt_hb0 : 8  ; /* [7:0] */
        unsigned int    vsif_pkt_hb1 : 8  ; /* [15:8] */
        unsigned int    vsif_pkt_hb2 : 8  ; /* [23:16] */
        unsigned int    rsv_56       : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VSIF_PKT_HEADER;

/* Define the union U_VSIF_SUB_PKT0_L */
/* Define the union U_VSIF_SUB_PKT0_L,ATTR:,ADDR:1984,INIT:00000000,MASK:ffffffff,CMMNT:Vendor-Specific信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsif_sub_pkt0_pb0 : 8  ; /* [7:0] */
        unsigned int    vsif_sub_pkt0_pb1 : 8  ; /* [15:8] */
        unsigned int    vsif_sub_pkt0_pb2 : 8  ; /* [23:16] */
        unsigned int    vsif_sub_pkt0_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VSIF_SUB_PKT0_L;

/* Define the union U_VSIF_SUB_PKT0_H */
/* Define the union U_VSIF_SUB_PKT0_H,ATTR:,ADDR:1988,INIT:00000000,MASK:ffffff,CMMNT:Vendor-Specific信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsif_sub_pkt0_pb4 : 8  ; /* [7:0] */
        unsigned int    vsif_sub_pkt0_pb5 : 8  ; /* [15:8] */
        unsigned int    vsif_sub_pkt0_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_57            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VSIF_SUB_PKT0_H;

/* Define the union U_VSIF_SUB_PKT1_L */
/* Define the union U_VSIF_SUB_PKT1_L,ATTR:,ADDR:198C,INIT:00000000,MASK:ffffffff,CMMNT:Vendor-Specific信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsif_sub_pkt1_pb0 : 8  ; /* [7:0] */
        unsigned int    vsif_sub_pkt1_pb1 : 8  ; /* [15:8] */
        unsigned int    vsif_sub_pkt1_pb2 : 8  ; /* [23:16] */
        unsigned int    vsif_sub_pkt1_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VSIF_SUB_PKT1_L;

/* Define the union U_VSIF_SUB_PKT1_H */
/* Define the union U_VSIF_SUB_PKT1_H,ATTR:,ADDR:1990,INIT:00000000,MASK:ffffff,CMMNT:Vendor-Specific信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsif_sub_pkt1_pb4 : 8  ; /* [7:0] */
        unsigned int    vsif_sub_pkt1_pb5 : 8  ; /* [15:8] */
        unsigned int    vsif_sub_pkt1_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_58            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VSIF_SUB_PKT1_H;

/* Define the union U_VSIF_SUB_PKT2_L */
/* Define the union U_VSIF_SUB_PKT2_L,ATTR:,ADDR:1994,INIT:00000000,MASK:ffffffff,CMMNT:Vendor-Specific信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsif_sub_pkt2_pb0 : 8  ; /* [7:0] */
        unsigned int    vsif_sub_pkt2_pb1 : 8  ; /* [15:8] */
        unsigned int    vsif_sub_pkt2_pb2 : 8  ; /* [23:16] */
        unsigned int    vsif_sub_pkt2_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VSIF_SUB_PKT2_L;

/* Define the union U_VSIF_SUB_PKT2_H */
/* Define the union U_VSIF_SUB_PKT2_H,ATTR:,ADDR:1998,INIT:00000000,MASK:ffffff,CMMNT:Vendor-Specific信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsif_sub_pkt2_pb4 : 8  ; /* [7:0] */
        unsigned int    vsif_sub_pkt2_pb5 : 8  ; /* [15:8] */
        unsigned int    vsif_sub_pkt2_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_59            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VSIF_SUB_PKT2_H;

/* Define the union U_VSIF_SUB_PKT3_L */
/* Define the union U_VSIF_SUB_PKT3_L,ATTR:,ADDR:199C,INIT:00000000,MASK:ffffffff,CMMNT:Vendor-Specific信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsif_sub_pkt3_pb0 : 8  ; /* [7:0] */
        unsigned int    vsif_sub_pkt3_pb1 : 8  ; /* [15:8] */
        unsigned int    vsif_sub_pkt3_pb2 : 8  ; /* [23:16] */
        unsigned int    vsif_sub_pkt3_pb3 : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VSIF_SUB_PKT3_L;

/* Define the union U_VSIF_SUB_PKT3_H */
/* Define the union U_VSIF_SUB_PKT3_H,ATTR:,ADDR:19A0,INIT:00000000,MASK:ffffff,CMMNT:Vendor-Specific信息帧包体配置寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsif_sub_pkt3_pb4 : 8  ; /* [7:0] */
        unsigned int    vsif_sub_pkt3_pb5 : 8  ; /* [15:8] */
        unsigned int    vsif_sub_pkt3_pb6 : 8  ; /* [23:16] */
        unsigned int    rsv_60            : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VSIF_SUB_PKT3_H;

/* Define the union U_CEA_AVI_CFG */
/* Define the union U_CEA_AVI_CFG,ATTR:,ADDR:19A4,INIT:00000000,MASK:3,CMMNT:AVI 信息帧控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_avi_en     : 1  ; /* [0] */
        unsigned int    cea_avi_rpt_en : 1  ; /* [1] */
        unsigned int    rsv_61         : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_AVI_CFG;

/* Define the union U_CEA_SPF_CFG */
/* Define the union U_CEA_SPF_CFG,ATTR:,ADDR:19A8,INIT:00000000,MASK:3,CMMNT:Source Product Descriptor信息帧控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_spf_en     : 1  ; /* [0] */
        unsigned int    cea_spf_rpt_en : 1  ; /* [1] */
        unsigned int    rsv_62         : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_SPF_CFG;

/* Define the union U_CEA_AUD_CFG */
/* Define the union U_CEA_AUD_CFG,ATTR:,ADDR:19AC,INIT:00000000,MASK:3,CMMNT:Audio 信息帧控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_aud_en     : 1  ; /* [0] */
        unsigned int    cea_aud_rpt_en : 1  ; /* [1] */
        unsigned int    rsv_63         : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_AUD_CFG;

/* Define the union U_CEA_MPEG_CFG */
/* Define the union U_CEA_MPEG_CFG,ATTR:,ADDR:19B0,INIT:00000000,MASK:3,CMMNT:MPEG信息帧控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_mpeg_en     : 1  ; /* [0] */
        unsigned int    cea_mpeg_rpt_en : 1  ; /* [1] */
        unsigned int    rsv_64          : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_MPEG_CFG;

/* Define the union U_CEA_GEN_CFG */
/* Define the union U_CEA_GEN_CFG,ATTR:,ADDR:19B4,INIT:00000000,MASK:3,CMMNT:通用信息帧#1 控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_gen_en     : 1  ; /* [0] */
        unsigned int    cea_gen_rpt_en : 1  ; /* [1] */
        unsigned int    rsv_65         : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_GEN_CFG;

/* Define the union U_CEA_CP_CFG */
/* Define the union U_CEA_CP_CFG,ATTR:,ADDR:19B8,INIT:00000000,MASK:3,CMMNT:General control信息帧控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_cp_en     : 1  ; /* [0] */
        unsigned int    cea_cp_rpt_en : 1  ; /* [1] */
        unsigned int	cea_cp_rpt_cnt: 8  ; /* [2:7] */
        unsigned int    rsv_66        : 22  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_CP_CFG;

/* Define the union U_CEA_GEN2_CFG */
/* Define the union U_CEA_GEN2_CFG,ATTR:,ADDR:19BC,INIT:00000000,MASK:3,CMMNT:通用信息帧#2 控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_gen2_en     : 1  ; /* [0] */
        unsigned int    cea_gen2_rpt_en : 1  ; /* [1] */
        unsigned int    rsv_67          : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_GEN2_CFG;

/* Define the union U_CEA_GEN3_CFG */
/* Define the union U_CEA_GEN3_CFG,ATTR:,ADDR:19C0,INIT:00000000,MASK:3,CMMNT:通用信息帧#3 控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_gen3_en     : 1  ; /* [0] */
        unsigned int    cea_gen3_rpt_en : 1  ; /* [1] */
        unsigned int    rsv_68          : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_GEN3_CFG;

/* Define the union U_CEA_GEN4_CFG */
/* Define the union U_CEA_GEN4_CFG,ATTR:,ADDR:19C4,INIT:00000000,MASK:3,CMMNT:通用信息帧#4 控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_gen4_en     : 1  ; /* [0] */
        unsigned int    cea_gen4_rpt_en : 1  ; /* [1] */
        unsigned int    rsv_69          : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_GEN4_CFG;

/* Define the union U_CEA_GEN5_CFG */
/* Define the union U_CEA_GEN5_CFG,ATTR:,ADDR:19C8,INIT:00000000,MASK:3,CMMNT:通用信息帧#5 控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_gen5_en     : 1  ; /* [0] */
        unsigned int    cea_gen5_rpt_en : 1  ; /* [1] */
        unsigned int    rsv_70          : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_GEN5_CFG;

/* Define the union U_CEA_GAMUT_CFG */
/* Define the union U_CEA_GAMUT_CFG,ATTR:,ADDR:19CC,INIT:00000000,MASK:3,CMMNT:Gamut Metadata信息帧控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_gamut_en     : 1  ; /* [0] */
        unsigned int    cea_gamut_rpt_en : 1  ; /* [1] */
        unsigned int    rsv_71           : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_GAMUT_CFG;

/* Define the union U_CEA_VSIF_CFG */
/* Define the union U_CEA_VSIF_CFG,ATTR:,ADDR:19D0,INIT:00000000,MASK:3,CMMNT:Vendor-Specific信息帧控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_vsif_en     : 1  ; /* [0] */
        unsigned int    cea_vsif_rpt_en : 1  ; /* [1] */
        unsigned int    rsv_72          : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_VSIF_CFG;

/* Define the union U_CEA_PKT_STATE */
/* Define the union U_CEA_PKT_STATE,ATTR:,ADDR:19E0,INIT:00000000,MASK:fff,CMMNT:保留寄存器*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cea_avi_state   : 1  ; /* [0] */
        unsigned int    cea_aud_state   : 1  ; /* [1] */
        unsigned int    cea_cp_state    : 1  ; /* [2] */
        unsigned int    cea_gen_state   : 1  ; /* [3] */
        unsigned int    cea_gen2_state  : 1  ; /* [4] */
        unsigned int    cea_gen3_state  : 1  ; /* [5] */
        unsigned int    cea_gen4_state  : 1  ; /* [6] */
        unsigned int    cea_gen5_state  : 1  ; /* [7] */
        unsigned int    cea_spd_state   : 1  ; /* [8] */
        unsigned int    cea_mpeg_state  : 1  ; /* [9] */
        unsigned int    cea_gamut_state : 1  ; /* [10] */
        unsigned int    cea_vsif_state  : 1  ; /* [11] */
        unsigned int    rsv_73          : 20  ; /* [31:12] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CEA_PKT_STATE;

/* Define the union U_AVMIXER_CONFIG */
/* Define the union U_AVMIXER_CONFIG,ATTR:,ADDR:1A08,INIT:00000000,MASK:ff,CMMNT:AVMIXER控制寄存器。*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hdmi_mode           : 1  ; /* [0] */
        unsigned int    dc_pkt_en           : 1  ; /* [1] */
        unsigned int    null_pkt_en         : 1  ; /* [2] */
        unsigned int    null_pkt_en_vs_high : 1  ; /* [3] */
        unsigned int    intr_encryption     : 1  ; /* [4] */
        unsigned int    pb_ovr_dc_pkt_en    : 1  ; /* [5] */
        unsigned int    pb_priotity_ctl     : 1  ; /* [6] */
        unsigned int    pkt_bypass_mode     : 1  ; /* [7] */
        unsigned int    rsv_74              : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AVMIXER_CONFIG;

/* Define the union U_CP_PKT_AVMUTE */
/* Define the union U_CP_PKT_AVMUTE,ATTR:,ADDR:1A0C,INIT:00000000,MASK:3,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cp_set_avmute : 1  ; /* [0] */
        unsigned int    cp_clr_avmute : 1  ; /* [1] */
        unsigned int    rsv_75        : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CP_PKT_AVMUTE;

/* Define the union U_VIDEO_BLANK_CFG */
/* Define the union U_VIDEO_BLANK_CFG,ATTR:,ADDR:1A10,INIT:00000000,MASK:ffffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    video_blank : 24  ; /* [23:0] */
        unsigned int    rsv_76      : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VIDEO_BLANK_CFG;

/* Define the union U_TMDS_BIST_CTRL */
/* Define the union U_TMDS_BIST_CTRL,ATTR:,ADDR:1A54,INIT:00000000,MASK:3fff,CMMNT:保留寄存器*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_tbist_en         : 1  ; /* [0] */
        unsigned int    reg_tbist_syn_pol    : 2  ; /* [2:1] */
        unsigned int    reg_tbist_timing_sel : 6  ; /* [8:3] */
        unsigned int    reg_tbist_patt_sel   : 5  ; /* [13:9] */
        unsigned int    rsv_77               : 18  ; /* [31:14] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TMDS_BIST_CTRL;

/* Define the union U_HDMI_ENC_CTRL */
/* Define the union U_HDMI_ENC_CTRL,ATTR:,ADDR:1A60,INIT:00000000,MASK:ff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    enc_hdmi2_on   : 1  ; /* [0] */
        unsigned int    enc_scr_on     : 1  ; /* [1] */
        unsigned int    enc_scr_md     : 1  ; /* [2] */
        unsigned int    enc_hdmi_val   : 1  ; /* [3] */
        unsigned int    enc_hdmi_ovr   : 1  ; /* [4] */
        unsigned int    enc_bypass     : 1  ; /* [5] */
        unsigned int    enc_ck_div_sel : 2  ; /* [7:6] */
        unsigned int    rsv_78         : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HDMI_ENC_CTRL;

/* Define the union U_ENC_CK_SHARP */
/* Define the union U_ENC_CK_SHARP,ATTR:,ADDR:1A64,INIT:000FFFE0,MASK:3fffffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    enc_ck_sharp0 : 10  ; /* [9:0] */
        unsigned int    enc_ck_sharp1 : 10  ; /* [19:10] */
        unsigned int    enc_ck_sharp2 : 10  ; /* [29:20] */
        unsigned int    rsv_79        : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENC_CK_SHARP;


//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_TX_PACK_FIFO_CTRL TX_PACK_FIFO_CTRL ; /* 1800 */
    volatile U_TX_PACK_FIFO_ST   TX_PACK_FIFO_ST   ; /* 1804 */
    volatile U_PCLK_REFER_CNT       PCLK_REFER_CNT       ; /* 1808 */
    volatile U_TCLK_LOWER_THRESHOLD TCLK_LOWER_THRESHOLD ; /* 180C */
    volatile U_TCLK_UPPER_THRESHOLD TCLK_UPPER_THRESHOLD ; /* 1810 */
	unsigned int                      Reserved_0[1];
    volatile U_AVI_PKT_HEADER    AVI_PKT_HEADER    ; /* 1818 */
    volatile U_AVI_SUB_PKT0_L    AVI_SUB_PKT0_L    ; /* 181C */
    volatile U_AVI_SUB_PKT0_H    AVI_SUB_PKT0_H    ; /* 1820 */
    volatile U_AVI_SUB_PKT1_L    AVI_SUB_PKT1_L    ; /* 1824 */
    volatile U_AVI_SUB_PKT1_H    AVI_SUB_PKT1_H    ; /* 1828 */
    volatile U_AVI_SUB_PKT2_L    AVI_SUB_PKT2_L    ; /* 182C */
    volatile U_AVI_SUB_PKT2_H    AVI_SUB_PKT2_H    ; /* 1830 */
    volatile U_AVI_SUB_PKT3_L    AVI_SUB_PKT3_L    ; /* 1834 */
    volatile U_AVI_SUB_PKT3_H    AVI_SUB_PKT3_H    ; /* 1838 */
    volatile U_AIF_PKT_HEADER    AIF_PKT_HEADER    ; /* 183C */
    volatile U_AIF_SUB_PKT0_L    AIF_SUB_PKT0_L    ; /* 1840 */
    volatile U_AIF_SUB_PKT0_H    AIF_SUB_PKT0_H    ; /* 1844 */
    volatile U_AIF_SUB_PKT1_L    AIF_SUB_PKT1_L    ; /* 1848 */
    volatile U_AIF_SUB_PKT1_H    AIF_SUB_PKT1_H    ; /* 184C */
    volatile U_AIF_SUB_PKT2_L    AIF_SUB_PKT2_L    ; /* 1850 */
    volatile U_AIF_SUB_PKT2_H    AIF_SUB_PKT2_H    ; /* 1854 */
    volatile U_AIF_SUB_PKT3_L    AIF_SUB_PKT3_L    ; /* 1858 */
    volatile U_AIF_SUB_PKT3_H    AIF_SUB_PKT3_H    ; /* 185C */
    volatile U_SPIF_PKT_HEADER   SPIF_PKT_HEADER   ; /* 1860 */
    volatile U_SPIF_SUB_PKT0_L   SPIF_SUB_PKT0_L   ; /* 1864 */
    volatile U_SPIF_SUB_PKT0_H   SPIF_SUB_PKT0_H   ; /* 1868 */
    volatile U_SPIF_SUB_PKT1_L   SPIF_SUB_PKT1_L   ; /* 186C */
    volatile U_SPIF_SUB_PKT1_H   SPIF_SUB_PKT1_H   ; /* 1870 */
    volatile U_SPIF_SUB_PKT2_L   SPIF_SUB_PKT2_L   ; /* 1874 */
    volatile U_SPIF_SUB_PKT2_H   SPIF_SUB_PKT2_H   ; /* 1878 */
    volatile U_SPIF_SUB_PKT3_L   SPIF_SUB_PKT3_L   ; /* 187C */
    volatile U_SPIF_SUB_PKT3_H   SPIF_SUB_PKT3_H   ; /* 1880 */
    volatile U_MPEG_PKT_HEADER   MPEG_PKT_HEADER   ; /* 1884 */
    volatile U_MPEG_SUB_PKT0_L   MPEG_SUB_PKT0_L   ; /* 1888 */
    volatile U_MPEG_SUB_PKT0_H   MPEG_SUB_PKT0_H   ; /* 188C */
    volatile U_MPEG_SUB_PKT1_L   MPEG_SUB_PKT1_L   ; /* 1890 */
    volatile U_MPEG_SUB_PKT1_H   MPEG_SUB_PKT1_H   ; /* 1894 */
    volatile U_MPEG_SUB_PKT2_L   MPEG_SUB_PKT2_L   ; /* 1898 */
    volatile U_MPEG_SUB_PKT2_H   MPEG_SUB_PKT2_H   ; /* 189C */
    volatile U_MPEG_SUB_PKT3_L   MPEG_SUB_PKT3_L   ; /* 18A0 */
    volatile U_MPEG_SUB_PKT3_H   MPEG_SUB_PKT3_H   ; /* 18A4 */
    volatile U_GEN_PKT_HEADER    GEN_PKT_HEADER    ; /* 18A8 */
    volatile U_GEN_SUB_PKT0_L    GEN_SUB_PKT0_L    ; /* 18AC */
    volatile U_GEN_SUB_PKT0_H    GEN_SUB_PKT0_H    ; /* 18B0 */
    volatile U_GEN_SUB_PKT1_L    GEN_SUB_PKT1_L    ; /* 18B4 */
    volatile U_GEN_SUB_PKT1_H    GEN_SUB_PKT1_H    ; /* 18B8 */
    volatile U_GEN_SUB_PKT2_L    GEN_SUB_PKT2_L    ; /* 18BC */
    volatile U_GEN_SUB_PKT2_H    GEN_SUB_PKT2_H    ; /* 18C0 */
    volatile U_GEN_SUB_PKT3_L    GEN_SUB_PKT3_L    ; /* 18C4 */
    volatile U_GEN_SUB_PKT3_H    GEN_SUB_PKT3_H    ; /* 18C8 */
    volatile U_GEN2_PKT_HEADER   GEN2_PKT_HEADER   ; /* 18CC */
    volatile U_GEN2_SUB_PKT0_L   GEN2_SUB_PKT0_L   ; /* 18D0 */
    volatile U_GEN2_SUB_PKT0_H   GEN2_SUB_PKT0_H   ; /* 18D4 */
    volatile U_GEN2_SUB_PKT1_L   GEN2_SUB_PKT1_L   ; /* 18D8 */
    volatile U_GEN2_SUB_PKT1_H   GEN2_SUB_PKT1_H   ; /* 18DC */
    volatile U_GEN2_SUB_PKT2_L   GEN2_SUB_PKT2_L   ; /* 18E0 */
    volatile U_GEN2_SUB_PKT2_H   GEN2_SUB_PKT2_H   ; /* 18E4 */
    volatile U_GEN2_SUB_PKT3_L   GEN2_SUB_PKT3_L   ; /* 18E8 */
    volatile U_GEN2_SUB_PKT3_H   GEN2_SUB_PKT3_H   ; /* 18EC */
    volatile U_GEN3_PKT_HEADER   GEN3_PKT_HEADER   ; /* 18F0 */
    volatile U_GEN3_SUB_PKT0_L   GEN3_SUB_PKT0_L   ; /* 18F4 */
    volatile U_GEN3_SUB_PKT0_H   GEN3_SUB_PKT0_H   ; /* 18F8 */
    volatile U_GEN3_SUB_PKT1_L   GEN3_SUB_PKT1_L   ; /* 18FC */
    volatile U_GEN3_SUB_PKT1_H   GEN3_SUB_PKT1_H   ; /* 1900 */
    volatile U_GEN3_SUB_PKT2_L   GEN3_SUB_PKT2_L   ; /* 1904 */
    volatile U_GEN3_SUB_PKT2_H   GEN3_SUB_PKT2_H   ; /* 1908 */
    volatile U_GEN3_SUB_PKT3_L   GEN3_SUB_PKT3_L   ; /* 190C */
    volatile U_GEN3_SUB_PKT3_H   GEN3_SUB_PKT3_H   ; /* 1910 */
    volatile U_GEN4_PKT_HEADER   GEN4_PKT_HEADER   ; /* 1914 */
    volatile U_GEN4_SUB_PKT0_L   GEN4_SUB_PKT0_L   ; /* 1918 */
    volatile U_GEN4_SUB_PKT0_H   GEN4_SUB_PKT0_H   ; /* 191C */
    volatile U_GEN4_SUB_PKT1_L   GEN4_SUB_PKT1_L   ; /* 1920 */
    volatile U_GEN4_SUB_PKT1_H   GEN4_SUB_PKT1_H   ; /* 1924 */
    volatile U_GEN4_SUB_PKT2_L   GEN4_SUB_PKT2_L   ; /* 1928 */
    volatile U_GEN4_SUB_PKT2_H   GEN4_SUB_PKT2_H   ; /* 192C */
    volatile U_GEN4_SUB_PKT3_L   GEN4_SUB_PKT3_L   ; /* 1930 */
    volatile U_GEN4_SUB_PKT3_H   GEN4_SUB_PKT3_H   ; /* 1934 */
    volatile U_GEN5_PKT_HEADER   GEN5_PKT_HEADER   ; /* 1938 */
    volatile U_GEN5_SUB_PKT0_L   GEN5_SUB_PKT0_L   ; /* 193C */
    volatile U_GEN5_SUB_PKT0_H   GEN5_SUB_PKT0_H   ; /* 1940 */
    volatile U_GEN5_SUB_PKT1_L   GEN5_SUB_PKT1_L   ; /* 1944 */
    volatile U_GEN5_SUB_PKT1_H   GEN5_SUB_PKT1_H   ; /* 1948 */
    volatile U_GEN5_SUB_PKT2_L   GEN5_SUB_PKT2_L   ; /* 194C */
    volatile U_GEN5_SUB_PKT2_H   GEN5_SUB_PKT2_H   ; /* 1950 */
    volatile U_GEN5_SUB_PKT3_L   GEN5_SUB_PKT3_L   ; /* 1954 */
    volatile U_GEN5_SUB_PKT3_H   GEN5_SUB_PKT3_H   ; /* 1958 */
    volatile U_GAMUT_PKT_HEADER  GAMUT_PKT_HEADER  ; /* 195C */
    volatile U_GAMUT_SUB_PKT0_L  GAMUT_SUB_PKT0_L  ; /* 1960 */
    volatile U_GAMUT_SUB_PKT0_H  GAMUT_SUB_PKT0_H  ; /* 1964 */
    volatile U_GAMUT_SUB_PKT1_L  GAMUT_SUB_PKT1_L  ; /* 1968 */
    volatile U_GAMUT_SUB_PKT1_H  GAMUT_SUB_PKT1_H  ; /* 196C */
    volatile U_GAMUT_SUB_PKT2_L  GAMUT_SUB_PKT2_L  ; /* 1970 */
    volatile U_GAMUT_SUB_PKT2_H  GAMUT_SUB_PKT2_H  ; /* 1974 */
    volatile U_GAMUT_SUB_PKT3_L  GAMUT_SUB_PKT3_L  ; /* 1978 */
    volatile U_GAMUT_SUB_PKT3_H  GAMUT_SUB_PKT3_H  ; /* 197C */
    volatile U_VSIF_PKT_HEADER   VSIF_PKT_HEADER   ; /* 1980 */
    volatile U_VSIF_SUB_PKT0_L   VSIF_SUB_PKT0_L   ; /* 1984 */
    volatile U_VSIF_SUB_PKT0_H   VSIF_SUB_PKT0_H   ; /* 1988 */
    volatile U_VSIF_SUB_PKT1_L   VSIF_SUB_PKT1_L   ; /* 198C */
    volatile U_VSIF_SUB_PKT1_H   VSIF_SUB_PKT1_H   ; /* 1990 */
    volatile U_VSIF_SUB_PKT2_L   VSIF_SUB_PKT2_L   ; /* 1994 */
    volatile U_VSIF_SUB_PKT2_H   VSIF_SUB_PKT2_H   ; /* 1998 */
    volatile U_VSIF_SUB_PKT3_L   VSIF_SUB_PKT3_L   ; /* 199C */
    volatile U_VSIF_SUB_PKT3_H   VSIF_SUB_PKT3_H   ; /* 19A0 */
    volatile U_CEA_AVI_CFG       CEA_AVI_CFG       ; /* 19A4 */
    volatile U_CEA_SPF_CFG       CEA_SPF_CFG       ; /* 19A8 */
    volatile U_CEA_AUD_CFG       CEA_AUD_CFG       ; /* 19AC */
    volatile U_CEA_MPEG_CFG      CEA_MPEG_CFG      ; /* 19B0 */
    volatile U_CEA_GEN_CFG       CEA_GEN_CFG       ; /* 19B4 */
    volatile U_CEA_CP_CFG        CEA_CP_CFG        ; /* 19B8 */
    volatile U_CEA_GEN2_CFG      CEA_GEN2_CFG      ; /* 19BC */
    volatile U_CEA_GEN3_CFG      CEA_GEN3_CFG      ; /* 19C0 */
    volatile U_CEA_GEN4_CFG      CEA_GEN4_CFG      ; /* 19C4 */
    volatile U_CEA_GEN5_CFG      CEA_GEN5_CFG      ; /* 19C8 */
    volatile U_CEA_GAMUT_CFG     CEA_GAMUT_CFG     ; /* 19CC */
    volatile U_CEA_VSIF_CFG      CEA_VSIF_CFG      ; /* 19D0 */
    unsigned int                          Reserved_1[3];
    volatile U_CEA_PKT_STATE     CEA_PKT_STATE     ; /* 19E0 */
    unsigned int                          Reserved_2[9];
    volatile U_AVMIXER_CONFIG    AVMIXER_CONFIG    ; /* 1A08 */
    volatile U_CP_PKT_AVMUTE     CP_PKT_AVMUTE     ; /* 1A0C */
    volatile U_VIDEO_BLANK_CFG   VIDEO_BLANK_CFG   ; /* 1A10 */
	unsigned int                          Reserved_3[16];
    volatile U_TMDS_BIST_CTRL    TMDS_BIST_CTRL    ; /* 1A54 */
	unsigned int                          Reserved_4[2];
    volatile U_HDMI_ENC_CTRL     HDMI_ENC_CTRL     ; /* 1A60 */
    volatile U_ENC_CK_SHARP      ENC_CK_SHARP      ; /* 1A64 */

} S_tx_hdmi_reg_REGS_TYPE;

/* Declare the struct pointor of the module tx_hdmi_reg */
//extern volatile S_tx_hdmi_reg_REGS_TYPE *goptx_hdmi_regAllReg;
int HDMI_TX_S_tx_hdmi_reg_REGS_TYPE_Init(void);
int HDMI_TX_S_tx_hdmi_reg_REGS_TYPE_DeInit(void);

/* Declare the functions that set the member value */
int HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeSet(unsigned int utmds_pack_mode);
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_auto_rst_enSet(unsigned int ureg_fifo_auto_rst_en);
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_manu_rstSet(unsigned int ureg_fifo_manu_rst);
int HDMI_TX_PACK_FIFO_CTRL_reg_clock_det_enSet(unsigned int ureg_clock_det_en);
int HDMI_TX_PACK_FIFO_CTRL_reg_ext_tmds_paraSet(unsigned int ureg_ext_tmds_para);
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_delay_cntSet(unsigned int ureg_fifo_delay_cnt);
int HDMI_TX_PACK_FIFO_ST_pclk2tclk_stableSet(unsigned int upclk2tclk_stable);
int HDMI_PCLK_REFER_CNT_reg_pclk_refer_cntSet(unsigned int ureg_pclk_refer_cnt);
int HDMI_TCLK_LOWER_THRESHOLD_reg_tcnt_lower_thresholdSet(unsigned int ureg_tcnt_lower_threshold);
int HDMI_TCLK_UPPER_THRESHOLD_reg_tcnt_upper_thresholdSet(unsigned int ureg_tcnt_upper_threshold);
/* Declare the AVI Infoframe Set */
int HDMI_AVI_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2);
int HDMI_AVI_SUB_PKT0_L_avi_pkt0_pbSet(unsigned int uavi_pkt0_pb0,unsigned int uavi_pkt0_pb1,unsigned int uavi_pkt0_pb2,unsigned int uavi_pkt0_pb3);
int HDMI_AVI_SUB_PKT0_H_avi_pkt0_pbSet(unsigned int uavi_pkt0_pb4,unsigned int uavi_pkt0_pb5,unsigned int uavi_pkt0_pb6);
int HDMI_AVI_SUB_PKT1_L_avi_pkt1_pbSet(unsigned int uavi_pkt1_pb0,unsigned int uavi_pkt1_pb1,unsigned int uavi_pkt1_pb2,unsigned int uavi_pkt1_pb3);
int HDMI_AVI_SUB_PKT1_H_avi_pkt1_pbSet(unsigned int uavi_pkt1_pb4,unsigned int uavi_pkt1_pb5,unsigned int uavi_pkt1_pb6);
int HDMI_AVI_SUB_PKT2_L_avi_pkt2_pbSet(unsigned int uavi_pkt2_pb0,unsigned int uavi_pkt2_pb1,unsigned int uavi_pkt2_pb2,unsigned int uavi_pkt2_pb3);
int HDMI_AVI_SUB_PKT2_H_avi_pkt2_pbSet(unsigned int uavi_pkt2_pb4,unsigned int uavi_pkt2_pb5,unsigned int uavi_pkt2_pb6);
int HDMI_AVI_SUB_PKT3_L_avi_pkt3_pbSet(unsigned int uavi_pkt3_pb0,unsigned int uavi_pkt3_pb1,unsigned int uavi_pkt3_pb2,unsigned int uavi_pkt3_pb3);
int HDMI_AVI_SUB_PKT3_H_avi_pkt3_pbSet(unsigned int uavi_pkt3_pb4,unsigned int uavi_pkt3_pb5,unsigned int uavi_pkt3_pb6);
/* Declare the AVI Infoframe Get */
int HDMI_AVI_PKT_HEADER_hbGet(U_AVI_PKT_HEADER* u_avi_pkt_header);
int HDMI_AVI_SUB_PKT0_L_avi_pkt0_pbGet(U_AVI_SUB_PKT0_L* u_avi_sub_pkt0_l);
int HDMI_AVI_SUB_PKT0_H_avi_pkt0_pbGet(U_AVI_SUB_PKT0_H* u_avi_sub_pkt0_h);
int HDMI_AVI_SUB_PKT1_L_avi_pkt1_pbGet(U_AVI_SUB_PKT1_L* u_avi_sub_pkt1_l);
int HDMI_AVI_SUB_PKT1_H_avi_pkt1_pbGet(U_AVI_SUB_PKT1_H* u_avi_sub_pkt1_h);
int HDMI_AVI_SUB_PKT2_L_avi_pkt2_pbGet(U_AVI_SUB_PKT2_L* u_avi_sub_pkt2_l);
int HDMI_AVI_SUB_PKT2_H_avi_pkt2_pbGet(U_AVI_SUB_PKT2_H* u_avi_sub_pkt2_h);
int HDMI_AVI_SUB_PKT3_L_avi_pkt3_pbGet(U_AVI_SUB_PKT3_L* u_avi_sub_pkt3_l);
int HDMI_AVI_SUB_PKT3_H_avi_pkt3_pbGet(U_AVI_SUB_PKT3_H* u_avi_sub_pkt3_h);

/* Declare the Audio Infoframe Set */
int HDMI_AUDIO_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2);
int HDMI_AUDIO_SUB_PKT0_L_audio_pkt0_pbSet(unsigned int uavi_pkt0_pb0,unsigned int uavi_pkt0_pb1,unsigned int uavi_pkt0_pb2,unsigned int uavi_pkt0_pb3);
int HDMI_AUDIO_SUB_PKT0_H_audio_pkt0_pbSet(unsigned int uavi_pkt0_pb4,unsigned int uavi_pkt0_pb5,unsigned int uavi_pkt0_pb6);
int HDMI_AUDIO_SUB_PKT1_L_audio_pkt1_pbSet(unsigned int uavi_pkt1_pb0,unsigned int uavi_pkt1_pb1,unsigned int uavi_pkt1_pb2,unsigned int uavi_pkt1_pb3);
int HDMI_AUDIO_SUB_PKT1_H_audio_pkt1_pbSet(unsigned int uavi_pkt1_pb4,unsigned int uavi_pkt1_pb5,unsigned int uavi_pkt1_pb6);
int HDMI_AUDIO_SUB_PKT2_L_audio_pkt2_pbSet(unsigned int uavi_pkt2_pb0,unsigned int uavi_pkt2_pb1,unsigned int uavi_pkt2_pb2,unsigned int uavi_pkt2_pb3);
int HDMI_AUDIO_SUB_PKT2_H_audio_pkt2_pbSet(unsigned int uavi_pkt2_pb4,unsigned int uavi_pkt2_pb5,unsigned int uavi_pkt2_pb6);
int HDMI_AUDIO_SUB_PKT3_L_audio_pkt3_pbSet(unsigned int uavi_pkt3_pb0,unsigned int uavi_pkt3_pb1,unsigned int uavi_pkt3_pb2,unsigned int uavi_pkt3_pb3);
int HDMI_AUDIO_SUB_PKT3_H_audio_pkt3_pbSet(unsigned int uavi_pkt3_pb4,unsigned int uavi_pkt3_pb5,unsigned int uavi_pkt3_pb6);
/* Declare the Audio Infoframe Get */
int HDMI_AIF_PKT_HEADER_hbGet(U_AIF_PKT_HEADER* u_aif_pkt_header);
int HDMI_AIF_SUB_PKT0_L_aif_pkt0_pbGet(U_AIF_SUB_PKT0_L* u_aif_sub_pkt0_l);
int HDMI_AIF_SUB_PKT0_H_aif_pkt0_pbGet(U_AIF_SUB_PKT0_H* u_aif_sub_pkt0_h);
int HDMI_AIF_SUB_PKT1_L_aif_pkt1_pbGet(U_AIF_SUB_PKT1_L* u_aif_sub_pkt1_l);
int HDMI_AIF_SUB_PKT1_H_aif_pkt1_pbGet(U_AIF_SUB_PKT1_H* u_aif_sub_pkt1_h);
int HDMI_AIF_SUB_PKT2_L_aif_pkt2_pbGet(U_AIF_SUB_PKT2_L* u_aif_sub_pkt2_l);
int HDMI_AIF_SUB_PKT2_H_aif_pkt2_pbGet(U_AIF_SUB_PKT2_H* u_aif_sub_pkt2_h);
int HDMI_AIF_SUB_PKT3_L_aif_pkt3_pbGet(U_AIF_SUB_PKT3_L* u_aif_sub_pkt3_l);
int HDMI_AIF_SUB_PKT3_H_aif_pkt3_pbGet(U_AIF_SUB_PKT3_H* u_aif_sub_pkt3_h);

/* Declare the SPD Infoframe Set */
int HDMI_SPIF_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2);
int HDMI_SPIF_SUB_PKT0_L_spif_pkt0_pbSet(unsigned int uspif_pkt0_pb0,unsigned int uspif_pkt0_pb1,unsigned int uspif_pkt0_pb2,unsigned int uspif_pkt0_pb3);
int HDMI_SPIF_SUB_PKT0_H_spif_pkt0_pbSet(unsigned int uspif_pkt0_pb4,unsigned int uspif_pkt0_pb5,unsigned int uspif_pkt0_pb6);
int HDMI_SPIF_SUB_PKT1_L_spif_pkt1_pbSet(unsigned int uspif_pkt1_pb0,unsigned int uspif_pkt1_pb1,unsigned int uspif_pkt1_pb2,unsigned int uspif_pkt1_pb3);
int HDMI_SPIF_SUB_PKT1_H_spif_pkt1_pbSet(unsigned int uspif_pkt1_pb4,unsigned int uspif_pkt1_pb5,unsigned int uspif_pkt1_pb6);
int HDMI_SPIF_SUB_PKT2_L_spif_pkt2_pbSet(unsigned int uspif_pkt2_pb0,unsigned int uspif_pkt2_pb1,unsigned int uspif_pkt2_pb2,unsigned int uspif_pkt2_pb3);
int HDMI_SPIF_SUB_PKT2_H_spif_pkt2_pbSet(unsigned int uspif_pkt2_pb4,unsigned int uspif_pkt2_pb5,unsigned int uspif_pkt2_pb6);
int HDMI_SPIF_SUB_PKT3_L_spif_pkt3_pbSet(unsigned int uspif_pkt3_pb0,unsigned int uspif_pkt3_pb1,unsigned int uspif_pkt3_pb2,unsigned int uspif_pkt3_pb3);
int HDMI_SPIF_SUB_PKT3_H_spif_pkt3_pbSet(unsigned int uspif_pkt3_pb4,unsigned int uspif_pkt3_pb5,unsigned int uspif_pkt3_pb6);
/* Declare the SPD Infoframe Get */
int HDMI_SPIF_PKT_HEADER_hbGet(U_SPIF_PKT_HEADER* u_spif_pkt_header);
int HDMI_SPIF_SUB_PKT0_L_spif_pkt0_pbGet(U_SPIF_SUB_PKT0_L* u_spif_sub_pkt0_l);
int HDMI_SPIF_SUB_PKT0_H_spif_pkt0_pbGet(U_SPIF_SUB_PKT0_H* u_spif_sub_pkt0_h);
int HDMI_SPIF_SUB_PKT1_L_spif_pkt1_pbGet(U_SPIF_SUB_PKT1_L* u_spif_sub_pkt1_l);
int HDMI_SPIF_SUB_PKT1_H_spif_pkt1_pbGet(U_SPIF_SUB_PKT1_H* u_spif_sub_pkt1_h);
int HDMI_SPIF_SUB_PKT2_L_spif_pkt2_pbGet(U_SPIF_SUB_PKT2_L* u_spif_sub_pkt2_l);
int HDMI_SPIF_SUB_PKT2_H_spif_pkt2_pbGet(U_SPIF_SUB_PKT2_H* u_spif_sub_pkt2_h);
int HDMI_SPIF_SUB_PKT3_L_spif_pkt3_pbGet(U_SPIF_SUB_PKT3_L* u_spif_sub_pkt3_l);
int HDMI_SPIF_SUB_PKT3_H_spif_pkt3_pbGet(U_SPIF_SUB_PKT3_H* u_spif_sub_pkt3_h);

/* Declare the MPEG Infoframe Set */
int HDMI_MPEG_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2);
int HDMI_MPEG_SUB_PKT0_L_mpeg_pkt0_pbSet(unsigned int umpeg_pkt0_pb0,unsigned int umpeg_pkt0_pb1,unsigned int umpeg_pkt0_pb2,unsigned int umpeg_pkt0_pb3);
int HDMI_MPEG_SUB_PKT0_H_mpeg_pkt0_pbSet(unsigned int umpeg_pkt0_pb4,unsigned int umpeg_pkt0_pb5,unsigned int umpeg_pkt0_pb6);
int HDMI_MPEG_SUB_PKT1_L_mpeg_pkt1_pbSet(unsigned int umpeg_pkt1_pb0,unsigned int umpeg_pkt1_pb1,unsigned int umpeg_pkt1_pb2,unsigned int umpeg_pkt1_pb3);
int HDMI_MPEG_SUB_PKT1_H_mpeg_pkt1_pbSet(unsigned int umpeg_pkt1_pb4,unsigned int umpeg_pkt1_pb5,unsigned int umpeg_pkt1_pb6);
int HDMI_MPEG_SUB_PKT2_L_mpeg_pkt2_pbSet(unsigned int umpeg_pkt2_pb0,unsigned int umpeg_pkt2_pb1,unsigned int umpeg_pkt2_pb2,unsigned int umpeg_pkt2_pb3);
int HDMI_MPEG_SUB_PKT2_H_mpeg_pkt2_pbSet(unsigned int umpeg_pkt2_pb4,unsigned int umpeg_pkt2_pb5,unsigned int umpeg_pkt2_pb6);
int HDMI_MPEG_SUB_PKT3_L_mpeg_pkt3_pbSet(unsigned int umpeg_pkt3_pb0,unsigned int umpeg_pkt3_pb1,unsigned int umpeg_pkt3_pb2,unsigned int umpeg_pkt3_pb3);
int HDMI_MPEG_SUB_PKT3_H_mpeg_pkt3_pbSet(unsigned int umpeg_pkt3_pb4,unsigned int umpeg_pkt3_pb5,unsigned int umpeg_pkt3_pb6);
/* Declare the MPEG Infoframe Get */
int HDMI_MPEG_PKT_HEADER_hbGet(U_MPEG_PKT_HEADER* u_mpeg_pkt_header);
int HDMI_MPEG_SUB_PKT0_L_mpeg_pkt0_pbGet(U_MPEG_SUB_PKT0_L* u_mpeg_sub_pkt0_l);
int HDMI_MPEG_SUB_PKT0_H_mpeg_pkt0_pbGet(U_MPEG_SUB_PKT0_H* u_mpeg_sub_pkt0_h);
int HDMI_MPEG_SUB_PKT1_L_mpeg_pkt1_pbGet(U_MPEG_SUB_PKT1_L* u_mpeg_sub_pkt1_l);
int HDMI_MPEG_SUB_PKT1_H_mpeg_pkt1_pbGet(U_MPEG_SUB_PKT1_H* u_mpeg_sub_pkt1_h);
int HDMI_MPEG_SUB_PKT2_L_mpeg_pkt2_pbGet(U_MPEG_SUB_PKT2_L* u_mpeg_sub_pkt2_l);
int HDMI_MPEG_SUB_PKT2_H_mpeg_pkt2_pbGet(U_MPEG_SUB_PKT2_H* u_mpeg_sub_pkt2_h);
int HDMI_MPEG_SUB_PKT3_L_mpeg_pkt3_pbGet(U_MPEG_SUB_PKT3_L* u_mpeg_sub_pkt3_l);
int HDMI_MPEG_SUB_PKT3_H_mpeg_pkt3_pbGet(U_MPEG_SUB_PKT3_H* u_mpeg_sub_pkt3_h);
/* Declare the gen Infoframe Set */
int HDMI_GEN_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2);
int HDMI_GEN_SUB_PKT0_L_gen_pkt0_pbSet(unsigned int ugen_pkt0_pb0,unsigned int ugen_pkt0_pb1,unsigned int ugen_pkt0_pb2,unsigned int ugen_pkt0_pb3);
int HDMI_GEN_SUB_PKT0_H_gen_pkt0_pbSet(unsigned int ugen_pkt0_pb4,unsigned int ugen_pkt0_pb5,unsigned int ugen_pkt0_pb6);
int HDMI_GEN_SUB_PKT1_L_gen_pkt1_pbSet(unsigned int ugen_pkt1_pb0,unsigned int ugen_pkt1_pb1,unsigned int ugen_pkt1_pb2,unsigned int ugen_pkt1_pb3);
int HDMI_GEN_SUB_PKT1_H_gen_pkt1_pbSet(unsigned int ugen_pkt1_pb4,unsigned int ugen_pkt1_pb5,unsigned int ugen_pkt1_pb6);
int HDMI_GEN_SUB_PKT2_L_gen_pkt2_pbSet(unsigned int ugen_pkt2_pb0,unsigned int ugen_pkt2_pb1,unsigned int ugen_pkt2_pb2,unsigned int ugen_pkt2_pb3);
int HDMI_GEN_SUB_PKT2_H_gen_pkt2_pbSet(unsigned int ugen_pkt2_pb4,unsigned int ugen_pkt2_pb5,unsigned int ugen_pkt2_pb6);
int HDMI_GEN_SUB_PKT3_L_gen_pkt3_pbSet(unsigned int ugen_pkt3_pb0,unsigned int ugen_pkt3_pb1,unsigned int ugen_pkt3_pb2,unsigned int ugen_pkt3_pb3);
int HDMI_GEN_SUB_PKT3_H_gen_pkt3_pbSet(unsigned int ugen_pkt3_pb4,unsigned int ugen_pkt3_pb5,unsigned int ugen_pkt3_pb6);
/* Declare the gen Infoframe Get */
int HDMI_GEN_PKT_HEADER_hbGet(U_GEN_PKT_HEADER* u_gen_pkt_header);
int HDMI_GEN_SUB_PKT0_L_gen_pkt0_pbGet(U_GEN_SUB_PKT0_L* u_gen_sub_pkt0_l);
int HDMI_GEN_SUB_PKT0_H_gen_pkt0_pbGet(U_GEN_SUB_PKT0_H* u_gen_sub_pkt0_h);
int HDMI_GEN_SUB_PKT1_L_gen_pkt1_pbGet(U_GEN_SUB_PKT1_L* u_gen_sub_pkt1_l);
int HDMI_GEN_SUB_PKT1_H_gen_pkt1_pbGet(U_GEN_SUB_PKT1_H* u_gen_sub_pkt1_h);
int HDMI_GEN_SUB_PKT2_L_gen_pkt2_pbGet(U_GEN_SUB_PKT2_L* u_gen_sub_pkt2_l);
int HDMI_GEN_SUB_PKT2_H_gen_pkt2_pbGet(U_GEN_SUB_PKT2_H* u_gen_sub_pkt2_h);
int HDMI_GEN_SUB_PKT3_L_gen_pkt3_pbGet(U_GEN_SUB_PKT3_L* u_gen_sub_pkt3_l);
int HDMI_GEN_SUB_PKT3_H_gen_pkt3_pbGet(U_GEN_SUB_PKT3_H* u_gen_sub_pkt3_h);
/* Declare the gen2 Infoframe Set */
int HDMI_GEN2_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2);
int HDMI_GEN2_SUB_PKT0_L_gen2_pkt0_pbSet(unsigned int ugen2_pkt0_pb0,unsigned int ugen2_pkt0_pb1,unsigned int ugen2_pkt0_pb2,unsigned int ugen2_pkt0_pb3);
int HDMI_GEN2_SUB_PKT0_H_gen2_pkt0_pbSet(unsigned int ugen2_pkt0_pb4,unsigned int ugen2_pkt0_pb5,unsigned int ugen2_pkt0_pb6);
int HDMI_GEN2_SUB_PKT1_L_gen2_pkt1_pbSet(unsigned int ugen2_pkt1_pb0,unsigned int ugen2_pkt1_pb1,unsigned int ugen2_pkt1_pb2,unsigned int ugen2_pkt1_pb3);
int HDMI_GEN2_SUB_PKT1_H_gen2_pkt1_pbSet(unsigned int ugen2_pkt1_pb4,unsigned int ugen2_pkt1_pb5,unsigned int ugen2_pkt1_pb6);
int HDMI_GEN2_SUB_PKT2_L_gen2_pkt2_pbSet(unsigned int ugen2_pkt2_pb0,unsigned int ugen2_pkt2_pb1,unsigned int ugen2_pkt2_pb2,unsigned int ugen2_pkt2_pb3);
int HDMI_GEN2_SUB_PKT2_H_gen2_pkt2_pbSet(unsigned int ugen2_pkt2_pb4,unsigned int ugen2_pkt2_pb5,unsigned int ugen2_pkt2_pb6);
int HDMI_GEN2_SUB_PKT3_L_gen2_pkt3_pbSet(unsigned int ugen2_pkt3_pb0,unsigned int ugen2_pkt3_pb1,unsigned int ugen2_pkt3_pb2,unsigned int ugen2_pkt3_pb3);
int HDMI_GEN2_SUB_PKT3_H_gen2_pkt3_pbSet(unsigned int ugen2_pkt3_pb4,unsigned int ugen2_pkt3_pb5,unsigned int ugen2_pkt3_pb6);
/* Declare the gen2 Infoframe Get */
int HDMI_GEN2_PKT_HEADER_hbGet(U_GEN2_PKT_HEADER* u_gen2_pkt_header);
int HDMI_GEN2_SUB_PKT0_L_gen2_pkt0_pbGet(U_GEN2_SUB_PKT0_L* u_gen2_sub_pkt0_l);
int HDMI_GEN2_SUB_PKT0_H_gen2_pkt0_pbGet(U_GEN2_SUB_PKT0_H* u_gen2_sub_pkt0_h);
int HDMI_GEN2_SUB_PKT1_L_gen2_pkt1_pbGet(U_GEN2_SUB_PKT1_L* u_gen2_sub_pkt1_l);
int HDMI_GEN2_SUB_PKT1_H_gen2_pkt1_pbGet(U_GEN2_SUB_PKT1_H* u_gen2_sub_pkt1_h);
int HDMI_GEN2_SUB_PKT2_L_gen2_pkt2_pbGet(U_GEN2_SUB_PKT2_L* u_gen2_sub_pkt2_l);
int HDMI_GEN2_SUB_PKT2_H_gen2_pkt2_pbGet(U_GEN2_SUB_PKT2_H* u_gen2_sub_pkt2_h);
int HDMI_GEN2_SUB_PKT3_L_gen2_pkt3_pbGet(U_GEN2_SUB_PKT3_L* u_gen2_sub_pkt3_l);
int HDMI_GEN2_SUB_PKT3_H_gen2_pkt3_pbGet(U_GEN2_SUB_PKT3_H* u_gen2_sub_pkt3_h);
/* Declare the gen3 Infoframe Set */
int HDMI_GEN3_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2);
int HDMI_GEN3_SUB_PKT0_L_gen3_pkt0_pbSet(unsigned int ugen3_pkt0_pb0,unsigned int ugen3_pkt0_pb1,unsigned int ugen3_pkt0_pb2,unsigned int ugen3_pkt0_pb3);
int HDMI_GEN3_SUB_PKT0_H_gen3_pkt0_pbSet(unsigned int ugen3_pkt0_pb4,unsigned int ugen3_pkt0_pb5,unsigned int ugen3_pkt0_pb6);
int HDMI_GEN3_SUB_PKT1_L_gen3_pkt1_pbSet(unsigned int ugen3_pkt1_pb0,unsigned int ugen3_pkt1_pb1,unsigned int ugen3_pkt1_pb2,unsigned int ugen3_pkt1_pb3);
int HDMI_GEN3_SUB_PKT1_H_gen3_pkt1_pbSet(unsigned int ugen3_pkt1_pb4,unsigned int ugen3_pkt1_pb5,unsigned int ugen3_pkt1_pb6);
int HDMI_GEN3_SUB_PKT2_L_gen3_pkt2_pbSet(unsigned int ugen3_pkt2_pb0,unsigned int ugen3_pkt2_pb1,unsigned int ugen3_pkt2_pb2,unsigned int ugen3_pkt2_pb3);
int HDMI_GEN3_SUB_PKT2_H_gen3_pkt2_pbSet(unsigned int ugen3_pkt2_pb4,unsigned int ugen3_pkt2_pb5,unsigned int ugen3_pkt2_pb6);
int HDMI_GEN3_SUB_PKT3_L_gen3_pkt3_pbSet(unsigned int ugen3_pkt3_pb0,unsigned int ugen3_pkt3_pb1,unsigned int ugen3_pkt3_pb2,unsigned int ugen3_pkt3_pb3);
int HDMI_GEN3_SUB_PKT3_H_gen3_pkt3_pbSet(unsigned int ugen3_pkt3_pb4,unsigned int ugen3_pkt3_pb5,unsigned int ugen3_pkt3_pb6);
/* Declare the gen3 Infoframe Get */
int HDMI_GEN3_PKT_HEADER_hbGet(U_GEN3_PKT_HEADER* u_gen3_pkt_header);
int HDMI_GEN3_SUB_PKT0_L_gen3_pkt0_pbGet(U_GEN3_SUB_PKT0_L* u_gen3_sub_pkt0_l);
int HDMI_GEN3_SUB_PKT0_H_gen3_pkt0_pbGet(U_GEN3_SUB_PKT0_H* u_gen3_sub_pkt0_h);
int HDMI_GEN3_SUB_PKT1_L_gen3_pkt1_pbGet(U_GEN3_SUB_PKT1_L* u_gen3_sub_pkt1_l);
int HDMI_GEN3_SUB_PKT1_H_gen3_pkt1_pbGet(U_GEN3_SUB_PKT1_H* u_gen3_sub_pkt1_h);
int HDMI_GEN3_SUB_PKT2_L_gen3_pkt2_pbGet(U_GEN3_SUB_PKT2_L* u_gen3_sub_pkt2_l);
int HDMI_GEN3_SUB_PKT2_H_gen3_pkt2_pbGet(U_GEN3_SUB_PKT2_H* u_gen3_sub_pkt2_h);
int HDMI_GEN3_SUB_PKT3_L_gen3_pkt3_pbGet(U_GEN3_SUB_PKT3_L* u_gen3_sub_pkt3_l);
int HDMI_GEN3_SUB_PKT3_H_gen3_pkt3_pbGet(U_GEN3_SUB_PKT3_H* u_gen3_sub_pkt3_h);
/* Declare the gen4 Infoframe Set */
int HDMI_GEN4_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2);
int HDMI_GEN4_SUB_PKT0_L_gen4_pkt0_pbSet(unsigned int ugen4_pkt0_pb0,unsigned int ugen4_pkt0_pb1,unsigned int ugen4_pkt0_pb2,unsigned int ugen4_pkt0_pb3);
int HDMI_GEN4_SUB_PKT0_H_gen4_pkt0_pbSet(unsigned int ugen4_pkt0_pb4,unsigned int ugen4_pkt0_pb5,unsigned int ugen4_pkt0_pb6);
int HDMI_GEN4_SUB_PKT1_L_gen4_pkt1_pbSet(unsigned int ugen4_pkt1_pb0,unsigned int ugen4_pkt1_pb1,unsigned int ugen4_pkt1_pb2,unsigned int ugen4_pkt1_pb3);
int HDMI_GEN4_SUB_PKT1_H_gen4_pkt1_pbSet(unsigned int ugen4_pkt1_pb4,unsigned int ugen4_pkt1_pb5,unsigned int ugen4_pkt1_pb6);
int HDMI_GEN4_SUB_PKT2_L_gen4_pkt2_pbSet(unsigned int ugen4_pkt2_pb0,unsigned int ugen4_pkt2_pb1,unsigned int ugen4_pkt2_pb2,unsigned int ugen4_pkt2_pb3);
int HDMI_GEN4_SUB_PKT2_H_gen4_pkt2_pbSet(unsigned int ugen4_pkt2_pb4,unsigned int ugen4_pkt2_pb5,unsigned int ugen4_pkt2_pb6);
int HDMI_GEN4_SUB_PKT3_L_gen4_pkt3_pbSet(unsigned int ugen4_pkt3_pb0,unsigned int ugen4_pkt3_pb1,unsigned int ugen4_pkt3_pb2,unsigned int ugen4_pkt3_pb3);
int HDMI_GEN4_SUB_PKT3_H_gen4_pkt3_pbSet(unsigned int ugen4_pkt3_pb4,unsigned int ugen4_pkt3_pb5,unsigned int ugen4_pkt3_pb6);
/* Declare the gen4 Infoframe Get */
int HDMI_GEN4_PKT_HEADER_hbGet(U_GEN4_PKT_HEADER* u_gen4_pkt_header);
int HDMI_GEN4_SUB_PKT0_L_gen4_pkt0_pbGet(U_GEN4_SUB_PKT0_L* u_gen4_sub_pkt0_l);
int HDMI_GEN4_SUB_PKT0_H_gen4_pkt0_pbGet(U_GEN4_SUB_PKT0_H* u_gen4_sub_pkt0_h);
int HDMI_GEN4_SUB_PKT1_L_gen4_pkt1_pbGet(U_GEN4_SUB_PKT1_L* u_gen4_sub_pkt1_l);
int HDMI_GEN4_SUB_PKT1_H_gen4_pkt1_pbGet(U_GEN4_SUB_PKT1_H* u_gen4_sub_pkt1_h);
int HDMI_GEN4_SUB_PKT2_L_gen4_pkt2_pbGet(U_GEN4_SUB_PKT2_L* u_gen4_sub_pkt2_l);
int HDMI_GEN4_SUB_PKT2_H_gen4_pkt2_pbGet(U_GEN4_SUB_PKT2_H* u_gen4_sub_pkt2_h);
int HDMI_GEN4_SUB_PKT3_L_gen4_pkt3_pbGet(U_GEN4_SUB_PKT3_L* u_gen4_sub_pkt3_l);
int HDMI_GEN4_SUB_PKT3_H_gen4_pkt3_pbGet(U_GEN4_SUB_PKT3_H* u_gen4_sub_pkt3_h);
/* Declare the gen5 Infoframe Set */
int HDMI_GEN5_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2);
int HDMI_GEN5_SUB_PKT0_L_gen5_pkt0_pbSet(unsigned int ugen5_pkt0_pb0,unsigned int ugen5_pkt0_pb1,unsigned int ugen5_pkt0_pb2,unsigned int ugen5_pkt0_pb3);
int HDMI_GEN5_SUB_PKT0_H_gen5_pkt0_pbSet(unsigned int ugen5_pkt0_pb4,unsigned int ugen5_pkt0_pb5,unsigned int ugen5_pkt0_pb6);
int HDMI_GEN5_SUB_PKT1_L_gen5_pkt1_pbSet(unsigned int ugen5_pkt1_pb0,unsigned int ugen5_pkt1_pb1,unsigned int ugen5_pkt1_pb2,unsigned int ugen5_pkt1_pb3);
int HDMI_GEN5_SUB_PKT1_H_gen5_pkt1_pbSet(unsigned int ugen5_pkt1_pb4,unsigned int ugen5_pkt1_pb5,unsigned int ugen5_pkt1_pb6);
int HDMI_GEN5_SUB_PKT2_L_gen5_pkt2_pbSet(unsigned int ugen5_pkt2_pb0,unsigned int ugen5_pkt2_pb1,unsigned int ugen5_pkt2_pb2,unsigned int ugen5_pkt2_pb3);
int HDMI_GEN5_SUB_PKT2_H_gen5_pkt2_pbSet(unsigned int ugen5_pkt2_pb4,unsigned int ugen5_pkt2_pb5,unsigned int ugen5_pkt2_pb6);
int HDMI_GEN5_SUB_PKT3_L_gen5_pkt3_pbSet(unsigned int ugen5_pkt3_pb0,unsigned int ugen5_pkt3_pb1,unsigned int ugen5_pkt3_pb2,unsigned int ugen5_pkt3_pb3);
int HDMI_GEN5_SUB_PKT3_H_gen5_pkt3_pbSet(unsigned int ugen5_pkt3_pb4,unsigned int ugen5_pkt3_pb5,unsigned int ugen5_pkt3_pb6);
/* Declare the gen5 Infoframe Get */
int HDMI_GEN5_PKT_HEADER_hbGet(U_GEN5_PKT_HEADER* u_gen5_pkt_header);
int HDMI_GEN5_SUB_PKT0_L_gen5_pkt0_pbGet(U_GEN5_SUB_PKT0_L* u_gen5_sub_pkt0_l);
int HDMI_GEN5_SUB_PKT0_H_gen5_pkt0_pbGet(U_GEN5_SUB_PKT0_H* u_gen5_sub_pkt0_h);
int HDMI_GEN5_SUB_PKT1_L_gen5_pkt1_pbGet(U_GEN5_SUB_PKT1_L* u_gen5_sub_pkt1_l);
int HDMI_GEN5_SUB_PKT1_H_gen5_pkt1_pbGet(U_GEN5_SUB_PKT1_H* u_gen5_sub_pkt1_h);
int HDMI_GEN5_SUB_PKT2_L_gen5_pkt2_pbGet(U_GEN5_SUB_PKT2_L* u_gen5_sub_pkt2_l);
int HDMI_GEN5_SUB_PKT2_H_gen5_pkt2_pbGet(U_GEN5_SUB_PKT2_H* u_gen5_sub_pkt2_h);
int HDMI_GEN5_SUB_PKT3_L_gen5_pkt3_pbGet(U_GEN5_SUB_PKT3_L* u_gen5_sub_pkt3_l);
int HDMI_GEN5_SUB_PKT3_H_gen5_pkt3_pbGet(U_GEN5_SUB_PKT3_H* u_gen5_sub_pkt3_h);
/* Declare the gamut Infoframe Set */
int HDMI_GAMUT_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2);
int HDMI_GAMUT_SUB_PKT0_L_gamut_pkt0_pbSet(unsigned int ugamut_pkt0_pb0,unsigned int ugamut_pkt0_pb1,unsigned int ugamut_pkt0_pb2,unsigned int ugamut_pkt0_pb3);
int HDMI_GAMUT_SUB_PKT0_H_gamut_pkt0_pbSet(unsigned int ugamut_pkt0_pb4,unsigned int ugamut_pkt0_pb5,unsigned int ugamut_pkt0_pb6);
int HDMI_GAMUT_SUB_PKT1_L_gamut_pkt1_pbSet(unsigned int ugamut_pkt1_pb0,unsigned int ugamut_pkt1_pb1,unsigned int ugamut_pkt1_pb2,unsigned int ugamut_pkt1_pb3);
int HDMI_GAMUT_SUB_PKT1_H_gamut_pkt1_pbSet(unsigned int ugamut_pkt1_pb4,unsigned int ugamut_pkt1_pb5,unsigned int ugamut_pkt1_pb6);
int HDMI_GAMUT_SUB_PKT2_L_gamut_pkt2_pbSet(unsigned int ugamut_pkt2_pb0,unsigned int ugamut_pkt2_pb1,unsigned int ugamut_pkt2_pb2,unsigned int ugamut_pkt2_pb3);
int HDMI_GAMUT_SUB_PKT2_H_gamut_pkt2_pbSet(unsigned int ugamut_pkt2_pb4,unsigned int ugamut_pkt2_pb5,unsigned int ugamut_pkt2_pb6);
int HDMI_GAMUT_SUB_PKT3_L_gamut_pkt3_pbSet(unsigned int ugamut_pkt3_pb0,unsigned int ugamut_pkt3_pb1,unsigned int ugamut_pkt3_pb2,unsigned int ugamut_pkt3_pb3);
int HDMI_GAMUT_SUB_PKT3_H_gamut_pkt3_pbSet(unsigned int ugamut_pkt3_pb4,unsigned int ugamut_pkt3_pb5,unsigned int ugamut_pkt3_pb6);
/* Declare the gamut Infoframe Get */
int HDMI_GAMUT_PKT_HEADER_hbGet(U_GAMUT_PKT_HEADER* u_gamut_pkt_header);
int HDMI_GAMUT_SUB_PKT0_L_gamut_pkt0_pbGet(U_GAMUT_SUB_PKT0_L* u_gamut_sub_pkt0_l);
int HDMI_GAMUT_SUB_PKT0_H_gamut_pkt0_pbGet(U_GAMUT_SUB_PKT0_H* u_gamut_sub_pkt0_h);
int HDMI_GAMUT_SUB_PKT1_L_gamut_pkt1_pbGet(U_GAMUT_SUB_PKT1_L* u_gamut_sub_pkt1_l);
int HDMI_GAMUT_SUB_PKT1_H_gamut_pkt1_pbGet(U_GAMUT_SUB_PKT1_H* u_gamut_sub_pkt1_h);
int HDMI_GAMUT_SUB_PKT2_L_gamut_pkt2_pbGet(U_GAMUT_SUB_PKT2_L* u_gamut_sub_pkt2_l);
int HDMI_GAMUT_SUB_PKT2_H_gamut_pkt2_pbGet(U_GAMUT_SUB_PKT2_H* u_gamut_sub_pkt2_h);
int HDMI_GAMUT_SUB_PKT3_L_gamut_pkt3_pbGet(U_GAMUT_SUB_PKT3_L* u_gamut_sub_pkt3_l);
int HDMI_GAMUT_SUB_PKT3_H_gamut_pkt3_pbGet(U_GAMUT_SUB_PKT3_H* u_gamut_sub_pkt3_h);
/* Declare the VSIF Infoframe Set */
int HDMI_VSIF_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2);
int HDMI_VSIF_SUB_PKT0_L_vsif_pkt0_pbSet(unsigned int uvsif_pkt0_pb0,unsigned int uvsif_pkt0_pb1,unsigned int uvsif_pkt0_pb2,unsigned int uvsif_pkt0_pb3);
int HDMI_VSIF_SUB_PKT0_H_vsif_pkt0_pbSet(unsigned int uvsif_pkt0_pb4,unsigned int uvsif_pkt0_pb5,unsigned int uvsif_pkt0_pb6);
int HDMI_VSIF_SUB_PKT1_L_vsif_pkt1_pbSet(unsigned int uvsif_pkt1_pb0,unsigned int uvsif_pkt1_pb1,unsigned int uvsif_pkt1_pb2,unsigned int uvsif_pkt1_pb3);
int HDMI_VSIF_SUB_PKT1_H_vsif_pkt1_pbSet(unsigned int uvsif_pkt1_pb4,unsigned int uvsif_pkt1_pb5,unsigned int uvsif_pkt1_pb6);
int HDMI_VSIF_SUB_PKT2_L_vsif_pkt2_pbSet(unsigned int uvsif_pkt2_pb0,unsigned int uvsif_pkt2_pb1,unsigned int uvsif_pkt2_pb2,unsigned int uvsif_pkt2_pb3);
int HDMI_VSIF_SUB_PKT2_H_vsif_pkt2_pbSet(unsigned int uvsif_pkt2_pb4,unsigned int uvsif_pkt2_pb5,unsigned int uvsif_pkt2_pb6);
int HDMI_VSIF_SUB_PKT3_L_vsif_pkt3_pbSet(unsigned int uvsif_pkt3_pb0,unsigned int uvsif_pkt3_pb1,unsigned int uvsif_pkt3_pb2,unsigned int uvsif_pkt3_pb3);
int HDMI_VSIF_SUB_PKT3_H_vsif_pkt3_pbSet(unsigned int uvsif_pkt3_pb4,unsigned int uvsif_pkt3_pb5,unsigned int uvsif_pkt3_pb6);
/* Declare the VSIF Infoframe Get */
int HDMI_VSIF_PKT_HEADER_hbGet(U_VSIF_PKT_HEADER* u_vsif_pkt_header);
int HDMI_VSIF_SUB_PKT0_L_vsif_pkt0_pbGet(U_VSIF_SUB_PKT0_L* u_vsif_sub_pkt0_l);
int HDMI_VSIF_SUB_PKT0_H_vsif_pkt0_pbGet(U_VSIF_SUB_PKT0_H* u_vsif_sub_pkt0_h);
int HDMI_VSIF_SUB_PKT1_L_vsif_pkt1_pbGet(U_VSIF_SUB_PKT1_L* u_vsif_sub_pkt1_l);
int HDMI_VSIF_SUB_PKT1_H_vsif_pkt1_pbGet(U_VSIF_SUB_PKT1_H* u_vsif_sub_pkt1_h);
int HDMI_VSIF_SUB_PKT2_L_vsif_pkt2_pbGet(U_VSIF_SUB_PKT2_L* u_vsif_sub_pkt2_l);
int HDMI_VSIF_SUB_PKT2_H_vsif_pkt2_pbGet(U_VSIF_SUB_PKT2_H* u_vsif_sub_pkt2_h);
int HDMI_VSIF_SUB_PKT3_L_vsif_pkt3_pbGet(U_VSIF_SUB_PKT3_L* u_vsif_sub_pkt3_l);
int HDMI_VSIF_SUB_PKT3_H_vsif_pkt3_pbGet(U_VSIF_SUB_PKT3_H* u_vsif_sub_pkt3_h);
/* Declare the Infoframe Enable */
int HDMI_CEA_AVI_CFG_cea_avi_enSet(unsigned int ucea_avi_en);
int HDMI_CEA_AVI_CFG_cea_avi_rpt_enSet(unsigned int ucea_avi_rpt_en);
int HDMI_CEA_SPF_CFG_cea_spf_enSet(unsigned int ucea_spf_en);
int HDMI_CEA_SPF_CFG_cea_spf_rpt_enSet(unsigned int ucea_spf_rpt_en);
int HDMI_CEA_AUD_CFG_cea_aud_enSet(unsigned int ucea_aud_en);
int HDMI_CEA_AUD_CFG_cea_aud_rpt_enSet(unsigned int ucea_aud_rpt_en);
int HDMI_CEA_MPEG_CFG_cea_mpeg_enSet(unsigned int ucea_mpeg_en);
int HDMI_CEA_MPEG_CFG_cea_mpeg_rpt_enSet(unsigned int ucea_mpeg_rpt_en);
int HDMI_CEA_GEN_CFG_cea_gen_enSet(unsigned int ucea_gen_en);
int HDMI_CEA_GEN_CFG_cea_gen_rpt_enSet(unsigned int ucea_gen_rpt_en);
int HDMI_CEA_CP_CFG_cea_cp_enSet(unsigned int ucea_cp_en);
int HDMI_CEA_CP_CFG_cea_cp_rpt_cntSet(unsigned int ucea_cp_rpt_cnt);
int HDMI_CEA_CP_CFG_cea_cp_rpt_enSet(unsigned int ucea_cp_rpt_en);
int HDMI_CEA_GEN2_CFG_cea_gen2_enSet(unsigned int ucea_gen2_en);
int HDMI_CEA_GEN2_CFG_cea_gen2_rpt_enSet(unsigned int ucea_gen2_rpt_en);
int HDMI_CEA_GEN3_CFG_cea_gen3_enSet(unsigned int ucea_gen3_en);
int HDMI_CEA_GEN3_CFG_cea_gen3_rpt_enSet(unsigned int ucea_gen3_rpt_en);
int HDMI_CEA_GEN4_CFG_cea_gen4_enSet(unsigned int ucea_gen4_en);
int HDMI_CEA_GEN4_CFG_cea_gen4_rpt_enSet(unsigned int ucea_gen4_rpt_en);
int HDMI_CEA_GEN5_CFG_cea_gen5_enSet(unsigned int ucea_gen5_en);
int HDMI_CEA_GEN5_CFG_cea_gen5_rpt_enSet(unsigned int ucea_gen5_rpt_en);
int HDMI_CEA_GAMUT_CFG_cea_gamut_enSet(unsigned int ucea_gamut_en);
int HDMI_CEA_GAMUT_CFG_cea_gamut_rpt_enSet(unsigned int ucea_gamut_rpt_en);
int HDMI_CEA_VSIF_CFG_cea_vsif_enSet(unsigned int ucea_vsif_en);
int HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enSet(unsigned int ucea_vsif_rpt_en);
int HDMI_CEA_PKT_STATE_cea_avi_stateSet(unsigned int ucea_avi_state);
int HDMI_CEA_PKT_STATE_cea_aud_stateSet(unsigned int ucea_aud_state);
int HDMI_CEA_PKT_STATE_cea_cp_stateSet(unsigned int ucea_cp_state);
int HDMI_CEA_PKT_STATE_cea_gen_stateSet(unsigned int ucea_gen_state);
int HDMI_CEA_PKT_STATE_cea_gen2_stateSet(unsigned int ucea_gen2_state);
int HDMI_CEA_PKT_STATE_cea_gen3_stateSet(unsigned int ucea_gen3_state);
int HDMI_CEA_PKT_STATE_cea_gen4_stateSet(unsigned int ucea_gen4_state);
int HDMI_CEA_PKT_STATE_cea_gen5_stateSet(unsigned int ucea_gen5_state);
int HDMI_CEA_PKT_STATE_cea_spd_stateSet(unsigned int ucea_spd_state);
int HDMI_CEA_PKT_STATE_cea_mpeg_stateSet(unsigned int ucea_mpeg_state);
int HDMI_CEA_PKT_STATE_cea_gamut_stateSet(unsigned int ucea_gamut_state);
int HDMI_CEA_PKT_STATE_cea_vsif_stateSet(unsigned int ucea_vsif_state);
int HDMI_AVMIXER_CONFIG_hdmi_modeSet(unsigned int uhdmi_mode);
int HDMI_AVMIXER_CONFIG_dc_pkt_enSet(unsigned int udc_pkt_en);
int HDMI_AVMIXER_CONFIG_null_pkt_enSet(unsigned int unull_pkt_en);
int HDMI_AVMIXER_CONFIG_null_pkt_en_vs_highSet(unsigned int unull_pkt_en_vs_high);
int HDMI_AVMIXER_CONFIG_intr_encryptionSet(unsigned int uintr_encryption);
int HDMI_AVMIXER_CONFIG_pb_ovr_dc_pkt_enSet(unsigned int upb_ovr_dc_pkt_en);
int HDMI_AVMIXER_CONFIG_pb_priotity_ctlSet(unsigned int upb_priotity_ctl);
int HDMI_AVMIXER_CONFIG_pkt_bypass_modeSet(unsigned int upkt_bypass_mode);
int HDMI_CP_PKT_AVMUTE_cp_set_avmuteSet(unsigned int ucp_set_avmute);
int HDMI_CP_PKT_AVMUTE_cp_clr_avmuteSet(unsigned int ucp_clr_avmute);
int HDMI_VIDEO_BLANK_CFG_video_blankSet(unsigned int uvideo_blank);
int HDMI_TMDS_BIST_CTRL_reg_tbist_enSet(unsigned int ureg_tbist_en);
int HDMI_TMDS_BIST_CTRL_reg_tbist_syn_polSet(unsigned int ureg_tbist_syn_pol);
int HDMI_TMDS_BIST_CTRL_reg_tbist_timing_selSet(unsigned int ureg_tbist_timing_sel);
int HDMI_TMDS_BIST_CTRL_reg_tbist_patt_selSet(unsigned int ureg_tbist_patt_sel);
int HDMI_HDMI_ENC_CTRL_enc_hdmi2_onSet(unsigned int uenc_hdmi2_on);
int HDMI_HDMI_ENC_CTRL_enc_scr_onSet(unsigned int uenc_scr_on);
int HDMI_HDMI_ENC_CTRL_enc_scr_mdSet(unsigned int uenc_scr_md);
int HDMI_HDMI_ENC_CTRL_enc_hdmi_valSet(unsigned int uenc_hdmi_val);
int HDMI_HDMI_ENC_CTRL_enc_hdmi_ovrSet(unsigned int uenc_hdmi_ovr);
int HDMI_HDMI_ENC_CTRL_enc_bypassSet(unsigned int uenc_bypass);
int HDMI_HDMI_ENC_CTRL_enc_ck_div_selSet(unsigned int uenc_ck_div_sel);
int HDMI_ENC_CK_SHARP_enc_ck_sharp0Set(unsigned int uenc_ck_sharp0);
int HDMI_ENC_CK_SHARP_enc_ck_sharp1Set(unsigned int uenc_ck_sharp1);
int HDMI_ENC_CK_SHARP_enc_ck_sharp2Set(unsigned int uenc_ck_sharp2);

int HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeGet(HI_VOID);
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_auto_rst_enGet(HI_VOID);
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_manu_rstGet(HI_VOID);
int HDMI_TX_PACK_FIFO_CTRL_reg_clock_det_enGet(HI_VOID);
int HDMI_TX_PACK_FIFO_CTRL_reg_ext_tmds_paraGet(HI_VOID);
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_delay_cntGet(HI_VOID);
int HDMI_TX_PACK_FIFO_ST_pclk2tclk_stableGet(HI_VOID);
int HDMI_PCLK_REFER_CNT_reg_pclk_refer_cntGet(HI_VOID);
int HDMI_TCLK_LOWER_THRESHOLD_reg_tcnt_lower_thresholdGet(HI_VOID);
int HDMI_TCLK_UPPER_THRESHOLD_reg_tcnt_upper_thresholdGet(HI_VOID);

int HDMI_CEA_AVI_CFG_cea_avi_enGet(HI_VOID);
int HDMI_CEA_AVI_CFG_cea_avi_rpt_enGet(HI_VOID);
int HDMI_CEA_SPF_CFG_cea_spf_enGet(HI_VOID);
int HDMI_CEA_SPF_CFG_cea_spf_rpt_enGet(HI_VOID);
int HDMI_CEA_AUD_CFG_cea_aud_enGet(HI_VOID);
int HDMI_CEA_AUD_CFG_cea_aud_rpt_enGet(HI_VOID);
int HDMI_CEA_MPEG_CFG_cea_mpeg_enGet(HI_VOID);
int HDMI_CEA_MPEG_CFG_cea_mpeg_rpt_enGet(HI_VOID);
int HDMI_CEA_GEN_CFG_cea_gen_enGet(HI_VOID);
int HDMI_CEA_GEN_CFG_cea_gen_rpt_enGet(HI_VOID);
int HDMI_CEA_CP_CFG_cea_cp_enGet(HI_VOID);
int HDMI_CEA_CP_CFG_cea_cp_rpt_enGet(HI_VOID);
int HDMI_CEA_CP_CFG_cea_cp_rpt_cntGet(HI_VOID);
int HDMI_CEA_GEN2_CFG_cea_gen2_enGet(HI_VOID);
int HDMI_CEA_GEN2_CFG_cea_gen2_rpt_enGet(HI_VOID);
int HDMI_CEA_GEN3_CFG_cea_gen3_enGet(HI_VOID);
int HDMI_CEA_GEN3_CFG_cea_gen3_rpt_enGet(HI_VOID);
int HDMI_CEA_GEN4_CFG_cea_gen4_enGet(HI_VOID);
int HDMI_CEA_GEN4_CFG_cea_gen4_rpt_enGet(HI_VOID);
int HDMI_CEA_GEN5_CFG_cea_gen5_enGet(HI_VOID);
int HDMI_CEA_GEN5_CFG_cea_gen5_rpt_enGet(HI_VOID);
int HDMI_CEA_GAMUT_CFG_cea_gamut_enGet(HI_VOID);
int HDMI_CEA_GAMUT_CFG_cea_gamut_rpt_enGet(HI_VOID);
int HDMI_CEA_VSIF_CFG_cea_vsif_enGet(HI_VOID);
int HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enGet(HI_VOID);
int HDMI_CEA_PKT_STATE_cea_avi_stateGet(HI_VOID);
int HDMI_CEA_PKT_STATE_cea_aud_stateGet(HI_VOID);
int HDMI_CEA_PKT_STATE_cea_cp_stateGet(HI_VOID);
int HDMI_CEA_PKT_STATE_cea_gen_stateGet(HI_VOID);
int HDMI_CEA_PKT_STATE_cea_gen2_stateGet(HI_VOID);
int HDMI_CEA_PKT_STATE_cea_gen3_stateGet(HI_VOID);
int HDMI_CEA_PKT_STATE_cea_gen4_stateGet(HI_VOID);
int HDMI_CEA_PKT_STATE_cea_gen5_stateGet(HI_VOID);
int HDMI_CEA_PKT_STATE_cea_spd_stateGet(HI_VOID);
int HDMI_CEA_PKT_STATE_cea_mpeg_stateGet(HI_VOID);
int HDMI_CEA_PKT_STATE_cea_gamut_stateGet(HI_VOID);
int HDMI_CEA_PKT_STATE_cea_vsif_stateGet(HI_VOID);
int HDMI_AVMIXER_CONFIG_hdmi_modeGet(HI_VOID);
int HDMI_AVMIXER_CONFIG_dc_pkt_enGet(HI_VOID);
int HDMI_AVMIXER_CONFIG_null_pkt_enGet(HI_VOID);
int HDMI_AVMIXER_CONFIG_null_pkt_en_vs_highGet(HI_VOID);
int HDMI_AVMIXER_CONFIG_intr_encryptionGet(HI_VOID);
int HDMI_AVMIXER_CONFIG_pb_ovr_dc_pkt_enGet(HI_VOID);
int HDMI_AVMIXER_CONFIG_pb_priotity_ctlGet(HI_VOID);
int HDMI_AVMIXER_CONFIG_pkt_bypass_modeGet(HI_VOID);
int HDMI_CP_PKT_AVMUTE_cp_set_avmuteGet(HI_VOID);
int HDMI_CP_PKT_AVMUTE_cp_clr_avmuteGet(HI_VOID);
int HDMI_VIDEO_BLANK_CFG_video_blankGet(HI_VOID);
int HDMI_TMDS_BIST_CTRL_reg_tbist_enGet(HI_VOID);
int HDMI_TMDS_BIST_CTRL_reg_tbist_syn_polGet(HI_VOID);
int HDMI_TMDS_BIST_CTRL_reg_tbist_timing_selGet(HI_VOID);
int HDMI_TMDS_BIST_CTRL_reg_tbist_patt_selGet(HI_VOID);
int HDMI_HDMI_ENC_CTRL_enc_hdmi2_onGet(HI_VOID);
int HDMI_HDMI_ENC_CTRL_enc_scr_onGet(HI_VOID);
int HDMI_HDMI_ENC_CTRL_enc_scr_mdGet(HI_VOID);
int HDMI_HDMI_ENC_CTRL_enc_hdmi_valGet(HI_VOID);
int HDMI_HDMI_ENC_CTRL_enc_hdmi_ovrGet(HI_VOID);
int HDMI_HDMI_ENC_CTRL_enc_bypassGet(HI_VOID);
int HDMI_HDMI_ENC_CTRL_enc_ck_div_selGet(HI_VOID);
int HDMI_ENC_CK_SHARP_enc_ck_sharp0Get(HI_VOID);
int HDMI_ENC_CK_SHARP_enc_ck_sharp1Get(HI_VOID);
int HDMI_ENC_CK_SHARP_enc_ck_sharp2Get(HI_VOID);

#endif // __TX_HDMI_REG_C_UNION_DEFINE_H__
