/**
 * @file tee_client_random_api.h
 *
 * Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：定义随机数产生接口\n
 */

/** @defgroup TEEC_RandomAPI 随机数生成
 *@ingroup TEEC_API
 */

#ifndef _TEE_CLIENT_RANDOM_API_H_
#define _TEE_CLIENT_RANDOM_API_H_

/**
 * @ingroup  TEEC_RandomAPI
 * @brief 打开安全加解密服务
 *
 * @par 描述:
 * 打开安全加解密服务成功，是对产生随机数的前提
 *
 * @attention 无
 * @param 无
 *
 * @retval #TEEC_SUCCESS 打开安全加解密服务成功
 * @retval 其它返回值参考 #TEEC_ReturnCode
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_random_api.h：该接口声明所在头文件
 * @see TEEC_UninitCryptoService
 * @since V100R002C00B302
 */
TEEC_Result TEEC_InitCryptoService();

/**
 * @ingroup  TEEC_RandomAPI
 * @brief 关闭安全加解密服务
 *
 * @par 描述:
 * 关闭安全加解密服务
 *
 * @attention 无
 * @param 无
 *
 * @retval #TEEC_SUCCESS 关闭安全加解密服务成功
 * @retval #TEEC_ERROR_BAD_STATE 加解密服务从未打开
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_random_api.h：该接口声明所在头文件
 * @see TEEC_InitCryptoService
 * @since V100R002C00B302
 */
TEEC_Result TEEC_UninitCryptoService();

/**
 * @ingroup  TEEC_RandomAPI
 * @brief 随机数产生
 *
 * @par 描述:
 * 生成大小为outsize的随机数，如果随机数产生成功，输出参数output为随机数内容
 *
 * @attention 无
 * @param output [OUT] 随机数缓冲区
 * @param outsize [IN] 随机数长度，以字节为单位，最大值为63KBytes
 *
 * @retval #TEEC_SUCCESS 随机数产生成功
 * @retval #TEEC_ERROR_BAD_PARAMETERS 参数output为空或outsize为零
 * @retval #TEEC_ERROR_BAD_STATE 加解密服务未打开
 * @retval 其它返回值参考 #TEEC_ReturnCode
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_random_api.h：该接口声明所在头文件
 * @since V100R002C00B306
 */
TEEC_Result TEEC_Random(uint8_t *output, uint32_t outsize);

#endif
/**
 * History: \n
 * 2013-6-14 h00211444: Create this file\n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
*/
