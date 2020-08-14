/*******************************************************************************
* All rights reserved, Copyright (C) huawei LIMITED 2012
*------------------------------------------------------------------------------
* File Name   : dx_infra_api.h
* Description :
* Platform    :
* Author      : qiqingchao
* Version     : V1.0
* Date        : 2013.5.17
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
* @file dx_infra_api.h
*
* Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
*
* 描述：DX功能API数据结构和接口定义\n
*/

/** @defgroup dx_infra_api DX安全侧接口
* @ingroup TEE_API
*/

#ifndef _DX_INFRA_API_
#define _DX_INFRA_API_

#include "tee_internal_api.h"
/**
 * @ingroup dx_infra_api
 * 无符号整型定义
 */
typedef unsigned int              DxUint32;
/**
 * @ingroup dx_infra_api
 * 无符号字符型定义
 */
typedef unsigned char             DxByte;
#if 1
//#ifndef CC_DRIVER
/**
 * @ingroup dx_infra_api
 * 返回结果定义
 */
typedef uint32_t DxBool;
#else
/**
 * @ingroup dx_infra_api
 * 状态布尔定义
 */
typedef DxBool_t DxBool;
#endif
/**
 * @ingroup dx_infra_api
 * 连接状态枚举定义
 */
typedef enum {
	CONNECTED = 0,              /**< 0 已连接 */
	UNCONNECTED                 /**< 1 未连接 */
} EDxTzConnectionStatus_t;
/**
 * @ingroup dx_infra_api
 * 安全内存标志位 0:仅安全访问;1:非安全和安全都能访问
 */
typedef enum {
	INIT = 0,                   /**< 0 设置内存为仅安全访问 */
	TERMINATE                   /**< 1 设置内存为安全和非安全都能访问 */
} CP_Flag;
/**
 * @ingroup dx_infra_api
 * 安全内存数据结构定义
 */
typedef struct {
	DxByte     *addr;           /**< 共享内存地址 */
	DxUint32    size;           /**< 共享内存长度 */
	CP_Flag     flag;           /**< 共享内存标识 */
} ContentPath;
/**
 * @ingroup dx_infra_api
 * 安全内存的数据和地址都需要32KB对齐
 */
#define ALIGN_SIZE 0x8000 //32KB
/**
 * @ingroup  dx_infra_api
 * @brief g_dx_content_path_addr 修改DX安全内存地址全局变量
 *
 * @par 描述:
 * DX安全内存通过全局变量管理地址，该接口会对全局变量进行修改
 *
 * @attention 全局变量澹(共享内存)只有一个，不支持并发操作
 * @param contentpath [IN]  安全内存数据结构 #ContentPath
 *
 * @retval #FALSE 全局变量赋值失败
 * @retval #TRUE 全局变量赋值成功
 *
 * @par 依赖:
 * @li dx_infra_api DX功能API
 * @li tee_internal_api.h TEE公共数据定义
 * @see DxTzService_IsSecureContentMemory
 * @since V100R002C00B302
*/
DxBool g_dx_content_path_addr(ContentPath *contentpath);
/**
 * @ingroup  dx_infra_api
 * @brief DxTzService_IsDeviceRooted 从安全侧检查设备是否被root
 *
 * @par 描述:
 * 从安全侧检查设备是否被root，如果被root则拥有root权限
 *
 * @attention 无
 * @param 无
 *
 * @retval 0 没有root
 * @retval 1 已经root
 *
 * @par 依赖:
 * @li dx_infra_api DX功能API
 * @li tee_internal_api.h TEE公共数据定义
 * @see 无
 * @since V100R002C00B302
*/
DxUint32  DxTzService_IsDeviceRooted(void);
/**
 * @ingroup  dx_infra_api
 * @brief DxTzService_IsSecureContentMemory 在安全侧检查一段内存是否为仅安全访问
 *
 * @par 描述:
 * 在安全侧检查一段内存是否为仅安全访问
 *
 * @attention 无
 * @param mem [IN] 内存地址 #DxByte
 * @param len [IN] 内存长度 #DxUint32
 *
 * @retval #TEE_SUCCESS 安全标志位设置成功
 * @retval #TEE_ERROR_BAD_PARAMETERS 参数错误
 *
 * @par 依赖:
 * @li dx_infra_api DX功能API
 * @li tee_internal_api.h TEE公共数据定义
 * @see DxTzService_IsSecureContentMemory
 * @since V100R002C00B302
*/
DxBool DxTzService_IsSecureContentMemory(DxByte *mem, DxUint32 len);
#endif
/**
* History: \n
* 2013-06-25，q00209673，Create \n
*
* vi: set expandtab ts=4 sw=4 tw=80:
*/
