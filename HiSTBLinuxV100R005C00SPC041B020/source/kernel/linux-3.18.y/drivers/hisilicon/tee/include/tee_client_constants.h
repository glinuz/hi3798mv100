/**
 * @file tee_client_constants.h
 *
 * Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：定义静态变量\n
 */

/** @defgroup TEEC_COMMON_DATA 公共数据定义
 *@ingroup TEEC_API
 */

#ifndef _TEE_CLIENT_CONSTANTS_H_
#define _TEE_CLIENT_CONSTANTS_H_

#if 0
enum global_service_cmd_id {
    GLOBAL_CMD_ID_INVALID = 0x0,
    GLOBAL_CMD_ID_BOOT_ACK,
    GLOBAL_CMD_ID_OPEN_SESSION,
    GLOBAL_CMD_ID_CLOSE_SESSION,
    GLOBAL_CMD_ID_LOAD_SECURE_APP,  /**< Global Task 动态加载安全应用*/
    GLOBAL_CMD_ID_NEED_LOAD_APP,    /**< Global Task 判断是否是需要加载安全应用*/
    GLOBAL_CMD_ID_REGISTER_AGENT,
    GLOBAL_CMD_ID_UNREGISTER_AGENT,
    GLOBAL_CMD_ID_REGISTER_NOTIFY_MEMORY,
    GLOBAL_CMD_ID_UNREGISTER_NOTIFY_MEMORY,
    GLOBAL_CMD_ID_INIT_CONTENT_PATH,        /**< Global Task初始化content path*/
    GLOBAL_CMD_ID_TERMINATE_CONTENT_PATH,   /**< Global Task释放content path*/
    GLOBAL_CMD_ID_ALLOC_EXCEPTION_MEM,
    GLOBAL_CMD_ID_ALLOC_SMC_MEM,
    GLOBAL_CMD_ID_FREE_SMC_MEM,
    GLOBAL_CMD_ID_TEE_TIME,
    GLOBAL_CMD_ID_UNKNOWN         = 0x7FFFFFFE,
    GLOBAL_CMD_ID_MAX             = 0x7FFFFFFF
};
#endif
/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 函数返回的错误码
 */
enum TEEC_ReturnCode {
    TEEC_SUCCESS = 0x0,  /**< 函数返回成功  */
    TEEC_ERROR_INVALID_CMD,                      /**< 非法命令，安全服务不支持的命令 */
    TEEC_ERROR_SERVICE_NOT_EXIST,                /**< 安全服务不存在 */
    TEEC_ERROR_SESSION_NOT_EXIST,                /**< 客户端应用与安全服务的链接不存在 */
    TEEC_ERROR_SESSION_MAXIMUM,                  /**< 安全服务的链接数已满 */
    TEEC_ERROR_REGISTER_EXIST_SERVICE,           /**< 注册已经存在的安全服务 */
    TEEC_ERROR_TAGET_DEAD_FATAL,                 /**< 安全服务Global出错 ，(安全服务Global是所有安全服务的基础) */
    TEEC_ERROR_READ_DATA,                        /**< 读取文件错误  */
    TEEC_ERROR_WRITE_DATA,                       /**< 写入文件错误  */
    TEEC_ERROR_TRUNCATE_OBJECT,                  /**< 截断文件错误  */
    TEEC_ERROR_SEEK_DATA,                        /**< 查找文件错误  */
    TEEC_ERROR_RENAME_OBJECT,                    /**< 重命名文件错误  */
    TEEC_ERROR_TRUSTED_APP_LOAD_ERROR,           /**< 打开会话时，加载安全服务失败*/
    TEEC_ERROR_GENERIC = 0xFFFF0000,  /**< 通用错误，初始化安全服务失败  */
    TEEC_ERROR_ACCESS_DENIED = 0xFFFF0001 ,  /**< 权限校验失败，
            打开TEE环境、打开会话和发送命令都会进行权限的校验，
            权限校验不通过会返回此类错误  */
    TEEC_ERROR_CANCEL = 0xFFFF0002 ,  /**< 操作已取消，
            如果参数的取消标志位已置位，再对此参数进行操作时返回此类错误 */
    TEEC_ERROR_ACCESS_CONFLICT = 0xFFFF0003 ,  /**< 并发访问导致权限冲突，
            安全存储服务中对文件的并发访问可能会产生此类错误  */
    TEEC_ERROR_EXCESS_DATA = 0xFFFF0004 ,  /**< 操作包含的数据太多 ，安全服务无法解析 */
    TEEC_ERROR_BAD_FORMAT = 0xFFFF0005 ,  /**< 数据格式不正确，
            客户端应用填充的参数格式不满足客户端应用与安全服务的通信协议，
            安全应用无法解析  */
    TEEC_ERROR_BAD_PARAMETERS = 0xFFFF0006 ,  /**< 参数无效，入参为空或非法等错误  */
    TEEC_ERROR_BAD_STATE = 0xFFFF0007,  /**< 当前状态下的操作无效，
            请求安全存储服务操作时，如果没有初始化安全存储服务，会返回此类错误*/
    TEEC_ERROR_ITEM_NOT_FOUND = 0xFFFF0008,  /**< 请求的数据未找到  */
    TEEC_ERROR_NOT_IMPLEMENTED = 0xFFFF0009,  /**< 请求的操作存在但暂未实现，
            请求取消操作时返回此类错误  */
    TEEC_ERROR_NOT_SUPPORTED = 0xFFFF000A,  /**< 请求的操作有效但未支持，
            请求安全加解密服务的一些算法如DSA等时返回此类错误  */
    TEEC_ERROR_NO_DATA = 0xFFFF000B,  /**< 数据错误 ，请求的操作找不到对应的数据 */
    TEEC_ERROR_OUT_OF_MEMORY = 0xFFFF000C,  /**< 系统可用资源不足，
            内存申请失败会返回此类错误  */
    TEEC_ERROR_BUSY = 0xFFFF000D,  /**< 系统繁忙，系统可能正在独占一些资源  */
    TEEC_ERROR_COMMUNICATION = 0xFFFF000E,  /**< 非安全世界应用程序与
            安全应用通信时发生错误  */
    TEEC_ERROR_SECURITY = 0xFFFF000F,  /**< 检测到安全错误，安全世界发生错误  */
    TEEC_ERROR_SHORT_BUFFER = 0xFFFF0010,  /**< 内存输入长度小于输出长度，
            使用类型为#TEEC_MEMREF_TEMP_OUTPUT时需要注意此类错误  */
    TEEC_PENDING = 0xFFFF2000,
    TEEC_PENDING2 = 0xFFFF2001,
    TEE_ERROR_TAGET_DEAD = 0xFFFF3024,
    TEE_ERROR_GT_DEAD = 0xFFFF3124,
    TEEC_ERROR_MAC_INVALID = 0xFFFF3071,  /**< MAC值校验错误  */
};

/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 函数返回错误码的来源
 */
enum TEEC_ReturnCodeOrigin {
    TEEC_ORIGIN_API = 0x1,  /**< 错误码来自客户端API  */
    TEEC_ORIGIN_COMMS = 0x2,  /**< 错误码来自非安全世界与安全世界的通信  */
    TEEC_ORIGIN_TEE = 0x3,  /**< 错误码来自安全世界  */
    TEEC_ORIGIN_TRUSTED_APP = 0x4,  /**< 错误码来自安全服务  */
};

/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 共享内存标识
 */
enum TEEC_SharedMemCtl {
    TEEC_MEM_INPUT = 0x1,  /**< 共享内存的数据流是从客户端应用到安全服务  */
    TEEC_MEM_OUTPUT = 0x2,  /**< 共享内存的数据流是从安全服务到客户端应用  */
    TEEC_MEM_INOUT = 0x3,  /**< 共享内存可在客户端应用与安全服务之间双向传递  */
};

/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * 参数类型定义
 */
enum TEEC_ParamType {
    TEEC_NONE = 0x0,  /**< 参数没有使用  */
    TEEC_VALUE_INPUT = 0x01,  /**< 与类型#TEEC_Value相对应，只能作为输入，
    数据流是从客户端应用到安全服务*/
    TEEC_VALUE_OUTPUT = 0x02,  /**< 与类型#TEEC_Value相对应，只能作为输出，
    数据流是从安全服务到客户端应用*/
    TEEC_VALUE_INOUT = 0x03,  /**< 与类型#TEEC_Value相对应，既可输入也可输出  */
    TEEC_MEMREF_TEMP_INPUT = 0x05,  /**< 与类型#TEEC_TempMemoryReference相对应，
    只能作为输入，数据流是从客户端应用到安全服务  */
    TEEC_MEMREF_TEMP_OUTPUT = 0x06,  /**< 与类型#TEEC_TempMemoryReference相对应，
    只能作为输出，数据流是从安全服务到客户端应用  */
    TEEC_MEMREF_TEMP_INOUT = 0x07,  /**< 与类型#TEEC_TempMemoryReference相对应，
    既可输入也可输出，可在客户端应用与安全服务之间双向传递*/
	TEEC_DX_SHARE_MEM = 0x08,
    TEEC_MEMREF_WHOLE = 0xc,  /**< 与类型#TEEC_RegisteredMemoryReference
    相对应，引用整块内存 ，数据流与所指向的共享内存的标识#TEEC_SharedMemCtl一致 */
    TEEC_MEMREF_PARTIAL_INPUT = 0xd,  /**< 与类型#TEEC_RegisteredMemoryReference相对应，
    只能作为输入，数据流是从客户端应用到安全服务 */
    TEEC_MEMREF_PARTIAL_OUTPUT = 0xe,  /**< 与类型#TEEC_RegisteredMemoryReference
    相对应，只能作为输出，数据流是从安全服务到客户端应用 */
    TEEC_MEMREF_PARTIAL_INOUT = 0xf  /**< 与类型#TEEC_RegisteredMemoryReference相对应，
    既可输入也可输出，可在客户端应用与安全服务之间双向传递*/
};
enum TEE_ParamType {
    TEE_PARAM_TYPE_NONE = 0x0,
    TEE_PARAM_TYPE_VALUE_INPUT = 0x1,
    TEE_PARAM_TYPE_VALUE_OUTPUT = 0x2,
    TEE_PARAM_TYPE_VALUE_INOUT = 0x3,
    TEE_PARAM_TYPE_MEMREF_INPUT = 0x5,
    TEE_PARAM_TYPE_MEMREF_OUTPUT = 0x6,
    TEE_PARAM_TYPE_MEMREF_INOUT = 0x7,
};

/****************************************************
 *		Session Login Methods
 ****************************************************/
/**
 * @ingroup  TEEC_COMMON_DATA
 *
 * Login方式
 */
enum TEEC_LoginMethod {
    TEEC_LOGIN_PUBLIC = 0x0,  /**< 不需要Login数据  */
    TEEC_LOGIN_USER,  /**< 提供用户运行客户端应用的Login数据  */
    TEEC_LOGIN_GROUP,  /**< 提供组用户运行客户端应用的Login数据  */
    TEEC_LOGIN_APPLICATION = 0x4,  /**< 提供客户端应用自己的Login数据  */
    TEEC_LOGIN_USER_APPLICATION = 0x5,  /**< 提供用户运行客户端应用
                      的Login数据，以及客户端应用自己的Login数据*/
    TEEC_LOGIN_GROUP_APPLICATION = 0x6,  /**< 提供组用户运行客户端应用
                      的Login数据，以及客户端应用自己的Login数据*/
    TEEC_LOGIN_IDENTIFY = 0x7,  /**< 使用安全存储时需要提供Login数据  */
};

#endif
/**
 * History: \n
 * 2013-4-12 h00211444: Create this file\n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
*/
