/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : otp.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __OTP_H__
#define __OTP_H__

#include "hi_type.h"
#include "hi_debug.h"

#define HI_ERR_OTP(fmt...)      HI_ERR_PRINT(HI_ID_OTP, fmt)
#define HI_INFO_OTP(fmt...)     HI_INFO_PRINT(HI_ID_OTP, fmt)

#ifndef HI_ADVCA_FUNCTION_RELEASE
#ifndef HI_MINIBOOT_SUPPORT
#define OTPCOMMANDTEST
#endif
#endif

#define OTP_CUSTOMER_KEY_LEN        (16)

/*
   SCS BACKUP
   If not 0x81, it will be considered as scs enable
*/
#define SCS_BAK_DISABLE             (0x81)
#define SCS_BAK_ENABLE              (0xFF)

/* otp register address */
#define OTP_V200_BASE_OFFSET        (0xF8AB0000)
#define OTP_V200_CHANNEL_SEL        (OTP_V200_BASE_OFFSET + 0x00)
#define OTP_V200_CPU_RW_CTRL        (OTP_V200_BASE_OFFSET + 0x04)
#define OTP_V200_WR_START           (OTP_V200_BASE_OFFSET + 0x08)
#define OTP_V200_CTRL_STATUS        (OTP_V200_BASE_OFFSET + 0x0c)
#define OTP_V200_RDATA              (OTP_V200_BASE_OFFSET + 0x10)
#define OTP_V200_WDATA              (OTP_V200_BASE_OFFSET + 0x14)
#define OTP_V200_RADDR              (OTP_V200_BASE_OFFSET + 0x18)
#define OTP_V200_WADDR              (OTP_V200_BASE_OFFSET + 0x1C)
#define OTP_V200_MODE               (OTP_V200_BASE_OFFSET + 0x20)

#define OTP_V200_CHIPID             (OTP_V200_BASE_OFFSET + 0xe8)
#define OTP_V200_STBSN              (OTP_V200_BASE_OFFSET + 0x110)
#define OTP_V200_MARKETID           (OTP_V200_BASE_OFFSET + 0x120)
#define OTP_V200_SecureBootEn       (OTP_V200_BASE_OFFSET + 0x98)
#define OTP_V200_CA_VENDOR_ID       (OTP_V200_BASE_OFFSET + 0x12C)
#define OTP_V200_PVREGISTER0        (OTP_V200_BASE_OFFSET + 0x80)

/* otp address */
#define OTP_INTERNAL_PV_0           (0x00)
#define OTP_INTERNAL_PV_1           (0x04)
#define OTP_INTERNAL_PVLOCK_0       (0x08)
#define OTP_INTERNAL_PVLOCK_1       (0x0C)
#define OTP_INTERNAL_DATALOCK_0     (0x10)
#define OTP_INTERNAL_DATALOCK_1     (0x14)
#define OTP_INTERNAL_ONEWAY_0       (0x18)
#define OTP_INTERNAL_ONEWAY_1       (0x1C)
#define OTP_RIGHT_CTRL_EN_ADDR      (0x19)
#define OTP_CHIP_ID_ADDR            (0x68)
#define OTP_ADVCA_ID_WORD_ADDR      (0xa8)      //0xa8 ~ 0xab
#define OTP_SCS_EN_BAK_ADDR         (0xad)
#define OTP_JTAG_MODE_BAK_ADDR      (0xae)
#define OTP_RIGHT_CTRL_EN_BAK_ADDR  (0xaf)
#define OTP_SELF_BOOT_DIABLE_BAK_ADDR   (0x1c)

#define ADVCA_ID_WORD               (0x6EDBE953)

#define OTP_STB_PRIV_DATA_ADDR      (0x2b0)
#define OTP_CUSTOMER_KEY_ADDR       (0x2c0)
#define OTP_RSA_KEY_E_ADDR          (0x2f0)
#define OTP_RSA_KEY_N_ADDR          (0x300)

typedef struct
{
    HI_U32 u32CustomerKey[4];
} OTP_CUSTOMER_KEY_S;

typedef struct
{
    HI_U32 u32Offset;
    HI_U8 u8Data;
} OTP_STB_PRIV_DATA_S;

typedef union
{
    struct
    {
        HI_U32 channel_sel         : 2; // [0-1]
        HI_U32 reserved            : 30; // [2-31]
    } bits;
    HI_U32 u32;
} OTP_V200_CHANNEL_SEL_U; //Offset:0x00

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
} OTP_V200_CPU_RW_CTRL_U; //Offset:0x04

typedef union
{
    struct
    {
        HI_U32 start              : 1; // [0]
        HI_U32 reserved           : 31; // [1-31]
    } bits;
    HI_U32 u32;
} OTP_V200_WR_START_U; //Offset:0x08

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
} OTP_V200_CTRL_STATUS_U;//Offset:0x0c

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
} OTP_V200_RDATA_U;//Offset:0x10

typedef union
{
    struct
    {
        HI_U32 wdata              : 32; // [0-31]
    } bits;
    HI_U32 u32;
} OTP_V200_WDATA_U;//Offset:0x14

typedef union
{
    struct
    {
        HI_U32 raddr              : 32; // [0-31]
    } bits;
    HI_U32 u32;
} OTP_V200_RADDR_U;//Offset:0x18

typedef union
{
    struct
    {
        HI_U32 waddr              : 32; // [0-31]
    } bits;
    HI_U32 u32;
} OTP_V200_WADDR_U;//Offset:0x1C

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
} OTP_V200_MODE_U;//Offset:0x20

#ifdef CHIP_TYPE_hi3716cv200es
typedef union
{
    struct
    {
        HI_U32 ca_kl_bus_cken  : 1; //[0]
        HI_U32 ca_ci_bus_cken  : 1; //[1]
        HI_U32 ca_ci_cken      : 1; //[2]
        HI_U32 otp_bus_cken    : 1; //[3]
        HI_U32 reserved0       : 4; //[7:4]
        HI_U32 ca_kl_srst_req  : 1; //[8]
        HI_U32 ca_ci_srst_req  : 1; //[9]
        HI_U32 otp_srst_req    : 1; //[10]
        HI_U32 reserved        : 1; //[11]
        HI_U32 ca_clk_sel      : 1; //[12]
        HI_U32 reserved1       : 19; //[31:13]
    }bits;
    HI_U32 u32;
}OTP_V200_CRG_CTRL_U;        //PERI_CRG48
#else
typedef union
{
    struct
    {
        HI_U32 reserved0        : 8; //[7:0]
        HI_U32 ca_kl_srst_req   : 1; //[8]
        HI_U32 ca_ci_srst_req   : 1; //[9]
        HI_U32 otp_srst_req     : 1; //[10]
        HI_U32 reserved1        : 1; //[11]
        HI_U32 ca_ci_clk_sel    : 1; //[12]
        HI_U32 reserved2        : 19;//[31:13]
    }bits;
    HI_U32 u32;
}OTP_V200_CRG_CTRL_U;        //PERI_CRG48
#endif

typedef enum
{
    OTP_KEY_LEV1 = 0,
    OTP_KEY_LEV2,
    OTP_KEY_LEV3,
    OTP_KEY_LEV_BUTT
}OTP_KEY_LEVEL_E;

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
        HI_U32 tskl_nv_disable_lock        : 1;//0x0a[3]  //as samle as tskl_sm4_disable_lock for Hi3716mv310
        HI_U32 tskl_csa3_disable_lock      : 1;//0x0a[4]
        HI_U32 tskl_others_disable_lock    : 1;//0x0a[5]
        HI_U32 mc_aes_hardonly_en_lock     : 1;//0x0a[6]
        HI_U32 mc_tdes_hardonly_en_lock    : 1;//0x0a[7]
        HI_U32 reserved3                   : 4;//0x0b[0]~0x0b[3]
        HI_U32 tskl_tdes_disable_lock      : 1;//0x0b[4]
        HI_U32 bload_mode_sel_lock         : 1;//0x0b[5]
        HI_U32 ddr_scramble_en_lock        : 1;//0x0b[6]
        HI_U32 otp_globle_lock_en_lock     : 1;//0x0b[7]
    } bits;
    HI_U32 u32;
}OTP_INTERNAL_PVLOCK_0_U;

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
        HI_U32 tskl_nv_disable      : 1;//0x02[3]
        HI_U32 tskl_csa3_disable    : 1;//0x02[4]
        HI_U32 tskl_others_disable  : 1;//0x02[5]
        HI_U32 mc_aes_hardonly_en   : 1;//0x02[6]
        HI_U32 mc_tdes_hardonly_en  : 1;//0x02[7]
        HI_U32 mc_des_hardonly_en   : 1;//0x03[0]
        HI_U32 mc_rc4_hardonly_en   : 1;//0x03[1]
        HI_U32 mckl_des_hard_disable: 1;//0x03[2]
        HI_U32 mckl_rc4_disable     : 1;//0x03[3]
        HI_U32 tskl_tdes_disable    : 1;//0x03[4]  DEMUX TDES engine control.
        HI_U32 bload_mode_sel       : 1;//0x03[5]
        HI_U32 ddr_scramble_en      : 1;//0x03[6]
        HI_U32 otp_global_lock_en   : 1;//0x03[7]
    } bits;
    HI_U32 u32;
} OTP_INTERNAL_PV_0_U;

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
        HI_U32 uart_disable          : 1;//0x05[2]
        HI_U32 pcie_disable          : 1;//0x05[3]
        HI_U32 usb_disable           : 1;//0x05[4]
        HI_U32 sata_disable          : 1;//0x05[5]
        HI_U32 gmac_disable          : 1;//0x05[6]
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
} OTP_INTERNAL_PV_1_U;

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
        HI_U32 reserved                    : 5;//0x0d[2]~0x0d[6]
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
}OTP_INTERNAL_PVLOCK_1_U;

typedef union
{
    struct
    {
        HI_U32 secret_key_lock          : 1;//  0x10[0] ONLY FOR CSA2 ROOT KEY LOCK
        HI_U32 ca_chip_id_lock          : 1;//  0x10[1]
        HI_U32 esck_lock                : 1;//  0x10[2]
        HI_U32 stb_rootkey_lock         : 1;//  0x10[3]
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
        HI_U32 rsv_data_1_lock          : 1;//  0x12[2]
        HI_U32 rsv_data_2_lock          : 1;//  0x12[3]
        HI_U32 hdcp_lock                : 1;//  0x12[4]
        HI_U32 rsv_hdcp_lock            : 1;//  0x12[5]
        HI_U32 rsv_data_3to10_lock      :8;//   0x12[6]~0x13[5]
        HI_U32 rsv_rsa_0_lock           :1;//       0x13[6]
        HI_U32 rsv_rsa_1_lock           :1;//       0x13[7]
    } bits;
    HI_U32 u32;
}OTP_INTERNAL_DATALOCK_0_U;

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
} OTP_ONE_WAY_0_U;//Offset:0x18

/*====================define for CA and mailbox===============================*/

/* CA_MAILBOX */
#define HI_ERR_CA_COMMON_INVALID_PARA   (HI_S32)(0x804E0005)
#define otp_read_reg(addr)              (*(volatile HI_U32 *)(addr))
#define otp_write_reg(addr, val)        ((*(volatile HI_U32 *)(addr)) = (val))

HI_VOID OTP_Init(void);
HI_VOID OTP_DeInit(void);
HI_U32 OTP_Read(HI_U32 addr);
HI_S32 OTP_Write(HI_U32 addr, HI_U32 u32Data);
HI_U8  OTP_ReadByte(HI_U32 addr);
HI_S32 OTP_SetByte(HI_U32 addr, HI_U8 tdata);
HI_S32 OTP_V200_getCustomerKey(OTP_CUSTOMER_KEY_S * pCustomerKey);
HI_S32 OTP_GetMarketID(HI_U8 u8Id[4]);
HI_S32 OTP_GetR2RLadderLevel(OTP_KEY_LEVEL_E *pLevel);
HI_S32 OTP_V200_Set_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData);
HI_S32 OTP_V200_Get_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData);



#endif  /* __OTP_H__ */
