
#ifndef __DEMUX_REG_H__
#define __DEMUX_REG_H__

#include "demux_config.h"

#define SYS_WRITE_REG(offset, value)    (*(volatile HI_U32*)((HI_U32)(HI_CRG_BASE) + (offset)) = (value))
#define SYS_READ_REG(offset)            (*(volatile HI_U32*)((HI_U32)(HI_CRG_BASE) + (offset)))

#define DMX_WRITE_REG(offset, value)    (*(volatile HI_U32*)((HI_U32)(HI_PVR_BASE) + (offset)) = (value))
#define DMX_READ_REG(offset)            (*(volatile HI_U32*)((HI_U32)(HI_PVR_BASE) + (offset)))

#if defined(CHIP_TYPE_hi3716h) || defined(CHIP_TYPE_hi3716c)

#define PERI_CRG25                      (0x00A4)

typedef union
{
    struct
    {
        HI_U32  pvr_srst_req        : 1;    // [0]
        HI_U32  reserved_0          : 7;    // [7:1]
        HI_U32  pvr_hcken           : 1;    // [8]
        HI_U32  dmx_cken            : 1;    // [9]
        HI_U32  dmx_clk_sel         : 1;    // [10]
        HI_U32  tsi0_cken           : 1;    // [11]
        HI_U32  tsi1_cken           : 1;    // [12]
        HI_U32  tsi2_cken           : 1;    // [13]
        HI_U32  tsi_clkout_cken     : 1;    // [14]
        HI_U32  reserved_1          : 1;    // [15]
        HI_U32  tsi0_inclk_pctrl    : 1;    // [16]
        HI_U32  tsi1_inclk_pctrl    : 1;    // [17]
        HI_U32  tsi_clkout_pctrl    : 1;    // [18]
        HI_U32  dmx_clk_scale       : 2;    // [20:19]
        HI_U32  reserved_2          : 11;   // [31:21]
    } bits;

    HI_U32 value;
} U_PERI_CRG25;

#elif defined(CHIP_TYPE_hi3716m)

#define PERI_CRG25                      (0x00A4)

typedef union
{
    struct
    {
        HI_U32  pvr_srst_req        : 1;    // [0]
        HI_U32  reserved_0          : 7;    // [7:1]
        HI_U32  pvr_hcken           : 1;    // [8]
        HI_U32  dmx_cken            : 1;    // [9]
        HI_U32  dmx_clk_sel         : 1;    // [10]
        HI_U32  tsi0_cken           : 1;    // [11]
        HI_U32  tsi1_cken           : 1;    // [12]
        HI_U32  tsi2_cken           : 1;    // [13]
        HI_U32  tsi_clkout_cken     : 1;    // [14]
        HI_U32  reserved_1          : 1;    // [15]
        HI_U32  tsi0_inclk_pctrl    : 1;    // [16]
        HI_U32  tsi1_inclk_pctrl    : 1;    // [17]
        HI_U32  tsi_clkout_pctrl    : 1;    // [18]
        HI_U32  dmx_clk_scale       : 2;    // [20:19]
        HI_U32  reserved_2          : 11;   // [31:21]
    } bits;

    HI_U32 value;
} U_PERI_CRG25;

typedef union
{
    struct
    {
        HI_U32  pvr_srst_req        : 1;    // [0]
        HI_U32  reserved_0          : 7;    // [7:1]
        HI_U32  pvr_hcken           : 1;    // [8]
        HI_U32  dmx_cken            : 1;    // [9]
        HI_U32  reserved_1          : 1;    // [10]
        HI_U32  tsi0_cken           : 1;    // [11]
        HI_U32  tsi1_cken           : 1;    // [12]
        HI_U32  tsi2_cken           : 1;    // [13]
        HI_U32  pvr_tsout_cken      : 1;    // [14]
        HI_U32  reserved_2          : 1;    // [15]
        HI_U32  tsi0_inclk_pctrl    : 1;    // [16]
        HI_U32  tsi1_inclk_pctrl    : 1;    // [17]
        HI_U32  pvr_tsout_pctrl     : 1;    // [18]
        HI_U32  dmx_clk_scale       : 2;    // [20:19]
        HI_U32  pvr_tsout_sel       : 2;    // [22:21]
        HI_U32  dmx_clk_sel         : 1;    // [23]
        HI_U32  pvr_tsout_div       : 4;    // [27:24]
        HI_U32  reserved_3          : 4;    // [31:28]
    } bits;

    HI_U32 value;
} U_PERI_CRG25_V300;

#elif defined(CHIP_TYPE_hi3712) || defined(CHIP_TYPE_hi3715)

#define PERI_CRG25                      (0x00A4)

typedef union
{
    struct
    {
        HI_U32  pvr_srst_req        : 1;    // [0]
        HI_U32  reserved_0          : 7;    // [7:1]
        HI_U32  pvr_hcken           : 1;    // [8]
        HI_U32  dmx_cken            : 1;    // [9]
        HI_U32  reserved_1          : 2;    // [11:10]
        HI_U32  tsi0_cken           : 1;    // [12]
        HI_U32  tsi1_cken           : 1;    // [13]
        HI_U32  tsi0_inclk_pctrl    : 1;    // [14]
        HI_U32  tsi1_inclk_pctrl    : 1;    // [15]
        HI_U32  dmx_clk_scale       : 2;    // [17:16]
        HI_U32  dmx_clk_sel         : 1;    // [18]
        HI_U32  reserved_2          : 13;   // [31:19]
    } bits;

    HI_U32 value;
} U_PERI_CRG25;

#endif

/******************************************************************************/
/*                      dmx int regeister                                     */
/******************************************************************************/
#define PVR_CSA3_RST                    (0x0A34)

typedef union
{
    struct
    {
        HI_U32  pvr_csa3_soft_rst   : 1;    // bit0
        HI_U32  reserved_0          : 31;   // bit[31:1]
    } bits;

    HI_U32 value;
} U_PVR_CSA3_RST;

/******************************************************************************/
/*                      interface regeister                                   */
/******************************************************************************/
#define TS_INTERFACE(PortId)            (0x1100 + ((PortId) << 8))

typedef union
{
    struct
    {
        HI_U32  syncon_th           : 3;    // [2:0]
        HI_U32  reserved_0          : 1;    // [3]
        HI_U32  syncoff_th          : 2;    // [5:4]
        HI_U32  reserved_1          : 6;    // [11:6]
        HI_U32  ser_2bit_mode       : 1;    // [12]
        HI_U32  ser_2bit_rev        : 1;    // [13]
        HI_U32  ser_nosync          : 1;    // [14]
        HI_U32  reserved_2          : 1;    // [15]
        HI_U32  nosync_fixed_204    : 1;    // [16]
        HI_U32  reserved_3          : 3;    // [19:17]
        HI_U32  sync_mode           : 2;    // [21:20]
        HI_U32  err_bypass          : 1;    // [22]
        HI_U32  sync_pol            : 1;    // [23]
        HI_U32  vld_pol             : 1;    // [24]
        HI_U32  err_pol             : 1;    // [25]
        HI_U32  reserved_4          : 3;    // [28:26]
        HI_U32  serial_sel          : 1;    // [29]
        HI_U32  bit_sel             : 1;    // [30]
        HI_U32  port_sel            : 1;    // [31]
    } bits;

    HI_U32  value;
} U_TS_INTERFACE;

#define TS_COUNT(PortId)                (0x1104 + ((PortId) << 8))

#define TS_COUNT_CTRL(PortId)           (0x1108 + ((PortId) << 8))

typedef union
{
    struct
    {
        HI_U32  ts_count_ctrl       : 2;    // [1:0]
        HI_U32  reserved_0          : 30;   // [31:2]
    } bits;

    HI_U32  value;
} U_TS_COUNT_CTRL;

#define ETS_COUNT(PortId)               (0x110C + ((PortId) << 8))

#define ETS_COUNT_CTRL(PortId)          (0x1110 + ((PortId) << 8))

typedef union
{
    struct
    {
        HI_U32  ets_count_ctrl      : 2;    // [1:0]
        HI_U32  reserved_0          : 30;   // [31:2]
    } bits;

    HI_U32  value;
} U_ETS_COUNT_CTRL;

/******************************************************************************/
/*                      parser regeister                                      */
/******************************************************************************/
#define DMX_PID_CTRL(ChanId)            (0x3000 + ((ChanId) << 2))

typedef union
{
    struct
    {
        HI_U32  data_type           : 2;    // [1:0]
        HI_U32  cw_index            : 5;    // [6:2]
        HI_U32  do_scram            : 1;    // [7]
        HI_U32  af_mode             : 2;    // [9:8]
        HI_U32  crc_mode            : 3;    // [12:10]
        HI_U32  cc_discon_ctrl      : 1;    // [13]
        HI_U32  pusi_disable        : 1;    // [14]
        HI_U32  cc_equ_rve          : 1;    // [15]
        HI_U32  ts_post_mode        : 1;    // [16]
        HI_U32  ts_post_threshold   : 6;    // [22:17]
        HI_U32  ch_attri            : 3;    // [25:23]
        HI_U32  reserved_0          : 6;    // [31:26]
    } bits;

    HI_U32  value;
} U_DMX_PID_CTRL;

#define DMX_PID_EN(ChanId)              (0x3200 + ((ChanId) << 2))

typedef union
{
    struct
    {
        HI_U32  pid_play_dmx_id     : 3;    // [2:0]
        HI_U32  reserved_0          : 1;    // [3]
        HI_U32  pid_rec_dmx_id      : 3;    // [6:4]
        HI_U32  reserved_1          : 25;   // [31:7]
    } bits;

    HI_U32  value;
} U_DMX_PID_EN;

#define DMX_PID_VALUE(ChanId)           (0x3400 + ((ChanId) << 2))

typedef union
{
    struct
    {
        HI_U32  pid_value       : 13;   // [12:0]
        HI_U32  pid_extra       : 1;    // [13]
        HI_U32  reserved_0      : 18;   // [31:14]
    } bits;

    HI_U32  value;
} U_DMX_PID_VALUE;

#define DMX_PID_REC_BUF(ChanId)         (0x3600 + ((ChanId) << 2))

typedef union
{
    struct
    {
        HI_U32  rec_buf         : 7;    // [6:0]
        HI_U32  reserved_0      : 1;    // [7]
        HI_U32  flt_hit_mode    : 1;    // [8]
        HI_U32  reserved_1      : 23;   // [31:9]
    } bits;

    HI_U32  value;
} U_DMX_PID_REC_BUF;

#define DMX_PID_PLAY_BUF(ChanId)        (0x3800 + ((ChanId) << 2))

typedef union
{
    struct
    {
        HI_U32  play_buf            : 7;    // [6:0]
        HI_U32  psi_crc_sub_en      : 1;    // [7]
        HI_U32  reserved_0          : 24;   // [31:8]
    } bits;

    HI_U32  value;
} U_DMX_PID_PLAY_BUF;

#define SWITCH_CFG                      (0x3A00)

typedef union
{
    struct
    {
        HI_U32  switch_cfg1         : 3;    // [2:0]
        HI_U32  reserved_0          : 1;    // [3]
        HI_U32  switch_cfg2         : 3;    // [6:4]
        HI_U32  reserved_1          : 1;    // [7]
        HI_U32  switch_cfg3         : 3;    // [10:8]
        HI_U32  reserved_2          : 1;    // [11]
        HI_U32  switch_cfg4         : 3;    // [14:12]
        HI_U32  reserved_3          : 1;    // [15]
        HI_U32  switch_cfg5         : 3;    // [18:16]
        HI_U32  reserved_4          : 12;   // [30:19]
        HI_U32  switch_fake_en      : 1;    // [31]
    } bits;

    HI_U32  value;
} U_SWITCH_CFG;

#define DMX_GLB_FLUSH                   (0x3A80)

typedef union
{
    struct
    {
        HI_U32  flush_ch            : 7;    // [6:0]
        HI_U32  reserved_0          : 1;    // [7]
        HI_U32  flush_type          : 2;    // [9:8]
        HI_U32  reserved_1          : 2;    // [11:10]
        HI_U32  flush_cmd           : 1;    // [12]
        HI_U32  reserved_2          : 3;    // [15:13]
        HI_U32  flush_done          : 1;    // [16]
        HI_U32  reserved_3          : 7;    // [23:17]
        HI_U32  clr_lost_cnt        : 3;    // [26:24]
        HI_U32  reserved_4          : 5;    // [31:27]
    } bits;

    HI_U32  value;
} U_DMX_GLB_FLUSH;

#define DMX_GLB_CTRL1(DmxId)            (0x3B00 + ((DmxId) << 4))   /* TS packet header parse control (DMX0~6)*/
#define DMX_GLB_CTRL2(DmxId)            (0x3B04 + ((DmxId) << 4))   /* TS record pointed buf(DMX0~6)*/
#define DMX_GLB_CTRL3(DmxId)            (0x3B08 + ((DmxId) << 4))   /* timeshift parameter set (DMX0~6) */

typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dmx_tei_ctrl          : 1   ; // [0]
        unsigned int    dmx_pesscr_flag_rve   : 1   ; // [1]
        unsigned int    dmx_tssc_flag_rve     : 1   ; // [2]
        unsigned int    dmx_cc_equ_judge_rule  : 1   ; // [3]
        unsigned int    dmx_cc_equ_pusi_rve   : 1   ; // [4]
        unsigned int    dmx_cc_disc_pusi_rve  : 1   ; // [5]
        unsigned int    dmx_tsp_pre_del       : 1   ; // [6]
        unsigned int    dmx_pid_mode          : 1   ; // [7]
        unsigned int    dmx_av_len_mode       : 1   ; // [8]
        unsigned int    cw_4word_reverse      : 1   ; // [9]
        unsigned int    cw_word_reverse       : 1   ; // [10]
        unsigned int    cw_byte_reverse       : 1   ; // [11]
        unsigned int    Reserved_0            : 20  ; // [31..12]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_GLB_CTRL1;


// Define the union U_DMX_GLB_CTRL2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dmx_tsrec_int_ch      : 7   ; // [6..0]
        unsigned int    Reserved_1            : 1   ; // [7]
        unsigned int    dmx_tsrec_buf         : 7   ; // [14..8]
        unsigned int    Reserved_0            : 17  ; // [31..15]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_GLB_CTRL2;

// Define the union U_DMX_GLB_CTRL3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dmx_spsply_pusi_ch    : 7   ; // [6..0]
        unsigned int    Reserved_1            : 1   ; // [7]
        unsigned int    dmx_spsrec_pusi_ch    : 8   ; // [15..8]
        unsigned int    dmx_sps_pusi_num      : 4   ; // [19..16]
        unsigned int    dmx_sps_type          : 1   ; // [20]
        unsigned int    Reserved_0            : 11  ; // [31..21]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_GLB_CTRL3;

typedef union
{
    struct
    {
        HI_U32  ip_nosync_len1      : 8;    // [7:0]
        HI_U32  ip_nosync_len2      : 8;    // [15:8]
    #ifdef HI_DMX_RAM_PORT_AUTO_SCAN_SUPPORT
        HI_U32  ip_nosync_region    : 8;    // [23:16]
        HI_U32  ip_nosync_step      : 2;    // [25:24]
        HI_U32  reserved0           : 6;    // [31:26]
    #else
        HI_U32  reserved0           : 16;   // [31:16]
    #endif
    } bits;

    HI_U32 value;
} U_DMX_SYNC_LEN_SET;

/******************************************************************************/
/*                      HiPVRV200 filter register definition                  */
/******************************************************************************/
#define DMX_FILTERxy(FlterId, depth)    (0x4000 + 64 * (FlterId) + 4 * (depth)) /* filter configuration register(software cann't read-back) */
#define DMX_FILTER_EN(ChanId)           (0x6000 + ((ChanId) << 2))      /*filter enable configuration register(aimed at 96 PID channels )*/
#define DMX_FILTER_MIN(ChanId)          (0x6200 + ((ChanId) << 2))      /* min filter byte set(aimed at 96 PID channels) */

#define DMX_FLT_NUM(FlterId)            (0x6400 + ((FlterId) << 2))
#define DMX_FILTER_CTRL                 (0x7A20)
#define DMX_FILTER_ID(ChanId, Offset)   (0x9000 + ((ChanId) << 5) + ((Offset) / 4 * 4))

typedef union
{
    struct
    {
        HI_U32  wdata_mask      : 8;    // [7:0]
        HI_U32  wdata_content   : 8;    // [15:8]
        HI_U32  wdata_mode      : 1;    // [16]
        HI_U32  reserved_0      : 15;   // [31:17]
    } bits;

    HI_U32  value;
} U_DMX_FILTER;

typedef union
{
    struct
    {
        HI_U32  flt_depth   : 5;    // [4:0]
        HI_U32  reserved_0  : 27;   // [31:5]
    } bits;

    HI_U32  value;
} U_DMX_FILTER_NUM;

typedef union
{
    struct
    {
        HI_U32  flt_start               : 1;    // [0]
        HI_U32  flt_byte_mode           : 1;    // [1]
        HI_U32  reserved_0              : 2;    // [3:2]
        HI_U32  sec_stuff_nopusi_e      : 1;    // [4]
        HI_U32  reserved_1              : 2;    // [6:5]
        HI_U32  sec_short_crc           : 1;    // [7]
        HI_U32  minlen_discard_by_flt   : 1;    // [8]
        HI_U32  minlen_mask             : 1;    // [9]
        HI_U32  point_err_mode          : 1;    // [10]
        HI_U32  point_err_correct       : 1;    // [11]
        HI_U32  point_newsec            : 1;    // [12]
        HI_U32  reserved_2              : 19;   // [31:13]
    } bits;

    HI_U32  value;
} U_DMX_FILTER_CTRL;

// Define the union U_IRAW_CLR_CHN
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iraw_clr_chn          : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IRAW_CLR_CHN;

/******************************************************************************/
/*                      HiPVR DAV_TOP register definition                     */
/******************************************************************************/
#define ENA_OQ_DESC_0(offset)           (0xC060 + ((offset) << 2))  /* the description sub-output interrupt enable of link channel 31~0 */
#define RAW_OQ_EOP_0(offset)            (0xC070 + ((offset) << 2))  /* the EOP raw interrupt of link channel 31 ~0 */
#define INT_OQ_EOP_0(offset)            (0xC080 + ((offset) << 2))  /* the EOP interrupt state of link channel 31~0 */
#define ENA_OQ_EOP_0(offset)            (0xC090 + ((offset) << 2))  /* the EOP interrupt enable of link channel 31~0 */
#define ENA_OQ_CHN_0(offset)            (0xC0C0 + ((offset) << 2))   /* the overflow or almost-overflow of link channel 31~0 interrrupt enble */
#define TYPE_OQ_CHN_0(offset)           (0xC0D0 + ((offset) << 2))   /* the overflow or almost-overflow of link channel 31~0 interrupt type denote */
#define RAW_CLR_CHN                     (0xC0E0)    /* clear channel operation finish raw interrupt */

#define INT_STA_TYPE                    (0xC100)    /* the same type interrupt interrupt state */
#define ENA_INT_TYPE                    (0xC104)    /* the same type interrupt interrupt enable */
#define INT_STA_ALL                     (0xC110)    /* the total interrupt state */
#define ENA_INT_ALL                     (0xC114)    /* the total interrupt enable */
#define OQ_ENA_0(offset)                (0xC130+ ((offset) << 2))   /* link channel output description sub-queue enable control bit, corresponding 31~0 queue */
#define FQ_ENA_0(offset)                (0xC140+ ((offset) << 2))   /* idle description sub-queue enable control bit, correspoding 31~0 queue */
#define FQ_INIT_DONE                    (0xC160)    /* idle description sub-queue logical automatic initialize finish denote */
#define OQ_INIT_DONE                    (0xC164)    /* output descript sub-queue logical automatic initialize finish denote */
#define CLR_WAIT_TIME                   (0xC180)    /* clear channel wait register */
#define CLR_CHN_CMD                     (0xC184)    /* clear channel start register */

typedef union
{
    struct
    {
        HI_U32  clear_chn       : 8;    // [7:0]
        HI_U32  clear_start     : 1;    // [8]
        HI_U32  clear_type      : 2;    // [10:9]
        HI_U32  reserved_0      : 21;   // [31:11]
    } bits;

    HI_U32  value;
} U_CLR_CHN_CMD;

#define IP_BP_CFG(PortId, offset)       (0xC210 + ((PortId) << 5) + ((offset) << 2))    /* IP channel 0 whether based on output description sub queue for counter-press enable config */
#define IP_BP_FQ_CFG(PortId, offset)    (0xC220 + ((PortId) << 5) + ((offset) << 2))    /* IP channel 0 whether based on free description sub queue for counter-press enable config */

#define IP_BP_FQ_STA(PortId, offset)    (0xC340 + ((PortId) << 3) + ((offset) << 2))

#define IP_DESC_SADDR(PortId)           (0xC800 + ((PortId) << 8))  /* IP channel 0 descrition queue start address */
#define IP_DESC_SIZE(PortId)            (0xC804 + ((PortId) << 8))  /* IP channel 0 description word queue size */
#define IP_DESC_PTR(PortId)             (0xC808 + ((PortId) << 8))  /* IP channel 0 description word queue read-write address */
#define IP_DESC_ADD(PortId)             (0xC810 + ((PortId) << 8))  /* toward IP channel 0 description word queue add description word item */
#define IP_INT_CNT_CFG(PortId)          (0xC818 + ((PortId) << 8))  /* IP channel 0 description word queue multi-word interrupt water-line set */
#define IP_SYNC_TH_CFG(PortId)          (0xC820 + ((PortId) << 8))  /* IP channel 0 sync check configuration */
#define IP_TS_RATE_CFG(PortId)          (0xC830 + ((PortId) << 8))  /* IP channel 0 output TS rate configuration */
#define IP_TS_OUT_STOP(PortId)          (0xC834 + ((PortId) << 8))  /* IP channnel 0 whether output TS  pause the configuration */
#define IP_CLRCHN_REQ(PortId)           (0xC840 + ((PortId) << 8))  /* IP channel 0 clear channel request */
#define IP_IRAW_INT(PortId)             (0xC880 + ((PortId) << 8))  /* IP channel 0 raw interrupt */
#define IP_IENA_INT(PortId)             (0xC884 + ((PortId) << 8))  /* IP channel 0 interrupt enable */
#define IP_IINT_INT(PortId)             (0xC888 + ((PortId) << 8))  /* IP channel 0 interrupt state */
#define IP_DBG_CNT_EN(PortId)           (0xC890 + ((PortId) << 8))  /* IP channel 0 debug counter enable control */
#define IP_DBG_OUT1(PortId)             (0xC898 + ((PortId) << 8))  /* IP channel 0 debug register  1 */
#define IP_SYNC_LEN(PortId)             (0xC8C0 + ((PortId) << 8))  /* IP port sync len config reg,just for chip mv300 */

#define ADDR_FQ_WORD0(FQId)             (0xD000 + ((FQId) << 4))    /* idle description sub queue the first WORD corresponding address */

typedef union
{
    struct
    {
        HI_U32  UsedCount       : 16;   // [15:0]
        HI_U32  Reserved0       : 8;    // [23:16]
        HI_U32  AlmostOverflow  : 8;    // [31:24]
    } bits;

    HI_U32  value;
} U_ADDR_FQ_WORD0;

#define ADDR_FQ_WORD1(FQId)             (0xD004 + ((FQId) << 4))

typedef union
{
    struct
    {
        HI_U32  Read        : 16;   // [15:0]
        HI_U32  FreeCount   : 16;   // [31:16]
    } bits;

    HI_U32  value;
} U_ADDR_FQ_WORD1;

#define ADDR_FQ_WORD2(FQId)             (0xD008 + ((FQId) << 4))

typedef union
{
    struct
    {
        HI_U32  Write   : 16;   // [15:0]
        HI_U32  Depth   : 16;   // [31:16]
    } bits;

    HI_U32  value;
} U_ADDR_FQ_WORD2;

#define ADDR_FQ_WORD3(FQId)             (0xD00C + ((FQId) << 4))

#define ADDR_INT_CNT(OQId)              (0xD800 + ((OQId) << 2))
#define CHANNEL_TS_COUNT(ChanId)        (0xDA00 + ((ChanId) << 2))

#define ADDR_OQ_WORD0(OQId)             (0xE000 + ((OQId) << 4))

typedef union
{
    struct
    {
        HI_U32  BQVal       : 8;    // [7:0]
        HI_U32  EopResByte  : 24;   // [31:8]
    } bits;

    HI_U32  value;
} U_ADDR_OQ_WORD0;

#define ADDR_OQ_WORD1(OQId)             (0xE004 + ((OQId) << 4))

typedef union
{
    struct
    {
        HI_U32  PVRCtrl     : 8;    // [7:0]
        HI_U32  WResByte    : 24;   // [31:8]
    } bits;

    HI_U32  value;
} U_ADDR_OQ_WORD1;

#define ADDR_OQ_WORD2(OQId)             (0xE008 + ((OQId) << 4))

typedef union
{
    struct
    {
        HI_U32  BBWaddr     : 16;   // [15:0]
        HI_U32  BBEopAddr   : 16;   // [31:16]
    } bits;

    HI_U32  value;
} U_ADDR_OQ_WORD2;

#define ADDR_OQ_WORD3(OQId)             (0xE00C + ((OQId) << 4))

typedef union
{
    struct
    {
        HI_U32  BQUse       : 14;   // [13:0]
        HI_U32  BQVal       : 2;    // [15:14]
        HI_U32  BBSize      : 16;   // [31:16]
    } bits;

    HI_U32  value;
} U_ADDR_OQ_WORD3;

#define ADDR_OQ_WORD4(OQId)             (0xE800 + ((OQId) << 4))
#define ADDR_OQ_WORD5(OQId)             (0xE804 + ((OQId) << 4))

typedef union
{
    struct
    {
        HI_U32  BQWPtr      : 10;   // [9:0]
        HI_U32  reserved_0  : 2;    // [11:10]
        HI_U32  BQIntCnt    : 4;    // [15:12]
        HI_U32  BQRPtr      : 10;   // [25:16]
        HI_U32  reserved_1  : 6;    // [31:26]
    } bits;

    HI_U32  value;
} U_ADDR_OQ_WORD5;

#define ADDR_OQ_WORD6(OQId)             (0xE808 + ((OQId) << 4))

typedef union
{
    struct
    {
        HI_U32  FqCfg       : 8;    // [7:0]
        HI_U32  BQAlovfl_TH : 8;    // [15:8]
        HI_U32  BQSize      : 10;   // [25:16]
        HI_U32  BQIntCfg    : 4;    // [29:26]
        HI_U32  reserved_0  : 2;    // [31:30]
    } bits;

    HI_U32  value;
} U_ADDR_OQ_WORD6;

#define ADDR_OQ_WORD7(OQId)             (0xE80C + ((OQId) << 4))

#endif  // __DEMUX_REG_H__

