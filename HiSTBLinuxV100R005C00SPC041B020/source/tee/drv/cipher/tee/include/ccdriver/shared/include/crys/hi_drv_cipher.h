/******************************************************************************
 *
 * Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
 * ******************************************************************************
 * File Name     : hi_drv_cipher.h
 * Version       : Initial
 * Author        : Hisilicon hisecurity team
 * Created       : 2013-12-28
 * Last Modified :
 * Description   : 
 * Function List :
 * History       :
 * ******************************************************************************/

#ifndef __HI_DRV_CIPHER_H__
#define __HI_DRV_CIPHER_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/***************************** Macro Definition ******************************/
#define CIPHER_SOFT_CHAN_NUM      CIPHER_CHAN_NUM
#define HASH_CHANNAL_MAX_NUM   (8)

#define CIPHER_INVALID_CHN        (0xffffffff)

/***/
typedef struct hiCIPHER_DATA_INFO_S
{
    HI_U32  u32src;          /**< */
    HI_U32  u32dest;         /**< */
    HI_U32  u32length;       /**< */
    HI_BOOL bDecrypt;        /**< */
    HI_U32  u32DataPkg[4];   /**< */
}HI_DRV_CIPHER_DATA_INFO_S;

typedef struct  hiCIPHER_DATA_S
{
    HI_HANDLE CIHandle;
    HI_U32 ScrPhyAddr;
    HI_U32 DestPhyAddr;
    HI_U32 u32DataLength;
}CIPHER_DATA_S;

typedef struct
{
    HI_U8  *pu8Input;
    HI_U8  *pu8Output;
    HI_U32  u32DataLen;
    HI_U8  *pu8N;
	HI_U8  *pu8K;
    HI_U8  *pu8P;                      /*!<  1st prime factor  */ /**< CNcomment: 指向RSA私钥P的指针*/
    HI_U8  *pu8Q;                      /*!<  2nd prime factor  */ /**< CNcomment: 指向RSA私钥Q的指针*/
    HI_U8  *pu8DP;                     /*!<  D % (P - 1)       */ /**< CNcomment: 指向RSA私钥DP的指针*/
    HI_U8  *pu8DQ;                     /*!<  D % (Q - 1)       */ /**< CNcomment: 指向RSA私钥DQ的指针*/
    HI_U8  *pu8QP;                     /*!<  1 / (Q % P)       */ /**< CNcomment: 指向RSA私钥QP的指针*/
    HI_U16 u16NLen;
    HI_U16 u16KLen;
    HI_U16 u16DLen;                   /**< length of private exponent */ /**< CNcomment: RSA私钥D的长度*/
    HI_U16 u16PLen;                   /**< length of 1st prime factor */ /**< CNcomment: RSA私钥P的长度*/
    HI_U16 u16QLen;                   /**< length of 2nd prime factor */ /**< CNcomment: RSA私钥Q的长度*/
    HI_U16 u16DPLen;                  /**< length of D % (P - 1) */      /**< CNcomment: RSA私钥DP的长度*/
    HI_U16 u16DQLen;                  /**< length of D % (Q - 1) */      /**< CNcomment: RSA私钥DQ的长度*/
    HI_U16 u16QPLen;                  /**< length of 1 / (Q % P) */      /**< CNcomment: RSA私钥QP的长度*/
}CIPHER_RSA_DATA_S;

/** RSA private key struct */
/** CNcomment:RSA私钥结构体 */
typedef struct
{
    HI_U8 *pu8N;                      /*!<  public modulus    */ /**< CNcomment: 指向RSA公钥N的指针*/
    HI_U8 *pu8E;                      /*!<  public exponent   */ /**< CNcomment: 指向RSA公钥E的指针*/
    HI_U8 *pu8D;                      /*!<  private exponent  */ /**< CNcomment: 指向RSA私钥D的指针*/
    HI_U8 *pu8P;                      /*!<  1st prime factor  */ /**< CNcomment: 指向RSA私钥P的指针*/
    HI_U8 *pu8Q;                      /*!<  2nd prime factor  */ /**< CNcomment: 指向RSA私钥Q的指针*/
    HI_U8 *pu8DP;                     /*!<  D % (P - 1)       */ /**< CNcomment: 指向RSA私钥DP的指针*/
    HI_U8 *pu8DQ;                     /*!<  D % (Q - 1)       */ /**< CNcomment: 指向RSA私钥DQ的指针*/
    HI_U8 *pu8QP;                     /*!<  1 / (Q % P)       */ /**< CNcomment: 指向RSA私钥QP的指针*/
    HI_U16 u16NLen;                   /**< length of public modulus */   /**< CNcomment: RSA公钥N的长度*/
    HI_U16 u16ELen;                   /**< length of public exponent */  /**< CNcomment: RSA公钥E的长度*/
    HI_U16 u16DLen;                   /**< length of private exponent */ /**< CNcomment: RSA私钥D的长度*/
    HI_U16 u16PLen;                   /**< length of 1st prime factor */ /**< CNcomment: RSA私钥P的长度*/
    HI_U16 u16QLen;                   /**< length of 2nd prime factor */ /**< CNcomment: RSA私钥Q的长度*/
    HI_U16 u16DPLen;                  /**< length of D % (P - 1) */      /**< CNcomment: RSA私钥DP的长度*/
    HI_U16 u16DQLen;                  /**< length of D % (Q - 1) */      /**< CNcomment: RSA私钥DQ的长度*/
    HI_U16 u16QPLen;                  /**< length of 1 / (Q % P) */      /**< CNcomment: RSA私钥QP的长度*/
}CIPHER_RSA_PRI_KEY_S;

typedef struct
{
    CIPHER_RSA_PRI_KEY_S stPriKey;
    HI_U32 u32NumBits;
    HI_U32 u32Exponent;
}CIPHER_RSA_KEY_S;

typedef struct hiCIPHER_TASK_S
{
    HI_DRV_CIPHER_DATA_INFO_S         	stData2Process;
    HI_U32                      u32CallBackArg;
}HI_DRV_CIPHER_TASK_S;

typedef enum hiCIPHER_HDCP_MODE_E
{
    CIPHER_HDCP_MODE_NO_HDCP_KEY                = 0x0,
    CIPHER_HDCP_MODE_HDCP_KEY,
    CIPHER_HDCP_MODE_BUTT,
}HI_DRV_CIPHER_HDCP_KEY_MODE_E;

typedef enum hiCIPHER_HDCP_KEY_RAM_MODE_E
{
    CIPHER_HDCP_KEY_RAM_MODE_READ         		= 0x0,
    CIPHER_HDCP_KEY_RAM_MODE_WRITE,
    CIPHER_HDCP_KEY_RAM_MODE_BUTT,
}HI_DRV_CIPHER_HDCP_KEY_RAM_MODE_E;

typedef enum hiCIPHER_HDCP_KEY_TYPE_E
{
    CIPHER_HDCP_KEY_TYPE_OTP_ROOT_KEY           = 0x0,
    CIPHER_HDCP_KEY_TYPE_HISI_DEFINED,
    CIPHER_HDCP_KEY_TYPE_HOST_ROOT_KEY,
    CIPHER_HDCP_KEY_TYPE_BUTT,
}HI_DRV_CIPHER_HDCP_ROOT_KEY_TYPE_E;

typedef struct hiCIPHER_FLASH_ENCRYPT_HDCPKEY_S
{
    HI_U8 u8Key[332];
}HI_DRV_CIPHER_FLASH_ENCRYPT_HDCPKEY_S;

typedef struct hiCIPHER_PBKDF2_S
{
    HI_U8 *password;
    HI_U32 plen; 
    HI_U8 *salt; 
    HI_U32 slen;
    HI_U32 iteration_count;
    HI_U32 key_length;
}CIPHER_PBKDF2_S;

typedef enum 
{
    CIPHER_TEST_PRINT_PHY = 0x01,
    CIPHER_TEST_PRINT_VIA,  
    CIPHER_TEST_MEMSET,
    CIPHER_TEST_MEMCMP,
    CIPHER_TEST_READ_REG,
    CIPHER_TEST_WRITE_REG,
    CIPHER_TEST_AES  = 0x10,
    CIPHER_TEST_HMAC,
    CIPHER_TEST_RSA,
    CIPHER_TEST_HASH,
    CIPHER_TEST_DES,
    CIPHER_TEST_RSA_PRIM,
    CIPHER_TEST_RSA_KG,
    CIPHER_TEST_RND,  
    CIPHER_TEST_BUTT,
}CIPHER_TEST;

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	/* End of #ifndef __HI_DRV_CIPHER_H__*/

