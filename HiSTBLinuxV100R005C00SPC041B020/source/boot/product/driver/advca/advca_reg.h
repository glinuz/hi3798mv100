/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : advca_reg.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef __ADVCA_REG_H__
#define __ADVCA_REG_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*the follows definens from ca_reg_v300.h*/
#if defined (CHIP_TYPE_hi3716mv310)
#define CA_V300_BASE_ADDR 0x10000000
#else
#define CA_V300_BASE_ADDR 0xF8A90000
#endif

#define CA_V300_CONFIG_STATE           (CA_V300_BASE_ADDR + 0x00)//ÅäÖÃ×´Ì¬¼Ä´æÆ÷
#define CA_V300_CSA2_CTRL              (CA_V300_BASE_ADDR + 0x04)//CSA¿ØÖÆ¼Ä´æÆ÷
#define CA_V300_R2R_CTRL               (CA_V300_BASE_ADDR + 0x08)//R2R¿ØÖÆ¼Ä´æÆ÷
#define CA_V300_SP_CTRL                (CA_V300_BASE_ADDR + 0x0C)//SP¿ØÖÆ¼Ä´æÆ÷
#define CA_V300_CSA3_CTRL              (CA_V300_BASE_ADDR + 0x10)//CSA¿ØÖÆ¼Ä´æÆ÷
#define CA_V300_LP_CTRL                (CA_V300_BASE_ADDR + 0x14)//LP¿ØÖÆ¼Ä´æÆ÷¡£
#define CA_V300_BL_CTRL_DEC            (CA_V300_BASE_ADDR + 0x18)//BootLoader½âÃÜ¿ØÖÆ¼Ä´æÆ÷
#define CA_V300_BL_CTRL_ENC            (CA_V300_BASE_ADDR + 0x1C)//BootLoader¼ÓÃÜ¿ØÖÆ¼Ä´æÆ÷
#define CA_V300_CA_DIN0                (CA_V300_BASE_ADDR + 0x20)//Key LadderÊäÈë´ý½âÃÜÊý¾Ý0¼Ä´æÆ÷
#define CA_V300_CA_DIN1                (CA_V300_BASE_ADDR + 0x24)//Key LadderÊäÈë´ý½âÃÜÊý¾Ý1¼Ä´æÆ÷
#define CA_V300_CA_DIN2                (CA_V300_BASE_ADDR + 0x28)//Key LadderÊäÈë´ý½âÃÜÊý¾Ý2¼Ä´æÆ÷
#define CA_V300_CA_DIN3                (CA_V300_BASE_ADDR + 0x2C)//Key LadderÊäÈë´ý½âÃÜÊý¾Ý3¼Ä´æÆ÷
#define CA_V300_TEST_CRTL              (CA_V300_BASE_ADDR + 0x30)//²âÊÔ¿ØÖÆ¼Ä´æÆ÷
//#define CA_V300_STB_KEY_CTRL           (CA_V300_BASE_ADDR + 0x34)//STB KEY¿ØÖÆ¼Ä´æÆ÷
#define CA_V300_CA_STATE               (CA_V300_BASE_ADDR + 0x38)//CAÄ£¿é×´Ì¬¼Ä´æÆ÷
#define CA_V300_LEVEL_REG              (CA_V300_BASE_ADDR + 0x3C)//keyladder ¼¶Êý¼Ä´æÆ÷
#define CA_V300_CHECKSUM_FLAG          (CA_V300_BASE_ADDR + 0x40)//secret keyµÄchecksum±êÖ¾Î»
#define CA_V300_CA_VERSION             (CA_V300_BASE_ADDR + 0x44)//Key LadderÄ£¿éÖÐ¶ÏÊ¹ÄÜ¼Ä´æÆ÷
#define CA_V300_CA_INT_RAW             (CA_V300_BASE_ADDR + 0x48)//Key LadderÄ£¿éÔ­Ê¼ÖÐ¶Ï×´Ì¬¼Ä´æÆ÷
#define CA_V300_SECURE_BOOT_STATE      (CA_V300_BASE_ADDR + 0x4C)//°²È«Æô¶¯×´Ì¬¼Ä´æÆ÷
#define CA_V300_LP_PARAMETER_BASE      (CA_V300_BASE_ADDR + 0x50)//parameter¼Ä´æÆ÷
#define CA_V300_BLK_ENC_RSLT           (CA_V300_BASE_ADDR + 0x60)//BLK¼ÓÃÜ½á¹û¼Ä´æÆ÷
#define CA_V300_GDRM_CTRL              (CA_V300_BASE_ADDR + 0x70)//GOOGLE DRM¿ØÖÆ¼Ä´æÆ÷
#define CA_V300_DCAS_CTRL              (CA_V300_BASE_ADDR + 0x74)//¿ÉÏÂÔØCA¿ØÖÆ¼Ä´æÆ÷
#define CA_V300_DEBUG_INFO             (CA_V300_BASE_ADDR + 0x78)//debugÐÅÏ¢¼Ä´æÆ÷
#define CA_V300_VERSION                (CA_V300_BASE_ADDR + 0x7C)//°æ±¾¼Ä´æÆ÷
#define CA_V300_DA_NOUCE               (CA_V300_BASE_ADDR + 0x80)//½âÃÜµÄÈÏÖ¤ÃÜÔ¿¼Ä´æÆ÷
#define CA_V300_TEST_KEY               (CA_V300_BASE_ADDR + 0x90)//²âÊÔÃÜÔ¿¼Ä´æÆ÷
#define CA_V300_TEST_RESULT            (CA_V300_BASE_ADDR + 0xA0)//²âÊÔ½á¹û¼Ä´æÆ÷
#define CA_V300_KEY_DOUT               (CA_V300_BASE_ADDR + 0xB0)//ÖÐ¼äÔËËã½á¹û¼Ä´æÆ÷
#define CA_V300_MISC_CTRL              (CA_V300_BASE_ADDR + 0xC0)//misc¿ØÖÆ¼Ä´æÆ÷
#define CA_V300_CAUK_CTRL               (CA_V300_BASE_ADDR + 0x110)//irdeto MSR2.2 CAUK¿ØÖÆ¼Ä´æÆ÷

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
        HI_U32 reserved0      : 5; //[7:3]
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
        HI_U32 reserved0      : 5; //[7:3]
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
        HI_U32 key_addr       : 8; //[7:0]
        HI_U32 reserved       : 24; //[31:8]
    }bits;
    HI_U32 u32;
}CA_V300_CAUK_CTRL_U; //only used for Irdeto MSR2.2, high level code authentication

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif