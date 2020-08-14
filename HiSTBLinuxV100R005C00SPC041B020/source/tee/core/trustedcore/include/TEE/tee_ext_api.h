#ifndef __TEE_EXT_API_H
#define __TEE_EXT_API_H
#include "tee_internal_api.h"

#define TEE_INFO_RESERVED (0xFFFFFFFF)
/* Any reasons for these command IDs??? */
#define TEE_RETURN_AGENT_BUFFER  0x99
#define TEE_INVALID_AGENT        0x66
#define TEE_AGENT_LOCK           0x33
TEE_Result TEE_EXT_GetDeviceUniqueId(uint8_t *DevUniqueID,  uint32_t *length);
TEE_Result TEE_EXT_GetAgentBuffer(uint32_t agent_id, void **buffer, uint32_t *length);
TEE_Result TEE_EXT_SendAgentRequest(uint32_t agent_id);


#define TEE_GET_REEINFO_SUCCESS 0
#define TEE_GET_REEINFO_FAILED 1
struct __ree_info {
	uint32_t userid;
	uint32_t appid;
	uint32_t reserve;
};
typedef struct __ree_info ree_info;
TEE_Result TEE_EXT_GetREEInfo(ree_info *ree_info_data,  uint32_t *length);
#define TEE_EXT_GET_REE_USERID ({ \
    TEE_Result ret; \
    ree_info ree_info_data; \
    uint32_t length = sizeof(ree_info); \
    uint32_t userid; \
    ret = TEE_EXT_GetREEInfo(&ree_info_data, &length); \
    if (ret == TEE_SUCCESS) \
        userid = ree_info_data.userid; \
    else \
        userid = 0; \
    userid;})

#define RESERVED_BUF_SIZE   (32)
typedef struct __caller_info {
	uint32_t session_type;
	union {
		TEE_UUID caller_uuid;
		uint8_t ca_info[RESERVED_BUF_SIZE]; //reserved
	} caller_identity;
	uint8_t reserved[RESERVED_BUF_SIZE]; //reserved
} caller_info;

TEE_Result TEE_EXT_GetCallerInfo(caller_info *caller_info_data, uint32_t length);
/**
 * @ingroup  crypto
 * @brief AES加解密集成模式
 *
 * @par 描述:
 * AES集成加解密，目前支持AES_CBC_NOPAD,AES_ECB_PKCS5，可以通过rootkey派生密钥
 *
 * @attention 目前只支持ECB和CBC
 * @param DB        [IN]        是否用传入的key和rootkey派生密钥
 * @param IV        [IN]        初始化向量指针
 * @param mode      [IN]        0:加密;1:解密
 * @param algothem  [IN]        AES算法 #TEE_CRYPTO_ALGORITHM_ID
 * @param key       [IN]        密钥指针
 * @param key_len   [IN]        指针长度
 * @param cyp_src   [IN]        输入数据
 * @param dst       [OUT]       输出数据
 * @param srclen    [IN]        输入数据长度
 * @param dstlen    [OUT]       输出数据长度
 *
 * @retval #TEE_SUCCESS                 加解密成功
 * @retval #TEE_ERROR_BAD_PARAMETERS    输入参数错误
 * @retval #TEE_ERROR_OUT_OF_MEMORY     分配内存失败
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_ext_api.h 扩展API头文件
 * @see TEE_CipherInit | TEE_CipherUpdate | TEE_CipherDoFinal
 * @since V100R005C00
*/
TEE_Result TEE_EXT_AES_CRYPTO(bool DB, uint8_t *IV, bool mode, uint32_t algothem, uint8_t *key, uint32_t key_len,
			      uint8_t *cyp_src, uint8_t *dst, uint32_t srclen, uint32_t *dstlen);

/**
 * @ingroup  crypto
 * @brief 计算摘要
 *
 * @par 描述:
 * 计算摘要
 *
 * @attention
 * @param algorithm     [IN]            摘要算法，如SHA1,SHA256
 * @param DataIn_ptr    [IN]            输入数据
 * @param DataSize      [IN]            输入数据长度
 * @param hash_result   [OUT]           摘要结果 SHA1 20bytes;SHA256 32bytes
 *
 * @retval #TEE_SUCCESS                 摘要成功
 * @retval #TEE_ERROR_BAD_PARAMETERS    输入参数错误
 * @retval #TEE_ERROR_GENERIC           摘要失败
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_ext_api.h 扩展API头文件
 * @since V100R005C00
*/
TEE_Result TEE_EXT_HASH(TEE_CRYPTO_ALGORITHM_ID algorithm, uint8_t *DataIn_ptr, uint32_t DataSize, uint8_t *hash_result);

/**
 * @ingroup  crypto
 * @brief 生成RSA密钥对
 *
 * @par 描述:
 * RSA密钥对生成，输出为加密后密钥对数据结构
 *
 * @attention 输出密钥对数据结构只能用于本模块产生密钥
 * @param pubExp        [IN]        公钥的exponent 例如0x10001
 * @param key_size      [IN]        密钥长度 一般为2048bit
 * @param key_blob      [OUT]       加密后的密钥对数据结构
 * @param key_blob_size [OUT]       key_blob长度
 *
 * @retval #TEE_SUCCESS                 加解密成功
 * @retval #TEE_ERROR_BAD_PARAMETERS    输入参数错误
 * @retval #TEE_ERROR_OUT_OF_MEMORY     分配内存失败
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_ext_api.h 扩展API头文件
 * @see
 * @since V100R005C00
*/
TEE_Result TEE_EXT_generate_keypair(uint32_t pubExp, uint32_t key_size, uint8_t *key_blob, uint32_t *key_blob_size);

/**
 * @ingroup  crypto
 * @brief 获取公钥
 *
 * @par 描述:
 * RSA公钥获取
 *
 * @attention
 * @param key_blob      [IN]        密钥对数据结构
 * @param exopnent      [OUT]       公钥的exponent
 * @param modulus       [OUT]       公钥的modulus
 * @param exponentSize  [OUT]       exponent长度
 * @param modulusSize   [OUT]       modulus长度
 *
 * @retval #TEE_SUCCESS                 获取公钥成功
 * @retval #TEE_ERROR_BAD_PARAMETERS    输入参数错误
 * @retval #TEE_ERROR_GENERIC           获取公钥失败
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_ext_api.h 扩展API头文件
 * @see
 * @since V100R005C00
*/
TEE_Result TEE_EXT_get_keypair_public(uint8_t *key_blob, uint8_t *exopnent, uint8_t *modulus,
				      uint32_t *exponentSize, uint32_t *modulusSize);

/**
 * @ingroup  crypto
 * @brief 对数据签名
 *
 * @par 描述:
 * 对数据签名，算法有限制，目前支持pkcs1v15和pkcs1v21以及nopad
 *
 * @attention
 * @param algorithm     [IN]        RSA算法
 * @param key_blob      [IN]        密钥对数据结构
 * @param datain        [IN]        需要签名的数据
 * @param inSize        [IN]        需要签名数据长度
 * @param dataout       [OUT]       签名后的数据
 * @param outSize       [OUT]       签名后的数据长度
 *
 * @retval #TEE_SUCCESS                 获取公钥成功
 * @retval #TEE_ERROR_BAD_PARAMETERS    输入参数错误
 * @retval #TEE_ERROR_GENERIC           获取公钥失败
 * @retval #TEE_ERROR_OUT_OF_MEMORY     分配内存失败
 * @retval #TEE_ERROR_NOT_SUPPORTED     算法不支持
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_ext_api.h 扩展API头文件
 * @see
 * @since V100R005C00
*/
TEE_Result TEE_EXT_sign_data(TEE_CRYPTO_ALGORITHM_ID algorithm, uint8_t *key_blob, uint8_t *datain,
			     uint32_t inSize, uint8_t *dataout, uint32_t *outSize);

/**
 * @ingroup  crypto
 * @brief 对数据签名
 *
 * @par 描述:
 * 对数据签名，算法有限制，目前支持pkcs1v15和pkcs1v21以及nopad
 *
 * @attention
 * @param algorithm     [IN]        RSA算法
 * @param key_blob      [IN]        密钥对数据结构
 * @param srcData       [IN]        需要解密的数据
 * @param srcLen        [IN]        需要解密数据长度
 * @param destData      [OUT]       解密后的数据
 * @param destLen       [OUT]       解密后的数据长度
 *
 * @retval #TEE_SUCCESS                 获取公钥成功
 * @retval #TEE_ERROR_BAD_PARAMETERS    输入参数错误
 * @retval #TEE_ERROR_GENERIC           获取公钥失败
 * @retval #TEE_ERROR_OUT_OF_MEMORY     分配内存失败
 * @retval #TEE_ERROR_NOT_SUPPORTED     算法不支持
 *
 * @par 依赖:
 * @li crypto 加解密模块
 * @li tee_ext_api.h 扩展API头文件
 * @see
 * @since V100R005C00
*/
TEE_Result TEE_EXT_RSA_decrypt(TEE_CRYPTO_ALGORITHM_ID algorithm, uint8_t *key_blob,
			       uint8_t *srcData, uint32_t srcLen, uint8_t *destData, uint32_t *destLen);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 在安全侧删除某个文件夹目录
 *
 * @par 描述:
 * 在安全侧删除某个文件夹目录
 *
 * @attention
 * 此接口会将整个目录删除，慎用
 * @param path     [IN]        需要删除的文件夹目录
 *
 * @retval
 *
 * @par 依赖:
 * @li tee_ext_api.h 扩展API头文件
 * @see
 * @since V100R005C00
*/
void TEE_EXT_rmDir(const char *path);

/**
 * @ingroup  TEE_HWI_IPC_MESSAGE
 * @brief Wait for registered messages from IRQs
*/
TEE_Result TEE_EXT_HwiMsgRegister(uint32_t hwi);

/**
 * @ingroup  TEE_HWI_IPC_MESSAGE
 * @brief Unregister to receive message for HWI number
 *
 * @param hwi     [IN]        IRQ number to unregister for
*/
TEE_Result TEE_EXT_HwiMsgDeregister(uint32_t hwi);

/**
 * @ingroup  TEE_HWI_IPC_MESSAGE
 * @brief Wait for registered messages from IRQs
*/
uint32_t TEE_EXT_HwiMsgWait();
/**
 * @ingroup  TEE_DEVICE_STATUS_CHECK
 * @brief check wheather device rooted 1:rooted, 0:unrooted
*/
bool TEE_EXT_IsDeviceRooted(void);
/**
 *  * @ingroup  TEE_DEVICE_STATUS_CHECK
 *   * @check device root status, the lowest BIT indicate overall status.
 *   */
uint32_t TEE_EXT_DeviceRootStatus(void);
/**
 * @ingroup  TEE_DEVICE_VSIM SHAREMEM GET
 * @brief get vsim sharemem:return:TEE_SUCCESS success to get, TEE_ERROR_BAD_PARAMETERS params error, TEE_ERROR_GENERIC:map error
*/
TEE_Result TEE_EXT_GetVsimShareMem(uint32_t *addr, uint32_t *size);


/**
 * @ingroup  TEE_TA_CALLER_INDENTIFY
 * @brief  verify TA's caller's identify
 *
 * @par  TA can call this API to add caller's info,
 *       which is allowed to call this TA.
 *       this API is for CA in form of binary-excuteble file.
 *
 * @param ca_name     [IN]		CA caller's packagename
 * @param ca_uid      [IN]		CA caller's uid
 *
 * @retval
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
*/
TEE_Result AddCaller_CA_exec(char *ca_name, uint32_t ca_uid);

/**
 * @ingroup  TEE_TA_CALLER_INDENTIFY
 * @brief  verify TA's caller's identify
 *
 * @par  TA can call this API to add caller's info,
 *       which is allowed to call this TA.
 *       this API is for CA in form of APK.
 *
 * @param ca_name       [IN]		CA caller's packagename
 * @param modulus       [IN]		CA caller's modulus
 * @param pub_exponent  [IN]		CA caller's pub_exponent
 *
 * @retval
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
*/
TEE_Result AddCaller_CA_apk(char *ca_name, char *modulus, char *pub_exponent);

/**
 * @ingroup  TEE_TA_CALLER_INDENTIFY
 * @brief  verify TA's caller's identify
 *
 * @par  TA can call this API to add caller's info,
 *       which is allowed to call this TA.
 *       this API is for caller is a TA.
 *       And there is no uuid check with this TA.
 *
 * @retval
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
*/
TEE_Result AddCaller_TA_all(void);

/**
 * @ingroup  ROOT_UuidDeriveKey
 * @brief  derive key from device rootkey and UUID of the current task
 *
 * @attention
 * @param salt          [IN]            data for salt
 * @param size          [IN]            salt length
 * @param key_size      [OUT]           size of generated key, fix-size 32 bytes
 * @param key           [OUT]           pointer where key is saved
 *
 * @retval #TEE_SUCCESS                 OK
 * @retval #TEE_ERROR_BAD_PARAMETERS    illegal parameters
 * @retval #TEE_ERROR_GENERIC           internal error
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
*/
TEE_Result TEE_EXT_ROOT_UuidDeriveKey(uint8_t *salt, uint32_t size, uint8_t *key, uint32_t *key_size);

/**
 * @ingroup  Cert
 * @brief  verify Cert in tee
 *
 * @attention
 * @param cert           [IN]            data for salt
 * @param cert_len       [IN]            salt length
 * @param parent_key     [IN]           size of generated key, fix-size 32 bytes
 * @param parent_key_len [IN]           pointer where key is saved
 *
 * @retval #TEE_SUCCESS                 OK
 * @retval #TEE_ERROR_BAD_PARAMETERS    illegal parameters
 * @retval #TEE_ERROR_GENERIC           internal error
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
*/
TEE_Result TEE_EXT_verify_dev_cert(uint8_t *cert, uint32_t cert_len, uint8_t *parent_key, uint32_t parent_key_len);

/**
 * @ingroup  Cert
 * @brief  create cert request in TEE
 *
 * @attention          return is the size of request
 *
 * @param buf          [OUT]            cert request output buffer
 * @param len          [OUT]            output buffer size
 * @param keytype      [IN]             key_type RSA 0; ECC 1
 * @param file_name    [IN]             pointer where key is saved
 *
 * @retval no-zero                      return is size of cert request
 * @retval #TEE_ERROR_BAD_PARAMETERS    illegal parameters
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
*/
TEE_Result TEE_EXT_create_cert_req(uint8_t *buf, size_t len, uint32_t keytype, uint8_t *file_name);

/**
 * @ingroup  TEE_EXT SKYTONE API
 * @brief  get skytone version
 *
 * @par
 *
 * @retval return the skytone version
 *
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
*/
uint32_t TEE_EXT_Get_Skytone_Version();

/**
 * @ingroup  rtc api for skytone
 * @brief  get rtc value from the reg
 *
 * @par
 *
 * @retval return the rtc value, if error return -1
 *
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
*/
uint32_t __sre_get_rtc_time();

/**
 * @ingroup  rtc api for skytone
 * @brief  set rtc value
 *
 * @par
 * @param time [IN] the value to be set
 *
 * @retval NA
 *
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
*/
void __sre_set_rtc_time(uint32_t time);

/**
 * @ingroup  share memory
 * @brief  get sharemem of verify boot information
 *
 * @par
 * @param buffer [OUT] the address to save verify boot info
 * @param size [IN] length of buffer
 *
 * @retval NA
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
*/
TEE_Result TEE_EXT_GetVerifyBootInfo(char *buffer, uint32_t size);

/**
 * @ingroup  derive key for keymaster
 * @brief  using root key to derive key for keymaster
 *
 * @par
 * @param buffer [IN] input secret
 * @param size [IN] input secret size
 * @param buffer [OUT] derived key
 * @param size [OUT] derived key size
 *
 * @retval NA
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
*/

#include "sre_sharedmem.h"
TEE_Result TEE_EXT_ROOT_DeriveKey2(uint8_t *secret, uint32_t secret_len,
				   uint8_t *key, uint32_t key_len);

extern int __tee_hal_get_dieid(unsigned int *pBuffer);
extern int __is_device_rooted(void);
extern int __get_vsim_sharemem(unsigned int *addr, unsigned int *size);
extern int32_t derive_private_key_from_secret(void *priv,
											uint8_t *secret,
											uint32_t secret_len,
											uint32_t bits,
											uint32_t keytype,
											uint8_t *file_name);

extern int32_t get_keytype_from_sp(const uint8_t *in, uint32_t inlen);
extern int32_t x509_cert_validate(uint8_t *cert, uint32_t cert_len, uint8_t *parent_key, uint32_t parent_key_len);
extern int32_t get_subject_public_key(uint8_t *pub, uint8_t *cert, uint32_t cert_len);
extern int32_t import_pub_from_sp(void *pub, const uint8_t *in, uint32_t inlen);
extern int __SRE_Get_SharedMem_Info(module_types type, unsigned int *buffer, unsigned int size);
TEE_Result TEE_EXT_read_endorsement_pub(uint8_t *pub, uint32_t *len);

TEE_Result tee_ConvertLswMswWordsToMsbLsbBytes(
	uint8_t *out8_ptr,
	uint32_t sizeOutBuffBytes,
	uint32_t *in32_ptr,
	uint32_t  sizeInBytes);

#endif
