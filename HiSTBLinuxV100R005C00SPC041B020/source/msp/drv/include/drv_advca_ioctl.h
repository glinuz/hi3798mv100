/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_advca_ioctl.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/
#ifndef __DRV_ADVCA_IOCTL_H__
#define __DRV_ADVCA_IOCTL_H__

#include "drv_advca_ext.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

typedef enum
{
    CMD_CHILD_ID_GET_VENDOR_ID,
    CMD_CHILD_ID_SET_SOC_OTP_TZ_EN,
    CMD_CHILD_ID_GET_SOC_OTP_TZ_EN,
    CMD_CHILD_ID_SET_KL_DPA_CLK_SEL_EN,
    CMD_CHILD_ID_GET_KL_DPA_CLK_SEL_EN,
    CMD_CHILD_ID_SET_KL_DPA_FILTER_CLK_EN,
    CMD_CHILD_ID_GET_KL_DPA_FILTER_CLK_EN,
    CMD_CHILD_ID_SET_MC_DPA_CLK_SEL_EN,
    CMD_CHILD_ID_GET_MC_DPA_CLK_SEL_EN,
    CMD_CHILD_ID_SET_PVR_DPA_FILTER_CLK_EN,
    CMD_CHILD_ID_GET_PVR_DPA_FILTER_CLK_EN,
    CMD_CHILD_ID_SET_MISC_LV_SEL,
    CMD_CHILD_ID_GET_MISC_LV_SEL,
    CMD_CHILD_ID_SET_OEM_ROOT_KEY,
    CMD_CHILD_ID_GET_OEM_ROOT_KEY,
    CMD_CHILD_ID_GET_OEM_ROOT_KEY_LOCK,
    CMD_CHILD_ID_SET_OTP_TZ,
    CMD_CHILD_ID_GET_OTP_TZ,
    CMD_CHILD_ID_LOCK_OTP_TZ,
    CMD_CHILD_ID_CA_GDRM_CRYPT,
    CMD_CHILD_ID_GET_CHECKSUM_FLAG,
    CMD_CHILD_ID_GET_CASTATE,
    CMD_CHILD_ID_MISC_SETALG,
    CMD_CHILD_ID_MISC_GETALG,
    CMD_CHILD_ID_SET_MISC_ROOT_KEY,
    CMD_CHILD_ID_GET_MISC_ROOT_KEY,
    CMD_CHILD_ID_GET_MISC_ROOT_KEY_LOCK,
    CMD_CHILD_ID_SET_ESCK,
    CMD_CHILD_ID_GET_ESCK,
    CMD_CHILD_ID_GET_ESCK_LOCK,
    CMD_CHILD_ID_SET_KEYLADDER_SESSION_KEY,
    CMD_CHILD_ID_LOCK_RSA_KEY,
    CMD_CHILD_ID_SET_R2R_ROOT_KEY_LOCK,
    CMD_CHILD_ID_GET_R2R_ROOT_KEY_LOCK,
    CMD_CHILD_ID_SET_SP_ROOT_KEY_LOCK,
    CMD_CHILD_ID_GET_SP_ROOT_KEY_LOCK,
    CMD_CHILD_ID_SET_DVB_CSA3_ROOT_KEY_LOCK,
    CMD_CHILD_ID_GET_DVB_CSA3_ROOT_KEY_LOCK,
    CMD_CHILD_ID_SET_JTAG_KEY_LOCK,
    CMD_CHILD_ID_GET_JTAG_KEY_LOCK,
    CMD_CHILD_ID_SET_ID_WORD,
    CMD_CHILD_ID_GET_ID_WORD,
    CMD_CHILD_ID_SET_ID_WORD_LOCK,
    CMD_CHILD_ID_GET_ID_WORD_LOCK,
    CMD_CHILD_ID_SET_CSA2_ROOTKEY_CRC,
    CMD_CHILD_ID_GET_CSA2_ROOTKEY_CRC,
    CMD_CHILD_ID_SET_CSA2_ROOTKEY_CRC_LOCK,
    CMD_CHILD_ID_GET_CSA2_ROOTKEY_CRC_LOCK,
    CMD_CHILD_ID_SET_R2R_ROOTKEY_CRC,
    CMD_CHILD_ID_GET_R2R_ROOTKEY_CRC,
    CMD_CHILD_ID_SET_R2R_ROOTKEY_CRC_LOCK,
    CMD_CHILD_ID_GET_R2R_ROOTKEY_CRC_LOCK,
    CMD_CHILD_ID_SET_SP_ROOTKEY_CRC,
    CMD_CHILD_ID_GET_SP_ROOTKEY_CRC,
    CMD_CHILD_ID_SET_SP_ROOTKEY_CRC_LOCK,
    CMD_CHILD_ID_GET_SP_ROOTKEY_CRC_LOCK,
    CMD_CHILD_ID_SET_CSA3_ROOTKEY_CRC,
    CMD_CHILD_ID_GET_CSA3_ROOTKEY_CRC,
    CMD_CHILD_ID_SET_CSA3_ROOTKEY_CRC_LOCK,
    CMD_CHILD_ID_GET_CSA3_ROOTKEY_CRC_LOCK,
    CMD_CHILD_ID_SET_JTAG_KEY_CRC,
    CMD_CHILD_ID_GET_JTAG_KEY_CRC,
    CMD_CHILD_ID_SET_JTAG_KEY_CRC_LOCK,
    CMD_CHILD_ID_GET_JTAG_KEY_CRC_LOCK,
    CMD_CHILD_ID_SET_SCS_EN_BAK,
    CMD_CHILD_ID_GET_SCS_EN_BAK,
    CMD_CHILD_ID_SET_SCS_EN_BAK_LOCK,
    CMD_CHILD_ID_GET_SCS_EN_BAK_LOCK,
    CMD_CHILD_ID_SET_JTAG_MODE_BAK,
    CMD_CHILD_ID_GET_JTAG_MODE_BAK,
    CMD_CHILD_ID_SET_JTAG_MODE_BAK_LOCK,
    CMD_CHILD_ID_GET_JTAG_MODE_BAK_LOCK,
    CMD_CHILD_ID_SET_RIGHT_CTRL_EN_BAK,
    CMD_CHILD_ID_GET_RIGHT_CTRL_EN_BAK,
    CMD_CHILD_ID_SET_RIGHT_CTRL_EN_BAK_LOCK,
    CMD_CHILD_ID_GET_RIGHT_CTRL_EN_BAK_LOCK,
    CMD_CHILD_ID_SET_MISC_KL_DISABLE,
    CMD_CHILD_ID_GET_MISC_KL_DISABLE,
    CMD_CHILD_ID_SET_MISC_KL_DISABLE_LOCK,
    CMD_CHILD_ID_GET_MISC_KL_DISABLE_LOCK,
    CMD_CHILD_ID_SET_GG_KL_DISABLE,
    CMD_CHILD_ID_GET_GG_KL_DISABLE,
    CMD_CHILD_ID_SET_GG_KL_DISABLE_LOCK,
    CMD_CHILD_ID_GET_GG_KL_DISABLE_LOCK,
    CMD_CHILD_ID_SET_BOOT_INFO_DISABLE,
    CMD_CHILD_ID_GET_BOOT_INFO_DISABLE,
    CMD_CHILD_ID_SET_CHECKSUM_READ_DISABLE,
    CMD_CHILD_ID_GET_CHECKSUM_READ_DISABLE,
    CMD_CHILD_ID_SET_SELF_BOOT_DISABLE_BAK,
    CMD_CHILD_ID_GET_SELF_BOOT_DISABLE_BAK,
    CMD_CHILD_ID_SET_TZ_AREA_LOCK,
    CMD_CHILD_ID_GET_TZ_AREA_LOCK,
    CMD_CHILD_ID_SET_BOOTMODE_0,
    CMD_CHILD_ID_GET_BOOTMODE_0,
    CMD_CHILD_ID_SET_BOOTMODE_1,
    CMD_CHILD_ID_GET_BOOTMODE_1,
    CMD_CHILD_ID_LOCK_MISC_KL_DISABLE,
    CMD_CHILD_ID_LOCK_GG_KL_DISABLE,
    CMD_CHILD_ID_LOCK_TSKL_CSA3_DISABLE,
    CMD_CHILD_ID_OTP_READ_BYTE,
    CMD_CHILD_ID_BUTT,
}CA_CMD_CHILD_ID_E;

typedef enum
{
    CA_CW_DEF  = 0x0,
    CA_CW_EVEN,
    CA_CW_ODD,
    CA_CW_BUTT
} CA_CW_TYPE;

typedef enum
{
    CA_CW  = 0x0,
    CA_R2R = 0x1
} CA_WORK_MODE;

typedef struct hiCA_CryptPm_S
{
    HI_UNF_ADVCA_KEYLADDER_LEV_E ladder;
    HI_UNF_ADVCA_CA_TARGET_E enKlTarget;
    HI_U32 pDin[8];			/* 32bytes for gdrm */
} CA_CRYPTPM_S;

typedef struct hiCA_GDRM_CryptPm_S
{
    CA_CRYPTPM_S stCACryptParams;
	HI_BOOL bIsDecrypt;
	HI_U32 u32KeyAddr;
	HI_U32 *pu32GDRMFlag;
	HI_U8 au8Output[16];
}CA_GDRM_CRYPTPM_S;

typedef struct hiCA_LOADLPK_S
{
    HI_U32 EncryptLpk[4];
}CA_LOADLPK_S;

typedef struct hiCA_DecryptLpData_S
{
    HI_U32 EncryptData[2];
    HI_U32 ClearData[2];
}CA_DECRYPTLPTDATA_S;

typedef struct hiCA_EncryptSWPK_S
{
    HI_U32 ClearSwpk[4];
    HI_U32 EncryptSwpk[4];
}CA_ENCRYPTSWPK_S;

typedef struct hiCA_RsvData_S
{
    HI_U32 type;
    HI_U32 bitOff;
    HI_U32 byteOff;
    HI_U32 val;
} CA_RSVDATA_S;

typedef struct hiCA_KEY_S
{
    HI_U8 KeyBuf[512];  
}CA_KEY_S;

/*
typedef struct hiCA_OTP_S
{
    HI_U32 Addr;
    HI_U32 Value;
}OTP_ENTRY_S;
*/
typedef struct hiCA_DCAS_PARAM_S
{
    HI_U32                       level;
    HI_U32                       pDin[4];
    HI_U32                       pDout[4];
} CA_DCAS_PARAM_S;

typedef struct hiCA_OTP_TZ_SET_STRUCT
{
    HI_U32 u32Addr;
    HI_U32 u32Len;
    HI_U8 u8Buf[128];
}CA_OTP_TZ_DATA_S;

typedef struct hiCA_CMD_SUPPER_ID_STRUCT
{
    CA_CMD_CHILD_ID_E enCmdChildID;
    HI_U8 pu8ParamBuf[256];
    HI_U32 u32ParamLen;
}CA_CMD_SUPPER_ID_S;

typedef struct hiCA_CMD_OEM_ROOT_KEY_STRUCT
{
    HI_U8 u8Key[16];
}CA_CMD_OEM_ROOT_KEY_S;

typedef struct hiCA_OTP_ATTR_S
{
    HI_UNF_ADVCA_OTP_FUSE_E enOtpFuseId;
    HI_UNF_ADVCA_OTP_ATTR_S stOtpAttr;
}CA_OTP_ATTR_S;

typedef struct hiCA_KEYLADDER_ATTR_S
{
    HI_UNF_ADVCA_KEYLADDER_TYPE_E enKeyLadderType;
    HI_UNF_ADVCA_KEYLADDER_ATTR_S stKeyladderAttr;
}CA_KEYLADDER_ATTR_S;

typedef struct hiCA_CAVENDOR_ATTR_S
{
    HI_UNF_ADVCA_VENDORID_E enCaVendor;
    HI_UNF_ADVCA_CA_VENDOR_OPT_S stCaVendorOpt;
}CA_VENDOR_OPT_S;

/*Revision number*/
#ifndef HI_ADVCA_FUNCTION_RELEASE
#define ADVCA_REVISION "V1.0.1.8_20130604\0"
#define NAGRA_REVISION "V1.0.1.4_20130607\0"
#else
#define ADVCA_REVISION ""
#define NAGRA_REVISION ""
#endif
/* Ioctl definitions */
#define CMD_CA_SET_CHIPID                       _IOW(HI_ID_CA, 0x01, HI_U32)
#define CMD_CA_GET_CHIPID                        _IOR(HI_ID_CA, 0x02, HI_U32)
#define CMD_CA_GET_UNIQUE_CHIPID                  _IOR(HI_ID_CA, 0x03, CA_KEY_S)
#define CMD_CA_SET_CHIPID_LOCK                  _IO(HI_ID_CA, 0x04)
#define CMD_CA_GET_CHIPID_LOCK                  _IOR(HI_ID_CA, 0x05, HI_U32)
#define CMD_CA_SET_MARKETID                      _IOW(HI_ID_CA, 0x06, HI_U32)
#define CMD_CA_GET_MARKETID                      _IOR(HI_ID_CA, 0x07, HI_U32)
#define CMD_CA_SET_MKTID_LOCK                   _IO(HI_ID_CA, 0x08)
#define CMD_CA_GET_MKTID_LOCK                   _IOR(HI_ID_CA, 0x09, HI_U32)
#define CMD_CA_SET_STBSN                         _IOW(HI_ID_CA, 0x0A, HI_U32)
#define CMD_CA_GET_STBSN                         _IOR(HI_ID_CA, 0x0B, HI_U32)
#define CMD_CA_SET_STBSN_LOCK                   _IO(HI_ID_CA, 0x0C)
#define CMD_CA_GET_STBSN_LOCK                   _IOR(HI_ID_CA, 0x0D, HI_U32)
#define CMD_CA_SET_VERSIONID                     _IOW(HI_ID_CA, 0x0E, HI_U32)
#define CMD_CA_GET_VERSIONID                     _IOR(HI_ID_CA, 0x0F, HI_U32)
#define CMD_CA_SET_VERSION_ID_LOCK              _IO(HI_ID_CA, 0x10)
#define CMD_CA_GET_VERSION_ID_LOCK              _IOR(HI_ID_CA, 0x11, HI_U32)

#define CMD_CA_LOCKHARDCWSEL                    _IO(HI_ID_CA, 0x12)
#define CMD_CA_STATHARDCWSEL                    _IOR(HI_ID_CA, 0x13, HI_U32)
#define CMD_CA_LOCKBOOTDECEN                    _IO(HI_ID_CA, 0x14)
#define CMD_CA_LOCKR2RHARDKEY                   _IO(HI_ID_CA, 0x15)
#define CMD_CA_STATR2RHARDKEY                   _IOR(HI_ID_CA, 0x16, HI_U32)
#define CMD_CA_DISABLELPT                       _IO(HI_ID_CA, 0x17)
#define CMD_CA_DISABLESELFBOOT                  _IO(HI_ID_CA, 0x18)
#define CMD_CA_STATTDESLOCK                     _IOR(HI_ID_CA, 0x19, HI_U32)

#define CMD_CA_DVB_CRYPT                        _IOW(HI_ID_CA, 0x1A, CA_CRYPTPM_S)
#define CMD_CA_DVB_SETLADDER                    _IOW(HI_ID_CA, 0x1B, HI_UNF_ADVCA_KEYLADDER_LEV_E)
#define CMD_CA_DVB_GETLADDER                    _IOR(HI_ID_CA, 0x1C, HI_UNF_ADVCA_KEYLADDER_LEV_E)
#define CMD_CA_DVB_SETLADDERLOCK                _IO(HI_ID_CA, 0x1D)
#define CMD_CA_DVB_GETLADDERLOCK                _IOR(HI_ID_CA, 0x1E, HI_U32)
#define CMD_CA_DVB_SETALG                       _IOW(HI_ID_CA, 0x1F, HI_UNF_ADVCA_ALG_TYPE_E)
#define CMD_CA_DVB_GETALG                       _IOR(HI_ID_CA, 0x20, HI_UNF_ADVCA_ALG_TYPE_E)
#define CMD_CA_R2R_CRYPT                        _IOW(HI_ID_CA, 0x21, CA_CRYPTPM_S)
#define CMD_CA_R2R_SETLADDER                    _IOW(HI_ID_CA, 0x22, HI_UNF_ADVCA_KEYLADDER_LEV_E)
#define CMD_CA_R2R_GETLADDER                    _IOR(HI_ID_CA, 0x23, HI_UNF_ADVCA_KEYLADDER_LEV_E)
#define CMD_CA_R2R_SETLADDERLOCK                _IO(HI_ID_CA, 0x24)
#define CMD_CA_R2R_GETLADDERLOCK                _IOR(HI_ID_CA, 0x25, HI_U32)
#define CMD_CA_R2R_SETALG                       _IOW(HI_ID_CA, 0x26, HI_UNF_ADVCA_ALG_TYPE_E)
#define CMD_CA_R2R_GETALG                       _IOR(HI_ID_CA, 0x27, HI_UNF_ADVCA_ALG_TYPE_E)
#define CMD_CA_SP_CRYPT                         _IOW(HI_ID_CA, 0x28, CA_CRYPTPM_S)
#define CMD_CA_SP_SETLADDER                     _IOW(HI_ID_CA, 0x29, HI_UNF_ADVCA_KEYLADDER_LEV_E)
#define CMD_CA_SP_GETLADDER                     _IOR(HI_ID_CA, 0x2A, HI_UNF_ADVCA_KEYLADDER_LEV_E)
#define CMD_CA_SP_SETLADDERLOCK                 _IO(HI_ID_CA, 0x2B)
#define CMD_CA_SP_GETLADDERLOCK                 _IOR(HI_ID_CA, 0x2C, HI_U32)
#define CMD_CA_SP_SETALG                        _IOW(HI_ID_CA, 0x2D, HI_UNF_ADVCA_ALG_TYPE_E)
#define CMD_CA_SP_GETALG                        _IOR(HI_ID_CA, 0x2E, HI_UNF_ADVCA_ALG_TYPE_E)
#define CMD_CA_CSA3_CRYPT                       _IOW(HI_ID_CA, 0x2F, CA_CRYPTPM_S)
#define CMD_CA_CSA3_SETLADDER                   _IOW(HI_ID_CA, 0x30, HI_UNF_ADVCA_KEYLADDER_LEV_E)
#define CMD_CA_CSA3_GETLADDER                   _IOR(HI_ID_CA, 0x31, HI_UNF_ADVCA_KEYLADDER_LEV_E)
#define CMD_CA_CSA3_SETLADDERLOCK               _IO(HI_ID_CA, 0x32)
#define CMD_CA_CSA3_GETLADDERLOCK               _IOR(HI_ID_CA, 0x33, HI_U32)
#define CMD_CA_DVB_CSA3_SETALG                  _IOW(HI_ID_CA, 0x34, HI_UNF_ADVCA_ALG_TYPE_E)
#define CMD_CA_DVB_CSA3_GETALG                  _IOR(HI_ID_CA, 0x35, HI_UNF_ADVCA_ALG_TYPE_E)

#define CMD_CA_SET_BOOTMODE                      _IOW(HI_ID_CA, 0x36, HI_UNF_ADVCA_FLASH_TYPE_E)
#define CMD_CA_GET_BOOTMODE                      _IOR(HI_ID_CA, 0x37, HI_UNF_ADVCA_FLASH_TYPE_E)
#define CMD_CA_SET_BOOT_SEL_LOCK_0              _IO(HI_ID_CA, 0x38)
#define CMD_CA_SET_BOOT_SEL_LOCK_1              _IO(HI_ID_CA, 0x39)                             //Only used for V300
#define CMD_CA_GET_BOOT_SEL_LOCK_0              _IOR(HI_ID_CA, 0x3A, HI_U32)
#define CMD_CA_GET_BOOT_SEL_LOCK_1              _IOR(HI_ID_CA, 0x3B, HI_U32)                    //Only used for V300
#define CMD_CA_SET_SCSACTIVE                     _IO(HI_ID_CA, 0x3C)
#define CMD_CA_GET_SCSACTIVE                     _IOR(HI_ID_CA, 0x3D, HI_U32)
#define CMD_CA_SET_JTAGPRTMODE                   _IOW(HI_ID_CA, 0x3E, HI_UNF_ADVCA_JTAG_MODE_E)
#define CMD_CA_GET_JTAGPRTMODE                   _IOR(HI_ID_CA, 0x3F, HI_UNF_ADVCA_JTAG_MODE_E)
#define CMD_CA_SET_RSVDATA                       _IOW(HI_ID_CA, 0x40, CA_RSVDATA_S)
#define CMD_CA_GET_RSVDATA                       _IOWR(HI_ID_CA, 0x41,CA_RSVDATA_S)

#define CMD_CA_SET_PROTECT                       _IO(HI_ID_CA, 0x42)
#define CMD_CA_GET_PROTECT                       _IOR(HI_ID_CA, 0x43, HI_U32)
#define CMD_CA_SET_ACCESS                        _IOW(HI_ID_CA, 0x44, HI_U32)

#define CMD_CA_LPK_LOAD                         _IOW(HI_ID_CA, 0x45, CA_LOADLPK_S)
#define CMD_CA_LPK_DECRYPT                      _IOWR(HI_ID_CA, 0x46, CA_DECRYPTLPTDATA_S)
#define CMD_CA_SWPK_CRYPT                       _IOWR(HI_ID_CA, 0x47, CA_ENCRYPTSWPK_S)

#define CMD_CA_EXT1_OTPREAD                     _IOWR(HI_ID_CA, 0x48, OTP_ENTRY_S)
#define CMD_CA_EXT1_OTPWRITE                    _IOW(HI_ID_CA, 0x49, OTP_ENTRY_S)
#define CMD_CA_EXT1_OTPDISFUNC                  _IOW(HI_ID_CA, 0x4A, HI_U32)
#define CMD_CA_EXT1_OTPWRITEBYTE                _IOW(HI_ID_CA, 0x4B, OTP_ENTRY_S)

#define CMD_CA_SET_BOOTSEL_CTRL                 _IOW(HI_ID_CA, 0x4C, HI_U32)
#define CMD_CA_GET_BOOTSEL_CTRL                 _IOR(HI_ID_CA, 0x4D, HI_U32)
#define CMD_CA_SET_BOOTSEL_CTRL_LOCK            _IO(HI_ID_CA, 0x4E)
#define CMD_CA_GET_BOOTSEL_CTRL_LOCK            _IOR(HI_ID_CA, 0x4F, HI_U32)
#define CMD_CA_SET_RSA_KEY_N                    _IOW(HI_ID_CA, 0x50, CA_KEY_S)
#define CMD_CA_SET_RSA_KEY_E                    _IOW(HI_ID_CA, 0x51, CA_KEY_S)
#define CMD_CA_GET_RSA_KEY_N                    _IOR(HI_ID_CA, 0x52, CA_KEY_S)
#define CMD_CA_GET_RSA_KEY_E                    _IOR(HI_ID_CA, 0x53, CA_KEY_S)
#define CMD_CA_SET_RSA_KEY_LOCK                 _IO(HI_ID_CA, 0x54)
#define CMD_CA_GET_RSA_KEY_LOCK                 _IOR(HI_ID_CA, 0x55, HI_U32)
#define CMD_CA_SET_DVB_ROOT_KEY                 _IOW(HI_ID_CA, 0x56, CA_KEY_S)
#define CMD_CA_GET_DVB_ROOT_KEY                 _IOR(HI_ID_CA, 0x57, CA_KEY_S)
#define CMD_CA_SET_DVB_CSA3_ROOT_KEY            _IOW(HI_ID_CA, 0x58, CA_KEY_S)
#define CMD_CA_GET_DVB_CSA3_ROOT_KEY            _IOR(HI_ID_CA, 0x59, CA_KEY_S)
#define CMD_CA_SET_R2R_ROOT_KEY                 _IOW(HI_ID_CA, 0x5A, CA_KEY_S)
#define CMD_CA_GET_R2R_ROOT_KEY                 _IOR(HI_ID_CA, 0x5B, CA_KEY_S)
#define CMD_CA_SET_SP_ROOT_KEY                  _IOW(HI_ID_CA, 0x5C, CA_KEY_S)
#define CMD_CA_GET_SP_ROOT_KEY                  _IOR(HI_ID_CA, 0x5D, CA_KEY_S)
#define CMD_CA_SET_JTAG_KEY                     _IOW(HI_ID_CA, 0x5E, CA_KEY_S)
#define CMD_CA_GET_JTAG_KEY                     _IOR(HI_ID_CA, 0x5F, CA_KEY_S)
#define CMD_CA_SET_SECRET_KEY_LOCK              _IO(HI_ID_CA, 0x60)
#define CMD_CA_GET_SECRET_KEY_LOCK              _IOR(HI_ID_CA, 0x61, HI_U32)

#define CMD_CA_SET_VENDOR_ID                    _IOW(HI_ID_CA, 0x62, HI_U32)
#define CMD_CA_GET_VENDOR_ID                    _IOR(HI_ID_CA, 0x63, HI_U32)
#define CMD_CA_SET_VENDOR_ID_LOCK               _IO(HI_ID_CA, 0x64)
#define CMD_CA_GET_VENDOR_ID_LOCK               _IOR(HI_ID_CA, 0x65, HI_U32)
#define CMD_CA_SET_HDCP_KEY                     _IOW(HI_ID_CA, 0x66, CA_KEY_S)
#define CMD_CA_GET_HDCP_KEY                     _IOR(HI_ID_CA, 0x67, CA_KEY_S)
#define CMD_CA_SET_HDCP_KEY_LOCK                _IO(HI_ID_CA, 0x68)
#define CMD_CA_GET_HDCP_KEY_LOCK                _IOR(HI_ID_CA, 0x69, HI_U32)

#define CMD_CA_SET_BLOAD_DEC_EN                 _IOW(HI_ID_CA, 0x6A, HI_U32)
#define CMD_CA_GET_BLOAD_DEC_EN                 _IOR(HI_ID_CA, 0x6B, HI_U32)
#define CMD_CA_SET_BLOAD_DEC_EN_LOCK            _IO(HI_ID_CA, 0x6C)
#define CMD_CA_GET_BLOAD_DEC_EN_LOCK            _IOR(HI_ID_CA, 0x6D, HI_U32)
#define CMD_CA_SET_BLOAD_ENC_EN                 _IOW(HI_ID_CA, 0x6E, HI_U32)                    
//#define CMD_CA_GET_BLOAD_ENC_EN                 _IOR(HI_ID_CA, 0x6F, HI_U32)                    /* TODO:*/
#define CMD_CA_SET_DES_HARDONLY_EN              _IOW(HI_ID_CA, 0x6F, HI_U32)
//#define CMD_CA_SET_BLOAD_ENC_EN_LOCK            _IO(HI_ID_CA, 0x70)                             /* TODO:*/
#define CMD_CA_LOCKSECRETKEY                    _IO(HI_ID_CA, 0x70)
//#define CMD_CA_GET_BLOAD_ENC_EN_LOCK            _IOR(HI_ID_CA, 0x71, HI_U32)                    /* TODO:*/
#define CMD_CA_SET_BLOAD_MODE_SEL               _IOW(HI_ID_CA, 0x72, HI_U32)
#define CMD_CA_GET_BLOAD_MODE_SEL               _IOR(HI_ID_CA, 0x73, HI_U32)
#define CMD_CA_SET_BLOAD_MODE_SEL_LOCK          _IO(HI_ID_CA, 0x74)
#define CMD_CA_GET_BLOAD_MODE_SEL_LOCK          _IOR(HI_ID_CA, 0x75, HI_U32)
#define CMD_CA_SET_SELFBOOT                     _IOW(HI_ID_CA, 0x76, HI_U32)                    /*Used for V300 with a parameter*/
#define CMD_CA_GET_SELFBOOT                     _IOR(HI_ID_CA, 0x77, HI_U32)
#define CMD_CA_SET_SELFBOOT_LOCK                _IO(HI_ID_CA, 0x78)
#define CMD_CA_GET_SELFBOOT_LOCK                _IOR(HI_ID_CA, 0x79, HI_U32)
#define CMD_CA_SET_LOWPOWER_DISABLE             _IOW(HI_ID_CA, 0x7A, HI_U32)
#define CMD_CA_GET_LOWPOWER_DISABLE             _IOR(HI_ID_CA, 0x7B, HI_U32)
#define CMD_CA_SET_LOWPOWER_DISABLE_LOCK        _IO(HI_ID_CA, 0x7C)
#define CMD_CA_GET_LOWPOWER_DISABLE_LOCK        _IOR(HI_ID_CA, 0x7D, HI_U32)

#define CMD_CA_SET_LINK_PRT_DISABLE             _IOW(HI_ID_CA, 0x7E, HI_U32)                    /*Used for V300 with a parameter*/
#define CMD_CA_GET_LINK_PRT_DISABLE             _IOR(HI_ID_CA, 0x7F, HI_U32)
#define CMD_CA_SET_LINK_PRT_DISABLE_LOCK        _IO(HI_ID_CA, 0x80)
#define CMD_CA_GET_LINK_PRT_DISABLE_LOCK        _IOR(HI_ID_CA, 0x81, HI_U32)
#define CMD_CA_SET_CSA2_HARDONLY_EN             _IOW(HI_ID_CA, 0x82, HI_U32)                    /*Used for V300 with a parameter*/
#define CMD_CA_GET_CSA2_HARDONLY_EN             _IOR(HI_ID_CA, 0x83, HI_U32)
#define CMD_CA_SET_CSA2_HARDONLY_EN_LOCK        _IO(HI_ID_CA, 0x84)
#define CMD_CA_GET_CSA2_HARDONLY_EN_LOCK        _IOR(HI_ID_CA, 0x85, HI_U32)
#define CMD_CA_SET_CSA3_HARDONLY_EN             _IOW(HI_ID_CA, 0x86, HI_U32)
#define CMD_CA_GET_CSA3_HARDONLY_EN             _IOR(HI_ID_CA, 0x87, HI_U32)
#define CMD_CA_SET_CSA3_HARDONLY_EN_LOCK        _IO(HI_ID_CA, 0x88)
#define CMD_CA_GET_CSA3_HARDONLY_EN_LOCK        _IOR(HI_ID_CA, 0x89, HI_U32)
#define CMD_CA_SET_SP_HARDONLY_EN               _IOW(HI_ID_CA, 0x8A, HI_U32)
#define CMD_CA_GET_SP_HARDONLY_EN               _IOR(HI_ID_CA, 0x8B, HI_U32)
#define CMD_CA_SET_SP_HARDONLY_EN_LOCK          _IO(HI_ID_CA, 0x8C)
#define CMD_CA_GET_SP_HARDONLY_EN_LOCK          _IOR(HI_ID_CA, 0x8D, HI_U32)
#define CMD_CA_SET_TSKL_CSA2_DISABLE            _IOW(HI_ID_CA, 0x8E, HI_U32)
#define CMD_CA_GET_TSKL_CSA2_DISABLE            _IOR(HI_ID_CA, 0x8F, HI_U32)
#define CMD_CA_SET_TSKL_CSA2_DISABLE_LOCK       _IO(HI_ID_CA, 0x90)
#define CMD_CA_GET_TSKL_CSA2_DISABLE_LOCK       _IOR(HI_ID_CA, 0x91, HI_U32)
#define CMD_CA_SET_TSKL_CSA3_DISABLE            _IOW(HI_ID_CA, 0x92, HI_U32)
#define CMD_CA_GET_TSKL_CSA3_DISABLE            _IOR(HI_ID_CA, 0x93, HI_U32)
#define CMD_CA_SET_TSKL_CSA3_DISABLE_LOCK       _IO(HI_ID_CA, 0x94)
#define CMD_CA_GET_TSKL_CSA3_DISABLE_LOCK       _IOR(HI_ID_CA, 0x95, HI_U32)
#define CMD_CA_SET_TSKL_SP_DISABLE              _IOW(HI_ID_CA, 0x96, HI_U32)
#define CMD_CA_GET_TSKL_SP_DISABLE              _IOR(HI_ID_CA, 0x97, HI_U32)
#define CMD_CA_SET_TSKL_SP_DISABLE_LOCK         _IO(HI_ID_CA, 0x98)
#define CMD_CA_GET_TSKL_SP_DISABLE_LOCK         _IOR(HI_ID_CA, 0x99, HI_U32)
#define CMD_CA_SET_TSKL_DES_DISABLE             _IOW(HI_ID_CA, 0x9A, HI_U32)
#define CMD_CA_GET_TSKL_DES_DISABLE             _IOR(HI_ID_CA, 0x9B, HI_U32)
#define CMD_CA_SET_TSKL_DES_DISABLE_LOCK        _IO(HI_ID_CA, 0x9C)
#define CMD_CA_GET_TSKL_DES_DISABLE_LOCK        _IOR(HI_ID_CA, 0x9D, HI_U32)
#define CMD_CA_SET_MC_AES_HARD_ONLY_EN          _IOW(HI_ID_CA, 0x9E, HI_U32)
#define CMD_CA_GET_MC_AES_HARD_ONLY_EN          _IOR(HI_ID_CA, 0x9F, HI_U32)
#define CMD_CA_SET_MC_AES_HARD_ONLY_EN_LOCK     _IO(HI_ID_CA, 0xA0)
#define CMD_CA_GET_MC_AES_HARD_ONLY_EN_LOCK     _IOR(HI_ID_CA, 0xA1, HI_U32)
#define CMD_CA_SET_MC_TDES_HARD_ONLY_EN         _IOW(HI_ID_CA, 0xA2, HI_U32)
#define CMD_CA_GET_MC_TDES_HARD_ONLY_EN         _IOR(HI_ID_CA, 0xA3, HI_U32)
#define CMD_CA_SET_MC_TDES_HARD_ONLY_EN_LOCK    _IO(HI_ID_CA, 0xA4)
#define CMD_CA_GET_MC_TDES_HARD_ONLY_EN_LOCK    _IOR(HI_ID_CA, 0xA5, HI_U32)
#define CMD_CA_SET_MCKL_DES_HARD_DISABLE        _IOW(HI_ID_CA, 0xA6, HI_U32)
#define CMD_CA_GET_MCKL_DES_HARD_DISABLE        _IOR(HI_ID_CA, 0xA7, HI_U32)
#define CMD_CA_SET_MCKL_DES_HARD_DISABLE_LOCK   _IO(HI_ID_CA, 0xA8)
#define CMD_CA_GET_MCKL_DES_HARD_DISABLE_LOCK   _IOR(HI_ID_CA, 0xA9, HI_U32)

#define CMD_CA_SET_DDR_SCRAMBLE_EN              _IOW(HI_ID_CA, 0xAA, HI_U32)
#define CMD_CA_GET_DDR_SCRAMBLE_EN              _IOR(HI_ID_CA, 0xAB, HI_U32)
#define CMD_CA_SET_DDR_SCRAMBLE_EN_LOCK         _IO(HI_ID_CA, 0xAC)
#define CMD_CA_GET_DDR_SCRAMBLE_EN_LOCK         _IOR(HI_ID_CA, 0xAD, HI_U32)
#define CMD_CA_SET_OTP_GLOBAL_LOCK_EN           _IOW(HI_ID_CA, 0xAE, HI_U32)
#define CMD_CA_GET_OTP_GLOBAL_LOCK_EN           _IOR(HI_ID_CA, 0xAF, HI_U32)
#define CMD_CA_SET_OTP_GLOBAL_LOCK_EN_LOCK      _IO(HI_ID_CA, 0xB0)
#define CMD_CA_GET_OTP_GLOBAL_LOCK_EN_LOCK      _IOR(HI_ID_CA, 0xB1, HI_U32)
#define CMD_CA_SET_SECURE_CHIP_FLAG             _IOW(HI_ID_CA, 0xB2, HI_U32)
#define CMD_CA_GET_SECURE_CHIP_FLAG             _IOR(HI_ID_CA, 0xB3, HI_U32)
#define CMD_CA_SET_SECURE_CHIP_FLAG_LOCK        _IO(HI_ID_CA, 0xB4)
#define CMD_CA_GET_SECURE_CHIP_FLAG_LOCK        _IOR(HI_ID_CA, 0xB5, HI_U32)
#define CMD_CA_SET_DCAS_KL_DISABLE              _IOW(HI_ID_CA, 0xB6, HI_U32)
#define CMD_CA_GET_DCAS_KL_DISABLE              _IOR(HI_ID_CA, 0xB7, HI_U32)
#define CMD_CA_SET_DCAS_KL_DISABLE_LOCK         _IO(HI_ID_CA, 0xB8)
#define CMD_CA_GET_DCAS_KL_DISABLE_LOCK         _IOR(HI_ID_CA, 0xB9, HI_U32)
#define CMD_CA_SET_TS_OUT_DISABLE               _IOW(HI_ID_CA, 0xBA, HI_U32)
#define CMD_CA_GET_TS_OUT_DISABLE               _IOR(HI_ID_CA, 0xBB, HI_U32)
#define CMD_CA_SET_TS_OUT_DISABLE_LOCK          _IO(HI_ID_CA, 0xBC)
#define CMD_CA_GET_TS_OUT_DISABLE_LOCK          _IOR(HI_ID_CA, 0xBD, HI_U32)
#define CMD_CA_SET_LPC_DISABLE                  _IOW(HI_ID_CA, 0xBE, HI_U32)
#define CMD_CA_GET_LPC_DISABLE                  _IOR(HI_ID_CA, 0xBF, HI_U32)
#define CMD_CA_SET_LPC_DISABLE_LOCK             _IO(HI_ID_CA, 0xC0)
#define CMD_CA_GET_LPC_DISABLE_LOCK             _IOR(HI_ID_CA, 0xC1, HI_U32)
#define CMD_CA_SET_LPC_MASTER_DISABLE           _IOW(HI_ID_CA, 0xC2, HI_U32)
#define CMD_CA_GET_LPC_MASTER_DISABLE           _IOR(HI_ID_CA, 0xC3, HI_U32)
#define CMD_CA_SET_LPC_MASTER_DISABLE_LOCK      _IO(HI_ID_CA, 0xC4)
#define CMD_CA_GET_LPC_MASTER_DISABLE_LOCK      _IOR(HI_ID_CA, 0xC5, HI_U32)

#define CMD_CA_SET_RUNTIME_CHECK_EN             _IOW(HI_ID_CA, 0xC6, HI_U32)
#define CMD_CA_GET_RUNTIME_CHECK_EN             _IOR(HI_ID_CA, 0xC7, HI_U32)
#define CMD_CA_SET_RUNTIME_CHECK_EN_LOCK        _IO(HI_ID_CA, 0xC8)
#define CMD_CA_GET_RUNTIME_CHECK_EN_LOCK        _IOR(HI_ID_CA, 0xC9, HI_U32)
#define CMD_CA_SET_WAKEUP_DDR_CHECK_EN          _IOW(HI_ID_CA, 0xCA, HI_U32)
#define CMD_CA_GET_WAKEUP_DDR_CHECK_EN          _IOR(HI_ID_CA, 0xCB, HI_U32)
#define CMD_CA_SET_WAKEUP_DDR_CHECK_EN_LOCK     _IO(HI_ID_CA, 0xCC)
#define CMD_CA_GET_WAKEUP_DDR_CHECK_EN_LOCK     _IOR(HI_ID_CA, 0xCD, HI_U32)
#define CMD_CA_SET_VERSION_CHECK_EN             _IOW(HI_ID_CA, 0xCE, HI_U32)
#define CMD_CA_GET_VERSION_CHECK_EN             _IOR(HI_ID_CA, 0xCF, HI_U32)
#define CMD_CA_SET_VERSION_CHECK_EN_LOCK        _IO(HI_ID_CA, 0xD0)
#define CMD_CA_GET_VERSION_CHECK_EN_LOCK        _IOR(HI_ID_CA, 0xD1, HI_U32)
#define CMD_CA_SET_BL_MSID_CHECK_EN             _IOW(HI_ID_CA, 0xD2, HI_U32)
#define CMD_CA_GET_BL_MSID_CHECK_EN             _IOR(HI_ID_CA, 0xD3, HI_U32)
#define CMD_CA_SET_BL_MSID_CHECK_EN_LOCK        _IO(HI_ID_CA, 0xD4)
#define CMD_CA_GET_BL_MSID_CHECK_EN_LOCK        _IOR(HI_ID_CA, 0xD5, HI_U32)
#define CMD_CA_SET_JTAG_READ_DISABLE            _IO(HI_ID_CA, 0xD6)
#define CMD_CA_GET_JTAG_READ_DISABLE            _IOR(HI_ID_CA, 0xD7, HI_U32)
#define CMD_CA_SET_JTAG_WRITE_DISABLE           _IO(HI_ID_CA, 0xD8)
#define CMD_CA_GET_JTAG_WRITE_DISABLE           _IOR(HI_ID_CA, 0xD9, HI_U32)
#define CMD_CA_SET_DEBUG_DISABLE                _IO(HI_ID_CA, 0xDA)
#define CMD_CA_GET_DEBUG_DISABLE                _IOR(HI_ID_CA, 0xDB, HI_U32)
#define CMD_CA_SET_RIGHT_CTRL_EN                _IO(HI_ID_CA, 0xDC)
#define CMD_CA_GET_RIGHT_CTRL_EN                _IOR(HI_ID_CA, 0xDD, HI_U32)

/*Caution: Be careful with the following commands!*/
#define CMD_CA_EXT0_SETCWDEACTIVE               _IO(HI_ID_CA, 0xDE)
#define CMD_CA_EXT0_GETCWDEACTIVE               _IOR(HI_ID_CA, 0xDF, HI_U32)
#define CMD_CA_EXT0_SETR2RDEACTIVE              _IO(HI_ID_CA, 0xE0)
#define CMD_CA_EXT0_GETR2RDEACTIVE              _IOR(HI_ID_CA, 0xE1, HI_U32)
#define CMD_CA_EXT0_SETCSA3DEACTIVE             _IO(HI_ID_CA, 0xE2)
#define CMD_CA_EXT0_GETCSA3DEACTIVE             _IOR(HI_ID_CA, 0xE3, HI_U32)
#define CMD_CA_EXT0_SETSPDEACTIVE               _IO(HI_ID_CA, 0xE4)
#define CMD_CA_EXT0_GETSPDEACTIVE               _IOR(HI_ID_CA, 0xE5, HI_U32)
#define CMD_CA_EXT0_SETDBGDEACTIVE              _IOW(HI_ID_CA, 0xE6, HI_U32)
#define CMD_CA_EXT0_GETDBGDEACTIVE              _IOR(HI_ID_CA, 0xE7, HI_U32)
#define CMD_CA_EXT0_SETRSAKEYLEN                _IOW(HI_ID_CA, 0xE8, HI_U32)
#define CMD_CA_EXT0_GETRSAKEYLEN                _IOR(HI_ID_CA, 0xE9, HI_U32)
#define CMD_CA_EXT0_SETJTAGKEYLEN               _IO(HI_ID_CA, 0xEA)
#define CMD_CA_EXT0_GETJTAGKEYLEN               _IOR(HI_ID_CA, 0xEB, HI_U32)
#define CMD_CA_EXT1_SETRSAKEY                   _IOW(HI_ID_CA, 0xEC, CA_KEY_S)
#define CMD_CA_EXT1_GETRSAKEY                   _IOR(HI_ID_CA, 0xED, CA_KEY_S)
#define CMD_CA_CHECK_MARKET_ID_SET              _IOR(HI_ID_CA, 0xEE, HI_U32)

#define CMD_CA_DCAS_OPEN                        _IOW(HI_ID_CA, 0xEF, HI_U32)
#define CMD_CA_DCAS_CLOSE                       _IO(HI_ID_CA, 0xF0)
#define CMD_CA_DCAS_PARAM_ID_SET                _IOWR(HI_ID_CA, 0xF1, CA_DCAS_PARAM_S)

#define CMD_CA_SWPK_KEY_LADDER_OPEN             _IO(HI_ID_CA, 0xF2)
#define CMD_CA_SWPK_KEY_LADDER_CLOSE            _IO(HI_ID_CA, 0xF3)

#define CMD_CA_GET_CHIP_VERSION                 _IOR(HI_ID_CA, 0xF4, HI_U32)
#define CMD_CA_SET_PCIE_EN                      _IOW(HI_ID_CA, 0xF5, HI_U32)
#define CMD_CA_GET_PCIE_EN                      _IOR(HI_ID_CA, 0xF6, HI_U32)
#define CMD_CA_SET_CSA3_EN                      _IO(HI_ID_CA, 0xF7)
#define CMD_CA_GET_CSA3_EN                      _IOR(HI_ID_CA, 0xF8, HI_U32)
#define CMD_CA_SETSR7                           _IOW(HI_ID_CA, 0xF9, HI_U32)
#define CMD_CA_GETSR7                           _IOR(HI_ID_CA, 0xFA, HI_U32)
#define CMD_CA_SETSR6                           _IOW(HI_ID_CA, 0xFB, HI_U32)
#define CMD_CA_GETSR6                           _IOR(HI_ID_CA, 0xFC, HI_U32)
#define CMD_CA_SETSR4                           _IOW(HI_ID_CA, 0xFD, HI_U32)
#define CMD_CA_GETSR4                           _IOR(HI_ID_CA, 0xFE, HI_U32)
#define CMD_CA_SETSR2                           _IOW(HI_ID_CA, 0xFF, HI_U32)
#define CMD_CA_GETSR2                           _IOR(HI_ID_CA, 0x100, HI_U32)
//#define CMD_CA_SET_DES_HARDONLY_EN              _IOW(HI_ID_CA, 0x101, HI_U32)
#define CMD_CA_SET_MISC_CTRL_EN                 _IO(HI_ID_CA, 0x102)
#define CMD_CA_GET_MISC_CTRL_EN                 _IOR(HI_ID_CA, 0x103, HI_U32)
//#define CMD_CA_LOCKSECRETKEY                    _IO(HI_ID_CA, 0x104)
#define CMD_CA_OTP_RESET                        _IO(HI_ID_CA, 0x105)
#define CMD_CA_GET_REVISION                     _IOR(HI_ID_CA, 0x106, CA_KEY_S)
#define CMD_CA_SP_SET_DSC_MODE                  _IOW(HI_ID_CA, 0x107, HI_UNF_ADVCA_SP_DSC_MODE_E)
#define CMD_CA_SP_GET_DSC_MODE                  _IOR(HI_ID_CA, 0x108, HI_UNF_ADVCA_SP_DSC_MODE_E)
#define CMD_CA_SUPPER_ID                        _IOWR(HI_ID_CA, 0x109, CA_CMD_SUPPER_ID_S)

#define CMD_CA_SET_OTP_FUSE                     _IOWR(HI_ID_CA, 0x10A, CA_OTP_ATTR_S)
#define CMD_CA_GET_OTP_FUSE                     _IOWR(HI_ID_CA, 0x10B, CA_OTP_ATTR_S)

#define CMD_CA_SET_KEYLADDER_ATTR               _IOWR(HI_ID_CA, 0x10C, CA_KEYLADDER_ATTR_S)

#define CMD_CA_SET_CAVENDOR_OPT                 _IOWR(HI_ID_CA, 0x10D, CA_VENDOR_OPT_S)

HI_S32 DRV_ADVCA_DecryptCws(HI_U32 AddrID,HI_U32 EvenOrOdd, HI_U8 *pData);
HI_S32 DRV_ADVCA_DecryptCsa3s(HI_U32 AddrID,HI_U32 EvenOrOdd, HI_U8 *pData);
HI_S32 DRV_ADVCA_DecryptSPs(HI_U32 AddrID,HI_U32 EvenOrOdd, HI_U8 *pData, DRV_ADVCA_CA_TARGET_E enCwTarget);
HI_S32 DRV_ADVCA_DecryptMisc(HI_U32 AddrID,HI_U32 EvenOrOdd, HI_U8 *pDataIn, HI_UNF_ADVCA_CA_TARGET_E enKlTarget);
HI_S32 DRV_ADVCA_CryptGDRM(HI_U32 AddrID, HI_U32 *pu32DataIn, HI_BOOL bIsDeCrypt, HI_UNF_ADVCA_CA_TARGET_E enKlTarget);
HI_S32 DRV_ADVCA_EncryptCipher(HI_U32 AddrID, HI_U32 *pDataIn);
HI_S32 DRV_ADVCA_DecryptCipher(HI_U32 AddrID, HI_U32 *pDataIn);

/** CA vendor type*/
/*
typedef enum hiUNF_ADVCA_VENDOR_TYPE_E
{
    HI_UNF_ADVCA_VENDOR_NONE  = 0x0,
    HI_UNF_ADVCA_VENDOR_NAGRA,
    HI_UNF_ADVCA_VENDOR_IRDETO,
    HI_UNF_ADVCA_VENDOR_CONAX,
    HI_UNF_ADVCA_VENDOR_NDS,
    HI_UNF_ADVCA_VENDOR_SUMA,
    HI_UNF_ADVCA_VENDOR_NOVEL,
    HI_UNF_ADVCA_VENDOR_VERIMATRIX,
    HI_UNF_ADVCA_VENDOR_CTI,
	HI_UNF_ADVCA_VENDOR_SAFEVIEW,
	HI_UNF_ADVCA_VENDOR_LATENSE,
    HI_UNF_ADVCA_VENDOR_SH_TELECOM,
    HI_UNF_ADVCA_VENDOR_VIACCESS,
    HI_UNF_ADVCA_VENDOR_BUTT
}HI_UNF_ADVCA_VENDOR_TYPE_E;
*/
    
typedef union
{
    struct 
    {
        HI_U32 CSA2_RootKey     :1; //[0]
        HI_U32 R2R_RootKey      :1; //[1]
        HI_U32 SP_RootKey       :1; //[2]
        HI_U32 CSA3_RootKey     :1; //[3]
        HI_U32 ChipID_JTAGKey   :1; //[4]
        HI_U32 ESCK             :1; //[5]
        HI_U32 STB_RootKey      :1; //[6]
        HI_U32 MISC_RootKey     :1; //[7]
        HI_U32 HDCP_RootKey     :1; //[8]
        HI_U32 OEM_RootKey      :1; //[9]
        HI_U32 SecureCPU_PSWD   :1; //[10]
        HI_U32 reserved         :21; //[11~31]
    }bits;
    HI_U32 u32;
}HI_UNF_ADVCA_CHECKSUM_FLAG_U;

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __DRV_ADVCA_IOCTL_H__ */
