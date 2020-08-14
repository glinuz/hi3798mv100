/**
 * @file tee_client_type.h
 *
 * Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：定义基本数据类型和数据结构\n
 */

#ifndef _TEE_CLIENT_TYPE_H_
#define _TEE_CLIENT_TYPE_H_

#include "tee_client_constants.h"

#if 0
/**
 * @ingroup TEEC_COMMON_DATA
 * 无符号整型定义
 */
typedef unsigned int      uint32_t;
/**
 * @ingroup TEEC_COMMON_DATA
 * 有符号整型定义
 */
typedef signed int        int32_t;
/**
 * @ingroup TEEC_COMMON_DATA
 * 无符号短整型定义
 */
typedef unsigned short    uint16_t;
/**
 * @ingroup TEEC_COMMON_DATA
 * 有符号短整型定义
 */
typedef signed short      int16_t;
/**
 * @ingroup TEEC_COMMON_DATA
 * 无符号字符型定义
 */
typedef unsigned char     uint8_t;
/**
 * @ingroup TEEC_COMMON_DATA
 * 有符号字符型定义
 */
typedef signed char       int8_t;
/**
 * @ingroup TEEC_COMMON_DATA
 * 数据长度类型定义
 */
typedef uint32_t	size_t;
#endif

/**
 * @ingroup TEEC_COMMON_DATA
 * 布尔类型定义
 */
#ifndef bool
#define bool    uint8_t
#endif

/**
 * @ingroup TEEC_COMMON_DATA
 * true值的定义
 */
#ifndef true
#define true    1
#endif

/**
 * @ingroup TEEC_COMMON_DATA
 * false值的定义
 */
#ifndef false
#define false   0
#endif

/**
 * @ingroup TEEC_COMMON_DATA
 * NULL值的定义
 */
#ifndef NULL
#define NULL 0
#endif

#include "tee_client_list.h"

/**
 * @ingroup TEEC_COMMON_DATA
 * 函数返回值类型定义
 *
 * 用于表示函数返回结果
 */
typedef uint32_t TEEC_Result;

/**
 * @ingroup TEEC_COMMON_DATA
 * UUID类型定义
 *
 * UUID类型遵循RFC4122 [2]，用于标识安全服务
 */
typedef struct {
    uint32_t timeLow;  /**< 时间戳的低4字节  */
    uint16_t timeMid;  /**< 时间戳的中2字节  */
    uint16_t timeHiAndVersion;  /**< 时间戳的高2字节与版本号的组合  */
    uint8_t clockSeqAndNode[8];  /**< 时钟序列与节点标识符的组合  */
} TEEC_UUID;

/**
 * @ingroup TEEC_COMMON_DATA
 * TEEC_Context结构体类型定义
 *
 * 描述客户端应用与安全世界之间建立的链接环境
 */
typedef struct {
    uint32_t fd;  /**< 文件描述符  */
    struct list_node session_list;  /**< 会话链表  */
    struct list_node shrd_mem_list;  /**< 共享内存链表  */
} TEEC_Context;

/**
 * @ingroup TEEC_COMMON_DATA
 * TEEC_Session结构体类型定义
 *
 * 描述客户端应用与安全世界之间建立的会话
 */
typedef struct {
    uint32_t session_id;  /**< 会话ID，由安全世界返回  */
    TEEC_UUID service_id;  /**< 安全服务的UUID，每个安全服务拥有唯一的UUID  */
    uint32_t ops_cnt;  /**< 在会话内的操作数  */
    struct list_node head;  /**< 会话链表头  */
    TEEC_Context* context;  /**< 指向会话所属的TEE环境  */
} TEEC_Session;

/**
 * @ingroup TEEC_COMMON_DATA
 * TEEC_SharedMemory结构体类型定义
 *
 * 描述一块共享内存，可以注册，也可以分配
 */
typedef struct {
    void* buffer;  /**< 内存指针  */
    size_t size;  /**< 内存大小  */
    uint32_t flags;  /**< 内存标识符，用于区别输入或输出，取值范围为#TEEC_SharedMemCtl  */
    uint32_t ops_cnt;  /**< 内存操作数  */
    bool is_allocated;  /**< 内存分配标示符，用于区别是注册的，还是分配的  */
    struct list_node head;  /**< 共享内存链表头  */
    TEEC_Context* context;  /**< 指向所属的TEE环境 */
} TEEC_SharedMemory;

/**
 * @ingroup TEEC_COMMON_DATA
 * TEEC_TempMemoryReference结构体类型定义
 *
 * 描述一块临时缓冲区指针\n
 * 可以用于#TEEC_Parameter的类型，与其相对应的类型可以是
 * #TEEC_MEMREF_TEMP_INPUT， #TEEC_MEMREF_TEMP_OUTPUT，或#TEEC_MEMREF_TEMP_INOUT
 */
typedef struct {
    void* buffer;  /**< 临时缓冲区指针  */
    size_t size;  /**< 临时缓冲区大小  */
} TEEC_TempMemoryReference;

/**
 * @ingroup TEEC_COMMON_DATA
 * TEEC_RegisteredMemoryReference结构体类型定义
 *
 * 描述共享内存指针，指向事先注册或分配好的共享内存\n
 * 可以用于#TEEC_Parameter的类型，与其相对应的类型可以是
 * #TEEC_MEMREF_WHOLE， #TEEC_MEMREF_PARTIAL_INPUT，
 * #TEEC_MEMREF_PARTIAL_OUTPUT，或 #TEEC_MEMREF_PARTIAL_INOUT
 */
typedef struct {
    TEEC_SharedMemory* parent;  /**< 共享内存指针  */
    size_t size;  /**< 共享内存的使用大小  */
    size_t offset;  /**< 共享内存的使用偏移  */
} TEEC_RegisteredMemoryReference;

/**
 * @ingroup TEEC_COMMON_DATA
 * TEEC_Value结构体类型定义
 *
 * 描述少量的数据\n
 * 可以用于#TEEC_Parameter的类型，与其相对应的类型可以是
 * #TEEC_VALUE_INPUT， #TEEC_VALUE_OUTPUT，或#TEEC_VALUE_INOUT
 */
typedef struct {
    uint32_t a;  /**< 整型数据a  */
    uint32_t b;  /**< 整型数据b  */
} TEEC_Value;

/**
 * @ingroup TEEC_COMMON_DATA
 * TEEC_Parameter联合体类型定义
 *
 * 描述#TEEC_Operation所对应的参数类型
 */
typedef union {
    TEEC_TempMemoryReference tmpref;  /**< 描述#TEEC_TempMemoryReference类型  */
    TEEC_RegisteredMemoryReference memref;  /**< 描述#TEEC_RegisteredMemoryReference类型  */
    TEEC_Value value;  /**< 描述#TEEC_Value类型  */
} TEEC_Parameter;

/**
 * @ingroup TEEC_COMMON_DATA
 * TEEC_Operation结构体类型定义
 *
 * 打开会话或发送命令时的参数可以通过此类型描述，
 * 取消操作也可以使用此参数
 */
typedef struct {
    uint32_t started;  /**< 标识是否是取消操作，0表示取消操作  */
    uint32_t paramTypes;  /**描述params的参数类型#TEEC_ParamType，需要使用宏#TEEC_PARAM_TYPES组合参数类型 */
    TEEC_Parameter params[4];  /**< 参数内容，类型为#TEEC_Parameter  */
} TEEC_Operation;

#endif
/**
 * History: \n
 * 2013-4-12 h00211444: Create this file\n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
*/
