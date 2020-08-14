/*******************************************************************************
* All rights reserved, Copyright (C) huawei LIMITED 2012
*------------------------------------------------------------------------------
* File Name   : tee_internal_api.h
* Description : Reference of TEE internal api and internal definitions
* Author      : yaodongdong@huawei.com
* Date        : 2012.12.31
* Notes       :
*
*------------------------------------------------------------------------------
* Modifications:
*   Date        Author          Modifications
*******************************************************************************/
/*******************************************************************************
 * This source code has been made available to you by HUAWEI on an
 * AS-IS basis. Anyone receiving this source code is licensed under HUAWEI
 * copyrights to use it in any way he or she deems fit, including copying it,
 * modifying it, compiling it, and redistributing it either with or without
 * modifications. Any person who transfers this source code or any derivative
 * work must include the HUAWEI copyright notice and this paragraph in
 * the transferred software.
*******************************************************************************/

/**
* @file tee_internal_api.h
*
* Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
*
* 描述：数据类型和常量定义\n
*/

/** @defgroup TEE_CommonDef 数据类型和常量定义
* @ingroup TEE_API
*/

#ifndef __TEE_INTERNAL_API_H
#define __TEE_INTERNAL_API_H

/**
 * @ingroup TEE_CommonDef
 * 无符号整型定义
 */
typedef unsigned long long uint64_t;
/**
 * @ingroup TEE_CommonDef
 * 无符号整型定义
 */
typedef unsigned int     uint32_t;

/**
 * @ingroup TEE_CommonDef
 * 有符号整型定义
 */
typedef signed int       int32_t;
/**
 * @ingroup TEE_CommonDef
 * 无符号短整型定义
 */
typedef unsigned short    uint16_t;
/**
 * @ingroup TEE_CommonDef
 * 有符号短整型定义
 */
typedef signed short      int16_t;
/**
 * @ingroup TEE_CommonDef
 * 无符号字符型定义
 */
typedef unsigned char     uint8_t;
/**
 * @ingroup TEE_CommonDef
 * 有符号字符型定义
 */
typedef signed char       int8_t;

#ifndef bool
/**
 * @ingroup TEE_CommonDef
 * 布尔型定义
 */
#define bool   _Bool
#endif
/**
 * @ingroup TEE_CommonDef
 * 数据长度类型定义
 */
typedef unsigned int    size_t;

typedef int *TEE_MUTEX_HANDLE;

/**
 * @ingroup TEE_CommonDef
 * true的值定义
 */
#undef true
#define true    1

/**
 * @ingroup TEE_CommonDef
 * false的值定义
 */
#undef false
#define false   0

/**
 * @ingroup TEE_CommonDef
 * NULL的值定义
 */
#ifndef NULL
#define  NULL   ((void*) 0)
#endif

#define PARAM_NOT_USED(val) ((void)val)

/**
 * @ingroup  TEE_CommonDef
 * TEE参数联合体结构定义
 *
 * 当参数是缓冲区类型时，联合体的内容指向memref\n
 * 当参数是数据类型时，联合体的内容指向value
 */
typedef union {
	struct {
		void *buffer;       /**< 缓冲区指针  */
		unsigned int size;  /**< 缓冲区大小  */
	} memref;
	struct {
		unsigned int a;     /**< 整形数据a  */
		unsigned int b;     /**< 整形数据b  */
	} value;
} TEE_Param;

/**
 * @ingroup TEE_CommonDef
 * 用于计算非安全世界与安全世界传递参数的数值
 */
#define TEE_PARAM_TYPES( param0Type, param1Type, param2Type, param3Type) \
    ((param3Type) << 12 | (param2Type) << 8 | (param1Type) << 4 | (param0Type))

/**
 * @ingroup TEE_CommonDef
 * 用于计算paramTypes中字段index的数值
 */
#define TEE_PARAM_TYPE_GET( paramTypes, index) \
    (((paramTypes) >> (4*(index))) & 0x0F)

/**
 * @ingroup TEE_CommonDef
 *
 * @brief 用于校验param type是否匹配
 *
 * @attention 无
 * @param param_to_check [IN]  待校验的参数类型
 * @param valid0-3 [IN] 正确的参数类型
 *
 * @retval #true 比较的参数类型匹配
 * @retval #false 比较的参数类型不匹配
 */
static inline bool check_param_type(uint32_t param_to_check, uint32_t valid0,
				    uint32_t valid1, uint32_t valid2, uint32_t valid3)
{
	return (TEE_PARAM_TYPES(valid0, valid1, valid2, valid3) == param_to_check);
}

/**
 * @ingroup  TEE_CommonDef
 *
 * 参数类型定义\n
 */
enum TEE_ParamType {
	TEE_PARAM_TYPE_NONE = 0x0,
	TEE_PARAM_TYPE_VALUE_INPUT = 0x1,
	TEE_PARAM_TYPE_VALUE_OUTPUT = 0x2,
	TEE_PARAM_TYPE_VALUE_INOUT = 0x3,
	TEE_PARAM_TYPE_MEMREF_INPUT = 0x5,
	TEE_PARAM_TYPE_MEMREF_OUTPUT = 0x6,
	TEE_PARAM_TYPE_MEMREF_INOUT = 0x7,
	TEE_PARAM_TYPE_ION_INPUT = 0x8,
};

#define S_VAR_NOT_USED(variable)  do{(void)(variable);}while(0);

/**
* @ingroup  TEE_CommonDef
*
* TEE对象结构体定义
*/
typedef struct {
	uint32_t objectType;        /**< 对象类型  */
	uint32_t objectSize;        /**< 对象数据长度  */
	uint32_t maxObjectSize;     /**< 对象允许的最大长度 */
	uint32_t objectUsage;       /**< 对象使用范围 */
	uint32_t dataSize;          /**< 对象的数据长度 */
	uint32_t dataPosition;      /**< 对象的数据位置 */
	uint32_t handleFlags;       /**< 对象数据访问方式 */
} TEE_ObjectInfo;

/**
* @ingroup  TEE_CommonDef
*
* TEE属性结构体定义
*/
typedef struct {
	uint32_t attributeID;           /**<Attribute ID  */
	union {
		struct {
			void *buffer;           /**< 缓冲区指针  */
			size_t length;          /**< 缓冲区长度  */
		} ref;
		struct {
			uint32_t a;             /**< 整形数据a  */
			uint32_t b;             /**< 整形数据b  */
		} value;
	} content;
} TEE_Attribute;

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 *
 * object attribute 的类型\n
*/
enum TEE_ObjectAttribute {
	TEE_ATTR_SECRET_VALUE = 0xC0000000,                   /**< attribute为SECRET_VALUE */
	TEE_ATTR_RSA_MODULUS = 0xD0000130,                    /**< attribute为RSA_MODULUS */
	TEE_ATTR_RSA_PUBLIC_EXPONENT = 0xD0000230,            /**< attribute为RSA_PUBLIC_EXPONENT */
	TEE_ATTR_RSA_PRIVATE_EXPONENT = 0xC0000330,           /**< attribute为RSA_PRIVATE_EXPONENT */
	TEE_ATTR_RSA_PRIME1 = 0xC0000430,                     /**< attribute为RSA_PRIME1 */
	TEE_ATTR_RSA_PRIME2 = 0xC0000530,                     /**< attribute为RSA_PRIME2 */
	TEE_ATTR_RSA_EXPONENT1 = 0xC0000630,                  /**< attribute为RSA_EXPONENT1 */
	TEE_ATTR_RSA_EXPONENT2 = 0xC0000730,                  /**< attribute为RSA_EXPONENT2 */
	TEE_ATTR_RSA_COEFFICIENT  = 0xC0000830,               /**< attribute为RSA_COEFFICIENT */
	TEE_ATTR_DSA_PRIME = 0xD0001031,                      /**< attribute为DSA_PRIME */
	TEE_ATTR_DSA_SUBPRIME = 0xD0001131,                   /**< attribute为DSA_SUBPRIME */
	TEE_ATTR_DSA_BASE = 0xD0001231,                       /**< attribute为DSA_BASE */
	TEE_ATTR_DSA_PUBLIC_VALUE = 0xD0000131,               /**< attribute为DSA_PUBLIC_VALUE */
	TEE_ATTR_DSA_PRIVATE_VALUE = 0xC0000231,              /**< attribute为DSA_PRIVATE_VALUE */
	TEE_ATTR_DH_PRIME = 0xD0001032,                       /**< attribute为DH_PRIME */
	TEE_ATTR_DH_SUBPRIME = 0xD0001132,                    /**< attribute为DH_SUBPRIME */
	TEE_ATTR_DH_BASE = 0xD0001232,                        /**< attribute为DH_BASE */
	TEE_ATTR_DH_X_BITS = 0xF0001332,                      /**< attribute为DH_X_BITS */
	TEE_ATTR_DH_PUBLIC_VALUE = 0xD0000132,                /**< attribute为DH_PUBLIC_VALUE */
	TEE_ATTR_DH_PRIVATE_VALUE = 0xC0000232,               /**< attribute为DH_PRIVATE_VALUE */
	TEE_ATTR_RSA_OAEP_LABEL = 0xD0000930,                 /**< attribute为RSA_OAEP_LABEL*/
	TEE_ATTR_RSA_PSS_SALT_LENGTH = 0xF0000A30,             /**< attribute为RSA_OAEP_LABEL */
	TEE_ATTR_ECC_PUBLIC_EXPONENT = 0xE0000001,            /**< attribute为ECC_PUBLIC_EXPONENT */
	TEE_ATTR_ECC_PRIVATE_EXPONENT = 0xE0000002,           /**< attribute为ECC_PRIVATE_EXPONENT */
	TEE_ATTR_ECC_DOMAINID = 0xE0000003,                   /**< attribute为ECC_DOMAINID */
	TEE_ATTR_ECC_HASHMODE = 0xE0000004,                   /**< attribute为ECC_HASHMODE */
};

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 *
 * object的类型，表明该object 中的key 是什么类型 \n
*/
enum TEE_ObjectType {
	TEE_TYPE_AES = 0xA0000010,                    /**< object中的key为AES类型 */
	TEE_TYPE_DES = 0xA0000011,                    /**< object中的key为DES类型 */
	TEE_TYPE_DES3 = 0xA0000013,                   /**< object中的key为DES3类型 */
	TEE_TYPE_HMAC_MD5 = 0xA0000001,               /**< object中的key为HMAC_MD5类型 */
	TEE_TYPE_HMAC_SHA1 = 0xA0000002,              /**< object中的key为HMAC_SHA1类型 */
	TEE_TYPE_HMAC_SHA224 = 0xA0000003,            /**< object中的key为HMAC_SHA224类型 */
	TEE_TYPE_HMAC_SHA256 = 0xA0000004,            /**< object中的key为HMAC_SHA256类型 */
	TEE_TYPE_HMAC_SHA384 = 0xA0000005,            /**< object中的key为HMAC_SHA384类型 */
	TEE_TYPE_HMAC_SHA512 = 0xA0000006,            /**< object中的key为HMAC_SHA512类型 */
	TEE_TYPE_RSA_PUBLIC_KEY = 0xA0000030,         /**< object中的key为RSA_PUBLIC_KEY类型 */
	TEE_TYPE_RSA_KEYPAIR = 0xA1000030,            /**< object中的key为RSA_KEYPAIR类型 */
	TEE_TYPE_DSA_PUBLIC_KEY = 0xA0000031,         /**< object中的key为DSA_PUBLIC_KEY类型 */
	TEE_TYPE_DSA_KEYPAIR = 0xA1000031,            /**< object中的key为DSA_KEYPAIR类型 */
	TEE_TYPE_DH_KEYPAIR = 0xA1000032,             /**< object中的key为DH_KEYPAIR类型 */
	TEE_TYPE_GENERIC_SECRET = 0xA0000000,         /**< object中的key为GENERIC_SECRET类型 */
	TEE_TYPE_DATA = 0xA1000033,                   /**< object没有key，为纯数据类型 */
	TEE_TYPE_ECC_PUBLIC_KEY = 0xA0000034,         /**< object中的key为RSA_PUBLIC_KEY类型 */
	TEE_TYPE_ECC_KEYPAIR = 0xA1000035,            /**< object中的key为RSA_KEYPAIR类型 */
	TEE_TYPE_ECC_PRIVATE_KEY = 0xA0000036,         /**< object中的key为DSA_PUBLIC_KEY类型 */

};

/**
* @ingroup  TEE_CommonDef
*
* TEE对象句柄结构体定义
*/
struct __TEE_ObjectHandle {
	void *dataPtr;                  /**< 对象的数据对象操作指针  */
	uint32_t dataLen;               /**< 对象的数据对象名称长度  */
	uint8_t dataName[255];          /**< 对象的数据对象名称  */
	TEE_ObjectInfo *ObjectInfo;     /**< 对象的信息数据指针  */
	TEE_Attribute *Attribute;       /**< 对象的属性指针  */
	uint32_t attributesLen;         /**< 对象的属性长度  */
	uint32_t CRTMode;               /**< 对象的CRT模式  */
	void *infoattrfd;               /**< 对象的信息指针  */
};
typedef struct __TEE_ObjectHandle *TEE_ObjectHandle;

typedef struct {
	uint32_t timeLow;
	uint16_t timeMid;
	uint16_t timeHiAndVersion;
	uint8_t clockSeqAndNode[8];
} TEE_UUID;

/**
* @ingroup  TEE_CommonDef
*
* 函数返回的错误码
*/
enum TEE_Result_Value {
	TEE_SUCCESS = 0x0,                          /**< 成功  */
	TEE_ERROR_INVALID_CMD,                      /**< 非法命令 */
	TEE_ERROR_SERVICE_NOT_EXIST,                /**< 服务不存在 */
	TEE_ERROR_SESSION_NOT_EXIST,                /**< 连接不存在 */
	TEE_ERROR_SESSION_MAXIMUM,                  /**< 连接数已满 */
	TEE_ERROR_REGISTER_EXIST_SERVICE,           /**< 注册已经存在的服务 */
	TEE_ERROR_TARGET_DEAD_FATAL,                 /**< Global Task 崩溃  */
	TEE_ERROR_READ_DATA,                        /**< 读取文件错误  */
	TEE_ERROR_WRITE_DATA,                       /**< 写入文件错误  */
	TEE_ERROR_TRUNCATE_OBJECT,                  /**< 截断文件错误  */
	TEE_ERROR_SEEK_DATA,                        /**< 查找文件错误  */
	TEE_ERROR_SYNC_DATA,                        /**< 同步文件错误  */
	TEE_ERROR_RENAME_OBJECT,                    /**< 重命名文件错误  */
	TEE_ERROR_TRUSTED_APP_LOAD_ERROR,           /**< 打开会话时，加载可信应用程序失败*/
	TEE_ERROR_STORAGE_EIO = 0x80001001,         /**<安全存储分区I/O错误*/
	TEE_ERROR_STORAGE_EAGAIN = 0x80001002,      /**<安全存储分区暂时不可用，请重试*/
	TEE_ERROR_STORAGE_ENOTDIR = 0x80001003,     /**<操作不是目录，无法操作*/
	TEE_ERROR_STORAGE_EISDIR = 0x80001004,      /**<操作对象是目录，无法操作*/
	TEE_ERROR_STORAGE_ENFILE = 0x80001005,      /**<对于系统来说，打开文件数已达到最大值*/
	TEE_ERROR_STORAGE_EMFILE = 0x80001006,      /**<对于进程来说，打开文件数已达到最大值*/
	TEE_ERROR_STORAGE_EROFS = 0x80001007,       /**<安全存储分区只读*/
	TEE_ERROR_CORRUPT_OBJECT = 0xF0100001,      /**<文件被损坏*/
	TEE_ERROR_STORAGE_NOT_AVAILABLE = 0xF0100003,   /**<安全存储分区不可用*/
	TEE_ISOCKET_ERROR_PROTOCOL = 0xF1007001,
	TEE_ISOCKET_ERROR_REMOTE_CLOSED = 0xF1007002,
	TEE_ISOCKET_ERROR_TIMEOUT = 0xF1007003,
	TEE_ISOCKET_ERROR_OUT_OF_RESOURCES = 0xF1007004,
	TEE_ISOCKET_ERROR_LARGE_BUFFER = 0xF1007005,
	TEE_ISOCKET_WARNING_PROTOCOL = 0xF1007006,
	TEE_ERROR_GENERIC = 0xFFFF0000,             /**< 通用错误  */
	TEE_ERROR_ACCESS_DENIED = 0xFFFF0001 ,      /**< 权限校验失败  */
	TEE_ERROR_CANCEL = 0xFFFF0002 ,             /**< 操作已取消  */
	TEE_ERROR_ACCESS_CONFLICT = 0xFFFF0003 ,    /**< 并发访问导致冲突  */
	TEE_ERROR_EXCESS_DATA = 0xFFFF0004 ,        /**< 操作包含的数据太多  */
	TEE_ERROR_BAD_FORMAT = 0xFFFF0005 ,         /**< 数据格式不正确  */
	TEE_ERROR_BAD_PARAMETERS = 0xFFFF0006 ,     /**< 参数无效  */
	TEE_ERROR_BAD_STATE = 0xFFFF0007,           /**< 当前状态下的操作无效  */
	TEE_ERROR_ITEM_NOT_FOUND = 0xFFFF0008,      /**< 请求的数据未找到  */
	TEE_ERROR_NOT_IMPLEMENTED = 0xFFFF0009,     /**< 请求的操作存在但暂未实现  */
	TEE_ERROR_NOT_SUPPORTED = 0xFFFF000A,       /**< 请求的操作有效但未支持  */
	TEE_ERROR_NO_DATA = 0xFFFF000B,             /**< 数据错误  */
	TEE_ERROR_OUT_OF_MEMORY = 0xFFFF000C,       /**< 系统没有可用资源  */
	TEE_ERROR_BUSY = 0xFFFF000D,                /**< 系统繁忙  */
	TEE_ERROR_COMMUNICATION = 0xFFFF000E,       /**< 与第三方通信失败  */
	TEE_ERROR_SECURITY = 0xFFFF000F,            /**< 检测到安全错误  */
	TEE_ERROR_SHORT_BUFFER = 0xFFFF0010,        /**< 内存输入长度小于输出长度  */
	TEE_ERROR_EXTERNAL_CANCEL = 0xFFFF0011,     /**< 操作被中断  */
	TEE_PENDING = 0xFFFF2000,                   /**< 可信服务处于等待状态(异步调用) */
	TEE_PENDING2 = 0xFFFF2001,                  /**< 可信服务处于等待状态2(命令未完成) */
	TEE_PENDING3 = 0xFFFF2002,
	TEE_ERROR_TIMEOUT = 0xFFFF3001,             /**< 请求超时 */
	TEE_ERROR_OVERFLOW = 0xFFFF300f,            /**< 计算溢出 */
	TEE_ERROR_TARGET_DEAD = 0xFFFF3024,          /**< Trusted Application崩溃  */
	TEE_ERROR_STORAGE_NO_SPACE = 0xFFFF3041,    /**< 没有足够的Flash空间来存储文件 */
	TEE_ERROR_MAC_INVALID = 0xFFFF3071,         /**< MAC值校验错误  */
	TEE_ERROR_SIGNATURE_INVALID = 0xFFFF3072,   /**< 校验失败 */
	TEE_ERROR_TIME_NOT_SET = 0xFFFF5000,        /**< 时间未设置 */
	TEE_ERROR_TIME_NEEDS_RESET = 0xFFFF5001,    /**< 时间需要重置 */
	TEE_FAIL = 0xFFFF5002,    /**< 时间需要重置 */
	TEE_ERROR_TIMER = 0xFFFF6000,
	TEE_ERROR_TIMER_CREATE_FAILED,
	TEE_ERROR_TIMER_DESTORY_FAILED,
	TEE_ERROR_TIMER_NOT_FOUND,
	TEE_ERROR_RPMB_BASE = 0xFFFF7000,           /**< RPMB安全存储错误码基址 */
	TEE_ERROR_RPMB_GENERIC = 0xFFFF7001,        /**< RPMB控制器通用错误 */
	TEE_ERROR_RPMB_MAC_FAIL,                    /**< RPMB控制器MAC校验错误 */
	TEE_ERROR_RPMB_COUNTER_FAIL,                /**< RPMB控制器计数器校验错误 */
	TEE_ERROR_RPMB_ADDR_FAIL,                   /**< RPMB控制器地址校验错误 */
	TEE_ERROR_RPMB_WRITE_FAIL,                  /**< RPMB控制器写错误 */
	TEE_ERROR_RPMB_READ_FAIL,                   /**< RPMB控制器读错误 */
	TEE_ERROR_RPMB_KEY_NOT_PROGRAM,             /**< RPMB Key未写入 */
	TEE_ERROR_RPMB_RESP_UNEXPECT_MSGTYPE = 0xFFFF7100,    /**< RPMB应答数据的消息类型校验错误 */
	TEE_ERROR_RPMB_RESP_UNEXPECT_BLKCNT,        /**< RPMB应答数据的数据块校验错误 */
	TEE_ERROR_RPMB_RESP_UNEXPECT_BLKIDX,        /**< RPMB应答数据的数据地址校验错误 */
	TEE_ERROR_RPMB_RESP_UNEXPECT_WRCNT,         /**< RPMB应答数据的计数器校验错误 */
	TEE_ERROR_RPMB_RESP_UNEXPECT_NONCE,         /**< RPMB应答数据的随机数校验错误 */
	TEE_ERROR_RPMB_RESP_UNEXPECT_MAC,           /**< RPMB应答数据的MAC校验错误 */
	TEE_ERROR_RPMB_FILE_NOT_FOUND,              /**< RPMB安全存储文件不存在 */
	TEE_ERROR_RPMB_NOSPC,                       /**< RPMB安全存储磁盘空间不足 */
	TEE_ERROR_RPMB_SPC_CONFLICT,                /**< RPMB安全存储磁盘空间用量已达TA上限 */
	TEE_ERROR_TUI_IN_USE = 0xFFFF7110,
	TEE_ERROR_TUI_SWITCH_CHANNAL,
	TEE_ERROR_TUI_CFG_DRIVER,
	TEE_ERROR_TUI_INVALID_EVENT,
	TEE_ERROR_TUI_POLL_EVENT,
	TEE_ERROR_TUI_CANCELED,
	TEE_ERROR_ANTIROOT_RSP_FAIL = 0xFFFF9110,  /** < AntiRoot Response verify failed. */
	TEE_ERROR_ANTIROOT_INVOKE_ERROR = 0xFFFF9111,  /** < AntiRoot ERROR during invokecmd . */
	TEE_FAIL2
};

/**
 * @ingroup  TEE_CommonDef
 *
 * Login方式
 */
enum TEE_LoginMethod {
	TEE_LOGIN_PUBLIC = 0x0,             /**< 不需要Login数据  */
	TEE_LOGIN_USER,                     /**< 提供用户运行非安全应用程序的Login数据  */
	TEE_LOGIN_GROUP,                    /**< 提供组用户运行非安全应用程序的Login数据  */
	TEE_LOGIN_APPLICATION = 0x4,        /**< 提供非安全应用程序自己的Login数据  */
	TEE_LOGIN_USER_APPLICATION = 0x5,   /**< 提供用户运行非安全应用程序的Login数据，
                                             以及非安全应用程序自己的Login数据*/
	TEE_LOGIN_GROUP_APPLICATION = 0x6,  /**< 提供组用户运行非安全应用程序的Login数据，
                                             以及非安全应用程序自己的Login数据*/
	TEE_LOGIN_IDENTIFY = 0x7,           /**< 调用安全存储要用  */
};

/**
 * @ingroup  TEE_CommonDef
 *
 * Client的身份标识
 */
typedef struct {
	uint32_t login;     /**< login的方式 */
	TEE_UUID uuid;      /**< Client的UUID */
} TEE_Identity;

/**
 * @ingroup TEE_CommonDef
 * 函数返回值类型定义
 *
 * 用于表示函数返回结果
 */
typedef enum TEE_Result_Value   TEE_Result;
typedef TEE_Result  TEEC_Result;

#define TEE_ORIGIN_TEE  0x00000003
#define TEE_ORIGIN_TRUSTED_APP 0x00000004

#ifndef _TEE_TASessionHandle
#define _TEE_TASessionHandle
typedef uint32_t TEE_TASessionHandle;
#endif

#include "tee_crypto_api.h"
#include "tee_mem_mgmt_api.h"
#include "tee_property_api.h"
#include "tee_time_api.h"
#include "tee_trusted_storage_api.h"
#include "tee_core_api.h"
#include "tee_internal_se_api.h"

#endif
