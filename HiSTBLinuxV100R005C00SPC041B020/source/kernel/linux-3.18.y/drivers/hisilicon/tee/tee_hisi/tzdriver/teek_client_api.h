/**
 * @file tee_client_api.h
 *
 * Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：定义非安全客户端接口\n
 */

/** @defgroup TEEC_API 客户端(非安全侧)接口
 */
/**@defgroup TEEC_BASIC_FUNC 通用接口
 *@ingroup TEEC_API
*/

#ifndef _TEE_CLIENT_API_H_
#define _TEE_CLIENT_API_H_

#include "teek_client_type.h"

/**
 * @ingroup TEEC_BASIC_FUNC
 * 用于计算非安全世界与安全世界传递参数的数值
 */
#define TEEC_PARAM_TYPES(param0Type, param1Type, param2Type, param3Type) \
	((param3Type) << 12 | (param2Type) << 8 | \
	 (param1Type) << 4 | (param0Type))

/**
 * @ingroup TEEC_BASIC_FUNC
 * 用于计算paramTypes中字段index的数值
 */
#define TEEC_PARAM_TYPE_GET(paramTypes, index) \
	(((paramTypes) >> (4*(index))) & 0x0F)

/**
 * @ingroup TEEC_BASIC_FUNC
 * 当参数类型为#TEEC_Value时，如果成员变量a或b没有给定值，需赋予此值，
 * 表示没有用到此成员变量
 */
#define TEEC_VALUE_UNDEF 0xFFFFFFFF
#ifdef CONFIG_DEVCHIP_PLATFORM
/**
 * @ingroup TEEC_BASIC_FUNC
 * 调试接口，在定义宏TEEC_DEBUG情况下有效，宏TEEC_DEBUG是调试打印开关
 */
#ifdef TEEC_DEBUG
#define TEEC_Debug(fmt, args...) printk(KERN_DEBUG "%s(%i, %s): " fmt "\n", \
		__func__, current->pid, current->comm, ## args)
#else
#define TEEC_Debug(fmt, args...)
#endif

/**
 * @ingroup TEEC_BASIC_FUNC
 * 调试接口，API函数内部打印错误信息
 */
#define TEEC_Error(fmt, args...) printk(KERN_ERR "%s(%i, %s): " fmt "\n", \
		__func__, current->pid, current->comm, ## args)
#define TEEC_Info(fmt, args...) printk(KERN_INFO "%s(%i, %s): " fmt "\n", \
		__func__, current->pid, current->comm, ## args)
#endif
/*
 * Function:      TEEK_IsAgentAlive
 * Description:   This function check if the special agent is launched.
 *              Used For HDCP key.
 *                e.g. If sfs agent is not alive,
 *                you can not do HDCP key write to SRAM.
 * Parameters:   agent_id.
 * Return:      1:agent is alive
 *              0:agent not exsit.
 */
int TEEK_IsAgentAlive(unsigned int agent_id);
/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief 初始化TEE环境
 *
 * @par 描述:
 * 初始化路径为name的TEE环境，参数name可以为空，
 * 初始化TEE环境是打开会话、发送命令的基础，
 * 初始化成功后，客户端应用与TEE建立一条链接。
 *
 * @attention 无
 * @param name [IN] TEE环境路径
 * @param context [IN/OUT] context指针，安全世界环境句柄
 *
 * @retval #TEEC_SUCCESS 初始化TEE环境成功
 * @retval #TEEC_ERROR_BAD_PARAMETERS 参数不正确，name不正确或context为空
 * @retval #TEEC_ERROR_GENERIC 系统可用资源不足等原因
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_api.h：该接口声明所在头文件
 * @see TEEC_FinalizeContext
 * @since V100R002C00B301
 */
TEEC_Result TEEK_InitializeContext(const char *name, TEEC_Context *context);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief 关闭TEE环境
 *
 * @par 描述:
 * 关闭context指向的TEE环境，断开客户端应用与TEE环境的链接
 *
 * @attention 无
 * @param context [IN/OUT] 指向已初始化成功的TEE环境
 *
 * @retval 无
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_api.h：该接口声明所在头文件
 * @see TEEC_InitializeContext
 * @since V100R002C00B301
 */
void TEEK_FinalizeContext(TEEC_Context *context);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief 打开会话
 *
 * @par 描述:
 * 在指定的TEE环境context下，为客户端应用与UUID
 * 为destination的安全服务建立一条链接，
 * 链接方式是connectionMethod，链接数据是connectionData,
 * 传递的数据包含在opetation里。
 * 打开会话成功后，输出参数session是对该链接的一个描述；
 * 如果打开会话失败，输出参数returnOrigin为错误来源。
 *
 * @attention 无
 * @param context [IN/OUT] 指向已初始化成功的TEE环境
 * @param session [OUT] 指向会话，取值不能为空
 * @param destination [IN] 安全服务的UUID，一个安全服务拥有唯一的UUID
 * @param connectionMethod [IN] 链接方式，取值范围为#TEEC_LoginMethod
 * @param connectionData [IN] 与链接方式相对应的链接数据，
 * 如果链接方式为#TEEC_LOGIN_PUBLIC, #TEEC_LOGIN_USE,
 * #TEEC_LOGIN_USER_APPLICATION,
 * #TEEC_LOGIN_GROUP_APPLICATION, 链接数据取值必须为空，
 * 如果链接方式为#TEEC_LOGIN_GROUP、#TEEC_LOGIN_GROUP_APPLICATION，
 * 链接数据必须指向类型为uint32_t的数据，此数据表示客户端应用期望链接的组用户
 * @param operation [IN/OUT] 客户端应用与安全服务传递的数据
 * @param returnOrigin [IN/OUT] 错误来源，取值范围为#TEEC_ReturnCodeOrigin
 *
 * @retval #TEEC_SUCCESS 打开会话成功
 * @retval #TEEC_ERROR_BAD_PARAMETERS 参数不正确，参数context为空或session为空或destination为空
 * @retval #TEEC_ERROR_ACCESS_DENIED 系统调用权限访问失败
 * @retval #TEEC_ERROR_OUT_OF_MEMORY 系统可用资源不足
 * @retval #TEEC_ERROR_TRUSTED_APP_LOAD_ERROR 加载安全服务失败
 * @retval 其它返回值参考 #TEEC_ReturnCode
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_api.h：该接口声明所在头文件
 * @see TEEC_CloseSession
 * @since V100R002C00B301
 */
TEEC_Result TEEK_OpenSession(TEEC_Context *context,
			     TEEC_Session *session,
			     const TEEC_UUID *destination,
			     uint32_t connectionMethod,
			     const void *connectionData,
			     TEEC_Operation *operation,
			     uint32_t *returnOrigin);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief 关闭会话
 *
 * @par 描述:
 * 关闭session指向的会话，断开客户端应用与安全服务的链接
 *
 * @attention 无
 * @param session [IN/OUT] 指向已成功打开的会话
 *
 * @retval 无
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_api.h：该接口声明所在头文件
 * @see TEEC_OpenSession
 * @since V100R002C00B301
 */
void TEEK_CloseSession(TEEC_Session *session);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief 发送命令
 *
 * @par 描述:
 * 在指定的会话session里，由客户端应用向安全服务发送命令commandID，
 * 发送的数据为operation，如果发送命令失败，输出参数returnOrigin为错误来源
 *
 * @attention 无
 * @param session [IN/OUT] 指向已打开成功的会话
 * @param commandID [IN] 安全服务支持的命令ID，由安全服务定义
 * @param operation [IN/OUT] 包含了客户端应用向安全服务发送的数据内容
 * @param returnOrigin [IN/OUT] 错误来源，取值范围为#TEEC_ReturnCodeOrigin
 *
 * @retval #TEEC_SUCCESS 发送命令成功
 * @retval #TEEC_ERROR_BAD_PARAMETERS 参数不正确，参数session为空或参数operation格式不正确
 * @retval #TEEC_ERROR_ACCESS_DENIED 系统调用权限访问失败
 * @retval #TEEC_ERROR_OUT_OF_MEMORY 系统可用资源不足
 * @retval 其它返回值参考 #TEEC_ReturnCode
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_api.h：该接口声明所在头文件
 * @see 无
 * @since V100R002C00B301
 */
TEEC_Result TEEK_InvokeCommand(TEEC_Session *session,
			       uint32_t commandID,
			       TEEC_Operation *operation,
			       uint32_t *returnOrigin);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief 注册共享内存
 *
 * @par 描述:
 * 在指定的TEE环境context内注册共享内存sharedMem，
 * 通过注册的方式获取共享内存来实现零拷贝需要操作系统的支持，
 * 目前的实现中，该方式不能实现零拷贝
 *
 * @attention 如果入参sharedMem的size域设置为0，函数会返回成功，但无法使用这块
 * 共享内存，因为这块内存没有大小
 * @param context [IN/OUT] 已初始化成功的TEE环境
 * @param sharedMem [IN/OUT] 共享内存指针，共享内存所指向的内存不能为空、大小不能为零
 *
 * @retval #TEEC_SUCCESS 发送命令成功
 * @retval #TEEC_ERROR_BAD_PARAMETERS 参数不正确，参数context为空或sharedMem为空，
 * 或共享内存所指向的内存为空
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_api.h：该接口声明所在头文件
 * @see TEEC_AllocateSharedMemory
 * @since V100R002C00B301
 */
TEEC_Result TEEK_RegisterSharedMemory(TEEC_Context *context,
				      TEEC_SharedMemory *sharedMem);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief 申请共享内存
 *
 * @par 描述:
 * 在指定的TEE环境context内申请共享内存sharedMem，
 * 通过共享内存可以实现非安全世界与安全世界传递数据时的零拷贝
 *
 * @attention 如果入参sharedMem的size域设置为0，函数会返回成功，但无法使用这块
 * 共享内存，因为这块内存既没有地址也没有大小
 * @param context [IN/OUT] 已初始化成功的TEE环境
 * @param sharedMem [IN/OUT] 共享内存指针，共享内存的大小不能为零
 *
 * @retval #TEEC_SUCCESS 发送命令成功
 * @retval #TEEC_ERROR_BAD_PARAMETERS 参数不正确，参数context为空或sharedMem为空
 * @retval #TEEC_ERROR_OUT_OF_MEMORY 系统可用资源不足，分配失败
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_api.h：该接口声明所在头文件
 * @see TEEC_RegisterSharedMemory
 * @since V100R002C00B301
 */
TEEC_Result TEEK_AllocateSharedMemory(TEEC_Context *context,
				      TEEC_SharedMemory *sharedMem);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief 释放共享内存
 *
 * @par 描述:
 * 释放已注册成功的的或已申请成功的共享内存sharedMem
 *
 * @attention 如果是通过#TEEC_AllocateSharedMemory方式获取的共享内存，
 * 释放时会回收这块内存；如果是通过#TEEC_RegisterSharedMemory方式
 * 获取的共享内存，释放时不会回收共享内存所指向的本地内存
 * @param sharedMem [IN/OUT] 指向已注册成功或申请成功的共享内存
 *
 * @retval 无
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_api.h：该接口声明所在头文件
 * @see TEEC_RegisterSharedMemory | TEEC_AllocateSharedMemory
 * @since V100R002C00B301
 */
void TEEK_ReleaseSharedMemory(TEEC_SharedMemory *sharedMem);

/**
 * @ingroup  TEEC_BASIC_FUNC
 * @brief cancel API
 *
 * @par 描述:
 * 取消掉一个正在运行的open Session或者是一个invoke command
 * 发送一个cancel的signal后立即返回
 *
 * @attention 此操作仅仅是发送一个cancel的消息，是否进行cancel操作由前面的TEE 或 TA决定
 * @param operation [IN/OUT] 包含了客户端应用向安全服务发送的数据内容
 *
 * @retval 无
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_api.h：该接口声明所在头文件
 * @see 无
 * @since V100R002C00B309
 */
void TEEK_RequestCancellation(TEEC_Operation *operation);

#endif
/**
 * History: \n
 * 2013-4-12 h00211444: Create this file\n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
*/
