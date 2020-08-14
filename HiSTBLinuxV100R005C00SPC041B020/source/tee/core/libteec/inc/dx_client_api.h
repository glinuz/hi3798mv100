/*******************************************************************************
* All rights reserved, Copyright (C) huawei LIMITED 2012
*------------------------------------------------------------------------------
* File Name   : dx_client_api.h
* Description :
* Platform    :
* Author      : qiqingchao
* Version     : V1.0
* Date        : 2013.5.29
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
 * @file dx_client_api.h
 *
 * Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：第三方DX content path客户端接口\n
 */

/**@defgroup dx_client_api 第三方DX客户端接口
 *@ingroup TEEC_API
*/

#ifndef _DX_CLIENT_API_H_
#define _DX_CLIENT_API_H_

#include "tee_client_type.h"
/**
 * @ingroup dx_client_api
 * 安全内存数据对齐长度定义
 */
#define ALIGN_SIZE 0x8000 //32KB
/**
 * @ingroup dx_client_api
 * 返回结果定义
 */
typedef TEEC_Result    DxStatus;
/**
 * @ingroup dx_client_api
 * DX客户端会话数据结构
 */
typedef struct {
	bool cp_init_flag;      /**< 设置安全内存的标志位 0:仅安全访问;1:安全和非安全都能访问*/
	TEEC_Session *session;  /**< 调用DX接口的会话备份 */
	uint32_t size;          /**< 分配buffer大小 */
	uint8_t *base_addr;     /**< 分配buffer基址 */
	uint8_t *virtual_addr;  /**< 分配buffer对齐地址 */
	TEEC_SharedMemory *sharedMem;   /**< 共享内存备份 */
} *Dx_Client_Tz_Session, Dx_Client_Tz_Session_val;
/**
 * @ingroup  dx_client_api
 * @brief DX客户端设置安全内存
 *
 * @par 描述:
 * DX客户端申请一块内存，并通知安全侧将其设置为仅安全访问
 *
 * @attention 无
 * @param pSession [IN] DX客户端会话数据结构 #Dx_Client_Tz_Session
 *
 * @retval #TEEC_SUCCESS 设置安全内存成功
 * @retval #TEEC_ERROR_BAD_PARAMETERS 输入参数错误
 * @retval #TEEC_ReturnCode 通用客户端API返回值
 *
 * @par 依赖:
 * @li DX客户端接口
 * @li tee_client_type.h 基本数据类型和数据结构
 * @see DxTzClient_TerminateSecureContentPath
 * @since V100R002C00B302
*/
DxStatus DxTzClient_InitSecureContentPath(Dx_Client_Tz_Session pSession);
/**
 * @ingroup  dx_client_api
 * @brief DX客户端解除安全内存
 *
 * @par 描述:
 * DX客户端解除安全内存的限制，并从非安全侧释放内存
 *
 * @attention 无
 * @param pSession [IN] DX客户端会话数据结构 #Dx_Client_Tz_Session
 *
 * @retval TEEC_SUCCESS 释放安全内存成功
 * @retval TEEC_ERROR_BAD_PARAMETERS 释放安全内存失败
 * @retval #TEEC_ReturnCode 通用客户端API返回值
 *
 * @par 依赖:
 * @li DX客户端接口
 * @li tee_client_type.h 基本数据类型和数据结构
 * @see DxTzClient_InitSecureContentPath
 * @since V100R002C00B302
*/
DxStatus DxTzClient_TerminateSecureContentPath(Dx_Client_Tz_Session pSession);

#endif
