/******************************************************************************
 *
 * Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
 * ******************************************************************************
 * File Name     : tee_drv_otp.h
 * Version       : Initial
 * Author        : Hisilicon hisecurity team
 * Created       : 2013-12-28
 * Last Modified :
 * Description   : 
 * Function List :
 * History       :
 * ******************************************************************************/
#ifndef __TEE_DRV_OTP_H__
#define __TEE_DRV_OTP_H__

#include "hi_type.h"

#define OTP_CUSTOMER_KEY_LEN        (16)
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

#define OTP_V200_PVREGISTER0        (OTP_V200_BASE_OFFSET + 0x80)

#define OTP_V200_CHIPID			    0x68
#define OTP_V200_DIEID              0xF0

#define OTP_V200_INTERNAL_WRITE_STATUS_ADDR    (0x17)
#define OTP_V200_INTERNAL_WRITE_ERROR_VALUE    (0x80)

#define OTP_V200_TZ_AREA_BASE                  (0x140)
#define OTP_V200_TZ_AREA_LOCK_BASE             (0x1d)

#define OTP_V200_OTP_RESET_REG                 (0xF8A220C0)
#if 0
#define HI_ERR_OTP(format, arg...)
#define HI_DBG_OTP(format, arg...)
#define HI_INFO_OTP(format, arg...)
#else
#define HI_ERR_OTP(format, arg...)  uart_printf_func("<<<OTP ERR>>> %s %d: " format, __FUNCTION__, __LINE__, ## arg)
#define HI_DBG_OTP(format, arg...)  uart_printf_func("<<<OTP DBG>>> %s %d: " format, __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_OTP(format, arg...)  uart_printf_func("<<<OTP INFO>>> %s %d: " format, __FUNCTION__, __LINE__, ## arg)
#endif

typedef struct hiOTP_CUSTOMER_KEY_S
{
    HI_U32 u32CustomerKey[4];
}OTP_CUSTOMER_KEY_S;

typedef struct hiOTP_STB_PRIV_DATA_S
{
    HI_U32 u32Offset;
    HI_U8 u8Data;
}OTP_STB_PRIV_DATA_S;

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
        HI_U32 waddr              : 32; // [10-31]
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

typedef union
{
    struct
    {
        HI_U32 reserved0  		: 8; //[7:0]
        HI_U32 ca_kl_srst_req	: 1; //[8]
        HI_U32 ca_ci_srst_req   : 1; //[9]
        HI_U32 otp_srst_req		: 1; //[10]
        HI_U32 reserved1		: 1; //[11]
        HI_U32 ca_ci_clk_sel	: 1; //[12]
        HI_U32 reserved2		: 19;//[31:13]
    }bits;
    HI_U32 u32;
}OTP_V200_CRG_CTRL_U;        /* PERI_CRG48 */

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




#define  HI_ERR_CA_NOT_INIT                         (HI_S32)(0x804E0003)
#define  HI_ERR_CA_INVALID_PARA                     (HI_S32)(0x804E0005)

#define otp_read_reg(addr)          (*(volatile HI_U32 *)(addr))
#define otp_write_reg(addr, val)    ((*(volatile HI_U32 *)(addr)) = (val))

HI_S32 OTP_Init(HI_VOID);
HI_S32 OTP_DeInit(HI_VOID);
HI_VOID OTP_SetReset(HI_VOID);
HI_S32 OTP_GetChipID(HI_U8 *pID);
HI_S32 OTP_ReadWord(HI_U32 Addr, HI_U32 *pu32Value);
HI_S32 OTP_WriteWord(HI_U32 Addr, HI_U32 u32Value);
HI_S32 OTP_ReadByte(HI_U32 Addr, HI_U8 *pu8Value);
HI_S32 OTP_WriteByte(HI_U32 Addr, HI_U8 u8Value);

#endif  /* __TEE_DRV_OTP_H__ */

