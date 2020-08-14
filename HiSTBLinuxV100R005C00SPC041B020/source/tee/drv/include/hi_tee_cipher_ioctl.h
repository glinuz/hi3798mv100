/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_cipher_ioctl.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/

#ifndef __TEE_DRV_CIPHER_IOCTL_H__
#define __TEE_DRV_CIPHER_IOCTL_H__

#include "hi_type.h"
#include "hi_tee_cipher.h"

#define TEE_CIPHER_IOC_NA	    0U
#define TEE_CIPHER_IOC_W	    1U
#define TEE_CIPHER_IOC_R	    2U
#define TEE_CIPHER_IOC_RW	    3U

#define HASH_BLOCK_SIZE		(64)

#define TEE_CIPHER_IOC(dir,type,nr,size) (((dir) << 30)|((size) << 16)|((type) << 8)|((nr) << 0))

#define TEE_CIPHER_IOR(type,nr,size)	TEE_CIPHER_IOC(TEE_CIPHER_IOC_R,(type),(nr), sizeof(size))
#define TEE_CIPHER_IOW(type,nr,size)	TEE_CIPHER_IOC(TEE_CIPHER_IOC_W,(type),(nr), sizeof(size))
#define TEE_CIPHER_IOWR(type,nr,size)	TEE_CIPHER_IOC(TEE_CIPHER_IOC_RW,(type),(nr),sizeof(size))

#define TEE_CIPHER_IOC_DIR(nr)		(((nr) >> 30) & 0x03)
#define TEE_CIPHER_IOC_TYPE(nr)		(((nr) >> 8) & 0xFF)
#define TEE_CIPHER_IOC_NR(nr)		(((nr) >> 0) & 0xFF)
#define TEE_CIPHER_IOC_SIZE(nr)		(((nr) >> 16) & 0x3FFF)

#define HI_ID_CIPHER 0x4D

HI_S32 TEE_CIPHER_Ioctl(HI_U32 cmd, HI_VOID *argp); 


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* special handle creation parameter */
typedef struct  hiCIPHER_HANDLE_S
{
    HI_HANDLE *phCIHandle;
    HI_TEE_CIPHER_ATTS_S *pstCipherAtts;
}CIPHER_HANDLE_S;

typedef struct hiCIPHER_Config_CTRL
{
    HI_HANDLE CIHandle;
    HI_TEE_CIPHER_CTRL_S *pstCIpstCtrl;
}CIPHER_Config_CTRL;

typedef struct  hiCIPHER_DATA_S
{
    HI_HANDLE CIHandle;
    HI_U32 ScrPhyAddr;
    HI_U32 DestPhyAddr;
    HI_U32 u32DataLength;
}CIPHER_DATA_S;

typedef struct  hiCIPHER_PKG_S
{
    HI_HANDLE CIHandle;
    HI_U32 u32PkgNum;
    HI_TEE_CIPHER_DATA_S* pu8Pkg;
}CIPHER_PKG_S;

typedef struct  hiCIPHER_INFO_S
{
    HI_HANDLE CIHandle;
    HI_TEE_CIPHER_INFO_S *pstInfo;
}CIPHER_INFO_S;

typedef struct
{
    HI_U32 u32TimeOutUs;
    HI_U32 *pu32RNG;
}CIPHER_RNG_S;

typedef enum
{
    HI_CIPHER_HMAC_KEY_FROM_CA  = 0,
    HI_CIPHER_HMAC_KEY_FROM_CPU = 1,
}CIPHER_HMAC_KEY_FROM_E;

typedef struct hiCIPHER_HASH_INIT_S
{
    HI_TEE_CIPHER_HASH_ATTS_S *pstHashAttr;
    HI_HANDLE *pHashHandle;
}CIPHER_HASH_INIT_S;

typedef struct hiCIPHER_HASH_UPDATE_S
{
    HI_HANDLE HashHandle;
    HI_U8 *pu8InputData;
    HI_U32 u32InputDataLen;
}CIPHER_HASH_UPDATE_S;

typedef struct hiCIPHER_HASH_FINISH_S
{
    HI_HANDLE HashHandle;
    HI_U8 *pu8OutputHash;
}CIPHER_HASH_FINISH_S;

typedef struct  hiCIPHER_TEST_DATA_S
{
    HI_U32 u32Cmd;
    HI_VOID *pu8Param;
    HI_U32  u32ParamSize;
}CIPHER_TEST_DATA_S;

typedef struct  hiCIPHER_PBKDF2_DATA_S
{
    HI_TEE_CIPHER_PBKDF2_S *pstInfo;
    HI_U8 *pu8Output;
}CIPHER_PBKDF2_DATA_S;


typedef struct hiCIPHER_RSA_DATA_S
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

#define    CMD_CIPHER_CREATEHANDLE           TEE_CIPHER_IOW(HI_ID_CIPHER,  0x1,  CIPHER_HANDLE_S)
#define    CMD_CIPHER_DESTROYHANDLE          TEE_CIPHER_IOW(HI_ID_CIPHER,  0x2,  HI_U32)
#define    CMD_CIPHER_CONFIGHANDLE           TEE_CIPHER_IOW(HI_ID_CIPHER,  0x3,  CIPHER_Config_CTRL)
#define    CMD_CIPHER_ENCRYPT                TEE_CIPHER_IOW(HI_ID_CIPHER,  0x4,  CIPHER_DATA_S)
#define    CMD_CIPHER_DECRYPT                TEE_CIPHER_IOW(HI_ID_CIPHER,  0x5,  CIPHER_DATA_S)
#define    CMD_CIPHER_DECRYPTMULTI           TEE_CIPHER_IOW(HI_ID_CIPHER,  0x6,  CIPHER_PKG_S)
#define    CMD_CIPHER_ENCRYPTMULTI           TEE_CIPHER_IOW(HI_ID_CIPHER,  0x7,  CIPHER_PKG_S)
#define    CMD_CIPHER_GETRANDOMNUMBER        TEE_CIPHER_IOW(HI_ID_CIPHER,  0x8,  CIPHER_RNG_S)
#define    CMD_CIPHER_GETHANDLECONFIG        TEE_CIPHER_IOW(HI_ID_CIPHER,  0x9,  CIPHER_Config_CTRL)
#define    CMD_CIPHER_CALCHASHINIT           TEE_CIPHER_IOW(HI_ID_CIPHER,  0xa,  CIPHER_HASH_INIT_S)
#define    CMD_CIPHER_CALCHASHUPDATE         TEE_CIPHER_IOW(HI_ID_CIPHER,  0xb,  CIPHER_HASH_UPDATE_S)
#define    CMD_CIPHER_CALCHASHFINAL          TEE_CIPHER_IOWR(HI_ID_CIPHER, 0xc,  CIPHER_HASH_FINISH_S)
#define    CMD_CIPHER_CALCRSA                TEE_CIPHER_IOWR(HI_ID_CIPHER, 0x10, CIPHER_RSA_DATA_S)
#define    CMD_CIPHER_TEST                   TEE_CIPHER_IOWR(HI_ID_CIPHER, 0x11, CIPHER_TEST_DATA_S)
#define    CMD_CIPHER_PBKDF2                 TEE_CIPHER_IOWR(HI_ID_CIPHER, 0x12, HI_TEE_CIPHER_PBKDF2_S)
#define    CMD_CIPHER_ENCRYPT_UNBLOCK        TEE_CIPHER_IOWR(HI_ID_CIPHER, 0x13, CIPHER_DATA_S)
#define    CMD_CIPHER_DECRYPT_UNBLOCK        TEE_CIPHER_IOWR(HI_ID_CIPHER, 0x14, CIPHER_DATA_S)
#define    CMD_CIPHER_GET_INFO               TEE_CIPHER_IOWR(HI_ID_CIPHER, 0x15, CIPHER_INFO_S)

#define    CMD_CIPHER_INIT                   TEE_CIPHER_IOWR(HI_ID_CIPHER, 0x20, NULL)
#define    CMD_CIPHER_DEINIT                 TEE_CIPHER_IOWR(HI_ID_CIPHER, 0x21, NULL)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __DRV_CIPHER_IOCTL_H__*/
