/**
 * @file tee_client_storage_api.h
 *
 * Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：定义安全存储接口\n
 */

/** @defgroup TEEC_StorageAPI 安全存储服务接口
 *@ingroup TEEC_API
 */

#ifndef _TEE_CLIENT_STORAGE_API_H_
#define _TEE_CLIENT_STORAGE_API_H_

#include "tee_client_type.h"

#define SFS_PROPERTY "trustedcore_sfs_property"
#define SFS_PROPERTY_VALUE "1"
/**
 * @ingroup  TEEC_StorageAPI
 *
 * 打开文件的模式
 */
enum Data_Flag_Constants {
	TEE_DATA_FLAG_ACCESS_READ = 0x00000001,  /**< 只读模式 */
	TEE_DATA_FLAG_ACCESS_WRITE = 0x00000002,  /**< 只写模式 */
	TEE_DATA_FLAG_ACCESS_WRITE_META = 0x00000004,  /**< 如果对文件进行删除或改名操作，
    必须使用此模式先打开文件 */
	TEE_DATA_FLAG_SHARE_READ = 0x00000010,  /**< 共享读模式 ，
    只有在此模式下，才可以以读方式同时打开同一个文件 */
	TEE_DATA_FLAG_SHARE_WRITE = 0x00000020,  /**< 共享写模式，
    只有在此模式下，才可以以写方式同时打开同一个文件 */
	TEE_DATA_FLAG_CREATE = 0x00000200,  /**< 不论文件存在与否，
    均创建新文件，若存在则覆盖原有文件 */
	TEE_DATA_FLAG_EXCLUSIVE = 0x00000400,  /**< 如果文件不存在，
    则创建文件，否则报错 */
};

/**
 * @ingroup  TEEC_StorageAPI
 * @brief 打开安全存储服务
 *
 * @par 描述:
 * 打开安全存储服务成功，是对文件操作的前提
 *
 * @attention 无
 * @param 无
 *
 * @retval #TEEC_SUCCESS 打开安全存储服务成功
 * @retval 其它返回值参考 #TEEC_ReturnCode
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_storage_api.h：该接口声明所在头文件
 * @see TEEC_UninitStorageService
 * @since V100R002C00B302
 */
TEEC_Result TEEC_InitStorageService();

/**
 * @ingroup  TEEC_StorageAPI
 * @brief 关闭安全存储服务
 *
 * @par 描述:
 * 关闭安全存储服务
 *
 * @attention 无
 * @param 无
 *
 * @retval #TEEC_SUCCESS 关闭安全存储服务成功
 * @retval #TEEC_ERROR_BAD_STATE 安全存储服务从未打开
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_storage_api.h：该接口声明所在头文件
 * @see TEEC_InitStorageService
 * @since V100R002C00B302
 */
TEEC_Result TEEC_UninitStorageService();

/**
 * @ingroup  TEEC_StorageAPI
 * @brief 打开文件
 *
 * @par 描述:
 * 打开路径为path的文件，打开模式是mode，取值范围为#Data_Flag_Constants
 *
 * @attention 无
 * @param path [IN] 文件路径
 * @param mode [IN] 打开文件的方式，取值范围为#Data_Flag_Constants
 *
 * @retval -1 打开文件失败，没有初始化安全存储服务、或文件不存在、
 * 或并发访问文件时权限校验失败，可通过#TEEC_GetFErr获取错误码
 * @retval >=0 打开文件成功，返回文件描述符
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_storage_api.h：该接口声明所在头文件
 * @see TEEC_FClose
 * @since V100R002C00B302
 */
int32_t TEEC_FOpen(char *path, uint32_t mode);

/**
 * @ingroup  TEEC_StorageAPI
 * @brief 关闭文件
 *
 * @par 描述:
 * 关闭fd所指向的文件
 *
 * @attention 无
 * @param fd [IN] 文件描述符
 *
 * @retval -1 关闭文件失败，没有初始化安全存储服务、或参数fd无效，
 * 可通过#TEEC_GetFErr获取错误码
 * @retval 0 关闭文件成功
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_storage_api.h：该接口声明所在头文件
 * @see TEEC_FOpen
 * @since V100R002C00B302
 */
int32_t TEEC_FClose(int32_t fd);

/**
 * @ingroup  TEEC_StorageAPI
 * @brief 读取文件
 *
 * @par 描述:
 * 从参数fd所指的文件读取数据到buf中，最多读取size个字节
 *
 * @attention 无
 * @param fd [IN] 文件描述符
 * @param buf [IN] 文件读取内容缓冲区
 * @param size [IN] 需要读取的大小，以字节为单位，不能大于buf指向的缓冲区
 *
 * @retval 小于size 实际读取到的大小，读取文件时发生错误或遇到文件尾，
 * 可通过函数#TEEC_GetFErr获取错误码
 * @retval 等于size 读取成功
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_storage_api.h：该接口声明所在头文件
 * @see TEEC_FOpen | TEEC_FClose | TEEC_GetFErr
 * @since V100R002C00B302
 */
size_t TEEC_FRead(int32_t fd, uint8_t *buf, size_t size);

/**
 * @ingroup  TEEC_StorageAPI
 * @brief 写入文件
 *
 * @par 描述:
 * 向参数fd所指的文件写入数据buf，最多写入size个字节
 *
 * @attention 无
 * @param fd [IN] 文件描述符
 * @param buf [IN] 文件写入内容缓冲区
 * @param size [IN] 需要写入的大小，以字节为单位，不能大于buf指向的缓冲区
 *
 * @retval 小于size 实际写入的大小，写入文件时发生错误或遇到文件尾，
 * 可通过函数#TEEC_GetFErr获取错误码
 * @retval 等于size 写入成功
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_storage_api.h：该接口声明所在头文件
 * @see TEEC_FOpen | TEEC_FClose | TEEC_GetFErr
 * @since V100R002C00B302
 */
size_t TEEC_FWrite(int32_t fd, uint8_t *buf, size_t size);
/**
 * @ingroup  TEEC_StorageAPI
 * @brief 重定位文件
 *
 * @par 描述:
 * 重定位文件
 *
 * @attention 无
 * @param 无
 *
 * @retval #TEEC_SUCCESS 获取成功
 * @retval
 * @retval 其它返回值参考 #TEEC_ReturnCode
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_storage_api.h：该接口声明所在头文件
 * @see TEEC_FOpen
 * @since V100R003C00B061
 */
int32_t TEEC_FSeek(int32_t fd, int32_t offset, int32_t fromwhere);
/**
 * @ingroup  TEEC_StorageAPI
 * @brief 获取文件信息
 *
 * @par 描述:
 * 获取当前文件位置以及文件长度
 *
 * @attention 无
 * @param 无
 *
 * @retval #TEEC_SUCCESS 获取成功
 * @retval
 * @retval 其它返回值参考 #TEEC_ReturnCode
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_storage_api.h：该接口声明所在头文件
 * @see TEEC_FOpen
 * @since V100R003C00B061
 */
size_t TEEC_FInfo(int32_t fd, uint32_t *pos, uint32_t *len);
/**
 * @ingroup  TEEC_StorageAPI
 * @brief 关闭并删除文件
 *
 * @par 描述:
 * 关闭并删除对应文件
 *
 * @attention 无
 * @param 无
 *
 * @retval #TEEC_SUCCESS 删除成功
 * @retval
 * @retval 其它返回值参考 #TEEC_ReturnCode
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_storage_api.h：该接口声明所在头文件
 * @see TEEC_FOpen
 * @since V100R003C00B061
 */
int32_t TEEC_FCloseAndDelete(int32_t fd);

/**
 * @ingroup  TEEC_StorageAPI
 * @brief sync文件到flash
 *
 * @par 描述:
 * sync文件到flash
 *
 * @attention 无
 * @param 无
 *
 * @retval #TEEC_SUCCESS 删除成功
 * @retval
 * @retval 其它返回值参考 #TEEC_ReturnCode
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_storage_api.h：该接口声明所在头文件
 * @see TEEC_FOpen
 * @since V100R003C00B061
 */
int32_t TEEC_FSync(int32_t fd);

/**
 * @ingroup  TEEC_StorageAPI
 * @brief 获取错误码
 *
 * @par 描述:
 * 获取安全存储服务的错误码
 *
 * @attention 无
 * @param 无
 *
 * @retval #TEEC_SUCCESS 没有错误产生
 * @retval #TEEC_ERROR_BAD_STATE 安全存储服务从未打开
 * @retval 其它返回值参考 #TEEC_ReturnCode
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_storage_api.h：该接口声明所在头文件
 * @see TEEC_FRead | TEEC_FWrite
 * @since V100R002C00B302
 */
TEEC_Result TEEC_GetFErr();

#endif
/**
 * History: \n
 * 2013-4-12 h00211444: Create this file\n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
*/
