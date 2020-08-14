/*******************************************************************************
*             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: drv_demux_reg.h
* Description: Register define for HiPVRV200
*
* History:
* Version   Date              Author    DefectNum       Description
* main\1    2009-09-27    y00106256     NULL            create this file.
*******************************************************************************/
#ifndef __DRV_DEMUX_REG_H__
#define __DRV_DEMUX_REG_H__

#include "hi_type.h"

#include "drv_demux_config.h"

#define DMX_READ_REG(base, offset)  ioread32((void *)(base + offset))
#define DMX_WRITE_REG(base, offset, value)   iowrite32(value, (void*)(base + offset))

#define DMX_SUBDEV_SHIFT_BIT        4
#define DMX_CHAN_SHIFT_BIT          2
#define DMX_SW_HW_OFFSET            1
#define DMX_DESC_SHIFT_BIT          4
#define DMX_BUF_SHIFT_BIT           2
#define DMX_SCD_SHIFT_BIT           2
#define DMX_CHAN_EOPADDR_OFFSET     64
#define DMX_WRITE_WAIT_TIME         10000
#define DMX_WORD_SHIFT_BIT          4
#define DMX_WORD_QUEQUE_CNT         32
#define DMX_QUEUE_DESC_LEN          16

/******************************************************************************/
/*                      HiPVRV200 dmx_int register definition                 */
/******************************************************************************/
#define PVR_INT_SCAN                    (0x0)       /* PVR interrupt look up register */
#define ENA_PVR_INT                     (0x4)       /* PVR interrupt enable register */
//#define RAW_INF_SYNC                    (0x100)     /* DVB synchronize or loss raw interrupt register */
#define RAW_PCR_ARRI                    (0x104)     /* pcr incoming raw interrupt register */
//#define RAW_SPS                         (0x108)     /* timeshift raw interrupt register */
#define RAW_TEI                         (0x10C)     /* tei raw interrupt register */

#define RAW_DISC0(offset)               (0x114 + ((offset) << 2))
#define RAW_FLTCRC0(offset)             (0x120 + ((offset) << 2))
#define RAW_PES_LEN0(offset)            (0x12C + ((offset) << 2))
//#define RAW_FLT_CLR                     (0x138)     /* flt clear channel raw interrupt */
//#define RAW_REC_CLR                     (0x13C)     /* rec cleat channel raw interrupt */
#define RAW_SWITCH_FULL_ERR              (0x0140)     /*the low 7 bit means demux 0~7 switch  buffer overflow*/

#define ENA_INF_SYNC                    (0x200)     /* DVB synchronize or loss interrupt enable */
#define ENA_PCR_ARRI                    (0x204)     /* pcr incomming interrupt enable */

#define STA_PCR_ARRI                    (0x304)     /* pcr incomming interrupt eable */

#define STA_TEI                         (0x30C)     /* tei interrupt state */

#define STA_DISC0(offset)               (0x314 + ((offset) << 2))   /* disc interrupt state 0 */
#define STA_FLTCRC0(offset)             (0x320 + ((offset) << 2))   /* flt crc error interrupt state 0 */
#define STA_PES_LEN0(offset)            (0x32C + ((offset) << 2))   /* flt pes len error interrupt state 0 */

#define PVR_CSA3_RST                    (0x0A34)
#define ALLREC_CFG0_29(RecCfgId)        (0x0C00 + ((RecCfgId) << 2))


typedef union
{
    struct
    {
        HI_U32  pvr_csa3_soft_rst   : 1;    // bit0
        HI_U32  reserved0           : 31;   // bit[31:1]
    } bits;

    HI_U32 value;
} U_PVR_CSA3_RST;

// Define the union U_PVR_INT_SCAN
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    total_int_peslen      : 1   ; // [0]
        unsigned int    total_int_fltcrc      : 1   ; // [1]
        unsigned int    total_int_disc        : 1   ; // [2]
        unsigned int    total_int_sps         : 1   ; // [3]
        unsigned int    total_int_pcr         : 1   ; // [4]
        unsigned int    total_int_clr         : 1   ; // [5]
        unsigned int    total_int_err         : 1   ; // [6]
        unsigned int    total_int_sync        : 1   ; // [7]
        unsigned int    total_int_dav         : 1   ; // [8]
        unsigned int    Reserved_1            : 7   ; // [15..9]
        unsigned int    total_raw_peslen      : 1   ; // [16]
        unsigned int    total_raw_fltcrc      : 1   ; // [17]
        unsigned int    total_raw_disc        : 1   ; // [18]
        unsigned int    total_raw_sps         : 1   ; // [19]
        unsigned int    total_raw_pcr         : 1   ; // [20]
        unsigned int    total_raw_clr         : 1   ; // [21]
        unsigned int    total_raw_err         : 1   ; // [22]
        unsigned int    total_raw_sync        : 1   ; // [23]
        unsigned int    Reserved_0            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_PVR_INT_SCAN;

// Define the union U_ENA_PVR_INT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ena_pvr_int           : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ENA_PVR_INT;

// Define the union U_RAW_INF_SYNC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    raw_inf1_syncon       : 1   ; // [0]
        unsigned int    raw_inf1_syncoff      : 1   ; // [1]
        unsigned int    raw_inf2_syncon       : 1   ; // [2]
        unsigned int    raw_inf2_syncoff      : 1   ; // [3]
        unsigned int    raw_inf3_syncon       : 1   ; // [4]
        unsigned int    raw_inf3_syncoff      : 1   ; // [5]
        unsigned int    Reserved_0            : 26  ; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_RAW_INF_SYNC;

// Define the union U_RAW_PCR_ARRI
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    raw_pcr               : 16  ; // [15..0]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_RAW_PCR_ARRI;

// Define the union U_RAW_SPS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    raw_sps               : 8   ; // [7..0]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_RAW_SPS;

// Define the union U_RAW_TEI
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    raw_tei               : 1   ; // [0]
        unsigned int    Reserved_2            : 7   ; // [7..1]
        unsigned int    tei_ch                : 7   ; // [14..8]
        unsigned int    Reserved_1            : 1   ; // [15]
        unsigned int    tei_dmx               : 3   ; // [18..16]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_RAW_TEI;

// Define the union U_RAW_ERR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    raw_err               : 1   ; // [0]
        unsigned int    Reserved_2            : 7   ; // [7..1]
        unsigned int    err_ch                : 7   ; // [14..8]
        unsigned int    Reserved_1            : 1   ; // [15]
        unsigned int    err_dmx               : 3   ; // [18..16]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_RAW_ERR;

// Define the union U_RAW_FLT_CLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    raw_flt_clr           : 1   ; // [0]
        unsigned int    Reserved_0            : 7   ; // [7..1]
        unsigned int    flt_clr_ch            : 7   ; // [14..8]
        unsigned int    resrved               : 17  ; // [31..15]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_RAW_FLT_CLR;

// Define the union U_RAW_REC_CLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    raw_rec_clr           : 1   ; // [0]
        unsigned int    Reserved_0            : 7   ; // [7..1]
        unsigned int    rec_clr_ch            : 7   ; // [14..8]
        unsigned int    resrved               : 17  ; // [31..15]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_RAW_REC_CLR;

// Define the union U_ENA_INF_SYNC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ena_inf1_syncon       : 1   ; // [0]
        unsigned int    ena_inf1_syncoff      : 1   ; // [1]
        unsigned int    ena_inf2_syncon       : 1   ; // [2]
        unsigned int    ena_inf2_syncoff      : 1   ; // [3]
        unsigned int    ena_inf3_syncon       : 1   ; // [4]
        unsigned int    ena_inf3_syncoff      : 1   ; // [5]
        unsigned int    Reserved_0            : 26  ; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ENA_INF_SYNC;

// Define the union U_ENA_PCR_ARRI
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ena_pcr               : 16  ; // [15..0]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ENA_PCR_ARRI;

// Define the union U_ENA_SPS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ena_sps               : 8   ; // [7..0]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ENA_SPS;

// Define the union U_ENA_TEI
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ena_tei               : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ENA_TEI;

// Define the union U_ENA_ERR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ena_err               : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ENA_ERR;

// Define the union U_ENA_FLT_CLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ena_flt_clr           : 1   ; // [0]
        unsigned int    Reserved_0            : 7   ; // [7..1]
        unsigned int    flt_clr_ch            : 7   ; // [14..8]
        unsigned int    resrved               : 17  ; // [31..15]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ENA_FLT_CLR;

// Define the union U_ENA_REC_CLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ena_rec_clr           : 1   ; // [0]
        unsigned int    Reserved_0            : 7   ; // [7..1]
        unsigned int    rec_clr_ch            : 7   ; // [14..8]
        unsigned int    resrved               : 17  ; // [31..15]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ENA_REC_CLR;

// Define the union U_STA_INF_SYNC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sta_inf1_syncon       : 1   ; // [0]
        unsigned int    sta_inf1_syncoff      : 1   ; // [1]
        unsigned int    sta_inf2_syncon       : 1   ; // [2]
        unsigned int    sta_inf2_syncoff      : 1   ; // [3]
        unsigned int    sta_inf3_syncon       : 1   ; // [4]
        unsigned int    sta_inf3_syncoff      : 1   ; // [5]
        unsigned int    Reserved_0            : 26  ; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STA_INF_SYNC;

// Define the union U_STA_PCR_ARRI
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sta_pcr               : 16  ; // [15..0]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STA_PCR_ARRI;

// Define the union U_STA_SPS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sta_sps               : 8   ; // [7..0]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STA_SPS;

// Define the union U_STA_TEI
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sta_tei               : 1   ; // [0]
        unsigned int    Reserved_2            : 7   ; // [7..1]
        unsigned int    tei_ch                : 7   ; // [14..8]
        unsigned int    Reserved_1            : 1   ; // [15]
        unsigned int    tei_dmx               : 3   ; // [18..16]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STA_TEI;

// Define the union U_STA_ERR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    err_tei               : 1   ; // [0]
        unsigned int    Reserved_2            : 7   ; // [7..1]
        unsigned int    err_ch                : 7   ; // [14..8]
        unsigned int    Reserved_1            : 1   ; // [15]
        unsigned int    err_dmx               : 3   ; // [18..16]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STA_ERR;

// Define the union U_STA_FLT_CLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sta_flt_clr           : 1   ; // [0]
        unsigned int    Reserved_0            : 7   ; // [7..1]
        unsigned int    flt_clr_ch            : 7   ; // [14..8]
        unsigned int    resrved               : 17  ; // [31..15]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STA_FLT_CLR;

// Define the union U_STA_REC_CLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sta_rec_clr           : 1   ; // [0]
        unsigned int    Reserved_0            : 7   ; // [7..1]
        unsigned int    rec_clr_ch            : 7   ; // [14..8]
        unsigned int    resrved               : 17  ; // [31..15]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STA_REC_CLR;

// Define the union U_STA_ALLREC_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    recdel_pid_value      : 13  ; // [12..0]
        unsigned int    Reserved_0            : 3   ; // [15..13]
        unsigned int    recdel_pid_dmxid      : 3   ; // [18..16]
        unsigned int    resrved               : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    value;

} U_STA_ALLREC_CFG;

#define TS_COUNT_CRTL_RESET     (0)
#define TS_COUNT_CRTL_START     (1)
#define TS_COUNT_CRTL_STOP      (2)

/******************************************************************************/
/*                      HiPVRV200 interface regeister definition              */
/******************************************************************************/
#define TS_TAG_MUX_CONFIG              (0x10f0)
#define TS_TAG_MUX_SOURCE              (0x10f8)
#define TS_TAG_LOW_REG(TSMuxId)    (0x1000 + ((TSMuxId) << 4))
#define TS_TAG_MID_REG(TSMuxId)    (0x1004 + ((TSMuxId) << 4))
#define TS_TAG_HIGH_REG(TSMuxId)    (0x1008 + ((TSMuxId) << 4))
#define TS_DBG_DVBT_INTERFACE(PortId)    (0x11d4 + ((PortId) << 8))

#define TS_INTERFACE(PortId)            (0x1100 + ((PortId) << 8))  /* interface register */
#define TS_COUNT(PortId)                (0x1104 + ((PortId) << 8))  /* all the packet counter register */
#define TS_COUNT_CTRL(PortId)           (0x1108 + ((PortId) << 8))  /* all the packet counter control register */
#define ETS_COUNT(PortId)               (0x110C + ((PortId) << 8))  /* error packet counter register */
#define ETS_COUNT_CTRL(PortId)          (0x1110 + ((PortId) << 8))  /* error packet counter control register */

typedef union
{
    struct
    {
        HI_U32 mux_dvb_inf_sel           :4;    //[3:0]
        HI_U32 reserved                       :28;   //[31:4]
    }bits;

    HI_U32 u32;
}U_MUX_SOURCE;

typedef union
{
    struct
    {
        HI_U32  tag_length                  : 4;    // [3:0]
        HI_U32  sync_mode_sel           : 1;    // [4]
        HI_U32  reserved                     : 26;    // [30:5]
        HI_U32  mux_work_en             : 1;    // [31]
    }bits;

    HI_U32 u32;
}U_MUX_CONFIG;

typedef union
{
    struct 
    {
        HI_U32 dbg_ser_allsave          :1 ;  //[0]
        HI_U32 reserved1                     :7;   //[1:7]
        HI_U32 dbg_ser_bit_sel           :3;   //[8:10]
        HI_U32 reserved2                      :1;  //[11]
        HI_U32 dbg_ser_len_bypass     :1;  //[12]
        HI_U32 reserved                      :19; //[13:31]
    }bits;
    
    HI_U32 u32;
}U_DBG_DVBT;

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
        HI_U32  reserved_4          : 2;    // [27:26]
        HI_U32  sync_clear          : 1;    // [28]
        HI_U32  serial_sel          : 1;    // [29]
        HI_U32  bit_sel             : 1;    // [30]
        HI_U32  port_sel            : 1;    // [31]
    } bits;

    HI_U32  u32;
} U_TS_INTERFACE;

// Define the union U_TS_COUNT_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ts_count_ctrl         : 2   ; // [1..0]
        unsigned int    Reserved_0            : 30  ; // [31..2]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_TS_COUNT_CTRL;

// Define the union U_ETS_COUNT_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ets_count_ctrl        : 2   ; // [1..0]
        unsigned int    Reserved_0            : 30  ; // [31..2]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ETS_COUNT_CTRL;

// Define the union U_TS_AFIFO_WFULL_ERR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wfull_err             : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_TS_AFIFO_WFULL_ERR;

// Define the union U_TS_AFIFO_WFULL_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    full                  : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_TS_AFIFO_WFULL_STATUS;

// Define the union U_TS_READ_FIFO_FSM
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fsm_st                : 4   ; // [3..0]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_TS_READ_FIFO_FSM;

// Define the union U_TS_SYNC_FSM
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fsm_sync204           : 4   ; // [3..0]
        unsigned int    Reserved_4            : 4   ; // [7..4]
        unsigned int    fsm_sync188           : 4   ; // [11..8]
        unsigned int    Reserved_3            : 4   ; // [15..12]
        unsigned int    switch_info           : 3   ; // [18..16]
        unsigned int    Reserved_2            : 5   ; // [23..19]
        unsigned int    period_sync_on        : 1   ; // [24]
        unsigned int    Reserved_1            : 3   ; // [27..25]
        unsigned int    period_sync_off       : 1   ; // [28]
        unsigned int    Reserved_0            : 3   ; // [31..29]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_TS_SYNC_FSM;

// Define the union U_TS_SYNC_FLAG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sync_204_success      : 1   ; // [0]
        unsigned int    sync_188_success      : 1   ; // [1]
        unsigned int    Reserved_0            : 30  ; // [31..2]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_TS_SYNC_FLAG;


/******************************************************************************/
/*                      HiPVRV200 parser  register definiton                   */
/******************************************************************************/
#define DMX_PID_CTRL(ChanId)            (0x3000 + ((ChanId) << 2))  /* PID channel set */
#define DMX_PID_EN(ChanId)              (0x3200 + ((ChanId) << 2))  /* PID channel enable */
#define DMX_PID_VALUE(ChanId)           (0x3400 + ((ChanId) << 2))  /* PID value set */
#define DMX_PID_REC_BUF(ChanId)         (0x3600 + ((ChanId) << 2))  /* pointed PID channel record buf number */
#define DMX_PID_PLAY_BUF(ChanId)        (0x3800 + ((ChanId) << 2))  /* pointed PID channel play buf number  */

#define SWITCH_CFG0                     (0x3A00)
#define SWITCH_CFG1                     (0x3A04)
#define SWITCH_FAKE_EN                (0x3A10)

#define TSOUT_CFG(PortId)               (0x3a08 + ((PortId) << 2))  /* tso CONFIG register */

#define DMX_SWITCH_STATE20       (0x3a28)

#define DMX_CTRL_FUNC                   (0x3A30)    /* timeshift record control cmd */
#define DMX_SPS_EXTRA                   (0x3A34)    /* timeshift dmx set register */

#define DMX_GLB_FLUSH                   (0x3A80)    /* clear channel set register */
#define DMX_PCR_SET(PcrId)              (0x3A90 + ((PcrId) << 2))   /* PCR channel painted register */
#define DMX_SCR_VALUE1                  (0x3ADC)   /* Low 32bit value of Current SCR ,generated by 90K clk */

#define DMX_GLB_CTRL1(DmxId)            (0x3B00 + ((DmxId) << 4))   /* TS packet header parse control (DMX0~6)*/
#define DMX_GLB_CTRL2(DmxId)            (0x3B04 + ((DmxId) << 4))   /* TS record pointed buf(DMX0~6)*/
#define DMX_GLB_CTRL3(DmxId)            (0x3B08 + ((DmxId) << 4))   /* timeshift parameter set (DMX0~6) */

#define CH_HIS(ChanId)                  (0x3C00 + ((ChanId) << 2))  /* scramble flag */

#define DMX_MODE_0                      (0X3E40)

#define DMX_CH_PCR_VALUE0(PcrId)        (0x3F00 + ((PcrId) << 4))
#define DMX_CH_PCR_VALUE1(PcrId)        (0x3F04 + ((PcrId) << 4))
#define DMX_CH_SCR_VALUE0(PcrId)        (0x3F08 + ((PcrId) << 4))
#define DMX_CH_SCR_VALUE1(PcrId)        (0x3F0C + ((PcrId) << 4))

// Define the union U_DMX_PID_CTRLn
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    data_type             : 2   ; // [1..0]
        unsigned int    cw_index              : 5   ; // [6..2]
        unsigned int    do_scram              : 1   ; // [7]
        unsigned int    af_mode               : 2   ; // [9..8]
        unsigned int    crc_mode              : 3   ; // [12..10]
        unsigned int    cc_discon_ctrl        : 1   ; // [13]
        unsigned int    pusi_disable          : 1   ; // [14]
        unsigned int    cc_equ_rve            : 1   ; // [15]
        unsigned int    ts_post_mode          : 1   ; // [16]
        unsigned int    ts_post_threshold     : 6   ; // [22..17]
        unsigned int    ch_attri              : 3   ; // [25..23]
        unsigned int    Reserved_0            : 6   ; // [31..26]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_PID_CTRL;

typedef union
{
    struct
    {
        HI_U32  byte_endian             : 1;    // [0]
        HI_U32  sync_pos                : 1;    // [1]
        HI_U32  vld_mode                : 1;    // [2]
        HI_U32  dis                     : 1;    // [3]
        HI_U32  clkgt_mode              : 1;    // [4]
        HI_U32  spi_mode                : 1;    // [5]
        HI_U32  serial_bit_sel          : 1;    // [6]
        HI_U32  reserved_1              : 1;    // [7]
        HI_U32  tsout_source_sel        : 8;    // [15:8]
        HI_U32  tsout0_press_en         : 1;   // [16]
        HI_U32  reserved_2              : 15;   // [31:17]
    } bits;
    HI_U32  u32;
} U_TSOUT_CFG;

// Define the union U_DMX_PID_EN
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    pid_play_dmx_id       : 3   ; // [2..0]
        unsigned int    Reserved_1            : 1   ; // [3]
        unsigned int    pid_rec_dmx_id        : 3   ; // [6..4]
        unsigned int    Reserved_0            : 25  ; // [31..7]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_PID_EN;

// Define the union U_DMX_PID_VALUE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    pid_value             : 13  ; // [12..0]
        unsigned int    pid_extra             : 1   ; // [13]
        unsigned int    Reserved_0            : 18  ; // [31..14]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_PID_VALUE;

// Define the union U_DMX_PID_REC_BUF
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    rec_buf               : 7   ; // [6..0]
        unsigned int    Reserved_1            : 1   ; // [7]
        unsigned int    flt_hit_mode          : 1   ; // [8]
        unsigned int    rec_tsph_nocheck    : 1 ; // [9]
        unsigned int    com_pid_indicator     : 1 ; //[10]
        unsigned int    Reserved_0            : 23  ; // [31..11]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_PID_REC_BUF;

// Define the union U_DMX_PID_PLAY_BUF
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    play_buf              : 7   ; // [6..0]
        unsigned int    psi_crc_sub_en        : 1   ; // [7]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_PID_PLAY_BUF;

// Define the union U_SWITCH_CFG0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    switch_cfg1           : 8   ; // [7..0]
        unsigned int    switch_cfg2           : 8   ; // [15..8]
        unsigned int    switch_cfg3           : 8   ; // [23..16]
        unsigned int    switch_cfg4           : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SWITCH_CFG0;

// Define the union U_SWITCH_CFG1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    switch_cfg5           : 8   ; // [7..0]
        unsigned int    switch_cfg6           : 8   ; // [15..8]
        unsigned int    switch_cfg7           : 8   ; // [23..16]
        unsigned int    Reserved_0            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SWITCH_CFG1;

typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_0            : 31  ; // [30..0]
        unsigned int    switch_fake_en        : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SWITCH_FAKE_EN;

// Define the union U_SWITCH_BLOCK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_1            : 7   ; // [6..0]
        unsigned int    Reserved_0            : 25  ; // [31..7]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SWITCH_BLOCK;

// Define the union U_DMX_CTRL_FUNC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dmx1_sps_ctrl         : 1   ; // [0]
        unsigned int    dmx1_rec_ctrl         : 3   ; // [3..1]
        unsigned int    dmx2_sps_ctrl         : 1   ; // [4]
        unsigned int    dmx2_rec_ctrl         : 3   ; // [7..5]
        unsigned int    dmx3_sps_ctrl         : 1   ; // [8]
        unsigned int    dmx3_rec_ctrl         : 3   ; // [11..9]
        unsigned int    dmx4_sps_ctrl         : 1   ; // [12]
        unsigned int    dmx4_rec_ctrl         : 3   ; // [15..13]
        unsigned int    dmx5_sps_ctrl         : 1   ; // [16]
        unsigned int    dmx5_rec_ctrl         : 3   ; // [19..17]
        unsigned int    dmx6_sps_ctrl         : 1   ; // [20]
        unsigned int    dmx6_rec_ctrl         : 3   ; // [23..21]
        unsigned int    dmx7_sps_ctrl         : 1   ; // [24]
        unsigned int    dmx7_rec_ctrl         : 3   ; // [27..25]
        unsigned int    Reserved_0            : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_CTRL_FUNC;


// Define the union U_DMX_SPS_EXTRA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sps_rec_dmx_id        : 3   ; // [2..0]
        unsigned int    Reserved_2            : 1   ; // [3]
        unsigned int    sps_play_dmx_id       : 3   ; // [6..4]
        unsigned int    Reserved_1            : 1   ; // [7]
        unsigned int    sps_dmx_priority      : 1   ; // [8]
        unsigned int    Reserved_0            : 23  ; // [31..9]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_SPS_EXTRA;


// Define the union U_DMX_GLB_FLUSH
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    flush_ch              : 7   ; // [6..0]
        unsigned int    Reserved_5            : 1   ; // [7]
        unsigned int    flush_type            : 2   ; // [9..8]
        unsigned int    Reserved_4            : 2   ; // [11..10]
        unsigned int    flush_cmd             : 1   ; // [12]
        unsigned int    Reserved_3            : 3   ; // [15..13]
        unsigned int    flush_done            : 1   ; // [16]
        unsigned int    Reserved_2            : 7   ; // [23..17]
        unsigned int    Reserved_1            : 3   ; // [26..24]
        unsigned int    Reserved_0            : 5   ; // [31..27]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_GLB_FLUSH;

// Define the union U_DMX_PCR_SETJ
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    pcr_pid               : 13  ; // [12..0]
        unsigned int    Reserved_1            : 3   ; // [15..13]
        unsigned int    pcr_dmx_id            : 3   ; // [18..16]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_PCR_SET;

// Define the union U_DMX_GLB_CTRL1
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
        unsigned int    dmx_afcheck_dis       : 1   ; // [12]
        unsigned int    dmx_47_replace        : 1   ; // [13]
        unsigned int    dmx_allrec_neg_en     : 1   ; // [14]
        unsigned int    dmx_sec_tei_ctrl      : 1   ; // [15]
        unsigned int    dmx_pes_tei_ctrl      : 1   ; // [16]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    value;

} U_DMX_GLB_CTRL1;


// Define the union U_DMX_GLB_CTRL2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dmx_tsrec_int_ch    : 7   ; // [6..0]
        unsigned int    Reserved_2             : 1   ; // [7]
        unsigned int    dmx_tsrec_buf         : 7   ; // [14..8]
        unsigned int    Reserved_1              : 1  ; // [15]
        unsigned int    tsrec_pidbuf_en       : 1  ; // [16]
        unsigned int    Reserved_0             : 17  ; // [31..17]
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
        HI_U32  cw_word_id      : 2;    // [1:0]
        HI_U32  reserved0       : 6;    // [7:2]
        HI_U32  cw_odd_even     : 1;    // [8]
        HI_U32  cw_group_id     : 5;    // [13:9]
        HI_U32  reserved1       : 2;    // [15:14]
        HI_U32  cw_type         : 8;    // [23:16]
        HI_U32  cw_iv_sel       : 1;    // [24]
        HI_U32  reserved2       : 7;    // [31:25]
    } bits;

    HI_U32  value;
} U_CW_SET1;

// Define the union U_CH_HIS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_2            : 12  ; // [11..0]
        unsigned int    dat_pas_flag          : 1   ; // [12]
        unsigned int    Reserved_1            : 5  ; // [17..13]
        unsigned int    pes_scr_flag          : 1   ; // [18]
        unsigned int    ts_scr_flag           : 1   ; // [19]
        unsigned int    Reserved_0            : 12  ; // [31..20]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CH_HIS;

// Define the union U_DMX_MODE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int sw_pidcopy_en         : 1; //[0]
        unsigned int sw_pidcopy_mode_01 :1; //[1]
        unsigned int sw_mode_02              :2; //[2]
        unsigned int reserved                       :28; //[31 .. 3]
    }bits;

    // Define an unsigned member
    unsigned int u32;
}U_DMX_MODE;

// Define the union U_DMX_CH_PCR_VALUE0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    pcr_base_32           : 1   ; // [0]
        unsigned int    Reserved_1            : 7   ; // [7..1]
        unsigned int    pcr_extra_8_0         : 9   ; // [16..8]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_CH_PCR_VALUE0;

// Define the union U_DMX_CH_SCR_VALUE0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    scr_base_32           : 1   ; // [0]
        unsigned int    Reserved_1            : 7   ; // [7..1]
        unsigned int    scr_extra_8_0         : 9   ; // [16..8]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_CH_SCR_VALUE0;


// Define the union U_DMX_TSOUT_CFG,for mv300
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    byte_endian           : 1   ; // [0]
        unsigned int    sync_pos              : 1   ; // [1]
        unsigned int    vld_mode              : 1   ; // [2]
        unsigned int    dis                   : 1   ; // [3]
        unsigned int    clkgt_mode            : 1   ; // [4]
        unsigned int    spi_mode              : 1   ; // [5]
        unsigned int    Reserved_0            : 26  ; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_TSOUT_CFG;

// Define the union U_CA_DBG_TYPE_RD,for mv300
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    group_id              : 5   ; // [4..0]
        unsigned int    Reserved_1            : 3   ; // [7..5]
        unsigned int    table_id              : 2   ; // [9..8]
        unsigned int    Reserved_0            : 22  ; // [31..10]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CA_DBG_TYPE_RD;

// Define the union U_DMX_CW_SET,for mv300
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dmx_cw_index            : 5   ; // [4..0]
        unsigned int    Reserved_1              : 1   ; // [5]
        unsigned int    dmx_cw_tabid            : 2   ; // [7..6]
        unsigned int    dmx_cw_open             : 1   ; // [8]
        unsigned int    dmx_cw_nocare_syntax    : 1   ; // [9]
        unsigned int    dmx_cw_dsc_chg_flag     : 1   ; // [10]
        unsigned int    cw_iv_en                : 1   ; // [11]
        unsigned int    dmx_cw_scr_chg_flag     : 1   ; // [12]
        unsigned int    Reserved_0              : 18  ; // [31..13]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_CW_SET;

typedef union
{
    struct
    {
        HI_U32  ip_nosync_len1      : 8;    // [7:0]
        HI_U32  ip_nosync_len2      : 8;    // [15:8]
    #ifdef DMX_RAM_PORT_AUTO_SCAN_SUPPORT
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

// Define the union U_DMX_FILTERxy
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wdata_mask            : 8   ; // [7..0]
        unsigned int    wdata_content         : 8   ; // [15..8]
        unsigned int    wdata_mode            : 1   ; // [16]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_FILTER;

// Define the union U_DMX_FILTER_MINn
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dmx_flt_min_byte      : 4   ; // [3..0]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_FILTER_MIN;

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

// Define the union U_DMX_FILTER_ID
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    flt_id0      : 8   ; // [7..0]
        unsigned int    flt_id1      : 8   ; // [15..8]
        unsigned int    flt_id2      : 8   ; // [23..16]
        unsigned int    flt_id3      : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_FILTER_ID;


#ifndef DMX_SCD_VERSION_2
/******************************************************************************/
/*                      HiPVRV200 DMX_SCD register definition                 */
/******************************************************************************/
#define SCD_FLT0_3                      (0x8000)    /* filter 0_3 filtrate es stream behind 000001 one start code byte, if match, scd index create.
relation of among filter is or, total 10 byte filter. the same below .*/

#define SCD_FLT8_11                     (0x8008)    /* range filter register 0 */
#define SCD_SETa(ScdId)                 (0x8020 + ((ScdId) << 2))   /* SCD0~7 configuration */
//#define TPIT_SETb                       (0x8060)    /* TPIT0~9 set */
//#define PREFIX_BYTE_Y                   (0x8090)    /* reserved. for the pointed 3 byte create index(default, used for the short header start code prefix)*/
//#define PREFIX_MASK_Y                   (0x8094)    /* reserved. pointed byte shield(default,used for the short header start code prefix) */
//#define TSDMX_BUFID                     (0x8098)    /* reserved. when pointed ts record, dmx corresponding to scd buf id */
#define SCD_BUF0(ScdId)                 (0x80A0 + ((ScdId) << 2))   /* scd_buf0, corresponding dav 128 buf, the same below */
#define SCD_CLRCH                       (0x80C0)    /* write this register, will cause clear operation once, clear SCD channel state info*/
#define SCD_CLRBUF                      (0x80C4)    /* write this register, will cause clear operation once, clear SCD buffer state info */
//#define SCD_DBG0_5                      (0x8100)    /* reserved, debug */
//#define SCD_CNT0_3                      (0x8120)
#define SCD_NEW_FLTEN(ScdId)            (0x8200 + ((ScdId) << 2))       /* new SCD's flten 0~15 configuration */
#define SCD_NEW_FLTSET(FilterId)        (0x8220 + ((FilterId) << 2))    /* new range register 0 */
#define SCD_NEW_FLT_NEG                 (0x8260)    /* new filter negate set */

//#define SCD_INFO                        (0x8400)    /* reserved, debug, read-write SCD RAM. one-line 128 bits, from low to high sequence access.*/

// Define the union U_SCD_FLT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    flt0                  : 8   ; // [7..0]
        unsigned int    flt1                  : 8   ; // [15..8]
        unsigned int    flt2                  : 8   ; // [23..16]
        unsigned int    flt3                  : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SCD_FLT;

// Define the union U_SCD_FLT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    byte_h                  : 8   ; // [7..0]
        unsigned int    byte_l                  : 8   ; // [15..8]
        unsigned int    mask                    : 8   ; // [23..16]
        unsigned int    reserved                : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SCD_FLT_V200;


// Define the union U_SCD_SETa
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    scd_ch                : 7   ; // [6..0]
        unsigned int    Reserved_0            : 1   ; // [7]
        unsigned int    flt_en0               : 10  ; // [17..8]
        unsigned int    flt_en1               : 2  ; // [19..18]
        unsigned int    m4_short_en           : 1   ; // [20]
        unsigned int    esscd_en              : 1   ; // [21]
        unsigned int    pes_en                : 1   ; // [22]
        unsigned int    tpit_en               : 1   ; // [23]
        unsigned int    scd_bufid             : 3   ; // [26..24]
        unsigned int    flt_en2               : 5   ; // [31..27]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SCD_SET;

// Define the union U_TPIT_SETb
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tpit_ch               : 7   ; // [6..0]
        unsigned int    tpit_en               : 1   ; // [7]
        unsigned int    tpit_bufid            : 3   ; // [10..8]
        unsigned int    Reserved_0            : 21  ; // [31..11]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_TPIT_SET;

// Define the union U_PREFIX_BYTE_Y
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ep_byte_y             : 24  ; // [23..0]
        unsigned int    Reserved_0            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_PREFIX_BYTE_Y;

// Define the union U_PREFIX_MASK_Y
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ep_mask_y             : 24  ; // [23..0]
        unsigned int    Reserved_0            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_PREFIX_MASK_Y;

// Define the union U_SCD_BUF
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    buf_num               : 7   ; // [6..0]
        unsigned int    Reserved_0            : 25  ; // [31..7]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SCD_BUF;

// Define the union U_SCD_CLRCH
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ch_id                 : 3   ; // [2..0]
        unsigned int    Reserved_0            : 29  ; // [31..3]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SCD_CLRCH;

// Define the union U_SCD_CLRBUF
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    buf_id                : 3   ; // [2..0]
        unsigned int    Reserved_0            : 29  ; // [31..3]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SCD_CLRBUF;

#else
#define SCD_SET(ScdId)                                     (0x8000 + 12UL * ScdId)    
#define SCD_BUF_SET(RecId)                             (0x8540 + (RecId << 2))
#define SCD_CLRCH                                           (0x8500)    
#define SCD_CLRBUF                                         (0x8504) 
#define SCD_BYTE_FLT(GrpOffset)                        (0x8300 + GrpOffset)  
#define SCD_BYTE_FLT_EN(ScdId)                        (0x8004 + 12UL * ScdId)
#define SCD_RANGE_FLT(FltId)                            (0x8310 + (FltId << 2))
#define SCD_RANGE_FLT_EN(ScdId)                      (0x8008 + 12UL * ScdId)

typedef union
{
    struct
    {
        unsigned int rec_pid_ch                   :8  ; // [0 .. 7]
        unsigned int reserved1                     :3  ;// [8 .. 10]
        unsigned int pid_ch_vld                    :1  ;//[11]
        unsigned int scd_buf_id                    :6  ;// [12 .. 17]
        unsigned int reserved2                     :6  ;//[18 .. 23]
        unsigned int scd_es_short_en           :1  ;// [24]
        unsigned int scd_es_en                     :1   ; // [25]
        unsigned int scd_pes_en                    :1  ;// [26]
        unsigned int scd_tpit_en                    :1 ;// [27]
        unsigned int reserved3                       :4 ;// [28 .. 31]
    }bits;

    unsigned int u32;
}U_SCD_SET;

typedef union
{
    struct
    {
        unsigned int buf_num                         :8  ;// [0 .. 7]
        unsigned int reserved                         :24 ; // [8 .. 31]
    }bits;

    unsigned int u32;
}U_SCD_BUF_SET;

typedef union
{
    struct
    {
        unsigned int    ch_id                 : 6   ; // [0..5]
        unsigned int    Reserved_0            : 26  ; // [6..31]
    } bits;

    unsigned int    u32;

} U_SCD_CLRCH;

typedef union
{
    struct
    {
        unsigned int    buf_id                : 6   ; // [0..5]
        unsigned int    Reserved_0            : 26  ; // [6..31]
    } bits;

    unsigned int    u32;

} U_SCD_CLRBUF;

typedef union
{
    struct
    {
        unsigned int    flt0                  : 8   ; // [7..0]
        unsigned int    flt1                  : 8   ; // [15..8]
        unsigned int    flt2                  : 8   ; // [23..16]
        unsigned int    flt3                  : 8   ; // [31..24]
    } bits;

    unsigned int    u32;

} U_SCD_FLT;

typedef union
{
    struct
    {
        unsigned int flt0_en                 :1  ;//[0]
        unsigned int flt1_en                 :1  ;//[1]
        unsigned int flt2_en                 :1  ;//[2]
        unsigned int flt3_en                 :1  ;//[3]
        unsigned int flt4_en                 :1  ;//[4]
        unsigned int flt5_en                 :1  ;//[5]
        unsigned int flt6_en                 :1  ;//[6]
        unsigned int flt7_en                 :1  ;//[7]
        unsigned int flt8_en                 :1  ;//[8]
        unsigned int flt9_en                 :1  ;//[9]
        unsigned int flt10_en                 :1  ;//[10]
        unsigned int flt11_en                 :1  ;//[11]
        unsigned int flt12_en                 :1  ;//[12]
        unsigned int flt13_en                 :1  ;//[13]
        unsigned int flt14_en                 :1  ;//[14]
        unsigned int flt15_en                 :1  ;//[14]
        unsigned int reserved                :16 ;//[16 .. 31]
    }bits;

    unsigned int  u32;
}U_SCD_BYTE_FLT_EN;

typedef union
{
    struct 
    {
        unsigned int lo                           :8  ;//[0.. 7]
        unsigned int hi                            :8  ; //[8 .. 15]
        unsigned int mask                       :8  ; //[16 .. 23]
        unsigned int reserved                   :8  ; // [24 .. 31]
    }bits;

    unsigned int u32;
}U_SCD_RANGE_FLT;

typedef union
{
    struct
    {
        unsigned int flt0_en                 :1  ;//[0]
        unsigned int flt1_en                 :1  ;//[1]
        unsigned int flt2_en                 :1  ;//[2]
        unsigned int flt3_en                 :1  ;//[3]
        unsigned int flt4_en                 :1  ;//[4]
        unsigned int flt5_en                 :1  ;//[5]
        unsigned int flt6_en                 :1  ;//[6]
        unsigned int flt7_en                 :1  ;//[7]
        unsigned int flt8_en                 :1  ;//[8]
        unsigned int flt9_en                 :1  ;//[9]
        unsigned int flt10_en                 :1  ;//[10]
        unsigned int flt11_en                 :1  ;//[11]
        unsigned int flt12_en                 :1  ;//[12]
        unsigned int flt13_en                 :1  ;//[13]
        unsigned int flt14_en                 :1  ;//[14]
        unsigned int flt15_en                 :1  ;//[15]
        unsigned int flt16_en                 :1  ;//[16]
        unsigned int flt17_en                 :1  ;//[17]
        unsigned int flt18_en                 :1  ;//[18]
        unsigned int flt19_en                 :1  ;//[19]
        unsigned int flt20_en                 :1  ;//[20]
        unsigned int flt21_en                 :1  ;//[21]
        unsigned int flt22_en                 :1  ;//[22]
        unsigned int flt23_en                 :1  ;//[23]
        unsigned int reserved                :8 ;//[24 .. 31]
    }bits;

    unsigned int u32;
}U_SCD_RANGE_FLT_EN;

#define REC_BUF_SET(RecId)                              (0x8400 + (RecId << 2))
#define DMX_REC_SET(DmxId)                            (0x84d0 + (DmxId << 2))

typedef union
{
    struct
    {
        unsigned int rec_buf_id                : 8   ;// [0 .. 7]
        unsigned int reserved1                 : 4   ;// [8..11]
        unsigned int rec_buf_vld               : 1   ;//[12]
        unsigned int reserved2                 : 19 ;//[13..31]
    }bits;

    unsigned int u32;
}U_REC_BUF_SET;

#endif

// Define the union U_ENA_ALL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iena_all              : 1   ; // [0]
        unsigned int    iena_fix_all          : 1   ; // [1]
        unsigned int    iena_eop_all          : 1   ; // [2]
        unsigned int    iena_ovfl_all         : 1   ; // [3]
        unsigned int    iena_alovfl_all       : 1   ; // [4]
        unsigned int    iena_time_all         : 1   ; // [5]
        unsigned int    iena_clear_chn        : 1   ; // [6]
        unsigned int    iena_scd_index        : 6   ; // [12..7]
        unsigned int    ena_d2b_f_ovfl        : 1   ; // [13]
        unsigned int    ena_d2b_r_ovfl        : 1   ; // [14]
        unsigned int    ena_ef_alovfl         : 1   ; // [15]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ENA_ALL;

// Define the union U_INT_ALL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iint_all              : 1   ; // [0]
        unsigned int    iint_fix_all          : 1   ; // [1]
        unsigned int    iint_eop_all          : 1   ; // [2]
        unsigned int    iint_ovfl_all         : 1   ; // [3]
        unsigned int    iint_alovfl_all       : 1   ; // [4]
        unsigned int    iint_time_all         : 1   ; // [5]
        unsigned int    iint_clear_chn        : 1   ; // [6]
        unsigned int    iint_scd_index        : 6   ; // [12..7]
        unsigned int    int_d2b_f_ovfl        : 1   ; // [13]
        unsigned int    int_d2b_r_ovfl        : 1   ; // [14]
        unsigned int    int_ef_alovfl         : 1   ; // [15]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_INT_ALL;

// Define the union U_DMX_AF_BUFID
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dmx_af_bufid         : 8   ; // [7..0]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DMX_AF_BUFID;


// Define the union U_TIMEOUT_ENA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    timeout_ena           : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_TIMEOUT_ENA;

// Define the union U_IP_BP_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ipaful_clr_time       : 24  ; // [23..0]
        unsigned int    ipaful_clr_ena        : 4   ; // [27..24]
        unsigned int    avbuf_alovfl_th       : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_BP_CFG;

// Define the union U_IP_CHN_BP_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_chn0_bp_bufid     : 8   ; // [7..0]
        unsigned int    ip_chn0_bp_ena       : 1   ; // [8]
        unsigned int    Reserved_1            : 7   ; // [15..9]
        unsigned int    ip_chn1_bp_bufid     : 8   ; // [23..16]
        unsigned int    ip_chn1_bp_ena       : 1   ; // [24]
        unsigned int    Reserved_0            : 7   ; // [31..25]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_CHN_BP_CFG;




// Define the union U_SBUF_INIT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sbuf_init             : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SBUF_INIT;

// Define the union U_SBUF_CMD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sbuf_chn              : 8   ; // [7..0]
        unsigned int    sbuf_type             : 2   ; // [9..8]
        unsigned int    Reserved_3            : 2   ; // [11..10]
        unsigned int    sbuf_addr             : 3   ; // [14..12]
        unsigned int    Reserved_2            : 1   ; // [15]
        unsigned int    sbuf_mask             : 4   ; // [19..16]
        unsigned int    sbuf_wr               : 1   ; // [20]
        unsigned int    Reserved_1            : 2   ; // [22..21]
        unsigned int    sbuf_ena              : 1   ; // [23]
        unsigned int    Reserved_0            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SBUF_CMD;

// Define the union U_SBUF_RDY
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sbuf_rdy              : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SBUF_RDY;

// Define the union U_AV_ADD_BUF
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    av_add_cnt            : 16  ; // [15..0]
        unsigned int    av_add_chn            : 8   ; // [23..16]
        unsigned int    Reserved_0            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AV_ADD_BUF;

// Define the union U_AV_ADD_DONE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    av_addr_done          : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AV_ADD_DONE;

// Define the union U_CLEAR_CHANNEL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    clear_chn             : 8   ; // [7..0]
        unsigned int    clear_start           : 1   ; // [8]
        unsigned int    clear_type            : 2   ; // [10..9]
        unsigned int    Reserved_0            : 21  ; // [31..11]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CLEAR_CHANNEL;

// Define the union U_CLR_WAIT_TIME
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    clr_wait_time         : 16  ; // [15..0]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CLR_WAIT_TIME;

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

// Define the union U_IRAW_D2B_OVFL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iraw_d2b_f_ovfl       : 1   ; // [0] play channel interal buffer overflow interrupt
        unsigned int    iraw_d2b_r_ovfl       : 1   ; // [1] record channel interal buffer overflow interrupt
        unsigned int    Reserved_0            : 30  ; // [31..2]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IRAW_D2B_OVFL;

// Define the union U_RAW_SCD_INT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iraw_scd_int          : 6   ; // [5..0]
        unsigned int    Reserved_0            : 26  ; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_RAW_SCD_INT;

// Define the union U_SCD_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    scd_bufid             : 7   ; // [6..0]
        unsigned int    Reserved_1            : 1   ; // [7]
        unsigned int    scd_chanid            : 3   ; // [10..8]
        unsigned int    Reserved_0            : 21  ; // [31..11]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SCD_STATUS;

// Define the union U_SCD_REC_ADDR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    scd_rec_addr          : 24  ; // [23..0]
        unsigned int    Reserved_0            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SCD_REC_ADDR;

// Define the union U_FIX_CNT_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fix_cnt0_bufid        : 7   ; // [6..0]
        unsigned int    fix_cnt0_ena          : 1   ; // [7]
        unsigned int    fix_cnt1_bufid        : 7   ; // [14..8]
        unsigned int    fix_cnt1_ena          : 1   ; // [15]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_FIX_CNT_CFG;

// Define the union U_FIX_INT_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fix_int_cnt0          : 16  ; // [15..0]
        unsigned int    fix_int_cnt1          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_FIX_INT_CNT;

// Define the union U_EOP_CNT_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    eop_cnt0_bufid        : 7   ; // [6..0]
        unsigned int    eop_cnt0_ena          : 1   ; // [7]
        unsigned int    eop_cnt1_bufid        : 7   ; // [14..8]
        unsigned int    eop_cnt1_ena          : 1   ; // [15]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_EOP_CNT_CFG;

// Define the union U_EOP_INT_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    eop_int_cnt0          : 16  ; // [15..0]
        unsigned int    eop_int_cnt1          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_EOP_INT_CNT;

// Define the union U_OVFL_CNT_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ovfl_cnt0_bufid       : 7   ; // [6..0]
        unsigned int    ovfl_cnt0_ena         : 1   ; // [7]
        unsigned int    ovfl_cnt1_bufid       : 7   ; // [14..8]
        unsigned int    ovfl_cnt1_ena         : 1   ; // [15]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_OVFL_CNT_CFG;

// Define the union U_OVFL_INT_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ovfl_int_cnt0         : 16  ; // [15..0]
        unsigned int    ovfl_int_cnt1         : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_OVFL_INT_CNT;

// Define the union U_TIME_CNT_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    time_cnt0_bufid       : 7   ; // [6..0]
        unsigned int    time_cnt0_ena         : 1   ; // [7]
        unsigned int    time_cnt1_bufid       : 7   ; // [14..8]
        unsigned int    time_cnt1_ena         : 1   ; // [15]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_TIME_CNT_CFG;

// Define the union U_TIME_INT_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    time_int_cnt0         : 16  ; // [15..0]
        unsigned int    time_int_cnt1         : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_TIME_INT_CNT;

// Define the union U_ALOVFL_CNT_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    alovfl_cnt0_bufid     : 7   ; // [6..0]
        unsigned int    alovfl_cnt0_ena       : 1   ; // [7]
        unsigned int    alovfl_cnt1_bufid     : 7   ; // [14..8]
        unsigned int    alovfl_cnt1_ena       : 1   ; // [15]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ALOVFL_CNT_CFG;

// Define the union U_ALOVFL_INT_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    alovfl_int_cnt0       : 16  ; // [15..0]
        unsigned int    alovfl_int_cnt1       : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ALOVFL_INT_CNT;

// Define the union U_IP_CNT_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cnt0_int_cfg          : 3   ; // [2..0]
        unsigned int    Reserved_7            : 1   ; // [3]
        unsigned int    cnt0_ip_cfg           : 2   ; // [5..4]
        unsigned int    Reserved_6            : 1   ; // [6]
        unsigned int    cnt0_ena              : 1   ; // [7]
        unsigned int    cnt1_int_cfg          : 3   ; // [10..8]
        unsigned int    Reserved_5            : 1   ; // [11]
        unsigned int    cnt1_ip_cfg           : 2   ; // [13..12]
        unsigned int    Reserved_4            : 1   ; // [14]
        unsigned int    cnt1_ena              : 1   ; // [15]
        unsigned int    cnt2_int_cfg          : 3   ; // [18..16]
        unsigned int    Reserved_3            : 1   ; // [19]
        unsigned int    cnt2_ip_cfg           : 2   ; // [21..20]
        unsigned int    Reserved_2            : 1   ; // [22]
        unsigned int    cnt2_ena              : 1   ; // [23]
        unsigned int    cnt3_int_cfg          : 3   ; // [26..24]
        unsigned int    Reserved_1            : 1   ; // [27]
        unsigned int    cnt3_ip_cfg           : 2   ; // [29..28]
        unsigned int    Reserved_0            : 1   ; // [30]
        unsigned int    cnt3_ena              : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_CNT_CFG;

// Define the union U_IP_INT_CNT01
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_int_cnt0           : 16  ; // [15..0]
        unsigned int    ip_int_cnt1           : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_INT_CNT01;

// Define the union U_IP_INT_CNT23
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_int_cnt2           : 16  ; // [15..0]
        unsigned int    ip_int_cnt3           : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_INT_CNT23;

// Define the union U_TIME_TABLE_SET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    last_time             : 24  ; // [23..0]
        unsigned int    ipg_select            : 3   ; // [26..24]
        unsigned int    scan_enable           : 1   ; // [27]
        unsigned int    Reserved_0            : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_TIME_TABLE_SET;

// Define the union U_RAW_D2B_OVFL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    raw_d2b_f_ovfl        : 1   ; // [0]
        unsigned int    raw_d2b_r_ovfl        : 1   ; // [1]
        unsigned int    Reserved_0            : 30  ; // [31..2]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_RAW_D2B_OVFL;

// Define the union U_CHN_D2B_OVFL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    d2b_f_ovfl_chn        : 8   ; // [7..0]
        unsigned int    d2b_r_ovfl_chn        : 8   ; // [15..8]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CHN_D2B_OVFL;

// Define the union U_D2B_AFUL_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fp_aful_ena           : 1   ; // [0]
        unsigned int    rp_aful_ena           : 1   ; // [1]
        unsigned int    Reserved_0            : 30  ; // [31..2]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_D2B_AFUL_CFG;

// Define the union U_D2B_RATE0_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    d2b_rate0_cfg         : 24  ; // [23..0]
        unsigned int    Reserved_0            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_D2B_RATE0_CFG;

// Define the union U_D2B_TEST_SET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    d2b_test_set          : 5   ; // [4..0]
        unsigned int    Reserved_0            : 27  ; // [31..5]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_D2B_TEST_SET;

// Define the union U_D2B_INFO_CLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    d2b_info_clr          : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_D2B_INFO_CLR;

// Define the union U_D2B_ERR_INFO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    d2b_err_info          : 5   ; // [4..0]
        unsigned int    Reserved_0            : 27  ; // [31..5]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_D2B_ERR_INFO;

// Define the union U_D2B_AFUL_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fp_aful_sta           : 1   ; // [0]
        unsigned int    rp_aful_sta           : 1   ; // [1]
        unsigned int    Reserved_1            : 2   ; // [3..2]
        unsigned int    fp_buf_cnt            : 4   ; // [7..4]
        unsigned int    rp_buf_cnt            : 4   ; // [11..8]
        unsigned int    Reserved_0            : 20  ; // [31..12]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_D2B_AFUL_STATUS;

// Define the union U_D2B_RATE_STOP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    d2b_rate_stop         : 4   ; // [3..0]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_D2B_RATE_STOP;

// Define the union U_D2B_RATE_CNT01
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    d2b_rate_cnt0         : 16  ; // [15..0]
        unsigned int    d2b_rate_cnt1         : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_D2B_RATE_CNT01;

// Define the union U_D2B_RATE_CNT23
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    d2b_rate_cnt2         : 16  ; // [15..0]
        unsigned int    d2b_rate_cnt3         : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_D2B_RATE_CNT23;

// Define the union U_BUS_ERRCNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    bus_errcnt            : 16  ; // [15..0]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_BUS_ERRCNT;

// Define the union U_BUS_CMD0_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cmd0_wcnt             : 16  ; // [15..0]
        unsigned int    cmd0_rcnt             : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_BUS_CMD0_CNT;

// Define the union U_BUS_CMD1_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cmd1_wcnt             : 16  ; // [15..0]
        unsigned int    cmd1_rcnt             : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_BUS_CMD1_CNT;

// Define the union U_BUS_ACK0_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ack0_wcnt             : 16  ; // [15..0]
        unsigned int    ack0_rcnt             : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_BUS_ACK0_CNT;

// Define the union U_BUS_ACK1_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ack1_wcnt             : 16  ; // [15..0]
        unsigned int    ack1_rcnt             : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_BUS_ACK1_CNT;

// Define the union U_BUS_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    if2mst_busy           : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_BUS_STATUS;

// Define the union U_AHB_TEST_ENA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ahb_test_ena          : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AHB_TEST_ENA;

// Define the union U_AHB_TEST_TYPE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ahb_test_len          : 8   ; // [7..0]
        unsigned int    ahb_test_vlen         : 8   ; // [15..8]
        unsigned int    ahb_test_cmd          : 1   ; // [16]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AHB_TEST_TYPE;

// Define the union U_AHB_TEST_START
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ahb_test_count        : 16  ; // [15..0]
        unsigned int    ahb_test_start        : 1   ; // [16]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AHB_TEST_START;

// Define the union U_AHB_TEST_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ahb_test_done         : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AHB_TEST_STATUS;

// Define the union U_IP_DESC_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_desc_size         : 16  ; // [15..0]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_DESC_SIZE;

// Define the union U_IP_DESC_PTR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_desc_rptr         : 16  ; // [15..0]
        unsigned int    ip_desc_wptr         : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_DESC_PTR;

// Define the union U_IP_VLDDESC_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_vld_cnt           : 16  ; // [15..0]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_VLDDESC_CNT;

// Define the union U_IP_DESC_ADD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_desc_add          : 16  ; // [15..0]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_DESC_ADD;

// Define the union U_IP_DESC_TH_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_alemp_th          : 16  ; // [15..0]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_DESC_TH_CFG;

// Define the union U_IP_INT_CNT_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_int_cfg           : 4   ; // [3..0]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_INT_CNT_CFG;

// Define the union U_IP_SYNC_TH_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_sync_th      : 3;    // [2..0]
        unsigned int    Reserved_2      : 1;    // [3]
        unsigned int    ip_loss_th      : 2;    // [5..4]
        unsigned int    Reserved_1      : 2;    // [7..6]
        unsigned int    ip_sync_type    : 2;    // [9..8]
        unsigned int    Reserved_0      : 22;   // [31..10]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_SYNC_TH_CFG;

// Define the union U_IP_RE_SYNC_EN
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_resync_en         : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_RE_SYNC_EN;

// Define the union U_IP_RE_SYNC_REQ
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_resync_req        : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_RE_SYNC_REQ;

// Define the union U_IP_TS_RATE_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_rate_cfg          : 8   ; // [7..0]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_TS_RATE_CFG;

// Define the union U_IP_TS_OUT_STOP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_stop_en           : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_TS_OUT_STOP;

// Define the union U_IP_CLRCHN_REQ
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_clrchn_req        : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_CLRCHN_REQ;

// Define the union U_IP_IRAW_INT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_iraw_syn_loss     : 1   ; // [0]
        unsigned int    ip_iraw_sync_on      : 1   ; // [1]
        unsigned int    ip_iraw_clr_done     : 1   ; // [2]
        unsigned int    ip_iraw_desc_alemp   : 1   ; // [3]
        unsigned int    ip_iraw_desc_empty   : 1   ; // [4]
        unsigned int    ip_iraw_desc_out     : 1   ; // [5]
        unsigned int    Reserved_0            : 26  ; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_IRAW_INT;

// Define the union U_IP_IENA_INT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_iena_syn_loss    : 1   ; // [0]
        unsigned int    ip_iena_sync_on     : 1   ; // [1]
        unsigned int    ip_iena_clr_done    : 1   ; // [2]
        unsigned int    ip_iena_desc_alemp  : 1   ; // [3]
        unsigned int    ip_iena_desc_empty  : 1   ; // [4]
        unsigned int    ip_iena_desc_out    : 1   ; // [5]
        unsigned int    Reserved_1          : 2   ; // [7..6]
        unsigned int    ip_iena_all         : 1   ; // [8]
        unsigned int    Reserved_0          : 23  ; // [31..9]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_IENA_INT;

// Define the union U_IP_IINT_INT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_iint_syn_loss     : 1   ; // [0]
        unsigned int    ip_iint_sync_on      : 1   ; // [1]
        unsigned int    ip_iint_clr_done     : 1   ; // [2]
        unsigned int    ip_iint_desc_alemp   : 1   ; // [3]
        unsigned int    ip_iint_desc_empty   : 1   ; // [4]
        unsigned int    ip_iint_desc_out     : 1   ; // [5]
        unsigned int    Reserved_1            : 2   ; // [7..6]
        unsigned int    ip_iint_all          : 1   ; // [8]
        unsigned int    Reserved_0            : 23  ; // [31..9]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_IINT_INT;

// Define the union U_IP_DBG_CNT_EN
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip_dbg_cnt_en        : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_DBG_CNT_EN;

/******************************************************************************/
/*                      HiPVR DAV_TOP register definition                     */
/******************************************************************************/
#define DMX_VERSION                    (0xA08 )
#define RAW_FQ_CHN_0(offset)            (0xC020 + ((offset) << 2))
#define INT_FQ_CHN_0(offset)            (0xC028 + ((offset) << 2))
#define ENA_FQ_CHN_0(offset)            (0xC030 + ((offset) << 2))
#define TYPE_FQ_CHN_0(offset)           (0xC038 + ((offset) << 2))

#define RAW_OQ_DESC_0(offset)           (0xC040 + ((offset) << 2))
#define INT_OQ_DESC_0(offset)           (0xC050 + ((offset) << 2))
#define ENA_OQ_DESC_0(offset)           (0xC060 + ((offset) << 2))  /* the description sub-output interrupt enable of link channel 31~0 */
#define RAW_OQ_EOP_0(offset)            (0xC070 + ((offset) << 2))  /* the EOP raw interrupt of link channel 31 ~0 */
#define INT_OQ_EOP_0(offset)            (0xC080 + ((offset) << 2))  /* the EOP interrupt state of link channel 31~0 */
#define ENA_OQ_EOP_0(offset)            (0xC090 + ((offset) << 2))  /* the EOP interrupt enable of link channel 31~0 */
#define RAW_OQ_CHN_0(offset)            (0xC0A0 + ((offset) << 2))
#define INT_OQ_CHN_0(offset)            (0xC0B0 + ((offset) << 2))
#define ENA_OQ_CHN_0(offset)            (0xC0C0 + ((offset) << 2))  /* the overflow or almost-overflow of link channel 31~0 interrrupt enble */
#define TYPE_OQ_CHN_0(offset)           (0xC0D0 + ((offset) << 2))  /* the overflow or almost-overflow of link channel 31~0 interrupt type denote */
#define RAW_CLR_CHN                     (0xC0E0)    /* clear channel operation finish raw interrupt */
#define RAW_D2B_OVFL                    (0xC0E4)    /* bus interal buffer overflow raw intterrupt */
#define ENA_OQ_SMMU_0(offset)           (0xCF30 + ((offset) << 2)) 
#define CLR_OQ_SMMU_0(offset)           (0xCF10 + ((offset) << 2)) 

#define INT_STA_TYPE                    (0xC100)    /* the same type interrupt interrupt state */
#define ENA_INT_TYPE                    (0xC104)    /* the same type interrupt interrupt enable */
#define INT_STA_ALL                     (0xC110)    /* the total interrupt state */
#define ENA_INT_ALL                     (0xC114)    /* the total interrupt enable */
#define OQ_ENA_0(offset)                (0xC130 + ((offset) << 2))  /* link channel output description sub-queue enable control bit, corresponding 31~0 queue */
#define FQ_ENA_0(offset)                (0xC140 + ((offset) << 2))  /* idle description sub-queue enable control bit, correspoding 31~0 queue */
//#define QUQUE_VALERR                    (0xC148)    /* the state of idle-queue and output description sub-queue remains the valid space calc whether error */
//#define OVFL_DROP_ENA                   (0xC14C)    /* whether make sure the data integrality when queue overflow */
//#define AF_CHN0_SET                     (0xC150)    /* AF data output link channel set */
//#define AF_CHN1_SET                     (0xC154)    /* AF data output link channel set */
#define FQ_INIT_DONE                    (0xC160)    /* idle description sub-queue logical automatic initialize finish denote */
#define OQ_INIT_DONE                    (0xC164)    /* output descript sub-queue logical automatic initialize finish denote */
//#define FQ_WR_MASK                      (0xC168)    /* idle description sub queue write operation shield bit set */
#define OQ_WR_MASK                      (0xC16C)    /* output description sub queue write operation shield bit set */
//#define D2B_BP_CFG                      (0xC170)    /* inner bus buffer whether counter-press IP channel enable config */
//#define D2B_BP_STATUS                   (0xC174)    /* inner bus buffer whether come out counter-press state */
//#define D2B_BUF_OVFL                    (0xC178)    /* when inner bus buffer come out overflow state, corresponding link channel number */
#define CLR_WAIT_TIME                   (0xC180)    /* clear channel wait register */
#define CLR_CHN_CMD                     (0xC184)    /* clear channel start register */
//#define BUS_STATUS                      (0xC190)    /* AHB bus real-time state look up */
//#define CNT_INIT_DONE                   (0xC1A0)    /* statistic count logic automatic initialize finish flag */
//#define CNT_ENA_CFG                     (0xC1A4)    /* interrupt statistic module enable config */
//#define CNT_TYPE_ENA_L                  (0xC1A8)    /* TS packet counter 0 of joining on someone record buffer low 32bit */
//#define CNT_TYPE_ENA_H                  (0xC1AC)    /* TS packet counter 0 of joining on someone record buffer low 32bit */

#define TS_CNT0_5_L(RecId)                (0xC1B0 + ((RecId) << 3))   /* TS packet counter 0-5 of joining on someone record buffer low 32bit */
#define TS_CNT0_5_H(RecId)                (0xC1B4 + ((RecId) << 3))   /* TS packet counter 0-5 of joining on someone record buffer high 8 bits */
#define TS_CNT6_31_L(RecId)                (0xC500 + ((RecId) << 3))   /* TS packet counter 6-31 of joining on someone record buffer low 32bit */
#define TS_CNT6_31_H(RecId)                (0xC504 + ((RecId) << 3))   /* TS packet counter 6-31 of joining on someone record buffer high 8 bits */

#define REC_TSCNT_CFG_0                 (0xC1E0)    /* config TS count 0~3 join in someone record buffer */
#define REC_TSCNT_CFG_1                 (0xC1E4)    /* config TS count 4~7 join in someone record buffer */
#define REC_TSCNT_CFG_2                 (0xC1E8)    /* config TS count 8~11 join in someone record buffer */
#define REC_TSCNT_CFG_3                 (0xC1EC)    /* config TS count 12~15 join in someone record buffer */
#define REC_TSCNT_CFG_4                 (0xC5D0)    /* config TS count 16~19 join in someone record buffer */
#define REC_TSCNT_CFG_5                 (0xC5D4)    /* config TS count 20~23 join in someone record buffer */
#define REC_TSCNT_CFG_6                 (0xC5D8)    /* config TS count 24~27 join in someone record buffer */
#define REC_TSCNT_CFG_7                 (0xC5DC)    /* config TS count 28~31 join in someone record buffer */

#define SCD_TSCNT_ENA                   (0xC1F0)    /* in record index, whether TS packet count need use DAV TS packet count to replace */
#define TIMESTAMP_CTRL                  (0xC1F8)    /* the type of time stamp added in ts packet */

#define DAV_DROP_TS_FLAG              (0xC61C)    /* shared rec pid ts dropped flag. */
#define DAV_DROP_TS_FLAG_CLR       (0xC618)    /* after read DAV_DROP_TS_FLAG , we need clear it. */

#define IP_BP_CLR_CFG                   (0xC200)    /* when IP channel timeout whether automatic clear config */

typedef union
{
    struct
    {
        HI_U32  ipaful_clr_time : 24;   // [23:0]
        HI_U32  ipaful_clr_ena  : 7;    // [30:24]
        HI_U32  reserved        : 1;    // [31]
    } bits;

    HI_U32 value;
} U_IP_BP_CLR_CFG;

typedef union
{
    struct
    {
        HI_U32  rec_timestamp_mode : 14;   // [13:0]
        HI_U32  reserved           : 18;    // [31:14]
    } bits;

    HI_U32 value;
} U_TIMESTAMP_CTRL;


#define IP_CHN_BP_STA                   (0xC204)    /* IP channel counter-press state */
//#define IP_BP_AUTOCLR_ENA               (0xC208)    /* IP channel counter-press state automatic clear enable register */

#define IP_BP_CFG(PortId, offset)       (0xC210 + ((PortId) << 5) + ((offset) << 2))    /* IP channel 0 whether based on output description sub queue for counter-press enable config */
#define IP_BP_FQ_CFG(PortId, offset)    (0xC220 + ((PortId) << 5) + ((offset) << 2))    /* IP channel 0 whether based on free description sub queue for counter-press enable config */

//#define AHB_TEST_ENA                    (0xC2B0)    /* AHB Master test enbale */
//#define AHB_TEST_TYPE                   (0xC2B4)    /* AHB Master send test cmd type when test*/
//#define AHB_TEST_SADDR                  (0xC2B8)    /* AHB Master test start address */
//#define AHB_TEST_START                  (0xC2BC)    /* AHB Master test start */
//#define AHB_TEST_STATUS                 (0xC2C0)    /* AHB Master test state */
//#define AHB_TEST_INFO0                  (0xC2C4)    /* AHB Master after test enable, cmd execute return state info register 0 */
//#define AHB_TEST_INFO1                  (0xC2C8)    /* AHB Master after test enable,  cmd execute return state info register 1 */
//#define AHB_TEST_RESULT                 (0xC2CC)    /* AHB Master whether test resut write compare is right or not */

#define IP_BP_FQ_STA(PortId, offset,addr)                         \
    do                                                            \
    {                                                             \
        if ((PortId) >= 4)                                        \
        {                                                         \
            if((PortId) == 4)                                     \
            {                                                     \
               (addr) =  (0xC370) + ((offset) << 2);              \
            }                                                     \
            else if((PortId) == 5)                                \
            {                                                     \
               (addr) =  (0xC388) + ((offset) << 2) ;             \
            }                                                     \
            else                                                  \
            {                                                     \
               (addr) = 0;                                        \
               HI_ERR_DEMUX("error PortId:%x\n",PortId);          \
            }                                                     \
        }                                                         \
        else                                                      \
        {                                                         \
            (addr) = (0xC340) + ((PortId) << 3) + ((offset) << 2);\
        }                                                         \
    }                                                             \
  while (0)

//#define D2B_DROP_FLAG0                  (0xC380)    /* DMX2BUF overflow discard flag 31~0 */
//#define D2B_DROP_FLAG1                  (0xC384)    /* DMX2BUF overflow discard flag 63~32 */
//#define D2B_DROP_FLAG2                  (0xC388)    /* DMX2BUF overflow discard flag 95~64 */
//#define D2B_DROP_FLAG3                  (0xC38C)    /* DMX2BUF overflow discard flag 127~96 */
//#define B2M_DROP_FLAG0                  (0xC390)    /* BUF2MAIN overflow discard flag 31~0 */
//#define B2M_DROP_FLAG1                  (0xC394)    /* BUF2MAIN overflow discard flag 63~32 */
//#define B2M_DROP_FLAG2                  (0xC398)    /* BUF2MAIN overflow discard flag 95~64 */
//#define B2M_DROP_FLAG3                  (0xC39C)    /* BUF2MAIN overflow discard flag 127~96 */
//#define REC_DROP_FLAG0                  (0xC3A0)    /* record channel overflow discard flag 31~0 */
//#define REC_DROP_FLAG1                  (0xC3A4)    /* record channel overflow discard flag 63~32 */
//#define REC_DROP_FLAG2                  (0xC3A8)    /* record channel overflow discard flag 95~64 */
//#define REC_DROP_FLAG3                  (0xC3AC)    /* record channel overflow discard flag 127~196 */
//#define B2M_DBG_STATUS0                 (0xC3B0)    /* BUF2MAIN inner handle state register 0 */
//#define B2M_DBG_STATUS1                 (0xC3B4)    /* BUF2MAIN inner handle state register 1*/

#define IP_DESC_SADDR(PortId)           (0xC800 + ((PortId) << 8))  /* IP channel 0 descrition queue start address */
#define IP_DESC_SIZE(PortId)            (0xC804 + ((PortId) << 8))  /* IP channel 0 description word queue size */
#define IP_DESC_PTR(PortId)             (0xC808 + ((PortId) << 8))  /* IP channel 0 description word queue read-write address */
#define IP_VLDDESC_CNT(PortId)          (0xC80C + ((PortId) << 8))  /* valid description word number in IP channel 0 description word queue*/
#define IP_DESC_ADD(PortId)             (0xC810 + ((PortId) << 8))  /* toward IP channel 0 description word queue add description word item */
//#define IP_DESC_TH_CFG(PortId)          (0xC814 + ((PortId) << 8))  /* IP channel 0 destription word queque almost empty water-line */
#define IP_INT_CNT_CFG(PortId)          (0xC818 + ((PortId) << 8))  /* IP channel 0 description word queue multi-word interrupt water-line set */
#define IP_SYNC_TH_CFG(PortId)          (0xC820 + ((PortId) << 8))  /* IP channel 0 sync check configuration */
//#define IP_RE_SYNC_EN(PortId)           (0xC824 + ((PortId) << 8))  /* IP channel 0 again-sync check enable configuration */
//#define IP_RE_SYNC_REQ(PortId)          (0xC828 + ((PortId) << 8))  /* IP channel 0 again-sync check request */
#define IP_TS_RATE_CFG(PortId)          (0xC830 + ((PortId) << 8))  /* IP channel 0 output TS rate configuration */
#define IP_TS_OUT_STOP(PortId)          (0xC834 + ((PortId) << 8))  /* IP channnel 0 whether output TS  pause the configuration */
#define IP_CLRCHN_REQ(PortId)           (0xC840 + ((PortId) << 8))  /* IP channel 0 clear channel request */
#define IP_IRAW_INT(PortId)             (0xC880 + ((PortId) << 8))  /* IP channel 0 raw interrupt */
#define IP_IENA_INT(PortId)             (0xC884 + ((PortId) << 8))  /* IP channel 0 interrupt enable */
#define IP_IINT_INT(PortId)             (0xC888 + ((PortId) << 8))  /* IP channel 0 interrupt state */
#define IP_DBG_CNT_EN(PortId)           (0xC890 + ((PortId) << 8))  /* IP channel 0 debug counter enable control */
#define IP_DBG_OUT0(PortId)             (0xC894 + ((PortId) << 8))  /* IP channel 0 debug register  0 */
#define IP_DBG_OUT1(PortId)             (0xC898 + ((PortId) << 8))  /* IP channel 0 debug register  1 */
#define IP_DBG_OUT2(PortId)             (0xC89C + ((PortId) << 8))  /* IP channel 0 debug register 2 */
//#define IP_DBG_SYNC0(PortId)            (0xC8A0 + ((PortId) << 8))  /* IP channel 0 sync check debug register 0 */
//#define IP_DBG_SYNC1(PortId)            (0xC8A4 + ((PortId) << 8))  /* IP channel 0 sync check debug register 1 */
//#define IP_DBG_SYNC2(PortId)            (0xC8A8 + ((PortId) << 8))  /* IP channel 0 sync check debug register 2 */
//#define IP_DBG_SYNC3(PortId)            (0xC8AC + ((PortId) << 8))  /* IP channel 0 sync check debug register 3 */
//#define IP_DBG_LOSS(PortId)             (0xC8B0 + ((PortId) << 8))  /* IP channel 0 loss-sync check debug register */
//#define IP_SEND_DBG(PortId)             (0xC8B4 + ((PortId) << 8))  /* IP channel 0 send debug register */
#define IP_SYNC_LEN(PortId)             (0xC8C0 + ((PortId) << 8))  /* IP port sync len config reg,just for chip mv300 */

#define ENA_IP_SMMU_0                      (0xCF20) 
#define CLR_IP_SMMU_0               	 (0xCF00) 

#define ADDR_FQ_WORD0(FQId)             (0xD000 + ((FQId) << 4))    /* idle description sub queue the first WORD corresponding address */
#define ADDR_FQ_WORD1(FQId)             (0xD004 + ((FQId) << 4))    /* idle description sub queue the second WORD corresponding address */
#define ADDR_FQ_WORD2(FQId)             (0xD008 + ((FQId) << 4))    /* idle description sub queue the third WORD corresponding address */
#define ADDR_FQ_WORD3(FQId)             (0xD00C + ((FQId) << 4))    /* idle description sub queue the fourth WORD corresponding address */

#define ADDR_INT_CNT(OQId)              (0xD800 + ((OQId) << 2))    /* interrupt statistic register */
#define CHANNEL_TS_COUNT(ChanId)        (0xDA00 + ((ChanId) << 2))

#define ADDR_OQ_WORD0(OQId)             (0xE000 + ((OQId) << 4))    /* output description sub link-queue the first WORD corresponding address */
#define ADDR_OQ_WORD1(OQId)             (0xE004 + ((OQId) << 4))    /* output description sub link-queue the second WORD corresponding address */
#define ADDR_OQ_WORD2(OQId)             (0xE008 + ((OQId) << 4))    /* output description sub link-queue the third WORD corresponding address */
#define ADDR_OQ_WORD3(OQId)             (0xE00C + ((OQId) << 4))    /* output description sub link-queue the fourth WORD corresponding address */
#define ADDR_OQ_WORD4(OQId)             (0xE800 + ((OQId) << 4))    /* output description sub link-queue the fifth WORD corresponding address */
#define ADDR_OQ_WORD5(OQId)             (0xE804 + ((OQId) << 4))    /* output description sub link-queue the sixth WORD corresponding address */
#define ADDR_OQ_WORD6(OQId)             (0xE808 + ((OQId) << 4))    /* output description sub link-queue the seventh WORD corresponding address */
#define ADDR_OQ_WORD7(OQId)             (0xE80C + ((OQId) << 4))    /* output description sub link-queue the eighth WORD corresponding address */

#define ADDR_DBG_DETECT0                    (0x2320)

// Define the union U_RAW_CLR_CHN
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

} U_RAW_CLR_CHN;

// Define the union U_RAW_D2B_OVFL


// Define the union U_RAW_BUS_ERR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iraw_bus_err          : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_RAW_BUS_ERR;

// Define the union U_RAW_CFG_ERR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iraw_fq_cfg_err       : 1   ; // [0]
        unsigned int    iraw_fq_desc_err      : 1   ; // [1]
        unsigned int    iraw_fq_ovfl_err      : 1   ; // [2]
        unsigned int    iraw_oq_ovfl_err      : 1   ; // [3]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_RAW_CFG_ERR;

// Define the union U_INT_STA_TYPE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iint_mmu              : 1   ; // [0]
        unsigned int    iint_desc_i           : 1   ; // [1]
        unsigned int    iint_chn_i            : 2   ; // [3..2]
        unsigned int    iint_desc_o           : 4   ; // [7..4]
        unsigned int    iint_eop_o            : 4   ; // [11..8]
        unsigned int    iint_chn_o            : 4   ; // [15..12]
        unsigned int    iint_clr_chn          : 1   ; // [16]
        unsigned int    iint_d2b_f_ovfl       : 1   ; // [17]
        unsigned int    iint_d2b_r_ovfl       : 1   ; // [18]
        unsigned int    iint_bus_err          : 1   ; // [19]
        unsigned int    iint_ip0_all          : 1   ; // [20]
        unsigned int    iint_ip1_all          : 1   ; // [21]
        unsigned int    iint_ip2_all          : 1   ; // [22]
        unsigned int    iint_ip3_all          : 1   ; // [23]
        unsigned int    iint_fq_cfg_err       : 1   ; // [24]
        unsigned int    iint_fq_desc_err      : 1   ; // [25]
        unsigned int    iint_fq_ovfl_err      : 1   ; // [26]
        unsigned int    iint_oq_ovfl_err      : 1   ; // [27]
        unsigned int    Reserved_0            : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_INT_STA_TYPE;

// Define the union U_ENA_INT_TYPE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iena_mmu              : 1   ; // [0]
        unsigned int    iena_desc_i           : 1   ; // [1]
        unsigned int    iena_chn_i            : 2   ; // [3..2]
        unsigned int    iena_desc_o           : 4   ; // [7..4]
        unsigned int    iena_eop_o            : 4   ; // [11..8]
        unsigned int    iena_chn_o            : 4   ; // [15..12]
        unsigned int    iena_clr_chn          : 1   ; // [16]
        unsigned int    iena_d2b_f_ovfl       : 1   ; // [17]
        unsigned int    iena_d2b_r_ovfl       : 1   ; // [18]
        unsigned int    iena_bus_err          : 1   ; // [19]
        unsigned int    iena_ip0_all          : 1   ; // [20]
        unsigned int    iena_ip1_all          : 1   ; // [21]
        unsigned int    iena_ip2_all          : 1   ; // [22]
        unsigned int    iena_ip3_all          : 1   ; // [23]
        unsigned int    iena_fq_cfg_err       : 1   ; // [24]
        unsigned int    iena_fq_desc_err      : 1   ; // [25]
        unsigned int    iena_fq_ovfl_err      : 1   ; // [26]
        unsigned int    iena_oq_ovfl_err      : 1   ; // [27]
        unsigned int    iena_ip4_all          : 1   ; // [28]
        unsigned int    iena_ip5_all          : 1   ; // [29]
        unsigned int    Reserved_0            : 2   ; // [31..30]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ENA_INT_TYPE;  /*0xc104*/

// Define the union U_INT_STA_ALL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iint_all              : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_INT_STA_ALL;

// Define the union U_ENA_INT_ALL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iena_all              : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_ENA_INT_ALL;

// Define the union U_FQ_ENA_1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fq_ena_1              : 8   ; // [7..0]
        unsigned int    Reserved_0            : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_FQ_ENA_1;

// Define the union U_AF_CHN0_SET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dmx1_af_bufid         : 8   ; // [7..0]
        unsigned int    dmx2_af_bufid         : 8   ; // [15..8]
        unsigned int    dmx3_af_bufid         : 8   ; // [23..16]
        unsigned int    dmx4_af_bufid         : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AF_CHN0_SET;

// Define the union U_AF_CHN1_SET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dmx5_af_bufid         : 8   ; // [7..0]
        unsigned int    dmx6_af_bufid         : 8   ; // [15..8]
        unsigned int    dmx7_af_bufid         : 8   ; // [23..16]
        unsigned int    Reserved_0            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AF_CHN1_SET;

// Define the union U_FQ_INIT_DONE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fq_init_done          : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_FQ_INIT_DONE;

// Define the union U_OQ_INIT_DONE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    oq_init_done          : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_OQ_INIT_DONE;

// Define the union U_FQ_WR_MASK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fq_mask               : 4   ; // [3..0]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_FQ_WR_MASK;

// Define the union U_OQ_WR_MASK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    oq_mask               : 4   ; // [3..0]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_OQ_WR_MASK;

// Define the union U_D2B_BP_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fp_aful_ena           : 1   ; // [0]
        unsigned int    rp_aful_ena           : 1   ; // [1]
        unsigned int    Reserved_0            : 30  ; // [31..2]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_D2B_BP_CFG;

// Define the union U_D2B_BP_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    fp_aful_sta           : 1   ; // [0]
        unsigned int    rp_aful_sta           : 1   ; // [1]
        unsigned int    Reserved_1            : 2   ; // [3..2]
        unsigned int    fp_buf_cnt            : 4   ; // [7..4]
        unsigned int    rp_buf_cnt            : 4   ; // [11..8]
        unsigned int    sp_buf_cnt            : 4   ; // [15..12]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_D2B_BP_STATUS;

// Define the union U_D2B_BUF_OVFL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    d2b_f_ovfl_chn        : 8   ; // [7..0]
        unsigned int    d2b_r_ovfl_chn        : 8   ; // [15..8]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_D2B_BUF_OVFL;


// Define the union U_CLR_CHN_CMD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    clear_chn             : 8   ; // [7..0]
        unsigned int    clear_start           : 1   ; // [8]
        unsigned int    clear_type            : 2   ; // [10..9]
        unsigned int    Reserved_0            : 21  ; // [31..11]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CLR_CHN_CMD;


// Define the union U_CNT_INIT_DONE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cnt_init_done         : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CNT_INIT_DONE;

// Define the union U_CNT_ENA_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cnt_clk_ena           : 1   ; // [0]
        unsigned int    cnt_func_ena          : 1   ; // [1]
        unsigned int    tscnt_cnt_type        : 1   ; // [2]
        unsigned int    cnt_rsv               : 1   ; // [3]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CNT_ENA_CFG;

// Define the union U_REC_TSCNT_CFG_0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tscnt0_oqid           : 7   ; // [6..0]
        unsigned int    tscnt0_ena            : 1   ; // [7]
        unsigned int    tscnt1_oqid           : 7   ; // [14..8]
        unsigned int    tscnt1_ena            : 1   ; // [15]
        unsigned int    tscnt2_oqid           : 7   ; // [22..16]
        unsigned int    tscnt2_ena            : 1   ; // [23]
        unsigned int    tscnt3_oqid           : 7   ; // [30..24]
        unsigned int    tscnt3_ena            : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_REC_TSCNT_CFG_0;

// Define the union U_REC_TSCNT_CFG_1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tscnt4_oqid           : 7   ; // [6..0]
        unsigned int    tscnt4_ena            : 1   ; // [7]
        unsigned int    tscnt5_oqid           : 7   ; // [14..8]
        unsigned int    tscnt5_ena            : 1   ; // [15]
        unsigned int    tscnt6_oqid           : 7   ; // [22..16]
        unsigned int    tscnt6_ena            : 1   ; // [23]
        unsigned int    tscnt7_oqid           : 7   ; // [30..24]
        unsigned int    tscnt7_ena            : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_REC_TSCNT_CFG_1;

// Define the union U_REC_TSCNT_CFG_2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tscnt8_oqid           : 7   ; // [6..0]
        unsigned int    tscnt8_ena            : 1   ; // [7]
        unsigned int    tscnt9_oqid           : 7   ; // [14..8]
        unsigned int    tscnt9_ena            : 1   ; // [15]
        unsigned int    tscnt10_oqid           : 7   ; // [22..16]
        unsigned int    tscnt10_ena            : 1   ; // [23]
        unsigned int    tscnt11_oqid           : 7   ; // [30..24]
        unsigned int    tscnt11_ena            : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_REC_TSCNT_CFG_2;

// Define the union U_REC_TSCNT_CFG_3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tscnt12_oqid           : 7   ; // [6..0]
        unsigned int    tscnt12_ena            : 1   ; // [7]
        unsigned int    tscnt13_oqid           : 7   ; // [14..8]
        unsigned int    tscnt13_ena            : 1   ; // [15]
        unsigned int    tscnt14_oqid           : 7   ; // [22..16]
        unsigned int    tscnt14_ena            : 1   ; // [23]
        unsigned int    tscnt15_oqid           : 7   ; // [30..24]
        unsigned int    tscnt15_ena            : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_REC_TSCNT_CFG_3;

// Define the union U_REC_TSCNT_CFG_4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tscnt16_oqid           : 7   ; // [6..0]
        unsigned int    tscnt16_ena            : 1   ; // [7]
        unsigned int    tscnt17_oqid           : 7   ; // [14..8]
        unsigned int    tscnt17_ena            : 1   ; // [15]
        unsigned int    tscnt18_oqid           : 7   ; // [22..16]
        unsigned int    tscnt18_ena            : 1   ; // [23]
        unsigned int    tscnt19_oqid           : 7   ; // [30..24]
        unsigned int    tscnt19_ena            : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_REC_TSCNT_CFG_4;

// Define the union U_REC_TSCNT_CFG_5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tscnt20_oqid           : 7   ; // [6..0]
        unsigned int    tscnt20_ena            : 1   ; // [7]
        unsigned int    tscnt21_oqid           : 7   ; // [14..8]
        unsigned int    tscnt21_ena            : 1   ; // [15]
        unsigned int    tscnt22_oqid           : 7   ; // [22..16]
        unsigned int    tscnt22_ena            : 1   ; // [23]
        unsigned int    tscnt23_oqid           : 7   ; // [30..24]
        unsigned int    tscnt23_ena            : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_REC_TSCNT_CFG_5;

// Define the union U_REC_TSCNT_CFG_6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tscnt24_oqid           : 7   ; // [6..0]
        unsigned int    tscnt24_ena            : 1   ; // [7]
        unsigned int    tscnt25_oqid           : 7   ; // [14..8]
        unsigned int    tscnt25_ena            : 1   ; // [15]
        unsigned int    tscnt26_oqid           : 7   ; // [22..16]
        unsigned int    tscnt26_ena            : 1   ; // [23]
        unsigned int    tscnt27_oqid           : 7   ; // [30..24]
        unsigned int    tscnt27_ena            : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_REC_TSCNT_CFG_6;

// Define the union U_REC_TSCNT_CFG_7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tscnt28_oqid           : 7   ; // [6..0]
        unsigned int    tscnt28_ena            : 1   ; // [7]
        unsigned int    tscnt29_oqid           : 7   ; // [14..8]
        unsigned int    tscnt29_ena            : 1   ; // [15]
        unsigned int    tscnt30_oqid           : 7   ; // [22..16]
        unsigned int    tscnt30_ena            : 1   ; // [23]
        unsigned int    tscnt31_oqid           : 7   ; // [30..24]
        unsigned int    tscnt31_ena            : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_REC_TSCNT_CFG_7;

// Define the union U_SCD_TSCNT_ENA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tscnt0_rep_ena        : 1   ; // [0]
        unsigned int    tscnt1_rep_ena        : 1   ; // [1]
        unsigned int    tscnt2_rep_ena        : 1   ; // [2]
        unsigned int    tscnt3_rep_ena        : 1   ; // [3]
        unsigned int    tscnt4_rep_ena        : 1   ; // [4]
        unsigned int    tscnt5_rep_ena        : 1   ; // [5]
        unsigned int    tscnt6_rep_ena        : 1   ; // [6]
        unsigned int    tscnt7_rep_ena        : 1   ; // [7]
        unsigned int    tscnt8_rep_ena        : 1   ; // [8]
        unsigned int    tscnt9_rep_ena        : 1   ; // [9]
        unsigned int    tscnt10_rep_ena        : 1   ; // [10]
        unsigned int    tscnt11_rep_ena        : 1   ; // [11]
        unsigned int    tscnt12_rep_ena        : 1   ; // [12]
        unsigned int    tscnt13_rep_ena        : 1   ; // [13]
        unsigned int    tscnt14_rep_ena        : 1   ; // [14]
        unsigned int    tscnt15_rep_ena        : 1   ; // [15]
        unsigned int    tscnt16_rep_ena        : 1   ; // [16]
        unsigned int    tscnt17_rep_ena        : 1   ; // [17]
        unsigned int    tscnt18_rep_ena        : 1   ; // [18]
        unsigned int    tscnt19_rep_ena        : 1   ; // [19]
        unsigned int    tscnt20_rep_ena        : 1   ; // [20]
        unsigned int    tscnt21_rep_ena        : 1   ; // [21]
        unsigned int    tscnt22_rep_ena        : 1   ; // [22]
        unsigned int    tscnt23_rep_ena        : 1   ; // [23]
        unsigned int    tscnt24_rep_ena        : 1   ; // [24]
        unsigned int    tscnt25_rep_ena        : 1   ; // [25]
        unsigned int    tscnt26_rep_ena        : 1   ; // [26]
        unsigned int    tscnt27_rep_ena        : 1   ; // [27]
        unsigned int    tscnt28_rep_ena        : 1   ; // [28]
        unsigned int    tscnt29_rep_ena        : 1   ; // [29]
        unsigned int    tscnt30_rep_ena        : 1   ; // [30]
        unsigned int    tscnt31_rep_ena        : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_SCD_TSCNT_ENA;

// Define the union U_IP_CHN_BP_STA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ip0_chn_bp_sta        : 1   ; // [0]
        unsigned int    ip1_chn_bp_sta        : 1   ; // [1]
        unsigned int    ip2_chn_bp_sta        : 1   ; // [2]
        unsigned int    ip3_chn_bp_sta        : 1   ; // [3]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_IP_CHN_BP_STA;

// Define the union U_AHB_TEST_RESULT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ahb_test_result       : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AHB_TEST_RESULT;

// Define the union U_DBG_DETECT0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dbg_out_sample_freq       : 16   ; // [15 .. 0]
        unsigned int    Reserved_0            : 16  ; // [31 .. 16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DBG_DETECT0;

#ifdef DMX_MMU_VERSION_1
/*
 * smmu registers sets.
 */
#define SMMU_GLB_CTL_OFFSET    (0x0000)

#define SMMU_INT_STA_OFFSET    (0x0028)
typedef union
{
    struct
    {
        unsigned int tlbmiss : 1 ; //[0]
        unsigned int ptwtrans : 1  ; // [1]
        unsigned int tlbinvalid : 1 ; //[2]
        unsigned int reserved : 29 ; //[31 .. 3] 
    }bits;
    
    unsigned int u32;
}SMMU_INT_STAT;

#define SMMU_INT_CLR_OFFSET    (0x002c)

#define SMMU_WRITE_ERR_ADDR   (0x0340)
#define SMMU_READ_ERR_ADDR     (0x0360)

#define SMMU_SCB_TTBR_OFFSET    (0x0208)
#define SMMU_CB_TTBR_OFFSET     (0x020C)
#define SMMU_ERR_RADDR_OFFSET   (0x0304)
#define SMMU_ERR_WRADDR_OFFSET  (0x0308)

typedef union
{
    struct 
    {
        unsigned int  glb_bypass : 1 ; // [0]
        unsigned int  reserved_1 : 2; //[2 .. 1]
        unsigned int  int_en         : 1; // [3]
        unsigned int  reserved_2 : 12; // [15 .. 4]
        unsigned int  ptw_pf         : 4; // [19 .. 16]
        unsigned int  reserved_3  : 12;// [31 .. 20]
    }bits;

    unsigned int u32;
}SMMU_GLB_CTL;

#elif defined(DMX_MMU_VERSION_2)

#define MMU_TAB_NONSEC_OFFSET		(0xCF40)
#define MMU_TAB_SEC_OFFSET		    (0xCF44)
#define MMU_BYPASS_OFFSET			(0xCF4C)
#define MMU_RAW_INT_OFFSET			(0xC700)
#define MMU_ENA_INT_OFFSET			(0xC704)
#define MMU_STA_INT_OFFSET			(0xC708)
#define MMU_ERR_PA_OFFSET			(0xCF50)
#define MMU_ERR_VA_OFFSET			(0xCF54)
#define MMU_ERR_INFO_OFFSET			(0xCF58)

typedef union
{
    struct
    {
	    unsigned int    raw_mmu_tab_err      : 1   ;
		unsigned int    raw_bus_rep_err      : 1   ;
		unsigned int	Reserved_0			 : 30	; // [31..2]
    }bits;
    
	unsigned int	u32;
}MMU_RAW_INT;

typedef union
{
    struct
    {
	    unsigned int    ena_mmu_tab_err      : 1   ;
		unsigned int    ena_bus_rep_err      : 1   ;
		unsigned int	Reserved_0			 : 30	; // [31..2]
    }bits;
    
	unsigned int	u32;
}MMU_ENA_INT;

typedef union
{
    struct
    {
	    unsigned int    sta_mmu_tab_err      : 1   ;
		unsigned int    sta_bus_rep_err      : 1   ;
		unsigned int	Reserved_0			 : 30	; // [31..2]
    }bits;
    
	unsigned int	u32;
}MMU_STA_INT;

typedef union
{
    struct
    {
	    unsigned int    Reserved_0      	 : 12   ;//[11:0]
		unsigned int	dbg_mmu_err_mmu_pa		 : 20	; // [31..12]
    }bits;
    
	unsigned int	u32;
}MMU_ERR_PA;

typedef union
{
    struct
    {
	    unsigned int    dbg_mmu_err_mmu_chan : 8   ; //[7:0]
		unsigned int	dbg_mmu_err_mmu_vld	 : 1   ; //[8]
		unsigned int    dbg_mmu_err_mmu_sec  : 1   ; //[9]
        unsigned int    reserved_0           : 3   ; //[12:10]
        unsigned int    dbg_mmu_err_mmu_wr   : 1   ; //[13]
		unsigned int    reserved_1			 : 18  ; //[31:14]
    }bits;
    
	unsigned int	u32;
}MMU_ERR_INFO;
#endif

#ifdef RMX_SUPPORT

#define RMX_PORT_CTL_OFFSET(RmxId) (0x2800 + (RmxId << 2))
#define RMX_PORT_ALL_PASS_OFFSET (0x2840)
#define RMX_PORT_DETECT_OFFSET(RmxPortId) (0x2a00 + (RmxPortId << 2))
#define RMX_PID_TAB_OFFSET(PumpId) (0x2c00 + (PumpId << 2))

typedef union
{
    struct
    {
        unsigned int sw_rmx_src0_sel : 8; // [7 .. 0]
        unsigned int sw_rmx_src1_sel : 8; // [15 .. 8]
        unsigned int sw_rmx_src2_sel : 8; // [23 .. 16]
        unsigned int sw_rmx_src3_sel : 8; // [31 .. 24]
    }bits;

    unsigned int u32;
}RMX_PORT_CTL;

typedef union
{
    struct
    {
        unsigned int dbg_full_err_cnt : 16 ;//[15 .. 0]
        unsigned int reserved : 15; // [30 .. 16]
        unsigned int sw_rmx_full_cnten : 1; //[31]
    }bits;
    
    unsigned int u32;
}RMX_PORT_DETECT;

typedef union
{
    struct
    {
        unsigned int original_pid : 13; // [12 .. 0]
        unsigned int remap_source_id : 3;// [15 .. 13]
        unsigned int pid_remap_pid : 13;//[28 .. 16]
        unsigned int pidtab_en : 1; // [29]
        unsigned int pid_remap_mod :1 ;//[30]
        unsigned int pid_del_mod : 1; //[31]
    }bits;

    unsigned int u32;
}RMX_PID_CTL;

#endif

#endif  // __DRV_DEMUX_REG_H__

