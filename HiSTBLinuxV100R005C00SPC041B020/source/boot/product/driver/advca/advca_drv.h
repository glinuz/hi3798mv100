/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : advca_drv.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2015-12-25
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __ADVCA_DRV_H__
#define __ADVCA_DRV_H__

#include "hi_type.h"

#if defined (CHIP_TYPE_hi3716mv310)
#include "hi_boot_common.h"
#else
#include "hi_common.h"
#endif
#include "hi_unf_cipher.h"
#include "hi_unf_advca.h"
#include "hi_error_mpi.h"

#ifdef HI_MINIBOOT_SUPPORT
#include "string.h"
#else
#include "common.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifdef  HI_ADVCA_FUNCTION_RELEASE
#define HI_ERR_CA(format, arg...)    
#define HI_INFO_CA(format, arg...)   
#else
#define HI_ERR_CA(fmt...)         HI_ERR_PRINT(HI_ID_CA, fmt)
#define HI_INFO_CA(fmt...)        HI_INFO_PRINT(HI_ID_CA, fmt)
#endif

#define ATE_STAGE

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_11 */
/** @{ */  /** <!-- [ADVCA] */



/** JTAG protect mode*/
typedef enum hiUNF_ADVCA_JTAG_MODE_E
{
    HI_UNF_ADVCA_JTAG_MODE_OPEN = 0,
    HI_UNF_ADVCA_JTAG_MODE_PROTECT,
    HI_UNF_ADVCA_JTAG_MODE_CLOSED,

    HI_UNF_ADVCA_JTAG_MODE_BUTT
}HI_UNF_ADVCA_JTAG_MODE_E;

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


typedef struct
{
    HI_BOOL                      OpenFlag;  //Flag for Keyladder usage
    HI_UNF_CIPHER_ALG_E          Alg;       //KeyLadder algorithm
    HI_UNF_ADVCA_SP_DSC_MODE_E   enDscMode;
    HI_UNF_ADVCA_KEYLADDER_LEV_E MaxLevel;  //Max level
    HI_U32                       Keyladder_Ready;     //Max level
    HI_U8                        SessionKey[16][16];
    HI_U32                       SessionKey_sec[16];
    HI_U8                        LastEvenKey[16];
    HI_U32                       LastEvenKey_secTime;
    HI_U8                        LastOddKey[16];
    HI_U32                       LastOddKey_secTime;
}CA_KEYLADDER_INFO_S;



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
        HI_U32 uart_disable          : 1;//0x05[2]
        HI_U32 pcie_disable          : 1;//0x05[3]
        HI_U32 usb_disable           : 1;//0x05[4]
        HI_U32 sata_disable          : 1;//0x05[5]
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

/** JTAG protect mode*/
typedef enum
{
    CA_OTP_KEY_LEV1 = 0,
    CA_OTP_KEY_LEV2,
    CA_OTP_KEY_LEV3,
    CA_OTP_KEY_LEV4,
    CA_OTP_KEY_LEV5,
    CA_OTP_KEY_LEV6,
    CA_OTP_KEY_LEV_BUTT
}CA_OTP_KEY_LEVEL_E;


/** CA vendor type*/
typedef enum
{
    CA_OTP_VENDOR_NONE  = 0x0,
    CA_OTP_VENDOR_NAGRA,
    CA_OTP_VENDOR_IRDETO,
    CA_OTP_VENDOR_CONAX,
    CA_OTP_VENDOR_NDS,
    CA_OTP_VENDOR_SUMA,
    CA_OTP_VENDOR_NOVEL,
    CA_OTP_VENDOR_VERIMATRIX,
    CA_OTP_VENDOR_CTI,
    CA_OTP_VENDOR_SAFEVIEW,
    CA_OTP_VERDOR_LATENSE,
    CA_OTP_VERDOR_SH_TELECOM,
    CA_OTP_VERDOR_VIACCESS,
    CA_OTP_VENDOR_BUTT
}CA_OTP_VENDOR_TYPE_E;

#define CA_OTP_V200_INTERNAL_PV_0                 (0x00)
#define CA_OTP_V200_INTERNAL_PV_1                 (0x04)

#define CA_OTP_V200_INTERNAL_CA_VENDOR_ID         (0xAC)

#define ca_rdReg(addr, val)  ((val) = (*(volatile HI_U32 *)(addr)))
#define ca_wtReg(addr, val)  ((*(volatile HI_U32 *)(addr)) = (val))

HI_S32 CA_Init(HI_VOID);
HI_S32 CA_DeInit(HI_VOID);

HI_S32 CA_GetR2RLadder(HI_UNF_ADVCA_KEYLADDER_LEV_E *psel);

HI_S32 CA_SetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_E type);

HI_S32 CA_DecryptR2R(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,HI_U32 *pDataIn,HI_U32 AddrID,HI_BOOL bEnOrDeCrypt);

HI_S32 CA_DecryptCipher(HI_U32 AddrID, HI_U32 *pDataIn);

HI_S32 CA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key);

HI_S32 CA_GetRSAKeyLockFlag(HI_UNF_ADVCA_OTP_ATTR_S *pstOtpFuseAttr);

HI_S32 CA_EnableSCSActive(HI_UNF_ADVCA_FLASH_TYPE_E enFlashType);

HI_S32 CA_GetSCSActiveFlag(HI_BOOL *pbSCSActiveFlag);

HI_S32 CA_GetBootMode(HI_UNF_ADVCA_FLASH_TYPE_E *penFlashType);

HI_S32 CA_SetRSAKeyLockFlag(HI_UNF_ADVCA_OTP_ATTR_S *pstOtpFuseAttr);

HI_S32 CA_EncryptSwpk(HI_U32 *pClearSwpk, HI_U32 *pEncryptSwpk);

HI_S32 CA_DecryptSwpk(HI_U32 AddrID, HI_U32 *pEncryptSwpk);

HI_S32 CA_MISCKeyLadder_Open(HI_VOID);

HI_S32 CA_MISCKeyLadder_Close(HI_VOID);

HI_S32 CA_SetMiscAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E enType);

HI_S32 CA_V300_GetMiscAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E *penType);

HI_S32 CA_SetMISCSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
