/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_unf_advca.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2014-09-16
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef __HI_UNF_ADVCA_H__
#define __HI_UNF_ADVCA_H__

#include "hi_type.h"

/** advanced CA session serect key class*/
typedef enum hiUNF_ADVCA_KEYLADDER_LEV_E
{
	HI_UNF_ADVCA_KEYLADDER_LEV1 = 0,   /**<session serect key level 1*/
	HI_UNF_ADVCA_KEYLADDER_LEV2 ,	   /**<session serect key level 2*/
	HI_UNF_ADVCA_KEYLADDER_LEV3 ,	   /**<session serect key level 3*/
	HI_UNF_ADVCA_KEYLADDER_LEV4,       /**<session serect key level 4*/
	HI_UNF_ADVCA_KEYLADDER_LEV5,       /**<session serect key level 5*/
	HI_UNF_ADVCA_KEYLADDER_LEV6,       /**<session serect key level 6*/

	HI_UNF_ADVCA_KEYLADDER_BUTT 
}HI_UNF_ADVCA_KEYLADDER_LEV_E;


/** advanced CA Encrypt arith*/
typedef enum hiUNF_ADVCA_ALG_TYPE_E
{
    HI_UNF_ADVCA_ALG_TYPE_TDES = 0,    /**<Encrypt arith :3 DES*/
    HI_UNF_ADVCA_ALG_TYPE_AES,         /**<Encrypt arith : AES*/

    HI_UNF_ADVCA_ALG_TYPE_BUTT
}HI_UNF_ADVCA_ALG_TYPE_E;

/** FLASH device types*/
typedef enum hiUNF_ADVCA_FLASH_TYPE_E
{
    HI_UNF_ADVCA_FLASH_TYPE_SPI = 0,     /**<SPI flash*/
    HI_UNF_ADVCA_FLASH_TYPE_NAND,        /**<nand flash*/
    HI_UNF_ADVCA_FLASH_TYPE_NOR,         /**<nor flash*/
    HI_UNF_ADVCA_FLASH_TYPE_EMMC,        /**<eMMC*/
    HI_UNF_ADVCA_FLASH_TYPE_SPI_NAND ,   /**<spi_nand flash*/
    HI_UNF_ADVCA_FLASH_TYPE_SD,          /**<FSD/TSD flash*/
    HI_UNF_ADVCA_FLASH_TYPE_BUTT
}HI_UNF_ADVCA_FLASH_TYPE_E;

typedef enum hiUNF_ADVCA_SP_DSC_MODE_E
{
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_CSA2      		= 0x0000,   /**<Demux PAYLOAD CSA2 */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_CSA3      		= 0x0010,   /**<Demux PAYLOAD CSA3 */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_AES_CBC_IDSA 		= 0x0020,   /**<Demux PAYLOAD AES  IPTV Mode */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_AES_ECB          	= 0x0021,   /**<Demux PAYLOAD AES  ECB Mode */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_AES_CBC_CI     	= 0x0022,   /**<Demux PAYLOAD AES  CIPLUS */
     HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_DES_IPTVT     	= 0x0030,   /**<Demux PAYLOAD DES  CIPLUS */
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

/**Defines the attribute of the CSA3 keyladder type ID.*/
/**CNcomment: 定义CSA3 keyladder属性类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_CSA3_ATTR_E
{
    HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_ALG = 0,            /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_SESSION_KEY,   /**<Keyladder session key*/
    HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_BUTT
}HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_E;

/**Defines the attribute of the CSA3 keyladder.*/
/**CNcomment: 定义CSA3 keyladder属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_CSA3_ATTR_S
{
    HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_E  enCsa3KlAttr; /**<CSA3 keyladder attribute ID*/
    HI_UNF_ADVCA_ALG_TYPE_E enAlgType;          /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage;   /**<Keyladder stage of the session key to be set*/
    HI_U8 u8SessionKey[16];                                    /**<Session key*/  
}HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_S;


/**Defines the attribute of the CSA2 keyladder type ID.*/
/**CNcomment: 定义CSA2 keyladder属性类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_CSA2_ATTR_E
{
    HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_ALG = 0,            /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_SESSION_KEY,   /**<Keyladder session key*/
    HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_BUTT
}HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_E;

/**Defines the attribute of the R2R keyladder type ID.*/
/**CNcomment: 定义R2R keyladder属性类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_R2R_ATTR_E
{
    HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_ALG = 0,            /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_SESSION_KEY,   /**<Keyladder session key*/
    HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_BUTT
}HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_E;

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

/**Defines the attribute of the LP keyladder type ID.*/
/**CNcomment: 定义LP keyladder属性类型ID.*/
typedef enum hiUNF_ADVCA_KEYLADDER_LP_ATTR_E
{
    HI_UNF_ADVCA_KEYLADDER_LP_ATTR_LOAD_LPK = 0,            /**<Load Link protection key*/
    HI_UNF_ADVCA_KEYLADDER_LP_ATTR_DECRYPT_PROTECTED_DATA,   /**<Decrypt Link protected data*/
    HI_UNF_ADVCA_KEYLADDER_LP_ATTR_BUTT
}HI_UNF_ADVCA_KEYLADDER_LP_ATTR_E;

/**Defines the attribute of the CSA2 keyladder.*/
/**CNcomment: 定义CSA2 keyladder属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_CSA2_ATTR_S
{
    HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_E  enCsa2KlAttr; /**<CSA2 keyladder attribute ID*/
    HI_UNF_ADVCA_ALG_TYPE_E enAlgType;          /**<Keyladder algorithm, TDES/AES*/
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage;   /**<Keyladder stage of the session key to be set*/
    HI_U8 u8SessionKey[16];                                   /**<Session key*/  
}HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_S;


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

/**Defines the attribute of all of the keyladder.*/
/**CNcomment: 定义所有 keyladder属性结构体.*/
typedef struct hiUNF_ADVCA_KEYLADDER_ATTR_S
{
    union
    {
        HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_S 	stCSA2KlAttr;   /**<CSA2 keyladder attribute*/
        HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_S 	stCSA3KlAttr;   /**<CSA3 keyladder attribute*/
        HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_S          stR2RKlAttr;     /**<R2R keyladder attribute*/
        HI_UNF_ADVCA_KEYLADDER_SP_ATTR_S 	        stSPKlAttr;       /**<SP keyladder attribute*/   
        HI_UNF_ADVCA_KEYLADDER_LP_ATTR_S             stLpKlAttr;       /**<LP keyladder attribute*/  
        HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_S        stMiscKlAttr;    /**<MISC keyladder attribute*/
        HI_UNF_ADVCA_KEYLADDER_TA_ATTR_S            stTAKlAttr;     /**<TA keyladder attribute, only for Irdeto MSR2.2*/
    }unKlAttr;
}HI_UNF_ADVCA_KEYLADDER_ATTR_S;

typedef enum hiUNF_ADVCA_KEYLADDER_TYPE_E
{
    HI_UNF_ADVCA_KEYLADDER_CSA2 = 0,    /**<CSA2 keyladder*/
    HI_UNF_ADVCA_KEYLADDER_CSA3,          /**<CSA3 keyladder*/
    HI_UNF_ADVCA_KEYLADDER_R2R,             /**<R2R keyladder*/
    HI_UNF_ADVCA_KEYLADDER_SP,              /**<SP keyladder*/
    HI_UNF_ADVCA_KEYLADDER_MISC,        /**<MISC keyladder*/
    HI_UNF_ADVCA_KEYLADDER_LP,              /**<LP keyladder*/
    HI_UNF_ADVCA_KEYLADDER_TA,            /**<TA keyladder, only for Irdeto MSR2.2*/ 
    HI_UNF_ADVCA_KEYLADDER_TYPE_BUTT
}HI_UNF_ADVCA_KEYLADDER_TYPE_E;

typedef struct
{
    HI_UNF_ADVCA_KEYLADDER_TYPE_E enKeyLadderType;
    HI_UNF_ADVCA_KEYLADDER_ATTR_S stKeyladderAttr;
}CA_KEYLADDER_ATTR_S;


/**Defines the attribute of the CA vendor type ID.*/
/**CNcomment: ?¨ò?CA vendorê?D?ààDíID.*/
typedef enum hi_ADVCA_CA_VENDOR_ATTR_E
{
    HI_ADVCA_CAVENDOR_ATTR_IRDETO_CBCMAC_CALC,  /**<To calculate the high level code CBC-MAC, ONLY used for Irdeto MSR2.2.*/
    HI_ADVCA_CAVENDOR_ATTR_IRDETO_CBCMAC_AUTH,  /**<To authenticate the high level code CBC-MAC, ONLY used for Irdeto MSR2.2.*/    
    HI_ADVCA_CAVENDOR_ATTR_BUTT
}HI_ADVCA_CA_VENDOR_OPT_E;


/** Advca CA VendorID */
typedef enum hi_ADVCA_VENDORID_E
{
    HI_ADVCA_NULL       = 0x00,        /**<No-Advcance CA chipset, Marked with 0*/
    HI_ADVCA_NAGRA      = 0x01,        /**<NAGRA  Chipse, Marked with R*/
    HI_ADVCA_IRDETO     = 0x02,        /**<IRDETO Chipset, Marked with I*/
    HI_ADVCA_CONAX      = 0x03,        /**<CONAX Chipset, Marked with C*/
    HI_ADVCA_SUMA       = 0x05,        /**<SUMA Chipset, Marked with S*/
    HI_ADVCA_NOVEL      = 0x06,        /**<NOVEL Chipset, Marked with Y*/
    HI_ADVCA_VERIMATRIX = 0x07,        /**<VERIMATRIX Chipset, Marked with M*/
    HI_ADVCA_CTI        = 0x08,        /**<CTI Chipset, Marked with T*/
    HI_ADVCA_COMMONDCA  = 0x0b,        /**<COMMONCA Chipset, Marked with H*/
    HI_ADVCA_DCAS       = 0x0c,        /**<DCAS CA Chipset*/
    HI_ADVCA_VENDORIDE_BUTT
}HI_UNF_ADVCA_VENDORID_E;

/**Defines the attribute of the Irdeto MSR2.2 high level code CBC-MAC.*/
/**CNcomment: ?¨ò?Irdeto MSR2.2 é?2?ó|ó?CBC-MACê?D??á11ì?.*/
typedef struct hi_ADVCA_IRDETO_CBCMAC_ATTR_S
{
    HI_U8 *pu8InputData;
    HI_U32 u32InputDataLen;
    HI_U8 u8OutputCbcMac[16];
    HI_U8 u8RefCbcMAC[16];  /**<Reference CBC-MAC*/
    HI_U32 u32AppLen; /**<High level code length*/   
}HI_ADVCA_IRDETO_CBCMAC_OPT_S;



/**Defines the attribute of the CA vendor private function.*/
typedef struct hi_ADVCA_CA_VENDOR_OPT_S
{	
    HI_ADVCA_CA_VENDOR_OPT_E enCaVendorOpt;
    union
    {
        HI_ADVCA_IRDETO_CBCMAC_OPT_S            stIrdetoCbcMac;
    }unCaVendorOpt;
}HI_UNF_ADVCA_CA_VENDOR_OPT_S;




typedef enum hiHI_UNF_ADVCA_OTP_FUSE_E
{
	HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG,
	HI_UNF_ADVCA_OTP_BUTT,
}HI_UNF_ADVCA_OTP_FUSE_E;

typedef struct
{
	HI_BOOL bIsLocked;
}HI_UNF_ADVCA_RSA_KEY_LOCK_FLAG_S;

typedef struct hiUNF_ADVCA_OTP_ATTR_S
{
	union
	{
		HI_UNF_ADVCA_RSA_KEY_LOCK_FLAG_S stRSAKeyLockFlag;
	}unOtpFuseAttr;
}HI_UNF_ADVCA_OTP_ATTR_S;


HI_S32 HI_UNF_ADVCA_Init(HI_VOID);

HI_S32 HI_UNF_ADVCA_DeInit(HI_VOID);

HI_S32 HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key);

HI_S32 HI_UNF_ADVCA_SetRSAKey(HI_U8 *pkey);

HI_S32 HI_UNF_ADVCA_GetRSAKey(HI_U8 *pkey);

HI_S32 HI_UNF_ADVCA_EnableSecBoot(HI_UNF_ADVCA_FLASH_TYPE_E enFlashType);

HI_S32 HI_UNF_ADVCA_GetSecBootStat(HI_BOOL *pbEnable,HI_UNF_ADVCA_FLASH_TYPE_E *penFlashType);

HI_S32 HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse, HI_UNF_ADVCA_OTP_ATTR_S *pstOtpFuseAttr);

HI_S32 HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse, HI_UNF_ADVCA_OTP_ATTR_S *pstOtpFuseAttr);

HI_S32 HI_UNF_ADVCA_CaVendorOperation(HI_UNF_ADVCA_VENDORID_E enCaVendor, HI_UNF_ADVCA_CA_VENDOR_OPT_S *pstCaVendorOpt);

HI_S32 HI_UNF_ADVCA_GetVendorID(HI_U32 *pu32VendorID);

HI_S32 HI_UNF_ADVCA_GetChipId(HI_U32 *pu32ChipId);

HI_S32 HI_UNF_ADVCA_GetStbSn(HI_U8 au8StbSn[4]);

HI_S32 HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_TYPE_E enKeyLadderType, HI_UNF_ADVCA_KEYLADDER_ATTR_S *pstKeyladderAttr);

HI_S32 HI_UNF_ADVCA_SetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_E enType);

HI_S32 HI_UNF_ADVCA_GetR2RKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E *penStage);

HI_S32 HI_UNF_ADVCA_EncryptSWPK(HI_U8 *pPlainSwpk,HI_U8 *pEncryptedSwpk);


#endif
