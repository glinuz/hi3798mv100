/**
 * @file rpmb_fcntl.h
 *
 * Copyright(C), 2013-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：TEE(Trusted Execution Environment，可信执行环境)RPMB安全存储API \n
*/

/**@defgroup TEE_RPMB_API  RPMB安全存储
* @ingroup TEE_API
*/

#ifndef _RPMB_FCNTL_H
#define _RPMB_FCNTL_H

/**
 * @ingroup  TEE_RPMB_API
 * @brief RPMB安全存储初始化
 *
 * @par 描述:
 * 分区初始化，执行写RPMB Key和格式化操作
 *
 * @attention 该函数只需执行一次
 * @param 无
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_RPMB_GENERIC RPMB控制器通用错误
 * @retval #TEE_ERROR_RPMB_MAC_FAIL RPMB控制器MAC校验错误
 * @retval #TEE_ERROR_RPMB_RESP_UNEXPECT_MAC RPMB应答数据的MAC校验错误
 *
 * @par 依赖:
 * @li rpmb_fcntl.h：该接口声明所在的头文件。
 * @since TrustedCore V100R005C00
*/
TEE_Result TEE_RPMB_FS_Init(void);

/**
 * @ingroup  TEE_RPMB_API
 * @brief RPMB安全存储完全格式化
 *
 * @par 描述:
 * RPMB安全存储完全格式化操作
 *
 * @attention 无
 * @param 无
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_RPMB_GENERIC RPMB控制器通用错误
 * @retval #TEE_ERROR_RPMB_MAC_FAIL RPMB控制器MAC校验错误
 * @retval #TEE_ERROR_RPMB_RESP_UNEXPECT_MAC RPMB应答数据的MAC校验错误
 *
 * @par 依赖:
 * @li rpmb_fcntl.h：该接口声明所在的头文件。
 * @since TrustedCore V100R005C00
*/
TEE_Result TEE_RPMB_FS_Format(void);

/**
 * @ingroup  TEE_RPMB_API
 * @brief RPMB安全存储存储文件
 *
 * @par 描述:
 * RPMB安全存储存储文件
 *
 * @attention 如果要提升写文件的性能，
 * 需要在TA的manifest中定义堆大小至少是文件大小的3倍再加上256KB，
 * 比如: 要写入大小是100KB的文件，定义的堆大小至少是556KB(3*100+256)。
 * 如果堆大小无法满足，写文件仍然会成功、但性能较低。
 * @param filename [IN]  写入数据的文件名，最大长度为64字节
 * @param buf [IN]  写入数据的缓冲区
 * @param size [IN]  写入数据的大小，最大大小为160KB
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_BAD_PARAMETERS 输入参数有误，或文件名长度超过96字节
 * @retval #TEE_ERROR_RPMB_NOSPC RPMB分区磁盘空间不足
 *
 * @par 依赖:
 * @li rpmb_fcntl.h：该接口声明所在的头文件。
 * @since TrustedCore V100R005C00
*/
TEE_Result TEE_RPMB_FS_Write(const char *filename, uint8_t *buf, size_t size);

/**
 * @ingroup  TEE_RPMB_API
 * @brief RPMB安全存储读取文件
 *
 * @par 描述:
 * RPMB安全存储读取文件
 *
 * @attention 如果要提升读文件的性能，
 * 需要在TA的manifest中定义堆大小至少是文件大小的3倍再加上256KB，
 * 比如: 要读取大小是100KB的文件，定义的堆大小至少是556KB(3*100+256)。
 * 如果堆大小无法满足，读文件仍然会成功、但性能较低。
 * @param filename [IN]  读取数据的文件名，最大长度为64字节
 * @param buf [IN]  读取数据的缓冲区
 * @param size [IN]  读取数据的大小
 * @param count [OUT]  实际读取数据的大小
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_BAD_PARAMETERS 输入参数有误，或文件名长度超过96字节
 * @retval #TEE_ERROR_RPMB_FILE_NOT_FOUND 文件不存在
 *
 * @par 依赖:
 * @li rpmb_fcntl.h：该接口声明所在的头文件。
 * @since TrustedCore V100R005C00
*/
TEE_Result TEE_RPMB_FS_Read(const char *filename, uint8_t *buf, size_t size, uint32_t *count);

/**
 * @ingroup  TEE_RPMB_API
 * @brief RPMB安全存储重命名文件
 *
 * @par 描述:
 * RPMB安全存储重命名文件
 *
 * @attention 无
 * @param old_name [IN]  旧文件名
 * @param new_name [IN]  新文件名
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_BAD_PARAMETERS 输入参数有误，或文件名长度超过96字节
 * @retval #TEE_ERROR_RPMB_FILE_NOT_FOUND 旧文件不存在
 *
 * @par 依赖:
 * @li rpmb_fcntl.h：该接口声明所在的头文件。
 * @since TrustedCore V100R005C00
*/
TEE_Result TEE_RPMB_FS_Rename(const char *old_name, const char *new_name);

/**
 * @ingroup  TEE_RPMB_API
 * @brief RPMB安全存储删除文件
 *
 * @par 描述:
 * RPMB安全存储删除文件
 *
 * @attention 无
 * @param filename [IN]  待删除的文件名
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_BAD_PARAMETERS 输入参数有误，或文件名长度超过96字节
 * @retval #TEE_ERROR_RPMB_FILE_NOT_FOUND 文件不存在
 *
 * @par 依赖:
 * @li rpmb_fcntl.h：该接口声明所在的头文件。
 * @since TrustedCore V100R005C00
*/
TEE_Result TEE_RPMB_FS_Rm(const char *filename);

/**
 * @ingroup  TEE_RPMB_API
 *
 * 存储于RPMB分区的文件状态，用于#TEE_RPMB_FS_Stat函数 \n
*/
struct rpmb_fs_stat {
	uint32_t size;    /**< 文件大小  */
	uint32_t reserved;    /**< 预留  */
};
/**
 * @ingroup  TEE_RPMB_API
 * @brief RPMB安全存储获取文件状态
 *
 * @par 描述:
 * RPMB安全存储获取文件状态
 *
 * @attention 无
 * @param filename [IN]  文件名
 * @param stat [OUT]  获取的文件状态信息
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_BAD_PARAMETERS 输入参数有误，或文件名长度超过96字节
 * @retval #TEE_ERROR_RPMB_FILE_NOT_FOUND 文件不存在
 *
 * @par 依赖:
 * @li rpmb_fcntl.h：该接口声明所在的头文件。
 * @since TrustedCore V100R005C00
*/
TEE_Result TEE_RPMB_FS_Stat(const char *filename, struct rpmb_fs_stat *stat);

/**
 * @ingroup  TEE_RPMB_API
 *
 * 存储于RPMB分区的磁盘状态，用于#TEE_RPMB_FS_StatDisk函数 \n
*/
struct rpmb_fs_statdisk {
	uint32_t disk_size;    /**< RPMB分区总大小  */
	uint32_t ta_used_size;    /**< TA已经使用的大小  */
	uint32_t free_size;    /**< RPMB可用于分配文件的空间大小  */
	uint32_t reserved;    /**< 预留  */
};
/**
 * @ingroup  TEE_RPMB_API
 * @brief RPMB安全存储获取磁盘状态
 *
 * @par 描述:
 * RPMB安全存储获取磁盘状态
 *
 * @attention 无
 * @param stat [OUT]  获取的磁盘状态信息
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_BAD_PARAMETERS 输入参数有误，或文件名长度超过96字节
 *
 * @par 依赖:
 * @li rpmb_fcntl.h：该接口声明所在的头文件。
 * @since TrustedCore V100R006C00
*/
TEE_Result TEE_RPMB_FS_StatDisk(struct rpmb_fs_statdisk *stat);

/**
 * @ingroup TEE_RPMB_API
 * 文件属性定义，表示文件在恢复出厂设置阶段不可擦出
 */
#define TEE_RPMB_FMODE_NON_ERASURE (1<<0)
/**
 * @ingroup TEE_RPMB_API
 * 文件属性定义，表示清除文件的属性值
 */
#define TEE_RPMB_FMODE_CLEAR (0)
/**
 * @ingroup  TEE_RPMB_API
 * @brief RPMB安全存储设置文件属性
 *
 * @par 描述:
 * RPMB安全存储设置文件属性
 *
 * @attention 无
 * @param filename [IN]  文件名
 * @param fmode [IN]
 * 文件属性，当前支持#TEE_RPMB_FMODE_NON_ERASURE和#TEE_RPMB_FMODE_CLEAR两种属性，其它值会返回#TEE_ERROR_BAD_PARAMETERS
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_BAD_PARAMETERS 输入参数有误
 * @retval #TEE_ERROR_RPMB_FILE_NOT_FOUND 文件不存在
 *
 * @par 依赖:
 * @li rpmb_fcntl.h：该接口声明所在的头文件。
 * @since TrustedCore V100R006C00
*/
TEE_Result TEE_RPMB_FS_SetAttr(const char *filename, uint32_t fmode);

/**
 * @ingroup  TEE_RPMB_API
 * @brief RPMB安全存储格式化
 *
 * @par 描述:
 * RPMB安全存储格式化，删除文件属性是可擦除的文件，
 * 保留文件属性是不可擦除的文件
 *
 * @attention 无
 * @param filename [IN]  文件名
 * @param fmode [IN]  文件属性
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_RPMB_GENERIC RPMB控制器通用错误
 * @retval #TEE_ERROR_RPMB_MAC_FAIL RPMB控制器MAC校验错误
 * @retval #TEE_ERROR_RPMB_RESP_UNEXPECT_MAC RPMB应答数据的MAC校验错误
 *
 * @par 依赖:
 * @li rpmb_fcntl.h：该接口声明所在的头文件。
 * @since TrustedCore V100R006C00
*/
TEE_Result TEE_RPMB_FS_Erase(void);

/**
 * @ingroup  TEE_RPMB_API
 *
 * RPMB Key的状态，用于#TEE_RPMB_KEY_Status函数 \n
*/
enum TEE_RPMB_KEY_STAT {
	TEE_RPMB_KEY_INVALID = 0x0,
	TEE_RPMB_KEY_SUCCESS = 0x1,    /**< RPMB Key已写入且匹配正确  */
	TEE_RPMB_KEY_NOT_PROGRAM,    /**< RPMB Key未写入  */
	TEE_RPMB_KEY_NOT_MATCH,        /**< RPMB Key已写入但匹配失败  */
};
/**
 * @ingroup  TEE_RPMB_API
 * @brief RPMB安全存储获取RPMB Key状态
 *
 * @par 描述:
 * RPMB安全存储获取RPMB Key状态
 *
 * @attention 无
 * @param 无
 *
 * @retval #TEE_RPMB_KEY_SUCCESS RPMB Key已写入且匹配正确
 * @retval #TEE_RPMB_KEY_NOT_PROGRAM RPMB Key未写入
 * @retval #TEE_RPMB_KEY_NOT_MATCH RPMB Key已写入但匹配失败
 * @retval #TEE_RPMB_KEY_INVALID RPMB Key状态无效
 *
 * @par 依赖:
 * @li rpmb_fcntl.h：该接口声明所在的头文件。
 * @since TrustedCore V100R005C00
*/
uint32_t TEE_RPMB_KEY_Status(void);

#endif //_RPMB_FCNTL_H

/**
 * History: \n
 * 2014-12-18，h00211444，初始化该文件 \n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
*/

