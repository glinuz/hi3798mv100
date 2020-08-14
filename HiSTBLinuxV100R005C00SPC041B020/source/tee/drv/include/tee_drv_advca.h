/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : tee_drv_advca.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef __DRV_ADVCA_H__
#define __DRV_ADVCA_H__

#include "hi_type.h"

#include "hi_tee_advca.h"
#include "hi_tee_cipher.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define  HI_ERR_CA_OPEN_ERR                         (HI_S32)(0x804E0001)
#define  HI_ERR_CA_CLOSE_ERR                        (HI_S32)(0x804E0002)
#define  HI_ERR_CA_NOT_INIT                         (HI_S32)(0x804E0003)
#define  HI_ERR_CA_COPY_DATA_ERR                    (HI_S32)(0x804E0004)
#define  HI_ERR_CA_INVALID_PARA                     (HI_S32)(0x804E0005)
#define  HI_ERR_CA_WAIT_TIMEOUT                     (HI_S32)(0x804E0006)
#define  HI_ERR_CA_CW_DECRYPT                       (HI_S32)(0x804E0007)
#define  HI_ERR_CA_R2R_DECRYPT                      (HI_S32)(0x804E0008)
#define  HI_ERR_CA_R2R_ENCRYPT                      (HI_S32)(0x804E0009)
#define  HI_ERR_CA_SETPARAM_AGAIN	                (HI_S32)(0x804E000A)
#define  HI_ERR_CA_DBG_INERR 		                (HI_S32)(0x804E000B)
#define  HI_ERR_CA_NOT_CONFIG                       (HI_S32)(0x804E000C)
#define  HI_ERR_CA_LPK_DECRYPT                      (HI_S32)(0x804E000D)
#define  HI_ERR_CA_NOT_SUPPORT                      (HI_S32)(0x804E000E)
#define  HI_ERR_CA_SWPK_ENCRYPT                     (HI_S32)(0x804E000F)
#define ATE_STAGE

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_11 */
/** @{ */  /** <!-- [ADVCA] */

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
    HI_TEE_ADVCA_KEYLADDER_LEV_E ladder;
    HI_U32                       pDin[4];
} CA_CRYPTPM_S;

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

typedef enum
{
    CA_VERSION_200,
    CA_VERSION_300,
    CA_VERSION_BUTT
}CA_VERSION_E;

typedef enum hiDRV_ADVCA_CA_TARGET_E
{
	DRV_ADVCA_CA_TARGET_DEMUX         = 0,
	DRV_ADVCA_CA_TARGET_MULTICIPHER,
}DRV_ADVCA_CA_TARGET_E;

typedef struct hiDRV_ADVCA_EXTFUNC_PARAM_S
{
    HI_TEE_CIPHER_CA_TYPE_E enCAType;
    HI_TEE_ADVCA_CA_TARGET_E enTarget;
    HI_U32 AddrID;
    HI_U32 EvenOrOdd;
    HI_U8 *pu8Data;
    HI_BOOL bIsDeCrypt;
}DRV_ADVCA_EXTFUNC_PARAM_S;


typedef enum
{
    CA_OTP_VENDOR_NONE          = 0x00,
    CA_OTP_VENDOR_NAGRA         = 0x01,
    CA_OTP_VENDOR_IRDETO        = 0x02,
    CA_OTP_VENDOR_CONAX         = 0x03,
    CA_OTP_VENDOR_NDS           = 0x04,
    CA_OTP_VENDOR_SUMA          = 0x05,
    CA_OTP_VENDOR_NOVEL         = 0x06,
    CA_OTP_VENDOR_VERIMATRIX    = 0x07,
    CA_OTP_VENDOR_CTI           = 0x08,
    CA_OTP_VENDOR_SAFEVIEW      = 0x09,
    CA_OTP_VENDOR_LATENSE       = 0x0a,
    CA_OTP_VENDOR_COMMONCA      = 0x0b,
    CA_OTP_VENDOR_DCAS          = 0x0c,
    CA_OTP_VENDOR_PANACCESS     = 0x0e,
    CA_OTP_VENDOR_BUTT
}CA_OTP_VENDOR_TYPE_E;


/***************************** Macro Definition ******************************/
/* CA_MAILBOX */
#define CA_BASE_ADDR 0x10000000
#define CA_CRG_ADDR  0x101F50B4

#define CA_V200_SECURE_BOOT_CFG_ADD   (CA_BASE_ADDR + 0x00)
#define CA_V200_SECURE_BOOT_STAT_ADD  (CA_BASE_ADDR + 0x04)
#define CA_V200_MARKET_SID_REF_ADD    (CA_BASE_ADDR + 0x08)
#define CA_V200_CHIP_ID_ADD           (CA_BASE_ADDR + 0x0c)
#define CA_V200_STB_SN_ADD            (CA_BASE_ADDR + 0x10)
#define CA_V200_DIN0_ADD              (CA_BASE_ADDR + 0x20)
#define CA_V200_DIN1_ADD              (CA_BASE_ADDR + 0x24)
#define CA_V200_DIN2_ADD              (CA_BASE_ADDR + 0x28)
#define CA_V200_DIN3_ADD              (CA_BASE_ADDR + 0x2c)
#define CA_V200_CTRL_ADD              (CA_BASE_ADDR + 0x30)
#define CA_V200_SET_ADD               (CA_BASE_ADDR + 0x34)
#define CA_V200_STAT_ADD              (CA_BASE_ADDR + 0x38)
#define CA_V200_INT_STAT_ADD          (CA_BASE_ADDR + 0x40)
#define CA_V200_INT_EN_ADD            (CA_BASE_ADDR + 0x44)
#define CA_V200_INT_RAW_ADD           (CA_BASE_ADDR + 0x48)
#define CA_V200_LOAD_OTP_ADD          (CA_BASE_ADDR + 0x4c)
#define CA_V200_RST_ST_ADD            (CA_BASE_ADDR + 0x50)
#define CA_V200_DOUT0_ADD             (CA_BASE_ADDR + 0x60)
#define CA_V200_DOUT1_ADD             (CA_BASE_ADDR + 0x64)
#define CA_V200_DOUT2_ADD             (CA_BASE_ADDR + 0x68)
#define CA_V200_DOUT3_ADD             (CA_BASE_ADDR + 0x6c)
#define CA_V200_JTAG_KEY0_ADD         (CA_BASE_ADDR + 0x70)
#define CA_V200_JTAG_KEY1_ADD         (CA_BASE_ADDR + 0x74)
#define CA_V200_CTRL_PROC_ADD         (CA_BASE_ADDR + 0x78)
#define CA_V200_PRM_OTP_ADD           (CA_BASE_ADDR + 0x7c)
#define CA_V200_DVB_ROOT_KEY0_ADD     (CA_BASE_ADDR + 0x80)
#define CA_V200_DVB_ROOT_KEY1_ADD     (CA_BASE_ADDR + 0x84)
#define CA_V200_DVB_ROOT_KEY2_ADD     (CA_BASE_ADDR + 0x88)
#define CA_V200_DVB_ROOT_KEY3_ADD     (CA_BASE_ADDR + 0x8C)
#define CA_V200_R2R_ROOT_KEY0_ADD     (CA_BASE_ADDR + 0x90)
#define CA_V200_R2R_ROOT_KEY1_ADD     (CA_BASE_ADDR + 0x94)
#define CA_V200_R2R_ROOT_KEY2_ADD     (CA_BASE_ADDR + 0x98)
#define CA_V200_R2R_ROOT_KEY3_ADD     (CA_BASE_ADDR + 0x9C)
#define CA_V200_LP_SET_ADD            (CA_BASE_ADDR + 0xa0)
#define CA_V200_DEBUG_INFO_ADD        (CA_BASE_ADDR + 0xa4)
#define CA_V200_LPK_RDY_ADD           (CA_BASE_ADDR + 0xa8)
#define CA_V200_LP_PARAMETER_0_ADD    (CA_BASE_ADDR + 0xb0)
#define CA_V200_LP_PARAMETER_1_ADD    (CA_BASE_ADDR + 0xb4)
#define CA_V200_LP_PARAMETER_2_ADD    (CA_BASE_ADDR + 0xb8)
#define CA_V200_LP_PARAMETER_3_ADD    (CA_BASE_ADDR + 0xbc)
#define CA_V200_RSV0_INFO_ADD         (CA_BASE_ADDR + 0xc0)
#define CA_V200_RSV1_INFO_ADD         (CA_BASE_ADDR + 0xc4)
#define CA_V200_RSV2_INFO_ADD         (CA_BASE_ADDR + 0xc8)
#define CA_V200_RSV3_INFO_ADD         (CA_BASE_ADDR + 0xcc)
#define CA_V200_VERSION_ADD           (CA_BASE_ADDR + 0xd0)




#define HI_ERR_ADVCA(format, arg...)  uart_printf_func("<<<ADVCA ERR>>> %s %d: " format, __FUNCTION__, __LINE__, ## arg)
#define HI_DEBUG_ADVCA(format, arg...) uart_printf_func("<<<ADVCA DEBUG>>> %s %d: ", format, __FUNCTION__, __LINE__, ## arg)

#define CA_CheckPointer(p) \
    do {  \
        if (HI_NULL == p)\
        {\
            HI_ERR_ADVCA("pointer parameter is NULL.\n"); \
            return -1; \
        } \
    } while (0)


#define ca_rdReg(addr, val)  ((val) = (*(volatile HI_U32 *)(addr)))
#define ca_wtReg(addr, val)  ((*(volatile HI_U32 *)(addr)) = (val))

HI_S32 DRV_ADVCA_Init(HI_VOID);

HI_S32 DRV_ADVCA_DeInit(HI_VOID);

HI_S32 DRV_ADVCA_GetOtpFuse(HI_TEE_ADVCA_OTP_FUSE_E enOtpFuse, HI_TEE_ADVCA_OTP_ATTR_S *pstOtpFuseAttr);


HI_S32 DRV_ADVCA_SetOtpFuse(HI_TEE_ADVCA_OTP_FUSE_E enOtpFuse, HI_TEE_ADVCA_OTP_ATTR_S *pstOtpFuseAttr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
