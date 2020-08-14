/******************************************************************************

Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_unf_advca.h
Version       : Initial
Author        : Hisilicon hisecurity team
Created       : 2013-08-28
Last Modified :
Description   : Hisilicon CA API declaration
Function List :
History       :
******************************************************************************/
#ifndef __HI_UNF_ADVCA_H__
#define __HI_UNF_ADVCA_H__

#include "hi_type.h"
#include "hi_unf_cipher.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      ADVCA */
/** @{ */  /** <!-- [ADVCA] */
#define MAX_URI_NUM         (2048)
#define MAX_FP_ID_LENGTH    (0x100)

/** advanced CA session serect key class*/
typedef enum hiUNF_ADVCA_KEYLADDER_LEV_E
{
	HI_UNF_ADVCA_KEYLADDER_LEV1     = 0,    /**<session serect key level 1*/
	HI_UNF_ADVCA_KEYLADDER_LEV2 ,	        /**<session serect key level 2*/
	HI_UNF_ADVCA_KEYLADDER_LEV3 ,	        /**<session serect key level 3*/
	HI_UNF_ADVCA_KEYLADDER_LEV4,			/**<session serect key level 4*/
	HI_UNF_ADVCA_KEYLADDER_LEV5,			/**<session serect key level 5*/
	HI_UNF_ADVCA_KEYLADDER_LEV6,			/**<session serect key level 6*/
	HI_UNF_ADVCA_KEYLADDER_BUTT
}HI_UNF_ADVCA_KEYLADDER_LEV_E;

/** advanced CA session keyladder target */
typedef enum hiUNF_ADVCA_CA_TARGET_E
{
	HI_UNF_ADVCA_CA_TARGET_DEMUX         = 0, /**<demux*/
	HI_UNF_ADVCA_CA_TARGET_MULTICIPHER,	      /**<multicipher*/
	HI_UNF_ADVCA_CA_TARGET_BUTT
}HI_UNF_ADVCA_CA_TARGET_E;

/** advanced CA Encrypt arith*/
typedef enum hiUNF_ADVCA_ALG_TYPE_E
{
    HI_UNF_ADVCA_ALG_TYPE_TDES      = 0,    /**<Encrypt arith :3 DES*/
    HI_UNF_ADVCA_ALG_TYPE_AES,              /**<Encrypt arith : AES*/
    HI_UNF_ADVCA_ALG_TYPE_BUTT
}HI_UNF_ADVCA_ALG_TYPE_E;

/** FLASH device types*/
typedef enum hiUNF_ADVCA_FLASH_TYPE_E
{
    HI_UNF_ADVCA_FLASH_TYPE_SPI     = 0,    /**<SPI flash*/
    HI_UNF_ADVCA_FLASH_TYPE_NAND ,          /**<nand flash*/
    HI_UNF_ADVCA_FLASH_TYPE_NOR ,           /**<nor flash*/
    HI_UNF_ADVCA_FLASH_TYPE_EMMC ,          /**<eMMC*/
    HI_UNF_ADVCA_FLASH_TYPE_SPI_NAND ,      /**<spi_nand flash*/
    HI_UNF_ADVCA_FLASH_TYPE_SD,             /**<FSD/TSD flash*/
    HI_UNF_ADVCA_FLASH_TYPE_BUTT
}HI_UNF_ADVCA_FLASH_TYPE_E;

/** JTAG protect mode*/
typedef enum hiUNF_ADVCA_JTAG_MODE_E
{
    HI_UNF_ADVCA_JTAG_MODE_OPEN     = 0,
    HI_UNF_ADVCA_JTAG_MODE_PROTECT,
    HI_UNF_ADVCA_JTAG_MODE_CLOSED,
    HI_UNF_ADVCA_JTAG_MODE_BUTT
}HI_UNF_ADVCA_JTAG_MODE_E;

/** =======================The following definition is for ADVCA PVR==========================================*/
/** Function pointer for setting CA private data when running PVR*/
typedef HI_S32 (*fpSetCAData)(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32CADataLen);
/** Function pointer for getting CA private data when running PVR*/
typedef HI_S32 (*fpGetCAData)(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32BufLen, HI_U32* u32CADataLen);
/** Parameter structure of API HI_UNF_ADVCA_PVR_WriteCallBack and HI_UNF_ADVCA_PVR_ReadCallBack*/
typedef struct hiUNF_PVR_CA_CALLBACK_ARGS_S
{
    HI_U32 u32ChnID;                /*channel ID of PVR*/
    HI_CHAR *pFileName;      /* index file name of recorded stream*/
    HI_U64 u64GlobalOffset;         /*Global Offset of the recording stream*/
    HI_U32 u32PhyAddr;		        /*physical address of the recording stream*/
    HI_U32 u32DataSize;             /*size of the the recording stream */ 
}HI_UNF_PVR_CA_CALLBACK_ARGS_S;


/*pay attention: centi_time is a 24bit value 
according to <basic specification 7.1> 
hour(bit 23-19),
minute(bit 18-13),
second(bit 12-7),
centi-second(bit 6-0),
and if fptime in fingerprint = 0xffffff,, which means
( u8Hour = 31,u8Minute = 63,u8Second = 63,u8Centisecond = 127) should show the fingerpring immediately*/
typedef struct HiCENTI_TIME_S
{
	HI_U8 u8Hour;			
	HI_U8 u8Minute;		
	HI_U8 u8Second;		
	HI_U8 u8Centisecond;	
}HI_CENTI_TIME_S;

typedef struct HiFP_ID_S
{
    HI_U16 u16DataLength;
    HI_U8 u8ID[MAX_FP_ID_LENGTH];	
}HI_FP_ID_S;

typedef struct HiFP_S
{
	HI_CENTI_TIME_S stCentiTime;
	HI_U16 u16Duration;            //in 0.01 second steps,for example: u16Duration = 2 means  20ms
	HI_U16 u16Position_X;
	HI_U16 u16Position_Y;
	HI_U8 u8Height;
}HI_FP_S;

typedef struct HiUNF_PVR_FP_INFO_S
{
    HI_U32 u32DisplayOffsetTime;
    HI_BOOL bisFromLPData;
    HI_FP_ID_S stFPID;  
    HI_FP_S stFP;
}HI_UNF_PVR_FP_INFO_S;


/** Maturity of program*/
typedef enum hiMATURITY_RATING_S
{
	EN_RAT_G = 0x01,                               /**for the family*/
	EN_RAT_PG = 0x02,                              /**with parental guidance*/
	EN_RAT_A = 0x04,                               /**over 18*/
	EN_RAT_X = 0x08,                               /**erotic*/
    EN_RAT_RESERVE,              
}HI_MATURITY_RATING_E;

/** Recorded stream information */
typedef struct hiUNF_PVR_CA_StreamInfo_S
{
    HI_U32 u32OffsetTime;
    HI_U8 Reserved[16];                   /* Record URI private data */
}HI_UNF_PVR_URI_S;

typedef struct
{
    HI_S32 s32Year; /**<The number of years since A.D. 0.,eg.2009*/
    HI_S32 s32Mon;  /**<The number of months since January, in the range 1 to 12.*/
    HI_S32 s32Mday; /**<The day of the month, in the range 1 to 31.*/
    HI_S32 s32Hour; /**<The number of hours past midnight, in the range 0 to 23.*/
    HI_S32 s32Min;  /**<The number of minutes after the hour, in the range 0 to 59.*/
    HI_S32 s32Sec;  /**<The  number of seconds after the minute, normally in the range 0 to 59, but can be up to 60 to allow for leap seconds.*/  
} HI_UNF_ADVCA_Time_S;

/**Status of a recordng channel status*//** CNcomment:录制通道的状态 */
typedef struct hiUNF_ADVCA_PVR_REC_STATUS_S
{
    HI_U32  u32UriNumbers;
    HI_U32  u32FpNumbers;
}HI_UNF_ADVCA_PVR_REC_STATUS_S;

typedef struct hiUNF_ADVCA_PVR_PlAY_INFOR_S
{
   HI_BOOL  bIsTimeshiftPlay;
   HI_U32   u32RecChnID;      /*only valid when bIsTimeshiftPlay is true*/
}HI_UNF_ADVCA_PVR_PlAY_INFOR_S;


typedef struct HiUNF_ADVCA_LOCK_TZ_OTP_PARAM
{
	HI_U32 u32Addr;
	HI_U32 u32Len;
}HI_UNF_ADVCA_LOCK_TZ_OTP_PARAM_S;

typedef enum hiUNF_ADVCA_SP_DSC_MODE_E
{
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_CSA2      		= 0x0000,   /**<Demux PAYLOAD CSA2 */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_CSA3      		= 0x0010,   /**<Demux PAYLOAD CSA3 */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_AES_CBC_IDSA 		= 0x0020,   /**<Demux PAYLOAD AES  IPTV Mode */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_AES_ECB          	= 0x0021,   /**<Demux PAYLOAD AES  ECB Mode */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_AES_CBC_CI     	= 0x0022,   /**<Demux PAYLOAD AES  CIPLUS */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_DES_IPTV      	= 0x0030,   /**<Demux PAYLOAD DES  CIPLUS */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_DES_CIPLUS     	= 0x0032,   /**<Demux PAYLOAD DES  CIPLUS */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_DES_CBC       	= 0x0033,   /**<Demux PAYLOAD DES  CBC */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_AES_NOVEL       	= 0x0040,   /**<Demux PAYLOAD AES  NOVEL */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_SMS4_NOVEL       	= 0x0041,   /**<Demux PAYLOAD SMS4 NOVEL */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_SMS4_IPTV       	= 0x0050,   /**<Demux PAYLOAD SMS4 IPTV */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_SMS4_ECB       	= 0x0051,   /**<Demux PAYLOAD SMS4 ECB */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_SMS4_CBC       	= 0x0053,   /**<Demux PAYLOAD SMS4 CBC */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_AES_CBC       	= 0x0063,   /**<Demux PAYLOAD AES  CBC */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_TDES_IPTV       	= 0x0070,   /**<Demux PAYLOAD TDES IPTV */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_TDES_ECB       	= 0x0071,   /**<Demux PAYLOAD TDES ECB */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_TDES_CBC      	= 0x0073,   /**<Demux PAYLOAD TDES CBC */
     HI_UNF_ADVCA_SP_DSC_MODE_RAW_AES_CBC               = 0x4020,   /**<MultiCipher AES CBC */
     HI_UNF_ADVCA_SP_DSC_MODE_RAW_AES_ECB               = 0x4021,   /**<MultiCipher AES ECB */
     HI_UNF_ADVCA_SP_DSC_MODE_RAW_AES_CBC_PIFF        	= 0x4022,   /**<MultiCipher AES CBC PIFF */
     HI_UNF_ADVCA_SP_DSC_MODE_RAW_AES_CBC_APPLE    		= 0x4023,   /**<MultiCipher AES CBC APPLE */
     HI_UNF_ADVCA_SP_DSC_MODE_RAW_AES_CTR               = 0x4024,   /**<MultiCipher AES CTR */
     HI_UNF_ADVCA_SP_DSC_MODE_RAW_TDES_CBC              = 0x4040,   /**<MultiCipher AES CBC */
     HI_UNF_ADVCA_SP_DSC_MODE_RAW_TDES_ECB              = 0x4041,   /**<MultiCipher AES ECB */
     HI_UNF_ADVCA_SP_DSC_MODE_BUTT
}HI_UNF_ADVCA_SP_DSC_MODE_E;

/** Advca CA VendorID */
typedef enum hiUNF_ADVCA_VENDORID_E
{
    HI_UNF_ADVCA_NULL       = 0x00,        /**<No-Advcance CA chipset, Marked with 0*/
    HI_UNF_ADVCA_NAGRA      = 0x01,        /**<NAGRA  Chipse, Marked with R*/
    HI_UNF_ADVCA_IRDETO     = 0x02,        /**<IRDETO Chipset, Marked with I*/
    HI_UNF_ADVCA_CONAX      = 0x03,        /**<CONAX Chipset, Marked with C*/
    HI_UNF_ADVCA_SUMA       = 0x05,        /**<SUMA Chipset, Marked with S*/
    HI_UNF_ADVCA_NOVEL      = 0x06,        /**<NOVEL Chipset, Marked with Y*/
    HI_UNF_ADVCA_VERIMATRIX = 0x07,        /**<VERIMATRIX Chipset, Marked with M*/
    HI_UNF_ADVCA_CTI        = 0x08,        /**<CTI Chipset, Marked with T*/
    HI_UNF_ADVCA_COMMONCA   = 0x0b,        /**<COMMONCA Chipset, Marked with H*/
    HI_UNF_ADVCA_DCAS       = 0x0c,        /**<DCAS CA Chipset*/
    HI_UNF_ADVCA_PANACCESS  = 0x0e,        /**<PANACCESS CA Chipset*/
    HI_UNF_ADVCA_VENDORIDE_BUTT
}HI_UNF_ADVCA_VENDORID_E;

/** Advca CA lock type */
typedef enum hiUNF_ADVCA_LOCK_TYPE
{
	HI_UNF_ADVCA_LOCK_RSA_KEY = 0,
	HI_UNF_ADVCA_LOCK_TZ_OTP,
	HI_UNF_ADVCA_LOCK_MISC_KL_DISABLE,
	HI_UNF_ADVCA_LOCK_GG_KL_DISABLE,
	HI_UNF_ADVCA_LOCK_TSKL_CSA3_DISABLE,
	HI_UNF_ADVCA_LOCK_BUTT,
}HI_UNF_ADVCA_LOCK_TYPE_E;


/** DCAS KeyLadder use mode */
typedef enum hiUNF_ADVCA_DCAS_KEYLADDER_LEV_E
{
	HI_UNF_ADVCA_DCAS_KEYLADDER_EK2 		= 0,   	/**<session EK2*/
	HI_UNF_ADVCA_DCAS_KEYLADDER_EK1 		= 1,	/**<session EK1*/
	HI_UNF_ADVCA_DCAS_KEYLADDER_VENDORSYSID = 8,	/**<session Vendor_SysID*/
	HI_UNF_ADVCA_DCAS_KEYLADDER_NONCE 		= 9,	/**<session nonce*/
	HI_UNF_ADVCA_DCAS_KEYLADDER_BUTT
}HI_UNF_ADVCA_DCAS_KEYLADDER_LEV_E;

/**Defines the attribute ID of the OTP fuse. Please be kindly noted that the OTP fuse is set permanently.
After setting the OTP fuse, please power off and then power on the STB to make the OTP fuse function effective */
/**CNcomment: 定义OTP位ID枚举类型. 请注意OTP位设置后不可改变.
设置后，请断电重启使OTP位功能生效.*/
typedef enum hiUNF_ADVCA_OTP_FUSE_E
{
    HI_UNF_ADVCA_OTP_NULL = 0,
    HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION, /**<Whether to enable the secure boot authentication, set the boot flash type together, HI_UNF_ADVCA_OTP_SECURE_BOOT_ATTR_S*//**<CNcomment: 安全启动是否开启，同时设置安全启动flash类型, HI_UNF_ADVCA_OTP_SECURE_BOOT_ATTR_S*/
    HI_UNF_ADVCA_OTP_BOOT_DECRYPTION_ACTIVATION, /**<Whether to enable the secure boot decryption, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: boot解密功能是否开启, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_SELF_BOOT_DEACTIVATION, /**<Whether to disable the self boot, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: 自举是否关闭, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_DDR_WAKEUP_DEACTIVATION, /**<Whether to disable the DDR wakeup, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: 待机原地唤醒是否关闭 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_CSA2_KL_LEVEL_SEL, /**<CSA2 keyladder stage, HI_UNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S*//**<CNcomment: CSA2 keyladder级数 , HI_UNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S*/
    HI_UNF_ADVCA_OTP_R2R_KL_LEVEL_SEL, /**<R2R keyladder stage, HI_UNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S*//**<CNcomment: R2R keyladder级数 , HI_UNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S*/
    HI_UNF_ADVCA_OTP_SP_KL_LEVEL_SEL, /**<SP keyladder stage, HI_UNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S*//**<CNcomment: SP keyladder级数 , HI_UNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S*/
    HI_UNF_ADVCA_OTP_CSA3_KL_LEVEL_SEL, /**<CSA3 keyladder stage, HI_UNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S*//**<CNcomment: CSA3 keyladder级数 , HI_UNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S*/
    HI_UNF_ADVCA_OTP_LP_DEACTIVATION, /**<Whether to disable the LP keyladder, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: LP keyladder是否关闭 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_CSA2_CW_HARDONLY_ACTIVATION, /**<Whether to enable the CSA2 hard CW only, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: CSA2硬CW是否使能 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_SP_CW_HARDONLY_ACTIVATION, /**<Whether to enable the SP hard CW only, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: SP硬CW是否使能 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_CSA3_CW_HARDONLY_ACTIVATION, /**<Whether to enable the CSA3 hard CW only, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: CSA3硬CW是否使能 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_CSA2_KL_DEACTIVATION, /**<Whether to disable the CSA2 keyladder crypto engine, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: CSA2 keyladder解密引擎是否关闭 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_SP_KL_DEACTIVATION, /**<Whether to disable the SP keyladder crypto engine, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: SP keyladder解密引擎是否关闭 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_CSA3_KL_DEACTIVATION, /**<Whether to disable the CSA3 keyladder crypto engine, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: CSA3 keyladder解密引擎是否关闭, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_MISC_KL_DEACTIVATION, /**<Whether to disable the MISC keyladder crypto engine, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: MISC keyladder解密引擎是否关闭 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_GOOGLE_KL_DEACTIVATION, /**<Whether to disable the google keyladder crypto engine, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: Google keyladder解密引擎是否关闭, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_DCAS_KL_DEACTIVATION, /**<Whether to disable the DCAS keyladder crypto engine, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: DCAS keyladder解密引擎是否关闭 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/   
    HI_UNF_ADVCA_OTP_DDR_SCRAMBLE_ACTIVATION, /**<Whether to enable the DDR scrambling, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: DDR加扰是否开启 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_GLOBAL_LOCK_ACTIVATION, /**<Whether to lock the whole OTP area, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: OTP写保护是否开启 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_RUNTIME_CHECK_ACTIVATION, /**<Whether to enable the runtime check, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: Runtime check是否开启 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_DDR_WAKEUP_CHECK_ACTIVATION, /**<Whether to enable the DDR check when wakeup from standby, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: 待机原地唤醒时DDR校验是否开启 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_VERSION_ID_CHECK_ACTIVATION, /**<Whether to enable the version id check, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: Version ID校验是否开启 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_BOOT_MSID_CHECK_ACTIVATION, /**<Whether to enable the boot MSID check, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: Boot MSID校验是否开启 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_JTAG_MODE, /**<JTAG protected mode, HI_UNF_ADVCA_JTAG_PRT_MODE_ATTR_S*//**<CNcomment: JTAG保护模式 , HI_UNF_ADVCA_JTAG_PRT_MODE_ATTR_S*/
    HI_UNF_ADVCA_OTP_JTAG_READ_DEACTIVATION, /**<Whether to disable to read OTP via JTAG, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: 通过JTAG读取OTP是否关闭 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_CSA2_ROOTKEY, /**<CSA2 keyladder root key, HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*//**<CNcomment: CSA2 keyladder根密钥 , HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*/
    HI_UNF_ADVCA_OTP_R2R_ROOTKEY, /**<R2R keyladder root key, HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*//**<CNcomment: R2R keyladder根密钥 , HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*/
    HI_UNF_ADVCA_OTP_SP_ROOTKEY, /**<SP keyladder root key, HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*//**<CNcomment: SP keyladder根密钥 , HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*/
    HI_UNF_ADVCA_OTP_CSA3_ROOTKEY, /**<CSA3 keyladder root key, HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*//**<CNcomment: CSA3 keyladder根密钥 , HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*/
    HI_UNF_ADVCA_OTP_MISC_ROOTKEY, /**<MISC keyladder root key, HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*//**<CNcomment: MISC keyladder根密钥 , HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*/
    HI_UNF_ADVCA_OTP_OEM_ROOTKEY, /**<OEM keyladder root key, HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*//**<CNcomment: OEM keyladder根密钥 , HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*/
    HI_UNF_ADVCA_OTP_ESCK_ROOTKEY, /**<ESCK keyladder root key, HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*//**<CNcomment: ESCK keyladder根密钥 , HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*/
    HI_UNF_ADVCA_OTP_JTAG_KEY, /**<JTAG key, HI_UNF_ADVCA_JTAG_KEY_ATTR_S*//**<CNcomment: JTAG密钥 , HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*/
    HI_UNF_ADVCA_OTP_CHIP_ID, /**<Chip ID, HI_UNF_ADVCA_CHIPID_ATTR_S*//**<CNcomment: Chip ID, HI_UNF_ADVCA_JTAG_KEY_ATTR_S*/
    HI_UNF_ADVCA_OTP_MARKET_SEGMENT_ID, /**<Market segment ID, HI_UNF_ADVCA_MARKET_ID_ATTR_S*//**<CNcomment: Market segment ID , HI_UNF_ADVCA_MARKET_ID_ATTR_S*/
    HI_UNF_ADVCA_OTP_VERSION_ID, /**<Version ID, HI_UNF_ADVCA_VERSION_ID_ATTR_S*//**<CNcomment: Version ID, HI_UNF_ADVCA_VERSION_ID_ATTR_S*/
    HI_UNF_ADVCA_OTP_MISC_KL_LEVEL_SEL, /**<MISC keyladder stage, HI_UNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S*//**<CNcomment: MISC keyladder级数 , HI_UNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S*/
    HI_UNF_ADVCA_OTP_VMX_BL_FUSE, /**<Burn verimatrix bootloader fuse*//**<CNcomment: 烧写verimatrix特定的bootloader fuse位*/
    HI_UNF_ADVCA_OTP_IRDETO_ITCSA3_ACTIVATION, /**<Whether to enbale the irdeto tweaked CSA3, it is for Irdeto MSR2.2 or higher chipset, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: Irdeto私有CSA3算法是否开启, 只有支持Irdeto MSR2.2以上标准的芯片才支持。HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_BOOTINFO_DEACTIVATION, /**<Whether to disable the bootinfo, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: Bootinfo是否关闭, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_ITCSA3_IMLB, /**<Irdeto MSR2.2 tweaked CSA3 IMLB, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: Irdeto私有CSA3算法IMLB,HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/    
    HI_UNF_ADVCA_OTP_USB_DEACTIVATION, /**<Whether to disable the usb host, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: 禁止USB设备, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_SERIAL_DEACTIVATION, /**<Whether to disable serial port, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: 禁止串口功能, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_ETHERNET_DEACTIVATION, /**<Whether to disable the ethernet, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: 禁止网络, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_SM4_CRYPTO_ENGINE_DEACTIVATION, /**<Whether to disable the SM4 crypto engine, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: SM4解密引擎是否关闭 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_TDES_CRYPTO_ENGINE_DEACTIVATION, /**<Whether to disable the TDES crypto engine, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: TDES解密引擎是否关闭 , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/

    HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION_ONLY,   /**<Enable secure boot authentication only, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*//**<CNcomment: 安全启动使能， HI_UNF_ADVCA_OTP_SECURE_BOOT_ATTR_S*/
    HI_UNF_ADVCA_OTP_BOOT_FLASH_TYPE,   /**<Boot flash type, HI_UNF_ADVCA_OTP_BOOT_FLASH_TYPE_ATTR_S*//**<CNcomment: 安全启动flash类型， HI_UNF_ADVCA_OTP_BOOT_FLASH_TYPE_ATTR_S*/
    
    HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG,   /**< Get the lock flag of RSA_Root_key, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/ /**<CNcomment: 获取RSA_Root_Key lock标记，HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/     
    HI_UNF_ADVCA_OTP_STBSN_LOCK_FLAG,   /**< Get the lock flag of STBSN, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/ /**<CNcomment: 获取STBSN lock标记，HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/     
    HI_UNF_ADVCA_OTP_MSID_LOCK_FLAG,   /**< Get the lock flag of MSID, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/ /**<CNcomment: 获取MSID lock标记，HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/     
    HI_UNF_ADVCA_OTP_VERSIONID_LOCK_FLAG,   /**< Get the lock flag of VersionID, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/ /**<CNcomment: 获取VersionID lock标记，HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/     
    HI_UNF_ADVCA_OTP_OEM_ROOTKEY_LOCK_FLAG,   /**< Get the lock flag of OEM_Root_key, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/ /**<CNcomment: 获取OEM_Root_key lock标记，HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/     
    HI_UNF_ADVCA_OTP_R2R_ROOTKEY_LOCK_FLAG,   /**< Get the lock flag of R2R_Root_key, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/ /**<CNcomment: 获取R2R_Root_key lock标记，HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/     
    HI_UNF_ADVCA_OTP_JTAG_KEY_LOCK_FLAG,   /**< Get the lock flag of JTAG key, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/ /**<CNcomment: 获取JTAG key lock标记，HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/     
    HI_UNF_ADVCA_OTP_TZ_AREA_LOCK_FLAG,   /**< Get the lock flag of OTP trust area, when otp_tz_area_enable is set, these lock flags can only be accessed by secure cpu , HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/ /**<CNcomment: 获取OTP trust area lock标记，当otp_tz_area_enable被设置时，这些标记只能被安全CPU读取，HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_BOOT_ENCRYPTION_DEACTIVATION, /**< whether to diable boot loader key generation, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/ /**<CNcomment: 生成bootloader key功能是否关闭，HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/
    HI_UNF_ADVCA_OTP_BOOT_ROOTKEY,        /**<Boot keyladder root key, HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*//**<CNcomment: boot keyladder根密钥 , HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*/
    HI_UNF_ADVCA_OTP_SEC_STORE_ROOTKEY,   /**<Sec store keyladder root key, HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*//**<CNcomment: Sec store keyladder根密钥 , HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S*/
    HI_UNF_ADVCA_OTP_BOOT_ROOTKEY_LOCK_FLAG, /**< Get the lock flag of BOOT_Root_key, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/ /**<CNcomment: 获取BOOT_Root_Key lock标记，HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/     
    HI_UNF_ADVCA_OTP_SEC_STORE_ROOTKEY_LOCK_FLAG, /**< Get the lock flag of SEC_STORE_Root_key, HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/ /**<CNcomment: 获取SEC_STORE_Root_Key lock标记，HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S*/     
    HI_UNF_ADVCA_OTP_FUSE_BUTT
}HI_UNF_ADVCA_OTP_FUSE_E;

/**Defines the attribute of secure boot check.*/
/**CNcomment: 定义安全启动校验属性结构体*/
typedef struct hiUNF_ADVCA_OTP_SECURE_BOOT_ATTR_S
{
    HI_BOOL bEnable;    /**<Is secure boot check enable or not*/
    HI_UNF_ADVCA_FLASH_TYPE_E enFlashType; /**<Boot flash type*/
}HI_UNF_ADVCA_OTP_SECURE_BOOT_ATTR_S;

/**Defines the boot flash type.*/
/**CNcomment: 定义安全启动flash类型结构体*/
typedef struct hiUNF_ADVCA_OTP_BOOT_FLASH_TYPE_ATTR_S
{
    HI_BOOL bBootSelCtrl;    /**<0--the boot flash type is defined by chipset pin, 1--the boot flash type is defined by OTP value*/
    HI_UNF_ADVCA_FLASH_TYPE_E enFlashType; /**<Boot flash type, only valid when bBootSelCtrl is 1*/
}HI_UNF_ADVCA_OTP_BOOT_FLASH_TYPE_ATTR_S;

/**Defines the default attribute of OTP fuse. If one OTP fuse can ONLY be "Enable" or "Disable",
then we will use this attribute.*/
/**CNcomment: 定义OTP位默认属性结构体. 适用于一个OTP位的功能
只是开启或关闭的情况*/
typedef struct hiUNF_ADVCA_OTP_DEFAULT_ATTR_S
{
    HI_BOOL bEnable;    /**<Is the feature enable or not*/
}HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S;

/**Defines the attribute of the keyladder level.*/
/**CNcomment: 定义keyladder级数属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S
{
    HI_UNF_ADVCA_KEYLADDER_LEV_E enKeyladderLevel;   /**<Keyladder level*/
}HI_UNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S;

/**Defines the attribute of the JTAG protected mode.*/
/**CNcomment: 定义JTAG接口保护属性结构体.*/
typedef struct hiUNF_ADVCA_JTAG_PRT_MODE_ATTR_S
{
    HI_UNF_ADVCA_JTAG_MODE_E enJtagMode;    /**<JTAG protection mode*/
}HI_UNF_ADVCA_JTAG_PRT_MODE_ATTR_S;

/**Defines the attribute of the keyladder root key.*/
/**CNcomment: 定义keyladder 根密钥属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S
{
    HI_U8 u8RootKey[16];    /**<Root key of the keyladder*/
}HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S;

/**Defines the attribute of the JTAG key.*/
/**CNcomment: 定义JTAG接口保护密钥属性结构体.*/
typedef struct hiUNF_ADVCA_JTAG_KEY_ATTR_S
{
    HI_U8 u8JtagKey[8];     /**<JTAG protected key*/
}HI_UNF_ADVCA_JTAG_KEY_ATTR_S;

/**Defines the attribute of the chip id.*/
/**CNcomment: 定义chip id属性结构体.*/
typedef struct hiUNF_ADVCA_CHIPID_ATTR_S
{
    HI_U8 au8ChipId[8];       /**<ChipID of the chipset, it is unique every chipset*/
}HI_UNF_ADVCA_CHIPID_ATTR_S;

/**Defines the attribute of the market segment id.*/
/**CNcomment: 定义market segment id属性结构体.*/
typedef struct hiUNF_ADVCA_MARKET_ID_ATTR_S
{
    HI_U8 u8MSID[4];    /**<Market Segment ID*/
}HI_UNF_ADVCA_MARKET_ID_ATTR_S;

/**Defines the attribute of the version id.*/
/**CNcomment: 定义version id属性结构体.*/
typedef struct hiUNF_ADVCA_VERSION_ID_ATTR_S
{
    HI_U8 u8VersionId[4];   /**<Bootloader Version ID*/
}HI_UNF_ADVCA_VERSION_ID_ATTR_S;

typedef struct hiUNF_ADVCA_VMX_BL_FUSE_ATTR_S
{
	HI_U32 u32VMXBLFuse;
}HI_UNF_ADVCA_VMX_BL_FUSE_S;

/**Defines the attribute of the Irdeto MSR2.2 tweaked CSA3 IMLB.*/
/**CNcomment: 定义Irdeto MSR2.2 tweaked CSA3 IMLB属性结构体.*/
typedef struct hiUNF_ADVCA_ITCSA3_IMLB_ATTR_S
{
    HI_U8 au8ItCsa3IMLB[2];   /**<Irdeto MSR2.2 tweaked CSA3 IMLB*/
}HI_UNF_ADVCA_ITCSA3_IMLB_ATTR_S;

/**Defines the attribute of the OTP fuse.*/
/**CNcomment: 定义OTP fuse属性结构体.*/
typedef struct hiUNF_ADVCA_OTP_ATTR_S
{
    union
    {
        HI_UNF_ADVCA_OTP_DEFAULT_ATTR_S          stDefaultAttr; /**<Default attribute, if one fuse can ONLY be "Enable" or "Disable", then we can use this attribute*/
        HI_UNF_ADVCA_OTP_SECURE_BOOT_ATTR_S      stEnableSecureBoot; /**<Secure boot attribute*/
        HI_UNF_ADVCA_KEYLADDER_LEVEL_SEL_ATTR_S  stKeyladderLevSel;  /**<Keyladder level attribute*/
        HI_UNF_ADVCA_JTAG_PRT_MODE_ATTR_S        stJtagPrtMode; /**<JTAG protected mode attribute*/
        HI_UNF_ADVCA_KEYLADDER_ROOTKEY_ATTR_S    stKeyladderRootKey; /**<Keyladder rootkey attribute*/
        HI_UNF_ADVCA_JTAG_KEY_ATTR_S             stJtagKey;   /**<JTAG key attribute*/
        HI_UNF_ADVCA_CHIPID_ATTR_S               stChipId;     /**<Chip ID attribute*/
        HI_UNF_ADVCA_MARKET_ID_ATTR_S            stMarketId;    /**<Market segment ID attribute*/
        HI_UNF_ADVCA_VERSION_ID_ATTR_S           stVersionId;  /**<Bootloadder version ID attribute*/
		HI_UNF_ADVCA_VMX_BL_FUSE_S				 stVMXBLFuse;  /**<Vmx Bootloadder specific fuse attribute*/
        HI_UNF_ADVCA_ITCSA3_IMLB_ATTR_S          stItCsa3IMLB; /**<Irdeto tweaked CSA3 IMLB attribute*/		
        HI_UNF_ADVCA_OTP_BOOT_FLASH_TYPE_ATTR_S  stBootFlashType;   /**<Boot flash type attribute*/
    }unOtpFuseAttr;
}HI_UNF_ADVCA_OTP_ATTR_S;

/**Defines the attribute of the keyladder type ID.*/
/**CNcomment: 定义keyladder类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_TYPE_E
{
    HI_UNF_ADVCA_KEYLADDER_CSA2 = 0,        /**<CSA2 keyladder*/
    HI_UNF_ADVCA_KEYLADDER_CSA3,            /**<CSA3 keyladder*/
    HI_UNF_ADVCA_KEYLADDER_R2R,             /**<R2R keyladder*/
    HI_UNF_ADVCA_KEYLADDER_SP,              /**<SP keyladder*/
    HI_UNF_ADVCA_KEYLADDER_MISC,            /**<MISC keyladder*/
    HI_UNF_ADVCA_KEYLADDER_LP,              /**<LP keyladder*/
    HI_UNF_ADVCA_KEYLADDER_TA,              /**<TA keyladder, only for Irdeto MSR2.2*/ 
    HI_UNF_ADVCA_KEYLADDER_GDRM,            /**<Google DRM key ladder*/
    HI_UNF_ADVCA_KEYLADDER_TYPE_BUTT
}HI_UNF_ADVCA_KEYLADDER_TYPE_E;

/**Defines the attribute of the CSA2 keyladder type ID.*/
/**CNcomment: 定义CSA2 keyladder属性类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_CSA2_ATTR_E
{
    HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_ALG = 0,            /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_SESSION_KEY,   /**<Keyladder session key*/
    HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_BUTT
}HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_E;

/**Defines the attribute of the CSA3 keyladder type ID.*/
/**CNcomment: 定义CSA3 keyladder属性类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_CSA3_ATTR_E
{
    HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_ALG = 0,            /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_SESSION_KEY,   /**<Keyladder session key*/
    HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_BUTT
}HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_E;

/**Defines the attribute of the R2R keyladder type ID.*/
/**CNcomment: 定义R2R keyladder属性类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_R2R_ATTR_E
{
    HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_ALG = 0,            /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_SESSION_KEY,   /**<Keyladder session key*/
    HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_BUTT
}HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_E;

/**Defines the attribute of the LP keyladder type ID.*/
/**CNcomment: 定义LP keyladder属性类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_LP_ATTR_E
{
    HI_UNF_ADVCA_KEYLADDER_LP_ATTR_LOAD_LPK = 0,            /**<Load Link protection key*/
    HI_UNF_ADVCA_KEYLADDER_LP_ATTR_DECRYPT_PROTECTED_DATA,   /**<Decrypt Link protected data*/
    HI_UNF_ADVCA_KEYLADDER_LP_ATTR_BUTT
}HI_UNF_ADVCA_KEYLADDER_LP_ATTR_E;

/**Defines the attribute of the SP keyladder type ID.*/
/**CNcomment: 定义SP keyladder属性类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_SP_ATTR_E
{
    HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ENABLE = 0,       /**<Use SPE keyladder or not for specal CA system */
    HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ALG ,            /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_SP_ATTR_DSC_MODE,   /**<Descrambling mode*/        
    HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY,   /**<Keyladder session key*/
    HI_UNF_ADVCA_KEYLADDER_SP_ATTR_BUTT
}HI_UNF_ADVCA_KEYLADDER_SP_ATTR_E;

/**Defines the attribute of the MISC keyladder type ID.*/
/**CNcomment: 定义MISC keyladder属性类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_MISC_ATTR_E
{
    HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_ENABLE = 0, /**<Use MISC keyladder or not. Before using the MISC keyladder, should set this attribute to HI_TRUE,
                                                                     and after using, should set it to HI_FALSE.*/
    HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_ALG,            /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_DSC_MODE,   /**<Descrambling mode*/ 
    HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_SESSION_KEY,   /**<Keyladder session key*/    
    HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_BUTT
}HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_E;

/**Defines the attribute of the TA keyladder type ID.*/
/**CNcomment: 定义TA keyladder属性类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_TA_ATTR_E
{
    HI_UNF_ADVCA_KEYLADDER_TA_ATTR_ENABLE = 0, /**<Use TA keyladder or not. Before using the TA keyladder, should set this attribute to HI_TRUE,
                                                                     and after using, should set it to HI_FALSE.*/
    HI_UNF_ADVCA_KEYLADDER_TA_ATTR_ALG,            /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_TA_ATTR_SESSION_KEY,   /**<Keyladder session key*/
    HI_UNF_ADVCA_KEYLADDER_TA_ATTR_LOAD_TRANDATA,   /**<Transformation data protection and loading*/    
    HI_UNF_ADVCA_KEYLADDER_TA_ATTR_BUTT
}HI_UNF_ADVCA_KEYLADDER_TA_ATTR_E;

/**Defines the attribute of the CSA2 keyladder.*/
/**CNcomment: 定义CSA2 keyladder属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_CSA2_ATTR_S
{
    HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_E  enCsa2KlAttr; /**<CSA2 keyladder attribute ID*/
    HI_UNF_ADVCA_ALG_TYPE_E enAlgType;          /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage;   /**<Keyladder stage of the session key to be set*/
    HI_U8 u8SessionKey[16];                                   /**<Session key*/  
}HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_S;

/**Defines the attribute of the CSA3 keyladder.*/
/**CNcomment: 定义CSA3 keyladder属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_CSA3_ATTR_S
{
    HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_E  enCsa3KlAttr; /**<CSA3 keyladder attribute ID*/
    HI_UNF_ADVCA_ALG_TYPE_E enAlgType;          /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage;   /**<Keyladder stage of the session key to be set*/
    HI_U8 u8SessionKey[16];                                    /**<Session key*/  
}HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_S;

/**Defines the attribute of the R2R keyladder.*/
/**CNcomment: 定义R2R keyladder属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_R2R_ATTR_S
{
    HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_E  enR2RKlAttr; /**<R2R keyladder attribute  ID*/
    HI_UNF_ADVCA_ALG_TYPE_E enAlgType;          /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage;   /**<Keyladder stage of the session key to be set*/
    HI_U8 u8SessionKey[16];                                   /**<Session key*/
}HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_S;

/**Defines the attribute of the SP keyladder.*/
/**CNcomment: 定义SP keyladder属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_SP_ATTR_S
{
    HI_UNF_ADVCA_KEYLADDER_SP_ATTR_E  enSPKlAttr; /**<SP keyladder attribute ID*/
    HI_BOOL bEnable;    /**<Use SP keyladder or not. Before using the SP keyladder*/
    HI_UNF_ADVCA_ALG_TYPE_E enAlgType;               /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_SP_DSC_MODE_E enDscMode;    /**<Descramblig mode*/
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage;         /**<Keyladder stage of the session key to be set*/
    HI_U8 u8SessionKey[16];                                         /**<Session key*/
}HI_UNF_ADVCA_KEYLADDER_SP_ATTR_S;

/**Defines the attribute of the LP keyladder.*/
/**CNcomment: 定义LP keyladder属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_LP_ATTR_S
{
    HI_UNF_ADVCA_KEYLADDER_LP_ATTR_E  enLPKlAttr; /**<LP keyladder attribute ID*/
    HI_U8 u8Lpk[16];                                /**<Link protection key to be loaded*/
    HI_U8 *pu8ProtectedData;                /**<Input link protected data to be decrypted*/
    HI_U32 u32ProtectedDataLen;          /**<Input link protected data length*/   
    HI_U8 *pu8OutputProtectedData;   /**<Output link protected data after decryption*/
}HI_UNF_ADVCA_KEYLADDER_LP_ATTR_S;

/**Defines the attribute of the MISC keyladder.*/
/**CNcomment: 定义MISC keyladder属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_MISC_ATTR_S
{
    HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_E  enMiscKlAttr; /**<MISC keyladder attribute ID*/
    HI_BOOL bEnable;    /**<Use MISC keyladder or not. Before using the MISC keyladder, should be set to HI_TRUE,
                            and after using, should be set to HI_FALSE.*/   
    HI_UNF_ADVCA_ALG_TYPE_E enAlgType;               /**<Keyladder algorithm, TDES/AES*/  
    HI_UNF_ADVCA_SP_DSC_MODE_E enDscMode;    /**<Descramblig mode*/
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage;         /**<Keyladder stage of the session key to be set*/
    HI_U8 u8SessionKey[16];                                         /**<Session key*/
}HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_S;

/**Defines the attribute of the TA keyladder.*/
/**CNcomment: 定义TA keyladder属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_TA_ATTR_S
{  
    HI_UNF_ADVCA_KEYLADDER_TA_ATTR_E  enTAKlAttr; /**<TA keyladder attribute ID*/
    HI_BOOL bEnable;    /**<Use TA keyladder or not. Before using the TA keyladder, should be set to HI_TRUE,
                            and after using, should be set to HI_FALSE.*/     
    HI_UNF_ADVCA_ALG_TYPE_E enAlgType;          /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage;   /**<Keyladder stage of the session key to be set*/
    HI_U8 au8SessionKey[16];                                   /**<Session key*/
    HI_U8 *pu8TranData;                                         /**<Transformation data*/
    HI_U32 u32TranDataLen;                                  /**<Transformation data length*/
}HI_UNF_ADVCA_KEYLADDER_TA_ATTR_S;

/**Defines the attribute of the Google DRM keyladder type ID.*/
/**CNcomment: 定义Google DRM keyladder属性类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_GDRM_ATTR_E
{
    HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_ENABLE = 0,    /**<Use GDRM keyladder or not. Before using the GDRM keyladder, should set this attribute to HI_TRUE, and after using, should set it to HI_FALSE.*/
    HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_ENCRYPT,       /**<Encrypt clear device key*/    
    HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_SESSION_KEY,   /**<Keyladder session key*/
    HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_GETFLAG,       /**<Get flag*/
    HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_BUTT
}HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_E;

/**Defines the attribute of Google DRM keyladder.*/
/**CNcomment: 定义Google DRM keyladder属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_GDRM_ATTR_S
{  
    HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_E  enGDRMKlAttr;
    HI_BOOL bEnable;                                  /**<Use GDRM keyladder or not. Before using the GDRM keyladder, should be set to HI_TRUE, and after using, should set it to HI_FALSE.*/
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage;       /**<Keyladder stage of the session key to be set*/
    HI_HANDLE hCipherHandle;                    /**<cipher handle, if enStage is HI_UNF_ADVCA_KEYLADDER_LEV3, hCipherHandle should be set*/
    HI_U8 au8SessionKey[32];                     /**<if select HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_ENCRYPT it is clear device key, if select HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_SESSION_KEY, it is session key*/
    HI_U8 au8Output[16];                          /**<if HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_ENCRYPT is set, it is encrypted device key, if HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_GETFLAG is set, it is flag*/                                                   
}HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_S;

/**Defines the attribute of all of the keyladder.*/
/**CNcomment: 定义所有 keyladder属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_ATTR_S
{
    union
    {
        HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_S 	    stCSA2KlAttr;       /**<CSA2 keyladder attribute*/
        HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_S 	    stCSA3KlAttr;       /**<CSA3 keyladder attribute*/
        HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_S       stR2RKlAttr;        /**<R2R keyladder attribute*/
        HI_UNF_ADVCA_KEYLADDER_SP_ATTR_S 	    stSPKlAttr;         /**<SP keyladder attribute*/   
        HI_UNF_ADVCA_KEYLADDER_LP_ATTR_S        stLpKlAttr;         /**<LP keyladder attribute*/  
        HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_S      stMiscKlAttr;       /**<MISC keyladder attribute*/
        HI_UNF_ADVCA_KEYLADDER_TA_ATTR_S        stTAKlAttr;         /**<TA keyladder attribute, only for Irdeto MSR2.2*/
        HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_S      stGDRMAttr;         /**<Google DRM keyladder attribute*/
    }unKlAttr;
}HI_UNF_ADVCA_KEYLADDER_ATTR_S;


/**Defines the attribute of the CA vendor type ID.*/
/**CNcomment: 定义CA vendor属性类型ID.*/
typedef enum hiUNF_ADVCA_CA_VENDOR_OPT_E
{
    HI_UNF_ADVCA_CA_VENDOR_OPT_IRDETO_CHIP_CONF_CMAC = 0, /**<To generate a secure message authentication code (MAC) tag for the OTP configuration data
                                                                                      ONLY used for Irdeto MSR2.2.*/
    HI_UNF_ADVCA_CA_VENDOR_OPT_IRDETO_CBCMAC_CALC,  /**<To calculate the high level code CBC-MAC, ONLY used for Irdeto MSR2.2.*/                                                                                  
    HI_UNF_ADVCA_CA_VENDOR_OPT_IRDETO_CBCMAC_AUTH,  /**<To authenticate the high level code CBC-MAC, ONLY used for Irdeto MSR2.2.*/
    HI_UNF_ADVCA_CA_VENDOR_OPT_IRDETO_ACTIVATION_CODE, /**<To set the activation code for the Irdeto tweaked CSA3, ONLY used for Irdeto MSR2.2.*/
    HI_UNF_ADVCA_CA_VENDOR_OPT_VMX_GET_RNG, /**<To get the RNG number, ONLY used for verimatrix currently.*/
    HI_UNF_ADVCA_CA_VENDOR_OPT_BUTT
}HI_UNF_ADVCA_CA_VENDOR_OPT_E;

/**Defines the attribute of the Irdeto MSR2.2 chip configuration CMAC.*/
/**CNcomment: 定义Irdeto MSR2.2产生OTP数据CMAC属性结构体.*/
typedef struct hiUNF_ADVCA_IRDETO_CHIP_CONF_CMAC_ATTR_S
{
    HI_U8 au8ChipConfBitm[16]; /**<128bit Chip configuration bit map*/
    HI_U8 au8ChipConfCmac[16];   /**<128bit Chip configuration CMAC*/
}HI_UNF_ADVCA_IRDETO_CHIP_CONF_CMAC_ATTR_S;

/**Defines the attribute of the Irdeto MSR2.2 tweak CSA3, Activation Code.*/
/**CNcomment: 定义Irdeto MSR2.2 私有CSA3算法需要设置的ACTIVATION CODE.*/
typedef struct hiUNF_ADVCA_IRDETO_ACTIVATION_CODE_ATTR_S
{
    HI_U8 au8CSA3ActCode[16];   /**<128bit Irdeto MSR2.2 tweak CSA3 Activation Code*/
}HI_UNF_ADVCA_IRDETO_ACTIVATION_CODE_ATTR_S;

/**Defines the attribute of the Irdeto MSR2.2 high level code CBC-MAC authentication.*/
/**CNcomment: 定义Irdeto MSR2.2 上层应用CBC-MAC校验属性结构体.*/
typedef struct hiUNF_ADVCA_IRDETO_CBCMAC_ATTR_S
{
    HI_U8 *pu8InputData;
    HI_U32 u32InputDataLen;
    HI_U8 au8OutputCBCMAC[16];
    HI_U8 au8RefCBCMAC[16];  /**<Reference CBC-MAC*/
    HI_U32 u32AppLen; /**<High level code length*/   
}HI_UNF_ADVCA_IRDETO_CBCMAC_ATTR_S;

typedef struct hiUNF_ADVCA_VMX_GetRNG_S
{
    HI_U32 u32RNG;
}HI_UNF_ADVCA_VMX_GetRNG_S;

/**Defines the attribute of the CA vendor private function.*/
/**CNcomment: 定义CA厂家私有功能属性结构体.*/
typedef struct hiADVCA_CA_VENDOR_OPT_S
{	
    HI_UNF_ADVCA_CA_VENDOR_OPT_E enCaVendorOpt;
    union
    {
    	HI_UNF_ADVCA_IRDETO_CHIP_CONF_CMAC_ATTR_S     stIrdetoChipConfCmac;
        HI_UNF_ADVCA_IRDETO_ACTIVATION_CODE_ATTR_S    stIrdetoCsa3ActCode;
        HI_UNF_ADVCA_IRDETO_CBCMAC_ATTR_S             stIrdetoCbcMac;
        HI_UNF_ADVCA_VMX_GetRNG_S                     stVMXRng;		
    }unCaVendorOpt;
}HI_UNF_ADVCA_CA_VENDOR_OPT_S;

/************************************************/

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      ADVCA */
/** @{ */  /** <!-- [ADVCA] */

/**
\brief Initializes the advanced CA module CNcomment:初始化advance CA模块 CNend
\attention \n
Call this application programming interface (API) before using the advanced CA module.
The code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:在进行advance CA相关操作前应该首先调用本接口\n
重复调用本接口，会返回成功 CNend
\param N/A CNcomment:无 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_OPEN_ERR The CA device fails to start CNcomment:HI_ERR_CA_OPEN_ERR 打开CA设备失败 CNend
\see \n
::HI_UNF_ADVCA_DeInit
*/
HI_S32 HI_UNF_ADVCA_Init(HI_VOID);

/**
\brief Deinitializes the advanced CA module CNcomment:去初始化advance CA模块 CNend
\attention \n
None CNcomment:无 CNend
\param N/A CNcomment:无 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_DeInit(HI_VOID);

/**
\brief Set the ChipId CNcomment:设置ChipId  CNend
\attention \n
This application programming interface (API) is allowed to invoked only once. 
It's not allowed to call this API repeatedly.
CHIP_ID should have been setting before chipset is delivered to STB Manufacture.
Please contact Hisilicon before Customer try to use this interface.
CNcomment:\n  CNend
CNcomment:该接口只允许调用一次，不能重复调用，请谨慎使用\n
CHIP_ID可能已按照CA公司要求设置，该接口如果客户需要使用该接口需要先通知海思\n  CNend
\param[in] Id chip id CNcomment:Id chip id\n  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\see \n
::HI_UNF_ADVCA_SetChipId
*/
HI_S32 HI_UNF_ADVCA_SetChipId(HI_U32 Id);

/**
\brief Obtains the chip ID CNcomment:获取芯片ID  CNend
\attention \n
The chip ID is read-only.
CNcomment:芯片ID只能读不能写 CNend
\param[out] pu32ChipId Chip ID CNcomment:pu32ChipId   芯片ID  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetChipId(HI_U32 *pu32ChipId);


/**
\brief Obtains the market ID CNcomment:获取Market ID  CNend
\attention \n
None CNcomment:无 CNend
\param[out] u8MarketId market ID CNcomment:u8MarketId   针对市场的标识序号 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized CNcomment:HI_ERR_CA_NOT_INIT  CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA  输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_SetMarketId
*/
HI_S32 HI_UNF_ADVCA_GetMarketId(HI_U8 u8MarketId[4]);

/**
\brief Sets the Market ID CNcomment:设置Market ID  CNend
\attention \n
The market ID of the set-top box (STB) is set before delivery. The market ID can be set once only and takes effects after the STB restarts.
CNcomment:在机顶盒出厂时设置，仅支持设置一次 设置后重启生效 CNend
\param[in] u8MarketId market ID CNcomment:u8MarketId   针对市场的标识序号 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT  CA未初始化 CNend
\retval ::HI_ERR_CA_SETPARAM_AGAIN The parameter has been set CNcomment:HI_ERR_CA_SETPARAM_AGAIN  重复设置 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetMarketId(HI_U8 u8MarketId[4]);


/**
\brief Obtains the serial number of the STB CNcomment:获取机顶盒序列号 CNend
\attention \n
None CNcomment:无 CNend
\param[out] u8StbSn serial number of the STB CNcomment:u8StbSn   机顶盒序列号 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_SetStbSn
*/
HI_S32 HI_UNF_ADVCA_GetStbSn(HI_U8 u8StbSn[4]);

/**
\brief Sets the serial number of the STB CNcomment:设置机顶盒序列号 CNend
\attention \n
The serial number of the STB is set before delivery. The market ID can be set once only and takes effects after the STB restarts.
CNcomment:在机顶盒出厂时设置，仅支持设置一次 设置后重启生效 CNend
\param[in] u8StbSn serial number of the STB CNcomment:u8StbSn   机顶盒序列号 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT  CA未初始化 CNend
\retval ::HI_ERR_CA_SETPARAM_AGAIN The parameter has been set CNcomment:HI_ERR_CA_SETPARAM_AGAIN  重复设置 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetStbSn(HI_U8 u8StbSn[4]);

/**
\brief Set the R2R RootKey and lock it. CNcomment:设置R2R RootKey并且锁住。  CNend
\attention \n
This application programming interface (API) is allowed to invoked only once. 
It's not allowed to call this API repeatedly.
R2R RootKey should have been setting before chipset is delivered to STB Manufacture.
Please contact Hisilicon before Customer try to use this interface.
CNcomment:该接口只允许调用一次，不能重复调用,请谨慎使用该接口\n
R2RRootKey可能已按照CA公司要求设置，该接口如果客户需要使用该接口需要先通知海思\n  CNend
\param[in] pkey R2R Root Key CNcomment:pkey  R2R根密钥\n  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
*/
HI_S32 HI_UNF_ADVCA_SetR2RRootKey(HI_U8 *pkey);

/**
\brief Obtains the security startup enable status CNcomment:获取安全启动使能状态 CNend
\attention \n
None CNcomment:无 CNend
\param[out] pbEnable: Security startup enable. CNcomment:pbEnable   安全启动是否使能， CNend
HI_TRUE enabled CNcomment:HI_TRUE 使能，  CNend
HI_FALSE disabled CNcomment:HI_FALSE 不使能 CNend
\param[out] penFlashType the startup flash type, only valid when SCS is enable
CNcomment:penFlashType 仅在安全启动使能时有效，表示安全启动的Flash类型 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetSecBootStat(HI_BOOL *pbEnable,HI_UNF_ADVCA_FLASH_TYPE_E *penFlashType);

/**
\brief Obtains the mode of the JTAG interface CNcomment:获取JTAG调试口模式 CNend
\attention \n
None CNcomment:无 CNend
\param[out] penJtagMode Mode of the JTAG interface CNcomment:penJtagMode   JTAG调试口模式 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_JTAG_MODE_E
*/
HI_S32 HI_UNF_ADVCA_GetJtagMode(HI_UNF_ADVCA_JTAG_MODE_E *penJtagMode);

/**
\brief Sets the mode of the JTAG interface CNcomment:设置JTAG调试口模式   CNend
\attention \n
If the mode of the JTAG interface is set to closed or password-protected, it cannot be opened.
If the JTAG interface is open, it can be closed or password-protected.
If the JATG interface is password-protected, it can be closed.
After being closed, the JATG interface cannot be set to open or password-protected mode.
CNcomment:不支持设置为打开状态。\n
打开的时候可以关闭或设置为密钥保护状态。\n
处于密钥保护状态时可以关闭。\n
关闭之后不能打开和设置为密钥保护状态 CNend
\param[in] enJtagMode Mode of the JTAG interface CNcomment:enJtagMode   JTAG调试口模式 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\retval ::HI_ERR_CA_SETPARAM_AGAIN The parameter has been set CNcomment:HI_ERR_CA_SETPARAM_AGAIN 重复设置 CNend
\see \n
::HI_UNF_ADVCA_JTAG_MODE_E
*/
HI_S32 HI_UNF_ADVCA_SetJtagMode(HI_UNF_ADVCA_JTAG_MODE_E enJtagMode);


/**
\brief Obtains the R2R key ladder stage CNcomment:获取R2R key ladder级数 CNend
\attention \n
None CNcomment:无 CNend
\param[out] penStage Key ladder stage CNcomment:penStage   key ladder级数 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_KEYLADDER_LEV_E
*/
HI_S32 HI_UNF_ADVCA_GetR2RKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E *penStage);

/**
\brief Sets the R2R key ladder stage CNcomment:设置R2R key ladder的级数    CNend
\attention \n
The key ladder stage can be set only once before delivery and cannot be changed. Please use default value.
CNcomment:机顶盒出厂时设置 仅能设置一次 不可更改,不建议使用该接口改变stage  CNend
\param[in] enStage Key ladder stage Its value is HI_UNF_ADVCA_KEYLADDER_LEV2 or HI_UNF_ADVCA_KEYLADDER_LEV3
CNcomment:enStage   key ladder级数\n 取值只能为HI_UNF_ADVCA_KEYLADDER_LEV2 或者 HI_UNF_ADVCA_KEYLADDER_LEV3  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_KEYLADDER_LEV_E
*/
HI_S32 HI_UNF_ADVCA_SetR2RKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage);

/**
\brief Obtains the digital video broadcasting (DVB) key ladder stage CNcomment:获取DVB key ladder的级数 CNend
\attention \n
None CNcomment:无 CNend
\param[out] penStage Key ladder stage CNcomment:penStage   key ladder级数 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_KEYLADDER_LEV_E
*/
HI_S32 HI_UNF_ADVCA_GetDVBKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E *penStage);

/**
\brief Sets the DVB key ladder stage CNcomment:设置DVB key ladder的级数    CNend
\attention \n
The key ladder stage can be set only once before delivery and cannot be changed. Please use default value.
CNcomment:机顶盒出厂时设置 仅能设置一次 不可更改,不建议使用该接口改变stage  CNend
\param[in] enStage Key ladder stage Its value is HI_UNF_ADVCA_KEYLADDER_LEV2 or HI_UNF_ADVCA_KEYLADDER_LEV3.
CNcomment:enStage  key ladder级数\n 取值只能为HI_UNF_ADVCA_KEYLADDER_lev2 或者 HI_UNF_ADVCA_KEYLADDER_lev3  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_KEYLADDER_LEV_E
*/
HI_S32 HI_UNF_ADVCA_SetDVBKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage);

/**
\brief Obtains the CSA3 key ladder stage CNcomment:获取CSA3 key ladder的级数 CNend
\attention \n
None CNcomment:无 CNend
\param[out] penStage Key ladder stage CNcomment:penStage   key ladder级数 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_KEYLADDER_LEV_E
*/
HI_S32 HI_UNF_ADVCA_GetCSA3KeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E *penStage);

/**
\brief Sets the CSA3 key ladder stage CNcomment:设置CSA3 key ladder的级数    CNend
\attention \n
The key ladder stage can be set only once before delivery and cannot be changed. Please use default value.
CNcomment:机顶盒出厂时设置 仅能设置一次 不可更改,不建议使用该接口改变stage  CNend
\param[in] enStage Key ladder stage Its value is HI_UNF_ADVCA_KEYLADDER_LEV2 or HI_UNF_ADVCA_KEYLADDER_LEV3.
CNcomment:enStage  key ladder级数\n 取值只能为HI_UNF_ADVCA_KEYLADDER_lev2 或者 HI_UNF_ADVCA_KEYLADDER_lev3  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_KEYLADDER_LEV_E
*/
HI_S32 HI_UNF_ADVCA_SetCSA3KeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage);

/**
\brief Sets session keys for an R2R key ladder CNcomment:为R2R key ladder配置会话密钥    CNend
\attention \n

The stage of the session key cannot be greater than the configured stage of the key ladder. The last stage of the session key is configured by calling the API of the CIPHER module rather than this API.
That is, only session key 1 and session key 2 need to be configured for a 3-stage key ladder.
Only session key 1 needs to be configured for a 2-stage key ladder.
You need to set the key ladder stage by calling HI_UNF_ADVCA_SetR2RKeyLadderStage first.
Session keys can be set during initialization or changed at any time.
CNcomment:注意配置的级数不能超过设置的级数值，最后一级由CIPHER模块内部配置，不用通过此接口配置。\n
也就是说，对于3级key ladder，只用配置会话密钥1和会话密钥2。\n
对于2级的key ladder，只用配置会话密钥1。\n
请先调用HI_UNF_ADVCA_SetR2RKeyLadderStage设置key ladder级数。\n
会话密钥可以初始时设置一次，也可以随时修改。 CNend
\param[in] enStage Key ladder stage Its value is HI_UNF_ADVCA_KEYLADDER_LEV2 or HI_UNF_ADVCA_KEYLADDER_LEV3.
CNcomment:enStage    密钥级数，[HI_UNF_ADVCA_KEYLADDER_LEV1 ~ HI_UNF_ADVCA_KEYLADDER_LEV2]  CNend
\param[in] pu8Key Protection key pointer, 128 bits (16 bytes) in total CNcomment:pu8Key     保护密钥指针，共128bit(16byte)  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS  成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\retval ::HI_ERR_CA_WAIT_TIMEOUT Timeout occurs when the CA module waits for encryption or decryption
CNcomment:HI_ERR_CA_WAIT_TIMEOUT CA等待加解密超时 CNend
\retval ::HI_ERR_CA_R2R_DECRYPT The R2R decryption fails CNcomment:HI_ERR_CA_R2R_DECRYPT  R2R解密失败 CNend
\see \n
::HI_UNF_ADVCA_KEYLADDER_LEV_E
*/
HI_S32 HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key);

/**
\brief Sets session keys for a DVB key ladder CNcomment:为DVB key ladder配置会话密钥    CNend
\attention \n
The stage of the session key cannot be greater than the configured stage of the key ladder. The last stage of the session key is configured by calling the API of the CIPHER module rather than this API.
That is, only session key 1 and session key 2 need to be configured for a 3-stage key ladder.
Only session key 1 needs to be configured for a 2-stage key ladder.
You need to set the key ladder stage by calling HI_UNF_ADVCA_SetDVBKeyLadderStage first.
 Session keys can be set during initialization or changed at any time.
CNcomment:注意配置的级数不能超过设置的级数值，最后一级由Descrambler模块内部配置，不用通过此接口配置。\n
也就是说，对于3级key ladder，只用配置会话密钥1和会话密钥2。\n
对于2级的key ladder，只用配置会话密钥1。\n
请先调用HI_UNF_ADVCA_SetDVBKeyLadderStage设置key ladder级数。\n
会话密钥可以初始时设置一次，也可以随时修改。 CNend
\param[in] enStage Key ladder stage Its value is HI_UNF_ADVCA_KEYLADDER_LEV2 or HI_UNF_ADVCA_KEYLADDER_LEV3.
CNcomment:enStage    密钥级数，[HI_UNF_ADVCA_KEYLADDER_LEV1 ~ HI_UNF_ADVCA_KEYLADDER_LEV2]  CNend
\param[in] pu8Key Protection key pointer, 128 bits (16 bytes) in total CNcomment:pu8Key     保护密钥指针，共128bit(16byte)  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS  成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\retval ::HI_ERR_CA_WAIT_TIMEOUT Timeout occurs when the CA module waits for encryption or decryption
CNcomment:HI_ERR_CA_WAIT_TIMEOUT CA等待加解密超时 CNend
\retval ::HI_ERR_CA_R2R_DECRYPT The CW decryption fails CNcomment:HI_ERR_CA_CW_DECRYPT   CW解密失败 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetDVBSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key);

/**
\brief Sets session keys for a CSA3 key ladder CNcomment:为CSA3 key ladder配置会话密钥    CNend
\attention \n
The stage of the session key cannot be greater than the configured stage of the key ladder. The last stage of the session key is configured by calling the API of the CIPHER module rather than this API.
That is, only session key 1 and session key 2 need to be configured for a 3-stage key ladder.
Only session key 1 needs to be configured for a 2-stage key ladder.
You need to set the key ladder stage by calling HI_UNF_ADVCA_SetDVBKeyLadderStage first.
 Session keys can be set during initialization or changed at any time.
CNcomment:注意配置的级数不能超过设置的级数值，最后一级由Descrambler模块内部配置，不用通过此接口配置。\n
也就是说，对于3级key ladder，只用配置会话密钥1和会话密钥2。\n
对于2级的key ladder，只用配置会话密钥1。\n
请先调用HI_UNF_ADVCA_SetDVBKeyLadderStage设置key ladder级数。\n
会话密钥可以初始时设置一次，也可以随时修改。 CNend
\param[in] enStage Key ladder stage Its value is HI_UNF_ADVCA_KEYLADDER_LEV2 or HI_UNF_ADVCA_KEYLADDER_LEV3.
CNcomment:enStage    密钥级数，[HI_UNF_ADVCA_KEYLADDER_LEV1 ~ HI_UNF_ADVCA_KEYLADDER_LEV2]  CNend
\param[in] pu8Key Protection key pointer, 128 bits (16 bytes) in total CNcomment:pu8Key     保护密钥指针，共128bit(16byte)  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS  成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\retval ::HI_ERR_CA_WAIT_TIMEOUT Timeout occurs when the CA module waits for encryption or decryption
CNcomment:HI_ERR_CA_WAIT_TIMEOUT CA等待加解密超时 CNend
\retval ::HI_ERR_CA_R2R_DECRYPT The CW decryption fails CNcomment:HI_ERR_CA_CW_DECRYPT   CW解密失败 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetCSA3SessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key);

/** 
\brief Set widevine keyladder session key CNcomment:设置widevine keyladder的各级输入密钥 CNend
\param[in]  enLevel keyladder level for input CNcomment:enLevel 输入密钥的级数 CNend
\param[in]  hCipherHandle cipher handle input when level 3 CNcomment:hCipherHandle 当输入第三级数据时，需要传入预先创建好的Cipher的句柄 CNend
\param[in]  pu8Input input data for keyladder CNcomment:pu8Input  keyladder的输入数据 CNend
\param[in]  u32InputLen input data length CNcomment:u32InputLen 数据的长度 CNend
\param[out] pu32GdrmFlag the flag value when level 3 CNcomment:pu32GdrmFlag  当输入第三级别密钥时，输出的GDRM标志  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS               成功 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT       CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA   输入参数非法 CNend
\retval ::HI_ERR_CA_NOT_SUPPORT The function is not supported CNcomment:HI_ERR_CA_NOT_SUPPORT    功能不支持 CNend
\retval ::HI_ERR_CA_WAIT_TIMEOUT Timeout occurs when the CA module waits for encryption or decryption
CNcomment:HI_ERR_CA_WAIT_TIMEOUT   CA等待加解密超时 CNend
\return ::HI_ERR_CA_SWPK_ENCRYPT SWPK encryption fails CNcomment:HI_ERR_CA_SWPK_ENCRYPT   SWPK加密失败 CNend
\see
\li ::
*/
HI_S32 HI_UNF_ADVCA_SetGDRMSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,
									HI_HANDLE hCipherHandle,
									HI_U8 *pu8Input,
									HI_U32 u32InputLen,
									HI_U32 *pu32GdrmFlag);


/** 
\brief Encrypts software protect keys (SWPKs) CNcomment:加密软件保护密钥 CNend
\attention
Before the delivery of the STB, you need to read the SWPKs in plain text format from the flash memory, encrypt SWPKs by calling this API, and store the encrypted SWPKs in the flash memory for security startup.
CNcomment:机顶盒出厂时 从Flash上读取明文的SWPK(Software Protect Key),调用该接口加密,将加密的SWPK存储在Flash中，用于安全启动 CNend
The fist 8 bytes of the SWPK can't be equal to the last 8 bytes.
CNcomment:SWPK的前8个字节与后8个字节不能相等 CNend
This API is only for special CA, please contact Hislicon before usage.
CNcomment:注意:此接口为特定CA专用，如需使用，请联系海思 CNend
\param[in]  pPlainSwpk SWPKs in plain text format CNcomment:pPlainSwpk    明文SWPK  CNend
\param[out] pEncryptedSwpk Encrypted SWPKs CNcomment:pEncryptedSwpk  加密后的SWPK  CNend

\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS               成功 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT       CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA   输入参数非法 CNend
\retval ::HI_ERR_CA_NOT_SUPPORT The function is not supported CNcomment:HI_ERR_CA_NOT_SUPPORT    功能不支持 CNend
\retval ::HI_ERR_CA_WAIT_TIMEOUT Timeout occurs when the CA module waits for encryption or decryption
CNcomment:HI_ERR_CA_WAIT_TIMEOUT   CA等待加解密超时 CNend
\return ::HI_ERR_CA_SWPK_ENCRYPT SWPK encryption fails CNcomment:HI_ERR_CA_SWPK_ENCRYPT   SWPK加密失败 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_EncryptSWPK(HI_U8 *pPlainSwpk,HI_U8 *pEncryptedSwpk);

/**
\brief Sets the algorithm of the DVB key ladder CNcomment:设置DVB key ladder的算法    CNend
\attention \n
You must set an algorithm before using a key ladder in a session. The default algorithm is TDES.
It is recommended that you retain the algorithm in a session.
CNcomment:每次会话过程中使用key ladder之前，须设置具体算法, 系统初始默认值 HI_UNF_ADVCA_ALG_TYPE_TDES；\n
本次会话过程中，建议保持算法的稳定不变。 CNend
\param[in] enType Key ladder algorithm CNcomment:enType  key ladder算法\n  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_ALG_TYPE_E
*/
HI_S32 HI_UNF_ADVCA_SetDVBAlg(HI_UNF_ADVCA_ALG_TYPE_E enType);

/**
\brief Sets the algorithm of the CSA2 key ladder CNcomment:设置CSA3 key ladder的算法    CNend
\attention \n
You must set an algorithm before using a key ladder in a session. The default algorithm is TDES.
It is recommended that you retain the algorithm in a session.
CNcomment:每次会话过程中使用key ladder之前，须设置具体算法, 系统初始默认值 HI_UNF_ADVCA_ALG_TYPE_TDES；\n
本次会话过程中，建议保持算法的稳定不变。 CNend
\param[in] enType Key ladder algorithm CNcomment:enType  key ladder算法\n  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_ALG_TYPE_E
*/
HI_S32 HI_UNF_ADVCA_SetCSA3Alg(HI_UNF_ADVCA_ALG_TYPE_E enType);

/**
\brief Sets the algorithm of the R2R key ladder CNcomment:设置R2R key ladder的算法    CNend
\attention \n
You must set an algorithm before using a key ladder in a session. The default algorithm is TDES.
It is recommended that you retain the algorithm in a session.
CNcomment:每次会话过程中使用key ladder之前，须设置具体算法, 系统初始默认值 HI_UNF_ADVCA_ALG_TYPE_TDES；\n
本次会话过程中，建议保持算法的稳定不变。 CNend
\param[in] enType Key ladder algorithm CNcomment:enType  key ladder算法\n  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_ALG_TYPE_E
*/
HI_S32 HI_UNF_ADVCA_SetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_E enType);

/**
\brief Obtains the algorithm of the DVB key ladder CNcomment: 获取 DVB key ladder的算法    CNend
\attention \n
None CNcomment:无 CNend
\param[in] pEnType Key ladder algorithm CNcomment:pEnType  key ladder算法\n  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_ALG_TYPE_E
*/
HI_S32 HI_UNF_ADVCA_GetDVBAlg(HI_UNF_ADVCA_ALG_TYPE_E *pEnType);

/**
\brief Obtains the algorithm of the R2R key ladder CNcomment:获取 R2R key ladder的算法    CNend
\attention \n
None CNcomment:无 CNend
\param[in] enType Key ladder algorithm CNcomment:enType  key ladder算法\n  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_ALG_TYPE_E
*/
HI_S32 HI_UNF_ADVCA_GetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_E *pEnType);

/**
  \brief set RSA key CNcomment:设置签名校验的RSA公钥 CNeng
  \attention \n
  If the mass-produced chipset have been set RSA Key by CA, there's no need to use this API to set RSA Key again.
  This API can been called only once, please be careful, after setting RSA key, you should lock it by call API 
  HI_UNF_ADVCA_ConfigLockFlag.The length of RSA key must be 512 Bytes.
  CNcomment:如果量产芯片已经按照CA公司设置RSAKey,则无需再次使用该接口设置RSAKey\n
     该接口只允许调用一次，不能重复调用,请谨慎使用该接口,并且设置后RSAKey后需要使用接口\n
     HI_UNF_ADVCA_ConfigLockFlag进行锁定。 RSA key的长度必须为512 Bytes  CNend
  \param[in] pkey RSA key CNcomment:pkey  RSA公钥\n  CNend
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
  \retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
  \see  \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetRSAKey(HI_U8 *pkey);

/**
  \brief set RSA key CNcomment:获取签名校验的RSA公钥 CNend
  \attention \n
     RSA key can be read out when only the RSAKey is not locked,The length of RSA key is 512 Bytes.
     RSA key should have been setting and lock before chipset is delivered to STB Manufacture.
     Please contact Hisilicon before Customer try to use this interface.
  CNcomment:该接口仅在RSAkey没有被锁定的情况下，读取出来, RSA key的长度为512 Bytes\n
     RSA key可能已按照CA公司要求设置并锁定，该接口如果客户需要使用该接口需要先通知海思\n  CNend
  \param[in] pkey RSA key CNcomment:pkey  RSA密公钥\n  CNend
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
  \retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
  \see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetRSAKey(HI_U8 *pkey);

/**
  \brief set RSA key CNcomment:获取签名校验的RSA密码 CNend
  \attention \n
    This API is used to lock RSA KEY/Trustzone OTP\misc keyladder etc.Please contact Hisilicon before Customer try to use this interface.
  CNcomment:该接口用于锁定RSAkey\Trustzone OTP\misc keyladder等，该接口如果客户需要使用该接口需要先通知海思\n  CNend
  \param[enType] enType the type you want to lock.  CNcomment:enType  需要锁定的类型\n  CNend
  \param[pu8ParamIn] pu8ParamIn reserved, default is null. CNcomment:pu8ParamIn  预留，默认为null\n  CNend
  \param[u32ParamLen] u32ParamLen reserved, default is 0. CNcomment:u32ParamLen  预留，默认为0\n  CNend
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
  \retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
  \see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_ConfigLockFlag(HI_UNF_ADVCA_LOCK_TYPE_E enType, HI_U8 *pu8ParamIn, HI_U32 u32ParamLen);


/**
  \brief This function is used to check if the MarketID is already set
  \attention \n
  None
  \param[in] pbIsMarketIdSet: the pointer point to the buffer to store the return value
  \param[out] pbIsMarketIdSet: save the return value
  \retval ::HI_SUCCESS Success
  \retval ::HI_FAILURE This API fails to be called
  \see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_IsMarketIdSet(HI_BOOL *pbIsMarketIdSet);

/**
  \brief This function is used to get the vendor type of the chipset
  \attention \n
  None
  \param[out] pu32VendorID: The number indicates the vendor id
  \retval ::HI_SUCCESS Success
  \retval ::HI_FAILURE This API fails to be called
  \see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetVendorID(HI_U32 *pu32VendorID);

/**
\brief Enables the security startup function and sets the type of flash memory for security startup
CNcomment:设置安全启动使能,同时指定安全启动的Flash类型 CNend
\attention \n
This function can be enabled only and cannot be disabled after being enabled.
CNcomment:只能使能，使能之后不可修改。 CNend
\param[in]  enFlashType Type of the flash memory for security startup CNcomment:enFlashType  启动的Flash类型 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_SETPARAM_AGAIN The parameter has been set CNcomment:HI_ERR_CA_SETPARAM_AGAIN  重复设置 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_EnableSecBoot(HI_UNF_ADVCA_FLASH_TYPE_E enFlashType);

/**
\brief Enables the security startup function. This API should be used with the API HI_UNF_ADVCA_SetFlashTypeEx.
CNcomment:设置安全启动使能，该接口必须和HI_UNF_ADVCA_SetFlashTypeEx配套使用。 CNend
\attention \n
\param[in]  None
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_SETPARAM_AGAIN The parameter has been set CNcomment:HI_ERR_CA_SETPARAM_AGAIN  重复设置 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_EnableSecBootEx(HI_VOID);

/**
\brief Sets the type of flash memory for security startup and disable the self boot, which mean that you cannot use the serial port to update the boot. This API should be used with the API HI_UNF_ADVCA_EnableSecBootEx
CNcomment:指定安全启动的Flash类型，同时关闭自举功能，即不能通过串口升级fastboot。该接口跟HI_UNF_ADVCA_EnableSecBootEx配套使用 CNend
\attention \n
The setting is performed before delivery and can be performed once only.
CNcomment:在机顶盒出厂时选择是否设置，仅支持设置一次 CNend
\param[in]  enFlashType Type of the flash memory for security startup CNcomment:enFlashType  启动的Flash类型 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_SETPARAM_AGAIN The parameter has been set CNcomment:HI_ERR_CA_SETPARAM_AGAIN  重复设置 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetFlashTypeEx(HI_UNF_ADVCA_FLASH_TYPE_E enFlashType);

/** 
\brief  Sets whether to use hardware CWs only CNcomment:设置固定使用硬件CW字 CNend
\attention
The setting is performed before delivery and can be performed once only.
By default, the CW type (hardware CWs or software CWs) depends on the configuration of the DEMUX.
CNcomment:在机顶盒出厂时选择是否设置，仅支持设置一次
默认根据Demux的配置选择使用硬件CW字还是软件CW字 CNend
\param  none
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT          CA未初始化 CNend
\return ::HI_ERR_CA_SETPARAM_AGAIN The parameter has been set CNcomment:HI_ERR_CA_SETPARAM_AGAIN    重复设置参数 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_LockHardCwSel(HI_VOID);

/** 
\brief Disables the self-boot function CNcomment:关闭SelfBoot功能,也就是boot下不能使用串口/网口升级 CNend
\attention
The setting is performed before delivery and can be performed once only.
The self-boot function is enabled by default.
CNcomment:在机顶盒出厂时选择是否设置，仅支持设置一次
默认使能SelfBoot功能 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized.  CNcomment:HI_ERR_CA_NOT_INIT          CA未初始化 CNend
\return ::HI_ERR_CA_SETPARAM_AGAIN The parameter has been set CNcomment:HI_ERR_CA_SETPARAM_AGAIN    重复设置参数 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_DisableSelfBoot(HI_VOID);

/**
\brief Obtains the self-boot status CNcomment:获取SelfBoot状态 CNend
\attention \n
None CNcomment:无 CNend
\param[out] pbDisable: self-boot status. CNcomment:pbEnable   SelfBoot是否禁用 CNend
HI_TRUE enabled CNcomment:HI_TRUE 禁用，  CNend
HI_FALSE disabled CNcomment:HI_FALSE 未禁用 CNend

\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetSelfBootStat(HI_BOOL *pbDisable);

/** 
\brief  Get whether to use hardware CWs only CNcomment:获取固定使用硬件CW字标志 CNend
\attention
None CNcomment:无 CNend
\param[out] pbLock indicates the state of hardware CWs  CNcomment:pbLock 硬件CW字的标志位 CNend

\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT          CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA  输入参数非法 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetHardCwSelStat(HI_BOOL *pbLock);

/** 
\brief  Open the SWPK key ladder CNcomment:打开boot key ladder  CNend
\attention
\param  none

\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT          CA未初始化 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SWPKKeyLadderOpen(HI_VOID);

/** 
\brief  Close the SWPK key ladder CNcomment:关闭boot key ladder  CNend
\attention
\param  none

\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT          CA未初始化 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SWPKKeyLadderClose(HI_VOID);

/**
\brief Obtains the Version ID CNcomment:获取Version ID  CNend
\attention \n
None CNcomment:无 CNend
\param[out] u8VersionId version ID CNcomment:u8VersionId   版本号标志 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized CNcomment:HI_ERR_CA_NOT_INIT  CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA  输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_SetVersionId
*/
HI_S32 HI_UNF_ADVCA_GetVersionId(HI_U8 u8VersionId[4]);

/**
\brief Sets the Version ID CNcomment:设置Version ID  CNend
\attention \n
The version ID of the set-top box (STB) is set before delivery. The version ID can be set once only and takes effects after the STB restarts.
CNcomment:在机顶盒出厂时设置，仅支持设置一次 设置后重启生效 CNend
\param[in] u8VersionId version ID CNcomment:u8VersionId   版本号标志 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT  CA未初始化 CNend
\retval ::HI_ERR_CA_SETPARAM_AGAIN The parameter has been set CNcomment:HI_ERR_CA_SETPARAM_AGAIN  重复设置 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetVersionId(HI_U8 u8VersionId[4]);

/** 
\brief  Sets whether to check the boot version CNcomment:设置是否检查Version  CNend
\attention
The setting is performed before delivery and can be performed once only.
By default, the version check function is disabled
CNcomment:在机顶盒出厂时选择是否设置，仅支持设置一次
默认不使能version check  CNend
\param  none
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT          CA未初始化 CNend
\return ::HI_ERR_CA_SETPARAM_AGAIN The parameter has been set CNcomment:HI_ERR_CA_SETPARAM_AGAIN    重复设置参数 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_EnableVersionCheck(HI_VOID);

/** 
\brief  Get the boot version check flag CNcomment:获取是否检查version的标志位 CNend
\attention \n
None CNcomment:无 CNend
\param[out] pu32Stat boot version check flag CNcomment:pu32Stat    version检查的标志位 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized CNcomment:HI_ERR_CA_NOT_INIT  CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA  输入参数非法 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetVersionCheckStat(HI_U32 *pu32Stat);

/** 
\brief  Sets whether to check the MSID in boot area CNcomment:设置是否检查boot area的MSID  CNend
\attention
The setting is performed before delivery and can be performed once only.
By default, the boot MSID check function is disabled
CNcomment:在机顶盒出厂时选择是否设置，仅支持设置一次
默认不使能boot MSID check  CNend
\param none
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT          CA未初始化 CNend
\return ::HI_ERR_CA_SETPARAM_AGAIN The parameter has been set CNcomment:HI_ERR_CA_SETPARAM_AGAIN    重复设置参数 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_EnableBootMSIDCheck(HI_VOID);

/** 
\brief  Get the boot MSID check flag CNcomment:获取是否检查MSID的标志位 CNend
\attention \n
None CNcomment:无 CNend
\param[out] pu32Stat boot MSID check flag CNcomment:pu32Stat    MSID检查的标志位 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized CNcomment:HI_ERR_CA_NOT_INIT  CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA  输入参数非法 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetBootMSIDCheckStat(HI_U32 *pu32Stat);

/** 
\brief  Get the software revision
CNcomment:获取软件的revision版本号 CNend
\attention \n
None CNcomment:无 CNend
\param[out] revision string of Revision CNcomment:revision    Revision版本号 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized CNcomment:HI_ERR_CA_NOT_INIT  CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA  输入参数非法 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetRevision(HI_U8 revision[25]);

/** 
\brief  Set the DDR Scramble flag. Normally, this flag has been set as required by CA vendor
CNcomment:设置DDR加扰标志位，一般高安芯片出厂时已按照CA公司要求设置 CNend
\attention \n
None CNcomment:无 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized CNcomment:HI_ERR_CA_NOT_INIT  CA未初始化 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetDDRScramble(HI_VOID);

/** 
\brief  Get the DDR Scramble flag
CNcomment:获取DDR加扰标志位 CNend
\attention \n
None CNcomment:无 CNend
\param[out] pu32Stat DDR Scramble flag CNcomment:pu32Stat    DDR加扰标志 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized CNcomment:HI_ERR_CA_NOT_INIT  CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA  输入参数非法 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetDDRScrambleStat(HI_U32 *pu32Stat);

/** 
\brief Sets whether to decrypt the BootLoader CNcomment:设置必须对BootLoader进行解密 CNend
\attention
The setting is performed before delivery and can be performed once only.
CNcomment:在机顶盒出厂时选择是否设置，仅支持设置一次
默认根据Flash中的数据标识，决定BootLoader是否需要解密 CNend

\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT          CA未初始化 CNend
\return ::HI_ERR_CA_SETPARAM_AGAIN The parameter has been set CNcomment:HI_ERR_CA_SETPARAM_AGAIN    重复设置参数 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_LockBootDecEn(HI_VOID);

/** 
\brief Get the BootLoader Decryption flag CNcomment:获取BootLoader解密的标志位 CNend
\attention \n
None CNcomment:无 CNend
\param[out] pu32Stat BootLoader Decryption flag CNcomment:pu32Stat    Bootloader解密标志位 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized CNcomment:HI_ERR_CA_NOT_INIT  CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA  输入参数非法 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetBootDecEnStat(HI_U32 *pu32Stat);





/*============the following is for PVR ===========*/


/** 
\brief  Open ADVCA PVR Record CNcomment:打开ADVCA PVR 录制 CNend
\attention
\param[in] u32RecChnID Channel ID of record CNcomment:u32RecChnID   录制通道号 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_FAILURE Faliure CNcomment:HI_FAILURE                  失败 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_RecOpen(HI_U32 u32RecChnID);

/** 
\brief  Close ADVCA PVR Record CNcomment:关闭ADVCA PVR 录制 CNend
\attention
\param[in] u32RecChnID Channel ID of record CNcomment:u32RecChnID   录制通道号 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_FAILURE Faliure CNcomment:HI_FAILURE                  失败 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_RecClose(HI_U32 u32RecChnID);

/** 
\brief  Open ADVCA PVR Play CNcomment:打开ADVCA PVR 播放 CNend
\attention
\param[in] u32PlayChnID Channel ID of record CNcomment:u32PlayChnID   播放通道号 CNend
\param[in] stPlayInfo play information CNcomment:stPlayInfo   播放信息 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_FAILURE Faliure CNcomment:HI_FAILURE                  失败 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_PlayOpen(HI_U32 u32PlayChnID, HI_UNF_ADVCA_PVR_PlAY_INFOR_S stPlayInfo);

/** 
\brief  Open ADVCA PVR Play CNcomment:关闭ADVCA PVR 播放 CNend
\attention
\param[in] u32PlayChnID Channel ID of record CNcomment:u32PlayChnID   播放通道号 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_FAILURE Faliure CNcomment:HI_FAILURE                  失败 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_PlayClose(HI_U32 u32PlayChnID);

/** 
\brief  Write callback function which should be registered by PVR,it achieve operating keyladder and M2M encryption 
CNcomment:应该被PVR注册的录制回调函数，它主要完成使用keyladder和multicipher对录制数据进行加密 CNend
\attention \n
None CNcomment:无 CNend
\param[in] pstCAPVRArgs Structure of parameters used by this function CNcomment:pstCAPVRArgs    本函数使用的参数结构体，
详细定义见HI_UNF_PVR_CA_CALLBACK_ARGS_S结构体的定义 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_FAILURE Faliure CNcomment:HI_FAILURE                  失败 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_WriteCallBack(HI_UNF_PVR_CA_CALLBACK_ARGS_S* pstCAPVRArgs);

/** 
\brief  Read callback function which should be registered by PVR,it achieve operating keyladder and M2M decryption 
CNcomment:应该被PVR注册的播放回调函数，它主要完成使用keyladder和multicipher对录制数据进行解密 CNend
\attention \n
None CNcomment:无 CNend
\param[in] pstCAPVRArgs Structure of parameters used by this function CNcomment:pstCAPVRArgs    本函数使用的参数结构体，
详细定义见HI_UNF_PVR_CA_CALLBACK_ARGS_S结构体的定义 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS                  成功 CNend
\retval ::HI_FAILURE Faliure CNcomment:HI_FAILURE                  失败 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_ReadCallBack(HI_UNF_PVR_CA_CALLBACK_ARGS_S* pstCAPVRArgs);



/**
\brief  Get the name of CA private data file by index file name
CNcomment:通过索引文件名字获取CA私有数据文件 CNend
\attention \n
None CNcomment:无 CNend

\param[in] pIndexFileName  The name of index file        CNcomment:索引文件名字 CNend
\param[out] CAPrivateFileName  The name  CA private data file  CNcomment:私有数据文件名字 CNend


\retval ::HI_SUCCESS Success     CNcomment: 成功 CNend
\retval ::HI_FAILURE Faliure       CNcomment:  失败 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_GetCAPrivateFileName(HI_CHAR * pIndexFileName,HI_CHAR CAPrivateFileName[128]);

/**
\brief  Create CA private data file
CNcomment: 创建CA私有数据文件 CNend
\attention \n
None CNcomment:无 CNend

\param[in] u32RecChnID  Record channel ID        CNcomment:录制通道ID  CNend
\param[in] pCAPrivateFileName  The name  CA private data file  CNcomment:私有数据文件名字 CNend
\param[in] pCurTime     The CurrentTime to record stream  CNcomment:当前创建时间 CNend

\retval ::HI_SUCCESS Success       CNcomment:成功 CNend
\retval ::HI_FAILURE Faliure          CNcomment: 失败 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_CreateCAPrivateFile( HI_U32 u32RecChnID,HI_CHAR * pCAPrivateFileName, HI_UNF_ADVCA_Time_S *pCurTime);

/**
\brief  Check  if the file of CA private data is correct
CNcomment: 核对CA 私有数据文件是否正确 CNend
\attention \n
None CNcomment:无 CNend

\param[in]  pCAPrivateFileName  The name of CA private data file .     CNcomment:CA 私有数据文件名字 CNend
\param[out] pOutRecordStartTime Get the Time to record stream  CNcomment:获取录制开始时间 CNend

\retval ::HI_SUCCESS Check successfully                   CNcomment:核对成功 CNend
\retval ::HI_FAILURE  Fail to check.                      CNcomment:核对失败 CNend
\retval ::0x101  Record storage time expire.              CNcomment:核对成功,但已经失效 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_CheckCAPrivateFileMAC( HI_CHAR * pCAPrivateFileName, HI_UNF_ADVCA_Time_S *pOutRecordStartTime);

/**
\brief  Save the information of maturity rate into the file of CA private data
CNcomment: 将成人级信息保存到CA 私有数据文件中 CNend
\attention \n
None CNcomment:无 CNend

\param[in]  u32RecChnID  The record channel ID .                             CNcomment:录制通道ID  CNend
\param[in]  pCAPrivateFileName  The name of CA private data file .     CNcomment:CA 私有数据文件名字 CNend
\param[in]  pstPVRURI  The information of maturity rate                   CNcomment:成人级信息 CNend


\retval ::HI_SUCCESS Check successfully                   CNcomment:保存成功 CNend
\retval ::HI_FAILURE  Fail to check.                          CNcomment:保存失败 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_SaveURI( HI_U32 u32RecChnID,HI_CHAR * pCAPrivateFileName ,HI_UNF_PVR_URI_S* pstPVRURI);

/**
\brief  Save the information of fingerprint into the file of CA private data
CNcomment: 将指纹信息保存到CA 私有数据文件中 CNend

\attention \n
None CNcomment:无 CNend

\param[in]  u32RecChnID  The record channel ID .                             CNcomment:录制通道ID  CNend
\param[in]  pCAPrivateFileName  The name of CA private data file .     CNcomment:CA 私有数据文件名字 CNend
\param[in]  pstFPInfo  The information of fingerprint                   CNcomment:指纹信息 CNend


\retval ::HI_SUCCESS Check successfully                   CNcomment:保存成功 CNend
\retval ::HI_FAILURE  Fail to check.                          CNcomment:保存失败 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_SaveFP( HI_U32 u32RecChnID,HI_CHAR * pCAPrivateFileName,HI_UNF_PVR_FP_INFO_S* pstFPInfo);

/**
\brief  Get the corresponding information of maturity rate based on the number from the file of CA private data
CNcomment: 根据number 从CA 私有数据文件中获取相应的成人级信息 CNend
\attention \n
None CNcomment:无 CNend

\param[in]  pCAPrivateFileName  The name of CA private data file .      CNcomment:CA 私有数据文件名字 CNend
\param[in]  u32URINum  The number of information of maturity level .  CNcomment:成人级信息number  CNend
\param[in]  pstPVRURI   The information of maturity rate                   CNcomment:成人级信息 CNend

\retval ::HI_SUCCESS  Get successfully                  CNcomment:获取成功 CNend
\retval ::HI_FAILURE  Fail to get.                          CNcomment:获取失败 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_GetURI( HI_CHAR * pCAPrivateFileName ,HI_U32 u32URINum, HI_UNF_PVR_URI_S* pstURI);

/**
\brief  Get the corresponding information of fingerprint based on the number from the file of CA private data
CNcomment: 根据number 从CA 私有数据文件中获取相应的指纹信息 CNend
\attention \n
None CNcomment:无 CNend

\param[in]  pCAPrivateFileName  The name of CA private data file .      CNcomment:CA 私有数据文件名字 CNend
\param[in]  u32FPNum  The number of information of fingerprint .  CNcomment:指纹信息number  CNend
\param[in]  pstPVRURI   The information of fingerprint                   CNcomment:指纹信息 CNend

\retval ::HI_SUCCESS  Get successfully                  CNcomment:获取成功 CNend
\retval ::HI_FAILURE  Fail to get.                          CNcomment:获取失败 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_GetFP( HI_CHAR * pCAPrivateFileName,HI_U32 u32FPNum,HI_UNF_PVR_FP_INFO_S* pstFPInfo);

/**
\brief  Get the numbers of maturity rate and fingerprint from the file of CA private data
CNcomment: 从CA 私有数据文件中获取成人级和指纹信息的数量 CNend
\attention \n
None CNcomment:无 CNend

\param[in]  pCAPrivateFileName  The name of CA private data file .        CNcomment:CA 私有数据文件名字 CNend
\param[in]  u32URINum  The numbers of information of maturity rate .   CNcomment:成人级信息的数量 CNend
\param[in]  u32FPNum   The numbers of information of fingerprint           CNcomment:指纹信息的数量 CNend

\retval ::HI_SUCCESS  Get successfully                  CNcomment:获取成功 CNend
\retval ::HI_FAILURE  Fail to get.                          CNcomment:获取失败 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_GetURIAndFPNum( HI_CHAR * pCAPrivateFileName,HI_U32* u32URINum,HI_U32* u32FPNum);

/**
\brief  Calculte the AES_CMAC value of data  
CNcomment: 计算数据的AES_CMAC 值 CNend
\attention \n
None CNcomment:无 CNend

\param[in]  buffer  pointer of data buffer .        CNcomment:数据buffer 指针 CNend
\param[in]  Length  The length  of data .   CNcomment:数据长度 CNend
\param[in]  Key   The key used in Calculte the AES_CMAC of data           CNcomment: AES_CMAC 计算中使用的key  CNend
\param[out]  MAC   The AES_CMAC value          CNcomment:AES_CMAC 值 CNend

\retval ::HI_SUCCESS  Success                 CNcomment:成功 CNend
\retval ::HI_FAILURE    Failure                     CNcomment:失败 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_CalculteAES_CMAC(HI_U8 *buffer, HI_U32 Length, HI_U8 Key[16], HI_U8 MAC[16]);

/**
\brief  Get the status of PVR recording  
CNcomment: 获取 PVR 录制信息 CNend
\attention \n
None CNcomment:无 CNend

\param[in]  u32RecChnID  The channel ID of record.        CNcomment:录制通道ID  CNend
\param[out]  pstRecStatus  The status of record channel.   CNcomment:   录制通道上状态信息  CNend


\retval ::HI_SUCCESS  Success                 CNcomment:成功 CNend
\retval ::HI_FAILURE    Failure                     CNcomment:失败 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_PVR_GetRecStatus(HI_U32 u32RecChnID, HI_UNF_ADVCA_PVR_REC_STATUS_S *pstRecStatus);

/**
\brief Sets the algorithm of the SP key ladder CNcomment:设置SP key ladder的算法    CNend
\attention \n
You must set an algorithm before using a key ladder in a session. The default algorithm is TDES.
It is recommended that you retain the algorithm in a session.
CNcomment:每次会话过程中使用key ladder之前，须设置具体算法, 系统初始默认值 HI_UNF_ADVCA_ALG_TYPE_TDES；\n
本次会话过程中，建议保持算法的稳定不变。 CNend
\param[in] enType Key ladder algorithm CNcomment:enType  key ladder算法\n  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_ALG_TYPE_E
*/
HI_S32 HI_UNF_ADVCA_SetSPAlg(HI_UNF_ADVCA_ALG_TYPE_E enType);

/**
\brief Obtains the algorithm of the SP key ladder CNcomment: 获取 SP key ladder的算法    CNend
\attention \n
None CNcomment:无 CNend
\param[in] pEnType Key ladder algorithm CNcomment:pEnType  key ladder算法\n  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_ALG_TYPE_E
*/
HI_S32 HI_UNF_ADVCA_GetSPAlg(HI_UNF_ADVCA_ALG_TYPE_E *pEnType);

/**
\brief Sets the SP key ladder stage CNcomment:设置SP key ladder的级数    CNend
\attention \n
The key ladder stage can be set only once before delivery and cannot be changed. Please use default value.
CNcomment:机顶盒出厂时设置 仅能设置一次 不可更改,不建议使用该接口改变stage  CNend
\param[in] enStage Key ladder stage Its value is HI_UNF_ADVCA_KEYLADDER_LEV2 or HI_UNF_ADVCA_KEYLADDER_LEV3.
CNcomment:enStage  key ladder级数\n 取值只能为HI_UNF_ADVCA_KEYLADDER_lev2 或者 HI_UNF_ADVCA_KEYLADDER_lev3  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_KEYLADDER_LEV_E
*/
HI_S32 HI_UNF_ADVCA_SetSPKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage);

/**
\brief Obtains the SP key ladder stage CNcomment:获取SP key ladder的级数 CNend
\attention \n
None CNcomment:无 CNend
\param[out] penStage Key ladder stage CNcomment:penStage   key ladder级数 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_KEYLADDER_LEV_E
*/
HI_S32 HI_UNF_ADVCA_GetSPKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E *penStage);


/**
\brief Sets session keys for a SP key ladder CNcomment:为SP key ladder配置会话密钥    CNend
\attention \n
The stage of the session key cannot be greater than the configured stage of the key ladder. The last stage of the session key is configured by calling the API of the CIPHER module rather than this API.
That is, only session key 1 and session key 2 need to be configured for a 3-stage key ladder.
Only session key 1 needs to be configured for a 2-stage key ladder.
You need to set the key ladder stage by calling HI_UNF_ADVCA_SetSPKeyLadderStage first.
 Session keys can be set during initialization or changed at any time.
CNcomment:注意配置的级数不能超过设置的级数值，最后一级由Descrambler模块内部配置，不用通过此接口配置。\n
也就是说，对于3级key ladder，只用配置会话密钥1和会话密钥2。\n
对于2级的key ladder，只用配置会话密钥1。\n
请先调用HI_UNF_ADVCA_SetSPKeyLadderStage设置key ladder级数。\n
会话密钥可以初始时设置一次，也可以随时修改。 CNend
\param[in] enStage Key ladder stage Its value is HI_UNF_ADVCA_KEYLADDER_LEV2 or HI_UNF_ADVCA_KEYLADDER_LEV3.
CNcomment:enStage    密钥级数，[HI_UNF_ADVCA_KEYLADDER_LEV1 ~ HI_UNF_ADVCA_KEYLADDER_LEV2]  CNend
\param[in] pu8Key Protection key pointer, 128 bits (16 bytes) in total CNcomment:pu8Key     保护密钥指针，共128bit(16byte)  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS  成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\retval ::HI_ERR_CA_WAIT_TIMEOUT Timeout occurs when the CA module waits for encryption or decryption
CNcomment:HI_ERR_CA_WAIT_TIMEOUT CA等待加解密超时 CNend
\retval ::HI_ERR_CA_R2R_DECRYPT The CW decryption fails CNcomment:HI_ERR_CA_CW_DECRYPT   CW解密失败 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetSPSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key);

/**
\brief Sets the descramble mode of the SP key ladder CNcomment:设置SP key ladder的解扰算法    CNend
\attention \n
You must set a descramble mode before using a key ladder in a session. The default algorithm is HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_AES_CBC_CI.
It is recommended that you retain the descramble mode in a session.
CNcomment:每次会话过程中使用key ladder之前，须设置具体算法, 系统初始默认值 HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_AES_CBC_CI；\n
本次会话过程中，建议保持算法的稳定不变。 CNend
\param[in] enType Key ladder algorithm CNcomment:enType  key ladder算法\n  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_ALG_TYPE_E
*/
HI_S32 HI_UNF_ADVCA_SetSPDscMode(HI_UNF_ADVCA_SP_DSC_MODE_E enType);

/**
\brief Obtains the descramble mode of the SP key ladder CNcomment: 获取 SP key ladder的算法    CNend
\attention \n
None CNcomment:无 CNend
\param[in] pEnType Key ladder algorithm CNcomment:pEnType  key ladder算法\n  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\see \n
::HI_UNF_ADVCA_ALG_TYPE_E
*/
HI_S32 HI_UNF_ADVCA_GetSPDscMode(HI_UNF_ADVCA_SP_DSC_MODE_E *pEnType);

/** 
\brief  Loads link protection keys (LPKs) CNcomment: 加载LPK  CNend
\attention
\param[in] pEbcryptedLpk LPK encrypted by R2RROOTKEY CNcomment:pEncryptedLpk  用R2RROOTKEY加密的LPK  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS               成功 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT       CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA   输入参数非法 CNend
\retval ::HI_ERR_CA_NOT_SUPPORT The function is not supported CNcomment:HI_ERR_CA_NOT_SUPPORT    功能不支持 CNend
\retval ::HI_ERR_CA_WAIT_TIMEOUT Timeout occurs when the CA module waits for encryption or decryption
CNcomment:HI_ERR_CA_WAIT_TIMEOUT   CA等待加解密超时 CNend
\retval ::HI_ERR_CA_LPK_DECRYPT The API fails to load the LPK CNcomment:HI_ERR_CA_LPK_DECRYPT    LPK解密失败 CNend
\see \n
None CNcomment:无 CNend
*/

HI_S32 HI_UNF_ADVCA_LoadLpk(HI_U8 *pEncryptedLpk);

/** 
\brief Decrypts a block of link protection key (LPK)-encrypted data
CNcomment:解密智能卡传送给CPU的经过链路保护的数据 (Link Protection)  CNend
\attention
\param[in] pCipherText buffer of the cipher text to be decrypted  CNcomment:pCipherText   密文 CNend
\param[in] s32TextLen the length of the cipher text should be larger than 8 byte CNcomment:s32TextLen    密文长度 要求大于一个Block长度 8byte  CNend
\param[out] pPlainText the buffer to store the clear text CNcomment:pPlainText   明文 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS               成功 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT       CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA   输入参数非法 CNend
\retval ::HI_ERR_CA_NOT_SUPPORT The function is not supported CNcomment:HI_ERR_CA_NOT_SUPPORT    功能不支持 CNend
\retval ::HI_ERR_CA_WAIT_TIMEOUT Timeout occurs when the CA module waits for encryption or decryption
CNcomment:HI_ERR_CA_WAIT_TIMEOUT   CA等待加解密超时 CNend
\retval ::HI_ERR_CA_LPK_DECRYPT The API fails to decrypt the LPK Param CNcomment:HI_ERR_CA_LPK_DECRYPT    LPK解密失败 CNend
\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_DecryptLptParam(HI_U8 *pCipherText,HI_S32 s32TextLen,HI_U8 *pPlainText);


/**
  \brief this function is used by the Novel CA for the encryption and decryption setting
   CNcomment:Novel内存加解密算法和key配置 CNend
  \attention \n
  This function is used for the Novel encryption and decrption
  CNcomment:  本函数是提供给 Novel 内存加解密方案使用，用作配置加解密过程keyladder算法和key CNend
  \retval ::HI_SUCCESS Success  CNcomment:HI_SUCCESS 成功 CNend
  \retval ::HI_FAILURE This API fails to be called  CNcomment:HI_FAILURE  API系统调用失败 CNend
  \see  \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_NovelCryptologyConfig(HI_VOID);

/**
  \brief Used for Novel CA to encrypt the data  CNcomment:Novel内存数据加密 CNend
  \attention \n
  None  CNcomment: 无 CNend
  \param[in]  pPlainText: the buffer to store the data to be encrypted CNcomment:pPlainText 待加密数据指针 CNend
  \param[in]  TextLen: the length of the data to be encrypted CNcomment:TextLen    待加密数据长度 CNend
  \param[in]  pCipherText: the buffer to store the encrypted data CNcomment:pCipherText 加密后数据指针 CNend
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_NovelDataEncrypt(HI_U8 *pPlainText, HI_U32 TextLen, HI_U8 *pCipherText);

/**
  \brief This function is used for Novel to decrypt the data CNcomment:TextNovel内存数据解密 CNend
  \attention \n
  None  CNcomment:无 CNend
  \param[in]  pPlainText: point to the data to be decrypted CNcomment:pPlainText 待解密密数据指针 CNend
  \param[in]  TextLen: the length of the data to be decrypted CNcomment:TextLen    待解密数据长度 CNend
  \param[in]  pCipherText: the buffer store the decrypted data CNcomment:pCipherText 解密后数据指针 CNend
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \see  \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_NovelDataDecrypt(HI_U8 *pCipherText, HI_U32 TextLen, HI_U8 *pPlainText);

/**
\brief Sets oem root key， it's mainly used for widevine keyladder CNcomment:OEM_root_key 主要用于Widevine keyladder    CNend
\attention \n
This root key could only be written only once be careful, thanks 	CNcomment: 这个密钥只允许写一次,请谨慎操作 CNend
\param[in] pu8OEMRootKey buffer of the input key	CNcomment:pu8OEMRootKey    输入密钥的buffer  CNend
\param[in] u32KeyLen key length of the input CNcomment:u32KeyLen     输入密钥的长度，限定为16字节  CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS  成功 CNend
\retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
\retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
\retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
\retval ::HI_ERR_CA_R2R_DECRYPT The CW decryption fails CNcomment:HI_ERR_CA_CW_DECRYPT   CW解密失败 CNend

\see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetOEMRootKey(HI_U8 *pu8OEMRootKey, HI_U32 u32KeyLen);

/**
  \brief This function is used to open the DCAS CNcomment:用于打开DCAS CNend
  \attention \n
  None  CNcomment:无 CNend
  \param[in]  enAlg: DCAS keyladder algorithm CNcomment:enAlg DCAS keyladder算法 CNend  
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \see  \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_DCASOpen(HI_UNF_CIPHER_ALG_E enAlg);

/**
  \brief This function is used to get the DCAS CHIPID CNcomment:用于获取DCAS CHIPID
  \attention \n
  None  CNcomment:无 CNend
  \param[out]  pu32MSBID: First 4 bytes of DCAS CHIPID CNcomment:DCAS CHIPID前4个字节 CNend  
  \param[out]  pu32LSBID: last 4 bytes of DCAS CHIPID CNcomment:DCAS CHIPID后4个字节 CNend 
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \see  \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetDCASChipId(HI_U32 *pu32MSBID, HI_U32 *pu32LSBID);

/**
  \brief This function is used to set the session key CNcomment:用于设置DCAS session密钥 CNend
  \attention \n
  None  CNcomment:无 CNend
  \param[in]  enDCASLevel: the DCAS keyladder level CNcomment:enAlg DCAS keyladder级数 CNend  
  \param[in]  pu8Key: the DCAS keyladder session key CNcomment:enAlg DCAS keyladder密钥 CNend  
  \param[in]  pu8Output: the output of the DCAS keyladder CNcomment:pu8Output DCAS keyladder输出 CNend  
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_LEV_E enDCASLevel, HI_U8 au8Key[16], HI_U8 au8Output[16]);

/**
  \brief This function is used to close the DCAS keyladder CNcomment:用于关闭DCAS CNend
  \attention \n
  None  CNcomment:无 CNend
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \see  \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_DCASClose(HI_VOID);

/**
  \brief This function is used to set the OTP fuse. CNcomment:用于设置OTP位 CNend
  \attention \n
  None  CNcomment:无 CNend
  \param[in]  enOtpFuse: the OTP fuse ID CNcomment:enOtpFuse 待设置的OTP位ID CNend    
  \param[in]  pstOtpFuseAttr: the OTP fuse attribute CNcomment:pstOtpFuseAttr 待设置的OTP位属性 CNend  
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS  成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
  \retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
  \see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse, HI_UNF_ADVCA_OTP_ATTR_S *pstOtpFuseAttr);

/**
  \brief This function is used to get the OTP fuse. CNcomment:用于获取OTP位 CNend
  \attention \n
  None  CNcomment:无 CNend
  \param[in]  enOtpFuse: the OTP fuse ID CNcomment:enOtpFuse 待获取的OTP位ID CNend      
  \param[in]  pstOtpFuseAttr: the OTP fuse attribute CNcomment:pstOtpFuseAttr 获取的OTP位属性 CNend  
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS  成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
  \retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
  \see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse, HI_UNF_ADVCA_OTP_ATTR_S *pstOtpFuseAttr);

/**
  \brief This function is used to set the keyladder attribute. CNcomment:用于设置keyladder属性 CNend
  \attention \n
  None  CNcomment:无 CNend
  \param[in]  enKeyLadderType: The keyladder type ID CNcomment:enKeyLadderType Keyladder类型ID CNend    
  \param[in]  pstKeyladderAttr: The keyladder attribute CNcomment:pstKeyladderAttr 待设置的keyladder属性CNend  
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS  成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
  \retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
  \see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_TYPE_E enKeyLadderType, HI_UNF_ADVCA_KEYLADDER_ATTR_S *pstKeyladderAttr);

/**
  \brief This function is used to achieve the CA vendor privated function. CNcomment:用于完成CA厂家特有的操作 CNend
  \attention \n
  None  CNcomment:无 CNend
  \param[in]  enCaVendor: The CA vendor type ID CNcomment:enCaVendor CA vendor类型ID CNend    
  \param[in]  pstCavendorOpt: The parameter to achieve the CA vendor private function CNcomment:pstCavendorOpt 完成CA厂家特有功能需要的参数CNend  
  \retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS  成功 CNend
  \retval ::HI_FAILURE This API fails to be called CNcomment:HI_FAILURE  API系统调用失败 CNend
  \retval ::HI_ERR_CA_NOT_INIT The advanced CA module is not initialized. CNcomment:HI_ERR_CA_NOT_INIT CA未初始化 CNend
  \retval ::HI_ERR_CA_INVALID_PARA The input parameter value is invalid CNcomment:HI_ERR_CA_INVALID_PARA 输入参数非法 CNend
  \see \n
None CNcomment:无 CNend
*/
HI_S32 HI_UNF_ADVCA_CaVendorOperation(HI_UNF_ADVCA_VENDORID_E enCaVendor, HI_UNF_ADVCA_CA_VENDOR_OPT_S *pstCaVendorOpt);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_ADVCA_H__ */


