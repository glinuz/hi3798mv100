/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_advca_v300_reg.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 2013/08/28
  Last Modified :
  Description   : ADVCA REG DEFINE
  Function List :
  History       :

******************************************************************************/
#ifndef __DRV_ADVCA_V300_REG_H__
#define __DRV_ADVCA_V300_REG_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define REG_SYS_GEN24       (0xF80000E0)
#define REG_SYS_BRM_DEBUG   (0xF8A90180)

#ifdef CHIP_TYPE_hi3716mv300
#define CA_V300_BASE_ADDR 0x10000000
#define OTP_V200_CRG_ADDR 0x101F5048
#else
#define CA_V300_BASE_ADDR 0xF8A90000
#define OTP_V200_CRG_ADDR 0xF8A220C0
#endif

#define CA_V300_CONFIG_STATE           (CA_V300_BASE_ADDR + 0x00)//配置状态寄存器
#define CA_V300_CSA2_CTRL              (CA_V300_BASE_ADDR + 0x04)//CSA控制寄存器
#define CA_V300_R2R_CTRL               (CA_V300_BASE_ADDR + 0x08)//R2R控制寄存器
#define CA_V300_SP_CTRL                (CA_V300_BASE_ADDR + 0x0C)//SP控制寄存器
#define CA_V300_CSA3_CTRL              (CA_V300_BASE_ADDR + 0x10)//CSA控制寄存器
#define CA_V300_LP_CTRL                (CA_V300_BASE_ADDR + 0x14)//LP控制寄存器。
#define CA_V300_BL_CTRL_DEC            (CA_V300_BASE_ADDR + 0x18)//BootLoader解密控制寄存器
#define CA_V300_BL_CTRL_ENC            (CA_V300_BASE_ADDR + 0x1C)//BootLoader加密控制寄存器

#define CA_V300_CA_NOMAL_DIN0          (CA_V300_BASE_ADDR + 0x20)//Key Ladder输入待解密数据0寄存器
#define CA_V300_CA_NOMAL_DIN1          (CA_V300_BASE_ADDR + 0x24)//Key Ladder输入待解密数据1寄存器
#define CA_V300_CA_NOMAL_DIN2          (CA_V300_BASE_ADDR + 0x28)//Key Ladder输入待解密数据2寄存器
#define CA_V300_CA_NOMAL_DIN3          (CA_V300_BASE_ADDR + 0x2C)//Key Ladder输入待解密数据3寄存器

#define CA_V300_CA_SECURE_DIN0         (CA_V300_BASE_ADDR + 0x1A0)//secure Key Ladder输入待解密数据0寄存器
#define CA_V300_CA_SECURE_DIN1         (CA_V300_BASE_ADDR + 0x1A4)//secure Key Ladder输入待解密数据1寄存器
#define CA_V300_CA_SECURE_DIN2         (CA_V300_BASE_ADDR + 0x1A8)//secure Key Ladder输入待解密数据2寄存器
#define CA_V300_CA_SECURE_DIN3         (CA_V300_BASE_ADDR + 0x1AC)//secure Key Ladder输入待解密数据3寄存器


#if   (defined(CHIP_TYPE_hi3798cv200) && !defined(HI_TEE_SUPPORT))  \
   || (defined(CHIP_TYPE_hi3798mv200) && !defined(HI_TEE_SUPPORT))  \
   || (defined(CHIP_TYPE_hi3798mv200_a) && !defined(HI_TEE_SUPPORT)) 
#define CA_V300_CA_DIN0                (CA_V300_CA_SECURE_DIN0)
#define CA_V300_CA_DIN1                (CA_V300_CA_SECURE_DIN1)
#define CA_V300_CA_DIN2                (CA_V300_CA_SECURE_DIN2)
#define CA_V300_CA_DIN3                (CA_V300_CA_SECURE_DIN3)
#else
#define CA_V300_CA_DIN0                (CA_V300_CA_NOMAL_DIN0)
#define CA_V300_CA_DIN1                (CA_V300_CA_NOMAL_DIN1)
#define CA_V300_CA_DIN2                (CA_V300_CA_NOMAL_DIN2)
#define CA_V300_CA_DIN3                (CA_V300_CA_NOMAL_DIN3)
#endif
#define CA_V300_STB_KEY_CTRL           (CA_V300_BASE_ADDR + 0x34)//STB KEY控制寄存器
#define CA_V300_CA_STATE               (CA_V300_BASE_ADDR + 0x38)//CA模块状态寄存器
#define CA_V300_LEVEL_REG              (CA_V300_BASE_ADDR + 0x3C)//keyladder 级数寄存器
#define CA_V300_CHECKSUM_FLAG          (CA_V300_BASE_ADDR + 0x40)//secret key的checksum标志位
#define CA_V300_CA_VERSION             (CA_V300_BASE_ADDR + 0x44)//Key Ladder模块版本号寄存器
#define CA_V300_CA_INT_RAW             (CA_V300_BASE_ADDR + 0x48)//Key Ladder模块原始中断状态寄存器
#define CA_V300_SECURE_BOOT_STATE      (CA_V300_BASE_ADDR + 0x4C)//安全启动状态寄存器
#define CA_V300_LP_PARAMETER_BASE      (CA_V300_BASE_ADDR + 0x50)//parameter寄存器
#define CA_V300_BLK_ENC_RSLT           (CA_V300_BASE_ADDR + 0x60)//BLK加密结果寄存器
#define CA_V300_GDRM_CTRL              (CA_V300_BASE_ADDR + 0x70)//GOOGLE DRM控制寄存器
#define CA_V300_DCAS_CTRL              (CA_V300_BASE_ADDR + 0x74)//可下载CA控制寄存器
#define CA_V300_DEBUG_INFO             (CA_V300_BASE_ADDR + 0x78)//debug信息寄存器
#define CA_V300_VERSION                (CA_V300_BASE_ADDR + 0x7C)//版本寄存器
#define CA_V300_DA_NOUCE               (CA_V300_BASE_ADDR + 0x80)//解密的认证密钥寄存器
#define CA_V300_TEST_KEY               (CA_V300_BASE_ADDR + 0x90)//测试密钥寄存器
#define CA_V300_TEST_RESULT            (CA_V300_BASE_ADDR + 0xA0)//测试结果寄存器
#define CA_V300_KEY_DOUT               (CA_V300_BASE_ADDR + 0xB0)//中间运算结果寄存器
#define CA_V300_MISC_CTRL              (CA_V300_BASE_ADDR + 0xC0)//misc控制寄存器
#define CA_V300_GDRM_FLAG              (CA_V300_BASE_ADDR + 0xc4)//Google DRM flag 寄存器
#define CA_V300_CFG_CMAC_CTRL          (CA_V300_BASE_ADDR + 0xCC)//irdeto MSR2.2 OTP Configuration CMAC control
#define CA_V300_GDRM_ENC_REST0         (CA_V300_BASE_ADDR + 0xD0)//Google DRM的device key的加密结果寄存器0。
#define CA_V300_GDRM_ENC_REST1         (CA_V300_BASE_ADDR + 0xD4)//Google DRM的device key的加密结果寄存器1。
#define CA_V300_GDRM_ENC_REST2         (CA_V300_BASE_ADDR + 0xD8)//Google DRM的device key的加密结果寄存器2。
#define CA_V300_GDRM_ENC_REST3         (CA_V300_BASE_ADDR + 0xDC)//Google DRM的device key的加密结果寄存器3。
#define CA_V300_CFG_CMAC_OUT           (CA_V300_BASE_ADDR + 0xF0)//irdeto MSR2.2 OTP Configuration CMAC output
#define CA_V300_TA_KL_CTRL             (CA_V300_BASE_ADDR + 0x100)//irdeto MSR2.2 TA keyladder control register
#define CA_V300_IVRK_CTRL              (CA_V300_BASE_ADDR + 0x10C)//irdeto MSR2.2 tweaked CSA3 IV/RK control register
#define CA_V300_CAUK_CTRL              (CA_V300_BASE_ADDR + 0x110)//irdeto MSR2.2 CAUK控制寄存器
#define CA_V300_KL_SEC_EN              (CA_V300_BASE_ADDR + 0x200) // secure keyladder enable
#define CA_V300_KL_EN_MASK             (0x00007FFF) // all keyladder set to secure keyladder

#define DVB_SYMMETRIC_KEY_ERROR         (6)
#define R2R_SP_SYMMETRIC_KEY_ERROR      (4)
#define MISC_KL_LEVEL_ERROR             (2)
#define DCAS_KL_DISABLE_ERROR           (1)

typedef union
{
    struct
    {
        HI_U32 st_vld       : 1;  //[0]
        HI_U32 reserved     : 31; //[31:1]
    }bits;
    HI_U32 u32;
}CA_V300_CONFIG_STATE_U;


typedef union
{
    struct
    {
        HI_U32 level_sel      : 2; //[1:0]
        HI_U32 tdes_aes_sel   : 1; //[2]
        HI_U32 ta_kl_flag     : 1; //[3] only use for Irdeto MSR2.2
        HI_U32 reserved0      : 4; //[7:4]
        HI_U32 EvenOrOdd      : 1; //[8]
        HI_U32 key_addr       : 7; //[15:9]
        HI_U32 dsc_mode       : 8; //[23:16]
        HI_U32 reserved       : 8; //[24:31]
    }bits;
    HI_U32 u32;
}CA_V300_CSA2_CTRL_U;

typedef union
{
    struct
    {
        HI_U32 level_sel      : 2; //[1:0]
        HI_U32 tdes_aes_sel   : 1; //[2]
        HI_U32 reserved0      : 5; //[7:3]
        HI_U32 key_addr       : 8; //[15:8]
        HI_U32 mc_alg_sel     : 8; //[23:16]
        HI_U32 reserved       : 8; //[31:24]
    }bits;
    HI_U32 u32;
}CA_V300_R2R_CTRL_U;

typedef union
{
    struct
    {
        HI_U32 level_sel      : 2; //[1:0]
        HI_U32 tdes_aes_sel   : 1; //[2]
        HI_U32 raw_mode       : 1; //[3]
        HI_U32 level_sel_5    : 1; //[4]
        HI_U32 ta_kl_flag     : 1; //[5] //for irdeto MSR2.2
        HI_U32 reserved1      : 2; //[7:6]
        HI_U32 key_addr       : 8; //[15:8]
        HI_U32 dsc_mode       : 8; //[23:16]
        HI_U32 reserved       : 8; //[31:24]
    }bits;
    HI_U32 u32;
}CA_V300_SP_CTRL_U;

typedef union
{
    struct
    {
        HI_U32 level_sel      : 2; //[1:0]
        HI_U32 tdes_aes_sel   : 1; //[2]
        HI_U32 ta_kl_flag     : 1; //[3] only use for Irdeto MSR2.2
        HI_U32 reserved0      : 4; //[7:4]
        HI_U32 EvenOrOdd      : 1; //[8]
        HI_U32 key_addr       : 7; //[15:9]
        HI_U32 dsc_mode       : 8; //[23:16]
        HI_U32 reserved       : 8; //[31:24]
    }bits;
    HI_U32 u32;
}CA_V300_CSA3_CTRL_U;

typedef union
{
    struct
    {
        HI_U32 level_sel      : 2; //[1:0]
        HI_U32 reserved0      : 6; //[7:2]
        HI_U32 reserved       : 24; //[31:8]
    }bits;
    HI_U32 u32;
}CA_V300_LP_CTRL_U;

typedef union
{
    struct
    {
        HI_U32 reserved0      : 8; //[7:0]
        HI_U32 key_addr       : 8; //[15:8]
        HI_U32 reserved       : 16; //[31:16]
    }bits;
    HI_U32 u32;
}CA_V300_BL_CTRL_DEC_U;

typedef union
{
    struct
    {
        HI_U32 alg_sel        : 1; //[0]
        HI_U32 decrypt        : 1; //[1]
        HI_U32 reserved       : 30; //[31:2]
    }bits;
    HI_U32 u32;
}CA_V300_TEST_CRTL_U;


typedef union
{
    struct
    {
        HI_U32 err_state           : 4; //[3:0]
        HI_U32 last_key_not_rdy    : 1; //[4]
        HI_U32 csa2_klad0_rdy      : 1; //[5]
        HI_U32 csa2_klad1_rdy      : 1; //[6]
        HI_U32 csa2_klad2_rdy      : 1; //[7]
        HI_U32 r2r_klad0_rdy       : 1; //[8]
        HI_U32 r2r_klad1_rdy       : 1; //[9]
        HI_U32 r2r_klad2_rdy       : 1; //[10]
        HI_U32 sp_klad0_rdy        : 1; //[11]
        HI_U32 sp_klad1_rdy        : 1; //[12]
        HI_U32 sp_klad2_rdy        : 1; //[13]
        HI_U32 csa3_klad0_rdy      : 1; //[14]
        HI_U32 csa3_klad1_rdy      : 1; //[15]
        HI_U32 csa3_klad2_rdy      : 1; //[16]
        HI_U32 misc_klad0_rdy      : 1; //[17]
        HI_U32 misc_klad1_rdy      : 1; //[18]
        HI_U32 misc_klad2_rdy      : 1; //[19]
        HI_U32 lp_klad0_rdy        : 1; //[20]
        HI_U32 lp_klad1_rdy        : 1; //[21]
        HI_U32 k3_rdy              : 1; //[22]
        HI_U32 k2_rdy              : 1; //[23]
        HI_U32 k1_rdy              : 1; //[24]
        HI_U32 reserved            : 6; //[25:30]
        HI_U32 klad_busy           : 1; //[31]
    }bits;
    HI_U32 u32;
}CA_V300_CA_STATE_U;

typedef union
{
    struct
    {
        HI_U32 csa2_lv      : 1; //[0]
        HI_U32 r2r_lv       : 1; //[1]
        HI_U32 sp_lv        : 1; //[2]
        HI_U32 csa3_lv      : 1; //[3]
        HI_U32 reserved     : 28; //[31:4]
    }bits;
    HI_U32 u32;
}CA_V300_LEVEL_REG_U;

typedef union
{
    struct
    {
        HI_U32 ca_finish_int  : 1; //[0]
        HI_U32 reserved_0     : 1; //[1]
        HI_U32 wdg_over_int   : 1; //[2]
        HI_U32 reserved       : 29; //[31:3]
    }bits;
    HI_U32 u32;
}CA_V300_CA_INT_STATE_U;

typedef union
{
    struct
    {
        HI_U32 ca_finish_en   : 1; //[0]
        HI_U32 reserved_0     : 1; //[1]
        HI_U32 wdg_over_en    : 1; //[2]
        HI_U32 reserved_1     : 4; //[6:3]
        HI_U32 ca_int_en      : 1; //[7]
        HI_U32 reserved       : 24; //[31:8]
    }bits;
    HI_U32 u32;
}CA_V300_CA_INT_EN_U;

typedef union
{
    struct
    {
        HI_U32 ca_finish_raw  : 1; //[0]
        HI_U32 reserved_0     : 1; //[1]
        HI_U32 wdg_over_raw   : 1; //[2]
        HI_U32 reserved       : 29; //[31:3]
    }bits;
    HI_U32 u32;
}CA_V300_CA_INT_RAW_U;

typedef union
{
    struct
    {
        HI_U32 bootrom_start  : 1; //[0]
        HI_U32 goin_normal    : 1; //[1]
        HI_U32 wrf_end        : 1; //[2]
        HI_U32 wrf_result     : 1; //[3]
        HI_U32 reserved       : 28; //[31:4]
    }bits;
    HI_U32 u32;
}CA_V300_SECURE_BOOT_STATE_U;

typedef union
{
    struct
    {
        HI_U32 key_addr              : 8; //[0:7]
        HI_U32 reserved              : 24;//[8:31]
    }bits;
    HI_U32 u32;
}CA_V300_STB_CTRL_U;

typedef union
{
    struct
    {
        HI_U32 level_sel               : 4; //[3:0]
        HI_U32 tdes_aes_sel            : 1; //[4]
        HI_U32 reserved_0              : 11; //[15:5]
        HI_U32 dsc_code_mc_alg_sel     : 8; //[23:16]
        HI_U32 even_or_odd             : 1; //[24]
        HI_U32 key_addr                : 7; //[31:25]
    }bits;
    HI_U32 u32;
}CA_V300_DCAS_CTRL_U;

typedef union
{
    struct
    {
        HI_U32 work_flag      : 2; //[1:0]
        HI_U32 jtag_cur_st    : 4; //[5:2]
        HI_U32 jtag_if_state  : 2; //[7:6]
        HI_U32 key_addr       : 24; //[31:8]
    }bits;
    HI_U32 u32;
}CA_V300_DEBUG_INFO_U;

typedef union
{
    struct
    {
        HI_U32 level_sel    : 2; //[1:0]
        HI_U32 target_sel   : 1; //[2]
        HI_U32 decryption   : 1; //[3]
        HI_U32 reserved0    : 4; //[7:4]
        HI_U32 even_or_odd  : 1; //[8]
        HI_U32 key_addr     : 7; //[15:9]
        HI_U32 reserved1    : 16;//[31:16]
    }bits;
    HI_U32 u32;
}CA_V300_GDRM_CTRL_U;

typedef union
{
    struct
    {
        HI_U32 level_sel      : 2; //[1:0]
        HI_U32 tdes_aes_sel   : 1; //[2]
        HI_U32 target_sel     : 1; //[3]
        HI_U32 level_sel_5    : 1; //[4]
        HI_U32 reserved_0     : 3; //[7:5]
        HI_U32 key_addr       : 8; //[15:8]
        HI_U32 dsc_mode       : 8; //[23:16]
        HI_U32 reserved       : 8; //[31:24]
    }bits;
    HI_U32 u32;
}CA_V300_MISC_CTRL_U;


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

typedef union CA_V300_JTAGKEY0_U
{
    struct
    {
    	HI_U32  data00: 1;
        HI_U32  data01: 1;
        HI_U32  data02: 1;
        HI_U32  data03: 1;
        HI_U32  data04: 1;
    	HI_U32  data05: 1;
        HI_U32  data06: 1;
        HI_U32  data07: 1;
        HI_U32  data08: 1;
        HI_U32  data09: 1;
    	HI_U32  data10: 1;
        HI_U32  data11: 1;
        HI_U32  data12: 1;
        HI_U32  data13: 1;
        HI_U32  data14: 1;
    	HI_U32  data15: 1;
        HI_U32  data16: 1;
        HI_U32  data17: 1;
        HI_U32  data18: 1;
        HI_U32  data19: 1;
    	HI_U32  data20: 1;
        HI_U32  data21: 1;
        HI_U32  data22: 1;
        HI_U32  data23: 1;
        HI_U32  data24: 1;
    	HI_U32  data25: 1;
        HI_U32  data26: 1;
        HI_U32  data27: 1;
        HI_U32  data28: 1;
        HI_U32  data29: 1;
    	HI_U32  data30: 1;
        HI_U32  data31: 1;
    }bits;
    HI_U32 u32;
}CA_V300_JTAGKEY0;

typedef union CA_V300_JTAGKEY1_U
{
    struct
    {
    	HI_U32  data00: 8;
        HI_U32  data01: 8;
        HI_U32  data02: 8;
        HI_U32  data03: 8;
    }bits;
    HI_U32 u32;
}CA_V300_JTAGKEY1;

typedef union CA_V300_CFG_CBC_MAC_U
{
    struct
    {
    	HI_U32  start      : 8; //[7:0] //写启动计算
        HI_U32  reserved   : 24;
    }bits;
    HI_U32 u32;
}CA_V300_CFG_CBC_MAC_U;

typedef union
{
    struct
    {
        HI_U32 level_sel       : 2; //[1:0]
        HI_U32 cur_128bit_cnt  : 6; //[7:2]
        HI_U32 last_time       : 1; //[8]
        HI_U32 lut_alg_sel     : 1; //[9]
        HI_U32 dec_lut_alg_sel : 1; //[10]
        HI_U32 reserved        : 21; //[31:11]
    }bits;
    HI_U32 u32;
}CA_V300_TA_CTRL_U; //only for Irdeto MSR2.2 TA keyladder

typedef union CA_V300_CFG_IVRK_CTRL_U
{
    struct
    {
    	HI_U32  alg_sel        : 1; //[0] //Algorithm selection
        HI_U32  reserved       : 21; //[1:31]
    }bits;
    HI_U32 u32;
}CA_V300_CFG_IVRK_CTRL_U; //only for Irdeto MSR2.2 tweaked CSA3

typedef union
{
    struct
    {
        HI_U32 key_addr        : 8; //[7:0]
        HI_U32 reserved        : 24; //[31:8]
    }bits;
    HI_U32 u32;
}CA_V300_CAUK_CTRL_U; //only used for Irdeto MSR2.2, high level code authentication

typedef union CA_V300_CFG_HL_APP_CBC_CTRL_U
{
    struct
    {
    	HI_U32  irdeto_verify_flag    : 1; //[0]
    	HI_U32 irdeto_verify_err      : 1; //[1]
    	HI_U32 irdeto_verify_finish   : 1; //[2]
        HI_U32  reserved              : 21; //[3:31]
    }bits;
    HI_U32 u32;
}CA_V300_CFG_HL_APP_CBC_CTRL_U; //only for Irdeto MSR2.2 high level code authentication

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif	/* __DRV_ADVCA_V300_REG_H__ */
