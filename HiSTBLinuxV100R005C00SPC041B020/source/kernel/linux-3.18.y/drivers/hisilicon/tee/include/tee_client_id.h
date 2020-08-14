/**
 * @file tee_client_id.h
 *
 * Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：安全世界提供的安全服务与命令定义头文件\n
 */
#ifndef _TEE_CLIENT_ID_H_
#define _TEE_CLIENT_ID_H_

/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 安全服务Global的UUID，安全服务Global是所有其它安全服务的基础
 */
#define TEE_SERVICE_GLOBAL \
{ \
    0x00000000, \
    0x0000, \
    0x0000, \
    { \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 \
    } \
}

/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 安全服务Echo的UUID
 */
#define TEE_SERVICE_ECHO \
{ \
    0x01010101, \
    0x0101, \
    0x0101, \
    { \
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 \
    } \
}

/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 安全存储服务的UUID
 */
#define TEE_SERVICE_STORAGE \
{ \
    0x02020202, \
    0x0202, \
    0x0202, \
    { \
        0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 \
    } \
}

/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 安全服务UT测试的UUID
 */
#define TEE_SERVICE_UT \
{ \
    0x03030303, \
    0x0303, \
    0x0303, \
    { \
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 \
    } \
}

/**
 * @ingroup  TEE_ID
 *
 * 加解密服务的UUID
 */
#define TEE_SERVICE_CRYPTO \
{ \
    0x04040404, \
    0x0404, \
    0x0404, \
    { \
        0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04 \
    } \
}

/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 安全服务Global支持的命令ID
 */
enum SVC_GLOBAL_CMD_ID {
    GLOBAL_CMD_ID_INVALID = 0x0,        /**< 安全服务Global，无效的命令*/
    GLOBAL_CMD_ID_BOOT_ACK,        /**< 安全服务Global，启动应答*/
    GLOBAL_CMD_ID_OPEN_SESSION,        /**< 安全服务Global，打开Session*/
    GLOBAL_CMD_ID_CLOSE_SESSION,        /**< 安全服务Global，关闭Session*/
    GLOBAL_CMD_ID_LOAD_SECURE_APP,  /**< 安全服务Global，动态加载安全应用*/
    GLOBAL_CMD_ID_NEED_LOAD_APP,    /**< 安全服务Global，判断是否是需要加载安全应用*/
    GLOBAL_CMD_ID_REGISTER_AGENT,   /**< 安全服务Global，注册代理*/
    GLOBAL_CMD_ID_UNREGISTER_AGENT, /**< 安全服务Global，注销代理*/
    GLOBAL_CMD_ID_REGISTER_NOTIFY_MEMORY,   /**< 安全服务Global，注册异步调用缓冲区*/
    GLOBAL_CMD_ID_UNREGISTER_NOTIFY_MEMORY, /**< 安全服务Global，注销异步调用缓冲区*/
    GLOBAL_CMD_ID_INIT_CONTENT_PATH,      /**< 安全服务Global，初始化content path*/
    GLOBAL_CMD_ID_TERMINATE_CONTENT_PATH,   /**< 安全服务Global，释放content path*/
    GLOBAL_CMD_ID_ALLOC_EXCEPTION_MEM,  /**< 安全服务Global，分配异常信息保存空间*/
    GLOBAL_CMD_ID_ALLOC_SMC_MEM,
    GLOBAL_CMD_ID_FREE_SMC_MEM,
    GLOBAL_CMD_ID_TEE_TIME,         /**< 安全服务Global，获取安全OS的时间 */
    //GLOBAL_CMD_ID_MAX,        /**< 安全服务Global，未支持的命令*/
    GLOBAL_CMD_ID_UNKNOWN         = 0x7FFFFFFE,
    GLOBAL_CMD_ID_MAX             = 0x7FFFFFFF
};

/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 安全服务echo支持的命令ID
 */
enum SVC_ECHO_CMD_ID {
    ECHO_CMD_ID_INVALID = 0x0,        /**< Echo Task无效的命令*/
    ECHO_CMD_ID_SEND_CMD,        /**< Echo Task发送命令*/
    ECHO_CMD_ID_MAX,        /**< Echo Task未支持的命令*/
};

/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 安全存储服务支持的命令ID
 */
enum SVC_STORAGE_CMD_ID {
    STORAGE_CMD_ID_INVALID = 0x10,        /**< 安全存储服务，无效的命令*/
    STORAGE_CMD_ID_OPEN,        /**< 安全存储服务，打开文件命令*/
    STORAGE_CMD_ID_CLOSE,        /**< 安全存储服务，关闭文件命令*/
    STORAGE_CMD_ID_CLOSEALL,        /**< 安全存储服务，关闭所有文件命令*/
    STORAGE_CMD_ID_READ,        /**< 安全存储服务，读取文件命令*/
    STORAGE_CMD_ID_WRITE,        /**< 安全存储服务，写入文件命令*/
    STORAGE_CMD_ID_MAX,        /**< 安全存储服务，未支持的命令*/
};

/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 安全服务CRYPTO支持的命令ID, 包括对称加解密、非对称加解密、摘要HMAC等\n
 * 注意:对称算法中的nopad模式需要TA(Trusted Application，可信应用)来做填充
 */
enum SVC_CRYPT_CMD_ID {
    CRYPT_CMD_ID_INVALID = 0x10,            /**< 无效ID */
    CRYPT_CMD_ID_ENCRYPT,                   /**< 加密 */
    CRYPT_CMD_ID_DECRYPT,                   /**< 解密 */
    CRYPT_CMD_ID_MD5,                       /**< 摘要算法MD5 */
    CRYPT_CMD_ID_SHA1,                      /**< 摘要算法SHA1 */
    CRYPT_CMD_ID_SHA224,                    /**< 摘要算法SHA224 */
    CRYPT_CMD_ID_SHA256,                    /**< 摘要算法SHA256 */
    CRYPT_CMD_ID_SHA384,                    /**< 摘要算法SHA384 */
    CRYPT_CMD_ID_SHA512,                    /**< 摘要算法SHA512 */
    CRYPT_CMD_ID_HMAC_MD5,                  /**< HMAC MD5 */
    CRYPT_CMD_ID_HMAC_SHA1,                 /**< HMAC SHA1 */
    CRYPT_CMD_ID_HMAC_SHA224,               /**< HMAC SHA224 */
    CRYPT_CMD_ID_HMAC_SHA256,               /**< HMAC SHA256 */
    CRYPT_CMD_ID_HMAC_SHA384,               /**< HMAC SHA384 */
    CRYPT_CMD_ID_HMAC_SHA512,               /**< HMAC SHA512 */
    CRYPT_CMD_ID_CIPHER_AES_CBC,        /**< 对称加解密AES 128bits密钥 CBC模式 */
    CRYPT_CMD_ID_CIPHER_AES_CBC_CTS,    /**< 对称加解密AES 128bits密钥 CBC_CTS模式 */
    CRYPT_CMD_ID_CIPHER_AES_ECB,        /**< 对称加解密AES 128bits密钥 ECB模式 */
    CRYPT_CMD_ID_CIPHER_AES_CTR,        /**< 对称加解密AES 128bits密钥 CTR模式 */
    CRYPT_CMD_ID_CIPHER_AES_CBC_MAC,    /**< 对称加解密AES 128bits密钥 CBC_MAC模式 */
    CRYPT_CMD_ID_CIPHER_AES_XCBC_MAC,   /**< 对称加解密AES 128bits密钥 XCBC_MAC模式 */
    CRYPT_CMD_ID_CIPHER_AES_CMAC,       /**< 对称加解密AES 128bits密钥 CMAC模式 */
    CRYPT_CMD_ID_CIPHER_AES_CCM,        /**< 对称加解密AES 128bits密钥 CCM模式 */
    CRYPT_CMD_ID_CIPHER_AES_XTS,        /**< 对称加解密AES 128bits密钥 XTS模式 */
    CRYPT_CMD_ID_CIPHER_DES_ECB,            /**< 对称加解密DES ECB模式 */
    CRYPT_CMD_ID_CIPHER_DES_CBC,            /**< 对称加解密DES CBC模式 */
    CRYPT_CMD_ID_CIPHER_DES3_ECB,           /**< 对称加解密DES3 ECB模式 */
    CRYPT_CMD_ID_CIPHER_DES3_CBC,           /**< 对称加解密DES3 CBC模式 */
    CRYPT_CMD_ID_CIPHER_RND,                /**< 随机数模式 */
    CRYPT_CMD_ID_CIPHER_DK,                 /**< 密钥派生模式 */
    CRYPT_CMD_ID_RSAES_PKCS1_V1_5,          /**< 非对称加解密PKCS1_V1_5模式 */
    CRYPT_CMD_ID_RSAES_PKCS1_OAEP_MGF1_SHA1,/**< 非对称加解密OAEP_SHA1模式 */
    CRYPT_CMD_ID_RSAES_PKCS1_OAEP_MGF1_SHA224,/**< 非对称加解密OAEP_SHA224模式 */
    CRYPT_CMD_ID_RSAES_PKCS1_OAEP_MGF1_SHA256,/**< 非对称加解密OAEP_SHA256模式 */
    CRYPT_CMD_ID_RSAES_PKCS1_OAEP_MGF1_SHA384,/**< 非对称加解密OAEP_SHA384模式 */
    CRYPT_CMD_ID_RSAES_PKCS1_OAEP_MGF1_SHA512,/**< 非对称加解密OAEP_SHA512模式 */
    CRYPT_CMD_ID_RSA_NOPAD,                 /**< 非对称加解密无填充模式 */
    CRYPT_CMD_ID_RSASSA_PKCS1_V1_5_MD5,     /**< 非对称签名验证PKCS1_V1_5_MD5模式 */
    CRYPT_CMD_ID_RSASSA_PKCS1_V1_5_SHA1,    /**< 非对称签名验证PKCS1_V1_5_SHA1模式 */
    CRYPT_CMD_ID_RSASSA_PKCS1_V1_5_SHA224,  /**< 非对称签名验证PKCS1_V1_5_SHA224模式 */
    CRYPT_CMD_ID_RSASSA_PKCS1_V1_5_SHA256,  /**< 非对称签名验证PKCS1_V1_5_SHA256模式 */
    CRYPT_CMD_ID_RSASSA_PKCS1_V1_5_SHA384,  /**< 非对称签名验证PKCS1_V1_5_SHA384模式 */
    CRYPT_CMD_ID_RSASSA_PKCS1_V1_5_SHA512,  /**< 非对称签名验证PKCS1_V1_5_SHA512模式 */
    CRYPT_CMD_ID_RSASSA_PKCS1_PSS_MGF1_SHA1,/**< 非对称签名验证PSS_MGF1_SHA1模式 */
    CRYPT_CMD_ID_RSASSA_PKCS1_PSS_MGF1_SHA224,/**< 非对称签名验证PSS_MGF1_SHA224模式 */
    CRYPT_CMD_ID_RSASSA_PKCS1_PSS_MGF1_SHA256,/**< 非对称签名验证PSS_MGF1_SHA256模式 */
    CRYPT_CMD_ID_RSASSA_PKCS1_PSS_MGF1_SHA384,/**< 非对称签名验证PSS_MGF1_SHA384模式 */
    CRYPT_CMD_ID_RSASSA_PKCS1_PSS_MGF1_SHA512,/**< 非对称签名验证PSS_MGF1_SHA512模式 */
    CRYPT_CMD_ID_DSA_SHA1,                  /**< 非对称签名验证DSA模式 */
    CRYPT_CMD_ID_UNKNOWN = 0x7FFFFFFE,      /**< 未知模式 */
    CRYPT_CMD_ID_MAX = 0x7FFFFFFF           /**< 最大CMD ID */
};
#endif
/**
 * History: \n
 * 2013-4-12 h00211444: Create this file\n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
*/
