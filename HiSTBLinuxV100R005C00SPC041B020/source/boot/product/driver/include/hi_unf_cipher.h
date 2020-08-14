/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_unf_cipher.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2014-09-16
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef __HI_UNF_CIPHER_H__
#define __HI_UNF_CIPHER_H__

#include "hi_type.h"

/** Encryption/Decryption type selecting */
/** CNcomment:CIPHE加解密类型选择 */
typedef enum
{
    HI_UNF_CIPHER_TYPE_NORMAL       = 0x0,
    HI_UNF_CIPHER_TYPE_COPY_AVOID,
    HI_UNF_CIPHER_TYPE_BUTT,
}HI_UNF_CIPHER_TYPE_E;

typedef struct
{
    HI_UNF_CIPHER_TYPE_E enCipherType;
}HI_UNF_CIPHER_ATTS_S;


/** Key ladder selecting parameters */
/** CNcomment:使用哪个key ladder标志 */
typedef enum hiUNF_CIPHER_CA_TYPE_E
{
    HI_UNF_CIPHER_CA_TYPE_R2R  = 0x0,    /**< Using R2R key ladder */                                                 /**< CNcomment:使用R2R key ladder */
    HI_UNF_CIPHER_CA_TYPE_SP   ,         /**< Using SP key ladder */                                                  /**< CNcomment:使用SP key ladder */
    HI_UNF_CIPHER_CA_TYPE_BLPK ,         /**< Using BLPK key ladder */                                                /**< CNcomment:使用BLPK key ladder */
    HI_UNF_CIPHER_CA_TYPE_IRDETO_HCA,   /**< Using Irdeto high level code authentication key ladder, only used for Irdeto MSR2.2 */ /**< CNcomment:Irdeto上层应用校验keyladder，仅用于Irdeto MSR2.2 */
}HI_UNF_CIPHER_CA_TYPE_E;


/** Cipher algorithm */
/** CNcomment:CIPHER加密算法 */
typedef enum hiHI_UNF_CIPHER_ALG_E
{
    HI_UNF_CIPHER_ALG_DES           = 0x0,  /**< Data encryption standard (DES) algorithm */     /**< CNcomment: DES算法 */
    HI_UNF_CIPHER_ALG_3DES          = 0x1,  /**< 3DES algorithm */                               /**< CNcomment: 3DES算法 */
    HI_UNF_CIPHER_ALG_AES           = 0x2,  /**< Advanced encryption standard (AES) algorithm */ /**< CNcomment: AES算法 */
    HI_UNF_CIPHER_ALG_BUTT          = 0x3   
}HI_UNF_CIPHER_ALG_E;

/** Key length */
/** CNcomment: 密钥长度 */
typedef enum hiHI_UNF_CIPHER_KEY_LENGTH_E
{
    HI_UNF_CIPHER_KEY_AES_128BIT    = 0x0,  /**< 128-bit key for the AES algorithm */ /**< CNcomment:AES运算方式下采用128bit密钥长度 */
    HI_UNF_CIPHER_KEY_AES_192BIT    = 0x1,  /**< 192-bit key for the AES algorithm */ /**< CNcomment:AES运算方式下采用192bit密钥长度 */
    HI_UNF_CIPHER_KEY_AES_256BIT    = 0x2,  /**< 256-bit key for the AES algorithm */ /**< CNcomment:AES运算方式下采用256bit密钥长度 */
    HI_UNF_CIPHER_KEY_DES_3KEY      = 0x2,  /**< Three keys for the DES algorithm */  /**< CNcomment:DES运算方式下采用3个key */
    HI_UNF_CIPHER_KEY_DES_2KEY      = 0x3,  /**< Two keys for the DES algorithm */    /**< CNcomment: DES运算方式下采用2个key  */
}HI_UNF_CIPHER_KEY_LENGTH_E;

/** Cipher bit width */
/** CNcomment: 加密位宽 */
typedef enum hiHI_UNF_CIPHER_BIT_WIDTH_E
{
    HI_UNF_CIPHER_BIT_WIDTH_64BIT   = 0x0,  /**< 64-bit width */   /**< CNcomment:64bit位宽 */
    HI_UNF_CIPHER_BIT_WIDTH_8BIT    = 0x1,  /**< 8-bit width */    /**< CNcomment:8bit位宽 */
    HI_UNF_CIPHER_BIT_WIDTH_1BIT    = 0x2,  /**< 1-bit width */    /**< CNcomment:1bit位宽 */
    HI_UNF_CIPHER_BIT_WIDTH_128BIT  = 0x3,  /**< 128-bit width */  /**< CNcomment:128bit位宽 */
}HI_UNF_CIPHER_BIT_WIDTH_E;

/** Cipher control parameters */
/** CNcomment:加密控制参数变更标志 */
typedef struct hiUNF_CIPHER_CTRL_CHANGE_FLAG_S
{
    HI_U32   bit1IV:1;              /**< Initial Vector change or not */ /**< CNcomment:向量变更 */
    HI_U32   bitsResv:31;           /**< Reserved */ 					 /**< CNcomment:保留 */
}HI_UNF_CIPHER_CTRL_CHANGE_FLAG_S;

/** Cipher work mode */
/** CNcomment:CIPHER工作模式 */
typedef enum hiHI_UNF_CIPHER_WORK_MODE_E
{
    HI_UNF_CIPHER_WORK_MODE_ECB     = 0x0,   /**<Electronic codebook (ECB) mode*/    /**< CNcomment:ECB模式 */
    HI_UNF_CIPHER_WORK_MODE_CBC     = 0x1,   /**<Cipher block chaining (CBC) mode*/  /**< CNcomment:CBC模式 */
    HI_UNF_CIPHER_WORK_MODE_CFB     = 0x2,   /**<Cipher feedback (CFB) mode*/        /**< CNcomment:CFB模式 */
    HI_UNF_CIPHER_WORK_MODE_OFB     = 0x3,   /**<Output feedback (OFB) mode*/        /**< CNcomment:OFB模式 */
    HI_UNF_CIPHER_WORK_MODE_CTR     = 0x4,   /**<Counter (CTR) mode*/                /**< CNcomment:CTR模式 */
    HI_UNF_CIPHER_WORK_MODE_BUTT    = 0x5
}HI_UNF_CIPHER_WORK_MODE_E;


/** Structure of the cipher control information */
/** CNcomment:加密控制信息结构 */
typedef struct hiHI_UNF_CIPHER_CTRL_S
{
    HI_U32 u32Key[8];                               /**< Key input */                                                                                                     /**< CNcomment:输入密钥 */
    HI_U32 u32IV[4];                                /**< Initialization vector (IV) */                                                                                    /**< CNcomment:初始向量 */
    HI_BOOL bKeyByCA;                               /**< Encryption using advanced conditional access (CA) or decryption using keys */                                    /**< CNcomment:是否使用高安全CA加密或解密Key */
    HI_UNF_CIPHER_CA_TYPE_E enCaType;                   /**< Select keyladder type when using advanced CA */                                                  		          /**< CNcomment:使用高安全CA时,选择何种类型的keyladder */
    HI_UNF_CIPHER_ALG_E enAlg;                      /**< Cipher algorithm */                                                                                              /**< CNcomment:加密算法 */
    HI_UNF_CIPHER_BIT_WIDTH_E enBitWidth;           /**< Bit width for encryption or decryption */                                                                        /**< CNcomment:加密或解密的位宽 */
    HI_UNF_CIPHER_WORK_MODE_E enWorkMode;           /**< Operating mode */                                                                                                /**< CNcomment:工作模式 */
    HI_UNF_CIPHER_KEY_LENGTH_E enKeyLen;            /**< Key length */                                                                                                    /**< CNcomment:密钥长度 */
    HI_UNF_CIPHER_CTRL_CHANGE_FLAG_S stChangeFlags; /**< control information exchange choices, we default all woulde be change except they have been in the choices */ 	  /**< CNcomment:控制信息变更选项，选项中没有标识的项默认全部变更 */
} HI_UNF_CIPHER_CTRL_S;


/** Hash algrithm type */
/** CNcomment:哈希算法类型 */
typedef enum hiHI_UNF_CIPHER_HASH_TYPE_E
{
    HI_UNF_CIPHER_HASH_TYPE_SHA1,
    HI_UNF_CIPHER_HASH_TYPE_SHA256,
    HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1,
    HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256,
    HI_UNF_CIPHER_HASH_TYPE_IRDETO_CBCMAC, /**< Irdeto MSR2.2 CBC-MAC*/ /**< CNcomment: Irdeto MSR2.2 CBC-MAC*/
    HI_UNF_CIPHER_HASH_TYPE_BUTT,
}HI_UNF_CIPHER_HASH_TYPE_E;

/** Hash init struct input */
/** CNcomment:哈希算法初始化输入结构体 */
typedef struct
{
    HI_U32 u32TotalDataLen;
    HI_U8  u8HMACKey[16];		/**< only used for HMAC-SHA1/HMAC-SHA2 algrithm */	/**< CNcomment:此参数仅仅用于HMAC-SHA1/HMAC-SHA2算法 */
    HI_UNF_CIPHER_HASH_TYPE_E eShaType;
}HI_UNF_CIPHER_HASH_ATTS_S;

typedef enum hiHI_UNF_CIPHER_RSA_ENC_SCHEME_E
{ 
    HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING,            /**< without padding */             /**< CNcomment: 不填充 */
    HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0,          /**< PKCS#1 block type 0 padding*/  /**< CNcomment: PKCS#1的block type 0填充方式*/
    HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1,          /**< PKCS#1 block type 1 padding*/  /**< CNcomment: PKCS#1的block type 1填充方式*/
    HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2,          /**< PKCS#1 block type 2 padding*/  /**< CNcomment: PKCS#1的block type 2填充方式*/
    HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1,       /**< PKCS#1 RSAES-OAEP-SHA1 padding*/    /**< CNcomment: PKCS#1的RSAES-OAEP-SHA1填充方式*/
    HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256,     /**< PKCS#1 RSAES-OAEP-SHA256 padding*/  /**< CNcomment: PKCS#1的RSAES-OAEP-SHA256填充方式*/
    HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5,      /**< PKCS#1 RSAES-PKCS1_V1_5 padding*/   /**< CNcomment: PKCS#1的PKCS1_V1_5填充方式*/
    HI_UNF_CIPHER_RSA_ENC_SCHEME_BUTT,
}HI_UNF_CIPHER_RSA_ENC_SCHEME_E;

typedef enum hiHI_UNF_CIPHER_RSA_SIGN_SCHEME_E
{ 
    HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1 = 0x100, /**< PKCS#1 RSASSA_PKCS1_V15_SHA1 signature*/   /**< CNcomment: PKCS#1 RSASSA_PKCS1_V15_SHA1签名算法*/
    HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256,       /**< PKCS#1 RSASSA_PKCS1_V15_SHA256 signature*/   /**< CNcomment: PKCS#1 RSASSA_PKCS1_V15_SHA256签名算法*/
    HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1,         /**< PKCS#1 RSASSA_PKCS1_PSS_SHA1 signature*/   /**< CNcomment: PKCS#1 RSASSA_PKCS1_PSS_SHA1签名算法*/
    HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256,       /**< PKCS#1 RSASSA_PKCS1_PSS_SHA256 signature*/   /**< CNcomment: PKCS#1 RSASSA_PKCS1_PSS_SHA256签名算法*/
    HI_UNF_CIPHER_RSA_SIGN_SCHEME_BUTT,
}HI_UNF_CIPHER_RSA_SIGN_SCHEME_E;

typedef struct
{
    HI_U8  *pu8N;              /**< point to public modulus  */   /**< CNcomment: 指向RSA公钥N的指针*/
	HI_U8  *pu8E;			   /**< point to public exponent */   /**< CNcomment: 指向RSA公钥E的指针*/
    HI_U16 u16NLen;            /**< length of public modulus, max value is 512Byte*/  /**< CNcomment: RSA公钥N的长度, 最大为512Byte*/
    HI_U16 u16ELen;            /**< length of public exponent, max value is 512Byte*/  /**< CNcomment: RSA公钥E的长度, 最大为512Byte*/
}HI_UNF_CIPHER_RSA_PUB_KEY_S;

/** RSA private key struct */
/** CNcomment:RSA私钥结构体 */
typedef struct
{
    HI_U8 *pu8N;                      /*!<  public modulus    */ /**< CNcomment: 指向RSA公钥N的指针*/
    HI_U8 *pu8D;                      /*!<  private exponent  */ /**< CNcomment: 指向RSA私钥D的指针*/
    HI_U16 u16NLen;                   /**< length of public modulus, max value is 512Byte*/   /**< CNcomment: RSA公钥N的长度, 最大为512Byte*/
    HI_U16 u16DLen;                   /**< length of private exponent, max value is 512Byte*/ /**< CNcomment: RSA私钥D的长度, 最大为512Byte*/
}HI_UNF_CIPHER_RSA_PRI_KEY_S;

/** RSA public key encryption struct input */
/** CNcomment:RSA 公钥加解密算法参数结构体 */
typedef struct
{
    HI_UNF_CIPHER_RSA_ENC_SCHEME_E enScheme;   /** RSA encryption scheme*/ /** CNcomment:RSA数据加解密算法策略*/
    HI_UNF_CIPHER_RSA_PUB_KEY_S stPubKey;      /** RSA private key struct */ /** CNcomment:RSA私钥结构体 */
}HI_UNF_CIPHER_RSA_PUB_ENC_S;

/** RSA private key decryption struct input */
/** CNcomment:RSA 私钥解密算法参数结构体 */
typedef struct
{
    HI_UNF_CIPHER_RSA_ENC_SCHEME_E enScheme; /** RSA encryption scheme */ /** CNcomment:RSA数据加解密算法*/
    HI_UNF_CIPHER_RSA_PRI_KEY_S stPriKey;    /** RSA public key struct */ /** CNcomment:RSA公钥结构体 */
}HI_UNF_CIPHER_RSA_PRI_ENC_S;

/** RSA signature struct input */
/** CNcomment:RSA签名算法参数结构体 */
typedef struct
{
    HI_UNF_CIPHER_RSA_SIGN_SCHEME_E enScheme;  /** RSA signature scheme*/ /** CNcomment:RSA数据签名策略*/
    HI_UNF_CIPHER_RSA_PRI_KEY_S stPriKey;      /** RSA private key struct */ /** CNcomment:RSA私钥结构体 */
 }HI_UNF_CIPHER_RSA_SIGN_S;

/** RSA signature verify struct input */
/** CNcomment:RSA签名验证算法参数输入结构体 */
typedef struct
{
    HI_UNF_CIPHER_RSA_SIGN_SCHEME_E enScheme; /** RSA signature scheme*/ /** CNcomment:RSA数据签名策略*/
    HI_UNF_CIPHER_RSA_PUB_KEY_S stPubKey;     /** RSA public key struct */ /** CNcomment:RSA公钥结构体 */
 }HI_UNF_CIPHER_RSA_VERIFY_S;


 
/******************************* API Declaration *****************************/
/** \addtogroup      CIPHER */
/** @{ */  /** <!-- [CIPHER] */
/* ---CIPHER---*/
/** 
\brief  Init the cipher device.  CNcomment:初始化CIPHER设备。 CNend
\attention \n
This API is used to start the cipher device.
CNcomment:调用此接口初始化CIPHER设备。 CNend
\param N/A                   													CNcomment:无 CNend
\retval ::HI_SUCCESS  Call this API succussful. 								CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE  Call this API fails.                                   	CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_FAILED_INIT  The cipher device fails to be initialized. CNcomment:CIPHER设备初始化失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_Init(HI_VOID);

/** 
\brief  Deinit the cipher device.
CNcomment:\brief  去初始化CIPHER设备。 CNend
\attention \n
This API is used to stop the cipher device. If this API is called repeatedly, HI_SUCCESS is returned, but only the first operation takes effect.
CNcomment:调用此接口关闭CIPHER设备。重复关闭返回成功，第一次起作用。 CNend

\param N/A                   													CNcomment:无 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 		CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE  Call this API fails.                            			CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized.  		CNcomment:CIPHER设备未初始化 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_DeInit(HI_VOID);

/** 
\brief Obtain a cipher handle for encryption and decryption.
CNcomment：创建一路Cipher句柄。 CNend

\param[in] cipher attributes                                             		CNcomment:cipher 属性。 CNend
\param[out] phCipher Cipher handle                                       		CNcomment:CIPHER句柄。 CNend
\retval ::HI_SUCCESS Call this API succussful. 						 			CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE Call this API fails.										CNcomment: API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized.  		CNcomment:CIPHER设备未初始化 CNend
\retval ::HI_ERR_CIPHER_INVALID_POINT  The pointer is null.              		CNcomment:指针参数为空 CNend
\retval ::HI_ERR_CIPHER_FAILED_GETHANDLE  The cipher handle fails to be obtained, because there are no available cipher handles. CNcomment: 获取CIPHER句柄失败，没有空闲的CIPHER句柄 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_CreateHandle(HI_HANDLE * phCipher, const HI_UNF_CIPHER_ATTS_S * pstCipherAttr);

/** 
\brief Destroy the existing cipher handle. CNcomment:销毁已存在的CIPHER句柄。 CNend
\attention \n
This API is used to destroy existing cipher handles.
CNcomment:调用此接口销毁已经创建的CIPHER句柄。 CNend

\param[in] hCipher Cipher handle                 								CNcomment:CIPHER句柄。 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 		CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE  Call this API fails.    									CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized. 		CNcomment:CIPHER设备未初始化 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_DestroyHandle(HI_HANDLE hCipher);

/** 
\brief Configures the cipher control information.
CNcomment:\brief 配置CIPHER控制信息。 CNend
\attention \n
Before encryption or decryption, you must call this API to configure the cipher control information.
The first 64-bit data and the last 64-bit data should not be the same when using TDES algorithm.
CNcomment:进行加密解密前必须先使用此接口配置CIPHER的控制信息。使用TDES算法时，输入密钥的前后64 bit数据不能相同。 CNend

\param[in] hCipher Cipher handle.                                        		CNcomment:CIPHER句柄 CNend
\param[in] pstCtrl Cipher control information.                           		CNcomment:CIPHER控制信息 CNend
\retval ::HI_SUCCESS Call this API succussful. 						 			CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE Call this API fails.                           			CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized. 		CNcomment:CIPHER设备未初始化 CNend
\retval ::HI_ERR_CIPHER_INVALID_POINT  The pointer is null.             		CNcomment:指针参数为空 CNend
\retval ::HI_ERR_CIPHER_INVALID_PARA  The parameter is invalid.         		CNcomment:参数错误 CNend
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  The handle is invalid.          		CNcomment:句柄非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S * pstCtrl);

/** 
\brief Performs encryption. 
CNcomment:\brief 进行加密。 CNend

\attention \n
This API is used to perform encryption by using the cipher module.
The length of the encrypted data should be a multiple of 8 in TDES mode and 16 in AES mode. Besides, the length can not be bigger than 0xFFFFF.After this operation, the result will affect next operation.If you want to remove vector, you need to config IV(config pstCtrl->stChangeFlags.bit1IV with 1) by transfering HI_UNF_CIPHER_ConfigHandle.
CNcomment:使用CIPHER进行加密操作。TDES模式下加密的数据长度应当是8的倍数，AES下应当是16的倍数。此外，加密数据长度不能长于0xFFFFF。本次操作完成后，此次操作的向量运算结果会作用于下一次操作，如果要清除向量，需要在下次加密操作之前调用HI_UNF_CIPHER_ConfigHandle重新配置IV(需要设置pstCtrl->stChangeFlags.bit1IV为1)。 CNend
\param[in] hCipher Cipher handle  												CNcomment:CIPHER句柄 CNend
\param[in] u32SrcPhyAddr Physical address of the source data            		CNcomment:源数据物理地址 CNend
\param[in] u32DestPhyAddr Physical address of the target data           		CNcomment:目的数据物理地址 CNend
\param[in] u32ByteLength   Length of the encrypted data                 		CNcomment:加密数据长度 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 		CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE  Call this API fails.                           			CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized. 		CNcomment:CIPHER设备未初始化 CNend
\retval ::HI_ERR_CIPHER_INVALID_PARA  The parameter is invalid.         		CNcomment:参数错误 CNend
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  The handle is invalid.          		CNcomment:句柄非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength);

/** 
\brief 进行加密
\attention \n
This API is used to perform decryption by using the cipher module.
The length of the decrypted data should be a multiple of 8 in TDES mode and 16 in AES mode. Besides, the length can not be bigger than 0xFFFFF.After this operation, the result will affect next operation.If you want to remove vector, you need to config IV(config pstCtrl->stChangeFlags.bit1IV with 1) by transfering HI_UNF_CIPHER_ConfigHandle.
CNcomment:使用CIPHER进行解密操作。TDES模式下解密的数据长度应当是8的倍数，AES下应当是16的倍数。此外，解密数据长度不能长于0xFFFFF。本次操作完成后，此次操作的向量运算结果会作用于下一次操作，如果要清除向量，需要在下次解密操作之前调用HI_UNF_CIPHER_ConfigHandle重新配置IV(需要设置pstCtrl->stChangeFlags.bit1IV为1)。 CNend
\param[in] hCipher Cipher handle.                                        		CNcomment:CIPHER句柄 CNend
\param[in] u32SrcPhyAddr Physical address of the source data.            		CNcomment:源数据物理地址 CNend
\param[in] u32DestPhyAddr Physical address of the target data.           		CNcomment:目的数据物理地址 CNend
\param[in] u32ByteLength Length of the decrypted data                   		CNcomment:解密数据长度 CNend
\retval ::HI_SUCCESS Call this API succussful. 						 			CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE Call this API fails.                           			CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized. 		CNcomment:CIPHER设备未初始化 CNend
\retval ::HI_ERR_CIPHER_INVALID_PARA  The parameter is invalid.         		CNcomment:参数错误 CNend
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  The handle is invalid.          		CNcomment:句柄非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength);

/** 
\brief Init the hash module, if other program is using the hash module, the API will return failure.
CNcomment:\brief 初始化HASH模块，如果有其他程序正在使用HASH模块，返回失败状态。 CNend

\attention \n
N/A

\param[in] pstHashAttr: The hash calculating structure input.                                      CNcomment:用于计算hash的结构体参数 CNend
\param[out] pHashHandle: The output hash handle.               									CNcomment:输出的hash句柄 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 						CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                       								CNcomment: API系统调用失败 CNend

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_HashInit(HI_UNF_CIPHER_HASH_ATTS_S * pstHashAttr, HI_HANDLE * pHashHandle);

/** 
\brief Calculate the hash, if the size of the data to be calculated is very big and the DDR ram is not enough, this API can calculate the data one block by one block.
CNcomment:\brief 计算hash值，如果需要计算的数据量比较大，该接口可以实现一个block一个block的计算，避免数据量比较大的情况下，内存不足的问题。 CNend

\attention \n
N/A

\param[in] hHashHandl:  Hash handle.                                  		CNcomment:Hash句柄。 CNend
\param[in] pu8InputData:  The input data buffer.                  			CNcomment:输入数据缓冲 CNend
\param[in] u32InputDataLen:  The input data length, attention: the block length input must be 4bytes aligned except the last block!			CNcomment:输入数据的长度。重要： 输入数据块的长度必须是4字节对齐，最后一个block无此限制。 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 	CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                       		CNcomment:API系统调用失败。 CNend

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_HashUpdate(HI_HANDLE hHashHandle, HI_U8 * pu8InputData, HI_U32 u32InputDataLen);

/** 
\brief Get the final hash value, after calculate all of the data, call this API to get the final hash value and close the handle.If there is some reason need to interupt the calculation, this API should also be call to close the handle.
CNcomment:获取hash值，在计算完所有的数据后，调用这个接口获取最终的hash值，该接口同时会关闭hash句柄。如果在计算过程中，需要中断计算，也必须调用该接口关闭hash句柄。 CNend

\attention \n
N/A

\param[in] hHashHandle:  Hash handle.                                  		CNcomment:Hash句柄。 CNend
\param[out] pu8OutputHash:  The final output hash value.                   	CNcomment:输出的hash值。 CNend

\retval ::HI_SUCCESS  Call this API succussful. 						 	CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                       		CNcomment:API系统调用失败。 CNend

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_HashFinal(HI_HANDLE hHashHandle, HI_U8 * pu8OutputHash);

/* The following SHA interfaces are used for the command test */
HI_S32 HI_CIPHER_SHA(HI_UNF_CIPHER_HASH_TYPE_E enType,
						const unsigned char *input,
						int iBufLen,
						unsigned char *output,
						int oBufLen);

/* encrypt data buffer by a given cipher handle, in a loop inside */
HI_S32 HI_CIPHER_EncryptEx(HI_HANDLE hCipherHandle, HI_U8 *input, HI_U32 datalen, HI_U8 *output);

/**
\brief Get the random number.
CNcomment:\brief 获取随机数。 CNend

\attention \n
This API is used to obtain the random number from the hardware.
CNcomment:调用此接口用于获取随机数。 CNend

\param[out] pu32RandomNumber Point to the random number.                                        CNcomment:随机数数值。 CNend
\retval ::HI_SUCCESS  Call this API succussful.                                                 CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                                                      CNcomment: API系统调用失败。 CNend

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_GetRandomNumber(HI_U32 *pu32RandomNumber);

/**
\brief RSA encryption a plaintext with a RSA public key.
CNcomment:使用RSA公钥加密一段明文。 CNend

\attention \n
N/A

\param[in] pstRsaEnc:   encryption struct.                                   CNcomment:加密属性结构体。 CNend
\param[in] pu8Input：   input data to be encryption                          CNcomment: 待加密的数据。 CNend
\param[out] u32InLen:   length of input data to be encryption                CNcomment: 待加密的数据长度。 CNend
\param[out] pu8Output： output data to be encryption                         CNcomment: 加密结果数据。 CNend
\param[out] pu32OutLen: length of output data to be encryption               CNcomment: 加密结果的数据长度。 CNend

\retval ::HI_SUCCESS  Call this API succussful.                         CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                              CNcomment:API系统调用失败。 CNend

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_RsaPublicEncrypt(HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaEnc, 
								  HI_U8 *pu8Input, HI_U32 u32InLen, 
								  HI_U8 *pu8Output, HI_U32 *pu32OutLen);

/**
\brief RSA decryption a ciphertext with a RSA private key.
CNcomment:使用RSA私钥解密一段密文。 CNend

\attention \n
N/A

\param[in] pstRsaDec:   decryption struct.                                   CNcomment: 公钥解密属性结构体。 CNend
\param[in] pu8Input：   input data to be decryption                          CNcomment: 待解密的数据。 CNend
\param[out] u32InLen:   length of input data to be decryption                CNcomment: 待解密的数据长度。 CNend
\param[out] pu8Output： output data to be decryption                         CNcomment: 解密结果数据。 CNend
\param[out] pu32OutLen: length of output data to be decryption               CNcomment: 解密结果的数据长度。 CNend

\retval ::HI_SUCCESS  Call this API succussful.                         CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                              CNcomment:API系统调用失败。 CNend

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_RsaPrivateDecrypt(HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaDec,								  
								   HI_U8 *pu8Input, HI_U32 u32InLen, 
								   HI_U8 *pu8Output, HI_U32 *pu32OutLen);

/**
\brief RSA encryption a plaintext with a RSA private key.
CNcomment:使用RSA私钥加密一段明文。 CNend

\attention \n
N/A

\param[in] pstRsaSign:   encryption struct.                                  CNcomment:加密属性结构体。 CNend
\param[in] pu8Input：   input data to be encryption                          CNcomment: 待加密的数据。 CNend
\param[out] u32InLen:   length of input data to be encryption                CNcomment: 待加密的数据长度。 CNend
\param[out] pu8Output： output data to be encryption                         CNcomment: 加密结果数据。 CNend
\param[out] pu32OutLen: length of output data to be encryption               CNcomment: 加密结果的数据长度。 CNend

\retval ::HI_SUCCESS  Call this API succussful.                         CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                              CNcomment:API系统调用失败。 CNend

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_RsaPrivateEncrypt(HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaEnc, 
							 	   HI_U8 *pu8Input, HI_U32 u32InLen, 
								   HI_U8 *pu8Output, HI_U32 *pu32OutLen);

/**
\brief RSA decryption a ciphertext with a RSA public key.
CNcomment:使用RSA公钥解密一段密文。 CNend

\attention \n
N/A

\param[in] pstRsaVerify:   decryption struct.                                CNcomment: 解密属性结构体。 CNend
\param[in] pu8Input：   input data to be decryption                          CNcomment: 待解密的数据。 CNend
\param[out] u32InLen:   length of input data to be decryption                CNcomment: 待解密的数据长度。 CNend
\param[out] pu8Output： output data to be decryption                         CNcomment: 解密结果数据。 CNend
\param[out] pu32OutLen: length of output data to be decryption               CNcomment: 解密结果的数据长度。 CNend

\retval ::HI_SUCCESS  Call this API succussful.                         CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                              CNcomment:API系统调用失败。 CNend

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_RsaPublicDecrypt(HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaDec,
							   HI_U8 *pu8Input, HI_U32 u32InLen,
							   HI_U8 *pu8Output, HI_U32 *pu32OutLen);

/**
\brief RSA signature a context with appendix, where a signer’s RSA private key is used.
CNcomment:使用RSA私钥签名一段文本。 CNend

\attention \n
N/A

\param[in] pstRsaSign:      signature struct.                                    CNcomment: 签名属性结构体。 CNend
\param[in] pu8Input：       input context to be signature，maybe null            CNcomment: 待签名的数据, 如果pu8HashData不为空，则该指空可以为空。 CNend
\param[in] u32InLen:        length of input context to be signature              CNcomment: 待签名的数据长度。 CNend
\param[in] pu8HashData：    hash value of context,if NULL, let pu8HashData = Hash(context) automatically   Ncomment: 待签名文本的HASH摘要，如果为空，则自动计算文本的HASH摘要。 CNend                     
\param[out] pu8OutSign：    output message of signature                          CNcomment: 签名信息。 CNend
\param[out] pu32OutSignLen: length of message of signature                       CNcomment: 签名信息的数据长度。 CNend

\retval ::HI_SUCCESS  Call this API succussful.                         CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                              CNcomment:API系统调用失败。 CNend

\see \n
N/A
*/								   
HI_S32 HI_UNF_CIPHER_RsaSign(HI_UNF_CIPHER_RSA_SIGN_S *pstRsaSign, 
							 HI_U8 *pu8InData, HI_U32 u32InDataLen,
							 HI_U8 *pu8HashData,
							 HI_U8 *pu8OutSign, HI_U32 *pu32OutSignLen);

/**
\brief RSA signature verification a context with appendix, where a signer’s RSA public key is used.
CNcomment:使用RSA公钥签名验证一段文本。 CNend

\attention \n
N/A

\param[in] pstRsaVerify:    signature verification struct.                         CNcomment: 签名验证属性结构体。 CNend
\param[in] pu8Input：       input context to be signature verification，maybe null CNcomment: 待签名验证的数据, 如果pu8HashData不为空，则该指空可以为空。 CNend
\param[in] u32InLen:        length of input context to be signature                CNcomment: 待签名验证的数据长度。 CNend
\param[in] pu8HashData：    hash value of context,if NULL, let pu8HashData = Hash(context) automatically   Ncomment: 待签名文本的HASH摘要，如果为空，则自动计算文本的HASH摘要。 CNend                     
\param[in] pu8InSign：      message of signature                                 CNcomment: 签名信息。 CNend
\param[in] pu32InSignLen:   length of message of signature                       CNcomment: 签名信息的数据长度。 CNend

\retval ::HI_SUCCESS  Call this API succussful.                         CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                              CNcomment:API系统调用失败。 CNend

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_RsaVerify(HI_UNF_CIPHER_RSA_VERIFY_S *pstRsaVerify,
							   HI_U8 *pu8InData, HI_U32 u32InDataLen,
							   HI_U8 *pu8HashData,
							   HI_U8 *pu8InSign, HI_U32 u32InSignLen);

#endif
