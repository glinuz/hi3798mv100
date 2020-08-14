/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_ca_otp_reg_v200.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   : CA OTPV200 REG DEFINE
  Function List :
  History       :
******************************************************************************/
#ifndef __DRV_CA_OTP_REG_V200_H__
#define __DRV_CA_OTP_REG_V200_H__

/* OTP Address */
#ifdef CHIP_TYPE_hi3716mv300
#define CA_OTP_V200_BASE_OFFSET        (0x10180000)
#else
#define CA_OTP_V200_BASE_OFFSET        (0xF8AB0000)
#endif

#define CA_OTP_V200_CHANNEL_SEL        (CA_OTP_V200_BASE_OFFSET + 0x00)
#define CA_OTP_V200_CPU_RW_CTRL        (CA_OTP_V200_BASE_OFFSET + 0x04)
#define CA_OTP_V200_WR_START           (CA_OTP_V200_BASE_OFFSET + 0x08)
#define CA_OTP_V200_CTRL_STATUS        (CA_OTP_V200_BASE_OFFSET + 0x0c)
#define CA_OTP_V200_RDATA              (CA_OTP_V200_BASE_OFFSET + 0x10)
#define CA_OTP_V200_WDATA              (CA_OTP_V200_BASE_OFFSET + 0x14)
#define CA_OTP_V200_RADDR              (CA_OTP_V200_BASE_OFFSET + 0x18)
#define CA_OTP_V200_WADDR              (CA_OTP_V200_BASE_OFFSET + 0x1C)
#define CA_OTP_V200_MODE               (CA_OTP_V200_BASE_OFFSET + 0x20)
#define CA_OTP_V200_LOCK_ADDR_0        (CA_OTP_V200_BASE_OFFSET + 0x24)
#define CA_OTP_V200_LOCK_ADDR_1        (CA_OTP_V200_BASE_OFFSET + 0x28)
#define CA_OTP_V200_LOCK_ADDR_2        (CA_OTP_V200_BASE_OFFSET + 0x2C)
#define CA_OTP_V200_LOCK_ADDR_3        (CA_OTP_V200_BASE_OFFSET + 0x30)
#define CA_OTP_V200_ADDR_LOCK_EN       (CA_OTP_V200_BASE_OFFSET + 0x34)
#define CA_OTP_V200_SOAK_TIME          (CA_OTP_V200_BASE_OFFSET + 0x48)
#define CA_OTP_V200_DEBUG_ADDR_BEGIN   (CA_OTP_V200_BASE_OFFSET + 0x4C)
#define CA_OTP_V200_DEBUG_ADDR_END     (CA_OTP_V200_BASE_OFFSET + 0x64)
#define CA_OTP_V200_DEBUG0             (CA_OTP_V200_BASE_OFFSET + 0x68)
#define CA_OTP_V200_DEBUG1             (CA_OTP_V200_BASE_OFFSET + 0x6C)
#define CA_OTP_V200_DEBUG3             (CA_OTP_V200_BASE_OFFSET + 0x74)
#define CA_OTP_V200_DEBUG4             (CA_OTP_V200_BASE_OFFSET + 0x78)
#define CA_OTP_V200_PV_0               (CA_OTP_V200_BASE_OFFSET + 0x80)
#define CA_OTP_V200_PV_1               (CA_OTP_V200_BASE_OFFSET + 0x84)
#define CA_OTP_V200_PV_LOCK_0          (CA_OTP_V200_BASE_OFFSET + 0x88)
#define CA_OTP_V200_PV_LOCK_1          (CA_OTP_V200_BASE_OFFSET + 0x8C)
#define CA_OTP_V200_DATA_LOCK_0        (CA_OTP_V200_BASE_OFFSET + 0x90)
#define CA_OTP_V200_DATA_LOCK_1        (CA_OTP_V200_BASE_OFFSET + 0x94)
#define CA_OTP_V200_ONE_WAY_0          (CA_OTP_V200_BASE_OFFSET + 0x98)
#define CA_OTP_V200_ONE_WAY_1          (CA_OTP_V200_BASE_OFFSET + 0x9C)
#define CA_OTP_V200_CSA2_ROOTKEY_0     (CA_OTP_V200_BASE_OFFSET + 0xA0)
#define CA_OTP_V200_CSA2_ROOTKEY_1     (CA_OTP_V200_BASE_OFFSET + 0xA4)
#define CA_OTP_V200_CSA2_ROOTKEY_2     (CA_OTP_V200_BASE_OFFSET + 0xA8)
#define CA_OTP_V200_CSA2_ROOTKEY_3     (CA_OTP_V200_BASE_OFFSET + 0xAC)
#define CA_OTP_V200_R2R_ROOTKEY_0      (CA_OTP_V200_BASE_OFFSET + 0xB0)
#define CA_OTP_V200_R2R_ROOTKEY_1      (CA_OTP_V200_BASE_OFFSET + 0xB4)
#define CA_OTP_V200_R2R_ROOTKEY_2      (CA_OTP_V200_BASE_OFFSET + 0xB8)
#define CA_OTP_V200_R2R_ROOTKEY_3      (CA_OTP_V200_BASE_OFFSET + 0xBC)
#define CA_OTP_V200_SP_ROOTKEY_0       (CA_OTP_V200_BASE_OFFSET + 0xC0)
#define CA_OTP_V200_SP_ROOTKEY_1       (CA_OTP_V200_BASE_OFFSET + 0xC4)
#define CA_OTP_V200_SP_ROOTKEY_2       (CA_OTP_V200_BASE_OFFSET + 0xC8)
#define CA_OTP_V200_SP_ROOTKEY_3       (CA_OTP_V200_BASE_OFFSET + 0xCC)
#define CA_OTP_V200_CSA3_ROOTKEY_0     (CA_OTP_V200_BASE_OFFSET + 0xD0)
#define CA_OTP_V200_CSA3_ROOTKEY_1     (CA_OTP_V200_BASE_OFFSET + 0xD4)
#define CA_OTP_V200_CSA3_ROOTKEY_2     (CA_OTP_V200_BASE_OFFSET + 0xD8)
#define CA_OTP_V200_CSA3_ROOTKEY_3     (CA_OTP_V200_BASE_OFFSET + 0xDC)
#define CA_OTP_V200_JTAG_KEY_0         (CA_OTP_V200_BASE_OFFSET + 0xE0)
#define CA_OTP_V200_JTAG_KEY_1         (CA_OTP_V200_BASE_OFFSET + 0xE4)
#define CA_OTP_V200_CA_CHIP_ID_0       (CA_OTP_V200_BASE_OFFSET + 0xE8)
#define CA_OTP_V200_CA_CHIP_ID_1       (CA_OTP_V200_BASE_OFFSET + 0xEC)
#define CA_OTP_V200_ESCK_0             (CA_OTP_V200_BASE_OFFSET + 0xF0)
#define CA_OTP_V200_ESCK_1             (CA_OTP_V200_BASE_OFFSET + 0xF4)
#define CA_OTP_V200_ESCK_2             (CA_OTP_V200_BASE_OFFSET + 0xF8)
#define CA_OTP_V200_ESCK_3             (CA_OTP_V200_BASE_OFFSET + 0xFC)
#define CA_OTP_V200_STB_ROOTKEY_0      (CA_OTP_V200_BASE_OFFSET + 0x100)
#define CA_OTP_V200_STB_ROOTKEY_1      (CA_OTP_V200_BASE_OFFSET + 0x104)
#define CA_OTP_V200_STB_ROOTKEY_2      (CA_OTP_V200_BASE_OFFSET + 0x108)
#define CA_OTP_V200_STB_ROOTKEY_3      (CA_OTP_V200_BASE_OFFSET + 0x10C)
#define CA_OTP_V200_STB_SN_0           (CA_OTP_V200_BASE_OFFSET + 0x110)
#define CA_OTP_V200_STB_SN_1           (CA_OTP_V200_BASE_OFFSET + 0x114)
#define CA_OTP_V200_STB_SN_2           (CA_OTP_V200_BASE_OFFSET + 0x118)
#define CA_OTP_V200_STB_SN_3           (CA_OTP_V200_BASE_OFFSET + 0x11C)
#define CA_OTP_V200_MKTS_ID            (CA_OTP_V200_BASE_OFFSET + 0x120)
#define CA_OTP_V200_DDR_ENC            (CA_OTP_V200_BASE_OFFSET + 0x180)
#define CA_OTP_V200_W_ONCE_ONLY        (CA_OTP_V200_BASE_OFFSET + 0x184)
#define CA_OTP_V200_DEBUG00            (CA_OTP_V200_BASE_OFFSET + 0x18C)
#define CA_OTP_V200_SECCPUJTAGKEY0     (CA_OTP_V200_BASE_OFFSET + 0x190)
#define CA_OTP_V200_SECCPUJTAGKEY1     (CA_OTP_V200_BASE_OFFSET + 0x194)
#define CA_OTP_V200_SECCPUJTAGKEY2     (CA_OTP_V200_BASE_OFFSET + 0x198)
#define CA_OTP_V200_SECCPUJTAGKEY3     (CA_OTP_V200_BASE_OFFSET + 0x19c)
#define CA_OTP_V200_DEBUG11            (CA_OTP_V200_BASE_OFFSET + 0x1B4)

/* OTP Structure */
typedef union
{
    struct
    {
        HI_U32 channel_sel         : 2; // [0-1]
        HI_U32 reserved            : 30; // [2-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_CHANNEL_SEL_U; //Offset:0x00

typedef union
{
    struct
    {
        HI_U32 wr_sel              : 1; // [0]
        HI_U32 rd_enable           : 1; // [1]
        HI_U32 wr_enable           : 1; // [2]
        HI_U32 rsv                 : 1; // [3]
        HI_U32 cpu_size            : 2; // [4-5]
        HI_U32 reserved            : 26; // [6-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_CPU_RW_CTRL_U; //Offset:0x04

typedef union
{
    struct
    {
        HI_U32 start              : 1; // [0]
        HI_U32 reserved           : 31; // [1-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_WR_START_U; //Offset:0x08

typedef union
{
    struct
    {
        HI_U32 ctrl_ready          : 1; // [0]
        HI_U32 fail_flag           : 1; // [1]
        HI_U32 soak_flag           : 1; // [2]
        HI_U32 rsv                 : 1; // [3]
        HI_U32 r_lock              : 1; // [4]
        HI_U32 w_lock              : 1; // [5]
        HI_U32 reserved            : 26; // [6-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_CTRL_STATUS_U;//Offset:0x0c

typedef union
{
    struct
    {
        HI_U32 rdata0              : 8; // [0-7]
        HI_U32 rdata1              : 8; // [8-15]
        HI_U32 rdata2              : 8; // [16-23]
        HI_U32 rdata3              : 8; // [24-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_RDATA_U;//Offset:0x10

typedef union
{
    struct
    {
        HI_U32 wdata              : 32; // [0-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_WDATA_U;//Offset:0x14

typedef union
{
    struct
    {
        HI_U32 raddr              : 10; // [0-9]
        HI_U32 reserved           : 22; // [10-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_RADDR_U;//Offset:0x18

typedef union
{
    struct
    {
        HI_U32 waddr              : 10; // [0-9]
        HI_U32 reserved           : 22; // [10-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_WADDR_U;//Offset:0x1C

typedef union
{
    struct
    {
        HI_U32 max_soak_times      : 4; // [0-3]
        HI_U32 otp_time            : 4; // [4-7]
        HI_U32 soak_en             : 1; // [8]
        HI_U32 time_en             : 1; // [9]
        HI_U32 reserved            : 22; // [10-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_MODE_U;//Offset:0x20

typedef union
{
    struct
    {
        HI_U32 addr_start        : 13; // [0-12]
        HI_U32 add_end           : 13; // [13-25]
        HI_U32 valid             : 1; // [26]
        HI_U32 reserved          : 5; // [27-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_LOCK_ADDR_0_U;//Offset:0x24

typedef union
{
    struct
    {
        HI_U32 addr_start        : 13; // [0-12]
        HI_U32 add_end           : 13; // [13-25]
        HI_U32 valid             : 1; // [26]
        HI_U32 reserved          : 5; // [27-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_LOCK_ADDR_1_U;//Offset:0x28

typedef union
{
    struct
    {
        HI_U32 addr_start        : 13; // [0-12]
        HI_U32 add_end           : 13; // [13-25]
        HI_U32 valid             : 1; // [26]
        HI_U32 reserved          : 5; // [27-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_LOCK_ADDR_2_U;//Offset:0x2c

typedef union
{
    struct
    {
        HI_U32 addr_start        : 13; // [0-12]
        HI_U32 add_end           : 13; // [13-25]
        HI_U32 valid             : 1; // [26]
        HI_U32 reserved          : 5; // [27-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_LOCK_ADDR_3_U;//Offset:0x30

typedef union
{
    struct
    {
        HI_U32 addr_lock_en      : 4; // [0-3]
        HI_U32 rsa_lock_en       : 1; // [4]
        HI_U32 reserved          : 27; // [5-31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_ADDR_LOCK_EN_U;//Offset:0x34

typedef union
{
    struct
    {
        HI_U32 rsv              : 4; // [3:0]
        HI_U32 soak_time1       : 4; // [7:4]
        HI_U32 soak_time2       : 4; // [11:8]
        HI_U32 soak_time3       : 4; // [15:12]
        HI_U32 soak_time4       : 4; // [19:16]
        HI_U32 soak_time5       : 4; // [23:20]
        HI_U32 soak_time6       : 4; // [27:24]
        HI_U32 soak_time7       : 4; // [31:28]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_SOAK_TIME_U;//Offset:0x48

typedef union
{
    struct
    {
        HI_U32 rsv              : 16; // [15:0]
        HI_U32 soak_addr        : 13; // [28:16]
        HI_U32 soak_time2       : 3; // [31:29]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_DEBUG_ADDR_0_U;//Offset:0x4c

typedef union
{
    struct
    {
        HI_U32 rdata          : 8; // [7:0]
        HI_U32 rfsm           : 8; // [15:8]
        HI_U32 raddr          : 16; // [31:16]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_DEBUG_0_U;//Offset:0x68

typedef union
{
    struct
    {
        HI_U32 wdata          : 8; // [7:0]
        HI_U32 wfsm           : 8; // [15:8]
        HI_U32 waddr          : 16; // [31:16]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_DEBUG_1_U;//Offset:0x6C

typedef union
{
    struct
    {
        HI_U32 jrdata            : 8; // [7:0]
        HI_U32 jaddr             : 10; // [17:8]
        HI_U32 reserved          : 6; // [23:18]
        HI_U32 jcs               : 1; // [24]
        HI_U32 jwr               : 1; // [25]
        HI_U32 jrdy              : 1; // [26]
        HI_U32 reserved1         : 1; // [27]
        HI_U32 jsize             : 2; // [29:28]
        HI_U32 otp_jtag_rw_flag  : 1; //[30]
        HI_U32 otp_ate_test_flag : 1; //[31]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_DEBUG_3_U;//Offset:0x74

typedef union
{
    struct
    {
        HI_U32 jwdata : 32; //[31:0]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_DEBUG_4_U;//Offset:0x78

typedef union
{
    struct
    {
        HI_U32 secret_key_lock    : 1;//    0x10[0]
        HI_U32 ca_chip_id_lock    : 1;//    0x10[1]
        HI_U32 esck_lock          : 1;//    0x10[2]
        HI_U32 Boot_RootKey_lock  : 1;//    0x10[3]
        HI_U32 stbsn0_lock        : 1;//    0x10[4]
        HI_U32 stbsn1_lock        : 1;//    0x10[5]
        HI_U32 stbsn2_lock        : 1;//    0x10[6]
        HI_U32 stbsn3_lock        : 1;//    0x10[7]
        HI_U32 msid_lock          : 1;//    0x11[0]
        HI_U32 version_id_lock    : 1;//    0x11[1]
        HI_U32 rsv32_1_lock       : 1;//    0x11[2]
        HI_U32 ca_vendor_id_lock  : 1;//    0x11[3]
        HI_U32 rsv8_0_lock        : 1;//    0x11[4]
        HI_U32 rsv8_1_lock        : 1;//    0x11[5]
        HI_U32 rsv8_2_lock        : 1;//    0x11[6]
        HI_U32 misc_rootkey_lock  : 1;//    0x11[7]
        HI_U32 HDCP_RootKey_lock  : 1;//    0x12[0]
        HI_U32 OEM_RootKey_lock   : 1;//    0x12[1]
        HI_U32 STB_RootKey_lock   : 1;//    0x12[2]
        HI_U32 rsv_data_2_lock    : 1;//    0x12[3] //lot_info_1_lock
        HI_U32 hdcp_lock          : 1;//    0x12[4]
        HI_U32 rsv_hdcp_lock      : 1;//    0x12[5]
        HI_U32 rsv_data_3to10_lock :8;//    0x12[6]~0x13[5]
        HI_U32 RSA_CRC_lock       :1;//     0x13[6]
        HI_U32 rsv_rsa_1_lock     :1;//     0x13[7]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_DATA_LOCK_0_U;//Offset:0x90

typedef union
{
    struct
    {
        HI_U32 rsa_key_lock : 1; //[0]
        HI_U32 reserved : 31; //[31:1]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_DATA_LOCK_1_U;//Offset:0x94

typedef union
{
    struct
    {
        HI_U32 SCS_activation           : 1; //18[0]
        HI_U32 jtag_prt_mode_0          : 1; //18[1]
        HI_U32 jtag_prt_mode_1          : 1; //18[2]
        HI_U32 csa2_deactivation        : 1; //18[3]
        HI_U32 csa3_deactivation        : 1; //18[4]
        HI_U32 r2r_deactivation         : 1; //18[5]
        HI_U32 sp_deactivation          : 1; //18[6]
        HI_U32 jtag_r_disable           : 1; //18[7]
        HI_U32 jtag_w_disable           : 1; //19[0]
        HI_U32 debug_disable            : 1; //19[1]
        HI_U32 self_test_disable        : 1; //19[2]
        HI_U32 right_ctrl_en            : 1; //19[3]
        HI_U32 arm_jtag_disable         : 1; //19[4]
        HI_U32 boot_info_disable        : 1; //19[5]
        HI_U32 checksum_read_disable    : 1; //19[6]
        HI_U32 reserved0                : 3; //19[7] ~ 1a[1]
        HI_U32 SecCPU_prt_mode_0        : 1; //1a[2]
        HI_U32 SecCPU_prt_mode_1        : 1; //1a[3]
        HI_U32 reserved1                : 2;//0x1a[5:4]
        HI_U32 SecCpuMode_0             : 1; //0x1a[6] //for Irdeto MSR2.2
        HI_U32 SecCpuMode_1             : 1; //0x1a[7] //for Irdeto MSR2.2
        HI_U32 dbg_port_mode            : 8; //0X1b
    } bits;
    HI_U32 u32;
} CA_OTP_V200_ONE_WAY_0_U;//Offset:0x18

typedef union
{
    struct
    {
        HI_U32 self_boot_disable_bak     : 8; //0x1c
        HI_U32 tz_area_lock              : 8; //0x1d
        HI_U32 DeviceID_lock_0           : 1; //0x1e[0]
        HI_U32 DeviceID_lock_1           : 1; //0x1e[1]
        HI_U32 CA_TA_RootKey_lock        : 1; //0x1e[2]
        HI_U32 STB_TA_RootKey_lock       : 1; //0x1e[3]
        HI_U32 TZ_JTAG_KEY_lock          : 1; //0x1e[4]
        HI_U32 SEC_STORE_RootKey_lock    : 1; //0x1e[5]
        HI_U32 LP_RootKey_lock           : 1; //0x1e[6]
        HI_U32 obfu_ext_key_lock         : 1; //0x1e[7]
        HI_U32 reserved                  : 8;
    } bits;
    HI_U32 u32;
} CA_OTP_V200_ONE_WAY_1_U;//Offset:0x1c




typedef union
{
    struct
    {
        HI_U32 ddr_en   : 1; //[0]
        HI_U32 reserved : 31; //[31:1]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_DDR_ENC_U;//Offset:0x180

typedef union
{
    struct
    {
        HI_U32 reg_w_once_only : 8; //[7:0]
        HI_U32 reserved        : 24; //[31:8]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_W_ONCE_ONLY_U;//Offset:0x184

/*
** OTP Permanence Value
*/
#define CA_OTP_V200_INTERNAL_PV_0                 (0x00)
//dolby_flag,dts_flag
#define CA_OTP_V200_INTERNAL_PV_1                 (0x04)
#define CA_OTP_V200_INTERNAL_PVLOCK_0             (0x08)
#define CA_OTP_V200_INTERNAL_PVLOCK_1             (0x0C)
#define CA_OTP_V200_INTERNAL_DATALOCK_0           (0x10)
#define CA_OTP_V200_INTERNAL_DATALOCK_1           (0x14)
#define CA_OTP_V200_INTERNAL_CHECKSUMLOCK         (0x430)
//debug_disable  ?
#define CA_OTP_V200_INTERNAL_ONEWAY_0             (0x18)
#define CA_OTP_V200_INTERNAL_ONEWAY_1             (0x1C)
#define CA_OTP_V200_INTERNAL_CSA2_ROOTKEY_0       (0x20)
#define CA_OTP_V200_INTERNAL_CSA2_ROOTKEY_1       (0x24)
#define CA_OTP_V200_INTERNAL_CSA2_ROOTKEY_2       (0x28)
#define CA_OTP_V200_INTERNAL_CSA2_ROOTKEY_3       (0x2C)
#define CA_OTP_V200_INTERNAL_R2R_ROOTKEY_0        (0x30)
#define CA_OTP_V200_INTERNAL_R2R_ROOTKEY_1        (0x34)
#define CA_OTP_V200_INTERNAL_R2R_ROOTKEY_2        (0x38)
#define CA_OTP_V200_INTERNAL_R2R_ROOTKEY_3        (0x3C)
#define CA_OTP_V200_INTERNAL_SP_ROOTKEY_0         (0x40)
#define CA_OTP_V200_INTERNAL_SP_ROOTKEY_1         (0x44)
#define CA_OTP_V200_INTERNAL_SP_ROOTKEY_2         (0x48)
#define CA_OTP_V200_INTERNAL_SP_ROOTKEY_3         (0x4C)
#define CA_OTP_V200_INTERNAL_CSA3_ROOTKEY_0       (0x50)
#define CA_OTP_V200_INTERNAL_CSA3_ROOTKEY_1       (0x54)
#define CA_OTP_V200_INTERNAL_CSA3_ROOTKEY_2       (0x58)
#define CA_OTP_V200_INTERNAL_CSA3_ROOTKEY_3       (0x5C)
#define CA_OTP_V200_INTERNAL_JTAG_KEY_0           (0x60)
#define CA_OTP_V200_INTERNAL_JTAG_KEY_1           (0x64)
//chip id
#define CA_OTP_V200_INTERNAL_CA_CHIP_ID_0         (0x68)
#define CA_OTP_V200_INTERNAL_CA_CHIP_ID_1         (0x6C)
//hdcp key
#define CA_OTP_V200_INTERNAL_ESCK_0               (0x70)
#define CA_OTP_V200_INTERNAL_ESCK_1               (0x74)
#define CA_OTP_V200_INTERNAL_ESCK_2               (0x78)
#define CA_OTP_V200_INTERNAL_ESCK_3               (0x7C)
#define CA_OTP_V200_INTERNAL_BOOT_ROOTKEY         (0x80)

#define CA_OTP_V200_INTERNAL_STB_SN_0             (0x90)
#define CA_OTP_V200_INTERNAL_STB_SN_1             (0x94)
#define CA_OTP_V200_INTERNAL_STB_SN_2             (0x98)
#define CA_OTP_V200_INTERNAL_STB_SN_3             (0x9C)
#define CA_OTP_V200_INTERNAL_ITCSA3_IMLB          (0x9C)
#define CA_OTP_V200_INTERNAL_MSID                 (0xA0)
#define CA_OTP_V200_INTERNAL_VERSIONID            (0xA4)
#define CA_OTP_V200_INTERNAL_IDWORD               (0xA8)
#define CA_OTP_V200_INTERNAL_CA_VENDOR_ID         (0xAC)
#define CA_OTP_V200_INTERNAL_MISC_ROOTKEY_0       (0xB0)
#define CA_OTP_V200_INTERNAL_MISC_ROOTKEY_1       (0xB4)
#define CA_OTP_V200_INTERNAL_MISC_ROOTKEY_2       (0xB8)
#define CA_OTP_V200_INTERNAL_MISC_ROOTKEY_3       (0xBC)
#define CA_OTP_V200_INTERNAL_HDCP_ROOTKEY_0       (0xC0)
#define CA_OTP_V200_INTERNAL_HDCP_ROOTKEY_1       (0xC4)
#define CA_OTP_V200_INTERNAL_HDCP_ROOTKEY_2       (0xC8)
#define CA_OTP_V200_INTERNAL_HDCP_ROOTKEY_3       (0xCC)
#define CA_OTP_V200_INTERNAL_OEM_ROOTKEY_0        (0xD0)
#define CA_OTP_V200_INTERNAL_OEM_ROOTKEY_1        (0xD4)
#define CA_OTP_V200_INTERNAL_OEM_ROOTKEY_2        (0xD8)
#define CA_OTP_V200_INTERNAL_OEM_ROOTKEY_3        (0xDC)

#define CA_OTP_V200_NOMAL_IDWORD_VALUE            (0x2A13C812)
/* When otp_tz_area_enable is 1£¬Only secure CPU can access them. */
#define CA_OTP_V200_INTERNAL_TZ_OTP_BASE          (0x100)   /* 0x100 ~ 0x17F */
#define CA_OTP_V200_INTERNAL_SEC_OTP_END          (0x180)
#define CA_OTP_V200_INTERNAL_SEC_STORE_ROOTKEY    (0x1D0)
#define CA_OTP_V200_INTERNAL_RSA_KEY_E_BASE       (0x2F0)
#define CA_OTP_V200_INTERNAL_RSA_KEY_N_BASE       (0x300)
#define CA_OTP_V200_INTERNAL_RSA_KEY_CRC_BASE     (0x2d0)
/* Chipset configure */
#define CA_OTP_V200_CHIPSET_CONFIGURE_0                    (0x400)
#define CA_OTP_V200_CHIPSET_CONFIGURE_1                    (0x404)
#define CA_OTP_V200_CHIPSET_CONFIGURE_2                    (0x408)
#define CA_OTP_V200_CHIPSET_CONFIGURE_LOCK_0               (0x410)
#define CA_OTP_V200_CHIPSET_CONFIGURE_LOCK_1               (0x414)
#define CA_OTP_V200_CHIPSET_CONFIGURE_LOCK_2               (0x418)

/* checksum for secret key */
#define CA_OTP_V200_INTERNAL_CHECKSUM_CSA2_ROOT_KEY        (0x420)
#define CA_OTP_V200_INTERNAL_CHECKSUM_R2R_ROOT_KEY         (0x421)
#define CA_OTP_V200_INTERNAL_CHECKSUM_SP_ROOT_KEY          (0x422)
#define CA_OTP_V200_INTERNAL_CHECKSUM_CSA3_ROOT_KEY        (0x423)
#define CA_OTP_V200_INTERNAL_CHECKSUM_JTAGKEY_CHIPID       (0x424)
#define CA_OTP_V200_INTERNAL_CHECKSUM_ESCK                 (0x425)
#define CA_OTP_V200_INTERNAL_CHECKSUM_BOOT_ROOT_KEY        (0x426)
#define CA_OTP_V200_INTERNAL_CHECKSUM_MISC_ROOT_KEY        (0x427)
#define CA_OTP_V200_INTERNAL_CHECKSUM_OEM_ROOT_KEY         (0x429)
#define CA_OTP_V200_INTERNAL_CHECKSUM_SEC_STORE_ROOT_KEY   (0x42E)

/* SELFBOOT BACKUP */
#define SELFBOOT_BAK_ENABLE                       (0x11)
#define SELFBOOT_BAK_DISABLE                      (0xFF)
/* SCS BACKUP */
#define SCS_BAK_DISABLE                           (0x81)
#define SCS_BAK_ENABLE                            (0xFF)        //If not 0x81, it will be considered as scs enable
/* JTAG MODE BACKUP */
#define JTAG_MODE_BAK_OPEN                        (0x42)
#define JTAG_MODE_BAK_PROTECT                     (0xFF)
#define JTAG_MODE_BAK_VMX_PROTECT                 (0x63)
#define JTAG_MODE_BAK_CLOSE                       (0xFF)        //If not 0x42 or 0x63, it will be considered as closed

#ifdef CHIP_TYPE_hi3716mv300
/*used for MV300 EC*/
#define CA_OTP_V200_INTERNAL_SCS_EN_BAK           (0x1B)
#define CA_OTP_V200_INTERNAL_JTAG_MODE_BAK        (0x1D)
#define CA_OTP_V200_INTERNAL_RIGHT_CTRL_EN_BAK    (0x1F)
#else
#define CA_OTP_V200_INTERNAL_SCS_EN_BAK           (0xAD)
#define CA_OTP_V200_INTERNAL_JTAG_MODE_BAK        (0xAE)
#define CA_OTP_V200_INTERNAL_RIGHT_CTRL_EN_BAK    (0xAF)
#define CA_OTP_V200_INTERNAL_DBG_PORT_MODE        (0X1B) //only use for irdeto MSR2.2
#endif

#define CA_OTP_V200_INTERNAL_SELFBOOT_DISABLE_BAK (0x1C)

/*used for MV300 EC*/
#define CA_OTP_V200_INTERNAL_ID_WORD_1            (0x05)
#define CA_OTP_V200_INTERNAL_ID_WORD_2            (0x1A)
#define CA_OTP_V200_INTERNAL_ID_WORD_3            (0x1C)
#define CA_OTP_V200_INTERNAL_ID_WORD_4            (0x1E)

#define SECURE_CHIP_ID_WORD_1                     (0x53)
#define SECURE_CHIP_ID_WORD_1_MASK                (0x7F)
#define SECURE_CHIP_ID_WORD_2                     (0xE9)
#define SECURE_CHIP_ID_WORD_3                     (0xDB)
#define SECURE_CHIP_ID_WORD_4                     (0x6E)

#define NONE_SECURE_CHIP_ID_WORD_1                (0x12)
#define NONE_SECURE_CHIP_ID_WORD_2                (0xC8)
#define NONE_SECURE_CHIP_ID_WORD_3                (0x13)
#define NONE_SECURE_CHIP_ID_WORD_4                (0x2A)
/*end used for MV300 EC*/

typedef union
{
    struct
    {
        HI_U32 boot_mode_sel_0      : 1;//0x00[0]
        HI_U32 boot_mode_sel_1      : 1;//0x00[1]
        HI_U32 bload_dec_en         : 1;//0x00[2]
        HI_U32 self_boot_disable    : 1;//0x00[3]
        HI_U32 ddr_wakeup_disable   : 1;//0x00[4]
        HI_U32 csa2_lv_sel          : 1;//0x00[5]
        HI_U32 r2r_lv_sel           : 1;//0x00[6]
        HI_U32 sp_lv_sel            : 1;//0x00[7]
        HI_U32 csa3_lv_sel          : 1;//0x01[0]
        HI_U32 link_prt_disable     : 1;//0x01[1]
        HI_U32 ts_csa2_hardonly_en  : 1;//0x01[2]
        HI_U32 ts_sp_hardonly_en    : 1;//0x01[3]
        HI_U32 ts_des_hardonly_en   : 1;//0x01[4]
        HI_U32 ts_nv_hardonly_en    : 1;//0x01[5]
        HI_U32 ts_csa3_hardonly_en  : 1;//0x01[6]
        HI_U32 ts_others_hardonly_en: 1;//0x01[7]
        HI_U32 tskl_csa2_disable    : 1;//0x02[0]
        HI_U32 tskl_sp_disable      : 1;//0x02[1]
        HI_U32 tskl_des_disable     : 1;//0x02[2]
        HI_U32 tskl_nv_disable      : 1;//0x02[3], as same as tskl_sm4_disable0 for Hi3716mv420
        HI_U32 tskl_csa3_disable    : 1;//0x02[4]
        HI_U32 tskl_others_disable  : 1;//0x02[5], tskl_sm4_disable1
        HI_U32 mc_aes_hardonly_en   : 1;//0x02[6]
        HI_U32 mc_tdes_hardonly_en  : 1;//0x02[7]
        HI_U32 mc_des_hardonly_en   : 1;//0x03[0], use as "itcsa3_disable" in irdeto MSR2.2
        HI_U32 mc_rc4_hardonly_en   : 1;//0x03[1], used as "itcsa3_enable" in irdeto MSR2.2
        HI_U32 mckl_des_hard_disable: 1;//0x03[2], use as "runcheck_lv_0" in irdeto MSR2.2
        HI_U32 mckl_rc4_disable     : 1;//0x03[3]
        HI_U32 tskl_tdes_disable    : 1;//0x03[4]  DEMUX TDES engine control.
        HI_U32 bload_mode_sel       : 1;//0x03[5]
        HI_U32 ddr_scramble_en      : 1;//0x03[6]
        HI_U32 otp_global_lock_en   : 1;//0x03[7]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_INTERNAL_PV_0_U;


typedef union
{
    struct
    {
        HI_U32 secure_chip_flag      : 1;//0x04[0]
        HI_U32 soc_tz_enable         : 1;//0x04[1]
        HI_U32 otp_tz_area_enable    : 1;//0x04[2]
        HI_U32 misc_kl_disable       : 1;//0x04[3]
        HI_U32 gg_kl_disable         : 1;//0x04[4]
        HI_U32 dcas_kl_disable       : 1;//0x04[5]
        HI_U32 kl_dpa_clk_sel_en     : 1;//0x04[6]
        HI_U32 kl_dpa_filter_clk_en  : 1;//0x04[7]
        HI_U32 mc_dpa_clk_sel_en     : 1;//0x05[0]
        HI_U32 pvr_dpa_filter_clk_en : 1;//0x05[1]
        HI_U32 uart_disable          : 1;//0x05[2] //reserved no use
        HI_U32 pcie_disable          : 1;//0x05[3] //reserved no use
        HI_U32 obfu_enable           : 1;//0x05[4]
        HI_U32 boot_mode_sel_2       : 1;//0x05[5] only use for hi3716mv410 and hi3716mv420
        HI_U32 vmx_bl_fuse           : 1;//0x05[6]
        HI_U32 ts_out_disable        : 1;//0x05[7]
        HI_U32 lpc_disable           : 1;//0x06[0]
        HI_U32 lpc_master_disable    : 1;//0x06[1]
        HI_U32 bootsel_ctrl          : 1;//0x06[2]
        HI_U32 bload_enc_disable     : 1;//0x06[3]
        HI_U32 runtime_check_en      : 1;//0x06[4]
        HI_U32 dolby_flag            : 1;//0x06[5]
        HI_U32 macrovision_flag      : 1;//0x06[6]
        HI_U32 dts_flag              : 1;//0x06[7]
        HI_U32 wakeup_ddr_check_en   : 1;//0x07[0]
        HI_U32 misc_lv_sel           : 1;//0x07[1]
        HI_U32 version_id_check_en   : 1;//0x07[2]
        HI_U32 bl_msid_check_en      : 1;//0x07[3]
        HI_U32 nf_rng_disable        : 1;//0x07[4]
        HI_U32 soft_hash_flag        : 1;//0x07[5]
        HI_U32 sp_lv_sel_2           : 1;//0x07[6]
        HI_U32 misc_lv_sel_2         : 1;//0x07[7]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_INTERNAL_PV_1_U;


typedef union
{
    struct
    {
        HI_U32 boot_mode_sel_0_lock        : 1;//0x08[0]
        HI_U32 boot_mode_sel_1_lock        : 1;//0x08[1]
        HI_U32 bload_dec_en_lock           : 1;//0x08[2]
        HI_U32 self_boot_disable_lock      : 1;//0x08[3]
        HI_U32 ddr_wakeup_disable_lock     : 1;//0x08[4]
        HI_U32 csa2_lv_sel_lock            : 1;//0x08[5]
        HI_U32 r2r_lv_sel_lock             : 1;//0x08[6]
        HI_U32 sp_lv_sel_lock              : 1;//0x08[7]
        HI_U32 csa3_lv_sel_lock            : 1;//0x09[0]
        HI_U32 link_prt_disable_lock       : 1;//0x09[1]
        HI_U32 ts_csa2_hardonly_en_lock    : 1;//0x09[2]
        HI_U32 ts_spe_hardonly_en_lock     : 1;//0x09[3]
        HI_U32 reserved1                   : 1;//0x09[4]
        HI_U32 ts_nv_hardonly_en_lock      : 1;//0x09[5]
        HI_U32 ts_csa3_hardonly_en_lock    : 1;//0x09[6]
        HI_U32 ts_others_hardonly_en_lock  : 1;//0x09[7]
        HI_U32 tskl_csa2_disable_lock      : 1;//0x0a[0]
        HI_U32 tskl_spe_disable_lock       : 1;//0x0a[1]
        HI_U32 reserved2                   : 1;//0x0a[2]
        HI_U32 tskl_nv_disable_lock        : 1;//0x0a[3] //as same as tskl_sm4_disable0_lock in Hi3716MV420
        HI_U32 tskl_csa3_disable_lock      : 1;//0x0a[4]
        HI_U32 tskl_others_disable_lock    : 1;//0x0a[5] //tskl_sm4_disable1_lock
        HI_U32 mc_aes_hardonly_en_lock     : 1;//0x0a[6]
        HI_U32 mc_tdes_hardonly_en_lock    : 1;//0x0a[7]
        HI_U32 reserved3                   : 4;//0x0b[0]~0x0b[3]
        HI_U32 tskl_tdes_disable_lock      : 1;//0x0b[4]
        HI_U32 bload_mode_sel_lock         : 1;//0x0b[5]
        HI_U32 ddr_scramble_en_lock        : 1;//0x0b[6]
        HI_U32 otp_globle_lock_en_lock     : 1;//0x0b[7]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_INTERNAL_PVLOCK_0_U;


typedef union
{
    struct
    {
        HI_U32 secure_chip_flag_lock       : 1;//0x0c[0]
        HI_U32 soc_tz_enable_lock          : 1;//0x0c[1]
        HI_U32 otp_tz_area_enable_lock     : 1;//0x0c[2]
        HI_U32 misc_kl_disable_lock        : 1;//0x0c[3]
        HI_U32 gg_kl_disable_lock          : 1;//0x0c[4]
        HI_U32 dcas_kl_en_lock             : 1;//0x0c[5]
        HI_U32 kl_dpa_clk_sel_en_lock      : 1;//0x0c[6]
        HI_U32 kl_dpa_filter_clk_en_lock   : 1;//0x0c[7]
        HI_U32 mc_dpa_clk_sel_en_lock      : 1;//0x0d[0]
        HI_U32 pvr_dpa_filter_clk_en_lock  : 1;//0x0d[1]
        HI_U32 reserved0                   : 2;//0x0d[2]~0x0d[3]
        HI_U32 obfu_enable_lock            : 1;//0x0d[4]
        HI_U32 boot_mode_sel_2_lock        : 1;//0x0d[5] //only use for hi3716mv410 and hi3716mv420
        HI_U32 reserved                    : 1;//0x0d[6]
        HI_U32 ts_out_disable_lock         : 1;//0x0d[7]
        HI_U32 lpc_disable_lock            : 1;//0x0e[0]
        HI_U32 lpc_master_disable_lock     : 1;//0x0e[1]
        HI_U32 bootsel_ctrl_lock           : 1;//0x0e[2]
        HI_U32 bload_enc_disable_lock      : 1;//0x0e[3]
        HI_U32 runtime_check_en_lock       : 1;//0x0e[4]
        HI_U32 reserved1                   : 3;//0x0e[5]~0x0e[7]
        HI_U32 wakeup_ddr_check_en_lock    : 1;//0x0f[0]
        HI_U32 misc_lv_sel_lock            : 1;//0x0f[1]
        HI_U32 version_id_check_en_lock    : 1;//0x0f[2]
        HI_U32 bl_msid_check_en_lock       : 1;//0x0f[3]
        HI_U32 nf_rng_disable_lock         : 1;//0x0f[4]
        HI_U32 soft_hash_flag_lock         : 1;//0x0f[5]
        HI_U32 sp_lv_sel_2_lock            : 1;//0x0f[6]
        HI_U32 misc_lv_sel_2_lock          : 1;//0x0f[7]
    } bits;
    HI_U32 u32;
}CA_OTP_V200_INTERNAL_PVLOCK_1_U;

typedef union
{
    struct
    {
        HI_U32 secret_key_lock          : 1;//  0x10[0] ONLY FOR CSA2 ROOT KEY LOCK
        HI_U32 ca_chip_id_lock          : 1;//  0x10[1]
        HI_U32 esck_lock                : 1;//  0x10[2]
        HI_U32 Boot_RootKey_lock        : 1;//  0x10[3]
        HI_U32 stbsn0_lock              : 1;//  0x10[4]
        HI_U32 stbsn1_lock              : 1;//  0x10[5]
        HI_U32 stbsn2_lock              : 1;//  0x10[6]
        HI_U32 stbsn3_lock              : 1;//  0x10[7] used as ITCSA3_IMLB LOCK in Irdeto MSR2.2
        HI_U32 msid_lock                : 1;//  0x11[0]
        HI_U32 version_id_lock          : 1;//  0x11[1]
        HI_U32 id_word_lock             : 1;//  0x11[2]
        HI_U32 ca_vendor_id_lock        : 1;//  0x11[3]
        HI_U32 scs_en_bak_lock          : 1;//  0x11[4]
        HI_U32 jtag_mode_bak_lock       : 1;//  0x11[5]
        HI_U32 right_ctrl_en_bak_lock   : 1;//  0x11[6]
        HI_U32 misc_rootkey_lock        : 1;//  0x11[7]
        HI_U32 HDCP_RootKey_lock        : 1;//  0x12[0]
        HI_U32 OEM_RootKey_lock         : 1;//  0x12[1]
        HI_U32 STB_RootKey_lock         : 1;//  0x12[2]
        HI_U32 rsv_data_2_lock          : 1;//  0x12[3] //lot_info_1_lock
        HI_U32 hdcp_lock                : 1;//  0x12[4]
        HI_U32 rsv_hdcp_lock            : 1;//  0x12[5]
        HI_U32 rsv_data_3to10_lock      : 8;//  0x12[6]~0x13[5]
        HI_U32 RSA_CRC_lock             : 1;//  0x13[6]
        HI_U32 rsv_rsa_1_lock           : 1;//  0x13[7]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_INTERNAL_DATALOCK_0_U;
typedef union
{
    struct
    {
        HI_U32 rsa_key_lock         : 1;//  0x14[0]
        HI_U32 reserved0            : 7;//  0x14[7~1]
        HI_U32 CSA3_RootKey_lock    : 1;//  0x15[0]
        HI_U32 R2R_RootKey_lock     : 1;//  0x15[1]
        HI_U32 SP_RootKey_lock      : 1;//  0x15[2]
        HI_U32 JTAG_Key_lock        : 1;//  0x15[3]
        HI_U32 reserved1            : 19;// 0x15[4] ~ 0x17[6]
        HI_U32 prm_err_flag         : 1;//  0x17[7]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_INTERNAL_DATALOCK_1_U;

//Chipset Configuration
typedef union
{
    struct
    {
        HI_U32 reserved0            : 3;//  0x400[2~0]
        HI_U32 gpu_disable          : 1;//  0x400[3]
        HI_U32 reserved1            : 3;//  0x400[6~4]
        HI_U32 vdec_control         : 1;//  0x400[7]
        HI_U32 qam_disable          : 1;//  0x401[0]
        HI_U32 reserved2            : 3;//  0x401[3~1]
        HI_U32 vdac_disable         : 4;//  0x401[7~4]
        HI_U32 soc_identity         : 5;//  0x402[4~0]
        HI_U32 reserved3            : 3;//  0x402[7~5]
        HI_U32 macrovision_disable  : 1;//  0x403[0]
        HI_U32 venc_ctrl            : 2;//  0x403[2~1]
        HI_U32 reserved4            : 5;//  0x403[7~3]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_CHIPSET_CFG_0_U;

typedef union
{
    struct
    {
        HI_U32 pcie_disable         : 1;//  0x404[0]
        HI_U32 sata_disable         : 1;//  0x404[1]
        HI_U32 reserved0            : 1;//  0x404[2]
        HI_U32 sf_disable           : 1;//  0x404[3]
        HI_U32 uart_disable_0       : 1;//  0x404[4]
        HI_U32 uart_disable_1       : 1;//  0x404[5]
        HI_U32 reserved1            : 3;//  0x404[7~6] ~ 0x405[0]
        HI_U32 real_disable         : 1;//  0x405[1]
        HI_U32 divx3_disable        : 1;//  0x405[2]
        HI_U32 vc1_disable          : 1;//  0x405[3]
        HI_U32 vp6_disable          : 1;//  0x405[4]
        HI_U32 vp8_disable          : 1;//  0x405[5]
        HI_U32 h264_disable         : 1;//  0x405[6]
        HI_U32 mpeg4_disable        : 1;//  0x405[7]
        HI_U32 avs_disable          : 1;//  0x406[0]
        HI_U32 rmii_disable         : 1;//  0x406[1]
        HI_U32 reserved2            : 1;//  0x406[2]
        HI_U32 usb2_disable         : 1;//  0x406[3]
        HI_U32 tsi0_disable         : 1;//  0x406[4]
        HI_U32 reserved3            : 11;//  0x406[7~5] ~0x407[7~0]

    } bits;
    HI_U32 u32;
} CA_OTP_V200_CHIPSET_CFG_1_U;


typedef union
{
    struct
    {
        HI_U32 mpeg2_disable        : 1;//  0x408[0]
        HI_U32 h265_disable         : 1;//  0x408[1]
        HI_U32 reserved0            : 6;//  0x408[7~2]
        HI_U32 venc_disable         : 1;//  0x409[0]
        HI_U32 gmii_disable         : 1;//  0x409[1]
        HI_U32 tsi1_disable         : 1;//  0x409[2]
        HI_U32 tsi2_disable         : 1;//  0x409[3]
        HI_U32 tsi3_disable         : 1;//  0x409[4]
        HI_U32 tsi4_disable         : 1;//  0x409[5]
        HI_U32 tsi5_disable         : 1;//  0x409[6]
        HI_U32 reserved3            : 17;// 0x409[7]~0x40b[7~0]

    } bits;
    HI_U32 u32;
} CA_OTP_V200_CHIPSET_CFG_2_U;

typedef union
{
    struct
    {
        HI_U32 reserved0               : 3; // 0x410[2~0]
        HI_U32 gpu_disable_lock        : 1; // 0x410[3]
        HI_U32 reserved1               : 3; // 0x410[6~4]
        HI_U32 vdec_control_lock       : 1; // 0x410[7]
        HI_U32 qam_disable_lock        : 1; // 0x411[0]
        HI_U32 reserved2               : 3; // 0x411[3~1]
        HI_U32 vdac_disable_lock       : 4; // 0x411[7~4]
        HI_U32 soc_identity_lock       : 5; // 0x412[4~0]
        HI_U32 reserved3               : 3; // 0x412[7~5]
        HI_U32 macrovision_disable_lock: 1; // 0x413[0]
        HI_U32 venc_ctrl_lock          : 2; // 0x413[2~1]
        HI_U32 reserved4               : 5; // 0x413[7~3]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_CHIPSET_CFG_LOCK_0_U;

typedef union
{
    struct
    {
        HI_U32 pcie_disable_lock      : 1; // 0x414[0]
        HI_U32 sata_disable_lock      : 1; // 0x414[1]
        HI_U32 reserved0              : 1; // 0x414[2]
        HI_U32 sf_disable_lock        : 1; // 0x414[3]
        HI_U32 uart_disable_0_lock    : 1; // 0x414[4]
        HI_U32 uart_disable_1_lock    : 1; // 0x414[5]
        HI_U32 reserved1              : 3; // 0x414[7~6] ~ 0x415[0]
        HI_U32 real_disable_lock      : 1; // 0x415[1]
        HI_U32 divx3_disable_lock     : 1; // 0x415[2]
        HI_U32 vc1_disable_lock       : 1; // 0x415[3]
        HI_U32 vp6_disable_lock       : 1; // 0x415[4]
        HI_U32 vp8_disable_lock       : 1; // 0x415[5]
        HI_U32 h264_disable_lock      : 1; // 0x415[6]
        HI_U32 mpeg4_disable_lock     : 1; // 0x415[7]
        HI_U32 avs_disable_lock       : 1; // 0x416[0]
        HI_U32 rmii_disable_lock      : 1; // 0x416[1]
        HI_U32 reserved2              : 1; // 0x416[2]
        HI_U32 usb2_disable_lock      : 1; // 0x416[3]
        HI_U32 tsi0_disable_lock      : 1; // 0x416[4]
        HI_U32 reserved3              : 11; // 0x416[7~5] ~ 0x417[7~0]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_CHIPSET_CFG_LOCK_1_U;


typedef union
{
    struct
    {
        HI_U32 mpeg2_disable_lock     : 1; // 0x418[0]
        HI_U32 h265_disable_lock      : 1; // 0x418[1]
        HI_U32 reserved0              : 6; // 0x418[7~2]
        HI_U32 venc_disable_lock      : 1; // 0x419[0]
        HI_U32 gmii_disable_lock      : 1; // 0x419[1]
        HI_U32 tsi1_disable_lock      : 1; // 0x419[2]
        HI_U32 tsi2_disable_lock      : 1; // 0x419[3]
        HI_U32 tsi3_disable_lock      : 1; // 0x419[4]
        HI_U32 tsi4_disable_lock      : 1; // 0x419[5]
        HI_U32 tsi5_disable_lock      : 1; // 0x419[6]
        HI_U32 reserved1              : 17; // 0x416[7~5] ~ 0x417[7~0]
    } bits;
    HI_U32 u32;
} CA_OTP_V200_CHIPSET_CFG_LOCK_2_U;

typedef union
{
    struct
    {
        HI_U32 locker_CSA2_RootKey          :1;// 0x430[0]
        HI_U32 locker_R2R_RootKey           :1;// 0x430[1]
        HI_U32 locker_SP_RootKey            :1;// 0x430[2]
        HI_U32 locker_CSA3_RootKey          :1;// 0x430[3]
        HI_U32 locker_JTAGKey_ChipID        :1;// 0x430[4]
        HI_U32 locker_ESCK                  :1;// 0x430[5]
        HI_U32 locker_BOOT_RootKey          :1;// 0x430[6]
        HI_U32 locker_MISC_RootKey          :1;// 0x430[7]
        HI_U32 locker_HDCP_RootKey          :1;// 0x431[0]
        HI_U32 locker_OEM_RootKey           :1;// 0x431[1]
        HI_U32 locker_STB_ROOTKEY           :1;// 0x431[2]
        HI_U32 locker_CA_TA_ROOTKEY         :1;// 0x431[3]
        HI_U32 locker_STB_TA_RootKey        :1;// 0x431[4]
        HI_U32 locker_TZ_JTAG_KEY           :1;// 0x431[5]
        HI_U32 locker_SEC_STORE_ROOTKEY     :1;// 0x431[6]
        HI_U32 locker_LP_ROOTKEY            :1;// 0x431[7]
        HI_U32 reserved                     :16;// 0x432~
    }bits;
    HI_U32 u32;
}CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U;

#endif/* __DRV_CA_OTP_REG_V200_H__ */
/*--------------------------------------END------------------------------------*/
