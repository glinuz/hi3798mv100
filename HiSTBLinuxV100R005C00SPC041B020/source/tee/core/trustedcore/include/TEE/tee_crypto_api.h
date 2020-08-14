/**
 * @file tee_crypto_api.h
 *
 * Copyright(C), 2013-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：TEE(Trusted Execution Environment，可信执行环境)加解密API \n
 * Message Digest Functions \n
 * Symmetric Cipher Functions \n
 * MAC Functions \n
 * Authenticated Encryption Functions \n
 * Asymmetric Functions \n
 * Key Derivation Functions \n
 * Random Data Generation Function \n
*/
#ifndef TEE_CRYPTO_API_H
#define TEE_CRYPTO_API_H

#include "tee_internal_api.h"
#include "tee_mem_mgmt_api.h"

#ifndef NULL
#define NULL            ((void *)0)
#endif

/** @defgroup crypto 加密和解密
* @ingroup TEE_API
*/

/**
 * @ingroup  crypto
 * 以比特为单位的最大密钥长度
 *
*/
#define TEE_MAX_KEY_SIZE_IN_BITS 2048

/**
 * @ingroup  crypto
 * RSA(公钥加密算法) 密钥长度
 *
*/
#define SW_RSA_KEYLEN 1024
#if 1
/**
 * @ingroup  crypto
 * 密钥派生时补充结构体最大字节
 *
*/
#define TEE_DH_MAX_SIZE_OF_OTHER_INFO  64 /*bytes*/
#endif
/**
 * @ingroup  crypto
 *
 * TEE的加解密操作枚举
*/
enum __TEE_Operation_Constants {
	TEE_OPERATION_CIPHER = 0x1,         /**< 加解密 */
	TEE_OPERATION_MAC,                  /**< MAC */
	TEE_OPERATION_AE,                   /**< 认证加密 */
	TEE_OPERATION_DIGEST,               /**< 摘要 */
	TEE_OPERATION_ASYMMETRIC_CIPHER,    /**< 非对称加解密 */
	TEE_OPERATION_ASYMMETRIC_SIGNATURE, /**< 非对称签名 */
	TEE_OPERATION_ASYMMETRIC_VERIFY,    /**< 非对称认证 */
	TEE_OPERATION_KEY_DERIVATION        /**< 密钥派生 */
};

/**
 * @ingroup  crypto
 *
 * TEE的加解密算法ID,包括对称加解密、非对称加解密、摘要HMAC等\n
 * 注意:对称算法中的nopad模式需要TA(Trusted Application，可信应用)来做填充
*/
enum __TEE_CRYPTO_ALGORITHM_ID {
	TEE_ALG_INVALID = 0x0,                              /**< 无效ID */
	TEE_ALG_AES_ECB_NOPAD =  0x10000010,                /**< AES_ECB_NOPAD */
	TEE_ALG_AES_CBC_NOPAD = 0x10000110,                 /**< AES_CBC_NOPAD */
	TEE_ALG_AES_CTR = 0x10000210,                       /**< AES_CTR */
	TEE_ALG_AES_CTS = 0x10000310,                       /**< AES_CTS */
	TEE_ALG_AES_XTS = 0x10000410,                       /**< AES_XTS */
	TEE_ALG_AES_CBC_MAC_NOPAD = 0x30000110,             /**< AES_CBC_MAC_NOPAD */
	TEE_ALG_AES_CBC_MAC_PKCS5 = 0x30000510,             /**< AES_CBC_MAC_PKCS5 */
	TEE_ALG_AES_CMAC = 0x30000610,                      /**< AES_CMAC */
	TEE_ALG_AES_CCM = 0x40000710,                       /**< AES_CCM */
	TEE_ALG_AES_GCM = 0x40000810,                       /**< AES_GCM */
	TEE_ALG_DES_ECB_NOPAD = 0x10000011,                 /**< DES_ECB_NOPAD */
	TEE_ALG_DES_CBC_NOPAD = 0x10000111,                 /**< DES_CBC_NOPAD */
	TEE_ALG_DES_CBC_MAC_NOPAD = 0x30000111,             /**< DES_CBC_MAC_NOPAD */
	TEE_ALG_DES_CBC_MAC_PKCS5 = 0x30000511,             /**< DES_CBC_MAC_PKCS5 */
	TEE_ALG_DES3_ECB_NOPAD = 0x10000013,                /**< DES3_ECB_NOPAD */
	TEE_ALG_DES3_CBC_NOPAD = 0x10000113,                /**< DES3_CBC_NOPAD */
	TEE_ALG_DES3_CBC_MAC_NOPAD = 0x30000113,            /**< DES3_CBC_MAC_NOPAD */
	TEE_ALG_DES3_CBC_MAC_PKCS5 = 0x30000513,            /**< DES3_CBC_MAC_PKCS5 */
	TEE_ALG_RSASSA_PKCS1_V1_5_MD5 = 0x70001830,         /**< RSASSA_PKCS1_V1_5_MD5 */
	TEE_ALG_RSASSA_PKCS1_V1_5_SHA1 = 0x70002830,        /**< RSASSA_PKCS1_V1_5_SHA1 */
	TEE_ALG_RSASSA_PKCS1_V1_5_SHA224 = 0x70003830,      /**< RSASSA_PKCS1_V1_5_SHA224 */
	TEE_ALG_RSASSA_PKCS1_V1_5_SHA256 = 0x70004830,      /**< RSASSA_PKCS1_V1_5_SHA256 */
	TEE_ALG_RSASSA_PKCS1_V1_5_SHA384 = 0x70005830,      /**< RSASSA_PKCS1_V1_5_SHA384 */
	TEE_ALG_RSASSA_PKCS1_V1_5_SHA512 = 0x70006830,      /**< RSASSA_PKCS1_V1_5_SHA512 */
	TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA1 = 0x70212930,    /**< RSASSA_PKCS1_PSS_MGF1_SHA1 */
	TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA224 = 0x70313930,  /**< RSASSA_PKCS1_PSS_MGF1_SHA224 */
	TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA256 = 0x70414930,  /**< RSASSA_PKCS1_PSS_MGF1_SHA256 */
	TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA384 = 0x70515930,  /**< RSASSA_PKCS1_PSS_MGF1_SHA384 */
	TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA512 = 0x70616930,  /**< RSASSA_PKCS1_PSS_MGF1_SHA512 */
	TEE_ALG_RSAES_PKCS1_V1_5 = 0x60000130,              /**< RSAES_PKCS1_V1_5 */
	TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA1 = 0x60210230,    /**< RSAES_PKCS1_OAEP_MGF1_SHA1 */
	TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA224 = 0x60211230,  /**< RSAES_PKCS1_OAEP_MGF1_SHA224 */
	TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA256 = 0x60212230,  /**< RSAES_PKCS1_OAEP_MGF1_SHA256 */
	TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA384 = 0x60213230,  /**< RSAES_PKCS1_OAEP_MGF1_SHA384 */
	TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512 = 0x60214230,  /**< RSAES_PKCS1_OAEP_MGF1_SHA512 */
	TEE_ALG_RSA_NOPAD = 0x60000030,                     /**< RSA_NOPAD */
	TEE_ALG_DSA_SHA1 = 0x70002131,                      /**< DSA_SHA1 */
	TEE_ALG_DH_DERIVE_SHARED_SECRET = 0x80000032,       /**< DH_DERIVE_SHARED_SECRET */
	TEE_ALG_MD5 = 0x50000001,                           /**< MD5 */
	TEE_ALG_SHA1 = 0x50000002,                          /**< SHA1 */
	TEE_ALG_SHA224 = 0x50000003,                        /**< SHA224 */
	TEE_ALG_SHA256 = 0x50000004,                        /**< SHA256 */
	TEE_ALG_SHA384 = 0x50000005,                        /**< SHA384 */
	TEE_ALG_SHA512 = 0x50000006,                        /**< SHA512 */
	TEE_ALG_HMAC_MD5 = 0x30000001,                      /**< HMAC_MD5 */
	TEE_ALG_HMAC_SHA1 = 0x30000002,                     /**< HMAC_SHA1 */
	TEE_ALG_HMAC_SHA224 = 0x30000003,                   /**< HMAC_SHA224 */
	TEE_ALG_HMAC_SHA256 = 0x30000004,                   /**< HMAC_SHA256 */
	TEE_ALG_HMAC_SHA384 = 0x30000005,                   /**< HMAC_SHA384 */
	TEE_ALG_HMAC_SHA512 = 0x30000006,                   /**< HMAC_SHA512 */
	TEE_ALG_AES_ECB_PKCS5 = 0x10000020,                 /**< AES_ECB_PKCS5 */
	TEE_ALG_AES_CBC_PKCS5 = 0x10000220,                 /**< AES_CBC_PKCS5 */
	TEE_ALG_ECC = 0x80000001,
} ;
/**
 * @ingroup  crypto
 *
 * TEE的加解密算法ID声明
*/
typedef enum __TEE_CRYPTO_ALGORITHM_ID TEE_CRYPTO_ALGORITHM_ID;
#if 1
/**
 * @ingroup  crypto
 *
 * TEE的密钥派生HASH模式
*/
typedef enum {
	TEE_DH_HASH_SHA1_mode = 0,              /**< HASH_SHA1 */
	TEE_DH_HASH_SHA224_mode = 1,            /**< HASH_SHA224 */
	TEE_DH_HASH_SHA256_mode = 2,            /**< HASH_SHA256 */
	TEE_DH_HASH_SHA384_mode = 3,            /**< HASH_SHA384 */
	TEE_DH_HASH_SHA512_mode = 4,            /**< HASH_SHA512 */
	TEE_DH_HASH_NumOfModes,                 /**< num of modes */
} TEE_DH_HASH_Mode;

/**
 * @ingroup  crypto
 *
 * TEE的密钥派生DH协议
*/
typedef enum {
	TEE_DH_PKCS3_mode  = 0,              /**< PKCS3 */
	TEE_DH_ANSI_X942_mode = 1,           /**< X942 */
	TEE_DH_NumOfModes,                   /**< num of modes */
} TEE_DH_OpMode_t;

/**
 * @ingroup  crypto
 *
 * TEE的密钥派生函数模式
*/
typedef enum {
	TEE_DH_ASN1_DerivMode = 0,                          /**< ASN1_DerivMode */
	TEE_DH_ConcatDerivMode = 1,                         /**< ConcatDerivMode */
	TEE_DH_X963_DerivMode = TEE_DH_ConcatDerivMode,     /**< X963_DerivMode */
	TEE_DH_OMADRM_DerivMode = 2,                        /**< OMADRM_DerivMode */
	TEE_DH_ISO18033_KDF1_DerivMode = 3,                 /**< ISO18033_KDF1_DerivMode */
	TEE_DH_ISO18033_KDF2_DerivMode = 4,                 /**< ISO18033_KDF2_DerivMode */
	TEE_DH_DerivFunc_NumOfModes,                        /**< num of modes */
} TEE_DH_DerivFuncMode;
#endif

/**
 * @ingroup  crypto
 *
 * TEE的密钥派生类型
*/
enum __TEE_DK_ObjectAttribute {
	TEE_DK_SECRECT = 0,         /**< A pointer to shared secret value */
	TEE_DK_OTHER,               /**< A pointer to structure containing other data */
	TEE_DK_HASH_MODE,           /**< The enumerator ID of the HASH function to be used */
	TEE_DK_DERIVATION_MODE      /**< The enumerator ID of the derivation function mode */
};
/**
 * @ingroup  crypto
 *
 * TEE的密钥派生类型声明
*/
typedef enum __TEE_DK_ObjectAttribute TEE_DK_ObjectAttribute;
/**
 * @ingroup  crypto
 *
 * TEE的加解密操作模式
*/
enum __TEE_OperationMode {
	TEE_MODE_ENCRYPT = 0x0, /**< 加密 */
	TEE_MODE_DECRYPT,       /**< 解密 */
	TEE_MODE_SIGN,          /**< 签名 */
	TEE_MODE_VERIFY,        /**< 验证 */
	TEE_MODE_MAC,           /**< MAC */
	TEE_MODE_DIGEST,        /**< 摘要 */
	TEE_MODE_DERIVE        /**< 派生 */
} ;

/**
 * @ingroup  crypto
 *
 * TEE的加解密操作模式声明
*/
typedef enum __TEE_OperationMode TEE_OperationMode;
#if 1
/**
 * @ingroup  crypto
 *
 * TEE密钥派生的other数据结构, 包含了密钥派生的一些可选数据，不需要则置NULL
 */
typedef struct {
	uint8_t    AlgorithmID[TEE_DH_MAX_SIZE_OF_OTHER_INFO];  /**< object唯一标示(OID) */
	uint32_t   SizeOfAlgorithmID;                           /**< AlgorithmID的长度 */
	uint8_t    PartyUInfo[TEE_DH_MAX_SIZE_OF_OTHER_INFO];   /**< 发送方的public信息 */
	uint32_t   SizeOfPartyUInfo;                            /**< PartyUInfo的长度 */
	uint8_t    PartyVInfo[TEE_DH_MAX_SIZE_OF_OTHER_INFO];   /**< 接收方的public信息 */
	uint32_t   SizeOfPartyVInfo;                            /**< PartyVInfo长度 */
	uint8_t    SuppPrivInfo[TEE_DH_MAX_SIZE_OF_OTHER_INFO]; /**< 双方共有的private信息 */
	uint32_t   SizeOfSuppPrivInfo;                          /**< SuppPrivInfo的长度 */
	uint8_t    SuppPubInfo[TEE_DH_MAX_SIZE_OF_OTHER_INFO];  /**< 双方共有的public信息 */
	uint32_t   SizeOfSuppPubInfo;                           /**< SuppPubInfo长度 */
} TEE_DH_OtherInfo;
#endif
/**
 * @ingroup  crypto
 *
 * TEE的加解密操作信息，用于#TEE_GetOperationInfo()
*/
struct __TEE_OperationInfo {
	uint32_t algorithm;         /**< #__TEE_CRYPTO_ALGORITHM_ID 算法 */
	uint32_t operationClass;    /**< #__TEE_Operation_Constants 加解密操作 */
	uint32_t mode;              /**< #__TEE_OperationMode 加解密操作模式 */
	uint32_t digestLength;      /**< 摘要长度 */
	uint32_t maxKeySize;        /**< 密钥最大长度 */
	uint32_t keySize;           /**< 密钥长度 */
	uint32_t requiredKeyUsage;  /**< 是否需要密钥 */
	uint32_t handleState;       /**< 操作状态 */
	void *keyValue;             /**< 密钥指针 */
} ;
/**
 * @ingroup  crypto
 *
 * TEE的加解密操作信息声明
*/
typedef struct __TEE_OperationInfo TEE_OperationInfo;

/**
 * @ingroup  crypto
 *
 * TEE的加解密全局句柄
*/
struct __TEE_OperationHandle {
	uint32_t algorithm;         /**< #__TEE_CRYPTO_ALGORITHM_ID 算法 */
	uint32_t operationClass;    /**< #__TEE_Operation_Constants 加解密操作 */
	uint32_t mode;              /**< #__TEE_OperationMode 加解密操作模式 */
	uint32_t digestLength;      /**< 摘要长度 */
	uint32_t maxKeySize;        /**< 密钥最大长度 */
	uint32_t keySize;           /**< 密钥长度 */
	uint32_t keySize2;          /**< 密钥2长度 */
	uint32_t requiredKeyUsage;  /**< 是否需要密钥 */
	uint32_t handleState;       /**< 操作状态 */
	void *keyValue;             /**< 密钥buffer */
	void *keyValue2;            /**< 密钥2buffer */
	void *crypto_ctxt;          /**< DX加解密结构体句柄 */
	void *hmac_rest_ctext;      /**< DX hmac 复位句柄 */
	void *IV;                   /**< 初始化向量指针 */
	void *publicKey;            /**< 公钥指针，非对称加解密使用 */
	uint32_t publicKeyLen;        /**< 公钥总长度*/
	void *privateKey;           /**< 私钥指针，非对称加解密使用 */
	uint32_t privateKeyLen;        /**< 私钥总长度*/
	uint32_t IVLen;             /**< 初始化向量长度 */
	//start of DH
	TEE_DH_OtherInfo *dh_otherinfo;         /**< #TEE_DH_OtherInfo DH other数据结构 */
	uint32_t dh_hash_mode;          /**< #TEE_DH_HASH_Mode DH hash模式 */
	uint32_t dh_derive_func;    /**< #TEE_DH_DerivFuncMode DH派生函数 */
	uint32_t dh_op_mode;             /**< #TEE_DH_OpMode_t DH协议 */
	//end of DH
};

/**
 * @ingroup  crypto
 *
 * 非对称加解密公钥结构体的最大值
 */
#define RSA_PUBKEY_MAXSIZE sizeof(CRYS_RSAUserPubKey_t)

/**
 * @ingroup  crypto
 *
 * 非对称加解密私钥结构体的最大值
 */
#define RSA_PRIVKEY_MAXSIZE sizeof(CRYS_RSAUserPrivKey_t)

/**
 * @ingroup  crypto
 *
 * TEE的加解密全局句柄指针声明
*/
typedef struct __TEE_OperationHandle *TEE_OperationHandle;
/**
 * @ingroup  crypto
 *
 * TEE的加解密全局句柄声明
*/
typedef struct __TEE_OperationHandle TEE_OperationHandleVar;
/**
 * @ingroup  crypto
 *
 * TEE的object句柄声明
*/
typedef struct __TEE_ObjectHandle TEE_ObjectHandleVar;

/**
 * @ingroup  crypto
 * @brief TEE_AllocateOperation 分配加解密资源
 *
 * @par 描述:
 * malloc #TEE_OperationHandle，为需要运行的算法分配资源，初始化部分变量
 *
 * @attention 在为摘要分配资源的时候做了初始化
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param algorithm [IN]  加解密算法 #TEE_CRYPTO_ALGORITHM_ID
 * @param mode [IN]  加解密模式 #TEE_OperationMode
 * @param maxKeySize [IN]  最大密钥长度 64bytes
 *
 * @retval #TEE_SUCCESS 安全侧分配加解密资源成功
 * @retval #TEE_ERROR_OUT_OF_MEMORY #TEE_OperationHandle malloc失败
 * @retval #TEE_ERROR_NOT_SUPPORTED 算法格式不支持
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_FreeOperation
 * @since V100R002C00B302
*/
TEE_Result TEE_AllocateOperation(TEE_OperationHandle *operation,
				 uint32_t algorithm, uint32_t mode, uint32_t maxKeySize);
/**
 * @ingroup  crypto
 * @brief TEE_FreeOperation 释放加解密资源
 *
 * @par 描述:
 * free TEE_OperationHandle，释放加解密相关的资源
 *
 * @attention 无
 * @param operation [IN/OUT]  模块句柄
 *
 * @retval 无
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_AllocateOperation
 * @since V100R002C00B302
*/
void TEE_FreeOperation(TEE_OperationHandle operation);
/**
 * @ingroup  crypto
 * @brief TEE_GetOperationInfo 获取加解密信息
 *
 * @par 描述:
 * 获取模块句柄的加解密信息，保存到operationInfo中
 *
 * @attention 无
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param operationInfo [IN/OUT]  加解密信息 #TEE_OperationInfo
 *
 * @retval 无
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see 无
 * @since V100R002C00B302
*/
void TEE_GetOperationInfo(TEE_OperationHandle operation,
			  TEE_OperationInfo *operationInfo);
/**
 * ingroup  crypto
 * @brief 重置加解密模块句柄
 *
 * @par 描述:
 * 重置加解密模块句柄
 *
 * @attention 无
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 *
 * @retval 无
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see 无
 * @since V100R002C00B302
*/
void TEE_ResetOperation(TEE_OperationHandle operation);
/**
 * @ingroup  crypto
 * @brief 设置加解密模块密钥
 *
 * @par 描述:
 * 设置加解密模块密钥
 *
 * @attention 无
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param key [IN/OUT]  密钥句柄 #TEE_ObjectHandle
 *
 * @retval #TEE_SUCCESS 设置密钥成功
 * @retval #TEE_ERROR_BAD_PARAMETERS  参数为NULL，或者参数错误
 * @retval #TEE_ERROR_OUT_OF_MEMORY  密钥buffer分配失败
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_SetOperationKey2
 * @since V100R002C00B302
*/
TEE_Result TEE_SetOperationKey(TEE_OperationHandle operation, TEE_ObjectHandle key);
/**
 * ingroup  crypto
 * @brief 设置加解密模块密钥1和密钥2
 *
 * @par 描述:
 * 设置加解密模块密钥1和密钥2
 *
 * @attention 无
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param key1 [IN/OUT]  密钥1句柄 #TEE_ObjectHandle
 * @param key2 [IN/OUT]  密钥2句柄 #TEE_ObjectHandle
 *
 * @retval #TEE_SUCCESS 设置2个密钥成功
 * @retval #TEE_ERROR_BAD_PARAMETERS  参数为NULL，或者参数错误
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_SetOperationKey
 * @since V100R002C00B302
*/
TEE_Result TEE_SetOperationKey2(TEE_OperationHandle operation, TEE_ObjectHandle key1, TEE_ObjectHandle key2);
/**
 * @ingroup  crypto
 * @brief 拷贝加解密模块句柄到另一个句柄
 *
 * @par 描述:
 * 拷贝加解密模块句柄到另一个句柄
 *
 * @attention 无
 * @param dstOperation [IN/OUT]  目标模块句柄 #TEE_OperationHandle
 * @param srcOperation [IN/OUT]  源模块句柄 #TEE_OperationHandle
 *
 * @retval 无
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see 无
 * @since V100R002C00B302
*/
void TEE_CopyOperation(TEE_OperationHandle dstOperation, TEE_OperationHandle srcOperation);
/**
 * @ingroup  crypto
 * @brief 加解密初始化
 *
 * @par 描述:
 * 加解密初始化，包括AES和DES初始化
 *
 * @attention 无
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param IV [IN]  初始化向量指针，如果不需要初始化向量的话设为NULL
 * @param IVLen [IN]  初始化向量长度，
 *
 * @retval 无
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_CipherUpdate | TEE_CipherDoFinal
 * @since V100R002C00B302
*/
void TEE_CipherInit(TEE_OperationHandle operation, void *IV, size_t IVLen);
/**
 * @ingroup  crypto
 * @brief 加解密运算
 *
 * @par 描述:
 * 加解密运算，包括AES和DES运算
 *
 * @attention 无
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param srcData [IN]  输入加密/解密数据指针
 * @param srcLen [IN]  输入加密/解密数据长度 AES数据长度为16字节倍数，DES数据长度为8字节倍数
 * @param destData [OUT] 输出解密/加密数据指针
 * @param destLen [OUT] 输出解密/加密数据长度
 *
 * @retval #TEE_SUCCESS 加解密运算成功
 * @retval #TEE_ERROR_BAD_PARAMETERS 参数为NULL，或传入错误
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_CipherInit | TEE_CipherDoFinal
 * @since V100R002C00B302
*/
TEE_Result TEE_CipherUpdate(TEE_OperationHandle operation,
			    void *srcData, size_t srcLen, void *destData, size_t *destLen);
/**
 * @ingroup  crypto
 * @brief 加解密运算结束
 *
 * @par 描述:
 * 加解密运算结束，只有AES需要执行此接口，DES不需要
 *
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param srcData [IN]  输入加密/解密数据指针
 * @param srcLen [IN]  输入加密/解密数据长度 AES数据长度为16字节倍数，DES数据长度为8字节倍数
 * @param destData [OUT] 输出解密/加密数据指针
 * @param destLen [OUT] 输出解密/加密数据长度
 *
 * @retval #TEE_SUCCESS 加解密结束成功
 * @retval #TEE_ERROR_BAD_PARAMETERS 参数为NULL，或传入错误
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_CipherInit | TEE_CipherUpdate
 * @since V100R002C00B302
*/
TEE_Result TEE_CipherDoFinal(TEE_OperationHandle operation,
			     void *srcData, size_t srcLen, void *destData, size_t *destLen);
/**
 * @ingroup  crypto
 * @brief 摘要运算
 *
 * @par 描述:
 * 摘要运算，支持SHA1，SHA224，SHA256，SHA384，SHA512
 *
 * @attention 不支持MD5
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param chunk [IN]  输入数据指针
 * @param chunkSize [IN]  输入数据长度
 *
 * @retval #TEE_SUCCESS 摘要运算成功
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_DigestDoFinal
 * @since V100R002C00B302
*/
TEE_Result TEE_DigestUpdate(TEE_OperationHandle operation, void *chunk, size_t chunkSize);
/**
 * @ingroup  crypto
 * @brief 摘要运算结束
 *
 * @par 描述:
 * 摘要运算结束，支持SHA1，SHA224，SHA256，SHA384，SHA512
 *
 * @attention 不支持MD5
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param chunk [IN]  输入数据指针
 * @param chunkSize [IN]  输入数据长度
 * @param hash [OUT]  输出摘要数据指针
 * @param hashLen [OUT]  输出摘要数据长度 固定长度且与算法有关
 *
 * @retval #TEE_SUCCESS 摘要结束成功
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_DigestUpdate
 * @since V100R002C00B302
*/
TEE_Result TEE_DigestDoFinal(TEE_OperationHandle operation,
			     void *chunk, size_t chunkLen, void *hash, size_t *hashLen);
/**
 * @ingroup  crypto
 * @brief MAC初始化
 *
 * @par 描述:
 * MAC初始化，支持SHA1，SHA224，SHA256，SHA384，SHA512
 *
 * @attention 不支持MD5
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param IV [IN]  初始化向量指针
 * @param IVLen [IN]  初始化向量长度
 *
 * @retval 无
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_MACUpdate | TEE_MACComputeFinal | TEE_MACCompareFinal
 * @since V100R002C00B302
*/
void TEE_MACInit(TEE_OperationHandle operation, void *IV, size_t IVLen);
/**
 * @ingroup  crypto
 * @brief MAC运算
 *
 * @par 描述:
 * MAC运算，支持SHA1，SHA224，SHA256，SHA384，SHA512
 *
 * @attention 不支持MD5
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param chunk [IN]  输入数据指针
 * @param chunkSize [IN]  输入数据长度
 *
 * @retval 无
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_MACInit | TEE_MACComputeFinal | TEE_MACCompareFinal
 * @since V100R002C00B302
*/
void TEE_MACUpdate(TEE_OperationHandle operation, void *chunk, size_t chunkSize);
/**
 * @ingroup  crypto
 * @brief MAC运算结束
 *
 * @par 描述:
 * MAC运算结束，支持SHA1，SHA224，SHA256，SHA384，SHA512
 *
 * @attention 不支持MD5
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param message [IN]  输入比较数据指针
 * @param messageLen [IN]  输入比较数据长度
 * @param mac [OUT] 输出摘要数据指针
 * @param macLen [OUT] 输出摘要数据长度
 *
 * @retval #TEE_SUCCESS MAC结束成功
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_MACInit | TEE_MACUpdate | TEE_MACCompareFinal
 * @since V100R002C00B302
*/
TEE_Result TEE_MACComputeFinal(TEE_OperationHandle operation,
			       void *message, size_t messageLen, void *mac, size_t *macLen);
/**
 * @ingroup  crypto
 * @brief MAC运算结束，并将结果和输入数据比较
 *
 * @par 描述:
 * MAC运算结束，支持SHA1，SHA224，SHA256，SHA384，SHA512
 *
 * @attention 不支持MD5
 * @param operation [IN/OUT]  模块句柄
 * @param message [IN]  输入比较数据指针
 * @param messageLen [IN]  输入比较数据长度
 * @param mac [IN] 输出摘要数据指针
 * @param macLen [IN] 输出摘要数据长度
 *
 * @retval #TEE_SUCCESS MAC验证成功
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 * @retval #TEE_ERROR_MAC_INVALID 校验失败
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_MACInit | TEE_MACUpdate | TEE_MACComputeFinal
 * @since V100R002C00B302
*/
TEE_Result TEE_MACCompareFinal(TEE_OperationHandle operation,
			       void *message, size_t messageLen, void *mac, size_t *macLen);
/**
 * @ingroup  crypto
 * @brief 根据传入参数生成配对公私钥
 *
 * @par 描述:
 * 根据传入参数生成配对公私钥，用于后期的密钥派生，该接口TEE未提供，属于自定义接口
 *
 * @attention 参数排列解析请开发文档
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param params [IN]  参数数据指针 #TEE_Attribute
 * @param paramCount [IN]  参数数据个数
 *
 * @retval 无
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see 无
 * @since V100R002C00B302
*/
void GeneratePubPrv(TEE_OperationHandle operation, TEE_Attribute *params, uint32_t paramCount);
/**
 * @ingroup  crypto
 * @brief 根据传入参数派生密钥
 *
 * @par 描述:
 * 根据传入参数派生密钥，传入参数为shared secret，派生密钥基于rootkey，长度固定为128bits
 *
 * @attention 无
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param params [IN]  共享机密数据指针 #TEE_Attribute
 * @param paramCount [IN]  共享机密数据个数
 * @param derivedKey [OUT] 派生密钥句柄 #TEE_ObjectHandle
 *
 * @retval 无
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see 无
 * @since V100R002C00B302
*/
void TEE_DeriveKey(TEE_OperationHandle operation, TEE_Attribute *params,
		   uint32_t paramCount, TEE_ObjectHandle derivedKey);
/**
 * @ingroup  crypto
 * @brief 生成随机数
 *
 * @par 描述:
 * 生成随机数
 *
 * @attention 无
 * @param randomBuffer [IN/OUT]  随机数指针
 * @param randomBufferLen [IN]  随机数长度
 *
 * @retval 无
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see 无
 * @since V100R002C00B302
*/
void TEE_GenerateRandom(void *randomBuffer, size_t randomBufferLen);
/**
 * @ingroup  crypto
 * @brief 认证加解密初始化
 *
 * @par 描述:
 * 认证加解密初始化，只支持AES-CCM
 *
 * @attention 不支持AES-GCM
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param nonce [IN]  输入随机数数据指针
 * @param nonceLen [IN]  输入随机数数据长度 [7,8,9,10,11,12,13]
 * @param tagLen [IN] 按位表示的摘要数据长度[128, 112, 96, 64, 48, 32]
 * @param AADLen [IN] 附加认证数据长度
 * @param payloadLen [IN] 加解密数据长度
 *
 * @retval #TEE_SUCCESS AE(Authenticated Encryption)初始化成功
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_AEUpdateAAD | TEE_AEUpdate | TEE_AEEncryptFinal | TEE_AEDecryptFinal
 * @since V100R002C00B302
*/
TEE_Result TEE_AEInit(TEE_OperationHandle operation,
		      void *nonce, size_t nonceLen, uint32_t tagLen,
		      uint32_t AADLen, uint32_t payloadLen);
/**
 * @ingroup  crypto
 * @brief 认证加解密AAD运算
 *
 * @par 描述:
 * 认证加解密AAD运算，只支持AES-CCM
 *
 * @attention 不支持AES-GCM
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param AADdata [IN]  输入附加认证数据指针
 * @param AADdataLen [IN]  输入附加认证数据长度
 *
 * @retval 无
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_AEInit | TEE_AEUpdate | TEE_AEEncryptFinal | TEE_AEDecryptFinal
 * @since V100R002C00B302
*/
void TEE_AEUpdateAAD(TEE_OperationHandle operation,
		     void *AADdata, size_t AADdataLen);
/**
 * ingroup  crypto
 * @brief 认证加解密运算
 *
 * @par 描述:
 * 认证加解密运算，只支持AES-CCM
 *
 * @attention 不支持AES-GCM
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param srcData [IN]  输入加密/解密数据指针
 * @param srcLen [IN]  输入加密/解密数据长度
 * @param destData [OUT] 输出解密/加密数据指针
 * @param destLen [OUT] 输出解密/加密数据长度
 *
 * @retval #TEE_SUCCESS AE(Authenticated Encryption)加解密运算成功
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_AEUpdateAAD | TEE_AEUpdateAAD | TEE_AEEncryptFinal | TEE_AEDecryptFinal
 * @since V100R002C00B302
*/
TEE_Result TEE_AEUpdate(TEE_OperationHandle operation, void *srcData,
			size_t srcLen, void *destData, size_t *destLen);
/**
 * @ingroup  crypto
 * @brief 认证加密运算结束，并计算tag
 *
 * @par 描述:
 * 认证加密运算结束，并计算tag，只支持AES-CCM
 *
 * @attention 不支持AES-GCM
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param srcData [IN]  输入加密/解密数据指针
 * @param srcLen [IN]  输入加密/解密数据长度
 * @param destData [OUT] 输出解密/加密数据指针
 * @param destLen [OUT] 输出解密/加密数据长度
 * @param tag [OUT] 校验数据指针
 * @param tagLen [OUT] 校验数据长度
 *
 * @retval #TEE_SUCCESS AE(Authenticated Encryption)加密运算结束
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_AEUpdateAAD | TEE_AEUpdateAAD | TEE_AEUpdate | TEE_AEDecryptFinal
 * @since V100R002C00B302
*/
TEE_Result TEE_AEEncryptFinal(TEE_OperationHandle operation,
			      void *srcData, size_t srcLen, void *destData, size_t *destLen,
			      void *tag, size_t *tagLen);
/**
 * @ingroup  crypto
 * @brief 认证解密运算结束，并计算tag
 *
 * @par 描述:
 * 认证解密运算结束，并计算tag，只支持AES-CCM
 *
 * @attention 不支持AES-GCM
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param srcData [IN]  输入加密/解密数据指针
 * @param srcLen [IN]  输入加密/解密数据长度
 * @param destData [OUT] 输出解密/加密数据指针
 * @param destLen [OUT] 输出解密/加密数据长度
 * @param tag [IN] 校验数据指针
 * @param tagLen [IN] 校验数据长度
 *
 * @retval #TEE_SUCCESS AE(Authenticated Encryption)解密运算结束
 * @retval #TEE_ERROR_MAC_INVALID 校验返回错误
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_AEUpdateAAD | TEE_AEUpdateAAD | TEE_AEUpdate | TEE_AEEncryptFinal
 * @since V100R002C00B302
*/
TEE_Result TEE_AEDecryptFinal(TEE_OperationHandle operation, void *srcData,
			      size_t srcLen, void *destData, size_t *destLen,
			      void *tag, size_t tagLen);
/**
 * @ingroup  crypto
 * @brief 非对称数据加密
 *
 * @par 描述:
 * 非对称数据加密，只执行一次。
 *
 * @attention 非对称加密只能使用公钥，私钥加密会返回错误
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param params [IN] 参数指针 #TEE_Attribute
 * @param paramCount [IN] 参数个数
 * @param srcData [IN]  输入加密数据指针
 * @param srcLen [IN]  输入加密数据长度
 * @param destData [OUT] 输出解密数据指针
 * @param destLen [OUT] 输出解密数据长度
 *
 * @retval #TEE_SUCCESS 非对称加密成功
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 * @retval #TEE_ERROR_BAD_PARAMETERS 参数为NULL，或传入错误
 * @retval #TEE_ERROR_NO_DATA 公钥为空，没有设置
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_AsymmetricDecrypt
 * @since V100R002C00B302
*/
TEE_Result TEE_AsymmetricEncrypt(TEE_OperationHandle operation,
				 TEE_Attribute *params, uint32_t paramCount, void *srcData,
				 size_t srcLen, void *destData, size_t *destLen);
/**
 * @ingroup  crypto
 * @brief 非对称数据解密
 *
 * @par 描述:
 * 非对称数据解密，只执行一次。
 *
 * @attention 非对称解密只能使用私钥，公钥解密会返回错误
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param params [IN] 参数指针 #TEE_Attribute
 * @param paramCount [IN] 参数个数
 * @param srcData [IN]  输入解密数据指针
 * @param srcLen [IN]  输入解密数据长度
 * @param destData [OUT] 输出加密数据指针
 * @param destLen [OUT] 输出加密数据长度
 *
 * @retval #TEE_SUCCESS 非对称加密成功
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 * @retval #TEE_ERROR_BAD_PARAMETERS 参数为NULL，或传入错误
 * @retval #TEE_ERROR_NO_DATA 私钥为空，没有设置
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_AsymmetricEncrypt
 * @since V100R002C00B302
*/
TEE_Result TEE_AsymmetricDecrypt(TEE_OperationHandle operation,
				 TEE_Attribute *params, uint32_t paramCount, void *srcData,
				 size_t srcLen, void *destData, size_t *destLen);
/**
 * @ingroup  crypto
 * @brief 非对称数据签名摘要
 *
 * @par 描述:
 * 非对称数据签名摘要
 *
 * @attention 只能使用私钥进行签名，公钥签名返回错误
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param params [IN] 参数指针 #TEE_Attribute
 * @param paramCount [IN] 参数个数
 * @param digest [IN]  摘要数据指针
 * @param digestLen [IN]  摘要数据长度
 * @param signature [OUT] 签名数据指针
 * @param signatureLen [OUT] 签名数据长度
 *
 * @retval #TEE_SUCCESS 非对称签名成功
 * @retval #TEE_ERROR_GENERIC 硬件加解密底层驱动错误
 * @retval #TEE_ERROR_BAD_PARAMETERS 参数为NULL，或传入错误
 * @retval #TEE_ERROR_NO_DATA 私钥为空，没有设置
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_AsymmetricVerifyDigest
 * @since V100R002C00B302
*/
TEE_Result TEE_AsymmetricSignDigest(TEE_OperationHandle operation,
				    TEE_Attribute *params, uint32_t paramCount, void *digest,
				    size_t digestLen, void *signature, size_t *signatureLen);
/**
 * @ingroup  crypto
 * @brief 非对称数据验证摘要
 *
 * @par 描述:
 * 非对称数据验证摘要
 *
 * @attention 只能使用公钥进行验证，私钥验证返回错误
 * @param operation [IN/OUT]  模块句柄 #TEE_OperationHandle
 * @param params [IN] 参数指针 #TEE_Attribute
 * @param paramCount [IN] 参数个数
 * @param digest [IN]  摘要数据指针
 * @param digestLen [IN]  摘要数据长度
 * @param signature [IN] 签名数据指针
 * @param signatureLen [IN] 签名数据长度
 *
 * @retval #TEE_SUCCESS 非对称验证成功
 * @retval #TEE_ERROR_GENERIC 调用硬件驱动返回错误
 * @retval #TEE_ERROR_BAD_PARAMETERS 参数为NULL，或传入错误
 * @retval #TEE_ERROR_NO_DATA 公钥为空，没有设置
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_crypto_api.h 加解密API头文件
 * @see TEE_AsymmetricSignDigest
 * @since V100R002C00B302
*/
TEE_Result TEE_AsymmetricVerifyDigest(TEE_OperationHandle operation,
				      TEE_Attribute *params, uint32_t paramCount, void *digest,
				      size_t digestLen, void *signature, size_t signatureLen);
#endif

/**
* History: \n
* 2013-04-11，q00209673，Create \n
*
* vi: set expandtab ts=4 sw=4 tw=80:
*/
